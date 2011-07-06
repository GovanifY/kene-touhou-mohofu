
#include "douchu.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�U���~�T�C���G
	-------------------------------------------------------
	�U���z�[�~���O�ɂ��悤�Ǝv�����̂�
	�Ȃ����V���b�g����(�쐬��)
	�F�X�������B
---------------------------------------------------------*/
#if 0/* �߂� */
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define boss_time_out		user_data03 	/* �������� */
#endif

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

static void enemy_homing_move(SPRITE *src)
{
	/* ���Ԑ؂�ŏI��� */
	if (0 > src->jyumyou)
	{
		src->jyumyou = JYUMYOU_NASI;
		return;
	}
	else
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
		obj_send1->cx256 	= (src->target_x256);					/* �e��x256 */
		obj_send1->cy256 	= (src->target_y256);					/* �e��y256 */
		tmp_angleCCW1024_src_nerai(obj_send1, src);
		#define ROTATE_1024HARF 	(1024/2)/*����*/
		const int sabun_aaa = (src->tmp_angleCCW1024-src->rotationCCW1024);
		if (0 > sabun_aaa/*(src->tmp_angleCCW1024-src->rotationCCW1024)*/ )/* 0���W�X�^+���ʍœK�� */
		{
			/* �ڕW�p�x < ����p�x */
			if ((ROTATE_1024HARF) > sabun_aaa/*(src->tmp_angleCCW1024-src->rotationCCW1024)*/ )/* ���ʍœK�� */
			{
				src->rotationCCW1024 -= 10;
			}
		}
		else	/* �ڕW�p�x > ����p�x */
		{
			if ((ROTATE_1024HARF) > -(sabun_aaa)/*(src->rotationCCW1024-src->tmp_angleCCW1024)*/ )
			{
				src->rotationCCW1024 += 10;
			}
		}
		#undef ROTATE_1024HARF
		// �����֐��ɂ�������
		{
			int radius_aaa;/* ���x */
		//	radius_aaa = (src->jyumyou-GENTEN_FRAME128);
		//	radius_aaa = (1+16);
		//	radius_aaa = (cos1024(((src->jyumyou-GENTEN_FRAME128)<<3)));
		//	radius_aaa = (cos1024(((src->jyumyou-GENTEN_FRAME128)<<1)));
		//	radius_aaa = (cos1024(((src->jyumyou-GENTEN_FRAME128)<<2)));
		//	radius_aaa = (cos1024(((src->jyumyou-GENTEN_FRAME128)<<2)));
			radius_aaa = (cos1024(((src->jyumyou-GENTEN_FRAME128)<<2)));
		//	src->vx256 = ((sin1024((src->rotationCCW1024)) ) ); /*CCW*/
		//	src->vy256 = ((cos1024((src->rotationCCW1024)) ) );
			src->vx256 = ((sin1024((src->rotationCCW1024))*(radius_aaa))>>6/*>>8*/);	/*CCW*/
			src->vy256 = ((cos1024((src->rotationCCW1024))*(radius_aaa))>>6/*>>8*/);
		}
	}
//
	/* ���ړ� */
	src->cx256 += src->vx256;	/*CCW*/
	src->cy256 += src->vy256;
}


/*---------------------------------------------------------
	�G���̗U���~�T�C�����쐬����
---------------------------------------------------------*/

global void bullet_create_enemy_homing(SPRITE *src)
{
	unsigned int i;
	for (i=0; i<(8)/*HOMING_NU M_OF_ENEMIES*/; i++)
	{
		SPRITE *h;
		h					= sprite_add_gu_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R		= ZAKO_ATARI_HOMING16_PNG;
			h->type 			= /*TEKI_00_BOSS11*/TEKI_63_HOMING_STUFF;
			h->callback_mover	= enemy_homing_move;
			h->flags			|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->cx256 			= src->cx256;
			h->cy256 			= src->cy256;
	//
			h->rotationCCW1024	= ((i<<7));//	�~�T�C���̌���
	//
			/* �ڕW�n�_ */
			{
				SPRITE *zzz_player;
				zzz_player = &obj00[FIX_OBJ_00_PLAYER];
				h->target_x256			= (zzz_player->cx256);
				h->target_y256			= (zzz_player->cy256);
			}
	//
			/*h->base.*/h->base_score		= score(5*2);
			/*h->base.*/h->base_hp			= (1);		/* (???)�Ȃ񂩋����C������ */

			h->jyumyou		= (256+GENTEN_FRAME128);
		}
	}
}

