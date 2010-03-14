
#include "douchu.h"

/*---------------------------------------------------------
		"Â—d¸4",(—\’è)		"FAIRY",	(r27‚©‚çV‹K’Ç‰Á)
	-------------------------------------------------------
	user_x: 	‘¬“x256[speed256]
	user_y: 	‰¡ˆÊ’u[dots]
---------------------------------------------------------*/

//#define tx256 	user_data00
//#define ty256 	user_data01
#define time_out	user_data02
#define speed256	user_data03
#define state		user_data04

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_ao_yousei4(SPRITE *src)
{
	src->time_out--;
//
	switch (src->state)
	{
	case 0: 	/* “oê UŒ‚ */
		src->speed256--;
		if (1 > src->speed256)
		{
			src->speed256 = 1;
		}
		switch (src->time_out)
		{
		case 30:
			src->state++;
			break;
		case 50:
			if (difficulty)
			{
				send1_obj->x256 = src->x256;
				send1_obj->y256 = src->y256;
				send1_obj->BULLET_REGIST_speed256			=		(t256(2.0)+((difficulty)<<6));
				send1_obj->BULLET_REGIST_angle512			=		ANGLE_JIKI_NERAI_DAN;
				send1_obj->BULLET_REGIST_div_angle512		=		(int)(512/24);
				send1_obj->BULLET_REGIST_bullet_obj_type	=		BULLET_KUNAI12_04_YUKARI+((ra_nd())&7);
				send1_obj->BULLET_REGIST_n_way				=		(8);
				bullet_regist_basic();
			}	/*‚È‚é‚×‚­‹¤’Ê‰»*/
			break;
		case 70:	/*not_break;*/
		case 120:	/*not_break;*/
		case 170:	/*not_break;*/
		case 220:
			if (difficulty) {	bullet_create_aka_maru_jikinerai(src, (512-src->time_out)+t256(difficulty));	}
			break;
		}
		break;
	case 1: 	/* —£’E */
		src->speed256 += 2;
		src->tmp_angleCCW512 += 2;
		if (255 < src->tmp_angleCCW512)
		{
			src->tmp_angleCCW512 = 255;/*^ã*/
		}
		gamen_gai_nara_osimai(src);/* ‰æ–ÊŠO‚È‚ç‚¨‚µ‚Ü‚¢ */
		break;
	}
	/*ˆÈ‰ºrwingx.c‚Æ“¯‚¶*/
/* CCW‚Ìê‡ */
	src->x256+=((sin512((src->tmp_angleCCW512))*src->speed256)>>8);/*fps_factor*/
	src->y256+=((cos512((src->tmp_angleCCW512))*src->speed256)>>8);/*fps_factor*/

//	src->yx_an im_frame = (8+((src->time_out>>2)&(8-1)));
//	src->yx_an im_frame = (0x20 | ((src->time_out)&(0x10)) | ((src->time_out>>2)&(4-1)));
/*
src->time_out		 ---a bc--
src->yx_an im_frame  yyyy xxxx
src->yx_an im_frame  --1a --bc
*/
	if (SP_DELETE != src->type)
	{
//		src->type				= (TEKI_12_YOUSEI1_1)+((src->time_out>>2)&(8-1));
//		src->type				= (TEKI_12_YOUSEI1_1)+((src->time_out>>2)&(8-1));
//		src->type				= (TEKI_12_YOUSEI1_1)+(0x08)+((src->time_out>>2)&(8-1));
		src->type				= (TEKI_12_YOUSEI1_1)+(0x04)+((src->time_out>>2)&(4-1));
	}
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

void add_zako_ao_yousei4(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_12_YOUSEI1_1;
	h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->callback_mover		= move_ao_yousei4;
	h->callback_loser		= lose_random_item;
	h->callback_hit_enemy	= callback_hit_zako;
	h->time_out 			= (320);
//
	h->y256 				= t256(-30);
	h->x256 				= ((l->user_y)<<8);/*lv*t256(35)+t256(40)*/
	h->speed256 			= ((l->user_x));			/*t256(0.5)*/	/*+0.15*di fficulty*/ /*0.7+0.3*di fficulty*/

	h->tmp_angleCCW512		= deg_360_to_512CCW(0);/*^‰º*/ 	/* CCW‚Ìê‡ */

	h->state				= 0;
	/*h->base.*/h->base_score		= score(20*2);
	/*h->base.*/h->base_hp			= ((8*1)+(1/*di fficulty*/<<(3)));	/*(3+(di fficulty))*/	/*1+(di fficulty<<2)*/
}
