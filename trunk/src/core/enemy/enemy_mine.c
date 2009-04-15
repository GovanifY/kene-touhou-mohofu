#include "enemy.h"

typedef struct
{
	ENEMY_BASE b;
	int state;
	int tx;
	int ty;
	int delay;
	int level;
} MINE_DATA;


static void enemy_supermine_move(SPRITE *s)
{
	MINE_DATA *d=(MINE_DATA *)s->data;

	if (d->level)
	{
		if (rand()%(100-d->level*10)==0)
		{
			enemy_bullet_create(s,3+d->level);
		}
	}

	s->x+=co_s512((d->state))*3*fps_factor;
	s->y+=si_n512((d->state))*3*fps_factor;

	if ((s->x<0-s->w)||(s->x>GAME_WIDTH)||(s->y<0-s->h)||(s->y>GAME_HEIGHT)) 	//ウィンドウ幅の変更
	{	s->type=SP_DELETE;}
}
static void enemy_supermine_add(SPRITE *c, int lv)
{
	SPRITE *s;
	MINE_DATA *data;
	int i;
	for (i=0;i<8;i++)
	{
		s				= sprite_add_file("12side.png",19,PR_ENEMY); 	s->anim_speed=2;
		s->type 		= SP_EN_MINE;
		s->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover		= enemy_supermine_move;
		s->aktframe 	= 0;
		s->x			= c->x;
		s->y			= c->y;
		data			= mmalloc(sizeof(MINE_DATA));
		s->data 		= data;
		data->b.score	= score(10*2)*(1+lv);
		data->b.health	= 1+lv+(difficulty<<4);
		data->state 	= (i<<6);//  (/*360*/512/8)*i;
		data->level 	= lv;
	}
}


static void enemy_mine_move(SPRITE *s)
{
	MINE_DATA *d=(MINE_DATA *)s->data;
	switch (d->state)
	{
	case 0: /* move */
		{
		/*double*/int angle512;
			angle512=atan_512(d->ty-s->y,d->tx-s->x);
			s->x+=co_s512(angle512)*1.5*fps_factor;
			s->y+=si_n512(angle512)*1.5*fps_factor;
		}
		if ((s->x >= d->tx-1) && (s->x <= d->tx+1) &&
		   (s->y >= d->ty-1) && (s->y <= d->ty+1))
		{
			d->state=1;
			d->delay=50;
		}
		break;
	case 1:
		d->delay-=fps_factor;
		if (d->delay<=0)
		{
			d->state=2;
			d->delay=30;
			s->anim_speed=0;
		}
		break;
	case 2:
		d->delay-=fps_factor;
		if (d->delay<=0)
		{
			enemy_supermine_add(s,d->level);
			s->type=SP_DELETE;
		}
		break;
	}
}

void enemy_mine_add(int lv)
{
	SPRITE *s;
	s				= sprite_add_file("12side.png",19,PR_ENEMY);
	s->anim_speed	= 1;
	s->type 		= SP_EN_MINE;
	s->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	s->mover		= enemy_mine_move;
	s->aktframe 	= 0;
	s->x			= rand()%(GAME_WIDTH+10)-10; 	//ウィンドウ幅の変更
	s->y			= -40;
	MINE_DATA *data;
	data			= mmalloc(sizeof(MINE_DATA));
	s->data 		= data;
	data->b.score	= score(30*2);
	data->b.health	= 12+lv*2+(difficulty<<2);
	data->tx		= rand()%270+50;
	data->ty		= rand()%300+50;
	data->state 	= 0;
	data->level 	= lv;
}
