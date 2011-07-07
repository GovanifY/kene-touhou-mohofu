
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
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
	#define boss_time_out		user_data03 	/* �������� */
	#define boss_base_state777	user_data03 	/* ��������(boss_time_out�Ɠ���) */
#endif


/*---------------------------------------------------------
	�{�X�s���A�� 4�`��
	�΂߈ړ�
---------------------------------------------------------*/

global void kaguya_04_keitai(SPRITE *src)
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
	if (SPELL_00==src->boss_base_spell_type)		/* �e�������I���Ȃ�e������ */
	{
		/* �C���ŖڕW�����߂�B */
		kaguya_target_x256 = ((ra_nd()&0xffff))+(t256(48));
		/* �ړ����������߂� */
		src->boss_base_state777++;
		src->boss_base_state777 &= (4-1);
		#if (1)
		if ((cg_game_difficulty) >=  src->boss_base_state777 )
		{
			spell_card_get_spell_number(src);
		}
		#endif
	}
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	bullet_angle_all_gamen_gai_nara_kesu();/* �p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�) */
}


/*---------------------------------------------------------
	�{�X�s���A�� 1�`��
	���㉺�ړ�
---------------------------------------------------------*/

global void kaguya_01_keitai(SPRITE *src)
{
	static int aaa_wait;
	aaa_wait += (10);
	mask1024(aaa_wait);
	src->cy256 = (t256(25)+(sin1024((aaa_wait))<<2));
//
	/* �e���������ĂȂ��ꍇ�ɒǉ� */
	if (SPELL_00==src->boss_base_spell_type)		/* �e�������I���Ȃ�e������ */
	{
		/* �ړ����������߂� */
		src->boss_base_state777++;
		src->boss_base_state777 &= (4-1);
		#if (1)
		if ((cg_game_difficulty) >= src->boss_base_state777 )
		{
			spell_card_get_spell_number(src);
		}
		#endif
	}
	bullet_angle_all_gamen_gai_nara_kesu();/* �p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�) */
}
