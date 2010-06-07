
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
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
	#define time_out			user_data03 	/* �������� */
#endif

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/
#if 0
//extern const u8 alice_danmaku_table[16];
static const u8 alice_danmaku_table[16] =
{
	DANMAKU_07_sakuya,
	DANMAKU_0b_alice_doll,	//DANMAKU_08_rumia,
	DANMAKU_0b_alice_doll,	//DANMAKU_04_sakuya,
	DANMAKU_0b_alice_doll,	//DANMAKU_09_alice,
	DANMAKU_0b_alice_doll,	//DANMAKU_05_sakuya,
	DANMAKU_0b_alice_doll,	//DANMAKU_0a_houka_kenran/*DANMAKU_09_alice*/,
	DANMAKU_0b_alice_doll,	//DANMAKU_06_sakuya,
	DANMAKU_0b_alice_doll,	//DANMAKU_09_alice,
//
	DANMAKU_07_sakuya,
	0,
	DANMAKU_01_sakuya,
	DANMAKU_02_sakuya,
	DANMAKU_03_sakuya,
	DANMAKU_04_sakuya,
	DANMAKU_05_sakuya,
	DANMAKU_06_sakuya,
};
#endif
/*static*/ void alice_01_keitai(SPRITE *src)
{
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* �e�������I���Ȃ�e������ */
	{
		alice_anime_count = 48;
		src->boss_base_danmaku_type 	= DANMAKU_09_alice; 	/*DANMAKU_01*/	/* �S�e�����Z�b�g */
	//	src->boss_base_danmaku_type 	= DANMAKU_13_perfect_freeze; 	/*DANMAKU_01*/	/* �S�e�����Z�b�g */
		danmaku_set_time_out(src);		/* �e���̎��Ԑ؂��ݒ� */
		src->vvv256 = t256(1.0);
	}
//
	bullet_angle_all_hamidasi_check();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}
/*static*/ void alice_02_keitai(SPRITE *src)
{
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* �e�������I���Ȃ�e������ */
	{
		alice_anime_count = 48;
	//	src->boss_base_danmaku_type 	= DANMAKU_09_alice; 	/*DANMAKU_01*/	/* �S�e�����Z�b�g */
		src->boss_base_danmaku_type 	= DANMAKU_13_perfect_freeze; 	/*DANMAKU_01*/	/* �S�e�����Z�b�g */
		danmaku_set_time_out(src);		/* �e���̎��Ԑ؂��ݒ� */
	}
//
	static int my_wait;
	my_wait--;/*fps_factor*/
	{
		if (my_wait <= 0)
		{
			my_wait 	= (300);	/* �ړ����ԁA�Ԋu */
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
	bullet_angle_all_hamidasi_check();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}

/*static*/ void alice_03_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait--;/*fps_factor*/
	{
		if (my_wait <= 0)
		{	/* �ړ����������߂� */
			src->boss_base_state001++;
			src->boss_base_state001 &= (8-1);
			{
				enum
				{
					PPP_00_TX256 = 0,			/* x �x�N�g���ړ��� */
					PPP_01_TY256,				/* y �x�N�g���ړ��� */
					PPP_02_WAIT_COUNT,			/* �E�F�C�g�J�E���^�̗� */
					PPP_03_IS_RESET_ANIME,		/* �A�j���[�V�������Z�b�g 0:���Ȃ� 1:���� 2:����(�e������) */
				};
				static const u16 ppp[8][4] =
				{
					{(256),(20),(10*8),( 1),},	/*�E���*/
					{(256),(20),( 5*8),( 2),},	/*wait*/
					{(320),(80),(10*8),( 1),},	/*�E����*/
					{(320),(80),( 1*8),( 0),},	/*wait*/
					{(128),(70),(10*8),( 1),},	/*�����*/
					{(128),(70),( 5*8),( 2),},	/*wait*/
					{(160),(30),(10*8),( 1),},	/*������*/
					{(160),(30),( 1*8),( 0),},	/*wait*/
				};
				src->target_x256 = (ppp[src->boss_base_state001][PPP_00_TX256]<<8);
				src->target_y256 = (ppp[src->boss_base_state001][PPP_01_TY256]<<8);
				my_wait = ppp[src->boss_base_state001][PPP_02_WAIT_COUNT];		/* �ړ��� */
				if (0!=ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME])
				{
					if (2==ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME]) 	/* �U���A�j���[�V���� */
					{
						alice_anime_count = 48;

						src->boss_base_danmaku_type 	= DANMAKU_0b_alice_doll;	/*DANMAKU_01*/	/* �S�e�����Z�b�g */
						danmaku_set_time_out(src);		/* �e���̎��Ԑ؂��ݒ� */
					}
					src->vvv256 = t256(1.0);
				}
			}
		}
	}
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	bullet_angle_all_hamidasi_check();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}

