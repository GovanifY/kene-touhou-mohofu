
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����̃U�R
	-------------------------------------------------------
		"�d��2",		"SPLASH",
	-------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_ao_yousei2(OBJ *src)
{
	if (50+10  < src->jyumyou)		/* �o��̓��� */
	{
		src->cx256 += src->vx256;/*fps_factor*/
		src->cy256 += src->vy256;/*fps_factor*/
	}
	else
	if (50	  < src->jyumyou)		/* �����҂� */
	{
		REG_02_DEST_X	= ((src->cx256));
		REG_03_DEST_Y	= ((src->cy256));
		calculate_jikinerai();
		src->tmp_angleCCW1024 = (deg65536to1024(HATSUDAN_03_angle65536));		/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
		mask1024(src->tmp_angleCCW1024);
	/* �A�j���[�V���� */
		zako_anime_type01(src);
	}
	else
	if (49	  < src->jyumyou)	/* �e������ */
	{
		zako_shot_supeka(src, ZAKO_SPEKA_1f_ao3_rendan);
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

static void regist_zako_020_ao_yousei2(GAME_COMMAND *l, OBJ *h)
{
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
	}
	#endif
	h->jyumyou					= (50+10+40);
	h->kaisu_nnn				= (3);	/* 3�񌂂� */
}
