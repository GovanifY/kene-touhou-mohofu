
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"メイド1",		"RWINGX",	rwingx
	-------------------------------------------------------
	上から横に等間隔で整列して現れ、プレイヤーに向かい
	ある程度近づくと、弾置いて反転して逃げる。
---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_meido1(SPRITE *src)
{
	if (512+0 > src->jyumyou)
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	}
	else
	if (512+1024 > src->jyumyou)
	{
		src->speed256	+= t256(0.1);/*加速しながら プレイヤーに突っ込んでくる*/
		if (src->cy256 >= t256(GAME_HEIGHT-150))
		{
		//	if (teki_rank)
			{
				obj_send1->cx256						= (src->cx256);
				obj_send1->cy256						= (src->cy256);
				br.BULLET_REGIST_00_speed256			= t256(2.8);				/* 弾速 */	 // //t256(1.0)+t256(teki_rank)/*t256(3)+t256(teki_rank)*/	/* 速過ぎ */
			//	br.BULLET_REGIST_00_speed256			= speed256; 				/* 弾速 */
				br.BULLET_REGIST_02_VECTOR_angle1024	= ANGLE_JIKI_NERAI_DAN; 	/* 自機狙い弾 */	/* 発射中心角度 / 特殊機能(自機狙い/他) */
				br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_MINI8_01_AKA;		/* [赤弾] */ /* 弾グラ */
				br.BULLET_REGIST_05_regist_type 		= REGIST_TYPE_00_MULTI_VECTOR;
				br.BULLET_REGIST_06_n_way				= (1);						/* [1way] */
			//	br.BULLET_REGIST_07_VECTOR_div_angle1024		= (0);						/* ダミー角度(未使用) */
				bullet_regist_vector();
			}
			src->jyumyou = (512+1023);
			src->tmp_angleCCW1024 += (1024/2);	/* (1024/2) == [180/360 度] 反転して逃げる */
			mask1024(src->tmp_angleCCW1024);
		//	src->speed256 = /*t256(6+teki_rank)*/;/*速過ぎ*/
		}
	}
	else
	if (512+2048 > src->jyumyou)
	{
		/* プレイヤーに突っ込んでくる / schnell richtung player */
		src->jyumyou = (512+2047);
		tmp_angleCCW1024_jiki_nerai(src);
	//	src->speed256	= /*t256(3+(teki_rank>>1))*/ /*(4+teki_rank)*/;/* 速過ぎ */
	}
	else
	if (512+2049 > src->jyumyou)/* 2048+1 */
	{
		if (0 < src->speed256)
		{
			src->speed256 -= (5);/*t256(0.1)*/ /*減速*/
		}
	}
	else
	if (512+2109 > src->jyumyou)/* 2048+1+60 */
	{
		if ( src->speed256 < t256(1.0))
		{
			src->speed256 += (5);/*t256(0.1)*/ /*加速*/
		}
	}
//
	/*以下rwingx.cと同じ*/
/* CCWの場合 */
	src->cx256 += ((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
	src->cy256 += ((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
	/* アニメーション */
	zako_anime_type01(src, TEKI_36_YOUSEI3_1);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

static void regist_zako_015_meido1(GAME_COMMAND *l, SPRITE *h)
{
	h->tmp_angleCCW1024 	= (0);/* 下向き */		/* CCWの場合 */
//
	h->speed256 			= t256(0);//t256(0.5);
	h->jyumyou				= (512+2209);//(100);//20;
}
