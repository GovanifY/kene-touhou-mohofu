#include "enemy.h"

typedef struct
{
	ENEMY_BASE b;
	int state;
	int tx;
	int ty;
	double speed;
	int level;
} BGPANEL_DATA;

static void enemy_bgpanel_move(SPRITE *s)
{
	BGPANEL_DATA *d=(BGPANEL_DATA *)s->data;
	if (s->y > (/*272*/GAME_HEIGHT+2)/*274*/)
	{
		s->type=SP_DELETE;
	}
	s->y+=fps_factor*d->speed;
}

void enemy_bgpanel_type_add_xy(const short xxx, const short yyy, const short speed256, int type) //actually lv is the x coord
{
	SPRITE *s;
	s=NULL;
	switch (type)
	{
	case (1-1): s=sprite_add_file("bgpanel.png",3,PR_BACK0);	break;
	case (2-1): s=sprite_add_file("bgpanel2.png",1,PR_BACK0);	break;
	}
	s->type 		= SP_EN_BGPANEL;
	//s->flags		|= (SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	s->flags		|= (SP_FLAG_VISIBLE);
	s->mover		= enemy_bgpanel_move;
	s->anim_speed	= 6;
	s->aktframe 	= 0;
	s->x			= xxx;
	s->y			= -s->h-yyy;
	BGPANEL_DATA *data;
	data			= mmalloc(sizeof(BGPANEL_DATA));
	s->data 		= data;
	data->b.score	= score(10*2);
	data->b.health	= 2;/*???*/
	data->state 	= 0;
	data->tx		= player->x;
	data->ty		= player->y;
	data->speed 	= t256_to_double(speed256);
	data->level 	= 0;
}
