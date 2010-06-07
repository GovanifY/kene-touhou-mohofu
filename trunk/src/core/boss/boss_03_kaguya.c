
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�H���R �P��
	Houraisan Kaguya.
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0/* �߂��F������́A���ʕ����Ŏg���Ă�B */
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define time_out			user_data03 	/* �������� */
#endif

/*---------------------------------------------------------
	���㉺�ړ�
---------------------------------------------------------*/

/*static*/ void kaguya_01_keitai(SPRITE *src)
{
	static int aaa_wait;
	aaa_wait += (10);
	aaa_wait &= (1024-1);
	src->y256 = (t256(25)+(sin1024((aaa_wait))<<2));
//
	/* �e���������ĂȂ��ꍇ�ɒǉ� */
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* �e�������I���Ȃ�e������ */
	{
		/* �ړ����������߂� */
		src->boss_base_state001++;
		src->boss_base_state001 &= (4-1);
		#if (1)
		if (difficulty >= src->boss_base_state001 )
		{
			src->boss_base_danmaku_type 	= DANMAKU_18_kaguya01;		/*DANMAKU_01*/	/* �S�e�����Z�b�g */
			danmaku_set_time_out(src);		/* �e���̎��Ԑ؂��ݒ� */
		}
		#endif
	}
	bullet_angle_all_hamidasi_check();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}

/*---------------------------------------------------------
	�΂߈ړ�
---------------------------------------------------------*/

/*static*/ void kaguya_04_keitai(SPRITE *src)
{
	static int kaguya_target_x256;
	static int aaa_wait;
	aaa_wait--;/*fps_factor*/
	if (aaa_wait <= 0)
	{
		aaa_wait = 8;
		/* �ڕW�ɂ����Â����Ɠw�́B */
		src->target_x256 += ((src->target_x256 < kaguya_target_x256) ? t256(8) : -t256(8) );
		src->target_x256 &= 0xffff;
		src->target_y256 = (t256(25)+(sin1024((src->target_x256>>4))<<6));
		src->vvv256 = t256(1.0);
	}
	/* �e���������ĂȂ��ꍇ�ɒǉ� */
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* �e�������I���Ȃ�e������ */
	{
		/* �C���ŖڕW�����߂�B */
		kaguya_target_x256 = ((ra_nd()&0xffff))+(t256(48));
		/* �ړ����������߂� */
		src->boss_base_state001++;
		src->boss_base_state001 &= (4-1);
		#if (1)
		if (difficulty >=  src->boss_base_state001 )
		{
			src->boss_base_danmaku_type 	= DANMAKU_19_kaguya04; /*DANMAKU_01*/	/* �S�e�����Z�b�g */
			danmaku_set_time_out(src);		/* �e���̎��Ԑ؂��ݒ� */
		}
		#endif
	}
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	bullet_angle_all_hamidasi_check();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}
