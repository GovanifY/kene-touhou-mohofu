
#include "enemy.h"

/*---------------------------------------------------------
		"ˆÍ—d‰ö1",		"CUBE",
	-------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE b;
	/*dou ble*/int radius256;
	/*dou ble*/int angle512;
	int flag0;
	int flag1;
	int flag2;
//	int level;
} KAKOMI1_DATA;

#define NUM_OF_ENEMIES (16)

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_kakomi1(SPRITE *s)
{
	KAKOMI1_DATA *d=(KAKOMI1_DATA *)s->data;
	d->flag2 += 1/*fps_fa ctor*/;
	if (d->flag2<500)
	{
		if (!d->flag1)
		{
			d->radius256 += t256(1)/*fps_fa ctor*/;
			if (d->radius256 > t256(150))	/* Å‘å”¼Œa */
			{
				d->flag1=1;
			}
		}
		else
		{
			d->radius256 -= t256(1)/*fps_fa ctor*/;
			if (d->radius256 < t256(32)/*<=10*/)	/* Å¬”¼Œa */
			{
				d->flag1=0;
			}
		}
		d->angle512 -= /*deg_360_to_512(3)*/(1/*2*/)/**fps_fa ctor*/;/* deg_360_to_512(3) ‘¬‚·‚¬‚é */
		mask512(d->angle512);	//if (d->angle360<0)		{	d->angle360+=360;}
		if (0==(ra_nd()&(512-1)/*%500*/))/* 1/512 © 1/500 ‚ÌŠm—¦‚Å’e‘Å‚Â */
		{
			bullet_create_aka_maru_jikinerai(s, t256(1));
		}
		if (0==(ra_nd()&(512-1)/*%600*/))/* 1/512 © 1/600 ‚ÌŠm—¦Šm—¦‚ÅƒAƒCƒeƒ€o‚· */
		{
			item_create(s, SP_ITEM_00_P001, 1, ITEM_MOVE_FLAG_06_RAND_XY);
		}
	}
	else
	{
		d->radius256 += t256(2)/**fps_fa ctor*/;
		if (d->radius256 > t256(350))
		{	s->type=SP_DELETE;}
	}
	s->x256=((cos512((d->angle512))*(d->radius256))>>8)+t256(GAME_WIDTH/2); 	//ƒEƒBƒ“ƒhƒE•‚Ì•ÏX
	s->y256=((sin512((d->angle512))*(d->radius256))>>8)+t256(GAME_HEIGHT/2);
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

void add_zako_kakomi1(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_GREAT_FAIRY02_PNG); 	s->anim_speed=5;/*1*/ /*16"cu be.png")*/
		s->type 			= SP_ZAKO/*_11_KAKOMI1*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_kakomi1;
		s->anim_frame		= 0/*i%s->frames*/;
		KAKOMI1_DATA *data;
		data				= mmalloc(sizeof(KAKOMI1_DATA));
		s->data 			= data;
		data->b.score		= score(15*2)*(1+lv);
		data->b.health		= 30+lv+(difficulty<<2);/*1+lv+(difficulty<<2)*/
		data->radius256 	= t256(350);
		data->angle512		= (i<<5);//  /*360*/(512/16)*i;
		data->flag0 		= 1;
		data->flag1 		= 0;
		data->flag2 		= 0;
//		data->level 		= lv;
	}
}
#undef NUM_OF_ENEMIES
