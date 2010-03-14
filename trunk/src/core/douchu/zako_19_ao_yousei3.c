
#include "douchu.h"

/*---------------------------------------------------------
		"�d��3",		"FAIRY",	(r27����V�K�ǉ�)
	-------------------------------------------------------
	user_x: 	���x256[speed256]
	user_y: 	���ʒu[dots]
---------------------------------------------------------*/

//#define tx256		user_data00
//#define ty256		user_data01
#define time_out	user_data02
//#define speed256	user_data03
#define state		user_data04

#define USE_X_HOUKOU (0)


//	int angleCCW512;	/* �����O�Ƃ��č����(�t���)�̊p�x */
//	int speed256;

//#define AO_YOUSEI3_aaa	vy256
//
//	int anime_houkou;

/*---------------------------------------------------------
	���p�ϐ�
---------------------------------------------------------*/

/* [�V�X�e��]�ێ��p�p�x(����0�x�ō����(�����v���)) */
/* ���̃U�R�ł�(���̕ϐ���)�g��Ȃ��̂ŋ��p(���p)����B */
#define KARASU3_anime_houkou	tmp_angleCCW512


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/
enum
{
	ST00 = 0,
	ST01,
};

static void move_ao_yousei3(SPRITE *src)
{
	src->time_out--;
//	if (0 > src->time_out)
//	{
//		src->type = SP_DELETE;	/* �����܂� */
//	}
	switch (src->state)
	{
	case ST00:	/* �o�� �U�� */
		switch (src->time_out)
		{
		case 16:
			src->state++;
			break;
		}
		/* �U�� */
		if (0==((src->time_out)&0x3f))
		{
			if (difficulty)
			{
				send1_obj->x256 = src->x256;
				send1_obj->y256 = src->y256;
				send1_obj->BULLET_REGIST_speed256			=		(t256(2.0)+((difficulty)<<6));
				send1_obj->BULLET_REGIST_angle512			=		ANGLE_JIKI_NERAI_DAN;
				send1_obj->BULLET_REGIST_div_angle512		=		(int)(512/(8));
				send1_obj->BULLET_REGIST_bullet_obj_type	=		BULLET_KUNAI12_04_YUKARI+((ra_nd())&7);
				send1_obj->BULLET_REGIST_n_way				=		(8);
				bullet_regist_basic();
			}	/*�Ȃ�ׂ����ʉ�*/
		}
		break;
	case ST01:	/* ���E */
		gamen_gai_nara_osimai(src);/* ��ʊO�Ȃ炨���܂� */
		break;
	}
	/* �ړ����� */
	src->x256 += (src->vx256);/*fps_factor*/
	#if (1==USE_X_HOUKOU)
	src->y256 += (src->vy256);/*fps_factor*/
	#endif

//	src->yx_an im_frame = (8+((src->time_out>>2)&(8-1)));
//	src->yx_an im_frame = (src->KARASU3_anime_houkou | ((src->time_out>>2)&(4-1)));
/*
src->time_out		 ---a bc--
src->yx_an im_frame  yyyy xxxx
src->yx_an im_frame  --1a --bc
*/
	if (SP_DELETE != src->type)
	{
		src->type				= (TEKI_12_YOUSEI1_1)+(src->KARASU3_anime_houkou | ((src->time_out>>2)&(4-1)));
		src->type				= (TEKI_12_YOUSEI1_1)+(src->KARASU3_anime_houkou | ((src->time_out>>2)&(4-1)));
	}
}
//		src->speed256 += 2;
	//	src->angleCCW512 += 2;
	//	if (255 < src->angleCCW512)
	//	{
	//		src->angleCCW512 = 255;/*�^��*/
	//	}
	//	if (0 > src->y256)

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_ao_yousei3(STAGE_DATA *l)
{
	{
		SPRITE *s;
		s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		s->type 				= TEKI_12_YOUSEI1_1;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_ao_yousei3;
		s->callback_loser		= lose_random_item;
		s->callback_hit_enemy	= callback_hit_zako;
		//
		s->y256 				= ((l->user_y)<<8);
		s->vx256			= ((l->user_x));
		#if (1==USE_X_HOUKOU)
		s->vy256			= (0);
		#endif
		//
		{
			if (0 < (s->vx256))
			{/* [���� user_y ���������̏ꍇ�A�E�ֈړ�(��)] [����y���������̏ꍇ�A���ֈړ�(��)] */
				s->x256 			= t256(-30);
			//	s->KARASU3_anime_houkou = ((0x00)>>2);
				s->KARASU3_anime_houkou = ((0x30)>>2);/*�E�����J���X*/
			}
			else
			{/* [���� user_y ���������̏ꍇ�A���ֈړ�(��)] [����y���������̏ꍇ�A��ֈړ�(��)] */
				s->x256 			= t256(360);/* 360 > 352(x_max) > 272(y_max) */
				s->KARASU3_anime_houkou = ((0x50)>>2);/*�������J���X*/
			}
		}
		//
		#if (1==USE_X_HOUKOU)
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
			s->KARASU3_anime_houkou 	= ((0x20)>>2);
		}
		#endif
		//
		s->time_out			= 320;
		s->state 			= 0;
		/*s->base.*/s->base_score		= score(20*2);
		/*s->base.*/s->base_hp 		= ((8*1)+(1/*di fficulty*/<<(3))); 	/*(3+(di fficulty))*/	/*1+(di fficulty<<2)*/
	}
}
