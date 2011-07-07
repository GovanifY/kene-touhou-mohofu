
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	アリス・マーガトロイド
	Alice Margatroid.
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0/*メモ*/
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define boss_time_out		user_data03 	/* 制限時間 */
	#define boss_base_state777	user_data03 	/* 制限時間(boss_time_outと同じ) */
#endif

/*---------------------------------------------------------
	弾幕生成の間隔と移動時間の間隔が同期。
	xyランダム差分移動。
---------------------------------------------------------*/
#if 0
global/*static*/ void core_boss_move_05_xy_douki_differential64(SPRITE *src)
{
	/* 弾幕を撃ってない場合に追加 */
	if (SPELL_00==src->boss_base_spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		#if 1
		/* 現在位置より左右 64dots の範囲でランダム移動。但し最低16dotsは動く。 */
		const unsigned short ra_nd16 = ra_nd();
		src->target_x256 = src->cx256;
		{	const unsigned short vector_value = ((ra_nd16&(((64)*256)-1))|(16*256));/* 移動量を決める。 但し最低16dotsは動く */
		//	if (ra_nd16&0x2000) /* 移動方向を決める */ /* 0x1fff == ((32)*256-1) */
		//	if (src->target_x256 < (ra_nd16+ra_nd16)) /* 移動方向を決める */
			if (src->target_x256 < (ra_nd16+(((GAME_WIDTH/2)-128)<<8))) /* 移動方向を決める */
					{	src->target_x256 += (vector_value); 	}
			else	{	src->target_x256 -= (vector_value); 	}
		}
		/* 現在位置より上下 8dots の範囲でランダム移動 */
		src->target_y256 = src->cy256;
		src->target_y256 += ((((ra_nd16))&((8+8)-1))<<8);
		src->target_y256 -= (t256(8));
		#endif
		spell_card_get_spell_number(src);	/* スペカシステムから、現在の形態で撃つ弾幕番号を手に入れ、標準のboss_base_spell_time_out時間を設定。 */
		src->vvv256 = t256(1.0);
	}
	#if 1
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	#endif
}
global void boss_move_05_xy_douki_differential64(SPRITE *src)
{
	core_boss_move_05_xy_douki_differential64(src);
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}
#endif
global/*static*/ void core_boss_move_04_xy_douki_differential32(SPRITE *src)
{
	/* 弾幕を撃ってない場合に追加 */
	if (SPELL_00==src->boss_base_spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		#if 1
		/* 現在位置より左右 32dots の範囲でランダム移動。但し最低16dotsは動く。 */
		const unsigned short ra_nd16 = ra_nd();
		src->target_x256 = src->cx256;
		{	const unsigned short vector_value = ((ra_nd16&(((32)*256)-1))|(16*256));/* 移動量を決める。 但し最低16dotsは動く */
		//	if (ra_nd16&0x2000) /* 移動方向を決める */ /* 0x1fff == ((32)*256-1) */
		//	if (src->target_x256 < (ra_nd16+ra_nd16)) /* 移動方向を決める */
			if (src->target_x256 < (ra_nd16+(((GAME_WIDTH/2)-128)<<8))) /* 移動方向を決める */
					{	src->target_x256 += (vector_value); 	}
			else	{	src->target_x256 -= (vector_value); 	}
		}
		/* 現在位置より上下 8dots の範囲でランダム移動 */
		src->target_y256 = src->cy256;
		src->target_y256 += ((((ra_nd16))&((8+8)-1))<<8);
		src->target_y256 -= (t256(8));
		#endif
		spell_card_get_spell_number(src);	/* スペカシステムから、現在の形態で撃つ弾幕番号を手に入れ、標準のboss_base_spell_time_out時間を設定。 */
		src->vvv256 = t256(1.0);
	}
	#if 1
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	#endif
}
global void boss_move_04_xy_douki_differential32(SPRITE *src)
{
	core_boss_move_04_xy_douki_differential32(src);
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	弾幕生成の間隔と移動時間の間隔が同期。
	xランダム移動。
	y移動しない。
---------------------------------------------------------*/

global void boss_move_03_x_douki(SPRITE *src)
{
	/* 弾幕を撃ってない場合に追加 */
	if (SPELL_00==src->boss_base_spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		#if 1
		const unsigned short ra_nd16 =ra_nd();
		src->target_x256 = ((ra_nd16/*&((256*256)-1)*/));	/* 0(???) =:= t256(((480-128-256)/2)-50)) */
		#endif
		spell_card_get_spell_number(src);	/* スペカシステムから、現在の形態で撃つ弾幕番号を手に入れ、標準のboss_base_spell_time_out時間を設定。 */
		src->vvv256 = t256(1.0);
	}
	#if 1
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	#endif
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	弾幕生成の間隔と移動時間の間隔が非同期。
	xランダム移動。
	yランダム移動。
---------------------------------------------------------*/

global void boss_move_02_xy_hidouki(SPRITE *src)
{
	if (SPELL_00==src->boss_base_spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		spell_card_get_spell_number(src);
	}
	/* 弾幕生成の間隔と移動時間の間隔が非同期。 */
	src->boss_time_out--;/*fps_factor*/
	{
		if (src->boss_time_out <= 0)
		{
			src->boss_time_out	= (300);	/* 移動時間、間隔 */
			int rand32 = ((ra_nd()));
			/* 移動座標を決める */
			src->target_x256 = ((rand32&0xff00)+(((rand32<<8)&0x3f00)));	/* (256+64) == 320 == (480-128-32) */
			src->target_y256 = (((rand32>>8)&0x3f00));	/* (64) */
			/* 攻撃アニメーション */
			src->vvv256 = t256(1.0);
		}
	}
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	動かないでスペカ撃つ。
---------------------------------------------------------*/

global void boss_move_01_not_move(SPRITE *src)
{
	if (SPELL_00==src->boss_base_spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		spell_card_get_spell_number(src);	/* スペカシステムから、現在の形態で撃つ弾幕番号を手に入れ、標準のboss_base_spell_time_out時間を設定。 */
		src->vvv256 = t256(1.0);
	}
//
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}
