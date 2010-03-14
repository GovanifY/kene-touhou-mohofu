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
	/* [咲夜] */
	DANMAKU_01_sakuya,		/* 咲夜禊弾幕 / 紅5面中ボス 咲夜 「通常攻撃」 */
	DANMAKU_02_sakuya,		/* 咲夜24弾幕 (奇術「ミスディレクションもどき(1/2)」) */
	DANMAKU_03_sakuya,		/* 咲夜11弾幕 (奇術「ミスディレクションもどき(2/2)」) */
//	咲夜予約
	DANMAKU_04_sakuya,		/* 紅5面ボス 咲夜 「通常攻撃1(1/2)」にちょっとだけ似た弾幕(予定) */
	DANMAKU_05_sakuya,		/* 紅5面ボス 咲夜 「通常攻撃1/2(2/2)」にちょっとだけ似た弾幕(予定) */
	DANMAKU_06_sakuya,		/* 紅5面ボス 咲夜 幻象「ルナクロック(1/2)」にちょっとだけ似た弾幕(予定) */
	DANMAKU_07_sakuya,		/* 紅5面ボス 咲夜 「通常攻撃3(1/2)」にちょっとだけ似た弾幕(予定) */
	/* [アリス(仮)] */
	DANMAKU_08_rumia,		/* 紅1面ボス ルーミア 夜符「ナイトバード」にちょっとだけ似た弾幕(予定) */
	DANMAKU_09_alice,		/* アリスっぽい？テキトー弾幕 */
	DANMAKU_0a_houka_kenran,/* 紅美鈴 華符「芳華絢爛」にちょっとだけ似た弾幕(予定) */
	DANMAKU_0b_alice_doll,	/* アリス人形弾幕 */
//40[sec]
	DANMAKU_0c_hana_test,	/* 魅魔、花てすと弾幕 */
	DANMAKU_0d_test,
	DANMAKU_0e_test,
	DANMAKU_0f_test,
	/* [チルノ(仮)] */
	DANMAKU_10_cirno,			/* チルノ禊弾幕 */
	DANMAKU_11_tengu,/*???*/
	DANMAKU_12_aya_merin_test,	/* 文に美鈴っぽい弾幕撃たせてみるテスト */
	DANMAKU_13,
//
	DANMAKU_14,
	DANMAKU_15,
	DANMAKU_16,
	DANMAKU_17,
//
	DANMAKU_18,
	DANMAKU_19,
	DANMAKU_1a,
	DANMAKU_1b,
//
	DANMAKU_1c,
	DANMAKU_1d,
	DANMAKU_1e,
	DANMAKU_1f,
//
	DANMAKU_MAX,
};

//#define DANMAKU_01_SET_TIME	/*(150)*/
//#define DANMAKU_01_SET_TIME	/*(ra_nd()&0xff)*/
#define DANMAKU_01_SET_TIME 	(64+1)	/* 禊弾幕の発生時間 */
#define DANMAKU_02_SET_TIME 	(48+1)	/* 24弾幕の発生時間 */
#define DANMAKU_03_SET_TIME 	(64+1)	/* 11弾幕の発生時間 */

extern void tmp_angle_jikinerai512(SPRITE *p, SPRITE *sss);
extern void danmaku_generator(SPRITE *src);



#endif /* _DANMAKU_H_ */


#ifndef _BOSS_H_
#define _BOSS_H_
enum
{
	DESTROY_CHECK_00_WIN_BOSS = 0,
	DESTROY_CHECK_01_IS_TIME_OUT,
};
/*static*/extern  void boss_destroy_check_type(SPRITE *src/*敵自体*/, int check_type);/*	★ 攻撃の場合の死亡判定 	★ 時間切れの場合の死亡判定 */
/*static*/extern  void callback_hit_boss_dummy(SPRITE *src, SPRITE *tama /*, int angle*/);
/*static*/extern  void callback_hit_boss(SPRITE *src, SPRITE *tama /*, int angle*/);

/*static*/extern  void common_boss_put_items(SPRITE *src);

/*static*/extern  unsigned int common_boss_flags;

/*static*/extern  void lose_boss(SPRITE *src);
#endif /* _BOSS_H_ */
