
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

/*(r33)*/
/* === Gu敵面 === */		/* --- まだ作ってない --- */
enum	/* ☆ 魔方陣面エフェクト */
{
	TEKI_00_BOSS11			/* ボス & 中-ボス[右側1] */ 					= /*0x0400*/SP_GROUP_TEKI,
	TEKI_01_BOSS12, 		/* ボス & 中-ボス[右側2] */
	TEKI_02_BOSS13, 		/* ボス & 中-ボス[右側3] */
	TEKI_03_BOSS14, 		/* ボス & 中-ボス[右側4] */
	TEKI_04_BOSS21, 		/* ボス & 中-ボス[左側1] */
	TEKI_05_BOSS22, 		/* ボス & 中-ボス[左側2] */
	TEKI_06_BOSS23, 		/* ボス & 中-ボス[左側3] */
	TEKI_07_BOSS24, 		/* ボス & 中-ボス[左側4] */
//
	TEKI_08_BOSS31, 		/* ボス & 中-ボス[中心1] */
	TEKI_09_BOSS32, 		/* ボス & 中-ボス[中心2] */
	TEKI_10_BOSS33, 		/* ボス & 中-ボス[中心3] */
	TEKI_11_BOSS34, 		/* ボス & 中-ボス[中心4] */
	TEKI_12_YOUSEI1_1,		/* 妖精1 */
	TEKI_13_YOUSEI1_2,		/* 妖精1 */
	TEKI_14_YOUSEI1_3,		/* 妖精1 */
	TEKI_15_YOUSEI1_4,		/* 妖精1 */
//
	TEKI_16_YOUSEI1_5,		/* 妖精1 */ 	/* BOSS_16_YOUSEI11 */
	TEKI_17_YOUSEI1_6,		/* 妖精1 */
	TEKI_18_YOUSEI1_7,		/* 妖精1 */
	TEKI_19_YOUSEI1_8,		/* 妖精1 */
	TEKI_20_YOUSEI1_9,		/* 妖精1 */
	TEKI_21_YOUSEI1_a,		/* 妖精1 */
	TEKI_22_YOUSEI1_b,		/* 妖精1 */
	TEKI_23_YOUSEI1_c,		/* 妖精1 */
//
	TEKI_24_YOUSEI2_1,		/* 妖精2 */
	TEKI_25_YOUSEI2_2,		/* 妖精2 */
	TEKI_26_YOUSEI2_3,		/* 妖精2 */
	TEKI_27_YOUSEI2_4,		/* 妖精2 */
	TEKI_28_YOUSEI2_5,		/* 妖精2 */
	TEKI_29_YOUSEI2_6,		/* 妖精2 */
	TEKI_30_YOUSEI2_7,		/* 妖精2 */
	TEKI_31_YOUSEI2_8,		/* 妖精2 */
//
	TEKI_32_YOUSEI2_9,		/* 妖精2 */
	TEKI_33_YOUSEI2_a,		/* 妖精2 */
	TEKI_34_YOUSEI2_b,		/* 妖精2 */
	TEKI_35_YOUSEI2_c,		/* 妖精2 */
	TEKI_36_YOUSEI3_1,		/* 妖精3 */
	TEKI_37_YOUSEI3_2,		/* 妖精3 */
	TEKI_38_YOUSEI3_3,		/* 妖精3 */
	TEKI_39_YOUSEI3_4,		/* 妖精3 */
//
	TEKI_40_YOUSEI3_5,		/* 妖精3 */
	TEKI_41_YOUSEI3_6,		/* 妖精3 */
	TEKI_42_YOUSEI3_7,		/* 妖精3 */
	TEKI_43_YOUSEI3_8,		/* 妖精3 */
	TEKI_44_YOUSEI3_9,		/* 妖精3 */
	TEKI_45_YOUSEI3_a,		/* 妖精3 */
	TEKI_46_YOUSEI3_b,		/* 妖精3 */
	TEKI_47_YOUSEI3_c,		/* 妖精3 */
//
	TEKI_48_EFFECT1_resurved,	/* r29現在 未使用 */
	TEKI_49_EFFECT2_resurved,	/* r29現在 未使用 */
	TEKI_50_EFFECT3_resurved,	/* r29現在 未使用 */
	TEKI_51_EFFECT4_resurved,	/* r29現在 未使用 */
	TEKI_52_MAHOJIN,			/* 後ろの魔方陣 */
	TEKI_53_INYOU_DAI,
	TEKI_54_CHOU1,
	TEKI_55_CHOU2,
//
	TEKI_56_CHEN,
	TEKI_57_BLUE_BOOK,
	TEKI_58_RED_BOOK,
	TEKI_59_HAI_KEDAMA,
	TEKI_60_AKA_KEDAMA,
	TEKI_61_NIJI_HOSI,
	TEKI_62_TATSUMAKI,
	TEKI_63_HOMING_STUFF,
};
#define BOSS_00_BOSS11		TEKI_00_BOSS11
#define BOSS_16_YOUSEI11	TEKI_16_YOUSEI1_5

/* === Gu魔方陣面 === */		/* --- まだ作ってない --- */

//enum	/* ☆ 魔方陣面エフェクト */
//{
//	MAHOU_JIN_00_aaa	= /*0x00010000*/SP_GROUP_MAHOU_JIN,
//	MAHOU_JIN_01_bbb,
//	MAHOU_JIN_02_,
//	MAHOU_JIN_03_,
//	MAHOU_JIN_04_,
//	MAHOU_JIN_05_,
//	MAHOU_JIN_06_,
//	MAHOU_JIN_07_,
//};

/* === Guフロント面 === */		/* --- まだ作ってない --- */

enum	/* ☆ フロント面エフェクト */
{	/* 自分のあたり判定位置表示用コア(フロント面エフェクト) */
	zzz_JIKI_CORE_00_REIMU_A	= /*0x4000*/SP_GROUP07_FRONT,
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
	SP_DUMMY_MUTEKI,//S P_GROUP_ETC_DUMMY_REMILIA,
	SP_FRONT_YUKI,//S P_GROUP_ETC_DUMMY_SLOW_BOMB,		//	SPELL_LOGO_,/* Border Power of Spiritual */ 	//	SPELL_TACHIE_,
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
//#define SP_DUMMY_MUTEKI S P_GROUP_ETC_DUMMY_REMILIA
//#define SP_FRONT_YUKI 	S P_GROUP_ETC_DUMMY_SLOW_BOMB

/* Border Power of Spiritual. 霊歌で力を発揮する境界. 霊歌での力の境界 */
/* Border Power of Spiritus.  酒精で力を発揮する境界. 酒精での力の境界 */
/*
Spiritus  酒精
Spiritual 霊歌
霊歌の境界力
Border Power of Spiritual. 霊歌のパワーに接してください。
Border Power of Spiritus. 酒精のパワーに接してください。
*/

/* === Guプレイヤー面 === */
enum/* オプションとインターリーブしてるのは、(オプションアニメ)プログラムの都合 */
{
// [5]
	JIKI_OPTION_00_00			= SP_GROUP_JIKI_GET_ITEM/*0x0800*/,
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
	JIKI_BOMBER_01, 		/* 誘導ボム(低速ボム) */
	JIKI_BOMBER_02, 		/* --- 未使用 --- / 結界白 / 長炎1 */
	JIKI_BOMBER_03, 		/* --- 未使用 --- / 結界黄 / 長炎2 */
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

/* === Gu弾幕面 === */

/* TAMA_GROUIP_08_MASK_0xfff8: 8 弾 == 1 グループとして管理する場合に、グループ区別をする為のマスク値 */
#define TAMA_GROUIP_08_MASK_0xfff8 (0xfff8)
#define is_tama_grouip08(aaa) (aaa & TAMA_GROUIP_08_MASK_0xfff8)
enum
{
//1 小丸弾
	BULLET_00_HOSI_TEN			= SP_GROUP_BULLETS/*0x0800*/,
	BULLET_MARU10_01_AKA,			// 赤
	BULLET_MARU10_02_YUKARI,		// 紫
	BULLET_MARU10_03_AOI,			// 青
	BULLET_MARU10_04_MIZU_IRO,		// 水
	BULLET_MARU10_05_MIDORI,		// 緑
	BULLET_MARU10_06_KI_IRO,		// 黄
	BULLET_MARU10_07_DAI_DAI,		// 橙
//2 米弾
	BULLET_KOME_00_SIRO,			// 白
	BULLET_KOME_01_AKA, 			// 赤
	BULLET_KOME_02_YUKARI,			// 紫
	BULLET_KOME_03_AOI, 			// 青
	BULLET_KOME_04_MIZU_IRO,		// 水
	BULLET_KOME_05_MIDORI,			// 緑
	BULLET_KOME_06_KI_IRO,			// 黄
	BULLET_KOME_07_DAI_DAI, 		// 橙
//3 ミニ弾
	BULLET_MINI8_00_SIRO,			// 白
	BULLET_MINI8_01_AKA,			// 赤
	BULLET_MINI8_02_YUKARI, 		// 紫
	BULLET_MINI8_03_AOI,			// 青
	BULLET_MINI8_04_MIZU_IRO,		// 水
	BULLET_MINI8_05_MIDORI, 		// 緑
	BULLET_MINI8_06_KI_IRO, 		// 黄
	BULLET_MINI8_07_DAI_DAI,		// 橙
//4 枠付き中丸弾
	BULLET_WAKU12_00_SIRO,			// 白	/*12*/	[チルノ]
	BULLET_WAKU12_01_AKA,			// 赤	/*12*/	[チルノ]
	BULLET_WAKU12_02_KURO_AKA,		// [紫] /*12*/	レミリア用の黒枠赤弾			//	BULLET_CAP16_04_KOME_SIROI, 	/* [青白米弾] */
	BULLET_WAKU12_03_AOI,			// 青	/*12*/	[チルノ]
	BULLET_CAP16_04_TUTU_SIROI, 	// [水] /*12*/						/* [青白実包弾] */
	BULLET_WAKU12_05_MIDORI,		// 緑	/*12*/	[チルノ]
	BULLET_WAKU12_06_KI_IRO,		// 黄	/*12*/	[チルノ]	//	BULLET_MINI8_12_dummy,
	BULLET_WAKU12_07_DAIDAI,		// 橙	/*12*/	[チルノ]	//	BULLET_MINI8_13_dummy,
//5 ふぐちり弾(鱗弾)
	BULLET_UROKO14_00_SIRO, 		// 白
	BULLET_UROKO14_01_AKA,			// 赤
	BULLET_UROKO14_02_YUKARI,		// 紫
	BULLET_UROKO14_03_AOI,			// 青
	BULLET_UROKO14_04_MIZUIRO,		// 水
	BULLET_UROKO14_05_MIDORI,		// 緑
	BULLET_UROKO14_06_KI_IRO,		// 黄
	BULLET_UROKO14_07_DAIDAI,		// 橙
//6 クナイ弾
	BULLET_KUNAI12_00_SIRO, 		// 白
	BULLET_KUNAI12_01_AKA,			// 赤
	BULLET_KUNAI12_02_YUKARI,		// 紫
	BULLET_KUNAI12_03_AOI,			// 青
	BULLET_KUNAI12_04_MIZUIRO,		// 水
	BULLET_KUNAI12_05_MIDORI,		// 緑
	BULLET_KUNAI12_06_KI_IRO,		// 黄
	BULLET_KUNAI12_07_DAIDAI,		// 橙
//7 札弾 / ナイフ弾
	BULLET_OFUDA12_00_AOI,
	BULLET_OFUDA12_01_AKA,
	BULLET_OFUDA12_02_YUKARI,
	BULLET_OFUDA12_03_MIDORI,
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

/* === GuITEM面 === */
enum /*sprite_type*/
{
/*	アイテム */
	SP_ITEM_00_P001 	/* [p]小 */ 								= /*0x1000*/SP_GROUP_ITEMS, 	/* Bonus items */
	SP_ITEM_01_P008,	/* [P]大 */
	SP_ITEM_02_P128,	/* [F] */
	SP_ITEM_03_1UP, 	/* [1up] */
	SP_ITEM_04_BOMB,	/* [B] */
	SP_ITEM_05_TENSU,	/* [点] */
	SP_ITEM_06_HOSI,	/* [☆] */
	SP_ITEM_07_SPECIAL,
};

/* === GuFRONT面 === */
