
#include "game_main.h"

/*---------------------------------------------------------
 東方模倣風 ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	スペル生成システム(スペル弾幕生成)
	-------------------------------------------------------
	現状「ボス」以外にも「中ザコ」、「小ザコ」がスペル生成できるので、「スペル生成」と「スペルカード管理」は別システムです。
	(現状「中-ボス」は作る予定があるが作ってない。カード履歴システムもまだ作ってない)
	-------------------------------------------------------
	分類		カード履歴		スペルカード	スペル生成
	ボス		○				○				○	// ボス
	中-ボス		×(?)			○				○	// <予定>			(雑魚の流れを「止める」のか「止めない」のか未定)
	中ザコ		×				×				○	// るーみあ 		スペルカードを撃てないのでカード名(符名)は無い。
	小ザコ		×				×				○	// 魔道書ザコ		スペルカードを撃てないのでカード名(符名)は無い。
	ザコ		×				×				×	// その他のザコ 	スペル生成出来ない。
	-------------------------------------------------------
	分類			雑魚生成	スペルカード
	ボス専用コア	×			○					// 雑魚生成関連の処理をしないのでその分速い。
	道中専用コア	○			×					// スペルカード管理関連の処理をしないのでその分速い。
	-------------------------------------------------------
	(r33)上記の専用コア計画は廃止。
	今の方式(r33)だとむしろ専用コアにした方が速度低下する。
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
}


/*---------------------------------------------------------
	自機(src)狙い弾(dest)の角度(中心→中心)を(destのtmp_angleCCW1024に)計算。
---------------------------------------------------------*/

global /*static*/ void tmp_angleCCW1024_src_nerai(SPRITE *src, SPRITE *dest)
{
//	dest->tmp_angleCCW1024		= (at an_1024(src->cy256-dest->cy256, src->cx256-dest->cx256));
	tmp_angleCCW65536_src_nerai(src, dest);
	dest->tmp_angleCCW1024	>>= (6);
}
global /*static*/ void tmp_angleCCW1024_jiki_nerai(SPRITE *dest)
{
	SPRITE *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	tmp_angleCCW1024_src_nerai(zzz_player, dest);
}


/*---------------------------------------------------------
	いちいち作成する場合のサブ。
	ぱちぇ & チルノ & さくや 等で汎用的に使ってる。
	(新ルーミア)で使う。
---------------------------------------------------------*/

static void set_sub_jikinerai(SPRITE *src)
{
	obj_send1->cx256					= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
	obj_send1->cy256					= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
	{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		tmp_angleCCW65536_src_nerai(zzz_player, src);/* 自機狙い角作成 */
	}
	br.BULLET_REGIST_02_angle65536		= (src->tmp_angleCCW65536); 	/* 自機狙い弾 */
}
/*---------------------------------------------------------
	スペル弾幕生成時間
---------------------------------------------------------*/

#define SPELL_TIME_0032 	(32)
#define SPELL_TIME_0048 	(48)
#define SPELL_TIME_0060 	(60)
#define SPELL_TIME_0064 	(64)
#define SPELL_TIME_0070 	(70)
#define SPELL_TIME_0096 	(96)
#define SPELL_TIME_0128 	(128)
#define SPELL_TIME_0192 	(192)
#define SPELL_TIME_0256 	(256)
#define SPELL_TIME_0512 	(512)
#define SPELL_TIME_0640 	(640)
#define SPELL_TIME_1024 	(1024)
#define SPELL_TIME_9999 	(16384)

/*---------------------------------------------------------
	角度弾を直接扱う
---------------------------------------------------------*/
#if 1/* 角度弾を直接扱うので、本来 bullet_angle.c に入るべきもの。 */

/*
	角度弾では、ベクトル移動をしない。
	代わりに基点座標として使う。
*/
#if 1/* 角度弾規格(策定案tama_system) */
	#define tx256				vx256/* 基点座標x */
	#define ty256				vy256/* 基点座標y */
	//
#endif

#endif


/*---------------------------------------------------------
	スペル難易度別テーブル
---------------------------------------------------------*/

	enum
	{
		H0=0,H1,H2,H3,H4,H5,H6
	};
	#define b_HOUGA_00_YEL_NUMS 		(H0*4)
	#define b_HOUGA_08_RED_NUMS 		(H1*4)
	#define HOUGA_04_YEL_DIV_ANGLE		(H2*4)
	#define HOUGA_16_YEL_ROTATE_ANGLE	(H3*4)
	#define HOUGA_12_RED_DIV_ANGLE		(HOUGA_16_YEL_ROTATE_ANGLE)
//
	#define b_chrno_00_BLUE_NUMS		(H4*4)
	#define chrno_04_BLUE_DIV_ANGLE 	(H5*4)
	#define spell_bunkatsu_nums_table spell_nan_ido_table
	const static /*s32*/u16/*s16 でいいかも？*/ spell_nan_ido_table[(H6*4)] =
	{
		// ★ 紅 美鈴、華符「芳華絢爛」用
		// easy 	華符「芳華絢爛」と同じ。(easyは時間で簡単になるように調整する)
		// normal	華符「芳華絢爛」と同じ。
		// hard 	--。
		// lunatic	ちゃんと避けれるの確認済み。(これ以上高密度だと辛いかも)
		/* easy 				normal					hard					lunatic 			*/
		#if 1/*(r33)*/
		(6),					(6),					(10),					(17),					/* 華符「芳華絢爛」用 */
		(6*8),					(6*8),					(10*8), 				(17*8), 				/* 華符「芳華絢爛」用 */
		(int)(65536/(6)),		(int)(65536/(6)),		(int)(65536/(10)),		(int)(65536/(17)),		/* 華符「芳華絢爛」用 */
		(int)(65536/(6*8)), 	(int)(65536/(6*8)), 	(int)(65536/(10*8)),	(int)(65536/(17*8)),	/* 華符「芳華絢爛」用 */
		#endif
		// ★ チルノ「」用
		(0),					(0),					(18),					(36),					/* 註:3より4の方が簡単 */
		(0),					(0),					(int)(65536/(18)),		(int)(65536/(36)),		/* 註:3より4の方が簡単 */
	};


/*---------------------------------------------------------
	スペル弾幕を生成。
	-------------------------------------------------------
---------------------------------------------------------*/

#include "spell_00_zako.h"
#include "spell_00_sonota.h"
//
#include "spell_01_mima.h"
#include "spell_01_alice.h"
#include "spell_01_rumia.h"
#include "spell_01_meirin.h"
#include "spell_01_chrno.h"
#include "spell_01_kaguya.h"
#include "spell_01_daiyousei.h"
#include "spell_01_aya.h"

//
#include "spell_01_pache.h"
#include "spell_01_sakuya.h"
#include "spell_01_remilia.h"


/*---------------------------------------------------------
	共通スペル弾幕生成部
	-------------------------------------------------------
	スペル弾幕ごとに予め生成時間が登録してある。
---------------------------------------------------------*/

typedef struct
{
	void (*spell_generate_callback)(SPRITE *sss);		/* スペル弾幕生成処理 */
	int spell_limit_max_time;							/* スペル弾幕寿命時間 */
} SPELL_RESOURCE;
//	/*const*/ static void (*spell_create_bbb[(SPELL_MAX/*16+1*/)])(SPRITE *sss) =
	/*const*/ static SPELL_RESOURCE system_spell_resource[(SPELL_MAX/*16+1*/)] =
{
	#define spell_create_99_mitei spell_create_10_cirno_misogi
//	{	NULL,										0			   },	/* 00 */	/* スペル弾幕生成しない */
	{	NULL,										SPELL_TIME_0128},	/* 00 */	/* スペル弾幕生成しない */
	// 0x00- 中ザコ / ボス共用スペル弾幕
	{	spell_create_01_sakuya_misogi_normal,		SPELL_TIME_0128},	/* 01 */	/* [咲夜] 紅5面中-ボス 「通常攻撃1」咲夜禊弾幕 */
	{	spell_create_02_24nerai,					SPELL_TIME_0048},	/* 02 */	/* [咲夜] 紅5面中-ボス 24弾幕 (奇術「ミスディレクションもどき(1/2)」) */
	{	spell_create_03_11nife, 					SPELL_TIME_0064},	/* 03 */	/* [咲夜] 紅5面中-ボス 11弾幕 (奇術「ミスディレクションもどき(2/2)」) */
	{	spell_create_04_pink_hearts,				SPELL_TIME_0640},	/* 04 */	/* [咲夜] 紅5面ボス 「通常攻撃1(1/2)」にちょっとだけ似た弾幕(予定) SPELL_TIME_0128(r32) */
	{	spell_create_05_32way_dual, 				SPELL_TIME_0128},	/* 05 */	/* [咲夜] 紅5面ボス 「通常攻撃1/2(2/2)」にちょっとだけ似た弾幕(予定) */
	{	spell_create_06_luna_clock_32way,			SPELL_TIME_0128},	/* 06 */	/* [咲夜] 紅5面ボス 幻象「ルナクロック(1/2)」にちょっとだけ似た弾幕(予定) */
	{	spell_create_07_80way_dual_five,			SPELL_TIME_0128},	/* 07 */	/* [咲夜] 紅5面ボス 「通常攻撃3(1/2)」にちょっとだけ似た弾幕(予定) */
	// 0x08- 中ザコ / ボス共用スペル弾幕
	{	spell_create_08_rumia_night_bird/*spell_create_08_rumia_night_bird_zako*/,		SPELL_TIME_0128},	/* 08 */	/* [ルーミア] 紅1面ボス 夜符「ナイトバード」にちょっとだけ似た弾幕(予定) */
	{	spell_create_09_zako_sentakki,				SPELL_TIME_0128},	/* 09 */	/* [雑魚] 洗濯機弾幕(とりあえず弾幕システム作成の実験で作ってみた奴) */
	{	spell_create_1a_dai_yousei_midori,			SPELL_TIME_0064},	/* 1a */	/* [大妖精] 緑巻き */
	{	spell_create_1b_dai_yousei_aka, 			SPELL_TIME_0064},	/* 1b */	/* [大妖精] 赤巻き */
	{	spell_create_14_sakuya_jack_oodama32,		SPELL_TIME_0256},	/* 14 */	/* [咲夜] 幻幽「ジャック・ザ・ルドビレ」になるかもしれないテスト */
	{	spell_create_22_sakuya_miss_direction,		SPELL_TIME_0256},	/* 1c */	/* [咲夜] 奇術「ミスディレクション」 */
	{	spell_create_1f_remilia_kougeki2,			SPELL_TIME_0256},	/* 1f */	/* [レミリア] 通常攻撃2 っぽいもの */
	{	spell_create_11_tengu_shot, 				SPELL_TIME_0256},	/* 11 */	/* [天狗様] 差分氏の妖怪1(天狗様?)が撃つ弾幕 */
	// 0x10- 中ザコ / ボス共用スペル弾幕
	{	spell_create_10_cirno_misogi,				SPELL_TIME_0060},	/* 10 */	/* [チルノ] 紅2面ボス チルノ禊弾幕 */
	{	spell_create_13_perfect_freeze, 			SPELL_TIME_0640},	/* 13 */	/* [チルノ] 紅2面ボス パーフェクトフリーズ */
	{	spell_create_16_diamond_blizzard,			SPELL_TIME_0128},	/* 16 */	/* [チルノ] 紅2面ボス 雪符「ダイアモンドブリザード」 */
	{	spell_create_25_alice_suwako,				SPELL_TIME_0256},	/* 25 */	/* [アリス] アリス	諏訪子っぽい(?)弾幕 */
	{	spell_create_17_alice_nejiri10sec,			SPELL_TIME_0192},	/* 17 */	/* [アリス] 通常攻撃(もどき) ねじり弾10秒 SPELL_TIME_0640 */
	{	spell_create_0e_aka_2nd,					SPELL_TIME_1024},	/* 0e */	/* [アリス] 怪EX面ボス 赤の魔法2段階目もどき */
	{	spell_create_0b_alice_doll, 				SPELL_TIME_0256},	/* 0b */	/* [アリス] SPELL_TIME_0128 妖3面ボス 人形弾幕(もどき)(256>240=4x60) */
	{	spell_create_1e_alice_doll, 				SPELL_TIME_0256},	/* 1e */	/* [アリス] 妖3面ボス 人形弾幕(もどき)(256>240=4x60) 蒼符「博愛の仏蘭西人形」 */
	// 0x18- 中ザコ / ボス共用スペル弾幕
	{	spell_create_0c_hana_test,					SPELL_TIME_0256},	/* 0c */	/* [魅魔] 花てすと弾幕 */
	{	spell_create_0d_mima_sekkin,				SPELL_TIME_0256},	/* 0d */	/* [魅魔] 接近弾幕 */
	{	spell_create_24_mima_toge,					SPELL_TIME_0256},	/* 24 */	/* [魅魔] トゲ弾幕 */
	{	spell_create_1b_cirno_icecle_fall,			SPELL_TIME_0128},	/* 26 */	/* [チルノ] 氷符「アイシクルフォール」 */
	{	spell_create_23_kaguya_tamanoe, 			SPELL_TIME_0640},	/* 1d */	/* [輝夜] 蓬莱の玉の枝もどき */
	{	spell_create_1d_amefuri_test,				SPELL_TIME_0640},	/* 1d */	/* [(文、仮)] 雨てすと */
	{	spell_create_18_kaguya01,					SPELL_TIME_0064},	/* 18 */	/* [輝夜] とりあえず強引に変換 */
	{	spell_create_19_kaguya04,					SPELL_TIME_0064},	/* 19 */	/* [輝夜] とりあえず強引に変換 */
	// 0x20- ボス専用スペル弾幕
	{	spell_create_15_aya_misogi1,				SPELL_TIME_0256},	/* 15 */	/* [文] の通常弾っぽい弾幕撃たせてみるテスト1 */
	{	spell_create_23_aya_misogi2,				SPELL_TIME_0128},	/* 23 */	/* [文]の通常弾っぽい弾幕撃たせてみるテスト2 */
	{	spell_create_22_aya_yatimata,				SPELL_TIME_0640},	/* 22 */	/* [文] 岐符「天の八衢」 */
	{	spell_create_12_aya_merin_test, 			SPELL_TIME_0640},	/* 12 */	/* [美鈴] 紅3面中-ボス 「通常攻撃」/ 文に美鈴っぽい弾幕撃たせてみるテスト */
	{	spell_create_0f_aya_doll,					SPELL_TIME_0128},	/* 0f */	/* [文] 人形弾幕(てすと弾幕「危険な種」) */
	{	spell_create_0a_houka_kenran,				SPELL_TIME_0128},	/* 0a */	/* [紅美鈴] 紅3面ボス 華符「芳華絢爛」にちょっとだけ似た弾幕(予定) */
	{	spell_create_26_aya_saifu,					SPELL_TIME_9999},	/* 26 */	/* [文] 塞符 */
	{	spell_create_99_mitei,						SPELL_TIME_0128},	/* 27 */	/* -------- */
	// 0x28- ボス専用スペル弾幕
	{	spell_create_08_rumia_night_bird/*spell_create_08_rumia_night_bird*/,			SPELL_TIME_0128},	/* 30 */	/* [ルーミア] 紅1面ボス 夜符「ナイトバード」にちょっとだけ似た弾幕(予定) */
	{	spell_create_29_rumia_demarcation,			SPELL_TIME_0512},	/* 31 */	/* [ルーミア] 紅1面ボス 闇符「ディマーケイション」にちょっとだけ似た弾幕(予定) */
	{	spell_create_2a_sakuya_baramaki1,			SPELL_TIME_0640},	/* 26 */	/* [咲夜] 紅5面中-ボス ばら撒き1 弾幕 () */
	{	spell_create_2b_sakuya_baramaki2,			SPELL_TIME_0640},	/* 26 */	/* [咲夜] 紅5面中-ボス ばら撒き2 弾幕 () */
	{	spell_create_2c_sakuya_blue_red_knife,		SPELL_TIME_0512},	/* 26 */	/* [咲夜] 紅5面中-ボス 「通常攻撃3」青赤ナイフ */
	{	spell_create_2d_sakuya_misogi_lunatic,		SPELL_TIME_0128},	/* 01 */	/* [咲夜] 紅5面中-ボス 「通常攻撃1」咲夜禊弾幕(lunatic only) */
	{	spell_create_2e_pache_agni_shine_1, 		SPELL_TIME_0128},	/* 2e */	/* [パチェ] "　　　　火符「アグニシャイン」" No.15 */	// (火がいっぱいだから) (輝く)アグニ(インド神話の火の神)
	{	spell_create_2f_pache_princess_undine,		SPELL_TIME_0256},	/* 2f */	/* [パチェ] "　水符「プリンセスウンディネ」" No.16 */	// (水がいっぱいだから) 「妖精の書(著者:錬金術師パラケルスス)」に登場する水の精。
	// 0x30- ボス専用スペル弾幕
	{	spell_create_30_pache_sylphy_horn_1,		SPELL_TIME_0128},	/* 30 */	/* [パチェ] "　　　　木符「シルフィホルン」" No.17 */	// (木がいっぱいだから) (風を司る精霊の)角笛
	{	spell_create_31_pache_rage_tririton_1,		SPELL_TIME_0128},	/* 31 */	/* [パチェ] "　　土符「レイジィトリリトン」" No.18 */	// (土がいっぱいだから) (怒る)トリリトン(ストーンヘンジ)
	{	spell_create_32_pache_metal_fatigue,		SPELL_TIME_0128},	/* 32 */	/* [パチェ] "　　金符「メタルファティーグ」" No.19 */	// (金(金属)がいっぱいだから) 金属疲労
	{	spell_create_33_pache_agni_shine_2, 		SPELL_TIME_0128},	/* 33 */	/* [パチェ] "　　火符「アグニシャイン上級」" No.20 */	// (火がいっぱいだから) (輝く)アグニ(インド神話の火の神)
	{	spell_create_34_pache_sylphy_horn_2,		SPELL_TIME_0128},	/* 34 */	/* [パチェ] "　　木符「シルフィホルン上級」" No.21 */	// (木がいっぱいだから) (風を司る精霊の)角笛
	{	spell_create_35_pache_rage_tririton_2,		SPELL_TIME_0128},	/* 35 */	/* [パチェ] "土符「レイジィトリリトン上級」" No.22 */	// (土がいっぱいだから) (怒る)トリリトン(ストーンヘンジ)
	{	spell_create_36_pache_agni_radiance,		SPELL_TIME_0128},	/* 36 */	/* [パチェ] "　火符「アグニレイディアンス」" No.23 */	// (火がいっぱいいっぱいだから) アグニ(インド神話の火の神)
	{	spell_create_37_pache_bury_in_lake, 		SPELL_TIME_0128},	/* 37 */	/* [パチェ] "　　　水符「ベリーインレイク」" No.24 */	// (水がいっぱいいっぱいだから)  湖に埋める(水葬)
	// 0x38- ボス専用スペル弾幕
	{	spell_create_38_pache_green_storm,			SPELL_TIME_0128},	/* 38 */	/* [パチェ] "　　　木符「グリーンストーム」" No.25 */	// (木がいっぱいいっぱいだから) 緑の嵐
	{	spell_create_39_pache_tririton_shake,		SPELL_TIME_0128},	/* 39 */	/* [パチェ] "　　土符「トリリトンシェイク」" No.26 */	// (土がいっぱいいっぱいだから) (揺れる)トリリトン(ストーンヘンジ)
	{	spell_create_3a_pache_silver_dragon,		SPELL_TIME_0128},	/* 3a */	/* [パチェ] "　　　金符「シルバードラゴン」" No.27 */	// (金(かね)がいっぱいいっぱいだから) 「金と銀(著者:福山)」
	{	spell_create_3b_pache_lava_cromlech,		SPELL_TIME_0128},	/* 3b */	/* [パチェ] "火＆土符「ラーヴァクロムレク」" No.28 */	// (火＋土＝溶岩) 環状列石(ストーンヘンジ)
	{	spell_create_3c_pache_forest_blaze, 		SPELL_TIME_0128},	/* 3c */	/* [パチェ] "木＆火符「フォレストブレイズ」" No.29 */	// (木＋火＝)森林火災
	{	spell_create_3d_pache_water_elf,			SPELL_TIME_0256},	/* 3d */	/* [パチェ] "　水＆木符「ウォーターエルフ」" No.30 */	// (水＋木＝)水溶性(水妖精)
	{	spell_create_3e_pache_mercury_poison,		SPELL_TIME_0128},	/* 3e */	/* [パチェ] "金＆水符「マーキュリポイズン」" No.31 */	// (金(金属)＋水＝)水銀中毒
	{	spell_create_3f_pache_emerald_megalith, 	SPELL_TIME_0128},	/* 3f */	/* [パチェ] "土＆金符「エメラルドメガリス」" No.32 */	// (土＋金(かね)＝)エメラルドの巨石
//
	{	spell_create_20_sonota_debug_cw_ao, 		SPELL_TIME_0128},	/* 40 */	/* [その他] デバッグ弾幕CW青 */
	{	spell_create_21_sonota_debug_ccw_aka,		SPELL_TIME_0128},	/* 41 */	/* [その他] デバッグ弾幕CCW赤 */
	{	spell_create_42_pache_laser1,				SPELL_TIME_0640},	/* 27 */	/* [パチェ] 通常攻撃(もどき) のレーザーもどき */
	{	spell_create_43_pache_laser2,				SPELL_TIME_0640},	/* 27 */	/* [パチェ] 通常攻撃(もどき) のレーザーもどき */
	{	spell_create_99_mitei,						SPELL_TIME_0640},	/* 26 */	/* 未定弾幕 */
	{	spell_create_99_mitei,						SPELL_TIME_0640},	/* 26 */	/* 未定弾幕 */
	{	spell_create_99_mitei,						SPELL_TIME_0640},	/* 26 */	/* 未定弾幕 */
	{	spell_create_99_mitei,						SPELL_TIME_0640},	/* 26 */	/* 未定弾幕 */


//	{	spell_create_20_sonota_debug_cw_ao, 		SPELL_TIME_0128},	/* 20 */	/* [その他] デバッグ弾幕CW青 */
//	{	spell_create_21_sonota_debug_ccw_aka,		SPELL_TIME_0128},	/* 21 */	/* [その他] デバッグ弾幕CCW赤 */
};

/*
dimmercation
Demarcation.
demarcation   境界

*/


/*---------------------------------------------------------
	スペル弾幕時間に予め登録された寿命時間をセットする。
---------------------------------------------------------*/

global void spell_set_time_out(SPRITE *src)
{
	src->boss_base_spell_time_out = system_spell_resource[(src->boss_base_spell_type/*-1*/)].spell_limit_max_time;
}


/*---------------------------------------------------------
	スペル弾幕を１フレーム生成する。
	スペル弾幕生成すると時間経過し、スペル弾幕寿命時間が自動で減る。
	スペル弾幕寿命時間が無くなった場合は、スペル弾幕タイプが無し(SPELL_00)になる。
---------------------------------------------------------*/

global void spell_generator(SPRITE *src)
{
	if (SPELL_00 != src->boss_base_spell_type)		/* スペル弾幕生成は必要？ */
	{
		src->boss_base_spell_time_out--;			/* 時間経過する */	/*fps_factor*/
		if (0 < src->boss_base_spell_time_out)		/* 時間切れ？ */
		{
			/* スペル弾幕生成中 */
			(system_spell_resource[(src->boss_base_spell_type/*-1*/)].spell_generate_callback)(src);
		}
		else
		{
			src->boss_base_spell_type = SPELL_00;		/* スペル弾幕生成終了 */
		}
	}
}
