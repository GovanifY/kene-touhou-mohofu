
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ボスの行動を定義する。
---------------------------------------------------------*/


/*---------------------------------------------------------
	[ボス共通カード移動]。
---------------------------------------------------------*/

/*---------------------------------------------------------
	カード生成の間隔と移動時間の間隔が同期。
	xyランダム差分移動。
---------------------------------------------------------*/
#if 0
global/*static*/ void core_boss_move_05_xy_douki_differential64(SPRITE *src)
{
	/* カードを撃ってない場合に追加 */
	if (SPELL_00==card.card_number) 	/* カード生成終了ならカード生成 */
	{
		#if 1
		/* 現在位置より左右 64dots の範囲でランダム移動。但し最低16dotsは動く。 */
		const unsigned short ra_nd16	= ra_nd();
		src->BOSS_DATA_00_target_x256	= src->cx256;
		{	const unsigned short vector_value = ((ra_nd16&(((64)*256)-1))|(16*256));/* 移動量を決める。 但し最低16dotsは動く */
		//	if (ra_nd16&0x2000) /* 移動方向を決める */ /* 0x1fff == ((32)*256-1) */
		//	if (src->BOSS_DATA_00_target_x256 < (ra_nd16+ra_nd16)) /* 移動方向を決める */
			if (src->BOSS_DATA_00_target_x256 < (ra_nd16+((GAME_X_OFFSET+(GAME_320_WIDTH/2)-128)<<8))) /* 移動方向を決める */
					{	src->BOSS_DATA_00_target_x256 += (vector_value);	}
			else	{	src->BOSS_DATA_00_target_x256 -= (vector_value);	}
		}
		/* 現在位置より上下 8dots の範囲でランダム移動 */
		src->BOSS_DATA_01_target_y256 = src->cy256;
		src->BOSS_DATA_01_target_y256 += ((((ra_nd16))&((8+8)-1))<<8);
		src->BOSS_DATA_01_target_y256 -= (t256(8));
		#endif
		card_maikai_init_and_get_spell_number(src); 		/* 現在の形態で撃てるカード番号をカードシステムに設定。 */
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* 攻撃アニメーション */
	}
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
}
global void boss_move_05_xy_douki_differential64(SPRITE *src)
{
	core_boss_move_05_xy_douki_differential64(src);
}
#endif
global/*static*/ void boss_move_04_xy_douki_differential32(SPRITE *src)
{
	/* カードを撃ってない場合に追加 */
	if (SPELL_00==card.card_number) 	/* カード生成終了ならカード生成 */
	{
		#if 1
		/* 現在位置より左右 32dots の範囲でランダム移動。但し最低16dotsは動く。 */
		const unsigned short ra_nd16	= ra_nd();
		src->BOSS_DATA_00_target_x256	= src->cx256;
		{	const unsigned short vector_value = ((ra_nd16&(((32)*256)-1))|(16*256));/* 移動量を決める。 但し最低16dotsは動く */
		//	if (ra_nd16&0x2000) /* 移動方向を決める */ /* 0x1fff == ((32)*256-1) */
		//	if (src->BOSS_DATA_00_target_x256 < (ra_nd16+ra_nd16)) /* 移動方向を決める */
			if (src->BOSS_DATA_00_target_x256 < (ra_nd16+((GAME_X_OFFSET+(GAME_320_WIDTH/2)-128)<<8))) /* 移動方向を決める */
					{	src->BOSS_DATA_00_target_x256 += (vector_value);	}
			else	{	src->BOSS_DATA_00_target_x256 -= (vector_value);	}
		}
		/* 現在位置より上下 8dots の範囲でランダム移動 */
		src->BOSS_DATA_01_target_y256 = src->cy256;
		src->BOSS_DATA_01_target_y256 += ((((ra_nd16))&((8+8)-1))<<8);
		src->BOSS_DATA_01_target_y256 -= (t256(8));
		#endif
		card_maikai_init_and_get_spell_number(src); 		/* 現在の形態で撃てるカード番号をカードシステムに設定。 */
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* 攻撃アニメーション */
	}
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
}


/*---------------------------------------------------------
	カード生成の間隔と移動時間の間隔が同期。
	xランダム移動。
	y移動しない。
---------------------------------------------------------*/

global void boss_move_03_x_douki(SPRITE *src)
{
	/* カードを撃ってない場合に追加 */
	if (SPELL_00==card.card_number) 	/* カード生成終了ならカード生成 */
	{
		#if 1
		const unsigned short ra_nd16	= ra_nd();
		src->BOSS_DATA_00_target_x256	= ((ra_nd16/*&((256*256)-1)*/));	/* 0(???) =:= t256(((480-128-256)/2)-50)) */
		#endif
		card_maikai_init_and_get_spell_number(src); 		/* 現在の形態で撃てるカード番号をカードシステムに設定。 */
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* 攻撃アニメーション */
	}
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
}


/*---------------------------------------------------------
	カード生成の間隔と移動時間の間隔が非同期。
	xランダム移動。
	yランダム移動。
---------------------------------------------------------*/

global void boss_move_02_xy_hidouki(SPRITE *src)
{
	if (SPELL_00==card.card_number) 	/* カード生成終了ならカード生成 */
	{
		card_maikai_init_and_get_spell_number(src);
	}
	/* カード生成の間隔と移動時間の間隔が非同期。 */
	src->BOSS_DATA_05_move_jyumyou--;/*fps_factor*/
	{
		if (src->BOSS_DATA_05_move_jyumyou <= 0)
		{
			src->BOSS_DATA_05_move_jyumyou	= (300);	/* 移動時間、間隔 */
			int rand32 = ((ra_nd()));
			/* 移動座標を決める */
			src->BOSS_DATA_00_target_x256 = ((rand32&0xff00)+(((rand32<<8)&0x3f00)));	/* (256+64) == 320 == (480-128-32) */
			src->BOSS_DATA_01_target_y256 = (((rand32>>8)&0x3f00)); 	/* (64) */
			src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* 攻撃アニメーション */
		}
	}
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
}


/*---------------------------------------------------------
	動かないでカードを撃つ。
---------------------------------------------------------*/

global void boss_move_01_not_move(SPRITE *src)
{
	if (SPELL_00==card.card_number) 	/* カード生成終了ならカード生成 */
	{
		card_maikai_init_and_get_spell_number(src); 		/* 現在の形態で撃てるカード番号をカードシステムに設定。 */
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* 攻撃アニメーション */
	}
}


/*---------------------------------------------------------
---------------------------------------------------------*/


/*---------------------------------------------------------
	[ボス共通カード行動]。
---------------------------------------------------------*/


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

static void boss_move_vx_vy_animation(SPRITE *src)
{
	src->cx256 += (src->vx256);
	src->cy256 += (src->vy256);
	#define USE_BOSS_HAMIDASI_CHECK (0)
	#if (1==USE_BOSS_HAMIDASI_CHECK)//1575035
		 if (src->cx256 < rect_clip.boss_clip_min.x256 )	{	src->cx256 = rect_clip.boss_clip_min.x256;	card.boss_hamidasi = (1);	}
	else if (src->cx256 > rect_clip.boss_clip_max.x256 )	{	src->cx256 = rect_clip.boss_clip_max.x256;	card.boss_hamidasi = (1);	}
//
		 if (src->cy256 < rect_clip.boss_clip_min.y256 )	{	src->cy256 = rect_clip.boss_clip_min.y256;	card.boss_hamidasi = (1);	}
	else if (src->cy256 > rect_clip.boss_clip_max.y256 )	{	src->cy256 = rect_clip.boss_clip_max.y256;	card.boss_hamidasi = (1);	}
	#else//1574939
	/*(とりあえず)*/
	src->cx256 = psp_min(src->cx256, rect_clip.boss_clip_max.x256);/* 右チェック(喰み出す) */
	src->cx256 = psp_max(src->cx256, rect_clip.boss_clip_min.x256);/* 左チェック(喰み出す) */
	src->cy256 = psp_min(src->cy256, rect_clip.boss_clip_max.y256);/* 下チェック(喰み出す) */
	src->cy256 = psp_max(src->cy256, rect_clip.boss_clip_min.y256);/* 上チェック(喰み出す) */
	#endif
//
	if (0 < src->BOSS_DATA_03_kougeki_anime_count)	/* 攻撃アニメーション */	/* 攻撃アニメーション用カウンタ / 0以下なら移動アニメーション */
	{
		src->BOSS_DATA_03_kougeki_anime_count--;
				if ((32)>src->BOSS_DATA_03_kougeki_anime_count) {	src->type = BOSS_09_32; }	/*src->an im_frame = 0x23;*/
		else	if ((40)>src->BOSS_DATA_03_kougeki_anime_count) {	src->type = BOSS_10_33; }	/*src->an im_frame = 0x22;*/
		else										{	src->type = BOSS_09_32; }	/*src->an im_frame = 0x21;*/
	}
	else	/* 移動アニメーション */
	{
		int aaa;
		aaa = (src->cx256> src->BOSS_DATA_00_target_x256)?(BOSS_00_11/*0x00*/):(BOSS_04_21/*0x10*/); /* 左右 */

		#if 0
				if ( ( 16) > src->BOSS_DATA_04_toutatu_wariai256)		{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else	if ( (200) > src->BOSS_DATA_04_toutatu_wariai256)		{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else	if ( (224) > src->BOSS_DATA_04_toutatu_wariai256)		{	aaa+=2;}	/*128.0==t256(0.5)*/
		else	if ( (240) > src->BOSS_DATA_04_toutatu_wariai256)		{	aaa+=1;}	/*179.2==t256(0.7)*/
		else									{	aaa+=0;}
		#else
				if ( (256- 16) < src->BOSS_DATA_04_toutatu_wariai256)	{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else	if ( (256-200) < src->BOSS_DATA_04_toutatu_wariai256)	{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else	if ( (256-224) < src->BOSS_DATA_04_toutatu_wariai256)	{	aaa+=2;}	/*128.0==t256(0.5)*/
		else	if ( (256-240) < src->BOSS_DATA_04_toutatu_wariai256)	{	aaa+=1;}	/*179.2==t256(0.7)*/
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
	card_boss_move_generate_check_regist(src);	/* カード登録可能なら登録 / カード生成 */
	boss_move_vx_vy_animation(src);
	move_card_square_effect(src);				/* 回エフェクト */
	card_generate(src); 						/* スペルをCPU実行し、カードを１フレーム生成する。 */
}
