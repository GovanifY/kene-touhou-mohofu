
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"�A�z��1",		"PLASMABALL",
	-------------------------------------------------------
	(��)���ыʁA(601��)�d��
	(4*6)��24�C���o�Ă��邪���܂�Ӗ��̂Ȃ��L����
	-------------------------------------------------------
	�ԉf�˕�(?)�ɕς����B
	���Ȃ�e�L�g�[�B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_inyou1(SPRITE *src)
{
//	if ( 0==(ra_nd()&(16-1)) ) /*�m���グ���B[1/16]��[1/20]*/ /*%20*/
	if ( 0==(ra_nd()&( 8-1)) ) /*�m���グ���B[1/8]��[1/20]*/ /*%20*/
	{
		obj_send1->cx256 = src->cx256;
		obj_send1->cy256 = src->cy256;
		br.BULLET_REGIST_speed256			= t256(5.0);				/* �e�� */
		br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* */
		br.BULLET_REGIST_div_angle1024		= (int)(1024/(24)); 		/* �����p�x */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_HARI32_01_AKA; 	/* [�Ԑj�e] */
		br.BULLET_REGIST_n_way				= (5);						/* [5way] */
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_vector();
	}
	else
	{
		item_create(src, (SP_ITEM_00_P001+(ra_nd()&1)), 1, ITEM_MOVE_FLAG_06_RAND_XY);
	}
}
/*50%(SP_ITEM_00_P001 or SP_ITEM_01_P008)*/
//66%==SP_ITEM_04_BOMB or 33%==SP_ITEM_01_P008	 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_inyou1(SPRITE *src)
{
	src->cx256 += src->vx256;
	/* ��ʗ��[�ł́A���˕Ԃ� */
	if (
		((t256(0)) > (src->cx256) ) ||
		((t256(GAME_WIDTH)) < (src->cx256) ) )
	{
		src->vx256 = -src->vx256;
	}
	/* �ړ������ɍ��킹�ăO����] */
	if (0 < src->vx256)
	{
		src->rotationCCW1024 += (24);	/*fps_factor*/
	}
	else
	{
		src->rotationCCW1024 -= (24);	/*fps_factor*/
	}
	/* ������ */
	if ((t256(2.50)) > src->vy256)	/* �ő�d�͉����x */
	{
		src->vy256 += (8);	/* �d�͉����x */
	}
	/* */
	src->cy256 += src->vy256;
	if ( (src->limit_y256) < (src->cy256) )
	{
	//	src->cy256			= src->limit_y256;
		src->limit_y256 	= t256(300);
		src->vy256			= -src->vy256;	/* ���˂� */
	}
	/* ��ʉ��܂ŗ�����A�����܂� */
	if (t256(272) < (src->cy256) )
	{
		src->jyumyou = JYUMYOU_NASI;	/* �����܂� */
	}
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_zako_inyou1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_53_INYOU_DAI;
		h->callback_mover		= move_inyou1;
		h->callback_loser		= lose_inyou1;
		h->limit_y256			= (t256(128)-((/*yyy*/(l->user_255_code)&0x0f)<<(5+8)));		/* 臒l */
								/*t256(200)-(j*t256(40))*/
								/*(-(xxx*t256(20))-(yyy*t256(50)))*/
		h->vx256				= (ra_nd()&0x1ff);
		h->vy256				= (ra_nd()&0x1ff);
	}
}
