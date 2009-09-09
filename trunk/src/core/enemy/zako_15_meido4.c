
#include "bullet_object.h"

/*---------------------------------------------------------
	"���C�h4",		"CIR",	rwingx_cir
	-------------------------------------------------------
	�E��������c���Ō���A���Ɍ������B���Ő���B
	��������㏸���A�E���Ɍ������B��Ő���B
	�E�����牺�Ɍ������B��ʊO�֑ޏ�B
	-------------------------------------------------------
	ok
---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE base;
	int angleCCW512;
	int speed256;
	int state;
//	int level;
} MEIDO4_DATA;

#define NUM_OF_ENEMIES (24)

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_meido4(SPRITE *s)
{
	item_create(s, SP_ITEM_06_TENSU, 1, ITEM_MOVE_FLAG_06_RAND_XY);
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_meido4(SPRITE *s)
{
	MEIDO4_DATA *data = (MEIDO4_DATA *)s->data;
	switch (data->state)
	{
	case 0: /* ���Ɍ����� / down */
		if (s->y256 > t256(128/*200*/)) 	/* ���S�t�߂��z���ĉ���������A�������x�𗎂Ƃ� */
		{
			data->state++/*=1*/;
			data->speed256=t256(2.5/*3.0*/);/* ���x�𗎂Ƃ� */
		}
		break;
	case 1: /* ���Ő��� / turn */
#if 0
/* CW�̏ꍇ */
		data->angle512 -= ((5))/*2*/ /*deg_360_to_512(2)*/ /**fps_fa ctor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
	//	if (data->angle512  <	deg_360_to_512(0))
	//	{	data->angle512  += deg_360_to_512(360);	}
		mask512(data->angle512);
		if ((data->angle512 >= deg_360_to_512((270-3))) &&		/* �قڐ^����������� */
			(data->angle512 <= deg_360_to_512((270+3))))
		{
			data->angle512 = deg_360_to_512((270));		/* �^��ɋ����␳ */
			data->state++/*=2*/;
			data->speed256=t256(2.0/*8.0*/);/* ���x�𗎂Ƃ� */
		}
#else
/* CCW�̏ꍇ */
		data->angleCCW512 += ((5))/*2*/ /*deg_360_to_512CCW(360-2)*/ /**fps_fa ctor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
	//	if (data->angleCCW512  <	deg_360_to_512CCW(360-0))
	//	{	data->angleCCW512  += deg_360_to_512CCW(360-360);	}
		mask512(data->angleCCW512);
		if ( data->angleCCW512 > deg_360_to_512CCW((180)) 		)		/* �قڐ^����������� */

		{
			data->angleCCW512 = deg_360_to_512CCW((180));		/* �^��ɋ����␳ */
			data->state++/*=2*/;
			data->speed256=t256(2.0/*8.0*/);/* ���x�𗎂Ƃ� */
		}
#endif
		break;
	case 2: /* up */
		if (s->y256 < t256(64/*50*/))	/* ���S�t�߂��z���ďオ������A�������x�𗎂Ƃ� */
		{
			data->state++/*=3*/;
			data->speed256=t256(1.5/*3.0*/);
		}
		break;
	case 3: /* turn */
#if 0
/* CW�̏ꍇ */
		data->angle512 += ((5))/*2*/ /*deg_360_to_512(2)*/ /**fps_fa ctor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
	//	if (data->angle512 < deg_360_to_512(0))
	//	{	data->angle512 += deg_360_to_512(360); 	}
		mask512(data->angle512);
		if ((data->angle512 >= deg_360_to_512((90-3))) &&		/* �قڐ^������������ */
			(data->angle512 <= deg_360_to_512((90+3))))
		{
			data->angle512 = deg_360_to_512((90));		/* �^���ɋ����␳ */
			data->state++/*=4*/;
			data->speed256=t256(1.0/*6.0*/);
		}
#else
/* CCW�̏ꍇ */
		data->angleCCW512 -= ((5))/*2*/ /*deg_360_to_512CCW(360-2)*/ /**fps_fa ctor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
	//	if (data->angleCCW512 < deg_360_to_512CCW(360-0))
	//	{	data->angleCCW512 += deg_360_to_512CCW(360-360); 	}
		mask512(data->angleCCW512);
		if (data->angleCCW512 > deg_360_to_512CCW(270) ) 	/* �قڐ^������������ */
		{
			data->angleCCW512 = deg_360_to_512CCW((0));		/* �^���ɋ����␳ */
			data->state++/*=4*/;
			data->speed256=t256(1.0/*6.0*/);
		}
#endif
		break;
	case 4: /* down */
		data->speed256 += t256(0.1);/*����*/
		if (s->y256 > t256(GAME_HEIGHT))
		{
			s->type = SP_DELETE;
		}
		break;
	}
	/*�ȉ�rwingx.c�Ɠ���*/
/* CCW�̏ꍇ */
	s->x256+=((sin512((data->angleCCW512))*data->speed256)>>8)/**fps_fa ctor*/;
	s->y256+=((cos512((data->angleCCW512))*data->speed256)>>8)/**fps_fa ctor*/;
//	s->anim_frame=(deg_512_to_360(data->angle512+deg_360_to_512(270))/10)%36;
//	s->anim_frame = ((((data->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
#if 1
/* [CCW�̏ꍇ(�V)] CW�̏ꍇ */
	s->anim_frame = ((((data->angleCCW512)&(512-1)))>>6);/*"rw ingx8.png"*/
	/* �� */
#else
/* CCW�̏ꍇ */
	/* �V(�܂�����ĂȂ�) */
	//s->anim_frame = ((((data->angleCCW512)&(512-1)))>>6);/*"rw ingx8.png"*/
	/*��������݊�*/
	{int aaa512;
		aaa512 = 128+ 512 - data->angleCCW512;
		mask512(aaa512);
		s->yx_anim_frame = (((aaa512))>>(6));
	}
#endif
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_meido4(STAGE_DATA *l)/*int lv*/
{
	int i;
	for (i=0;i<NUM_OF_ENEMIES;i++)
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_AKA_MEIDO08_PNG); 	//s->anim_speed=0;/*37"rw ingx8.png""rw ingx.png"*/
		s->type 			= SP_ZAKO/*_15_MEIDO4*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_meido4;
		s->callback_loser	= lose_meido4;
//		s->anim_frame 		= 0;
		MEIDO4_DATA *data;
		data				= mmalloc(sizeof(MEIDO4_DATA));
		s->data 			= data;
		data->base.score	= score(/*25*/15*2);
		data->base.health	= 5+(difficulty<<2);
#if 0
/* CW�̏ꍇ */
		data->angle512		= deg_360_to_512(90);		/* �^�� */
#else
/* CCW�̏ꍇ */
		data->angleCCW512	= deg_360_to_512CCW(0); 	/* �^�� */
#endif
		data->speed256		= t256(3.0/*6.0*/);
		data->state 		= 0;
//		data->level 		= lv;
		s->x256 			= (0==(i&(2-1)/*%2*/))?t256(30):t256(50);
		s->y256 			= -(i*t256(20));
	}
}
#undef NUM_OF_ENEMIES
