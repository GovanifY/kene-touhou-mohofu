
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
	ENEMY_BASE base;
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

static void lose_meido1(SPRITE *s)
{
	item_create(s, (enemy_get_random_item()), 1, ITEM_MOVE_FLAG_06_RAND_XY);
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_meido1(SPRITE *s)
{
	MEIDO1_DATA *data = (MEIDO1_DATA *)s->data;
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
			data->angleCCW512	= atan_512(player->y256-s->y256,player->x256-s->x256);
		//	data->speed256	= /*t256(3+(data->level>>1))*/ /*(4+data->level)*/;/*���߂�*/
		//	data->wait		= 50;/*???*/
		}
		break;
	case 2/*3*/: /* move (data->wait) ticks */
		data->speed256	+= t256(0.1);/*�������Ȃ��� �v���C���[�ɓ˂�����ł���*/
		if (s->y256 >= t256(GAME_HEIGHT-150))
		{
			if (data->level)
			{	bullet_create_aka_maru_jikinerai(s, t256(1.0)+t256(data->level)/*t256(3)+t256(data->level)*/ );}/*���߂�*/
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
		if ((s->x256<-((s->w128))) || (s->x256 > t256(GAME_WIDTH)) ||
			(s->y256<-((s->h128))) || (s->y256 > t256(GAME_HEIGHT)))
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
		s					= sprite_add_res(BASE_AKA_MEIDO08_PNG); 	//s->anim_speed = 0;/*37"rw ingx8.png""rw ingx.png"*/
		s->type 			= SP_ZAKO/*_12_MEIDO1*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_meido1;
		s->callback_loser	= lose_meido1;
		MEIDO1_DATA *data;
		data				= mmalloc(sizeof(MEIDO1_DATA));
		s->data 			= data;
		data->base.score	= score(25*2)*(1+lv);
		data->base.health	= 1+lv+(difficulty<<2);
#if 0
/* CW�̏ꍇ */
		data->angle512		= deg_360_to_512(90);// ������	deg_360_to_512(270);/*������H�H*/
#else
/* CCW�̏ꍇ */
		data->angleCCW512	= deg_360_to_512CCW(0/*360-90*/);// ������ deg_360_to_512CCW(360-270);/*������H�H*/
#endif
		data->speed256		= t256(0);//t256(0.5);
		data->state 		= 0;
		data->wait			= 100;//20;
		data->level 		= lv;
		s->x256 			= t256(GAME_WIDTH/8)*i+t256(10/*-20*/); 	//�E�B���h�E���̕ύX
		s->y256 			= t256(-32)/*-50*/;
	}
}
#undef NUM_OF_ENEMIES
