
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"���[�~�A", 				"GFAIRY",
		"�d��2"(��),				"GFAIRY",
		"�d��1"(��),(�b��I)		"GFAIRY",
		"�d��3"(��),(�b��I)		"GFAIRY",
	-------------------------------------------------------
	; (72==64[������]+8[���[�~�A�e])
	; (79==64[������]+15[�V��e])		(�������̗d���݊�)�B
---------------------------------------------------------*/

#if 0/*����*/
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define boss_time_out		user_data03 	/* �������� */
#endif

#define YOKAI1_DATA_repeat				user_data04 	/* �J��Ԃ��� */
#define YOKAI1_DATA_start_limit_y256	user_data05 	/* �o��y���W */
#define yokai_type						user_data06 	/* �d���̎��(0 ... 3) */
#define YOKAI1_DATA_start_danmaku		user_data07 	/* �J�n����e���ԍ� */

//
#define YOKAI1_DATA_s2					target_obj		/* SPRITE *s2;*/

enum /* _yokai_type_ */
{
	YOKAI_TYPE_00_RUMIA = 0,	/* 0:00 ���[�~�A */
	YOKAI_TYPE_01_YOUKAI02, 	/* 1:32 "�d��2"(��) */
	YOKAI_TYPE_02_YOUKAI01, 	/* 2:64 "�d��1"(��) */
	YOKAI_TYPE_03_YOUKAI03		/* 3:96 "�d��3"(��) */
};


/*---------------------------------------------------------
	SPRITE *src;	��-�{�X�G����
	SPRITE *tama;	���e
---------------------------------------------------------*/

static void callback_hit_chuu_boss(SPRITE *src, SPRITE *tama)
{
	/* �{�X & ��-�{�X�Ɏ��e�����������ꍇ�̉ΉԃG�t�F�N�g��o�^(����Gu�������Ȃ��̂ŕ`�悵�ĂȂ�) */
	bakuhatsu_add_type_ddd(tama, BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/* ��Ɏ��s������������ */
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
			item_type = (SP_GROUP_ITEMS|(item_tbl[(cg_game_difficulty)]));
		}
		else
		{
			item_type = SP_ITEM_05_TENSU;	/* �_�����o�� */
		}
		item_create(src, item_type, 7, ITEM_MOVE_FLAG_06_RAND_XY);	/* ����΂� */
		player_dummy_add_score(src->base_score);
//
		bakuhatsu_add_circle(src, 0);
	//	src->type						= SP_DELETE;
		/* �|�����ꍇ�w��̖����w�������Ă��邪�A���������ꍇ�w��̖����w������B */
		src->YOKAI1_DATA_s2->jyumyou		= JYUMYOU_NASI; /* �|�����ꍇ�A�w��̖����w�������B */
		src->jyumyou						= (512-1);
		/* �R�[���o�b�N�o�^ */
		src->callback_hit_teki = NULL;	/* �_�~�[�R�[���o�b�N�o�^ */
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_chuu_boss(SPRITE *src)
{
	if (512 > src->jyumyou) 	// SS04:	/* ��֑ޏ� */
	{
		#if 1/* ���݂̎��������́A����ܗǂ��Ȃ��C������B */
		/* �|�����ꍇ�w��̖����w�������Ă��邪�A���������ꍇ�w��̖����w������B */
	//	if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* �`�F�b�N�͎��������ˑ��B */
		if (JYUMYOU_NASI <= src->YOKAI1_DATA_s2->jyumyou)/* �w��̖����w������Έړ��B */
		{
			src->YOKAI1_DATA_s2->cy256	-= t256(1.5);	/*fps_factor*/
		}
		#endif
		src->cy256						-= t256(1.5);	/*fps_factor*/
		if ( 0 > (src->cy256+t256(55.0)))/* 55�h�b�g�Ƃ���(�����_�A�c�̃T�C�Y) */
	//	if ( 0 > src->cy256)/* �ȗ��� */
		{
			src->jyumyou						= JYUMYOU_NASI;
		//	if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* �`�F�b�N�͎��������ˑ��B */
			if (JYUMYOU_NASI <= src->YOKAI1_DATA_s2->jyumyou)/* �w��̖����w������Ώ����B */
			{
				src->YOKAI1_DATA_s2->jyumyou	= JYUMYOU_NASI;
			}
			/* �ޏꏈ�� */
			chu_boss_mode = 0;
			//hold_game_mode_off();
		}
	}
	else
	if (512+100 > src->jyumyou) 	// SS03:	/* ���΂��҂� */
	{
	//	if (0 > src->time_out)
		{
	//		src->time_out = 100/*150*/;
			/* �K��̌J��Ԃ��񐔂��Ȃ�����ޏ� */
			src->YOKAI1_DATA_repeat--;
			if ((0) != src->YOKAI1_DATA_repeat) 	{	src->jyumyou = (512+100+512+100-1);/*SS01;*/		}/* �J��Ԃ� */
			else									{	src->jyumyou = (512-1);/* = SS04*/					}/* ��֑ޏ� */
		}
	}
	else
	if (512+100+512 > src->jyumyou) 	// SS02:	/* �S�e�����I���܂őҋ@ */
	{
	//	danmaku_state_check_holding(src);
		if (SPELL_00 == src->boss_base_spell_type)
		{
			src->jyumyou = (512+100-1);
		}
	}
	else
	if (512+100+512+100 > src->jyumyou) 	// SS01:	/* �e���Z�b�g */			/* 8�����e 7��e */
	{
			src->jyumyou = (512+100+512-1);/* = SS02;*/
			src->boss_base_spell_type	= src->YOKAI1_DATA_start_danmaku;	/* �e�����Z�b�g */		/* "�d��2"&"���[�~�A"��p */
			spell_set_time_out(src);		/* �e���̎��Ԑ؂��ݒ� */
	}
	else
//	if (512+100+512+100+100 > src->jyumyou) // SS00:	/* �ォ��o�� */
	{
		src->YOKAI1_DATA_s2->cy256	+= t256(2.0);	/*fps_factor*/
		src->cy256					+= t256(2.0);	/*fps_factor*/
		if (src->YOKAI1_DATA_start_limit_y256 < src->cy256)
		{
			src->jyumyou = (512+30-1);		/*	src->time_out = 30; SS03;*/
		}
	}

//	if (SP_DELETE != src->type)
	if (JYUMYOU_NASI < src->jyumyou)/* ����� */
	/* �A�j���[�V���� */
	{
		if (YOKAI_TYPE_00_RUMIA == src->yokai_type)
		{
			src->type			= TEKI_00_BOSS11+((src->jyumyou&0x30)>>4);
		}
		else
		{
			src->type			= TEKI_54_CHOU1+((src->jyumyou&0x10)>>4);
		}
	}
//	if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* �`�F�b�N�͎��������ˑ��B */
	if (JYUMYOU_NASI < src->YOKAI1_DATA_s2->jyumyou)/* ����� */
	{
		src->YOKAI1_DATA_s2->rotationCCW1024 += (3);
		mask1024(src->YOKAI1_DATA_s2->rotationCCW1024);
	}
//
	if (512 > src->jyumyou)
	{
		;	/* ��֑ޏ� */
	}
	else
	{
		spell_generator(src); /* �e������ */
	}
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
global int chu_boss_mode;
global void game_command_04_regist_chuu_boss(GAME_COMMAND *l)
{
	if (0==chu_boss_mode)
	{
		chu_boss_mode = 1;
		//hold_game_mode_on();
	/* �v���C�I���e�B�[(�\���D�揇��)�����邩��A�w��ɕ\��������ׂɁA���߂Ɍ��̖����w���m�ہB */
	SPRITE *s2; 		/* ���̖����w */
	s2								= obj_add_01_teki_error();
	if (NULL!=s2/*h*/)/* �o�^�ł����ꍇ�̂� */
	{
		/* ���̖����w���m�ۏo������{�̂��m�ہB */
		SPRITE *s1; 	/* �{�� */
		s1									= obj_add_01_teki_error();
		if (NULL!=s1/*h*/)/* �o�^�ł����ꍇ�̂� */
		{
			/* 0ttd dddd
				'0': force only zero.
				't': yokai type. (0-3)
				'd': danmaku type. (0-31)
			*/
			int my_yokai_type;
		//	my_yokai_type = (((l->user_255_code)>>5)&(4-1));
			my_yokai_type = (((l->user_255_code)   )&(4-1));
		//
			obj_send1->cx256			= ((l->user_x)<<(8));
			obj_send1->cy256			= t256(-30.0);
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
					MAKE32RGBA(0x5f, 0xff, 0xaf, 0x7f), 	/* �����ۂ�(���F�ɂȂ���) */
				};
				s2->color32 				= color_table[my_yokai_type];
			}
			s2->cx256					= (obj_send1->cx256);
			s2->cy256					= (obj_send1->cy256);
		//
			s1->m_Hit256R				= ZAKO_ATARI16_PNG;
			s1->yokai_type				= my_yokai_type;

			if (YOKAI_TYPE_00_RUMIA == (my_yokai_type))/* ���[�~�A */
			{
				s1->type				= TEKI_00_BOSS11;
			}
			else	/* �d��2 */
			{
				s1->type				= TEKI_54_CHOU1;
			}
			s1->flags					|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			s1->callback_mover			= move_chuu_boss;
		//	s1->callback_loser			= lose_youkai1;
			s1->callback_hit_teki		= callback_hit_chuu_boss;	/* �R�[���o�b�N�o�^ */
			s1->cx256					= (obj_send1->cx256);
			s1->cy256					= (obj_send1->cy256);
		//
			/* easy�ł����݊�����ۂÂ���ׂ� 200 �͕K�v */ 	// 50+150*di fficulty;
			s1->base_hp 				= ((l->user_hp));		/* �ݒ�t�@�C������̗͂����߂�B */
			s1->base_score				= ((l->user_score));	/* �ݒ�t�@�C������l���X�R�A�����߂�B */
		//
			s1->YOKAI1_DATA_repeat				= (2+2+1);
			s1->YOKAI1_DATA_start_limit_y256	= ((l->user_y)<<(8));/* t256(50.0) */
			s1->YOKAI1_DATA_s2					= s2;
			{
				#if 1
			//------------ �e���֘A
				s1->boss_base_spell_type			= 0;
			//	s1->YOKAI1_DATA_start_danmaku		= ((((l->user_255_code)&0x1f)+0x10/*�Ƃ肠����r27�݊�*/)&0x1f);
			//	s1->YOKAI1_DATA_start_danmaku		= ((l->user_255_code)&0x1f);
				s1->YOKAI1_DATA_start_danmaku		= ((l->user_kougeki_type)&0x3f);
				#endif
			}
		}
		else
		{
			/* �{��(s1)���o�^�ł��Ȃ������ꍇ�A�w��̖����w(s2)�̓o�^��������߂�B */
			s2->jyumyou 			= JYUMYOU_NASI;/* �o�^�ł��Ȃ������ꍇ */
		}
	}
	}
}
		//	s1->base_hp 				= 1856;//(8*200)+(2/*di fficulty*/<<(4+3)); 	/* ���[�~�A */
		//	s1->base_hp 				= (8*160)+(1/*di fficulty*/<<(4+3-1));	/* �d��2 */
		//	s1->base_score				= score(100)+score(100)*(cg_game_difficulty);

/*
 (46-24)/2 == 11
*/

/*
 64=1*16+48
 80=2*16+48
112=4*16+48
160=7*16+48
*/
