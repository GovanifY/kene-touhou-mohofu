#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <SDL/SDL.h>
#include <math.h>

#include "support.h"
#include "sprite.h"
//#include "particle.h"
#include "enemy.h"


enum _select_pl 		//[***090203		�ǉ�
{
	REIMU=0,
	MARISA,
	REMILIA,
	CIRNO,
	YUYUKO,
};

/*
	bossmode�ɂ���
	0:���ɂȂ��B							// [�Q�[����]
	1:�{�X�Ƃ̐퓬���B						// [�Q�[����]
	4:�`�ԕύX���B���G���j�B�e����҂��B	// [�Q�[����](enemy_greatfairy.c�ł��g��)
	2:�{�X���j���B�e����҂��B				// [�Q�[����]
//
	5:�J�n�C�x���g�t�@�C���ǂݍ��݁B		// [�퓬�O]
	7:�I���C�x���g�t�@�C���ǂݍ��݁B		// [���j��]
	3:�{�X�J�n�C�x���g(��b�V�[��)�B		// [�퓬�O]
	6:�{�X�I���C�x���g(��b�V�[��)�B		// [���j��]
	8:�{�X�Ȃ�炵�A1�{�X�Ƃ̐퓬�ցB 	// [�퓬�O]
	9:stage�ǂݍ��݁B						// [���j��]
*/
#if 1
	/* ���d�l */
	enum _boss_mode 		//[***090223		�ǉ�
	{
		B00_NONE=0, 		// [�Q�[����]
		B01_BATTLE, 		// [�Q�[����]
		B02_DEATH_WAIT, 	// [�Q�[����]
		B03_BEFORE_EVENT,	// [�퓬�O]
		B04_CHANGE, 		// [�Q�[����]
		B05_BEFORE_LOAD,	// [�퓬�O]
		B06_AFTER_EVENT,	// [���j��]
		B07_AFTER_LOAD, 	// [���j��]
		B08_START,			// [�퓬�O]
		B09_STAGE_LOAD, 	// [���j��]
	};
#else
	enum _boss_mode 		//[***090223		�ǉ�
	{
		B00_NONE=0, 		// [�Q�[����]
		B01_BATTLE, 		// [�Q�[����]
		B04_CHANGE, 		// [�Q�[����]
		B02_DEATH_WAIT, 	// [�Q�[����]
	//
		B05_BEFORE_LOAD,	// [�퓬�O]
		B07_AFTER_LOAD, 	// [���j��]
		B03_BEFORE_EVENT,	// [�퓬�O]
		B06_AFTER_EVENT,	// [���j��]
		B08_START,			// [�퓬�O]
		B09_STAGE_LOAD, 	// [���j��]
	};
#endif

/*�Ȃ�*/
#define BONUS_FLAG_00_NONE							0x00
/* [�㕔�������W] MAX���Ƀv���C���[�㕔���W�Ŏ����A�C�e�����W */
#define BONUS_FLAG_01_PLAYER_UP_AUTO_GET_ITEM		0x01
/* [�{���ɂ�鎩�����W] �{�������Ŏ����A�C�e�����W */
#define BONUS_FLAG_02_BOMB_AUTO_GET_ITEM			0x02
/* [�X�R�A�������W] �{�X�|����Ɏ����_�����W(���A�C�e���p) */
#define BONUS_FLAG_03_SCORE_AUTO_GET_ITEM			0x04



#define BONUS_FLAG_07_IS_GRAZE						0x40
#define BONUS_FLAG_08_OPTION_HIDE					0x80


//#define BONUS_FLAG_0123_AUTO_GET_ITEM (BONUS_FLAG_01_PLAYER_UP_AUTO_GET_ITEM|BONUS_FLAG_02_BOMB_AUTO_GET_ITEM|BONUS_FLAG_03_SCORE_AUTO_GET_ITEM)

typedef struct
{
	int lives;		/* �c��`�����X */
	int bombs;		/* �{���� */
	int graze;		/* �O���C�Y���_ */
	int score;		/* �X�R�A���_ */
//	int now_stage/*level*/;
	int bossmode;
	int state;
//	int explode;// ���K��0�Ȃ̂ňӖ��Ȃ������B�Ȃ񂩂̋@�\�̎c�肾�����H�H�H
	/*double*/int save_delay;
//	/*double*/int an im_delay;	�p�~
	int weapon; 				/*	0x00-0x7f  (0-127 ��128�i�K==�{�ƂƓ���)   max==127==�u128�i�K�v*/
	int player_speed;
//	int player_speed_minimum;	/*�e�v���C���[���Ƃ̌Œ�l�Ȃ̂ō폜*/
//	int player_speed_maximum;	/*�e�v���C���[���Ƃ̌Œ�l�Ȃ̂ō폜*/
	int extra_type;
	int bonus_flag; 		/* �{�[�i�X�A�C�e���������ɏW�܂��Ԃ̐ݒ�t���O */	//[***090116		�ǉ�
	int hit_bomb_wait;		//[***090125		�ǉ�
//	int option_flag;		//	int bonus_flag;�{�[�i�X�t���O�ɋz��
//	double extra_interval;/*����g�p�Ԋu�̋��ʎ��Ԃ͔p�~(�e����ŊԊu�͊Ǘ�)*/
	/*double*/int bomber_time;
//	double weapon_interval;
	SPRITE *core;
	SPRITE *enemy;			//[***090125		�ǉ�:player�ɓ���������
	SPRITE *boss;			//[***090305		�ǉ�
	void (*callback_boss_hitbyweapon)(SPRITE *c, SPRITE *s);		//[***090325		�ǉ�
	void (*callback_boss_hitbyplayer)(SPRITE *c);					//[***090325		�ǉ�
} PLAYER_DATA;


enum _player_extras
{
	PLX_NONE=0,
//	PLX_HOMING,
//	PLX_HLASER,
//	PLX_SHIELD,
	PLX_BOMB,
	PLX_LAST		/* �ő吔 */
};

typedef struct
{
	int strength;
} WEAPON_BASE;

extern void player_init(void);


//static void player_move_core(SPRITE *s2);
//static void player_move_fireball(SPRITE *s);
//static void player_move_plasma(SPRITE *s);
//static void player_move_killray(SPRITE *s);
//static void player_move_bomb(SPRITE *s);
//static int search_enemy();
//static void player_move_homing(SPRITE *s);
//static void player_move_shield(SPRITE *s);
//static void player_move_shield2(SPRITE *s);
//static void player_move_levarie(SPRITE *s);
//static void player_move_hlaser(SPRITE *s);

//static void player_move(SPRITE *s);
//static void player_keycontrol(SPRITE *s);
//static void player_colcheck(SPRITE *s, int mask);

//static void weapon_check(int w);

//static SPRITE *player_add_core(SPRITE *s1);

//static void re_move_option(SPRITE *s);
//static void ma_move_option(SPRITE *s);

//static void re_add_option(SPRITE *s);
//static void ma_add_option(SPRITE *s);

//static void player_add_doublefireball(SPRITE *player);
//static void player_add_triplefireball(SPRITE *player);
//static void player_add_fifthfireball(SPRITE *player);

//static void re_add_fireball(SPRITE *s, int x, int y);
//static void ma_add_fireball(SPRITE *s, int x, int y);

//static void player_controller_hlaser(CONTROLLER *c);

//static void player_add_doublefireball_ma(SPRITE *player);
//static void player_add_triplefireball_ma(SPRITE *player);
//static void player_add_fifthfireball_ma(SPRITE *player);
//static void player_add_fuda(SPRITE *player);
//static void player_add_doublefuda(SPRITE *player);
//static void player_add_triplefuda(SPRITE *player);
//static void player_add_quadfuda(SPRITE *player);
//static void player_add_fifthfuda(SPRITE *player);

//static void player_add_star(SPRITE *player);
//static void player_add_doublestar(SPRITE *player);
//static void player_add_triplestar(SPRITE *player);
//static void player_add_quadstar(SPRITE *player);

//static void player_add_killray(SPRITE *player);
//static void player_add_bomb(SPRITE *s);
//static void player_add_homing(SPRITE *s);
//static void player_add_shield(SPRITE *s);
//static void player_add_levarie(SPRITE *s);
//static void player_add_hlaser(SPRITE *s);

//static void weapon_colcheck(SPRITE *s, /*int angle,*/ int destroy, int check_bullets);
#endif
