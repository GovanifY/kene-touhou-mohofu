
#include "enemy.h"

/*---------------------------------------------------------
		"‚¨‚Î‚¯1",		"DRAGER",
	-------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE b;
	int anim_dir;
	int anim_speed;
	int anim_count;
} OBAKE_DATA;

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_obake(SPRITE *s)
{
	OBAKE_DATA *d=(OBAKE_DATA *)s->data;

	d->anim_count++;
	if (d->anim_count==d->anim_speed)
	{
		d->anim_count=0;
		if (d->anim_dir==0)
		{
			s->anim_frame++;
			if (s->anim_frame==7/*10*/)
			{	d->anim_dir=1;}
		}
		else
		{
			s->anim_frame--;
			if (s->anim_frame==0)
			{	d->anim_dir=0;}
		}
	}
	s->y256 += t256(1.5)/**fps_fa ctor*/;
	if (s->x256 > t256(240))	{	s->x256 -= t256(0.6)/**fps_fa ctor*/;}
	else						{	s->x256 += t256(0.6)/**fps_fa ctor*/;}
	if (s->y256 > t256(GAME_HEIGHT))
	{	s->type=SP_DELETE;}
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

void add_zako_obake(STAGE_DATA *l)/*int lv*/
{
	int i;
	for (i=0;i<2;i++)
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_OBAKE08_PNG);	//	s->anim_speed=0;/*11"ir is.png"*/
		s->type 			= SP_ZAKO/*_01_OBAKE*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_obake;
		s->anim_frame		= 0;
		s->x256 			= t256(90)+t256(200)*i;
		s->y256 			= t256(-30);
		OBAKE_DATA *data;
		data				= mmalloc(sizeof(OBAKE_DATA));
		s->data 			= data;
		data->b.score		= score(15*2);
		data->b.health		= 3+(difficulty<<2);
		data->anim_dir		= 0;
		data->anim_speed	= 5;
		data->anim_count	= 0;
	}
}
