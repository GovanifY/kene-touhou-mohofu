
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
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
	�G�̗U���z�[�~���O�{��(�Z�p�`�Ş�F�̓z)
---------------------------------------------------------*/

/* �v���C���[�̂����ʒu�ɓ��B�������� */
#define GENTEN_FRAME128 (128)
#define ZERO_FRAME256 (256)

static void teki_homing_move(SPRITE *src)
{
	/* ���Ԑ؂�ŏI��� */
//	if (0 > src->jyumyou)
//	{
//		src->jyumyou = JYUMYOU_NASI;
//		return;
//	}
//	else
	if (GENTEN_FRAME128 > src->jyumyou) //if (src->range<=0)
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
//
//	src->target_x256 += /*src->v*/src->vx256;/*fps_factor*/
//	src->target_y256 += /*src->v*/src->vy256;/*fps_factor*/
//
//	if ((GENTEN_FRAME128-16) < src->jyumyou)	/* ��莞�Ԃ܂� */
//	if ((256+GENTEN_FRAME128-256-64+16) < src->jyumyou) /* ��莞�Ԃ܂� */
//	if ((256-16) < src->jyumyou)	/* (�x��) ��莞�Ԃ܂� */
	if ((256+32) < src->jyumyou)	/* ��莞�Ԃ܂� */
	{
		REG_00_SRC_X 	= (src->target_x256);					/* �e��x256 */
		REG_01_SRC_Y 	= (src->target_y256);					/* �e��y256 */
		REG_02_DEST_X	= ((src->cx256));
		REG_03_DEST_Y	= ((src->cy256));
		tmp_angleCCW65536_src_nerai();
		src->tmp_angleCCW1024 = (deg65536to1024(HATSUDAN_03_angle65536));		/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
		#define ROTATE_1024HARF 	(1024/2)/*����*/
		const int sabun_aaa = (src->tmp_angleCCW1024-src->rotationCCW1024);
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
		//		GENTEN_FRAME128-63, 	//-(((255)>>2)-GENTEN_FRAME128),/*(63==���m�ɑ_��)  (r34�݊�)*/
		//	};
			int radius_aaa;/* ���x */
		//	radius_aaa = (src->jyumyou-GENTEN_FRAME128);
		//	radius_aaa = (1+16);
		//	radius_aaa = (cos1024(((src->jyumyou-GENTEN_FRAME128)<<3)));
		//	radius_aaa = (cos1024(((src->jyumyou-GENTEN_FRAME128)<<1)));
		//	radius_aaa = (cos1024(((src->jyumyou-GENTEN_FRAME128)<<2)));/*(r34�݊�)*/
		//	radius_aaa = (si n1024(((src->jyumyou+((255)>>2)-GENTEN_FRAME128)<<2)));/*(r34�݊�)*/
		//	radius_aaa = (si n1024(((src->jyumyou-ddd[(cg_game_di fficulty)])<<2)));
		{
			int sin_value_t256; 		//	sin_value_t256 = 0;
			int cos_value_t256; 		//	cos_value_t256 = 0;
			int256_sincos1024( (((src->jyumyou-(GENTEN_FRAME128-63))<<2)), &sin_value_t256, &cos_value_t256);/*(63==���m�ɑ_��)  (r34�݊�)*/
			radius_aaa = (sin_value_t256);
		}
		//	src->vx256 = ((si n1024((src->rotationCCW1024)) ) ); /*CCW*/
		//	src->vy256 = ((co s1024((src->rotationCCW1024)) ) );
			#if (0)//
			src->vx256 = ((si n1024((src->rotationCCW1024))*(radius_aaa))>>(6)/*>>8*/);/*fps_factor*/ 	/* CCW�̏ꍇ */
			src->vy256 = ((co s1024((src->rotationCCW1024))*(radius_aaa))>>(6)/*>>8*/);/*fps_factor*/
			#else
			{
				int sin_value_t256; 	//	sin_value_t256 = 0;
				int cos_value_t256; 	//	cos_value_t256 = 0;
				int256_sincos1024( (src->rotationCCW1024), &sin_value_t256, &cos_value_t256);
				src->vx256 = ((sin_value_t256*(radius_aaa))>>(6)/*>>8*/);/*fps_factor*/
				src->vy256 = ((cos_value_t256*(radius_aaa))>>(6)/*>>8*/);/*fps_factor*/
			}
			#endif
		}
	}
	/* ���ړ� */
	src->cx256 += (src->vx256);
	src->cy256 += (src->vy256);
}


/*---------------------------------------------------------
	�G���̗U���~�T�C�����쐬����
---------------------------------------------------------*/

global void bullet_create_teki_homing(SPRITE *src)
{
	unsigned int i;
	for (i=0; i<(8); i++)/* 8==HOMING_NU M_OF_ENEMIES */
	{
		SPRITE *h;
		h					= obj_add_01_teki_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI_HOMING16_PNG;
			h->type 				= ZAKO_63_HOMING_STUFF;
			h->callback_mover		= teki_homing_move;
			/* (r34u1)�󂹂�悤�ɂ��� */
			h->callback_hit_teki	= callback_hit_zako;		/* �u�U�R�Ɏ��e�����������ꍇ�̏����v�ɁA�W���̏�����ݒ� */
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->cx256 				= (src->cx256);
			h->cy256 				= (src->cy256);
	//
			h->rotationCCW1024		= ((i<<7));//	�~�T�C���̌���
			/* �ڕW�n�_ */
			{
				SPRITE *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
				h->target_x256			= (zzz_player->cx256);
				h->target_y256			= (zzz_player->cy256);
			}
			/*h->base.*/h->base_score		= score(5*2);
			/*h->base.*/h->base_hp			= (1);		/* (???)�Ȃ񂩋����C������ */
			h->jyumyou		= (256+GENTEN_FRAME128);
		}
	}
}
