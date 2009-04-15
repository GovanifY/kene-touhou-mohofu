#include "thegame.h"

extern SDL_Surface *screen;

extern SPRITE *player;
extern GAMESTATE state;

extern HSC_LIST hsc_table[];
extern int lastscore;
extern LEVELENTRY *leveltab;

extern SDL_Surface *bg2;			//情報ウィンドウ
extern SDL_Surface *weapon_p;		//ウェポンゲージ
extern SDL_Surface *boss_hp;		//ボスHPゲージ

extern int is_bg_add;	//[***090202		追加
extern int is_bg_end;	//[***090202		追加
extern int is_bg_fin;	//[***090202		追加
extern int difficulty;
extern int select_player;
extern int practice_mode;


/* ザコで宣言が必要なもの(グローバル) */
extern void enemy_xev_add(int lv);
extern void enemy_crusher_add(int lv);
extern void enemy_eyefo_add(int lv);
extern void enemy_cube_add(int lv);
extern void enemy_drager_add(int lv);
extern void enemy_mine_add(int lv);
extern void enemy_rwingx_add(int lv);
extern void enemy_cir_add(int lv);
extern void enemy_zatak_add(int lv);
extern void enemy_curver_add(int lv);
extern void enemy_badguy_add(int lv);
extern void enemy_proball_add(int lv);
extern void enemy_plasmaball_add(int lv);
extern void enemy_ming_add(int lv);
extern void enemy_greeter_add(int lv);
extern void enemy_splash_add(int lv);
extern void enemy_fairy_add(int lv);
extern void enemy_greatfairy_add(int lv);
extern void enemy_boss01_add(int lv);
extern void enemy_boss02_add(int lv);
extern void enemy_boss03_add(int lv);
extern void enemy_boss04_add(int lv);



/*extern*/ int is_panel_window;

//static int world_y;
//static int world_dir=1;
//static int gframe;
static int gameover_delay;
static Uint32 level_start_time;
static Uint32 game_start_time;




static void enemy_error(int lv)
{
	error(ERR_WARN,"unknown enemy ");
}

static void thegame_level(LEVELENTRY *l/*, int lev*/)
{
	switch (l->command)
	{
	case 'E':		/* add enemy */
		#if 0
			 if (!strcmp(l->para1,"XEV"))			{	enemy_xev_add(l->para2);			}
		else if (!strcmp(l->para1,"CRUSHER"))		{	enemy_crusher_add(l->para2);		}
		else if (!strcmp(l->para1,"EYEFO")) 		{	enemy_eyefo_add(l->para2);			}
		else if (!strcmp(l->para1,"CUBE"))			{	enemy_cube_add(l->para2);			}
		else if (!strcmp(l->para1,"DRAGER"))		{	enemy_drager_add(l->para2); 		}
		else if (!strcmp(l->para1,"MINE"))			{	enemy_mine_add(l->para2);			}
		else if (!strcmp(l->para1,"RWINGX"))		{	enemy_rwingx_add(l->para2); 		}
		else if (!strcmp(l->para1,"CIR"))			{	enemy_cir_add(l->para2);			}
		else if (!strcmp(l->para1,"ZATAK")) 		{	enemy_zatak_add(l->para2);			}
		else if (!strcmp(l->para1,"CURVER"))		{	enemy_curver_add(l->para2); 		}
		else if (!strcmp(l->para1,"BADGUY"))		{	enemy_badguy_add(l->para2); 		}
		else if (!strcmp(l->para1,"PROBALL"))		{	enemy_proball_add(l->para2);		}
		else if (!strcmp(l->para1,"PLASMABALL"))	{	enemy_plasmaball_add(l->para2); 	}
		else if (!strcmp(l->para1,"MING"))			{	enemy_ming_add(l->para2);			}
		else if (!strcmp(l->para1,"GREETER"))		{	enemy_greeter_add(l->para2);		}
		else if (!strcmp(l->para1,"SPLASH"))		{	enemy_splash_add(l->para2); 		}		//[***090124		追加
		else if (!strcmp(l->para1,"FAIRY")) 		{	enemy_fairy_add(l->para2);			}		//[***090207		追加
		else if (!strcmp(l->para1,"GFAIRY"))		{	enemy_greatfairy_add(l->para2); 	}		//[***090207		追加
		else if (!strcmp(l->para1,"BOSS01"))		{	enemy_boss01_add(l->para2); 		}
		else if (!strcmp(l->para1,"BOSS02"))		{	enemy_boss02_add(l->para2); 		}
		else if (!strcmp(l->para1,"BOSS03"))		{	enemy_boss03_add(l->para2); 		}
		else if (!strcmp(l->para1,"BOSS04"))		{	enemy_boss04_add(l->para2); 		}
		else {			error(ERR_WARN,"unknown enemy '%s'",l->para1);		}
		#else
	{void (*aaa[])(int) =		/* enemyの生成を番号で管理(loadlv.cのctype_name[]に対応している) */
		{
		enemy_error,
		enemy_xev_add,
		enemy_crusher_add,
		enemy_eyefo_add,
		enemy_cube_add,
		enemy_drager_add,
		enemy_mine_add,
		enemy_rwingx_add,
		enemy_cir_add,
		enemy_zatak_add,
		enemy_curver_add,
		enemy_badguy_add,
		enemy_proball_add,
		enemy_plasmaball_add,
		enemy_ming_add,
		enemy_greeter_add,
		enemy_splash_add,		//[***090124		追加
		enemy_fairy_add,		//[***090207		追加
		enemy_greatfairy_add,	//[***090207		追加
		enemy_boss01_add,
		enemy_boss02_add,
		enemy_boss03_add,
		enemy_boss04_add,	//[***090207		追加
		};
		(*aaa[ (int)(/*l->*/ /*mem_pool_stage[jj].*/l->para0/*ctype*/) ])(/*l->*/ /*mem_pool_stage[jj].*/l->para2);
	}
		#endif
		break;
	case 'T':		/* text */
		enemy_gametext_add(l->para1,l->para2);
		break;
	case 'B':		/* Background */
		if (l->para2!=3 && l->para2!=9)
		{	bg_destroy();}
		switch (l->para2)
		{
		case 0: 	bg_init(BG_CLOUDS/*,lev*/); 		break; /* Clouds */
		case 1: 	bg_init(BG_STARS/*,lev*/);			break; /* Stars */
		case 2: 	bg_init(BG_TILE/*,lev*/);			break; /* Tile */
		case 3:
			if (!is_bg_end && !is_bg_add)
			{
				tile_add(/*lev*/);		//二枚目の背景
			}
			else
			{
				pspDebugScreenPrintf("Too early\n please be later");
				sceKernelDelayThread(1000000);
			}
			break;
		case 9:
			is_bg_fin=1;		//ループしない
			break;
		default:
			error(ERR_WARN,"unknown background %d, using clouds",l->para2);
			bg_init(BG_CLOUDS/*,lev*/);
			break;
		}
		break;
	case 'P':		/* [***090313	追加	 Picture */
		enemy_gameimg_add(l->para1,l->para0,l->para2);
		break;
	// add background tiles....
	case BTYPE_01_BGPANEL1: 		enemy_bgpanel_type_add_xy(	l->para0, l->para2, l->para3, /*type*/1-1);	break;
	case BTYPE_02_BGPANEL2: 		enemy_bgpanel_type_add_xy(	l->para0, l->para2, l->para3, /*type*/2-1);	break;
	case BTYPE_03_GROUNDER: 		enemy_grounder_add_xy(		l->para0, l->para2, l->para3);				break;
	case BTYPE_04_MAGIC_FORMATION:	enemy_magicformation_add_xy(l->para0, l->para2, l->para3);				break;
	}
}

extern int continue_stage;

#define TIME_20_DBWAIT /*20*/20/*2*/
static int dbwait/*=TIME_20_DBWAIT*/;		//ボスを倒したときに弾を消滅させるための時間確保
void thegame_init(void)
{
	playChunk(1);

	dbwait=TIME_20_DBWAIT;
	bg2=loadbmp("bg2.png");
//	gframe=0;
	gameover_delay=100;
//
	weapon_p			= loadbmp("weapon_p.png");
	SDL_SetColorKey(weapon_p,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	boss_hp 			= loadbmp2("health.png");
//
	controller_remove_all();
	player_init();
//


//
	#if (1==USE_ENDING_DEBUG)
	/*player_init();より後の必要*/
	if (5==continue_stage)
	{	PLAYER_DATA *d=(PLAYER_DATA *)player->data;
		d->bossmode		= B07_AFTER_LOAD;
		continue_stage--;
		practice_mode=0;
	}
	#endif //(1==USE_ENDING_DEBUG)
	player_now_stage/*data->now_stage*/ /*level*/	= continue_stage/*+1-1*/ /*1*/;


	game_start_time=PSP_GetTicks();
	level_start_time=PSP_GetTicks();
//	int level;	level=((PLAYER_DATA *)player->data)->level;


//	level_start_time=PSP_GetTicks();
//	gt=(PSP_GetTicks()-level_start_time); //denis
	loadlv();//if (loadlv(/*level*/)==0)	{	error(ERR_WARN,"no entrys for level %d",level);}
	//	play_music(BGM_01_stage1);	コメントアウト
}


extern void game_clear_set_password(void);
void thegame_work(void)
{
	PLAYER_DATA *d=(PLAYER_DATA *)player->data;
	Uint32 gt;


//	int *level=&d->level;

	if (state.mainstate!=ST_GAME_PLAY || state.newstate==1) return;

	/* gt=Zeit seit Spielbeginn in 1/10 sec. */
	gt=(PSP_GetTicks()-level_start_time);


	if (d->lives<0)
	{
		/* GAMEOUT中 */
		player->flags &= ~SP_FLAG_VISIBLE;
		//PLAYER_DATA *d=(PLAYER_DATA *)player->data;
		//d->core->alpha=0; 		//○を消すために入れたけど意味無かったかもしれない。
		//ここがダメっぽい
		if (gameover_delay)
		{
			gameover_delay--;
		}
		else
		{
			bg_destroy();
			lastscore=((PLAYER_DATA *)player->data)->score;
//		//	controller_remove_all();
//		//	sprite_remove_all(SP_SHOW_ALL);
//			parsys_remove_all();
//			score_cleanup();
			newstate(ST_GAME_OVER,0,1);
		}
	}
	else
	{
		/* 生きてる */
		if ((d->bossmode!=B03_BEFORE_EVENT) && (d->bossmode!=B06_AFTER_EVENT))
		{
			if (my_pad & PSP_KEY_PAUSE/*keybo ard[KEY_PAUSE]*/)
			{
				newstate(ST_MENU,MENU_PAUSE,1);
			}
		}
			#if 1
			{LEVELENTRY *l;
			/*
				This routine, serch back to begin.
				このルーチンは逆順に検索します。
			*/
				l = leveltab;
				while (l != NULL)/* [head ==NULL] then end. */
				{
					if (l->done == 0)	/* enemy set done flag */
					{
						#if 1
						if (gt >= l->time)
						#else
						if (v_time >= ((l->time) ) )
						#endif
						{
							thegame_level(l/*, *level*/);
							l->done=1;	/* enemy set done flag */
						}
					}
					l=l->next;/* choice alter. */
				}
			}
			#else

			#endif
//
		if (d->bossmode==B05_BEFORE_LOAD)		//[***090313		追加
		{
			char buffer2[16/*10*/];
			sprintf(buffer2,"stage%d-%d",player_now_stage,select_player);
			setMusicVolume(80);
			if (!script_init(buffer2, NULL, 380))
			{	d->bossmode=B08_START;		}
			else{	d->bossmode=B03_BEFORE_EVENT;	}
		}
		else
		if (d->bossmode==B07_AFTER_LOAD)		//[***0903		追加
		{
			char buffer1[16/*10*/];
			sprintf(buffer1,"stage%d-%d_end",player_now_stage,select_player);
			if (!script_init(buffer1, NULL, 380))		//ファイルがない場合はイベントを飛ばす
			{	d->bossmode=B09_STAGE_LOAD;	}
			else{	d->bossmode=B06_AFTER_EVENT;	}
		}
		if (d->bossmode==B02_DEATH_WAIT) //ボスを倒したときの処理
		{
		//	if (TIME_20_DBWAIT==dbwait)
		//	{
		//	}
		//	else
			if (0==dbwait)		//[***090313		変更
			{
				//TIME_20_DBWAITフレーム待ってから実行。ボスを倒した時に画面に表示されている弾を全て消す処理のために必要。
				clear_music();
				playChunk(3);		//[***090313		追加	もっとスマートなやり方がありそうだけど思いつかなかった。
				;
				if (0==practice_mode)/*練習モードではボス後イベントは見れないよ。*/
				{
					d->bossmode=B07_AFTER_LOAD;
				}
				else/*練習モード*/
				{
					d->lives=0-1;/* 死に中 */
				}
			}
			dbwait--;
		}
		else
		if (d->bossmode==B09_STAGE_LOAD) //	9:stage読み込み
		{
			//PLAYER_DATA *d=(PLAYER_DATA *)player->data;
			d->bossmode=B00_NONE;
			dbwait=TIME_20_DBWAIT;
			level_start_time=PSP_GetTicks();
			gt=(PSP_GetTicks()-level_start_time); //denis
			/* Load next level */
			loadlv();//if (loadlv(/**level*/)==0)	{	error(ERR_WARN,"no entrys for level %d",*level);}
		}
		else
		if (d->bossmode==B08_START) //	8:ボス曲を鳴らし、1ボスとの戦闘へ。
		{
			setMusicVolume(128);
			play_music( player_now_stage+8 );
			d->bossmode=B01_BATTLE;
		}
//
		/* エンディング後の終了判定 */
		if (/**level*/player_now_stage==6	)
		{
			if (gt>125000000/*60500000*/)/*125.0[sec]←60.5[sec]*/
			{
				//	error(ERR_DEBUG,"sorry, no more levels in this alpha-version");
				d->score+=d->lives*(2000+(difficulty*4000));
				d->lives=0-1;/* 死に中 */
				game_clear_set_password();
				//return;
			}
		}
	}

	bg_work();
	controller_work();
	sprite_work(SP_SHOW_ALL);
	sprite_display(SP_SHOW_ALL);
	if (d->bossmode==B03_BEFORE_EVENT)		{	if (thescript()==1)	{	d->bossmode=B08_START;		}	}	//[***090313	追加
	else if (d->bossmode==B06_AFTER_EVENT)	{	if (thescript()==1)	{	d->bossmode=B09_STAGE_LOAD;	}	}	//[***090313	追加	ここに移動。
	//parsys_display();
	if (1==is_panel_window)		{	score_display();}
//	gframe++;
}

void adjust_start_time(Uint32 pause_time)
{
	level_start_time+=PSP_GetTicks()-pause_time;
}
