
#include "douchu.h"
/*static*/extern void bullet_create_enemy_homing(SPRITE *src);

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"おばけ2"(ばけばけ) 	"虹毛玉1",		"EYEFO",
	-------------------------------------------------------
	バグあり？？？
---------------------------------------------------------*/
#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif

#define xcenter256			user_data00
#define ycenter256			user_data01
#define time_out			user_data03

#define radius256			user_data04
#define flag1				user_data05

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_obake2(SPRITE *src)
{
	src->time_out--;/*fps_factor*/
	if (0 > src->time_out)
	{
		src->type = SP_DELETE;	/* おしまい */
	}
	else
	if (600 > src->time_out)	/* 上へ帰る */
	{
		src->ycenter256 -= t256(1.0);/*fps_factor*/
	}
	else
	if (900 < src->time_out)	/* 下へ来る */
	{
	//	if (src->ycenter256 < t256(GAME_HEIGHT))
	//	if (src->ycenter256 < t256(difficulty<<6) )/* 難易度により下がってくる位置が違う。最大192(3*64) */
		if (src->ycenter256 < t256(difficulty<<5) )/* 難易度により下がってくる位置が違う。最大96(3*32) */
		{
			src->ycenter256 += t256(1.0);/*fps_factor*/
		}
	}
//
	if (0 == src->flag1)
	{
		src->radius256 += t256(1.0);	/*fps_factor*/
		if (src->radius256 > t256(64.0)/*150*/) /* 最大半径 */
		{
			src->flag1=1;
		}
	}
	else
	{
		src->radius256 -= t256(2.0);	/*fps_factor*/	/*1*/
		if (src->radius256 < t256(8.0)) /* 最小半径 */	/*<=10*/
		{
			src->flag1=0;
		}
	}
	src->tmp_angleCCW1024 += (2);	/*fps_factor*/	/* cv1024r(5) 速すぎる */
	mask1024(src->tmp_angleCCW1024);
/* CCWの場合 */
	src->x256 = ((sin1024((src->tmp_angleCCW1024))*(src->radius256))>>8)+src->xcenter256;
	src->y256 = ((cos1024((src->tmp_angleCCW1024))*(src->radius256))>>8)+src->ycenter256;

	if (0 != difficulty)/* easy はホーミング撃たない */
	{
	//	if (src->enemy_rank)
		{
		//	if (0==(ra_nd()%(1000-(src->enemy_rank)*100)))
		//	if (0==(ra_nd()%((10-(src->enemy_rank))*100)))
	//		if (0==(ra_nd()&((1<<(10-(src->enemy_rank)))-1)))/*テキトー*/	/* [10]確率(撃つ頻度)高すぎる気がする */
//			if (0==(ra_nd()&((1<<(12-(src->enemy_rank)))-1)))/*テキトー*/
			if (0==(ra_nd()&((1<<(11-(3/*src->enemy_rank*/)))-1)))/*テキトー*/
			{
				/* 画面外ならば撃たない */
				if (0 < src->y256)	/* 画面内なら撃つ */
				{
					bullet_create_enemy_homing(src);	/* 敵側の誘導ミサイルを作成する */
				}
			}
		}
	}
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_28_YOUSEI2_5+(((src->y256>>8)&0x03));
	}
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_obake2(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_28_YOUSEI2_5;
		h->callback_mover		= move_obake2;

		h->radius256			= t256(10);
		h->flag1				= 0;
		h->tmp_angleCCW1024 	= (ra_nd()&(1024-1));
		h->time_out 			= (1200);
		h->xcenter256			= t256( 62) + ((ra_nd()&((256*256)-1)))/*t256*/  /*(GAME_WIDTH/2)*/;/* GAME_WIDTH 380 ~= 380 == 62+256+62 */
		h->ycenter256			= t256(-32);/*-100*/
	}
}
