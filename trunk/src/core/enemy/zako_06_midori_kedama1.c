
#include "enemy.h"

/*---------------------------------------------------------
		"—Î–Ñ‹Ê1",		"XEV",
	-------------------------------------------------------
	—Î–Ñ‹Ê	’P‘Ì‚Å‚Å“Ë‚Áž‚ñ‚Å‚­‚éB
---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE b;
	int state;
	int tx256;
	int ty256;
	int level;
} MIDORI_KEDAMA1_DATA;

#define NUM_OF_ENEMIES (3)

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_midori_kedama1(SPRITE *s)
{
	/*dou ble*/int speed256=t256(2.5/*0*/);
	MIDORI_KEDAMA1_DATA *d=(MIDORI_KEDAMA1_DATA *)s->data;
	switch (d->state)
	{
	case 0:
		speed256 = t256(2.5/*3*/);
		if ((s->x256 >= player->x256)||
			(s->y256 > t256(GAME_HEIGHT-48) ) )/*480-80*/
		{
			d->state		= 1;
			d->tx256		= t256(-100);
		//	d->ty256		= player->y256-t256(48);
			d->ty256		= player->y256-t256(48);
			if (d->level)
			{	bullet_create_aka_maru_jikinerai(s, t256(1)+((d->level)<<8) );}
			{
				item_create(s, enemy_get_random_item(), 1, ITEM_MOVE_FLAG_06_RAND_XY);
			}
		}
		break;/* ??? [***20090210	’Ç‰Á  */
	case 1:
		speed256 = t256(3.5/*6*/);
		if (s->x256 <= -((s->w128+s->w128)))
		{
			s->type=SP_DELETE;
		}
		break;/* ??? [***20090409	’Ç‰Á  */
	}
	{	/*dou ble*/int angle512;
		angle512=atan_512(d->ty256-s->y256,d->tx256-s->x256);
		s->x256+=((cos512(angle512)*speed256)>>8)/**fps_fa ctor*/;
		s->y256+=((sin512(angle512)*speed256)>>8)/**fps_fa ctor*/;
	}
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

void add_zako_midori_kedama1(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_MIDOORI_KEDAMA16_PNG);	//s->anim_speed=1;	/*15"cr usher.png"*/	//		s->anim_speed	= /*-*/1;	/*‹t“]ƒAƒjƒ‹ÖŽ~‚É•ÏX*/
		s->type 			= SP_ZAKO/*_06_MIDORI_KEDAMA1*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_midori_kedama1;
		s->anim_frame		= 0;
		switch (ra_nd()&(4-1)/*%3*/)
		{
		case 0: s->x256 	=							  (ra_nd()&((32*256)-1))/*(ra_nd()%20)*/; break;
		case 1: s->x256 	= t256(GAME_WIDTH/3)		 +(ra_nd()&((32*256)-1))/*(ra_nd()%20)*/; break;
		case 2: s->x256 	= t256(GAME_WIDTH  )		 -(ra_nd()&((32*256)-1))/*(ra_nd()%20)*/; break;
		case 3: s->x256 	= t256(GAME_WIDTH/2)+t256(10)+(ra_nd()&((32*256)-1))/*(ra_nd()%20)*/; break;
		}
		s->y256 			= (ra_nd()&((32*256)-1))/*(ra_nd()%20)*/-t256(50);
		MIDORI_KEDAMA1_DATA *data;
		data				= mmalloc(sizeof(MIDORI_KEDAMA1_DATA));
		s->data 			= data;
		data->b.score		= score(5*2);
		data->b.health		= (8*8)+(difficulty<<2)/*(1+(difficulty<<2))*/;/*‚â‚í‚ç‚©‚·‚¬*/
		data->state 		= 0;
		data->tx256 		= player->x256;
	//	data->ty256 		= player->y256; 		/* “ÁU‹ÖŽ~ */
		data->ty256 		= ((player->y256)>>1);
		data->level 		= lv;
	}
}
#undef NUM_OF_ENEMIES
