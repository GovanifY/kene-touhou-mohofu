
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�e�v���C���[(�얲 �� ������ �� ���~���A �� �`���m �� �H�X�q)
	REIMU(A/B) MARISA(A/B/C) REMILIA CIRNO YUYUKO
	-------------------------------------------------------
---------------------------------------------------------*/

#include "jiki_local.h"

#include "kanji_system.h"

/*---------------------------------------------------------

---------------------------------------------------------*/

global PLAYER_SPEC pd;

/*---------------------------------------------------------
	���ʃI�u�W�F�N�g
---------------------------------------------------------*/
#if 1

//global SPRITE *obj00[FIX_OBJ_00_PLAYER];
//static SPRITE *obj_maru;		/* �� */
//static SPRITE *option[4];

global SPRITE *global_obj_boss; //obj00[FIX_OBJ_08_BOSS]

global SPRITE *obj_send1;
//global SPRITE *obj_effect[8]; /* [��]�i�X�y���J�[�h�̃O���t�B�b�N�j */

//#define TEISOKU_EFFECT_00_OBJ (6)
//#define TEISOKU_EFFECT_01_OBJ (7)

#endif


	/*---------------------------------------------------------
		�e�v���C���[�̐��\��
	---------------------------------------------------------*/
/* �얲 ����\�́F��炢�{���̎�t���Ԃ����� */
/* �`���m ����\�́F�H */

/*static*/global const u8 player_fix_status[BASE_MAX] =
{/* REIMU(A/B) MARISA(A/B)	REMILIA YUYUKO CIRNO(A/Q) */
//	/* BASE_OPT_SHOT_INTERVAL �I�v�V�����V���b�g�̍X�V�Ԋu / option shot interval. */
//	  14,  13,	11,   8,	8,	31,   8,   9,	 /* WEAPON_L0 */
//	  12,  11,	10,   8,	8,	28,   8,   9,	 /* WEAPON_L1 */
//	  10,	9,	 9,   8,	8,	25,   8,   9,	 /* WEAPON_L2 */
//	   8,	7,	 8,   8,	8,	22,   8,   9,	 /* WEAPON_L3 */
//	   6,	5,	 7,   8,	8,	19,   8,   9,	 /* WEAPON_L4 */
//	   4,	3,	 6,   8,	8,	16,   8,   9,	 /* WEAPON_L5 */
//
	  14,  13,	11,   4,	8,	31,   8,   9,	 /* WEAPON_L0 */
	  12,  11,	10,   4,	8,	28,   8,   9,	 /* WEAPON_L1 */
	  10,	9,	 9,   4,	8,	25,   8,   9,	 /* WEAPON_L2 */
	   8,	7,	 8,   4,	8,	22,   8,   9,	 /* WEAPON_L3 */
	   6,	5,	 7,   4,	8,	19,   7,   8,	 /* WEAPON_L4 */
	   4,	3,	 6,   3,	8,	16,   5,   6,	 /* WEAPON_L5 */
//	 /* BASE_SPEED_ANIME	   �v���C���[�̃A�j���[�V�������x */
	   8,	8,	 2,   2,	3,	 4,   1,   1,	 /* BASE_SPEED_ANIME	   �v���C���[�̃A�j���[�V�������x */
//	  16,  16,	 8,   8,	4,	12,   9,   9,	 /* BASE_HIT_BOMB_DEC_WAIT	   ��炢�{���̎�t���� / hit_bomb_wait. */
//	  16-16,  16-16,	 16-8,	 16-8,	16-4,	16-12,	 16-9,	 16-9,	 /* BASE_HIT_BOMB_DEC_WAIT ��炢�{���̎󂯕t���P�\�������� / hit_bomb_wait. */
//	  0,  0,	 8,   8,	12,  4,   7,   7,	 /* BASE_HIT_BOMB_DEC_WAIT ��炢�{���̎󂯕t���P�\�������� / hit_bomb_wait.(�l�����������A�󂯕t�����Ԃ�����) */
	  0,  0,	 8,   8,	12,  4,   3,   2,	 /* BASE_HIT_BOMB_DEC_WAIT ��炢�{���̎󂯕t���P�\�������� / hit_bomb_wait.(�l�����������A�󂯕t�����Ԃ�����) */
//	  40,  40,	40,  40,   24,	48,   8,   8,	 /* BASE_STD_BOMB_STRENGTH �ʏ�{���̋��� / standard bomb strength. */
//	 192, 192, 255, 255,   96, 160,  96,  96,	 /* BASE_LOW_BOMB_STRENGTH �ᑬ�{���̋��� / lower bomb strength. */
	#if 0
	  48,  40,	40,  40,   32,	48,   8,   9,	 /* BASE_STD_BOMB_STRENGTH �ʏ�{���̋��� / standard bomb strength. */
	 160, 192, 240, 255,  127, 160,  96, 127,	 /* BASE_LOW_BOMB_STRENGTH �ᑬ�{���̋��� / lower bomb strength. */
	#else
	  48,  40,	40,  40,   32,	48, 240, 255,	 /* BASE_STD_BOMB_STRENGTH �ʏ�{���̋��� / standard bomb strength. */
	 160, 192, 240, 255,  127, 160, 240, 255,	 /* BASE_LOW_BOMB_STRENGTH �ᑬ�{���̋��� / lower bomb strength. */
//	 (255), 192, 240, 255,	127, 160,  (244),  96,	 /* BASE_LOW_BOMB_STRENGTH �ᑬ�{���̋��� / lower bomb strength. */
	#endif
//
	   0,	0,	 0,   0,	0,	 1,   1,   1,	 /* BASE_OPT_ANIM_TYPE	   0==��], 1==�p�^�[�� */
	   2,	2,	 2,   2,	2,	12,   3,   3,	 /* BASE_OPT_ANIM_SPEED    ��]/�p�^�[�����x */
};
//	  40,	 40,	5*8,	 40,	 24,	6*8,	 1*8,	1*8,	/* BASE_STD_BOMB_STRENGTH �ʏ�{���̋��� / standard bomb strength. */
//	 30*8,	30*8,  48*8,	48*8,	12*8,  24*8,	12*8,  12*8,	/* BASE_LOW_BOMB_STRENGTH �ᑬ�{���̋��� / lower bomb strength. */

/*
���~���A�{��: �ʏ�{���̋���:[��==8] [�V==4]
�� 3��==((256/64)-1) [24==(8)*3]
�V 7��==((256/32)-1) [21==(3)*7]
REIMU		5,[�V==5] [��==3]
MARISA		5,
REMILIA 	3,[�V==3] [��==8]
YUYUKO		6,[�V==6] [��==4]
CIRNO		1,[�V==1] [��==4]
	�ʏ�{���̋��� / standard bomb strength.
*/





/*---------------------------------------------------------
	�v���C���[����̂����蔻��
---------------------------------------------------------*/

/*static*/global void player_weapon_colision_check(SPRITE *shot, int erase_shot_type)
{
//	SPRITE *shot;	/* ���e */
	SPRITE *tt; 	/* �ꎞ�g�p�̃e���|����(�G�X�v���C�g�A�܂��́A�G�e�X�v���C�g) */
	#define teki_obj		tt
	#define tekidan_obj 	tt
	/* �G�e�ɂ��������ꍇ�ɓG�e������(�{���n�̂�) */
	if (/*PLAYER_WEAPON_TYPE_01_BOMB==*/erase_shot_type/*erase_bullets*/)	/* �{���n�̂� */
	{
		/* ���e�ɂ��������͓̂G�e�Ȃ̂����ׂ�D */
		tekidan_obj = sprite_collision_check_444(shot, SP_GROUP_BULLETS);/* �e����p(�e��) */
		if (NULL != tekidan_obj)		/* �G�e�ɓ��������� */
		{
			tekidan_obj->jyumyou = JYUMYOU_NASI;	/* �G�e������ */
			voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
		}
	}
	/* �G�ɂ��������ꍇ�ɓG������ */
	{
		/* ���e�ɂ��������͓̂G���̂Ȃ̂����ׂ�D */
		teki_obj = sprite_collision_check_SDL_teki(shot);//, (SP_GROUP_TEKI));	/*|SP_GROUP_BOSS*/	/*SP_GROUP_ENEMYS*/
		if (NULL != teki_obj)			/* �G���̂ɓ��������� */
		{
			if (NULL != (teki_obj->callback_hit_enemy)) 	/*	*/
			{
				(teki_obj->callback_hit_enemy)(teki_obj/*�G����*/, shot/*���e*/);
			}
			/* ���e���G�ɂ��������ꍇ�Ɏ��e�����ł��� */
			if (/*erase_player_tama*/PLAYER_WEAPON_TYPE_00_SHOT == erase_shot_type) 	/* �V���b�g�n�̂� */
			{
			//	/* ���e���ŕ��@������ȏꍇ */
			//	if (NULL != (shot->callback_loser))
			//	{
			//		(shot->callback_loser)(shot/*���e*/);
			//	}
				shot->jyumyou = JYUMYOU_NASI;		/* ���e������ */
				/* �V���b�g���G�ɓ��������ꍇ�A�ł����ݓ_�����Z�B(���G�L�����ł��������ݓ_�ŉ҂���) */
				player_dummy_add_score(score(10));
			}
		}
	}
}


/*---------------------------------------------------------
	�G��T���q�֐�
---------------------------------------------------------*/

/*static*/global SPRITE *search_enemy_by_sprite(void)
{
	int ii;
	for (ii=0; ii<SPRITE_333POOL_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *h;
		h = &obj33[ii];
	//
		if (
			#if 1
		//	(SP_DELETE != h->type ) && /* �폜�ς݂͔�΂� */
			(JYUMYOU_NASI <= h->jyumyou) && /* �폜�ς݂͔�΂� */
			#endif
			(0 != (h->type & (SP_GROUP_TEKI/*|SP_GROUP_BOSS*/))/*SP_GROUP_ENEMYS*/) 	/* �v���C���[�ɂƂ��Ă̓G(�U�R��{�X) */
		)
		{
//			if (
#if 0
//				(��) && 						/* ����t���O�������G(�ʂ̗U���e�������Ă��Ȃ��G) */
#endif
//				(h->flags & SP_FLAG_VISIBLE)										/* �g�p���̓G */
//			)
			{
				#if 1/*Gu(���S���W)*/
				if ((h->cx256 > 0) && (h->cx256 < t256(GAME_WIDTH ) ) &&
					(h->cy256 > 0) && (h->cy256 < t256(GAME_HEIGHT) ))	/* �\����ʓ��̓G */
				#endif
				{
					return (h);/*��������*/
				}
			}
		}
	}
	return (&obj00[FIX_OBJ_00_PLAYER]);/*������Ȃ�����*/
}


/*---------------------------------------------------------
	�X�y�J�������̗����G
---------------------------------------------------------*/

static void player_move_tachie(SPRITE *src) 	/* [***100113 �ǉ� */
{
	if ((128-1) < pd.bomber_time)/* �c�莞�Ԃ� 128 �ȏ�Ȃ�A�ړ����đҋ@ */
	{
		/* ��ʊO����o���A��~�B���΂��҂B */
		if ( t256(100) > src->cx256 )
		{
			src->cx256 += t256(5.0);
		}
	}
	else
	if ((1/*-1*/) < pd.bomber_time)/* �g�� */
	{
		/* (24+(1))  �A�����ł� pd.bomber_time �́A�ő�0x7f�Ȃ̂ŁB */
		src->color32		= (pd.bomber_time<<(24+(1)))|0x00ffffff;
		#if (1==USE_ZOOM_XY)
		src->m_zoom_x256	+= (10);
		src->m_zoom_y256	+= (10);
		#else
		src->m_zoom_xy256	+= (10);
		#endif
	}
	else
	{
		src->jyumyou = JYUMYOU_NASI;	/* ���Ԑ����ł����܂� */
	}
}


/* ====================================================================================== */
/* ====================================================================================== */
/* ====================================================================================== */

/*---------------------------------------------------------
	�v���C���[�e(�V���b�g/�j���p)�����A�q�֐�
	�v���C���[�A�j�e(�얲)�̒ǉ�	 [***090220 �ǉ�
		(�얲)		[�j�e]�A
		(������)	[�X�e]�A
		(���~���A)	[�R�E�����e]�A
		(�H�X�q)	[���e] �A
		(���`���m)	[�ەX�e]�A
---------------------------------------------------------*/

/*static*/ int weapon_level_offset; /* �����������탌�x��(�ԍ�)�̂W�{�Ƀv���C���[�l�𑫂����l���I�t�Z�b�g�Ƃ��ĕێ� */
/*
	weapon_level_offset == player�ԍ� + (���탌�x�� x 8)
	weapon_level_offset =  select_player + (weapon_level<<3);
*/


/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�(�얲�A�������A���H�X�q)
---------------------------------------------------------*/

//#define BASE_OPT_SHOT_ANIME	(PLAYERS8*3)
//	 12,  12,	6,	 6,    3,  yuuyko(12),	 CIRNO(3),	 CIRNO(3),	/* �I�v�V�����V���b�g�̃A�j���[�V�������x */

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̒�`
---------------------------------------------------------*/
/*static*/extern void player_move_option(SPRITE *src);

/*---new add-----*/


/*---------------------------------------------------------
	�v���C���[�����G����
	(�H�X�q)[�H�X�q�{���̐�{��]
---------------------------------------------------------*/

static void player_create_bomber_tachie(void/*SPRITE *src*/)
{
	SPRITE *h;
	h					= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/*(?)*/
	{
		h->m_Hit256R			= JIKI_ATARI_ITEM_80;
//		h->flags				|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
		h->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);
		h->callback_mover		= player_move_tachie;
		h->type 				= (JIKI_BOMBER_00|SP_GROUP_SHOT_ZAKO);	/*SP_GROUP_JIKI_GET_ITEM*/ /*JI KI_SHOT_00*/	/* �{�X�̒��ڍU���͋֎~ */	/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ���*/
		/* �����G�����ʒu */
		h->cx256				= -t256(150);	// (200)	/*0*/  /*(src->cx256) */
		h->cy256				=  t256(150);	// (100)	/*GAME_HEIGHT+1*/ /*(src->cy256)*/
	//
		/* �`��p�p�x */
	//	h->rotationCCW1024		= (256);
		h->rotationCCW1024		= (0);
	//
	/*???*/ 	h->base_weapon_strength 	= (1/*8*1*/);		/* ��{�� �̋��� */
	}
}






enum /*_player_state_*/
{
	//...
	LIMIT_m255_CONTINUE 		= (-255),
	LIMIT_m127_MIN				= (-127),
	LIMIT_m16_GAME_OUT			= (-16),
	LIMIT_m1_KURAI_BOMB_UKETUKE = (-1),
	LIMIT_0_ZERO	= (0),
	LIMIT_1_ONE 	= (1),
	LIMIT_128		= (128),
	LIMIT_256		= (256),
	LIMIT_512		= (512),
	LIMIT_65535 	= (65535),
};

/* 0==����C�x���g�������Ă��Ȃ��B */
static signed int pds_status_timer; /* ����C�x���g�������^�C�}�[ */
//static int pd.special_ivent_type; /* ����C�x���g�̎�� */


extern void reimu_create_bomber_kekkai_parrent(SPRITE *src);				/* �얲��p �{���̐e */
extern void marisa_create_bomber_master_spark_parrent(SPRITE *src); 		/* ������ B��p �{���̐e */
extern void yuyuko_create_bomber_gyastry_dream_parrent(SPRITE *src);
extern void marisa_create_bomber_star_dust_levarie_parrent(SPRITE *src);	/* ������ A��p �ᑬ�{���̐e */
extern void remilia_create_bomber_cross_shot_parrent(SPRITE *src);			/* ���~���A�{���̐e */
extern void yuyuko_create_bomber_meifu_parrent(SPRITE *src);				/* �H�X�q��p �ᑬ�{���̐e */
extern void marisa_create_bomber_homing_parrent(SPRITE *src);				/* �ᑬ�{���̐e */
extern void reimu_create_bomber_homing_shot_parrent(SPRITE *src);			/* �얲��p �ᑬ�{���̐e */
/*---------------------------------------------------------
	�v���C���[�L�[����
---------------------------------------------------------*/

//static int /*bomb_wait*/d->bomber_time;		/* �{���̗L������ */	//���̃{�����o����܂ł̎���

extern /*global*/short my_analog_x; /* �A�i���O�ʁA�␳�ς� */
extern /*global*/short my_analog_y; /* �A�i���O�ʁA�␳�ς� */
extern void set_bg_alpha(int set_bg_alpha);
extern void register_main_shot(SPRITE *s1);


//enum /*_player_state_*/
//{
//	PLAYER_STATE_00_NORMAL,
//	PLAYER_STATE_01_KURAI_BOMB_UKETUKE, 	/* [***090125		�ǉ�:PLAYER_STATE_01_KURAI_BOMB_UKETUKE */
//	PLAYER_STATE_02_SAVE_01,
//	PLAYER_STATE_03_SAVE_02,
//	PLAYER_STATE_04_GAME_OUT,
//};

static void player_keycontrol(SPRITE *s1)
{
	/*---------------------------------------------------------
		�v���C���[�{���֘A����
	---------------------------------------------------------*/
//	if (STATE_FLAG_06_IS_SCRIPT==(pd.state_flag & STATE_FLAG_06_IS_SCRIPT))
//	{
//		;	/* ��b�C�x���g���̓{���֘A�̏����͂��Ȃ� */
//	}
//	else
		/* �V�i���I���ɂ͗U���e��ǉ����Ȃ� */
	if (0==(pd.state_flag & STATE_FLAG_06_IS_SCRIPT))
	{
	//	if (d->bomber_time>0)
		if (0 < /*bomb_wait*/pd.bomber_time)		/* �{���E�F�C�g���� */
		{
			//	d->bomber_time-=fps_fa_ctor;
			/*bomb_wait*/pd.bomber_time--/*-=fps_fa_ctor*/;/**/
			if (2 > /*bomb_wait*/pd.bomber_time)
			{
				set_bg_alpha(255);/* ��ʂ𖾂邭���� */
		//	}
		//	else
		//	if (/*bomb_wait*/pd.bomber_time < 190)
		//	{
				pd.state_flag &= (~STATE_FLAG_02_BOMB_AUTO_GET_ITEM);	/* �{���ɂ�鎩�����W�͏I��� */
			}
		}
		else
		{
			pd.bomber_time = 0;
		//}
		//if (/*bomb_wait*/d->bomber_time<=0)
		//{
			if (my_pad & PSP_KEY_BOMB_CANCEL)
			{
				if (0 < pd.bombs)
				{
					pd.bombs--;
					pd.used_bomber++;	/* �{���g�p�� */
					voice_play(VOICE07_BOMB, TRACK02_ALEART_IVENT);/*�e�L�g�[*/
					set_bg_alpha(50);/* ��ʂ��Â����� */
//					set_bg_alpha(100);/* ��ʂ��Â����� */
//					set_bg_alpha(127);/* ��ʂ��Â����� */
					/* �����G */
					player_create_bomber_tachie(/*obj00[FIX_OBJ_00_PLAYER]*/);
					//
					static /*const*/ void (*regist_call_table[/*16*/(PLAYERS8*2)])(SPRITE *src) =
					{
						reimu_create_bomber_homing_shot_parrent,				reimu_create_bomber_kekkai_parrent, 		/* �얲 A */
						reimu_create_bomber_kekkai_parrent, 					marisa_create_bomber_homing_parrent,		/* �얲 B */
						marisa_create_bomber_star_dust_levarie_parrent, 		marisa_create_bomber_homing_parrent,		/* ������ A */
						marisa_create_bomber_master_spark_parrent,				marisa_create_bomber_homing_parrent,		/* ������ B */
						remilia_create_bomber_cross_shot_parrent,				marisa_create_bomber_homing_parrent,		/* ���~���A */
						yuyuko_create_bomber_gyastry_dream_parrent, 			yuyuko_create_bomber_meifu_parrent, 		/* �H�X�q */
						marisa_create_bomber_star_dust_levarie_parrent, 		reimu_create_bomber_homing_shot_parrent,	/* �`���m A */
						yuyuko_create_bomber_gyastry_dream_parrent, 			marisa_create_bomber_homing_parrent,		/* �`���m Q */
					};
						int index_aaa;
						index_aaa = ((select_player)+(select_player)+((my_pad & PSP_KEY_SLOW)?1:0));
					{
						(regist_call_table[(index_aaa)])(&obj00[FIX_OBJ_00_PLAYER]);
					}
					s1->color32 				= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	/* ������ */
					{
						SPRITE *zzz_obj_maru;
						zzz_obj_maru = &obj00[FIX_OBJ_01_JIKI_MARU];
						zzz_obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	/* ������ */
					}
//					if (PLAYER_STATE_01_KURAI_BOMB_UKETUKE==pd.special_ivent_type)/* ��炢�{����t�� */
					if (LIMIT_0_ZERO > pds_status_timer)/* ��炢�{����t�� */
					{
						pd.use_kurai_bomb++;/* ��炢�{������ */
					}
//					pds_status_timer = LIMIT_128;// pd.special_ivent_type	= PLAYER_STATE_03_SAVE_02;
					pd.bomber_time		= (PD_BOMBER_JYUMYOU180);//pd.bomber_time_table[(index_aaa)];
					//	#define USER_BOMOUT_WAIT (30)
					pds_status_timer	= pd.bomber_time + USER_BOMOUT_WAIT;/*�������G����*/
					/*	�������G���� == (�ݒ薳�G���� + USER_BOMOUT_WAIT)�B 	�������G���� == �ݒ薳�G���� ���ƁA�킩��ɂ����̂Ŏ኱�̗]�T���K�v�B */
				//
					kanji_window_clear_line(0); 	/* �����E�B���h�E��1�s��(==0)�̓��e�������B */
					set_cursor(0, 0);				/* �J�[�\����1�s��(==0)�ֈړ� */
					{
						msg_time = pd.bomber_time;	/*(60*5)*/
						typedef struct
						{
							const char *spell_str_name; 	/* �X�y�J���� */
						} aaa_RESOURCE;
						static const aaa_RESOURCE my_aaa_resource[(PLAYERS8*2)] =
						{
//	 "feeddccbbaa99887766554433221100",'\0' 	 "feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{"�아�u���z����v" 				"\n"},	{"�P���u���z���E�v" 				"\n"},//�아�u���z���E�E�~�v
	{"�����u���z���E�v" 				"\n"},	{"�~���u���z�����v" 				"\n"},//�����u���z���E�E�a�v
	{"�����u�X�^�[�_�X�g�����@���G�v"	"\n"},	{"�M���u���쐬�U���v"				"\n"},
	{"�����u�}�X�^�[�X�p�[�N�v" 		"\n"},	{"�����u���쐬�U���v"				"\n"},
	{"�g���u�s���u���b�h�v"			"\n"},	{"�����u�A�N�}�N���X�v" 			"\n"},
	{"�����u�M���X�g���h���[���v"		"\n"},	{"�����u���y�̍s���v"				"\n"},//�����u���쐬�U���v	/* dream cast. ghastly dream(�d�X��). ghastly dream(�얲). */
	{"�ŕ��u�A�C�V�N���X�g���C�N�v" 	"\n"},	{"�X���u�I���U���b�N�v" 			"\n"},	/* ��(�����): �J����������A�n���������Ⴊ�Ăѓ����č~��B */
	{"�ŋ��u�A�^�C�̌��v"				"\n"},	{"��蹁u�㕶�L�b�N�v"				"\n"},	/* �� ���� ��(�݂���): �J�ƐႪ�������č~��B */
						};
						print_kanji000(my_aaa_resource[index_aaa].spell_str_name, /*int color_type*/7, /*int wait*/0);
					}
				}
			}
		}
	}


	#if 0/* ���܂Ō��������{������(��Ȃ���) */
//	/* �ᑬ�{�� */			pd.bomber_time = 200/*320*/ /*400*/ /*800*/;			/* �{���̗L������(�ݒ薳�G����)[���P�d�l���ς�����̂Ŕ����ɂ���] */
//	/* �얲 */				pd.bomber_time = 180/*200*/;							/* �{���̗L������(�ݒ薳�G����) */	/* ������̓{�X�L��(�U���^) */
//	/* ������	�`���m */	pd.bomber_time = 100/*32*/ /*100*/; 					/* �{���̗L������(�ݒ薳�G����) */
//	/* �� �L������ */		pd.bomber_time = 255/*255==((16*4*2*2)-1)*/;			/* �{���̗L������(�ݒ薳�G����) */
//	���~���A�{��			pd.bomber_time = 0x7e	/* 255==0xff==8��	254==0xfe==7��==0xbf=192 4��==0x7f 150==0x96*/;
	static const u8 pd.bomber_time_table[/*16*/(PLAYERS8*2)] =
	{
		180,		180,		/* 180 200 �얲 A */
		180,		200,		/* 180 200 �얲 B */
		100,		180,		/* 100 200 ������ A */
		100,		200,		/* 100 200 ������ B */
		0x7e,		255,		/* 0x7e 200���~���A */
		180,		180,		/* 255 200 �H�X�q */
		100,		180,		/* 100 200 �`���m A */
		100,		255,		/* 100 200 �`���m Q */
	};
	#endif
//
//	if (PLAYER_STATE_01_KURAI_BOMB_UKETUKE==pd.special_ivent_type) return;/* ��炢�{����t�� */
	if (LIMIT_0_ZERO > pds_status_timer)	{	return; 	}	/* ��炢�{����t�� */
//
	/*---------------------------------------------------------
		�v���C���[�ړ��֘A����
	---------------------------------------------------------*/
	/*
		256�Œ菬���_�Ŏ��@�̈ړ���
		1.0dot	 == 256 == t256(1.0),			�c���̏ꍇ
		��0.7dot == 181 == t256(1.0/��2.0)		�΂߂̏ꍇ
	*/
	static const s16 jiki_move_length[16][2] =
	{
	/*LDRU*/ /* y x */
	// �΂߈ړ������߂���̂��C���B
	/*0000*/ {	  0,	0},
	/*0001*/ {	  0, -256},/*0 U*/
	/*0010*/ {	256,	0},/*2 R*/
	/*0011*/ {	181, -181},/*1 UR*/
	/*0100*/ {	  0,  256},/*4 D*/
	/*0101*/ {	  0,	0},
	/*0110*/ {	181,  181},/*3 DR*/
	/*0111*/ {	  0,	0},
	/*1000*/ { -256,	0},/*6 L*/
	/*1001*/ { -181, -181},/*7 UL*/
	/*1010*/ {	  0,	0},
	/*1011*/ {	  0,	0},
	/*1100*/ { -181,  181},/*5 DL*/
	/*1101*/ {	  0,	0},
	/*1110*/ {	  0,	0},
	/*1111*/ {	  0,	0},
	};
/* �ԉf�ˉ�b������T�C�g http://ambriel.hp.infoseek.co.jp/th9/  ��{���\��r �ړ����x(�������[�h)
														[51200/xxxF]
200/50		50F ��										0x0400[1024.000000000000000000000000000000]3.63636363636363636363636363636364
200/55		55F [������] �~�X�e�B�A (���f�B,  �l�G) 	0x03a3[ 930.909090909090909090909090909091]3.63636363636363636363636363636364
200/58		58F [�`���m]								0x0373[ 882.758620689655172413793103448276]3.44827586206896551724137931034483
200/61		61F (���,	���i�T) �Ă�					0x0347[ 839.344262295081967213114754098361]3.27868852459016393442622950819672		[REMILIA]
200/65		65F �����J									0x0314[ 787.692307692307692307692307692308]3.07692307692307692307692307692308
200/67		67F ����									0x02fc[ 764.179104477611940298507462686567]2.98507462686567164179104477611940
200/68		68F [�얲] ��� 							0x02f1[ 752.941176470588235294117647058824]2.94117647058823529411764705882353
200/72		72F ��������								0x02c7[ 711.111111111111111111111111111111]2.77777777777777777777777777777778		[YUYUKO]
200/91		91F �H��									0x0233[ 562.637362637362637362637362637363]2.19780219780219780219780219780220

�z��ړ����x(�ᑬ���[�h)
200/79		79F 	��									0x0288[ 648.101265822784810126582278481013]2.53164556962025316455696202531646
200/80		80F 	�~�X�e�B�A							0x0280[ 640.000000000000000000000000000000]2.50000000000000000000000000000000
200/83		83F 	[�`���m]							0x0269[ 616.867469879518072289156626506024]2.40963855421686746987951807228916
200/85		85F 	���								0x025a[ 602.352941176470588235294117647059]2.35294117647058823529411764705882
200/91		91F 	[������]							0x0233[ 562.637362637362637362637362637363]2.19780219780219780219780219780220
200/109 	109F	(���,	���i�T) 					0x01d6[ 469.724770642201834862385321100917]1.83486238532110091743119266055046		[REMILIA]
200/114 	114F	(���f�B,  �l�G) 					0x01c1[ 449.122807017543859649122807017544]1.75438596491228070175438596491228
200/124 	124F	�����J ���� 						0x019d[ 412.903225806451612903225806451613]1.61290322580645161290322580645161
200/130 	130F	�d��								0x018a[ 393.846153846153846153846153846154]1.53846153846153846153846153846154
200/136 	136F	�������� [�얲] 					0x0178[ 376.470588235294117647058823529412]1.47058823529411764705882352941176
200/145 	145F	�Ă�								0x0161[ 353.103448275862068965517241379310]1.37931034482758620689655172413793		[YUYUKO]
200/148 	148F	�H��								0x015a[ 345.945945945945945945945945945946]1.35135135135135135135135135135135

*/
//#define BASE_SPEED_MINIMUM		(PLAYERS8*4)
//#define BASE_SPEED_MAXIMUM		(PLAYERS8*5)
//	 2,  3,  4,  7,  3, 	/* �Œᑬ�x player_speed_minimum */
//	 4,  5,  4,  7,  3, 	/* �ō����x player_speed_maximum */
	/* �������[�h(�ʏ펞) */								/* �������[�h(�{��������) */
	#define SPEED_HIGH_REIMU_A		(0x02f1)	/* t256(2.94) 2.94140625		  t256(3.0), */
	#define SPEED_HIGH_REIMU_B		(0x02f1)	/* t256(2.94) 2.94140625		  t256(3.0), */
	#define SPEED_HIGH_MARISA_A 	(0x03a3)	/* t256(3.64) 3.63671875		  t256(2.0), */
	#define SPEED_HIGH_MARISA_B 	(0x03a3)	/* t256(3.64) 3.63671875		  t256(2.0), */
	#define SPEED_HIGH_REMILIA		(0x0347)	/* t256(3.28) 3.27734375		  t256(5.0), */
	#define SPEED_HIGH_YUYUKO		(0x02c7)	/* t256(2.78) 2.77734375		  t256(5.0), */
	#define SPEED_HIGH_CIRNO_A		(0x0373)	/* t256(3.45) 3.44921875		  t256(4.5), */ 			/* �H������ᑬ�̕������� */
	#define SPEED_HIGH_CIRNO_Q		(0x0373)	/* t256(3.45) 3.44921875		  t256(4.5), */ 			/* �H������ᑬ�̕������� */
														/* �ᑬ���[�h(�ʏ펞) */								/* �ᑬ���[�h(�{��������) */
	#define SPEED_LOW_REIMU_A		(0x0178)	/* t256(1.47) 1.46875000 t256(2.0) t256(2.0), */
	#define SPEED_LOW_REIMU_B		(0x0178)	/* t256(1.47) 1.46875000 t256(2.0) t256(2.0), */
	#define SPEED_LOW_MARISA_A		(0x0233)	/* t256(2.20) 2.19921875 t256(2.5) t256(2.0), */
	#define SPEED_LOW_MARISA_B		(0x0233)	/* t256(2.20) 2.19921875 t256(2.5) t256(2.0), */
	#define SPEED_LOW_REMILIA		(0x01d6)	/* t256(1.84) 1.83593750 t256(2.5) t256(4.0), */
	#define SPEED_LOW_YUYUKO		(0x0161)	/* t256(1.38) 1.37890625 t256(2.0) t256(4.5), */
	#define SPEED_LOW_CIRNO_A		(0x0269)	/* t256(2.41) 2.41015625 t256(7.0) t256(5.0), */ /* 0x0700 �H������ᑬ�̕������� 0x0400==��������==���z���ő��H */
	#define SPEED_LOW_CIRNO_Q		(0x0700)	/* t256(2.41) 2.41015625 t256(7.0) t256(5.0), */ /* 0x0700 �H������ᑬ�̕������� 0x0400==��������==���z���ő��H */

	#define PLAYER_WIDTH			(50)	/*	�Œ�T�C�Y */
	#define PLAYER_HEIGHT			(50)	/*	�Œ�T�C�Y */

	static const signed /*int*/short player_speed256[(PLAYERS8*4)] =
	{	/* �������[�h(�ʏ펞) */								/* �������[�h(�{��������) */
		SPEED_HIGH_REIMU_A, 		/* REIMU_A */
		SPEED_HIGH_REIMU_B, 		/* REIMU_B */
		SPEED_HIGH_MARISA_A,		/* MARISA_A */
		SPEED_HIGH_MARISA_B,		/* MARISA_B */
		SPEED_HIGH_REMILIA, 		/* REMILIA */
		SPEED_HIGH_YUYUKO,			/* YUYUKO */
		SPEED_HIGH_CIRNO_A, 		/* CIRNO_A */
		SPEED_HIGH_CIRNO_Q, 		/* CIRNO_Q */
		/* �ᑬ���[�h(�ʏ펞) */								/* �ᑬ���[�h(�{��������) */
		SPEED_LOW_REIMU_A,			/* REIMU_A */
		SPEED_LOW_REIMU_B,			/* REIMU_B */
		SPEED_LOW_MARISA_A, 		/* MARISA_A */
		SPEED_LOW_MARISA_B, 		/* MARISA_B */
		SPEED_LOW_REMILIA,			/* REMILIA */
		SPEED_LOW_YUYUKO,			/* YUYUKO */
		SPEED_LOW_CIRNO_A,			/* CIRNO_A */
		SPEED_LOW_CIRNO_Q,			/* CIRNO_Q */
	#if 0/*[�}�X�^�[�X�p�[�N���́A���x�����B]*/
		/* �������[�h(�ʏ펞) */								/* �������[�h(�{��������) */
		SPEED_HIGH_REIMU_A, 		/* REIMU_A */
		SPEED_HIGH_REIMU_B, 		/* REIMU_B */
		SPEED_HIGH_MARISA_A,		/* MARISA_A */
		(SPEED_HIGH_MARISA_B>>1),	/* MARISA_B */
		SPEED_HIGH_REMILIA, 		/* REMILIA */
		SPEED_HIGH_YUYUKO,			/* YUYUKO */
		SPEED_HIGH_CIRNO_A, 		/* CIRNO_A */
		SPEED_HIGH_CIRNO_Q, 		/* CIRNO_Q */
		/* �ᑬ���[�h(�ʏ펞) */								/* �ᑬ���[�h(�{��������) */
		SPEED_LOW_REIMU_A,			/* REIMU_A */
		SPEED_LOW_REIMU_B,			/* REIMU_B */
		SPEED_LOW_MARISA_A, 		/* MARISA_A */
		(SPEED_LOW_MARISA_B>>1),	/* MARISA_B */
		SPEED_LOW_REMILIA,			/* REMILIA */
		SPEED_LOW_YUYUKO,			/* YUYUKO */
		SPEED_LOW_CIRNO_A,			/* CIRNO_A */
		SPEED_LOW_CIRNO_Q,			/* CIRNO_Q */
	#endif
	};
	/* ���@���x�����߂�B */
	unsigned int is_slow;
	is_slow = (my_pad & PSP_KEY_SLOW);
	/*const*/ signed int my_speed = player_speed256[select_player + ((is_slow)?(PLAYERS8):(0))];
	/* �ړ��ʂ����߂�B(�ړ��� = ���@���x x �A�i���O�L�[�A�f�W�^���̏ꍇ�͗\�߃A�i���O�L�[�ړ��ʂɕϊ����Ă���) */
	#if 1/*[�}�X�^�[�X�p�[�N���́A���x�����B]*/
	if (
		(MARISA_B==select_player) &&	/* ������B(����) */
		(0<pd.bomber_time)				/* �}�X�^�[�X�p�[�N�� */
	)
		#if 0
	{
		;/* �}�X�^�[�X�p�[�N���̓V���b�g�����ĂȂ��炵�� */
	}
	if (is_master_spark)
		#endif
	{	(my_speed >>= 1);	}
	#endif
	short	aaa_my_analog_x = (((my_speed)*(my_analog_x))>>8);
	short	aaa_my_analog_y = (((my_speed)*(my_analog_y))>>8);
	/* �΂߂��l�����Ĉړ�����B */
	s1->cx256 += ((((signed int)(jiki_move_length[((my_pad&0xf0)>>4)][0]))*(aaa_my_analog_x))>>8);	/*fps_factor*/
	s1->cy256 += ((((signed int)(jiki_move_length[((my_pad&0xf0)>>4)][1]))*(aaa_my_analog_y))>>8);	/*fps_factor*/
	/* ��ʊO�ɁA�݂͂�������C���B */
	#if 1/*Gu(���S���W)*/
//		 if (s1->cx256 < t256(0))				{	s1->cx256 = t256(0);				}/* ���`�F�b�N */
//	else if (s1->cx256 > t256(GAME_WIDTH)  )	{	s1->cx256 = t256(GAME_WIDTH);		}/* �E�`�F�b�N */
//		 if (s1->cy256 < t256(0))				{	s1->cy256 = t256(0);				}/* ��`�F�b�N */
//	else if (s1->cy256 > t256(GAME_HEIGHT) )	{	s1->cy256 = t256(GAME_HEIGHT);		}/* ���`�F�b�N */
	/* psp �́A�e������ɂ͂�����Ȃ�ł��㉺����������B�㉺����ݏo���̂̓Q�[�����ƁA�Q�[����ʐ݌v�̗����̖��ňӐ}�I�ł���B
		�Q�[����ʐ݌v��͍��E�͋�ݏo���Ă��ǂ��̂����A�͕핗�ł͋�ݏo���Ȃ����ɂ���B
		����͍��E�̕ǂŔ��˂���e�������A��ݏo�����ƒx���Ȃ肻���Ȃ̂œs����̖��B
		��邳��̏�����͉����ɂȂ�����o����̂��B */
		 if (s1->cx256 < t256(PLAYER_WIDTH/2))					{	s1->cx256 = t256(PLAYER_WIDTH/2);					}/* ���`�F�b�N(��ݏo���Ȃ�) */
	else if (s1->cx256 > t256(GAME_WIDTH-(PLAYER_WIDTH/2))	)	{	s1->cx256 = t256(GAME_WIDTH-(PLAYER_WIDTH/2));		}/* �E�`�F�b�N(��ݏo���Ȃ�) */
//		 if (s1->cy256 < t256(PLAYER_HEIGHT/2)) 				{	s1->cy256 = t256((PLAYER_HEIGHT/2));				}/* ��`�F�b�N(��ݏo���Ȃ�) */
//	else if (s1->cy256 > t256(GAME_HEIGHT-(PLAYER_HEIGHT/2)) )	{	s1->cy256 = t256(GAME_HEIGHT-(PLAYER_HEIGHT/2));	}/* ���`�F�b�N(��ݏo���Ȃ�) */
		 if (s1->cy256 < t256(0))								{	s1->cy256 = t256(0);								}/* ��`�F�b�N(�オ��ݏo�����A��ݏo���Ȃ����܂��d�l�����܂��ĂȂ�) */
	else if (s1->cy256 > t256(GAME_HEIGHT)) 					{	s1->cy256 = t256(GAME_HEIGHT);						}/* ���`�F�b�N(������ݏo���͈̂Ӑ}�I) */
	#endif
	/* �R�A�ړ� */
	{
		SPRITE *s2;
	//	s2 = obj_maru;
		s2 = &obj00[FIX_OBJ_01_JIKI_MARU];
		#if 1/*Gu(���S���W)*/
		s2->cx256 = s1->cx256;	/* ���S���W�Ȃ̂ŃR�A�I�t�Z�b�g�͂Ȃ� */
		s2->cy256 = s1->cy256;	/* ���S���W�Ȃ̂ŃR�A�I�t�Z�b�g�͂Ȃ� */
		#endif
//		if (is_slow)	{	s2->type		&= (~(0x08));/*s2->flags		|= ( (SP_FLAG_VISIBLE));*/	}	/* ���A�\�� */
//		else			{	s2->type		|= ( (0x08));/*s2->flags		&= (~(SP_FLAG_VISIBLE));*/	}	/* ���A��\�� */
		/* �ᑬ���[�h�p�G�t�F�N�g�̕\�� */
		{
			SPRITE *obj_tei_soku;
//			obj_tei_soku					= obj_effect[TEISOKU_EFFECT_00_OBJ];
			obj_tei_soku					= &obj00[FIX_OBJ_15_JIKI_TEISOKU_EFFECT];
			if (is_slow)
			{
//				SPRITE *zzz_obj_maru;
//				zzz_obj_maru				= &obj00[FIX_OBJ_01_JIKI_MARU];
//				obj_tei_soku->type			= (SPELL_SQUERE_);
				obj_tei_soku->type			= (SPELL_SQUERE_);
//				obj_tei_soku->flags 		|= (/*SP_FLAG_COLISION_CHECK*/SP_FLAG_VISIBLE/*|SP_FLAG_VISIBLE*/);
//				obj_tei_soku->flags 		|= ( (SP_FLAG_VISIBLE));		/* �ᑬ���[�h�p�G�t�F�N�g�A�\�� */
				obj_tei_soku->cx256 		= (s1->cx256);	/* ���S���W�Ȃ̂ŃR�A�I�t�Z�b�g�͂Ȃ� */
				obj_tei_soku->cy256 		= (s1->cy256);	/* ���S���W�Ȃ̂ŃR�A�I�t�Z�b�g�͂Ȃ� */
				#if 1
				/* �`��p�p�x(����0�x�ō����(�����v���)) */
				obj_tei_soku->rotationCCW1024 += (10);
				#endif
			}
			else
			{
				obj_tei_soku->type			= (SP_DUMMY_MUTEKI);/*(S P_GROUP_ETC_DUMMY_REMILIA);*/
			//	obj_tei_soku->type			= (CORE_HIDE_10_REIMU_A);
//				obj_tei_soku->flags 		&= (~(SP_FLAG_VISIBLE));		/* �ᑬ���[�h�p�G�t�F�N�g�A��\�� */
			//	obj_tei_soku->type			= (SPELL_SQUERE_);
//				obj_tei_soku->type			= FIX_OBJ_OFF;	/* �Œ�obj�̏ꍇ�̒P�Ȃ�OFF */	/* �܂��Œ�obj�ɂȂ��ĂȂ� */
			}
		}
	}
	/* MAX���̃A�C�e���������W */
	if (my_pad & PSP_KEY_UP/*PSP_CTRL_UP*/) 	/* ���F�΂ߏ�ł�����\ */ /*&& (s1->y>0)*/
	{
		#if 1
			/* �S�L����: �A�C�e���㕔���W�����ł��\ */
		#else
		if ((pd.weapon_power==MAX_POWER_IS_128) 	// 128[***090123 �ύX /*max==MAX_POWER_IS_128==�u129�i�K�v*/
			/* ������ ����\�́F�A�C�e���㕔���W�����ł��\ */
			|| (MARISA_A==select_player)
			|| (MARISA_B==select_player)
		//	|| (MARISA_C==select_player)
		)	/* �������͏�ɏ㕔�������W���\ */
		#endif
		{
			if (s1->cy256 < PLAYER_SPLIT_LINE256/*t256(50)*/)/* [FULL�p���[���̃A�C�e�������񂹃��C��] */
			{
				pd.state_flag |= STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM; /* �㕔�������W�\ */
			}
		}
	}
	else if (my_pad & PSP_KEY_DOWN/*PSP_CTRL_DOWN*/)	/* ���F�΂߉��ł���߂� */ /*&& (s1->y<screen->h*-s1->h)*/
	{
		//if (pd.state_flag & (STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM /*| STATE_FLAG_02_BOMB_AUTO_GET_ITEM*/) )/*???���Ԃ�*/
		{
			pd.state_flag &= (~STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM);	/* �㕔�������W�s�� */
		}
	}
//
		/*---------------------------------------------------------
			�v���C���[�V���b�g�֘A����
		---------------------------------------------------------*/
	/* �V�i���I�X�N���v�g���[�h�ł̓V���b�g�{�^�����͖��� */
	if (0==(pd.state_flag & STATE_FLAG_06_IS_SCRIPT))
	{
		if (my_pad & PSP_KEY_SHOT_OK)
		{
			pd.state_flag |= STATE_FLAG_15_KEY_SHOT;	/* on */
			register_main_shot(s1);
		}
	}
	#if 0/*�f�o�b�O�p*/
	p d_my_score	= (weapon_level_offset);/*�f�o�b�O*/
	p d_graze_point = (weapon_level_offset);/*�f�o�b�O*/
	#endif

	/*---------------------------------------------------------
		�v���C���[�A�j���[�V�����֘A����
	---------------------------------------------------------*/
	{static int anime_delay;
		anime_delay -= 1;	/*fps_fa_ctor*/
		if (0 > anime_delay)
		{
			anime_delay = player_fix_status[BASE_SPEED_ANIME+select_player];/*2*/
			//
			static int auto_anime_frame = (4);/*5*/
				 if (my_pad & PSP_KEY_LEFT/*PSP_CTRL_LEFT*/)	{	if ( (0) < auto_anime_frame )	{auto_anime_frame--;} }
			else if (my_pad & PSP_KEY_RIGHT/*PSP_CTRL_RIGHT*/)	{	if ( (8) > auto_anime_frame )	{auto_anime_frame++;} }
			else
			{
				if (auto_anime_frame>4/*5*/) auto_anime_frame--;
				if (auto_anime_frame<4/*5*/) auto_anime_frame++;
			}
			#if 1
			{
				int now_anime_frame;
				static int auto_aniime;
				auto_aniime++;
				auto_aniime &= 3;
					 if (4==auto_anime_frame)	{	now_anime_frame = (auto_anime_frame + auto_aniime); }	/* ���S1-4 */
				else if (4<auto_anime_frame)	{	now_anime_frame = (auto_anime_frame + (3) + (8) );	}	/* �E��1-4 */	/* +(8) == �I�v�V�����C���^�[���[�u */
				else							{	now_anime_frame = (auto_anime_frame );				}	/* ����1-4 */
				s1->type = (JIKI_PLAYER_00_LEFT)+(now_anime_frame);
			}
			#else
			{
				static int auto_aniime;
				auto_aniime++;
				auto_aniime &= 3;
				s1->type = (JIKI_PLAYER_00_LEFT)+(auto_anime_frame );
					 if (4==auto_anime_frame)	{	s1->type += ( auto_aniime); }	/* ���S1-4 */
				else if (4<auto_anime_frame)	{	s1->type += ( 3+8); 		}	/* �E��1-4 */
			//	else							{								}	/* ����1-4 */
			}
			#endif
		}
	}
/*
0 1 2 3 4 5 6  7  8  :: auto_anime_frame[0 to 8]
0 1 2 3 4 8 9 10 11  :: now_anime_frame [0 to 11]
		5
		6
		7
*/
}

/*---------------------------------------------------------
	���탌�x�����ύX���ꂽ�ꍇ�ɁA�����I�v�V�����̏�Ԃ��`�F�b�N���ĕύX
---------------------------------------------------------*/
/* [pd.weapon_power���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
static void check_weapon_level(void)
{
	/*---------------------------------------------------------
		����̒i�K�����߂�
	---------------------------------------------------------*/
	//weapon�̒i�K���獡�̑��������߂�		//���X�g�����đ����̕ύX
	// [***090123	�ő�128�ցB
	// [***100123	�ő�129�ցB
	// (0-128��129�i�K�ɏC��)
	#if 0//1968661
	/* ���낢��撣���Ă݂����A���̒���(6,11,61,86)�͒N��(?)�Ȃ̂ŕς���(2�{�𒴂���l�̒����͈Ӗ������C��������) */
	u8 pds_weapon;	pds_weapon = pd.weapon_power;
		 if (pds_weapon < ( 8)) 	{	weapon_level_offset = (WEAPON_L0<<3);	}	/* WEAPON_L0(P000-P008) */	/* 6*/
	else if (pds_weapon < ( 16))	{	weapon_level_offset = (WEAPON_L1<<3);	}	/* WEAPON_L1(P008-P015) */	/*11*/
	else if (pds_weapon < ( 32))	{	weapon_level_offset = (WEAPON_L2<<3);	}	/* WEAPON_L2(P016-P031) */	/*61*/
	else if (pds_weapon < ( 64))	{	weapon_level_offset = (WEAPON_L3<<3);	}	/* WEAPON_L3(P032-P063) */	/*86*/
	else if (pds_weapon < (128))	{	weapon_level_offset = (WEAPON_L4<<3);	}	/* WEAPON_L4(P064-P127) */
	else							{	weapon_level_offset = (WEAPON_L5<<3);	}	/* WEAPON_L5(P128)		*/	/*max==P128==�u129�i�K�v*/
	weapon_level_offset += select_player;
	#endif
	#if 1//1968181
	u8 pds_weapon;	pds_weapon = pd.weapon_power;
	/* 0000 0001 1111 1--- */
	/* �g�A���ׂ��璚�x2�{���ۂ��B */
	{	/* ���x2�{���Ƃ���Ȋ����Ŋȗ����ł���B */
		weapon_level_offset = (select_player/*0*/);
		weapon_level_offset += (WEAPON_L5<<3);
		int jj;
		for (jj=(0); jj<(5); jj++)
		{
			if (pds_weapon & ((0x80)>>jj))
			{
				goto loop_end;//break;
			}
			weapon_level_offset -= (0x08);
		}
		loop_end:;
	}/*
0000 0000 0000 0000
		  5432 1 0
		  0123 4
*/
	#endif
//
	/*---------------------------------------------------------
		�v���C���[�I�v�V�����`�F�b�N(�K�v�ł���΃I�v�V�������Z�b�g)
	---------------------------------------------------------*/
	#if 1
	{
		/* ���낢��撣���Ă݂����A���̒���(35,55,80,108)�͒N��(?)�Ȃ̂ŕς���(2�{�𒴂���l�̒����͈Ӗ������C��������) */
	//	static const u8 jjj_tbl[4] =
	//	{
	//		( 8),		//(35-1),
	//		(16),		//(55-1),
	//		(32),		//(80-1),
	//		(64)		//(108-1),
	//	};
		int jj;
		for (jj=(0); jj<(4); jj++)/* �I�v�V������4�� */
		{
			SPRITE *zzz_obj_option;
			zzz_obj_option = &obj00[FIX_OBJ_04_JIKI_OPTION0+jj];
			if ( (pds_weapon > ((0x08)<<jj)/*jjj_tbl[jj]*/))	/* ���x2�{���Ƃ���Ȋ����Ŋȗ����ł���B */
					{	zzz_obj_option->flags	|= ( (SP_FLAG_OPTION_VISIBLE)); 	}	// ���t���O��On(��)
			else	{	zzz_obj_option->flags	&= (~(SP_FLAG_OPTION_VISIBLE)); 	zzz_obj_option->cy256=t256(GAME_HEIGHT+1+16);	}	// ���t���O��OFF(�s��)
		}
	}
	#endif
}
/*
����
;-------------------
�얲 �아	�V���b�g�i�z�[�~���O�A�~�����b�g�j		�{�� �i���z����j

L0	1way(�܂�������)								P000-P007			(���i1way)
�I�v�V����2�ǉ�									P008-P015
L1	2way(�E��A����)								P016-	28			(�Ԋu������2way)

L3	3way(�܂��������A�E�Ώ�A���Ώ�)				P032-	43			(�Ԋu���L��3way)

Lx	3way(�܂��������A�E�Ώ�A���Ώ�)				P064-	113 	124 (�Ԋu���L��3way)

124 �I�v�V����3����

Lx	4way(�܂������E�A�܂��������A�E�Ώ�A���Ώ�)			MAX(128)	(�Ԋu���L��4way)

;-------------------
�얲 ����	�V���b�g�i�p�X�E�F�C�W�����j�[�h���j	�{�� �i�����w�j

;-------------------
������ ���� �V���b�g�i�}�W�b�N�~�T�C���j			�{�� �i�X�^�[�_�X�g�����@���G�j

;-------------------
������ ���� �V���b�g�i�C�����[�W�������[�U�[�j		�{�� �i�}�X�^�[�X�p�[�N�j

;-------------------
*/

/*---------------------------------------------------------
	�v���C���[�ƃA�C�e���̂����蔻��1(�A�C�e����p)
	-------------------------------------------------------
		src 	�v���C���[
		ttt 	�A�C�e��
---------------------------------------------------------*/
static void player_add_power_type(SPRITE *src, SPRITE *ttt)
{
	{	const u8 aaa[4] = { 1, 8, 127, 1/*�`���m�p*/};
		const int add_power = aaa[/*((ttt_type))*/((NULL==ttt)?(0/*3 �`���m�p*/):((ttt->type)&0x03))];
		int previous_power; previous_power = pd.weapon_power;		/* �A�C�e���擾���钼�O�̃p���[�l */
		pd.weapon_power += add_power;	/* �p���[�l�A���Z */
		/* �p���[�l�A�����Ă���C�� */
		if ((MAX_POWER_IS_128-1) < pd.weapon_power )/*127==128�i�K��*/
		{
			pd.weapon_power = MAX_POWER_IS_128;/* �ő�l */
		}
		int add_score_point;
		if (MAX_POWER_IS_128 == pd.weapon_power/*previous_power*/) /* max�̏ꍇ */
		{
			if (previous_power != pd.weapon_power)/* ���O��MAX����Ȃ������̂ɁA����MAX�ɂȂ����ꍇ */
			{
				#if 1
				/* ����@�\ */
				bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
				#endif
			}
			pd.chain_point += add_power;	/* �`�F�C���A���Z */
			/* �`�F�C���A�����Ă���C�� */
			if (31 < pd.chain_point)
			{
				pd.chain_point = 31;/* �ő�l */
			}
			/* (�m�[�~�X�{�[�i�X)�E�F�|���{�[�i�X���_�v�Z */
			add_score_point = (/*score_type*/(pd.chain_point));
			#if 1
			/* �`�F�C���̓{��MAX�łȂ��Ǝ����A���܂�Ȃ� */
				#if 0
		//	if (NULL==ttt)/* �`���m�Ȃ� */
				#else
			/* ���~���A & �H�X�q & �`���m�́A�{������^(��{�I�ɂ��܂蒙�߂Ȃ��łǂ�ǂ�g��==���ƒ������܂�)�ɂ��悤���ƁB */
			if ((REMILIA-1) < select_player)/* ���~���A & �H�X�q & �`���m�Ȃ� */
				#endif
			{
				if ((8-1) < pd.bombs)	{	;	}/* ���ɍő�l(8)�Ȃ�΁A�Ȃɂ������B */
				else
				{
					pd.bombs++; 	/* �{���ǉ� */
					pd.weapon_power = (MAX_POWER_IS_128>>1);/* �ő�l�̔��� */
				}
			}
			#endif
		}
		else	/* max�łȂ��ꍇ�A���_�v�Z */
		{
			/* (MAX���ȊO��) [P]��10�_ */
			/* (MAX���ȊO��) [P��]��80�_ */
			add_score_point = ((add_power/*-1*/)/*SCORE_10*/);
		}
		if (NULL!=ttt)/* �`���m�ȊO�Ȃ� */
		{
			bonus_info_score_nodel(ttt, add_score_point);
			/* */ttt->jyumyou = JYUMYOU_NASI;/* �����܂� */
		}
	}
	check_weapon_level();/* [pd.weapon_power���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
//
	voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*�e�L�g�[*/
}

static void player_colision_check_item(SPRITE *src)/*, int mask*/	/*,SP_GROUP_ITEMS*/
{
	/* ���@���A�C�e���ɂ��������ꍇ */
	SPRITE *ttt;	//�Ώ�
	ttt = sprite_collision_check_444(src, SP_GROUP_ITEMS);	/* �e����p(�A�C�e��) */	/*mask*/
	if (NULL != ttt)
	{
		switch (ttt->type)
		{
		case SP_ITEM_00_P001:	//	player_add_power(src, ttt, 1);		break;	// �E�F�|���A�C�e��[��p]
		case SP_ITEM_01_P008:	//	player_add_power(src, ttt, 8);		break;	// �E�F�|���A�C�e��[��P]	// [***090123		�ǉ�
		case SP_ITEM_02_P128:	//	player_add_power(src, ttt, 127);	break;	// �E�F�|���A�C�e��[F]		// [***090123		�ǉ�
									player_add_power_type(src, ttt);	break;	// �E�F�|���A�C�e��[��p][��P][F]
		//
		case SP_ITEM_04_BOMB:
			//player_add_bomb(t);		// [*****�{���̓R�����g�A�E�g���Ȃ���
			if ((8-1) < pd.bombs)	{	goto add_10000pts;	}/* ���ɍő�l(8)�Ȃ�΁A10000+ [pts] */
			pd.bombs++;
			#if 1/*�o�Ofix?*/
			ttt->jyumyou = JYUMYOU_NASI;/* �����܂� */
			#endif
			voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*�e�L�g�[*/
			break;

		case SP_ITEM_03_1UP:
			if ((8-1) < pd.zanki)	{	goto add_10000pts;	}/* ���ɍő�l(8)�Ȃ�΁A10000+ [pts] */
			pd.zanki++;
			#if 1/*�o�Ofix?*/
			ttt->jyumyou = JYUMYOU_NASI;/* �����܂� */
			#endif
			/*effect_sound_number=*/voice_play(VOICE06_EXTEND, TRACK03_SHORT_MUSIC);/*�e�L�g�[*/
			break;

		case SP_ITEM_07_SPECIAL:
			break;
		#if 0
		/* ���_�́A�����蔻��Ȃ� */
		case S P_ITEM_05_HOSI:		/*not_break;*/
			ttt->jyumyou = JYUMYOU_NASI;/* ���_�̂ݓ��ʏ��� */
//
			voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*�e�L�g�[*/
			break;
		#endif
		case SP_ITEM_05_TENSU:
			{
				/* PLAYER_SPLIT_LINE256 ����Ŏ��� 10000pts. ... ���Ŏ���(?)��100pts. */
				/* (���90pts�A���ɓ�����A����΂��(�����钼�O��3���C��)70pts�܂ŉ\�炵��) */
			//
				int add_score_point;
				if (PLAYER_SPLIT_LINE256 < src->cy256)	/* PLAYER_SPLIT_LINE256 �����̏ꍇ�́APLAYER_SPLIT_LINE256�܂ł̋����ɂ������Č��_ */
				{
				//	add_score_point -= ((ttt->cy256-PLAYER_SPLIT_LINE256)>>(4+8));
				//	add_score_point = ((/*0x1f-(3)*/(SCORE_9000)-((ttt->cy256-PLAYER_SPLIT_LINE256)>>(3+8)))/*&0x1f*/);
					add_score_point = (SCORE_9000) - ((ttt->cy256-PLAYER_SPLIT_LINE256)>>(3+8));
				}
				else
				{
					add_10000pts:/* �����ɓ��_10000+ [pts] */
				//	add_score_point = (SCORE_10000);	/* ��{�_ 10000[pts] */
					add_score_point = (SCORE_10000+difficulty); 	/* ��{�_ 10000[pts](easy), 11000(normal), 12000(hard), 512000(lunatic). */
				}
				//player_dummy_add_score(score(1000));		// [***090123		�ύX
			//
				bonus_info_score_nodel(ttt, add_score_point);
				/* */ttt->jyumyou = JYUMYOU_NASI;/* �����܂� */
//
			}
			voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*�e�L�g�[*/
			break;
		}
	}
}


/*---------------------------------------------------------
	�҂���[�񏈗�(�T�u)
	-------------------------------------------------------
	�v���C���[�� �G�O���[�v �܂��� �G�e �ɓ��������ꍇ�A
	��炢�{���J�n�ׂ̈́A�҂���[�񏈗����s���B
---------------------------------------------------------*/
static void player_explode(SPRITE *s1);
static void player_set_pichuun(void)
{
//	#if 0//(0==US E_ATARI_DE BUG)/* �����蔻��f�o�b�O���͖��G�B */
//	/* �ʏ펞(�f�o�b�O�ȊO�͎���(��炢�{�����[�h��)) */
//	pd.special_ivent_type		= PLAYER_STATE_01_KURAI_BOMB_UKETUKE;						/* ��炢�{�����͎󂯕t�����[�h�ֈڍs����B */
//	pds_status_timer	= player_fix_status[BASE_HIT_BOMB_DEC_WAIT+select_player];			/* ��炢�{���̓��͎󂯕t�����Ԃ�ݒ� */
//	pds_status_timer	= (-1);/*�Ƃ肠����*/
//	pds_status_timer	= (-1);/*�Ƃ肠����*/	/* �f�o�b�O(��炢�{�����ԁA�S���얲���݂ɒ���)*/
//	#else
	pds_status_timer	= (-1)-player_fix_status[BASE_HIT_BOMB_DEC_WAIT+select_player]; 	/* ��炢�{���̓��͎󂯕t�����Ԃ�ݒ� */
//	#endif
	voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */
	{
		SPRITE *zzz_player;
		zzz_player = &obj00[FIX_OBJ_00_PLAYER];
		player_explode(zzz_player);/*�v���C���[����*/
	}
}



/*---------------------------------------------------------
	�A�C�e���ȊO�̃v���C���[�̂����蔻��B
	-------------------------------------------------------
	�u�G�e�ɂ��O���C�Y�v
	�u�G�e�ɂ�鎀�S�v
	�u�G(�G�����{�X)�ɂ�鎀�S�v
---------------------------------------------------------*/
	//	player_colision_check_graze(s1);	/* �O���C�Y�̓����蔻�� */
	//	player_colision_check_enemy(s1);	/* �G�����{�X�̓����蔻�� */

#define TEST_FIX_GRAZE048	(t256(128.0))
#define TEST_FIX_DEATH001	(t256(1.0))
#define TEST_FIX_TEKI001	(t256(1.0))

static void player_colision_check_all(SPRITE *src)
{
	//static void player_colision_check_graze(SPRITE *src)
	/*---------------------------------------------------------
		�v���C���[�ƓG�e�̂����蔻��2(�O���C�Y��p)
		-------------------------------------------------------
			src 	�v���C���[
			ttt 	�G�e
	---------------------------------------------------------*/
	#if (1)
	src->m_Hit256R		= TEST_FIX_GRAZE048;/* �Ă��ƁA�O���C�Y����B */
	#endif
	SPRITE *ttt;	/* �G�e */
	ttt = NULL;
	ttt = sprite_collision_check_444(src, SP_GROUP_BULLETS);/* �e����p(�e��) */
	if (NULL != ttt)
	{
		/* �v���C���[�ɓG�e�����������ꍇ�̓O���C�Y���� */
		if (0==(ttt->flags & SP_FLAG_GRAZE)) /* �O���C�Y��? */
		{
			ttt->flags |= SP_FLAG_GRAZE;/*�O���C�Y��*/
			pd.graze_point++;/*�O���C�Y��*/
			// �`���m�̏ꍇ�A�O���C�Y�Ńp���[�A�b�v�B
			if ( (CIRNO_A-1) < (select_player) )/* 6:CIRNO_A or 7:CIRNO_Q */
			{
				player_add_power_type(src, /*ttt*/NULL);
			}
			else
			{
				player_dummy_add_score((score(100)+score(200)*difficulty));/*score(500)*/
			}
				/* �O���C�Y�� */
				voice_play(VOICE09_GRAZE, TRACK07_GRAZE);/*�e�L�g�[*/
		}
#if 1
		/* �v���C���[�X�v���C�g���e�ɐG��Ă��邩 */
		{
			SPRITE *uuu;	/* uuu */
			uuu = NULL;
	#if (0)
			SPRITE *zzz_obj_maru;
			zzz_obj_maru = &obj00[FIX_OBJ_01_JIKI_MARU];
			/* (�O���C�Y���Ȃ�R�A�ƓG�e�̂����蔻�������) */
//			#if (0)
//			/* �R�A�Ŕ���(�p�~) */
//			uuu = sprite_collision_check_444(zzz_obj_maru, SP_GROUP_BULLETS);/* �e����p(�e��) */
//			#else
			/* ���@�Ŕ��� */
			int chache_m_Hit256R;
			chache_m_Hit256R	= src->m_Hit256R;/* ���@�̂����蔻��T�C�Y */
			src->m_Hit256R		= zzz_obj_maru->m_Hit256R;/* ���̂����蔻��T�C�Y */
			uuu = sprite_collision_check_444(src, SP_GROUP_BULLETS);/* �e����p(�e��) */
			src->m_Hit256R		= chache_m_Hit256R;/* �߂� */
//			#endif
	#else
			src->m_Hit256R		= TEST_FIX_DEATH001;/* �Ă��ƁA���S����B */
			/* ���@�Ŕ��� */
			uuu = sprite_collision_check_444(src, SP_GROUP_BULLETS);/* �e����p(�e��) */
			src->m_Hit256R		= TEST_FIX_GRAZE048;/* �Ă��ƁA�O���C�Y����B */
	#endif
			if (NULL != uuu)	{	player_set_pichuun();	}
		}
#endif
	}

	#if (1)
	src->m_Hit256R		= TEST_FIX_TEKI001;/* �Ă��ƁA�G����B(�G�Ƃ̓����蔻��Ŏ���) */
	#endif
	/*---------------------------------------------------------
		�v���C���[�ƓG�̂����蔻��3(�G��p)
		-------------------------------------------------------
			src 	�v���C���[
			ttt 	�G
		-------------------------------------------------------
		���@���G(�{�X/���^/�U�R)�ɑ̓����肳�ꂽ�ꍇ
	---------------------------------------------------------*/
	//	static void player_colision_check_enemy(SPRITE *src)
	{
		SPRITE *ttt;	/* �G����(�{�X/���^/�U�R) */
		/*SP_GROUP_ENEMYS*/  // (SP_GROUP_TEKI/*|SP_GROUP_BOSS*/);
		ttt = sprite_collision_check_SDL_teki(src);// (SP_GROUP_TEKI)  /*SP_GROUP_ENEMYS*/);/*�G��p*/
		if (NULL != ttt)	{	player_set_pichuun();	}
	}

}


/*---------------------------------------------------------
	�v���C���[�ړ�
---------------------------------------------------------*/

/*---------------------------------------------------------
	�v���C���[���[�v�𔲂��鏈��
---------------------------------------------------------*/
global void player_loop_quit(void)
{
	pd.state_flag		|= STATE_FLAG_14_GAME_LOOP_QUIT;
	#if (0000)
	option[0]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* �I�v�V���������� */
	option[1]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* �I�v�V���������� */
	option[2]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* �I�v�V���������� */
	option[3]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* �I�v�V���������� */
	#endif/*0000*/
//
//	pds_status_timer	= (150);	/* 150	 120�ł͎኱�Z�� 100 */
//	pd.special_ivent_type	= PLAYER_STATE_04_GAME_OUT;/* GAME_OUT�� */
	pds_status_timer	= (LIMIT_m16_GAME_OUT); 	/* �Ƃ肠���� */	/* 150	 120�ł͎኱�Z�� 100 */
}
	#if (00)
	obj00[FIX_OBJ_00_PLAYER]->flags &= (~(SP_FLAG_VISIBLE));	/* ���@�A��\�� */
	obj_maru->flags 	&= (~(SP_FLAG_VISIBLE));	/* ���A��\�� */
//	obj_maru->alpha 	= 0x00; 		// �����������߂ɓ��ꂽ���ǈӖ�����������������Ȃ��B	// �Q�[���I�[�o�[���́��̕\������
	#endif

	/* �I�v�V��������\���̏ꍇ�A�������Ȃ��B */
//	if (0==(src->flags & ( SP_FLAG_OPTION_VISIBLE)))	{	return; 	}


/*---------------------------------------------------------
	�v���C���[��������
---------------------------------------------------------*/
/* �X�e�[�W�f�[�^�[�ǂݍ��݌�̖��G�ݒ� */
global void player_load_stage_muteki(void)
{
//	pds_status_timer	= (40); 						// ���G���� 		/*120*/ /*150-120*/
//	pd.special_ivent_type	= PLAYER_STATE_02_SAVE_01;	// ���G��ԁH
	pds_status_timer	= (LIMIT_512);					// �������G���� 		/*120*/ /*150-120*/
}
/* �X�e�[�W���ɕ�������ꍇ�̖��G�ݒ� */
static void player_few_muteki(void)
{
//	pds_status_timer	= (40); 						// ���G���� 		/*120*/ /*150-120*/
//	pd.special_ivent_type	= PLAYER_STATE_02_SAVE_01;	// ���G��ԁH
	pds_status_timer	= (LIMIT_m127_MIN); 			// ���G���� 		/*120*/ /*150-120*/
}

static void player_fukkatsu_bbb(SPRITE *s1)
{
		/* �v���C���[�ʒu�̏����� */
		#if 1/*Gu(���S���W)*/
		s1->cx256 = t256(GAME_WIDTH/2);
		s1->cy256 = t256(220/*GAME_HEIGHT*/);/*240*/
		#endif
	player_few_muteki();/* �X�e�[�W�J�n���̂ݎ኱�̖��G��ԂɃZ�b�g */
	#if (00)
	/*obj00[FIX_OBJ_00_PLAYER]*/s1->flags			|= ( (SP_FLAG_VISIBLE));			/* ���@�A�\�� */
	#endif
	/*obj00[FIX_OBJ_00_PLAYER]*/s1->color32 		= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);						/* ���@�A������ */	/*	s1->alpha			= 0x50;*/
//
	{
		SPRITE *zzz_obj_maru;
		zzz_obj_maru = &obj00[FIX_OBJ_01_JIKI_MARU];
	//	obj_maru->flags |= ( (SP_FLAG_VISIBLE));						/* ���A�\�� */
		zzz_obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);								/* ���A������ */	/*	obj_maru->alpha = 0x50;*/
	}
	// �`���m�̏ꍇ�A�u�{�������v����X�^�[�g.
	if ( (CIRNO_A-1) < (select_player) )/* 6:CIRNO_A or 7:CIRNO_Q */
	{
		pd.bombs			= (0);	/* �{���������̏����� */
	}
	else/* �`���m�ȊO */
	{
		pd.bombs			= option_config[OPTION_CONFIG_01_BOMB]; 	/* �{���������̏����� */	/*player_fix_status[BASE_BOMBS+select_player]*/  /*3*/
	}
	pd.chain_point		= (0);										/* �m�[�~�X�{�[�i�X�̏����� */	// pd.chain_point(�m�[�~�X�{�[�i�X)�̏����� // �ǂꂾ���A��(�m�[�~�X)��wepon_up���������
//
	check_weapon_level();/* [pd.weapon_power���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
}

/*---------------------------------------------------------
	�v���C���[���ꏈ��
---------------------------------------------------------*/

static void player_explode(SPRITE *s1)
{
	/* �A�C�e����f���o�� */	//	[***090124������ƒǉ�
	{
		if ((0==pd.zanki)
			/*&&(0==difficulty)*/	/* (�g�A���ׂ����Փx�Ɋ֌W�Ȃ�[F]���o��)	*/
		)
		{
			/* �R���e�B�j���[�̏ꍇ(GAME_OUT)easy �̏ꍇ */
			/* ���ʂ�8�f������  */
			/* �R���e�B�j���[�̏ꍇeasy �̏ꍇ�F (�L���b�`�o���鐔�œ_���͈Ⴄ���ǁA�ǂꂩ�P�L���b�`����� POWER �͓���������) */
		//	item_create(s1, (0==difficulty)?(SP_ITEM_02_P128):(SP_ITEM_01_P008), 8, ITEM_MOVE_FLAG_06_RAND_XY);
			item_create(s1, (SP_ITEM_02_P128), 8, ITEM_MOVE_FLAG_06_RAND_XY);
		}
		else
		{
			/* �R���e�B�j���[�̏ꍇnormal�ȏ�̏ꍇ�F FIRE_POWER_G (8) x (8) == 64/128 (�S���W�߂�50%) �B */
			/* �R���e�B�j���[�̏ꍇ(GAME_OUT)normal�ȏ�̏ꍇ */
			/* �R���e�B�j���[�ȊO�̏ꍇ */
			/* �ɒ[�Ɏキ�Ď��񂾏ꍇ�̋~�ύ�:
				  ��肢�l��(���)���Ղ� FULLPOWER �ɂȂ�Ǝv�����A
				���ɖ}�~�X�Ŏ��񂾂Ƃ��Ă��{������Ȃǂ� POWER ���ێ�����̂ŁA
				�ɒ[�Ɏキ�Ď��ʎ��͂قڂȂ��B
				  �ɒ[�Ɏキ�Ď��ʂ̂͌��\���肾�Ǝv���̂ŁA�኱�̋~�ύ􂪕K�v���Ǝv���B
				���̏ꍇ FIRE_POWER_G (8) x (7+1) == 64/128 (�S���W�߂�50%) �ɂ����B
			*/
			#if (0)
			/*
				�p���[�A�b�v�V�X�e�����኱���������̂ŁA(30-1) -> (16) �֊�������������B
 WEAPON_L0(P000-P008)
 WEAPON_L1(P008-P015)
 WEAPON_L2(P016-P031)
 WEAPON_L3(P032-P063)
 WEAPON_L4(P064-P127)
 WEAPON_L5(P128)
					WEAPON_L1�ȉ��Ŏ��񂾏ꍇ�̂݋~�ύ􂪂���A
					�S������WEAPON_L3�ɂȂ�̂ŁA�Ó����Ǝv���B���A������������?
					WEAPON_L1[P15] -> ���� -> �S�����[P8]x6[��] -> [P15+P8x6]==WEAPON_L3[P63]
					WEAPON_L1[P15] -> ���� -> �P���[P8]x1[��] -> [P15+P8x1]==WEAPON_L2[P23]
					WEAPON_L0[P00] -> ���� -> �S�����[P8]x6[��] -> [P00+P8x6]==WEAPON_L3[P48]
					WEAPON_L0[P00] -> ���� -> �P���[P8]x1[��] -> [P00+P8x1]==WEAPON_L1[P08]
					WEAPON_L0[P00] -> ���� -> �R���[P8]x3[��] -> [P00+P8x3]==WEAPON_L2[P24]
					WEAPON_L2[P16] -> ���� -> �S�����[P1]x6[��] -> [P16+P1x6]==WEAPON_L2[P22]
					����Ŕ���(3��)���ƕK��WEAPON_L2�ȏ�ɂȂ�B
			*/
		//	item_create(s1, (((30-1) > pd.weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), (7)/*5*/, ITEM_MOVE_FLAG_06_RAND_XY);
		//	item_create(s1, (((16) > pd.weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), (6), ITEM_MOVE_FLAG_06_RAND_XY);/* �����牺��ł��A������������? */
		//	item_create(s1, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
			#else
			/* �����C�����Ă݂� */
			item_create(s1, (((16) > pd.weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), (3), ITEM_MOVE_FLAG_06_RAND_XY);/* ����Ȑl������΂��3���鎖���O�� */
			item_create(s1, SP_ITEM_00_P001, (5), ITEM_MOVE_FLAG_06_RAND_XY);/* P001���΂�܂�(���܂��A����Ȑl�́A����΂��Ă������ɂ͎��Ȃ��Ƃ��������O��) */
			#endif
		}
		//
		/* ���~���A ����\�́F������������ƕK���{�����o�� */
		if (REMILIA==select_player) 	/* ���~���A�̏ꍇ */
		{
			/* ������������� */
			if (0 != pd.bombs)
			{
				/* �g���ĂȂ����̎����Ă�{����f���o�� */
				item_create(s1, SP_ITEM_04_BOMB, pd.bombs, ITEM_MOVE_FLAG_06_RAND_XY);
			}
		}
	}
//		pd.explode		= 0;
//	if (0==pd.explode)
	{

	//	bakuhatsu_add_type(t->cx256+t256(5),t->cy256+t256(5),/*0,*/BAKUHATSU_MINI00);
	//	t->jyumyou = JYUMYOU_NASI;/* �����܂� */			/* ���������G���� */
	//	pd.explode=0;
	//	pd.bonus=0;
		//	voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */ 	// [***090127	�ꏊ���ړ�����B// �ύX��
		/* �����G�t�F�N�g */

		#if 0/* ���������G�͓|���Ȃ��G��������Ȃ��̂ŁA�����ŕK�������̂͂܂����C������ */
		/* ���������G���� */
		bakuhatsu_add_type(pd.enemy->cx256+t256( 5), pd.enemy->cy256+t256( 5), /*0,*/ BAKUHATSU_MINI00);
		pd.enemy->type	= SP_DELETE;/*???*/
		#endif
		/* �������� */
		#if 1
		{
			int jj;
			for (jj=0; jj<(4); jj++)
			{
				bakuhatsu_add_type_ddd(s1, BAKUHATSU_MINI00);
			}
		}
		#endif
		/*	*/
		pd.bomber_time	= 0;			/* �����������������Ŗ�����(&�`���m)�{�����ĂȂ��Ȃ��Ă� */
		#if 1
	//	pd.bomber_time = 0;/*�s����*/
		set_bg_alpha(255);/* ��ʂ𖾂邭���� */
		#endif

	//	pd.state_flag		= STATE_FLAG_00_NONE;/*???*/
		pd.state_flag		&= (~(	STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM | 	/* �I��� */
									STATE_FLAG_02_BOMB_AUTO_GET_ITEM |			/* �I��� */
									STATE_FLAG_03_SCORE_AUTO_GET_ITEM));		/* �I��� */

	}
}

/*---------------------------------------------------------
	�v���C���[�ړ��A���ꏈ��
	-------------------------------------------------------
	���ꒆ�A�������A�{�����AGAME_OUT���Ȃǂ̓��ꏈ��
	-------------------------------------------------------
	�\��:�^�C�}�[�Đ݌v�B

	�O�ȊO�̏ꍇ�^�C�}�[�͎�����1���Z(�f�N�������g)�B
	pds_status_timer--;
	pds_status_timer &= 0xff; // ���炩�̒l�Ń}�X�N
//
	���l(-255):
		������							(�����L�[���͖����B�{���ᑬ���L�[���́A����)
	���l(-127): LIMIT_m127_MIN
		�Q�[���I�[�o�[�f��				(�����L�[���͖����B�{���ᑬ���L�[���́A�����B�G�����薳���B�e�����薳���B)
	���l(-16): LIMIT_m16_GAME_OUT		PLAYER_STATE_04_GAME_OUT����(-256)�B
		��炢�{����t					(�����L�[���͖����B�{���ᑬ���L�[���́A�L���B�G�����薳���B�e�����薳���B)
		�}�C�i�X���l�܂ł͋�炢�{����t���ԁB�e�ɂ��������ꍇ�A�L�����ʂŏ����l(-240�Ƃ�-200�Ƃ�)���Ⴄ�B
	���l(-1): LIMIT_m1_KURAI_BOMB_UKETUKE
		�e�ɂ��������ꍇ�A(�l��)���[�v
	ZERO(0):	LIMIT_0_ZERO			�ʏ펞(psp��ZERO���W�X�^�����邽��0��r�͑���)
		�ʏ펞							(�����L�[���͗L���B�{���ᑬ���L�[���́A�L���B�G�����蔻��L��B�e�����蔻��L��)
	ZERO(0):	LIMIT_0_ZERO			�ʏ펞(psp��ZERO���W�X�^�����邽��0��r�͑���)
	ITI(1): 	LIMIT_1_ONE 			�X�N���v�g���쎞(�C�x���g��)
		�������ɕt�����G�B				(�����L�[���͗L���B�{���ᑬ���L�[���́A�L���B�G�����薳���B�e�����薳���B)
	128?(128):	LIMIT_128				�v���C���[�������ԁB(�Œ�?)
		�������ɕt�����G�B				(�����L�[���͗L���B�{���ᑬ���L�[���́A�L���B�G�����薳���B�e�����薳���B)
	255:
		�Q�[���J�n���ɕt�����G�B		(�����L�[���͗L���B�{���ᑬ���L�[���́A�L���B�G�����薳���B�e�����薳���B)
	512:
---------------------------------------------------------*/
//void render_continue(void/*int now_max_continue*/);
/*static*/ int	now_max_continue;
static void player_move_special_ivent(SPRITE *s1)
{
	pds_status_timer--;
	#if 0
//	pds_status_timer = (signed int)(pds_status_timer & (0x1ff)); // ���炩�̒l�Ń}�X�N
	#else
	if (LIMIT_m255_CONTINUE > pds_status_timer) {	pds_status_timer += (512); }
//	if (-511 > pds_status_timer) {	pds_status_timer += 1024; }/*�f�o�b�O���ؗp*/
	#endif
//
	if (LIMIT_m127_MIN > pds_status_timer)
	{
		//�������̏���
		/* �g�債�Ă���k�� */
		#if (1==USE_ZOOM_XY)
		/* X �̂ݏk���BY �͕ύX�����B */
		if (t256(1.0) < s1->m_zoom_x256)
		{
			s1->m_zoom_x256 -= (5);
		//	s1->m_zoom_y256 -= (5);
		}
		else
		{
			s1->m_zoom_x256 = (t256(1.0));
		//	s1->m_zoom_y256 = (t256(1.0));
		}
		#else
		if (t256(1.0) < s1->m_zoom_xy256)
		{
			s1->m_zoom_xy256 -= (5);
		}
		else
		{
			s1->m_zoom_xy256 = (t256(1.0));
		}
		#endif
	}
	else
	if (LIMIT_m127_MIN == pds_status_timer)
	{
		//�Q�[���I�[�o�[�f��
//		case PLAYER_STATE_04_GAME_OUT:/* GAME_OUT�� */
//			pd.special_ivent_type	= 0;/*???*/
			now_max_continue--;
			if (
					(0 < now_max_continue)
				#if (0)/* (0==DE BUG) */
				/* �܂����� */
				&& ( (6) != player_now_stage)	/* �ŏI�ʂ̓R���e�B�j���[�o���Ȃ��B */
				#endif
				)
			{
				pd.use_continue++;	/* �R���e�B�j���[�� */
			//	render_continue(/*now_max_continue*/);
				main_call_func = ask_continue_menu_start;
			}
			else
			{
				main_call_func = gameover_start;
			}
//			break;
	}
	else
	if (LIMIT_m16_GAME_OUT > pds_status_timer)
	{
		;/* [�Ȃɂ����Ȃ�] */
		/* (�~�X�m�肵�Ă���)�Q�[���A�E�g�ɂȂ�܂ł̗P�\���� */
		/* (�����̃A�C�e������юU�鎞��) */
	}
	else
	if (LIMIT_m16_GAME_OUT == pds_status_timer) /* �v���C���[�A�K�b�c�������� */
	{
		/* ���݃I�v�V�������������āA�㔼�d���Ȃ�o�O������B */
		#if (0000)
		option[0]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* �I�v�V���������� */
		option[1]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* �I�v�V���������� */
		option[2]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* �I�v�V���������� */
		option[3]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* �I�v�V���������� */
		#endif/*0000*/
		pd.count_miss++;	/* �~�X�� */
		/* �c�`�����X���炷 */
		pd.zanki--;
		if (0 > pd.zanki)	/* �c�`�����X���Ȃ���� */
		{
			player_loop_quit();
		}
		else
		{
		/*	MAX(128)->����->(112)->����->(96)->����->(80)->����->(64)	(���16���ň��)	*/
			pd.weapon_power -= (16);	/* ���탌�x���̒ቺ(�g�A���ׂ����Փx�Ɋ֌W�Ȃ�P16��������) */
			if (0 > pd.weapon_power)	{	pd.weapon_power = 0;	}
			player_fukkatsu_bbb(s1);
			#if (1==USE_ZOOM_XY)
			s1->m_zoom_x256 = t256(1.0)+(120*5);/* �g�� */
			s1->m_zoom_y256 = t256(1.0);//+(120*5);/* �g�� */
			#else
			s1->m_zoom_xy256 = t256(1.0)+(120*5);/* �g�� */
			#endif
		}
	}
	else
	if (LIMIT_m1_KURAI_BOMB_UKETUKE > pds_status_timer)
	{
		;/* [�Ȃɂ����Ȃ�] */
		/* ��炢�{����t�̗P�\���� */
		//��炢�{����t���̏���
//		case PLAYER_STATE_01_KURAI_BOMB_UKETUKE:/* ��炢�{����t�� */			// [***090125		�ǉ�
//			/* ���S�m�� */
//			pd.state_flag |= STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL;		/* �L�[���͖���(1) */
//			break;
	}
	else
	/*
		(LIMIT_1_ONE-1)�͂܂� ���� �l(0) �����ǁA�Ӗ���(LIMIT_1_ONE-1)�Ƃ����Ӗ��B
		(LIMIT_1_ONE���� 1 ���������́A���� �l(-1) ��(player_move_special_ivent();�J�n����)pds_status_timer--;��␳���镪�B)
		(���ʓI��(���݂�)LIMIT_0_ZERO�Ɠ��������A�Ӗ���LIMIT_0_ZERO�Ɠ����ł͂Ȃ��B)
		LIMIT_1_ONE�͌Œ�l(1)�ł͂Ȃ��A���� �l(1) �����Ǐ����͔���Ȃ��B
		*/
	if ((LIMIT_1_ONE-1) == pds_status_timer)
	{
		/* ��������ʏ�ɖ߂� */
		s1->color32 		= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);					/*	s1->alpha			= 0xff;*/
		{
			SPRITE *zzz_obj_maru;
			zzz_obj_maru = &obj00[FIX_OBJ_01_JIKI_MARU];
			zzz_obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/* ���A */	/*	zzz_obj_maru->alpha = 0xff;*/
		}
	}
//	else
//	if ((LIMIT_128-1) == pds_status_timer)
//	{
//		case PLAYER_STATE_03_SAVE_02:/*�v���C���[�������Q*/
//			pd.special_ivent_type	= PLAYER_STATE_00_NORMAL;
//				s1->m_zoom_x256 			= t256(1.0)+100;	 /* �T�C�Y */
//			break;
//	}
}


/*---------------------------------------------------------
	�v���C���[�ړ��A���C������
	-------------------------------------------------------
	�{�X�Ń{�������G
	�{�����́A�{�X / ���U�R�G �ɂ������Ď���
	�X�N���v�g���쒆 �́A�G�����蔻��͂Ȃ�
	�L�[���͖�����(==������) �́A�G�����蔻��͂Ȃ�
	�{�����́A �{�X/�U�R�G �ɂ������Ď��ȂȂ�
	��炢�{����t�� �́A�G�����蔻��͂Ȃ�
---------------------------------------------------------*/

static void any_player_move(SPRITE *s1)
{
	pd.state_flag &= (~STATE_FLAG_15_KEY_SHOT); /* off */
	if (0!=(pd.state_flag & (STATE_FLAG_06_IS_SCRIPT)))
	{
		pds_status_timer = (LIMIT_1_ONE);	/* �X�N���v�g���쒆 �� 1 */
	}
	/* LIMIT_m16_GAME_OUT���傫���ꍇ�A�ړ��\�A�{�^���󂯕t���\ */
	if (LIMIT_m16_GAME_OUT < pds_status_timer)
	{
		player_keycontrol(s1);/* �ړ��{�{�^������ */
	}
	/* �ʏ펞 ���� ��O���̏����B(��O���̏����͕������Ƃ�) */
	if (0 == pds_status_timer)	/* �ʏ펞�̏ꍇ */
	{
		/* �v���C���[�ƁA�u�G�e�ɂ��O���C�Y�v/�u�G�e�ɂ�鎀�S�v/�u�G(�G�����{�X)�ɂ�鎀�S�v�̓����蔻�� */
		player_colision_check_all(s1);
	}
	else	/* ��O��(���ꏈ��)�̏ꍇ */
	{
		player_move_special_ivent(s1);	/* ���ꏈ�� */
	}
	/* �A�C�e���͏�Ɏ��� */
	player_colision_check_item(s1); 		/* �A�C�e���̓����蔻�� */
}

//		bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
//�p�~	if (0==(pd.state_flag & STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL))	/* �L�[���͗L��(0) */
//		if (0 <= pds_status_timer)	/* ���l�̏ꍇ�A�ړ��\ */

	#if 0/*�f�o�b�O*/
	if (0 <= pds_status_timer)	/* ���l�̏ꍇ */
	{
		p d_my_score	= pds_status_timer;
		p d_graze_point = 0;
	}
	else
	{
		p d_my_score	= -pds_status_timer;
		p d_graze_point = 11;
	}
	#endif


/*---------------------------------------------------------
	�v���C���[������()
---------------------------------------------------------*/
#if (1==USE_EXTEND_CHECK)
extern void player_init_extend_score(void);
#endif
//extern int zanki;
global void player_continue_value(void)
{
	pd.zanki				= (1+option_config[OPTION_CONFIG_00_PLAYER]);	/*pd.base_zanki*/	/*((zanki&0x03)+2)*/ /*player_fix_status[BASE_LIVES+select_player]*/
	pd.game_score			= score(0);
	pd.state_flag			&= (~STATE_FLAG_14_GAME_LOOP_QUIT); 	/* ���A */
	#if (1==USE_EXTEND_CHECK)
	player_init_extend_score();
	#endif
	player_fukkatsu_bbb(&obj00[FIX_OBJ_00_PLAYER]);/* �I�v�V������ǉ����� */

	/*
		[�R���e�B�j���[�X�^�[�g��]�y�сA[�V�K�X�^�[�g��]�̓��ʏ����B(�܂�Q�[����������[�ȊO��]�ꍇ)
		------------------------------------------------------------
		�����̏ꍇ�́A�I����ɖ��G�ɂȂ�A
		�������ɓ�����(�����L�[���͗L��...)�悤�ɍ׍H����B
		------------------------------------------------------------
		(�Q�[�����������̏ꍇ�͈�莞�ԓ����Ȃ��B)
	*/
	/* �g�嗦��1.0�{�ɂ���B */
//	s1->m_zoom_x256 = (t256(1.0));
	/* (���G���Ԃ�)�����ɓ�����B */
	pds_status_timer = (LIMIT_m255_CONTINUE);
}

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̒ǉ�
	�얲	������	�H�X�q ���~���A �� �`���m
---------------------------------------------------------*/

static void option_create(void) /* ���~���A �� �`���m [***090220 �ǉ� */
{
	SPRITE *zzz_player;
	zzz_player = &obj00[FIX_OBJ_00_PLAYER];
	unsigned int jj;
	for (jj=0; jj<4; jj++)
	{
		SPRITE *h;
		h						= sprite_add_direct(FIX_OBJ_04_JIKI_OPTION0/*+OPTION_C1*/+jj);	/* �K���o�^�ł���B  */
		h->jyumyou				= JYUMYOU_MUGEN;/* ���ԂŎ����������Ȃ� */
		{
		//	h->type 				= (JIKI_OPTION_00_00/*|S P_MUTEKI*/)+kk; kk += (16);/* �I�v�V�����C���^�[���[�u */ /*8*/
			h->type 				= (JIKI_OPTION_00_00/*|S P_MUTEKI*/)+(jj<<4); /* �I�v�V�����C���^�[���[�u */ /*8*/
			{
				static const /*int*/s16 aaa_tbl[4] =
				{
					cv1024r(360-(45*5)),	/*cv1024r(45*1)*/ /*1024*1/8*/ /*;*/ /*�`���m*/
					cv1024r(360-(45*7)),	/*cv1024r(45*5)*/ /*1024*5/8*/ /*;*/ /*�`���m*/
					cv1024r(360-(45*1)),	/*cv1024r(45*3)*/ /*1024*3/8*/ /*;*/ /*�`���m*/
					cv1024r(360-(45*3)) 	/*cv1024r(45*7)*/ /*1024*7/8*/ /*;*/ /*�`���m*/
				};
				h->PL_OPTION_DATA_angleCCW1024/*REMILIA_angle1024*/ = aaa_tbl[jj];
			}
				h->PL_OPTION_DATA_opt_anime_add_id		= (OPTION_C1+jj);

		//
			#if 1/*Gu(���S���W)*/
			h->cx256					= (zzz_player->cx256);
			h->cy256					= (zzz_player->cy256);
			#endif
			h->m_Hit256R			= TAMA_ATARI_BULLET_DUMMY;
		//	h->flags				|= (SP_FLAG_OPTION_VISIBLE);
			h->flags				&= (~(SP_FLAG_OPTION_VISIBLE)); 	/* ���t���O��OFF(�s��) */
			h->flags				&= (~(SP_FLAG_COLISION_CHECK)); 	/* �����蔻���OFF(���G) */

	/*???*/ h->base_weapon_strength 				= (1/*8*5*/);/* [***090214 �ǉ� */
			{
			//
				h->callback_mover		= player_move_option;
			//
			//	h->PL_OPTION_DATA_next					= obj00[FIX_OBJ_00_PLAYER];
			//	h->P L_OPTION_DATA_state2				= 0;
				h->PL_OPTION_DATA_offset_x256			= t256(0);
				h->PL_OPTION_DATA_offset_y256			= t256(0);
			//	h->PL_OPTION_DATA_ccc_angle512			= cv1024r((0));
				h->PL_OPTION_DATA_slow_count			= 0;
			}
			h->PL_OPTION_DATA_opt_shot_interval 	= 0;
		//	h->PL_OPTION_DATA_state 				= 0;
		//	h->PL_OPTION_DATA_state1				= 0;///
			h->PL_OPTION_DATA_anime_wait			= 0;
			/* ���~���A�̃I�v�V�����͔��������ۂ� */
			if (REMILIA==select_player)
			{
			//	h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x96);	/*	h->alpha			= 0x96; */	/* ����߂��� */
				h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x80);	/*	h->alpha			= 0x80;*/	/* ������ */
			//	h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	/*	h->alpha			= 0x50;*/	/* �Â�߂��� */
			}
		}
	}
}
/*---------------------------------------------------------
	�v���C���[������
---------------------------------------------------------*/
extern void sprite_initialize_position(SPRITE *h);
extern int boss_hp_dec_by_frame;/*�{�X�U�������l�A�t���[���P��*/
extern int practice_mode;
extern void select_jiki_load_surface(void);
extern void boss_effect_sprite_add(void);
extern void sprite_panel_init(void);
global void player_init(void)
{
	select_jiki_load_surface();
//
	sprite_all_cleanup();/* �Sobj���� */
//
	SPRITE *s1;
	s1					= sprite_add_direct(FIX_OBJ_00_PLAYER); 	/* �K���o�^�ł���B  */
	s1->jyumyou 		= JYUMYOU_MUGEN;/* ���ԂŎ����������Ȃ� */
	{
		static const s32 graze_atari[8] =
		{
			t256(16.0), 	/* �얲A */ 	//	BASE_CORE_ATARI_0a,/*6*/		/* �얲A */ 	/*	5 */	BASE_CORE_ATARI_0a,/*6*/
			t256(16.0), 	/* �얲B */ 	//	BASE_CORE_ATARI_0b,/*6*/		/* �얲B */ 	/*	5 */	BASE_CORE_ATARI_0a,/*6*/
			t256(32.0), 	/* ������A */	//	BASE_CORE_ATARI_0c,/*8*/		/* ������A */	/*	6 */	BASE_CORE_ATARI_0b,/*8*/
			t256(32.0), 	/* ������B */	//	BASE_CORE_ATARI_0d,/*8*/		/* ������B */	/*	6 */	BASE_CORE_ATARI_0b,/*8*/
			t256(16.0), 	/* ���~���A */	//	BASE_CORE_ATARI_0e,/*8*/		/* ������C */	/*	7 */	BASE_CORE_ATARI_0c,/*12*/
			t256(48.0), 	/* �H�X�q */	//	BASE_CORE_ATARI_0f,/*12*/		/* ���~���A */	/*	9 */	BASE_CORE_ATARI_0e,/*8*/
			t256(48.0), 	/* �`���mA */	//	BASE_CORE_ATARI_0g,/*16*/		/* �`���m */	/*	8 */	BASE_CORE_ATARI_0d,/*16*/
			t256(48.0), 	/* �`���mQ */	//	BASE_CORE_ATARI_0h,/*8*/		/* �H�X�q */	/*	6 */	BASE_CORE_ATARI_0b,/*8*/
		};
	//	s1->m_Hit256R		= JIKI_ATARI_ITEM_16;/* t256(16.0); �O���C�Y���� */
		s1->m_Hit256R		= ((graze_atari[select_player]));/* �O���C�Y���� */
	}
	s1->type			= (SP_GROUP_JIKI_GET_ITEM);
	s1->flags			|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	s1->callback_mover	= any_player_move;
	#if 1/*Gu(���S���W)*/
	s1->cx256			= (t256(GAME_WIDTH/2));
	s1->cy256			= (t256(GAME_HEIGHT));
	#endif
//
	/* �v���C���[�A��������p�R�A(obj_maru == ��)�̒ǉ� */

	SPRITE *zzz_obj_maru;
	zzz_obj_maru				= sprite_add_direct(FIX_OBJ_01_JIKI_MARU);	/* �K���o�^�ł���B  */
	zzz_obj_maru->jyumyou		= JYUMYOU_MUGEN;/* ���ԂŎ����������Ȃ� */
//	zzz_obj_maru->type			= (JIKI_CORE_00_REIMU_A+select_player);
//	zzz_obj_maru->type			= (SPELL_SQUERE_);
	zzz_obj_maru->type			= (SP_DUMMY_MUTEKI);/*(S P_GROUP_ETC_DUMMY_REMILIA);*/
	{
		static const s32 base_core_atari[8] =
		{
			t256(0.75), 	/* �얲A */ 	//	BASE_CORE_ATARI_0a,/*6*/		/* �얲A */ 	/*	5 */	BASE_CORE_ATARI_0a,/*6*/
			t256(0.75), 	/* �얲B */ 	//	BASE_CORE_ATARI_0b,/*6*/		/* �얲B */ 	/*	5 */	BASE_CORE_ATARI_0a,/*6*/
			t256(1.50), 	/* ������A */	//	BASE_CORE_ATARI_0c,/*8*/		/* ������A */	/*	6 */	BASE_CORE_ATARI_0b,/*8*/
			t256(1.50), 	/* ������B */	//	BASE_CORE_ATARI_0d,/*8*/		/* ������B */	/*	6 */	BASE_CORE_ATARI_0b,/*8*/
			t256(1.75), 	/* ���~���A */	//	BASE_CORE_ATARI_0e,/*8*/		/* ������C */	/*	7 */	BASE_CORE_ATARI_0c,/*12*/
			t256(2.00), 	/* �H�X�q */	//	BASE_CORE_ATARI_0f,/*12*/		/* ���~���A */	/*	9 */	BASE_CORE_ATARI_0e,/*8*/
			t256(0.90), 	/* �`���mA */	//	BASE_CORE_ATARI_0g,/*16*/		/* �`���m */	/*	8 */	BASE_CORE_ATARI_0d,/*16*/
			t256(0.25), 	/* �`���mQ */	//	BASE_CORE_ATARI_0h,/*8*/		/* �H�X�q */	/*	6 */	BASE_CORE_ATARI_0b,/*8*/
		};
		//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* �����o�^ */
		zzz_obj_maru->m_Hit256R 		= ((base_core_atari[select_player]));
	}

//
//	#if (0==US E_BOSS_COMMON_MALLOC)
//	obj00[FIX_OBJ_08_BOSS]				= NULL;
//	#else
//----[BOSS]
	/* �����蔻��̓s���� */
	{	SPRITE *h;
		h								= sprite_add_gu_error();	/* �Gobj�ԍ���0�Ԃ�K���擾�ł���B */
	//	h	�����蔻��̓s����ł��Ȃ�							= sprite_add_direct(FIX_OBJ_08_BOSS);	/* �K���o�^�ł���B  */
	//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* �����o�^ */		/* �d�l�o�O(?) */
	//	h = &obj00[FIX_OBJ_08_BOSS];
		sprite_initialize_position(h);
		h->jyumyou				= JYUMYOU_MUGEN;/* ���ԂŎ����������Ȃ� */
		global_obj_boss 			= h;
	}
//	#endif
	{
		boss_effect_sprite_add();
		boss_effect_initialize_position();
	}

//
	pd.use_continue 		= 0;	/* �R���e�B�j���[�� */
	pd.count_miss			= 0;	/* �~�X�� */
	pd.used_bomber			= 0;	/* �{���g�p�� */
	pd.use_kurai_bomb		= 0;	/* ��炢�{���g�p�� */
//	pd.count_bonus			= 0;	/* �X�y���J�[�h�{�[�i�X�� */
//
	pd.bomber_time			= 0;	/*==bomb_wait*/
	pd.graze_point			= 0;
//
	pd.state_flag			= STATE_FLAG_00_NONE;
//	if (MARISA==select_player)		{	pd.state_flag		|= STATE_FLAG_04_IS_MARISA; 	}	/* �������͏�Ɏ������W */
//


//
	boss_hp_dec_by_frame	= 0;/*�{�X�U�������l�A�t���[���P��*/
//
	weapon_level_offset 	= select_player + (/*0==���탌�x��*/0<<3);
	/* ���K���[�h�̏ꍇ�̓t���p���[�Ŏn�߂�(���̑���N���A��̃C�x���g������Ȃ�) */
	pd.weapon_power 		= (0==practice_mode)?(0):(MAX_POWER_IS_128);
//
	/* �I�v�V������ǉ� */
	option_create();
//
	now_max_continue		= DEFAULT_MAX_CONTINUE;/*(3)*/
//
	#if 1
	player_continue_value();
	#else
//	p d_zanki				= /*pd.base_zanki*/ player_fix_status[BASE_LIVES+select_player];
//	p d_my_score			= score(0);
//	player_fukkatsu_bbb(obj00[FIX_OBJ_00_PLAYER]);/* �I�v�V������ǉ����� */
	#endif
//�ǂݍ��ݖ��G�ōĐݒ肳���̂Ŗ��Ӗ�//	pds_status_timer	= (LIMIT_512);					// �����o�ꎞ�A���G����
	#if (1==USE_ZOOM_XY)
	s1->m_zoom_x256 = t256(1.0);/*	�����o�ꎞ�A�g�嗦 x 1.0[�{] */
	s1->m_zoom_y256 = t256(1.0);/*	�����o�ꎞ�A�g�嗦 x 1.0[�{] */
	#else
	s1->m_zoom_xy256 = t256(1.0);/* �����o�ꎞ�A�g�嗦 x 1.0[�{] */
	#endif
	sprite_panel_init();
}


/*---------------------------------------------------------
	�G����
	�{�X�|�����ꍇ�̏���
	(�v���C���[���܂�)
	-------------------------------------------------------
???��R�����g???	�{�X��|�����炷���Ă΂��(�v���C���[�𖳓G�ɂ����)
???��R�����g???	(�t���O�͎d�l�㎞�ԑ҂�������ׁA���݂��̗p�r�ɂ͎g���Ȃ�)
---------------------------------------------------------*/
global void set_clear_pdc_special_ivent(void)
{
	#if (0)
//	pds_status_timer	= /*pd.bomber_time*/(255) + USER_BOMOUT_WAIT;/*(30)*/	/*�������G����*/
	#else
//	pds_status_timer	= (6);/* 6[�t���[��] �{�X�|���Ă��玟(�V�i���I)�ɐi�ނ܂ł̑҂����� */
//	pds_status_timer	= (60);/* 60[�t���[��] �{�X�|���Ă���A�{�X����ʊO�ɑޔ�����܂ł̑҂����� */
//	pds_status_timer	= (9999);/* �ޔ���ɑޔ���Ŏw��B	60[�t���[��] �{�X�|���Ă���A�{�X����ʊO�ɑޔ�����܂ł̑҂����� */
//	pds_status_timer	= (600);/* �ޔ���ɑޔ���Ŏw��B	60[�t���[��] �{�X�|���Ă���A�{�X����ʊO�ɑޔ�����܂ł̑҂����� */
	#endif
//	pds_status_timer	= 40/*120*/ /*150-120*/;									// ���G����
//	pd.special_ivent_type	= PLAYER_STATE_02_SAVE_01;	// ���G��ԁH
//	pd.special_ivent_type	= PLAYER_STATE_03_SAVE_02;	/* �H�ɁA���܂������Ȃ� */
//	pds_status_timer	= (LIMIT_m127_MIN); 				// ���G���� 		/*120*/ /*150-120*/
	pds_status_timer	= (LIMIT_65535);					// ���G���� 		/*120*/ /*150-120*/
}


/*---------------------------------------------------------
	���[�U�[
---------------------------------------------------------*/
global void add_laser_off(SPRITE *src)
{
	pd.laser_mode = (0);/* off */
}
global void add_laser_on(SPRITE *src)
{
	pd.laser_mode = (1);/* on */
}
