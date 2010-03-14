
#include "douchu.h"

/*---------------------------------------------------------
	"���C�h4",		"CIR",	rwingx_cir
	-------------------------------------------------------
	�E��������c���Ō���A���Ɍ������B���Ő���B
	��������㏸���A�E���Ɍ������B��Ő���B
	�E�����牺�Ɍ������B��ʊO�֑ޏ�B
---------------------------------------------------------*/

#define state		user_data02
#define speed256	user_data03

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

static void move_meido4(SPRITE *src)
{
	switch (src->state)
	{
	case 0: /* ���Ɍ����� / down */
		if (src->y256 > t256(128/*200*/)) 	/* ���S�t�߂��z���ĉ���������A�������x�𗎂Ƃ� */
		{
			src->state++/*=1*/;
			src->speed256=t256(2.5/*3.0*/);/* ���x�𗎂Ƃ� */
		}
		break;
	case 1: /* ���Ő��� / turn */
#if 0
/* CW�̏ꍇ */
		src->angle512 -= ((5))/*2*/ /*deg_360_to_512(2)*/ ;/*fps_factor*/
	//	if (src->angle512  <	deg_360_to_512(0))
	//	{	src->angle512  += deg_360_to_512(360);	}
		mask512(src->angle512);
		if ((src->angle512 >= deg_360_to_512((270-3))) &&		/* �قڐ^����������� */
			(src->angle512 <= deg_360_to_512((270+3))))
		{
			src->angle512 = deg_360_to_512((270));		/* �^��ɋ����␳ */
			src->state++/*=2*/;
			src->speed256=t256(2.0/*8.0*/);/* ���x�𗎂Ƃ� */
		}
#else
/* CCW�̏ꍇ */
		src->tmp_angleCCW512 += ((5))/*2*/ /*deg_360_to_512CCW(360-2)*/ ;/*fps_factor*/
	//	if (src->tmp_angleCCW512  <	deg_360_to_512CCW(360-0))
	//	{	src->tmp_angleCCW512  += deg_360_to_512CCW(360-360);	}
		mask512(src->tmp_angleCCW512);
		if ( src->tmp_angleCCW512 > deg_360_to_512CCW((180)) 		)		/* �قڐ^����������� */

		{
			src->tmp_angleCCW512 = deg_360_to_512CCW((180));		/* �^��ɋ����␳ */
			src->state++/*=2*/;
			src->speed256=t256(2.0/*8.0*/);/* ���x�𗎂Ƃ� */
		}
#endif
		break;
	case 2: /* up */
		if (src->y256 < t256(64/*50*/))	/* ���S�t�߂��z���ďオ������A�������x�𗎂Ƃ� */
		{
			src->state++/*=3*/;
			src->speed256=t256(1.5/*3.0*/);
		}
		break;
	case 3: /* turn */
#if 0
/* CW�̏ꍇ */
		src->angle512 += ((5))/*2*/ /*deg_360_to_512(2)*/ ;/*fps_factor*/
	//	if (src->angle512 < deg_360_to_512(0))
	//	{	src->angle512 += deg_360_to_512(360); 	}
		mask512(src->angle512);
		if ((src->angle512 >= deg_360_to_512((90-3))) &&		/* �قڐ^������������ */
			(src->angle512 <= deg_360_to_512((90+3))))
		{
			src->angle512 = deg_360_to_512((90));		/* �^���ɋ����␳ */
			src->state++/*=4*/;
			src->speed256=t256(1.0/*6.0*/);
		}
#else
/* CCW�̏ꍇ */
		src->tmp_angleCCW512 -= ((5))/*2*/ /*deg_360_to_512CCW(360-2)*/ ;/*fps_factor*/
	//	if (src->tmp_angleCCW512 < deg_360_to_512CCW(360-0))
	//	{	src->tmp_angleCCW512 += deg_360_to_512CCW(360-360); 	}
		mask512(src->tmp_angleCCW512);
		if (src->tmp_angleCCW512 > deg_360_to_512CCW(270) ) 	/* �قڐ^������������ */
		{
			src->tmp_angleCCW512 = deg_360_to_512CCW((0));		/* �^���ɋ����␳ */
			src->state++/*=4*/;
			src->speed256=t256(1.0/*6.0*/);
		}
#endif
		break;
	case 4: /* down */
		src->speed256 += t256(0.1);/*����*/
		gamen_gai_nara_osimai(src);/* ��ʊO�Ȃ炨���܂� */
		break;
	}
	/*�ȉ�rwingx.c�Ɠ���*/
/* CCW�̏ꍇ */
	src->x256+=((sin512((src->tmp_angleCCW512))*src->speed256)>>8);/*fps_factor*/
	src->y256+=((cos512((src->tmp_angleCCW512))*src->speed256)>>8);/*fps_factor*/
//	src->an im_frame=(deg_512_to_360(src->angle512+deg_360_to_512(270))/10)%36;
//	src->an im_frame = ((((src->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
#if 00
/* [CCW�̏ꍇ(�V)] CW�̏ꍇ */
	src->an im_frame = ((((src->tmp_angleCCW512)&(512-1)))>>6);/*"rw ingx8.png"*/
	/* �� */
//#else
/* CCW�̏ꍇ */
	/* �V(�܂�����ĂȂ�) */
	//src->an im_frame = ((((src->tmp_angleCCW512)&(512-1)))>>6);/*"rw ingx8.png"*/
	/*��������݊�*/
	{int aaa512;
		aaa512 = 128+ 512 - src->tmp_angleCCW512;
		mask512(aaa512);
		src->yx_an im_frame = (((aaa512))>>(6));
	}
#endif
	if (SP_DELETE != src->type)
	{
		src->type 			= TEKI_36_YOUSEI3_1+((((src->tmp_angleCCW512)&(512-1)))>>6);
	}
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_meido4(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_36_YOUSEI3_1;/*SP_ZAKO*/ /*_15_MEIDO4*/
	add_zako_common(l, h);
//	h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->callback_mover		= move_meido4;
	h->callback_loser		= lose_meido4;
	h->tmp_angleCCW512		= deg_360_to_512CCW(0); 	/* �^�� */	/* CCW�̏ꍇ */
	h->speed256				= t256(3.0/*6.0*/);
	h->state 				= 0;
}
//	h->callback_hit_enemy	= callback_hit_zako;
//
//	/*h->base.*/h->base_score		= score(/*25*/15*2);
//	/*h->base.*/h->base_hp			= (8*5)+(1/*di fficulty*/<<(2+3));
//	h->x256 				= (0==(i&(2-1)/*%2*/))?t256(30):t256(50);
//	h->y256 				= -(i*t256(20));
