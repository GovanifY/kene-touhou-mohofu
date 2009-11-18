#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "game_main.h"
#include "bullet_object.h"


// > �yPSP�zPSP�œ�����肽���z8�y�e���z
// > 200 �F���������񁗂��������ς��B�F2009/01/27(��) 18:41:00 ID:xQgI7hCU
// > ���낻��S�A�C�e�������ė~�����ȁB
// > ����FULL�p���[���̃A�C�e�������񂹃��C���͂���������Ɖ��̕��������Ǝv���B
// > ��̓I�ɂ͂��̕ӂ聫
// > ttp://www4.uploader.jp/user/isisuke/images/isisuke_uljp00081.bmp
// ���̈ӌ��͂킽�������l�Ɏv�����̂ŁA�݂�Ȃ������v�����ɈႢ�Ȃ��B
// ���̉摜��484x283[dots]�ŁAy==63[line]�ɐԐ��������Ă������B
// psp�̉𑜓x�́A 480x272[dots]�Ȃ̂� x/272=63/283 , x=(63*272)/283 == 60.55[line]
// ����/���쎁	50(49)[line] �����シ����B
// �X��8��200�� ��60[line] (���o�I��)
// �����ŃL�����̍����������邵�A64(63)�������Ɖ�72(71)���炢�ł������̂ł́H�Ƃ������64�ɂ��Ď������Ă݂�B
// ���ʁF����ς����������������B8[dots]�Ƃ��킸16[dots]���炢���������B�Ăю������Ă݂�B
// ���ʁF����Ȃ��񂩂ȁH�Ƃ����킯��80(79)[dots]�Ɍ���B����8[dots]��88(87)�ł����������ˁB
// �{�Ƃ̊��o��厖�ɂ���Ȃ炱��Ȃ��񂾂Ǝv���B
#define PLAYER_SPLIT_LINE256		(t256(80))
// http://hossy.info/game/toho/k_score.php �����̉摜���t�Z���Čv�Z����� 77 dots ������B
//#define PLAYER_SPLIT_LINE256		(t256(77))

/* ���̃��C����艺����͓G�������Ȃ� */
#define ENEMY_LAST_SHOT_LINE256 	(t256(272-72))


enum /*_select_pl_*/		// [***090203		�ǉ�
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
	MAX_PLAYER
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
		#if (0000)
#define STATE_FLAG_08_OPTION_HIDE					(0x0080)
		#endif/*0000*/

/* on */
//efine ST ATE_FLAG_IS_PANEL_WINDOW_SHIFT			(8)
//efine ST ATE_FLAG_09_IS_PANEL_WINDOW				(0x0100)
/* �{�X��|�����ꍇ��on(���Ԑ؂�ł�OFF) */
#define STATE_FLAG_09_IS_WIN_BOSS					(0x0100)
/* on */
#define STATE_FLAG_10_IS_LOAD_SCRIPT				(0x0200)
/* on */
//#define ST ATE_FLAG_11_IS_BOSS_DESTROY				(0x0400)/* �p�~ */
/* on */
#define STATE_FLAG_12_END_SCRIPT					(0x0800)
/* on */
#define STATE_FLAG_13_DRAW_BOSS_GAUGE				(0x1000)

//#define STATE_FLAG_0123_AUTO_GET_ITEM (STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM|STATE_FLAG_02_BOMB_AUTO_GET_ITEM|STATE_FLAG_03_SCORE_AUTO_GET_ITEM)

#define STATE_FLAG_14_GAME_LOOP_QUIT				(0x2000)
#define STATE_FLAG_15_KEY_SHOT						(0x4000)
#define STATE_FLAG_16_NOT_KEY_CONTROL				(0x8000)


typedef struct
{
	int state_flag; 		/* �ݒ�t���O */	// [***090116		�ǉ�
	int weapon_power;		/*	0x00-0x7f  (0-127 ��128�i�K==�{�ƂƓ���)   max==127==�u128�i�K�v*/
	int chain_point;
	int bomber_time_dummy;
//
	int my_score;			/* �X�R�A���_ */
	int graze_point;		/* �O���C�Y���_ */
	int bombs;				/* �{���� */
	int zanki;				/* �c��`�����X */
//
	SPRITE *core;
	SPRITE *boss;			// [***090305		�ǉ�
} PLAYER_DATA;
extern	int pd_bomber_time;

//typedef struct
//{
//	int strength;
//} WEAPON_BASE;

//#define DEFAULT_MAX_CONTINUE	(3)
//#define DEFAULT_MAX_CONTINUE	(30)
#define DEFAULT_MAX_CONTINUE	(30)


#endif /* _PLAYER_H_ */
