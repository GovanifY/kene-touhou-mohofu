
#include "douchu.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"���C�h1",		"RWINGX",	rwingx
	-------------------------------------------------------
	�ォ�牡�ɓ��Ԋu�Ő��񂵂Č���A�v���C���[�Ɍ�����
	������x�߂Â��ƁA�e�u���Ĕ��]���ē�����B
---------------------------------------------------------*/
#if 0/* �߂� */
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define time_out			user_data03 	/* �������� */
#endif

#define time_out	user_data03
#define speed256	user_data04


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_meido1(SPRITE *src)
{
	src->time_out--;/*fps_factor*/
	if (0 > src->time_out)
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
	else
	if (1024 > src->time_out)
	{
		src->speed256	+= t256(0.1);/*�������Ȃ��� �v���C���[�ɓ˂�����ł���*/
		if (src->y256 >= t256(GAME_HEIGHT-150))
		{
		//	if (src->enemy_rank)
			{
				obj_send1->x256 					= src->x256;
				obj_send1->y256 					= src->y256;
				br.BULLET_REGIST_speed256			= t256(2.8);				/* �e�� */	 // //t256(1.0)+t256(src->enemy_rank)/*t256(3)+t256(src->enemy_rank)*/	/*���߂�*/
			//	br.BULLET_REGIST_speed256			= speed256; 				/* �e�� */
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* ���@�_���e */	/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			//	br.BULLET_REGIST_div_angle1024		= (0);						/* �_�~�[�p�x(���g�p) */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU8_00_AKA;		/* [�Ԓe] */ /* �e�O�� */
				br.BULLET_REGIST_n_way				= (1);						/* [1way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
			src->time_out = 1023;
			src->tmp_angleCCW1024 += (1024/2);	/* (1024/2) == [180/360 �x] ���]���ē����� */
			mask1024(src->tmp_angleCCW1024);
		//	src->speed256 = /*t256(6+src->enemy_rank)*/;/*���߂�*/
		}
	}
	else
	if (2048 > src->time_out)
	{
		/* �v���C���[�ɓ˂�����ł��� / schnell richtung player */
		src->time_out = 2047;
		tmp_angleCCW1024_jikinerai(obj_player, src);
	//	src->speed256	= /*t256(3+(src->enemy_rank>>1))*/ /*(4+src->enemy_rank)*/;/*���߂�*/
	}
	else
	if (2049 > src->time_out)/* 2048+1 */
	{
		if (0 < src->speed256)
		{
			src->speed256 -= (5);/*t256(0.1)*/ /*����*/
		}
	}
	else
	if (2109 > src->time_out)/* 2048+1+60 */
	{
		if ( src->speed256 < t256(1.0))
		{
			src->speed256 += (5);/*t256(0.1)*/ /*����*/
		}
	}
//
	/*�ȉ�rwingx.c�Ɠ���*/
/* CCW�̏ꍇ */
	src->x256 += ((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
	src->y256 += ((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_36_YOUSEI3_1+((((src->tmp_angleCCW1024)&(1024-1)))>>7);
	}
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_zako_meido1(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_36_YOUSEI3_1;	/*SP_ZAKO*/ /*_12_MEIDO1*/
		h->callback_mover		= move_meido1;
	//
		h->tmp_angleCCW1024 	= (0);/* ������ */		/* CCW�̏ꍇ */
	//
		h->speed256 			= t256(0);//t256(0.5);
		h->time_out 			= (2209);//(100);//20;
	}
}
