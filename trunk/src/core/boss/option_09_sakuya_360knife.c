
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�q�������w �e��
	-------------------------------------------------------
	C.360�i�C�t������
	-------------------------------------------------------
	(���ʂɕ��G�������̂ŁA
		A.���[�U�[�����@�\�ƁA
		B.���^��`���@�\�ƁA
		C.360�i�C�t�����@�\�ƁA
	�𕪗�������)
---------------------------------------------------------*/
#if 0/*����*/
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define boss_time_out		user_data03 	/* �������� */
#endif
#ifndef boss_time_out
	#define boss_time_out		user_data03 	/* �������� */
#endif

/*---------------------------------------------------------
	�q�������w �ړ�
---------------------------------------------------------*/

static void move_doll03(SPRITE *src)
{
	check_boss_option_time_out(src);	/* ���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B */
//
	#if 1
	/* �����w�A�j���[�V���� */
	src->rotationCCW1024--;/* �E��� */
	mask1024(src->rotationCCW1024);
	#endif
//
	if ((64*8) < src->boss_time_out)
	{
		if (0==((src->boss_time_out)&(64-1)))/* 64���1�񔭒e���� */
		{
			static int count_jjj;
			count_jjj++;
			if ((3-difficulty) < count_jjj)
			{
				count_jjj = 0;
				/*---------------------------------------------------------
					360�i�C�t
					�����w�p
					�S�����ɐi�C�t�e������
				---------------------------------------------------------*/
				obj_send1->cx256 = (src->cx256)+t256(8.0);/* 8.0[dots]�E */
				obj_send1->cy256 = (src->cy256)+t256(4.0);/* 4.0[dots]�� */
				br.BULLET_REGIST_speed256			= (t256(1.2));					/*(speed256)*/
				br.BULLET_REGIST_angle1024			= (0);							/* �e���p�x */
				br.BULLET_REGIST_div_angle1024		= (int)(1024/(18)); 			/* �����p�x([360/360]�x��18����) */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_06_YUKARI; 	/* [�s���N�i�C�t�e] */	/*BULLET_KNIFE20_04_AOI*/
				br.BULLET_REGIST_n_way				= (18); 						/* [18way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
	}
//
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

/*static*/ void add_zako_sakuya_doll_03_360knife(SPRITE *src)
{
	SPRITE *h;
	h								= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		h->m_Hit256R				= ZAKO_ATARI16_PNG;
	//	h->type 					= TEKI_51_MAHOJIN1;
		h->type 					= BOSS_16_YOUSEI11;
		h->base_hp					= (9999);/* �|���Ȃ� */
		h->flags					= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//	h->color32					= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);
		h->color32					= MAKE32RGBA(0x88, 0x33, 0xff, 0x80); /* ���ۂ� */
		h->callback_mover			= move_doll03;
		/* �q�������w�A�z�u�ʒu */
		#if 1
		h->cx256 					= src->cx256;
		h->cy256 					= src->cy256-t256(16);/*��� ����ɔz�u*/
		#endif
		h->boss_time_out				= (64*8)+(16*64);	/* �������� */	/* 16[�e]���� */
	}
}
