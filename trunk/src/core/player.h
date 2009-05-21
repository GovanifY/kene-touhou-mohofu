#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "support.h"
#include "enemy.h"


enum /*_select_pl_*/		// [***090203		�ǉ�
{
	REIMU = 0,
	MARISA,
	REMILIA,
	CIRNO,
	YUYUKO,
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
/* on */
#define STATE_FLAG_06_IS_SCRIPT 					(0x0020)
/* on */
#define STATE_FLAG_07_IS_GRAZE						(0x0040)
/* on */
#define STATE_FLAG_08_OPTION_HIDE					(0x0080)

/* on */
//efine STATE_FLAG_IS_PANEL_WINDOW_SHIFT			(8)
#define STATE_FLAG_09_IS_PANEL_WINDOW				(0x0100)
/* on */
#define STATE_FLAG_10_IS_LOAD_SCRIPT				(0x0200)
/* on */
#define STATE_FLAG_11_IS_BOSS_DESTROY				(0x0400)
/* on */
#define STATE_FLAG_12_END_SCRIPT					(0x0800)
/* on */
#define STATE_FLAG_13_DRAW_BOSS_GAUGE				(0x1000)

//#define STATE_FLAG_0123_AUTO_GET_ITEM (STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM|STATE_FLAG_02_BOMB_AUTO_GET_ITEM|STATE_FLAG_03_SCORE_AUTO_GET_ITEM)

#define STATE_FLAG_14_GAME_LOOP_QUIT				(0x2000)
#define STATE_FLAG_15_KEY_SHOT						(0x4000)
#define STATE_FLAG_16_KEY_CONTROL					(0x8000)


typedef struct
{
	int state_flag; 		/* �ݒ�t���O */	// [***090116		�ǉ�
	int weapon_power;		/*	0x00-0x7f  (0-127 ��128�i�K==�{�ƂƓ���)   max==127==�u128�i�K�v*/
	int chain_point;
	int bomber_time;/*dou ble*/
//
	int my_score;			/* �X�R�A���_ */
	int graze_point;		/* �O���C�Y���_ */
	int bombs;				/* �{���� */
	int zanki;				/* �c��`�����X */
//
	SPRITE *core;
	SPRITE *boss;			// [***090305		�ǉ�
	void (*callback_boss_hitbyweapon)(SPRITE *c, SPRITE *s);		// [***090325		�ǉ�
} PLAYER_DATA;

typedef struct
{
	int strength;
} WEAPON_BASE;

#endif /* _PLAYER_H_ */
