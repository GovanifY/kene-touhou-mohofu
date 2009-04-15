#include "enemy.h"

typedef struct
{
	ENEMY_BASE b;
	int c1;
	int c2;
	int level;
} CRUSHER_DATA;


#define NUM_OF_ENEMIES (10)
static void enemy_crusher_controller(CONTROLLER *c)
{
	int i;
//	int *id_array=c->e;
	/*int*/ SPRITE **id_array= (SPRITE **)c->e;
	SPRITE *s;
	int invisible=0;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		s=/*sprite_get_by_id*/(id_array[i]);
		if (s!=NULL)
		{
			if (!s->flags&SP_FLAG_VISIBLE)
			{
				invisible++;
			}
		}
	}
	if (invisible==NUM_OF_ENEMIES)
	{
		for (i=0; i<NUM_OF_ENEMIES; i++)
		{
			s=/*sprite_get_by_id*/(id_array[i]);
			s->type=SP_DELETE;
		}
		controller_remove(c);
		return;
	}
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		s=/*sprite_get_by_id*/(id_array[i]);
		if (s!=NULL)
		{
			id_array[NUM_OF_ENEMIES]=s/*->x*/;
		//	id_array[NUM_OF_ENEMIES+1]=s->y;
			return;
		}
	}
	bonus_multi_add(/*zzz*/ id_array[NUM_OF_ENEMIES]/*,id_array[NUM_OF_ENEMIES+1]*/,SP_BONUS_06_COIN, 1, BONUS_FLAG_RAND_XY);
	controller_remove(c);
}

static void enemy_crusher_move(SPRITE *s)
{
	CRUSHER_DATA *d=(CRUSHER_DATA *)s->data;

	if (d->c2<2)
	{
		if (!d->c1)
		{
			s->y+=(3+d->level)*fps_factor;
			if (s->y>200)
			{
				d->c1=1;
				d->c2+=1;
			}
		}
		else
		{
			s->y-=(2+d->level/3)*fps_factor;
			if (s->y<50)
			{	d->c1=0;}
		}
		if (d->level>0)
		{
			if (rand()%(100-d->level*10)==0)
			{
				enemy_bullet_create(s,3+d->level/2);
			}
		}
	}
	else
	{
		s->y-=4;
		if (s->y<-(s->w))
		{	s->flags&= ~SP_FLAG_VISIBLE;}
	}
}

void enemy_crusher_add(int lv)
{
	CONTROLLER *c;
	c				=controller_add();
//	NUM_OF_ENEMIES=10;
	int *id_array;
	id_array		=mmalloc(sizeof(int)*(NUM_OF_ENEMIES+2));
	c->e			=id_array;
	c->con			=enemy_crusher_controller;
//
	/*int*/ SPRITE **id_array_bbb= (SPRITE **)c->e;
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s				=sprite_add_file("crusher.png",15,PR_ENEMY);
		id_array_bbb[i] =s/*->id*/;
		s->type 		=SP_EN_CRUSHER;
		s->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover		=enemy_crusher_move;
		s->anim_speed	=i-5;
		s->aktframe 	=i/*%s->frames*/;
		s->x			=(i*25)+10;
		if (i<=4)
		{	s->y=-(i*10);}
		else
		{	s->y=-((9-i)*10);}
		CRUSHER_DATA *data;
		data			=mmalloc(sizeof(CRUSHER_DATA));
		s->data 		=data;
		data->b.score	=score(10*2)*(1+lv);
		data->b.health	=1+lv+(difficulty<<2);
		data->c1		=0;
		data->c2		=0;
		data->level 	=lv;
	}
}
#undef NUM_OF_ENEMIES
