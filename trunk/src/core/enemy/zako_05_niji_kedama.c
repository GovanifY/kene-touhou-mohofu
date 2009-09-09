
#include "bullet_object.h"

/*---------------------------------------------------------
		"���ы�1",		"EYEFO",
	-------------------------------------------------------
	�o�O����H�H�H
---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE base;
	int angle512;
	int radius256;
	int level;
	int flag1;
	int xcenter256;
	int ycenter256;
	int time_out;
} NIJI_KEDAMA1_DATA;
static int destoroy;

#define NUM_OF_ENEMIES (16)

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_niji_kedama1(SPRITE *s)
{
	destoroy++;
	if ( (NUM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
	{
		destoroy = 0;
		item_create(/*zzz*/ s,
			(SP_ITEM_01_P008+(ra_nd()&1))/*50%(SP_ITEM_01_P008 or SP_ITEM_02_BOMB)*/
			//66%==SP_ITEM_02_BOMB or 33%==SP_ITEM_01_P008 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),
			, 1, ITEM_MOVE_FLAG_06_RAND_XY);
	}
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_niji_kedama1(SPRITE *s)
{
	NIJI_KEDAMA1_DATA *d = (NIJI_KEDAMA1_DATA *)s->data;
	if (0 == d->flag1)
	{
		d->radius256 += t256(1)/*fps_fa ctor*/;
		if (d->radius256 > t256(64)/*150*/) /* �ő唼�a */
		{
			d->flag1=1;
		}
	}
	else
	{
		d->radius256 -= t256(2)/*1*/ /*fps_fa ctor*/;
		if (d->radius256 < t256(8) /*<=10*/)	/* �ŏ����a */
		{
			d->flag1=0;
		}
	}
	d->angle512 += /*deg_360_to_512(5)*/(1)/**fps_fa ctor*/;/* deg_360_to_512(5) �������� */
	mask512(d->angle512);// if (d->angle>=360)	{	d->angle-=360;}
/* CCW�̏ꍇ */
	s->x256=((sin512((d->angle512))*(d->radius256))>>8)+d->xcenter256;
	s->y256=((cos512((d->angle512))*(d->radius256))>>8)+d->ycenter256;

	if (0 != difficulty)/* easy �̓z�[�~���O�����Ȃ� */
	{
		if (d->level)
		{
		//	if (0==(ra_nd()%(1000-(d->level)*100)))
		//	if (0==(ra_nd()%((10-(d->level))*100)))
			if (0==(ra_nd()&((1<<(10-(d->level)))-1)))/*�e�L�g�[*/
			{
				bullet_create_enemy_homing(s);	/* �G���̗U���~�T�C�����쐬���� */
			}
		}
	}
	d->time_out += 1/*fps_fa ctor*/;
	if (d->time_out < 300)
	{
	//	if (d->ycenter256 < t256(GAME_HEIGHT))
	//	if (d->ycenter256 < t256(difficulty<<6) )/* ��Փx�ɂ�艺�����Ă���ʒu���Ⴄ�B�ő�192(3*64) */
		if (d->ycenter256 < t256(difficulty<<5) )/* ��Փx�ɂ�艺�����Ă���ʒu���Ⴄ�B�ő�96(3*32) */
		{
			d->ycenter256 += t256(1)/*fps_fa ctor*/;
		}
	}
	else if (d->time_out > 1200)
	{
		s->type = SP_DELETE;
	}
	else if (d->time_out > 600)
	{
		d->ycenter256 -= t256(1)/*fps_fa ctor*/;
	//	if (d->ycenter256+s->y256 < -((s->w)<<8))
	//	{
	//	//	s->type = SP_DELETE;
	//	//	s->flags &= (~(SP_FLAG_VISIBLE));
	//		/* ���炩�ɏ����͂ݏo�āA�܂��߂��Ă���O�ՂȂ̂ɁA */
	//		/*	��ʊO�ɏo���̂������Ȃ�����(��x�o���猩���Ȃ��܂�)�Ȃ̂� */
	//		/*	���̏ꍇ�͈Ӗ����� */
	//	}
	}
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_niji_kedama1(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	destoroy = 0;
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_NIJI_KEDAMA16_PNG);	//s->anim_speed=1/*2*/;/*15"ey efo.png"*/
		s->anim_speed=1;	/*???*/
		s->type 			= SP_ZAKO/*_05_NIJI_KEDAMA1*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_niji_kedama1;
		s->callback_loser	= lose_niji_kedama1;
		s->anim_frame		= i/*%s->frames*/;
		NIJI_KEDAMA1_DATA *data;
		data				= mmalloc(sizeof(NIJI_KEDAMA1_DATA));
		s->data 			= data;
		data->base.score	= score(30*2);
		data->base.health	= /*50*/ /*48*/32+(difficulty/*<<2*/);/*10+(difficulty<<2)*/	/* ���������|���鋭���ɂ��Ƃ�(�U���~�T�C���������̂�) */
		data->radius256 	= t256(10);
		data->flag1 		= 0;
		data->angle512		= (/*360*/512/NUM_OF_ENEMIES)*i;
		data->time_out		= 0;
		data->xcenter256	= t256( 62) + ((ra_nd()&((256*256)-1)))/*t256*/  /*(GAME_WIDTH/2)*/;/* GAME_WIDTH 380 ~= 380 == 62+256+62 */
		data->ycenter256	= t256(-32)/*-100*/;
		data->level 		= lv;
	}
}
#undef NUM_OF_ENEMIES
