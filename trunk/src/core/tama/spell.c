
#include "game_main.h"

/*---------------------------------------------------------
 �����͕핗 �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�X�y�������V�X�e��(�X�y���e������)
	-------------------------------------------------------
	����u�{�X�v�ȊO�ɂ��u���U�R�v�A�u���U�R�v���X�y�������ł���̂ŁA�u�X�y�������v�Ɓu�X�y���J�[�h�Ǘ��v�͕ʃV�X�e���ł��B
	(����u��-�{�X�v�͍��\�肪���邪����ĂȂ��B�J�[�h�����V�X�e�����܂�����ĂȂ�)
	-------------------------------------------------------
	����		�J�[�h����		�X�y���J�[�h	�X�y������
	�{�X		��				��				��	// �{�X
	��-�{�X		�~(?)			��				��	// <�\��>			(�G���̗�����u�~�߂�v�̂��u�~�߂Ȃ��v�̂�����)
	���U�R		�~				�~				��	// ��[�݂� 		�X�y���J�[�h�����ĂȂ��̂ŃJ�[�h��(����)�͖����B
	���U�R		�~				�~				��	// �������U�R		�X�y���J�[�h�����ĂȂ��̂ŃJ�[�h��(����)�͖����B
	�U�R		�~				�~				�~	// ���̑��̃U�R 	�X�y�������o���Ȃ��B
	-------------------------------------------------------
	����			�G������	�X�y���J�[�h
	�{�X��p�R�A	�~			��					// �G�������֘A�̏��������Ȃ��̂ł��̕������B
	������p�R�A	��			�~					// �X�y���J�[�h�Ǘ��֘A�̏��������Ȃ��̂ł��̕������B
	-------------------------------------------------------
	(r33)��L�̐�p�R�A�v��͔p�~�B
	���̕���(r33)���Ƃނ����p�R�A�ɂ����������x�ቺ����B
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
}


/*---------------------------------------------------------
	���@(src)�_���e(dest)�̊p�x(���S�����S)��(dest��tmp_angleCCW1024��)�v�Z�B
---------------------------------------------------------*/

global /*static*/ void tmp_angleCCW1024_src_nerai(SPRITE *src, SPRITE *dest)
{
//	dest->tmp_angleCCW1024		= (at an_1024(src->cy256-dest->cy256, src->cx256-dest->cx256));
	tmp_angleCCW65536_src_nerai(src, dest);
	dest->tmp_angleCCW1024	>>= (6);
}
global /*static*/ void tmp_angleCCW1024_jiki_nerai(SPRITE *dest)
{
	SPRITE *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	tmp_angleCCW1024_src_nerai(zzz_player, dest);
}


/*---------------------------------------------------------
	���������쐬����ꍇ�̃T�u�B
	�ς��� & �`���m & ������ ���Ŕėp�I�Ɏg���Ă�B
	(�V���[�~�A)�Ŏg���B
---------------------------------------------------------*/

static void set_sub_jikinerai(SPRITE *src)
{
	obj_send1->cx256					= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
	obj_send1->cy256					= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
	{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		tmp_angleCCW65536_src_nerai(zzz_player, src);/* ���@�_���p�쐬 */
	}
	br.BULLET_REGIST_02_angle65536		= (src->tmp_angleCCW65536); 	/* ���@�_���e */
}
/*---------------------------------------------------------
	�X�y���e����������
---------------------------------------------------------*/

#define SPELL_TIME_0032 	(32)
#define SPELL_TIME_0048 	(48)
#define SPELL_TIME_0060 	(60)
#define SPELL_TIME_0064 	(64)
#define SPELL_TIME_0070 	(70)
#define SPELL_TIME_0096 	(96)
#define SPELL_TIME_0128 	(128)
#define SPELL_TIME_0192 	(192)
#define SPELL_TIME_0256 	(256)
#define SPELL_TIME_0512 	(512)
#define SPELL_TIME_0640 	(640)
#define SPELL_TIME_1024 	(1024)
#define SPELL_TIME_9999 	(16384)

/*---------------------------------------------------------
	�p�x�e�𒼐ڈ���
---------------------------------------------------------*/
#if 1/* �p�x�e�𒼐ڈ����̂ŁA�{�� bullet_angle.c �ɓ���ׂ����́B */

/*
	�p�x�e�ł́A�x�N�g���ړ������Ȃ��B
	����Ɋ�_���W�Ƃ��Ďg���B
*/
#if 1/* �p�x�e�K�i(�����tama_system) */
	#define tx256				vx256/* ��_���Wx */
	#define ty256				vy256/* ��_���Wy */
	//
#endif

#endif


/*---------------------------------------------------------
	�X�y����Փx�ʃe�[�u��
---------------------------------------------------------*/

	enum
	{
		H0=0,H1,H2,H3,H4,H5,H6
	};
	#define b_HOUGA_00_YEL_NUMS 		(H0*4)
	#define b_HOUGA_08_RED_NUMS 		(H1*4)
	#define HOUGA_04_YEL_DIV_ANGLE		(H2*4)
	#define HOUGA_16_YEL_ROTATE_ANGLE	(H3*4)
	#define HOUGA_12_RED_DIV_ANGLE		(HOUGA_16_YEL_ROTATE_ANGLE)
//
	#define b_chrno_00_BLUE_NUMS		(H4*4)
	#define chrno_04_BLUE_DIV_ANGLE 	(H5*4)
	#define spell_bunkatsu_nums_table spell_nan_ido_table
	const static /*s32*/u16/*s16 �ł��������H*/ spell_nan_ido_table[(H6*4)] =
	{
		// �� �g ����A�ؕ��u�F�؈�ࣁv�p
		// easy 	�ؕ��u�F�؈�ࣁv�Ɠ����B(easy�͎��ԂŊȒP�ɂȂ�悤�ɒ�������)
		// normal	�ؕ��u�F�؈�ࣁv�Ɠ����B
		// hard 	--�B
		// lunatic	�����Ɣ������̊m�F�ς݁B(����ȏ㍂���x���Ɛh������)
		/* easy 				normal					hard					lunatic 			*/
		#if 1/*(r33)*/
		(6),					(6),					(10),					(17),					/* �ؕ��u�F�؈�ࣁv�p */
		(6*8),					(6*8),					(10*8), 				(17*8), 				/* �ؕ��u�F�؈�ࣁv�p */
		(int)(65536/(6)),		(int)(65536/(6)),		(int)(65536/(10)),		(int)(65536/(17)),		/* �ؕ��u�F�؈�ࣁv�p */
		(int)(65536/(6*8)), 	(int)(65536/(6*8)), 	(int)(65536/(10*8)),	(int)(65536/(17*8)),	/* �ؕ��u�F�؈�ࣁv�p */
		#endif
		// �� �`���m�u�v�p
		(0),					(0),					(18),					(36),					/* ��:3���4�̕����ȒP */
		(0),					(0),					(int)(65536/(18)),		(int)(65536/(36)),		/* ��:3���4�̕����ȒP */
	};


/*---------------------------------------------------------
	�X�y���e���𐶐��B
	-------------------------------------------------------
---------------------------------------------------------*/

#include "spell_00_zako.h"
#include "spell_00_sonota.h"
//
#include "spell_01_mima.h"
#include "spell_01_alice.h"
#include "spell_01_rumia.h"
#include "spell_01_meirin.h"
#include "spell_01_chrno.h"
#include "spell_01_kaguya.h"
#include "spell_01_daiyousei.h"
#include "spell_01_aya.h"

//
#include "spell_01_pache.h"
#include "spell_01_sakuya.h"
#include "spell_01_remilia.h"


/*---------------------------------------------------------
	���ʃX�y���e��������
	-------------------------------------------------------
	�X�y���e�����Ƃɗ\�ߐ������Ԃ��o�^���Ă���B
---------------------------------------------------------*/

typedef struct
{
	void (*spell_generate_callback)(SPRITE *sss);		/* �X�y���e���������� */
	int spell_limit_max_time;							/* �X�y���e���������� */
} SPELL_RESOURCE;
//	/*const*/ static void (*spell_create_bbb[(SPELL_MAX/*16+1*/)])(SPRITE *sss) =
	/*const*/ static SPELL_RESOURCE system_spell_resource[(SPELL_MAX/*16+1*/)] =
{
	#define spell_create_99_mitei spell_create_10_cirno_misogi
//	{	NULL,										0			   },	/* 00 */	/* �X�y���e���������Ȃ� */
	{	NULL,										SPELL_TIME_0128},	/* 00 */	/* �X�y���e���������Ȃ� */
	// 0x00- ���U�R / �{�X���p�X�y���e��
	{	spell_create_01_sakuya_misogi_normal,		SPELL_TIME_0128},	/* 01 */	/* [���] �g5�ʒ�-�{�X �u�ʏ�U��1�v����S�e�� */
	{	spell_create_02_24nerai,					SPELL_TIME_0048},	/* 02 */	/* [���] �g5�ʒ�-�{�X 24�e�� (��p�u�~�X�f�B���N�V�������ǂ�(1/2)�v) */
	{	spell_create_03_11nife, 					SPELL_TIME_0064},	/* 03 */	/* [���] �g5�ʒ�-�{�X 11�e�� (��p�u�~�X�f�B���N�V�������ǂ�(2/2)�v) */
	{	spell_create_04_pink_hearts,				SPELL_TIME_0640},	/* 04 */	/* [���] �g5�ʃ{�X �u�ʏ�U��1(1/2)�v�ɂ�����Ƃ��������e��(�\��) SPELL_TIME_0128(r32) */
	{	spell_create_05_32way_dual, 				SPELL_TIME_0128},	/* 05 */	/* [���] �g5�ʃ{�X �u�ʏ�U��1/2(2/2)�v�ɂ�����Ƃ��������e��(�\��) */
	{	spell_create_06_luna_clock_32way,			SPELL_TIME_0128},	/* 06 */	/* [���] �g5�ʃ{�X ���ہu���i�N���b�N(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
	{	spell_create_07_80way_dual_five,			SPELL_TIME_0128},	/* 07 */	/* [���] �g5�ʃ{�X �u�ʏ�U��3(1/2)�v�ɂ�����Ƃ��������e��(�\��) */
	// 0x08- ���U�R / �{�X���p�X�y���e��
	{	spell_create_08_rumia_night_bird/*spell_create_08_rumia_night_bird_zako*/,		SPELL_TIME_0128},	/* 08 */	/* [���[�~�A] �g1�ʃ{�X �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������e��(�\��) */
	{	spell_create_09_zako_sentakki,				SPELL_TIME_0128},	/* 09 */	/* [�G��] ����@�e��(�Ƃ肠�����e���V�X�e���쐬�̎����ō���Ă݂��z) */
	{	spell_create_1a_dai_yousei_midori,			SPELL_TIME_0064},	/* 1a */	/* [��d��] �Ί��� */
	{	spell_create_1b_dai_yousei_aka, 			SPELL_TIME_0064},	/* 1b */	/* [��d��] �Ԋ��� */
	{	spell_create_14_sakuya_jack_oodama32,		SPELL_TIME_0256},	/* 14 */	/* [���] ���H�u�W���b�N�E�U�E���h�r���v�ɂȂ邩������Ȃ��e�X�g */
	{	spell_create_22_sakuya_miss_direction,		SPELL_TIME_0256},	/* 1c */	/* [���] ��p�u�~�X�f�B���N�V�����v */
	{	spell_create_1f_remilia_kougeki2,			SPELL_TIME_0256},	/* 1f */	/* [���~���A] �ʏ�U��2 ���ۂ����� */
	{	spell_create_11_tengu_shot, 				SPELL_TIME_0256},	/* 11 */	/* [�V��l] �������̗d��1(�V��l?)�����e�� */
	// 0x10- ���U�R / �{�X���p�X�y���e��
	{	spell_create_10_cirno_misogi,				SPELL_TIME_0060},	/* 10 */	/* [�`���m] �g2�ʃ{�X �`���m�S�e�� */
	{	spell_create_13_perfect_freeze, 			SPELL_TIME_0640},	/* 13 */	/* [�`���m] �g2�ʃ{�X �p�[�t�F�N�g�t���[�Y */
	{	spell_create_16_diamond_blizzard,			SPELL_TIME_0128},	/* 16 */	/* [�`���m] �g2�ʃ{�X �ᕄ�u�_�C�A�����h�u���U�[�h�v */
	{	spell_create_25_alice_suwako,				SPELL_TIME_0256},	/* 25 */	/* [�A���X] �A���X	�z�K�q���ۂ�(?)�e�� */
	{	spell_create_17_alice_nejiri10sec,			SPELL_TIME_0192},	/* 17 */	/* [�A���X] �ʏ�U��(���ǂ�) �˂���e10�b SPELL_TIME_0640 */
	{	spell_create_0e_aka_2nd,					SPELL_TIME_1024},	/* 0e */	/* [�A���X] ��EX�ʃ{�X �Ԃ̖��@2�i�K�ڂ��ǂ� */
	{	spell_create_0b_alice_doll, 				SPELL_TIME_0256},	/* 0b */	/* [�A���X] SPELL_TIME_0128 �d3�ʃ{�X �l�`�e��(���ǂ�)(256>240=4x60) */
	{	spell_create_1e_alice_doll, 				SPELL_TIME_0256},	/* 1e */	/* [�A���X] �d3�ʃ{�X �l�`�e��(���ǂ�)(256>240=4x60) �����u�����̕������l�`�v */
	// 0x18- ���U�R / �{�X���p�X�y���e��
	{	spell_create_0c_hana_test,					SPELL_TIME_0256},	/* 0c */	/* [����] �ԂĂ��ƒe�� */
	{	spell_create_0d_mima_sekkin,				SPELL_TIME_0256},	/* 0d */	/* [����] �ڋߒe�� */
	{	spell_create_24_mima_toge,					SPELL_TIME_0256},	/* 24 */	/* [����] �g�Q�e�� */
	{	spell_create_1b_cirno_icecle_fall,			SPELL_TIME_0128},	/* 26 */	/* [�`���m] �X���u�A�C�V�N���t�H�[���v */
	{	spell_create_23_kaguya_tamanoe, 			SPELL_TIME_0640},	/* 1d */	/* [�P��] �H���̋ʂ̎}���ǂ� */
	{	spell_create_1d_amefuri_test,				SPELL_TIME_0640},	/* 1d */	/* [(���A��)] �J�Ă��� */
	{	spell_create_18_kaguya01,					SPELL_TIME_0064},	/* 18 */	/* [�P��] �Ƃ肠���������ɕϊ� */
	{	spell_create_19_kaguya04,					SPELL_TIME_0064},	/* 19 */	/* [�P��] �Ƃ肠���������ɕϊ� */
	// 0x20- �{�X��p�X�y���e��
	{	spell_create_15_aya_misogi1,				SPELL_TIME_0256},	/* 15 */	/* [��] �̒ʏ�e���ۂ��e���������Ă݂�e�X�g1 */
	{	spell_create_23_aya_misogi2,				SPELL_TIME_0128},	/* 23 */	/* [��]�̒ʏ�e���ۂ��e���������Ă݂�e�X�g2 */
	{	spell_create_22_aya_yatimata,				SPELL_TIME_0640},	/* 22 */	/* [��] �򕄁u�V�̔��ˁv */
	{	spell_create_12_aya_merin_test, 			SPELL_TIME_0640},	/* 12 */	/* [����] �g3�ʒ�-�{�X �u�ʏ�U���v/ ���ɔ�����ۂ��e���������Ă݂�e�X�g */
	{	spell_create_0f_aya_doll,					SPELL_TIME_0128},	/* 0f */	/* [��] �l�`�e��(�Ă��ƒe���u�댯�Ȏ�v) */
	{	spell_create_0a_houka_kenran,				SPELL_TIME_0128},	/* 0a */	/* [�g����] �g3�ʃ{�X �ؕ��u�F�؈�ࣁv�ɂ�����Ƃ��������e��(�\��) */
	{	spell_create_26_aya_saifu,					SPELL_TIME_9999},	/* 26 */	/* [��] �Ǖ� */
	{	spell_create_99_mitei,						SPELL_TIME_0128},	/* 27 */	/* -------- */
	// 0x28- �{�X��p�X�y���e��
	{	spell_create_08_rumia_night_bird/*spell_create_08_rumia_night_bird*/,			SPELL_TIME_0128},	/* 30 */	/* [���[�~�A] �g1�ʃ{�X �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������e��(�\��) */
	{	spell_create_29_rumia_demarcation,			SPELL_TIME_0512},	/* 31 */	/* [���[�~�A] �g1�ʃ{�X �ŕ��u�f�B�}�[�P�C�V�����v�ɂ�����Ƃ��������e��(�\��) */
	{	spell_create_2a_sakuya_baramaki1,			SPELL_TIME_0640},	/* 26 */	/* [���] �g5�ʒ�-�{�X �΂�T��1 �e�� () */
	{	spell_create_2b_sakuya_baramaki2,			SPELL_TIME_0640},	/* 26 */	/* [���] �g5�ʒ�-�{�X �΂�T��2 �e�� () */
	{	spell_create_2c_sakuya_blue_red_knife,		SPELL_TIME_0512},	/* 26 */	/* [���] �g5�ʒ�-�{�X �u�ʏ�U��3�v�ԃi�C�t */
	{	spell_create_2d_sakuya_misogi_lunatic,		SPELL_TIME_0128},	/* 01 */	/* [���] �g5�ʒ�-�{�X �u�ʏ�U��1�v����S�e��(lunatic only) */
	{	spell_create_2e_pache_agni_shine_1, 		SPELL_TIME_0128},	/* 2e */	/* [�p�`�F] "�@�@�@�@�Ε��u�A�O�j�V���C���v" No.15 */	// (�΂������ς�������) (�P��)�A�O�j(�C���h�_�b�̉΂̐_)
	{	spell_create_2f_pache_princess_undine,		SPELL_TIME_0256},	/* 2f */	/* [�p�`�F] "�@�����u�v�����Z�X�E���f�B�l�v" No.16 */	// (���������ς�������) �u�d���̏�(����:�B���p�t�p���P���X�X)�v�ɓo�ꂷ�鐅�̐��B
	// 0x30- �{�X��p�X�y���e��
	{	spell_create_30_pache_sylphy_horn_1,		SPELL_TIME_0128},	/* 30 */	/* [�p�`�F] "�@�@�@�@�ؕ��u�V���t�B�z�����v" No.17 */	// (�؂������ς�������) (�����i�鐸���)�p�J
	{	spell_create_31_pache_rage_tririton_1,		SPELL_TIME_0128},	/* 31 */	/* [�p�`�F] "�@�@�y���u���C�W�B�g�����g���v" No.18 */	// (�y�������ς�������) (�{��)�g�����g��(�X�g�[���w���W)
	{	spell_create_32_pache_metal_fatigue,		SPELL_TIME_0128},	/* 32 */	/* [�p�`�F] "�@�@�����u���^���t�@�e�B�[�O�v" No.19 */	// (��(����)�������ς�������) ������J
	{	spell_create_33_pache_agni_shine_2, 		SPELL_TIME_0128},	/* 33 */	/* [�p�`�F] "�@�@�Ε��u�A�O�j�V���C���㋉�v" No.20 */	// (�΂������ς�������) (�P��)�A�O�j(�C���h�_�b�̉΂̐_)
	{	spell_create_34_pache_sylphy_horn_2,		SPELL_TIME_0128},	/* 34 */	/* [�p�`�F] "�@�@�ؕ��u�V���t�B�z�����㋉�v" No.21 */	// (�؂������ς�������) (�����i�鐸���)�p�J
	{	spell_create_35_pache_rage_tririton_2,		SPELL_TIME_0128},	/* 35 */	/* [�p�`�F] "�y���u���C�W�B�g�����g���㋉�v" No.22 */	// (�y�������ς�������) (�{��)�g�����g��(�X�g�[���w���W)
	{	spell_create_36_pache_agni_radiance,		SPELL_TIME_0128},	/* 36 */	/* [�p�`�F] "�@�Ε��u�A�O�j���C�f�B�A���X�v" No.23 */	// (�΂������ς������ς�������) �A�O�j(�C���h�_�b�̉΂̐_)
	{	spell_create_37_pache_bury_in_lake, 		SPELL_TIME_0128},	/* 37 */	/* [�p�`�F] "�@�@�@�����u�x���[�C�����C�N�v" No.24 */	// (���������ς������ς�������)  �΂ɖ��߂�(����)
	// 0x38- �{�X��p�X�y���e��
	{	spell_create_38_pache_green_storm,			SPELL_TIME_0128},	/* 38 */	/* [�p�`�F] "�@�@�@�ؕ��u�O���[���X�g�[���v" No.25 */	// (�؂������ς������ς�������) �΂̗�
	{	spell_create_39_pache_tririton_shake,		SPELL_TIME_0128},	/* 39 */	/* [�p�`�F] "�@�@�y���u�g�����g���V�F�C�N�v" No.26 */	// (�y�������ς������ς�������) (�h���)�g�����g��(�X�g�[���w���W)
	{	spell_create_3a_pache_silver_dragon,		SPELL_TIME_0128},	/* 3a */	/* [�p�`�F] "�@�@�@�����u�V���o�[�h���S���v" No.27 */	// (��(����)�������ς������ς�������) �u���Ƌ�(����:���R)�v
	{	spell_create_3b_pache_lava_cromlech,		SPELL_TIME_0128},	/* 3b */	/* [�p�`�F] "�΁��y���u���[���@�N�������N�v" No.28 */	// (�΁{�y���n��) ����(�X�g�[���w���W)
	{	spell_create_3c_pache_forest_blaze, 		SPELL_TIME_0128},	/* 3c */	/* [�p�`�F] "�؁��Ε��u�t�H���X�g�u���C�Y�v" No.29 */	// (�؁{�΁�)�X�щ΍�
	{	spell_create_3d_pache_water_elf,			SPELL_TIME_0256},	/* 3d */	/* [�p�`�F] "�@�����ؕ��u�E�H�[�^�[�G���t�v" No.30 */	// (���{�؁�)���n��(���d��)
	{	spell_create_3e_pache_mercury_poison,		SPELL_TIME_0128},	/* 3e */	/* [�p�`�F] "���������u�}�[�L�����|�C�Y���v" No.31 */	// (��(����)�{����)���⒆��
	{	spell_create_3f_pache_emerald_megalith, 	SPELL_TIME_0128},	/* 3f */	/* [�p�`�F] "�y�������u�G�������h���K���X�v" No.32 */	// (�y�{��(����)��)�G�������h�̋���
//
	{	spell_create_20_sonota_debug_cw_ao, 		SPELL_TIME_0128},	/* 40 */	/* [���̑�] �f�o�b�O�e��CW�� */
	{	spell_create_21_sonota_debug_ccw_aka,		SPELL_TIME_0128},	/* 41 */	/* [���̑�] �f�o�b�O�e��CCW�� */
	{	spell_create_42_pache_laser1,				SPELL_TIME_0640},	/* 27 */	/* [�p�`�F] �ʏ�U��(���ǂ�) �̃��[�U�[���ǂ� */
	{	spell_create_43_pache_laser2,				SPELL_TIME_0640},	/* 27 */	/* [�p�`�F] �ʏ�U��(���ǂ�) �̃��[�U�[���ǂ� */
	{	spell_create_99_mitei,						SPELL_TIME_0640},	/* 26 */	/* ����e�� */
	{	spell_create_99_mitei,						SPELL_TIME_0640},	/* 26 */	/* ����e�� */
	{	spell_create_99_mitei,						SPELL_TIME_0640},	/* 26 */	/* ����e�� */
	{	spell_create_99_mitei,						SPELL_TIME_0640},	/* 26 */	/* ����e�� */


//	{	spell_create_20_sonota_debug_cw_ao, 		SPELL_TIME_0128},	/* 20 */	/* [���̑�] �f�o�b�O�e��CW�� */
//	{	spell_create_21_sonota_debug_ccw_aka,		SPELL_TIME_0128},	/* 21 */	/* [���̑�] �f�o�b�O�e��CCW�� */
};

/*
dimmercation
Demarcation.
demarcation   ���E

*/


/*---------------------------------------------------------
	�X�y���e�����Ԃɗ\�ߓo�^���ꂽ�������Ԃ��Z�b�g����B
---------------------------------------------------------*/

global void spell_set_time_out(SPRITE *src)
{
	src->boss_base_spell_time_out = system_spell_resource[(src->boss_base_spell_type/*-1*/)].spell_limit_max_time;
}


/*---------------------------------------------------------
	�X�y���e�����P�t���[����������B
	�X�y���e����������Ǝ��Ԍo�߂��A�X�y���e���������Ԃ������Ō���B
	�X�y���e���������Ԃ������Ȃ����ꍇ�́A�X�y���e���^�C�v������(SPELL_00)�ɂȂ�B
---------------------------------------------------------*/

global void spell_generator(SPRITE *src)
{
	if (SPELL_00 != src->boss_base_spell_type)		/* �X�y���e�������͕K�v�H */
	{
		src->boss_base_spell_time_out--;			/* ���Ԍo�߂��� */	/*fps_factor*/
		if (0 < src->boss_base_spell_time_out)		/* ���Ԑ؂�H */
		{
			/* �X�y���e�������� */
			(system_spell_resource[(src->boss_base_spell_type/*-1*/)].spell_generate_callback)(src);
		}
		else
		{
			src->boss_base_spell_type = SPELL_00;		/* �X�y���e�������I�� */
		}
	}
}
