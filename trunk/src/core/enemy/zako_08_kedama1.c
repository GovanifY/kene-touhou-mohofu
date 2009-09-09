
#include "bullet_object.h"

/*---------------------------------------------------------
		"–Ñ‹Ê1",	"BADGUY",
	-------------------------------------------------------
	–{‰Æ–Ñ‹Ê
---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE base;
	int state;
	int tx256;
	int ty256;
	int speed256;

} KEDAMA1_DATA;

static int level;

#define NUM_OF_ENEMIES (10)

/*---------------------------------------------------------
	“G‚â‚ç‚ê
---------------------------------------------------------*/

static void lose_kedama1(SPRITE *s)
{
//	case SP_ZAKO_08_KEDAMA1:
//	if (rand_percent(10))
	{
		item_create(s, enemy_get_random_item(), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
	}
}

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_kedama1(SPRITE *s)
{
	KEDAMA1_DATA *d = (KEDAMA1_DATA *)s->data;
	switch (d->state)
	{
	case 0:
		if ((s->x256 >= player->x256) ||
			(s->y256 > /*(480-80)*/t256(GAME_HEIGHT/2)/*(272-32)*/ ) )
		{
			d->state = 1;
			s->anim_speed = /*-*/3;	/*‹t“]ƒAƒjƒ‹ÖŽ~‚É•ÏX*/
			d->tx256 = t256(-100);
			d->ty256 = player->y256;
			if (0 < /*d->*/level)
			{
				bullet_create_aka_maru_jikinerai(s, t256(1)+t256((/*d->*/level)>>1) );
			}
			d->speed256 =	/*6*/t256(5)	/**fps_fa ctor*/;
		}
		break;/*??? [***090215 ’Ç‰Á */
	case 1:
		if (s->x256 <= -((s->w128+s->w128)))
		{
			s->type = SP_DELETE;
		}
		break;/*??? [***090215 ’Ç‰Á */
	}
	{	int angle512;
		angle512=atan_512(d->ty256-s->y256-t256(64)/*+(20)*/,d->tx256-s->x256);
		s->x256+=((cos512(angle512)*d->speed256)>>8);
		s->y256+=((sin512(angle512)*d->speed256)>>8);
	}
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

void add_zako_kedama1(STAGE_DATA *l)/*int lv*/
{
	/*data->*/level 	= l->user_y;
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_KEDAMA16_PNG);	//s->anim_speed=5; /*3*/ /*9"ba dguy.png"*/
		s->type 			= SP_ZAKO/*_08_KEDAMA1*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_kedama1;
		s->callback_loser	= lose_kedama1;
		s->anim_frame		= 0;
		switch (i&3/*i%5*/)
		{
		case 0: s->x256 	=				   (ra_nd()&((64*256)-1))/*ra_nd()%40*/; break;
		case 1: s->x256 	= t256(GAME_WIDTH)-(ra_nd()&((64*256)-1))/*ra_nd()%40*/; break; 	// GAME_WIDTH ƒEƒBƒ“ƒhƒE•‚Ì•ÏX
		case 2: s->x256 	= t256(200)+	   (ra_nd()&((64*256)-1))/*ra_nd()%40*/; break;
		case 3: s->x256 	= t256(90)+ 	   (ra_nd()&((64*256)-1))/*ra_nd()%40*/; break;
	//	case 4: s->x256 	= t256(310)+	   (ra_nd()&((64*256)-1))/*ra_nd()%40*/; break;
		}
		s->y256 			= ((ra_nd()&((32*256)-1))-t256(80));//	(ra_nd()%40-90);
		KEDAMA1_DATA *data;
		data				= mmalloc(sizeof(KEDAMA1_DATA));
		s->data 			= data;
		data->base.score	= score(/*50*/5*2)*(level+1);
		data->base.health	= (8*8)+(difficulty<<2)/*(1+(difficulty<<2))*/;/*‚â‚í‚ç‚©‚·‚¬*/
		data->state 		= 0;
		data->tx256 		= player->x256;
		data->ty256 		= player->y256;
	//	data->speed256		= /*fps_fa ctor**/((ra_nd()&((256*2)-1)))*(1+difficulty+level/3);
		data->speed256		= /*fps_fa ctor**/((ra_nd()&((256*2)-1))) + ((difficulty<<8)+ (level<<7/**86*2*/) );
	}
}
#undef NUM_OF_ENEMIES
