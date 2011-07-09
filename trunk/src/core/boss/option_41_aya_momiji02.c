
#include "game_main.h"

/*---------------------------------------------------------
	“Œ•û–Í•í•— ` Toho Imitation Style.
	ƒvƒƒWƒFƒNƒgƒy[ƒW http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	’e–‹
	-------------------------------------------------------
---------------------------------------------------------*/

#define MOMIJI_KODOMO_DATA_angle1024			user_data00 	/*union BULLET_ANGLE_DATA*/
#define MOMIJI_KODOMO_DATA_speed256 			user_data01 	/*union BULLET_ANGLE_DATA*/
//
#define time_out								user_data03 	/* Œo‰ßŽžŠÔ */
//
#define MOMIJI_KODOMO_DATA_angle222_1024		user_data04
#define MOMIJI_KODOMO_DATA_adjust_speed256		user_data05


/*---------------------------------------------------------
	•ª—ô(last)
---------------------------------------------------------*/

static void move_aya_doll_last_burrets(SPRITE *src)
{
	br.BULLET_REGIST_00_speed256				= (t256(0.75)+(ra_nd()&0xff));					/* ’e‘¬ */
	br.BULLET_REGIST_02_VECTOR_angle1024		= ((ra_nd()&(1024-1))); 						/* src->tmp_angleCCW1024 */
//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
	br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_01_AKA+(((cg_game_difficulty)&0x02)<<1);	/* [Ô(0x01)/—Î(0x05)•Ä’e] */
	br.BULLET_REGIST_06_n_way					= (1+(cg_game_difficulty)); 							/* [7way] [8way] */
	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/23);								/* Šp“x (1024/27) (1024/24) */
	bullet_regist_multi_vector_send1_xy_src(src); 	/* ’eŒ¹x256 y256 ’†S‚©‚ç”­’eB */
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_bullet_momiji(SPRITE *src)
{
	src->time_out--;
	if ((0 > src->time_out))
	{
		move_aya_doll_last_burrets(src);
		src->jyumyou = JYUMYOU_NASI;	/* ‚¨‚µ‚Ü‚¢ */
	}
	else
	{
		if (src->MOMIJI_KODOMO_DATA_speed256 < t256(0.5) )
		{
			src->MOMIJI_KODOMO_DATA_angle222_1024 += (1024/2);	/* (1024/2) == ”¼Žü == [180/360 “x] */
			#if 1
			/* ‚Ë‚ñ‚Ì‚½‚ß */
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
		/* ‚Ë‚ñ‚Ì‚½‚ß */
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
	5•ûŒü‚à‚Ý‚¶¬’e‚ð¶¬
---------------------------------------------------------*/
#define MOMIJI_KODOMO_next_angle1024	tmp_angleCCW1024	/* Žq‹Ÿ’eA¶¬Šp“x */

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
		if (NULL!=h)/* “o˜^‚Å‚«‚½ê‡‚Ì‚Ý */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= (TEKI_32_YOUSEI2_1)+((angle1024>>7)&0x07);/* 0 ... 8 */
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->callback_mover		= move_bullet_momiji;
			h->time_out 			= (50); 	/* 200Žõ–½ */
			{
				/* ‰ŠúˆÊ’u */
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
