#include "enemy.h"

extern double fps_factor;
extern int difficulty;
extern SPRITE *player;

typedef struct
{
	ENEMY_BASE b;
	int state;
	int wait1;	//s“®wait
	int wait2;	//UŒ‚wait
	int n;
} GREAT_FAIRY_DATA;

void enemy_nonshield_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/)
{
	GREAT_FAIRY_DATA *d=(GREAT_FAIRY_DATA *)c->data;
	WEAPON_BASE *w=(WEAPON_BASE *)s->data;

	explosion_add(s->x,s->y,0,EXPLOSION_TYPE04);
	d->b.health-=w->strength;

	if (d->b.health<=0) {
		bonus_multi_add(c->x, c->y,SP_BONUS_COIN,7,1);
		((PLAYER_DATA *)player->data)->score+=d->b.score;
		((PLAYER_DATA *)player->data)->bossmode=4;
	}
}

static void enemy_greatfairy_move(SPRITE *s)
{
	GREAT_FAIRY_DATA *d=(GREAT_FAIRY_DATA *)s->data;

	if (((PLAYER_DATA *)player->data)->bossmode==4)
	{
		d->n--;
		if (d->n<-1){
			s->type=-1;
			((PLAYER_DATA *)player->data)->bossmode=0;
		}
	}
	else{
		switch (d->state){
			case 0:
				if (s->y>50){
					d->state=1;
				}
				else
					s->y+=2*fps_factor;
				break;
			case 1:
				if (d->wait1<0){
					if (d->n==2)
						d->state=5;
					else{
						d->state=2;
						d->wait1=150;
					}
				}
				else
					d->wait1--;
				break;
			case 2:
				if (d->wait1<0){
					d->state=3;
					d->wait2=10;
					d->wait1=30;
				}
				else
				{
					d->wait1--;
					if (d->wait2<0)
					{
						d->wait2=10;
						playChunk(14);
						enemy_n_way_bullet(s, "kugel.png", 0, 8, 5+difficulty, atan2(player->y+player->h/2-s->y-s->h/2,player->x-player->w/2-s->x-s->w/2));
					}
					else
					{	d->wait2--;}
				}
				break;
			case 3:
				if (d->wait1<0)
				{
					d->state=4;
					d->wait1=100;
				}
				else
					d->wait1--;
				break;
			case 4:
				if (d->wait1<0)
				{
					d->state=1;
					d->wait2=10;
					d->wait1=30;
					d->n++;
				}
				else
				{
					d->wait1--;
					if (d->wait2<0)
					{
						d->wait2=10;
						playChunk(14);
						enemy_n_way_bullet(s, "kugel.png", 0, 7, 6+difficulty, atan2(player->y+player->h/2-s->y-s->h/2,player->x-player->w/2-s->x-s->w/2));
					}
					else
					{	d->wait2--;}
				}
				break;
			case 5:
				if (s->y<-s->h)
				{
					s->type=-1;
				}
				else
				{
					s->y-=2*fps_factor;
				}
				break;
		}
	}
}

void enemy_greatfairy_add(int lv)
{
	SPRITE *s;
	GREAT_FAIRY_DATA *data;

	s=sprite_add_file("great_fairy.png",2,PR_ENEMY);	s->anim_speed	=3;
	s->type 		=SP_EN_GFAIRY;
	s->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	s->mover		=enemy_greatfairy_move;
	data			=mmalloc(sizeof(GREAT_FAIRY_DATA));
	s->data 		=data;
	s->y			=-30;
	s->x			=lv*35+40;
	data->state 	=0;
	data->wait1 	=30;
	data->wait2 	=0;
	data->b.score	=score(100)+score(100)*difficulty;
	data->b.health	=50+150*difficulty;
	data->n 		=0;
}
