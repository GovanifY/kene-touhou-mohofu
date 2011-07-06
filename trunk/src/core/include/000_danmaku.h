
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _DANMAKU_H_
#define _DANMAKU_H_
	/*---------------------------------------------------------
	�e���V�X�e��(���炩���ߒe���̒�`�����Ă����V�X�e��)
	-------------------------------------------------------
	�͕핗�ł͒e���̓V�X�e���ŃT�|�[�g���Ă��܂��B
	�e���Ƃ��Ē�`���Ă����ƁA�ǂ̃{�X���A�ǂ̒e���ł����Ă�悤�ɂȂ�܂��B
	-------------------------------------------------------
	�܂��e���̓X�y���J�[�h�V�X�e���ɔԍ���o�^���Ă����ƁA�X�y���J�[�h�V�X�e���Œe�����擾�ł��܂��B
	(�܂�X�y���J�[�h�V�X�e���o�R�Œe�������Ă܂�)
	-------------------------------------------------------
	(r32)���݁A�G��(�͕핗��dat�̒��{�X)���e�������Ă܂����A
	���̋@�\�͏����Ȃ��Ȃ邩���m��܂���B
	�G��(�͕핗��dat�̒��{�X)���e�������Ă�@�\�́u���_�\�v�Ŏv�������̂ł����A
	���������@�\�����������Q�[���̑��x�������o����C�����邵�A
	�����������炱�������@�\�͖{�Ƃɖ����̂����Ǝv�����Ƃ�����܂��B�ǂ�����܂���B
	-------------------------------------------------------
	boss->boss_base_danmaku_type �Ɍ��������e�����Z�b�g����ƁA�e�������J�n�B
	�e���������I������Ǝ����I�� boss->boss_base_danmaku_type �� DANMAKU_00 �ɂȂ�B
	-------------------------------------------------------
	�e���������� boss->boss_base_danmaku_time_out ������ɖ��t���[�����Z����A0 �ɂȂ�ƒe�������I���B
	���̎��Ԃ͒e�����ƂɈႤ�W�����Ԃ��ݒ肳��Ă���̂ŁA�����ݒ肷��ɂ�
	danmaku_set_time_out(boss); 	// �e���̎��Ԑ؂��ݒ�
	�Ƃ���B
	---------------------------------------------------------*/

/* �e���ԍ�(�e�e������ʂ���ׂ̔ԍ�) */
enum
{
	DANMAKU_00 = 0, 					/* �e�������I���t���O�B */
//	/* �G���p */
	DANMAKU_01_sakuya,					/* [���] �g5�ʒ��{�X �S�e�� / �g5�ʒ��{�X ��� �u�ʏ�U���v */
	DANMAKU_02_sakuya_old_test, 		/* [���] �g5�ʒ��{�X 24�e�� (��p�u�~�X�f�B���N�V�������ǂ�(1/2)�v) */
	DANMAKU_03_sakuya_old_test, 		/* [���] �g5�ʒ��{�X 11�e�� (��p�u�~�X�f�B���N�V�������ǂ�(2/2)�v) */
	DANMAKU_04_sakuya,					/* [���] �g5�ʃ{�X [���] �u�ʏ�U��1(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
	DANMAKU_05_sakuya,					/* [���] �g5�ʃ{�X [���] �u�ʏ�U��1/2(2/2)�v�ɂ�����Ƃ��������e��(�\��) */
	DANMAKU_06_sakuya,					/* [���] �g5�ʃ{�X [���] ���ہu���i�N���b�N(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
	DANMAKU_07_sakuya,					/* [���] �g5�ʃ{�X [���] �u�ʏ�U��3(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
//
	DANMAKU_08_rumia,					/* [���[�~�A] �g1�ʃ{�X [���[�~�A] �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������e��(�\��) */
	DANMAKU_09_alice,					/* [�A���X] ���ۂ��H�e�L�g�[�e�� */
	DANMAKU_0a_dai_yousei_01,			/* [��d��] �S�� 48�����A���v���A�΃N�i�C�e */
	DANMAKU_0b_dai_yousei_02,			/* [��d��] �S�� 48�����A�����v���A�ԃN�i�C�e */
	DANMAKU_0c_sakuya_jack32,			/* [���] ���H�u�W���b�N�E�U�E���h�r���v�ɂȂ邩������Ȃ��e�X�g */
	DANMAKU_0d_sakuya_miss_direction,	/* [���] ��p�u�~�X�f�B���N�V�����v */
	DANMAKU_0e_remilia_00,				/* [���~���A] �e�X�g */
	DANMAKU_0f_tengu,					/* [���̑�] �������̗d��1(�V��l?)�����e��(�\��) */		/*???*/
//
	DANMAKU_10_cirno,					/* [�`���m] �S�e�� */
	DANMAKU_11_perfect_freeze,			/* [�`���m] �p�[�t�F�N�g�t���[�Y */
	DANMAKU_12_diamond_blizzard,		/* [�`���m] �ᕄ�u�_�C�A�����h�u���U�[�h�v */
	DANMAKU_13_alice_suwako,			/* [�A���X] �z�K�q���ۂ�(?)�e�� */
	DANMAKU_14_alice_youmu300,			/* [�A���X] �d3�ʃ{�X  normal�ʏ�U��3(�̈ꕔ) */
	DANMAKU_15_alice_aka_2nd,			/* [�A���X] ��EX�ʃ{�X �Ԃ̖��@2�i�K�ڂ��ǂ� */
	DANMAKU_16_alice_doll,				/* [�A���X] �d3�ʃ{�X [�A���X]�l�`�e��(���ǂ�) */	// 40[sec]
	DANMAKU_17_alice_doll_02,			/* [�A���X] �쐬�� */
//
	DANMAKU_18_hana_test,				/* [����] �ԂĂ��ƒe�� */
	DANMAKU_19_mima_sekkin, 			/* [����] �ڋߒe�� */
	DANMAKU_1a_mima_toge,				/* [����] �g�Q�Ă��ƒe�� */
	DANMAKU_1b,
	DANMAKU_1c_kakuya_tamanoe,			/* [�P��]�A�H���̋ʂ̎}���ǂ��B�쐬�� */
	DANMAKU_1d,
	DANMAKU_1e_kaguya01,				/* [�P��] �ނ���ϊ�1 */
	DANMAKU_1f_kaguya04,				/* [�P��] �ނ���ϊ�2 */
//	/* �{�X�p */
	DANMAKU_20_aya_misogi1, 			/* [��] �̒ʏ�e���ۂ��e���������Ă݂�e�X�g1 */
	DANMAKU_21_aya_misogi2, 			/* [��] �̒ʏ�e���ۂ��e���������Ă݂�e�X�g2 */
	DANMAKU_22_ame_no_yatimata, 		/* [��] �򕄁u�V�̔��ˁv */
	DANMAKU_23_aya_merin_test,			/* [��] �ɔ�����ۂ��e���������Ă݂�e�X�g */
	DANMAKU_24_aya_doll,				/* [��] �l�`�e�� */ //40[sec]
	DANMAKU_25_houka_kenran,			/* [�g����] �ؕ��u�F�؈�ࣁv�ɂ�����Ƃ��������e��(�\��) */
	DANMAKU_26_aya_saifu,				/* [��] �Ǖ� */
	DANMAKU_27_PACHE_LASER2,			/* [�p�`�F] �̃��[�U�[���ǂ� */
//
	DANMAKU_28_rumia_night_bird,		/* [���[�~�A] �g1�ʃ{�X �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������e��(�\��) */
	DANMAKU_29_rumia_demarcation,		/* [���[�~�A] �g1�ʃ{�X �ŕ��u�f�B�}�[�P�C�V�����v�ɂ�����Ƃ��������e��(�\��) */
	DANMAKU_2a_amefuri_test,			/* [] �J�Ă��� */
	DANMAKU_2b,
	DANMAKU_2c,
	DANMAKU_2d,
	DANMAKU_2e_SONOTA_DEBUG_CW_AO,		/* [���̑�] �f�o�b�O�e��CW�� */
	DANMAKU_2f_SONOTA_DEBUG_CCW_AKA,	/* [���̑�] �f�o�b�O�e��CCW�� */
//
	DANMAKU_MAX,
};

/* target->tmp_angleCCW1024 �� ���@�_���e�̊p�x���v�Z */
extern void tmp_angleCCW1024_src_nerai(SPRITE *src, SPRITE *dest);
extern void tmp_angleCCW1024_jiki_nerai(SPRITE *target);
extern void tmp_angleCCW65536_src_nerai(SPRITE *src, SPRITE *dest);
extern void tmp_angleCCW65536_jiki_nerai(SPRITE *target);

/* �e���̎��Ԑ؂��ݒ� */
extern void danmaku_set_time_out(SPRITE *src);

/* �e������ */
extern void danmaku_generator(SPRITE *src);



#endif /* _DANMAKU_H_ */


#ifndef _BOSS_COMMON_H_
#define _BOSS_COMMON_H_
enum
{
	DESTROY_CHECK_00_WIN_BOSS = 0,
	DESTROY_CHECK_01_IS_TIME_OUT,
};
/*static*/extern  void boss_destroy_check_type(SPRITE *src/*�G����*/, int check_type);/*	�� �U���̏ꍇ�̎��S���� 	�� ���Ԑ؂�̏ꍇ�̎��S���� */
/*static*/extern  void callback_hit_boss_dummy(SPRITE *src, SPRITE *tama /*, int angle*/);
/*static*/extern  void callback_hit_boss(SPRITE *src, SPRITE *tama /*, int angle*/);

/*static*/extern  void common_boss_put_items(SPRITE *src);

/*static*/extern  void lose_boss(SPRITE *src);






/* �X�y�J�V�X�e�� */
/*static*/extern void spell_card_get_danmaku_number(SPRITE *src);
#endif /* _BOSS_H_ */
