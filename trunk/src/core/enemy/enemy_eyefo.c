#include "enemy.h"

typedef struct
{
	ENEMY_BASE b;
	/*double*/int angle512;
	double radius;
	int level;
	int flag1;
	double counter;
	double xcenter;
	double ycenter;
} EYEFO_DATA;


#define NUM_OF_ENEMIES (16)
static void enemy_eyefo_controller(CONTROLLER *c)
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
	bonus_multi_add(/*zzz*/ id_array[NUM_OF_ENEMIES]/*,id_array[NUM_OF_ENEMIES+1]*/,
		(SP_BONUS_01_FIREPOWER_G+(rand()&1))/*50%(SP_BONUS_01_FIREPOWER_G or SP_BONUS_02_BOMB)*/
		//66%==SP_BONUS_02_BOMB or 33%==SP_BONUS_01_FIREPOWER_G (SP_BONUS_EXTRA_HOMING+(rand()&3/*%3*/)),
		, 1, BONUS_FLAG_RAND_XY);
	controller_remove(c);
}

static void enemy_eyefo_move(SPRITE *s)
{
	EYEFO_DATA *d=(EYEFO_DATA *)s->data;

	if (!d->flag1)
	{
		d->radius+=fps_factor;
		if (d->radius>150)
		{	d->flag1=1;}
	}
	else
	{
		d->radius-=fps_factor;
		if (d->radius<=10)
		{	d->flag1=0;}
	}
	d->angle512 += deg_360_to_512(5)/**fps_factor*/;
	mask512(d->angle512);//	if (d->angle>=360)	{	d->angle-=360;}
	s->x=(co_s512((d->angle512))*d->radius)+d->xcenter;
	s->y=(si_n512((d->angle512))*d->radius)+d->ycenter;

	if (d->level)
	{
		if (rand()%(1000-d->level*100)==0)
		{
			enemy_homing_create(s);
		}
	}

	d->counter+=fps_factor;
	if (d->counter<300)
	{
		if (d->ycenter<GAME_HEIGHT)	//denis
		{
			d->ycenter+=fps_factor;
		}
	}
	else if (d->counter>600)
	{
		d->ycenter-=fps_factor;
		if (d->ycenter+s->y<-s->w)
		{
			// s->type=SP_DELETE;
			s->flags&= ~SP_FLAG_VISIBLE;
		}
	}
}

void enemy_eyefo_add(int lv)
{
	CONTROLLER *c;
	c			= controller_add();
//	NUM_OF_ENEMIES=16;
	/*int*/void *id_array;
	id_array	= mmalloc(sizeof(int)*(NUM_OF_ENEMIES+2));
	c->e		= id_array;
	c->con		= enemy_eyefo_controller;
//
	/*int*/ SPRITE **id_array_bbb= (SPRITE **)c->e;
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s				= sprite_add_file("eyefo.png",15,PR_ENEMY); s->anim_speed=2;
		id_array_bbb[i] = s/*->id*/;
		s->type 		= SP_EN_EYEFO;
		s->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover		= enemy_eyefo_move;
		s->aktframe 	= i/*%s->frames*/;
		EYEFO_DATA *data;
		data			= mmalloc(sizeof(EYEFO_DATA));
		s->data 		= data;
		data->b.score	= score(30*2);
		data->b.health	= 10+(difficulty<<2);
		data->radius	= 10;
		data->flag1 	= 0;
		data->angle512	= (/*360*/512/NUM_OF_ENEMIES)*i;
		data->counter	= 0;
		data->xcenter	= (GAME_WIDTH/2);		//ウィンドウ幅の変更
		data->ycenter	= -100;
		data->level 	= lv;
	}
}
#undef NUM_OF_ENEMIES
