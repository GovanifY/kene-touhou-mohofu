
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�{�X�̍s�����`����B
---------------------------------------------------------*/


/*---------------------------------------------------------
	[�{�X���ʃJ�[�h�ړ�]�B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�J�[�h�����I���Ȃ�J�[�h�ݒ肵�A�����B
---------------------------------------------------------*/
/*static*/global OBJ_CALL_FUNC(boss_move_00_card_settei_seisei)
{
	if (SPELL_00==card.spell_used_number)	/* �J�[�h�����I���Ȃ�J�[�h���� */
	{
		card_maikai_init_and_get_spell_number(src); 		/* ���݂̌`�ԂŌ��Ă�J�[�h�ԍ����J�[�h�V�X�e���ɐݒ�B */
	}
}

/*---------------------------------------------------------
	x�����_��32[pixel]�ȓ������ړ��B
	y�����_�� 8[pixel]�ȓ������ړ��B
---------------------------------------------------------*/

static OBJ_CALL_FUNC(boss_set_position_differential32)
{
	#if 1
	/* ���݈ʒu��荶�E 32[pixel] �͈̔͂Ń����_���ړ��B�A���Œ�16[pixel]�͓����B */
	const unsigned short ra_nd16	= ra_nd();
	REG_02_DEST_X	= src->center.x256;
	{	const unsigned short vector_value = ((ra_nd16&(((32)*256)-1))|(16*256));/* �ړ��ʂ����߂�B �A���Œ�16[pixel]�͓��� */
	//	if (ra_nd16&0x2000) /* �ړ����������߂� */ /* 0x1fff == ((32)*256-1) */
	//	if (REG_02_DEST_X < (ra_nd16+ra_nd16)) /* �ړ����������߂� */
		if (REG_02_DEST_X < (ra_nd16+((GAME_X_OFFSET+(GAME_320_WIDTH/2)-128)<<8))) /* �ړ����������߂� */
				{	REG_02_DEST_X += (vector_value);	}
		else	{	REG_02_DEST_X -= (vector_value);	}
	}
	/* ���݈ʒu���㉺ 8[pixel] �͈̔͂Ń����_���ړ� */
	REG_03_DEST_Y = src->center.y256;
	REG_03_DEST_Y += ((((ra_nd16))&((8+8)-1))<<8);
	REG_03_DEST_Y -= (t256(8));
	#endif
}
#if 0
/*---------------------------------------------------------
	x�����_��64[pixel]�ȓ������ړ��B
	y�����_�� 8[pixel]�ȓ������ړ��B
---------------------------------------------------------*/

static OBJ_CALL_FUNC(boss_set_position_differential64)
{
	#if 1
	/* ���݈ʒu��荶�E 64[pixel] �͈̔͂Ń����_���ړ��B�A���Œ�16[pixel]�͓����B */
	const unsigned short ra_nd16	= ra_nd();
	REG_02_DEST_X	= src->center.x256;
	{	const unsigned short vector_value = ((ra_nd16&(((64)*256)-1))|(16*256));/* �ړ��ʂ����߂�B �A���Œ�16[pixel]�͓��� */
	//	if (ra_nd16&0x2000) /* �ړ����������߂� */ /* 0x1fff == ((32)*256-1) */
	//	if (REG_02_DEST_X < (ra_nd16+ra_nd16)) /* �ړ����������߂� */
		if (REG_02_DEST_X < (ra_nd16+((GAME_X_OFFSET+(GAME_320_WIDTH/2)-128)<<8))) /* �ړ����������߂� */
				{	REG_02_DEST_X += (vector_value);	}
		else	{	REG_02_DEST_X -= (vector_value);	}
	}
	/* ���݈ʒu���㉺ 8[pixel] �͈̔͂Ń����_���ړ� */
	REG_03_DEST_Y = src->center.y256;
	REG_03_DEST_Y += ((((ra_nd16))&((8+8)-1))<<8);
	REG_03_DEST_Y -= (t256(8));
	#endif
}
#endif
/*---------------------------------------------------------
	�J�[�h�����̊Ԋu�ƈړ����Ԃ̊Ԋu�������B
	xy�����_�������ړ��B
---------------------------------------------------------*/

//obal/*static*/ OBJ_CALL_FUNC(boss_move_05_xy_douki_differential64)
global/*static*/ OBJ_CALL_FUNC(boss_move_04_xy_douki_differential32)
{
	/* �J�[�h�������ĂȂ��ꍇ�ɒǉ� */
	if (SPELL_00==card.spell_used_number)	/* �J�[�h�����I���Ȃ�A�V�ړ�������߂�B */
	{
	//	boss_set_position_differential64(src);
		boss_set_position_differential32(src);
		#if (1)
		/* �ړ����W�����߂� */
	//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* �U���A�j���[�V���� */
		boss_set_new_position(src);/*(�U���ړ����W��ݒ�)*/
		#endif
	}
	/*(�ȉ��͏����I�ɋ��ʉ������\��)*/
	boss_move_00_card_settei_seisei(src);/*(�J�[�h�����I���Ȃ�J�[�h�ݒ肵�A�����B)*/
//r36���Ԃ�B	boss_re gacy_yuudou(src);
}


/*---------------------------------------------------------
	�J�[�h�����̊Ԋu�ƈړ����Ԃ̊Ԋu�������B
	x�����_���ړ��B
	y�ړ����Ȃ��B
---------------------------------------------------------*/

global OBJ_CALL_FUNC(boss_move_03_x_douki)
{
	/* �J�[�h�������ĂȂ��ꍇ�ɒǉ� */
	if (SPELL_00==card.spell_used_number)	/* �J�[�h�����I���Ȃ�A�V�ړ�������߂�B */
	{
		#if 1
		const unsigned short ra_nd16	= ra_nd();
		REG_02_DEST_X	= ((ra_nd16/*&((256*256)-1)*/));	/* 0(???) =:= t256(((480-128-256)/2)-50)) */
		REG_03_DEST_Y	= (src->center.y256);/*(t256()�`��)*/
		#endif
		#if (1)
		/* �ړ����W�����߂� */
	//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* �U���A�j���[�V���� */
		boss_set_new_position(src);/*(�U���ړ����W��ݒ�)*/
		#endif
	}
	/*(�ȉ��͏����I�ɋ��ʉ������\��)*/
	boss_move_00_card_settei_seisei(src);/*(�J�[�h�����I���Ȃ�J�[�h�ݒ肵�A�����B)*/
//r36���Ԃ�B	boss_re gacy_yuudou(src);
}


/*---------------------------------------------------------
	�J�[�h�����̊Ԋu�ƈړ����Ԃ̊Ԋu���񓯊��B
	x�����_���ړ��B
	y�����_���ړ��B
---------------------------------------------------------*/

global OBJ_CALL_FUNC(boss_move_02_xy_hidouki)
{
	/* �J�[�h�����̊Ԋu�ƈړ����Ԃ̊Ԋu���񓯊��B */
	src->BOSS_DATA_05_move_jyumyou--;/*fps_factor*/
	{
		if (src->BOSS_DATA_05_move_jyumyou <= 0)
		{
			src->BOSS_DATA_05_move_jyumyou	= (300);	/* �ړ����ԁA�Ԋu */
			#if 1
			int rand32 = ((ra_nd()));
			/* �ړ����W�����߂� */
			REG_02_DEST_X = ((rand32&0xff00)+(((rand32<<8)&0x3f00)));	/* (256+64) == 320 == (480-128-32) */
			REG_03_DEST_Y = (((rand32>>8)&0x3f00)); 	/* (64) */
			#endif
			#if (1)
			/* �ړ����W�����߂� */
		//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* �U���A�j���[�V���� */
			boss_set_new_position(src);/*(�U���ړ����W��ݒ�)*/
			#endif
		}
	}
	/*(�ȉ��͏����I�ɋ��ʉ������\��)*/
	boss_move_00_card_settei_seisei(src);/*(�J�[�h�����I���Ȃ�J�[�h�ݒ肵�A�����B)*/
//r36���Ԃ�B	boss_re gacy_yuudou(src);
}
/* �o����x���W */
//#define BOSS_XP256		(t256(GAME_WIDTH/2))	/* ���S���W�Ȃ̂� */
#define BOSS_XP256			(t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH/2))	/* ���S���W�Ȃ̂� */

/*---------------------------------------------------------
	�Œ�ޔ��ʒu�ֈړ���A�����Ȃ��ŃJ�[�h�����B
	-------------------------------------------------------
	�Œ�ޔ��ʒu�͏㕔�AX���͒��S�B�Ƃ肠�����B
---------------------------------------------------------*/

global OBJ_CALL_FUNC(boss_move_01_taihi_ue_naka)
{
	#if (1)
	/* �ړ����W�����߂� */
//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* �U���A�j���[�V���� */
//	REG_02_DEST_X	= (src->BOSS_DATA_00_target_x256);/*(t256()�`��)*/
//	REG_03_DEST_Y	= (src->BOSS_DATA_01_target_y256);/*(t256()�`��)*/
	REG_02_DEST_X	= (BOSS_XP256);/*(t256()�`��)*/
	REG_03_DEST_Y	= (t256(16.0));/*(t256()�`��)*/
	boss_set_new_position(src);/*(�U���ړ����W��ݒ�)*/
	#endif
	card.boss_move_card_callback = boss_move_00_card_settei_seisei;/*(�J�[�h�����I���Ȃ�J�[�h�ݒ肵�A�����B)*/
}

#if 1
/*---------------------------------------------------------
	��11�`��: �ŏI�`��(����3)
---------------------------------------------------------*/

global OBJ_CALL_FUNC(boss_move_17_sakuya_bimyou_idou)
{
	u32 iii;
	iii = ra_nd();
	if (0==(iii&0x07))
	{
		src->math_vector.x256 = (((signed int)((iii>> 8)&0xff))-127);
		src->math_vector.y256 = (((signed int)((iii>>16)&0xff))-127);
	}
	/*(�ȉ��͏����I�ɋ��ʉ������\��)*/
	boss_move_00_card_settei_seisei(src);/*(�J�[�h�����I���Ȃ�J�[�h�ݒ肵�A�����B)*/
}
#endif

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

static OBJ_CALL_FUNC(boss_move_vx_vy_animation)
{
	src->center.x256 += (src->math_vector.x256);
	src->center.y256 += (src->math_vector.y256);
	#define USE_BOSS_HAMIDASI_CHECK (0)
	#if (1==USE_BOSS_HAMIDASI_CHECK)
		 if (src->center.x256 < rect_clip.boss_clip_min.x256 )	{	src->center.x256 = rect_clip.boss_clip_min.x256;	card.boss_hamidasi = (1);	}
	else if (src->center.x256 > rect_clip.boss_clip_max.x256 )	{	src->center.x256 = rect_clip.boss_clip_max.x256;	card.boss_hamidasi = (1);	}
//
		 if (src->center.y256 < rect_clip.boss_clip_min.y256 )	{	src->center.y256 = rect_clip.boss_clip_min.y256;	card.boss_hamidasi = (1);	}
	else if (src->center.y256 > rect_clip.boss_clip_max.y256 )	{	src->center.y256 = rect_clip.boss_clip_max.y256;	card.boss_hamidasi = (1);	}
	#else
	/*(�Ƃ肠����)*/
	src->center.x256 = psp_min(src->center.x256, rect_clip.boss_clip_max.x256);/* �E�`�F�b�N(��ݏo��) */
	src->center.x256 = psp_max(src->center.x256, rect_clip.boss_clip_min.x256);/* ���`�F�b�N(��ݏo��) */
	src->center.y256 = psp_min(src->center.y256, rect_clip.boss_clip_max.y256);/* ���`�F�b�N(��ݏo��) */
	src->center.y256 = psp_max(src->center.y256, rect_clip.boss_clip_min.y256);/* ��`�F�b�N(��ݏo��) */
	#endif
//
	/*(�U���A�j���[�V�����w��������΁A�U���A�j���[�V������D�悷��B)*/
	if (0 < src->BOSS_DATA_03_kougeki_anime_count)	/* �U���A�j���[�V���� */	/* �U���A�j���[�V�����p�J�E���^ / 0�ȉ��Ȃ�ړ��A�j���[�V���� */
	{
		src->BOSS_DATA_03_kougeki_anime_count--;
				if ((32)>src->BOSS_DATA_03_kougeki_anime_count) {	src->obj_type_set = BOSS_09_32; }	/*src->an im_frame = 0x23;*/
		else	if ((40)>src->BOSS_DATA_03_kougeki_anime_count) {	src->obj_type_set = BOSS_10_33; }	/*src->an im_frame = 0x22;*/
		else													{	src->obj_type_set = BOSS_09_32; }	/*src->an im_frame = 0x21;*/
	}
	/*(�U���A�j���[�V�����w�����Ȃ���΁A�ړ��A�j���[�V����������B)*/
	else	/* �ړ��A�j���[�V���� */
	{
		s32 aaa;
		aaa = (src->center.x256 > src->BOSS_DATA_00_target_x256)?(BOSS_00_11/*0x00*/):(BOSS_04_21/*0x10*/); /* ���E */
		s32 bbb;
		bbb = src->BOSS_DATA_04_toutatu_wariai256;
		bbb = psp_min(bbb, (0xff) );
		bbb = 0xff-bbb;
		bbb &= 0xff;
		src->obj_type_set = aaa + ((src->BOSS_DATA_04_toutatu_wariai256>>6) );
	}
}

/*---------------------------------------------------------
	�S�{�X�s��
	(boss->callback_mover)�ŌĂ΂�郋�[�g(����)�B
---------------------------------------------------------*/

global OBJ_CALL_FUNC(root_boss_mover)
{
	/*(�{�X�s���I������)*/
	if (NULL != src->callback_root_supeka)
	{
		(*src->callback_root_supeka)(src);
	}
//	card_boss_move_generate_check_regist(src);	/* �J�[�h�o�^�\�Ȃ�o�^ / �J�[�h���� */
	boss_move_vx_vy_animation(src);
	move_card_square_effect(src);				/* ��G�t�F�N�g */
	/*(�X�y����������)*/
	card_generate(src); 						/* �X�y����CPU���s���A�J�[�h���P�t���[����������B */
}
