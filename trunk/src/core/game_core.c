
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ゲームコア
	シューティングゲーム中は、「ゲームコア」で行います。
	ここから外に出ると、CPUの命令キャッシュが破壊されるので、
	速度が低下します。
	-------------------------------------------------------
	pause中等は、外に出ます。
	-------------------------------------------------------
以下残件:(r31時点)
	遅さは単なる主観です。根拠はないです。
	-------------------------------------------------------
	遅さ:★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	スクショはSDLルーチンに行くので論外に速度低下します。
	-------------------------------------------------------
	遅さ:★★★★★★★☆☆☆
	シナリオは、SDLルーチンに行くので40/60[fps]程度まで速度低下します。
	珠にキャッシュに乗って偶然速い事もありますが、偶然です。
	SDL使わない様に書き直さないといけません。
	-------------------------------------------------------
	遅さ:★★☆☆☆☆☆☆☆☆
	サイドのパネルもSDLルーチンに行くので速度低下要因です。
	SDL使わない様に書き直せばその分速度向上します。
	SDL使うにしても、キャッシュ式にすれば速度を稼げます。
	ちなみに「Noiz2sa for psp version 0.7」はキャッシュ式で速度を稼いでいる例です。
---------------------------------------------------------*/

#include "kanji_system.h"

/*---------------------------------------------------------
	ザコで宣言が必要なもの(グローバル)
---------------------------------------------------------*/

	/* その他(特殊機能) */
extern void add_clouds( 				STAGE_DATA *l);/* どの敵でもない場合は、演出用画像 */
extern void add_enemy_all_clear(		STAGE_DATA *l);/* 全面クリアーの場合、この敵を追加 */
extern void bg2_control(				STAGE_DATA *l);

	/* ボス */
extern void add_boss_common(			STAGE_DATA *l);/* 1面はアリス */

	/* [中型敵]妖怪 */
extern void add_chuu_boss(				STAGE_DATA *l);

	/* 魔方陣 */
extern void add_enemy_mahoujin( 		STAGE_DATA *l);

	/* 竜巻 陰陽玉 */
extern void add_zako_inyou1(			STAGE_DATA *l);
extern void add_zako_tatsumaki1(		STAGE_DATA *l);
	/* 妖怪 */
extern void add_zako_kakomi1(			STAGE_DATA *l);
extern void add_zako_aka_kedama1(		STAGE_DATA *l); 	/* 橙 */
	/* 毛玉 */
extern void add_zako_inseki1(			STAGE_DATA *l); 	/* その他ザコ */
extern void add_zako_yukari2(			STAGE_DATA *l); 	/* その他ザコ */
extern void add_zako_midori_kedama1(	STAGE_DATA *l);
extern void add_zako_midori_kedama2(	STAGE_DATA *l);
extern void add_zako_kedama1(			STAGE_DATA *l);
extern void add_zako_kedama2(			STAGE_DATA *l);
	/* [C妖精]その他ザコ */
extern void add_zako_obake1(			STAGE_DATA *l);
extern void add_zako_obake2(			STAGE_DATA *l);
extern void add_zako_karasu1(			STAGE_DATA *l);
	/* [B妖精]中妖精 */
extern void add_zako_meido1(			STAGE_DATA *l);
extern void add_zako_meido2(			STAGE_DATA *l);
extern void add_zako_meido3(			STAGE_DATA *l);
extern void add_zako_meido4(			STAGE_DATA *l);
	/* [A妖精]小妖精 */
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
		add_boss_common,			/* 共通 */
	/* 特殊敵[中型敵] */
		add_chuu_boss,				/* 中ボスの予定 */
	/* 魔方陣 */
		add_enemy_mahoujin, 		/*	""	*/
		add_enemy_mahoujin, 		/*	""	*/
	/* 竜巻 陰陽玉 */
		add_zako_inyou1,			/*	-"陰陽玉1"	*/
		add_zako_tatsumaki1,		/*	C"竜巻1"	*/		/*enemy_error*/
	/* 妖怪 */
		add_zako_kakomi1,			/*	-"囲妖怪1"	*/
		add_zako_aka_kedama1,		/*	a"赤毛玉1"	*/		/* 橙 */
	/* 毛玉 */
		add_zako_inseki1,			/*	C"隕石1"	*/		/*enemy_error*/ 	/* その他ザコ */
		add_zako_yukari2,			/*	C"紫編隊2"	*/		/*enemy_error*/ 	/* その他ザコ */
		add_zako_midori_kedama1,	/*	-"緑毛玉1"	*/
		add_zako_midori_kedama2,	/*	C"緑毛玉2"	*/		/*enemy_error*/
		add_zako_kedama1,			/*	-"毛玉1"	*/
		add_zako_kedama2,			/*	-"毛玉2"	*/
	/* [C妖精]その他ザコ */
		add_zako_obake1,			/*	-"おばけ1"	*/
		add_zako_obake2,			/*	-"おばけ2"	C"虹毛玉1"	*/		/*enemy_error*/
		add_zako_karasu1,			/*	-"烏1"	*/
	/* [B妖精]中妖精 */
		add_zako_meido1,			/*	-"メイド1"	*/
		add_zako_meido2,			/*	-"メイド2"	*/
		add_zako_meido3,			/*	C"メイド3"	*/		/*enemy_error*/
		add_zako_meido4,			/*	-"メイド4"	*/
	/* [A妖精]小妖精 */
		add_zako_ao_yousei1,		/*	="青妖精1"	*/		// [***090207	追加
		add_zako_ao_yousei2,		/*	="青妖精2"	*/		// [***090124	追加
		add_zako_ao_yousei3,		/*	="青妖精3"	*/		//	追加
		add_zako_ao_yousei4,		/*	="青妖精4"	*/		//	追加
//
		add_enemy_kanji_string, 	/* ETYPE_01_SJIS_TEXT */
		add_enemy_load_bg,			/* ETYPE_02_LOAD_BG */
	//	add_enemy_load_picture, 	/* ETYPE_03_PICTURE */		/* この方式は処理落ち解消しにくい(Gu化も難しい)ので都合により廃止 */
	};
	(*aaa[ (int)(l->user_i_code) ])(l); 	/* 中間コード形式のコマンドから各関数に分岐する */
}

/*---------------------------------------------------------

---------------------------------------------------------*/

extern STAGE_DATA *stage_data_table;

extern int select_player;
extern int practice_mode;

global int difficulty = RANK_EASY;		/*	RANK_NORMAL*/

//static u32 stage_start_time;
//static u32 game_start_time;

/*---------------------------------------------------------

---------------------------------------------------------*/
static u32 game_v_time;
static int v_time_hold_mode;
static void init_stage_start_time(void)
{
//	stage_start_time = psp_get_uint32_ticks();
	game_v_time = 0;
}

global void hold_game_time(void)/* ゲーム時間の一時停止(咲夜、新規格イベント(構想中)等、使う) */
{
	v_time_hold_mode = 1;
}

global void continue_game_time(void)/* ゲーム時間の動作開始 */
{
	v_time_hold_mode = 0;
}

/*---------------------------------------------------------
	シューティングゲーム本体の初期化
---------------------------------------------------------*/
extern int continue_stage;

extern void set_rnd_seed(int set_seed);
extern /*int*/void load_stage(void/*int level*/);
extern void player_init(void);
extern void player_few_muteki(void);
extern void score_panel_init(void);
global void common_load_init(void)
{
	set_rnd_seed(player_now_stage); 	/* 乱数系列の初期化 */
//
	/* Load next stage */
	load_stage();//if (0==load_stage(/*level*/))	{	error(ERR_WARN, "no entrys for level %d",level);}
	// ロード中は処理落ちしているので、ロード後に時間を再作成する。
	init_stage_start_time();
//
	player_few_muteki();/* ステージ開始時のみ若干の無敵状態にセット */
//
	kanji_window_clear(); 	/* 漢字ウィンドウの内容を消す。 */
	home_cursor();			/* カーソルをホームポジションへ移動 */
	continue_game_time();/* ゲーム時間の動作開始 */
			#if 1/*Gu化完了したら要らなくなる*/
			{
				psp_clear_screen(); /* [PAUSE] 復帰時にSDL画面を消す。 */
			}
			#endif
	main_call_func = shooting_game_core_work;
}


/*---------------------------------------------------------
	ゲームコア初回(ゲーム開始時)限定の初期化
---------------------------------------------------------*/

//global void shooting_game_core_1st_init(void)
global void stage_first_init(void)
{
	score_panel_init();
	//sprite_controller_remove_all();
//
	player_init();/* 初回のみ設定 */
//	player_few_muteki();/* ステージ開始時のみ若干の無敵状態にセット */
//
	kanji_window_clear(); 	/* 漢字ウィンドウの内容を消す。 */
	home_cursor();			/* カーソルをホームポジションへ移動 */
//
	player_now_stage/*data->now_stage*/ /*level*/	= continue_stage/*+1-1*/ /*1*/;
//
	main_call_func = common_load_init;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

global void incliment_scene(void)
{
	{
		/*ボス戦闘後イベント*/
	//	if (B09_STAGE_LOAD==pd_bo ssmode) // 9:stage読み込み
		if (/*STATE_FLAG_05_IS_BOSS == */(pd_state_flag & STATE_FLAG_05_IS_BOSS))
		{
			main_call_func = stage_clear_result_screen_start;	/* ステージクリアー時のリザルト画面 */
		}
		/*ボス戦闘前イベント*/
		else
	//	if (B08_START == pd_bo ssmode) // 8:ボス曲を鳴らし、1ボスとの戦闘へ。
		{
			pd_state_flag |= (STATE_FLAG_05_IS_BOSS|STATE_FLAG_13_DRAW_BOSS_GAUGE);
		}
	}
}
//
extern void script_ivent_load(void);
/* 注意：static関数にしない */global void my_special(void)
{
	#if 1
	/*
		★「(喰らいボム受付期間中に)ボスと相打ちするとハングアップ」バグ(～r29)対策
	*/
	if (0 < /*bomb_wait*/pd_bomber_time)		/* ボムウェイト処理 */
	{
		return;/* ボム発動中は待機 */
	}
	#endif
//
	#if 0
	/*
		★「(喰らいボム受付期間中に)ボスと相打ちするとハングアップ」バグ(～r29)対策
	*/
	/* キー入力無効中(==復活中) は、敵あたり判定はない */
	if (0==(pd_state_flag & (/*STATE_FLAG_06_IS_SCRIPT|*/STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL)))
	{
		return;/* ボム発動中は待機 */
	}
	#endif
//	if (pd_bo ssmode==B05_BEFORE_LOAD)		// [***090313	追加
//	if (pd_bo ssmode==B07_AFTER_LOAD)		// [***090313	追加
	if (pd_state_flag & (STATE_FLAG_10_IS_LOAD_SCRIPT)) 	// [***090313	追加
	{
		pd_state_flag &= (~(STATE_FLAG_10_IS_LOAD_SCRIPT));/*off*/
		script_ivent_load(/*0 1*/);
	}

//	if (pd_state_flag & (ST ATE_FLAG_11_IS_BOSS_DESTROY))
//	{
//		pd_state_flag &= (~(ST ATE_FLAG_11_IS_BOSS_DESTROY));/*off*/
//		boss_destroy_aaa();
//	}
	/* スクリプトが終わった？ */
	if (pd_state_flag & (STATE_FLAG_12_END_SCRIPT))
	{
		pd_state_flag &= (~(STATE_FLAG_12_END_SCRIPT));/*off*/	/*	pd_bo ssmode=B00_NONE;*/
	//	pd_state_flag &= (~(STATE_FLAG_12_END_SCRIPT));/*off*/	/*	pd_bo ssmode=B00_NONE;*/	/*B01_BA TTLE*/
		incliment_scene();
	}
}


/*---------------------------------------------------------
	シューティングゲーム本体のメインルーチン
	-------------------------------------------------------
	ここに追記すればするほど、シューティングゲーム本体が
	遅くなるので、注意して追記してくれ。
	めったに実行しない物は関数化して外に追い出そう。
	-------------------------------------------------------
	ここは、現在一つしかないが、
	単純コールバック方式(いくつあっても速度低下しない方式にした)にしたので、
	複数にすることが出来るようになった。
	そこで、
		道中用
		会話用
		ボス用
	の少なくとも３つに分ける予定。
---------------------------------------------------------*/

extern void vbl_draw_screen(void);/*support.c*/

extern void script_move_main(void);
extern void score_display(void);
extern void bg2_move_main(void);
extern void draw_SDL_score_chache(void);

global void shooting_game_core_work(void)
{
	{
my_game_core_loop:
		if (0==v_time_hold_mode)
		{
			/* game_v_time=Zeit seit Spielbeginn in 1/10 sec. */
			game_v_time++;//=(psp_get_uint32_ticks()-stage_start_time);
		}
	//
		if (pd_state_flag & STATE_FLAG_14_GAME_LOOP_QUIT)
		{
			;	/* GAMEOUT中 */
		}
		else
		{
			/* 生きてる */
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
							l->v_time = (-1);	/* 処理済みをマーク */	/* enemy set done flag */	//l->done = 1;
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
		//	if (B00_NONE != pd_bo ssmode)
			if (pd_state_flag & (STATE_FLAG_10_IS_LOAD_SCRIPT|STATE_FLAG_12_END_SCRIPT))/*|ST ATE_FLAG_11_IS_BOSS_DESTROY*/
			{
				my_special();/* 注意：static関数にしない */
			}
			#endif
//
		#if 0/*ゲーム時間デバッグ用*/
		/* パネルのスコア欄にゲーム時間を 表示させる。っていうか書き換えちゃう。 */
		pd_score		= (game_v_time);
		#endif
//
		/*
			動作させる。
			動作と描画は違う概念なのできちんと分離する事。
			もし、処理が遅くなって、描画をフレームスキップさせる場合でも、
			動作はフレームスキップさせない。
		*/
		bg2_move_main();
		//controller_work();
//		sprite_work222(SP_GROUP_ALL_SDL_WORK_TYPE);/*弾幕用*/
//		sprite_work000(SP_GROUP_ALL_SDL_WORK_TYPE);
		sprite_move_main_SDL_222();/*gu汎用*/	/*SP_GROUP_ALL_SDL_CORE_TYPE*/
		sprite_move_main_Gu_444();/*弾幕専用*/	/*SP_GROUP_ALL_SDL_CORE_TYPE*/
		/* 描画 */
//		sprite_display222(SP_GROUP_ALL_SDL_DRAW_TYPE);/*弾幕用*/
//		sprite_display000((SP_GROUP_ALL_SDL_DRAW_TYPE & (~SP_GROUP_TEKI)));
//		sprite_display000(SP_GROUP_ALL_SDL_DRAW_TYPE);
	//	pause_sprite_display();/* SDL表示(現状SP_GROUP_PAUSE_S P_ME NU_TEXTのみSDL描画) */
	//	draw_SDL_score_chache();/* SDL描画 */
		// この辺は速度低下するのでコールバックにすべき
		if ((pd_state_flag & STATE_FLAG_06_IS_SCRIPT))	{	script_move_main();	}	/*STATE_FLAG_06_IS_SCRIPT==*/
		if (0!=draw_side_panel) 						{	score_display();	}	/*ST ATE_FLAG_09_IS_PANEL_WINDOW==*/	/*(pd_state_flag & ST ATE_FLAG_09_IS_PANEL_WINDOW)*/
//

	// ハングアップ対策：常にポーズ可能に変更する。(2010-02-11)
	// メニューのキー入力が仕様変更になったので、ここもそれに併せて仕様変更。(2010-06-01)
	//	if (0==my_pad)
		if (0==(my_pad_alter & PSP_KEY_PAUSE))/* さっきポーズが押されてなくて */
		{
			if (my_pad & PSP_KEY_PAUSE)/* 今ポーズが押されたら */
			{
			//	if (0==(pd_state_flag & STATE_FLAG_06_IS_SCRIPT))/*たまにうまくいかない事がある*/
				{
					main_call_func			= pause_menu_start;
					pause_out_call_func 	= shooting_game_core_work;/* ポーズ復帰後の戻り先を決める */
				}
			}
		}
//
		vbl_draw_screen();	/* 画面描画とキー入力(本当は v-blanc タイミングで) */
		if (shooting_game_core_work == main_call_func)
		{
			goto my_game_core_loop;
		}
	}
}
