
#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�q�������w �e��
	-------------------------------------------------------
	A.���[�U�[������
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

#define my_difficulty		user_data06 		/* ��Փx�����p */

#define my_angle1024		tmp_angleCCW1024	/* �ێ��p�x[���[�U�[�̌��������ێ�����p�x] */


/*---------------------------------------------------------
	�q�������w �ړ�
	-------------------------------------------------------
	�ʒu
	(r34) 120/360 �P�ʂŕ���(1����3����)����ƍr�߂��Ă�����ۂ��Ȃ�Ȃ��B
	�Œ�ł�6�����͂��ׂ��B 60/360 �P��(1����6����)�֕ύX�B
---------------------------------------------------------*/

static void move_doll_02_tama_oki(SPRITE *src)
{
	check_boss_option_time_out(src);	/* ���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B */
//
	if (0x0ff < src->boss_time_out)/* 0x100�����͋��ʋK�i�㌂���Ȃ� */
	{
		if (0x000==(src->boss_time_out&0x10f))/* */
		{
			/* CCW�̏ꍇ */
			send1_xy(src);	/* �e��x256 y256 ���S���甭�e�B */
			br.BULLET_REGIST_00_speed256						= (t256(2.0))+((src->boss_time_out&0xff)/*>>4*/<<3/*1/10*/)/**256*/;
			br.BULLET_REGIST_03_VECTOR_regist_type				= LEGACY_REGIST_TYPE_01_TOMARI;
			br.BULLET_REGIST_04_bullet_obj_type 				= (BULLET_MINI8_01_AKA+(0)); /*S P_BULLET*/ /*SP_LASER*/
			br.BULLET_REGIST_07_VECTOR_legacy_dan_delta256		= (1);//t256(0.02); /* �����萔 */
		//
			const u32 ppp = (src->my_angle1024) + ((src->boss_time_out)<<2);
			u32 jj;
			for (jj=(0); jj<(1024); jj+=((u32)(1024/6)))
			{
				br.BULLET_REGIST_02_VECTOR_angle1024					= (ppp+jj);
				br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024	= (ppp+jj) + ((u32)(1024/6));
				bullet_regist_legacy_vector_direct();
			}
			//
				/* deg_360_to_512(8.59) 8.59436717317284029518323968680208	PI*2/41.8879008478638666666666666666667 ra d2deg512(0.15)*/
				/* 24.44...  == 1024/41.8879008478638666666666666666667 */
				src->my_angle1024 -= (24);/* �E���(CCW�Ȃ̂�) */
		}
	//	if (0x100==(src->boss_time_out&0x103))/* */
	//	{
	//		src->my_difficulty--;
	//		if (0 > src->my_difficulty)
	//		{
	//			src->my_difficulty = (1+3-(cg_game_difficulty));/* luna,��1/4��, hard ��1/8��, norma1/12��, easy��1/16��, */
	//		}
	//	}
	}
	//
	set_timeout_alpha(src);
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

/*---------------------------------------------------------
	�q�������w ���� 	�q�������w�O���t�B�b�N����
---------------------------------------------------------*/

/*static*/ void add_zako_sakuya_doll_02_tama_oki(SPRITE *src)
{
	SPRITE *h;
	h						= obj_add_01_teki_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= BOSS_16_YOUSEI11;
	//	h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
		h->flags				&= (~SP_FLAG_COLISION_CHECK);	/* ���� */
		h->callback_mover		= move_doll_02_tama_oki;
		h->base_hp				= (9999);/* �|���Ȃ� */

	//	h->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);	/*	obj_doll->alpha 		= 0x00;*/
	//	h->color32				= MAKE32RGBA(0xff, 0x88, 0xaa, 0x80);	/*	obj_doll->alpha 		= 0x00;*/ /* �����ۂ� */
		h->color32				= MAKE32RGBA(0xff, 0x55, 0x66, 0x77);	/*	obj_doll->alpha 		= 0x00;*/ /*  ���ۂ� */
		/* �q�������w�A�z�u�ʒu */
		#if 1
		h->cx256				= (src->cx256);
		h->cy256				= (src->cy256);/* �����ʒu�ɔz�u */
		#endif
		h->my_angle1024 		= (0);
		h->my_difficulty		= (0);	/* ��Փx�����p */
	//
		h->boss_time_out		= (0x0400); 	/* (1024)�������� */
	}
}
