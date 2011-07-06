
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
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
		//	src->cy256 += (t256(3.00)+t256(data->enemy_rank));			/*fps_factor*/ /*�͂₷��*/
			src->cy256 += (t256(1.5));		/*fps_factor*/
			if (src->cy256 > ((difficulty<<5) + t256(64)) )			/* ����  t256(160) t256(200) */
			{
				src->wait1 = 1;
				src->kaisu_nnn -= 1;
			}
		}
		else
		{
		//	src->cy256 -= (t256(2.00)+t256(data->enemy_rank));			/*fps_factor*/ /*�͂₷��*/
			src->cy256 -= (t256(0.75));		/*fps_factor*/
			if (src->cy256 < t256(32))						/* ���  t256(50) */
			{
				src->wait1 = 0;
			}
		}
	//	if (/*0 <*/ /*data->*/enemy_rank)	/* easy�͑����񂾂���ꍇ�������Ȃ� */
		{
			/*
			0  0/100   0/65536
			1  1/90  728/65536
			2  1/80  819/65536
			3  1/70  936/65536
			*/
		//	static const u16 kakuritu_tbl[4] =
		//	{ 0, 728, 819, 936 };
		//	if (0==(ra_nd()%(100-data->enemy_rank*10)))
		//	if (kakuritu_tbl[3/*data->enemy_rank*/] > (ra_nd()&(65536-1)))
			if (936 > (ra_nd()&(65536-1)))
			{
				obj_send1->cx256 					= src->cx256;
				obj_send1->cy256 					= src->cy256;
				br.BULLET_REGIST_speed256			= t256(2.5)+(2/*data->enemy_rank*/<<6); 				/* �e�� */	/*�����e*/
			//	br.BULLET_REGIST_speed256			= speed256; 				/* �e�� */
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* ���@�_���e */	/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			//	br.BULLET_REGIST_div_angle1024		= (0);						/* �_�~�[�p�x(���g�p) */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU8_01_AKA;		/* [�Ԓe] */ /* �e�O�� */
				br.BULLET_REGIST_n_way				= (1);						/* [1way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
	}
	else
	{
	//	src->cy256 -= t256(4);/*�͂₷��*/
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

global void add_zako_midori_kedama2(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_60_AKA_KEDAMA;
		h->callback_mover		= move_midori_kedama2;
		h->wait1				= 0;
		h->kaisu_nnn			= (5);/*2*/
	}
}
