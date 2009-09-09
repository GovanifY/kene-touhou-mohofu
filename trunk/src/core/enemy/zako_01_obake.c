
#include "bullet_object.h"

/*---------------------------------------------------------
		"おばけ1",		"DRAGER",
	-------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE base;
	int anim_houkou;	/* アニメーション方向 */
	int anim_speed; 	/* アニメーション速度 */
	int anim_count; 	/* アニメーション現在再生コマ数 */
} OBAKE_DATA;

#define NUM_OF_ENEMIES (2)

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_obake(SPRITE *s)
{
	OBAKE_DATA *d = (OBAKE_DATA *)s->data;

	d->anim_count++;
	if (d->anim_count == d->anim_speed)
	{
		d->anim_count = 0;
		if (0 == d->anim_houkou)
		{
			s->anim_frame++;
			if (s->anim_frame == 7/*10*/)
			{	d->anim_houkou = 1;}
		}
		else
		{
			s->anim_frame--;
			if (s->anim_frame == 0)
			{	d->anim_houkou = 0;}
		}
	}
	if (s->x256 > t256(240))	{	s->x256 -= t256(0.6)/**fps_fa ctor*/;}
	else						{	s->x256 += t256(0.6)/**fps_fa ctor*/;}
	s->y256 += t256(1.5)/**fps_fa ctor*/;
	if (s->y256 > t256(GAME_HEIGHT))
	{
		s->type = SP_DELETE;
	}
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_obake(STAGE_DATA *l)/*int lv*/
{
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_OBAKE08_PNG); //	s->anim_speed=0;/*11"ir is.png"*/
		s->type 			= SP_ZAKO/*_01_OBAKE*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_obake;
		s->anim_frame		= 0;
		s->x256 			= t256(90)+t256(200)*i;
		s->y256 			= t256(-30);
		OBAKE_DATA *data;
		data				= mmalloc(sizeof(OBAKE_DATA));
		s->data 			= data;
		data->base.score	= score(15*2);
		data->base.health	= 3+(difficulty<<2);
		data->anim_houkou	= 0;
		data->anim_speed	= 5;
		data->anim_count	= 0;
	}
}
#undef NUM_OF_ENEMIES
