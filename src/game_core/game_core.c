
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
	ゲームコマンドの新規追加。
	-------------------------------------------------------
	ここに追記するとシューティングゲーム本体が遅くなるので追記しません。
	字句解析(parth)等は load_stage.c で予め済ませておきます。
---------------------------------------------------------*/
extern void game_command_00_kanji_hyouji(			GAME_COMMAND *l);/* どの敵でもない場合は、漢字表示 */
extern void game_command_01_game_all_clear( 		GAME_COMMAND *l);/* 全面クリアーの場合、このゲームコマンドを追加 */
extern void game_command_02_bg_control( 			GAME_COMMAND *l);/* 背景コントロール(スクロール速度等を指定) */
extern void game_command_03_check_secret_bonus( 	GAME_COMMAND *l);/* 隠しボーナスチェック終了。結果表示。 */
extern void game_command_04_begin_secret_bonus( 	GAME_COMMAND *l);/* 隠しボーナスチェック開始。 */
extern void game_command_05_kaiwa_start_boss(		GAME_COMMAND *l);/* ボス、シナリオ会話起動 */
extern void game_command_06_regist_chuu_boss(		GAME_COMMAND *l);/* [中型敵]妖怪、生成処理 */
extern void game_command_07_regist_zako(			GAME_COMMAND *l);/* ザコ、生成処理 */

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


extern void set_rnd_seed(int set_seed);
extern void load_stage(void);

extern void player_init_stage(void);

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
	#if (1)/*(漢字関連の初期化)*/
	set_kanji_origin_xy((10+6), (10+192));/*(表示原点の設定)*/
	set_kanji_origin_kankaku(18);/*(字間を標準にする)*/
//	set_kanji_hide_line(ML_LINE_02);/*(2行目以下を非表示にする。)*/
	set_kanji_hide_line(ML_LINE_04);/*(4行目以下を非表示にする。)*/
	kanji_window_all_clear();				/* 漢字画面を全行消す。漢字カーソルをホームポジションへ移動。 */
	#endif
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
	cb.main_call_func = shooting_game_core_work;
}


/*---------------------------------------------------------
	ゲームコア初回(ゲーム開始時)限定の初期化
---------------------------------------------------------*/

//global void shooting_game_core_1st_init(void)
extern void score_panel_init(void);
extern void jiki_class_initialize(void);
global void stage_first_init(void)
{
	score_panel_init();
//	sprite_controller_remove_all();
//
	jiki_class_initialize();/* (C++にする訳ではないが)自機クラス(jiki::jiki();)を作成し初期化。初回のみ設定 */
//
	cg.game_now_stage	= cg.game_continue_stage;
	cb.main_call_func	= common_load_init;
}


/*---------------------------------------------------------
	(シナリオ会話処理が終わったので)
	イベントシーンを次に進める。
---------------------------------------------------------*/

global void incliment_scene(void)
{
	set_kanji_hide_line(ML_LINE_02);/*(2行目以下を非表示にする。)*/
	if ((cg.state_flag & STATE_FLAG_05_IS_BOSS))
	{	/*(「ボス戦闘後の会話が終了した状態」の場合)*/
		cb.main_call_func = stage_clear_result_screen_start;	/* ステージクリアー時のリザルト画面 */
	}
	else
	{
	//	cg.state_flag |= STATE_FLAG_05_IS_BOSS; 	/* ボス戦闘前の会話終了を設定 */
		/*(「ボス戦闘前の会話が終了した状態」の場合)*/
		/*(r32)*/cg.state_flag |= (STATE_FLAG_05_IS_BOSS|STATE_FLAG_13_DRAW_BOSS_GAUGE);
		/* 道中コマンド追加読み込み処理を停止する。 */
		cg.state_flag			&= (~STATE_FLAG_14_DOUCHU_TUIKA);	/* off */
	}
}


/*---------------------------------------------------------
	ゲームコマンド(主に雑魚)追加処理
	-------------------------------------------------------
---------------------------------------------------------*/
extern GAME_COMMAND *stage_command_table;
static void game_core_teki_tuika(void)
{
	/*
		This routine, search back to begin.
		このルーチンは逆順に検索します。
	*/
	{
		GAME_COMMAND *aaa;
		aaa = stage_command_table;
		/* コマンドリストの終わり(NULL)まで調べる */	/* [head ==NULL] then end. */
		while (NULL != aaa)
		{
			/* コマンド処理済み？ */
			if (0 < (aaa->v_time) )
			{	/* 未処理のコマンド */
				/* (現在時間 >= 設定時間) なら、コマンドを生成 */
				if (game_v_time >= (aaa->v_time))
				{	/* コマンド処理済みをマーク */
					aaa->v_time = (-1);
					/* コマンド生成する(コマンドが雑魚敵の場合、雑魚敵を生成する) */
				//	regist_game_command(aaa);
				//	static void regist_game_command(GAME_COMMAND *aaa)
					{	/* 中間コード形式のコマンドから各関数に分岐する */
						/* game_commandの生成を番号で管理(load_stage.c の ctype_name[]に対応している) */
						void (*bbb[/*(6)*/GC_CODE_MAX/*(8)*/])(GAME_COMMAND *aaa) =
						{
							game_command_00_kanji_hyouji,			/* 漢字で文字表示。 */		//	NULL,/* [番兵区切り] (で必要)*/
							game_command_01_game_all_clear, 		/* ゲーム 全ステージ クリアーを指示。 */
							game_command_02_bg_control, 			/* 背景の制御コマンド。 */
							game_command_03_check_secret_bonus, 	/* 隠しボーナスチェック終了。結果表示。 */
						//
							game_command_04_begin_secret_bonus, 	/* 隠しボーナスチェック開始。 */
							game_command_05_kaiwa_start_boss,		/* ボスを追加する(r35)。かつ ボスのシナリオ会話を起動する。(シナリオ会話内でボス追加) */
							game_command_06_regist_chuu_boss,		/* 中ボスを追加する。 (特殊敵[中型敵]) */
							game_command_07_regist_zako,			/* ザコを追加する。 */
						//	game_command_01_game_all_clear, 		/* (.align)ダミー。 */
						//	game_command_01_game_all_clear, 		/* (.align)ダミー。 */
						};
						(*bbb[ (int)(aaa->user_i_code) ])(aaa); 	/* 中間コード形式のコマンドから各関数に分岐する */
					}
				}
			}
			aaa = aaa->next;	/* 次を調べる */	/* choice alter. */
		}
	}
	#if (1)/*(とりあえず)*/
	if (SPELL_00 == card.card_number)		/* カード生成しない場合、道中用コールバックを呼ぶ。 */
	{
		danmaku_system_callback();/* 弾幕コールバックシステム(を単純に呼ぶ) */
	}
	#endif
}
	#if (0)/*(r35巧くいってない。何故かボス中に有効になる)*/
	/* 道中の場合勝手に喰み出しチェックを行い弾を消す(暫定的) */
	#if 0
//	if (0!=(cg.state_flag & STATE_FLAG_05_IS_NOT_BOSS))
	if (0==(cg.state_flag & STATE_FLAG_05_IS_BOSS))/*(r32)*/
	#endif
	{
		/*
			弾消しは「弾幕コールバックシステム」に標準搭載されているものを使う。
			このシステムを使用して弾消しを行う場合は、システムの初期化が必要なので、
			その為の初期化は各面開始前のローディング時に行う。
		*/
		#if (1)
		/* 弾消しは、毎フレーム行う必要がないので、道中が遅いなら、danmaku_system_callback();を数フレームに一度呼ぶように変更すれば良い。 */
		static int aaaa=0;
		aaaa++;
		aaaa &= (0x07);
		if (0==aaaa)
		#endif
		{
			danmaku_system_callback();/* 弾幕コールバックシステム(を単純に呼ぶ) */
		}
	//	danmaku_action_00_gamen_gai_nara_tama_wo_kesu();/* 角度弾の喰み出しチェックを行う(毎フレーム行う必要はない) */
	}
	#endif

/*---------------------------------------------------------
	特殊イベントの実行処理
	特殊処理(たまにしか実行しない処理)
	-------------------------------------------------------
	-------------------------------------------------------
	コア(メインループに)に追加すると、シューティングゲーム本体が遅くなるので、
	動作が遅すぎてゲームに、ならなくなってしまう。
	そこで動作速度的観点から、頻度の少ない特殊機能はここで実行する。
---------------------------------------------------------*/
extern void kaiwa_system_SDL_draw(void);
extern void kaiwa_system_execute_move_only_main(void);
extern void kaiwa_load_ivent(void);
/* 注意：(動作速度低下するので)static関数にしない */global void my_special(void)
{
	/* 道中コマンド追加読み込み処理 */
	if (cg.state_flag & (STATE_FLAG_14_DOUCHU_TUIKA))
	{
		game_core_teki_tuika();
	}
	/* ★「(喰らいボム受付期間中に)ボスと相打ちするとハングアップ」バグ(～r29)対策 */
	if (0 < /*bomb_wait*/cg.bomber_time)		/* ボムウェイト処理 */
	{
		return;/* ボム発動中は待機 */
	}
	/* シナリオ会話をロードして開始処理 */
	if (cg.state_flag & (STATE_FLAG_10_IS_LOAD_KAIWA_TXT))
	{
		cg.state_flag &= (~(STATE_FLAG_10_IS_LOAD_KAIWA_TXT));/*off*/
		kaiwa_load_ivent();/*(シナリオ会話をロードして開始する)*/
	}
	/*(会話モード処理が終わった？)*/
	if (cg.state_flag & (STATE_FLAG_12_END_KAIWA_MODE))
	{
		cg.state_flag &= (~(STATE_FLAG_12_END_KAIWA_MODE));/*off*/
		incliment_scene();/*(次の状態に進める)*/
	}
	/* 会話モード動作が必要？ */
	if (cg.state_flag & STATE_FLAG_06_IS_KAIWA_MODE)
	{
		kaiwa_system_SDL_draw();					/* シナリオ会話 SDL 描画(遅い) */
		kaiwa_system_execute_move_only_main();		/* シナリオ会話 動作(移動) */
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
		/* [B] 特殊処理イベントが発生している場合、特殊処理を行う。 */
		#if (1)
		/* 特殊処理(たまにしか実行しない処理)
			コアvoid shooting_game_core_work(void)関数のサイズが大きいと、
			CPUの命令キャッシュがフローする事により処理落ちするので、
			特殊処理(たまにしか実行しない処理)はコアの外に追い出します。
			その際「 static関数にしない」様に注意します。
			static関数にすると、GCCが勝手に __inline__ 関数に変換する為(-O3の場合)
			追い出した意味が無くなります。(インライン展開される)
		 */
		if (cg.state_flag & (
			STATE_FLAG_10_IS_LOAD_KAIWA_TXT |	//
			STATE_FLAG_12_END_KAIWA_MODE |		//
			STATE_FLAG_06_IS_KAIWA_MODE |		//
			STATE_FLAG_14_DOUCHU_TUIKA))		// 道中はザコ追加必要なので特殊処理
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
		if (0!=(cg.side_panel_draw_flag))
		{
			score_display();		/* スコアパネル SDL 描画(遅い) */
		}
		/* Gu描画 */
		{
			vbl_draw_screen();	/* 画面描画とキー入力(本当は v-blanc タイミングで) */
		}
		/* シューティングゲーム中は、常にポーズ可能。 */
		if (0==(psp_pad.pad_data_alter & PSP_KEY_PAUSE))/* さっきポーズが押されてなくて */
		{
			if (psp_pad.pad_data & PSP_KEY_PAUSE)/* 今ポーズが押されたら */
			{
				cb.main_call_func			= pause_menu_start;
				cb.pause_out_call_func		= shooting_game_core_work;/* ポーズ復帰後の戻り先を決める */
			}
		}
		/* ゲームコアから外に出ると、(CPUの命令キャッシュが壊れるので(?))、極端に速度低下する。
			必要ない場合は外に出ない為の処置 */
		if (shooting_game_core_work == cb.main_call_func)
		{
			goto my_game_core_loop;
		}
		/* ここに来たら、外に出るので遅くなるという事 */
	}
}
