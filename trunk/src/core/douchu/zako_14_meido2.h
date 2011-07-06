
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
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
			obj_send1->cx256 					= (src->cx256);
			obj_send1->cy256 					= (src->cy256);
			//efine HARIDAN_SPEED ( /*speed256*/(t256(5)+((data_le vel/*src->le vel*/)<<8) ) )
			#define HARIDAN_SPEED ( /*speed256*/t256(4/*5*/) + (/*data_hari_rank256*/(1<<8)/*src->le vel*/) )
			br.BULLET_REGIST_speed256			= HARIDAN_SPEED;				/* �e�� */
			br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
		//	br.BULLET_REGIST_div_angle1024		= (0);							/* �_�~�[�p�x(���g�p) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_HARI32_00_AOI; 		/* [ �e] */
			br.BULLET_REGIST_n_way				= (1);							/* [1way] */
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
			bullet_regist_vector();
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
	src->cx256+=((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/ 	/* CCW�̏ꍇ */
	src->cy256+=((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
//
	/* �A�j���[�V���� */
	zako_anime_type01(src, TEKI_36_YOUSEI3_1);
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_zako_meido2(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_36_YOUSEI3_1;/*SP_ZAKO*/ /*_13_MEIDO2*/
		h->callback_mover		= move_meido2;
		h->jyumyou				= 512+512+512+512+512+(40);
		h->speed256 			= ((ra_nd()&((256*4)-1)));/*2.0*(((dou ble)ra_nd()/RAND_MAX)*2)*/
	/* CCW�̏ꍇ */
		if (t256(GAME_WIDTH/2) > h->cx256 )
				{	h->tmp_angleCCW1024 	= cv1024r(180-90-10);	}/* ��ʍ�������o���B�E���� */
		else	{	h->tmp_angleCCW1024 	= cv1024r(180+90+10);	}/* ��ʉE������o���B������ */
	}
}
