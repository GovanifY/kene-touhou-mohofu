#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "game_main.h"

#include "bonus.h"
#include "load_stage.h"

enum
{
	BAKUHATSU_MINI00 = 0,/*小爆発*/
	BAKUHATSU_MINI01,
	BAKUHATSU_MINI02,
	BAKUHATSU_MINI03,
//
	BAKUHATSU_ZAKO04,/*ザコ消滅爆発*/
	BAKUHATSU_ZAKO05,
	BAKUHATSU_ZAKO06,
	BAKUHATSU_ZAKO07,
//
	BAKUHATSU_FIRE08,/*火炎爆発*/
	BAKUHATSU_FIRE09,
	BAKUHATSU_FIRE10,
	BAKUHATSU_FIRE11,
//
	BAKUHATSU_MOVE12,/*移動爆発*/
	BAKUHATSU_MOVE13,
	BAKUHATSU_MOVE14,
	BAKUHATSU_MOVE15,
};


#if 1
	/* 角度値(0-511)が512 を超えた場合の特殊機能 */
//
	/* 自機狙い弾 */
	#define ANGLE_JIKI_NERAI_DAN	(999)
//
	/* 特別に撃たない弾 */
	#define ANGLE_NO_SHOT_DAN		(777)
//
	#if 0
		/* 偶数弾の場合に自機狙い(動かないと当たる) / even bullet */
		//#define ANGLE_JIKI_NERAI_GUSUDAN 999
		/* 奇数弾の場合に自機狙い(動かなければ当たらない) / odd bullet */
		//#define ANGLE_JIKI_NERAI_KISUDAN 888
		/*
		よく考えたら、ANGLE_JIKI_NERAI_KISUDAN は必要ない。
		ANGLE_JIKI_NERAI_DAN だけあれば良く、
		発弾数に偶数を指定すれば偶数弾。
		発弾数に奇数を指定すれば奇数弾。
		あたりまえだけど、気が付かなかったww。
		*/
	#endif
#endif

//typedef struct
//{
//	int health; 		/* 共用 */
//	int score;			/* 共用 */
//} ENEMY_BASE;

//typedef struct
//{
//------------ スペカ関連

//	int boss_health;		/* 共用 */	/* (下位)体力 */
//	int /*boss_*/score; 	/* 共用 */
	//int boss_life;			/* 共用 */	// (上位)体力
//------------ 弾幕関連
//	int dummy_tmp_angleCCW512;	/* 保持角度 */
#define boss_base_danmaku_type		user_data10
#define boss_base_danmaku_time_out	user_data11
#define boss_base_danmaku_test		user_data12
#define boss_base_state001			user_data13
	/*	int dummy_bbbb;*/ /* 予約(多分タイマーで使う) */
//} BO SS_BASE;

extern int boss_hamidasi;			/* 「移動できなかったフラグ」(使用前に手動でOFF==0にしとく) */
extern POINT256 boss_clip_min;		/* ボス移動範囲(最小値) */
extern POINT256 boss_clip_max;		/* ボス移動範囲(最大値) */


//typedef struct		/* enemyの一般形 */
//{
//	ENEMY_BASE b;	/* 共用 */
//	int state;
//} ENE MY_DATA;


#if 1
enum
{
	BULLET_RES_00_,
	BULLET_RES_01_,
	BULLET_RES_02_,
	BULLET_RES_03_,
	BULLET_RES_04_,
	BULLET_RES_05_,
	BULLET_RES_06_,
	BULLET_RES_07_,
	BULLET_RES_MAX,
};

/*
	ToDo:
	パフォーマンス(実行速度)的観点から、ここは全部一つに共通化すべき。
	特殊な弾幕は、弾自身がコールバックで弾生成すべき。
	(例えば共通規格move_bullet内で変身するような弾は、共通規格move_bullet内で共通規格のbullet_createをコールする(コールバック)。)
	(その辺の規格を構造体で決めてシンプルにする予定)
*/
/* 弾の生成規格(案) */
typedef struct _bullet_create_status
{
	SPRITE *src;		// 発弾座標
	int speed256;		// 速度
	int angle512;		// 角度
	int create_count;	// 0 == 初回生成
//
	int argument_00;	// 引数
	int argument_01;	// 引数
	int argument_02;	// 引数
	int argument_03;	// 引数
//
//	int create_type;	// 生成種類
//	_bullet_create_status *bbb;
//	int dummy_08;
//	int dummy_09;
//
	void (*callback_bullet_create0)(struct _bullet_create_status *b);	// NULLで終了(初回==弾幕生成)
	void (*callback_bullet_create1)(struct _bullet_create_status *b);	// NULLで終了(変身1回目)
	void (*callback_bullet_create2)(struct _bullet_create_status *b);	// NULLで終了(変身2回目)
	void (*callback_bullet_create3)(struct _bullet_create_status *b);	// NULLで終了(変身3回目)
} BULLET_STATUS;
extern BULLET_STATUS bullet_resource[BULLET_RES_MAX];
//extern void bullet_create(BULLET_STATUS *b);/* 弾の生成 */
extern void bullet_create_resource(int type);/* 弾の生成 */


/* 弾の生成 */

extern void bullet_create_aka_maru_jikinerai(		SPRITE *src, int speed256);
extern void bullet_create_jyuryoku_dan000(			SPRITE *src, int speed256, int angle512, int delta256, int bullet_obj_type );
extern void bullet_create_offset_dan_type000(		SPRITE *src, int speed256, int angle512, int bullet_obj_type );/*, int x_offset256, int y_offset256*/

extern void bullet_create_enemy_homing( 			SPRITE *src);

extern void bullet_create_maru8_frame(				SPRITE *src, int speed256, int angle512, int set_frame);
extern void bullet_create_hazumi_dan(				SPRITE *src, int speed256, int angle512, int delta256, int bound_counts);
extern void bullet_create_tomari_dan(				SPRITE *src, int speed256, int angle512, int delta256);
extern void bullet_create_tomari2_dan(				SPRITE *src, int speed256, int angle512, int a256, int next_angle512);

extern void bullet_create_rot4096_dan(				SPRITE *src, int speed256, int angle512, int d_angle4096);
extern void bullet_create_sakuya_knife( 			SPRITE *src, int speed256, int angle512, int p_angle512);/*anim*/
extern void bullet_create_sakuya_no_rot_knife(		SPRITE *src, int speed256, int angle512, int gra256);
extern void bullet_create_sakuya_kurukuru_knife(	void);//SPRITE *src, int speed256, int angle512, int height);
extern void bullet_create_sakuya_ryoute_knife(		SPRITE *src);


//extern void bullet_create_n_way_dan_type( 			SPRITE *src, int speed256, int angle512, int bullet_obj_type, int n_way);/* char *filename, int frame,*/
extern void bullet_regist_basic(void
	/*	SPRITE *src,*/
//	int speed256,
//	int angle512,
//	int sa_angle512,
//	int bullet_obj_type,
//	int n_way
	);/* char *filename, int frame,*/
#define BULLET_REGIST_speed256							user_data10 	/* aa */
#define BULLET_REGIST_angle512							user_data11 	/* bb */
#define BULLET_REGIST_div_angle512						user_data12 	/* cc */
#define BULLET_REGIST_bullet_obj_type					user_data13 	/* dd */
#define BULLET_REGIST_n_way 							user_data14 	/* ee */
//
#define BULLET_REGIST_sakuya_kurukurku_knife_height 	user_data15 	/* ff */

#endif

//extern void bakuhatsu_add_type(int x256, int y256, /*dou ble int delay_wait,*/ int type);
extern void bakuhatsu_add_type_ddd(SPRITE *src/*int x256, int y256*/, /*dou ble int delay_wait,*/ int type);
extern void bakuhatsu_add_rect(SPRITE *src);
extern void bakuhatsu_add_circle(SPRITE *src, int mode);
extern void bakuhatsu_add_zako04(SPRITE *src);

extern void callback_hit_zako(SPRITE *src, SPRITE *t);

/* ボス移動処理の共通ルーチン */

extern void boss_move_clip_rect(SPRITE *src);

// 共通形態
extern void common_99_keitai(SPRITE *src);/* 撃破後に画面外にボスが逃げる */

/* thegame.c のみで ザコ／ボスで宣言が必要なもの(グローバル)は、thegame.c へ移動した。 */

/* ボスで宣言が必要なもの(グローバル) */

extern void enemy_set_random_seed(void/*int set_seed*/);
//extern int en emy_get_random_item(void);
extern void lose_random_item(SPRITE *src);

#include "danmaku.h"

#endif /* _ENEMY_H_ */

#ifndef _SPELL_CARD_H_
#define _SPELL_CARD_H_
//	SPELL_CARD_00_OFF = 0,
enum
{
// 咲夜 6面
	SPELL_CARD_00_sakuya_000 = 0,	/* "形態変更" */
	SPELL_CARD_01_sakuya_aaa,		/* 第一形態: 左右に動いて小弾撃ち */
	SPELL_CARD_02_sakuya_bbb,		/* 第二形態: 全方位、豆まき */
	SPELL_CARD_03_sakuya_ccc,		/* 第三形態: 垂直ナイフが落ちてくるよ */
	SPELL_CARD_04_sakuya_ddd,		/* 第四形態: 魔方陣生成 */
	SPELL_CARD_05_sakuya_eee,		/* 第五形態: (黄色マスカット弾) */
//	SPELL_CARD_06_sakuya_fff,		/* 第六形態: (黄色マスカット弾) */
//	SPELL_CARD_07_sakuya_ggg,		/* 第七形態: (分散魔方陣)追加計画中 */
//	SPELL_CARD_08_sakuya_hhh,		/* 第八形態: (時止めナイフ)追加計画中 */
	SPELL_CARD_09_sakuya_iii,		/* 第九形態: 最終形態(その1) */
	SPELL_CARD_10_sakuya_jjj,		/* 第10形態: 最終形態(その2) */
	SPELL_CARD_11_sakuya_kkk,		/* 第11形態: 最終形態(その3) */
// パチェ 5面
	SPELL_CARD_00_pache_000,
	SPELL_CARD_11_pache_bbb,
	SPELL_CARD_12_pache_ccc,
	SPELL_CARD_13_pache_ddd,
	SPELL_CARD_14_pache_eee,
	SPELL_CARD_15_pache_fff,
	SPELL_CARD_16_pache_ggg,
	SPELL_CARD_17_pache_hhh,
//	SPELL_CARD_18_pache_iii,
//	SPELL_CARD_19_pache_jjj,
// 文 4面
	SPELL_CARD_00_aya_000,
	SPELL_CARD_11_aya_bbb,
	SPELL_CARD_12_aya_ccc,
	SPELL_CARD_13_aya_ddd,
	SPELL_CARD_14_aya_eee,
	SPELL_CARD_15_aya_fff,
	SPELL_CARD_16_aya_ggg,
	SPELL_CARD_17_aya_hhh,
	SPELL_CARD_18_aya_iii,
	SPELL_CARD_19_aya_jjj,
// 輝夜 3面
	SPELL_CARD_00_kaguya_000,
	SPELL_CARD_11_kaguya_bbb,
	SPELL_CARD_12_kaguya_ccc,
	SPELL_CARD_13_kaguya_ddd,
	SPELL_CARD_14_kaguya_eee,
	SPELL_CARD_15_kaguya_fff,
	SPELL_CARD_16_kaguya_ggg,
	SPELL_CARD_17_kaguya_hhh,
// 未定 2面
	SPELL_CARD_00_mima_000,
	SPELL_CARD_11_mima_bbb,
	SPELL_CARD_12_mima_ccc,
	SPELL_CARD_13_mima_ddd,
	SPELL_CARD_14_mima_eee,
	SPELL_CARD_15_mima_fff,
	SPELL_CARD_16_mima_ggg,
	SPELL_CARD_17_mima_hhh,
// アリス 1面
	SPELL_CARD_00_alice_000,
	SPELL_CARD_11_alice_bbb,
	SPELL_CARD_12_alice_ccc,
	SPELL_CARD_13_alice_ddd,
	SPELL_CARD_14_alice_eee,
//	SPELL_CARD_15_alice_fff,
//	SPELL_CARD_16_alice_ggg,
//	SPELL_CARD_17_alice_hhh,
//	SPELL_CARD_18_alice_iii,
//	SPELL_CARD_19_alice_jjj,
//
	SPELL_CARD_MAX	/* 最大数 */
};

//------------ スペカ関連

extern void spell_card_generator222(SPRITE *src);
extern void create_spell_card(SPRITE *src, int spell_card_type);

#if (0==USE_BOSS_COMMON_MALLOC)
extern void spell_card_boss_init_regist(SPRITE *src);
#else
extern void spell_card_boss_init_regist_void(void/*SPRITE *src*/);
#endif

extern void regist_spell_card222(SPRITE *src);

extern int spell_card_mode; 			/* スペカモード */
extern int spell_card_limit_health; 	/* 規定値以下になればスペカモード解除 */
extern int spell_card_boss_state;		/* 負値になればボススペカモードに入らない */
extern int spell_card_boss_timer;		/* [共用]制限時間 */

extern int spell_card_number;			/* [共用]スペカ番号 */
extern int spell_card_max;				/* [共用]スペカ番号最大限界値 */

/* 出現時x座標 */
#define BOSS_XP256	(t256(GAME_WIDTH/2)-(t256(32/2)))/*sakuya->w128*/


//------------ "回"みたいなマークのエフェクト

/*static*/extern  void boss_effect(SPRITE *src);
/*static*/extern  void boss_effect_init(void);
/*static*/extern  void boss_effect_term(void);


//	ザコ登録の共通ルーチン
//extern void add_zako_common(STAGE_DATA *l, SPRITE *src);

#endif /* _SPELL_CARD_H_ */
