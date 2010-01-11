
#include "bullet_object.h"

/*---------------------------------------------------------
		"緑毛玉2",		"CRUSHER",
	-------------------------------------------------------
	バグあり？？？
	-------------------------------------------------------
	緑毛玉	横一列に並んで、上から突っ込んでくる。
---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	int enemy_rank;
	int wait1;	/* 上下に動いたカウンタ */
	int wait2;	/* 繰り返した回数のカウンタ */
} MIDORI_KEDAMA2_DATA;
static int destoroy;
//static int enemy_rank;

#define NUM_OF_ENEMIES (9/*10*/)

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_midori_kedama2(SPRITE *src)
{
	destoroy++;
	if ( (NUM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
	{
		destoroy = 0;
		item_create(/*zzz*/ src, SP_ITEM_05_TENSU, 1, ITEM_MOVE_FLAG_06_RAND_XY);
	}
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_midori_kedama2(SPRITE *src)
{
	MIDORI_KEDAMA2_DATA *data = (MIDORI_KEDAMA2_DATA *)src->data;
	if (0 < data->wait2)
	{
		if (0 == data->wait1)
		{
		//	src->y256 += (t256(3)+t256(data->enemy_rank))/*(t256(3)+t256(data->enemy_rank))*/ /**fps_fa ctor*/;/*はやすぎ*/
			src->y256 += (t256(0.75)+((data->enemy_rank)<<6))/*(t256(3)+t256(data->enemy_rank))*/ /**fps_fa ctor*/;
			if (src->y256 > ((difficulty<<5) + t256(64)) )	/* 下限  t256(160) t256(200) */
			{
				data->wait1 = 1;
				data->wait2 -= 1;
			}
		}
		else
		{
		//	src->y256 -= (t256(2)+t256(data->enemy_rank))/*(t256(2)+t256(data->enemy_rank/3))*/ /**fps_fa ctor*/;/*はやすぎ*/
			src->y256 -= (t256(0.5)+((data->enemy_rank)<<5))/*(t256(2)+t256(data->enemy_rank/3))*/ /**fps_fa ctor*/;
			if (src->y256 < t256(32)) /* 上限  t256(50) */
			{
				data->wait1=0;
			}
		}
	//	if (/*0 <*/ /*data->*/enemy_rank)	/* easyは速いんだから場合分けしない */
		{
			/*
			0  0/100   0/65536
			1  1/90  728/65536
			2  1/80  819/65536
			3  1/70  936/65536
			*/
			static const Uint16 kakuritu_tbl[4] =
			{ 0, 728, 819, 936 };
		//	if (0==(ra_nd()%(100-data->enemy_rank*10)))
			if (kakuritu_tbl[data->enemy_rank] > (ra_nd()&(65536-1)))
			{
				bullet_create_aka_maru_jikinerai(src,  t256(2.5)+(data->enemy_rank<<6) );/*高速弾*/ 	//t256(3)+((/*data->*/enemy_rank<<7))
			}
		}
	}
	else
	{
	//	src->y256 -= t256(4);/*はやすぎ*/
		src->y256 -= t256(1.5);
		if (src->y256 < -((src->h128+src->h128)) )	/* 画面外に出たら */	/*-((src->w)<<8)*/
		{
		//	src->flags &= (~(SP_FLAG_VISIBLE));
			src->type = SP_DELETE;	/* おしまい */
		}
	}
//
	src->m_angleCCW512 += 5;
	mask512(src->m_angleCCW512);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_midori_kedama2(STAGE_DATA *l)/*int lv*/
{
	int enemy_rank; 	enemy_rank	= l->user_y;
//
	destoroy = 0;
	int x_offs256;
	x_offs256=(ra_nd()&((256*128)-1));
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
//		s						= sp rite_add_res(BASE_MIDOORI_KEDAMA16_PNG);	//s->anim_speed = 4;	/*15"cr usher.png"*/	/*負方向はない(i-5)*/
		s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);	//s->anim_speed = 4;	/*15"cr usher.png"*/	/*負方向はない(i-5)*/
		s->type 				= /*SP_ZAKO*/TEKI_60_AKA_KEDAMA/*_07_MIDORI_KEDAMA2*/;
//		s->type 				= SP_ZAKO/*_07_MIDORI_KEDAMA2*/;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_midori_kedama2;
		s->callback_loser		= lose_midori_kedama2;
		s->callback_hit_enemy	= callback_hit_zako;
//		s->an im_frame			= i/*%s->frames*/;
		s->x256 				= (i*t256(24))+(x_offs256)/*(i*t256(25))+t256(10)*/;
		if (i<=4)	{	s->y256 = -((  i)*t256(16/*10*/));}
		else		{	s->y256 = -((9-i)*t256(16/*10*/));}
		MIDORI_KEDAMA2_DATA *data;
		data					= mmalloc(sizeof(MIDORI_KEDAMA2_DATA));
		s->data 				= data;
		data->enemy_rank		= enemy_rank;
		/*data->base.*/s->base_score		= score(10*2)*(1+enemy_rank);
		/*data->base.*/s->base_health		= ((2*8)+(enemy_rank<<2)+(difficulty<<2));/* (1+enemy_rank+(difficulty<<2)) よわすぎ*/
		data->wait1 			= 0;
		data->wait2 			= 5;/*2*/
	}
}
#undef NUM_OF_ENEMIES
