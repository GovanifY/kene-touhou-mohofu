
/*---------------------------------------------------------
 東方模倣風  ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	大妖精の弾幕を生成します。
---------------------------------------------------------*/


/*---------------------------------------------------------
	紅2面中ボス 大妖精	通常攻撃1(1/3)
	-------------------------------------------------------
緑クナイ弾
下から始めて、時計回り、2段づつ
---------------------------------------------------------*/
static void danmaku_create_1a_dai_yousei_midori(SPRITE *src)
{
	static int aaa_tmp_angleCCW65536 = 0;
	static int bbb = 0;
	if (64==(src->boss_base_danmaku_time_out))
	{
		aaa_tmp_angleCCW65536 = 0;
		bbb = 0;
	}
	if ((64-(48))<((src->boss_base_danmaku_time_out)))
	{
	//	if (0x00==((src->boss_base_danmaku_time_out)&(2-1)))/* 2回に1回 */
		{
			int i;
			for (i=0; i<2; i++)
			{
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
				obj_send1->cx256 					= (src->cx256);				/* 弾源x256 */
				obj_send1->cy256 					= (src->cy256);				/* 弾源y256 */
			//
				br.BULLET_REGIST_speed256			= (t256(0.6)+(bbb<<2)+(i<<4));			/* 弾速 */
				br.BULLET_REGIST_angle65536 		= (aaa_tmp_angleCCW65536);
				br.BULLET_REGIST_div_angle65536 	= (int)(65536/(48));		/* 分割角度(1024[360/360度]を 48 分割) */	/* 1周をn分割した角度 */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_02_MIDORI; 	/* [ 弾] */
				br.BULLET_REGIST_n_way				= (1);/* [nway] */
				br.BULLET_REGIST_speed_offset		= t256(1);/*てすと*/
			//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
				bullet_regist_angle();
			}
			aaa_tmp_angleCCW65536 -= (65536/48);
			bbb++;
		}
	}
}

/*---------------------------------------------------------
	紅2面中ボス 大妖精	通常攻撃1(2/3)
	-------------------------------------------------------
赤クナイ弾
下から始めて、反時計回り、2段づつ
---------------------------------------------------------*/
static void danmaku_create_1b_dai_yousei_aka(SPRITE *src)
{
	static int aaa_tmp_angleCCW65536 = 0;
	static int bbb = 0;
	if (64==(src->boss_base_danmaku_time_out))
	{
		aaa_tmp_angleCCW65536 = 0;
		bbb = 0;
	}
	if ((64-(48))<((src->boss_base_danmaku_time_out)))
	{
	//	if (0x00==((src->boss_base_danmaku_time_out)&(2-1)))/* 2回に1回 */
		{
			int i;
			for (i=0; i<2; i++)
			{
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
				obj_send1->cx256 					= (src->cx256);				/* 弾源x256 */
				obj_send1->cy256 					= (src->cy256);				/* 弾源y256 */
			//
				br.BULLET_REGIST_speed256			= (t256(0.6)+(bbb<<2)+(i<<4));			/* 弾速 */
				br.BULLET_REGIST_angle65536 		= (aaa_tmp_angleCCW65536);
				br.BULLET_REGIST_div_angle65536 	= (int)(65536/(48));		/* 分割角度(1024[360/360度]を 48 分割) */	/* 1周をn分割した角度 */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;	/* [ 弾] */
				br.BULLET_REGIST_n_way				= (1);/* [nway] */
				br.BULLET_REGIST_speed_offset		= t256(1);/*てすと*/
			//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
				bullet_regist_angle();
			}
			aaa_tmp_angleCCW65536 += (65536/48);
			bbb++;
		}
	}
}


