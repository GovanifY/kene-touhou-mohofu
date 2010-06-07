
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	2�� ����
	���� �i��? (���A�P��A������)
	Mitei Madakimattenai.
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
	�{�X�s��
---------------------------------------------------------*/

global void mima_01_keitai(SPRITE *src)
{
	/* �e���������ĂȂ��ꍇ�ɒǉ� */
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* �e�������I���Ȃ�e������ */
	{
		src->boss_base_state001++;
		src->boss_base_state001 &= (8-1);
		if (0==src->boss_base_state001)
		{
		//	if (1<difficulty)
			{
				/* �ړ����W�����߂� */
				src->target_x256 = (obj_player->x256);
				#if 0
			//	if (t256(16.0) > (src->target_y256))
				if ( (((obj_player->y256)-t256(16.0))>>16) )
				{
					src->target_y256 = t256(16.0);
				}
				else
				#endif
				{
					src->target_y256 = (obj_player->y256);
			//		src->target_y256 -= t256(8.0);	/* ??? ���̂��I�������Ȃ��B */
				}
				/* �e�������߂� */
				src->boss_base_danmaku_type 	= DANMAKU_0d_mima_sekkin;	/* �e�����Z�b�g */
			}
		}
		else
		{
			static const POINT_u16 hosi_no_zahyou[(8)] = /* �ڕW���W */
			{	/* {(x),(y)}, */
				{(171),( 18)},	/* ��l�������� */
				{(202),( 92)},	/* �k�l����(5)�� */
				{(257),(107)},	/* �k�l����(6)�� */
				{(339),( 74)},	/* �k�l����(7)�� */
				{(	5),(116)},	/* �k�l����(1)�� */
				{(	6),( 46)},	/* �k�l����(2)�� */
				{(107),( 26)},	/* �k�l����(3)�� */
				{(133),( 78)},	/* �k�l����(4)�� */
			};
			/* �ړ����W�����߂� */
			src->target_x256 = ((hosi_no_zahyou[src->boss_base_state001].x)<<8);
			src->target_y256 = ((hosi_no_zahyou[src->boss_base_state001].y)<<8);
			/* �e�������߂� */
			src->boss_base_danmaku_type 	= DANMAKU_0c_hana_test; 	/* �e�����Z�b�g */
		}
		danmaku_set_time_out(src);		/* �e���̐������Ԃ�ݒ�(�\�ߒe�����Ƃɐݒ肳��Ă���W�����Ԃɐݒ�) */
		/* */
		src->vvv256 = t256(1.0);
	}
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	bullet_angle_all_hamidasi_check();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}