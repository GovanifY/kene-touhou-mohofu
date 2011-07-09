
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
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
	(スクショSDL新規サーフェイス作ってる上に、メモカアクセスするので遅いです)
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
	敵の追加
	-------------------------------------------------------
	ここに追記するとシューティングゲーム本体が遅くなるので追記しません。
	字句解析(parth)等は load_stage.c で予め済ませておきます。
---------------------------------------------------------*/
extern void game_command_00_kanji_hyouji(		GAME_COMMAND *l);/* どの敵でもない場合は、漢字表示 */
extern void game_command_01_game_all_clear( 	GAME_COMMAND *l);/* 全面クリアーの場合、この敵を追加 */
extern void game_command_02_bg_control( 		GAME_COMMAND *l);/* 背景コントロール(スクロール速度等を指定) */
extern void game_command_03_regist_boss(		GAME_COMMAND *l);/* ボス、スクリプト起動 */
extern void game_command_04_regist_chuu_boss(	GAME_COMMAND *l);/* [中型敵]妖怪、生成処理 */
extern void game_command_05_regist_zako(		GAME_COMMAND *l);/* ザコ、生成処理 */
static void add_game_command(GAME_COMMAND *l)
{
	/* 中間コード形式のコマンドから各関数に分岐する */
	/* game_commandの生成を番号で管理(load_stage.c の ctype_name[]に対応している) */
	void (*aaa[GC_CODE_MAX])(GAME_COMMAND *l) =
	{
		game_command_00_kanji_hyouji,		/* 漢字で文字表示 */	//	NULL,/* [番兵区切り] (で必要)*/
	/* その他 */
		game_command_01_game_all_clear, 	/* ゲーム 全ステージ クリアー */
		game_command_02_bg_control, 		/* */
	/* 敵 */
		game_command_03_regist_boss,		/* ボス共通 */
		game_command_04_regist_chuu_boss,	/* 中-ボスの予定 特殊敵[中型敵] */
		game_command_05_regist_zako,		/* ザコ */
	//
	};
	(*aaa[ (int)(l->user_i_code) ])(l); 	/* 中間コード形式のコマンドから各関数に分岐する */
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static u32 game_v_time;/* ゲーム時間 game flame time counter. */

#if (1==USE_HOLD_GAME_MODE)
static int v_time_hold_mode;/* 咲夜用に止めたり動かしたり出来るようにしとく */
#endif /* (1==USE_HOLD_GAME_MODE) */

static void init_stage_start_time(void)
{
//	stage_start_time = psp_get_uint32_ticks();
	game_v_time = 0;
}
/*static*/global void set_core_game_time_MAX(void)
{
	game_v_time = 65535;/* 適当に大きな値[flame](65535[flame]==約18[分]==18.xxx x 60 x 60 ) */
}

#if (1==USE_HOLD_GAME_MODE)
global void hold_game_mode_on(void)/* ゲーム時間の一時停止(咲夜、新規格イベント(構想中)等、使う) */
{
	v_time_hold_mode = 1;
}

global void hold_game_mode_off(void)/* ゲーム時間の動作開始 */
{
	v_time_hold_mode = 0;
}
#endif /* (1==USE_HOLD_GAME_MODE) */


/*---------------------------------------------------------
	シューティングゲーム本体の初期化
---------------------------------------------------------*/

extern void sprite_test_debug_init(void);/* r32:とりあえずバグあるのを無理やり回避(?) */
extern void set_rnd_seed(int set_seed);
extern void load_stage(void);
extern void player_init_first(void);
extern void player_init_stage(void);
extern void score_panel_init(void);
global void common_load_init(void)
{
	set_rnd_seed(cg.game_now_stage);	/* 乱数系列の初期化 */
//
	/* Load next stage */
	load_stage();
	// ロード中は処理落ちしているので、ロード後に時間を再作成する。
	init_stage_start_time();
//
	player_init_stage();/* ステージ開始時のみ若干の無敵状態にセット */
//
	kanji_window_clear();	/* 漢字ウィンドウの内容を消す。 */
	home_cursor();			/* カーソルをホームポジションへ移動 */
	//
	#if (1==USE_HOLD_GAME_MODE)
	hold_game_mode_off();/* ゲーム時間の動作開始 */
	#endif /* (1==USE_HOLD_GAME_MODE) */
	//
	#if 1/*Gu化完了したら要らなくなる*/
	{
		psp_clear_screen(); /* [PAUSE] 復帰時にSDL画面を消す。 */
	}
	#endif
	sprite_test_debug_init();/* r32:とりあえずバグあるのを無理やり回避(?) */
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
	player_init_first();/* 初回のみ設定 */
//	player_init_stage();/* ステージ開始時のみ若干の無敵状態にセット */
//
	kanji_window_clear();	/* 漢字ウィンドウの内容を消す。 */
	home_cursor();			/* カーソルをホームポジションへ移動 */
//
	cg.game_now_stage	= cg.game_continue_stage;
//
	main_call_func = common_load_init;
}


/*---------------------------------------------------------
	イベントシーンを次に進める。
---------------------------------------------------------*/

global void incliment_scene(void)
{
	if ((cg.state_flag & STATE_FLAG_05_IS_BOSS))
	{
		/*(ボス戦闘後イベント)*/
		main_call_func = stage_clear_result_screen_start;	/* ステージクリアー時のリザルト画面 */
	}
	else
	{
		/*(ボス戦闘前イベント)*/
		/*(r32)*/cg.state_flag |= (STATE_FLAG_05_IS_BOSS|STATE_FLAG_13_DRAW_BOSS_GAUGE);
		/* 雑魚追加読み込み処理を停止する。 */
		cg.state_flag			&= (~STATE_FLAG_14_ZAKO_TUIKA); 	/* off / 雑魚追加読み込み処理を停止する。 */
	}
}


/*---------------------------------------------------------
	特殊イベントの実行処理
	特殊処理(たまにしか実行しない処理)
	-------------------------------------------------------
	コア(メインループに)に追加すると、シューティングゲーム本体が遅くなるので、
	動作が遅すぎて弾幕シューティングゲームに、ならなくなってしまう。
	そこで動作速度的観点から、頻度の少ない特殊機能はここで実行する。
---------------------------------------------------------*/
extern GAME_COMMAND *stage_command_table;
static void game_core_zako_tuika(void)
{
	/*
		This routine, search back to begin.
		このルーチンは逆順に検索します。
	*/
	{
		GAME_COMMAND *l;
		l = stage_command_table;
		while (NULL != l)	/* コマンドリストの終わり(NULL)まで調べる */	/* [head ==NULL] then end. */
		{
			if (0 < l->v_time ) 	/* コマンド処理済み？ */
			{
				if (game_v_time >= (l->v_time)) 	/* (現在時間 >= 設定時間) なら、敵をセット */
				{
					add_game_command(l);	/* コマンド生成する(コマンドが雑魚敵の場合、雑魚敵を生成する) */
					l->v_time = (-1);		/* コマンド処理済みをマーク */
				}
			}
			l = l->next;	/* 次を調べる */	/* choice alter. */
		}
	}
	/* 道中の場合勝手に喰み出しチェックを行い弾を消す(暫定的) */
	#if 0
//	if (0!=(cg.state_flag & STATE_FLAG_05_IS_NOT_BOSS))
	if (0==(cg.state_flag & STATE_FLAG_05_IS_BOSS))/*(r32)*/
	#endif
	{
		bullet_angle_all_gamen_gai_nara_kesu();/* 角度弾の喰み出しチェックを行う(毎フレーム行う必要はない) */
	}
}
//
extern void script_system_SDL_draw(void);
extern void script_move_main(void);
extern void script_ivent_load(void);
/* 注意：(動作速度低下するので)static関数にしない */global void my_special(void)
{
	#if 1
	/* 雑魚を検索し登場させる処理 */
	{
	//	if (cg.state_flag & (ST ATE_FLAG_14_GAME_LOOP_QUIT))
	//	{
	//		;	/* GAMEOUT中 */
	//	}
	//	else
		if (cg.state_flag & (STATE_FLAG_14_ZAKO_TUIKA))
		{
			/* 生きてる */
			game_core_zako_tuika();
			/* [旧]特殊処理のあった位置 */
		}
	}
	#endif

	#if 1
	/*
		★「(喰らいボム受付期間中に)ボスと相打ちするとハングアップ」バグ(～r29)対策
	*/
	if (0 < /*bomb_wait*/cg.bomber_time)		/* ボムウェイト処理 */
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
	if (0==(cg.state_flag & (/*STATE_FLAG_06_IS_SCRIPT|*/STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL)))
	{
		return;/* ボム発動中は待機 */
	}
	#endif
	if (cg.state_flag & (STATE_FLAG_10_IS_LOAD_SCRIPT))
	{
		cg.state_flag &= (~(STATE_FLAG_10_IS_LOAD_SCRIPT));/*off*/
		script_ivent_load();/*0 1*/
	}

//	if (cg.state_flag & (ST ATE_FLAG_11_IS_BOSS_DESTROY))
//	{
//		cg.state_flag &= (~(ST ATE_FLAG_11_IS_BOSS_DESTROY));/*off*/
//		boss_destroy_aaa();
//	}
	/* スクリプトが終わった？ */
	if (cg.state_flag & (STATE_FLAG_12_END_SCRIPT))
	{
		cg.state_flag &= (~(STATE_FLAG_12_END_SCRIPT));/*off*/
		incliment_scene();
	}
	/* スクリプト動作が必要？ */
	/*(r32)*/if (cg.state_flag & STATE_FLAG_06_IS_SCRIPT)
	{
		script_system_SDL_draw();	/* スクリプト SDL 描画(遅い) */
		script_move_main(); 		/* スクリプト動作(移動) */
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

extern void score_display(void);
extern void bg2_move_main(void);
extern void draw_SDL_score_chache(void);

global void shooting_game_core_work(void)
{
	{
my_game_core_loop:
		/* [A] ゲーム時間を経過させる。 */
		#if (1==USE_HOLD_GAME_MODE)
		if (0==v_time_hold_mode)/* 咲夜用に止めたり動かしたり出来るようにしとく */
		#endif /* (1==USE_HOLD_GAME_MODE) */
		{
			game_v_time++;	/* ゲーム時間はフレーム単位(game time resolutions about 1/60 seconds.) */
		}
		#if 0/* ゲーム時間デバッグ用 */
		/* パネルのスコア欄にゲーム時間を 表示させる。っていうか書き換えちゃう。 */
		pd_score		= (game_v_time);
		#endif
	//
		/* [B] 特殊処理イベントが発生している場合、特殊処理を行う。 */
		#if (1)
		/*
			★「ボスと相打ちするとハングアップ」バグ(～r26)対策
			[新]特殊処理の位置:
			ボスを倒した場合に自分が死んでいて、喰らいボム判定中の場合、
			[旧]特殊処理の位置では、喰らいボム判定の為ST ATE_FLAG_14_GAME_LOOP_QUITなので
			ST ATE_FLAG_11_IS_BOSS_DESTROY(特殊処理で判定)が判定できないバグ(～r26)がある。
			そこで位置を動かした。(ST ATE_FLAG_11_IS_BOSS_DESTROYの判定は少なくともこちら側でする必要がある)
		 */
		/* 特殊処理(たまにしか実行しない処理)
			コアvoid shooting_game_core_work(void)関数のサイズが大きいと、
			CPUの命令キャッシュがフローする事により処理落ちするので、
			特殊処理(たまにしか実行しない処理)はコアの外に追い出します。
			その際「 static関数にしない」様に注意します。
			static関数にすると、GCCが勝手に __inline__ 関数に変換する為(-O3の場合)
			追い出した意味が無くなります。(インライン展開される)
		 */
		/*(r32)*/if (cg.state_flag & (
			STATE_FLAG_10_IS_LOAD_SCRIPT |	//
			STATE_FLAG_12_END_SCRIPT |		//
			STATE_FLAG_06_IS_SCRIPT |		//
			STATE_FLAG_14_ZAKO_TUIKA))		// 道中はザコ追加必要なので特殊処理
			/*|ST ATE_FLAG_11_IS_BOSS_DESTROY*/
		{
			my_special();/* 注意：(動作速度低下するので)static関数にしない */
		}
		#endif
		/*
			[C] 動作(移動)させる。
			動作(移動)と描画は違う概念なのできちんと分離する事。
			もし、処理が遅くなって、描画をフレームスキップさせる場合でも、
			動作(移動)はフレームスキップさせない。
		*/
		bg2_move_main();	/* 背景の移動処理 */
		sprite_move_all();	/* スプライトオブジェクトの移動処理 */
		/* [D] 描画 */
		// この辺は速度低下するのでコールバックにすべき
		/*(r32)*/if (0!=(cg.side_panel_draw_flag))
		{
			score_display();		/* スコアパネル SDL 描画(遅い) */
		}	/*ST ATE_FLAG_09_IS_PANEL_WINDOW==*/	/*(cg.state_flag & ST ATE_FLAG_09_IS_PANEL_WINDOW)*/
//
	// ハングアップ対策：常にポーズ可能に変更する。(2010-02-11)
	// メニューのキー入力が仕様変更になったので、ここもそれに併せて仕様変更。(2010-06-01)
	//	if (0==my_pad)
		if (0==(psp_pad.pad_data_alter & PSP_KEY_PAUSE))/* さっきポーズが押されてなくて */
		{
			if (psp_pad.pad_data & PSP_KEY_PAUSE)/* 今ポーズが押されたら */
			{
			//	if (0==(cg.state_flag & STATE_FLAG_06_IS_SCRIPT))/*たまにうまくいかない事がある*/
				{
					main_call_func			= pause_menu_start;
					pause_out_call_func 	= shooting_game_core_work;/* ポーズ復帰後の戻り先を決める */
				}
			}
		}
		/* Gu描画 */
		{
			vbl_draw_screen();	/* 画面描画とキー入力(本当は v-blanc タイミングで) */
		}
		/* ゲームコアから外に出ると、(CPUの命令キャッシュが壊れるので(?))、極端に速度低下する。
			必要ない場合は外に出ない為の処置 */
		if (shooting_game_core_work == main_call_func)
		{
			goto my_game_core_loop;
		}
		/* ここに来たら、外に出るので遅くなるという事 */
	}
}
