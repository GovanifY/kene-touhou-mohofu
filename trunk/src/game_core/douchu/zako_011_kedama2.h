
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����̃U�R
	-------------------------------------------------------
		"�ы�2",	"BADGUY",	(r27����V�K�ǉ�)
	-------------------------------------------------------
	�{�Ɩы�
---------------------------------------------------------*/

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_kedama2(SPRITE *src)
{
	if (512 > src->jyumyou)
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
	else
	if (512+64 > src->jyumyou)	/* �U�� */
	{
		zako_shot_supeka(src, (src->kougeki_type));
	}
	else
	{
		/* �΂�΂�ɓo�ꂳ���� */
		if (
			(0 >= src->cx256)
			|| (t256(GAME_WIDTH) < src->cx256)
			#if (1==USE_X_HOUKOU)
			|| (0 >= src->cy256)
			|| (t256(GAME_HEIGHT) < src->cy256)
			#endif
		)
		{
			/*��ʊO*/;
		}
		else
		{
			src->jyumyou	= (512+64-1); /* �o�ꂷ�� */
		}
	}

	/* �ړ����� */
	src->cx256 += (src->vx256);
	src->cy256 += (src->vy256);
	/* �A�j���[�V���� */
	src->zako_anime_rotate_angle1024 = ( 10);
	zako_anime_type04(src); 	/* �O����] */
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

static void regist_zako_011_kedama2(GAME_COMMAND *l, SPRITE *h)
{
	h->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0xaa);		/* �����ۂ� */
	//
	h->cy256				= ((l->user_y)<<8);
	h->vx256				= ((l->user_x));
	h->vy256				= (0);
	//
	{
		if (0 < (h->vx256))
		{/* [���� user_y ���������̏ꍇ�A�E�ֈړ�(��)] [����y���������̏ꍇ�A���ֈړ�(��)] */
			h->cx256			= t256(-30)/*-(i<<(5+8))*/;
		}
		else
		{/* [���� user_y ���������̏ꍇ�A���ֈړ�(��)] [����y���������̏ꍇ�A��ֈړ�(��)] */
			h->cx256			= t256(360)/*+(i<<(5+8))*/;/* 360 > 352(x_max) > 272(y_max) */
		}
	}
	//
	if (0 < (h->cy256))
	{	;	}
	else
	{
		/* y���W���������̏ꍇ�́Ax���W�w��Ȃ̂�(x���W y���W ���t�ɂ���) */
		{
			int s_sss;
			s_sss				= h->cx256;
			h->cx256			= -(h->cy256);
			h->cy256			= s_sss;
		}
		h->vy256			= (h->vx256);
		h->vx256			= (0);
	}
//	h->jyumyou			= 512+64+512+64;	/* �K���ɑ傫�Ȓl */	/*+(i<<4)*/
	h->kougeki_type 	= ZAKO_SPEKA_00_hai_kedama1 + ((((l->user_kougeki_type)&1)<<2)|((cg.game_difficulty)/*&0x03*/));
}
