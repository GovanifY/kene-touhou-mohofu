
#include "bullet_object.h"

/*---------------------------------------------------------
	�q�������w �e��
	-------------------------------------------------------
	���^�`����
	-------------------------------------------------------
	(���ʂɕ��G�������̂ŁA
		���[�U�[�����@�\�ƁA
		���^��`���@�\�ƁA
		360�i�C�t�����@�\�ƁA
	�𕪗�������)
---------------------------------------------------------*/

#define my_angle512 		tmp_angleCCW512 	/* �ێ��p�x[���^��`���ꍇ�Ɏg���p�x](doll_point_angle512) */

#define doll_vx256			vx256
#define doll_vy256			vy256


#define time_out			user_data02 		/* �o�ߎ��� */


/*---------------------------------------------------------
	�q�������w �ړ�
---------------------------------------------------------*/

static SPRITE hosi_position_obj;/* ���^�`���Ă���W�ʒu��ێ��B */
/*static*/extern void enemy_sp2_bullet_create01(SPRITE *src, int angle512, int time_out/**sd_nnn*/);
static void move_doll02(SPRITE *src)
{
	#if 1
	/* �����w�A�j���[�V���� */
	src->m_angleCCW512--;/* �E��� */
	mask512(src->m_angleCCW512);
	#endif
//
	src->time_out--;
	if ((64*8) < src->time_out)
	{
		/* ���^��`���� */
		if (0==((src->time_out)&0x0f))
		{
			src->my_angle512 += (512*2/5);
			mask512(src->my_angle512);
			#define DOLL_SPEED10		(4)/*8*/
			src->doll_vx256 = ((sin512((src->my_angle512))*/*t256*/(DOLL_SPEED10))/*>>8*/);/*fps_factor*/				/* CCW�̏ꍇ */
			src->doll_vy256 = ((cos512((src->my_angle512))*/*t256*/(DOLL_SPEED10))/*>>8*/);/*fps_factor*/
			#undef DOLL_SPEED10
		}
		else
		{
			/* CCW�̏ꍇ */
			enemy_sp2_bullet_create01(&hosi_position_obj,
				src->my_angle512,
			//	t256(0.015),
			/* �^����c�莞�� */	((src->time_out)&0x1ff)//((src->time_out)-(64*8))
			//	&(((BOSS06_DOLL_DATA *)src->data)->nnn)
			);
		}
		// ����
		hosi_position_obj.x256 += (src->doll_vx256);/*fps_factor*/
		hosi_position_obj.y256 += (src->doll_vy256);/*fps_factor*/
	}
	#if 1
	else
	if (0 > src->time_out)
	{
		src->base_hp		= (0);/* �|���� */
		src->type = SP_DELETE;/* �����܂� */
	}
	#endif
//
	#if 0
	//if (data->c->type==SP_DELETE) 		/*����ł͂��܂������Ȃ��ꍇ������*/
	if (0/*FLG_NO_DOLL*/==common_boss_flags)	/* �P���|���ƊF�j�󂳂��B */
	{
		src->base_hp		= (0);/* �|���� */
		src->type = SP_DELETE;
	}
	#endif
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

/*static*/ void add_zako_sakuya_doll_02_star(SPRITE *src)
{
	SPRITE *h;
	h							= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
//	h->type 					= TEKI_51_MAHOJIN1;
//	h->type 					= SP_MUTEKI;
	h->type 					= BOSS_16_YOUSEI11;
	h->base_hp					= (9999);/* �|���Ȃ� */
	h->flags					= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//	h->color32					= 0x00ffffff;	/*	obj_doll->alpha 		= 0x00;*/
//	h->color32					= 0x80ff3388;	/*	obj_doll->alpha 		= 0x00;*/ /* �g���ۂ� */
	h->color32					= 0x806633ff;	/*	obj_doll->alpha 		= 0x00;*/ /* �g���ۂ� */
	h->callback_mover			= move_doll02;
//
	/* �q�������w�A�z�u�ʒu */
	#if 1
	h->x256 					= (src->x256);
	h->y256 					= (src->y256)-t256(16);/*��� ����ɔz�u*/
	#endif
	/* �e�������n�߂�ʒu(���^�`���n�߂�ʒu) */
	hosi_position_obj.x256		= (src->x256)+(ra_nd()&0xfff);
	hosi_position_obj.y256		= (src->y256)+(ra_nd()&0xfff);/* ��� ��������`�� */
//
	h->time_out 				= ((64*8)+(5*16)+1);

	/* ���^��`������ */
	h->my_angle512				= 0;
//	h->doll_vx256				= t256(1.0);	/*-t256(0.125)*/ /*t256(1.0)*/
//	h->doll_vy256				= t256(0.0);	/* t256(2.0)*/ /*t256(0.0)*/
}
