
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�U�R�͓Z�߂�\��B
---------------------------------------------------------*/
//extern void zako_anime_type01(SPRITE *src, int type_base);
//extern void zako_anime_type02(SPRITE *src, int type_base);

#if 0/* �߂� */
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define boss_time_out		user_data03 	/* �������� */
#endif


/*---------------------------------------------------------
	�U�R�ړ�����
---------------------------------------------------------*/
	#define target_x256 			user_data00
	#define target_y256 			user_data01
	#define radius256				user_data02
static void zako_move_type11_radius(SPRITE *src)
{
	src->cx256 = ((sin1024((src->tmp_angleCCW1024))*(src->radius256))>>8)+(src->target_x256);/* CCW�̏ꍇ */
	src->cy256 = ((cos1024((src->tmp_angleCCW1024))*(src->radius256))>>8)+(src->target_y256);
}
	#define speed256				user_data04 	/* ���x */
	#define kaisu_nnn				user_data05 	/* �J��Ԃ����񐔂̃J�E���^ */

/*---------------------------------------------------------
	�U�R�A�j������
---------------------------------------------------------*/
static void zako_anime_type01(SPRITE *src, int type_base)
{
	src->type				= (type_base)+((((src->tmp_angleCCW1024)&(1024-1)))>>7);
}
static void zako_anime_type02(SPRITE *src, int type_base)
{
	src->type				= (type_base)+((src->jyumyou>>2)&(4-1));
}


/*---------------------------------------------------------

---------------------------------------------------------*/
	#define MAHOU_TEKI_wait1		user_data04
	#define MAHOU_TEKI_wait2		user_data05
	#define start_y256				user_data06
#include "zako_000_mahoujin.h"			/* �����w[A/B] */

	#define limit_y256				user_data05 	/* ���z�n�� */
#include "zako_002_inyou1.h"			/* ���� �A�z�� */

//	#define speed256				user_data04 	/* ���x */
	#define vx_r					user_data05 	/* ��]����(��]��) */
	#define px256					user_data06 	/* ���zx�ʒu(���]�p) */
#include "zako_003_tatsumaki1.h"		/* ���� �A�z�� */

//	#define target_x256 			user_data00
//	#define target_y256 			user_data01
//	#define radius256				user_data02
	/* �G�̌��� tmp_angleCCW1024 */
#include "zako_004_kakomi1.h"			/* �d�� */
#include "zako_005_aka_kedama.h"		/* �d�� */


//	#define speed256				user_data04
	#define max_y256				user_data05
#include "zako_007_yukari2.h"			/* �ы� */
	#define target_x256 			user_data00
	#define target_y256 			user_data01
#include "zako_008_midori_kedama1.h"	/* �ы� */
	#define wait1					user_data04 	/* �㉺�ɓ������J�E���^ */
//	#define kaisu_nnn				user_data05 	/* �J��Ԃ����񐔂̃J�E���^ */
#include "zako_009_midori_kedama2.h"	/* �ы� */

	#define target_x256 			user_data00
	#define target_y256 			user_data01
//	#define speed256				user_data04
#include "zako_010_kedama1.h"			/* �ы� */
	#define kougeki_type			user_data05
	#define USE_X_HOUKOU (1)
#include "zako_011_kedama2.h"			/* �ы� */


	/* �G�̌��� tmp_angleCCW1024 */
	#define gra_anime_type			user_data04 	/* �O���^�C�v */
#include "zako_012_obake1.h"			/* [C�d��]���̑��U�R */
//	#define target_x256 			user_data00
//	#define target_y256 			user_data01
//	#define radius256				user_data02
	#define flag1					user_data05
#include "zako_013_obake2.h"			/* [C�d��]���̑��U�R */
	#define USE_X_HOUKOU_karasu (0)
	#define KARASU3_houkou			user_data05
#include "zako_014_karasu1.h"			/* [C�d��]���̑��U�R */


//	#define speed256				user_data04
#include "zako_015_meido1.h"			/* [B�d��]���d�� */
//	#define speed256				user_data04
#include "zako_016_meido2.h"			/* [B�d��]���d�� */
	#define clip_left256			user_data00
	#define clip_right256			user_data01
	#define turnspeed1024			user_data03
//	#define speed256				user_data04
//	#define kaisu_nnn				user_data05 	/* �^�[������� */
#include "zako_017_meido3.h"			/* [B�d��]���d�� */
//	#define speed256				user_data04
#include "zako_018_meido4.h"			/* [B�d��]���d�� */

	/* �G�̌��� tmp_angleCCW1024 */
//	#define speed256				user_data04 	/* ���x */
#include "zako_019_ao_yousei1.h"		/* [A�d��]���d�� */
//	#define kaisu_nnn				user_data05 	/* �e������ */
#include "zako_020_ao_yousei2.h"		/* [A�d��]���d�� */
	#define USE_X_HOUKOU_ao3 (0)
	#define KARASU3_houkou			user_data05
#include "zako_021_ao_yousei3.h"		/* [A�d��]���d�� */
	/* �G�̌��� tmp_angleCCW1024 */
//	#define speed256				user_data04 	/* ���x */
#include "zako_022_ao_yousei4.h"		/* [A�d��]���d�� */


/*
src->jyumyou		 ---a bc--
src->yx_an im_frame  yyyy xxxx
src->yx_an im_frame  --1a --bc
*/
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�U�R�o�^�̋��ʃ��[�`��
	-------------------------------------------------------
---------------------------------------------------------*/


static void regist_zako_common(GAME_COMMAND *l, SPRITE *src)
{
	src->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);	/* �f�t�H���g�͂����蔻�肠�� */
	src->callback_loser 	= lose_random_item; 		/* �u�U�R�����ꂽ�ꍇ�̏����v�ɁA�u�����_���e�[�u������A�C�e�������v��ݒ� */
	src->callback_hit_teki	= callback_hit_zako;		/* �u�U�R�Ɏ��e�����������ꍇ�̏����v�ɁA�W���̏�����ݒ� */
//
	src->cx256				= ((l->user_x)<<8); 	/* �ݒ�t�@�C������o��x���W�����߂�B */
	src->cy256				= ((l->user_y)<<8); 	/* �ݒ�t�@�C������o��y���W�����߂�B */
	src->base_hp			= ((l->user_hp));		/* �ݒ�t�@�C������̗͂����߂�B */
	src->base_score 		= ((l->user_score));	/* �ݒ�t�@�C������l���X�R�A�����߂�B */
//
	src->type				= (SP_GROUP_TEKI)|((l->user_select_gazou)); 	/* �ݒ�t�@�C������摜�����߂�B */
//	src->type				= TEKI_58_RED_BOOK; 	/* �ݒ�t�@�C������O�������߂�B */
	src->m_Hit256R			= ZAKO_ATARI16_PNG; 	/* �O�����炠���蔻������߂�B */
}


typedef struct
{
	void (*aaa_init_callback)(GAME_COMMAND *l, SPRITE *src);	/* �������ړ����� */
	void (*aaa_move_callback)(SPRITE *sss); /* �X�y���J�[�h�ړ����� */
} SPELL_aaa_RESOURCE;

global void game_command_05_regist_zako(GAME_COMMAND *l)
{
	if (0!=chu_boss_mode)
	{
		/* ��-�{�X������ꍇ�ɂ͎G���ǉ����Ȃ��B */
		return;/* error */
	}
	if (23 < l->user_255_code)
	{
		return;/* error */
	}
	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
	/* teki�̐�����ԍ��ŊǗ�(load_stage.c �� ctype_name[]�ɑΉ����Ă���) */
//	void (*aaa[ETYPE_MAX])(GAME_COMMAND *l, SPRITE *src) =
//	void (*aaa[23/*ETYPE_MAX*/])(GAME_COMMAND *l, SPRITE *src) =
	SPELL_aaa_RESOURCE aaa_resource[23/*ETYPE_MAX*/] =
	{
//		regist_zako_dummy,/* [�ԕ���؂�] */	/*NULL*/	/* �����Ō�����Ȃ��ꍇ�́Ateki_error(); */
//	/* ���̑� */
//		regist_zako_dummy,		/* �Q�[�� �S�X�e�[�W �N���A�[ */
//		regist_zako_dummy,/*CTYPE_02_BG_CONTROL*/
//	/* �{�X */
//		regist_zako_dummy,				/* ���� */
//	/* ����G[���^�G] */
//		regist_zako_dummy,				/* ��-�{�X�̗\�� */
	/* �����w */
		{regist_zako_000_mahoujin1, 		move_kougeki_mahoujin}, 	/* "�����w1"	TEKI_58_RED_BOOK	CTYPE_19_MAHOUJIN_A */
		{regist_zako_001_mahoujin2, 		move_kougeki_mahoujin}, 	/* "�����w2"	TEKI_52_MAHOJIN 	CTYPE_20_MAHOUJIN_B */
	/* ���� �A�z�� */
		{regist_zako_002_inyou1,			move_inyou1},				/* "�A�z��1"	TEKI_53_INYOU_DAI */
		{regist_zako_003_tatsumaki1,		move_tatsumaki1},			/* "����1"		TEKI_62_TATSUMAKI */
	/* �d�� */
		{regist_zako_004_kakomi1,			move_kakomi1},				/* "�͗d��1"	TEKI_54_CHOU1 */
		{regist_zako_005_aka_kedama1,		move_aka_kedama1_1st},		/* "�Ԗы�1"	TEKI_56_CHEN */ 	/* �� */
	/* �ы� */
		{regist_zako_006_inseki1,			move_obake1},				/* "覐�1"		TEKI_61_NIJI_HOSI */	/* ���̑��U�R */
		{regist_zako_007_yukari2,			move_yukari2},				/* "���ґ�2"	TEKI_61_NIJI_HOSI */	/* ���̑��U�R */
		{regist_zako_008_midori_kedama1,	move_midori_kedama1},		/* "�Ζы�1"	TEKI_60_AKA_KEDAMA */
		{regist_zako_009_midori_kedama2,	move_midori_kedama2},		/* "�Ζы�2"	TEKI_60_AKA_KEDAMA */
		{regist_zako_010_kedama1,			move_kedama1},				/* "�ы�1"		TEKI_59_HAI_KEDAMA */
		{regist_zako_011_kedama2,			move_kedama2},				/* "�ы�2"		TEKI_59_HAI_KEDAMA */
	/* [C�d��]���̑��U�R */
		{regist_zako_012_obake1,			move_obake1},				/* "���΂�1"	TEKI_28_YOUSEI2_5 */
		{regist_zako_013_obake2,			move_obake2},				/* "���΂�2"	TEKI_28_YOUSEI2_5	"���ы�1"	*/		//;
		{regist_zako_014_karasu1,			move_karasu1},				/* "�G1"		TEKI_24_YOUSEI2_1 */
	/* [B�d��]���d�� */
		{regist_zako_015_meido1,			move_meido1},				/* "���C�h1"	TEKI_36_YOUSEI3_1 */
		{regist_zako_016_meido2,			move_meido2},				/* "���C�h2"	TEKI_36_YOUSEI3_1 */
		{regist_zako_017_meido3,			move_meido3},				/* "���C�h3"	TEKI_36_YOUSEI3_1 */
		{regist_zako_018_meido4,			move_meido4},				/* "���C�h4"	TEKI_36_YOUSEI3_1 */
	/* [A�d��]���d�� */
		{regist_zako_019_ao_yousei1,		move_ao_yousei1},			/* "�d��1"	TEKI_12_YOUSEI1_1 */
		{regist_zako_020_ao_yousei2,		move_ao_yousei2},			/* "�d��2"	TEKI_12_YOUSEI1_1 */
		{regist_zako_021_ao_yousei3,		move_ao_yousei3},			/* "�d��3"	TEKI_12_YOUSEI1_1 */
		{regist_zako_022_ao_yousei4,		move_ao_yousei4},			/* "�d��4"	TEKI_12_YOUSEI1_1 */ //(r33�ł͖��g�p)
//
//		regist_zako_dummy,				/* ETYPE_01_SJIS_TEXT */
//		regist_zako_dummy,				/* ETYPE_02_LOAD_BG */
//	//	regist_zako_dummy,				/* ETYPE_03_PICTURE */		/* ���̕����͏��������������ɂ���(Gu�������)�̂œs���ɂ��p�~ */
	};
	SPRITE *h;
	h							= obj_add_01_teki_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		regist_zako_common(l, h);
	//	(*aaa[ (int)(l->user_i_code) ])(l, h);	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
	//	(*aaa[ (int)(l->user_255_code) ])(l, h);	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
		h->callback_mover = aaa_resource[ (int)(l->user_255_code) ].aaa_move_callback;	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
		(aaa_resource[ (int)(l->user_255_code) ].aaa_init_callback)(l, h);				/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
	}
}
