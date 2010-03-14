#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "game_main.h"

/*---------------------------------------------------------
	�X�v���C�g �}�l�[�W��
--------------------------------------------------------- */

/* Sprites */
//#define MAX_SPRITE 200

/*---------------------------------------------------------
	���\�[�X resource
--------------------------------------------------------- */
enum
{
	#if 0
	//TAMA_TYPE_KUGEL_PNG,
	//TAMA_TYPE_KUGEL_MINI2_PNG,
	TAMA_TYPE_BULLET_BEAM16_PNG,
	TAMA_TYPE_BULLET_MARU16_PNG,
//
	TAMA_TYPE_BULLET_MING32_PNG,
	TAMA_TYPE_BULLET_JIPPOU32_PNG,
//
	TAMA_TYPE_OODAMA_08_PNG,/* ��e(����...)	����(��) PRIORITY_03_ENEMY �́A�����蔻�蕔�� */
//	TAMA_TYPE_OODAMA_00_PNG,/* ����(��)  �����蔻�蕔��*/
//	TAMA_TYPE_OODAMA_01_PNG,/* ��e(��) �\������ */
//	TAMA_TYPE_OODAMA_02_PNG,/* ��e(��) �\������*/
//
	TAMA_TYPE_BULLET_KNIFE01_PNG,/*�����~���i�C�t(��)*/
	TAMA_TYPE_BULLET_KNIFE18_PNG,/*�S�����i�C�t(��)*/
	#else
	TAMA_ATARI_02,
	TAMA_ATARI_04,
	#endif
	TAMA_TYPE_HOMING16_PNG,
//
	JIKI_ATARI_ITEM_16,
	JIKI_ATARI_ITEM_80,/* ��{�� */
//
	BASE_BOMBER_ATARI_0a,/*4*/		/* �얲A */
	BASE_BOMBER_ATARI_0b,/*4*/		/* �얲B */
	BASE_BOMBER_ATARI_0c,/*16*/ 	/* ������A */
	BASE_BOMBER_ATARI_0d,/*16*/ 	/* ������B */
	BASE_BOMBER_ATARI_0e,/*16*/ 	/* ������C */
	BASE_BOMBER_ATARI_0f,/*16*/ 	/* ���~���A */
	BASE_BOMBER_ATARI_0g,/*32*/ 	/* �`���m */
	BASE_BOMBER_ATARI_0h,/*4*/		/* �H�X�q */
//
	BASE_SHOT_ATARI_0a,/*4*/		/* �얲A */
	BASE_SHOT_ATARI_0b,/*4*/		/* �얲B */
	BASE_SHOT_ATARI_0c,/*4*/		/* ������A */
	BASE_SHOT_ATARI_0d,/*4*/		/* ������B */
	BASE_SHOT_ATARI_0e,/*4*/		/* ������C */
	BASE_SHOT_ATARI_0f,/*4*/		/* ���~���A */
	BASE_SHOT_ATARI_0g,/*6*/		/* �`���m */
	BASE_SHOT_ATARI_0h,/*8*/		/* �H�X�q */
//
	BASE_NEEDLE_ATARI_0a,/*6*/		/* �얲A */
	BASE_NEEDLE_ATARI_0b,/*6*/		/* �얲B */
	BASE_NEEDLE_ATARI_0c,/*8*/		/* ������A */
	BASE_NEEDLE_ATARI_0d,/*8*/		/* ������B */
	BASE_NEEDLE_ATARI_0e,/*8*/		/* ������C */
	BASE_NEEDLE_ATARI_0f,/*12*/ 	/* ���~���A */
	BASE_NEEDLE_ATARI_0g,/*16*/ 	/* �`���m */
	BASE_NEEDLE_ATARI_0h,/*8*/		/* �H�X�q */
//
	BASE_CORE_ATARI_0a,/*6*/		/* �얲A */
	BASE_CORE_ATARI_0b,/*6*/		/* �얲B */
	BASE_CORE_ATARI_0c,/*8*/		/* ������A */
	BASE_CORE_ATARI_0d,/*8*/		/* ������B */
	BASE_CORE_ATARI_0e,/*8*/		/* ������C */
	BASE_CORE_ATARI_0f,/*12*/		/* ���~���A */
	BASE_CORE_ATARI_0g,/*16*/		/* �`���m */
	BASE_CORE_ATARI_0h,/*8*/		/* �H�X�q */
};
#if 0
	#define TAMA_TYPE_BULLET_DUMMY TAMA_TYPE_BULLET_MARU16_PNG
#else
	#define TAMA_TYPE_BULLET_DUMMY			TAMA_ATARI_02
	#define TAMA_TYPE_BULLET_BEAM16_PNG 	TAMA_ATARI_02
	#define TAMA_TYPE_BULLET_MARU16_PNG 	TAMA_ATARI_02
	#define TAMA_TYPE_BULLET_MING32_PNG 	TAMA_ATARI_02
	#define TAMA_TYPE_BULLET_JIPPOU32_PNG	TAMA_ATARI_02
	#define TAMA_TYPE_OODAMA_08_PNG 		TAMA_ATARI_04
	#define TAMA_TYPE_BULLET_KNIFE01_PNG	TAMA_ATARI_02
	#define TAMA_TYPE_BULLET_KNIFE18_PNG	TAMA_ATARI_02
	#define ZAKO_TYPE_ATARI04_PNG			TAMA_ATARI_04
	#define ZAKO_TYPE_ATARI16_PNG			JIKI_ATARI_ITEM_16
#endif



#define USER_BOMOUT_WAIT (30)

/* �g�p�ς݂���������Ă���t���O */
#define SP_DELETE				(0)
/*	0 �Ȃ�Δ��ʏ������ȗ��ł���B -1 ���ƈ�X���ʏ���������
	KETM�ł͔��ʂ��Ȃ��ƂȂ�Ȃ��ꏊ�ɔ��ʏ����������ĂȂ��ׁA
	�X�v���C�g�����������Ȃ�o�O�����������ɂ���B */
/*
	�����͎�ޕʂł͂Ȃ��āA�@�\�ʂɕ��ނ��Ă��������B
*/
#define SP_GROUP_SHOT_ZAKO				(0x0000)/*0x0100*/
#define SP_GROUP_SHOT_SPECIAL			(0x0080)/*0x0100*/		/* �얲�̉�]�V���b�g */		/* kodomo_hlaser�̔��� */
//
#define SP_GROUP_JIKI_GET_ITEM			(0x0100)/*0x0100*/		/* === Gu�v���C���[�� === */
//#define dummy_SP_GROUP_PLAYER_CORE	(0x0200)/*0x0100*/
#define SP_GROUP_TEKI					(0x0400)/*0x0400*/		/* === Gu�G�� === */
//#define SP_GROUP_BOSS 				(0x0800)/*0x0400*/		/* === Gu�G�� === */
//#define SP_GROUP_TEKI 				(0x0400)/*0x0400*/
//#define SP_GROUP_TEKI 				(SP_GROUP_TEKI|0)
//#define SP_GROUP_BOSS 				(SP_GROUP_TEKI|SP_GROUP_SHOT_SPECIAL)
#define SP_GROUP_BULLETS				(0x0800)/*(0x1000)0x0800*/		/* === Gu�e���� === */
#define SP_GROUP_ITEMS					(0x1000)/*(0x2000)0x1000*/
//#define SP_GROUP_PAUSE_S P_ME NU_TEXT 	(0x4000)/*0x2000*/		/*SDL*/
#define SP_GROUP_ETC					(0x2000)/*(0x8000)*/				/* === Gu�t�����g�� === */

//#define SP_GROUP_ALL_SDL_TYPE (0xff00)
#define SP_GROUP_ALL_SDL_WORK_TYPE		(0x3f00)/* SDL���� */
//#define SP_GROUP_ALL_SDL_CORE_TYPE		((0x3f00) & (~(SP_GROUP_PAUSE_S P_ME NU_TEXT|0)))/* game_core���� */
#define SP_GROUP_ALL_SDL_CORE_TYPE		(0x3f00)/* game_core���� */

//#define SP_GROUP_ALL_SDL_DRAW_TYPE	(SP_GROUP_ALL_SDL_WORK_TYPE & (~(SP_GROUP_TEKI|SP_GROUP_ITEMS|SP_GROUP_BULLETS)))/*SDL�\��*/
/* �G���A�C�e�����G�e�͏��Ȃ��Ƃ�SDL�ŕ`���Ȃ� */
//#define SP_GROUP_ALL_SDL_DRAW_TYPE	(0x4000)/* SDL�\��(����SP_GROUP_PAUSE_S P_ME NU_TEXT�̂�SDL�`��) */

#define SP_GROUP_SUB_TYPE_256			(0x00ff)
#define SP_GROUP_SUB_TYPE_128			(0x007f)
/* �ȉ������蔻��Ȃ� */
#define SP_GROUP_MAHOU_JIN				(0x4000)/*(0x00010000)*/			/* === Gu�����w�� === */
#define SP_GROUP_PANEL					(0x8000)/*(0x00020000)*/			/* === Gu�p�l���� === */
#define SP_GROUP_ALL_GAME_OBJS			(0x00ffff00)			/* �Q�[���R�A�I�����ɂ����Â���obj */
//#define SP_GROUP_SHOTS				(0x0200)
//#define SP_GROUP_ETC					(0x4000)
//#define SP_GROUP_ENEMYS				(0x0200)/*(0x0400)*/
/* ���p */
//#define SP_GROUP_TEXTS				(SP_GROUP_PLAYER)/*(0x2000)*/

/* SP_GROUP_ETC == SP_GROUP_FRONT */
#define SP_GROUP_FRONT					(SP_GROUP_ETC)/* �t�����g�� */

		//���E�F�|���A�C�e��(��)	//�E�F�|���A�C�e��(��)��SP_ITEM_EXTRA_SHIELD,
		//�������͒ᑬ�{���ɋz�����ꂽ�BSP_ITEM_EXTRA_HOMING, SP_ITEM_EXTRA_HLASER,
/*	����A */
//	S P_PLAYER_ITEM/*�v���C���[�̖{��(�����蔻�肪�A�C�e���擾)*/	= /*0x0100*/SP_GROUP_JIKI_GET_ITEM, /* Unser Held */
/*	���e */
//	S P_SHOT_BOSS,/*�v���C���[�̃{�X�ɗL���e*/					//	= /*0x0200*/SP_GROUP_SHOTS, 	/* Waffen des Helden */
//	S P_SHOT_ZAKO,/*�v���C���[�̃{�X�ɖ����e*/
/*	����B */
	//dummy_SP_PLAYER_CORE/*�v���C���[�̃R�A(�����蔻�肪���S����)*/		= /*0x0100*/SP_GROUP_PLAYER_CORE,
/* ------------- */
/*	�U�R�G (�{�����A�̓�����Ńv���C���[���ȂȂ�)  */
//	SP_ZAKO 														= /*0x0400*/SP_GROUP_TEKI,		/* Die Boesen */
/* ------------- */
/*	�{�X�G / ���U�R�G (�{�����A�̓�����Ńv���C���[����)  */
//	S P_CHUU,// /*����G[���^�G]*/ /*S P_ZA KO_YOKAI1*/					= /*0x0400*/SP_GROUP_BOSS,		/* [***090325 */
//	S P_BO SS,
/* ------------- */
/*	�G�e */
	//dummy_SP_BULLET												= /*0x0800*/SP_GROUP_BULLETS,	/* ... und ihre Waffen */
/* ------------- */

enum /*sprite_type*/
{
/* ------------- */
/*	�|�[�Y���ړ��\�� */
//	S P_ME NU_TEXT	/* ���� */										= /*0x2000*/SP_GROUP_PAUSE_S P_ME NU_TEXT,
/*	���̑� */
	SP_MUTEKI														= /*0x4000*/SP_GROUP_ETC,		/* diverses */
};


/* === Gu�G�� === */		/* --- �܂�����ĂȂ� --- */
enum	/* �� �����w�ʃG�t�F�N�g */
{
	TEKI_00_BOSS11			/* �{�X & ���{�X[�E��1] */						= /*0x0400*/SP_GROUP_TEKI,
	TEKI_01_BOSS12,			/* �{�X & ���{�X[�E��2] */
	TEKI_02_BOSS13,			/* �{�X & ���{�X[�E��3] */
	TEKI_03_BOSS14,			/* �{�X & ���{�X[�E��4] */
	TEKI_04_BOSS21,			/* �{�X & ���{�X[����1] */
	TEKI_05_BOSS22,			/* �{�X & ���{�X[����2] */
	TEKI_06_BOSS23,			/* �{�X & ���{�X[����3] */
	TEKI_07_BOSS24,			/* �{�X & ���{�X[����4] */
//
	TEKI_08_BOSS31,			/* �{�X & ���{�X[���S1] */
	TEKI_09_BOSS32,			/* �{�X & ���{�X[���S2] */
	TEKI_10_BOSS33,			/* �{�X & ���{�X[���S3] */
	TEKI_11_BOSS34,			/* �{�X & ���{�X[���S4] */
	TEKI_12_YOUSEI1_1,		/* �d��1 */
	TEKI_13_YOUSEI1_2,		/* �d��1 */
	TEKI_14_YOUSEI1_3,		/* �d��1 */
	TEKI_15_YOUSEI1_4,		/* �d��1 */
//
	TEKI_16_YOUSEI1_5,		/* �d��1 */ 	/* BOSS_16_YOUSEI11 */
	TEKI_17_YOUSEI1_6,		/* �d��1 */
	TEKI_18_YOUSEI1_7,		/* �d��1 */
	TEKI_19_YOUSEI1_8,		/* �d��1 */
	TEKI_20_YOUSEI1_9,		/* �d��1 */
	TEKI_21_YOUSEI1_a,		/* �d��1 */
	TEKI_22_YOUSEI1_b,		/* �d��1 */
	TEKI_23_YOUSEI1_c,		/* �d��1 */
//
	TEKI_24_YOUSEI2_1,		/* �d��2 */
	TEKI_25_YOUSEI2_2,		/* �d��2 */
	TEKI_26_YOUSEI2_3,		/* �d��2 */
	TEKI_27_YOUSEI2_4,		/* �d��2 */
	TEKI_28_YOUSEI2_5,		/* �d��2 */
	TEKI_29_YOUSEI2_6,		/* �d��2 */
	TEKI_30_YOUSEI2_7,		/* �d��2 */
	TEKI_31_YOUSEI2_8,		/* �d��2 */
//
	TEKI_32_YOUSEI2_9,		/* �d��2 */
	TEKI_33_YOUSEI2_a,		/* �d��2 */
	TEKI_34_YOUSEI2_b,		/* �d��2 */
	TEKI_35_YOUSEI2_c,		/* �d��2 */
	TEKI_36_YOUSEI3_1,		/* �d��3 */
	TEKI_37_YOUSEI3_2,		/* �d��3 */
	TEKI_38_YOUSEI3_3,		/* �d��3 */
	TEKI_39_YOUSEI3_4,		/* �d��3 */
//
	TEKI_40_YOUSEI3_5,		/* �d��3 */
	TEKI_41_YOUSEI3_6,		/* �d��3 */
	TEKI_42_YOUSEI3_7,		/* �d��3 */
	TEKI_43_YOUSEI3_8,		/* �d��3 */
	TEKI_44_YOUSEI3_9,		/* �d��3 */
	TEKI_45_YOUSEI3_a,		/* �d��3 */
	TEKI_46_YOUSEI3_b,		/* �d��3 */
	TEKI_47_YOUSEI3_c,		/* �d��3 */
//
	TEKI_48_EFFECT1_resurved,	/* r29���� ���g�p */
	TEKI_49_EFFECT2_resurved,	/* r29���� ���g�p */
	TEKI_50_EFFECT3_resurved,	/* r29���� ���g�p */
	TEKI_51_EFFECT4_resurved,	/* r29���� ���g�p */
	TEKI_52_MAHOJIN,			/* ���̖����w */
	TEKI_53_INYOU_DAI,
	TEKI_54_CHOU1,
	TEKI_55_CHOU2,
//
	TEKI_56_CHEN,
	TEKI_57_BLUE_BOOK,
	TEKI_58_RED_BOOK,
	TEKI_59_HAI_KEDAMA,
	TEKI_60_AKA_KEDAMA,
	TEKI_61_NIJI_HOSI,
	TEKI_62_TATSUMAKI,
	TEKI_63_HOMING_STUFF,
};
#define BOSS_00_BOSS11		TEKI_00_BOSS11
#define BOSS_16_YOUSEI11	TEKI_16_YOUSEI1_5

/* === Gu�����w�� === */		/* --- �܂�����ĂȂ� --- */

enum	/* �� �����w�ʃG�t�F�N�g */
{
	MAHOU_JIN_00_aaa	= /*0x00010000*/SP_GROUP_MAHOU_JIN,
	MAHOU_JIN_01_bbb,
	MAHOU_JIN_02_,
	MAHOU_JIN_03_,
	MAHOU_JIN_04_,
	MAHOU_JIN_05_,
	MAHOU_JIN_06_,
	MAHOU_JIN_07_,
};

/* === Gu�t�����g�� === */		/* --- �܂�����ĂȂ� --- */

enum	/* �� �t�����g�ʃG�t�F�N�g */
{	/* �����̂����蔻��ʒu�\���p�R�A(�t�����g�ʃG�t�F�N�g) */
	JIKI_CORE_00_REIMU_A	= /*0x4000*/SP_GROUP_ETC,
	JIKI_CORE_01_REIMU_B,
	JIKI_CORE_02_MARISA_A,
	JIKI_CORE_03_MARISA_B,
	JIKI_CORE_04_MARISA_C,
	JIKI_CORE_05_REMILIA,
	JIKI_CORE_06_CHIRNO,
	JIKI_CORE_07_YUYUKO,
//	/* CORE�B���p */
	CORE_HIDE_10_REIMU_A,
	CORE_HIDE_11_REIMU_B,
	CORE_HIDE_12_MARISA_A,
	CORE_HIDE_13_MARISA_B,
	CORE_HIDE_14_MARISA_C,
	CORE_HIDE_15_REMILIA,
	CORE_HIDE_16_CHIRNO,
	CORE_HIDE_17_YUYUKO,
//	/* ������ / �U�R���Ŕ���(�t�����g�ʃG�t�F�N�g) */
	BAKUHA00,	/* ������(���F?�A���݃_�~�[) */
	BAKUHA01,	/* ������(��)BASE_TR_BLUE_PNG */
	BAKUHA02,	/* ������(��)BASE_TR_RED_PNG */
	BAKUHA03,	/* ������(��)BASE_TR_GREEN_PNG */
	BAKUHA04,	/* �U�R���Ŕ���(���F�A���݃_�~�[) */
	BAKUHA05,	/* �U�R���Ŕ���(��)BASE_BAKUHA05_PNG */
	BAKUHA06,	/* �U�R���Ŕ���(��)BASE_BAKUHA06_PNG */
	BAKUHA07,	/* �U�R���Ŕ���(��)BASE_BAKUHA07_PNG */
//	/* �Ή�����(�t�����g�ʃG�t�F�N�g) */
	BAKUHA08,	/* �Ή����� */
	BAKUHA09,	/* �Ή����� */
	BAKUHA0a,	/* �Ή����� */
	BAKUHA0b,	/* �Ή����� */
	BAKUHA0c,	/* �Ή����� */
	BAKUHA0d,	/* �Ή����� */
	BAKUHA0e,	/* �Ή����� */
	BAKUHA0f,	/* �Ή����� */
//
	SPELL_SQUERE_,	/* [��] x 5 */
	SP_GROUP_ETC_DUMMY_REMILIA,
	SP_GROUP_ETC_DUMMY_SLOW_BOMB,
//	SPELL_LOGO_,/* Border Power of Spiritual */
	SPELL_TACHIE_,
};
/* Border Power of Spiritual. ��̂ŗ͂𔭊����鋫�E. ��̂ł̗͂̋��E */
/* Border Power of Spiritus.  �𐸂ŗ͂𔭊����鋫�E. �𐸂ł̗͂̋��E */
/*
Spiritus  ��
Spiritual ���
��̂̋��E��
Border Power of Spiritual. ��̂̃p���[�ɐڂ��Ă��������B
Border Power of Spiritus. �𐸂̃p���[�ɐڂ��Ă��������B
*/

/* === Gu�v���C���[�� === */
enum
{
//	�v���C���[�A�j���[�V����
//	���ֈړ��̃A�j���[�V����	[���S������4������3������2������1������1������1��...]
	JIKI_PLAYER_00_LEFT 	/* ����1 */ 	/* �ł��� */			= SP_GROUP_JIKI_GET_ITEM/*0x0800*/,
	JIKI_PLAYER_01_LEFT,	/* ����2 */
	JIKI_PLAYER_02_LEFT,	/* ����3 */
	JIKI_PLAYER_03_LEFT,	/* ����4 */ 	/* ����荶 */
//	�J��Ԃ��A�j���[�V����		[���S1�����S2�����S3�����S4�����S1�����S2�����S3�����S4��...]
	JIKI_PLAYER_04_MID, 	/* ���S1 */
	JIKI_PLAYER_05_MID, 	/* ���S2 */
	JIKI_PLAYER_06_MID, 	/* ���S3 */
	JIKI_PLAYER_07_MID, 	/* ���S4 */
//	�E�ֈړ��̃A�j���[�V����	[���S���E��1���E��2���E��3���E��4���E��4���E��4��...]
	JIKI_PLAYER_08_RIGHT,	/* �E��1 */ 	/* �����E */
	JIKI_PLAYER_09_RIGHT,	/* �E��2 */
	JIKI_PLAYER_10_RIGHT,	/* �E��3 */
	JIKI_PLAYER_11_RIGHT,	/* �E��4 */ 	/* �ł��E */
	JIKI_ETC_00,			/* --- ���g�p --- */
	JIKI_ETC_01,			/* --- ���g�p --- */
	JIKI_ETC_02,			/* --- ���g�p --- */
	JIKI_ETC_03,			/* --- ���g�p --- */
//
	JIKI_BOMBER_00, 		/* �{���o�[�������̗����G */
	JIKI_BOMBER_01, 		/* �U���{��(�ᑬ�{��)     */
	JIKI_BOMBER_02, 		/* --- ���g�p --- / ���E�� / ����1 */
	JIKI_BOMBER_03, 		/* --- ���g�p --- / ���E�� / ����2 */
	JIKI_BOMBER_04, 		/* ���D1 / ��1 / �\����1 / ��1 / �` */
	JIKI_BOMBER_05, 		/* ���D2 / ��2 / �\����2 / ��2 / �� */
	JIKI_BOMBER_06, 		/* ���D3 / ��3 / �\����3 / ��3 / �m */
	JIKI_BOMBER_07, 		/* ���D4 / ��4 / �\����4 / ��4 / �H */
//
	JIKI_SHOT_00,			/* �ԎDC /  /  / ����3 /  */
	JIKI_SHOT_01,			/* �ԎDB /  /  / ����4 /  */
	JIKI_SHOT_02,			/* �ԎDA / ���� / ���� / �X / �� */
	JIKI_SHOT_03,			/* / ��������e1 / ���D1 */
	JIKI_SHOT_04,			/* / ��������e2 / ���D2 */
	JIKI_SHOT_05,			/* / ��������e3 / ���D3 */
	JIKI_SHOT_06,			/* / ��������e4 / ���D4 */
	JIKI_SHOT_07,			/* / ��������e5 / �j�e / �X�e / �E�F�C�u�e / �s���N���e */
//
	JIKI_OPTION_00_00,
	JIKI_OPTION_00_01,
	JIKI_OPTION_00_02,
	JIKI_OPTION_00_03,
	JIKI_OPTION_00_04,
	JIKI_OPTION_00_05,
	JIKI_OPTION_00_06,
	JIKI_OPTION_00_07,
//
	JIKI_OPTION_01_00,
	JIKI_OPTION_01_01,
	JIKI_OPTION_01_02,
	JIKI_OPTION_01_03,
	JIKI_OPTION_01_04,
	JIKI_OPTION_01_05,
	JIKI_OPTION_01_06,
	JIKI_OPTION_01_07,
//
	JIKI_OPTION_02_00,
	JIKI_OPTION_02_01,
	JIKI_OPTION_02_02,
	JIKI_OPTION_02_03,
	JIKI_OPTION_02_04,
	JIKI_OPTION_02_05,
	JIKI_OPTION_02_06,
	JIKI_OPTION_02_07,
//
	JIKI_OPTION_03_00,
	JIKI_OPTION_03_01,
	JIKI_OPTION_03_02,
	JIKI_OPTION_03_03,
	JIKI_OPTION_03_04,
	JIKI_OPTION_03_05,
	JIKI_OPTION_03_06,
	JIKI_OPTION_03_07,
};

/* === Gu�e���� === */
/*haisi_BULLET_MINI8_00_AKA */
//	haisi_BULLET_MINI8_03_AKA,
//	haisi_BULLET_MINI8_04_KIIRO,
//	haisi_BULLET_MINI8_05_AOI,
//	haisi_BULLET_MINI8_06_AOI,
//	haisi_BULLET_MINI8_07_YUKARI,
//	haisi_BULLET_MINI8_01_AKA,
//	haisi_BULLET_MINI8_02_KIIRO,

enum
{
//1
	BULLET_00_HOSI_TEN			= SP_GROUP_BULLETS/*0x0800*/,
	BULLET_KOME_01_AOI,
	BULLET_KOME_02_AKA,
	BULLET_KOME_03_YUKARI,
	BULLET_KOME_04_MIDORI,
	BULLET_KOME_05_KIIRO,
	BULLET_KOME_06_dummy,
	BULLET_KOME_07_dummy,
//2
	BULLET_MARU8_00_AKA,
	BULLET_MARU8_01_YUKARI,
	BULLET_MARU8_02_AOI,
	BULLET_MARU8_03_MIDORI,
	BULLET_MARU8_04_MIDORI,
	BULLET_MARU8_05_MIDORI,
	BULLET_MARU8_06_KIIRO,
	BULLET_MARU8_07_AOI,
//3
	BULLET_MARU8_08_dummy,
	BULLET_MARU8_09_dummy,
	BULLET_MARU8_10_dummy,
	BULLET_MARU8_11_dummy,
	BULLET_MARU8_12_dummy,
	BULLET_MARU8_13_dummy,
	BULLET_CAP16_04_SIROI,
	BULLET_CAP16_05_SIROI,
//4
	BULLET_MARU10_00_AOI,
	BULLET_MARU10_01_MIDORI,
	BULLET_MARU10_02_MIDORI,
	BULLET_MARU10_03_MIDORI,
	BULLET_MARU12_00_SIRO,	/*12*/
	BULLET_MARU12_01_AKA,	/*12*/
	BULLET_MARU12_02_MIDORI,/*12*/
	BULLET_MARU12_03_AOI,	/*12*/
//5
	BULLET_UROKO14_00_AOI,
	BULLET_UROKO14_01_AKA,
	BULLET_UROKO14_02_YUKARI,
	BULLET_UROKO14_03_MIDORI,
	BULLET_UROKO14_04_MIZUIRO,
	BULLET_UROKO14_05_KIIRO,
	BULLET_UROKO14_06_dummy,
	BULLET_UROKO14_07_dummy,
//6
	BULLET_KUNAI12_00_AOI,
	BULLET_KUNAI12_01_AKA,
	BULLET_KUNAI12_02_MIDORI,
	BULLET_KUNAI12_03_MIDORI,
	BULLET_KUNAI12_04_YUKARI,
	BULLET_KUNAI12_05_AOI,
	BULLET_KUNAI12_06_KIN,
	BULLET_KUNAI12_07_KIN,
//7
	BULLET_OFUDA12_00_AOI,
	BULLET_OFUDA12_01_AKA,
	BULLET_OFUDA12_02_YUKARI,
	BULLET_OFUDA12_03_MIDORI,
	BULLET_KNIFE20_04_AOI,
	BULLET_KNIFE20_05_AKA,
	BULLET_KNIFE20_06_YUKARI,
	BULLET_KNIFE20_07_MIDORI,
//8
	BULLET_HARI32_00_AOI,
	BULLET_HARI32_01_AKA,
	BULLET_HARI32_02_KIIRO,
	BULLET_HARI32_03_DAIDAI,
	BULLET_OODAMA32_00_SIROI,
	BULLET_OODAMA32_01_AOI,
	BULLET_OODAMA32_02_AKA,
	BULLET_OODAMA32_03_YUKARI,
};
//	BULLET_OODAMA32_04_MIDORI,
//	BULLET_OODAMA32_05_AOI,
//	BULLET_OODAMA32_06_KIIRO,
//	BULLET_OODAMA32_07_PINK,

/* === GuITEM�� === */
enum /*sprite_type*/
{
/*	�A�C�e�� */
	SP_ITEM_00_P001 	/* [p]�� */ 								= /*0x1000*/SP_GROUP_ITEMS, 	/* Bonus items */
	SP_ITEM_01_P008,	/* [P]�� */
	SP_ITEM_02_P128,	/* [F] */
	SP_ITEM_03_1UP, 	/* [1up] */
	SP_ITEM_04_BOMB,	/* [B] */
	SP_ITEM_05_TENSU,	/* [�_] */
	SP_ITEM_06_HOSI,	/* [��] */
	SP_ITEM_07_SPECIAL,
};

/* === GuFRONT�� === */



/*-------------------*/
/* �j�e�Ƃ����邩��A���ݐ}�`���肵�Ȃ��ƍ��\����ɂȂ�B */
/* �����蔻�薳�����Ă����x�͕ς��Ȃ������������ߖ�ł���̂ŏ����I�ɂ͂��̕ӂ����ǂ������B */
//#define USE_ZUKEI_ATARI_HANTEI (1)
//#define USE_ZUKEI_ATARI_HANTEI (0)

typedef struct /*_point256*/
{
	int x256;	/*	union POINT */	/* x�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
	int y256;	/*	union POINT */	/* y�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
} POINT256;
/* t256���W */

typedef struct /*_point_vector256*/
{
	int x256;	/*	union POINT */	/* x�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
	int y256;	/*	union POINT */	/* y�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
	int vx256;	/*	union POINT_VECTOR */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
	int vy256;	/*	union POINT_VECTOR */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
} POINT_VECTOR256;
/* t256���W + t256�x�N�g�� */


#if 1
// union ���p
	#define base_hp 					user00
	#define base_weapon_strength		user00
	#define base_time_out				user00/*bakuhatsu*/
	#define base_score					user01
	//#define base_state					user01
#endif
//
/* �X�v���C�g�̎g����

����		���@		���e					�G					�G�e			����
user00		--			base_weapon_strength	base_hp 			base_time_out	base_time_out
			--			���틭��				�̗�				����			����
user01		--			--						base_score			--				--
			--			--						�X�R�A				--				--
2018897 2018881
*/
//

/*
	x256�Ay256�́uGu�̏ꍇ�͒��S���W�v�B
	�uSDL�̏ꍇ�͍������W�v�ł��B
	�]���� SDL_w128�ASDL_h128 ��SDL�ł����g���܂���B
	�_���e�A���̓����蔻��ł͂������g���܂���B
*/
typedef struct _sprite
{
	int x256;			/*	union POINT */			/* x�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
	int y256;			/*	union POINT */			/* y�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
	int vx256;			/*	union POINT_VECTOR */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
	int vy256;			/*	union POINT_VECTOR */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
//[4]
	//int SDL_w128;						/* �����̔��� (256�Œ菬���_�`��) / Breite, H�he */
	//int SDL_h128;						/* �����̔��� (256�Œ菬���_�`��) / Breite, H�he */
//
	int m_angleCCW512;				/* �`��p�p�x(����0�x�ō����(�����v���)) */
	int tmp_angleCCW512;			/* �ێ��p�p�x(����0�x�ō����(�����v���)) */
	int user00; 		/* user00 ENEMY_BASE base; */	//int health;	int time_out;		/* user03 */
	int user01; 		/* user01 ENEMY_BASE base; */	//int score;	int state;			/* user02 */
	//int timeover_ticks;			/* �쐬���Ă���̌o�ߎ��� (����KETM���̂Ƀo�O������ׁA��莞�Ԍo�߂���Ƌ�����������) / number of ticks since creation */
//[8]
	int type;						/* ��ދy�юg�p�� / Sprite-Type, (siehe enum SPRITE_TYPE), 0 = remove. */
	u32 color32;					/* Gu color AGBR8888 */
	//SDL_Surface *sprite_bmp;		/* �摜 / Images */
	int m_Hit256R;					/* �����蔻��p */
	int m_zoom_x256;				/* �\���g�嗦 */
//[12]
//
//
//[16]
	/* �Ƃ肠�����ڍs�p(1)�]���݊�(�e) */
	int user_data00;
	int user_data01;
	int user_data02;
	int user_data03;
//
	int user_data04;
	int user_data05;
	int user_data06;
	int user_data07;
//
	/* �Ƃ肠�����ڍs�p(2)�ǉ��e�A��{�@�\ */
	int user_data10;
	int user_data11;
	int user_data12;
	int user_data13;
//
	int user_data14;
	int user_data15;
	int user_data16;
	int user_data17;
//
	void *data; 									/* Zeiger auf Strukur mit zus�tzlichen Data. */
	void (*callback_loser)(struct _sprite *src);	/* Custom-loser-Routine (���ꂽ��Ƀ{�[�i�X���o���Ƃ��A�Ⴄ��������Ƃ�) / ���@�I�v�V�����̃A�j���[�V�����R�[���o�b�N */
	void (*callback_mover)(struct _sprite *src);	/* Custom-Move-Routine */
	void (*callback_hit_enemy)(struct _sprite *c, struct _sprite *src); 	// [***090325		�ǉ�
//[20]
	struct _sprite *next;
	/*u8*/u32 flags;					/* �t���O�Z�b�g / siehe unten (SP_FLAG...) */
	//u8 dummy_priority;				/* �\���D�揇�� / Priority for drawing. (0: unten, 9 oben) */
	//u8 dummy_1_;
	//u8 dummy_2_;
//
//	u8 yx_an im_frame;			/* / �A�j���p�^�[���̔ԍ�(���R�}�ڂ�) */	 /*int*/			/* action. Frame */
//	u8 yx_fr ames;				/* / �\���p�^�[������ / Anzahl Frames */
//	u8 an im_count; 			/*int*/ 	// Sint8 anim_count;			/* intern: Zaehler f. an im_speed */
//	u8 an im_speed/* 256 */;		/*�t�]�A�j���֎~�ɕύX*/		/* �A�j���[�V�������x / Geschw. der Animation */	 /*s8*/ /*u8*/
//
//[16]
} SPRITE;
//#define callback_animate 	callback_loser
//	u8 dummy_alpha; 				/* �����x	 / 255: opak, -  0: durchsichtig */
//	u8 dummy_alpha_chache;			/* �O��̃A���t�@�l */

#define YX_FRAME_LOW_X	(0x0f)
#define YX_FRAME_HIGH_Y (0xf0)
//#define an im_frame yx_an im_frame


#define SP_FLAG_COLISION_CHECK	0x01	/* Col-Check aktiv */
#define SP_FLAG_VISIBLE 		0x02	/* Sprite sichtbar */
#define SP_FLAG_NOT_CACHE		0x04	/* Image nicht im Cache */
#define SP_FLAG_FREE_SURFACE	0x08	/* Surface loeschen */
#define SP_FLAG_GRAZE			0x10	/* �O���C�Y�ς݂��Ƃ����̃t���O */
//#define SP_FLAG_CHEAP_ALPHA	0x10	/* "Cheapalpha", nur jedes 2te Pixel setzen */
#define SP_FLAG_LOCK_TARGET 	0x20	/* SIE sind hinter mir her */

#define SP_FLAG_TIME_OVER		0x80	/* ��莞�Ԃł̎��������A���t���O(ON�Ȃ玩����������) */



/*---------------------------------------------------------
	���\�[�X resource
---------------------------------------------------------*/

extern SPRITE *sprite_add_gu(			int bullet_type_num);		/* gu�ڍs���A�ėp�X�v���C�g */
extern SPRITE *sprite_add_only_bullet(	int bullet_type_num);		/* gu�ڍs���A�e��p */

#if (0)
extern SPRITE *spr ite_add_000xy00(
	SDL_Surface *surface,
	int total_frames,
	int x_divide_frames,
	int y_divide_frames,
	u8 priority,
	int set_flags/*nocache*/,
	int anime_speed
);
#endif

extern void sprite_remove_all_SDL(int type);/*gu�ėp(��SDL)*/
extern void sprite_remove_all_444(int type);/*�e����p*/

extern void sprite_work_SDL(int type);/*gu�ėp(��SDL)*/
extern void sprite_work_444(int type);/*gu�e����p*/

//extern void sprite_display000(int type);/**/
//extern void sprite_display222(int type);/*�e���p*/
extern void pause_sprite_display(void);/**/

extern SPRITE *sprite_collision_check_SDL_teki(SPRITE *tocheck);/*�G��p*/	//, int type);
extern SPRITE *sprite_collision_check_444(SPRITE *tocheck, int type);/*gu�e����p*/

extern void gamen_gai_nara_osimai(SPRITE *src);/* ��ʊO�Ȃ炨���܂� */


//#if 1
//	/* �\�t�g�E�F�A�[�� Z�\�[�g */
//	#define USE_ZBUFFER 	(0)
//#else
//	/* �n�[�h�E�F�A�[�Ń\�[�g */
//	#define USE_ZBUFFER 	(1)
//#endif
//
//#if 1
//	/* �P���g�� */
//	#define USE_ZOOM_XY 	(0)
//#else
//	/* �c���g�� */
//	#define USE_ZOOM_XY 	(1)
//#endif


//#ifndef GRP_SCREEN_H
//#define GRP_SCREEN_H
/*	@since		Jul.27.2005 GRP_SCREEN_H		��ʕ\���Ǘ� */
/* --- �Ǘ�����ő�X�v���C�g�� */
//#define MAX_TRANS_GU_SPRITE (1024)
//#define MAX_TRANS_GU_SPRITE (256)
#define MAX_TRANS_GU_SPRITE 	(512)

typedef struct
{
	UINT8 used;
	UINT8 bullet_num;	/* �G�e�̔ԍ� */			//	UINT8 dummy_alpha;
	UINT8 harf_wide;	/* (1/2)��[�h�b�g] */		//	UINT8 texture_id;
	UINT8 harf_height;	/* (1/2)����[�h�b�g] */ 	//	UINT8 center;	/*dummy*/
	int rotation_z;
	int rotation_tmp;
	u32 color8888;//	int color8888;
//
	/* - public */
	int x256;
	int y256;
	int w;
	int h;
//
	int tx;
	int ty;
	#if (1==USE_ZOOM_XY)
	int zoom_x256;
	int zoom_y256;
	#else //(0==USE_ZOOM_XY)
	int zoom_xy256;
	#endif/* (1==USE_ZOOM_XY) */
	#if (1==USE_ZBUFFER)
	int priority;		/* �\���D�揇�� */	/* unsigned short */
	#endif/* (1==USE_ZBUFFER) */
	int aaa;
//	Image *Texture;//
	//SDL_Surface *Texture_ptr;
} TGameSprite /*, *PTGameSprite*/;



#endif/* _SPRITE_H_ */
