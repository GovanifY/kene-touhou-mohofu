
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	オプションメニュー
	-------------------------------------------------------
	(r34)実装上の制限: このメニューでポーズをかけることはできません。
	(return_call_func変数を共用してるので出来ない。)
	-------------------------------------------------------
	多少コード効率は落ちるが、全体の構造をシンプルに改定。(r34)
	本当は前の奴(r33)の方が psp に向いて効率的な方式なんだけど、
	将来改変した場合にバグが出にくい事を優先する。
---------------------------------------------------------*/

enum
{
	OPTION_MENU_00_PLAYER = 0,
	OPTION_MENU_01_BOMB,
	OPTION_MENU_02_BGM,
	OPTION_MENU_03_SOUND,
//	OPTION_MENU_04_CR_DIFF,
//	OPTION_MENU_05_CR_PLAYER,
//	OPTION_MENU_06_ANALOG,
	// ------------- これより上は特別機能 ----------
	OPTION_MENU_07_RESET,
	OPTION_MENU_08_KEYCONFIG,
	OPTION_MENU_09_QUIT,
	OPTION_MENU_MAX,
};
//#define OPTION_MENU_CONFIG_MAX (OPTION_MENU_06_ANALOG+1)
#define OPTION_MENU_CONFIG_MAX (OPTION_MENU_07_RESET)

enum
{
	KEY_TAB_00_0 = 0,
	KEY_TAB_01_1,
	KEY_TAB_02_2,
	KEY_TAB_03_3,
	KEY_TAB_04_4,
	KEY_TAB_05_5,
//	KEY_TAB_06_6,
//	KEY_TAB_07_7,
//
	KEY_TAB_08_OFF,
	KEY_TAB_09_ON,
//	KEY_TAB_10_OFF,
//	KEY_TAB_11_OGG,
//	KEY_TAB_12_IT,
//
	KEY_TAB_17_MAX,
};

/*---------------------------------------------------------

---------------------------------------------------------*/

/*extern*/global int option_config[OPTION_CONFIG_08_MAX];						// 数字=ボタン変数

typedef struct
{
	u8 item;
	u8 chr_length;
	u8 offset;
	u8 item_default;/*[初期設定値(オプションファイルの読み込みに失敗した場合、や初期化した場合)]*/
} OPTION_STATUS_SETTING;
#define EXT_MAX (2)
static const OPTION_STATUS_SETTING option_status_setting[OPTION_MENU_CONFIG_MAX+EXT_MAX] =
{
	{5, (2<<4), KEY_TAB_01_1,		(2)}, // (2)==3機設定。 	OPTION_CONFIG_00_PLAYER = 0,
	{4, (2<<4), KEY_TAB_00_0,		(3)}, // (3)==3ボム設定。	OPTION_CONFIG_01_BOMB,
	{2, (4<<4), KEY_TAB_08_OFF, 	(1)}, // (1)==BGM 音出す。	OPTION_CONFIG_02_BGM,
	{2, (4<<4), KEY_TAB_08_OFF, 	(1)}, // (1)==効果音出す。	OPTION_CONFIG_03_SOUND,
//OPTION_MENU_CONFIG_MAX
	{4, (4<<4), KEY_TAB_00_0,		(1)}, // (1)==normal, (0)==easy OPTION_CONFIG_04_CURRENT_DIFFICULTY,
	{8, (1<<4), KEY_TAB_00_0,		(1)}, // (1)==霊夢(A)	OPTION_CONFIG_05_CURRENT_PLAYER,
//	{2, (4<<4), KEY_TAB_08_OFF, 	(1)}, // OPTION_CONFIG_06_ANALOG,
//	ゲーム進捗設定							OPTION_CONFIG_07_OPEN,
//											OPTION_CONFIG_08_MAX	/* 最大数 */
/*
	「ゲーム進捗設定(隠し要素)」はリセットしません。
	(練習できる面、選べるプレイヤー、エキストラステージ等)
*/
};


/*---------------------------------------------------------
	設定値をデフォルトに戻す。
---------------------------------------------------------*/

global /*static*/ void set_default_option(int *option_setting_map)
{
	unsigned int i;
	for (i=0; i<(OPTION_MENU_CONFIG_MAX); i++)
	{
		option_setting_map[i] = option_status_setting[i].item_default;
	}
}


/*---------------------------------------------------------
	設定値の範囲をチェックする。
---------------------------------------------------------*/

global /*static*/ void check_limit_value_option(int *option_setting_map)
{
	unsigned int i;
	for (i=0; i<(OPTION_MENU_CONFIG_MAX+EXT_MAX); i++)
	{
		if ((unsigned)option_status_setting[i].item <= (unsigned)option_setting_map[i])
		{
			option_setting_map[i] = option_status_setting[i].item_default;
		}
	}
}


/*---------------------------------------------------------
	変数 / 定数
---------------------------------------------------------*/

static int option_setting[OPTION_CONFIG_08_MAX/*OPTION_MENU_MAX*/]; 	/* オプション項目 */

#define MAX_KEY_NAMES_12	((KEY_TAB_17_MAX+1-1)+(OPTION_MENU_MAX))
static SDL_Surface *option_menu_name_surface[(MAX_KEY_NAMES_12)];		/* 文字の表示用画像 */

#define X_LOCATE_OFFSET_02	(8/*80*/)
#define X_LOCATE_OFFSET_04	(160/*250*/)
#define Y_LOCATE_OFFSET 	(128-16/*40*/)

static int menu_cursor1;
//
static int bg_alpha_aaa;
#define slide_offset_x	bg_alpha_aaa


/*---------------------------------------------------------
	描画
---------------------------------------------------------*/

static void option_menu_byouga(void)
{
	/* 描画 */
//	/*static*/ SDL_Rect rect_src_offset;		/* 位置 */
	/*static*/ SDL_Rect rect_locate_offset; 	/* 表示位置 */
	int i;
	u8 aaa_alpha;
	int jj;
	int kk;/* 縦の文字間隔 */
	/* 機能番号の描画 */
	kk = 0;/* 縦の文字間隔 */
	for (i=0; i</*12*/(OPTION_MENU_CONFIG_MAX); i++)
	{
		/* ボタン設定描画 */
		int i2;
		int ix3;
		ix3 = 0;
		int kinou_number;
		kinou_number = /*option_setting[i]+*/(option_status_setting[i].offset);
		for (i2=0; i2</*12*/(option_status_setting[i].item); i2++)
		{
			if (i2==option_setting[i])	/* 選択行 */
			{
				if ((menu_cursor1==i))
						{	jj = 3; 	aaa_alpha = (0xff); /* color32 = 0xffff3fbf; 橙不透明 */	}	/* 選択列 */
				else	{	jj = 2; 	aaa_alpha = (0xff); /* color32 = 0xff7f7f7f; 灰不透明 */	}
			}
			else	/* 非選択行 */
			{
				if ((menu_cursor1==i))
						{	jj = 0; 	aaa_alpha = (0xbf); /* color32 = 0x7fff3fbf; 橙半透明 */	}	/* 選択済み項目 */
				else	{	jj = 0; 	aaa_alpha = (0x7f); /* color32 = 0x7f7f7f7f; 灰半透明 */	}
			}
			rect_locate_offset.x = slide_offset_x + X_LOCATE_OFFSET_04-(jj)+ix3;
			ix3 += ((option_status_setting[i].chr_length));
			rect_locate_offset.y = (Y_LOCATE_OFFSET)+(/*num*/kk)-(jj);
			SDL_SetAlpha(	option_menu_name_surface[kinou_number+i2], SDL_SRCALPHA, aaa_alpha );
			PSPL_UpperBlit(option_menu_name_surface[kinou_number+i2], NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_locate_offset);
		}
		kk += (20);/* 縦の文字間隔 */
	}
	/* メニュー名称描画 */
	kk = 0;
	for (i=0; i<(OPTION_MENU_MAX); i++)
	{
		if (menu_cursor1==i)	{	jj= 2;	}
		else					{	jj= 0;	}
		rect_locate_offset.x = slide_offset_x + (X_LOCATE_OFFSET_02)-(jj);
		rect_locate_offset.y = (Y_LOCATE_OFFSET)+(/*num*/ /*gazo_iti_henkan[i]*/kk)-(jj);
		SDL_SetAlpha(	option_menu_name_surface[i+(KEY_TAB_17_MAX)], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
		PSPL_UpperBlit(option_menu_name_surface[i+(KEY_TAB_17_MAX)], NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_locate_offset);
		kk += (20);/* 縦の文字間隔 */
	}
}


/*---------------------------------------------------------
	おしまい
---------------------------------------------------------*/

static MAIN_CALL_FUNC(option_menu_terminate)
{
	#if 1/* 文字の表示用画像を開放する */
	{
		unsigned int i;
		for (i=0; i<(MAX_KEY_NAMES_12); i++)
		{
			/* ここでハングアップ */
			SDL_FreeSurface(option_menu_name_surface[i]);/*xxx_ok?*/
		}
	}
	#endif
	/* 指定した場所へ移動 */
	cb.main_call_func	= cb.menu_out_call_func;
}


/*---------------------------------------------------------
	選択中
---------------------------------------------------------*/

static MAIN_CALL_FUNC(option_menu_state_01_select_menu)
{
	/* メニュー選択 */
	{
		psp_pop_screen();
		if (0==psp_pad.pad_data_alter)/* さっき何も押されてなかった場合にキーチェック(原作準拠) */
		{
			if (OPTION_MENU_07_RESET > menu_cursor1)
			{
				/* あまり重要でないので上下音、無しにしてみた */
				if (psp_pad.pad_data & (/*PSP_KEY_UP|PSP_KEY_DOWN|*/PSP_KEY_LEFT|PSP_KEY_RIGHT))	// 上下左右ボタン入力 */
				{
					voice_play_menu(VOICE02_MENU_SELECT, TRACK01_MENU01);
				}
			}
		//
			if (psp_pad.pad_data & PSP_KEY_UP)				// 上ボタン入力 */
			{
				if (OPTION_MENU_00_PLAYER == menu_cursor1)		{	menu_cursor1 = (OPTION_MENU_MAX-1);}
				else											{	menu_cursor1--;}
			}
			else
			if (psp_pad.pad_data & PSP_KEY_DOWN)			// 下ボタン入力 */
			{
				if ((OPTION_MENU_MAX-1) == menu_cursor1)		{	menu_cursor1 = OPTION_MENU_00_PLAYER;}
				else											{	menu_cursor1++;}
			}
			else	/* 上下以外の通常処理(斜め入力を排除する為、上下の入力があった場合はそれ以外の処理をしない) */
			{
				if (OPTION_MENU_07_RESET == menu_cursor1)			// 項目[ RESET ] を選んでいる場合
				{
					if (psp_pad.pad_data & PSP_KEY_SHOT_OK) // ショットボタン入力 */
					{
						voice_play_menu(VOICE07_BOMB, TRACK01_MENU01);/*テキトー*/
						set_default_option(option_setting);
						menu_cursor1 = OPTION_MENU_09_QUIT;
					}
				}
				else
				if (OPTION_MENU_09_QUIT == menu_cursor1)	// 項目[ QUIT ] を選んでいる場合
				{
					if (psp_pad.pad_data & PSP_KEY_BOMB_CANCEL) 	// キャンセルボタン入力
					{
						voice_play_menu(VOICE04_SHIP_HAKAI, TRACK02_MENU02/*TRACK01_MENU01*/);/* 自機死に音は、なるべく重ねない */
						cb.menu_out_call_func	= title_menu_start; 	/* タイトルメニューへ移動指示 */
						cb.main_call_func		= option_menu_terminate;
					}
					else
					if (psp_pad.pad_data & PSP_KEY_SHOT_OK) 	// ショットボタン入力 */
					{
						// メニューを抜ける。 */
						{
							// 最終的に代入される物 */
							{
								int i;
								for (i=0; i<OPTION_CONFIG_08_MAX; i++)
								{
									//	システム(PSPのハードウェア)順 */
									option_config[i] = option_setting[i];
								}
							}
						//	ini_file_save(); */
							voice_play_menu(VOICE02_MENU_SELECT, TRACK01_MENU01);/*(トラック配置はテキトー)*/
						}
						cb.menu_out_call_func	= title_menu_start; 	/* タイトルメニューへ移動指示 */
						cb.main_call_func		= option_menu_terminate;
					}
				}
				else
				if (OPTION_MENU_08_KEYCONFIG == menu_cursor1)	/* 項目[ KEY CONFIG ] を選んでいる場合 */
				{
					if (psp_pad.pad_data & PSP_KEY_SHOT_OK) 	/* ショットボタン入力 */
					{
						voice_play_menu(VOICE07_BOMB, TRACK01_MENU01);/*(トラック配置はテキトー)*/
						cb.menu_out_call_func	= key_config_start; 	/* キーコンフィグメニューへ移動指示 */
						cb.main_call_func		= option_menu_terminate;
					}
				}
				else	/* 設定項目を変更する場合( 項目[ RESET ] 項目[ QUIT ]以外を選んでいる場合) */
				{
					if (psp_pad.pad_data & (PSP_KEY_LEFT|PSP_KEY_RIGHT))	/* 左右ボタン入力 */
					{
						int menu_cursor2;
						/* 選択した所に何が入っているのか調べる */
						menu_cursor2 = option_setting[menu_cursor1];
						//
						if (psp_pad.pad_data & PSP_KEY_LEFT)		/* 左ボタン入力 */
						{
							menu_cursor2--;
							if ((0/*KINOU_00_NONE*/) > menu_cursor2)	{	menu_cursor2 = (option_status_setting[menu_cursor1].item-1); }
						}
						else
						if (psp_pad.pad_data & PSP_KEY_RIGHT)		/* 右ボタン入力 */
						{
							menu_cursor2++;
							if ((option_status_setting[menu_cursor1].item-1) < menu_cursor2)	{	menu_cursor2 = 0/*KINOU_00_NONE*/; }
						}
						option_setting[menu_cursor1] = menu_cursor2;
						//
//						voice_play(VOICE02_MENU_SELECT, TRACK01_MENU01);/*テキトー*/
					}
				}
			}
		}
	}
	option_menu_byouga();/*(描画)*/
}


/*---------------------------------------------------------
	フェードイン中
---------------------------------------------------------*/

static MAIN_CALL_FUNC(option_menu_state_00_slide_in)
{
	{
	//	bg_alpha_aaa += /*1*/2/*6*/;/*fps_factor*/
		slide_offset_x -= (10);/*(8)*/		/*fps_factor*/
	//	if ((/*250-6*/224) < bg_alpha_aaa)
		if (slide_offset_x < 0)
		{
	//		bg_alpha_aaa = 255;
			slide_offset_x = 0;
//			psp_push_screen();
			cb.main_call_func = option_menu_state_01_select_menu;
		}
//		SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
		psp_pop_screen();
	}
	option_menu_byouga();/*(描画)*/
}


/*---------------------------------------------------------
	開始
---------------------------------------------------------*/

global MAIN_CALL_FUNC(option_menu_start)
{
	#if 0
	//	システム(PSPのハードウェア)順
	option_setting[ 0] = option_config[ 0];
	option_setting[ 1] = option_config[ 1];
	option_setting[ 2] = option_config[ 2];
	option_setting[ 3] = option_config[ 3];
	option_setting[ 4] = option_config[ 4];
	option_setting[ 5] = option_config[ 5];
	option_setting[ 6] = option_config[ 6];
	option_setting[ 7] = option_config[ 7];
	#else
	{
		int i;
		for (i=0; i<OPTION_CONFIG_08_MAX; i++)
		{
			option_setting[i] = option_config[i];
		}
	}
	#endif
	{
		static const char *const_key_str[(MAX_KEY_NAMES_12)] =
		{
			"0",
			"1",
			"2",
			"3",
			"4",
			"5",
//			"6",
//			"7",

			"Off",
			"On",
//			"Off",
//			"Ogg",
//			"IT",

			"Player",
			"Bomb",
			"BGM",
			"Sound",
//			"Current Diff",
//			"Current Player",
//			"Analog",

			"Reset",
			"Keyconfig",
			"Quit",
		};
		{
			unsigned int i;
			for (i=0; i<(MAX_KEY_NAMES_12); i++)
			{
				strcpy(my_font_text, (char *)const_key_str[i]);
				option_menu_name_surface[i] = font16_render_new_surface();
			}
		}
	}
	menu_cursor1				= OPTION_MENU_09_QUIT/*0*/;
	slide_offset_x/*bg_alpha_aaa*/				= (256);	/*(480) 0*/
	//
//	menu_out_call_func = title_menu_start;		/* タイトルメニューへ移動指示 */
	cb.main_call_func = option_menu_state_00_slide_in;
}
