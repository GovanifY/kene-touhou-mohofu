
/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"陰陽玉1",		"PLASMABALL",
	-------------------------------------------------------
	(旧)白毛玉、(601氏)青妖精
	(4*6)で24匹も出てくるがあまり意味のないキャラ
	-------------------------------------------------------
	花映塚風(?)に変えた。
	かなりテキトー。
---------------------------------------------------------*/

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_inyou1(SPRITE *src)
{
//	if ( 0==(ra_nd()&(16-1)) ) /*確率上げた。[1/16]←[1/20]*/ /*%20*/
	if ( 0==(ra_nd()&( 8-1)) ) /*確率上げた。[1/8]←[1/20]*/ /*%20*/
	{
		obj_send1->cx256 = src->cx256;
		obj_send1->cy256 = src->cy256;
		br.BULLET_REGIST_speed256			= t256(5.0);				/* 弾速 */
		br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* */
		br.BULLET_REGIST_div_angle1024		= (int)(1024/(24)); 		/* 分割角度 */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_HARI32_01_AKA; 	/* [赤針弾] */
		br.BULLET_REGIST_n_way				= (5);						/* [5way] */
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_vector();
	}
	else
	{
		item_create(src, (SP_ITEM_00_P001+(ra_nd()&1)), 1, ITEM_MOVE_FLAG_06_RAND_XY);
	}
}
/*50%(SP_ITEM_00_P001 or SP_ITEM_01_P008)*/
//66%==SP_ITEM_04_BOMB or 33%==SP_ITEM_01_P008	 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_inyou1(SPRITE *src)
{
	src->cx256 += src->vx256;
	/* 画面両端では、跳ね返る */
	if (
		((t256(0)) > (src->cx256) ) ||
		((t256(GAME_WIDTH)) < (src->cx256) ) )
	{
		src->vx256 = -src->vx256;
	}
	/* 移動向きに合わせてグラ回転 */
	if (0 < src->vx256)
	{
		src->rotationCCW1024 += (24);	/*fps_factor*/
	}
	else
	{
		src->rotationCCW1024 -= (24);	/*fps_factor*/
	}
	/* 落ちる */
	if ((t256(2.50)) > src->vy256)	/* 最大重力加速度 */
	{
		src->vy256 += (8);	/* 重力加速度 */
	}
	/* */
	src->cy256 += src->vy256;
	if ( (src->limit_y256) < (src->cy256) )
	{
	//	src->cy256			= src->limit_y256;
		src->limit_y256 	= t256(300);
		src->vy256			= -src->vy256;	/* 跳ねる */
	}
	/* 画面下まで来たら、おしまい */
	if (t256(272) < (src->cy256) )
	{
		src->jyumyou = JYUMYOU_NASI;	/* おしまい */
	}
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_inyou1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_53_INYOU_DAI;
		h->callback_mover		= move_inyou1;
		h->callback_loser		= lose_inyou1;
		h->limit_y256			= (t256(128)-((/*yyy*/(l->user_255_code)&0x0f)<<(5+8)));		/* 閾値 */
								/*t256(200)-(j*t256(40))*/
								/*(-(xxx*t256(20))-(yyy*t256(50)))*/
		h->vx256				= (ra_nd()&0x1ff);
		h->vy256				= (ra_nd()&0x1ff);
	}
}
