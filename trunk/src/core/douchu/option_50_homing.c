
#include "douchu.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	誘導ミサイル敵
	-------------------------------------------------------
	誘導ホーミングにしようと思ったのに
	なぜかショット風に(作成中)
	色々未完成。
---------------------------------------------------------*/
#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define boss_time_out		user_data03 	/* 制限時間 */
#endif

#define target_x256 		user_data00 	/* 目標地点(出現時のプレイヤー位置) */
#define target_y256 		user_data01 	/* 目標地点(出現時のプレイヤー位置) */

#define radius				user_data04 	/* 半径 */
#define ax256				user_data05 	/* 量 */
#define ay256				user_data06 	/* 量 */


/*---------------------------------------------------------
	敵の誘導ホーミングボム(六角形で橙色の奴)
---------------------------------------------------------*/

/* プレイヤーのいた位置に到達する基準時間 */
#define GENTEN_FRAME128 (128)
#define ZERO_FRAME256 (256)

static void enemy_homing_move(SPRITE *src)
{
	/* 時間切れで終わり */
	if (0 > src->jyumyou)
	{
		src->jyumyou = JYUMYOU_NASI;
		return;
	}
	else
	if (GENTEN_FRAME128 > src->jyumyou) //if (src->range<=0)
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	}
//
//	src->target_x256 += /*src->v*/src->vx256;/*fps_factor*/
//	src->target_y256 += /*src->v*/src->vy256;/*fps_factor*/
//
//	if ((GENTEN_FRAME128-16) < src->jyumyou)	/* 一定時間まで */
//	if ((256+GENTEN_FRAME128-256-64+16) < src->jyumyou) /* 一定時間まで */
//	if ((256-16) < src->jyumyou)	/* (遅い) 一定時間まで */
	if ((256+32) < src->jyumyou)	/* 一定時間まで */
	{
		obj_send1->cx256 	= (src->target_x256);					/* 弾源x256 */
		obj_send1->cy256 	= (src->target_y256);					/* 弾源y256 */
		tmp_angleCCW1024_src_nerai(obj_send1, src);
		#define ROTATE_1024HARF 	(1024/2)/*半周*/
		const int sabun_aaa = (src->tmp_angleCCW1024-src->rotationCCW1024);
		if (0 > sabun_aaa/*(src->tmp_angleCCW1024-src->rotationCCW1024)*/ )/* 0レジスタ+共通最適化 */
		{
			/* 目標角度 < 制御角度 */
			if ((ROTATE_1024HARF) > sabun_aaa/*(src->tmp_angleCCW1024-src->rotationCCW1024)*/ )/* 共通最適化 */
			{
				src->rotationCCW1024 -= 10;
			}
		}
		else	/* 目標角度 > 制御角度 */
		{
			if ((ROTATE_1024HARF) > -(sabun_aaa)/*(src->rotationCCW1024-src->tmp_angleCCW1024)*/ )
			{
				src->rotationCCW1024 += 10;
			}
		}
		#undef ROTATE_1024HARF
		// 正弦関数による加減速
		{
			int radius_aaa;/* 速度 */
		//	radius_aaa = (src->jyumyou-GENTEN_FRAME128);
		//	radius_aaa = (1+16);
		//	radius_aaa = (cos1024(((src->jyumyou-GENTEN_FRAME128)<<3)));
		//	radius_aaa = (cos1024(((src->jyumyou-GENTEN_FRAME128)<<1)));
		//	radius_aaa = (cos1024(((src->jyumyou-GENTEN_FRAME128)<<2)));
		//	radius_aaa = (cos1024(((src->jyumyou-GENTEN_FRAME128)<<2)));
			radius_aaa = (cos1024(((src->jyumyou-GENTEN_FRAME128)<<2)));
		//	src->vx256 = ((sin1024((src->rotationCCW1024)) ) ); /*CCW*/
		//	src->vy256 = ((cos1024((src->rotationCCW1024)) ) );
			src->vx256 = ((sin1024((src->rotationCCW1024))*(radius_aaa))>>6/*>>8*/);	/*CCW*/
			src->vy256 = ((cos1024((src->rotationCCW1024))*(radius_aaa))>>6/*>>8*/);
		}
	}
//
	/* 実移動 */
	src->cx256 += src->vx256;	/*CCW*/
	src->cy256 += src->vy256;
}


/*---------------------------------------------------------
	敵側の誘導ミサイルを作成する
---------------------------------------------------------*/

global void bullet_create_enemy_homing(SPRITE *src)
{
	unsigned int i;
	for (i=0; i<(8)/*HOMING_NU M_OF_ENEMIES*/; i++)
	{
		SPRITE *h;
		h					= sprite_add_gu_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R		= ZAKO_ATARI_HOMING16_PNG;
			h->type 			= /*TEKI_00_BOSS11*/TEKI_63_HOMING_STUFF;
			h->callback_mover	= enemy_homing_move;
			h->flags			|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->cx256 			= src->cx256;
			h->cy256 			= src->cy256;
	//
			h->rotationCCW1024	= ((i<<7));//	ミサイルの向き
	//
			/* 目標地点 */
			{
				SPRITE *zzz_player;
				zzz_player = &obj00[FIX_OBJ_00_PLAYER];
				h->target_x256			= (zzz_player->cx256);
				h->target_y256			= (zzz_player->cy256);
			}
	//
			/*h->base.*/h->base_score		= score(5*2);
			/*h->base.*/h->base_hp			= (1);		/* (???)なんか強い気がする */

			h->jyumyou		= (256+GENTEN_FRAME128);
		}
	}
}

