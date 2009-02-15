#include "enemy.h"

typedef struct
{
	ENEMY_BASE b;
	int state;
	int dir;
	double speed;
	int level;
} PROBALL_DATA;


#define NUM_OF_ENEMIES (24)
static void enemy_proball_controller(CONTROLLER *c)
{
	int i;
	int *id_array=c->e;
	SPRITE *s;
	int invisible=0;

	for (i=0;i<NUM_OF_ENEMIES;i++) {
		s=sprite_get_by_id(id_array[i]);
		if (s!=NULL)
			if (!s->flags&SP_FLAG_VISIBLE)
				invisible++;
	}
	if (invisible==NUM_OF_ENEMIES) {
		for (i=0;i<NUM_OF_ENEMIES;i++) {
			s=sprite_get_by_id(id_array[i]);
			s->type=-1;
		}
		controller_remove(c);
		return;
	}

	for (i=0;i<NUM_OF_ENEMIES;i++) {
		s=sprite_get_by_id(id_array[i]);
		if (s!=NULL) {
			id_array[NUM_OF_ENEMIES]=s->x;
			id_array[NUM_OF_ENEMIES+1]=s->y;
			return;
		}
	}
	bonus_add(id_array[NUM_OF_ENEMIES],id_array[NUM_OF_ENEMIES+1],SP_BONUS_EXTRA,0);
	controller_remove(c);
}

static void enemy_proball_move(SPRITE *s)
{
	PROBALL_DATA *d=(PROBALL_DATA *)s->data;
	switch (d->state)
	{
	case 0:
		s->y+=d->speed*fps_factor;
		if (s->y>=250)
		{
			d->speed=2;
			d->state=1;
		}
		break;
	case 1:
		d->speed+=0.1;
		if (d->dir==0)
		{
			s->x-=d->speed*fps_factor;
			if (s->x<=10)
			{
				s->x=10;
				d->speed=3;
				d->state=2;
			}
		} else {
			s->x+=d->speed*fps_factor;
			if (s->x>=(WIDTH2-10)-s->w) 	//ウィンドウ幅の変更
			{
				s->x=(WIDTH2-10)-s->w;		//ウィンドウ幅の変更
				d->speed=3;
				d->state=2;
			}
		}
		break;
	case 2:
		s->y-=d->speed*fps_factor;
		if (s->y<=0)
		{
			s->y=0;
			d->speed=2;
			d->state=3;
		}
		break;
	case 3:
		d->speed+=0.2;
		s->y+=d->speed*fps_factor;
		if (d->dir==0)
		{
			s->x+=d->speed*fps_factor;
		}
		else
		{
			s->x-=d->speed*fps_factor;
		}
		if (s->y >= HEIGHT) //denis 480
		{
		//	s->type=-1;
			s->flags&=~SP_FLAG_VISIBLE;
		}
		break;
	}
}

void enemy_proball_add(int lv)
{
	SPRITE *s;
	PROBALL_DATA *data;
	CONTROLLER *c;
	int *id_array;

	c=controller_add();
//	NUM_OF_ENEMIES=24;
	id_array=mmalloc(sizeof(int)*(NUM_OF_ENEMIES+2));
	c->e=id_array;
	c->con=enemy_proball_controller;

	int i;
	for (i=0;i<NUM_OF_ENEMIES;i++)
	{
		s=sprite_add_file("protectball.png",11,PR_ENEMY);		s->anim_speed=3;
		id_array[i]=s->id;
		s->type=SP_EN_PROBALL;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_proball_move;
		s->aktframe=i%11;
		s->x=(WIDTH2/2)-s->w/2; 	//ウィンドウ幅の変更
		s->y=-s->h-i*s->h;
		data=mmalloc(sizeof(PROBALL_DATA));
		s->data=data;
		data->b.score=score(10*2);
		data->b.health=2;
		data->state=0;
		data->speed=3+difficulty+lv/3;
		data->dir=(i&1)/*i%2==0?0:1*/;
		data->level=lv;
	}
}
#undef NUM_OF_ENEMIES
