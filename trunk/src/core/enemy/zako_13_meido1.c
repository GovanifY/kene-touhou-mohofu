
#include "bullet_object.h"

/*---------------------------------------------------------
	"���C�h1",		"RWINGX",	rwingx
	-------------------------------------------------------
	�ォ�牡�ɓ��Ԋu�Ő��񂵂Č���A�v���C���[�Ɍ�����
	������x�߂Â��ƁA�e�u���Ĕ��]���ē�����B
	-------------------------------------------------------
	ok
---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	int angleCCW512;
	int speed256;
	int state;
	int level;
//
	int wait;
} MEIDO1_DATA;


#define NUM_OF_ENEMIES (8)

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_meido1(SPRITE *src)
{
	item_create(src, (enemy_get_random_item()), 1, ITEM_MOVE_FLAG_06_RAND_XY);
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_meido1(SPRITE *src)
{
	MEIDO1_DATA *data = (MEIDO1_DATA *)src->data;
	switch (data->state)
	{
//	case 0: /* sleep(data->wait) ticks */
//	//	if (data->wait > 0)
//	//	{
//	//		data->wait -= 1/*fps_fa ctor*/;
//	//	}
//	//	else
//		{
//			/* �������o�� / langsam nach unten */
//			data->state 	= 1;
//			data->angle512	= deg_360_to_512(90);/*������*/
//		//	data->speed256	= t256(1.0);
//			data->wait		= 100;
//		}
//		break;
	case 0/*1*/: /* move (data->wait) ticks */
		if (data->wait > 0)
		{
			data->wait -= 1/*fps_fa ctor*/;
			if ( data->speed256 < t256(1.0))
			{
				data->speed256 += 5/*t256(0.1)*/;/*����*/
			}
		}
		else
		{
			data->state 	= 1/*2*/;
			data->wait		= 60;
		//	data->speed256	= t256(0);
		}
		break;
	case 1/*2*/: /* sleep(data->wait) ticks */
		if (data->wait > 0)
		{
			data->wait -= 1/*fps_fa ctor*/;
			if (0 < data->speed256)
			{
				data->speed256 -= 5/*t256(0.1)*/;/*����*/
			}
		}
		else
		{
			/* �v���C���[�ɓ˂�����ł��� / schnell richtung player */
			data->state 	= 2/*3*/;
			data->angleCCW512	= atan_512(player->y256-src->y256,player->x256-src->x256);
		//	data->speed256	= /*t256(3+(data->level>>1))*/ /*(4+data->level)*/;/*���߂�*/
		//	data->wait		= 50;/*???*/
		}
		break;
	case 2/*3*/: /* move (data->wait) ticks */
		data->speed256	+= t256(0.1);/*�������Ȃ��� �v���C���[�ɓ˂�����ł���*/
		if (src->y256 >= t256(GAME_HEIGHT-150))
		{
			if (data->level)
			{	bullet_create_aka_maru_jikinerai(src, t256(1.0)+t256(data->level)/*t256(3)+t256(data->level)*/ );}/*���߂�*/
			data->state = 3/*4*/;
#if 0
/* CW�̏ꍇ */
			data->angle512 += deg_360_to_512(180);/* ���]���ē����� */
#else
/* CCW�̏ꍇ */
			data->angleCCW512 += deg_360_to_512CCW(180/*360-180*/);/* ���]���ē����� */
#endif
			mask512(data->angleCCW512);// data->angle %= deg_360_to_512(360);
		//	data->speed256 = /*t256(6+data->level)*/;/*���߂�*/
		}
		break;
	case 3/*4*/:
		if ((src->x256<-((src->w128))) || (src->x256 > t256(GAME_WIDTH)) ||
			(src->y256<-((src->h128))) || (src->y256 > t256(GAME_HEIGHT)))
		{
			src->type = SP_DELETE;	/* �����܂� */
		}
		break;
	}
	/*�ȉ�rwingx.c�Ɠ���*/
/* CCW�̏ꍇ */
	src->x256+=((sin512((data->angleCCW512))*data->speed256)>>8)/**fps_fa ctor*/;
	src->y256+=((cos512((data->angleCCW512))*data->speed256)>>8)/**fps_fa ctor*/;
//	src->anim_frame=(deg_512_to_360(data->angle512+deg_360_to_512(270))/10)%36;
//	src->anim_frame = ((((data->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
#if 1
/* [CCW�̏ꍇ(�V)] CW�̏ꍇ */
	src->anim_frame = ((((data->angleCCW512)&(512-1)))>>6);/*"rw ingx8.png"*/
	/* �� */
#else
/* CCW�̏ꍇ */
	/* �V(�܂�����ĂȂ�) */
	//src->anim_frame = ((((data->angleCCW512)&(512-1)))>>6);/*"rw ingx8.png"*/
	/*��������݊�*/
	{int aaa512;
		aaa512 = 128+ 512 - data->angleCCW512;
		mask512(aaa512);
		src->yx_anim_frame = (((aaa512))>>(6));
	}
#endif
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_meido1(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s						= sprite_add_res(BASE_AKA_MEIDO08_PNG); 	//s->anim_speed = 0;/*37"rw ingx8.png""rw ingx.png"*/
		s->type 				= SP_ZAKO/*_12_MEIDO1*/;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_meido1;
		s->callback_loser		= lose_meido1;
		s->callback_hit_enemy	= callback_hit_zako;
		MEIDO1_DATA *data;
		data					= mmalloc(sizeof(MEIDO1_DATA));
		s->data 				= data;
		/*data->base.*/s->base_score		= score(25*2)*(1+lv);
		/*data->base.*/s->base_health		= 1+lv+(difficulty<<2);
#if 0
/* CW�̏ꍇ */
		data->angle512			= deg_360_to_512(90);// ������	deg_360_to_512(270);/*������H�H*/
#else
/* CCW�̏ꍇ */
		data->angleCCW512		= deg_360_to_512CCW(0/*360-90*/);// ������ deg_360_to_512CCW(360-270);/*������H�H*/
#endif
		data->speed256			= t256(0);//t256(0.5);
		data->state 			= 0;
		data->wait				= 100;//20;
		data->level 			= lv;
		s->x256 				= t256(GAME_WIDTH/8)*i+t256(10/*-20*/); 	//�E�B���h�E���̕ύX
		s->y256 				= t256(-32)/*-50*/;
	}
}
#undef NUM_OF_ENEMIES
