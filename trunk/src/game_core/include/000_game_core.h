
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _GAME_CORE_
#define _GAME_CORE_
/*---------------------------------------------------------
	ゲームランク
---------------------------------------------------------*/

enum /*_game_rank_*/
{
	RANK_EASY = 0,
	RANK_NORMAL,	/*=1*/
	RANK_HARD,		/*=2*/
	RANK_LUNATIC,	/*=3*/
	RANK_MAX		/* ランクの最大数==(最高ランク+1) */
};

/*---------------------------------------------------------
	コールバック
---------------------------------------------------------*/
#if 1/*単純コールバック方式*/
extern MAIN_CALL_FUNC(common_load_init);
extern MAIN_CALL_FUNC(stage_clear_result_screen_start);/* for game_core.c ??stage_clear.c */
//
extern MAIN_CALL_FUNC(stage_first_init);/* for game_core.c select_player.c */
extern MAIN_CALL_FUNC(shooting_game_core_work);/* for pause.c ask_continue.c */
extern MAIN_CALL_FUNC(gameover_start);/* for ask_continue.c */
extern MAIN_CALL_FUNC(name_entry_start);/* for game_over.c */
//
extern MAIN_CALL_FUNC(stage_select_menu_start);
extern MAIN_CALL_FUNC(option_menu_start);
extern MAIN_CALL_FUNC(story_mode_start);
extern MAIN_CALL_FUNC(yume_no_kiroku_start);
extern MAIN_CALL_FUNC(key_config_start);
extern MAIN_CALL_FUNC(music_room_start);
extern MAIN_CALL_FUNC(title_menu_start);/*for pause.c ... */
extern MAIN_CALL_FUNC(pause_menu_start);
extern MAIN_CALL_FUNC(ask_continue_menu_start);
extern MAIN_CALL_FUNC(rank_select_menu_start);
//

#define pause_out_call_func 	return_call_func/* ポーズ時の戻り先 */
#define menu_out_call_func		return_call_func/* 自機選択時の戻り先 / オプションメニューの戻り先 */

/*
	return_call_func
	★ 用途1: pause_out_call_func : ポーズ時の戻り先を保持。
	★ 用途2: menu_out_call_func  : メニューシステムでメニューがバックする(戻る)場合に何処のメニューへ戻るかを保持。
*/
#endif

/*---------------------------------------------------------
	メニュー関連
---------------------------------------------------------*/

#if 1/*(メニュー関連)*/
/* キャンセル音を鳴らしてメニューに強制復帰 */
extern void menu_cancel_and_voice(void);
#endif


#endif /* _GAME_CORE_ */

#ifndef _JIKI_H_
#define _JIKI_H_

// > 【PSP】PSPで東方やりたい奴8【弾幕】
// > 200 ：名無しさん＠お腹いっぱい。：2009/01/27(火) 18:41:00 ID:xQgI7hCU
// > そろそろSアイテム消して欲しいな。
// > あとFULLパワー時のアイテム引き寄せラインはもうちょっと下の方がいいと思う。
// > 具体的にはこの辺り↓
// > http://www4.uploader.jp/user/isisuke/images/isisuke_uljp00081.bmp
// この意見はわたしも同様に思ったので、みんながそう思ったに違いない。
// この画像は484x283[pixel]で、y==63[line]に赤線が引いてあった。
// pspの解像度は、 480x272[pixel]なので x/272=63/283, x=(63*272)/283 == 60.55[line]
// 差分/製作氏	50(49)[line] 少し上すぎる。
// スレ8の200氏 約60[line] (感覚的に)
// ここでキャラの高さ分もあるし、64(63)かもっと下72(71)ぐらいでもいいのでは？という訳で64にして実験してみる。
// 結果：やっぱもう少し下がいい。8[pixel]といわず16[pixel]ぐらい下がいい。再び実験してみる。
// 結果：こんなもんかな？というわけで80(79)[pixel]に決定。もう8[pixel]下88(87)でもいいかもね。
// 本家の感覚を大事にするならこんなもんだと思う。
//---------------
//「アイテム引き寄せライン」以下で取ると「アイテム引き寄せライン」から322ラインまで距離で減点。
//
//プレイフィールドは 384x448 なので、448-322 == 126
//「アイテム引き寄せライン」は多分上から128ドットかな？
//
//(Bアイテムの大きさが16x16[pixel]なので、Bアイテムの消失判定は 448+(16/2)ラインで行うから、
//最後にBアイテムの判定があるのが、BアイテムのＹ軸中心が456[ライン]にきた場合。
//456-322 == 134, 134-128 == 6. この6[ライン]はそもそも自機中心が画面下部に行けない分が数ドット(5[pixel]?)ある。
//残り1[pixel]は不等号で判定したから? )
//---------------
// もし同比率なら、
// (模倣風アイテム引き寄せライン)/(本家アイテム引き寄せライン) == (模倣風ゲームフィールド縦)/(本家ゲームフィールド縦)
// が成立する。(模倣風アイテム引き寄せライン)が知りたいのだから移行して、
// (模倣風アイテム引き寄せライン) == (本家アイテム引き寄せライン) * (模倣風ゲームフィールド縦)/(本家ゲームフィールド縦)
// (模倣風アイテム引き寄せライン) == 128 * 272/448 == 77.7142857142857142857142857142857 =:= 78[pixel] 単純な比率計算の場合。
// http://hossy.info/game/toho/k_score.php ここの画像を逆算して計算すると 77 [pixel] あたり。
/* アイテム引き寄せライン */
//#define ITEM_GET_BORDER_LINE256		(t256(77))/* 画像から仮定 */
//#define ITEM_GET_BORDER_LINE256		(t256(78))/* 本家と同比率と仮定し逆算 */
#define ITEM_GET_BORDER_LINE256 		(t256(80))/* 中心座標で管理してない奴(-r31) */
/* 現状(r33) プレイ感覚としてこの3つ(77, 78, 80)の中では (80)が一番本家に近い。(77, 78)は上過ぎる。
あまり下にしても面白くないから、(80)程度が妥当な気がする。 */

/* 敵が撃たないライン (このラインより下からは敵が撃たない) */
#define ENEMY_LAST_SHOT_LINE256 	(t256(272-72))


enum /*_select_player_*/
{
	/*0*/	REIMU_A = 0,
	/*1*/	REIMU_B,
	/*2*/	MARISA_A,
	/*3*/	MARISA_B,
//
	/*4*/	REMILIA,
	/*5*/	YUYUKO,
	/*6*/	CIRNO_A,
	/*7*/	CIRNO_Q,
//
	MAX_08_PLAYER
};

/*(全フラグを消す特殊機能)*/
#define JIKI_FLAG_0x00_ALL_OFF							(0x0000)
//--------
/*(割り込みシステム(未定)用に確保)*/
#define II_SYSTEM_TASK_NUMBER							(0x000f)
//--------
/* プレイヤーが上部へ移動し、手動でアイテム収集行う場合にon。 */
#define JIKI_FLAG_0x0010_JYOU_BU_SYUU_SYUU				(0x0010)
/* ボムが発動した際に、自動でアイテム収集を行う場合にon。 */
#define JIKI_FLAG_0x0020_BOMBER_SYUU_SYUU				(0x0020)
/* ボス倒し後に点数(★アイテム)やボスが出すアイテムを自動で収集する。この場合にon。 */
#define JIKI_FLAG_0x0040_BOSS_GO_ITEM_JIDOU_SYUU_SYUU	(0x0040)
//-------
/* ★ [プレイヤーのオプションがショットを撃つ場合の通信用フラグ] */
#define JIKI_FLAG_0x0080_SHOT_KEY_SEND_FOR_OPTION		(0x0080)
/* 1==オプションがショットを撃つ。 */
/* 0==オプションがショットを撃たない。 */
//-------


	#define SCENE_FLAG_0x0000_KAIWA_TATAKAU 	(0x0000)/*(共通)*/
	#define SCENE_FLAG_0x0100_KAIWA_LOAD		(0x0100)/*(共通)*/
	#define SCENE_FLAG_0x0200_KAIWA_MODE		(0x0200)/*(共通)*/
	#define SCENE_FLAG_0x0300_KAIWA_END 		(0x0300)/*(共通)*/
	#define SCENE_FLAG_0x0300_KAIWA_MASK		(0x0300)/*(共通)*/
	#define SCENE_NUMBER_0x0400_DOUCHUU 		(0x0400)
	#define SCENE_NUMBER_0x0800_BOSS_TATAKAU	(0x0800)
	#define SCENE_NUMBER_MASK					(0xff00)

#define USE_r36_SCENE_FLAG (1)
//#define USE_r36_SCENE_FLAG	(0)
#if (1==USE_r36_SCENE_FLAG)
	/*
	SCENE_FLAG:
0000: 0x0000: ステージタイトル、道中。
0001: 0x0100: ステージタイトル、会話読みこみ。"0"
0010: 0x0200: ステージタイトル、会話モード。
0011: 0x0300: ステージタイトル、会話モードおしまい指示。
0100: 0x0400: ボス戦闘前、道中、ボス指示。				[特殊処理on:道中追加]
0101: 0x0500: ボス戦闘前、会話読みこみ。"1" 			[特殊処理on:会話読みこみ]
0110: 0x0600: ボス戦闘前、会話モード。					[特殊処理on:会話モード]
0111: 0x0700: ボス戦闘前、会話モードおしまい指示。		[特殊処理on:会話モードおしまい指示]
//
1000: 0x0800: ボス戦闘中。								[特殊処理off:なし]
1001: 0x0900: ボス戦闘後、good会話読みこみ。"2"(or"3")	[特殊処理on:会話読みこみ]
1010: 0x0a00: ボス戦闘後、good会話モード。				[特殊処理on:会話モード]
1011: 0x0b00: ボス戦闘後、good会話おしまい指示。		[特殊処理on:会話モードおしまい指示]
1100: 0x0c00: ボス戦闘中。(ダミー。この状態には絶対にならない。この状態だとボス体力ゲージが描けない)
1101: 0x0d00: ボス戦闘後、bad会話読みこみ。"3"
1110: 0x0e00: ボス戦闘後、bad会話モード。
1111: 0x0f00: ボス戦闘後、bad会話モードおしまい指示。
	  0x1000:
	  0x8000:プレイヤーループを抜ける処理(とりあえず)
*/
//	#define SCENE_FLAG_TEST_IS_BOSS 			(0x0800)
//	#define SCENE_NUMBER_MASK_IS_BOSS			(0x0d00)


	#define IS_SCENE(aaa)		((cg.state_flag & SCENE_NUMBER_MASK))
	#define NEXT_SCENE			cg.state_flag += 0x0100;

//
	#define IS_SCENE_DOUCHU_TUIKA		(SCENE_NUMBER_0x0400_DOUCHUU>=(cg.state_flag & (SCENE_NUMBER_MASK)))
	#define IS_SCENE_KAIWA_TATAKAU		(SCENE_FLAG_0x0000_KAIWA_TATAKAU	==(cg.state_flag & (SCENE_FLAG_0x0300_KAIWA_MASK)))
	#define IS_SCENE_KAIWA_LOAD 		(SCENE_FLAG_0x0100_KAIWA_LOAD		==(cg.state_flag & (SCENE_FLAG_0x0300_KAIWA_MASK)))
	#define IS_SCENE_KAIWA_MODE 		(SCENE_FLAG_0x0200_KAIWA_MODE		==(cg.state_flag & (SCENE_FLAG_0x0300_KAIWA_MASK)))
	#define IS_SCENE_END_KAIWA_MODE 	(SCENE_FLAG_0x0300_KAIWA_END		==(cg.state_flag & (SCENE_FLAG_0x0300_KAIWA_MASK)))
#else
	#define STATE_FLAG_0x0100_IS_LOAD_KAIWA_TXT 			(0x0100)
	/* on */

	/* ゲーム中、会話モードで on になる。ゲームが再開されると off になる。*/
	#define STATE_FLAG_0x0200_IS_KAIWA_MODE 				(0x0200)
	/* on */

	#define STATE_FLAG_0x0300_END_KAIWA_MODE				(0x0400)
	/* on */

	/* ボスと戦う際にon(ボス前イベントではOFF) */
	#define STATE_FLAG_0x0800_IS_BOSS						(0x0800)

	/* ボスを倒した場合にon(時間切れではOFF) */
	//#define STATE_FLAG_09_IS_WIN_BOSS 					(0x0400)
	/* on */
//-------
	#define STATE_FLAG_13_GAME_TERMINATE					(0x1000)/* result後にゲーム終了 */
	#define STATE_FLAG_14_DOUCHU_TUIKA						(0x2000)/* 道中でコマンド追加紺処理を行う場合on */
	#define STATE_FLAG_15_DRAW_BOSS_GAUGE					(0x4000)
//-------
	#define IS_SCENE_DOUCHU_TUIKA		(cg.state_flag & STATE_FLAG_14_DOUCHU_TUIKA)
	#define IS_SCENE_KAIWA_TATAKAU		(0==(cg.state_flag & STATE_FLAG_0x0200_IS_KAIWA_MODE))
	#define IS_SCENE_KAIWA_LOAD 		(cg.state_flag & STATE_FLAG_0x0100_IS_LOAD_KAIWA_TXT)
	#define IS_SCENE_KAIWA_MODE 		(cg.state_flag & STATE_FLAG_0x0200_IS_KAIWA_MODE)
	#define IS_SCENE_END_KAIWA_MODE 	(cg.state_flag & STATE_FLAG_0x0300_END_KAIWA_MODE)
#endif


/*(使用予定あり。未検証)*/
#define USE_BOSS_JIKANGIRE	(0)
/* 1:使う。 0:使わない。スペカ時間切れの場合アイテム出さない機能。未検証。 */


#if (1==USE_BOSS_JIKANGIRE)/*(使用予定あり。未検証)*/
	/*(発弾時にspellcpuで消す)*/
	#define JIKI_FLAG_16_0x8000_BOSS_JIKAN_GIRE 			(0x8000)
#endif


//#define STATE_FLAG_0123_AUTO_GET_ITEM (JIKI_FLAG_0x0010_JYOU_BU_SYUU_SYUU|JIKI_FLAG_0x0020_BOMBER_SYUU_SYUU|JIKI_FLAG_0x0040_BOSS_GO_ITEM_JIDOU_SYUU_SYUU)



//	/*extern*/global void *callback_gu_draw_haikei_modosu;	/* (カード時から通常時へ)戻す用 */
//	/*extern*/global void *callback_gu_draw_haikei_supeka;	/* カード時用 */
//	global void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;
//	extern void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;				/* 背景ウィンドウ表示フラグ */
enum
{
	SDL_00_VIEW_SCREEN		= 0,
	SDL_01_BACK_SCREEN,
//	SDL_02_TEX_SCREEN,
//	SDL_03_000_SCREEN,
	SDL_99_MAX_SCREEN,
};

#define USE_PAD_STRUCT (1)
/*(0: 共通 / GAME_CORE_GLOBAL_CALLBACKに入れる)*/
/*(1: 独立 / GAME_CORE_GLOBAL_CALLBACKに入れない)*/

/*(0: アナログ量を使わない[リプレイ対応するならアナログ量を使わない方が妥当な気がする。1byteフォーマット(上下左右,ＡＢＣ[Slow])に出来る。さらにメモリ上でランレングス圧縮できる。(圧縮しながら記録/再生)])*/
/*(1: アナログ量を使う[アナログの場合ランレングス圧縮が効かないので純粋にメモリを食う。0.5M[bytes]程メモリを食うが、取れるか不明])*/
//r39で廃止済み#define USE_ANA LOG_VALUE (0)
/*[1/60sec]==4[byte]*/
/*[1sec]==60*4[byte]*/
/*[1min]==60*60*4[byte]*/
/*[30min]==30*60*60*4[byte]==432000==421.875k[Bytes]==0.4119873046875M[Bytes]*/
/*(模倣風はクリアまで30分以上かかる)*/
typedef struct _game_core_global_callback_
{
	void (*main_call_func)(void);/* メインコールバック */
	void (*return_call_func)(void);/* ポーズ復帰後の戻り先コールバック */
	/* カード時でない通常時のプログラムを指示 */
	void *callback_gu_draw_haikei_modosu;	/* (カード時から通常時へ)戻す用。(==通常時) */
	/* カード時のプログラムを指示 */
	void *callback_gu_draw_haikei_supeka;	/* カード時用 */
//
	/* 現在使用するプログラムを設定 */
	void (*callback_gu_draw_haikei)(void);	/* ゲーム中、背景ウィンドウ表示フラグ */
/*
	文字を描画する描画先のテクスチャー
*/
		#if (0==USE_32BIT_DRAW_MODE)
	u16 *kanji_window_screen_image; 					/* メッセージウィンドウ(画像) */
		#else
	u32 *kanji_window_screen_image; 					/* メッセージウィンドウ(画像) */
		#endif
//extern S_DL_Surface *screen;
/*extern*/ SDL_Surface *sdl_screen[SDL_99_MAX_SCREEN];/*(4)*/
//
	//
//	u16 *show_frame;	/*(表示画面)*/
//	u16 *draw_frame;	/*(描画画面)*/
//	u16 *work_frame;	/*(一時作業用画面)*/
//	u16 *tex_frame; 	/*(キャッシュ画像用画面)*/
	//
	#if (0==USE_PAD_STRUCT)
	/*(共通)*/
	u32 pad_data;						/*今回入力*/				//	extern	u32 cg_m y_pad;
	u32 pad_data_alter; 				/*前回入力*/				//	extern	u32 cg_m y_pad_alter;
	#endif/*(USE_PAD_STRUCT)*/
} GAME_CORE_GLOBAL_CALLBACK;
extern GAME_CORE_GLOBAL_CALLBACK cb;

	#if (1==USE_PAD_STRUCT)/*(r34)*/
	/*(独立)*/
	/* アライメント関係(???) (s16)で GAME_CORE_GLOBAL_CLASSに入れると巧くいかない */
	/* 意図的に入れないもの */
typedef struct _psp_pad_global_class_
{
	u32 pad_data;						/*今回入力*/				//	extern	u32 cg_m y_pad;
	u32 pad_data_alter; 				/*前回入力*/				//	extern	u32 cg_m y_pad_alter;
} PSP_PAD_GLOBAL_CLASS;
extern PSP_PAD_GLOBAL_CLASS psp_pad;
	#else/*(0==USE_PAD_STRUCT)*/
		/*(共通)*/
		#define psp_pad cb/*_global_pad_*/
	#endif/*(USE_PAD_STRUCT)*/

typedef struct _game_core_global_class_
{
	u32 state_flag; 	/* 設定フラグ(集) */
	int weapon_power;	/* 0x00-0x80  (0-128 の129段階==本家と同じ)   max==128==「129段階」*/
	int chain_point;
	s32 bomber_time;	/* Use Gu */  // /* bomb_wait */ /* ボムの有効時間 */
//[0x10 4]
	u32 game_score; 	/* スコア得点 */
	u32 graze_point;	/* グレイズ得点 */
	int bombs;			/* ボム数 */
	int zanki;			/* 残りチャンス */
//[0x20 8]
	s32 game_difficulty;/* (pspのmax,min命令を使っているので signed int の必要がある ) */
	s32 game_rank;		/* ランク変動システムのゲームランク */
	unsigned int jiki_weapon_level; 	/* (r35u2)強さのみ */		//	/*u8*/unsigned int jiki_weapon_level_offset;/* 装備した武器レベル(番号)の８倍にプレイヤー値を足した値をオフセットとして保持 */
	unsigned int conv_bg_alpha;
//[0x30 12]
	int current_bg0_y_scroll_speed256;	/* bg0のスクロール、現在速度 */
	/*(描画用)*/
	int draw_boss_hp_value; /* ボスhp描画値 */ ///??????????
	#if (1)/*(仮対応)*/
	/* 暫定 SDL フォント */
//	int dummy111;/*無いと巧く動作しない*/
//	暫定 [ラインレンダーに移行予定。]
	int msg_time;/* メッセージ(仮対応)表示時間 */	/* せりふウィンドウ表示時間(仮) */
	#endif
//	/* 集計システム(player_data) */
	int player_data_use_continue;	/* コンティニュー回数 */
//[0x40 16]
	int player_data_count_miss; 	/* ミス回数 */
	int player_data_used_bomber;	/* ボム使用回数 */
	int player_data_use_kurai_bomb; /* 喰らいボム成功回数 */
	int player_data_count_bonus;	/* カードボーナス回数 */
//[0x48 18]
//	この場所は 4[byte]境界ではダメで、何故か32[byte]境界に合わせないと正常動作しない。(4[byte]境界では異常動作する)
	s8 game_now_stage;				/* 現在ステージ番号 */
	s8 game_continue_stage; 		/* 現在コンティニューするステージ番号を保持 */
	s8 game_now_max_continue;		/* コンティニュー可能な回数 */
	s8 game_practice_mode;			/* 練習モード */
//
	s8 side_panel_draw_flag;/* パネル表示on(0以外)/off(0) */
	s8 chuu_boss_mode;
	s8 dummy_status;				/* [未使用]ダミー */
	u8 player_option_mode;/* ここに入れない方がいいかも？、とりあえず */
//	int dummy2222;/*無いと何故か巧く動作しない*/
//	int dummy3333;/*無いと何故か巧く動作しない*/
	#if (1)/*(仮対応)*/
	/* 暫定 SDL フォント */
	int PSPL_font_x;
	int PSPL_font_y;
	#endif
//[0x50 20]
//[0x60 24]
} GAME_CORE_GLOBAL_CLASS;
extern GAME_CORE_GLOBAL_CLASS cg;
// /* 予備2(.align合わせで必要) */
//	s8 dr aw_flag_ka iwa_screen;		/* 会話画面の会話用せりふウィンドウ表示フラグ */



//
//	u8 laser_mode;	/* てすと(レーザーモード 0:off, 1:on) */
	/* 集計システム以外の保持状態 */
//	u8 game_select_player;

/* 意図的に入れないもの */
extern unsigned int cg_game_select_player;/* : (将来はともかく)現状(r33)は GAME_CORE_GLOBAL_CLASSに入れない方が良いっぽい。 */



#if 1
//
//extern int bo ss_life_value;	/* ボスhp体力値 / ボス魔方陣サイズ描画値 */
#endif


#define USE_HOLD_GAME_MODE	(0)
#if (1==USE_HOLD_GAME_MODE)
extern void hold_game_mode_on(void);/* ゲーム時間の一時停止 */
extern void hold_game_mode_off(void);/* ゲーム時間の動作開始 */
#endif

//#define IS_KEY BOARD_PULLED ((0==psp_pad.pad_data)&&((psp_pad.pad_data)^(psp_pad.pad_data_alter)))/* 何かキーを離されたら */
/*	キーを離した瞬間が確定とする。押した瞬間が確定だと、押された間だけ動作する物が同時にあると都合が悪い。 */
/*
	r31:原作ではキーを押した瞬間が確定のものが多いので、おかしいところはキーを押した瞬間が確定に修正した。
*/

/* 127 かと思っていたけど、128みたい(つまり129段階) */
//#define MAX_POWER_IS_128 (127)/* 0x00-0x7f  (0-127 の128段階) */
//#define MAX_POWER_IS_128 (128)/* 0x00-0x80  (0-128 の129段階) */
#define MAX_POWER_IS_128 (128)

//typedef struct
//{
//	int strength;
//} WEAPON_BASE;

//#define DEFAULT_MAX_CONTINUE	(3)
//#define DEFAULT_MAX_CONTINUE	(30)
#define DEFAULT_MAX_CONTINUE	(30)


#endif /* _JIKI_H_ */


#ifndef _BONUS_H_
#define _BONUS_H_

enum
{
	SCORE_76800 = 0,
				SCORE_10,	/* (MAX時以外の) [P]は10点 */
							SCORE_20,	SCORE_30,
	SCORE_40,	SCORE_50,	SCORE_60,	SCORE_70,
	SCORE_80,	SCORE_90,	SCORE_100,	SCORE_200,
	SCORE_300,	SCORE_400,	SCORE_500,	SCORE_600,
	SCORE_700,	SCORE_800,	SCORE_900,	SCORE_1000,
	SCORE_2000, SCORE_3000, SCORE_4000, SCORE_5000,
	SCORE_6000, SCORE_7000, SCORE_8000, SCORE_9000,
	SCORE_10000,SCORE_11000,SCORE_12000,SCORE_51200/*(最大値)*/
};

#define ITEM_MOVE_FLAG_00_NONE			0x00
#define ITEM_MOVE_FLAG_01_COLLECT		0x01
#define ITEM_MOVE_FLAG_02_WAIT			0x02
//
#define ITEM_MOVE_FLAG_02_RAND_X		0x04
#define ITEM_MOVE_FLAG_04_RAND_Y		0x08
#define ITEM_MOVE_FLAG_06_RAND_XY		(ITEM_MOVE_FLAG_02_RAND_X|ITEM_MOVE_FLAG_04_RAND_Y)

#define ITEM_CREATE_MODE_01 	((4*8/*ITEM_MAX*/)*0)
#define ITEM_CREATE_MODE_02 	((4*8/*ITEM_MAX*/)*1)

/*(アイテム作成系)*/
//廃止extern void item_create(			OBJ *src, int type, int num, int up_flags);
extern void item_create_mono(			OBJ *src, int type);
extern void item_create_flags_num(		OBJ *src, int type, int num_of_creates, int set_item_flags);
extern void item_create_for_boss(		OBJ *src, int item_create_mode);
extern void item_create_random_table(	OBJ *src);

/*()*/
extern void bonus_info_score_nodel( 		OBJ *src, int score_type);		/*int x, int y*/
extern void bonus_info_any_score_nodel( 	OBJ *src, u32 score_num_pts);	/*int x, int y*/

/*(弾を全て星点に変換する)*/
extern void bullets_to_hosi(void);

/*(スコア加算する場合は、カンスト チェックがあるので、必ずここを使う)*/
extern void player_dummy_add_score(u32 score_num_pts);
extern void player_add_score(u32 score_num_pts);
#if (1==USE_EXTEND_CHECK)
extern void player_check_extend_score(void);/*(エクステンドチェック)*/
#endif /* (1==USE_EXTEND_CHECK) */

/*(難易度スコア補正)*/
extern u32 adjust_score_by_difficulty(u32 convert_score);

#endif /* _BONUS_H_ */



#ifndef _LOAD_STAGE_FILE_H_
#define _LOAD_STAGE_FILE_H_

//#define  MAX_IVENT_ENTRY	512
//#define  MAX_PARA1_36		/*64*/(64-20-8-4+32)/*(64-(4*5))*/

typedef struct _stage_command_
{
	u32 		v_time; 				/* 正数(0<v_time)の場合 1/60 単位の出現時間カウンタ。(-1)の場合出現済み。(0の場合error何もしない。) */
	struct _stage_command_ *next;
	s32 		user_locate_x;			/* [pixel単位] locate_x */
	s32 		user_locate_y;			/* [pixel単位] locate_y */
	s32 		user_angle1024;			/* [deg1024単位] vector_x */
	s32 		user_speed256;			/* [t256単位] vector_y */
//(16[Bytes] == (4x4) )
	u8			user_i_code;			/* intermediate code 中間コード */
	u8			user_255_code;			/* 'E'とか'T'とか	*/
	u8			user_item8; 			/* item */
	u8			user_select_gazou;		/* アニメ/画像種類 */
	int 		user_hp;				/* hp */
	int 		user_score; 			/* score */
	int 		user_kougeki_type;		/* 敵が攻撃する場合のカードの種類 */
//(20[Bytes] == (4x5) )
//	char		user_string[MAX_PARA1_36/*(64)*/];/* strings */
//(64[Bytes] == (4x4)+(4)+(44) )
} GAME_COMMAND; 	/* .align 64 [bytes] */
//	u16 	dummy_scroll_speed256;	/* para3 scroll speed256 */


/* 中間コード形式のコマンド */
/* ゲーム中は、中間コード形式のコマンドを扱い構文解析はしない。(構文解析はload_stage.cで行う) */

enum /*_game_command_code_*/
{
/*中間コード*/
/*0x07*/		GC_CODE_00_ZAKO = 0,				/* 敵 ザコ */
/*0x06*/		GC_CODE_01_CHUU_BOSS_COMMON,	/* 敵 ザコ中-ボス(スク リプト起動しない) */
/*0x05*/		GC_CODE_02_BOSS_COMMON, 		/* 敵 (ボス / 中-ボス)用 スク リプト起動 */
/*0x02*/		GC_CODE_03_BG_CONTROL,			/* 背景コントロール。(スクロール速度等) */
//
/*0x01*/		GC_CODE_04_ALL_CLEAR,			/* ゲーム 全ステージ クリアー */
/*0x01*/		GC_CODE_05_RESURVED,			/* 予約 */
/*0x01*/		GC_CODE_06_RESURVED,			/* 予約 */
/*0x01*/		GC_CODE_07_RESURVED,			/* 予約 */
//
/*0x00*/	//	GC_CODE_00_SJIS_TEXT = 0,		/* 漢字表示[番兵区切りとしても必要] */
/*0x03*/	//	GC_CODE_03_CHECK_SECRET_BONUS,	/* 隠しボーナスチェック "出力" */
/*0x04*/	//	GC_CODE_04_BEGIN_SECRET_BONUS,	/* 隠しボーナスチェック "確認" */
			//
/*0x08*/	/* 拡張可能 */
/*...*/ 	/* ... */
/*0x2f*/	/* 拡張可能 */
/*0x30*/	GC_CODE_MAX,		/* 0x30 == '0' == カードコマンド (== 速度コマンド、廃止) */
};

#endif /* _LOAD_STAGE_FILE_H_ */


#ifndef _BOSS_COMMON_H_
#define _BOSS_COMMON_H_

/* カードシステム */

/* カードの初期化。カードが変わると毎回行う必要がある。 */
extern OBJ_CALL_FUNC(card_maikai_init); 						/*(毎回初期化)*/
extern OBJ_CALL_FUNC(card_maikai_init_and_get_spell_number);	/*(毎回初期化)+(現在撃つべく番号をカードシステムから取得)*/


#endif /* _BOSS_H_ */


#ifndef _ATARI_HANTEI_H_
#define _ATARI_HANTEI_H_

enum
{
	BAKUHATSU_MINI00 = 0,/* 小爆発 */
	BAKUHATSU_MINI01,
	BAKUHATSU_MINI02,
	BAKUHATSU_MINI03,
//
	BAKUHATSU_ZAKO04,/* ザコ消滅爆発 */
	BAKUHATSU_ZAKO05,
	BAKUHATSU_ZAKO06,
	BAKUHATSU_ZAKO07,
//
	BAKUHATSU_FIRE08,/* 火炎爆発 */
	BAKUHATSU_FIRE09,
	BAKUHATSU_FIRE10,
	BAKUHATSU_FIRE11,
//
	BAKUHATSU_MOVE12,/* 移動爆発 */
	BAKUHATSU_MOVE13,
	BAKUHATSU_MOVE14,
	BAKUHATSU_MOVE15,
};
#define TAMA_ATARI_02					((2)<<8)
#define TAMA_ATARI_04					((4)<<8)

#define TAMA_ATARI_COMMON16_PNG 		TAMA_ATARI_02
#define TAMA_ATARI_MARU16_PNG			TAMA_ATARI_02
#define TAMA_ATARI_KNIFE18_PNG			TAMA_ATARI_02
#define TAMA_ATARI_JIPPOU32_PNG 		TAMA_ATARI_02

#define TAMA_ATARI_OODAMA_08_PNG		TAMA_ATARI_04



#define TAMA_ATARI_BULLET_DUMMY 		TAMA_ATARI_02
#define JIKI_ATARI_ITEM_16				((16)<<8)
#define JIKI_ATARI_ITEM_80				((80)<<8)


#define ZAKO_ATARI02_PNG				TAMA_ATARI_02
#define ZAKO_ATARI04_PNG				TAMA_ATARI_04
#define ZAKO_ATARI16_PNG				JIKI_ATARI_ITEM_16
#define ZAKO_ATARI_HOMING16_PNG 		JIKI_ATARI_ITEM_16


	/* 発弾エフェクトの時間分 */
	#define HATUDAN_FRAME64 	(64)

/**/extern int boss_hamidasi;		/* 「移動できなかったフラグ」(使用前に手動でOFF==0にしとく) */
typedef struct /*_clip_class_*/
{
	POINT256 boss_clip_min; 	/* ボス移動範囲(最小値) */
	POINT256 boss_clip_max; 	/* ボス移動範囲(最大値) */

	POINT256 bullet_clip_min;	/* 弾の範囲(最小値) */
	POINT256 bullet_clip_max;	/* 弾の範囲(最大値) */
} RECT_CLIP_CLASS;
extern RECT_CLIP_CLASS rect_clip;

extern void bakuhatsu_add_type_ccc(int type);
extern OBJ_CALL_FUNC(bakuhatsu_add_rect);
extern void bakuhatsu_add_circle(OBJ/**/ *src, int mode);
extern OBJ_CALL_FUNC(bakuhatsu_add_zako04);

extern void callback_hit_zako(OBJ/**/ *src, OBJ/**/ *tama);

#endif /* _ATARI_HANTEI_H_ */
