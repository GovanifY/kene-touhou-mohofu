
#include "douchu.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"���C�h4",		"CIR",	rwingx_cir
	-------------------------------------------------------
	�E��������c���Ō���A���Ɍ������B���Ő���B
	��������㏸���A�E���Ɍ������B��Ő���B
	�E�����牺�Ɍ������B��ʊO�֑ޏ�B
---------------------------------------------------------*/
#if 0/* �߂� */
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define time_out			user_data03 	/* �������� */
#endif

#define state		user_data04
#define speed256	user_data05

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_meido4(SPRITE *src)
{
	item_create(src, SP_ITEM_05_TENSU, 1, ITEM_MOVE_FLAG_06_RAND_XY);
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

static void move_meido4(SPRITE *src)
{
	switch (src->state)
	{
	case SS00: /* ���Ɍ����� / down */
		if (src->y256 > t256(128))			/* ���S�t�߂��z���ĉ���������A�������x�𗎂Ƃ� */	/*200*/
		{
			src->state++;/*SS01*/
			src->speed256 = t256(2.5);		/* ���x�𗎂Ƃ� */	/*3.0*/
		}
		break;
	case SS01: /* ���Ő��� / turn */
#if 0
/* CW�̏ꍇ */
		src->angle1024 -= (10);				/*4*/ /*cv1024r(2)*/ /*fps_factor*/
		mask1024(src->angle1024);
		if ((src->angle1024 >= cv1024r((270-3))) &&		/* �قڐ^����������� */
			(src->angle1024 <= cv1024r((270+3))))
		{
			src->angle1024 = cv1024r((270));		/* �^��ɋ����␳ */
			src->state++;/*SS02*/
			src->speed256 = t256(2.0);		/*8.0*/ /* ���x�𗎂Ƃ� */
		}
#else
/* CCW�̏ꍇ */
		src->tmp_angleCCW1024 += (10);		/*4*/ /*cv1024r(360-2)*/ /*fps_factor*/
		mask1024(src->tmp_angleCCW1024);
		if ( src->tmp_angleCCW1024 > cv1024r((180))	)		/* �قڐ^����������� */
		{
			src->tmp_angleCCW1024 = cv1024r((180));		/* �^��ɋ����␳ */
			src->state++;/*SS02*/
			src->speed256 = t256(2.0);		/*8.0*/ 	/* ���x�𗎂Ƃ� */
		}
#endif
		break;
	case SS02: /* up */
		if (src->y256 < t256(64/*50*/)) 	/* ���S�t�߂��z���ďオ������A�������x�𗎂Ƃ� */
		{
			src->state++;/*SS03*/
			src->speed256 = t256(1.5);		/*3.0*/
		}
		break;
	case SS03: /* turn */
#if 0
/* CW�̏ꍇ */
		src->angle1024 += (10);				/*4*/ /*cv1024r(2)*/ /*fps_factor*/
		mask1024(src->angle1024);
		if ((src->angle1024 >= cv1024r((90-3))) &&		/* �قڐ^������������ */
			(src->angle1024 <= cv1024r((90+3))))
		{
			src->angle1024 = cv1024r((90));		/* �^���ɋ����␳ */
			src->state++;/*=SS04*/
			src->speed256 = t256(1.0);	/*6.0*/
		}
#else
/* CCW�̏ꍇ */
		src->tmp_angleCCW1024 -= (10);		/*2*/ /*cv1024r(360-2)*/ /*fps_factor*/
		mask1024(src->tmp_angleCCW1024);
		if (src->tmp_angleCCW1024 > cv1024r(270) ) 	/* �قڐ^������������ */
		{
			src->tmp_angleCCW1024 = cv1024r((0));		/* �^���ɋ����␳ */
			src->state++;/*=SS04*/
			src->speed256 = t256(1.0);	/*6.0*/
		}
#endif
		break;
	case SS04: /* down */
		src->speed256 += t256(0.1);/*����*/
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
		break;
	}
	/*�ȉ�rwingx.c�Ɠ���*/
	src->x256+=((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/ 	/* CCW�̏ꍇ */
	src->y256+=((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_36_YOUSEI3_1+((((src->tmp_angleCCW1024)&(1024-1)))>>7);
	}
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_zako_meido4(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_36_YOUSEI3_1;/*SP_ZAKO*/ /*_15_MEIDO4*/
		h->callback_mover		= move_meido4;
		h->callback_loser		= lose_meido4;
		h->tmp_angleCCW1024		= cv1024r(0);		/* �^�� */	/* CCW�̏ꍇ */
		h->speed256 			= t256(3.0);	/*6.0*/
		h->state				= SS00;
	}
}
//	h->callback_hit_enemy	= callback_hit_zako;
