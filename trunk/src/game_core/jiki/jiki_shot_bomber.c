
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�e�v���C���[(�얲 �� ������ �� ���~���A �� �H�X�q �� �`���m)
	REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q)
	-------------------------------------------------------
---------------------------------------------------------*/

#include "jiki_local.h"
#define OBJ_Z80_SHOT_SPECIAL				(0x8000)/*0x0100*/		/* �얲�̉�]�V���b�g / kodomo_hlaser�̔��� */

/* �얲 A (���z�c�ޏ�) / ������ A (�X���@�g��) �p�̗U���ڕW���W */
#define PL_SHOT_DATA_target_x256		user_data00
#define PL_SHOT_DATA_target_y256		user_data01

#define PL_SHOT_DATA_SHOT_SPECIAL		user_data03
//#define PL_SHOT_DATA_parrent_jyumyou		user_data03
/*
����
;-------------------
�얲 �아	�V���b�g�i�z�[�~���O�A�~�����b�g�j		�{�� �i���z����j

L0	1way(�܂�������)								P000-P007			(���i1way)
�I�v�V����2�ǉ�									P008-P015
L1	2way(�E��A����)								P016-	28			(�Ԋu������2way)

L3	3way(�܂��������A�E�Ώ�A���Ώ�)				P032-	43			(�Ԋu���L��3way)

Lx	3way(�܂��������A�E�Ώ�A���Ώ�)				P064-	113 	124 (�Ԋu���L��3way)

124 �I�v�V����3����

Lx	4way(�܂������E�A�܂��������A�E�Ώ�A���Ώ�)			MAX(128)	(�Ԋu���L��4way)

;-------------------
�얲 ����	�V���b�g�i�p�X�E�F�C�W�����j�[�h���j	�{�� �i�����w�j

;-------------------
������ ���� �V���b�g�i�}�W�b�N�~�T�C���j			�{�� �i�X�^�[�_�X�g�����@���G�j

;-------------------
������ ���� �V���b�g�i�C�����[�W�������[�U�[�j		�{�� �i�}�X�^�[�X�p�[�N�j

;-------------------
 */

enum/* ���e�ł���V���b�g�̑S��ށB(�j�܂�) */
{
//	/* (�U���V���b�g) */
	REIMU_SHOT_TYPE_00 = 0,
	/* (��U���V���b�g) */
	REIMU_SHOT_TYPE_01,
	REIMU_SHOT_TYPE_02,
	REIMU_SHOT_TYPE_03,
	REIMU_SHOT_TYPE_04,
	REIMU_SHOT_TYPE_05,
	REIMU_SHOT_TYPE_06,
	REIMU_SHOT_TYPE_07,
	REIMU_SHOT_TYPE_08,
	REIMU_SHOT_TYPE_09,
//	/* ��U���V���b�g(�H�X�q) */
	YUYUKO_SHOT_TYPE_08,
	YUYUKO_SHOT_TYPE_09,
	YUYUKO_SHOT_TYPE_0a,
	YUYUKO_SHOT_TYPE_0b,
//	/* �j(�T�u�E�F�|��) */
	NEEDLE_ANGLE_270_M, 	/* �I�v�V�������S���甭�e */
	NEEDLE_ANGLE_270_L, 	/* �I�v�V���������甭�e (�얲B ��p) */
	NEEDLE_ANGLE_270_R, 	/* �I�v�V�����E���甭�e (�얲B ��p) */
	NEEDLE_ANGLE_263,		/* ���~���A �� �`���m ��p */
	NEEDLE_ANGLE_277,		/* ���~���A �� �`���m ��p */
	NEEDLE_ANGLE_271,		/* �H�X�q ��p */
//
	REIMU_SHOT_TYPE_MAX 	/* �ő吔 */
};

enum/* �V���b�g�̐��\�A�ŗL�l�B(�L�����ʂɏ����ݒ肷��) */
{
	// ���@�V���b�g�̋���
	SHOT_STATUS_00_SHOT_STRENGTH_L1 = 0,
	SHOT_STATUS_01_SHOT_STRENGTH_L2,
	SHOT_STATUS_02_SHOT_STRENGTH_L3,
	SHOT_STATUS_03_SHOT_STRENGTH_L4,
	SHOT_STATUS_04_SHOT_STRENGTH_L5,
	SHOT_STATUS_05_SHOT_STRENGTH_L6,
	SHOT_STATUS_06_SHOT_SPEED,	// ���@�V���b�g�̈ړ����x
	SHOT_STATUS_07_HARI_SPEED,	// �I�v�V�����V���b�g�̈ړ����x
	// �I�v�V�����V���b�g�̋���
	SHOT_STATUS_08_HARI_STRENGTH_L1,
	SHOT_STATUS_09_HARI_STRENGTH_L2,
	SHOT_STATUS_0A_HARI_STRENGTH_L3,
	SHOT_STATUS_0B_HARI_STRENGTH_L4,
	SHOT_STATUS_0C_HARI_STRENGTH_L5,
	SHOT_STATUS_0D_HARI_STRENGTH_L6,
	SHOT_STATUS_0E_SHOT_ATARI_HANKEI,	/*(���@�V���b�g�̂����蔻�蔼�a)*/
	SHOT_STATUS_0F_HARI_ATARI_HANKEI,	/*(�I�v�V�����V���b�g�̂����蔻�蔼�a)*/
	// ���@�V���b�g�̍X�V�Ԋu
	SHOT_STATUS_10_JIKI_SHOT_INTERVAL_L1,
	SHOT_STATUS_11_JIKI_SHOT_INTERVAL_L2,
	SHOT_STATUS_12_JIKI_SHOT_INTERVAL_L3,
	SHOT_STATUS_13_JIKI_SHOT_INTERVAL_L4,
	SHOT_STATUS_14_JIKI_SHOT_INTERVAL_L5,
	SHOT_STATUS_15_JIKI_SHOT_INTERVAL_L6,
	SHOT_STATUS_16_STANDARD_BOMB_STRENGTH,	/*(�ʏ�{���̋���)*/
	SHOT_STATUS_17_LOWER_BOMB_STRENGTH, 	/*(�ᑬ�{���̋���)*/
	// �I�v�V�����V���b�g�̍X�V�Ԋu
	SHOT_STATUS_18_OPTION_SHOT_INTERVAL_L1,
	SHOT_STATUS_19_OPTION_SHOT_INTERVAL_L2,
	SHOT_STATUS_1A_OPTION_SHOT_INTERVAL_L3,
	SHOT_STATUS_1B_OPTION_SHOT_INTERVAL_L4,
	SHOT_STATUS_1C_OPTION_SHOT_INTERVAL_L5,
	SHOT_STATUS_1D_OPTION_SHOT_INTERVAL_L6,
	SHOT_STATUS_1E_STANDARD_BOMB_ATARI_HANKEI,	/*(�ʏ�{���̂����蔻�蔼�a)*/
	SHOT_STATUS_1F_LOWER_BOMB_ATARI_HANKEI, 	/*(�ᑬ�{���̂����蔻�蔼�a)*/
	//
	SHOT_STATUS_20_BOMBER_KAITEN_SOKUDO,		/*(��]���x)*/
	SHOT_STATUS_21_BOMBER_KAKUDAI_SOKUDO,		/*(�g�呬�x)*/
	SHOT_STATUS_22_KURAI_BOMB_UKETUKE_JIKAN,	/*(��炢�{���󂯕t�����ԁA�l�������������󂯕t�����Ԃ������B)*/
	SHOT_STATUS_23_PLAYER_GRAZE_ATARI_HANKEI,	/*(�v���C���[�B�O���C�Y�̓����蔻�蔼�a)*/
	SHOT_STATUS_24_PLAYER_SET_BOMBER_TIME,/*(�{���̎���)*/
	SHOT_STATUS_25_PLAYER_ANIME_SPEED,/*(�v���C���[�̃A�j���[�V�������x)*/
	SHOT_STATUS_26_OPTION_ANIME_KAITEN,
	SHOT_STATUS_27_OPTION_ANIME_SPEED,
	//
	SHOT_STATUS_32_MAX		/* �ő吔 */
};

typedef struct /*_SHOT_STATUS_tag_*/
{
	u8 shot_status[SHOT_STATUS_32_MAX];
} SHOT_STATUS;

static SHOT_STATUS current;


/*---------------------------------------------------------
	�v���C���[����̂����蔻��
OBJ *tt:
	�U�R�G�{�́B
	�G�e�B
OBJ *shot:
	���@�̃V���b�g�e�B
	���@�̃{���o�[�B
	���@���̂ő̓�����B
erase_shot_type:
	PLAYER_WEAPON_TYPE_00_SHOT: 		���e���ł���B	�G�e�����Ȃ��B
		�V���b�g�e�B
	PLAYER_WEAPON_TYPE_01_BOMB_PLAYER:	���e���ł��Ȃ��B�G�e�����B
		�{���o�[�B
		���G���ԓ��̎��@�ɂ��̓�����B
---------------------------------------------------------*/

/* static */global void player_weapon_collision_check(OBJ *shot, int erase_shot_type)
{
//	OBJ *shot;	/* ���e */
	OBJ *tt;	/* �ꎞ�g�p�̃e���|����(�G�X�v���C�g�A�܂��́A�G�e�X�v���C�g) */
	#define teki_obj		tt
	#define tekidan_obj 	tt
	/* �G�e�ɂ��������ꍇ�ɓG�e������(�{���n�̂�) */
	if (/* PLAYER_WEAPON_TYPE_01_BOMB_PLAYER== */erase_shot_type)	/* �{���n�̂� */	/* erase_bullets */
	{
		/* ���e�ɂ��������͓̂G�e�Ȃ̂����ׂ�D */
		tekidan_obj = obj_collision_check_00_tama(shot, OBJ_Z04_TAMA);/* �e��p */
		if (NULL != tekidan_obj)		/* �G�e�ɓ��������� */
		{
			tekidan_obj->jyumyou = JYUMYOU_NASI;	/* �G�e������ */
			voice_play_menu(VOICE02_MENU_SELECT, /*(???)*/TRACK05_ZAKO_DEATH);/*TRACK03_IVENT_DAN*/ /*TRA CK01_PICHUN*/
		}
	}
	/* �G�ɂ��������ꍇ�ɓG������ */
	{
		/* ���e�ɂ��������͓̂G���̂Ȃ̂����ׂ�D */
		teki_obj = obj_collision_check_A01_A02_teki(shot);//, (OBJ_Z02_TEKI));
		if (NULL != teki_obj)			/* �G���̂ɓ��������� */
		{
			if (NULL != (teki_obj->callback_hit_teki))	/*	 */
			{
				(teki_obj->callback_hit_teki)(teki_obj/* �G���� */, shot/* ���e */);
			}
			/* ���e���G�ɂ��������ꍇ�Ɏ��e�����ł��� */
			if (PLAYER_WEAPON_TYPE_00_SHOT == erase_shot_type)	/* �V���b�g�n�̂� */	/* erase_player_tama */
			{
			//	/* ���e���ŕ��@������ȏꍇ */
			//	if (NULL != (shot->callback_loser))
			//	{
			//		(shot->callback_loser)(shot/* ���e */);
			//	}
				shot->jyumyou = JYUMYOU_NASI;		/* ���e������ */
				/* �V���b�g���G�ɓ��������ꍇ�A�ł����ݓ_�����Z�B(���G�L�����ł��������ݓ_�ŉ҂���) */
				player_dummy_add_score(score(10));
			}
		}
	}
}

/*---------------------------------------------------------
	�v���C���[�A�j�e�A�V���b�g�e�̈ړ�
	�얲 �� ������ �� ���~���A �� �H�X�q �� �`���m
---------------------------------------------------------*/

/*---------------------------------------------------------
	�V���b�g�e�̂����蔻����`�F�b�N����B
	��ʊO�Ȃ�����B
---------------------------------------------------------*/

static OBJ_CALL_FUNC(check_collision_gamengai)
{
	if (((src->center.y256)+(t256(-10))) < t256(0))/*(��ʊO�ɏo����)*/
	{
		src->jyumyou = JYUMYOU_NASI;/*(�V���b�g�����܂�)*/
	}
	else/*��ʓ��Ȃ�*/
	{
		player_weapon_collision_check(src, PLAYER_WEAPON_TYPE_00_SHOT);/*(�����蔻��`�F�b�N)*/
	}
}

/*---------------------------------------------------------
	���̑��ėp�A�V���b�g�e�̈ړ�
---------------------------------------------------------*/

static OBJ_CALL_FUNC(other_move_object)
{
	src->center.x256 += src->math_vector.x256;		/*fps_factor*/
	src->center.y256 += src->math_vector.y256;		/*fps_factor*/
	check_collision_gamengai(src);
}
/*---------------------------------------------------------
	���~���A��p�A�V���b�g�e�̈ړ�
---------------------------------------------------------*/

static OBJ_CALL_FUNC(remilia_move_object)
{
//	if (REMILIA==(cg_ga me_select_player))	/* ���~���A�͂���� */
	{	u16 rand_int;
		rand_int = ra_nd(); 	/*(���v���C���ɍČ������K�v)*/
	//	rand_int = vfpu_rand_8888(0, 255);
		src->center.x256 -= (rand_int&0x0200);/*0x0100*/
		src->center.x256 += (rand_int&0x0100);/*0x0080*/
		src->center.x256 += (rand_int&0x0100);/*0x0080*/
	}
	#if (0)
	src->center.x256 += src->math_vector.x256;		/*fps_factor*/
	src->center.y256 += src->math_vector.y256;		/*fps_factor*/
	check_collision_gamengai(src);
	#else
	other_move_object(src);
	#endif
}

/*---------------------------------------------------------
	�U���T�u�B
---------------------------------------------------------*/
static void yuudo_aaa(OBJ/**/ *src, int bbb)
{
	#if (1)
	/* �p���[�̗ʂɔ�Ⴕ�ăV���b�g�̋O�Ղ��Ȃ���B */
	src->math_vector.x256 += ((src->math_vector.x256 * bbb)>>11);
	#endif
	//
	#if (1)
	/* �V���b�g��U������B */
	OBJ *obj_boss;
	obj_boss			= &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+TEKI_OBJ_00_BOSS00_HONTAI];/*(�{�X�{��)*/
	if (obj_boss->center.x256 < src->center.x256)/*(�{�X���� < �H�X�q���E)*/
	{
		if (0 < src->math_vector.x256)/*(��==�H�X�q�V���b�g���E�ֈړ���)*/
		{
			src->math_vector.x256 -= src->math_vector.x256;/* �������](������==�H�X�q�V���b�g�����֗U��) */
		}
	//	else/*(��==�H�X�q�V���b�g�����ֈړ���)*/
	//	{;}/*(�␳����K�v�͖���)*/
	}
	else/*(�H�X�q���� > �{�X���E)*/
	{
		if (0 > src->math_vector.x256)/*(��==�H�X�q�V���b�g�����ֈړ���)*/
		{
			src->math_vector.x256 -= src->math_vector.x256;/* �������](������==�H�X�q�V���b�g���E�֗U��) */
		}
	//	else/*(��==�H�X�q�V���b�g���E�ֈړ���)*/
	//	{;}/*(�␳����K�v�͖���)*/
	}
	#endif
}
	#if 0
	/* �U������ */
	if (src->center.x256 < src->PL_SHOT_DATA_target_x256 )
	{
	//	src->center.x256 += src->math_vector.x256;		/*fps_factor*/
	//	src->center.x256 -= src->math_vector.x256;		/*fps_factor*/
	}
	else
	{
		src->center.x256 -= src->math_vector.x256;		/*fps_factor*/
		src->center.x256 -= src->math_vector.x256;		/*fps_factor*/
	}
	#endif

/*---------------------------------------------------------
	�얲A�U�u�g����p�A�V���b�g�e�̈ړ�
---------------------------------------------------------*/
static OBJ_CALL_FUNC(zabuton_move_object)
{
	//[���ʒu]
//	if (REIMU==(cg_ga me_select_player))			/* �얲�̉�]�V���b�g */
//	if ((REIMU_B+1) > (cg_ga me_select_player)) 	/* �얲�̉�]�V���b�g */
	{
	//�U�u�g����p	if (((JIKI_SHOT_01)|OBJ_Z80_SHOT_SPECIAL)==src->obj_type_set)
		{
			/* �`��p�O����] */
			src->rotationCCW1024 += (32);	/* cv1024r(10) */
			mask1024(src->rotationCCW1024);
		}
	}
	yuudo_aaa(src, (128) );
	#if (0)
	src->center.x256 += src->math_vector.x256;		/*fps_factor*/
	src->center.y256 += src->math_vector.y256;		/*fps_factor*/
	check_collision_gamengai(src);
	#else
	other_move_object(src);
	#endif
}

/*---------------------------------------------------------
	�H�X�q��p�A�V���b�g�e�̈ړ�
---------------------------------------------------------*/
/* �H�X�q�͂���� */

static OBJ_CALL_FUNC(yuyuko_move_object)
{
	static unsigned short pow_low;
	pow_low++;
	if ((1024) < pow_low)/* 1024���ړ����邲�ƂɌ䕠���� */
	{
		pow_low = 0;
		if (0 < cg.weapon_power)/* �p���[������� */
		{
			cg.weapon_power--;/* ���炷 */
		}
	}
	//[���ʒu]
	yuudo_aaa(src, (cg.weapon_power) );
	//
	#if (0)
	src->center.x256 += src->math_vector.x256;		/*fps_factor*/
	src->center.y256 += src->math_vector.y256;		/*fps_factor*/
	check_collision_gamengai(src);
	#else
	other_move_object(src);
	#endif
}

/* ���C���V���b�g(�D) / �T�u�E�F�|��(�j) */
#define reimu_a_move_shot	zabuton_move_object/*(�����c��p)*/
#define reimu_a_move_hari	other_move_object/*(�ėp)*/
#define reimu_b_move_shot	other_move_object/*(�ėp)*/
#define reimu_b_move_hari	other_move_object/*(�ėp)*/
#define marisa_a_move_shot	other_move_object/*(�ėp)*/
#define marisa_a_move_hari	other_move_object/*(�ėp)*/
#define marisa_b_move_shot	other_move_object/*(�ėp)*/
#define marisa_b_move_hari	other_move_object/*(�ėp)*/

#define remilia_move_shot	remilia_move_object/*(���~���A��p)*/
#define remilia_move_hari	remilia_move_object/*(���~���A��p)*/
#define yuyuko_move_shot	yuyuko_move_object/*(�H�X�q��p)*/
#define yuyuko_move_hari	yuyuko_move_object/*(�H�X�q��p)*/
#define chrno_a_move_shot	other_move_object/*(�ėp)*/
#define chrno_a_move_hari	other_move_object/*(�ėp)*/
#define chrno_q_move_shot	other_move_object/*(�ėp)*/
#define chrno_q_move_hari	other_move_object/*(�ėp)*/

/*---------------------------------------------------------

---------------------------------------------------------*/

enum
{
	REI00_x_offset = 0,
	REI01_y_offset,
	REI02_vx256,
	REI03_status,	/* 2==�얲�̉�]�V���b�g */
	REI04_MAX		/* �ő吔 */
};

/*
���� �g�������B
;-------------------
�얲A�̎D�O���͏�ɂ�������]���Ă���B��]�����͋t������]���x�͋��ɓ����B
��D(�V���b�g)�́A�����v���(CCW)�B
�U���D(�T�u�V���b�g)�́A���v���(CW)�B(�z�[�~���O�A�~�����b�g)
;-------------------
�얲A�̏ꍇ�A
��D(�V���b�g)�́A������(���Ȃ蔖���F)�B
�U���D(�T�u�V���b�g)�A������(���Ȃ蔖���F)�B(�z�[�~���O�A�~�����b�g)
;-------------------
�͕핗�� �얲A �ł́A��]����U���D�͖����B
����ɁA��]���Ȃ��z�[�~���O�A�~�����b�g�ɂȂ��Ă���B
*/

//global void player_register_shot_object(OBJ/**/ *src, int /*set_*/shot_type);
/*static*/static/*global*/ void player_register_shot_object(OBJ/**/ *src, int /*set_*/shot_type)
{
	OBJ *h;/* shot */
	h					= obj_regist_teki();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		static const s8 r_tbl[REIMU_SHOT_TYPE_MAX][REI04_MAX] =
		{	/*x_offset256	y_offset256  vx256 status */
	//	/* �U���V���b�g(�얲 �� ������) */
	/*00*/	{ (  0),	(  0),	(	0), 	0x01},	/* */
	//	/* ��U���V���b�g(�얲) */
	/*01*/	{ (  0),	(  0),	(	0), 	0x00},	/* �� �܂����� */
	/*02*/	{ ( -5),	(  0),	(	0), 	0x00},	/* �� �܂����� */
	/*03*/	{ ( +5),	(  0),	(	0), 	0x00},	/* �E �܂����� */
	/*04*/	{ ( -5),	(  0),	( -20), 	0x00},	/* �� ���� */
	/*05*/	{ ( +5),	(  0),	(  20), 	0x00},	/* �E �E�� */
	/*06*/	{ ( -5),	(  0),	( -35), 	0x00},	/* �� ���Ώ�(��) */
	/*07*/	{ ( +5),	(  0),	(  35), 	0x00},	/* �E �E�Ώ�(��) */
	/*06*/	{ ( -5),	(  0),	( -88), 	0x00},	/* �� ���Ώ�(�L) */
	/*07*/	{ ( +5),	(  0),	(  88), 	0x00},	/* �E �E�Ώ�(�L) */
	//	/* ��U���V���b�g(�H�X�q) */
	/*08*/	{ (-15),	(-25),	( -25), 	0x00},	 /* �H�X�q */					 /*(-32)==(4*(- 8))*/
	/*09*/	{ (+15),	(-25),	( +25), 	0x00},	 /* �H�X�q */					 /*( 32)==(4*(	8))*/
	/*0a*/	{ (-25),	(-25),	( -50), 	0x00},	 /* �H�X�q */					 /*(-64)==(4*(-16))*/
	/*0b*/	{ (+25),	(-25),	( +50), 	0x00},	 /* �H�X�q */					 /*( 64)==(4*( 16))*/
	//		/* CCW�̏ꍇ */
			{ (  0),	( -7),	(	0), 	0x01},// NEEDLE_ANGLE_270_M 	cv1024r((180)), 			/* ������ A/B �얲 ���~���A �� �`���m */
			{ ( -5),	( +4),	(	0), 	0x01},// NEEDLE_ANGLE_270_L 	cv1024r((180)), 			/* �I�v�V���������甭�e (�얲B ��p) */
			{ ( +5),	( +4),	(	0), 	0x01},// NEEDLE_ANGLE_270_R 	cv1024r((180)), 			/* �I�v�V�����E���甭�e (�얲B ��p) */
			//
			{ (  0),	( -7),	(-108), 	0x01},// NEEDLE_ANGLE_263	cv1024r((180-25)),/*263=270-7*/ /* ���~���A �� �`���m */
			{ (  0),	( -7),	( 108), 	0x01},// NEEDLE_ANGLE_277	cv1024r((180+25)),/*277=270+7*/ /* ���~���A �� �`���m */
			{ (  0),	(-47),	(	5), 	0x01},// NEEDLE_ANGLE_271	cv1024r((181)), 				/* �H�X�q��p */	/*-48*/ /*t256( 0), t256(-40),*/
		};
	//	int shot_type;
		int shot_status_hari;/* �j? */
		shot_status_hari = ((r_tbl[shot_type][REI03_status]));

		int is_needle;/* �j? */
	//	is_needle = (shot_type <= NEEDLE_ANGLE_270)?(1/* �j */):(0/* �e */);
		is_needle = ((shot_status_hari)&0x01);
	//
	//	h->m_Hit256R			= ((base_shot_atari[(OFFS_IS_NEEDLE)])<<8);/* �����蔻��̑傫�� */
		h->m_Hit256R			= (((int)current.shot_status[SHOT_STATUS_0E_SHOT_ATARI_HANKEI+is_needle])<<8);/* �����蔻��̑傫�� */

	//	h->type 			= (/*OBJ_Z01_JIKI_GET_ITEM*/JI KI_SHOT_02|OBJ_Z80_SHOT_SPECIAL)-shot_status_hari;
		#if 1
	//	else
	//	if (0x01== shot_status) 	/* �j */
		{
			h->obj_type_set 			= (JIKI_SHOT_07|OBJ_Z80_SHOT_SPECIAL);	/*OBJ_Z01_JIKI_GET_ITEM*/	/*SP_PL_FIREBALL*/
		}
	//
		if (0x00 == shot_status_hari)	/* �j�ȊO */
		{
			if (REIMU_B+1 > (cg_game_select_player))	/* �얲? */
			{
			//	if (0x02== shot_status_hari) /* �얲�̉�]�V���b�g */
				if (REIMU_SHOT_TYPE_00 != shot_type)/* �U���V���b�g�ȊO�H */
				{
					h->obj_type_set 			= (JIKI_SHOT_01|OBJ_Z80_SHOT_SPECIAL);	/*OBJ_Z01_JIKI_GET_ITEM*/
					/* �`��p�O����] */
					/*(���v���C���ɍČ������K�v�Ȃ�)*/
				//	h->rotationCCW1024	= (ra_nd() & (1024-1));/* �o�����̊p�x�̓����_���łȂ���(���[�U�[�݂�����)�ςȉ�ʂɂȂ� */
					h->rotationCCW1024	= (vfpu_rand_8888(0, 255) & (1024-1));/* �o�����̊p�x�̓����_���łȂ���(���[�U�[�݂�����)�ςȉ�ʂɂȂ� */
				}
			/*	else	{	h->type = �U���V���b�g�́A�j�Ɠ���; 	}	*/
			}
			else
			{
				h->obj_type_set 			= (JIKI_SHOT_02|OBJ_Z80_SHOT_SPECIAL);	/*OBJ_Z01_JIKI_GET_ITEM*/
			}
		}
	//	else
		#endif
		#if 1/*Gu(���S���W)*/
		h->center.x256			= (src->center.x256) + ((r_tbl[shot_type][REI00_x_offset])<<8); /*+ x_offs*/
		h->center.y256			= (src->center.y256) + ((r_tbl[shot_type][REI01_y_offset])<<8);/*(20)*/
		#endif
	//	h->callback_mover	= player_move_shot;
		void (*player_move_shot_table[16])(OBJ/**/ *src) =
		{
			reimu_a_move_shot,		/* �얲A */
			reimu_b_move_shot,		/* �얲B */
			marisa_a_move_shot, 	/* ������A */
			marisa_b_move_shot, 	/* ������B */
			remilia_move_shot,		/* ���~���A */
			yuyuko_move_shot,		/* �H�X�q */
			chrno_a_move_shot,		/* �`���m A */
			chrno_q_move_shot,		/* �`���m Q */
		//
			reimu_a_move_hari,		/* �얲A */
			reimu_b_move_hari,		/* �얲B */
			marisa_a_move_hari, 	/* ������A */
			marisa_b_move_hari, 	/* ������B */
			remilia_move_hari,		/* ���~���A */
			yuyuko_move_hari,		/* �H�X�q */
			chrno_a_move_hari,		/* �`���m A */
			chrno_q_move_hari,		/* �`���m Q */
		};
		h->callback_mover	= *player_move_shot_table[(((cg_game_select_player)+(is_needle<<3)))];
		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x96);	/*	h->alpha		= 0x96; 150 �������ɂ���B */
	//

	//	h->kougeki_ti = ddd_tbl[DDD_STRENGTH_L1+(cg.jiki_weapon_level_offset)];
	//	h->kougeki_ti = ccc_tbl[CCC_STRENGTH_L1+(cg.jiki_weapon_level_offset)+(is_needle<<(4+2))];/* 64 == 8[players]*8[line] */
		/*(r35u2-)*/
		h->kougeki_ti = (((int)current.shot_status[SHOT_STATUS_00_SHOT_STRENGTH_L1+(cg.jiki_weapon_level)+(is_needle<<(3))])<<2);/* 64 == 8[players]*8[line] */
		/*
			�U���l: �{���o�[�̈З́B
		*/
	//
	//	int ok;
	//	ok=0;
		{
			#if 0
		//	if (1==r_tbl[shot_type][REI03_yudo])	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
			if (0 != r_tbl[shot_type][REI03_yudo])/* �U���V���b�g���H */
			#else
		//	if (REIMU_SHOT_TYPE_01 > shot_type)/* �U���V���b�g���H */
			if (REIMU_SHOT_TYPE_00 == shot_type)/* �U���V���b�g���H */
			#endif
			{
				OBJ *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
				OBJ *target;
				target = obj_teki_search();
				if (target != zzz_player)/* ���������� */
				{
					h->PL_SHOT_DATA_target_x256  = target->center.x256;/* �U���ڕW���W���o���Ƃ� */
				//	h->PL_SHOT_DATA_target_y256  = target->center.y256;/* �U���ڕW���W���o���Ƃ� */
					//
				//	if (REIMU_SHOT_TYPE_00 == shot_type)/* �U���V���b�g���H */
					{
						REG_00_SRC_X	= (target->center.x256);			/*(�_����)*/
						REG_01_SRC_Y	= (target->center.y256);			/*(�_����)*/
						REG_02_DEST_X	= (zzz_player->center.x256);		/*(�_����)*/
						REG_03_DEST_Y	= (zzz_player->center.y256);		/*(�_����)*/
						tmp_angleCCW65536_src_nerai();
						int int_angle1024;
						int_angle1024	= HATSUDAN_03_angle65536;
						int_angle1024 >>= (6);
					//	if (511 < int_angle1024 )/* ���180-360�x[/360�x]�̗̈�̂ݒǔ����� */
						if (/*(384)*/(256*3) < (( int_angle1024+128+256)&(1024-1)) )/* ���225-315�x[/360�x]�̗̈�̂ݒǔ����� */	/* CCW�̏ꍇ */
						{
							/*16==ccc_tbl[CCC_SPEED+(cg_ga me_select_player)]*/
							/*16==ccc_tbl[CCC_SPEED+(cg_ga me_select_player)]*/
							#if (0)//
							h->math_vector.x256  = si n1024((int_angle1024))*(/*p->speed*/(16));/*fps_factor*/		/* CCW�̏ꍇ */
							h->math_vector.y256  = co s1024((int_angle1024))*(/*p->speed*/(16));/*fps_factor*/
							#else
							{
								int sin_value_t256; 		//	sin_value_t256 = 0;
								int cos_value_t256; 		//	cos_value_t256 = 0;
								int256_sincos1024( (int_angle1024), &sin_value_t256, &cos_value_t256);
								h->math_vector.x256  = sin_value_t256*(/*p->speed*/(16));/*fps_factor*/
								h->math_vector.y256  = cos_value_t256*(/*p->speed*/(16));/*fps_factor*/
							}
							#endif
							#if 1
							/* �`��p�O����] */
							if (MARISA_A==(cg_game_select_player))/* (������A������)�}�W�b�N�~�T�C���͌X���Ȃ��B */
							{	int_angle1024 = (0);	}
							else/* (�얲A�아��)�z�[�~���O�A�~�����b�g�͖͕핗�ł͌X���鎖�ɂ���B */
							{
							//	h->rotationCCW1024	= int_angle1024;
								/* �`��p�O����](�V���b�g�͏オ���ʂȂ̂ŁA��]�p���v�Z)[180/360�x]�񂷁B */
								int_angle1024 += cv1024r((180));
								mask1024(int_angle1024);
							}
							h->rotationCCW1024	= int_angle1024;/* �������ʂ̊p�x���`��p�p�x */
							#endif
						//	ok=1;/* �����ǔ��\ */
							return;
						}
					}
				}
			}
		}
	//
	//	if (0==ok)/* �����ǔ��s�H */
		{	/* �ŗL�̐ݒ�p�x���g�� */
		//	h->math_vector.x256 = ((/*int_vx*/r_tbl[shot_type][REI02_vx256]))*/*p->speed*/(ccc_tbl[CCC_SPEED+OFFS_IS_NEEDLE]);/*fps_factor*/
		//	h->math_vector.y256 = -(((int)ccc_tbl[CCC_SPEED+OFFS_IS_NEEDLE])<<8);/*fps_factor*/ /*p->speed*/
			h->math_vector.x256 = ((/*int_vx*/r_tbl[shot_type][REI02_vx256]))*/*p->speed*/(current.shot_status[SHOT_STATUS_06_SHOT_SPEED+is_needle]);/*fps_factor*/
			h->math_vector.y256 = -(((int)current.shot_status[SHOT_STATUS_06_SHOT_SPEED+is_needle])<<8);/*fps_factor*/ /*p->speed*/
		}
	}
}


/*---------------------------------------------------------
	�v���C���[�e(�V���b�g�̂�)����
---------------------------------------------------------*/

/* �얲A (�U���ޏ�) �U���V���b�g */

static void shot_regist_re_yudou(OBJ *s) /* �얲A ������A */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_00);/* �U���V���b�g */
}

static void shot_regist_yuyuko(OBJ *s)	/* �H�X�q */
{
	player_register_shot_object(s, NEEDLE_ANGLE_271);
}

/* �얲B (�j�ޏ�) / ������A/B / ���~���A �� �`���m */

static void shot_regist_re_hari(OBJ *s) /* ������A/B */
{
	player_register_shot_object(s, NEEDLE_ANGLE_270_M);
}

/*---------------------------------------------------------
	�I�v�V�����̍U���𐬒��i�K�ɂ���Ă�����B
	-------------------------------------------------------
	 �얲 B(3�j�ɐ���)
	���~���A
	�`���m A
	�`���m Q
---------------------------------------------------------*/
static void shot_regist_ci_gggg(OBJ *s) /* �얲 B �� ���~���A �� �`���m */
{
	if (REIMU_B==(cg_game_select_player))/* �얲B��3�j�ɐ�������B */
	{	/*(3�j)*/	/* �얲 B�̂� */
		player_register_shot_object(s, NEEDLE_ANGLE_270_L );
		player_register_shot_object(s, NEEDLE_ANGLE_270_M );
		player_register_shot_object(s, NEEDLE_ANGLE_270_R );
	}
	else	/* �� �`���m */
//	if (REMILIA==(cg_game_select_player))/* */
	{	/*(���E�ɕ������)*/	/* ���~���A�̂� */
		player_register_shot_object(s, NEEDLE_ANGLE_277 );
		player_register_shot_object(s, NEEDLE_ANGLE_263 );
	}
}
static void shot_regist_ma_lazer(OBJ *s) /* ������B ���[�U�[ */
{
	{
		static u8 count128;
		count128++;
	//	count128 &= 0x7f;
		/* -110 0000 */
		#if 0
	//	if (0x00==(count128&0x60))	/* ���͂����A1/4���Ԃ������ʂ��Ȃ��B */
		#else
		count128 &= 0x7f;
	//	if ((43)>(count128 ))	/* ���͂����A1/3���Ԃ������ʂ��Ȃ��B(r33t4) */
		if ((26)>(count128 ))	/* ���͂����A1/5���Ԃ������ʂ��Ȃ��B(r33u1) */
		#endif
		{
			shot_regist_re_hari(s);
		}
	}
}


/*---------------------------------------------------------
	�v���C���[�e(�V���b�g�̂�)����
---------------------------------------------------------*/

	/* �ʏ�V���b�g(narrow 1way�̏ꍇ) */
static void regist_n1_way(OBJ *s)
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* �ʏ�V���b�g */
}
	/* �ʏ�V���b�g(narrow 2way�̏ꍇ) */
static void regist_n2_way(OBJ *s)
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* �ʏ�V���b�g */
}
	/* �ʏ�V���b�g(narrow 3way�̏ꍇ) */
static void regist_n3_way(OBJ *s)
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* �ʏ�V���b�g */
}
	/* �ʏ�V���b�g(narrow 4way�̏ꍇ) */
static void regist_n4_way(OBJ *s)
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* �ʏ�V���b�g */
}
	/* �ʏ�V���b�g(narrow 5way�̏ꍇ) */
static void regist_n5_way(OBJ *s)
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_06); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_07); 	/* �ʏ�V���b�g */
}

/*---------------------------------------------------------
	�얲A�p(�V���b�g�e)����
---------------------------------------------------------*/

	/* �ʏ�V���b�g(wide 4way�̏ꍇ) */
static void regist_w4_way(OBJ *s)
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_06); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_07); 	/* �ʏ�V���b�g */
}
	/* �ʏ�V���b�g(wide 5way�̏ꍇ) */
static void regist_w5_way(OBJ *s)
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_08); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_09); 	/* �ʏ�V���b�g */
}
	/* �ʏ�V���b�g(wide 6way�̏ꍇ) */
static void regist_w6_way(OBJ *s)
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_08); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_06); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_07); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_09); 	/* �ʏ�V���b�g */
}

/*---------------------------------------------------------
	�H�X�q�p(�V���b�g�e)����
---------------------------------------------------------*/
	/* �ʏ�V���b�g(�H�X�q�̏ꍇ) */
static void regist_0_way(OBJ *s)
{
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_08);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_09);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_0a);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_0b);
}


/*---------------------------------------------------------
	�I�v�V�����̍U���𐬒��i�K�ɂ���Ă�����B
	-------------------------------------------------------
	 �얲 B(3�j�ɐ���)
	���~���A
	�`���m A
	�`���m Q
---------------------------------------------------------*/
/*
 WEAPON_L0(P000-P008)
 WEAPON_L1(P008-P015)
 WEAPON_L2(P016-P031)
 WEAPON_L3(P032-P063)
 WEAPON_L4(P064-P127)
 WEAPON_L5(P128)
*/
/*static*/static OBJ_CALL_FUNC(shot_regist_option_glow)
{	/* �U���x�� */
	if (src->PL_OPTION_DATA_opt_anime_add_id < OPTION_C3)
	{
	// OPTION_C1: OPTION_C2:
	//	if (cg.weapon_power > (100-1))	/* ���V�X�e���̒����l 100 */
		if (cg.weapon_power > (96)) 	/* �V�X�e�����ς�����̂ŏC�����Ă݂�(96==128-32) */
		{
			shot_regist_ci_gggg(src);	//	src->PL_OPTION_DATA_opt_shot_interval=(7+3);
		}
		else
		{
			shot_regist_re_hari(src);	//	src->PL_OPTION_DATA_opt_shot_interval=(5+3);
		}
	}
	else
	{
	// OPTION_C3: OPTION_C4:
		shot_regist_re_hari(src);		//	src->PL_OPTION_DATA_opt_shot_interval=(5+3);
	}
}

/*---------------------------------------------------------
	�I�v�V���� �A�j���[�V����
---------------------------------------------------------*/

static OBJ_CALL_FUNC(player_animate_option)
{
//	if (0==anime_fix_status[BASE_OPT_ANIM_TYPE+(cg_ga me_select_player)])
	if (0==current.shot_status[SHOT_STATUS_26_OPTION_ANIME_KAITEN])
	{	/* �I�v�V��������]����ꍇ */
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
	//	src->rotationCCW1024 += (anime_fix_status[BASE_OPT_ANIM_SPEED+(cg_ga me_select_player)]);	/* �`��p�O����]���x */
		src->rotationCCW1024 += (current.shot_status[SHOT_STATUS_27_OPTION_ANIME_SPEED]);	/* �`��p�O����]���x */
		mask1024(src->rotationCCW1024);
	}
	else
	{	/* �I�v�V�������A�j���[�V��������ꍇ */
		src->PL_OPTION_DATA_anime_wait--;
		if (src->PL_OPTION_DATA_anime_wait <= 0)
		{
		//	src->PL_OPTION_DATA_anime_wait = (anime_fix_status[BASE_OPT_ANIM_SPEED+(cg_ga me_select_player)]);
			src->PL_OPTION_DATA_anime_wait = (current.shot_status[SHOT_STATUS_27_OPTION_ANIME_SPEED]);
			#if 1
			src->obj_type_set++;
			src->obj_type_set &= (0xfffffff7); /* ��ʃr�b�g�ɉe����^���Ȃ��ŃA�j���[�V���� */
			#endif
		}
	}
}


/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�����
---------------------------------------------------------*/
/*static*/extern OBJ_CALL_FUNC(player_control_option);	/* �S�� */
/*static*/global OBJ_CALL_FUNC(player_move_option)		/* �S�� */
{
	/* �I�v�V��������\���̏ꍇ�A�������Ȃ��B */
	if (PL_OPTION_FLAG_00_OPTION_OFF==(src->PL_OPTION_DATA_yuukou_flag)) {	return; 	}
//
	/* �A�j���[�V���� */
	player_animate_option(src);
//
	/* �V�i���I��b���[�h�ł̓V���b�g�{�^�����͖��� */
//	if (0==(cg.state_flag & STATE_FLAG_0x0200_IS_KAIWA_MODE))
	if (cg.state_flag & JIKI_FLAG_0x0080_SHOT_KEY_SEND_FOR_OPTION)/* off(�I�v�V�������V���b�g�����ꍇ) */
	{
		src->PL_OPTION_DATA_opt_shot_interval--;
		if ((src->PL_OPTION_DATA_opt_shot_interval) < 0)
		{
		//	src->PL_OPTION_DATA_opt_shot_interval = player_bbb_status[(cg.jiki_weapon_level_offset)];/* �I�v�V�����̃V���b�g�Ԋu */
			src->PL_OPTION_DATA_opt_shot_interval = current.shot_status[SHOT_STATUS_18_OPTION_SHOT_INTERVAL_L1+(cg.jiki_weapon_level)];/* �I�v�V�����̃V���b�g�Ԋu */
			#if 1//
			static /*const*/ void (*ggg[PLAYERS8])(OBJ *sss) =
			{
				/*REIMU_A*/ 	shot_regist_re_yudou,			/* �얲 A(�U���ԎD) */
				/*REIMU_B*/ 	shot_regist_option_glow,		/* [�����I�v�V�����̏ꍇ]�얲 B(�j) �� ���~���A �� �`���m */
				/*MARISA_A*/	shot_regist_re_yudou,			/* ������ A(�U���X�e) */
				/*MARISA_B*/	shot_regist_ma_lazer,			/* ������ B(���܂��ꃌ�[�U�[) */
				/*REMILIA*/ 	shot_regist_option_glow,		/* �얲 B(�j) �� ���~���A �� �`���m */
				/*YUYUKO*/		shot_regist_yuyuko, 			/* �H�X�q(��) */
				/*CIRNO_A*/ 	shot_regist_option_glow,		/* [�����I�v�V�����̏ꍇ]�얲 B(�j) �� ���~���A �� �`���m */
				/*CIRNO_Q*/ 	shot_regist_option_glow,		/* [�����I�v�V�����̏ꍇ]�얲 B(�j) �� ���~���A �� �`���m */
			};
			(ggg[(cg_game_select_player)])(src);
			#endif
		}
	}
	player_control_option(src);
}


/*---------------------------------------------------------
	���@�̃V���b�g�{�^���������ςȂ��Ŏ����V���b�g�����ꍇ
---------------------------------------------------------*/

global void register_main_shot(OBJ *s1)
{
	if (
		(MARISA_B==(cg_game_select_player)) &&	/* ������B(����) */
		(0<cg.bomber_time)				/* �}�X�^�[�X�p�[�N�� */
	)
	{
		;/* �}�X�^�[�X�p�[�N���̓V���b�g�����ĂȂ��炵�� */
	}
	else
	{
		static int weapon_interval = (0);
		/*cg.*/weapon_interval--;/*fps_fa_ctor*/
		if (/*cg.*/weapon_interval <= 0)
		{
		//	/*cg.*/weapon_interval = jiki_auto_shot_interval_table[(weapon_List<<3)+(cg_ga me_select_player)];
		//	/*cg.*/weapon_interval = jiki_auto_shot_interval_table[(cg.jiki_weapon_level_offset)];
			/*cg.*/weapon_interval = current.shot_status[SHOT_STATUS_10_JIKI_SHOT_INTERVAL_L1+(cg.jiki_weapon_level)];
			voice_play_menu(VOICE00_SHOT, TRACK00_JIKI_SHOT);
			static /*const*/ void (*bbb[(WEAPON_L_MAX)*(PLAYERS8)])(OBJ *sss) =
			{	/* �U���ޏ�(A)	 �j�ޏ�(B)		 �~�T�}��(A)	 ���C�}��(B)  */
				/*REIMU_A*/ 	/*REIMU_B*/ 	/*MARISA_A*/	/*MARISA_B*/	/*REMILIA*/ 	/*YUYUKO*/		/*CIRNO_A*/ 	/*CIRNO_Q*/
				regist_n1_way,	regist_n1_way,	regist_n1_way,	regist_n1_way,	regist_n1_way,	regist_0_way,	regist_n2_way,	regist_n2_way,	/* WEAPON_L0:(P000-P008) */
				regist_n2_way,	regist_n1_way,	regist_n2_way,	regist_n2_way,	regist_n1_way,	regist_0_way,	regist_n2_way,	regist_n2_way,	/* WEAPON_L1:(P008-P015) */
				regist_n3_way,	regist_n2_way,	regist_n2_way,	regist_n3_way,	regist_n2_way,	regist_0_way,	regist_n2_way,	regist_n3_way,	/* WEAPON_L2:(P016-P031) */
				regist_w4_way,	regist_n2_way,	regist_n3_way,	regist_n4_way,	regist_n2_way,	regist_0_way,	regist_n3_way,	regist_n3_way,	/* WEAPON_L3:(P032-P063) */
				regist_w5_way,	regist_n2_way,	regist_n3_way,	regist_n4_way,	regist_n2_way,	regist_0_way,	regist_n3_way,	regist_n3_way,	/* WEAPON_L4:(P064-P127) */
				regist_w6_way,	regist_n3_way,	regist_n3_way,	regist_n5_way,	regist_n2_way,	regist_0_way,	regist_n3_way,	regist_n3_way,	/* WEAPON_L5:(P128) 	 */
			};
		//	(bbb[(cg.jiki_weapon_level_offset)])(s1);
			(bbb[(cg.jiki_weapon_level<<3)+cg_game_select_player])(s1);
		}
	}
}

//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�e�v���C���[(�얲 �� ������ �� ���~���A �� �`���m �� �H�X�q)
	REIMU(A/B) MARISA(A/B/C) REMILIA CIRNO YUYUKO
	-------------------------------------------------------
---------------------------------------------------------*/

//#include "jiki_local.h"

/*---------------------------------------------------------
	player_bomber.h
---------------------------------------------------------*/

//typedef struct
//{
//	int speed256;
//	int state;
//	int time_out;			/* �ݒ莞�ԓ��Ɍ������Ȃ��ꍇ�͂����܂� */
//	OBJ *target_obj;	/* int target_id */
//} PL_HOMING_DATA; 		/* == PL_HLASER_DATA */

#define YUUDOU_BOMBER_speed256					user_data01
#define YUUDOU_BOMBER_reimu_musou_id			user_data03
//efine PL_HOMING_time_out						YUUDOU_BOMBER_reimu_musou_id		/* �ݒ莞�ԓ��Ɍ������Ȃ��ꍇ�͂����܂� */
//efine PL_HOMING_KODOMO_DATA_color256			user_data04

//	int angleCCW1024;
//	int nr; 				/* �E����(�z�[�~���O), nr==number */


//typedef struct
//{
#define PL_HOMING_KODOMO_DATA_check_x256		user_data00
#define PL_HOMING_KODOMO_DATA_check_y256		user_data01
/*???*/#define PL_HOMING_KODOMO_DATA_time_out	user_data03 		/* �ݒ莞�ԓ��Ɍ������Ȃ��ꍇ�͂����܂� */
//#define PL_HOMING_KODOMO_DATA_color256		user_data04
//	OBJ *target_obj;	/* int target_id */
//} PL_HOMING_KODOMO_DATA;	/* == PL_HLASER_DATA */

//typedef struct
//{
#define PL_KEKKAI_DATA_angleCCW1024 			user_data00
#define PL_KEKKAI_DATA_add_r1024				user_data01
#define PL_KEKKAI_DATA_radius					user_data03 		/* ���a(radius) */
//} PL_KEKKAI_DATA;



/*
collision_check �����蔻�� -------------------------------------------------------------------------
*/

/*---------------------------------------------------------
	�v���C���[�A�������{���B��ʊO�͂ݏo���`�F�b�N�B�����蔻��`�F�b�N�B
---------------------------------------------------------*/
static OBJ_CALL_FUNC(player_bomber_out_collision_check)
{
	#if 1/*Gu(���S���W)*/
	if ((src->center.x256 < t256(0))||(src->center.x256 > t256(GAME_WIDTH))||
		(src->center.y256 < t256(0))||(src->center.y256 > t256(GAME_HEIGHT)))
	#endif
	{
		src->jyumyou = JYUMYOU_NASI;/* ��ʊO�ɏo���炨���܂� */
	}
	else
	{
		player_weapon_collision_check(src, PLAYER_WEAPON_TYPE_01_BOMB_PLAYER);/* �{���œG��|�������蔻�� */
	}
}



/*
move �V���b�g/�{���ړ� -------------------------------------------------------------------------
*/

/*---------------------------------------------------------
	�������{���̈ړ�(�얲A)
	���v��� �� �����v��� �� �X�N�G�A���E(�얲B) ���p
	�������{���̈ړ�(�H�X�q)
---------------------------------------------------------*/

static OBJ_CALL_FUNC(reimu_yuyuko_move_rotate_kekkai) /* �얲 */
{
	src->PL_KEKKAI_DATA_angleCCW1024 += (src->PL_KEKKAI_DATA_add_r1024);	/*fps_factor*/	/*...*/
	mask1024(src->PL_KEKKAI_DATA_angleCCW1024);
	#if 1
	/* �`��p�p�x(����0�x�ō����(�����v���)) */
	src->rotationCCW1024 = (src->PL_KEKKAI_DATA_angleCCW1024);
	#endif
//
	int genten_cx256;
	int genten_cy256;
	{
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+((YUYUKO==(cg_game_select_player))?(FIX_OBJ_02_BOMBER_HONTAI):(FIX_OBJ_00_PLAYER))];
		genten_cx256 = zzz_player->center.x256;
		genten_cy256 = zzz_player->center.y256;
	}
	#if (0)//
	src->center.x256 = genten_cx256 + ((si n1024((src->PL_KEKKAI_DATA_angleCCW1024))*(src->PL_KEKKAI_DATA_radius)) );/*fps_factor*/ 	/* CCW�̏ꍇ */
	src->center.x256 = genten_cx256 + ((si n1024((src->PL_KEKKAI_DATA_angleCCW1024))*(src->PL_KEKKAI_DATA_radius)) );/* CCW�̏ꍇ */
	src->center.y256 = genten_cy256 + ((co s1024((src->PL_KEKKAI_DATA_angleCCW1024))*(src->PL_KEKKAI_DATA_radius)) );
	src->center.y256 = genten_cy256 + ((co s1024((src->PL_KEKKAI_DATA_angleCCW1024))*(src->PL_KEKKAI_DATA_radius)) );/*fps_factor*/
	#else
	{
		int sin_value_t256; 	//	sin_value_t256 = 0;
		int cos_value_t256; 	//	cos_value_t256 = 0;
		int256_sincos1024( (src->PL_KEKKAI_DATA_angleCCW1024), &sin_value_t256, &cos_value_t256);
		src->center.x256 = genten_cx256 + ((sin_value_t256*(src->PL_KEKKAI_DATA_radius)) );/*fps_factor*/
		src->center.y256 = genten_cy256 + ((cos_value_t256*(src->PL_KEKKAI_DATA_radius)) );/*fps_factor*/
	}
	#endif

	/* �X�N�G�A���E(�얲B) */
//	if (REIMU_A != (cg_game_select_player))
	if (REIMU_B == (cg_game_select_player)) /* �X�N�G�A���E(�얲B) */
	{
		if (0 < src->PL_KEKKAI_DATA_add_r1024)
		{
			if (genten_cx256 < src->center.x256)
					{	src->center.x256 = genten_cx256 + ((src->PL_KEKKAI_DATA_radius)<<8);	}
			else	{	src->center.x256 = genten_cx256 - ((src->PL_KEKKAI_DATA_radius)<<8);	}
		}
		else
		{
			if (genten_cy256 < src->center.y256)
					{	src->center.y256 = genten_cy256 + ((src->PL_KEKKAI_DATA_radius)<<8);	}
			else	{	src->center.y256 = genten_cy256 - ((src->PL_KEKKAI_DATA_radius)<<8);	}
		}
	}
//	if (REIMU==(cg_game_select_player))
	{
		if (src->PL_KEKKAI_DATA_radius<255)
		{
			src->PL_KEKKAI_DATA_radius += (ra_nd()&1);
		}	/* �������L����悤�ɂ��� */
		else/* ��]���L�������炨���܂� */
		{
			src->jyumyou = JYUMYOU_NASI;
			cg.state_flag &= (~JIKI_FLAG_0x0020_BOMBER_SYUU_SYUU);	/* �{���ɂ�鎩�����W�͏I��� */
		}
	//	src->alpha				= ((int)(255-src->PL_KEKKAI_DATA_radius))&(256-1);	/* ���񂾂������悤�ɂ��� */
	//	src->color32			= ((((255-src->PL_KEKKAI_DATA_radius))&(0xff))<<24)|0x00ffffff; /* ���񂾂������悤�ɂ��� */
		src->color32			= ((((255-src->PL_KEKKAI_DATA_radius))&(0xff))<<24)|0x00ffffff; /* ���񂾂������悤�ɂ��� */
	}
//	else //if (YUYUKO==(cg_game_select_player))
//	{
//		if (pd_bomber_time<=0)	{	src->jyumyou = JYUMYOU_NASI;}/* ���Ԑ����ł����܂� */
//	}
	if ((ra_nd()&0xff) < cg.bomber_time)
	{
		if (0==((src->PL_KEKKAI_DATA_radius)&7))
		{
			//player_weapon_collision_check_kekkai(src, src->PL_KEKKAI_DATA_radius);
			bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
		}
	}
	player_weapon_collision_check(src, PLAYER_WEAPON_TYPE_01_BOMB_PLAYER);/* �{���œG��|�������蔻�� */
}


/*---------------------------------------------------------
	�v���C���[�A�������{���̈ړ�(������B �}�X�^�[�X�p�[�N��p)
---------------------------------------------------------*/
#define MASKER_SPARK_LENGTH144 (144)
static OBJ_CALL_FUNC(marisa_move_master_spark)
{
	OBJ *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
//	src->center.x256 += src->math_vector.x256;	/*fps_factor*/
//	src->center.y256 += src->math_vector.y256;	/*fps_factor*/
	src->center.x256 = zzz_player->center.x256; 			/*fps_factor*/
	src->center.y256 = zzz_player->center.y256-t256(MASKER_SPARK_LENGTH144);	/*fps_factor*/
	{
		static const u16 maspa_kaiten[4] =
		{
			(18),
			(1024-24),
			(10),
			(1024-6),
			/* ��]���x */
		};
		/* �\���p�x(����0�x�ō����(�����v���)) */
		src->tmp_angleCCW1024	+= maspa_kaiten[((src->obj_type_set)& (0x03))];
	//	mask1024((src->tmp_angleCCW1024));
		{
			int sin_value_t256; 		//	sin_value_t256 = 0;
			int cos_value_t256; 		//	cos_value_t256 = 0;
			int256_sincos1024( ((((src->tmp_angleCCW1024))/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
			src->rotationCCW1024 = (( ((sin_value_t256)))>>2);
		}
	}
	/* ���Ԃł����܂� */
	if (0 >= cg.bomber_time)	{ src->jyumyou = JYUMYOU_NASI;}/* ���Ԑ����ł����܂� */
	else
	if (0x40 > cg.bomber_time)	/* 0x3f ... 0x01 */
	{
		src->color32			= ((((cg.bomber_time))/*&(0x3f)*/)<<(24+2))|0x00ffffff; /* ���񂾂������悤�ɂ��� */
		/* (���̒l�͈̔͂Ȃ�)���̓}�X�N�K�v�Ȃ��B */
	}
	player_weapon_collision_check(src, PLAYER_WEAPON_TYPE_01_BOMB_PLAYER);/* �{���œG��|�������蔻�� */
//	player_bomber_out_collision_check(src);/* ��ʊO�ɏo���炨���܂� & �{���œG��|�������蔻�� */
}

/*---------------------------------------------------------
	�v���C���[�A�������{���̈ړ�(�������A�`���m�A���H�X�q(�Ⴄ�����ɂ�����) )
---------------------------------------------------------*/

static OBJ_CALL_FUNC(marisa_yuyuko_move_levarie_gyastry_dream)
{
	src->center.x256 += src->math_vector.x256;	/*fps_factor*/
	src->center.y256 += src->math_vector.y256;	/*fps_factor*/
	#if 1
	/* YUYUKO MARISA_A �ȊO */
	if (
	//	(CIRNO_A==(cg_game_select_player)) || (CIRNO_Q==(cg_game_select_player)) ||
		((CIRNO_A-1)<(cg_game_select_player)) //||
	//	(MARISA_B==(cg_game_select_player))/* �}�X�p(���ǂ�) */
	)
	#endif
	{	/*�`���m�p(�b��I)*/	/*1.5*(d->speed);*/ 	/*fps_factor*/
		src->center.y256 -= (abs((src->math_vector.x256+(src->math_vector.x256>>1) )));
		src->center.y256 -= (abs((src->math_vector.y256+(src->math_vector.y256>>1) )));
		src->center.y256 -= t256(1.0);
	}
	{

		/* �\���p�x(����0�x�ō����(�����v���)) */
	//	src->rotationCCW1024	+= aaa_sss[(cg_game_select_player)];
		src->rotationCCW1024	+= current.shot_status[SHOT_STATUS_20_BOMBER_KAITEN_SOKUDO];
		mask1024((src->rotationCCW1024));
		if (t256(4.0) > src->m_zoom.x256)
		{
		//	src->m_zoom.y256 += aaa_sss[(cg_game_select_player)+(PLAYERS8)];
		//	src->m_zoom.x256 += aaa_sss[(cg_game_select_player)+(PLAYERS8)];
			src->m_zoom.x256 += current.shot_status[SHOT_STATUS_21_BOMBER_KAKUDAI_SOKUDO];
			src->m_zoom.y256 = src->m_zoom.x256;
		}
	}
	player_bomber_out_collision_check(src);
}

/*---------------------------------------------------------
	�v���C���[�A�������{���̈ړ�(���~���A)
---------------------------------------------------------*/

static OBJ_CALL_FUNC(remilia_move_burn_fire)
{
	#if 1
	// ����
	// x1.10
//	src->math_vector.x256 = ((src->math_vector.x256 * t256(1.1))>>8);//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
//	src->math_vector.y256 = ((src->math_vector.x256 * t256(1.1))>>8);//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
	// x1.125
	src->math_vector.x256 += ((src->math_vector.x256)>>3);//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
	src->math_vector.y256 += ((src->math_vector.y256)>>3);//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
	// x1.0625
//	src->math_vector.x256 += ((src->math_vector.x256)>>4);//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
//	src->math_vector.y256 += ((src->math_vector.y256)>>4);//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
	/* �`�悪�d������̂ŉ������\�A�኱����( x1.0625�� x1.125)���� */
	//
	#endif
	src->center.x256 += src->math_vector.x256;	//	src->math_vector.x256;//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
	src->center.y256 += src->math_vector.y256;	//	src->math_vector.y256;//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
	//
	player_bomber_out_collision_check(src);
}

/*---------------------------------------------------------
	�U���e�̗U���ړ��T�u���[�`��
---------------------------------------------------------*/

static OBJ_CALL_FUNC(yuudou_idou)
{
	OBJ *target;	/* �ڕW */
	target = src->target_obj;
	/* �ڕW����ʓ��Ȃ�ΖڕW�Ɍ����� */
	if (
		(target->center.x256 > t256(0)) &&
		(target->center.x256 < (t256(GAME_WIDTH)) ) &&
		(target->center.y256 > t256(0)) &&
		(target->center.y256 < (t256(GAME_HEIGHT)) ))	/*Gu(���S���W)*/
	{
		#if (1)/*�U��(000)*/
		{int ta1024;
		#if 1
		/* CCW�̏ꍇ */
			REG_00_SRC_X	= (target->center.x256);		/*(�_����)*/
			REG_01_SRC_Y	= (target->center.y256);		/*(�_����)*/
			REG_02_DEST_X	= (src->center.x256);		/*(�_����)*/
			REG_03_DEST_Y	= (src->center.y256);		/*(�_����)*/
			tmp_angleCCW65536_src_nerai();
			ta1024 = HATSUDAN_03_angle65536;
			ta1024 >>= (6);
			#if 1
			/* �x��U�� */
			/* KETM�͍̂l���������G�Ȃ̂Ŕj���A0�x����̍����ōl��180�x�����ɕ��򂷂�Γ���(1����360�x�����Ȃ��̂�) */
		//	const int sabun_aaa = (src->tmp_angleCCW1024-src->rotationCCW1024);
			const int sabun_aaa1024 = (ta1024-src->rotationCCW1024);
			if (0 > sabun_aaa1024/*(src->tmp_angleCCW1024-src->rotationCCW1024)*/ )/* 0���W�X�^+���ʍœK�� */
			{	/* �ڕW�p�x < ����p�x */
				if ((1024/2) > sabun_aaa1024/*(src->tmp_angleCCW1024-src->rotationCCW1024)*/)/* ���ʍœK�� */
						{	src->rotationCCW1024 -= 10; }	/* 180�x�ȓ��Ȃ�߂��ق������� */
				#if 0	/* �ȗ����Ė��Ȃ��H */
				else	{	src->rotationCCW1024 += 10; }	/* 180�x�ȏ�Ȃ甽�Ύ���̂ق����߂� */
				#endif
			}
			else	/* �ڕW�p�x > ����p�x */
			{
				if ((1024/2) > -(sabun_aaa1024)/*(src->rotationCCW1024-src->tmp_angleCCW1024)*/)
						{	src->rotationCCW1024 += 10; }	/* 180�x�ȓ��Ȃ�߂��ق������� */
				#if 0	/* �ȗ����Ė��Ȃ��H */
				else	{	src->rotationCCW1024 -= 10; }	/* 180�x�ȏ�Ȃ甽�Ύ���̂ق����߂� */
				#endif
			}
			#else
			/* ���ڗU�� */
			src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/ = ta1024;
			#endif
		#endif
		}
		mask1024(src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/);
		#endif/*�U��(000)*/
	}
	else	/* �ڕW����ʊO�Ȃ�ΐV�����ڕW��T���B */
	{
		/* Ziel wurde anderweitig vernichtet, neues Ziel suchen */
		src->target_obj = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];		/* ��ʊO�ɓ�����ꂽ */
	}
}

/* ------------------------------------------------------------------------ */

/*---------------------------------------------------------
	�v���C���[�A�U���e�̈ړ�(�e)����
---------------------------------------------------------*/

enum
{
	HOMING_096_TIME_OUT = 96,
	HOMING_128_TIME_OUT = 128,
};
static OBJ_CALL_FUNC(marisa_move_parrent_hlaser)
{
	/* ���̗U���{�����A���ɓ|�������H */
	if (&obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER] == src->target_obj) /* ������Ȃ�(���̗U���{�����|���Ă��܂����ꍇ) */
	{
		/* �^�[�Q�b�g�����A�G��T���B */
		src->target_obj = obj_teki_search();
	}
	else	/* �܂��^�[�Q�b�g�������Ă� */
	{
		if (HOMING_096_TIME_OUT < src->jyumyou)
		{
			/* ��莞�Ԍ��� */
			src->YUUDOU_BOMBER_speed256 -= t256(0.5);/*����*/
			// �~�܂�ƍ���
			src->YUUDOU_BOMBER_speed256 = psp_max((src->YUUDOU_BOMBER_speed256), t256(0.75));	/* �ŏ��l */
		}
		else
		{
			/* ��莞�Ԃ𒴂�������� */
			src->YUUDOU_BOMBER_speed256 += t256(0.5);/*����*/
			// ���߂���̂�����
			src->YUUDOU_BOMBER_speed256 = psp_min((src->YUUDOU_BOMBER_speed256), t256(16.0));	/* �ő�l */
		}
		//
		yuudou_idou(src);
	}
	#if 1
	/* (�����K�v�Ȃ�����)�O�̈׃}�X�N */
	mask1024(src->rotationCCW1024);/*src->YUUDOU_BOMBER_angleCCW1024*/
	#endif
	#if 1
	/* �\���p */
//	src->rotationCCW1024	= src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/;
	#endif
	/*src->YUUDOU_BOMBER_angleCCW1024*/
	#if (0)//
	src->center.x256 += ((si n1024((src->rotationCCW1024))*(src->YUUDOU_BOMBER_speed256))>>8);/*fps_factor*/		/* CCW�̏ꍇ */
	src->center.y256 += ((co s1024((src->rotationCCW1024))*(src->YUUDOU_BOMBER_speed256))>>8);/*fps_factor*/
	#else
	{
		int sin_value_t256; 	//	sin_value_t256 = 0;
		int cos_value_t256; 	//	cos_value_t256 = 0;
		int256_sincos1024( (src->rotationCCW1024), &sin_value_t256, &cos_value_t256);
		src->center.x256 += ((sin_value_t256*(src->YUUDOU_BOMBER_speed256))>>8);/*fps_factor*/
		src->center.y256 += ((cos_value_t256*(src->YUUDOU_BOMBER_speed256))>>8);/*fps_factor*/
	}
	#endif
	player_weapon_collision_check(src, PLAYER_WEAPON_TYPE_00_SHOT);
//
//	src->jyumyou--;/*fps_factor*/
//	if (0 > src->jyumyou ) /* ���Ԑ؂�H */
//	{
//		src->jyumyou = JYUMYOU_NASI;		/* ���Ԑ؂�Ȃ̂ł����܂� */
//	}
}

/*---------------------------------------------------------
	�얲��p �ᑬ�{��
	�아�얲
---------------------------------------------------------*/

static OBJ_CALL_FUNC(move_reimu_musou_fuuin)
{
	/* ���̗U���{�����A���ɓ|�������H */
	if (&obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER] == src->target_obj) /* ������Ȃ�(���̗U���{�����|���Ă��܂����ꍇ) */
	{
		/* �^�[�Q�b�g�����A�G��T���B */
		src->target_obj = obj_teki_search();
	}
	else	/* �܂��^�[�Q�b�g�������Ă� */
	{
		if (t256(3.0) > src->YUUDOU_BOMBER_speed256)
		{
			src->YUUDOU_BOMBER_speed256 += /*(int)*/t256(0.5);/*����*/
		}
		//
		yuudou_idou(src);
	}
// �{���͍ő� 7 ��(8)
//	if ((cg.bomber_time&0x0f)==(src->YUUDOU_BOMBER_reimu_musou_id&0x0f))
	if ((cg.bomber_time&0x07)==(src->YUUDOU_BOMBER_reimu_musou_id&0x07))
	{
		/*src->YUUDOU_BOMBER_angleCCW1024*/
		#if (0)//
		src->math_vector.x256 = ((si n1024((src->rotationCCW1024))*(src->YUUDOU_BOMBER_speed256))>>8);/*fps_factor*/		/* CCW�̏ꍇ */
		src->math_vector.y256 = ((co s1024((src->rotationCCW1024))*(src->YUUDOU_BOMBER_speed256))>>8);/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (src->rotationCCW1024), &sin_value_t256, &cos_value_t256);
			src->math_vector.x256 = ((sin_value_t256*(src->YUUDOU_BOMBER_speed256))>>8);/*fps_factor*/
			src->math_vector.y256 = ((cos_value_t256*(src->YUUDOU_BOMBER_speed256))>>8);/*fps_factor*/
		}
		#endif
	}
		src->center.x256 += (src->math_vector.x256);/*fps_factor*/
		src->center.y256 += (src->math_vector.y256);/*fps_factor*/
	player_weapon_collision_check(src, PLAYER_WEAPON_TYPE_01_BOMB_PLAYER/*PLAYER_WEAPON_TYPE_00_SHOT*/);
//
	if (0 >= cg.bomber_time)	/* ���Ԑ؂�̏ꍇ */
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
		src->kougeki_ti--;/* ���Ԑ؂�̏ꍇ�A�{���o�[(���D)�̌���(�З�)�����炷 */
	}
//
	if (0 > src->kougeki_ti)	{	src->jyumyou = JYUMYOU_NASI;	}	/* �{���o�[(���D)�̌���(�З�)�؂�ł����܂� */
}
/*---------------------------------------------------------
	�v���C���[�A�U���e�̈ړ�(�q��)
---------------------------------------------------------*/

static OBJ_CALL_FUNC(marisa_move_kodomo_hlaser)
{
	#if 1
	OBJ *oya;
	oya = src->target_obj;
	if (
			(JYUMYOU_NASI > oya->jyumyou) ||			/* �e�������ς݂Ȃ�Ύq������ */
		//	((OBJ_Z01_JIKI_GET_ITEM|OBJ_Z80_SHOT_SPECIAL) != oya->obj_type_set) 		/* (OBJ_Z01_JIKI_GET_ITEM|OBJ_Z80_SHOT_SPECIAL)�ȊO�͐e�łȂ��̂ŏ��� */
			(0==(OBJ_Z80_SHOT_SPECIAL & oya->obj_type_set)) /* (OBJ_Z01_JIKI_GET_ITEM|OBJ_Z80_SHOT_SPECIAL)�ȊO�͐e�łȂ��̂ŏ��� */
		)
	{
		src->jyumyou = JYUMYOU_NASI;/*�����܂�*/
	}
//
	#if 1
	/* �\���p */
	src->rotationCCW1024	= oya->rotationCCW1024;/*src->PL_HOMING_KODOMO_DATA_angleCCW1024*/
	#endif
	src->center.x256 = oya->center.x256;
	src->center.y256 = oya->center.y256;
//	src->PL_HOMING_KODOMO_DATA_color256 = ((oya->PL_HOMING_KODOMO_DATA_color256*t256(0.96))>>8)&0xff;
//	src->PL_HOMING_KODOMO_DATA_color256 = ((oya->PL_HOMING_KODOMO_DATA_color256*t256(0.90))>>8)&0xff;
//	src->PL_HOMING_KODOMO_DATA_color256 = ((oya->PL_HOMING_KODOMO_DATA_color256 - 0x10)&0xff);
//	src->color32 = (src->PL_HOMING_KODOMO_DATA_color256<<24)|(0x00ffffff);
	#if (1)/*???*/
	/* ���t���[�������Ȃ��ꍇ�́A������������ */
	if ((src->PL_HOMING_KODOMO_DATA_check_x256 != src->center.x256) ||
		(src->PL_HOMING_KODOMO_DATA_check_y256 != src->center.y256))
	{
		src->PL_HOMING_KODOMO_DATA_time_out = 8;/* �������玩���������Ȃ� */
	}
	src->PL_HOMING_KODOMO_DATA_time_out--;
	if (0 > src->PL_HOMING_KODOMO_DATA_time_out)
	{
		src->jyumyou = JYUMYOU_NASI;/*�����܂�*/
	}
	#endif
	src->PL_HOMING_KODOMO_DATA_check_x256 = src->center.x256;
	src->PL_HOMING_KODOMO_DATA_check_y256 = src->center.y256;
	#endif
}


/*
add1-------------------------------------------------------------------------
*/
static void set_bomb_str_STD(OBJ *h)
{
	h->m_Hit256R	= ((current.shot_status[SHOT_STATUS_1E_STANDARD_BOMB_ATARI_HANKEI])<<8);
	h->kougeki_ti	= current.shot_status[SHOT_STATUS_16_STANDARD_BOMB_STRENGTH];
	/* �U���l: ==�{���o�[�̈З́B */
}
static void set_bomb_str_LOW(OBJ *h)
{
	h->m_Hit256R	= ((current.shot_status[SHOT_STATUS_1F_LOWER_BOMB_ATARI_HANKEI])<<8);
	h->kougeki_ti	= current.shot_status[SHOT_STATUS_17_LOWER_BOMB_STRENGTH];
	/* �U���l: ==�{���o�[�̈З́B */
}
/* (�H�X�q�ᑬ)�\�����{���̉��̕��� */

static OBJ_CALL_FUNC(yuyuko_add_meifu)
{
		/* (24+(1))  �A�����ł� cg.bomber_time �́A�ő�0x7f�Ȃ̂ŁB */
		src->color32		= (cg.bomber_time<<(24+(1)))|0x00ffffff;
		src->m_zoom.x256	+= (10);
		src->m_zoom.y256	+= (10);
//	int j;
//	for (j=0; j<(1); j++)
	{
		OBJ *h;
		h = obj_regist_teki();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{	set_bomb_str_LOW(h);
			h->obj_type_set 			= (JIKI_BOMBER_01|OBJ_Z80_SHOT_SPECIAL);/* �{�X�ɗL�� */
			h->callback_mover	= remilia_move_burn_fire;
		//	h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x64);		/*	h->alpha			= 0x64 100;*/
			h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x64);		/*	h->alpha			= 0x64 100;*/
			#if 1
			/* �`��p�p�x(����0�x�ō����(�����v���)) */
			h->rotationCCW1024		= (0);
			#endif
			{
		u32 my_rand;
			my_rand = ra_nd();
			{
				h->center.x256			= (src->center.x256) - t256(16) + (((my_rand<<8)&0x1fff));//+ ((0==j)?(-t256(32)):t256(32));
				h->center.y256			= (src->center.y256) - t256(16) + (((my_rand   )&0x1fff));//(u16)((my_rand>>16)) + t256((GAME_HEIGHT-256)/2);
			}
		int rnd_spd256		= (my_rand & 0xff/*angCCW512*/) + t256(1.5);
		int ang_aaa_1024;
			ang_aaa_1024 = (my_rand);
			mask1024(ang_aaa_1024);
			//------------------
			HATSUDAN_01_speed256	= ((rnd_spd256));/*(t256�`��)*/
			HATSUDAN_03_angle65536	= deg1024to65536(ang_aaa_1024);
			sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
			h->math_vector.x256 = REG_03_DEST_Y;//sin_value_t256	// �c
			h->math_vector.y256 = REG_02_DEST_X;//cos_value_t256	// ��
			}
		}
	}
}

/* �\���V���b�g�{���̉��̕��� */

static OBJ_CALL_FUNC(remilia_add_burn_shot)
{
	{
		OBJ *h;
		h = obj_regist_teki();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{	set_bomb_str_STD(h);
			//
//			h->obj_type_set 			= (/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_04|OBJ_Z80_SHOT_SPECIAL)/*�{�X�ɗL��*/;
			h->obj_type_set 			= (/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_01|OBJ_Z80_SHOT_SPECIAL);/*�{�X�ɗL��*/
			h->callback_mover	= remilia_move_burn_fire;
//			h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x64);		/*	h->alpha			= 0x64 100;*/
			h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	h->alpha			= 0x64 100;*/
			#if 1/*Gu(���S���W)*/
			h->center.x256			= src->center.x256;
			h->center.y256			= src->center.y256+t256(15);
			#endif
			{
			#if 1
			/* �`��p�p�x(����0�x�ō����(�����v���)) */
//			h->rotationCCW1024		= ang_aaa_1024;
			h->rotationCCW1024		= (0);
			#endif
			h->math_vector.x256 	= (src->math_vector.x256);/*fps_factor*/ /* CCW�̏ꍇ */
			h->math_vector.y256 	= (src->math_vector.y256);/*fps_factor*/
			u32 rnd_spd 		= (ra_nd() ) ;
			if (rnd_spd&0x03)	{	h->math_vector.x256 += h->math_vector.x256; }/* [�ʒu2�{(��/��)-2�Ȃ�-4, 2�Ȃ�4 ] */
			if (rnd_spd&0x01)	{	h->math_vector.x256 = - h->math_vector.x256;	}
			if (rnd_spd&0x02)	{	h->math_vector.y256 = - h->math_vector.y256;	}
			}
		}
	}
}

/*---------------------------------------------------------
	�얲 A ��p �ᑬ�{���ǉ�
	�아�얲
	�{��:���z����
	�z�[�~���O���e��7���o���B�G�����Ȃ����͂��΂炭���̏�ɒ�؁B
	�З͂͗����ɂ��ō����A���G���Ԃ������B���e�̋O�ՁA�y��ɒe�����B
---------------------------------------------------------*/

static OBJ_CALL_FUNC(reimu_add_reifu_musofuuin)
{
	{
		static int musou_id;
		musou_id++;
		musou_id &= 0x07;
		{
			OBJ *h;
			h						= obj_regist_teki();
			if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
			{	set_bomb_str_LOW(h);
	//			h->obj_type_set 			= (OBJ_Z01_JIKI_GET_ITEM|OBJ_Z80_SHOT_SPECIAL)/*�{�X�ɗL��*/;
				h->obj_type_set 			= (/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_01|OBJ_Z80_SHOT_SPECIAL);/*�{�X�ɗL��*/
				#if 1/*Gu(���S���W)*/
			//	h->center.x256			= src->center.x256 - t256(5);
			//	h->center.y256			= src->center.y256 + t256(15);
				h->center.x256			= src->center.x256;
				h->center.y256			= src->center.y256;
				#endif
	//			if (0==i)
				{
					h->callback_mover	= move_reimu_musou_fuuin;
	//				h->rotationCCW1024	= cv1024r(0);
	//				h->rotationCCW1024	= (0/*j&1*/)?(cv1024r(	0+10)):(cv1024r(360-10));
	//				h->rotationCCW1024	= (i<<6);
					/* 180 ... 0 (1024/128==8) */
					h->rotationCCW1024	= ( (((int)cg.bomber_time)<<3)+(512));/* �ˏo������� */
					#if 0
					/* (�����K�v�Ȃ�����)�O�̈׃}�X�N */
					mask1024(src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/);
					#endif
					h->YUUDOU_BOMBER_speed256				= t256(1.2);
					h->jyumyou								= byou60(20);	/* 20�b �e�L�g�[ */ 	/* (100) (200)*/
					h->YUUDOU_BOMBER_reimu_musou_id 		= (musou_id);	/*(ii)(200)*/
					h->target_obj							= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
				/* (h->target_obj == obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER])==������Ȃ� */
				}
			}
		}
	}
}

/*
add2-------------------------------------------------------------------------
*/
/*---------------------------------------------------------
	�v���C���[�U���e����
---------------------------------------------------------*/

static OBJ_CALL_FUNC(marisa_add_teisoku_yuudoudan)
{
	int j;	j=(ra_nd()&(2-1));//for (j=0;j<2;j++)
	{
		OBJ *h_old; h_old = NULL;
		OBJ *h; 	h = NULL;
		//#define hlaser_NUM_OF_ENEMIES (24)
		//#define hlaser_NUM_OF_ENEMIES (12)		/* �����ɂ��Ă݂� */
		#define hlaser_NUM_OF_ENEMIES (8)			/* 8���ʂŏ\������(?) */
		unsigned int i;
		unsigned int tmp_color32;
		tmp_color32 = 0xffffffff;
		for (i=0; i<hlaser_NUM_OF_ENEMIES; i++)
		#undef hlaser_NUM_OF_ENEMIES
		{	h_old = h;
			h					= obj_regist_teki();
		//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* �����o�^ */
			if (NULL==h)return;/* �o�^�ł����ꍇ�̂� */
			{
/* ���[��... */
				set_bomb_str_LOW(h);
//				h->obj_type_set 			= (OBJ_Z01_JIKI_GET_ITEM|OBJ_Z80_SHOT_SPECIAL)/*�{�X�ɗL��*/;
				h->obj_type_set 			= (/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_01|OBJ_Z80_SHOT_SPECIAL)/*�{�X�ɗL��*/;
				#if 1/*Gu(���S���W)*/
			//	h->center.x256			= src->center.x256 - t256(5);
			//	h->center.y256			= src->center.y256 + t256(15);
				h->center.x256			= src->center.x256;
				h->center.y256			= src->center.y256;
				#endif
//				h->PL_HOMING_KODOMO_DATA_color256		= (t256(1.0));	/* ���邳(�A���t�@�����l) */
				h->color32			= tmp_color32;
//				tmp_color32 -= 0x10000000;
//				tmp_color32 -= 0x20000000;
				tmp_color32 -= 0x20202020;
				if (0==i)
				{
					h->callback_mover	= marisa_move_parrent_hlaser;
//					h->rotationCCW1024						= cv1024r(0);
					h->rotationCCW1024						= (j&1)?(cv1024r(  0+10)):(cv1024r(360-10));
					h->YUUDOU_BOMBER_speed256				= t256(4.0);//t256(12.0);
					h->jyumyou								= (HOMING_128_TIME_OUT);	/*100(200)*/
					h->target_obj							= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
				/* (h->target_obj==obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER])==������Ȃ� */
				}
				else
				{
					h->callback_mover	= marisa_move_kodomo_hlaser;/*NULL*/
					h->target_obj		= h_old;
					h->PL_HOMING_KODOMO_DATA_time_out		= 8;
				}
			}
		}
	}
}

//(r31)
//	 0	1  2  3  4	5  6  7  8	9 10 11 [�ړ���������]
//	�� �� �� �� �� �� �� �� �� �� �� ��
//	 p	0  1  2  3	4  5  6  7	8  9 10
// ���ʓI�� �S���d�Ȃ�o�O�B

//(r32)
//	11	10	9  8  7  6	5  4  3  2	1  0 [�ړ���������]
//	��	�� �� �� �� �� �� �� �� �� �� ��
//	10	 9	8  7  6  5	4  3  2  1	0  p

/* tuika �ǉ��n ------------------------------------------------------------------------ */

static OBJ_CALL_FUNC(bomber_hontai_off_check)
{
	if (0 >= cg.bomber_time)
	{
		src->callback_mover = NULL;/*(off!)*/
	}
/* ���Ԑ����ł����܂� */	//	if (0 >= cg.bomber_time)	{ src->jyumyou = JYUMYOU_NASI;}
}

/*---------------------------------------------------------
	(�얲)���z����̒ǉ�[�{���ǉ��{��]
	�얲 A (�ʏ�{��) / �`���m A(�ᑬ�{��) �̐e
---------------------------------------------------------*/

static OBJ_CALL_FUNC(bomber_hontai_reimu_tuika_musofuuin_shot)
{
	/* �V�i���I���ɂ͗U���e��ǉ����Ȃ� */
	if (IS_SCENE_KAIWA_MODE)	{	return; 	}
	bomber_hontai_off_check(src);
	//
	/*(r36�e�X�g�A32 �����͒ǉ����Ȃ�)*/
	if (32 > cg.bomber_time)	{	return; 	}
	//
	OBJ *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
	//
//	if (0x3f==(((int)pd_bomber_time)&0x3f))
//	if (0x0f==(((int)pd_bomber_time)&0x0f))
	if (0x0f==((src->jyumyou)&0x0f))/* �g�������^�C�~���O */
	{
		reimu_add_reifu_musofuuin(zzz_player);/* �g������(�������t?) */
	}
	#if 1/*Gu(���S���W)*/
	src->center.x256 = zzz_player->center.x256;
	src->center.y256 = zzz_player->center.y256;
	#endif
}

/*---------------------------------------------------------
	�������A�`���m���A�ᑬ�{���̒ǉ�[�{���ǉ��{��]
---------------------------------------------------------*/
static OBJ_CALL_FUNC(bomber_hontai_marisa_tuika_bomber_teisoku_yuudoudan)
{
	/* �V�i���I���ɂ͗U���e��ǉ����Ȃ� */
	if (IS_SCENE_KAIWA_MODE)	{	return; 	}
	bomber_hontai_off_check(src);
	//
	OBJ *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
	//
//	if (0==(((int)pd_bomber_time)&0x3f))
//	if (0==(((int)pd_bomber_time)&0x0f))/*(r31)*/
	if (0==((src->jyumyou)&0x07))/*(r32)�ǉ��ʂ�2�{�ɂ��Ă݂�B*/
	{
		/* �V�i���I���ɂ͗U���e��ǉ����Ȃ� */
	//	if (0==(cg.state_flag & STATE_FLAG_0x0200_IS_KAIWA_MODE))
		{
			marisa_add_teisoku_yuudoudan(zzz_player);
		}
	}
}

/*---------------------------------------------------------
	���~���A�{���̏\���V���b�g�̒ǉ�[�{���ǉ��{��]
---------------------------------------------------------*/

static OBJ_CALL_FUNC(bomber_hontai_remilia_tuika_cross_shot)
{
	/* �V�i���I���ɂ͗U���e��ǉ����Ȃ� */
	if (IS_SCENE_KAIWA_MODE)	{	return; 	}
	bomber_hontai_off_check(src);
	//
	/* �V�i���I���ɂ͗U���e��ǉ����Ȃ� */
//	if (0==(cg.state_flag & STATE_FLAG_0x0200_IS_KAIWA_MODE))
	{
		remilia_add_burn_shot(src);/* �g������(�������t?) */
	}
	#if 1/*Gu(���S���W)*/
	/* �e�ړ� */
	src->center.x256 += src->math_vector.x256;
	src->center.y256 += src->math_vector.y256;
	#endif
}

/*---------------------------------------------------------
	(�H�X�q�ᑬ)���~���A�{���̏\�����̒ǉ�[�{���ǉ��{��]
---------------------------------------------------------*/

static OBJ_CALL_FUNC(bomber_hontai_yuyuko_tuika_meifu)
{
	/* �V�i���I���ɂ͗U���e��ǉ����Ȃ� */
	if (IS_SCENE_KAIWA_MODE)	{	return; 	}
	bomber_hontai_off_check(src);
	//
	yuyuko_add_meifu(src);/* �g������(�������t?) */
	#if 1/*Gu(���S���W)*/
//	src->center.x256 = zzz_player->center.x256;
//	src->center.y256 = zzz_player->center.y256;
	#endif
}


/*----------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------*/

/*
parrent �e�n -------------------------------------------------------------------------
*/
/*---------------------------------------------------------
	�v���C���[�V�[���h�����̐e(�얲)
---------------------------------------------------------*/

/*static*/global OBJ_CALL_FUNC(reimu_yuyuko_create_bomber_kekkai_parrent)		/* �얲 */		//�V�[���h�̒ǉ�
{
	int ii;//	int i;	/* ���ی����� */
	int jj;
	ii = 0;
//	for (ii=0; ii<(8); ii++)//	for (i=0; i<360; i+=45)
	for (jj=0; jj<(1024);  )//	for (i=0; i<360; i+=45)
	{
		OBJ *h;
		h				= obj_regist_teki();
	//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		if (NULL==h)return;/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R					= TAMA_ATARI_04;
			h->kougeki_ti					= (8*1);	/*5*/
			/* �U���l: ==���D�̌��́B==�{���o�[�̈З́B */
			#if 1
			if (YUYUKO==(cg_game_select_player))
			{
				if (0 == (ii&1))// if (i%90 == 0)
				{
					h->PL_KEKKAI_DATA_add_r1024 	= ( 4); /* ���v��� */
					h->PL_KEKKAI_DATA_radius		= (16); 	/*(48)*/	//d->radius=38; 	/* �������L���锼�a�����l */
				}
				else
				{
					h->PL_KEKKAI_DATA_add_r1024 	= (-3); /* �����v��� */
					h->PL_KEKKAI_DATA_radius		= (16); 	/*(45)*/	//d->radius=35; 	/* �������L���锼�a�����l */
				}
			}
			else
			#endif
			{
				if (0 == (ii&1))// if (i%90 == 0)
				{
					h->PL_KEKKAI_DATA_add_r1024 	= ( 46);	/* ���v��� */
					h->PL_KEKKAI_DATA_radius		= (16); 	/*(48)*/	//d->radius=38; 	/* �������L���锼�a�����l */
				}
				else
				{
					h->PL_KEKKAI_DATA_add_r1024 	= (-34);	/* �����v��� */
					h->PL_KEKKAI_DATA_radius		= (0);		/*(45)*/	//d->radius=35; 	/* �������L���锼�a�����l */
				}
			}
			/* �얲 �� �H�X�q */
			h->obj_type_set 			= (ii&(4-1))+(/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_04|OBJ_Z80_SHOT_SPECIAL);/* �{�X�ɗL�� */
			h->callback_mover			= reimu_yuyuko_move_rotate_kekkai;		/*	���v��� �� �����v��� �� �X�N�G�A���E(�얲B) ���p */
			h->PL_KEKKAI_DATA_angleCCW1024		= (jj);//(ii<<7);	/* (1024/8) */
			/* �o�n�߂̓v���C���[���W */
		//	{
		//		OBJ *zzz_player;
		//		zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
		//		h->center.x256					= zzz_player->center.x256;
		//		h->center.y256					= zzz_player->center.y256;
				h->center.x256					= src->center.x256; /* ���e���̃v���C���[�̍��W�������p���B */
				h->center.y256					= src->center.y256; /* ���e���̃v���C���[�̍��W�������p���B */
		//	}
		}
		/* �얲A��	 8 == 4�e x 2��� */
		/* �얲B��	16 == 8�e x 2��� */
		/* �H�X�q�� 16 == 8�e x 2��� */
	//	jj += ((YUYUKO==(cg_game_select_player))?(1024/16):(1024/8));
		jj += ((REIMU_A==(cg_game_select_player))?(1024/8):(1024/16));
		ii++;
	}
	#if (1)/*(�H�X�q�p)*/
	{
		OBJ *s1;
		s1					= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_02_BOMBER_HONTAI];
		s1->center.x256 		= (src->center.x256); /* ���e���̃v���C���[�̍��W�������p���B */
		s1->center.y256 		= (src->center.y256); /* ���e���̃v���C���[�̍��W�������p���B */
	}
	#endif
}

/*---------------------------------------------------------
	�M���X�g���h���[��
	�v���C���[�{�������̐e
---------------------------------------------------------*/

//	/*static*/global OBJ_CALL_FUNC(yuyuko_create_bomber_gyastry_dream_parrent)		/* �얲 */		//�V�[���h�̒ǉ�
//{
//	reimu_create_bomber_kekkai_parrent(src);
//}

/*---------------------------------------------------------
	�}�X�^�[�X�p�[�N�A�{�������̐e
	(������B ��p)
---------------------------------------------------------*/

/*static*/global OBJ_CALL_FUNC(marisa_create_bomber_master_spark_parrent)
{
	int iii;
	for (iii=0; iii<(3)/*16*/; iii++)
	{
		OBJ *h;
		h					= obj_regist_teki();
	//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		if (NULL==h)return;/* �o�^�ł����ꍇ�̂� */ 	/*���[��H�H�H*/
		{	set_bomb_str_STD(h);

			static const u32 color_mask32[(4)] =
			{	//		   RED	 GREEN BLUE  ALPHA
				MAKE32RGBA(0xff, 0x00, 0x00, 0x3f), 	// (��)
				MAKE32RGBA(0x00, 0xff, 0x00, 0x3f), 	// (��)
				MAKE32RGBA(0x00, 0x00, 0xff, 0x3f), 	// (��)
				MAKE32RGBA(0xff, 0xff, 0xff, 0x3f), 	// (��)
			};
			h->color32					= (color_mask32[iii]);
			h->obj_type_set 					= (/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_04|OBJ_Z80_SHOT_SPECIAL);/* �{�X�ɗL�� */
			h->callback_mover			= marisa_move_master_spark;
			h->tmp_angleCCW1024 		= ((iii)<<8);
			h->m_zoom.x256				= (256*4);	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
			h->m_zoom.y256				= (256*4);	/* �`��p�{���̏����T�C�Y / size of bomb at first. */

			h->math_vector.x256 				= (0);	/*fps_factor*/	/* CCW�̏ꍇ */
			h->math_vector.y256 				= (0);	/*fps_factor*/
			#if (1)/* �ȗ��\�����H */
			h->center.x256					= src->center.x256;
			h->center.y256					= src->center.y256-t256(MASKER_SPARK_LENGTH144);
			#endif
//			/* �`��p�p�x(����0�x�ō����(�����v���)) */
//			h->rotationCCW1024		= (0);
		}
	}
}


/*---------------------------------------------------------
	�`���m�H��p
	�v���C���[�{�������̐e
---------------------------------------------------------*/

/*static*/global OBJ_CALL_FUNC(chrno_q_create_bomber_gyastry_dream_parrent)
{
	int jjj;		jjj=0;
	int angCCW1024; angCCW1024 = (0);
	int iii;
	for (iii=0; iii<(18*3)/*16*/; iii++)
	{
		OBJ *h;
		h					= obj_regist_teki();
	//
	//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		if (NULL==h)return;/* �o�^�ł����ꍇ�̂� */ 	/*���[��H�H�H*/
		{
			set_bomb_str_STD(h);
			//
			jjj++;
			jjj &= 0x03;	//	if (4==jjj) 	{	jjj=0;	}
			h->obj_type_set 		= (/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_04|OBJ_Z80_SHOT_SPECIAL);/* �{�X�ɗL�� */
			h->obj_type_set += jjj;
	//
			h->callback_mover	= marisa_yuyuko_move_levarie_gyastry_dream;
			#if 0
			{
				static const u16 player_fix_status_ggg[8] =
				{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
					256, 256, 256, 256, 32, 256, 32, 32,	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
				};
				h->m_zoom.x256		= player_fix_status_ggg[(cg_game_select_player)];/*8*/ /* 64 == (1/4) */
			}
			#else
		//	if (YUYUKO!=(cg_game_select_player))
			{	/*(�`���m�p)*/
				h->m_zoom.x256		= 1+(ra_nd()&(64-1));	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
				h->m_zoom.y256		= 1+(ra_nd()&(64-1));	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
			}
			#endif

			/* MARISA�͋������g�U���Ă��܂��̂ł�����Ȃ��B */
			/* �`���m�͓��Ă₷���̂ł��̕��キ���Ȃ��Ƌ��������Ⴄ�B */
			int d_speed256;
			#if 0
			if (0==(iii&(2-1)/*%2*/))
			{			/*d->*/d_speed256	= t256(1.0);	c->alpha	= 150;	}
			else	{	/*d->*/d_speed256	= t256(1.2);	c->alpha	= 180;	}
			#else
			/*d->*/d_speed256	= t256(1.0)+(iii<<3);
			#endif
			#if (0)//
			h->math_vector.x256 	= ((si n1024((angCCW1024))*(d_speed256))>>8);/*fps_factor*/ 	/* CCW�̏ꍇ */
			h->math_vector.y256 	= ((co s1024((angCCW1024))*(d_speed256))>>8);/*fps_factor*/
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (angCCW1024), &sin_value_t256, &cos_value_t256);
				h->math_vector.x256 	= ((sin_value_t256*(d_speed256))>>8);/*fps_factor*/
				h->math_vector.y256 	= ((cos_value_t256*(d_speed256))>>8);/*fps_factor*/
			}
			#endif
			#if 1
			/* �`��p�p�x(����0�x�ō����(�����v���)) */
			h->rotationCCW1024		= angCCW1024;
			#endif
		//	angCCW1024 += (int)((1024)/16); // 22.5�x/360, 2x��/16 /* 0.392699081698724154810 */
			angCCW1024 += (int)((1024)/18); // 20.0�x/360, 2x��/18 /* 0.349065850398865915384 */
	//
			#if 1/*Gu(���S���W)*/
			h->center.x256			= src->center.x256;
			h->center.y256			= src->center.y256;
			#endif
			/* �M���X�g���h���[��(���ǂ�)�B�X�^�[�_�X�g�����@���G�ƈႢ�ߐڐ��\�͖����B */
//			if (YUYUKO==(cg_game_select_player))	/* �H�X�q */
//			{
//				h->center.x256		+= ((h->math_vector.x256)<<5);
//				h->center.y256		+= ((h->math_vector.y256)<<5);
//	//			/* �H�X�q �������邩��ȁ`(���ʎア�����ʂ���̂��o����悤�ɃV�X�e�����������ق�����������) */
//			}
		}
	}
}


/*---------------------------------------------------------
	�����u�X�^�[�_�X�g�����@���G�v
	-------------------------------------------------------
	[�v���C���[�V�[���h]�{�������̐e(������ A)
	-------------------------------------------------------
    �X�^�[�_�X�g�����@���G(3x8==�v24�����o��)
    ��8�����ɐ����o��B���̐F�́A���܂��Ă���B

        ����    ��      �E��
        (��0)   (��2)   (��1)

        ��              �E
        (��1)           (��2)

        ����    ��      �E��
        (��2)   (��0)   (��1)

    ��==0
    ��==1
    ��==2

    -------------------------------------------------------
    ������ł́A3�����o��B�摜�̍�����W��(x,y)�Ƃ���ƁA

    �启=�������A���W(x,y)
    ����=�������A���W(x,y)
    ����=�s�����A���W(x+64,y+64)    // 64 �� �e�L�g�[�Ȉ��l�B
                                    // psp�̏ꍇ�́A��ʂ������̂�48���Ó��H
    ---------------------------------------------------------*/

/*static*/global OBJ_CALL_FUNC(marisa_create_bomber_star_dust_levarie_parrent)
{
	int angCCW1024; angCCW1024 = (0);
	int mmm;
	mmm = (0x100);
	int kkk;
	for (kkk=0; kkk<(3); kkk++)/*3�T�C�Y*/
	{
		int iii;
		for (iii=0; iii<(8); iii++)/*8����*/
		{
			OBJ *h;
			h					= obj_regist_teki();
		//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
			if (NULL==h)return;/* �o�^�ł����ꍇ�̂� */ 	/*���[��H�H�H*/
			{
				set_bomb_str_STD(h);
				//
				static const u32 color_mask32[8] =	/* �启�A�����͔������B�����͕K���s�����B */
				{	//		   RED	 GREEN BLUE  ALPHA
					MAKE32RGBA(0xff, 0x00, 0x00, 0x7f), 	//	  ��(��0)
					MAKE32RGBA(0x00, 0xff, 0x00, 0x7f), 	//	�E��(��1)
					MAKE32RGBA(0x00, 0x00, 0xff, 0x7f), 	//	�E	(��2)
					MAKE32RGBA(0x00, 0xff, 0x00, 0x7f), 	//	�E��(��1)
					MAKE32RGBA(0x00, 0x00, 0xff, 0x7f), 	//	  ��(��2)
					MAKE32RGBA(0xff, 0x00, 0x00, 0x7f), 	//	����(��0)
					MAKE32RGBA(0x00, 0xff, 0x00, 0x7f), 	//	��	(��1)
					MAKE32RGBA(0x00, 0x00, 0xff, 0x7f), 	//	����(��2)
				};
				h->color32					= (color_mask32[((iii)/*&0x07*/)]);
				h->obj_type_set 			= (/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_04|OBJ_Z80_SHOT_SPECIAL);/*�{�X�ɗL��*/
				//
				h->callback_mover			= marisa_yuyuko_move_levarie_gyastry_dream;
				h->m_zoom.x256				= (mmm);	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
				h->m_zoom.y256				= (mmm);	/* �`��p�{���̏����T�C�Y / size of bomb at first. */
				{
					const int d_speed256	= t256(1.5);	/*(���x���)*/
					#if (0)//
					h->math_vector.x256 	= ((si n1024((angCCW1024))*(d_speed256))>>8);/*fps_factor*/ 	/* CCW�̏ꍇ */
					h->math_vector.y256 	= ((co s1024((angCCW1024))*(d_speed256))>>8);/*fps_factor*/
					#else
					{
						int sin_value_t256; 				//	sin_value_t256 = 0;
						int cos_value_t256; 				//	cos_value_t256 = 0;
						int256_sincos1024( (angCCW1024), &sin_value_t256, &cos_value_t256);
						h->math_vector.x256 	= ((sin_value_t256*(d_speed256))>>8);/*fps_factor*/
						h->math_vector.y256 	= ((cos_value_t256*(d_speed256))>>8);/*fps_factor*/
					}
					#endif
				}
					#if 1
					/* �`��p�p�x(����0�x�ō����(�����v���)) */
					h->rotationCCW1024		= angCCW1024;
					angCCW1024 += (int)((1024)/8);	// 45.0�x/360, 64
					#endif
				#if 1/*Gu(���S���W)*/
				{
					h->center.x256			= src->center.x256;
					h->center.y256			= src->center.y256;
				}
				if (0==kkk)/*(mini star only.)(�����̏ꍇ�B)*/
				{
					h->center.x256			+= (t256(48));	/*(�����̂�(x,y�����l��)�I�t�Z�b�g�K���t���B)*/
					h->center.y256			+= (t256(48));	/*(�����̂�(x,y�����l��)�I�t�Z�b�g�K���t���B)*/
					h->color32			|= (MAKE32RGBA(0, 0, 0, 0xff)); 	/*(not alpha)(�����͕K���s�����B)*/
				}
				#endif
			}
		}
		mmm += mmm;
	}
}

/*----------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------*/


//		h->m_Hit256R		= JIKI_ATARI_ITEM_16;

//		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0xdc);		/*	h->alpha			= 0xdc;*/	/* ��\�� */
//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* ���[��... */
	//	#if 1/*Gu(���S���W)*/
	//	h->center.x256			= t256(GAME_NOT_VISIBLE480);//(src->center.x256); /* ��\�� */
//	//	h->center.y256			= (src->center.y256); /* ��\�� */
	//	#endif


/*---------------------------------------------------------
	���@�V���b�g�̐e���\���ɂ���B
---------------------------------------------------------*/

global void jiki_shot_oya_hide(void)
{
	OBJ *h;
	h						= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_02_BOMBER_HONTAI];
	{
		#if 1/*Gu(���S���W)*/
		h->center.x256			= t256(GAME_NOT_VISIBLE480);//(src->center.x256); /* ��\�� */
		h->center.y256			= t256(272+64); /* ��\�� */
		#endif
	}
}

/*---------------------------------------------------------
	�v���C���[�V�[���h����(���~���A)�̐e[���~���A�{���̏\���V���b�g�{��]
---------------------------------------------------------*/

/*static*/global OBJ_CALL_FUNC(remilia_create_bomber_cross_shot_parrent)
{
	OBJ *h;
	h						= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_02_BOMBER_HONTAI];
	{
		h->callback_mover	= bomber_hontai_remilia_tuika_cross_shot;
		h->obj_type_set 			= (/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_01|OBJ_Z80_SHOT_SPECIAL);/* �{�X�ɗL�� */
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
		#if 1/*Gu(���S���W)*/
		/*�J�n���̍��W*/
		h->center.x256 = zzz_player->center.x256;
		h->center.y256 = zzz_player->center.y256;
		#endif
		{
			OBJ *obj_boss;
			obj_boss			= &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+TEKI_OBJ_00_BOSS00_HONTAI];/*(�{�X�{��)*/
			int int_angle1024;
			int int_aaa;
			REG_00_SRC_X	= (obj_boss->center.x256);			/*(�_����)*/
			REG_01_SRC_Y	= (obj_boss->center.y256);			/*(�_����)*/
			REG_02_DEST_X	= (zzz_player->center.x256);		/*(�_����)*/
			REG_03_DEST_Y	= (zzz_player->center.y256);		/*(�_����)*/
			tmp_angleCCW65536_src_nerai();
			int_angle1024 = HATSUDAN_03_angle65536;
			int_angle1024 >>= (6);
			#if (0)//
			int_aaa = si n1024((int_angle1024));	h->math_vector.x256  = int_aaa+int_aaa;/*fps_factor*/	/* CCW�̏ꍇ */
			int_aaa = co s1024((int_angle1024));	h->math_vector.y256  = int_aaa+int_aaa;/*fps_factor*/
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (int_angle1024), &sin_value_t256, &cos_value_t256);
				int_aaa = (sin_value_t256); 	h->math_vector.x256  = int_aaa+int_aaa;/*fps_factor*/
				int_aaa = (cos_value_t256); 	h->math_vector.y256  = int_aaa+int_aaa;/*fps_factor*/
			}
			#endif
			#if 1
			/* �`��p�O����] */
		//	h->rotationCCW1024	= int_angle1024;
			/* �`��p�O����](�V���b�g�͏オ���ʂȂ̂ŁA��]�p���v�Z)[180/360�x]�񂷁B */
			int_angle1024 += cv1024r((180));
			mask1024(int_angle1024);
			h->rotationCCW1024	= int_angle1024;/* �������ʂ̊p�x���`��p�p�x */
			#endif
		}
	}
}

/*---------------------------------------------------------
	�������A�`���m���A�ᑬ�{���̐e
---------------------------------------------------------*/
/*static*/global OBJ_CALL_FUNC(marisa_create_bomber_homing_parrent)
{
	OBJ *h;
	h						= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_02_BOMBER_HONTAI];
	{
		h->callback_mover	= bomber_hontai_marisa_tuika_bomber_teisoku_yuudoudan;
		h->obj_type_set 			= (/*�\�����Ȃ�*/SP_DUMMY_MUTEKI);/*�{�X�ɗL��*/	/* �ᑬ�{�����{�X�ɗL���Ƃ��� */
		#if 1/*Gu(���S���W)*/
		h->center.x256			= t256(GAME_NOT_VISIBLE480);//src->center.x256; /* ��\�� */
//		h->center.y256			= src->center.y256; /* ��\�� */
		#endif
//
		#if 1
		/* �{�������������A�L���ɂ���ƁA�d������B */
		/* ����@�\ */
		bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
		#endif
	}
}
/*---------------------------------------------------------
	�얲 A (�ʏ�{��) / �`���m A(�ᑬ�{��) �̐e
---------------------------------------------------------*/
/*static*/global OBJ_CALL_FUNC(reimu_create_bomber_homing_shot_parrent) /* reimu_move_add_bomber_hlaser */
{
	OBJ *h;
	h						= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_02_BOMBER_HONTAI];
	{
		h->callback_mover	= bomber_hontai_reimu_tuika_musofuuin_shot;
		h->obj_type_set 	= (JIKI_BOMBER_01|OBJ_Z80_SHOT_SPECIAL);/* �{�X�ɗL�� */
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
		#if 1/*Gu(���S���W)*/
		/*�J�n���̍��W*/
		h->center.x256 = zzz_player->center.x256;
		h->center.y256 = zzz_player->center.y256;
		#endif
		{
			#if 1
			/* �`��p�O����] */
		//	h->rotationCCW1024	= int_angle1024;
			/* �`��p�O����](�V���b�g�͏オ���ʂȂ̂ŁA��]�p���v�Z)[180/360�x]�񂷁B */
			h->rotationCCW1024	= 0;/* �������ʂ̊p�x���`��p�p�x */
			#endif
		}
	}
	#if 0
	/* �{�������������A�L���ɂ���ƁA�d������B */
	/* ����@�\ */
	bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
	#endif
}
/*---------------------------------------------------------
	�H�X�q��p �ᑬ�{���̐e 	[(�H�X�q�ᑬ)�{���̏\�����{��]
---------------------------------------------------------*/
/*static*/global OBJ_CALL_FUNC(yuyuko_create_bomber_meifu_parrent)
{
	OBJ *h;
	h						= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_02_BOMBER_HONTAI];
	{
		h->center.x256			= (src->center.x256); /* ���e���v���C���[���W�������p�� */
		h->center.y256			= (src->center.y256); /* ���e���v���C���[���W�������p�� */
		h->m_zoom.x256		= t256(1.00);
		h->m_zoom.y256		= t256(1.00);
		h->callback_mover	= bomber_hontai_yuyuko_tuika_meifu;
//		h->obj_type_set 	= (/*�\�����Ȃ�*/SP_DUMMY_MUTEKI); /* �{�X�̒��ڍU���͋֎~ */
		h->obj_type_set 	= (JIKI_SHOT_00); /* [�H�X�q�{���̐�{��] */ /* �{�X�̒��ڍU���͋֎~ */
		h->atari_hantei 	= (0); /* �{�X�̒��ڍU���͋֎~ */
		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x64);		/*	h->alpha			= 0x64 100;*/
		#if 1/*Gu(���S���W)*/
//		h->center.x256			= t256(GAME_NOT_VISIBLE480);//(src->center.x256); /* ��\�� */
//		h->center.y256			= (src->center.y256); /* ��\�� */
		#endif
	}
}

/*----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------
	�e�v���C���[�̐��\��
---------------------------------------------------------*/
/* �얲 ����\�́F��炢�{���̎�t���Ԃ����� */
/* �`���m ����\�́F�H */

/*---------------------------------------------------------
	�v���C���[�B �{���̎�����ݒ�
--------------------------------------------------------- */
global void jiki_shot_set_bomber_time(void)
{
	//	#define PD_BOMBER_JYUMYOU180					(180)		/* �{���̎���==180[frame] */
	//	cg.bomber_time		= (PD_BOMBER_JYUMYOU180);//cg.bomber_time_table[(index_aaa)];
	cg.bomber_time		= (current.shot_status[SHOT_STATUS_24_PLAYER_SET_BOMBER_TIME]);//cg.bomber_time_table[(index_aaa)];
}
/*---------------------------------------------------------
	�v���C���[�B�O���C�Y�̓����蔻�蔼�a��ݒ�
--------------------------------------------------------- */
global int jiki_shot_get_player_graze_hankei(void)
{
	return (current.shot_status[SHOT_STATUS_23_PLAYER_GRAZE_ATARI_HANKEI]);
}

/*---------------------------------------------------------
	�v���C���[�̃A�j���[�V�������x��ݒ�
--------------------------------------------------------- */
global int jiki_shot_get_player_animation_speed(void)
{
	return (current.shot_status[SHOT_STATUS_25_PLAYER_ANIME_SPEED]);
}

/*---------------------------------------------------------
	��炢�{���̓��͎󂯕t�����Ԃ�ݒ�
---------------------------------------------------------*/
global int jiki_shot_get_kurai_bombtimer(void)
{
	return ((-1)-current.shot_status[SHOT_STATUS_22_KURAI_BOMB_UKETUKE_JIKAN]);
}

/*---------------------------------------------------------
	�V���b�g�֘A�̐��\�̏����l��Z�߂Ĉꊇ�ݒ肷��B
---------------------------------------------------------*/

global void init_player_shot(void)
{
	SHOT_STATUS aaa_fix[(PLAYERS8)] =
	{
		#if 0/*(�l�̐���)*/
		// �e1 ... �e6: �V���b�g�̋��� L1 ... L6�B
		// �j1 ... �j6: �T�u�E�F�|���̋��� L1 ... L6�B
		// ��1 ... ��6: ���@�V���b�g�����Ԋu L1 ... L6�B
		// �I1 ... �I6: �I�v�V�����V���b�g�����Ԋu L1 ... L6�B
		// �e��: �V���b�g�̐i�s���x�B�V���b�g���̂̑����B
		// �j��: �T�u�E�F�|���̐i�s���x�B�T�u�E�F�|�����̂̑����B
		// �e��: �V���b�g�̂����蔻�蔼�a�B[pixel]
		// �j��: �T�u�E�F�|���̂����蔻�蔼�a�B[pixel]
		// �W�{: �ʏ�{���̋���
		// ��{: �ᑬ�{���̋���
		// �W��: �ʏ�{���̂����蔻�蔼�a�B[pixel]
		// �ᔻ: �ᑬ�{���̂����蔻�蔼�a�B[pixel]
		// ��: �{���̉�]���x�B
		// �g��: �{���̊g�呬�x�B
		// ��: ��炢�{���̎󂯕t���P�\�������ԁB�l�����������A�󂯕t�����Ԃ������B / hit_bomb_wait.
		// �O��: �O���C�Y�̂����蔻�蔼�a�B[pixel]
		// �{��: �{���L�����ԁB�{���̎����B(�ʏ�{���ƒᑬ�{�����ʂȂ̂Œ���)
		// �v�A: �v���C���[�̃A�j���[�V�������x�B
		// ��]: �I�v�V��������]�A�j���[�V����������(0)�����Ȃ�(����ȊO)���B
		// �A��: �I�v�V��������]�̏ꍇ�A��]���x�B�A�j���[�V�����̏ꍇ�A�A�j���[�V�������x�B
		{	/*(���O)*/
			{
				�e1,  �e2,	�e3,  �e4,	�e5,  �e6,		�e��,  �j��,
				�j1,  �j2,	�j3,  �j4,	�j5,  �j6,		�e��,  �j��,
				��1,  ��2,	��3,  ��4,	��5,  ��6,		�W�{,  ��{,
				�I1,  �I2,	�I3,  �I4,	�I5,  �I6		�W��,  �ᔻ,
				��, �g��, ��, �O��, (�{��), �v�A,	��],  �A��,
			}
		},
		#endif
		{	/*(�얲A)*/
			{
				 20,  23,  13,	11,   7,   5,		  9,  9,
				 30,  28,  26,	24,  22,  20,		  4, 10,
				  5,   5,	5,	 5,   5,   5,		 48,160,
				 99,  77,  55,	33,  10,   4,		  2, 16,	// (r33)�U���ޏ��̃I�v�V�����͎ア�ƑS�R���C���Ȃ��B
				  0,   0,	0,	16, (180),	 8, 	  0,  4,
			}
		},
		{	/*(�얲B)*/
			{
				 20,  16,	8,	 8,   4,   6,		  9, 18,	// �얲 (��) - �j�[�h�� :: �j�[�h���͒ʏ�e��2�{��
				 16,  16,  10,	10,  10,  12,		  4,  6,
				  5,   5,	5,	 5,   5,   5,		 40,192,
				 13,  11,	9,	 7,   5,   3,		  2, 16,
				  0,   0,	0,	16, (180),	 8, 	  0,  4,
			}
		},
		{	/*(������A)*/
			{
				 20,  14,  16,	12,  16,  18,		  9,  6, // 6.3==9*0.7 ������ (��) - �~�T�C�� :: �~�T�C���͒ʏ�e��70���B
				 20,  20,  14,	12,  12,  14,		  4,  8,
				  6,   6,	5,	 5,   5,   5,		 40,240,
				 11,  10,	9,	 8,   7,   6,		 32, 16,
				 20,   0,	8,	32, (180),	 2, 	  0,  6,
			}
		},
		{	/*(������B / �}�X�p / �U����)*/
			{
				 20,  22,  14,	12,  12,  10,		  9,  9, // ?? ??	   ������ (��) - ���[�U�[ :: �X�s�[�h����s�\�B�Ƃ������S�Ăɂ����đ��荢��B
				 32,  64,  80,	88,  98, 108,		  4,  6,
				  6,   6,	5,	 5,   5,   5,		255,255,	// 40
				  4,   4,	4,	 4,   4,   3,		128, 16,	// 255 128 �}�X�p
				 16,   0,	8,	32, (180),	 2, 	  0,  8,
			}
		},
		{	/* ���~���A */
			{
				 40,  37,  36,	35,  34,  33,		  9,  3,
				 48,  48,  49,	49,  50,  52,		  4, 12,
				 14,  13,  12,	11,  10,  10,		 40,127,	//(r35u1, 40==8*5)	 32,127,
				  8,   8,	8,	 8,   8,   8,		 16, 16,
				  0,   0,  12,	16, (180),	 3, 	  0,  4,
			}
		},
		{	/* �H�X�q /  / ��(�Ԓe) */
			{
			//	128, 129, 130, 132, 136, 144,		  4,  4,
				 80,  96, 112, 128, 132, 144,		  4,  4,
				 64,  64,  64,	65,  65,  66,		  8,  8,
				 17,  16,  15,	14,  13,  13,		 48,127,	// 40(r35u1, 40==8*5)	 48,160,
				 31,  28,  25,	22,  19,  16,		  4, 16,
				  0,   0,	4,	48, (255),	 4, 	  1, 12,
			}
		},
		{	/* �`���mA */
			{
				 40,  40,  40,	40,  48,  64,		  8,  3,
				 48,  48,  48,	48,  56,  80,		  6, 16,
				  9,   9,	9,	 9,   9,   9,		240,240,
				  8,   8,	8,	 8,   7,   5,		 32, 16,
				 18,   3,	3,	48, (180),	 1, 	  1,  3,
			}
		},
		{	/* �`���m�H */
			{
				 40,  40,  40,	40,  44,  60,		  9,  4,
				 48,  48,  48,	48,  52,  78,		  9, 24,
				  9,   9,	9,	 9,   9,   9,		255,255,
				  9,   9,	9,	 9,   8,   6,		 32, 16,
				 18,   3,	2,	48, (180),	 1, 	  1,  3,
			}
		},
	};
	unsigned int i;
	for (i=0; i<SHOT_STATUS_32_MAX; i++)
	{
		current.shot_status[i] = aaa_fix[(cg_game_select_player)].shot_status[i];
	}
}


	#if 0/* ���܂Ō��������{������(��Ȃ���) */
//	/* �ᑬ�{�� */			cg.bomber_time = 200/* 320 */ /* 400 */ /* 800 */;		/* �{���̗L������(�ݒ薳�G����)[���P�d�l���ς�����̂Ŕ����ɂ���] */
//	/* �얲 */				cg.bomber_time = 180/* 200 */;							/* �{���̗L������(�ݒ薳�G����) */	/* ������̓{�X�L��(�U���^) */
//	/* ������	�`���m */	cg.bomber_time = 100/* 32 */ /* 100 */; 				/* �{���̗L������(�ݒ薳�G����) */
//	/* �� �L������ */		cg.bomber_time = 255/* 255==((16*4*2*2)-1) */;			/* �{���̗L������(�ݒ薳�G����) */
//	���~���A�{��			cg.bomber_time = 0x7e	/* 255==0xff==8��	254==0xfe==7��==0xbf=192 4��==0x7f 150==0x96 */;
	static const u8 cg.bomber_time_table[/* 16 */(PLAYERS8*2)] =
	{
		180,		180,		/* 180 200 �얲 A */
		180,		200,		/* 180 200 �얲 B */
		100,		180,		/* 100 200 ������ A */
		100,		200,		/* 100 200 ������ B */
		0x7e,		255,		/* 0x7e 200���~���A */
		180,		180,		/* 255 200 �H�X�q */
		100,		180,		/* 100 200 �`���m A */
		100,		255,		/* 100 200 �`���m Q */
	};
	#endif
