#include "enemy.h"

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
	explosion_add(s->x,s->y,0,EXPLOSION_MINI00);
	d->b.health-=w->strength;
	if (d->b.health<=0)
	{
		explosion_add_circle(c);
		bonus_multi_add(c, SP_BONUS_06_COIN, 7, BONUS_FLAG_COLLECT);
		((PLAYER_DATA *)player->data)->score+=d->b.score;
		((PLAYER_DATA *)player->data)->bossmode=B04_CHANGE;
	}
}

static void enemy_greatfairy_move(SPRITE *s)
{
	GREAT_FAIRY_DATA *d=(GREAT_FAIRY_DATA *)s->data;
	if (((PLAYER_DATA *)player->data)->bossmode==B04_CHANGE)
	{
		d->n--;
		if (d->n<-1)
		{
			s->type=SP_DELETE;
			((PLAYER_DATA *)player->data)->bossmode=B00_NONE;
		}
	}
	else
	{
		switch (d->state)
		{
		case 0:
			if (s->y>50){
				d->state=1;
			}
			else
			{	s->y+=2*fps_factor;}
			break;
		case 1:
			if (d->wait1<0){
				if (d->n==2)
				{	d->state=5;}
				else{
					d->state=2;
					d->wait1=150;
				}
			}
			else
			{	d->wait1--;}
			break;
		case 2:
			if (d->wait1<0){
				d->state=3;
				d->wait2=8/*10*/;
				d->wait1=30;
			}
			else
			{
				d->wait1--;
				if (d->wait2<0)
				{
					d->wait2=8/*10*/;
					playChunk(14);
					enemy_n_way_bullet_type(s, BU_TYPE01_KUGEL_PNG/*"kugel.png", 0*/, 8, 5-1+difficulty, ANGLE_JIKINERAI_KISUDAN);
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
			{	d->wait1--;}
			break;
		case 4:
			if (d->wait1<0)
			{
				d->state=1;
				d->wait2=8/*10*/;
				d->wait1=30;
				d->n++;
			}
			else
			{
				d->wait1--;
				if (d->wait2<0)
				{
					d->wait2=8/*10*/;
					playChunk(14);
					enemy_n_way_bullet_type(s, BU_TYPE01_KUGEL_PNG/*"kugel.png", 0*/, 7, 6-1+difficulty, ANGLE_JIKINERAI_KISUDAN);
				}
				else
				{	d->wait2--;}
			}
			break;
		case 5:
			if (s->y<-s->h)
			{
				s->type=SP_DELETE;
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
	s				= sprite_add_file("great_fairy.png",2,PR_ENEMY);	s->anim_speed	= 3;
	s->type 		= SP_EN_GFAIRY;
	s->flags		|= (SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	s->mover		= enemy_greatfairy_move;
	s->y			= -30;
	s->x			= lv*35+40;
	GREAT_FAIRY_DATA *data;
	data			= mmalloc(sizeof(GREAT_FAIRY_DATA));
	s->data 		= data;
	data->state 	= 0;
	data->wait1 	= 30;
	data->wait2 	= 0;
	data->b.score	= score(100)+score(100)*difficulty;
	data->b.health	= 64+(difficulty<<5/*7*/);//50+150*difficulty;
	data->n 		= 0;
}
