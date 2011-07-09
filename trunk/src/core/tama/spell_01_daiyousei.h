
/*---------------------------------------------------------
 �����͕핗 �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	��d���̒e���𐶐����܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�g2�ʒ�-�{�X ��d�� �ʏ�U��1(1/3)
	-------------------------------------------------------
�΃N�i�C�e
������n�߂āA���v���A2�i�Â�
---------------------------------------------------------*/
static void spell_create_1a_dai_yousei_midori(SPRITE *src)
{
	static int aaa_tmp_angleCCW65536 = 0;
	static int bbb = 0;
	if (64==(src->boss_spell_timer))
	{
		aaa_tmp_angleCCW65536 = 0;
		bbb = 0;
	}
	if ((64-(48))<((src->boss_spell_timer)))
	{
	//	if (0x00==((src->boss_spell_timer)&(2-1)))/*(2���1��)*/
		{
				br.BULLET_REGIST_01_speed_offset			= t256(1);/*(�e�X�g)*/
				br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KUNAI12_05_MIDORI; 	/* [ �e] */
			u32 i;
			for (i=0; i<(1+cg_game_difficulty); i++)/*(2�i�Â�)*/
			{
				br.BULLET_REGIST_00_speed256				= (t256(0.6)+(bbb<<2)+(i<<6));			/* �e�� */
				br.BULLET_REGIST_02_angle65536				= (aaa_tmp_angleCCW65536);
				tama_system_regist_single();
			}
			aaa_tmp_angleCCW65536 -= (65536/48);
			bbb++;
		}
	}
}


/*---------------------------------------------------------
	�g2�ʒ�-�{�X ��d�� �ʏ�U��1(2/3)
	-------------------------------------------------------
�ԃN�i�C�e
������n�߂āA�����v���A2�i�Â�
����:Lunatic���ꉞ����48�����e�B(4��������o��2�i?3�i�ɐ���?)
---------------------------------------------------------*/
static void spell_create_1b_dai_yousei_aka(SPRITE *src)
{
	static int aaa_tmp_angleCCW65536 = 0;
	static int bbb = 0;
	if (64==(src->boss_spell_timer))
	{
		aaa_tmp_angleCCW65536 = 0;
		bbb = 0;
	}
	if ((64-(48))<((src->boss_spell_timer)))
	{
	//	if (0x00==((src->boss_spell_timer)&(2-1)))/*(2���1��)*/
		{
				br.BULLET_REGIST_01_speed_offset			= t256(1);/*(�e�X�g)*/
				br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KUNAI12_01_AKA;	/* [ �e] */
			u32 i;
			for (i=0; i<(1+cg_game_difficulty); i++)/*(2�i�Â�)*/
			{
			//
				br.BULLET_REGIST_00_speed256				= (t256(0.6)+(bbb<<2)+(i<<6));			/* �e�� */
				br.BULLET_REGIST_02_angle65536				= (aaa_tmp_angleCCW65536);
				tama_system_regist_single();
			}
			aaa_tmp_angleCCW65536 += (65536/48);
			bbb++;
		}
	}
}
