#include "enemy.h"

typedef struct
{
	int state;
	double speed;
	double wait;
	int level;
	int n;
} MAGICF_DATA;


static void enemy_magicformation_move(SPRITE *s)
{
	//double angle;
	MAGICF_DATA *d=(MAGICF_DATA *)s->data;
	int al,sp;
	switch (d->state)
	{
	case 0:
		if (d->wait<0){
			d->wait=5;
			d->state=1;
		}
		else
		{	d->wait-=fps_factor;}
		break;
	case 1:
		if (s->alpha<180)
		{	s->alpha+=4*fps_factor;}
		else{
			d->state=2;
			s->alpha=180;
		}
		break;
	case 2:
		if (d->n){		//[***090128		•ÏX
			if (d->wait<0)
			{
				playChunk(10);
				al=rand()%360;
				#if 0
				/* sp ‚Í –w‚Ç2‚©3‚ÅA–ñ1/80‚ÌŠm—¦‚Å‹É‹H‚É1‚ðo‚· */
				sp=rand()%3/*+1-1*/;	/* 0 1 2 */
				if (sp==0/*<2-1*/)
				{
					sp=rand()%3/*+1-1*/;
					if (sp==0/*<2-1*/)
					{
						sp=rand()%3/*+1-1*/;
						if (sp==0/*<2-1*/)
						{
							sp=rand()%3/*+1-1*/;
						}
					}
				}
				#else
				sp=rand()%80;
				if (0!=sp)
				{
					sp = (sp&1)+1;
				}
				#endif
				sp++;	/* sp = 1 or 2 or 3 */
				enemy_stop_bullet_create(s, sp, degtorad(al), 0.04);
				d->wait=5;
				d->n--;
			}
			else
			{	d->wait--;}
		}
		else
		{	d->state=3;}
		break;
	case 3:
		if (s->alpha>0)
		{	s->alpha-=fps_factor*2;}
		else
		{
			s->type=-1;
			s->alpha=0;
		}
		break;
	}
}

void enemy_magicformation_add_xy(const short xxx, const short yyy, const short speed256)
{
	SPRITE *s;
	MAGICF_DATA *data;
	s=sprite_add_file("grounder.png",9,PR_GROUNDER);
	s->anim_speed=2;
	s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	s->mover=enemy_magicformation_move;
	s->aktframe=0;
	s->type=SP_PLAYER2;
	s->x=xxx;
	s->y=-s->h+yyy;
	s->alpha=0;
	data=mmalloc(sizeof(MAGICF_DATA));
	s->data=data;
	data->state=0;
	data->wait=10;
	data->n=10+7*difficulty;
	data->level=0;
}
