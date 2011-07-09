
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�U�R�͓Z�߂�\��B
---------------------------------------------------------*/


#if 0/* �߂� */
/* �{�X���ʋK�i */
	#define target_x256 			user_data00 	/* �ڕWx���W */
	#define target_y256 			user_data01 	/* �ڕWy���W */
	#define toutatu_wariai256		user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define kougeki_anime_count 	user_data03 	/* �U���A�j���[�V�����p�J�E���^ */
	#define boss_time_out			user_data04 	/* �������� */
	#define boss_base_state777		user_data04 	/* ��������(boss_time_out�Ɠ���) */
//
	#define boss_spell_timer		user_data05 	/* �X�y������ */
#endif

//#define zako_pattern_base 			user_data07 		/* �p�^�[���̊�ʒu�B�A�j������ꍇ�̃x�[�X */
#define zako_anime_rotate_angle1024 	user_data07/*zako_anime_base*/	/* �A�j����]�p�x1024 */

/*---------------------------------------------------------
	��-�{�X�G����
---------------------------------------------------------*/
/*global*/static void item_create_15_rumia(SPRITE *src)
{
	const u8 item_tbl[4] =
	{
		(SP_ITEM_02_P128&0xff), 	/* [F] */
		(SP_ITEM_01_P008&0xff), 	/* [P]�� */
		(SP_ITEM_00_P001&0xff), 	/* [p]�� */
		(SP_ITEM_05_TENSU&0xff),	/* [�_] */
	};
	item_create_num(src, /*item_type*/(SP_GROUP_ITEMS|(item_tbl[(cg_game_difficulty)])), (7) ); 	/* ����΂� */
}
/*global*/static void item_create_14_yokai(SPRITE *src)
{
	item_create_num(src, SP_ITEM_05_TENSU, (7) );	/* ����΂� */	/* �_�����o�� */
}
/*---------------------------------------------------------

---------------------------------------------------------*/


/*global*/static/*static*/ void item_create_00_item(SPRITE *src)	{	item_create_mono(src, SP_ITEM_00_P001  );}
/*global*/static/*static*/ void item_create_01_item(SPRITE *src)	{	item_create_mono(src, SP_ITEM_01_P008  );}
/*global*/static/*static*/ void item_create_02_item(SPRITE *src)	{	item_create_mono(src, SP_ITEM_02_P128  );}
/*global*/static/*static*/ void item_create_03_item(SPRITE *src)	{	item_create_mono(src, SP_ITEM_03_1UP   );}
/*global*/static/*static*/ void item_create_04_item(SPRITE *src)	{	item_create_mono(src, SP_ITEM_04_BOMB  );}
/*global*/static/*static*/ void item_create_05_item(SPRITE *src)	{	item_create_mono(src, SP_ITEM_05_TENSU );}//static void item_create_018_meido4
/*global*/static/*static*/ void item_create_06_item(SPRITE *src)	{	item_create_mono(src, SP_ITEM_06_HOSI  );}
#define item_create_07_item item_create_99_random_item

//static void item_create_018_meido4
//	h->callback_loser		= item_create_05_item;//item_create_018_meido4;
//	h->callback_loser		= item_create_04_item;//item_create_003_tatsumaki1;/*old*/


/*---------------------------------------------------------
	�U�R�G����
---------------------------------------------------------*/

/*global*/static/*static*/ void item_create_000_kougeki_mahoujin(SPRITE *src)
{
	if (rand_percent(30))	{	item_create_mono(src, SP_ITEM_00_P001  );}
	if (rand_percent(30))	{	item_create_mono(src, SP_ITEM_00_P001  );}
	if (rand_percent(30))	{	item_create_mono(src, SP_ITEM_05_TENSU );}
}


/*global*/static/*static*/ void item_create_002_inyou_zako(SPRITE *src)
{
//	if ( 0==(ra_nd()&(16-1)) ) /*�m���グ���B[1/16]��[1/20]*/ /*%20*/
	if ( 0==(ra_nd()&( 8-1)) ) /*�m���グ���B[1/8]��[1/20]*/ /*%20*/
	{
		br.BULLET_REGIST_00_speed256				= t256(5.0);				/* �e�� */
		br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN; 	/* */
	//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
		br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_HARI32_01_AKA; 	/* [�Ԑj�e] */
		br.BULLET_REGIST_06_n_way					= (5);						/* [5way] */
		br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/(24)); 		/* �����p�x */
		bullet_regist_multi_vector_send1_xy_src(src);	/* �e��x256 y256 ���S���甭�e�B */
	}
	else
	{
		item_create_mono(src, (SP_ITEM_00_P001+(ra_nd()&1)) );
	}
}
/*50%(SP_ITEM_00_P001 or SP_ITEM_01_P008)*/
//66%==SP_ITEM_04_BOMB or 33%==SP_ITEM_01_P008	 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),


//static void item_create_003_tatsumaki1(SPRITE *src)
//{
//	destoroy++;
//	if ( (N UM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
//	{
//		destoroy = 0;
//		if (rand_percent(50/*10*/))
//		{
//			item_create(src, (SP_ITEM_04_BOMB), 1, ITEM_MOVE_FLAG_06_RAND_XY);
//		}
//	}
//}
/*50%(SP_ITEM_01_P008 or SP_ITEM_04_BOMB) (SP_ITEM_01_P008+(ra_nd()&1)) */
//66%==SP_ITEM_04_BOMB or 33%==SP_ITEM_01_P008 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),


//static void item_create_017_meido3(SPRITE *src)
//{
//	destoroy[static_last]++;
//	if ( (N UM_OF_ENEMIES-1) < destoroy[static_last]/*all_destoroy*/)
//	{
//		destoroy[static_last] = 0;
//		item_create(src, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
//	}
//}
//		if (0==(ra_nd()&(512-1)))	/* 1/512 �� 1/600 �̊m���m���ŃA�C�e���o�� */	/*%600*/
//		{
//			item_create(src, SP_ITEM_00_P001, 1, ITEM_MOVE_FLAG_06_RAND_XY);
//		}



/*---------------------------------------------------------
	�U�R�V���b�g����
---------------------------------------------------------*/
static void zako_shot_ra_nd(SPRITE *src)
{
	if (ENEMY_LAST_SHOT_LINE256 > src->cy256)	/* ���̃��C����艺����͓G�������Ȃ� */
	{
		/*
		0  0/100   0/65536
		1  1/90  728/65536
		2  1/80  819/65536
		3  1/70  936/65536
		*/
		static const u16 kakuritu_tbl[4] =
		{ 0, 728, 819, 936 };
		if (kakuritu_tbl[((cg_game_difficulty))/*(teki_rank)*/] > (ra_nd()&(65536-1)))
		{
			bullet_regist_multi_vector_send1_xy_src(src);	/* �e��x256 y256 ���S���甭�e�B */
		}
	}
}

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
/* 0000 0100 0000 0000 = 0x0400 = 1024 */
/* 0000 0011 1--- ---- */
static void zako_anime_type_99_rotate(SPRITE *src)
{
	src->rotationCCW1024 += (src->zako_anime_rotate_angle1024);/*�O����]*/
	mask1024(src->rotationCCW1024);
}
static void zako_anime_type01(SPRITE *src)
{
	if (0==(src->zako_anime_rotate_angle1024))
	{
	//	src->type				= ((src->type)&(~0x07)) + ((((src->tmp_angleCCW1024)&(1024-1)))>>7);
		src->type				= ((src->type)&(~0x07)) + ((((src->tmp_angleCCW1024)&(0x0380)))>>7);
	}
	else
	{
		zako_anime_type_99_rotate(src);
	}
}
static void zako_anime_type02(SPRITE *src)
{
	if (0==(src->zako_anime_rotate_angle1024))
	{
		src->type				= ((src->type)&(~0x03)) + ((src->jyumyou>>2)&(4-1));
	}
	else
	{
		zako_anime_type_99_rotate(src);
	}
}
static void zako_anime_type05(SPRITE *src)
{
	if (0==(src->zako_anime_rotate_angle1024))
	{
		src->type				= ((src->type)&(~0x03)) + ((src->jyumyou>>4)&(4-1));
	}
	else
	{
		zako_anime_type_99_rotate(src);
	}
}
static void zako_anime_type03(SPRITE *src)
{
	if (0==(src->zako_anime_rotate_angle1024))
	{
		src->type				= ((src->type)&(~0x01)) + ((src->jyumyou&0x10)>>4);
	}
	else
	{
		zako_anime_type_99_rotate(src);
	}
}
static void zako_anime_type04(SPRITE *src)
{
	src->rotationCCW1024 += (src->zako_anime_rotate_angle1024);/*�O����]*/
	mask1024(src->rotationCCW1024);
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


/*---------------------------------------------------------
	�|�����ꍇ�̃A�C�e���w��
	���{�X�Ƌ��p����K�v������B
---------------------------------------------------------*/

static void regist_items_common(GAME_COMMAND *l, SPRITE *src)
{
	void (*aaa[(16)])(SPRITE *src) =
	{
		item_create_00_item,				item_create_01_item,				item_create_02_item,			item_create_03_item,
		item_create_04_item,				item_create_05_item,				item_create_06_item,			item_create_07_item,
		item_create_99_random_item, 		item_create_000_kougeki_mahoujin,	item_create_002_inyou_zako, 	item_create_99_random_item,
		item_create_99_random_item, 		item_create_99_random_item, 		item_create_14_yokai,			item_create_15_rumia,
	};
	src->callback_loser 	= aaa[(((l->user_item8))&0x0f)];			/* �u�U�R�����ꂽ�ꍇ�̏����v�ɁA�u�����_���e�[�u������A�C�e�������v��ݒ� */
}
/*
src->jyumyou		 ---a bc--
src->yx_an im_frame  yyyy xxxx
src->yx_an im_frame  --1a --bc
*/

/*---------------------------------------------------------
	�|�����ꍇ�̃A�C�e���w��
	���{�X�Ƌ��p����K�v������B
---------------------------------------------------------*/

/*global*/static void regist_settei_common(GAME_COMMAND *l, SPRITE *src)
{
	regist_items_common(l, src);/* ���{�X�Ƌ��p����K�v������B */
	src->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);	/* �f�t�H���g�͂����蔻�肠�� */
	src->m_Hit256R			= ZAKO_ATARI16_PNG; 	/* �O�����炠���蔻������߂�B */
	//
	src->cx256				= ((l->user_x)<<8); 	/* �ݒ�t�@�C������o��x���W�����߂�B */
	src->cy256				= ((l->user_y)<<8); 	/* �ݒ�t�@�C������o��y���W�����߂�B */
	src->base_hp			= ((l->user_hp));		/* �ݒ�t�@�C������̗͂����߂�B */
	src->base_score 		= ((l->user_score));	/* �ݒ�t�@�C������l���X�R�A�����߂�B */
//
	src->type				= (SP_GROUP_TEKI)|((l->user_select_gazou)&0x3f); 	/* �ݒ�t�@�C������摜�����߂�B */
	/*
		DAT�͐l�Ԃ������̂ŁA�Ԉ���Ă��肷��B
		�p�^�[���A�j���[�V������p�̃U�R�L�����ɁA
		��]�n�O�����w�肵���ꍇ�A
		�p�^�[���A�j���[�V�����o���Ȃ��̂ŁA
		�o�O�����L�����ɂȂ��Ă��܂��B
		�����ł��������ꍇ�́A�����I�ɉ�]�n�ɕ␳����B
	*/
	if (11<((l->user_select_gazou)&0x0f))/*(12, 13, 14, 15) */
	{
		src->zako_anime_rotate_angle1024 = (10);	/* ��]���鑬�� */
	}
}

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�U�R�o�^�̋��ʃ��[�`��
	-------------------------------------------------------
---------------------------------------------------------*/

static void regist_zako_common(GAME_COMMAND *l, SPRITE *src)
{
	regist_settei_common(l, src);/* ���{�X�Ƌ��p����K�v������B */
	src->callback_hit_teki	= callback_hit_zako;		/* �u�U�R�Ɏ��e�����������ꍇ�̏����v�ɁA�W���̏�����ݒ� */
}


typedef struct
{
	void (*aaa_init_callback)(GAME_COMMAND *l, SPRITE *src);	/* ������ ���� */
	void (*aaa_move_callback)(SPRITE *sss); 					/* �ړ� ���� */
} SPELL_aaa_RESOURCE;

global void game_command_05_regist_zako(GAME_COMMAND *l)
{
	if (0 != cg.chuu_boss_mode)
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
	SPELL_aaa_RESOURCE aaa_resource[(23)] =
	{
	/* �����w */
		{regist_zako_000_mahoujin1, 		move_kougeki_mahoujin}, 	/* "�����w1"	TEKI_RED_BOOK	 MAHOUJIN A */	/* �΂�܂��U�������w */
		{regist_zako_001_mahoujin2, 		move_kougeki_mahoujin}, 	/* "�����w2"	TEKI_MAHOJIN 	 MAHOUJIN B */	/* �����閂���w */
	/* ���� �A�z�� */
		{regist_zako_002_inyou1,			move_inyou1},				/* "�A�z��1"	TEKI_INYOU_DAI */
		{regist_zako_003_tatsumaki1,		move_tatsumaki1},			/* "����1"		TEKI_TATSUMAKI */
	/* �d�� */
		{regist_zako_004_kakomi1,			move_kakomi1},				/* "�͗d��1"	TEKI_CHOU1 */
		{regist_zako_005_aka_kedama1,		move_aka_kedama1_1st},		/* "�Ԗы�1"	TEKI_CHEN */ 	/* �� */
	/* �ы� */
		{regist_zako_006_inseki1,			move_obake1},				/* "覐�1"		TEKI_NIJI_HOSI */	/* ���̑��U�R */
		{regist_zako_007_yukari2,			move_yukari2},				/* "���ґ�2"	TEKI_NIJI_HOSI */	/* ���̑��U�R */
		{regist_zako_008_midori_kedama1,	move_midori_kedama1},		/* "�Ζы�1"	TEKI_AKA_KEDAMA */
		{regist_zako_009_midori_kedama2,	move_midori_kedama2},		/* "�Ζы�2"	TEKI_AKA_KEDAMA */
		{regist_zako_010_kedama1,			move_kedama1},				/* "�ы�1"		TEKI_HAI_KEDAMA */
		{regist_zako_011_kedama2,			move_kedama2},				/* "�ы�2"		TEKI_HAI_KEDAMA */
	/* [C�d��]���̑��U�R */
		{regist_zako_012_obake1,			move_obake1},				/* "���΂�1"	TEKI_YOUSEI2_5 */
		{regist_zako_013_obake2,			move_obake2},				/* "���΂�2"	TEKI_YOUSEI2_5	"���ы�1"	*/		//;
		{regist_zako_014_karasu1,			move_karasu1},				/* "�G1"		TEKI_YOUSEI2_1 */
	/* [B�d��]���d�� */
		{regist_zako_015_meido1,			move_meido1},				/* "���C�h1"	TEKI_YOUSEI3_1 */
		{regist_zako_016_meido2,			move_meido2},				/* "���C�h2"	TEKI_YOUSEI3_1 */
		{regist_zako_017_meido3,			move_meido3},				/* "���C�h3"	TEKI_YOUSEI3_1 */
		{regist_zako_018_meido4,			move_meido4},				/* "���C�h4"	TEKI_YOUSEI3_1 */
	/* [A�d��]���d�� */
		{regist_zako_019_ao_yousei1,		move_ao_yousei1},			/* "�d��1"	TEKI_YOUSEI1_1 */
		{regist_zako_020_ao_yousei2,		move_ao_yousei2},			/* "�d��2"	TEKI_YOUSEI1_1 */
		{regist_zako_021_ao_yousei3,		move_ao_yousei3},			/* "�d��3"	TEKI_YOUSEI1_1 */
		{regist_zako_022_ao_yousei4,		move_ao_yousei4},			/* "�d��4"	TEKI_YOUSEI1_1 */ //(r33�ł͖��g�p)
	};
	SPRITE *h;
	h							= obj_add_01_teki_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		regist_zako_common(l, h);
	//	(*aaa[ (int)(l->user_i_code) ])(l, h);	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
	//	(*aaa[ (int)(l->user_255_code) ])(l, h);	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
		h->callback_mover = aaa_resource[ (int)(l->user_255_code) ].aaa_move_callback; /* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
		(aaa_resource[ (int)(l->user_255_code) ].aaa_init_callback)(l, h);				/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
	}
}
#include "chuu_boss.h"
