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

typedef struct
{
//------------ スペカ関連

//	int boss_health;		/* 共用 */	/* (下位)体力 */
//	int /*boss_*/score; 	/* 共用 */
	//int boss_life;			/* 共用 */	// (上位)体力
//------------ 弾幕関連
//	int dummy_tmp_angleCCW512;	/* 保持角度 */
	int danmaku_type;
	int danmaku_time_out;
	int danmaku_test;
	int dummy_aaaa;/* 予約(多分タイマーで使う) */
} BOSS_BASE;

//typedef struct		/* enemyの一般形 */
//{
//	ENEMY_BASE b;	/* 共用 */
//	int state;
//} ENE MY_DATA;


#if 1
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
extern void bullet_create(BULLET_STATUS *b);/* 弾の生成 */

// [r13]
//void bullet_create_aka_maru_n_way(SPRITE *src, int speed256, int angle512, int angle2_512, int bu_type, int way)
/* 弾の生成 */

extern void bullet_create_aka_maru_jikinerai(		SPRITE *src, int speed256);
extern void bullet_create_jyuryoku_dan000( 			SPRITE *src, int speed256, int angle512, int delta256, int bullet_obj_type );

//廃止extern void enemy_laser_create(				SPRITE *src, int speed256);/*廃止(bullet_create_hari_dan180()にversion up)*/
//廃止extern void enemy_laser_create2(				SPRITE *src, int speed256, int angle512);/*廃止(bullet_create_hari_dan180()にversion up)*/
//廃止extern void bullet_create_hari_dan180(		SPRITE *src, int speed256, int angle512, int x_offset256, int y_offset256 );/*廃止(bullet_create_offset_dan_type()にversion up)*/
extern void bullet_create_offset_dan_type000(		SPRITE *src, int speed256, int angle512/*, int x_offset256, int y_offset256*/, int bullet_obj_type );

extern void bullet_create_enemy_homing( 			SPRITE *src);
//static void enemy_homing_update(					SPRITE *src);

extern void bullet_create_maru8_frame(				SPRITE *src, int speed256, int angle512, int set_frame);
extern void bullet_create_momiji_dan(				SPRITE *src, int speed256, int angle512);
//static void bullet_create_momiji_seed(			SPRITE *src, int speed256, int angle512, dou ble a);
extern void bullet_create_oodama00(					SPRITE *src, int speed256, int angle512, int ransu512, int add_speed256/*, int xoffs256, int yoffs256*/);
//tern void bullet_create_oodama1(					SPRITE *src, int xoffs256, int yoffs256, int speed256, int angle512, int ransu);
//tern void bullet_create_oodama2(					SPRITE *src, int speed256, int angle512, int a256);
//廃止extern void bullet_create_gg_dan( 			SPRITE *src, int speed256, int angle512, int state_hi, int state_lo);/*廃止(bullet_create_n_way_dan_type()を使う)*/
extern void bullet_create_hazumi_dan(				SPRITE *src, int speed256, int angle512, int delta256, int bound_counts);
extern void bullet_create_tomari_dan(				SPRITE *src, int speed256, int angle512, int delta256);
//extern id enemy_stop_bullet2_create(				SPRITE *src, int speed256, int angle512, int a256, dou ble next_angle);
extern void bullet_create_tomari2_dan(				SPRITE *src, int speed256, int angle512, int a256, int next_angle512);

extern void bullet_create_rot4096_dan(				SPRITE *src, int speed256, int angle512, int d_angle4096);
extern void bullet_create_sakuya_knife( 			SPRITE *src, int speed256, int angle512, int p_angle512/*anim*/);
extern void bullet_create_sakuya_no_rot_knife(		SPRITE *src, int speed256, int angle512, int gra256);
extern void bullet_create_sakuya_follow_knife1( 	SPRITE *src, int speed256, int angle512, int height);
//extern void bullet_create_sakuya_follow_knife2(	SPRITE *src, int speed256, int angle512, int height); /*dou ble x, dou ble y,*/
extern void bullet_create_sakuya_even_knife(		SPRITE *src, int speed256, int length, int r_or_l);


//extern void bullet_create_n_way_dan_type( 		SPRITE *src, int speed256, int angle512, int bullet_obj_type, int n_way);/* char *filename, int frame,*/
extern void bullet_create_n_way_dan_sa_type(		SPRITE *src, int speed256, int angle512, int sa_angle512, int bullet_obj_type, int n_way);/* char *filename, int frame,*/

#endif

//extern void bakuhatsu_add_type(int x256, int y256, /*dou ble int delay_wait,*/ int type);
extern void bakuhatsu_add_type_ddd(SPRITE *src/*int x256, int y256*/, /*dou ble int delay_wait,*/ int type);
extern void bakuhatsu_add_rect(SPRITE *src);
extern void bakuhatsu_add_circle(SPRITE *src, int mode);
extern void bakuhatsu_add_zako04(SPRITE *src);

extern void callback_hit_zako(SPRITE *src, SPRITE *t);

/* ボス移動処理の共通ルーチン */

extern void boss_move96(SPRITE *src);


/* thegame.c のみで ザコ／ボスで宣言が必要なもの(グローバル)は、thegame.c へ移動した。 */

/* ボスで宣言が必要なもの(グローバル) */

extern void enemy_set_random_seed(void/*int set_seed*/);
extern int enemy_get_random_item(void);

#include "danmaku.h"

#endif /* _ENEMY_H_ */

#ifndef _SPELL_CARD_H_
#define _SPELL_CARD_H_

enum
{

/*0*/	SAKUYA_01_KEITAI = 0,	/* 第一形態: 左右に動いて小弾撃ち */
/*1*/	SAKUYA_02_KEITAI,		/* 第二形態: 全方位、豆まき */
/*2*/	SAKUYA_03_KEITAI,		/* 第三形態: 垂直ナイフが落ちてくるよ */
/*3*/
		SAKUYA_04_KEITAI_A, 	/* 第四形態: 魔方陣生成 */
		SAKUYA_05_KEITAI_B, 	/* 第五形態: (黄色マスカット弾) */
		SAKUYA_06_KEITAI_C, 	/* 第六形態: (黄色マスカット弾) */
//		SAKUYA_07_KEITAI,		/* 第七形態: (分散魔方陣)追加計画中 */
//		SAKUYA_08_KEITAI,		/* 第八形態: (時止めナイフ)追加計画中 */
//	最終形態
/*4*/	SAKUYA_09_KEITAI,		/* 第九形態: 最終形態(その1) */
/*5*/	SAKUYA_10_KEITAI,		/* 第10形態: 最終形態(その2) */



	#if 0
//	SPELL_CARD_00_OFF = 0,
// 咲夜 6面
	SPELL_CARD_01_sakuya_aaa,
	SPELL_CARD_02_sakuya_bbb,
	SPELL_CARD_03_sakuya_ccc,
	SPELL_CARD_04_sakuya_ddd,
	SPELL_CARD_05_sakuya_eee,
	SPELL_CARD_06_sakuya_fff,
	SPELL_CARD_07_sakuya_ggg,
// パチェ 5面
	SPELL_CARD_08_pache_aaa,
	SPELL_CARD_09_pache_bbb,
	SPELL_CARD_0a_pache_ccc,
	SPELL_CARD_0b_pache_ddd,
	SPELL_CARD_0c_pache_eee,
	SPELL_CARD_0d_pache_fff,
	SPELL_CARD_0e_pache_ggg,
	SPELL_CARD_0f_pache_hhh,
// 輝夜 4面
	SPELL_CARD_10_kaguya_aaa,
	SPELL_CARD_11_kaguya_bbb,
	SPELL_CARD_12_kaguya_ccc,
	SPELL_CARD_13_kaguya_ddd,
	SPELL_CARD_14_kaguya_eee,
	SPELL_CARD_15_kaguya_fff,
	SPELL_CARD_16_kaguya_ggg,
	SPELL_CARD_17_kaguya_hhh,
// 未定 3面
	SPELL_CARD_18_mitei_aaa,
	SPELL_CARD_19_mitei_bbb,
	SPELL_CARD_1a_mitei_ccc,
	SPELL_CARD_1b_mitei_ddd,
	SPELL_CARD_1c_mitei_eee,
	SPELL_CARD_1d_mitei_fff,
	SPELL_CARD_1e_mitei_ggg,
	SPELL_CARD_1f_mitei_hhh,
// 文 2面
	SPELL_CARD_20_aya_aaa,
	SPELL_CARD_21_aya_bbb,
	SPELL_CARD_22_aya_ccc,
	SPELL_CARD_23_aya_ddd,
	SPELL_CARD_24_aya_eee,
	SPELL_CARD_25_aya_fff,
	SPELL_CARD_26_aya_ggg,
	SPELL_CARD_27_aya_hhh,
	#endif
// アリス 1面
	SPELL_CARD_28_alice_aaa,
	SPELL_CARD_29_alice_bbb,
	SPELL_CARD_2a_alice_ccc,
	SPELL_CARD_2b_alice_ddd,
	SPELL_CARD_2c_alice_eee,
	SPELL_CARD_2d_alice_fff,
	SPELL_CARD_2e_alice_ggg,
	SPELL_CARD_2f_alice_hhh,
//
	SPELL_CARD_MAX	/* 最大数 */
};

extern void create_spell_card(SPRITE *src, int spell_card_type);
extern void create_spell_card_init_dummy(void);
extern void draw_spell_card_name(void);
//------------ スペカ関連

extern int spell_card_mode; 			/* スペカモード */
extern int spell_card_limit_health; 	/* 規定値以下になればスペカモード解除 */
extern int spell_card_boss_state;		/* 負値になればボススペカモードに入らない */
extern int spell_card_boss_timer; 		/* 共用 */	// 制限時間

extern int spell_card_number; 			/* 共用 */	// スペカ番号


#endif /* _SPELL_CARD_H_ */
