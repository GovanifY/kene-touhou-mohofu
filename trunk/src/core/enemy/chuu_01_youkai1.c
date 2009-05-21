
#include "enemy.h"

/*---------------------------------------------------------
		"�d��1",		"GFAIRY",
	-------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE b;
	int state;
	int wait1;	//�s��wait
	int wait2;	//�U��wait
	int nnn;
	SPRITE *s2;
} YOKAI1_DATA;

/*---------------------------------------------------------

---------------------------------------------------------*/

void enemy_zako_yokai1_nonshield_hitbyweapon(SPRITE *s, SPRITE *t/*, int angle*/)
{
	/*
		s = �{�X�{�� boss sprite
		t = �v���C���[�̒e player's weapon
	*/
	YOKAI1_DATA *data=(YOKAI1_DATA *)s->data;
	WEAPON_BASE *w=(WEAPON_BASE *)t->data;
	explosion_add_type(t->x256,t->y256,/*0,*/EXPLOSION_MINI00);
	data->b.health -= w->strength;
	if (data->b.health <= 0)
	{
		item_from_bullets(SP_ITEM_05_HOSI);
	//
		item_create(s, SP_ITEM_05_HOSI/*SP_ITEM_06_TENSU*/, 7, ITEM_MOVE_FLAG_01_COLLECT);/*���_���o��*/
		player_add_score(data->b.score);
//
		explosion_add_circle(s, 0);
		s->type=SP_DELETE;
		data->s2->type=SP_DELETE;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_yokai1(SPRITE *s)
{
	YOKAI1_DATA *data=(YOKAI1_DATA *)s->data;
	switch (data->state)
	{
	case 0:/* �ォ��o�� */
		data->s2->y256 += t256(2);		/**fps_fa ctor*/
		s->y256 += t256(2); 		/**fps_fa ctor*/
		if (t256(50) < s->y256) 	{				data->state=1;	}
		break;
	case 1:
		data->wait1--;
		if (1 > data->wait1)
		{
			if (2 != data->nnn) {	data->wait1=150;	data->state=2;	}
			else				{					data->state=5;	}
		}
		break;
	case 2:
		data->wait1--;
		if (1 > data->wait1)	{	data->wait1=30; data->state=3;	/*data->wait2=8 10*/;	}
		else
		{
			data->wait2--;
			if (1 > data->wait2)
			{
				/* 0: 20 == 20-(0*4) */
				/* 1: 16 == 20-(1*4) */
				/* 2: 12 == 20-(2*4) */
				/* 3:  8 == 20-(3*4) */
				data->wait2 = (20-((difficulty)<<2))/*8*/ /*10*/;
				play_voice_auto_track(VOICE14_YOKAI1_E_SHOT01);
				bullet_create_n_way_dan_type(s, (t256(3.0)+((difficulty)<<6)), ANGLE_JIKINERAI_KISUDAN, BU_TYPE01_KUGEL_PNG/*"kugel.png", 0*/, 8);
			}
		}
		break;
	case 3:
		data->wait1--;
		if (1 > data->wait1)	{	data->wait1=100;	data->state=4;	}
		break;
	case 4:
		data->wait1--;
		if (1 > data->wait1)	{	data->wait1=30; data->state=1;	/*data->wait2=8 10*/;	data->nnn++;	}
		else
		{
			data->wait2--;
			if (1 > data->wait2)
			{
				data->wait2 = (20-((difficulty)<<2))/*8*/ /*10*/;
				play_voice_auto_track(VOICE14_YOKAI1_E_SHOT01);
				bullet_create_n_way_dan_type(s, t256(3.25)+((difficulty)<<6), ANGLE_JIKINERAI_KISUDAN, BU_TYPE01_KUGEL_PNG/*"kugel.png", 0*/, 7);
			}
		}
		break;
	case 5:/* ��֑ޏ� */
		data->s2->y256 -= t256(2);		/**fps_fa ctor*/
		s->y256 -= t256(2)/**fps_fa ctor*/;
		if ( -((s->h)<<8) > s->y256)
		{
			s->type=SP_DELETE;
			data->s2->type=SP_DELETE;
		}
		break;
	}
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

static SPRITE *create_usiro_no_mahojin(SPRITE *s)	//�����w�O���t�B�b�N����
{
	SPRITE *s2; 		// �����w�O���t�B�b�N�̃X�v���C�g
	s2					= sprite_add_res(BASE_MAHOUJIN_0_PNG);		//s2->anim_speed	= 0;/*"boss04-lo.png"*/
	s2->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	s2->anim_frame		= 0;
	s2->type			= SP_PLAYER2;
	s2->alpha			= 0;
	s2->x256			= s->x256+((s->w-s2->w)<<7);
	s2->y256			= s->y256+((s->h-s2->h)<<7);
	return (s2);
}

void add_chuu_youkai1(STAGE_DATA *l)/*int lv*/
{
	SPRITE *s;
	s					= sprite_add_res(BASE_GREAT_FAIRY02_PNG);	s->anim_speed	= 3;
	s->type 			= SP_CHUU/*SP_ZAKO_YOKAI1*/;
	s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	s->callback_mover	= move_yokai1;
	s->y256 			= t256(-30);
//	s->x256 			= (l->user_y)*t256(35)+t256(40);
//	s->x256 			= ((l->user_y)*t256(32))+t256(48);
	s->x256 			= ((l->user_y)<<(8+5))+t256(48);
	YOKAI1_DATA *data;
	data				= mmalloc(sizeof(YOKAI1_DATA));
	s->data 			= data;
	data->state 		= 0;
	data->wait1 		= 30;
	data->wait2 		= 10;/*0*/
	data->b.score		= score(100)+score(100)*difficulty;
	data->b.health		= 200+(difficulty<<4);	/* easy�ł����݊�����ۂÂ���ׂ� 200 �͕K�v */ 	// 50+150*difficulty;
	data->nnn			= 0;
	data->s2			= create_usiro_no_mahojin(s);
}
