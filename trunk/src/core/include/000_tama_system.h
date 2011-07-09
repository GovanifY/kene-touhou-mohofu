
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _TAMA_SYSTEM_H_
#define _TAMA_SYSTEM_H_

/*---------------------------------------------------------
	�e�V�X�e��
---------------------------------------------------------*/

/*---------------------------------------------------------
	�e���̎󂯓n���ݒ�(�p�x�e�̏ꍇ)
	-------------------------------------------------------
	�p�x�e�̕���\��65536�x�Ɍ���B
---------------------------------------------------------*/

typedef struct _bullet_regist_
{
	// �ȉ��́A �e�̐ݒ�B
	int BULLET_REGIST_00_speed256;					/* ���x */
	int BULLET_REGIST_01_speed_offset;				/* �����x */

	int BULLET_REGIST_02_angle65536;				/* �p�x */					/* <����\65536�x�Ɍ���> */
	int BULLET_REGIST_03_tama_data; 				/* ���p�x(�����e�p) */

	int BULLET_REGIST_04_bullet_obj_type;			/* �e�O���t�B�b�N�̎�� */
	// �ȉ��́A n_way�e�̏ꍇ�ɕK�v�ȗv�f�B
	int BULLET_REGIST_06_n_way; 					/* n way �e�̒e�� */
	int BULLET_REGIST_07_div_angle65536;			/* n way �e�̕����p�x */	/* <����\65536�x�Ɍ���> */
	// �ȉ��́A �e�����X�g�ɓo�^����ꍇ�̐ݒ�B
//	int BULLET_REGIST_start_number; 				/* �J�n�ԍ� */
//	int BULLET_REGIST_max_size; 					/* �T�C�Y */
//	int dummy_BULLET_REGIST_05_regist_type; 		/* �o�^����(�����̎��) */	/* �d�l�ύX(r33) */
} BULLET_REGIST;
extern BULLET_REGIST br;/* ���V�X�e����������p�� */

/*---------------------------------------------------------
	�e���̎󂯓n���ݒ�(�x�N�g���e�̏ꍇ)
	-------------------------------------------------------
	�� ����(r33)�x�N�g���e�̕���\��1024�x�Ȃ̂ŁA���ӁB
	�� 1024�x���Ɛ��x�s���Ȃ̂�65536�x�ɕύX���邩������Ȃ���
	���x�I�ȓs���Ō���(r33)����B
	�� ����(r33)���x�I�ȓs����1024�x�ɂȂ��Ă��邪�A
	���x�I�ȓs��������������65536�x�ɕύX�������B
---------------------------------------------------------*/

#define BULLET_REGIST_00_VECTOR_hosi_gata_time_out			BULLET_REGIST_00_speed256
#define BULLET_REGIST_02_VECTOR_hosi_gata_angle1024 		BULLET_REGIST_02_angle65536
#define BULLET_REGIST_03_VECTOR_regist_type 				BULLET_REGIST_03_tama_data
#define BULLET_REGIST_07_VECTOR_hosi_gata_add_speed256		BULLET_REGIST_07_div_angle65536

#define BULLET_REGIST_02_VECTOR_angle1024					BULLET_REGIST_02_angle65536
#define BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024	BULLET_REGIST_06_n_way
#define BULLET_REGIST_07_VECTOR_legacy_dan_delta256 		BULLET_REGIST_07_div_angle65536

#define BULLET_REGIST_07_VECTOR_div_angle1024				BULLET_REGIST_07_div_angle65536


#define USE_HAZUMI (0)

#if (1==USE_HAZUMI)
	/*
		0==BULLET_REGIST_06_VECTOR_HANERU_KAISUU				�d�͒e
		0!=BULLET_REGIST_06_VECTOR_HANERU_KAISUU				�͂��ݒe(�P���p)
	*/
	#define VECTOR_TYPE_000_HANE_NAI							(0)/* �d�͒e */
	#define BULLET_REGIST_06_VECTOR_HANERU_KAISUU				BULLET_REGIST_06_n_way
#endif /* (1==USE_HAZUMI) */

/*---------------------------------------------------------
	�o�^���(�p�x�e�̏ꍇ)
---------------------------------------------------------*/

extern void tama_system_regist_single(void);			/* �P�� */
extern void tama_system_regist_katayori_n_way(void);	/* �΂� n way�e */
extern void tama_system_regist_n_way(void); 			/* �ʏ� n way�e */

/*---------------------------------------------------------
	�o�^���(�x�N�g���e�̏ꍇ)
	-------------------------------------------------------
	�x�N�g���e�͊p�x�e�Ɠ�������ׂɁA�ڍs���ł��B
---------------------------------------------------------*/

/* �x�N�g������e(���A�ڍs�`��) */
extern void bullet_regist_legacy_vector_direct(void);
extern void bullet_regist_legacy_vector_send1_xy_src(SPRITE *src);

/* �x�N�g���������e(���A�ڍs�`��) */
extern void bullet_regist_multi_vector_direct(void);
extern void bullet_regist_multi_vector_send1_xy_src(SPRITE *src);

/*---------------------------------------------------------
	�x�N�g������e�̎�ށA�p�~�\��(���݊�)
	-------------------------------------------------------
	����e�͏����I�ɒe���X�N���v�g�ֈڍs�����ꍇ�ɁA
	�e���X�N���v�g�ŋL�q�ł��Ȃ��̂ŁA�X�y�J�Ɉڍs���Ċ��S�ɖ����Ȃ�܂��B
	�������A�X�y�J�ɂȂ�Ȃ��U�R����e�͎c�邩������܂���B(���x����)
---------------------------------------------------------*/

enum/*_tama_data_*/
{
	LEGACY_REGIST_TYPE_00_HANERU,			/* �d�͒e / �P��A�ŏI�`�Ԃœ����Ă��邩�Ȃ薳���Ȓe�B */
	LEGACY_REGIST_TYPE_01_TOMARI,			/* �~�܂�e */
	LEGACY_REGIST_TYPE_99_MAX,
};
// (r34)�p�~��: 	VECTOR_REGIST_TY PE_00_MULTI_VECTOR,	/* �������e */
// (r33)�p�~�ς�:	REGIST_TY PE_04_KURU_KURU,				/* ���邭��e */

/*---------------------------------------------------------
	�p�x�e�K�i(����āA���^�p���B)
	��_���W�֘A�͓���@�\�ɐU���ĕύX���邩���m��Ȃ��B
---------------------------------------------------------*/

	#define tama_system_speed65536				user_data00 	/* ������ */	//	#define tama_system_speed256				user_data02 	/* ������ */
	#define tama_system_tra65536				user_data01 	/* ���������� */
	#define tama_system_tama_data				user_data02 	/* ��ʊO��������┽�ˋ@�\ */
	#define tama_system_flame_counter			user_data03 	/* ���e�t���[���J�E���^(���l�Ŕ��e) */
//	#define tama_system_aaa 					user_data04 	/* �g���\��(?) (r33���ݖ��g�p) / tama_system_add_rot ate1024 ���p�x�A��]�p�x����(�����e�p) t256�`��() */
//	#define tama_system_bbb 					user_data05 	/* �g���\��(?) (r33���ݖ��g�p) / */
//	#define tama_system_ccc 					user_data06 	/* �g���\��(?) (r33���ݖ��g�p) / */
//(r33) #define tama_system_radius256				user_data07 	/* ���a */
//�p�~	#define tama_system_kyori256				user_data07 	/* 1[f]������ɐi�ދ����B(���a) */

#if 1/* �p�x�e�K�iB(�������) */
	#define TAMA_DATA_0000_TILT 				(0x0000)/* �X���e(�ʏ�e) */
	#define TAMA_DATA_8000_NON_TILT 			(0x8000)/* ��X���e */
	/* (��) */
	#define TAMA_DATA_SAYUU_HANSYA_BIT			(0x0001)
	#define TAMA_DATA_RUMIA_SAYUU_BIT			(0x0001)
	#define TAMA_DATA_RUMIA_DIMMER_RENDAN_BIT	(0x0002)
#endif

#if 0
enum
{
	TAMA_TYPE_00_ANGLE_TILT = 0,	/* �ʏ�e */
	TAMA_TYPE_01_ANGLE_NON_TILT,	/* �X���Ȃ��e(��ʒe�A�`���m�e�A���p) */
//	TAMA_TYPE_02_,					/* �e */
//	TAMA_TYPE_03_,					/* �e */
	/* ... */
	TAMA_TYPE_99_MAX/* �ő�l */
};
#else
	#define dummy_r33(aaa) ;
#endif




#endif /* _TAMA_SYSTEM_H_ */

/*---------------------------------------------------------
	���[�U�[�V�X�e��
---------------------------------------------------------*/

#ifndef _LASER_SYSTEM_H_
#define _LASER_SYSTEM_H_

/*
	�p�`�F�u�ʏ�U���v
	�t�����֊��u���[���@�e�C���v
	�t�����֒e�u�ߋ������ގ��v�v
	�����̃X�y�J����邩�ǂ����͕ʖ��Ƃ��āA�����̃��[�U�[�e��
	�V�X�e���őΉ��ł���悤�ɐ݌v���Ƃ��K�v������B
*/
enum
{
	LASER_TYPE_00_OFF		= 0,//	LASER_TYPE_00_1_PACHE	= 0,	// 0==off, lz[0]==off�̏ꍇ�A����@�\�őS��off�B
	LASER_TYPE_01_4_PACHE,	/* 4�{�A�O���^�C�v(�p�`�F�u�ʏ�U���v�p) */
	LASER_TYPE_02_1_FRAN,	/* 1�{�A�^�C�v	  (�t�����֊��u���[���@�e�C���v�p) */
	LASER_TYPE_03_4_FRAN,	/* 4�{�A�����^�C�v(�t�����֒e�u�ߋ������ގ��v�v�p) */
	MAX_99_LASER_TYPE
};
enum
{
	LASER_00_ = 0,
	LASER_01_,
	MAX_99_LASER
	/*	2�e�H==�֊��u���[���@�e�C���v(�u�n�߁v�Ɓu�Ōォ�^��������v�̂ݔ��肪����΁A�c���S���ɔ��肪�Ȃ��Ă��債�ĕς��Ȃ��C������)
		8�e�H==�֊��u���[���@�e�C���v
		�Ō�łȂ��āA�^��������ɂ���͓̂�Փx�𒲐�����ׁB(�Ō�̕����኱���)
		�������Ȃ̂ŁA���ۂ́u���肬��œ������Ă�v�̂����ǁA���̏ꍇ�������ĂȂ����ɂ��������u���肬��œ�����Ȃ������v�Ƃ����z�B
		�Q�{�v�闝�R�́A�����ړ��L�����ŁA1�{�ڂł��蔲���Ă��A�Q�{�ڂɓ�����B
	 */
};
typedef struct _laser_
{
	int laser_type; 		/* ���[�U�[�̎�� / ����off�w��. LASER_TYPE */
	int laser_angle65536;	/* ���[�U�[�̊p�x */
} LASER;
extern LASER lz[MAX_99_LASER];
/* lz[0]==0;�̏ꍇ�A���ׂ�off�B(���[�U�[���[�h 0:off, 1:on) */

#endif /* _LASER_SYSTEM_H_ */
