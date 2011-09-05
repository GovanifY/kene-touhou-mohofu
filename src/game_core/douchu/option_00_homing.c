
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
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
	敵の誘導ホーミングボム(緑色の奴)
---------------------------------------------------------*/

/* プレイヤーのいた位置に到達する基準時間 */
#define GENTEN_FRAME128 (128)
#define ZERO_FRAME256	(256)

static void teki_homing_move(OBJ *src)
{
	if (GENTEN_FRAME128 > src->jyumyou)
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	}
//	if ((GENTEN_FRAME128-16) < src->jyumyou)	/* 一定時間まで */
//	if ((256+GENTEN_FRAME128-256-64+16) < src->jyumyou) /* 一定時間まで */
//	if ((256-16) < src->jyumyou)	/* (遅い) 一定時間まで */
	if ((256+32) < src->jyumyou)	/* 一定時間まで */
	{
		REG_00_SRC_X	= (src->target_x256);					/* 弾源x256 */
		REG_01_SRC_Y	= (src->target_y256);					/* 弾源y256 */
		REG_02_DEST_X	= ((src->cx256));
		REG_03_DEST_Y	= ((src->cy256));
		tmp_angleCCW65536_src_nerai();
		#define ROTATE_1024HARF 	(1024/2)/*半周*/
		const int sabun_aaa = ((deg65536to1024(HATSUDAN_03_angle65536))-src->rotationCCW1024);
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
		//		GENTEN_FRAME128-63, 	//-(((255)>>2)-GENTEN_FRAME128),/*(63==正確に狙う)	(r34互換)*/
		//	};
		//	int radius_aaa;/* 速度 */
			//------------------
			HATSUDAN_01_speed256	= (t256(1.0));
			HATSUDAN_03_angle65536	= (deg1024to65536((((src->jyumyou-(GENTEN_FRAME128-63))<<2))));
			sincos256();/*(破壊レジスタ多いので注意)*/
		//	radius_aaa = REG_03_DEST_Y;//sin_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
		//	src->vy256 = REG_02_DEST_X;//cos_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
			//------------------
			HATSUDAN_01_speed256	= ((REG_03_DEST_Y)<<2);
			HATSUDAN_03_angle65536	= (deg1024to65536(src->rotationCCW1024));
			sincos256();/*(破壊レジスタ多いので注意)*/
			src->vx256 = REG_03_DEST_Y;//sin_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
			src->vy256 = REG_02_DEST_X;//cos_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
			//------------------
		}
	}
	/* 実移動 */
	src->cx256 += (src->vx256);
	src->cy256 += (src->vy256);
}


/*---------------------------------------------------------
	敵側の誘導ミサイルを作成する
---------------------------------------------------------*/

global void bullet_create_teki_homing(OBJ *src)
{
	/* 目標地点 */
	OBJ *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER];
	//
	unsigned int i_angle;
	for (i_angle=0; i_angle<(1024); i_angle += (1024/8) )/* */
	{
		OBJ *h;
		h					= obj_add_A01_teki_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI_HOMING16_PNG;
			h->obj_type_set 		= ZAKO_63_HOMING_STUFF;
			h->callback_mover		= teki_homing_move;
			/* (r34u1)壊せるようにした */
			h->callback_hit_teki	= callback_hit_zako;		/* 「ザコに自弾があたった場合の処理」に、標準の処理を設定 */
			h->atari_hantei 		= (score(5*2)/*スコア兼用*/);
			h->base_hp				= (1);		/* 誘導ミサイルの硬さ。 */
			h->cx256				= (src->cx256);/* 発弾位置 */
			h->cy256				= (src->cy256);/* 発弾位置 */
	//
			h->rotationCCW1024		= (i_angle);// ミサイルの向き
			{
				h->target_x256			= (zzz_player->cx256);/* 目標位置 */
				h->target_y256			= (zzz_player->cy256);/* 目標位置 */
			}
			h->jyumyou		= (256+GENTEN_FRAME128);
		}
	}
}
