
/*---------------------------------------------------------

---------------------------------------------------------*/

#include "game_main.h"
//#include "bonus.h"

#include "bullet_object.h"
#include "load_stage.h"
#include "scenario_script.h"

extern STAGE_DATA *stage_data_table;

extern int select_player;
extern int practice_mode;

/*---------------------------------------------------------
	ザコで宣言が必要なもの(グローバル)
---------------------------------------------------------*/

	/* その他(特殊機能) */
extern void add_clouds( 				STAGE_DATA *l);/* どの敵でもない場合は、演出用画像 */
extern void add_enemy_all_clear(		STAGE_DATA *l);/* 全面クリアーの場合、この敵を追加 */
extern void bg2_control(				STAGE_DATA *l);

	/* ボス */
extern void add_boss_kene(				STAGE_DATA *l);/* extra 2面 */
extern void add_boss_cirno( 			STAGE_DATA *l);/* extra 1面 */
//
extern void add_boss_sakuya(			STAGE_DATA *l);/* 6面は咲夜 */
extern void add_boss_pache( 			STAGE_DATA *l);/* 5面はパチュリー */
extern void add_boss_kaguya(			STAGE_DATA *l);/* 4面は輝夜 */
extern void add_boss_mitei( 			STAGE_DATA *l);/* 3面はどうするか未定 */
extern void add_boss_aya(				STAGE_DATA *l);/* 2面は文 */
extern void add_boss_alice( 			STAGE_DATA *l);/* 1面はアリス */
	/* [中型敵]妖怪 */
extern void add_chuu_koakuma(			STAGE_DATA *l);
extern void add_chuu_rumia( 			STAGE_DATA *l);
extern void add_chuu_youkai1(			STAGE_DATA *l);
extern void add_chuu_youkai2(			STAGE_DATA *l);
	/* 魔方陣 */
extern void add_enemy_mahoujin( 		STAGE_DATA *l);

	/* 竜巻 陰陽玉 */
extern void add_zako_inyou1(			STAGE_DATA *l);
extern void add_zako_tatsumaki1(		STAGE_DATA *l);
	/* 妖怪 */
extern void add_zako_kakomi1(			STAGE_DATA *l);
	/* その他ザコ */
extern void add_zako_obake1(			STAGE_DATA *l);
extern void add_zako_obake2(			STAGE_DATA *l);
extern void add_zako_yukari1(			STAGE_DATA *l);
extern void add_zako_yukari2(			STAGE_DATA *l);
	/* 毛玉 */
extern void add_zako_aka_kedama1(		STAGE_DATA *l);
extern void add_zako_midori_kedama1(	STAGE_DATA *l);
extern void add_zako_midori_kedama2(	STAGE_DATA *l);
extern void add_zako_kedama1(			STAGE_DATA *l);
extern void add_zako_kedama2(			STAGE_DATA *l);
	/* 中妖精 */
extern void add_zako_meido1(			STAGE_DATA *l);
extern void add_zako_meido2(			STAGE_DATA *l);
extern void add_zako_meido3(			STAGE_DATA *l);
extern void add_zako_meido4(			STAGE_DATA *l);
	/* 小妖精 */
extern void add_zako_ao_yousei1(		STAGE_DATA *l);
extern void add_zako_ao_yousei2(		STAGE_DATA *l);
extern void add_zako_ao_yousei3(		STAGE_DATA *l);
extern void add_zako_ao_yousei4(		STAGE_DATA *l);
//

extern void add_enemy_load_bg(			STAGE_DATA *l);
extern void add_enemy_kanji_string( 	STAGE_DATA *l);

/* この方式は処理落ち解消しにくい(Gu化も難しい)ので都合により廃止 */
//extern void add_enemy_load_picture(	STAGE_DATA *l);


/*---------------------------------------------------------

---------------------------------------------------------*/

int difficulty = RANK_EASY/*  RANK_NORMAL*/;

//static Uint32 stage_start_time;
//static Uint32 game_start_time;

/*---------------------------------------------------------
	敵の追加
	-------------------------------------------------------
	ここに追記するとシューティングゲーム本体が遅くなるので追記しません。
	字句解析(parth)等は load_stage.c で予め済ませておきます。
---------------------------------------------------------*/

static void add_all_teki(STAGE_DATA *l)
{
	/* 中間コード形式のコマンドから各関数に分岐する */
	/* enemyの生成を番号で管理(load_stage.c の ctype_name[]に対応している) */
	void (*aaa[ETYPE_MAX])(STAGE_DATA *l) =
	{
		add_clouds,/* [番兵区切り] */	/*NULL*/	/* add_clouds();内部で見つからない場合は、enemy_error(); */
	/* その他 */
		add_enemy_all_clear,		/* ゲーム 全ステージ クリアー */
		bg2_control,/*CTYPE_02_BG_CONTROL*/
	/* ボス */
		add_boss_mitei, 			/*add_boss_kene*/		/* extra 2面 */
		add_boss_pache, 			/*add_boss_cirno*/		/* extra 1面 */
		add_boss_sakuya,			/* 6面は咲夜 */ 		// [***090207		追加
		add_boss_pache, 			/* 5面はパチュリー */	//		追加
		add_boss_kaguya,			/* 4面は輝夜 */
		add_boss_mitei, 			/* 3面はどうするか未定(永琳? 慧音?) */
		add_boss_aya,				/* 2面は文 */
		add_boss_alice, 			/* 1面はアリス */
	/* 特殊敵[中型敵] */
		add_chuu_rumia, 			/*	="ルーミア" */		/*	6面は? */
		add_chuu_rumia, 			/*add_chuu_koakuma*/	/*	5面は="小悪魔" */
		add_chuu_rumia, 			/*add_chuu_udonge*/ 	/*	4面は鈴仙? */
		add_chuu_rumia, 			/*add_chuu_tei*/		/*	3面はてゐ? */
		add_chuu_rumia, 			/*	="ルーミア" */		/*	2面は天狗? */
		add_chuu_rumia, 			/*	="ルーミア" */		/*	1面はルーミア */
//		廃止?
		add_chuu_youkai2,			/*	="妖怪2"	*/		// [***090207	追加
		add_chuu_youkai1,			/*	="妖怪1"	*/		// [***090207	追加
	/* 魔方陣 */
		add_enemy_mahoujin, 		/*	""	*/
		add_enemy_mahoujin, 		/*	""	*/
	/* 竜巻 陰陽玉 */
		add_zako_inyou1,			/*	-"陰陽玉1"	*/
		add_zako_tatsumaki1,		/*	C"竜巻1"	*/		/*enemy_error*/
	/* 妖怪 */
		add_zako_kakomi1,			/*	-"囲妖怪1"	*/
	/* その他ザコ */
		add_zako_obake1,			/*	-"おばけ1"	*/
		add_zako_obake2,			/*	-"おばけ2"	C"虹毛玉1"	*/		/*enemy_error*/
		add_zako_yukari1,			/*	C"紫編隊1"	*/		/*enemy_error*/
		add_zako_yukari2,			/*	C"紫編隊2"	*/		/*enemy_error*/
	/* 毛玉 */
		add_zako_aka_kedama1,		/*	a"赤毛玉1"	*/
		add_zako_midori_kedama1,	/*	-"緑毛玉1"	*/
		add_zako_midori_kedama2,	/*	C"緑毛玉2"	*/		/*enemy_error*/
		add_zako_kedama1,			/*	-"毛玉1"	*/
		add_zako_kedama2,			/*	-"毛玉2"	*/
	/* 中妖精 */
		add_zako_meido1,			/*	-"メイド1"	*/
		add_zako_meido2,			/*	-"メイド2"	*/
		add_zako_meido3,			/*	C"メイド3"	*/		/*enemy_error*/
		add_zako_meido4,			/*	-"メイド4"	*/
	/* 小妖精 */
		add_zako_ao_yousei1,		/*	="青妖精1"	*/		// [***090207	追加
		add_zako_ao_yousei2,		/*	="青妖精2"	*/		// [***090124	追加
		add_zako_ao_yousei3,		/*	="青妖精3"	*/		//	追加
		add_zako_ao_yousei4,		/*	="青妖精4"	*/		//	追加
//
//
		add_enemy_kanji_string, 	/* ETYPE_01_SJIS_TEXT */
		add_enemy_load_bg,			/* ETYPE_02_LOAD_BG */
	//	add_enemy_load_picture, 	/* ETYPE_03_PICTURE */		/* この方式は処理落ち解消しにくい(Gu化も難しい)ので都合により廃止 */
	};
	(*aaa[ (int)(l->user_i_code) ])(l); 	/* 中間コード形式のコマンドから各関数に分岐する */
}


/*---------------------------------------------------------

---------------------------------------------------------*/
static Uint32 game_v_time;
static void init_stage_start_time(void)
{
//	stage_start_time = psp_get_uint32_ticks();
	game_v_time = 0;
}


/*---------------------------------------------------------
	シューティングゲーム本体の初期化
---------------------------------------------------------*/
extern int continue_stage;

extern void set_rnd_seed(int set_seed);
extern /*int*/void load_stage(void/*int level*/);
extern void player_init(void);
extern void score_panel_init(void);
void common_load_init(void)
{
	set_rnd_seed(player_now_stage); 	/* 乱数系列の初期化 */
//
	/* Load next stage */
	load_stage();//if (0==load_stage(/*level*/))	{	error(ERR_WARN, "no entrys for level %d",level);}
	// ロード中は処理落ちしているので、ロード後に時間を再作成する。
	init_stage_start_time();/*stage_start_time = psp_get_uint32_ticks();*/
//	game_start_time = psp_get_uint32_ticks();

//	stage_start_time = psp_get_uint32_ticks();
//	game_v_time = (psp_get_uint32_ticks()-stage_start_time);
//	game_v_time = (psp_get_uint32_ticks()-stage_start_time);
	//	play_music_num(BGM_01_stage1);	コメントアウト
//
	script_message_window_clear();/*スクリプトメッセージ画面を消す*/
	psp_loop = (ST_WORK_GAME_PLAY|0);
}
//	int stage;	stage=((PLAYER_DATA *)player->data)->stage;


/*---------------------------------------------------------
	ゲームコア初回(ゲーム開始時)限定の初期化
---------------------------------------------------------*/

extern void player_result_init(void);
//void shooting_game_core_1st_init(void)
void stage_first_init(void)
{
	#if (000)/* ??? */
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE01_MENU_OK);
		#else
		voice_play(VOICE01_MENU_OK, TRACK01_EXPLODE);
		#endif
	#endif
//
	score_panel_init();
	player_result_init();
	//sprite_controller_remove_all();
	player_init();
//
	script_message_window_clear();
//
//	#if (1 == US E_ENDING_DEBUG)
//	/*player_init();より後の必要*/
//	if (MA X_STAGE6_FOR_CHECK == continue_stage)
//	{	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
//	//	pd->bo ssmode	= B07_AF TER_LOAD;
//		pd->state_flag	|= (STATE_FLAG_10_IS_LOAD_SCRIPT|STATE_FLAG_05_IS_BOSS|ST ATE_FLAG_11_IS_BOSS_DESTROY);
//		continue_stage--;
//		practice_mode = 0;
//	}
//	#endif //(1==US E_ENDING_DEBUG)
	player_now_stage/*data->now_stage*/ /*level*/	= continue_stage/*+1-1*/ /*1*/;
//
	psp_loop = (ST_INIT_GAME_PLAY_common|0);
//	common_load_init();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

void incliment_scene(void)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
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
			if (0!=boss_bgm_mode)
			{	/* ボスイベント後にボス曲 */
				set_music_volume(128);/* 曲音量戻す */
				play_music_num( (BGM_10_boss1-1)+player_now_stage );/* ボス曲鳴らす */
			}
		//	else
		//	{	/* ボスイベント前にボス曲 */
		//	}
			pd->state_flag |= (STATE_FLAG_05_IS_BOSS|STATE_FLAG_13_DRAW_BOSS_GAUGE);
		}
	}
}
//
extern void script_load(void);
/* 注意：static関数にしない */void my_special(void)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
//	if (pd->bo ssmode==B05_BEFORE_LOAD) 	// [***090313	追加
//	if (pd->bo ssmode==B07_AFTER_LOAD)		// [***090313	追加
	if (pd->state_flag & (STATE_FLAG_10_IS_LOAD_SCRIPT))		// [***090313	追加
	{
		pd->state_flag &= (~(STATE_FLAG_10_IS_LOAD_SCRIPT));/*off*/
		script_load(/*0 1*/);
	}
//	if (pd->state_flag & (ST ATE_FLAG_11_IS_BOSS_DESTROY))
//	{
//		pd->state_flag &= (~(ST ATE_FLAG_11_IS_BOSS_DESTROY));/*off*/
//		boss_destroy_aaa();
//	}
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

extern void vbl_draw_screen(void);/*support.c*/

extern void script_display(void);
extern void score_display(void);
extern void bg_work_draw(void);
extern void draw_score_chache(void);
void shooting_game_core_work(void)
{
	while ((ST_WORK_GAME_PLAY>>8) == (psp_loop>>8) )
	{
	//	if (psp_loop != (ST_WORK_GAME_PLAY&0xff00) /*|| state.newsta te==1*/) return;
		/* game_v_time=Zeit seit Spielbeginn in 1/10 sec. */
		game_v_time++;//=(psp_get_uint32_ticks()-stage_start_time);
	//
		PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
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
				l = stage_data_table;
				while (NULL != l)	/* 敵リストの終わり(NULL)まで調べる */	/* [head ==NULL] then end. */
				{
					/* 処理済み？ */
					if (0 < l->v_time )//if (0 == l->done ) 	/* enemy set done flag */
					{
					//	#if 1
						if (game_v_time >= (l->v_time)) 	/* (現在時間 >= 設定時間) なら、敵をセット */
					//	#else
					//	if (v_time >= ((l->time) ) )
					//	#endif
						{
							add_all_teki(l);	/* 敵をセット */
							l->v_time = (-1); 	/* 処理済みをマーク */	/* enemy set done flag */	//l->done = 1;
						}
					}
					l = l->next;	/* 次を調べる */	/* choice alter. */
				}
			}
			#else

			#endif
			/*	[旧]特殊処理のあった位置 */
		}
//
			/*
				★「ボスと相打ちするとハングアップ」バグ(～r26)対策
				[新]特殊処理の位置:
				ボスを倒した場合に自分が死んでいて、喰らいボム判定中の場合、
				[旧]特殊処理の位置では、喰らいボム判定の為STATE_FLAG_14_GAME_LOOP_QUITなので
				ST ATE_FLAG_11_IS_BOSS_DESTROY(特殊処理で判定)が判定できないバグ(～r26)がある。
				そこで位置を動かした。(ST ATE_FLAG_11_IS_BOSS_DESTROYの判定は少なくともこちら側でする必要がある)
			 */
			#if (1)
			/* 特殊処理(たまにしか実行しない処理)
				コアvoid shooting_game_core_work(void)関数のサイズが大きいと、
				CPUの命令キャッシュがフローする事により処理落ちするので、
				特殊処理(たまにしか実行しない処理)はコアの外に追い出します。
				その際「 static関数にしない」様に注意します。
				static関数にすると、GCCが勝手に __inline__ 関数に変換する為(-O3の場合)
				追い出した意味が無くなります。(インライン展開される)
			 */
		//	if (B00_NONE != pd->bo ssmode)
			if (pd->state_flag & (STATE_FLAG_10_IS_LOAD_SCRIPT/*|ST ATE_FLAG_11_IS_BOSS_DESTROY*/|STATE_FLAG_12_END_SCRIPT))
			{
				my_special();/* 注意：static関数にしない */
			}
			#endif
//
		#if 0/*ゲーム時間デバッグ用*/
		/* パネルのスコア欄にゲーム時間を 表示させる。っていうか書き換えちゃう。 */
		((PLAYER_DATA *)player->data)->score		= (game_v_time);
		#endif
//
		/* 動作 */
		bg_work_draw();
		//controller_work();
//		sprite_work222(SP_GROUP_ALL_SDL_WORK_TYPE);/*弾幕用*/
//		sprite_work000(SP_GROUP_ALL_SDL_WORK_TYPE);
		sprite_work000(SP_GROUP_ALL_SDL_CORE_TYPE);
		/* 描画 */
//		sprite_display222(SP_GROUP_ALL_SDL_DRAW_TYPE);/*弾幕用*/
//		sprite_display000((SP_GROUP_ALL_SDL_DRAW_TYPE & (~SP_GROUP_TEKI)));
//		sprite_display000(SP_GROUP_ALL_SDL_DRAW_TYPE);
	//	pause_sprite_display();/* SDL表示(現状SP_GROUP_PAUSE_SP_MENU_TEXTのみSDL描画) */
		draw_score_chache();
		// この辺は速度低下するのでコールバックにすべき
		if ((pd->state_flag & STATE_FLAG_06_IS_SCRIPT)) 	{	script_display();	}	/*STATE_FLAG_06_IS_SCRIPT==*/	//parsys_display();
		if (0!=draw_side_panel) 							{	score_display();	}	/*ST ATE_FLAG_09_IS_PANEL_WINDOW==*/	/*(pd->state_flag & ST ATE_FLAG_09_IS_PANEL_WINDOW)*/
//
		vbl_draw_screen();	/* 画面描画とキー入力(本当は v-blanc タイミングで) */
	}
//
}
