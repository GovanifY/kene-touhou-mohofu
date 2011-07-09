
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"毛玉1",	"BADGUY",
	-------------------------------------------------------
	本家毛玉
---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void my_nerai(SPRITE *src)
{
	obj_send1->cx256	= (src->target_x256);						/* 弾源x256 */
	obj_send1->cy256	= (src->target_y256-t256(64)/*+(20)*/); 	/* 弾源y256 */
	tmp_angleCCW65536_src_nerai(obj_send1, src);
	src->vx256			= ((sin65536(src->tmp_angleCCW65536)));
	src->vy256			= ((cos65536(src->tmp_angleCCW65536)));
}

static void move_kedama1(SPRITE *src)
{
	if (512 > src->jyumyou)/* 退場 */
	{
		my_nerai(src);
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	}
	else
	if (512+64 > src->jyumyou)/* */
	{
		if (0==(src->jyumyou & 0x0f))
		{
			if (ENEMY_LAST_SHOT_LINE256 > src->cy256)	/* このラインより下からは敵が撃たない */
			{
				br.BULLET_REGIST_00_speed256				= t256(1.0);	//t256(1.0)+t256((teki_rank)>>1),
				br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN;
			//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_00_SIRO+(src->cx256&0x07);
				br.BULLET_REGIST_06_n_way					= (3+(cg_game_difficulty)+(cg_game_difficulty));
				br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/24);
				bullet_regist_multi_vector_send1_xy_src(src); 	/* 弾源x256 y256 中心から発弾。 */
			}
		}
	}
	else
//	if (512+64+64 > src->jyumyou)	/* 登場する */
	{
		my_nerai(src);
		{
			SPRITE *zzz_player;
			zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
			if (//(src->cx256 >= zzz_player->cx256) ||
				(src->cy256 > t256(GAME_HEIGHT/2) ) /*(480-80)*/ /*(272-32)*/
			 )
			{
				src->vy256 = (-src->vy256);
				src->jyumyou	= (512+64-1);
				/* 第二目標は、 */
				src->target_x256		= t256(((480-128)/2));	//= t256(-100);
				src->target_y256		= t256(0);/*((zzz_player->cy256)&(128-1))*/
				src->speed256			= t256(2.5/*5.0*/);/*fps_factor*/	/*6*/
			}
		}
	}
	/* 移動する */
	src->cx256 += (((src->vx256)*src->speed256)>>8);
	src->cy256 += (((src->vy256)*src->speed256)>>8);
	/* アニメーション */
	src->zako_anime_rotate_angle1024 = ( 10);
	zako_anime_type04(src); 	/* グラ回転 */
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

static void regist_zako_010_kedama1(GAME_COMMAND *l, SPRITE *h)
{
	h->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0xaa);		/*白っぽく */
//
	h->jyumyou			= (512+64+64);
	{
		/* 第一目標はプレイヤーのいた位置 */
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		h->target_x256				= (zzz_player->cx256);
		h->target_y256				= (zzz_player->cy256);
	}
	h->vx256				= (0);
	h->vy256				= (0);
//	h->speed256 			= ((ra_nd()&((256*2)-1)))*(1+difficulty+teki_rank/3);/*fps_factor*/
	h->speed256 			= ((ra_nd()&((256*2)-1))) + (((cg_game_difficulty)<<8)+ (/*teki_rank*/5<<7/**86*2*/) );/*fps_factor*/
}
