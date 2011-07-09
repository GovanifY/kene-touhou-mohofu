
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _BULLET_OBJECT_H_
#define _BULLET_OBJECT_H_

enum
{
	BAKUHATSU_MINI00 = 0,/* ������ */
	BAKUHATSU_MINI01,
	BAKUHATSU_MINI02,
	BAKUHATSU_MINI03,
//
	BAKUHATSU_ZAKO04,/* �U�R���Ŕ��� */
	BAKUHATSU_ZAKO05,
	BAKUHATSU_ZAKO06,
	BAKUHATSU_ZAKO07,
//
	BAKUHATSU_FIRE08,/* �Ή����� */
	BAKUHATSU_FIRE09,
	BAKUHATSU_FIRE10,
	BAKUHATSU_FIRE11,
//
	BAKUHATSU_MOVE12,/* �ړ����� */
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

	/* ���e�G�t�F�N�g�̎��ԕ� */
	#define HATUDAN_FRAME64 	(64)



/**/extern int boss_hamidasi;		/* �u�ړ��ł��Ȃ������t���O�v(�g�p�O�Ɏ蓮��OFF==0�ɂ��Ƃ�) */
extern POINT256 boss_clip_min;		/* �{�X�ړ��͈�(�ŏ��l) */
extern POINT256 boss_clip_max;		/* �{�X�ړ��͈�(�ő�l) */

extern POINT256 bullet_clip_min;	/* �e�͈̔�(�ŏ��l) */
extern POINT256 bullet_clip_max;	/* �e�͈̔�(�ő�l) */




//extern void bakuhatsu_add_type(int x256, int y256, /*dou ble int delay_wait,*/ int type);
extern void bakuhatsu_add_type_ddd(SPRITE *src/*int x256, int y256*/, /*dou ble int delay_wait,*/ int type);
extern void bakuhatsu_add_rect(SPRITE *src);
extern void bakuhatsu_add_circle(SPRITE *src, int mode);
extern void bakuhatsu_add_zako04(SPRITE *src);

extern void callback_hit_zako(SPRITE *src, SPRITE *t);


// ���ʌ`��
extern void common_99_keitai(SPRITE *src);/* ���j��ɉ�ʊO�Ƀ{�X�������� */

/* thegame.c �݂̂� �U�R�^�{�X�Ő錾���K�v�Ȃ���(�O���[�o��)�́Athegame.c �ֈړ������B */

/* �{�X�Ő錾���K�v�Ȃ���(�O���[�o��) */

//#include "danmaku.h"

#endif /* _BULLET_OBJECT_H_ */

#ifndef _SPELL_CARD_H_
#define _SPELL_CARD_H_

//------------ �X�y�J�֘A

extern void create_spell_card(SPRITE *src, int spell_card_type);

#if 0
//extern void re gist_spell_card222(SPRITE *src);/* �X�y���J�[�h�̓o�^ */
//extern void sp ell_card_generator222(SPRITE *src);
#else
//extern void ch eck_regist_spell_card(SPRITE *src);/* �X�y���J�[�h�̍X�V�`�F�b�N */
extern void check_regist_generate_spell_card(SPRITE *src);	/* �X�y�J�o�^�\�Ȃ�o�^ / �X�y�J���� */
#endif

// extern	void dan maku_state_check_holding(SPRITE *src);/* �e�����I���܂ő҂B */

enum
{
	SPELL_CARD_MODE_00_OFF = 0, 		/* �X�y�J���g�p���Ȃ�(�ʏ�U����)�B(�X�y�J�����Ă邩�ǂ������f) */
	SPELL_CARD_MODE_01_IDO_JYUNNBI, 	/* ���Ă�ꍇ�B���e�ʒu�܂ňړ� */
	SPELL_CARD_MODE_02_TAIHI,			/* �ޔ𒆁B */
	SPELL_CARD_MODE_03_HATUDAN, 		/* ���e���B */
};

typedef struct /*_spell_card_global_class_*/
{
	int mode;				/* �X�y�J���[�h */
	int limit_health;		/* �K��l�ȉ��ɂȂ�΃X�y�J���[�h���� */
	int boss_state; 		/* ���l�ɂȂ�΃{�X�X�y�J���[�h�ɓ���Ȃ� */
	int boss_timer; 		/* [���p]�������� */
	//
	int number; 			/* [���p]�X�y�J�ԍ� */
	int number_temporaly;	/* [�ꎞ�g�p]�X�y�J�ԍ� */
	int spell_type; 		/* */
	int boss_hp_dec_by_frame;/* �{�X�U�������l�A�t���[���P�� */
/* �Ƃ肠���� */
#if 0
//	#define boss_base_resurved000				user_data10/*(r32)���݁A���g�p�B*/
//	#define bo ss_base_state001 				user_data10/*�p�~*/
	#define boss_base_spell_temporaly			user_data11
	#define boss_base_spell_type				user_data12
	#define boss_base_spell_time_out			user_data13
#endif

} SPELL_CARD_GLOBAL_CLASS;
extern SPELL_CARD_GLOBAL_CLASS spell_card;

/* �o����x���W */
#define BOSS_XP256		(t256(GAME_WIDTH/2))	/* ���S���W�Ȃ̂� */


//------------ "��"�݂����ȃ}�[�N�̃G�t�F�N�g
/*
�u"��"�݂����ȃ}�[�N�v�́A�{�X�������Ă�X�y���J�[�h�������ł��B
������{���́A�X�y�J�̖�������������A���������炳�Ȃ��Ⴂ���Ȃ��̂��ȁH
�ł��{�Ƃ����Ȃ��ĂȂ���ˁB
*/

/*static*/extern  void move_boss_effect(SPRITE *src);

/*static*/extern  void boss_effect_initialize_position(void);


//	�U�R�o�^�̋��ʃ��[�`��
//extern void add_za ko_common(GAME_COMMAND *l, SPRITE *src);

#endif /* _SPELL_CARD_H_ */
