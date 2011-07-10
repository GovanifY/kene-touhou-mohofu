
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	誘導ミサイル敵
	-------------------------------------------------------
	誘導ホーミングにしようと思ったのに
	なぜかショット風に(作成中)
	色々未完成。
---------------------------------------------------------*/

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

static void teki_homing_move(SPRITE *src)
{
	/* 時間切れで終わり */
//	if (0 > src->jyumyou)
//	{
//		src->jyumyou = JYUMYOU_NASI;
//		return;
//	}
//	else
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
		REG_00_SRC_X 	= (src->target_x256);					/* 弾源x256 */
		REG_01_SRC_Y 	= (src->target_y256);					/* 弾源y256 */
		REG_02_DEST_X	= ((src->cx256));
		REG_03_DEST_Y	= ((src->cy256));
		tmp_angleCCW65536_src_nerai();
		src->tmp_angleCCW1024 = (deg65536to1024(HATSUDAN_03_angle65536));		/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
		#define ROTATE_1024HARF 	(1024/2)/*半周*/
		const int sabun_aaa = (src->tmp_angleCCW1024-src->rotationCCW1024);
		if (0 > (sabun_aaa) )/* 0レジスタ+共通最適化 */
		{
			/* 目標角度 < 制御角度 */
			if ((ROTATE_1024HARF) > (sabun_aaa) )/* 共通最適化 */
			{
				src->rotationCCW1024 -= (10);
			}
		}
		else	/* 目標角度 > 制御角度 */
		{
			if ((ROTATE_1024HARF) > -(sabun_aaa) )
			{
				src->rotationCCW1024 += (10);
			}
		}
		#undef ROTATE_1024HARF
		// 正弦関数による加減速
		{
		//	const u8 ddd[4] =
		//	{
		//		GENTEN_FRAME128-31, 	//-(((255)>>2)-GENTEN_FRAME128),/*(まず狙わない。ほぼ見せるだけ)*/
		//		GENTEN_FRAME128- 0, 	//-(((255)>>2)-GENTEN_FRAME128),/*(ほぼ狙わない。大して難しくない)*/
		//		GENTEN_FRAME128-47, 	//-(((255)>>2)-GENTEN_FRAME128),/*(少し狙う。)*/
		//		GENTEN_FRAME128-63, 	//-(((255)>>2)-GENTEN_FRAME128),/*(63==正確に狙う)  (r34互換)*/
		//	};
			int radius_aaa;/* 速度 */
		//	radius_aaa = (src->jyumyou-GENTEN_FRAME128);
		//	radius_aaa = (1+16);
		//	radius_aaa = (cos1024(((src->jyumyou-GENTEN_FRAME128)<<3)));
		//	radius_aaa = (cos1024(((src->jyumyou-GENTEN_FRAME128)<<1)));
		//	radius_aaa = (cos1024(((src->jyumyou-GENTEN_FRAME128)<<2)));/*(r34互換)*/
		//	radius_aaa = (si n1024(((src->jyumyou+((255)>>2)-GENTEN_FRAME128)<<2)));/*(r34互換)*/
		//	radius_aaa = (si n1024(((src->jyumyou-ddd[(cg_game_di fficulty)])<<2)));
		{
			int sin_value_t256; 		//	sin_value_t256 = 0;
			int cos_value_t256; 		//	cos_value_t256 = 0;
			int256_sincos1024( (((src->jyumyou-(GENTEN_FRAME128-63))<<2)), &sin_value_t256, &cos_value_t256);/*(63==正確に狙う)  (r34互換)*/
			radius_aaa = (sin_value_t256);
		}
		//	src->vx256 = ((si n1024((src->rotationCCW1024)) ) ); /*CCW*/
		//	src->vy256 = ((co s1024((src->rotationCCW1024)) ) );
			#if (0)//
			src->vx256 = ((si n1024((src->rotationCCW1024))*(radius_aaa))>>(6)/*>>8*/);/*fps_factor*/ 	/* CCWの場合 */
			src->vy256 = ((co s1024((src->rotationCCW1024))*(radius_aaa))>>(6)/*>>8*/);/*fps_factor*/
			#else
			{
				int sin_value_t256; 	//	sin_value_t256 = 0;
				int cos_value_t256; 	//	cos_value_t256 = 0;
				int256_sincos1024( (src->rotationCCW1024), &sin_value_t256, &cos_value_t256);
				src->vx256 = ((sin_value_t256*(radius_aaa))>>(6)/*>>8*/);/*fps_factor*/
				src->vy256 = ((cos_value_t256*(radius_aaa))>>(6)/*>>8*/);/*fps_factor*/
			}
			#endif
		}
	}
	/* 実移動 */
	src->cx256 += (src->vx256);
	src->cy256 += (src->vy256);
}


/*---------------------------------------------------------
	敵側の誘導ミサイルを作成する
---------------------------------------------------------*/

global void bullet_create_teki_homing(SPRITE *src)
{
	unsigned int i;
	for (i=0; i<(8); i++)/* 8==HOMING_NU M_OF_ENEMIES */
	{
		SPRITE *h;
		h					= obj_add_01_teki_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI_HOMING16_PNG;
			h->type 				= ZAKO_63_HOMING_STUFF;
			h->callback_mover		= teki_homing_move;
			/* (r34u1)壊せるようにした */
			h->callback_hit_teki	= callback_hit_zako;		/* 「ザコに自弾があたった場合の処理」に、標準の処理を設定 */
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->cx256 				= (src->cx256);
			h->cy256 				= (src->cy256);
	//
			h->rotationCCW1024		= ((i<<7));//	ミサイルの向き
			/* 目標地点 */
			{
				SPRITE *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
				h->target_x256			= (zzz_player->cx256);
				h->target_y256			= (zzz_player->cy256);
			}
			/*h->base.*/h->base_score		= score(5*2);
			/*h->base.*/h->base_hp			= (1);		/* (???)なんか強い気がする */
			h->jyumyou		= (256+GENTEN_FRAME128);
		}
	}
}
