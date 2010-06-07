
/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _SPRITE_H_
#define _SPRITE_H_

/*---------------------------------------------------------
	スプライト マネージャ
--------------------------------------------------------- */

//#define SP_FLAG_COLISION_CHECK	(0x01)				/* あたり判定ありフラグ */
//#define SP_FLAG_GRAZE 			(0x10)				/* グレイズ済みかとうかのフラグ */
#define SP_FLAG_COLISION_CHECK		(0x8000)			/* あたり判定ありフラグ */
#define SP_FLAG_GRAZE				(0x0080)			/* グレイズ済みかとうかのフラグ */
#define SP_FLAG_OPTION_VISIBLE		(SP_FLAG_GRAZE) 	/* プレイヤーオプション有効フラグ */


/* Sprites */
//#define MAX_SPRITE 200

#define USER_BOMOUT_WAIT (30)

/* 使用済みだから消してくれフラグ */
#define SP_DELETE				(0)
/*	0 ならば判別処理が省略できる。 -1 だと一々判別処理が入る
	KETMでは判別しないとならない場所に判別処理が書いてない為、
	スプライトがおかしくなるバグが複数ヶ所にある。 */
/*
	ここは種類別ではなくて、機能別に分類してください。
*/
#define SP_GROUP_SHOT_ZAKO					(0x0000)/*0x0100*/
#define SP_GROUP_SHOT_SPECIAL				(0x0080)/*0x0100*/		/* 霊夢の回転ショット */		/* kodomo_hlaserの判別 */
//
#define SP_GROUP_JIKI_GET_ITEM				(0x0100)/*0x0100*/		/* === Guプレイヤー面 === */
//#define dummy_SP_GROUP_PLAYER_CORE		(0x0200)/*0x0100*/
#define SP_GROUP_TEKI						(0x0200)/*0x0400*/		/* === Gu敵面 === */
//#define SP_GROUP_BOSS 					(0x0800)/*0x0400*/		/* === Gu敵面 === */
//#define SP_GROUP_TEKI 					(0x0400)/*0x0400*/
//#define SP_GROUP_TEKI 					(SP_GROUP_TEKI|0)
//#define SP_GROUP_BOSS 					(SP_GROUP_TEKI|SP_GROUP_SHOT_SPECIAL)
#define SP_GROUP_BULLETS					(0x0400)/*(0x1000)0x0800*/		/* === Gu弾幕面 === */
#define SP_GROUP_ITEMS						(0x0800)/*(0x2000)0x1000*/
//#define SP_GROUP_PAUSE_S P_ME NU_TEXT 	(0x4000)/*0x2000*/		/*SDL*/
#define SP_GROUP_ETC						(0x1000)/*(0x8000)*/				/* === Guフロント面 === */

//#define SP_GROUP_ALL_SDL_TYPE (0xff00)
#define SP_GROUP_ALL_SDL_WORK_TYPE			(0x1f00)/* SDL動作 */
//#define SP_GROUP_ALL_SDL_CORE_TYPE		((0x1f00) & (~(SP_GROUP_PAUSE_S P_ME NU_TEXT|0)))/* game_core動作 */
#define SP_GROUP_ALL_SDL_CORE_TYPE			(0x1f00)/* game_core動作 */

//#define SP_GROUP_ALL_SDL_DRAW_TYPE		(SP_GROUP_ALL_SDL_WORK_TYPE & (~(SP_GROUP_TEKI|SP_GROUP_ITEMS|SP_GROUP_BULLETS)))/*SDL表示*/
/* 敵＆アイテム＆敵弾は少なくともSDLで描かない */
//#define SP_GROUP_ALL_SDL_DRAW_TYPE		(0x4000)/* SDL表示(現状SP_GROUP_PAUSE_S P_ME NU_TEXTのみSDL描画) */

#define SP_GROUP_SUB_TYPE_256				(0x00ff)
#define SP_GROUP_SUB_TYPE_128				(0x007f)
/* 以下あたり判定なし */
#define SP_GROUP_MAHOU_JIN					(0x2000)/*(0x00010000)*/			/* === Gu魔方陣面 === */
#define SP_GROUP_PANEL						(0x4000)/*(0x00020000)*/			/* === Guパネル面 === */
//#define SP_FLAG_COLISION_CHECK			(0x8000)/*(0x00020000)*/			/* === Guパネル面 === */

#define SP_GROUP_ALL_GAME_OBJS				(0x00ffff00)			/* ゲームコア終了時にかたづけるobj */
//#define SP_GROUP_SHOTS					(0x0200)
//#define SP_GROUP_ETC						(0x4000)
//#define SP_GROUP_ENEMYS					(0x0200)/*(0x0400)*/
/* 共用 */
//#define SP_GROUP_TEXTS					(SP_GROUP_PLAYER)/*(0x2000)*/

/* SP_GROUP_ETC == SP_GROUP_FRONT */
#define SP_GROUP_FRONT						(SP_GROUP_ETC)/* フロント面 */

		//←ウェポンアイテム(中)	//ウェポンアイテム(強)→SP_ITEM_EXTRA_SHIELD,
		//→これらは低速ボムに吸収された。SP_ITEM_EXTRA_HOMING, SP_ITEM_EXTRA_HLASER,
/*	自分A */
//	S P_PLAYER_ITEM/*プレイヤーの本体(あたり判定がアイテム取得)*/			= /*0x0100*/SP_GROUP_JIKI_GET_ITEM, /* Unser Held */
/*	自弾 */
//	S P_SHOT_BOSS,/*プレイヤーのボスに有効弾*/							//	= /*0x0200*/SP_GROUP_SHOTS, 	/* Waffen des Helden */
//	S P_SHOT_ZAKO,/*プレイヤーのボスに無効弾*/
/*	自分B */
	//dummy_SP_PLAYER_CORE/*プレイヤーのコア(あたり判定が死亡判定)*/		= /*0x0100*/SP_GROUP_PLAYER_CORE,
/* ------------- */
/*	ザコ敵 (ボム中、体当たりでプレイヤー死なない)  */
//	SP_ZAKO 																= /*0x0400*/SP_GROUP_TEKI,		/* Die Boesen */
/* ------------- */
/*	ボス敵 / 中ザコ敵 (ボム中、体当たりでプレイヤー死ぬ)  */
//	S P_CHUU,// /*特殊敵[中型敵]*/ /*S P_ZA KO_YOKAI1*/ 					= /*0x0400*/SP_GROUP_BOSS,		/* [***090325 */
//	S P_BO SS,
/* ------------- */
/*	敵弾 */
	//dummy_SP_BULLET														= /*0x0800*/SP_GROUP_BULLETS,	/* ... und ihre Waffen */
/* ------------- */

//enum /*sprite_type*/
//{
/* ------------- */
/*	ポーズ中移動可能物 */
//	S P_ME NU_TEXT	/* 文字 */												= /*0x2000*/SP_GROUP_PAUSE_S P_ME NU_TEXT,
/*	その他 */
//	SP_MUTEKI																= /*0x4000*/SP_GROUP_ETC,		/* diverses */
//};

#define SP_MUTEKI	SP_GROUP_ETC

/* === Gu敵面 === */		/* --- まだ作ってない --- */
enum	/* ☆ 魔方陣面エフェクト */
{
	TEKI_00_BOSS11			/* ボス & 中ボス[右側1] */						= /*0x0400*/SP_GROUP_TEKI,
	TEKI_01_BOSS12, 		/* ボス & 中ボス[右側2] */
	TEKI_02_BOSS13, 		/* ボス & 中ボス[右側3] */
	TEKI_03_BOSS14, 		/* ボス & 中ボス[右側4] */
	TEKI_04_BOSS21, 		/* ボス & 中ボス[左側1] */
	TEKI_05_BOSS22, 		/* ボス & 中ボス[左側2] */
	TEKI_06_BOSS23, 		/* ボス & 中ボス[左側3] */
	TEKI_07_BOSS24, 		/* ボス & 中ボス[左側4] */
//
	TEKI_08_BOSS31, 		/* ボス & 中ボス[中心1] */
	TEKI_09_BOSS32, 		/* ボス & 中ボス[中心2] */
	TEKI_10_BOSS33, 		/* ボス & 中ボス[中心3] */
	TEKI_11_BOSS34, 		/* ボス & 中ボス[中心4] */
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

enum	/* ☆ 魔方陣面エフェクト */
{
	MAHOU_JIN_00_aaa	= /*0x00010000*/SP_GROUP_MAHOU_JIN,
	MAHOU_JIN_01_bbb,
	MAHOU_JIN_02_,
	MAHOU_JIN_03_,
	MAHOU_JIN_04_,
	MAHOU_JIN_05_,
	MAHOU_JIN_06_,
	MAHOU_JIN_07_,
};

/* === Guフロント面 === */		/* --- まだ作ってない --- */

enum	/* ☆ フロント面エフェクト */
{	/* 自分のあたり判定位置表示用コア(フロント面エフェクト) */
	JIKI_CORE_00_REIMU_A	= /*0x4000*/SP_GROUP_ETC,
	JIKI_CORE_01_REIMU_B,
	JIKI_CORE_02_MARISA_A,
	JIKI_CORE_03_MARISA_B,
	JIKI_CORE_04_MARISA_C,
	JIKI_CORE_05_REMILIA,
	JIKI_CORE_06_CHIRNO,
	JIKI_CORE_07_YUYUKO,
//	/* CORE隠す用 */
	CORE_HIDE_10_REIMU_A,
	CORE_HIDE_11_REIMU_B,
	CORE_HIDE_12_MARISA_A,
	CORE_HIDE_13_MARISA_B,
	CORE_HIDE_14_MARISA_C,
	CORE_HIDE_15_REMILIA,
	CORE_HIDE_16_CHIRNO,
	CORE_HIDE_17_YUYUKO,
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
	SP_GROUP_ETC_DUMMY_REMILIA,
	SP_GROUP_ETC_DUMMY_SLOW_BOMB,
//	SPELL_LOGO_,/* Border Power of Spiritual */
	SPELL_TACHIE_,
};
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
	JIKI_BOMBER_01, 		/* 誘導ボム(低速ボム)	  */
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
	JIKI_SHOT_00,			/* 赤札C /	/  / 長炎3 /  */
	JIKI_SHOT_01,			/* 赤札B /	/  / 長炎4 /  */
	JIKI_SHOT_02,			/* 赤札A / 黄星 / 小炎 / 氷 / 青蝶 */
	JIKI_SHOT_03,			/* / こうもり弾1 / 黄札1 */
	JIKI_SHOT_04,			/* / こうもり弾2 / 黄札2 */
	JIKI_SHOT_05,			/* / こうもり弾3 / 黄札3 */
	JIKI_SHOT_06,			/* / こうもり弾4 / 黄札4 */
	JIKI_SHOT_07,			/* / こうもり弾5 / 針弾 / 森弾 / ウェイブ弾 / ピンク蝶弾 */

};

/* === Gu弾幕面 === */
/*haisi_BULLET_MINI8_00_AKA */
//	haisi_BULLET_MINI8_03_AKA,
//	haisi_BULLET_MINI8_04_KIIRO,
//	haisi_BULLET_MINI8_05_AOI,
//	haisi_BULLET_MINI8_06_AOI,
//	haisi_BULLET_MINI8_07_YUKARI,
//	haisi_BULLET_MINI8_01_AKA,
//	haisi_BULLET_MINI8_02_KIIRO,

enum
{
//1
	BULLET_00_HOSI_TEN			= SP_GROUP_BULLETS/*0x0800*/,
	BULLET_KOME_01_AOI,
	BULLET_KOME_02_AKA,
	BULLET_KOME_03_YUKARI,
	BULLET_KOME_04_MIDORI,
	BULLET_KOME_05_KIIRO,
	BULLET_KOME_06_dummy,
	BULLET_KOME_07_dummy,
//2
	BULLET_MARU8_00_AKA,
	BULLET_MARU8_01_YUKARI,
	BULLET_MARU8_02_AOI,
	BULLET_MARU8_03_MIDORI,
	BULLET_MARU8_04_MIDORI,
	BULLET_MARU8_05_MIDORI,
	BULLET_MARU8_06_KIIRO,
	BULLET_MARU8_07_AOI,
//3
	BULLET_MARU12_00_SIRO,		/*12*/
	BULLET_MARU12_01_AKA,		/*12*/
	BULLET_MARU12_02_AOI,		/*12*/
	BULLET_MARU12_03_MIDORI,	/*12*/
	BULLET_MARU12_04_KIIRO, 	/*12*/	//	BULLET_MARU8_12_dummy,
	BULLET_MARU12_05_DAIDAI,	/*12*/	//	BULLET_MARU8_13_dummy,
	BULLET_CAP16_04_KOME_SIROI, 	/* [青白米弾] */
	BULLET_CAP16_05_TUTU_SIROI, 	/* [青白実包弾] */
//4
	BULLET_MARU10_00_FUKA_MIDORI,	/* 深緑色*/
	BULLET_MARU10_01_KI_MIDORI, 	/* 黄緑色*/
	BULLET_MARU10_02_KIIRO, 		/* 黄色*/
	BULLET_MARU10_03_AOI,			/* 青*/
	BULLET_MARU8_08_dummy,
	BULLET_MARU8_09_dummy,
	BULLET_MARU8_10_dummy,
	BULLET_MARU8_11_dummy,
//5
	BULLET_UROKO14_00_AOI,
	BULLET_UROKO14_01_AKA,
	BULLET_UROKO14_02_YUKARI,
	BULLET_UROKO14_03_MIDORI,
	BULLET_UROKO14_04_MIZUIRO,
	BULLET_UROKO14_05_KIIRO,
	BULLET_UROKO14_06_dummy,
	BULLET_UROKO14_07_dummy,
//6
	BULLET_KUNAI12_00_AOI,
	BULLET_KUNAI12_01_AKA,
	BULLET_KUNAI12_02_MIDORI,
	BULLET_KUNAI12_03_MIDORI,
	BULLET_KUNAI12_04_YUKARI,
	BULLET_KUNAI12_05_AOI,
	BULLET_KUNAI12_06_KIN,
	BULLET_KUNAI12_07_KIN,
//7
	BULLET_OFUDA12_00_AOI,
	BULLET_OFUDA12_01_AKA,
	BULLET_OFUDA12_02_YUKARI,
	BULLET_OFUDA12_03_MIDORI,
	BULLET_KNIFE20_04_AOI,
	BULLET_KNIFE20_05_AKA,
	BULLET_KNIFE20_06_YUKARI,
	BULLET_KNIFE20_07_MIDORI,
//8
	BULLET_HARI32_00_AOI,
	BULLET_HARI32_01_AKA,
	BULLET_HARI32_02_KIIRO,
	BULLET_HARI32_03_DAIDAI,
	BULLET_OODAMA32_00_SIROI,
	BULLET_OODAMA32_01_AOI,
	BULLET_OODAMA32_02_AKA,
	BULLET_OODAMA32_03_YUKARI,
};
//	BULLET_OODAMA32_04_MIDORI,
//	BULLET_OODAMA32_05_AOI,
//	BULLET_OODAMA32_06_KIIRO,
//	BULLET_OODAMA32_07_PINK,

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



/*-------------------*/
/* 針弾とかあるから、現在図形判定しないと詐欺判定になる。 */
/* あたり判定無くしても速度は変わらないがメモリが節約できるので将来的にはこの辺も改良したい。 */
//#define USE_ZUKEI_ATARI_HANTEI (1)
//#define USE_ZUKEI_ATARI_HANTEI (0)

typedef struct /*_point256*/
{
	Uint16 x;	/*	union POINT */	/* x 位置 */
	Uint16 y;	/*	union POINT */	/* y 位置 */
} POINT_u16;
/* u16座標 */

typedef struct /*_point256*/
{
	int x256;	/*	union POINT */	/* x表示位置  (256固定小数点形式) / akt. Position */
	int y256;	/*	union POINT */	/* y表示位置  (256固定小数点形式) / akt. Position */
} POINT256;
/* t256座標 */

typedef struct /*_point_vector256*/
{
	int x256;	/*	union POINT */	/* x表示位置  (256固定小数点形式) / akt. Position */
	int y256;	/*	union POINT */	/* y表示位置  (256固定小数点形式) / akt. Position */
	int vx256;	/*	union POINT_VECTOR */	/* 共用regist_vector(); */	/*union BULLET_VECTOR_DATA*/
	int vy256;	/*	union POINT_VECTOR */	/* 共用regist_vector(); */	/*union BULLET_VECTOR_DATA*/
} POINT_VECTOR256;
/* t256座標 + t256ベクトル */


#if 1
// union 共用
	#define base_hp 					user00
	#define base_weapon_strength		user00
	#define base_time_out				user00/*bakuhatsu*/
	#define base_score					user01
	//#define base_state					user01
#endif
//
/* スプライトの使い方

分類		自機		自弾					敵					敵弾			爆発
user00		--			base_weapon_strength	base_hp 			base_time_out	base_time_out
			--			武器強さ				体力				時間			時間
user01		--			--						base_score			--				--
			--			--						スコア				--				--
2018897 2018881
*/
//

/*
	x256、y256は「Guの場合は中心座標」。
	「SDLの場合は左隅座標」です。
	従って SDL_w128、SDL_h128 はSDLでしか使いません。
	狙い弾、等の当たり判定ではこれらを使いません。
*/

#define tmp_angleCCW65536 tmp_angleCCW1024
typedef struct _sprite
{
	int x256;						/*	union POINT */			/* x表示位置  (256固定小数点形式) / akt. Position */
	int y256;						/*	union POINT */			/* y表示位置  (256固定小数点形式) / akt. Position */
	int vx256;						/*	union POINT_VECTOR */	/* 共用regist_vector(); */	/*union BULLET_VECTOR_DATA*/
	int vy256;						/*	union POINT_VECTOR */	/* 共用regist_vector(); */	/*union BULLET_VECTOR_DATA*/
//[4]
	int m_angleCCW1024; 			/* 描画用角度(下が0度で左回り(反時計回り), 一周は1024分割, 0-1023度) */
	int tmp_angleCCW1024;			/* 保持用角度(下が0度で左回り(反時計回り), 一周は1024分割, 0-1023度) */
	int user00; 					/* user00 ENEMY_BASE base; */	//int health;	int time_out;		/* user03 */
	int user01; 					/* user01 ENEMY_BASE base; */	//int score;	int state;			/* user02 */
//[8]
	int type;						/* 種類及び使用可否 / Sprite-Type, (siehe enum SPRITE_TYPE), 0 = remove. */
	u32 color32;					/* Gu color AGBR8888 MAKE32RGBA(RED, GREEN, BLUE, ALPHA)で各要素 0 から 255 (0x00 から 0xff)まで。 */
	int m_Hit256R;					/* あたり判定用 */
	int m_zoom_x256;				/* 表示拡大率 */
//[12]
//
//
//[16]
	/* とりあえず移行用(1)従来互換(弾) */
	int user_data00;
	int user_data01;
	int user_data02;
	int user_data03;
//
	int user_data04;
	int user_data05;
	int user_data06;
	int user_data07;
//
	/* とりあえず移行用(2)追加弾、基本機能 */
	int user_data10;
	int user_data11;
	int user_data12;
	int user_data13;
//
	void (*callback_loser)(struct _sprite *src);	/* Custom-loser-Routine (やられた後にボーナスを出すとか、違う爆発するとか) / 自機オプションのアニメーションコールバック */
	void (*callback_mover)(struct _sprite *src);	/* Custom-Move-Routine */
	void (*callback_hit_enemy)(struct _sprite *c, struct _sprite *src); 	// [***090325		追加
//[20]
	struct _sprite *next;				/* 移行したらなくなる。 */
	struct _sprite *target_obj; 		/* とりあえず移行用(3)移行したらなくなる。 */
	/*u8*/u32 flags;					/* フラグセット / siehe unten (SP_FLAG...) */
} SPRITE;
/*	struct _sprite *target_obj;
	用途は(
		player用誘導弾、
		後ろの魔方陣"妖怪2"(赤)
	)
	ほんとは複雑にキャストすれば要らないんだけどバグでそうだし。
 */


/*---------------------------------------------------------
	life_cycle sprite_time_out sprite_longevity
	-------------------------------------------------------
	名前は忘れたけど、KETMにあった考え方。
	一度は排除したが、これをもう一度復活させる。
	ただし、システムで強制的に値を加算する。
	スプライトが生成されると、値が 1。
	スプライトをフレーム処理する度に、1 加算される。
	-------------------------------------------------------
	pspは 0 レジスタがあるので、0 未満、
	つまり負数は 自動消去。
	-------------------------------------------------------
	int じゃなくて short にする案。
	0x7fff==32767 は正数だが、1加算すると signed short の場合、
	0x8000==32768==-32768==負数になるので、負数になったら自動消去される。
---------------------------------------------------------*/

/*---------------------------------------------------------
	リソース resource
---------------------------------------------------------*/

extern SPRITE *sprite_add_gu_error(void);				/* gu移行中、汎用スプライト 		int bullet_type_num */
extern SPRITE *sprite_add_only_bullet_error(void);		/* gu移行中、弾専用 				int bullet_type_num */

extern void sprite_remove_all_SDL_void(void);			/*int type*/ /*gu汎用(旧SDL)*/
extern void sprite_remove_all_444_void(void);			/*int type*/ /*弾幕専用*/

extern void sprite_move_main_SDL_222(void); 			/*int type*/ /*gu汎用(旧SDL)*/
extern void sprite_move_main_Gu_444(void);				/*int type*/ /*gu弾幕専用*/

//extern void sprite_display000(int type);/**/
//extern void sprite_display222(int type);/*弾幕用*/
extern void pause_sprite_display(void);/**/

extern SPRITE *sprite_collision_check_SDL_teki(SPRITE *tocheck);/*敵専用*/	//, int type);
extern SPRITE *sprite_collision_check_444(SPRITE *tocheck, int type);/*gu弾幕専用*/

extern void gamen_gai_nara_zako_osimai(SPRITE *src);/* 画面外ならおしまい */
extern void check_boss_option_time_out(SPRITE *src);/* */

extern void bullet_angle_all_hamidasi_check(void);/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/


/* 使用中であるが退避したい場合。画面外にスプライトを移動させ、無効にする。 */
extern void sprite_initialize_position(SPRITE *src);

//#if 1
//	/* ソフトウェアーで Zソート */
//	#define USE_ZBUFFER 	(0)
//#else
//	/* ハードウェアーでソート */
//	#define USE_ZBUFFER 	(1)
//#endif
//
//#if 1
//	/* 単純拡大 */
//	#define USE_ZOOM_XY 	(0)
//#else
//	/* 縦横拡大 */
//	#define USE_ZOOM_XY 	(1)
//#endif


//#ifndef GRP_SCREEN_H
//#define GRP_SCREEN_H
/*	@since		Jul.27.2005 GRP_SCREEN_H		画面表示管理 */
/* --- Guスプライトの最大表示数 */
#define MAX_TRANS_GU_SPRITE 	(1024)
//#define MAX_TRANS_GU_SPRITE	(256)
//#define MAX_TRANS_GU_SPRITE	(512)



typedef struct
{
	UINT8 used;
	UINT8 dummy_bullet_num; 	/* 敵弾の番号 */			//	UINT8 dummy_alpha;
	UINT8 dummy_harf_wide;		/* (1/2)幅[ドット] */		//	UINT8 texture_id;
	UINT8 dummy_harf_height;	/* (1/2)高さ[ドット] */ 	//	UINT8 center;	/*dummy*/
	int rotation_1024z;
	int rotation_tmp;
	u32 color8888;//	int color8888;
//
	/* - public */
	int x256;
	int y256;
	int w;
	int h;
//
	int tx;
	int ty;
	#if (1==USE_ZOOM_XY)
	int zoom_x256;
	int zoom_y256;
	#else //(0==USE_ZOOM_XY)
	int zoom_xy256;
	#endif/* (1==USE_ZOOM_XY) */
	#if (1==USE_ZBUFFER)
	int priority;		/* 表示優先順位 */	/* unsigned short */
	#endif/* (1==USE_ZBUFFER) */
	int aaa;
//	Image *Texture;//
	//SDL_Surface *Texture_ptr;
} TGameSprite /*, *PTGameSprite*/;

enum /*_common_my_obj_*/
{
	MY_OBJ_00_JIKI = 0,
	MY_OBJ_01_JIKI_MARU,
	MY_OBJ_02_JIKI_OPT01,
	MY_OBJ_03_JIKI_OPT02,
	MY_OBJ_04_JIKI_OPT03,
	MY_OBJ_05_JIKI_OPT04,
	MY_OBJ_06_SEND1,
	MY_OBJ_07_,
//
	MY_OBJ_08_BOSS,
	MY_OBJ_09_,
	MY_OBJ_0a_,
	MY_OBJ_0b_EFFECT01,
	MY_OBJ_0c_EFFECT02,
	MY_OBJ_0d_EFFECT03,
	MY_OBJ_0e_EFFECT04,
	MY_OBJ_0f_EFFECT05,
//
	MY_OBJ_99_MAX
};

#endif/* _SPRITE_H_ */



#ifndef _BULLET_SYSTEM_H_
#define _BULLET_SYSTEM_H_

/*---------------------------------------------------------
	敵弾管理システム
---------------------------------------------------------*/

#if 000/* 移行中につき無効 */
/* MAX_POOL_BULLET: 2のn乗の必要あり(1024とか2048とか) */
#define MAX_POOL_BULLET 1024
extern TGameSprite bullet_pool[MAX_POOL_BULLET];

extern void bullet_system_init(void);
extern void bullet_system_exit(void);


#endif /* 000 移行中につき無効 */




#if 1
/*---------------------------------------------------------
	新、敵弾管理システム(仮)
---------------------------------------------------------*/
/*
 参考: kouma_tr013/東方紅魔郷/マニュアル/html/faq.html
Ｑ２３　アイテムの表示限界は？

　画面内５１２個までです。
　敵弾の数が６４０個なので、全てがボーナスアイテムになるとは限りませんが、
　殆どの場合はスコアパターンに影響するようなことはありません。

*/

//#define SPRITE_444POOL_MAX		(512)/* とりあえず */
#define SPRITE_444POOL_MAX		(1024)/* 弾登録プログラムの都合上 2^n である必要があります。(1024==(1<<10) ) */
extern SPRITE sprite_444pool[SPRITE_444POOL_MAX];	/* スプライトのリスト構造 */

#endif

#endif /* _BULLET_SYSTEM_H_ */
