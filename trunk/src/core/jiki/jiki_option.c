
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#include "jiki_local.h"

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�(���~���A �� �`���m)
---------------------------------------------------------*/

enum	/* _remilia_formation_ */
{
	FORMATION_00 = 0,
	FORMATION_01,
	FORMATION_02,
	FORMATION_03,
};
/*
	�ڕW:st ate1
	FORMATION_00: ���~���A�̒��O�Ɏl�� / �`���m�̒���Ɏl�l
	FORMATION_01: ���~���A�̎��������
	FORMATION_02: ��ʌ������x������� (y=250�̈ʒu�ɎU��΂�)
	FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) (���~���A�ɏd�Ȃ�悤�ɁBslow�������ƒi�K�I��option��~)
*/
				enum
				{	/* �ʏ펞 */
/* 0 */ 			FORMATION_00_LOCATE_X_REIMU_A = 0,		/* 5 */
/* 1 */ 			FORMATION_01_LOCATE_X_REIMU_B,			/* 5 */
/* 2 */ 			FORMATION_02_LOCATE_X_MARISA_A, 		/* 5 */
/* 3 */ 			FORMATION_03_LOCATE_X_MARISA_B, 		/* 5 */
/* 4 */ 			FORMATION_04_LOCATE_X_REMILIA,			/* 5 */
/* 5 */ 			FORMATION_05_LOCATE_X_YUYUKO,			/* 5 */
/* 6 */ 			FORMATION_06_LOCATE_X_CIRNO_A,			/* 5 */
/* 7 */ 			FORMATION_07_LOCATE_X_CIRNO_Q,			/* 5 */
//
/* 0 */ 			FORMATION_08_LOCATE_Y_REIMU_A,
/* 1 */ 			FORMATION_09_LOCATE_Y_REIMU_B,
/* 2 */ 			FORMATION_0a_LOCATE_Y_MARISA_A,
/* 3 */ 			FORMATION_0b_LOCATE_Y_MARISA_B,
/* 4 */ 			FORMATION_0c_LOCATE_Y_REMILIA,	/* 4 */ 	/* == 4 == (��2) == REMILIA �v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
/* 5 */ 			FORMATION_0d_LOCATE_Y_YUYUKO,
/* 6 */ 			FORMATION_0e_LOCATE_Y_CIRNO_A,	/* 6 */ 	/* == 6 == (��3) == CIRNO	�v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
/* 7 */ 			FORMATION_0f_LOCATE_Y_CIRNO_Q,	/* 7 */ 	/* == 7 == (��3) == CIRNO	�v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
//					/* �ᑬ�� */
/* 0 */ 			FORMATION_10_LOCATE_X_REIMU_A,			/* 5 */
/* 1 */ 			FORMATION_11_LOCATE_X_REIMU_B,			/* 5 */
/* 2 */ 			FORMATION_12_LOCATE_X_MARISA_A, 		/* 5 */
/* 3 */ 			FORMATION_13_LOCATE_X_MARISA_B, 		/* 5 */
/* 4 */ 			FORMATION_14_LOCATE_X_REMILIA,			/* 5 */
/* 5 */ 			FORMATION_15_LOCATE_X_YUYUKO,			/* 5 */
/* 6 */ 			FORMATION_16_LOCATE_X_CIRNO_A,			/* 5 */
/* 7 */ 			FORMATION_17_LOCATE_X_CIRNO_Q,			/* 5 */
//
/* 0 */ 			FORMATION_18_LOCATE_Y_REIMU_A,
/* 1 */ 			FORMATION_19_LOCATE_Y_REIMU_B,
/* 2 */ 			FORMATION_1a_LOCATE_Y_MARISA_A,
/* 3 */ 			FORMATION_1b_LOCATE_Y_MARISA_B,
/* 4 */ 			FORMATION_1c_LOCATE_Y_REMILIA,	/* 4 */ 	/* == 4 == (��2) == REMILIA �v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
/* 5 */ 			FORMATION_1d_LOCATE_Y_YUYUKO,
/* 6 */ 			FORMATION_1e_LOCATE_Y_CIRNO_A,	/* 6 */ 	/* == 6 == (��3) == CIRNO	�v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
/* 7 */ 			FORMATION_1f_LOCATE_Y_CIRNO_Q,	/* 7 */ 	/* == 7 == (��3) == CIRNO	�v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
					/* �Œ�p */
				//	FORMATION_02_LOCATE_X,			/* 2 */
				//	FORMATION_02_LOCATE_Y,			/* 3 */
					/* ��]�p */
					FORMATION_30_RADIUS_R,			/* 1 */
					FORMATION_31_ADD_R, 			/* 0 */
					FORMATION_99_MAX					/* 8 */ 	/* �ő吔 */
				};
//				enum
//				{
//					FORMATION_MAX					/* 8 */ 	/* �ő吔 */
//				};
				//	{	t256(15),  t256(-15),	t256(25),  t256(-25) }, 				/* FORMATION_00:			��FORMATION_00_LOCATE_X */
				//	{	t256(15),  t256( 15),	t256(20),  t256( 20) }, 				/* FORMATION_00:			��FORMATION_00_LOCATE_Y */
//				//	{	( 1*115+10),  ( 2*115+10),	( 0*115+10),  ( 3*115+10) },		/* FORMATION_02:			��ʌ������x�������(wideth380dot) */
				static const s8 ggg[FORMATION_99_MAX][4] =
				{
//#define JIKI_X_HARF (14)/*(31/2) ���@�̔��� */
/* �ʏ펞 (X���W�ʒu�I�t�Z�b�g) (6�̔{������{�ʒu, 75%�k���̈� 8�̔{����75%��6�̔{���ɂȂ�)6, 12, 18, 24, 30, 36, 42, 48, 54, 60, 66, 72, ... */
{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 0 �얲 A */
{	( 24),		 (-24),  ( 12), 	  (-12),	},	/* 1 �얲 B */
{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 2 ������ A */
{	( 54),		 (-54),  ( 24), 	  (-24),	},	/* 3 ������ B (Type B �ђʑ���) */
//	[3]�E�O 	[0]���O [2]�E�� 	[1]����
{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 4 ���~���A */
{	( 24),		 (-24),  ( 24), 	  (-24),	},	/* 5 �H�X�q */
{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 6 �`���m */
{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 7 �`���m */
												/* �ʏ펞 (Y���W�ʒu�I�t�Z�b�g) */
{	( -4),		 (-4),	 (-22), 	 (-22), 	},	/* 0 �얲 A */
{	(  5),		  (5),	 (-20), 	 (-20), 	},	/* 1 �얲 B */
{	(  5),		  (5),	 (-20), 	 (-20), 	},	/* 2 ������ A */
{	( -4),		 (-4),	 (-22), 	 (-22), 	},	/* 3 ������ B */
{	(-15),		(-15),	 (-29), 	 (-29), 	},	/* 4 ���~���A */	 /* �O���z�u */
{	(  7),		  (7),	  (-7), 	  (-7), 	},	/* 5 �H�X�q */
{	( 17),		 (17),	  (12), 	  (12), 	},	/* 6 �`���m A */
{	( 17),		 (17),	  (12), 	  (12), 	},	/* 7 �`���m Q */
												/* �ᑬ�� (X���W�ʒu�I�t�Z�b�g) */
{	( 12),		 (-12),   (24), 	 (-24), 	},	/* 0 �얲 A */
{	( 12),		 (-12),   (3),		  (-3), 	},	/* 1 �얲 B */
{	( 12),		 (-12),   (3),		  (-3), 	},	/* 2 ������ A */
{	(  9),		  (-9),   (6),		  (-6), 	},	/* 3 ������ B (Type B �ђʑ���) */
//	[3]�E�O 	[0]���O [2]�E�� 	[1]����
{	( 24),		 (-24),   (6),		  (-6), 	},	/* 4 ���~���A */
{	( 18),		 (-18),  (30),		 (-30), 	},	/* 5 �H�X�q */
{	( 24),		 (-24),   (6),		  (-6), 	},	/* 6 �`���m */
{	( 24),		 (-24),   (6),		  (-6), 	},	/* 7 �`���m */
												/* �ᑬ�� (Y���W�ʒu�I�t�Z�b�g) */
{	( 29),		 (29),	 (15),		 (15),		},	/* 0 �얲 A */
{	(-12),		(-12),	(-18),		(-18),		},	/* 1 �얲 B */
{	(-12),		(-12),	(-18),		(-18),		},	/* 2 ������ A */
{	( -4),		 (-4),	(-22),		(-22),		},	/* 3 ������ B */
{	(-15),		(-15),	(-29),		(-29),		},	/* 4 ���~���A */	/* �O���z�u */
{	( 18),		 (18),	  (4),		  (4),		},	/* 5 �H�X�q */
{	( 17),		 (17),	 (12),		 (12),		},	/* 6 �`���m A */
{	( 17),		 (17),	 (12),		 (12),		},	/* 7 �`���m Q */
					/* �Œ�p */
//
					/* ��]�p */
					{	(20),  ( 30),	(40),  ( 50) }, 								/* 1 FORMATION_30_RADIUS_R: ��]���a */
					{	( 1),  (  2),	(-3),  ( -6) }, 								/* 0 FORMATION_31_ADD_R:	��]�����A�p�x���Z�l */
			//		{	( 1*104+20),  ( 2*104+20),	( 0*104+20),  ( 3*104+20) },		/* 2 FORMATION_02_LOCATE_X: ��ʌ������x�������(width352dot) */
			//		{  (240),  (240),  (220),  (220) }, 								/* 3 FORMATION_02_LOCATE_Y: ��ʌ������x������� */
				};


/*---------------------------------------------------------

---------------------------------------------------------*/
static void move_delta_xy(SPRITE *src, int delta_x256, int delta_y256)
{	/* �ڕW�n�_�Ɉړ����� */
	int ccc_angle1024;
//	ccc_angle1024 = at an_1024(((delta_y256)>>8), ((delta_x256)>>8));
	ccc_angle1024 = atan_65536(((delta_y256)>>8), ((delta_x256)>>8));
	ccc_angle1024 >>= (6);

//	int ccc_speed256;
//	ccc_speed256 = (/*10*/((REMILIA==(cg_game_select_player))?t256(8.0):t256(1.0)));
	const int bbb_speed_tbl[(8)] =/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
	{	t256(4.0),	t256(4.0),	t256(4.0),	t256(4.0),	t256(8.0),	t256(2.0),	t256(1.0),	t256(1.0)	};
	const int ccc_speed256 = bbb_speed_tbl[(cg_game_select_player)];
	src->cx256 += ((sin1024(( ccc_angle1024))*(ccc_speed256))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
	src->cy256 += ((cos1024(( ccc_angle1024))*(ccc_speed256))>>8);/*fps_factor*/
}
/*---------------------------------------------------------

---------------------------------------------------------*/
#define oz_offset_r 	(src->PL_OPTION_DATA_offset_x256)	/* FORMATION_01: �~�̔��a�ƌ��p */
#define oz_offset_add_r (src->PL_OPTION_DATA_offset_y256)	/* FORMATION_01: �~�̉�]�����A�p�x���Z�l�ƌ��p */

/*---------------------------------------------------------
	FORMATION_00: ���~���A�̒��O�Ɏl�� / �`���m�̒���Ɏl�l
	FORMATION_02: ��ʌ������x�������
---------------------------------------------------------*/
	static int state_zz1=0;

static void control_formation00(SPRITE *src) /* FORMATION_00 */
{
	int player_offs_x256;
	int player_offs_y256;
	if (FORMATION_02==state_zz1)
	{
		player_offs_x256 = (0);
		player_offs_y256 = (0);
		/* ������ */
		{
			static const s16 aaa[(2+2)][4] =
			{
				{	( 1*104-20),  ( 2*104+20),	( 0*104+20),  ( 3*104-20) },		/* 2 FORMATION_02_LOCATE_X: ��ʌ������x�������(width352dot) */
				{  (240),  (240),  (220),  (220) }, 								/* 3 FORMATION_02_LOCATE_Y: ��ʌ������x������� */
				/* �ᑬ�p */
				{	( 170),  ( 210),	( 100),  ( 280) },		/* 2 FORMATION_02_LOCATE_X: ��ʌ������x�������(width352dot) */
				{  (240),  (240),  (220),  (220) }, 								/* 3 FORMATION_02_LOCATE_Y: ��ʌ������x������� */
			};
			const int if_teisoku = ((psp_pad.pad_data & PSP_KEY_SLOW))?( (2)):(0);
			src->PL_OPTION_DATA_offset_x256 = ((aaa[if_teisoku+0][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);
			src->PL_OPTION_DATA_offset_y256 = ((aaa[if_teisoku+1][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);/*240*/ /* �v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
		}
	}
	else
	{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		#if 1/*Gu(���S���W)*/
		player_offs_x256 = zzz_player->cx256;
		player_offs_y256 = zzz_player->cy256;
		#endif
		/* ������ */
		{
		//	/* FORMATION_00: ���~���A�̒��O�Ɏl�� / �`���m�̒���Ɏl�l */
			const int if_teisoku = ((psp_pad.pad_data & PSP_KEY_SLOW))?( (8+8)):(0);
			src->PL_OPTION_DATA_offset_x256 = ((ggg[if_teisoku+(cg_game_select_player)	][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);
			src->PL_OPTION_DATA_offset_y256 = ((ggg[if_teisoku+(cg_game_select_player)+(8)][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);/*240*/ /* �v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
		}
	}

	/* �ڕW�n�_�ֈړ��� */
	{
		int delta_y256=0;				/* delta_y : y���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
		int delta_x256=0;				/* delta_x : x���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
		{
	//	/* FORMATION_00: ���~���A�̒��O�Ɏl�� / �`���m�̒���Ɏl�l */
	//	/* FORMATION_02: ��ʌ������x������� */
			#if 1/*Gu(���S���W)*/
			delta_x256 = (-src->cx256+src->PL_OPTION_DATA_offset_x256);
			delta_y256 = (-src->cy256+src->PL_OPTION_DATA_offset_y256);/*240*/
			#endif
		//	not_break;
			delta_y256 += player_offs_y256;
			delta_x256 += player_offs_x256;
	//		break;
		}
		move_delta_xy(src, delta_x256, delta_y256); 	/* �ڕW�n�_�Ɉړ����� */
	#if 1
		/* ���~���A�p */
//		if (REMILIA==(cg_game_select_player))
		{
			/* �ڕW�t�߂܂ł��ǂ蒅������ */
			if (( t256(-5) < delta_y256) &&
				(delta_y256 < t256(5)) )
			{
				/* �ʒu�̈ێ� & �z�u����flag */
			//	/* FORMATION_00: ���~���A�̒��O�Ɏl�� / �`���m�̒���Ɏl�l */
			//	/* FORMATION_02: ��ʌ������x������� */
				src->cx256 = src->PL_OPTION_DATA_offset_x256;
				src->cy256 = src->PL_OPTION_DATA_offset_y256;
			//	not_break;
			//	src->cx256 = player_offs_x256 + src->PL_OPTION_DATA_offset_x256;
			//	src->cy256 = player_offs_y256 + src->PL_OPTION_DATA_offset_y256;
				src->cx256 += player_offs_x256;
				src->cy256 += player_offs_y256;
		//		break;
			}
		}
	#endif
	}
}
/*---------------------------------------------------------

---------------------------------------------------------*/
static void control_formation01(SPRITE *src) /* FORMATION_01 */
{
	int player_offs_x256;
	int player_offs_y256;
	{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		#if 1/*Gu(���S���W)*/
		player_offs_x256 = zzz_player->cx256;
		player_offs_y256 = zzz_player->cy256;
		#endif
	}
	/* ������ */
	{
	//	/* FORMATION_01: ���~���A�̎�������� */
			src->PL_OPTION_DATA_offset_x256 = ggg[FORMATION_30_RADIUS_R][(src->PL_OPTION_DATA_opt_anime_add_id)];/*40*/ /* �~�̔��a�ƌ��p */
			src->PL_OPTION_DATA_offset_y256 = ggg[FORMATION_31_ADD_R   ][(src->PL_OPTION_DATA_opt_anime_add_id)];		/* �~�̉�]�����A�p�x���Z�l�ƌ��p */
	}
	/* �ڕW�n�_�ֈړ��� */
	{
		int delta_y256=0;				/* delta_y : y���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
		int delta_x256=0;				/* delta_x : x���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
		{
	//	/* FORMATION_01: ���~���A�̎�������� */
			delta_x256 = player_offs_x256-src->cx256 + (sin1024((src->PL_OPTION_DATA_angleCCW1024/*REMILIA_angle512*/))*oz_offset_r);		/* CCW�̏ꍇ */
			delta_y256 = player_offs_y256-src->cy256 + (cos1024((src->PL_OPTION_DATA_angleCCW1024/*REMILIA_angle512*/))*oz_offset_r);
		}
		move_delta_xy(src, delta_x256, delta_y256); /* �ڕW�n�_�Ɉړ����� */
	#if 1
		/* ���~���A�p */
//		if (REMILIA==(cg_game_select_player))
		{
			/* �ڕW�t�߂܂ł��ǂ蒅������ */
			if (( t256(-5) < delta_y256) &&
				(delta_y256 < t256(5)) )
			{
			/* �ʒu�̈ێ� & �z�u����flag */
		/* FORMATION_01: ���~���A�̎�������� */
		/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024 += oz_offset_add_r;	/*5==ra d2deg512(0.05)*/	/*fps_factor*/
		mask1024(/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024);
		src->cx256 = player_offs_x256 + sin1024((src->PL_OPTION_DATA_angleCCW1024/*REMILIA_angle512*/))*oz_offset_r; /* CCW�̏ꍇ */
		src->cy256 = player_offs_y256 + cos1024((src->PL_OPTION_DATA_angleCCW1024/*REMILIA_angle512*/))*oz_offset_r;
			}
		}
	#endif
	}
}
			#if 0
			/* ���~���A�p */
			if (REMILIA==(cg_game_select_player))
			{
				/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024 += oz_offset_add_r;	/*5==ra d2deg512(0.05)*/	/*fps_factor*/
				mask1024(/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024);
			}
			#endif


/*---------------------------------------------------------

---------------------------------------------------------*/

static void control_formation03(SPRITE *src) /* FORMATION_03 */
{
	int player_offs_x256;
	int player_offs_y256;
	{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		#if 1/*Gu(���S���W)*/
		player_offs_x256 = zzz_player->cx256;
		player_offs_y256 = zzz_player->cy256;
		#endif
	}
//	/* ������ */
//	/* �ڕW�n�_�ֈړ��� */
	{
		int delta_y256=0;				/* delta_y : y���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
		int delta_x256=0;				/* delta_x : x���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
	//	switch ( state_zz1)
		{
	//	/* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
			delta_y256 = player_offs_y256-src->cy256;
			delta_x256 = player_offs_x256-src->cx256;
	//		break;
		}
		move_delta_xy(src, delta_x256, delta_y256); /* �ڕW�n�_�Ɉړ����� */
		#if 1
		/* ���~���A�p */
//		if (REMILIA==(cg_game_select_player))
		{
			/* �ڕW�t�߂܂ł��ǂ蒅������ */
			if (( t256(-5) < delta_y256) &&
				(delta_y256 < t256(5)) )
			{
//				/* �ʒu�̈ێ� & �z�u����flag */
				/* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
				src->cx256 = player_offs_x256;
				src->cy256 = player_offs_y256;
			}
		}
		#endif
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/ void player_control_option(SPRITE *src)
{
	int player_offs_x256;
	int player_offs_y256;
	{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		#if 1/*Gu(���S���W)*/
		player_offs_x256 = zzz_player->cx256;
		player_offs_y256 = zzz_player->cy256;
		#endif
	}
	/* ���Ȃ��Ƃ��ᑬ�ړ��ŃI�v�V�������~�߂���̂̓��~���A�̂�(�`���m�ɂ͍���Ȃ�) */
	if (REMILIA==(cg_game_select_player))
	{
		if (state_zz1==FORMATION_03)/* FORMATION_03: �������p(�D���Ȉʒu�ɒu�����) */
		{
			if (psp_pad.pad_data & PSP_KEY_SLOW)
			{
				/* ���~���A�p */
				src->PL_OPTION_DATA_slow_count += (1<<2);
				if (
					(src->PL_OPTION_DATA_slow_count < ((src->PL_OPTION_DATA_opt_anime_add_id+1)<<(6+2))/* *30 64*/))
				{
					src->cx256 = player_offs_x256;
					src->cy256 = player_offs_y256;
				}
			}
			else
			{
				/* ���~���A�p */
				if (src->PL_OPTION_DATA_slow_count)
				{
					src->PL_OPTION_DATA_slow_count=0;
				}
			}
		}
	}
	//
	int slow_remilia_flag;
	slow_remilia_flag = 0;
	/* �ᑬ�ړ� */
	if (REMILIA==(cg_game_select_player))
	{
		if (psp_pad.pad_data & PSP_KEY_SLOW)
		{
			slow_remilia_flag=1;
		}
	}
//	if (1==slow_remilia_flag)	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	if (0!=slow_remilia_flag)	/* ���Ȃ��Ƃ��ᑬ�ړ��ŃI�v�V�������~�߂���̂̓��~���A�̂�(�`���m�ɂ͍���Ȃ�) */
	{
		;
	}
	else
	{
	//
		/* �t�H�[���[�V�����ύX */
		/* OPTION_C1 �̏ꍇ�̂݃L�[���͎󂯕t�� */
		if (OPTION_C1==src->PL_OPTION_DATA_opt_anime_add_id) //opt_anime_add_id==1�ɂ��flag�Ǘ�
		{
		//	if (my_pad & PSP_KEY_OPTION)
		/* (�I�v�V�����L�[�L�[�𗣂����u�ԂȂ�) */
			if ((0==(psp_pad.pad_data & PSP_KEY_OPTION)))	/* �I�v�V�����L�[��������Ă��� */
			{
				if ( (psp_pad.pad_data^psp_pad.pad_data_alter) & PSP_KEY_OPTION)	/* �I�v�V�����L�[�̏�Ԃ��ς����(�g�O���A�������܂��͗������ꍇ) */
				{
	#if 0/*�f�o�b�O�p*/
	/*	���u�{�X�Ƒ��ł�����ƃn���O�A�b�v�v�o�O(�`r26)�΍� */
	/*(�{�X�|�����ɋ�炢�{�����?����start��������Ȃ��Ȃ�o�O������B���I�v�V�����L�[�͌���)*/
	/* �p�l���̃X�R�A����move_type���A�O���C�Y����state_zz1��\��������B���Ă����������������Ⴄ�B */
	/*
	my_score0000460960 == 0xb410
	0x1
		STATE_FLAG_05_IS_BOSS						(0x0010)
	0x4
		ST ATE_FLAG_11_IS_BOSS_DESTROY				(0x0400)
	0xb
		ST ATE_FLAG_13_DRAW_BOSS_GAUGE				(0x1000)
		ST ATE_FLAG_14_GAME_LOOP_QUIT				(0x2000)
		ST ATE_FLAG_16_NOT_ALLOW_KEY_CONTROL		(0x8000)
	 */
	p d_my_score	= cg.state_flag;
	p d_graze_point = state_zz1;
	#endif
					state_zz1++;
					if ( state_zz1==(FORMATION_03+1) )
					{	 state_zz1=FORMATION_00;}
				}
			}
		}
		{
			static /*const*/ void (*formation_call_table[(4)])(SPRITE *src) =
			{
				control_formation00,
				control_formation01,
				control_formation00,
				control_formation03
			};
			(formation_call_table[(state_zz1&0x03)])(src);
//					if (FORMATION_00==state_zz1)	{control_formation00(src);}
//			else	if (FORMATION_01==state_zz1)	{control_formation01(src);}
//			else	if (FORMATION_02==state_zz1)	{control_formation02(src);}
//			else	if (FORMATION_03==state_zz1)	{control_formation03(src);}
		}
	}
}
