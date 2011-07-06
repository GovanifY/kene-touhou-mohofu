
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	子供魔方陣 弾幕
	-------------------------------------------------------
	C.360ナイフを撃つぜ
	-------------------------------------------------------
	(無駄に複雑だったので、
		A.レーザーを撃つ機能と、
		B.星型を描く機能と、
		C.360ナイフを撃つ機能と、
	を分離させた)
---------------------------------------------------------*/
#if 0/*メモ*/
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define boss_time_out		user_data03 	/* 制限時間 */
#endif
#ifndef boss_time_out
	#define boss_time_out		user_data03 	/* 制限時間 */
#endif

/*---------------------------------------------------------
	子供魔方陣 移動
---------------------------------------------------------*/

static void move_doll03(SPRITE *src)
{
	check_boss_option_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
//
	#if 1
	/* 魔方陣アニメーション */
	src->rotationCCW1024--;/* 右回り */
	mask1024(src->rotationCCW1024);
	#endif
//
	if ((64*8) < src->boss_time_out)
	{
		if (0==((src->boss_time_out)&(64-1)))/* 64回に1回発弾する */
		{
			static int count_jjj;
			count_jjj++;
			if ((3-difficulty) < count_jjj)
			{
				count_jjj = 0;
				/*---------------------------------------------------------
					360ナイフ
					魔方陣用
					全方向に青ナイフ弾を撃つ
				---------------------------------------------------------*/
				obj_send1->cx256 = (src->cx256)+t256(8.0);/* 8.0[dots]右 */
				obj_send1->cy256 = (src->cy256)+t256(4.0);/* 4.0[dots]下 */
				br.BULLET_REGIST_speed256			= (t256(1.2));					/*(speed256)*/
				br.BULLET_REGIST_angle1024			= (0);							/* 弾源角度 */
				br.BULLET_REGIST_div_angle1024		= (int)(1024/(18)); 			/* 分割角度([360/360]度を18分割) */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_06_YUKARI; 	/* [ピンクナイフ弾] */	/*BULLET_KNIFE20_04_AOI*/
				br.BULLET_REGIST_n_way				= (18); 						/* [18way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
	}
//
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

/*static*/ void add_zako_sakuya_doll_03_360knife(SPRITE *src)
{
	SPRITE *h;
	h								= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		h->m_Hit256R				= ZAKO_ATARI16_PNG;
	//	h->type 					= TEKI_51_MAHOJIN1;
		h->type 					= BOSS_16_YOUSEI11;
		h->base_hp					= (9999);/* 倒せない */
		h->flags					= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//	h->color32					= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);
		h->color32					= MAKE32RGBA(0x88, 0x33, 0xff, 0x80); /* っぽく */
		h->callback_mover			= move_doll03;
		/* 子供魔方陣、配置位置 */
		#if 1
		h->cx256 					= src->cx256;
		h->cy256 					= src->cy256-t256(16);/*咲夜 上方に配置*/
		#endif
		h->boss_time_out				= (64*8)+(16*64);	/* 制限時間 */	/* 16[弾]撃つ */
	}
}
