
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"�d��2",		"SPLASH",
	-------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_ao_yousei2(SPRITE *src)
{
	if (50+10  < src->jyumyou)		/* �o��̓��� */
	{
		src->cx256 += src->vx256;/*fps_factor*/
		src->cy256 += src->vy256;/*fps_factor*/
	}
	else
	if (50	  < src->jyumyou)		/* �����҂� */
	{
		tmp_angleCCW1024_jiki_nerai(src);
		mask1024(src->tmp_angleCCW1024);
		zako_anime_type01(src, TEKI_12_YOUSEI1_1);
	}
	else
	if (49	  < src->jyumyou)	/* �e������ */
	{
		{
			int k;
			for (k=0; k<6/*4*/; k++)
			{
				const static u16 bk1024_tbl[4] =
				{((int)(1024*1/8)), ((int)(1024*1/12)), ((int)(1024*1/16)), ((int)(1024*1/24))};
				obj_send1->cx256					= (src->cx256);
				obj_send1->cy256					= (src->cy256);
				br.BULLET_REGIST_speed256			= (t256(2.5)+(k<<6));					/* �e�� */
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 				/* */
				br.BULLET_REGIST_div_angle1024		= (bk1024_tbl[(difficulty)]);			/* �����p�x */
				br.BULLET_REGIST_bullet_obj_type	= (BULLET_MARU8_01_AKA+(/*0*/6-k/*�Ƃ肠����*/)); /* [ �e] */
				br.BULLET_REGIST_n_way				= (7);									/* [7way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
		src->kaisu_nnn--;
		if (0 > src->kaisu_nnn)
		{
			;/* �ޏꏀ�� */
		}
		else
		{
			src->jyumyou = (50+10);/*�߂�*/
		}
	}
	else	/* �ޏ�̓��� */
	{
		src->cx256 -= src->vx256;/*fps_factor*/
		src->cy256 -= src->vy256;/*fps_factor*/
	}

}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_zako_ao_yousei2(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_12_YOUSEI1_1;
	//
		h->callback_mover		= move_ao_yousei2;
		h->cx256				= ((l->user_x)<<8);
		h->cy256				= ((l->user_y)<<8);
	//
		/* �o��̓��� / �ޏ�̓��� */
			 if (t256(64)			 > h->cx256)	{	h->vx256 =	t256(2.0);	}	/* �o���ʒu�����Ȃ�A�E�ֈړ� */
		else if (t256(GAME_WIDTH-64) < h->cx256)	{	;	}						/* ���S�t�߂Ȃ獶�E�Ɉړ����Ȃ� */
		else										{	h->vx256 = -t256(2.0);	}	/* �o���ʒu���E�Ȃ�A���ֈړ� */

		h->vy256 = (0);

		#if 1//(1==USE_X_HOUKOU)
		if (0 < (h->cy256))
		{	;	}
		else
		{
			/* y���W���������̏ꍇ�́Ax���W�w��Ȃ̂�(x���W y���W ���t�ɂ���) */
			{
				int s_sss;
				s_sss				=  (h->cx256);
				h->cx256			= -(h->cy256);
				h->cy256			= s_sss;
			}
			h->vy256			= (h->vx256);
			h->vx256			= (0);
	//		h->AO_YOUSEI3_anime_houkou		= ((0x20)>>2);
		}
		#endif
		h->jyumyou					= (50+10+40);
		h->kaisu_nnn				= 3;	/* 3�񌂂� */
	}
}
