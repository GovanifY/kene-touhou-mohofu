
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"���[�~�A", 				"GFAIRY",
		"�d��2"(��),				"GFAIRY",
		"�d��1"(��),(�b��I)		"GFAIRY",
	-------------------------------------------------------
	�������̗d���A����͔p�~���� "�d��2" �ɋz���\��B
---------------------------------------------------------*/

#if 0/*����*/
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define time_out			user_data03 	/* �������� */
#endif

#ifndef time_out
	#define time_out	user_data03
#endif
//#define speed256	user_data03
//#define state 	user_data04

//typedef struct
//{
//	ENEMY_BASE base;
//	BO SS_BASE boss_base;
//------------ �ړ��֘A
//	int st ate1;
//	int time_out;			/* �s��wait */
#define YOKAI1_DATA_repeat				user_data04 	/* �J��Ԃ��� */
#define YOKAI1_DATA_start_limit_y256	user_data05 	/* �o��y���W */
#define yokai_type						user_data06 	/* */
#define YOKAI1_DATA_start_danmaku		user_data07 	/* ???() */

//
#define YOKAI1_DATA_s2					target_obj		/* SPRITE *s2;*/
//} YOKAI1_DATA;
//	int wait2;	/* �U��wait */

enum /* _yokai_type_ */
{
	YOKAI_TYPE_00_RUMIA = 0,	/* 0 ���[�~�A */
	YOKAI_TYPE_01_YOUKAI02, 	/* 1 "�d��2"(��) */
	YOKAI_TYPE_02_YOUKAI01, 	/* 2 "�d��1"(��)���������ۂ��� */
};

enum
{
	SS00 = 0,
	SS01,
	SS02,
	SS03,
	SS04,
};


/*---------------------------------------------------------
	s = �{�X�{�� boss sprite
	t = �v���C���[�̒e player's weapon
---------------------------------------------------------*/

static void callback_hit_chuu_boss(SPRITE *src/*�G����*/, SPRITE *tama /*���e*/)
{
	bakuhatsu_add_type_ddd(tama, BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);
//
	src->base_hp -= tama->base_weapon_strength; 	/* �U�����đ̗͌��炷(����������) */
	if (0 >= src->base_hp)			/* �O�����l�Ȃ�A�|�����B */
	{
		#if 1/*�v��́H*/
		src->base_hp	= (9999);/* �Q��͓|���Ȃ� */
		#endif
		bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
	//
		int item_type;
		if (YOKAI_TYPE_00_RUMIA == src->yokai_type)
		{
			const u8 item_tbl[4] =
			{
				(SP_ITEM_02_P128&0xff), 	/* [F] */
				(SP_ITEM_01_P008&0xff), 	/* [P]�� */
				(SP_ITEM_00_P001&0xff), 	/* [p]�� */
				(SP_ITEM_05_TENSU&0xff),	/* [�_] */
			};
			item_type = (SP_GROUP_ITEMS|(item_tbl[difficulty]));
		}
		else
		{
			item_type = SP_ITEM_05_TENSU;	/*�_�����o��*/
		}
		item_create(src, item_type, 7, ITEM_MOVE_FLAG_06_RAND_XY);	/* ����΂� */
		player_add_score( src->base_score);
//
		bakuhatsu_add_circle(src, 0);
	//	src->type						= SP_DELETE;
		/* �|�����ꍇ�w��̖����w�������Ă��邪�A���������ꍇ�w��̖����w������B */
		src->YOKAI1_DATA_s2->type		= SP_DELETE;	/* �|�����ꍇ�A�w��̖����w�������B */
		src->boss_base_state001 		= SS04; 		/* ��֑ޏ� */
		/* �R�[���o�b�N�o�^ */
		src->callback_hit_enemy = NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

#if 0
	;/* 8�����e */
	data->wait2--;
	if (0 > data->wait2)
	{
		/* 0: 20 == 20-(0*4) */
		/* 1: 16 == 20-(1*4) */
		/* 2: 12 == 20-(2*4) */
		/* 3:  8 == 20-(3*4) */
		data->wait2 = ((20-1)-((difficulty)<<2));/*(8-1)*/ /*(10-1)*/
		voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*�e�L�g�[*/
		obj_send1->x256 					= src->x256;
		obj_send1->y256 					= src->y256;
		//
		br.BULLET_REGIST_speed256			= (t256(3.0)+((difficulty)<<6));
		br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_03_MIDORI;
		br.BULLET_REGIST_n_way				= (8);
		//
		br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
		br.BULLET_REGIST_div_angle1024		= (int)(1024/24);
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_vector();
	}
#endif

#if 0
	;/* 7��e */
	data->wait2--;
	if (0 > data->wait2)
	{
		data->wait2 = ((20-1)-((difficulty)<<2));/*(8-1)*/ /*(10-1)*/
		voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*�e�L�g�[*/
		obj_send1->x256 					= src->x256;
		obj_send1->y256 					= src->y256;
		//
		br.BULLET_REGIST_speed256			= t256(3.25)+((difficulty)<<6);
		br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_04_MIZUIRO;
		br.BULLET_REGIST_n_way				= (7);
		//
		br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
		br.BULLET_REGIST_div_angle1024		= (int)(1024/24);
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_vector();
	}
#endif

static void move_chuu_boss(SPRITE *src)
{
	src->time_out--;
	switch (src->boss_base_state001)
	{
	case SS00:	/* �ォ��o�� */
		src->YOKAI1_DATA_s2->y256	+= t256(2.0);	/*fps_factor*/
		src->y256					+= t256(2.0);	/*fps_factor*/
		if (src->YOKAI1_DATA_start_limit_y256 < src->y256)	{	src->time_out = 30; src->boss_base_state001 = SS03; }
		break;
//---------
	case SS01:	/* �e���Z�b�g */
			/*src->YOKAI1_DATA_wait2 = (8-1) (10-1)*/
			src->boss_base_state001++;/* = SS02;*/
			;/* 8�����e */
			;/* 7��e */
			src->boss_base_danmaku_type 	= src->YOKAI1_DATA_start_danmaku;	/*DANMAKU_01*/	/* �S�e�����Z�b�g */		/* "�d��2"&"���[�~�A"��p */
			danmaku_set_time_out(src);		/* �e���̎��Ԑ؂��ݒ� */
		break;
	case SS02:	/* �S�e�����I���܂őҋ@ */
		danmaku_state_check_holding(src);	/*, SA07*/ /*, SAKUYA_ANIME_04_CENTER_A*/
		break;
	case SS03:	/* ���΂��҂� */
		if (0 > src->time_out)
		{
			src->time_out = 100/*150*/;
			/* �K��̌J��Ԃ��񐔂��Ȃ�����ޏ� */
			src->YOKAI1_DATA_repeat--;
			if ((0) != src->YOKAI1_DATA_repeat) 	{	src->boss_base_state001 	= SS01; 		}/* �J��Ԃ� */
			else									{	src->boss_base_state001++;/* = SS04*/		}/* ��֑ޏ� */
		}
		break;
//---------
	case SS04:	/* ��֑ޏ� */
		/* �|�����ꍇ�w��̖����w�������Ă��邪�A���������ꍇ�w��̖����w������B */
		if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* �`�F�b�N�͎��������ˑ��B */
		{
			src->YOKAI1_DATA_s2->y256	-= t256(1.5);	/*fps_factor*/
		}
		src->y256						-= t256(1.5);	/*fps_factor*/
		#if 0/*SDL(�������W)*/
		if ( -((src->h128+src->h128)) > src->y256)
		#endif
		#if 0/*Gu(���S���W)*/
		if ( 0 > src->y256)
		#endif
		#if 0/*���݁A�����_�Ȃ̂ŁA�{���͂���*/
		if ( 0 > (src->y256+((src->height_size256/*�c�̃T�C�Y*/))))
		#endif
		#if 0/*Gu(���S���W��_�ɂȂ�����A�{���͂����B*/
		if ( 0 > (src->y256+((src->height_harf_size256/*�c�̔����T�C�Y*/))))
		#endif
		if ( 0 > (src->y256+t256(55.0)))/* 55�h�b�g�Ƃ���(�����_�A�c�̃T�C�Y) */
	//	if ( 0 > src->y256)/* �ȗ��� */
		{
			src->type = SP_DELETE;
			if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* �`�F�b�N�͎��������ˑ��B */
			{
				src->YOKAI1_DATA_s2->type = SP_DELETE;
			}
		}
		break;
	}
	if (SP_DELETE != src->type)
	{
		if (YOKAI_TYPE_00_RUMIA == src->yokai_type)
		{
			src->type			= TEKI_00_BOSS11+((src->time_out&0x30)>>4);
		}
		else
		{
			src->type			= TEKI_54_CHOU1+((src->time_out&0x10)>>4);
		}
	}
//
	if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* �`�F�b�N�͎��������ˑ��B */
	{
		src->YOKAI1_DATA_s2->m_angleCCW1024 += 3;
		mask1024(src->YOKAI1_DATA_s2->m_angleCCW1024);
	}
//
	danmaku_generator(src); /* �e������ */
}

static void move_youkai1(SPRITE *src)
{
	src->time_out--;
	switch (src->boss_base_state001)
	{
	case SS00:	/* �ォ��o�� */
		src->YOKAI1_DATA_s2->y256	+= t256(2.0);	/*fps_factor*/
		src->y256					+= t256(2.0);	/*fps_factor*/
		if (src->YOKAI1_DATA_start_limit_y256 < src->y256)	{	src->time_out=30;	src->boss_base_state001 = SS03; }
		break;
//---------
	case SS01:
		if (0 > src->time_out)	{	src->time_out = 30; src->boss_base_state001=SS03;	/*src->YOKAI1_DATA_wait2=(8-1) (10-1);*/	}
		else
		{
		//	static int YOKAI1_DATA_wait2	= (10-1);/*0*/
			static int YOKAI1_DATA_wait2	= 0;		/* �U��wait */
			YOKAI1_DATA_wait2--;
			if (0 > YOKAI1_DATA_wait2)
			{
				/* 0: 20 == 20-(0*4) */
				/* 1: 16 == 20-(1*4) */
				/* 2: 12 == 20-(2*4) */
				/* 3:  8 == 20-(3*4) */
				YOKAI1_DATA_wait2 = ((20-1)-((difficulty)<<2));/*(8-1)*/ /*(10-1)*/
				voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*�e�L�g�[*/
				obj_send1->x256 = src->x256;
				obj_send1->y256 = src->y256;
				#if (0) // 1975405
				if (src->YOKAI1_DATA_repeat&1)
				{
					br.BULLET_REGIST_speed256			= (t256(3.25)+((difficulty)<<6));
					br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_04_MIZUIRO;
					br.BULLET_REGIST_n_way				= (7);
				}
				else
				{
					br.BULLET_REGIST_speed256			= (t256(3.0)+((difficulty)<<6));
					br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_03_MIDORI;
					br.BULLET_REGIST_n_way				= (8);
				}
				#else // 1975357
				{	const int aaa = (src->YOKAI1_DATA_repeat&1);
					br.BULLET_REGIST_speed256			= (t256(3.0)+((difficulty)<<6))+(aaa<<(8-2));
					br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_03_MIDORI+(aaa);
					br.BULLET_REGIST_n_way				= (7)+(aaa);
				}
				#endif
				br.BULLET_REGIST_angle1024				= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_div_angle1024			= (int)(1024/24);
				br.BULLET_REGIST_regist_type			= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
		break;
	case SS02:	/* �S�e�����I���܂őҋ@ */
		src->boss_base_state001++;
		break;
	case SS03:	/* ���΂��҂� */
		if (0 > src->time_out)
		{
			src->time_out = 100/*150*/;
			/* �K��̌J��Ԃ��񐔂��Ȃ�����ޏ� */
			src->YOKAI1_DATA_repeat--;
			if ((0) != src->YOKAI1_DATA_repeat) 	{	src->boss_base_state001 = SS01; 			}/* �J��Ԃ� */
			else									{	src->boss_base_state001++;/* = SS04*/		}/* ��֑ޏ� */
		}
		break;
//---------
	case SS04:	/* ��֑ޏ� */
		/* �|�����ꍇ�w��̖����w�������Ă��邪�A���������ꍇ�w��̖����w������B */
		if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* �`�F�b�N�͎��������ˑ��B */
		{
			src->YOKAI1_DATA_s2->y256	-= t256(1.5);	/*fps_factor*/
		}
		src->y256						-= t256(1.5);	/*fps_factor*/
		#if 0/*SDL(�������W)*/
		if ( -((src->h128+src->h128)) > src->y256)
		#else/*Gu(���S���W)*/
		if ( 0 > src->y256)
		#endif
		{
			src->type					= SP_DELETE;
			if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* �`�F�b�N�͎��������ˑ��B */
			{
				src->YOKAI1_DATA_s2->type	= SP_DELETE;
			}
		}
		break;
	}
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_54_CHOU1+((src->time_out&0x10)>>4);
	}
//
	if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* �`�F�b�N�͎��������ˑ��B */
	{
		src->YOKAI1_DATA_s2->m_angleCCW1024 += 3;
		mask1024(src->YOKAI1_DATA_s2->m_angleCCW1024);
	}
}



/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

/*
 (46-24)/2 == 11
*/
	//	s1->x256				= ((l->user_x)*t256(35.0))+t256(40.0);
	//	s1->x256				= ((l->user_x)*t256(32.0))+t256(48.0);
	//	s1->x256				= ((l->user_x)<<(8+5))+t256(48.0);


/*

 64=1*16+48
 80=2*16+48
112=4*16+48
160=7*16+48

*/

global void add_chuu_boss(STAGE_DATA *l)
{
	SPRITE *s1;
	s1									= sprite_add_gu_error();
	if (NULL!=s1/*h*/)/* �o�^�ł����ꍇ�̂� */
	{
		SPRITE *s2; 		/* ���̖����w */
		s2								= sprite_add_gu_error();
		if (NULL!=s2/*h*/)/* �o�^�ł����ꍇ�̂� */
		{
			/* 0ttd dddd
				'0': force only zero.
				't': yokai type. (0-3)
				'd': danmaku type. (0-31)
			*/
			int my_yokai_type;
			my_yokai_type = (((l->user_255_code)>>5)&(4-1));
		//
			obj_send1->x256 			= ((l->user_x)<<(8));
			obj_send1->y256 			= t256(-30.0);
			/* �����w���� */
			s2->m_Hit256R				= ZAKO_ATARI02_PNG;
			s2->type					= TEKI_52_MAHOJIN;/*S P_M UTEKI*/
			s2->flags					|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			{
				const u32 color_table[4] =
				{	/*	s2->alpha			= 0x00;*/
					MAKE32RGBA(0xff, 0xff, 0xff, 0x7f), 	/* �D���ۂ� */
					MAKE32RGBA(0xff, 0x3f, 0x3f, 0x7f), 	/* �Ԃ��ۂ� */
					MAKE32RGBA(0x3f, 0x00, 0xff, 0x7f), 	/* ���ۂ� */
					MAKE32RGBA(0x5f, 0xff, 0xaf, 0x7f), 	/* �����ۂ� */
				};
				s2->color32 				= color_table[my_yokai_type];
			}
			s2->x256					= obj_send1->x256;/*+((obj_send1->w128-s2->w128))*/ /*-t256((8.0))*/
			s2->y256					= obj_send1->y256;/*+((obj_send1->h128-s2->h128))*/ /*-t256((8.0))*/
		//
			s1->m_Hit256R				= ZAKO_ATARI16_PNG;
			s1->yokai_type				= my_yokai_type;
			if (YOKAI_TYPE_00_RUMIA == (my_yokai_type))/*���[�~�A*/
			{
				s1->type				= TEKI_00_BOSS11;
				s2->x256				-= t256(10);/* 11.50 == (46-23)/2 */
				s2->y256				-= t256( 4);/*	5.50 == (46-35)/2 */
			}
			else	/*�d��2*/
			{
				s1->type				= TEKI_54_CHOU1;
			}
			s1->flags					|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			if (YOKAI_TYPE_02_YOUKAI01 == (my_yokai_type))/*������*/
			{
				s1->callback_mover			= move_youkai1;/*������*/
			}
			else
			{
				s1->callback_mover			= move_chuu_boss;
			}
		//	s1->callback_loser			= lose_youkai1;
			s1->callback_hit_enemy		= callback_hit_chuu_boss;	/* �R�[���o�b�N�o�^ */
			s1->x256					= obj_send1->x256;
			s1->y256					= obj_send1->y256;
		//
			s1->base_hp 				= (8*200)+(2/*di fficulty*/<<(4+3));	/*���[�~�A*/		/* easy�ł����݊�����ۂÂ���ׂ� 200 �͕K�v */ 	// 50+150*di fficulty;
		//	s1->base_hp 				= (8*160)+(1/*di fficulty*/<<(4+3-1));	/*�d��2*/			/* easy�ł����݊�����ۂÂ���ׂ� 200 �͕K�v */ 	// 50+150*di fficulty;
			s1->base_score				= score(100)+score(100)*difficulty;
		//
			s1->boss_base_state001				= SS00;
			s1->YOKAI1_DATA_repeat				= (2+2+1);
			s1->YOKAI1_DATA_start_limit_y256	= ((l->user_y)<<(8));/* t256(50.0) */
			s1->YOKAI1_DATA_s2					= s2;
		//	s1->YOKAI1_DATA_wait2				= (10-1);/*0*/
		//	s1->time_out						= 30;
			/* "�d��2"&"���[�~�A"��p */
			{
				#if 1
			//------------ �e���֘A
				s1->boss_base_danmaku_type			= 0;
			//	s1->YOKAI1_DATA_start_danmaku		= ((((l->user_255_code)&0x1f)+0x10/*�Ƃ肠����r27�݊�*/)&0x1f); 	/* "�d��2"&"���[�~�A"��p */
				s1->YOKAI1_DATA_start_danmaku		= ((l->user_255_code)&0x1f);	/* "�d��2"&"���[�~�A"��p */
				#endif
			}
		}
		else
		{
			s1->type				= SP_DELETE;/* �o�^�ł��Ȃ������ꍇ */
		}
	}
}
