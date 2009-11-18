
#include "bullet_object.h"

/*---------------------------------------------------------
		"�d��4",(�\��)		"FAIRY",	(r27����V�K�ǉ�)
	-------------------------------------------------------
	user_x: 	���x256[speed256]
	user_y: 	���ʒu[dots]
---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	int angleCCW512;	/* �����O�Ƃ��č����(�t���)�̊p�x */
	int speed256;
	int state;
	int time_out;
} AO_YOUSEI4_DATA;


/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_ao_yousei4(SPRITE *src)
{
//	case SP_ZAKO_16_AO_YOUSEI1:
	//	if (rand_percent(30))	{	put_item=(c->x, c->y, SP_ITEM_00_P001&0xff);}
	//item_create(src, (((ra_nd()&0x07)&0xff)|SP_ITEM_00_P001), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
	item_create(src, (enemy_get_random_item()), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_ao_yousei4(SPRITE *src)
{
	AO_YOUSEI4_DATA *data = (AO_YOUSEI4_DATA *)src->data;
//
	data->time_out--;
//	if (1 > data->time_out)
//	{
//		src->type = SP_DELETE;	/* �����܂� */
//	}
	switch (data->state)
	{
	case 0: 	/* �o�� �U�� */
		data->speed256--;
		if (1 > data->speed256)
		{
			data->speed256 = 1;
		}
		switch (data->time_out)
		{
		case 30:
			data->state++;
			break;
		case 50:
			if (difficulty)
			{
				bullet_create_n_way_dan_sa_type(src,
					(t256(2.0)+((difficulty)<<6)),
					ANGLE_JIKI_NERAI_DAN,
					(int)(512/24),
					BULLET_KUNAI12_04_AOI+((ra_nd())&3),
					8);
			}	/*�Ȃ�ׂ����ʉ�*/
			break;	/*(t256(3.0)+((difficulty)<<6))*/	/*((difficulty<<(1+8)))*/
					/*t256((difficulty<<2))*/ /*5*/ 	/*"kugel.png", 0*/
					/*at an2(player->y-src->y+player->h/2,player->x-src->x-player->w/2)*/
		case 70:	/*not_break;*/
		case 120:	/*not_break;*/
		case 170:	/*not_break;*/
		case 220:
			if (difficulty) {	bullet_create_aka_maru_jikinerai(src, (512-data->time_out)+t256(difficulty));	}
			break;
		case 250:/* �|����悤�ɂ��� */
			/*data->base.*/src->base_health	= (1+(difficulty)); 	/*(3+(difficulty))*/	/*1+(difficulty<<2)*/
			break;
		}
		break;
	case 1: 	/* ���E */
		data->speed256 += 2;
		data->angleCCW512 += 2;
		if (255 < data->angleCCW512)
		{
			data->angleCCW512 = 255;/*�^��*/
		}
		if (1 > src->y256)
		{
			src->type = SP_DELETE;	/* �����܂� */
		}
		break;
	}
	/*�ȉ�rwingx.c�Ɠ���*/
/* CCW�̏ꍇ */
	src->x256+=((sin512((data->angleCCW512))*data->speed256)>>8)/**fps_fa ctor*/;
	src->y256+=((cos512((data->angleCCW512))*data->speed256)>>8)/**fps_fa ctor*/;
//
//	src->yx_anim_frame = (8+((data->time_out>>2)&(8-1)));
	src->yx_anim_frame = (0x20 | ((data->time_out)&(0x10)) | ((data->time_out>>2)&(4-1)));
/*
data->time_out 	  ---a bc--
src->yx_anim_frame  yyyy xxxx
src->yx_anim_frame  --1a --bc
*/
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_ao_yousei4(STAGE_DATA *l)/*int lv*/
{
	SPRITE *s;
	s						= sprite_add_res(BASE_AO_YOUSEI24_PNG);/*7"fairy.png"*/
	s->type 				= SP_ZAKO/*_16_AO_YOUSEI1*/;
	s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	s->callback_mover		= move_ao_yousei4;
	s->callback_loser		= lose_ao_yousei4;
	s->callback_hit_enemy	= callback_hit_zako;
	s->anim_speed			= 0/*4*/;
	AO_YOUSEI4_DATA *data;
	data					= mmalloc(sizeof(AO_YOUSEI4_DATA));
	s->data 				= data;
	s->y256 				= t256(-30);
	s->x256 				= ((l->user_y)<<8);/*lv*t256(35)+t256(40)*/
	data->time_out			= 320;
	data->speed256			= ((l->user_x));			/*t256(0.5)*/	/*+0.15*difficulty*/ /*0.7+0.3*difficulty*/
#if 0
/* CW�̏ꍇ */
	data->angleCCW512		= deg_360_to_512(0);/*�^��*/
#else
/* CCW�̏ꍇ */
	data->angleCCW512		= deg_360_to_512CCW(360-0);/*�^��*/
#endif
	data->state 			= 0;
	/*data->base.*/s->base_score		= score(20*2);
	/*data->base.*/s->base_health		= 999;/* �o�ꎞ�͓|���Ȃ� */	/*3+(difficulty)*/	/*1+(difficulty<<2)*/
}