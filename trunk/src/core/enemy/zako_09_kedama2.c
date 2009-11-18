
#include "bullet_object.h"

/*---------------------------------------------------------
		"�ы�2",	"BADGUY",	(r27����V�K�ǉ�)
	-------------------------------------------------------
	�{�Ɩы�
---------------------------------------------------------*/

#define USE_X_HOUKOU (1)

typedef struct
{
//	ENEMY_BASE base;
	int state;
	int time_out;
} KEDAMA1_DATA;


//	int t x256;
//	int t y256;
//	int v x256;
//	int v y256;
//	int speed256;

//static int level;

#define NUM_OF_ENEMIES (6)

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_kedama2(SPRITE *src)
{
//	case SP_ZAKO_08_KEDAMA1:
//	if (rand_percent(10))
	{
		item_create(src, enemy_get_random_item(), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
	}
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/
enum
{
	ST00 = 0,
	ST02,
	ST03,
};

static void move_kedama2(SPRITE *src)
{
	KEDAMA1_DATA *data = (KEDAMA1_DATA *)src->data;

	data->time_out -= 1/**fps_fa ctor*/;
//

//
	switch (data->state)
	{
	case ST00:	/* �΂�΂�ɓo�ꂳ���� */
		if (
			(0 >= src->x256)
			|| (t256(GAME_WIDTH) < src->x256)
			#if (1==USE_X_HOUKOU)
			|| (0 >= src->y256)
			|| (t256(GAME_HEIGHT) < src->y256)
			#endif
		)
		{
			/*��ʊO*/;
		}
		else
		{
			data->time_out	= 64;
			data->state++;	/* �o�ꂷ�� */
		}
		break;
	case ST02:	/* �U�� */
		if (0==(data->time_out & 0x0f))
		{
			bullet_create_n_way_dan_sa_type(src,
				(t256(2.5)+((difficulty)<<6)),/* [2.5 - 3.25] */			//	(t256(2.0)+((difficulty)<<6)),/* [2.0 - 2.75] */
				ANGLE_JIKI_NERAI_DAN,
				(int)(512/24),
				BULLET_KOME_01_AOI+(src->x256&0x07),
				(3/*+difficulty+difficulty*/));
		}
		if (0 > data->time_out)
		{
			data->state++;
		}
		break;
	case ST03:
		/* ��ʊO�ɏo����I��� */
		if (
			(0 >= src->x256)
			|| (t256(GAME_WIDTH) < src->x256)
			#if (1==USE_X_HOUKOU)
			|| (0 >= src->y256)
			|| (t256(GAME_HEIGHT) < src->y256)
			#endif
		)
		{
			src->type = SP_DELETE;	/* �����܂� */
		}
		break;
	}

	/* �ړ����� */
	src->x256 += (src->vx256);
	src->y256 += (src->vy256);
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_kedama2(STAGE_DATA *l)/*int lv*/
{
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s						= sprite_add_res(BASE_KEDAMA16_PNG);	//s->anim_speed=5; /*3*/ /*9"ba dguy.png"*/
		s->anim_speed			= /*-*/3; /*�t�]�A�j���֎~�ɕύX*/
		s->type 				= SP_ZAKO/*_08_KEDAMA1*/;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_kedama2;
		s->callback_loser		= lose_kedama2;
		s->callback_hit_enemy	= callback_hit_zako;
		s->anim_frame			= 0;
		//
		s->y256 				= ((l->user_y)<<8);
		s->vx256			= ((l->user_x));
		s->vy256			= (0);
		//
		{
			if (0 < (s->vx256))
			{/* [���� user_y ���������̏ꍇ�A�E�ֈړ�(��)] [����y���������̏ꍇ�A���ֈړ�(��)] */
				s->x256 			= t256(-30)-(i<<(5+8));
			}
			else
			{/* [���� user_y ���������̏ꍇ�A���ֈړ�(��)] [����y���������̏ꍇ�A��ֈړ�(��)] */
				s->x256 			= t256(360)+(i<<(5+8));/* 360 > 352(x_max) > 272(y_max) */
			}
		}
		//
		if (0 < (s->y256))
		{	;	}
		else
		{
			/* y���W���������̏ꍇ�́Ax���W�w��Ȃ̂�(x���W y���W ���t�ɂ���) */
			{
				int s_sss;
				s_sss				= s->x256;
				s->x256 			= -(s->y256);
				s->y256 			= s_sss;
			}
			s->vy256			= (s->vx256);
			s->vx256			= (0);
		}
		//
		KEDAMA1_DATA *data;
		data					= mmalloc(sizeof(KEDAMA1_DATA));
		s->data 				= data;
		/*data->base.*/s->base_score		= score(/*50*/5*2*4)/**(level+1)*/;
		/*data->base.*/s->base_health		= (8/**8*/)/*+(difficulty<<2)*/ /*(1+(difficulty<<2))*/;/*���炩����*/
		data->state 			= ST00;
		data->time_out			= 64+(i<<4);
	}
}
#undef NUM_OF_ENEMIES
//			data->anime_houkou	= 0x50;
//			data->anime_houkou	= 0x00;

	//	s->x256 				= t256(200)+	   (ra_nd()&((64*256)-1))/*ra_nd()%40*/;
	//	s->y256 				= ((ra_nd()&((32*256)-1))-t256(80));//	(ra_nd()%40-90);
	//	s->vx256			= (0);
	//	s->vy256			= (0);
//	/*data->*/level 	= l->user_y;

//		data->anime_houkou	= 0x20;

//		data->tx256 			= player->x256;
//		data->ty256 			= player->y256;

