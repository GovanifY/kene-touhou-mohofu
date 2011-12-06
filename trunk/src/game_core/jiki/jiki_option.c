
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#include "jiki_local.h"

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�(���~���A �� �`���m)
---------------------------------------------------------*/

enum	/* _formation_ */
{	/* �ʏ펞 */
	FORMATION_00_LOCATE_X_REIMU_A = 0,	/* 0 */ 	/* 5 */
	FORMATION_01_LOCATE_X_REIMU_B,		/* 1 */ 	/* 5 */
	FORMATION_02_LOCATE_X_MARISA_A, 	/* 2 */ 	/* 5 */
	FORMATION_03_LOCATE_X_MARISA_B, 	/* 3 */ 	/* 5 */
	FORMATION_04_LOCATE_X_REMILIA,		/* 4 */ 	/* 5 */
	FORMATION_05_LOCATE_X_YUYUKO,		/* 5 */ 	/* 5 */
	FORMATION_06_LOCATE_X_CIRNO_A,		/* 6 */ 	/* 5 */
	FORMATION_07_LOCATE_X_CIRNO_Q,		/* 7 */ 	/* 5 */
	//
	FORMATION_08_LOCATE_Y_REIMU_A,		/* 0 */
	FORMATION_09_LOCATE_Y_REIMU_B,		/* 1 */
	FORMATION_0a_LOCATE_Y_MARISA_A, 	/* 2 */
	FORMATION_0b_LOCATE_Y_MARISA_B, 	/* 3 */
	FORMATION_0c_LOCATE_Y_REMILIA,		/* 4 */ 	/* 4 */ 	/* == 4 == (��2) == REMILIA �v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
	FORMATION_0d_LOCATE_Y_YUYUKO,		/* 5 */
	FORMATION_0e_LOCATE_Y_CIRNO_A,		/* 6 */ 	/* 6 */ 	/* == 6 == (��3) == CIRNO	�v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
	FORMATION_0f_LOCATE_Y_CIRNO_Q,		/* 7 */ 	/* 7 */ 	/* == 7 == (��3) == CIRNO	�v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
	/* �ᑬ�� */
	FORMATION_10_LOCATE_X_REIMU_A,		/* 0 */ 	/* 5 */
	FORMATION_11_LOCATE_X_REIMU_B,		/* 1 */ 	/* 5 */
	FORMATION_12_LOCATE_X_MARISA_A, 	/* 2 */ 	/* 5 */
	FORMATION_13_LOCATE_X_MARISA_B, 	/* 3 */ 	/* 5 */
	FORMATION_14_LOCATE_X_REMILIA,		/* 4 */ 	/* 5 */
	FORMATION_15_LOCATE_X_YUYUKO,		/* 5 */ 	/* 5 */
	FORMATION_16_LOCATE_X_CIRNO_A,		/* 6 */ 	/* 5 */
	FORMATION_17_LOCATE_X_CIRNO_Q,		/* 7 */ 	/* 5 */
	//
	FORMATION_18_LOCATE_Y_REIMU_A,		/* 0 */
	FORMATION_19_LOCATE_Y_REIMU_B,		/* 1 */
	FORMATION_1a_LOCATE_Y_MARISA_A, 	/* 2 */
	FORMATION_1b_LOCATE_Y_MARISA_B, 	/* 3 */
	FORMATION_1c_LOCATE_Y_REMILIA,		/* 4 */ 	/* 4 */ 	/* == 4 == (��2) == REMILIA �v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
	FORMATION_1d_LOCATE_Y_YUYUKO,		/* 5 */
	FORMATION_1e_LOCATE_Y_CIRNO_A,		/* 6 */ 	/* 6 */ 	/* == 6 == (��3) == CIRNO	�v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
	FORMATION_1f_LOCATE_Y_CIRNO_Q,		/* 7 */ 	/* 7 */ 	/* == 7 == (��3) == CIRNO	�v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
	/* �Œ�p */
//	FORMATION_TYPE_02_LOCATE_X, 		/* z */ 	/* 2 */
//	FORMATION_TYPE_02_LOCATE_Y, 		/* z */ 	/* 3 */
	/* ��]�p */
	FORMATION_30_RADIUS_R,				/* z */ 	/* 1 */
	FORMATION_31_ADD_R, 				/* z */ 	/* 0 */
	FORMATION_99_MAX					/* z */ 	/* 8 */ 	/* �ő吔 */
};

enum	/* _formation_type_ */
{
	FORMATION_TYPE_00 = 0,	/* �O���z�u */
	FORMATION_TYPE_01,		/* �q���z�u */
	FORMATION_TYPE_02,		/* ����z�u */
	FORMATION_TYPE_03,		/* �W�ϔz�u */
	FORMATION_TYPE_99_MAX/*(==4)*/
};
/*
	�ڕW:st ate1
	FORMATION_TYPE_00: ���~���A�̒��O�Ɏl�� / �`���m�̒���Ɏl�l
	FORMATION_TYPE_01: ���~���A�̎��������
	FORMATION_TYPE_02: ��ʌ������x������� (y=250�̈ʒu�ɎU��΂�)
	FORMATION_TYPE_03: �������p(�D���Ȉʒu�ɒu�����) (���~���A�ɏd�Ȃ�悤�ɁBslow�������ƒi�K�I��option��~)
*/
	//#define JIKI_X_HARF (14)/*(31/2) ���@�̔��� */
/*
���W�ʒu�I�t�Z�b�g�ɂ���:
	�����������A�ʒu�f�[�^�[�ߖ�ׂ̈炵���A�I�v�V�����̊�ʒu�͔C�ӂ̍��W�ł͂Ȃ�
	8�̔{���̍��W�ɂ����u���Ȃ��d�l�ɂȂ��Ă���悤�ł��B
	�͕핗�� 75% �k���C���[�W����{�ł��̂ŁA��{�ʒu��6�̔{���Ƃ��܂��B(6.00 == 8.00 * 0.75)
	(6�̔{������{�ʒu, 75%�k���̈� 8�̔{����75%��6�̔{���ɂȂ�)
	6, 12, 18, 24, 30, 36, 42, 48, 54, 60, 66, 72, ...
	�������P����6�̔{���ɂ���ƈʒu�����������Ȃ�(���E�ł����Ƃ��A�Ԋu���ςƂ�)�̂ŁA
	�����������̂Ɋւ��Ă͈ʒu���C�����܂��B
*/
static const s8 ggg[FORMATION_99_MAX][4] =
{
	/* �ʏ펞 (X���W�ʒu�I�t�Z�b�g)*/
	{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 0 �얲 A */
	{	( 24),		 (-24),  ( 12), 	  (-12),	},	/* 1 �얲 B */
	{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 2 ������ A */
	{	( 54),		 (-54),  ( 24), 	  (-24),	},	/* 3 ������ B (Type B �ђʑ���) */
	//	[3]�E�O 	[0]���O [2]�E�� 	[1]����
	{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 4 ���~���A */
	{	( 24),		 (-24),  ( 24), 	  (-24),	},	/* 5 �H�X�q */
	{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 6 �`���m A */
	{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 7 �`���m Q */
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
	{	( 24),		 (-24),   (6),		  (-6), 	},	/* 6 �`���m A */
	{	( 24),		 (-24),   (6),		  (-6), 	},	/* 7 �`���m Q */
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
};
//	{	( 1*104+20),  ( 2*104+20),	( 0*104+20),  ( 3*104+20) },		/* 2 FORMATION_TYPE_02_LOCATE_X: ��ʌ������x�������(width352[pixel]) */
//	{  (240),  (240),  (220),  (220) }, 								/* 3 FORMATION_TYPE_02_LOCATE_Y: ��ʌ������x������� */

//	{	t256(15),  t256(-15),	t256(25),  t256(-25) }, 				/* FORMATION_TYPE_00:			��FORMATION_TYPE_00_LOCATE_X */
//	{	t256(15),  t256( 15),	t256(20),  t256( 20) }, 				/* FORMATION_TYPE_00:			��FORMATION_TYPE_00_LOCATE_Y */
//	{	( 1*115+10),  ( 2*115+10),	( 0*115+10),  ( 3*115+10) },		/* FORMATION_TYPE_02:			��ʌ������x�������(wideth380[pixel]) */


/*---------------------------------------------------------

---------------------------------------------------------*/
static void move_delta_xy(OBJ/**/ *src, int delta_x256, int delta_y256)
{	/* �ڕW�n�_�Ɉړ����� */
//	int ccc_angle1024 = at an_1024(((delta_y256)>>8), ((delta_x256)>>8));
//	int ccc_speed256 = (/*10*/((REMILIA==(cg_game_select_player))?t256(8.0):t256(1.0)));

	REG_00_SRC_X	= (((delta_x256)>>8));			/*(�_����)*/
	REG_01_SRC_Y	= (((delta_y256)>>8));			/*(�_����)*/
	REG_02_DEST_X	= (0);							/*(�_����)*/
	REG_03_DEST_Y	= (0);							/*(�_����)*/
	tmp_angleCCW65536_src_nerai();
	const int cc_angle65536 = HATSUDAN_03_angle65536;
	const int bbb_speed_tbl[(8)] =/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
	{	t256(4.0),	t256(4.0),	t256(4.0),	t256(4.0),	t256(8.0),	t256(2.0),	t256(1.0),	t256(1.0)	};
	const int ccc_speed256 = bbb_speed_tbl[(cg_game_select_player)];
	#if (0)//
	src->center.x256 += ((si n1024((ccc_angle1024))*(ccc_speed256))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
	src->center.y256 += ((co s1024((ccc_angle1024))*(ccc_speed256))>>8);/*fps_factor*/
	//#el se
	{
		int sin_value_t256; 	//	sin_value_t256 = 0;
		int cos_value_t256; 	//	cos_value_t256 = 0;
		int256_si nco s1024( (deg65536to1024(cc_angle65536)), &sin_value_t256, &cos_value_t256);	/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
		src->center.x256 += ((sin_value_t256*(ccc_speed256))>>8);/*fps_factor*/
		src->center.y256 += ((cos_value_t256*(ccc_speed256))>>8);/*fps_factor*/
	}
	#endif
	//------------------
	HATSUDAN_01_speed256	= (ccc_speed256);
	HATSUDAN_03_angle65536	= (cc_angle65536);
	sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
	src->center.x256 += REG_03_DEST_Y;//sin_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
	src->center.y256 += REG_02_DEST_X;//cos_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
	//------------------

}
/*---------------------------------------------------------

---------------------------------------------------------*/
#define remi_offset_r		(src->PL_OPTION_DATA_offset_x256)	/* FORMATION_TYPE_01: �~�̔��a�ƌ��p */
#define remi_offset_add_r	(src->PL_OPTION_DATA_offset_y256)	/* FORMATION_TYPE_01: �~�̉�]�����A�p�x���Z�l�ƌ��p */

/*---------------------------------------------------------
	FORMATION_TYPE_00: ���~���A�̒��O�Ɏl�� / �`���m�̒���Ɏl�l
	FORMATION_TYPE_02: ��ʌ������x�������
---------------------------------------------------------*/
//	static u8 cg.player_option_mode = 0;
#define control_formation00 control_formation00_and_02
#define control_formation02 control_formation00_and_02

static OBJ_CALL_FUNC(control_formation00_and_02) /* FORMATION_TYPE_00 */ /* FORMATION_TYPE_02 */
{
	int player_offs_x256;
	int player_offs_y256;
	if (FORMATION_TYPE_00==cg.player_option_mode)
	{
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
		#if 1/*Gu(���S���W)*/
		player_offs_x256 = zzz_player->center.x256;
		player_offs_y256 = zzz_player->center.y256;
		#endif
		/* ������ */
		{
		//	/* FORMATION_TYPE_00: ���~���A�̒��O�Ɏl�� / �`���m�̒���Ɏl�l */
			const int if_teisoku = ((psp_pad.pad_data & PSP_KEY_SLOW))?( (8+8)):(0);
			src->PL_OPTION_DATA_offset_x256 = ((ggg[if_teisoku+(cg_game_select_player)	  ][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);
			src->PL_OPTION_DATA_offset_y256 = ((ggg[if_teisoku+(cg_game_select_player)+(8)][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);/*240*/ /* �v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
		}
	}
	else
//	if (FORMATION_TYPE_02==cg.player_option_mode)
	{
		player_offs_x256 = (0);
		player_offs_y256 = (0);
		/* ������ */
		{
			static const s16 aaa[(2+2)][4] =
			{
				{
					(GAME_X_OFFSET+ (1*104) - 20),
					(GAME_X_OFFSET+ (2*104) + 20),
					(GAME_X_OFFSET+ (0*104) + 20),
					(GAME_X_OFFSET+ (3*104) - 20)
				},		/* 2 FORMATION_TYPE_02_LOCATE_X: ��ʌ������x�������(width352[pixel]) */
				{
					(240),
					(240),
					(220),
					(220)
				},								/* 3 FORMATION_TYPE_02_LOCATE_Y: ��ʌ������x������� */
				/* �ᑬ�p */
				{
					(GAME_X_OFFSET+ (1*70) + 100   -32),
					(GAME_X_OFFSET+ (2*70) +  70   -32),
					(GAME_X_OFFSET+ (0*70) + 100   -32),
					(GAME_X_OFFSET+ (3*70) +  70   -32)
				},								/* 2 FORMATION_TYPE_02_LOCATE_X: ��ʌ������x�������(width352[pixel]) */
				{
					(240),
					(240),
					(220),
					(220)
				},								/* 3 FORMATION_TYPE_02_LOCATE_Y: ��ʌ������x������� */
			};
			const int if_teisoku = ((psp_pad.pad_data & PSP_KEY_SLOW))?( (2)):(0);
			src->PL_OPTION_DATA_offset_x256 = ((aaa[if_teisoku+0][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);
			src->PL_OPTION_DATA_offset_y256 = ((aaa[if_teisoku+1][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);/*240*/ /* �v���C���[�ԍ��ɍ��킹�Ă��鎖�ɒ��� */
		}
	}

	/* �ڕW�n�_�ֈړ��� */
	{
		int delta_y256	= (0);				/* delta_y : y���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
		int delta_x256	= (0);				/* delta_x : x���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
		{
	//	/* FORMATION_TYPE_00: ���~���A�̒��O�Ɏl�� / �`���m�̒���Ɏl�l */
	//	/* FORMATION_TYPE_02: ��ʌ������x������� */
			#if 1/*Gu(���S���W)*/
			delta_x256 = (-src->center.x256+src->PL_OPTION_DATA_offset_x256);
			delta_y256 = (-src->center.y256+src->PL_OPTION_DATA_offset_y256);/*240*/
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
			//	/* FORMATION_TYPE_00: ���~���A�̒��O�Ɏl�� / �`���m�̒���Ɏl�l */
			//	/* FORMATION_TYPE_02: ��ʌ������x������� */
				src->center.x256 = src->PL_OPTION_DATA_offset_x256;
				src->center.y256 = src->PL_OPTION_DATA_offset_y256;
			//	not_break;
			//	src->center.x256 = player_offs_x256 + src->PL_OPTION_DATA_offset_x256;
			//	src->center.y256 = player_offs_y256 + src->PL_OPTION_DATA_offset_y256;
				src->center.x256 += player_offs_x256;
				src->center.y256 += player_offs_y256;
		//		break;
			}
		}
	#endif
	}
}
/*---------------------------------------------------------

---------------------------------------------------------*/
static OBJ_CALL_FUNC(control_formation01) /* FORMATION_TYPE_01 */
{
	int player_offs_x256;
	int player_offs_y256;
	{
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
		#if 1/*Gu(���S���W)*/
		player_offs_x256 = zzz_player->center.x256;
		player_offs_y256 = zzz_player->center.y256;
		#endif
	}
	/* ������ */
	{
	//	/* FORMATION_TYPE_01: ���~���A�̎�������� */
			src->PL_OPTION_DATA_offset_x256 = ggg[FORMATION_30_RADIUS_R][(src->PL_OPTION_DATA_opt_anime_add_id)];/*40*/ /* �~�̔��a�ƌ��p */
			src->PL_OPTION_DATA_offset_y256 = ggg[FORMATION_31_ADD_R   ][(src->PL_OPTION_DATA_opt_anime_add_id)];		/* �~�̉�]�����A�p�x���Z�l�ƌ��p */
	}
	/* �ڕW�n�_�ֈړ��� */
	{
		int delta_y256; 			/* delta_y : y���W�ɂ�����ڕW�n�_�ƌ��ݒn�_�Ƃ̍� */
		int delta_x256; 			/* delta_x : x���W�ɂ�����ڕW�n�_�ƌ��ݒn�_�Ƃ̍� */
		//	delta_y256 = 0;
		//	delta_x256 = 0;
	//	/* FORMATION_TYPE_01: ���~���A�̎�������� */
		/*REMILIA_angle512*/
		//------------------
		HATSUDAN_01_speed256	= ((remi_offset_r)<<8);
		HATSUDAN_03_angle65536	= deg1024to65536((src->PL_OPTION_DATA_angleCCW1024));
		sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
		delta_x256 = (player_offs_x256-src->center.x256) + REG_03_DEST_Y;//sin_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
		delta_y256 = (player_offs_y256-src->center.y256) + REG_02_DEST_X;//cos_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
		//------------------
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
		/* FORMATION_TYPE_01: ���~���A�̎�������� */
		/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024 += remi_offset_add_r;	/*5==ra d2deg512(0.05)*/	/*fps_factor*/
		mask1024(/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024);
		/*REMILIA_angle512*/
		//------------------
		HATSUDAN_01_speed256	= ((remi_offset_r)<<8);
		HATSUDAN_03_angle65536	= deg1024to65536((src->PL_OPTION_DATA_angleCCW1024));
		sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
		src->center.x256 = (player_offs_x256) + REG_03_DEST_Y;//sin_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
		src->center.y256 = (player_offs_y256) + REG_02_DEST_X;//cos_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
		//------------------
			}
		}
	#endif
	}
}

			#if 0
			/* ���~���A�p */
			if (REMILIA==(cg_game_select_player))
			{
				/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024 += remi_offset_add_r;	/*5==ra d2deg512(0.05)*/	/*fps_factor*/
				mask1024(/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024);
			}
			#endif


/*---------------------------------------------------------

---------------------------------------------------------*/

static OBJ_CALL_FUNC(control_formation03) /* FORMATION_TYPE_03 */
{
	int player_offs_x256;
	int player_offs_y256;
	{
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
		player_offs_x256 = zzz_player->center.x256;
		player_offs_y256 = zzz_player->center.y256;
	}
//	/* ������ */
//	/* �ڕW�n�_�ֈړ��� */
	{
		int delta_y256 = (0);				/* delta_y : y���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
		int delta_x256 = (0);				/* delta_x : x���W�ɂ�����ڕW�n�_�ƌ��ݒn�̍� */
		/* FORMATION_TYPE_03: �������p(�D���Ȉʒu�ɒu�����) */
		delta_y256 = (player_offs_y256) - (src->center.y256);
		delta_x256 = (player_offs_x256) - (src->center.x256);
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
				/* FORMATION_TYPE_03: �������p(�D���Ȉʒu�ɒu�����) */
				src->center.x256 = player_offs_x256;
				src->center.y256 = player_offs_y256;
			}
		}
		#endif
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/ OBJ_CALL_FUNC(player_control_option)
{
	unsigned int slow_remilia_flag;
	slow_remilia_flag = 0;
	/* �ᑬ�ړ� */
	if (REMILIA==(cg_game_select_player))
	{
		if (FORMATION_TYPE_03==cg.player_option_mode)/* FORMATION_TYPE_03: �������p(�D���Ȉʒu�ɒu�����) */
		{
			if (psp_pad.pad_data & PSP_KEY_SLOW)
			{
				/* ���~���A�p */
				src->PL_OPTION_DATA_slow_count += (1<<2);
				if (
					(src->PL_OPTION_DATA_slow_count < ((src->PL_OPTION_DATA_opt_anime_add_id+1)<<(6+2))/* *30 64*/))
				{
					OBJ *zzz_player;
					zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
					/* FORMATION_TYPE_03: �������p(�D���Ȉʒu�ɒu�����) */
					src->center.x256 = zzz_player->center.x256;
					src->center.y256 = zzz_player->center.y256;
				}
			}
			else
			{
				/* ���~���A�p */
				if (src->PL_OPTION_DATA_slow_count)
				{
					src->PL_OPTION_DATA_slow_count = (0);
				}
			}
		}
		if (psp_pad.pad_data & PSP_KEY_SLOW)
		{
			slow_remilia_flag = 1;
		}
	}
//	if (1==slow_remilia_flag)	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	/* ���Ȃ��Ƃ��ᑬ�ړ��ŃI�v�V�������~�߂���̂̓��~���A�̂�(�`���m�ɂ͍���Ȃ�) */
	if (0!=slow_remilia_flag)
	{
		;/* ���~���A���ᑬ�ړ��� */
	}
	else	/* ����ȊO�B(���L���� + ���~���A���ʏ푬�ړ���) */
	{
		/* �t�H�[���[�V�����ύX */
		/* OPTION_C1 �̏ꍇ�̂݃L�[���͎󂯕t�� */
		if (OPTION_C1==src->PL_OPTION_DATA_opt_anime_add_id) //opt_anime_add_id==1�ɂ��flag�Ǘ�
		{
			/* (�I�v�V�����L�[�L�[�𗣂����u�ԂȂ�) */
			if ((0==(psp_pad.pad_data & PSP_KEY_OPTION)))	/* �I�v�V�����L�[��������Ă��� */
			{
				if ((psp_pad.pad_data^psp_pad.pad_data_alter) & PSP_KEY_OPTION)	/* �I�v�V�����L�[�̏�Ԃ��ς����(�g�O���A�������܂��͗������ꍇ) */
				{
					cg.player_option_mode++;
					cg.player_option_mode &= (4-1);//if (FORMATION_TYPE_99_MAX == cg.player_option_mode)	{	 cg.player_option_mode = FORMATION_TYPE_00;}
				}
			}
		}
		{
			static /*const*/ void (*formation_call_table[(4)])(OBJ/**/ *src) =
			{
				control_formation00,
				control_formation01,
				control_formation02,
				control_formation03
			};
			(formation_call_table[(cg.player_option_mode&(0x03))])(src);
		}
	}
}

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̈ړ�(�얲�A�������A���H�X�q)
---------------------------------------------------------*/

/*---------------------------------------------------------
	�v���C���[�A�I�v�V�����̒�`
---------------------------------------------------------*/
/* static */extern OBJ_CALL_FUNC(player_move_option);
global/*static*/ void jiki_option_create(void)
{
	unsigned int jj;
	for (jj=0; jj<(OPTION_04_MAX); jj++)
	{
		OBJ *h;
		h						= obj_regist_direct_number(OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_04_JIKI_OPTION0/* +OPTION_C1 */+jj);	/* �K���o�^�ł���B */
		h->jyumyou				= JYUMYOU_MUGEN;/* ���ԂŎ����������Ȃ� */
		{
		//	h->obj_type_set 				= (JIKI_OPTION_00_00)+kk; kk += (16);/* �I�v�V�����C���^�[���[�u */ /* 8 */
			h->obj_type_set 				= (JIKI_OPTION_00_00)+(jj<<4); /* �I�v�V�����C���^�[���[�u */ /* 8 */
			{
				static const /* int */s16 aaa_tbl[4] =
				{
					(1024/8)*(3),	//cv1024r(45)*(8-5),	/* cv1024r(45)*1 */ /* 1*1024/8 */
					(1024/8)*(1),	//cv1024r(45)*(8-7),	/* cv1024r(45)*5 */ /* 5*1024/8 */
					(1024/8)*(7),	//cv1024r(45)*(8-1),	/* cv1024r(45)*3 */ /* 3*1024/8 */
					(1024/8)*(5)	//cv1024r(45)*(8-3) 	/* cv1024r(45)*7 */ /* 7*1024/8 */
				};
				h->PL_OPTION_DATA_angleCCW1024 = aaa_tbl[jj];/* REMILIA_angle1024 */
			}
				h->PL_OPTION_DATA_opt_anime_add_id		= (OPTION_C1+jj);
		//
			h->m_Hit256R					= TAMA_ATARI_BULLET_DUMMY;
		//	h->PL_OPTION_DATA_yuukou_flag 	= (PL_OPTION_FLAG_01_OPTION_ON);
			h->PL_OPTION_DATA_yuukou_flag 	= (PL_OPTION_FLAG_00_OPTION_OFF); 	/* ���t���O��OFF(�s��) */
			h->atari_hantei 				= ATARI_HANTEI_OFF; 				/* �����蔻���OFF(���G) */
//	/* ??? */ h->kougeki_ti 				= (0/* 1 8*5 */);/*(�I�v�V�������G���ɑ̓����肵���ꍇ�̋����B1�Ȃ�|����B�����蔻��͂��邪0�Ȃ�|���Ȃ��B)*/
		//
			h->callback_mover				= player_move_option;
		//
			h->PL_OPTION_DATA_offset_x256			= t256(0);
			h->PL_OPTION_DATA_offset_y256			= t256(0);
			h->PL_OPTION_DATA_slow_count			= 0;
			h->PL_OPTION_DATA_opt_shot_interval 	= 0;
			h->PL_OPTION_DATA_anime_wait			= 0;
			/* ���~���A�̃I�v�V�����͔��������ۂ� */
			if (REMILIA==(cg_game_select_player))
			{
			//	h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x96);	/*	h->alpha			= 0x96; */	/* ����߂��� */
				h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x80);	/*	h->alpha			= 0x80; */	/* ������ */
			//	h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	/*	h->alpha			= 0x50; */	/* �Â�߂��� */
			}
		}
	}
}
	//	h->PL_OPTION_DATA_next					= obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
	//	h->PL_OPTION_DATA_ccc_angle512			= cv1024r((0));
	//	h->PL_OPTION_DATA_state2				= 0;
	//	h->PL_OPTION_DATA_state 				= 0;
	//	h->PL_OPTION_DATA_state1				= 0;///
			#if /*(�X�e�[�W�J�n���ɍĐݒ肳���̂ŗv��Ȃ�)*/0/* Gu(���S���W) */
			h->center.x256					= (s1->center.x256);	/* �v���C���[�I�v�V�����ʒu�̏����� */
			h->center.y256					= (s1->center.y256);	/* �v���C���[�I�v�V�����ʒu�̏����� */
			#endif

	#if (0)/*�f�o�b�O�p*/
	/*	���u�{�X�Ƒ��ł�����ƃn���O�A�b�v�v�o�O(�`r26)�΍� */
	/*(�{�X�|�����ɋ�炢�{�����?����start��������Ȃ��Ȃ�o�O������B���I�v�V�����L�[�͌���)*/
	/* �p�l���̃X�R�A����move_type���A�O���C�Y����cg.player_option_mode��\��������B���Ă����������������Ⴄ�B */
	/*
	my_score0000460960 == 0xb410
	0x1
		STATE_FLAG_0x0800_IS_BOSS						(0x0010)
	0x4
	0xb
		ST ATE_FLAG_13_DRAW_BOSS_GAUGE				(0x1000)
		ST ATE_FLAG_14_GAME_LOOP_QUIT				(0x2000)
		ST ATE_FLAG_16_NOT_ALLOW_KEY_CONTROL		(0x8000)
	 */
	cg.game_score	= cg.state_flag;
	cg.graze_point	= cg.player_option_mode;
	#endif
