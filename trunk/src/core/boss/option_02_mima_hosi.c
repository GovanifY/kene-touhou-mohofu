
#include "bullet_object.h"

/*---------------------------------------------------------
	���� �I�v�V����
	-------------------------------------------------------


---------------------------------------------------------*/

#define xcenter256			user_data00
#define ycenter256			user_data01
#define time_out			user_data02

#define rotate_angle512 	user_data03 	/* �{�X�𒆐S�Ƃ��āA��]�p�x�B(��CCW512�`��) */
#define fire_wait1			user_data04 	/* */
#define shot_angle512		user_data05 	/* */


/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_mima_doll(SPRITE *src)
{
	bakuhatsu_add_type_ddd(src, BAKUHATSU_FIRE08);
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
}


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_mima_doll(SPRITE *src)
{
	/* �I�v�V�����ʒu�A��]�ړ� */
	src->rotate_angle512++;
	mask512(src->rotate_angle512);
	#define HANKEI_45_DOT (45)				/* ���a */
	src->x256 = src->xcenter256 + ((sin512((src->rotate_angle512))*(HANKEI_45_DOT)));	/*CCW*/
	src->y256 = src->ycenter256 + ((cos512((src->rotate_angle512))*(HANKEI_45_DOT)));
	#undef HANKEI_45_DOT
//
	{
		/*	���� & ��镗�A��]�V���b�g */
		src->fire_wait1--;
		if (0 > src->fire_wait1)
		{
			src->fire_wait1 = (8);				/* ���̃V���b�g�����܂ł̊Ԋu�A���ԁB */
			src->shot_angle512 += (512/18); 	/* �V���b�g�����������A��]������B */
		//
			send1_obj->x256 = src->x256+t256(6.0);/* 6.0[dots]�E */
			send1_obj->y256 = src->y256+t256(4.0);/* 4.0[dots]�� */
			send1_obj->BULLET_REGIST_speed256			=	(t256(1.5))+(((difficulty)<<6));	/* �e�� */
			send1_obj->BULLET_REGIST_angle512			=	(src->shot_angle512);				/* */
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/160); 					/* �����e */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KUNAI12_01_AKA;				/* [�ԃN�i�C�e] */
			send1_obj->BULLET_REGIST_n_way				=	(2+difficulty); 					/* [2-5way] */
			bullet_regist_basic();
		}
	}
//
	#if 1
	src->time_out--;
	if (0 > src->time_out)/* �X�y�J�V�X�e���̓s���ŗv��B */
	{
		/* ������|���ƊF�j�󂳂��B */
		if (0==draw_boss_hp_value)
		{
			src->type = SP_DELETE;
		}
	}
	#endif
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_mima_dolls(SPRITE *src)
{
//	#define ADD_ANGLE (171) 	/* 1����3���������p�x�A170.66 == 512/3 */
//	#define ADD_ANGLE (85)		/* 1����6���������p�x�A 85.33 == 512/6 */
	#define ADD_ANGLE (102) 	/* 1����5���������p�x�A102.40 == 512/5 */
//	const int add_angle = (ADD_ANGLE);	/* ���Z�p�x */
//
	int i_angle;	/* �ώZ�p�x */
	for (i_angle=0; i_angle<(512);	i_angle += (ADD_ANGLE)) /* ��� */
	#undef ADD_ANGLE
	{
		SPRITE *h;
		h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		h->type 				= BOSS_16_YOUSEI11;
		h->flags				= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//
		h->callback_mover		= move_mima_doll;
		h->callback_loser		= lose_mima_doll;
		h->callback_hit_enemy	= callback_hit_zako;
//
		h->xcenter256			= (src->x256);
		h->ycenter256			= (src->y256);
//
		h->base_hp				= (8192-1);
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
		h->rotate_angle512		= i_angle;
		h->fire_wait1			= (0);
		h->shot_angle512		= (0);
		h->time_out 			= (100);/*�悭�킩��Ȃ�(�X�y�J�V�X�e���̂����ł��܂������Ȃ�)*/
	}
}
