
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"緑毛玉1",		"XEV",
	-------------------------------------------------------
	緑毛玉	単体で突っ込んでくる。
---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_midori_kedama1(SPRITE *src)
{
	int speed256;
	if ((JYUMYOU_ZAKO_HASSEI-320) > src->jyumyou)		/* 離脱 */
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
		speed256 = t256(3.5/*6*/);
	}
	else
	{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		if ((src->cx256 >= zzz_player->cx256)||
			(src->cy256 > t256(GAME_HEIGHT-48) ) )/*480-80*/
		{
			src->jyumyou	= (JYUMYOU_ZAKO_HASSEI-320);
			src->target_x256	= t256(-100);
		//	src->target_y256	= zzz_player->cy256-t256(48);
			src->target_y256	= zzz_player->cy256-t256(48);
			{
				br.BULLET_REGIST_00_speed256			= t256(2.75);				/* 弾速 */
				br.BULLET_REGIST_02_VECTOR_angle1024	= ANGLE_JIKI_NERAI_DAN; 	/* 自機狙い弾 */	/* 発射中心角度 / 特殊機能(自機狙い/他) */
			//	br.BULLET_REGIST_03_VECTOR_regist_type	= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
				br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_MINI8_01_AKA;		/* [赤弾] */ /* 弾グラ */
				br.BULLET_REGIST_06_n_way				= (1);						/* [1way] */
			//	br.BULLET_REGIST_07_VECTOR_div_angle1024		= (0);						/* ダミー角度(未使用) */
				bullet_regist_multi_vector_send1_xy_src(src); 	/* 弾源x256 y256 中心から発弾。 */
			}//t256(1.0)+((4.0/*teki_rank*/)<<8)
		}
		speed256 = t256(2.5);/*(3.0)*/
	}
//	send1_xy(src);	/* 弾源x256 y256 中心から発弾。 */
	obj_send1->cx256	= (src->target_x256);					/* 弾源x256 */
	obj_send1->cy256	= (src->target_y256);					/* 弾源y256 */
	tmp_angleCCW65536_src_nerai(obj_send1, src);
	src->cx256 += ((sin65536(src->tmp_angleCCW65536)*speed256)>>8);/*fps_factor*/ /* CCWの場合 */
	src->cy256 += ((cos65536(src->tmp_angleCCW65536)*speed256)>>8);/*fps_factor*/
	/* アニメーション */
	src->zako_anime_rotate_angle1024 = (  5);
	zako_anime_type04(src); 	/* グラ回転 */
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

static void regist_zako_008_midori_kedama1(GAME_COMMAND *l, SPRITE *h)
{
	{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		h->target_x256			= zzz_player->cx256;
	//	h->target_y256			= zzz_player->cy256;		/* 特攻禁止 */
		h->target_y256			= ((zzz_player->cy256)>>1);
	}
}
