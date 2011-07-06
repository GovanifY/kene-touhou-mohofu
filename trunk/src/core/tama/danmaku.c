
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
	���@(src)�_���e(dest)�̊p�x(���S�����S)��(dest��tmp_angleCCW65536��)�v�Z�B
	-------------------------------------------------------
	��(src)�̒��S���W����A
	��(dest)�̒��S���W�֑_���p�x���v�Z���A
	��(dest)�̈ꎞ�p�x�ϐ�(tmp_angleCCW65536)�Ɍv�Z����B
	-------------------------------------------------------
	�Ⴆ�΁A��(src)==���@�A��(dest)==�G�e�Ȃ�A���@�_���e�B
---------------------------------------------------------*/

global /*static*/ void tmp_angleCCW65536_src_nerai(SPRITE *src, SPRITE *dest)
{
	dest->tmp_angleCCW65536 	= (atan_65536(src->cy256-dest->cy256, src->cx256-dest->cx256));
	#if (0)
	dest->tmp_angleCCW65536 	-= (ra_nd()) & ((65536/32)-1);/* [��̏C��] ����_��Ȃ��̂ŃQ�[���ɂȂ�Ȃ�����d���Ȃ��B */
	#endif
	#if (1)
	dest->tmp_angleCCW65536 	-= ((65536/64)-1);/* [��̏C��] ����_��Ȃ��̂ŃQ�[���ɂȂ�Ȃ�����d���Ȃ��B */
	#endif
}


/*---------------------------------------------------------
	���@(src)�_���e(dest)�̊p�x(���S�����S)��(dest��tmp_angleCCW1024��)�v�Z�B
---------------------------------------------------------*/

global /*static*/ void tmp_angleCCW1024_src_nerai(SPRITE *src, SPRITE *dest)
{
//	dest->tmp_angleCCW1024		= (at an_1024(src->cy256-dest->cy256, src->cx256-dest->cx256));
//	#if (0)
//	dest->tmp_angleCCW1024  	-= (ra_nd()) & ((1024/32)-1);/* [��̏C��] ����_��Ȃ��̂ŃQ�[���ɂȂ�Ȃ�����d���Ȃ��B */
//	#endif
	tmp_angleCCW65536_src_nerai(src, dest);
	dest->tmp_angleCCW1024  >>= (6);
}
global /*static*/ void tmp_angleCCW1024_jiki_nerai(SPRITE *dest)
{
	SPRITE *zzz_player;
	zzz_player = &obj00[FIX_OBJ_00_PLAYER];
	tmp_angleCCW1024_src_nerai(zzz_player, dest);
}


/*---------------------------------------------------------
	�e����������
---------------------------------------------------------*/

#define DANMAKU_0032_TIME	(32)
#define DANMAKU_0048_TIME	(48)
#define DANMAKU_0064_TIME	(64)
#define DANMAKU_0070_TIME	(70)
#define DANMAKU_0096_TIME	(96)
#define DANMAKU_0128_TIME	(128)
#define DANMAKU_0192_TIME	(192)
#define DANMAKU_0256_TIME	(256)
#define DANMAKU_0640_TIME	(640)
#define DANMAKU_1024_TIME	(1024)
#define DANMAKU_9999_TIME	(16384)

/*---------------------------------------------------------
	�p�x�e�𒼐ڈ���
---------------------------------------------------------*/
#if 1/* �p�x�e�𒼐ڈ����̂ŁA�{�� bullet_angle.c �ɓ���ׂ����́B */

/*
	�p�x�e�ł́A�x�N�g���ړ������Ȃ��B
	����Ɋ�_���W�Ƃ��Ďg���B
*/
#if 1/* �p�x�e�K�i(�����) */
	#define tx256				vx256/* ��_���Wx */
	#define ty256				vy256/* ��_���Wy */
	//
	#define radius256			user_data01 	/* ���a */
//	#define speed256			user_data02 	/* ������ */
	#define speed65536			user_data02 	/* ������ */
	#define tra65536			user_data03 	/* ���������� */
//	#define rotate1024			user_data04 	/* ��]�p�x���� */
//	#define bullet_status_mask	user_data05 	/* ��ʊO��������┽�ˋ@�\ */
#endif

#endif


/*---------------------------------------------------------
	�e���𐶐��B
	-------------------------------------------------------
---------------------------------------------------------*/

#include "danmaku_00_zako.h"
#include "danmaku_00_sonota.h"
//
#include "danmaku_01_mima.h"
#include "danmaku_01_alice.h"
#include "danmaku_01_rumia.h"
#include "danmaku_01_meirin.h"
#include "danmaku_01_chrno.h"
#include "danmaku_01_kaguya.h"
#include "danmaku_01_daiyousei.h"
#include "danmaku_01_aya.h"

//
#include "danmaku_01_pache.h"
#include "danmaku_01_sakuya.h"
#include "danmaku_01_remilia.h"


/*---------------------------------------------------------
	���ʒe��������
	-------------------------------------------------------
	�e�����Ƃɗ\�ߐ������Ԃ��o�^���Ă���B
---------------------------------------------------------*/

typedef struct
{
	void (*danmaku_callback)(SPRITE *sss);		/* �e���������� */
	int danmaku_time;							/* �e���������� */
} DANMAKU_RESOURCE;
//	/*const*/ static void (*danmaku_create_bbb[(DANMAKU_MAX/*16+1*/)])(SPRITE *sss) =
	/*const*/ static DANMAKU_RESOURCE my_danmaku_resource[(DANMAKU_MAX/*16+1*/)] =
{
	#define danmaku_create_99_mitei danmaku_create_10_cirno_misogi
	{	NULL,										DANMAKU_0128_TIME}, 	/* 00 */	/* �e���������Ȃ� */
	// 0x00- ���U�R / �{�X���p�e��
	{	danmaku_create_01_sakuya_misogi,			DANMAKU_0128_TIME}, 	/* 01 */	/* [���] �g5�ʒ��{�X �u�ʏ�U���v����S�e�� */
	{	danmaku_create_02_24nerai,					DANMAKU_0048_TIME}, 	/* 02 */	/* [���] �g5�ʒ��{�X 24�e�� (��p�u�~�X�f�B���N�V�������ǂ�(1/2)�v) */
	{	danmaku_create_03_11nife,					DANMAKU_0064_TIME}, 	/* 03 */	/* [���] �g5�ʒ��{�X 11�e�� (��p�u�~�X�f�B���N�V�������ǂ�(2/2)�v) */
	{	danmaku_create_04_pink_hearts,				DANMAKU_0128_TIME}, 	/* 04 */	/* [���] �g5�ʃ{�X �u�ʏ�U��1(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
	{	danmaku_create_05_32way_dual,				DANMAKU_0128_TIME}, 	/* 05 */	/* [���] �g5�ʃ{�X �u�ʏ�U��1/2(2/2)�v�ɂ�����Ƃ��������e��(�\��) */
	{	danmaku_create_06_luna_clock_32way, 		DANMAKU_0128_TIME}, 	/* 06 */	/* [���] �g5�ʃ{�X ���ہu���i�N���b�N(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
	{	danmaku_create_07_80way_dual_five,			DANMAKU_0128_TIME}, 	/* 07 */	/* [���] �g5�ʃ{�X �u�ʏ�U��3(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
	// 0x08- ���U�R / �{�X���p�e��
	{	danmaku_create_08_rumia_night_bird_zako,	DANMAKU_0128_TIME}, 	/* 08 */	/* [���[�~�A] �g1�ʃ{�X �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������e��(�\��) */
	{	danmaku_create_09_zako_sentakki,			DANMAKU_0128_TIME}, 	/* 09 */	/* [�G��] ����@�e��(�Ƃ肠�����e���V�X�e���쐬�̎����ō���Ă݂��z) */
	{	danmaku_create_1a_dai_yousei_midori,		DANMAKU_0064_TIME}, 	/* 1a */	/* [��d��] �Ί��� */
	{	danmaku_create_1b_dai_yousei_aka,			DANMAKU_0064_TIME}, 	/* 1b */	/* [��d��] �Ԋ��� */
	{	danmaku_create_14_sakuya_jack_oodama32, 	DANMAKU_0256_TIME}, 	/* 14 */	/* [���] ���H�u�W���b�N�E�U�E���h�r���v�ɂȂ邩������Ȃ��e�X�g */
	{	danmaku_create_22_sakuya_miss_direction,	DANMAKU_0256_TIME}, 	/* 1c */	/* [���] ��p�u�~�X�f�B���N�V�����v */
	{	danmaku_create_1f_remilia_kougeki2, 		DANMAKU_0256_TIME}, 	/* 1f */	/* [���~���A] �ʏ�U��2 ���ۂ����� */
	{	danmaku_create_11_tengu_shot,				DANMAKU_0256_TIME}, 	/* 11 */	/* [�V��l] �������̗d��1(�V��l?)�����e�� */
	// 0x10- ���U�R / �{�X���p�e��
	{	danmaku_create_10_cirno_misogi, 			DANMAKU_0070_TIME}, 	/* 10 */	/* [�`���m] �g2�ʃ{�X �`���m�S�e�� */
	{	danmaku_create_13_perfect_freeze,			DANMAKU_0640_TIME}, 	/* 13 */	/* [�`���m] �g2�ʃ{�X �p�[�t�F�N�g�t���[�Y */
	{	danmaku_create_16_diamond_blizzard, 		DANMAKU_0128_TIME}, 	/* 16 */	/* [�`���m] �g2�ʃ{�X �ᕄ�u�_�C�A�����h�u���U�[�h�v */
	{	danmaku_create_25_alice_suwako, 			DANMAKU_0256_TIME}, 	/* 25 */	/* [�A���X] �A���X	�z�K�q���ۂ�(?)�e�� */
	{	danmaku_create_17_alice_nejiri10sec,		DANMAKU_0192_TIME}, 	/* 17 */	/* [�A���X] �ʏ�U��(���ǂ�) �˂���e10�b DANMAKU_0640_TIME  */
	{	danmaku_create_0e_aka_2nd,					DANMAKU_1024_TIME}, 	/* 0e */	/* [�A���X] ��EX�ʃ{�X �Ԃ̖��@2�i�K�ڂ��ǂ� */
	{	danmaku_create_0b_alice_doll,				DANMAKU_0256_TIME}, 	/* 0b */	/* [�A���X] DANMAKU_0128_TIME �d3�ʃ{�X �l�`�e��(���ǂ�)(256>240=4x60) */
	{	danmaku_create_1e_alice_doll,				DANMAKU_0256_TIME}, 	/* 1e */	/* [�A���X] �d3�ʃ{�X �l�`�e��(���ǂ�)(256>240=4x60) �����u�����̕������l�`�v */
	// 0x18- ���U�R / �{�X���p�e��
	{	danmaku_create_0c_hana_test,				DANMAKU_0256_TIME}, 	/* 0c */	/* [����] �ԂĂ��ƒe�� */
	{	danmaku_create_0d_mima_sekkin,				DANMAKU_0256_TIME}, 	/* 0d */	/* [����] �ڋߒe�� */
	{	danmaku_create_24_mima_toge,				DANMAKU_0256_TIME}, 	/* 24 */	/* [����] �g�Q�e�� */
	{	danmaku_create_99_mitei,					DANMAKU_0128_TIME}, 	/* 26 */	/* ����e�� */
	{	danmaku_create_23_kaguya_tamanoe,			DANMAKU_0640_TIME}, 	/* 1d */	/* [�P��] �H���̋ʂ̎}���ǂ� */
	{	danmaku_create_99_mitei,					DANMAKU_0128_TIME}, 	/* 26 */	/* ����e�� */
	{	danmaku_create_18_kaguya01, 				DANMAKU_0064_TIME}, 	/* 18 */	/* [�P��] �Ƃ肠���������ɕϊ� */
	{	danmaku_create_19_kaguya04, 				DANMAKU_0064_TIME}, 	/* 19 */	/* [�P��] �Ƃ肠���������ɕϊ� */
	// 0x20- �{�X��p�e��
	{	danmaku_create_15_aya_misogi1,				DANMAKU_0256_TIME}, 	/* 15 */	/* [��] �̒ʏ�e���ۂ��e���������Ă݂�e�X�g1 */
	{	danmaku_create_23_aya_misogi2,				DANMAKU_0128_TIME}, 	/* 23 */	/* [��]�̒ʏ�e���ۂ��e���������Ă݂�e�X�g2 */
	{	danmaku_create_22_aya_yatimata, 			DANMAKU_0640_TIME}, 	/* 22 */	/* [��] �򕄁u�V�̔��ˁv */
	{	danmaku_create_12_aya_merin_test,			DANMAKU_0640_TIME}, 	/* 12 */	/* [����] �g3�ʒ��{�X �u�ʏ�U���v/ ���ɔ�����ۂ��e���������Ă݂�e�X�g */
	{	danmaku_create_0f_aya_doll, 				DANMAKU_0128_TIME}, 	/* 0f */	/* [��] �l�`�e��(�Ă��ƒe���u�댯�Ȏ�v) */
	{	danmaku_create_0a_houka_kenran, 			DANMAKU_0128_TIME}, 	/* 0a */	/* [�g����] �g3�ʃ{�X �ؕ��u�F�؈�ࣁv�ɂ�����Ƃ��������e��(�\��) */
	{	danmaku_create_26_aya_saifu,				DANMAKU_9999_TIME}, 	/* 26 */	/* [��] �Ǖ� */
	{	danmaku_create_27_pache_laser2, 			DANMAKU_0640_TIME}, 	/* 27 */	/* [�p�`�F] �̃��[�U�[���ǂ� */
	// 0x28- �{�X��p�e��
	{	danmaku_create_28_rumia_night_bird, 		DANMAKU_0128_TIME}, 	/* 30 */	/* [���[�~�A] �g1�ʃ{�X �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������e��(�\��) */
	{	danmaku_create_29_rumia_demarcation,		DANMAKU_0640_TIME}, 	/* 31 */	/* [���[�~�A] �g1�ʃ{�X �ŕ��u�f�B�}�[�P�C�V�����v�ɂ�����Ƃ��������e��(�\��) */
	{	danmaku_create_2a_amefuri_test,				DANMAKU_0640_TIME}, 	/* 26 */	/* �J�Ă��� */
	{	danmaku_create_99_mitei,					DANMAKU_0128_TIME}, 	/* 26 */	/* ����e�� */
	{	danmaku_create_99_mitei,					DANMAKU_0128_TIME}, 	/* 26 */	/* ����e�� */
	{	danmaku_create_99_mitei,					DANMAKU_0128_TIME}, 	/* 26 */	/* ����e�� */
	{	danmaku_create_20_sonota_debug_cw_ao,		DANMAKU_0128_TIME}, 	/* 20 */	/* [���̑�] �f�o�b�O�e��CW�� */
	{	danmaku_create_21_sonota_debug_ccw_aka, 	DANMAKU_0128_TIME}, 	/* 21 */	/* [���̑�] �f�o�b�O�e��CCW�� */
};

/*
dimmercation
Demarcation.
demarcation   ���E

*/


/*---------------------------------------------------------
	�e�����Ԃɗ\�ߓo�^���ꂽ�������Ԃ��Z�b�g����B
---------------------------------------------------------*/

global void danmaku_set_time_out(SPRITE *src)
{
	src->boss_base_danmaku_time_out = my_danmaku_resource[(src->boss_base_danmaku_type/*-1*/)].danmaku_time;
}


/*---------------------------------------------------------
	�e�����P�t���[����������B
	�e����������Ǝ��Ԍo�߂��A�e���������Ԃ������Ō���B
	�e���������Ԃ������Ȃ����ꍇ�́A�e���^�C�v������(DANMAKU_00)�ɂȂ�B
---------------------------------------------------------*/

global void danmaku_generator(SPRITE *src)
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
