
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	キーコンフィグ
---------------------------------------------------------*/
#include "111_my_file.h"/*(bg読みこみ。)*/

// 基本は絶対値指定(機能固定値)でsupport.cのkeypollの中のpad_configだけ
// 配列指定番号に変数（pad_config[key_xxx）を取る
// またkeypollに入る値は、0と1だけのままだとボタン設定上問題が出るのでビットで管理するようにした


enum	/*(キーコンフィグ設定メニューの表示行に名前を付ける。)*/
{	//	システム(PSPのハードウェア)順
	MENU_LINE_Y_00_SELECT = 0,	//	#define PSP_KEY_SELECT			PSP_CTRL_SELECT
	MENU_LINE_Y_01_START,		//	#define PSP_KEY_PAUSE			PSP_CTRL_START
//	KEY_CONFIG_MENU_02_UE,
//	KEY_CONFIG_MENU_03_MIGI,
//	KEY_CONFIG_MENU_04_SITA,
//	KEY_CONFIG_MENU_05_HIDARI,
//	MENU_LINE_Y_06_L = 0,
//	MENU_LINE_Y_07_R,
//	MENU_LINE_Y_08_SANKAKU,
//	MENU_LINE_Y_09_MARU,
//	MENU_LINE_Y_10_BATU,
//	MENU_LINE_Y_11_SIKAKU,

	MENU_LINE_Y_06_SNAP_SHOT,		//	#define PSP_KEY_SNAP_SHOT		PSP_CTRL_LTRIGGER
	MENU_LINE_Y_07_SYSTEM,			//	#define PSP_KEY_SYSTEM			PSP_CTRL_RTRIGGER
	MENU_LINE_Y_08_SLOW,			//	#define PSP_KEY_SLOW			PSP_CTRL_TRIANGLE
	MENU_LINE_Y_09_OPTION,			//	#define PSP_KEY_OPTION			PSP_CTRL_CIRCLE
	MENU_LINE_Y_10_SHOT,			//	#define PSP_KEY_SHOT_OK 		PSP_CTRL_CROSS
	MENU_LINE_Y_11_BOMB,			//	#define PSP_KEY_BOMB_CANCEL 	PSP_CTRL_SQUARE
	//	初期設定 / 終了
	MENU_LINE_Y_12_RESET,
	MENU_LINE_Y_13_QUIT,
	MENU_LINE_Y_14_MAX,
};

/*---------------------------------------------------------
	KINOU_07_SNAP_SHOT は ゲーム(即押し)にあまり関係ないので、
	一般的にクリティカルな動作では使いづらいと思われるＬボタンに移動。
	ストロークが短い分、即押ししやすいフラットボタンにslowを割り当てる。
	(Ｌ、Ｒ、ボタンはフラットボタンに比べてストロークが長い分、押し始めてから認識までに時間がかかる。)
	また携帯機ゆえに、Ｌ、Ｒ、ボタンは微妙に画面がぶれるので、
	slow切り替えを酷使する状況だとドット単位の精密避けが難しい。
---------------------------------------------------------*/
		//		PSP_KEY_UP, 			PSP_KEY_RIGHT,		PSP_KEY_DOWN,			PSP_KEY_LEFT, /* KINOU_12_BOMB KINOU_00_NONE*/

/*static*/global void set_default_key(u32 *key_setting_map, int key_setting_type)
{
	static const u32 default_key_map[4][KINOU_08_WARIATE_MAX] =
	{
		#if (0)
		/* 割り当ての場所の意味。メモ */
	//	{	PSP_KEY_SELECT, 		PSP_KEY_PAUSE,		PSP_CTRL_LTRIGGER,		PSP_CTRL_RTRIGGER,
	//		PSP_CTRL_TRIANGLE,		PSP_CTRL_CIRCLE,	PSP_CTRL_CROSS, 		PSP_CTRL_SQUARE,
	//	},
		#endif
		/* type 01 模倣風 標準 (スレ内の意見で決まった)。このタイプの欠点は △slowは無茶な位置なので余り使わなく、[R]slowは画面がぶれる。 */
		{	PSP_KEY_SELECT, 		PSP_KEY_PAUSE,		PSP_KEY_SNAP_SHOT,		PSP_KEY_SLOW,
			PSP_KEY_SLOW,			PSP_KEY_OPTION, 	PSP_KEY_SHOT_OK,		PSP_KEY_BOMB_CANCEL,
		},
		/* type 02 toho DX 互換 大雑把に言って type01 と似たような傾向。どっちが慣れて馴染むか人次第。好みの範囲。 */
		{	PSP_KEY_SELECT, 		PSP_KEY_PAUSE,		PSP_KEY_SNAP_SHOT,		PSP_KEY_SLOW,
			PSP_KEY_SLOW,			PSP_KEY_OPTION, 	PSP_KEY_BOMB_CANCEL,	PSP_KEY_SHOT_OK,
		},
		/* type 03 psp-1000 (□ボタンがカスなので使えない場合) □ボタンが死んでるから、×か○がショット(基本的に押しっぱなし)にならざる得ない。 */
		{	PSP_KEY_SELECT, 		PSP_KEY_PAUSE,		PSP_KEY_SNAP_SHOT,		PSP_KEY_SLOW,
			PSP_KEY_SLOW,			PSP_KEY_SHOT_OK,	PSP_KEY_BOMB_CANCEL,	PSP_KEY_OPTION,
		},
		/* type 04 psp-2000 (□ボタンがショットの場合)slowを酷使する場合は良いけど、他が若干使いにくい。 */
		{	PSP_KEY_SELECT, 		PSP_KEY_PAUSE,		PSP_KEY_SNAP_SHOT,		PSP_KEY_SLOW,
			PSP_KEY_BOMB_CANCEL,	PSP_KEY_OPTION, 	PSP_KEY_SLOW,			PSP_KEY_SHOT_OK,
		},
	};
	{
		unsigned int i;
		for (i=0; i<KINOU_08_WARIATE_MAX; i++)
		{
			key_setting_map[i] = default_key_map[key_setting_type][i];
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

#define MAX_BUTTON_12			(KINOU_08_WARIATE_MAX)
#define MAX_MENU_02 			(2)
#define MAX_DEFAULT_MAP_04		(4)

static u32 edit_pad_config[/*(KINOU_08_WARIATE_MAX)+(2)*/MENU_LINE_Y_14_MAX/*MENU_LINE_Y_14_MAX*/]; // 数字=ボタン変数

	/* DRAW_FONT_Y_ADJUST = (フォント高さ+縦の行間); */
//	#define DRAW_FONT_Y_ADJUST		(16)/*(-r34)*/
	#define DRAW_FONT_Y_ADJUST		(20)/*(r35-)*/			/* 縦に並べる間隔 */
	#define DRAW_FONT_X_ADJUST		(36)					/* 横に並べる間隔 */

//	#define DRAW_OFFSET_X_TYPE01	(64)/*(80-16)*/
/* メニュー縦位置(左上からドット単位で設定) */
	#define DRAW_OFFSET_Y			(56)/*(40)*/
/* メニュー横位置(左上からドット単位で設定) */
	#define DRAW_OFFSET_X_TYPE02	(20)/*(64)*/ /*(80)*/
/* リセットタイプ(左上からドット単位で設定) */
	#define DRAW_OFFSET_X_TYPE03	(180)/*(288)*/ /*(250-32)*/ /*(200)*/
/* ボタン機能の設定内容(左上からドット単位で設定) */
	#define DRAW_OFFSET_X_TYPE04	(180)/*(288)*/ /*(250)*/



//
	#define MAX_KEY_NAMES_33		(MENU_LINE_Y_14_MAX+MAX_BUTTON_12+MAX_MENU_02+MAX_DEFAULT_MAP_04)
static SDL_Surface *key_name_surface[MAX_KEY_NAMES_33]; 	/* 文字の表示用画像 */

static int menu_cursor1;
static int menu_cursor2;
static int key_setting_default_type;
//
static int bg_alpha_aaa;

/*---------------------------------------------------------

---------------------------------------------------------*/

static MAIN_CALL_FUNC(key_config_state_04_fade_out)
{
	bg_alpha_aaa += (2);	/*fps_factor*/		/*1*/		/*8*/
	if ((/*250-8*/224) < bg_alpha_aaa)
	{
		bg_alpha_aaa = (255);
		#if 0/* 文字の表示用画像を開放する */
		{
			unsigned int i;
			for (i=0; i<(MAX_KEY_NAMES_33-1); i++)
			{
				/* ここでハングアップ */
				SDL_FreeSurface(key_name_surface[i]);
			}
		}
		#endif
		cb.main_call_func = title_menu_start;	/* タイトルメニューへ移動 */
	}
	SDL_SetAlpha( cb.sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
	psp_pop_screen();
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static MAIN_CALL_FUNC(key_config_state_03_fade_init)
{
//	pl ay_music_num(BGM_27_menu01);
	my_file_common_name[0] = BG_TYPE_00_title_bg;psp_load_bg_file_name();
	bg_alpha_aaa		= (0);
	cb.main_call_func = key_config_state_04_fade_out;
}


/*---------------------------------------------------------

---------------------------------------------------------*/
static const u32 aaa_table[KINOU_08_WARIATE_MAX] = /* 機能ビットを調べるテーブル。 */
{
	PSP_CTRL_SELECT,
	PSP_CTRL_START,
	//
	PSP_CTRL_LTRIGGER,	//2 KEY_NUM06_L_TRIG = 0,	/* L */
	PSP_CTRL_RTRIGGER,	//3 KEY_NUM07_R_TRIG,		/* R */
	PSP_CTRL_TRIANGLE,	//4 KEY_NUM08_TRIANGLE, 	/* △ */
	PSP_CTRL_CIRCLE,	//5 KEY_NUM09_CIRCLE,		/* ○ */
	PSP_CTRL_CROSS, 	//6 KEY_NUM10_CROSS,		/* × */
	PSP_CTRL_SQUARE,	//7 KEY_NUM11_SQUARE,		/* □ */
};

extern void ini_file_save(void);
static MAIN_CALL_FUNC(key_config_state_02_select_left_menu)
{
	psp_pop_screen();
	if (0==psp_pad.pad_data_alter)/* さっき何も押されてなかった場合にキーチェック(原作準拠) */
	{
		if (psp_pad.pad_data & PSP_KEY_UP)				// 上ボタン入力
		{
			if (MENU_LINE_Y_00_SELECT == menu_cursor1)		{	menu_cursor1 = (MENU_LINE_Y_14_MAX-1);}
			else											{	menu_cursor1--;}
		}
		else
		if (psp_pad.pad_data & PSP_KEY_DOWN)			// 下ボタン入力
		{
			if ((MENU_LINE_Y_14_MAX-1) == menu_cursor1) 	{	menu_cursor1 = MENU_LINE_Y_00_SELECT ;}
			else											{	menu_cursor1++;}
		}
		else	/* 上下以外の通常処理(斜め入力を排除する為、上下の入力があった場合はそれ以外の処理をしない) */
		{
			if (MENU_LINE_Y_12_RESET == menu_cursor1)			// 項目[ RESET ] を選んでいる場合
			{
				if (psp_pad.pad_data & PSP_KEY_LEFT)	// 左ボタン入力
				{
					key_setting_default_type--;
					key_setting_default_type &= (4-1);
					voice_play_menu(VOICE02_MENU_SELECT, TRACK01_MENU01);/*(トラック配置はテキトー)*/
				}
				else
				if (psp_pad.pad_data & PSP_KEY_RIGHT) // 右ボタン入力
				{
					key_setting_default_type++;
					key_setting_default_type &= (4-1);
					voice_play_menu(VOICE02_MENU_SELECT, TRACK01_MENU01);/*(トラック配置はテキトー)*/
				}
				else
				if (psp_pad.pad_data & PSP_KEY_SHOT_OK) // ショットボタン入力
				{
					voice_play_menu(VOICE07_BOMB, TRACK01_MENU01);/*(トラック配置はテキトー)*/
					set_default_key(edit_pad_config, key_setting_default_type);
					menu_cursor1 = MENU_LINE_Y_13_QUIT;
				}
			}
			else
			if (MENU_LINE_Y_13_QUIT == menu_cursor1)	// 項目[ QUIT ] を選んでいる場合
			{
				if (psp_pad.pad_data & PSP_KEY_BOMB_CANCEL) // キャンセルボタン入力
				{
					voice_play_menu(VOICE04_SHIP_HAKAI, TRACK02_MENU02/*TRACK01_MENU01*/);/* 自機死に音は、なるべく重ねない */
					cb.main_call_func = key_config_state_03_fade_init;
				}
				else
				if (psp_pad.pad_data & PSP_KEY_SHOT_OK) // ショットボタン入力
				{
					int saiteigen_exsist;	// 存在確認用。0で存在しない。存在しないとメニューを抜けれない。
					saiteigen_exsist = (0);
					{
						unsigned int i;
						for (i=0; i<KINOU_08_WARIATE_MAX/*KEY_CONFIG_MENU_14_MAX*/; i++)
						{
							saiteigen_exsist	|= edit_pad_config[i];
						}
					}
					if (0!=(saiteigen_exsist&(PSP_KEY_SHOT_OK/*|PSP_KEY_DOWN|PSP_KEY_UP*/)))	// ショットボタン&↑&↓があるか確認
					{
						cb.main_call_func = key_config_state_03_fade_init;
						// 最終的に代入される物
						#if 0
						//	システム(PSPのハードウェア)順
						pad_config[KEY_NUM00_SELECT]	= edit_pad_config[NNN_00];
						pad_config[KEY_NUM01_START] 	= edit_pad_config[NNN_01];
					//	pad_config[KEY_NUM02_UP]		= edit_pad_config[NNN_02];
					//	pad_config[KEY_NUM03_RIGHT] 	= edit_pad_config[NNN_03];
					//	pad_config[KEY_NUM04_DOWN]		= edit_pad_config[NNN_04];
					//	pad_config[KEY_NUM05_LEFT]		= edit_pad_config[NNN_05];
						pad_config[KEY_NUM06_L_TRIG]	= edit_pad_config[NNN_06];
						pad_config[KEY_NUM07_R_TRIG]	= edit_pad_config[NNN_07];
						pad_config[KEY_NUM08_TRIANGLE]	= edit_pad_config[NNN_08];
						pad_config[KEY_NUM09_CIRCLE]	= edit_pad_config[NNN_09];
						pad_config[KEY_NUM10_CROSS] 	= edit_pad_config[NNN_10];
						pad_config[KEY_NUM11_SQUARE]	= edit_pad_config[NNN_11];
						#else
						{
							unsigned int i;
							for (i=0; i<KINOU_08_WARIATE_MAX; i++)
							{
								pad_config[i] = edit_pad_config[i];
							}
						}
						#endif
						ini_file_save();
						voice_play_menu(VOICE02_MENU_SELECT, TRACK01_MENU01);/*(トラック配置はテキトー)*/
					}
					else
					{
					//	"dame dayo"
						voice_play_menu(VOICE09_GRAZE, TRACK01_MENU01);/*(トラック配置はテキトー)*/
					}
				}
			}
			else	/* 設定項目を変更する場合( 項目[ RESET ] 項目[ QUIT ]以外を選んでいる場合) */
			{
				if (psp_pad.pad_data & (PSP_KEY_LEFT|PSP_KEY_RIGHT))		// 左右ボタン入力
				{
					if (psp_pad.pad_data & PSP_KEY_LEFT)		// 左ボタン入力
					{
						menu_cursor2--;
					}
					else
					if (psp_pad.pad_data & PSP_KEY_RIGHT)		// 右ボタン入力
					{
						menu_cursor2++;
					}
					menu_cursor2 = psp_min(menu_cursor2, 7);
					menu_cursor2 = psp_max(menu_cursor2, 0);
					//
					voice_play_menu(VOICE02_MENU_SELECT, TRACK01_MENU01);/*(トラック配置はテキトー)*/
				}
				if (psp_pad.pad_data & (PSP_KEY_SHOT_OK))		// ショットボタン入力
				{
					edit_pad_config[menu_cursor2] ^= aaa_table[(menu_cursor1)];
					voice_play_menu(VOICE07_BOMB, TRACK01_MENU01);/*(トラック配置はテキトー)*/
				}
			}
		}
	}
	/* 描画 */
	{
	//	/*static*/ SDL_Rect rect_src_offset;		/* 位置 */
		/*static*/ SDL_Rect rect_locate_offset; 	/* 表示位置 */
		int ii;
		int jj;
		{
			/* ボタン機能の描画 */
			u32 uu;
			uu = 0;
			for (ii=0; ii<KINOU_08_WARIATE_MAX; ii++)
			{
				int position_x;
				position_x = 0;
				int kk;
				for (kk=0; kk</*12*/(MENU_LINE_Y_12_RESET/*12*/)/*KEY_CONFIG_MENU_14_MAX*/; kk++)
				{
					if (menu_cursor2==kk)	{	jj= 2;	}
					else					{	jj= 0;	}
					rect_locate_offset.x	= DRAW_OFFSET_X_TYPE04 + (position_x) - (jj);
					rect_locate_offset.y	= (DRAW_OFFSET_Y) + (uu) - (jj);
					int kinou_number;
					if (edit_pad_config[kk] & aaa_table[(ii)] )
							{	kinou_number	= (kk+1);	}
					else	{	kinou_number	= (0);		}
					u8 u8_alpha;
					u8_alpha = (127);
					if (0 != jj)
					{
						if (ii==menu_cursor1)
						{
							u8_alpha = (127+(2/*jj*/<<6));
						}
					}
					SDL_SetAlpha(	key_name_surface[(kinou_number)], SDL_SRCALPHA, (u8_alpha) );
					PSPL_UpperBlit(key_name_surface[(kinou_number)], NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_locate_offset);
				//	rect_src_offset.x = 0;
				//	rect_src_offset.y = (kinou_number<<4);
				//	rect_src_offset.w = (16*9);
				//	rect_src_offset.h = (16);
				//	PSPL_UpperBlit(sdl_screen[SDL_01_BACK_SCREEN], &rect_src_offset, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_locate_offset);
					position_x += DRAW_FONT_X_ADJUST;
				}
				uu += (DRAW_FONT_Y_ADJUST);
			}
		}
		/*
			ボタン機能はとりあえず縦に描画している。
			横の描画の方が都合良いかもしれない。というか多分都合が良いので後で変えよう。
			(フォントのシステムが、ライン毎独立に変わる予定なので。横は一気に書けるが縦は大変。)
		*/
		#define DRAW_01_HAIFUN				(1) 					/* "-"のラベル分 */
		#define DRAW_08_KINOU				(KINOU_08_WARIATE_MAX)	/* 機能を示すボタンのラベル */
		#define DRAW_08_SETUMEI_LABEL		(8) 					/* 機能の説明用ラベル */
		#define DRAW_02_EXIT_LABEL			(2) 					/* RESET / QUITの２つのラベル */
		#define DRAW_LABEL_OFFSET_Y_KINOU	(DRAW_01_HAIFUN + DRAW_08_KINOU)	/*MAX_KINOU_11*/
		#define DRAW_LABEL_OFFSET_Y_TYPE	(DRAW_01_HAIFUN + DRAW_08_KINOU + DRAW_08_SETUMEI_LABEL + DRAW_02_EXIT_LABEL)/*(MAX_KINOU_11+MAX_BUTTON_12+MAX_MENU_02)*/
		/* リセットタイプの描画 */
		{
			if (menu_cursor1==(MENU_LINE_Y_12_RESET/*12*/)) 	{	jj= 2;	}
			else						{	jj= 0;	}
			rect_locate_offset.x = DRAW_OFFSET_X_TYPE03-(jj);
			rect_locate_offset.y = (DRAW_OFFSET_Y) + (/*num*/(MENU_LINE_Y_12_RESET*DRAW_FONT_Y_ADJUST))-(jj);
			SDL_SetAlpha(	key_name_surface[DRAW_LABEL_OFFSET_Y_TYPE + key_setting_default_type], SDL_SRCALPHA, (127+(jj<<6)));
			PSPL_UpperBlit(key_name_surface[DRAW_LABEL_OFFSET_Y_TYPE + key_setting_default_type], NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_locate_offset);
		}
		/* ボタン機能名称描画 / RESET描画 / QUIT描画 */
		{	int position_y;
			position_y = 0;
			for (ii=0; ii</*(MAX_BUTTON_12+MAX_MENU_02)*/(MENU_LINE_Y_14_MAX)/*MENU_LINE_Y_14_MAX*/; ii++)
			{
				if (menu_cursor1==ii)	{	jj = 2; 	}
				else					{	jj = 0; 	}
				rect_locate_offset.x = (DRAW_OFFSET_X_TYPE02) - (jj);
				rect_locate_offset.y = (DRAW_OFFSET_Y) + (/*num*/ /*gazo_iti_henkan[i]*/position_y)-(jj);
				SDL_SetAlpha(	key_name_surface[DRAW_LABEL_OFFSET_Y_KINOU + (ii)], SDL_SRCALPHA, (127+(jj<<6)));
				PSPL_UpperBlit(key_name_surface[DRAW_LABEL_OFFSET_Y_KINOU + (ii)], NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_locate_offset);
				position_y += DRAW_FONT_Y_ADJUST;
			}
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static MAIN_CALL_FUNC(key_config_state_01_fade_in)
{
	bg_alpha_aaa += (2);	/*fps_factor*/	/*1 6*/
	if ((/*250-6*/224) < bg_alpha_aaa)
	{
		bg_alpha_aaa = (255);
		psp_push_screen();
		cb.main_call_func = key_config_state_02_select_left_menu;
	}
	SDL_SetAlpha(cb.sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
	psp_pop_screen();
}


/*---------------------------------------------------------

---------------------------------------------------------*/
//enum /*FONTS*/
//{
//	F_16K,			/* 16x16 x キーコンフィグ用 */
//	F_16W,			/* 16x16 x WHITE 白 */
//};
global MAIN_CALL_FUNC(key_config_start)
{
	play_music_num(BGM_26_menu02);
	my_file_common_name[0] = BG_TYPE_02_key_config;psp_load_bg_file_name();
	#if 0
	//	システム(PSPのハードウェア)順
	edit_pad_config[NNN_00] = pad_config[KEY_NUM00_SELECT];
	edit_pad_config[NNN_01] = pad_config[KEY_NUM01_START];
//	edit_pad_config[NNN_02] = pad_config[KEY_NUM02_UP];
//	edit_pad_config[NNN_03] = pad_config[KEY_NUM03_RIGHT];
//	edit_pad_config[NNN_04] = pad_config[KEY_NUM04_DOWN];
//	edit_pad_config[NNN_05] = pad_config[KEY_NUM05_LEFT];
	edit_pad_config[NNN_06] = pad_config[KEY_NUM06_L_TRIG];
	edit_pad_config[NNN_07] = pad_config[KEY_NUM07_R_TRIG];
	edit_pad_config[NNN_08] = pad_config[KEY_NUM08_TRIANGLE];
	edit_pad_config[NNN_09] = pad_config[KEY_NUM09_CIRCLE];
	edit_pad_config[NNN_10] = pad_config[KEY_NUM10_CROSS];
	edit_pad_config[NNN_11] = pad_config[KEY_NUM11_SQUARE];
	#else
	{
		unsigned int i;
		for (i=0; i<KINOU_08_WARIATE_MAX; i++)
		{
			edit_pad_config[i] = pad_config[i];
		}
	}
	#endif
	{

//typedef struct
//{
//	u32 font_type;
//	const char *key_str;
//} FONT_AAA;
		static const /*FONT_AAA*/char *aaa[(MAX_KEY_NAMES_33)] =
		{
			// [[ 選択する並び順序 ]]
			// 機能順序
			"-",		// "--" "None"
			"\xe2",//"b",		// "Se" "Select"
			"\xe3",//"c",		// "St" "Start"
		//	"Up",		// 無し(アナログ対応の為、ややこしい。ゲーム速度が低下する)
		//	"Right",	// 無し(アナログ対応の為、ややこしい。ゲーム速度が低下する)
		//	"Down", 	// 無し(アナログ対応の為、ややこしい。ゲーム速度が低下する)
		//	"Left", 	// 無し(アナログ対応の為、ややこしい。ゲーム速度が低下する)
			"\xe8",//"h",		//"L "	 "SS"	 "Snap Shot",			//0 KEY_NUM06_L_TRIG = 0,	/* L */
			"\xe9",//"i",		//"R "	 "ZZ"	 "System",				//1 KEY_NUM07_R_TRIG,		/* R */
			"\xec",//"l",		//"A "	 "SL"	 "Slow",				//2 KEY_NUM08_TRIANGLE, 	/* △ */	"Sankaku",
			"\xed",//"m",		//"O "	 "FM"	 "Option",				//3 KEY_NUM09_CIRCLE,		/* ○ */	"Maru",
			"\xee",//"n",		//"X "	 "OK"	 "Shot / OK",			//4 KEY_NUM10_CROSS,		/* × */	"Batu",
			"\xef",//"o",		//"H "	 "NG"	 "Bomb / Cancel",		//5 KEY_NUM11_SQUARE,		/* □ */	"Sikaku",
			#if 1
			// システム(PSPのハードウェア)順
			"Select",	// "Select" 	/*"ショートカット機能"*/
			"Pause",	// "Start"		/*"一時停止機能"*/
		//	"Up",
		//	"Right",
		//	"Down",
		//	"Left",
			"Snap Shot",	//	"L" 			"Snap Shot" 	"System"		/*"画面保存機能"*/
			"System",		//	"R" 			"System"		"Slow"			/*"未使用"*/
			"Slow", 		//	"Sankaku"		"Slow"			"Snap Shot" 	/*"低速"*/
			"Option",		//	"Maru"			"Option"						/*"再配置"*/
			"Shot / OK",	//	"Batu"			"Shot / OK" 					/*"ショット / OK"*/
			"Bomb / NG",	//	"Sikaku"		"Bomb / Cancel" 				/*"ボム / キャンセル"*/
		//	"Teisoku Bomb",
			#endif
			// 初期設定 / 終了
			"Reset",		// "リセット"
			"Quit", 		// "終了"
			// 初期設定タイプの選択
			"Type 01",
			"Type 02",
			"Type 03",
			"Type 04",
			NULL,
		};
		{
			unsigned int i;
			i = 0;
			while (NULL != aaa[i]/*.key_str*/)
			{
				#if 0
				if (F_16K==aaa[i].font_type)
				{
					my_font_text[0] = ((aaa[i].key_str[0]) ^ 0x80);
				//	my_font_text[0] = (('b') ^ 0x80);
					my_font_text[1] = 0;
				}
				else
				#endif
				{
					strcpy(my_font_text, (char *)aaa[i]/*.key_str*/);
				}
				key_name_surface[i] = font16_render_new_surface();
				i++;
			}
		}
	}
	menu_cursor1				= MENU_LINE_Y_13_QUIT;/*0*/
	key_setting_default_type	= (0);
	bg_alpha_aaa				= (0);
	cb.main_call_func = key_config_state_01_fade_in;
}
