
/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
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
#define PLAYER_SPLIT_LINE256		(t256(80))
// http://hossy.info/game/toho/k_score.php ここの画像を逆算して計算すると 77 dots あたり。
//#define PLAYER_SPLIT_LINE256		(t256(77))

/* このラインより下からは敵が撃たない */
#define ENEMY_LAST_SHOT_LINE256 	(t256(272-72))


enum /*_select_player_*/		// [***090203		追加
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
	MAX_PLAYER
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

#define STATE_FLAG_14_GAME_LOOP_QUIT				(0x2000)
#define STATE_FLAG_15_KEY_SHOT						(0x4000)
//#define STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL 		(0x8000)/* 廃止? */



typedef struct _player_spec_
{
	int state_flag; 	/* 設定フラグ */	// [***090116		追加
	int weapon_power;	/* 0x00-0x80  (0-128 の129段階==本家と同じ)   max==128==「129段階」*/
	int chain_point;
	int player_now_stage; /* */
//
	u32 game_score; 	/* スコア得点 */
	u32 graze_point;	/* グレイズ得点 */
	int bombs;			/* ボム数 */
	int zanki;			/* 残りチャンス */
//
	int bomber_time;		/* Use Gu */
//	int difficulty;
//
//	/* 集計システム */
	int use_continue;	/* コンティニュー回数 */
	int count_miss; 	/* ミス回数 */
	int used_bomber;	/* ボム使用回数 */
	int use_kurai_bomb; /* 喰らいボム成功回数 */
	int count_bonus;	/* スペルカードボーナス回数 */
//
	int laser_mode; 	/* てすと(レーザーモード 0:off, 1:on) */
} PLAYER_SPEC;
extern PLAYER_SPEC pd;

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
