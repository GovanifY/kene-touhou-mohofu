
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�H���R �P��
	Houraisan Kaguya.
	-------------------------------------------------------
---------------------------------------------------------*/


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
		src->toutatu_wariai256 = t256(1.0);
	}
	/* �e���������ĂȂ��ꍇ�ɒǉ� */
	if (SPELL_00==spell_card.spell_type)		/* �e�������I���Ȃ�e������ */
	{
		/* �C���ŖڕW�����߂�B */
		kaguya_target_x256 = ((ra_nd()&0xffff))+(t256(48));
		/* �ړ����������߂� */
		src->boss_base_state777++;
		src->boss_base_state777 &= (4-1);
		#if (1)
		if ((cg_game_difficulty) >= (unsigned)(src->boss_base_state777) )/*(���O�̘_��AND�ŕK��unsigned���ۏ؂���邩��unsigned��r���g��)*/
		{
			spell_card_get_spell_number(src);
		}
		#endif
	}
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
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
	if (SPELL_00==spell_card.spell_type)		/* �e�������I���Ȃ�e������ */
	{
		/* �ړ����������߂� */
		src->boss_base_state777++;
		src->boss_base_state777 &= (4-1);
		#if (1)
		if ((cg_game_difficulty) >= (unsigned)(src->boss_base_state777) )/*(���O�̘_��AND�ŕK��unsigned���ۏ؂���邩��unsigned��r���g��)*/
		{
			spell_card_get_spell_number(src);
		}
		#endif
	}
	bullet_angle_all_gamen_gai_nara_kesu();/* �p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�) */
}
