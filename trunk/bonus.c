#include "bonus.h"

extern double fps_factor;
extern SDL_Surface *screen;
extern SPRITE *player;		//***090116		’Ç‰Á

void bonus_multi_add(int x, int y,int type, int num){

int i;

for(i=0;i<num;i++){
		SPRITE *s;
	BONUS_DATA *data;
switch(type) {
	case SP_BONUS_FIREPOWER:
		s=sprite_add_file("bonus_p.png",16,PR_BONUS);
		s->anim_speed=3;
		break;
	case SP_BONUS_ADDSPEED:
		s=sprite_add_file("bonus_s.png",16,PR_BONUS);
		s->anim_speed=2;
		break;
	case SP_BONUS_BOMB:
		s=sprite_add_file("bonus_f.png",16,PR_BONUS);
		s->anim_speed=3;
		break;
	case SP_BONUS_COIN:
		s=sprite_add_file("coin.png",20,PR_BONUS);
		s->anim_speed=3;
		break;
	case SP_BONUS_EXTRA:
		s=sprite_add_file("bonus_x.png",16,PR_BONUS);
		s->anim_speed=3;
		break;
	case SP_BONUS_HEALTH:
		s=sprite_add_file("bonus_h.png",9,PR_BONUS);
		s->anim_speed=2;
		break;
	default:
		CHECKPOINT;
		error(ERR_WARN,"cant add unknown bonus! debug me! please! now!");
		return;
	}

	s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	
	s->x = x + (rand()%100-50);
	s->y = y + (rand()%60-30);
	
	if(x<480)
		s->x=s->x+20;
	else
		s->x=s->x-10;
	//s->y=y;
	s->type=type;
	s->mover=bonus_move;
	data=mmalloc(sizeof(BONUS_DATA));
	s->data=data;
	data->sum=-5;
	data->gra=0.08;
	data->pl_up=0;
}
}

void bonus_add(int x, int y, int type)
{
	SPRITE *s;
	BONUS_DATA *data;

	switch(type) {
	case SP_BONUS_FIREPOWER:
		s=sprite_add_file("bonus_p.png",16,PR_BONUS);
		s->anim_speed=3;
		break;
	case SP_BONUS_ADDSPEED:
		s=sprite_add_file("bonus_s.png",16,PR_BONUS);
		s->anim_speed=2;
		break;
	case SP_BONUS_BOMB:
		s=sprite_add_file("bonus_f.png",16,PR_BONUS);
		s->anim_speed=3;
		break;
	case SP_BONUS_COIN:
		s=sprite_add_file("coin.png",20,PR_BONUS);
		s->anim_speed=3;
		break;
	case SP_BONUS_EXTRA:
		s=sprite_add_file("bonus_x.png",16,PR_BONUS);
		s->anim_speed=3;
		break;
	case SP_BONUS_HEALTH:
		s=sprite_add_file("bonus_h.png",9,PR_BONUS);
		s->anim_speed=2;
		break;
	default:
		CHECKPOINT;
		error(ERR_WARN,"cant add unknown bonus! debug me! please! now!");
		return;
	}
	s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	if(x<600)
		s->x=x+20;
	else
		s->x=x-10;
	s->y=y;
	s->type=type;
	s->mover=bonus_move;
	data=mmalloc(sizeof(BONUS_DATA));
	s->data=data;
	data->sum=-2;
	data->gra=0.08;
	data->pl_up=0;
}

void bonus_move(SPRITE *s)
{
	BONUS_DATA *d=(BONUS_DATA *)s->data;
	if(((PLAYER_DATA *)player->data)->bonus)
		d->pl_up=1;
	if(!d->pl_up)
	{
		if(d->sum<4)
			d->sum+=d->gra;
		s->y+=d->sum*fps_factor;
		if(s->y>272) s->type=-1; //denis 480
	}
	else
	{
		d->sum-=4;
		if(d->sum<0)
		{	
			d->gra=atan2(player->y-s->y,player->x-s->x);
			d->sum=20;
		}
		s->x+=cos(d->gra)*15*fps_factor;
		s->y+=sin(d->gra)*15*fps_factor;
	}
}

void bonus_info_add(int x, int y, char *filename)
{
	SPRITE *c;
	c=sprite_add_file(filename,1,PR_BONUS);
	c->flags=SP_FLAG_VISIBLE;
	c->type=SP_ETC;
	c->x=x;
	c->y=y;
	c->alpha=255;
	c->mover=bonus_info_move;
}

void bonus_info_move(SPRITE *c)
{
	if(c->alpha>=3*fps_factor) {
		c->alpha-=3*fps_factor;
		c->y-=fps_factor;
	} else {
		c->type=-1;
	}
}

void bonus_info_text(int x, int y, char *text, int font)
{
	SDL_Surface *t;
	SPRITE *s;

	t=font_render(text,font);
	s=sprite_add(t,1,PR_BONUS,1);
	s->flags|=SP_FLAG_FREESURFACE|SP_FLAG_VISIBLE;
	s->type=SP_ETC;
	s->x=x;
	s->y=y;
	s->alpha=255;
	s->data=mmalloc(sizeof(BIT_DATA));
	((BIT_DATA *)s->data)->distance=255;
	s->mover=bonus_infotext_move;
}

void bonus_infotext_move(SPRITE *c)
{
	BIT_DATA *b=(BIT_DATA *)c->data;
	 SDL_Surface *s;

	c->y+=fps_factor*2;
	b->distance-=fps_factor*3;
	c->alpha=b->distance;
	if(b->distance<=0) {
		 //s=sprite_getcurrimg(c);
		 parsys_add(s,1,1,c->x,c->y,30,0,0,100,PIXELIZE,NULL);
		c->type=-1;
	}
		
}
