
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _SPRITE_H_
#define _SPRITE_H_

/*---------------------------------------------------------
	�X�v���C�g �}�l�[�W��
--------------------------------------------------------- */
/*
����:	s32 JYUMYOU
1[�b]                                   60           >                64 [flame]
1[��]                                   60 x 60      >           64 x 64 [flame]
1[����] 0x034BC0 ==   216000            60 x 60 x 60 >      64 x 64 x 64 [flame] ==   262144 0x040000
2.5[��] 0xC5C100 == 12960000 2.5 x 24 x 60 x 60 x 60 > 64 x 64 x 64 x 64 [flame] == 16777216 0x01000000
-------------------------------------------------
�����A���t���[�����Z����J�E���^������Ƃ��āA
signed int ���ƁA�����l�� 0x01ff ffff �������ꍇ�A2.5[��]�o�߂��Ă��A�J�E���^�̒l��0x01-- ----�B
�܂�ŏ��bit �͕����Ƃ��āA�ŏ��bit��������7bit�͎��R�Ɏg����B(�J�E���^���o���Ƃ�0x00ffffff�Ń}�X�N����Ƃ���)
*/

#define JYUMYOU_MUGEN	(16777216)
#define JYUMYOU_1MIN	(60*60)
#define JYUMYOU_NASI	(0)
/*
JYUMYOU_MUGEN:	(��������==�����A��2.5[��])�v���C���[��I�v�V�������A�����ŏ����Ă͍�����́B
JYUMYOU_1MIN:	(�����A��1[��])
JYUMYOU_NASI:	(�����A�Ȃ��B�����ɂȂ�ƁA�����ŏ��������̂Œ��ӁB�u���t���[������=2�ɐݒ肵�Ƃ��Ώ����Ȃ��B(����=1���Ə������Ⴄ����)�v)

*/

/* �e�̐�������(�W��) */
#define JYUMYOU_TAMA_HASSEI 	(JYUMYOU_1MIN)
/* �U�R�̐�������(�W��) */
#define JYUMYOU_ZAKO_HASSEI 	(JYUMYOU_1MIN)
/* ��ʊO�Ȃ�U�R�����ꍇ�́A���~�b�g���� */
//#define JYUMYOU_ZAKO_CLIP 		(2048)				/* 2048/60 = 34.1333333333333333333333333333333[�b] */

/*
	�������Ԃ���_�Ƃ��������̏ꍇ�A(��ŃV�X�e�����ς�邩������Ȃ�����)
	��L(JYUMYOU_TAMA_HASSEI / JYUMYOU_ZAKO_HASSEI)����Ƃ��Ă����B
*/


//#define SP_FLAG_COLISION_CHECK	(0x01)				/* �����蔻�肠��t���O */
//#define SP_FLAG_GRAZE 			(0x10)				/* �O���C�Y�ς݂��Ƃ����̃t���O */
#define SP_FLAG_COLISION_CHECK		(0x8000)			/* �����蔻�肠��t���O */
#define SP_FLAG_GRAZE				(0x0080)			/* �O���C�Y�ς݂��Ƃ����̃t���O */
#define SP_FLAG_OPTION_VISIBLE		(SP_FLAG_GRAZE) 	/* �v���C���[�I�v�V�����L���t���O */


/* Sprites */
//#define MAX_SPRITE 200

#define USER_BOMOUT_WAIT (30)

/* �g�p�ς݂���������Ă���t���O */
//#define SP_DELETE 			(0)
/*	0 �Ȃ�Δ��ʏ������ȗ��ł���B -1 ���ƈ�X���ʏ���������
	KETM�ł͔��ʂ��Ȃ��ƂȂ�Ȃ��ꏊ�ɔ��ʏ����������ĂȂ��ׁA
	�X�v���C�g�����������Ȃ�o�O�����������ɂ���B */

/* �Ǘ��������Ⴄ�̂ŁA�P�Ȃ�OFF */
#define FIX_OBJ_OFF 			(SP_DELETE)

/*
	�����͎�ޕʂł͂Ȃ��āA�@�\�ʂɕ��ނ��Ă��������B
*/
#define SP_GROUP_SHOT_ZAKO					(0x0000)/*0x0100*/
#define SP_GROUP_SHOT_SPECIAL				(0x0080)/*0x0100*/		/* �얲�̉�]�V���b�g */		/* kodomo_hlaser�̔��� */
//
#define SP_GROUP_JIKI_GET_ITEM				(0x0100)/*0x0100*/		/* === Gu�v���C���[�� === */
//#define dummy_SP_GROUP_PLAYER_CORE		(0x0200)/*0x0100*/
#define SP_GROUP_TEKI						(0x0200)/*0x0400*/		/* === Gu�G�� === */
//#define SP_GROUP_BOSS 					(0x0800)/*0x0400*/		/* === Gu�G�� === */
//#define SP_GROUP_TEKI 					(0x0400)/*0x0400*/
//#define SP_GROUP_TEKI 					(SP_GROUP_TEKI|0)
//#define SP_GROUP_BOSS 					(SP_GROUP_TEKI|SP_GROUP_SHOT_SPECIAL)
#define SP_GROUP_BULLETS					(0x0400)/*(0x1000)0x0800*/		/* === Gu�e���� === */
#define SP_GROUP_ITEMS						(0x0800)/*(0x2000)0x1000*/
//#define SP_GROUP_PAUSE_S P_ME NU_TEXT 	(0x4000)/*0x2000*/		/*SDL*/
/*???*/#define SP_GROUP07_FRONT 				(0x1000)/*(0x8000)*/				/* === Gu�t�����g�� === */

//#define SP_GROUP_ALL_SDL_TYPE (0xff00)
#define SP_GROUP_ALL_SDL_WORK_TYPE			(0x1f00)/* SDL���� */
//#define SP_GROUP_ALL_SDL_CORE_TYPE		((0x1f00) & (~(SP_GROUP_PAUSE_S P_ME NU_TEXT|0)))/* game_core���� */
#define SP_GROUP_ALL_SDL_CORE_TYPE			(0x1f00)/* game_core���� */

//#define SP_GROUP_ALL_SDL_DRAW_TYPE		(SP_GROUP_ALL_SDL_WORK_TYPE & (~(SP_GROUP_TEKI|SP_GROUP_ITEMS|SP_GROUP_BULLETS)))/*SDL�\��*/
/* �G���A�C�e�����G�e�͏��Ȃ��Ƃ�SDL�ŕ`���Ȃ� */
//#define SP_GROUP_ALL_SDL_DRAW_TYPE		(0x4000)/* SDL�\��(����SP_GROUP_PAUSE_S P_ME NU_TEXT�̂�SDL�`��) */

//#define SP_GROUP_SUB_TYPE_256 			(0x00ff)
#define SP_GROUP_SUB_TYPE_128				(0x007f)
/* �ȉ������蔻��Ȃ� */
/*???*/ 	//#define SP_GROUP_MAHOU_JIN			(0x2000)/*(0x00010000)*/			/* === Gu�����w�� === */
/*???*/ 	//#define SP_GROUP_PANEL				(0x4000)/*(0x00020000)*/			/* === Gu�p�l���� === */
//#define SP_FLAG_COLISION_CHECK			(0x8000)/*(0x00020000)*/			/* === Gu�p�l���� === */

#define SP_GROUP_ALL_GAME_OBJS				(0x00ffff00)			/* �Q�[���R�A�I�����ɂ����Â���obj */
//#define SP_GROUP_SHOTS					(0x0200)
//#define SP_GROUP07_FRONT						(0x4000)
//#define SP_GROUP_ENEMYS					(0x0200)/*(0x0400)*/
/* ���p */
//#define SP_GROUP_TEXTS					(SP_GROUP_PLAYER)/*(0x2000)*/

/* SP_GROUP07_FRONT == SP_GROUP07_FRONT */
//#define SP_GROUP07_FRONT						(SP_GROUP07_FRONT)/* �t�����g�� */

		//���E�F�|���A�C�e��(��)	//�E�F�|���A�C�e��(��)��SP_ITEM_EXTRA_SHIELD,
		//�������͒ᑬ�{���ɋz�����ꂽ�BSP_ITEM_EXTRA_HOMING, SP_ITEM_EXTRA_HLASER,
/*	����A */
//	S P_PLAYER_ITEM/*�v���C���[�̖{��(�����蔻�肪�A�C�e���擾)*/			= /*0x0100*/SP_GROUP_JIKI_GET_ITEM, /* Unser Held */
/*	���e */
//	S P_SHOT_BOSS,/*�v���C���[�̃{�X�ɗL���e*/							//	= /*0x0200*/SP_GROUP_SHOTS, 	/* Waffen des Helden */
//	S P_SHOT_ZAKO,/*�v���C���[�̃{�X�ɖ����e*/
/*	����B */
	//dummy_SP_PLAYER_CORE/*�v���C���[�̃R�A(�����蔻�肪���S����)*/		= /*0x0100*/SP_GROUP_PLAYER_CORE,
/* ------------- */
/*	�U�R�G (�{�����A�̓�����Ńv���C���[���ȂȂ�)  */
//	SP_ZAKO 																= /*0x0400*/SP_GROUP_TEKI,		/* Die Boesen */
/* ------------- */
/*	�{�X�G / ���U�R�G (�{�����A�̓�����Ńv���C���[����)  */
//	S P_CHUU,// /*����G[���^�G]*/ /*S P_ZA KO_YOKAI1*/ 					= /*0x0400*/SP_GROUP_BOSS,		/* [***090325 */
//	S P_BO SS,
/* ------------- */
/*	�G�e */
	//dummy_SP_BULLET														= /*0x0800*/SP_GROUP_BULLETS,	/* ... und ihre Waffen */
/* ------------- */

//enum /*sprite_type*/
//{
/* ------------- */
/*	�|�[�Y���ړ��\�� */
//	S P_ME NU_TEXT	/* ���� */												= /*0x2000*/SP_GROUP_PAUSE_S P_ME NU_TEXT,
/*	���̑� */
//	SP_MUTEKI																= /*0x4000*/SP_GROUP07_FRONT,		/* diverses */
//};

//#define SP_MUTEKI 	SP_GROUP07_FRONT

/* === Gu�G�� === */		/* --- �܂�����ĂȂ� --- */
enum	/* �� �����w�ʃG�t�F�N�g */
{
	TEKI_00_BOSS11			/* �{�X & ���{�X[�E��1] */						= /*0x0400*/SP_GROUP_TEKI,
	TEKI_01_BOSS12, 		/* �{�X & ���{�X[�E��2] */
	TEKI_02_BOSS13, 		/* �{�X & ���{�X[�E��3] */
	TEKI_03_BOSS14, 		/* �{�X & ���{�X[�E��4] */
	TEKI_04_BOSS21, 		/* �{�X & ���{�X[����1] */
	TEKI_05_BOSS22, 		/* �{�X & ���{�X[����2] */
	TEKI_06_BOSS23, 		/* �{�X & ���{�X[����3] */
	TEKI_07_BOSS24, 		/* �{�X & ���{�X[����4] */
//
	TEKI_08_BOSS31, 		/* �{�X & ���{�X[���S1] */
	TEKI_09_BOSS32, 		/* �{�X & ���{�X[���S2] */
	TEKI_10_BOSS33, 		/* �{�X & ���{�X[���S3] */
	TEKI_11_BOSS34, 		/* �{�X & ���{�X[���S4] */
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

//enum	/* �� �����w�ʃG�t�F�N�g */
//{
//	MAHOU_JIN_00_aaa	= /*0x00010000*/SP_GROUP_MAHOU_JIN,
//	MAHOU_JIN_01_bbb,
//	MAHOU_JIN_02_,
//	MAHOU_JIN_03_,
//	MAHOU_JIN_04_,
//	MAHOU_JIN_05_,
//	MAHOU_JIN_06_,
//	MAHOU_JIN_07_,
//};

/* === Gu�t�����g�� === */		/* --- �܂�����ĂȂ� --- */

enum	/* �� �t�����g�ʃG�t�F�N�g */
{	/* �����̂����蔻��ʒu�\���p�R�A(�t�����g�ʃG�t�F�N�g) */
	zzz_JIKI_CORE_00_REIMU_A	= /*0x4000*/SP_GROUP07_FRONT,
	zzz_JIKI_CORE_01_REIMU_B,
	zzz_JIKI_CORE_02_MARISA_A,
	zzz_JIKI_CORE_03_MARISA_B,
	zzz_JIKI_CORE_04_MARISA_C,
	zzz_JIKI_CORE_05_REMILIA,
	zzz_JIKI_CORE_06_CHIRNO,
	zzz_JIKI_CORE_07_YUYUKO,
//	/* CORE�B���p */
	zzz_CORE_HIDE_10_REIMU_A,
	zzz_CORE_HIDE_11_REIMU_B,
	zzz_CORE_HIDE_12_MARISA_A,
	zzz_CORE_HIDE_13_MARISA_B,
	zzz_CORE_HIDE_14_MARISA_C,
	zzz_CORE_HIDE_15_REMILIA,
	zzz_CORE_HIDE_16_CHIRNO,
	zzz_CORE_HIDE_17_YUYUKO,
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
	SP_DUMMY_MUTEKI,//S P_GROUP_ETC_DUMMY_REMILIA,
	SP_FRONT_YUKI,//S P_GROUP_ETC_DUMMY_SLOW_BOMB,		//	SPELL_LOGO_,/* Border Power of Spiritual */ 	//	SPELL_TACHIE_,
	PANEL_STR_MAX,
	PANEL_STR_EASY,
	PANEL_STR_NORMAL,
	PANEL_STR_HARD,
	PANEL_STR_LUNATIC,
//
	PANEL_STR_EXTRA,
	PANEL_STR_TIME,
	PANEL_STR_fps,
	PANEL_STR_0_roman,
	PANEL_STR_1_roman,
	PANEL_STR_2_roman,
	PANEL_STR_3_roman,
	PANEL_STR_4_roman,
//
	PANEL_STR_5_roman,
	PANEL_STR_6_roman,
	PANEL_STR_7_roman,
	PANEL_STR_8_roman,
	PANEL_STR_9_roman,
	PANEL_STR_enemy,
	PANEL_STR_0_kanji,
	PANEL_STR_1_kanji,
//
	PANEL_STR_2_kanji,
	PANEL_STR_3_kanji,
	PANEL_STR_4_kanji,
	PANEL_STR_5_kanji,
	PANEL_STR_6_kanji,
	PANEL_STR_7_kanji,
	PANEL_STR_8_kanji,
	PANEL_STR_9_kanji,
//
};
/*
	SP_DUMMY_MUTEKI == FRONT �ʂȂ̂ł����蔻��Ȃ��B���ꂩ��\������Ȃ��B
	SP_FRONT_YUKI == FRONT �ʂȂ̂ł����蔻��Ȃ��B(�Ƃ肠��������)��݂����ȕ\���B
*/
//#define SP_DUMMY_MUTEKI S P_GROUP_ETC_DUMMY_REMILIA
//#define SP_FRONT_YUKI 	S P_GROUP_ETC_DUMMY_SLOW_BOMB

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
enum/* �I�v�V�����ƃC���^�[���[�u���Ă�̂́A(�I�v�V�����A�j��)�v���O�����̓s�� */
{
// [5]
	JIKI_OPTION_00_00			= SP_GROUP_JIKI_GET_ITEM/*0x0800*/,
	JIKI_OPTION_00_01,
	JIKI_OPTION_00_02,
	JIKI_OPTION_00_03,
	JIKI_OPTION_00_04,
	JIKI_OPTION_00_05,
	JIKI_OPTION_00_06,
	JIKI_OPTION_00_07,

//	�v���C���[�A�j���[�V����
// [1]	���ֈړ��̃A�j���[�V����	[���S������4������3������2������1������1������1��...]
	JIKI_PLAYER_00_LEFT,	/* ����1 */ 	/* �ł��� */
	JIKI_PLAYER_01_LEFT,	/* ����2 */
	JIKI_PLAYER_02_LEFT,	/* ����3 */
	JIKI_PLAYER_03_LEFT,	/* ����4 */ 	/* ����荶 */
//	�J��Ԃ��A�j���[�V����		[���S1�����S2�����S3�����S4�����S1�����S2�����S3�����S4��...]
	JIKI_PLAYER_04_MID, 	/* ���S1 */
	JIKI_PLAYER_05_MID, 	/* ���S2 */
	JIKI_PLAYER_06_MID, 	/* ���S3 */
	JIKI_PLAYER_07_MID, 	/* ���S4 */

// [6]
	JIKI_OPTION_01_00,
	JIKI_OPTION_01_01,
	JIKI_OPTION_01_02,
	JIKI_OPTION_01_03,
	JIKI_OPTION_01_04,
	JIKI_OPTION_01_05,
	JIKI_OPTION_01_06,
	JIKI_OPTION_01_07,

// [2]	�E�ֈړ��̃A�j���[�V����	[���S���E��1���E��2���E��3���E��4���E��4���E��4��...]
	JIKI_PLAYER_08_RIGHT,	/* �E��1 */ 	/* �����E */
	JIKI_PLAYER_09_RIGHT,	/* �E��2 */
	JIKI_PLAYER_10_RIGHT,	/* �E��3 */
	JIKI_PLAYER_11_RIGHT,	/* �E��4 */ 	/* �ł��E */
	JIKI_ETC_00,			/* --- ���g�p --- */
	JIKI_ETC_01,			/* --- ���g�p --- */
	JIKI_ETC_02,			/* --- ���g�p --- */
	JIKI_ETC_03,			/* --- ���g�p --- */

// [7]
	JIKI_OPTION_02_00,
	JIKI_OPTION_02_01,
	JIKI_OPTION_02_02,
	JIKI_OPTION_02_03,
	JIKI_OPTION_02_04,
	JIKI_OPTION_02_05,
	JIKI_OPTION_02_06,
	JIKI_OPTION_02_07,

// [3]
	JIKI_BOMBER_00, 		/* �{���o�[�������̗����G */
	JIKI_BOMBER_01, 		/* �U���{��(�ᑬ�{��)	  */
	JIKI_BOMBER_02, 		/* --- ���g�p --- / ���E�� / ����1 */
	JIKI_BOMBER_03, 		/* --- ���g�p --- / ���E�� / ����2 */
	JIKI_BOMBER_04, 		/* ���D1 / ��1 / �\����1 / ��1 / �` */
	JIKI_BOMBER_05, 		/* ���D2 / ��2 / �\����2 / ��2 / �� */
	JIKI_BOMBER_06, 		/* ���D3 / ��3 / �\����3 / ��3 / �m */
	JIKI_BOMBER_07, 		/* ���D4 / ��4 / �\����4 / ��4 / �H */

// [8]
	JIKI_OPTION_03_00,
	JIKI_OPTION_03_01,
	JIKI_OPTION_03_02,
	JIKI_OPTION_03_03,
	JIKI_OPTION_03_04,
	JIKI_OPTION_03_05,
	JIKI_OPTION_03_06,
	JIKI_OPTION_03_07,

// [4]
	JIKI_SHOT_00,			/* �ԎDC /	/  / ����3 /  */
	JIKI_SHOT_01,			/* �ԎDB /	/  / ����4 /  */
	JIKI_SHOT_02,			/* �ԎDA / ���� / ���� / �X / �� */
	JIKI_SHOT_03,			/* / ��������e1 / ���D1 */
	JIKI_SHOT_04,			/* / ��������e2 / ���D2 */
	JIKI_SHOT_05,			/* / ��������e3 / ���D3 */
	JIKI_SHOT_06,			/* / ��������e4 / ���D4 */
	JIKI_SHOT_07,			/* / ��������e5 / �j�e / �X�e / �E�F�C�u�e / �s���N���e */

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

/* TAMA_GROUIP_08_MASK_0xfff8: 8 �e == 1 �O���[�v�Ƃ��ĊǗ�����ꍇ�ɁA�O���[�v��ʂ�����ׂ̃}�X�N�l */
#define TAMA_GROUIP_08_MASK_0xfff8 (0xfff8)
#define is_tama_grouip08(aaa) (aaa & TAMA_GROUIP_08_MASK_0xfff8)
enum
{
//1
	BULLET_00_HOSI_TEN			= SP_GROUP_BULLETS/*0x0800*/,
	BULLET_MARU10_01_AKA,			//�� BULLET_MARU10_00_FUKA_MIDORI,	/* �[�ΐF*/
	BULLET_MARU10_02_YUKARI,		//�� BULLET_MARU10_01_KI_MIDORI,	/* ���ΐF*/
	BULLET_MARU10_03_AOI,			//�� BULLET_MARU10_02_KIIRO,		/* ���F*/
	BULLET_MARU10_04_MIZU_IRO,		//�� BULLET_MARU10_03_AOI,			/* ��*/
	BULLET_MARU10_05_MIDORI,		//�� BULLET_MARU8_08_dummy,
	BULLET_MARU10_06_KI_IRO,		//�� BULLET_MARU8_09_dummy,
	BULLET_MARU10_07_DAI_DAI,		//�� BULLET_MARU8_10_dummy,
//2
	BULLET_KOME_00_SIRO,			//��	//BULLET_KOME_00_AKA,		BULLET_MARU8_11_dummy,
	BULLET_KOME_01_AKA, 			//��	//BULLET_KOME_01_AOI,		BULLET_KOME_01_AOI,
	BULLET_KOME_02_YUKARI,			//��	//BULLET_KOME_02_AKA,		BULLET_KOME_02_AKA,
	BULLET_KOME_03_AOI, 			//��	//BULLET_KOME_03_YUKARI,	BULLET_KOME_03_YUKARI,
	BULLET_KOME_04_MIZU_IRO,		//��	//BULLET_KOME_04_MIDORI,	BULLET_KOME_04_MIDORI,
	BULLET_KOME_05_MIDORI,			//��	//BULLET_KOME_05_KIIRO, 	BULLET_KOME_05_KIIRO,
	BULLET_KOME_06_KI_IRO,			//��	//BULLET_KOME_06_dummy, 	BULLET_KOME_06_dummy,
	BULLET_KOME_07_DAI_DAI, 		//��	//BULLET_KOME_07_dummy, 	BULLET_KOME_07_dummy,
//3
	BULLET_MARU8_00_SIRO, 			//��	//BULLET_MARU8_00_AKA,		//
	BULLET_MARU8_01_AKA,			//��	//BULLET_MARU8_01_YUKARI,	//
	BULLET_MARU8_02_YUKARI, 		//��	//BULLET_MARU8_02_AOI,		//
	BULLET_MARU8_03_AOI,			//��	//BULLET_MARU8_03_MIDORI,	//
	BULLET_MARU8_04_MIZU_IRO, 		//��	//BULLET_MARU8_04_MIDORI,	//
	BULLET_MARU8_05_MIDORI, 		//��	//BULLET_MARU8_05_MIDORI,	//
	BULLET_MARU8_06_KI_IRO,			//��	//BULLET_MARU8_06_KIIRO,	//
	BULLET_MARU8_07_DAI_DAI,		//��	//BULLET_MARU8_07_AOI,		//
//4
	BULLET_MARU12_00_SIRO,		/*12*/
	BULLET_MARU12_01_AKA,		/*12*/
	BULLET_MARU12_02_AOI,		/*12*/
	BULLET_MARU12_03_MIDORI,	/*12*/
	BULLET_MARU12_04_KIIRO, 	/*12*/	//	BULLET_MARU8_12_dummy,
	BULLET_MARU12_05_DAIDAI,	/*12*/	//	BULLET_MARU8_13_dummy,
	BULLET_CAP16_04_KOME_SIROI, 	/* [���Ēe] */
	BULLET_CAP16_05_TUTU_SIROI, 	/* [������e] */
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
	BULLET_OODAMA32_00_AOI, 	/*�F*/
	BULLET_OODAMA32_01_AKA, 	/*�ԐF*/
	BULLET_OODAMA32_02_KIIRO,	/*���F*/
	BULLET_OODAMA32_03_MIDORI,	/*�ΐF*/
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

/*---------------------------------------------------------
	�I�u�W�F�o���N�V�X�e��
--------------------------------------------------------- */

/* �Ǘ�����I�u�W�F�N�g�o���N */
enum
{
	OBJ_BANK_00_TAMA = 0,		// �G�e�p�o���N			//  ��1.�G�e/�A�C�e���p�e�N�X�`���͋��p
	OBJ_BANK_01_ITEM,			// �A�C�e���p�o���N		//  ��1.�G�e/�A�C�e���p�e�N�X�`���͋��p
	OBJ_BANK_02_FRONT_BANK0,	// �t�����g�ʗp�o���N	//  ��2.�t�����g/�p�l���p�e�N�X�`���͋��p
//	OBJ_BANK_07_FRONT_BANK1,	// �p�o���N
//	OBJ_BANK_07_FRONT_BANK2,	// �p�o���N
	OBJ_BANK_03_TITLE_dummy,	/*[�\��]*/	// �^�C�g����ʗp�o���N	//  ��2.�t�����g/�p�l���p�e�N�X�`���͋��p
//	OBJ_BANK_09_EFFECT, 		/*[�\��]*/	// �p�o���N
//
	OBJ_BANK_01_REIMU_A,	// ���@�p�o���N()
	OBJ_BANK_02_REIMU_B,	// ���@�p�o���N()
	OBJ_BANK_03_MARISA_A,	// ���@�p�o���N()
	OBJ_BANK_04_MARISA_B,	// ���@�p�o���N()
	OBJ_BANK_05_REMILIA,	// ���@�p�o���N()
	OBJ_BANK_06_YUYUKO, 	// ���@�p�o���N()
	OBJ_BANK_07_CIRNO_A,	// ���@�p�o���N()
	OBJ_BANK_08_CIRNO_Q,	// ���@�p�o���N()
//
	OBJ_BANK_21_BOSS_STAGE1,	// �{�X�p�o���N(�A���X)
	OBJ_BANK_22_BOSS_STAGE2,	// �{�X�p�o���N(����)
	OBJ_BANK_23_BOSS_STAGE3,	// �{�X�p�o���N(�P��)
	OBJ_BANK_24_BOSS_STAGE4,	// �{�X�p�o���N(��)
	OBJ_BANK_25_BOSS_STAGE5,	// �{�X�p�o���N(�p�`�����[)
	OBJ_BANK_26_BOSS_STAGE6,	// �{�X�p�o���N(���)
	OBJ_BANK_11_ZAKO_STAGE1,	// �U�R�p�o���N 		// OBJ_BANK_27_BOSS_STAGE7_dummy,
	OBJ_BANK_28_BOSS_STAGE8,
//
//	OBJ_BANK_11_ZAKO_STAGE1,
//	OBJ_BANK_12_ZAKO_STAGE2,
//	OBJ_BANK_13_ZAKO_STAGE3,
//	OBJ_BANK_14_ZAKO_STAGE4,
//	OBJ_BANK_15_ZAKO_STAGE5,
//	OBJ_BANK_16_ZAKO_STAGE6,
//	OBJ_BANK_17_ZAKO_STAGE7_dummy,
//	OBJ_BANK_18_ZAKO_STAGE8,
//
	OBJ_BANK_MAX
};
#define 	OBJ_BANK_SIZE			(8*8)
#define 	OBJ_BANK_SIZE_00_TAMA	(OBJ_BANK_00_TAMA*OBJ_BANK_SIZE)
#define 	OBJ_BANK_SIZE_01_ITEM	(OBJ_BANK_01_ITEM*OBJ_BANK_SIZE)

/*---------------------------------------------------------
	�X�v���C�g
--------------------------------------------------------- */


/*-------------------*/
/* �j�e�Ƃ����邩��A���ݐ}�`���肵�Ȃ��ƍ��\����ɂȂ�B */
/* �����蔻�薳�����Ă����x�͕ς��Ȃ������������ߖ�ł���̂ŏ����I�ɂ͂��̕ӂ����ǂ������B */
//#define USE_ZUKEI_ATARI_HANTEI (1)
//#define USE_ZUKEI_ATARI_HANTEI (0)

typedef struct /*_point256*/
{
	Uint16 x;	/*	union POINT */	/* x �ʒu */
	Uint16 y;	/*	union POINT */	/* y �ʒu */
} POINT_u16;
/* u16���W */

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
*/

#define tmp_angleCCW65536 tmp_angleCCW1024
typedef struct _sprite
{
	int cx256;						/*	union POINT */			/* center x �\�� ���S���W�ʒu  (256�Œ菬���_�`��) / akt. Position */
	int cy256;						/*	union POINT */			/* center y �\�� ���S���W�ʒu  (256�Œ菬���_�`��) / akt. Position */
	int vx256;						/*	union POINT_VECTOR */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
	int vy256;						/*	union POINT_VECTOR */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
//[4]
	int rotationCCW1024;			/* �`��p�p�x(����0�x�ō����(�����v���), �����1024����, 0-1023�x) */
	int tmp_angleCCW1024;			/* �ێ��p�p�x(����0�x�ō����(�����v���), �����1024����, 0-1023�x) */
	int user00; 					/* user00 ENEMY_BASE base; */	//int health;	int time_out;		/* user03 */
	int user01; 					/* user01 ENEMY_BASE base; */	//int score;	int state;			/* user02 */
//[8]
	int type;						/* ��ދy�юg�p�� / Sprite-Type, (siehe enum SPRITE_TYPE), 0 = remove. */
	u32 color32;					/* Gu color AGBR8888 MAKE32RGBA(RED, GREEN, BLUE, ALPHA)�Ŋe�v�f 0 ���� 255 (0x00 ���� 0xff)�܂ŁB */
	int m_Hit256R;					/* �����蔻��p */
	#if (1==USE_ZOOM_XY)
	int m_zoom_x256;				/* �\���g�嗦x */
	int m_zoom_y256;				/* �\���g�嗦y */
	#else //(0==USE_ZOOM_XY)
	int m_zoom_xy256;				/* �\���g�嗦xy */
	#endif/* (1==USE_ZOOM_XY) */
//[12]
	/* �Ƃ肠�����ڍs�p(1)�]���݊�(�e) */
	int user_data00;
	int user_data01;
	int user_data02;
	int user_data03;
//[16]
	int user_data04;
	int user_data05;
	int user_data06;
	int user_data07;
//[20]
	/* �Ƃ肠�����ڍs�p(2)�ǉ��e�A��{�@�\ */
	int user_data10;
	int user_data11;
	int user_data12;
	int user_data13;
//
	void (*callback_loser)(struct _sprite *src);	/* Custom-loser-Routine (���ꂽ��Ƀ{�[�i�X���o���Ƃ��A�Ⴄ��������Ƃ�) / ���@�I�v�V�����̃A�j���[�V�����R�[���o�b�N */
	void (*callback_mover)(struct _sprite *src);	/* Custom-Move-Routine */
	void (*callback_hit_enemy)(struct _sprite *c, struct _sprite *src); 	// [***090325		�ǉ�
	struct _sprite *target_obj; 		/* �Ƃ肠�����ڍs�p(3)�ڍs������Ȃ��Ȃ�B */
//[20]
// �`��p
	int w;
	int h;
	int tx; 	/* u �e�N�X�`�����W */
	int ty; 	/* v �e�N�X�`�����W */
//
	/*u8*/u32 flags;					/* �t���O�Z�b�g / siehe unten (SP_FLAG...) */
	s32 jyumyou;	/* ���� */
//	u32 dummy_used;/*???*/
//	int dummy111;/* �g���ĂȂ��_�~�[ */
} SPRITE;

/*---------------------------------------------------------
	jyumyou ���� life_cycle sprite_time_out sprite_longevity
	-------------------------------------------------------
	���O�͖Y�ꂽ���ǁAKETM�ɂ������l�����B
	��x�͔r���������A�����������x����������B
	�������A�V�X�e���ŋ����I�ɒl�����Z����B
	�X�v���C�g�����������ƁA�l�� 1�B
	�X�v���C�g���t���[����������x�ɁA1 ���Z�����B
	-------------------------------------------------------
	psp�� 0 ���W�X�^������̂ŁA0 �����A
	�܂蕉���� ���������B
	-------------------------------------------------------
	int ����Ȃ��� short �ɂ���āB
	0x7fff==32767 �͐��������A1���Z����� signed short �̏ꍇ�A
	0x8000==32768==-32768==�����ɂȂ�̂ŁA�����ɂȂ����玩�����������B
---------------------------------------------------------*/

/*	struct _sprite *target_obj;
	�p�r��(
		player�p�U���e�A
		���̖����w"�d��2"(��)
	)
	�ق�Ƃ͕��G�ɃL���X�g����Ηv��Ȃ��񂾂��ǃo�O�ł��������B
 */


//#ifndef GRP_SCREEN_H
//#define GRP_SCREEN_H
/*	@since		Jul.27.2005 GRP_SCREEN_H		��ʕ\���Ǘ� */



/*---------------------------------------------------------
	���\�[�X resource
---------------------------------------------------------*/

extern SPRITE *sprite_add_444only_bullet_error(void);				/* gu�ڍs���A�e��p 				int bullet_type_num */
extern SPRITE *sprite_add_gu_error(void);							/* gu�ڍs���A�ėp�X�v���C�g 		int bullet_type_num */
extern SPRITE *sprite_add_direct(unsigned int register_number); 	/* ��p�Œ�X�v���C�g */

extern void sprite_all_cleanup(void);
extern void sprite_move_all(void);	/* �X�v���C�g�I�u�W�F�N�g�̈ړ����� */

//extern void sprite_display000(int type);/**/
//extern void sprite_display222(int type);/*�e���p*/
extern void pause_sprite_display(void);/**/

extern SPRITE *sprite_collision_check_SDL_teki(SPRITE *tocheck);/*�G��p*/	//, int type);
extern SPRITE *sprite_collision_check_444(SPRITE *tocheck, int type);/*gu�e����p*/

extern void gamen_gai_nara_zako_osimai(SPRITE *src);/* ��ʊO�Ȃ炨���܂� */
extern void check_boss_option_time_out(SPRITE *src);/* */

#if 1
/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
/*��ʊO�Ȃ�e������*/
extern void bullet_angle_all_gamen_gai_nara_kesu(void);

/*��ʊO�Ȃ�e�𔽎�*/
extern void bullet_angle_all_gamen_gai_nara_hansya(void);		/* ���˒e�̂Ă��� */

/*��ʊO�Ȃ�e�𔽎ˌ���*/
extern void bullet_angle_all_gamen_gai_nara_hansya_gensoku(void);	/* ���ˌ����e�̂Ă��� */
#endif

/* �g�p���ł��邪�ޔ��������ꍇ�B��ʊO�ɃX�v���C�g���ړ������A�����ɂ���B */
extern void sprite_initialize_position(SPRITE *src);

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



#endif/* _SPRITE_H_ */



#ifndef _BULLET_SYSTEM_H_
#define _BULLET_SYSTEM_H_

/*---------------------------------------------------------
	�G�e�Ǘ��V�X�e��
---------------------------------------------------------*/

//#if 000/* �ڍs���ɂ����� */
//	/* MAX_POOL_BULLET: 2��n��̕K�v����(1024�Ƃ�2048�Ƃ�) */
//#define MAX_POOL_BULLET 1024
//extern TGameSprite bullet_pool[MAX_POOL_BULLET];
//
//extern void bullet_system_init(void);
//extern void bullet_system_exit(void);
//#endif /* 000 �ڍs���ɂ����� */




#if 1
/*---------------------------------------------------------
	�V�A�G�e�Ǘ��V�X�e��(��)
---------------------------------------------------------*/

enum
{
	PANEL_OBJ_00_S00 = 0,	/* �X�R�A10���� */
	PANEL_OBJ_01_S01,		/* �X�R�A9���� */
	PANEL_OBJ_02_S02,		/* �X�R�A8���� */
	PANEL_OBJ_03_S03,		/* �X�R�A7���� */
	PANEL_OBJ_04_S04,		/* �X�R�A6���� */
	PANEL_OBJ_05_S05,		/* �X�R�A5���� */
	PANEL_OBJ_06_S06,		/* �X�R�A4���� */
	PANEL_OBJ_07_S07,		/* �X�R�A3���� */
	PANEL_OBJ_08_S08,		/* �X�R�A2���� */
	PANEL_OBJ_09_S09,		/* �X�R�A1���� */
	PANEL_OBJ_0a_P00,		/* �p���[1���� */
	PANEL_OBJ_0b_P01,		/* �p���[�����_1���� */
	PANEL_OBJ_0c_P02,		/* �p���[�����_2���� */
	PANEL_OBJ_0d_Time,		/* "Time:" */
	PANEL_OBJ_0e_P0p,		/* �p���['.' */
	PANEL_OBJ_0f_NAN_IDO,	/* ��Փx�\�� "Easy", "Normal", "Hard", "Lunatic", */
	//
	PANEL_OBJ_10_H00,		/* �n�C�X�R�A10���� */
	PANEL_OBJ_11_H01,		/* �n�C�X�R�A9���� */
	PANEL_OBJ_12_H02,		/* �n�C�X�R�A8���� */
	PANEL_OBJ_13_H03,		/* �n�C�X�R�A7���� */
	PANEL_OBJ_14_H04,		/* �n�C�X�R�A6���� */
	PANEL_OBJ_15_H05,		/* �n�C�X�R�A5���� */
	PANEL_OBJ_16_H06,		/* �n�C�X�R�A4���� */
	PANEL_OBJ_17_H07,		/* �n�C�X�R�A3���� */
	PANEL_OBJ_18_H08,		/* �n�C�X�R�A2���� */
	PANEL_OBJ_19_H09,		/* �n�C�X�R�A1���� */
	PANEL_OBJ_1a_fps00, 	/* fps2���� */
	PANEL_OBJ_1b_fps01, 	/* fps1���� */
	PANEL_OBJ_1c_fps02, 	/* fps�����_1���� */
	PANEL_OBJ_1d_fps03, 	/* fps�����_2���� */
	PANEL_OBJ_1e_fps0p, 	/* fps '.' */
	PANEL_OBJ_1f_fps_str,	/* "fps" */
	//
	PANEL_OBJ_20_N00,		/* �c��K�b�c8���� */
	PANEL_OBJ_21_N01,		/* �c��K�b�c7���� */
	PANEL_OBJ_22_N02,		/* �c��K�b�c6���� */
	PANEL_OBJ_23_N03,		/* �c��K�b�c5���� */
	PANEL_OBJ_24_N04,		/* �c��K�b�c4���� */
	PANEL_OBJ_25_N05,		/* �c��K�b�c3���� */
	PANEL_OBJ_26_N06,		/* �c��K�b�c2���� */
	PANEL_OBJ_27_N07,		/* �c��K�b�c1���� */
	PANEL_OBJ_28_B00,		/* �{��8���� */
	PANEL_OBJ_29_B01,		/* �{��7���� */
	PANEL_OBJ_2a_B02,		/* �{��6���� */
	PANEL_OBJ_2b_B03,		/* �{��5���� */
	PANEL_OBJ_2c_B04,		/* �{��4���� */
	PANEL_OBJ_2d_B05,		/* �{��3���� */
	PANEL_OBJ_2e_B06,		/* �{��2���� */
	PANEL_OBJ_2f_B07,		/* �{��1���� */
	//
	PANEL_OBJ_30_J00,		/* �c�莞��3���� */
	PANEL_OBJ_30_J01,		/* �c�莞��2���� */
	PANEL_OBJ_30_J02,		/* �c�莞��1���� */
	PANEL_OBJ_31_G03,		/* �O���C�Y5���� */
	PANEL_OBJ_31_G04,		/* �O���C�Y4���� */
	PANEL_OBJ_31_G05,		/* �O���C�Y3���� */
	PANEL_OBJ_32_G06,		/* �O���C�Y2���� */
	PANEL_OBJ_33_G07,		/* �O���C�Y1���� */
	//
	SPRITE_222POOL_MAX/* MAX */
};
//enum /*_common_my_obj_*/
//{
//	MY_O BJ_00_JIKI = 0,		/*[r32�ύX�ς�]*/
//	MY_O BJ_01_JIKI_MARU,		/**/		/*[r32�ύX�ς�]*/
//	MY_O BJ_02_JIKI_OPT01,		/**/		/*[r32�ύX�ς�]*/
//	MY_O BJ_03_JIKI_OPT02,		/**/		/*[r32�ύX�ς�]*/
//	MY_O BJ_04_JIKI_OPT03,		/**/		/*[r32�ύX�ς�]*/
//	MY_O BJ_05_JIKI_OPT04,		/**/		/*[r32�ύX�ς�]*/
//	MY_O BJ_06_SEND1,			/*[�\��]*/
//	MY_O BJ_07_,				/*[�\��]*/
//
//	MY_O BJ_08_BOSS,			/*[�\��]*/
//	MY_O BJ_09_,				/*[�\��]*/
//	MY_O BJ_0a_,				/*[�\��]*/
//	MY_O BJ_0b_EFFECT01,		/*[�\��]*/
//	MY_O BJ_0c_EFFECT02,		/*[�\��]*/
//	MY_O BJ_0d_EFFECT03,		/*[�\��]*/
//	MY_O BJ_0e_EFFECT04,		/*[�\��]*/
//	MY_O BJ_0f_EFFECT05,		/*[�\��]*/
//
//	MY_O BJ_99_MAX
//};
enum
{
	/* �`��v���C�I���e�B�[�����@�e�N�X�`���[�̂��� */
	FIX_OBJ_00_PLAYER = 0,		/*<�g�p��>[r32�ύX�ς�] sprite_add_direct(FIX_OBJ_00_PLAYER); */
	FIX_OBJ_01_JIKI_MARU,		/*<�g�p��>[r32�ύX�ς�] sprite_add_direct(FIX_OBJ_01_JIKI_MARU); */
	FIX_O_BJ_02_SEND1,			/*[�\��]*/
	FIX_O_BJ_03_SEND2,			/*[�\��]*/
	FIX_OBJ_04_JIKI_OPTION0,	/*<�g�p��>[r32�ύX�ς�] sprite_add_direct(FIX_OBJ_04_JIKI_OPTION0+jj); */
	FIX_OBJ_05_JIKI_OPTION1,	/*<�g�p��>[r32�ύX�ς�]*/
	FIX_OBJ_06_JIKI_OPTION2,	/*<�g�p��>[r32�ύX�ς�]*/
	FIX_OBJ_07_JIKI_OPTION3 ,	/*<�g�p��>[r32�ύX�ς�]*/
//
	/* �`��v���C�I���e�B�[��front�e�N�X�`���[�̂��� */
//	xxx_FIX_OBJ_08_BOSS,		/* <�����蔻��̓s����o���Ȃ�> */	/*<�g�p��>[r32�ύX�ς�] sprite_add_direct(FIX_OBJ_08_BOSS); */
//	FIX_O_BJ_09_,				/*[�\��]*/
//	FIX_O_BJ_10_,				/*[�\��]*/
//	FIX_OBJ_11_EFFECT01,				/*<�g�p��>*/
//	FIX_O_BJ_12_SUPPORT0,		/*[�\��]*/		/*<�g�p��>*/
//	FIX_O_BJ_13_SUPPORT1,		/*[�\��]*/		/*<�g�p��>*/
//	FIX_O_BJ_14_SUPPORT2,		/*[�\��]*/		/*<�g�p��>*/
//	FIX_O_BJ_15_SUPPORT3,		/*[�\��]*/		/*<�g�p��>*/
	FIX_OBJ_08_EFFECT01,		/*[�\��]*/		/*<�g�p��>*/
	FIX_OBJ_09_EFFECT02,		/*[�\��]*/		/*<�g�p��>*/
	FIX_OBJ_10_EFFECT03,		/*[�\��]*/		/*<�g�p��>*/
	FIX_OBJ_11_EFFECT04,		/*[�\��]*/		/*<�g�p��>*/
	FIX_OBJ_12_EFFECT05,		/*[�\��]*/		/*<�g�p��>*/
	FIX_OBJ_13, 	/*[�\��]*/
	FIX_OBJ_14, 	/*[�\��]*/
	FIX_OBJ_15_JIKI_TEISOKU_EFFECT, 	/*[�\��]*/				/*<�g�p��>*/
	SPRITE_111POOL_MAX/*MAX*/
};
	/* �`��v���C�I���e�B�[�����@�e�N�X�`���[�̂��� */
/* -- �v���C�I���e�B�[���U�D���@��`�� */
//#define SPRITE_111JIKI_POOL_MAX FIX_OBJ_07_JIKI_OPTION3

/*
 �Q�l: kouma_tr013/�����g����/�}�j���A��/html/faq.html
�p�Q�R�@�A�C�e���̕\�����E�́H

�@��ʓ��T�P�Q�܂łł��B
�@�G�e�̐����U�S�O�Ȃ̂ŁA�S�Ă��{�[�i�X�A�C�e���ɂȂ�Ƃ͌���܂��񂪁A
�@�w�ǂ̏ꍇ�̓X�R�A�p�^�[���ɉe������悤�Ȃ��Ƃ͂���܂���B

*/

/* �e�̍ő吔�� SPRITE_444POOL_MAX (==1024) ����ȏ�o�^���悤�Ƃ��Ă��o�^����Ȃ��B */
//#define SPRITE_444POOL_MAX		(512)/* �Ƃ肠���� */
#define SPRITE_444POOL_MAX		(1024)/* �e�o�^�v���O�����̓s���� 2^n �ł���K�v������܂��B(1024==(1<<10) ) */
extern SPRITE obj44[SPRITE_444POOL_MAX];	/* �e��p�X�v���C�g�̃��X�g�\�� */

/* �G(���@�A�I�v�V�����A���e���A�e�ȊO�̑S�ẴX�v���C�g�܂�)�̍ő吔�� SPRITE_333POOL_MAX (==256) ����ȏ�o�^���悤�Ƃ��Ă��o�^����Ȃ��B */
#define SPRITE_333POOL_MAX		(256)/* �G�o�^�v���O�����̓s���� 2^n �ł���K�v������܂��B(1024==(1<<10) ) */
extern SPRITE obj33[SPRITE_333POOL_MAX];	/* �G��p�X�v���C�g�̃��X�g�\�� */

extern SPRITE obj00[SPRITE_111POOL_MAX];	/* �G��p�X�v���C�g�̃��X�g�\�� */

/* �p�l���p�X�v���C�g�� SPRITE_222POOL_MAX (==256) ����ȏ�o�^���悤�Ƃ��Ă��o�^����Ȃ��B */
//#define SPRITE_222POOL_MAX		(256)/* �G�o�^�v���O�����̓s���� 2^n �ł���K�v������܂��B(1024==(1<<10) ) */
extern SPRITE obj22[SPRITE_222POOL_MAX];	/* �p�l���p�X�v���C�g�̃��X�g�\�� */


/*---------------------------------------------------------
	�I�u�W�F�o���N�V�X�e��
--------------------------------------------------------- */
extern void reflect_sprite_spec444(SPRITE *sss, unsigned int bank_offset);	/* �I�u�W�F�N�g�̃o���N�ݒ�𔽉f������B */
#endif

#endif /* _BULLET_SYSTEM_H_ */
