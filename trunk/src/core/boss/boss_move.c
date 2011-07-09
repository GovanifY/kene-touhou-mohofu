
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
	#define target_x256 			user_data00 	/* �ڕWx���W */
	#define target_y256 			user_data01 	/* �ڕWy���W */
	#define toutatu_wariai256		user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define kougeki_anime_count 	user_data03 	/* �U���A�j���[�V�����p�J�E���^ */
	#define boss_time_out			user_data04 	/* �������� */
	#define boss_base_state777		user_data04 	/* ��������(boss_time_out�Ɠ���) */
//
	#define boss_spell_timer		user_data05 	/* �X�y������ */
#endif

/*---------------------------------------------------------
	[�{�X���ʃX�y�J�ړ�]�B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�e�������̊Ԋu�ƈړ����Ԃ̊Ԋu�������B
	xy�����_�������ړ��B
---------------------------------------------------------*/
#if 0
global/*static*/ void core_boss_move_05_xy_douki_differential64(SPRITE *src)
{
	/* �e���������ĂȂ��ꍇ�ɒǉ� */
	if (SPELL_00==spell_card.spell_type)		/* �e�������I���Ȃ�e������ */
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
		src->toutatu_wariai256 = t256(1.0);
	}
	#if 1
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
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
	if (SPELL_00==spell_card.spell_type)		/* �e�������I���Ȃ�e������ */
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
		src->toutatu_wariai256 = t256(1.0);
	}
	#if 1
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
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
	if (SPELL_00==spell_card.spell_type)		/* �e�������I���Ȃ�e������ */
	{
		#if 1
		const unsigned short ra_nd16 =ra_nd();
		src->target_x256 = ((ra_nd16/*&((256*256)-1)*/));	/* 0(???) =:= t256(((480-128-256)/2)-50)) */
		#endif
		spell_card_get_spell_number(src);	/* �X�y�J�V�X�e������A���݂̌`�ԂŌ��e���ԍ�����ɓ���A�W����boss_base_spell_time_out���Ԃ�ݒ�B */
		src->toutatu_wariai256 = t256(1.0);
	}
	#if 1
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
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
	if (SPELL_00==spell_card.spell_type)		/* �e�������I���Ȃ�e������ */
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
			src->toutatu_wariai256 = t256(1.0);
		}
	}
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}


/*---------------------------------------------------------
	�����Ȃ��ŃX�y�J���B
---------------------------------------------------------*/

global void boss_move_01_not_move(SPRITE *src)
{
	if (SPELL_00==spell_card.spell_type)		/* �e�������I���Ȃ�e������ */
	{
		spell_card_get_spell_number(src);	/* �X�y�J�V�X�e������A���݂̌`�ԂŌ��e���ԍ�����ɓ���A�W����boss_base_spell_time_out���Ԃ�ݒ�B */
		src->toutatu_wariai256 = t256(1.0);
	}
//
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}


/*---------------------------------------------------------
---------------------------------------------------------*/
/*---------------------------------------------------------
---------------------------------------------------------*/
/*---------------------------------------------------------
---------------------------------------------------------*/


/*---------------------------------------------------------
	[�{�X���ʃX�y�J�s��]�B
---------------------------------------------------------*/




/* �{�X�ړ������̋��ʃ��[�`�� */

//extern void bo ss_move_vx_vy(SPRITE *src);
//extern void bo ss_clip_rect(SPRITE *src);

/*---------------------------------------------------------
	�{�X�ړ������̋��ʃ��[�`��
	-------------------------------------------------------
�� �ړ��͈͂̓o�^�A�@�\�F
	���炩���߃{�X�ړ��͈͂�o�^���Ă����B
�� �͈͓��Ȃ�Έړ��A�@�\�F
	�ړ��͈͓��Ȃ�΁A�{�X���ړ�����B
	�ړ��͈͊O�Ȃ�΁A�{�X�͈ړ����Ȃ��B
�� �ړ�����A�@�\�F
	�{�X���ړ����Ȃ��ꍇ�́A�u�ړ��ł��Ȃ������t���O�v���n�m�ɂȂ�B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�{�X�U���䗦�v�Z�B
---------------------------------------------------------*/
/*extern*/global POINT256 boss_clip_min;		/* �{�X�ړ��͈�(�ŏ��l) */
/*extern*/global POINT256 boss_clip_max;		/* �{�X�ړ��͈�(�ő�l) */

/*extern*/global int boss_hamidasi; 			/* �u�ړ��ł��Ȃ������t���O�v(�g�p�O�Ɏ蓮��OFF==0�ɂ��Ƃ�) */

/*global*/static/*static*/ void boss_move_vx_vy_animation(SPRITE *src)
{
	src->cx256 += (src->vx256);
	src->cy256 += (src->vy256);
		 if (src->cx256 < boss_clip_min.x256 )	{	src->cx256 = boss_clip_min.x256;	boss_hamidasi=1;	}
	else if (src->cx256 > boss_clip_max.x256 )	{	src->cx256 = boss_clip_max.x256;	boss_hamidasi=1;	}
//
		 if (src->cy256 < boss_clip_min.y256 )	{	src->cy256 = boss_clip_min.y256;	boss_hamidasi=1;	}
	else if (src->cy256 > boss_clip_max.y256 )	{	src->cy256 = boss_clip_max.y256;	boss_hamidasi=1;	}
//
	if (0 < src->kougeki_anime_count)	/* �U���A�j���[�V���� */	/* �U���A�j���[�V�����p�J�E���^ / 0�ȉ��Ȃ�ړ��A�j���[�V���� */
	{
		src->kougeki_anime_count--;
				if ((32)>src->kougeki_anime_count)	{	src->type = TEKI_09_BOSS32; }	/*src->an im_frame = 0x23;*/
		else	if ((40)>src->kougeki_anime_count)	{	src->type = TEKI_10_BOSS33; }	/*src->an im_frame = 0x22;*/
		else										{	src->type = TEKI_09_BOSS32; }	/*src->an im_frame = 0x21;*/
	}
	else	/* �ړ��A�j���[�V���� */
	{
		int aaa;
		aaa = (src->cx256> src->target_x256)?(TEKI_00_BOSS11/*0x00*/):(TEKI_04_BOSS21/*0x10*/); /* ���E */

		#if 0
				if ( ( 16) > src->toutatu_wariai256)		{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else	if ( (200) > src->toutatu_wariai256)		{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else	if ( (224) > src->toutatu_wariai256)		{	aaa+=2;}	/*128.0==t256(0.5)*/
		else	if ( (240) > src->toutatu_wariai256)		{	aaa+=1;}	/*179.2==t256(0.7)*/
		else									{	aaa+=0;}
		#else
				if ( (256- 16) < src->toutatu_wariai256)	{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else	if ( (256-200) < src->toutatu_wariai256)	{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else	if ( (256-224) < src->toutatu_wariai256)	{	aaa+=2;}	/*128.0==t256(0.5)*/
		else	if ( (256-240) < src->toutatu_wariai256)	{	aaa+=1;}	/*179.2==t256(0.7)*/
		else									{	aaa+=0;}
		#endif
		src->type = aaa;
	}
}
/*---------------------------------------------------------
	�{�X�s��
---------------------------------------------------------*/

global void common_boss_move(SPRITE *src)
{
	check_regist_generate_spell_card(src);	/* �X�y�J�o�^�\�Ȃ�o�^ / �X�y�J���� */
//
	boss_move_vx_vy_animation(src);
//
	move_boss_effect(src);	/* ��G�t�F�N�g */
	spell_generator(src);	/* �e������ */
}







