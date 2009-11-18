
#include "game_main.h"

/*---------------------------------------------------------

---------------------------------------------------------*/

//extern SDL_Surface *back_screen;

static int volume/*=2*/; /* 0-3 */
/*static*/ int zanki/*=2*/; /* 0-3 */

int continue_stage;
int practice_mode;
int boss_bgm_mode;

/*---------------------------------------------------------

---------------------------------------------------------*/
#define MAX_MENU_ITEMS (8)

//	SDL_Surface 	*bg;
typedef struct
{
	SPRITE			*menu_item_sprite[MAX_MENU_ITEMS/*20*/]/*[5]*/; 	// 項目、アニメーション(残像)レイヤー
	SDL_Surface 	*menu_item_surface[MAX_MENU_ITEMS/*20*/];			// 画像化した各項目???
	int active_item;	/* 現在メニュー上で選択されている項目番号 */
	int max_items;		// オプションの項目数
//
	int alpha;
	int menu_state; 	// 各項目のα値の状態。フェードin or out or done???
	int fade_out;
	int time_out;
//
} MENU;
static int generic_menu_done_flag;	/* 共通メニュー使用中か否かのフラグ */

static MENU start_menu;

static MENU pause_menu;
static MENU ask_continue_menu;

static MENU stage_select_menu;
static MENU difficulty_menu;


//static MENU option_menu;
//static MENU volume_menu;
//static MENU zanki_menu;
//static MENU player_sl_menu;

/*---------------------------------------------------------

---------------------------------------------------------*/

enum
{
	GENERIC_MENU_STATE_00_INIT = 0,
	GENERIC_MENU_STATE_01_GENERIC,
	GENERIC_MENU_STATE_02_DONE,
	GENERIC_MENU_STATE_99_MAX
};

enum
{
	MENU_STATE_00_INIT_MENU = 0,
	MENU_STATE_00_FADE_IN_MENU,
	MENU_STATE_01_WORK_MENU,
	MENU_STATE_02_FADE_OUT,
	MENU_STATE_03_FININSH,
	MENU_STATE_04_BRANCH,
	MENU_STATE_99_MAX
};

enum
{
	RES00_MAIN_MENU=0,
	RES01_STAGE_SELECT,
	RES02_DIFFICULTY,
//	RES03_OPTION,
//	RES04_VOLUME,
//	RES05_ZANKI,
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
	//	#if (1==US E_ENDING_DEBUG)
	//	{	250,		"ENDING DEBUG", 	},/* 330 */
	//	#else
		{	250,		"EXTRA START",		},/* 330 */
	//	#endif //(1==US E_ENDING_DEBUG)
		{	245,		"PRACTICE START",	},
	//	{	400,		"REPLAY",			},
		{	320,		"STORY",			},		//ちょっと変更
		{	285,		"RESULT",			},
	//	{	310,		"MUSIC ROOM",		},
		{	335,		"OPTION",			},
		{	310,		"QUIT", 			},
		{	0,			NULL,				},
	},
	{	//	RES01_STAGE_SELECT	/*const*/ char *stage_select_menu_options[] =/*(char *)*/stage_select_menu_options
		{	360,		"1",			},	//stage1
		{	350,		"2",			},	//stage2
		{	340,		"3",			},	//stage3
		{	330,		"4",			},	//stage4
		{	320,		"5",			},	//stage5
		{	310,		"6",			},	//stage6
	//	#if (1==US E_ENDING_DEBUG)
	//	{	275,		"ENDING",		},	//ending
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
//	{	//	RES03_OPTION			//	/*const*/ char *option_menu_options[] = /*(char *)*/main_menu_options
//	//	"DIFFICULTY",
//		{	200,		"KEY CONFIG",	},
//	//	#if (1==DEBUG_MODE)
//	//	"STAGE SELECT0",
//	//	#endif
//		{	180,		"SOUND VOLUME", },
//	//	{	200,		"HIGH SCORE",	},		//変更
//		{	200,		"PLAYER",		},		//変更
//		{	245,		"QUIT", 		},
//		{	200,NULL,		},
//	},
//	{	//	RES04_VOLUME	/*const*/ char *volume_menu_options[] =/*(char *)*/volume_menu_options
//		{	212,		"100",			},
//		{	220,		"75",			},
//		{	220,		"25",			},
//		{	227,		"0",			},
//		{	0,NULL, 	},
//	},
//	{	//	RES05_ZANKI /*const*/ char *zanki_menu_options[] =/*(char *)*/zanki_menu_options
//		{	227,		"2",			},
//		{	227,		"3",			},
//		{	227,		"4",			},
//		{	227,		"5",			},
//		{	0,NULL, 	},
//	},
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

typedef struct	// たぶん矩形データ用?
{
	int i0; 	// 表示用 x オフセット
	int i1; 	// 表示用 y オフセット
	int i2; 	// スプライト用(メニュー選択時、横 x の揺れ幅)
	int i3; 	// スプライト用(揺れ幅が徐々に戻る用)
} MENU_DATA;

#if 0
	extern dou ble fps_fa ctor;
	#define FPS_MENU_FACTOR 	(fps_fa ctor)
	#define FPS_MENU_FACTOR10	(10/fps_fa ctor)/*←どうも不安定(Division by Zero ???)*/
	#define FPS_MENU_FACTOR8	(8*fps_fa ctor)
	#define FPS_MENU_FACTOR15	(15*fps_fa ctor)
#else
	#define FPS_MENU_FACTOR 	(1)
	#define FPS_MENU_FACTOR10	(16/*64*/)/* 10/FPS_MENU_FACTOR */
	#define FPS_MENU_FACTOR8	(16)/* 8*FPS_MENU_FACTOR */
	#define FPS_MENU_FACTOR15	(3/*2*/)
#endif

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
		if (m->time_out>0)
		{
			m->time_out -= FPS_MENU_FACTOR;
			// m->time_out-=1;
		}
		if (m->fade_out)
		{
			psp_clear_screen();
			SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN],SDL_SRCALPHA,255-(m->alpha/2));
		}
		psp_pop_screen();	//SDL_BlitSurface(sdl_screen[SDL_01_BACK_SCREEN],NULL,screen,NULL);


	//	static int www=10;

		switch (m->menu_state)
		{
		case MENU_STATE_00_FADE_IN_MENU: /* Increasing alpha, fade menu in */
			m->alpha += FPS_MENU_FACTOR8;
			if (m->alpha>=255)
			{
				m->menu_state = MENU_STATE_01_WORK_MENU;
				m->alpha=255;
			}
			break;
		case MENU_STATE_01_WORK_MENU: /* fadein complete */
		//if ( (m->menu_state<2))
		{
			if (0==my_pad)
			{
				if (my_pad_alter & (PSP_KEY_DOWN|PSP_KEY_UP|PSP_KEY_PAUSE|PSP_KEY_RIGHT))
				{
					#if (0==USE_DESIGN_TRACK)
					play_voice_auto_track(VOICE02_MENU_SELECT);
					#else
					voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
					#endif
				}
				if (my_pad_alter & PSP_KEY_DOWN)
				{
//					play_voice_auto_track(VOICE02_MENU_SELECT);
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
								if (6>(option_config[OPTION_CONFIG_07_OPEN] & 0x07))
								{
									m->active_item++;
								}
							}
						}
						m->active_item++;
					}
				//	www=FPS_MENU_FACTOR10;
					m->time_out = -1;
				}
				else if (my_pad_alter & PSP_KEY_UP)
				{
//					play_voice_auto_track(VOICE02_MENU_SELECT);
					if (0 == m->active_item)
					{	m->active_item = m->max_items-1;}
					else
					{
						if ((ST_WORK_MENU|ST_MENU_SUB_MAIN_MENU)==(psp_loop))
						{
							if ((MAIN_MENU_EXTRA_START+1)==m->active_item)
							{
								if(6>(option_config[OPTION_CONFIG_07_OPEN] & 0x07))
								{
									m->active_item--;
								}
							}
						}
						m->active_item--;
					}
				//	www=FPS_MENU_FACTOR10;
					m->time_out = -1;
				}
				if ((ST_WORK_MENU|ST_MENU_SUB_PAUSE)==(psp_loop)/*state.substate==ST_MENU_SUB_PAUSE*/) // ポーズメニューの場合は
				{
					if (my_pad_alter & PSP_KEY_PAUSE)	// startボタンでポーズ解除
					{
//						play_voice_auto_track(VOICE02_MENU_SELECT);
						pause_menu.active_item = 0;
						m->menu_state = MENU_STATE_02_FADE_OUT;
						m->time_out = -1;
					}
					/* ボスデバッグ用 */
					#if 0/*(1==DEBUG_MODE)*/
					if (my_pad_alter & PSP_KEY_RIGHT)
					{
//						play_voice_auto_track(VOICE02_MENU_SELECT);
						now_max_continue = 90;/*test*/	/* ランキングにさせない */
//						((PLAYER_DATA *)player->data)->my_score=9;/*test*/
//						((PLAYER_DATA *)player->data)->zanki=9;/*test*/
					//	((PLAYER_DATA *)player->data)->bombs=9;/*test*/
					//	#if 0
					//	/* ボスチェック用 */
					//	((PLAYER_DATA *)player->data)->weapon_power=127;/*test*/
					//	#endif
					}
					#endif
				}
			}
		}
			break;
		case MENU_STATE_02_FADE_OUT:	/* selection done, decreasing alpha, fade menu out */
			m->alpha -= FPS_MENU_FACTOR8;
			if (m->alpha<=0)
			{
				m->menu_state	= MENU_STATE_03_FININSH;
				m->alpha		= 0;
			}
			break;
		case MENU_STATE_03_FININSH: 	/* fadout fininshed, menu done */
			SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN],SDL_SRCALPHA,255);
			psp_pop_screen();	//SDL_BlitSurface(sdl_screen[SDL_01_BACK_SCREEN],NULL,screen,NULL);
			//SDL_FreeSurface(sdl_screen[SDL_01_BACK_SCREEN]);
			{	int i;
				for (i=0; i<m->max_items; i++)
				{
					//for (j=0;j<5;j++)
					{
						m->menu_item_sprite[i]/*[j]*/->type = SP_DELETE;
					}
					//SDL_FreeSurface(m->menu_item_surface[i]);
				}
			}
			if (-2 == m->time_out) { m->time_out = -3; }
			/*m->*/generic_menu_done_flag = 1;
		//	www=FPS_MENU_FACTOR10;
			return;
		}
//
		{
			static int angle512/*=0*/;
			angle512 += FPS_MENU_FACTOR15;
			mask512(angle512);//if (angle>=360) {	angle-=360;}		// angle%=360;
			int i;
			for (i=0;i<m->max_items;i++)
			{
				MENU_DATA *data;
				data=(MENU_DATA *)m->menu_item_sprite[i]/*[0]*/->data;
				if (i==m->active_item)	/* 選択された */
				{
					data->i2 = 12; /* ゆれ幅を１２にする */
				}
				else	/* 選択されてない */
				{
					if ((data->i2) && (0==data->i3))
					{
						data->i2--;
						data->i3 = 3;
					}
				}
				if (data->i3)
				{	data->i3--;}
				m->menu_item_sprite[i]/*[0]*/->x256 = (data->i0*256)+(cos512((angle512))*data->i2);
			//	m->menu_item_sprite[i]/*[0]*/->y256 = (data->i1*256)+(sin512((angle512))*data->i2);
				m->menu_item_sprite[i]/*[0]*/->y256 = (data->i1*256);
			//	int j;
			//	for (j=4;j>0;j--)
			//	{
			//		m->menu_item_sprite[i]/*[j]*/->alpha		= m->alpha;
					m->menu_item_sprite[i]/*[j]*/->color32		= (((m->alpha)<<24)|(0x00ffffff));
			//		m->menu_item_sprite[i][j]->x256 = m->menu_item_sprite[i][j-1]->x256;
			//		m->menu_item_sprite[i][j]->y256 = m->menu_item_sprite[i][j-1]->y256;
			//		if ((m->menu_item_sprite[i][j]->x256 > 0)&&
			//			(m->menu_item_sprite[i][j]->y256 > 0))
					{	m->menu_item_sprite[i]/*[j]*/->flags |= (SP_FLAG_VISIBLE);}
			//	}
			}
		}
		sprite_work000(SP_GROUP_PAUSE_OBJS/*SP_GROUP_TEXTS*/);
		sprite_display000(SP_GROUP_PAUSE_OBJS/*SP_GROUP_TEXTS*/);
		if (0==my_pad)
		{
			if (my_pad_alter & PSP_KEY_SHOT_OK)
			{
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE01_MENU_OK/*VOICE02_MENU_SELECT*/);
				#else
				voice_play(VOICE01_MENU_OK/*VOICE02_MENU_SELECT*/, TRACK01_EXPLODE);
				#endif
				m->menu_state	= MENU_STATE_02_FADE_OUT;/* メニュー消去準備 */
				m->time_out 	= -1;/*時間切れなし*/
			}
		}
		if (0==m->time_out)/*時間切れ*/
		{
			m->menu_state	= MENU_STATE_02_FADE_OUT;/* メニュー消去準備 */
			m->time_out 	= -2;/*時間切れあり*/
		}
	}
	//parsys_display();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void generic_menu_init(int res_num/*char *options[]*/, MENU *m, int fade_out, int time_out)
{
	int i;
//	int j;
	MENU_DATA *d;
	for (i=0; i<MAX_MENU_ITEMS/*20*/; i++)
	{
		m->menu_item_surface[i] = NULL;
	//	for (j=0;j<5;j++)
		{ m->menu_item_sprite[i]/*[j]*/ = NULL; }
	}
//	sdl_screen[SDL_01_BACK_SCREEN] = NULL;
	m->max_items		= 0;
	/*m->*/generic_menu_done_flag	= 0;	//ここまでMENUの初期化
//
	static IMAGE_RESOURCE my_resource[1] =
	{
		{
			NULL,/*dummy*/
			0,/*dummy*/
			1,
			iyx(1,1),
			PRIORITY_04_ITEM,/*???*/	//	/*j+*/5, /* ????  PRIORITY_*/
			0, 0, 0
		}
	};
//
	{
loop:
	if ((ST_INIT_MENU|ST_MENU_SUB_STAGE_SELECT)==(psp_loop))
	{
		if (m->max_items>(option_config[OPTION_CONFIG_07_OPEN]&0x07))	{	goto loop_quit;}
	}
	/*while*/if (NULL == /*options[m->max_items]*/my_111resource[res_num][m->max_items].str_name)	{	goto loop_quit;}
	//	m->menu_item_surface[m->max_items] = font_render(options[m->max_items],FONT01);
		m->menu_item_surface[m->max_items] = font_render(/*options[m->max_items]*/	(char *)my_111resource[res_num][m->max_items].str_name,FONT03);
	//	for (j=0;j<5;j++)		//各スプライトに情報を与えてていく
		{
			m->menu_item_sprite[m->max_items]/*[j]*/		=
			sprite_add_res_list(
				m->menu_item_surface[m->max_items],
			//	1,
			//	1,
			//	1,
			//	/*j+*/5, /* ????  PRIORITY_*/
				SP_FLAG_NOT_CACHE/*1*/,
		//	0	/*anime_speed*/
				(IMAGE_RESOURCE *)my_resource
			);
			d												= mmalloc(sizeof(MENU_DATA));
			m->menu_item_sprite[m->max_items]/*[j]*/->data	= d;
			m->menu_item_sprite[m->max_items]/*[j]*/->type	= SP_MENU_TEXT;
		//	m->menu_item_sprite[m->max_items]/*[j]*/->alpha = 0x00;
			m->menu_item_sprite[m->max_items]/*[j]*/->color32 = 0x00ffffff;
			m->menu_item_sprite[m->max_items]/*[j]*/->x256	= t256(0);
			m->menu_item_sprite[m->max_items]/*[j]*/->y256	= t256(0);
		}
		m->max_items++;
		goto loop;
	}
loop_quit:
	for (i=0; i<m->max_items; i++)
	{
	//	for (j=0;j<5;j++)
		{
			d		= (MENU_DATA *)m->menu_item_sprite[i]/*[j]*/->data;
			d->i0	= my_111resource[res_num][i/*m->max_items*/].x_offset/*(PSP_WIDTH480/2)-(m->menu_item_sprite[i][j]->w/2)*/;
			int jjj = ((m->menu_item_sprite[i]/*[j]*/->h128)>>7)+5;
			d->i1	= (PSP_HEIGHT272/2+40) -((jjj)*((m->max_items)>>1)) +(i*(jjj));
			d->i2	= 0;
			d->i3	= 0;
		}
	}

//	sdl_screen[SDL_01_BACK_SCREEN]=SDL_CreateRGBSurface(SDL_HWSURFACE,PSP_WIDTH480,PSP_HEIGHT272,
//		screen->format->BitsPerPixel,
//		screen->format->Rmask,
//		screen->format->Gmask,
//		screen->format->Bmask,
//		screen->format->Amask);
	psp_push_screen();	//SDL_BlitSurface(screen,NULL,sdl_screen[SDL_01_BACK_SCREEN],NULL);

	m->alpha		= 0;
	m->menu_state	= MENU_STATE_00_FADE_IN_MENU;
	m->fade_out 	= fade_out;
	m->time_out 	= time_out;
	//keybo ard_clear();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

//static Uint32 pause_start_time = 0;

static void pause_menu_init(void)
{
	generic_menu_init(RES09_PAUSE_MENU,&pause_menu,1,-1);
//	pause_start_time	= psp_get_uint32_ticks();
}

//extern void adjust_start_time(Uint32 pause_time);
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
			psp_loop = (ST_WORK_GAME_PLAY|0);					//	newsta te(ST_GAME_PLAY,0,0);
			//adjust_start_time(pause_start_time);
			break;
		case 1: /* Quit Game */
			gamecore_term();
			psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);	/* メインメニューに戻る */	//	newsta te(/*ST_INTRO*/ST_MENU,0,1);
			break;
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

//static Uint32 pause_start_time = 0;
void render_continue(void/*int now_max_continue*/);
static void ask_continue_menu_init(void)
{
	render_continue(/*now_max_continue*/);
	generic_menu_init(RES08_ASK_CONTINUE_MENU,&ask_continue_menu,1,-1);
//	pause_start_time	= psp_get_uint32_ticks();
}

//extern void adjust_start_time(Uint32 pause_time);
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
			psp_loop = (ST_WORK_GAME_PLAY|0);					//	newsta te(ST_GAME_PLAY,0,0);
			//adjust_start_time(pause_start_time);
			break;
		case 1: /* Game over /Quit Game */
		//	gamecore_term();
		//	psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);	/* メインメニューに戻る */	//	newsta te(/*ST_INTRO*/ST_MENU,0,1);
			psp_loop=(ST_WORK_GAME_OVER|0);//newsta te(ST_GAME_OVER,0,1);
			break;
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

//	enum
//	{
//	//	OPT_MENU_DIFFICULTY = 0,
//		OPT_MENU_KEY_CONFIG = 0,
//	//	#if (1==DEBUG_MODE)
//	//	OPT_MENU_STAGE_SELECT,
//	//	#endif
//		OPT_MENU_VOLUME,
//		OPT_MENU_ZANKI,
//	//	OPT_MENU_RESULT,
//		OPT_MENU_QUIT
//	};

/*---------------------------------------------------------

---------------------------------------------------------*/

//static void option_menu_init(void)
//{
//	generic_menu_init(RES03_OPTION,&option_menu,0,-1);
//}

/*---------------------------------------------------------

---------------------------------------------------------*/

//static void option_menu_work(void)
//{
//	if (/*!option_menu.*/0==generic_menu_done_flag)
//	{
//		generic_menu_work(&option_menu);
//	}
//	else
//	{
//	//	/*option_menu.*/generic_menu_done_flag=0;
//		switch (option_menu.active_item)
//		{
//#if (0)/*???*/
//		#if (0==USE_DESIGN_TRACK)
//		play_voice_auto_track(VOICE02_MENU_SELECT);
//		#else
//		voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
//		#endif
//#endif
//	//	case OPT_MENU_DIFFICULTY:	newsta te(ST_MENU,ST_MENU_SUB_RANK_SELECT,1);				break;	/* difficulty */
//		case OPT_MENU_KEY_CONFIG:	psp_loop=(ST_WORK_KEY_CONFIG/*ST_INIT_KEY_CONFIG*/|0);		break;	/* Key Config */				//	newsta te(ST_KEY_CONFIG,0,1);
//		case OPT_MENU_VOLUME:		psp_loop=(ST_INIT_MENU|ST_MENU_SUB_VOLUME); 				break;	/* change Sound-FX Volume */	//	newsta te(ST_MENU,ST_MENU_SUB_VOLUME,1);
//		case OPT_MENU_ZANKI:		psp_loop=(ST_INIT_MENU|ST_MENU_SUB_ZANKI);					break;	/* change ZANKI */
//	//	case OPT_MENU_RESULT:		newsta te(ST_RESULT,0,1);									break;	/* Hiscore */
//		case OPT_MENU_QUIT: 		psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);		break;	/* Back to Game */		/* メインメニューに戻る */	//	newsta te(ST_MENU,0/*ST_ME NU_SUB_MAIN_MENU*/,1);
//		}
//	}
//}
	//	#if (1==DEBUG_MODE)
	//	case OPT_MENU_STAGE_SELECT: newsta te(ST_MENU,ST_MENU_SUB_STAGE_SELECT,1);				break;	/* stage select */
	//	#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

//#if (1==DEBUG_MODE)
static void stage_select_menu_init(void)
{
	generic_menu_init(RES01_STAGE_SELECT,&stage_select_menu,0,-1);
	stage_select_menu.active_item = continue_stage;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void stage_select_menu_work(void)
{
	if (/*!stage_select_menu.*/0==generic_menu_done_flag)
	{
		generic_menu_work(&stage_select_menu);
	}
	else
	{
//		/*stage_select_menu.*/generic_menu_done_flag=0;
		continue_stage = stage_select_menu.active_item;
		/* プラクティス ゲーム開始 */
	//	newsta te(ST_PLAYER_SELECT,0,1); //newsta te(ST_MENU,ST_MENU_SUB_OPTION,1);
		psp_loop=(ST_INIT_MENU|ST_MENU_SUB_RANK_SELECT);	//newsta te(ST_MENU,ST_MENU_SUB_RANK_SELECT,1);
	}
}
//#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

//static void volume_menu_init(void)
//{
//	generic_menu_init(RES04_VOLUME,&volume_menu,0,-1);
//	volume_menu.active_item = volume;
//}

/*---------------------------------------------------------

---------------------------------------------------------*/

//static void volume_menu_work(void)
//{
//	if (/*!volume_menu.*/0==generic_menu_done_flag)
//	{
//		generic_menu_work(&volume_menu);
//	}
//	else
//	{
//	//	/*volume_menu.*/generic_menu_done_flag=0;
//		volume = volume_menu.active_item;
//		//set_voice_volume(volume*40);
//		{const char vol[4] = {3*40,2*40,1*40,0*40};
//			set_voice_volume(vol[(volume&(4-1))]);
//		}
//		psp_loop=(ST_INIT_MENU|ST_MENU_SUB_OPTION); 	//newsta te(ST_MENU,ST_MENU_SUB_OPTION,1);
//	}
//}

/*---------------------------------------------------------

---------------------------------------------------------*/

//static void zanki_menu_init(void)
//{
//	generic_menu_init(RES05_ZANKI,&zanki_menu,0,-1);
//	zanki_menu.active_item = zanki;
//}

/*---------------------------------------------------------

---------------------------------------------------------*/

//static void zanki_menu_work(void)
//{
//	if (/*!zanki_menu.*/0==generic_menu_done_flag)
//	{
//		generic_menu_work(&zanki_menu);
//	}
//	else
//	{
//	//	/*zanki_menu.*/generic_menu_done_flag=0;
//		zanki=zanki_menu.active_item;
//		//set_zanki(zanki*40);
//	//	{const char vol[4] = {3*40,2*40,1*40,0*40};
//	//		set_zanki(vol[(zanki&(4-1))]);
//	//	}
//		psp_loop=(ST_INIT_MENU|ST_MENU_SUB_OPTION); 	//newsta te(ST_MENU,ST_MENU_SUB_OPTION,1);
//	}
//}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void difficulty_menu_init(void)
{
	generic_menu_init(RES02_DIFFICULTY,&difficulty_menu,0,-1);
	difficulty_menu.active_item = difficulty;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

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
	//	newsta te(ST_MENU,ST_MENU_SUB_OPTION,1);
	//	ini_save();
		/* 通常／プラクティス、ゲーム開始 */
		psp_loop=(ST_WORK_PLAYER_SELECT|0); 	//newsta te(ST_INIT_PLAYER_SELECT,0,1);
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
		SDL_BlitSurface(intropic, NULL, /*sdl_screen[SDL_01_BACK_SCREEN]*/sdl_screen[SDL_00_SCREEN], NULL);
		unloadbmp_by_surface(intropic); 	intropic = NULL;/*unload_bmp_by_name("bg/title_bg.jpg");*/
	}
	generic_menu_init(RES00_MAIN_MENU,&start_menu,0,/*6000*/-1/*500*/);/*RESULTにしたから要らなくなったが、将来デモとか付けるならいる*/
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void start_menu_work(void)
{
	if (/*!start_menu.*/0==generic_menu_done_flag)
	{
		generic_menu_work(&start_menu);
	}
	else
	{
		#if 0/*RESULTにしたから要らなくなったが、将来デモとか付けるならいる*/
		if (-3 == start_menu.time_out)
		{
			/* 時間切れで、強制的にデモ表示  / Timeout, go on and show HighScore List */
			psp_loop = (ST_WORK_RESULT|0);//newsta te(ST_RESULT,0,1);
		}
		else
		#endif
		{
//			/*start_menu.*/generic_menu_done_flag = 0;
			switch (start_menu.active_item)
			{
			case MAIN_MENU_START:			practice_mode=0;	continue_stage=0;
											psp_loop=(ST_INIT_MENU|ST_MENU_SUB_RANK_SELECT);	break; /* Start */			//	newsta te(ST_MENU,ST_MENU_SUB_RANK_SELECT,1);	/*newsta te(ST_PLAYER_SELECT,0,1); */				//newsta te(ST_GAME_PLAY,0,1);
			case MAIN_MENU_EXTRA_START: 	practice_mode=0;	continue_stage=(8-1)/*6*/;		/* STAGE8 == 7 == (8-1)*/
											psp_loop=(ST_INIT_MENU|ST_MENU_SUB_RANK_SELECT);	break; /* Extra Start */	//	newsta te(ST_PLAYER_SELECT,0,1);		//newsta te(ST_GAME_PLAY,0,1);
			case MAIN_MENU_PRACTICE_START:	practice_mode=1;
											psp_loop=(ST_INIT_MENU|ST_MENU_SUB_STAGE_SELECT);	break; /* Practice Start */ //	newsta te(ST_MENU,ST_MENU_SUB_STAGE_SELECT,1);*/ /* stage select */ //newsta te(ST_PLAYER_SELECT,0,1);	break;	//newsta te(ST_GAME_PLAY,0,1);
		//	case MAIN_MENU_REPLAY:																break; /* Replay */ 		//	newsta te(ST_MENU,ST_MENU_SUB_OPTION,1);
			case MAIN_MENU_STORY:			psp_loop=(ST_WORK_STORY|0); 						break; /* Story */			//	newsta te(ST_STORY,0,1);
			case MAIN_MENU_RESULT:			psp_loop=(ST_WORK_RESULT|0);						break; /* Result */ 		//	newsta te(ST_RESULT,0,1); /* Hiscore */
		//	case MAIN_MENU_MUSIC_ROOM:															break; /* Music Room */ 	//	newsta te(ST_MENU,ST_MENU_SUB_OPTION,1);
		//	case MAIN_MENU_OPTION:			psp_loop=(ST_INIT_MENU|ST_MENU_SUB_OPTION); 		break; /* Option */ 		//	newsta te(ST_MENU,ST_MENU_SUB_OPTION,1);
			case MAIN_MENU_OPTION:			psp_loop=(ST_WORK_OPTION_MENU|0);					break; /* Option */ 		//	newsta te(ST_MENU,ST_MENU_SUB_OPTION,1);
			case MAIN_MENU_QUIT:			psp_loop=ST_PSP_QUIT;								break; /* Quit */			//	newsta te(ST_PSP_QUIT,0,1);
			}
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

void all_menu_init(void)
{
	switch ((Uint8)(psp_loop&0xff)/*state.substate*/)
	{
	case ST_MENU_SUB_MAIN_MENU: 	start_menu_init();			break;
	case ST_MENU_SUB_PAUSE: 		pause_menu_init();			break;
	case ST_MENU_SUB_ASK_CONTINUE:	ask_continue_menu_init();	break;
//	case ST_MENU_SUB_OPTION:		option_menu_init(); 		break;
	case ST_MENU_SUB_STAGE_SELECT:	stage_select_menu_init();	break;
//	case ST_MENU_SUB_VOLUME:		volume_menu_init(); 		break;
//	case ST_MENU_SUB_ZANKI: 		zanki_menu_init();			break;
	case ST_MENU_SUB_RANK_SELECT:	difficulty_menu_init(); 	break;
	}
	psp_loop += (0x0100);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

void all_menu_work(void)
{
	switch ((Uint8)(psp_loop&0xff)/*state.substate*/)
	{
//	play_voice_auto_track(VOICE02_MENU_SELECT);/*??*/
	case ST_MENU_SUB_MAIN_MENU: 	start_menu_work();			break;
	case ST_MENU_SUB_PAUSE: 		pause_menu_work();			break;
	case ST_MENU_SUB_ASK_CONTINUE:	ask_continue_menu_work();	break;
//	case ST_MENU_SUB_OPTION:		option_menu_work(); 		break;
	case ST_MENU_SUB_STAGE_SELECT:	stage_select_menu_work();	break;
//	case ST_MENU_SUB_VOLUME:		volume_menu_work(); 		break;
//	case ST_MENU_SUB_ZANKI: 		zanki_menu_work();			break;
	case ST_MENU_SUB_RANK_SELECT:	difficulty_menu_work(); 	break;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

void menusystem_init(void)
{
	continue_stage					= 0;
//	practice_mode					= 0;
	volume							= 0;
	zanki							= 2;
//
	start_menu.active_item			= 0;
	pause_menu.active_item			= 0;
	ask_continue_menu.active_item	= 0;
	stage_select_menu.active_item	= 0/*continue_stage*/;
	difficulty_menu.active_item 	= 0;

//	option_menu.active_item 		= 0;
//	volume_menu.active_item 		= 0/*volume*/;
//	zanki_menu.active_item			= 2/*zanki*/;
//	player_sl_menu.active_item		= 0;
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

int select_player;

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


static int sp_diff256;
static void player_opt_img256(SDL_Surface *src, int scale256, int l_or_r)	//r=0, l=1
{
	SDL_Rect sr;
	SDL_Rect dr;
	sr.w = (src->w);
	sr.h = (src->h);
	sr.x = 0;
	sr.y = 0;
//	dr.w = ((src->w*scale256)>>8);
	dr.w = (src->w);
	dr.h = (src->h);
//	if (l_or_r) 	{	dr.x = SP_X;}
//	else			{	dr.x = SP_X+200-dr.w;}
	{int aaa;
	//	aaa=(((scale256))>>1);
		aaa=(((sp_diff256))>>8)-16;
	//	if (l_or_r) {;} else	{	aaa = -(aaa);}
		if (l_or_r) {	aaa = -(aaa);}
		dr.x = SP_X+(aaa);
	}
	dr.y = (50);
//	blit_scaled(src, &sr, sdl_screen[SDL_00_SCREEN], &dr);
//	SDL_SetColorKey(src, SDL_SRCCOLORKEY, 0x00000000);
	SDL_SetAlpha(src, SDL_SRCALPHA, (t256(1)-scale256));
	SDL_BlitSurface(src, &sr, sdl_screen[SDL_00_SCREEN], &dr);
}

/*---------------------------------------------------------

---------------------------------------------------------*/
void menu_cancel_and_voice(void)
{
	#if (0==USE_DESIGN_TRACK)
	play_voice_auto_track(VOICE04_SHIP_HAKAI);
	#else
	voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* 自機死に音は、なるべく重ねない */
	#endif
	/* メインメニューに戻る */
	psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);	//newsta te(ST_MENU/*ST_INTRO*/,0/*1*/,1);
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static u8 can_player_bit ;	/* 霊夢 A / 魔理沙 A のみ */		/* [***090222	追加 */

void player_opt_work(void)
{
	static SDL_Surface *player_select_bg0_surface;
	static SDL_Surface *player_select_bg2_surface;
	static SDL_Surface *player_select_fg0_surface;
	static SDL_Surface *player_select_fg2_surface;
	static int sp_scale256;
	static int is_turn_right = 0;
//	static int sp_wait = 0;
	static int bg_alpha_bbb;
	SDL_Rect sr;
	sr.x = SP_X;
	sr.y = 50;
//	psp_clear_screen();
	switch (psp_loop&0x0f)
	{
// 0000 0001 霊夢 A
// 0000 0010 霊夢 B
// 0000 0100 魔理沙 A
// 0000 1000 魔理沙 B

// 0001 0000 魔理沙 C
// 0010 0000 レミリア
// 0100 0000 チルノ
// 1000 0000 幽々子


// 0000 0001 霊夢 A
// 0000 0010 霊夢 B
// 0000 0100 魔理沙 A
// 0000 1000 魔理沙 B

// 0001 0000 レミリア
// 0010 0000 幽々子
// 0100 0000 チルノ A
// 1000 0000 チルノ Q
	case 0://init
	//	bg_alpha_bbb=0;
		sp_scale256 = t256(1);sp_diff256=0;
	//
		#if 0
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
		can_player_bit=0xff;	/* テスト */
	//	if (select_player > can_player_bit)
	//	{
	//		select_player	= 0;
	//	}
		select_player	&= 7;
		player_select_bg0_surface			= loadbmp0( (char *)player00_res[BASE_SP_BG_PNG+select_player], 0, 0/*1*/);
		player_select_fg0_surface			= loadbmp0( (char *)player00_res[BASE_SP_ST_PNG+select_player], 0, 0/*1*/);
		SDL_SetColorKey(player_select_fg0_surface, SDL_SRCCOLORKEY, 0x00000000);
		SDL_BlitSurface(player_select_bg0_surface,NULL,sdl_screen[SDL_00_SCREEN],NULL);
		{
			SDL_Rect sr;
			sr.x			= SP_X;
			sr.y			= 50;
			SDL_BlitSurface(player_select_fg0_surface,NULL,sdl_screen[SDL_00_SCREEN],&sr);
		}
		psp_loop	= (ST_WORK_PLAYER_SELECT|1)/*4*/;
		break;
	case 1://選択
	//case 4://選択
		SDL_BlitSurface(player_select_bg0_surface,NULL,sdl_screen[SDL_00_SCREEN],NULL);
		SDL_BlitSurface(player_select_fg0_surface,NULL,sdl_screen[SDL_00_SCREEN],&sr);
		if (0==my_pad)//(sp_wait<0) /*	*/
		{
			if (my_pad_alter & (PSP_KEY_LEFT|PSP_KEY_RIGHT|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL)/*左か右かショット」かキャンセルのいづれか*/  ) //左右
			{	/* 状態が変わる場合で */
				unloadbmp_by_surface(player_select_bg0_surface);	player_select_bg0_surface = NULL;
				unloadbmp_by_surface(player_select_fg0_surface);	player_select_fg0_surface = NULL;
			}
			if (my_pad_alter & (PSP_KEY_LEFT|PSP_KEY_RIGHT)/*左か右の両方*/  ) //左右ボタン入力
			{
				is_turn_right=(my_pad_alter & PSP_KEY_RIGHT)?1:0;/*右なら1, 左なら0 */
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE02_MENU_SELECT);
				#else
				voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
				#endif
				player_select_bg2_surface=loadbmp0( (char *)player00_res[BASE_SP_BG_PNG+select_player] /*sp_bg_list[select_player]*/, 0, 0/*1*/);
				player_select_fg2_surface=loadbmp0( (char *)player00_res[BASE_SP_ST_PNG+select_player] /*sp_st_list[select_player]*/, 0, 0/*1*/);
				SDL_SetColorKey(player_select_fg2_surface, SDL_SRCCOLORKEY, 0x00000000);
				if (0==is_turn_right)
				{	/* [***090222	追加 */
				//	if (0==select_player)	{	select_player=can_player_bit;	}
				//	else					{	select_player--;}
					do
					{
						select_player--;
						select_player &= 0x07;
					} while (0==((u8)((1<<select_player))&can_player_bit));
				}
				else
				{	/* [***090222	追加 */
				//	if (select_player==can_player_bit)	{	select_player=0;	}
				//	else								{	select_player++;	}
					do
					{
						select_player++;
						select_player &= 0x07;
					} while (0==(((u8)(1<<select_player))&can_player_bit));
				}
				player_select_bg0_surface=loadbmp0( (char *)player00_res[BASE_SP_BG_PNG+select_player] /*sp_bg_list[select_player]*/, 0, 0/*1*/);
				player_select_fg0_surface=loadbmp0( (char *)player00_res[BASE_SP_ST_PNG+select_player] /*sp_st_list[select_player]*/, 0, 0/*1*/);
				SDL_SetColorKey(player_select_fg0_surface, SDL_SRCCOLORKEY, 0x00000000);
				psp_loop=(ST_WORK_PLAYER_SELECT|2)/*0*/ /*(旧仕様は、右なら5, 左なら6)*/;
			//	sp_wait=10;
				sp_scale256 = t256(1);sp_diff256=0;
				bg_alpha_bbb=0;
			}
			if (my_pad_alter & PSP_KEY_SHOT_OK)
			{
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE01_MENU_OK);
				#else
				voice_play(VOICE01_MENU_OK, TRACK01_EXPLODE);/*テキトー*/
				#endif
				psp_loop=(ST_WORK_STAGE_FIRST/*ST_INIT_GAME_PLAY*/|0); //newsta te(ST_GAME_PLAY,0,1);
			}
			else
			if (my_pad_alter & PSP_KEY_BOMB_CANCEL)
			{
				menu_cancel_and_voice();	/* メインメニューに戻る */
			}
		}
		//else
		//{
		//	sp_wait-=1/**fps_fa ctor*/;
		//}
		break;
//	case 5: //右
//			player_opt_img256(player_select_fg0_surface, t256(1)-sp_scale256, 0);	// 5右0 6左1
//			player_opt_img256(player_select_fg2_surface,		 sp_scale256, 1);	// 5右1 6左0
//	case 6: //左
//			player_opt_img256(player_select_fg0_surface, t256(1)-sp_scale256, 1);
//			player_opt_img256(player_select_fg2_surface,		 sp_scale256, 0);
//	case 5: //右
	case 2:
		sp_scale256 -= t256(0.07);sp_diff256+=(sp_scale256+sp_scale256);
		bg_alpha_bbb += 15;
		if (bg_alpha_bbb < 255)
		{
			SDL_SetAlpha(player_select_bg0_surface, SDL_SRCALPHA, bg_alpha_bbb);
		}
		else if (bg_alpha_bbb==255)
		{
			;
		}
		else
		{
			bg_alpha_bbb = 255;
			SDL_SetAlpha(player_select_bg0_surface, SDL_SRCALPHA, bg_alpha_bbb);
		}
		SDL_BlitSurface(player_select_bg2_surface,NULL,sdl_screen[SDL_00_SCREEN],NULL);
		SDL_BlitSurface(player_select_bg0_surface,NULL,sdl_screen[SDL_00_SCREEN],NULL);

		if (sp_scale256 < t256(0) )
		{
			psp_loop=(ST_WORK_PLAYER_SELECT|1)/*4*/;
			unloadbmp_by_surface(player_select_bg2_surface);	player_select_bg2_surface = NULL;
			unloadbmp_by_surface(player_select_fg2_surface);	player_select_fg2_surface = NULL;
			SDL_SetAlpha(player_select_bg0_surface, SDL_SRCALPHA, 255);
		}
		else
		{
		//	player_opt_img256(player_select_fg0_surface, t256(1)-sp_scale256, (1-is_turn_right)/*0*/);	// 5右0 6左1
		//	player_opt_img256(player_select_fg2_surface,		 sp_scale256, (  is_turn_right)/*1*/);	// 5右1 6左0
			player_opt_img256(player_select_fg0_surface,		 sp_scale256, (1-is_turn_right)/*0*/);	// 5右0 6左1
			player_opt_img256(player_select_fg2_surface, t256(1)-sp_scale256, (  is_turn_right)/*1*/);	// 5右1 6左0
		}
		break;
	}
}
