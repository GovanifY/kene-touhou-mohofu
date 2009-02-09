#include "enemy.h"

extern SPRITE *player;
extern double fps_factor;
extern int difficulty;

typedef struct {
	int state;
	double speed;
	double wait;
	int level;
	int n;
} MAGICF_DATA;

void enemy_magicformation_add(int lv, char wait)
{
	SPRITE *s;
	MAGICF_DATA *data;

	s=sprite_add_file("grounder.png",9,PR_GROUNDER);
	s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	s->mover=enemy_magicformation_move;
	s->anim_speed=2;
	s->aktframe=0;
	s->type=SP_PLAYER2;
	s->x=lv/1000;
	s->y=-s->h+lv%1000;
	s->alpha=0;
	data=mmalloc(sizeof(MAGICF_DATA));
	s->data=data;
	data->state=0;
	data->wait=10;
	data->n=10+7*difficulty;
	data->level=0;

}

void enemy_magicformation_move(SPRITE *s)
{
	//double angle;
	MAGICF_DATA *d=(MAGICF_DATA *)s->data;
	int al,sp;
	
	switch(d->state) {
		case 0:
			if(d->wait<0){
				d->wait=5;
				d->state=1;
			}
			else
				d->wait-=fps_factor;
			break;
		case 1:
			if(s->alpha<180)
				s->alpha+=4*fps_factor;
			else{
				d->state=2;
				s->alpha=180;
			}
			break;
		case 2:
			if(d->n){		//***090128		•ÏX
				if(d->wait<0)
				{
					playChunk(10);
					al=rand()%360;
					sp=rand()%3+1;
					if(sp<2){
						sp=rand()%3+1;
						if(sp<2){
							sp=rand()%3+1;
							if(sp<2){
								sp=rand()%3+1;
							}
						}
					}
					enemy_stop_bullet_create(s, sp, degtorad(al), 0.04);
					d->wait=5;
					d->n--;
				}
				else
					d->wait--;
			}
			else
				d->state=3;
			break;
		case 3:
			if(s->alpha>0)
				s->alpha-=fps_factor*2;
			else{
				s->type=-1;
				s->alpha=0;
			}
			break;
	}
}
