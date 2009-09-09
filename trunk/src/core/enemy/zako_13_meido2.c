
#include "bullet_object.h"

/*---------------------------------------------------------
	"���C�h2",		"ZATAK",
	-------------------------------------------------------
	rwingx_zatak (�Ԃ��d��)
	���E���猻��A���@�_���e��ł�����A�^���֒��i���Ă���B
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
//
	int wait;
} MEIDO2_DATA;

static int data_level256;
#define NUM_OF_ENEMIES (12)

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_meido2(SPRITE *s)
{
	item_create(s, (enemy_get_random_item()), 1, ITEM_MOVE_FLAG_06_RAND_XY);
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_meido2(SPRITE *s)
{
	MEIDO2_DATA *data = (MEIDO2_DATA *)s->data;
	data->wait += 1/*fps_fa ctor*/;
	switch (data->state)
	{
	case 0:
		if (data->wait >= 40)
		{
			data->state++/*=1*/;
		}
		break;
	case 1:
		data->speed256 -= t256(0.02/**fps_fa ctor*/);
		if (data->speed256 <= 0)
		{
			data->speed256 = 0;
			data->state++/*=2*/;
		}
		break;
	case 2:
//efine HARIDAN_SPEED ( /*speed256*/(t256(5)+((data_level/*data->level*/)<<8) ) )
#define HARIDAN_SPEED ( /*speed256*/t256(4/*5*/) + (data_level256/*data->level*/)	)

#if 0
/* CW�̏ꍇ */
		if (data->angle512 < deg_360_to_512(90))
		{
			data->angle512 += deg_360_to_512(3)/**fps_fa ctor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
			if (data->angle512 >= deg_360_to_512(89))
			{
				data->state++/* = 3*/;
			}
		}
		else
		{
			data->angle512 -= deg_360_to_512(3)/**fps_fa ctor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
			if (data->angle512 <= deg_360_to_512(91))
			{
				data->state++/* = 3*/;
			}
		}
#else
/* CCW�̏ꍇ */
		if (data->angleCCW512 < deg_360_to_512CCW(180/*360+90-90*/))
		{
			data->angleCCW512 -= deg_360_to_512CCW(3)/**fps_fa ctor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
			if (data->angleCCW512 < deg_360_to_512CCW(0/*360+90-89*/))
			{
				data->state++/* = 3*/;
			}
		}
		else
		{
			data->angleCCW512 += deg_360_to_512CCW(3)/**fps_fa ctor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
			if (data->angleCCW512 > deg_360_to_512CCW(360/*360+90-91*/))
			{
				data->state++/* = 3*/;
			}
		}
#endif
		break;
	case 3: /*�e����*/
#if 0
/* CW�̏ꍇ */
				data->angle512 = deg_360_to_512(90);
				if (data_level256/*data->level*/)
				{
					bullet_create_offset_dan_type(s, HARIDAN_SPEED, ANGLE_JIKI_NERAI_DAN, t256(0), t256(0), BULLET_HARI32_00_AOI);
				}
#else
/* CCW�̏ꍇ */
				data->angleCCW512 = deg_360_to_512CCW(0);
				if (data_level256/*data->level*/)
				{
					bullet_create_offset_dan_type(s, HARIDAN_SPEED, ANGLE_JIKI_NERAI_DAN, t256(0), t256(0), BULLET_HARI32_00_AOI);
				}
#endif
				data->state++/* = 4*/;
		break;
	case 4: /*���U*/
		data->speed256 += t256(0.2)/*0.3*/;/*����*/
		if (data->speed256 >= t256(2.5)/*3*/ /*(6+data->level)*/)
		{
			data->speed256 = t256(2.5)/*3*/ /*(6+data->level)*/;/* �����ő�l */
			data->state++/* = 5*/;
		}
		break;
	case 5:/*�ޏ�*/
#if 0
		if ()
		{
			if ()
			{
			}else
			{
			}
		}
#endif
	//	if (s->y256 < 0)
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

/*
(���P):��̂�version����Y���܂����Ă邪�Afps_fa ctor����肵�Ȃ�����A
fps_fa ctor�����悤���Y����̂�KETM���̂̎d�l�B
*/

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_meido2(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	data_level256=(lv<<7);	//	data->level=lv;
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_AKA_MEIDO08_PNG); 	//s->anim_speed=0;/*37"rw ingx8.png""rw ingx.png"*/
		s->type 			= SP_ZAKO/*_13_MEIDO2*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_meido2;
		s->callback_loser	= lose_meido2;
//		s->anim_frame		= 0;
		MEIDO2_DATA *data;
		data				= mmalloc(sizeof(MEIDO2_DATA));
		s->data 			= data;
		data->base.score	= score(25*2);
		data->base.health	= 1+(difficulty<<2);
		data->state 		= 0;
		data->wait			= 0;
		data->speed256		= ((ra_nd()&((256*4)-1)));/*2.0*(((dou ble)ra_nd()/RAND_MAX)*2)*/
#if 0
/* CW�̏ꍇ */
		if (i<6)
		{
			data->angle512	= deg_360_to_512(10);
			s->x256 		= -((s->w128+s->w128));
		}
		else
		{
			data->angle512	= deg_360_to_512(180-10/*170*/);
			s->x256 		= t256(GAME_WIDTH);
		}
#else
/* CCW�̏ꍇ */
		if (i<6)
		{
			data->angleCCW512	= deg_360_to_512CCW(90-10/*360-10*/);
			s->x256 		= -t256(32)/*-((s->w128+s->w128))*/;
		}
		else
		{
			data->angleCCW512	= deg_360_to_512CCW(180+90+10/*360-170*/);
			s->x256 		= t256(GAME_WIDTH);
		}
#endif
		s->y256 = t256(50);
	}
}
#undef NUM_OF_ENEMIES
