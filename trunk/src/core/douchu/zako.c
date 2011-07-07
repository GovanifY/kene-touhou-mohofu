
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ザコは纏める予定。
---------------------------------------------------------*/
//extern void zako_anime_type01(SPRITE *src, int type_base);
//extern void zako_anime_type02(SPRITE *src, int type_base);

#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define boss_time_out		user_data03 	/* 制限時間 */
#endif


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
static void zako_anime_type01(SPRITE *src, int type_base)
{
	src->type				= (type_base)+((((src->tmp_angleCCW1024)&(1024-1)))>>7);
}
static void zako_anime_type02(SPRITE *src, int type_base)
{
	src->type				= (type_base)+((src->jyumyou>>2)&(4-1));
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


/*
src->jyumyou		 ---a bc--
src->yx_an im_frame  yyyy xxxx
src->yx_an im_frame  --1a --bc
*/
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ザコ登録の共通ルーチン
	-------------------------------------------------------
---------------------------------------------------------*/


static void regist_zako_common(GAME_COMMAND *l, SPRITE *src)
{
	src->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);	/* デフォルトはあたり判定あり */
	src->callback_loser 	= lose_random_item; 		/* 「ザコがやられた場合の処理」に、「ランダムテーブルからアイテム生成」を設定 */
	src->callback_hit_teki	= callback_hit_zako;		/* 「ザコに自弾があたった場合の処理」に、標準の処理を設定 */
//
	src->cx256				= ((l->user_x)<<8); 	/* 設定ファイルから出現x座標を決める。 */
	src->cy256				= ((l->user_y)<<8); 	/* 設定ファイルから出現y座標を決める。 */
	src->base_hp			= ((l->user_hp));		/* 設定ファイルから体力を決める。 */
	src->base_score 		= ((l->user_score));	/* 設定ファイルから獲得スコアを決める。 */
//
	src->type				= (SP_GROUP_TEKI)|((l->user_select_gazou)); 	/* 設定ファイルから画像を決める。 */
//	src->type				= TEKI_58_RED_BOOK; 	/* 設定ファイルからグラを決める。 */
	src->m_Hit256R			= ZAKO_ATARI16_PNG; 	/* グラからあたり判定を決める。 */
}


typedef struct
{
	void (*aaa_init_callback)(GAME_COMMAND *l, SPRITE *src);	/* 初期化移動処理 */
	void (*aaa_move_callback)(SPRITE *sss); /* スペルカード移動処理 */
} SPELL_aaa_RESOURCE;

global void game_command_05_regist_zako(GAME_COMMAND *l)
{
	if (0!=chu_boss_mode)
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
//	void (*aaa[ETYPE_MAX])(GAME_COMMAND *l, SPRITE *src) =
//	void (*aaa[23/*ETYPE_MAX*/])(GAME_COMMAND *l, SPRITE *src) =
	SPELL_aaa_RESOURCE aaa_resource[23/*ETYPE_MAX*/] =
	{
//		regist_zako_dummy,/* [番兵区切り] */	/*NULL*/	/* 内部で見つからない場合は、teki_error(); */
//	/* その他 */
//		regist_zako_dummy,		/* ゲーム 全ステージ クリアー */
//		regist_zako_dummy,/*CTYPE_02_BG_CONTROL*/
//	/* ボス */
//		regist_zako_dummy,				/* 共通 */
//	/* 特殊敵[中型敵] */
//		regist_zako_dummy,				/* 中-ボスの予定 */
	/* 魔方陣 */
		{regist_zako_000_mahoujin1, 		move_kougeki_mahoujin}, 	/* "魔方陣1"	TEKI_58_RED_BOOK	CTYPE_19_MAHOUJIN_A */
		{regist_zako_001_mahoujin2, 		move_kougeki_mahoujin}, 	/* "魔方陣2"	TEKI_52_MAHOJIN 	CTYPE_20_MAHOUJIN_B */
	/* 竜巻 陰陽玉 */
		{regist_zako_002_inyou1,			move_inyou1},				/* "陰陽玉1"	TEKI_53_INYOU_DAI */
		{regist_zako_003_tatsumaki1,		move_tatsumaki1},			/* "竜巻1"		TEKI_62_TATSUMAKI */
	/* 妖怪 */
		{regist_zako_004_kakomi1,			move_kakomi1},				/* "囲妖怪1"	TEKI_54_CHOU1 */
		{regist_zako_005_aka_kedama1,		move_aka_kedama1_1st},		/* "赤毛玉1"	TEKI_56_CHEN */ 	/* 橙 */
	/* 毛玉 */
		{regist_zako_006_inseki1,			move_obake1},				/* "隕石1"		TEKI_61_NIJI_HOSI */	/* その他ザコ */
		{regist_zako_007_yukari2,			move_yukari2},				/* "紫編隊2"	TEKI_61_NIJI_HOSI */	/* その他ザコ */
		{regist_zako_008_midori_kedama1,	move_midori_kedama1},		/* "緑毛玉1"	TEKI_60_AKA_KEDAMA */
		{regist_zako_009_midori_kedama2,	move_midori_kedama2},		/* "緑毛玉2"	TEKI_60_AKA_KEDAMA */
		{regist_zako_010_kedama1,			move_kedama1},				/* "毛玉1"		TEKI_59_HAI_KEDAMA */
		{regist_zako_011_kedama2,			move_kedama2},				/* "毛玉2"		TEKI_59_HAI_KEDAMA */
	/* [C妖精]その他ザコ */
		{regist_zako_012_obake1,			move_obake1},				/* "おばけ1"	TEKI_28_YOUSEI2_5 */
		{regist_zako_013_obake2,			move_obake2},				/* "おばけ2"	TEKI_28_YOUSEI2_5	"虹毛玉1"	*/		//;
		{regist_zako_014_karasu1,			move_karasu1},				/* "烏1"		TEKI_24_YOUSEI2_1 */
	/* [B妖精]中妖精 */
		{regist_zako_015_meido1,			move_meido1},				/* "メイド1"	TEKI_36_YOUSEI3_1 */
		{regist_zako_016_meido2,			move_meido2},				/* "メイド2"	TEKI_36_YOUSEI3_1 */
		{regist_zako_017_meido3,			move_meido3},				/* "メイド3"	TEKI_36_YOUSEI3_1 */
		{regist_zako_018_meido4,			move_meido4},				/* "メイド4"	TEKI_36_YOUSEI3_1 */
	/* [A妖精]小妖精 */
		{regist_zako_019_ao_yousei1,		move_ao_yousei1},			/* "青妖精1"	TEKI_12_YOUSEI1_1 */
		{regist_zako_020_ao_yousei2,		move_ao_yousei2},			/* "青妖精2"	TEKI_12_YOUSEI1_1 */
		{regist_zako_021_ao_yousei3,		move_ao_yousei3},			/* "青妖精3"	TEKI_12_YOUSEI1_1 */
		{regist_zako_022_ao_yousei4,		move_ao_yousei4},			/* "青妖精4"	TEKI_12_YOUSEI1_1 */ //(r33では未使用)
//
//		regist_zako_dummy,				/* ETYPE_01_SJIS_TEXT */
//		regist_zako_dummy,				/* ETYPE_02_LOAD_BG */
//	//	regist_zako_dummy,				/* ETYPE_03_PICTURE */		/* この方式は処理落ち解消しにくい(Gu化も難しい)ので都合により廃止 */
	};
	SPRITE *h;
	h							= obj_add_01_teki_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		regist_zako_common(l, h);
	//	(*aaa[ (int)(l->user_i_code) ])(l, h);	/* 中間コード形式のコマンドから各関数に分岐する */
	//	(*aaa[ (int)(l->user_255_code) ])(l, h);	/* 中間コード形式のコマンドから各関数に分岐する */
		h->callback_mover = aaa_resource[ (int)(l->user_255_code) ].aaa_move_callback;	/* 中間コード形式のコマンドから各関数に分岐する */
		(aaa_resource[ (int)(l->user_255_code) ].aaa_init_callback)(l, h);				/* 中間コード形式のコマンドから各関数に分岐する */
	}
}
