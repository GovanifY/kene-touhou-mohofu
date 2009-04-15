#include "enemy.h"

typedef struct
{
	ENEMY_BASE b;
	int state;
	int tx;
	int ty;
	double speed;
	int level;
	int wait;		//[***090124		追加
	int n;
} GROUNDER_DATA;


static void enemy_grounder_move(SPRITE *s)	//[***090124		攻撃パターンを変える
{
	GROUNDER_DATA *d=(GROUNDER_DATA *)s->data;
	switch (d->state)
	{
	case 0:
		if ((s->y >= player->y)||(s->y>100))
		{
			d->state=1;
			enemy_bullet_create(s,1);
		}
		break;/*??? [***090210 追加 */
	case 1:
		if (s->y > 290)
		{
			s->type=SP_DELETE;
		}
	}
	if (d->n<(difficulty*7)+10) //[***090128		変更
	{
		if (d->wait<0)
		{
			playChunk(10);
			int sp;
			#if 0
			/* sp は 殆ど4で、約42%の確率で 1 2 3 を出す */
			sp=((rand()&(4-1))/*+1-1*/);/* 0 1 2 3 */
			if (sp<(4-1))
			{
				sp=((rand()&(4-1))/*+1-1*/);/* 0 1 2 3 */
				if (sp<(4-1))
				{
					sp=((rand()&(4-1))/*+1-1*/);/* 0 1 2 3 */
					/* この時点で 0 1 2 が出る確率は(0.75*0.75*0.75==0.421875%) */
				}
			}
			sp++;
			#else
			#define rand_percent(aaa) ((unsigned char)(rand())<=(unsigned char)( (aaa*256)/100 ) ) /* aa%の確率で */
			if (rand_percent(56))	/*56%の確率で*/
			{
				sp=rand()&3;
				/* この時点で 1 2 3 が出る確率は(約42%==0.56*3/4) */
				if (0==sp)
				{
					sp = 4;/*残りは4*/
				}
			}else
			{
				sp = 4;/*残りは4*/
			}
			#endif
			/* sp == 1 or 2 or 3 or 4 */
			enemy_stop_bullet_create(s, sp, /*deg512_2rad*/((rand()&(512-1)))/*de gtorad(rand()%360)*/, t256(0.03));
			d->wait=25-(difficulty*5);
		}
		else
		{	d->wait--;}
	}
	s->y+=fps_factor*d->speed;
}

void enemy_grounder_add_xy(const short xxx, const short yyy, const short speed256) //actually lv is the x coord
{
	SPRITE *s;
	s				= sprite_add_file("grounder.png",9,PR_GROUNDER); s->anim_speed=1;
	s->type 		= SP_EN_GROUNDER;
	s->flags		|= (SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	s->mover		= enemy_grounder_move;
	s->aktframe 	= 0;
	s->x			= xxx;
	s->y			= -s->h-yyy;
	GROUNDER_DATA *data;
	data			= mmalloc(sizeof(GROUNDER_DATA));
	s->data 		= data;
	data->b.score	= score(200*2);
	data->b.health	= 25+(difficulty<<3/*4*/);//+(difficulty*15);
	data->state 	= 0;
	data->tx		= player->x;
	data->ty		= player->y;
	data->wait		= 100;	//[***090124		追加
	data->n 		= 0;
	data->speed 	= t256_to_double(speed256);
	data->level 	= 0;
}
