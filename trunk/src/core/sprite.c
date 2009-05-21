
/*---------------------------------------------------------
	スプライト マネージャ
---------------------------------------------------------*/

#include "support.h"

/*---------------------------------------------------------
	リソース resource
---------------------------------------------------------*/

typedef struct
{
	const char *file_name;
	Uint8 use_alpha;
	Uint8 frames;
	Uint8 priority;
	Uint8 anime_speed;
} IMAGE_RESOURCE;

static IMAGE_RESOURCE my_resource[] =
{	/*ショット*/
	/*	0 REIMU */	{	"jiki/shot_re.png", 		0,	 1, PRIORITY_01_SHOT,	0 },	/*"plasma.png"*/
	/*	1 MARISA */ {	"jiki/shot_ma.png", 		0,	 1, PRIORITY_01_SHOT,	0 },	/*"plasma_ma.png"*/
	/*	2 REMILIA */{	"jiki/shot_oz.png", 		0,	 1, PRIORITY_01_SHOT,	0 },	/*"plasma_oz.png"*/
	/*	3 CIRNO */	{	"jiki/shot_ci.png", 		0,	 1, PRIORITY_01_SHOT,	0 },	/*"plasma_ci.png"*/
	/*	4 YUYUKO */ {	"jiki/shot_yu.png", 		0,	 1, PRIORITY_01_SHOT,	0 },
//
	/*	5 REIMU */	{	"jiki/needle_re.png",		1,	 2, PRIORITY_01_SHOT,	1 },	//shot->anim_speed=1;
	/*	6 MARISA */ {	"jiki/needle_ma.png",		0,	 1, PRIORITY_01_SHOT,	0 },	//shot->anim_speed=0;
	/*	7 REMILIA */{	"jiki/needle_oz.png",		1,	 5, PRIORITY_01_SHOT,	3 },	//shot->anim_speed=3;	/* alpha==1に修正。 alpha==0なのにalpha出てる？？？*/
	/*	8 CIRNO */	{	"jiki/needle_ci.png",		0,	 1, PRIORITY_01_SHOT,	0 },	//shot->anim_speed=0;
	/*	9 YUYUKO */ {	"jiki/needle_yu.png",		0,	 1, PRIORITY_01_SHOT,	0 },	//shot->anim_speed=0;
//
	/* 10 REIMU */	{	"jiki/homing_re.png",		0,	 6, PRIORITY_01_SHOT,	0 },	/*"tshoot.png",*/
	/* 11 MARISA */ {	"jiki/homing_ma.png",		0,	 6, PRIORITY_01_SHOT,	0 },	/*"tshoot-ma.png"*/
	/* 12 REMILIA */{	"jiki/homing_oz.png",		0,	 6, PRIORITY_01_SHOT,	0 },	/*"tshoot-oz.png"*/
	/* 13 CIRNO */	{	"jiki/homing_ci.png",		0,	 6, PRIORITY_01_SHOT,	0 },	/*"tshoot-ci.png"*/
	/* 14 YUYUKO */ {	"jiki/homing_yu.png",		0,	 6, PRIORITY_01_SHOT,	0 },
//	/*自分*/
	/* 15 REIMU */	{	"jiki/jiki_re.png", 		0,	12, PRIORITY_02_PLAYER, 0 },	//"ship-med.png",		0,	 11,
	/* 16 MARISA */ {	"jiki/jiki_ma.png", 		0,	12, PRIORITY_02_PLAYER, 0 },	//"ship-med-ma.png",	0,	 11,
	/* 17 REMILIA */{	"jiki/jiki_oz.png", 		0,	12, PRIORITY_02_PLAYER, 0 },	//"ship-med-oz.png",	0,	 11,
	/* 18 CIRNO */	{	"jiki/jiki_ci.png", 		0,	12, PRIORITY_02_PLAYER, 0 },	//"ship-med-ci.png",	0,	 11,
	/* 19 YUYUKO */ {	"jiki/jiki_yu.png", 		0,	12, PRIORITY_02_PLAYER, 0 },
//	/*あたり判定*/
	/* 20 REIMU */	{	"jiki/core_re.png", 		0,	 1, PRIORITY_03_ENEMY,	0 },	//"core.png",
	/* 21 MARISA */ {	"jiki/core_ma.png", 		0,	 1, PRIORITY_03_ENEMY,	0 },	//"core-ma.png",
	/* 22 REMILIA */{	"jiki/core_oz.png", 		0,	 1, PRIORITY_03_ENEMY,	0 },	//"core-oz.png",
	/* 23 CIRNO */	{	"jiki/core_ci.png", 		0,	 1, PRIORITY_03_ENEMY,	0 },	//"core-ci.png",
	/* 24 YUYUKO */ {	"jiki/core_yu.png", 		0,	 1, PRIORITY_03_ENEMY,	0 },
//	/* オプション用素材 */
	/* 25 REIMU */	{	"jiki/option1_re.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"option.png"*/
	/* 26 MARISA */ {	"jiki/option1_ma.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"mari_op.png"*/
	/* 27 REMILIA */{	"jiki/option1_oz.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"oze_op1.png"*/
	/* 28 CIRNO */	{	"jiki/option1_ci.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"ci_op1.png"*/
	/* 29 YUYUKO */ {	"jiki/option1_yu.png",		0,	 8, PRIORITY_02_PLAYER, 0 },
//
	/* 30 REIMU */	{	"jiki/option2_re.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*[起動チェックで使うので有る名前で]*/
	/* 31 MARISA */ {	"jiki/option2_ma.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*[起動チェックで使うので有る名前で]*/
	/* 32 REMILIA */{	"jiki/option2_oz.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"oze_op2.png"*/
	/* 33 CIRNO */	{	"jiki/option2_ci.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"ci_op2.png"*/
	/* 34 YUYUKO */ {	"jiki/option2_yu.png",		0,	 8, PRIORITY_02_PLAYER, 0 },
//
	/* 35 REIMU */	{	"jiki/option3_re.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*[起動チェックで使うので有る名前で]*/
	/* 36 MARISA */ {	"jiki/option3_ma.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*[起動チェックで使うので有る名前で]*/
	/* 37 REMILIA */{	"jiki/option3_oz.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"oze_op3.png"*/
	/* 38 CIRNO */	{	"jiki/option3_ci.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"ci_op3.png"*/
	/* 39 YUYUKO */ {	"jiki/option3_yu.png",		0,	 8, PRIORITY_02_PLAYER, 0 },
//
	/* 40 REIMU */	{	"jiki/option4_re.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*[起動チェックで使うので有る名前で]*/
	/* 41 MARISA */ {	"jiki/option4_ma.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*[起動チェックで使うので有る名前で]*/
	/* 42 REMILIA */{	"jiki/option4_oz.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"oze_op4.png"*/
	/* 43 CIRNO */	{	"jiki/option4_ci.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"ci_op4.png"*/
	/* 44 YUYUKO */ {	"jiki/option4_yu.png",		0,	 8, PRIORITY_02_PLAYER, 0 },

//	/* ボム用素材 */
	/* 45 REIMU */	{	"jiki/bomber1_re.png",		1,	32, PRIORITY_03_ENEMY,	0 },	/*"cshoot1.png""cshoot.png"*/
	/* 46 MARISA */ {	"jiki/bomber1_ma.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"star_shield_blue.png"*/
	/* 47 REMILIA */{	"jiki/bomber1_oz.png",		0,	10, PRIORITY_01_SHOT,	3 },	/*"cross_red.png"*/
	/* 48 CIRNO */	{	"jiki/bomber1_ci.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"cirno_shield_blue.png"*/
	/* 49 YUYUKO */ {	"jiki/bomber1_yu.png",		0,	 3, PRIORITY_01_SHOT,	5 },
//
	/* 50 REIMU */	{	"jiki/bomber2_re.png",		1,	32, PRIORITY_03_ENEMY,	0 },	/*"cshoot2r.png""cshoot2.png"*/
	/* 51 MARISA */ {	"jiki/bomber2_ma.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"star_shields_red.png"*/
	/* 52 REMILIA */{	"jiki/bomber2_oz.png",	1/*0*/,  4, PRIORITY_01_SHOT,	3 },	/*"fire_wind_l.png"*/
	/* 53 CIRNO */	{	"jiki/bomber2_ci.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"cirno_shields_red.png"*/
	/* 54 YUYUKO */ {	"jiki/bomber2_yu.png",		0,	 3, PRIORITY_01_SHOT,	5 },
//
	/* 55 REIMU */	{	"jiki/bomber1_re.png",		1,	32, PRIORITY_03_ENEMY,	0 },	/*[起動チェックで使うので有る名前で]*/
	/* 56 MARISA */ {	"jiki/bomber3_ma.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"star_shield_green.png"*/
	/* 57 REMILIA */{	"jiki/bomber3_oz.png",	1/*0*/,  4, PRIORITY_01_SHOT,	3 },	/*"fire_wind_r.png"*/
	/* 58 CIRNO */	{	"jiki/bomber3_ci.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"cirno_shield_green.png"*/
	/* 59 YUYUKO */ {	"jiki/bomber3_yu.png",		0,	 3, PRIORITY_01_SHOT,	5 },
//
	/* 60 REIMU */	{	"jiki/bomber1_re.png",		1,	32, PRIORITY_03_ENEMY,	0 },	/*[起動チェックで使うので有る名前で]*/
	/* 61 MARISA */ {	"jiki/bomber4_ma.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"star_shields_blue.png"*/
	/* 62 REMILIA */{	"jiki/bomber4_oz.png",	1/*0*/,  4, PRIORITY_01_SHOT,	3 },	/*"fire_wind_u.png"*/
	/* 63 CIRNO */	{	"jiki/bomber4_ci.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"cirno_shields_blue.png"*/
	/* 64 YUYUKO */ {	"jiki/bomber4_yu.png",		0,	 3, PRIORITY_01_SHOT,	5 },
//
	/* 65 REIMU */	{	"jiki/bomber1_re.png",		1,	32, PRIORITY_03_ENEMY,	0 },	/*[起動チェックで使うので有る名前で]*/
	/* 66 MARISA */ {	"jiki/bomber5_ma.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"star_shield_red.png"*/
	/* 67 REMILIA */{	"jiki/bomber4_oz.png",	1/*0*/,  4, PRIORITY_01_SHOT,	3 },	/*[起動チェックで使うので有る名前で]*/
	/* 68 CIRNO */	{	"jiki/bomber5_ci.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"cirno_shield_red.png"*/
	/* 69 YUYUKO */ {	"jiki/bomber3_yu.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*[起動チェックで使うので有る名前で]*/
//
	/* 70 REIMU */	{	"jiki/bomber1_re.png",		1,	32, PRIORITY_03_ENEMY,	0 },	/*[起動チェックで使うので有る名前で]*/
	/* 71 MARISA */ {	"jiki/bomber6_ma.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"star_shields_green.png"*/
	/* 72 REMILIA */{	"jiki/bomber4_oz.png",	1/*0*/,  4, PRIORITY_01_SHOT,	3 },	/*[起動チェックで使うので有る名前で]*/
	/* 73 CIRNO */	{	"jiki/bomber6_ci.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"cirno_shields_green.png"*/
	/* 74 YUYUKO */ {	"jiki/bomber4_yu.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*[起動チェックで使うので有る名前で]*/
//
	/* 75 */		{	"jiki/bomber_slow.png", 	1,	 4, PRIORITY_01_SHOT,	0 },
	/* 76 */		{	"panel/bonus_items.png",	0,	 8, PRIORITY_04_ITEM,	0 },
//
	/* 77 */	//	{	"tama/kugel.png",			0,	 1, PRIORITY_05_BULLETS,	0 },
	/* 78 */	//	{	"tama/kugel2.png",			0,	 1, PRIORITY_05_BULLETS,	0 },
	/* 79 */		{	"tama/bullet_beam16.png",	0,	16, PRIORITY_05_BULLETS,	0 },
	/* 80 */		{	"tama/bullet_maru16.png",	0,	16, PRIORITY_05_BULLETS,	0 },
//
	/* 81 */		{	"tama/bullet_ming32.png",	0,	32, PRIORITY_05_BULLETS,	0 },
	/* 82 */		{	"tama/jippou32.png",		0,	32, PRIORITY_05_BULLETS,	0 },
//
	/* 83 */		{	"tama/oodama08.png",		1,	 8, PRIORITY_05_BULLETS,	0 },/* 大玉(黒青赤...)  黒玉(輪) PRIORITY_03_ENEMY は、あたり判定部分 */
	/* 83 */	//	{	"tama/bigkugel2.png",		0,	 1, PRIORITY_03_ENEMY,		0 },/* 黒玉(輪)  あたり判定部分*/
	/* 84 */	//	{	"tama/bigkugel1.png",		1,	 1, PRIORITY_05_BULLETS,	0 },/* 大玉(青) 表示部分 */
	/* 85 */	//	{	"tama/new_bigkugel.png",	1,	 1, PRIORITY_05_BULLETS,	0 },/* 大玉(赤) 表示部分*/
//
	/* 86 */		{	"tama/knife.png",			1,	 1, PRIORITY_05_BULLETS,	0 },/*垂直降下ナイフ(赤)*/
	/* 87 */		{	"tama/knife_core16.png",	1,	16, PRIORITY_05_BULLETS,	0 },/*全方向ナイフ(青)*/
//
	/* 88 */		{	"zako/mahoujin_0.png",		1/*0*/,  2, PRIORITY_01_SHOT,	0 },	/**/
	/* 89 */		{	"zako/tikei_bgpanel1.png",		0,	 1, PRIORITY_00_BG, 	0 },
	/* 90 */		{	"zako/tikei_bgpanel2.png",		0,	 1, PRIORITY_00_BG, 	0 },
	/* 91 */		{	"zako/tikei_grounder08.png",	0,	 8, PRIORITY_01_SHOT,	2 },
//
	/* 92 */		{	"zako/obake08.png", 			0,	 8, PRIORITY_03_ENEMY,	0 },	/**/
	/* 93 */		{	"zako/yukari32.png",			0,	32, PRIORITY_03_ENEMY,	0 },	/**/
	/* 94 */		{	"zako/aka_kedama08.png",		0,	 8, PRIORITY_03_ENEMY,	4 },	/*4 1*/
	/* 95 */		{	"zako/niji_kedama16.png",		0,	16, PRIORITY_03_ENEMY,	1 },	/**/
//
	/* 96 */		{	"zako/midoori_kedama16.png",	0,	16, PRIORITY_03_ENEMY,	4 },	/*4 1*/
	/* 97 */		{	"zako/kedama16.png",			0,	16, PRIORITY_03_ENEMY,	5 },	/**/
	/* 98 */		{	"zako/inyou1_16.png",			0,	16, PRIORITY_03_ENEMY,	0 },	/**/
	/* 99 */		{	"zako/tatsumaki16.png", 		0,	16, PRIORITY_03_ENEMY,	2 },	/**/
//
	/*100 */		{	"zako/great_fairy02.png",		0,	 2, PRIORITY_03_ENEMY,	0 },	/*-*/
	/*101 */		{	"zako/aka_meido08.png", 		0,	 8, PRIORITY_03_ENEMY,	0 },	/**/
	/*102 */		{	"zako/ao_yousei24.png", 		0,	24, PRIORITY_03_ENEMY,	0 },	/**/
	/*103 */		{	"zako/homing16.png",			0,	16, PRIORITY_05_BULLETS,0 },	/**/
//	/* 小爆発 */
	/*104 */		{	"effect/tr_blue.png",			0,	 6, PRIORITY_04_ITEM,	1 },
	/*105 */		{	"effect/tr_red.png",			0,	 6, PRIORITY_04_ITEM,	1 },
	/*106 */		{	"effect/tr_green.png",			0,	 6, PRIORITY_04_ITEM,	1 },
//	/* ザコ消滅爆発 */
	/*107 */		{	"effect/bakuha05.png",			1,	 5, PRIORITY_04_ITEM,	3 },
	/*108 */		{	"effect/bakuha06.png",			1,	 5, PRIORITY_04_ITEM,	3 },
	/*109 */		{	"effect/bakuha07.png",			1,	 5, PRIORITY_04_ITEM,	3 },
//	/* 火炎爆発 */
	/*110 */		{	"effect/ex.png",				0,	29, PRIORITY_04_ITEM,	3 },
//
	/*111 */		{	"boss/boss01_0.png",			0,	4, PRIORITY_03_ENEMY,	8 },
	/*112 */		{	"boss/boss01_1.png",			0,	8, PRIORITY_03_ENEMY,	0 },
	/*113 */		{	"boss/boss01_2.png",			0,	4, PRIORITY_03_ENEMY,	8 },
	/*114 */		{	"boss/boss01_3.png",			0,	2, PRIORITY_03_ENEMY,	8 },
	/*115 */		{	"boss/boss01_4.png",			0,	2, PRIORITY_03_ENEMY,	8 },
//
	/*116 */		{	"boss/aya.png", 				0,	9, PRIORITY_03_ENEMY,	0 },
//
	/*117 */		{	"boss/boss04_0.png",			0,	2, PRIORITY_03_ENEMY,	0 },
	/*118 */		{	"boss/boss04_1.png",			0,	2, PRIORITY_03_ENEMY,	0 },
	/*119 */		{	"boss/boss04_2.png",			0,	2, PRIORITY_03_ENEMY,	0 },
	/*120 */		{	"boss/boss04_3.png",			0,	2, PRIORITY_03_ENEMY,	0 },
	/*121 */		{	"boss/boss04_4.png",			0,	2, PRIORITY_03_ENEMY,	0 },
	/*122 */		{	"boss/boss04_5.png",			0,	2, PRIORITY_03_ENEMY,	0 },
//
	/*123 */		{	"boss/sakuya.png",				0,	19, PRIORITY_03_ENEMY,	0 },
//
	/*124 */	//	{	"panel/key_icon.png",			1,	12, PRIORITY_01_SHOT,	2 },
};


/*---------------------------------------------------------
	あたり判定 collision
---------------------------------------------------------*/

#if (1==USE_ZUKEI_ATARI_HANTEI)
/*---------------------------------------------------------
	図形あたり判定の子関数(単色図形であたり判定を判断)
---------------------------------------------------------*/

//static SDL_Rect rc;/* チェック用、矩形判定領域 */
static int rc_x;/* チェック用、矩形判定領域 */
static int rc_y;/* チェック用、矩形判定領域 */
#if 1
static int sprite_memory_and(Uint8 *s1, Uint8 *s2, unsigned int shift1, unsigned int shift2, int length)
{
	Uint16 i1 = (1<<shift1);
	Uint16 i2 = (1<<shift2);
	int b;
	for (b=0; b<length; b++)
	{
		if (( (*s1) & (i1) ) &&
			( (*s2) & (i2) )	)
		{
			return (1)/*(b+1)*/;/*あたった*/
		}
		i1 += i1;/*i1 <<= 1;*/		if (0x100 == i1) { i1 = 0x01; s1++; }
		i2 += i2;/*i2 <<= 1;*/		if (0x100 == i2) { i2 = 0x01; s2++; }
	}
	return (0);/*あたってない*/
}
#else
static int sprite_memory_and(Uint8 *s1, Uint8 *s2, unsigned int shift1, unsigned int shift2, int length)
{
	const Uint8 stbl[8] =
	{
		0x01, 0x02, 0x04, 0x08,
		0x10, 0x20, 0x40, 0x80,
	};
	int b;
	for (b=0; b<length; b++)
	{
		if (( (*s1) & (stbl[shift1]) ) &&
			( (*s2) & (stbl[shift2]) )	)
		{
			return (1)/*(b+1)*/;/*あたった*/
		}
		shift1++;	if (8 == shift1) { shift1 = 0; s1++; }
		shift2++;	if (8 == shift2) { shift2 = 0; s2++; }
	}
	return (0);/*あたってない*/
}
#endif

/*---------------------------------------------------------
	図形あたり判定
---------------------------------------------------------*/

static int sprite_collision_pixel(SPRITE *a, SPRITE *b)
{
	Sint16 w1 = a->w;
	Sint16 h1 = a->h;
	Sint16 x1 = ((a->x256)>>8);
	Sint16 y1 = ((a->y256)>>8);
	Sint16 w2 = b->w;
	Sint16 h2 = b->h;
	Sint16 x2 = ((b->x256)>>8);
	Sint16 y2 = ((b->y256)>>8);

	Sint32 x1o = 0;
	Sint32 x2o = 0;
	Sint32 y1o = 0;
	Sint32 y2o = 0;
	Sint32 offs;
	unsigned int i1 = 0;
	unsigned int i2 = 0;
//
	Sint16 y;
	Sint16 length;
	Uint8 *map1 = a->colision_bmp->col[a->anim_frame];
	Uint8 *map2 = b->colision_bmp->col[b->anim_frame];
//
	if ((rc_x==x2) && (rc_y==y2))
	{
		x1o = x2-x1;
		y1o = y2-y1;
		offs = (w1 * y1o) + x1o;
		map1 += (offs>>3);	i1 = (offs&(8-1));
	}
	else if ((rc_x==x2) && (rc_y==y1))
	{
		x1o = x2-x1;
		y2o = y1-y2;
		map1 += (x1o>>3);	i1 = (x1o&(8-1));
		offs = (w2 * y2o);
		map2 += (offs>>3);	i2 = (offs&(8-1));
	}
	else if ((rc_x==x1) && (rc_y==y1))
	{
		x2o = x1-x2;
		y2o = y1-y2;
		offs = (w2 * y2o) + x2o;
		map2 += (offs>>3);	i2 = (offs&(8-1));
	}
	else if ((rc_x==x1) && (rc_y==y2))
	{
		y1o = y2-y1;
		x2o = x1-x2;
		offs = (w1 * y1o);
		map1 += (offs>>3);	i1 = (offs&(8-1));
		map2 += (x2o>>3);	i2 = (x2o&(8-1));
	}
	else
	{
		return (0);/*あたってない*/
	}
//
	if (x1+w1 < x2+w2)
	{
		length = w1-x1o;
	}
	else
	{
		length = w2-x2o;
	}
//
	for (y=rc_y; ((y<=y1+h1) && (y<=y2+h2)); y++)
	{
		if (/*offs offs =*/ sprite_memory_and(map1, map2, i1, i2, length) )
		{
			// col-koord: x=rc_x+offs-1, y=y
			return (1);/*あたった*/
		}
		offs = ((y-y1) * w1) + x1o;
		map1 = a->colision_bmp->col[a->anim_frame];
		map1 += (offs>>3);
		i1 = (offs&(8-1));
		//
		offs = ((y-y2) * w2) + x2o;
		map2 = b->colision_bmp->col[b->anim_frame];
		map2 += (offs>>3);
		i2 = (offs&(8-1));
	}
	return (0);/*あたってない*/
}
#endif
/*---------------------------------------------------------
	矩形あたり判定
---------------------------------------------------------*/

static int sprite_collision_bounding(SPRITE *a, SPRITE *b)
{
	if (a->x256 < b->x256)
	{
		if (a->x256+((a->w)<<8) > b->x256)
		{
			if (a->y256 < b->y256)
			{	if (a->y256+((a->h)<<8) > b->y256)
				{
					#if (1==USE_ZUKEI_ATARI_HANTEI)
					rc_x=((b->x256)>>8);	rc_y=((b->y256)>>8);
					#endif
					return (1);/*あたった*/
				}
			}
			else
			{	if (b->y256+((b->h)<<8) > a->y256)
				{
					#if (1==USE_ZUKEI_ATARI_HANTEI)
					rc_x=((b->x256)>>8);	rc_y=((a->y256)>>8);
					#endif
					return (1);/*あたった*/
				}
			}
		}
	}
	else
	{
		if (b->x256+((b->w)<<8) > a->x256)
		{
			if (b->y256 < a->y256)
			{	if (b->y256+((b->h)<<8) > a->y256)
				{
					#if (1==USE_ZUKEI_ATARI_HANTEI)
					rc_x=((a->x256)>>8);	rc_y=((a->y256)>>8);
					#endif
					return (1);/*あたった*/
			}	}
			else
			{	if (a->y256+((a->h)<<8) > b->y256)
				{
					#if (1==USE_ZUKEI_ATARI_HANTEI)
					rc_x=((a->x256)>>8);	rc_y=((b->y256)>>8);
					#endif
					return (1);/*あたった*/
			}	}
		}
	}
	return (0);/*あたってない*/
}

/*---------------------------------------------------------
	スプライトのあたり判定
---------------------------------------------------------*/

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
					/* 4. 矩形あたり判定チェック */
					if (sprite_collision_bounding(s,tocheck))	/* 矩形あたり判定 */	/* Bounding Box Collosion? */
					{
						/* 5. 図形あたり判定チェック */
						#if (1==USE_ZUKEI_ATARI_HANTEI)
						if (sprite_collision_pixel(s,tocheck))	/* 図形あたり判定 */	/* Pixel-Level Collison */
						#endif
						{
							return (s);/*あたった*/
						}
					}
				}
			}
		}
		s = s->next;/*次*/
	}
	return (NULL);/*あたってない*/
}

#if (1==USE_ZUKEI_ATARI_HANTEI)

/*---------------------------------------------------------
	図形あたり判定用の図形を削除
---------------------------------------------------------*/

static void sprite_remove_colision_map(COLISION_MAP_CACHE *c)
{
	if (c->refcount > 0)
	{
		c->refcount--;
	}
	else
	{
		CHECKPOINT;
		error(ERR_WARN,"COLISION_MAP_CACHE refcount already zero");
	}
}

/*---------------------------------------------------------
	図形あたり判定用の図形を渡す
---------------------------------------------------------*/

static COLISION_MAP_CACHE *colmap_cache = NULL;/* 図形あたり判定用の図形キャッシュリスト */

static COLISION_MAP_CACHE *sprite_get_colision_map(SDL_Surface *img, int frames)
{
	COLISION_MAP_CACHE *c = colmap_cache;
	while (NULL != c)
	{
		if ((c->src==img) && (c->frames==frames))
		{
			return (c);
		}
		c = c->next;
	}
	return (NULL);
}

/*---------------------------------------------------------
	図形あたり判定用の図形を新規作成の子関数
---------------------------------------------------------*/

static Uint32 getpixel16(SDL_Surface *surface, int x, int y)
{
	Uint8 *p = (Uint8 *)surface->pixels+y*surface->pitch+x+x;
	return (*(Uint16 *)p);
}

static void sprite_create_colision_map(SDL_Surface *src, Uint8 *dst, int w, int f)
{
	if (SDL_MUSTLOCK(src)) { SDL_LockSurface(src); }
	{
		const Uint32 color_key16		= src->format->colorkey;
		Sint16 xstart;		xstart		= (w * (f));
		Sint16 xstop;		xstop		= (xstart)+w;
		Sint16 bit = 0x01;
		Sint16 y;
		for (y=0; y<(src->h); y++)
		{
			Sint16 x;
			for (x=xstart; x<xstop; x++)
			{
				if (getpixel16(src,x,y) != color_key16)
				{
					*dst = (*dst) | bit;
				}
				bit += bit/*bit <<= 1*/;	if (0x100 == bit) { bit = 0x01; dst++; }
			}
		}
	}
	if (SDL_MUSTLOCK(src)) { SDL_UnlockSurface(src); }
}

/*---------------------------------------------------------
	図形あたり判定用の図形を新規作成(図形あたり判定用の図形キャッシュリストに追加)
---------------------------------------------------------*/

static COLISION_MAP_CACHE *sprite_add_colision_map(SDL_Surface *img, int frames)
{
	COLISION_MAP_CACHE *c;
	int i;
	int colbsize;

	if (NULL != (c = sprite_get_colision_map(img, frames)))
	{
		c->refcount++;
		return (c);
	}

	c				= mmalloc(sizeof(COLISION_MAP_CACHE));
	c->src			= img;
	c->frames		= frames;
	c->refcount 	= 1;
	c->col			= mmalloc(sizeof(Uint8 *)*frames);

	colbsize		= (((img->w/frames)*(img->h+2))>>3)/*/8*/;

	for (i=0;i<frames;i++)
	{
		c->col[i]	= mmalloc(colbsize);
		memset(c->col[i],0x00,(colbsize));
		sprite_create_colision_map(img, c->col[i], (img->w/frames), i);
	}

	if (colmap_cache == NULL)
	{
		c->next = NULL;
	}
	else
	{
		c->next = colmap_cache;
	}
	colmap_cache = c;
	return (c);
}
#endif /* (1==USE_ZUKEI_ATARI_HANTEI) */

/*---------------------------------------------------------
	スプライト リストに登録されたスプライトを消す。
---------------------------------------------------------*/

static void sprite_remove_one(SPRITE *src)
{
	SPRITE *s = sprite_list000_head;/* スプライト リストの先頭 から探す */
	SPRITE *p = NULL;
	SPRITE *n = NULL;
	while (NULL != s)/* スプライト リストの終わりまで探す */
	{
		n = s->next;
		if (s == src)/*見つかった？*/
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
			return;/*正常*/
		}
		p = s;
		s = n;
	}
	CHECKPOINT;/*異常*/
	error(ERR_WARN,"sprite not found in list");
}

/*---------------------------------------------------------
	スプライト リストに登録されたスプライトを全部消す。
---------------------------------------------------------*/

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
---------------------------------------------------------*/

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
				/*逆転アニメ禁止に変更*/
			//	if (s->anim_speed != 0)
				if (0 < s->anim_speed)
				{
					s->anim_count++;			//		s->anim_count++/*=fps_fa ctor*/;
					// if (s->anim_count==abs(s->anim_speed))
					if (s->anim_count >= /*abs*/(s->anim_speed))
					{
						s->anim_count = 0;
					//	if (s->anim_speed > 0)
						{
							s->anim_frame++;
							if (s->anim_frame >= (s->frames))
							{	s->anim_frame = 0;}
						}
					//	else		/*逆転アニメ禁止に変更*/
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
				if ( 1000 < (s->timeover_ticks) )/*16秒*/	/*(1024-1) <*/ /*(0x0400 & (s->ticks))*/
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
				#if 0
				//	(s->priority==PRIORITY_03_ENEMY || s->priority== PRIORITY_05_BULLETS) && /* 敵か敵の武器の場合で */
					(s->type >= SP_GROUP_ENEMYS) &&
					(s->type <= SP_GROUP_BULLETS) && /* 敵か敵の武器の場合で */
					#if 0
					s->type!=SP_BOSS01 &&	/* 各ボス以外で */
					s->type!=SP_BOSS02 &&
					s->type!=SP_BOSS03 &&
					s->type!=SP_BOSS04 &&
					#else
					(s->type!=SP_BOSS) &&		/* 各ボス以外で */
					#endif
					(s->type!=SP_MENU_TEXT) 		/* メニュー以外の場合、 */
				#endif

/*---------------------------------------------------------
	スプライトを表示する。
---------------------------------------------------------*/
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
	SDL_Rect t;
	t.y=0;
	while (NULL != s)
	{
		#if (1==USE_DEBUG)
		if (s->type != SP_DELETE )
		{
			debug_count1++;/*有効 スプライト カウント*/
			//	max 	debug_count2/debug_count1			/*	*/
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
				debug_count2++;/*有効 スプライト カウント*/
			}
		}
		#endif
		#if 0
		if (s->type != SP_DELETE ) /* 削除済みは飛ばす */	//	SP_DELETEが 0 になったので要らなくなった．
		#endif
		{
			if (s->type & type) /* typeが一致する場合のみ */
			{
				if ((s->flags&SP_FLAG_VISIBLE)) /* 表示可能な場合のみ */
				{
					SDL_Rect r;
					r.x = ((s->x256)>>8);
					r.y = ((s->y256)>>8);
					r.w = s->w;
					r.h = s->h;
//
					t.w = s->w;
					t.h = s->h;
					t.x = s->w*s->anim_frame;
//
//					if (s->flags&SP_FLAG_CHEAP_ALPHA)
//					{
//						blit_cheap_alpha(s->sprite_bmp,&t,screen,&r);
//					}
//					else
//					{
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
						SDL_BlitSurface(s->sprite_bmp,&t,screen,&r);
//					}
				}
			}
		}
		s=s->next;/*次*/
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
---------------------------------------------------------*/

SPRITE *sprite_add_000(SDL_Surface *surface, int frames, Uint8 priority, int set_flags/*nocache*/, int anime_speed)
{
	SPRITE *obj;/* 新規作成するスプライト */
	SPRITE *s;
	SPRITE *tail_obj/*=NULL*/;	/* 追加するリストの候補位置。切断する場合は切断位置。 */
	obj 		= mmalloc(sizeof(SPRITE));
	s			= sprite_list000_head;	// スプライトリストの先頭。呼び出し
	tail_obj	= NULL;
	/*---------------------------------------------------------
		スプライトの表示プライオリティーを満たす位置を調べて、そこに挿入する。
	---------------------------------------------------------*/
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
	---------------------------------------------------------*/
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
	obj->w				= surface->w/frames;
	obj->h				= surface->h;

	#if (1==USE_ZUKEI_ATARI_HANTEI)
	obj->colision_bmp		= sprite_add_colision_map(surface, frames);
	#endif /* (1==USE_ZUKEI_ATARI_HANTEI) */

	obj->frames 	= frames;
	obj->priority	= priority;
	obj->flags		= set_flags; /*0;	if (nocache) s->flags |= SP_FLAG_NOT_CACHE;*/
	obj->alpha		= 255;

	obj->anim_frame = 0;
	obj->anim_speed = anime_speed/*0*/;
	obj->anim_count = 0;

	obj->timeover_ticks 	= 0;
//
	obj->data				= NULL;
	obj->callback_mover 	= NULL;
	obj->callback_loser 	= NULL;
	return (obj);
}

/* clouds敵の場合、ファイル名 */
/*static*/ SPRITE *sprite_add_internal_000(char *filename, int frames, Uint8 priority, int use_alpha, int anime_speed)
{
	SDL_Surface *s;
	SPRITE *sp;
	s		= loadbmp0(filename, use_alpha, 1);
	if (0 == use_alpha)
	{
		SDL_SetColorKey(s,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	}
	sp		= sprite_add_000(s, frames, priority, 0/*flags*/, anime_speed);
	return (sp);
}
SPRITE *sprite_add_res(int image_resource_num)
{
	char *fff;	fff = (char *)my_resource[image_resource_num].file_name;
	int aaa;	aaa = my_resource[image_resource_num].frames;
	int bbb;	bbb = my_resource[image_resource_num].priority;
	int ccc;	ccc = my_resource[image_resource_num].use_alpha;
	int ddd;	ddd = my_resource[image_resource_num].anime_speed;
	return (sprite_add_internal_000( fff, aaa, bbb, ccc, ddd));
}

/*---------------------------------------------------------
	画像キャッシュに載せる(ゲーム中に画像展開すると処理落ちが激しくて
	ゲームにならない。そこで予めキャッシュに載せとく)
---------------------------------------------------------*/

/*static*/ void load_bg2_chache(char *filename, int use_alpha)
{
	SDL_Surface *tmp;
	tmp = loadbmp0(filename, /*0*/use_alpha, 1);
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
---------------------------------------------------------*/
extern void load_bg2_chache(char *filename, int use_alpha);
void preload_gfx(void)
{
	int image_resource_num;
	for (image_resource_num=0; image_resource_num<(FILE_RESOURCE_MAX); image_resource_num++)
	{
		char *fff;	fff = (char *)my_resource[image_resource_num].file_name;
		int ccc;	ccc = my_resource[image_resource_num].use_alpha;
		load_bg2_chache(fff/*char *filename*/, ccc /*use_alpha*/);
	}
}

//SPRITE *sprite_add_file 0(char *filename, int frames, Uint8 priority, int use_alpha)
//{
//	SDL_Surface *s;
//	SPRITE *sp;
//	s		= loadbmp0(filename, use_alpha, 1);
//	if (0 == use_alpha)
//	{
//		SDL_SetColorKey(s,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
//	}
//	sp		= sprite_add_000(s, frames, priority, 0/*flags*/, 0);
//	return (sp);
//}
//SPRITE *sprite_add_file1(char *filename, int frames, Uint8 priority)
//{
//	return (sprite_add_file 0( filename, frames,  priority, 0));
//}
//SPRITE *sprite_add_file2(char *filename, int frames, Uint8 priority)
//{
//	return sprite_add_file 0( filename, frames,  priority, 1);
//}
//SPRITE *sprite_add_res(int image_resource_num)
//{
//	char *fff;	fff = (char *)my_resource[image_resource_num].file_name;
//	int aaa;	aaa = my_resource[image_resource_num].frames;
//	int bbb;	bbb = my_resource[image_resource_num].priority;
//	int ccc;	ccc = my_resource[image_resource_num].use_alpha;
//	return (sprite_add_file 0( fff, aaa, bbb, ccc));
//}
