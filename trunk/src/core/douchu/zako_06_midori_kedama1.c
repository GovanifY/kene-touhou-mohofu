
#include "douchu.h"

/*---------------------------------------------------------
	“Œ•û–Í•í•—	` Toho Imitation Style.
	ƒvƒƒWƒFƒNƒgƒy[ƒW http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"—Î–Ñ‹Ê1",		"XEV",
	-------------------------------------------------------
	—Î–Ñ‹Ê	’P‘Ì‚Å“Ë‚Á‚ñ‚Å‚­‚éB
---------------------------------------------------------*/
#if 0/* ‚ß‚à */
/* ƒ{ƒX‹¤’Ê‹KŠi */
	#define target_x256 		user_data00 	/* –Ú•WxÀ•W */
	#define target_y256 		user_data01 	/* –Ú•WyÀ•W */
	#define vvv256				user_data02 	/* –Ú•WÀ•W‚Ö‚Ì“’BŠ„‡ */
	#define time_out			user_data03 	/* §ŒÀŠÔ */
#endif

	#define target_x256 		user_data00
	#define target_y256 		user_data01

	#define time_out			user_data03

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_midori_kedama1(SPRITE *src)
{
	int speed256;
	src->time_out--;
	if (0 > src->time_out)		/* —£’E */
	{
		gamen_gai_nara_zako_osimai(src);/* ‰æ–ÊŠO‚È‚ç‚¨‚µ‚Ü‚¢ */
		speed256 = t256(3.5/*6*/);
	}
	else
	{
		if ((src->x256 >= obj_player->x256)||
			(src->y256 > t256(GAME_HEIGHT-48) ) )/*480-80*/
		{
			src->time_out	= 0;
			src->target_x256	= t256(-100);
		//	src->target_y256	= obj_player->y256-t256(48);
			src->target_y256	= obj_player->y256-t256(48);
			{
				obj_send1->x256 					= src->x256;
				obj_send1->y256 					= src->y256;
				br.BULLET_REGIST_speed256			= t256(2.75);				/* ’e‘¬ */
			//	br.BULLET_REGIST_speed256			= speed256; 				/* ’e‘¬ */
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* ©‹@‘_‚¢’e */	/* ”­Ë’†SŠp“x / “Áê‹@”\(©‹@‘_‚¢/‘¼) */
			//	br.BULLET_REGIST_div_angle1024		= (0);						/* ƒ_ƒ~[Šp“x(–¢g—p) */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU8_00_AKA;		/* [Ô’e] */ /* ’eƒOƒ‰ */
				br.BULLET_REGIST_n_way				= (1);						/* [1way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			//
				lose_random_item(src);
			}//t256(1)+((4/*data->enemy_rank*/)<<8)
		}
		speed256 = t256(2.5/*3*/);
	}
	obj_send1->x256 	= (src->target_x256);					/* ’eŒ¹x256 */
	obj_send1->y256 	= (src->target_y256);					/* ’eŒ¹y256 */
	tmp_angleCCW1024_jikinerai(obj_send1, src);
	src->x256 += ((sin1024(src->tmp_angleCCW1024)*speed256)>>8);/*fps_factor*/	/* CCW‚Ìê‡ */
	src->y256 += ((cos1024(src->tmp_angleCCW1024)*speed256)>>8);/*fps_factor*/
//
	src->m_angleCCW1024 += 5;
	mask1024(src->m_angleCCW1024);
}

/*---------------------------------------------------------
	“G‚ğ’Ç‰Á‚·‚é
---------------------------------------------------------*/

global void add_zako_midori_kedama1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* “o˜^‚Å‚«‚½ê‡‚Ì‚İ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_60_AKA_KEDAMA;
		h->callback_mover		= move_midori_kedama1;
	//
		h->target_x256			= obj_player->x256;
	//	h->target_y256			= obj_player->y256; 		/* “ÁU‹Ö~ */
		h->target_y256			= ((obj_player->y256)>>1);
		h->time_out 			= (320);/* ƒeƒLƒg[ */
	}
}
