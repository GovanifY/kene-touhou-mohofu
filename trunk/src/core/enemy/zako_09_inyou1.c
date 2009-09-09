
#include "bullet_object.h"

/*---------------------------------------------------------
	"‰A—z‹Ê1",		"PLASMABALL",
	-------------------------------------------------------
	(‹Œ)”’–Ñ‹ÊA(601Ž)Â—d¸
	(4*6)‚Å‚Q‚S•C‚ào‚Ä‚­‚é‚ª‚ ‚Ü‚èˆÓ–¡‚Ì‚È‚¢ƒLƒƒƒ‰
---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE base;
	int angle512;	/* Šp“x[‚PŽü‚ª512] */
	int speed256;	/* ‘¬“x */
	int state;					/* ó‘Ô */
//
//	int destx256;
	int limit_y256; 			/* ‰¼‘z’n–Ê */
	int kaiten_houkou;			/* ‰ñ“]•ûŒü */
	int bounds256;				/* ‚Í‚Ë‚½‰ñ” */
	int vy256;					/* c•ûŒü‰Á‘¬“x */
} INYOU1_DATA;


//#define NUM_OF_ENEMIES (4*6)

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_inyou1(SPRITE *s)
{
	INYOU1_DATA *d = (INYOU1_DATA *)s->data;
	switch (d->state)
	{
	case 0:
		s->y256 += d->speed256;/**fps_fa ctor*/
		if (s->y256 >= d->limit_y256)
		{
			s->y256 			= d->limit_y256;
			d->angle512 		= (256); /*(0)*/
			d->kaiten_houkou	= 0;
			d->bounds256		= t256(0);
			d->state			= 1;
			d->vy256			= t256(2);
		}
		break;
	case 1:
		if (0==d->kaiten_houkou)
		{
			d->angle512 += 12; /*rad2 deg512(0.3)*/ /*24*/ /**fps_fa ctor*/ /* rad2 deg512( 0.3)== 24.4461992589151487740904245119933 */
			if (d->angle512 > (256+224)) /*(512)*/
			{
				d->kaiten_houkou	= 1;
				d->bounds256 += t256(0.5);
			}
			s->anim_frame++;
		}
		else
		{
			d->angle512 -= 12; /*rad2 deg512(0.3)*/ /*24*/ /**fps_fa ctor*/ /* rad2 deg512( 0.3)== 24.4461992589151487740904245119933 */
			if (d->angle512 < (256-224)) /*(0)*/
			{
				d->kaiten_houkou	= 0;
			}
			s->anim_frame--;
		}
		s->anim_frame &= (16-1);
		//
		d->limit_y256 -= t256(0.75);/* ã‚ª‚é */
	//	s->x256 = d->destx256 + ((sin512((d->angle512)))<<4);
	//	s->y256 = d->limit_y256 + (cos512((d->angle512))*((32-((d->kaiten_kaisu)))));/*(1+6-d->kaiten_kaisu)*/
	//	+ (cos512((d->angle512))*((32-((d->kaiten_kaisu)))));/*(1+6-d->kaiten_kaisu)*/
		d->vy256 += t256(1);
		s->y256 += d->vy256;
		if (s->y256 >= d->limit_y256)
		{
			d->vy256 = -t256(8) + d->bounds256;
		}
		if (
		//	(d->angle512 > -57/*rad2 deg512(-0.7)*/ ) &&	/* rad2 deg512(-0.7)==-57.0411316041353471395443238613177 */
		//	(d->angle512 <	32/*rad2 deg512( 0.4)*/ ) &&	/* rad2 deg512( 0.4)== 32.5949323452201983654538993493244 */
			(d->bounds256 > t256(5)/*==5*/))
		{
			if ( 0==(ra_nd()&(16-1)) ) /*Šm—¦ã‚°‚½B1/16©1/20*/ /*%20*/
			{
				item_create(s,
					(SP_ITEM_01_P008+(ra_nd()&1))/*50%(SP_ITEM_01_P008 or SP_ITEM_02_BOMB)*/
					//66%==SP_ITEM_02_BOMB or 33%==SP_ITEM_01_P008	 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),
				, 1, ITEM_MOVE_FLAG_06_RAND_XY);
			}
			if ( 0==(ra_nd()&(16-1)) ) /*Šm—¦ã‚°‚½B[1/16]©[1/20]*/ /*%20*/
			{
				bullet_create_offset_dan_type(s,
					t256(5),
					ANGLE_JIKI_NERAI_DAN,
					t256(0),
					t256(0),
					BULLET_HARI32_01_AKA);
			}
			d->state=2;
		}
		break;
	case 2:
	//	d->limit_y256 -= t256(4)/*t256(50)*/;/* ã‚ª‚é©‰º‚ª‚é */
	//	if (d->limit_y256 > (GAME_HEIGHT) )
		if (d->limit_y256 < (0) )
		{	s->type = SP_DELETE;}
		else
		{	d->state=0;}
		break;
	}
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

void add_zako_inyou1(STAGE_DATA *l)/*int lv*/
{
	int yyy;
	int xxx;
	for (yyy=0; yyy<4; yyy++)
	{
		for (xxx=0; xxx<5/*6*/; xxx++)
		{
			SPRITE *s;
			s					= sprite_add_res(BASE_INYOU1_16_PNG); 	//s->anim_speed=0/*2*/ /*1*/;/*11"pl asmaball.png"*/
			s->type 			= SP_ZAKO;	/*_09_INYOU1*/
			s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
			s->callback_mover	= move_inyou1;
			s->anim_frame		= (ra_nd()&(16-1));/*(0) (ra_nd()&(8-1)) (ra_nd()%s->frames)*/
			INYOU1_DATA *data;
			data				= mmalloc(sizeof(INYOU1_DATA));
			s->data 			= data;
			data->base.score	= score(25*2);
			data->base.health	= (8*8)+(difficulty<<2);	/*‚â‚í‚ç‚©‚·‚¬*/	/*(2+(difficulty<<2))*/
			data->state 		= 0;
			data->speed256		= ((ra_nd()&(512-1)))+128;  	/*((ra_nd()&(256-1)))*3*/ /*3*((dou ble)ra_nd()/RAND_MAX+1)*/
			data->limit_y256	= t256(128)-((yyy<<(5+8))); 	/*t256(200)-(j*t256(40))*/ 	/* è‡’l */
			s->x256 			= t256( 32)+((xxx<<(6+8))); 	/*t256( 40)+(i*t256(35))*/
			s->y256 			= (-((s->w128+s->w128))-(xxx*t256(16))-(yyy*t256(64)));
								/*(-((s->w128+s->w128))-(xxx*t256(20))-(yyy*t256(50)))*/
		}
	}
}
//#undef NUM_OF_ENEMIES
