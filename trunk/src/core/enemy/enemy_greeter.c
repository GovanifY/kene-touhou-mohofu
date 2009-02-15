#include "enemy.h"

typedef struct
{
	ENEMY_BASE b;
	int state;
	int level;
	double angle;
	double speed;
	double max_y;
} GREETER_DATA;

/*
５機一列に続がり、編隊飛行してくるザコ敵
*/
#define NUM_OF_ENEMIES (5)
static void enemy_greeter_controller(CONTROLLER *c)
{
	int i;
	int *id_array=c->e;
	SPRITE *s;
	int invisible=0;
	static int bonus=SP_BONUS_ADDSPEED;

	for (i=0;i<NUM_OF_ENEMIES;i++)
	{
		s=sprite_get_by_id(id_array[i]);
		if (s!=NULL)
		{
			if (!s->flags&SP_FLAG_VISIBLE)
			{	invisible++;}
		}
	}
	if (invisible==NUM_OF_ENEMIES)
	{
		for (i=0;i<NUM_OF_ENEMIES;i++)
		{
			s=sprite_get_by_id(id_array[i]);
			s->type=-1;
		}
		controller_remove(c);
		return;
	}

	for (i=0;i<NUM_OF_ENEMIES;i++)
	{
		s=sprite_get_by_id(id_array[i]);
		if (s!=NULL)
		{
			id_array[NUM_OF_ENEMIES]=s->x;
			id_array[NUM_OF_ENEMIES+1]=s->y;
			return;
		}
	}
	bonus_add(id_array[NUM_OF_ENEMIES],id_array[NUM_OF_ENEMIES+1],bonus,0);
	bonus++;
	if (bonus==SP_BONUS_HEALTH)
	{	bonus=SP_BONUS_FIREPOWER;}
	controller_remove(c);
}

static void enemy_greeter_move(SPRITE *s)
{
	GREETER_DATA *d=(GREETER_DATA *)s->data;
	switch (d->state)
	{
	case 0: /* nach unten */
		if (s->y>=d->max_y)
		{
			d->angle=atan2(0-s->y,WIDTH2/2-s->x);				//ウィンドウ幅の変更
			d->state=1;
			d->speed=3+difficulty;								//[***090201		変更
		}
		if (d->level)
		{
			if (rand()%(105-(d->level-2+difficulty)*10)==0) 	//[***090126		若干変更
			{
				enemy_bullet_create(s,1+difficulty+d->level/3);
			}
		}
		break;
	case 1:
		if (s->y<-s->h)
		{
			s->flags&=~SP_FLAG_VISIBLE;
		}
		break;
	}
	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor;
	s->aktframe=(radtodeg(d->angle)/10)%36;
}

void enemy_greeter_add(int lv)
{
	SPRITE *s;
	GREETER_DATA *data;
	CONTROLLER *c;
	int *id_array;
	static int last=0;

	c=controller_add();
//	NUM_OF_ENEMIES=5;
	id_array=mmalloc(sizeof(int)*(NUM_OF_ENEMIES+2));
	c->e=id_array;
	c->con=enemy_greeter_controller;

	int i;
	for (i=0;i<NUM_OF_ENEMIES;i++)
	{
		s=sprite_add_file("ming.png",36,PR_ENEMY);		s->anim_speed=0;
		id_array[i]=s->id;
		s->type=SP_EN_GREETER;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_greeter_move;
		data=mmalloc(sizeof(GREETER_DATA));
		s->data=data;
		if (last)
			s->x=WIDTH2-s->w;		//ウィンドウ幅の変更
		else
			s->x=0;
		s->y=-i*s->h;
		data->angle=atan2((HEIGHT-s->h-60)-s->y,WIDTH2/2-s->x); 	//ウィンドウ幅の変更
		data->max_y=(HEIGHT-s->h-60);
		data->speed=4;
		data->state=0;
		data->b.score=score(5*2);
		data->b.health=1;
		data->level=lv;
	}
	last=~last;
}
#undef NUM_OF_ENEMIES
