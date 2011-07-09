
#include "game_main.h"

/*---------------------------------------------------------
  �����͕핗 �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�e�V�X�e��
	-------------------------------------------------------
	(r33)�������ėp�I�Ȓe���˃V�X�e�����\�z�������Ă݂�B
	�� bullet_angle.c ��
	�� bullet_vector.c ��
	�����ɓ��������\��B
	���[�U�[�֘A�͓�������Ȃ��ŁA�ʃV�X�e���ɂȂ�\��B
	-------------------------------------------------------
	(r32-)��ݏo���`�F�b�N�ɂ��āA
	�u�ړ����Ɂv��ݏo���`�F�b�N�����Ȃ������O��Ő݌v����Ă���e���������B
	���ׁ̈u�ړ����Ɂv�V�X�e���ŋ�ݏo���`�F�b�N�����܂���B
	�Ⴆ�Ή�ʒ[�Œ��˕Ԃ�e���́u�ړ����Ɂv��ݏo���`�F�b�N�����Ȃ����ɂ��������Ă���@�\�ł��B
	�u�ړ��ƕʂŁv�e�����X�g�𒲂ׂāu��ݏo���`�F�b�N�v���s���܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�����e�͐F�X�e�ɂ���ĈႢ����������B(�X�̎��ԂƂ�)
	�S���V�X�e�����ŋz������ƁA�x���Ȃ�C������B
	�����e�V�X�e�����ŋz������Ȃ�A�p�x�e�Ɠ������Ċp�x�e�������ׂ�
	������(�����͂Ƃ�����)����ł͊p�x�e�ėp�ɂ���ƒx������B
	-------------------------------------------------------
	����̑��x�Ȃ�A�����e�͒e�����X�g���������őΉ����ׂ��B
	(�p�`�F�Ƃ����������̗��ɂȂ肻���ł����B)
	�V�X�e���������e�Ή��́A�����Ȃ肻���B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�p�x�e�A��{��`
	-------------------------------------------------------
	�p�x�e�ł́A�x�N�g���ړ������Ȃ��B
	����Ɋ�_���W�Ƃ��Ďg���B
---------------------------------------------------------*/

#if 1/* �p�x�e�K�i(�����tama_system) */
	#define ox256				vx256/* ���e�ʒu ���Wx */
	#define oy256				vy256/* ���e�ʒu ���Wy */
#endif


/*---------------------------------------------------------
	�p�x�e
	-------------------------------------------------------
	�p�x�e�̈ړ����s���B
---------------------------------------------------------*/

#define tama_system_kousadan_angle65536 tmp_angleCCW1024

/*---------------------------------------------------------
	#001 �p�x�e�̈ړ����s��(�ʏ�e�p)
---------------------------------------------------------*/

static void tama_system_move_angle_001(SPRITE *src)
{
	#if 1/* ������(�����Ă��o���邯�ǁA�����������ȒP) */
	src->tama_system_speed65536 		+= (src->tama_system_tra65536); 		/* ���������� */
	#endif
	/*( 1[flame]������ɐi�ދ����B(���a) )*/int aaa 		 = (src->tama_system_speed65536>>8);	/* ���x */
//
	src->cx256 += ((sin1024((src->rotationCCW1024))*(aaa))>>8);/*fps_factor*/
	src->cy256 += ((cos1024((src->rotationCCW1024))*(aaa))>>8);/*fps_factor*/
}


/*---------------------------------------------------------
	���e�G�t�F�N�g
	-------------------------------------------------------
	�X���Ȃ��e / �X���e(�ʏ�)
---------------------------------------------------------*/

static void move_bullet_hatsudan_effect(SPRITE *src)
{
//	tama_system_common_hatudan_000(src);
	{
		int aaabbb = (src->tama_system_flame_counter);
		src->cx256 = (src->ox256) + ((sin1024((src->rotationCCW1024))*(aaabbb)) );/*fps_factor*/
		src->cy256 = (src->oy256) + ((cos1024((src->rotationCCW1024))*(aaabbb)) );/*fps_factor*/
		src->color32		= ((256-aaabbb)<<(24))|0x00ffffff;
		src->m_zoom_x256	= t256(1.0) + (aaabbb) + (aaabbb);
		src->m_zoom_y256	= t256(1.0) + (aaabbb) + (aaabbb);
	}
	src->tama_system_flame_counter -= (4);
	if (0 > src->tama_system_flame_counter)
	{
	//	src->tama_system_flame_counter = 0;
		src->flags |= (SP_FLAG_COLISION_CHECK);/* �����蔻��L�� */
		src->callback_mover 			= tama_system_move_angle_001;
		if (src->tama_system_tama_data & TAMA_DATA_8000_NON_TILT)/* ��X���e */
		{
			src->m_zoom_y256 = M_ZOOM_Y256_NO_TILT;/* ����@�\�ŌX���Ȃ��悤�V�X�e���g��(r33)�B */
		}
	}
}


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
		�{�X���X�y�J�V�X�e���Ɉڍs�o���ĂȂ��̂Ŗ��ʂȏ���������B
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

global void tama_system_regist_single(void)
{
	SPRITE		*h;
	h									= obj_add_00_tama_error();
	if (NULL != h)
	{
		h->ox256						= (obj_send1->cx256);/* ���e�ʒu ���Wx */
		h->oy256						= (obj_send1->cy256);/* ���e�ʒu ���Wy */
		h->type 						= (br.BULLET_REGIST_04_bullet_obj_type);
		h->flags &= ~(SP_FLAG_COLISION_CHECK);/* �����蔻�薳��(���e�G�t�F�N�g�p) */
		reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
	//
		h->callback_mover						= move_bullet_hatsudan_effect;
	//
		h->tama_system_kousadan_angle65536		= (br.BULLET_REGIST_02_angle65536); /* �����e�p */
		h->rotationCCW1024						= ((br.BULLET_REGIST_02_angle65536)>>6);/* (i<<4) deg_360_to_512(90) */
		h->m_Hit256R							= TAMA_ATARI_JIPPOU32_PNG;
	//
	//	h->tama_system_speed256 				= ((br.BULLET_REGIST_00_speed256)	 ); 	/* ���x */	/* ����(�ł��o�����x) */
		h->tama_system_speed65536				= ((br.BULLET_REGIST_00_speed256)<<8);		/* ���x */	/* ����(�ł��o�����x) */
		h->tama_system_flame_counter			= (0xff);//(0);//(-(0x100));
//		h->tama_system_base_time_out			= (120);/*(100)*/
	//	h->tama_system_kyori256 				= t256(0);/* ���a */
//		h->tama_system_tra65536 				= ((br.BU LLET_REGIST_speed_offset)<<8);	/* t256�`���ŁB ���������e */	/* ���̕����ɂȂ邩������ */
		h->tama_system_tra65536 				= ((br.BULLET_REGIST_01_speed_offset)); 	/* t256�`���ŁB ���������e */	/* ���̕����ɂȂ邩������ */
		h->tama_system_tama_data				= ((br.BULLET_REGIST_03_tama_data));		/* �p�x�e�K�iB(�������)����@�\ ���̕����ɂȂ邩������ */
		//
	//	h->tama_system_add_rotate1024			= ((br.BULLET_REGIST_angle_offset1024)>>(8-6)); 	/* t256�`���ŁB �����e */		/* ���̕����ɂȂ邩������ */
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
	br.BULLET_REGIST_02_angle65536	[�j��]
---------------------------------------------------------*/

global void tama_system_regist_katayori_n_way(void)
{
	int i_angle65536;
	i_angle65536	= (br.BULLET_REGIST_02_angle65536);/* �n�߂̊J�n�p�x�A���� */
	int i;
	for (i=(0); i<(br.BULLET_REGIST_06_n_way); i++)/* n way�e��o�^����B */
	{
		br.BULLET_REGIST_02_angle65536 = i_angle65536;
		tama_system_regist_single();
		/* ���̊p�x */
		i_angle65536 += (br.BULLET_REGIST_07_div_angle65536);
		mask65536(i_angle65536);
	}
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
	br.BULLET_REGIST_02_angle65536		[�j��]
	br.BULLET_REGIST_06_n_way			[�j��]
	br.BULLET_REGIST_07_div_angle65536	[�j��]
*/
global void tama_system_regist_n_way(void)
{
	const int j_angle65536	= (br.BULLET_REGIST_02_angle65536);/* �J�n���� */
	const int j_n_way		= (br.BULLET_REGIST_06_n_way)&0xff;/* �e�� */
	const int j_n_way_harf	= (j_n_way>>1)&0xff;/* �e�� */
	//
	{
		/* �����e�̏ꍇ�̍����𐶐�
			�����e�̏ꍇ�A�����p�x�̔����̊p�x���I�t�Z�b�g�p�x�B
			��e�̏ꍇ�I�t�Z�b�g�p�x�͕K���[���B
		*/
	//	const int j_guusuu_harf_angle_offset = ((br.BULLET_REGIST_07_div_angle65536)>>1);	/* �����e�Ȃ番���p�̔����������A��e�͂Ȃ� */
		const int j_guusuu_harf_angle_offset = (0==(j_n_way&1)) ? ((br.BULLET_REGIST_07_div_angle65536)>>1) : (0);	/* �����e�Ȃ番���p�̔����������A��e�͂Ȃ� */

		/* �E����(���������l�̏ꍇ) */
		br.BULLET_REGIST_02_angle65536 = j_angle65536 + j_guusuu_harf_angle_offset;/* �����e & ��e */
		br.BULLET_REGIST_06_n_way		= (j_n_way - (j_n_way_harf));	/* �Ⴆ�� 7way �Ȃ� 4�e�`���B 8way �Ȃ� 4�e�`���B */
		tama_system_regist_katayori_n_way();
		#if 0/* �f�o�b�O�p */
			br.BULLET_REGIST_04_bullet_obj_type ^= 2;/* �f�o�b�O�p: �����O���[�v�����ǁA���E�ňႤ�F�̒e�ɂ��Ă݂�B */
		#endif
		/* �����p�x���t�ɂ���B(�t����ɕ`��way�e�ɂȂ�) */
		br.BULLET_REGIST_07_div_angle65536 = (65536)-(br.BULLET_REGIST_07_div_angle65536);
		#if 1
		br.BULLET_REGIST_02_angle65536 = j_angle65536 - j_guusuu_harf_angle_offset;/* �����e & ��e */
		/* ������(���������l�̏ꍇ) */
		if (0==(j_n_way&1))/* �����e */
		{;/* �Ȃɂ����Ȃ� */}
		else/* ��e */
		{
			br.BULLET_REGIST_02_angle65536 += br.BULLET_REGIST_07_div_angle65536;/* ��e(��1�e���Ȃ��̂ł��炷) */
		}
	//	br.BULLET_REGIST_02_angle65536 = j_angle65536 + ((0==(j_n_way&1))?(-j_guusuu_harf_angle_offset):(br.BULLET_REGIST_07_div_angle65536));
		br.BULLET_REGIST_06_n_way		= ( 		 (j_n_way_harf));	/* �Ⴆ�� 7way �Ȃ� 3�e�`���B 8way �Ȃ� 4�e�`���B */
		tama_system_regist_katayori_n_way();
		#endif
	}
}
