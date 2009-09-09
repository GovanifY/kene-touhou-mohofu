#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "game_main.h"

#include "bonus.h"
#include "load_stage.h"

enum
{
	EXPLOSION_MINI00=0,/*������*/
	EXPLOSION_MINI01,
	EXPLOSION_MINI02,
	EXPLOSION_MINI03,
//
	EXPLOSION_ZAKO04,/*�U�R���Ŕ���*/
	EXPLOSION_ZAKO05,
	EXPLOSION_ZAKO06,
	EXPLOSION_ZAKO07,
//
	EXPLOSION_FIRE08,/*�Ή�����*/
	EXPLOSION_FIRE09,
	EXPLOSION_FIRE10,
	EXPLOSION_FIRE11,
};


#if 1
	/* �p�x�l(0-511)��512 �𒴂����ꍇ�̓���@�\ */
//
	/* ���@�_���e */
	#define ANGLE_JIKI_NERAI_DAN	(999)
//
	/* ���ʂɌ����Ȃ��e */
	#define ANGLE_NO_SHOT_DAN		(777)
//
	#if 0
		/* �����e�̏ꍇ�Ɏ��@�_��(�����Ȃ��Ɠ�����) / even bullet */
		//#define ANGLE_JIKI_NERAI_GUSUDAN 999
		/* ��e�̏ꍇ�Ɏ��@�_��(�����Ȃ���Γ�����Ȃ�) / odd bullet */
		//#define ANGLE_JIKI_NERAI_KISUDAN 888
		/*
		�悭�l������AANGLE_JIKI_NERAI_KISUDAN �͕K�v�Ȃ��B
		ANGLE_JIKI_NERAI_DAN ��������Ηǂ��A
		���e���ɋ������w�肷��΋����e�B
		���e���Ɋ���w�肷��Ί�e�B
		������܂������ǁA�C���t���Ȃ�����ww�B
		*/
	#endif
#endif

typedef struct
{
	int health; 		/* ���p */
	int score;			/* ���p */
} ENEMY_BASE;

typedef struct
{
	int boss_health;		/* ���p */	/* (����)�̗� */
	int /*boss_*/score; 	/* ���p */
	int boss_timer; 		/* ���p */	// ��������
	int boss_life;			/* ���p */	// (���)�̗�
//------------ �e���֘A
	int tmp_angleCCW512;	/* �ێ��p�x */
	int danmaku_type;
	int danmaku_time_out;
	int danmaku_test;
} BOSS_BASE;

//typedef struct		/* enemy�̈�ʌ` */
//{
//	ENEMY_BASE b;	/* ���p */
//	int state;
//} ENE MY_DATA;


#if 1
/*
	ToDo:
	�p�t�H�[�}���X(���s���x)�I�ϓ_����A�����͑S����ɋ��ʉ����ׂ��B
	����Ȓe���́A�e���g���R�[���o�b�N�Œe�������ׂ��B
	(�Ⴆ�΋��ʋK�imove_bullet���ŕϐg����悤�Ȓe�́A���ʋK�imove_bullet���ŋ��ʋK�i��bullet_create���R�[������(�R�[���o�b�N)�B)
	(���̕ӂ̋K�i���\���̂Ō��߂ăV���v���ɂ���\��)
*/
/* �e�̐����K�i(��) */
typedef struct _bullet_create_status
{
	SPRITE *s;			// ���e���W
	int speed256;		// ���x
	int angle512;		// �p�x
	int create_count;	// 0 == ���񐶐�
//
	int argument_00;	// ����
	int argument_01;	// ����
	int argument_02;	// ����
	int argument_03;	// ����
//
//	int create_type;	// �������
//	_bullet_create_status *bbb;
//	int dummy_08;
//	int dummy_09;
//
	void (*callback_bullet_create0)(struct _bullet_create_status *b);	// NULL�ŏI��(����==�e������)
	void (*callback_bullet_create1)(struct _bullet_create_status *b);	// NULL�ŏI��(�ϐg1���)
	void (*callback_bullet_create2)(struct _bullet_create_status *b);	// NULL�ŏI��(�ϐg2���)
	void (*callback_bullet_create3)(struct _bullet_create_status *b);	// NULL�ŏI��(�ϐg3���)
} BULLET_STATUS;
extern void bullet_create(BULLET_STATUS *b);/* �e�̐��� */

// [r13]
//void bullet_create_aka_maru_n_way(SPRITE *s, int speed256, int angle512, int angle2_512, int bu_type, int way)
/* �e�̐��� */

extern void bullet_create_aka_maru_jikinerai(		SPRITE *s, int speed256);
extern void bullet_create_jyuryoku_dan( 			SPRITE *s, int speed256, int angle512, int delta256);

//�p�~extern void enemy_laser_create(				SPRITE *s, int speed256);/*�p�~(bullet_create_hari_dan180()��version up)*/
//�p�~extern void enemy_laser_create2(				SPRITE *s, int speed256, int angle512);/*�p�~(bullet_create_hari_dan180()��version up)*/
//�p�~extern void bullet_create_hari_dan180(		SPRITE *s, int speed256, int angle512, int x_offset256, int y_offset256 );/*�p�~(bullet_create_offset_dan_type()��version up)*/
extern void bullet_create_offset_dan_type(			SPRITE *s, int speed256, int angle512, int x_offset256, int y_offset256, int bullet_obj_type );

extern void bullet_create_enemy_homing( 			SPRITE *s);
//static void enemy_homing_update(					SPRITE *s);

extern void bullet_create_maru8_frame(				SPRITE *s, int speed256, int angle512, int set_frame);
extern void bullet_create_momiji_dan(				SPRITE *s, int speed256, int angle512);
//static void bullet_create_momiji_seed(			SPRITE *s, int speed256, int angle512, dou ble a);
extern void bullet_create_oodama0(					SPRITE *s, int speed256, int angle512, int ransu512, int add_speed256, int xoffs256, int yoffs256);
//tern void bullet_create_oodama1(					SPRITE *s, int xoffs256, int yoffs256, int speed256, int angle512, int ransu);
//tern void bullet_create_oodama2(					SPRITE *s, int speed256, int angle512, int a256);
//�p�~extern void bullet_create_gg_dan( 			SPRITE *s, int speed256, int angle512, int state_hi, int state_lo);/*�p�~(bullet_create_n_way_dan_type()���g��)*/
extern void bullet_create_hazumi_dan(				SPRITE *s, int speed256, int angle512, int delta256, int bound_counts);
extern void bullet_create_tomari_dan(				SPRITE *s, int speed256, int angle512, int delta256);
//extern id enemy_stop_bullet2_create(				SPRITE *s, int speed256, int angle512, int a256, dou ble next_angle);
extern void bullet_create_tomari2_dan(				SPRITE *s, int speed256, int angle512, int a256, int next_angle512);

extern void bullet_create_rot4096_dan(				SPRITE *s, int speed256, int angle512, int d_angle4096);
extern void bullet_create_sakuya_knife( 			SPRITE *s, int speed256, int angle512, int p_angle512/*anim*/);
extern void bullet_create_sakuya_no_rot_knife(		SPRITE *s, int speed256, int angle512, int gra256);
extern void bullet_create_sakuya_follow_knife1( 	SPRITE *s, int speed256, int angle512, int height);
//extern void bullet_create_sakuya_follow_knife2(	SPRITE *s, int speed256, int angle512, int height); /*dou ble x, dou ble y,*/
extern void bullet_create_sakuya_even_knife(		SPRITE *s, int speed256, int length, int r_or_l);


//extern void bullet_create_n_way_dan_type( 		SPRITE *s, int speed256, int angle512, int bullet_obj_type, int n_way);/* char *filename, int frame,*/
extern void bullet_create_n_way_dan_sa_type(		SPRITE *s, int speed256, int angle512, int sa_angle512, int bullet_obj_type, int n_way);/* char *filename, int frame,*/

#endif

extern void explosion_add_type(int x256, int y256, /*dou ble int delay_wait,*/ int type);
extern void explosion_add_rect(SPRITE *src);
extern void explosion_add_circle(SPRITE *src, int mode);


/* thegame.c �݂̂� �U�R�^�{�X�Ő錾���K�v�Ȃ���(�O���[�o��)�́Athegame.c �ֈړ������B */

/* �{�X�Ő錾���K�v�Ȃ���(�O���[�o��) */

extern void enemy_set_random_seed(void/*int set_seed*/);
extern int enemy_get_random_item(void);


/* �{�X��|�����炷���Ă΂��(�v���C���[�𖳓G�ɂ����)(�t���O�͎d�l�㎞�ԑ҂�������ׁA���݂��̗p�r�ɂ͎g���Ȃ�) */
extern void player_set_destoroy_boss(void);

#include "danmaku.h"

#endif /* _ENEMY_H_ */
