#include "thegame.h"

extern SDL_Surface *screen;
extern int keyboard[];
extern SPRITE *player;
extern GAMESTATE state;
extern KEYCONFIG keyconfig;
extern HSC_LIST hsc_table[];
extern int lastscore;
extern LEVELENTRY *leveltab;
extern SDL_Surface *bg2;		//情報ウィンドウ

extern int is_bg_add;	//[***090202		追加
extern int is_bg_end;	//[***090202		追加
extern int is_bg_fin;	//[***090202		追加
extern int n_bg;		//[***090209		追加
extern int difficulty;
extern int select_player;


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

//static int world_y;
//static int world_dir=1;
//static int gframe;
static int gameover_delay;
static Uint32 level_start_time;
static Uint32 game_start_time;

static int screennum=0;
#define TIME_20_DBWAIT 20/*2*/
static int dbwait=TIME_20_DBWAIT;		//ボスを倒したときに弾を消滅させるための時間確保




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
			 if (!strcmp(l->para1,"XEV"))		{	enemy_xev_add(l->para2);			}
		else if (!strcmp(l->para1,"CRUSHER"))	{	enemy_crusher_add(l->para2);		}
		else if (!strcmp(l->para1,"EYEFO")) 	{	enemy_eyefo_add(l->para2);			}
		else if (!strcmp(l->para1,"CUBE"))		{	enemy_cube_add(l->para2);			}
		else if (!strcmp(l->para1,"DRAGER"))	{	enemy_drager_add(l->para2); 		}
		else if (!strcmp(l->para1,"MINE"))		{	enemy_mine_add(l->para2);			}
		else if (!strcmp(l->para1,"RWINGX"))	{	enemy_rwingx_add(l->para2); 		}
		else if (!strcmp(l->para1,"CIR"))		{	enemy_cir_add(l->para2);			}
		else if (!strcmp(l->para1,"ZATAK")) 	{	enemy_zatak_add(l->para2);			}
		else if (!strcmp(l->para1,"CURVER"))	{	enemy_curver_add(l->para2); 		}
		else if (!strcmp(l->para1,"BADGUY"))	{	enemy_badguy_add(l->para2); 		}
		else if (!strcmp(l->para1,"PROBALL"))	{	enemy_proball_add(l->para2);		}
		else if (!strcmp(l->para1,"PLASMABALL")){	enemy_plasmaball_add(l->para2); 	}
		else if (!strcmp(l->para1,"MING"))		{	enemy_ming_add(l->para2);			}
		else if (!strcmp(l->para1,"GREETER"))	{	enemy_greeter_add(l->para2);		}
		else if (!strcmp(l->para1,"SPLASH"))	{	enemy_splash_add(l->para2); 		}		//[***090124		追加
		else if (!strcmp(l->para1,"FAIRY")) 	{	enemy_fairy_add(l->para2);			}		//[***090207		追加
		else if (!strcmp(l->para1,"GFAIRY"))	{	enemy_greatfairy_add(l->para2); 	}		//[***090207		追加
		else if (!strcmp(l->para1,"BOSS01"))	{	enemy_boss01_add(l->para2); 		}
		else if (!strcmp(l->para1,"BOSS02"))	{	enemy_boss02_add(l->para2); 		}
		else if (!strcmp(l->para1,"BOSS03"))	{	enemy_boss03_add(l->para2); 		}
		else if (!strcmp(l->para1,"BOSS04"))	{	enemy_boss04_add(l->para2); 		}
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
void thegame_init(void)
{
	player_now_stage/*data->now_stage*/ /*level*/	= continue_stage/*+1-1*/ /*1*/;

					playChunk(1);

	dbwait=TIME_20_DBWAIT;
	bg2=loadbmp("bg2.png");
//	gframe=0;
	gameover_delay=100;

	is_bg_add=0;
	is_bg_end=0;
	is_bg_fin=0;
	n_bg=0;


	controller_remove_all();
	player_init();
	game_start_time=PSP_GetTicks();
	level_start_time=PSP_GetTicks();

//	int level;	level=((PLAYER_DATA *)player->data)->level;

	loadlv();//if (loadlv(/*level*/)==0)	{	error(ERR_WARN,"no entrys for level %d",level);}
	//	play_music(BGM_01_stage1);	コメントアウト
}


void thegame_work(void)
{
/*
	bossmodeについて
	0:特になし。
	1:ボスとの戦闘中。
	2:ボス撃破。終了イベントファイル読み込み。
	3:ボス開始イベント。
	4:ボス形態変更。強敵撃破。
	5:開始イベントファイル読み込み。
	6:ボス終了イベント。
*/
	PLAYER_DATA *d=(PLAYER_DATA *)player->data;
	Uint32 gt;
	LEVELENTRY *l;
	static int button_d;
//	int *level=&d->level;

	if (state.mainstate!=ST_GAME_PLAY || state.newstate==1) return;

	/* gt=Zeit seit Spielbeginn in 1/10 sec. */
	gt=(PSP_GetTicks()-level_start_time);

	if (keyboard[KEY_SC_SHOT])		//スクリーンショット機能。keypollに入れると何故かうまくいかなかったのでこっちに場所を変更。
	{
	/*static*/ char screenbuf[32/*20*/];
		sprintf(screenbuf,"ms0:/PICTURE/Toho_Moho%d.bmp",screennum++);		//保存場所の変更。
		SDL_SaveBMP(screen,screenbuf);
	}
	if (d->lives<0)
	{
		/* GAMEOUT中 */
		player->flags&=~SP_FLAG_VISIBLE;
		//PLAYER_DATA *d=(PLAYER_DATA *)player->data;
		//d->core->alpha=0; 		//○を消すために入れたけど意味無かったかもしれない。
		//ここがダメっぽい
		if (gameover_delay) {
			gameover_delay--;
		} else {
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
		if (keyboard[KEY_PAUSE])
		{
			if(d->bossmode!=3 && d->bossmode!=6 && button_d<0)
			{
				newstate(ST_MENU,MENU_PAUSE,1);
			}
			else
			{
				button_d=20;
			}
		}
		else{	button_d--;	}

		l=leveltab;
		while (l!=NULL)
		{
			if ((gt>=l->time)&&(l->done==0))
			{
				thegame_level(l/*, *level*/);
				l->done=1;
			}
			l=l->next;
		}

		if (d->bossmode==2) //ボスを倒したときの処理
		{
			clear_music();
			if (dbwait==0)		//[***090313		変更
			{ //TIME_20_DBWAITフレーム待ってから実行。ボスを倒した時に画面に表示されている弾を全て消す処理のために必要。
				char buffer1[16/*10*/];
				sprintf(buffer1,"stage%d-%d_end",player_now_stage,select_player);
				if(!script_init(buffer1, NULL,380)){		//ファイルがない場合はイベントを飛ばす
					dbwait=TIME_20_DBWAIT;
					d->bossmode=0;
					level_start_time=PSP_GetTicks();
					gt=(PSP_GetTicks()-level_start_time);
					loadlv();
					is_bg_add=0;
					is_bg_end=0;
					is_bg_fin=0;
					n_bg=0;
				}
				else{	d->bossmode=6;	}
			}
			dbwait--;
		}
		/* エンディング後の終了判定 */
		if (/**level*/player_now_stage==6	)
		{	if (gt>60500000)
			{
			//	error(ERR_DEBUG,"sorry, no more levels in this alpha-version");
			d->score+=d->lives*(2000+(difficulty*4000));
			d->lives=0-1;/* 死に中 */
			if(difficulty>0)
			{
				/******自分でコンパイルする人へ******
				このままだと配布バイナリと違うので
				thegame.c	:	と
				menu.c		:	の
				文字列"無駄"を適当に変えといて下さい。
				************************************/
				strcpy(password,"RUMIAISMYBRIDE");
			}
			return;
			}
		}
	}

	bg_work();
	controller_work();
	sprite_work(SP_SHOW_ALL);
	sprite_display(SP_SHOW_ALL);
	if(d->bossmode==5)		//[***090313		追加
	{
		char buffer2[16/*10*/];
		sprintf(buffer2,"stage%d-%d",player_now_stage,select_player);
		setMusicVolume(80);
		if(!script_init(buffer2, NULL,380)){	d->bossmode=1;	}
		else{	d->bossmode=3;	}
	}
	else if(d->bossmode==3)		//[***090313		追加
	{
		if(thescript()==1){
			d->bossmode=1;
			play_music( player_now_stage+8 );
			setMusicVolume(128);
		}
	}
	else if(d->bossmode==6)		//[***090313		追加	ここに移動。
	{
		if(thescript()==1){
			dbwait=TIME_20_DBWAIT;
			d->bossmode=0;
			level_start_time=PSP_GetTicks();
			gt=(PSP_GetTicks()-level_start_time); //denis
			/* Load next level */
			loadlv();//if (loadlv(/**level*/)==0)	{	error(ERR_WARN,"no entrys for level %d",*level);}
			is_bg_add=0;
			is_bg_end=0;
			is_bg_fin=0;
			n_bg=0;
		}
	}
	//parsys_display();
	score_display();
//	gframe++;
}

void adjust_start_time(Uint32 pause_time)
{
	level_start_time+=PSP_GetTicks()-pause_time;
}
