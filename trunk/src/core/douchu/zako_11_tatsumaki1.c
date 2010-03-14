
#include "douchu.h"

/*---------------------------------------------------------
		"竜巻1",		"PROBALL",
	-------------------------------------------------------
	バグあり？？？
	-------------------------------------------------------
→	↓	 ←
↑. ↓	/↑
↑ .↓ / ↑
↑	↓/  ↑
←←++→→
   /  .
---------------------------------------------------------*/

#define state			user_data02 	/* 状態 */
#define speed256		user_data03 	/* 速度 */
#define kaiten_houkou	user_data04 	/* 回転方向 */
#define px256			user_data05 	/* 仮想x位置(反転用) */


/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

//static void lose_tatsumaki1(SPRITE *src)
//{
//	destoroy++;
//	if ( (N UM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
//	{
//		destoroy = 0;
//		if (rand_percent(50/*10*/))
//		{
//			item_create(/*zzz*/ src, (SP_ITEM_04_BOMB), 1, ITEM_MOVE_FLAG_06_RAND_XY);
//		}
//	}
//}
/*50%(SP_ITEM_01_P008 or SP_ITEM_04_BOMB) (SP_ITEM_01_P008+(ra_nd()&1)) */
//66%==SP_ITEM_04_BOMB or 33%==SP_ITEM_01_P008 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
enum
{
	SS00 = 0,
	SS01,
	SS02,
};
static void move_tatsumaki1(SPRITE *src)
{
	src->speed256++;	/* t256(0.1) t256(0.2)速すぎ */
	switch (src->state)
	{
	case SS00:/* 下へ移動 */
		src->y256 += t256(1.0);/*fps_factor*/
		if (t256(GAME_HEIGHT/3/*2*/) < src->y256 )	/*t256(250)*/
		{
			src->state++;/* = SS01;*/
		}
		break;
	case SS01:/* 上へ移動 */
		src->y256 -= t256(1.125);/*fps_factor*/
		if (0 > src->y256 )
		{
		//	src->speed256 = (-(src->speed256));/* 反転 */
			src->state++;/*  = SS02;*/
		}
		break;
	case SS02:/* 下へ移動 */
		src->y256 += t256(1.25);/*fps_factor*/
		gamen_gai_nara_osimai(src);/* 画面外ならおしまい */
		break;
	}
	src->px256 -= (src->speed256);/*fps_factor*/
	if ((0 > src->px256)||(t256(GAME_WIDTH-10) < src->px256))
	{
		src->speed256 = (-(src->speed256));/* 反転 */
	}
//
	if (0 == src->kaiten_houkou)
			{	src->m_angleCCW512 += 5;	src->x256	= (src->px256); 	}
	else	{	src->m_angleCCW512 -= 5;	src->x256	= t256(GAME_WIDTH)-(src->px256); }
	mask512(src->m_angleCCW512);
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_tatsumaki1(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_62_TATSUMAKI;
	add_zako_common(l, h);
	h->callback_mover		= move_tatsumaki1;
//	h->callback_loser		= lose_tatsumaki1;
//	h->callback_hit_enemy	= callback_hit_zako;
//	h->m_angleCCW512		= ((i&(16-1))<<5);
//
	h->state 			= SS00;
	h->speed256			= t256(1.2);/* t256(2.0)速すぎ */	/*3+difficulty+lv/3*/
	h->px256 			= /*0*/t256(GAME_WIDTH-10)/*t256(GAME_WIDTH/2)-((s->w128))*/;
	/* 画面右側の場合、左方向へ進む */
	if ( t256((GAME_WIDTH)/2) < h->x256)
	{
		h->kaiten_houkou 	= 1;//(i&1);/*(0==i%2)?(0):(1)*/
		//	/*左方向*/
	}
	else
	{
		h->kaiten_houkou 	= 0;//(i&1);/*(0==i%2)?(0):(1)*/
	}
}

//	h->y256 				= -(((i+1)*(t256(24)/*s->h128+s->h128*/)));
//	/*h->base.*/h->base_score		= score(10*2);
//	/*h->base.*/h->base_hp		= (8*(4*8))+(1/*di fficulty*/<<(2+3));/*やわらかすぎ*/	/*(2+(di fficulty<<2))*/
//	h->enemy_rank 			= enemy_rank;

//#undef N UM_OF_ENEMIES
//	int enemy_rank;	enemy_rank	= l->user_y;
//
//	destoroy = 0;
