
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	蓬莱山 輝夜
	Houraisan Kaguya.
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0/* めも：かぐやは、共通部分で使ってる。 */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif

/*---------------------------------------------------------
	微上下移動
---------------------------------------------------------*/

/*static*/ void kaguya_01_keitai(SPRITE *src)
{
	static int aaa_wait;
	aaa_wait += (10);
	aaa_wait &= (1024-1);
	src->y256 = (t256(25)+(sin1024((aaa_wait))<<2));
//
	/* 弾幕を撃ってない場合に追加 */
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		/* 移動方向を決める */
		src->boss_base_state001++;
		src->boss_base_state001 &= (4-1);
		#if (1)
		if (difficulty >= src->boss_base_state001 )
		{
			src->boss_base_danmaku_type 	= DANMAKU_18_kaguya01;		/*DANMAKU_01*/	/* 禊弾幕をセット */
			danmaku_set_time_out(src);		/* 弾幕の時間切れを設定 */
		}
		#endif
	}
	bullet_angle_all_hamidasi_check();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}

/*---------------------------------------------------------
	斜め移動
---------------------------------------------------------*/

/*static*/ void kaguya_04_keitai(SPRITE *src)
{
	static int kaguya_target_x256;
	static int aaa_wait;
	aaa_wait--;/*fps_factor*/
	if (aaa_wait <= 0)
	{
		aaa_wait = 8;
		/* 目標にちかづこうと努力。 */
		src->target_x256 += ((src->target_x256 < kaguya_target_x256) ? t256(8) : -t256(8) );
		src->target_x256 &= 0xffff;
		src->target_y256 = (t256(25)+(sin1024((src->target_x256>>4))<<6));
		src->vvv256 = t256(1.0);
	}
	/* 弾幕を撃ってない場合に追加 */
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		/* 気分で目標を決める。 */
		kaguya_target_x256 = ((ra_nd()&0xffff))+(t256(48));
		/* 移動方向を決める */
		src->boss_base_state001++;
		src->boss_base_state001 &= (4-1);
		#if (1)
		if (difficulty >=  src->boss_base_state001 )
		{
			src->boss_base_danmaku_type 	= DANMAKU_19_kaguya04; /*DANMAKU_01*/	/* 禊弾幕をセット */
			danmaku_set_time_out(src);		/* 弾幕の時間切れを設定 */
		}
		#endif
	}
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	bullet_angle_all_hamidasi_check();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}
