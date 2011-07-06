
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	キーコンフィグ
---------------------------------------------------------*/

//	[***20080110-20080115
//	[***20090110
//キーコンフィグ追加。
//基本は絶対値指定(機能固定値)でsupport.cのkeypollの中のpad_configだけ
//配列指定番号に変数（pad_config[key_xxx）を取る
//またkeypollに入る値は、0と1だけのままだとボタン設定上問題が出るのでビットで管理するようにした

enum
{	//	システム(PSPのハードウェア)順
	KEY_CONFIG_MENU_00_SELECT = 0,
	KEY_CONFIG_MENU_01_START,
	KEY_CONFIG_MENU_02_UE,
	KEY_CONFIG_MENU_03_MIGI,
	KEY_CONFIG_MENU_04_SITA,
	KEY_CONFIG_MENU_05_HIDARI,
	KEY_CONFIG_MENU_06_L,
	KEY_CONFIG_MENU_07_R,
	KEY_CONFIG_MENU_08_SANKAKU,
	KEY_CONFIG_MENU_09_MARU,
	KEY_CONFIG_MENU_10_BATU,
	KEY_CONFIG_MENU_11_SIKAKU,
	//	初期設定 / 終了
	KEY_CONFIG_MENU_12_RESET,
	KEY_CONFIG_MENU_13_QUIT,
	KEY_CONFIG_MENU_MAX,
};

/*---------------------------------------------------------
	KINOU_07_SNAP_SHOT は ゲーム(即押し)にあまり関係ないので、
	一般的にクリティカルな動作では使いづらいと思われるＬボタンに移動。
	ストロークが短い分、即押ししやすいフラットボタンにslowを割り当てる。
	(Ｌ、Ｒ、ボタンはフラットボタンに比べてストロークが長い分、押し始めてから認識までに時間がかかる。)
	また携帯機ゆえに、Ｌ、Ｒ、ボタンは微妙に画面がぶれるので、
	slow切り替えを酷使する状況だとドット単位の精密避けが難しい。
---------------------------------------------------------*/

/*static*/global void set_default_key(int *key_setting_map, int key_setting_type)
{
	static const int default_key_map[4][KEY_NUM12_MAX] =
	{
		{	/* type 01 模倣風 標準 (スレ内の意見で決まった) */
			PSP_KEY_SELECT, 		PSP_KEY_PAUSE,		PSP_KEY_UP, 			PSP_KEY_RIGHT, /* KINOU_12_BOMB KINOU_00_NONE*/
			PSP_KEY_DOWN,			PSP_KEY_LEFT,		PSP_KEY_SNAP_SHOT,		PSP_KEY_SLOW,
			PSP_KEY_SLOW,			PSP_KEY_OPTION, 	PSP_KEY_SHOT_OK,		PSP_KEY_BOMB_CANCEL,
		},
		{	/* type 02 toho DX 互換 */
			PSP_KEY_SELECT, 		PSP_KEY_PAUSE,		PSP_KEY_UP, 			PSP_KEY_RIGHT, /* KINOU_12_BOMB KINOU_00_NONE*/
			PSP_KEY_DOWN,			PSP_KEY_LEFT,		PSP_KEY_SNAP_SHOT,		PSP_KEY_SLOW,
			PSP_KEY_SLOW,			PSP_KEY_OPTION, 	PSP_KEY_BOMB_CANCEL,	PSP_KEY_SHOT_OK,
		},
		{	/* type 03 psp-1000 (□ボタンがカスなので使えない場合) */
			PSP_KEY_SELECT, 		PSP_KEY_PAUSE,		PSP_KEY_UP, 			PSP_KEY_RIGHT, /* KINOU_12_BOMB KINOU_00_NONE*/
			PSP_KEY_DOWN,			PSP_KEY_LEFT,		PSP_KEY_SNAP_SHOT,		PSP_KEY_SLOW,
			PSP_KEY_SLOW,			PSP_KEY_SHOT_OK,	PSP_KEY_BOMB_CANCEL,	PSP_KEY_OPTION,
		},
		{	/* type 04 psp-2000 (□ボタンがショットの場合) */
			PSP_KEY_SELECT, 		PSP_KEY_PAUSE,		PSP_KEY_UP, 			PSP_KEY_RIGHT, /* KINOU_12_BOMB KINOU_00_NONE*/
			PSP_KEY_DOWN,			PSP_KEY_LEFT,		PSP_KEY_SNAP_SHOT,		PSP_KEY_SLOW,
			PSP_KEY_BOMB_CANCEL,	PSP_KEY_OPTION, 	PSP_KEY_SLOW,			PSP_KEY_SHOT_OK,
		},
	};
	{
		int i;
		for (i=0; i<12; i++)
		{
			key_setting_map[i] = default_key_map[key_setting_type][i];
		}
	}
}

enum /*_keynum_*/		//キーコンフィグ用
{
	KINOU_00_NONE = 0,
	KINOU_01_SELECT,
	KINOU_02_PAUSE,
	KINOU_03_UP,
	KINOU_04_RIGHT,
	KINOU_05_DOWN,
	KINOU_06_LEFT,
	KINOU_07_SNAP_SHOT,
	KINOU_08_SYSTEM,
	KINOU_09_SLOW,
	KINOU_10_OPTION,
	KINOU_11_SHOT,
	KINOU_12_BOMB,
	KINOU_13_MAX	/* キーコンフィグ用の最大数 */
};

extern void ini_save(void); 	// [***090115

enum
{
	KEY_CONFIG_STATE_00_INIT = 0,
	KEY_CONFIG_STATE_01_FADE_IN,
	KEY_CONFIG_STATE_02_SELECT_LEFT_MENU,
	KEY_CONFIG_STATE_06_FADE_INIT,
	KEY_CONFIG_STATE_07_FADE_OUT,
};

static u8 my_ppp_loop;
static void key_config_local_work(void)
{
#define MAX_KINOU_11			(12+1)
#define MAX_BUTTON_12			(12)
#define MAX_MENU_02 			(2)
#define MAX_DEFAULT_MAP_04		(4)

	static int key_setting[KEY_CONFIG_MENU_MAX];						// 数字=ボタン変数
	static const int const_pad_setting[KEY_CONFIG_MENU_MAX/*11*/] = 	// 数字=機能定数
	{	// 数字 = 機能定数 並び。	[[ 選択する並び順序 ]]
		PSP_KEY_NONE,			//	KINOU_00_NONE,
		PSP_KEY_SELECT, 		//	KINOU_01_SELECT,
		PSP_KEY_PAUSE,			//	KINOU_02_PAUSE,
		PSP_KEY_UP, 			//	KINOU_03_UP,
		PSP_KEY_RIGHT,			//	KINOU_04_RIGHT,
		PSP_KEY_DOWN,			//	KINOU_05_DOWN,
		PSP_KEY_LEFT,			//	KINOU_06_LEFT,
		PSP_KEY_SYSTEM, 		//	KINOU_07_SNAP_SHOT,
		PSP_KEY_SLOW,			//	KINOU_08_SYSTEM,
		PSP_KEY_SNAP_SHOT,		//	KINOU_09_SLOW,
		PSP_KEY_OPTION, 		//	KINOU_10_OPTION,
		PSP_KEY_SHOT_OK,		//	KINOU_11_SHOT,
		PSP_KEY_BOMB_CANCEL,	//	KINOU_12_BOMB,
		0						//	0
	};

	#define X_LOCATE_OFFSET_01	(64)/*(80-16)*/
	#define X_LOCATE_OFFSET_02	(80)
	#define X_LOCATE_OFFSET_03	(200)
	#define X_LOCATE_OFFSET_04	(250)
	#define Y_LOCATE_OFFSET 	(40)
	#define MAX_KEY_NAMES_12	(KEY_CONFIG_MENU_MAX+MAX_BUTTON_12+MAX_MENU_02+MAX_DEFAULT_MAP_04)
	static SDL_Surface *key_name_surface[MAX_KEY_NAMES_12]; 	/* 文字の表示用画像 */

	static int menu_cursor1;
	static int key_setting_default_type;
//
	static int bg_alpha_aaa;
//	int i;
	switch ((u8)(my_ppp_loop))
	{
	case (KEY_CONFIG_STATE_00_INIT):
		play_music_num(BGM_22_menu02);
		load_SDL_bg(BG_TYPE_02_key_config);
		#if 0
		//	システム(PSPのハードウェア)順
		key_setting[ 0] = pad_config[KEY_NUM00_SELECT];
		key_setting[ 1] = pad_config[KEY_NUM01_START];
		key_setting[ 2] = pad_config[KEY_NUM02_UP ];
		key_setting[ 3] = pad_config[KEY_NUM03_RIGHT ];
		key_setting[ 4] = pad_config[KEY_NUM04_DOWN ];
		key_setting[ 5] = pad_config[KEY_NUM05_LEFT ];
		key_setting[ 6] = pad_config[KEY_NUM06_L_TRIG];
		key_setting[ 7] = pad_config[KEY_NUM07_R_TRIG];
		key_setting[ 8] = pad_config[KEY_NUM08_TRIANGLE];
		key_setting[ 9] = pad_config[KEY_NUM09_CIRCLE];
		key_setting[10] = pad_config[KEY_NUM10_CROSS];
		key_setting[11] = pad_config[KEY_NUM11_SQUARE];
		#else
		{
			int i;
			for (i=0; i<KEY_NUM12_MAX; i++)
			{
				key_setting[i] = pad_config[i];
			}
		}
		#endif
		{
			static const char *const_key_str[(MAX_KEY_NAMES_12)] =
			{
				//	[[ 選択する並び順序 ]]
				"NONE",
				#if 1
				"SELECT",
				"PAUSE",
				"UP",
				"RIGHT",
				"DOWN",
				"LEFT",
				"SYSTEM",		//	"SNAP SHOT",
				"SLOW", 		//	"SYSTEM",
				"SNAP SHOT",	//	"SLOW",
				"OPTION",
				"SHOT / OK",
				"BOMB / CANCEL",
				#endif
				//	システム(PSPのハードウェア)順
				"SELECT",
				"START",
				"UP",
				"RIGHT",
				"DOWN",
				"LEFT",
				"L",
				"R",
				"SANKAKU",
				"MARU",
				"BATU",
				"SIKAKU",
				//	初期設定 / 終了
				"RESET",
				"QUIT",
				//	初期設定タイプの選択
				"TYPE 01",
				"TYPE 02",
				"TYPE 03",
				"TYPE 04",
				NULL
			};
			{
				int i;
				i = 0;
				while (NULL != const_key_str[i])
				{
					key_name_surface[i] = font_render( (char *)const_key_str[i], FONT16R);
					i++;
				}
			}
		}
		menu_cursor1				= KEY_CONFIG_MENU_13_QUIT;/*0*/
		key_setting_default_type	= 0;

		bg_alpha_aaa		= (0);
		my_ppp_loop++;// = (KEY_CONFIG_STATE_01_FADE_IN);
		break;
	case (KEY_CONFIG_STATE_01_FADE_IN):
		bg_alpha_aaa += (2);	/*fps_factor*/	/*1*/	/*6*/
		if ((/*250-6*/224) < bg_alpha_aaa)
		{
			bg_alpha_aaa = (255);
			psp_push_screen();
			my_ppp_loop++;// = (KEY_CONFIG_STATE_02_SELECT_LEFT_MENU);
		}
		SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
		psp_pop_screen();
		break;
	case (KEY_CONFIG_STATE_02_SELECT_LEFT_MENU):
		psp_pop_screen();
		if (0==my_pad_alter)/* さっき何も押されてなかった場合にキーチェック(原作準拠) */
		{
			if (my_pad & PSP_KEY_UP)				// 上ボタン入力
			{
				if (KEY_CONFIG_MENU_00_SELECT == menu_cursor1)	{	menu_cursor1 = (KEY_CONFIG_MENU_MAX-1);}
				else											{	menu_cursor1--;}
			}
			else
			if (my_pad & PSP_KEY_DOWN)			// 下ボタン入力
			{
				if ((KEY_CONFIG_MENU_MAX-1) == menu_cursor1)	{	menu_cursor1 = KEY_CONFIG_MENU_00_SELECT;}
				else											{	menu_cursor1++;}
			}
			else	/* 上下以外の通常処理(斜め入力を排除する為、上下の入力があった場合はそれ以外の処理をしない) */
			{
				if (KEY_CONFIG_MENU_12_RESET == menu_cursor1)			// 項目[ RESET ] を選んでいる場合
				{
					if (my_pad & PSP_KEY_LEFT)	// 左ボタン入力
					{
						key_setting_default_type--;
						key_setting_default_type &= (4-1);
						voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);/*テキトー*/
					}
					else
					if (my_pad & PSP_KEY_RIGHT) // 右ボタン入力
					{
						key_setting_default_type++;
						key_setting_default_type &= (4-1);
						voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);/*テキトー*/
					}
					else
					if (my_pad & PSP_KEY_SHOT_OK) // ショットボタン入力
					{
						voice_play(VOICE07_BOMB, TRACK01_EXPLODE);/*テキトー*/
						set_default_key(key_setting, key_setting_default_type);
						menu_cursor1 = KEY_CONFIG_MENU_13_QUIT;
					}
				}
				else
				if (KEY_CONFIG_MENU_13_QUIT == menu_cursor1)	// 項目[ QUIT ] を選んでいる場合
				{
					if (my_pad & PSP_KEY_BOMB_CANCEL)	// キャンセルボタン入力
					{
						voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* 自機死に音は、なるべく重ねない */
						my_ppp_loop++;// = (KEY_CONFIG_STATE_06_FADE_INIT);
					}
					else
					if (my_pad & PSP_KEY_SHOT_OK)	// ショットボタン入力
					{
						int saiteigen_exsist;	// 存在確認用。0で存在しない。存在しないとメニューを抜けれない。
						saiteigen_exsist = 0;
						{
							int i;
							for (i=0; i<KEY_NUM12_MAX/*KEY_CONFIG_MENU_MAX*/; i++)
							{
								saiteigen_exsist	|= key_setting[i];
							}
						}
						if (0!=(saiteigen_exsist&(PSP_KEY_SHOT_OK|PSP_KEY_DOWN|PSP_KEY_UP)))	// ショットボタン&↑&↓があるか確認
						{
							my_ppp_loop++;// = (KEY_CONFIG_STATE_06_FADE_INIT);
							// 最終的に代入される物
							#if 0
							//	システム(PSPのハードウェア)順
							pad_config[KEY_NUM00_SELECT]	= key_setting[ 0];
							pad_config[KEY_NUM01_START] 	= key_setting[ 1];
							pad_config[KEY_NUM02_UP]		= key_setting[ 2];
							pad_config[KEY_NUM03_RIGHT] 	= key_setting[ 3];
							pad_config[KEY_NUM04_DOWN]		= key_setting[ 4];
							pad_config[KEY_NUM05_LEFT]		= key_setting[ 5];
							pad_config[KEY_NUM06_L_TRIG]	= key_setting[ 6];
							pad_config[KEY_NUM07_R_TRIG]	= key_setting[ 7];
							pad_config[KEY_NUM08_TRIANGLE]	= key_setting[ 8];
							pad_config[KEY_NUM09_CIRCLE]	= key_setting[ 9];
							pad_config[KEY_NUM10_CROSS] 	= key_setting[10];
							pad_config[KEY_NUM11_SQUARE]	= key_setting[11];
							#else
							{
								int i;
								for (i=0; i<KEY_NUM12_MAX; i++)
								{
									pad_config[i] = key_setting[i];
								}
							}
							#endif
							ini_save();
							voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);/*テキトー*/
						}
						else
						{
						//	"dame dayo"
							voice_play(VOICE09_GRAZE, TRACK01_EXPLODE);/*テキトー*/
						}
					}
				}
				else	/* 設定項目を変更する場合( 項目[ RESET ] 項目[ QUIT ]以外を選んでいる場合) */
				{
					if (my_pad & (PSP_KEY_LEFT|PSP_KEY_RIGHT))		// 左右ボタン入力
					{
						int menu_cursor2;
						for (menu_cursor2=0; menu_cursor2<(KINOU_13_MAX-1)/*11 ???*/; menu_cursor2++)
						{
							if (const_pad_setting[menu_cursor2] == key_setting[menu_cursor1])		// 選択した所に何が入っているのか調べる
							{
								break;
							}
						}
						//
						if (my_pad & PSP_KEY_LEFT)			// 左ボタン入力
						{
							menu_cursor2--;
							if ((KINOU_00_NONE) > menu_cursor2) 	{	menu_cursor2 = (KINOU_13_MAX-1); }
						}
						else
						if (my_pad & PSP_KEY_RIGHT) 		// 右ボタン入力
						{
							menu_cursor2++;
							if ((KINOU_13_MAX-1) < menu_cursor2)	{	menu_cursor2 = KINOU_00_NONE; }
						}
						key_setting[menu_cursor1] = const_pad_setting[menu_cursor2];
						//
						voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);/*テキトー*/
					}
				}
			}
		}
		/* 描画 */
		{
		//	/*static*/ SDL_Rect rect_src_offset;		/* 位置 */
			/*static*/ SDL_Rect rect_locate_offset; 	/* 表示位置 */
			int i;
			int jj;
			/* 機能番号の描画 */
			for (i=0; i</*12*/(12)/*KEY_CONFIG_MENU_MAX*/; i++)
			{
				/* ボタン設定描画 */
				//kp_search(key_setting[i], i);
				//static void kp_search(int btn, int num)
				{
					int kinou_number;
					for (kinou_number=0; kinou_number<(MAX_KEY_NAMES_12-1); kinou_number++)/* (11==kinou_number は NULL) */
					{
						if (const_pad_setting[kinou_number] == /*btn*/key_setting[i])
						{
							goto aaa_break/*break*/;
						}
					}
				aaa_break:;
					if ((MAX_KEY_NAMES_12-1)==kinou_number)/* 見付からなかった (11==kinou_number は NULL) */
					{	kinou_number = 0;}
					if (menu_cursor1==i)	{	jj= 2;	}
					else					{	jj= 0;	}
					rect_locate_offset.x = X_LOCATE_OFFSET_04-(jj);
					rect_locate_offset.y = Y_LOCATE_OFFSET+(/*num*/i<<4)-(jj);
					SDL_SetAlpha(	key_name_surface[kinou_number], SDL_SRCALPHA, (127+(jj<<6)));
					SDL_BlitSurface(key_name_surface[kinou_number], NULL,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
				//	rect_src_offset.x = 0;
				//	rect_src_offset.y = (kinou_number<<4);
				//	rect_src_offset.w = (16*9);
				//	rect_src_offset.h = (16);
				//	SDL_BlitSurface(sdl_screen[SDL_01_BACK_SCREEN],&rect_src_offset,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
				}
			}
			/* リセットタイプの描画 */
			{
				if (menu_cursor1==12)	{	jj= 2;	}
				else					{	jj= 0;	}
				rect_locate_offset.x = X_LOCATE_OFFSET_03-(jj);
				rect_locate_offset.y = Y_LOCATE_OFFSET+(/*num*/12/*i*/<<4)-(jj);
				SDL_SetAlpha(	key_name_surface[(MAX_KINOU_11+MAX_BUTTON_12+MAX_MENU_02)+key_setting_default_type], SDL_SRCALPHA, (127+(jj<<6)));
				SDL_BlitSurface(key_name_surface[(MAX_KINOU_11+MAX_BUTTON_12+MAX_MENU_02)+key_setting_default_type], NULL,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
			}
			/* ボタン名称描画 */
			for (i=0; i<(MAX_BUTTON_12+MAX_MENU_02)/*KEY_CONFIG_MENU_MAX*/; i++)
			{
				if (menu_cursor1==i)	{	jj = 2; 	}
				else					{	jj = 0; 	}
				rect_locate_offset.x = (X_LOCATE_OFFSET_02)-(jj);
				rect_locate_offset.y = (Y_LOCATE_OFFSET)+(/*num*/ /*gazo_iti_henkan[i]*/i<<4)-(jj);
				SDL_SetAlpha(	key_name_surface[i+(MAX_KINOU_11)], SDL_SRCALPHA, (127+(jj<<6)));
				SDL_BlitSurface(key_name_surface[i+(MAX_KINOU_11)], NULL,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
			}
		}
		break;
	case (KEY_CONFIG_STATE_06_FADE_INIT):
		play_music_num(BGM_23_menu01);
		load_SDL_bg(BG_TYPE_00_title_bg);
		bg_alpha_aaa		= (0);
		my_ppp_loop++;// = (KEY_CONFIG_STATE_07_FADE_OUT);
		break;
	case (KEY_CONFIG_STATE_07_FADE_OUT):
		bg_alpha_aaa += (2);	/*fps_factor*/		/*1*/		/*8*/
		if ((/*250-8*/224) < bg_alpha_aaa)
		{
			bg_alpha_aaa = (255);
			#if 0/* 文字の表示用画像を開放する */
			{
				int i;
				for (i=0; i<(MAX_KEY_NAMES_12-1); i++)
				{
					/* ここでハングアップ */
					SDL_FreeSurface(key_name_surface[i]);
				}
			}
			#endif
			main_call_func = title_menu_start;	/* タイトルメニューへ移動 */
		}
		SDL_SetAlpha( sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
		psp_pop_screen();
		break;
	}
}

global void key_config_start(void)
{
	my_ppp_loop = 0;
	main_call_func = key_config_local_work;
}
