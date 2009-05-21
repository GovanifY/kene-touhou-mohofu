
#include "enemy.h"

/*---------------------------------------------------------
		"緑毛玉2",		"CRUSHER",
	-------------------------------------------------------
	バグあり？？？
	-------------------------------------------------------
	緑毛玉	横一列に並んで、上から突っ込んでくる。
---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE b;
	int wait1;	/* 上下に動いたカウンタ */
	int wait2;	/* 繰り返した回数のカウンタ */
} MIDORI_KEDAMA2_DATA;
static int destoroy;
static int level;

#define NUM_OF_ENEMIES (9/*10*/)

/*---------------------------------------------------------

---------------------------------------------------------*/

static void lose_midori_kedama2(SPRITE *s)
{
	destoroy++;
	if ( (NUM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
	{
		destoroy = 0;
		item_create(/*zzz*/ s, SP_ITEM_06_TENSU, 1, ITEM_MOVE_FLAG_06_RAND_XY);
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_midori_kedama2(SPRITE *s)
{
	MIDORI_KEDAMA2_DATA *d=(MIDORI_KEDAMA2_DATA *)s->data;
	if (0 < d->wait2)
	{
		if (0 == d->wait1)
		{
		//	s->y256 += (t256(3)+t256(/*d->*/level))/*(t256(3)+t256(d->level))*/ /**fps_fa ctor*/;/*はやすぎ*/
			s->y256 += (t256(0.75)+((/*d->*/level)<<6))/*(t256(3)+t256(d->level))*/ /**fps_fa ctor*/;
			if (s->y256 > ((difficulty<<5) + t256(64)) )	/* 下限  t256(160) t256(200) */
			{
				d->wait1 = 1;
				d->wait2 -= 1;
			}
		}
		else
		{
		//	s->y256 -= (t256(2)+t256(/*d->*/level))/*(t256(2)+t256(d->level/3))*/ /**fps_fa ctor*/;/*はやすぎ*/
			s->y256 -= (t256(0.5)+((/*d->*/level)<<5))/*(t256(2)+t256(d->level/3))*/ /**fps_fa ctor*/;
			if (s->y256 < t256(32)) /* 上限  t256(50) */
			{
				d->wait1=0;
			}
		}
	//	if (/*0 <*/ /*d->*/level)	/* easyは速いんだから場合分けしない */
		{
			/*
			0  0/100   0/65536
			1  1/90  728/65536
			2  1/80  819/65536
			3  1/70  936/65536
			*/
			static const Uint16 kakuritu_tbl[4] =
			{ 0, 728, 819, 936 };
		//	if (0==(ra_nd()%(100-/*d->*/level*10)))
			if (kakuritu_tbl[/*d->*/level] > (ra_nd()&(65536-1)))
			{
				bullet_create_aka_maru_jikinerai(s,  t256(2.5)+(/*d->*/level<<6) );/*高速弾*/	//t256(3)+((/*d->*/level<<7))
			}
		}
	}
	else
	{
	//	s->y256 -= t256(4);/*はやすぎ*/
		s->y256 -= t256(1.5);
		if (s->y256 < -((s->h)<<8) )	/* 画面外に出たら */	/*-((s->w)<<8)*/
		{
			s->flags &= (~(SP_FLAG_VISIBLE));
		}
	}
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_midori_kedama2(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
	/*data->*/level 	= lv;
//
	destoroy = 0;
	int x_offs256;
	x_offs256=(ra_nd()&((256*128)-1));
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_MIDOORI_KEDAMA16_PNG);	//s->anim_speed	= 4;	/*15"cr usher.png"*/	/*負方向はない(i-5)*/
		s->type 			= SP_ZAKO/*_07_MIDORI_KEDAMA2*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_midori_kedama2;
		s->callback_loser	= lose_midori_kedama2;
		s->anim_frame		= i/*%s->frames*/;
		s->x256 			= (i*t256(24))+(x_offs256)/*(i*t256(25))+t256(10)*/;
		if (i<=4)	{	s->y256 = -((  i)*t256(16/*10*/));}
		else		{	s->y256 = -((9-i)*t256(16/*10*/));}
		MIDORI_KEDAMA2_DATA *data;
		data				= mmalloc(sizeof(MIDORI_KEDAMA2_DATA));
		s->data 			= data;
		data->b.score		= score(10*2)*(1+lv);
		data->b.health		= ((2*8)+(lv<<2)+(difficulty<<2));/* (1+lv+(difficulty<<2)) よわすぎ*/
		data->wait1 		= 0;
		data->wait2 		= 5;/*2*/
	}
}
#undef NUM_OF_ENEMIES
