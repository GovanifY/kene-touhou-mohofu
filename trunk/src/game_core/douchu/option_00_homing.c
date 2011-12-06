
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�U���~�T�C���G
	-------------------------------------------------------
	�U���z�[�~���O�ɂ��悤�Ǝv�����̂�
	�Ȃ����V���b�g����(�쐬��)
	�F�X�������B
---------------------------------------------------------*/

#define target_x256 		user_data00 	/* �ڕW�n�_(�o�����̃v���C���[�ʒu) */
#define target_y256 		user_data01 	/* �ڕW�n�_(�o�����̃v���C���[�ʒu) */

#define radius				user_data04 	/* ���a */
#define ax256				user_data05 	/* �� */
#define ay256				user_data06 	/* �� */


/*---------------------------------------------------------
	�G�̗U���z�[�~���O�{��(�ΐF�̓z)
---------------------------------------------------------*/

/* �v���C���[�̂����ʒu�ɓ��B�������� */
#define GENTEN_FRAME128 (128)
#define ZERO_FRAME256	(256)

static OBJ_CALL_FUNC(teki_homing_move)
{
	if (GENTEN_FRAME128 > src->jyumyou)
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
//	if ((GENTEN_FRAME128-16) < src->jyumyou)	/* ��莞�Ԃ܂� */
//	if ((256+GENTEN_FRAME128-256-64+16) < src->jyumyou) /* ��莞�Ԃ܂� */
//	if ((256-16) < src->jyumyou)	/* (�x��) ��莞�Ԃ܂� */
	if ((256+32) < src->jyumyou)	/* ��莞�Ԃ܂� */
	{
		REG_00_SRC_X	= (src->target_x256);					/* �e��x256 */
		REG_01_SRC_Y	= (src->target_y256);					/* �e��y256 */
		REG_02_DEST_X	= ((src->center.x256));
		REG_03_DEST_Y	= ((src->center.y256));
		tmp_angleCCW65536_src_nerai();
		#define ROTATE_1024HARF 	(1024/2)/*����*/
		const int sabun_aaa = ((deg65536to1024(HATSUDAN_03_angle65536))-src->rotationCCW1024);
		if (0 > (sabun_aaa) )/* 0���W�X�^+���ʍœK�� */
		{
			/* �ڕW�p�x < ����p�x */
			if ((ROTATE_1024HARF) > (sabun_aaa) )/* ���ʍœK�� */
			{
				src->rotationCCW1024 -= (10);
			}
		}
		else	/* �ڕW�p�x > ����p�x */
		{
			if ((ROTATE_1024HARF) > -(sabun_aaa) )
			{
				src->rotationCCW1024 += (10);
			}
		}
		#undef ROTATE_1024HARF
		// �����֐��ɂ�������
		{
		//	const u8 ddd[4] =
		//	{
		//		GENTEN_FRAME128-31, 	//-(((255)>>2)-GENTEN_FRAME128),/*(�܂��_��Ȃ��B�قڌ����邾��)*/
		//		GENTEN_FRAME128- 0, 	//-(((255)>>2)-GENTEN_FRAME128),/*(�قڑ_��Ȃ��B�債�ē���Ȃ�)*/
		//		GENTEN_FRAME128-47, 	//-(((255)>>2)-GENTEN_FRAME128),/*(�����_���B)*/
		//		GENTEN_FRAME128-63, 	//-(((255)>>2)-GENTEN_FRAME128),/*(63==���m�ɑ_��)	(r34�݊�)*/
		//	};
		//	int radius_aaa;/* ���x */
			//------------------
			HATSUDAN_01_speed256	= (t256(1.0));
			HATSUDAN_03_angle65536	= (deg1024to65536((((src->jyumyou-(GENTEN_FRAME128-63))<<2))));
			sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
		//	radius_aaa = REG_03_DEST_Y;//sin_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
		//	src->math_vector.y256 = REG_02_DEST_X;//cos_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
			//------------------
			HATSUDAN_01_speed256	= ((REG_03_DEST_Y)<<2);
			HATSUDAN_03_angle65536	= (deg1024to65536(src->rotationCCW1024));
			sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
			src->math_vector.x256 = REG_03_DEST_Y;//sin_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
			src->math_vector.y256 = REG_02_DEST_X;//cos_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
			//------------------
		}
	}
	/* ���ړ� */
	src->center.x256 += (src->math_vector.x256);
	src->center.y256 += (src->math_vector.y256);
}


/*---------------------------------------------------------
	�G���̗U���~�T�C�����쐬����
---------------------------------------------------------*/

global OBJ_CALL_FUNC(bullet_create_teki_homing)
{
	/* �ڕW�n�_ */
	OBJ *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
	//
	unsigned int i_angle;
	for (i_angle=0; i_angle<(1024); i_angle += (1024/8) )/* */
	{
		OBJ *h;
		h					= obj_regist_teki();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI_HOMING16_PNG;
			h->obj_type_set 		= ZAKO_63_HOMING_STUFF;
			h->callback_mover		= teki_homing_move;
			/* (r34u1)�󂹂�悤�ɂ��� */
			h->callback_hit_teki	= callback_hit_zako;		/* �u�U�R�Ɏ��e�����������ꍇ�̏����v�ɁA�W���̏�����ݒ� */
			h->atari_hantei 		= (score(5*2)/*�X�R�A���p*/);
			h->base_hp				= (1);		/* �U���~�T�C���̍d���B */
			h->center.x256				= (src->center.x256);/* ���e�ʒu */
			h->center.y256				= (src->center.y256);/* ���e�ʒu */
	//
			h->rotationCCW1024		= (i_angle);// �~�T�C���̌���
			{
				h->target_x256			= (zzz_player->center.x256);/* �ڕW�ʒu */
				h->target_y256			= (zzz_player->center.y256);/* �ڕW�ʒu */
			}
			h->jyumyou		= (256+GENTEN_FRAME128);
		}
	}
}
