
#include "douchu.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"青妖精3",		"FAIRY",	(r27から新規追加)
	-------------------------------------------------------
	user_x: 	速度256[speed256]
	user_y: 	横位置[dots]
---------------------------------------------------------*/
#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif

#define USE_X_HOUKOU (0)

#define time_out		user_data03
#define KARASU3_houkou	user_data04

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_ao_yousei3(SPRITE *src)
{
	src->time_out--;
	if (0 > src->time_out)		/* 離脱 */
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	}
	else
	{
		/* 攻撃 */
		if (0==((src->time_out)&0x3f))
		{
			if (difficulty)
			{
				obj_send1->x256 					= src->x256;
				obj_send1->y256 					= src->y256;
				br.BULLET_REGIST_speed256			= (t256(2.0)+((difficulty)<<6));
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_div_angle1024		= (int)(1024/(8));
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_04_YUKARI+((ra_nd())&7);
				br.BULLET_REGIST_n_way				= (8);
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
	}
	/* 移動する */
	src->x256 += (src->vx256);/*fps_factor*/
	#if (1==USE_X_HOUKOU)
	src->y256 += (src->vy256);/*fps_factor*/
	#endif

//	src->yx_an im_frame = (8+((src->time_out>>2)&(8-1)));
//	src->yx_an im_frame = (src->KARASU3_houkou | ((src->time_out>>2)&(4-1)));
/*
src->time_out		 ---a bc--
src->yx_an im_frame  yyyy xxxx
src->yx_an im_frame  --1a --bc
*/
	if (SP_DELETE != src->type)
	{
		src->type				= (TEKI_12_YOUSEI1_1)+(src->KARASU3_houkou | ((src->time_out>>2)&(4-1)));
	}
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_ao_yousei3(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_12_YOUSEI1_1;
		h->callback_mover		= move_ao_yousei3;
		//
		h->y256 				= ((l->user_y)<<8);
		h->vx256				= ((l->user_x));
		#if (1==USE_X_HOUKOU)
		h->vy256				= (0);
		#endif
		//
		{
			if (0 < (h->vx256))
			{/* [引数 user_y が正方向の場合、右へ移動(→)] [引数yが負方向の場合、下へ移動(↓)] */
				h->x256 				= t256(-30);
			//	h->KARASU3_houkou		= ((0x00)>>2);
				h->KARASU3_houkou		= ((0x30)>>2);/*右向きカラス*/
			}
			else
			{/* [引数 user_y が正方向の場合、左へ移動(←)] [引数yが負方向の場合、上へ移動(↑)] */
				h->x256 				= t256(360);/* 360 > 352(x_max) > 272(y_max) */
				h->KARASU3_houkou		= ((0x50)>>2);/*左向きカラス*/
			}
		}
		//
		#if (1==USE_X_HOUKOU)
		if (0 < (h->y256))
		{	;	}
		else
		{
			/* y座標が負方向の場合は、x座標指定なので(x座標 y座標 を逆にする) */
			{
				int h_hhh;
				h_hhh			= h->x256;
				h->x256 		= -(h->y256);
				h->y256 		= h_hhh;
			}
			h->vy256			= (h->vx256);
			h->vx256			= (0);
			h->KARASU3_houkou	= ((0x20)>>2);
		}
		#endif
		//
		h->time_out 			= (320);
	}
}
