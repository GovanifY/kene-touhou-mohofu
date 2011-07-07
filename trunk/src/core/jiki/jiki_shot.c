
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�e�v���C���[(�얲 �� ������ �� ���~���A �� �H�X�q �� �`���m)
	REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q)
	-------------------------------------------------------
---------------------------------------------------------*/

#include "jiki_local.h"

/* �얲 A (���z�c�ޏ�) / ������ A (�X���@�g��) �p�̗U���ڕW���W */
#define PL_SHOT_DATA_target_x256		user_data00
#define PL_SHOT_DATA_target_y256		user_data01


enum
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
//	/* ��U���V���b�g(�H�X�q) */
	YUYUKO_SHOT_TYPE_08,
	YUYUKO_SHOT_TYPE_09,
	YUYUKO_SHOT_TYPE_0a,
	YUYUKO_SHOT_TYPE_0b,
//	/* �j(�T�u�V���b�g) */
	NEEDLE_ANGLE_270,
	NEEDLE_ANGLE_263,	/* ���~���A �p */
	NEEDLE_ANGLE_277,	/* ���~���A �p */
	NEEDLE_ANGLE_271,	/* �H�X�q �p */
//
	REIMU_SHOT_TYPE_MAX 	/* �ő吔 */	//	NEEDLE_MAX
};

/*static*/extern int cg_jiki_weapon_level_offset; /* �����������탌�x��(�ԍ�)�̂W�{�Ƀv���C���[�l�𑫂����l���I�t�Z�b�g�Ƃ��ĕێ� */
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

/*---------------------------------------------------------
	�v���C���[�A�j�e�A�V���b�g�e�̈ړ�
	�얲 �� ������ �� ���~���A �� �H�X�q �� �`���m
---------------------------------------------------------*/


static void remilia_move_object(SPRITE *src)
{
	src->cx256 += src->vx256;		/*fps_factor*/
	src->cy256 += src->vy256;		/*fps_factor*/
//	if (REMILIA==(cg_game_select_player))	/* ���~���A�͂���� */
	{	u16 rand_int;
		rand_int = ra_nd();
		src->cx256 -= (rand_int&0x0200);/*0x0100*/
		src->cx256 += (rand_int&0x0100);/*0x0080*/
		src->cx256 += (rand_int&0x0100);/*0x0080*/
	}
	if (src->cy256 < t256(-10))
	{
		src->jyumyou = JYUMYOU_NASI;
	}
	else
	{
		player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_00_SHOT);
	}
}

static void yuyuko_move_object(SPRITE *src)
{
	src->cx256 += src->vx256;		/*fps_factor*/
	src->cy256 += src->vy256;		/*fps_factor*/
//	if (YUYUKO==(cg_game_select_player))		/* �H�X�q�͂���� */
	{
		src->vx256 += ((src->vx256*cg.weapon_power)>>11);
	}
	if (src->cy256 < t256(-10))
	{
		src->jyumyou = JYUMYOU_NASI;
	}
	else
	{
		player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_00_SHOT);
	}
}
static void zabuton_move_object(SPRITE *src)
{
	if (src->cx256 < src->PL_SHOT_DATA_target_x256 )
	{
		src->cx256 += src->vx256;		/*fps_factor*/
	}
	else
	{
		src->cx256 -= src->vx256;		/*fps_factor*/
	}
	src->cy256 += src->vy256;		/*fps_factor*/
//
//
//	if (REIMU==(cg_game_select_player)) 		/* �얲�̉�]�V���b�g */
//	if ((REIMU_B+1) > (cg_game_select_player))	/* �얲�̉�]�V���b�g */
	{
		if (((JIKI_SHOT_01)|SP_GROUP_SHOT_SPECIAL)==src->type)
		{
			/* �`��p�O����] */
			src->rotationCCW1024 += (32);	/* cv1024r(10) */
			mask1024(src->rotationCCW1024);
		}
	}
	if (src->cy256 < t256(-10))
	{
		src->jyumyou = JYUMYOU_NASI;
	}
	else
	{
		player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_00_SHOT);
	}
}

static void other_move_object(SPRITE *src)
{
	src->cx256 += src->vx256;		/*fps_factor*/
	src->cy256 += src->vy256;		/*fps_factor*/
	if (src->cy256 < t256(-10))
	{
		src->jyumyou = JYUMYOU_NASI;
	}
	else
	{
		player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_00_SHOT);
	}
}
#define reimu_a_move_shot	zabuton_move_object
#define reimu_a_move_hari	other_move_object
#define reimu_b_move_shot	other_move_object
#define reimu_b_move_hari	other_move_object
#define marisa_a_move_shot	other_move_object
#define marisa_a_move_hari	other_move_object
#define marisa_b_move_shot	other_move_object
#define marisa_b_move_hari	other_move_object

#define remilia_move_shot	remilia_move_object
#define remilia_move_hari	remilia_move_object
#define yuyuko_move_shot	yuyuko_move_object
#define yuyuko_move_hari	yuyuko_move_object
#define chrno_a_move_shot	other_move_object
#define chrno_a_move_hari	other_move_object
#define chrno_q_move_shot	other_move_object
#define chrno_q_move_hari	other_move_object

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


//global void player_register_shot_object(SPRITE *src, int /*set_*/shot_type);
/*static*/static/*global*/ void player_register_shot_object(SPRITE *src, int /*set_*/shot_type)
{
	SPRITE *h;/* shot */
	h					= obj_add_01_teki_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		static const s8 r_tbl[REIMU_SHOT_TYPE_MAX][REI04_MAX] =
		{	/*x_offset256	y_offset256  vx256 status */
	//	/* �U���V���b�g(�얲 �� ������) */
	/*00*/	{ (  0),	(  0),	(	0), 	0x01},	 /*shot_regist_triple_fuda*/
	//	/* ��U���V���b�g(�얲) */
	/*01*/	{ (  0),	(  0),	(	0), 	0x00},	/* �� �܂����� */
	/*02*/	{ ( -5),	(  0),	(	0), 	0x00},	/* �� �܂����� */
	/*03*/	{ ( +5),	(  0),	(	0), 	0x00},	/* �E �܂����� */
	/*04*/	{ ( -5),	(  0),	( -20), 	0x00},	/* �� ���� */
	/*05*/	{ ( +5),	(  0),	(  20), 	0x00},	/* �E �E�� */
	/*06*/	{ ( -5),	(  0),	( -35), 	0x00},	/* �� ���Ώ� */
	/*07*/	{ ( +5),	(  0),	(  35), 	0x00},	/* �E �E�Ώ� */
	//	/* ��U���V���b�g(�H�X�q) */
	/*08*/	{ (-15),	(-25),	( -25), 	0x00},	 /* �H�X�q */					 /*(-32)==(4*(- 8))*/
	/*09*/	{ (+15),	(-25),	( +25), 	0x00},	 /* �H�X�q */					 /*( 32)==(4*(	8))*/
	/*0a*/	{ (-25),	(-25),	( -50), 	0x00},	 /* �H�X�q */					 /*(-64)==(4*(-16))*/
	/*0b*/	{ (+25),	(-25),	( +50), 	0x00},	 /* �H�X�q */					 /*( 64)==(4*( 16))*/
	//		/* CCW�̏ꍇ */
			{ (  0),	( -7),	(	0), 	0x01},//	cv1024r((180)), 				/* ������ A/B �얲 ���~���A �� �`���m */		/*t256( 0), t256( 0),*/
			{ (  0),	( -7),	(-108), 	0x01},//	cv1024r((180-25)),/*263=270-7*/ /* ���~���A �� �`���m */ /*t256( 0), t256( 0),*/
			{ (  0),	( -7),	( 108), 	0x01},//	cv1024r((180+25)),/*277=270+7*/ /* ���~���A �� �`���m */ /*t256( 0), t256( 0),*/
			{ (  0),	(-47),	(	5), 	0x01},//	cv1024r((181)), 				/* �H�X�q��p */	/*-48*/ /*t256( 0), t256(-40),*/
		};
	//	int shot_type;
		int shot_status;/* �j? */
		shot_status = ((r_tbl[shot_type][REI03_status]));

		int is_needle;/* �j? */
	//	is_needle = (shot_type <= NEEDLE_ANGLE_270)?(1/* �j */):(0/* �e */);
		is_needle = ((shot_status)&0x01);
	//
		#define OFFS_IS_NEEDLE ((cg_game_select_player)+(is_needle<<3))
		const static u8 base_shot_atari[16] =
		{	/* ���C���V���b�g(�D)�̂����蔻�� BASE_SHOT_ATARI */
			(4),	/* �얲 A */		//	/*	4  5  4 */
			(4),	/* �얲 B */		//	/*	4  5  4 */
			(4),	/* ������ A */		//	/*	4  6  4 */
			(4),	/* ������ B */		//	/*	4  6  4 */
			(4),	/* ���~���A */		//	/*	4  7  4 */
			(8),	/* �H�X�q */		//	/*	4  9  8 */
			(6),	/* �`���m A */		//	/*	6  8  6 */
			(9),	/* �`���m Q */		//	/*	8  6  4 */
			/* �T�u�V���b�g(�j)�̂����蔻�� BASE_NEEDLE_ATARI */
			(6),	/* �얲 A */		//	/*	6  5  6 */
			(6),	/* �얲 B */		//	/*	6  5  6 */
			(8),	/* ������ A */		//	/*	8  6  8 */
			(8),	/* ������ B */		//	/*	8  6  8 */
		   (12),	/* ���~���A */		//	/*	8  7 12 */
			(8),	/* �H�X�q */		//	/* 12  9  8 */
		   (16),	/* �`���m A */		//	/* 16  8 16 */
		   (24),	/* �`���m Q */		//	/*	8  6  8 */
		};
		h->m_Hit256R			= ((base_shot_atari[(OFFS_IS_NEEDLE)])<<8);/* �����蔻��̑傫�� */

	//	h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JI KI_SHOT_02|SP_GROUP_SHOT_SPECIAL)-shot_status;
		#if 1
	//	else
	//	if (0x01== shot_status)	/* �j */
		{
			h->type 			= (JIKI_SHOT_07|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/	/*SP_PL_FIREBALL*/
		}
	//
		if (0x00 == shot_status)	/* �j�ȊO */
		{
			if (REIMU_B+1 > (cg_game_select_player))	/* �얲? */
			{
			//	if (0x02== shot_status)	/* �얲�̉�]�V���b�g */
				if (REIMU_SHOT_TYPE_00 != shot_type)/* �U���V���b�g�ȊO�H */
				{
					h->type 			= (JIKI_SHOT_01|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/
					/* �`��p�O����] */
					h->rotationCCW1024	= (ra_nd() & (1024-1));/* �o�����̊p�x�̓����_���łȂ���(���[�U�[�݂�����)�ςȉ�ʂɂȂ� */
				}
			/*	else	{	h->type = �U���V���b�g�́A�j�Ɠ���; 	}	*/
			}
			else
			{
				h->type 			= (JIKI_SHOT_02|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/
			}
		}
	//	else
		#endif
		#if 1/*Gu(���S���W)*/
		h->cx256			= (src->cx256) + ((r_tbl[shot_type][REI00_x_offset])<<8); /*+ x_offs*/
		h->cy256			= (src->cy256) + ((r_tbl[shot_type][REI01_y_offset])<<8);/*(20)*/
		#endif
	//	h->callback_mover	= player_move_shot;
		void (*player_move_shot_table[16])(SPRITE *src) =
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
		h->callback_mover	= *player_move_shot_table[(OFFS_IS_NEEDLE)];
//		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x96);	/*	h->alpha		= 0x96; 150 �������ɂ���B */
	//
		#define CCC_STRENGTH_L1 	(PLAYERS8* 0)/* WEAPON_L0 */
		#define CCC_STRENGTH_L2 	(PLAYERS8* 1)/* WEAPON_L1 */
		#define CCC_STRENGTH_L3 	(PLAYERS8* 2)/* WEAPON_L2 */
		#define CCC_STRENGTH_L4 	(PLAYERS8* 3)/* WEAPON_L3 */
		#define CCC_STRENGTH_L5 	(PLAYERS8* 4)/* WEAPON_L4 */
		#define CCC_STRENGTH_L6 	(PLAYERS8* 5)/* WEAPON_L5 */
		#define CCC_SPEED			(PLAYERS8* 6)
		#define DDD_SPEED			(PLAYERS8* 7)
	//
		#define DDD_STRENGTH_L1 	(PLAYERS8* 8)/* WEAPON_L0 */
		#define DDD_STRENGTH_L2 	(PLAYERS8* 9)/* WEAPON_L1 */
		#define DDD_STRENGTH_L3 	(PLAYERS8*10)/* WEAPON_L2 */
		#define DDD_STRENGTH_L4 	(PLAYERS8*11)/* WEAPON_L3 */
		#define DDD_STRENGTH_L5 	(PLAYERS8*12)/* WEAPON_L4 */
		#define DDD_STRENGTH_L6 	(PLAYERS8*13)/* WEAPON_L5 */
	//	#define DDD_MAX 			(PLAYERS8*2)		/* �ő吔 */
		#define CCC_MAX 			(PLAYERS8*14)		/* �ő吔 */
		static const u8/*u8*/ ccc_tbl[CCC_MAX] =
		{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
	//		 40, 40,  40, 40,  40,128,	40, 40, 	/* WEAPON_L0  strength �V���b�g�̋��� */ /* (L1�V���b�g�́A�e�������Ȃ��̂� 40�ȉ��ɂ��Ȃ�) */
	//		 16, 24,  24, 16,  32,128,	40, 40, 	/* WEAPON_L1  strength �V���b�g�̋��� */
	//		 16, 24,  24,  8,  32,128,	40, 40, 	/* WEAPON_L2  strength �V���b�g�̋��� */
	//		 16, 24,  24,  8,  32,128,	40, 40, 	/* WEAPON_L3  strength �V���b�g�̋��� */
	//		 16, 24,  24,  8,  32,128,	40, 40, 	/* WEAPON_L4  strength �V���b�g�̋��� */
	//		 16, 24,  24,  8,  32,128,	40, 40, 	/* WEAPON_L5  strength �V���b�g�̋��� */
	//
			/*(r32�͂����������@����������̂������ŁA�{�X���ǂ�����Ă��X�y�J�O�Ƀ{�X�����ʁB����ŃQ�[�����j�]���Ă���B)*/
		//	#if 0/*(r32)(�V���b�g���x���d�l��ウ���̂�r33�ƌ݊����͂Ȃ�)�����܂Ń��� */
		//	 40, 40,  40, 40,  40,128,	40, 40, 	/* WEAPON_L0  strength �V���b�g�̋��� */ /* (L1�V���b�g�́A�e�������Ȃ��̂� 40�ȉ��ɂ��Ȃ�) */
		//	 20, 24,  24, 24,  37,129,	40, 40, 	/* WEAPON_L1  strength �V���b�g�̋��� */
		//	 17, 24,  24, 24,  36,130,	40, 40, 	/* WEAPON_L2  strength �V���b�g�̋��� */
		//	 16, 24,  24, 24,  35,132,	40, 40, 	/* WEAPON_L3  strength �V���b�g�̋��� */
		//	 15, 25,  24, 24,  34,136,	48, 44, 	/* WEAPON_L4  strength �V���b�g�̋��� */
		//	 14, 26,  25, 25,  33,144,	64, 60, 	/* WEAPON_L5  strength �V���b�g�̋��� */
		//	#endif
			/* �ȉ��͒e�����l�����Ē���*/
			#if 0/*(r33a)�e�X�g�ŁA�S�̓I�ɋ�������B */
			 20, 20,  20, 20,  40,128,	40, 40, 	/* WEAPON_L0  strength �V���b�g�̋��� */
			 22, 24,  14, 22,  37,129,	40, 40, 	/* WEAPON_L1  strength �V���b�g�̋��� */
			 15, 15,  17, 15,  36,130,	40, 40, 	/* WEAPON_L2  strength �V���b�g�̋��� */
			 12, 14,  12, 12,  35,132,	40, 40, 	/* WEAPON_L3  strength �V���b�g�̋��� */
			 13, 13,  16, 13,  34,136,	48, 44, 	/* WEAPON_L4  strength �V���b�g�̋��� */
			 10, 12,  18, 10,  33,144,	64, 60, 	/* WEAPON_L5  strength �V���b�g�̋��� */
			#endif
			#if 0/*(r33t2)������x�Ó�(�얲-�������̂�)�����A�����܂����@����������B(���@���{�X�������Ǝキ����ׂ�) */
			 10, 10,  10, 10,  40,128,	40, 40, 	/* WEAPON_L0  strength �V���b�g�̋��� */
			 11, 12,   7, 11,  37,129,	40, 40, 	/* WEAPON_L1  strength �V���b�g�̋��� */
			  7,  7,   8,  7,  36,130,	40, 40, 	/* WEAPON_L2  strength �V���b�g�̋��� */
			  6,  7,   6,  6,  35,132,	40, 40, 	/* WEAPON_L3  strength �V���b�g�̋��� */
			  6,  6,   8,  6,  34,136,	48, 44, 	/* WEAPON_L4  strength �V���b�g�̋��� */
			  5,  6,   9,  5,  33,144,	64, 60, 	/* WEAPON_L5  strength �V���b�g�̋��� */
			#endif
			#if 0/*(r33t3)�U��/�j�ޏ��Ή� */
			 10, 10,  10, 10,  40,128,	40, 40, 	/* WEAPON_L0  strength �V���b�g�̋��� */
			 11,  8,   7, 11,  37,129,	40, 40, 	/* WEAPON_L1  strength �V���b�g�̋��� */
			  5,  4,   8,  7,  36,130,	40, 40, 	/* WEAPON_L2  strength �V���b�g�̋��� */
			  4,  4,   6,  6,  35,132,	40, 40, 	/* WEAPON_L3  strength �V���b�g�̋��� */
			  3,  2,   8,  6,  34,136,	48, 44, 	/* WEAPON_L4  strength �V���b�g�̋��� */
			  2,  3,   9,  5,  33,144,	64, 60, 	/* WEAPON_L5  strength �V���b�g�̋��� */
			#endif
			#if 1/*(r33t4) (�얲 & ������) / ���@�V���b�g�シ����C�����Ȃ��ł��Ȃ��̂�x2�{�ɂ��Ă݂�B */
			 20, 20,  20, 20,  40,128,	40, 40, 	/* WEAPON_L0  strength �V���b�g�̋��� */
			 22, 16,  14, 22,  37,129,	40, 40, 	/* WEAPON_L1  strength �V���b�g�̋��� */
			 10,  8,  16, 14,  36,130,	40, 40, 	/* WEAPON_L2  strength �V���b�g�̋��� */
			  8,  8,  12, 12,  35,132,	40, 40, 	/* WEAPON_L3  strength �V���b�g�̋��� */
			  6,  4,  16, 12,  34,136,	48, 44, 	/* WEAPON_L4  strength �V���b�g�̋��� */
			  4,  6,  18, 10,  33,144,	64, 60, 	/* WEAPON_L5  strength �V���b�g�̋��� */
			#endif

#if 0
-----------(r33t3)[�j�ޏ�(��)�͐j�ƃV���b�g�ł����ƍ���t����]
�얲 A(��)�V���b�g		  �얲 A(��)�j(�U��)				  / �얲 B(��)�V���b�g		  �얲 B(��)�j						  /
10 x 1 == 10 == 1.00[sum] -- x 0 ------------------ 1.00[ave] / 10 x 1 == 10 == 1.00[sum] -- x 0 ------------------- 1.00[ave] /
11 x 1 == 11 == 1.10[sum] 11 x 1 == 11 == 1.10[sum] 1.10[ave] /  8 x 1 ==  8 == 0.80[sum] 16 x 1 == 16 == 1.60[sum] 1.20[ave] /
 5 x 2 == 10 == 1.00[sum]  9 x 2 == 18 == 1.80[sum] 1.40[ave] /  4 x 2 ==  8 == 0.80[sum] 10 x 2 == 20 == 2.00[sum] 1.40[ave] /
 4 x 3 == 12 == 1.20[sum]  9 x 3 == 27 == 2.70[sum] 1.95[ave] /  4 x 3 == 12 == 1.20[sum] 10 x 3 == 30 == 3.00[sum] 2.10[ave] /
 3 x 3 ==  9 == 0.90[sum]  7 x 4 == 28 == 2.80[sum] 1.85[ave] /  2 x 4 ==  8 == 0.80[sum] 10 x 4 == 40 == 4.00[sum] 2.40[ave] /
 2 x 4 ==  8 == 0.80[sum]  8 x 4 == 32 == 3.20[sum] 2.00[ave] /  3 x 5 == 15 == 1.50[sum] 12 x 4 == 48 == 4.80[sum] 3.15[ave] /
������ A(��)�V���b�g	  ������ A(��)�j					  / ������ B(��)�V���b�g	  ������ B(��)�j					  /
10 x 1 == 10 == 1.00[sum] -- x 0 ------------------ 1.00[ave] / 10 x 1 == 10 == 1.00[sum] -- x 0 ------------------ 1.00[ave] /
 7 x 2 == 14 == 1.40[sum] 10 x 1 == 10 == 1.00[sum] 1.20[ave] /    x 2 ==	 == 1.00[sum] 20 x 1 == 20 == 1.00[sum] 1.00[ave] /
 8 x 2 == 16 == 1.60[sum]  7 x 2 == 14 == 1.40[sum] 1.50[ave] /    x 3 ==	 ==  . 0[sum]	 x 2 ==    ==  . 0[sum] 1.00[ave] /
 6 x 3 == 18 == 1.80[sum]  6 x 3 == 18 == 1.80[sum] 1.80[ave] /    x 4 ==	 ==  . 0[sum]	 x 3 ==    ==  . 0[sum] 1.00[ave] /
 8 x 3 == 24 == 2.40[sum]  6 x 4 == 24 == 2.40[sum] 2.40[ave] /    x 5 ==	 ==  . 0[sum]	 x 4 ==    ==  . 0[sum] 1.00[ave] /
 9 x 3 == 27 == 2.70[sum]  7 x 4 == 28 == 2.80[sum] 2.75[ave] /    x 5 ==	 ==  . 0[sum]	 x 4 ==    ==  . 0[sum] 1.00[ave] /
-----------(r33t2t3�`��)[���Ɠ���������(t3�`����)���������Ă݂�]
�얲 A(��)�V���b�g		  �얲 A(��)�j						  / �얲 B(��)�V���b�g		  �얲 B(��)�j						  /
10 x 1 == 10 == 1.00[sum] -- x 0 ------------------ 1.00[ave] / 10 x 1 == 10 == 1.00[sum] -- x 0 ------------------ 1.00[ave] /
11 x 1 == 11 == 1.10[sum] 10 x 1 == 10 == 1.00[sum] 1.05[ave] / 12 x 1 == 12 == 1.20[sum] 10 x 1 == 10 == 1.00[sum] 1.10[ave] /
 7 x 2 == 14 == 1.40[sum]  6 x 2 == 12 == 1.20[sum] 1.30[ave] /  7 x 2 == 14 == 1.40[sum]  7 x 2 == 14 == 1.40[sum] 1.40[ave] /
 6 x 3 == 18 == 1.80[sum]  5 x 3 == 15 == 1.50[sum] 1.65[ave] /  7 x 3 == 21 == 2.10[sum]  7 x 3 == 21 == 2.10[sum] 2.10[ave] /
 6 x 3 == 18 == 1.80[sum]  4 x 4 == 16 == 1.60[sum] 1.70[ave] /  6 x 4 == 24 == 2.40[sum]  6 x 4 == 24 == 2.40[sum] 2.40[ave] /
 5 x 4 == 20 == 2.00[sum]  5 x 4 == 20 == 2.00[sum] 2.00[ave] /  6 x 5 == 30 == 3.00[sum]  8 x 4 == 32 == 3.20[sum] 3.10[ave] /
������ A(��)�V���b�g	  ������ A(��)�j					  / ������ B(��)�V���b�g	  ������ B(��)�j					  /
10 x 1 == 10 == 1.00[sum] -- x 0 ------------------ 1.00[ave] / 10 x 1 == 10 == 1.00[sum] -- x 0 ------------------ 1.00[ave] /
 7 x 2 == 14 == 1.40[sum] 10 x 1 == 10 == 1.00[sum] 1.20[ave] /    x 2 ==	 == 1.00[sum] 20 x 1 == 20 == 1.00[sum] 1.00[ave] /
 8 x 2 == 16 == 1.60[sum]  7 x 2 == 14 == 1.40[sum] 1.50[ave] /    x 3 ==	 ==  . 0[sum]	 x 2 ==    ==  . 0[sum] 1.00[ave] /
 6 x 3 == 18 == 1.80[sum]  6 x 3 == 18 == 1.80[sum] 1.80[ave] /    x 4 ==	 ==  . 0[sum]	 x 3 ==    ==  . 0[sum] 1.00[ave] /
 8 x 3 == 24 == 2.40[sum]  6 x 4 == 24 == 2.40[sum] 2.40[ave] /    x 5 ==	 ==  . 0[sum]	 x 4 ==    ==  . 0[sum] 1.00[ave] /
 9 x 3 == 27 == 2.70[sum]  7 x 4 == 28 == 2.80[sum] 2.75[ave] /    x 5 ==	 ==  . 0[sum]	 x 4 ==    ==  . 0[sum] 1.00[ave] /
-----------(r33t2)
�얲 A(��)					�얲 B(��)					/
10 x 1 == 10 == 1.00[total] 10 x 1 == 10 == 1.00[total] /
11 x 1 == 11 == 1.10[total] 12 x 1 == 12 == 1.20[total] /
 7 x 2 == 14 == 1.40[total]  7 x 2 == 14 == 1.40[total] /
 6 x 3 == 18 == 1.80[total]  7 x 3 == 21 == 2.10[total] /
 6 x 3 == 18 == 1.80[total]  6 x 4 == 24 == 2.40[total] /
 5 x 4 == 20 == 2.00[total]  6 x 5 == 30 == 3.00[total] /
������ A(��)				������ B(��)				/
10 x 1 == 10 == 1.00[total] 10 x 1 == 10 == 1.00[total] /
 7 x 2 == 14 == 1.40[total]    x 2 ==	 == 1.00[total] /
 8 x 2 == 16 == 1.60[total]    x 3 ==	 ==  . 0[total] /
 6 x 3 == 18 == 1.80[total]    x 4 ==	 ==  . 0[total] /
 8 x 3 == 24 == 2.40[total]    x 5 ==	 ==  . 0[total] /
 9 x 3 == 27 == 2.70[total]    x 5 ==	 ==  . 0[total] /
-----------(r33a)
�얲 A(��)					�얲 B(��)					/
20 x 1 == 20 == 1.00[total] 20 x 1 == 20 == 1.00[total] /
22 x 1 == 22 == 1.10[total] 24 x 1 == 24 == 1.20[total] /
15 x 2 == 30 == 1.50[total] 15 x 2 == 30 == 1.50[total] /
12 x 3 == 36 == 1.80[total] 14 x 3 == 42 == 2.10[total] /
13 x 3 == 39 == 1.95[total] 13 x 4 == 52 == 2.60[total] /
10 x 4 == 40 == 2.00[total] 12 x 5 == 60 == 3.00[total] /
������ A(��)				������ B(��)				/
20 x 1 == 20 == 1.00[total] 20 x 1 == 20 == 1.00[total] /
14 x 2 == 28 == 1.40[total]    x 2 ==	 == 1.00[total] /
17 x 2 == 34 == 1.70[total]    x 3 ==	 ==  . 0[total] /
12 x 3 == 36 == 1.80[total]    x 4 ==	 ==  . 0[total] /
16 x 3 == 48 == 2.40[total]    x 5 ==	 ==  . 0[total] /
18 x 3 == 54 == 2.70[total]    x 5 ==	 ==  . 0[total] /
#endif

			#if 0/*(r32)*/
			 16, 14,  10, 12,	9,	4,	 8,  9, 	/* speed	�V���b�g�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */
			 18, 18,  10, 10,	3,	4,	 3,  4, 	/* speed	�j�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */
			#endif
			#if 1/*(r33)*/
	// �Q�l http://hossy.info/game/toho/k_chara.php
	//				 �얲 (��) - �U���e �U���e�͒ʏ�e�Ɠ������x
	//				 �얲 (��) - �j�[�h�� �j�[�h���͒ʏ�e��2�{��
	// 11==16* 0.7 ������ (��) - �~�T�C�� �ʏ�e��70���B
	// ?? ??	   ������ (��) - ���[�U�[ �X�s�[�h����s�\�B�Ƃ������S�Ăɂ����đ��荢��B
		//	 a	 a	  a    ?
		//	 a	 2a  0.7a  ?
			  9,  9,   9,  9,	9,	4,	 8,  9, 	/* speed	�V���b�g�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */
			  9, 18,   6,  9,	3,	4,	 3,  4, 	/* speed	�j�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */
		//	 ��  ��   ��  ��   ��  �H	�`	�H
			#endif
	//
			/*(r32�͂����������@����������̂������ŁA�{�X���ǂ�����Ă��X�y�J�O�Ƀ{�X�����ʁB����ŃQ�[�����j�]���Ă���B)*/
		//	#if 0/*(r32)(�V���b�g���x���d�l��ウ���̂�r33�ƌ݊����͂Ȃ�)�����܂Ń��� */
		//	 16, 32,  32,160,  48, 64,	48, 48, 	/* WEAPON_L0  strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
		//	 16, 32,  32,162,  48, 64,	48, 48, 	/* WEAPON_L1  strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
		//	 16, 33,  32,163,  49, 64,	48, 48, 	/* WEAPON_L2  strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
		//	 16, 34,  32,164,  49, 65,	48, 48, 	/* WEAPON_L3  strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
		//	 15, 35,  32,165,  50, 65,	56, 52, 	/* WEAPON_L4  strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
		//	 14, 36,  32,166,  52, 66,	80, 78, 	/* WEAPON_L5  strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
		//	#endif
	/* (r32)�ǂ��l���Ă��j��������B */
			/* �ȉ��͒e�����l�����Ē��� */
			#if 0/*(r33a)�e�X�g�ŁA�S�̓I�ɋ�������B */
			 20, 20,  20, 20,  48, 64,	48, 48, 	/* WEAPON_L0(P000-P008)  OPT0 strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
			 20, 20,  20,108,  48, 64,	48, 48, 	/* WEAPON_L1(P008-P015)  OPT1 strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
			 12, 15,  15,128,  49, 64,	48, 48, 	/* WEAPON_L2(P016-P031)  OPT2 strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
			 10, 14,  12,144,  49, 65,	48, 48, 	/* WEAPON_L3(P032-P063)  OPT3 strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
			  9, 13,  12,155,  50, 65,	56, 52, 	/* WEAPON_L4(P064-P127)  OPT4 strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
			 10, 16,  14,166,  52, 66,	80, 78, 	/* WEAPON_L5(P128)		 OPT4 strength �j�̋��� */	/*REMILIA, CIRNO,  48 40 ��������*/
			#endif
			#if 0/*(r33t2)������x�Ó�(�얲-�������̂�)�����A�����܂����@����������B(���@���{�X�������Ǝキ����ׂ�) */
			 10, 10,  10, 20,  48, 64,	48, 48, 	/* WEAPON_L0(P000-P008)  OPT0 strength �j�̋��� */
			 10, 10,  10,108,  48, 64,	48, 48, 	/* WEAPON_L1(P008-P015)  OPT1 strength �j�̋��� */
			  6,  7,   7,128,  49, 64,	48, 48, 	/* WEAPON_L2(P016-P031)  OPT2 strength �j�̋��� */
			  5,  7,   6,144,  49, 65,	48, 48, 	/* WEAPON_L3(P032-P063)  OPT3 strength �j�̋��� */
			  4,  6,   6,155,  50, 65,	56, 52, 	/* WEAPON_L4(P064-P127)  OPT4 strength �j�̋��� */
			  5,  8,   7,166,  52, 66,	80, 78, 	/* WEAPON_L5(P128)		 OPT4 strength �j�̋��� */
			#endif
			#if 0/*(r33t3)�U��/�j�ޏ��Ή� */
			 11, 16,  10, 20,  48, 64,	48, 48, 	/* WEAPON_L0(P000-P008)  OPT0 strength �j�̋��� */
			 11, 16,  10,108,  48, 64,	48, 48, 	/* WEAPON_L1(P008-P015)  OPT1 strength �j�̋��� */
			  9, 10,   7,128,  49, 64,	48, 48, 	/* WEAPON_L2(P016-P031)  OPT2 strength �j�̋��� */
			  9, 10,   6,144,  49, 65,	48, 48, 	/* WEAPON_L3(P032-P063)  OPT3 strength �j�̋��� */
			  7, 10,   6,155,  50, 65,	56, 52, 	/* WEAPON_L4(P064-P127)  OPT4 strength �j�̋��� */
			  8, 12,   7,166,  52, 66,	80, 78, 	/* WEAPON_L5(P128)		 OPT4 strength �j�̋��� */
			#endif
			#if 1/*(r33t4)�U��/�j�ޏ��Ή��B�U���ޏ�(�ƐX�}��)���܂�Ɏシ����̂ŁA��������B
				�U���ޏ��Ɋւ��Ă̓I�v�V�����ǉ��Ԋu�̑��x��(r33�ȍ~��)�x������悤�ɂȂ����̂ŁA
				�����̈З͂𑝂₵�Ă��������ċ����Ȃ�Ȃ�����A�����̈З͂𑝂₵�����Ă����Ȃ��B
			*/
			 11*2, 16,  10*2, 20,  48, 64,	48, 48, 	/* WEAPON_L0(P000-P008)  OPT0 strength �j�̋��� */
			 11*2, 16,  10*2,108,  48, 64,	48, 48, 	/* WEAPON_L1(P008-P015)  OPT1 strength �j�̋��� */
			  9*2, 10,   7*2,128,  49, 64,	48, 48, 	/* WEAPON_L2(P016-P031)  OPT2 strength �j�̋��� */
			  9*2, 10,   6*2,144,  49, 65,	48, 48, 	/* WEAPON_L3(P032-P063)  OPT3 strength �j�̋��� */
			  7*2, 10,   6*2,155,  50, 65,	56, 52, 	/* WEAPON_L4(P064-P127)  OPT4 strength �j�̋��� */
			  8*2, 12,   7*2,166,  52, 66,	80, 78, 	/* WEAPON_L5(P128)		 OPT4 strength �j�̋��� */
			#endif
#if 0
-----------(r33t3)[�j�ޏ�(��)�͐j�ƃV���b�g�ł����ƍ���t����]
�V���b�g�Ɛj�̈З̓o�����X���d�v�Ȃ̂ŁA�����ɏ����Ɖ�����Ȃ��̂ŁA��Q�ƁB
-----------(r33t2)
�얲 A(��)					�얲 B(��)					/
10 x 1 == 10 == 1.00[total] 10 x 1 == 10 == 1.00[total] /
 6 x 2 == 12 == 1.20[total]  7 x 2 == 14 == 1.40[total] /
 5 x 3 == 15 == 1.50[total]  7 x 3 == 21 == 2.10[total] /
 4 x 4 == 16 == 1.60[total]  6 x 4 == 24 == 2.40[total] /
 5 x 4 == 20 == 2.00[total]  8 x 4 == 32 == 3.20[total] /
������ A(��)				������ B(��)				/
10 x 1 == 10 == 1.00[total] 20 x 1 == 20 == 1.00[total] /
 7 x 2 == 14 == 1.40[total]    x 2 ==	 ==  . 0[total] /
 6 x 3 == 18 == 1.80[total]    x 3 ==	 ==  . 0[total] /
 6 x 4 == 24 == 2.40[total]    x 4 ==	 ==  . 0[total] /
 7 x 4 == 28 == 2.80[total]    x 4 ==	 ==  . 0[total] /
-----------(r33a)
�얲 A(��)					�얲 B(��)					/
20 x 1 == 20 == 1.00[total] 20 x 1 == 20 == 1.00[total] /
12 x 2 == 24 == 1.20[total] 15 x 2 == 30 == 1.50[total] /
10 x 3 == 30 == 1.50[total] 14 x 3 == 42 == 2.10[total] /
 9 x 4 == 36 == 1.80[total] 13 x 4 == 52 == 2.60[total] /
10 x 4 == 40 == 2.00[total] 16 x 4 == 64 == 3.20[total] /
������ A(��)				������ B(��)				/
20 x 1 == 20 == 1.00[total] 20 x 1 == 20 == 1.00[total] /
15 x 2 == 30 == 1.50[total]    x 2 ==	 ==  . 0[total] /
12 x 3 == 36 == 1.80[total]    x 3 ==	 ==  . 0[total] /
12 x 4 == 48 == 2.40[total]    x 4 ==	 ==  . 0[total] /
14 x 4 == 56 == 2.80[total]    x 4 ==	 ==  . 0[total] /
	// �Q�l http://hossy.info/game/toho/k_chara.php
//	����: �����̑���f�[�^�[�́u�V���b�g(�D)�v�Ɓu�T�u�V���b�g(�j)�v�𑫂����З͂Ȃ̂ŁA
// �u�V���b�g(�D)�v�A�u�T�u�V���b�g(�j)�v���ꂼ��P�̂̋�̓I�З͂ɂ��Ă͉�������Ȃ��B
// (�P�̂̋�̓I�З͂ɂ��ẮA�Q�[�����ł͊ȒP�ɂ͑���ł��Ȃ��B(���荢��))
	//				 �얲 (��) - �U���e �U���e�͒ʏ�e�Ɠ������x
	//				 �얲 (��) - �j�[�h�� �j�[�h���͒ʏ�e��2�{��
	   P |���C��|�T�u|	WAY|�_���[�W|�ł����ݓ_||  P |���C��|�T�u|OPT|�_���[�W|�ł����ݓ_
 0x8=  0 |	 6	|  0 |1-0-0| �~ 1.0 |	540    ||  0 |	 6	|  0 |0-0| �~ 1.0 |   540
 1x8=  8 |	 6	|  2 |	2-1| �~ 1.1 |	590    ||  8 |	 6	|  4 |	 | �~ 1.2 |   620
 2x8= 16 |	12	|  2 |	2-1| �~ 1.5 |	770    || 16 |	12	|  6 |	 | �~ 1.5 |   840
 4x8= 32 |	18	|  2 |3-   | �~ 1.8 |	890    || 32 |	12	|  8 |	 | �~ 1.6 |   860
 6x8= 48 |	18	|  4 |3-   | �~ 1.9 |	920    || 48 |	18	|  8 |	 | �~ 1.8 |   910
 8x8= 64 |	18	|  6 |3-   | �~ 1.9 |	950    || 64 |	18	| 20 |	 | �~ 2.1 |  1050
10x8= 80 |	18	|  6 |3-   | �~ 1.9 |	950    || 80 |	18	| 32 |	 | �~ 2.1 |  1240
12x8= 96 |	18	| 12 |3-   | �~ 2.0 |  1020    || 96 |	18	| 38 |	 | �~ 2.8 |  1520
16x8=MAX |	24	| 16 |4-   | �~ 2.0 |  1060    ||MAX |	30	| 52 |	 | �~ 3.1 |  1720

	// 11==16* 0.7 ������ (��) - �~�T�C�� �ʏ�e��70���B
	// ?? ??	   ������ (��) - ���[�U�[ �X�s�[�h����s�\�B�Ƃ������S�Ăɂ����đ��荢��B
  P |���C��|�T�u|OPT|�_���[�W|�ł����ݓ_||	 P |���C��|�T�u|OPT|�_���[�W|�ł����ݓ_
  0 |  6   |  0 |0-0| �~ 1.0 |	 540	||	 0 |   6  |  0 |0-0| �~ 1.0 |  540
  8 |  6   |  2 |	| �~ 1.4 |	 500	||	 8 |   6  |  1 |   | �~ -.- |	 ?
 16 |  6   |  4 |	| �~ 1.7 |	 480	||	16 |   6  |  1 |   | �~ -.- |	 ?
 32 |  6   |  4 |	| �~ 1.7 |	 510	||	32 |  12  |  1 |   | �~ -.- |	 ?
 48 |  6   |  4 |	| �~ 1.8 |	 520	||	48 |  12  |  2 |   | �~ -.- |	 ?
 64 | 12   | 10 |	| �~ 2.2 |	 830	||	64 |  12  |  2 |   | �~ -.- |	 ?
 80 | 12   | 12 |	| �~ 2.3 |	 910	||	80 |  18  |  2 |   | �~ -.- |	 ?
 96 | 18   | 16 |	| �~ 2.6 |	1150	||	96 |  18  |  3 |   | �~ -.- |	 ?
MAX | 18   | 18 |	| �~ 2.8 |	1300	|| MAX |  30  |  3 |   | �~ -.- |	 ?

#endif
	/* [***20090822 REIMU 24 <- 16 (�Ƃ肠�����j�シ����̂�) */
	/* r32 �얲A�̗U���V���b�g�B��������C������̂ŁA32 �� 16 �ɂ��Ă݂�B*/
		};
	//		 2, 3,	3, 2,  4,16, 5, 5,	/* strength �V���b�g�̋��� */
	//		16,14, 10,12,  9, 4, 8, 8,	/* speed	�V���b�g�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */
		//};
	//		 2, 2, 3, 3, 4, 3, 3, 3,	/* strength �V���b�g�̋��� */
	//		15,15,12,12,18,12,12,12,	/* speed	�V���b�g�̑���	*/
	/* �ɒ[�ɋ@�̂̐��\���΂�̂͗ǂ��Ȃ��B(�����đ����̂͂��邢) */
		//static const u8 ddd_tbl[DDD_MAX] =
		//{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
	//		 3, 3, 4, 4, 6, 8, 6, 6,	/* strength �j�̋��� */ 	/* REMILIA, CIRNO,	6 5 �������� */
	/* �얲(�������Ă݂� [***20090930 ) */
	//		 4, 4, 4,10, 6, 8, 6, 6,	/* strength �j�̋��� */ 	/* REMILIA, CIRNO,	6 5 �������� */
	//		 4, 4, 4,25, 6, 8, 6, 6,	/* strength �j�̋��� */ 	/* REMILIA, CIRNO,	6 5 �������� */
	//		18,18,10,15, 3, 4, 3, 3,	/* speed	�j�̑��� (�x��������ʏ�̒e����������̂ŋ����B�������d���Ȃ�) */


		h->base_weapon_strength = ccc_tbl[CCC_STRENGTH_L1+(cg_jiki_weapon_level_offset)+(is_needle<<(4+2))];/* 64 == 8[players]*8[line] */
	//	h->base_weapon_strength = ddd_tbl[DDD_STRENGTH_L1+(cg_jiki_weapon_level_offset)];

	//
	//	int ok;
	//	ok=0;
		{
			#if 0
		//	if (1==r_tbl[shot_type][REI03_yudo])	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
			if (0 != r_tbl[shot_type][REI03_yudo])/* �U���V���b�g���H */
			#else
		//	if ( (REIMU_SHOT_TYPE_01) > shot_type)/* �U���V���b�g���H */
			if ( (REIMU_SHOT_TYPE_00) == shot_type)/* �U���V���b�g���H */
			#endif
			{
				SPRITE *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
				SPRITE *target;
				target = search_teki_from_obj();
				if (target != zzz_player)/* ���������� */
				{
					h->PL_SHOT_DATA_target_x256  = target->cx256;/* �U���ڕW���W���o���Ƃ� */
				//	h->PL_SHOT_DATA_target_y256  = target->cy256;/* �U���ڕW���W���o���Ƃ� */
					//
				//	if ( (REIMU_SHOT_TYPE_00) == shot_type)/* �U���V���b�g���H */
					{
						int int_angle1024;
					//	int_angle1024	= at an_1024(target->cy256-zzz_player->cy256, target->cx256-zzz_player->cx256);
						int_angle1024	= atan_65536(target->cy256-zzz_player->cy256, target->cx256-zzz_player->cx256);
						int_angle1024 >>= (6);
					//	if (511 < int_angle1024 )/* ���180-360�x[/360�x]�̗̈�̂ݒǔ����� */
						if ( /*(384)*/(256*3) < (( int_angle1024+128+256)&(1024-1)) )/* ���225-315�x[/360�x]�̗̈�̂ݒǔ����� */	/* CCW�̏ꍇ */
						{
							h->vx256	 = sin1024((int_angle1024))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+(cg_game_select_player)]*/);/*fps_factor*/ /* CCW�̏ꍇ */
							h->vy256	 = cos1024((int_angle1024))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+(cg_game_select_player)]*/);/*fps_factor*/
							#if 1
							/* �`��p�O����] */
							if (MARISA_A==(cg_game_select_player))/* (������������)�}�W�b�N�~�T�C���͌X���Ȃ��B */
							{	int_angle1024 = (0);	}
							else/* (�얲�아��)�z�[�~���O�A�~�����b�g�͖͕핗�ł͌X���鎖�ɂ���B */
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
			h->vx256 = ((/*int_vx*/r_tbl[shot_type][REI02_vx256]))*/*p->speed*/(ccc_tbl[CCC_SPEED+OFFS_IS_NEEDLE]);/*fps_factor*/
			h->vy256 = -(((int)ccc_tbl[CCC_SPEED+OFFS_IS_NEEDLE])<<8);/*fps_factor*/ /*p->speed*/
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------
	�v���C���[�e(�V���b�g�̂�)����
---------------------------------------------------------*/

/* �얲A (�U���ޏ�) �U���V���b�g */

static void shot_regist_re_yudou(SPRITE *s) /* �얲A ������A */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_00);/* �U���V���b�g */
}

static void shot_regist_yuyuko(SPRITE *s)	/* �H�X�q */
{
	player_register_shot_object(s, NEEDLE_ANGLE_271);
}

/* �얲B (�j�ޏ�) / ������A/B / ���~���A �� �`���m */

static void shot_regist_re_hari(SPRITE *s)	/* ������A/B */ 	/* ��(���݃_�~�[�p) */
{
	player_register_shot_object(s, NEEDLE_ANGLE_270);
}

static void shot_regist_ci_gggg(SPRITE *s)	/* ���~���A �� �`���m */
{
	player_register_shot_object(s, NEEDLE_ANGLE_277 );
	player_register_shot_object(s, NEEDLE_ANGLE_263 );
}
static void shot_regist_ma_lazer(SPRITE *s) /* ������B ���[�U�[ */
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
		if ((43)>(count128 ))	/* ���͂����A1/3���Ԃ������ʂ��Ȃ��B */
		#endif
		{
			shot_regist_re_hari(s);
		}
	}
}

/*---------------------------------------------------------
	�v���C���[�e(�V���b�g�̂�)����
---------------------------------------------------------*/

/* �얲A/B ��]�V���b�g */	/* �ʏ�V���b�g */
static void shot_regist_1(SPRITE *s)	/* �얲 */	/* �얲 �� ������ �� �`���m �� ���~���A */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* �ʏ�V���b�g */
}
static void shot_regist_2(SPRITE *s)	/* �얲 */	/* �얲 �� ������ �� �`���m �� ���~���A */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* �ʏ�V���b�g */
}
static void shot_regist_3(SPRITE *s)	/* �얲 */	/* ������ �� �`���m */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* �ʏ�V���b�g */
}
static void shot_regist_4(SPRITE *s)	/* �얲 */	/* ������ */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* �ʏ�V���b�g */
}

static void shot_regist_5(SPRITE *s)	/* �얲B */ /* ������ */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_06); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* �ʏ�V���b�g */
	player_register_shot_object(s, REIMU_SHOT_TYPE_07); 	/* �ʏ�V���b�g */
}

/* �얲B (�j�ޏ�) */

/* ������A(�~�T�C��������) */
/* ������B(���[�U�[������) */


static void shot_regist_chou(SPRITE *s) 		/* �H�X�q */
{
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_08);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_09);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_0a);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_0b);
}


/*---------------------------------------------------------
	�I�v�V�����̍U���𐬒��i�K�ɂ���Ă�����B
---------------------------------------------------------*/
/*
 WEAPON_L0(P000-P008)
 WEAPON_L1(P008-P015)
 WEAPON_L2(P016-P031)
 WEAPON_L3(P032-P063)
 WEAPON_L4(P064-P127)
 WEAPON_L5(P128)
*/
/*static*/static  void shot_regist_remilia_cirno(SPRITE *src)
{	/* �U���x�� */
	if (src->PL_OPTION_DATA_opt_anime_add_id < OPTION_C3)
	{
	//case OPTION_C1: OPTION_C2:
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
	//case OPTION_C3: OPTION_C4:
		shot_regist_re_hari(src);		//	src->PL_OPTION_DATA_opt_shot_interval=(5+3);
	}
}

/*---------------------------------------------------------
	�I�v�V���� �A�j���[�V����
---------------------------------------------------------*/
//
static void player_animate_option(SPRITE *src)
{
	#define BASE_OPT_ANIM_TYPE			(PLAYERS8*0)
	#define BASE_OPT_ANIM_SPEED 		(PLAYERS8*1)
	#define BASE_OPT_ANIM_MAX			(PLAYERS8*2)
	/*static*/static const u8 anime_fix_status[BASE_OPT_ANIM_MAX] =
	{/* REIMU(A/B) MARISA(A/B)	REMILIA YUYUKO CIRNO(A/Q) */
		   0,	0,	 0,   0,	0,	 1,   1,   1,	 /* BASE_OPT_ANIM_TYPE	   0==��], 1==�p�^�[�� */
		   4,	4,	 6,   8,	4,	12,   3,   3,	 /* BASE_OPT_ANIM_SPEED    0�Ȃ��]���x / 1�Ȃ�p�^�[�����x */
	};
	if (0==anime_fix_status[BASE_OPT_ANIM_TYPE+(cg_game_select_player)])
	{	/* �I�v�V��������]����ꍇ */
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
		src->rotationCCW1024 += (anime_fix_status[BASE_OPT_ANIM_SPEED+(cg_game_select_player)]);	/* �`��p�O����]���x */
		mask1024(src->rotationCCW1024);
	}
	else
	{	/* �I�v�V�������A�j���[�V��������ꍇ */
		src->PL_OPTION_DATA_anime_wait--;
		if (src->PL_OPTION_DATA_anime_wait<1)
		{
			src->PL_OPTION_DATA_anime_wait = (anime_fix_status[BASE_OPT_ANIM_SPEED+(cg_game_select_player)]);
			#if 1
			src->type++;
			src->type &= (0xfffffff7); /* ��ʃr�b�g�ɉe����^���Ȃ��ŃA�j���[�V���� */
			#endif
		}
	}
}


/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�����
---------------------------------------------------------*/
/*static*/extern void player_control_option(SPRITE *src);	/* �S�� */
/*static*/global void player_move_option(SPRITE *src)	/* �S�� */
{
	/* �I�v�V��������\���̏ꍇ�A�������Ȃ��B */
	if (0==(src->flags & ( SP_FLAG_OPTION_VISIBLE)))	{	return; 	}
//
	/* �A�j���[�V���� */
	player_animate_option(src);
//
	if (cg.state_flag & STATE_FLAG_15_KEY_SHOT)
	{
		{
			{
				src->PL_OPTION_DATA_opt_shot_interval--;
				if (src->PL_OPTION_DATA_opt_shot_interval<0)
				{
	static const u8 player_bbb_status[(PLAYERS8*6)/*BASE_MAX*/] =
{/* REIMU(A/B) MARISA(A/B)	REMILIA YUYUKO CIRNO(A/Q) */
//	/* BASE_OPT_SHOT_INTERVAL �I�v�V�����V���b�g�̍X�V�Ԋu / option shot interval. */
//	  14,  13,	11,   8,	8,	31,   8,   9,	 /* WEAPON_L0 */
//	  12,  11,	10,   8,	8,	28,   8,   9,	 /* WEAPON_L1 */
//	  10,	9,	 9,   8,	8,	25,   8,   9,	 /* WEAPON_L2 */
//	   8,	7,	 8,   8,	8,	22,   8,   9,	 /* WEAPON_L3 */
//	   6,	5,	 7,   8,	8,	19,   8,   9,	 /* WEAPON_L4 */
//	   4,	3,	 6,   8,	8,	16,   8,   9,	 /* WEAPON_L5 */
//(r32)
//	  14,  13,	11,   4,	8,	31,   8,   9,	 /* WEAPON_L0 */
//	  12,  11,	10,   4,	8,	28,   8,   9,	 /* WEAPON_L1 */
//	  10,	9,	 9,   4,	8,	25,   8,   9,	 /* WEAPON_L2 */
//	   8,	7,	 8,   4,	8,	22,   8,   9,	 /* WEAPON_L3 */
//	   6,	5,	 7,   4,	8,	19,   7,   8,	 /* WEAPON_L4 */
//	   4,	3,	 6,   3,	8,	16,   5,   6,	 /* WEAPON_L5 */
//(r33)�U���ޏ��̃I�v�V�����͎ア�ƑS�R���C���Ȃ��B
	  99,  13,	11,   4,	8,	31,   8,   9,	 /* WEAPON_L0 */
	  77,  11,	10,   4,	8,	28,   8,   9,	 /* WEAPON_L1 */
	  55,	9,	 9,   4,	8,	25,   8,   9,	 /* WEAPON_L2 */
	  33,	7,	 8,   4,	8,	22,   8,   9,	 /* WEAPON_L3 */
	  10,	5,	 7,   4,	8,	19,   7,   8,	 /* WEAPON_L4 */
	   4,	3,	 6,   3,	8,	16,   5,   6,	 /* WEAPON_L5 */
};
					src->PL_OPTION_DATA_opt_shot_interval = player_bbb_status[(cg_jiki_weapon_level_offset)];/* (�얲 ������	���H�X�q)�I�v�V�����̃V���b�g�Ԋu */
					#if 1//1975613
					static /*const*/ void (*ggg[PLAYERS8])(SPRITE *sss) =
					{
						/*REIMU_A*/ 	shot_regist_re_yudou,			/* �얲 A(�U���ԎD) */
						/*REIMU_B*/ 	shot_regist_re_hari,			/* �얲 B(�j) */	/*re_bbbb*/
						/*MARISA_A*/	shot_regist_re_yudou,			/* ������ A(�U���X�e) */
						/*MARISA_B*/	shot_regist_ma_lazer,			/* ������ B(���܂��ꃌ�[�U�[) */
						/*REMILIA*/ 	shot_regist_remilia_cirno,		/* ���~���A �� �`���m */
						/*YUYUKO*/		shot_regist_yuyuko, 			/* �H�X�q(��) */
						/*CIRNO_A*/ 	shot_regist_remilia_cirno,		/* ���~���A �� �`���m */
						/*CIRNO_Q*/ 	shot_regist_remilia_cirno,		/* ���~���A �� �`���m */
					};
					(ggg[(cg_game_select_player)])(src);
					#endif
				}
			}
		}
	}
	player_control_option(src);
}


/*---------------------------------------------------------
	���@�̃V���b�g�{�^���������ςȂ��Ŏ����V���b�g�����ꍇ
---------------------------------------------------------*/

global void register_main_shot(SPRITE *s1)
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
		{static int weapon_interval=0;
			/*cg.*/weapon_interval--/*=fps_fa_ctor*/;
			if (/*cg.*/weapon_interval < 1 )
			{	/* ���@�̃V���b�g�{�^���������ςȂ��Ŏ����V���b�g�����ꍇ�́A���Ԋu[�t���[���P��]�̃e�[�u���B */
				static const u8 jiki_auto_shot_interval_table[(WEAPON_L_MAX)*(PLAYERS8)] =
				{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */    /* ���~���A��������̂Œ���(+5) */
						 5, 5,	6, 6,  7+5,  9+8,  9,  9,	/* WEAPON_L0(P000-P008) */
						 5, 5,	6, 6,  6+5,  8+8,  9,  9,	/* WEAPON_L1(P008-P015) */
						 5, 5,	5, 5,  7+5,  7+8,  9,  9,	/* WEAPON_L2(P016-P031) */
						 5, 5,	5, 5,  6+5,  6+8,  9,  9,	/* WEAPON_L3(P032-P063) */
						 5, 5,	5, 5,  5+5,  5+8,  9,  9,	/* WEAPON_L4(P064-P127) */
						 5, 5,	5, 5,  5+5,  5+8,  9,  9,	/* WEAPON_L5(P128)		*/
				};
			//	/*cg.*/weapon_interval = jiki_auto_shot_interval_table[(weapon_List<<3)+(cg_game_select_player)];
				/*cg.*/weapon_interval = jiki_auto_shot_interval_table[(cg_jiki_weapon_level_offset)];
				voice_play(VOICE00_SHOT, TRACK00_BULLETS);
				static /*const*/ void (*bbb[(WEAPON_L_MAX)*(PLAYERS8)])(SPRITE *sss) =
				{
	/*REIMU_A*/ 	/*REIMU_B*/ 	/*MARISA_A*/	/*MARISA_B*/	/*REMILIA*/ 	/*YUYUKO ���i�K���� */	/*CIRNO_A*/ 	/*CIRNO_Q*/
	shot_regist_1,	shot_regist_1,	shot_regist_1,	shot_regist_1,	shot_regist_1,	shot_regist_chou,		shot_regist_2,	shot_regist_2,	/* WEAPON_L0:(P000-P008) */
	shot_regist_1,	shot_regist_1,	shot_regist_2,	shot_regist_2,	shot_regist_1,	shot_regist_chou,		shot_regist_2,	shot_regist_2,	/* WEAPON_L1:(P008-P015) */
	shot_regist_2,	shot_regist_2,	shot_regist_2,	shot_regist_3,	shot_regist_2,	shot_regist_chou,		shot_regist_2,	shot_regist_3,	/* WEAPON_L2:(P016-P031) */
	shot_regist_3,	shot_regist_3,	shot_regist_3,	shot_regist_4,	shot_regist_2,	shot_regist_chou,		shot_regist_3,	shot_regist_3,	/* WEAPON_L3:(P032-P063) */
	shot_regist_3,	shot_regist_4,	shot_regist_3,	shot_regist_4,	shot_regist_2,	shot_regist_chou,		shot_regist_3,	shot_regist_3,	/* WEAPON_L4:(P064-P127) */
	shot_regist_4,	shot_regist_5,	shot_regist_3,	shot_regist_5,	shot_regist_2,	shot_regist_chou,		shot_regist_3,	shot_regist_3,	/* WEAPON_L5:(P128) 	 */
				};
				(bbb[(cg_jiki_weapon_level_offset)])(s1);
			}
		}
	}
}
