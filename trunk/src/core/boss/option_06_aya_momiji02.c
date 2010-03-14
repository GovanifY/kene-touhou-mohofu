
#include "bullet_object.h"

/*---------------------------------------------------------

’e–‹
	-------------------------------------------------------


---------------------------------------------------------*/

#define MOMIJI_KODOMO_DATA_angle512 			user_data00 	/*union BULLET_ANGLE_DATA*/
#define MOMIJI_KODOMO_DATA_speed256 			user_data01 	/*union BULLET_ANGLE_DATA*/
//
#define time_out	user_data02 						/* Œo‰ßŽžŠÔ */
//
#define MOMIJI_KODOMO_DATA_angle222_512 		user_data03
#define MOMIJI_KODOMO_DATA_adjust_speed256		user_data04


/*---------------------------------------------------------
	•ª—ô(last)
---------------------------------------------------------*/

static void move_aya_doll_last_burrets(SPRITE *src)
{
	send1_obj->x256 = src->x256;		/* ’eŒ¹x256 */
	send1_obj->y256 = src->y256;		/* ’eŒ¹y256 */
	send1_obj->BULLET_REGIST_speed256			=	(t256(0.75)+(ra_nd()&0xff));		/* ’e‘¬ */		/*+((src->boss_base_danmaku_time_out)<<2)*/
	send1_obj->BULLET_REGIST_angle512			=	((ra_nd()&0x1ff));					/* src->tmp_angleCCW512 */
	send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/23);						/* Šp“x (512/27) (512/24) */
	send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KOME_02_AKA+(difficulty&0x02);				/* [Ô•Ä’e] */
	send1_obj->BULLET_REGIST_n_way				=	(1+difficulty); 							/* [7way] [8way] */
	bullet_regist_basic();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_bullet_momiji(SPRITE *src)
{
	src->time_out--;
	if ((0 > src->time_out))
	{
		move_aya_doll_last_burrets(src);
		src->type = SP_DELETE;	/* ‚¨‚µ‚Ü‚¢ */
	}
	else
	{
		if (src->MOMIJI_KODOMO_DATA_speed256 < t256(0.5) )
		{
			src->MOMIJI_KODOMO_DATA_angle222_512 += 256/*rad2 deg512(M_PI)*/;
			#if 1
			/*‚Ë‚ñ‚Ì‚½‚ß*/
			mask512(src->MOMIJI_KODOMO_DATA_angle222_512);
			#endif
			src->MOMIJI_KODOMO_DATA_angle512	= src->MOMIJI_KODOMO_DATA_angle222_512;
			src->MOMIJI_KODOMO_DATA_adjust_speed256 = (-(src->MOMIJI_KODOMO_DATA_adjust_speed256));
		}
		src->MOMIJI_KODOMO_DATA_speed256 += src->MOMIJI_KODOMO_DATA_adjust_speed256;
		if (src->MOMIJI_KODOMO_DATA_angle512-src->MOMIJI_KODOMO_DATA_angle222_512 < (int)(512*1/12) )
		{
			src->MOMIJI_KODOMO_DATA_angle512 += 1/*0.01*/;
		}
		#if 1
		/*‚Ë‚ñ‚Ì‚½‚ß*/
		mask512(src->MOMIJI_KODOMO_DATA_angle512);
		#endif
	//	mono_angle_move(src,(BULLET_ANGLE_DATA *)data);
		src->vx256 = ((sin512((src->MOMIJI_KODOMO_DATA_angle512))*src->MOMIJI_KODOMO_DATA_speed256)>>8);/*fps_factor*/
		src->vy256 = ((cos512((src->MOMIJI_KODOMO_DATA_angle512))*src->MOMIJI_KODOMO_DATA_speed256)>>8);/*fps_factor*/
		src->x256 += (src->vx256);/*fps_factor*/
		src->y256 += (src->vy256);/*fps_factor*/
	}
}

/*---------------------------------------------------------
	5•ûŒü‚à‚Ý‚¶¬’e‚ð¶¬
---------------------------------------------------------*/
#define MOMIJI_KODOMO_next_angle512 tmp_angleCCW512 	/* Žq‹Ÿ’eA¶¬Šp“x */

/*static*/ void add_zako_aya_5_momiji(SPRITE *src)
{
	int angle512;
	angle512 = (src->MOMIJI_KODOMO_next_angle512)-((int)(512*1/20));/*data->angle512*/
//
	int jj;
	for (jj=(0); jj<(5); jj++ )
	{
		SPRITE *h;
	//	h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_MARU16_PNG);
		h					= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	//	h->type 			= /*BULLET_MINI8_00_AKA*/BULLET_MARU8_00_AKA+((angle512>>6)&7); /*BULLET_MINI8_00_AKA*/ /*S P_BULLET*/
		h->type 			= TEKI_24_YOUSEI2_1+((angle512>>6)&0x07);/* 0 ... 8 */
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		h->callback_mover	= move_bullet_momiji;
	//	h->callback_loser		= NULL;
//		h->callback_hit_enemy	= callback_hit_zako;/*???*/
		h->time_out 		= (50); 	/* 200Žõ–½ */
		{
			/* ‰ŠúˆÊ’u */
			h->x256 			= src->x256;
			h->y256 			= src->y256;
		}
	//
		h->MOMIJI_KODOMO_DATA_angle222_512		= (angle512);
		h->MOMIJI_KODOMO_DATA_angle512			= (angle512);
		h->MOMIJI_KODOMO_DATA_speed256			= t256(3.0);
		h->MOMIJI_KODOMO_DATA_adjust_speed256	= t256(-0.04);
//
		angle512 += (int)(512*1/20);/*offset_angle512*/
		mask512(angle512);
	}
}
