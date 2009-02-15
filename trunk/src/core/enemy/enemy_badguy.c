#include "enemy.h"

typedef struct
{
	ENEMY_BASE b;
	int state;
	int tx;
	int ty;
	double speed;
	int level;
} BADGUY_DATA;


static void enemy_badguy_move(SPRITE *s)
{
	double angle;
	BADGUY_DATA *d=(BADGUY_DATA *)s->data;
	switch (d->state)
	{
	case 0:
		if ((s->x >= player->x)||(s->y>400)) {
			d->state=1;
			s->anim_speed=-3;
			d->tx=-100;
			d->ty=player->y;
			if (d->level>0)
				enemy_bullet_create(s,1+d->level/2);
			d->speed=6*fps_factor;
		}
		break;/*??? [***090215 追加 */
	case 1:
		if (s->x <= -s->w) {
			s->type=-1;
		}
	}
	angle=atan2(d->ty+20-s->y,d->tx-s->x);
	s->x+=cos(angle)*d->speed;
	s->y+=sin(angle)*d->speed;
}

void enemy_badguy_add(int lv)
{
	SPRITE *s;
	BADGUY_DATA *data;

	int i;
	for (i=0;i<10;i++)
	{
		s=sprite_add_file("badguy.png",9,PR_ENEMY);		s->anim_speed=3;
		s->type=SP_EN_BADGUY;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_badguy_move;
		s->aktframe=0;
		switch (i%5) {
			case 0: s->x=rand()%40; break;
			case 1: s->x=WIDTH2-rand()%40; break;		//ウィンドウ幅の変更
			case 2: s->x=200+rand()%40; break;
			case 3: s->x=90+rand()%40; break;
			case 4: s->x=310+rand()%40; break;
		}
		s->y=rand()%40-90;
		data=mmalloc(sizeof(BADGUY_DATA));
		s->data=data;
		data->b.score=score(50*2)*(lv+1);
		data->b.health=1+difficulty;
		data->state=0;
		data->tx=player->x;
		data->ty=player->y;
		data->speed=fps_factor*((double)(rand()%200)/100)*(1+difficulty+lv/3);
		data->level=lv;
	}
}
