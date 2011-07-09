
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
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
			//efine HARIDAN_SPEED ( /*speed256*/(t256(5)+((data_le vel/*src->le vel*/)<<8) ) )
			#define HARIDAN_SPEED ( /*speed256*/t256(4/*5*/) + (/*data_hari_rank256*/(1<<8)/*src->le vel*/) )
			br.BULLET_REGIST_00_speed256				= HARIDAN_SPEED;				/* �e�� */
			br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN;
		//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_HARI32_00_AOI; 		/* [ �e] */
			br.BULLET_REGIST_06_n_way					= (1);							/* [1way] */
		//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (0);							/* �_�~�[�p�x(���g�p) */
			bullet_regist_multi_vector_send1_xy_src(src); 	/* �e��x256 y256 ���S���甭�e�B */
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
	src->cx256+=((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/	/* CCW�̏ꍇ */
	src->cy256+=((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
//
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
