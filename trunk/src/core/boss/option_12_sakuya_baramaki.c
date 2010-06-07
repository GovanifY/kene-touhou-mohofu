
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	��� �΂�T���e �I�v�V����
	-------------------------------------------------------
	�߂�1:������(5)�e
		�Ēe					�ΕĒe
		�E���					�����
		(+90�x) 				(-45�x)

			�ΕĒe			�Ēe
			�����			�E���
			(+45�x) 		(0�x)
	-------------------------------------------------------
	�߂�2:�����N�i�C(6)�e	[���̕ǂŔ��˂���]
		�ԃN�i�C�e				�N�i�C�e
		�E���					�����

			�N�i�C�e		�ԃN�i�C�e
			�����			�E���
---------------------------------------------------------*/
#if 0/*����*/
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define time_out			user_data03 	/* �������� */
#endif


#define shot_angle1024		user_data04 	/* �V���b�g����������ێ� */


/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_sakuya_baramaki(SPRITE *src)
{
	bakuhatsu_add_type_ddd(src, BAKUHATSU_FIRE08);
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
}


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_sakuya_baramaki(SPRITE *src)
{
	check_boss_option_time_out(src); 	/* ���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B */
//
	#define AO_OR_AKA	(src->target_x256&1)/* [�ԏ��] */
//
	/*	���� & ��镗�A��]�V���b�g */
	{
		if (0 == ((src->time_out)&0x07))	/* ���̃V���b�g�����܂ł̊Ԋu�A���ԁB */
		{
			if (0 < src->vvv256 )
			{
				src->vvv256 -= (1);
			}
		//
			src->shot_angle1024 += ((AO_OR_AKA)?(-(1024/18)):((1024/18)));	/* �V���b�g�����������A��]������B */
		//
			obj_send1->x256 					= (src->x256)+t256(20.0);/* 6.0[dots]�E */
			obj_send1->y256 					= (src->y256)+t256(14.0);/* 4.0[dots]�� */
			br.BULLET_REGIST_speed256			= (t256(1.5))+(((difficulty)<<6));		/* �e�� */
			br.BULLET_REGIST_angle1024			= (src->shot_angle1024);				/* */
			br.BULLET_REGIST_div_angle1024		= (int)(1024/160);						/* �����e */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_00_AOI+(AO_OR_AKA);	/* [�ԃN�i�C�e] */
			br.BULLET_REGIST_n_way				= (2+difficulty);						/* [2-5way] */
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
			bullet_regist_vector();
		}
	}
	#if 1
	/* �����w�A�j���[�V���� */
//	src->m_angleCCW1024--;/* �E��� */
	src->m_angleCCW1024 += (((AO_OR_AKA)<<2)-2);
	mask1024(src->m_angleCCW1024);
	#endif
	if (0x1ff > src->time_out)
	{
		src->color32		= (src->color32 & 0x00ffffff) | ((src->time_out<<(23))&0xff000000);
	}
	/* �I�v�V�����ʒu�A�ړ� */
	/* �ڕW��ݒ肵�A�U���ړ� */
//	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_sakuya_baramaki(SPRITE *src)
{
	int i;
	for (i=0; i<(4); i++) /* 4�� */
	{
		SPRITE *h;
		h							= sprite_add_gu_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= BOSS_16_YOUSEI11;
			h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//
			h->callback_mover		= move_sakuya_baramaki;
			h->callback_loser		= lose_sakuya_baramaki;
			h->callback_hit_enemy	= callback_hit_zako;
	//
			h->x256 				= ((src->x256) & 0xfffffffe);/* [�ԏ��]�C���^�[���[�u�p�r�b�g��1�r�b�g�m�� */
			h->y256 				= (src->y256);
			h->vvv256				= t256(1.0);
		//
			{
				const s8 locate_xy_table[(4+4)] =
				{	/* �� */	/* �� */	/* �� */	/* �� */
					(-90),		(-120), 	(+120), 	(+90),	// x0, x1, x2, x3
					(+90),		(0),		(0),		(+90),	// y0, y1, y2, y3
				};
				h->target_x256	= (h->x256)+((locate_xy_table[i  ])<<8) + (i&1);/* [�ԏ��]���C���^�[���[�u */
				h->target_y256	= (h->y256)+((locate_xy_table[i+4])<<8);
			}
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
			h->shot_angle1024		= (0);
			h->time_out 			= (0x03ff); 	/* �������� */
		}
	}
}
