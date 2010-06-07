
#include "douchu.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"竜巻1",		"PROBALL",
	-------------------------------------------------------
	-------------------------------------------------------
→	↓	 ←
↑. ↓	/↑
↑ .↓ / ↑
↑	↓/  ↑
←←++→→
   /  .
---------------------------------------------------------*/
#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif

#define state			user_data04 	/* 状態 */
#define speed256		user_data05 	/* 速度 */
#define kaiten_houkou	user_data06 	/* 回転方向 */
#define px256			user_data07 	/* 仮想x位置(反転用) */


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
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
		break;
	}
	src->px256 -= (src->speed256);/*fps_factor*/
	if ((0 > src->px256)||(t256(GAME_WIDTH-10) < src->px256))
	{
		src->speed256 = (-(src->speed256));/* 反転 */
	}
//
	if (0 == src->kaiten_houkou)
			{	src->m_angleCCW1024 += 10;	src->x256	= (src->px256); 	}
	else	{	src->m_angleCCW1024 -= 10;	src->x256	= t256(GAME_WIDTH)-(src->px256); }
	mask1024(src->m_angleCCW1024);
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_tatsumaki1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_62_TATSUMAKI;
		h->callback_mover		= move_tatsumaki1;
	//	h->callback_loser		= lose_tatsumaki1;/*old*/
	//	h->m_angleCCW1024		= ((i&(16-1))<<6);
	//
		h->state				= SS00;
		h->speed256 			= t256(1.2);/* t256(2.0)速すぎ */	/*3+difficulty+lv/3*/
		h->px256				= t256(GAME_WIDTH-10);	/*0*/	/*t256(GAME_WIDTH/2)-((s->w128))*/
		/* 画面右側の場合、左方向へ進む */
		if ( t256((GAME_WIDTH)/2) < h->x256)
		{
			h->kaiten_houkou	= (1);	/*左方向*/
		}
		else
		{
			h->kaiten_houkou	= (0);
		}
	}
}
