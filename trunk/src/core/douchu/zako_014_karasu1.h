
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"�G1",		"", (r27����V�K�ǉ�)
	-------------------------------------------------------
	user_x: 	���x256[speed256]
	user_y: 	���ʒu[dots]
---------------------------------------------------------*/


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_karasu1(SPRITE *src)
{
	if ((JYUMYOU_ZAKO_HASSEI-320) > src->jyumyou)		/* ���E */
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
	else
	{
		/* �U�� */
		if (0==((src->jyumyou)&0x3f))
		{
			if ((cg_game_difficulty))
			{
				obj_send1->cx256							= (src->cx256);
				obj_send1->cy256							= (src->cy256);
				br.BULLET_REGIST_00_speed256				= (t256(2.0)+(((cg_game_difficulty))<<6));
				br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KUNAI12_00_SIRO+((ra_nd())&7);
				br.BULLET_REGIST_05_regist_type 			= REGIST_TYPE_00_MULTI_VECTOR;
				br.BULLET_REGIST_06_n_way					= (8);
				br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/(8));
				bullet_regist_vector();
			}
		}
	}
	/* �ړ����� */
	src->cx256 += (src->vx256);/*fps_factor*/
	#if (1==USE_X_HOUKOU_karasu)
	src->cy256 += (src->vy256);/*fps_factor*/
	#endif
	/* �A�j�� */
	{
	//	src->type				= (TEKI_12_YOUSEI1_1)+(src->KARASU3_houkou) | ((src->jyumyou>>2)&(4-1));
		zako_anime_type02(src, (TEKI_12_YOUSEI1_1)+(src->KARASU3_houkou));
	}
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

static void regist_zako_014_karasu1(GAME_COMMAND *l, SPRITE *h)
{
	h->cy256				= ((l->user_y)<<8);
	h->vx256				= ((l->user_x));
	#if (1==USE_X_HOUKOU_karasu)
	h->vy256				= (0);
	#endif
	//
	{
		if (0 < (h->vx256))
		{/* [���� user_y ���������̏ꍇ�A�E�ֈړ�(��)] [����y���������̏ꍇ�A���ֈړ�(��)] */
			h->cx256				= t256(-30);
		//	h->KARASU3_houkou		= ((0x00)>>2);
			h->KARASU3_houkou		= ((0x30)>>2);/*�E�����J���X*/
		}
		else
		{/* [���� user_y ���������̏ꍇ�A���ֈړ�(��)] [����y���������̏ꍇ�A��ֈړ�(��)] */
			h->cx256				= t256(360);/* 360 > 352(x_max) > 272(y_max) */
			h->KARASU3_houkou		= ((0x50)>>2);/*�������J���X*/
		}
	}
	//
	#if (1==USE_X_HOUKOU_karasu)
	if (0 < (h->cy256))
	{	;	}
	else
	{
		/* y���W���������̏ꍇ�́Ax���W�w��Ȃ̂�(x���W y���W ���t�ɂ���) */
		{
			int h_hhh;
			h_hhh			= h->cx256;
			h->cx256		= -(h->cy256);
			h->cy256		= h_hhh;
		}
		h->vy256			= (h->vx256);
		h->vx256			= (0);
		h->KARASU3_houkou	= ((0x20)>>2);
	}
	#endif
}
