
#include "bullet_object.h"

/*---------------------------------------------------------
		"���ґ�2",		"GREETER",
	-------------------------------------------------------
	�o�O����H�H�H
	-------------------------------------------------------
	�T�@���ɑ�����A�ґ���s���Ă���U�R�G
	-------------------------------------------------------
	ming_greeter
	rwingx_curver(ming)�Ɏ��Ă�
---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	int angle512;
	int speed256;
	int state;
	int level;
//
	int max_y256;
} YUKARI2_DATA;
static int destoroy;

#define NUM_OF_ENEMIES (5)

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_yukari2(SPRITE *src)
{
	item_create(src, enemy_get_random_item(), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
//
	destoroy++;
	if ( (NUM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
	{
		destoroy = 0;
		if (rand_percent(30))
		{
			item_create(/*zzz*/ src, SP_ITEM_00_P001, 1, ITEM_MOVE_FLAG_06_RAND_XY);
		}
	}
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_yukari2(SPRITE *src)
{
	YUKARI2_DATA *data = (YUKARI2_DATA *)src->data;
	switch (data->state)
	{
	case 0: /* nach unten */
		if (src->y256 >= data->max_y256)
		{
			data->state=1;
			data->angle512 = atan_512(t256(0)-src->y256,t256(GAME_WIDTH/2)-src->x256); 			//�E�B���h�E���̕ύX
			if (t256(1.5/*2.0*/) < data->speed256)
			{	/*����*/
				data->speed256 -= t256(0.2);
			//	data->speed256 = t256(2)/*3+difficulty*/;								// [***090201		�ύX
			}
		}
		if (data->level)
		{
		//	if (0==(ra_nd()%(105-(data->level-2+difficulty)*10)))	// [***090126		�኱�ύX
		//	if (0==(ra_nd()%(11-(data->level-2+difficulty) ))) 	// [***090126		�኱�ύX
		//	if (0==(ra_nd()%(16-(data->level-2+difficulty) ))) 	// [***090126		�኱�ύX
		//	if ((data->level-2+difficulty) >= (ra_nd()&(16-1)))	// [***090126		�኱�ύX
			if ((data->level-2+difficulty) >= (ra_nd()&(64-1)))	// [***090126		�኱�ύX
			{
				bullet_create_aka_maru_jikinerai(src, t256(1)+t256(difficulty)+(data->level<<6)/*t256(data->level/3)*/ );
			}
		}
		break;
	case 1:
	//	if (src->y256 < -((src->h128+src->h128)) )
		if (src->y256 < -(t256(16)) )
		{
		//	src->flags	&= (~(SP_FLAG_VISIBLE));
			src->type = SP_DELETE;	/* �����܂� */
		}
		break;
	}
	/*���Ă邪������ƈႤ--�ȉ�rwingx.c�Ɠ���*/
/* CCW�̏ꍇ */
	src->x256+=((sin512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
	src->y256+=((cos512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
//	src->anim_frame=(deg_512_to_360(data->angle512+deg_360_to_512(270))/10)%36;
//	src->anim_frame = ((((data->angle512/*+deg_360_to_512(270)*/)&(512-1))*(36/2))>>8);
//	src->anim_frame = ((((data->angle512/*+deg_360_to_512(270)*/)&(512-1))*(32/2))>>8);
//	src->anim_frame = ((((data->angle512/*+deg_360_to_512(270)*/)&(512-1)))>>4);
	src->yx_anim_frame = ( ((data->angle512>>3)&(0x30)) | ((data->angle512>>4)&(0x07)) );
/* "yukari8x4.png"
data->angle512       a bcde ----
src->yx_anim_frame    yyyy xxxx
src->yx_anim_frame    --ab -cde
*/
}
#if 0
	case 0: 	/* �E�ֈړ��� */
		if (src->x >= data->clip_right)
		{	data->state=1;}
		break;
	case 1: 	/* �E����ŉ�]�� */
		data->angle512 -= data->turnspeed512/**fps_fa ctor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
		if (data->angle512 <= deg_360_to_512(180))
		{
			data->angle512 = deg_360_to_512(180);
			data->state=2;
			if (data->level>0)
			{	bullet_create_aka_maru_jikinerai(s, t256(3)+(data->level<<7) );}
		}
		break;
	case 2: 	/* ���ֈړ��� */
		if (src->x <= data->clip_left)
		{	data->state=3;}
		break;
	case 3: 	/* ������ŉ�]�� */
		data->angle512 += data->turnspeed512/**fps_fa ctor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
		if (data->angle512 >= deg_360_to_512(360) )
		{
			data->angle512 = deg_360_to_512(0);
			data->state=0/*4*/;
			if (data->level>0)
			{	bullet_create_aka_maru_jikinerai(s, t256(3)+(data->level<<7) );}
		}
		break;

	case 8: 	/* �E�ֈړ��� */
		if (src->x > GAME_WIDTH)		//�E�B���h�E���̕ύX
		{	src->flags &= (~(SP_FLAG_VISIBLE));}
		break;
#endif

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_yukari2(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	destoroy = 0;
	static int static_last=0;/* �o��ʒu�̐؂�ւ� */
	static_last++;
	static_last &= 1;
//
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s						= sprite_add_res(BASE_YUKARI32_PNG);	//s->anim_speed=0;/*36"mi ng.png"*/
		s->type 				= SP_ZAKO/*_03_YUKARI2*/;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_yukari2;
		s->callback_loser		= lose_yukari2;
		s->callback_hit_enemy	= callback_hit_zako;
		if (0==static_last) {	s->x256=t256(0);								}	//�E�ォ��o��
		else				{	s->x256=t256(GAME_WIDTH)-(s->w128+s->w128); 	}	//���ォ��o��
		s->y256 				= -i*(s->h128+s->h128);
		YUKARI2_DATA *data;
		data					= mmalloc(sizeof(YUKARI2_DATA));
		s->data 				= data;
		data->angle512			= atan_512((t256(GAME_HEIGHT)-((s->h128+s->h128))-t256(60))-s->y256,t256(GAME_WIDTH/2)-s->x256);	//�E�B���h�E���̕ύX
		data->max_y256			= (t256(GAME_HEIGHT)-((s->h128+s->h128))-t256(60));
		data->speed256			= (t256(2.5/*3.0*/)+((difficulty)<<4) ) /*4*/;/*�n�߂����͑���*/
		data->state 			= 0;
		/*data->base.*/s->base_score		= score(5*2);
		/*data->base.*/s->base_health		= 1+(difficulty<<2);
		data->level 			= lv;
	}
}
#undef NUM_OF_ENEMIES
