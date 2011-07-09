
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�p�`�����[�E�m�[���b�W
	Patchouli Knowledge.
	-------------------------------------------------------
---------------------------------------------------------*/


/*---------------------------------------------------------
	�e���������ĂȂ��ꍇ�ɒǉ�
---------------------------------------------------------*/

static void pache_add_spell(SPRITE *src)
{
	/* �e���������ĂȂ��ꍇ�ɒǉ� */
	if (SPELL_00==spell_card.spell_type)		/* �e�������I���Ȃ�e������ */
	{
		spell_card.number_temporaly++;
		spell_card.number_temporaly &= 0x0f;
		#if 1
		static const u8 pache_spell_table[16] =
		{
			SPELL_07_sakuya,
			SPELL_0a_dai_yousei_01,
			SPELL_0b_dai_yousei_02,
			SPELL_08_rumia,
			//
			SPELL_04_sakuya_pink_hearts,	// ???
			SPELL_09_alice,
			SPELL_05_sakuya,
			SPELL_25_houka_kenran,		/* SPELL_09_alice */
			//
			SPELL_10_cirno, 			// SPELL_06_sakuya,	/* �`���m�S�e�� */		// SPELL_09_alice,
			SPELL_18_hana_test, 		// SPELL_07_sakuya,
			0,							// �b���ő��؂�
			SPELL_01_sakuya_misogi_normal,
			//
			SPELL_02_sakuya_old_test,	/* ���̒e���́A���ɍ��ł͎g���ĂȂ�(r32) */
			SPELL_03_sakuya_old_test,	/* ���̒e���́A���ɍ��ł͎g���ĂȂ�(r32) */
			SPELL_11_perfect_freeze,	// SPELL_04_sakuya_pink_hearts, 	// SPELL_05_sakuya,
			SPELL_16_alice_doll,		// SPELL_06_sakuya,
		};
		#endif
		spell_card.spell_type	= pache_spell_table[spell_card.number_temporaly];	/* �e�����Z�b�g */
		spell_set_time_out(src);		/* �e���̎��Ԑ؂��ݒ� */
//		src->toutatu_wariai256 = t256(1.0);
	}
}


/*---------------------------------------------------------
	�{�X�s���A�� 4�`��
---------------------------------------------------------*/

global void pache_04_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait--;/*fps_factor*/
	if (my_wait <= 0)
	{	/* �ړ����������߂� */
		u16 my_rand;	my_rand = ra_nd();
		my_wait 	= (my_rand & 0xff); 	/* 50*4 60 �ړ��� */
		//
		src->target_y256 = (((my_rand)&0x7f00));
		src->target_x256 = ((my_rand&0x1ff)<<8);
		if (src->cx256 > t256(GAME_WIDTH)-t256(48) )	{	src->target_x256 = -t256(256);}
		{
			if (0==(my_rand&0x04))
			{
				src->kougeki_anime_count = (48);	/* �U���A�j���[�V�����J�n */
			}
			src->toutatu_wariai256 = t256(1.0);
		}
	}
//
	pache_add_spell(src);/* �e���������ĂȂ��ꍇ�ɒǉ� */
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
	bullet_angle_all_gamen_gai_nara_kesu();/* �p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�) */
}


/*---------------------------------------------------------
	�{�X�s���A�� 1�`��
---------------------------------------------------------*/

global void pache_01_keitai(SPRITE *src)
{
	pache_add_spell(src);/* �e���������ĂȂ��ꍇ�ɒǉ� */
	bullet_angle_all_gamen_gai_nara_kesu();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
#if 0
global void bullet_regist_multi_fix(void);/* �p�`�F���[�U�[�p���ꏉ���� */
global void boss_init_pache(SPRITE *h)
{
//	bullet_regist_multi_fix();/* �p�`�F���[�U�[�p���ꏉ���� */
}
#endif
