
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	道中のザコ
	-------------------------------------------------------
		"メイド3",		"CURVER",	rwingx_curver
	-------------------------------------------------------
上から現れてS字をn回描いて横へ

←←
↓
→→→→→
		↓
←←←←←
↓
→→→→→
		↓
←←←←←
	-------------------------------------------------------
	バグあり。
---------------------------------------------------------*/


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_meido3(SPRITE *src)
{
	if (512 > src->jyumyou) 				// SS03:	/* 左(斜め上)へ移動中 */
	{
		if (0 < src->kaisu_nnn)
		{
			if (src->cx256 <= src->clip_left256)	{	src->kaisu_nnn--;	src->jyumyou = (512+512+512+512-1);/*SS00*/ 	}
		}
		else	/* 退場中 */
		{
			gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
		}
	}
	else
	if (512+512 > src->jyumyou) 			// SS02:	/* 右周りで回転中 */
	{
		src->tmp_angleCCW1024 -= src->turnspeed1024;/*fps_factor*/		/* CCWの場合 */
		mask1024(src->tmp_angleCCW1024);
		/* 左(斜め上)を向いたら */
	//	if ( (512+32) < src->tmp_angleCCW1024 ) 			//cv1024r(180 /*270*/ /*90+*/ /*180-90-90*/ /*+10*/)
		/* CCWの場合 */
		if ( 0!=(0x200 & src->tmp_angleCCW1024) )			/* (cv1024r(180) < src->tmp_angleCCW1024) */
		{
			if ( (512+256-32) > src->tmp_angleCCW1024 ) 	//cv1024r(180 /*270*/ /*90+*/ /*180-90-90*/ /*+10*/)
			{
			//	src->tmp_angleCCW1024 = cv1024r(180/*+10*/);
				src->jyumyou = (512-1);/*SS03*/
				zako_shot_supeka(src, ZAKO_SPEKA_1a_meido3);	/* easy以外は狙い弾を撃つ */
			}
		}
	}
	else
	if (512+512+512 > src->jyumyou) 		// SS01:	/* 右(斜め上)へ移動中 */
	{
		if (0 < src->kaisu_nnn)
		{
			if (src->cx256 >= src->clip_right256)	{	src->kaisu_nnn--;	src->jyumyou = (512+512-1);/*SS02*/  }
		}
		else	/* 退場中 */
		{
			gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
		}
	}
	else
//	if (512+512+512+512 > src->jyumyou) 	// SS00:	/* 左周りで回転中 */
	{
		src->tmp_angleCCW1024 += src->turnspeed1024;/*fps_factor*/		/* CCWの場合 */
		mask1024(src->tmp_angleCCW1024);
		/* 右(斜め上)を向いたら */
		/* CWの場合 */
	//	if ( (1024-32) < src->tmp_angleCCW1024 )		/*eps*/ /*<= cv1024r(0)*/	//cv1024r(10/*90*/ /*-90*/ /*360*/ /*-10*/
		/* CCWの場合 */
		if ( 0==(0x200 & src->tmp_angleCCW1024) )/* (cv1024r(180) > src->tmp_angleCCW1024) */
		{
			if ( (256+32) < src->tmp_angleCCW1024 ) 	/*eps*/ /*<= cv1024r(0)*/	//cv1024r(10/*90*/ /*-90*/ /*360*/ /*-10*/
			{
			//	src->tmp_angleCCW1024 = cv1024r(0/*360*/ /*-10*/)/*cv1024r(360)*/;
				src->jyumyou = (512+512+512-1);/*SS01*/
				zako_shot_supeka(src, ZAKO_SPEKA_1a_meido3);	/* easy以外は狙い弾を撃つ */
			}
		}
	}

	/*以下rwingx.cと同じ*/
	zako_move_vector(src);
	/* アニメーション */
	zako_anime_type01(src);
/*現在仕様上おかしい*/
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

static void regist_zako_017_meido3(GAME_COMMAND *l, SPRITE *h)
{
	h->speed256 			= t256(1.5);		/*3+difficulty*/ /*はやすぎ*/
	h->turnspeed1024		= (7*2);
	h->kaisu_nnn			= (4);
//
	h->jyumyou			 =	(1<<10);	/* (1024-1) or (2048-1) */		/*SS00*/
	h->clip_left256 	 = (t256(32+16));	/*50*/			/* 32,	224(128+64+32)	*/
	if (t256(GAME_WIDTH/2) > h->cx256 )
	{		/* 画面左側から出現 */
		h->tmp_angleCCW1024 	= cv1024r(180-90-10);			//	h->cx256				= -t256(32);
	}
	else	/* 画面右側から出現 */
	{
		h->tmp_angleCCW1024 	= cv1024r(180+90+10/*360-170*/);		//	h->cx256				= t256(GAME_WIDTH);
		h->jyumyou			+= (1<<(10)) ;	/* (1024-1) or (2048-1) */		/*SS00*/
		h->clip_left256 	+= (t256((128+64)));	/*50*/			/* 32,	224(128+64+32)	*/
	}
	h->clip_right256	= (h->clip_left256)+t256(128-32); /* 128, 320(256+64) */	 //  /*GAME_WIDTH-50-s->w*/
}

//	h->tmp_angleCCW1024 		= (256+128/*0*/);	/*256+16*/ /*cv1024r(270)*/;
