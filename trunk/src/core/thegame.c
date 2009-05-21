
/*---------------------------------------------------------

---------------------------------------------------------*/

#include "support.h"

#include "enemy.h"
#include "loadlv.h"
#include "scenario.h"

extern STAGE_DATA *leveltab;

extern int select_player;
extern int practice_mode;

/*---------------------------------------------------------

---------------------------------------------------------*/

extern void add_clouds( 				STAGE_DATA *l);

/* ザコで宣言が必要なもの(グローバル) */
	//	その他
extern void add_enemy_all_clear(		STAGE_DATA *l);
	//	ボス
extern void add_boss_alice( 			STAGE_DATA *l);/*int lv*/
extern void add_boss_cirno( 			STAGE_DATA *l);/*int lv*/
extern void add_boss_aya(				STAGE_DATA *l);/*int lv*/
extern void add_boss_kaguya(			STAGE_DATA *l);/*int lv*/
extern void add_boss_sakuya(			STAGE_DATA *l);/*int lv*/
	//	[中型敵]妖怪
extern void add_chuu_youkai1(			STAGE_DATA *l);/*int lv*/

	//	その他ザコ
extern void add_zako_obake( 			STAGE_DATA *l);/*int lv*/
extern void add_zako_yukari1(			STAGE_DATA *l);/*int lv*/
extern void add_zako_yukari2(			STAGE_DATA *l);/*int lv*/
	//	毛玉
extern void add_zako_aka_kedama1(		STAGE_DATA *l);/*int lv*/
extern void add_zako_niji_kedama1(		STAGE_DATA *l);/*int lv*/
extern void add_zako_midori_kedama1(	STAGE_DATA *l);/*int lv*/
extern void add_zako_midori_kedama2(	STAGE_DATA *l);/*int lv*/
extern void add_zako_kedama1(			STAGE_DATA *l);/*int lv*/
	//	竜巻 陰陽玉
extern void add_zako_inyou1(			STAGE_DATA *l);/*int lv*/
extern void add_zako_tatsumaki1(		STAGE_DATA *l);/*int lv*/
	//	妖怪
extern void add_zako_kakomi1(			STAGE_DATA *l);/*int lv*/
	//	中妖精
extern void add_zako_meido1(			STAGE_DATA *l);/*int lv*/
extern void add_zako_meido2(			STAGE_DATA *l);/*int lv*/
extern void add_zako_meido3(			STAGE_DATA *l);/*int lv*/
extern void add_zako_meido4(			STAGE_DATA *l);/*int lv*/
	//	小妖精
extern void add_zako_ao_yousei1(		STAGE_DATA *l);/*int lv*/
extern void add_zako_ao_yousei2(		STAGE_DATA *l);/*int lv*/

extern void add_enemy_panels(			STAGE_DATA *l);/* const short xxx, const short yyy, const short speed256, int type*/

extern void bg2_control(				STAGE_DATA *l);

extern void add_enemy_load_bg(			STAGE_DATA *l);// unsigned char bg_type/*l->user_y*/, int err_time/*l->time*/
extern void add_enemy_eigo_string(		STAGE_DATA *l);// /*char *text, int y*/
extern void add_enemy_load_picture( 	STAGE_DATA *l);// /*char *filename,int xpos, int ypos*/


/*---------------------------------------------------------

---------------------------------------------------------*/

int difficulty=RANK_NORMAL;

static Uint32 stage_start_time;
//static Uint32 game_start_time;

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_error(STAGE_DATA *l)/*int lv*/
{
//	error(ERR_WARN,"unknown enemy ");
}

/*---------------------------------------------------------
	シューティング ゲーム データーの解析
	-------------------------------------------------------
	ここに追記すればするほど、シューティングゲーム本体が
	遅くなるので、注意して追記してくれ。
	字句解析(parth)等はloadlv.cで予め済ませておこう。
---------------------------------------------------------*/

static void thegame_level(STAGE_DATA *l)/*, int lev*/
{
	/* 中間コード形式のコマンドから各関数に分岐する */
	/* enemyの生成を番号で管理(loadlv.cのctype_name[]に対応している) */
	void (*aaa[ETYPE_MAX])(STAGE_DATA *l) =/*int*/
	{
		add_clouds,/* [番兵区切り] */	/*NULL*/	/* add_clouds();内部で見つからない場合は、enemy_error(); */
	//	その他
		add_enemy_all_clear,		/* ゲーム 全ステージ クリアー */
	//	ボス
		add_boss_alice,
		enemy_error/*add_boss_cirno*/,
		add_boss_aya,
		add_boss_kaguya,
		add_boss_sakuya,		// [***090207		追加
	//	特殊敵[中型敵]
		add_chuu_youkai1,			/*	="妖怪1",	ok	*/		// [***090207	追加
	//	その他ザコ
		add_zako_obake, 			/*	-"おばけ1", ok. */
		add_zako_yukari1,			/*	C"紫編隊1", ok? */		/*enemy_error*/
		add_zako_yukari2,			/*	C"紫編隊2", ok? */		/*enemy_error*/
	//	毛玉
		add_zako_aka_kedama1,		/*	a"赤毛玉1", ok. */
		add_zako_niji_kedama1,		/*	C"虹毛玉1", 	*/		/*enemy_error*/
		add_zako_midori_kedama1,	/*	-"緑毛玉1", ok. */
		add_zako_midori_kedama2,	/*	C"緑毛玉2", ok? */		/*enemy_error*/
		add_zako_kedama1,			/*	-"毛玉1",	ok. */
	//	竜巻 陰陽玉
		add_zako_inyou1,			/*	-"陰陽玉1", ok. */
		add_zako_tatsumaki1,		/*	C"竜巻1",	ok? */		/*enemy_error*/
	//	妖怪
		add_zako_kakomi1,			/*	-"囲妖怪1", ok. */
	//	中妖精
		add_zako_meido1,			/*	-"メイド1", ok. */
		add_zako_meido2,			/*	-"メイド2", ok. */
		add_zako_meido3,			/*	C"メイド3", ok--?	*/	/*enemy_error*/
		add_zako_meido4,			/*	-"メイド4", ok. */
	//	小妖精
		add_zako_ao_yousei1,		/*	="青妖精1", ok	*/		// [***090207	追加
		add_zako_ao_yousei2,		/*	="青妖精2", ok	*/		// [***090124	追加
//
		enemy_error,/* [番兵区切り] */	/*BTYPE_00_NONE*/
		add_enemy_panels,/*BTYPE_01_BGPANEL1*/
		add_enemy_panels,/*BTYPE_02_BGPANEL2*/
		add_enemy_panels,/*BTYPE_03_GROUNDER*/
		add_enemy_panels,/*BTYPE_04_MAGIC_FORMATION*/
		bg2_control,/*BTYPE_05_BG_CONTROL*/
//
		add_enemy_eigo_string,/*ETYPE_01_ENGLISH_TEXT*/
		add_enemy_load_bg,/*ETYPE_02_LOAD_BG*/
		add_enemy_load_picture,/*ETYPE_03_PICTURE*/
	};
	(*aaa[ (int)(l->user_command) ])(l);	/* 中間コード形式のコマンドから各関数に分岐する */
}

/*---------------------------------------------------------

---------------------------------------------------------*/
void init_stage_start_time(void)
{
	stage_start_time=psp_get_uint32_ticks();
}

/*---------------------------------------------------------
	シューティングゲーム本体の初期化
---------------------------------------------------------*/
extern int continue_stage;

#define TIME_20_DBWAIT /*20*/20/*2*/
static int dbwait/*=TIME_20_DBWAIT*/;		//ボスを倒したときに弾を消滅させるための時間確保
extern void set_rnd_seed(int set_seed);
extern /*int*/void load_stage(void/*int level*/);
extern void player_init(void);
extern void score_panel_init(void);
void common_load_init(void)
{
	set_rnd_seed(player_now_stage); 	/* 乱数系列の初期化 */
//
	/* Load next stage */
	load_stage();//if (load_stage(/*level*/)==0)	{	error(ERR_WARN,"no entrys for level %d",level);}
	// ロード中は処理落ちしているので、ロード後に時間を再作成する。
	init_stage_start_time();/*stage_start_time=psp_get_uint32_ticks();*/
//	game_start_time=psp_get_uint32_ticks();

//	stage_start_time=psp_get_uint32_ticks();
//	game_time=(psp_get_uint32_ticks()-stage_start_time);
//	game_time=(psp_get_uint32_ticks()-stage_start_time);
	//	play_music(BGM_01_stage1);	コメントアウト
	psp_loop = (ST_WORK_GAME_PLAY|0);
}
//	int stage;	stage=((PLAYER_DATA *)player->data)->stage;

void shooting_game_init(void)
{
	play_voice_auto_track(VOICE01_HIT);
//
	dbwait=TIME_20_DBWAIT;
//
	score_panel_init();
	//controller_remove_all();
	player_init();
//
	#if (1==USE_ENDING_DEBUG)
	/*player_init();より後の必要*/
	if (5==continue_stage)
	{	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	//	pd->bo ssmode	= B07_AF TER_LOAD;
		pd->state_flag	|= (STATE_FLAG_10_IS_LOAD_SCRIPT|STATE_FLAG_05_IS_BOSS|STATE_FLAG_11_IS_BOSS_DESTROY);
		continue_stage--;
		practice_mode=0;
	}
	#endif //(1==USE_ENDING_DEBUG)
	player_now_stage/*data->now_stage*/ /*level*/	= continue_stage/*+1-1*/ /*1*/;
//
	common_load_init();
}

extern void init_stage_start_time(void);
extern void player_stage_clear(void);
void stage_clear_work(void)
{
	//PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	//
	player_stage_clear();
//
	common_load_init();
}

/*---------------------------------------------------------
	ボス特殊処理
---------------------------------------------------------*/
static Uint32 game_time;
//	if (B02_BOSS_DESTROY==pd->bo ssmode) //ボスを倒したときの処理
//	if (/*STATE_FLAG_11_IS_BOSS_DESTROY==*/ (pd->state_flag & STATE_FLAG_11_IS_BOSS_DESTROY))
//	{
//	}
//	else
extern void player_loop_quit(void);
void boss_destroy(void) 	//ボスを倒したときの処理
{
		item_from_bullets(SP_ITEM_05_HOSI);
		//TIME_20_DBWAITフレーム待ってから実行。ボスを倒した時に画面に表示されている弾を全て消す処理のために必要。
		stop_music();
		play_voice_auto_track(VOICE03_BOSS_HAKAI);		// [***090313		追加	もっとスマートなやり方がありそうだけど思いつかなかった。
		;
//
	{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
		pd->state_flag &= (~(STATE_FLAG_13_DRAW_BOSS_GAUGE));/*off*/
//		pd->state_flag		&= (~(STATE_FLAG_03_SCORE_AUTO_GET_ITEM));		/* 終わり */
		if (0==practice_mode)/*練習モードではボス後イベントは見れないよ。*/
		{
		//	pd->bo ssmode=B07_AFTER_LOAD;
			pd->state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
		}
		else/*練習モード*/
		{
			player_loop_quit();
		}
	}
}
void incliment_scene(void)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	{
		/*ボス戦闘後イベント*/
	//	if (B09_STAGE_LOAD==pd->bo ssmode) // 9:stage読み込み
		if (/*STATE_FLAG_05_IS_BOSS == */(pd->state_flag & STATE_FLAG_05_IS_BOSS))
		{
			psp_loop = (ST_WORK_STAGE_CLEAR|0);
		}
		/*ボス戦闘前イベント*/
		else
	//	if (B08_START == pd->bo ssmode) // 8:ボス曲を鳴らし、1ボスとの戦闘へ。
		{
			set_music_volume(128);
			play_music( player_now_stage+8 );
			pd->state_flag |= (STATE_FLAG_05_IS_BOSS|STATE_FLAG_13_DRAW_BOSS_GAUGE);
		}
	}
}
//
extern void script_load(void);
void my_special(void)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
//	if (pd->bo ssmode==B05_BEFORE_LOAD) 	// [***090313	追加
//	if (pd->bo ssmode==B07_AFTER_LOAD)		// [***090313	追加
	if (pd->state_flag & STATE_FLAG_10_IS_LOAD_SCRIPT)		// [***090313	追加
	{
		pd->state_flag &= (~(STATE_FLAG_10_IS_LOAD_SCRIPT));/*off*/
		script_load(/*0 1*/);
	}
	if (pd->state_flag & (STATE_FLAG_11_IS_BOSS_DESTROY))
	{
	//	pd->state_flag |= STATE_FLAG_03_SCORE_AUTO_GET_ITEM;
	//	if (TIME_20_DBWAIT==dbwait)
	//	{
	//	}
	//	else
		dbwait--;
	//	if (0==dbwait)		// [***090313		変更
		if (1>dbwait)		// [***090313		変更
		{
			dbwait=TIME_20_DBWAIT;
		//
			pd->state_flag &= (~(STATE_FLAG_11_IS_BOSS_DESTROY));/*off*/
			boss_destroy();
		//
		}
	}
	/* スクリプトが終わった？ */
	if (pd->state_flag & (STATE_FLAG_12_END_SCRIPT))
	{
		pd->state_flag &= (~(STATE_FLAG_12_END_SCRIPT));/*off*/ 	/*	pd->bo ssmode=B00_NONE;*/
	//	pd->state_flag &= (~(STATE_FLAG_12_END_SCRIPT));/*off*/ 	/*	pd->bo ssmode=B00_NONE;*/	/*B01_BA TTLE*/
		incliment_scene();
	}
}
/*---------------------------------------------------------
	シューティングゲーム本体のメインルーチン
	-------------------------------------------------------
	ここに追記すればするほど、シューティングゲーム本体が
	遅くなるので、注意して追記してくれ。
	めったに実行しない物は関数化して外に追い出そう。
---------------------------------------------------------*/

extern void script_display(void);
extern void score_display(void);
extern void bg_work_draw(void);
extern void draw_score_chache(void);
void shooting_game_work(void)
{
//	if (psp_loop != (ST_WORK_GAME_PLAY&0xff00) /*|| state.newsta te==1*/) return;
	/* game_time=Zeit seit Spielbeginn in 1/10 sec. */
	game_time=(psp_get_uint32_ticks()-stage_start_time);
//
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->state_flag & STATE_FLAG_14_GAME_LOOP_QUIT)
	{
		;	/* GAMEOUT中 */
	}
	else
	{
		/* 生きてる */
	//	if (0==my_pad)
		if (0==(my_pad & PSP_KEY_PAUSE))
		{
			if (my_pad_alter & PSP_KEY_PAUSE)
			{
			//	if (0==(pd->state_flag & STATE_FLAG_06_IS_SCRIPT))/*たまにうまくいかない事がある*/
				{
					psp_loop=(ST_INIT_MENU|ST_MENU_SUB_PAUSE);//newsta te(ST_MENU,ST_MENU_SUB_PAUSE,1);
				}
			}
		}
		#if 1
		{
			STAGE_DATA *l;
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
					if (game_time >= l->time)
					#else
					if (v_time >= ((l->time) ) )
					#endif
					{
						thegame_level(l);	/*, *level*/
						l->done=1;	/* enemy set done flag */
					}
				}
				l=l->next;/* choice alter. */
			}
		}
		#else

		#endif
//
		/* 特殊処理 */
	//	if (B00_NONE != pd->bo ssmode)
		if (pd->state_flag & (STATE_FLAG_10_IS_LOAD_SCRIPT|STATE_FLAG_11_IS_BOSS_DESTROY|STATE_FLAG_12_END_SCRIPT))
		{
			my_special();
		}
//
	}
//
	#if 0/*ゲーム時間デバッグ用*/
	/* パネルのスコア欄にゲーム時間を 表示させる。っていうか書き換えちゃう。 */
	((PLAYER_DATA *)player->data)->score		= (game_time);
	#endif
//
	/* 動作 */
	bg_work_draw();
	//controller_work();
	sprite_work000(SP_GROUP_ALL);
//	sprite_work222(SP_GROUP_ALL);/*弾幕用*/
	/* 描画 */
	sprite_display000(SP_GROUP_ALL);
//	sprite_display222(SP_GROUP_ALL);/*弾幕用*/
	draw_score_chache();
	// この辺は速度低下するのでコールバックにすべき
	if (/*STATE_FLAG_06_IS_SCRIPT==*/(pd->state_flag & STATE_FLAG_06_IS_SCRIPT))				{	script_display();	}		//parsys_display();
	if (/*STATE_FLAG_09_IS_PANEL_WINDOW==*/(pd->state_flag & STATE_FLAG_09_IS_PANEL_WINDOW))	{	score_display();	}
}

/*---------------------------------------------------------
	ポーズメニュー用時間調整
	-------------------------------------------------------
	ポーズ中は時間が狂うので(ゲーム時間を)調整する為の仕組み
	メニュー(menu.c)のポーズから復帰する際にのみ使用する。
---------------------------------------------------------*/

void adjust_start_time(Uint32 pause_time)
{
	stage_start_time += psp_get_uint32_ticks()-pause_time;
}
