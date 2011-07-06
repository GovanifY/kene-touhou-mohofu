

/*static*/extern void bullet_create_enemy_homing(SPRITE *src);

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"おばけ2"(ばけばけ) 	"虹毛玉1",		"EYEFO",
	-------------------------------------------------------
	バグあり？？？
---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_obake2(SPRITE *src)
{
//	if (0 > src->jyumyou)
//	{
//		src->jyumyou = JYUMYOU_NASI;	/* おしまい */
//	}
//	else
	if (600 > src->jyumyou) 	/* 上へ帰る */
	{
		src->target_y256 -= t256(1.0);/*fps_factor*/
	}
	else
	if (900 < src->jyumyou) 	/* 下へ来る */
	{
	//	if (src->target_y256 < t256(GAME_HEIGHT))
	//	if (src->target_y256 < t256(difficulty<<6) )/* 難易度により下がってくる位置が違う。最大192(3*64) */
		if (src->target_y256 < t256(difficulty<<5) )/* 難易度により下がってくる位置が違う。最大96(3*32) */
		{
			src->target_y256 += t256(1.0);/*fps_factor*/
		}
	}
//
	if (0 == src->flag1)
	{
		src->radius256 += t256(1.0);	/*fps_factor*/
		if (src->radius256 > t256(64.0)/*150*/) /* 最大半径 */
		{
			src->flag1=1;
		}
	}
	else
	{
		src->radius256 -= t256(2.0);	/*fps_factor*/	/*1*/
		if (src->radius256 < t256(8.0)) /* 最小半径 */	/*<=10*/
		{
			src->flag1=0;
		}
	}
	src->tmp_angleCCW1024 += (2);	/*fps_factor*/	/* cv1024r(5) 速すぎる */
	mask1024(src->tmp_angleCCW1024);
	//
	zako_move_type11_radius(src);

	if (0 != difficulty)/* easy はホーミング撃たない */
	{
	//	if (src->enemy_rank)
		{
		//	if (0==(ra_nd()%(1000-(src->enemy_rank)*100)))
		//	if (0==(ra_nd()%((10-(src->enemy_rank))*100)))
	//		if (0==(ra_nd()&((1<<(10-(src->enemy_rank)))-1)))/*テキトー*/	/* [10]確率(撃つ頻度)高すぎる気がする */
//			if (0==(ra_nd()&((1<<(12-(src->enemy_rank)))-1)))/*テキトー*/
			if (0==(ra_nd()&((1<<(11-(3/*src->enemy_rank*/)))-1)))/*テキトー*/
			{
				/* 画面外ならば撃たない */
				if (0 < src->cy256)	/* 画面内なら撃つ */
				{
					bullet_create_enemy_homing(src);	/* 敵側の誘導ミサイルを作成する */
				}
			}
		}
	}
	/* アニメーション */
	{
	//	src->type			= TEKI_28_YOUSEI2_5+(((src->cy256>>8)&0x03));
		zako_anime_type02(src, TEKI_28_YOUSEI2_5);
	}
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_obake2(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_28_YOUSEI2_5;
		h->callback_mover		= move_obake2;

		h->radius256			= t256(10);
		h->flag1				= 0;
		h->tmp_angleCCW1024 	= (ra_nd()&(1024-1));
		h->jyumyou				= (1200);
		h->target_x256			= t256( 62) + ((ra_nd()&((256*256)-1)))/*t256*/  /*(GAME_WIDTH/2)*/;/* GAME_WIDTH 380 ~= 380 == 62+256+62 */
		h->target_y256			= t256(-32);/*-100*/
	}
}
