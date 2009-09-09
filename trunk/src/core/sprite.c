
/*---------------------------------------------------------
	スプライト マネージャ
--------------------------------------------------------- */

#include "game_main.h"

/*---------------------------------------------------------
	リソース resource
--------------------------------------------------------- */
static IMAGE_RESOURCE my_bullet_resource[] =
{
#if 1
	/* [有る名前で] */
	//#define DUMMY_IMG	"tama/bullet_maru16.png"
	#define DUMMY_IMG	"jiki/core_re.png"
	/* 77 */	//	{	"tama/kugel.png",							0,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },
	/* 78 */	//	{	"tama/kugel2.png",							0,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },
	/* 79 */		{	/*"tama/bullet_beam16.png"*/DUMMY_IMG,		0,	16,  iyx(16,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* 針弾 (敵弾) */
	/* 80 */		{	/*"tama/bullet_maru16.png"*/DUMMY_IMG,		0,	16,  iyx(16,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* 白丸(小) (敵弾) */	/* 赤丸(中) (敵弾) */
//
	/* 81 */		{	/*"tama/bullet_ming32.png"*/DUMMY_IMG,		0,	32,  iyx(32,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* ゆかりん弾 (敵弾) */
	/* 82 */		{	/*"tama/jippou32.png"*/DUMMY_IMG,			0,	32,  iyx(32,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* 白模擬弾 (敵弾) */
//
					/* 追加予定 */																				/* クナイ弾 (敵弾) */
//
	/* 83 */		{	/*"tama/oodama08.png"*/DUMMY_IMG,			1,	 8,  iyx( 8,   1), PRIORITY_05_BULLETS,	  0, 4, 0 },/* 大玉(黒青赤...)	黒玉(輪) PRIORITY_03_ENEMY は、あたり判定部分 */
	/* 83 */	//	{	"tama/bigkugel2.png",						0,	 1,  iyx( 1,   1), PRIORITY_03_ENEMY,	  0, 2, 0 },/* 黒玉(輪)  あたり判定部分 */
	/* 84 */	//	{	"tama/bigkugel1.png",						1,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },/* 大玉(青) 表示部分 */
	/* 85 */	//	{	"tama/new_bigkugel.png",					1,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },/* 大玉(赤) 表示部分 */
//
	/* 86 */		{	/*"tama/knife.png"*/DUMMY_IMG,				1,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },/* 垂直降下ナイフ(赤) */
	/* 87 */		{	/*"tama/knife_core16.png"*/DUMMY_IMG,		1,	16,  iyx(16,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },/* 全方向ナイフ(青) */

	/*103 */		{	"zako/homing16.png",						0,	16,  iyx(16,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* ザコ 誘導弾 */

#endif
};


static IMAGE_RESOURCE my_resource[] =
{	/*ショット */
	/*	0 REIMU */	{	"jiki/shot_re.png", 			0,	 1,  iyx( 1,   1), PRIORITY_01_SHOT,   0, 4, 0 },	/* "plasma.png" */
	/*	1 MARISA */ {	"jiki/shot_ma.png", 			0,	 1,  iyx( 1,   1), PRIORITY_01_SHOT,   0, 4, 0 },	/* "plasma_ma.png" */
	/*	2 REMILIA */{	"jiki/shot_oz.png", 			0,	 1,  iyx( 1,   1), PRIORITY_01_SHOT,   0, 4, 0 },	/* "plasma_oz.png" */
	/*	3 CIRNO */	{	"jiki/shot_ci.png", 			0,	 1,  iyx( 1,   1), PRIORITY_01_SHOT,   0, 6, 0 },	/* "plasma_ci.png" */
	/*	4 YUYUKO */ {	"jiki/shot_yu.png", 			0,	 1,  iyx( 1,   1), PRIORITY_01_SHOT,   0, 8, 0 },
//
	/*	5 REIMU */	{	"jiki/needle_re.png",			1,	 2,  iyx( 2,   1), PRIORITY_01_SHOT,   1, 6, 0 },	// 針弾
	/*	6 MARISA */ {	"jiki/needle_ma.png",			0,	 1,  iyx( 1,   1), PRIORITY_01_SHOT,   0, 8, 0 },	// 森弾
	/*	7 REMILIA */{	"jiki/needle_oz.png",			1,	 5,  iyx( 5,   1), PRIORITY_01_SHOT,   3,12/*8*/, 0 },	// こうもり弾
	/*	8 CIRNO */	{	"jiki/needle_ci.png",			0,	 1,  iyx( 1,   1), PRIORITY_01_SHOT,   0,16, 0 },	// ウェイブ弾
	/*	9 YUYUKO */ {	"jiki/needle_yu.png",			0,	 1,  iyx( 1,   1), PRIORITY_01_SHOT,   0, 8, 0 },	// ピンク蝶弾
//
	/* 10 REIMU */	{	"jiki/homing_re.png",			0,	 6,  iyx( 6,   1), PRIORITY_01_SHOT,   0,16, 0 },	/* "tshoot.png", */
	/* 11 MARISA */ {	"jiki/homing_ma.png",			0,	 6,  iyx( 6,   1), PRIORITY_01_SHOT,   0,16, 0 },	/* "tshoot-ma.png" */
	/* 12 REMILIA */{	"jiki/homing_oz.png",			0,	 6,  iyx( 6,   1), PRIORITY_01_SHOT,   0,16, 0 },	/* "tshoot-oz.png" */
	/* 13 CIRNO */	{	"jiki/homing_ci.png",			0,	 6,  iyx( 6,   1), PRIORITY_01_SHOT,   0,16, 0 },	/* "tshoot-ci.png" */
	/* 14 YUYUKO */ {	"jiki/homing_yu.png",			0,	 6,  iyx( 6,   1), PRIORITY_01_SHOT,   0,16, 0 },	// 青蝶弾
//	/*自分 */
	/* 15 REIMU */	{	"jiki/jiki_re.png", 			0,	12,  iyx(12,   1), PRIORITY_02_PLAYER, 0,16, 0 },	// "ship-med.png",		0,	 11,
	/* 16 MARISA */ {	"jiki/jiki_ma.png", 			0,	12,  iyx(12,   1), PRIORITY_02_PLAYER, 0,16, 0 },	// "ship-med-ma.png",	0,	 11,
	/* 17 REMILIA */{	"jiki/jiki_oz.png", 			0,	12,  iyx(12,   1), PRIORITY_02_PLAYER, 0,16, 0 },	// "ship-med-oz.png",	0,	 11,
	/* 18 CIRNO */	{	"jiki/jiki_ci.png", 			0,	12,  iyx(12,   1), PRIORITY_02_PLAYER, 0,16, 0 },	// "ship-med-ci.png",	0,	 11,
	/* 19 YUYUKO */ {	"jiki/jiki_yu.png", 			0,	12,  iyx(12,   1), PRIORITY_02_PLAYER, 0,16, 0 },
//	/*あたり判定 */
	/* 20 REIMU */	{	"jiki/core_re.png", 			0,	 1,  iyx( 1,   1), PRIORITY_03_ENEMY,  0, 1/*2*/, 0 },	// "core.png",
	/* 21 MARISA */ {	"jiki/core_ma.png", 			0,	 1,  iyx( 1,   1), PRIORITY_03_ENEMY,  0, 2/*4*/, 0 },	// "core-ma.png",
	/* 22 REMILIA */{	"jiki/core_oz.png", 			0,	 1,  iyx( 1,   1), PRIORITY_03_ENEMY,  0, 2/*4*/, 0 },	// "core-oz.png",
	/* 23 CIRNO */	{	"jiki/core_ci.png", 			0,	 1,  iyx( 1,   1), PRIORITY_03_ENEMY,  0, 1/*3*/, 0 },	// "core-ci.png",
	/* 24 YUYUKO */ {	"jiki/core_yu.png", 			0,	 1,  iyx( 1,   1), PRIORITY_03_ENEMY,  0, 2/*4*/, 0 },
//	/* オプション用素材 */
	/* 25 REIMU */	{	"jiki/option1_re.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "option.png" */
	/* 26 MARISA */ {	"jiki/option1_ma.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "mari_op.png" */
	/* 27 REMILIA */{	"jiki/option1_oz.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "oze_op1.png" */
	/* 28 CIRNO */	{	"jiki/option1_ci.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "ci_op1.png" */
	/* 29 YUYUKO */ {	"jiki/option1_yu.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },
//
	/* 30 REIMU */	{	"jiki/option2_re.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* [起動チェックで使うので有る名前で] */
	/* 31 MARISA */ {	"jiki/option2_ma.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* [起動チェックで使うので有る名前で] */
	/* 32 REMILIA */{	"jiki/option2_oz.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "oze_op2.png" */
	/* 33 CIRNO */	{	"jiki/option2_ci.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "ci_op2.png" */
	/* 34 YUYUKO */ {	"jiki/option2_yu.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },
//
	/* 35 REIMU */	{	"jiki/option3_re.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* [起動チェックで使うので有る名前で] */
	/* 36 MARISA */ {	"jiki/option3_ma.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* [起動チェックで使うので有る名前で] */
	/* 37 REMILIA */{	"jiki/option3_oz.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "oze_op3.png" */
	/* 38 CIRNO */	{	"jiki/option3_ci.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "ci_op3.png" */
	/* 39 YUYUKO */ {	"jiki/option3_yu.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },
//
	/* 40 REIMU */	{	"jiki/option4_re.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* [起動チェックで使うので有る名前で] */
	/* 41 MARISA */ {	"jiki/option4_ma.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* [起動チェックで使うので有る名前で] */
	/* 42 REMILIA */{	"jiki/option4_oz.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "oze_op4.png" */
	/* 43 CIRNO */	{	"jiki/option4_ci.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "ci_op4.png" */
	/* 44 YUYUKO */ {	"jiki/option4_yu.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },

//	/* ボム用素材 */
	/* 45 REIMU */	{	"jiki/bomber1_re.png",			1,	32,  iyx(16,   2), PRIORITY_03_ENEMY,  0, 4, 0 },	/* "cshoot1.png""cshoot.png" */
	/* 46 MARISA */ {	"jiki/bomber1_ma.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5,16, 0 },	/* "star_shield_blue.png" */
	/* 47 REMILIA */{	"jiki/bomber1_oz.png",			0,	10,  iyx(10,   1), PRIORITY_01_SHOT,   3,16, 0 },	/* "cross_red.png" */
	/* 48 CIRNO */	{	"jiki/bomber1_ci.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5,32, 0 },	/* "cirno_shield_blue.png" */
	/* 49 YUYUKO */ {	"jiki/bomber1_yu.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5, 4, 0 },
//
	/* 50 REIMU */	{	"jiki/bomber2_re.png",			1,	32,  iyx(16,   2), PRIORITY_03_ENEMY,  0, 4, 0 },	/* "cshoot2r.png""cshoot2.png" */
	/* 51 MARISA */ {	"jiki/bomber2_ma.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5,16, 0 },	/* "star_shields_red.png" */
	/* 52 REMILIA */{	"jiki/bomber2_oz.png",			1,	 4,  iyx( 4,   1), PRIORITY_01_SHOT,   3,16, 0 },	/* "fire_wind_l.png" */ 	/*0a */
	/* 53 CIRNO */	{	"jiki/bomber2_ci.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5,32, 0 },	/* "cirno_shields_red.png" */
	/* 54 YUYUKO */ {	"jiki/bomber2_yu.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5, 4, 0 },
//
	/* 55 REIMU */	{	"jiki/bomber1_re.png",			1,	32,  iyx(16,   2), PRIORITY_03_ENEMY,  0, 2, 0 },	/* [起動チェックで使うので有る名前で] */
	/* 56 MARISA */ {	"jiki/bomber3_ma.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5,16, 0 },	/* "star_shield_green.png" */
	/* 57 REMILIA */{	"jiki/bomber3_oz.png",			1,	 4,  iyx( 4,   1), PRIORITY_01_SHOT,   3,16, 0 },	/* "fire_wind_r.png" */ 	/*0a */
	/* 58 CIRNO */	{	"jiki/bomber3_ci.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5,32, 0 },	/* "cirno_shield_green.png" */
	/* 59 YUYUKO */ {	"jiki/bomber3_yu.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5, 4, 0 },
//
	/* 60 REIMU */	{	"jiki/bomber1_re.png",			1,	32,  iyx(16,   2), PRIORITY_03_ENEMY,  0, 2, 0 },	/* [起動チェックで使うので有る名前で] */
	/* 61 MARISA */ {	"jiki/bomber4_ma.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5, 8, 0 },	/* "star_shields_blue.png" */
	/* 62 REMILIA */{	"jiki/bomber4_oz.png",			1,	 4,  iyx( 4,   1), PRIORITY_01_SHOT,   3,16, 0 },	/* "fire_wind_u.png" */ 	/*0a */
	/* 63 CIRNO */	{	"jiki/bomber4_ci.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5,24, 0 },	/* "cirno_shields_blue.png" */
	/* 64 YUYUKO */ {	"jiki/bomber4_yu.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5, 4, 0 },
//
	/* 65 REIMU */	{	"jiki/bomber1_re.png",			1,	32,  iyx(16,   2), PRIORITY_03_ENEMY,  0, 2, 0 },	/* [起動チェックで使うので有る名前で] */
	/* 66 MARISA */ {	"jiki/bomber5_ma.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5, 8, 0 },	/* "star_shield_red.png" */
	/* 67 REMILIA */{	"jiki/bomber4_oz.png",			1,	 4,  iyx( 4,   1), PRIORITY_01_SHOT,   3, 2, 0 },	/* [起動チェックで使うので有る名前で] */	/*0a */
	/* 68 CIRNO */	{	"jiki/bomber5_ci.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5,24, 0 },	/* "cirno_shield_red.png" */
	/* 69 YUYUKO */ {	"jiki/bomber5_yu.png",			0,	 8,  iyx( 8,   1), PRIORITY_01_SHOT,   0,80, 0 },	/* 幽々子ボムの扇本体 */
//
	/* 70 REIMU */	{	"jiki/bomber1_re.png",			1,	32,  iyx(16,   2), PRIORITY_03_ENEMY,  0, 2, 0 },	/* [起動チェックで使うので有る名前で] */
	/* 71 MARISA */ {	"jiki/bomber6_ma.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5, 8, 0 },	/* "star_shields_green.png" */
	/* 72 REMILIA */{	"jiki/bomber4_oz.png",			1,	 4,  iyx( 4,   1), PRIORITY_01_SHOT,   3, 2, 0 },	/* [起動チェックで使うので有る名前で] */	/*0a */
	/* 73 CIRNO */	{	"jiki/bomber6_ci.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5,24, 0 },	/* "cirno_shields_green.png" */
	/* 74 YUYUKO */ {	"jiki/bomber4_yu.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5, 2, 0 },	/* [起動チェックで使うので有る名前で] */
//
	/* 75 */		{	"jiki/bomber_slow.png", 		1,	 4,  iyx( 4,   1), PRIORITY_01_SHOT,   0, 0, 0 },
	/* 76 */		{	"panel/bonus_items.png",		0,	 8,  iyx( 8,   1), PRIORITY_04_ITEM,   0,12, 0 },
//
#if 0
	/* 77 */	//	{	"tama/kugel.png",				0,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },
	/* 78 */	//	{	"tama/kugel2.png",				0,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },
	/* 79 */		{	"tama/bullet_beam16.png",		0,	16,  iyx(16,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* 針弾 (敵弾) */
	/* 80 */		{	"tama/bullet_maru16.png",		0,	16,  iyx(16,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* 白丸(小) (敵弾) */	/* 赤丸(中) (敵弾) */
//
	/* 81 */		{	"tama/bullet_ming32.png",		0,	32,  iyx(32,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* ゆかりん弾 (敵弾) */
	/* 82 */		{	"tama/jippou32.png",			0,	32,  iyx(32,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* 白模擬弾 (敵弾) */
//
					/* 追加予定 */																				/* クナイ弾 (敵弾) */
//
	/* 83 */		{	"tama/oodama08.png",			1,	 8,  iyx( 8,   1), PRIORITY_05_BULLETS,	  0, 4, 0 },/* 大玉(黒青赤...)	黒玉(輪) PRIORITY_03_ENEMY は、あたり判定部分 */
	/* 83 */	//	{	"tama/bigkugel2.png",			0,	 1,  iyx( 1,   1), PRIORITY_03_ENEMY,	  0, 2, 0 },/* 黒玉(輪)  あたり判定部分 */
	/* 84 */	//	{	"tama/bigkugel1.png",			1,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },/* 大玉(青) 表示部分 */
	/* 85 */	//	{	"tama/new_bigkugel.png",		1,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },/* 大玉(赤) 表示部分 */
//
	/* 86 */		{	"tama/knife.png",				1,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },/* 垂直降下ナイフ(赤) */
	/* 87 */		{	"tama/knife_core16.png",		1,	16,  iyx(16,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },/* 全方向ナイフ(青) */

	/*103 */		{	"zako/homing16.png",			0,	16,  iyx(16,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* ザコ 誘導弾 */

#endif
//
	/* 88 */		{	"zako/mahoujin_0.png",			1,	 2,  iyx( 2,   1), PRIORITY_01_SHOT,   0, 0, 0 },/*0a */ /* 敵の後ろのアレ */
	/* 89 */		{	"zako/tikei_bgpanel1.png",		0,	 1,  iyx( 1,   1), PRIORITY_00_BG,	  0, 0, 0 },	/* 地形３連パネル */
	/* 90 */		{	"zako/tikei_bgpanel2.png",		0,	 1,  iyx( 1,   1), PRIORITY_00_BG,	  0, 0, 0 },	/* 地形パネル棒 */
	/* 91 */		{	"zako/tikei_grounder08.png",	0,	 8,  iyx( 8,   1), PRIORITY_01_SHOT,   2,16, 0 },	/* マンホール */
//
	/* 92 */		{	"zako/obake08.png", 			0,	 8,  iyx( 8,   1), PRIORITY_03_ENEMY,  0, 8, 0 },	/* ザコ */
	/* 93 */		{	"zako/yukari8x4.png",			0,	32,  iyx( 8,   4), PRIORITY_03_ENEMY,  0, 2, 0 },	/* ザコ */
	/* 94 */		{	"zako/aka_kedama08.png",		0,	 8,  iyx( 8,   1), PRIORITY_03_ENEMY,  4, 6, 0 },	/* ザコ 4 1 */
	/* 95 */		{	"zako/niji_kedama16.png",		0,	16,  iyx(16,   1), PRIORITY_03_ENEMY,  1, 8, 0 },	/* ザコ */
//
	/* 96 */		{	"zako/midoori_kedama16.png",	0,	16,  iyx(16,   1), PRIORITY_03_ENEMY,  4, 4, 0 },	/* ザコ 4 1 */
	/* 97 */		{	"zako/kedama16.png",			0,	16,  iyx(16,   1), PRIORITY_03_ENEMY,  5, 4, 0 },	/* ザコ */
	/* 98 */		{	"zako/inyou1_16.png",			0,	16,  iyx(16,   1), PRIORITY_03_ENEMY,  0,40, 0 },	/* ザコ */
	/* 99 */		{	"zako/tatsumaki16.png", 		0,	16,  iyx(16,   1), PRIORITY_03_ENEMY,  2,16, 0 },	/* ザコ */
//
	/*100 */		{	"zako/great_fairy02.png",		0,	 2,  iyx( 2,   1), PRIORITY_03_ENEMY,  0,16, 0 },	/* ザコ(中ザコ) - */
	/*101 */		{	"zako/aka_meido08.png", 		0,	 8,  iyx( 8,   1), PRIORITY_03_ENEMY,  0,24, 0 },	/* ザコ */
	/*102 */		{	"zako/ao_yousei4x6.png", 		0,	24,  iyx( 4,   6), PRIORITY_03_ENEMY,  0,12, 0 },	/* ザコ "zako/ao_yousei24.png" */
//	/* 小爆発 */
	/*104 */		{	"effect/tr_blue.png",			0,	 6,  iyx( 6,   1), PRIORITY_04_ITEM,   1, 0, 0 },
	/*105 */		{	"effect/tr_red.png",			0,	 6,  iyx( 6,   1), PRIORITY_04_ITEM,   1, 0, 0 },
	/*106 */		{	"effect/tr_green.png",			0,	 6,  iyx( 6,   1), PRIORITY_04_ITEM,   1, 0, 0 },
//	/* ザコ消滅爆発 */
	/*107 */		{	"effect/bakuha05.png",			1,	 5,  iyx( 5,   1), PRIORITY_04_ITEM,   3, 0, 0 },
	/*108 */		{	"effect/bakuha06.png",			1,	 5,  iyx( 5,   1), PRIORITY_04_ITEM,   3, 0, 0 },
	/*109 */		{	"effect/bakuha07.png",			1,	 5,  iyx( 5,   1), PRIORITY_04_ITEM,   3, 0, 0 },
//	/* 火炎爆発 */
	/*110 */		{	"effect/ex.png",				0,	29,  iyx(29,   1), PRIORITY_04_ITEM,   3, 0, 0 },
//
	/*111 */		{	"boss/boss01_0.png",			0,	 4,  iyx( 4,   1), PRIORITY_03_ENEMY,  8, 4, 0 },
	/*112 */		{	"boss/boss01_1.png",			0,	 8,  iyx( 8,   1), PRIORITY_03_ENEMY,  0, 8, 0 },
	/*113 */		{	"boss/boss01_2.png",			0,	 4,  iyx( 4,   1), PRIORITY_03_ENEMY,  8, 4, 0 },
	/*114 */		{	"boss/boss01_3.png",			0,	 2,  iyx( 2,   1), PRIORITY_03_ENEMY,  8, 4, 0 },
	/*115 */		{	"boss/boss01_4.png",			0,	 2,  iyx( 2,   1), PRIORITY_03_ENEMY,  8, 4, 0 },
//
	/*116 */		{	"boss/aya.png", 				0,	 9,  iyx( 9,   1), PRIORITY_03_ENEMY,  0, 8, 0 },
//
	/*117 */		{	"boss/kaguya.png",				0,	 6,  iyx( 6,   1), PRIORITY_03_ENEMY,  0, 8, 0 },
//	/*117 */		{	"boss/boss04_0.png",			0,	 2,  iyx( 1,   1), PRIORITY_03_ENEMY,  0, 4, 0 },
//	/*118 */		{	"boss/boss04_1.png",			0,	 2,  iyx( 1,   1), PRIORITY_03_ENEMY,  0, 8, 0 },
//	/*119 */		{	"boss/boss04_2.png",			0,	 2,  iyx( 1,   1), PRIORITY_03_ENEMY,  0, 4, 0 },
//	/*120 */		{	"boss/boss04_3.png",			0,	 2,  iyx( 1,   1), PRIORITY_03_ENEMY,  0, 4, 0 },
//	/*121 */		{	"boss/boss04_4.png",			0,	 2,  iyx( 1,   1), PRIORITY_03_ENEMY,  0, 4, 0 },
//	/*122 */		{	"boss/boss04_5.png",			0,	 2,  iyx( 1,   1), PRIORITY_03_ENEMY,  0, 4, 0 },
//
	/*123 */		{	"boss/sakuya.png",				0,	19,  iyx(10,   2), PRIORITY_03_ENEMY,  0, 8, 0 },
//
	/*124 */	//	{	"panel/key_icon.png",			1,	12,  iyx(12,   1), PRIORITY_01_SHOT,   2, 2, 0 },
};


/*---------------------------------------------------------
	あたり判定 collision
--------------------------------------------------------- */

/*---------------------------------------------------------
	矩形/円 あたり判定
	-------------------------------------------------------
	オブジェクト同士の当たり判定
	大まかに矩形で判別した後、近そうなら円の衝突判定
--------------------------------------------------------- */

#if (1)
static /*BOOL */int collision_hit( /*obj_t */SPRITE *obj1, /*obj_t */SPRITE *obj2 )
{
	/* --- 判定有効なオブジェクトでなかったら終了 */
//	if (obj2->m_Hit256R == 0/*.0 */ /*FALSE */)
	if (256 > obj2->m_Hit256R /*FALSE */)
	{
		return (0/*FALSE */);	/* not hit. 当たってない */
	}
//	if (obj1->m_Hit256R == 0/*.0 */ /*FALSE */)
	if (256 > obj1->m_Hit256R /*FALSE */)
	{
		return (0/*FALSE */);	/* not hit. 当たってない */
	}
//
	/*flo at */int dr, dx, dy;
	dr = (obj2->m_Hit256R + obj1->m_Hit256R);
	dx = ((obj2->x256+obj2->w128) - (obj1->x256+obj1->w128));
	dy = ((obj2->y256+obj2->h128) - (obj1->y256+obj1->h128));
	/* check collision, delta x position. 矩形判定（Ｘ軸） */
	if (dx < 0/*.0 */)	{	dx = (-dx); }	/*dx = abs(dx); */
	if (dx > dr)	{	return (0/*FALSE */);	}	/* not hit, because not near the dx. Ｘの差分が大きいので、当たってない */

	/* check collision, delta y position. 矩形判定（Ｙ軸） */
	if (dy < 0/*.0 */)	{	dy = (-dy); }	/*dy = abs(dy); */
	if (dy > dr)	{	return (0/*FALSE */);	}	/*	not hit, because not near the dy. Ｙの差分が大きいので、当たってない */

	/* check collision, circle delta round distance.  円の半径で判定 */
	dr *= dr;
	dx *= dx;
	dy *= dy;
	dx += dy;
	if (dr < dx)/*if ( (dr^2) < ( (dx^2) + (dy^2) )) */
	{
		return (0/*FALSE */);	/* not hit. 当たってない */
	}
	return (1/*TRUE */); /* hit. 当たった */
}
#endif


/*---------------------------------------------------------
	スプライトのあたり判定
--------------------------------------------------------- */

SPRITE *sprite_list000_head = NULL; 	// スプライトのリスト構造

SPRITE *sprite_collision_check(SPRITE *tocheck, int type)
{
	SPRITE *s = sprite_list000_head;/* スプライト リストの先頭 から探す */
	while (NULL != s)
	{
		/* 1. 自分自身チェック */
		/* 判定対象(自分自身)は飛ばす(全く同じもの同士のあたり判定を取っても意味がないので取らない) / zu testendes Sprite ueberspringen */
		if (s != tocheck)
		{
			/* 2. 判定種類チェック */
			if (
				//		(s->type != SP_DELETE ) && /* 削除済みは飛ばす */	//	SP_DELETEが 0 になったので要らなくなった．
						(s->type & type)			 /* typeが一致する場合のみ */	/* Passender Sprite-Type? */
				)
			{
				/* 3. 判定フラグチェック */
				if (
						( (SP_FLAG_COLISION_CHECK|SP_FLAG_VISIBLE) == (s->flags&(SP_FLAG_COLISION_CHECK|SP_FLAG_VISIBLE)))
						/* あたり判定があり、かつ、表示可能なもののみチェック */
					)
				{
					#if 1
					/* 4. 矩形/円あたり判定チェック   大まかに矩形で判別した後、近そうなら円の衝突判定 */
					if (collision_hit(s,tocheck))	/* 矩形/円あたり判定 */
					{
						return (s);/*あたった */
					}
					#endif
				}
			}
		}
		s = s->next;/*次 */
	}
	return (NULL);/*あたってない */
}

/*---------------------------------------------------------
	スプライト リストに登録されたスプライトを消す。
--------------------------------------------------------- */

static void sprite_remove_one(SPRITE *src)
{
	SPRITE *s = sprite_list000_head;/* スプライト リストの先頭 から探す */
	SPRITE *p = NULL;
	SPRITE *n = NULL;
	while (NULL != s)/* スプライト リストの終わりまで探す */
	{
		n = s->next;
		if (s == src)/*見つかった？ */
		{
			if (NULL == p)
			{
				sprite_list000_head = n;	// スプライトリストの先頭を書き換える。
			}
			else
			{
				p->next = n;
			}
			if (0==(s->flags&SP_FLAG_NOT_CACHE))
			{
				unloadbmp_by_surface(s->sprite_bmp);
			}
			#if 0
		//	if ((s->flags&SP_FLAG_FREE_SURFACE))
			if ((s->flags&(SP_FLAG_FREE_SURFACE|SP_FLAG_NOT_CACHE)))
			{
				/* KETMでここは無効になってるが、
				ここが無効になってる限り(点数表示用フォントで)
				延々メインメモリを消費しちゃうので、
				開放してみるテスト */
				/* ここでハングアップ */
				SDL_FreeSurface(s->sprite_bmp);
			}
			#endif
			#if (1==USE_ZUKEI_ATARI_HANTEI)
			sprite_remove_colision_map(s->colision_bmp);
			#endif /* (1==USE_ZUKEI_ATARI_HANTEI) */
			if (NULL != s->data)
			{
				free (s->data);
			}
			free (s);
			return;/* 正常 */
		}
		p = s;
		s = n;
	}
	CHECKPOINT;/* 異常 */
	error(ERR_WARN, "sprite not found in list");
}

/*---------------------------------------------------------
	スプライト リストに登録されたスプライトを全部消す。
--------------------------------------------------------- */

//void sprite_remove_all222(int type){}
void sprite_remove_all000(int type)
{
	SPRITE *s = sprite_list000_head;/* スプライト リストの先頭 から探す */
	SPRITE *n = NULL;
	while (NULL != s)
	{
		n = s->next;
		if (s->type & type) /* typeが一致する場合のみ */
		{
			sprite_remove_one(s);
		}
		s = n;
	}
}

//void sprite_remove_all_type(int type)
//{
//	SPRITE *s = sprite;
//	SPRITE *n = NULL;
//	while (NULL != s)
//	{
//		n = s->next;
//		if (s->type == type)
//		{
//			sprite_remove(s);
//		}
//		s = n;
//	}
//}

/*---------------------------------------------------------
	スプライトを一フレーム分動作させる。
--------------------------------------------------------- */

//void sprite_work222(int type){}
void sprite_work000(int type)
{
	SPRITE *s;
	SPRITE *n;
	s = sprite_list000_head;/* スプライト リストの先頭 から探す */
	while (NULL != s)
	{
		n = s->next;
		#if 0
		if (s->type != SP_DELETE ) /* 削除済みは飛ばす */	//	SP_DELETEが 0 になったので要らなくなった．
		#endif
		{
			/* animate */
			//s->ticks++;
			if (s->type & type) /* typeが一致する場合のみ */
			{
				/*逆転アニメ禁止に変更 */
			//	if (s->anim_speed != 0)
				if (0 < s->anim_speed)
				{
					s->anim_count++;			//		s->anim_count++/*=fps_fa ctor */;
					// if (s->anim_count==abs(s->anim_speed))
					if (s->anim_count >= /*abs */(s->anim_speed))
					{
						s->anim_count = 0;
					//	if (s->anim_speed > 0)
						{
							s->yx_anim_frame += 0x01;
							if (((s->yx_anim_frame)&YX_FRAME_LOW_X) >= ((s->yx_frames)&YX_FRAME_LOW_X))
							{
								#if 0
							//	s->yx_anim_frame -= 0x01;
							//	s->yx_anim_frame += 0x10;
								#else
								s->yx_anim_frame += 0x0f;
								#endif
							}
								if (s->yx_anim_frame >= s->yx_frames)
								{	s->yx_anim_frame = 0;}
						}
					//	else		/*逆転アニメ禁止に変更 */
					//	{
					//		s->anim_frame--;
					//		if (s->anim_frame < 0)
					//		{	s->anim_frame = s->frames-1;}
					//	}
					}
				}
				/* move */
				if (NULL != s->callback_mover)
				{
					(s->callback_mover)(s);
				}
			}
		}
		s = n;
	}
	#if 1
	/* 使用済みスプライトを探して、使用済みスプライトがあれば消す */
	if ( ST_WORK_MENU != (psp_loop&0xff00) )/* メニュー以外の場合、(ポーズメニューで時間が経つ場合に消えると困るので) */
	{
		s = sprite_list000_head;/* スプライト リストの先頭 から探す */
		while (NULL != s)
		{
			n = s->next;
			#if 1/* 自動的に消える機能 */
			/* 一定時間での自動消去、許可フラグONの場合、 */
			if ( (SP_FLAG_TIME_OVER == (s->flags&SP_FLAG_TIME_OVER)) )
			{
				s->timeover_ticks++;
				if ( 1000 < (s->timeover_ticks) )/*16秒 */	/*(1024-1) < */ /*(0x0400 & (s->ticks)) */
				{
				//	s->timeover_ticks = 0;
					s->type = SP_DELETE;	/* 自動消去にする。 */
				}
			}
			#endif
			if (SP_DELETE == s->type)	/* 消去？ */
			{
				sprite_remove_one(s);	/* 削除済みを消す */
			}
			s = n;
		}
	}
	#endif
}

/*---------------------------------------------------------
	スプライトを描画する。
--------------------------------------------------------- */
static void sprite_draw000(SPRITE *s)
{
		SDL_Rect dest;
		SDL_Rect src;
		dest.x = (t256_floor(s->x256));
		dest.y = (t256_floor(s->y256));
		dest.w = ((s->w128)>>7);
		dest.h = ((s->h128)>>7);
//
		src.w = dest.w/*s->w*/; 	src.x = /*s->w*/(dest.w)*(s->yx_anim_frame&YX_FRAME_LOW_X);
		src.h = dest.h/*s->h*/; 	src.y = /*s->w*/(dest.h)*((s->yx_anim_frame&YX_FRAME_HIGH_Y)>>4)/*0*/;

//
//		if (s->flags&SP_FLAG_CHEAP_ALPHA)
//		{
//			blit_cheap_alpha(s->sprite_bmp, &t, sdl_screen[SDL_00_SCREEN], &r);
//		}
//		else
//		{
			#if 0
			/* アルファ値が変わった場合のみ */
			if (s->alpha_chache != s->alpha)
			{
				s->alpha_chache = s->alpha;
				SDL_SetAlpha(s->sprite_bmp,SDL_SRCALPHA,s->alpha_chache);
			}
			#else
				SDL_SetAlpha(s->sprite_bmp,SDL_SRCALPHA,s->alpha);
			#endif
			SDL_BlitSurface(s->sprite_bmp, &src, sdl_screen[SDL_00_SCREEN], &dest);
//		}
}
/*---------------------------------------------------------
	スプライトを表示する。
--------------------------------------------------------- */

#define USE_DEBUG	(0)
#if (1==USE_DEBUG)
extern int debug_num1;
extern int debug_num2;
#endif
//void sprite_display222(int type){}
void sprite_display000(int type)
{
	#if (1==USE_DEBUG)
	int debug_count1;
	int debug_count2;
	debug_count1 = 0;
	debug_count2 = 0;
	#endif

	SPRITE *s;
	s = sprite_list000_head;/* スプライト リストの先頭 から探す */
	while (NULL != s)
	{
		#if (1==USE_DEBUG)
		if (s->type != SP_DELETE )
		{
			debug_count1++;/*有効 スプライト カウント */
			//	max 	debug_count2/debug_count1			/*	 */
			//	max 				PRIORITY_00_BG
			//	max 				PRIORITY_01_SHOT		/* 自弾/地上ザコ敵/魔方陣 */
			//	max 				PRIORITY_02_PLAYER		/* 自機 */
			//	max 				PRIORITY_03_ENEMY		/* ボス/ザコ敵 */
			//	max 				PRIORITY_04_ITEM		/* アイテム/漢字スコア/当たり表示 */
			//	max 	446/623 	PRIORITY_05_BULLETS 	/* 敵弾 */
			//	咲夜4096精密弾lunatic435/589[]
			//	咲夜4096精密弾lunatic437/623
			//	咲夜4096精密弾lunatic446/455	(輝夜 第3形態lunatic 337/351)
			if (PRIORITY_05_BULLETS==s->priority)
			{
				debug_count2++;/*有効 スプライト カウント */
			}
		}
		#endif
		#if 0
		if (s->type != SP_DELETE ) /* 削除済みは飛ばす */	//	SP_DELETEが 0 になったので要らなくなった．
		#endif
		{
			if (s->type & type) /* typeが一致する場合のみ */
			{
				#if 1
				if (0==(s->type & SP_GROUP_BULLETS)) /* とりあえず弾は描かない */
				#endif
				{
					if ((s->flags&SP_FLAG_VISIBLE)) /* 表示可能な場合のみ */
					{
						sprite_draw000(s);
					}
				}
			}
		}
		s=s->next;/*次 */
	}
//
	#if (1==USE_DEBUG)
	/* ピーク(最大値)を調べる */
	if (debug_num1 < debug_count1 ) 	{	debug_num1 = debug_count1;	}
	if (debug_num2 < debug_count2 ) 	{	debug_num2 = debug_count2;	}
	#endif
}

/*---------------------------------------------------------
	スプライトをリストに追加する。
--------------------------------------------------------- */

#if (1)
SPRITE *sprite_add_res_list(
	SDL_Surface *surface,
	int set_flags/*nocache */,
	IMAGE_RESOURCE *image_resource_ptr
	)
#endif
{
	int priority;			priority		= image_resource_ptr->priority;
//
	SPRITE *obj;/* 新規作成するスプライト */
	SPRITE *s;
	SPRITE *tail_obj/*=NULL */; /* 追加するリストの候補位置。切断する場合は切断位置。 */
	obj 		= mmalloc(sizeof(SPRITE));
	s			= sprite_list000_head;	// スプライトリストの先頭。呼び出し
	tail_obj	= NULL;
	/*---------------------------------------------------------
		スプライトの表示プライオリティーを満たす位置を調べて、そこに挿入する。
	--------------------------------------------------------- */
	while (
		(NULL != s) &&				/* リストの頭までしらべたなら終わり */
		(s->priority < priority)	/* 表示プライオリティーを満たす位置まで来たら終わり */
		)
	{
		tail_obj	= s;
		s			= s->next;
	}
	obj->next		= s;	/* 新規作成したスプライトの最後に連結 */
	if (NULL == tail_obj)/* 先頭なら(リストの切断がなければ) */
	{
		/* 先頭の場合は->nextが無いので */
		sprite_list000_head = obj;		// スプライトリストの先頭を書き換える。
	}
	else	//if (NULL != tail_obj)
	{
		tail_obj->next		= obj;	/* リストに挿入 */
	}
	/*---------------------------------------------------------
		スプライトの設定をする。
	--------------------------------------------------------- */
	#if 0
	{	/* この機能(個別のスプライトに番号を割り当て番号で判別する)は大した意味がないので無くした。
			(個別のスプライトを判別したい場合は、ポインタで判別すればこの機能は不要なので) */
		static int current_id	= 0;
		obj->id 				= current_id;
		current_id++;
	}
	#endif
//
	obj->sprite_bmp 	= surface;
//
	int x_divide_frames_m1 = image_resource_ptr->x_divide_frames_m1;
	int y_divide_frames_m1 = image_resource_ptr->y_divide_frames_m1;
//
	obj->yx_frames	= (((y_divide_frames_m1)<<4)|((x_divide_frames_m1)));
//
	x_divide_frames_m1++;
	y_divide_frames_m1++;
//
	obj->w128				= ((surface->w/(x_divide_frames_m1))<<7);
	obj->h128				= ((surface->h/(y_divide_frames_m1))<<7);
//
//	int total_frames;		total_frames	= image_resource_ptr->total_frames;
	int anime_speed;		anime_speed 	= image_resource_ptr->anime_speed;
	int m_Hit256R;			m_Hit256R		= image_resource_ptr->atari_hankei;
//

	obj->m_Hit256R	= (m_Hit256R<<8);

	#if (1==USE_ZUKEI_ATARI_HANTEI)
	obj->colision_bmp		= sprite_add_colision_map(surface, total_frames);
	#endif /* (1==USE_ZUKEI_ATARI_HANTEI) */

//	obj->frames 	= total_frames;
	obj->priority	= priority;
	obj->flags		= set_flags; /*0;	if (nocache) s->flags |= SP_FLAG_NOT_CACHE; */
	obj->alpha		= 255;

	obj->yx_anim_frame = 0x00;
	obj->anim_speed = anime_speed/*0 */;
	obj->anim_count = 0;
	obj->m_angle512 = 0;
//
	obj->timeover_ticks 	= 0;
//
	obj->data				= NULL;
	obj->callback_mover 	= NULL;
	obj->callback_loser 	= NULL;
	return (obj);
}

/* clouds敵の場合、ファイル名 */
/*static */ SPRITE *sprite_add_internal_res(IMAGE_RESOURCE *image_resource_ptr)
{
	char *filename; 		filename		= (char *)image_resource_ptr->file_name;
	int use_alpha;			use_alpha		= image_resource_ptr->use_alpha;
//
	SDL_Surface *s;
	SPRITE *sp;
	s		= loadbmp0(filename, use_alpha, 1);
	if (0 == use_alpha)
	{
		SDL_SetColorKey(s,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	}
	sp	= sprite_add_res_list( s, 0/*flags */, image_resource_ptr);
	return (sp);
}
SPRITE *sprite_add_res(int image_resource_num)
{
	IMAGE_RESOURCE *image_resource_ptr;
	image_resource_ptr = &my_resource[image_resource_num];
	return (sprite_add_internal_res( image_resource_ptr ));
}
SPRITE *sprite_add_bullet(int image_resource_num)
{
	//image_resource_num=BASE_YUKARI32_PNG;
//
	IMAGE_RESOURCE *image_resource_ptr;
	image_resource_ptr = &my_bullet_resource[image_resource_num];
	return (sprite_add_internal_res( image_resource_ptr ));
}
/*---------------------------------------------------------
	画像キャッシュに載せる(ゲーム中に画像展開すると処理落ちが激しくて
	ゲームにならない。そこで予めキャッシュに載せとく)
--------------------------------------------------------- */

/*static */ void load_bg2_chache(char *filename, int use_alpha)
{
	SDL_Surface *tmp;
	tmp = loadbmp0(filename, /*0 */use_alpha, 1);
	unloadbmp_by_surface(tmp);
};

/*---------------------------------------------------------
	読み込んだ順番に画像キャッシュに配置されるので、
	画像キャッシュの順番を決める為の読み込み。
	-------------------------------------------------------
	画像キャッシュに載せる必要があるものを、
	先に画像キャッシュに載せる為の読み込み
	画像キャッシュに載せたい順に読み込む。
	ファイルの確認も兼ねる。
	alle benoetigten Bilder in den Cache laden
--------------------------------------------------------- */
extern void load_bg2_chache(char *filename, int use_alpha);
void preload_gfx(void)
{
	int image_resource_num;
	for (image_resource_num=0; image_resource_num<(FILE_RESOURCE_MAX); image_resource_num++)
	{
		char *fff;	fff = (char *)my_resource[image_resource_num].file_name;
		int ccc;	ccc = my_resource[image_resource_num].use_alpha;
		load_bg2_chache(fff/*char *filename */, ccc /*use_alpha */);
	}
}
