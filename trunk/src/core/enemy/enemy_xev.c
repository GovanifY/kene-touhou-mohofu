#include "enemy.h"

extern SPRITE *player;
extern double fps_factor;

typedef struct
{
	ENEMY_BASE b;
	int state;
	int tx;
	int ty;
	int level;
} XEV_DATA;

static void enemy_xev_move(SPRITE *s)
{
	double angle;
	double speed=0;
	XEV_DATA *d=(XEV_DATA *)s->data;
	static const int item_table[10]={	/* [***20090223	追加 テーブルにしたよ */
		0x1000,
		0x1003,
		0x1000,
		0x1002,
		0x1000,
		0x1007,
		0x1003,
		0x1002,
		0x1001,
		0x1005
	};
	static unsigned int drop_item;
	if(drop_item>9)
		drop_item=0;

	switch (d->state)
	{
	case 0:
		speed=3;
		if ((s->x >= player->x)||(s->y>400))
		{
			d->state=1;
			s->anim_speed=-1;
			d->tx=-100;
			d->ty=player->y;
			if (d->level)
				enemy_bullet_create(s,1+d->level);
			bonus_add(s->x,s->y,item_table[drop_item++],0);
		}
		break;/* ??? [***20090210	追加  */
	case 1:
		speed=6;
		if (s->x <= -s->w)
		{
			s->type=-1;
		}
	}
	angle=atan2(d->ty-s->y,d->tx-s->x);
	s->x+=cos(angle)*speed*fps_factor;
	s->y+=sin(angle)*speed*fps_factor;
}

void enemy_xev_add(int lv)
{
	SPRITE *s;
	XEV_DATA *data;
	int i;
	for (i=0;i<3;i++)
	{
		s=sprite_add_file("crusher.png",15,PR_ENEMY);		s->anim_speed=1;
		s->type=SP_EN_XEV;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_xev_move;
		s->aktframe=0;
		switch (rand()%3)
		{
		case 0: s->x=rand()%20; break;
		case 1: s->x=WIDTH2-rand()%20; break;		//ウィンドウ幅の変更
		case 2: s->x=(WIDTH2/2)+10+rand()%20; break;		//ウィンドウ幅の変更
		}
		s->y=rand()%20-50;
		data=mmalloc(sizeof(XEV_DATA));
		s->data=data;
		data->b.score=score(5*2);
		data->b.health=1;
		data->state=0;
		data->tx=player->x;
		data->ty=player->y;
		data->level=lv;
	}
}
