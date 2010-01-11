/*
�y���zPSP�Œe��STG��肽���z��肽���z15�y���z
722 �F231�F2009/09/08(��) 23:17:49 ID:qnDIO3of
�͕핗�̗\��ɂ���
�X�y�J�͎����\�肪����܂��B�͕핗��������z�肵�Ă���X�y�J�V�X�e���͈ȉ��̂悤�Ȃ��̂ł��B

�{�X �� spell_card.c �� danmaku.c �� bullet_object.c �Ƃ����o�H���l���Ă��܂��B
���̂悤�ɕ����Ă���̂́A���݂܂�����܂��񂪁A
���U�R �� danmaku.c �� bullet_object.c �Ƃ����o�H���l���Ă��邩��ł��B

spell_card.c �̓X�y�J���Ǘ�����V�X�e���ł��B

�{�X���X�y�J�����Ă���̗͂�����ꍇ�B�X�y�J�ɓo�^���ꂽ���̗͂ƈ��������ɃX�y�J�������܂��B
�X�y�J�����Ă�`�����X�������Ă��A�X�y�J�ɓo�^���ꂽ���̗͂ɖ����Ȃ��ꍇ�̓X�y�J�������܂���B(���Ă܂���)

�{�X���X�y�J�����Ɠ����Ƀ{�X�̗̑͂���X�y�J����������āA�X�y�J���[�h�ɂȂ�܂��B
�X�y�J���[�h���Ƀ{�X���U������ƃ{�X�̗̑͂͌��炳�Ȃ��ŁA�X�y�J(spell_card.c)���Ǘ����Ă���
�X�y�J���̗̑�(�X�y�J�Q�[�W���\��)�����炵�܂��B
�X�y�J���[�h���ɃX�y�J�ɓo�^���ꂽ��莞�Ԃ��o�߂��邩�A�X�y�J(spell_card.c)���Ǘ����Ă���
�X�y�J���̗̑�(�X�y�J�Q�[�W)�������Ȃ�ƒʏ탂�[�h�ɕ��A���܂��B
���̂Ƃ��X�y�J���̗̑�(�X�y�J�Q�[�W)�͋����I�� 0 �ɂȂ�܂��B

���̂悤�ȃX�y�J�V�X�e����z�肵�Ă���ׁA�X�y�J�V�X�e���ɑ̗͂����炳����悤�Ƀ{�X������������K�v������܂��B
�X�y�J�V�X�e�������̂����̂ƃ{�X�̉����ɂ͎��Ԃ��|����(���Ԃ񎟉�(�㌎��2009-09-27�\��)�ɂ͊Ԃɍ����܂���)�̂�
�C���ɂ��҂����������B

��L�ȊO�̈Ă̎����\��͂���܂��񂪁A�����Ɨǂ��Ă�����΂����ɏ����Ă�����
�Q�l�ɂ��܂����A�ǂ��ĂȂ�Ύ������邩������܂���B

*/

#include "bullet_object.h"
#include "scenario_script.h"

/*---------------------------------------------------------
	�X�y�J�Ǘ��V�X�e��
	-------------------------------------------------------
	���X�y�J�̓V�X�e���ŃT�|�[�g����\�z������܂��B(�\�z��)
	�V�X�e���ŃT�|�[�g�����ƁA�ǂ̃{�X���A�ǂ̃X�y�J�ł����Ă�悤�ɂȂ�܂��B
	-------------------------------------------------------
	ToDo:
	���U���̊Ǘ�
	������̊Ǘ�(���\����)
---------------------------------------------------------*/
// ��� 6��
extern void sakuya_11_keitai(SPRITE *src);
extern void sakuya_10_keitai(SPRITE *src);
extern void sakuya_09_keitai(SPRITE *src);
extern void sakuya_06_keitai(SPRITE *src);
extern void sakuya_04_keitai(SPRITE *src);
extern void sakuya_03_keitai(SPRITE *src);
extern void sakuya_02_keitai(SPRITE *src);
extern void sakuya_01_keitai(SPRITE *src);

// �p�`�F 5��
//tern void pache_05_keitai(SPRITE *src);
extern void pache_04_keitai(SPRITE *src);
//tern void pache_03_keitai(SPRITE *src);
//tern void pache_02_keitai(SPRITE *src);
extern void pache_01_keitai(SPRITE *src);

// �P�� 4��
//tern void kaguya_06_keitai(SPRITE *src);
//tern void kaguya_05_keitai(SPRITE *src);
extern void kaguya_04_keitai(SPRITE *src);
//tern void kaguya_03_keitai(SPRITE *src);
//tern void kaguya_02_keitai(SPRITE *src);
extern void kaguya_01_keitai(SPRITE *src);

// ���� 3��
extern void mitei_01_keitai(SPRITE *src);

// �� 2��
extern void aya_05_keitai(SPRITE *src);
extern void aya_04_keitai(SPRITE *src);
//tern void aya_03_keitai(SPRITE *src);
extern void aya_02_keitai(SPRITE *src);
extern void aya_01_keitai(SPRITE *src);

// �A���X 1��
extern void alice_01_keitai(SPRITE *src);

//------------ �X�y�J�֘A
/*extern*/ int spell_card_limit_health; 	/* �K��l�ȉ��ɂȂ�΃X�y�J���[�h���� */
/*extern*/ int spell_card_boss_state;		/* ���l�ɂȂ�΃{�X�X�y�J���[�h�ɓ���Ȃ� */
/*extern*/ int spell_card_mode; 			/* �X�y�J���[�h */
/*extern*/ int spell_card_boss_timer;		/* ���p */	// ��������

/*extern*/ int spell_card_number;			/* ���p */	// �X�y�J�ԍ�
/*extern*/ int spell_card_max;				/* ���p */	// �X�y�J�ԍ��ő���E�l

typedef struct
{
	int spell_life; 	/* �X�y�J�ɓo�^���ꂽ���̗� */
	int spell_time; 	/* �X�y�J�ɓo�^���ꂽ��莞�� */
	void (*spell_move_callback)(SPRITE *sss);	/* �ړ����� */
	const char *spell_str_name; 	/* �X�y�J���� */
} SPELL_CARD_RESOURCE;

//	{	 500,	  20,		"�X���u�J����000�v" },/*NULL*/		/* �X�y�J�������Ȃ� */
	/* ���O�̓e�L�g�[�ł� */

/* �X�y������(==�P��[�t���[��])�́A64�{(==�P��[��1�b])���K�i */
#define s_time(aaa) ((aaa)<<6)

/*static*/ SPELL_CARD_RESOURCE my_111resource[SPELL_CARD_MAX] =
{
// ��� 6��
	{	 500,	  s_time(160),	common_00_keitai,	"�`�ԕύX"							"\n"	},/* 500 160 ����̍U����(�蓮�ݒ�) */
	{	 500,	  s_time(20),	sakuya_01_keitai,	"�ʏ�U��"							"\n"	},		/*	"���`��: ���E�ɓ����ď��e����(�ʏ�U��)"		*/
	{	 500,	  s_time(20),	sakuya_02_keitai,	"��p�u�~�X�f�B���N�V�����v"		"\n"	},		/*	"���`��: �S���ʁA���܂�(�~�X�f�B���N�V����)"	*/
	{	 500,	  s_time(20),	sakuya_03_keitai,	"���J�u�o�[�e�B�J���i�C�t�v"		"\n"	},		/*	"��O�`��: �����i�C�t�������Ă����"			*/
	{	 500,	  s_time(20),	sakuya_04_keitai,	"�����u�A�����~�e�b�h���C�h�v"		"\n"	},		/*	"��l�`��: �����w����"							*/
	{	2500,	  s_time(20),	sakuya_06_keitai,	"����u�f���W�����X���[���h�v"		"\n"	},		/*	"��܌`��: (���F�}�X�J�b�g�e)"					*/
//	{	 500,	  s_time(20),	common_00_keitai,	"��p�u���e�X�g007�v" 			"\n"	},		/*	"�掵�`��: (���U�����w)�ǉ��v�撆"				*/
//	{	 500,	  s_time(20),	common_00_keitai,	"��p�u���e�X�g008�v" 			"\n"	},		/*	"�攪�`��: (���~�߃i�C�t)�ǉ��v�撆"			*/
	{	2500,	  s_time(20),	sakuya_09_keitai,	"�����u�t�F�X�e�B�o���i�C�t�v"		"\n"	},		/*	"���`��: �ŏI�`��(����1)" 					*/
	{	1500,	  s_time(20),	sakuya_10_keitai,	"���C�h��Z�u�t�@�C�i���^�C�v�v"	"\n"	},		/*	"��10�`��: �ŏI�`��(����2)" 					*/
	{	9000,	  s_time(20),	sakuya_11_keitai,	"���C�h��Z�u�t�@�C�i�����v"		"\n"	},		/*	"��11�`��: �ŏI�`��(����3)" 					*/
// �p�`�F 5��
	{	 500,	  s_time(120),	common_00_keitai,	"�`�ԕύX"							"\n"	},/* 3069 120 ����̍U����(�蓮�ݒ�) */
	{	 500,	  s_time(20),	pache_01_keitai,	"�ʏ�U��"							"\n"	},
	{	 500,	  s_time(40),	pache_01_keitai,	"�Ε��u�p�`�����[���쐬000�v"		"\n"	},
	{	 500,	  s_time(20),	pache_01_keitai,	"�����u�p�`�����[���쐬001�v"		"\n"	},
	{	 500,	  s_time(40),	pache_04_keitai,	"�ؕ��u�V���t�B�[���쐬002�v"		"\n"	},
	{	 500,	  s_time(40),	pache_04_keitai,	"�����u�p�`�����[���쐬003�v"		"\n"	},
	{	 500,	  s_time(40),	pache_04_keitai,	"�y���u�p�`�����[���쐬004�v"		"\n"	},
	{	 500,	  s_time(40),	pache_04_keitai,	"�����u�p�`�����[���쐬005�v"		"\n"	},
//	{	 500,	  s_time(40),	pache_04_keitai,	"�����u�T�C�����g���쐬006�v"		"\n"	},
//	{	 500,	  s_time(40),	pache_04_keitai,	"�X���u�p�`�����[���쐬007�v"		"\n"	},
// �P�� 4��
	{	 500,	  s_time(99),	common_00_keitai,	"�`�ԕύX"							"\n"	},/* 500 99 ����̍U����(�蓮�ݒ�) */
	{	 500,	  s_time(20),	kaguya_01_keitai,	"�ʏ�U��"							"\n"	},
	{	 500,	  s_time(40),	kaguya_01_keitai,	"���u���쐬001�v" 				"\n"	},
	{	 500,	  s_time(40),	kaguya_01_keitai,	"���u���쐬002�v" 				"\n"	},
	{	 700,	  s_time(40),	kaguya_04_keitai,	"���u���쐬003�v" 				"\n"	},
	{	 700,	  s_time(40),	kaguya_04_keitai,	"���u���쐬004�v" 				"\n"	},
	{	 700,	  s_time(40),	kaguya_04_keitai,	"���u���쐬005�v" 				"\n"	},
	{	 700,	  s_time(40),	kaguya_04_keitai,	"���u���쐬006�v" 				"\n"	},
// ���� 3��
	{	 500,	  s_time(40),	common_00_keitai,	"�`�ԕύX"							"\n"	},/* 500 40 ����̍U����(�蓮�ݒ�) */
	{	 500,	  s_time(20),	mitei_01_keitai,	"�ʏ�U��"							"\n"	},
	{	 500,	  s_time(20),	mitei_01_keitai,	"����u���쐬�X�y�J001�v"			"\n"	},
	{	 500,	  s_time(40),	mitei_01_keitai,	"����u���쐬�X�y�J002�v"			"\n"	},
	{	 500,	  s_time(40),	mitei_01_keitai,	"����u���쐬�X�y�J003�v"			"\n"	},
	{	 500,	  s_time(40),	mitei_01_keitai,	"����u���쐬�X�y�J004�v"			"\n"	},
	{	 500,	  s_time(40),	mitei_01_keitai,	"����u���쐬�X�y�J005�v"			"\n"	},
	{	 500,	  s_time(40),	mitei_01_keitai,	"����u���쐬�X�y�J006�v"			"\n"	},
// �� 2��
	{	 500,	  s_time(120),	common_00_keitai,	"�`�ԕύX"							"\n"	},/* 500 120 ����̍U����(�蓮�ݒ�) */
	{	 500,	  s_time(20),	aya_01_keitai,		"�ʏ�U��"							"\n"	},/*4*/
	{	1500,	  s_time(40),	aya_02_keitai,		"�ؕ��u�F�؈�ࣁv�̃e�X�g"			"\n"	},/*4*/
	{	 500,	  s_time(20),	aya_04_keitai,		"�򕄁u���쐬�X�y�J001�v"			"\n"	},/*5*/
	{	1000,	  s_time(40),	aya_04_keitai,		"�򕄁u���쐬�X�y�J002�v"			"\n"	},/*4*/
	{	1000,	  s_time(40),	aya_05_keitai,		"�򕄁u���쐬�X�y�J003�v"			"\n"	},/*5*/
	{	1000,	  s_time(40),	aya_04_keitai,		"�򕄁u���쐬�X�y�J004�v"			"\n"	},/*4*/
	{	1000,	  s_time(40),	aya_05_keitai,		"�򕄁u���쐬�X�y�J005�v"			"\n"	},/*5*/
	{	1000,	  s_time(40),	aya_05_keitai,		"�򕄁u���쐬�X�y�J006�v"			"\n"	},/*5*/
	{	1000,	  s_time(40),	aya_05_keitai,		"�򕄁u���쐬�X�y�J007�v"			"\n"	},/*5*/
// �A���X 1��
	{	1023,	  s_time(75),	common_00_keitai,	"�`�ԕύX"							"\n"	},/* 1023 75 ����̍U����(�蓮�ݒ�) */
	{	 500,	  s_time(20),	alice_01_keitai,	"�ʏ�U��"							"\n"	},
	{	 500,	  s_time(40),	alice_01_keitai,	"�����u���쐬�̕������l�`000�v" 	"\n"	},
	{	 500,	  s_time(20),	alice_01_keitai,	"�����u���쐬�̈����ɐl�`001�v" 	"\n"	},
	{	 500,	  s_time(40),	alice_01_keitai,	"�����u���쐬�̈ɑ������l�`002�v"	"\n"	},
	{	 500,	  s_time(40),	alice_01_keitai,	"�����u���쐬�̉p�g���l�`003�v" 	"\n"	},
	{	 500,	  s_time(40),	alice_01_keitai,	"�����u���쐬�̘I�����l�`004�v" 	"\n"	},
	{	 500,	  s_time(40),	alice_01_keitai,	"�����u���쐬�̐����l�`005�v"		"\n"	},
	{	 500,	  s_time(40),	alice_01_keitai,	"�����u���쐬�̔����`�l�`006�v" 	"\n"	},
	{	 500,	  s_time(40),	alice_01_keitai,	"�����u���쐬�̔g���l�`007�v"		"\n"	},
//
};

/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]	�X�y�J����
---------------------------------------------------------*/

/*static*/ void spell_card_generator222(SPRITE *src)
{
	(my_111resource[(spell_card_number)].spell_move_callback)(src);
	/*---------------------------------------------------------
		�X�y�J�`�F�b�N
	---------------------------------------------------------*/
	{
		if (0/*off*/==spell_card_mode)
		{
			if (0/*off*/==spell_card_boss_timer)
			{
				spell_card_limit_health = 0;
				spell_card_mode 		= 1/*on*/;
			}
		}
	}
}

/*---------------------------------------------------------
	�X�y�J���̕\��
---------------------------------------------------------*/

void draw_spell_card_name(void)
{
	script_message_window_clear();/*�X�N���v�g���b�Z�[�W��ʂ�����*/
	msg_time = (60*5);
	print_kanji000(/*SDL_Surface *drawmap,*/ /*SDL_Rect *rect*/ /*0,*/
		my_111resource[spell_card_number].spell_str_name, //	aaa_str[(spell_card_number&15)],
		/*int color_type*/7, /*int wait*/0
	);
}


/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]	�X�y�J���j��A�C�e���o��
---------------------------------------------------------*/
//	#define alice_put_items common_boss_put_items
//	#define mitei_put_items common_boss_put_items
//	#define kaguya_put_items common_boss_put_items
//	#define pache_put_items common_boss_put_items

/*---------------------------------------------------------

---------------------------------------------------------*/
#if 0
/*extern*/ void create_spell_card(SPRITE *src, int spell_card_type)
{
//	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
//
	int aaa;	/* ���ݑ̗� ���� ���������X�y�J�� �������̗͒l */
	aaa = ((/*data->boss_base.boss*/src->base_health) - (my_111resource[spell_card_type].spell_life));
	/* �{�X���X�y�J�����Ă���̗͂�����ꍇ */
	if ( 0 < aaa )
	{
		spell_card_limit_health = aaa;
		spell_card_mode 		= 1/*on*/;
	}
}
#endif

/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]	�X�y�J�o�^
---------------------------------------------------------*/

/*static*/ void regist_spell_card222(SPRITE *src)
{
//	spell_card_limit_health -= 1000/*500*/;
	spell_card_limit_health -= (my_111resource[spell_card_number+1].spell_life);	/*1000 500*/
	if (0 >= spell_card_limit_health)
	{
		spell_card_limit_health = 0;
		spell_card_mode 		= 0/*off*/;
	}
	else
	{
	//	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
		spell_card_mode 		= 1/*on*/;
		spell_card_number++;
		if (spell_card_max < spell_card_number)
		{
			/* �`�ԕύX���Ȃ��A�A�C�e���f���Ȃ� */
			spell_card_number--;
		//	src->callback_loser 			= lose_boss;
		}
		else
		{
			/* �A�C�e���f�� */
			if (NULL != src->callback_loser)
			{
				(src->callback_loser)(src); 	/* sakuya_put_items(src); */
			}
		}
	}
}
#if 0

/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]	�X�y�J�o�^
---------------------------------------------------------*/

static void regist_spell_card(SPRITE *src)
{
	spell_card_limit_health -= 1000/*500*/;
	if (0 >= spell_card_limit_health)
	{
		spell_card_limit_health = 0;
		spell_card_mode 		= 0/*off*/;
	}
	else
	{
	//	BOSS02_DATA *data = (BOSS02_DATA *)src->data;
	//	if (b_health_alter_low1024 < (data->boss_base.boss_health & (1024-1)))/* �`�ԕύX������ */
		spell_card_mode 		= 1/*on*/;
		spell_card_number++;
		if (spell_card_max < spell_card_number)
		{
			spell_card_number--;
		}
//		if (0==sakuya->base_health)
		{
//			if (0 != (data->boss_base.boss_life))/* �`�ԕύX������ */
			{
//				data->boss_base.boss_life--;
				(src->callback_loser)(src);
			}
		}
	}
}

#endif

/*---------------------------------------------------------
	�{�X�ړ������̋��ʃ��[�`��
	-------------------------------------------------------
�� �ړ��͈͂̓o�^�A�@�\�F
	���炩���߃{�X�ړ��͈͂�o�^���Ă����B
�� �͈͓��Ȃ�Έړ��A�@�\�F
	�ړ��͈͓��Ȃ�΁A�{�X���ړ�����B
	�ړ��͈͊O�Ȃ�΁A�{�X�͈ړ����Ȃ��B
�� �ړ�����A�@�\�F
	�{�X���ړ����Ȃ��ꍇ�́A�u�ړ��ł��Ȃ������t���O�v���n�m�ɂȂ�B
---------------------------------------------------------*/
/*extern*/ int boss_hamidasi;			/* �u�ړ��ł��Ȃ������t���O�v(�g�p�O�Ɏ蓮��OFF==0�ɂ��Ƃ�) */
/*extern*/ POINT256 boss_clip_min;		/* �{�X�ړ��͈�(�ŏ��l) */
/*extern*/ POINT256 boss_clip_max;		/* �{�X�ړ��͈�(�ő�l) */
void boss_move_clip_rect(SPRITE *src)
{
	src->x256 += (src->vx256);
		 if (src->x256 < boss_clip_min.x256 )	{	src->x256 = boss_clip_min.x256; 	boss_hamidasi=1;	}
	else if (src->x256 > boss_clip_max.x256 )	{	src->x256 = boss_clip_max.x256; 	boss_hamidasi=1;	}
//
	src->y256 += (src->vy256);
		 if (src->y256 < boss_clip_min.y256 )	{	src->y256 = boss_clip_min.y256; 	boss_hamidasi=1;	}
	else if (src->y256 > boss_clip_max.y256 )	{	src->y256 = boss_clip_max.y256; 	boss_hamidasi=1;	}
}

/*---------------------------------------------------------
	�{�X�̋��ʁA�P��ڏ��������[�`��(����A�U���s��)
---------------------------------------------------------*/

/*extern*/ void spell_card_boss_init_regist(SPRITE *src)
{
	((PLAYER_DATA *)player->data)->boss = src;/*�P��{�l*/
//
	src->x256						= BOSS_XP256/*t256(GAME_WIDTH/2)-(sakuya->w128)*/;
	src->y256						= t256(-100);
//
	src->callback_hit_enemy 		= NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
//
	/* spell_card common init */
//	{
//	//	spell_card_limit_health 	= 0;
//	//	spell_card_mode 			= 0/*off*/;
//	}
	spell_card_boss_timer	= (((my_111resource[spell_card_number].spell_time)));		/* 75*64==75[count] 	��99[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
	spell_card_limit_health = (src->base_health) - (my_111resource[spell_card_number].spell_life);/* 500 1000 �ʏ�U��(����U��)�̍U����(�蓮�ݒ�) */
	spell_card_mode 		= 1/*on*/;	/* ����H */
	/* boss_rect_init */
	boss_clip_min.x256	= t256( 		0)+t256(24);
	boss_clip_max.x256	= t256(GAME_WIDTH)-t256(24);
	boss_clip_min.y256	= t256(0);
	boss_clip_max.y256	= t256(96);
	boss_effect_init();
}

/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]
---------------------------------------------------------*/

/*static*/ void common_00_keitai(SPRITE *src)
{
	if (t256(15/*40*/ /*30*/) > src->y256)	/* ���ֈړ� */
	{
		src->y256 += t256(1/*2*/);	/**fps_fa ctor*/
	}
	else
	if (t256(16/*40*/ /*30*/) > src->y256)	/* ���ֈړ� */
	{
		src->y256 += t256(1/*2*/);	/**fps_fa ctor*/
		((PLAYER_DATA *)player->data)->state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
	}
	else	/* ��b�I����҂� */
	{
		if ( ((((PLAYER_DATA *)player->data)->state_flag) & STATE_FLAG_05_IS_BOSS) )	/* ��b�I��? */
		{
		//	common_boss_init_2nd(src);	/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
			/*---------------------------------------------------------
				�{�X�̋��ʁA�Q��ڏ��������[�`��(�U���\)
			---------------------------------------------------------*/
		//	/*extern*/static void common_boss_init_2nd(SPRITE *src)
			{
				/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
				src->callback_hit_enemy = callback_hit_boss;	/* �R�[���o�b�N�o�^ */
				/* spell_card common init */
				spell_card_mode 		= 1/*on*/;
			}
		//	/*���Ԑ����J�E���g�L����*/
		//	data->state1++/* = ST_02*/;
			spell_card_number++;
		}
	}
}

