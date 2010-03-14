
#include "douchu.h"

/*---------------------------------------------------------
		"緑毛玉2",		"CRUSHER",
	-------------------------------------------------------
	バグあり？？？
	-------------------------------------------------------
	緑毛玉	横一列に並んで、上から突っ込んでくる。
---------------------------------------------------------*/

#define wait1	user_data03 	/* 上下に動いたカウンタ */
#define wait2	user_data04 	/* 繰り返した回数のカウンタ */

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_midori_kedama2(SPRITE *src)
{
	if (0 < src->wait2)
	{
		if (0 == src->wait1)
		{
		//	src->y256 += (t256(3.00)+t256(data->enemy_rank));		/*fps_factor*/	/*(t256(3)+t256(data->enemy_rank))*/ /*はやすぎ*/
			src->y256 += (t256(0.75)+((3/*data->enemy_rank*/)<<6)); 	/*fps_factor*/	/*(t256(3)+t256(data->enemy_rank))*/
			if (src->y256 > ((difficulty<<5) + t256(64)) )			/* 下限  t256(160) t256(200) */
			{
				src->wait1 = 1;
				src->wait2 -= 1;
			}
		}
		else
		{
		//	src->y256 -= (t256(2.00)+t256(data->enemy_rank));			/*fps_factor*/	/*(t256(2)+t256(data->enemy_rank/3))*/ /*はやすぎ*/
			src->y256 -= (t256(0.50)+((2/*data->enemy_rank*/)<<5)); 	/*fps_factor*/	/*(t256(2)+t256(data->enemy_rank/3))*/
			if (src->y256 < t256(32))						/* 上限  t256(50) */
			{
				src->wait1 = 0;
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
		//	static const u16 kakuritu_tbl[4] =
		//	{ 0, 728, 819, 936 };
		//	if (0==(ra_nd()%(100-data->enemy_rank*10)))
		//	if (kakuritu_tbl[3/*data->enemy_rank*/] > (ra_nd()&(65536-1)))
			if (936 > (ra_nd()&(65536-1)))
			{
				bullet_create_aka_maru_jikinerai(src,
					t256(2.5)+(2/*data->enemy_rank*/<<6)
				);/*高速弾*/ 	//t256(3)+((/*data->*/enemy_rank<<7))
			}
		}
	}
	else
	{
	//	src->y256 -= t256(4);/*はやすぎ*/
		src->y256 -= t256(1.5);
		gamen_gai_nara_osimai(src);/* 画面外ならおしまい */
	}
//
	src->m_angleCCW512 += 5;
	mask512(src->m_angleCCW512);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_midori_kedama2(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_60_AKA_KEDAMA;
	add_zako_common(l, h);
	h->callback_mover		= move_midori_kedama2;
	h->wait1 				= 0;
	h->wait2 				= 5;/*2*/
}

//	h->callback_loser		= lose_midori_kedama2;
//	h->callback_hit_enemy	= callback_hit_zako;

//	destoroy = 0;
//	int x_offs256;
//	x_offs256=(ra_nd()&((256*128)-1));

//	h->x256 				= (i*t256(24))+(x_offs256); 	/*(i*t256(25))+t256(10)*/
//	if (i<=4)	{	h->y256 = -((  i)*t256(16/*10*/));}
//	else		{	h->y256 = -((9-i)*t256(16/*10*/));}

//	data->enemy_rank		= enemy_rank;
//	/*data->base.*/h->base_score		= score(10*2)*(1+1/*enemy_rank*/);
//	/*data->base.*/h->base_hp		= ((8*(2*8))+/*(enemy_rank<<(2+3))+*/(1/*di fficulty*/<<(2+3)));/* (1+enemy_rank+(di fficulty<<2)) よわすぎ*/
