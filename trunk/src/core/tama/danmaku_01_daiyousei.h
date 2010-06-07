
/*---------------------------------------------------------
 �����͕핗  �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	��d���̒e���𐶐����܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�g2�ʒ��{�X ��d��	�ʏ�U��1(1/3)
	-------------------------------------------------------
�΃N�i�C�e
������n�߂āA���v���A2�i�Â�
---------------------------------------------------------*/
static void danmaku_create_20_dai_yousei_midori(SPRITE *src)
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
	//	if (0x00==((src->boss_base_danmaku_time_out)&(2-1)))/* 2���1�� */
		{
			int i;
			for (i=0; i<2; i++)
			{
				obj_send1->x256 					= (src->x256);				/* �e��x256 */
				obj_send1->y256 					= (src->y256);				/* �e��y256 */
			//
				br.BULLET_REGIST_speed256			= (t256(0.6)+(bbb<<2)+(i<<4));			/* �e�� */
				br.BULLET_REGIST_angle65536 		= (aaa_tmp_angleCCW65536);
				br.BULLET_REGIST_div_angle65536 	= (int)(65536/(48));		/* �����p�x(1024[360/360�x]�� 48 ����) */	/* 1����n���������p�x */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_02_MIDORI; 	/* [ �e] */
				br.BULLET_REGIST_n_way				= (1);/* [nway] */
				br.BULLET_REGIST_speed_offset		= (1);/*�Ă���*/
			//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
				bullet_regist_angle();/* bullet_regist_vector(); ���኱�V���v���ȈׁA�����Ɍ݊����Ȃ��B */
			}
			aaa_tmp_angleCCW65536 -= (65536/48);
			bbb++;
		}
	}
}

/*---------------------------------------------------------
	�g2�ʒ��{�X ��d��	�ʏ�U��1(2/3)
	-------------------------------------------------------
�ԃN�i�C�e
������n�߂āA�����v���A2�i�Â�
---------------------------------------------------------*/
static void danmaku_create_21_dai_yousei_aka(SPRITE *src)
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
	//	if (0x00==((src->boss_base_danmaku_time_out)&(2-1)))/* 2���1�� */
		{
			int i;
			for (i=0; i<2; i++)
			{
				obj_send1->x256 					= (src->x256);				/* �e��x256 */
				obj_send1->y256 					= (src->y256);				/* �e��y256 */
			//
				br.BULLET_REGIST_speed256			= (t256(0.6)+(bbb<<2)+(i<<4));			/* �e�� */
				br.BULLET_REGIST_angle65536 		= (aaa_tmp_angleCCW65536);
				br.BULLET_REGIST_div_angle65536 	= (int)(65536/(48));		/* �����p�x(1024[360/360�x]�� 48 ����) */	/* 1����n���������p�x */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;	/* [ �e] */
				br.BULLET_REGIST_n_way				= (1);/* [nway] */
				br.BULLET_REGIST_speed_offset		= (1);/*�Ă���*/
			//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
				bullet_regist_angle();/* bullet_regist_vector(); ���኱�V���v���ȈׁA�����Ɍ݊����Ȃ��B */
			}
			aaa_tmp_angleCCW65536 += (65536/48);
			bbb++;
		}
	}
}


