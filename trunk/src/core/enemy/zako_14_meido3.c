
#include "bullet_object.h"

/*---------------------------------------------------------
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

typedef struct
{
	ENEMY_BASE base;
	int angleCCW512;
	int speed256;
	int state;
	int turn_count;
//
	int turnspeed512;
	int clip_left256;
	int clip_right256;
} MEIDO3_DATA;

static int static_last=0;/* �o��ʒu�̐؂�ւ� */
static int level;
static int destoroy[2];

//	NUM_OF_ENEMIES=10;		10�@
//#define NUM_OF_ENEMIES (10)
//#define NUM_OF_ENEMIES (4+difficulty)
#define NUM_OF_ENEMIES (4+3)

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_meido3(SPRITE *s)
{
	destoroy[static_last]++;
	if ( (NUM_OF_ENEMIES-1) < destoroy[static_last]/*all_destoroy*/)
	{
		destoroy[static_last] = 0;
		item_create(s, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
	}
}

/*---------------------------------------------------------
	�G�U��
---------------------------------------------------------*/

static void shot_meido3(SPRITE *s)
{
	if (0 < /*data->*/level)
	{
		bullet_create_n_way_dan_sa_type(s,
			/*speed256*/t256(2),
			ANGLE_JIKI_NERAI_DAN/*angle512*/,
			(int)(512/24),
			BULLET_UROKO14_01_AKA,
			/*n*/(difficulty<<1)
		);
	}	/* easy�ȊO�͑_���e������ */
}
			/*bullet_create_aka_maru_jikinerai(s, t256(3+(data->level>>1)));*/
			/*bullet_create_aka_maru_jikinerai(s, t256(3+(data->level>>1)));*/

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_meido3(SPRITE *s)
{
	MEIDO3_DATA *data = (MEIDO3_DATA *)s->data;
	switch (data->state)
	{
	case 0: 	/* ������ŉ�]�� */
		/* CW�̏ꍇ */
	//	data->angleCCW512 -= data->turnspeed512/**fps_fa ctor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
		/* CCW�̏ꍇ */
		data->angleCCW512 += data->turnspeed512/**fps_fa ctor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
		mask512(data->angleCCW512);
		/* �E(�΂ߏ�)���������� */
		/* CW�̏ꍇ */
	//	if ( (512-16) < data->angleCCW512 )/*eps*/ /*<= deg_360_to_512(0)*/ 	//deg_360_to_512(10/*90*/ /*-90*/ /*360*/ /*-10*/
		/* CCW�̏ꍇ */
		if ( 0==(0x100 & data->angleCCW512) )/* (deg_360_to_512CCW(180) > data->angleCCW512) */
		{
			if ( (128+16) < data->angleCCW512 )/*eps*/ /*<= deg_360_to_512(0)*/ 	//deg_360_to_512(10/*90*/ /*-90*/ /*360*/ /*-10*/
			{
			//	data->angleCCW512 = deg_360_to_512(0/*360*/ /*-10*/)/*deg_360_to_512(360)*/;
				data->state++/*=1*/;
				shot_meido3(s); 	/* easy�ȊO�͑_���e������ */
			}
		}
		break;
	case 1: 	/* �E(�΂ߏ�)�ֈړ��� */
		if (0 < data->turn_count)
		{
			if (s->x256 >= data->clip_right256) {	data->state++/*=2*/;	data->turn_count--; }
		}
		else	/* �ޏꒆ */
		{
			if (s->x256 > t256(GAME_WIDTH) )
			{
				s->flags &= (~(SP_FLAG_VISIBLE));
				s->type = SP_DELETE;
			}
		}
		break;
	case 2: 	/* �E����ŉ�]�� */
	//	data->angleCCW512 += data->turnspeed512/**fps_fa ctor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
		/* CCW�̏ꍇ */
		data->angleCCW512 -= data->turnspeed512/**fps_fa ctor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
		mask512(data->angleCCW512);
		/* ��(�΂ߏ�)���������� */
	//	if ( (256+16) < data->angleCCW512 ) 	//deg_360_to_512(180 /*270*/ /*90+*/ /*180-90-90*/ /*+10*/)
		/* CCW�̏ꍇ */
		if ( 0!=(0x100 & data->angleCCW512) )/* (deg_360_to_512CCW(180) < data->angleCCW512) */
		{
			if ( (256+128-16) > data->angleCCW512 ) 	//deg_360_to_512(180 /*270*/ /*90+*/ /*180-90-90*/ /*+10*/)
			{
			//	data->angleCCW512 = deg_360_to_512(180/*+10*/);
				data->state++/*=3*/;
				shot_meido3(s); 	/* easy�ȊO�͑_���e������ */
			}
		}
		break;
	case 3: 	/* ��(�΂ߏ�)�ֈړ��� */
		if (0 < data->turn_count)
		{
			if (s->x256 <= data->clip_left256) {	data->state=0;	data->turn_count--; 	}
		}
		else	/* �ޏꒆ */
		{
			if (s->x256 < t256(0) )
			{
				s->flags &= (~(SP_FLAG_VISIBLE));
				s->type = SP_DELETE;
			}
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
/*���ݎd�l�エ������*/
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_meido3(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
	/*data->*/level 		= lv;
//
	static_last++;
	static_last &= 1;
//
	destoroy[static_last] = 0;
//
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_AKA_MEIDO08_PNG); 	//s->anim_speed=0;/*37"rw ingx8.png""rw ingx.png"*/
		s->type 			= SP_ZAKO/*_14_MEIDO3*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_meido3;
		s->callback_loser	= lose_meido3;
		MEIDO3_DATA *data;
		data				= mmalloc(sizeof(MEIDO3_DATA));
		s->data 			= data;
		data->base.score	= score(/*25*/50*2);
	//	data->base.health	= (12*8)+(difficulty<<3/*4*/)/*5+(difficulty<<2)*/;/*��킷��*/
	//	data->base.health	= (12*8)+(difficulty<<2/*4*/)/*5+(difficulty<<2)*/;/*���������̂Ŏキ����*/
		data->base.health	= (10*8)+(difficulty<<2/*4*/)/*5+(difficulty<<2)*/;/*���������̂Ŏキ����*/
		data->angleCCW512	= (256+128/*0*/)/*256+16*/ /*deg_360_to_512(270)*/;
		data->speed256		= t256(1.5) /*3+difficulty*/;/*�͂₷��*/
		data->turnspeed512	= (7) /*deg_360_to_512*/ /*(4+difficulty)*/;/*�ȗ���(360�x��512�x)*/
//		data->clip_left256	= (t256(static_last<<7)+t256(static_last<<6)+t256(32)) /*50*/;			/* 32,	224(128+64+32)	*/
//		data->clip_right256 = (t256(static_last<<7)+t256(static_last<<6)+t256(32))+t256(128-32);	/* 128, 320(256+64)   */	 //  /*GAME_WIDTH-50-s->w*/ �E�B���h�E���̕ύX
		data->clip_left256	= (t256(static_last<<7)+t256(static_last<<6)+t256(32)) /*50*/;			/* 32,	224(128+64+32)	*/
		data->clip_right256 = (t256(static_last<<7)+t256(static_last<<6)+t256(32))+t256(128-32-32);	/* 128, 320(256+64)   */	 //  /*GAME_WIDTH-50-s->w*/ �E�B���h�E���̕ύX
		data->state 		= (static_last<<1)/*0*/;
		data->turn_count	= 4;
//		s->x256 			= t256((static_last<<7))+t256((32))+(((s->w128)*(i+1))*t256(0.375))/* 0.375=0.75/2.0 -(s->w*(i+1))*0.5*/;/*����������*/
		s->x256 			= t256(10);/*����������*/
//		s->y256 			= t256(0)/*50*/;
		s->y256 			= t256(10)/*50*/;
	}
}
#undef NUM_OF_ENEMIES
