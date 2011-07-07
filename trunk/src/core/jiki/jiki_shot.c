
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	各プレイヤー(霊夢 ＆ 魔理沙 ＆ レミリア ＆ 幽々子 ＆ チルノ)
	REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q)
	-------------------------------------------------------
---------------------------------------------------------*/

#include "jiki_local.h"

/* 霊夢 A (座布団巫女) / 魔理沙 A (森魔法使い) 用の誘導目標座標 */
#define PL_SHOT_DATA_target_x256		user_data00
#define PL_SHOT_DATA_target_y256		user_data01


enum
{
//	/* (誘導ショット) */
	REIMU_SHOT_TYPE_00 = 0,
	/* (非誘導ショット) */
	REIMU_SHOT_TYPE_01,
	REIMU_SHOT_TYPE_02,
	REIMU_SHOT_TYPE_03,
	REIMU_SHOT_TYPE_04,
	REIMU_SHOT_TYPE_05,
	REIMU_SHOT_TYPE_06,
	REIMU_SHOT_TYPE_07,
//	/* 非誘導ショット(幽々子) */
	YUYUKO_SHOT_TYPE_08,
	YUYUKO_SHOT_TYPE_09,
	YUYUKO_SHOT_TYPE_0a,
	YUYUKO_SHOT_TYPE_0b,
//	/* 針(サブショット) */
	NEEDLE_ANGLE_270,
	NEEDLE_ANGLE_263,	/* レミリア 用 */
	NEEDLE_ANGLE_277,	/* レミリア 用 */
	NEEDLE_ANGLE_271,	/* 幽々子 用 */
//
	REIMU_SHOT_TYPE_MAX 	/* 最大数 */	//	NEEDLE_MAX
};

/*static*/extern int cg_jiki_weapon_level_offset; /* 装備した武器レベル(番号)の８倍にプレイヤー値を足した値をオフセットとして保持 */
/*
メモ
;-------------------
霊夢 霊符	ショット（ホーミングアミュレット）		ボム （夢想封印）

L0	1way(まっすぐ中)								P000-P007			(直進1way)
オプション2つ追加									P008-P015
L1	2way(右上、左上)								P016-	28			(間隔が狭い2way)

L3	3way(まっすぐ中、右斜上、左斜上)				P032-	43			(間隔が広い3way)

Lx	3way(まっすぐ中、右斜上、左斜上)				P064-	113 	124 (間隔が広い3way)

124 オプション3方向

Lx	4way(まっすぐ右、まっすぐ左、右斜上、左斜上)			MAX(128)	(間隔が広い4way)

;-------------------
霊夢 夢符	ショット（パスウェイジョンニードル）	ボム （封魔陣）

;-------------------
魔理沙 魔符 ショット（マジックミサイル）			ボム （スターダストレヴァリエ）

;-------------------
魔理沙 恋符 ショット（イリュージョンレーザー）		ボム （マスタースパーク）

;-------------------
 */

/*---------------------------------------------------------
	プレイヤー、針弾、ショット弾の移動
	霊夢 ＆ 魔理沙 ＆ レミリア ＆ 幽々子 ＆ チルノ
---------------------------------------------------------*/


static void remilia_move_object(SPRITE *src)
{
	src->cx256 += src->vx256;		/*fps_factor*/
	src->cy256 += src->vy256;		/*fps_factor*/
//	if (REMILIA==(cg_game_select_player))	/* レミリアはゆらゆら */
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
//	if (YUYUKO==(cg_game_select_player))		/* 幽々子はそれる */
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
//	if (REIMU==(cg_game_select_player)) 		/* 霊夢の回転ショット */
//	if ((REIMU_B+1) > (cg_game_select_player))	/* 霊夢の回転ショット */
	{
		if (((JIKI_SHOT_01)|SP_GROUP_SHOT_SPECIAL)==src->type)
		{
			/* 描画用グラ回転 */
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
	REI03_status,	/* 2==霊夢の回転ショット */
	REI04_MAX		/* 最大数 */
};


//global void player_register_shot_object(SPRITE *src, int /*set_*/shot_type);
/*static*/static/*global*/ void player_register_shot_object(SPRITE *src, int /*set_*/shot_type)
{
	SPRITE *h;/* shot */
	h					= obj_add_01_teki_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		static const s8 r_tbl[REIMU_SHOT_TYPE_MAX][REI04_MAX] =
		{	/*x_offset256	y_offset256  vx256 status */
	//	/* 誘導ショット(霊夢 ＆ 魔理沙) */
	/*00*/	{ (  0),	(  0),	(	0), 	0x01},	 /*shot_regist_triple_fuda*/
	//	/* 非誘導ショット(霊夢) */
	/*01*/	{ (  0),	(  0),	(	0), 	0x00},	/* 中 まっすぐ */
	/*02*/	{ ( -5),	(  0),	(	0), 	0x00},	/* 左 まっすぐ */
	/*03*/	{ ( +5),	(  0),	(	0), 	0x00},	/* 右 まっすぐ */
	/*04*/	{ ( -5),	(  0),	( -20), 	0x00},	/* 左 左上 */
	/*05*/	{ ( +5),	(  0),	(  20), 	0x00},	/* 右 右上 */
	/*06*/	{ ( -5),	(  0),	( -35), 	0x00},	/* 左 左斜上 */
	/*07*/	{ ( +5),	(  0),	(  35), 	0x00},	/* 右 右斜上 */
	//	/* 非誘導ショット(幽々子) */
	/*08*/	{ (-15),	(-25),	( -25), 	0x00},	 /* 幽々子 */					 /*(-32)==(4*(- 8))*/
	/*09*/	{ (+15),	(-25),	( +25), 	0x00},	 /* 幽々子 */					 /*( 32)==(4*(	8))*/
	/*0a*/	{ (-25),	(-25),	( -50), 	0x00},	 /* 幽々子 */					 /*(-64)==(4*(-16))*/
	/*0b*/	{ (+25),	(-25),	( +50), 	0x00},	 /* 幽々子 */					 /*( 64)==(4*( 16))*/
	//		/* CCWの場合 */
			{ (  0),	( -7),	(	0), 	0x01},//	cv1024r((180)), 				/* 魔理沙 A/B 霊夢 レミリア ＆ チルノ */		/*t256( 0), t256( 0),*/
			{ (  0),	( -7),	(-108), 	0x01},//	cv1024r((180-25)),/*263=270-7*/ /* レミリア ＆ チルノ */ /*t256( 0), t256( 0),*/
			{ (  0),	( -7),	( 108), 	0x01},//	cv1024r((180+25)),/*277=270+7*/ /* レミリア ＆ チルノ */ /*t256( 0), t256( 0),*/
			{ (  0),	(-47),	(	5), 	0x01},//	cv1024r((181)), 				/* 幽々子専用 */	/*-48*/ /*t256( 0), t256(-40),*/
		};
	//	int shot_type;
		int shot_status;/* 針? */
		shot_status = ((r_tbl[shot_type][REI03_status]));

		int is_needle;/* 針? */
	//	is_needle = (shot_type <= NEEDLE_ANGLE_270)?(1/* 針 */):(0/* 弾 */);
		is_needle = ((shot_status)&0x01);
	//
		#define OFFS_IS_NEEDLE ((cg_game_select_player)+(is_needle<<3))
		const static u8 base_shot_atari[16] =
		{	/* メインショット(札)のあたり判定 BASE_SHOT_ATARI */
			(4),	/* 霊夢 A */		//	/*	4  5  4 */
			(4),	/* 霊夢 B */		//	/*	4  5  4 */
			(4),	/* 魔理沙 A */		//	/*	4  6  4 */
			(4),	/* 魔理沙 B */		//	/*	4  6  4 */
			(4),	/* レミリア */		//	/*	4  7  4 */
			(8),	/* 幽々子 */		//	/*	4  9  8 */
			(6),	/* チルノ A */		//	/*	6  8  6 */
			(9),	/* チルノ Q */		//	/*	8  6  4 */
			/* サブショット(針)のあたり判定 BASE_NEEDLE_ATARI */
			(6),	/* 霊夢 A */		//	/*	6  5  6 */
			(6),	/* 霊夢 B */		//	/*	6  5  6 */
			(8),	/* 魔理沙 A */		//	/*	8  6  8 */
			(8),	/* 魔理沙 B */		//	/*	8  6  8 */
		   (12),	/* レミリア */		//	/*	8  7 12 */
			(8),	/* 幽々子 */		//	/* 12  9  8 */
		   (16),	/* チルノ A */		//	/* 16  8 16 */
		   (24),	/* チルノ Q */		//	/*	8  6  8 */
		};
		h->m_Hit256R			= ((base_shot_atari[(OFFS_IS_NEEDLE)])<<8);/* あたり判定の大きさ */

	//	h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JI KI_SHOT_02|SP_GROUP_SHOT_SPECIAL)-shot_status;
		#if 1
	//	else
	//	if (0x01== shot_status)	/* 針 */
		{
			h->type 			= (JIKI_SHOT_07|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/	/*SP_PL_FIREBALL*/
		}
	//
		if (0x00 == shot_status)	/* 針以外 */
		{
			if (REIMU_B+1 > (cg_game_select_player))	/* 霊夢? */
			{
			//	if (0x02== shot_status)	/* 霊夢の回転ショット */
				if (REIMU_SHOT_TYPE_00 != shot_type)/* 誘導ショット以外？ */
				{
					h->type 			= (JIKI_SHOT_01|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/
					/* 描画用グラ回転 */
					h->rotationCCW1024	= (ra_nd() & (1024-1));/* 出だしの角度はランダムでないと(レーザーみたいな)変な画面になる */
				}
			/*	else	{	h->type = 誘導ショットは、針と同じ; 	}	*/
			}
			else
			{
				h->type 			= (JIKI_SHOT_02|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/
			}
		}
	//	else
		#endif
		#if 1/*Gu(中心座標)*/
		h->cx256			= (src->cx256) + ((r_tbl[shot_type][REI00_x_offset])<<8); /*+ x_offs*/
		h->cy256			= (src->cy256) + ((r_tbl[shot_type][REI01_y_offset])<<8);/*(20)*/
		#endif
	//	h->callback_mover	= player_move_shot;
		void (*player_move_shot_table[16])(SPRITE *src) =
		{
			reimu_a_move_shot,		/* 霊夢A */
			reimu_b_move_shot,		/* 霊夢B */
			marisa_a_move_shot, 	/* 魔理沙A */
			marisa_b_move_shot, 	/* 魔理沙B */
			remilia_move_shot,		/* レミリア */
			yuyuko_move_shot,		/* 幽々子 */
			chrno_a_move_shot,		/* チルノ A */
			chrno_q_move_shot,		/* チルノ Q */
		//
			reimu_a_move_hari,		/* 霊夢A */
			reimu_b_move_hari,		/* 霊夢B */
			marisa_a_move_hari, 	/* 魔理沙A */
			marisa_b_move_hari, 	/* 魔理沙B */
			remilia_move_hari,		/* レミリア */
			yuyuko_move_hari,		/* 幽々子 */
			chrno_a_move_hari,		/* チルノ A */
			chrno_q_move_hari,		/* チルノ Q */
		};
		h->callback_mover	= *player_move_shot_table[(OFFS_IS_NEEDLE)];
//		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x96);	/*	h->alpha		= 0x96; 150 半透明にする。 */
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
	//	#define DDD_MAX 			(PLAYERS8*2)		/* 最大数 */
		#define CCC_MAX 			(PLAYERS8*14)		/* 最大数 */
		static const u8/*u8*/ ccc_tbl[CCC_MAX] =
		{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
	//		 40, 40,  40, 40,  40,128,	40, 40, 	/* WEAPON_L0  strength ショットの強さ */ /* (L1ショットは、弾数が少ないので 40以下にしない) */
	//		 16, 24,  24, 16,  32,128,	40, 40, 	/* WEAPON_L1  strength ショットの強さ */
	//		 16, 24,  24,  8,  32,128,	40, 40, 	/* WEAPON_L2  strength ショットの強さ */
	//		 16, 24,  24,  8,  32,128,	40, 40, 	/* WEAPON_L3  strength ショットの強さ */
	//		 16, 24,  24,  8,  32,128,	40, 40, 	/* WEAPON_L4  strength ショットの強さ */
	//		 16, 24,  24,  8,  32,128,	40, 40, 	/* WEAPON_L5  strength ショットの強さ */
	//
			/*(r32はそもそも自機が強すぎるのが原因で、ボスをどう作ってもスペカ前にボスが死ぬ。それでゲームが破綻している。)*/
		//	#if 0/*(r32)(ショット速度等仕様を代えたのでr33と互換性はない)あくまでメモ */
		//	 40, 40,  40, 40,  40,128,	40, 40, 	/* WEAPON_L0  strength ショットの強さ */ /* (L1ショットは、弾数が少ないので 40以下にしない) */
		//	 20, 24,  24, 24,  37,129,	40, 40, 	/* WEAPON_L1  strength ショットの強さ */
		//	 17, 24,  24, 24,  36,130,	40, 40, 	/* WEAPON_L2  strength ショットの強さ */
		//	 16, 24,  24, 24,  35,132,	40, 40, 	/* WEAPON_L3  strength ショットの強さ */
		//	 15, 25,  24, 24,  34,136,	48, 44, 	/* WEAPON_L4  strength ショットの強さ */
		//	 14, 26,  25, 25,  33,144,	64, 60, 	/* WEAPON_L5  strength ショットの強さ */
		//	#endif
			/* 以下は弾数も考慮して調整*/
			#if 0/*(r33a)テスト版、全体的に強すぎる。 */
			 20, 20,  20, 20,  40,128,	40, 40, 	/* WEAPON_L0  strength ショットの強さ */
			 22, 24,  14, 22,  37,129,	40, 40, 	/* WEAPON_L1  strength ショットの強さ */
			 15, 15,  17, 15,  36,130,	40, 40, 	/* WEAPON_L2  strength ショットの強さ */
			 12, 14,  12, 12,  35,132,	40, 40, 	/* WEAPON_L3  strength ショットの強さ */
			 13, 13,  16, 13,  34,136,	48, 44, 	/* WEAPON_L4  strength ショットの強さ */
			 10, 12,  18, 10,  33,144,	64, 60, 	/* WEAPON_L5  strength ショットの強さ */
			#endif
			#if 0/*(r33t2)ある程度妥当(霊夢-魔理沙のみ)だが、正直まだ自機が強すぎる。(自機もボスももっと弱くするべき) */
			 10, 10,  10, 10,  40,128,	40, 40, 	/* WEAPON_L0  strength ショットの強さ */
			 11, 12,   7, 11,  37,129,	40, 40, 	/* WEAPON_L1  strength ショットの強さ */
			  7,  7,   8,  7,  36,130,	40, 40, 	/* WEAPON_L2  strength ショットの強さ */
			  6,  7,   6,  6,  35,132,	40, 40, 	/* WEAPON_L3  strength ショットの強さ */
			  6,  6,   8,  6,  34,136,	48, 44, 	/* WEAPON_L4  strength ショットの強さ */
			  5,  6,   9,  5,  33,144,	64, 60, 	/* WEAPON_L5  strength ショットの強さ */
			#endif
			#if 0/*(r33t3)誘導/針巫女対応 */
			 10, 10,  10, 10,  40,128,	40, 40, 	/* WEAPON_L0  strength ショットの強さ */
			 11,  8,   7, 11,  37,129,	40, 40, 	/* WEAPON_L1  strength ショットの強さ */
			  5,  4,   8,  7,  36,130,	40, 40, 	/* WEAPON_L2  strength ショットの強さ */
			  4,  4,   6,  6,  35,132,	40, 40, 	/* WEAPON_L3  strength ショットの強さ */
			  3,  2,   8,  6,  34,136,	48, 44, 	/* WEAPON_L4  strength ショットの強さ */
			  2,  3,   9,  5,  33,144,	64, 60, 	/* WEAPON_L5  strength ショットの強さ */
			#endif
			#if 1/*(r33t4) (霊夢 & 魔理沙) / 自機ショット弱すぎる気もしないでもないのでx2倍にしてみる。 */
			 20, 20,  20, 20,  40,128,	40, 40, 	/* WEAPON_L0  strength ショットの強さ */
			 22, 16,  14, 22,  37,129,	40, 40, 	/* WEAPON_L1  strength ショットの強さ */
			 10,  8,  16, 14,  36,130,	40, 40, 	/* WEAPON_L2  strength ショットの強さ */
			  8,  8,  12, 12,  35,132,	40, 40, 	/* WEAPON_L3  strength ショットの強さ */
			  6,  4,  16, 12,  34,136,	48, 44, 	/* WEAPON_L4  strength ショットの強さ */
			  4,  6,  18, 10,  33,144,	64, 60, 	/* WEAPON_L5  strength ショットの強さ */
			#endif

#if 0
-----------(r33t3)[針巫女(夢)は針とショットでちゃんと差を付ける]
霊夢 A(霊)ショット		  霊夢 A(霊)針(誘導)				  / 霊夢 B(夢)ショット		  霊夢 B(夢)針						  /
10 x 1 == 10 == 1.00[sum] -- x 0 ------------------ 1.00[ave] / 10 x 1 == 10 == 1.00[sum] -- x 0 ------------------- 1.00[ave] /
11 x 1 == 11 == 1.10[sum] 11 x 1 == 11 == 1.10[sum] 1.10[ave] /  8 x 1 ==  8 == 0.80[sum] 16 x 1 == 16 == 1.60[sum] 1.20[ave] /
 5 x 2 == 10 == 1.00[sum]  9 x 2 == 18 == 1.80[sum] 1.40[ave] /  4 x 2 ==  8 == 0.80[sum] 10 x 2 == 20 == 2.00[sum] 1.40[ave] /
 4 x 3 == 12 == 1.20[sum]  9 x 3 == 27 == 2.70[sum] 1.95[ave] /  4 x 3 == 12 == 1.20[sum] 10 x 3 == 30 == 3.00[sum] 2.10[ave] /
 3 x 3 ==  9 == 0.90[sum]  7 x 4 == 28 == 2.80[sum] 1.85[ave] /  2 x 4 ==  8 == 0.80[sum] 10 x 4 == 40 == 4.00[sum] 2.40[ave] /
 2 x 4 ==  8 == 0.80[sum]  8 x 4 == 32 == 3.20[sum] 2.00[ave] /  3 x 5 == 15 == 1.50[sum] 12 x 4 == 48 == 4.80[sum] 3.15[ave] /
魔理沙 A(魔)ショット	  魔理沙 A(魔)針					  / 魔理沙 B(恋)ショット	  魔理沙 B(恋)針					  /
10 x 1 == 10 == 1.00[sum] -- x 0 ------------------ 1.00[ave] / 10 x 1 == 10 == 1.00[sum] -- x 0 ------------------ 1.00[ave] /
 7 x 2 == 14 == 1.40[sum] 10 x 1 == 10 == 1.00[sum] 1.20[ave] /    x 2 ==	 == 1.00[sum] 20 x 1 == 20 == 1.00[sum] 1.00[ave] /
 8 x 2 == 16 == 1.60[sum]  7 x 2 == 14 == 1.40[sum] 1.50[ave] /    x 3 ==	 ==  . 0[sum]	 x 2 ==    ==  . 0[sum] 1.00[ave] /
 6 x 3 == 18 == 1.80[sum]  6 x 3 == 18 == 1.80[sum] 1.80[ave] /    x 4 ==	 ==  . 0[sum]	 x 3 ==    ==  . 0[sum] 1.00[ave] /
 8 x 3 == 24 == 2.40[sum]  6 x 4 == 24 == 2.40[sum] 2.40[ave] /    x 5 ==	 ==  . 0[sum]	 x 4 ==    ==  . 0[sum] 1.00[ave] /
 9 x 3 == 27 == 2.70[sum]  7 x 4 == 28 == 2.80[sum] 2.75[ave] /    x 5 ==	 ==  . 0[sum]	 x 4 ==    ==  . 0[sum] 1.00[ave] /
-----------(r33t2t3形式)[下と同じだけど(t3形式で)書き直してみる]
霊夢 A(霊)ショット		  霊夢 A(霊)針						  / 霊夢 B(夢)ショット		  霊夢 B(夢)針						  /
10 x 1 == 10 == 1.00[sum] -- x 0 ------------------ 1.00[ave] / 10 x 1 == 10 == 1.00[sum] -- x 0 ------------------ 1.00[ave] /
11 x 1 == 11 == 1.10[sum] 10 x 1 == 10 == 1.00[sum] 1.05[ave] / 12 x 1 == 12 == 1.20[sum] 10 x 1 == 10 == 1.00[sum] 1.10[ave] /
 7 x 2 == 14 == 1.40[sum]  6 x 2 == 12 == 1.20[sum] 1.30[ave] /  7 x 2 == 14 == 1.40[sum]  7 x 2 == 14 == 1.40[sum] 1.40[ave] /
 6 x 3 == 18 == 1.80[sum]  5 x 3 == 15 == 1.50[sum] 1.65[ave] /  7 x 3 == 21 == 2.10[sum]  7 x 3 == 21 == 2.10[sum] 2.10[ave] /
 6 x 3 == 18 == 1.80[sum]  4 x 4 == 16 == 1.60[sum] 1.70[ave] /  6 x 4 == 24 == 2.40[sum]  6 x 4 == 24 == 2.40[sum] 2.40[ave] /
 5 x 4 == 20 == 2.00[sum]  5 x 4 == 20 == 2.00[sum] 2.00[ave] /  6 x 5 == 30 == 3.00[sum]  8 x 4 == 32 == 3.20[sum] 3.10[ave] /
魔理沙 A(魔)ショット	  魔理沙 A(魔)針					  / 魔理沙 B(恋)ショット	  魔理沙 B(恋)針					  /
10 x 1 == 10 == 1.00[sum] -- x 0 ------------------ 1.00[ave] / 10 x 1 == 10 == 1.00[sum] -- x 0 ------------------ 1.00[ave] /
 7 x 2 == 14 == 1.40[sum] 10 x 1 == 10 == 1.00[sum] 1.20[ave] /    x 2 ==	 == 1.00[sum] 20 x 1 == 20 == 1.00[sum] 1.00[ave] /
 8 x 2 == 16 == 1.60[sum]  7 x 2 == 14 == 1.40[sum] 1.50[ave] /    x 3 ==	 ==  . 0[sum]	 x 2 ==    ==  . 0[sum] 1.00[ave] /
 6 x 3 == 18 == 1.80[sum]  6 x 3 == 18 == 1.80[sum] 1.80[ave] /    x 4 ==	 ==  . 0[sum]	 x 3 ==    ==  . 0[sum] 1.00[ave] /
 8 x 3 == 24 == 2.40[sum]  6 x 4 == 24 == 2.40[sum] 2.40[ave] /    x 5 ==	 ==  . 0[sum]	 x 4 ==    ==  . 0[sum] 1.00[ave] /
 9 x 3 == 27 == 2.70[sum]  7 x 4 == 28 == 2.80[sum] 2.75[ave] /    x 5 ==	 ==  . 0[sum]	 x 4 ==    ==  . 0[sum] 1.00[ave] /
-----------(r33t2)
霊夢 A(霊)					霊夢 B(夢)					/
10 x 1 == 10 == 1.00[total] 10 x 1 == 10 == 1.00[total] /
11 x 1 == 11 == 1.10[total] 12 x 1 == 12 == 1.20[total] /
 7 x 2 == 14 == 1.40[total]  7 x 2 == 14 == 1.40[total] /
 6 x 3 == 18 == 1.80[total]  7 x 3 == 21 == 2.10[total] /
 6 x 3 == 18 == 1.80[total]  6 x 4 == 24 == 2.40[total] /
 5 x 4 == 20 == 2.00[total]  6 x 5 == 30 == 3.00[total] /
魔理沙 A(魔)				魔理沙 B(恋)				/
10 x 1 == 10 == 1.00[total] 10 x 1 == 10 == 1.00[total] /
 7 x 2 == 14 == 1.40[total]    x 2 ==	 == 1.00[total] /
 8 x 2 == 16 == 1.60[total]    x 3 ==	 ==  . 0[total] /
 6 x 3 == 18 == 1.80[total]    x 4 ==	 ==  . 0[total] /
 8 x 3 == 24 == 2.40[total]    x 5 ==	 ==  . 0[total] /
 9 x 3 == 27 == 2.70[total]    x 5 ==	 ==  . 0[total] /
-----------(r33a)
霊夢 A(霊)					霊夢 B(夢)					/
20 x 1 == 20 == 1.00[total] 20 x 1 == 20 == 1.00[total] /
22 x 1 == 22 == 1.10[total] 24 x 1 == 24 == 1.20[total] /
15 x 2 == 30 == 1.50[total] 15 x 2 == 30 == 1.50[total] /
12 x 3 == 36 == 1.80[total] 14 x 3 == 42 == 2.10[total] /
13 x 3 == 39 == 1.95[total] 13 x 4 == 52 == 2.60[total] /
10 x 4 == 40 == 2.00[total] 12 x 5 == 60 == 3.00[total] /
魔理沙 A(魔)				魔理沙 B(恋)				/
20 x 1 == 20 == 1.00[total] 20 x 1 == 20 == 1.00[total] /
14 x 2 == 28 == 1.40[total]    x 2 ==	 == 1.00[total] /
17 x 2 == 34 == 1.70[total]    x 3 ==	 ==  . 0[total] /
12 x 3 == 36 == 1.80[total]    x 4 ==	 ==  . 0[total] /
16 x 3 == 48 == 2.40[total]    x 5 ==	 ==  . 0[total] /
18 x 3 == 54 == 2.70[total]    x 5 ==	 ==  . 0[total] /
#endif

			#if 0/*(r32)*/
			 16, 14,  10, 12,	9,	4,	 8,  9, 	/* speed	ショットの速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */
			 18, 18,  10, 10,	3,	4,	 3,  4, 	/* speed	針の速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */
			#endif
			#if 1/*(r33)*/
	// 参考 http://hossy.info/game/toho/k_chara.php
	//				 霊夢 (霊) - 誘導弾 誘導弾は通常弾と同じ速度
	//				 霊夢 (夢) - ニードル ニードルは通常弾の2倍速
	// 11==16* 0.7 魔理沙 (魔) - ミサイル 通常弾の70％。
	// ?? ??	   魔理沙 (恋) - レーザー スピード測定不能。というか全てにおいて測定困難。
		//	 a	 a	  a    ?
		//	 a	 2a  0.7a  ?
			  9,  9,   9,  9,	9,	4,	 8,  9, 	/* speed	ショットの速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */
			  9, 18,   6,  9,	3,	4,	 3,  4, 	/* speed	針の速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */
		//	 霊  夢   魔  恋   レ  幽	チ	⑨
			#endif
	//
			/*(r32はそもそも自機が強すぎるのが原因で、ボスをどう作ってもスペカ前にボスが死ぬ。それでゲームが破綻している。)*/
		//	#if 0/*(r32)(ショット速度等仕様を代えたのでr33と互換性はない)あくまでメモ */
		//	 16, 32,  32,160,  48, 64,	48, 48, 	/* WEAPON_L0  strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
		//	 16, 32,  32,162,  48, 64,	48, 48, 	/* WEAPON_L1  strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
		//	 16, 33,  32,163,  49, 64,	48, 48, 	/* WEAPON_L2  strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
		//	 16, 34,  32,164,  49, 65,	48, 48, 	/* WEAPON_L3  strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
		//	 15, 35,  32,165,  50, 65,	56, 52, 	/* WEAPON_L4  strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
		//	 14, 36,  32,166,  52, 66,	80, 78, 	/* WEAPON_L5  strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
		//	#endif
	/* (r32)どう考えても針強すぎる。 */
			/* 以下は弾数も考慮して調整 */
			#if 0/*(r33a)テスト版、全体的に強すぎる。 */
			 20, 20,  20, 20,  48, 64,	48, 48, 	/* WEAPON_L0(P000-P008)  OPT0 strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
			 20, 20,  20,108,  48, 64,	48, 48, 	/* WEAPON_L1(P008-P015)  OPT1 strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
			 12, 15,  15,128,  49, 64,	48, 48, 	/* WEAPON_L2(P016-P031)  OPT2 strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
			 10, 14,  12,144,  49, 65,	48, 48, 	/* WEAPON_L3(P032-P063)  OPT3 strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
			  9, 13,  12,155,  50, 65,	56, 52, 	/* WEAPON_L4(P064-P127)  OPT4 strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
			 10, 16,  14,166,  52, 66,	80, 78, 	/* WEAPON_L5(P128)		 OPT4 strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
			#endif
			#if 0/*(r33t2)ある程度妥当(霊夢-魔理沙のみ)だが、正直まだ自機が強すぎる。(自機もボスももっと弱くするべき) */
			 10, 10,  10, 20,  48, 64,	48, 48, 	/* WEAPON_L0(P000-P008)  OPT0 strength 針の強さ */
			 10, 10,  10,108,  48, 64,	48, 48, 	/* WEAPON_L1(P008-P015)  OPT1 strength 針の強さ */
			  6,  7,   7,128,  49, 64,	48, 48, 	/* WEAPON_L2(P016-P031)  OPT2 strength 針の強さ */
			  5,  7,   6,144,  49, 65,	48, 48, 	/* WEAPON_L3(P032-P063)  OPT3 strength 針の強さ */
			  4,  6,   6,155,  50, 65,	56, 52, 	/* WEAPON_L4(P064-P127)  OPT4 strength 針の強さ */
			  5,  8,   7,166,  52, 66,	80, 78, 	/* WEAPON_L5(P128)		 OPT4 strength 針の強さ */
			#endif
			#if 0/*(r33t3)誘導/針巫女対応 */
			 11, 16,  10, 20,  48, 64,	48, 48, 	/* WEAPON_L0(P000-P008)  OPT0 strength 針の強さ */
			 11, 16,  10,108,  48, 64,	48, 48, 	/* WEAPON_L1(P008-P015)  OPT1 strength 針の強さ */
			  9, 10,   7,128,  49, 64,	48, 48, 	/* WEAPON_L2(P016-P031)  OPT2 strength 針の強さ */
			  9, 10,   6,144,  49, 65,	48, 48, 	/* WEAPON_L3(P032-P063)  OPT3 strength 針の強さ */
			  7, 10,   6,155,  50, 65,	56, 52, 	/* WEAPON_L4(P064-P127)  OPT4 strength 針の強さ */
			  8, 12,   7,166,  52, 66,	80, 78, 	/* WEAPON_L5(P128)		 OPT4 strength 針の強さ */
			#endif
			#if 1/*(r33t4)誘導/針巫女対応。誘導巫女(と森マリ)あまりに弱すぎるので、強くする。
				誘導巫女に関してはオプション追加間隔の速度が(r33以降で)遅すぎるようになったので、
				ここの威力を増やしてもたいして強くならないから、ここの威力を増やしすぎても問題ない。
			*/
			 11*2, 16,  10*2, 20,  48, 64,	48, 48, 	/* WEAPON_L0(P000-P008)  OPT0 strength 針の強さ */
			 11*2, 16,  10*2,108,  48, 64,	48, 48, 	/* WEAPON_L1(P008-P015)  OPT1 strength 針の強さ */
			  9*2, 10,   7*2,128,  49, 64,	48, 48, 	/* WEAPON_L2(P016-P031)  OPT2 strength 針の強さ */
			  9*2, 10,   6*2,144,  49, 65,	48, 48, 	/* WEAPON_L3(P032-P063)  OPT3 strength 針の強さ */
			  7*2, 10,   6*2,155,  50, 65,	56, 52, 	/* WEAPON_L4(P064-P127)  OPT4 strength 針の強さ */
			  8*2, 12,   7*2,166,  52, 66,	80, 78, 	/* WEAPON_L5(P128)		 OPT4 strength 針の強さ */
			#endif
#if 0
-----------(r33t3)[針巫女(夢)は針とショットでちゃんと差を付ける]
ショットと針の威力バランスが重要なので、ここに書くと解からないので、上参照。
-----------(r33t2)
霊夢 A(霊)					霊夢 B(夢)					/
10 x 1 == 10 == 1.00[total] 10 x 1 == 10 == 1.00[total] /
 6 x 2 == 12 == 1.20[total]  7 x 2 == 14 == 1.40[total] /
 5 x 3 == 15 == 1.50[total]  7 x 3 == 21 == 2.10[total] /
 4 x 4 == 16 == 1.60[total]  6 x 4 == 24 == 2.40[total] /
 5 x 4 == 20 == 2.00[total]  8 x 4 == 32 == 3.20[total] /
魔理沙 A(魔)				魔理沙 B(恋)				/
10 x 1 == 10 == 1.00[total] 20 x 1 == 20 == 1.00[total] /
 7 x 2 == 14 == 1.40[total]    x 2 ==	 ==  . 0[total] /
 6 x 3 == 18 == 1.80[total]    x 3 ==	 ==  . 0[total] /
 6 x 4 == 24 == 2.40[total]    x 4 ==	 ==  . 0[total] /
 7 x 4 == 28 == 2.80[total]    x 4 ==	 ==  . 0[total] /
-----------(r33a)
霊夢 A(霊)					霊夢 B(夢)					/
20 x 1 == 20 == 1.00[total] 20 x 1 == 20 == 1.00[total] /
12 x 2 == 24 == 1.20[total] 15 x 2 == 30 == 1.50[total] /
10 x 3 == 30 == 1.50[total] 14 x 3 == 42 == 2.10[total] /
 9 x 4 == 36 == 1.80[total] 13 x 4 == 52 == 2.60[total] /
10 x 4 == 40 == 2.00[total] 16 x 4 == 64 == 3.20[total] /
魔理沙 A(魔)				魔理沙 B(恋)				/
20 x 1 == 20 == 1.00[total] 20 x 1 == 20 == 1.00[total] /
15 x 2 == 30 == 1.50[total]    x 2 ==	 ==  . 0[total] /
12 x 3 == 36 == 1.80[total]    x 3 ==	 ==  . 0[total] /
12 x 4 == 48 == 2.40[total]    x 4 ==	 ==  . 0[total] /
14 x 4 == 56 == 2.80[total]    x 4 ==	 ==  . 0[total] /
	// 参考 http://hossy.info/game/toho/k_chara.php
//	注意: これらの測定データーは「ショット(札)」と「サブショット(針)」を足した威力なので、
// 「ショット(札)」、「サブショット(針)」それぞれ単体の具体的威力については何も判らない。
// (単体の具体的威力については、ゲーム中では簡単には測定できない。(測定困難))
	//				 霊夢 (霊) - 誘導弾 誘導弾は通常弾と同じ速度
	//				 霊夢 (夢) - ニードル ニードルは通常弾の2倍速
	   P |メイン|サブ|	WAY|ダメージ|打ち込み点||  P |メイン|サブ|OPT|ダメージ|打ち込み点
 0x8=  0 |	 6	|  0 |1-0-0| × 1.0 |	540    ||  0 |	 6	|  0 |0-0| × 1.0 |   540
 1x8=  8 |	 6	|  2 |	2-1| × 1.1 |	590    ||  8 |	 6	|  4 |	 | × 1.2 |   620
 2x8= 16 |	12	|  2 |	2-1| × 1.5 |	770    || 16 |	12	|  6 |	 | × 1.5 |   840
 4x8= 32 |	18	|  2 |3-   | × 1.8 |	890    || 32 |	12	|  8 |	 | × 1.6 |   860
 6x8= 48 |	18	|  4 |3-   | × 1.9 |	920    || 48 |	18	|  8 |	 | × 1.8 |   910
 8x8= 64 |	18	|  6 |3-   | × 1.9 |	950    || 64 |	18	| 20 |	 | × 2.1 |  1050
10x8= 80 |	18	|  6 |3-   | × 1.9 |	950    || 80 |	18	| 32 |	 | × 2.1 |  1240
12x8= 96 |	18	| 12 |3-   | × 2.0 |  1020    || 96 |	18	| 38 |	 | × 2.8 |  1520
16x8=MAX |	24	| 16 |4-   | × 2.0 |  1060    ||MAX |	30	| 52 |	 | × 3.1 |  1720

	// 11==16* 0.7 魔理沙 (魔) - ミサイル 通常弾の70％。
	// ?? ??	   魔理沙 (恋) - レーザー スピード測定不能。というか全てにおいて測定困難。
  P |メイン|サブ|OPT|ダメージ|打ち込み点||	 P |メイン|サブ|OPT|ダメージ|打ち込み点
  0 |  6   |  0 |0-0| × 1.0 |	 540	||	 0 |   6  |  0 |0-0| × 1.0 |  540
  8 |  6   |  2 |	| × 1.4 |	 500	||	 8 |   6  |  1 |   | × -.- |	 ?
 16 |  6   |  4 |	| × 1.7 |	 480	||	16 |   6  |  1 |   | × -.- |	 ?
 32 |  6   |  4 |	| × 1.7 |	 510	||	32 |  12  |  1 |   | × -.- |	 ?
 48 |  6   |  4 |	| × 1.8 |	 520	||	48 |  12  |  2 |   | × -.- |	 ?
 64 | 12   | 10 |	| × 2.2 |	 830	||	64 |  12  |  2 |   | × -.- |	 ?
 80 | 12   | 12 |	| × 2.3 |	 910	||	80 |  18  |  2 |   | × -.- |	 ?
 96 | 18   | 16 |	| × 2.6 |	1150	||	96 |  18  |  3 |   | × -.- |	 ?
MAX | 18   | 18 |	| × 2.8 |	1300	|| MAX |  30  |  3 |   | × -.- |	 ?

#endif
	/* [***20090822 REIMU 24 <- 16 (とりあえず針弱すぎるので) */
	/* r32 霊夢Aの誘導ショット。強すぎる気がするので、32 → 16 にしてみる。*/
		};
	//		 2, 3,	3, 2,  4,16, 5, 5,	/* strength ショットの強さ */
	//		16,14, 10,12,  9, 4, 8, 8,	/* speed	ショットの速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */
		//};
	//		 2, 2, 3, 3, 4, 3, 3, 3,	/* strength ショットの強さ */
	//		15,15,12,12,18,12,12,12,	/* speed	ショットの速さ	*/
	/* 極端に機体の性能が偏るのは良くない。(強くて速いのはずるい) */
		//static const u8 ddd_tbl[DDD_MAX] =
		//{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
	//		 3, 3, 4, 4, 6, 8, 6, 6,	/* strength 針の強さ */ 	/* REMILIA, CIRNO,	6 5 強すぎる */
	/* 霊夢(強くしてみる [***20090930 ) */
	//		 4, 4, 4,10, 6, 8, 6, 6,	/* strength 針の強さ */ 	/* REMILIA, CIRNO,	6 5 強すぎる */
	//		 4, 4, 4,25, 6, 8, 6, 6,	/* strength 針の強さ */ 	/* REMILIA, CIRNO,	6 5 強すぎる */
	//		18,18,10,15, 3, 4, 3, 3,	/* speed	針の速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */


		h->base_weapon_strength = ccc_tbl[CCC_STRENGTH_L1+(cg_jiki_weapon_level_offset)+(is_needle<<(4+2))];/* 64 == 8[players]*8[line] */
	//	h->base_weapon_strength = ddd_tbl[DDD_STRENGTH_L1+(cg_jiki_weapon_level_offset)];

	//
	//	int ok;
	//	ok=0;
		{
			#if 0
		//	if (1==r_tbl[shot_type][REI03_yudo])	/* pspは0レジスタがあるので0と比較したほうが速い */
			if (0 != r_tbl[shot_type][REI03_yudo])/* 誘導ショットか？ */
			#else
		//	if ( (REIMU_SHOT_TYPE_01) > shot_type)/* 誘導ショットか？ */
			if ( (REIMU_SHOT_TYPE_00) == shot_type)/* 誘導ショットか？ */
			#endif
			{
				SPRITE *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
				SPRITE *target;
				target = search_teki_from_obj();
				if (target != zzz_player)/* 見つかったら */
				{
					h->PL_SHOT_DATA_target_x256  = target->cx256;/* 誘導目標座標を覚えとく */
				//	h->PL_SHOT_DATA_target_y256  = target->cy256;/* 誘導目標座標を覚えとく */
					//
				//	if ( (REIMU_SHOT_TYPE_00) == shot_type)/* 誘導ショットか？ */
					{
						int int_angle1024;
					//	int_angle1024	= at an_1024(target->cy256-zzz_player->cy256, target->cx256-zzz_player->cx256);
						int_angle1024	= atan_65536(target->cy256-zzz_player->cy256, target->cx256-zzz_player->cx256);
						int_angle1024 >>= (6);
					//	if (511 < int_angle1024 )/* 上方180-360度[/360度]の領域のみ追尾する */
						if ( /*(384)*/(256*3) < (( int_angle1024+128+256)&(1024-1)) )/* 上方225-315度[/360度]の領域のみ追尾する */	/* CCWの場合 */
						{
							h->vx256	 = sin1024((int_angle1024))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+(cg_game_select_player)]*/);/*fps_factor*/ /* CCWの場合 */
							h->vy256	 = cos1024((int_angle1024))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+(cg_game_select_player)]*/);/*fps_factor*/
							#if 1
							/* 描画用グラ回転 */
							if (MARISA_A==(cg_game_select_player))/* (魔理沙魔符の)マジックミサイルは傾かない。 */
							{	int_angle1024 = (0);	}
							else/* (霊夢霊符の)ホーミングアミュレットは模倣風では傾ける事にする。 */
							{
							//	h->rotationCCW1024	= int_angle1024;
								/* 描画用グラ回転(ショットは上が正面なので、回転角を計算)[180/360度]回す。 */
								int_angle1024 += cv1024r((180));
								mask1024(int_angle1024);
							}
							h->rotationCCW1024	= int_angle1024;/* 下が正面の角度が描画用角度 */
							#endif
						//	ok=1;/* 自動追尾可能 */
							return;
						}
					}
				}
			}
		}
	//
	//	if (0==ok)/* 自動追尾不可？ */
		{	/* 固有の設定角度を使う */
			h->vx256 = ((/*int_vx*/r_tbl[shot_type][REI02_vx256]))*/*p->speed*/(ccc_tbl[CCC_SPEED+OFFS_IS_NEEDLE]);/*fps_factor*/
			h->vy256 = -(((int)ccc_tbl[CCC_SPEED+OFFS_IS_NEEDLE])<<8);/*fps_factor*/ /*p->speed*/
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------
	プレイヤー弾(ショットのみ)生成
---------------------------------------------------------*/

/* 霊夢A (誘導巫女) 誘導ショット */

static void shot_regist_re_yudou(SPRITE *s) /* 霊夢A 魔理沙A */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_00);/* 誘導ショット */
}

static void shot_regist_yuyuko(SPRITE *s)	/* 幽々子 */
{
	player_register_shot_object(s, NEEDLE_ANGLE_271);
}

/* 霊夢B (針巫女) / 魔理沙A/B / レミリア ＆ チルノ */

static void shot_regist_re_hari(SPRITE *s)	/* 魔理沙A/B */ 	/* 兼(現在ダミー用) */
{
	player_register_shot_object(s, NEEDLE_ANGLE_270);
}

static void shot_regist_ci_gggg(SPRITE *s)	/* レミリア ＆ チルノ */
{
	player_register_shot_object(s, NEEDLE_ANGLE_277 );
	player_register_shot_object(s, NEEDLE_ANGLE_263 );
}
static void shot_regist_ma_lazer(SPRITE *s) /* 魔理沙B レーザー */
{
	{
		static u8 count128;
		count128++;
	//	count128 &= 0x7f;
		/* -110 0000 */
		#if 0
	//	if (0x00==(count128&0x60))	/* 強力だが、1/4時間しか効果がない。 */
		#else
		count128 &= 0x7f;
		if ((43)>(count128 ))	/* 強力だが、1/3時間しか効果がない。 */
		#endif
		{
			shot_regist_re_hari(s);
		}
	}
}

/*---------------------------------------------------------
	プレイヤー弾(ショットのみ)生成
---------------------------------------------------------*/

/* 霊夢A/B 回転ショット */	/* 通常ショット */
static void shot_regist_1(SPRITE *s)	/* 霊夢 */	/* 霊夢 ＆ 魔理沙 ＆ チルノ ＆ レミリア */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* 通常ショット */
}
static void shot_regist_2(SPRITE *s)	/* 霊夢 */	/* 霊夢 ＆ 魔理沙 ＆ チルノ ＆ レミリア */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* 通常ショット */
}
static void shot_regist_3(SPRITE *s)	/* 霊夢 */	/* 魔理沙 ＆ チルノ */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* 通常ショット */
}
static void shot_regist_4(SPRITE *s)	/* 霊夢 */	/* 魔理沙 */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* 通常ショット */
}

static void shot_regist_5(SPRITE *s)	/* 霊夢B */ /* 魔理沙 */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_06); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_07); 	/* 通常ショット */
}

/* 霊夢B (針巫女) */

/* 魔理沙A(ミサイル魔理沙) */
/* 魔理沙B(レーザー魔理沙) */


static void shot_regist_chou(SPRITE *s) 		/* 幽々子 */
{
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_08);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_09);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_0a);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_0b);
}


/*---------------------------------------------------------
	オプションの攻撃を成長段階によってかえる。
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
{	/* 攻撃支援 */
	if (src->PL_OPTION_DATA_opt_anime_add_id < OPTION_C3)
	{
	//case OPTION_C1: OPTION_C2:
	//	if (cg.weapon_power > (100-1))	/* 旧システムの調整値 100 */
		if (cg.weapon_power > (96)) 	/* システムが変わったので修正してみる(96==128-32) */
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
	オプション アニメーション
---------------------------------------------------------*/
//
static void player_animate_option(SPRITE *src)
{
	#define BASE_OPT_ANIM_TYPE			(PLAYERS8*0)
	#define BASE_OPT_ANIM_SPEED 		(PLAYERS8*1)
	#define BASE_OPT_ANIM_MAX			(PLAYERS8*2)
	/*static*/static const u8 anime_fix_status[BASE_OPT_ANIM_MAX] =
	{/* REIMU(A/B) MARISA(A/B)	REMILIA YUYUKO CIRNO(A/Q) */
		   0,	0,	 0,   0,	0,	 1,   1,   1,	 /* BASE_OPT_ANIM_TYPE	   0==回転, 1==パターン */
		   4,	4,	 6,   8,	4,	12,   3,   3,	 /* BASE_OPT_ANIM_SPEED    0なら回転速度 / 1ならパターン速度 */
	};
	if (0==anime_fix_status[BASE_OPT_ANIM_TYPE+(cg_game_select_player)])
	{	/* オプションが回転する場合 */
		/* 描画用角度(下が0度で左回り(反時計回り)) */
		src->rotationCCW1024 += (anime_fix_status[BASE_OPT_ANIM_SPEED+(cg_game_select_player)]);	/* 描画用グラ回転速度 */
		mask1024(src->rotationCCW1024);
	}
	else
	{	/* オプションがアニメーションする場合 */
		src->PL_OPTION_DATA_anime_wait--;
		if (src->PL_OPTION_DATA_anime_wait<1)
		{
			src->PL_OPTION_DATA_anime_wait = (anime_fix_status[BASE_OPT_ANIM_SPEED+(cg_game_select_player)]);
			#if 1
			src->type++;
			src->type &= (0xfffffff7); /* 上位ビットに影響を与えないでアニメーション */
			#endif
		}
	}
}


/*---------------------------------------------------------
	プレイヤー、オプションの移動制御
---------------------------------------------------------*/
/*static*/extern void player_control_option(SPRITE *src);	/* 全員 */
/*static*/global void player_move_option(SPRITE *src)	/* 全員 */
{
	/* オプションが非表示の場合、何もしない。 */
	if (0==(src->flags & ( SP_FLAG_OPTION_VISIBLE)))	{	return; 	}
//
	/* アニメーション */
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
//	/* BASE_OPT_SHOT_INTERVAL オプションショットの更新間隔 / option shot interval. */
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
//(r33)誘導巫女のオプションは弱いと全然やる気がない。
	  99,  13,	11,   4,	8,	31,   8,   9,	 /* WEAPON_L0 */
	  77,  11,	10,   4,	8,	28,   8,   9,	 /* WEAPON_L1 */
	  55,	9,	 9,   4,	8,	25,   8,   9,	 /* WEAPON_L2 */
	  33,	7,	 8,   4,	8,	22,   8,   9,	 /* WEAPON_L3 */
	  10,	5,	 7,   4,	8,	19,   7,   8,	 /* WEAPON_L4 */
	   4,	3,	 6,   3,	8,	16,   5,   6,	 /* WEAPON_L5 */
};
					src->PL_OPTION_DATA_opt_shot_interval = player_bbb_status[(cg_jiki_weapon_level_offset)];/* (霊夢 魔理沙	仮幽々子)オプションのショット間隔 */
					#if 1//1975613
					static /*const*/ void (*ggg[PLAYERS8])(SPRITE *sss) =
					{
						/*REIMU_A*/ 	shot_regist_re_yudou,			/* 霊夢 A(誘導赤札) */
						/*REIMU_B*/ 	shot_regist_re_hari,			/* 霊夢 B(針) */	/*re_bbbb*/
						/*MARISA_A*/	shot_regist_re_yudou,			/* 魔理沙 A(誘導森弾) */
						/*MARISA_B*/	shot_regist_ma_lazer,			/* 魔理沙 B(きまぐれレーザー) */
						/*REMILIA*/ 	shot_regist_remilia_cirno,		/* レミリア ＆ チルノ */
						/*YUYUKO*/		shot_regist_yuyuko, 			/* 幽々子(蝶) */
						/*CIRNO_A*/ 	shot_regist_remilia_cirno,		/* レミリア ＆ チルノ */
						/*CIRNO_Q*/ 	shot_regist_remilia_cirno,		/* レミリア ＆ チルノ */
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
	自機のショットボタン押しっぱなしで自動ショットを撃つ場合
---------------------------------------------------------*/

global void register_main_shot(SPRITE *s1)
{
	if (
		(MARISA_B==(cg_game_select_player)) &&	/* 魔理沙B(恋符) */
		(0<cg.bomber_time)				/* マスタースパーク中 */
	)
	{
		;/* マスタースパーク中はショットが撃てないらしい */
	}
	else
	{
		{static int weapon_interval=0;
			/*cg.*/weapon_interval--/*=fps_fa_ctor*/;
			if (/*cg.*/weapon_interval < 1 )
			{	/* 自機のショットボタン押しっぱなしで自動ショットを撃つ場合の、撃つ間隔[フレーム単位]のテーブル。 */
				static const u8 jiki_auto_shot_interval_table[(WEAPON_L_MAX)*(PLAYERS8)] =
				{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */    /* レミリア強すぎるので調整(+5) */
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
	/*REIMU_A*/ 	/*REIMU_B*/ 	/*MARISA_A*/	/*MARISA_B*/	/*REMILIA*/ 	/*YUYUKO 無段階成長 */	/*CIRNO_A*/ 	/*CIRNO_Q*/
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
