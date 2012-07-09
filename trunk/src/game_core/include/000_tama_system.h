
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _CARD_CPU_H_
#define _CARD_CPU_H_
/*---------------------------------------------------------
	�J�[�hCPU
---------------------------------------------------------*/

/*---------------------------------------------------------
	�J�[�hCPU�p�̃��W�X�^(���[�J���������[)
	-------------------------------------------------------
	�����I�Ɂu�J�[�h�v�́A�S�āu�J�[�h�X�N���v�g�v�ŏ�������p�Ɉڍs����(�\��)�B
	-------------------------------------------------------
	���́u�J�[�h�X�N���v�g�v�́A�u�J�[�hCPU
	(�J�[�h�X�N���v�g�����������j�b�g)
	(SPEll-CArd script, Central Prcessing Unit.)�v
	���A���ۂ̏������s���B
	-------------------------------------------------------
	���̃J�[�hCPU�ɂ́A���W�X�^(Resister)�ƌĂ΂��J�[�hCPU�̓���������������B
	�J�[�hCPU�����s������ꍇ(�܂�X�y���J�[�h�����ꍇ)�ɁA�J�[�hCPU�̓��W�X�^�ȊO�A�N�Z�X�ł��Ȃ��B
	-------------------------------------------------------
	�d�l:
		�S���W�X�^(reg00-reg1f)�́A�J�[�h�X�N���v�g�J�n���ɕK��(0)�ŏ��������B
		���ɁA��Փx���W�X�^�ɓ�Փx������B
		���̌�A�J�[�h�X�N���v�g�p��[�������Z�N�V����]�Ŏw�����ꂽ��������������x�����s����B
		([�������Z�N�V����]�ł͓�Փx���W�X�^���K���g�p�ł���)
		���e�����œ�Փx���W�X�^���g��Ȃ��ꍇ�́A�ėp���W�X�^�Ƃ��Ďg�p���Ēl��j�󂵂Ă��ǂ��B
		(�l��j�󂵂Ă����̃X�y�J�ł͍ď����������̂ŉe��������)
---------------------------------------------------------*/

enum
{
	// ���W�X�^(�S�Ă̌v�Z�͓������W�X�^0x00-0x0f�ł����o���Ȃ�)
	REG_NUM_00_SRC_X = (0), 		// 0�󂯓n�����W�X�^�B
	REG_NUM_01_SRC_Y,				// 1�󂯓n�����W�X�^�B
	REG_NUM_02_DEST_X,				// 2�󂯓n�����W�X�^�B
	REG_NUM_03_DEST_Y,				// 3�󂯓n�����W�X�^�B
	REG_NUM_04_CONDITION_CODE0, 	// CC0���W�X�^�BIF��LOOP���ňÖقɗ��p�B
	REG_NUM_05_CONDITION_CODE1, 	// CC1���W�X�^�BIF��LOOP���ňÖقɗ��p�B
	REG_NUM_06_CONDITION_CODE2, 	// CC2���W�X�^�BIF��LOOP���ňÖقɗ��p�B
	REG_NUM_07_CONDITION_CODE3, 	// CC3���W�X�^�BIF��LOOP���ňÖقɗ��p�B
	REG_NUM_08_REG0,				// �ėp���W�X�^0�B �J�E���^���W�X�^0�B�J�E���^�l1��ێ��B
	REG_NUM_09_REG1,				// �ėp���W�X�^1�B �J�E���^���W�X�^1�B�J�E���^�l2��ێ��B
	REG_NUM_0a_REG2,				// �ėp���W�X�^2�B �J�E���^���W�X�^2�B�J�E���^�l3��ێ��B
	REG_NUM_0b_REG3,				// �ėp���W�X�^3�B �J�E���^���W�X�^3�B�J�E���^�l4��ێ��B
	REG_NUM_0c_REG4,				// �ėp���W�X�^4�B
	REG_NUM_0d_REG5,				// �ėp���W�X�^5�B
	REG_NUM_0e_REG6,				// �ėp���W�X�^6�B
	REG_NUM_0f_REG7_difficulty_only,// �ėp���W�X�^7�B(��Փx���W�X�^)
	// I/O�|�[�g(�O���̓��o�͂�I/O�|�[�g0x10-0x1f���o�R���Ȃ��Əo���Ȃ�)
	REG_NUM_10_BOSS_SPELL_TIMER,	// �X�y���o�ߎ��ԗp���W�X�^�B
	REG_NUM_11_TAMA1,				// ���e�p���W�X�^1�Btama_system �ɐڑ��BHATSUDAN_01_speed256
	REG_NUM_12_TAMA2,				// ���e�p���W�X�^2�Btama_system �ɐڑ��BHATSUDAN_02_speed_offset
	REG_NUM_13_TAMA3,				// ���e�p���W�X�^3�Btama_system �ɐڑ��BHATSUDAN_03_angle65536
	REG_NUM_14_TAMA4,				// ���e�p���W�X�^4�Btama_system �ɐڑ��BHATSUDAN_04_tama_spec
	REG_NUM_15_TAMA5,				// ���e�p���W�X�^5�Btama_system �ɐڑ��BHATSUDAN_05_bullet_obj_type
	REG_NUM_16_TAMA6,				// ���e�p���W�X�^6�Btama_system �ɐڑ��BHATSUDAN_06_n_way
	REG_NUM_17_TAMA7,				// ���e�p���W�X�^7�Btama_system �ɐڑ��BHATSUDAN_07_div_angle65536
	// �X�N���v�g������ʉ����ǂ��������d�l���m�肵�Ă��Ȃ��B
	REG_NUM_18_VOICE_NUMBER,		// REG_NUM_18_SOUND_REG0,// [����]���ʉ��p���W�X�^0�B
	REG_NUM_19_VOICE_TRACK, 		// REG_NUM_19_SOUND_REG1,// [����]���ʉ��p���W�X�^1�B
	REG_NUM_1a_,					// REG_NUM_1a_SOUND_REG2,// [����]���ʉ��p���W�X�^2�B
	REG_NUM_1b_,					// REG_NUM_1b_SOUND_REG3,// [����]���ʉ��p���W�X�^3�B
	REG_NUM_1c_,					// REG_NUM_1c_SOUND_NUM0,// [����]���ʉ��ԍ��ێ��p���W�X�^0�B
	REG_NUM_1d_,					// REG_NUM_1d_SOUND_NUM1,// [����]���ʉ��ԍ��ێ��p���W�X�^1�B
	REG_NUM_1e_,					// REG_NUM_1e_SOUND_NUM2,// [����]���ʉ��ԍ��ێ��p���W�X�^2�B
	REG_NUM_1f_,					// REG_NUM_1f_SOUND_NUM3,// [����]���ʉ��ԍ��ێ��p���W�X�^3�B
	#if 0
	// ������(���W�X�^�ޔ�p�̈�)
	MEM_NUM_20_ 					// �������B
	MEM_NUM_21_ 					// �������B
	MEM_NUM_22_ 					// �������B
	MEM_NUM_23_ 					// �������B
	MEM_NUM_24_ 					// �������B
	MEM_NUM_25_ 					// �������B
	MEM_NUM_26_ 					// �������B
	MEM_NUM_27_ 					// �������B
	MEM_NUM_28_ 					// �������B
	MEM_NUM_29_ 					// �������B
	MEM_NUM_2a_ 					// �������B
	MEM_NUM_2b_ 					// �������B
	MEM_NUM_2c_ 					// �������B
	MEM_NUM_2d_ 					// �������B
	MEM_NUM_2e_ 					// �������B
	MEM_NUM_2f_ 					// �������B
	// �g��������
	MEM_NUM_30_ 					// �g���������B
	MEM_NUM_31_ 					// �g���������B
	MEM_NUM_32_ 					// �g���������B
	MEM_NUM_33_ 					// �g���������B
	MEM_NUM_34_ 					// �g���������B
	MEM_NUM_35_ 					// �g���������B
	MEM_NUM_36_ 					// �g���������B
	MEM_NUM_37_ 					// �g���������B
	MEM_NUM_38_ 					// �g���������B
	MEM_NUM_39_ 					// �g���������B
	MEM_NUM_3a_ 					// �g���������B
	MEM_NUM_3b_ 					// �g���������B
	MEM_NUM_3c_ 					// �g���������B
	MEM_NUM_3d_ 					// �g���������B
	MEM_NUM_3e_ 					// �g���������B
	MEM_NUM_3f_ 					// �g���������B
	#endif
	REG_NUM_99_MAX// = 0x20/*(.align 32)*/
};
//#define MAX_FIX_VALUE (REG_NUM_01_FIX_VALUE+1)/*�Œ�l�͈̔�*/
//�J�[�h�X�N���v�g���s���ɂ͖��t���[���A�{�X�̌��ݍ��W���X�V�����B
/*static*/extern int spell_register[REG_NUM_99_MAX];

/* ���ۂ̃��W�X�^ */

#define REG_00_SRC_X				spell_register[REG_NUM_00_SRC_X]
#define REG_01_SRC_Y				spell_register[REG_NUM_01_SRC_Y]
#define REG_02_DEST_X				spell_register[REG_NUM_02_DEST_X]
#define REG_03_DEST_Y				spell_register[REG_NUM_03_DEST_Y]
//
#define REG_08_REG0 				spell_register[REG_NUM_08_REG0]
#define REG_09_REG1 				spell_register[REG_NUM_09_REG1]
#define REG_0a_REG2 				spell_register[REG_NUM_0a_REG2]
#define REG_0b_REG3 				spell_register[REG_NUM_0b_REG3]
#define REG_0c_REG4 				spell_register[REG_NUM_0c_REG4]
#define REG_0d_REG5 				spell_register[REG_NUM_0d_REG5]
#define REG_0e_REG6 				spell_register[REG_NUM_0e_REG6]
/*---------------------------------------------------------
	��Փx���W�X�^�B
	�d�l�����܂��Ă��Ȃ��̂ŁA��芸�������蓖�Ă�B
	���蓖�Ĉʒu�͕ς��\���������B
	-------------------------------------------------------
	���̃��W�X�^�́A�J�[�h���������ɓ�Փx������B
---------------------------------------------------------*/
//#define REG_0f_REG7				spell_register[REG_NUM_0f_REG7]
#define REG_0f_GAME_DIFFICULTY		spell_register[REG_NUM_0f_REG7_difficulty_only]
//
#define REG_10_BOSS_SPELL_TIMER 	spell_register[REG_NUM_10_BOSS_SPELL_TIMER] 	/* �X�y���o�ߎ��� */


/*(CARD CPU ��������)*/
extern void sincos256(void);
#endif /* _CARD_CPU_H_ */

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////

#ifndef _HATSUDAN_SYSTEM_H_
#define _HATSUDAN_SYSTEM_H_
/*---------------------------------------------------------
	���e�V�X�e��
	-------------------------------------------------------
	�ǂ�Ȓe�ł����e����ꍇ�͕K���u���e�V�X�e���v��ʂ�Ȃ���΂Ȃ�Ȃ��B
	����́A�ꃖ���ɖ����Ƌɒ[�ɑ��x�ቺ����̂���ȗ��R�B
	-------------------------------------------------------
	���e�V�X�e���͎�ɔ��e�G�t�F�N�g�̎w������邪�A
	���e�G�t�F�N�g�̖����e�ł����R���e�V�X�e����ʂ�Ȃ���΂Ȃ�Ȃ��B
	-------------------------------------------------------
	���e�G�t�F�N�g�͌��󔭒e�V�X�e�����S�����Ă��邪�A
	�����I�ɂ����ƌ����I�ȕʃV�X�e���ɂȂ�B
	�u�e���v�ɓ������āu�e(�e)�v��(�ړ�)�������o���Ȃ��Ȃ�B
	����́u�e���v�V���[�e�B���O�̏ꍇ�A�e�̈ړ������͕K�v�����ׁA
	�e�̈ړ������͖����B����ƒx���B
---------------------------------------------------------*/

/*---------------------------------------------------------
	���e�P�ʂ̎󂯓n���ݒ�(�p�x�e�̏ꍇ)
	-------------------------------------------------------
	�p�x�e�̕���\��65536�x�Ɍ���B
	-------------------------------------------------------
	�����J�[�h�X�N���v�g�V�X�e���Ɉڍs�����ꍇ�A
	���̍\���̂́A�J�[�h�X�N���v�g�p�̃��W�X�^(���[�J���������[)��
	�ړ�����B(�łȂ��ƃJ�[�h�X�N���v�g�ォ��e���ǂ������ł��Ȃ��̂�)
---------------------------------------------------------*/

#if 1
	/*([tama_system �ɒ��ڐڑ����Ă��郌�W�X�^])*/
	// �ȉ��́A �e�̐ݒ�B
	#define HATSUDAN_01_speed256			spell_register[REG_NUM_11_TAMA1]	/* ���x */
	#define HATSUDAN_02_speed_offset		spell_register[REG_NUM_12_TAMA2]	/* �����x */
	#define HATSUDAN_03_angle65536			spell_register[REG_NUM_13_TAMA3]	/* �p�x */					/* <����\65536�x�Ɍ���> */
	#define HATSUDAN_04_tama_spec			spell_register[REG_NUM_14_TAMA4]	/* �e�̔\�͑��� */
	#define HATSUDAN_05_bullet_obj_type 	spell_register[REG_NUM_15_TAMA5]	/* �e�O���t�B�b�N�̎�� */
	// �ȉ��́A n_way�e�̏ꍇ�ɕK�v�ȗv�f�B
	#define HATSUDAN_06_n_way				spell_register[REG_NUM_16_TAMA6]	/* n way �e�̒e�� */
	#define HATSUDAN_07_div_angle65536		spell_register[REG_NUM_17_TAMA7]	/* n way �e�̕����p�x */	/* <����\65536�x�Ɍ���> */
	// �ʖ���`�B(alias)
	/* �����������ǁA 65536�����Ȃ̂� 1024 �����Ȃ̂���������ׂɕK�v�B */
	#define HATSUDAN_03_angle1024		HATSUDAN_03_angle65536	/* �`��p�p�x */	/* <�`��p�̕���\1024�x�ɗ��Ƃ��Ă��鎖�𖾎�����B> */
	/* �����������ǁA  �B */
	#define REG_11_GOUSEI_WARIAI256 	HATSUDAN_01_speed256
#endif

#if 1
	/*([tukaima_system �ɒ��ڐڑ����Ă��郌�W�X�^])*/
	/*
		�g�����Ɋւ��ẮA���ڃX�N���v�g����R���g���[���ł��Ȃ��Ƙb�ɂȂ�Ȃ��̂ŁA
		���ڐڑ����Ă��郌�W�X�^������B
	*/
#endif

#if 1
	/*([audio_system�ɒ��ڐڑ����Ă��郌�W�X�^])*/
	/*
		���ʉ��ABGM�����W�X�^�o�R�łȂ��ƃX�N���v�g���o���Ȃ��B
	*/
#endif

/*---------------------------------------------------------
	�o�^���
---------------------------------------------------------*/

extern void hatudan_system_regist_single(void); 			/* �P�� */
extern void hatudan_system_regist_katayori_n_way(void); 	/* �΂� n way�e */
extern void hatudan_system_regist_n_way(void);				/* �ʏ� n way�e */


/*---------------------------------------------------------
	���e�o�^�K�i
---------------------------------------------------------*/
	#define hatudan_register_spec_data			system_data00	/* ��ʊO��������┽�ˋ@�\ */
	//
	#define hatudan_register_speed65536 		user_data00 	/* ������ */	//	#define hatudan_system_speed256 user_data02 	/* ������ */
	#define hatudan_register_tra65536			user_data01 	/* ���������� */
	#define hatudan_register_frame_counter		user_data03 	/* ���e�t���[���J�E���^(���l�Ŕ��e) */
	#define hatudan_register_user_data_a01		user_data04 	/* (r35) */
//	#define hatudan_system_bbb					user_data05 	/* �g���\��(?) (r33���ݖ��g�p) / */
//	#define hatudan_system_ccc					user_data06 	/* �g���\��(?) (r33���ݖ��g�p) / */

#define hatudan_register_2nd_angle65536 		hatudan_register_user_data_a01	/*(2��ڂɕ��􂷂�p�x��ێ�)*/

/*---------------------------------------------------------
	�e�X�y�b�N
	�e�̔\�́A��Ԃ�����킷�B
	�`�悷��ꍇ�̃O���t�B�b�N�A�X���e�B��X���e�B�̋�ʁB
	�e���V�X�e���̂ǂ̃��C���[�ɏ�������e���̋�ʁB
---------------------------------------------------------*/

#if 1
	#define TAMA_SPEC_0000_TILT 				(0x0000)/* �X���e(�ʏ�e) */
	#define TAMA_SPEC_8000_NON_TILT 			(0x8000)/* ��X���e */		/* �X���Ȃ��e(��ʒe�A�`���m�e�A���p) */
//	#define TAMA_SPEC_4000_NON_MOVE 			(0x4000)/* �ړ������Ȃ� */
//	#define TAMA_SPEC_4000_NON_MOVE 			(0x0000)/* �ړ������Ȃ�(r35�W��) */
	#define TAMA_SPEC_4000_GRAZE				(0x4000)/* �O���C�Y�ς݂��Ƃ����̃t���O */
	#define TAMA_SPEC_3000_EFFECT_MASK			(0x3000)/* �G�t�F�N�g�I�� */
	#define TAMA_SPEC_3000_EFFECT_NONE			(0x3000)/* �G�t�F�N�g�Ȃ� */
	#define TAMA_SPEC_2000_EFFECT_MINI			(0x2000)/* �G�t�F�N�g�� */
	#define TAMA_SPEC_1000_EFFECT_MIDDLE		(0x1000)/* �G�t�F�N�g�� */
	#define TAMA_SPEC_0000_EFFECT_LARGE 		(0x0000)/* �G�t�F�N�g��(��r34�݊�) */
#endif
	/*(CCW)*/
	#define TAMA_SPEC_0x100_CCW_BIT 			(0x0100)
	/*(����̋Ȃ���N�i�C�A�e)*/
	#define TAMA_SPEC_AKA_AO_KUNAI_BIT			(TAMA_SPEC_0x100_CCW_BIT)
	/*(�p�`�F�̃}�L�����[�|�C�Y��)*/
	#define TAMA_SPEC_AO_MIZU_MERCURY_BIT		(TAMA_SPEC_0x100_CCW_BIT)
	/*(���`���m���E�̕ǂ�1�x��������)*/
	#define TAMA_SPEC_KABE_SAYUU_HANSYA_BIT 	(TAMA_SPEC_0x100_CCW_BIT)
	/*(���~���A�A���f�u�u���h�E�c�F�y�V���̎􂢁v�A��]�����t���O)*/
	/*(���[�~�A�A�ŕ��u�f�B�}�[�P�C�V�����v�A��]�����t���O)*/
	#define TAMA_SPEC_KAITEN_HOUKOU_BIT 		(TAMA_SPEC_0x100_CCW_BIT)
//	#define TAMA_SPEC_KAITEN_HOUKOU_CW			(0x0000)
//	#define TAMA_SPEC_KAITEN_HOUKOU_CCW 		(TAMA_SPEC_0x100_CCW_BIT)


#endif /* _HATSUDAN_SYSTEM_H_ */


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

/*---------------------------------------------------------
	���[�U�[�V�X�e��
---------------------------------------------------------*/

#ifndef _LASER_SYSTEM_H_
#define _LASER_SYSTEM_H_

/*
	�p�`�F�u�ʏ�U���v
	�t�����֊��u���[���@�e�C���v
	�t�����֒e�u�ߋ������ގ��v�v
	�����̃J�[�h����邩�ǂ����͕ʖ��Ƃ��āA�����̃��[�U�[�e��
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

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


#ifndef _SPELL_SYSTEM_H_
#define _SPELL_SYSTEM_H_
/*
	�X�y���V�X�e��==�J�[�h�̊Ǘ��V�X�e���B(�J�[�h�V�X�e���Ƃ͈Ⴄ�̂Œ���)
*/

/*---------------------------------------------------------
	�X�y�������V�X�e��(�J�[�h�𐶐�)
---------------------------------------------------------*/

	/* �e��x256 �e��y256 �{�X���S���甭�e�B */
//#define set_REG_DEST_XY(aaa) {REG_02_SEND1_BOSS_X_LOCATE = (aaa->center.x256);	REG_03_SEND1_BOSS_Y_LOCATE = (aaa->center.y256); }
extern OBJ_CALL_FUNC(set_REG_DEST_XY);
#if 0/*���ƂŗL���ɂ���*/
extern void spell_cpu_douchuu_init(void);
extern OBJ_CALL_FUNC(card_maikai_init);
extern OBJ_CALL_FUNC(card_generate);
#endif


/* HATSUDAN_03_angle65536 �� ���@�_���e�̊p�x���v�Z */
extern CPU_FUNC(tmp_angleCCW65536_src_nerai);

/* HATSUDAN_03_angle65536 �� ���@�_���e�̊p�x���v�Z */
extern CPU_FUNC(calculate_jikinerai);

/*---------------------------------------------------------
	(r36)�J�[�h�X�N���v�g�p����(multiprex_rate_vector)
	�������������B
---------------------------------------------------------*/
extern CPU_FUNC(multiprex_rate_vector);


/* �X�y����CPU���s���A�J�[�h���P�t���[����������B */
extern OBJ_CALL_FUNC(card_generate);



#endif /* _SPELL_SYSTEM_H_ */

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#ifndef _CARD_SYSTEM_H_
#define _CARD_SYSTEM_H_

//------------ �J�[�h�֘A

extern void create_card(OBJ/**/ *src, int card_number);


//extern OBJ_CALL_FUNC(ch eck_regist_card);/* �J�[�h�̍X�V�`�F�b�N */
extern OBJ_CALL_FUNC(card_boss_move_generate_check_regist); /* �J�[�h�o�^�\�Ȃ�o�^ / �J�[�h���� */


// extern OBJ_CALL_FUNC(card_state_check_holding);/* �J�[�h���I���܂ő҂B */


enum
{
	DANMAKU_LAYER_00 = 0,	//(0)/* �e���R���g���[�����Ȃ��ʏ�e(��ʊO�Œe����) */
	DANMAKU_LAYER_01,		//(1)/* �e���R���g���[���O���[�v(1)�e */
	DANMAKU_LAYER_02,		//(2)/* �e���R���g���[���O���[�v(2)�e */
	DANMAKU_LAYER_03,		//(3)/* �e���R���g���[���O���[�v(3)�e */
	DANMAKU_LAYER_04_MAX/* �e���R���g���[���O���[�v�ő吔 */
};

typedef struct /*_card_global_class_*/
{
	void (*boss_move_card_callback)(OBJ/**/ *sss);			/* �{�Xcard���u�ړ��v���� */
	int limit_health;		/* �K��l�ȉ��ɂȂ�� �J�[�h���[�h���� */
	int card_timer; 		/* [���p]�X�y�J�̐������� */
	int tukaima_used_number;/*(�g�����V�X�e��)*/
	//
	int address_set;		/* [���p]�J�[�h�A�h���X�ԍ� */
	int address_temporaly;	/* [�ꎞ�g�p]�J�[�h�A�h���X�ԍ� */
	int spell_used_number;	/* [�g�p���̃X�y���ԍ�] */
	int boss_hp_dec_by_frame;/* �{�X�U�������l�A�t���[���P�� */
	//
	void (*danmaku_callback[(DANMAKU_LAYER_04_MAX)])(OBJ/**/ *sss); /* �e���R�[���o�b�N�V�X�e��(�X�y���ϐg����) */
} CARD_SYSTEM_GLOBAL_CLASS;
extern CARD_SYSTEM_GLOBAL_CLASS card;
//		/* ���l�ɂȂ�΃{�X���J�[�h���[�h�ɓ���Ȃ� */
	// �u�ړ��ł��Ȃ������t���O�v(�g�p�O�Ɏ蓮��OFF==0�ɂ��Ƃ�)
//	int/*u8*/ boss_hamidasi;			/* �u�ړ��ł��Ȃ������t���O�v(�g�p�O�Ɏ蓮��OFF==0�ɂ��Ƃ�) */
//	void (*boss_sel ect_action_callback)(OBJ/**/ *sss);		/* �{�Xcard���u�s���I���v���� */

//	int dummy2;
/* �Ƃ肠���� */


//------------ "��"�݂����ȃ}�[�N�̃G�t�F�N�g
/*
�u"��"�݂����ȃ}�[�N�v�́A�{�X�������Ă�J�[�h�������ł��B
������{���́A�J�[�h�̖�������������A���������炳�Ȃ��Ⴂ���Ȃ��̂��ȁH
�ł��{�Ƃ����Ȃ��ĂȂ���ˁB
*/
/*static*/extern OBJ_CALL_FUNC(move_card_square_effect);
/*static*/extern void boss_effect_kotei_obj_r36_taihi(void);

extern void game_core_danmaku_system_callback(void);
#endif /* _CARD_SYSTEM_H_ */
