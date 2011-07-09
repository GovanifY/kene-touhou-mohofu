
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _PLAYER_H_
#define _PLAYER_H_

// > 【PSP】PSPで東方やりたい奴8【弾幕】
// > 200 ：名無しさん＠お腹いっぱい。：2009/01/27(火) 18:41:00 ID:xQgI7hCU
// > そろそろSアイテム消して欲しいな。
// > あとFULLパワー時のアイテム引き寄せラインはもうちょっと下の方がいいと思う。
// > 具体的にはこの辺り↓
// > ttp://www4.uploader.jp/user/isisuke/images/isisuke_uljp00081.bmp
// この意見はわたしも同様に思ったので、みんながそう思ったに違いない。
// この画像は484x283[dots]で、y==63[line]に赤線が引いてあった。
// pspの解像度は、 480x272[dots]なので x/272=63/283, x=(63*272)/283 == 60.55[line]
// 差分/製作氏	50(49)[line] 少し上すぎる。
// スレ8の200氏 約60[line] (感覚的に)
// ここでキャラの高さ分もあるし、64(63)かもっと下72(71)ぐらいでもいいのでは？という訳で64にして実験してみる。
// 結果：やっぱもう少し下がいい。8[dots]といわず16[dots]ぐらい下がいい。再び実験してみる。
// 結果：こんなもんかな？というわけで80(79)[dots]に決定。もう8[dots]下88(87)でもいいかもね。
// 本家の感覚を大事にするならこんなもんだと思う。
//---------------
//「アイテム引き寄せライン」以下で取ると「アイテム引き寄せライン」から322ラインまで距離で減点。
//
//プレイフィールドは 384x448 なので、448-322 == 126
//「アイテム引き寄せライン」は多分上から128ドットかな？
//
//(Bアイテムの大きさが16x16[dot]なので、Bアイテムの消失判定は 448+(16/2)ラインで行うから、
//最後にBアイテムの判定があるのが、BアイテムのＹ軸中心が456[ライン]にきた場合。
//456-322 == 134, 134-128 == 6. この6[ライン]はそもそも自機中心が画面下部に行けない分が数ドット(5[dot?])ある。
//残り1[dot]は不等号で判定したから? )
//---------------
// もし同比率なら、
// (模倣風アイテム引き寄せライン)/(本家アイテム引き寄せライン) == (模倣風ゲームフィールド縦)/(本家ゲームフィールド縦)
// が成立する。(模倣風アイテム引き寄せライン)が知りたいのだから移行して、
// (模倣風アイテム引き寄せライン) == (本家アイテム引き寄せライン) * (模倣風ゲームフィールド縦)/(本家ゲームフィールド縦)
// (模倣風アイテム引き寄せライン) == 128 * 272/448 == 77.7142857142857142857142857142857 =:= 78[dots] 単純な比率計算の場合。
// http://hossy.info/game/toho/k_score.php ここの画像を逆算して計算すると 77 dots あたり。
/* アイテム引き寄せライン */
//#define PLAYER_SPLIT_LINE256		(t256(77))/* 画像から仮定 */
//#define PLAYER_SPLIT_LINE256		(t256(78))/* 本家と同比率と仮定し逆算 */
#define PLAYER_SPLIT_LINE256		(t256(80))/* 中心座標で管理してない奴(-r31) */
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

/*なし*/
#define STATE_FLAG_00_NONE							(0x0000)
/* [上部自動収集] MAX時にプレイヤー上部収集で自動アイテム収集 */
#define STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM		(0x0001)
/* [ボムによる自動収集] ボム発動で自動アイテム収集 */
#define STATE_FLAG_02_BOMB_AUTO_GET_ITEM			(0x0002)
/* [スコア自動収集] ボス倒し後に自動点数収集(★アイテム用) */
#define STATE_FLAG_03_SCORE_AUTO_GET_ITEM			(0x0004)
/* [魔理沙の場合 アイテム上部収集がいつでも可能] */
#define STATE_FLAG_04_IS_MARISA 					(0x0008)

/* ボスと戦う際にon(ボス前イベントではOFF) */
#define STATE_FLAG_05_IS_BOSS						(0x0010)

/* ゲーム中シナリオスクリプトモードで on になる。ゲームが再開されると off になる。*/
#define STATE_FLAG_06_IS_SCRIPT 					(0x0020)
/* on */
#define STATE_FLAG_07_IS_GRAZE						(0x0040)
/* on */
//#define STATE_FLAG_08_OPTION_HIDE 				(0x0080)/* 廃止 */

/* on */
//efine ST ATE_FLAG_IS_PANEL_WINDOW_SHIFT			(8)
//efine ST ATE_FLAG_09_IS_PANEL_WINDOW				(0x0100)
/* ボスを倒した場合にon(時間切れではOFF) */
#define STATE_FLAG_09_IS_WIN_BOSS					(0x0100)
/* on */
#define STATE_FLAG_10_IS_LOAD_SCRIPT				(0x0200)
/* on */
//#define STATE_FLAG_11_IS_BOSS_DESTROY 			(0x0400)/* 廃止 */
/* on */
#define STATE_FLAG_12_END_SCRIPT					(0x0800)
/* on */
#define STATE_FLAG_13_DRAW_BOSS_GAUGE				(0x1000)

//#define STATE_FLAG_0123_AUTO_GET_ITEM (STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM|STATE_FLAG_02_BOMB_AUTO_GET_ITEM|STATE_FLAG_03_SCORE_AUTO_GET_ITEM)

//#define ST ATE_FLAG_14_GAME_LOOP_QUIT 			(0x2000)
#define STATE_FLAG_14_ZAKO_TUIKA					(0x2000)/* 雑魚追加処理をする場合on */

#define STATE_FLAG_15_KEY_SHOT						(0x4000)
//#define STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL		(0x8000)/* 廃止? */

#define STATE_FLAG_16_GAME_TERMINATE		(0x8000)/* result後にゲーム終了 */


typedef struct _game_core_global_class_
{
	int state_flag; 	/* 設定フラグ(集) */
	int weapon_power;	/* 0x00-0x80  (0-128 の129段階==本家と同じ)   max==128==「129段階」*/
	int chain_point;
	int bomber_time;	/* Use Gu */
//
	u32 game_score; 	/* スコア得点 */
	u32 graze_point;	/* グレイズ得点 */
	int bombs;			/* ボム数 */
	int zanki;			/* 残りチャンス */
//
//	/* 集計システム(player_data) */
	int player_data_use_continue;	/* コンティニュー回数 */
	int player_data_count_miss; 	/* ミス回数 */
	int player_data_used_bomber;	/* ボム使用回数 */
	int player_data_use_kurai_bomb; /* 喰らいボム成功回数 */
	int player_data_count_bonus;	/* スペルカードボーナス回数 */
//
	int msg_time;/* メッセージ(仮対応)表示時間 */
//
//	u8 laser_mode;	/* てすと(レーザーモード 0:off, 1:on) */
//	u8 dummy1;
	/* 集計システム以外の保持状態 */
//	u8 game_select_player;
//遅過ぎる。	u8 game_difficulty/* = RANK_EASY*/; 	/*	RANK_NORMAL*/
//遅過ぎる。	int game_difficulty;
//
	s8 game_now_stage;				/* 現在ステージ番号 */
	s8 game_continue_stage; 		/* 現在コンティニューするステージ番号を保持 */
	s8 game_now_max_continue;		/* コンティニュー可能な回数 */
	s8 game_practice_mode;			/* 練習モード */
//
	s8 side_panel_draw_flag;/* パネル表示on(0以外)/off(0) */
	s8 chuu_boss_mode;//1802514
	s8 draw_flag_script_screen; 	/* せりふウィンドウ表示フラグ */
	s8 dummy2;/* 予備2(.align合わせで必要) */
} GAME_CORE_GLOBAL_CLASS;
extern GAME_CORE_GLOBAL_CLASS cg;

/* 意図的に入れないもの */
extern unsigned int cg_game_select_player;/* cg_game_difficulty: (将来はともかく)現状(r33)は GAME_CORE_GLOBAL_CLASSに入れない方が良いっぽい。 */
extern unsigned int cg_game_difficulty;/* cg_game_difficulty: GAME_CORE_GLOBAL_CLASSに入れると速度低下する。 */

// /*extern*/ int msg_time; 					/* せりふウィンドウ表示時間(仮) */
#if 1
	#if 0/* アライメント関係(???) (s16)で GAME_CORE_GLOBAL_CLASSに入れると巧くいかない */
	/* 意図的に入れないもの */
	extern	u32 cg_my_pad;			/*今回入力*/
	extern	u32 cg_my_pad_alter;	/*前回入力*/
	extern	s16 cg_analog_x;		/* アナログ量、補正済み */
	extern	s16 cg_analog_y;		/* アナログ量、補正済み */
	#else/*(r34)*/
typedef struct _psp_pad_global_class_
{
	u32 pad_data;						/*今回入力*/
	u32 pad_data_alter; 				/*前回入力*/
	s16 analog_absolute_value_x;		/* アナログ量、補正済み */
	s16 analog_absolute_value_y;		/* アナログ量、補正済み */
} PSP_PAD_GLOBAL_CLASS;
extern PSP_PAD_GLOBAL_CLASS psp_pad;
	#endif
//
extern int draw_boss_hp_value;	/* ボスhp描画値 */
extern int boss_life_value; 	/* ボスhp体力値 / ボス魔方陣サイズ描画値 */
#endif
extern int boss_x256;
extern int boss_y256;


#define USE_HOLD_GAME_MODE	(0)
#if (1==USE_HOLD_GAME_MODE)
extern void hold_game_mode_on(void);/* ゲーム時間の一時停止 */
extern void hold_game_mode_off(void);/* ゲーム時間の動作開始 */
#endif

//extern u32 my_pad;			/*今回入力*/
//extern u32 my_pad_alter;		/*前回入力*/
//extern /*global*/short cg_analog_x; /* アナログ量、補正済み */
//extern /*global*/short cg_analog_y; /* アナログ量、補正済み */

//#define IS_KEY BOARD_PULLED ((0==my_pad)&&(my_pad^my_pad_alter))/* 何かキーを離されたら */
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


#endif /* _PLAYER_H_ */
