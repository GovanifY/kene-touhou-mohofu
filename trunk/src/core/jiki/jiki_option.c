
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#include "jiki_local.h"

/*---------------------------------------------------------
	プレイヤー、オプションの移動(レミリア ＆ チルノ)
---------------------------------------------------------*/

enum	/* _remilia_formation_ */
{
	FORMATION_00 = 0,
	FORMATION_01,
	FORMATION_02,
	FORMATION_03,
};
/*
	目標:st ate1
	FORMATION_00: レミリアの直前に四つ / チルノの直後に四人
	FORMATION_01: レミリアの周りを回るよ
	FORMATION_02: 画面後方から支援するよ (y=250の位置に散らばる)
	FORMATION_03: 減速時用(好きな位置に置けるよ) (レミリアに重なるように。slowを押すと段階的にoption停止)
*/
				enum
				{	/* 通常時 */
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
/* 4 */ 			FORMATION_0c_LOCATE_Y_REMILIA,	/* 4 */ 	/* == 4 == (旧2) == REMILIA プレイヤー番号に合わせている事に注意 */
/* 5 */ 			FORMATION_0d_LOCATE_Y_YUYUKO,
/* 6 */ 			FORMATION_0e_LOCATE_Y_CIRNO_A,	/* 6 */ 	/* == 6 == (旧3) == CIRNO	プレイヤー番号に合わせている事に注意 */
/* 7 */ 			FORMATION_0f_LOCATE_Y_CIRNO_Q,	/* 7 */ 	/* == 7 == (旧3) == CIRNO	プレイヤー番号に合わせている事に注意 */
//					/* 低速時 */
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
/* 4 */ 			FORMATION_1c_LOCATE_Y_REMILIA,	/* 4 */ 	/* == 4 == (旧2) == REMILIA プレイヤー番号に合わせている事に注意 */
/* 5 */ 			FORMATION_1d_LOCATE_Y_YUYUKO,
/* 6 */ 			FORMATION_1e_LOCATE_Y_CIRNO_A,	/* 6 */ 	/* == 6 == (旧3) == CIRNO	プレイヤー番号に合わせている事に注意 */
/* 7 */ 			FORMATION_1f_LOCATE_Y_CIRNO_Q,	/* 7 */ 	/* == 7 == (旧3) == CIRNO	プレイヤー番号に合わせている事に注意 */
					/* 固定用 */
				//	FORMATION_02_LOCATE_X,			/* 2 */
				//	FORMATION_02_LOCATE_Y,			/* 3 */
					/* 回転用 */
					FORMATION_30_RADIUS_R,			/* 1 */
					FORMATION_31_ADD_R, 			/* 0 */
					FORMATION_99_MAX					/* 8 */ 	/* 最大数 */
				};
//				enum
//				{
//					FORMATION_MAX					/* 8 */ 	/* 最大数 */
//				};
				//	{	t256(15),  t256(-15),	t256(25),  t256(-25) }, 				/* FORMATION_00:			旧FORMATION_00_LOCATE_X */
				//	{	t256(15),  t256( 15),	t256(20),  t256( 20) }, 				/* FORMATION_00:			旧FORMATION_00_LOCATE_Y */
//				//	{	( 1*115+10),  ( 2*115+10),	( 0*115+10),  ( 3*115+10) },		/* FORMATION_02:			画面後方から支援するよ(wideth380dot) */
				static const s8 ggg[FORMATION_99_MAX][4] =
				{
//#define JIKI_X_HARF (14)/*(31/2) 自機の半分 */
/* 通常時 (X座標位置オフセット) (6の倍数が基本位置, 75%縮小の為 8の倍数の75%が6の倍数になる)6, 12, 18, 24, 30, 36, 42, 48, 54, 60, 66, 72, ... */
{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 0 霊夢 A */
{	( 24),		 (-24),  ( 12), 	  (-12),	},	/* 1 霊夢 B */
{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 2 魔理沙 A */
{	( 54),		 (-54),  ( 24), 	  (-24),	},	/* 3 魔理沙 B (Type B 貫通装備) */
//	[3]右外 	[0]左外 [2]右内 	[1]左内
{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 4 レミリア */
{	( 24),		 (-24),  ( 24), 	  (-24),	},	/* 5 幽々子 */
{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 6 チルノ */
{	( 24),		 (-24),  (	6), 	   (-6),	},	/* 7 チルノ */
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
{	( 24),		 (-24),   (6),		  (-6), 	},	/* 6 チルノ */
{	( 24),		 (-24),   (6),		  (-6), 	},	/* 7 チルノ */
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
			//		{	( 1*104+20),  ( 2*104+20),	( 0*104+20),  ( 3*104+20) },		/* 2 FORMATION_02_LOCATE_X: 画面後方から支援するよ(width352dot) */
			//		{  (240),  (240),  (220),  (220) }, 								/* 3 FORMATION_02_LOCATE_Y: 画面後方から支援するよ */
				};


/*---------------------------------------------------------

---------------------------------------------------------*/
static void move_delta_xy(SPRITE *src, int delta_x256, int delta_y256)
{	/* 目標地点に移動する */
	int ccc_angle1024;
//	ccc_angle1024 = at an_1024(((delta_y256)>>8), ((delta_x256)>>8));
	ccc_angle1024 = atan_65536(((delta_y256)>>8), ((delta_x256)>>8));
	ccc_angle1024 >>= (6);

//	int ccc_speed256;
//	ccc_speed256 = (/*10*/((REMILIA==(cg_game_select_player))?t256(8.0):t256(1.0)));
	const int bbb_speed_tbl[(8)] =/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
	{	t256(4.0),	t256(4.0),	t256(4.0),	t256(4.0),	t256(8.0),	t256(2.0),	t256(1.0),	t256(1.0)	};
	const int ccc_speed256 = bbb_speed_tbl[(cg_game_select_player)];
	src->cx256 += ((sin1024(( ccc_angle1024))*(ccc_speed256))>>8);/*fps_factor*/	/* CCWの場合 */
	src->cy256 += ((cos1024(( ccc_angle1024))*(ccc_speed256))>>8);/*fps_factor*/
}
/*---------------------------------------------------------

---------------------------------------------------------*/
#define oz_offset_r 	(src->PL_OPTION_DATA_offset_x256)	/* FORMATION_01: 円の半径と兼用 */
#define oz_offset_add_r (src->PL_OPTION_DATA_offset_y256)	/* FORMATION_01: 円の回転方向、角度加算値と兼用 */

/*---------------------------------------------------------
	FORMATION_00: レミリアの直前に四つ / チルノの直後に四人
	FORMATION_02: 画面後方から支援するよ
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
		/* 初期化 */
		{
			static const s16 aaa[(2+2)][4] =
			{
				{	( 1*104-20),  ( 2*104+20),	( 0*104+20),  ( 3*104-20) },		/* 2 FORMATION_02_LOCATE_X: 画面後方から支援するよ(width352dot) */
				{  (240),  (240),  (220),  (220) }, 								/* 3 FORMATION_02_LOCATE_Y: 画面後方から支援するよ */
				/* 低速用 */
				{	( 170),  ( 210),	( 100),  ( 280) },		/* 2 FORMATION_02_LOCATE_X: 画面後方から支援するよ(width352dot) */
				{  (240),  (240),  (220),  (220) }, 								/* 3 FORMATION_02_LOCATE_Y: 画面後方から支援するよ */
			};
			const int if_teisoku = ((psp_pad.pad_data & PSP_KEY_SLOW))?( (2)):(0);
			src->PL_OPTION_DATA_offset_x256 = ((aaa[if_teisoku+0][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);
			src->PL_OPTION_DATA_offset_y256 = ((aaa[if_teisoku+1][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);/*240*/ /* プレイヤー番号に合わせている事に注意 */
		}
	}
	else
	{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		#if 1/*Gu(中心座標)*/
		player_offs_x256 = zzz_player->cx256;
		player_offs_y256 = zzz_player->cy256;
		#endif
		/* 初期化 */
		{
		//	/* FORMATION_00: レミリアの直前に四つ / チルノの直後に四人 */
			const int if_teisoku = ((psp_pad.pad_data & PSP_KEY_SLOW))?( (8+8)):(0);
			src->PL_OPTION_DATA_offset_x256 = ((ggg[if_teisoku+(cg_game_select_player)	][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);
			src->PL_OPTION_DATA_offset_y256 = ((ggg[if_teisoku+(cg_game_select_player)+(8)][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);/*240*/ /* プレイヤー番号に合わせている事に注意 */
		}
	}

	/* 目標地点へ移動中 */
	{
		int delta_y256=0;				/* delta_y : y座標における目標地点と現在地の差 */
		int delta_x256=0;				/* delta_x : x座標における目標地点と現在地の差 */
		{
	//	/* FORMATION_00: レミリアの直前に四つ / チルノの直後に四人 */
	//	/* FORMATION_02: 画面後方から支援するよ */
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
			//	/* FORMATION_00: レミリアの直前に四つ / チルノの直後に四人 */
			//	/* FORMATION_02: 画面後方から支援するよ */
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
		#if 1/*Gu(中心座標)*/
		player_offs_x256 = zzz_player->cx256;
		player_offs_y256 = zzz_player->cy256;
		#endif
	}
	/* 初期化 */
	{
	//	/* FORMATION_01: レミリアの周りを回るよ */
			src->PL_OPTION_DATA_offset_x256 = ggg[FORMATION_30_RADIUS_R][(src->PL_OPTION_DATA_opt_anime_add_id)];/*40*/ /* 円の半径と兼用 */
			src->PL_OPTION_DATA_offset_y256 = ggg[FORMATION_31_ADD_R   ][(src->PL_OPTION_DATA_opt_anime_add_id)];		/* 円の回転方向、角度加算値と兼用 */
	}
	/* 目標地点へ移動中 */
	{
		int delta_y256=0;				/* delta_y : y座標における目標地点と現在地の差 */
		int delta_x256=0;				/* delta_x : x座標における目標地点と現在地の差 */
		{
	//	/* FORMATION_01: レミリアの周りを回るよ */
			delta_x256 = player_offs_x256-src->cx256 + (sin1024((src->PL_OPTION_DATA_angleCCW1024/*REMILIA_angle512*/))*oz_offset_r);		/* CCWの場合 */
			delta_y256 = player_offs_y256-src->cy256 + (cos1024((src->PL_OPTION_DATA_angleCCW1024/*REMILIA_angle512*/))*oz_offset_r);
		}
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
		/* FORMATION_01: レミリアの周りを回るよ */
		/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024 += oz_offset_add_r;	/*5==ra d2deg512(0.05)*/	/*fps_factor*/
		mask1024(/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024);
		src->cx256 = player_offs_x256 + sin1024((src->PL_OPTION_DATA_angleCCW1024/*REMILIA_angle512*/))*oz_offset_r; /* CCWの場合 */
		src->cy256 = player_offs_y256 + cos1024((src->PL_OPTION_DATA_angleCCW1024/*REMILIA_angle512*/))*oz_offset_r;
			}
		}
	#endif
	}
}
			#if 0
			/* レミリア用 */
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
		#if 1/*Gu(中心座標)*/
		player_offs_x256 = zzz_player->cx256;
		player_offs_y256 = zzz_player->cy256;
		#endif
	}
//	/* 初期化 */
//	/* 目標地点へ移動中 */
	{
		int delta_y256=0;				/* delta_y : y座標における目標地点と現在地の差 */
		int delta_x256=0;				/* delta_x : x座標における目標地点と現在地の差 */
	//	switch ( state_zz1)
		{
	//	/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
			delta_y256 = player_offs_y256-src->cy256;
			delta_x256 = player_offs_x256-src->cx256;
	//		break;
		}
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
				/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
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
		#if 1/*Gu(中心座標)*/
		player_offs_x256 = zzz_player->cx256;
		player_offs_y256 = zzz_player->cy256;
		#endif
	}
	/* 少なくとも低速移動でオプションが止められるのはレミリアのみ(チルノには合わない) */
	if (REMILIA==(cg_game_select_player))
	{
		if (state_zz1==FORMATION_03)/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
		{
			if (psp_pad.pad_data & PSP_KEY_SLOW)
			{
				/* レミリア用 */
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
				/* レミリア用 */
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
	/* 低速移動 */
	if (REMILIA==(cg_game_select_player))
	{
		if (psp_pad.pad_data & PSP_KEY_SLOW)
		{
			slow_remilia_flag=1;
		}
	}
//	if (1==slow_remilia_flag)	/* pspは0レジスタがあるので0と比較したほうが速い */
	if (0!=slow_remilia_flag)	/* 少なくとも低速移動でオプションが止められるのはレミリアのみ(チルノには合わない) */
	{
		;
	}
	else
	{
	//
		/* フォーメーション変更 */
		/* OPTION_C1 の場合のみキー入力受け付け */
		if (OPTION_C1==src->PL_OPTION_DATA_opt_anime_add_id) //opt_anime_add_id==1によるflag管理
		{
		//	if (my_pad & PSP_KEY_OPTION)
		/* (オプションキーキーを離した瞬間なら) */
			if ((0==(psp_pad.pad_data & PSP_KEY_OPTION)))	/* オプションキーが離されている */
			{
				if ( (psp_pad.pad_data^psp_pad.pad_data_alter) & PSP_KEY_OPTION)	/* オプションキーの状態が変わった(トグル、押したまたは離した場合) */
				{
	#if 0/*デバッグ用*/
	/*	★「ボスと相打ちするとハングアップ」バグ(～r26)対策 */
	/*(ボス倒す時に喰らいボム状態?だとstartすら効かなくなるバグがある。がオプションキーは効く)*/
	/* パネルのスコア欄にmove_typeを、グレイズ欄にstate_zz1を表示させる。っていうか書き換えちゃう。 */
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
