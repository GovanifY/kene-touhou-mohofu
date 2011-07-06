
/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"囲妖怪1",		"CUBE",
	-------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_kakomi1(SPRITE *src)
{
	if ((JYUMYOU_ZAKO_HASSEI-295) > src->jyumyou)		/* 離脱 */
	{
		src->radius256 += t256(1.2);		/*fps_factor*/	/*2.0*/
		if (src->radius256 > t256(350.0))
		{
			src->jyumyou = JYUMYOU_NASI;	/* おしまい */
		}
	}
	else
	{
		src->radius256 -= t256(1.0);		/*fps_factor*/
		if (0==(ra_nd()&(512-1)))	/* 1/512 ← 1/500 の確率で弾打つ */ 			/*%500*/
		{
			obj_send1->cx256 					= src->cx256;
			obj_send1->cy256 					= src->cy256;
			br.BULLET_REGIST_speed256			= t256(1.0);				/* 弾速 */
		//	br.BULLET_REGIST_speed256			= speed256; 				/* 弾速 */
			br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* 自機狙い弾 */	/* 発射中心角度 / 特殊機能(自機狙い/他) */
		//	br.BULLET_REGIST_div_angle1024		= (0);						/* ダミー角度(未使用) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU8_04_MIZU_IRO;		/* [赤弾] */ /* 弾グラ */
			br.BULLET_REGIST_n_way				= (1);						/* [1way] */
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
			bullet_regist_vector();
		}
		if (0==(ra_nd()&(512-1)))	/* 1/512 ← 1/600 の確率確率でアイテム出す */	/*%600*/
		{
			item_create(src, SP_ITEM_00_P001, 1, ITEM_MOVE_FLAG_06_RAND_XY);
		}
		src->tmp_angleCCW1024 -= (2);		/*fps_factor*/	/*2*/	/* deg_360_to_512(3) 速すぎる */
		mask1024(src->tmp_angleCCW1024);
	}
	//
	zako_move_type11_radius(src);
	/* アニメーション */
	{
		src->type			= TEKI_54_CHOU1+((src->jyumyou&0x10)>>4);				/* アニメーション */
	//	zako_anime_type02(src, (TEKI_54_CHOU1));
	}
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_kakomi1(STAGE_DATA *l)
{
	SPRITE *h;
	h								= sprite_add_gu_error();
	if (NULL!=h)
	{
		add_zako_common(l, h);
		h->m_Hit256R				= ZAKO_ATARI16_PNG;
		h->type 					= TEKI_54_CHOU1;
		h->callback_mover			= move_kakomi1;
		h->callback_hit_enemy		= callback_hit_zako;
		//
		h->radius256				= t256(350);
		#if 1
		/* r30互換 */
		h->tmp_angleCCW1024 		= (l->user_x + l->user_x);/*(i<<5)*/	//	/*360*/(1024/16)*i;
		#else
		/* r31 */
		h->tmp_angleCCW1024 		= (l->user_x);/*(i<<5)*/	//	/*360*/(1024/16)*i;
		#endif
		h->target_x256			= t256(GAME_WIDTH/2);/* GAME_WIDTH 380 ~= 380 == 62+256+62 */
		h->target_y256			= t256(GAME_HEIGHT/2);
	}
}

/*
;[16分割]
  0,  32,  64,	96,
128, 160, 192, 224,
256, 288, 320, 352,
384, 416, 448, 480,
[0==512,]
;[13分割]
  0,   0, -0
 39,  40, -1
 78,  80, -2
118, 120, -2
157, 160, -3
196, 200, -4
236, 240, -4
275, 280, -5
314, 320, -6
354, 360, -6
393, 400, -7
432, 440, -8
472, 480, -8
*/
