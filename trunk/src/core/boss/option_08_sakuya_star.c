
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�q�������w �e��
	-------------------------------------------------------
	B.���^�`����
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
//	�{�X���ʋK�i�Ɠ���(boss.h�C���N���[�h���Ă����Ȃ��Ă��Ή�)
#ifndef boss_time_out
	#define boss_time_out		user_data03 	/* �������� */
#endif
//
#define my_angle1024		tmp_angleCCW1024	/* �ێ��p�x[���^��`���ꍇ�Ɏg���p�x] */

#define doll_vx256			vx256
#define doll_vy256			vy256


/*---------------------------------------------------------
	�q�������w �ړ�
---------------------------------------------------------*/

static SPRITE hosi_position_obj;/* ���^�`���Ă���W�ʒu��ێ��B */
/*static*/extern void bullet_crate_sakuya_hosi_gata(SPRITE *src);
static void move_doll02(SPRITE *src)
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
		/* ���^��`���� */
		if (0==((src->boss_time_out)&0x0f))
		{
			src->my_angle1024 += (1024*2/5);
			mask1024(src->my_angle1024);
			#define DOLL_SPEED10		(4)/*8*/
			src->doll_vx256 = ((sin1024((src->my_angle1024))*/*t256*/(DOLL_SPEED10))/*>>8*/);/*fps_factor*/ 			/* CCW�̏ꍇ */
			src->doll_vy256 = ((cos1024((src->my_angle1024))*/*t256*/(DOLL_SPEED10))/*>>8*/);/*fps_factor*/
			#undef DOLL_SPEED10
		}
		else
		{
			br.BULLET_REGIST_hosi_gata_angle1024		= ((src->my_angle1024))+(((src->boss_time_out)&0x0f)<<4/*3*/);
			br.BULLET_REGIST_hosi_gata_time_out 		= ((src->boss_time_out)&0x1ff); //((src->boss_time_out)-(64*8))
			br.BULLET_REGIST_hosi_gata_add_speed256 	= (((src->boss_time_out>>2)&0x3)+4);	/* �������x(��-��������) */
		//	br.BULLET_REGIST_hosi_gata_add_speed256 	= (((src->boss_time_out )&0xf)|0x03);	/* �������x(��-��������) */
/*
0123 3333 4444	0+4
4567 7777 5555	1+4
89ab bbbb 6666	2+4
cdef ffff 7777	3+4
*/
			/* CCW�̏ꍇ */
			bullet_crate_sakuya_hosi_gata(&hosi_position_obj);
		}
		// ����
		hosi_position_obj.cx256 += (src->doll_vx256);/*fps_factor*/
		hosi_position_obj.cy256 += (src->doll_vy256);/*fps_factor*/
	}
//
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

/*static*/ void add_zako_sakuya_doll_02_star(SPRITE *src)
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
	//	h->color32					= MAKE32RGBA(0x88, 0x33, 0xff, 0x80); /* �g���ۂ� */
		h->color32					= MAKE32RGBA(0xff, 0x33, 0x66, 0x80); /* �g���ۂ� */
		h->callback_mover			= move_doll02;
	//
		/* �q�������w�A�z�u�ʒu */
		#if 1
		h->cx256					= (src->cx256);
		h->cy256					= (src->cy256)-t256(16);/*��� ����ɔz�u*/
		#endif
		/* �e�������n�߂�ʒu(���^�`���n�߂�ʒu) */
		hosi_position_obj.cx256 	= (src->cx256)+(ra_nd()&0xfff);
		hosi_position_obj.cy256 	= (src->cy256)+(ra_nd()&0xfff);/* ��� ��������`�� */
	//
		h->boss_time_out			= ((64*8)+(5*16)+1);	/* �������� */

		/* ���^��`������ */
		h->my_angle1024 			= (0);
	//	h->doll_vx256				= t256(1.0);	/*-t256(0.125)*/ /*t256(1.0)*/
	//	h->doll_vy256				= t256(0.0);	/* t256(2.0)*/ /*t256(0.0)*/
	}
}
