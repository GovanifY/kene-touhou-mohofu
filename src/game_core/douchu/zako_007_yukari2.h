
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	道中のザコ
	-------------------------------------------------------
	"紫編隊2",		"GREETER",
	-------------------------------------------------------
	５機一列に続がり、編隊飛行してくるザコ敵
	-------------------------------------------------------
	ming_greeter
	rwingx_curver(ming)に似てる
---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_yukari2(SPRITE *src)
{
	if (byou60(10) > src->jyumyou)
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	}
	else
	{
		if (src->cy256 >= src->max_y256)
		{
			src->jyumyou = byou60(10);
			if (t256(1.5/*2.0*/) < src->speed256)
			{	/* 減速 */
				src->speed256 -= t256(0.2);
			//	src->speed256 = t256(2)/*3+difficulty*/;
			}
		//
			REG_00_SRC_X		= (t256(GAME_WIDTH/2)); 		/* 弾源x256 */
			REG_01_SRC_Y		= (t256(0));					/* 弾源y256 */
			set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
			tmp_angleCCW65536_src_nerai();
			#if (0)//
			src->vx256 = ((si n1024((deg65536to1024(HATSUDAN_03_angle65536)))*(src->speed256))>>8);/*fps_factor*/	/* CCWの場合 */
			src->vy256 = ((co s1024((deg65536to1024(HATSUDAN_03_angle65536)))*(src->speed256))>>8);/*fps_factor*/
			#else
			{
				int sin_value_t256; 	//	sin_value_t256 = 0;
				int cos_value_t256; 	//	cos_value_t256 = 0;
				int256_sincos1024( (deg65536to1024(HATSUDAN_03_angle65536)), &sin_value_t256, &cos_value_t256);	/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
				src->vx256		= ((sin_value_t256*(src->speed256))>>8);/*fps_factor*/
				src->vy256		= ((cos_value_t256*(src->speed256))>>8);/*fps_factor*/
			}
			#endif
		}
		zako_shot_supeka(src, ZAKO_SPEKA_0d_yukari);
	}
	/* 似てるがちょっと違う--以下rwingx.cと同じ */
	src->cx256 += (src->vx256);/* fps_factor */
	src->cy256 += (src->vy256);/* fps_factor */
	/* アニメーション */
	src->zako_anime_rotate_angle1024 = ( 10);
	zako_anime_type04(src); 	/* グラ回転 */
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

static void regist_zako_007_yukari2(GAME_COMMAND *l, SPRITE *h)
{
	h->max_y256 			= (t256(GAME_HEIGHT)-t256(60));
	h->speed256 			= (t256(2.5)+(48) );/*3.0*/ /*4*/ /* 始めだけは速い */
	//
	set_REG_DEST_XY(h);
	calculate_jikinerai();
	/* CCWの場合 */
	#if (0)//
	h->vx256		= ((si n1024((deg65536to1024(HATSUDAN_03_angle65536)))*(h->speed256))>>8);/*fps_factor*/ 	/* CCWの場合 */
	h->vy256		= ((co s1024((deg65536to1024(HATSUDAN_03_angle65536)))*(h->speed256))>>8);/*fps_factor*/
	#else
	{
		int sin_value_t256; 	//	sin_value_t256 = 0;
		int cos_value_t256; 	//	cos_value_t256 = 0;
		int256_sincos1024( (deg65536to1024(HATSUDAN_03_angle65536)), &sin_value_t256, &cos_value_t256);	/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
		h->vx256		= ((sin_value_t256*(h->speed256))>>8);/*fps_factor*/
		h->vy256		= ((cos_value_t256*(h->speed256))>>8);/*fps_factor*/
	}
	#endif
//
//	h->jyumyou				= byou60(60);/* テキトー */
}
