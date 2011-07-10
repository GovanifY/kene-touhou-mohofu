
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����̃U�R
	-------------------------------------------------------
	"���C�h2",		"ZATAK",
	-------------------------------------------------------
	rwingx_zatak (�Ԃ��d��)
	���E���猻��A���@�_���e��ł�����A�^���֒��i���Ă���B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_meido2(SPRITE *src)
{
	if (512 > src->jyumyou)/* �ޏ� */
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
	else
	if (512+512 > src->jyumyou)/* �^���ɒ��i */
	{
		src->speed256 += t256(0.2);/*����*/ /*t256(0.3)*/
		if (src->speed256 > t256(2.5)) /*t256(3.0)*/
		{
			src->speed256 = t256(2.5); /* �����ő�l */
			src->jyumyou	= (512-1);
		}
	}
	else
	if (512+512+512 > src->jyumyou)/* �e���� */
	{
		/* CCW�̏ꍇ */
		src->tmp_angleCCW1024					= cv1024r(0);/* �^���Ɍ������Œ� */
		//if (data_hari_rank256/*src->le vel*/)
		{
			zako_shot_supeka(src, ZAKO_SPEKA_0b_meido2);
		}
		src->jyumyou	= (512+512-1);/* ���� */
	}
	else
	if (512+512+512+512 > src->jyumyou)/* �قڐ^�������� */
	{
		/* CCW�̏ꍇ */
		if (src->tmp_angleCCW1024 < cv1024r(180))
		{
			src->tmp_angleCCW1024 -= cv1024r(3);/*fps_factor*/
			if (src->tmp_angleCCW1024 < cv1024r(0))
			{
				src->jyumyou	= (512+512+512-1);/* ���� */
			}
		}
		else
		{
			src->tmp_angleCCW1024 += cv1024r(3);/*fps_factor*/
			if (src->tmp_angleCCW1024 > cv1024r(360))
			{
				src->jyumyou	= (512+512+512-1);/* ���� */
			}
		}
	}
	else
	if (512+512+512+512+512 > src->jyumyou)/* ��ʍ��E����o�� */
	{
		src->speed256 -= t256(0.02);/*fps_factor*/	/* ���� */
		if (src->speed256 <= 0)/* ���x�������Ȃ� */
		{
			src->speed256 = 0;/* ��~ */
			src->jyumyou	= (512+512+512+512-1);/* ���� */
		}
	}
	/*�ȉ�rwingx.c�Ɠ���*/
	zako_move_vector(src);
	/* �A�j���[�V���� */
	zako_anime_type01(src);
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

static void regist_zako_016_meido2(GAME_COMMAND *l, SPRITE *h)
{
	h->jyumyou				= 512+512+512+512+512+(40);
	h->speed256 			= ((ra_nd()&((256*4)-1)));/*2.0*(((dou ble)ra_nd()/RAND_MAX)*2)*/
/* CCW�̏ꍇ */
	if (t256(GAME_WIDTH/2) > h->cx256 )
			{	h->tmp_angleCCW1024 	= cv1024r(180-90-10);	}/* ��ʍ�������o���B�E���� */
	else	{	h->tmp_angleCCW1024 	= cv1024r(180+90+10);	}/* ��ʉE������o���B������ */
}
