
#include "game_main.h"

/*---------------------------------------------------------
 東方模倣風  ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	弾幕生成
	-------------------------------------------------------
	★弾幕はシステムでサポートする構想があります。(構想中)
	システムでサポートされると、どのボスが、どの弾幕でも撃てるようになります。
---------------------------------------------------------*/


/*---------------------------------------------------------
	自機(src)狙い弾(dest)の角度(中心→中心)を(destのtmp_angleCCW65536に)計算。
	-------------------------------------------------------
	元(src)の中心座標から、
	先(dest)の中心座標へ狙う角度を計算し、
	先(dest)の一時角度変数(tmp_angleCCW65536)に計算する。
	-------------------------------------------------------
	例えば、元(src)==自機、先(dest)==敵弾なら、自機狙い弾。
---------------------------------------------------------*/

global /*static*/ void tmp_angleCCW65536_src_nerai(SPRITE *src, SPRITE *dest)
{
	dest->tmp_angleCCW65536 	= (atan_65536(src->cy256-dest->cy256, src->cx256-dest->cx256));
	#if (0)
	dest->tmp_angleCCW65536 	-= (ra_nd()) & ((65536/32)-1);/* [謎の修正] 現状狙わないのでゲームにならないから仕方なく。 */
	#endif
	#if (1)
	dest->tmp_angleCCW65536 	-= ((65536/64)-1);/* [謎の修正] 現状狙わないのでゲームにならないから仕方なく。 */
	#endif
}


/*---------------------------------------------------------
	自機(src)狙い弾(dest)の角度(中心→中心)を(destのtmp_angleCCW1024に)計算。
---------------------------------------------------------*/

global /*static*/ void tmp_angleCCW1024_src_nerai(SPRITE *src, SPRITE *dest)
{
//	dest->tmp_angleCCW1024		= (at an_1024(src->cy256-dest->cy256, src->cx256-dest->cx256));
//	#if (0)
//	dest->tmp_angleCCW1024  	-= (ra_nd()) & ((1024/32)-1);/* [謎の修正] 現状狙わないのでゲームにならないから仕方なく。 */
//	#endif
	tmp_angleCCW65536_src_nerai(src, dest);
	dest->tmp_angleCCW1024  >>= (6);
}
global /*static*/ void tmp_angleCCW1024_jiki_nerai(SPRITE *dest)
{
	SPRITE *zzz_player;
	zzz_player = &obj00[FIX_OBJ_00_PLAYER];
	tmp_angleCCW1024_src_nerai(zzz_player, dest);
}


/*---------------------------------------------------------
	弾幕生成時間
---------------------------------------------------------*/

#define DANMAKU_0032_TIME	(32)
#define DANMAKU_0048_TIME	(48)
#define DANMAKU_0064_TIME	(64)
#define DANMAKU_0070_TIME	(70)
#define DANMAKU_0096_TIME	(96)
#define DANMAKU_0128_TIME	(128)
#define DANMAKU_0192_TIME	(192)
#define DANMAKU_0256_TIME	(256)
#define DANMAKU_0640_TIME	(640)
#define DANMAKU_1024_TIME	(1024)
#define DANMAKU_9999_TIME	(16384)

/*---------------------------------------------------------
	角度弾を直接扱う
---------------------------------------------------------*/
#if 1/* 角度弾を直接扱うので、本来 bullet_angle.c に入るべきもの。 */

/*
	角度弾では、ベクトル移動をしない。
	代わりに基点座標として使う。
*/
#if 1/* 角度弾規格(策定案) */
	#define tx256				vx256/* 基点座標x */
	#define ty256				vy256/* 基点座標y */
	//
	#define radius256			user_data01 	/* 半径 */
//	#define speed256			user_data02 	/* 加減速 */
	#define speed65536			user_data02 	/* 加減速 */
	#define tra65536			user_data03 	/* 加減速調整 */
//	#define rotate1024			user_data04 	/* 回転角度調整 */
//	#define bullet_status_mask	user_data05 	/* 画面外消去判定や反射機能 */
#endif

#endif


/*---------------------------------------------------------
	弾幕を生成。
	-------------------------------------------------------
---------------------------------------------------------*/

#include "danmaku_00_zako.h"
#include "danmaku_00_sonota.h"
//
#include "danmaku_01_mima.h"
#include "danmaku_01_alice.h"
#include "danmaku_01_rumia.h"
#include "danmaku_01_meirin.h"
#include "danmaku_01_chrno.h"
#include "danmaku_01_kaguya.h"
#include "danmaku_01_daiyousei.h"
#include "danmaku_01_aya.h"

//
#include "danmaku_01_pache.h"
#include "danmaku_01_sakuya.h"
#include "danmaku_01_remilia.h"


/*---------------------------------------------------------
	共通弾幕生成部
	-------------------------------------------------------
	弾幕ごとに予め生成時間が登録してある。
---------------------------------------------------------*/

typedef struct
{
	void (*danmaku_callback)(SPRITE *sss);		/* 弾幕生成処理 */
	int danmaku_time;							/* 弾幕寿命時間 */
} DANMAKU_RESOURCE;
//	/*const*/ static void (*danmaku_create_bbb[(DANMAKU_MAX/*16+1*/)])(SPRITE *sss) =
	/*const*/ static DANMAKU_RESOURCE my_danmaku_resource[(DANMAKU_MAX/*16+1*/)] =
{
	#define danmaku_create_99_mitei danmaku_create_10_cirno_misogi
	{	NULL,										DANMAKU_0128_TIME}, 	/* 00 */	/* 弾幕生成しない */
	// 0x00- 中ザコ / ボス共用弾幕
	{	danmaku_create_01_sakuya_misogi,			DANMAKU_0128_TIME}, 	/* 01 */	/* [咲夜] 紅5面中ボス 「通常攻撃」咲夜禊弾幕 */
	{	danmaku_create_02_24nerai,					DANMAKU_0048_TIME}, 	/* 02 */	/* [咲夜] 紅5面中ボス 24弾幕 (奇術「ミスディレクションもどき(1/2)」) */
	{	danmaku_create_03_11nife,					DANMAKU_0064_TIME}, 	/* 03 */	/* [咲夜] 紅5面中ボス 11弾幕 (奇術「ミスディレクションもどき(2/2)」) */
	{	danmaku_create_04_pink_hearts,				DANMAKU_0128_TIME}, 	/* 04 */	/* [咲夜] 紅5面ボス 「通常攻撃1(1/2)」にちょっとだけ似た弾幕(予定) */
	{	danmaku_create_05_32way_dual,				DANMAKU_0128_TIME}, 	/* 05 */	/* [咲夜] 紅5面ボス 「通常攻撃1/2(2/2)」にちょっとだけ似た弾幕(予定) */
	{	danmaku_create_06_luna_clock_32way, 		DANMAKU_0128_TIME}, 	/* 06 */	/* [咲夜] 紅5面ボス 幻象「ルナクロック(1/2)」にちょっとだけ似た弾幕(予定) */
	{	danmaku_create_07_80way_dual_five,			DANMAKU_0128_TIME}, 	/* 07 */	/* [咲夜] 紅5面ボス 「通常攻撃3(1/2)」にちょっとだけ似た弾幕(予定) */
	// 0x08- 中ザコ / ボス共用弾幕
	{	danmaku_create_08_rumia_night_bird_zako,	DANMAKU_0128_TIME}, 	/* 08 */	/* [ルーミア] 紅1面ボス 夜符「ナイトバード」にちょっとだけ似た弾幕(予定) */
	{	danmaku_create_09_zako_sentakki,			DANMAKU_0128_TIME}, 	/* 09 */	/* [雑魚] 洗濯機弾幕(とりあえず弾幕システム作成の実験で作ってみた奴) */
	{	danmaku_create_1a_dai_yousei_midori,		DANMAKU_0064_TIME}, 	/* 1a */	/* [大妖精] 緑巻き */
	{	danmaku_create_1b_dai_yousei_aka,			DANMAKU_0064_TIME}, 	/* 1b */	/* [大妖精] 赤巻き */
	{	danmaku_create_14_sakuya_jack_oodama32, 	DANMAKU_0256_TIME}, 	/* 14 */	/* [咲夜] 幻幽「ジャック・ザ・ルドビレ」になるかもしれないテスト */
	{	danmaku_create_22_sakuya_miss_direction,	DANMAKU_0256_TIME}, 	/* 1c */	/* [咲夜] 奇術「ミスディレクション」 */
	{	danmaku_create_1f_remilia_kougeki2, 		DANMAKU_0256_TIME}, 	/* 1f */	/* [レミリア] 通常攻撃2 っぽいもの */
	{	danmaku_create_11_tengu_shot,				DANMAKU_0256_TIME}, 	/* 11 */	/* [天狗様] 差分氏の妖怪1(天狗様?)が撃つ弾幕 */
	// 0x10- 中ザコ / ボス共用弾幕
	{	danmaku_create_10_cirno_misogi, 			DANMAKU_0070_TIME}, 	/* 10 */	/* [チルノ] 紅2面ボス チルノ禊弾幕 */
	{	danmaku_create_13_perfect_freeze,			DANMAKU_0640_TIME}, 	/* 13 */	/* [チルノ] 紅2面ボス パーフェクトフリーズ */
	{	danmaku_create_16_diamond_blizzard, 		DANMAKU_0128_TIME}, 	/* 16 */	/* [チルノ] 紅2面ボス 雪符「ダイアモンドブリザード」 */
	{	danmaku_create_25_alice_suwako, 			DANMAKU_0256_TIME}, 	/* 25 */	/* [アリス] アリス	諏訪子っぽい(?)弾幕 */
	{	danmaku_create_17_alice_nejiri10sec,		DANMAKU_0192_TIME}, 	/* 17 */	/* [アリス] 通常攻撃(もどき) ねじり弾10秒 DANMAKU_0640_TIME  */
	{	danmaku_create_0e_aka_2nd,					DANMAKU_1024_TIME}, 	/* 0e */	/* [アリス] 怪EX面ボス 赤の魔法2段階目もどき */
	{	danmaku_create_0b_alice_doll,				DANMAKU_0256_TIME}, 	/* 0b */	/* [アリス] DANMAKU_0128_TIME 妖3面ボス 人形弾幕(もどき)(256>240=4x60) */
	{	danmaku_create_1e_alice_doll,				DANMAKU_0256_TIME}, 	/* 1e */	/* [アリス] 妖3面ボス 人形弾幕(もどき)(256>240=4x60) 蒼符「博愛の仏蘭西人形」 */
	// 0x18- 中ザコ / ボス共用弾幕
	{	danmaku_create_0c_hana_test,				DANMAKU_0256_TIME}, 	/* 0c */	/* [魅魔] 花てすと弾幕 */
	{	danmaku_create_0d_mima_sekkin,				DANMAKU_0256_TIME}, 	/* 0d */	/* [魅魔] 接近弾幕 */
	{	danmaku_create_24_mima_toge,				DANMAKU_0256_TIME}, 	/* 24 */	/* [魅魔] トゲ弾幕 */
	{	danmaku_create_99_mitei,					DANMAKU_0128_TIME}, 	/* 26 */	/* 未定弾幕 */
	{	danmaku_create_23_kaguya_tamanoe,			DANMAKU_0640_TIME}, 	/* 1d */	/* [輝夜] 蓬莱の玉の枝もどき */
	{	danmaku_create_99_mitei,					DANMAKU_0128_TIME}, 	/* 26 */	/* 未定弾幕 */
	{	danmaku_create_18_kaguya01, 				DANMAKU_0064_TIME}, 	/* 18 */	/* [輝夜] とりあえず強引に変換 */
	{	danmaku_create_19_kaguya04, 				DANMAKU_0064_TIME}, 	/* 19 */	/* [輝夜] とりあえず強引に変換 */
	// 0x20- ボス専用弾幕
	{	danmaku_create_15_aya_misogi1,				DANMAKU_0256_TIME}, 	/* 15 */	/* [文] の通常弾っぽい弾幕撃たせてみるテスト1 */
	{	danmaku_create_23_aya_misogi2,				DANMAKU_0128_TIME}, 	/* 23 */	/* [文]の通常弾っぽい弾幕撃たせてみるテスト2 */
	{	danmaku_create_22_aya_yatimata, 			DANMAKU_0640_TIME}, 	/* 22 */	/* [文] 岐符「天の八衢」 */
	{	danmaku_create_12_aya_merin_test,			DANMAKU_0640_TIME}, 	/* 12 */	/* [美鈴] 紅3面中ボス 「通常攻撃」/ 文に美鈴っぽい弾幕撃たせてみるテスト */
	{	danmaku_create_0f_aya_doll, 				DANMAKU_0128_TIME}, 	/* 0f */	/* [文] 人形弾幕(てすと弾幕「危険な種」) */
	{	danmaku_create_0a_houka_kenran, 			DANMAKU_0128_TIME}, 	/* 0a */	/* [紅美鈴] 紅3面ボス 華符「芳華絢爛」にちょっとだけ似た弾幕(予定) */
	{	danmaku_create_26_aya_saifu,				DANMAKU_9999_TIME}, 	/* 26 */	/* [文] 塞符 */
	{	danmaku_create_27_pache_laser2, 			DANMAKU_0640_TIME}, 	/* 27 */	/* [パチェ] のレーザーもどき */
	// 0x28- ボス専用弾幕
	{	danmaku_create_28_rumia_night_bird, 		DANMAKU_0128_TIME}, 	/* 30 */	/* [ルーミア] 紅1面ボス 夜符「ナイトバード」にちょっとだけ似た弾幕(予定) */
	{	danmaku_create_29_rumia_demarcation,		DANMAKU_0640_TIME}, 	/* 31 */	/* [ルーミア] 紅1面ボス 闇符「ディマーケイション」にちょっとだけ似た弾幕(予定) */
	{	danmaku_create_2a_amefuri_test,				DANMAKU_0640_TIME}, 	/* 26 */	/* 雨てすと */
	{	danmaku_create_99_mitei,					DANMAKU_0128_TIME}, 	/* 26 */	/* 未定弾幕 */
	{	danmaku_create_99_mitei,					DANMAKU_0128_TIME}, 	/* 26 */	/* 未定弾幕 */
	{	danmaku_create_99_mitei,					DANMAKU_0128_TIME}, 	/* 26 */	/* 未定弾幕 */
	{	danmaku_create_20_sonota_debug_cw_ao,		DANMAKU_0128_TIME}, 	/* 20 */	/* [その他] デバッグ弾幕CW青 */
	{	danmaku_create_21_sonota_debug_ccw_aka, 	DANMAKU_0128_TIME}, 	/* 21 */	/* [その他] デバッグ弾幕CCW赤 */
};

/*
dimmercation
Demarcation.
demarcation   境界

*/


/*---------------------------------------------------------
	弾幕時間に予め登録された寿命時間をセットする。
---------------------------------------------------------*/

global void danmaku_set_time_out(SPRITE *src)
{
	src->boss_base_danmaku_time_out = my_danmaku_resource[(src->boss_base_danmaku_type/*-1*/)].danmaku_time;
}


/*---------------------------------------------------------
	弾幕を１フレーム生成する。
	弾幕生成すると時間経過し、弾幕寿命時間が自動で減る。
	弾幕寿命時間が無くなった場合は、弾幕タイプが無し(DANMAKU_00)になる。
---------------------------------------------------------*/

global void danmaku_generator(SPRITE *src)
{
	if (DANMAKU_00 != src->boss_base_danmaku_type)	/* 弾幕生成は必要？ */
	{
		src->boss_base_danmaku_time_out--;			/* 時間経過する  */ 	/*fps_factor*/
		if (0 < src->boss_base_danmaku_time_out)	/* 時間切れ？ */
		{
			/* 弾幕生成中 */
			(my_danmaku_resource[(src->boss_base_danmaku_type/*-1*/)].danmaku_callback)(src);
		}
		else
		{
			src->boss_base_danmaku_type = DANMAKU_00;			/* 弾幕生成終了 */
		}
	}
}
