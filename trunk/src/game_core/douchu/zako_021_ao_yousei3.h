
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����̃U�R
	-------------------------------------------------------
		"�d��3",		"FAIRY",	(r27����V�K�ǉ�)
	-------------------------------------------------------
	user_x: 	���x256[speed256]
	user_y: 	���ʒu[dots]
---------------------------------------------------------*/

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_ao_yousei3(SPRITE *src)
{
	if (512 > src->jyumyou) 	/* ���E */
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
	else
	{
		/* �U�� */
		if (0==((src->jyumyou)&0x3f))
		{
			if ((cg.game_difficulty))
			{
				zako_shot_supeka(src, ZAKO_SPEKA_1d_karasu);
			}
		}
	}
	/* �ړ����� */
	src->cx256 += (src->vx256);/*fps_factor*/
	#if (1==USE_X_HOUKOU_ao3)
	src->cy256 += (src->vy256);/*fps_factor*/
	#endif

	/* �A�j���[�V���� */
	zako_anime_type02(src);
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

static void regist_zako_021_ao_yousei3(GAME_COMMAND *l, SPRITE *h)
{
	h->vx256				= ((h->cx256)>>8);
	#if (1==USE_X_HOUKOU_ao3)
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
	#if (1==USE_X_HOUKOU_ao3)
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
	//
	h->jyumyou				= (512+320);
}
