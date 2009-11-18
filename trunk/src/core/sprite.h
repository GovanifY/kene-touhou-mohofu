#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "game_main.h"

/*---------------------------------------------------------
	スプライト マネージャ
--------------------------------------------------------- */

/* Sprites */
//#define MAX_SPRITE 200

/*---------------------------------------------------------
	リソース resource
--------------------------------------------------------- */
enum
{
	#if 0
	//TAMA_TYPE_KUGEL_PNG,
	//TAMA_TYPE_KUGEL_MINI2_PNG,
	TAMA_TYPE_BULLET_BEAM16_PNG,
	TAMA_TYPE_BULLET_MARU16_PNG,
//
	TAMA_TYPE_BULLET_MING32_PNG,
	TAMA_TYPE_BULLET_JIPPOU32_PNG,
//
	TAMA_TYPE_OODAMA_08_PNG,/* 大弾(黒青赤...)	黒玉(輪) PRIORITY_03_ENEMY は、あたり判定部分 */
//	TAMA_TYPE_OODAMA_00_PNG,/* 黒玉(輪)  あたり判定部分*/
//	TAMA_TYPE_OODAMA_01_PNG,/* 大弾(青) 表示部分 */
//	TAMA_TYPE_OODAMA_02_PNG,/* 大弾(赤) 表示部分*/
//
	TAMA_TYPE_BULLET_KNIFE01_PNG,/*垂直降下ナイフ(赤)*/
	TAMA_TYPE_BULLET_KNIFE18_PNG,/*全方向ナイフ(青)*/
	#else
	TAMA_ATARI_02,
	TAMA_ATARI_04,
	#endif
	TAMA_TYPE_HOMING16_PNG,
//
	JIKI_ATARI_ITEM_16,
	JIKI_ATARI_ITEM_80,/* 扇本体 */
//
	BASE_BOMBER_ATARI_0a,/*4*/
	BASE_BOMBER_ATARI_0b,/*16*/
	BASE_BOMBER_ATARI_0c,/*16*/
	BASE_BOMBER_ATARI_0d,/*32*/
	BASE_BOMBER_ATARI_0e,/*4*/
//
	BASE_SHOT_ATARI_0a,/*4*/
	BASE_SHOT_ATARI_0b,/*4*/
	BASE_SHOT_ATARI_0c,/*4*/
	BASE_SHOT_ATARI_0d,/*6*/
	BASE_SHOT_ATARI_0e,/*8*/
//
	BASE_NEEDLE_ATARI_0a,/*6*/
	BASE_NEEDLE_ATARI_0b,/*8*/
	BASE_NEEDLE_ATARI_0c,/*12*/
	BASE_NEEDLE_ATARI_0d,/*16*/
	BASE_NEEDLE_ATARI_0e,/*8*/
//
};
#if 0
	#define TAMA_TYPE_BULLET_DUMMY TAMA_TYPE_BULLET_MARU16_PNG
#else
	#define TAMA_TYPE_BULLET_DUMMY			TAMA_ATARI_02
	#define TAMA_TYPE_BULLET_BEAM16_PNG 	TAMA_ATARI_02
	#define TAMA_TYPE_BULLET_MARU16_PNG 	TAMA_ATARI_02
	#define TAMA_TYPE_BULLET_MING32_PNG 	TAMA_ATARI_02
	#define TAMA_TYPE_BULLET_JIPPOU32_PNG	TAMA_ATARI_02
	#define TAMA_TYPE_OODAMA_08_PNG 		TAMA_ATARI_04
	#define TAMA_TYPE_BULLET_KNIFE01_PNG	TAMA_ATARI_02
	#define TAMA_TYPE_BULLET_KNIFE18_PNG	TAMA_ATARI_02
#endif

enum
{
	/* ☆ BG面エフェクト */
	BASE_TIKEI_BGPANEL1_PNG = 0,
	BASE_TIKEI_BGPANEL2_PNG,
//	/* ☆ SHOT面エフェクト */
//
//	BASE_BOMBER_SLOW_PNG,
	BASE_MAHOUJIN_0_PNG,
//
	BASE_TIKEI_GROUNDER08_PNG,
//	/* ☆ ザコ面エフェクト */
//	/* ザコ(ザコ面エフェクト) */
	BASE_OBAKE08_PNG,
	BASE_YUKARI32_PNG,
	BASE_AKA_KEDAMA08_PNG,
	BASE_NIJI_KEDAMA16_PNG,
//
	BASE_MIDOORI_KEDAMA16_PNG,
	BASE_KEDAMA16_PNG,
	BASE_INYOU1_16_PNG,
	BASE_TATSUMAKI16_PNG,
//
	BASE_GREAT_FAIRY02_PNG,
	BASE_AKA_MEIDO08_PNG,
	BASE_AO_YOUSEI24_PNG,
//
//	/* ボス(ザコ面エフェクト) */
//	BASE_BOSS_ALICE_0_PNG,
//	BASE_BOSS_ALICE_1_PNG,
//	BASE_BOSS_ALICE_2_PNG,
//	BASE_BOSS_ALICE_3_PNG,
//	BASE_BOSS_ALICE_4_PNG,
	BASE_BOSS_ALICE_PNG,
	BASE_BOSS_ALICE_DOLLS_PNG,
	BASE_BOSS_PACHE_PNG,
	BASE_BOSS_PACHE_DOLLS_PNG,
//
	BASE_BOSS_AYA_PNG,
//
	BASE_BOSS_KAGUYA_PNG,
//	BASE_BOSS_KAGUYA_0_PNG,
//	BASE_BOSS_KAGUYA_1_PNG,
//	BASE_BOSS_KAGUYA_2_PNG,
//	BASE_BOSS_KAGUYA_3_PNG,
//	BASE_BOSS_KAGUYA_4_PNG,
//	BASE_BOSS_KAGUYA_5_PNG,
//
	BASE_BOSS_SAKUYA_PNG,
//
	//BASE_KEY_ICON_PNG,
//
	/* ☆ アイテム面エフェクト */
//	/* その他の自機素材(アイテム面エフェクト) */
	BASE_BONUS_ITEMS_PNG,
//
	/* ☆ 弾幕面エフェクト */
//
	/* ☆ フロント面エフェクト */
//	/* 自分のあたり判定位置表示用コア(フロント面エフェクト) */
	BASE_CORE_PNG,
	BASE_CORE_PNG_ma,
	BASE_CORE_PNG_oz,
	BASE_CORE_PNG_ci,
	BASE_CORE_PNG_yu,
//
	#if 000
//	/* 小爆発(フロント面エフェクト) */
	BASE_TR_BLUE_PNG,
	BASE_TR_RED_PNG,
	BASE_TR_GREEN_PNG,
//	/* ザコ消滅爆発(フロント面エフェクト) */
	BASE_BAKUHA05_PNG,
	BASE_BAKUHA06_PNG,
	BASE_BAKUHA07_PNG,
//	/* 火炎爆発(フロント面エフェクト) */
	BASE_BAKUHA_EX_PNG,
	#endif //000
	/* ボンバーテロップロゴ(フロント面エフェクト) */

	/* /[コンティニュー文字(bank00)/メニュー文字(bank01)/メニュー文字(bank02)] */

	FILE_RESOURCE_MAX/*最大数*/
};

// 弾幕について
// 弾幕(敵弾のみ自弾は含まない)はGuで描く事を計画中(というか妄想中)。
// その場合、SDL画面描画後にGuで描くので、
// 弾幕のプライオリティーは最前面になる。
//
// 紅魔郷を参考に見たが、自機よりもアイテムよりも
// 弾幕は前(つまりあとで纏めて描画している)だった
//
//多少順序は違うかもしれないが、多分本家はこんな感じ(想像)
// [背景関連]
// １．仮想画面(448x448)に3D背景をレンダリング(喰み出し描画)
// [自機関連]
// ２．仮想画面(448x448)に自弾をレンダリング(喰み出し描画)					(自弾は自機より下)
// ３．仮想画面(448x448)に自機オプションをレンダリング(喰み出し描画)		(自機／自機オプションはボス／敵より下)
// ４．仮想画面(448x448)に自機をレンダリング(喰み出し描画)					(自機／自機オプションはボス／敵より下)
// [敵関連]
// ５．仮想画面(448x448)に敵やボスをレンダリング(喰み出し描画)
// [アイテム関連]
// ６．仮想画面(448x448)にアイテムをレンダリング(喰み出し描画)				(アイテムは敵弾より下/アイテムは敵より上)
// ９．仮想画面(448x448)に漢字スコアなどをレンダリング(喰み出し描画)		(漢字スコアは敵弾より下(???))
// [敵弾関連]
// ７．仮想画面(448x448)にレーザー等ライン描画をレンダリング(喰み出し描画)
// ８．仮想画面(448x448)に弾幕をレンダリング(喰み出し描画)
// [イベント関連]
// ９．仮想画面(448x448)に符やボスメータなどをレンダリング(喰み出し描画)
// １０．仮想画面(448x448)にイベント画像をレンダリング(喰み出し描画)
// １１．仮想画面(448x448)にイベント文字をレンダリング(喰み出し描画)
// １２．仮想画面(448x448)の一部(384x448)を矩形に切り出して、本画面(640x480)に転送する。
/*	註：384==12x32、448==14x32なので都合よく高速転送可能。 */
/*	註：ここの転送で実質的にクリッピング処理ができる。 */
// １３．得点などの表示は始めから本画面(640x480)に描画し仮想画面(448x448)は使わない。
// １４．本画面(640x480)をVRAMに転送(たぶん)。
/*	註：仮想画面(448x448)は多分仮想画面(512x512)。こちらの方が色々処理が省略できて、都合がよい。 */
//かえいずかとか除いて星蓮船まで一貫してゲーム画面は(384x448)だよね？

/*New r23*/
enum /*_priority_*/
{
// [背景関連]
//	PR IORITY_00_,			/*Gu化まだ*/	//	P R_BACK0 = 0,			/* 演出用浮遊物 */	/* 地形３連パネル */	/* 地形パネル棒 */
	PRIORITY_01_SHOT = 0,	/*Gu化まだ?*/	//	PRIORITY_01_SHOTBG	P RIORITY_01_GROUNDER,		/*自弾/地上ザコ敵/魔方陣*/
// [自機関連]
	PRIORITY_02_PLAYER, 	/*Gu化途中*/	/* 自機 */
// [敵関連]
	PRIORITY_03_ENEMY,		/*Gu化まだ*/	/* ボス/ザコ敵 */
// [アイテム関連]
	PRIORITY_04_ITEM,		/*Gu化まだ*/	/* アイテム/漢字スコア */	//P R_PLAYER2,			/*当たり表示*/
// [敵弾関連]
	PRIORITY_05_BULLETS,	/*Gu化途中*/	/*敵弾*/	/*PRIORITY_03_ENEMY_WEAPON*/
// [Guフロント面(Guで半透明は最前面に無いと描画できないので新設した)]
/*	PRIORITY_06_FRONT,*/	/*Gu化途中*/	/* Guのみ(SDL側では元々無し) */
};
// プライオリティは Gu に移行した場合に、
// 半透明がある場合はハードウェアー処理できないので
// 結局ソフトウェアーで対応(Zソート)する事になります。
// (半透明無しの場合はdepth-buffer使えばハードウェアー処理できる。)
// (プライオリティは3Dとして考えた場合のZ軸、描画で拡大縮小等はしない)
// したがって、なるべく少なくした方が、「あとで」(速度面で)有利です。
// (リスト処理は遅いので、プライオリティ別のバッファをプライオリティ数用意すると、
// 同プライオリティーではソートする必要がないので、ソートが省略できる)


//	P R_BACK1,
//	P R_BACK2,

//
//	P R_TARGET,
//	P R_TMP,
// [テキスト表示]
//	PR IORITY_06_TEXT,		/*テキスト表示*/
//	P R_FRONT0,
//	P R_FRONT1,
//	P R_FRONT2

#define USER_BOMOUT_WAIT (30)

/* 使用済みだから消してくれフラグ */
#define SP_DELETE				(0)
/*	0 ならば判別処理が省略できる。 -1 だと一々判別処理が入る
	KETMでは判別しないとならない場所に判別処理が書いてない為、
	スプライトがおかしくなるバグが複数ヶ所にある。 */
/*
	ここは種類別ではなくて、機能別に分類してください。
*/
#define SP_GROUP_SHOT_ZAKO		(0x0000)/*0x0100*/
#define SP_GROUP_SHOT_SPECIAL	(0x0080)/*0x0100*/
//
#define SP_GROUP_JIKI_GET_ITEM	(0x0100)/*0x0100*/		/* === Guプレイヤー面 === */
#define SP_GROUP_PLAYER_CORE	(0x0200)/*0x0100*/
#define SP_GROUP_ZAKO			(0x0400)/*0x0400*/
#define SP_GROUP_BOSS			(0x0800)/*0x0400*/
//#define SP_GROUP_TEKI 		(0x0400)/*0x0400*/
//#define SP_GROUP_ZAKO 		(SP_GROUP_TEKI|0)
//#define SP_GROUP_BOSS 		(SP_GROUP_TEKI|SP_GROUP_SHOT_SPECIAL)
#define SP_GROUP_BULLETS		(0x1000)/*0x0800*/		/* === Gu弾幕面 === */
#define SP_GROUP_ITEMS			(0x2000)/*0x1000*/
#define SP_GROUP_PAUSE_OBJS 	(0x4000)/*0x2000*/
#define SP_GROUP_ETC			(0x8000)				/* === Guフロント面 === */
#define SP_GROUP_ALL_SDL_TYPE	(0xff00)
#define SP_GROUP_SUB_TYPE_256	(0x00ff)
#define SP_GROUP_SUB_TYPE_128	(0x007f)
/* 以下あたり判定なし */
#define SP_GROUP_MAHOU_JIN			(0x00010000)			/* === Gu魔方陣面 === */
#define SP_GROUP_PANEL				(0x00020000)			/* === Guパネル面 === */
#define SP_GROUP_ALL_GAME_OBJS		(0x00ffff00)			/* ゲームコア終了時にかたづけるobj */
//#define SP_GROUP_SHOTS		0x0200
//#define SP_GROUP_ETC			0x4000
//#define SP_GROUP_ENEMYS		0x0200/*0x0400 */
/* 共用 */
//#define SP_GROUP_TEXTS			(SP_GROUP_PLAYER)/*0x2000 */

/* SP_GROUP_ETC == SP_GROUP_FRONT */

enum /*sprite_type*/
{
/*	自分A */
//	S P_PLAYER_ITEM/*プレイヤーの本体(あたり判定がアイテム取得)*/	= /*0x0100*/SP_GROUP_JIKI_GET_ITEM, /* Unser Held */
/*	自弾 */
//	S P_SHOT_BOSS,/*プレイヤーのボスに有効弾*/					//	= /*0x0200*/SP_GROUP_SHOTS, 	/* Waffen des Helden */
//	S P_SHOT_ZAKO,/*プレイヤーのボスに無効弾*/
/*	自分B */
	SP_PLAYER_CORE/*プレイヤーのコア(あたり判定が死亡判定)*/		= /*0x0100*/SP_GROUP_PLAYER_CORE,
/* ------------- */
/*	ザコ敵 (ボム中、体当たりでプレイヤー死なない)  */
	SP_ZAKO 														= /*0x0400*/SP_GROUP_ZAKO,		/* Die Boesen */
/* ------------- */
/*	ボス敵 / 中ザコ敵 (ボム中、体当たりでプレイヤー死ぬ)  */
	SP_CHUU /*特殊敵[中型敵]*/ /*SP_ZAKO_YOKAI1*/					= /*0x0400*/SP_GROUP_BOSS,		/* [***090325 */
	SP_BOSS,
/* ------------- */
/*	敵弾 */
	SP_BULLET														= /*0x0800*/SP_GROUP_BULLETS,	/* ... und ihre Waffen */
/* ------------- */
/*	アイテム */
	SP_ITEM_00_P001 												= /*0x1000*/SP_GROUP_ITEMS, 	/* Bonus items */
	SP_ITEM_01_P008,	//←ウェポンアイテム(中)	//ウェポンアイテム(強)→SP_ITEM_EXTRA_SHIELD,
	SP_ITEM_02_BOMB,	//→これらは低速ボムに吸収された。SP_ITEM_EXTRA_HOMING, SP_ITEM_EXTRA_HLASER,
	SP_ITEM_03_1UP,
	SP_ITEM_04_P128,
	SP_ITEM_05_HOSI,
	SP_ITEM_06_TENSU,
	SP_ITEM_07_SPECIAL,
/* ------------- */
/*	ポーズ中移動可能物 */
	SP_MENU_TEXT	/* 文字 */										= /*0x2000*/SP_GROUP_PAUSE_OBJS,
/*	その他 */
	SP_MUTEKI														= /*0x4000*/SP_GROUP_ETC,		/* diverses */
};


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
	JIKI_CORE_00_REIMU	= /*0x4000*/SP_GROUP_ETC,
	JIKI_CORE_01_MARISA,
	JIKI_CORE_02_,
	JIKI_CORE_03_,
	JIKI_CORE_04_,
	JIKI_CORE_05_,
	JIKI_CORE_06_,
	JIKI_CORE_07_,
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
	SPELL_LOGO_,/* Border Power of Spiritual */
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
enum
{
//	プレイヤーアニメーション
//	左へ移動のアニメーション	[中心→左側4→左側3→左側2→左側1→左側1→左側1→...]
	JIKI_PLAYER_00_LEFT 	/* 左側1 */ 	/* 最も左 */			= SP_GROUP_JIKI_GET_ITEM/*0x0800*/,
	JIKI_PLAYER_01_LEFT,	/* 左側2 */
	JIKI_PLAYER_02_LEFT,	/* 左側3 */
	JIKI_PLAYER_03_LEFT,	/* 左側4 */ 	/* 中より左 */
//	繰り返しアニメーション		[中心1→中心2→中心3→中心4→中心1→中心2→中心3→中心4→...]
	JIKI_PLAYER_04_MID, 	/* 中心1 */
	JIKI_PLAYER_05_MID, 	/* 中心2 */
	JIKI_PLAYER_06_MID, 	/* 中心3 */
	JIKI_PLAYER_07_MID, 	/* 中心4 */
//	右へ移動のアニメーション	[中心→右側1→右側2→右側3→右側4→右側4→右側4→...]
	JIKI_PLAYER_08_RIGHT,	/* 右側1 */ 	/* 中より右 */
	JIKI_PLAYER_09_RIGHT,	/* 右側2 */
	JIKI_PLAYER_10_RIGHT,	/* 右側3 */
	JIKI_PLAYER_11_RIGHT,	/* 右側4 */ 	/* 最も右 */
	JIKI_ETC_00,			/* レーザー1 */
	JIKI_ETC_01,			/* レーザー2 */
	JIKI_ETC_02,			/* レーザー3 */
	JIKI_ETC_03,			/* レーザー4 */
//
	JIKI_BOMBER_00, 		/* レーザー5 */
	JIKI_BOMBER_01, 		/* レーザー6 */
	JIKI_BOMBER_02, 		/* 星1 / 蝶1 / 十字炎1 */
	JIKI_BOMBER_03, 		/* 星2 / 蝶2 / 十字炎2 */
	JIKI_BOMBER_04, 		/* 星3 / 蝶3 / 十字炎3 */
	JIKI_BOMBER_05, 		/* 星4 / 蝶4 / 十字炎4 */
	JIKI_BOMBER_06, 		/* 星5 / 蝶5 / 長炎1 / 結界白 */
	JIKI_BOMBER_07, 		/* 星6 / 蝶6 / 長炎2 / 結界黄  */
//
	JIKI_SHOT_00,			/* 長炎3 / 赤札C */
	JIKI_SHOT_01,			/* 長炎4 / 赤札B */
	JIKI_SHOT_02,			/* 赤札A / 黄星 / 小炎 / 氷 / 青蝶 */
	JIKI_SHOT_03,			/* こうもり弾1 / 黄札1 */
	JIKI_SHOT_04,			/* こうもり弾2 / 黄札2 */
	JIKI_SHOT_05,			/* こうもり弾3 / 黄札3 */
	JIKI_SHOT_06,			/* こうもり弾4 / 黄札4 */
	JIKI_SHOT_07,			/* こうもり弾5 / 針弾 / 森弾 / ウェイブ弾 / ピンク蝶弾 */
//
	JIKI_OPTION_00_00,
	JIKI_OPTION_00_01,
	JIKI_OPTION_00_02,
	JIKI_OPTION_00_03,
	JIKI_OPTION_00_04,
	JIKI_OPTION_00_05,
	JIKI_OPTION_00_06,
	JIKI_OPTION_00_07,
//
	JIKI_OPTION_01_00,
	JIKI_OPTION_01_01,
	JIKI_OPTION_01_02,
	JIKI_OPTION_01_03,
	JIKI_OPTION_01_04,
	JIKI_OPTION_01_05,
	JIKI_OPTION_01_06,
	JIKI_OPTION_01_07,
//
	JIKI_OPTION_02_00,
	JIKI_OPTION_02_01,
	JIKI_OPTION_02_02,
	JIKI_OPTION_02_03,
	JIKI_OPTION_02_04,
	JIKI_OPTION_02_05,
	JIKI_OPTION_02_06,
	JIKI_OPTION_02_07,
//
	JIKI_OPTION_03_00,
	JIKI_OPTION_03_01,
	JIKI_OPTION_03_02,
	JIKI_OPTION_03_03,
	JIKI_OPTION_03_04,
	JIKI_OPTION_03_05,
	JIKI_OPTION_03_06,
	JIKI_OPTION_03_07,
};

/* === Gu弾幕面 === */
enum
{
	BULLET_MARU8_00_AKA 				= SP_GROUP_BULLETS/*0x0800*/,
	BULLET_MARU8_01_YUKARI,
	BULLET_MARU8_02_AOI,
	BULLET_MARU8_03_MIDORI,
	BULLET_MARU8_04_MIDORI,
	BULLET_MARU8_05_MIDORI,
	BULLET_MARU8_06_KIIRO,
	BULLET_MARU8_07_AOI,
//
	BULLET_MINI8_00_AKA,
	BULLET_MINI8_01_AKA,
	BULLET_MINI8_02_KIIRO,
	BULLET_MINI8_03_AKA,
	BULLET_MINI8_04_KIIRO,
	BULLET_MINI8_05_AOI,
	BULLET_MINI8_06_AOI,
	BULLET_MINI8_07_YUKARI,
//
	BULLET_MARU12_00_SIRO,	/*12*/
	BULLET_MARU12_01_AKA,	/*12*/
	BULLET_MARU12_02_MIDORI,/*12*/
	BULLET_MARU12_03_AOI,	/*12*/
	BULLET_MARU10_00_AOI,
	BULLET_MARU10_01_MIDORI,
	BULLET_MARU10_02_MIDORI,
	BULLET_MARU10_03_MIDORI,
//4
	BULLET_HARI32_00_AOI,
	BULLET_HARI32_01_AKA,
	BULLET_HARI32_02_KIIRO,
	BULLET_HARI32_03_DAIDAI,
	BULLET_KNIFE20_04_AOI,
	BULLET_KNIFE20_05_AKA,
	BULLET_KNIFE20_06_YUKARI,
	BULLET_KNIFE20_07_MIDORI,
//5
	BULLET_UROKO14_00_AOI,
	BULLET_UROKO14_01_AKA,
	BULLET_UROKO14_02_YUKARI,
	BULLET_UROKO14_03_MIDORI,
	BULLET_UROKO14_04_MIZUIRO,
	BULLET_UROKO14_05_KIIRO,
	BULLET_CAP16_04_SIROI,
	BULLET_CAP16_05_SIROI,
//6
	BULLET_KOME_01_AOI,
	BULLET_KOME_02_AKA,
	BULLET_KOME_03_YUKARI,
	BULLET_KOME_04_MIDORI,
	BULLET_KOME_05_KIIRO,
	BULLET_KUNAI12_06_AOI,
	BULLET_KUNAI12_00_AKA,
	BULLET_KUNAI12_07_MIDORI,
//7
	BULLET_KUNAI12_01_AKA,
	BULLET_KUNAI12_02_MIDORI,
	BULLET_KUNAI12_03_YUKARI,
	BULLET_KUNAI12_04_AOI,
	BULLET_OFUDA12_00_AOI,
	BULLET_OFUDA12_01_AKA,
	BULLET_OFUDA12_02_YUKARI,
	BULLET_OFUDA12_03_MIDORI,
//8
	BULLET_OODAMA32_00_SIROI,
	BULLET_OODAMA32_01_AOI,
	BULLET_OODAMA32_02_AKA,
	BULLET_OODAMA32_03_YUKARI,
	BULLET_OODAMA32_04_MIDORI,
	BULLET_OODAMA32_05_AOI,
	BULLET_OODAMA32_06_KIIRO,
	BULLET_OODAMA32_07_PINK,
};


/* 針弾とかあるから、現在図形判定しないと詐欺判定になる。 */
/* あたり判定無くしても速度は変わらないがメモリが節約できるので将来的にはこの辺も改良したい。 */
//#define USE_ZUKEI_ATARI_HANTEI (1)
//#define USE_ZUKEI_ATARI_HANTEI (0)

typedef struct /*_point256*/
{
	int x256;	/*	union POINT */	/* x表示位置  (256固定小数点形式) / akt. Position */
	int y256;	/*	union POINT */	/* y表示位置  (256固定小数点形式) / akt. Position */
} POINT256;

typedef struct /*_point_vector256*/
{
	int x256;	/*	union POINT */	/* x表示位置  (256固定小数点形式) / akt. Position */
	int y256;	/*	union POINT */	/* y表示位置  (256固定小数点形式) / akt. Position */
	int vx256;	/*	union POINT_VECTOR */	/* 共用regist_vector(); */	/*union BULLET_VECTOR_DATA*/
	int vy256;	/*	union POINT_VECTOR */	/* 共用regist_vector(); */	/*union BULLET_VECTOR_DATA*/
} POINT_VECTOR256;


#if 1
// union 共用
	#define base_health 				user00
	#define base_weapon_strength		user00
	#define base_time_out				user00/*bakuhatsu*/
	#define base_score					user01
	//#define base_state					user01
#endif
//
typedef struct _sprite
{
	int x256;			/*	union POINT */			/* x表示位置  (256固定小数点形式) / akt. Position */
	int y256;			/*	union POINT */			/* y表示位置  (256固定小数点形式) / akt. Position */
	int vx256;			/*	union POINT_VECTOR */	/* 共用regist_vector(); */	/*union BULLET_VECTOR_DATA*/
	int vy256;			/*	union POINT_VECTOR */	/* 共用regist_vector(); */	/*union BULLET_VECTOR_DATA*/
//[4]
	int w128;						/* 横幅の半分 (256固定小数点形式) / Breite, He */
	int h128;						/* 高さの半分 (256固定小数点形式) / Breite, He */
//
	int m_angleCCW512;				/* 描画用角度(下が0度で左回り(反時計回り)) */
	int tmp_angleCCW512;			/* 保持用角度(下が0度で左回り(反時計回り)) */
//[8]
	int type;						/* 種類及び使用可否 / Sprite-Type, (siehe enum SPRITE_TYPE), 0 = remove. */
	SDL_Surface *sprite_bmp;		/* 画像 / Images */
	int m_Hit256R;					/* あたり判定用 */
	int m_zoom_x256;				/* 表示拡大率 */
//[12]
	int user00; 		/* user00 ENEMY_BASE base; */	//int health;	int time_out;		/* user03 */
	int user01; 		/* user01 ENEMY_BASE base; */	//int score;	int state;			/* user02 */
	//int timeover_ticks;			/* 作成してからの経過時間 (現在KETM自体にバグがある為、一定時間経過すると強制消去する) / number of ticks since creation */
//
	u32 color32;					/* Gu color AGBR8888 */
//
	struct _sprite *next;
//[16]
	void *data; 					/* Zeiger auf Strukur mit zus舩zlichen Data. */
	void (*callback_loser)(struct _sprite *src);/* Custom-loser-Routine (やられた後にボーナスを出すとか、違う爆発するとか) */
	void (*callback_mover)(struct _sprite *src);/* Custom-Move-Routine */
	void (*callback_hit_enemy)(struct _sprite *c, struct _sprite *src); 	// [***090325		追加
//[20]
	Uint8 flags;					/* フラグセット / siehe unten (SP_FLAG...) */
	Uint8 priority; 				/* 表示優先順位 / Priority for drawing. (0: unten, 9 oben) */
//
	Uint8 yx_anim_frame;			/* / アニメパターンの番号(何コマ目か) */	 /*int*/			/* action. Frame */
	Uint8 yx_frames;				/* / 表示パターン総数 / Anzahl Frames */
	Uint8 anim_count;				/*int*/ 	// Sint8 anim_count;			/* intern: Zaehler f. anim_speed */
	Uint8 anim_speed/* 256 */;		/*逆転アニメ禁止に変更*/		/* アニメーション速度 / Geschw. der Animation */	 /*Sint8*/ /*Uint8*/
//
//[16]
} SPRITE;
//	Uint8 dummy_alpha;					/* 透明度	 / 255: opak, -  0: durchsichtig */
//	Uint8 dummy_alpha_chache;			/* 前回のアルファ値 */

#define YX_FRAME_LOW_X	(0x0f)
#define YX_FRAME_HIGH_Y (0xf0)
#define anim_frame yx_anim_frame


#define SP_FLAG_COLISION_CHECK	0x01	/* Col-Check aktiv */
#define SP_FLAG_VISIBLE 		0x02	/* Sprite sichtbar */
#define SP_FLAG_NOT_CACHE		0x04	/* Image nicht im Cache */
#define SP_FLAG_FREE_SURFACE	0x08	/* Surface loeschen */
#define SP_FLAG_GRAZE			0x10	/* グレイズ済みかとうかのフラグ */
//#define SP_FLAG_CHEAP_ALPHA	0x10	/* "Cheapalpha", nur jedes 2te Pixel setzen */
#define SP_FLAG_LOCK_TARGET 	0x20	/* SIE sind hinter mir her */

#define SP_FLAG_TIME_OVER		0x80	/* 一定時間での自動消去、許可フラグ(ONなら自動消去する) */



/*---------------------------------------------------------
	リソース resource
---------------------------------------------------------*/

typedef struct
{
	const char *file_name;				/* 素材のファイル名 */
	Uint8 use_alpha;					/* アルファ(画像透明度)使用可否 */
	Uint8 total_frames_dummy_resurved;	/* 現在ダミー */
	Uint8 x_divide_frames_m1;			/* (横分割数-1) */
	Uint8 y_divide_frames_m1;			/* (縦分割数-1) */
//
	Uint8 priority; 					/* 描画プライオリティー(表示優先度) */
	Uint8 anime_speed;					/* アニメーション速度 */
	Uint8 atari_hankei; 				/* あたり判定円の半径(あたり判定の大きさ) */
	Uint8 aaa_dummy_resurved;			/* 現在ダミー */
} IMAGE_RESOURCE;

//#define iyx(aaa,bbb) (((aaa-1)<<4)+(bbb-1)),1
#define iyx(aaa,bbb) (aaa-1),(bbb-1)



//extern SPRITE *spr ite_add_file 0(char *filename, int frames, Uint8 priority, int use_alpha);
//extern SPRITE *spr ite_add_file(char *filename, int frames, Uint8 priority);
//extern SPRITE *spr ite_add_file2(char *filename, int frames, Uint8 priority);
extern SPRITE *sprite_add_res(int image_resource_num);
extern SPRITE *sprite_add_bullet(int bullet_type_num);

#if (0)
extern SPRITE *spr ite_add_000xy00(
	SDL_Surface *surface,
	int total_frames,
	int x_divide_frames,
	int y_divide_frames,
	Uint8 priority,
	int set_flags/*nocache*/,
	int anime_speed
);
#endif
#if (1)
extern SPRITE *sprite_add_res_list(
	SDL_Surface *surface,
//	int total_frames,
//	int x_divide_frames,
//	int y_divide_frames,
//	Uint8 priority,
	int set_flags/*nocache*/,
//	int anime_speed
	IMAGE_RESOURCE *image_resource_ptr
	);
#endif

extern void sprite_remove_all000(int type);
//extern void sprite_remove_all222(int type);/*弾幕用*/

extern void sprite_display000(int type);/**/
//extern void sprite_display222(int type);/*弾幕用*/
extern void sprite_work000(int type);/**/
//extern void sprite_work222(int type);/*弾幕用*/

extern SPRITE *sprite_collision_check(SPRITE *tocheck, int type);


#if 1
	/* ソフトウェアーで Zソート */
	#define USE_ZBUFFER 	(0)
#else
	/* ハードウェアーでソート */
	#define USE_ZBUFFER 	(1)
#endif

#if 1
	/* 単純拡大 */
	#define USE_ZOOM_XY 	0
#else
	/* 縦横拡大 */
	#define USE_ZOOM_XY 	1
#endif


//#ifndef GRP_SCREEN_H
//#define GRP_SCREEN_H
/*	@since		Jul.27.2005 GRP_SCREEN_H		画面表示管理 */
/* --- 管理する最大スプライト数 */
//#define SPRITEMAX 1024
//#define SPRITEMAX 256
#define SPRITEMAX	512

typedef struct
{
	UINT8 used;
	UINT8 dummy_alpha;
	UINT8 texture_id;
	UINT8 center;	/*dummy*/
//
	/* - public */
	int x256;
	int y256;
	int w;
	int h;
	int tx;
	int ty;
	#if (1==USE_ZOOM_XY)
	int zoom_x256;
	int zoom_y256;
	#else //(0==USE_ZOOM_XY)
	int zoom_xy256;
	#endif/* (1==USE_ZOOM_XY) */
	int rotation_z;
	#if (1==USE_ZBUFFER)
	int priority;		/* 表示優先順位 */	/* unsigned short */
	#endif/* (1==USE_ZBUFFER) */
	u32 color8888;//	int color8888;
//	Image *Texture;//
	//SDL_Surface *Texture_ptr;
} TGameSprite /*, *PTGameSprite*/;



#endif/* _SPRITE_H_ */
