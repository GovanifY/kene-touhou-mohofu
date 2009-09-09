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

	/* ��e�̏ꍇ�Ɏ��@�_��(�����Ȃ���Γ�����Ȃ�) / odd bullet */
	#define ANGLE_JIKINERAI_KISUDAN 999

	/* �����e�̏ꍇ�Ɏ��@�_��(�����Ȃ��Ɠ�����) / even bullet */
	//#define ANGLE_JIKINERAI_GUSUDAN 888

	/* ���@�_���e */
	#define ANGLE_JIKINERAI_DAN 	(ANGLE_JIKINERAI_KISUDAN)

	/* ���ʂɌ����Ȃ��e */
	#define ANGLE_NO_SHOT_DAN		777
#endif


typedef struct
{
	int health; 		/* ���p */
	int score;			/* ���p */
} ENEMY_BASE;

typedef struct
{
	int health; 		/* ���p */
	int score;			/* ���p */
	int boss_timer; 	/* ���p */	// ��������
} BOSS_BASE;

//typedef struct		/* enemy�̈�ʌ` */
//{
//	ENEMY_BASE b;	/* ���p */
//	int state;
//} ENE MY_DATA;


typedef struct
{
	int angle512;	/* ���p */
	int speed256;	/* ���p */
} BULLET_DATA;


// [r13]
//void bullet_create_aka_maru_n_way(SPRITE *s, int speed256, int angle512, int angle2_512, int bu_type, int way)
/* �e�̐��� */

extern void bullet_create_aka_maru_jikinerai(	SPRITE *s, int speed256);
extern void bullet_create_jyuryoku_dan( 		SPRITE *s, int speed256, int angle512, int delta256);

//�p�~extern void enemy_laser_create(			SPRITE *s, int speed256);/*�p�~(bullet_create_hari_dan180()��version up)*/
//�p�~extern void enemy_laser_create2(			SPRITE *s, int speed256, int angle512);/*�p�~(bullet_create_hari_dan180()��version up)*/
//�p�~extern void bullet_create_hari_dan180(	SPRITE *s, int speed256, int angle512, int x_offset256, int y_offset256 );/*�p�~(bullet_create_offset_dan_type()��version up)*/
extern void bullet_create_offset_dan_type(		SPRITE *s, int speed256, int angle512, int x_offset256, int y_offset256, int bullet_obj_type );

extern void bullet_create_enemy_homing( 		SPRITE *s);
//static void enemy_homing_update(				SPRITE *s);

extern void bullet_create_maru8_frame(			SPRITE *s, int speed256, int angle512, int set_frame);
extern void bullet_create_momiji_dan(			SPRITE *s, int speed256, int angle512);
//static void bullet_create_momiji_seed(		SPRITE *s, int speed256, int angle512, dou ble a);
extern void bullet_create_oodama0(				SPRITE *s, int speed256, int angle512, int ransu512, int add_speed256, int xoffs256, int yoffs256);
//tern void bullet_create_oodama1(				SPRITE *s, int xoffs256, int yoffs256, int speed256, int angle512, int ransu);
//tern void bullet_create_oodama2(				SPRITE *s, int speed256, int angle512, int a256);
//�p�~extern void bullet_create_gg_dan( 		SPRITE *s, int speed256, int angle512, int state_hi, int state_lo);/*�p�~(bullet_create_n_way_dan_type()���g��)*/
extern void bullet_create_hazumi_dan(			SPRITE *s, int speed256, int angle512, int delta256, int bound_counts);
extern void bullet_create_tomari_dan(			SPRITE *s, int speed256, int angle512, int delta256);
//extern id enemy_stop_bullet2_create(			SPRITE *s, int speed256, int angle512, int a256, dou ble next_angle);
extern void bullet_create_tomari2_dan(			SPRITE *s, int speed256, int angle512, int a256, int next_angle512);

extern void bullet_create_rot4096_dan(			SPRITE *s, int speed256, int angle512, int d_angle4096);
extern void bullet_create_sakuya_knife( 		SPRITE *s, int speed256, int angle512, int p_angle512/*anim*/);
extern void bullet_create_sakuya_no_rot_knife(		SPRITE *s, int speed256, int angle512, int gra256);
extern void bullet_create_sakuya_follow_knife1( 		SPRITE *s, int speed256, int angle512, int height);
//extern void bullet_create_sakuya_follow_knife2(		SPRITE *s, int speed256, int angle512, int height); /*dou ble x, dou ble y,*/
extern void bullet_create_sakuya_even_knife(			SPRITE *s, int speed256, int length, int r_or_l);


extern void explosion_add_type(int x256, int y256, /*dou ble int delay_wait,*/ int type);
extern void explosion_add_rect(SPRITE *src);
extern void explosion_add_circle(SPRITE *src, int mode);


/* thegame.c �݂̂� �U�R�^�{�X�Ő錾���K�v�Ȃ���(�O���[�o��)�́Athegame.c �ֈړ������B */

/* �{�X�Ő錾���K�v�Ȃ���(�O���[�o��) */

//extern void bullet_create_n_way_dan_type(SPRITE *s, int speed256, int angle512, int bullet_obj_type, int n_way);/* char *filename, int frame,*/
extern void bullet_create_n_way_dan_sa_type(SPRITE *s, int speed256, int angle512, int sa_angle512, int bullet_obj_type, int n_way);/* char *filename, int frame,*/

extern void enemy_set_random_seed(void/*int set_seed*/);
extern int enemy_get_random_item(void);


/* �{�X��|�����炷���Ă΂��(�v���C���[�𖳓G�ɂ����)(�t���O�͎d�l�㎞�ԑ҂�������ׁA���݂��̗p�r�ɂ͎g���Ȃ�) */
extern void player_set_destoroy_boss(void);

#endif /* _ENEMY_H_ */
