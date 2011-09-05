
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	道中のザコ
	-------------------------------------------------------
		"竜巻1",		"PROBALL",
	-------------------------------------------------------
	-------------------------------------------------------
→	↓	 ←
↑. ↓	/↑
↑ .↓ / ↑
↑	↓/  ↑
←←++→→
   /  .
---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_tatsumaki1(OBJ *src)
{
	if (512 > src->jyumyou)/* 下へ移動 */
	{
		src->cy256 += t256(1.25);/*fps_factor*/
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	}
	else
	if (512+512 > src->jyumyou)/* 上へ移動 */
	{
		src->cy256 -= t256(1.125);/*fps_factor*/
		if (0 > src->cy256 )
		{
		//	src->speed256 = (-(src->speed256));/* 反転 */
			src->jyumyou = (512-1);
		}
	}
	else
//	if (512+512+512 > src->jyumyou)/* 下へ移動 */
	{
		src->cy256 += t256(1.0);/*fps_factor*/
		if (t256(GAME_HEIGHT/3/*2*/) < src->cy256 ) /*t256(250)*/
		{
			src->jyumyou = (512+512-1);
		}
	}
	//
	src->speed256++;	/* t256(0.1) t256(0.2)速すぎ */
	src->px256 -= (src->speed256);/*fps_factor*/
	if ((0 > src->px256)||(t256(GAME_WIDTH-10) < src->px256))
	{
		src->speed256 = (-(src->speed256));/* 反転 */
	}
//
	if (0 < (src->vx_r) )
			{	src->cx256	= (src->px256); 	}
	else	{	src->cx256	= t256(GAME_WIDTH)-(src->px256); }
	//
	/* アニメーション */
	src->zako_anime_rotate_angle1024 = (src->vx_r);
	zako_anime_type04(src);
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

static void regist_zako_003_tatsumaki1(GAME_COMMAND *l, OBJ *h)
{
//	h->rotationCCW1024		= ((i&(16-1))<<6);
//
//	h->jyumyou = (512+512+512-1);
	h->speed256 			= t256(1.2);/* t256(2.0)速すぎ */	/*3+difficulty+lv/3*/
	h->px256				= t256(GAME_WIDTH-10);	/*0*/	/*t256(GAME_WIDTH/2)*/
	/* 画面右側の場合、左方向へ進む */
	if (t256((GAME_WIDTH)/2) < h->cx256)
	{
		h->vx_r = (10); /*左方向*/
	}
	else
	{
		h->vx_r = (-10);
	}
}
