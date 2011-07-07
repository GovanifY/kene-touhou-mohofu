
/*---------------------------------------------------------
 東方模倣風 ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	大妖精の弾幕を生成します。
---------------------------------------------------------*/


/*---------------------------------------------------------
	紅2面中-ボス 大妖精 通常攻撃1(1/3)
	-------------------------------------------------------
緑クナイ弾
下から始めて、時計回り、2段づつ
---------------------------------------------------------*/
static void spell_create_1a_dai_yousei_midori(SPRITE *src)
{
	static int aaa_tmp_angleCCW65536 = 0;
	static int bbb = 0;
	if (64==(src->boss_base_spell_time_out))
	{
		aaa_tmp_angleCCW65536 = 0;
		bbb = 0;
	}
	if ((64-(48))<((src->boss_base_spell_time_out)))
	{
				obj_send1->cx256							= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
				obj_send1->cy256							= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
	//	if (0x00==((src->boss_base_spell_time_out)&(2-1)))/* 2回に1回 */
		{
				br.BULLET_REGIST_01_speed_offset			= t256(1);/*(テスト)*/
				br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KUNAI12_05_MIDORI; 	/* [ 弾] */
			//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)標準弾 */
			int i;
			for (i=0; i<2; i++)
			{
				br.BULLET_REGIST_00_speed256				= (t256(0.6)+(bbb<<2)+(i<<4));			/* 弾速 */
				br.BULLET_REGIST_02_angle65536				= (aaa_tmp_angleCCW65536);
				tama_system_regist_single();
			}
			aaa_tmp_angleCCW65536 -= (65536/48);
			bbb++;
		}
	}
}

/*---------------------------------------------------------
	紅2面中-ボス 大妖精 通常攻撃1(2/3)
	-------------------------------------------------------
赤クナイ弾
下から始めて、反時計回り、2段づつ
メモ:Lunaticも一応同じ48方向弾。(4ヶ所から出て2段?3段に整列?)
---------------------------------------------------------*/
static void spell_create_1b_dai_yousei_aka(SPRITE *src)
{
	static int aaa_tmp_angleCCW65536 = 0;
	static int bbb = 0;
	if (64==(src->boss_base_spell_time_out))
	{
		aaa_tmp_angleCCW65536 = 0;
		bbb = 0;
	}
	if ((64-(48))<((src->boss_base_spell_time_out)))
	{
				obj_send1->cx256							= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
				obj_send1->cy256							= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
	//	if (0x00==((src->boss_base_spell_time_out)&(2-1)))/* 2回に1回 */
		{
				br.BULLET_REGIST_01_speed_offset			= t256(1);/*(テスト)*/
				br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KUNAI12_01_AKA;	/* [ 弾] */
			//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)標準弾 */
			int i;
			for (i=0; i<2; i++)
			{
			//
				br.BULLET_REGIST_00_speed256				= (t256(0.6)+(bbb<<2)+(i<<4));			/* 弾速 */
				br.BULLET_REGIST_02_angle65536				= (aaa_tmp_angleCCW65536);
				tama_system_regist_single();
			}
			aaa_tmp_angleCCW65536 += (65536/48);
			bbb++;
		}
	}
}
