
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	道中のザコ
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

static void move_aka_kedama1_2nd(OBJ *src)
{
	if (ENEMY_LAST_SHOT_LINE256 > src->cy256)	/* このラインより下からは敵が撃たない */
	{
		/*
		0  0/100   0/65536
		1  1/90  728/65536
		2  1/80  819/65536
		3  1/70  936/65536
		*/
		static const u16 kakuritu_tbl[4] =
		{ 0, 728, 819, 936 };
		if (kakuritu_tbl[((cg.game_difficulty))/*(teki_rank)*/] > (ra_nd()&(65536-1)))
		{
			zako_shot_supeka(src, ZAKO_SPEKA_0c_aka_kedama1);
		}
	}
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

static void add_local_aka_kedama1_2nd(OBJ *src/*, int lv*/)
{
	unsigned int i;
	for (i=0; i<8; i++)/*分裂*/
	{
		OBJ *h;
		h						= obj_add_A01_teki_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->cx256				= src->cx256;			/* 分裂前をそのまま引き継ぐ */
			h->cy256				= src->cy256;			/* 分裂前をそのまま引き継ぐ */
			h->obj_type_set 		= src->obj_type_set;	/* 分裂前をそのまま引き継ぐ */
			h->base_score			= src->base_score;		/* 爆発させたほうがスコアが稼げる */	/* 分裂前をそのまま引き継ぐ */
			h->base_hp				= src->base_hp; 		/* 分裂前の体力をそのまま引き継ぐ(減らしてあればその分軟らかい) */
			//
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->callback_mover		= move_aka_kedama1_2nd;
			h->callback_loser		= src->callback_loser;	/* 分裂前をそのまま引き継ぐ */
			h->callback_hit_teki	= callback_hit_zako;	/* 「ザコに自弾があたった場合の処理」に、標準の処理を設定 */
			//
			{
				const int data_zzz_angle1024				= (i<<7);	/* 分裂角度 */	//	(/*360*/1024/8)*i;
			//	const int data_zzz_speed256 				= (t256(1.5)+((cg_game_di fficulty)<<6));	/* 分裂速度 */	/*(3.0)*/
				const int data_zzz_speed256 				= (t256(1.5)+((3)<<6)); 	/* 分裂速度 */	/*(3.0)*/
				//------------------
				HATSUDAN_01_speed256	= (data_zzz_speed256);
				HATSUDAN_03_angle65536	= deg1024to65536(data_zzz_angle1024);
				sincos256();/*(破壊レジスタ多いので注意)*/
				h->vx256 = REG_03_DEST_Y;//sin_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
				h->vy256 = REG_02_DEST_X;//cos_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
				//------------------
			}
		}
	}
}

//			/*data->base.*/h->base_score		= score(10*2)*(1+(2/*di fficulty*/));		/*(teki_rank)*/ 	/*9*/
//			/*data->base.*/h->base_hp			= (8*(1+ 9))+(1/*di fficulty*/<<(4+3)); 	/*(teki_rank)*/

/*---------------------------------------------------------
	敵移動(本体)
---------------------------------------------------------*/

static void move_aka_kedama1_1st(OBJ *src)
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
		src->zako_anime_rotate_angle1024 = ( 10);	/* グラ回転 */
	}
	/* アニメーション */
	zako_anime_type04(src); 	/* グラ回転 */
}


/*---------------------------------------------------------
	敵を追加する(本体)
---------------------------------------------------------*/

static void regist_zako_005_aka_kedama1(GAME_COMMAND *l, OBJ *h)
{
//	/* 初期位置 */
	REG_00_SRC_X	= ((ra_nd()&((256*256)-1)))+t256(64);/*320?*/ /*t256(ra_nd()%270)+t256(50)*/	/* 弾源x256 */
	REG_01_SRC_Y	= ((ra_nd()&((256*128)-1)))+t256(16);/*350?*/ /*t256(ra_nd()%300)+t256(50)*/	/* 弾源y256 (256*256) */
	set_REG_DEST_XY(h);
	tmp_angleCCW65536_src_nerai();
	//------------------
	HATSUDAN_01_speed256	= (t256(1.5));
//	HATSUDAN_03_angle65536	= (HATSUDAN_03_angle65536);
	sincos256();/*(破壊レジスタ多いので注意)*/
	h->vx256 = REG_03_DEST_Y;//sin_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
	h->vy256 = REG_02_DEST_X;//cos_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
	//------------------
//
	h->jyumyou				= (80)+byou60(1)+(2);
}
