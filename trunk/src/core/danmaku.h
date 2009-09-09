#ifndef _DANMAKU_H_
#define _DANMAKU_H_
	/*---------------------------------------------------------
		�e����`
	-------------------------------------------------------
	���e���̓V�X�e���ŃT�|�[�g����\�z������܂��B(�\�z��)
	�V�X�e���ŃT�|�[�g�����ƁA�ǂ̃{�X���A�ǂ̒e���ł����Ă�悤�ɂȂ�܂��B
	---------------------------------------------------------*/
enum
{
	DANMAKU_00 = 0,
//	���
	DANMAKU_01,
	DANMAKU_02,
	DANMAKU_03,
//	���\��
	DANMAKU_04,
	DANMAKU_05,
	DANMAKU_06,
	DANMAKU_07,
//	�A���X(��)
	DANMAKU_08,
	DANMAKU_09,
	DANMAKU_10,
	DANMAKU_11,
//40[sec]
	DANMAKU_12,
	DANMAKU_13,
	DANMAKU_14,
	DANMAKU_15,
	DANMAKU_16,
//	�A���X(��)�\��

};

//#define DANMAKU_01_SET_TIME	/*(150)*/
//#define DANMAKU_01_SET_TIME	/*(ra_nd()&0xff)*/
#define DANMAKU_01_SET_TIME 	(64+1)	/* �S�e���̔������� */
#define DANMAKU_02_SET_TIME 	(48+1)	/* 24�e���̔������� */
#define DANMAKU_03_SET_TIME 	(64+1)	/* 11�e���̔������� */

extern void tmp_angle_jikinerai512(SPRITE *p, SPRITE *s);
extern void danmaku_generator(SPRITE *s);
typedef struct /*_boss05_data*/
{
	BOSS_BASE boss_base;
//------------ �ړ��֘A
	int dummy_state1; 							/* �s�� */
	int dummy_move_type;		/*	�ړ��^�C�v */	/* �`�� */
//
	int dummy_wait1;
	int dummy_wait2_256;
//
	int dummy_wait3;
//	int dummy_level;
	int dummy_move_angle512;
	int dummy_length_s_p256;	/* ���ƃv���C���[�Ƃ̋��� */
	int dummy_aaaa;
} BOSS99_DATA;/*�Ƃ肠�����_�~�[��`*/
#endif /* _DANMAKU_H_ */
