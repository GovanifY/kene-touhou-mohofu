
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���e�V�X�e��
	-------------------------------------------------------
	�G�����e����ꍇ�́A���e�G�t�F�N�g�������Ă������Ă��A
	�K�����e�V�X�e�����o�R���Ĕ��e����B
	-------------------------------------------------------
	���[�U�[�֘A�̔��e�͓�������Ȃ��ŁA�ʃV�X�e���ɂȂ�\��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�p�x�e�A��{��`
	-------------------------------------------------------
	�p�x�e�ł́A�x�N�g���ړ������Ȃ��B
	����Ɋ�_���W�Ƃ��Ďg���B
	base��_�Aorigin==���_�B
---------------------------------------------------------*/
#define origin									math_vector/*(���e�ʒu ���W)*/
#define hatudan_system_kousadan_angle65536		tmp_angleCCW1024

//	void (*aaa[4/*8*/])(OBJ/**/ *bbb) =
//	{
//	/* �ړ��������� */	move_bullet_hatsudan_effect,/* �G�t�F�N�g��(��r34�݊�) */
//	/* �ړ��������� */	move_bullet_hatsudan_effect,/* �G�t�F�N�g�� */
//	/* �ړ��������� */	move_bullet_hatsudan_effect,/* �G�t�F�N�g�� */
//	/* �ړ��������� */	NULL,//hatudan_system_move_angle_001,/* �G�t�F�N�g�Ȃ� */
//	/* �ړ������Ȃ� */	move_bullet_hatsudan_effect,/* �G�t�F�N�g��(��r34�݊�) */
//	/* �ړ������Ȃ� */	move_bullet_hatsudan_effect,/* �G�t�F�N�g�� */
//	/* �ړ������Ȃ� */	move_bullet_hatsudan_effect,/* �G�t�F�N�g�� */
//	/* �ړ������Ȃ� */	NULL,//hatudan_system_move_dummy_000,/* �G�t�F�N�g�Ȃ� */
//	};
//	src->callback_mover 	= aaa[((src->hatudan_register_spec_data)>>12)&0x07];
//	src->callback_mover 	= aaa[((src->hatudan_register_spec_data)>>12)&0x03];

/*---------------------------------------------------------
	�ړ������I���B
---------------------------------------------------------*/
static OBJ_CALL_FUNC(move_bullet_hatsudan_effect);
static OBJ_CALL_FUNC(set_mover)
{
	if (0x03==(((src->hatudan_register_spec_data)>>12)&0x03))
	{	/* �G�t�F�N�g�Ȃ�==���e�J�n���� */
		src->callback_mover 	= NULL;
		{
			/* ���e�J�n���O�ɂ����蔻���L���ɂ���B */
			/* ��X���e�́A���e�J�n���O�ɐݒ�B */
			src->atari_hantei	= (ATARI_HANTEI_TAOSENAI/*�X�R�A���p*/);/* �����蔻��L�� */
			if (src->hatudan_register_spec_data & TAMA_SPEC_8000_NON_TILT)/* ��X���e */
			{
				src->m_zoom.y256 = M_ZOOM_Y256_NO_TILT;/* ����@�\�ŌX���Ȃ��悤�V�X�e���g��(r33)�B */
			}
			#if (1)/*(���e�G�t�F�N�g�������ꍇ�ɍ���)*/
			src->center.x256 = (src->origin.x256);/*fps_factor*/
			src->center.y256 = (src->origin.y256);/*fps_factor*/
			#endif
		}
	}
	else/* �G�t�F�N�g����==(���e�G�t�F�N�g����)*/
	{
		src->callback_mover 	= move_bullet_hatsudan_effect;
	}
}
/*---------------------------------------------------------
	���e�G�t�F�N�g
	-------------------------------------------------------
	�X���Ȃ��e / �X���e(�ʏ�)
---------------------------------------------------------*/
static OBJ_CALL_FUNC(set_mover);
static OBJ_CALL_FUNC(move_bullet_hatsudan_effect)
{
//	hatudan_system_common_hatudan_000(src);
	{
		const int aaabbb = (src->hatudan_register_frame_counter);
		#if (0)
		src->center.x256 = (src->origin.x256) + ((si n1024((src->rotationCCW1024))*(aaabbb)) );/*fps_factor*/
		src->center.y256 = (src->origin.y256) + ((co s1024((src->rotationCCW1024))*(aaabbb)) );/*fps_factor*/
		#else
		{
			int sin_value_t256; 	// sin_value_t256 = 0;
			int cos_value_t256; 	// cos_value_t256 = 0;
			int256_sincos1024( (src->rotationCCW1024), &sin_value_t256, &cos_value_t256);
			src->center.x256 = (src->origin.x256) + ((sin_value_t256*(aaabbb))>>(((src->hatudan_register_spec_data)>>12)&0x03));/*fps_factor*/
			src->center.y256 = (src->origin.y256) + ((cos_value_t256*(aaabbb))>>(((src->hatudan_register_spec_data)>>12)&0x03));/*fps_factor*/
		}
		#endif
		src->color32		= ((256-aaabbb)<<(24))|0x00ffffff;
		src->m_zoom.x256	= t256(1.0) + (aaabbb) + (aaabbb);
		src->m_zoom.y256	= t256(1.0) + (aaabbb) + (aaabbb);
	}
	src->hatudan_register_frame_counter -= (4);
	if (0 > src->hatudan_register_frame_counter)
	{
		src->hatudan_register_frame_counter = (0);/*(���̌�Ŏg���Ȃ�)*/
		/*(�G�t�F�N�g�Ȃ� �ɐݒ肷��B)*/
		src->hatudan_register_spec_data |= (TAMA_SPEC_3000_EFFECT_NONE);
		set_mover(src);
	}
}
//		if (0==(src->hatudan_register_spec_data & TAMA_SPEC_4000_NON_MOVE))
//		{
//			src->callback_mover 			= hatudan_system_move_angle_001;/*(�G�����ł͖�薳�����A�e���ɂ͒x������)*/
//		}
//		else	/* �{�Ƃ͋��炭���ʈړ������͂������������B */
//		{
//		//	src->callback_mover 			= NULL;/*(�e���������Ɉړ�����������̂ŁAOBJ�ňړ������͕s�v)*/
//			src->callback_mover 			= hatudan_system_move_dummy_000;/*(�e���������Ɉړ�����������̂ŁAOBJ�ňړ������͕s�v)*/
//		}

/*---------------------------------------------------------
	�e��o�^����B
	-------------------------------------------------------
	�e�������̃G�t�F�N�g���̊֌W��A���e�����͑S���Z�߂�K�v������B
	-------------------------------------------------------
	�x�N�g���e�Ɗp�x�e�𓝍����邩������Ȃ��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�W���̊p�x�e�𕡐�(n way�e)�o�^����B
	-------------------------------------------------------
	�e�̔��e�����́A1�����ɓZ�߂�\��Ȃ̂����A
	���ݎd�l�����܂��Ă��Ȃ��B
	�e�̔��e���A1�����ɓZ�܂��Ă��Ȃ��ƁA���e�̃G�t�F�N�g�����Ȃ��B
	�u���e�̃G�t�F�N�g�����Ȃ��v�Ƃ����̂́A
	�e�̔��e���A1�����ɓZ�܂��Ă��Ȃ��̂ɋ����Ɂu���ƒx���Ȃ�v�����肽���Ȃ��Ƃ����Ӗ��B
	����(r33)�����x���l�b�N(�x��)�ɂȂ��Ă���̂ŁA�����������͂��Ȃ��B�Ƃ������o���Ȃ��B
	�������1�����ɓZ�܂��đ��x�������ł����甭�e�̃G�t�F�N�g�����\��B
	����(r33)�̑��x�ቺ�́A
		�T�C�h�̃p�l����SDL�`��B
		��b��SDL�`��B
		Gu��]�`��ŉ�slice���ĂȂ��B
		�{�X���J�[�h�V�X�e���Ɉڍs�o���ĂȂ��̂Ŗ��ʂȏ���������B
	�����肪�A��ȏ��������B
	-------------------------------------------------------
	�����܂Ŏb��d�l(r32)
	-------------------------------------------------------
	BULLET_REGIST_06_n_way: 		n way�e�̑��e��
		��x�ɒe�������B1���������Ȃ��Ȃ�1�B
	BULLET_REGIST_02_angle65536:		�J�n����
		�P����65536���������l(0 ... 65535)��nway�e�̔��e�J�n�p�x���w�肷��B
		��������0�x�Ŕ����v���B(��CCW)
	BULLET_REGIST_04_bullet_obj_type:	�e�̎��(��̓I�ɂ̓O��)�̔ԍ�

	BULLET_REGIST_00_speed256:		����(�ł��o�����x)

	BULLET_REGIST_07_div_angle65536:	n way�e�̕����p�x

	BULLET_REGIST_01_speed_offset:	�����x(���͌����x)

---------------------------------------------------------*/

/*---------------------------------------------------------
	�e�V�X�e��:
	��{�e����
	-------------------------------------------------------
	�d�l�F �����͔j�󂵂Ȃ��B
---------------------------------------------------------*/

global void hatudan_system_regist_single(void)
{
	OBJ 	*h;
	h											= obj_regist_tama();/* ���e�o�^ */
	if (NULL != h)/* �o�^�ł����ꍇ */
	{
		h->origin.x256							= REG_02_DEST_X;/*(���e�ʒu ���Wx)*/
		h->origin.y256							= REG_03_DEST_Y;/*(���e�ʒu ���Wy)*/
		h->obj_type_set 						= (HATSUDAN_05_bullet_obj_type);
		h->atari_hantei 						= (ATARI_HANTEI_OFF/*�X�R�A���p*/);/* �����蔻�薳��(���e�G�t�F�N�g�p) */
		reflect_sprite_spec(h, OBJ_BANK_SIZE_00_TAMA);
	//
		h->hatudan_system_kousadan_angle65536	= (HATSUDAN_03_angle65536); 				/* �����e�p */
		h->rotationCCW1024						= (deg65536to1024(HATSUDAN_03_angle65536)); /* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */	/* (i<<4) deg_360_to_512(90) */
		h->m_Hit256R							= TAMA_ATARI_JIPPOU32_PNG;
	//
		h->hatudan_register_speed65536			= ((HATSUDAN_01_speed256)<<8);		/* t65536�`���ŁB ���x */	/* ����(�ł��o�����x) */
		h->hatudan_register_frame_counter		= (0xff);
		h->hatudan_register_tra65536			= (HATSUDAN_02_speed_offset);		/* t65536�`���ŁB ���������e */ 	/* ���̕����ɂȂ邩������ */
		h->hatudan_register_spec_data			= (HATSUDAN_04_tama_spec);			/* �e�̊�{�����\�͂�ݒ肷��B */
		set_mover(h);
	}
}


/*---------------------------------------------------------
	�e�V�X�e��:
	�΂� n way �e
	�g������ n way �e�V�X�e���́A�ǂ��������������̂��ۂ��B
	�� 5�ʃ{�X���́u�ʏ�U��1�v�y�сu�ʏ�U��2�v��
	��邪�������������w�́A 5way �Ƃ� 6way �Ƃ� �e�������Ă��邪�A
	��C�ɕ`���Ȃ��ŁA���E��2��ɂ����ĕ`���B
	�܂荶�E�Ńv���C�I���e�B�[(�e�̏d�Ȃ�)���Ⴄ�B

	�� 5�ʒ�-�{�X���́u�ʏ�U���v��(�i�C�t�A�ԃN�i�C����)���E���Œ�̌Œ�e�Ȃ̂ŁA
	�������� n way �e���Ɠs�����ǂ��B(����̂Ȃ��������Č�����̂��s�����ǂ�)
	�� 5�ʒ�-�{�X���́u�~�X�f�B���N�V�����v���A���m�Ȏ��@�_�����ۏ؂����̂ŁA
	�������� n way �e���Ɠs�����ǂ��B
	-------------------------------------------------------
	�ȉ��͊�{�����Ō��Ă�̂��A����Ƃ��኱���ς������̂Ȃ̂��s��(���Ԃ��{�����Ō��Ă�)

	�� 5�ʃ{�X���́u�ʏ�U��1�v�̃s���N�̃i�C�t�͊�{�����Ō��Ă�̂�
	����Ƃ���{�������኱���ς������̂��͔���Ȃ����w�Ǔ������́B

	�� ���[�~�A�̍g�����́u�i�C�g�o�[�h�v�͊�{�����Ō��Ă�̂�
	����Ƃ���{�������኱���ς������̂��͔���Ȃ����w�Ǔ������́B
	-------------------------------------------------------
	�d�l�F ���L�̈�����j�󂷂�̂Œ���
	HATSUDAN_03_angle65536	[�j��]
---------------------------------------------------------*/

global void hatudan_system_regist_katayori_n_way(void)
{
	#if (0)/*[�����L���ɂ���΁A���Ԃ񕜋��\]*/
	const int j_angle65536	= (HATSUDAN_03_angle65536);/* �n�߂̊J�n�p�x�A���� */
	#endif
	{
		int i;
		for (i=(0); i<(HATSUDAN_06_n_way); i++)/* n way�e��o�^����B */
		{
			hatudan_system_regist_single();
			/* ���̊p�x */
			HATSUDAN_03_angle65536 += (HATSUDAN_07_div_angle65536);
		//	mask65536(HATSUDAN_03_angle65536);���Ԃ�v��Ȃ��B
		}
	}
	#if (0)/*[�����L���ɂ���΁A���Ԃ񕜋��\]*/
	HATSUDAN_03_angle65536		= (j_angle65536);/*[����]*/
	#endif
}
/*
	�e�V�X�e��:
	n way �e
	n way �e�́A��C�ɐ������Ȃ��ŁA�΂� n way �e��2��ĂԎ��Ő�������B
	����͊�e�̏ꍇ�̑_���덷���ŏ��ɂ���ׂɕK�v�ȑ[�u�B
	��e�̐ώZ�덷���傫���Ǝ��@�_���e�̂���Ő������Ă�
	���e�������̐ώZ�덷�Ŋp�x�������̂ŁA���@��_��Ȃ��Ȃ��Ă��܂��B
	�v���C�I���e�B�[(�e�̏d�Ȃ�)�𒲂ׂ�ƌ���ł����̕������̗p���Ă��鎖���킩��B
	����ł͒e�̐������Ƀv���C�I���e�B�[(Z�o�b�t�@�Ȃ̂�Z�l)�����߂Ă���B
	-------------------------------------------------------
	�d�l�F ���L�̈�����j�󂷂�̂Œ���
	HATSUDAN_03_angle65536		[�j��]
	HATSUDAN_06_n_way			[�j��]
	HATSUDAN_07_div_angle65536	[�j��]
*/
global void hatudan_system_regist_n_way(void)
{
	const int j_angle65536	= (HATSUDAN_03_angle65536);/* �J�n���� */
	const int j_n_way		= (HATSUDAN_06_n_way)&0xff;/* �e�� */
	const int j_n_way_harf	= (j_n_way>>1)&0xff;/* �e�� */
	//
	{
		/* �����e�̏ꍇ�̍����𐶐�
			�����e�̏ꍇ�A�����p�x�̔����̊p�x���I�t�Z�b�g�p�x�B
			��e�̏ꍇ�I�t�Z�b�g�p�x�͕K���[���B
		*/
	//	const int j_guusuu_harf_angle_offset = ((HATSUDAN_07_div_angle65536)>>1);	/* �����e�Ȃ番���p�̔����������A��e�͂Ȃ� */
		const int j_guusuu_harf_angle_offset = (0==(j_n_way&1)) ? ((HATSUDAN_07_div_angle65536)>>1) : (0);	/* �����e�Ȃ番���p�̔����������A��e�͂Ȃ� */
		/*(�E�����𐶐�)*/
		/* �E����(���������l�̏ꍇ) */
		HATSUDAN_03_angle65536		= j_angle65536 + j_guusuu_harf_angle_offset;/* �����e & ��e */
		HATSUDAN_06_n_way			= (j_n_way - (j_n_way_harf));	/* �Ⴆ�� 7way �Ȃ� 4�e�`���B 8way �Ȃ� 4�e�`���B */
		hatudan_system_regist_katayori_n_way();
		/*(�������𐶐�)*/
		#if 0/* �f�o�b�O�p */
			HATSUDAN_05_bullet_obj_type ^= 2;/* �f�o�b�O�p: �����O���[�v�����ǁA���E�ňႤ�F�̒e�ɂ��Ă݂�B */
		#endif
		/* �����p�x���t�ɂ���B(�t����ɕ`��way�e�ɂȂ�) */
		HATSUDAN_07_div_angle65536	= (65536)-(HATSUDAN_07_div_angle65536);
		#if 1
		HATSUDAN_03_angle65536		= j_angle65536 - j_guusuu_harf_angle_offset;/* �����e & ��e */
		/* ������(���������l�̏ꍇ) */
		if (0!=(j_n_way&1))/* ��e */
		{
			HATSUDAN_03_angle65536 += HATSUDAN_07_div_angle65536;/* ��e(��1�e���Ȃ��̂ł��炷) */
		}
	//	else/* �����e */
	//	{;/* �Ȃɂ����Ȃ� */}
	//	HATSUDAN_03_angle65536		= j_angle65536 + ((0==(j_n_way&1))?(-j_guusuu_harf_angle_offset):(HATSUDAN_07_div_angle65536));
		HATSUDAN_06_n_way			= ( 		 (j_n_way_harf));	/* �Ⴆ�� 7way �Ȃ� 3�e�`���B 8way �Ȃ� 4�e�`���B */
		hatudan_system_regist_katayori_n_way();
		#endif
	}
	#if (0)/*[�����L���ɂ���΁A���Ԃ񕜋��\]*/
	HATSUDAN_03_angle65536		= (j_angle65536);/*[����]*/
	HATSUDAN_06_n_way			= (j_n_way);/*[����]*/
	HATSUDAN_07_div_angle65536	= (65536)-(HATSUDAN_07_div_angle65536);/*[���Ԃ񕜋�]*/
	#endif
}
