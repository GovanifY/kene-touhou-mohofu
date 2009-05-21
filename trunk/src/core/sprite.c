
/*---------------------------------------------------------
	�X�v���C�g �}�l�[�W��
---------------------------------------------------------*/

#include "support.h"

/*---------------------------------------------------------
	���\�[�X resource
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
{	/*�V���b�g*/
	/*	0 REIMU */	{	"jiki/shot_re.png", 		0,	 1, PRIORITY_01_SHOT,	0 },	/*"plasma.png"*/
	/*	1 MARISA */ {	"jiki/shot_ma.png", 		0,	 1, PRIORITY_01_SHOT,	0 },	/*"plasma_ma.png"*/
	/*	2 REMILIA */{	"jiki/shot_oz.png", 		0,	 1, PRIORITY_01_SHOT,	0 },	/*"plasma_oz.png"*/
	/*	3 CIRNO */	{	"jiki/shot_ci.png", 		0,	 1, PRIORITY_01_SHOT,	0 },	/*"plasma_ci.png"*/
	/*	4 YUYUKO */ {	"jiki/shot_yu.png", 		0,	 1, PRIORITY_01_SHOT,	0 },
//
	/*	5 REIMU */	{	"jiki/needle_re.png",		1,	 2, PRIORITY_01_SHOT,	1 },	//shot->anim_speed=1;
	/*	6 MARISA */ {	"jiki/needle_ma.png",		0,	 1, PRIORITY_01_SHOT,	0 },	//shot->anim_speed=0;
	/*	7 REMILIA */{	"jiki/needle_oz.png",		1,	 5, PRIORITY_01_SHOT,	3 },	//shot->anim_speed=3;	/* alpha==1�ɏC���B alpha==0�Ȃ̂�alpha�o�Ă�H�H�H*/
	/*	8 CIRNO */	{	"jiki/needle_ci.png",		0,	 1, PRIORITY_01_SHOT,	0 },	//shot->anim_speed=0;
	/*	9 YUYUKO */ {	"jiki/needle_yu.png",		0,	 1, PRIORITY_01_SHOT,	0 },	//shot->anim_speed=0;
//
	/* 10 REIMU */	{	"jiki/homing_re.png",		0,	 6, PRIORITY_01_SHOT,	0 },	/*"tshoot.png",*/
	/* 11 MARISA */ {	"jiki/homing_ma.png",		0,	 6, PRIORITY_01_SHOT,	0 },	/*"tshoot-ma.png"*/
	/* 12 REMILIA */{	"jiki/homing_oz.png",		0,	 6, PRIORITY_01_SHOT,	0 },	/*"tshoot-oz.png"*/
	/* 13 CIRNO */	{	"jiki/homing_ci.png",		0,	 6, PRIORITY_01_SHOT,	0 },	/*"tshoot-ci.png"*/
	/* 14 YUYUKO */ {	"jiki/homing_yu.png",		0,	 6, PRIORITY_01_SHOT,	0 },
//	/*����*/
	/* 15 REIMU */	{	"jiki/jiki_re.png", 		0,	12, PRIORITY_02_PLAYER, 0 },	//"ship-med.png",		0,	 11,
	/* 16 MARISA */ {	"jiki/jiki_ma.png", 		0,	12, PRIORITY_02_PLAYER, 0 },	//"ship-med-ma.png",	0,	 11,
	/* 17 REMILIA */{	"jiki/jiki_oz.png", 		0,	12, PRIORITY_02_PLAYER, 0 },	//"ship-med-oz.png",	0,	 11,
	/* 18 CIRNO */	{	"jiki/jiki_ci.png", 		0,	12, PRIORITY_02_PLAYER, 0 },	//"ship-med-ci.png",	0,	 11,
	/* 19 YUYUKO */ {	"jiki/jiki_yu.png", 		0,	12, PRIORITY_02_PLAYER, 0 },
//	/*�����蔻��*/
	/* 20 REIMU */	{	"jiki/core_re.png", 		0,	 1, PRIORITY_03_ENEMY,	0 },	//"core.png",
	/* 21 MARISA */ {	"jiki/core_ma.png", 		0,	 1, PRIORITY_03_ENEMY,	0 },	//"core-ma.png",
	/* 22 REMILIA */{	"jiki/core_oz.png", 		0,	 1, PRIORITY_03_ENEMY,	0 },	//"core-oz.png",
	/* 23 CIRNO */	{	"jiki/core_ci.png", 		0,	 1, PRIORITY_03_ENEMY,	0 },	//"core-ci.png",
	/* 24 YUYUKO */ {	"jiki/core_yu.png", 		0,	 1, PRIORITY_03_ENEMY,	0 },
//	/* �I�v�V�����p�f�� */
	/* 25 REIMU */	{	"jiki/option1_re.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"option.png"*/
	/* 26 MARISA */ {	"jiki/option1_ma.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"mari_op.png"*/
	/* 27 REMILIA */{	"jiki/option1_oz.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"oze_op1.png"*/
	/* 28 CIRNO */	{	"jiki/option1_ci.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"ci_op1.png"*/
	/* 29 YUYUKO */ {	"jiki/option1_yu.png",		0,	 8, PRIORITY_02_PLAYER, 0 },
//
	/* 30 REIMU */	{	"jiki/option2_re.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*[�N���`�F�b�N�Ŏg���̂ŗL�閼�O��]*/
	/* 31 MARISA */ {	"jiki/option2_ma.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*[�N���`�F�b�N�Ŏg���̂ŗL�閼�O��]*/
	/* 32 REMILIA */{	"jiki/option2_oz.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"oze_op2.png"*/
	/* 33 CIRNO */	{	"jiki/option2_ci.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"ci_op2.png"*/
	/* 34 YUYUKO */ {	"jiki/option2_yu.png",		0,	 8, PRIORITY_02_PLAYER, 0 },
//
	/* 35 REIMU */	{	"jiki/option3_re.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*[�N���`�F�b�N�Ŏg���̂ŗL�閼�O��]*/
	/* 36 MARISA */ {	"jiki/option3_ma.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*[�N���`�F�b�N�Ŏg���̂ŗL�閼�O��]*/
	/* 37 REMILIA */{	"jiki/option3_oz.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"oze_op3.png"*/
	/* 38 CIRNO */	{	"jiki/option3_ci.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"ci_op3.png"*/
	/* 39 YUYUKO */ {	"jiki/option3_yu.png",		0,	 8, PRIORITY_02_PLAYER, 0 },
//
	/* 40 REIMU */	{	"jiki/option4_re.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*[�N���`�F�b�N�Ŏg���̂ŗL�閼�O��]*/
	/* 41 MARISA */ {	"jiki/option4_ma.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*[�N���`�F�b�N�Ŏg���̂ŗL�閼�O��]*/
	/* 42 REMILIA */{	"jiki/option4_oz.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"oze_op4.png"*/
	/* 43 CIRNO */	{	"jiki/option4_ci.png",		0,	 8, PRIORITY_02_PLAYER, 0 },	/*"ci_op4.png"*/
	/* 44 YUYUKO */ {	"jiki/option4_yu.png",		0,	 8, PRIORITY_02_PLAYER, 0 },

//	/* �{���p�f�� */
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
	/* 55 REIMU */	{	"jiki/bomber1_re.png",		1,	32, PRIORITY_03_ENEMY,	0 },	/*[�N���`�F�b�N�Ŏg���̂ŗL�閼�O��]*/
	/* 56 MARISA */ {	"jiki/bomber3_ma.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"star_shield_green.png"*/
	/* 57 REMILIA */{	"jiki/bomber3_oz.png",	1/*0*/,  4, PRIORITY_01_SHOT,	3 },	/*"fire_wind_r.png"*/
	/* 58 CIRNO */	{	"jiki/bomber3_ci.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"cirno_shield_green.png"*/
	/* 59 YUYUKO */ {	"jiki/bomber3_yu.png",		0,	 3, PRIORITY_01_SHOT,	5 },
//
	/* 60 REIMU */	{	"jiki/bomber1_re.png",		1,	32, PRIORITY_03_ENEMY,	0 },	/*[�N���`�F�b�N�Ŏg���̂ŗL�閼�O��]*/
	/* 61 MARISA */ {	"jiki/bomber4_ma.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"star_shields_blue.png"*/
	/* 62 REMILIA */{	"jiki/bomber4_oz.png",	1/*0*/,  4, PRIORITY_01_SHOT,	3 },	/*"fire_wind_u.png"*/
	/* 63 CIRNO */	{	"jiki/bomber4_ci.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"cirno_shields_blue.png"*/
	/* 64 YUYUKO */ {	"jiki/bomber4_yu.png",		0,	 3, PRIORITY_01_SHOT,	5 },
//
	/* 65 REIMU */	{	"jiki/bomber1_re.png",		1,	32, PRIORITY_03_ENEMY,	0 },	/*[�N���`�F�b�N�Ŏg���̂ŗL�閼�O��]*/
	/* 66 MARISA */ {	"jiki/bomber5_ma.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"star_shield_red.png"*/
	/* 67 REMILIA */{	"jiki/bomber4_oz.png",	1/*0*/,  4, PRIORITY_01_SHOT,	3 },	/*[�N���`�F�b�N�Ŏg���̂ŗL�閼�O��]*/
	/* 68 CIRNO */	{	"jiki/bomber5_ci.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"cirno_shield_red.png"*/
	/* 69 YUYUKO */ {	"jiki/bomber3_yu.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*[�N���`�F�b�N�Ŏg���̂ŗL�閼�O��]*/
//
	/* 70 REIMU */	{	"jiki/bomber1_re.png",		1,	32, PRIORITY_03_ENEMY,	0 },	/*[�N���`�F�b�N�Ŏg���̂ŗL�閼�O��]*/
	/* 71 MARISA */ {	"jiki/bomber6_ma.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"star_shields_green.png"*/
	/* 72 REMILIA */{	"jiki/bomber4_oz.png",	1/*0*/,  4, PRIORITY_01_SHOT,	3 },	/*[�N���`�F�b�N�Ŏg���̂ŗL�閼�O��]*/
	/* 73 CIRNO */	{	"jiki/bomber6_ci.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*"cirno_shields_green.png"*/
	/* 74 YUYUKO */ {	"jiki/bomber4_yu.png",		0,	 3, PRIORITY_01_SHOT,	5 },	/*[�N���`�F�b�N�Ŏg���̂ŗL�閼�O��]*/
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
	/* 83 */		{	"tama/oodama08.png",		1,	 8, PRIORITY_05_BULLETS,	0 },/* ���(����...)  ����(��) PRIORITY_03_ENEMY �́A�����蔻�蕔�� */
	/* 83 */	//	{	"tama/bigkugel2.png",		0,	 1, PRIORITY_03_ENEMY,		0 },/* ����(��)  �����蔻�蕔��*/
	/* 84 */	//	{	"tama/bigkugel1.png",		1,	 1, PRIORITY_05_BULLETS,	0 },/* ���(��) �\������ */
	/* 85 */	//	{	"tama/new_bigkugel.png",	1,	 1, PRIORITY_05_BULLETS,	0 },/* ���(��) �\������*/
//
	/* 86 */		{	"tama/knife.png",			1,	 1, PRIORITY_05_BULLETS,	0 },/*�����~���i�C�t(��)*/
	/* 87 */		{	"tama/knife_core16.png",	1,	16, PRIORITY_05_BULLETS,	0 },/*�S�����i�C�t(��)*/
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
//	/* ������ */
	/*104 */		{	"effect/tr_blue.png",			0,	 6, PRIORITY_04_ITEM,	1 },
	/*105 */		{	"effect/tr_red.png",			0,	 6, PRIORITY_04_ITEM,	1 },
	/*106 */		{	"effect/tr_green.png",			0,	 6, PRIORITY_04_ITEM,	1 },
//	/* �U�R���Ŕ��� */
	/*107 */		{	"effect/bakuha05.png",			1,	 5, PRIORITY_04_ITEM,	3 },
	/*108 */		{	"effect/bakuha06.png",			1,	 5, PRIORITY_04_ITEM,	3 },
	/*109 */		{	"effect/bakuha07.png",			1,	 5, PRIORITY_04_ITEM,	3 },
//	/* �Ή����� */
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
	�����蔻�� collision
---------------------------------------------------------*/

#if (1==USE_ZUKEI_ATARI_HANTEI)
/*---------------------------------------------------------
	�}�`�����蔻��̎q�֐�(�P�F�}�`�ł����蔻��𔻒f)
---------------------------------------------------------*/

//static SDL_Rect rc;/* �`�F�b�N�p�A��`����̈� */
static int rc_x;/* �`�F�b�N�p�A��`����̈� */
static int rc_y;/* �`�F�b�N�p�A��`����̈� */
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
			return (1)/*(b+1)*/;/*��������*/
		}
		i1 += i1;/*i1 <<= 1;*/		if (0x100 == i1) { i1 = 0x01; s1++; }
		i2 += i2;/*i2 <<= 1;*/		if (0x100 == i2) { i2 = 0x01; s2++; }
	}
	return (0);/*�������ĂȂ�*/
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
			return (1)/*(b+1)*/;/*��������*/
		}
		shift1++;	if (8 == shift1) { shift1 = 0; s1++; }
		shift2++;	if (8 == shift2) { shift2 = 0; s2++; }
	}
	return (0);/*�������ĂȂ�*/
}
#endif

/*---------------------------------------------------------
	�}�`�����蔻��
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
		return (0);/*�������ĂȂ�*/
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
			return (1);/*��������*/
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
	return (0);/*�������ĂȂ�*/
}
#endif
/*---------------------------------------------------------
	��`�����蔻��
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
					return (1);/*��������*/
				}
			}
			else
			{	if (b->y256+((b->h)<<8) > a->y256)
				{
					#if (1==USE_ZUKEI_ATARI_HANTEI)
					rc_x=((b->x256)>>8);	rc_y=((a->y256)>>8);
					#endif
					return (1);/*��������*/
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
					return (1);/*��������*/
			}	}
			else
			{	if (a->y256+((a->h)<<8) > b->y256)
				{
					#if (1==USE_ZUKEI_ATARI_HANTEI)
					rc_x=((a->x256)>>8);	rc_y=((b->y256)>>8);
					#endif
					return (1);/*��������*/
			}	}
		}
	}
	return (0);/*�������ĂȂ�*/
}

/*---------------------------------------------------------
	�X�v���C�g�̂����蔻��
---------------------------------------------------------*/

SPRITE *sprite_list000_head = NULL; 	// �X�v���C�g�̃��X�g�\��

SPRITE *sprite_collision_check(SPRITE *tocheck, int type)
{
	SPRITE *s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	while (NULL != s)
	{
		/* 1. �������g�`�F�b�N */
		/* ����Ώ�(�������g)�͔�΂�(�S���������̓��m�̂����蔻�������Ă��Ӗ����Ȃ��̂Ŏ��Ȃ�) / zu testendes Sprite ueberspringen */
		if (s != tocheck)
		{
			/* 2. �����ރ`�F�b�N */
			if (
				//		(s->type != SP_DELETE ) && /* �폜�ς݂͔�΂� */	//	SP_DELETE�� 0 �ɂȂ����̂ŗv��Ȃ��Ȃ����D
						(s->type & type)			 /* type����v����ꍇ�̂� */	/* Passender Sprite-Type? */
				)
			{
				/* 3. ����t���O�`�F�b�N */
				if (
						( (SP_FLAG_COLISION_CHECK|SP_FLAG_VISIBLE) == (s->flags&(SP_FLAG_COLISION_CHECK|SP_FLAG_VISIBLE)))
						/* �����蔻�肪����A���A�\���\�Ȃ��̂̂݃`�F�b�N */
					)
				{
					/* 4. ��`�����蔻��`�F�b�N */
					if (sprite_collision_bounding(s,tocheck))	/* ��`�����蔻�� */	/* Bounding Box Collosion? */
					{
						/* 5. �}�`�����蔻��`�F�b�N */
						#if (1==USE_ZUKEI_ATARI_HANTEI)
						if (sprite_collision_pixel(s,tocheck))	/* �}�`�����蔻�� */	/* Pixel-Level Collison */
						#endif
						{
							return (s);/*��������*/
						}
					}
				}
			}
		}
		s = s->next;/*��*/
	}
	return (NULL);/*�������ĂȂ�*/
}

#if (1==USE_ZUKEI_ATARI_HANTEI)

/*---------------------------------------------------------
	�}�`�����蔻��p�̐}�`���폜
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
	�}�`�����蔻��p�̐}�`��n��
---------------------------------------------------------*/

static COLISION_MAP_CACHE *colmap_cache = NULL;/* �}�`�����蔻��p�̐}�`�L���b�V�����X�g */

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
	�}�`�����蔻��p�̐}�`��V�K�쐬�̎q�֐�
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
	�}�`�����蔻��p�̐}�`��V�K�쐬(�}�`�����蔻��p�̐}�`�L���b�V�����X�g�ɒǉ�)
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
	�X�v���C�g ���X�g�ɓo�^���ꂽ�X�v���C�g�������B
---------------------------------------------------------*/

static void sprite_remove_one(SPRITE *src)
{
	SPRITE *s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	SPRITE *p = NULL;
	SPRITE *n = NULL;
	while (NULL != s)/* �X�v���C�g ���X�g�̏I���܂ŒT�� */
	{
		n = s->next;
		if (s == src)/*���������H*/
		{
			if (NULL == p)
			{
				sprite_list000_head = n;	// �X�v���C�g���X�g�̐擪������������B
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
				/* KETM�ł����͖����ɂȂ��Ă邪�A
				�����������ɂȂ��Ă����(�_���\���p�t�H���g��)
				���X���C����������������Ⴄ�̂ŁA
				�J�����Ă݂�e�X�g */
				/* �����Ńn���O�A�b�v */
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
			return;/*����*/
		}
		p = s;
		s = n;
	}
	CHECKPOINT;/*�ُ�*/
	error(ERR_WARN,"sprite not found in list");
}

/*---------------------------------------------------------
	�X�v���C�g ���X�g�ɓo�^���ꂽ�X�v���C�g��S�������B
---------------------------------------------------------*/

//void sprite_remove_all222(int type){}
void sprite_remove_all000(int type)
{
	SPRITE *s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	SPRITE *n = NULL;
	while (NULL != s)
	{
		n = s->next;
		if (s->type & type) /* type����v����ꍇ�̂� */
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
	�X�v���C�g����t���[�������삳����B
---------------------------------------------------------*/

//void sprite_work222(int type){}
void sprite_work000(int type)
{
	SPRITE *s;
	SPRITE *n;
	s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	while (NULL != s)
	{
		n = s->next;
		#if 0
		if (s->type != SP_DELETE ) /* �폜�ς݂͔�΂� */	//	SP_DELETE�� 0 �ɂȂ����̂ŗv��Ȃ��Ȃ����D
		#endif
		{
			/* animate */
			//s->ticks++;
			if (s->type & type) /* type����v����ꍇ�̂� */
			{
				/*�t�]�A�j���֎~�ɕύX*/
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
					//	else		/*�t�]�A�j���֎~�ɕύX*/
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
	/* �g�p�ς݃X�v���C�g��T���āA�g�p�ς݃X�v���C�g������Ώ��� */
	if ( ST_WORK_MENU != (psp_loop&0xff00) )/* ���j���[�ȊO�̏ꍇ�A(�|�[�Y���j���[�Ŏ��Ԃ��o�ꍇ�ɏ�����ƍ���̂�) */
	{
		s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
		while (NULL != s)
		{
			n = s->next;
			#if 1/* �����I�ɏ�����@�\ */
			/* ��莞�Ԃł̎��������A���t���OON�̏ꍇ�A */
			if ( (SP_FLAG_TIME_OVER == (s->flags&SP_FLAG_TIME_OVER)) )
			{
				s->timeover_ticks++;
				if ( 1000 < (s->timeover_ticks) )/*16�b*/	/*(1024-1) <*/ /*(0x0400 & (s->ticks))*/
				{
				//	s->timeover_ticks = 0;
					s->type = SP_DELETE;	/* ���������ɂ���B */
				}
			}
			#endif
			if (SP_DELETE == s->type)	/* �����H */
			{
				sprite_remove_one(s);	/* �폜�ς݂����� */
			}
			s = n;
		}
	}
	#endif
}
				#if 0
				//	(s->priority==PRIORITY_03_ENEMY || s->priority== PRIORITY_05_BULLETS) && /* �G���G�̕���̏ꍇ�� */
					(s->type >= SP_GROUP_ENEMYS) &&
					(s->type <= SP_GROUP_BULLETS) && /* �G���G�̕���̏ꍇ�� */
					#if 0
					s->type!=SP_BOSS01 &&	/* �e�{�X�ȊO�� */
					s->type!=SP_BOSS02 &&
					s->type!=SP_BOSS03 &&
					s->type!=SP_BOSS04 &&
					#else
					(s->type!=SP_BOSS) &&		/* �e�{�X�ȊO�� */
					#endif
					(s->type!=SP_MENU_TEXT) 		/* ���j���[�ȊO�̏ꍇ�A */
				#endif

/*---------------------------------------------------------
	�X�v���C�g��\������B
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
	s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	SDL_Rect t;
	t.y=0;
	while (NULL != s)
	{
		#if (1==USE_DEBUG)
		if (s->type != SP_DELETE )
		{
			debug_count1++;/*�L�� �X�v���C�g �J�E���g*/
			//	max 	debug_count2/debug_count1			/*	*/
			//	max 				PRIORITY_00_BG
			//	max 				PRIORITY_01_SHOT		/* ���e/�n��U�R�G/�����w */
			//	max 				PRIORITY_02_PLAYER		/* ���@ */
			//	max 				PRIORITY_03_ENEMY		/* �{�X/�U�R�G */
			//	max 				PRIORITY_04_ITEM		/* �A�C�e��/�����X�R�A/������\�� */
			//	max 	446/623 	PRIORITY_05_BULLETS 	/* �G�e */
			//	���4096�����elunatic435/589[]
			//	���4096�����elunatic437/623
			//	���4096�����elunatic446/455	(�P�� ��3�`��lunatic 337/351)
			if (PRIORITY_05_BULLETS==s->priority)
			{
				debug_count2++;/*�L�� �X�v���C�g �J�E���g*/
			}
		}
		#endif
		#if 0
		if (s->type != SP_DELETE ) /* �폜�ς݂͔�΂� */	//	SP_DELETE�� 0 �ɂȂ����̂ŗv��Ȃ��Ȃ����D
		#endif
		{
			if (s->type & type) /* type����v����ꍇ�̂� */
			{
				if ((s->flags&SP_FLAG_VISIBLE)) /* �\���\�ȏꍇ�̂� */
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
						/* �A���t�@�l���ς�����ꍇ�̂� */
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
		s=s->next;/*��*/
	}
//
	#if (1==USE_DEBUG)
	/* �s�[�N(�ő�l)�𒲂ׂ� */
	if (debug_num1 < debug_count1 ) 	{	debug_num1 = debug_count1;	}
	if (debug_num2 < debug_count2 ) 	{	debug_num2 = debug_count2;	}
	#endif
}

/*---------------------------------------------------------
	�X�v���C�g�����X�g�ɒǉ�����B
---------------------------------------------------------*/

SPRITE *sprite_add_000(SDL_Surface *surface, int frames, Uint8 priority, int set_flags/*nocache*/, int anime_speed)
{
	SPRITE *obj;/* �V�K�쐬����X�v���C�g */
	SPRITE *s;
	SPRITE *tail_obj/*=NULL*/;	/* �ǉ����郊�X�g�̌��ʒu�B�ؒf����ꍇ�͐ؒf�ʒu�B */
	obj 		= mmalloc(sizeof(SPRITE));
	s			= sprite_list000_head;	// �X�v���C�g���X�g�̐擪�B�Ăяo��
	tail_obj	= NULL;
	/*---------------------------------------------------------
		�X�v���C�g�̕\���v���C�I���e�B�[�𖞂����ʒu�𒲂ׂāA�����ɑ}������B
	---------------------------------------------------------*/
	while (
		(NULL != s) &&				/* ���X�g�̓��܂ł���ׂ��Ȃ�I��� */
		(s->priority < priority)	/* �\���v���C�I���e�B�[�𖞂����ʒu�܂ŗ�����I��� */
		)
	{
		tail_obj	= s;
		s			= s->next;
	}
	obj->next		= s;	/* �V�K�쐬�����X�v���C�g�̍Ō�ɘA�� */
	if (NULL == tail_obj)/* �擪�Ȃ�(���X�g�̐ؒf���Ȃ����) */
	{
		/* �擪�̏ꍇ��->next�������̂� */
		sprite_list000_head = obj;		// �X�v���C�g���X�g�̐擪������������B
	}
	else	//if (NULL != tail_obj)
	{
		tail_obj->next		= obj;	/* ���X�g�ɑ}�� */
	}
	/*---------------------------------------------------------
		�X�v���C�g�̐ݒ������B
	---------------------------------------------------------*/
	#if 0
	{	/* ���̋@�\(�ʂ̃X�v���C�g�ɔԍ������蓖�Ĕԍ��Ŕ��ʂ���)�͑債���Ӗ����Ȃ��̂Ŗ��������B
			(�ʂ̃X�v���C�g�𔻕ʂ������ꍇ�́A�|�C���^�Ŕ��ʂ���΂��̋@�\�͕s�v�Ȃ̂�) */
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

/* clouds�G�̏ꍇ�A�t�@�C���� */
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
	�摜�L���b�V���ɍڂ���(�Q�[�����ɉ摜�W�J����Ə�����������������
	�Q�[���ɂȂ�Ȃ��B�����ŗ\�߃L���b�V���ɍڂ��Ƃ�)
---------------------------------------------------------*/

/*static*/ void load_bg2_chache(char *filename, int use_alpha)
{
	SDL_Surface *tmp;
	tmp = loadbmp0(filename, /*0*/use_alpha, 1);
	unloadbmp_by_surface(tmp);
};

/*---------------------------------------------------------
	�ǂݍ��񂾏��Ԃɉ摜�L���b�V���ɔz�u�����̂ŁA
	�摜�L���b�V���̏��Ԃ����߂�ׂ̓ǂݍ��݁B
	-------------------------------------------------------
	�摜�L���b�V���ɍڂ���K�v��������̂��A
	��ɉ摜�L���b�V���ɍڂ���ׂ̓ǂݍ���
	�摜�L���b�V���ɍڂ��������ɓǂݍ��ށB
	�t�@�C���̊m�F�����˂�B
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
