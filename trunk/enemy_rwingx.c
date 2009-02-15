#include "enemy.h"

typedef struct
{
	ENEMY_BASE b;
	int angle;
	double speed;
	int state;
	double wait;
	int level;
} RWINGX_DATA;


#define NUM_OF_ENEMIES (8)
static void enemy_rwingx_move(SPRITE *s)
{
	RWINGX_DATA *d=(RWINGX_DATA *)s->data;
	switch (d->state)
	{
	case 0: /* sleep(d->wait) ticks */
		if (d->wait>0) {
			d->wait-=fps_factor;
		} else {
			/* langsam nach unten */
			d->state=1;
			d->angle=90;
			d->speed=1;
			d->wait=100;
		}
		break;
	case 1: /* move (d->wait) ticks */
		if (d->wait>0) {
			d->wait-=fps_factor;
		} else {
			d->state=2;
			d->wait=60;
			d->speed=0;
		}
		break;
	case 2: /* sleep(d->wait) ticks */
		if (d->wait>0) {
			d->wait-=fps_factor;
		} else {
			/* schnell richtung player */
			d->state=3;
			d->angle=radtodeg(atan2(player->y-s->y,player->x-s->x));
			d->speed=4+d->level;
			d->wait=50;
		}
		break;
	case 3: /* move (d->wait) ticks */
		if (s->y>=HEIGHT-150) {
			if (d->level)
				enemy_bullet_create(s,3+d->level);
			d->state=4;
			d->angle+=180;
			d->angle%=360;
			d->speed=6+d->level;
		}
		break;
	case 4:
		if ((s->x<-s->w) || (s->x>WIDTH2) || (s->y<-s->h) || (s->y>HEIGHT))		//�E�B���h�E���̕ύX
			s->type=-1;
		break;
	}

	s->x+=cos(degtorad(d->angle))*d->speed*fps_factor;
	s->y+=sin(degtorad(d->angle))*d->speed*fps_factor;
	s->aktframe=((d->angle+270)/10)%36;
}

void enemy_rwingx_add(int lv)
{
	int i;
	SPRITE *s;
	RWINGX_DATA *data;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		s				= sprite_add_file("rwingx.png",37,PR_ENEMY);		s->anim_speed	= 0;
		s->type 		= SP_EN_RWINGX;
		s->flags		|= (SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover		= enemy_rwingx_move;
		data			= mmalloc(sizeof(RWINGX_DATA));
		s->data 		= data;
		data->b.score	= score(25*2)*(1+lv);
		data->b.health	= 1+lv;
		data->angle 	= 270;
		data->speed 	= 0.5;
		data->state 	= 0;
		data->wait		= 20;
		data->level 	= lv;
		s->x			= (WIDTH2/8)*i-20;		//�E�B���h�E���̕ύX
		s->y			= -50;
	}
}
#undef NUM_OF_ENEMIES
