
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"紫編隊2",		"GREETER",
	-------------------------------------------------------
	５機一列に続がり、編隊飛行してくるザコ敵
	-------------------------------------------------------
	ming_greeter
	rwingx_curver(ming)に似てる
---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_yukari2(SPRITE *src)
{
	if (byou60(10) > src->jyumyou)
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	}
	else
	{
		if (src->cy256 >= src->max_y256)
		{
			src->jyumyou = byou60(10);
			if (t256(1.5/*2.0*/) < src->speed256)
			{	/* 減速 */
				src->speed256 -= t256(0.2);
			//	src->speed256 = t256(2)/*3+difficulty*/;						// [***090201		変更
			}
		//
			obj_send1->cx256	= (t256(GAME_WIDTH/2)); 		/* 弾源x256 */
			obj_send1->cy256	= (t256(0));					/* 弾源y256 */
			tmp_angleCCW1024_src_nerai(obj_send1, src);
			src->vx256=((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);		/* CCWの場合 */
			src->vy256=((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);
		}
		if (ENEMY_LAST_SHOT_LINE256 > src->cy256)	/* このラインより下からは敵が撃たない */
		{
			//if (2 teki_rank)
			{
			//	if (0==(ra_nd()%(105-(src->teki_rank-2+difficulty)*10)))		// [***090126		若干変更
			//	if (0==(ra_nd()%(11-(src->teki_rank-2+difficulty) )))			// [***090126		若干変更
			//	if (0==(ra_nd()%(16-(src->teki_rank-2+difficulty) )))			// [***090126		若干変更
			//	if ((src->teki_rank-2+difficulty) >= (ra_nd()&(16-1)))			// [***090126		若干変更
				if (((cg_game_difficulty)) >= (ra_nd()&(64-1))) 	// [***090126		若干変更
				{
					obj_send1->cx256						= src->cx256;
					obj_send1->cy256						= src->cy256;
					br.BULLET_REGIST_00_speed256					= t256((cg_game_difficulty))+t256(1.5); 			/* 弾速 */
				//	br.BULLET_REGIST_00_speed256					= speed256; 				/* 弾速 */
					br.BULLET_REGIST_02_VECTOR_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* 自機狙い弾 */	/* 発射中心角度 / 特殊機能(自機狙い/他) */
					br.BULLET_REGIST_04_bullet_obj_type 			= BULLET_MINI8_07_DAI_DAI;		/* [ 弾] */ /* 弾グラ */
					br.BULLET_REGIST_05_regist_type 				= REGIST_TYPE_00_MULTI_VECTOR;
					br.BULLET_REGIST_06_n_way						= (1);						/* [1way] */
				//	br.BULLET_REGIST_07_VECTOR_div_angle1024		= (0);						/* ダミー角度(未使用) */
					bullet_regist_vector();
				}
			}
		}
	}
	/* 似てるがちょっと違う--以下rwingx.cと同じ */
	src->cx256 += (src->vx256);/* fps_factor */
	src->cy256 += (src->vy256);/* fps_factor */
//
	src->rotationCCW1024 += (10);/* グラ回転 */
	mask1024(src->rotationCCW1024);
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

static void regist_zako_007_yukari2(GAME_COMMAND *l, SPRITE *h)
{
	h->max_y256 			= (t256(GAME_HEIGHT)-t256(60));
	tmp_angleCCW1024_jiki_nerai(h);
	h->speed256 			= (t256(2.5/*3.0*/)+((3/*di fficulty*/)<<4) ); /*4*/ /* 始めだけは速い */
	/* CCWの場合 */
	h->vx256=((sin1024((h->tmp_angleCCW1024))*h->speed256)>>8);
	h->vy256=((cos1024((h->tmp_angleCCW1024))*h->speed256)>>8);
//
//	h->jyumyou				= byou60(60);/* テキトー */
}
