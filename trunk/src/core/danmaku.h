#ifndef _DANMAKU_H_
#define _DANMAKU_H_
	/*---------------------------------------------------------
		弾幕定義
	-------------------------------------------------------
	★弾幕はシステムでサポートする構想があります。(構想中)
	システムでサポートされると、どのボスが、どの弾幕でも撃てるようになります。
	---------------------------------------------------------*/
enum
{
	DANMAKU_00 = 0,
//	咲夜
	DANMAKU_01,
	DANMAKU_02,
	DANMAKU_03,
//	咲夜予約
	DANMAKU_04,
	DANMAKU_05,
	DANMAKU_06,
	DANMAKU_07,
//	アリス(仮)
	DANMAKU_08,
	DANMAKU_09,
	DANMAKU_10,
	DANMAKU_11,
//40[sec]
	DANMAKU_12,
	DANMAKU_13,
	DANMAKU_14,
	DANMAKU_15,
	DANMAKU_16,
//	アリス(仮)予約

};

//#define DANMAKU_01_SET_TIME	/*(150)*/
//#define DANMAKU_01_SET_TIME	/*(ra_nd()&0xff)*/
#define DANMAKU_01_SET_TIME 	(64+1)	/* 禊弾幕の発生時間 */
#define DANMAKU_02_SET_TIME 	(48+1)	/* 24弾幕の発生時間 */
#define DANMAKU_03_SET_TIME 	(64+1)	/* 11弾幕の発生時間 */

extern void tmp_angle_jikinerai512(SPRITE *p, SPRITE *s);
extern void danmaku_generator(SPRITE *s);
typedef struct /*_boss05_data*/
{
	BOSS_BASE boss_base;
//------------ 移動関連
	int dummy_state1; 							/* 行動 */
	int dummy_move_type;		/*	移動タイプ */	/* 形態 */
//
	int dummy_wait1;
	int dummy_wait2_256;
//
	int dummy_wait3;
//	int dummy_level;
	int dummy_move_angle512;
	int dummy_length_s_p256;	/* 咲夜とプレイヤーとの距離 */
	int dummy_aaaa;
} BOSS99_DATA;/*とりあえずダミー定義*/
#endif /* _DANMAKU_H_ */
