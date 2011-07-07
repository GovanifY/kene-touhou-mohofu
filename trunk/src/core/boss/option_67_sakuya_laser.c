
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
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define boss_time_out		user_data03 	/* �������� */
#endif

#define my_difficulty		user_data04 		/* ��Փx�����p */

#define my_angle1024		tmp_angleCCW1024	/* �ێ��p�x[���[�U�[�̌��������ێ�����p�x] */


/*---------------------------------------------------------
	�q�������w �ړ�
---------------------------------------------------------*/

static void move_doll01_laser(SPRITE *src)
{
	check_boss_option_time_out(src);	/* ���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B */
//
	if (0x0ff < src->boss_time_out)/* 0x100�����͋��ʋK�i�㌂���Ȃ� */
	{
		if (0x00==(src->boss_time_out&0x10f))/* */
		{
			/* CCW�̏ꍇ */
			obj_send1->cx256							= (src->cx256);/* �����w�̒��S����e���� */
			obj_send1->cy256							= (src->cy256);/* �����w�̒��S����e���� */
			br.BULLET_REGIST_00_speed256						= ((src->boss_time_out&0xff)/*>>4*/<<3/*1/10*/)/**256*/;
			br.BULLET_REGIST_07_VECTOR_tomari_dan_delta256		= t256(0.02);
		//
			br.BULLET_REGIST_05_regist_type 					= REGIST_TYPE_03_TOMARI;
			const int aaa = (src->my_angle1024)+(src->boss_time_out+src->boss_time_out);
			br.BULLET_REGIST_02_VECTOR_angle1024				= src->my_angle1024;				br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024	= aaa+cv1024r((180+  0));	bullet_regist_vector(); //ra d2deg512((src->boss_time_out)>>7/*/100*/)
			br.BULLET_REGIST_02_VECTOR_angle1024				= src->my_angle1024-cv1024r((120)); br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024	= aaa+cv1024r((180+120));	bullet_regist_vector(); //ra d2deg512((src->boss_time_out)>>7/*/100*/)
			br.BULLET_REGIST_02_VECTOR_angle1024				= src->my_angle1024+cv1024r((120)); br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024	= aaa+cv1024r((180-120));	bullet_regist_vector(); //ra d2deg512((src->boss_time_out)>>7/*/100*/)
		}
		if (0x100==(src->boss_time_out&0x103))/* */
		{
			src->my_difficulty--;
			if (0 > src->my_difficulty)
			{
				src->my_difficulty = (1+3-(cg_game_difficulty));/* luna,��1/4��, hard ��1/8��, norma1/12��, easy��1/16��, */
			//
				voice_play(VOICE16_BOSS_KYUPIN, TRACK04_TEKIDAN);
				obj_send1->cx256 = (src->cx256);/* �����w�̒��S����e���� */
				obj_send1->cy256 = (src->cy256);/* �����w�̒��S����e���� */
						br.BULLET_REGIST_07_VECTOR_div_angle1024		= cv1024r(120); 			/* �p�x */
						br.BULLET_REGIST_04_bullet_obj_type 			= BULLET_HARI32_00_AOI; 	/* [ �e] */
						br.BULLET_REGIST_06_n_way						= (3);						/* [3way] */
						br.BULLET_REGIST_05_regist_type 				= REGIST_TYPE_00_MULTI_VECTOR;
				{unsigned int i;
					for (i=t256(5); i<t256(9); i+=t256(1))
					{
						/* CCW�̏ꍇ */
						br.BULLET_REGIST_00_speed256					= (i);						/* �e�� */
						//
						br.BULLET_REGIST_02_VECTOR_angle1024			= src->my_angle1024;
						bullet_regist_vector();
					}
				}
				/* deg_360_to_512(8.59) 8.59436717317284029518323968680208	PI*2/41.8879008478638666666666666666667 ra d2deg512(0.15)*/
				/* 24.44...  == 1024/41.8879008478638666666666666666667 */
				src->my_angle1024 -= (24);/* �E���(CCW�Ȃ̂�) */
			}
		}
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

/*static*/ void add_zako_sakuya_doll_01_laser(SPRITE *src)
{
	SPRITE *h;
	h						= obj_add_01_teki_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= BOSS_16_YOUSEI11;
		h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
		h->callback_mover		= move_doll01_laser;
		h->base_hp				= (9999);/* �|���Ȃ� */

	//	h->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);	/*	obj_doll->alpha 		= 0x00;*/
		h->color32				= MAKE32RGBA(0xff, 0x88, 0xaa, 0x80);	/*	obj_doll->alpha 		= 0x00;*/ /* �����ۂ� */
		/* �q�������w�A�z�u�ʒu */
		#if 1
		h->cx256				= (src->cx256);
		h->cy256				= (src->cy256)-t256(16);/*��� ����ɔz�u*/
		#endif
		h->my_angle1024 		= (0);
		h->my_difficulty		= (0);	/* ��Փx�����p */
	//
		h->boss_time_out		= (0x0800); 	/* �������� */
	}
}
