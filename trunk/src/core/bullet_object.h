#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "game_main.h"

#include "bonus.h"
#include "load_stage.h"

enum
{
	EXPLOSION_MINI00=0,/*小爆発*/
	EXPLOSION_MINI01,
	EXPLOSION_MINI02,
	EXPLOSION_MINI03,
//
	EXPLOSION_ZAKO04,/*ザコ消滅爆発*/
	EXPLOSION_ZAKO05,
	EXPLOSION_ZAKO06,
	EXPLOSION_ZAKO07,
//
	EXPLOSION_FIRE08,/*火炎爆発*/
	EXPLOSION_FIRE09,
	EXPLOSION_FIRE10,
	EXPLOSION_FIRE11,
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

typedef struct
{
	int health; 		/* 共用 */
	int score;			/* 共用 */
} ENEMY_BASE;

typedef struct
{
	int boss_health;		/* 共用 */	/* (下位)体力 */
	int /*boss_*/score; 	/* 共用 */
	int boss_timer; 		/* 共用 */	// 制限時間
	int boss_life;			/* 共用 */	// (上位)体力
//------------ 弾幕関連
	int tmp_angleCCW512;	/* 保持角度 */
	int danmaku_type;
	int danmaku_time_out;
	int danmaku_test;
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
	SPRITE *s;			// 発弾座標
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
//void bullet_create_aka_maru_n_way(SPRITE *s, int speed256, int angle512, int angle2_512, int bu_type, int way)
/* 弾の生成 */

extern void bullet_create_aka_maru_jikinerai(		SPRITE *s, int speed256);
extern void bullet_create_jyuryoku_dan( 			SPRITE *s, int speed256, int angle512, int delta256);

//廃止extern void enemy_laser_create(				SPRITE *s, int speed256);/*廃止(bullet_create_hari_dan180()にversion up)*/
//廃止extern void enemy_laser_create2(				SPRITE *s, int speed256, int angle512);/*廃止(bullet_create_hari_dan180()にversion up)*/
//廃止extern void bullet_create_hari_dan180(		SPRITE *s, int speed256, int angle512, int x_offset256, int y_offset256 );/*廃止(bullet_create_offset_dan_type()にversion up)*/
extern void bullet_create_offset_dan_type(			SPRITE *s, int speed256, int angle512, int x_offset256, int y_offset256, int bullet_obj_type );

extern void bullet_create_enemy_homing( 			SPRITE *s);
//static void enemy_homing_update(					SPRITE *s);

extern void bullet_create_maru8_frame(				SPRITE *s, int speed256, int angle512, int set_frame);
extern void bullet_create_momiji_dan(				SPRITE *s, int speed256, int angle512);
//static void bullet_create_momiji_seed(			SPRITE *s, int speed256, int angle512, dou ble a);
extern void bullet_create_oodama0(					SPRITE *s, int speed256, int angle512, int ransu512, int add_speed256, int xoffs256, int yoffs256);
//tern void bullet_create_oodama1(					SPRITE *s, int xoffs256, int yoffs256, int speed256, int angle512, int ransu);
//tern void bullet_create_oodama2(					SPRITE *s, int speed256, int angle512, int a256);
//廃止extern void bullet_create_gg_dan( 			SPRITE *s, int speed256, int angle512, int state_hi, int state_lo);/*廃止(bullet_create_n_way_dan_type()を使う)*/
extern void bullet_create_hazumi_dan(				SPRITE *s, int speed256, int angle512, int delta256, int bound_counts);
extern void bullet_create_tomari_dan(				SPRITE *s, int speed256, int angle512, int delta256);
//extern id enemy_stop_bullet2_create(				SPRITE *s, int speed256, int angle512, int a256, dou ble next_angle);
extern void bullet_create_tomari2_dan(				SPRITE *s, int speed256, int angle512, int a256, int next_angle512);

extern void bullet_create_rot4096_dan(				SPRITE *s, int speed256, int angle512, int d_angle4096);
extern void bullet_create_sakuya_knife( 			SPRITE *s, int speed256, int angle512, int p_angle512/*anim*/);
extern void bullet_create_sakuya_no_rot_knife(		SPRITE *s, int speed256, int angle512, int gra256);
extern void bullet_create_sakuya_follow_knife1( 	SPRITE *s, int speed256, int angle512, int height);
//extern void bullet_create_sakuya_follow_knife2(	SPRITE *s, int speed256, int angle512, int height); /*dou ble x, dou ble y,*/
extern void bullet_create_sakuya_even_knife(		SPRITE *s, int speed256, int length, int r_or_l);


//extern void bullet_create_n_way_dan_type( 		SPRITE *s, int speed256, int angle512, int bullet_obj_type, int n_way);/* char *filename, int frame,*/
extern void bullet_create_n_way_dan_sa_type(		SPRITE *s, int speed256, int angle512, int sa_angle512, int bullet_obj_type, int n_way);/* char *filename, int frame,*/

#endif

extern void explosion_add_type(int x256, int y256, /*dou ble int delay_wait,*/ int type);
extern void explosion_add_rect(SPRITE *src);
extern void explosion_add_circle(SPRITE *src, int mode);


/* thegame.c のみで ザコ／ボスで宣言が必要なもの(グローバル)は、thegame.c へ移動した。 */

/* ボスで宣言が必要なもの(グローバル) */

extern void enemy_set_random_seed(void/*int set_seed*/);
extern int enemy_get_random_item(void);


/* ボスを倒したらすぐ呼ばれる(プレイヤーを無敵にする為)(フラグは仕様上時間待ちがある為、現在この用途には使えない) */
extern void player_set_destoroy_boss(void);

#include "danmaku.h"

#endif /* _ENEMY_H_ */
