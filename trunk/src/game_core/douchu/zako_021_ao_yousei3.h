
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	道中のザコ
	-------------------------------------------------------
		"青妖精3",		"FAIRY",	(r27から新規追加)
	-------------------------------------------------------
	user_x: 	速度256[speed256]
	user_y: 	横位置[dots]
---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_ao_yousei3(SPRITE *src)
{
	if (512 > src->jyumyou) 	/* 離脱 */
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	}
	else
	{
		/* 攻撃 */
		if (0==((src->jyumyou)&0x3f))
		{
			if ((cg.game_difficulty))
			{
				zako_shot_supeka(src, ZAKO_SPEKA_1d_karasu);
			}
		}
	}
	/* 移動する */
	src->cx256 += (src->vx256);/*fps_factor*/
	#if (1==USE_X_HOUKOU_ao3)
	src->cy256 += (src->vy256);/*fps_factor*/
	#endif

	/* アニメーション */
	zako_anime_type02(src);
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

static void regist_zako_021_ao_yousei3(GAME_COMMAND *l, SPRITE *h)
{
	h->vx256				= ((h->cx256)>>8);
	#if (1==USE_X_HOUKOU_ao3)
	h->vy256				= (0);
	#endif
	//
	{
		if (0 < (h->vx256))
		{/* [引数 user_y が正方向の場合、右へ移動(→)] [引数yが負方向の場合、下へ移動(↓)] */
			h->cx256				= t256(-30);
		//	h->KARASU3_houkou		= ((0x00)>>2);
			h->KARASU3_houkou		= ((0x30)>>2);/*右向きカラス*/
		}
		else
		{/* [引数 user_y が正方向の場合、左へ移動(←)] [引数yが負方向の場合、上へ移動(↑)] */
			h->cx256				= t256(360);/* 360 > 352(x_max) > 272(y_max) */
			h->KARASU3_houkou		= ((0x50)>>2);/*左向きカラス*/
		}
	}
	//
	#if (1==USE_X_HOUKOU_ao3)
	if (0 < (h->cy256))
	{	;	}
	else
	{
		/* y座標が負方向の場合は、x座標指定なので(x座標 y座標 を逆にする) */
		{
			int h_hhh;
			h_hhh			= h->cx256;
			h->cx256		= -(h->cy256);
			h->cy256		= h_hhh;
		}
		h->vy256			= (h->vx256);
		h->vx256			= (0);
		h->KARASU3_houkou	= ((0x20)>>2);
	}
	#endif
	//
	h->jyumyou				= (512+320);
}
