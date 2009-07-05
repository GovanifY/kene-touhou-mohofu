
#include "enemy.h"

/*---------------------------------------------------------
		"ê¬ódê∏1",		"FAIRY",
	-------------------------------------------------------
	user_x: 	ë¨ìx256[speed256]
	user_y: 	â°à íu[dots]
---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE b;
	int angleL512;
	int speed256;
	int state;
	int time_out;
} AO_YOUSEI1_DATA;


/*---------------------------------------------------------
	ìGÇ‚ÇÁÇÍ
---------------------------------------------------------*/

static void lose_ao_yousei1(SPRITE *s)
{
//	case SP_ZAKO_16_AO_YOUSEI1:
	//	if (rand_percent(30))	{	put_item=(c->x, c->y, SP_ITEM_00_P001&0xff);}
	//item_create(s, (((ra_nd()&0x07)&0xff)|SP_ITEM_00_P001), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
	item_create(s, (enemy_get_random_item()), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
}

/*---------------------------------------------------------
	ìGà⁄ìÆ
---------------------------------------------------------*/

static void move_ao_yousei1(SPRITE *s)
{
	AO_YOUSEI1_DATA *d=(AO_YOUSEI1_DATA *)s->data;
//
	d->time_out--;
//	if (1 > d->time_out)
//	{
//		s->type=SP_DELETE;
//	}
	switch (d->state)
	{
	case 0: 	/* ìoèÍ çUåÇ */
		d->speed256--;
		if (1 > d->speed256)
		{
			d->speed256 = 1;
		}
		switch (d->time_out)
		{
		case 30:
			d->state++;
			break;
		case 50:
			if (difficulty) {	bullet_create_n_way_dan_type(s, (t256(2.0)+((difficulty)<<6)), ANGLE_JIKINERAI_KISUDAN, BU_TYPE01_KUGEL_PNG, 8);	}	/*Ç»ÇÈÇ◊Ç≠ã§í âª*/
			break;	/*(t256(3.0)+((difficulty)<<6))*/	/*((difficulty<<(1+8)))*/
					/*t256((difficulty<<2))*/ /*5*/ 	/*"kugel.png", 0*/
					/*at an2(player->y-s->y+player->h/2,player->x-s->x-player->w/2)*/
		case 70:	/*not_break;*/
		case 120:	/*not_break;*/
		case 170:	/*not_break;*/
		case 220:
			if (difficulty) {	bullet_create_aka_maru_jikinerai(s, (512-d->time_out)+t256(difficulty));	}
			break;
		case 250:/* ì|ÇπÇÈÇÊÇ§Ç…Ç∑ÇÈ */
			d->b.health 	= (1+(difficulty)); 	/*(3+(difficulty))*/	/*1+(difficulty<<2)*/
			break;
		}
		break;
	case 1: 	/* ó£íE */
		d->speed256 += 2;
		d->angleL512 += 2;
		if (255 < d->angleL512)
		{
			d->angleL512 = 255;
		}
		if (1 > s->y256)
		{
			s->type=SP_DELETE;
		}
		break;
	}
	/*à»â∫rwingx.cÇ∆ìØÇ∂*/
	s->x256+=((sin512((d->angleL512))*d->speed256)>>8)/**fps_fa ctor*/;
	s->y256+=((cos512((d->angleL512))*d->speed256)>>8)/**fps_fa ctor*/;
//
	s->anim_frame = (8+((d->time_out>>2)&(8-1)));
}

/*---------------------------------------------------------
	ìGÇí«â¡Ç∑ÇÈ
---------------------------------------------------------*/

void add_zako_ao_yousei1(STAGE_DATA *l)/*int lv*/
{
	SPRITE *s;
	s					= sprite_add_res(BASE_AO_YOUSEI24_PNG);/*7"fairy.png"*/
	s->type 			= SP_ZAKO/*_16_AO_YOUSEI1*/;
	s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	s->callback_mover	= move_ao_yousei1;
	s->callback_loser	= lose_ao_yousei1;
	s->anim_speed		= 0/*4*/;
	AO_YOUSEI1_DATA *data;
	data				= mmalloc(sizeof(AO_YOUSEI1_DATA));
	s->data 			= data;
	s->y256 			= t256(-30);
	s->x256 			= ((l->user_y)<<8);/*lv*t256(35)+t256(40)*/
	data->time_out		= 320;
	data->speed256		= ((l->user_x));			/*t256(0.5)*/	/*+0.15*difficulty*/ /*0.7+0.3*difficulty*/
	data->angleL512 	= deg_360_to_512(0);
	data->state 		= 0;
	data->b.score		= score(20*2);
	data->b.health		= 999;/* ìoèÍéûÇÕì|ÇπÇ»Ç¢ */	/*3+(difficulty)*/	/*1+(difficulty<<2)*/
}
