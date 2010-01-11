
#include "bullet_object.h"

/*---------------------------------------------------------
	"—Î–Ñ‹Ê1",		"XEV",
	-------------------------------------------------------
	—Î–Ñ‹Ê	’P‘Ì‚Å“Ë‚Áž‚ñ‚Å‚­‚éB
---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	int state;
	int tx256;
	int ty256;
	int enemy_rank;
} MIDORI_KEDAMA1_DATA;

#define NUM_OF_ENEMIES (3)

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_midori_kedama1(SPRITE *src)
{
	int speed256 = t256(2.5/*0*/);
	MIDORI_KEDAMA1_DATA *data = (MIDORI_KEDAMA1_DATA *)src->data;
	switch (data->state)
	{
	case 0:
		speed256 = t256(2.5/*3*/);
		if ((src->x256 >= player->x256)||
			(src->y256 > t256(GAME_HEIGHT-48) ) )/*480-80*/
		{
			data->state 	= 1;
			data->tx256 	= t256(-100);
		//	data->ty256 	= player->y256-t256(48);
			data->ty256 	= player->y256-t256(48);
			if (data->enemy_rank)
			{	bullet_create_aka_maru_jikinerai(src, t256(1)+((data->enemy_rank)<<8) );}
			{
				item_create(src, enemy_get_random_item(), 1, ITEM_MOVE_FLAG_06_RAND_XY);
			}
		}
		break;/* ??? [***20090210	’Ç‰Á  */
	case 1:
		speed256 = t256(3.5/*6*/);
		if (src->x256 <= -((src->w128+src->w128)))
		{
			src->type = SP_DELETE;	/* ‚¨‚µ‚Ü‚¢ */
		}
		break;/* ??? [***20090409	’Ç‰Á  */
	}
	{	int angle512;
		angle512=atan_512(data->ty256-src->y256,data->tx256-src->x256);
/* CCW‚Ìê‡ */
		src->x256+=((sin512(angle512)*speed256)>>8)/**fps_fa ctor*/;
		src->y256+=((cos512(angle512)*speed256)>>8)/**fps_fa ctor*/;
	}
//
	src->m_angleCCW512 += 5;
	mask512(src->m_angleCCW512);
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

void add_zako_midori_kedama1(STAGE_DATA *l)/*int lv*/
{
	int enemy_rank; 	enemy_rank	= l->user_y;
//
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
//		s						= sp rite_add_res(BASE_MIDOORI_KEDAMA16_PNG);	//s->anim_speed=1;	/*15"cr usher.png"*/	//		s->anim_speed	= /*-*/1;	/*‹t“]ƒAƒjƒ‹ÖŽ~‚É•ÏX*/
		s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG); 		//s->anim_speed=1;	/*15"cr usher.png"*/	//		s->anim_speed	= /*-*/1;	/*‹t“]ƒAƒjƒ‹ÖŽ~‚É•ÏX*/
		s->type 				= /*SP_ZAKO*/TEKI_60_AKA_KEDAMA/*_06_MIDORI_KEDAMA1*/;
//		s->type 				= SP_ZAKO/*_06_MIDORI_KEDAMA1*/;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_midori_kedama1;
		s->callback_hit_enemy	= callback_hit_zako;
//		s->an im_frame			= 0;
		switch (ra_nd()&(4-1)/*%3*/)
		{
		case 0: s->x256 		=							  (ra_nd()&((32*256)-1))/*(ra_nd()%20)*/; break;
		case 1: s->x256 		= t256(GAME_WIDTH/3)		 +(ra_nd()&((32*256)-1))/*(ra_nd()%20)*/; break;
		case 2: s->x256 		= t256(GAME_WIDTH  )		 -(ra_nd()&((32*256)-1))/*(ra_nd()%20)*/; break;
		case 3: s->x256 		= t256(GAME_WIDTH/2)+t256(10)+(ra_nd()&((32*256)-1))/*(ra_nd()%20)*/; break;
		}
		s->y256 				= (ra_nd()&((32*256)-1))/*(ra_nd()%20)*/-t256(50);
		MIDORI_KEDAMA1_DATA *data;
		data					= mmalloc(sizeof(MIDORI_KEDAMA1_DATA));
		s->data 				= data;
		/*data->base.*/s->base_score		= score(5*2);
		/*data->base.*/s->base_health		= (8*8)+(difficulty<<2)/*(1+(difficulty<<2))*/;/*‚â‚í‚ç‚©‚·‚¬*/
		data->state 			= 0;
		data->tx256 			= player->x256;
	//	data->ty256 			= player->y256; 		/* “ÁU‹ÖŽ~ */
		data->ty256 			= ((player->y256)>>1);
		data->enemy_rank		= enemy_rank;
	}
}
#undef NUM_OF_ENEMIES
