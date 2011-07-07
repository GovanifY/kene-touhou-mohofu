
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�q�������w �e��
	-------------------------------------------------------
	�p�`�F���[�U�[
---------------------------------------------------------*/
#if 0/*����*/
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define boss_time_out		user_data03 	/* �������� */
#endif
//	�{�X���ʋK�i�Ɠ���(boss.h�C���N���[�h���Ă����Ȃ��Ă��Ή�)
#ifndef boss_time_out
	#define boss_time_out		user_data03 	/* �������� */
#endif
//

#define target_x256 	user_data00 	/* ��Œ� x�ʒu */
#define target_y256 	user_data01 	/* ��Œ� y�ʒu */
//#define time_out		user_data03 	/* �o�ߎ��� */
#define radius			user_data04 	/* �~�̔��a */
#define rotate			user_data05 	/* ��]���� */



#define my_angle1024		tmp_angleCCW1024	/* �ێ��p�x[���[�U�[�̌��������ێ�����p�x] */


/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_doll01_laser(SPRITE *src)
{
//	check_boss_option_time_out(src);	/* ���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B */
//
	src->boss_time_out--;
	if (0 > src->boss_time_out)
	{
		src->jyumyou		= JYUMYOU_NASI;
	}

	if ((256-85)<(src->boss_time_out))
	{
		src->radius++;
	}
	else
	{
	//	if (0x00==(src->boss_time_out&0x03))/* */
		{
//			voice_play(VOICE16_BOSS_KYUPIN, TRACK04_TEKIDAN);
			obj_send1->cx256							= (src->cx256);/* �����w�̒��S����e���� */
			obj_send1->cy256							= (src->cy256);/* �����w�̒��S����e���� */

			br.BULLET_REGIST_00_speed256				= t256(2.0);				/* �e�� */
			br.BULLET_REGIST_01_speed_offset			= t256(0);/*(�e�X�g)*/
			br.BULLET_REGIST_02_angle65536				= ((src->my_angle1024)<<6);
			br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_HARI32_00_AOI; 	/* [ �j�e] */
		//����br.BULLET_REGIST_05_regist_type			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
			br.BULLET_REGIST_06_n_way					= (3);						/* [3way] */
			br.BULLET_REGIST_07_div_angle65536			= (65536/3);				/* �����p�x */
			/* CCW�̏ꍇ */
			tama_system_regist_katayori_n_way();/* (r33-) */
			src->my_angle1024 -= (src->rotate);/* ��]���� */
			mask1024(src->my_angle1024);
		}
	}
//
			src->cx256 = src->target_x256 + ((sin1024((src->my_angle1024))*src->radius));/*fps_factor*/
			src->cy256 = src->target_y256 + ((cos1024((src->my_angle1024))*src->radius));/*fps_factor*/
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

/*---------------------------------------------------------
	�q�������w ���� 	�q�������w�O���t�B�b�N����
---------------------------------------------------------*/

/*static*/ void add_zako_pache_laser(SPRITE *src)
{
	static u8 aaa_count;
	aaa_count++;
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
		h->target_x256			= (src->cx256);
		h->target_y256			= (src->cy256);
		#endif
		h->radius				= (0);
		if (0==(aaa_count & 0x02))
		{
			h->my_angle1024 		= (1024/6);
			h->rotate				= (1);
		}
		else
		{
			h->my_angle1024 		= (1024)-(1024/6);
			h->rotate				= (-1);
		}
	//
		h->boss_time_out		= (256);	/* �������� */
	}
}
