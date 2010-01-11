
#include "bullet_object.h"

/*---------------------------------------------------------
	"���΂�2"(�΂��΂�)		"���ы�1",		"EYEFO",
	-------------------------------------------------------
	�o�O����H�H�H
---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
//	int angle512;
	int radius256;
	int enemy_rank;
	int flag1;
	int xcenter256;
	int ycenter256;
	int time_out;
} OBAKE2_DATA;

static int destoroy;
//static int enemy_rank;

#define NUM_OF_ENEMIES (16)

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_obake2(SPRITE *src)
{
	destoroy++;
	if ( (NUM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
	{
		destoroy = 0;
		item_create(/*zzz*/ src,
			(SP_ITEM_01_P008+(ra_nd()&1))/*50%(SP_ITEM_01_P008 or SP_ITEM_04_BOMB)*/
			//66%==SP_ITEM_04_BOMB or 33%==SP_ITEM_01_P008 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),
			, 1, ITEM_MOVE_FLAG_06_RAND_XY);
	}
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_obake2(SPRITE *src)
{
	OBAKE2_DATA *data = (OBAKE2_DATA *)src->data;
	if (0 == data->flag1)
	{
		data->radius256 += t256(1)/*fps_fa ctor*/;
		if (data->radius256 > t256(64)/*150*/) /* �ő唼�a */
		{
			data->flag1=1;
		}
	}
	else
	{
		data->radius256 -= t256(2)/*1*/ /*fps_fa ctor*/;
		if (data->radius256 < t256(8) /*<=10*/)	/* �ŏ����a */
		{
			data->flag1=0;
		}
	}
	src->tmp_angleCCW512 += /*deg_360_to_512(5)*/(1)/**fps_fa ctor*/;/* deg_360_to_512(5) �������� */
	mask512(src->tmp_angleCCW512);// if (data->angle>=360)	{	data->angle-=360;}
/* CCW�̏ꍇ */
	src->x256=((sin512((src->tmp_angleCCW512))*(data->radius256))>>8)+data->xcenter256;
	src->y256=((cos512((src->tmp_angleCCW512))*(data->radius256))>>8)+data->ycenter256;

	if (0 != difficulty)/* easy �̓z�[�~���O�����Ȃ� */
	{
		if (data->enemy_rank)
		{
		//	if (0==(ra_nd()%(1000-(data->enemy_rank)*100)))
		//	if (0==(ra_nd()%((10-(data->enemy_rank))*100)))
	//		if (0==(ra_nd()&((1<<(10-(data->enemy_rank)))-1)))/*�e�L�g�[*/	/* [10]�m��(���p�x)��������C������ */
//			if (0==(ra_nd()&((1<<(12-(data->enemy_rank)))-1)))/*�e�L�g�[*/
			if (0==(ra_nd()&((1<<(11-(data->enemy_rank)))-1)))/*�e�L�g�[*/
			{
				/* ��ʊO�Ȃ�Ό����Ȃ� */
				if (0 < src->y256)	/* ��ʓ��Ȃ猂�� */
				{
					bullet_create_enemy_homing(src);	/* �G���̗U���~�T�C�����쐬���� */
				}
			}
		}
	}
	data->time_out += 1/*fps_fa ctor*/;
	if (data->time_out < 300)
	{
	//	if (data->ycenter256 < t256(GAME_HEIGHT))
	//	if (data->ycenter256 < t256(difficulty<<6) )/* ��Փx�ɂ�艺�����Ă���ʒu���Ⴄ�B�ő�192(3*64) */
		if (data->ycenter256 < t256(difficulty<<5) )/* ��Փx�ɂ�艺�����Ă���ʒu���Ⴄ�B�ő�96(3*32) */
		{
			data->ycenter256 += t256(1)/*fps_fa ctor*/;
		}
	}
	else if (data->time_out > 1200)
	{
		src->type = SP_DELETE;	/* �����܂� */
	}
	else if (data->time_out > 600)
	{
		data->ycenter256 -= t256(1)/*fps_fa ctor*/;
	//	if (data->ycenter256+src->y256 < -((src->w)<<8))
	//	{
	//	//	src->type = SP_DELETE;	/* �����܂� */
	//	//	src->flags &= (~(SP_FLAG_VISIBLE));
	//		/* ���炩�ɏ����͂ݏo�āA�܂��߂��Ă���O�ՂȂ̂ɁA */
	//		/*	��ʊO�ɏo���̂������Ȃ�����(��x�o���猩���Ȃ��܂�)�Ȃ̂� */
	//		/*	���̏ꍇ�͈Ӗ����� */
	//	}
	}
	if (SP_DELETE != src->type)
	{
		src->type 			= TEKI_12_OBAKE01+(((src->y256>>8)&0x03));
	}
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_obake2(STAGE_DATA *l)/*int lv*/
{
	int enemy_rank;
	enemy_rank	= l->user_y;
//
	destoroy = 0;
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
//		s						= sp rite_add_res(BASE_NIJI_KEDAMA16_PNG);	//s->anim_speed=1/*2*/;/*15"ey efo.png"*/
		s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG); 	//s->anim_speed=1/*2*/;/*15"ey efo.png"*/
		s->type 				= /*SP_ZAKO*/TEKI_12_OBAKE01/*_05_NIJI_KEDAMA1*/;
//		s->type 				= SP_ZAKO/*_05_NIJI_KEDAMA1*/;
	//	s->anim_speed			= 1;	/*???*/
//		s->anim_speed			= 8;	/*???*/
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_obake2;
		s->callback_loser		= lose_obake2;
		s->callback_hit_enemy	= callback_hit_zako;
//		s->an im_frame			= i/*%s->frames*/;
		OBAKE2_DATA *data;
		data					= mmalloc(sizeof(OBAKE2_DATA));
		s->data 				= data;
		/*data->base.*/s->base_score		= score(30*2);
		/*data->base.*/s->base_health		= /*50*/ /*48*/32+(difficulty/*<<2*/);/*10+(difficulty<<2)*/	/* ���������|���鋭���ɂ��Ƃ�(�U���~�T�C���������̂�) */
		data->radius256 		= t256(10);
		data->flag1 			= 0;
		s->tmp_angleCCW512		= (/*360*/512/NUM_OF_ENEMIES)*i;
		data->time_out			= 0;
		data->xcenter256		= t256( 62) + ((ra_nd()&((256*256)-1)))/*t256*/  /*(GAME_WIDTH/2)*/;/* GAME_WIDTH 380 ~= 380 == 62+256+62 */
		data->ycenter256		= t256(-32)/*-100*/;
		data->enemy_rank 		= enemy_rank;
	}
}
#undef NUM_OF_ENEMIES
