
/*---------------------------------------------------------
	東方模倣風 〜 Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
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
		{	enum
			{
				K00_KOUGEKI_KANKAKU_MASK = 0,
				K01_SUKIMA_KAKUDO,
				K02_TAMA_KAZU,
				K99_MAX/* 最大数 */
			};
			const u16 kougeki_tbl[(4*2)][K99_MAX] =
			{
				/* 0:紅っぽい毛玉(狭角度 奇数弾) */
				{0x3f, (u16)(1024/12), (5)},	/* (u32)(1024/12):広角なので簡単 */
				{0x1f, (u16)(1024/16), (3)},	/* 0x0f:普通に避けれる間隔 */
				{0x0f, (u16)(1024/20), (3)},	/* 0x0f:普通に避けれる間隔 */
				{0x07, (u16)(1024/24), (5)},	/* (u32)(1024/24):普通の狭角 */
				/* 1:妖っぽい毛玉(90度 4方向) (無駄弾なので難易度は低い筈) */
				{0x3f, (u16)(1024/8),  (8)},	/* 8:見せ弾(無駄弾なので難易度は低い筈) */
				{0x1f, (u16)(1024/8),  (8)},	/* 0x1f:普通に避けれる間隔 */
				{0x07, (u16)(1024/4),  (4)},
				{0x03, (u16)(1024/4),  (4)},	/* 0x03:連続弾っぽく */
			};
			if (0==(src->jyumyou & kougeki_tbl[src->kougeki_type][K00_KOUGEKI_KANKAKU_MASK]))/*0x0f*/
			{
				br.BULLET_REGIST_00_speed256				= (t256(2.5)+(((cg_game_difficulty))<<6));/* [2.5 - 3.25] */		//	(t256(2.0)+((difficulty)<<6)),/* [2.0 - 2.75] */
				br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN;
			//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_00_SIRO+(src->cx256&0x07);
				br.BULLET_REGIST_06_n_way					= kougeki_tbl[src->kougeki_type][K02_TAMA_KAZU];		//	(3/*+difficulty+difficulty*/)
				br.BULLET_REGIST_07_VECTOR_div_angle1024	= kougeki_tbl[src->kougeki_type][K01_SUKIMA_KAKUDO];	//	(int)(1024/24),
				bullet_regist_multi_vector_send1_xy_src(src); 	/* 弾源x256 y256 中心から発弾。 */
			}
		}
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
	h->kougeki_type 	= (((l->user_kougeki_type)&1)<<2)|((cg_game_difficulty)/*&0x03*/);
}
