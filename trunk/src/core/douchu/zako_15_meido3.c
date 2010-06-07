
#include "douchu.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"���C�h3",		"CURVER",	rwingx_curver
	-------------------------------------------------------
�ォ�猻���S����n��`���ĉ���

����
��
����������
		��
����������
��
����������
		��
����������
	-------------------------------------------------------
	�o�O����B
---------------------------------------------------------*/
#if 0/* �߂� */
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define time_out			user_data03 	/* �������� */
#endif

#define clip_left256		user_data00
#define clip_right256		user_data01
#define state				user_data02
#define speed256			user_data03
#define turn_count			user_data04
#define turnspeed1024		user_data05

static int static_last = 0; 	/* �o��ʒu�̐؂�ւ� */


/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

//static void lose_meido3(SPRITE *src)
//{
//	destoroy[static_last]++;
//	if ( (N UM_OF_ENEMIES-1) < destoroy[static_last]/*all_destoroy*/)
//	{
//		destoroy[static_last] = 0;
//		item_create(src, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
//	}
//}

/*---------------------------------------------------------
	�G�U��
---------------------------------------------------------*/

static void shot_meido3(SPRITE *src)
{
//	if (0 < /*src->*/enemy_rank)
	{
		obj_send1->x256 					= (src->x256);
		obj_send1->y256 					= (src->y256);
		br.BULLET_REGIST_speed256			= t256(2.0);
		br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
		br.BULLET_REGIST_div_angle1024		= (int)(1024/24);
		br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_01_AKA;
		br.BULLET_REGIST_n_way				= (difficulty<<1);			/*[n]*/
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_vector();
	}	/* easy�ȊO�͑_���e������ */
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

enum
{
	SS00 = 0,
	SS01,
	SS02,
	SS03,
	SS04,
};

static void move_meido3(SPRITE *src)
{
	switch (src->state)
	{
	case SS00:	/* ������ŉ�]�� */
	//	src->tmp_angleCCW1024 -= src->turnspeed1024;/*fps_factor*/		/* CW�̏ꍇ */
		src->tmp_angleCCW1024 += src->turnspeed1024;/*fps_factor*/		/* CCW�̏ꍇ */
		mask1024(src->tmp_angleCCW1024);
		/* �E(�΂ߏ�)���������� */
		/* CW�̏ꍇ */
	//	if ( (1024-32) < src->tmp_angleCCW1024 )		/*eps*/ /*<= cv1024r(0)*/	//cv1024r(10/*90*/ /*-90*/ /*360*/ /*-10*/
		/* CCW�̏ꍇ */
		if ( 0==(0x200 & src->tmp_angleCCW1024) )/* (cv1024r(180) > src->tmp_angleCCW1024) */
		{
			if ( (256+32) < src->tmp_angleCCW1024 ) 	/*eps*/ /*<= cv1024r(0)*/	//cv1024r(10/*90*/ /*-90*/ /*360*/ /*-10*/
			{
			//	src->tmp_angleCCW1024 = cv1024r(0/*360*/ /*-10*/)/*cv1024r(360)*/;
				src->state++/*SS01*/;
				shot_meido3(src);	/* easy�ȊO�͑_���e������ */
			}
		}
		break;
	case SS01:	/* �E(�΂ߏ�)�ֈړ��� */
		if (0 < src->turn_count)
		{
			if (src->x256 >= src->clip_right256)	{ src->state++/*SS02*/; 	src->turn_count--; }
		}
		else	/* �ޏꒆ */
		{
			gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
		}
		break;
	case SS02:	/* �E����ŉ�]�� */
	//	src->tmp_angleCCW1024 += src->turnspeed1024;/*fps_factor*/		/* CW�̏ꍇ */
		src->tmp_angleCCW1024 -= src->turnspeed1024;/*fps_factor*/		/* CCW�̏ꍇ */
		mask1024(src->tmp_angleCCW1024);
		/* ��(�΂ߏ�)���������� */
	//	if ( (512+32) < src->tmp_angleCCW1024 ) 			//cv1024r(180 /*270*/ /*90+*/ /*180-90-90*/ /*+10*/)
		/* CCW�̏ꍇ */
		if ( 0!=(0x200 & src->tmp_angleCCW1024) )			/* (cv1024r(180) < src->tmp_angleCCW1024) */
		{
			if ( (512+256-32) > src->tmp_angleCCW1024 ) 	//cv1024r(180 /*270*/ /*90+*/ /*180-90-90*/ /*+10*/)
			{
			//	src->tmp_angleCCW1024 = cv1024r(180/*+10*/);
				src->state++/*SS03*/;
				shot_meido3(src);	/* easy�ȊO�͑_���e������ */
			}
		}
		break;
	case SS03:	/* ��(�΂ߏ�)�ֈړ��� */
		if (0 < src->turn_count)
		{
			if (src->x256 <= src->clip_left256) 	{	src->state=0;	src->turn_count--;	}
		}
		else	/* �ޏꒆ */
		{
			gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
		}
		break;
	}
	/*�ȉ�rwingx.c�Ɠ���*/
	src->x256+=((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/ /* CCW�̏ꍇ */
	src->y256+=((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_36_YOUSEI3_1+((((src->tmp_angleCCW1024)&(1024-1)))>>7);
	}
/*���ݎd�l�エ������*/
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_zako_meido3(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_36_YOUSEI3_1;	/*SP_ZAKO*/ /*_14_MEIDO3*/
		h->callback_mover		= move_meido3;
		h->speed256 			= t256(1.5);		/*3+difficulty*/ /*�͂₷��*/
		h->turnspeed1024		= (7*2);			/*(8+difficulty+difficulty)*/
		if (t256(GAME_WIDTH/2) > h->x256 )
		{
			h->tmp_angleCCW1024 	= cv1024r(90-10/*360-10*/);
		//	h->x256 				= -t256(32);
			static_last 			= 0;
		}
		else
		{
			h->tmp_angleCCW1024 	= cv1024r(180+90+10/*360-170*/);
		//	h->x256 				= t256(GAME_WIDTH);
			static_last 			= 1;
		}

	//	h->clip_left256 	= (t256(static_last<<7)+t256(static_last<<6)+t256(32)) /*50*/;			/* 32,	224(128+64+32)	*/
	//	h->clip_right256	= (t256(static_last<<7)+t256(static_last<<6)+t256(32))+t256(128-32);	/* 128, 320(256+64)   */	 //  /*GAME_WIDTH-50-s->w*/
		h->clip_left256 	= (t256(static_last<<7)+t256(static_last<<6)+t256(32)) /*50*/;			/* 32,	224(128+64+32)	*/
		h->clip_right256	= (t256(static_last<<7)+t256(static_last<<6)+t256(32))+t256(128-32-32); /* 128, 320(256+64)   */	 //  /*GAME_WIDTH-50-s->w*/
		h->state			= (static_last<<1);/*SS00*/
		h->turn_count		= 4;
	}
}

//	h->tmp_angleCCW1024 		= (256+128/*0*/);	/*256+16*/ /*cv1024r(270)*/;
