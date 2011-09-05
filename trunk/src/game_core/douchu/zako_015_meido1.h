
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����̃U�R
	-------------------------------------------------------
	"���C�h1",		"RWINGX",	rwingx
	-------------------------------------------------------
	�ォ�牡�ɓ��Ԋu�Ő��񂵂Č���A�v���C���[�Ɍ�����
	������x�߂Â��ƁA�e�u���Ĕ��]���ē�����B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_meido1(OBJ *src)
{
	if (512+0 > src->jyumyou)
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
	else
	if (512+1024 > src->jyumyou)
	{
		src->speed256	+= t256(0.1);/*�������Ȃ��� �v���C���[�ɓ˂�����ł���*/
		if (src->cy256 >= t256(GAME_HEIGHT-150))
		{
			zako_shot_supeka(src, ZAKO_SPEKA_0a_midori1);
			src->jyumyou = (512+1023);
			src->tmp_angleCCW1024 += (1024/2);	/* (1024/2) == [180/360 �x] ���]���ē����� */
			mask1024(src->tmp_angleCCW1024);
		//	src->speed256 = /*t256(6+teki_rank)*/;/*���߂�*/
		}
	}
	else
	if (512+2048 > src->jyumyou)
	{
		/* �v���C���[�ɓ˂�����ł��� / schnell richtung player */
		src->jyumyou = (512+2047);
		REG_02_DEST_X	= ((src->cx256));
		REG_03_DEST_Y	= ((src->cy256));
		calculate_jikinerai();
		src->tmp_angleCCW1024 = (deg65536to1024(HATSUDAN_03_angle65536));		/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
	//	src->speed256	= /*t256(3+(teki_rank>>1))*/ /*(4+teki_rank)*/;/* ���߂� */
	}
	else
	if (512+2049 > src->jyumyou)/* 2048+1 */
	{
		if (t256(0.0) < src->speed256)
		{
			src->speed256 -= (5);/*t256(0.1)*/ /*����*/
		}
	}
	else
	if (512+2109 > src->jyumyou)/* 2048+1+60 */
	{
		if (t256(1.0) > src->speed256)
		{
			src->speed256 += (5);/*t256(0.1)*/ /*����*/
		}
	}
//
	/*�ȉ�rwingx.c�Ɠ���*/
	zako_move_vector(src);
	/* �A�j���[�V���� */
	zako_anime_type01(src);
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

static void regist_zako_015_meido1(GAME_COMMAND *l, OBJ *h)
{
	h->tmp_angleCCW1024 	= (0);/* ������ */		/* CCW�̏ꍇ */
//
	h->speed256 			= t256(0);//t256(0.5);
	h->jyumyou				= (512+2209);//(100);//20;
}
