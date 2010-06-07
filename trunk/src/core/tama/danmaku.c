
#include "game_main.h"

/*---------------------------------------------------------
 �����͕핗  �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�e������
	-------------------------------------------------------
	���e���̓V�X�e���ŃT�|�[�g����\�z������܂��B(�\�z��)
	�V�X�e���ŃT�|�[�g�����ƁA�ǂ̃{�X���A�ǂ̒e���ł����Ă�悤�ɂȂ�܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	���@�_���e�̊p�x���v�Z
---------------------------------------------------------*/

global /*static*/ void tmp_angleCCW65536_jikinerai(SPRITE *p, SPRITE *t)
{
	/* �ȗ���(�e�Ǝ������傫���������Ȃ炸��Ȃ��A�Ⴄ�Ƃ��̕��덷�ɂȂ�) */
	t->tmp_angleCCW65536		= (atan_65536(p->y256-t->y256, p->x256-t->x256));
}


/*---------------------------------------------------------
	���@�_���e�̊p�x���v�Z
---------------------------------------------------------*/

global /*static*/ void tmp_angleCCW1024_jikinerai(SPRITE *p, SPRITE *t)
{
	/* �ȗ���(�e�Ǝ������傫���������Ȃ炸��Ȃ��A�Ⴄ�Ƃ��̕��덷�ɂȂ�) */
	t->tmp_angleCCW1024 		= (atan_1024(p->y256-t->y256, p->x256-t->x256));
}

#include "danmaku_00_sonota.h"
//
#include "danmaku_01_sakuya.h"
#include "danmaku_01_mima.h"
#include "danmaku_01_alice.h"
#include "danmaku_01_rumia.h"
#include "danmaku_01_meirin.h"
#include "danmaku_01_chrno.h"
#include "danmaku_01_kaguya.h"
#include "danmaku_01_daiyousei.h"

/*---------------------------------------------------------
	���ʒe��������
	-------------------------------------------------------

---------------------------------------------------------*/

//#define DANMAKU_064_TIME	/*(150)*/
//#define DANMAKU_064_TIME	/*(ra_nd()&0xff)*/
//	src->boss_base_danmaku_time_out = (128+1);			/* �S�e���̔������� x 2 */
	#define DANMAKU_064_TIME	(64+1)
	#define DANMAKU_048_TIME	(48+1)
	#define DANMAKU_128_TIME	(128+1)
	#define DANMAKU_256_TIME	(256+1)
	#define DANMAKU_640_TIME	(640+1)

typedef struct
{
	void (*danmaku_callback)(SPRITE *sss);		/* �������ړ����� */
	int danmaku_time;							/* ���� */
} DANMAKU_RESOURCE;
//	/*const*/static void (*danmaku_create_bbb[(DANMAKU_MAX/*16+1*/)])(SPRITE *sss) =
	/*const*/static DANMAKU_RESOURCE my_danmaku_resource[(DANMAKU_MAX/*16+1*/)] =
{
	#define danmaku_create_99_mitei danmaku_create_10_cirno_misogi
	{	NULL,									DANMAKU_128_TIME},	/* 00 */	/* �e���������Ȃ� */
	//													/* [���] */
	{	danmaku_create_01_sakuya_misogi,		DANMAKU_064_TIME},	/* 01 */	/* ����S�e�� / �g5�ʒ��{�X ��� �u�ʏ�U���v */
	{	danmaku_create_02_24nerai,				DANMAKU_048_TIME},	/* 02 */	/* ���24�e�� (��p�u�~�X�f�B���N�V�������ǂ�(1/2)�v) */
	{	danmaku_create_03_11nife,				DANMAKU_064_TIME},	/* 03 */	/* ���11�e�� (��p�u�~�X�f�B���N�V�������ǂ�(2/2)�v) */
	{	danmaku_create_04_pink_hearts,			DANMAKU_128_TIME},	/* 04 */	/* �g5�ʃ{�X ��� �u�ʏ�U��1(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
	{	danmaku_create_05_32way_dual,			DANMAKU_128_TIME},	/* 05 */	/* �g5�ʃ{�X ��� �u�ʏ�U��1/2(2/2)�v�ɂ�����Ƃ��������e��(�\��) */
	{	danmaku_create_06_luna_clock_32way, 	DANMAKU_128_TIME},	/* 06 */	/* �g5�ʃ{�X ��� ���ہu���i�N���b�N(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
	{	danmaku_create_07_80way_dual_five,		DANMAKU_128_TIME},	/* 07 */	/* �g5�ʃ{�X ��� �u�ʏ�U��3(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
	//													/* [�A���X(��)] */
	{	danmaku_create_08_night_bird,			DANMAKU_128_TIME},	/* 08 */	/* �g1�ʃ{�X ���[�~�A �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������e��(�\��) */
	{	danmaku_create_09_alice_sentakki,		DANMAKU_128_TIME},	/* 09 */	/* ����@�e�� */
	{	danmaku_create_0a_houka_kenran, 		DANMAKU_128_TIME},	/* 0a */	/* �g���� �ؕ��u�F�؈�ࣁv�ɂ�����Ƃ��������e��(�\��) */
	{	danmaku_create_0b_alice_doll,			DANMAKU_128_TIME},	/* 0b */	/* �A���X�l�`�e�� */
	{	danmaku_create_0c_hana_test,			DANMAKU_256_TIME},	/* 0c */	/* �����A�ԂĂ��ƒe�� */
	{	danmaku_create_0d_mima_sekkin,			DANMAKU_256_TIME},	/* 0d */	/* �����A�ڋߒe�� */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 0e */	/* ����e�� */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 0f */	/* ����e�� */
	//													/* [�`���m(��)] */
	{	danmaku_create_10_cirno_misogi, 		DANMAKU_064_TIME},	/* 10 */	/* �`���m�S�e�� */
	{	danmaku_create_11_tengu_shot,			DANMAKU_128_TIME},	/* 11 */	/* �������̗d��1(�V��l?)�����e��(�\��) */
	{	danmaku_create_12_aya_merin_test,		DANMAKU_128_TIME},	/* 12 */	/* ���ɔ�����ۂ��e���������Ă݂�e�X�g */
	{	danmaku_create_13_perfect_freeze,		DANMAKU_640_TIME},	/* 13 */	/* �p�[�t�F�N�g�t���[�Y */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 14 */	/* ����e�� */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 15 */	/* ����e�� */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 16 */	/* ����e�� */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 17 */	/* ����e�� */
	//
	{	danmaku_create_18_kaguya01, 			DANMAKU_064_TIME},	/* 18 */	/* �Ƃ肠���������ɕϊ� */
	{	danmaku_create_19_kaguya04, 			DANMAKU_064_TIME},	/* 19 */	/* �Ƃ肠���������ɕϊ� */
	{	danmaku_create_20_dai_yousei_midori,	DANMAKU_064_TIME},	/* 1a */	/* ����e�� */
	{	danmaku_create_21_dai_yousei_aka,		DANMAKU_064_TIME},	/* 1b */	/* ����e�� */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 1c */	/* ����e�� */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 1d */	/* ����e�� */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 1e */	/* ����e�� */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 1f */	/* ����e�� */
};
global void danmaku_set_time_out(SPRITE *src)
{
	src->boss_base_danmaku_time_out = my_danmaku_resource[(src->boss_base_danmaku_type/*-1*/)].danmaku_time;
}
global /*static*/ void danmaku_generator(SPRITE *src)
{
	if (DANMAKU_00 != src->boss_base_danmaku_type)	/* �e�������͕K�v�H */
	{
		src->boss_base_danmaku_time_out--;			/* ���Ԍo�߂���  */ 	/*fps_factor*/
		if (0 < src->boss_base_danmaku_time_out)	/* ���Ԑ؂�H */
		{
			/* �e�������� */
			(my_danmaku_resource[(src->boss_base_danmaku_type/*-1*/)].danmaku_callback)(src);
		}
		else
		{
			src->boss_base_danmaku_type = DANMAKU_00;			/* �e�������I�� */
		}
	}
}
