
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ザコは纏める予定。
---------------------------------------------------------*/


#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 			user_data00 	/* 目標x座標 */
	#define target_y256 			user_data01 	/* 目標y座標 */
	#define toutatu_wariai256		user_data02 	/* 目標座標への到達割合 */
	#define kougeki_anime_count 	user_data03 	/* 攻撃アニメーション用カウンタ */
	#define boss_time_out			user_data04 	/* 制限時間 */
	#define boss_base_state777		user_data04 	/* 制限時間(boss_time_outと同じ) */
//
	#define boss_spell_timer		user_data05 	/* スペル時間 */
#endif

//#define zako_pattern_base 			user_data07 		/* パターンの基準位置。アニメする場合のベース */
#define zako_anime_rotate_angle1024 	user_data07/*zako_anime_base*/	/* アニメ回転角度1024 */

/*---------------------------------------------------------
	中-ボス敵やられ
---------------------------------------------------------*/
/*global*/static void item_create_15_rumia(SPRITE *src)
{
	const u8 item_tbl[4] =
	{
		(SP_ITEM_02_P128&0xff), 	/* [F] */
		(SP_ITEM_01_P008&0xff), 	/* [P]大 */
		(SP_ITEM_00_P001&0xff), 	/* [p]小 */
		(SP_ITEM_05_TENSU&0xff),	/* [点] */
	};
	item_create_num(src, /*item_type*/(SP_GROUP_ITEMS|(item_tbl[(cg_game_difficulty)])), (7) ); 	/* ちらばる */
}
/*global*/static void item_create_14_yokai(SPRITE *src)
{
	item_create_num(src, SP_ITEM_05_TENSU, (7) );	/* ちらばる */	/* 点数を出す */
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
	ザコ敵やられ
---------------------------------------------------------*/

/*global*/static/*static*/ void item_create_000_kougeki_mahoujin(SPRITE *src)
{
	if (rand_percent(30))	{	item_create_mono(src, SP_ITEM_00_P001  );}
	if (rand_percent(30))	{	item_create_mono(src, SP_ITEM_00_P001  );}
	if (rand_percent(30))	{	item_create_mono(src, SP_ITEM_05_TENSU );}
}


/*global*/static/*static*/ void item_create_002_inyou_zako(SPRITE *src)
{
//	if ( 0==(ra_nd()&(16-1)) ) /*確率上げた。[1/16]←[1/20]*/ /*%20*/
	if ( 0==(ra_nd()&( 8-1)) ) /*確率上げた。[1/8]←[1/20]*/ /*%20*/
	{
		br.BULLET_REGIST_00_speed256				= t256(5.0);				/* 弾速 */
		br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN; 	/* */
	//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
		br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_HARI32_01_AKA; 	/* [赤針弾] */
		br.BULLET_REGIST_06_n_way					= (5);						/* [5way] */
		br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/(24)); 		/* 分割角度 */
		bullet_regist_multi_vector_send1_xy_src(src);	/* 弾源x256 y256 中心から発弾。 */
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
//		if (0==(ra_nd()&(512-1)))	/* 1/512 ← 1/600 の確率確率でアイテム出す */	/*%600*/
//		{
//			item_create(src, SP_ITEM_00_P001, 1, ITEM_MOVE_FLAG_06_RAND_XY);
//		}



/*---------------------------------------------------------
	ザコショット共通
---------------------------------------------------------*/
static void zako_shot_ra_nd(SPRITE *src)
{
	if (ENEMY_LAST_SHOT_LINE256 > src->cy256)	/* このラインより下からは敵が撃たない */
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
			bullet_regist_multi_vector_send1_xy_src(src);	/* 弾源x256 y256 中心から発弾。 */
		}
	}
}

/*---------------------------------------------------------
	ザコ移動共通
---------------------------------------------------------*/
	#define target_x256 			user_data00
	#define target_y256 			user_data01
	#define radius256				user_data02
static void zako_move_type11_radius(SPRITE *src)
{
	src->cx256 = ((sin1024((src->tmp_angleCCW1024))*(src->radius256))>>8)+(src->target_x256);/* CCWの場合 */
	src->cy256 = ((cos1024((src->tmp_angleCCW1024))*(src->radius256))>>8)+(src->target_y256);
}
	#define speed256				user_data04 	/* 速度 */
	#define kaisu_nnn				user_data05 	/* 繰り返した回数のカウンタ */

/*---------------------------------------------------------
	ザコアニメ共通
---------------------------------------------------------*/
/* 0000 0100 0000 0000 = 0x0400 = 1024 */
/* 0000 0011 1--- ---- */
static void zako_anime_type_99_rotate(SPRITE *src)
{
	src->rotationCCW1024 += (src->zako_anime_rotate_angle1024);/*グラ回転*/
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
	src->rotationCCW1024 += (src->zako_anime_rotate_angle1024);/*グラ回転*/
	mask1024(src->rotationCCW1024);
}
/*---------------------------------------------------------

---------------------------------------------------------*/
	#define MAHOU_TEKI_wait1		user_data04
	#define MAHOU_TEKI_wait2		user_data05
	#define start_y256				user_data06
#include "zako_000_mahoujin.h"			/* 魔方陣[A/B] */

	#define limit_y256				user_data05 	/* 仮想地面 */
#include "zako_002_inyou1.h"			/* 竜巻 陰陽玉 */

//	#define speed256				user_data04 	/* 速度 */
	#define vx_r					user_data05 	/* 回転方向(回転量) */
	#define px256					user_data06 	/* 仮想x位置(反転用) */
#include "zako_003_tatsumaki1.h"		/* 竜巻 陰陽玉 */

//	#define target_x256 			user_data00
//	#define target_y256 			user_data01
//	#define radius256				user_data02
	/* 敵の向き tmp_angleCCW1024 */
#include "zako_004_kakomi1.h"			/* 妖怪 */
#include "zako_005_aka_kedama.h"		/* 妖怪 */


//	#define speed256				user_data04
	#define max_y256				user_data05
#include "zako_007_yukari2.h"			/* 毛玉 */
	#define target_x256 			user_data00
	#define target_y256 			user_data01
#include "zako_008_midori_kedama1.h"	/* 毛玉 */
	#define wait1					user_data04 	/* 上下に動いたカウンタ */
//	#define kaisu_nnn				user_data05 	/* 繰り返した回数のカウンタ */
#include "zako_009_midori_kedama2.h"	/* 毛玉 */

	#define target_x256 			user_data00
	#define target_y256 			user_data01
//	#define speed256				user_data04
#include "zako_010_kedama1.h"			/* 毛玉 */
	#define kougeki_type			user_data05
	#define USE_X_HOUKOU (1)
#include "zako_011_kedama2.h"			/* 毛玉 */


	/* 敵の向き tmp_angleCCW1024 */
	#define gra_anime_type			user_data04 	/* グラタイプ */
#include "zako_012_obake1.h"			/* [C妖精]その他ザコ */
//	#define target_x256 			user_data00
//	#define target_y256 			user_data01
//	#define radius256				user_data02
	#define flag1					user_data05
#include "zako_013_obake2.h"			/* [C妖精]その他ザコ */
	#define USE_X_HOUKOU_karasu (0)
	#define KARASU3_houkou			user_data05
#include "zako_014_karasu1.h"			/* [C妖精]その他ザコ */


//	#define speed256				user_data04
#include "zako_015_meido1.h"			/* [B妖精]中妖精 */
//	#define speed256				user_data04
#include "zako_016_meido2.h"			/* [B妖精]中妖精 */
	#define clip_left256			user_data00
	#define clip_right256			user_data01
	#define turnspeed1024			user_data03
//	#define speed256				user_data04
//	#define kaisu_nnn				user_data05 	/* ターンする回数 */
#include "zako_017_meido3.h"			/* [B妖精]中妖精 */
//	#define speed256				user_data04
#include "zako_018_meido4.h"			/* [B妖精]中妖精 */

	/* 敵の向き tmp_angleCCW1024 */
//	#define speed256				user_data04 	/* 速度 */
#include "zako_019_ao_yousei1.h"		/* [A妖精]小妖精 */
//	#define kaisu_nnn				user_data05 	/* 弾を撃つ回数 */
#include "zako_020_ao_yousei2.h"		/* [A妖精]小妖精 */
	#define USE_X_HOUKOU_ao3 (0)
	#define KARASU3_houkou			user_data05
#include "zako_021_ao_yousei3.h"		/* [A妖精]小妖精 */
	/* 敵の向き tmp_angleCCW1024 */
//	#define speed256				user_data04 	/* 速度 */
#include "zako_022_ao_yousei4.h"		/* [A妖精]小妖精 */


/*---------------------------------------------------------
	倒した場合のアイテム指定
	中ボスと共用する必要がある。
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
	src->callback_loser 	= aaa[(((l->user_item8))&0x0f)];			/* 「ザコがやられた場合の処理」に、「ランダムテーブルからアイテム生成」を設定 */
}
/*
src->jyumyou		 ---a bc--
src->yx_an im_frame  yyyy xxxx
src->yx_an im_frame  --1a --bc
*/

/*---------------------------------------------------------
	倒した場合のアイテム指定
	中ボスと共用する必要がある。
---------------------------------------------------------*/

/*global*/static void regist_settei_common(GAME_COMMAND *l, SPRITE *src)
{
	regist_items_common(l, src);/* 中ボスと共用する必要がある。 */
	src->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);	/* デフォルトはあたり判定あり */
	src->m_Hit256R			= ZAKO_ATARI16_PNG; 	/* グラからあたり判定を決める。 */
	//
	src->cx256				= ((l->user_x)<<8); 	/* 設定ファイルから出現x座標を決める。 */
	src->cy256				= ((l->user_y)<<8); 	/* 設定ファイルから出現y座標を決める。 */
	src->base_hp			= ((l->user_hp));		/* 設定ファイルから体力を決める。 */
	src->base_score 		= ((l->user_score));	/* 設定ファイルから獲得スコアを決める。 */
//
	src->type				= (SP_GROUP_TEKI)|((l->user_select_gazou)&0x3f); 	/* 設定ファイルから画像を決める。 */
	/*
		DATは人間が書くので、間違ってたりする。
		パターンアニメーション専用のザコキャラに、
		回転系グラを指定した場合、
		パターンアニメーション出来ないので、
		バグったキャラになってしまう。
		そこでそういう場合は、強制的に回転系に補正する。
	*/
	if (11<((l->user_select_gazou)&0x0f))/*(12, 13, 14, 15) */
	{
		src->zako_anime_rotate_angle1024 = (10);	/* 回転する速さ */
	}
}

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ザコ登録の共通ルーチン
	-------------------------------------------------------
---------------------------------------------------------*/

static void regist_zako_common(GAME_COMMAND *l, SPRITE *src)
{
	regist_settei_common(l, src);/* 中ボスと共用する必要がある。 */
	src->callback_hit_teki	= callback_hit_zako;		/* 「ザコに自弾があたった場合の処理」に、標準の処理を設定 */
}


typedef struct
{
	void (*aaa_init_callback)(GAME_COMMAND *l, SPRITE *src);	/* 初期化 処理 */
	void (*aaa_move_callback)(SPRITE *sss); 					/* 移動 処理 */
} SPELL_aaa_RESOURCE;

global void game_command_05_regist_zako(GAME_COMMAND *l)
{
	if (0 != cg.chuu_boss_mode)
	{
		/* 中-ボスがいる場合には雑魚追加しない。 */
		return;/* error */
	}
	if (23 < l->user_255_code)
	{
		return;/* error */
	}
	/* 中間コード形式のコマンドから各関数に分岐する */
	/* tekiの生成を番号で管理(load_stage.c の ctype_name[]に対応している) */
	SPELL_aaa_RESOURCE aaa_resource[(23)] =
	{
	/* 魔方陣 */
		{regist_zako_000_mahoujin1, 		move_kougeki_mahoujin}, 	/* "魔方陣1"	TEKI_RED_BOOK	 MAHOUJIN A */	/* ばらまき攻撃魔方陣 */
		{regist_zako_001_mahoujin2, 		move_kougeki_mahoujin}, 	/* "魔方陣2"	TEKI_MAHOJIN 	 MAHOUJIN B */	/* 消える魔方陣 */
	/* 竜巻 陰陽玉 */
		{regist_zako_002_inyou1,			move_inyou1},				/* "陰陽玉1"	TEKI_INYOU_DAI */
		{regist_zako_003_tatsumaki1,		move_tatsumaki1},			/* "竜巻1"		TEKI_TATSUMAKI */
	/* 妖怪 */
		{regist_zako_004_kakomi1,			move_kakomi1},				/* "囲妖怪1"	TEKI_CHOU1 */
		{regist_zako_005_aka_kedama1,		move_aka_kedama1_1st},		/* "赤毛玉1"	TEKI_CHEN */ 	/* 橙 */
	/* 毛玉 */
		{regist_zako_006_inseki1,			move_obake1},				/* "隕石1"		TEKI_NIJI_HOSI */	/* その他ザコ */
		{regist_zako_007_yukari2,			move_yukari2},				/* "紫編隊2"	TEKI_NIJI_HOSI */	/* その他ザコ */
		{regist_zako_008_midori_kedama1,	move_midori_kedama1},		/* "緑毛玉1"	TEKI_AKA_KEDAMA */
		{regist_zako_009_midori_kedama2,	move_midori_kedama2},		/* "緑毛玉2"	TEKI_AKA_KEDAMA */
		{regist_zako_010_kedama1,			move_kedama1},				/* "毛玉1"		TEKI_HAI_KEDAMA */
		{regist_zako_011_kedama2,			move_kedama2},				/* "毛玉2"		TEKI_HAI_KEDAMA */
	/* [C妖精]その他ザコ */
		{regist_zako_012_obake1,			move_obake1},				/* "おばけ1"	TEKI_YOUSEI2_5 */
		{regist_zako_013_obake2,			move_obake2},				/* "おばけ2"	TEKI_YOUSEI2_5	"虹毛玉1"	*/		//;
		{regist_zako_014_karasu1,			move_karasu1},				/* "烏1"		TEKI_YOUSEI2_1 */
	/* [B妖精]中妖精 */
		{regist_zako_015_meido1,			move_meido1},				/* "メイド1"	TEKI_YOUSEI3_1 */
		{regist_zako_016_meido2,			move_meido2},				/* "メイド2"	TEKI_YOUSEI3_1 */
		{regist_zako_017_meido3,			move_meido3},				/* "メイド3"	TEKI_YOUSEI3_1 */
		{regist_zako_018_meido4,			move_meido4},				/* "メイド4"	TEKI_YOUSEI3_1 */
	/* [A妖精]小妖精 */
		{regist_zako_019_ao_yousei1,		move_ao_yousei1},			/* "青妖精1"	TEKI_YOUSEI1_1 */
		{regist_zako_020_ao_yousei2,		move_ao_yousei2},			/* "青妖精2"	TEKI_YOUSEI1_1 */
		{regist_zako_021_ao_yousei3,		move_ao_yousei3},			/* "青妖精3"	TEKI_YOUSEI1_1 */
		{regist_zako_022_ao_yousei4,		move_ao_yousei4},			/* "青妖精4"	TEKI_YOUSEI1_1 */ //(r33では未使用)
	};
	SPRITE *h;
	h							= obj_add_01_teki_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		regist_zako_common(l, h);
	//	(*aaa[ (int)(l->user_i_code) ])(l, h);	/* 中間コード形式のコマンドから各関数に分岐する */
	//	(*aaa[ (int)(l->user_255_code) ])(l, h);	/* 中間コード形式のコマンドから各関数に分岐する */
		h->callback_mover = aaa_resource[ (int)(l->user_255_code) ].aaa_move_callback; /* 中間コード形式のコマンドから各関数に分岐する */
		(aaa_resource[ (int)(l->user_255_code) ].aaa_init_callback)(l, h);				/* 中間コード形式のコマンドから各関数に分岐する */
	}
}
#include "chuu_boss.h"
