
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/



/* === Gu魔方陣面 === */		/* --- まだ作ってない --- */

//enum	/* ☆ 魔方陣面エフェクト */
//{
//	MAHOU_JIN_00_aaa	= OBJ_Z00_MAHOU_JIN,
//	MAHOU_JIN_01_bbb,
//	MAHOU_JIN_02_,
//	MAHOU_JIN_03_,
//	MAHOU_JIN_04_,
//	MAHOU_JIN_05_,
//	MAHOU_JIN_06_,
//	MAHOU_JIN_07_,
//};

/* === Guプレイヤー面 === */
enum/* オプションとインターリーブしてるのは、(オプションアニメ)プログラムの都合 */
{
// [5]
	JIKI_OPTION_00_00			= (OBJ_Z01_JIKI_GET_ITEM),
	JIKI_OPTION_00_01,
	JIKI_OPTION_00_02,
	JIKI_OPTION_00_03,
	JIKI_OPTION_00_04,
	JIKI_OPTION_00_05,
	JIKI_OPTION_00_06,
	JIKI_OPTION_00_07,

//	プレイヤーアニメーション
// [1]	左へ移動のアニメーション	[中心→左側4→左側3→左側2→左側1→左側1→左側1→...]
	JIKI_PLAYER_00_LEFT,	/* 左側1 */ 	/* 最も左 */
	JIKI_PLAYER_01_LEFT,	/* 左側2 */
	JIKI_PLAYER_02_LEFT,	/* 左側3 */
	JIKI_PLAYER_03_LEFT,	/* 左側4 */ 	/* 中より左 */
//	繰り返しアニメーション		[中心1→中心2→中心3→中心4→中心1→中心2→中心3→中心4→...]
	JIKI_PLAYER_04_MID, 	/* 中心1 */
	JIKI_PLAYER_05_MID, 	/* 中心2 */
	JIKI_PLAYER_06_MID, 	/* 中心3 */
	JIKI_PLAYER_07_MID, 	/* 中心4 */

// [6]
	JIKI_OPTION_01_00,
	JIKI_OPTION_01_01,
	JIKI_OPTION_01_02,
	JIKI_OPTION_01_03,
	JIKI_OPTION_01_04,
	JIKI_OPTION_01_05,
	JIKI_OPTION_01_06,
	JIKI_OPTION_01_07,

// [2]	右へ移動のアニメーション	[中心→右側1→右側2→右側3→右側4→右側4→右側4→...]
	JIKI_PLAYER_08_RIGHT,	/* 右側1 */ 	/* 中より右 */
	JIKI_PLAYER_09_RIGHT,	/* 右側2 */
	JIKI_PLAYER_10_RIGHT,	/* 右側3 */
	JIKI_PLAYER_11_RIGHT,	/* 右側4 */ 	/* 最も右 */
	JIKI_ETC_00,			/* --- 未使用 --- */
	JIKI_ETC_01,			/* --- 未使用 --- */
	JIKI_ETC_02,			/* --- 未使用 --- */
	JIKI_ETC_03,			/* --- 未使用 --- */

// [7]
	JIKI_OPTION_02_00,
	JIKI_OPTION_02_01,
	JIKI_OPTION_02_02,
	JIKI_OPTION_02_03,
	JIKI_OPTION_02_04,
	JIKI_OPTION_02_05,
	JIKI_OPTION_02_06,
	JIKI_OPTION_02_07,

// [3]
	JIKI_BOMBER_00, 		/* ボンバー発動時の立ち絵 */
	JIKI_BOMBER_01, 		/* 赤88弾 / 誘導ボム(低速ボム) */
	JIKI_BOMBER_02, 		/* --- 未使用 --- / 蝶弾16赤 / 結界白 / 長炎1 */
	JIKI_BOMBER_03, 		/* --- 未使用 --- / 蝶弾16青 / 結界黄 / 長炎2 */
	JIKI_BOMBER_04, 		/* 黄札1 / 星1 / 十字炎1 / 蝶1 / チ */
	JIKI_BOMBER_05, 		/* 黄札2 / 星2 / 十字炎2 / 蝶2 / ル */
	JIKI_BOMBER_06, 		/* 黄札3 / 星3 / 十字炎3 / 蝶3 / ノ */
	JIKI_BOMBER_07, 		/* 黄札4 / 星4 / 十字炎4 / 蝶4 / ⑨ */

// [8]
	JIKI_OPTION_03_00,
	JIKI_OPTION_03_01,
	JIKI_OPTION_03_02,
	JIKI_OPTION_03_03,
	JIKI_OPTION_03_04,
	JIKI_OPTION_03_05,
	JIKI_OPTION_03_06,
	JIKI_OPTION_03_07,

// [4]
	JIKI_SHOT_00,			/* 赤札C /	/  / 長炎3 / */
	JIKI_SHOT_01,			/* 赤札B /	/  / 長炎4 / */
	JIKI_SHOT_02,			/* 赤札A / 黄星 / 小炎 / 氷 / 青蝶 */
	JIKI_SHOT_03,			/* / こうもり弾1 / 黄札1 */
	JIKI_SHOT_04,			/* / こうもり弾2 / 黄札2 */
	JIKI_SHOT_05,			/* / こうもり弾3 / 黄札3 */
	JIKI_SHOT_06,			/* / こうもり弾4 / 黄札4 */
	JIKI_SHOT_07,			/* / こうもり弾5 / 針弾 / 森弾 / ウェイブ弾 / ピンク蝶弾 */

};

/* === Gu敵面 === */		/* --- まだ作ってない --- */
	/* ☆ 魔方陣面エフェクト */

/* 敵共通規格(ボス/中ボス/ザコ/ボスオプション/その他) */
enum /*_teki_common_kikaku_*/
{
/*
xx:未使用。
ok:変換済み。
*/
/*r34	r33*/
/*00xx	*/	TEKI_00_00		/* 妖精0[右側1] */		= (OBJ_Z02_TEKI),
/*01xx	*/	TEKI_01_01, 	/* 妖精0[右側2] */
/*02xx	*/	TEKI_02_02, 	/* 妖精0[右側3] */
/*03xx	*/	TEKI_03_03, 	/* 妖精0[右側4] */
/*04xx	*/	TEKI_04_04, 	/* 妖精0[左側1] */
/*05xx	*/	TEKI_05_05, 	/* 妖精0[左側2] */
/*06xx	*/	TEKI_06_06, 	/* 妖精0[左側3] */
/*07xx	*/	TEKI_07_07, 	/* 妖精0[左側4] */
//
/*08xx	*/	TEKI_08_08, 	/* 妖精0[攻撃1] */
/*09xx	*/	TEKI_09_09, 	/* 妖精0[攻撃2] */
/*10	*/	TEKI_10_0a, 	/* 妖精0[攻撃3] */
/*11	*/	TEKI_11_0b, 	/* 妖精0[攻撃4] */
/*12	*/	TEKI_12_0c, 	/* 妖精0[未定1] */
/*13	*/	TEKI_13_0d, 	/* 妖精0[未定2] */
/*14	*/	TEKI_14_0e, 	/* 妖精0[未定3] */
/*15	*/	TEKI_15_0f, 	/* 妖精0[未定4] */
//
/*16ok	*/	TEKI_16_10, 	/* 妖精1[右向き1] */
/*17xx	*/	TEKI_17_11, 	/* 妖精1[右向き2] */
/*18xx	*/	TEKI_18_12, 	/* 妖精1[右向き3] */
/*19xx	*/	TEKI_19_13, 	/* 妖精1[右向き4] */
/*20xx	*/	TEKI_20_14, 	/* 妖精1[正面向き1] */	/* BOSS_20_YOUSEI1_SYOUMEN */
/*21xx	*/	TEKI_21_15, 	/* 妖精1[正面向き2] */
/*22xx	*/	TEKI_22_16, 	/* 妖精1[正面向き3] */
/*23xx	*/	TEKI_23_17, 	/* 妖精1[正面向き4] */
//
/*24	*/	TEKI_24_18, 	/* 妖精1[左向き1] */
/*25	*/	TEKI_25_19, 	/* 妖精1[左向き2] */
/*26	*/	TEKI_26_1a, 	/* 妖精1[左向き3] */
/*27	*/	TEKI_27_1b, 	/* 妖精1[左向き4] */
/*28ok	*/	TEKI_28_1c, 	/* 妖精1[未定1] */
/*29ok	*/	TEKI_29_1d, 	/* 妖精1[未定2] */
/*30ok	*/	TEKI_30_1e, 	/* 妖精1[未定3] */
/*31xx	*/	TEKI_31_1f, 	/* 妖精1[未定4] */
//
/*32ok	*/	TEKI_32_20, 	/* 妖精2[右向き1] */
/*33	*/	TEKI_33_21, 	/* 妖精2[右向き2] */
/*34	*/	TEKI_34_22, 	/* 妖精2[右向き3] */
/*35	*/	TEKI_35_23, 	/* 妖精2[右向き4] */
/*36ok	*/	TEKI_36_24, 	/* 妖精2[正面向き1] */
/*37	*/	TEKI_37_25, 	/* 妖精2[正面向き2] */
/*38	*/	TEKI_38_26, 	/* 妖精2[正面向き3] */
/*39	*/	TEKI_39_27, 	/* 妖精2[正面向き4] */
//
/*40	*/	TEKI_40_28, 	/* 妖精2[左向き1] */
/*41	*/	TEKI_41_29, 	/* 妖精2[左向き2] */
/*42	*/	TEKI_42_2a, 	/* 妖精2[左向き3] */
/*43	*/	TEKI_43_2b, 	/* 妖精2[左向き4] */
/*44ok	*/	TEKI_44_2c, 	/* 妖精2[未定1] */
/*45xx	*/	TEKI_45_2d, 	/* 妖精2[未定2] */
/*46ok	*/	TEKI_46_2e, 	/* 妖精2[未定3] */
/*47ok	*/	TEKI_47_2f, 	/* 妖精2[未定4] */
//
/*48ok	*/	TEKI_48_30, 	/* 妖精3[右向き1] */
/*49	*/	TEKI_49_31, 	/* 妖精3[右向き2] */
/*50	*/	TEKI_50_32, 	/* 妖精3[右向き3] */
/*51	*/	TEKI_51_33, 	/* 妖精3[右向き4] */
/*52	*/	TEKI_52_34, 	/* 妖精3[正面向き1] */
/*53	*/	TEKI_53_35, 	/* 妖精3[正面向き2] */
/*54	*/	TEKI_54_36, 	/* 妖精3[正面向き3] */
/*55	*/	TEKI_55_37, 	/* 妖精3[正面向き4] */
//
/*56	*/	TEKI_56_38, 	/* 妖精3[左向き1] */
/*57	*/	TEKI_57_39, 	/* 妖精3[左向き2] */
/*58	*/	TEKI_58_3a, 	/* 妖精3[左向き3] */
/*59	*/	TEKI_59_3b, 	/* 妖精3[左向き4] */
/*60ok	*/	TEKI_60_3c, 	/* 妖精3[未定1] */
/*61ok	*/	TEKI_61_3d, 	/* 妖精3[未定2] */
/*62ok	*/	TEKI_62_3e, 	/* 妖精3[未定3] */
/*63	*/	TEKI_63_3f, 	/* 妖精3[未定4] */
};
#if (1)
	/* ボス/ボスオプション用設定 */
	#define 	BOSS_00_11				TEKI_00_00/* ボス & 中-ボス[右側1] */
//	#define 	BOSS_01_12				TEKI_01_01/* ボス & 中-ボス[右側2] */
//	#define 	BOSS_02_13				TEKI_02_02/* ボス & 中-ボス[右側3] */
//	#define 	BOSS_03_14				TEKI_03_03/* ボス & 中-ボス[右側4] */
	#define 	BOSS_04_21				TEKI_04_04/* ボス & 中-ボス[左側1] */
//	#define 	BOSS_05_22				TEKI_05_05/* ボス & 中-ボス[左側2] */
//	#define 	BOSS_06_23				TEKI_06_06/* ボス & 中-ボス[左側3] */
//	#define 	BOSS_07_24				TEKI_07_07/* ボス & 中-ボス[左側4] */

//	#define 	BOSS_08_31				TEKI_08_08/* ボス & 中-ボス[攻撃1] */
	#define 	BOSS_09_32				TEKI_09_09/* ボス & 中-ボス[攻撃2] */
	#define 	BOSS_10_33				TEKI_10_0a/* ボス & 中-ボス[攻撃3] */
//	#define 	BOSS_11_34				TEKI_11_0b/* ボス & 中-ボス[攻撃4] */
//	#define 	BOSS_12_41				TEKI_12_0c/* ボス & 中-ボス[未定1] */
//	#define 	BOSS_13_42				TEKI_13_0d/* ボス & 中-ボス[未定2] */
//	#define 	BOSS_14_43				TEKI_14_0e/* ボス & 中-ボス[未定3] */
//	#define 	BOSS_15_44				TEKI_15_0f/* ボス & 中-ボス[未定4] */

	#define BOSS_20_YOUSEI1_SYOUMEN		TEKI_20_14
	#define ZAKO_YOUSEI_01				TEKI_16_10

	/* ザコ用設定 */
//	#define ZAKO_12_EFFECT1_resurved	TEKI_12_0c		/* r29現在 未使用 */
//	#define ZAKO_13_EFFECT2_resurved	TEKI_13_0d		/* r29現在 未使用 */
//	#define ZAKO_14_EFFECT3_resurved	TEKI_14_0e		/* r29現在 未使用 */
//	#define ZAKO_15_EFFECT4_resurved	TEKI_15_0f		/* r29現在 未使用 */

	#define ZAKO_28_MAHOJIN 			TEKI_28_1c		/* 後ろの魔方陣 */


	#define ZAKO_63_HOMING_STUFF		TEKI_63_3f
#endif


/* === GuITEM面 === */
enum /*sprite_type*/
{
	/* アイテム */
	SP_ITEM_00_P001 	/* [p]小 */ 		= (OBJ_Z03_ITEM), 	/* Bonus items */
	SP_ITEM_01_P008,	/* [P]大 */
	SP_ITEM_02_P128,	/* [F] */
	SP_ITEM_03_1UP, 	/* [1up] */
	SP_ITEM_04_BOMB,	/* [B] */
	SP_ITEM_05_TENSU,	/* [点] */
	SP_ITEM_06_HOSI,	/* [☆] */
	SP_ITEM_07_SPECIAL,
};


/* === Gu弾面 === */

// グループ弾の場合、個別に色指定しない。
// 色指定する場合は、下記共通色指定を使う。
enum
{
	TAMA_IRO_00_SIRO = 0,	// 白
	TAMA_IRO_01_AKA,		// 赤
	TAMA_IRO_02_YUKARI, 	// 紫
	TAMA_IRO_03_AOI,		// 青
	TAMA_IRO_04_MIZU_IRO,	// 水
	TAMA_IRO_05_MIDORI, 	// 緑
	TAMA_IRO_06_KI_IRO, 	// 黄
	TAMA_IRO_07_DAI_DAI,	// 橙
};
enum
{
//1 小丸弾(グループ弾)
	BULLET_MARU10_BASE/*[星点]*/			= (OBJ_Z04_TAMA),
	BULLET_MARU10_01_,// 赤
	BULLET_MARU10_02_,// 紫
	BULLET_MARU10_03_,// 青
	BULLET_MARU10_04_,// 水
	BULLET_MARU10_05_,// 緑
	BULLET_MARU10_06_,// 黄
	BULLET_MARU10_07_,// 橙
//2 米弾(グループ弾)
	BULLET_KOME_BASE,// 白
	BULLET_KOME_01_,// 赤
	BULLET_KOME_02_,// 紫
	BULLET_KOME_03_,// 青
	BULLET_KOME_04_,// 水
	BULLET_KOME_05_,// 緑
	BULLET_KOME_06_,// 黄
	BULLET_KOME_07_,// 橙
//3 ミニ弾(グループ弾)
	BULLET_MINI8_BASE,// 白
	BULLET_MINI8_01_,// 赤
	BULLET_MINI8_02_,// 紫
	BULLET_MINI8_03_,// 青
	BULLET_MINI8_04_,// 水
	BULLET_MINI8_05_,// 緑
	BULLET_MINI8_06_,// 黄
	BULLET_MINI8_07_,// 橙
//4 枠付き中丸弾(グループ弾)
	BULLET_WAKU12_BASE,// 白	/*12*/	[チルノ]
	BULLET_WAKU12_01_,/// 赤	/*12*/	[チルノ]
	BULLET_WAKU12_02_,//KURO_AKA,		// [紫] /*12*/	レミリア用の黒枠赤弾			//	BULLET_CAP16_04_KOME_SIROI, 	/* [青白米弾] */
	BULLET_WAKU12_03_,// 青 	/*12*/	[チルノ]
	BULLET_WAKU12_04_,// 水 	//(r35) BULLET_CAP16_04_TUTU_SIROI, 	// [水] /*12*/						/* [青白実包弾] */
	BULLET_WAKU12_05_,// 緑 	/*12*/	[チルノ]
	BULLET_WAKU12_06_,// 黄 	/*12*/	[チルノ]	//	BULLET_MINI8_12_dummy,
	BULLET_WAKU12_07_,// 橙 	/*12*/	[チルノ]	//	BULLET_MINI8_13_dummy,
//5 ふぐちり弾(鱗弾) (グループ弾)
	BULLET_UROKO14_BASE,// 白
	BULLET_UROKO14_01_,// 赤
	BULLET_UROKO14_02_,// 紫
	BULLET_UROKO14_03_,// 青
	BULLET_UROKO14_04_,// 水
	BULLET_UROKO14_05_,// 緑
	BULLET_UROKO14_06_,// 黄
	BULLET_UROKO14_07_,// 橙
//6 クナイ弾(グループ弾)
	BULLET_KUNAI12_BASE,// 白
	BULLET_KUNAI12_01_,// 赤
	BULLET_KUNAI12_02_,// 紫
	BULLET_KUNAI12_03_,// 青
	BULLET_KUNAI12_04_,// 水
	BULLET_KUNAI12_05_,// 緑
	BULLET_KUNAI12_06_,// 黄
	BULLET_KUNAI12_07_,// 橙
//7 札弾 / ナイフ弾
	BULLET_OFUDA12_00_AOI,
	BULLET_OFUDA12_01_AKA,
	BULLET_OFUDA12_02_YUKARI,
	BULLET_CAP16_04_TUTU_SIROI, 	//(r35) BULLET_OFUDA12_03_MIDORI,
	BULLET_KNIFE20_04_AOI,
	BULLET_KNIFE20_05_AKA,
	BULLET_KNIFE20_06_YUKARI,
	BULLET_KNIFE20_07_MIDORI,
//8 針弾 / 大玉弾
	BULLET_HARI32_00_AOI,
	BULLET_HARI32_01_AKA,
	BULLET_HARI32_02_KIIRO,
	BULLET_HARI32_03_DAIDAI,
	BULLET_OODAMA32_00_AOI, 	/* 青色 */
	BULLET_OODAMA32_01_AKA, 	/* 赤色 */
	BULLET_OODAMA32_02_KIIRO,	/* 黄色 */
	BULLET_OODAMA32_03_MIDORI,	/* 緑色 */
};
#define BULLET_00_HOSI_TEN BULLET_MARU10_BASE


/* === Guフロント面 === */		/* --- まだ作ってない --- */

enum	/* ☆ フロント面エフェクト */
{	/* 自分のあたり判定位置表示用コア(フロント面エフェクト) */
	zzz_JIKI_CORE_00_REIMU_A	= (OBJ_Z05_FRONT),
	zzz_JIKI_CORE_01_REIMU_B,
	zzz_JIKI_CORE_02_MARISA_A,
	zzz_JIKI_CORE_03_MARISA_B,
	zzz_JIKI_CORE_04_MARISA_C,
	zzz_JIKI_CORE_05_REMILIA,
	zzz_JIKI_CORE_06_CHIRNO,
	zzz_JIKI_CORE_07_YUYUKO,
//	/* CORE隠す用 */
	zzz_CORE_HIDE_10_REIMU_A,
	zzz_CORE_HIDE_11_REIMU_B,
	zzz_CORE_HIDE_12_MARISA_A,
	zzz_CORE_HIDE_13_MARISA_B,
	zzz_CORE_HIDE_14_MARISA_C,
	zzz_CORE_HIDE_15_REMILIA,
	zzz_CORE_HIDE_16_CHIRNO,
	zzz_CORE_HIDE_17_YUYUKO,
//	/* 小爆発 / ザコ消滅爆発(フロント面エフェクト) */
	BAKUHA00,	/* 小爆発(黄色?、現在ダミー) */
	BAKUHA01,	/* 小爆発(青)BASE_TR_BLUE_PNG */
	BAKUHA02,	/* 小爆発(赤)BASE_TR_RED_PNG */
	BAKUHA03,	/* 小爆発(緑)BASE_TR_GREEN_PNG */
	BAKUHA04,	/* ザコ消滅爆発(黄色、現在ダミー) */
	BAKUHA05,	/* ザコ消滅爆発(青)BASE_BAKUHA05_PNG */
	BAKUHA06,	/* ザコ消滅爆発(赤)BASE_BAKUHA06_PNG */
	BAKUHA07,	/* ザコ消滅爆発(緑)BASE_BAKUHA07_PNG */
//	/* 火炎爆発(フロント面エフェクト) */
	BAKUHA08,	/* 火炎爆発 */
	BAKUHA09,	/* 火炎爆発 */
	BAKUHA0a,	/* 火炎爆発 */
	BAKUHA0b,	/* 火炎爆発 */
	BAKUHA0c,	/* 火炎爆発 */
	BAKUHA0d,	/* 火炎爆発 */
	BAKUHA0e,	/* 火炎爆発 */
	BAKUHA0f,	/* 火炎爆発 */
//
	SPELL_SQUERE_,	/* [□] x 5 */
	SP_DUMMY_MUTEKI,//
	SP_FRONT_YUKI,// //	SPELL_LOGO_,/* Border Power of Spiritual */ 	//	SPELL_TACHIE_,
	PANEL_STR_MAX,
	PANEL_STR_EASY,
	PANEL_STR_NORMAL,
	PANEL_STR_HARD,
	PANEL_STR_LUNATIC,
//
	PANEL_STR_EXTRA,
	PANEL_STR_TIME,
	PANEL_STR_fps,
	PANEL_STR_0_roman,
	PANEL_STR_1_roman,
	PANEL_STR_2_roman,
	PANEL_STR_3_roman,
	PANEL_STR_4_roman,
//
	PANEL_STR_5_roman,
	PANEL_STR_6_roman,
	PANEL_STR_7_roman,
	PANEL_STR_8_roman,
	PANEL_STR_9_roman,
	PANEL_STR_ene_my,
	PANEL_STR_0_kanji,
	PANEL_STR_1_kanji,
//
	PANEL_STR_2_kanji,
	PANEL_STR_3_kanji,
	PANEL_STR_4_kanji,
	PANEL_STR_5_kanji,
	PANEL_STR_6_kanji,
	PANEL_STR_7_kanji,
	PANEL_STR_8_kanji,
	PANEL_STR_9_kanji,
//
};
/*
	SP_DUMMY_MUTEKI == FRONT 面なのであたり判定なし。それから表示されない。
	SP_FRONT_YUKI == FRONT 面なのであたり判定なし。(とりあえず仮で)雪みたいな表示。
*/


/* Border Power of Spiritual. 霊歌で力を発揮する境界. 霊歌での力の境界 */
/* Border Power of Spiritus.  酒精で力を発揮する境界. 酒精での力の境界 */
/*
Spiritus  酒精
Spiritual 霊歌
霊歌の境界力
Border Power of Spiritual. 霊歌のパワーに接してください。
Border Power of Spiritus. 酒精のパワーに接してください。
*/
