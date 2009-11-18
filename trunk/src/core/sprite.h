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
	BASE_BOMBER_ATARI_0a,/*4*/
	BASE_BOMBER_ATARI_0b,/*16*/
	BASE_BOMBER_ATARI_0c,/*16*/
	BASE_BOMBER_ATARI_0d,/*32*/
	BASE_BOMBER_ATARI_0e,/*4*/
//
	BASE_SHOT_ATARI_0a,/*4*/
	BASE_SHOT_ATARI_0b,/*4*/
	BASE_SHOT_ATARI_0c,/*4*/
	BASE_SHOT_ATARI_0d,/*6*/
	BASE_SHOT_ATARI_0e,/*8*/
//
	BASE_NEEDLE_ATARI_0a,/*6*/
	BASE_NEEDLE_ATARI_0b,/*8*/
	BASE_NEEDLE_ATARI_0c,/*12*/
	BASE_NEEDLE_ATARI_0d,/*16*/
	BASE_NEEDLE_ATARI_0e,/*8*/
//
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
#endif

enum
{
	/* �� BG�ʃG�t�F�N�g */
	BASE_TIKEI_BGPANEL1_PNG = 0,
	BASE_TIKEI_BGPANEL2_PNG,
//	/* �� SHOT�ʃG�t�F�N�g */
//
//	BASE_BOMBER_SLOW_PNG,
	BASE_MAHOUJIN_0_PNG,
//
	BASE_TIKEI_GROUNDER08_PNG,
//	/* �� �U�R�ʃG�t�F�N�g */
//	/* �U�R(�U�R�ʃG�t�F�N�g) */
	BASE_OBAKE08_PNG,
	BASE_YUKARI32_PNG,
	BASE_AKA_KEDAMA08_PNG,
	BASE_NIJI_KEDAMA16_PNG,
//
	BASE_MIDOORI_KEDAMA16_PNG,
	BASE_KEDAMA16_PNG,
	BASE_INYOU1_16_PNG,
	BASE_TATSUMAKI16_PNG,
//
	BASE_GREAT_FAIRY02_PNG,
	BASE_AKA_MEIDO08_PNG,
	BASE_AO_YOUSEI24_PNG,
//
//	/* �{�X(�U�R�ʃG�t�F�N�g) */
//	BASE_BOSS_ALICE_0_PNG,
//	BASE_BOSS_ALICE_1_PNG,
//	BASE_BOSS_ALICE_2_PNG,
//	BASE_BOSS_ALICE_3_PNG,
//	BASE_BOSS_ALICE_4_PNG,
	BASE_BOSS_ALICE_PNG,
	BASE_BOSS_ALICE_DOLLS_PNG,
	BASE_BOSS_PACHE_PNG,
	BASE_BOSS_PACHE_DOLLS_PNG,
//
	BASE_BOSS_AYA_PNG,
//
	BASE_BOSS_KAGUYA_PNG,
//	BASE_BOSS_KAGUYA_0_PNG,
//	BASE_BOSS_KAGUYA_1_PNG,
//	BASE_BOSS_KAGUYA_2_PNG,
//	BASE_BOSS_KAGUYA_3_PNG,
//	BASE_BOSS_KAGUYA_4_PNG,
//	BASE_BOSS_KAGUYA_5_PNG,
//
	BASE_BOSS_SAKUYA_PNG,
//
	//BASE_KEY_ICON_PNG,
//
	/* �� �A�C�e���ʃG�t�F�N�g */
//	/* ���̑��̎��@�f��(�A�C�e���ʃG�t�F�N�g) */
	BASE_BONUS_ITEMS_PNG,
//
	/* �� �e���ʃG�t�F�N�g */
//
	/* �� �t�����g�ʃG�t�F�N�g */
//	/* �����̂����蔻��ʒu�\���p�R�A(�t�����g�ʃG�t�F�N�g) */
	BASE_CORE_PNG,
	BASE_CORE_PNG_ma,
	BASE_CORE_PNG_oz,
	BASE_CORE_PNG_ci,
	BASE_CORE_PNG_yu,
//
	#if 000
//	/* ������(�t�����g�ʃG�t�F�N�g) */
	BASE_TR_BLUE_PNG,
	BASE_TR_RED_PNG,
	BASE_TR_GREEN_PNG,
//	/* �U�R���Ŕ���(�t�����g�ʃG�t�F�N�g) */
	BASE_BAKUHA05_PNG,
	BASE_BAKUHA06_PNG,
	BASE_BAKUHA07_PNG,
//	/* �Ή�����(�t�����g�ʃG�t�F�N�g) */
	BASE_BAKUHA_EX_PNG,
	#endif //000
	/* �{���o�[�e���b�v���S(�t�����g�ʃG�t�F�N�g) */

	/* /[�R���e�B�j���[����(bank00)/���j���[����(bank01)/���j���[����(bank02)] */

	FILE_RESOURCE_MAX/*�ő吔*/
};

// �e���ɂ���
// �e��(�G�e�̂ݎ��e�͊܂܂Ȃ�)��Gu�ŕ`�������v�撆(�Ƃ������ϑz��)�B
// ���̏ꍇ�ASDL��ʕ`����Gu�ŕ`���̂ŁA
// �e���̃v���C�I���e�B�[�͍őO�ʂɂȂ�B
//
// �g�������Q�l�Ɍ������A���@�����A�C�e������
// �e���͑O(�܂肠�ƂœZ�߂ĕ`�悵�Ă���)������
//
//���������͈Ⴄ��������Ȃ����A�����{�Ƃ͂���Ȋ���(�z��)
// [�w�i�֘A]
// �P�D���z���(448x448)��3D�w�i�������_�����O(��ݏo���`��)
// [���@�֘A]
// �Q�D���z���(448x448)�Ɏ��e�������_�����O(��ݏo���`��)					(���e�͎��@��艺)
// �R�D���z���(448x448)�Ɏ��@�I�v�V�����������_�����O(��ݏo���`��)		(���@�^���@�I�v�V�����̓{�X�^�G��艺)
// �S�D���z���(448x448)�Ɏ��@�������_�����O(��ݏo���`��)					(���@�^���@�I�v�V�����̓{�X�^�G��艺)
// [�G�֘A]
// �T�D���z���(448x448)�ɓG��{�X�������_�����O(��ݏo���`��)
// [�A�C�e���֘A]
// �U�D���z���(448x448)�ɃA�C�e���������_�����O(��ݏo���`��)				(�A�C�e���͓G�e��艺/�A�C�e���͓G����)
// �X�D���z���(448x448)�Ɋ����X�R�A�Ȃǂ������_�����O(��ݏo���`��)		(�����X�R�A�͓G�e��艺(???))
// [�G�e�֘A]
// �V�D���z���(448x448)�Ƀ��[�U�[�����C���`��������_�����O(��ݏo���`��)
// �W�D���z���(448x448)�ɒe���������_�����O(��ݏo���`��)
// [�C�x���g�֘A]
// �X�D���z���(448x448)�ɕ���{�X���[�^�Ȃǂ������_�����O(��ݏo���`��)
// �P�O�D���z���(448x448)�ɃC�x���g�摜�������_�����O(��ݏo���`��)
// �P�P�D���z���(448x448)�ɃC�x���g�����������_�����O(��ݏo���`��)
// �P�Q�D���z���(448x448)�̈ꕔ(384x448)����`�ɐ؂�o���āA�{���(640x480)�ɓ]������B
/*	���F384==12x32�A448==14x32�Ȃ̂œs���悭�����]���\�B */
/*	���F�����̓]���Ŏ����I�ɃN���b�s���O�������ł���B */
// �P�R�D���_�Ȃǂ̕\���͎n�߂���{���(640x480)�ɕ`�悵���z���(448x448)�͎g��Ȃ��B
// �P�S�D�{���(640x480)��VRAM�ɓ]��(���Ԃ�)�B
/*	���F���z���(448x448)�͑������z���(512x512)�B������̕����F�X�������ȗ��ł��āA�s�����悢�B */
//�����������Ƃ������Đ��@�D�܂ň�т��ăQ�[����ʂ�(384x448)����ˁH

/*New r23*/
enum /*_priority_*/
{
// [�w�i�֘A]
//	PR IORITY_00_,			/*Gu���܂�*/	//	P R_BACK0 = 0,			/* ���o�p���V�� */	/* �n�`�R�A�p�l�� */	/* �n�`�p�l���_ */
	PRIORITY_01_SHOT = 0,	/*Gu���܂�?*/	//	PRIORITY_01_SHOTBG	P RIORITY_01_GROUNDER,		/*���e/�n��U�R�G/�����w*/
// [���@�֘A]
	PRIORITY_02_PLAYER, 	/*Gu���r��*/	/* ���@ */
// [�G�֘A]
	PRIORITY_03_ENEMY,		/*Gu���܂�*/	/* �{�X/�U�R�G */
// [�A�C�e���֘A]
	PRIORITY_04_ITEM,		/*Gu���܂�*/	/* �A�C�e��/�����X�R�A */	//P R_PLAYER2,			/*������\��*/
// [�G�e�֘A]
	PRIORITY_05_BULLETS,	/*Gu���r��*/	/*�G�e*/	/*PRIORITY_03_ENEMY_WEAPON*/
// [Gu�t�����g��(Gu�Ŕ������͍őO�ʂɖ����ƕ`��ł��Ȃ��̂ŐV�݂���)]
/*	PRIORITY_06_FRONT,*/	/*Gu���r��*/	/* Gu�̂�(SDL���ł͌��X����) */
};
// �v���C�I���e�B�� Gu �Ɉڍs�����ꍇ�ɁA
// ������������ꍇ�̓n�[�h�E�F�A�[�����ł��Ȃ��̂�
// ���ǃ\�t�g�E�F�A�[�őΉ�(Z�\�[�g)���鎖�ɂȂ�܂��B
// (�����������̏ꍇ��depth-buffer�g���΃n�[�h�E�F�A�[�����ł���B)
// (�v���C�I���e�B��3D�Ƃ��čl�����ꍇ��Z���A�`��Ŋg��k�����͂��Ȃ�)
// ���������āA�Ȃ�ׂ����Ȃ����������A�u���ƂŁv(���x�ʂ�)�L���ł��B
// (���X�g�����͒x���̂ŁA�v���C�I���e�B�ʂ̃o�b�t�@���v���C�I���e�B���p�ӂ���ƁA
// ���v���C�I���e�B�[�ł̓\�[�g����K�v���Ȃ��̂ŁA�\�[�g���ȗ��ł���)


//	P R_BACK1,
//	P R_BACK2,

//
//	P R_TARGET,
//	P R_TMP,
// [�e�L�X�g�\��]
//	PR IORITY_06_TEXT,		/*�e�L�X�g�\��*/
//	P R_FRONT0,
//	P R_FRONT1,
//	P R_FRONT2

#define USER_BOMOUT_WAIT (30)

/* �g�p�ς݂���������Ă���t���O */
#define SP_DELETE				(0)
/*	0 �Ȃ�Δ��ʏ������ȗ��ł���B -1 ���ƈ�X���ʏ���������
	KETM�ł͔��ʂ��Ȃ��ƂȂ�Ȃ��ꏊ�ɔ��ʏ����������ĂȂ��ׁA
	�X�v���C�g�����������Ȃ�o�O�����������ɂ���B */
/*
	�����͎�ޕʂł͂Ȃ��āA�@�\�ʂɕ��ނ��Ă��������B
*/
#define SP_GROUP_SHOT_ZAKO		(0x0000)/*0x0100*/
#define SP_GROUP_SHOT_SPECIAL	(0x0080)/*0x0100*/
//
#define SP_GROUP_JIKI_GET_ITEM	(0x0100)/*0x0100*/		/* === Gu�v���C���[�� === */
#define SP_GROUP_PLAYER_CORE	(0x0200)/*0x0100*/
#define SP_GROUP_ZAKO			(0x0400)/*0x0400*/
#define SP_GROUP_BOSS			(0x0800)/*0x0400*/
//#define SP_GROUP_TEKI 		(0x0400)/*0x0400*/
//#define SP_GROUP_ZAKO 		(SP_GROUP_TEKI|0)
//#define SP_GROUP_BOSS 		(SP_GROUP_TEKI|SP_GROUP_SHOT_SPECIAL)
#define SP_GROUP_BULLETS		(0x1000)/*0x0800*/		/* === Gu�e���� === */
#define SP_GROUP_ITEMS			(0x2000)/*0x1000*/
#define SP_GROUP_PAUSE_OBJS 	(0x4000)/*0x2000*/
#define SP_GROUP_ETC			(0x8000)				/* === Gu�t�����g�� === */
#define SP_GROUP_ALL_SDL_TYPE	(0xff00)
#define SP_GROUP_SUB_TYPE_256	(0x00ff)
#define SP_GROUP_SUB_TYPE_128	(0x007f)
/* �ȉ������蔻��Ȃ� */
#define SP_GROUP_MAHOU_JIN			(0x00010000)			/* === Gu�����w�� === */
#define SP_GROUP_PANEL				(0x00020000)			/* === Gu�p�l���� === */
#define SP_GROUP_ALL_GAME_OBJS		(0x00ffff00)			/* �Q�[���R�A�I�����ɂ����Â���obj */
//#define SP_GROUP_SHOTS		0x0200
//#define SP_GROUP_ETC			0x4000
//#define SP_GROUP_ENEMYS		0x0200/*0x0400 */
/* ���p */
//#define SP_GROUP_TEXTS			(SP_GROUP_PLAYER)/*0x2000 */

/* SP_GROUP_ETC == SP_GROUP_FRONT */

enum /*sprite_type*/
{
/*	����A */
//	S P_PLAYER_ITEM/*�v���C���[�̖{��(�����蔻�肪�A�C�e���擾)*/	= /*0x0100*/SP_GROUP_JIKI_GET_ITEM, /* Unser Held */
/*	���e */
//	S P_SHOT_BOSS,/*�v���C���[�̃{�X�ɗL���e*/					//	= /*0x0200*/SP_GROUP_SHOTS, 	/* Waffen des Helden */
//	S P_SHOT_ZAKO,/*�v���C���[�̃{�X�ɖ����e*/
/*	����B */
	SP_PLAYER_CORE/*�v���C���[�̃R�A(�����蔻�肪���S����)*/		= /*0x0100*/SP_GROUP_PLAYER_CORE,
/* ------------- */
/*	�U�R�G (�{�����A�̓�����Ńv���C���[���ȂȂ�)  */
	SP_ZAKO 														= /*0x0400*/SP_GROUP_ZAKO,		/* Die Boesen */
/* ------------- */
/*	�{�X�G / ���U�R�G (�{�����A�̓�����Ńv���C���[����)  */
	SP_CHUU /*����G[���^�G]*/ /*SP_ZAKO_YOKAI1*/					= /*0x0400*/SP_GROUP_BOSS,		/* [***090325 */
	SP_BOSS,
/* ------------- */
/*	�G�e */
	SP_BULLET														= /*0x0800*/SP_GROUP_BULLETS,	/* ... und ihre Waffen */
/* ------------- */
/*	�A�C�e�� */
	SP_ITEM_00_P001 												= /*0x1000*/SP_GROUP_ITEMS, 	/* Bonus items */
	SP_ITEM_01_P008,	//���E�F�|���A�C�e��(��)	//�E�F�|���A�C�e��(��)��SP_ITEM_EXTRA_SHIELD,
	SP_ITEM_02_BOMB,	//�������͒ᑬ�{���ɋz�����ꂽ�BSP_ITEM_EXTRA_HOMING, SP_ITEM_EXTRA_HLASER,
	SP_ITEM_03_1UP,
	SP_ITEM_04_P128,
	SP_ITEM_05_HOSI,
	SP_ITEM_06_TENSU,
	SP_ITEM_07_SPECIAL,
/* ------------- */
/*	�|�[�Y���ړ��\�� */
	SP_MENU_TEXT	/* ���� */										= /*0x2000*/SP_GROUP_PAUSE_OBJS,
/*	���̑� */
	SP_MUTEKI														= /*0x4000*/SP_GROUP_ETC,		/* diverses */
};


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
	JIKI_CORE_00_REIMU	= /*0x4000*/SP_GROUP_ETC,
	JIKI_CORE_01_MARISA,
	JIKI_CORE_02_,
	JIKI_CORE_03_,
	JIKI_CORE_04_,
	JIKI_CORE_05_,
	JIKI_CORE_06_,
	JIKI_CORE_07_,
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
	SPELL_LOGO_,/* Border Power of Spiritual */
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
	JIKI_ETC_00,			/* ���[�U�[1 */
	JIKI_ETC_01,			/* ���[�U�[2 */
	JIKI_ETC_02,			/* ���[�U�[3 */
	JIKI_ETC_03,			/* ���[�U�[4 */
//
	JIKI_BOMBER_00, 		/* ���[�U�[5 */
	JIKI_BOMBER_01, 		/* ���[�U�[6 */
	JIKI_BOMBER_02, 		/* ��1 / ��1 / �\����1 */
	JIKI_BOMBER_03, 		/* ��2 / ��2 / �\����2 */
	JIKI_BOMBER_04, 		/* ��3 / ��3 / �\����3 */
	JIKI_BOMBER_05, 		/* ��4 / ��4 / �\����4 */
	JIKI_BOMBER_06, 		/* ��5 / ��5 / ����1 / ���E�� */
	JIKI_BOMBER_07, 		/* ��6 / ��6 / ����2 / ���E��  */
//
	JIKI_SHOT_00,			/* ����3 / �ԎDC */
	JIKI_SHOT_01,			/* ����4 / �ԎDB */
	JIKI_SHOT_02,			/* �ԎDA / ���� / ���� / �X / �� */
	JIKI_SHOT_03,			/* ��������e1 / ���D1 */
	JIKI_SHOT_04,			/* ��������e2 / ���D2 */
	JIKI_SHOT_05,			/* ��������e3 / ���D3 */
	JIKI_SHOT_06,			/* ��������e4 / ���D4 */
	JIKI_SHOT_07,			/* ��������e5 / �j�e / �X�e / �E�F�C�u�e / �s���N���e */
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
enum
{
	BULLET_MARU8_00_AKA 				= SP_GROUP_BULLETS/*0x0800*/,
	BULLET_MARU8_01_YUKARI,
	BULLET_MARU8_02_AOI,
	BULLET_MARU8_03_MIDORI,
	BULLET_MARU8_04_MIDORI,
	BULLET_MARU8_05_MIDORI,
	BULLET_MARU8_06_KIIRO,
	BULLET_MARU8_07_AOI,
//
	BULLET_MINI8_00_AKA,
	BULLET_MINI8_01_AKA,
	BULLET_MINI8_02_KIIRO,
	BULLET_MINI8_03_AKA,
	BULLET_MINI8_04_KIIRO,
	BULLET_MINI8_05_AOI,
	BULLET_MINI8_06_AOI,
	BULLET_MINI8_07_YUKARI,
//
	BULLET_MARU12_00_SIRO,	/*12*/
	BULLET_MARU12_01_AKA,	/*12*/
	BULLET_MARU12_02_MIDORI,/*12*/
	BULLET_MARU12_03_AOI,	/*12*/
	BULLET_MARU10_00_AOI,
	BULLET_MARU10_01_MIDORI,
	BULLET_MARU10_02_MIDORI,
	BULLET_MARU10_03_MIDORI,
//4
	BULLET_HARI32_00_AOI,
	BULLET_HARI32_01_AKA,
	BULLET_HARI32_02_KIIRO,
	BULLET_HARI32_03_DAIDAI,
	BULLET_KNIFE20_04_AOI,
	BULLET_KNIFE20_05_AKA,
	BULLET_KNIFE20_06_YUKARI,
	BULLET_KNIFE20_07_MIDORI,
//5
	BULLET_UROKO14_00_AOI,
	BULLET_UROKO14_01_AKA,
	BULLET_UROKO14_02_YUKARI,
	BULLET_UROKO14_03_MIDORI,
	BULLET_UROKO14_04_MIZUIRO,
	BULLET_UROKO14_05_KIIRO,
	BULLET_CAP16_04_SIROI,
	BULLET_CAP16_05_SIROI,
//6
	BULLET_KOME_01_AOI,
	BULLET_KOME_02_AKA,
	BULLET_KOME_03_YUKARI,
	BULLET_KOME_04_MIDORI,
	BULLET_KOME_05_KIIRO,
	BULLET_KUNAI12_06_AOI,
	BULLET_KUNAI12_00_AKA,
	BULLET_KUNAI12_07_MIDORI,
//7
	BULLET_KUNAI12_01_AKA,
	BULLET_KUNAI12_02_MIDORI,
	BULLET_KUNAI12_03_YUKARI,
	BULLET_KUNAI12_04_AOI,
	BULLET_OFUDA12_00_AOI,
	BULLET_OFUDA12_01_AKA,
	BULLET_OFUDA12_02_YUKARI,
	BULLET_OFUDA12_03_MIDORI,
//8
	BULLET_OODAMA32_00_SIROI,
	BULLET_OODAMA32_01_AOI,
	BULLET_OODAMA32_02_AKA,
	BULLET_OODAMA32_03_YUKARI,
	BULLET_OODAMA32_04_MIDORI,
	BULLET_OODAMA32_05_AOI,
	BULLET_OODAMA32_06_KIIRO,
	BULLET_OODAMA32_07_PINK,
};


/* �j�e�Ƃ����邩��A���ݐ}�`���肵�Ȃ��ƍ��\����ɂȂ�B */
/* �����蔻�薳�����Ă����x�͕ς��Ȃ������������ߖ�ł���̂ŏ����I�ɂ͂��̕ӂ����ǂ������B */
//#define USE_ZUKEI_ATARI_HANTEI (1)
//#define USE_ZUKEI_ATARI_HANTEI (0)

typedef struct /*_point256*/
{
	int x256;	/*	union POINT */	/* x�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
	int y256;	/*	union POINT */	/* y�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
} POINT256;

typedef struct /*_point_vector256*/
{
	int x256;	/*	union POINT */	/* x�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
	int y256;	/*	union POINT */	/* y�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
	int vx256;	/*	union POINT_VECTOR */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
	int vy256;	/*	union POINT_VECTOR */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
} POINT_VECTOR256;


#if 1
// union ���p
	#define base_health 				user00
	#define base_weapon_strength		user00
	#define base_time_out				user00/*bakuhatsu*/
	#define base_score					user01
	//#define base_state					user01
#endif
//
typedef struct _sprite
{
	int x256;			/*	union POINT */			/* x�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
	int y256;			/*	union POINT */			/* y�\���ʒu  (256�Œ菬���_�`��) / akt. Position */
	int vx256;			/*	union POINT_VECTOR */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
	int vy256;			/*	union POINT_VECTOR */	/* ���pregist_vector(); */	/*union BULLET_VECTOR_DATA*/
//[4]
	int w128;						/* �����̔��� (256�Œ菬���_�`��) / Breite, H�he */
	int h128;						/* �����̔��� (256�Œ菬���_�`��) / Breite, H�he */
//
	int m_angleCCW512;				/* �`��p�p�x(����0�x�ō����(�����v���)) */
	int tmp_angleCCW512;			/* �ێ��p�p�x(����0�x�ō����(�����v���)) */
//[8]
	int type;						/* ��ދy�юg�p�� / Sprite-Type, (siehe enum SPRITE_TYPE), 0 = remove. */
	SDL_Surface *sprite_bmp;		/* �摜 / Images */
	int m_Hit256R;					/* �����蔻��p */
	int m_zoom_x256;				/* �\���g�嗦 */
//[12]
	int user00; 		/* user00 ENEMY_BASE base; */	//int health;	int time_out;		/* user03 */
	int user01; 		/* user01 ENEMY_BASE base; */	//int score;	int state;			/* user02 */
	//int timeover_ticks;			/* �쐬���Ă���̌o�ߎ��� (����KETM���̂Ƀo�O������ׁA��莞�Ԍo�߂���Ƌ�����������) / number of ticks since creation */
//
	u32 color32;					/* Gu color AGBR8888 */
//
	struct _sprite *next;
//[16]
	void *data; 					/* Zeiger auf Strukur mit zus�tzlichen Data. */
	void (*callback_loser)(struct _sprite *src);/* Custom-loser-Routine (���ꂽ��Ƀ{�[�i�X���o���Ƃ��A�Ⴄ��������Ƃ�) */
	void (*callback_mover)(struct _sprite *src);/* Custom-Move-Routine */
	void (*callback_hit_enemy)(struct _sprite *c, struct _sprite *src); 	// [***090325		�ǉ�
//[20]
	Uint8 flags;					/* �t���O�Z�b�g / siehe unten (SP_FLAG...) */
	Uint8 priority; 				/* �\���D�揇�� / Priority for drawing. (0: unten, 9 oben) */
//
	Uint8 yx_anim_frame;			/* / �A�j���p�^�[���̔ԍ�(���R�}�ڂ�) */	 /*int*/			/* action. Frame */
	Uint8 yx_frames;				/* / �\���p�^�[������ / Anzahl Frames */
	Uint8 anim_count;				/*int*/ 	// Sint8 anim_count;			/* intern: Zaehler f. anim_speed */
	Uint8 anim_speed/* 256 */;		/*�t�]�A�j���֎~�ɕύX*/		/* �A�j���[�V�������x / Geschw. der Animation */	 /*Sint8*/ /*Uint8*/
//
//[16]
} SPRITE;
//	Uint8 dummy_alpha;					/* �����x	 / 255: opak, -  0: durchsichtig */
//	Uint8 dummy_alpha_chache;			/* �O��̃A���t�@�l */

#define YX_FRAME_LOW_X	(0x0f)
#define YX_FRAME_HIGH_Y (0xf0)
#define anim_frame yx_anim_frame


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

typedef struct
{
	const char *file_name;				/* �f�ނ̃t�@�C���� */
	Uint8 use_alpha;					/* �A���t�@(�摜�����x)�g�p�� */
	Uint8 total_frames_dummy_resurved;	/* ���݃_�~�[ */
	Uint8 x_divide_frames_m1;			/* (��������-1) */
	Uint8 y_divide_frames_m1;			/* (�c������-1) */
//
	Uint8 priority; 					/* �`��v���C�I���e�B�[(�\���D��x) */
	Uint8 anime_speed;					/* �A�j���[�V�������x */
	Uint8 atari_hankei; 				/* �����蔻��~�̔��a(�����蔻��̑傫��) */
	Uint8 aaa_dummy_resurved;			/* ���݃_�~�[ */
} IMAGE_RESOURCE;

//#define iyx(aaa,bbb) (((aaa-1)<<4)+(bbb-1)),1
#define iyx(aaa,bbb) (aaa-1),(bbb-1)



//extern SPRITE *spr ite_add_file 0(char *filename, int frames, Uint8 priority, int use_alpha);
//extern SPRITE *spr ite_add_file(char *filename, int frames, Uint8 priority);
//extern SPRITE *spr ite_add_file2(char *filename, int frames, Uint8 priority);
extern SPRITE *sprite_add_res(int image_resource_num);
extern SPRITE *sprite_add_bullet(int bullet_type_num);

#if (0)
extern SPRITE *spr ite_add_000xy00(
	SDL_Surface *surface,
	int total_frames,
	int x_divide_frames,
	int y_divide_frames,
	Uint8 priority,
	int set_flags/*nocache*/,
	int anime_speed
);
#endif
#if (1)
extern SPRITE *sprite_add_res_list(
	SDL_Surface *surface,
//	int total_frames,
//	int x_divide_frames,
//	int y_divide_frames,
//	Uint8 priority,
	int set_flags/*nocache*/,
//	int anime_speed
	IMAGE_RESOURCE *image_resource_ptr
	);
#endif

extern void sprite_remove_all000(int type);
//extern void sprite_remove_all222(int type);/*�e���p*/

extern void sprite_display000(int type);/**/
//extern void sprite_display222(int type);/*�e���p*/
extern void sprite_work000(int type);/**/
//extern void sprite_work222(int type);/*�e���p*/

extern SPRITE *sprite_collision_check(SPRITE *tocheck, int type);


#if 1
	/* �\�t�g�E�F�A�[�� Z�\�[�g */
	#define USE_ZBUFFER 	(0)
#else
	/* �n�[�h�E�F�A�[�Ń\�[�g */
	#define USE_ZBUFFER 	(1)
#endif

#if 1
	/* �P���g�� */
	#define USE_ZOOM_XY 	0
#else
	/* �c���g�� */
	#define USE_ZOOM_XY 	1
#endif


//#ifndef GRP_SCREEN_H
//#define GRP_SCREEN_H
/*	@since		Jul.27.2005 GRP_SCREEN_H		��ʕ\���Ǘ� */
/* --- �Ǘ�����ő�X�v���C�g�� */
//#define SPRITEMAX 1024
//#define SPRITEMAX 256
#define SPRITEMAX	512

typedef struct
{
	UINT8 used;
	UINT8 dummy_alpha;
	UINT8 texture_id;
	UINT8 center;	/*dummy*/
//
	/* - public */
	int x256;
	int y256;
	int w;
	int h;
	int tx;
	int ty;
	#if (1==USE_ZOOM_XY)
	int zoom_x256;
	int zoom_y256;
	#else //(0==USE_ZOOM_XY)
	int zoom_xy256;
	#endif/* (1==USE_ZOOM_XY) */
	int rotation_z;
	#if (1==USE_ZBUFFER)
	int priority;		/* �\���D�揇�� */	/* unsigned short */
	#endif/* (1==USE_ZBUFFER) */
	u32 color8888;//	int color8888;
//	Image *Texture;//
	//SDL_Surface *Texture_ptr;
} TGameSprite /*, *PTGameSprite*/;



#endif/* _SPRITE_H_ */
