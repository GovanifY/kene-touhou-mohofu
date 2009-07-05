#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "support.h"

#include "bonus.h"
#include "loadlv.h"

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
/* ��e�̏ꍇ�Ɏ��@�_�� */
#define ANGLE_JIKINERAI_KISUDAN 999

#define ANGLE_JIKINERAI_DAN 	999

/* ���ʂɌ����Ȃ��e */
#define ANGLE_NO_SHOT_DAN		777

typedef struct
{
	int score;			/* ���p */
	int health; 		/* ���p */
} ENEMY_BASE;

typedef struct
{
	int score;			/* ���p */
	int health; 		/* ���p */
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
//void bullet_way(SPRITE *s, int way, dou ble angle, dou ble angle2, dou ble spd);

/* �e�̐��� */

extern void bullet_create_aka_maru_jikinerai(	SPRITE *s, int speed256);
extern void enemy_gr_bullet_create( 			SPRITE *s, int speed256, int angle512, int delta256);

//extern void enemy_laser_create(				SPRITE *s, int speed256);/*�p�~(bullet_create_hari_dan180()��version up)*/
//extern void enemy_laser_create2(				SPRITE *s, int speed256, int angle512);/*�p�~(bullet_create_hari_dan180()��version up)*/
extern void bullet_create_hari_dan180(			SPRITE *s, int speed256, int angle512, int x_offset256, int y_offset256 );

extern void bullet_create_enemy_homing( 		SPRITE *s);
//static void enemy_homing_update(				SPRITE *s);

extern void bullet_create_maru8_frame(			SPRITE *s, int speed256, int angle512, int set_frame);
extern void enemy_momiji_create(				SPRITE *s, int speed256, int angle512);
//static void enemy_smallbullet_re_create(		SPRITE *s, int speed256, int angle512, dou ble a);
extern void bullet_create_oodama0(				SPRITE *s, int speed256, int angle512, int ransu512, int add_speed256, int xoffs256, int yoffs256);
//tern void bullet_create_oodama1(				SPRITE *s, int xoffs256, int yoffs256, int speed256, int angle512, int ransu);
//tern void bullet_create_oodama2(				SPRITE *s, int speed256, int angle512, int a256);
extern void enemy_gg_bullet_create( 			SPRITE *s, int speed256, int angle512, int state_hi, int state_lo);
extern void bullet_create_hazumi_dan(			SPRITE *s, int speed256, int angle512, int delta256, int bounds);
extern void bullet_create_tomari_dan(			SPRITE *s, int speed256, int angle512, int delta256);
//extern id enemy_stop_bullet2_create(			SPRITE *s, int speed256, int angle512, int a256, dou ble next_angle);
extern void bullet_create_tomari2_dan(			SPRITE *s, int speed256, int angle512, int a256, int next_angle512);

extern void enemy_angle_bullet_create(			SPRITE *s, int speed256, int angle512, int d_angle4096);
extern void enemy_knife_create( 				SPRITE *s, int speed256, int angle512, int anim);
extern void enemy_fall_knife_create(			SPRITE *s, int speed256, int angle512, int gra256);
extern void enemy_follow_knife_create1( 		SPRITE *s, int speed256, int angle512, int height);
//extern void enemy_follow_knife_create2(		SPRITE *s, int speed256, int angle512, int height); /*dou ble x, dou ble y,*/
extern void enemy_even_knife_create(			SPRITE *s, int speed256, int length, int r_or_l);


extern void explosion_add_type(int x256, int y256, /*dou ble int delay_wait,*/ int type);
extern void explosion_add_rect(SPRITE *src);
extern void explosion_add_circle(SPRITE *src, int mode);


/* thegame.c �݂̂� �U�R�^�{�X�Ő錾���K�v�Ȃ���(�O���[�o��)�́Athegame.c �ֈړ������B */

/* �{�X�Ő錾���K�v�Ȃ���(�O���[�o��) */

void bullet_create_n_way_dan_type(SPRITE *s, int speed256, int angle512, int bu_type, int n);/* char *filename, int frame,*/
#define BU_TYPE01_KUGEL_PNG 0
#define BU_TYPE01_KUNAI_PNG 0

extern void enemy_set_random_seed(void/*int set_seed*/);
extern int enemy_get_random_item(void);


/* �{�X��|�����炷���Ă΂��(�v���C���[�𖳓G�ɂ����)(�t���O�͎d�l�㎞�ԑ҂�������ׁA���݂��̗p�r�ɂ͎g���Ȃ�) */
extern void player_set_destoroy_boss(void);

#endif /* _ENEMY_H_ */
