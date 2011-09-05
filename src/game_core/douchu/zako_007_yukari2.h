
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
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

static void move_yukari2(OBJ *src)
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
			//------------------
			HATSUDAN_01_speed256	= (src->speed256);
		//	HATSUDAN_03_angle65536	= (HATSUDAN_03_angle65536);
			sincos256();/*(破壊レジスタ多いので注意)*/
			src->vx256 = REG_03_DEST_Y;//sin_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
			src->vy256 = REG_02_DEST_X;//cos_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
			//------------------
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

static void regist_zako_007_yukari2(GAME_COMMAND *l, OBJ *h)
{
	h->max_y256 			= (t256(GAME_HEIGHT)-t256(60));
	h->speed256 			= (t256(2.5)+(48) );/*3.0*/ /*4*/ /* 始めだけは速い */
	//
	set_REG_DEST_XY(h);
	calculate_jikinerai();
	//------------------
	HATSUDAN_01_speed256	= (h->speed256);
//	HATSUDAN_03_angle65536	= (HATSUDAN_03_angle65536);
	sincos256();/*(破壊レジスタ多いので注意)*/
	h->vx256 = REG_03_DEST_Y;//sin_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
	h->vy256 = REG_02_DEST_X;//cos_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
	//------------------
//
//	h->jyumyou				= byou60(60);/* テキトー */
}
