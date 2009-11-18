
#include "bullet_object.h"

/*---------------------------------------------------------
		"�d��2",		"GFAIRY",
	-------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	BOSS_BASE boss_base;
//------------ �ړ��֘A
	int state1;
	int time_out;	/* �s��wait */
	int repeat; 	/* �J��Ԃ��� */
	int start_danmaku;
	int start_x256;	/* �o��x���W */
	SPRITE *s2;
} YOKAI1_DATA;
//	int wait2;	/* �U��wait */


/*---------------------------------------------------------
	s = �{�X�{�� boss sprite
	t = �v���C���[�̒e player's weapon
---------------------------------------------------------*/

static void callback_hit_youkai1(SPRITE *src/*�G����*/, SPRITE *tama/*���e*/)
{
	dummy_obj->x256 = tama->x256;
	dummy_obj->y256 = tama->y256;
	bakuhatsu_add_type_ddd(dummy_obj/*tama->x256,tama->y256*/,/*0,*/BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);
//
	YOKAI1_DATA *data	= (YOKAI1_DATA *)src->data;
//	WEAPON_BASE *w		= (WEAPON_BASE *)tama->data;
	/*data->base.*/src->base_health -= /*w->*/tama->base_weapon_strength;
	if (/*data->base.*/src->base_health <= 0)
	{
		bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
	//
		item_create(src, SP_ITEM_06_TENSU, 7, ITEM_MOVE_FLAG_06_RAND_XY/*ITEM_MOVE_FLAG_01_COLLECT*/);/*�_�����o��*/	/* ����΂� */
		player_add_score(/*data->base.*/src->base_score);
//
		bakuhatsu_add_circle(src, 0);
		src->type				= SP_DELETE;
		data->s2->type			= SP_DELETE;
		/* �R�[���o�b�N�o�^ */
		src->callback_hit_enemy = NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

//static void lose_youkai1(SPRITE *src)
//{
//
//}


/*---------------------------------------------------------
	�e�����I���܂ő҂B
	-------------------------------------------------------

---------------------------------------------------------*/

static void danmaku_state_check_holding(SPRITE *src)
{
	YOKAI1_DATA *data = (YOKAI1_DATA *)src->data;
	if (DANMAKU_00 == data->boss_base.danmaku_type)
	{
		data->state1++/* = nextstate*/;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
enum
{
	SS00 = 0,
	SS01,
	SS02,
	SS03,
	SS04,
};
//	ST02,
//	ST02bbb,
//	ST03,

#if 0
			;/* 8�����e */
			data->wait2--;
			if (1 > data->wait2)
			{
				/* 0: 20 == 20-(0*4) */
				/* 1: 16 == 20-(1*4) */
				/* 2: 12 == 20-(2*4) */
				/* 3:  8 == 20-(3*4) */
				data->wait2 = (20-((difficulty)<<2))/*8*/ /*10*/;
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
				#else
				voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*�e�L�g�[*/
				#endif
				bullet_create_n_way_dan_sa_type(src,
					(t256(3.0)+((difficulty)<<6)),
					ANGLE_JIKI_NERAI_DAN,
					(int)(512/24),
					BULLET_UROKO14_03_MIDORI,
					8);
			}
#endif

#if 0
			;/* 7��e */
			data->wait2--;
			if (1 > data->wait2)
			{
				data->wait2 = (20-((difficulty)<<2))/*8*/ /*10*/;
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
				#else
				voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*�e�L�g�[*/
				#endif
				bullet_create_n_way_dan_sa_type(src,
					t256(3.25)+((difficulty)<<6),
					ANGLE_JIKI_NERAI_DAN,
					(int)(512/24),
					BULLET_UROKO14_04_MIZUIRO,
					7);
			}
#endif
static void move_youkai1(SPRITE *src)
{
	YOKAI1_DATA *data=(YOKAI1_DATA *)src->data;
	data->time_out--;
	switch (data->state1)
	{
	case SS00:	/* �ォ��o�� */
		data->s2->y256	+= t256(2); 	/**fps_fa ctor*/
		src->y256		+= t256(2); 	/**fps_fa ctor*/
		if (data->start_x256 < src->y256)	{	data->time_out=30;	data->state1 = SS03; }
		break;
//---------
	case SS01:	/* �e���Z�b�g */
			/*data->wait2=8 10*/
			data->state1++;/* = SS02;*/
			;/* 8�����e */
			;/* 7��e */
			data->boss_base.danmaku_type		= data->start_danmaku/*DANMAKU_01*/;			/* �S�e�����Z�b�g */
			data->boss_base.danmaku_time_out	= DANMAKU_01_SET_TIME;	/* �S�e���̔������� */
		break;
	case SS02:	/* �S�e�����I���܂őҋ@ */
		danmaku_state_check_holding(src);	/*, SA07*/ /*, SAKUYA_ANIME_04_CENTER_A*/
		break;
	case SS03:	/* ���΂��҂� */
		if (1 > data->time_out)
		{
			data->time_out=100/*150*/;
			/* �K��̌J��Ԃ��񐔂��Ȃ�����ޏ� */
			data->repeat--;
			if ((0) != data->repeat)	{	data->state1	= SS01; 		}/* �J��Ԃ� */
			else						{	data->state1++/* = SS04*/;		}/* ��֑ޏ� */
		}
		break;
//---------
	case SS04:	/* ��֑ޏ� */
		data->s2->y256	-= t256(2); 	/**fps_fa ctor*/
		src->y256		-= t256(2); 	/**fps_fa ctor*/
		if ( -((src->h128+src->h128)) > src->y256)
		{
			src->type = SP_DELETE;
			data->s2->type = SP_DELETE;
		}
		break;
	}
//	src->an im_frame 	= ((data->time_out&0x10)>>4);
	if (SP_DELETE != src->type)
	{
		src->type 			= TEKI_54_CHOU1+((data->time_out&0x10)>>4);
	}
//
	data->s2->m_angleCCW512++;
	mask512(data->s2->m_angleCCW512);
//
	danmaku_generator(src); /* �e������ */
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

static SPRITE *create_usiro_no_mahojin(SPRITE *src) //�����w�O���t�B�b�N����
{
	SPRITE *s2; 		// �����w�O���t�B�b�N�̃X�v���C�g
//	s2					= sp rite_add_res(BASE_MAHOUJIN_0_PNG);		//s2->anim_speed	= 0;/*"boss04-lo.png"*/
//	s2					= sprite_add_gu(TAMA_TYPE_BULLET_JIPPOU32_PNG); 	//s2->anim_speed	= 0;/*"boss04-lo.png"*/
	s2					= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG); 			//s2->anim_speed	= 0;/*"boss04-lo.png"*/
	s2->type			= TEKI_51_MAHOJIN1/*SP_MUTEKI*/;
//	s2->type			= MAHOU_JIN_00_aaa/*SP_MUTEKI*/;
	s2->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	s2->an im_frame		= 0;
	s2->color32 		= 0xaa0000ff;		/* �Ԃ��ۂ� */		/*	s2->alpha			= 0x00;*/
	s2->x256			= src->x256/*+((src->w128-s2->w128))*/-t256(8);
	s2->y256			= src->y256/*+((src->h128-s2->h128))*/-t256(8);
	return (s2);
}

void add_chuu_youkai2(STAGE_DATA *l)/*int lv*/
{
	{
		SPRITE *s1;
		SPRITE *s2;
	//	s1->x256				= ((l->user_x)*t256(35))+t256(40);
	//	s1->x256				= ((l->user_x)*t256(32))+t256(48);
	//	s1->x256				= ((l->user_x)<<(8+5))+t256(48);
		dummy_obj->x256 		= ((l->user_x)<<(8));
		dummy_obj->y256 		= t256(-30);

		s2						= create_usiro_no_mahojin(dummy_obj);

//		s1						= sp rite_add_res(BASE_GREAT_FAIRY02_PNG);	//s->anim_speed = 3;
		s1						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);	//s->anim_speed = 3;
		s1->type				= /*SP_CHUU*/TEKI_54_CHOU1/*SP_ZAKO_YOKAI1*/;
//		s1->type				= SP_CHUU/*SP_ZAKO_YOKAI1*/;
		s1->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s1->callback_mover		= move_youkai1;
	//	s1->callback_loser		= lose_youkai1;
		s1->callback_hit_enemy	= callback_hit_youkai1; 	/* �R�[���o�b�N�o�^ */
		s1->x256				= dummy_obj->x256;
		s1->y256				= dummy_obj->y256;
//
		/*data->base.*/s1->base_health		= 200+(difficulty<<4);	/* easy�ł����݊�����ۂÂ���ׂ� 200 �͕K�v */ 	// 50+150*difficulty;
		/*data->base.*/s1->base_score		= score(100)+score(100)*difficulty;
//
		YOKAI1_DATA *data;
		data								= mmalloc(sizeof(YOKAI1_DATA));
		s1->data						= data;
		data->state1						= SS00;
//		data->time_out						= 30;
//		data->wait2 						= 10;/*0*/
		data->repeat						= (2+2+1);
		data->start_x256					= ((l->user_y)<<(8));/* t256(50) */
		data->s2							= s2;
		#if 1
	//------------ �e���֘A
		data->start_danmaku 				= ((l->user_1_moji+0x10/*�Ƃ肠����r27�݊�*/)&0x1f);
		data->boss_base.danmaku_type		= 0;
		data->boss_base.danmaku_time_out	= 0;
		#endif
	}
}
