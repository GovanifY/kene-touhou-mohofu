
#include "enemy.h"

/*---------------------------------------------------------
	"メイド2",		"ZATAK",
	-------------------------------------------------------
	rwingx_zatak (赤い妖精)
	左右から現れ、自機狙い弾を打った後、真下へ直進してくる。
---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE b;
	/*dou ble*/int angle512;
	/*dou ble*/int speed256;
	int state;
//	int level;
//
	/*dou ble*/int wait;
} MEIDO2_DATA;

static int data_level256;
#define NUM_OF_ENEMIES (12)

/*---------------------------------------------------------

---------------------------------------------------------*/

static void lose_meido2(SPRITE *s)
{
	item_create(s, (enemy_get_random_item()), 1, ITEM_MOVE_FLAG_06_RAND_XY);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_meido2(SPRITE *s)
{
	MEIDO2_DATA *d=(MEIDO2_DATA *)s->data;
	d->wait += 1/*fps_fa ctor*/;
	switch (d->state)
	{
	case 0:
		if (d->wait >= 40)
		{
			d->state=1;
		}
		break;
	case 1:
		d->speed256 -= t256(0.02/**fps_fa ctor*/);
		if (d->speed256<=0)
		{
			d->speed256=0;
			d->state=2;
		}
		break;
	case 2:
//efine HARIDAN_SPEED ( /*speed256*/(t256(5)+((data_level/*d->level*/)<<8) ) )
#define HARIDAN_SPEED ( /*speed256*/t256(4/*5*/) + (data_level256/*d->level*/)	)
		if (d->angle512 < deg_360_to_512(90))
		{
			d->angle512 += deg_360_to_512(3)/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
			if (d->angle512 >= deg_360_to_512(89))
			{
				d->angle512 = deg_360_to_512(90);
				d->state=3;
				if (data_level256/*d->level*/)
				{
					bullet_create_hari_dan180(s, HARIDAN_SPEED, ANGLE_JIKINERAI_DAN, t256(0), t256(0));
				}
			}
		}
		else
		{
			d->angle512 -= deg_360_to_512(3)/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
			if (d->angle512 <= deg_360_to_512(91))
			{
				d->angle512 = deg_360_to_512(90);
				d->state=3;
				if (data_level256/*d->level*/)
				{
					bullet_create_hari_dan180(s, HARIDAN_SPEED, ANGLE_JIKINERAI_DAN, t256(0), t256(0));
				}
			}
		}
		break;
	case 3: /*特攻*/
		d->speed256 += t256(0.2)/*0.3*/;/*加速*/
		if (d->speed256 >= t256(2.5)/*3*/ /*(6+d->level)*/)
		{
			d->speed256 = t256(2.5)/*3*/ /*(6+d->level)*/;/* 加速最大値 */
			d->state = 4;
		}
		break;
	case 4:
#if 0
		if ()
		{
			if ()
			{
			}else
			{
			}
		}
#endif
		if (s->y256 < 0)
		{	s->type=SP_DELETE;}
		break;
	}
	/*以下rwingx.cと同じ*/
	s->x256+=((cos512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
	s->y256+=((sin512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
//	s->anim_frame=(deg_512_to_360(d->angle512+deg_360_to_512(270))/10)%36;
//	s->anim_frame = ((((d->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
	s->anim_frame = ((((d->angle512)&(512-1)))>>6);/*"rw ingx8.png"*/
}
/*
(※１):大昔のversionからズレまくってるが、fps_fa ctorが一定しないから、
fps_fa ctorを入れようがズレるのはKETM自体の仕様。
*/

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_meido2(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	data_level256=(lv<<7);	//	data->level=lv;
	int i;
	for (i=0;i<NUM_OF_ENEMIES;i++)
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_AKA_MEIDO08_PNG); 	//s->anim_speed=0;/*37"rw ingx8.png""rw ingx.png"*/
		s->type 			= SP_ZAKO/*_13_MEIDO2*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_meido2;
		s->callback_loser	= lose_meido2;
//		s->anim_frame		= 0;
		MEIDO2_DATA *data;
		data				= mmalloc(sizeof(MEIDO2_DATA));
		s->data 			= data;
		data->b.score		= score(25*2);
		data->b.health		= 1+(difficulty<<2);
		data->state 		= 0;
		data->wait			= 0;
		data->speed256		= /*t256_to_dou ble*/((ra_nd()&((256*4)-1)))/*2.0*(((dou ble)ra_nd()/RAND_MAX)*2)*/;
		if (i<6)
		{
			data->angle512	= deg_360_to_512(10);
			s->x256 		= -((s->w)<<8);
		}
		else
		{
			data->angle512	= deg_360_to_512(170);
			s->x256 		= t256(GAME_WIDTH);
		}
		s->y256 = t256(50);
	}
}
#undef NUM_OF_ENEMIES
