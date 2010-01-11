
#include "bullet_object.h"

/*---------------------------------------------------------
		"ˆÍ—d‰ö1",		"CUBE",
	-------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	int radius256;
	int angle512;
	int flag0;
	int flag1;
	int wait1;
} KAKOMI1_DATA;
//	int enemy_rank; 	/* Ý’èƒtƒ@ƒCƒ‹‚©‚ç‚Ì“G‚Ì‹­‚³ */

#define NUM_OF_ENEMIES (16)

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_kakomi1(SPRITE *src)
{
	KAKOMI1_DATA *data = (KAKOMI1_DATA *)src->data;
	data->wait1 += 1/*fps_fa ctor*/;
	if (data->wait1<500)
	{
		if (!data->flag1)
		{
			data->radius256 += t256(1)/*fps_fa ctor*/;
			if (data->radius256 > t256(150))	/* Å‘å”¼Œa */
			{
				data->flag1=1;
			}
		}
		else
		{
			data->radius256 -= t256(1)/*fps_fa ctor*/;
			if (data->radius256 < t256(32)/*<=10*/) /* Å¬”¼Œa */
			{
				data->flag1=0;
			}
		}
		data->angle512 -= /*deg_360_to_512(3)*/(1/*2*/)/**fps_fa ctor*/;/* deg_360_to_512(3) ‘¬‚·‚¬‚é */
		mask512(data->angle512);	//if (data->angle360<0) 	{	data->angle360+=360;}
		if (0==(ra_nd()&(512-1)/*%500*/))/* 1/512 © 1/500 ‚ÌŠm—¦‚Å’e‘Å‚Â */
		{
			bullet_create_aka_maru_jikinerai(src, t256(1));
		}
		if (0==(ra_nd()&(512-1)/*%600*/))/* 1/512 © 1/600 ‚ÌŠm—¦Šm—¦‚ÅƒAƒCƒeƒ€o‚· */
		{
			item_create(src, SP_ITEM_00_P001, 1, ITEM_MOVE_FLAG_06_RAND_XY);
		}
	}
	else
	{
		data->radius256 += t256(2)/**fps_fa ctor*/;
		if (data->radius256 > t256(350))
		{
			src->type = SP_DELETE;	/* ‚¨‚µ‚Ü‚¢ */
		}
	}
	if (SP_DELETE != src->type)
	{
	//	src->an im_frame	= ((data->wait1&0x10)>>4);
		src->type			= TEKI_54_CHOU1+((data->wait1&0x10)>>4);
	}
/* CCW‚Ìê‡ */
	src->x256=((sin512((data->angle512))*(data->radius256))>>8)+t256(GAME_WIDTH/2);
	src->y256=((cos512((data->angle512))*(data->radius256))>>8)+t256(GAME_HEIGHT/2);
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

void add_zako_kakomi1(STAGE_DATA *l)/*int lv*/
{
	int enemy_rank;
	enemy_rank	= l->user_y;
//
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
//		s						= sp rite_add_res(BASE_GREAT_FAIRY02_PNG);	//s->anim_speed=5;/*1*/ /*16"cu be.png")*/
		s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG); 	//s->anim_speed=5;/*1*/ /*16"cu be.png")*/
		s->type 				= /*SP_CHUU*/TEKI_54_CHOU1/*_11_KAKOMI1*/;
//		s->type 				= SP_ZAKO/*_11_KAKOMI1*/;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_kakomi1;
		s->callback_hit_enemy	= callback_hit_zako;
//		s->an im_frame			= 0/*i%s->frames*/;
		KAKOMI1_DATA *data;
		data					= mmalloc(sizeof(KAKOMI1_DATA));
		s->data 				= data;
		/*data->base.*/s->base_score		= score(15*2)*(1+enemy_rank);
		/*data->base.*/s->base_health		= 30+enemy_rank+(difficulty<<2);/*1+lv+(difficulty<<2)*/
		data->radius256 		= t256(350);
		data->angle512			= (i<<5);//  /*360*/(512/16)*i;
		data->flag0 			= 1;
		data->flag1 			= 0;
		data->wait1 			= 0;
//		data->enemy_rank			= enemy_rank;
	}
}
#undef NUM_OF_ENEMIES
