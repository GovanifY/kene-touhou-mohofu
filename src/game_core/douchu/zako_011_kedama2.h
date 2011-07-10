
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	道中のザコ
	-------------------------------------------------------
		"毛玉2",	"BADGUY",	(r27から新規追加)
	-------------------------------------------------------
	本家毛玉
---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_kedama2(SPRITE *src)
{
	if (512 > src->jyumyou)
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	}
	else
	if (512+64 > src->jyumyou)	/* 攻撃 */
	{
		zako_shot_supeka(src, (src->kougeki_type));
	}
	else
	{
		/* ばらばらに登場させる */
		if (
			(0 >= src->cx256)
			|| (t256(GAME_WIDTH) < src->cx256)
			#if (1==USE_X_HOUKOU)
			|| (0 >= src->cy256)
			|| (t256(GAME_HEIGHT) < src->cy256)
			#endif
		)
		{
			/*画面外*/;
		}
		else
		{
			src->jyumyou	= (512+64-1); /* 登場する */
		}
	}

	/* 移動する */
	src->cx256 += (src->vx256);
	src->cy256 += (src->vy256);
	/* アニメーション */
	src->zako_anime_rotate_angle1024 = ( 10);
	zako_anime_type04(src); 	/* グラ回転 */
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

static void regist_zako_011_kedama2(GAME_COMMAND *l, SPRITE *h)
{
	h->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0xaa);		/* 白っぽく */
	//
	h->cy256				= ((l->user_y)<<8);
	h->vx256				= ((l->user_x));
	h->vy256				= (0);
	//
	{
		if (0 < (h->vx256))
		{/* [引数 user_y が正方向の場合、右へ移動(→)] [引数yが負方向の場合、下へ移動(↓)] */
			h->cx256			= t256(-30)/*-(i<<(5+8))*/;
		}
		else
		{/* [引数 user_y が正方向の場合、左へ移動(←)] [引数yが負方向の場合、上へ移動(↑)] */
			h->cx256			= t256(360)/*+(i<<(5+8))*/;/* 360 > 352(x_max) > 272(y_max) */
		}
	}
	//
	if (0 < (h->cy256))
	{	;	}
	else
	{
		/* y座標が負方向の場合は、x座標指定なので(x座標 y座標 を逆にする) */
		{
			int s_sss;
			s_sss				= h->cx256;
			h->cx256			= -(h->cy256);
			h->cy256			= s_sss;
		}
		h->vy256			= (h->vx256);
		h->vx256			= (0);
	}
//	h->jyumyou			= 512+64+512+64;	/* 適当に大きな値 */	/*+(i<<4)*/
	h->kougeki_type 	= ZAKO_SPEKA_00_hai_kedama1 + ((((l->user_kougeki_type)&1)<<2)|((cg.game_difficulty)/*&0x03*/));
}
