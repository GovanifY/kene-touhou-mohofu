#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "game_main.h"

#include "bonus.h"
#include "load_stage.h"

enum
{
	BAKUHATSU_MINI00 = 0,/*������*/
	BAKUHATSU_MINI01,
	BAKUHATSU_MINI02,
	BAKUHATSU_MINI03,
//
	BAKUHATSU_ZAKO04,/*�U�R���Ŕ���*/
	BAKUHATSU_ZAKO05,
	BAKUHATSU_ZAKO06,
	BAKUHATSU_ZAKO07,
//
	BAKUHATSU_FIRE08,/*�Ή�����*/
	BAKUHATSU_FIRE09,
	BAKUHATSU_FIRE10,
	BAKUHATSU_FIRE11,
//
	BAKUHATSU_MOVE12,/*�ړ�����*/
	BAKUHATSU_MOVE13,
	BAKUHATSU_MOVE14,
	BAKUHATSU_MOVE15,
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

//typedef struct
//{
//	int health; 		/* ���p */
//	int score;			/* ���p */
//} ENEMY_BASE;

typedef struct
{
//------------ �X�y�J�֘A

//	int boss_health;		/* ���p */	/* (����)�̗� */
//	int /*boss_*/score; 	/* ���p */
	//int boss_life;			/* ���p */	// (���)�̗�
//------------ �e���֘A
//	int dummy_tmp_angleCCW512;	/* �ێ��p�x */
	int danmaku_type;
	int danmaku_time_out;
	int danmaku_test;
	int dummy_aaaa;/* �\��(�����^�C�}�[�Ŏg��) */
} BOSS_BASE;
extern int boss_hamidasi;			/* �u�ړ��ł��Ȃ������t���O�v(�g�p�O�Ɏ蓮��OFF==0�ɂ��Ƃ�) */
extern POINT256 boss_clip_min;		/* �{�X�ړ��͈�(�ŏ��l) */
extern POINT256 boss_clip_max;		/* �{�X�ړ��͈�(�ő�l) */


//typedef struct		/* enemy�̈�ʌ` */
//{
//	ENEMY_BASE b;	/* ���p */
//	int state;
//} ENE MY_DATA;


#if 1
enum
{
	BULLET_RES_00_,
	BULLET_RES_01_,
	BULLET_RES_02_,
	BULLET_RES_03_,
	BULLET_RES_04_,
	BULLET_RES_05_,
	BULLET_RES_06_,
	BULLET_RES_07_,
	BULLET_RES_MAX,
};

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
	SPRITE *src;		// ���e���W
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
extern BULLET_STATUS bullet_resource[BULLET_RES_MAX];
//extern void bullet_create(BULLET_STATUS *b);/* �e�̐��� */
extern void bullet_create_resource(int type);/* �e�̐��� */

// [r13]
//void bullet_create_aka_maru_n_way(SPRITE *src, int speed256, int angle512, int angle2_512, int bu_type, int way)
/* �e�̐��� */

extern void bullet_create_aka_maru_jikinerai(		SPRITE *src, int speed256);
extern void bullet_create_jyuryoku_dan000(			SPRITE *src, int speed256, int angle512, int delta256, int bullet_obj_type );

//�p�~extern void enemy_laser_create(				SPRITE *src, int speed256);/*�p�~(bullet_create_hari_dan180()��version up)*/
//�p�~extern void enemy_laser_create2(				SPRITE *src, int speed256, int angle512);/*�p�~(bullet_create_hari_dan180()��version up)*/
//�p�~extern void bullet_create_hari_dan180(		SPRITE *src, int speed256, int angle512, int x_offset256, int y_offset256 );/*�p�~(bullet_create_offset_dan_type()��version up)*/
extern void bullet_create_offset_dan_type000(		SPRITE *src, int speed256, int angle512/*, int x_offset256, int y_offset256*/, int bullet_obj_type );

extern void bullet_create_enemy_homing( 			SPRITE *src);
//static void enemy_homing_update(					SPRITE *src);

extern void bullet_create_maru8_frame(				SPRITE *src, int speed256, int angle512, int set_frame);
extern void bullet_create_momiji_dan(				SPRITE *src, int speed256, int angle512);
//static void bullet_create_momiji_seed(			SPRITE *src, int speed256, int angle512, dou ble a);
extern void bullet_create_oodama00( 				SPRITE *src, int speed256, int angle512, int ransu512, int add_speed256/*, int xoffs256, int yoffs256*/);
//tern void bullet_create_oodama1(					SPRITE *src, int xoffs256, int yoffs256, int speed256, int angle512, int ransu);
//tern void bullet_create_oodama2(					SPRITE *src, int speed256, int angle512, int a256);
//�p�~extern void bullet_create_gg_dan( 			SPRITE *src, int speed256, int angle512, int state_hi, int state_lo);/*�p�~(bullet_create_n_way_dan_type()���g��)*/
extern void bullet_create_hazumi_dan(				SPRITE *src, int speed256, int angle512, int delta256, int bound_counts);
extern void bullet_create_tomari_dan(				SPRITE *src, int speed256, int angle512, int delta256);
//extern id enemy_stop_bullet2_create(				SPRITE *src, int speed256, int angle512, int a256, dou ble next_angle);
extern void bullet_create_tomari2_dan(				SPRITE *src, int speed256, int angle512, int a256, int next_angle512);

extern void bullet_create_rot4096_dan(				SPRITE *src, int speed256, int angle512, int d_angle4096);
extern void bullet_create_sakuya_knife( 			SPRITE *src, int speed256, int angle512, int p_angle512/*anim*/);
extern void bullet_create_sakuya_no_rot_knife(		SPRITE *src, int speed256, int angle512, int gra256);
extern void bullet_create_sakuya_follow_knife1( 	SPRITE *src, int speed256, int angle512, int height);
//extern void bullet_create_sakuya_follow_knife2(	SPRITE *src, int speed256, int angle512, int height); /*dou ble x, dou ble y,*/
//extern void bullet_create_sakuya_even_knife(		SPRITE *src, int speed256, int length, int r_or_l);
extern void bullet_create_sakuya_even_knife_bbb(	SPRITE *src);


//extern void bullet_create_n_way_dan_type( 		SPRITE *src, int speed256, int angle512, int bullet_obj_type, int n_way);/* char *filename, int frame,*/
extern void bullet_create_n_way_dan_sa_type(		SPRITE *src, int speed256, int angle512, int sa_angle512, int bullet_obj_type, int n_way);/* char *filename, int frame,*/

#endif

//extern void bakuhatsu_add_type(int x256, int y256, /*dou ble int delay_wait,*/ int type);
extern void bakuhatsu_add_type_ddd(SPRITE *src/*int x256, int y256*/, /*dou ble int delay_wait,*/ int type);
extern void bakuhatsu_add_rect(SPRITE *src);
extern void bakuhatsu_add_circle(SPRITE *src, int mode);
extern void bakuhatsu_add_zako04(SPRITE *src);

extern void callback_hit_zako(SPRITE *src, SPRITE *t);

/* �{�X�ړ������̋��ʃ��[�`�� */

extern void boss_move_clip_rect(SPRITE *src);


/* thegame.c �݂̂� �U�R�^�{�X�Ő錾���K�v�Ȃ���(�O���[�o��)�́Athegame.c �ֈړ������B */

/* �{�X�Ő錾���K�v�Ȃ���(�O���[�o��) */

extern void enemy_set_random_seed(void/*int set_seed*/);
extern int enemy_get_random_item(void);

#include "danmaku.h"

#endif /* _ENEMY_H_ */

#ifndef _SPELL_CARD_H_
#define _SPELL_CARD_H_
//	SPELL_CARD_00_OFF = 0,
enum
{
// ��� 6��
	SPELL_CARD_00_sakuya_000 = 0,	/* "�`�ԕύX" */
	SPELL_CARD_01_sakuya_aaa,		/* ���`��: ���E�ɓ����ď��e���� */
	SPELL_CARD_02_sakuya_bbb,		/* ���`��: �S���ʁA���܂� */
	SPELL_CARD_03_sakuya_ccc,		/* ��O�`��: �����i�C�t�������Ă���� */
	SPELL_CARD_04_sakuya_ddd, 		/* ��l�`��: �����w���� */
	SPELL_CARD_05_sakuya_eee, 		/* ��܌`��: (���F�}�X�J�b�g�e) */
//	SPELL_CARD_06_sakuya_fff, 		/* ��Z�`��: (���F�}�X�J�b�g�e) */
//	SPELL_CARD_07_sakuya_ggg,		/* �掵�`��: (���U�����w)�ǉ��v�撆 */
//	SPELL_CARD_08_sakuya_hhh,		/* �攪�`��: (���~�߃i�C�t)�ǉ��v�撆 */
	SPELL_CARD_09_sakuya_iii,		/* ���`��: �ŏI�`��(����1) */
	SPELL_CARD_10_sakuya_jjj,		/* ��10�`��: �ŏI�`��(����2) */
	SPELL_CARD_11_sakuya_kkk,		/* ��11�`��: �ŏI�`��(����3) */
// �p�`�F 5��
	SPELL_CARD_00_pache_000,
	SPELL_CARD_11_pache_bbb,
	SPELL_CARD_12_pache_ccc,
	SPELL_CARD_13_pache_ddd,
	SPELL_CARD_14_pache_eee,
	SPELL_CARD_15_pache_fff,
	SPELL_CARD_16_pache_ggg,
	SPELL_CARD_17_pache_hhh,
//	SPELL_CARD_18_pache_iii,
//	SPELL_CARD_19_pache_jjj,
// �P�� 4��
	SPELL_CARD_00_kaguya_000,
	SPELL_CARD_11_kaguya_bbb,
	SPELL_CARD_12_kaguya_ccc,
	SPELL_CARD_13_kaguya_ddd,
	SPELL_CARD_14_kaguya_eee,
	SPELL_CARD_15_kaguya_fff,
	SPELL_CARD_16_kaguya_ggg,
	SPELL_CARD_17_kaguya_hhh,
// ���� 3��
	SPELL_CARD_00_mitei_000,
	SPELL_CARD_11_mitei_bbb,
	SPELL_CARD_12_mitei_ccc,
	SPELL_CARD_13_mitei_ddd,
	SPELL_CARD_14_mitei_eee,
	SPELL_CARD_15_mitei_fff,
	SPELL_CARD_16_mitei_ggg,
	SPELL_CARD_17_mitei_hhh,
// �� 2��
	SPELL_CARD_00_aya_000,
	SPELL_CARD_11_aya_bbb,
	SPELL_CARD_12_aya_ccc,
	SPELL_CARD_13_aya_ddd,
	SPELL_CARD_14_aya_eee,
	SPELL_CARD_15_aya_fff,
	SPELL_CARD_16_aya_ggg,
	SPELL_CARD_17_aya_hhh,
	SPELL_CARD_18_aya_iii,
	SPELL_CARD_19_aya_jjj,
// �A���X 1��
	SPELL_CARD_00_alice_000,
	SPELL_CARD_11_alice_bbb,
	SPELL_CARD_12_alice_ccc,
	SPELL_CARD_13_alice_ddd,
	SPELL_CARD_14_alice_eee,
	SPELL_CARD_15_alice_fff,
	SPELL_CARD_16_alice_ggg,
	SPELL_CARD_17_alice_hhh,
	SPELL_CARD_18_alice_iii,
	SPELL_CARD_19_alice_jjj,
//
	SPELL_CARD_MAX	/* �ő吔 */
};

//------------ �X�y�J�֘A

extern void spell_card_generator222(SPRITE *src);
extern void create_spell_card(SPRITE *src, int spell_card_type);

extern void spell_card_boss_init_regist(SPRITE *src);
extern void regist_spell_card222(SPRITE *src);
extern void common_00_keitai(SPRITE *src);

extern int spell_card_mode; 			/* �X�y�J���[�h */
extern int spell_card_limit_health; 	/* �K��l�ȉ��ɂȂ�΃X�y�J���[�h���� */
extern int spell_card_boss_state;		/* ���l�ɂȂ�΃{�X�X�y�J���[�h�ɓ���Ȃ� */
extern int spell_card_boss_timer;		/* ���p */	// ��������

extern int spell_card_number;			/* ���p */	// �X�y�J�ԍ�
extern int spell_card_max;				/* ���p */	// �X�y�J�ԍ��ő���E�l

/* �o����x���W */
#define BOSS_XP256	(t256(GAME_WIDTH/2)-(t256(32/2)))/*sakuya->w128*/


//------------ ��

/*static*/ void boss_effect(SPRITE *src);
/*static*/ void boss_effect_init(void);
/*static*/ void boss_effect_term(void);

#endif /* _SPELL_CARD_H_ */
