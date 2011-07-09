
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _PLAYER_H_
#define _PLAYER_H_

// > �yPSP�zPSP�œ�����肽���z8�y�e���z
// > 200 �F���������񁗂��������ς��B�F2009/01/27(��) 18:41:00 ID:xQgI7hCU
// > ���낻��S�A�C�e�������ė~�����ȁB
// > ����FULL�p���[���̃A�C�e�������񂹃��C���͂���������Ɖ��̕��������Ǝv���B
// > ��̓I�ɂ͂��̕ӂ聫
// > ttp://www4.uploader.jp/user/isisuke/images/isisuke_uljp00081.bmp
// ���̈ӌ��͂킽�������l�Ɏv�����̂ŁA�݂�Ȃ������v�����ɈႢ�Ȃ��B
// ���̉摜��484x283[dots]�ŁAy==63[line]�ɐԐ��������Ă������B
// psp�̉𑜓x�́A 480x272[dots]�Ȃ̂� x/272=63/283, x=(63*272)/283 == 60.55[line]
// ����/���쎁	50(49)[line] �����シ����B
// �X��8��200�� ��60[line] (���o�I��)
// �����ŃL�����̍����������邵�A64(63)�������Ɖ�72(71)���炢�ł������̂ł́H�Ƃ������64�ɂ��Ď������Ă݂�B
// ���ʁF����ς����������������B8[dots]�Ƃ��킸16[dots]���炢���������B�Ăю������Ă݂�B
// ���ʁF����Ȃ��񂩂ȁH�Ƃ����킯��80(79)[dots]�Ɍ���B����8[dots]��88(87)�ł����������ˁB
// �{�Ƃ̊��o��厖�ɂ���Ȃ炱��Ȃ��񂾂Ǝv���B
//---------------
//�u�A�C�e�������񂹃��C���v�ȉ��Ŏ��Ɓu�A�C�e�������񂹃��C���v����322���C���܂ŋ����Ō��_�B
//
//�v���C�t�B�[���h�� 384x448 �Ȃ̂ŁA448-322 == 126
//�u�A�C�e�������񂹃��C���v�͑����ォ��128�h�b�g���ȁH
//
//(B�A�C�e���̑傫����16x16[dot]�Ȃ̂ŁAB�A�C�e���̏�������� 448+(16/2)���C���ōs������A
//�Ō��B�A�C�e���̔��肪����̂��AB�A�C�e���̂x�����S��456[���C��]�ɂ����ꍇ�B
//456-322 == 134, 134-128 == 6. ����6[���C��]�͂����������@���S����ʉ����ɍs���Ȃ��������h�b�g(5[dot?])����B
//�c��1[dot]�͕s�����Ŕ��肵������? )
//---------------
// �������䗦�Ȃ�A
// (�͕핗�A�C�e�������񂹃��C��)/(�{�ƃA�C�e�������񂹃��C��) == (�͕핗�Q�[���t�B�[���h�c)/(�{�ƃQ�[���t�B�[���h�c)
// ����������B(�͕핗�A�C�e�������񂹃��C��)���m�肽���̂�����ڍs���āA
// (�͕핗�A�C�e�������񂹃��C��) == (�{�ƃA�C�e�������񂹃��C��) * (�͕핗�Q�[���t�B�[���h�c)/(�{�ƃQ�[���t�B�[���h�c)
// (�͕핗�A�C�e�������񂹃��C��) == 128 * 272/448 == 77.7142857142857142857142857142857 =:= 78[dots] �P���Ȕ䗦�v�Z�̏ꍇ�B
// http://hossy.info/game/toho/k_score.php �����̉摜���t�Z���Čv�Z����� 77 dots ������B
/* �A�C�e�������񂹃��C�� */
//#define PLAYER_SPLIT_LINE256		(t256(77))/* �摜���牼�� */
//#define PLAYER_SPLIT_LINE256		(t256(78))/* �{�ƂƓ��䗦�Ɖ��肵�t�Z */
#define PLAYER_SPLIT_LINE256		(t256(80))/* ���S���W�ŊǗ����ĂȂ��z(-r31) */
/* ����(r33) �v���C���o�Ƃ��Ă���3��(77, 78, 80)�̒��ł� (80)����Ԗ{�Ƃɋ߂��B(77, 78)�͏�߂���B
���܂艺�ɂ��Ă��ʔ����Ȃ�����A(80)���x���Ó��ȋC������B */

/* �G�������Ȃ����C�� (���̃��C����艺����͓G�������Ȃ�) */
#define ENEMY_LAST_SHOT_LINE256 	(t256(272-72))


enum /*_select_player_*/
{
	/*0*/	REIMU_A = 0,
	/*1*/	REIMU_B,
	/*2*/	MARISA_A,
	/*3*/	MARISA_B,
//
	/*4*/	REMILIA,
	/*5*/	YUYUKO,
	/*6*/	CIRNO_A,
	/*7*/	CIRNO_Q,
//
	MAX_08_PLAYER
};

/*�Ȃ�*/
#define STATE_FLAG_00_NONE							(0x0000)
/* [�㕔�������W] MAX���Ƀv���C���[�㕔���W�Ŏ����A�C�e�����W */
#define STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM		(0x0001)
/* [�{���ɂ�鎩�����W] �{�������Ŏ����A�C�e�����W */
#define STATE_FLAG_02_BOMB_AUTO_GET_ITEM			(0x0002)
/* [�X�R�A�������W] �{�X�|����Ɏ����_�����W(���A�C�e���p) */
#define STATE_FLAG_03_SCORE_AUTO_GET_ITEM			(0x0004)
/* [�������̏ꍇ �A�C�e���㕔���W�����ł��\] */
#define STATE_FLAG_04_IS_MARISA 					(0x0008)

/* �{�X�Ɛ키�ۂ�on(�{�X�O�C�x���g�ł�OFF) */
#define STATE_FLAG_05_IS_BOSS						(0x0010)

/* �Q�[�����V�i���I�X�N���v�g���[�h�� on �ɂȂ�B�Q�[�����ĊJ������ off �ɂȂ�B*/
#define STATE_FLAG_06_IS_SCRIPT 					(0x0020)
/* on */
#define STATE_FLAG_07_IS_GRAZE						(0x0040)
/* on */
//#define STATE_FLAG_08_OPTION_HIDE 				(0x0080)/* �p�~ */

/* on */
//efine ST ATE_FLAG_IS_PANEL_WINDOW_SHIFT			(8)
//efine ST ATE_FLAG_09_IS_PANEL_WINDOW				(0x0100)
/* �{�X��|�����ꍇ��on(���Ԑ؂�ł�OFF) */
#define STATE_FLAG_09_IS_WIN_BOSS					(0x0100)
/* on */
#define STATE_FLAG_10_IS_LOAD_SCRIPT				(0x0200)
/* on */
//#define STATE_FLAG_11_IS_BOSS_DESTROY 			(0x0400)/* �p�~ */
/* on */
#define STATE_FLAG_12_END_SCRIPT					(0x0800)
/* on */
#define STATE_FLAG_13_DRAW_BOSS_GAUGE				(0x1000)

//#define STATE_FLAG_0123_AUTO_GET_ITEM (STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM|STATE_FLAG_02_BOMB_AUTO_GET_ITEM|STATE_FLAG_03_SCORE_AUTO_GET_ITEM)

//#define ST ATE_FLAG_14_GAME_LOOP_QUIT 			(0x2000)
#define STATE_FLAG_14_ZAKO_TUIKA					(0x2000)/* �G���ǉ�����������ꍇon */

#define STATE_FLAG_15_KEY_SHOT						(0x4000)
//#define STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL		(0x8000)/* �p�~? */

#define STATE_FLAG_16_GAME_TERMINATE		(0x8000)/* result��ɃQ�[���I�� */


typedef struct _game_core_global_class_
{
	int state_flag; 	/* �ݒ�t���O(�W) */
	int weapon_power;	/* 0x00-0x80  (0-128 ��129�i�K==�{�ƂƓ���)   max==128==�u129�i�K�v*/
	int chain_point;
	int bomber_time;	/* Use Gu */
//
	u32 game_score; 	/* �X�R�A���_ */
	u32 graze_point;	/* �O���C�Y���_ */
	int bombs;			/* �{���� */
	int zanki;			/* �c��`�����X */
//
//	/* �W�v�V�X�e��(player_data) */
	int player_data_use_continue;	/* �R���e�B�j���[�� */
	int player_data_count_miss; 	/* �~�X�� */
	int player_data_used_bomber;	/* �{���g�p�� */
	int player_data_use_kurai_bomb; /* ��炢�{�������� */
	int player_data_count_bonus;	/* �X�y���J�[�h�{�[�i�X�� */
//
	int msg_time;/* ���b�Z�[�W(���Ή�)�\������ */
//
//	u8 laser_mode;	/* �Ă���(���[�U�[���[�h 0:off, 1:on) */
//	u8 dummy1;
	/* �W�v�V�X�e���ȊO�̕ێ���� */
//	u8 game_select_player;
//�x�߂���B	u8 game_difficulty/* = RANK_EASY*/; 	/*	RANK_NORMAL*/
//�x�߂���B	int game_difficulty;
//
	s8 game_now_stage;				/* ���݃X�e�[�W�ԍ� */
	s8 game_continue_stage; 		/* ���݃R���e�B�j���[����X�e�[�W�ԍ���ێ� */
	s8 game_now_max_continue;		/* �R���e�B�j���[�\�ȉ� */
	s8 game_practice_mode;			/* ���K���[�h */
//
	s8 side_panel_draw_flag;/* �p�l���\��on(0�ȊO)/off(0) */
	s8 chuu_boss_mode;//1802514
	s8 draw_flag_script_screen; 	/* ����ӃE�B���h�E�\���t���O */
	s8 dummy2;/* �\��2(.align���킹�ŕK�v) */
} GAME_CORE_GLOBAL_CLASS;
extern GAME_CORE_GLOBAL_CLASS cg;

/* �Ӑ}�I�ɓ���Ȃ����� */
extern unsigned int cg_game_select_player;/* cg_game_difficulty: (�����͂Ƃ�����)����(r33)�� GAME_CORE_GLOBAL_CLASS�ɓ���Ȃ������ǂ����ۂ��B */
extern unsigned int cg_game_difficulty;/* cg_game_difficulty: GAME_CORE_GLOBAL_CLASS�ɓ����Ƒ��x�ቺ����B */

// /*extern*/ int msg_time; 					/* ����ӃE�B���h�E�\������(��) */
#if 1
	#if 0/* �A���C�����g�֌W(???) (s16)�� GAME_CORE_GLOBAL_CLASS�ɓ����ƍI�������Ȃ� */
	/* �Ӑ}�I�ɓ���Ȃ����� */
	extern	u32 cg_my_pad;			/*�������*/
	extern	u32 cg_my_pad_alter;	/*�O�����*/
	extern	s16 cg_analog_x;		/* �A�i���O�ʁA�␳�ς� */
	extern	s16 cg_analog_y;		/* �A�i���O�ʁA�␳�ς� */
	#else/*(r34)*/
typedef struct _psp_pad_global_class_
{
	u32 pad_data;						/*�������*/
	u32 pad_data_alter; 				/*�O�����*/
	s16 analog_absolute_value_x;		/* �A�i���O�ʁA�␳�ς� */
	s16 analog_absolute_value_y;		/* �A�i���O�ʁA�␳�ς� */
} PSP_PAD_GLOBAL_CLASS;
extern PSP_PAD_GLOBAL_CLASS psp_pad;
	#endif
//
extern int draw_boss_hp_value;	/* �{�Xhp�`��l */
extern int boss_life_value; 	/* �{�Xhp�̗͒l / �{�X�����w�T�C�Y�`��l */
#endif
extern int boss_x256;
extern int boss_y256;


#define USE_HOLD_GAME_MODE	(0)
#if (1==USE_HOLD_GAME_MODE)
extern void hold_game_mode_on(void);/* �Q�[�����Ԃ̈ꎞ��~ */
extern void hold_game_mode_off(void);/* �Q�[�����Ԃ̓���J�n */
#endif

//extern u32 my_pad;			/*�������*/
//extern u32 my_pad_alter;		/*�O�����*/
//extern /*global*/short cg_analog_x; /* �A�i���O�ʁA�␳�ς� */
//extern /*global*/short cg_analog_y; /* �A�i���O�ʁA�␳�ς� */

//#define IS_KEY BOARD_PULLED ((0==my_pad)&&(my_pad^my_pad_alter))/* �����L�[�𗣂��ꂽ�� */
/*	�L�[�𗣂����u�Ԃ��m��Ƃ���B�������u�Ԃ��m�肾�ƁA�����ꂽ�Ԃ������삷�镨�������ɂ���Ɠs���������B */
/*
	r31:����ł̓L�[���������u�Ԃ��m��̂��̂������̂ŁA���������Ƃ���̓L�[���������u�Ԃ��m��ɏC�������B
*/

/* 127 ���Ǝv���Ă������ǁA128�݂���(�܂�129�i�K) */
//#define MAX_POWER_IS_128 (127)/* 0x00-0x7f  (0-127 ��128�i�K) */
//#define MAX_POWER_IS_128 (128)/* 0x00-0x80  (0-128 ��129�i�K) */
#define MAX_POWER_IS_128 (128)

//typedef struct
//{
//	int strength;
//} WEAPON_BASE;

//#define DEFAULT_MAX_CONTINUE	(3)
//#define DEFAULT_MAX_CONTINUE	(30)
#define DEFAULT_MAX_CONTINUE	(30)


#endif /* _PLAYER_H_ */
