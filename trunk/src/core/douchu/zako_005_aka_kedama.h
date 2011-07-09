
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"赤毛玉1",		"MINE",
	-------------------------------------------------------
	ぐるぐるその場で動かないで、回転アニメしてて
	ほっとくと8つに分裂して
	最後に高速弾撃って来るやつ(橙)
	-------------------------------------------------------
	このキャラのみ分裂するんだけど、システムとして
	分裂はサポートしなくなるかもしれない(?)。
	その場合は始めから、8つ重なって出てくる。
	分裂は、つまりアリス弾だからサポートされるとしたら、
	最終的なアリス弾の仕組み(まだ確定していない)と同じ仕組みになると思う。
---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動(分裂後)
---------------------------------------------------------*/

static void move_aka_kedama1_2nd(SPRITE *src)
{
	br.BULLET_REGIST_00_speed256				= t256(2.5);					/* 弾速 */	/*高速弾*/	/*t256(3+teki_rank)+(0(teki_rank<<6))*/
//	br.BULLET_REGIST_00_speed256				= t256((cg_game_difficulty))+t256(1.5); 			/* 弾速 */
	br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN; 		/* 自機狙い弾 */	/* 発射中心角度 / 特殊機能(自機狙い/他) */
//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
	br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_MINI8_07_DAI_DAI;		/* [赤弾] */ /* 弾グラ */
	br.BULLET_REGIST_06_n_way					= (1);							/* [1way] */
//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (0);							/* ダミー角度(未使用) */
	zako_shot_ra_nd(src);
/* CCWの場合 */
	src->cx256 += (src->vx256);/*fps_factor*/
	src->cy256 += (src->vy256);/*fps_factor*/
	gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	/* アニメーション */
	src->zako_anime_rotate_angle1024 = ( 16);/*(20)*/
	zako_anime_type04(src); 	/* グラ回転 */
}


/*---------------------------------------------------------
	敵を追加する(変身)
---------------------------------------------------------*/

static void add_local_aka_kedama1_2nd(SPRITE *src/*, int lv*/)
{
	int i;
	for (i=0; i<8; i++)/*分裂*/
	{
		SPRITE *h;
		h						= obj_add_01_teki_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= src->type;			/* 分裂前をそのまま引き継ぐ */
			h->callback_mover		= move_aka_kedama1_2nd;
			h->callback_loser		= src->callback_loser;	/* 爆発させたほうがアイテムが稼げる */
			h->callback_hit_teki	= callback_hit_zako;	/* 「ザコに自弾があたった場合の処理」に、標準の処理を設定 */
			//
			h->flags				= src->flags;			/* 分裂前をそのまま引き継ぐ */
			h->cx256				= src->cx256;			/* 分裂前をそのまま引き継ぐ */
			h->cy256				= src->cy256;			/* 分裂前をそのまま引き継ぐ */
			h->base_score			= src->base_score;		/* 爆発させたほうがスコアが稼げる */
			h->base_hp				= src->base_hp; 		/* 分裂前の体力をそのまま引き継ぐ(減らしてあればその分軟らかい) */
			{
				const int data_zzz_angle1024				= (i<<7);	/* 分裂角度 */	//	(/*360*/1024/8)*i;
				const int data_zzz_speed					= (t256(1.5)+((cg_game_difficulty)<<6));	/* 分裂速度 */	/*(3.0)*/
			//
				h->vx256 = ((sin1024((data_zzz_angle1024))*(data_zzz_speed))>>8);/*fps_factor*/
				h->vy256 = ((cos1024((data_zzz_angle1024))*(data_zzz_speed))>>8);/*fps_factor*/
			}
		}
	}
}

//			/*data->base.*/h->base_score		= score(10*2)*(1+(2/*di fficulty*/));		/*(teki_rank)*/ 	/*9*/
//			/*data->base.*/h->base_hp			= (8*(1+ 9))+(1/*di fficulty*/<<(4+3)); 	/*(teki_rank)*/

/*---------------------------------------------------------
	敵移動(本体)
---------------------------------------------------------*/

static void move_aka_kedama1_1st(SPRITE *src)
{
	if (2 == src->jyumyou)	/* 分裂 */
	{
		add_local_aka_kedama1_2nd(src);/* 変身 分裂 */	/*,data->teki_rank*/
		src->jyumyou = JYUMYOU_NASI;		/* 本体はおしまい */
	}
	else
	if (31+2 > src->jyumyou)
	{
		src->zako_anime_rotate_angle1024 = (  0);	/* 止まる */
	}
	else
	{
										/* 移動 move */
		src->cx256 += (src->vx256); 	/*fps_factor*/
		src->cy256 += (src->vy256); 	/*fps_factor*/
		src->zako_anime_rotate_angle1024 = ( 10); 	/* グラ回転 */
	}
	/* アニメーション */
	zako_anime_type04(src); 	/* グラ回転 */
}


/*---------------------------------------------------------
	敵を追加する(本体)
---------------------------------------------------------*/

static void regist_zako_005_aka_kedama1(GAME_COMMAND *l, SPRITE *h)
{
//	/* 初期位置 */
	obj_send1->cx256		= ((ra_nd()&((256*256)-1)))+t256(64);/*320?*/ /*t256(ra_nd()%270)+t256(50)*/	/* 弾源x256 */
//	obj_send1->cy256		= ((ra_nd()&((256*256)-1)))+t256(16);/*350?*/ /*t256(ra_nd()%300)+t256(50)*/
	obj_send1->cy256		= ((ra_nd()&((256*128)-1)))+t256(16);/*350?*/ /*t256(ra_nd()%300)+t256(50)*/	/* 弾源y256 */
	tmp_angleCCW65536_src_nerai(obj_send1, h);
	h->vx256				= ((sin65536(h->tmp_angleCCW65536)*t256(1.5))>>8);/*fps_factor*/	/* CCWの場合 */
	h->vy256				= ((cos65536(h->tmp_angleCCW65536)*t256(1.5))>>8);/*fps_factor*/
//
	h->jyumyou				= (80)+byou60(1)+(2);
}
