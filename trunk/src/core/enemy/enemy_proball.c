#include "enemy.h"

typedef struct
{
	ENEMY_BASE b;
	int state;
	int dir;
	double speed;
	int level;
} PROBALL_DATA;
/*
→  ↓   ←
↑. ↓  /↑
↑ .↓ / ↑
↑  ↓/  ↑
←←++→→
   /  .
*/

#define NUM_OF_ENEMIES (24)
static void enemy_proball_controller(CONTROLLER *c)
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
		//66%==SP_BONUS_02_BOMB or 33%==SP_BONUS_01_FIREPOWER_G	(SP_BONUS_EXTRA_HOMING+(rand()&3/*%3*/)),
		, 1, BONUS_FLAG_RAND_XY);
	controller_remove(c);
}

static void enemy_proball_move(SPRITE *s)
{
	PROBALL_DATA *d=(PROBALL_DATA *)s->data;
	switch (d->state)
	{
	case 0:
		s->y+=d->speed*fps_factor;
		if (s->y >= (GAME_HEIGHT/2)/*250*/ )
		{
			d->speed=1/*2*/;/*速すぎ*/
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
				d->speed=2/*3*/;/*速すぎ*/
				d->state=2;
			}
		}
		else
		{
			s->x+=d->speed*fps_factor;
			if (s->x>=(GAME_WIDTH-10)-s->w) 	//ウィンドウ幅の変更
			{
				s->x=(GAME_WIDTH-10)-s->w;		//ウィンドウ幅の変更
				d->speed=2/*3*/;/*速すぎ*/
				d->state=2;
			}
		}
		break;
	case 2:
		s->y-=d->speed*fps_factor;
		if (s->y<=0)
		{
			s->y=0;
			d->speed=1/*2*/;/*速すぎ*/
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
		if (s->y >= GAME_HEIGHT) //denis 480
		{
		//	s->type=SP_DELETE;
			s->flags	&= ~SP_FLAG_VISIBLE;
		}
		break;
	}
}

void enemy_proball_add(int lv)
{
//	NUM_OF_ENEMIES=24;
	CONTROLLER *c;
	c=controller_add();
	int *id_array;
	id_array			= mmalloc(sizeof(int)*(NUM_OF_ENEMIES+2));
	c->e				= id_array;
	c->con				= enemy_proball_controller;
//
	/*int*/ SPRITE **id_array_bbb= (SPRITE **)c->e;
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s				= sprite_add_file("protectball.png",11,PR_ENEMY);		s->anim_speed=3;
		id_array_bbb[i] = s/*->id*/;
		s->type 		= SP_EN_PROBALL;
		s->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover		= enemy_proball_move;
		s->aktframe 	= (i&(8-1))/*i%11*/;
		s->x			= (GAME_WIDTH/2)-s->w/2;	//ウィンドウ幅の変更
		s->y			= -s->h-i*s->h;
		PROBALL_DATA *data;
		data			= mmalloc(sizeof(PROBALL_DATA));
		s->data 		= data;
		data->b.score	= score(10*2);
		data->b.health	= 2+(difficulty<<2);
		data->state 	= 0;
		data->speed 	= 2/*3+difficulty+lv/3*/;/*速すぎ*/
		data->dir		= (i&1)/*i%2==0?0:1*/;
		data->level 	= lv;
	}
}
#undef NUM_OF_ENEMIES
