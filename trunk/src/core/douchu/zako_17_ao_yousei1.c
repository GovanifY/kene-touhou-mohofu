
#include "douchu.h"

/*---------------------------------------------------------
		"青妖精1",		"FAIRY",
	-------------------------------------------------------
	user_x: 	速度256[speed256]
	user_y: 	横位置[dots]
---------------------------------------------------------*/

/* 敵の向き tmp_angleCCW512  */

#define speed256		user_data01 	/* 速度 */
#define time_out		user_data02 	/* 経過時間 */

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
#define BASE_TIME (512)
static void move_ao_yousei1(SPRITE *src)
{
	src->time_out--;
//	if (0 > src->time_out)
//	{
//		src->type = SP_DELETE;	/* おしまい */
//	}

	if (BASE_TIME > src->time_out)
	{
//	case 1: 	/* 離脱 */
		src->speed256 += 2/*+(1)*/;
		src->tmp_angleCCW512 += 2;
		if (255 < src->tmp_angleCCW512)
		{
			src->tmp_angleCCW512 = 255;/*真上*/
		}
		if (0 > src->y256)
		{
			src->type = SP_DELETE;	/* おしまい */
		}
//		break;
	}
	else	/* 登場 攻撃 */
	{
		switch (src->time_out)
		{
		case (BASE_TIME+30):
			src->time_out = BASE_TIME-1;
			break;
		case (BASE_TIME+50):
			if (difficulty)
			{
				send1_obj->x256 = src->x256;
				send1_obj->y256 = src->y256;
				send1_obj->BULLET_REGIST_speed256			=	(t256(2.0));								/* 弾速 */	/*+((difficulty)<<6)*/
				send1_obj->BULLET_REGIST_angle512			=	ANGLE_JIKI_NERAI_DAN;
				send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/24)-(difficulty); 				/* 分割角度 */
				send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KUNAI12_04_YUKARI+((ra_nd())&7); 	/* [弾グラ] */
				send1_obj->BULLET_REGIST_n_way				=	(8);										/* 8way弾 */
				bullet_regist_basic();
			}
			break;
		case (BASE_TIME+70):	/*not_break;*/
		case (BASE_TIME+120):	/*not_break;*/
		case (BASE_TIME+170):	/*not_break;*/
		case (BASE_TIME+220):
			if (difficulty)
			{
				send1_obj->x256 = src->x256;
				send1_obj->y256 = src->y256;
				send1_obj->BULLET_REGIST_speed256			=	(t256(2.0));					/* 弾速 */
				send1_obj->BULLET_REGIST_angle512			=	ANGLE_JIKI_NERAI_DAN;
				send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/24)-(difficulty); 	/* 分割角度 */
				send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_00_AOI;			/* [赤弾] */	//	BULLET_MARU8_00_AKA,
				send1_obj->BULLET_REGIST_n_way				=	(difficulty);					/* [1way] */	/*1*/
				bullet_regist_basic();
			}
			break;
		}
		src->speed256 -= (5);
		if (1 > src->speed256)
		{
			src->speed256 = 1;
		}
	}
	/*以下rwingx.cと同じ*/
/* CCWの場合 */
//	src->x256 += ((sin512((src->tmp_angleCCW512))*src->speed256)>>8);/*fps_factor*/
	src->y256 += ((cos512((src->tmp_angleCCW512))*src->speed256)>>8);/*fps_factor*/

//	src->yx_an im_frame = (8+((src->time_out>>2)&(8-1)));
//	src->yx_an im_frame = (0x20 | ((src->time_out)&(0x10)) | ((src->time_out>>2)&(4-1)));
/*
src->time_out		 ---a bc--
src->yx_an im_frame  yyyy xxxx
src->yx_an im_frame  --1a --bc
*/
	if (SP_DELETE != src->type)
	{
//		src->type				= (TEKI_12_YOUSEI1_1)+(0x08)+((src->time_out>>2)&(8-1));
		src->type				= (TEKI_12_YOUSEI1_1)+(0x04)+((src->time_out>>2)&(4-1));
	}
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_ao_yousei1(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_12_YOUSEI1_1;
	add_zako_common(l, h);
	h->callback_mover		= move_ao_yousei1;
	h->callback_loser		= lose_random_item;
//	h->callback_hit_enemy	= callback_hit_zako;
//
//	h->x256 				= (((u32)l->user_x)<<8);/*lv*t256(35)+t256(40)*/
//	h->y256 				= ((l->user_y)<<8);
	h->y256 				+= t256(-128);
//
	h->time_out				= (BASE_TIME+320);
	h->speed256				= ((512));			/*t256(0.5)*/	/*+0.15*di fficulty*/ /*0.7+0.3*di fficulty*/
/* CCWの場合 */
	h->tmp_angleCCW512		= deg_360_to_512CCW(360-0);/*真下*/
//	data->state 			= 0;
//	/*data->base.*/h->base_score		= score(20*2);
//	/*data->base.*/h->base_hp 		= ((8*1)+(di fficulty<<(3))); 	/*(3+(di fficulty))*/	/*1+(di fficulty<<2)*/
}
