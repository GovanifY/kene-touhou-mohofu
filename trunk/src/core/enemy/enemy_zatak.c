#include "enemy.h"

/*rwingx_zatak*/

typedef struct
{
	ENEMY_BASE b;
	/*double*/int angle512;
	double speed;
	int state;
	int level;
//
	double wait;
} ZATAK_DATA;
/*
左右から現れ、自機狙い弾を打った後
真下へ直進してくる。

*/

#define NUM_OF_ENEMIES (12)
static void enemy_zatak_move(SPRITE *s)
{
	ZATAK_DATA *d=(ZATAK_DATA *)s->data;

	d->wait+=fps_factor;
	switch (d->state)
	{
	case 0:
		if (d->wait>=40)
		{
			d->state=1;
		}
		break;
	case 1:
		d->speed-=(0.02*fps_factor);
		if (d->speed<=0)
		{
			d->speed=0;
			d->state=2;
		}
		break;
	case 2:
		if (d->angle512 < deg_360_to_512(90))
		{
			d->angle512 += deg_360_to_512(3)/**fps_factor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
			if (d->angle512 >= deg_360_to_512(89))
			{
				d->angle512 = deg_360_to_512(90);
				d->state=3;
				if (d->level)
				{
					bullet_create_haridan180(s, ANGLE_JIKINERAI_DAN, (/*speed256*/(t256(5)+((d->level)<<8))), 0, 0);
				}
			}
		}
		else
		{
			d->angle512 -= deg_360_to_512(3)/**fps_factor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
			if (d->angle512 <= deg_360_to_512(91))
			{
				d->angle512 = deg_360_to_512(90);
				d->state=3;
				if (d->level)
				{
					bullet_create_haridan180(s, ANGLE_JIKINERAI_DAN, (/*speed256*/(t256(5)+((d->level)<<8))), 0, 0);
				}
			}
		}
		break;
	case 3:	/*特攻*/
		d->speed+=0.3;/*加速*/
		if (d->speed>=3/*(6+d->level)*/)
		{
			d->speed=3/*(6+d->level)*/;/* 加速最大値 */
			d->state=4;
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
		if (s->y<0)
		{	s->type=SP_DELETE;}
		break;
	}
	/*以下rwingx.cと同じ*/
	s->x+=co_s512((d->angle512))*d->speed*fps_factor;
	s->y+=si_n512((d->angle512))*d->speed*fps_factor;
//	s->aktframe=(deg_512_to_360(d->angle512+deg_360_to_512(270))/10)%36;
//	s->aktframe = ((((d->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
	s->aktframe = ((((d->angle512)&(512-1)))>>6);/*"rwingx8.png"*/
}
/*
(※１):大昔のversionからズレまくってるが、fps_factorが一定しないから、
fps_factorを入れようがズレるのはKETM自体の仕様。
*/

void enemy_zatak_add(int lv)
{
	int i;
	for (i=0;i<NUM_OF_ENEMIES;i++)
	{
		SPRITE *s;
		s				= sprite_add_file(/*"rwing x.png"*/"rwingx8.png",8/*37*/,PR_ENEMY);		s->anim_speed=0;
		s->type 		= SP_EN_ZATAK;
		s->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover		= enemy_zatak_move;
//		s->aktframe 	= 0;
		ZATAK_DATA *data;
		data			= mmalloc(sizeof(ZATAK_DATA));
		s->data 		= data;
		data->b.score	= score(25*2);
		data->b.health	= 1+(difficulty<<2);
		data->state 	= 0;
		data->wait		= 0;
		data->speed 	= t256_to_double((rand()&((256*4)-1)))/*2.0*(((dou ble)rand()/RAND_MAX)*2)*/;
		if (i<6)
		{
			data->angle512 = deg_360_to_512(10);
			s->x=-s->w;
		}
		else
		{
			data->angle512 = deg_360_to_512(170);
			s->x=GAME_WIDTH;		//ウィンドウ幅の変更
		}
			s->y=50;
		data->level=lv;
	}
}
#undef NUM_OF_ENEMIES
