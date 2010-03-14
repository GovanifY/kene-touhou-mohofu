
#include "douchu.h"

/*---------------------------------------------------------
	�U���~�T�C���G
	-------------------------------------------------------
�U���z�[�~���O�ɂ��悤�Ǝv�����̂�
�Ȃ����V���b�g����(�쐬��)
---------------------------------------------------------*/

#define tx256		user_data00 	/* �ڕW�n�_(�o�����̃v���C���[�ʒu) */
#define ty256		user_data01 	/* �ڕW�n�_(�o�����̃v���C���[�ʒu) */
#define time_out	user_data02
//#define speed256	user_data03
//#define state 	user_data04
#define radius		user_data03 	/* ���a */

#define ax256		user_data04 	/* �� */
#define ay256		user_data05 	/* �� */

#if 0
/*---------------------------------------------------------
	���@�_���e�̊p�x���v�Z
---------------------------------------------------------*/
static int angle_jikinerai512(SPRITE *p, SPRITE *t)
{
	return (atan_512(p->y256-t->y256, p->x256-t->x256));
	/* �����e�̏ꍇ�Ɏ��@�_��(�ȗ���) �e�Ǝ������傫���������Ȃ炸��Ȃ��A�Ⴄ�Ƃ��̕��덷�ɂȂ� */
}
#endif

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
		gamen_gai_nara_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
//
//	src->tx256 += /*src->v*/src->vx256;/*fps_factor*/
//	src->ty256 += /*src->v*/src->vy256;/*fps_factor*/
//
//	if ((GENTEN_FRAME128-16) < src->time_out) 	/* ��莞�Ԃ܂� */
//	if ((256+GENTEN_FRAME128-256-64+16) < src->time_out) 	/* ��莞�Ԃ܂� */
//	if ((256-16) < src->time_out) 	/* (�x��) ��莞�Ԃ܂� */
	if ((256+32) < src->time_out) 	/* ��莞�Ԃ܂� */
	{
		src->tmp_angleCCW512 = (atan_512(src->ty256-src->y256, src->tx256-src->x256));
		const int sabun_aaa = (src->tmp_angleCCW512-src->m_angleCCW512);
		if (0 > sabun_aaa/*(src->tmp_angleCCW512-src->m_angleCCW512)*/ )/* 0���W�X�^+���ʍœK�� */
		{	/* �ڕW�p�x < ����p�x */
			if (256 > sabun_aaa/*(src->tmp_angleCCW512-src->m_angleCCW512)*/ )/* ���ʍœK�� */
			{
				src->m_angleCCW512 -= 5;
			}
		}
		else	/* �ڕW�p�x > ����p�x */
		{
			if (256 > -(sabun_aaa)/*(src->m_angleCCW512-src->tmp_angleCCW512)*/ )
			{
				src->m_angleCCW512 += 5;
			}
		}
		// �����֐��ɂ�������
		{
			int radius_aaa;/* ���x */
		//	radius_aaa = (src->time_out-GENTEN_FRAME128);
		//	radius_aaa = (1+16);
		//	radius_aaa = (cos512(((src->time_out-GENTEN_FRAME128)<<2)));
		//	radius_aaa = (cos512(((src->time_out-GENTEN_FRAME128)   )));
		//	radius_aaa = (cos512(((src->time_out-GENTEN_FRAME128)<<1)));
			radius_aaa = (cos512(((src->time_out-GENTEN_FRAME128)<<1)));
		//	src->vx256 = ((sin512((src->m_angleCCW512)) ) );	/*CCW*/
		//	src->vy256 = ((cos512((src->m_angleCCW512)) ) );
			src->vx256 = ((sin512((src->m_angleCCW512))*(radius_aaa))>>6/*>>8*/);	/*CCW*/
			src->vy256 = ((cos512((src->m_angleCCW512))*(radius_aaa))>>6/*>>8*/);
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

void bullet_create_enemy_homing(SPRITE *src)
{
	int i;
	for (i=0; i<(8)/*HOMING_NU M_OF_ENEMIES*/; i++)
	{
		SPRITE *h;
		h					= sprite_add_gu(TAMA_TYPE_HOMING16_PNG);
		h->type 			= /*TEKI_00_BOSS11*/TEKI_63_HOMING_STUFF;
		h->callback_mover	= enemy_homing_move;
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		h->x256 			= src->x256;
		h->y256 			= src->y256;
//
		h->m_angleCCW512	= ((i<<6));//	�~�T�C���̌���
//
		/* �ڕW�n�_ */
		h->tx256			= (player->x256);
		h->ty256			= (player->y256);
//
		/*h->base.*/h->base_score		= score(5*2);
		/*h->base.*/h->base_hp			= 1;		/* (???)�Ȃ񂩋����C������ */

		h->time_out 	= 256+GENTEN_FRAME128;
	}
}

