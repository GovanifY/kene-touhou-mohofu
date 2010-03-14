
#include "douchu.h"

/*---------------------------------------------------------
		"烏1",		"", (r27から新規追加)
	-------------------------------------------------------
	user_x: 	速度256[speed256]
	user_y: 	横位置[dots]
---------------------------------------------------------*/

#define USE_X_HOUKOU (0)


#define time_out			user_data02
#define state				user_data04

//	int angleCCW512;	/* 下が０として左回り(逆回り)の角度 */
//	int speed256;

//#define KARASU3_aaa	vy256
//
//	int anime_houkou;

/*---------------------------------------------------------
	共用変数
---------------------------------------------------------*/

/* [システム]保持用角度(下が0度で左回り(反時計回り)) */
/* このザコでは(この変数は)使わないので共用(流用)する。 */
#define KARASU3_anime_houkou	tmp_angleCCW512


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
enum
{
	ST00 = 0,
	ST01,
};

static void move_karasu1(SPRITE *src)
{
	src->time_out--;
//	if (0 > src->time_out)
//	{
//		src->type = SP_DELETE;	/* おしまい */
//	}
	switch (src->state)
	{
	case ST00:	/* 登場 攻撃 */
		switch (src->time_out)
		{
		case 16:
			src->state++;
			break;
		}
		/* 攻撃 */
		if (0==((src->time_out)&0x3f))
		{
			if (difficulty)
			{
				send1_obj->x256 = src->x256;
				send1_obj->y256 = src->y256;
				send1_obj->BULLET_REGIST_speed256			=		(t256(2.0)+((difficulty)<<6));
				send1_obj->BULLET_REGIST_angle512			=		ANGLE_JIKI_NERAI_DAN;
				send1_obj->BULLET_REGIST_div_angle512		=		(int)(512/(8));
				send1_obj->BULLET_REGIST_bullet_obj_type	=		BULLET_KUNAI12_04_YUKARI+((ra_nd())&7);
				send1_obj->BULLET_REGIST_n_way				=		(8);
				bullet_regist_basic();
			}	/*なるべく共通化*/
		}
		break;
	case ST01:	/* 離脱 */
		gamen_gai_nara_osimai(src);/* 画面外ならおしまい */
		break;
	}
	/* 移動する */
	src->x256 += (src->vx256);/*fps_factor*/
	#if (1==USE_X_HOUKOU)
	src->y256 += (src->vy256);/*fps_factor*/
	#endif

//	src->yx_an im_frame = (8+((src->time_out>>2)&(8-1)));
//	src->yx_an im_frame = (src->KARASU3_anime_houkou | ((src->time_out>>2)&(4-1)));
/*
src->time_out		 ---a bc--
src->yx_an im_frame  yyyy xxxx
src->yx_an im_frame  --1a --bc
*/
	if (SP_DELETE != src->type)
	{
		src->type				= (TEKI_12_YOUSEI1_1)+(src->KARASU3_anime_houkou | ((src->time_out>>2)&(4-1)));
	}
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_karasu1(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_24_YOUSEI2_1;	/* 妖精2 */
	add_zako_common(l, h);
//	h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->callback_mover		= move_karasu1;
//	h->callback_loser		= lose_random_item;
//	h->callback_hit_enemy	= callback_hit_zako;
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
			h->x256 			= t256(-30);
		//	h->KARASU3_anime_houkou = ((0x00)>>2);
			h->KARASU3_anime_houkou = ((0x30)>>2);/*右向きカラス*/
		}
		else
		{/* [引数 user_y が正方向の場合、左へ移動(←)] [引数yが負方向の場合、上へ移動(↑)] */
			h->x256 			= t256(360);/* 360 > 352(x_max) > 272(y_max) */
			h->KARASU3_anime_houkou = ((0x50)>>2);/*左向きカラス*/
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
			int s_sss;
			s_sss				= s->x256;
			s->x256 			= -(s->y256);
			s->y256 			= s_sss;
		}
		s->vy256			= (s->vx256);
		s->vx256			= (0);
		s->KARASU3_anime_houkou 	= ((0x20)>>2);
	}
	#endif
//
	h->time_out			= (320);
	h->state 			= 0;
}
