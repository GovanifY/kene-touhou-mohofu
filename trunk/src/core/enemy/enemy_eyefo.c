#include "enemy.h"

typedef struct
{
	ENEMY_BASE b;
	double radius;
	int flag1;
	double angle;
	double counter;
	double xcenter;
	double ycenter;
	int level;
} EYEFO_DATA;


#define NUM_OF_ENEMIES (16)
static void enemy_eyefo_controller(CONTROLLER *c)
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

static void enemy_eyefo_move(SPRITE *s)
{
	EYEFO_DATA *d=(EYEFO_DATA *)s->data;

	if (!d->flag1) {
		d->radius+=fps_factor;
		if (d->radius>150)
			d->flag1=1;
	} else {
		d->radius-=fps_factor;
		if (d->radius<=10)
			d->flag1=0;
	}
	d->angle+=5*fps_factor;
	if (d->angle>=360)
		d->angle-=360;
	s->x=(cos(degtorad(d->angle))*d->radius)+d->xcenter;
	s->y=(sin(degtorad(d->angle))*d->radius)+d->ycenter;

	if (d->level)
		if (rand()%(1000-d->level*100)==0)
			enemy_homing_create(s);

	d->counter+=fps_factor;
	if (d->counter<300) {
		if (d->ycenter<HEIGHT)	//denis
			d->ycenter+=fps_factor;
	} else if (d->counter>600) {
			d->ycenter-=fps_factor;
			if (d->ycenter+s->y<-s->w)
				// s->type=-1;
				s->flags&=~SP_FLAG_VISIBLE;
	}
}

void enemy_eyefo_add(int lv)
{
	SPRITE *s;
	EYEFO_DATA *data;
	CONTROLLER *c;
	int *id_array;

	c=controller_add();
//	NUM_OF_ENEMIES=16;
	id_array=mmalloc(sizeof(int)*(NUM_OF_ENEMIES+2));
	c->e=id_array;
	c->con=enemy_eyefo_controller;

	int i;
	for (i=0;i<16;i++)
	{
		s=sprite_add_file("eyefo.png",15,PR_ENEMY); 	s->anim_speed=2;
		id_array[i]=s->id;
		s->type=SP_EN_EYEFO;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_eyefo_move;
		s->aktframe=i%s->frames;
		data=mmalloc(sizeof(EYEFO_DATA));
		s->data=data;
		data->b.score=score(30*2);
		data->b.health=10;
		data->radius=10;
		data->flag1=0;
		data->angle=360/16*i;
		data->counter=0;
		data->xcenter=(WIDTH2/2);		//ウィンドウ幅の変更
		data->ycenter=-100;
		data->level=lv;
	}
}
#undef NUM_OF_ENEMIES
