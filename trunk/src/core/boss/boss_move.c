
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
	#define target_x256 			user_data00 	/* 目標x座標 */
	#define target_y256 			user_data01 	/* 目標y座標 */
	#define toutatu_wariai256		user_data02 	/* 目標座標への到達割合 */
	#define kougeki_anime_count 	user_data03 	/* 攻撃アニメーション用カウンタ */
	#define boss_time_out			user_data04 	/* 制限時間 */
	#define boss_base_state777		user_data04 	/* 制限時間(boss_time_outと同じ) */
//
	#define boss_spell_timer		user_data05 	/* スペル時間 */
#endif

/*---------------------------------------------------------
	[ボス共通スペカ移動]。
---------------------------------------------------------*/

/*---------------------------------------------------------
	弾幕生成の間隔と移動時間の間隔が同期。
	xyランダム差分移動。
---------------------------------------------------------*/
#if 0
global/*static*/ void core_boss_move_05_xy_douki_differential64(SPRITE *src)
{
	/* 弾幕を撃ってない場合に追加 */
	if (SPELL_00==spell_card.spell_type)		/* 弾幕生成終了なら弾幕生成 */
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
		src->toutatu_wariai256 = t256(1.0);
	}
	#if 1
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
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
	if (SPELL_00==spell_card.spell_type)		/* 弾幕生成終了なら弾幕生成 */
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
		src->toutatu_wariai256 = t256(1.0);
	}
	#if 1
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
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
	if (SPELL_00==spell_card.spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		#if 1
		const unsigned short ra_nd16 =ra_nd();
		src->target_x256 = ((ra_nd16/*&((256*256)-1)*/));	/* 0(???) =:= t256(((480-128-256)/2)-50)) */
		#endif
		spell_card_get_spell_number(src);	/* スペカシステムから、現在の形態で撃つ弾幕番号を手に入れ、標準のboss_base_spell_time_out時間を設定。 */
		src->toutatu_wariai256 = t256(1.0);
	}
	#if 1
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
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
	if (SPELL_00==spell_card.spell_type)		/* 弾幕生成終了なら弾幕生成 */
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
			src->toutatu_wariai256 = t256(1.0);
		}
	}
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	動かないでスペカ撃つ。
---------------------------------------------------------*/

global void boss_move_01_not_move(SPRITE *src)
{
	if (SPELL_00==spell_card.spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		spell_card_get_spell_number(src);	/* スペカシステムから、現在の形態で撃つ弾幕番号を手に入れ、標準のboss_base_spell_time_out時間を設定。 */
		src->toutatu_wariai256 = t256(1.0);
	}
//
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
---------------------------------------------------------*/
/*---------------------------------------------------------
---------------------------------------------------------*/
/*---------------------------------------------------------
---------------------------------------------------------*/


/*---------------------------------------------------------
	[ボス共通スペカ行動]。
---------------------------------------------------------*/




/* ボス移動処理の共通ルーチン */

//extern void bo ss_move_vx_vy(SPRITE *src);
//extern void bo ss_clip_rect(SPRITE *src);

/*---------------------------------------------------------
	ボス移動処理の共通ルーチン
	-------------------------------------------------------
★ 移動範囲の登録、機能：
	あらかじめボス移動範囲を登録しておく。
★ 範囲内ならば移動、機能：
	移動範囲内ならば、ボスが移動する。
	移動範囲外ならば、ボスは移動しない。
★ 移動判定、機能：
	ボスが移動しない場合は、「移動できなかったフラグ」がＯＮになる。
---------------------------------------------------------*/

/*---------------------------------------------------------
	ボス誘導比率計算。
---------------------------------------------------------*/
/*extern*/global POINT256 boss_clip_min;		/* ボス移動範囲(最小値) */
/*extern*/global POINT256 boss_clip_max;		/* ボス移動範囲(最大値) */

/*extern*/global int boss_hamidasi; 			/* 「移動できなかったフラグ」(使用前に手動でOFF==0にしとく) */

/*global*/static/*static*/ void boss_move_vx_vy_animation(SPRITE *src)
{
	src->cx256 += (src->vx256);
	src->cy256 += (src->vy256);
		 if (src->cx256 < boss_clip_min.x256 )	{	src->cx256 = boss_clip_min.x256;	boss_hamidasi=1;	}
	else if (src->cx256 > boss_clip_max.x256 )	{	src->cx256 = boss_clip_max.x256;	boss_hamidasi=1;	}
//
		 if (src->cy256 < boss_clip_min.y256 )	{	src->cy256 = boss_clip_min.y256;	boss_hamidasi=1;	}
	else if (src->cy256 > boss_clip_max.y256 )	{	src->cy256 = boss_clip_max.y256;	boss_hamidasi=1;	}
//
	if (0 < src->kougeki_anime_count)	/* 攻撃アニメーション */	/* 攻撃アニメーション用カウンタ / 0以下なら移動アニメーション */
	{
		src->kougeki_anime_count--;
				if ((32)>src->kougeki_anime_count)	{	src->type = TEKI_09_BOSS32; }	/*src->an im_frame = 0x23;*/
		else	if ((40)>src->kougeki_anime_count)	{	src->type = TEKI_10_BOSS33; }	/*src->an im_frame = 0x22;*/
		else										{	src->type = TEKI_09_BOSS32; }	/*src->an im_frame = 0x21;*/
	}
	else	/* 移動アニメーション */
	{
		int aaa;
		aaa = (src->cx256> src->target_x256)?(TEKI_00_BOSS11/*0x00*/):(TEKI_04_BOSS21/*0x10*/); /* 左右 */

		#if 0
				if ( ( 16) > src->toutatu_wariai256)		{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else	if ( (200) > src->toutatu_wariai256)		{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else	if ( (224) > src->toutatu_wariai256)		{	aaa+=2;}	/*128.0==t256(0.5)*/
		else	if ( (240) > src->toutatu_wariai256)		{	aaa+=1;}	/*179.2==t256(0.7)*/
		else									{	aaa+=0;}
		#else
				if ( (256- 16) < src->toutatu_wariai256)	{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else	if ( (256-200) < src->toutatu_wariai256)	{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else	if ( (256-224) < src->toutatu_wariai256)	{	aaa+=2;}	/*128.0==t256(0.5)*/
		else	if ( (256-240) < src->toutatu_wariai256)	{	aaa+=1;}	/*179.2==t256(0.7)*/
		else									{	aaa+=0;}
		#endif
		src->type = aaa;
	}
}
/*---------------------------------------------------------
	ボス行動
---------------------------------------------------------*/

global void common_boss_move(SPRITE *src)
{
	check_regist_generate_spell_card(src);	/* スペカ登録可能なら登録 / スペカ生成 */
//
	boss_move_vx_vy_animation(src);
//
	move_boss_effect(src);	/* 回エフェクト */
	spell_generator(src);	/* 弾幕生成 */
}







