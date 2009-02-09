#include "enemy.h"

extern double fps_factor;
extern int difficulty;
extern SPRITE *player;

typedef struct {
	ENEMY_BASE b;
	int state;
	double speed;
} FAIRY_DATA;

void enemy_fairy_add(int lv)
{
	SPRITE *s;
	FAIRY_DATA *data;

	s=sprite_add_file("fairy.png",1,PR_ENEMY);
	s->type=SP_EN_FAIRY;
	s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	s->mover=enemy_fairy_move;
	s->anim_speed=0;
	data=mmalloc(sizeof(FAIRY_DATA));
	s->data=data;
	s->y=-30;
	s->x=lv*35+40;
	data->state=0;
	data->speed=0.7+0.3*difficulty;
	data->b.score=20;
	data->b.health=1;
}

void enemy_fairy_move(SPRITE *s)
{
	FAIRY_DATA *d=(FAIRY_DATA *)s->data;
	
	s->y+=d->speed*fps_factor;
	if(s->y>HEIGHT)
		s->type=-1;
	if(difficulty){
		switch(d->state){
			case 0:
				if(s->y>100){
					enemy_bullet_create(s, difficulty);
					d->state=1;
				}
				break;
			case 1:
				if(s->y>150){
					enemy_bullet_create(s, 1+difficulty);
					d->state=2;
				}
				break;
			case 2:
				if(s->y>200){
					enemy_bullet_create(s, 2+difficulty);
					d->state=3;
				}
				break;
			case 3:
				if(s->y>250){
					enemy_bullet_create(s, 3+difficulty);
					d->state=4;
				}
				break;
			case 4:
				if(s->y>270){
					enemy_n_way_bullet(s, "kugel.png", 0, 8, difficulty*5, atan2(player->y+player->h/2-s->y,player->x-player->w/2-s->x));
					d->state=5;
				}
				break;
			case 5:
				if(s->y>272){
					s->type=-1;
				}
				break;
		}
	}
}