
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
		�e����`
	-------------------------------------------------------
	���e���̓V�X�e���ŃT�|�[�g����\�z������܂��B(�\�z��)
	�V�X�e���ŃT�|�[�g�����ƁA�ǂ̃{�X���A�ǂ̒e���ł����Ă�悤�ɂȂ�܂��B
	-------------------------------------------------------
	boss->boss_base_danmaku_type �Ɍ��������e�����Z�b�g����ƁA�e�������J�n�B
	�e���������I������Ǝ����I�� boss->boss_base_danmaku_type �� DANMAKU_00 �ɂȂ�B
	-------------------------------------------------------
	�e���������� boss->boss_base_danmaku_time_out ������ɖ��t���[�����Z����A0 �ɂȂ�ƒe�������I���B
	���̎��Ԃ͒e�����ƂɈႤ�W�����Ԃ��ݒ肳��Ă���̂ŁA�����ݒ肷��ɂ�
	danmaku_set_time_out(boss); 	// �e���̎��Ԑ؂��ݒ�
	�Ƃ���B
	---------------------------------------------------------*/
enum
{
	DANMAKU_00 = 0, 			/* �e�������I���t���O�B */
	/* [���] */
	DANMAKU_01_sakuya,			/* ����S�e�� / �g5�ʒ��{�X ��� �u�ʏ�U���v */
	DANMAKU_02_sakuya,			/* ���24�e�� (��p�u�~�X�f�B���N�V�������ǂ�(1/2)�v) */
	DANMAKU_03_sakuya,			/* ���11�e�� (��p�u�~�X�f�B���N�V�������ǂ�(2/2)�v) */
//	���\��
	DANMAKU_04_sakuya,			/* �g5�ʃ{�X ��� �u�ʏ�U��1(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
	DANMAKU_05_sakuya,			/* �g5�ʃ{�X ��� �u�ʏ�U��1/2(2/2)�v�ɂ�����Ƃ��������e��(�\��) */
	DANMAKU_06_sakuya,			/* �g5�ʃ{�X ��� ���ہu���i�N���b�N(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
	DANMAKU_07_sakuya,			/* �g5�ʃ{�X ��� �u�ʏ�U��3(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
//	/* [�A���X(��)] -------------------- */
	DANMAKU_08_rumia,			/* �g1�ʃ{�X ���[�~�A �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������e��(�\��) */
	DANMAKU_09_alice,			/* �A���X���ۂ��H�e�L�g�[�e�� */
	DANMAKU_0a_houka_kenran,	/* �g���� �ؕ��u�F�؈�ࣁv�ɂ�����Ƃ��������e��(�\��) */
	DANMAKU_0b_alice_doll,		/* �A���X�l�`�e�� */	//40[sec]
	DANMAKU_0c_hana_test,		/* �����A�ԂĂ��ƒe�� */
	DANMAKU_0d_mima_sekkin, 	/* �����A�ڋߒe�� */
	DANMAKU_0e_test,
	DANMAKU_0f_test,
//	/* [�`���m(��)] -------------------- */
	DANMAKU_10_cirno,			/* �`���m�S�e�� */
	DANMAKU_11_tengu,/*???*/
	DANMAKU_12_aya_merin_test,	/* ���ɔ�����ۂ��e���������Ă݂�e�X�g */
	DANMAKU_13_perfect_freeze,	/* �p�[�t�F�N�g�t���[�Y */
	DANMAKU_14,
	DANMAKU_15,
	DANMAKU_16,
	DANMAKU_17,
//	/* [�P��(��)] -------------------- */
	DANMAKU_18_kaguya01,
	DANMAKU_19_kaguya04,
	DANMAKU_1a_dai_yousei_01,	/* �S�� 48�����A���v���A�΃N�i�C�e */
	DANMAKU_1b_dai_yousei_02,	/* �S�� 48�����A�����v���A�ԃN�i�C�e */
	DANMAKU_1c,
	DANMAKU_1d,
	DANMAKU_1e,
	DANMAKU_1f,
//
	DANMAKU_MAX,
};

/* target->tmp_angleCCW1024 �� ���@�_���e�̊p�x���v�Z */
extern void tmp_angleCCW1024_jikinerai(SPRITE *p, SPRITE *target);
extern void tmp_angleCCW65536_jikinerai(SPRITE *p, SPRITE *target);

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
#endif /* _BOSS_H_ */
