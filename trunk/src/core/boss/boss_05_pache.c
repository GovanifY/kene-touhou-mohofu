
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�p�`�����[�E�m�[���b�W
	Patchouli Knowledge.
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
#if 1
static const u8 pache_danmaku_table[16] =
{
	DANMAKU_07_sakuya,
	DANMAKU_1a_dai_yousei_01,
	DANMAKU_1b_dai_yousei_02,
	DANMAKU_08_rumia,
	//
	DANMAKU_04_sakuya,//???
	DANMAKU_09_alice,
	DANMAKU_05_sakuya,
	DANMAKU_0a_houka_kenran/*DANMAKU_09_alice*/,
	//
	DANMAKU_10_cirno,//DANMAKU_06_sakuya,	/* �`���m�S�e�� */
//	DANMAKU_09_alice,
//
	DANMAKU_0c_hana_test,//DANMAKU_07_sakuya,
	0,
	DANMAKU_01_sakuya,
	//
	DANMAKU_02_sakuya,
	DANMAKU_03_sakuya,
	DANMAKU_13_perfect_freeze,//DANMAKU_04_sakuya,
//	DANMAKU_05_sakuya,
	DANMAKU_0b_alice_doll,//DANMAKU_06_sakuya,
};
#endif

static void pache_add_danmaku(SPRITE *src)
{
	/* �e���������ĂȂ��ꍇ�ɒǉ� */
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* �e�������I���Ȃ�e������ */
	{
		src->boss_base_danmaku_test++;
		src->boss_base_danmaku_test &= 0x0f;
		src->boss_base_danmaku_type 	= pache_danmaku_table[src->boss_base_danmaku_test]; 	/* �e�����Z�b�g */
		danmaku_set_time_out(src);		/* �e���̎��Ԑ؂��ݒ� */
//		src->vvv256 = t256(1.0);
//		alice_anime_count = 48;
	}
}


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

/*static*/ void pache_01_keitai(SPRITE *src)
{
	pache_add_danmaku(src);/* �e���������ĂȂ��ꍇ�ɒǉ� */
	bullet_angle_all_hamidasi_check();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

/*static*/ void pache_04_keitai(SPRITE *src)
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
		if (src->x256 > t256(GAME_WIDTH)-t256(48) ) 	{	src->target_x256 = -t256(256);}
		{
			if (0==(my_rand&0x04))
			{
				alice_anime_count = 48;
			}
			src->vvv256 = t256(1.0);
		}
	}
//
	pache_add_danmaku(src);/* �e���������ĂȂ��ꍇ�ɒǉ� */
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	bullet_angle_all_hamidasi_check();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}

