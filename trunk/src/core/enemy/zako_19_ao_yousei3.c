
#include "bullet_object.h"

/*---------------------------------------------------------
		"�d��3",		"FAIRY",	(r27����V�K�ǉ�)
	-------------------------------------------------------
	user_x: 	���x256[speed256]
	user_y: 	���ʒu[dots]
---------------------------------------------------------*/

#define USE_X_HOUKOU (0)

typedef struct
{
//	ENEMY_BASE base;
	int state;
	int time_out;

} AO_YOUSEI3_DATA;

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
#define AO_YOUSEI3_anime_houkou 	tmp_angleCCW512

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_ao_yousei3(SPRITE *src)
{
//	case SP_ZAKO_16_AO_YOUSEI1:
	//	if (rand_percent(30))	{	put_item=(c->x, c->y, SP_ITEM_00_P001&0xff);}
	//item_create(src, (((ra_nd()&0x07)&0xff)|SP_ITEM_00_P001), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
	item_create(src, (enemy_get_random_item()), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
}

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
	AO_YOUSEI3_DATA *data = (AO_YOUSEI3_DATA *)src->data;
//
	data->time_out--;
//	if (1 > data->time_out)
//	{
//		src->type = SP_DELETE;	/* �����܂� */
//	}
	switch (data->state)
	{
	case ST00:	/* �o�� �U�� */
		switch (data->time_out)
		{
		case 16:
			data->state++;
			break;
		case 250:/* �|����悤�ɂ��� */
			/*data->base.*/src->base_health = (1+(difficulty)); 	/*(3+(difficulty))*/	/*1+(difficulty<<2)*/
			break;
		}
		/* �U�� */
		if (0==((data->time_out)&0x3f))
		{
			if (difficulty)
			{
			send1_obj->x256 = src->x256;
			send1_obj->y256 = src->y256;
			#if 1
			/* ���Ƃŗv�� */
	//		send1_obj->h128 = src->h128;
	//		send1_obj->w128 = src->w128;
			#endif
			//	bullet_create_n_way_dan_sa_type(src,
				send1_obj->BULLET_REGIST_speed256			=		(t256(2.0)+((difficulty)<<6));
				send1_obj->BULLET_REGIST_angle512			=		ANGLE_JIKI_NERAI_DAN;
				send1_obj->BULLET_REGIST_div_angle512		=		(int)(512/(8));
				send1_obj->BULLET_REGIST_bullet_obj_type	=		BULLET_KUNAI12_04_YUKARI+((ra_nd())&7);
				send1_obj->BULLET_REGIST_n_way				=		(8);
				bullet_regist_basic();
			}	/*�Ȃ�ׂ����ʉ�*/
		//	if (difficulty) {	bullet_create_aka_maru_jikinerai(s, (512-data->time_out)+t256(difficulty)); }
					/*(t256(3.0)+((difficulty)<<6))*/	/*((difficulty<<(1+8)))*/
					/*t256((difficulty<<2))*/ /*5*/ 	/*"kugel.png", 0*/
					/*at an2(player->y-src->y+player->h/2,player->x-src->x-player->w/2)*/
		}
		break;
	case ST01:	/* ���E */

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
	src->x256 += (src->vx256)/**fps_fa ctor*/;
	#if (1==USE_X_HOUKOU)
	src->y256 += (src->vy256)/**fps_fa ctor*/;
	#endif

//	src->yx_an im_frame = (8+((data->time_out>>2)&(8-1)));
//	src->yx_an im_frame = (src->AO_YOUSEI3_anime_houkou | ((data->time_out>>2)&(4-1)));
/*
data->time_out		 ---a bc--
src->yx_an im_frame  yyyy xxxx
src->yx_an im_frame  --1a --bc
*/
	if (SP_DELETE != src->type)
	{
		src->type				= (TEKI_16_YOUSEI11)+(src->AO_YOUSEI3_anime_houkou | ((data->time_out>>2)&(4-1)));
	}
}
//		data->speed256 += 2;
	//	data->angleCCW512 += 2;
	//	if (255 < data->angleCCW512)
	//	{
	//		data->angleCCW512 = 255;/*�^��*/
	//	}
	//	if (1 > src->y256)

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_ao_yousei3(STAGE_DATA *l)/*int lv*/
{
	{
		SPRITE *s;
	//	s						= sp rite_add_res(BASE_AO_YOUSEI24_PNG);/*7"fairy.png"*/
		s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		s->type 				= /*SP_ZAKO*/TEKI_16_YOUSEI11/*_16_AO_YOUSEI1*/;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_ao_yousei3;
		s->callback_loser		= lose_ao_yousei3;
		s->callback_hit_enemy	= callback_hit_zako;
//		s->anim_speed			= 0/*4*/;
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
				s->AO_YOUSEI3_anime_houkou	= ((0x00)>>2);
			}
			else
			{/* [���� user_y ���������̏ꍇ�A���ֈړ�(��)] [����y���������̏ꍇ�A��ֈړ�(��)] */
				s->x256 			= t256(360);/* 360 > 352(x_max) > 272(y_max) */
				s->AO_YOUSEI3_anime_houkou	= ((0x50)>>2);
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
			s->AO_YOUSEI3_anime_houkou		= ((0x20)>>2);
		}
		#endif
		//
		AO_YOUSEI3_DATA *data;
		data					= mmalloc(sizeof(AO_YOUSEI3_DATA));
		s->data 				= data;
		data->time_out			= 320;
		data->state 			= 0;
		/*data->base.*/s->base_score		= score(20*2);
		/*data->base.*/s->base_health		= 999;/* �o�ꎞ�͓|���Ȃ� */	/*3+(difficulty)*/	/*1+(difficulty<<2)*/
	}
}
