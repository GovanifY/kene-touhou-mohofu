#include "enemy.h"

extern SPRITE *player;
extern double fps_factor;
extern int difficulty;

typedef struct
{
	ENEMY_BASE b;
	int state;
	int level;
	int wait;
	int ani_turn;
	int n;
} SPLASH_DATA;

static void enemy_splash_move(SPRITE *s)
{
	SPLASH_DATA *d=(SPLASH_DATA *)s->data;
	double p;
	int i2;

	switch (d->state)
	{
	case 0: 	//登場
		if (d->wait<40)
		{
			if (d->level<3)
			{
				s->x-=2*fps_factor;
			}
			else if (d->level<7)
			{
				s->y+=2*fps_factor;
			}
			else
			{
				s->x+=2*fps_factor;
			}
			d->wait++;
		}
		else
		{
			d->state=1;
			d->wait=10;
		}
		break;
	case 1: 	//wait
		if (d->wait<0)
		{	d->state=2;}
		else
		{
			p=atan2(player->y-s->y,player->x-s->x);
			if (((p>=M_PI/8)&&(p<7*M_PI/8))||((p>=-5*M_PI/8)&&(p<-3*M_PI/8)))
															s->aktframe=(s->aktframe&(4-1))+8;
			else if ((p>=-M_PI/8)&&(p<M_PI/8))				s->aktframe=(s->aktframe&(4-1))+16;
			else if ((p>=-3*M_PI/8)&&(p<-M_PI/8))			s->aktframe=(s->aktframe&(4-1))+12;
			else if ((p>=-7*M_PI/8)&&(p<-5*M_PI/8)) 		s->aktframe=(s->aktframe&(4-1))+4;
			else if ((p>=7*M_PI/8)||(p<-7*M_PI/8))			s->aktframe=(s->aktframe&(4-1));
			d->wait--;
		}
		break;
	case 2: 	//shot
		d->n++;
		d->wait=50;
		d->state=1;
		enemy_laser_create(s, 3);
		enemy_laser_create(s, 4);

		p=atan2(player->y+player->h/2-s->y,player->x-player->w/2-s->x);
		SPRITE *h;
		BULLET_DATA *data;
		if (difficulty>0)
		{
			for (i2=0;i2<4;i2++)
			{
				h=NULL;
				data=NULL;
				h=sprite_add_file("bshoot.png",1,PR_ENEMY);
				h->type=SP_EN_LASER;
				h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
				h->mover=enemy_laser_move;
				h->aktframe=0;
				h->x=s->x+(s->w/2-h->w/2);
				h->y=s->y+(s->h/2-h->h/2);

				data=mmalloc(sizeof(BULLET_DATA));
				h->data=data;

//				data->id=rand()%1000;
				data->angle=p+(1-i2%2*2)*M_PI/6;
				data->speed=3+i2/2;
			}
			if (difficulty>1)
			{
				for (i2=0;i2<4;i2++)
				{
					h=NULL;
					data=NULL;
					h=sprite_add_file("bshoot.png",1,PR_ENEMY);
					h->type=SP_EN_LASER;
					h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
					h->mover=enemy_laser_move;
					h->aktframe=0;
					h->x=s->x+(s->w/2-h->w/2);
					h->y=s->y+(s->h/2-h->h/2);

					data=mmalloc(sizeof(BULLET_DATA));
					h->data=data;

//					data->id=rand()%1000;
					data->angle=p+(1-i2%2*2)*M_PI/12;
					data->speed=2+i2/2;
				}
			}
		}
		if (d->n>3)
			d->state=3;
		break;
	case 3: 	//退場準備
		if (d->level<3)
		{	s->aktframe=(s->aktframe&(4-1))+16;}
		else if (d->level<7)
		{	s->aktframe=(s->aktframe&(4-1))+8;}
		else
		{	s->aktframe=(s->aktframe&(4-1))+16;}
		d->state=4;
		d->wait=0;
		break;
	case 4: 	//退場
		if (d->wait>50)
		{	s->type=-1;}
		else{
			if (d->level<3)
			{	s->x+=2*fps_factor;}
			else if (d->level<7)
			{	s->y-=2*fps_factor;}
			else
			{	s->x-=2*fps_factor;}
		}
		break;
	}

	if (!d->ani_turn)
	{
		if ((s->aktframe&(4-1))==3)
		{
			d->ani_turn=1;
			s->aktframe--;
		}
		s->aktframe++;
	}
	else
	{
		if ((s->aktframe&(4-1))==0)
		{
			d->ani_turn=0;
			s->aktframe++;
		}
		s->aktframe--;
	}
}

void enemy_splash_add(int lv)
{
	SPRITE *s;
	SPLASH_DATA *data;

	s=sprite_add_file("splash.png",20,PR_ENEMY);	s->anim_speed=0;
	s->type=SP_EN_SPLASH;
	s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	s->mover=enemy_splash_move;
	data=mmalloc(sizeof(SPLASH_DATA));
	s->data=data;
	data->b.score=score(50*2);
	data->b.health=20;
	data->state=0;
	data->level=lv;
	data->wait=0;
	data->ani_turn=0;
	data->n=0;
	switch (lv)
	{
	case 0: 	//右下
		s->x=WIDTH2-s->w+20;
		s->y=100;
		s->aktframe=0;
		break;
	case 1: 	//右中
		s->x=WIDTH2-s->w+40;
		s->y=70;
		s->aktframe=0;
		break;
	case 2: 	//右上
		s->x=WIDTH2-s->w+60;
		s->y=40;
		s->aktframe=0;
		break;
	case 3: 	//上右右
		s->x=300-s->w/2;
		s->y=-30;
		s->aktframe=8;
		break;
	case 4: 	//上右
		s->x=220-s->w/2;
		s->y=-50;
		s->aktframe=8;
		break;
	case 5: 	//上左
		s->x=160-s->w/2;
		s->y=-50;
		s->aktframe=8;
		break;
	case 6: 	//上左左
		s->x=80-s->w/2;
		s->y=-30;
		s->aktframe=8;
		break;
	case 7: 	//左上
		s->x=-20;
		s->y=40;
		s->aktframe=16;
		break;
	case 8: 	//左中
		s->x=-40;
		s->y=70;
		s->aktframe=16;
		break;
	case 9: 	//左下
		s->x=-60;
		s->y=100;
		s->aktframe=16;
		break;
	}
}
