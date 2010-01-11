
#include "bullet_object.h"

/*---------------------------------------------------------
		"�Ԗы�1",		"MINE",
	-------------------------------------------------------
	���邮�邻�̏�œ����Ȃ��ŁA��]�A�j�����Ă�
	�ق��Ƃ���8�ɕ��􂵂�
	�Ō�ɍ����e�����ė�����(��)
---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	int state;
//	int enemy_rank;
	int tx256;
	int ty256;
	int time_out;
} AKA_KEDAMA1_DATA;

static int enemy_rank;

#define USE_OLD 1

#if (1==USE_OLD)
	/* ���L�ϐ� / shered */
	#define zzz_angle512 state
#endif

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_aka_kedama1(SPRITE *src)
{
	/* �����������ق����A�C�e�����҂��� */
	item_create(/*zzz*/ src, SP_ITEM_00_P001, 3/*1*/, ITEM_MOVE_FLAG_06_RAND_XY);
}

/*---------------------------------------------------------
	�G�ړ�(�����)
---------------------------------------------------------*/

static void move_aka_kedama1_2nd(SPRITE *src)
{
#if (1==USE_OLD)
	AKA_KEDAMA1_DATA *data = (AKA_KEDAMA1_DATA *)src->data;
#endif
//	if (/*0 <*/ /*data->*/enemy_rank)	/* easy�͑����񂾂���ꍇ�������Ȃ� */
	{
		/*
		0  0/100   0/65536
		1  1/90  728/65536
		2  1/80  819/65536
		3  1/70  936/65536
		*/
		static const Uint16 kakuritu_tbl[4] =
		{ 0, 728, 819, 936 };
	//	if (0==(ra_nd()%(100-/*data->*/enemy_rank*10)))
		if (kakuritu_tbl[/*data->*/enemy_rank] > (ra_nd()&(65536-1)))
		{
			bullet_create_aka_maru_jikinerai(src, t256(2.5)+(/*data->*/enemy_rank<<6) );/*�����e*/	/*t256(3+data->enemy_rank)*/
		}
	}
/* CCW�̏ꍇ */
#if (1==USE_OLD)
	src->x256 += ((sin512((data->zzz_angle512))*t256(3))>>8)/**fps_fa ctor*/;
	src->y256 += ((cos512((data->zzz_angle512))*t256(3))>>8)/**fps_fa ctor*/;
#else
	src->x256 += (src->vx256)/**fps_fa ctor*/;
	src->y256 += (src->vy256)/**fps_fa ctor*/;
#endif
	if ((src->x256<0-((src->w128+src->w128)))||(src->x256 > t256(GAME_WIDTH))||
		(src->y256<0-((src->h128+src->h128)))||(src->y256 > t256(GAME_HEIGHT)))
	{
		src->type = SP_DELETE;	/* �����܂� */
	}
//
	src->m_angleCCW512 += 10;
	mask512(src->m_angleCCW512);
}

/*---------------------------------------------------------
	�G��ǉ�����(�ϐg)
---------------------------------------------------------*/

static void add_local_aka_kedama1_2nd(SPRITE *src/*, int lv*/)
{
//	data->enemy_rank	= enemy_rank;
	int i;
	for (i=0; i<8; i++)/*����*/
	{
		SPRITE *s;
//		s						= sp rite_add_res(BASE_AKA_KEDAMA08_PNG);	s->anim_speed=i/*6 2*/;/*19"12 side.png"*/
		s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG); //	s->anim_speed=i/*6 2*/;/*19"12 side.png"*/
		s->type 				= /*SP_ZAKO*/TEKI_56_CHEN/*_04_AKA_KEDAMA1*/;
//		s->type 				= SP_ZAKO/*_04_AKA_KEDAMA1*/;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_aka_kedama1_2nd;
		s->callback_loser		= lose_aka_kedama1;
		s->callback_hit_enemy	= callback_hit_zako;
//		s->an im_frame			= 0;
		s->x256 				= src->x256;
		s->y256 				= src->y256;
#if (1==USE_OLD)
#else
	//	data->zzz_angle512		= (i<<6);//  (/*360*/512/8)*i;
		s->vx256 = ((sin512(((i<<6)/*data->zzz_angle512*/))*t256(1.5/*3.0*/))>>8)/**fps_fa ctor*/;
		s->vy256 = ((cos512(((i<<6)/*data->zzz_angle512*/))*t256(1.5/*3.0*/))>>8)/**fps_fa ctor*/;
#endif

#if (1==USE_OLD)
		AKA_KEDAMA1_DATA *data;
		data					= mmalloc(sizeof(AKA_KEDAMA1_DATA));
		s->data 				= data;
#endif
	//	data->enemy_rank		= ((AKA_KEDAMA1_DATA *)(src->data))->enemy_rank;
		/*data->base.*/s->base_score		= score(10*2)*(1+/*data->*/enemy_rank);
		/*data->base.*/s->base_health		= (1+/*data->*/enemy_rank)+(difficulty<<4);
#if (1==USE_OLD)
		data->zzz_angle512		= (i<<6);//  (/*360*/512/8)*i;
#endif
	}
}

/*---------------------------------------------------------
	�G�ړ�(�{��)
---------------------------------------------------------*/

static void move_aka_kedama1_1st(SPRITE *src)
{
	AKA_KEDAMA1_DATA *data = (AKA_KEDAMA1_DATA *)src->data;
	switch (data->state)
	{
	case 0: /* move */
		{
		int angle512;
			angle512=atan_512(data->ty256-src->y256,data->tx256-src->x256);
/* CCW�̏ꍇ */
			src->x256 += ((sin512(angle512)*t256(1.5))>>8)/**fps_fa ctor*/;
			src->y256 += ((cos512(angle512)*t256(1.5))>>8)/**fps_fa ctor*/;
		}
	//	if ((data->tx-1 <= src->x) && (src->x <= data->tx+1) &&
	//		(data->ty-1 <= src->y) && (src->y <= data->ty+1))
		if ((data->tx256-256 <= src->x256) && (src->x256 <= data->tx256+256) &&
			(data->ty256-256 <= src->y256) && (src->y256 <= data->ty256+256))
		{
			data->state = 1;
			data->time_out = 80;
		}
	//
		src->m_angleCCW512 += 5;
		mask512(src->m_angleCCW512);
		break;
	case 1:
		data->time_out--;
		if (31 > data->time_out)
		{
			data->state 	= 2;
//			src->anim_speed = 0;
		}
		break;
	case 2:
		data->time_out--;
		if (1 > data->time_out)
		{
			add_local_aka_kedama1_2nd(src/*,data->enemy_rank*/);/* �ϐg ���� */
			src->type = SP_DELETE;	/* �{�̂͂����܂� */
		}
		break;
	}

}

/*---------------------------------------------------------
	�G��ǉ�����(�{��)
---------------------------------------------------------*/

void add_zako_aka_kedama1(STAGE_DATA *l)/*int lv*/
{
//	int enemy_rank;
	enemy_rank	= l->user_y;
//
	SPRITE *s;
//	s						= sp rite_add_res(BASE_AKA_KEDAMA08_PNG);	//s->anim_speed 	= 1;/*19"12 side.png"*/
	s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG); 	//s->anim_speed 	= 1;/*19"12 side.png"*/
	s->type 				= /*SP_ZAKO*/TEKI_56_CHEN/*_04_AKA_KEDAMA1*/;
//	s->type 				= SP_ZAKO/*_04_AKA_KEDAMA1*/;
	s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	s->callback_mover		= move_aka_kedama1_1st;
	s->callback_loser		= lose_aka_kedama1;
	s->callback_hit_enemy	= callback_hit_zako;
//	s->an im_frame			= 0;
//	s->x256 				= t256(ra_nd()%(GAME_WIDTH+10))-t256(10);
	s->x256 				= ((ra_nd()&((256*256)-1)))+((ra_nd()&((256*64)-1)))+t256(32);
	s->y256 				= t256(-40);
	AKA_KEDAMA1_DATA *data;
	data					= mmalloc(sizeof(AKA_KEDAMA1_DATA));
	s->data 				= data;
//	data->enemy_rank		= enemy_rank;
	/*data->base.*/s->base_score		= score(30*2);
//	/*data->base.*/s->base_health		= (12)+(enemy_rank+enemy_rank)+(difficulty<<2);
	/*data->base.*/s->base_health		= (6*8)+(enemy_rank+enemy_rank)+(difficulty<<2);
	data->tx256 			= ((ra_nd()&((256*256)-1)))+t256(64);/*320?*/ /*t256(ra_nd()%270)+t256(50)*/
//	data->ty256 			= ((ra_nd()&((256*256)-1)))+t256(16);/*350?*/ /*t256(ra_nd()%300)+t256(50)*/
	data->ty256 			= ((ra_nd()&((256*128)-1)))+t256(16);/*350?*/ /*t256(ra_nd()%300)+t256(50)*/
	data->state 			= 0;
}
