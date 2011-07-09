
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"メイド2",		"ZATAK",
	-------------------------------------------------------
	rwingx_zatak (赤い妖精)
	左右から現れ、自機狙い弾を打った後、真下へ直進してくる。
---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_meido2(SPRITE *src)
{
	if (512 > src->jyumyou)/* 退場 */
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	}
	else
	if (512+512 > src->jyumyou)/* 真下に直進 */
	{
		src->speed256 += t256(0.2);/*加速*/ /*t256(0.3)*/
		if (src->speed256 > t256(2.5)) /*t256(3.0)*/
		{
			src->speed256 = t256(2.5); /* 加速最大値 */
			src->jyumyou	= (512-1);
		}
	}
	else
	if (512+512+512 > src->jyumyou)/* 弾撃つ */
	{
		/* CCWの場合 */
		src->tmp_angleCCW1024					= cv1024r(0);/* 真下に向きを固定 */
		//if (data_hari_rank256/*src->le vel*/)
		{
			//efine HARIDAN_SPEED ( /*speed256*/(t256(5)+((data_le vel/*src->le vel*/)<<8) ) )
			#define HARIDAN_SPEED ( /*speed256*/t256(4/*5*/) + (/*data_hari_rank256*/(1<<8)/*src->le vel*/) )
			br.BULLET_REGIST_00_speed256				= HARIDAN_SPEED;				/* 弾速 */
			br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN;
		//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_HARI32_00_AOI; 		/* [ 弾] */
			br.BULLET_REGIST_06_n_way					= (1);							/* [1way] */
		//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (0);							/* ダミー角度(未使用) */
			bullet_regist_multi_vector_send1_xy_src(src); 	/* 弾源x256 y256 中心から発弾。 */
		}
		src->jyumyou	= (512+512-1);/* 次へ */
	}
	else
	if (512+512+512+512 > src->jyumyou)/* ほぼ真下を向く */
	{
		/* CCWの場合 */
		if (src->tmp_angleCCW1024 < cv1024r(180))
		{
			src->tmp_angleCCW1024 -= cv1024r(3);/*fps_factor*/
			if (src->tmp_angleCCW1024 < cv1024r(0))
			{
				src->jyumyou	= (512+512+512-1);/* 次へ */
			}
		}
		else
		{
			src->tmp_angleCCW1024 += cv1024r(3);/*fps_factor*/
			if (src->tmp_angleCCW1024 > cv1024r(360))
			{
				src->jyumyou	= (512+512+512-1);/* 次へ */
			}
		}
	}
	else
	if (512+512+512+512+512 > src->jyumyou)/* 画面左右から登場 */
	{
		src->speed256 -= t256(0.02);/*fps_factor*/	/* 減速 */
		if (src->speed256 <= 0)/* 速度が無いなら */
		{
			src->speed256 = 0;/* 停止 */
			src->jyumyou	= (512+512+512+512-1);/* 次へ */
		}
	}
	/*以下rwingx.cと同じ*/
	src->cx256+=((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/	/* CCWの場合 */
	src->cy256+=((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
//
	/* アニメーション */
	zako_anime_type01(src);
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

static void regist_zako_016_meido2(GAME_COMMAND *l, SPRITE *h)
{
	h->jyumyou				= 512+512+512+512+512+(40);
	h->speed256 			= ((ra_nd()&((256*4)-1)));/*2.0*(((dou ble)ra_nd()/RAND_MAX)*2)*/
/* CCWの場合 */
	if (t256(GAME_WIDTH/2) > h->cx256 )
			{	h->tmp_angleCCW1024 	= cv1024r(180-90-10);	}/* 画面左側から出現。右下へ */
	else	{	h->tmp_angleCCW1024 	= cv1024r(180+90+10);	}/* 画面右側から出現。左下へ */
}
