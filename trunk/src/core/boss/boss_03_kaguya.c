
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
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
	#define boss_time_out		user_data03 	/* 制限時間 */
	#define boss_base_state777	user_data03 	/* 制限時間(boss_time_outと同じ) */
#endif


/*---------------------------------------------------------
	ボス行動、第 4形態
	斜め移動
---------------------------------------------------------*/

global void kaguya_04_keitai(SPRITE *src)
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
	if (SPELL_00==src->boss_base_spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		/* 気分で目標を決める。 */
		kaguya_target_x256 = ((ra_nd()&0xffff))+(t256(48));
		/* 移動方向を決める */
		src->boss_base_state777++;
		src->boss_base_state777 &= (4-1);
		#if (1)
		if ((cg_game_difficulty) >=  src->boss_base_state777 )
		{
			spell_card_get_spell_number(src);
		}
		#endif
	}
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	bullet_angle_all_gamen_gai_nara_kesu();/* 角度弾の喰み出しチェックを行う(毎フレーム行う必要はない) */
}


/*---------------------------------------------------------
	ボス行動、第 1形態
	微上下移動
---------------------------------------------------------*/

global void kaguya_01_keitai(SPRITE *src)
{
	static int aaa_wait;
	aaa_wait += (10);
	mask1024(aaa_wait);
	src->cy256 = (t256(25)+(sin1024((aaa_wait))<<2));
//
	/* 弾幕を撃ってない場合に追加 */
	if (SPELL_00==src->boss_base_spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		/* 移動方向を決める */
		src->boss_base_state777++;
		src->boss_base_state777 &= (4-1);
		#if (1)
		if ((cg_game_difficulty) >= src->boss_base_state777 )
		{
			spell_card_get_spell_number(src);
		}
		#endif
	}
	bullet_angle_all_gamen_gai_nara_kesu();/* 角度弾の喰み出しチェックを行う(毎フレーム行う必要はない) */
}
