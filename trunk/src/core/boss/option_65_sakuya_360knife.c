
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
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
	#define target_x256 			user_data00 	/* �ڕWx���W */
	#define target_y256 			user_data01 	/* �ڕWy���W */
	#define toutatu_wariai256		user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define kougeki_anime_count 	user_data03 	/* �U���A�j���[�V�����p�J�E���^ */
	#define boss_time_out			user_data04 	/* �������� */
	#define boss_base_state777		user_data04 	/* ��������(boss_time_out�Ɠ���) */
//
	#define boss_spell_timer		user_data05 	/* �X�y������ */
#endif
#ifndef boss_time_out
	#define boss_time_out		user_data04 	/* �������� */
#endif

/*---------------------------------------------------------
	�q�������w �ړ�
---------------------------------------------------------*/

static void move_sakuya_doll_04_360knife(SPRITE *src)
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
			static u8 count_jjj;
			count_jjj++;
			if ((3-(cg_game_difficulty)) < count_jjj)
			{
				count_jjj = 0;
				/*---------------------------------------------------------
					360�i�C�t
					�����w�p
					�S�����ɐi�C�t�e������
				---------------------------------------------------------*/
				br.BULLET_REGIST_00_speed256				= (t256(1.2));					/*(speed256)*/
				br.BULLET_REGIST_02_VECTOR_angle1024		= (0);							/* �e���p�x */
			//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KNIFE20_06_YUKARI; 	/* [�s���N�i�C�t�e] */	/*BULLET_KNIFE20_04_AOI*/
				br.BULLET_REGIST_06_n_way					= (18); 						/* [18way] */
				br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/(18)); 			/* �����p�x([360/360]�x��18����) */
				bullet_regist_multi_vector_send1_xy_src(src);	/* �e��x256 y256 ���S���甭�e�B */
			}
		}
	}
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

/*static*/ void add_zako_sakuya_doll_04_360knife(SPRITE *src)
{
	SPRITE *h;
	h								= obj_add_01_teki_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		h->m_Hit256R				= ZAKO_ATARI16_PNG;
	//	h->type 					= TEKI_51_MAHOJIN1;
		h->type 					= BOSS_16_YOUSEI11;
		h->base_hp					= (9999);/* �|���Ȃ� */
		h->flags					= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//	h->color32					= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);
		h->color32					= MAKE32RGBA(0x88, 0x33, 0xff, 0x80); /* ���ۂ� */
		h->callback_mover			= move_sakuya_doll_04_360knife;
		/* �q�������w�A�z�u�ʒu */
		#if 1
		h->cx256					= src->cx256;
		h->cy256					= src->cy256-t256(16);/*��� ����ɔz�u*/
		#endif
		h->boss_time_out				= (64*8)+(16*64);	/* �������� */	/* 16[�e]���� */
	}
}
