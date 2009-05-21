
#include "support.h"

/*---------------------------------------------------------
	キーコンフィグ
---------------------------------------------------------*/

enum
{
	KEY_CONFIG_STATE_00_INIT = 0,
	KEY_CONFIG_STATE_01_FADE_IN,
	KEY_CONFIG_STATE_02_SELECT_LEFT_MENU,
	KEY_CONFIG_STATE_06_FADE_INIT,
	KEY_CONFIG_STATE_07_FADE_OUT,
};
//	[***20080110-20080115
//	[***20090110
//キーコンフィグ追加。
//基本は絶対値指定(機能固定値)でsupport.cのkeypollの中のkeyconfigだけ配列指定番号に変数（keyconfig[key_xxx）を取る
//またkeypollに入る値は、0と1だけのままだとボタン設定上問題が出るのでビットで管理するようにした
/*static*/ //void key_config_init()
//{
//	//key_config_state= (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_00_INIT);
//	psp_loop=(ST_WORK_KEY_CONFIG|0);
//}

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

/*static*/ void set_default_key(int *key_setting_map, int key_setting_type)
{
	static const /*int*/Uint8 default_key_map[4][12] =
	{
		{	/* type 01 模倣風 標準 (スレ内の意見で決まった) */
			KINOU_01_SELECT,	KINOU_02_PAUSE, 	KINOU_03_UP,			KINOU_04_RIGHT, /* KINOU_12_BOMB KINOU_00_NONE*/
			KINOU_05_DOWN,		KINOU_06_LEFT,		KINOU_07_SNAP_SHOT, 	KINOU_09_SLOW,
			KINOU_09_SLOW,		KINOU_10_OPTION,	KINOU_11_SHOT,			KINOU_12_BOMB,
		},
		{	/* type 02 toho DX 互換 */
			KINOU_01_SELECT,	KINOU_02_PAUSE, 	KINOU_03_UP,			KINOU_04_RIGHT, /* KINOU_12_BOMB KINOU_00_NONE*/
			KINOU_05_DOWN,		KINOU_06_LEFT,		KINOU_07_SNAP_SHOT, 	KINOU_09_SLOW,
			KINOU_09_SLOW,		KINOU_10_OPTION,	KINOU_12_BOMB,			KINOU_11_SHOT,
		},
		{	/* type 03 psp-1000 (□ボタンがカスなので使えない場合) */
			KINOU_01_SELECT,	KINOU_02_PAUSE, 	KINOU_03_UP,			KINOU_04_RIGHT, /* KINOU_12_BOMB KINOU_00_NONE*/
			KINOU_05_DOWN,		KINOU_06_LEFT,		KINOU_07_SNAP_SHOT, 	KINOU_09_SLOW,
			KINOU_09_SLOW,		KINOU_11_SHOT,		KINOU_12_BOMB,			KINOU_10_OPTION,
		},
		{	/* type 04 psp-2000 (□ボタンがショットの場合) */
			KINOU_01_SELECT,	KINOU_02_PAUSE, 	KINOU_03_UP,			KINOU_04_RIGHT, /* KINOU_12_BOMB KINOU_00_NONE*/
			KINOU_05_DOWN,		KINOU_06_LEFT,		KINOU_07_SNAP_SHOT, 	KINOU_09_SLOW,
			KINOU_12_BOMB,		KINOU_10_OPTION,	KINOU_09_SLOW,			KINOU_11_SHOT,
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

extern SDL_Surface *back_screen;
extern void ini_save(void); 	// [***090115
/*static*/ void key_config_work(void)
{

#define MAX_KINOU_11			(12+1)
#define MAX_BUTTON_12			(12)
#define MAX_MENU_02 			(2)
#define MAX_DEFAULT_MAP_04		(4)

#define MAX_KEY_SETTING_12 (1+12+1)
	static int key_setting[MAX_KEY_SETTING_12]; 						// 数字=ボタン変数 並びは "bg/key_haikei_surface.png"
	static const int const_key_setting[MAX_KEY_SETTING_12/*11*/] =		// 数字=機能定数 並びは "fonts/keylist.png"
	{	// 数字 = 機能定数 並び。	[[ 選択する並び順序 ]]
		KINOU_00_NONE,
	#if 0
		KINOU_11_SHOT,
		KINOU_12_BOMB,
		KINOU_09_SLOW,
		KINOU_03_UP,
		KINOU_05_DOWN,
		KINOU_06_LEFT,
		KINOU_04_RIGHT,
		KINOU_02_PAUSE,
		KINOU_10_OPTION,
		KINOU_07_SNAP_SHOT,
		KINOU_01_SELECT,
		KINOU_08_SYSTEM,
	#else
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
	#endif
		0
	};
	#define X_LOCATE_OFFSET_01	(64)/*(80-16)*/
	#define X_LOCATE_OFFSET_02	(80)
	#define X_LOCATE_OFFSET_03	(200)
	#define X_LOCATE_OFFSET_04	(250)
	#define Y_LOCATE_OFFSET 	(40)
	#define MAX_KEY_NAMES_12	(MAX_KEY_SETTING_12+MAX_BUTTON_12+MAX_MENU_02+MAX_DEFAULT_MAP_04)
	static SDL_Surface *key_name_surface[MAX_KEY_NAMES_12]; 	// 文字の画像


	static int menu_cursor1;
	static int key_setting_default_type;
//
	static int bg_alpha_aaa;
//	int i;
//1231881
	switch (psp_loop/*key_config_state*/)
	{
	case (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_00_INIT):
		play_music(BGM_05_stage5);
		{
			SDL_Surface *loadpic	= loadbmp0("bg/key_config.jpg", 0, 0);/*"bg/key_haikei_surface.png"*/
			SDL_BlitSurface(loadpic, NULL, back_screen, NULL);
			unloadbmp_by_surface(loadpic);
			//unloadbmp_by_surface(key_haikei_surface);
		}

		#if 0
		//	システム(PSPのハードウェア)順
		key_setting[ 0] = keyconfig[key_00_sl];
		key_setting[ 1] = keyconfig[key_01_st];
		key_setting[ 2] = keyconfig[key_02_u ];
		key_setting[ 3] = keyconfig[key_03_r ];
		key_setting[ 4] = keyconfig[key_04_d ];
		key_setting[ 5] = keyconfig[key_05_l ];
		key_setting[ 6] = keyconfig[key_06_lt];
		key_setting[ 7] = keyconfig[key_07_rt];
		key_setting[ 8] = keyconfig[key_08_sa];
		key_setting[ 9] = keyconfig[key_09_ma];
		key_setting[10] = keyconfig[key_10_ba];
		key_setting[11] = keyconfig[key_11_si];
		#else
		{
			int i;
			for (i=0; i<12; i++)
			{
				key_setting[i] = keyconfig[i];
			}
		}
		#endif
		{
			static const char *const_key_str[(MAX_KEY_NAMES_12)] =
			{
				//	[[ 選択する並び順序 ]]
				"NONE",
				#if 0
				"SHOT / OK",
				"BOMB / CANCEL",
				"SLOW",
				"UP",
				"DOWN",
				"LEFT",
				"RIGHT",
				"PAUSE",
				"OPTION",
				"SNAP SHOT",
				"SELECT",
				"SYSTEM",
				#else
				"SELECT",
				"PAUSE",
				"UP",
				"RIGHT",
				"DOWN",
				"LEFT",
				"SNAP SHOT",
				"SYSTEM",
				"SLOW",
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
					key_name_surface[i] = font_render((char *)const_key_str[i],FONT03/*FO NT06*/);
					i++;
				}
			}
		}
		menu_cursor1				= KEY_CONFIG_MENU_13_QUIT/*0*/;
		key_setting_default_type	= 0;
		//SDL_SetAlpha(key_haikei_surface, SDL_SRCALPHA, 0);
		//keyboa rd_clear();

		bg_alpha_aaa		= 0;
		psp_loop++;//key_config_state = (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_01_FADE_IN);
		break;
	case (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_01_FADE_IN):
		bg_alpha_aaa += /*1*/2/*6*/;/**fps_fa ctor*/
		if ((/*250-6*/224) < bg_alpha_aaa)
		{
			bg_alpha_aaa = 255;
			psp_push_screen();
			psp_loop++;//key_config_state = (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_02_SELECT_LEFT_MENU);
		}
		SDL_SetAlpha(/*key_haikei_surface*/back_screen, SDL_SRCALPHA, bg_alpha_aaa);
		psp_pop_screen();//SDL_BlitSurface(key_haikei_surface,NULL,screen,NULL);
		break;
	case (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_02_SELECT_LEFT_MENU):
		psp_pop_screen();//SDL_BlitSurface(key_haikei_surface,NULL,screen,NULL);
		if (0==my_pad)
		{
			if (my_pad_alter & PSP_KEY_UP)				// 上ボタン入力
			{
				if (KEY_CONFIG_MENU_00_SELECT == menu_cursor1)	{	menu_cursor1 = (KEY_CONFIG_MENU_MAX-1);}
				else											{	menu_cursor1--;}
			}
			else
			if (my_pad_alter & PSP_KEY_DOWN)			// 下ボタン入力
			{
				if ((KEY_CONFIG_MENU_MAX-1) == menu_cursor1)	{	menu_cursor1 = KEY_CONFIG_MENU_00_SELECT;}
				else											{	menu_cursor1++;}
			}
			else	/* 上下以外の通常処理(斜め入力を排除する為、上下の入力があった場合はそれ以外の処理をしない) */
			{
				if (KEY_CONFIG_MENU_12_RESET == menu_cursor1)			// 項目[ RESET ] を選んでいる場合
				{
					if (my_pad_alter & PSP_KEY_LEFT)	// 左ボタン入力
					{
						key_setting_default_type--;
						key_setting_default_type &= (4-1);
						play_voice_auto_track(VOICE02_ZAKO_HAKAI);
					}
					else
					if (my_pad_alter & PSP_KEY_RIGHT)	// 右ボタン入力
					{
						key_setting_default_type++;
						key_setting_default_type &= (4-1);
						play_voice_auto_track(VOICE02_ZAKO_HAKAI);
					}
					else
					if (my_pad_alter & PSP_KEY_SHOT_OK) // ショットボタン入力
					{
						play_voice_auto_track(VOICE07_BOMB);
						set_default_key(key_setting, key_setting_default_type);
						menu_cursor1 = KEY_CONFIG_MENU_13_QUIT;
					}
				}
				else
				if (KEY_CONFIG_MENU_13_QUIT == menu_cursor1)	// 項目[ QUIT ] を選んでいる場合
				{
					if (my_pad_alter & PSP_KEY_BOMB_CANCEL) 	// キャンセルボタン入力
					{
						play_voice_auto_track(VOICE04_SHIP_HAKAI);
						psp_loop++;//	key_config_state = (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_06_FADE_INIT);
						//key_haikei_surface = loadbmp("bg/title_bg.jpg");
//						bg_alpha_aaa = 255;
//						play_music(BGM_00_intro);
					}
					else
					if (my_pad_alter & PSP_KEY_SHOT_OK) 	// ショットボタン入力
					{
						#define FLAG_KEY_SHOT	(1<<0)
						#define FLAG_KEY_UP 	(1<<1)
						#define FLAG_KEY_DOWN	(1<<2)
						int saiteigen_exsist;	// 存在確認用。0で存在しない。存在しないとメニューを抜けれない。
						saiteigen_exsist = 0;
						{
							int i;
							for (i=0; i<MAX_KEY_SETTING_12; i++)	// ショットボタン&↑&↓があるか確認
							{
								if (key_setting[i] == KINOU_11_SHOT)	{	saiteigen_exsist	|= FLAG_KEY_SHOT;	}
								if (key_setting[i] == KINOU_03_UP)		{	saiteigen_exsist	|= FLAG_KEY_UP; 	}
								if (key_setting[i] == KINOU_05_DOWN)	{	saiteigen_exsist	|= FLAG_KEY_DOWN;	}
							}
						}
						if ((FLAG_KEY_SHOT|FLAG_KEY_DOWN|FLAG_KEY_UP)==saiteigen_exsist)
						{
							psp_loop++;//	key_config_state = (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_06_FADE_INIT);
							//key_haikei_surface = loadbmp("bg/title_bg.jpg");
//							bg_alpha_aaa = 255;
//							play_music(BGM_00_intro);
							// 最終的に代入される物
							#if 0
							//	システム(PSPのハードウェア)順
							keyconfig[key_00_sl] = key_setting[ 0];
							keyconfig[key_01_st] = key_setting[ 1];
							keyconfig[key_02_u]  = key_setting[ 2];
							keyconfig[key_03_r]  = key_setting[ 3];
							keyconfig[key_04_d]  = key_setting[ 4];
							keyconfig[key_05_l]  = key_setting[ 5];
							keyconfig[key_06_lt] = key_setting[ 6];
							keyconfig[key_07_rt] = key_setting[ 7];
							keyconfig[key_08_sa] = key_setting[ 8];
							keyconfig[key_09_ma] = key_setting[ 9];
							keyconfig[key_10_ba] = key_setting[10];
							keyconfig[key_11_si] = key_setting[11];
							#else
							{
								int i;
								for (i=0; i<12; i++)
								{
									keyconfig[i] = key_setting[i];
								}
							}
							#endif
							ini_save();
							play_voice_auto_track(VOICE02_ZAKO_HAKAI);
						}
						else
						{
						//	"dame dayo"
							play_voice_auto_track(VOICE09_GRAZE);
						}
					}
				}
				else	/* 設定項目を変更する場合( 項目[ RESET ] 項目[ QUIT ]以外を選んでいる場合) */
				{
					if (my_pad_alter & (PSP_KEY_LEFT|PSP_KEY_RIGHT))		// 左右ボタン入力
					{
						int menu_cursor2;
						for (menu_cursor2=0; menu_cursor2<(KINOU_13_MAX-1)/*11 ???*/; menu_cursor2++)
						{
							if (const_key_setting[menu_cursor2] == key_setting[menu_cursor1])		// 選択した所に何が入っているのか調べる
							{
								break;
							}
						}
						//
						if (my_pad_alter & PSP_KEY_LEFT)			// 左ボタン入力
						{
							menu_cursor2--;
							if ((KINOU_00_NONE) > menu_cursor2) {	menu_cursor2 = (KINOU_13_MAX-1); }
						}
						else
						if (my_pad_alter & PSP_KEY_RIGHT)			// 右ボタン入力
						{
							menu_cursor2++;
							if ((KINOU_13_MAX-1) < menu_cursor2)	{	menu_cursor2 = KINOU_00_NONE; }
						}
						key_setting[menu_cursor1] = const_key_setting[menu_cursor2];
						//
						play_voice_auto_track(VOICE02_ZAKO_HAKAI);
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
			for (i=0; i</*12*/(12)/*MAX_KEY_SETTING_12*/; i++)
			{
				/* ボタン設定描画 */
				//kp_search(key_setting[i], i);
				//static void kp_search(int btn, int num)
				{
					int kinou_number;
					for (kinou_number=0; kinou_number<(MAX_KEY_NAMES_12-1); kinou_number++)/* (11==kinou_number は NULL) */
					{
						if (const_key_setting[kinou_number] == /*btn*/key_setting[i])
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
					{	/* 標準形式からkey_config画像形式(key_bgの並び方)へ変換する  */
					//	static const Uint8 gazo_iti_henkan[12] = { 10,11,4,7, 5,6,9,8, 3,0,1,2 };
						rect_locate_offset.y = Y_LOCATE_OFFSET+(/*num*/ /*gazo_iti_henkan[i]*/i<<4)-(jj);
					}
					SDL_SetAlpha(	key_name_surface[kinou_number], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
					SDL_BlitSurface(key_name_surface[kinou_number], NULL,screen,&rect_locate_offset);
				//	rect_src_offset.x = 0;
				//	rect_src_offset.y = (kinou_number<<4);
				//	rect_src_offset.w = (16*9);
				//	rect_src_offset.h = (16);
				//	SDL_BlitSurface(back_screen,&rect_src_offset,screen,&rect_locate_offset);
				}
			}
			/* リセットタイプの描画 */
			{
				if (menu_cursor1==12)	{	jj= 2;	}
				else					{	jj= 0;	}
				rect_locate_offset.x = X_LOCATE_OFFSET_03-(jj);
				rect_locate_offset.y = Y_LOCATE_OFFSET+(/*num*/12/*i*/<<4)-(jj);
				SDL_SetAlpha(	key_name_surface[(MAX_KINOU_11+MAX_BUTTON_12+MAX_MENU_02)+key_setting_default_type], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
				SDL_BlitSurface(key_name_surface[(MAX_KINOU_11+MAX_BUTTON_12+MAX_MENU_02)+key_setting_default_type], NULL,screen,&rect_locate_offset);
			}
			/* ボタン名称描画 */
			for (i=0; i<(MAX_BUTTON_12+MAX_MENU_02)/*MAX_KEY_SETTING_12*/; i++)
			{
				if (menu_cursor1==i)	{	jj= 2;	}
				else					{	jj= 0;	}
				rect_locate_offset.x = (X_LOCATE_OFFSET_02)-(jj);
				rect_locate_offset.y = (Y_LOCATE_OFFSET)+(/*num*/ /*gazo_iti_henkan[i]*/i<<4)-(jj);
				SDL_SetAlpha(	key_name_surface[i+(MAX_KINOU_11)], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
				SDL_BlitSurface(key_name_surface[i+(MAX_KINOU_11)], NULL,screen,&rect_locate_offset);
			}
		}
		break;
	case (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_06_FADE_INIT):
		#if 0
		{
			int i;
			for (i=0; i<(MAX_KEY_NAMES_12-1)/*12 ???*/; i++)
			{
				/* ここでハングアップ */
				SDL_FreeSurface(key_name_surface[i]);
			}
		}
		#endif
		play_music(BGM_00_intro);
		{
			SDL_Surface *loadpic	= loadbmp0("bg/title_bg.jpg", 0, 0);/*"bg/key_haikei_surface.png"*/
			SDL_BlitSurface(loadpic, NULL, back_screen, NULL);
			unloadbmp_by_surface(loadpic);
			//unloadbmp_by_surface(key_haikei_surface);
		}
		bg_alpha_aaa		= 0;
		psp_loop++;//	key_config_state = (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_07_FADE_OUT);
		break;
	case (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_07_FADE_OUT):
		bg_alpha_aaa += /*1*/2/*8*/;/**fps_fa ctor*/
		if ((/*250-8*/224) < bg_alpha_aaa)
		{
			bg_alpha_aaa = 255;
			/* メインメニューに戻る */
			psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);	//newsta te(ST_MENU/*ST_INTRO*/,0,1);
		}
		SDL_SetAlpha(/*key_haikei_surface*/back_screen, SDL_SRCALPHA, bg_alpha_aaa);
		psp_pop_screen();//SDL_BlitSurface(key_haikei_surface,NULL,screen,NULL);
		break;
	}
}
