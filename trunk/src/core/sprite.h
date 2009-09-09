#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "game_main.h"

/* Sprites */
//#define MAX_SPRITE 200

enum
{
	BASE_PLAZMA_PNG = 0,
	BASE_PLAZMA_PNG_ma,
	BASE_PLAZMA_PNG_oz,
	BASE_PLAZMA_PNG_ci,
	BASE_PLAZMA_PNG_yu,
//
	BASE_NEEDLE_PNG,
	BASE_NEEDLE_PNG_ma,
	BASE_NEEDLE_PNG_oz,
	BASE_NEEDLE_PNG_ci,
	BASE_NEEDLE_PNG_yu,
//
	BASE_TSHOOT_PNG,
	BASE_TSHOOT_PNG_ma,
	BASE_TSHOOT_PNG_oz,
	BASE_TSHOOT_PNG_ci,
	BASE_TSHOOT_PNG_yu,
//
	BASE_SHIP_MED_PNG,
	BASE_SHIP_MED_PNG_ma,
	BASE_SHIP_MED_PNG_oz,
	BASE_SHIP_MED_PNG_ci,
	BASE_SHIP_MED_PNG_yu,
//
	BASE_CORE_PNG,
	BASE_CORE_PNG_ma,
	BASE_CORE_PNG_oz,
	BASE_CORE_PNG_ci,
	BASE_CORE_PNG_yu,
//
	BASE_OPTION1_PNG,
	BASE_OPTION1_PNG_ma,
	BASE_OPTION1_PNG_oz,
	BASE_OPTION1_PNG_ci,
	BASE_OPTION1_PNG_yu,
//
	BASE_OPTION2_PNG,
	BASE_OPTION2_PNG_ma,
	BASE_OPTION2_PNG_oz,
	BASE_OPTION2_PNG_ci,
	BASE_OPTION2_PNG_yu,
//
	BASE_OPTION3_PNG,
	BASE_OPTION3_PNG_ma,
	BASE_OPTION3_PNG_oz,
	BASE_OPTION3_PNG_ci,
	BASE_OPTION3_PNG_yu,
//
	BASE_OPTION4_PNG,
	BASE_OPTION4_PNG_ma,
	BASE_OPTION4_PNG_oz,
	BASE_OPTION4_PNG_ci,
	BASE_OPTION4_PNG_yu,
//
//
	BASE_BOMBER1_PNG,
	BASE_BOMBER1_PNG_ma,
	BASE_BOMBER1_PNG_oz,
	BASE_BOMBER1_PNG_ci,
	BASE_BOMBER1_PNG_yu,
//
	BASE_BOMBER2_PNG,
	BASE_BOMBER2_PNG_ma,
	BASE_BOMBER2_PNG_oz,
	BASE_BOMBER2_PNG_ci,
	BASE_BOMBER2_PNG_yu,
//
	BASE_BOMBER3_PNG,
	BASE_BOMBER3_PNG_ma,
	BASE_BOMBER3_PNG_oz,
	BASE_BOMBER3_PNG_ci,
	BASE_BOMBER3_PNG_yu,
//
	BASE_BOMBER4_PNG,
	BASE_BOMBER4_PNG_ma,
	BASE_BOMBER4_PNG_oz,
	BASE_BOMBER4_PNG_ci,
	BASE_BOMBER4_PNG_yu,
//
	BASE_BOMBER5_PNG,
	BASE_BOMBER5_PNG_ma,
	BASE_BOMBER5_PNG_oz,
	BASE_BOMBER5_PNG_ci,
	BASE_BOMBER5_PNG_yu,
//
	BASE_BOMBER6_PNG,
	BASE_BOMBER6_PNG_ma,
	BASE_BOMBER6_PNG_oz,
	BASE_BOMBER6_PNG_ci,
	BASE_BOMBER6_PNG_yu,
//
	BASE_BOMBER_SLOW_PNG,
	BASE_BONUS_ITEMS_PNG,
//
#if 0
	//BASE_TAMA_KUGEL_PNG,
	//BASE_TAMA_KUGEL_MINI2_PNG,
	BASE_TAMA_BULLET_BEAM16_PNG,
	BASE_TAMA_BULLET_MARU16_PNG,
//
	BASE_TAMA_BULLET_MING32_PNG,
	BASE_TAMA_BULLET_JIPPOU32_PNG,
//
	BASE_TAMA_OODAMA_08_PNG,/* 大玉(黒青赤...)	黒玉(輪) PRIORITY_03_ENEMY は、あたり判定部分 */
//	BASE_TAMA_OODAMA_00_PNG,/* 黒玉(輪)  あたり判定部分*/
//	BASE_TAMA_OODAMA_01_PNG,/* 大玉(青) 表示部分 */
//	BASE_TAMA_OODAMA_02_PNG,/* 大玉(赤) 表示部分*/
//
	BASE_TAMA_BULLET_KNIFE01_PNG,/* 垂直降下ナイフ(赤) */
	BASE_TAMA_BULLET_KNIFE18_PNG,/* 全方向ナイフ(青) */

	BASE_HOMING16_PNG,/* ザコ 誘導弾 */
#endif
//
	BASE_MAHOUJIN_0_PNG,
	BASE_TIKEI_BGPANEL1_PNG,
	BASE_TIKEI_BGPANEL2_PNG,
	BASE_TIKEI_GROUNDER08_PNG,
//
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
//	/* 小爆発 */
	BASE_TR_BLUE_PNG,
	BASE_TR_RED_PNG,
	BASE_TR_GREEN_PNG,
//	/* ザコ消滅爆発 */
	BASE_BAKUHA05_PNG,
	BASE_BAKUHA06_PNG,
	BASE_BAKUHA07_PNG,
//	/* 火炎爆発 */
	BASE_BAKUHA_EX_PNG,
//
	BASE_BOSS_ALICE_0_PNG,
	BASE_BOSS_ALICE_1_PNG,
	BASE_BOSS_ALICE_2_PNG,
	BASE_BOSS_ALICE_3_PNG,
	BASE_BOSS_ALICE_4_PNG,
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
	FILE_RESOURCE_MAX/*最大数*/
};
enum
{
	//TAMA_TYPE_KUGEL_PNG,
	//TAMA_TYPE_KUGEL_MINI2_PNG,
	TAMA_TYPE_BULLET_BEAM16_PNG,
	TAMA_TYPE_BULLET_MARU16_PNG,
//
	TAMA_TYPE_BULLET_MING32_PNG,
	TAMA_TYPE_BULLET_JIPPOU32_PNG,
//
	TAMA_TYPE_OODAMA_08_PNG,/* 大玉(黒青赤...)	黒玉(輪) PRIORITY_03_ENEMY は、あたり判定部分 */
//	TAMA_TYPE_OODAMA_00_PNG,/* 黒玉(輪)  あたり判定部分*/
//	TAMA_TYPE_OODAMA_01_PNG,/* 大玉(青) 表示部分 */
//	TAMA_TYPE_OODAMA_02_PNG,/* 大玉(赤) 表示部分*/
//
	TAMA_TYPE_BULLET_KNIFE01_PNG,/*垂直降下ナイフ(赤)*/
	TAMA_TYPE_BULLET_KNIFE18_PNG,/*全方向ナイフ(青)*/
	TAMA_TYPE_HOMING16_PNG,
};
#define TAMA_TYPE_BULLET_DUMMY TAMA_TYPE_BULLET_MARU16_PNG


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
	PRIORITY_00_BG = 0, 	//	P R_BACK0 = 0,
	PRIORITY_01_SHOT,		//	P RIORITY_01_GROUNDER,		/*自弾/地上ザコ敵/魔方陣*/
// [自機関連]
	PRIORITY_02_PLAYER, 	/* 自機 */
// [敵関連]
	PRIORITY_03_ENEMY,		/* ボス/ザコ敵 */
// [アイテム関連]
	PRIORITY_04_ITEM,		/* アイテム/漢字スコア */	//P R_PLAYER2,			/*当たり表示*/
// [敵弾関連]
	PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY_WEAPON*/,		/*敵弾*/
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
#define SP_GROUP_PLAYER 		0x0100/*0x0100*/
#define SP_GROUP_ZAKO			0x0200/*0x0400*/
#define SP_GROUP_BOSS			0x0400/*0x0400*/
#define SP_GROUP_BULLETS		0x0800/*0x0800*/
#define SP_GROUP_ITEMS			0x1000/*0x1000*/
#define SP_GROUP_PAUSE_OBJS 	0x2000/*0x2000*/
#define SP_GROUP_ALL_TYPE		0xff00
#define SP_GROUP_SUB_TYPE		0x00ff
//#define SP_GROUP_SHOTS		0x0200
//#define SP_GROUP_ETC			0x4000
//#define SP_GROUP_ENEMYS		0x0200/*0x0400*/
// 共用 */
//#define SP_GROUP_TEXTS			(SP_GROUP_PLAYER)/*0x2000*/
enum /*sprite_type*/
{
//	自分 */
	SP_PLAYER/*プレイヤーの本体(あたり判定がアイテム取得)*/ 	= /*0x0100*/SP_GROUP_PLAYER,	/* Unser Held */
	SP_PLAYER2,/*プレイヤーのコア(あたり判定が死亡判定)*/
//	自弾 */
	SP_SHOT_BOSS/*プレイヤーのボスに有効弾*/,// 			//	= /*0x0200*/SP_GROUP_SHOTS, 	/* Waffen des Helden */
	SP_SHOT_ZAKO,/*プレイヤーのボスに無効弾*/
//	その他 */
	SP_ETC,//												//	= /*0x4000*/SP_GROUP_ETC,		/* diverses */
//------------- */
//	ザコ敵 (ボム中、体当たりでプレイヤー死なない)  */
	SP_ZAKO 													= /*0x0400*/SP_GROUP_ZAKO,		/* Die Boesen */
//------------- */
//	ボス敵 / 中ザコ敵 (ボム中、体当たりでプレイヤー死ぬ)  */
	SP_CHUU /*特殊敵[中型敵]*/ /*SP_ZAKO_YOKAI1*/				= /*0x0400*/SP_GROUP_BOSS,		// [***090325
	SP_BOSS,
//------------- */
//	敵弾 */
	SP_BULLET													= /*0x0800*/SP_GROUP_BULLETS,	/* ... und ihre Waffen */
//------------- */
//	アイテム */
	SP_ITEM_00_P001 											= /*0x1000*/SP_GROUP_ITEMS, 	/* Bonus items */
	SP_ITEM_01_P008,	//←ウェポンアイテム(中)	//ウェポンアイテム(強)→SP_ITEM_EXTRA_SHIELD,
	SP_ITEM_02_BOMB,	//→これらは低速ボムに吸収された。SP_ITEM_EXTRA_HOMING, SP_ITEM_EXTRA_HLASER,
	SP_ITEM_03_1UP,
	SP_ITEM_04_P128,
	SP_ITEM_05_HOSI,
	SP_ITEM_06_TENSU,
	SP_ITEM_07_SPECIAL,
//------------- */
//	ポーズ中移動可能物 */
	SP_MENU_TEXT	/* 文字 */									= /*0x2000*/SP_GROUP_PAUSE_OBJS,

};

enum
{
	BULLET_MARU8_00_AKA 				= /*0x0800*/SP_GROUP_BULLETS,
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
//
	BULLET_HARI32_00_AOI,
	BULLET_HARI32_01_AKA,
	BULLET_HARI32_02_KIIRO,
	BULLET_HARI32_03_DAIDAI,
	BULLET_CAP16_04_SIROI,
	BULLET_CAP16_05_AKA,
	BULLET_KNIFE20_06_AOI,
	BULLET_KNIFE20_07_AKA,
//
	BULLET_UROKO14_00_AOI,
	BULLET_UROKO14_01_AKA,
	BULLET_UROKO14_02_YUKARI,
	BULLET_UROKO14_03_MIDORI,
	BULLET_UROKO14_04_MIZUIRO,
	BULLET_UROKO14_05_KIIRO,
	BULLET_KUNAI12_06_AOI,
	BULLET_KUNAI12_07_MIDORI,
//
	BULLET_KUNAI12_00_AKA,
	BULLET_KUNAI12_01_AKA,
	BULLET_KUNAI12_02_MIDORI,
	BULLET_KUNAI12_03_YUKARI,
	BULLET_KUNAI12_04_AOI,
	BULLET_OFUDA12_05_AOI,
	BULLET_OFUDA12_06_AKA,
	BULLET_OFUDA12_07_MIDORI,
//
	BULLET_OODAMA32_00_SIROI,
	BULLET_OODAMA32_01_AOI,
	BULLET_OODAMA32_02_AKA,
	BULLET_OODAMA32_03_YUKARI,
	BULLET_OODAMA32_04_MIDORI,
	BULLET_OODAMA32_05_AOI,
	BULLET_OODAMA32_06_KIIRO,
	BULLET_OODAMA32_07_PINK,
};



//#define SP_LASER			SP_BULLET
//#define SP_BOSS02ICE		SP_BULLET
//#define SP_BIGBULLET	SP_BULLET

//	SP_ZAKO //	SP_ENEMY_HOMING,/*特殊敵[敵側のホーミング弾]*/
//	SP_ZAKO //	SP_TIKEI,	//SP_GROUNDER,SP_BGPANEL,

//	SP_BOSS01,
//	SP_BOSS02,
//	SP_BOSS03,
//	SP_BOSS04,

//	SP_LASER,
//	SP_BOSS02ICE,
//	SP_BIGBULLET,
//	SP_BIGBULLET_DUMMY == SP_ETC,

//	SP_PL_FIREBALL,
//	SP_PL_KILLRAY,
//	SP_PL_BOMB,
//	SP_PL_HOMING,
//	SP_PL_HLASER,
	#if 0
	/* この辺分ける必要が無いかも (注意；ここの順序は中間コード形式と同じに合わせる予定) */
	//	その他
	SP_ZAKO_01_OBAKE	= /*0x0400*/SP_GROUP_ENEMYS, /* Die Boesen */
	SP_ZAKO_02_YUKARI1,
	SP_ZAKO_03_YUKARI2,
	//	毛玉
	SP_ZAKO_04_AKA_KEDAMA1,
	SP_ZAKO_05_NIJI_KEDAMA1,
	SP_ZAKO_06_MIDORI_KEDAMA1,
	SP_ZAKO_07_MIDORI_KEDAMA2,
	SP_ZAKO_08_KEDAMA1,
	//	竜巻 陰陽玉
	SP_ZAKO_09_INYOU1,
	SP_ZAKO_10_TATSUMAKI1,
	//	妖怪
	SP_ZAKO_11_KAKOMI1,
	//	中妖精
	SP_ZAKO_12_MEIDO1,
	SP_ZAKO_13_MEIDO2,
	SP_ZAKO_14_MEIDO3,
	SP_ZAKO_15_MEIDO4,
	//	小妖精
	SP_ZAKO_16_AO_YOUSEI1,
	SP_ZAKO_17_AO_YOUSEI2,
	/* この辺分ける必要が無いかも (注意；ここの順序は中間コード形式と同じに合わせる予定) */
	#endif



typedef struct _colision_map_cache
{
	SDL_Surface *src;	/* Zeiger auf Quell-Src in Pixmap cache (nur fuer Index, *NO* free!) */
	int frames; 		/* Anzahl eintraege in dyn.Array **col */
	int refcount;		/* Wieviele Sprites nutzen diese Colision map? */
	Uint8 **col;		/* Dyn.Array mit Zeiger auf Colision maps */
	struct _colision_map_cache *next;
} COLISION_MAP_CACHE;

/* 針弾とかあるから、現在図形判定しないと詐欺判定になる。 */
/* あたり判定無くしても速度は変わらないがメモリが節約できるので将来的にはこの辺も改良したい。 */
//#define USE_ZUKEI_ATARI_HANTEI (1)
#define USE_ZUKEI_ATARI_HANTEI (0)

typedef struct _sprite
{
	int type;						/* 種類及び使用可否 / Sprite-Type, (siehe enum SPRITE_TYPE), 0 = remove */
	SDL_Surface *sprite_bmp;				/* 画像 / Images */
	#if (1==USE_ZUKEI_ATARI_HANTEI)
	COLISION_MAP_CACHE *colision_bmp;		/* あたり判定用画像 / Zeiger auf Col-Map-Cache entry */
	#endif /* (1==USE_ZUKEI_ATARI_HANTEI) */
	int m_Hit256R;					/* あたり判定用 */
	int m_angle512; 				/* 表示角度 */
//
	int timeover_ticks; 			/* 作成してからの経過時間 (現在KETM自体にバグがある為、一定時間経過すると強制消去する) / number of ticks since creation */
//[4]
	int x256;						/* x表示位置  (256固定小数点形式) / akt. Position */
	int y256;						/* y表示位置  (256固定小数点形式) / akt. Position */
	int w128;						/* 横幅の半分 (256固定小数点形式) / Breite, He */
	int h128;						/* 高さの半分 (256固定小数点形式) / Breite, He */
//[8]
	void *data; 					/* Zeiger auf Strukur mit zus舩zlichen Daten */
	void (*callback_mover)(struct _sprite *s);/* Custom-Move-Routine */
	void (*callback_loser)(struct _sprite *s);/* Custom-loser-Routine (やられた後にボーナスを出すとか、違う爆発するとか) */
	struct _sprite *next;
//[12]
	Uint8 flags;					/* フラグセット / siehe unten (SP_FLAG...) */
	Uint8 priority; 				/* 表示優先順位 / Prioritaet (0: unten, 9 oben) */
	Uint8 alpha;					/* 透明度	 / 255: opak, -  0: durchsichtig */
	Uint8 alpha_chache; 			/* 前回のアルファ値 */
//
	Uint8 yx_anim_frame;			/* / アニメパターンの番号(何コマ目か) */	 /*int*/			/* akt. Frame */
	Uint8 yx_frames;				/* / 表示パターン総数 / Anzahl Frames */
	Uint8 anim_count;				/*int*/ 	// Sint8 anim_count;			/* intern: Zaehler f. anim_speed */
	Uint8 anim_speed/* 256 */;		/*逆転アニメ禁止に変更*/		/* アニメーション速度 / Geschw. der Animation (negative Werte: Anim l舫ft r�ckw舐ts */	 /*Sint8*/ /*Uint8*/
//[16]
} SPRITE;
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
extern void sprite_remove_all222(int type);/*弾幕用*/

extern void sprite_display000(int type);/**/
extern void sprite_display222(int type);/*弾幕用*/
extern void sprite_work000(int type);/**/
extern void sprite_work222(int type);/*弾幕用*/

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
//#define  SPRITEMAX	1024

//#define SPRITEMAX 256
#define SPRITEMAX	512

typedef struct
{
	UINT8 used;
	UINT8 alpha;
	UINT8 TextureId;
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

//	Image *Texture;//
	//SDL_Surface *Texture_ptr;
} TGameSprite /*, *PTGameSprite*/;



#endif/* _SPRITE_H_ */
