
/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"青妖精2",		"SPLASH",
	-------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_ao_yousei2(SPRITE *src)
{
	if (50+10  < src->jyumyou)		/* 登場の動き */
	{
		src->cx256 += src->vx256;/*fps_factor*/
		src->cy256 += src->vy256;/*fps_factor*/
	}
	else
	if (50	  < src->jyumyou)		/* 少し待つ */
	{
		tmp_angleCCW1024_jiki_nerai(src);
		mask1024(src->tmp_angleCCW1024);
		zako_anime_type01(src, TEKI_12_YOUSEI1_1);
	}
	else
	if (49	  < src->jyumyou)	/* 弾を撃つ */
	{
		{
			int k;
			for (k=0; k<6/*4*/; k++)
			{
				const static u16 bk1024_tbl[4] =
				{((int)(1024*1/8)), ((int)(1024*1/12)), ((int)(1024*1/16)), ((int)(1024*1/24))};
				obj_send1->cx256					= (src->cx256);
				obj_send1->cy256					= (src->cy256);
				br.BULLET_REGIST_speed256			= (t256(2.5)+(k<<6));					/* 弾速 */
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 				/* */
				br.BULLET_REGIST_div_angle1024		= (bk1024_tbl[(difficulty)]);			/* 分割角度 */
				br.BULLET_REGIST_bullet_obj_type	= (BULLET_MARU8_01_AKA+(/*0*/6-k/*とりあえず*/)); /* [ 弾] */
				br.BULLET_REGIST_n_way				= (7);									/* [7way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
		src->kaisu_nnn--;
		if (0 > src->kaisu_nnn)
		{
			;/* 退場準備 */
		}
		else
		{
			src->jyumyou = (50+10);/*戻る*/
		}
	}
	else	/* 退場の動き */
	{
		src->cx256 -= src->vx256;/*fps_factor*/
		src->cy256 -= src->vy256;/*fps_factor*/
	}

}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_ao_yousei2(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_12_YOUSEI1_1;
	//
		h->callback_mover		= move_ao_yousei2;
		h->cx256				= ((l->user_x)<<8);
		h->cy256				= ((l->user_y)<<8);
	//
		/* 登場の動き / 退場の動き */
			 if (t256(64)			 > h->cx256)	{	h->vx256 =	t256(2.0);	}	/* 出現位置が左なら、右へ移動 */
		else if (t256(GAME_WIDTH-64) < h->cx256)	{	;	}						/* 中心付近なら左右に移動しない */
		else										{	h->vx256 = -t256(2.0);	}	/* 出現位置が右なら、左へ移動 */

		h->vy256 = (0);

		#if 1//(1==USE_X_HOUKOU)
		if (0 < (h->cy256))
		{	;	}
		else
		{
			/* y座標が負方向の場合は、x座標指定なので(x座標 y座標 を逆にする) */
			{
				int s_sss;
				s_sss				=  (h->cx256);
				h->cx256			= -(h->cy256);
				h->cy256			= s_sss;
			}
			h->vy256			= (h->vx256);
			h->vx256			= (0);
	//		h->AO_YOUSEI3_anime_houkou		= ((0x20)>>2);
		}
		#endif
		h->jyumyou					= (50+10+40);
		h->kaisu_nnn				= 3;	/* 3回撃つ */
	}
}
