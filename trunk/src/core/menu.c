#include "menu.h"

extern SDL_Surface *screen;
extern GAMESTATE state;
static int volume/*=2*/; /* 0-3 */
extern int difficulty;
extern KEYCONFIG keyconfig;
extern double fps_factor;
extern int fps_display;
extern Uint32 videoflags;
extern int fps_method;

extern SDL_Surface *back_screen;/*test*/


int continue_stage;
int practice_mode;


static MENU startmenu;
static MENU pausemenu;
static MENU optionmenu;
//#if (1==DEBUG_MODE)
static MENU STAGE_SELECTmenu;
//#endif
static MENU volumemenu;
static MENU difficultymenu;
//static MENU player_slmenu;

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
static MENU_RESOURCE my_resource[RES99_MAX][8/*10*/] =
{
	{	//	RES00_MAIN_MENU 	//		/*const*/ char *startmenu_options[] =/*(char *)*/startmenu_options
		{	360,		"START",			},
	//	{	330,		"EXTRA START",		},
		{	245,		"PRACTICE START",	},
	//	{	400,		"REPLAY",		},
		{	320,		"STORY",		},		//ちょっと変更
		{	285,		"RESULT",		},
	//	{	310,		"MUSIC ROOM",	},
		{	335,		"OPTION",		},
		{	310,		"QUIT", 		},
		{	0,			NULL,			},
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
	{	//	RES02_DIFFICULTY		//			/*const*/ char *difficultymenu_options[]=/*(char *)*/difficultymenu_options
		{	275,		"EASY", 		},
		{	245,		"NORMAL",		},
		{	220,		"HARD", 		},
		{	185,		"LUNATIC",		},
		{	0,NULL, 	},
	},
	{	//	RES03_OPTION			//	/*const*/ char *mainmenu_options[]=/*(char *)*/mainmenu_options
	//	"DIFFICULTY",
		{	200,		"KEY CONFIG",	},
	//	#if (1==DEBUG_MODE)
	//	"STAGE SELECT",
	//	#endif
		{	180,		"SOUND VOLUME", },
		{	200,		"HIGH SCORE",	},		//変更
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




typedef struct	//たぶん矩形データ用?
{
	int i0; 	//表示用xオフセット
	int i1; 	//y
	int i2; 	//スプライト用(メニュー選択時、横xの揺れ幅)
	int i3; 	//スプライト用()
} MENU_DATA;

#if 0
	extern dou ble fps_factor;
	#define FPS_MENU_FACTOR 	(fps_factor)
	#define FPS_MENU_FACTOR10	(10/fps_factor)/*←どうも不安定(Division by Zero ???)*/
	#define FPS_MENU_FACTOR8	(8*fps_factor)
	#define FPS_MENU_FACTOR15	(15*fps_factor)
#else
	#define FPS_MENU_FACTOR 	(1)
	#define FPS_MENU_FACTOR10	(16/*64*/)/* 10/FPS_MENU_FACTOR */
	#define FPS_MENU_FACTOR8	(16)/* 8*FPS_MENU_FACTOR */
	#define FPS_MENU_FACTOR15	(3/*2*/)
#endif

static void genericmenu_work(MENU *m)
{
	static int w=10;
	static /*double*/int angle512/*=0*/;
	int i,j;
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

		if ((w<=0) && (m->alphadir<2))
		{
			if (my_pad & PSP_KEY_DOWN/*keybo ard[KEY_DOWN]*/)
			{
				playChunk(2);
				if (m->active_item==m->nopt-1)
				{	m->active_item=0;}
				else
				{	m->active_item++;}
				w=FPS_MENU_FACTOR10;
				m->timeout=-1;
			}
			else if (my_pad & PSP_KEY_UP/*keybo ard[KEY_UP]*/)
			{
				playChunk(2);
				if (!m->active_item)
				{	m->active_item=m->nopt-1;}
				else
				{	m->active_item--;}
				w=FPS_MENU_FACTOR10;
				m->timeout=-1;
			}
			if (state.substate==MENU_PAUSE) //ポーズメニューの場合は
			{
				if (my_pad & PSP_KEY_PAUSE/*keybo ard[KEY_PAUSE]*/) 	//startボタンでポーズ解除
				{
					playChunk(2);
					pausemenu.active_item=0;
					m->alphadir=2;
					m->timeout=-1;
				}
				/* ボスデバッグ用 */
				#if 1/*(1==DEBUG_MODE)*/
				if (my_pad & PSP_KEY_RIGHT/*keybo ard[KEY_RIGHT]*/)
				{
					((PLAYER_DATA *)player->data)->score=9;/*test*/
					((PLAYER_DATA *)player->data)->lives=9;/*test*/
					((PLAYER_DATA *)player->data)->bombs=9;/*test*/
					#if 1
					/* ボスチェック用 */
					((PLAYER_DATA *)player->data)->weapon=127;/*test*/
					#endif
				}
				#endif
			}
		} else {w--;}

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
			for (i=0;i<m->nopt;i++)
			{
				for (j=0;j<5;j++)
				{
					m->opts[i][j]->type=SP_DELETE;
				}
				//SDL_FreeSurface(m->opts_surface[i]);
			}
			if (m->timeout==-2) m->timeout=-3;
			m->select_finish=1;
			w=FPS_MENU_FACTOR10;
			return;
		}

		angle512 += FPS_MENU_FACTOR15;
		mask512(angle512);//if (angle>=360) {	angle-=360;}		// angle%=360;

		for (i=0;i<m->nopt;i++)
		{
			d=(MENU_DATA *)m->opts[i][0]->data;
			if (i==m->active_item)	/* 選択された */
			{
				d->i2=12;	/* ゆれ幅を１２にする */
			}
			else	/* 選択されてない */
			{
				if (d->i2 && (!d->i3))
				{
					d->i2--;
					d->i3=3;
				}
			}
			if (d->i3)
			{	d->i3--;}
			m->opts[i][0]->x=d->i0+(co_s512((angle512))*(double)d->i2);
		//	m->opts[i][0]->y=d->i1+(si_n512((angle512))*(double)d->i2);
			m->opts[i][0]->y=d->i1;

			for (j=4;j>0;j--)
			{
				m->opts[i][j]->alpha=m->alpha;
				m->opts[i][j]->x=m->opts[i][j-1]->x;
				m->opts[i][j]->y=m->opts[i][j-1]->y;
				if ((m->opts[i][j]->x>0)&&(m->opts[i][j]->y>0))
				{	m->opts[i][j]->flags|=SP_FLAG_VISIBLE;}
			}
		}
		sprite_work(SP_SHOW_MENUTEXT);
		sprite_display(SP_SHOW_MENUTEXT);

		if (my_pad & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)
		{
			playChunk(2);
			m->alphadir=2;
			m->timeout=-1;
		}
		if (m->timeout==0)
		{
			m->alphadir=2;
			m->timeout=-2;
		}
	}
	//parsys_display();
}
/*-------------------*/
static void genericmenu_init(int res_num/*char *options[]*/, MENU *m, int fadeout, int timeout)
{
	int i,j;
	MENU_DATA *d;
	for (i=0;i<20;i++)
	{
		m->opts_surface[i]=NULL;
		for (j=0;j<5;j++) { m->opts[i][j]=NULL; }
	}
//	back_screen=NULL;
	m->nopt=0;
	m->select_finish=0; 	//ここまでMENUの初期化

	while (/*options[m->nopt]*/my_resource[res_num][m->nopt].str_name != NULL)
	{
		// m->opts_surface[m->nopt]=font_render(options[m->nopt],FONT01);
		m->opts_surface[m->nopt]=font_render(/*options[m->nopt]*/  (char *)my_resource[res_num][m->nopt].str_name,FONT06);
		for (i=0;i<5;i++)		//各スプライトに情報を与えてていく
		{
			m->opts[m->nopt][i] 		=sprite_add(m->opts_surface[m->nopt],1,i+5,1);
			d							=mmalloc(sizeof(MENU_DATA));
			m->opts[m->nopt][i]->data	=d;
			m->opts[m->nopt][i]->type	=SP_MENUTEXT;
			m->opts[m->nopt][i]->alpha	=0;
			m->opts[m->nopt][i]->x		=0;
			m->opts[m->nopt][i]->y		=0;
		}
		m->nopt++;
	}
	for (i=0;i<m->nopt;i++)
	{
		for (j=0;j<5;j++)
		{
			d		= (MENU_DATA *)m->opts[i][j]->data;
			d->i0	= my_resource[res_num][i/*m->nopt*/].x_offset/*(PSP_WIDTH/2)-(m->opts[i][j]->w/2)*/;
			d->i1	= (PSP_HEIGHT/2+40)-((m->opts[i][j]->h+5)*m->nopt/2)+i*(m->opts[i][j]->h+5);
			d->i2	= 0;
			d->i3	= 0;
		}
	}

//	back_screen=SDL_CreateRGBSurface(SDL_HWSURFACE,PSP_WIDTH,PSP_HEIGHT,
//		screen->format->BitsPerPixel,
//		screen->format->Rmask,
//		screen->format->Gmask,
//		screen->format->Bmask,
//		screen->format->Amask);
	psp_push_screen();	//SDL_BlitSurface(screen,NULL,back_screen,NULL);

	m->alpha=0;
	m->alphadir=0;
	m->fadeout=fadeout;
	m->timeout=timeout;
	keyboard_clear();
}
/*-------------------*/


static Uint32 pause_start_time=0;

static void pausemenu_init()
{
	genericmenu_init(RES09_PAUSE_MENU,&pausemenu,1,-1);
	pause_start_time=PSP_GetTicks();
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
		pausemenu.select_finish=0;
		switch (pausemenu.active_item)
		{
		case 0: /* Continue Game */
			newstate(ST_GAME_PLAY,0,0);
			adjust_start_time(pause_start_time);
			break;
		case 1: /* Quit Game */
			#if 1
			gamecore_term();
			#else
			bg_destroy();		//[***090126		追加
			sprite_remove_all(SP_SHOW_ALL);
			//parsys_remove_all();
			controller_remove_all();
			//score_cleanup();
			#endif
			//stopMusic();		//[***090123		コメントアウト
			play_music(BGM_00_intro);
			newstate(/*ST_INTRO*/ST_MENU,0,1);
			break;
		}
	}
}
enum
{
//	OPT_MENU_DIFFICULTY = 0,
	OPT_MENU_KEY_CONFIG = 0,
//	#if (1==DEBUG_MODE)
//	OPT_MENU_STAGE_SELECT,
//	#endif
	OPT_MENU_VOLUME,
	OPT_MENU_SHOW_HCLIST,
	OPT_MENU_QUIT
};
static void optionmenu_init(void)
{
	genericmenu_init(RES03_OPTION,&optionmenu,0,-1);
}

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
		playChunk(2);
	//	case OPT_MENU_DIFFICULTY:	newstate(ST_MENU,MENU_DIFF,1);				break;	/* difficulty */
		case OPT_MENU_KEY_CONFIG:	newstate(ST_KEY_CONFIG,0,1);				break;	/* Key Config */
	//	#if (1==DEBUG_MODE)
	//	case OPT_MENU_STAGE_SELECT: newstate(ST_MENU,MENU_STAGE_SELECT,1);		break;	/* stage select */
	//	#endif
		case OPT_MENU_VOLUME:		newstate(ST_MENU,MENU_VOLUME,1);			break;	/* change Sound-FX Volume */
		case OPT_MENU_SHOW_HCLIST:	newstate(ST_SHOW_HCLIST,0,1);				break;	/* Hiscore */
		case OPT_MENU_QUIT: 		newstate(ST_MENU,0/*MENU_START*/,1);		break;	/* Back to Game */
		}
	}
}
//#if (1==DEBUG_MODE)
static void STAGE_SELECTmenu_init()
{
	genericmenu_init(RES01_STAGE_SELECT,&STAGE_SELECTmenu,0,-1);
	STAGE_SELECTmenu.active_item=continue_stage;
}

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
	//	newstate(ST_PLAYER_SELECT,0,1); //newstate(ST_MENU,MENU_OPTION,1);
		newstate(ST_MENU,MENU_DIFF,1);
	}
}
//#endif

static void volumemenu_init()
{
	genericmenu_init(RES04_VOLUME,&volumemenu,0,-1);
	volumemenu.active_item=volume;
}

static void volumemenu_work()
{
	if (!volumemenu.select_finish)
	{
		genericmenu_work(&volumemenu);
	}
	else
	{
		volumemenu.select_finish=0;
		volume=volumemenu.active_item;
		//setChunkVolume(volume*40);
		{const char vol[4] = {3*40,2*40,1*40,0*40};
			setChunkVolume(vol[(volume&(4-1))]);
		}
		newstate(ST_MENU,MENU_OPTION,1);
	}
}

static void difficultymenu_init(void)
{
	genericmenu_init(RES02_DIFFICULTY,&difficultymenu,0,-1);
	difficultymenu.active_item=difficulty;
}

static void difficultymenu_work(void)
{
	if (!difficultymenu.select_finish)
	{
		genericmenu_work(&difficultymenu);
	}
	else
	{
		difficultymenu.select_finish=0;
		difficulty=difficultymenu.active_item;
	//	newstate(ST_MENU,MENU_OPTION,1);
	//	ini_save();
		/* 通常／プラクティス、ゲーム開始 */
		newstate(ST_PLAYER_SELECT,0,1);
	}
}

//[*****************************
//keyconfig用	***20080110-0115
static int key_pos[12]; 	// 数字=ボタン変数 並びは"key_bg.png"
static int keys2[11] =		// 数字=機能定数 並びは"keylist.png"
{	// 数字=機能定数 並びは"keylist.png"に依存するので変えないように。
	KEY_NONE,
	KEY_SHOT,
	KEY_BOMB,
	KEY_SLOW,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_PAUSE,
	KEY_CANCEL,
	KEY_SC_SHOT
};
static SDL_Surface *key_sf[12]; 	//機能リストの画像"keylist.png"
static SDL_Surface *key_bg;
static SDL_Surface *key_set;
static SPRITE *keyicon;
static SPRITE *keyicon2;
static SDL_Rect rect_k;
static int select_key;
static int select_key2;
static int select_key_dir;		//menu画面内の状態
static int bg_level;			//menu画面の状態
//[******************************
static int bg_alpha;		//共用
//[******************************
//player select用
#define MAX_PLAYER 4
int select_player;
static int can_select_player;		/* [***090222	追加 */

static /*dou ble*/int sp_scale256;
static SDL_Surface *sp_bg;
static SDL_Surface *tmp_sp_bg;
static SDL_Surface *sp_st;
static SDL_Surface *tmp_sp_st;
//[******************************
/*
	typedef struct {
		int u;	//上
		int d;	//下
		int l;	//左
		int r;	//右
		int ba; //×
		int ma; //○
		int sa; //△
		int si; //□
		int rt; //R
		int lt; //L
		int sl; //SELECT
		int st; //START
	} KEYCONFIG;
*/
//[***090110
//キーコンフィグ追加。
//基本は絶対値指定(機能固定値)でsupport.cのkeypollの中のkeyconfigだけ配列指定番号に変数（keyconfig.xxx）を取る
//またkeypollに入る値は、0と1だけのままだとボタン設定上問題が出るのでビットで管理するようにした
/*static*/ void key_config_init()
{
	char *options_key[]=
	{
		"NONE",
		"SHOT",
		"BOMB",
		"SLOW",
		"UP",
		"DOWN",
		"LEFT",
		"RIGHT",
		"PAUSE",
		"CANCEL",
		"SC SHOT",
		NULL
	};
	key_pos[0]=keyconfig.ma;
	key_pos[1]=keyconfig.ba;
	key_pos[2]=keyconfig.si;
	key_pos[3]=keyconfig.sa;
	key_pos[4]=keyconfig.u;
	key_pos[5]=keyconfig.d;
	key_pos[6]=keyconfig.l;
	key_pos[7]=keyconfig.r;
	key_pos[8]=keyconfig.rt;
	key_pos[9]=keyconfig.lt;
	key_pos[10]=keyconfig.sl;
	key_pos[11]=keyconfig.st;
	play_music(BGM_05_stage5);
	int i=0;
	select_key=0;
	select_key_dir=0;
	bg_level=0;

	bg_alpha=0;
	keyicon=sprite_add_file("key_icon.png" , 12, PR_FRONT0);	keyicon->anim_speed=2;
	key_bg=loadbmp("key_bg.png");
	key_set=loadbmp("keylist.png");
	SDL_SetColorKey(key_set,SDL_SRCCOLORKEY,0x00000000);

	keyicon->type=SP_MENUTEXT;
	keyicon->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	keyicon->x=68;
	keyicon->y=49;
	keyicon->aktframe=0;

	while (options_key[i]!=NULL)
	{
		key_sf[i]=font_render(options_key[i],FONT03);
		i++;
	}
	//SDL_SetAlpha(key_bg, SDL_SRCALPHA, 0);

	keyboard_clear();
}


static void kp_search(int btn, int num)
{
	int i;
	for (i=0;i<11;i++)
		if (keys2[i]==btn)
			break;
	if (i==11)
		i=0;
	rect_k.x=150;
	rect_k.y=49+(num*15);
	SDL_BlitSurface(key_sf[i],NULL,screen,&rect_k);
}
/*static*/ void key_config_work()
{
	static int key_shot;		//存在確認用。0で存在しない。存在しないとメニューを抜けれない。
	static int key_down;
	static int key_up;
	static int bg_wait;
	int i;

	SDL_BlitSurface(key_bg,NULL,screen,NULL);
	switch (bg_level)
	{
	case 0:
		if (bg_alpha<255)
		{	bg_alpha+=6*fps_factor;}
		else
		{
			bg_alpha=255;
			bg_level=1;
			bg_wait=0;
		}
		SDL_SetAlpha(key_bg, SDL_SRCALPHA, bg_alpha);
		break;
	case 1:
		switch (select_key_dir)
		{
		case 0: 	//第一層
			if (bg_wait==0)
			{
				if (my_pad & PSP_KEY_UP/*keybo ard[KEY_UP]*/)		//上ボタン
				{
					if (select_key==0)
					{	select_key=13;}
					else
					{	select_key--;}
					bg_wait=10;
				}
				if (my_pad & PSP_KEY_DOWN/*keybo ard[KEY_DOWN]*/)	//下ボタン
				{
					if (select_key==13)
					{	select_key=0;}
					else
					{	select_key++;}
					bg_wait=10;
				}
				if (my_pad & PSP_KEY_LEFT/*keybo ard[KEY_LEFT]*/)	//左
				{
					select_key=0;
				}
				if (my_pad & PSP_KEY_RIGHT/*keybo ard[KEY_RIGHT]*/) //右
				{
					select_key=13;
				}

				if (my_pad & PSP_KEY_CANCEL/*keybo ard[KEY_CANCEL]*/)		//キャンセルボタン
				{
					playChunk(4);
//					keyicon->type=SP_DELETE;
					bg_level=2;
					//key_bg=loadbmp("moon.jpg");
//					bg_alpha=255;
//					play_music(BGM_00_intro);
				}

				if (my_pad & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)	//ショットボタン
				{
					playChunk(2);
					if (select_key==13) 	//OKを選んだ場合
					{
						key_shot=0;
						key_down=0;
						key_up=0;
						for (i=0;i<12;i++)	//ショットボタン&↑&↓があるか確認
						{
							if (key_pos[i]==KEY_SHOT)
							{	key_shot++;}
							if (key_pos[i]==KEY_UP)
							{	key_up++;}
							if (key_pos[i]==KEY_DOWN)
							{	key_down++;}
						}
						if (key_shot && key_up && key_down)
						{
//							keyicon->type=SP_DELETE;
							bg_level=2;
							//key_bg=loadbmp("moon.jpg");
//							bg_alpha=255;
//							play_music(BGM_00_intro);

							//最終的に代入される物
							keyconfig.ma=key_pos[0];
							keyconfig.ba=key_pos[1];
							keyconfig.si=key_pos[2];
							keyconfig.sa=key_pos[3];
							keyconfig.u= key_pos[4];
							keyconfig.d= key_pos[5];
							keyconfig.l= key_pos[6];
							keyconfig.r= key_pos[7];
							keyconfig.rt=key_pos[8];
							keyconfig.lt=key_pos[9];
							keyconfig.sl=key_pos[10];
							keyconfig.st=key_pos[11];
							ini_save();
						}
						else
						{	pspDebugScreenPrintf("damedayo\n");}
					}
					else if (select_key==12)	//resetを選んだ場合
					{
						playChunk(7);
						bg_wait=10;
						key_pos[0]= KEY_CANCEL;
						key_pos[1]= KEY_SHOT;
						key_pos[2]= KEY_BOMB;
						key_pos[3]= KEY_SC_SHOT;
						key_pos[4]= KEY_UP;
						key_pos[5]= KEY_DOWN;
						key_pos[6]= KEY_LEFT;
						key_pos[7]= KEY_RIGHT;
						key_pos[8]= KEY_SLOW;
						key_pos[9]= KEY_NONE;
						key_pos[10]=KEY_NONE;
						key_pos[11]=KEY_PAUSE;
						select_key=13;
					}
					else		//キーの選択
					{
						select_key_dir=1;
					}
				}
			}
			else
			{	bg_wait--;}

			keyicon->y=49+(select_key*15);
			break;

		case 1: 	//第二層準備
			for (i=0;i<11;i++)
			{
				if (keys2[i]==key_pos[select_key])		//選択した所に何が入っているのか調べる
				{
					break;
				}
			}
			bg_alpha=0;
			keyicon2=sprite_add_file("key_icon.png" , 12, PR_FRONT0);
			keyicon2->anim_speed=1;
			keyicon2->type=SP_MENUTEXT;
			keyicon2->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
			keyicon2->x=295;
			keyicon2->y=40+(i*19);
			keyicon2->aktframe=0;
			select_key2=i;
			select_key_dir=2;
			break;

		case 2:
			if (bg_alpha<255)
			{	bg_alpha+=12*fps_factor;}
			else
			{
				bg_alpha=255;
				select_key_dir=3;
			}
			rect_k.x=310;
			rect_k.y=36;
			SDL_SetAlpha(key_set, SDL_SRCALPHA, bg_alpha);
			SDL_BlitSurface(key_set,NULL,screen,&rect_k);
			break;

		case 3: 	//第二層
			if (bg_wait==0)
			{
				if (my_pad & PSP_KEY_UP/*keybo ard[KEY_UP]*/)
				{
					if (select_key2==0)
					{	select_key2=10;}
					else
					{	select_key2--;}
					bg_wait=10;
				}
				if (my_pad & PSP_KEY_DOWN/*keybo ard[KEY_DOWN]*/)
				{
					if (select_key2==10)
					{	select_key2=0;}
					else
					{	select_key2++;}
					bg_wait=10;
				}
				if (my_pad & PSP_KEY_LEFT/*keybo ard[KEY_LEFT]*/)
				{
					select_key2=0;
				}
				if (my_pad & PSP_KEY_RIGHT/*keybo ard[KEY_RIGHT]*/)
				{
					select_key2=10;
				}
				if (my_pad & PSP_KEY_CANCEL/*keybo ard[KEY_CANCEL]*/)
				{
					bg_wait=20;
					select_key_dir=0;
					keyicon2->type=SP_DELETE;
				}
				if (my_pad & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)
				{
					playChunk(2);
					bg_wait=20;
					for (i=0;i<12;i++)
					{
						if (key_pos[i]==keys2[select_key2])
						{	break;}
					}
					key_pos[select_key]=keys2[select_key2];

					select_key_dir=0;
					keyicon2->type=SP_DELETE;
				}
			}
			else
			{	bg_wait--;}

			rect_k.x=310;
			rect_k.y=36;
			SDL_BlitSurface(key_set,NULL,screen,&rect_k);

			keyicon2->y=40+(select_key2*19);
			break;
		}
		for (i=0;i<12;i++)
		{
			kp_search(key_pos[i], i);
		}
		break;
	case 2:
		if (bg_alpha<255)
		{	bg_alpha+=8*fps_factor;}
		else
		{
			//unloadbmp_by_surface(key_bg);
			bg_alpha=255;
			newstate(ST_MENU/*ST_INTRO*/,0/*1*/,1);
			for (i=0;i<12;i++)
			{	SDL_FreeSurface(key_sf[i]);}
		}
		//SDL_SetAlpha(key_bg, SDL_SRCALPHA, bg_alpha);
		break;
	}

	if (bg_level!=2)
	{
		sprite_work(SP_SHOW_MENUTEXT);
		sprite_display(SP_SHOW_MENUTEXT);
	}
	else
	{
		keyicon->y=PSP_HEIGHT+1;
		keyicon->type=SP_DELETE;
		bg_alpha=255;
		play_music(BGM_00_intro);
		newstate(/*ST_INTRO*/ST_MENU,0,1);
	}
}


/******自分でコンパイルする人へ******
このままだと配布バイナリと違うので
文字列"無駄"を適当に変えといて下さい。
************************************/
/* チルド (chilled) は、冷却されること。
「冷やす」を意味する英語の動詞チル (chill) の過去分詞である。
何で chillno じゃなくて cirno なんだろな？ */

#define CIRNO_PASSWORD	 "CIRNO9PASSWORD"
#define REMILIA_PASSWORD "RUMIAISMYBRIDE"

void game_clear_set_password(void)
{
	if (difficulty>0)
	{
		/* [***090222	追加 */
		if (!strcmp(password,CIRNO_PASSWORD))	/* 既にクリアしてたら */
		{
			;
		}
		else
		if (!strcmp(password,REMILIA_PASSWORD)) /* 既にクリアしてたら */
		{
			strcpy(password,CIRNO_PASSWORD);	/* チルノ＆レミリア開放 */
		}
		else
		{
			strcpy(password,REMILIA_PASSWORD);	/* レミリア開放 */
		}
		ini_save();
	}
}


#define SP_X 250

//static /*const*/ char *sp_bg_list[] =
//{
//	#define BASE_CORE_PNG 0
//	"sp_reimu_bg.jpg",
//	"sp_marisa_bg.jpg",
//	"sp_remiria_bg.jpg",
//	NULL
//};
//static /*const*/ char *sp_st_list[] =
//{
//	#define BASE_TSHOOT_PNG 4
//	"sp_reimu_st.png",
//	"sp_marisa_st.png",
//	"sp_remiria_st.png",
//	NULL
//};

#define BASE_SP_BG_PNG		(4*0)
#define BASE_SP_ST_PNG		(4*1)
static /*const*/ char *player00_res[8/*20*/]=
{
	/*	0 REIMU */		"sp_reimu_bg.jpg",
	/*	1 MARISA */ 	"sp_marisa_bg.jpg",
	/*	2 REMIRIA */	"sp_remiria_bg.jpg",
	/*	3 CIRNO */		"sp_cirno_bg.jpg",
//
	/*	4 REIMU */		"sp_reimu_st.png",
	/*	5 MARISA */ 	"sp_marisa_st.png",
	/*	6 REMIRIA */	"sp_remiria_st.png",
	/*	7 CIRNO */		"sp_cirno_st.png",
};

static int select_mode;
void player_opt_init()
{
	SDL_Rect sr;
	sr.x=SP_X;
	sr.y=50;
	select_player=0;
	select_mode=1/*4*/;
	sp_bg=loadbmp( (char *)player00_res[BASE_SP_BG_PNG+select_player] /*sp_bg_list[select_player]*/);
	sp_st=loadbmp( (char *)player00_res[BASE_SP_ST_PNG+select_player] /*sp_st_list[select_player]*/);
	SDL_SetColorKey(sp_st, SDL_SRCCOLORKEY, 0x00000000);
	SDL_BlitSurface(sp_bg,NULL,screen,NULL);
	SDL_BlitSurface(sp_st,NULL,screen,&sr);

	if (!strcmp(password,CIRNO_PASSWORD))		/* [***090222	追加 */
	{
		can_select_player=MAX_PLAYER-1;
	}
	else if (!strcmp(password,REMILIA_PASSWORD))		/* [***090222	追加 */
	{
		can_select_player=MAX_PLAYER-2;
	}
	else
	{
		can_select_player=MAX_PLAYER-3;
	}

	sp_scale256 = t256(1);
}
static void player_opt_img256(SDL_Surface *src, /*dou ble*/int scale256, int l_or_r)	//r=0, l=1
{
	SDL_Rect sr,dr;

	sr.w=src->w;
	sr.h=src->h;
	sr.x=0;
	sr.y=0;
	dr.w=((src->w*scale256)>>8);
	dr.h=src->h;
	if (l_or_r)
	{	dr.x=SP_X;}
	else
	{	dr.x=200+SP_X-dr.w;}
	dr.y=50;
	blit_scaled(src, &sr, screen, &dr);
}

void player_opt_work()
{
	static int is_turn_right=0;
//	static int sp_wait=0;

	SDL_Rect sr;
	sr.x=SP_X;
	sr.y=50;
	psp_clear_screen();
	//switch (select_mode)
	if (select_mode)
	{
	//case 4://選択
		SDL_BlitSurface(sp_bg,NULL,screen,NULL);
		SDL_BlitSurface(sp_st,NULL,screen,&sr);
		if (0==my_pad)//(sp_wait<0)	/*  */
		{
			if ((/*my_pad^*/my_pad_alter) & (PSP_KEY_LEFT|PSP_KEY_RIGHT|PSP_KEY_SHOT|PSP_KEY_CANCEL)/*左か右かショット」かキャンセルのいづれか*/  ) //左右
			{	/* 状態が変わる場合で */
				unloadbmp_by_surface(sp_bg);	sp_bg=NULL;
				unloadbmp_by_surface(sp_st);	sp_st=NULL;
			}
		//	if (my_pad & PSP_KEY_RIGHT/*keybo ard[KEY_RIGHT]*/) 	//右
		//	if (my_pad & PSP_KEY_LEFT /*keybo ard[KEY_LEFT]*/ ) 	//左
			if ((/*my_pad^*/my_pad_alter) & (PSP_KEY_LEFT|PSP_KEY_RIGHT)/*左か右の両方*/  ) //左右
			{
				is_turn_right=((/*my_pad^*/my_pad_alter) & PSP_KEY_RIGHT)?1:0;/*右なら1, 左なら0 */
				playChunk(2);
				tmp_sp_bg=loadbmp( (char *)player00_res[BASE_SP_BG_PNG+select_player] /*sp_bg_list[select_player]*/);
				tmp_sp_st=loadbmp( (char *)player00_res[BASE_SP_ST_PNG+select_player] /*sp_st_list[select_player]*/);
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
				sp_bg=loadbmp( (char *)player00_res[BASE_SP_BG_PNG+select_player] /*sp_bg_list[select_player]*/);
				sp_st=loadbmp( (char *)player00_res[BASE_SP_ST_PNG+select_player] /*sp_st_list[select_player]*/);
				SDL_SetColorKey(sp_st, SDL_SRCCOLORKEY, 0x00000000);
				select_mode=0/*(旧仕様は、右なら5, 左なら6)*/;
			//	sp_wait=10;
				sp_scale256 = t256(1);
				bg_alpha=0;
			}
			if ((/*my_pad^*/my_pad_alter) & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)
			{
				playChunk(1);
				newstate(ST_GAME_PLAY,0,1);
			}
			else
			if ((/*my_pad^*/my_pad_alter) & PSP_KEY_CANCEL/*keybo ard[KEY_CANCEL]*/)
			{
				playChunk(4);
				newstate(ST_MENU/*ST_INTRO*/,0/*1*/,1);
			}
		}
		//else
		//{
		//	sp_wait-=1/**fps_factor*/;
		//}
	}
	else
	{
//		break;
//	case 5: //右
		sp_scale256 -= t256(0.07);
		bg_alpha+=15;
		if (bg_alpha<255)
		{
			SDL_SetAlpha(sp_bg, SDL_SRCALPHA, bg_alpha);
		}
		else if (bg_alpha==255)
		{
			;
		}
		else
		{
			bg_alpha = 255;
			SDL_SetAlpha(sp_bg, SDL_SRCALPHA, bg_alpha);
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
			player_opt_img256(tmp_sp_st, sp_scale256, (is_turn_right)/*1*/);	// 5右1 6左0

//	case 5: //右
//			player_opt_img256(sp_st, t256(1)-sp_scale256, 0); // 5右0 6左1
//			player_opt_img256(tmp_sp_st, sp_scale256, 1);	// 5右1 6左0
//	case 6: //左
//			player_opt_img256(sp_st, t256(1)-sp_scale256, 1);
//			player_opt_img256(tmp_sp_st, sp_scale256, 0);
		}
//		break;
	}
}



/*-------------------*/

static SDL_Surface *intropic/*=NULL*/;
static void startmenu_init(void)
{
	setMusicVolume(128);/*とりあえず*/
	{
		if (NULL==intropic)
		{
			intropic=loadbmp("moon.jpg");
		}
	//	psp_clear_screen();
		SDL_SetAlpha(intropic, SDL_SRCALPHA, 255);
		SDL_BlitSurface(intropic, NULL, /*back_screen*/screen, NULL);
		unloadbmp_by_surface(intropic); 	intropic=NULL;/*unloadbmp_by_name("moon.jpg");*/
	}
	genericmenu_init(RES00_MAIN_MENU,&startmenu,0,/*6000*/-1/*500*/);/*現状HIGHSCOREで死ぬからとりあえず無し(仮)で*/
}

static void startmenu_work(void)
{
	if (!startmenu.select_finish)
	{
		genericmenu_work(&startmenu);
	}
	else
	{
		#if 0/*現状HIGHSCOREで死ぬからとりあえず無し(仮)で*/
		if (startmenu.timeout==-3)
		{
			/* Timeout, go on and show HighScore List */
			newstate(ST_SHOW_HCLIST,0,1);
		}
		else
		#endif
		{
			startmenu.select_finish=0;
			switch (startmenu.active_item)
			{
			case 0: 	practice_mode=0;	continue_stage=0;	newstate(ST_MENU,MENU_DIFF,1);	/*newstate(ST_PLAYER_SELECT,0,1); */	break; /* Start */				//newstate(ST_GAME_PLAY,0,1);
		//	case 1: 	newstate(ST_PLAYER_SELECT,0,1); 	break; /* Extra Start */		//newstate(ST_GAME_PLAY,0,1);
			case 1: 	practice_mode=1;	newstate(ST_MENU,MENU_STAGE_SELECT,1);		break;	/* stage select */	//newstate(ST_PLAYER_SELECT,0,1);	break; /* Practice Start */ 	//newstate(ST_GAME_PLAY,0,1);
		//	case 1: 	newstate(ST_MENU,MENU_OPTION,1);	break; /* Replay */
			case 2: 	newstate(ST_STORY,0,1); 			break; /* Story */
			case 3: 	newstate(ST_SHOW_HCLIST,0,1);		break; /* Result */ /* Hiscore */
		//	case 3: 	newstate(ST_MENU,MENU_OPTION,1);	break; /* Music Room */
			case 4: 	newstate(ST_MENU,MENU_OPTION,1);	break; /* Option */
			case 5: 	newstate(ST_GAME_QUIT,0,1); 		break; /* Quit */
			}
		}
	}
}
/*-------------------*/

void menu_init(void)
{
	switch (state.substate)
	{
	case MENU_START:		startmenu_init();			break;
	case MENU_PAUSE:		pausemenu_init();			break;
	case MENU_OPTION:		optionmenu_init();			break;
	//#if (1==DEBUG_MODE)
	case MENU_STAGE_SELECT: STAGE_SELECTmenu_init();	break;
	//#endif
	case MENU_VOLUME:		volumemenu_init();			break;
	case MENU_DIFF: 		difficultymenu_init();		break;
	}
}

void menu_work(void)
{
	switch (state.substate)
	{
	playChunk(2);
	case MENU_START:		startmenu_work();			break;
	case MENU_PAUSE:		pausemenu_work();			break;
	case MENU_OPTION:		optionmenu_work();			break;
	//#if (1==DEBUG_MODE)
	case MENU_STAGE_SELECT: STAGE_SELECTmenu_work();	break;
	//#endif
	case MENU_VOLUME:		volumemenu_work();			break;
	case MENU_DIFF: 		difficultymenu_work();		break;
	}
}

/*-------------------*/

void menusystem_init(void)
{
	continue_stage=0;
//	practice_mode=0;
	volume=0;
//
	startmenu.active_item=0;
	pausemenu.active_item=0;
	optionmenu.active_item=0;
	//#if (1==DEBUG_MODE)
	STAGE_SELECTmenu.active_item=0/*continue_stage*/;
	//#endif
	volumemenu.active_item=0/*volume*/;
	difficultymenu.active_item=0;
//	player_slmenu.active_item=0;
}

