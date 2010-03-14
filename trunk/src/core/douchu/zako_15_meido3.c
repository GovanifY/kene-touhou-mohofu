
#include "douchu.h"

/*---------------------------------------------------------
		"ƒƒCƒh3",		"CURVER",	rwingx_curver
	-------------------------------------------------------
ã‚©‚çŒ»‚ê‚ÄSš‚ğn‰ñ•`‚¢‚Ä‰¡‚Ö

©©
«
¨¨¨¨¨
		«
©©©©©
«
¨¨¨¨¨
		«
©©©©©
	-------------------------------------------------------
	ƒoƒO‚ ‚èB
---------------------------------------------------------*/

#define clip_left256	user_data00
#define clip_right256	user_data01
#define state			user_data02
#define speed256		user_data03
#define turn_count		user_data04
#define turnspeed512	user_data05

static int static_last=0;		/* “oêˆÊ’u‚ÌØ‚è‘Ö‚¦ */


/*---------------------------------------------------------
	“G‚â‚ç‚ê
---------------------------------------------------------*/

//static void lose_meido3(SPRITE *src)
//{
//	destoroy[static_last]++;
//	if ( (N UM_OF_ENEMIES-1) < destoroy[static_last]/*all_destoroy*/)
//	{
//		destoroy[static_last] = 0;
//		item_create(src, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
//	}
//}

/*---------------------------------------------------------
	“GUŒ‚
---------------------------------------------------------*/

static void shot_meido3(SPRITE *src)
{
//	if (0 < /*src->*/enemy_rank)
	{
		send1_obj->x256 = src->x256;
		send1_obj->y256 = src->y256;
		send1_obj->BULLET_REGIST_speed256			=	t256(2.0);				/*speed256*/
		send1_obj->BULLET_REGIST_angle512			=	ANGLE_JIKI_NERAI_DAN;	/*angle512*/
		send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/24);
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_01_AKA;
		send1_obj->BULLET_REGIST_n_way				=	(difficulty<<1);		/*n*/
		bullet_regist_basic();
	}	/* easyˆÈŠO‚Í‘_‚¢’e‚ğŒ‚‚Â */
}

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_meido3(SPRITE *src)
{
	switch (src->state)
	{
	case 0: 	/* ¶ü‚è‚Å‰ñ“]’† */
		/* CW‚Ìê‡ */
	//	src->tmp_angleCCW512 -= src->turnspeed512;/*fps_factor*/
		/* CCW‚Ìê‡ */
		src->tmp_angleCCW512 += src->turnspeed512;/*fps_factor*/
		mask512(src->tmp_angleCCW512);
		/* ‰E(Î‚ßã)‚ğŒü‚¢‚½‚ç */
		/* CW‚Ìê‡ */
	//	if ( (512-16) < src->tmp_angleCCW512 )/*eps*/ /*<= deg_360_to_512(0)*/	//deg_360_to_512(10/*90*/ /*-90*/ /*360*/ /*-10*/
		/* CCW‚Ìê‡ */
		if ( 0==(0x100 & src->tmp_angleCCW512) )/* (deg_360_to_512CCW(180) > src->tmp_angleCCW512) */
		{
			if ( (128+16) < src->tmp_angleCCW512 )/*eps*/ /*<= deg_360_to_512(0)*/	//deg_360_to_512(10/*90*/ /*-90*/ /*360*/ /*-10*/
			{
			//	src->tmp_angleCCW512 = deg_360_to_512(0/*360*/ /*-10*/)/*deg_360_to_512(360)*/;
				src->state++/*=1*/;
				shot_meido3(src);	/* easyˆÈŠO‚Í‘_‚¢’e‚ğŒ‚‚Â */
			}
		}
		break;
	case 1: 	/* ‰E(Î‚ßã)‚ÖˆÚ“®’† */
		if (0 < src->turn_count)
		{
			if (src->x256 >= src->clip_right256) { src->state++/*=2*/;	src->turn_count--; }
		}
		else	/* ‘Şê’† */
		{
			gamen_gai_nara_osimai(src);/* ‰æ–ÊŠO‚È‚ç‚¨‚µ‚Ü‚¢ */
		}
		break;
	case 2: 	/* ‰Eü‚è‚Å‰ñ“]’† */
	//	src->tmp_angleCCW512 += src->turnspeed512;/*fps_factor*/
		/* CCW‚Ìê‡ */
		src->tmp_angleCCW512 -= src->turnspeed512;/*fps_factor*/
		mask512(src->tmp_angleCCW512);
		/* ¶(Î‚ßã)‚ğŒü‚¢‚½‚ç */
	//	if ( (256+16) < src->tmp_angleCCW512 )	//deg_360_to_512(180 /*270*/ /*90+*/ /*180-90-90*/ /*+10*/)
		/* CCW‚Ìê‡ */
		if ( 0!=(0x100 & src->tmp_angleCCW512) )/* (deg_360_to_512CCW(180) < src->tmp_angleCCW512) */
		{
			if ( (256+128-16) > src->tmp_angleCCW512 )	//deg_360_to_512(180 /*270*/ /*90+*/ /*180-90-90*/ /*+10*/)
			{
			//	src->tmp_angleCCW512 = deg_360_to_512(180/*+10*/);
				src->state++/*=3*/;
				shot_meido3(src);	/* easyˆÈŠO‚Í‘_‚¢’e‚ğŒ‚‚Â */
			}
		}
		break;
	case 3: 	/* ¶(Î‚ßã)‚ÖˆÚ“®’† */
		if (0 < src->turn_count)
		{
			if (src->x256 <= src->clip_left256) {	src->state=0;	src->turn_count--;	}
		}
		else	/* ‘Şê’† */
		{
			gamen_gai_nara_osimai(src);/* ‰æ–ÊŠO‚È‚ç‚¨‚µ‚Ü‚¢ */
		}
		break;
	}
	/*ˆÈ‰ºrwingx.c‚Æ“¯‚¶*/
/* CCW‚Ìê‡ */
	src->x256+=((sin512((src->tmp_angleCCW512))*src->speed256)>>8);/*fps_factor*/
	src->y256+=((cos512((src->tmp_angleCCW512))*src->speed256)>>8);/*fps_factor*/
//	src->an im_frame=(deg_512_to_360(src->angle512+deg_360_to_512(270))/10)%36;
//	src->an im_frame = ((((src->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
#if 00
/* [CCW‚Ìê‡(V)] CW‚Ìê‡ */
	src->an im_frame = ((((src->tmp_angleCCW512)&(512-1)))>>6);/*"rw ingx8.png"*/
	/* ‹Œ */
//#else
/* CCW‚Ìê‡ */
	/* V(‚Ü‚¾ì‚Á‚Ä‚È‚¢) */
	//src->an im_frame = ((((src->tmp_angleCCW512)&(512-1)))>>6);/*"rw ingx8.png"*/
	/*–³—–î—‹ŒŒİŠ·*/
	{int aaa512;
		aaa512 = 128+ 512 - src->tmp_angleCCW512;
		mask512(aaa512);
		src->yx_an im_frame = (((aaa512))>>(6));
	}
#endif
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_36_YOUSEI3_1+((((src->tmp_angleCCW512)&(512-1)))>>6);
	}
/*Œ»İd—lã‚¨‚©‚µ‚¢*/
}

/*---------------------------------------------------------
	“G‚ğ’Ç‰Á‚·‚é
---------------------------------------------------------*/

void add_zako_meido3(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_36_YOUSEI3_1;	/*SP_ZAKO*/ /*_14_MEIDO3*/
	add_zako_common(l, h);
//	h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->callback_mover		= move_meido3;
//	h->callback_loser		= lose_meido3;
//	h->callback_hit_enemy	= callback_hit_zako;

	h->speed256 			= t256(1.5);		/*3+difficulty*/ /*‚Í‚â‚·‚¬*/
	h->turnspeed512 		= (7);				/*deg_360_to_512*/ /*(4+difficulty)*/ /*ŠÈ—ª‰»(360“x¨512“x)*/
	if (t256(GAME_WIDTH/2) > h->x256 )
	{
		h->tmp_angleCCW512	= deg_360_to_512CCW(90-10/*360-10*/);
	//	h->x256 			= -t256(32);
		static_last 		= 0;
	}
	else
	{
		h->tmp_angleCCW512	= deg_360_to_512CCW(180+90+10/*360-170*/);
	//	h->x256 			= t256(GAME_WIDTH);
		static_last 		= 1;
	}

//	h->clip_left256 	= (t256(static_last<<7)+t256(static_last<<6)+t256(32)) /*50*/;			/* 32,	224(128+64+32)	*/
//	h->clip_right256	= (t256(static_last<<7)+t256(static_last<<6)+t256(32))+t256(128-32);	/* 128, 320(256+64)   */	 //  /*GAME_WIDTH-50-s->w*/
	h->clip_left256 	= (t256(static_last<<7)+t256(static_last<<6)+t256(32)) /*50*/;			/* 32,	224(128+64+32)	*/
	h->clip_right256	= (t256(static_last<<7)+t256(static_last<<6)+t256(32))+t256(128-32-32); /* 128, 320(256+64)   */	 //  /*GAME_WIDTH-50-s->w*/
	h->state			= (static_last<<1);/*0*/
	h->turn_count		= 4;
}

//	h->x256 				= t256((static_last<<7))+t256((32))+(((s->w128)*(i+1))*t256(0.375))/* 0.375=0.75/2.0 -(s->w*(i+1))*0.5*/;/*‚­‚Á‚Â‚«‚·‚¬*/
//	h->x256 				= t256(10);/*‚­‚Á‚Â‚«‚·‚¬*/
//	h->y256 				= t256(0);/*50*/
//	h->y256 				= t256(10);/*50*/

//	/*h->base.*/h->base_score	= score(/*25*/50*2);
//	/*h->base.*/h->base_hp		= (8*12*8)+(di fficulty<<(3+3)/*4*/)/*5+(di fficulty<<2)*/;/*‚æ‚í‚·‚¬*/
//	/*h->base.*/h->base_hp		= (8* 8*8)+(1/*di fficulty*/<<(2+3)/*4*/)/*5+(di fficulty<<2)*/;/*‘¼‚ª‹­‚¢‚Ì‚Åã‚­‚·‚é*/
//	h->tmp_angleCCW512		= (256+128/*0*/);	/*256+16*/ /*deg_360_to_512(270)*/;

//	int enemy_rank;
//	enemy_rank	= l->user_y;
//
//	static_last++;
//	static_last &= 1;
//
//	destoroy[static_last] = 0;
//


