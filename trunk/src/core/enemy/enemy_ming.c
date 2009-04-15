#include "enemy.h"

/*ming*/
/*rwingx_curverに似てる*/

typedef struct
{
	ENEMY_BASE b;
	/*double*/int angle512;
	double speed;
	int state;
	int level;
//
	int type;
} MING_DATA;


/*
10機三角錐状に続がり、編隊飛行してくるザコ敵
*/
#define NUM_OF_ENEMIES (10)
/*enemy_curver_controller()とほぼ同じ(共通化可能かも？)*/
static void enemy_ming_controller(CONTROLLER *c)
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
	bonus_multi_add(/*zzz*/ id_array[NUM_OF_ENEMIES]/*,id_array[NUM_OF_ENEMIES+1]*/,SP_BONUS_00_FIREPOWER, 1, BONUS_FLAG_RAND_XY);
	controller_remove(c);
}

static void enemy_ming_move(SPRITE *s)
{
	MING_DATA *d=(MING_DATA *)s->data;
	switch (d->state)
	{
	case 0:
		if (s->y>=200)
		{
			d->state=1;
			d->speed=5;
		}
		break;
	case 1:
		if (d->type==1)
		{
			d->angle512 += deg_360_to_512(4)/**fps_factor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
			if (d->angle512 >= deg_360_to_512(180))
			{
				d->angle512 = deg_360_to_512(180);
				d->state=2;
				if (d->level>0)
				{	enemy_bullet_create(s,d->level+2);}
			}
		}
		else
		if (d->type==2)
		{
			d->angle512 -= deg_360_to_512(4)/**fps_factor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
			if (d->angle512 <= deg_360_to_512(0))
			{
				d->angle512 = deg_360_to_512(0);
				d->state=2;
				if (d->level>0)
				{	enemy_bullet_create(s,d->level+2);}
			}
		}
		else
		{
			d->state=2;
			if (d->level>0)
			{	enemy_bullet_create(s,d->level+2);}
		}
		break;
	case 2:
		if ((s->x<-s->w)||(s->x>GAME_WIDTH)||(s->y<-s->h)||(s->y>GAME_HEIGHT))		//ウィンドウ幅の変更
		{	s->flags	&= ~SP_FLAG_VISIBLE;}
		break;
	}
	/*似てるがちょっと違う--以下rwingx.cと同じ*/
	s->x+=co_s512((d->angle512))*d->speed*fps_factor;
	s->y+=si_n512((d->angle512))*d->speed*fps_factor;
//	s->aktframe=(deg_512_to_360(d->angle512+deg_360_to_512(270))/10)%36;
	s->aktframe = ((((d->angle512/*+deg_360_to_512(270)*/)&(512-1))*(36/2))>>8);
}
/*
(※１):大昔のversionからズレまくってるが、fps_factorが一定しないから、
fps_factorを入れようがズレるのはKETM自体の仕様。
*/


void enemy_ming_add(int lv)
{
	CONTROLLER *c;
	c=controller_add();
//	NUM_OF_ENEMIES=10;
	int *id_array;
	id_array		= mmalloc(sizeof(int)*(NUM_OF_ENEMIES+2));
	c->e			= id_array;
	c->con			= enemy_ming_controller;
//
	/*int*/ SPRITE **id_array_bbb= (SPRITE **)c->e;
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s				= sprite_add_file("ming.png",36,PR_ENEMY);		s->anim_speed=0;
		id_array_bbb[i] = s/*->id*/;
		s->type 		= SP_EN_MING;
		s->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover		= enemy_ming_move;
		s->aktframe 	= 0;
		MING_DATA *data;
		data			= mmalloc(sizeof(MING_DATA));
		s->data 		= data;
		data->b.score	= score(25*2);
		data->b.health	= 1+(difficulty<<2);
		data->angle512	= deg_360_to_512(90);
		data->speed 	= 2/*2+difficulty+lv/2*/;/*速すぎ*/
		data->state 	= 0;
		data->level 	= lv;
		//ウィンドウ幅の変更
		switch (i)
		{
		case 0: 	s->x=(GAME_WIDTH/2);				s->y=-30;			data->type=0;	break;
		case 1: 	s->x=(GAME_WIDTH/2)-(s->w/2);		s->y=-30-s->h;		data->type=1;	break;
		case 2: 	s->x=(GAME_WIDTH/2)+(s->w/2);		s->y=-30-s->h;		data->type=2;	break;
		case 3: 	s->x=(GAME_WIDTH/2)-s->w;			s->y=-30-2*s->h;	data->type=1;	break;
		case 4: 	s->x=(GAME_WIDTH/2);				s->y=-30-2*s->h;	data->type=0;	break;
		case 5: 	s->x=(GAME_WIDTH/2)+s->w;			s->y=-30-2*s->h;	data->type=2;	break;
		case 6: 	s->x=(GAME_WIDTH/2)-s->w-(s->w/2);	s->y=-30-3*s->h;	data->type=1;	break;
		case 7: 	s->x=(GAME_WIDTH/2)-s->w/2; 		s->y=-30-3*s->h;	data->type=1;	break;
		case 8: 	s->x=(GAME_WIDTH/2)+s->w/2; 		s->y=-30-3*s->h;	data->type=2;	break;
		case 9: 	s->x=(GAME_WIDTH/2)+s->w+s->w/2;	s->y=-30-3*s->h;	data->type=2;	break;
		}
	}
}
#undef NUM_OF_ENEMIES
