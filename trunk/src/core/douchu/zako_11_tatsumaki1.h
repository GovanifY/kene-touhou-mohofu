
/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
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
	敵やられ
---------------------------------------------------------*/

//static void lose_tatsumaki1(SPRITE *src)
//{
//	destoroy++;
//	if ( (N UM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
//	{
//		destoroy = 0;
//		if (rand_percent(50/*10*/))
//		{
//			item_create(/*zzz*/ src, (SP_ITEM_04_BOMB), 1, ITEM_MOVE_FLAG_06_RAND_XY);
//		}
//	}
//}
/*50%(SP_ITEM_01_P008 or SP_ITEM_04_BOMB) (SP_ITEM_01_P008+(ra_nd()&1)) */
//66%==SP_ITEM_04_BOMB or 33%==SP_ITEM_01_P008 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_tatsumaki1(SPRITE *src)
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
	src->rotationCCW1024 += (src->vx_r);
	if (0 < (src->vx_r) )
			{	src->cx256	= (src->px256); 	}
	else	{	src->cx256	= t256(GAME_WIDTH)-(src->px256); }
	mask1024(src->rotationCCW1024);
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_tatsumaki1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_62_TATSUMAKI;
		h->callback_mover		= move_tatsumaki1;
	//	h->callback_loser		= lose_tatsumaki1;/*old*/
	//	h->rotationCCW1024		= ((i&(16-1))<<6);
	//
//		h->jyumyou = (512+512+512-1);
		h->speed256 			= t256(1.2);/* t256(2.0)速すぎ */	/*3+difficulty+lv/3*/
		h->px256				= t256(GAME_WIDTH-10);	/*0*/	/*t256(GAME_WIDTH/2)*/
		/* 画面右側の場合、左方向へ進む */
		if ( t256((GAME_WIDTH)/2) < h->cx256)
		{
			h->vx_r = (10); /*左方向*/
		}
		else
		{
			h->vx_r = (-10);
		}
	}
}
