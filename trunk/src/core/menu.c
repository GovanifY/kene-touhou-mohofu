
#include "support.h"

/*---------------------------------------------------------

---------------------------------------------------------*/

extern SDL_Surface *back_screen;

static int volume/*=2*/; /* 0-3 */

int continue_stage;
int practice_mode;

/*---------------------------------------------------------

---------------------------------------------------------*/
#define MAX_MENU_ITEMS (8)

typedef struct
{
	SPRITE			*menu_item_sprite[MAX_MENU_ITEMS/*20*/]/*[5]*/; 	// 項目、アニメーション(残像)レイヤー
	SDL_Surface 	*menu_item_surface[MAX_MENU_ITEMS/*20*/];			// 画像化した各項目???
//	SDL_Surface 	*bg;
	int active_item;
	int select_finish;
	int max_items;		// オプションの項目数
	int alpha;
	int alphadir;		// 各項目のα値の状態。フェードin or out or done???
	int fadeout;
	int timeout;
} MENU;

static MENU startmenu;
static MENU pausemenu;
static MENU optionmenu;
//#if (1==DEBUG_MODE)
static MENU STAGE_SELECTmenu;
//#endif
static MENU volumemenu;
static MENU difficultymenu;
//static MENU player_slmenu;

/*---------------------------------------------------------

---------------------------------------------------------*/

enum
{
	RES00_MAIN_MENU=0,
	RES01_STAGE_SELECT,
	RES02_DIFFICULTY,
	RES03_OPTION,
	RES04_VOLUME,
	RES09_PAUSE_MENU,
	RES99_MAX
};

typedef struct
{
	int x_offset;
	const char *str_name;
} MENU_RESOURCE;
//	int fadeout;
//	int timeout;
static MENU_RESOURCE my_resource[RES99_MAX][/*8*/MAX_MENU_ITEMS/*10*/] =
{
	{	//	RES00_MAIN_MENU 	//		/*const*/ char *startmenu_options[] =/*(char *)*/startmenu_options
		{	360,		"START",			},
	//	{	330,		"EXTRA START",		},
		{	245,		"PRACTICE START",	},
	//	{	400,		"REPLAY",			},
		{	320,		"STORY",			},		//ちょっと変更
		{	285,		"RESULT",			},
	//	{	310,		"MUSIC ROOM",		},
		{	335,		"OPTION",			},
		{	310,		"QUIT", 			},
		{	0,			NULL,				},
	},
	{	//	RES01_STAGE_SELECT	/*const*/ char *STAGE_SELECTmenu_options[] =/*(char *)*/STAGE_SELECTmenu_options
		{	360,		"1",			},	//stage1
		{	350,		"2",			},	//stage2
		{	340,		"3",			},	//stage3
		{	330,		"4",			},	//stage4
		{	320,		"5",			},	//stage5
		#if (1==USE_ENDING_DEBUG)
		{	275,		"ENDING",		},	//ending
		#endif //(1==USE_ENDING_DEBUG)
		{	0,NULL, 	},
	},
	{	//	RES02_DIFFICULTY		//			/*const*/ char *difficultymenu_options[] = /*(char *)*/difficultymenu_options
		{	275,		"EASY", 		},
		{	245,		"NORMAL",		},
		{	220,		"HARD", 		},
		{	185,		"LUNATIC",		},
		{	0,NULL, 	},
	},
	{	//	RES03_OPTION			//	/*const*/ char *mainmenu_options[] = /*(char *)*/mainmenu_options
	//	"DIFFICULTY",
		{	200,		"KEY CONFIG",	},
	//	#if (1==DEBUG_MODE)
	//	"STAGE SELECT",
	//	#endif
		{	180,		"SOUND VOLUME", },
	//	{	200,		"HIGH SCORE",	},		//変更
		{	245,		"QUIT", 		},
		{	200,NULL,		},
	},
	{	//	RES04_VOLUME	/*const*/ char *volumemenu_options[] =/*(char *)*/volumemenu_options
		{	212,		"100",			},
		{	220,		"75",			},
		{	220,		"25",			},
		{	227,		"0",			},
		{	0,NULL, 	},
	},
	{	//	RES09_PAUSE_MENU		//	/*const*/ char *pausemenu_options[] =/*(char *)*/pausemenu_options
		{	155,		"CONTINUE GAME",	},
		{	200,		"QUIT GAME",		},
		{	0,NULL, 	},
	},
};

/*---------------------------------------------------------

---------------------------------------------------------*/

typedef struct	//たぶん矩形データ用?
{
	int i0; 	//表示用xオフセット
	int i1; 	//y
	int i2; 	//スプライト用(メニュー選択時、横xの揺れ幅)
	int i3; 	//スプライト用()
} MENU_DATA;

#if 0
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


/*---------------------------------------------------------

---------------------------------------------------------*/

static void genericmenu_work(MENU *m)
{
//	static int www=10;
	static /*dou ble*/int angle512/*=0*/;
	int i;
//	int j;
	MENU_DATA *d;
	if (!m->select_finish)
	{
		if (m->timeout>0)
		{
			m->timeout-=FPS_MENU_FACTOR;
			// m->timeout-=1;
		}
		if (m->fadeout)
		{
			psp_clear_screen();
			SDL_SetAlpha(back_screen,SDL_SRCALPHA,255-(m->alpha/2));
		}
		psp_pop_screen();	//SDL_BlitSurface(back_screen,NULL,screen,NULL);

		if ( (m->alphadir<2))
		{
			if (0==my_pad)
			{
				if (my_pad_alter & PSP_KEY_DOWN)
				{
					play_voice_auto_track(VOICE02_ZAKO_HAKAI);
					if (m->active_item==m->max_items-1)
					{	m->active_item=0;}
					else
					{	m->active_item++;}
				//	www=FPS_MENU_FACTOR10;
					m->timeout=-1;
				}
				else if (my_pad_alter & PSP_KEY_UP)
				{
					play_voice_auto_track(VOICE02_ZAKO_HAKAI);
					if (!m->active_item)
					{	m->active_item=m->max_items-1;}
					else
					{	m->active_item--;}
				//	www=FPS_MENU_FACTOR10;
					m->timeout=-1;
				}
				if ((ST_WORK_MENU|ST_MENU_SUB_PAUSE)==(psp_loop)/*state.substate==ST_MENU_SUB_PAUSE*/) // ポーズメニューの場合は
				{
					if (my_pad_alter & PSP_KEY_PAUSE)	// startボタンでポーズ解除
					{
						play_voice_auto_track(VOICE02_ZAKO_HAKAI);
						pausemenu.active_item=0;
						m->alphadir=2;
						m->timeout=-1;
					}
					/* ボスデバッグ用 */
					#if 1/*(1==DEBUG_MODE)*/
					if (my_pad_alter & PSP_KEY_RIGHT)
					{
						((PLAYER_DATA *)player->data)->my_score=9;/*test*/
						((PLAYER_DATA *)player->data)->zanki=9;/*test*/
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

		switch (m->alphadir)
		{
		case 0: /* Increasing alpha, fade menu in */
			m->alpha+=FPS_MENU_FACTOR8;
			if (m->alpha>=255)
			{
				m->alphadir=1;
				m->alpha=255;
			}
			break;
		/* case 1: fadein complete */
		case 2:
			/* selection done, decreasing alpha, fade menu out */
			m->alpha-=FPS_MENU_FACTOR8;
			if (m->alpha<=0)
			{
				m->alphadir=3;
				m->alpha=0;
			}
			break;
		case 3:
			/* fadout fininshed, menu done */
			SDL_SetAlpha(back_screen,SDL_SRCALPHA,255);
			psp_pop_screen();	//SDL_BlitSurface(back_screen,NULL,screen,NULL);
			//SDL_FreeSurface(back_screen);
			for (i=0;i<m->max_items;i++)
			{
				//for (j=0;j<5;j++)
				{
					m->menu_item_sprite[i]/*[j]*/->type = SP_DELETE;
				}
				//SDL_FreeSurface(m->menu_item_surface[i]);
			}
			if (m->timeout==-2) m->timeout=-3;
			m->select_finish=1;
		//	www=FPS_MENU_FACTOR10;
			return;
		}

		angle512 += FPS_MENU_FACTOR15;
		mask512(angle512);//if (angle>=360) {	angle-=360;}		// angle%=360;

		for (i=0;i<m->max_items;i++)
		{
			d=(MENU_DATA *)m->menu_item_sprite[i]/*[0]*/->data;
			if (i==m->active_item)	/* 選択された */
			{
				d->i2 = 12; /* ゆれ幅を１２にする */
			}
			else	/* 選択されてない */
			{
				if (d->i2 && (0==d->i3))
				{
					d->i2--;
					d->i3 = 3;
				}
			}
			if (d->i3)
			{	d->i3--;}
			m->menu_item_sprite[i]/*[0]*/->x256 = (d->i0*256)+(cos512((angle512))*d->i2);
		//	m->menu_item_sprite[i]/*[0]*/->y256 = (d->i1*256)+(sin512((angle512))*d->i2);
			m->menu_item_sprite[i]/*[0]*/->y256 = (d->i1*256);

		//	for (j=4;j>0;j--)
		//	{
				m->menu_item_sprite[i]/*[j]*/->alpha = m->alpha;
		//		m->menu_item_sprite[i][j]->x256 = m->menu_item_sprite[i][j-1]->x256;
		//		m->menu_item_sprite[i][j]->y256 = m->menu_item_sprite[i][j-1]->y256;
		//		if ((m->menu_item_sprite[i][j]->x256 > 0)&&
		//			(m->menu_item_sprite[i][j]->y256 > 0))
				{	m->menu_item_sprite[i]/*[j]*/->flags |= (SP_FLAG_VISIBLE);}
		//	}
		}
		sprite_work000(SP_GROUP_TEXTS);
		sprite_display000(SP_GROUP_TEXTS);
		if (0==my_pad)
		{
			if (my_pad_alter & PSP_KEY_SHOT_OK)
			{
				play_voice_auto_track(VOICE02_ZAKO_HAKAI);
				m->alphadir=2;
				m->timeout=-1;
			}
		}
		if (m->timeout==0)
		{
			m->alphadir=2;
			m->timeout=-2;
		}
	}
	//parsys_display();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void genericmenu_init(int res_num/*char *options[]*/, MENU *m, int fadeout, int timeout)
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
//	back_screen = NULL;
	m->max_items = 0;
	m->select_finish = 0;	//ここまでMENUの初期化

	while (/*options[m->max_items]*/my_resource[res_num][m->max_items].str_name != NULL)
	{
	//	m->menu_item_surface[m->max_items] = font_render(options[m->max_items],FONT01);
		m->menu_item_surface[m->max_items] = font_render(/*options[m->max_items]*/	(char *)my_resource[res_num][m->max_items].str_name,FONT03);
	//	for (j=0;j<5;j++)		//各スプライトに情報を与えてていく
		{
			m->menu_item_sprite[m->max_items]/*[j]*/		= sprite_add_000(m->menu_item_surface[m->max_items], 1, /*j+*/5, SP_FLAG_NOT_CACHE/*1*/, 0);
			d							= mmalloc(sizeof(MENU_DATA));
			m->menu_item_sprite[m->max_items]/*[j]*/->data	= d;
			m->menu_item_sprite[m->max_items]/*[j]*/->type	= SP_MENU_TEXT;
			m->menu_item_sprite[m->max_items]/*[j]*/->alpha = 0;
			m->menu_item_sprite[m->max_items]/*[j]*/->x256	= t256(0);
			m->menu_item_sprite[m->max_items]/*[j]*/->y256	= t256(0);
		}
		m->max_items++;
	}
	for (i=0; i<m->max_items; i++)
	{
	//	for (j=0;j<5;j++)
		{
			d		= (MENU_DATA *)m->menu_item_sprite[i]/*[j]*/->data;
			d->i0	= my_resource[res_num][i/*m->max_items*/].x_offset/*(PSP_WIDTH480/2)-(m->menu_item_sprite[i][j]->w/2)*/;
			d->i1	= (PSP_HEIGHT272/2+40)-((m->menu_item_sprite[i]/*[j]*/->h+5)*m->max_items/2)+i*(m->menu_item_sprite[i]/*[j]*/->h+5);
			d->i2	= 0;
			d->i3	= 0;
		}
	}

//	back_screen=SDL_CreateRGBSurface(SDL_HWSURFACE,PSP_WIDTH480,PSP_HEIGHT272,
//		screen->format->BitsPerPixel,
//		screen->format->Rmask,
//		screen->format->Gmask,
//		screen->format->Bmask,
//		screen->format->Amask);
	psp_push_screen();	//SDL_BlitSurface(screen,NULL,back_screen,NULL);

	m->alpha		= 0;
	m->alphadir 	= 0;
	m->fadeout		= fadeout;
	m->timeout		= timeout;
	//keybo ard_clear();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static Uint32 pause_start_time = 0;

static void pausemenu_init()
{
	genericmenu_init(RES09_PAUSE_MENU,&pausemenu,1,-1);
	pause_start_time	= psp_get_uint32_ticks();
}

extern void adjust_start_time(Uint32 pause_time);
extern void gamecore_term(void);
static void pausemenu_work(void)
{
	if (!pausemenu.select_finish)
	{
		genericmenu_work(&pausemenu);
	}
	else
	{
		pausemenu.select_finish = 0;
		switch (pausemenu.active_item)
		{
		case 0: /* Continue Game */
			psp_loop = (ST_WORK_GAME_PLAY|0);					//	newsta te(ST_GAME_PLAY,0,0);
			adjust_start_time(pause_start_time);
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

enum
{
//	OPT_MENU_DIFFICULTY = 0,
	OPT_MENU_KEY_CONFIG = 0,
//	#if (1==DEBUG_MODE)
//	OPT_MENU_STAGE_SELECT,
//	#endif
	OPT_MENU_VOLUME,
//	OPT_MENU_RESULT,
	OPT_MENU_QUIT
};

/*---------------------------------------------------------

---------------------------------------------------------*/

static void optionmenu_init(void)
{
	genericmenu_init(RES03_OPTION,&optionmenu,0,-1);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void optionmenu_work(void)
{
	if (!optionmenu.select_finish)
	{
		genericmenu_work(&optionmenu);
	}
	else
	{
		optionmenu.select_finish=0;
		switch (optionmenu.active_item)
		{
		play_voice_auto_track(VOICE02_ZAKO_HAKAI);
	//	case OPT_MENU_DIFFICULTY:	newsta te(ST_MENU,ST_MENU_SUB_RANK_SELECT,1);				break;	/* difficulty */
		case OPT_MENU_KEY_CONFIG:	psp_loop=(ST_WORK_KEY_CONFIG/*ST_INIT_KEY_CONFIG*/|0);		break;	/* Key Config */				//	newsta te(ST_KEY_CONFIG,0,1);
	//	#if (1==DEBUG_MODE)
	//	case OPT_MENU_STAGE_SELECT: newsta te(ST_MENU,ST_MENU_SUB_STAGE_SELECT,1);				break;	/* stage select */
	//	#endif
		case OPT_MENU_VOLUME:		psp_loop=(ST_INIT_MENU|ST_MENU_SUB_VOLUME); 				break;	/* change Sound-FX Volume */	//	newsta te(ST_MENU,ST_MENU_SUB_VOLUME,1);
	//	case OPT_MENU_RESULT:		newsta te(ST_RESULT,0,1);									break;	/* Hiscore */
		case OPT_MENU_QUIT: 		psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);		break;	/* Back to Game */		/* メインメニューに戻る */	//	newsta te(ST_MENU,0/*ST_ME NU_SUB_MAIN_MENU*/,1);
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

//#if (1==DEBUG_MODE)
static void STAGE_SELECTmenu_init(void)
{
	genericmenu_init(RES01_STAGE_SELECT,&STAGE_SELECTmenu,0,-1);
	STAGE_SELECTmenu.active_item=continue_stage;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void STAGE_SELECTmenu_work(void)
{
	if (!STAGE_SELECTmenu.select_finish)
	{
		genericmenu_work(&STAGE_SELECTmenu);
	}
	else
	{
		STAGE_SELECTmenu.select_finish=0;
		continue_stage = STAGE_SELECTmenu.active_item;
		/* プラクティス ゲーム開始 */
	//	newsta te(ST_PLAYER_SELECT,0,1); //newsta te(ST_MENU,ST_MENU_SUB_OPTION,1);
		psp_loop=(ST_INIT_MENU|ST_MENU_SUB_RANK_SELECT);	//newsta te(ST_MENU,ST_MENU_SUB_RANK_SELECT,1);
	}
}
//#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

static void volumemenu_init(void)
{
	genericmenu_init(RES04_VOLUME,&volumemenu,0,-1);
	volumemenu.active_item=volume;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void volumemenu_work(void)
{
	if (!volumemenu.select_finish)
	{
		genericmenu_work(&volumemenu);
	}
	else
	{
		volumemenu.select_finish=0;
		volume=volumemenu.active_item;
		//set_voice_volume(volume*40);
		{const char vol[4] = {3*40,2*40,1*40,0*40};
			set_voice_volume(vol[(volume&(4-1))]);
		}
		psp_loop=(ST_INIT_MENU|ST_MENU_SUB_OPTION); 	//newsta te(ST_MENU,ST_MENU_SUB_OPTION,1);
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void difficultymenu_init(void)
{
	genericmenu_init(RES02_DIFFICULTY,&difficultymenu,0,-1);
	difficultymenu.active_item=difficulty;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void difficultymenu_work(void)
{
	if (!difficultymenu.select_finish)
	{
		genericmenu_work(&difficultymenu);
	}
	else
	{
		difficultymenu.select_finish = 0;
		difficulty=difficultymenu.active_item;
	//	newsta te(ST_MENU,ST_MENU_SUB_OPTION,1);
	//	ini_save();
		/* 通常／プラクティス、ゲーム開始 */
		psp_loop=(ST_INIT_PLAYER_SELECT|0); 	//newsta te(ST_PLAYER_SELECT,0,1);
	}
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

#define YUYUKO_PASSWORD  "TABERARENAINO9"
#define CIRNO_PASSWORD	 "CIRNO9PASSWORD"
#define REMILIA_PASSWORD "RUMIAISMYBRIDE"
extern int tiny_strcmp(char *aaa, const char *bbb);

/*static*/ char str_pass_word[32/*20*/];		/* [***090222 */
void game_clear_set_password(void)
{
	if (0 < difficulty)/* NORMAL 以上でクリアせよ */
	{
		/* [***090222	追加 */
		if (0==tiny_strcmp(str_pass_word, YUYUKO_PASSWORD)) /* 既にクリアしてたら */
		{
		/*	strcpy(str_pass_word, FURAN_PASSWORD)*/;	/* ＆ 幽々子 ＆ チルノ ＆ レミリア 開放 */
		}
		else
		if (0==tiny_strcmp(str_pass_word, CIRNO_PASSWORD))	/* 既にクリアしてたら */
		{
			strcpy(str_pass_word, YUYUKO_PASSWORD); /* 幽々子 ＆ チルノ＆ レミリア 開放 */
		}
		else
		if (0==tiny_strcmp(str_pass_word, REMILIA_PASSWORD)) /* 既にクリアしてたら */
		{
			strcpy(str_pass_word, CIRNO_PASSWORD);	/* チルノ＆ レミリア 開放 */
		}
		else
		{
			strcpy(str_pass_word, REMILIA_PASSWORD);	/* レミリア 開放 */
		}
		ini_save();
	}
}


#define SP_X 250


//player select用

int select_player;

#define MAX_PLAYER (5)/*4*/

#define BASE_SP_BG_PNG		(MAX_PLAYER*0)
#define BASE_SP_ST_PNG		(MAX_PLAYER*1)
#define BASE_SP_PNG_MAX 	(MAX_PLAYER*2)

/*---------------------------------------------------------

---------------------------------------------------------*/

static /*const*/ char *player00_res[(BASE_SP_PNG_MAX)] =
{
	/*	0 REIMU */		"select/bg_re.jpg", 	// "select/sp_reimu_bg.jpg",
	/*	1 MARISA */ 	"select/bg_ma.jpg", 	// "select/sp_marisa_bg.jpg",
	/*	2 REMILIA */	"select/bg_oz.jpg", 	// "select/sp_remiria_bg.jpg",
	/*	3 CIRNO */		"select/bg_ci.jpg", 	// "select/sp_cirno_bg.jpg",
	/*	4 YUYUKO */ 	"select/bg_yu.jpg", 	// "select/sp_yuyuko_bg.jpg",
//
	/*	5 REIMU */		"select/p_re.png",  	// "select/sp_reimu_st.png",
	/*	6 MARISA */ 	"select/p_ma.png",  	// "select/sp_marisa_st.png",
	/*	7 REMILIA */	"select/p_oz.png",  	// "select/sp_remiria_st.png",
	/*	8 CIRNO */		"select/p_ci.png",  	// "select/sp_cirno_st.png",
	/*	9 YUYUKO */ 	"select/p_yu.png",  	// "select/sp_yuyuko_st.png",
};

/*---------------------------------------------------------

---------------------------------------------------------*/

static SDL_Surface *sp_bg;
static SDL_Surface *tmp_sp_bg;
static SDL_Surface *sp_st;
static SDL_Surface *tmp_sp_st;
static /*dou ble*/int sp_scale256;
static int can_select_player;		/* [***090222	追加 */
static int select_mode;
void player_opt_init(void)
{
//	bg_alpha_bbb=0;
	sp_scale256 = t256(1);
	select_mode 	= 1/*4*/;
//
	if (0==tiny_strcmp(str_pass_word, YUYUKO_PASSWORD)) 	/* [***090222	追加 */
	{
		can_select_player=MAX_PLAYER-1;
	}
	else
	if (0==tiny_strcmp(str_pass_word, CIRNO_PASSWORD))		/* [***090222	追加 */
	{
		can_select_player=MAX_PLAYER-2;
	}
	else
	if (0==tiny_strcmp(str_pass_word, REMILIA_PASSWORD))	/* [***090222	追加 */
	{
		can_select_player=MAX_PLAYER-3;
	}
	else
	{
		can_select_player=MAX_PLAYER-4;
	}
	if (select_player > can_select_player)
	{
		select_player	= 0;
	}
	sp_bg			= loadbmp0( (char *)player00_res[BASE_SP_BG_PNG+select_player], 0, 0/*1*/);
	sp_st			= loadbmp0( (char *)player00_res[BASE_SP_ST_PNG+select_player], 0, 0/*1*/);
	SDL_SetColorKey(sp_st, SDL_SRCCOLORKEY, 0x00000000);
	SDL_BlitSurface(sp_bg,NULL,screen,NULL);
	{
		SDL_Rect sr;
		sr.x			= SP_X;
		sr.y			= 50;
		SDL_BlitSurface(sp_st,NULL,screen,&sr);
	}
	psp_loop=(ST_WORK_PLAYER_SELECT|0); //newsta te();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void player_opt_img256(SDL_Surface *src, /*dou ble*/int scale256, int l_or_r)	//r=0, l=1
{
	SDL_Rect sr;
	SDL_Rect dr;
	sr.w = src->w;
	sr.h = src->h;
	sr.x = 0;
	sr.y = 0;
	dr.w = ((src->w*scale256)>>8);
	dr.h = src->h;
	if (l_or_r)
	{	dr.x = SP_X;}
	else
	{	dr.x = 200+SP_X-dr.w;}
	dr.y = 50;
	blit_scaled(src, &sr, screen, &dr);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

void player_opt_work(void)
{
	static int is_turn_right = 0;
//	static int sp_wait = 0;
	static int bg_alpha_bbb;
	SDL_Rect sr;
	sr.x = SP_X;
	sr.y = 50;
	psp_clear_screen();
	//switch (select_mode)
	if (select_mode)
	{
	//case 4://選択
		SDL_BlitSurface(sp_bg,NULL,screen,NULL);
		SDL_BlitSurface(sp_st,NULL,screen,&sr);
		if (0==my_pad)//(sp_wait<0) /*	*/
		{
			if (my_pad_alter & (PSP_KEY_LEFT|PSP_KEY_RIGHT|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL)/*左か右かショット」かキャンセルのいづれか*/  ) //左右
			{	/* 状態が変わる場合で */
				unloadbmp_by_surface(sp_bg);	sp_bg=NULL;
				unloadbmp_by_surface(sp_st);	sp_st=NULL;
			}
			if (my_pad_alter & (PSP_KEY_LEFT|PSP_KEY_RIGHT)/*左か右の両方*/  ) //左右ボタン入力
			{
				is_turn_right=(my_pad_alter & PSP_KEY_RIGHT)?1:0;/*右なら1, 左なら0 */
				play_voice_auto_track(VOICE02_ZAKO_HAKAI);
				tmp_sp_bg=loadbmp0( (char *)player00_res[BASE_SP_BG_PNG+select_player] /*sp_bg_list[select_player]*/, 0, 0/*1*/);
				tmp_sp_st=loadbmp0( (char *)player00_res[BASE_SP_ST_PNG+select_player] /*sp_st_list[select_player]*/, 0, 0/*1*/);
				SDL_SetColorKey(tmp_sp_st, SDL_SRCCOLORKEY, 0x00000000);
				if (0==is_turn_right)
				{	/* [***090222	追加 */
					if (select_player==0)	{	select_player=can_select_player;	}
					else					{	select_player--;}
				}
				else
				{	/* [***090222	追加 */
					if (select_player==can_select_player)	{	select_player=0;	}
					else									{	select_player++;	}
				}
				sp_bg=loadbmp0( (char *)player00_res[BASE_SP_BG_PNG+select_player] /*sp_bg_list[select_player]*/, 0, 0/*1*/);
				sp_st=loadbmp0( (char *)player00_res[BASE_SP_ST_PNG+select_player] /*sp_st_list[select_player]*/, 0, 0/*1*/);
				SDL_SetColorKey(sp_st, SDL_SRCCOLORKEY, 0x00000000);
				select_mode=0/*(旧仕様は、右なら5, 左なら6)*/;
			//	sp_wait=10;
				sp_scale256 = t256(1);
				bg_alpha_bbb=0;
			}
			if (my_pad_alter & PSP_KEY_SHOT_OK)
			{
				play_voice_auto_track(VOICE01_HIT);
				psp_loop=(ST_INIT_GAME_PLAY|0); //newsta te(ST_GAME_PLAY,0,1);
			}
			else
			if (my_pad_alter & PSP_KEY_BOMB_CANCEL)
			{
				play_voice_auto_track(VOICE04_SHIP_HAKAI);
				/* メインメニューに戻る */
				psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);	//newsta te(ST_MENU/*ST_INTRO*/,0/*1*/,1);
			}
		}
		//else
		//{
		//	sp_wait-=1/**fps_fa ctor*/;
		//}
	}
	else
	{
//		break;
//	case 5: //右
		sp_scale256 -= t256(0.07);
		bg_alpha_bbb += 15;
		if (bg_alpha_bbb < 255)
		{
			SDL_SetAlpha(sp_bg, SDL_SRCALPHA, bg_alpha_bbb);
		}
		else if (bg_alpha_bbb==255)
		{
			;
		}
		else
		{
			bg_alpha_bbb = 255;
			SDL_SetAlpha(sp_bg, SDL_SRCALPHA, bg_alpha_bbb);
		}
		SDL_BlitSurface(tmp_sp_bg,NULL,screen,NULL);
		SDL_BlitSurface(sp_bg,NULL,screen,NULL);

		if (sp_scale256 < t256(0) )
		{
			select_mode=1/*4*/;
			unloadbmp_by_surface(tmp_sp_bg);	tmp_sp_bg=NULL;
			unloadbmp_by_surface(tmp_sp_st);	tmp_sp_st=NULL;
			SDL_SetAlpha(sp_bg, SDL_SRCALPHA, 255);
		}
		else
		{
			player_opt_img256(sp_st, t256(1)-sp_scale256, (1-is_turn_right)/*0*/);	// 5右0 6左1
			player_opt_img256(tmp_sp_st, sp_scale256, (is_turn_right)/*1*/);		// 5右1 6左0

//	case 5: //右
//			player_opt_img256(sp_st, t256(1)-sp_scale256, 0);	// 5右0 6左1
//			player_opt_img256(tmp_sp_st, sp_scale256, 1);		// 5右1 6左0
//	case 6: //左
//			player_opt_img256(sp_st, t256(1)-sp_scale256, 1);
//			player_opt_img256(tmp_sp_st, sp_scale256, 0);
		}
//		break;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static SDL_Surface *intropic/*=NULL*/;
static void startmenu_init(void)
{
	set_music_volume(128);/*とりあえず*/
	{
		if (NULL==intropic)
		{
			intropic=loadbmp0("bg/title_bg.jpg", 0, /*0*/1/*1*/);
		}
	//	psp_clear_screen();
		SDL_SetAlpha(intropic, SDL_SRCALPHA, 255);
		SDL_BlitSurface(intropic, NULL, /*back_screen*/screen, NULL);
		unloadbmp_by_surface(intropic); 	intropic=NULL;/*unload_bmp_by_name("bg/title_bg.jpg");*/
	}
	genericmenu_init(RES00_MAIN_MENU,&startmenu,0,/*6000*/-1/*500*/);/*RESULTにしたから要らなくなったが、将来デモとか付けるならいる*/
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void startmenu_work(void)
{
	if (!startmenu.select_finish)
	{
		genericmenu_work(&startmenu);
	}
	else
	{
		#if 0/*RESULTにしたから要らなくなったが、将来デモとか付けるならいる*/
		if (startmenu.timeout==-3)
		{
			/* 時間切れで、強制的にデモ表示  / Timeout, go on and show HighScore List */
			psp_loop = (ST_WORK_RESULT|0);//newsta te(ST_RESULT,0,1);
		}
		else
		#endif
		{
			startmenu.select_finish=0;
			switch (startmenu.active_item)
			{
			case 0: 	practice_mode=0;	continue_stage=0;
						psp_loop=(ST_INIT_MENU|ST_MENU_SUB_RANK_SELECT);	break; /* Start */			//	newsta te(ST_MENU,ST_MENU_SUB_RANK_SELECT,1);	/*newsta te(ST_PLAYER_SELECT,0,1); */				//newsta te(ST_GAME_PLAY,0,1);
		//	case 1: 														break; /* Extra Start */	//	newsta te(ST_PLAYER_SELECT,0,1);		//newsta te(ST_GAME_PLAY,0,1);
			case 1: 	practice_mode=1;
						psp_loop=(ST_INIT_MENU|ST_MENU_SUB_STAGE_SELECT);	break; /* Practice Start */ //	newsta te(ST_MENU,ST_MENU_SUB_STAGE_SELECT,1);*/ /* stage select */ //newsta te(ST_PLAYER_SELECT,0,1);	break;	//newsta te(ST_GAME_PLAY,0,1);
		//	case 1: 														break; /* Replay */ 		//	newsta te(ST_MENU,ST_MENU_SUB_OPTION,1);
			case 2: 	psp_loop=(ST_WORK_STORY|0); 						break; /* Story */			//	newsta te(ST_STORY,0,1);
			case 3: 	psp_loop=(ST_WORK_RESULT|0);						break; /* Result */ 		//	newsta te(ST_RESULT,0,1); /* Hiscore */
		//	case 3: 														break; /* Music Room */ 	//	newsta te(ST_MENU,ST_MENU_SUB_OPTION,1);
			case 4: 	psp_loop=(ST_INIT_MENU|ST_MENU_SUB_OPTION); 		break; /* Option */ 		//	newsta te(ST_MENU,ST_MENU_SUB_OPTION,1);
			case 5: 	psp_loop=ST_PSP_QUIT;								break; /* Quit */			//	newsta te(ST_PSP_QUIT,0,1);
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
	case ST_MENU_SUB_MAIN_MENU: 	startmenu_init();			break;
	case ST_MENU_SUB_PAUSE: 		pausemenu_init();			break;
	case ST_MENU_SUB_OPTION:		optionmenu_init();			break;
	//#if (1==DEBUG_MODE)
	case ST_MENU_SUB_STAGE_SELECT:	STAGE_SELECTmenu_init();	break;
	//#endif
	case ST_MENU_SUB_VOLUME:		volumemenu_init();			break;
	case ST_MENU_SUB_RANK_SELECT:	difficultymenu_init();		break;
	}
	psp_loop += (0x0100);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

void all_menu_work(void)
{
	switch ((Uint8)(psp_loop&0xff)/*state.substate*/)
	{
//	play_voice_auto_track(VOICE02_ZAKO_HAKAI);/*??*/
	case ST_MENU_SUB_MAIN_MENU: 	startmenu_work();			break;
	case ST_MENU_SUB_PAUSE: 		pausemenu_work();			break;
	case ST_MENU_SUB_OPTION:		optionmenu_work();			break;
	//#if (1==DEBUG_MODE)
	case ST_MENU_SUB_STAGE_SELECT:	STAGE_SELECTmenu_work();	break;
	//#endif
	case ST_MENU_SUB_VOLUME:		volumemenu_work();			break;
	case ST_MENU_SUB_RANK_SELECT:	difficultymenu_work();		break;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

void menusystem_init(void)
{
	continue_stage					= 0;
//	practice_mode					= 0;
	volume							= 0;
//
	startmenu.active_item			= 0;
	pausemenu.active_item			= 0;
	optionmenu.active_item			= 0;
	//#if (1==DEBUG_MODE)
	STAGE_SELECTmenu.active_item	= 0/*continue_stage*/;
	//#endif
	volumemenu.active_item			= 0/*volume*/;
	difficultymenu.active_item		= 0;
//	player_slmenu.active_item		= 0;
}


