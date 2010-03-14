
#include "douchu.h"

/*---------------------------------------------------------
	誘導ミサイル敵
	-------------------------------------------------------
誘導ホーミングにしようと思ったのに
なぜかショット風に(作成中)
---------------------------------------------------------*/

#define tx256		user_data00 	/* 目標地点(出現時のプレイヤー位置) */
#define ty256		user_data01 	/* 目標地点(出現時のプレイヤー位置) */
#define time_out	user_data02
//#define speed256	user_data03
//#define state 	user_data04
#define radius		user_data03 	/* 半径 */

#define ax256		user_data04 	/* 量 */
#define ay256		user_data05 	/* 量 */

#if 0
/*---------------------------------------------------------
	自機狙い弾の角度を計算
---------------------------------------------------------*/
static int angle_jikinerai512(SPRITE *p, SPRITE *t)
{
	return (atan_512(p->y256-t->y256, p->x256-t->x256));
	/* 偶数弾の場合に自機狙い(簡略版) 弾と自分が大きさが同じならずれない、違うとその分誤差になる */
}
#endif

/*---------------------------------------------------------
	敵の誘導ホーミングボム(六角形で橙色の奴)
---------------------------------------------------------*/

/* プレイヤーのいた位置に到達する基準時間 */
#define GENTEN_FRAME128 (128)
#define ZERO_FRAME256 (256)

static void enemy_homing_move(SPRITE *src)
{
	src->time_out--;	/*fps_factor*/
	/* 時間切れで終わり */
	if (0 > src->time_out)
	{
		src->type = SP_DELETE;
		return;
	}
	else
	if (GENTEN_FRAME128 > src->time_out) //if (src->range<=0)
	{
		gamen_gai_nara_osimai(src);/* 画面外ならおしまい */
	}
//
//	src->tx256 += /*src->v*/src->vx256;/*fps_factor*/
//	src->ty256 += /*src->v*/src->vy256;/*fps_factor*/
//
//	if ((GENTEN_FRAME128-16) < src->time_out) 	/* 一定時間まで */
//	if ((256+GENTEN_FRAME128-256-64+16) < src->time_out) 	/* 一定時間まで */
//	if ((256-16) < src->time_out) 	/* (遅い) 一定時間まで */
	if ((256+32) < src->time_out) 	/* 一定時間まで */
	{
		src->tmp_angleCCW512 = (atan_512(src->ty256-src->y256, src->tx256-src->x256));
		const int sabun_aaa = (src->tmp_angleCCW512-src->m_angleCCW512);
		if (0 > sabun_aaa/*(src->tmp_angleCCW512-src->m_angleCCW512)*/ )/* 0レジスタ+共通最適化 */
		{	/* 目標角度 < 制御角度 */
			if (256 > sabun_aaa/*(src->tmp_angleCCW512-src->m_angleCCW512)*/ )/* 共通最適化 */
			{
				src->m_angleCCW512 -= 5;
			}
		}
		else	/* 目標角度 > 制御角度 */
		{
			if (256 > -(sabun_aaa)/*(src->m_angleCCW512-src->tmp_angleCCW512)*/ )
			{
				src->m_angleCCW512 += 5;
			}
		}
		// 正弦関数による加減速
		{
			int radius_aaa;/* 速度 */
		//	radius_aaa = (src->time_out-GENTEN_FRAME128);
		//	radius_aaa = (1+16);
		//	radius_aaa = (cos512(((src->time_out-GENTEN_FRAME128)<<2)));
		//	radius_aaa = (cos512(((src->time_out-GENTEN_FRAME128)   )));
		//	radius_aaa = (cos512(((src->time_out-GENTEN_FRAME128)<<1)));
			radius_aaa = (cos512(((src->time_out-GENTEN_FRAME128)<<1)));
		//	src->vx256 = ((sin512((src->m_angleCCW512)) ) );	/*CCW*/
		//	src->vy256 = ((cos512((src->m_angleCCW512)) ) );
			src->vx256 = ((sin512((src->m_angleCCW512))*(radius_aaa))>>6/*>>8*/);	/*CCW*/
			src->vy256 = ((cos512((src->m_angleCCW512))*(radius_aaa))>>6/*>>8*/);
		}
	}
//
	/* 実移動 */
	src->x256 += src->vx256;	/*CCW*/
	src->y256 += src->vy256;
}


/*---------------------------------------------------------
	敵側の誘導ミサイルを作成する
---------------------------------------------------------*/

void bullet_create_enemy_homing(SPRITE *src)
{
	int i;
	for (i=0; i<(8)/*HOMING_NU M_OF_ENEMIES*/; i++)
	{
		SPRITE *h;
		h					= sprite_add_gu(TAMA_TYPE_HOMING16_PNG);
		h->type 			= /*TEKI_00_BOSS11*/TEKI_63_HOMING_STUFF;
		h->callback_mover	= enemy_homing_move;
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		h->x256 			= src->x256;
		h->y256 			= src->y256;
//
		h->m_angleCCW512	= ((i<<6));//	ミサイルの向き
//
		/* 目標地点 */
		h->tx256			= (player->x256);
		h->ty256			= (player->y256);
//
		/*h->base.*/h->base_score		= score(5*2);
		/*h->base.*/h->base_hp			= 1;		/* (???)なんか強い気がする */

		h->time_out 	= 256+GENTEN_FRAME128;
	}
}

