
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	����
	Mitei Madakimattenai.
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0/*����*/
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define boss_time_out		user_data03 	/* �������� */
	#define boss_base_state777	user_data03 	/* ��������(boss_time_out�Ɠ���) */
#endif


/*---------------------------------------------------------
	�{�X�s���A�� 1�`��
---------------------------------------------------------*/
extern void add_zako_mima_dolls(SPRITE *src);
global void mima_01_keitai(SPRITE *src)
{
	static int mima_jikan;
	mima_jikan--;
	/* �e���������ĂȂ��ꍇ�ɒǉ� */
//	if (DANMAKU_00==src->boss_base_danmaku_type)		/* �e�������I���Ȃ�e������ */
	if (0 > mima_jikan)		/* �e�������I���Ȃ�e������ */
	{
		{
			const unsigned char aa_ra_nd = ra_nd();
			if (0==(aa_ra_nd&0x03))
			{
				add_zako_mima_dolls(src);
			}
			mima_jikan = /*123*/64 + (aa_ra_nd&0x3f);
		}
		//
		src->boss_base_state777++;
		src->boss_base_state777 &= (8-1);
		if (0==src->boss_base_state777)
		{
		//	if (1<difficulty)
			{
				/* �ړ����W�����߂� */
				SPRITE *zzz_player;
				zzz_player = &obj00[FIX_OBJ_00_PLAYER];
				src->target_x256 = (zzz_player->cx256);
				#if 0
			//	if (t256(16.0) > (src->target_y256))
				if ( (((zzz_player->cy256)-t256(16.0))>>16) )
				{
					src->target_y256 = t256(16.0);
				}
				else
				#endif
				{
					src->target_y256 = (zzz_player->cy256);
			//		src->target_y256 -= t256(8.0);	/* ??? ���̂��I�������Ȃ��B */
				}
				#if 1
				/* �e�������߂� */
				src->boss_base_danmaku_type 	= DANMAKU_19_mima_sekkin;	/* �e�����Z�b�g */
				danmaku_set_time_out(src);		/* �e���̐������Ԃ�ݒ�(�\�ߒe�����Ƃɐݒ肳��Ă���W�����Ԃɐݒ�) */
				#endif
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
			src->target_x256 = ((hosi_no_zahyou[src->boss_base_state777].x)<<8);
			src->target_y256 = ((hosi_no_zahyou[src->boss_base_state777].y)<<8);
			/* �e�������߂� */
			spell_card_get_danmaku_number(src); 	/* �X�y�J�V�X�e������A���݂̌`�ԂŌ��e���ԍ�����ɓ���A�W����boss_base_danmaku_time_out���Ԃ�ݒ�B */
		//	src->boss_base_danmaku_type 	= DANMAKU_0c_hana_test; 	/* �e�����Z�b�g */
		//	danmaku_set_time_out(src);		/* �e���̐������Ԃ�ݒ�(�\�ߒe�����Ƃɐݒ肳��Ă���W�����Ԃɐݒ�) */
		}
		/* */
		src->vvv256 = t256(1.0);
	}
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}
