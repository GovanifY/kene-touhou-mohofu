
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
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
	#define time_out			user_data03 	/* �������� */
#endif
//	�{�X���ʋK�i�Ɠ���(boss.h�C���N���[�h���Ă����Ȃ��Ă��Ή�)
#ifndef time_out
	#define time_out			user_data03 	/* �������� */
#endif
//
#define my_difficulty		user_data04 		/* ��Փx�����p */

#define my_angle1024		tmp_angleCCW1024	/* �ێ��p�x[���[�U�[�̌��������ێ�����p�x] */


enum
{
//--- ���^�ȑO
	DS00 = 0,
	DS02,
	DS03,
};
/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_doll01_laser(SPRITE *src)
{
	check_boss_option_time_out(src);	/* ���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B */
//
	if (0x00==(src->time_out&0x10f))/* */
	{
		/* CCW�̏ꍇ */
		obj_send1->x256 							= (src->x256)+t256(8.0);/* 8.0[dots]�E */
		obj_send1->y256 							= (src->y256)+t256(4.0);/* 4.0[dots]�� */
		br.BULLET_REGIST_speed256					= ((src->time_out&0xff)/*>>4*/<<3/*1/10*/)/**256*/;
		br.BULLET_REGIST_tomari2_dan_delta256		= t256(0.02);
	//
		br.BULLET_REGIST_regist_type				= REGIST_TYPE_03_TOMARI;
		const int aaa = (src->my_angle1024)+(src->time_out+src->time_out);
		br.BULLET_REGIST_angle1024	= src->my_angle1024;				br.BULLET_REGIST_tomari2_dan_next_angle1024 	= aaa+cv1024r((180+  0));	bullet_regist_vector(); //ra d2deg512((src->time_out)>>7/*/100*/)
		br.BULLET_REGIST_angle1024	= src->my_angle1024-cv1024r((120)); br.BULLET_REGIST_tomari2_dan_next_angle1024 	= aaa+cv1024r((180+120));	bullet_regist_vector(); //ra d2deg512((src->time_out)>>7/*/100*/)
		br.BULLET_REGIST_angle1024	= src->my_angle1024+cv1024r((120)); br.BULLET_REGIST_tomari2_dan_next_angle1024 	= aaa+cv1024r((180-120));	bullet_regist_vector(); //ra d2deg512((src->time_out)>>7/*/100*/)
	}
	if (0x100==(src->time_out&0x103))/* */
	{
		src->my_difficulty--;
		if (0 > src->my_difficulty)
		{
			src->my_difficulty = (1+3-difficulty);/* luna,��1/4��, hard ��1/8��, norma1/12��, easy��1/16��,  */
		//
			voice_play(VOICE16_BOSS_KYUPIN, TRACK04_TEKIDAN);
			obj_send1->x256 = (src->x256)+t256(8.0);/* 8.0[dots]�E */
			obj_send1->y256 = (src->y256)+t256(4.0);/* 4.0[dots]�� */
				//	br.BULLET_REGIST_div_angle1024		= (0);						/* �_�~�[�p�x(���g�p) */
					br.BULLET_REGIST_bullet_obj_type	= BULLET_HARI32_00_AOI; 	/* [ �e] */
					br.BULLET_REGIST_n_way				= (1);						/* [1way] */
					br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
			{int i;
				for (i=t256(5); i<t256(9); i+=t256(1))
				{
					/* CCW�̏ꍇ */
					br.BULLET_REGIST_speed256			= (i);						/* �e�� */
					//
					br.BULLET_REGIST_angle1024			= src->my_angle1024;					bullet_regist_vector();
					br.BULLET_REGIST_angle1024			= src->my_angle1024-cv1024r(120);		bullet_regist_vector();
					br.BULLET_REGIST_angle1024			= src->my_angle1024+cv1024r(120);		bullet_regist_vector();
				}
			}
			/* deg_360_to_512(8.59) 8.59436717317284029518323968680208	PI*2/41.8879008478638666666666666666667 ra d2deg512(0.15)*/
			/* 24.44...  == 1024/41.8879008478638666666666666666667 */
			src->my_angle1024 -= (24);/* �E���(CCW�Ȃ̂�) */
		}
	}
//
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
	h						= sprite_add_gu_error();
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
		h->x256 				= (src->x256);
		h->y256 				= (src->y256)-t256(16);/*��� ����ɔz�u*/
		#endif
		h->my_angle1024 		= (0);
		h->my_difficulty		= (0);	/* ��Փx�����p */
	//
		h->time_out 			= (0x0800); 	/* �������� */
	}
}
