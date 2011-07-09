
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 〜 Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	弾幕
	-------------------------------------------------------
---------------------------------------------------------*/

#define MOMIJI_KODOMO_DATA_angle1024			user_data00 	/*union BULLET_ANGLE_DATA*/
#define MOMIJI_KODOMO_DATA_speed256 			user_data01 	/*union BULLET_ANGLE_DATA*/
//
#define time_out								user_data03 	/* 経過時間 */
//
#define MOMIJI_KODOMO_DATA_angle222_1024		user_data04
#define MOMIJI_KODOMO_DATA_adjust_speed256		user_data05


/*---------------------------------------------------------
	分裂(last)
---------------------------------------------------------*/

static void move_aya_doll_last_burrets(SPRITE *src)
{
	br.BULLET_REGIST_00_speed256				= (t256(0.75)+(ra_nd()&0xff));					/* 弾速 */
	br.BULLET_REGIST_02_VECTOR_angle1024		= ((ra_nd()&(1024-1))); 						/* src->tmp_angleCCW1024 */
//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
	br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_01_AKA+(((cg_game_difficulty)&0x02)<<1);	/* [赤(0x01)/緑(0x05)米弾] */
	br.BULLET_REGIST_06_n_way					= (1+(cg_game_difficulty)); 							/* [7way] [8way] */
	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/23);								/* 角度 (1024/27) (1024/24) */
	bullet_regist_multi_vector_send1_xy_src(src); 	/* 弾源x256 y256 中心から発弾。 */
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_bullet_momiji(SPRITE *src)
{
	src->time_out--;
	if ((0 > src->time_out))
	{
		move_aya_doll_last_burrets(src);
		src->jyumyou = JYUMYOU_NASI;	/* おしまい */
	}
	else
	{
		if (src->MOMIJI_KODOMO_DATA_speed256 < t256(0.5) )
		{
			src->MOMIJI_KODOMO_DATA_angle222_1024 += (1024/2);	/* (1024/2) == 半周 == [180/360 度] */
			#if 1
			/* ねんのため */
			mask1024(src->MOMIJI_KODOMO_DATA_angle222_1024);
			#endif
			src->MOMIJI_KODOMO_DATA_angle1024	= src->MOMIJI_KODOMO_DATA_angle222_1024;
			src->MOMIJI_KODOMO_DATA_adjust_speed256 = (-(src->MOMIJI_KODOMO_DATA_adjust_speed256));
		}
		src->MOMIJI_KODOMO_DATA_speed256 += src->MOMIJI_KODOMO_DATA_adjust_speed256;
		if (src->MOMIJI_KODOMO_DATA_angle1024-src->MOMIJI_KODOMO_DATA_angle222_1024 < (int)(1024*1/12) )
		{
			src->MOMIJI_KODOMO_DATA_angle1024 += (1+1);/*0.01*/
		}
		#if 1
		/* ねんのため */
		mask1024(src->MOMIJI_KODOMO_DATA_angle1024);
		#endif
	//	mono_angle_move(src,(BULLET_ANGLE_DATA *)data);
		src->vx256 = ((sin1024((src->MOMIJI_KODOMO_DATA_angle1024))*src->MOMIJI_KODOMO_DATA_speed256)>>8);/*fps_factor*/
		src->vy256 = ((cos1024((src->MOMIJI_KODOMO_DATA_angle1024))*src->MOMIJI_KODOMO_DATA_speed256)>>8);/*fps_factor*/
		src->cx256 += (src->vx256);/*fps_factor*/
		src->cy256 += (src->vy256);/*fps_factor*/
	}
}

/*---------------------------------------------------------
	5方向もみじ小弾を生成
---------------------------------------------------------*/
#define MOMIJI_KODOMO_next_angle1024	tmp_angleCCW1024	/* 子供弾、生成角度 */

/*static*/ void add_zako_aya_5_momiji(SPRITE *src)
{
	int angle1024;
	angle1024 = (src->MOMIJI_KODOMO_next_angle1024)-((int)(1024*1/20));
//
	int jj;
	for (jj=(0); jj<(5); jj++ )
	{
		SPRITE *h;
		h							= obj_add_01_teki_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= (TEKI_32_YOUSEI2_1)+((angle1024>>7)&0x07);/* 0 ... 8 */
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->callback_mover		= move_bullet_momiji;
			h->time_out 			= (50); 	/* 200寿命 */
			{
				/* 初期位置 */
				h->cx256			= src->cx256;
				h->cy256			= src->cy256;
			}
		//
			h->MOMIJI_KODOMO_DATA_angle222_1024 	= (angle1024);
			h->MOMIJI_KODOMO_DATA_angle1024 		= (angle1024);
			h->MOMIJI_KODOMO_DATA_speed256			= t256(3.0);
			h->MOMIJI_KODOMO_DATA_adjust_speed256	= t256(-0.04);
		//
			angle1024 += (int)(1024*1/20);/*offset_angle*/
			mask1024(angle1024);
		}
	}
}
