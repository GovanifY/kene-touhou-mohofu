
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"�Ζы�2",		"CRUSHER",
	-------------------------------------------------------
	�Ζы�	�����ɕ���ŁA�ォ��˂�����ł���B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_midori_kedama2(SPRITE *src)
{
	if (0 < src->kaisu_nnn)
	{
		if (0 == src->wait1)
		{
		//	src->cy256 += (t256(3.00)+t256(teki_rank)); 		/*fps_factor*/ /*�͂₷��*/
			src->cy256 += (t256(1.5));		/*fps_factor*/
			if (src->cy256 > (((cg_game_difficulty)<<5) + t256(64)) )		/* ����  t256(160) t256(200) */
			{
				src->wait1		= 1;
				src->kaisu_nnn -= 1;
			}
		}
		else
		{
		//	src->cy256 -= (t256(2.00)+t256(teki_rank)); 		/*fps_factor*/ /*�͂₷��*/
			src->cy256 -= (t256(0.75)); 	/*fps_factor*/
			if (src->cy256 < t256(32))						/* ���  t256(50) */
			{
				src->wait1 = 0;
			}
		}
	//	if (/*0 <*/ teki_rank)	/* easy�͑����񂾂���ꍇ�������Ȃ� */
		{
			/*
			0  0/100   0/65536
			1  1/90  728/65536
			2  1/80  819/65536
			3  1/70  936/65536
			*/
		//	static const u16 kakuritu_tbl[4] =
		//	{ 0, 728, 819, 936 };
		//	if (0==(ra_nd()%(100-teki_rank*10)))
		//	if (kakuritu_tbl[3/*teki_rank*/] > (ra_nd()&(65536-1)))
			if (936 > (ra_nd()&(65536-1)))
			{
				obj_send1->cx256						= (src->cx256);
				obj_send1->cy256						= (src->cy256);
				br.BULLET_REGIST_00_speed256			= t256(2.5)+(2/*teki_rank*/<<6);				/* �e�� */	/*�����e*/
			//	br.BULLET_REGIST_00_speed256			= speed256; 				/* �e�� */
				br.BULLET_REGIST_02_VECTOR_angle1024	= ANGLE_JIKI_NERAI_DAN; 	/* ���@�_���e */	/* ���˒��S�p�x / ����@�\(���@�_��/��) */
				br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_MINI8_01_AKA;		/* [�Ԓe] */ /* �e�O�� */
				br.BULLET_REGIST_05_regist_type 		= REGIST_TYPE_00_MULTI_VECTOR;
				br.BULLET_REGIST_06_n_way				= (1);						/* [1way] */
			//	br.BULLET_REGIST_07_VECTOR_div_angle1024		= (0);						/* �_�~�[�p�x(���g�p) */
				bullet_regist_vector();
			}
		}
	}
	else
	{
	//	src->cy256 -= t256(4);/* �͂₷�� */
		src->cy256 -= t256(1.5);
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
//
	src->rotationCCW1024 += (10);
	mask1024(src->rotationCCW1024);
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

static void regist_zako_009_midori_kedama2(GAME_COMMAND *l, SPRITE *h)
{
	h->wait1				= 0;
	h->kaisu_nnn			= (5);/*2*/
}
