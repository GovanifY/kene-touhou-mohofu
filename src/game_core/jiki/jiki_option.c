
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#include "jiki_local.h"

/*---------------------------------------------------------
	プレイヤー、オプションの移動(レミリア ＆ チルノ)
---------------------------------------------------------*/

enum	/* _formation_ */
{	/* 通常時 */
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
	FORMATION_0c_LOCATE_Y_REMILIA,		/* 4 */ 	/* 4 */ 	/* == 4 == (旧2) == REMILIA プレイヤー番号に合わせている事に注意 */
	FORMATION_0d_LOCATE_Y_YUYUKO,		/* 5 */
	FORMATION_0e_LOCATE_Y_CIRNO_A,		/* 6 */ 	/* 6 */ 	/* == 6 == (旧3) == CIRNO	プレイヤー番号に合わせている事に注意 */
	FORMATION_0f_LOCATE_Y_CIRNO_Q,		/* 7 */ 	/* 7 */ 	/* == 7 == (旧3) == CIRNO	プレイヤー番号に合わせている事に注意 */
	/* 低速時 */
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
	FORMATION_1c_LOCATE_Y_REMILIA,		/* 4 */ 	/* 4 */ 	/* == 4 == (旧2) == REMILIA プレイヤー番号に合わせている事に注意 */
	FORMATION_1d_LOCATE_Y_YUYUKO,		/* 5 */
	FORMATION_1e_LOCATE_Y_CIRNO_A,		/* 6 */ 	/* 6 */ 	/* == 6 == (旧3) == CIRNO	プレイヤー番号に合わせている事に注意 */
	FORMATION_1f_LOCATE_Y_CIRNO_Q,		/* 7 */ 	/* 7 */ 	/* == 7 == (旧3) == CIRNO	プレイヤー番号に合わせている事に注意 */
	/* 固定用 */
//	FORMATION_TYPE_02_LOCATE_X, 		/* z */ 	/* 2 */
//	FORMATION_TYPE_02_LOCATE_Y, 		/* z */ 	/* 3 */
	/* 回転用 */
	FORMATION_30_RADIUS_R,				/* z */ 	/* 1 */
	FORMATION_31_ADD_R, 				/* z */ 	/* 0 */
	FORMATION_99_MAX					/* z */ 	/* 8 */ 	/* 最大数 */
};

enum	/* _formation_type_ */
{
	FORMATION_TYPE_00 = 0,	/* 前方配置 */
	FORMATION_TYPE_01,		/* 衛星配置 */
	FORMATION_TYPE_02,		/* 後方配置 */
	FORMATION_TYPE_03,		/* 集積配置 */
	FORMATION_TYPE_99_MAX/*(==4)*/
};
/*
	目標:st ate1
	FORMATION_TYPE_00: レミリアの直前に四つ / チルノの直後に四人
	FORMATION_TYPE_01: レミリアの周りを回るよ
	FORMATION_TYPE_02: 画面後方から支援するよ (y=250の位置に散らばる)
	FORMATION_TYPE_03: 減速時用(好きな位置に置けるよ) (レミリアに重なるように。slowを押すと段階的にoption停止)
*/
	//#define JIKI_X_HARF (14)/*(31/2) 自機の半分 */
/*
座標位置オフセットについて:
	調査した処、位置データー節約の為らしく、オプションの基準位置は任意の座標ではなく
	8の倍数の座標にしか置けない仕様になっているようです。
	模倣風は 75% 縮小イメージが基本ですので、基本位置を6の倍数とします。(6.00 == 8.00 * 0.75)
	(6の倍数が基本位置, 75%縮小の為 8の倍数の75%が6の倍数になる)
	6, 12, 18, 24, 30, 36, 42, 48, 54, 60, 66, 72, ...
	しかし単純に6の倍数にすると位置がおかしくなる(左右でずれるとか、間隔が変とか)ので、
	そう言うものに関しては位置を修正します。
*/
static const s8 ggg[FORMATION_99_MAX][4] =
{
	/* 通常時 (X座標位置オフセット)*/
	{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 0 霊夢 A */
	{	( 24),		 (-24),  ( 12), 	  (-12),	},	/* 1 霊夢 B */
	{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 2 魔理沙 A */
	{	( 54),		 (-54),  ( 24), 	  (-24),	},	/* 3 魔理沙 B (Type B 貫通装備) */
	//	[3]右外 	[0]左外 [2]右内 	[1]左内
	{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 4 レミリア */
	{	( 24),		 (-24),  ( 24), 	  (-24),	},	/* 5 幽々子 */
	{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 6 チルノ A */
	{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 7 チルノ Q */
													/* 通常時 (Y座標位置オフセット) */
	{	( -4),		 (-4),	 (-22), 	 (-22), 	},	/* 0 霊夢 A */
	{	(  5),		  (5),	 (-20), 	 (-20), 	},	/* 1 霊夢 B */
	{	(  5),		  (5),	 (-20), 	 (-20), 	},	/* 2 魔理沙 A */
	{	( -4),		 (-4),	 (-22), 	 (-22), 	},	/* 3 魔理沙 B */
	{	(-15),		(-15),	 (-29), 	 (-29), 	},	/* 4 レミリア */	 /* 前方配置 */
	{	(  7),		  (7),	  (-7), 	  (-7), 	},	/* 5 幽々子 */
	{	( 17),		 (17),	  (12), 	  (12), 	},	/* 6 チルノ A */
	{	( 17),		 (17),	  (12), 	  (12), 	},	/* 7 チルノ Q */
													/* 低速時 (X座標位置オフセット) */
	{	( 12),		 (-12),   (24), 	 (-24), 	},	/* 0 霊夢 A */
	{	( 12),		 (-12),   (3),		  (-3), 	},	/* 1 霊夢 B */
	{	( 12),		 (-12),   (3),		  (-3), 	},	/* 2 魔理沙 A */
	{	(  9),		  (-9),   (6),		  (-6), 	},	/* 3 魔理沙 B (Type B 貫通装備) */
	//	[3]右外 	[0]左外 [2]右内 	[1]左内
	{	( 24),		 (-24),   (6),		  (-6), 	},	/* 4 レミリア */
	{	( 18),		 (-18),  (30),		 (-30), 	},	/* 5 幽々子 */
	{	( 24),		 (-24),   (6),		  (-6), 	},	/* 6 チルノ A */
	{	( 24),		 (-24),   (6),		  (-6), 	},	/* 7 チルノ Q */
													/* 低速時 (Y座標位置オフセット) */
	{	( 29),		 (29),	 (15),		 (15),		},	/* 0 霊夢 A */
	{	(-12),		(-12),	(-18),		(-18),		},	/* 1 霊夢 B */
	{	(-12),		(-12),	(-18),		(-18),		},	/* 2 魔理沙 A */
	{	( -4),		 (-4),	(-22),		(-22),		},	/* 3 魔理沙 B */
	{	(-15),		(-15),	(-29),		(-29),		},	/* 4 レミリア */	/* 前方配置 */
	{	( 18),		 (18),	  (4),		  (4),		},	/* 5 幽々子 */
	{	( 17),		 (17),	 (12),		 (12),		},	/* 6 チルノ A */
	{	( 17),		 (17),	 (12),		 (12),		},	/* 7 チルノ Q */
	/* 固定用 */
	//
	/* 回転用 */
	{	(20),  ( 30),	(40),  ( 50) }, 								/* 1 FORMATION_30_RADIUS_R: 回転半径 */
	{	( 1),  (  2),	(-3),  ( -6) }, 								/* 0 FORMATION_31_ADD_R:	回転方向、角度加算値 */
};
//	{	( 1*104+20),  ( 2*104+20),	( 0*104+20),  ( 3*104+20) },		/* 2 FORMATION_TYPE_02_LOCATE_X: 画面後方から支援するよ(width352[pixel]) */
//	{  (240),  (240),  (220),  (220) }, 								/* 3 FORMATION_TYPE_02_LOCATE_Y: 画面後方から支援するよ */

//	{	t256(15),  t256(-15),	t256(25),  t256(-25) }, 				/* FORMATION_TYPE_00:			旧FORMATION_TYPE_00_LOCATE_X */
//	{	t256(15),  t256( 15),	t256(20),  t256( 20) }, 				/* FORMATION_TYPE_00:			旧FORMATION_TYPE_00_LOCATE_Y */
//	{	( 1*115+10),  ( 2*115+10),	( 0*115+10),  ( 3*115+10) },		/* FORMATION_TYPE_02:			画面後方から支援するよ(wideth380[pixel]) */


/*---------------------------------------------------------

---------------------------------------------------------*/
static void move_delta_xy(OBJ *src, int delta_x256, int delta_y256)
{	/* 目標地点に移動する */
//	int ccc_angle1024 = at an_1024(((delta_y256)>>8), ((delta_x256)>>8));
//	int ccc_speed256 = (/*10*/((REMILIA==(cg_game_select_player))?t256(8.0):t256(1.0)));

	REG_00_SRC_X	= (((delta_x256)>>8));			/*(狙い先)*/
	REG_01_SRC_Y	= (((delta_y256)>>8));			/*(狙い先)*/
	REG_02_DEST_X	= (0);							/*(狙い元)*/
	REG_03_DEST_Y	= (0);							/*(狙い元)*/
	tmp_angleCCW65536_src_nerai();
	const int cc_angle65536 = HATSUDAN_03_angle65536;
	const int bbb_speed_tbl[(8)] =/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
	{	t256(4.0),	t256(4.0),	t256(4.0),	t256(4.0),	t256(8.0),	t256(2.0),	t256(1.0),	t256(1.0)	};
	const int ccc_speed256 = bbb_speed_tbl[(cg_game_select_player)];
	#if (0)//
	src->cx256 += ((si n1024((ccc_angle1024))*(ccc_speed256))>>8);/*fps_factor*/	/* CCWの場合 */
	src->cy256 += ((co s1024((ccc_angle1024))*(ccc_speed256))>>8);/*fps_factor*/
	//#el se
	{
		int sin_value_t256; 	//	sin_value_t256 = 0;
		int cos_value_t256; 	//	cos_value_t256 = 0;
		int256_si nco s1024( (deg65536to1024(cc_angle65536)), &sin_value_t256, &cos_value_t256);	/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
		src->cx256 += ((sin_value_t256*(ccc_speed256))>>8);/*fps_factor*/
		src->cy256 += ((cos_value_t256*(ccc_speed256))>>8);/*fps_factor*/
	}
	#endif
	//------------------
	HATSUDAN_01_speed256	= (ccc_speed256);
	HATSUDAN_03_angle65536	= (cc_angle65536);
	sincos256();/*(破壊レジスタ多いので注意)*/
	src->cx256 += REG_03_DEST_Y;//sin_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
	src->cy256 += REG_02_DEST_X;//cos_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
	//------------------

}
/*---------------------------------------------------------

---------------------------------------------------------*/
#define remi_offset_r		(src->PL_OPTION_DATA_offset_x256)	/* FORMATION_TYPE_01: 円の半径と兼用 */
#define remi_offset_add_r	(src->PL_OPTION_DATA_offset_y256)	/* FORMATION_TYPE_01: 円の回転方向、角度加算値と兼用 */

/*---------------------------------------------------------
	FORMATION_TYPE_00: レミリアの直前に四つ / チルノの直後に四人
	FORMATION_TYPE_02: 画面後方から支援するよ
---------------------------------------------------------*/
//	static u8 cg.player_option_mode = 0;
#define control_formation00 control_formation00_and_02
#define control_formation02 control_formation00_and_02

static void control_formation00_and_02(OBJ *src) /* FORMATION_TYPE_00 */ /* FORMATION_TYPE_02 */
{
	int player_offs_x256;
	int player_offs_y256;
	if (FORMATION_TYPE_00==cg.player_option_mode)
	{
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER];
		#if 1/*Gu(中心座標)*/
		player_offs_x256 = zzz_player->cx256;
		player_offs_y256 = zzz_player->cy256;
		#endif
		/* 初期化 */
		{
		//	/* FORMATION_TYPE_00: レミリアの直前に四つ / チルノの直後に四人 */
			const int if_teisoku = ((psp_pad.pad_data & PSP_KEY_SLOW))?( (8+8)):(0);
			src->PL_OPTION_DATA_offset_x256 = ((ggg[if_teisoku+(cg_game_select_player)	  ][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);
			src->PL_OPTION_DATA_offset_y256 = ((ggg[if_teisoku+(cg_game_select_player)+(8)][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);/*240*/ /* プレイヤー番号に合わせている事に注意 */
		}
	}
	else
//	if (FORMATION_TYPE_02==cg.player_option_mode)
	{
		player_offs_x256 = (0);
		player_offs_y256 = (0);
		/* 初期化 */
		{
			static const s16 aaa[(2+2)][4] =
			{
				{
					(GAME_X_OFFSET+ (1*104) - 20),
					(GAME_X_OFFSET+ (2*104) + 20),
					(GAME_X_OFFSET+ (0*104) + 20),
					(GAME_X_OFFSET+ (3*104) - 20)
				},		/* 2 FORMATION_TYPE_02_LOCATE_X: 画面後方から支援するよ(width352[pixel]) */
				{
					(240),
					(240),
					(220),
					(220)
				},								/* 3 FORMATION_TYPE_02_LOCATE_Y: 画面後方から支援するよ */
				/* 低速用 */
				{
					(GAME_X_OFFSET+ (1*70) + 100   -32),
					(GAME_X_OFFSET+ (2*70) +  70   -32),
					(GAME_X_OFFSET+ (0*70) + 100   -32),
					(GAME_X_OFFSET+ (3*70) +  70   -32)
				},								/* 2 FORMATION_TYPE_02_LOCATE_X: 画面後方から支援するよ(width352[pixel]) */
				{
					(240),
					(240),
					(220),
					(220)
				},								/* 3 FORMATION_TYPE_02_LOCATE_Y: 画面後方から支援するよ */
			};
			const int if_teisoku = ((psp_pad.pad_data & PSP_KEY_SLOW))?( (2)):(0);
			src->PL_OPTION_DATA_offset_x256 = ((aaa[if_teisoku+0][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);
			src->PL_OPTION_DATA_offset_y256 = ((aaa[if_teisoku+1][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);/*240*/ /* プレイヤー番号に合わせている事に注意 */
		}
	}

	/* 目標地点へ移動中 */
	{
		int delta_y256	= (0);				/* delta_y : y座標における目標地点と現在地の差 */
		int delta_x256	= (0);				/* delta_x : x座標における目標地点と現在地の差 */
		{
	//	/* FORMATION_TYPE_00: レミリアの直前に四つ / チルノの直後に四人 */
	//	/* FORMATION_TYPE_02: 画面後方から支援するよ */
			#if 1/*Gu(中心座標)*/
			delta_x256 = (-src->cx256+src->PL_OPTION_DATA_offset_x256);
			delta_y256 = (-src->cy256+src->PL_OPTION_DATA_offset_y256);/*240*/
			#endif
		//	not_break;
			delta_y256 += player_offs_y256;
			delta_x256 += player_offs_x256;
	//		break;
		}
		move_delta_xy(src, delta_x256, delta_y256); 	/* 目標地点に移動する */
	#if 1
		/* レミリア用 */
//		if (REMILIA==(cg_game_select_player))
		{
			/* 目標付近までたどり着いたら */
			if (( t256(-5) < delta_y256) &&
				(delta_y256 < t256(5)) )
			{
				/* 位置の維持 & 配置完了flag */
			//	/* FORMATION_TYPE_00: レミリアの直前に四つ / チルノの直後に四人 */
			//	/* FORMATION_TYPE_02: 画面後方から支援するよ */
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
static void control_formation01(OBJ *src) /* FORMATION_TYPE_01 */
{
	int player_offs_x256;
	int player_offs_y256;
	{
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER];
		#if 1/*Gu(中心座標)*/
		player_offs_x256 = zzz_player->cx256;
		player_offs_y256 = zzz_player->cy256;
		#endif
	}
	/* 初期化 */
	{
	//	/* FORMATION_TYPE_01: レミリアの周りを回るよ */
			src->PL_OPTION_DATA_offset_x256 = ggg[FORMATION_30_RADIUS_R][(src->PL_OPTION_DATA_opt_anime_add_id)];/*40*/ /* 円の半径と兼用 */
			src->PL_OPTION_DATA_offset_y256 = ggg[FORMATION_31_ADD_R   ][(src->PL_OPTION_DATA_opt_anime_add_id)];		/* 円の回転方向、角度加算値と兼用 */
	}
	/* 目標地点へ移動中 */
	{
		int delta_y256; 			/* delta_y : y座標における目標地点と現在地点との差 */
		int delta_x256; 			/* delta_x : x座標における目標地点と現在地点との差 */
		//	delta_y256 = 0;
		//	delta_x256 = 0;
	//	/* FORMATION_TYPE_01: レミリアの周りを回るよ */
		/*REMILIA_angle512*/
		//------------------
		HATSUDAN_01_speed256	= ((remi_offset_r)<<8);
		HATSUDAN_03_angle65536	= deg1024to65536((src->PL_OPTION_DATA_angleCCW1024));
		sincos256();/*(破壊レジスタ多いので注意)*/
		delta_x256 = (player_offs_x256-src->cx256) + REG_03_DEST_Y;//sin_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
		delta_y256 = (player_offs_y256-src->cy256) + REG_02_DEST_X;//cos_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
		//------------------
		move_delta_xy(src, delta_x256, delta_y256); /* 目標地点に移動する */
	#if 1
		/* レミリア用 */
//		if (REMILIA==(cg_game_select_player))
		{
			/* 目標付近までたどり着いたら */
			if (( t256(-5) < delta_y256) &&
				(delta_y256 < t256(5)) )
			{
			/* 位置の維持 & 配置完了flag */
		/* FORMATION_TYPE_01: レミリアの周りを回るよ */
		/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024 += remi_offset_add_r;	/*5==ra d2deg512(0.05)*/	/*fps_factor*/
		mask1024(/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024);
		/*REMILIA_angle512*/
		//------------------
		HATSUDAN_01_speed256	= ((remi_offset_r)<<8);
		HATSUDAN_03_angle65536	= deg1024to65536((src->PL_OPTION_DATA_angleCCW1024));
		sincos256();/*(破壊レジスタ多いので注意)*/
		src->cx256 = (player_offs_x256) + REG_03_DEST_Y;//sin_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
		src->cy256 = (player_offs_y256) + REG_02_DEST_X;//cos_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
		//------------------
			}
		}
	#endif
	}
}

			#if 0
			/* レミリア用 */
			if (REMILIA==(cg_game_select_player))
			{
				/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024 += remi_offset_add_r;	/*5==ra d2deg512(0.05)*/	/*fps_factor*/
				mask1024(/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024);
			}
			#endif


/*---------------------------------------------------------

---------------------------------------------------------*/

static void control_formation03(OBJ *src) /* FORMATION_TYPE_03 */
{
	int player_offs_x256;
	int player_offs_y256;
	{
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER];
		player_offs_x256 = zzz_player->cx256;
		player_offs_y256 = zzz_player->cy256;
	}
//	/* 初期化 */
//	/* 目標地点へ移動中 */
	{
		int delta_y256 = (0);				/* delta_y : y座標における目標地点と現在地の差 */
		int delta_x256 = (0);				/* delta_x : x座標における目標地点と現在地の差 */
		/* FORMATION_TYPE_03: 減速時用(好きな位置に置けるよ) */
		delta_y256 = (player_offs_y256) - (src->cy256);
		delta_x256 = (player_offs_x256) - (src->cx256);
		move_delta_xy(src, delta_x256, delta_y256); /* 目標地点に移動する */
		#if 1
		/* レミリア用 */
//		if (REMILIA==(cg_game_select_player))
		{
			/* 目標付近までたどり着いたら */
			if (( t256(-5) < delta_y256) &&
				(delta_y256 < t256(5)) )
			{
//				/* 位置の維持 & 配置完了flag */
				/* FORMATION_TYPE_03: 減速時用(好きな位置に置けるよ) */
				src->cx256 = player_offs_x256;
				src->cy256 = player_offs_y256;
			}
		}
		#endif
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/ void player_control_option(OBJ *src)
{
	unsigned int slow_remilia_flag;
	slow_remilia_flag = 0;
	/* 低速移動 */
	if (REMILIA==(cg_game_select_player))
	{
		if (FORMATION_TYPE_03==cg.player_option_mode)/* FORMATION_TYPE_03: 減速時用(好きな位置に置けるよ) */
		{
			if (psp_pad.pad_data & PSP_KEY_SLOW)
			{
				/* レミリア用 */
				src->PL_OPTION_DATA_slow_count += (1<<2);
				if (
					(src->PL_OPTION_DATA_slow_count < ((src->PL_OPTION_DATA_opt_anime_add_id+1)<<(6+2))/* *30 64*/))
				{
					OBJ *zzz_player;
					zzz_player = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER];
					/* FORMATION_TYPE_03: 減速時用(好きな位置に置けるよ) */
					src->cx256 = zzz_player->cx256;
					src->cy256 = zzz_player->cy256;
				}
			}
			else
			{
				/* レミリア用 */
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
//	if (1==slow_remilia_flag)	/* pspは0レジスタがあるので0と比較したほうが速い */
	/* 少なくとも低速移動でオプションが止められるのはレミリアのみ(チルノには合わない) */
	if (0!=slow_remilia_flag)
	{
		;/* レミリア＆低速移動中 */
	}
	else	/* それ以外。(他キャラ + レミリア＆通常速移動中) */
	{
		/* フォーメーション変更 */
		/* OPTION_C1 の場合のみキー入力受け付け */
		if (OPTION_C1==src->PL_OPTION_DATA_opt_anime_add_id) //opt_anime_add_id==1によるflag管理
		{
			/* (オプションキーキーを離した瞬間なら) */
			if ((0==(psp_pad.pad_data & PSP_KEY_OPTION)))	/* オプションキーが離されている */
			{
				if ((psp_pad.pad_data^psp_pad.pad_data_alter) & PSP_KEY_OPTION)	/* オプションキーの状態が変わった(トグル、押したまたは離した場合) */
				{
					cg.player_option_mode++;
					cg.player_option_mode &= (4-1);//if (FORMATION_TYPE_99_MAX == cg.player_option_mode)	{	 cg.player_option_mode = FORMATION_TYPE_00;}
				}
			}
		}
		{
			static /*const*/ void (*formation_call_table[(4)])(OBJ *src) =
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
	プレイヤー、オプションの移動(霊夢、魔理沙、仮幽々子)
---------------------------------------------------------*/

/*---------------------------------------------------------
	プレイヤー、オプションの定義
---------------------------------------------------------*/
/* static */extern void player_move_option(OBJ *src);

global/*static*/ void jiki_option_create(void)
{
	unsigned int jj;
	for (jj=0; jj<(OPTION_04_MAX); jj++)
	{
		OBJ *h;
		h						= obj_add_Ann_direct(OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_04_JIKI_OPTION0/* +OPTION_C1 */+jj);	/* 必ず登録できる。 */
		h->jyumyou				= JYUMYOU_MUGEN;/* 時間で自動消去しない */
		{
		//	h->obj_type_set 				= (JIKI_OPTION_00_00)+kk; kk += (16);/* オプションインターリーブ */ /* 8 */
			h->obj_type_set 				= (JIKI_OPTION_00_00)+(jj<<4); /* オプションインターリーブ */ /* 8 */
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
			#if /*(ステージ開始時に再設定されるので要らない)*/0/* Gu(中心座標) */
			h->cx256					= (s1->cx256);	/* プレイヤーオプション位置の初期化 */
			h->cy256					= (s1->cy256);	/* プレイヤーオプション位置の初期化 */
			#endif
			h->m_Hit256R				= TAMA_ATARI_BULLET_DUMMY;
		//	h->PL_OPTION_DATA_yuukou_flag 			= (PL_OPTION_FLAG_01_OPTION_ON);
			h->PL_OPTION_DATA_yuukou_flag 			= (PL_OPTION_FLAG_00_OPTION_OFF); 	/* 可視フラグのOFF(不可視) */
			h->atari_hantei 			= ATARI_HANTEI_OFF; 				/* あたり判定のOFF(無敵) */

//	/* ??? */ h->kougeki_ti 			= (0/* 1 8*5 */);/*(オプションが雑魚に体当たりした場合の強さ。1なら倒せる。あたり判定はあるが0なら倒せない。)*/
			{
			//
				h->callback_mover		= player_move_option;
			//
			//	h->PL_OPTION_DATA_next					= obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER];
			//	h->P L_OPTION_DATA_state2				= 0;
				h->PL_OPTION_DATA_offset_x256			= t256(0);
				h->PL_OPTION_DATA_offset_y256			= t256(0);
			//	h->PL_OPTION_DATA_ccc_angle512			= cv1024r((0));
				h->PL_OPTION_DATA_slow_count			= 0;
			}
			h->PL_OPTION_DATA_opt_shot_interval 	= 0;
		//	h->PL_OPTION_DATA_state 				= 0;
		//	h->PL_OPTION_DATA_state1				= 0;///
			h->PL_OPTION_DATA_anime_wait			= 0;
			/* レミリアのオプションは半透明っぽい */
			if (REMILIA==(cg_game_select_player))
			{
			//	h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x96);	/*	h->alpha			= 0x96; */	/* 明る過ぎる */
				h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x80);	/*	h->alpha			= 0x80; */	/* 半透明 */
			//	h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	/*	h->alpha			= 0x50; */	/* 暗ら過ぎる */
			}
		}
	}
}












	#if (0)/*デバッグ用*/
	/*	★「ボスと相打ちするとハングアップ」バグ(～r26)対策 */
	/*(ボス倒す時に喰らいボム状態?だとstartすら効かなくなるバグがある。がオプションキーは効く)*/
	/* パネルのスコア欄にmove_typeを、グレイズ欄にcg.player_option_modeを表示させる。っていうか書き換えちゃう。 */
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
