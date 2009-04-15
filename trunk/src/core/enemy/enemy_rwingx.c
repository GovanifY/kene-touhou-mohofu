#include "enemy.h"

/*rwingx*/

typedef struct
{
	ENEMY_BASE b;
	int angle512;
	double speed;
	int state;
	int level;
//
	double wait;
} RWINGX_DATA;


#define NUM_OF_ENEMIES (8)
static void enemy_rwingx_move(SPRITE *s)
{
	RWINGX_DATA *d=(RWINGX_DATA *)s->data;
	switch (d->state)
	{
	case 0: /* sleep(d->wait) ticks */
		if (d->wait>0)
		{
			d->wait-=fps_factor;
		}
		else
		{
			/* 上方から登場 / langsam nach unten */
			d->state		= 1;
			d->angle512 	= deg_360_to_512(90);
			d->speed		= 1;
			d->wait 		= 100;
		}
		break;
	case 1: /* move (d->wait) ticks */
		if (d->wait>0)
		{
			d->wait-=fps_factor;
		}
		else
		{
			d->state		= 2;
			d->wait 		= 60;
			d->speed		= 0;
		}
		break;
	case 2: /* sleep(d->wait) ticks */
		if (d->wait>0)
		{
			d->wait-=fps_factor;
		}
		else
		{
			/* schnell richtung player */
			d->state		= 3;
			d->angle512 	= atan_512(player->y-s->y,player->x-s->x);
			d->speed		= 4+d->level;
			d->wait 		= 50;
		}
		break;
	case 3: /* move (d->wait) ticks */
		if (s->y>=GAME_HEIGHT-150)
		{
			if (d->level)
			{	enemy_bullet_create(s,3+d->level);}
			d->state	= 4;
			d->angle512 += deg_360_to_512(180);
			mask512(d->angle512);//	d->angle %= deg_360_to_512(360);
			d->speed=6+d->level;
		}
		break;
	case 4:
		if ((s->x<-s->w) || (s->x>GAME_WIDTH) ||
			(s->y<-s->h) || (s->y>GAME_HEIGHT))		//ウィンドウ幅の変更
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

void enemy_rwingx_add(int lv)
{
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s				= sprite_add_file(/*"rwing x.png"*/"rwingx8.png",8/*37*/,PR_ENEMY); 	s->anim_speed	= 0;
		s->type 		= SP_EN_RWINGX;
		s->flags		|= (SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover		= enemy_rwingx_move;
		RWINGX_DATA *data;
		data			= mmalloc(sizeof(RWINGX_DATA));
		s->data 		= data;
		data->b.score	= score(25*2)*(1+lv);
		data->b.health	= 1+lv+(difficulty<<2);
		data->angle512	= deg_360_to_512(270);
		data->speed 	= 0.5;
		data->state 	= 0;
		data->wait		= 20;
		data->level 	= lv;
		s->x			= (GAME_WIDTH/8)*i-20;		//ウィンドウ幅の変更
		s->y			= -50;
	}
}
#undef NUM_OF_ENEMIES
