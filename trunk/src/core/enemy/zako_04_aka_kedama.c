
#include "enemy.h"

/*---------------------------------------------------------
		"Ô–Ñ‹Ê1",		"MINE",
	-------------------------------------------------------
	‚®‚é‚®‚é‚»‚Ìê‚Å“®‚©‚È‚¢‚ÅA‰ñ“]ƒAƒjƒ‚µ‚Ä‚Ä
	‚Ù‚Á‚Æ‚­‚Æ8‚Â‚É•ª—ô‚µ‚Ä
	ÅŒã‚É‚‘¬’eŒ‚‚Á‚Ä—ˆ‚é‚â‚Â
---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE b;
	int state;
	int tx256;
	int ty256;
	int time_out;
} AKA_KEDAMA1_DATA;

static int level;

/* ‹¤—L•Ï” / shered */
#define mmm_angle512 state

/*---------------------------------------------------------
	“G‚â‚ç‚ê
---------------------------------------------------------*/

static void lose_aka_kedama1(SPRITE *s)
{
	/* ”š”­‚³‚¹‚½‚Ù‚¤‚ªƒAƒCƒeƒ€‚ª‰Ò‚°‚é */
	item_create(/*zzz*/ s, SP_ITEM_00_P001, 1, ITEM_MOVE_FLAG_06_RAND_XY);
}

/*---------------------------------------------------------
	“GˆÚ“®(•ª—ôŒã)
---------------------------------------------------------*/

static void move_aka_kedama1_2nd(SPRITE *s)
{
	AKA_KEDAMA1_DATA *d=(AKA_KEDAMA1_DATA *)s->data;
//	if (/*0 <*/ /*d->*/level)	/* easy‚Í‘¬‚¢‚ñ‚¾‚©‚çê‡•ª‚¯‚µ‚È‚¢ */
	{
		/*
		0  0/100   0/65536
		1  1/90  728/65536
		2  1/80  819/65536
		3  1/70  936/65536
		*/
		static const Uint16 kakuritu_tbl[4] =
		{ 0, 728, 819, 936 };
	//	if (0==(ra_nd()%(100-/*d->*/level*10)))
		if (kakuritu_tbl[/*d->*/level] > (ra_nd()&(65536-1)))
		{
			bullet_create_aka_maru_jikinerai(s, t256(2.5)+(/*d->*/level<<6) );/*‚‘¬’e*/	/*t256(3+d->level)*/
		}
	}
	s->x256+=((cos512((d->mmm_angle512))*t256(3))>>8)/**fps_fa ctor*/;
	s->y256+=((sin512((d->mmm_angle512))*t256(3))>>8)/**fps_fa ctor*/;
	if ((s->x256<0-((s->w128+s->w128)))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256<0-((s->h128+s->h128)))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type=SP_DELETE;
	}
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é(•Ïg)
---------------------------------------------------------*/

static void add_local_aka_kedama1_2nd(SPRITE *src/*, int lv*/)
{
//	data->level 	= lv;
	int i;
	for (i=0; i<8; i++)/*•ª—ô*/
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_AKA_KEDAMA08_PNG);	s->anim_speed=i/*6 2*/;/*19"12 side.png"*/
		s->type 			= SP_ZAKO/*_04_AKA_KEDAMA1*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_aka_kedama1_2nd;
		s->callback_loser	= lose_aka_kedama1;
		s->anim_frame		= 0;
		s->x256 			= src->x256;
		s->y256 			= src->y256;
		AKA_KEDAMA1_DATA *data;
		data				= mmalloc(sizeof(AKA_KEDAMA1_DATA));
		s->data 			= data;
		data->b.score		= score(10*2)*(1+level/*lv*/);
		data->b.health		= (1+level/*lv*/)+(difficulty<<4);
		data->mmm_angle512	= (i<<6);//  (/*360*/512/8)*i;
	}
}

/*---------------------------------------------------------
	“GˆÚ“®(–{‘Ì)
---------------------------------------------------------*/

static void move_aka_kedama1_1st(SPRITE *s)
{
	AKA_KEDAMA1_DATA *d=(AKA_KEDAMA1_DATA *)s->data;
	switch (d->state)
	{
	case 0: /* move */
		{
		/*dou ble*/int angle512;
			angle512=atan_512(d->ty256-s->y256,d->tx256-s->x256);
			s->x256 += ((cos512(angle512)*t256(1.5))>>8)/**fps_fa ctor*/;
			s->y256 += ((sin512(angle512)*t256(1.5))>>8)/**fps_fa ctor*/;
		}
	//	if ((d->tx-1 <= s->x) && (s->x <= d->tx+1) &&
	//		(d->ty-1 <= s->y) && (s->y <= d->ty+1))
		if ((d->tx256-256 <= s->x256) && (s->x256 <= d->tx256+256) &&
			(d->ty256-256 <= s->y256) && (s->y256 <= d->ty256+256))
		{
			d->state	= 1;
			d->time_out = 80;
		}
		break;
	case 1:
		d->time_out--;
		if (31 > d->time_out)
		{
			d->state		=2;
			s->anim_speed	=0;
		}
		break;
	case 2:
		d->time_out--;
		if (1 > d->time_out)
		{
			add_local_aka_kedama1_2nd(s/*,d->level*/);/* •Ïg •ª—ô */
			s->type=SP_DELETE;	/* –{‘Ì‚Í‚¨‚µ‚Ü‚¢ */
		}
		break;
	}
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é(–{‘Ì)
---------------------------------------------------------*/

void add_zako_aka_kedama1(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
	/*data->*/level 	= lv;
//
	SPRITE *s;
	s					= sprite_add_res(BASE_AKA_KEDAMA08_PNG);	//s->anim_speed 	= 1;/*19"12 side.png"*/
	s->type 			= SP_ZAKO/*_04_AKA_KEDAMA1*/;
	s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	s->callback_mover	= move_aka_kedama1_1st;
	s->callback_loser	= lose_aka_kedama1;
	s->anim_frame		= 0;
//	s->x256 			= t256(ra_nd()%(GAME_WIDTH+10))-t256(10);
	s->x256 			= ((ra_nd()&((256*256)-1)))+((ra_nd()&((256*64)-1)))+t256(32);
	s->y256 			= t256(-40);
	AKA_KEDAMA1_DATA *data;
	data				= mmalloc(sizeof(AKA_KEDAMA1_DATA));
	s->data 			= data;
	data->b.score		= score(30*2);
	data->b.health		= 12+lv*2+(difficulty<<2);
	data->tx256 		= ((ra_nd()&((256*256)-1)))+t256(64);/*320?*/ /*t256(ra_nd()%270)+t256(50)*/
//	data->ty256 		= ((ra_nd()&((256*256)-1)))+t256(16);/*350?*/ /*t256(ra_nd()%300)+t256(50)*/
	data->ty256 		= ((ra_nd()&((256*128)-1)))+t256(16);/*350?*/ /*t256(ra_nd()%300)+t256(50)*/
	data->state 		= 0;
}
