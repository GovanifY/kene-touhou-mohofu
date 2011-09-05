
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	道中のザコ
	-------------------------------------------------------
	"メイド1",		"RWINGX",	rwingx
	-------------------------------------------------------
	上から横に等間隔で整列して現れ、プレイヤーに向かい
	ある程度近づくと、弾置いて反転して逃げる。
---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_meido1(OBJ *src)
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
			zako_shot_supeka(src, ZAKO_SPEKA_0a_midori1);
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
		REG_02_DEST_X	= ((src->cx256));
		REG_03_DEST_Y	= ((src->cy256));
		calculate_jikinerai();
		src->tmp_angleCCW1024 = (deg65536to1024(HATSUDAN_03_angle65536));		/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
	//	src->speed256	= /*t256(3+(teki_rank>>1))*/ /*(4+teki_rank)*/;/* 速過ぎ */
	}
	else
	if (512+2049 > src->jyumyou)/* 2048+1 */
	{
		if (t256(0.0) < src->speed256)
		{
			src->speed256 -= (5);/*t256(0.1)*/ /*減速*/
		}
	}
	else
	if (512+2109 > src->jyumyou)/* 2048+1+60 */
	{
		if (t256(1.0) > src->speed256)
		{
			src->speed256 += (5);/*t256(0.1)*/ /*加速*/
		}
	}
//
	/*以下rwingx.cと同じ*/
	zako_move_vector(src);
	/* アニメーション */
	zako_anime_type01(src);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

static void regist_zako_015_meido1(GAME_COMMAND *l, OBJ *h)
{
	h->tmp_angleCCW1024 	= (0);/* 下向き */		/* CCWの場合 */
//
	h->speed256 			= t256(0);//t256(0.5);
	h->jyumyou				= (512+2209);//(100);//20;
}
