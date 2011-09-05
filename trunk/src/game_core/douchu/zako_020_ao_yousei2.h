
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	道中のザコ
	-------------------------------------------------------
		"青妖精2",		"SPLASH",
	-------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_ao_yousei2(OBJ *src)
{
	if (50+10  < src->jyumyou)		/* 登場の動き */
	{
		src->cx256 += src->vx256;/*fps_factor*/
		src->cy256 += src->vy256;/*fps_factor*/
	}
	else
	if (50	  < src->jyumyou)		/* 少し待つ */
	{
		REG_02_DEST_X	= ((src->cx256));
		REG_03_DEST_Y	= ((src->cy256));
		calculate_jikinerai();
		src->tmp_angleCCW1024 = (deg65536to1024(HATSUDAN_03_angle65536));		/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
		mask1024(src->tmp_angleCCW1024);
	/* アニメーション */
		zako_anime_type01(src);
	}
	else
	if (49	  < src->jyumyou)	/* 弾を撃つ */
	{
		zako_shot_supeka(src, ZAKO_SPEKA_1f_ao3_rendan);
		src->kaisu_nnn--;
		if (0 > src->kaisu_nnn)
		{
			;/* 退場準備 */
		}
		else
		{
			src->jyumyou = (50+10);/*戻る*/
		}
	}
	else	/* 退場の動き */
	{
		src->cx256 -= src->vx256;/*fps_factor*/
		src->cy256 -= src->vy256;/*fps_factor*/
	}

}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

static void regist_zako_020_ao_yousei2(GAME_COMMAND *l, OBJ *h)
{
	/* 登場の動き / 退場の動き */
		 if (t256(64)			 > h->cx256)	{	h->vx256 =	t256(2.0);	}	/* 出現位置が左なら、右へ移動 */
	else if (t256(GAME_WIDTH-64) < h->cx256)	{	;	}						/* 中心付近なら左右に移動しない */
	else										{	h->vx256 = -t256(2.0);	}	/* 出現位置が右なら、左へ移動 */

	h->vy256 = (0);

	#if 1//(1==USE_X_HOUKOU)
	if (0 < (h->cy256))
	{	;	}
	else
	{
		/* y座標が負方向の場合は、x座標指定なので(x座標 y座標 を逆にする) */
		{
			int s_sss;
			s_sss				=  (h->cx256);
			h->cx256			= -(h->cy256);
			h->cy256			= s_sss;
		}
		h->vy256			= (h->vx256);
		h->vx256			= (0);
	}
	#endif
	h->jyumyou					= (50+10+40);
	h->kaisu_nnn				= (3);	/* 3回撃つ */
}
