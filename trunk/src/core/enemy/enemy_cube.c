#include "enemy.h"

typedef struct
{
	ENEMY_BASE b;
	double radius;
	/*double*/int angle512;
	int flag0;
	int flag1;
	int flag2;
	int level;
} CUBE_DATA;

static void enemy_cube_move(SPRITE *s)
{
	CUBE_DATA *d=(CUBE_DATA *)s->data;
	d->flag2+=fps_factor;
	if (d->flag2<500)
	{
		if (!d->flag1)
		{
			d->radius+=fps_factor;
			if (d->radius>150) d->flag1=1;
		}
		else
		{
			d->radius-=fps_factor;
			if (d->radius<=10) d->flag1=0;
		}
		d->angle512 -= deg_360_to_512(3)/**fps_factor*/;
		mask512(d->angle512);	//if (d->angle360<0)		{	d->angle360+=360;}
		if (!(rand()&(512-1)/*%500*/))/* 1/512 ← 1/500 の確率で弾打つ */
		{
			enemy_bullet_create(s,1);
		}
		if (!(rand()&(512-1)/*%600*/))/* 1/512 ← 1/600 の確率確率でアイテム出す */
		{
			bonus_multi_add(s, SP_BONUS_00_FIREPOWER, 1, BONUS_FLAG_RAND_XY);
		}
	}
	else
	{
		d->radius+=2*fps_factor;
		if (d->radius>350)
		{	s->type=SP_DELETE;}
	}
	s->x=(co_s512((d->angle512))*d->radius)+GAME_WIDTH/2;		//ウィンドウ幅の変更
	s->y=(si_n512((d->angle512))*d->radius)+GAME_HEIGHT/2;
}

void enemy_cube_add(int lv)
{
	int i;
	for (i=0;i<16;i++)
	{
		SPRITE *s;
		s				= sprite_add_file("cube.png",16,PR_ENEMY);		s->anim_speed=1;
		s->type 		= SP_EN_CUBE;
		s->flags		|= (SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover		= enemy_cube_move;
		s->aktframe 	= 0/*i%s->frames*/;
		CUBE_DATA *data;
		data			= mmalloc(sizeof(CUBE_DATA));
		s->data 		= data;
		data->b.score	= score(15*2)*(1+lv);
		data->b.health	= 1+lv+(difficulty<<2);
		data->radius	= 350;
		data->angle512	= (i<<5);//  /*360*/(512/16)*i;
		data->flag0 	= 1;
		data->flag1 	= 0;
		data->flag2 	= 0;
		data->level 	= lv;
	}
}
