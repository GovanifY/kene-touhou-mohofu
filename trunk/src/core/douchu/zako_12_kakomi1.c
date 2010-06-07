
#include "douchu.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"�͗d��1",		"CUBE",
	-------------------------------------------------------

---------------------------------------------------------*/
#if 0/* �߂� */
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define time_out			user_data03 	/* �������� */
#endif

#define time_out		user_data03
#define angle1024		user_data04
#define radius256		user_data05

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_kakomi1(SPRITE *src)
{
	src->time_out--; /*fps_factor*/
	if (0 < src->time_out)
	{
		{
			src->radius256 -= t256(1.0);		/*fps_factor*/
		}
		if (0==(ra_nd()&(512-1)))	/* 1/512 �� 1/500 �̊m���Œe�ł� */ 			/*%500*/
		{
			obj_send1->x256 					= src->x256;
			obj_send1->y256 					= src->y256;
			br.BULLET_REGIST_speed256			= t256(1.0);				/* �e�� */
		//	br.BULLET_REGIST_speed256			= speed256; 				/* �e�� */
			br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* ���@�_���e */	/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		//	br.BULLET_REGIST_div_angle1024		= (0);						/* �_�~�[�p�x(���g�p) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU8_00_AKA;		/* [�Ԓe] */ /* �e�O�� */
			br.BULLET_REGIST_n_way				= (1);						/* [1way] */
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
			bullet_regist_vector();
		}
		if (0==(ra_nd()&(512-1)))	/* 1/512 �� 1/600 �̊m���m���ŃA�C�e���o�� */	/*%600*/
		{
			item_create(src, SP_ITEM_00_P001, 1, ITEM_MOVE_FLAG_06_RAND_XY);
		}
		src->angle1024 -= (2);		/*fps_factor*/	/*2*/	/* deg_360_to_512(3) �������� */
		mask1024(src->angle1024);
	}
	else
	{
		src->radius256 += t256(1.2/*2.0*/); 		/*fps_factor*/
		if (src->radius256 > t256(350.0))
		{
			src->type = SP_DELETE;	/* �����܂� */
		}
	}
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_54_CHOU1+((src->time_out&0x10)>>4);				/* �A�j���[�V���� */
	}
	src->x256	= ((sin1024((src->angle1024))*(src->radius256))>>8)+t256(GAME_WIDTH/2); /* CCW�̏ꍇ */
	src->y256	= ((cos1024((src->angle1024))*(src->radius256))>>8)+t256(GAME_HEIGHT/2);
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_zako_kakomi1(STAGE_DATA *l)
{
	SPRITE *h;
	h								= sprite_add_gu_error();
	if (NULL!=h)
	{
		add_zako_common(l, h);
		h->m_Hit256R				= ZAKO_ATARI16_PNG;
		h->type 					= TEKI_54_CHOU1;
		h->callback_mover			= move_kakomi1;
		h->callback_hit_enemy		= callback_hit_zako;
		//
		h->radius256				= t256(350);
		#if 1
		/* r30�݊� */
		h->angle1024				= (l->user_x + l->user_x);/*(i<<5)*/	//	/*360*/(1024/16)*i;
		#else
		/* r31 */
		h->angle1024				= (l->user_x);/*(i<<5)*/	//	/*360*/(1024/16)*i;
		#endif
		h->time_out 				= (295);
	}
}
//		/*h->base.*/h->base_hp		= (8*30)+/*enemy_rank+*/(1/*di fficulty*/<<(2+3));	/*1+lv+(di fficulty<<2)*/
//		/*h->base.*/h->base_score	= score(15*2)*(1+2/*enemy_rank*/);

/*
;[16����]
  0,  32,  64,	96,
128, 160, 192, 224,
256, 288, 320, 352,
384, 416, 448, 480,
[0==512,]
;[13����]
  0,   0, -0
 39,  40, -1
 78,  80, -2
118, 120, -2
157, 160, -3
196, 200, -4
236, 240, -4
275, 280, -5
314, 320, -6
354, 360, -6
393, 400, -7
432, 440, -8
472, 480, -8
*/
