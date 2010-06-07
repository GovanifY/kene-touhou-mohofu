
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
	#define time_out			user_data03 	/* �������� */
#endif

#define target_x256 		user_data00 	/* �ڕW�n�_(�o�����̃v���C���[�ʒu) */
#define target_y256 		user_data01 	/* �ڕW�n�_(�o�����̃v���C���[�ʒu) */
#define time_out			user_data03
//#define speed256			user_data03
//#define state 			user_data04
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
	src->time_out--;	/*fps_factor*/
	/* ���Ԑ؂�ŏI��� */
	if (0 > src->time_out)
	{
		src->type = SP_DELETE;
		return;
	}
	else
	if (GENTEN_FRAME128 > src->time_out) //if (src->range<=0)
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
//
//	src->target_x256 += /*src->v*/src->vx256;/*fps_factor*/
//	src->target_y256 += /*src->v*/src->vy256;/*fps_factor*/
//
//	if ((GENTEN_FRAME128-16) < src->time_out)	/* ��莞�Ԃ܂� */
//	if ((256+GENTEN_FRAME128-256-64+16) < src->time_out)	/* ��莞�Ԃ܂� */
//	if ((256-16) < src->time_out)	/* (�x��) ��莞�Ԃ܂� */
	if ((256+32) < src->time_out)	/* ��莞�Ԃ܂� */
	{
		obj_send1->x256 	= (src->target_x256);					/* �e��x256 */
		obj_send1->y256 	= (src->target_y256);					/* �e��y256 */
		tmp_angleCCW1024_jikinerai(obj_send1, src);
		#define ROTATE_1024HARF 	(1024/2)/*����*/
		const int sabun_aaa = (src->tmp_angleCCW1024-src->m_angleCCW1024);
		if (0 > sabun_aaa/*(src->tmp_angleCCW1024-src->m_angleCCW1024)*/ )/* 0���W�X�^+���ʍœK�� */
		{
			/* �ڕW�p�x < ����p�x */
			if ((ROTATE_1024HARF) > sabun_aaa/*(src->tmp_angleCCW1024-src->m_angleCCW1024)*/ )/* ���ʍœK�� */
			{
				src->m_angleCCW1024 -= 10;
			}
		}
		else	/* �ڕW�p�x > ����p�x */
		{
			if ((ROTATE_1024HARF) > -(sabun_aaa)/*(src->m_angleCCW1024-src->tmp_angleCCW1024)*/ )
			{
				src->m_angleCCW1024 += 10;
			}
		}
		#undef ROTATE_1024HARF
		// �����֐��ɂ�������
		{
			int radius_aaa;/* ���x */
		//	radius_aaa = (src->time_out-GENTEN_FRAME128);
		//	radius_aaa = (1+16);
		//	radius_aaa = (cos1024(((src->time_out-GENTEN_FRAME128)<<3)));
		//	radius_aaa = (cos1024(((src->time_out-GENTEN_FRAME128)<<1)));
		//	radius_aaa = (cos1024(((src->time_out-GENTEN_FRAME128)<<2)));
		//	radius_aaa = (cos1024(((src->time_out-GENTEN_FRAME128)<<2)));
			radius_aaa = (cos1024(((src->time_out-GENTEN_FRAME128)<<2)));
		//	src->vx256 = ((sin1024((src->m_angleCCW1024)) ) );	/*CCW*/
		//	src->vy256 = ((cos1024((src->m_angleCCW1024)) ) );
			src->vx256 = ((sin1024((src->m_angleCCW1024))*(radius_aaa))>>6/*>>8*/); 	/*CCW*/
			src->vy256 = ((cos1024((src->m_angleCCW1024))*(radius_aaa))>>6/*>>8*/);
		}
	}
//
	/* ���ړ� */
	src->x256 += src->vx256;	/*CCW*/
	src->y256 += src->vy256;
}


/*---------------------------------------------------------
	�G���̗U���~�T�C�����쐬����
---------------------------------------------------------*/

global void bullet_create_enemy_homing(SPRITE *src)
{
	int i;
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
			h->x256 			= src->x256;
			h->y256 			= src->y256;
	//
			h->m_angleCCW1024	= ((i<<7));//	�~�T�C���̌���
	//
			/* �ڕW�n�_ */
			h->target_x256			= (obj_player->x256);
			h->target_y256			= (obj_player->y256);
	//
			/*h->base.*/h->base_score		= score(5*2);
			/*h->base.*/h->base_hp			= 1;		/* (???)�Ȃ񂩋����C������ */

			h->time_out 	= 256+GENTEN_FRAME128;
		}
	}
}

