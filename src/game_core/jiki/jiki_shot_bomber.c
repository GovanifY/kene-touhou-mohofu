
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	各プレイヤー(霊夢 ＆ 魔理沙 ＆ レミリア ＆ 幽々子 ＆ チルノ)
	REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q)
	-------------------------------------------------------
---------------------------------------------------------*/

#include "jiki_local.h"
#define OBJ_Z80_SHOT_SPECIAL				(0x8000)/*0x0100*/		/* 霊夢の回転ショット / kodomo_hlaserの判別 */

/* 霊夢 A (座布団巫女) / 魔理沙 A (森魔法使い) 用の誘導目標座標 */
#define PL_SHOT_DATA_target_x256		user_data00
#define PL_SHOT_DATA_target_y256		user_data01

#define PL_SHOT_DATA_SHOT_SPECIAL		user_data03
//#define PL_SHOT_DATA_parrent_jyumyou		user_data03
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

enum/* 発弾できるショットの全種類。(針含む) */
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
	REIMU_SHOT_TYPE_08,
	REIMU_SHOT_TYPE_09,
//	/* 非誘導ショット(幽々子) */
	YUYUKO_SHOT_TYPE_08,
	YUYUKO_SHOT_TYPE_09,
	YUYUKO_SHOT_TYPE_0a,
	YUYUKO_SHOT_TYPE_0b,
//	/* 針(サブウェポン) */
	NEEDLE_ANGLE_270_M, 	/* オプション中心から発弾 */
	NEEDLE_ANGLE_270_L, 	/* オプション左から発弾 (霊夢B 専用) */
	NEEDLE_ANGLE_270_R, 	/* オプション右から発弾 (霊夢B 専用) */
	NEEDLE_ANGLE_263,		/* レミリア ＆ チルノ 専用 */
	NEEDLE_ANGLE_277,		/* レミリア ＆ チルノ 専用 */
	NEEDLE_ANGLE_271,		/* 幽々子 専用 */
//
	REIMU_SHOT_TYPE_MAX 	/* 最大数 */
};

enum/* ショットの性能、固有値。(キャラ別に初期設定する) */
{
	// 自機ショットの強さ
	SHOT_STATUS_00_SHOT_STRENGTH_L1 = 0,
	SHOT_STATUS_01_SHOT_STRENGTH_L2,
	SHOT_STATUS_02_SHOT_STRENGTH_L3,
	SHOT_STATUS_03_SHOT_STRENGTH_L4,
	SHOT_STATUS_04_SHOT_STRENGTH_L5,
	SHOT_STATUS_05_SHOT_STRENGTH_L6,
	SHOT_STATUS_06_SHOT_SPEED,	// 自機ショットの移動速度
	SHOT_STATUS_07_HARI_SPEED,	// オプションショットの移動速度
	// オプションショットの強さ
	SHOT_STATUS_08_HARI_STRENGTH_L1,
	SHOT_STATUS_09_HARI_STRENGTH_L2,
	SHOT_STATUS_0A_HARI_STRENGTH_L3,
	SHOT_STATUS_0B_HARI_STRENGTH_L4,
	SHOT_STATUS_0C_HARI_STRENGTH_L5,
	SHOT_STATUS_0D_HARI_STRENGTH_L6,
	SHOT_STATUS_0E_SHOT_ATARI_HANKEI,	/*(自機ショットのあたり判定半径)*/
	SHOT_STATUS_0F_HARI_ATARI_HANKEI,	/*(オプションショットのあたり判定半径)*/
	// 自機ショットの更新間隔
	SHOT_STATUS_10_JIKI_SHOT_INTERVAL_L1,
	SHOT_STATUS_11_JIKI_SHOT_INTERVAL_L2,
	SHOT_STATUS_12_JIKI_SHOT_INTERVAL_L3,
	SHOT_STATUS_13_JIKI_SHOT_INTERVAL_L4,
	SHOT_STATUS_14_JIKI_SHOT_INTERVAL_L5,
	SHOT_STATUS_15_JIKI_SHOT_INTERVAL_L6,
	SHOT_STATUS_16_STANDARD_BOMB_STRENGTH,	/*(通常ボムの強さ)*/
	SHOT_STATUS_17_LOWER_BOMB_STRENGTH, 	/*(低速ボムの強さ)*/
	// オプションショットの更新間隔
	SHOT_STATUS_18_OPTION_SHOT_INTERVAL_L1,
	SHOT_STATUS_19_OPTION_SHOT_INTERVAL_L2,
	SHOT_STATUS_1A_OPTION_SHOT_INTERVAL_L3,
	SHOT_STATUS_1B_OPTION_SHOT_INTERVAL_L4,
	SHOT_STATUS_1C_OPTION_SHOT_INTERVAL_L5,
	SHOT_STATUS_1D_OPTION_SHOT_INTERVAL_L6,
	SHOT_STATUS_1E_STANDARD_BOMB_ATARI_HANKEI,	/*(通常ボムのあたり判定半径)*/
	SHOT_STATUS_1F_LOWER_BOMB_ATARI_HANKEI, 	/*(低速ボムのあたり判定半径)*/
	//
	SHOT_STATUS_20_BOMBER_KAITEN_SOKUDO,		/*(回転速度)*/
	SHOT_STATUS_21_BOMBER_KAKUDAI_SOKUDO,		/*(拡大速度)*/
	SHOT_STATUS_22_KURAI_BOMB_UKETUKE_JIKAN,	/*(喰らいボム受け付け時間、値が小さい方が受け付け時間が長い。)*/
	SHOT_STATUS_23_PLAYER_GRAZE_ATARI_HANKEI,	/*(プレイヤー。グレイズの当たり判定半径)*/
	SHOT_STATUS_24_PLAYER_SET_BOMBER_TIME,/*(ボムの寿命)*/
	SHOT_STATUS_25_PLAYER_ANIME_SPEED,/*(プレイヤーのアニメーション速度)*/
	SHOT_STATUS_26_OPTION_ANIME_KAITEN,
	SHOT_STATUS_27_OPTION_ANIME_SPEED,
	//
	SHOT_STATUS_32_MAX		/* 最大数 */
};

typedef struct /*_SHOT_STATUS_tag_*/
{
	u8 shot_status[SHOT_STATUS_32_MAX];
} SHOT_STATUS;

static SHOT_STATUS current;


/*---------------------------------------------------------
	プレイヤー武器のあたり判定
OBJ *tt:
	ザコ敵本体。
	敵弾。
OBJ *shot:
	自機のショット弾。
	自機のボンバー。
	自機自体で体当たり。
erase_shot_type:
	PLAYER_WEAPON_TYPE_00_SHOT: 		自弾消滅する。	敵弾消さない。
		ショット弾。
	PLAYER_WEAPON_TYPE_01_BOMB_PLAYER:	自弾消滅しない。敵弾消す。
		ボンバー。
		無敵期間内の自機による体当たり。
---------------------------------------------------------*/

/* static */global void player_weapon_collision_check(OBJ *shot, int erase_shot_type)
{
//	OBJ *shot;	/* 自弾 */
	OBJ *tt;	/* 一時使用のテンポラリ(敵スプライト、または、敵弾スプライト) */
	#define teki_obj		tt
	#define tekidan_obj 	tt
	/* 敵弾にあたった場合に敵弾を消す(ボム系のみ) */
	if (/* PLAYER_WEAPON_TYPE_01_BOMB_PLAYER== */erase_shot_type)	/* ボム系のみ */	/* erase_bullets */
	{
		/* 自弾にあたったのは敵弾なのか調べる． */
		tekidan_obj = obj_collision_check_00_tama(shot, OBJ_Z04_TAMA);/* 弾専用 */
		if (NULL != tekidan_obj)		/* 敵弾に当たったら */
		{
			tekidan_obj->jyumyou = JYUMYOU_NASI;	/* 敵弾が消滅 */
			voice_play(VOICE02_MENU_SELECT, /*TRACK03_IVENT_DAN*//*(???)*/TRACK05_ZAKO_DEATH /*TRA CK01_PICHUN*/);
		}
	}
	/* 敵にあたった場合に敵を消す */
	{
		/* 自弾にあたったのは敵自体なのか調べる． */
		teki_obj = obj_collision_check_A01_A02_teki(shot);//, (OBJ_Z02_TEKI));
		if (NULL != teki_obj)			/* 敵自体に当たったら */
		{
			if (NULL != (teki_obj->callback_hit_teki))	/*	 */
			{
				(teki_obj->callback_hit_teki)(teki_obj/* 敵自体 */, shot/* 自弾 */);
			}
			/* 自弾が敵にあたった場合に自弾が消滅する */
			if (PLAYER_WEAPON_TYPE_00_SHOT == erase_shot_type)	/* ショット系のみ */	/* erase_player_tama */
			{
			//	/* 自弾消滅方法が特殊な場合 */
			//	if (NULL != (shot->callback_loser))
			//	{
			//		(shot->callback_loser)(shot/* 自弾 */);
			//	}
				shot->jyumyou = JYUMYOU_NASI;		/* 自弾が消滅 */
				/* ショットが敵に当たった場合、打ち込み点を加算。(無敵キャラでも撃ち込み点で稼げる) */
				player_dummy_add_score(score(10));
			}
		}
	}
}

/*---------------------------------------------------------
	プレイヤー、針弾、ショット弾の移動
	霊夢 ＆ 魔理沙 ＆ レミリア ＆ 幽々子 ＆ チルノ
---------------------------------------------------------*/

/*---------------------------------------------------------
	ショット弾のあたり判定をチェックする。
	画面外なら消去。
---------------------------------------------------------*/

static OBJ_CALL_FUNC(check_collision_gamengai)
{
	if (((src->center.y256)+(t256(-10))) < t256(0))/*(画面外に出たら)*/
	{
		src->jyumyou = JYUMYOU_NASI;/*(ショットおしまい)*/
	}
	else/*画面内なら*/
	{
		player_weapon_collision_check(src, PLAYER_WEAPON_TYPE_00_SHOT);/*(あたり判定チェック)*/
	}
}

/*---------------------------------------------------------
	その他汎用、ショット弾の移動
---------------------------------------------------------*/

static OBJ_CALL_FUNC(other_move_object)
{
	src->center.x256 += src->math_vector.x256;		/*fps_factor*/
	src->center.y256 += src->math_vector.y256;		/*fps_factor*/
	check_collision_gamengai(src);
}
/*---------------------------------------------------------
	レミリア専用、ショット弾の移動
---------------------------------------------------------*/

static OBJ_CALL_FUNC(remilia_move_object)
{
//	if (REMILIA==(cg_ga me_select_player))	/* レミリアはゆらゆら */
	{	u16 rand_int;
		rand_int = ra_nd(); 	/*(リプレイ時に再現性が必要)*/
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
	誘導サブ。
---------------------------------------------------------*/
static void yuudo_aaa(OBJ/**/ *src, int bbb)
{
	#if (1)
	/* パワーの量に比例してショットの軌跡を曲げる。 */
	src->math_vector.x256 += ((src->math_vector.x256 * bbb)>>11);
	#endif
	//
	#if (1)
	/* ショットを誘導する。 */
	OBJ *obj_boss;
	obj_boss			= &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+TEKI_OBJ_00_BOSS00_HONTAI];/*(ボス本体)*/
	if (obj_boss->center.x256 < src->center.x256)/*(ボスが左 < 幽々子が右)*/
	{
		if (0 < src->math_vector.x256)/*(正==幽々子ショットが右へ移動中)*/
		{
			src->math_vector.x256 -= src->math_vector.x256;/* 符号反転(負方向==幽々子ショットを左へ誘導) */
		}
	//	else/*(負==幽々子ショットが左へ移動中)*/
	//	{;}/*(補正する必要は無し)*/
	}
	else/*(幽々子が左 > ボスが右)*/
	{
		if (0 > src->math_vector.x256)/*(負==幽々子ショットが左へ移動中)*/
		{
			src->math_vector.x256 -= src->math_vector.x256;/* 符号反転(正方向==幽々子ショットを右へ誘導) */
		}
	//	else/*(正==幽々子ショットが右へ移動中)*/
	//	{;}/*(補正する必要は無し)*/
	}
	#endif
}
	#if 0
	/* 誘導する */
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
	霊夢Aザブトン専用、ショット弾の移動
---------------------------------------------------------*/
static OBJ_CALL_FUNC(zabuton_move_object)
{
	//[旧位置]
//	if (REIMU==(cg_ga me_select_player))			/* 霊夢の回転ショット */
//	if ((REIMU_B+1) > (cg_ga me_select_player)) 	/* 霊夢の回転ショット */
	{
	//ザブトン専用	if (((JIKI_SHOT_01)|OBJ_Z80_SHOT_SPECIAL)==src->obj_type_set)
		{
			/* 描画用グラ回転 */
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
	幽々子専用、ショット弾の移動
---------------------------------------------------------*/
/* 幽々子はそれる */

static OBJ_CALL_FUNC(yuyuko_move_object)
{
	static unsigned short pow_low;
	pow_low++;
	if ((1024) < pow_low)/* 1024発移動するごとに御腹が空く */
	{
		pow_low = 0;
		if (0 < cg.weapon_power)/* パワーがあれば */
		{
			cg.weapon_power--;/* 減らす */
		}
	}
	//[旧位置]
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

/* メインショット(札) / サブウェポン(針) */
#define reimu_a_move_shot	zabuton_move_object/*(座蒲団専用)*/
#define reimu_a_move_hari	other_move_object/*(汎用)*/
#define reimu_b_move_shot	other_move_object/*(汎用)*/
#define reimu_b_move_hari	other_move_object/*(汎用)*/
#define marisa_a_move_shot	other_move_object/*(汎用)*/
#define marisa_a_move_hari	other_move_object/*(汎用)*/
#define marisa_b_move_shot	other_move_object/*(汎用)*/
#define marisa_b_move_hari	other_move_object/*(汎用)*/

#define remilia_move_shot	remilia_move_object/*(レミリア専用)*/
#define remilia_move_hari	remilia_move_object/*(レミリア専用)*/
#define yuyuko_move_shot	yuyuko_move_object/*(幽々子専用)*/
#define yuyuko_move_hari	yuyuko_move_object/*(幽々子専用)*/
#define chrno_a_move_shot	other_move_object/*(汎用)*/
#define chrno_a_move_hari	other_move_object/*(汎用)*/
#define chrno_q_move_shot	other_move_object/*(汎用)*/
#define chrno_q_move_hari	other_move_object/*(汎用)*/

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

/*
メモ 紅魔郷より。
;-------------------
霊夢Aの札グラは常にゆっくり回転している。回転方向は逆だが回転速度は共に同じ。
主札(ショット)は、反時計回り(CCW)。
誘導札(サブショット)は、時計回り(CW)。(ホーミングアミュレット)
;-------------------
霊夢Aの場合、
主札(ショット)は、半透明(かなり薄い色)。
誘導札(サブショット)、半透明(かなり薄い色)。(ホーミングアミュレット)
;-------------------
模倣風の 霊夢A では、回転する誘導札は無い。
代わりに、回転しないホーミングアミュレットになっている。
*/

//global void player_register_shot_object(OBJ/**/ *src, int /*set_*/shot_type);
/*static*/static/*global*/ void player_register_shot_object(OBJ/**/ *src, int /*set_*/shot_type)
{
	OBJ *h;/* shot */
	h					= obj_regist_teki();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		static const s8 r_tbl[REIMU_SHOT_TYPE_MAX][REI04_MAX] =
		{	/*x_offset256	y_offset256  vx256 status */
	//	/* 誘導ショット(霊夢 ＆ 魔理沙) */
	/*00*/	{ (  0),	(  0),	(	0), 	0x01},	/* */
	//	/* 非誘導ショット(霊夢) */
	/*01*/	{ (  0),	(  0),	(	0), 	0x00},	/* 中 まっすぐ */
	/*02*/	{ ( -5),	(  0),	(	0), 	0x00},	/* 左 まっすぐ */
	/*03*/	{ ( +5),	(  0),	(	0), 	0x00},	/* 右 まっすぐ */
	/*04*/	{ ( -5),	(  0),	( -20), 	0x00},	/* 左 左上 */
	/*05*/	{ ( +5),	(  0),	(  20), 	0x00},	/* 右 右上 */
	/*06*/	{ ( -5),	(  0),	( -35), 	0x00},	/* 左 左斜上(狭) */
	/*07*/	{ ( +5),	(  0),	(  35), 	0x00},	/* 右 右斜上(狭) */
	/*06*/	{ ( -5),	(  0),	( -88), 	0x00},	/* 左 左斜上(広) */
	/*07*/	{ ( +5),	(  0),	(  88), 	0x00},	/* 右 右斜上(広) */
	//	/* 非誘導ショット(幽々子) */
	/*08*/	{ (-15),	(-25),	( -25), 	0x00},	 /* 幽々子 */					 /*(-32)==(4*(- 8))*/
	/*09*/	{ (+15),	(-25),	( +25), 	0x00},	 /* 幽々子 */					 /*( 32)==(4*(	8))*/
	/*0a*/	{ (-25),	(-25),	( -50), 	0x00},	 /* 幽々子 */					 /*(-64)==(4*(-16))*/
	/*0b*/	{ (+25),	(-25),	( +50), 	0x00},	 /* 幽々子 */					 /*( 64)==(4*( 16))*/
	//		/* CCWの場合 */
			{ (  0),	( -7),	(	0), 	0x01},// NEEDLE_ANGLE_270_M 	cv1024r((180)), 			/* 魔理沙 A/B 霊夢 レミリア ＆ チルノ */
			{ ( -5),	( +4),	(	0), 	0x01},// NEEDLE_ANGLE_270_L 	cv1024r((180)), 			/* オプション左から発弾 (霊夢B 専用) */
			{ ( +5),	( +4),	(	0), 	0x01},// NEEDLE_ANGLE_270_R 	cv1024r((180)), 			/* オプション右から発弾 (霊夢B 専用) */
			//
			{ (  0),	( -7),	(-108), 	0x01},// NEEDLE_ANGLE_263	cv1024r((180-25)),/*263=270-7*/ /* レミリア ＆ チルノ */
			{ (  0),	( -7),	( 108), 	0x01},// NEEDLE_ANGLE_277	cv1024r((180+25)),/*277=270+7*/ /* レミリア ＆ チルノ */
			{ (  0),	(-47),	(	5), 	0x01},// NEEDLE_ANGLE_271	cv1024r((181)), 				/* 幽々子専用 */	/*-48*/ /*t256( 0), t256(-40),*/
		};
	//	int shot_type;
		int shot_status_hari;/* 針? */
		shot_status_hari = ((r_tbl[shot_type][REI03_status]));

		int is_needle;/* 針? */
	//	is_needle = (shot_type <= NEEDLE_ANGLE_270)?(1/* 針 */):(0/* 弾 */);
		is_needle = ((shot_status_hari)&0x01);
	//
	//	h->m_Hit256R			= ((base_shot_atari[(OFFS_IS_NEEDLE)])<<8);/* あたり判定の大きさ */
		h->m_Hit256R			= (((int)current.shot_status[SHOT_STATUS_0E_SHOT_ATARI_HANKEI+is_needle])<<8);/* あたり判定の大きさ */

	//	h->type 			= (/*OBJ_Z01_JIKI_GET_ITEM*/JI KI_SHOT_02|OBJ_Z80_SHOT_SPECIAL)-shot_status_hari;
		#if 1
	//	else
	//	if (0x01== shot_status) 	/* 針 */
		{
			h->obj_type_set 			= (JIKI_SHOT_07|OBJ_Z80_SHOT_SPECIAL);	/*OBJ_Z01_JIKI_GET_ITEM*/	/*SP_PL_FIREBALL*/
		}
	//
		if (0x00 == shot_status_hari)	/* 針以外 */
		{
			if (REIMU_B+1 > (cg_game_select_player))	/* 霊夢? */
			{
			//	if (0x02== shot_status_hari) /* 霊夢の回転ショット */
				if (REIMU_SHOT_TYPE_00 != shot_type)/* 誘導ショット以外？ */
				{
					h->obj_type_set 			= (JIKI_SHOT_01|OBJ_Z80_SHOT_SPECIAL);	/*OBJ_Z01_JIKI_GET_ITEM*/
					/* 描画用グラ回転 */
					/*(リプレイ時に再現性が必要ない)*/
				//	h->rotationCCW1024	= (ra_nd() & (1024-1));/* 出だしの角度はランダムでないと(レーザーみたいな)変な画面になる */
					h->rotationCCW1024	= (vfpu_rand_8888(0, 255) & (1024-1));/* 出だしの角度はランダムでないと(レーザーみたいな)変な画面になる */
				}
			/*	else	{	h->type = 誘導ショットは、針と同じ; 	}	*/
			}
			else
			{
				h->obj_type_set 			= (JIKI_SHOT_02|OBJ_Z80_SHOT_SPECIAL);	/*OBJ_Z01_JIKI_GET_ITEM*/
			}
		}
	//	else
		#endif
		#if 1/*Gu(中心座標)*/
		h->center.x256			= (src->center.x256) + ((r_tbl[shot_type][REI00_x_offset])<<8); /*+ x_offs*/
		h->center.y256			= (src->center.y256) + ((r_tbl[shot_type][REI01_y_offset])<<8);/*(20)*/
		#endif
	//	h->callback_mover	= player_move_shot;
		void (*player_move_shot_table[16])(OBJ/**/ *src) =
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
		h->callback_mover	= *player_move_shot_table[(((cg_game_select_player)+(is_needle<<3)))];
		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x96);	/*	h->alpha		= 0x96; 150 半透明にする。 */
	//

	//	h->kougeki_ti = ddd_tbl[DDD_STRENGTH_L1+(cg.jiki_weapon_level_offset)];
	//	h->kougeki_ti = ccc_tbl[CCC_STRENGTH_L1+(cg.jiki_weapon_level_offset)+(is_needle<<(4+2))];/* 64 == 8[players]*8[line] */
		/*(r35u2-)*/
		h->kougeki_ti = (((int)current.shot_status[SHOT_STATUS_00_SHOT_STRENGTH_L1+(cg.jiki_weapon_level)+(is_needle<<(3))])<<2);/* 64 == 8[players]*8[line] */
		/*
			攻撃値: ボンバーの威力。
		*/
	//
	//	int ok;
	//	ok=0;
		{
			#if 0
		//	if (1==r_tbl[shot_type][REI03_yudo])	/* pspは0レジスタがあるので0と比較したほうが速い */
			if (0 != r_tbl[shot_type][REI03_yudo])/* 誘導ショットか？ */
			#else
		//	if (REIMU_SHOT_TYPE_01 > shot_type)/* 誘導ショットか？ */
			if (REIMU_SHOT_TYPE_00 == shot_type)/* 誘導ショットか？ */
			#endif
			{
				OBJ *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
				OBJ *target;
				target = obj_teki_search();
				if (target != zzz_player)/* 見つかったら */
				{
					h->PL_SHOT_DATA_target_x256  = target->center.x256;/* 誘導目標座標を覚えとく */
				//	h->PL_SHOT_DATA_target_y256  = target->center.y256;/* 誘導目標座標を覚えとく */
					//
				//	if (REIMU_SHOT_TYPE_00 == shot_type)/* 誘導ショットか？ */
					{
						REG_00_SRC_X	= (target->center.x256);			/*(狙い先)*/
						REG_01_SRC_Y	= (target->center.y256);			/*(狙い先)*/
						REG_02_DEST_X	= (zzz_player->center.x256);		/*(狙い元)*/
						REG_03_DEST_Y	= (zzz_player->center.y256);		/*(狙い元)*/
						tmp_angleCCW65536_src_nerai();
						int int_angle1024;
						int_angle1024	= HATSUDAN_03_angle65536;
						int_angle1024 >>= (6);
					//	if (511 < int_angle1024 )/* 上方180-360度[/360度]の領域のみ追尾する */
						if (/*(384)*/(256*3) < (( int_angle1024+128+256)&(1024-1)) )/* 上方225-315度[/360度]の領域のみ追尾する */	/* CCWの場合 */
						{
							/*16==ccc_tbl[CCC_SPEED+(cg_ga me_select_player)]*/
							/*16==ccc_tbl[CCC_SPEED+(cg_ga me_select_player)]*/
							#if (0)//
							h->math_vector.x256  = si n1024((int_angle1024))*(/*p->speed*/(16));/*fps_factor*/		/* CCWの場合 */
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
							/* 描画用グラ回転 */
							if (MARISA_A==(cg_game_select_player))/* (魔理沙A魔符の)マジックミサイルは傾かない。 */
							{	int_angle1024 = (0);	}
							else/* (霊夢A霊符の)ホーミングアミュレットは模倣風では傾ける事にする。 */
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
		//	h->math_vector.x256 = ((/*int_vx*/r_tbl[shot_type][REI02_vx256]))*/*p->speed*/(ccc_tbl[CCC_SPEED+OFFS_IS_NEEDLE]);/*fps_factor*/
		//	h->math_vector.y256 = -(((int)ccc_tbl[CCC_SPEED+OFFS_IS_NEEDLE])<<8);/*fps_factor*/ /*p->speed*/
			h->math_vector.x256 = ((/*int_vx*/r_tbl[shot_type][REI02_vx256]))*/*p->speed*/(current.shot_status[SHOT_STATUS_06_SHOT_SPEED+is_needle]);/*fps_factor*/
			h->math_vector.y256 = -(((int)current.shot_status[SHOT_STATUS_06_SHOT_SPEED+is_needle])<<8);/*fps_factor*/ /*p->speed*/
		}
	}
}


/*---------------------------------------------------------
	プレイヤー弾(ショットのみ)生成
---------------------------------------------------------*/

/* 霊夢A (誘導巫女) 誘導ショット */

static void shot_regist_re_yudou(OBJ *s) /* 霊夢A 魔理沙A */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_00);/* 誘導ショット */
}

static void shot_regist_yuyuko(OBJ *s)	/* 幽々子 */
{
	player_register_shot_object(s, NEEDLE_ANGLE_271);
}

/* 霊夢B (針巫女) / 魔理沙A/B / レミリア ＆ チルノ */

static void shot_regist_re_hari(OBJ *s) /* 魔理沙A/B */
{
	player_register_shot_object(s, NEEDLE_ANGLE_270_M);
}

/*---------------------------------------------------------
	オプションの攻撃を成長段階によってかえる。
	-------------------------------------------------------
	 霊夢 B(3針に成長)
	レミリア
	チルノ A
	チルノ Q
---------------------------------------------------------*/
static void shot_regist_ci_gggg(OBJ *s) /* 霊夢 B ＆ レミリア ＆ チルノ */
{
	if (REIMU_B==(cg_game_select_player))/* 霊夢Bは3針に成長する。 */
	{	/*(3針)*/	/* 霊夢 Bのみ */
		player_register_shot_object(s, NEEDLE_ANGLE_270_L );
		player_register_shot_object(s, NEEDLE_ANGLE_270_M );
		player_register_shot_object(s, NEEDLE_ANGLE_270_R );
	}
	else	/* ＆ チルノ */
//	if (REMILIA==(cg_game_select_player))/* */
	{	/*(左右に分かれる)*/	/* レミリアのみ */
		player_register_shot_object(s, NEEDLE_ANGLE_277 );
		player_register_shot_object(s, NEEDLE_ANGLE_263 );
	}
}
static void shot_regist_ma_lazer(OBJ *s) /* 魔理沙B レーザー */
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
	//	if ((43)>(count128 ))	/* 強力だが、1/3時間しか効果がない。(r33t4) */
		if ((26)>(count128 ))	/* 強力だが、1/5時間しか効果がない。(r33u1) */
		#endif
		{
			shot_regist_re_hari(s);
		}
	}
}


/*---------------------------------------------------------
	プレイヤー弾(ショットのみ)生成
---------------------------------------------------------*/

	/* 通常ショット(narrow 1wayの場合) */
static void regist_n1_way(OBJ *s)
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* 通常ショット */
}
	/* 通常ショット(narrow 2wayの場合) */
static void regist_n2_way(OBJ *s)
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* 通常ショット */
}
	/* 通常ショット(narrow 3wayの場合) */
static void regist_n3_way(OBJ *s)
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* 通常ショット */
}
	/* 通常ショット(narrow 4wayの場合) */
static void regist_n4_way(OBJ *s)
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* 通常ショット */
}
	/* 通常ショット(narrow 5wayの場合) */
static void regist_n5_way(OBJ *s)
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_06); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_07); 	/* 通常ショット */
}

/*---------------------------------------------------------
	霊夢A用(ショット弾)生成
---------------------------------------------------------*/

	/* 通常ショット(wide 4wayの場合) */
static void regist_w4_way(OBJ *s)
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_06); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_07); 	/* 通常ショット */
}
	/* 通常ショット(wide 5wayの場合) */
static void regist_w5_way(OBJ *s)
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_08); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_09); 	/* 通常ショット */
}
	/* 通常ショット(wide 6wayの場合) */
static void regist_w6_way(OBJ *s)
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_08); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_06); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_07); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_09); 	/* 通常ショット */
}

/*---------------------------------------------------------
	幽々子用(ショット弾)生成
---------------------------------------------------------*/
	/* 通常ショット(幽々子の場合) */
static void regist_0_way(OBJ *s)
{
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_08);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_09);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_0a);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_0b);
}


/*---------------------------------------------------------
	オプションの攻撃を成長段階によってかえる。
	-------------------------------------------------------
	 霊夢 B(3針に成長)
	レミリア
	チルノ A
	チルノ Q
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
{	/* 攻撃支援 */
	if (src->PL_OPTION_DATA_opt_anime_add_id < OPTION_C3)
	{
	// OPTION_C1: OPTION_C2:
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
	// OPTION_C3: OPTION_C4:
		shot_regist_re_hari(src);		//	src->PL_OPTION_DATA_opt_shot_interval=(5+3);
	}
}

/*---------------------------------------------------------
	オプション アニメーション
---------------------------------------------------------*/

static OBJ_CALL_FUNC(player_animate_option)
{
//	if (0==anime_fix_status[BASE_OPT_ANIM_TYPE+(cg_ga me_select_player)])
	if (0==current.shot_status[SHOT_STATUS_26_OPTION_ANIME_KAITEN])
	{	/* オプションが回転する場合 */
		/* 描画用角度(下が0度で左回り(反時計回り)) */
	//	src->rotationCCW1024 += (anime_fix_status[BASE_OPT_ANIM_SPEED+(cg_ga me_select_player)]);	/* 描画用グラ回転速度 */
		src->rotationCCW1024 += (current.shot_status[SHOT_STATUS_27_OPTION_ANIME_SPEED]);	/* 描画用グラ回転速度 */
		mask1024(src->rotationCCW1024);
	}
	else
	{	/* オプションがアニメーションする場合 */
		src->PL_OPTION_DATA_anime_wait--;
		if (src->PL_OPTION_DATA_anime_wait <= 0)
		{
		//	src->PL_OPTION_DATA_anime_wait = (anime_fix_status[BASE_OPT_ANIM_SPEED+(cg_ga me_select_player)]);
			src->PL_OPTION_DATA_anime_wait = (current.shot_status[SHOT_STATUS_27_OPTION_ANIME_SPEED]);
			#if 1
			src->obj_type_set++;
			src->obj_type_set &= (0xfffffff7); /* 上位ビットに影響を与えないでアニメーション */
			#endif
		}
	}
}


/*---------------------------------------------------------
	プレイヤー、オプションの移動制御
---------------------------------------------------------*/
/*static*/extern OBJ_CALL_FUNC(player_control_option);	/* 全員 */
/*static*/global OBJ_CALL_FUNC(player_move_option)		/* 全員 */
{
	/* オプションが非表示の場合、何もしない。 */
	if (PL_OPTION_FLAG_00_OPTION_OFF==(src->PL_OPTION_DATA_yuukou_flag)) {	return; 	}
//
	/* アニメーション */
	player_animate_option(src);
//
	/* シナリオ会話モードではショットボタン入力無効 */
//	if (0==(cg.state_flag & STATE_FLAG_0x0200_IS_KAIWA_MODE))
	if (cg.state_flag & JIKI_FLAG_0x0080_SHOT_KEY_SEND_FOR_OPTION)/* off(オプションがショットを撃つ場合) */
	{
		src->PL_OPTION_DATA_opt_shot_interval--;
		if ((src->PL_OPTION_DATA_opt_shot_interval) < 0)
		{
		//	src->PL_OPTION_DATA_opt_shot_interval = player_bbb_status[(cg.jiki_weapon_level_offset)];/* オプションのショット間隔 */
			src->PL_OPTION_DATA_opt_shot_interval = current.shot_status[SHOT_STATUS_18_OPTION_SHOT_INTERVAL_L1+(cg.jiki_weapon_level)];/* オプションのショット間隔 */
			#if 1//
			static /*const*/ void (*ggg[PLAYERS8])(OBJ *sss) =
			{
				/*REIMU_A*/ 	shot_regist_re_yudou,			/* 霊夢 A(誘導赤札) */
				/*REIMU_B*/ 	shot_regist_option_glow,		/* [成長オプションの場合]霊夢 B(針) ＆ レミリア ＆ チルノ */
				/*MARISA_A*/	shot_regist_re_yudou,			/* 魔理沙 A(誘導森弾) */
				/*MARISA_B*/	shot_regist_ma_lazer,			/* 魔理沙 B(きまぐれレーザー) */
				/*REMILIA*/ 	shot_regist_option_glow,		/* 霊夢 B(針) ＆ レミリア ＆ チルノ */
				/*YUYUKO*/		shot_regist_yuyuko, 			/* 幽々子(蝶) */
				/*CIRNO_A*/ 	shot_regist_option_glow,		/* [成長オプションの場合]霊夢 B(針) ＆ レミリア ＆ チルノ */
				/*CIRNO_Q*/ 	shot_regist_option_glow,		/* [成長オプションの場合]霊夢 B(針) ＆ レミリア ＆ チルノ */
			};
			(ggg[(cg_game_select_player)])(src);
			#endif
		}
	}
	player_control_option(src);
}


/*---------------------------------------------------------
	自機のショットボタン押しっぱなしで自動ショットを撃つ場合
---------------------------------------------------------*/

global void register_main_shot(OBJ *s1)
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
		static int weapon_interval = (0);
		/*cg.*/weapon_interval--;/*fps_fa_ctor*/
		if (/*cg.*/weapon_interval <= 0)
		{
		//	/*cg.*/weapon_interval = jiki_auto_shot_interval_table[(weapon_List<<3)+(cg_ga me_select_player)];
		//	/*cg.*/weapon_interval = jiki_auto_shot_interval_table[(cg.jiki_weapon_level_offset)];
			/*cg.*/weapon_interval = current.shot_status[SHOT_STATUS_10_JIKI_SHOT_INTERVAL_L1+(cg.jiki_weapon_level)];
			voice_play(VOICE00_SHOT, TRACK00_JIKI_SHOT);
			static /*const*/ void (*bbb[(WEAPON_L_MAX)*(PLAYERS8)])(OBJ *sss) =
			{	/* 誘導巫女(A)	 針巫女(B)		 ミサマリ(A)	 レイマリ(B)  */
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
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	各プレイヤー(霊夢 ＆ 魔理沙 ＆ レミリア ＆ チルノ ＆ 幽々子)
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
//	int time_out;			/* 設定時間内に見つけられない場合はおしまい */
//	OBJ *target_obj;	/* int target_id */
//} PL_HOMING_DATA; 		/* == PL_HLASER_DATA */

#define YUUDOU_BOMBER_speed256					user_data01
#define YUUDOU_BOMBER_reimu_musou_id			user_data03
//efine PL_HOMING_time_out						YUUDOU_BOMBER_reimu_musou_id		/* 設定時間内に見つけられない場合はおしまい */
//efine PL_HOMING_KODOMO_DATA_color256			user_data04

//	int angleCCW1024;
//	int nr; 				/* 右か左(ホーミング), nr==number */


//typedef struct
//{
#define PL_HOMING_KODOMO_DATA_check_x256		user_data00
#define PL_HOMING_KODOMO_DATA_check_y256		user_data01
/*???*/#define PL_HOMING_KODOMO_DATA_time_out	user_data03 		/* 設定時間内に見つけられない場合はおしまい */
//#define PL_HOMING_KODOMO_DATA_color256		user_data04
//	OBJ *target_obj;	/* int target_id */
//} PL_HOMING_KODOMO_DATA;	/* == PL_HLASER_DATA */

//typedef struct
//{
#define PL_KEKKAI_DATA_angleCCW1024 			user_data00
#define PL_KEKKAI_DATA_add_r1024				user_data01
#define PL_KEKKAI_DATA_radius					user_data03 		/* 半径(radius) */
//} PL_KEKKAI_DATA;



/*
collision_check あたり判定 -------------------------------------------------------------------------
*/

/*---------------------------------------------------------
	プレイヤー、高速時ボム。画面外はみ出しチェック。あたり判定チェック。
---------------------------------------------------------*/
static OBJ_CALL_FUNC(player_bomber_out_collision_check)
{
	#if 1/*Gu(中心座標)*/
	if ((src->center.x256 < t256(0))||(src->center.x256 > t256(GAME_WIDTH))||
		(src->center.y256 < t256(0))||(src->center.y256 > t256(GAME_HEIGHT)))
	#endif
	{
		src->jyumyou = JYUMYOU_NASI;/* 画面外に出たらおしまい */
	}
	else
	{
		player_weapon_collision_check(src, PLAYER_WEAPON_TYPE_01_BOMB_PLAYER);/* ボムで敵を倒すあたり判定 */
	}
}



/*
move ショット/ボム移動 -------------------------------------------------------------------------
*/

/*---------------------------------------------------------
	高速時ボムの移動(霊夢A)
	時計回り ＆ 反時計回り ＆ スクエア結界(霊夢B) 兼用
	高速時ボムの移動(幽々子)
---------------------------------------------------------*/

static OBJ_CALL_FUNC(reimu_yuyuko_move_rotate_kekkai) /* 霊夢 */
{
	src->PL_KEKKAI_DATA_angleCCW1024 += (src->PL_KEKKAI_DATA_add_r1024);	/*fps_factor*/	/*...*/
	mask1024(src->PL_KEKKAI_DATA_angleCCW1024);
	#if 1
	/* 描画用角度(下が0度で左回り(反時計回り)) */
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
	src->center.x256 = genten_cx256 + ((si n1024((src->PL_KEKKAI_DATA_angleCCW1024))*(src->PL_KEKKAI_DATA_radius)) );/*fps_factor*/ 	/* CCWの場合 */
	src->center.x256 = genten_cx256 + ((si n1024((src->PL_KEKKAI_DATA_angleCCW1024))*(src->PL_KEKKAI_DATA_radius)) );/* CCWの場合 */
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

	/* スクエア結界(霊夢B) */
//	if (REIMU_A != (cg_game_select_player))
	if (REIMU_B == (cg_game_select_player)) /* スクエア結界(霊夢B) */
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
		}	/* 少しずつ広がるようにした */
		else/* 回転が広がったらおしまい */
		{
			src->jyumyou = JYUMYOU_NASI;
			cg.state_flag &= (~JIKI_FLAG_0x0020_BOMBER_SYUU_SYUU);	/* ボムによる自動収集は終わり */
		}
	//	src->alpha				= ((int)(255-src->PL_KEKKAI_DATA_radius))&(256-1);	/* だんだん消えるようにした */
	//	src->color32			= ((((255-src->PL_KEKKAI_DATA_radius))&(0xff))<<24)|0x00ffffff; /* だんだん消えるようにした */
		src->color32			= ((((255-src->PL_KEKKAI_DATA_radius))&(0xff))<<24)|0x00ffffff; /* だんだん消えるようにした */
	}
//	else //if (YUYUKO==(cg_game_select_player))
//	{
//		if (pd_bomber_time<=0)	{	src->jyumyou = JYUMYOU_NASI;}/* 時間制限でおしまい */
//	}
	if ((ra_nd()&0xff) < cg.bomber_time)
	{
		if (0==((src->PL_KEKKAI_DATA_radius)&7))
		{
			//player_weapon_collision_check_kekkai(src, src->PL_KEKKAI_DATA_radius);
			bullets_to_hosi();/* 弾全部、星アイテムにする */
		}
	}
	player_weapon_collision_check(src, PLAYER_WEAPON_TYPE_01_BOMB_PLAYER);/* ボムで敵を倒すあたり判定 */
}


/*---------------------------------------------------------
	プレイヤー、高速時ボムの移動(魔理沙B マスタースパーク専用)
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
			/* 回転速度 */
		};
		/* 表示角度(下が0度で左回り(反時計回り)) */
		src->tmp_angleCCW1024	+= maspa_kaiten[((src->obj_type_set)& (0x03))];
	//	mask1024((src->tmp_angleCCW1024));
		{
			int sin_value_t256; 		//	sin_value_t256 = 0;
			int cos_value_t256; 		//	cos_value_t256 = 0;
			int256_sincos1024( ((((src->tmp_angleCCW1024))/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
			src->rotationCCW1024 = (( ((sin_value_t256)))>>2);
		}
	}
	/* 時間でおしまい */
	if (0 >= cg.bomber_time)	{ src->jyumyou = JYUMYOU_NASI;}/* 時間制限でおしまい */
	else
	if (0x40 > cg.bomber_time)	/* 0x3f ... 0x01 */
	{
		src->color32			= ((((cg.bomber_time))/*&(0x3f)*/)<<(24+2))|0x00ffffff; /* だんだん消えるようにした */
		/* (この値の範囲なら)実はマスク必要ない。 */
	}
	player_weapon_collision_check(src, PLAYER_WEAPON_TYPE_01_BOMB_PLAYER);/* ボムで敵を倒すあたり判定 */
//	player_bomber_out_collision_check(src);/* 画面外に出たらおしまい & ボムで敵を倒すあたり判定 */
}

/*---------------------------------------------------------
	プレイヤー、高速時ボムの移動(魔理沙、チルノ、仮幽々子(違う感じにしたい) )
---------------------------------------------------------*/

static OBJ_CALL_FUNC(marisa_yuyuko_move_levarie_gyastry_dream)
{
	src->center.x256 += src->math_vector.x256;	/*fps_factor*/
	src->center.y256 += src->math_vector.y256;	/*fps_factor*/
	#if 1
	/* YUYUKO MARISA_A 以外 */
	if (
	//	(CIRNO_A==(cg_game_select_player)) || (CIRNO_Q==(cg_game_select_player)) ||
		((CIRNO_A-1)<(cg_game_select_player)) //||
	//	(MARISA_B==(cg_game_select_player))/* マスパ(もどき) */
	)
	#endif
	{	/*チルノ用(暫定的)*/	/*1.5*(d->speed);*/ 	/*fps_factor*/
		src->center.y256 -= (abs((src->math_vector.x256+(src->math_vector.x256>>1) )));
		src->center.y256 -= (abs((src->math_vector.y256+(src->math_vector.y256>>1) )));
		src->center.y256 -= t256(1.0);
	}
	{

		/* 表示角度(下が0度で左回り(反時計回り)) */
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
	プレイヤー、高速時ボムの移動(レミリア)
---------------------------------------------------------*/

static OBJ_CALL_FUNC(remilia_move_burn_fire)
{
	#if 1
	// 加速
	// x1.10
//	src->math_vector.x256 = ((src->math_vector.x256 * t256(1.1))>>8);//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
//	src->math_vector.y256 = ((src->math_vector.x256 * t256(1.1))>>8);//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
	// x1.125
	src->math_vector.x256 += ((src->math_vector.x256)>>3);//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
	src->math_vector.y256 += ((src->math_vector.y256)>>3);//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
	// x1.0625
//	src->math_vector.x256 += ((src->math_vector.x256)>>4);//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
//	src->math_vector.y256 += ((src->math_vector.y256)>>4);//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
	/* 描画が重すぎるので加速性能、若干速く( x1.0625→ x1.125)する */
	//
	#endif
	src->center.x256 += src->math_vector.x256;	//	src->math_vector.x256;//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
	src->center.y256 += src->math_vector.y256;	//	src->math_vector.y256;//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
	//
	player_bomber_out_collision_check(src);
}

/*---------------------------------------------------------
	誘導弾の誘導移動サブルーチン
---------------------------------------------------------*/

static OBJ_CALL_FUNC(yuudou_idou)
{
	OBJ *target;	/* 目標 */
	target = src->target_obj;
	/* 目標が画面内ならば目標に向かう */
	if (
		(target->center.x256 > t256(0)) &&
		(target->center.x256 < (t256(GAME_WIDTH)) ) &&
		(target->center.y256 > t256(0)) &&
		(target->center.y256 < (t256(GAME_HEIGHT)) ))	/*Gu(中心座標)*/
	{
		#if (1)/*誘導(000)*/
		{int ta1024;
		#if 1
		/* CCWの場合 */
			REG_00_SRC_X	= (target->center.x256);		/*(狙い先)*/
			REG_01_SRC_Y	= (target->center.y256);		/*(狙い先)*/
			REG_02_DEST_X	= (src->center.x256);		/*(狙い元)*/
			REG_03_DEST_Y	= (src->center.y256);		/*(狙い元)*/
			tmp_angleCCW65536_src_nerai();
			ta1024 = HATSUDAN_03_angle65536;
			ta1024 >>= (6);
			#if 1
			/* 遅れ誘導 */
			/* KETMのは考え方が複雑なので破棄、0度からの差分で考え180度を境に分岐すれば同等(1周は360度しかないので) */
		//	const int sabun_aaa = (src->tmp_angleCCW1024-src->rotationCCW1024);
			const int sabun_aaa1024 = (ta1024-src->rotationCCW1024);
			if (0 > sabun_aaa1024/*(src->tmp_angleCCW1024-src->rotationCCW1024)*/ )/* 0レジスタ+共通最適化 */
			{	/* 目標角度 < 制御角度 */
				if ((1024/2) > sabun_aaa1024/*(src->tmp_angleCCW1024-src->rotationCCW1024)*/)/* 共通最適化 */
						{	src->rotationCCW1024 -= 10; }	/* 180度以内なら近いほうから回る */
				#if 0	/* 省略して問題ない？ */
				else	{	src->rotationCCW1024 += 10; }	/* 180度以上なら反対周りのほうが近い */
				#endif
			}
			else	/* 目標角度 > 制御角度 */
			{
				if ((1024/2) > -(sabun_aaa1024)/*(src->rotationCCW1024-src->tmp_angleCCW1024)*/)
						{	src->rotationCCW1024 += 10; }	/* 180度以内なら近いほうから回る */
				#if 0	/* 省略して問題ない？ */
				else	{	src->rotationCCW1024 -= 10; }	/* 180度以上なら反対周りのほうが近い */
				#endif
			}
			#else
			/* 直接誘導 */
			src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/ = ta1024;
			#endif
		#endif
		}
		mask1024(src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/);
		#endif/*誘導(000)*/
	}
	else	/* 目標が画面外ならば新しい目標を探す。 */
	{
		/* Ziel wurde anderweitig vernichtet, neues Ziel suchen */
		src->target_obj = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];		/* 画面外に逃げられた */
	}
}

/* ------------------------------------------------------------------------ */

/*---------------------------------------------------------
	プレイヤー、誘導弾の移動(親)制御
---------------------------------------------------------*/

enum
{
	HOMING_096_TIME_OUT = 96,
	HOMING_128_TIME_OUT = 128,
};
static OBJ_CALL_FUNC(marisa_move_parrent_hlaser)
{
	/* 他の誘導ボムが、既に倒したか？ */
	if (&obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER] == src->target_obj) /* 見つからない(他の誘導ボムが倒してしまった場合) */
	{
		/* ターゲット検索、敵を探す。 */
		src->target_obj = obj_teki_search();
	}
	else	/* まだターゲットが生きてる */
	{
		if (HOMING_096_TIME_OUT < src->jyumyou)
		{
			/* 一定時間減速 */
			src->YUUDOU_BOMBER_speed256 -= t256(0.5);/*減速*/
			// 止まると困る
			src->YUUDOU_BOMBER_speed256 = psp_max((src->YUUDOU_BOMBER_speed256), t256(0.75));	/* 最小値 */
		}
		else
		{
			/* 一定時間を超えたら加速 */
			src->YUUDOU_BOMBER_speed256 += t256(0.5);/*加速*/
			// 速過ぎるのも困る
			src->YUUDOU_BOMBER_speed256 = psp_min((src->YUUDOU_BOMBER_speed256), t256(16.0));	/* 最大値 */
		}
		//
		yuudou_idou(src);
	}
	#if 1
	/* (多分必要ないけど)念の為マスク */
	mask1024(src->rotationCCW1024);/*src->YUUDOU_BOMBER_angleCCW1024*/
	#endif
	#if 1
	/* 表示用 */
//	src->rotationCCW1024	= src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/;
	#endif
	/*src->YUUDOU_BOMBER_angleCCW1024*/
	#if (0)//
	src->center.x256 += ((si n1024((src->rotationCCW1024))*(src->YUUDOU_BOMBER_speed256))>>8);/*fps_factor*/		/* CCWの場合 */
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
//	if (0 > src->jyumyou ) /* 時間切れ？ */
//	{
//		src->jyumyou = JYUMYOU_NASI;		/* 時間切れなのでおしまい */
//	}
}

/*---------------------------------------------------------
	霊夢専用 低速ボム
	霊符霊夢
---------------------------------------------------------*/

static OBJ_CALL_FUNC(move_reimu_musou_fuuin)
{
	/* 他の誘導ボムが、既に倒したか？ */
	if (&obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER] == src->target_obj) /* 見つからない(他の誘導ボムが倒してしまった場合) */
	{
		/* ターゲット検索、敵を探す。 */
		src->target_obj = obj_teki_search();
	}
	else	/* まだターゲットが生きてる */
	{
		if (t256(3.0) > src->YUUDOU_BOMBER_speed256)
		{
			src->YUUDOU_BOMBER_speed256 += /*(int)*/t256(0.5);/*加速*/
		}
		//
		yuudou_idou(src);
	}
// ボムは最大 7 個(8)
//	if ((cg.bomber_time&0x0f)==(src->YUUDOU_BOMBER_reimu_musou_id&0x0f))
	if ((cg.bomber_time&0x07)==(src->YUUDOU_BOMBER_reimu_musou_id&0x07))
	{
		/*src->YUUDOU_BOMBER_angleCCW1024*/
		#if (0)//
		src->math_vector.x256 = ((si n1024((src->rotationCCW1024))*(src->YUUDOU_BOMBER_speed256))>>8);/*fps_factor*/		/* CCWの場合 */
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
	if (0 >= cg.bomber_time)	/* 時間切れの場合 */
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
		src->kougeki_ti--;/* 時間切れの場合、ボンバー(お札)の効力(威力)を減らす */
	}
//
	if (0 > src->kougeki_ti)	{	src->jyumyou = JYUMYOU_NASI;	}	/* ボンバー(お札)の効力(威力)切れでおしまい */
}
/*---------------------------------------------------------
	プレイヤー、誘導弾の移動(子供)
---------------------------------------------------------*/

static OBJ_CALL_FUNC(marisa_move_kodomo_hlaser)
{
	#if 1
	OBJ *oya;
	oya = src->target_obj;
	if (
			(JYUMYOU_NASI > oya->jyumyou) ||			/* 親が消去済みならば子も消去 */
		//	((OBJ_Z01_JIKI_GET_ITEM|OBJ_Z80_SHOT_SPECIAL) != oya->obj_type_set) 		/* (OBJ_Z01_JIKI_GET_ITEM|OBJ_Z80_SHOT_SPECIAL)以外は親でないので消去 */
			(0==(OBJ_Z80_SHOT_SPECIAL & oya->obj_type_set)) /* (OBJ_Z01_JIKI_GET_ITEM|OBJ_Z80_SHOT_SPECIAL)以外は親でないので消去 */
		)
	{
		src->jyumyou = JYUMYOU_NASI;/*おしまい*/
	}
//
	#if 1
	/* 表示用 */
	src->rotationCCW1024	= oya->rotationCCW1024;/*src->PL_HOMING_KODOMO_DATA_angleCCW1024*/
	#endif
	src->center.x256 = oya->center.x256;
	src->center.y256 = oya->center.y256;
//	src->PL_HOMING_KODOMO_DATA_color256 = ((oya->PL_HOMING_KODOMO_DATA_color256*t256(0.96))>>8)&0xff;
//	src->PL_HOMING_KODOMO_DATA_color256 = ((oya->PL_HOMING_KODOMO_DATA_color256*t256(0.90))>>8)&0xff;
//	src->PL_HOMING_KODOMO_DATA_color256 = ((oya->PL_HOMING_KODOMO_DATA_color256 - 0x10)&0xff);
//	src->color32 = (src->PL_HOMING_KODOMO_DATA_color256<<24)|(0x00ffffff);
	#if (1)/*???*/
	/* 数フレーム動かない場合は、自動消去する */
	if ((src->PL_HOMING_KODOMO_DATA_check_x256 != src->center.x256) ||
		(src->PL_HOMING_KODOMO_DATA_check_y256 != src->center.y256))
	{
		src->PL_HOMING_KODOMO_DATA_time_out = 8;/* 動いたら自動消去しない */
	}
	src->PL_HOMING_KODOMO_DATA_time_out--;
	if (0 > src->PL_HOMING_KODOMO_DATA_time_out)
	{
		src->jyumyou = JYUMYOU_NASI;/*おしまい*/
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
	/* 攻撃値: ==ボンバーの威力。 */
}
static void set_bomb_str_LOW(OBJ *h)
{
	h->m_Hit256R	= ((current.shot_status[SHOT_STATUS_1F_LOWER_BOMB_ATARI_HANKEI])<<8);
	h->kougeki_ti	= current.shot_status[SHOT_STATUS_17_LOWER_BOMB_STRENGTH];
	/* 攻撃値: ==ボンバーの威力。 */
}
/* (幽々子低速)十字炎ボムの炎の部分 */

static OBJ_CALL_FUNC(yuyuko_add_meifu)
{
		/* (24+(1))  、ここでの cg.bomber_time は、最大0x7fなので。 */
		src->color32		= (cg.bomber_time<<(24+(1)))|0x00ffffff;
		src->m_zoom.x256	+= (10);
		src->m_zoom.y256	+= (10);
//	int j;
//	for (j=0; j<(1); j++)
	{
		OBJ *h;
		h = obj_regist_teki();
		if (NULL!=h)/* 登録できた場合のみ */
		{	set_bomb_str_LOW(h);
			h->obj_type_set 			= (JIKI_BOMBER_01|OBJ_Z80_SHOT_SPECIAL);/* ボスに有効 */
			h->callback_mover	= remilia_move_burn_fire;
		//	h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x64);		/*	h->alpha			= 0x64 100;*/
			h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x64);		/*	h->alpha			= 0x64 100;*/
			#if 1
			/* 描画用角度(下が0度で左回り(反時計回り)) */
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
			HATSUDAN_01_speed256	= ((rnd_spd256));/*(t256形式)*/
			HATSUDAN_03_angle65536	= deg1024to65536(ang_aaa_1024);
			sincos256();/*(破壊レジスタ多いので注意)*/
			h->math_vector.x256 = REG_03_DEST_Y;//sin_value_t256	// 縦
			h->math_vector.y256 = REG_02_DEST_X;//cos_value_t256	// 横
			}
		}
	}
}

/* 十字ショットボムの炎の部分 */

static OBJ_CALL_FUNC(remilia_add_burn_shot)
{
	{
		OBJ *h;
		h = obj_regist_teki();
		if (NULL!=h)/* 登録できた場合のみ */
		{	set_bomb_str_STD(h);
			//
//			h->obj_type_set 			= (/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_04|OBJ_Z80_SHOT_SPECIAL)/*ボスに有効*/;
			h->obj_type_set 			= (/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_01|OBJ_Z80_SHOT_SPECIAL);/*ボスに有効*/
			h->callback_mover	= remilia_move_burn_fire;
//			h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x64);		/*	h->alpha			= 0x64 100;*/
			h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	h->alpha			= 0x64 100;*/
			#if 1/*Gu(中心座標)*/
			h->center.x256			= src->center.x256;
			h->center.y256			= src->center.y256+t256(15);
			#endif
			{
			#if 1
			/* 描画用角度(下が0度で左回り(反時計回り)) */
//			h->rotationCCW1024		= ang_aaa_1024;
			h->rotationCCW1024		= (0);
			#endif
			h->math_vector.x256 	= (src->math_vector.x256);/*fps_factor*/ /* CCWの場合 */
			h->math_vector.y256 	= (src->math_vector.y256);/*fps_factor*/
			u32 rnd_spd 		= (ra_nd() ) ;
			if (rnd_spd&0x03)	{	h->math_vector.x256 += h->math_vector.x256; }/* [位置2倍(正/負)-2なら-4, 2なら4 ] */
			if (rnd_spd&0x01)	{	h->math_vector.x256 = - h->math_vector.x256;	}
			if (rnd_spd&0x02)	{	h->math_vector.y256 = - h->math_vector.y256;	}
			}
		}
	}
}

/*---------------------------------------------------------
	霊夢 A 専用 低速ボム追加
	霊符霊夢
	ボム:夢想封印
	ホーミング光弾を7発出す。敵がいない時はしばらくその場に停滞。
	威力は恋符についで高く、無敵時間も長い。光弾の軌跡、炸裂に弾消し。
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
			if (NULL!=h)/* 登録できた場合のみ */
			{	set_bomb_str_LOW(h);
	//			h->obj_type_set 			= (OBJ_Z01_JIKI_GET_ITEM|OBJ_Z80_SHOT_SPECIAL)/*ボスに有効*/;
				h->obj_type_set 			= (/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_01|OBJ_Z80_SHOT_SPECIAL);/*ボスに有効*/
				#if 1/*Gu(中心座標)*/
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
					h->rotationCCW1024	= ( (((int)cg.bomber_time)<<3)+(512));/* 射出する方向 */
					#if 0
					/* (多分必要ないけど)念の為マスク */
					mask1024(src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/);
					#endif
					h->YUUDOU_BOMBER_speed256				= t256(1.2);
					h->jyumyou								= byou60(20);	/* 20秒 テキトー */ 	/* (100) (200)*/
					h->YUUDOU_BOMBER_reimu_musou_id 		= (musou_id);	/*(ii)(200)*/
					h->target_obj							= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
				/* (h->target_obj == obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER])==見つからない */
				}
			}
		}
	}
}

/*
add2-------------------------------------------------------------------------
*/
/*---------------------------------------------------------
	プレイヤー誘導弾生成
---------------------------------------------------------*/

static OBJ_CALL_FUNC(marisa_add_teisoku_yuudoudan)
{
	int j;	j=(ra_nd()&(2-1));//for (j=0;j<2;j++)
	{
		OBJ *h_old; h_old = NULL;
		OBJ *h; 	h = NULL;
		//#define hlaser_NUM_OF_ENEMIES (24)
		//#define hlaser_NUM_OF_ENEMIES (12)		/* 半分にしてみる */
		#define hlaser_NUM_OF_ENEMIES (8)			/* 8枚位で十分かも(?) */
		unsigned int i;
		unsigned int tmp_color32;
		tmp_color32 = 0xffffffff;
		for (i=0; i<hlaser_NUM_OF_ENEMIES; i++)
		#undef hlaser_NUM_OF_ENEMIES
		{	h_old = h;
			h					= obj_regist_teki();
		//	if (NULL!=h)/* 登録できた場合のみ */	/* 強制登録 */
			if (NULL==h)return;/* 登録できた場合のみ */
			{
/* うーん... */
				set_bomb_str_LOW(h);
//				h->obj_type_set 			= (OBJ_Z01_JIKI_GET_ITEM|OBJ_Z80_SHOT_SPECIAL)/*ボスに有効*/;
				h->obj_type_set 			= (/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_01|OBJ_Z80_SHOT_SPECIAL)/*ボスに有効*/;
				#if 1/*Gu(中心座標)*/
			//	h->center.x256			= src->center.x256 - t256(5);
			//	h->center.y256			= src->center.y256 + t256(15);
				h->center.x256			= src->center.x256;
				h->center.y256			= src->center.y256;
				#endif
//				h->PL_HOMING_KODOMO_DATA_color256		= (t256(1.0));	/* 明るさ(アルファ初期値) */
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
				/* (h->target_obj==obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER])==見つからない */
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
//	 0	1  2  3  4	5  6  7  8	9 10 11 [移動処理順→]
//	↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓
//	 p	0  1  2  3	4  5  6  7	8  9 10
// 結果的に 全部重なるバグ。

//(r32)
//	11	10	9  8  7  6	5  4  3  2	1  0 [移動処理順→]
//	↓	↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓
//	10	 9	8  7  6  5	4  3  2  1	0  p

/* tuika 追加系 ------------------------------------------------------------------------ */

static OBJ_CALL_FUNC(bomber_hontai_off_check)
{
	if (0 >= cg.bomber_time)
	{
		src->callback_mover = NULL;/*(off!)*/
	}
/* 時間制限でおしまい */	//	if (0 >= cg.bomber_time)	{ src->jyumyou = JYUMYOU_NASI;}
}

/*---------------------------------------------------------
	(霊夢)夢想封印の追加[ボム追加本体]
	霊夢 A (通常ボム) / チルノ A(低速ボム) の親
---------------------------------------------------------*/

static OBJ_CALL_FUNC(bomber_hontai_reimu_tuika_musofuuin_shot)
{
	/* シナリオ時には誘導弾を追加しない */
	if (IS_SCENE_KAIWA_MODE)	{	return; 	}
	bomber_hontai_off_check(src);
	//
	/*(r36テスト、32 未満は追加しない)*/
	if (32 > cg.bomber_time)	{	return; 	}
	//
	OBJ *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
	//
//	if (0x3f==(((int)pd_bomber_time)&0x3f))
//	if (0x0f==(((int)pd_bomber_time)&0x0f))
	if (0x0f==((src->jyumyou)&0x0f))/* 波動を撃つタイミング */
	{
		reimu_add_reifu_musofuuin(zzz_player);/* 波動発動(早口言葉?) */
	}
	#if 1/*Gu(中心座標)*/
	src->center.x256 = zzz_player->center.x256;
	src->center.y256 = zzz_player->center.y256;
	#endif
}

/*---------------------------------------------------------
	魔理沙、チルノ等、低速ボムの追加[ボム追加本体]
---------------------------------------------------------*/
static OBJ_CALL_FUNC(bomber_hontai_marisa_tuika_bomber_teisoku_yuudoudan)
{
	/* シナリオ時には誘導弾を追加しない */
	if (IS_SCENE_KAIWA_MODE)	{	return; 	}
	bomber_hontai_off_check(src);
	//
	OBJ *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
	//
//	if (0==(((int)pd_bomber_time)&0x3f))
//	if (0==(((int)pd_bomber_time)&0x0f))/*(r31)*/
	if (0==((src->jyumyou)&0x07))/*(r32)追加量を2倍にしてみる。*/
	{
		/* シナリオ時には誘導弾を追加しない */
	//	if (0==(cg.state_flag & STATE_FLAG_0x0200_IS_KAIWA_MODE))
		{
			marisa_add_teisoku_yuudoudan(zzz_player);
		}
	}
}

/*---------------------------------------------------------
	レミリアボムの十字ショットの追加[ボム追加本体]
---------------------------------------------------------*/

static OBJ_CALL_FUNC(bomber_hontai_remilia_tuika_cross_shot)
{
	/* シナリオ時には誘導弾を追加しない */
	if (IS_SCENE_KAIWA_MODE)	{	return; 	}
	bomber_hontai_off_check(src);
	//
	/* シナリオ時には誘導弾を追加しない */
//	if (0==(cg.state_flag & STATE_FLAG_0x0200_IS_KAIWA_MODE))
	{
		remilia_add_burn_shot(src);/* 波動発動(早口言葉?) */
	}
	#if 1/*Gu(中心座標)*/
	/* 親移動 */
	src->center.x256 += src->math_vector.x256;
	src->center.y256 += src->math_vector.y256;
	#endif
}

/*---------------------------------------------------------
	(幽々子低速)レミリアボムの十字炎の追加[ボム追加本体]
---------------------------------------------------------*/

static OBJ_CALL_FUNC(bomber_hontai_yuyuko_tuika_meifu)
{
	/* シナリオ時には誘導弾を追加しない */
	if (IS_SCENE_KAIWA_MODE)	{	return; 	}
	bomber_hontai_off_check(src);
	//
	yuyuko_add_meifu(src);/* 波動発動(早口言葉?) */
	#if 1/*Gu(中心座標)*/
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
parrent 親系 -------------------------------------------------------------------------
*/
/*---------------------------------------------------------
	プレイヤーシールド生成の親(霊夢)
---------------------------------------------------------*/

/*static*/global OBJ_CALL_FUNC(reimu_yuyuko_create_bomber_kekkai_parrent)		/* 霊夢 */		//シールドの追加
{
	int ii;//	int i;	/* 半象限ずつ */
	int jj;
	ii = 0;
//	for (ii=0; ii<(8); ii++)//	for (i=0; i<360; i+=45)
	for (jj=0; jj<(1024);  )//	for (i=0; i<360; i+=45)
	{
		OBJ *h;
		h				= obj_regist_teki();
	//	if (NULL!=h)/* 登録できた場合のみ */
		if (NULL==h)return;/* 登録できた場合のみ */
		{
			h->m_Hit256R					= TAMA_ATARI_04;
			h->kougeki_ti					= (8*1);	/*5*/
			/* 攻撃値: ==お札の効力。==ボンバーの威力。 */
			#if 1
			if (YUYUKO==(cg_game_select_player))
			{
				if (0 == (ii&1))// if (i%90 == 0)
				{
					h->PL_KEKKAI_DATA_add_r1024 	= ( 4); /* 時計回り */
					h->PL_KEKKAI_DATA_radius		= (16); 	/*(48)*/	//d->radius=38; 	/* 少しずつ広がる半径初期値 */
				}
				else
				{
					h->PL_KEKKAI_DATA_add_r1024 	= (-3); /* 反時計回り */
					h->PL_KEKKAI_DATA_radius		= (16); 	/*(45)*/	//d->radius=35; 	/* 少しずつ広がる半径初期値 */
				}
			}
			else
			#endif
			{
				if (0 == (ii&1))// if (i%90 == 0)
				{
					h->PL_KEKKAI_DATA_add_r1024 	= ( 46);	/* 時計回り */
					h->PL_KEKKAI_DATA_radius		= (16); 	/*(48)*/	//d->radius=38; 	/* 少しずつ広がる半径初期値 */
				}
				else
				{
					h->PL_KEKKAI_DATA_add_r1024 	= (-34);	/* 反時計回り */
					h->PL_KEKKAI_DATA_radius		= (0);		/*(45)*/	//d->radius=35; 	/* 少しずつ広がる半径初期値 */
				}
			}
			/* 霊夢 ＆ 幽々子 */
			h->obj_type_set 			= (ii&(4-1))+(/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_04|OBJ_Z80_SHOT_SPECIAL);/* ボスに有効 */
			h->callback_mover			= reimu_yuyuko_move_rotate_kekkai;		/*	時計回り ＆ 反時計回り ＆ スクエア結界(霊夢B) 兼用 */
			h->PL_KEKKAI_DATA_angleCCW1024		= (jj);//(ii<<7);	/* (1024/8) */
			/* 出始めはプレイヤー座標 */
		//	{
		//		OBJ *zzz_player;
		//		zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
		//		h->center.x256					= zzz_player->center.x256;
		//		h->center.y256					= zzz_player->center.y256;
				h->center.x256					= src->center.x256; /* 発弾時のプレイヤーの座標を引き継ぐ。 */
				h->center.y256					= src->center.y256; /* 発弾時のプレイヤーの座標を引き継ぐ。 */
		//	}
		}
		/* 霊夢Aは	 8 == 4弾 x 2種類 */
		/* 霊夢Bは	16 == 8弾 x 2種類 */
		/* 幽々子は 16 == 8弾 x 2種類 */
	//	jj += ((YUYUKO==(cg_game_select_player))?(1024/16):(1024/8));
		jj += ((REIMU_A==(cg_game_select_player))?(1024/8):(1024/16));
		ii++;
	}
	#if (1)/*(幽々子用)*/
	{
		OBJ *s1;
		s1					= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_02_BOMBER_HONTAI];
		s1->center.x256 		= (src->center.x256); /* 発弾時のプレイヤーの座標を引き継ぐ。 */
		s1->center.y256 		= (src->center.y256); /* 発弾時のプレイヤーの座標を引き継ぐ。 */
	}
	#endif
}

/*---------------------------------------------------------
	ギャストリドリーム
	プレイヤーボム生成の親
---------------------------------------------------------*/

//	/*static*/global OBJ_CALL_FUNC(yuyuko_create_bomber_gyastry_dream_parrent)		/* 霊夢 */		//シールドの追加
//{
//	reimu_create_bomber_kekkai_parrent(src);
//}

/*---------------------------------------------------------
	マスタースパーク、ボム生成の親
	(魔理沙B 専用)
---------------------------------------------------------*/

/*static*/global OBJ_CALL_FUNC(marisa_create_bomber_master_spark_parrent)
{
	int iii;
	for (iii=0; iii<(3)/*16*/; iii++)
	{
		OBJ *h;
		h					= obj_regist_teki();
	//	if (NULL!=h)/* 登録できた場合のみ */
		if (NULL==h)return;/* 登録できた場合のみ */ 	/*うーん？？？*/
		{	set_bomb_str_STD(h);

			static const u32 color_mask32[(4)] =
			{	//		   RED	 GREEN BLUE  ALPHA
				MAKE32RGBA(0xff, 0x00, 0x00, 0x3f), 	// (赤)
				MAKE32RGBA(0x00, 0xff, 0x00, 0x3f), 	// (緑)
				MAKE32RGBA(0x00, 0x00, 0xff, 0x3f), 	// (青)
				MAKE32RGBA(0xff, 0xff, 0xff, 0x3f), 	// (白)
			};
			h->color32					= (color_mask32[iii]);
			h->obj_type_set 					= (/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_04|OBJ_Z80_SHOT_SPECIAL);/* ボスに有効 */
			h->callback_mover			= marisa_move_master_spark;
			h->tmp_angleCCW1024 		= ((iii)<<8);
			h->m_zoom.x256				= (256*4);	/* 描画用ボムの初期サイズ / size of bomb at first. */
			h->m_zoom.y256				= (256*4);	/* 描画用ボムの初期サイズ / size of bomb at first. */

			h->math_vector.x256 				= (0);	/*fps_factor*/	/* CCWの場合 */
			h->math_vector.y256 				= (0);	/*fps_factor*/
			#if (1)/* 省略可能かも？ */
			h->center.x256					= src->center.x256;
			h->center.y256					= src->center.y256-t256(MASKER_SPARK_LENGTH144);
			#endif
//			/* 描画用角度(下が0度で左回り(反時計回り)) */
//			h->rotationCCW1024		= (0);
		}
	}
}


/*---------------------------------------------------------
	チルノ⑨専用
	プレイヤーボム生成の親
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
	//	if (NULL!=h)/* 登録できた場合のみ */
		if (NULL==h)return;/* 登録できた場合のみ */ 	/*うーん？？？*/
		{
			set_bomb_str_STD(h);
			//
			jjj++;
			jjj &= 0x03;	//	if (4==jjj) 	{	jjj=0;	}
			h->obj_type_set 		= (/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_04|OBJ_Z80_SHOT_SPECIAL);/* ボスに有効 */
			h->obj_type_set += jjj;
	//
			h->callback_mover	= marisa_yuyuko_move_levarie_gyastry_dream;
			#if 0
			{
				static const u16 player_fix_status_ggg[8] =
				{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
					256, 256, 256, 256, 32, 256, 32, 32,	/* 描画用ボムの初期サイズ / size of bomb at first. */
				};
				h->m_zoom.x256		= player_fix_status_ggg[(cg_game_select_player)];/*8*/ /* 64 == (1/4) */
			}
			#else
		//	if (YUYUKO!=(cg_game_select_player))
			{	/*(チルノ用)*/
				h->m_zoom.x256		= 1+(ra_nd()&(64-1));	/* 描画用ボムの初期サイズ / size of bomb at first. */
				h->m_zoom.y256		= 1+(ra_nd()&(64-1));	/* 描画用ボムの初期サイズ / size of bomb at first. */
			}
			#endif

			/* MARISAは強いが拡散してしまうのであたらない。 */
			/* チルノは当てやすいのでその分弱くしないと強すぎちゃう。 */
			int d_speed256;
			#if 0
			if (0==(iii&(2-1)/*%2*/))
			{			/*d->*/d_speed256	= t256(1.0);	c->alpha	= 150;	}
			else	{	/*d->*/d_speed256	= t256(1.2);	c->alpha	= 180;	}
			#else
			/*d->*/d_speed256	= t256(1.0)+(iii<<3);
			#endif
			#if (0)//
			h->math_vector.x256 	= ((si n1024((angCCW1024))*(d_speed256))>>8);/*fps_factor*/ 	/* CCWの場合 */
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
			/* 描画用角度(下が0度で左回り(反時計回り)) */
			h->rotationCCW1024		= angCCW1024;
			#endif
		//	angCCW1024 += (int)((1024)/16); // 22.5度/360, 2xπ/16 /* 0.392699081698724154810 */
			angCCW1024 += (int)((1024)/18); // 20.0度/360, 2xπ/18 /* 0.349065850398865915384 */
	//
			#if 1/*Gu(中心座標)*/
			h->center.x256			= src->center.x256;
			h->center.y256			= src->center.y256;
			#endif
			/* ギャストリドリーム(もどき)。スターダストレヴァリエと違い近接性能は無い。 */
//			if (YUYUKO==(cg_game_select_player))	/* 幽々子 */
//			{
//				h->center.x256		+= ((h->math_vector.x256)<<5);
//				h->center.y256		+= ((h->math_vector.y256)<<5);
//	//			/* 幽々子 強すぎるからな～(効果弱いが効果あるのも出来るようにシステム見直したほうがいいかも) */
//			}
		}
	}
}


/*---------------------------------------------------------
	魔符「スターダストレヴァリエ」
	-------------------------------------------------------
	[プレイヤーシールド]ボム生成の親(魔理沙 A)
	-------------------------------------------------------
    スターダストレヴァリエ(3x8==計24個星が出る)
    は8方向に星が出る。星の色は、決まっている。

        左上    上      右上
        (赤0)   (青2)   (緑1)

        左              右
        (緑1)           (青2)

        左下    下      右下
        (青2)   (赤0)   (緑1)

    赤==0
    緑==1
    青==2

    -------------------------------------------------------
    一方向では、3つ星が出る。画像の左上座標を(x,y)とすると、

    大星=半透明、座標(x,y)
    中星=半透明、座標(x,y)
    小星=不透明、座標(x+64,y+64)    // 64 は テキトーな一定値。
                                    // pspの場合は、画面が狭いので48が妥当？
    ---------------------------------------------------------*/

/*static*/global OBJ_CALL_FUNC(marisa_create_bomber_star_dust_levarie_parrent)
{
	int angCCW1024; angCCW1024 = (0);
	int mmm;
	mmm = (0x100);
	int kkk;
	for (kkk=0; kkk<(3); kkk++)/*3サイズ*/
	{
		int iii;
		for (iii=0; iii<(8); iii++)/*8方向*/
		{
			OBJ *h;
			h					= obj_regist_teki();
		//	if (NULL!=h)/* 登録できた場合のみ */
			if (NULL==h)return;/* 登録できた場合のみ */ 	/*うーん？？？*/
			{
				set_bomb_str_STD(h);
				//
				static const u32 color_mask32[8] =	/* 大星、中星は半透明。小星は必ず不透明。 */
				{	//		   RED	 GREEN BLUE  ALPHA
					MAKE32RGBA(0xff, 0x00, 0x00, 0x7f), 	//	  下(赤0)
					MAKE32RGBA(0x00, 0xff, 0x00, 0x7f), 	//	右下(緑1)
					MAKE32RGBA(0x00, 0x00, 0xff, 0x7f), 	//	右	(青2)
					MAKE32RGBA(0x00, 0xff, 0x00, 0x7f), 	//	右上(緑1)
					MAKE32RGBA(0x00, 0x00, 0xff, 0x7f), 	//	  上(青2)
					MAKE32RGBA(0xff, 0x00, 0x00, 0x7f), 	//	左上(赤0)
					MAKE32RGBA(0x00, 0xff, 0x00, 0x7f), 	//	左	(緑1)
					MAKE32RGBA(0x00, 0x00, 0xff, 0x7f), 	//	左下(青2)
				};
				h->color32					= (color_mask32[((iii)/*&0x07*/)]);
				h->obj_type_set 			= (/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_04|OBJ_Z80_SHOT_SPECIAL);/*ボスに有効*/
				//
				h->callback_mover			= marisa_yuyuko_move_levarie_gyastry_dream;
				h->m_zoom.x256				= (mmm);	/* 描画用ボムの初期サイズ / size of bomb at first. */
				h->m_zoom.y256				= (mmm);	/* 描画用ボムの初期サイズ / size of bomb at first. */
				{
					const int d_speed256	= t256(1.5);	/*(速度一定)*/
					#if (0)//
					h->math_vector.x256 	= ((si n1024((angCCW1024))*(d_speed256))>>8);/*fps_factor*/ 	/* CCWの場合 */
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
					/* 描画用角度(下が0度で左回り(反時計回り)) */
					h->rotationCCW1024		= angCCW1024;
					angCCW1024 += (int)((1024)/8);	// 45.0度/360, 64
					#endif
				#if 1/*Gu(中心座標)*/
				{
					h->center.x256			= src->center.x256;
					h->center.y256			= src->center.y256;
				}
				if (0==kkk)/*(mini star only.)(小星の場合。)*/
				{
					h->center.x256			+= (t256(48));	/*(小星のみ(x,y同じ値の)オフセット必ず付く。)*/
					h->center.y256			+= (t256(48));	/*(小星のみ(x,y同じ値の)オフセット必ず付く。)*/
					h->color32			|= (MAKE32RGBA(0, 0, 0, 0xff)); 	/*(not alpha)(小星は必ず不透明。)*/
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

//		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0xdc);		/*	h->alpha			= 0xdc;*/	/* 非表示 */
//	if (NULL!=h)/* 登録できた場合のみ */	/* うーん... */
	//	#if 1/*Gu(中心座標)*/
	//	h->center.x256			= t256(GAME_NOT_VISIBLE480);//(src->center.x256); /* 非表示 */
//	//	h->center.y256			= (src->center.y256); /* 非表示 */
	//	#endif


/*---------------------------------------------------------
	自機ショットの親を非表示にする。
---------------------------------------------------------*/

global void jiki_shot_oya_hide(void)
{
	OBJ *h;
	h						= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_02_BOMBER_HONTAI];
	{
		#if 1/*Gu(中心座標)*/
		h->center.x256			= t256(GAME_NOT_VISIBLE480);//(src->center.x256); /* 非表示 */
		h->center.y256			= t256(272+64); /* 非表示 */
		#endif
	}
}

/*---------------------------------------------------------
	プレイヤーシールド生成(レミリア)の親[レミリアボムの十字ショット本体]
---------------------------------------------------------*/

/*static*/global OBJ_CALL_FUNC(remilia_create_bomber_cross_shot_parrent)
{
	OBJ *h;
	h						= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_02_BOMBER_HONTAI];
	{
		h->callback_mover	= bomber_hontai_remilia_tuika_cross_shot;
		h->obj_type_set 			= (/*OBJ_Z01_JIKI_GET_ITEM*/JIKI_BOMBER_01|OBJ_Z80_SHOT_SPECIAL);/* ボスに有効 */
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
		#if 1/*Gu(中心座標)*/
		/*開始時の座標*/
		h->center.x256 = zzz_player->center.x256;
		h->center.y256 = zzz_player->center.y256;
		#endif
		{
			OBJ *obj_boss;
			obj_boss			= &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+TEKI_OBJ_00_BOSS00_HONTAI];/*(ボス本体)*/
			int int_angle1024;
			int int_aaa;
			REG_00_SRC_X	= (obj_boss->center.x256);			/*(狙い先)*/
			REG_01_SRC_Y	= (obj_boss->center.y256);			/*(狙い先)*/
			REG_02_DEST_X	= (zzz_player->center.x256);		/*(狙い元)*/
			REG_03_DEST_Y	= (zzz_player->center.y256);		/*(狙い元)*/
			tmp_angleCCW65536_src_nerai();
			int_angle1024 = HATSUDAN_03_angle65536;
			int_angle1024 >>= (6);
			#if (0)//
			int_aaa = si n1024((int_angle1024));	h->math_vector.x256  = int_aaa+int_aaa;/*fps_factor*/	/* CCWの場合 */
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
			/* 描画用グラ回転 */
		//	h->rotationCCW1024	= int_angle1024;
			/* 描画用グラ回転(ショットは上が正面なので、回転角を計算)[180/360度]回す。 */
			int_angle1024 += cv1024r((180));
			mask1024(int_angle1024);
			h->rotationCCW1024	= int_angle1024;/* 下が正面の角度が描画用角度 */
			#endif
		}
	}
}

/*---------------------------------------------------------
	魔理沙、チルノ等、低速ボムの親
---------------------------------------------------------*/
/*static*/global OBJ_CALL_FUNC(marisa_create_bomber_homing_parrent)
{
	OBJ *h;
	h						= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_02_BOMBER_HONTAI];
	{
		h->callback_mover	= bomber_hontai_marisa_tuika_bomber_teisoku_yuudoudan;
		h->obj_type_set 			= (/*表示しない*/SP_DUMMY_MUTEKI);/*ボスに有効*/	/* 低速ボムもボスに有効とする */
		#if 1/*Gu(中心座標)*/
		h->center.x256			= t256(GAME_NOT_VISIBLE480);//src->center.x256; /* 非表示 */
//		h->center.y256			= src->center.y256; /* 非表示 */
		#endif
//
		#if 1
		/* ボム中＆復活中、有効にすると、重複する。 */
		/* 特殊機能 */
		bullets_to_hosi();/* 弾全部、星アイテムにする */
		#endif
	}
}
/*---------------------------------------------------------
	霊夢 A (通常ボム) / チルノ A(低速ボム) の親
---------------------------------------------------------*/
/*static*/global OBJ_CALL_FUNC(reimu_create_bomber_homing_shot_parrent) /* reimu_move_add_bomber_hlaser */
{
	OBJ *h;
	h						= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_02_BOMBER_HONTAI];
	{
		h->callback_mover	= bomber_hontai_reimu_tuika_musofuuin_shot;
		h->obj_type_set 	= (JIKI_BOMBER_01|OBJ_Z80_SHOT_SPECIAL);/* ボスに有効 */
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
		#if 1/*Gu(中心座標)*/
		/*開始時の座標*/
		h->center.x256 = zzz_player->center.x256;
		h->center.y256 = zzz_player->center.y256;
		#endif
		{
			#if 1
			/* 描画用グラ回転 */
		//	h->rotationCCW1024	= int_angle1024;
			/* 描画用グラ回転(ショットは上が正面なので、回転角を計算)[180/360度]回す。 */
			h->rotationCCW1024	= 0;/* 下が正面の角度が描画用角度 */
			#endif
		}
	}
	#if 0
	/* ボム中＆復活中、有効にすると、重複する。 */
	/* 特殊機能 */
	bullets_to_hosi();/* 弾全部、星アイテムにする */
	#endif
}
/*---------------------------------------------------------
	幽々子専用 低速ボムの親 	[(幽々子低速)ボムの十字炎本体]
---------------------------------------------------------*/
/*static*/global OBJ_CALL_FUNC(yuyuko_create_bomber_meifu_parrent)
{
	OBJ *h;
	h						= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_02_BOMBER_HONTAI];
	{
		h->center.x256			= (src->center.x256); /* 発弾時プレイヤー座標を引き継ぐ */
		h->center.y256			= (src->center.y256); /* 発弾時プレイヤー座標を引き継ぐ */
		h->m_zoom.x256		= t256(1.00);
		h->m_zoom.y256		= t256(1.00);
		h->callback_mover	= bomber_hontai_yuyuko_tuika_meifu;
//		h->obj_type_set 	= (/*表示しない*/SP_DUMMY_MUTEKI); /* ボスの直接攻撃は禁止 */
		h->obj_type_set 	= (JIKI_SHOT_00); /* [幽々子ボムの扇本体] */ /* ボスの直接攻撃は禁止 */
		h->atari_hantei 	= (0); /* ボスの直接攻撃は禁止 */
		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x64);		/*	h->alpha			= 0x64 100;*/
		#if 1/*Gu(中心座標)*/
//		h->center.x256			= t256(GAME_NOT_VISIBLE480);//(src->center.x256); /* 非表示 */
//		h->center.y256			= (src->center.y256); /* 非表示 */
		#endif
	}
}

/*----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------
	各プレイヤーの性能差
---------------------------------------------------------*/
/* 霊夢 特殊能力：喰らいボムの受付時間が長い */
/* チルノ 特殊能力：⑨ */

/*---------------------------------------------------------
	プレイヤー。 ボムの寿命を設定
--------------------------------------------------------- */
global void jiki_shot_set_bomber_time(void)
{
	//	#define PD_BOMBER_JYUMYOU180					(180)		/* ボムの寿命==180[frame] */
	//	cg.bomber_time		= (PD_BOMBER_JYUMYOU180);//cg.bomber_time_table[(index_aaa)];
	cg.bomber_time		= (current.shot_status[SHOT_STATUS_24_PLAYER_SET_BOMBER_TIME]);//cg.bomber_time_table[(index_aaa)];
}
/*---------------------------------------------------------
	プレイヤー。グレイズの当たり判定半径を設定
--------------------------------------------------------- */
global int jiki_shot_get_player_graze_hankei(void)
{
	return (current.shot_status[SHOT_STATUS_23_PLAYER_GRAZE_ATARI_HANKEI]);
}

/*---------------------------------------------------------
	プレイヤーのアニメーション速度を設定
--------------------------------------------------------- */
global int jiki_shot_get_player_animation_speed(void)
{
	return (current.shot_status[SHOT_STATUS_25_PLAYER_ANIME_SPEED]);
}

/*---------------------------------------------------------
	喰らいボムの入力受け付け時間を設定
---------------------------------------------------------*/
global int jiki_shot_get_kurai_bombtimer(void)
{
	return ((-1)-current.shot_status[SHOT_STATUS_22_KURAI_BOMB_UKETUKE_JIKAN]);
}

/*---------------------------------------------------------
	ショット関連の性能の初期値を纏めて一括設定する。
---------------------------------------------------------*/

global void init_player_shot(void)
{
	SHOT_STATUS aaa_fix[(PLAYERS8)] =
	{
		#if 0/*(値の説明)*/
		// 弾1 ... 弾6: ショットの強さ L1 ... L6。
		// 針1 ... 針6: サブウェポンの強さ L1 ... L6。
		// 自1 ... 自6: 自機ショットを撃つ間隔 L1 ... L6。
		// オ1 ... オ6: オプションショットを撃つ間隔 L1 ... L6。
		// 弾速: ショットの進行速度。ショット自体の速さ。
		// 針速: サブウェポンの進行速度。サブウェポン自体の速さ。
		// 弾判: ショットのあたり判定半径。[pixel]
		// 針判: サブウェポンのあたり判定半径。[pixel]
		// 標ボ: 通常ボムの強さ
		// 低ボ: 低速ボムの強さ
		// 標判: 通常ボムのあたり判定半径。[pixel]
		// 低判: 低速ボムのあたり判定半径。[pixel]
		// 回速: ボムの回転速度。
		// 拡速: ボムの拡大速度。
		// 喰時: 喰らいボムの受け付け猶予減少時間。値が小さい程、受け付け時間が長い。 / hit_bomb_wait.
		// グ判: グレイズのあたり判定半径。[pixel]
		// ボ時: ボム有効時間。ボムの寿命。(通常ボムと低速ボム共通なので注意)
		// プア: プレイヤーのアニメーション速度。
		// 回転: オプションが回転アニメーションをする(0)かしない(それ以外)か。
		// ア速: オプションが回転の場合、回転速度。アニメーションの場合、アニメーション速度。
		{	/*(名前)*/
			{
				弾1,  弾2,	弾3,  弾4,	弾5,  弾6,		弾速,  針速,
				針1,  針2,	針3,  針4,	針5,  針6,		弾判,  針判,
				自1,  自2,	自3,  自4,	自5,  自6,		標ボ,  低ボ,
				オ1,  オ2,	オ3,  オ4,	オ5,  オ6		標判,  低判,
				回速, 拡速, 喰時, グ判, (ボ時), プア,	回転,  ア速,
			}
		},
		#endif
		{	/*(霊夢A)*/
			{
				 20,  23,  13,	11,   7,   5,		  9,  9,
				 30,  28,  26,	24,  22,  20,		  4, 10,
				  5,   5,	5,	 5,   5,   5,		 48,160,
				 99,  77,  55,	33,  10,   4,		  2, 16,	// (r33)誘導巫女のオプションは弱いと全然やる気がない。
				  0,   0,	0,	16, (180),	 8, 	  0,  4,
			}
		},
		{	/*(霊夢B)*/
			{
				 20,  16,	8,	 8,   4,   6,		  9, 18,	// 霊夢 (夢) - ニードル :: ニードルは通常弾の2倍速
				 16,  16,  10,	10,  10,  12,		  4,  6,
				  5,   5,	5,	 5,   5,   5,		 40,192,
				 13,  11,	9,	 7,   5,   3,		  2, 16,
				  0,   0,	0,	16, (180),	 8, 	  0,  4,
			}
		},
		{	/*(魔理沙A)*/
			{
				 20,  14,  16,	12,  16,  18,		  9,  6, // 6.3==9*0.7 魔理沙 (魔) - ミサイル :: ミサイルは通常弾の70％。
				 20,  20,  14,	12,  12,  14,		  4,  8,
				  6,   6,	5,	 5,   5,   5,		 40,240,
				 11,  10,	9,	 8,   7,   6,		 32, 16,
				 20,   0,	8,	32, (180),	 2, 	  0,  6,
			}
		},
		{	/*(魔理沙B / マスパ / 誘導レ)*/
			{
				 20,  22,  14,	12,  12,  10,		  9,  9, // ?? ??	   魔理沙 (恋) - レーザー :: スピード測定不能。というか全てにおいて測定困難。
				 32,  64,  80,	88,  98, 108,		  4,  6,
				  6,   6,	5,	 5,   5,   5,		255,255,	// 40
				  4,   4,	4,	 4,   4,   3,		128, 16,	// 255 128 マスパ
				 16,   0,	8,	32, (180),	 2, 	  0,  8,
			}
		},
		{	/* レミリア */
			{
				 40,  37,  36,	35,  34,  33,		  9,  3,
				 48,  48,  49,	49,  50,  52,		  4, 12,
				 14,  13,  12,	11,  10,  10,		 40,127,	//(r35u1, 40==8*5)	 32,127,
				  8,   8,	8,	 8,   8,   8,		 16, 16,
				  0,   0,  12,	16, (180),	 3, 	  0,  4,
			}
		},
		{	/* 幽々子 /  / 扇(赤弾) */
			{
			//	128, 129, 130, 132, 136, 144,		  4,  4,
				 80,  96, 112, 128, 132, 144,		  4,  4,
				 64,  64,  64,	65,  65,  66,		  8,  8,
				 17,  16,  15,	14,  13,  13,		 48,127,	// 40(r35u1, 40==8*5)	 48,160,
				 31,  28,  25,	22,  19,  16,		  4, 16,
				  0,   0,	4,	48, (255),	 4, 	  1, 12,
			}
		},
		{	/* チルノA */
			{
				 40,  40,  40,	40,  48,  64,		  8,  3,
				 48,  48,  48,	48,  56,  80,		  6, 16,
				  9,   9,	9,	 9,   9,   9,		240,240,
				  8,   8,	8,	 8,   7,   5,		 32, 16,
				 18,   3,	3,	48, (180),	 1, 	  1,  3,
			}
		},
		{	/* チルノ⑨ */
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


	#if 0/* 今まで検討したボム時間(主なもの) */
//	/* 低速ボム */			cg.bomber_time = 200/* 320 */ /* 400 */ /* 800 */;		/* ボムの有効時間(設定無敵時間)[※１仕様が変わったので半分にした] */
//	/* 霊夢 */				cg.bomber_time = 180/* 200 */;							/* ボムの有効時間(設定無敵時間) */	/* こちらはボス有効(攻撃型) */
//	/* 魔理沙	チルノ */	cg.bomber_time = 100/* 32 */ /* 100 */; 				/* ボムの有効時間(設定無敵時間) */
//	/* 扇 有効時間 */		cg.bomber_time = 255/* 255==((16*4*2*2)-1) */;			/* ボムの有効時間(設定無敵時間) */
//	レミリアボム			cg.bomber_time = 0x7e	/* 255==0xff==8発	254==0xfe==7発==0xbf=192 4発==0x7f 150==0x96 */;
	static const u8 cg.bomber_time_table[/* 16 */(PLAYERS8*2)] =
	{
		180,		180,		/* 180 200 霊夢 A */
		180,		200,		/* 180 200 霊夢 B */
		100,		180,		/* 100 200 魔理沙 A */
		100,		200,		/* 100 200 魔理沙 B */
		0x7e,		255,		/* 0x7e 200レミリア */
		180,		180,		/* 255 200 幽々子 */
		100,		180,		/* 100 200 チルノ A */
		100,		255,		/* 100 200 チルノ Q */
	};
	#endif
