
#include "game_main.h"

/*---------------------------------------------------------
	共通メニュー
	-------------------------------------------------------
	実際、現在の共通メニューは使いにくいし、
	戻れないという致命的な欠陥もありますが、
	それでも使ってるのは、「作るのがめんどくさい」＆「先に他にやる事がたくさんある」
	からです。
	そのうち、全廃して改良する予定です。
	順序からいって、Gu化完了してからの方がやり易いです。
	というかSDLで作っても２度手間(作り直し)になる。
---------------------------------------------------------*/

//extern SDL_Surface *back_screen;

//	static int volume/*=2*/; /* 0-3 */
//	/*static*/ int zanki/*=2*/; /* 0-3 */

int continue_stage;
int practice_mode;

/*---------------------------------------------------------

---------------------------------------------------------*/
#define MAX_MENU_ITEMS (8)
typedef struct
{
	int x256;
	int y256;
//
	int MENU_DATA_i0; 	// 表示用 x オフセット */
	int MENU_DATA_i1; 	// 表示用 y オフセット */
	int MENU_DATA_i2; 	// スプライト用(メニュー選択時、横 x の揺れ幅) */
	int MENU_DATA_i3; 	// スプライト用(揺れ幅が徐々に戻る用) */
} MY_OBJ;

//	int type;
//z	int color32;
//	int flags;
//	SDL_Surface 	*bg;

typedef struct
{
	MY_OBJ			menu_item_my_obj[MAX_MENU_ITEMS/*20*/]; 	/* オブジェクト */
	SDL_Surface 	*menu_item_surface[MAX_MENU_ITEMS/*20*/];			/* 文字の画像 */
	int active_item;	/* 現在メニュー上で選択されている項目番号 */
	int max_items;		/* オプションの項目数 */
//
	int menu_brite; 	/* メニューの明るさ(α値) */
	int menu_state; 	/* メニューの状態。 MENU_STATE */
	int time_out_flag;	/* -3==,  -2==, -1==時間切れなし, 0==時間切れ, 時間カウント中=1... */
	int fade_out_flag_dummy;
} MENU;
#define M1_NOT_TIME_OUT 	(-1)/* 時間切れなしに設定(時間切れなしモード) */
//#define M2_STATE_TIME_OVER	(-2)/* 時間切れありモードで時間切れになった状態 */

static int generic_menu_done_flag;	/* 共通メニュー使用中か否かのフラグ */

static MENU start_menu;

static MENU pause_menu;
static MENU ask_continue_menu;

static MENU stage_select_menu;
static MENU difficulty_menu;


/*---------------------------------------------------------

---------------------------------------------------------*/

enum
{
	GENERIC_MENU_STATE_00_INIT = 0,
	GENERIC_MENU_STATE_01_GENERIC,
	GENERIC_MENU_STATE_02_DONE,
	GENERIC_MENU_STATE_99_MAX
};

/* メニューの状態。 MENU_STATE */
enum
{
	MENU_STATE_00_INIT_MENU = 0,
	MENU_STATE_00_FADE_IN_MENU,
	MENU_STATE_01_WORK_MENU,
	MENU_STATE_02_FADE_OUT,
	MENU_STATE_03_FININSH,
//	MENU_STATE_04_BRANCH,
	MENU_STATE_04_MAX
};

enum
{
	RES00_MAIN_MENU = 0,
	RES01_STAGE_SELECT,
	RES02_DIFFICULTY,
//
	RES08_ASK_CONTINUE_MENU,
	RES09_PAUSE_MENU,
	RES99_MAX
};

typedef struct
{
	int x_offset;
	const char *str_name;
} MENU_RESOURCE;
//	int fade_out;
//	int time_out;
static MENU_RESOURCE my_111resource[RES99_MAX][/*8*/MAX_MENU_ITEMS/*10*/] =
{
	{	//	RES00_MAIN_MENU 	//		/*const*/ char *start_menu_options[] =/*(char *)*/start_menu_options
		{	360,		"START",			},
		{	250,		"EXTRA START",		},/* 330 */
		{	245,		"PRACTICE START",	},
	//	{	400,		"REPLAY",			},
		{	320,		"STORY",			},		// ちょっと変更 */
		{	285,		"RESULT",			},
	//	{	310,		"MUSIC ROOM",		},
		{	335,		"OPTION",			},
		{	310,		"QUIT", 			},
		{	0,			NULL,				},
	},
	{	//	RES01_STAGE_SELECT	/*const*/ char *stage_select_menu_options[] =/*(char *)*/stage_select_menu_options
		{	360,		"1",			},	// stage1 */
		{	350,		"2",			},	// stage2 */
		{	340,		"3",			},	// stage3 */
		{	330,		"4",			},	// stage4 */
		{	320,		"5",			},	// stage5 */
		{	310,		"6",			},	// stage6 */
	//	#if (1==US E_ENDING_DEBUG)
	//	{	275,		"ENDING",		},	// ending */
	//	#endif //(1==US E_ENDING_DEBUG)
		{	0,NULL, 	},
	},
	{	//	RES02_DIFFICULTY		//			/*const*/ char *difficulty_menu_options[] = /*(char *)*/difficulty_menu_options
		{	275,		"EASY", 		},
		{	245,		"NORMAL",		},
		{	220,		"HARD", 		},
		{	185,		"LUNATIC",		},
		{	0,NULL, 	},
	},
	{	//	RES08_ASK_CONTINUE_MENU 	//	/*const*/ char *ask_continue_menu_options[] =/*(char *)*/ask_continue_menu_options
		{	80, 		"CONTINUE YES", 	},
		{	90, 		"CONTINUE NO",		},
		{	0,NULL, 	},
	},
	{	//	RES09_PAUSE_MENU		//	/*const*/ char *pause_menu_options[] =/*(char *)*/pause_menu_options
		{	60, 		"CONTINUE GAME",	},/*155*/
		{	100,		"QUIT GAME",		},/*200*/
		{	0,NULL, 	},
	},
};

/*---------------------------------------------------------

---------------------------------------------------------*/

//typedef struct	// たぶん矩形データ用? */
//{
//	int i0; 	// 表示用 x オフセット */
//	int i1; 	// 表示用 y オフセット */
//	int i2; 	// スプライト用(メニュー選択時、横 x の揺れ幅) */
//	int i3; 	// スプライト用(揺れ幅が徐々に戻る用) */
//} MENU_DATA;

//#if 0
//	#define FPS_MENU_FACTOR 	(fps_factor)
//	#define FPS_MENU_FACTOR10	(10/fps_factor)/*←どうも不安定(Division by Zero ???)*/
//	#define FPS_MENU_FACTOR8	(8*fps_factor)
//	#define FPS_MENU_FACTOR15	(15*fps_factor)
//#else
	#define FPS_MENU_FACTOR 	(1)
	#define FPS_MENU_FACTOR10	(16/*64*/)/* 10/FPS_MENU_FACTOR */
	#define FPS_MENU_FACTOR8	(16)/* 8*FPS_MENU_FACTOR */
	#define FPS_MENU_FACTOR15	(3/*2*/)
//#endif

enum
{
	MAIN_MENU_START = 0,
	MAIN_MENU_EXTRA_START,
	MAIN_MENU_PRACTICE_START,
//	MAIN_MENU_REPLAY,
	MAIN_MENU_STORY,
	MAIN_MENU_RESULT,
//	MAIN_MENU_MUSIC_ROOM,
	MAIN_MENU_OPTION,
	MAIN_MENU_QUIT
};

/*---------------------------------------------------------

---------------------------------------------------------*/
extern int now_max_continue;
static void generic_menu_work(MENU *m)
{
//	if (/*!m->*/0==generic_menu_done_flag)
	{
	//	if (m->fade_out_flag)
	//	{
	//		psp_clear_screen();
	//		SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN],SDL_SRCALPHA, 255-((m->menu_brite)>>1) );
	//	}
		psp_pop_screen();
	//	static int www=10;
		switch (m->menu_state)
		{
		case MENU_STATE_00_FADE_IN_MENU:	/* Increasing menu_brite, fade menu in */
			m->menu_brite += FPS_MENU_FACTOR8;
			if (255 <= (m->menu_brite) )
			{
				m->menu_state = MENU_STATE_01_WORK_MENU;
				m->menu_brite = 255;
			}
			break;
		case MENU_STATE_01_WORK_MENU: /* fadein complete */
		//if ( (m->menu_state<2))
		{
			if (0==my_pad)
			{
				if (my_pad_alter & (PSP_KEY_DOWN|PSP_KEY_UP|PSP_KEY_PAUSE|PSP_KEY_RIGHT))
				{
					voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
				}
				if (my_pad_alter & PSP_KEY_DOWN)
				{
					if (m->active_item == m->max_items-1)
					{	m->active_item = 0;}
					else
					{
						if ((ST_WORK_MENU|ST_MENU_SUB_MAIN_MENU)==(psp_loop))
						{
							if ((MAIN_MENU_EXTRA_START-1)==m->active_item)
							{
								/* 0==						(					  1面までの練習は選べる) */
								/* 1==		  2面まで到達	(					  2面までの練習は選べる) */
								/* ... */
								/* 5==最終面==6面まで到達	(EXTRA STAGE選べない、6面までの練習は選べる) */
								/* 6==最終面==6面クリア 	(EXTRA STAGE選べる) */
								/* 7==EXTRA面==EXTRA面クリア(EXTRA STAGE選べる) */
								if (6 > (option_config[OPTION_CONFIG_07_OPEN] & 0x07))
								{
									m->active_item++;
								}
							}
						}
						m->active_item++;
					}
				//	www=FPS_MENU_FACTOR10;
					m->time_out_flag = M1_NOT_TIME_OUT;/* 時間切れなし */
				}
				else if (my_pad_alter & PSP_KEY_UP)
				{
					if (0 == m->active_item)
					{	m->active_item = m->max_items-1;}
					else
					{
						if ((ST_WORK_MENU|ST_MENU_SUB_MAIN_MENU)==(psp_loop))
						{
							if ((MAIN_MENU_EXTRA_START+1)==m->active_item)
							{
								if (6 > (option_config[OPTION_CONFIG_07_OPEN] & 0x07))
								{
									m->active_item--;
								}
							}
						}
						m->active_item--;
					}
				//	www=FPS_MENU_FACTOR10;
					m->time_out_flag = M1_NOT_TIME_OUT;/* 時間切れなし */
				}
				if ((ST_WORK_MENU|ST_MENU_SUB_PAUSE)==(psp_loop)/*state.substate==ST_MENU_SUB_PAUSE*/) // ポーズメニューの場合は */
				{
					if (my_pad_alter & PSP_KEY_PAUSE)	// startボタンでポーズ解除 */
					{
						pause_menu.active_item = 0;
						m->menu_state = MENU_STATE_02_FADE_OUT;
						m->time_out_flag = M1_NOT_TIME_OUT;/* 時間切れなし */
					}
					/* ボスデバッグ用 */
					#if 0/*(1==DEBUG_MODE)*/
					if (my_pad_alter & PSP_KEY_RIGHT)
					{
						now_max_continue = 90;/*test*/	/* ランキングにさせない */
//						pd_my_score=8;/*test*/
//						pd_zanki=8;/*test*/
					//	pd_bombs=8;/*test*/
					//	#if 0
					//	/* ボスチェック用 */
					//	pd_weapon_power=MAX_POWER_IS_128;/*test*/
					//	#endif
					}
					#endif
				}
			}
		}
			break;
		case MENU_STATE_02_FADE_OUT:	/* selection done, decreasing menu_brite, fade menu out */
			m->menu_brite -= FPS_MENU_FACTOR8;
			if (0 >= (m->menu_brite) )
			{
				m->menu_state	= MENU_STATE_03_FININSH;
				m->menu_brite	= 0;
			}
			break;
		case MENU_STATE_03_FININSH: 	/* fadout fininshed, menu done */
			SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN],SDL_SRCALPHA,255);
			psp_pop_screen();

//			{	int i;
//				for (i=0; i<m->max_items; i++)
//				{
//					m->menu_item_my_obj[i].type = SP_DELETE;
//					//SDL_FreeSurface(m->menu_item_surface[i]);
//				}
//			}
			#if 0
//			if ((-2) == m->time_out_flag)/*(???)*/
//			{
//				m->time_out_flag = (-3);	/*(???)*/
//			}
			#endif
			/*m->*/generic_menu_done_flag = 1;
		//	www=FPS_MENU_FACTOR10;
			return;
		}
//
		{
			static int angle512/*=0*/;
			angle512 += FPS_MENU_FACTOR15;
			mask512(angle512);
			int i;
			for (i=0; i<m->max_items; i++)
			{
				if (i==m->active_item)	/* 選択された */
				{
					m->menu_item_my_obj[i].MENU_DATA_i2 = 12;	/* ゆれ幅を 12 にする */
				}
				else	/* 選択されてない */
				{
					if ((m->menu_item_my_obj[i].MENU_DATA_i2) && (0==m->menu_item_my_obj[i].MENU_DATA_i3))
					{
						m->menu_item_my_obj[i].MENU_DATA_i2--;
						m->menu_item_my_obj[i].MENU_DATA_i3 = 3;
					}
				}
				if (m->menu_item_my_obj[i].MENU_DATA_i3)
				{	m->menu_item_my_obj[i].MENU_DATA_i3--;}
				m->menu_item_my_obj[i].x256 = (m->menu_item_my_obj[i].MENU_DATA_i0*256)+(cos512((angle512))*m->menu_item_my_obj[i].MENU_DATA_i2);
			//	m->menu_item_my_obj[i].y256 = (m->menu_item_my_obj[i].MENU_DATA_i1*256)+(sin512((angle512))*m->menu_item_my_obj[i].MENU_DATA_i2);
				m->menu_item_my_obj[i].y256 = (m->menu_item_my_obj[i].MENU_DATA_i1*256);
			//	{
			//		m->menu_item_my_obj[i].menu_brite	= m->menu_brite;
//z					m->menu_item_my_obj[i].color32		= (((m->menu_brite)<<24)|(0x00ffffff));
			//		if ((m->menu_item_my_obj[i]->x256 > 0)&&
			//			(m->menu_item_my_obj[i]->y256 > 0))
//					{	m->menu_item_my_obj[i].flags |= (SP_FLAG_VISIBLE);}
			//	}
			}
		}
//		sp rite_work_SDL(SP_GROUP_PAUSE_S P_ME NU_TEXT);/*SP_GROUP_TEXTS*/
//		pause_sp rite_display();/*SP_GROUP_TEXTS*/
		/* 描画 */
		{			/*static*/ SDL_Rect rect_locate_offset; 	/* 表示位置 */
			int i;
			for (i=0; i<m->max_items; i++)
			{
				rect_locate_offset.x = ((m->menu_item_my_obj[i].x256)>>8);
				rect_locate_offset.y = ((m->menu_item_my_obj[i].y256)>>8);
			//	SDL_SetAlpha(	m->menu_item_surface[i], SDL_SRCALPHA, /*bg_alpha_aaa*/(255));
				SDL_SetAlpha(	m->menu_item_surface[i], SDL_SRCALPHA, /*bg_alpha_aaa*/(m->menu_brite));
				SDL_BlitSurface(m->menu_item_surface[i], NULL,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
			}
		}
		if (0==my_pad)
		{
			if (my_pad_alter & PSP_KEY_SHOT_OK)
			{
				voice_play(VOICE01_MENU_OK/*VOICE02_MENU_SELECT*/, TRACK01_EXPLODE);
				m->menu_state		= MENU_STATE_02_FADE_OUT;/* メニュー消去準備 */
				m->time_out_flag	= M1_NOT_TIME_OUT;/* 時間切れなし */
			}
		}
	}
	/* 時間切れありモード用、時間経過 */
	if (0 < (m->time_out_flag) )
	{
		m->time_out_flag--;// -= FPS_MENU_FACTOR;
		//	m->time_out -= (1);
		if ((0) == m->time_out_flag)/* 時間切れ */
		{
			/* 時間切れありモードで時間切れになった */
			m->menu_state		= MENU_STATE_02_FADE_OUT;/* メニュー消去準備 */
		//	m->time_out_flag	= M2_STATE_TIME_OVER/*(-2)*/;/* 時間切れあり */
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void generic_menu_init(
	int res_num,
	MENU *m,
	int pause_filter_flag,//	int set_fade_out_flag,
	int set_time_out_flag
)/*char *options[]*/
{
	int i;
	for (i=0; i<MAX_MENU_ITEMS/*20*/; i++)
	{
		m->menu_item_surface[i] = NULL;
		{
			m->menu_item_my_obj[i].x256 			= 0;
			m->menu_item_my_obj[i].y256 			= 0;

			m->menu_item_my_obj[i].MENU_DATA_i0 	= 0;
			m->menu_item_my_obj[i].MENU_DATA_i1 	= 0;
			m->menu_item_my_obj[i].MENU_DATA_i2 	= 0;
			m->menu_item_my_obj[i].MENU_DATA_i3 	= 0;
		}
	}
	m->max_items		= 0;
	/*m->*/generic_menu_done_flag	= 0;	// ここまでMENUの初期化 */
//

	{
loop:
	if ((ST_INIT_MENU|ST_MENU_SUB_STAGE_SELECT)==(psp_loop))
	{
		if (m->max_items>(option_config[OPTION_CONFIG_07_OPEN]&0x07))	{	goto loop_quit;}
	}
		/*while*/if (NULL == /*options[m->max_items]*/my_111resource[res_num][m->max_items].str_name)
		{	goto loop_quit;}
	//	m->menu_item_surface[m->max_items] = font_render(options[m->max_items],FONT01);
		m->menu_item_surface[m->max_items] = font_render(/*options[m->max_items]*/
			(char *)my_111resource[res_num][m->max_items].str_name,FONT03);
//		SDL_SetColorKey(m->menu_item_surface[m->max_items], SDL_SRCCOLORKEY|SDL_RLEACCEL, 0x00000000);
//		/* カラーキー(抜き色、透明色)は黒 */

			m->menu_item_my_obj[m->max_items].x256	= t256(0);
			m->menu_item_my_obj[m->max_items].y256	= t256(0);

		m->max_items++;
		goto loop;
	}
loop_quit:
	for (i=0; i<m->max_items; i++)
	{
		m->menu_item_my_obj[i].MENU_DATA_i0 	= my_111resource[res_num][i/*m->max_items*/].x_offset/*(PSP_WIDTH480/2)-(m->menu_item_my_obj[i]->w/2)*/;
//		int jjj = ((m->menu_item_my_obj[i]->SDL_h128)>>7)+5;
		int jjj = 5+16;
		m->menu_item_my_obj[i].MENU_DATA_i1 	= (PSP_HEIGHT272/2+40) -((jjj)*((m->max_items)>>1)) +(i*(jjj));
		m->menu_item_my_obj[i].MENU_DATA_i2 	= 0;
		m->menu_item_my_obj[i].MENU_DATA_i3 	= 0;
	}


	#if 1
	/* てすと、構造的問題 */
	//if (RES09_PAUSE_MENU==res_num)
	if (0!=pause_filter_flag)
	{
		psp_pause_filter(); 	/* ポーズフィルター予定 */
	}
	#endif
	{
		psp_push_screen();
	}
	m->menu_brite		= (0);
	m->menu_state		= MENU_STATE_00_FADE_IN_MENU;
//	m->fade_out_flag	= (0)/*set_fade_out_flag*/;/*構造的問題*/
	m->time_out_flag	= set_time_out_flag;
	//keybo ard_clear();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

//static u32 pause_start_time = 0;

static void pause_menu_init(void)
{
	generic_menu_init(RES09_PAUSE_MENU, &pause_menu, /*pause_filter_flag*/(1)/*1 構造的問題*/, M1_NOT_TIME_OUT/*時間切れなし*/);
//	pause_start_time	= psp_get_uint32_ticks();
}

//extern void adjust_start_time(u32 pause_time);
extern void gamecore_term(void);
static void pause_menu_work(void)
{
	if (/*!pause_menu.*/0==generic_menu_done_flag)
	{
		generic_menu_work(&pause_menu);
	}
	else
	{
//		/*pause_menu.*/generic_menu_done_flag = 0;
		switch (pause_menu.active_item)
		{
		case 0: /* Continue Game */
			#if 1/*Gu化完了したら要らなくなる*/
			{
				psp_clear_screen(); 	/* [PAUSE] 復帰時にSDL画面を消す。 */
			}
			#endif
			psp_loop = (ST_WORK_GAME_PLAY|0);
			//adjust_start_time(pause_start_time);
			break;
		case 1: /* Quit Game */
			gamecore_term();
			psp_loop = (ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);	/* メインメニューに戻る */
			break;
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

//static u32 pause_start_time = 0;
void render_continue(void/*int now_max_continue*/);
static void ask_continue_menu_init(void)
{
	render_continue(/*now_max_continue*/);
	generic_menu_init(
		RES08_ASK_CONTINUE_MENU,
		&ask_continue_menu,
		/*pause_filter_flag*/(1)/*1 構造的問題*/,
		M1_NOT_TIME_OUT/*時間切れなし*/);
//	pause_start_time	= psp_get_uint32_ticks();
}

//extern void adjust_start_time(u32 pause_time);
extern void player_continue_value(void);
static void ask_continue_menu_work(void)
{
	if (/*!ask_continue_menu.*/0==generic_menu_done_flag)
	{
		generic_menu_work(&ask_continue_menu);
	}
	else
	{
//		/*ask_continue_menu.*/generic_menu_done_flag = 0;
		switch (ask_continue_menu.active_item)
		{
		case 0: /* Continue Game */
			player_continue_value();
			#if 1/*Gu化完了したら要らなくなる*/
			{
				psp_clear_screen();	/* [PAUSE] 復帰時にSDL画面を消す。 */
			}
			#endif
			psp_loop = (ST_WORK_GAME_PLAY|0);
			//adjust_start_time(pause_start_time);
			break;
		case 1: /* Game over /Quit Game */
		//	gamecore_term();
		//	psp_loop = (ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);	/* メインメニューに戻る */
			psp_loop = (ST_WORK_GAME_OVER|0);
			break;
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

//#if (1==DEBUG_MODE)
static void stage_select_menu_init(void)
{
	generic_menu_init(RES01_STAGE_SELECT, &stage_select_menu, 0, M1_NOT_TIME_OUT/*時間切れなし*/);
	#if 1
	/*
		エキストラステージをプレイした後で、
		「エキストラステージ、プラクティス」と
		「エンディング、スタッフロール、プラクティス」が
		選べるバグがあるので、修正する。
	*/
	if ( (6-1) < continue_stage)	{continue_stage = (6-1);	/* (6-1) */}
	#endif
	stage_select_menu.active_item = continue_stage;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void stage_select_menu_work(void)
{
	if (0==generic_menu_done_flag)
	{
		generic_menu_work(&stage_select_menu);
	}
	else
	{
//		/*stage_select_menu.*/generic_menu_done_flag=0;
		continue_stage = stage_select_menu.active_item;
		/* プラクティス ゲーム開始 */
		psp_loop = (ST_INIT_MENU|ST_MENU_SUB_RANK_SELECT);
	}
}
//#endif


/*---------------------------------------------------------

---------------------------------------------------------*/

static void difficulty_menu_init(void)
{
	generic_menu_init(RES02_DIFFICULTY, &difficulty_menu, 0, M1_NOT_TIME_OUT/*時間切れなし*/);
	difficulty_menu.active_item = difficulty;
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static int is_turn_right;
int select_player;

static void difficulty_menu_work(void)
{
	if (/*!difficulty_menu.*/0==generic_menu_done_flag)
	{
		generic_menu_work(&difficulty_menu);
	}
	else
	{
//		/*difficulty_menu.*/generic_menu_done_flag = 0;
		difficulty = difficulty_menu.active_item;
		/* 通常／プラクティス、ゲーム開始 */
		// static void player_opt_init(void)
		{
			is_turn_right = 0;/* 必要 */
			select_player++;/* プログラムplayer_opt_work()の都合上調整 */	/*player_opt_work()::[init]*/
		}
		psp_loop = (ST_WORK_PLAYER_SELECT|0);
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
extern void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;				/* 背景ウィンドウ表示フラグ */

static SDL_Surface *intropic/*=NULL*/;
static void start_menu_init(void)
{
	callback_gu_draw_haikei = NULL;//dr aw_bg_screen = 0;	/* 背景ウィンドウ表示off */
	set_music_volume(128);/*とりあえず*/
	{
		if (NULL==intropic)
		{
			intropic=loadbmp0("bg/title_bg.jpg", 0, /*0*/1/*1*/);
		}
	//	psp_clear_screen();
		SDL_SetAlpha(intropic, SDL_SRCALPHA, 255);
		SDL_BlitSurface(intropic, NULL, sdl_screen[SDL_00_VIEW_SCREEN], NULL);
		unloadbmp_by_surface(intropic); 	intropic = NULL;/*unload_bmp_by_name("bg/title_bg.jpg");*/
	}
	generic_menu_init(RES00_MAIN_MENU, &start_menu, 0, M1_NOT_TIME_OUT/*時間切れなし*/);
	/* 時間切れ == 500 6000 RESULTにしたから要らなくなったが、将来デモとか付けるならいる */
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void start_menu_work(void)
{
	if (0==generic_menu_done_flag)
	{
		generic_menu_work(&start_menu);
	}
	else
	{
		#if 0/*RESULTにしたから要らなくなったが、将来デモとか付けるならいる*/
		if (-3 == start_menu.time_out)
		{
			/* 時間切れで、強制的にデモ表示  / Timeout, go on and show HighScore List */
			psp_loop = (ST_WORK_RESULT|0);
		}
		else
		#endif
		{
//			generic_menu_done_flag = 0;
			switch (start_menu.active_item)
			{
			case MAIN_MENU_START:			practice_mode = 0;	continue_stage = (1-1); 	/*(0)*/ 		/* STAGE1 == 0 == (1-1)*/
											psp_loop = (ST_INIT_MENU|ST_MENU_SUB_RANK_SELECT);	break; /* Start */
			case MAIN_MENU_EXTRA_START: 	practice_mode = 0;	continue_stage = (8-1); 	/*(6)*/ 	/* STAGE8 == 7 == (8-1)*/
											psp_loop = (ST_INIT_MENU|ST_MENU_SUB_RANK_SELECT);	break; /* Extra Start */
			case MAIN_MENU_PRACTICE_START:	practice_mode = 1;
											psp_loop = (ST_INIT_MENU|ST_MENU_SUB_STAGE_SELECT); break; /* Practice Start */
		//	case MAIN_MENU_REPLAY:																break; /* Replay */
			case MAIN_MENU_STORY:			psp_loop = (ST_WORK_STORY|0);						break; /* Story */
			case MAIN_MENU_RESULT:			psp_loop = (ST_WORK_RESULT|0);						break; /* Result */
		//	case MAIN_MENU_MUSIC_ROOM:															break; /* Music Room */
			case MAIN_MENU_OPTION:			psp_loop = (ST_WORK_OPTION_MENU|0); 				break; /* Option */
			case MAIN_MENU_QUIT:			psp_loop = (ST_PSP_QUIT);							break; /* Quit */
			}
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

void all_menu_init(void)
{
	switch ((u8)(psp_loop&0xff))
	{
	case ST_MENU_SUB_MAIN_MENU: 	start_menu_init();			break;
	case ST_MENU_SUB_PAUSE: 		pause_menu_init();			break;
	case ST_MENU_SUB_ASK_CONTINUE:	ask_continue_menu_init();	break;
	case ST_MENU_SUB_STAGE_SELECT:	stage_select_menu_init();	break;
	case ST_MENU_SUB_RANK_SELECT:	difficulty_menu_init(); 	break;
	}
	psp_loop += (0x0100);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

void all_menu_work(void)
{
	switch ((u8)(psp_loop&0xff))
	{
	case ST_MENU_SUB_MAIN_MENU: 	start_menu_work();			break;
	case ST_MENU_SUB_PAUSE: 		pause_menu_work();			break;
	case ST_MENU_SUB_ASK_CONTINUE:	ask_continue_menu_work();	break;
	case ST_MENU_SUB_STAGE_SELECT:	stage_select_menu_work();	break;
	case ST_MENU_SUB_RANK_SELECT:	difficulty_menu_work(); 	break;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

void menusystem_init(void)
{
	continue_stage					= (1-1);	/* (0) */
//	practice_mode					= 0;
//	volume							= 0;
//	zanki							= 2;
//
	start_menu.active_item			= 0;
	pause_menu.active_item			= 0;
	ask_continue_menu.active_item	= 0;
	stage_select_menu.active_item	= (1-1);	/* (0) */ /*continue_stage*/;
	difficulty_menu.active_item 	= 0;/*(初期値はeasy)*/
}




/*---------------------------------------------------------

---------------------------------------------------------*/
extern void ini_save(void); 	// [***090115
/******自分でコンパイルする人へ******
このままだと配布バイナリと違うので
文字列"無駄"を適当に変えといて下さい。
************************************/
/* チルド (chilled) は、冷却されること。
「冷やす」を意味する英語の動詞チル (chill) の過去分詞である。
何で chillno じゃなくて cirno なんだろな？ */

//#define YUYUKO_PASSWORD  "TABERARENAINO9"
//#define CIRNO_PASSWORD	 "CIRNO9PASSWORD"
//#define REMILIA_PASSWORD "RUMIAISMYBRIDE"

//	/*static*/ char str_pa ss_word[32/*20*/];		/* [***090222 */
void game_clear_set_password(void)
{
#if 0
	if (0 < difficulty)/* NORMAL 以上でクリアせよ */
	{
		/* [***090222	追加 */
		if (0==tiny_strcmp(str_pass_word, YUYUKO_PASSWORD)) /* 既にクリアしてたら */
		{
		/*	strcpy(str_pass_word, FURAN_PASSWORD)*/;	/* ??? ＆ 幽々子 ＆ チルノ ＆ レミリア 開放 */
		}
		else
		if (0==tiny_strcmp(str_pass_word, CIRNO_PASSWORD))	/* 既にクリアしてたら */
		{
			strcpy(str_pass_word, YUYUKO_PASSWORD); /* 幽々子 ＆ チルノ ＆ レミリア 開放 */
		}
		else
		if (0==tiny_strcmp(str_pass_word, REMILIA_PASSWORD)) /* 既にクリアしてたら */
		{
			strcpy(str_pass_word, CIRNO_PASSWORD);	/* チルノ ＆ レミリア 開放 */
		}
		else
		{
			strcpy(str_pass_word, REMILIA_PASSWORD);	/* レミリア 開放 */
		}
	}
#endif
	{
		ini_save();
	}
}

#define SP_X 250


//player select用

#define MAX_PLAYER (8/*5*/)/*4*/

#define BASE_SP_BG_PNG		(MAX_PLAYER*0)
#define BASE_SP_ST_PNG		(MAX_PLAYER*1)
#define BASE_SP_PNG_MAX 	(MAX_PLAYER*2)

/*---------------------------------------------------------

---------------------------------------------------------*/

static /*const*/ char *player00_res[(BASE_SP_PNG_MAX)] =
{
	/*	0 REIMU */		"select/bg00.jpg",	//	"select/bg_re.jpg", 	// "select/sp_reimu_bg.jpg",
	/*	1 REIMU */		"select/bg01.jpg",	//	"select/bg_re.jpg", 	// "select/sp_reimu_bg.jpg",
	/*	2 MARISA */ 	"select/bg02.jpg",	//	"select/bg_ma.jpg", 	// "select/sp_marisa_bg.jpg",
	/*	3 MARISA */ 	"select/bg03.jpg",	//	"select/bg_ma.jpg", 	// "select/sp_marisa_bg.jpg",
	/*	4 REMILIA */	"select/bg04.jpg",	//	"select/bg_ma.jpg", 	// "select/sp_marisa_bg.jpg",
	/*	5 YUYUKO */ 	"select/bg05.jpg",	//	"select/bg_oz.jpg", 	// "select/sp_remiria_bg.jpg",
	/*	6 CIRNO */		"select/bg06.jpg",	//	"select/bg_ci.jpg", 	// "select/sp_cirno_bg.jpg",
	/*	7 CIRNO */		"select/bg07.jpg",	//	"select/bg_yu.jpg", 	// "select/sp_yuyuko_bg.jpg",
//
	/*	8 REIMU */		"select/pl00.png",	//	"select/p_re.png",		// "select/sp_reimu_st.png",
	/*	9 REIMU */		"select/pl01.png",	//	"select/p_re.png",		// "select/sp_reimu_st.png",
	/* 10 MARISA */ 	"select/pl02.png",	//	"select/p_ma.png",		// "select/sp_marisa_st.png",
	/* 11 MARISA */ 	"select/pl03.png",	//	"select/p_ma.png",		// "select/sp_marisa_st.png",
	/* 12 REMILIA */	"select/pl04.png",	//	"select/p_ma.png",		// "select/sp_marisa_st.png",
	/* 13 YUYUKO */ 	"select/pl05.png",	//	"select/p_oz.png",		// "select/sp_remiria_st.png",
	/* 14 CIRNO */		"select/pl06.png",	//	"select/p_ci.png",		// "select/sp_cirno_st.png",
	/* 15 CIRNO */		"select/pl07.png",	//	"select/p_yu.png",		// "select/sp_yuyuko_st.png",
};


/*---------------------------------------------------------

---------------------------------------------------------*/

void menu_cancel_and_voice(void)
{
	voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* 自機死に音は、なるべく重ねない */
	/* メインメニューに戻る */
	psp_loop = (ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);
}

/*---------------------------------------------------------

---------------------------------------------------------*/
		#if 0
// 0000 0001 霊夢 A
// 0000 0010 霊夢 B
// 0000 0100 魔理沙 A
// 0000 1000 魔理沙 B

// 0001 0000 レミリア
// 0010 0000 幽々子
// 0100 0000 チルノ A
// 1000 0000 チルノ Q
		if (0==tiny_strcmp(str_pass_word, YUYUKO_PASSWORD)) 	/* [***090222	追加 */
		{
			can_player_bit=0x7f/*MAX_PLAYER-1*/;	/* 幽々子 開放 */
		}
		else
		if (0==tiny_strcmp(str_pass_word, CIRNO_PASSWORD))		/* [***090222	追加 */
		{
			can_player_bit=0x5f/*MAX_PLAYER-2*/;	/* 魔理沙 B / チルノ A 開放 */
		}
		else
		if (0==tiny_strcmp(str_pass_word, REMILIA_PASSWORD))	/* [***090222	追加 */
		{
			can_player_bit=0x17/*MAX_PLAYER-3*/;	/* 霊夢 B / レミリア 開放 */
		}
		else
		{
			can_player_bit=0x05/*MAX_PLAYER-4*/;	/* 霊夢 A / 魔理沙 A のみ */
		}
		#endif


/*---------------------------------------------------------

---------------------------------------------------------*/

static int sp_diff256;


enum
{
	PLAYER_SELECT_00_LOAD = 0,
	PLAYER_SELECT_01_MOVE_DRAW,
	PLAYER_SELECT_02_SELECT,
};

			#if 0/* [描画2] */
			SDL_SetAlpha(player_select_bg0_surface, SDL_SRCALPHA, 255);
			SDL_BlitSurface(player_select_bg0_surface,NULL,sdl_screen[SDL_00_VIEW_SCREEN],NULL);
			{
				SDL_Rect dr;
				dr.x	= SP_X;
				dr.y	= (50);
				SDL_BlitSurface(player_select_fg0_surface,NULL,sdl_screen[SDL_00_VIEW_SCREEN],&dr);
			}
			#endif

void player_opt_work(void)
{
	static SDL_Surface *player_select_bg0_surface;
	static SDL_Surface *player_select_bg2_surface;
	static SDL_Surface *player_select_fg0_surface;
	static SDL_Surface *player_select_fg2_surface;
	static int sp_scale256;
//	psp_clear_screen();

	if ((ST_WORK_PLAYER_SELECT|PLAYER_SELECT_00_LOAD)==psp_loop)/* [load] */
	{
		select_player	&= 7;
//
		player_select_bg2_surface=loadbmp0( (char *)player00_res[BASE_SP_BG_PNG+select_player], 0, 0/*1*/);
		player_select_fg2_surface=loadbmp0( (char *)player00_res[BASE_SP_ST_PNG+select_player], 0, 0/*1*/);
		SDL_SetColorKey(player_select_fg2_surface, (SDL_SRCCOLORKEY), 0x00000000);
		if (0==is_turn_right)
				{	select_player--;	}
		else	{	select_player++;	}
		select_player &= 0x07;
		player_select_bg0_surface=loadbmp0( (char *)player00_res[BASE_SP_BG_PNG+select_player], 0, 0/*1*/);
		player_select_fg0_surface=loadbmp0( (char *)player00_res[BASE_SP_ST_PNG+select_player], 0, 0/*1*/);
		SDL_SetColorKey(player_select_fg0_surface, (SDL_SRCCOLORKEY), 0x00000000);
//
		sp_scale256 	= (255);
		sp_diff256   	= (0);
		psp_loop++;
	}
	else
	if ((ST_WORK_PLAYER_SELECT|PLAYER_SELECT_01_MOVE_DRAW)==psp_loop)/* [描画1] */
	{
		sp_scale256 -= 15/*18*/;
		if (sp_scale256 < 0 )
		{
			sp_scale256 = (0);
			psp_loop++;/* [選択] */
		}
		SDL_SetAlpha(player_select_bg0_surface, SDL_SRCALPHA, (255-sp_scale256)/*bg_alpha_bbb*/);
		SDL_BlitSurface(player_select_bg2_surface,NULL,sdl_screen[SDL_00_VIEW_SCREEN],NULL);
		SDL_BlitSurface(player_select_bg0_surface,NULL,sdl_screen[SDL_00_VIEW_SCREEN],NULL);
		{
			SDL_Rect dr;
	 		dr.y = (50);
			SDL_Surface *src;
		//	player_opt_img256(player_select_fg2_surface, (256-sp_scale256), (  is_turn_right)/*1*/);	/* 右1 左0 */
		//	player_opt_img256(player_select_fg0_surface, (	  sp_scale256), (1-is_turn_right)/*0*/);	/* 右0 左1 */
		//static void player_opt_img256() /* 0==r, 1==l, */
			{int aaa;
				sp_diff256+=(sp_scale256);
				aaa=(((sp_diff256))>>8)-16;
				if ((  is_turn_right)) {   aaa = -(aaa);}
				dr.x = SP_X+(aaa);
			}
			src 		= player_select_fg2_surface;
			dr.w = (src->w);
			dr.h = (src->h);
		//	SDL_SetColorKey(src, SDL_SRCCOLORKEY, 0x00000000);
			SDL_SetAlpha(src, SDL_SRCALPHA, (	  sp_scale256));
			SDL_BlitSurface(src, NULL, sdl_screen[SDL_00_VIEW_SCREEN], &dr);
			{int aaa;
				sp_diff256+=(sp_scale256);
				aaa=(((sp_diff256))>>8)-16;
				if ((1-is_turn_right)) {   aaa = -(aaa);}
				dr.x = SP_X+(aaa);
			}
			src 		= player_select_fg0_surface;
			dr.w = (src->w);
			dr.h = (src->h);
		//	SDL_SetColorKey(src, SDL_SRCCOLORKEY, 0x00000000);
			SDL_SetAlpha(src, SDL_SRCALPHA, (255-sp_scale256));
			SDL_BlitSurface(src, NULL, sdl_screen[SDL_00_VIEW_SCREEN], &dr);
		}
	}
	else
	if ((ST_WORK_PLAYER_SELECT|PLAYER_SELECT_02_SELECT)==psp_loop)/* [選択] */
	{
		if (0==my_pad)
		{
			if (my_pad_alter & (PSP_KEY_LEFT|PSP_KEY_RIGHT|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL)) /* 左か右かショットかキャンセルのいづれか */
			{	/* 状態が変わる場合で*/
				/* 状態が変わる場合、ちゃんと SDLサーフェイスを開放 しないとメモリーリークするよ */
				unloadbmp_by_surface(player_select_bg0_surface);	player_select_bg0_surface = NULL;
				unloadbmp_by_surface(player_select_fg0_surface);	player_select_fg0_surface = NULL;
				unloadbmp_by_surface(player_select_bg2_surface);	player_select_bg2_surface = NULL;
				unloadbmp_by_surface(player_select_fg2_surface);	player_select_fg2_surface = NULL;
			//
				if (my_pad_alter & (PSP_KEY_LEFT|PSP_KEY_RIGHT)/*左か右の両方*/  ) /* 左右ボタン入力 */
				{
					is_turn_right=(my_pad_alter & PSP_KEY_RIGHT)?1:0;/* 右なら1, 左なら0 */
					voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
					psp_loop=(ST_WORK_PLAYER_SELECT|PLAYER_SELECT_00_LOAD); 	/* [load] */
				}
				if (my_pad_alter & PSP_KEY_SHOT_OK)
				{
					voice_play(VOICE01_MENU_OK, TRACK01_EXPLODE);/* テキトー */
					psp_loop = (ST_WORK_STAGE_FIRST|0);
				}
				else
				if (my_pad_alter & PSP_KEY_BOMB_CANCEL)
				{
					menu_cancel_and_voice();	/* メインメニューに戻る */
				}
			}
		}
	}
}
