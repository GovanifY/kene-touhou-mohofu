
#include "douchu.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"緑毛玉2",		"CRUSHER",
	-------------------------------------------------------
	緑毛玉	横一列に並んで、上から突っ込んでくる。
---------------------------------------------------------*/
#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif

#define wait1	user_data04 	/* 上下に動いたカウンタ */
#define wait2	user_data05 	/* 繰り返した回数のカウンタ */

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_midori_kedama2(SPRITE *src)
{
	if (0 < src->wait2)
	{
		if (0 == src->wait1)
		{
		//	src->y256 += (t256(3.00)+t256(data->enemy_rank));			/*fps_factor*/	/*(t256(3)+t256(data->enemy_rank))*/ /*はやすぎ*/
			src->y256 += (t256(0.75)+((3/*data->enemy_rank*/)<<6)); 	/*fps_factor*/	/*(t256(3)+t256(data->enemy_rank))*/
			if (src->y256 > ((difficulty<<5) + t256(64)) )			/* 下限  t256(160) t256(200) */
			{
				src->wait1 = 1;
				src->wait2 -= 1;
			}
		}
		else
		{
		//	src->y256 -= (t256(2.00)+t256(data->enemy_rank));			/*fps_factor*/	/*(t256(2)+t256(data->enemy_rank/3))*/ /*はやすぎ*/
			src->y256 -= (t256(0.50)+((2/*data->enemy_rank*/)<<5)); 	/*fps_factor*/	/*(t256(2)+t256(data->enemy_rank/3))*/
			if (src->y256 < t256(32))						/* 上限  t256(50) */
			{
				src->wait1 = 0;
			}
		}
	//	if (/*0 <*/ /*data->*/enemy_rank)	/* easyは速いんだから場合分けしない */
		{
			/*
			0  0/100   0/65536
			1  1/90  728/65536
			2  1/80  819/65536
			3  1/70  936/65536
			*/
		//	static const u16 kakuritu_tbl[4] =
		//	{ 0, 728, 819, 936 };
		//	if (0==(ra_nd()%(100-data->enemy_rank*10)))
		//	if (kakuritu_tbl[3/*data->enemy_rank*/] > (ra_nd()&(65536-1)))
			if (936 > (ra_nd()&(65536-1)))
			{
				obj_send1->x256 					= src->x256;
				obj_send1->y256 					= src->y256;
				br.BULLET_REGIST_speed256			= t256(2.5)+(2/*data->enemy_rank*/<<6); 				/* 弾速 */	/*高速弾*/	//t256(3)+((/*data->*/enemy_rank<<7))
			//	br.BULLET_REGIST_speed256			= speed256; 				/* 弾速 */
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* 自機狙い弾 */	/* 発射中心角度 / 特殊機能(自機狙い/他) */
			//	br.BULLET_REGIST_div_angle1024		= (0);						/* ダミー角度(未使用) */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU8_00_AKA;		/* [赤弾] */ /* 弾グラ */
				br.BULLET_REGIST_n_way				= (1);						/* [1way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
	}
	else
	{
	//	src->y256 -= t256(4);/*はやすぎ*/
		src->y256 -= t256(1.5);
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	}
//
	src->m_angleCCW1024 += 10;
	mask1024(src->m_angleCCW1024);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_midori_kedama2(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_60_AKA_KEDAMA;
		h->callback_mover		= move_midori_kedama2;
		h->wait1				= 0;
		h->wait2				= 5;/*2*/
	}
}
