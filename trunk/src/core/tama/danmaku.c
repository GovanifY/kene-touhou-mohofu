
#include "game_main.h"

/*---------------------------------------------------------
 東方模倣風  〜 Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	弾幕生成
	-------------------------------------------------------
	★弾幕はシステムでサポートする構想があります。(構想中)
	システムでサポートされると、どのボスが、どの弾幕でも撃てるようになります。
---------------------------------------------------------*/


/*---------------------------------------------------------
	自機狙い弾の角度を計算
---------------------------------------------------------*/

global /*static*/ void tmp_angleCCW65536_jikinerai(SPRITE *p, SPRITE *t)
{
	/* 簡略版(弾と自分が大きさが同じならずれない、違うとその分誤差になる) */
	t->tmp_angleCCW65536		= (atan_65536(p->y256-t->y256, p->x256-t->x256));
}


/*---------------------------------------------------------
	自機狙い弾の角度を計算
---------------------------------------------------------*/

global /*static*/ void tmp_angleCCW1024_jikinerai(SPRITE *p, SPRITE *t)
{
	/* 簡略版(弾と自分が大きさが同じならずれない、違うとその分誤差になる) */
	t->tmp_angleCCW1024 		= (atan_1024(p->y256-t->y256, p->x256-t->x256));
}

#include "danmaku_00_sonota.h"
//
#include "danmaku_01_sakuya.h"
#include "danmaku_01_mima.h"
#include "danmaku_01_alice.h"
#include "danmaku_01_rumia.h"
#include "danmaku_01_meirin.h"
#include "danmaku_01_chrno.h"
#include "danmaku_01_kaguya.h"
#include "danmaku_01_daiyousei.h"

/*---------------------------------------------------------
	共通弾幕生成部
	-------------------------------------------------------

---------------------------------------------------------*/

//#define DANMAKU_064_TIME	/*(150)*/
//#define DANMAKU_064_TIME	/*(ra_nd()&0xff)*/
//	src->boss_base_danmaku_time_out = (128+1);			/* 禊弾幕の発生時間 x 2 */
	#define DANMAKU_064_TIME	(64+1)
	#define DANMAKU_048_TIME	(48+1)
	#define DANMAKU_128_TIME	(128+1)
	#define DANMAKU_256_TIME	(256+1)
	#define DANMAKU_640_TIME	(640+1)

typedef struct
{
	void (*danmaku_callback)(SPRITE *sss);		/* 初期化移動処理 */
	int danmaku_time;							/* 時間 */
} DANMAKU_RESOURCE;
//	/*const*/static void (*danmaku_create_bbb[(DANMAKU_MAX/*16+1*/)])(SPRITE *sss) =
	/*const*/static DANMAKU_RESOURCE my_danmaku_resource[(DANMAKU_MAX/*16+1*/)] =
{
	#define danmaku_create_99_mitei danmaku_create_10_cirno_misogi
	{	NULL,									DANMAKU_128_TIME},	/* 00 */	/* 弾幕生成しない */
	//													/* [咲夜] */
	{	danmaku_create_01_sakuya_misogi,		DANMAKU_064_TIME},	/* 01 */	/* 咲夜禊弾幕 / 紅5面中ボス 咲夜 「通常攻撃」 */
	{	danmaku_create_02_24nerai,				DANMAKU_048_TIME},	/* 02 */	/* 咲夜24弾幕 (奇術「ミスディレクションもどき(1/2)」) */
	{	danmaku_create_03_11nife,				DANMAKU_064_TIME},	/* 03 */	/* 咲夜11弾幕 (奇術「ミスディレクションもどき(2/2)」) */
	{	danmaku_create_04_pink_hearts,			DANMAKU_128_TIME},	/* 04 */	/* 紅5面ボス 咲夜 「通常攻撃1(1/2)」にちょっとだけ似た弾幕(予定) */
	{	danmaku_create_05_32way_dual,			DANMAKU_128_TIME},	/* 05 */	/* 紅5面ボス 咲夜 「通常攻撃1/2(2/2)」にちょっとだけ似た弾幕(予定) */
	{	danmaku_create_06_luna_clock_32way, 	DANMAKU_128_TIME},	/* 06 */	/* 紅5面ボス 咲夜 幻象「ルナクロック(1/2)」にちょっとだけ似た弾幕(予定) */
	{	danmaku_create_07_80way_dual_five,		DANMAKU_128_TIME},	/* 07 */	/* 紅5面ボス 咲夜 「通常攻撃3(1/2)」にちょっとだけ似た弾幕(予定) */
	//													/* [アリス(仮)] */
	{	danmaku_create_08_night_bird,			DANMAKU_128_TIME},	/* 08 */	/* 紅1面ボス ルーミア 夜符「ナイトバード」にちょっとだけ似た弾幕(予定) */
	{	danmaku_create_09_alice_sentakki,		DANMAKU_128_TIME},	/* 09 */	/* 洗濯機弾幕 */
	{	danmaku_create_0a_houka_kenran, 		DANMAKU_128_TIME},	/* 0a */	/* 紅美鈴 華符「芳華絢爛」にちょっとだけ似た弾幕(予定) */
	{	danmaku_create_0b_alice_doll,			DANMAKU_128_TIME},	/* 0b */	/* アリス人形弾幕 */
	{	danmaku_create_0c_hana_test,			DANMAKU_256_TIME},	/* 0c */	/* 魅魔、花てすと弾幕 */
	{	danmaku_create_0d_mima_sekkin,			DANMAKU_256_TIME},	/* 0d */	/* 魅魔、接近弾幕 */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 0e */	/* 未定弾幕 */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 0f */	/* 未定弾幕 */
	//													/* [チルノ(仮)] */
	{	danmaku_create_10_cirno_misogi, 		DANMAKU_064_TIME},	/* 10 */	/* チルノ禊弾幕 */
	{	danmaku_create_11_tengu_shot,			DANMAKU_128_TIME},	/* 11 */	/* 差分氏の妖怪1(天狗様?)が撃つ弾幕(予定) */
	{	danmaku_create_12_aya_merin_test,		DANMAKU_128_TIME},	/* 12 */	/* 文に美鈴っぽい弾幕撃たせてみるテスト */
	{	danmaku_create_13_perfect_freeze,		DANMAKU_640_TIME},	/* 13 */	/* パーフェクトフリーズ */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 14 */	/* 未定弾幕 */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 15 */	/* 未定弾幕 */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 16 */	/* 未定弾幕 */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 17 */	/* 未定弾幕 */
	//
	{	danmaku_create_18_kaguya01, 			DANMAKU_064_TIME},	/* 18 */	/* とりあえず強引に変換 */
	{	danmaku_create_19_kaguya04, 			DANMAKU_064_TIME},	/* 19 */	/* とりあえず強引に変換 */
	{	danmaku_create_20_dai_yousei_midori,	DANMAKU_064_TIME},	/* 1a */	/* 未定弾幕 */
	{	danmaku_create_21_dai_yousei_aka,		DANMAKU_064_TIME},	/* 1b */	/* 未定弾幕 */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 1c */	/* 未定弾幕 */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 1d */	/* 未定弾幕 */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 1e */	/* 未定弾幕 */
	{	danmaku_create_99_mitei,				DANMAKU_128_TIME},	/* 1f */	/* 未定弾幕 */
};
global void danmaku_set_time_out(SPRITE *src)
{
	src->boss_base_danmaku_time_out = my_danmaku_resource[(src->boss_base_danmaku_type/*-1*/)].danmaku_time;
}
global /*static*/ void danmaku_generator(SPRITE *src)
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
