
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _BULLET_OBJECT_H_
#define _BULLET_OBJECT_H_

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
#define TAMA_ATARI_02					((2)<<8)
#define TAMA_ATARI_04					((4)<<8)

#define TAMA_ATARI_COMMON16_PNG 		TAMA_ATARI_02
#define TAMA_ATARI_MARU16_PNG			TAMA_ATARI_02
#define TAMA_ATARI_KNIFE18_PNG			TAMA_ATARI_02
#define TAMA_ATARI_JIPPOU32_PNG 		TAMA_ATARI_02

#define TAMA_ATARI_OODAMA_08_PNG		TAMA_ATARI_04



#define TAMA_ATARI_BULLET_DUMMY 		TAMA_ATARI_02
#define JIKI_ATARI_ITEM_16				((16)<<8)
#define JIKI_ATARI_ITEM_80				((80)<<8)


#define ZAKO_ATARI02_PNG				TAMA_ATARI_02
#define ZAKO_ATARI04_PNG				TAMA_ATARI_04
#define ZAKO_ATARI16_PNG				JIKI_ATARI_ITEM_16
#define ZAKO_ATARI_HOMING16_PNG 		JIKI_ATARI_ITEM_16


#if 1
	/* �p�x�l(0-1023)��1024 �𒴂����ꍇ�̓���@�\ */
//
	/* ���@�_���e */
	#define ANGLE_JIKI_NERAI_DAN	(9999)
//
	/* ���ʂɌ����Ȃ��e */
	#define ANGLE_NO_SHOT_DAN		(8888)
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


/* �Ƃ肠���� */
#if 1
	#define boss_base_danmaku_type				user_data10
	#define boss_base_danmaku_time_out			user_data11
	#define boss_base_danmaku_test				user_data12
	#define boss_base_state001					user_data13
#endif


extern int boss_hamidasi;			/* �u�ړ��ł��Ȃ������t���O�v(�g�p�O�Ɏ蓮��OFF==0�ɂ��Ƃ�) */
extern POINT256 boss_clip_min;		/* �{�X�ړ��͈�(�ŏ��l) */
extern POINT256 boss_clip_max;		/* �{�X�ړ��͈�(�ő�l) */

extern POINT256 bullet_clip_min;	/* �e�͈̔�(�ŏ��l) */
extern POINT256 bullet_clip_max;	/* �e�͈̔�(�ő�l) */

#if 1
enum
{
	REGIST_TYPE_00_MULTI_VECTOR,		/* �������e */
	REGIST_TYPE_01_HAZUMI,		/* �d�͒e01 �P��A�ŏI�`�Ԃœ����Ă��邩�Ȃ薳���Ȓe�B */
	REGIST_TYPE_02_GRAVITY02,	/* �d�͒e02 */
	REGIST_TYPE_03_TOMARI,		/*	�e */
	REGIST_TYPE_04_KURU_KURU,	/*	�e */
	REGIST_TYPE_99_MAX,
};

/* �e�o�^ */
extern void bullet_regist_vector(void);
extern void bullet_regist_angle(void);
/* �x�N�g���e�̏ꍇ */
typedef struct _bullet_regist_
{
	int BULLET_REGIST_speed256; 							//		/* aa */
	int BULLET_REGIST_angle1024;							//		/* bb */
	int BULLET_REGIST_div_angle1024;						//		/* cc */
	int BULLET_REGIST_bullet_obj_type;						//		/* dd */

	int BULLET_REGIST_n_way;								//		/* ee */
	int BULLET_REGIST_sakuya_kurukurku_knife_height;		//		/* ff */

	int BULLET_REGIST_regist_type;							/* �o�^���� */
} BULLET_REGIST;
extern BULLET_REGIST br;
#endif
#define BULLET_REGIST_hosi_gata_time_out			BULLET_REGIST_speed256
#define BULLET_REGIST_hosi_gata_angle1024			BULLET_REGIST_angle1024
#define BULLET_REGIST_hosi_gata_add_speed256		BULLET_REGIST_div_angle1024

#define BULLET_REGIST_jyuryoku_dan_delta256 		BULLET_REGIST_div_angle1024
#define BULLET_REGIST_jyuryoku_dan_bound_counts 	BULLET_REGIST_n_way

#define BULLET_REGIST_tomari2_dan_delta256			BULLET_REGIST_div_angle1024
#define BULLET_REGIST_tomari2_dan_next_angle1024	BULLET_REGIST_n_way

//
/* �p�x�e�̏ꍇ */

//typedef struct _bullet_regist_
//{
//	int BULLET_REGIST_speed256; 					//		/* aa */	<����>
#define BULLET_REGIST_angle65536			BULLET_REGIST_angle1024 		/* <65536�x�Ɍ���> */	//		/* bb */
#define BULLET_REGIST_div_angle65536		BULLET_REGIST_div_angle1024 	/* <65536�x�Ɍ���> */
//	int BULLET_REGIST_bullet_obj_type;				//		/* dd */	<����>
//
//	int BULLET_REGIST_n_way;						//		/* ee */	<����>
#define BULLET_REGIST_speed_offset			BULLET_REGIST_sakuya_kurukurku_knife_height 	//		/* ff */	<����>
//
//	int BULLET_REGIST_regist_type;					/* �o�^���� */		<����>
//} BULLET_REGIST;


//extern void bakuhatsu_add_type(int x256, int y256, /*dou ble int delay_wait,*/ int type);
extern void bakuhatsu_add_type_ddd(SPRITE *src/*int x256, int y256*/, /*dou ble int delay_wait,*/ int type);
extern void bakuhatsu_add_rect(SPRITE *src);
extern void bakuhatsu_add_circle(SPRITE *src, int mode);
extern void bakuhatsu_add_zako04(SPRITE *src);

extern void callback_hit_zako(SPRITE *src, SPRITE *t);

/* �{�X�ړ������̋��ʃ��[�`�� */

extern void boss_move_vx_vy(SPRITE *src);
extern void boss_clip_rect(SPRITE *src);


// ���ʌ`��
extern void common_99_keitai(SPRITE *src);/* ���j��ɉ�ʊO�Ƀ{�X�������� */

/* thegame.c �݂̂� �U�R�^�{�X�Ő錾���K�v�Ȃ���(�O���[�o��)�́Athegame.c �ֈړ������B */

/* �{�X�Ő錾���K�v�Ȃ���(�O���[�o��) */

extern void enemy_set_random_seed(void/*int set_seed*/);
//extern int en emy_get_random_item(void);
extern void lose_random_item(SPRITE *src);

//#include "danmaku.h"

#endif /* _BULLET_OBJECT_H_ */

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
	SPELL_CARD_04_sakuya_ddd,		/* ��l�`��: �����w���� */
	SPELL_CARD_05_sakuya_eee,		/* ��܌`��: (���F�}�X�J�b�g�e) */
//	SPELL_CARD_06_sakuya_fff,		/* ��Z�`��: (���F�}�X�J�b�g�e) */
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
// �� 4��
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
// �P�� 3��
	SPELL_CARD_00_kaguya_000,
	SPELL_CARD_11_kaguya_bbb,
	SPELL_CARD_12_kaguya_ccc,
	SPELL_CARD_13_kaguya_ddd,
	SPELL_CARD_14_kaguya_eee,
	SPELL_CARD_15_kaguya_fff,
	SPELL_CARD_16_kaguya_ggg,
	SPELL_CARD_17_kaguya_hhh,
// ���� 2��
	SPELL_CARD_00_mima_000,
	SPELL_CARD_11_mima_bbb,
	SPELL_CARD_12_mima_ccc,
	SPELL_CARD_13_mima_ddd,
	SPELL_CARD_14_mima_eee,
	SPELL_CARD_15_mima_fff,
	SPELL_CARD_16_mima_ggg,
	SPELL_CARD_17_mima_hhh,
// �A���X 1��
	SPELL_CARD_00_alice_000,
	SPELL_CARD_11_alice_bbb,
	SPELL_CARD_12_alice_ccc,
	SPELL_CARD_13_alice_ddd,
	SPELL_CARD_14_alice_eee,
//	SPELL_CARD_15_alice_fff,
//	SPELL_CARD_16_alice_ggg,
//	SPELL_CARD_17_alice_hhh,
//	SPELL_CARD_18_alice_iii,
//	SPELL_CARD_19_alice_jjj,
//
	SPELL_CARD_MAX	/* �ő吔 */
};

//------------ �X�y�J�֘A

extern void create_spell_card(SPRITE *src, int spell_card_type);

#if 0
//extern void re gist_spell_card222(SPRITE *src);/* �X�y���J�[�h�̓o�^ */
//extern void sp ell_card_generator222(SPRITE *src);
#else
//extern void ch eck_regist_spell_card(SPRITE *src);/* �X�y���J�[�h�̍X�V�`�F�b�N */
extern void check_regist_generate_spell_card(SPRITE *src);	/* �X�y�J�o�^�\�Ȃ�o�^ / �X�y�J���� */
#endif


extern	void danmaku_state_check_holding(SPRITE *src);/* �e�����I���܂ő҂B */

enum
{
	SPELL_CARD_MODE_00_OFF = 0, 		/* �X�y�J���g�p���Ȃ�(�ʏ�U����)�B(�X�y�J�����Ă邩�ǂ������f) */
	SPELL_CARD_MODE_01_IDO_JYUNNBI, 	/* ���Ă�ꍇ�B���e�ʒu�܂ňړ� */
	SPELL_CARD_MODE_02_TAIHI,		/* ���B */
	SPELL_CARD_MODE_03_HATUDAN, 		/* ���e���B */
};

extern int spell_card_mode; 			/* �X�y�J���[�h */
extern int spell_card_limit_health; 	/* �K��l�ȉ��ɂȂ�΃X�y�J���[�h���� */
extern int spell_card_boss_state;		/* ���l�ɂȂ�΃{�X�X�y�J���[�h�ɓ���Ȃ� */
extern int spell_card_boss_timer;		/* [���p]�������� */

extern int spell_card_number;			/* [���p]�X�y�J�ԍ� */
extern int spell_card_max;				/* [���p]�X�y�J�ԍ��ő���E�l */

/* �o����x���W */
#define BOSS_XP256	(t256(GAME_WIDTH/2)-(t256(32/2)))/*sakuya->w128*/


//------------ "��"�݂����ȃ}�[�N�̃G�t�F�N�g
/*
�u"��"�݂����ȃ}�[�N�v�́A�{�X�������Ă�X�y���J�[�h�������ł��B
������{���́A�X�y�J�̖�������������A���������炳�Ȃ��Ⴂ���Ȃ��̂��ȁH
�ł��{�Ƃ����Ȃ��ĂȂ���ˁB
*/

/*static*/extern  void move_boss_effect(SPRITE *src);

/*static*/extern  void boss_effect_initialize_position(void);


//	�U�R�o�^�̋��ʃ��[�`��
//extern void add_zako_common(STAGE_DATA *l, SPRITE *src);

#endif /* _SPELL_CARD_H_ */
