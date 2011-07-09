
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"青妖精2",		"SPLASH",
	-------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_ao_yousei2(SPRITE *src)
{
	if (50+10  < src->jyumyou)		/* 登場の動き */
	{
		src->cx256 += src->vx256;/*fps_factor*/
		src->cy256 += src->vy256;/*fps_factor*/
	}
	else
	if (50	  < src->jyumyou)		/* 少し待つ */
	{
		tmp_angleCCW65536_jiki_nerai(src);
		src->tmp_angleCCW1024 = ((src->tmp_angleCCW65536)>>6);		/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
		mask1024(src->tmp_angleCCW1024);
	/* アニメーション */
		zako_anime_type01(src);
	}
	else
	if (49	  < src->jyumyou)	/* 弾を撃つ */
	{
		{
			int k;
			for (k=0; k<6/*4*/; k++)
			{
				const static u16 bk1024_tbl[4] =
				{((int)(1024*1/8)), ((int)(1024*1/12)), ((int)(1024*1/16)), ((int)(1024*1/24))};
				br.BULLET_REGIST_00_speed256				= (t256(2.5)+(k<<6));					/* 弾速 */
				br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN; 				/* */
			//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
				br.BULLET_REGIST_04_bullet_obj_type 		= (BULLET_MINI8_01_AKA+(/*0*/6-k/*とりあえず*/)); /* [ 弾] */
				br.BULLET_REGIST_06_n_way					= (7);									/* [7way] */
				br.BULLET_REGIST_07_VECTOR_div_angle1024	= (bk1024_tbl[((cg_game_difficulty))]); 		/* 分割角度 */
				bullet_regist_multi_vector_send1_xy_src(src); 	/* 弾源x256 y256 中心から発弾。 */
			}
		}
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

static void regist_zako_020_ao_yousei2(GAME_COMMAND *l, SPRITE *h)
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
