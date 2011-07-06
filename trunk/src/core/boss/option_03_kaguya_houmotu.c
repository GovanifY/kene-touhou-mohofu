
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�P�� �I�v�V����
	-------------------------------------------------------
---------------------------------------------------------*/

#define xcenter256			user_data00
#define ycenter256			user_data01

#define time_out			user_data03

#define rotate_angle1024	user_data04 	/* �{�X�𒆐S�Ƃ��āA��]�p�x�B(��CCW1024�`��) */
//#define fire_wait1		user_data05 	/* */
//#define shot_angle1024	user_data06 	/* */


/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_kaguya_doll(SPRITE *src)
{
	bakuhatsu_add_type_ddd(src, BAKUHATSU_FIRE08);
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
}


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_kaguya_doll(SPRITE *src)
{
	check_boss_option_time_out(src);	/* ���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B */
//
	/* �I�v�V�����ʒu�A��]�ړ� */
	src->rotate_angle1024 += (2);
	mask1024(src->rotate_angle1024);
	#define HANKEI_45_DOT (45)				/* ���a */
	src->cx256 = src->xcenter256 + ((sin1024((src->rotate_angle1024))*(HANKEI_45_DOT))); 	/*CCW*/
	src->cy256 = src->ycenter256 + ((cos1024((src->rotate_angle1024))*(HANKEI_45_DOT)));
	#undef HANKEI_45_DOT
	src->color32		= (src->color32 & 0x00ffffff) | ((src->time_out<<(23))&0xff000000);
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_kaguya_houmotsu(SPRITE *src)
{
//	#define ADD_ANGLE (171*2)	/* 1����3���������p�x�A170.66*2 == 1024/3 */
//	#define ADD_ANGLE (85*2)	/* 1����6���������p�x�A 85.33*2 == 1024/6 */
	#define ADD_ANGLE (102*2)	/* 1����5���������p�x�A102.40*2 == 1024/5 */
//	const int add_angle = (ADD_ANGLE);	/* ���Z�p�x */
//
	int i;
	i=0;
	int i_angle1024;	/* �ώZ�p�x */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += (ADD_ANGLE)) /* ��� */
	#undef ADD_ANGLE
	{
		SPRITE *h;
		h							= sprite_add_gu_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= BOSS_16_YOUSEI11+(i); 	i++;
			h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//
			h->callback_mover		= move_kaguya_doll;
			h->callback_loser		= lose_kaguya_doll;
			h->callback_hit_enemy	= callback_hit_zako;
	//
			h->xcenter256			= (src->cx256);
			h->ycenter256			= (src->cy256);
	//
			h->base_hp				= ((8*1024)-1); 	/* 8192==(8*1024) */
			h->base_score			= adjust_score_by_difficulty(score( 400000));	/* 40�� */
		//
			h->rotate_angle1024 	= i_angle1024;
			h->time_out 			= (0x01ff); 	/* �������� */
		}
	}
}
