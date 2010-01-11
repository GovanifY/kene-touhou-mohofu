
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
//	ENEMY_BASE base;
//	int angleCCW512;
	int speed256;
	int state;
//	int enemy_rank; 	/* �ݒ�t�@�C������̓G�̋��� */
//
	int wait;
} MEIDO2_DATA;

static int data_hari_rank256;
#define NUM_OF_ENEMIES (12)

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_meido2(SPRITE *src)
{
	item_create(src, (enemy_get_random_item()), 1, ITEM_MOVE_FLAG_06_RAND_XY);
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_meido2(SPRITE *src)
{
	MEIDO2_DATA *data = (MEIDO2_DATA *)src->data;
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
//efine HARIDAN_SPEED ( /*speed256*/(t256(5)+((data_le vel/*data->le vel*/)<<8) ) )
#define HARIDAN_SPEED ( /*speed256*/t256(4/*5*/) + (data_hari_rank256/*data->le vel*/)	)

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
		if (src->tmp_angleCCW512 < deg_360_to_512CCW(180/*360+90-90*/))
		{
			src->tmp_angleCCW512 -= deg_360_to_512CCW(3)/**fps_fa ctor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
			if (src->tmp_angleCCW512 < deg_360_to_512CCW(0/*360+90-89*/))
			{
				data->state++/* = 3*/;
			}
		}
		else
		{
			src->tmp_angleCCW512 += deg_360_to_512CCW(3)/**fps_fa ctor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
			if (src->tmp_angleCCW512 > deg_360_to_512CCW(360/*360+90-91*/))
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
				if (data_hari_rank256/*data->le vel*/)
				{
					bullet_create_offset_dan_type000(src, HARIDAN_SPEED, ANGLE_JIKI_NERAI_DAN/*, t256(0), t256(0)*/, BULLET_HARI32_00_AOI);
				}
#else
/* CCW�̏ꍇ */
				src->tmp_angleCCW512 = deg_360_to_512CCW(0);
				if (data_hari_rank256/*data->le vel*/)
				{
					bullet_create_offset_dan_type000(src, HARIDAN_SPEED, ANGLE_JIKI_NERAI_DAN/*, t256(0), t256(0)*/, BULLET_HARI32_00_AOI);
				}
#endif
				data->state++/* = 4*/;
		break;
	case 4: /*���U*/
		data->speed256 += t256(0.2)/*0.3*/;/*����*/
		if (data->speed256 >= t256(2.5)/*3*/ /*(6+data->le vel)*/)
		{
			data->speed256 = t256(2.5)/*3*/ /*(6+data->le vel)*/;/* �����ő�l */
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
	//	if (src->y256 < 0)
		if (src->y256 > t256(GAME_HEIGHT))
		{
			src->type = SP_DELETE;	/* �����܂� */
		}
		break;
	}
	/*�ȉ�rwingx.c�Ɠ���*/
/* CCW�̏ꍇ */
	src->x256+=((sin512((src->tmp_angleCCW512))*data->speed256)>>8)/**fps_fa ctor*/;
	src->y256+=((cos512((src->tmp_angleCCW512))*data->speed256)>>8)/**fps_fa ctor*/;
//
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_40_CHUU_YOUSEI1+((((src->tmp_angleCCW512)&(512-1)))>>6);
	}
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
	int enemy_rank;
		enemy_rank	= l->user_y;
//
	data_hari_rank256 = (enemy_rank<<7);	//	data->le vel = enemy_rank;
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
//		s						= sp rite_add_res(BASE_AKA_MEIDO08_PNG);	//s->anim_speed=0;/*37"rw ingx8.png""rw ingx.png"*/
		s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG); 	//s->anim_speed=0;/*37"rw ingx8.png""rw ingx.png"*/
		s->type 				= /*SP_ZAKO*/TEKI_40_CHUU_YOUSEI1/*_13_MEIDO2*/;
//		s->type 				= SP_ZAKO/*_13_MEIDO2*/;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_meido2;
		s->callback_loser		= lose_meido2;
		s->callback_hit_enemy	= callback_hit_zako;
//		s->an im_frame			= 0;
		MEIDO2_DATA *data;
		data					= mmalloc(sizeof(MEIDO2_DATA));
		s->data 				= data;
		/*data->base.*/s->base_score		= score(25*2);
		/*data->base.*/s->base_health		= 1+(difficulty<<2);
		data->state 			= 0;
		data->wait				= 0;
		data->speed256			= ((ra_nd()&((256*4)-1)));/*2.0*(((dou ble)ra_nd()/RAND_MAX)*2)*/
#if 0
/* CW�̏ꍇ */
		if (i<6)
		{
			data->angle512		= deg_360_to_512(10);
			s->x256 			= -((s->w128+s->w128));
		}
		else
		{
			data->angle512		= deg_360_to_512(180-10/*170*/);
			s->x256 			= t256(GAME_WIDTH);
		}
#else
/* CCW�̏ꍇ */
		if (i<6)
		{
			s->tmp_angleCCW512	= deg_360_to_512CCW(90-10/*360-10*/);
			s->x256 			= -t256(32)/*-((s->w128+s->w128))*/;
		}
		else
		{
			s->tmp_angleCCW512	= deg_360_to_512CCW(180+90+10/*360-170*/);
			s->x256 			= t256(GAME_WIDTH);
		}
#endif
		s->y256 = t256(50);
	}
}
#undef NUM_OF_ENEMIES
