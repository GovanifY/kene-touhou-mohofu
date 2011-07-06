
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���� �I�v�V����
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0/*����*/
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define boss_time_out		user_data03 	/* �������� */
#endif
//	�{�X���ʋK�i�Ɠ���(boss.h�C���N���[�h���Ă����Ȃ��Ă��Ή�)
#ifndef target_x256
	#define target_x256 		user_data00
#endif
#ifndef target_y256
	#define target_y256 		user_data01
#endif
#ifndef boss_time_out
	#define boss_time_out		user_data03 	/* �������� */
#endif
//

#define rotate_angle1024	user_data04 	/* �{�X�𒆐S�Ƃ��āA��]�p�x�B(��CCW1024�`��) */
#define shot_angle1024		user_data05 	/* */


/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_mima_doll(SPRITE *src)
{
	bakuhatsu_add_type_ddd(src, BAKUHATSU_FIRE08);
//	item_create_for_boss(src, ITEM_CREATE_MODE_02);/* easy�̓{�����o���Ȃ��čςވʓ�炩������ */
	#if 1
	item_create(src, SP_ITEM_05_TENSU, 5, ITEM_MOVE_FLAG_06_RAND_XY);
//	item_create(src, SP_ITEM_00_P001, 5, ITEM_MOVE_FLAG_06_RAND_XY);
	#else
	{	int i;
		for (i=0; i<(5); i++)
		{
			lose_random_item(src);
		}
	}
	#endif
}


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_mima_doll(SPRITE *src)
{
	check_boss_option_time_out(src);	/* ���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B */
//
	if ( (unsigned int)(0x01ff/3) < src->boss_time_out)/* �Ō�� 1/3 ���Ԃ͌����ė��Ȃ��B */
	{
		/*	���� & ��镗�A��]�V���b�g */
		if (0 == ((src->boss_time_out)&0x07))	/* ���̃V���b�g�����܂ł̊Ԋu�A���ԁB */
		{
			src->shot_angle1024 += (1024/18);	/* �V���b�g�����������A��]������B */
		//
			obj_send1->cx256 					= (src->cx256)+t256(6.0);/* 6.0[dots]�E */
			obj_send1->cy256 					= (src->cy256)+t256(4.0);/* 4.0[dots]�� */
			br.BULLET_REGIST_speed256			= (t256(1.5))+(((difficulty)<<6));	/* �e�� */
			br.BULLET_REGIST_angle1024			= (src->shot_angle1024);			/* */
			br.BULLET_REGIST_div_angle1024		= (int)(1024/160);					/* �����e */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;			/* [�ԃN�i�C�e] */
			br.BULLET_REGIST_n_way				= (2+difficulty);					/* [2-5way] */
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
			bullet_regist_vector();
		}
	}
//
	/* �I�v�V�����ʒu�A��]�ړ� */
	src->rotate_angle1024 += (2);
	mask1024(src->rotate_angle1024);
	#define HANKEI_45_DOT (45)				/* ���a */
	src->cx256 = src->target_x256 + ((sin1024((src->rotate_angle1024))*(HANKEI_45_DOT)));	/*CCW*/
	src->cy256 = src->target_y256 + ((cos1024((src->rotate_angle1024))*(HANKEI_45_DOT)));
	#undef HANKEI_45_DOT
	src->color32		= (src->color32 & 0x00ffffff) | ((src->boss_time_out<<(23))&0xff000000);
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_mima_dolls(SPRITE *src)
{
//	#define ADD_ANGLE (171*2)	/* 1����3���������p�x�A170.66*2 == 1024/3 */
//	#define ADD_ANGLE (85*2)	/* 1����6���������p�x�A 85.33*2 == 1024/6 */
	#define ADD_ANGLE (102*2)	/* 1����5���������p�x�A102.40*2 == 1024/5 */
//	const int add_angle = (ADD_ANGLE);	/* ���Z�p�x */
//
	int i_angle1024;	/* �ώZ�p�x */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += (ADD_ANGLE)) /* ��� */
	#undef ADD_ANGLE
	{
		SPRITE *h;
		h							= sprite_add_gu_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= BOSS_16_YOUSEI11;
			h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//
			h->callback_mover		= move_mima_doll;
			h->callback_loser		= lose_mima_doll;
			h->callback_hit_enemy	= callback_hit_zako;
	//
			h->target_x256			= (src->cx256);
			h->target_y256			= (src->cy256);
	//
		//	h->base_hp				= ((8*1024)-1); 	/* 8192==(8*1024) */
		//	h->base_hp				= ((3*1024)-1); 	/* 8192==(8*1024) */	/* �d�����Ǔ|���� */
		//	h->base_hp				= (((difficulty+1)<<10)-1); 	/* 8192==(8*1024) */	/* �d�����Ǔ|���� */
			/* ��x�オ��Ɣ����ɍd���Ȃ�(==�|���̂ɂ��ꂾ�����Ԃ�������) */
			h->base_hp				= (((1024)+(difficulty<<8))-1); 	/* 8192==(8*1024) */	/* �d�����Ǔ|���� */
			{
				static const unsigned int difficulty_score_tbl[4] =
				{
					score( 100000), 	/* easy 10�� (�v 50��==5x 10��) */
					score( 300000), 	/* norm 30�� (�v150��==5x 30��) */
					score( 500000), 	/* hard 50�� (�v250��==5x 50��) */
					score(1000000), 	/* luna100�� (�v500��==5x100��) */
				};
			h->base_score			= difficulty_score_tbl[difficulty]; 	/* ��Փx�ɂ�邪�A���Ȃ�҂���B */
		//	h->base_score			= adjust_score_by_difficulty(score( 500000));	/* 50�� (�v300��==6x50��) */
			}
		//
			h->rotate_angle1024 	= i_angle1024;
			h->shot_angle1024		= (0);
			h->boss_time_out			= (0x01ff); 	/* �������� / �\�z�ȏ�ɑ���������ʂɁB */
		}
	}
}
