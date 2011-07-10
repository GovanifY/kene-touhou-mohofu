
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�{�X�̍s�����`����B
---------------------------------------------------------*/


/*---------------------------------------------------------
	[�{�X���ʃJ�[�h�ړ�]�B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�J�[�h�����̊Ԋu�ƈړ����Ԃ̊Ԋu�������B
	xy�����_�������ړ��B
---------------------------------------------------------*/
#if 0
global/*static*/ void core_boss_move_05_xy_douki_differential64(SPRITE *src)
{
	/* �J�[�h�������ĂȂ��ꍇ�ɒǉ� */
	if (SPELL_00==card.card_number) 	/* �J�[�h�����I���Ȃ�J�[�h���� */
	{
		#if 1
		/* ���݈ʒu��荶�E 64dots �͈̔͂Ń����_���ړ��B�A���Œ�16dots�͓����B */
		const unsigned short ra_nd16	= ra_nd();
		src->BOSS_DATA_00_target_x256	= src->cx256;
		{	const unsigned short vector_value = ((ra_nd16&(((64)*256)-1))|(16*256));/* �ړ��ʂ����߂�B �A���Œ�16dots�͓��� */
		//	if (ra_nd16&0x2000) /* �ړ����������߂� */ /* 0x1fff == ((32)*256-1) */
		//	if (src->BOSS_DATA_00_target_x256 < (ra_nd16+ra_nd16)) /* �ړ����������߂� */
			if (src->BOSS_DATA_00_target_x256 < (ra_nd16+((GAME_X_OFFSET+(GAME_320_WIDTH/2)-128)<<8))) /* �ړ����������߂� */
					{	src->BOSS_DATA_00_target_x256 += (vector_value);	}
			else	{	src->BOSS_DATA_00_target_x256 -= (vector_value);	}
		}
		/* ���݈ʒu���㉺ 8dots �͈̔͂Ń����_���ړ� */
		src->BOSS_DATA_01_target_y256 = src->cy256;
		src->BOSS_DATA_01_target_y256 += ((((ra_nd16))&((8+8)-1))<<8);
		src->BOSS_DATA_01_target_y256 -= (t256(8));
		#endif
		card_maikai_init_and_get_spell_number(src); 		/* ���݂̌`�ԂŌ��Ă�J�[�h�ԍ����J�[�h�V�X�e���ɐݒ�B */
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* �U���A�j���[�V���� */
	}
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
}
global void boss_move_05_xy_douki_differential64(SPRITE *src)
{
	core_boss_move_05_xy_douki_differential64(src);
}
#endif
global/*static*/ void boss_move_04_xy_douki_differential32(SPRITE *src)
{
	/* �J�[�h�������ĂȂ��ꍇ�ɒǉ� */
	if (SPELL_00==card.card_number) 	/* �J�[�h�����I���Ȃ�J�[�h���� */
	{
		#if 1
		/* ���݈ʒu��荶�E 32dots �͈̔͂Ń����_���ړ��B�A���Œ�16dots�͓����B */
		const unsigned short ra_nd16	= ra_nd();
		src->BOSS_DATA_00_target_x256	= src->cx256;
		{	const unsigned short vector_value = ((ra_nd16&(((32)*256)-1))|(16*256));/* �ړ��ʂ����߂�B �A���Œ�16dots�͓��� */
		//	if (ra_nd16&0x2000) /* �ړ����������߂� */ /* 0x1fff == ((32)*256-1) */
		//	if (src->BOSS_DATA_00_target_x256 < (ra_nd16+ra_nd16)) /* �ړ����������߂� */
			if (src->BOSS_DATA_00_target_x256 < (ra_nd16+((GAME_X_OFFSET+(GAME_320_WIDTH/2)-128)<<8))) /* �ړ����������߂� */
					{	src->BOSS_DATA_00_target_x256 += (vector_value);	}
			else	{	src->BOSS_DATA_00_target_x256 -= (vector_value);	}
		}
		/* ���݈ʒu���㉺ 8dots �͈̔͂Ń����_���ړ� */
		src->BOSS_DATA_01_target_y256 = src->cy256;
		src->BOSS_DATA_01_target_y256 += ((((ra_nd16))&((8+8)-1))<<8);
		src->BOSS_DATA_01_target_y256 -= (t256(8));
		#endif
		card_maikai_init_and_get_spell_number(src); 		/* ���݂̌`�ԂŌ��Ă�J�[�h�ԍ����J�[�h�V�X�e���ɐݒ�B */
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* �U���A�j���[�V���� */
	}
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
}


/*---------------------------------------------------------
	�J�[�h�����̊Ԋu�ƈړ����Ԃ̊Ԋu�������B
	x�����_���ړ��B
	y�ړ����Ȃ��B
---------------------------------------------------------*/

global void boss_move_03_x_douki(SPRITE *src)
{
	/* �J�[�h�������ĂȂ��ꍇ�ɒǉ� */
	if (SPELL_00==card.card_number) 	/* �J�[�h�����I���Ȃ�J�[�h���� */
	{
		#if 1
		const unsigned short ra_nd16	= ra_nd();
		src->BOSS_DATA_00_target_x256	= ((ra_nd16/*&((256*256)-1)*/));	/* 0(???) =:= t256(((480-128-256)/2)-50)) */
		#endif
		card_maikai_init_and_get_spell_number(src); 		/* ���݂̌`�ԂŌ��Ă�J�[�h�ԍ����J�[�h�V�X�e���ɐݒ�B */
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* �U���A�j���[�V���� */
	}
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
}


/*---------------------------------------------------------
	�J�[�h�����̊Ԋu�ƈړ����Ԃ̊Ԋu���񓯊��B
	x�����_���ړ��B
	y�����_���ړ��B
---------------------------------------------------------*/

global void boss_move_02_xy_hidouki(SPRITE *src)
{
	if (SPELL_00==card.card_number) 	/* �J�[�h�����I���Ȃ�J�[�h���� */
	{
		card_maikai_init_and_get_spell_number(src);
	}
	/* �J�[�h�����̊Ԋu�ƈړ����Ԃ̊Ԋu���񓯊��B */
	src->BOSS_DATA_05_move_jyumyou--;/*fps_factor*/
	{
		if (src->BOSS_DATA_05_move_jyumyou <= 0)
		{
			src->BOSS_DATA_05_move_jyumyou	= (300);	/* �ړ����ԁA�Ԋu */
			int rand32 = ((ra_nd()));
			/* �ړ����W�����߂� */
			src->BOSS_DATA_00_target_x256 = ((rand32&0xff00)+(((rand32<<8)&0x3f00)));	/* (256+64) == 320 == (480-128-32) */
			src->BOSS_DATA_01_target_y256 = (((rand32>>8)&0x3f00)); 	/* (64) */
			src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* �U���A�j���[�V���� */
		}
	}
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
}


/*---------------------------------------------------------
	�����Ȃ��ŃJ�[�h�����B
---------------------------------------------------------*/

global void boss_move_01_not_move(SPRITE *src)
{
	if (SPELL_00==card.card_number) 	/* �J�[�h�����I���Ȃ�J�[�h���� */
	{
		card_maikai_init_and_get_spell_number(src); 		/* ���݂̌`�ԂŌ��Ă�J�[�h�ԍ����J�[�h�V�X�e���ɐݒ�B */
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* �U���A�j���[�V���� */
	}
}


/*---------------------------------------------------------
---------------------------------------------------------*/


/*---------------------------------------------------------
	[�{�X���ʃJ�[�h�s��]�B
---------------------------------------------------------*/


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

static void boss_move_vx_vy_animation(SPRITE *src)
{
	src->cx256 += (src->vx256);
	src->cy256 += (src->vy256);
	#define USE_BOSS_HAMIDASI_CHECK (0)
	#if (1==USE_BOSS_HAMIDASI_CHECK)//1575035
		 if (src->cx256 < rect_clip.boss_clip_min.x256 )	{	src->cx256 = rect_clip.boss_clip_min.x256;	card.boss_hamidasi = (1);	}
	else if (src->cx256 > rect_clip.boss_clip_max.x256 )	{	src->cx256 = rect_clip.boss_clip_max.x256;	card.boss_hamidasi = (1);	}
//
		 if (src->cy256 < rect_clip.boss_clip_min.y256 )	{	src->cy256 = rect_clip.boss_clip_min.y256;	card.boss_hamidasi = (1);	}
	else if (src->cy256 > rect_clip.boss_clip_max.y256 )	{	src->cy256 = rect_clip.boss_clip_max.y256;	card.boss_hamidasi = (1);	}
	#else//1574939
	/*(�Ƃ肠����)*/
	src->cx256 = psp_min(src->cx256, rect_clip.boss_clip_max.x256);/* �E�`�F�b�N(��ݏo��) */
	src->cx256 = psp_max(src->cx256, rect_clip.boss_clip_min.x256);/* ���`�F�b�N(��ݏo��) */
	src->cy256 = psp_min(src->cy256, rect_clip.boss_clip_max.y256);/* ���`�F�b�N(��ݏo��) */
	src->cy256 = psp_max(src->cy256, rect_clip.boss_clip_min.y256);/* ��`�F�b�N(��ݏo��) */
	#endif
//
	if (0 < src->BOSS_DATA_03_kougeki_anime_count)	/* �U���A�j���[�V���� */	/* �U���A�j���[�V�����p�J�E���^ / 0�ȉ��Ȃ�ړ��A�j���[�V���� */
	{
		src->BOSS_DATA_03_kougeki_anime_count--;
				if ((32)>src->BOSS_DATA_03_kougeki_anime_count) {	src->type = BOSS_09_32; }	/*src->an im_frame = 0x23;*/
		else	if ((40)>src->BOSS_DATA_03_kougeki_anime_count) {	src->type = BOSS_10_33; }	/*src->an im_frame = 0x22;*/
		else										{	src->type = BOSS_09_32; }	/*src->an im_frame = 0x21;*/
	}
	else	/* �ړ��A�j���[�V���� */
	{
		int aaa;
		aaa = (src->cx256> src->BOSS_DATA_00_target_x256)?(BOSS_00_11/*0x00*/):(BOSS_04_21/*0x10*/); /* ���E */

		#if 0
				if ( ( 16) > src->BOSS_DATA_04_toutatu_wariai256)		{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else	if ( (200) > src->BOSS_DATA_04_toutatu_wariai256)		{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else	if ( (224) > src->BOSS_DATA_04_toutatu_wariai256)		{	aaa+=2;}	/*128.0==t256(0.5)*/
		else	if ( (240) > src->BOSS_DATA_04_toutatu_wariai256)		{	aaa+=1;}	/*179.2==t256(0.7)*/
		else									{	aaa+=0;}
		#else
				if ( (256- 16) < src->BOSS_DATA_04_toutatu_wariai256)	{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else	if ( (256-200) < src->BOSS_DATA_04_toutatu_wariai256)	{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else	if ( (256-224) < src->BOSS_DATA_04_toutatu_wariai256)	{	aaa+=2;}	/*128.0==t256(0.5)*/
		else	if ( (256-240) < src->BOSS_DATA_04_toutatu_wariai256)	{	aaa+=1;}	/*179.2==t256(0.7)*/
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
	card_boss_move_generate_check_regist(src);	/* �J�[�h�o�^�\�Ȃ�o�^ / �J�[�h���� */
	boss_move_vx_vy_animation(src);
	move_card_square_effect(src);				/* ��G�t�F�N�g */
	card_generate(src); 						/* �X�y����CPU���s���A�J�[�h���P�t���[����������B */
}
