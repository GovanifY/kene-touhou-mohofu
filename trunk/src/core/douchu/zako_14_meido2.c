
#include "douchu.h"

/*---------------------------------------------------------
	"ƒƒCƒh2",		"ZATAK",
	-------------------------------------------------------
	rwingx_zatak (Ô‚¢—d¸)
	¶‰E‚©‚çŒ»‚êAŽ©‹@‘_‚¢’e‚ð‘Å‚Á‚½ŒãA^‰º‚Ö’¼i‚µ‚Ä‚­‚éB
---------------------------------------------------------*/

#define state		user_data02
#define speed256	user_data03
#define wait		user_data04

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_meido2(SPRITE *src)
{
	src->wait++;/*fps_factor*/
	switch (src->state)
	{
	case 0:
		if (src->wait >= 40)
		{
			src->state++/*=1*/;
		}
		break;
	case 1:
		src->speed256 -= t256(0.02);/*fps_factor*/
		if (src->speed256 <= 0)
		{
			src->speed256 = 0;
			src->state++/*=2*/;
		}
		break;
	case 2:
//efine HARIDAN_SPEED ( /*speed256*/(t256(5)+((data_le vel/*src->le vel*/)<<8) ) )
#define HARIDAN_SPEED ( /*speed256*/t256(4/*5*/) + (/*data_hari_rank256*/(1<<8)/*src->le vel*/) )

#if 0
/* CW‚Ìê‡ */
		if (src->angle512 < deg_360_to_512(90))
		{
			src->angle512 += deg_360_to_512(3);/*fps_factor*/
			if (src->angle512 >= deg_360_to_512(89))
			{
				src->state++/* = 3*/;
			}
		}
		else
		{
			src->angle512 -= deg_360_to_512(3);/*fps_factor*/
			if (src->angle512 <= deg_360_to_512(91))
			{
				src->state++/* = 3*/;
			}
		}
#else
/* CCW‚Ìê‡ */
		if (src->tmp_angleCCW512 < deg_360_to_512CCW(180/*360+90-90*/))
		{
			src->tmp_angleCCW512 -= deg_360_to_512CCW(3);/*fps_factor*/
			if (src->tmp_angleCCW512 < deg_360_to_512CCW(0/*360+90-89*/))
			{
				src->state++/* = 3*/;
			}
		}
		else
		{
			src->tmp_angleCCW512 += deg_360_to_512CCW(3);/*fps_factor*/
			if (src->tmp_angleCCW512 > deg_360_to_512CCW(360/*360+90-91*/))
			{
				src->state++/* = 3*/;
			}
		}
#endif
		break;
	case 3: /*’eŒ‚‚Â*/
#if 0
/* CW‚Ìê‡ */
				src->angle512 = deg_360_to_512(90);
				if (data_hari_rank256/*src->le vel*/)
				{
					bullet_create_offset_dan_type000(src, HARIDAN_SPEED, ANGLE_JIKI_NERAI_DAN, BULLET_HARI32_00_AOI);	/*, t256(0), t256(0)*/
				}
#else
/* CCW‚Ìê‡ */
				src->tmp_angleCCW512 = deg_360_to_512CCW(0);
				//if (data_hari_rank256/*src->le vel*/)
				{
					bullet_create_offset_dan_type000(src, HARIDAN_SPEED, ANGLE_JIKI_NERAI_DAN, BULLET_HARI32_00_AOI);	/*, t256(0), t256(0)*/
				}
#endif
				src->state++/* = 4*/;
		break;
	case 4: /*“ÁU*/
		src->speed256 += t256(0.2);/*‰Á‘¬*/ /*t256(0.3)*/
		if (src->speed256 >= t256(2.5)) /*t256(3.0)*/ /*(6+src->le vel)*/
		{
			src->speed256 = t256(2.5); /* ‰Á‘¬Å‘å’l */ /*3*/ /*(6+src->le vel)*/
			src->state++;	/* = 5*/
		}
		break;
	case 5:/*‘Þê*/
		gamen_gai_nara_osimai(src);/* ‰æ–ÊŠO‚È‚ç‚¨‚µ‚Ü‚¢ */
		break;
	}
	/*ˆÈ‰ºrwingx.c‚Æ“¯‚¶*/
/* CCW‚Ìê‡ */
	src->x256+=((sin512((src->tmp_angleCCW512))*src->speed256)>>8);/*fps_factor*/
	src->y256+=((cos512((src->tmp_angleCCW512))*src->speed256)>>8);/*fps_factor*/
//
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_36_YOUSEI3_1+((((src->tmp_angleCCW512)&(512-1)))>>6);
	}
}


/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

void add_zako_meido2(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_36_YOUSEI3_1;/*SP_ZAKO*/ /*_13_MEIDO2*/
	add_zako_common(l, h);
//	h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->callback_mover		= move_meido2;
//	h->callback_loser		= lose_random_item;
//	h->callback_hit_enemy	= callback_hit_zako;


//	/*h->base.*/h->base_score	= score(25*2);
//	/*h->base.*/h->base_hp		= (8*1)+(1/*di fficulty*/<<(2+3));
	h->state				= 0;
	h->wait 				= 0;
	h->speed256 			= ((ra_nd()&((256*4)-1)));/*2.0*(((dou ble)ra_nd()/RAND_MAX)*2)*/
/* CCW‚Ìê‡ */
//	if ((i&1))
	if (t256(GAME_WIDTH/2) > h->x256 )
	{
		h->tmp_angleCCW512	= deg_360_to_512CCW(90-10/*360-10*/);
	//	h->x256 			= -t256(32);
	}
	else
	{
		h->tmp_angleCCW512	= deg_360_to_512CCW(180+90+10/*360-170*/);
	//	h->x256 			= t256(GAME_WIDTH);
	}
//	h->y256 = t256(50);
}
