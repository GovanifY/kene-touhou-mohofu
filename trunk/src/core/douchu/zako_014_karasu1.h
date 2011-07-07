
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"烏1",		"", (r27から新規追加)
	-------------------------------------------------------
	user_x: 	速度256[speed256]
	user_y: 	横位置[dots]
---------------------------------------------------------*/


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_karasu1(SPRITE *src)
{
	if ((JYUMYOU_ZAKO_HASSEI-320) > src->jyumyou)		/* 離脱 */
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	}
	else
	{
		/* 攻撃 */
		if (0==((src->jyumyou)&0x3f))
		{
			if ((cg_game_difficulty))
			{
				obj_send1->cx256							= (src->cx256);
				obj_send1->cy256							= (src->cy256);
				br.BULLET_REGIST_00_speed256				= (t256(2.0)+(((cg_game_difficulty))<<6));
				br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KUNAI12_00_SIRO+((ra_nd())&7);
				br.BULLET_REGIST_05_regist_type 			= REGIST_TYPE_00_MULTI_VECTOR;
				br.BULLET_REGIST_06_n_way					= (8);
				br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/(8));
				bullet_regist_vector();
			}
		}
	}
	/* 移動する */
	src->cx256 += (src->vx256);/*fps_factor*/
	#if (1==USE_X_HOUKOU_karasu)
	src->cy256 += (src->vy256);/*fps_factor*/
	#endif
	/* アニメ */
	{
	//	src->type				= (TEKI_12_YOUSEI1_1)+(src->KARASU3_houkou) | ((src->jyumyou>>2)&(4-1));
		zako_anime_type02(src, (TEKI_12_YOUSEI1_1)+(src->KARASU3_houkou));
	}
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

static void regist_zako_014_karasu1(GAME_COMMAND *l, SPRITE *h)
{
	h->cy256				= ((l->user_y)<<8);
	h->vx256				= ((l->user_x));
	#if (1==USE_X_HOUKOU_karasu)
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
	#if (1==USE_X_HOUKOU_karasu)
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
}
