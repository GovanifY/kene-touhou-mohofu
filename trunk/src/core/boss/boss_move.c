
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�A���X�E�}�[�K�g���C�h
	Alice Margatroid.
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0/*����*/
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define boss_time_out		user_data03 	/* �������� */
	#define boss_base_state777	user_data03 	/* ��������(boss_time_out�Ɠ���) */
#endif

/*---------------------------------------------------------
	�e�������̊Ԋu�ƈړ����Ԃ̊Ԋu�������B
	xy�����_�������ړ��B
---------------------------------------------------------*/
#if 0
global/*static*/ void core_boss_move_05_xy_douki_differential64(SPRITE *src)
{
	/* �e���������ĂȂ��ꍇ�ɒǉ� */
	if (SPELL_00==src->boss_base_spell_type)		/* �e�������I���Ȃ�e������ */
	{
		#if 1
		/* ���݈ʒu��荶�E 64dots �͈̔͂Ń����_���ړ��B�A���Œ�16dots�͓����B */
		const unsigned short ra_nd16 = ra_nd();
		src->target_x256 = src->cx256;
		{	const unsigned short vector_value = ((ra_nd16&(((64)*256)-1))|(16*256));/* �ړ��ʂ����߂�B �A���Œ�16dots�͓��� */
		//	if (ra_nd16&0x2000) /* �ړ����������߂� */ /* 0x1fff == ((32)*256-1) */
		//	if (src->target_x256 < (ra_nd16+ra_nd16)) /* �ړ����������߂� */
			if (src->target_x256 < (ra_nd16+(((GAME_WIDTH/2)-128)<<8))) /* �ړ����������߂� */
					{	src->target_x256 += (vector_value); 	}
			else	{	src->target_x256 -= (vector_value); 	}
		}
		/* ���݈ʒu���㉺ 8dots �͈̔͂Ń����_���ړ� */
		src->target_y256 = src->cy256;
		src->target_y256 += ((((ra_nd16))&((8+8)-1))<<8);
		src->target_y256 -= (t256(8));
		#endif
		spell_card_get_spell_number(src);	/* �X�y�J�V�X�e������A���݂̌`�ԂŌ��e���ԍ�����ɓ���A�W����boss_base_spell_time_out���Ԃ�ݒ�B */
		src->vvv256 = t256(1.0);
	}
	#if 1
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	#endif
}
global void boss_move_05_xy_douki_differential64(SPRITE *src)
{
	core_boss_move_05_xy_douki_differential64(src);
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}
#endif
global/*static*/ void core_boss_move_04_xy_douki_differential32(SPRITE *src)
{
	/* �e���������ĂȂ��ꍇ�ɒǉ� */
	if (SPELL_00==src->boss_base_spell_type)		/* �e�������I���Ȃ�e������ */
	{
		#if 1
		/* ���݈ʒu��荶�E 32dots �͈̔͂Ń����_���ړ��B�A���Œ�16dots�͓����B */
		const unsigned short ra_nd16 = ra_nd();
		src->target_x256 = src->cx256;
		{	const unsigned short vector_value = ((ra_nd16&(((32)*256)-1))|(16*256));/* �ړ��ʂ����߂�B �A���Œ�16dots�͓��� */
		//	if (ra_nd16&0x2000) /* �ړ����������߂� */ /* 0x1fff == ((32)*256-1) */
		//	if (src->target_x256 < (ra_nd16+ra_nd16)) /* �ړ����������߂� */
			if (src->target_x256 < (ra_nd16+(((GAME_WIDTH/2)-128)<<8))) /* �ړ����������߂� */
					{	src->target_x256 += (vector_value); 	}
			else	{	src->target_x256 -= (vector_value); 	}
		}
		/* ���݈ʒu���㉺ 8dots �͈̔͂Ń����_���ړ� */
		src->target_y256 = src->cy256;
		src->target_y256 += ((((ra_nd16))&((8+8)-1))<<8);
		src->target_y256 -= (t256(8));
		#endif
		spell_card_get_spell_number(src);	/* �X�y�J�V�X�e������A���݂̌`�ԂŌ��e���ԍ�����ɓ���A�W����boss_base_spell_time_out���Ԃ�ݒ�B */
		src->vvv256 = t256(1.0);
	}
	#if 1
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	#endif
}
global void boss_move_04_xy_douki_differential32(SPRITE *src)
{
	core_boss_move_04_xy_douki_differential32(src);
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}


/*---------------------------------------------------------
	�e�������̊Ԋu�ƈړ����Ԃ̊Ԋu�������B
	x�����_���ړ��B
	y�ړ����Ȃ��B
---------------------------------------------------------*/

global void boss_move_03_x_douki(SPRITE *src)
{
	/* �e���������ĂȂ��ꍇ�ɒǉ� */
	if (SPELL_00==src->boss_base_spell_type)		/* �e�������I���Ȃ�e������ */
	{
		#if 1
		const unsigned short ra_nd16 =ra_nd();
		src->target_x256 = ((ra_nd16/*&((256*256)-1)*/));	/* 0(???) =:= t256(((480-128-256)/2)-50)) */
		#endif
		spell_card_get_spell_number(src);	/* �X�y�J�V�X�e������A���݂̌`�ԂŌ��e���ԍ�����ɓ���A�W����boss_base_spell_time_out���Ԃ�ݒ�B */
		src->vvv256 = t256(1.0);
	}
	#if 1
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	#endif
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}


/*---------------------------------------------------------
	�e�������̊Ԋu�ƈړ����Ԃ̊Ԋu���񓯊��B
	x�����_���ړ��B
	y�����_���ړ��B
---------------------------------------------------------*/

global void boss_move_02_xy_hidouki(SPRITE *src)
{
	if (SPELL_00==src->boss_base_spell_type)		/* �e�������I���Ȃ�e������ */
	{
		spell_card_get_spell_number(src);
	}
	/* �e�������̊Ԋu�ƈړ����Ԃ̊Ԋu���񓯊��B */
	src->boss_time_out--;/*fps_factor*/
	{
		if (src->boss_time_out <= 0)
		{
			src->boss_time_out	= (300);	/* �ړ����ԁA�Ԋu */
			int rand32 = ((ra_nd()));
			/* �ړ����W�����߂� */
			src->target_x256 = ((rand32&0xff00)+(((rand32<<8)&0x3f00)));	/* (256+64) == 320 == (480-128-32) */
			src->target_y256 = (((rand32>>8)&0x3f00));	/* (64) */
			/* �U���A�j���[�V���� */
			src->vvv256 = t256(1.0);
		}
	}
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}


/*---------------------------------------------------------
	�����Ȃ��ŃX�y�J���B
---------------------------------------------------------*/

global void boss_move_01_not_move(SPRITE *src)
{
	if (SPELL_00==src->boss_base_spell_type)		/* �e�������I���Ȃ�e������ */
	{
		spell_card_get_spell_number(src);	/* �X�y�J�V�X�e������A���݂̌`�ԂŌ��e���ԍ�����ɓ���A�W����boss_base_spell_time_out���Ԃ�ݒ�B */
		src->vvv256 = t256(1.0);
	}
//
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}
