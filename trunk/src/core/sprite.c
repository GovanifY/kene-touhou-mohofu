
/*---------------------------------------------------------
	�X�v���C�g �}�l�[�W��
--------------------------------------------------------- */

#include "game_main.h"

/*---------------------------------------------------------
	���\�[�X resource
--------------------------------------------------------- */
static IMAGE_RESOURCE my_bullet_resource[] =
{
#if 1
	/* [�L�閼�O��] */
	//#define DUMMY_IMG	"tama/bullet_maru16.png"
	#define DUMMY_IMG	"jiki/core_re.png"
	/* 77 */	//	{	"tama/kugel.png",							0,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },
	/* 78 */	//	{	"tama/kugel2.png",							0,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },
	/* 79 */		{	/*"tama/bullet_beam16.png"*/DUMMY_IMG,		0,	16,  iyx(16,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* �j�e (�G�e) */
	/* 80 */		{	/*"tama/bullet_maru16.png"*/DUMMY_IMG,		0,	16,  iyx(16,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* ����(��) (�G�e) */	/* �Ԋ�(��) (�G�e) */
//
	/* 81 */		{	/*"tama/bullet_ming32.png"*/DUMMY_IMG,		0,	32,  iyx(32,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* �䂩���e (�G�e) */
	/* 82 */		{	/*"tama/jippou32.png"*/DUMMY_IMG,			0,	32,  iyx(32,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* ���͋[�e (�G�e) */
//
					/* �ǉ��\�� */																				/* �N�i�C�e (�G�e) */
//
	/* 83 */		{	/*"tama/oodama08.png"*/DUMMY_IMG,			1,	 8,  iyx( 8,   1), PRIORITY_05_BULLETS,	  0, 4, 0 },/* ���(����...)	����(��) PRIORITY_03_ENEMY �́A�����蔻�蕔�� */
	/* 83 */	//	{	"tama/bigkugel2.png",						0,	 1,  iyx( 1,   1), PRIORITY_03_ENEMY,	  0, 2, 0 },/* ����(��)  �����蔻�蕔�� */
	/* 84 */	//	{	"tama/bigkugel1.png",						1,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },/* ���(��) �\������ */
	/* 85 */	//	{	"tama/new_bigkugel.png",					1,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },/* ���(��) �\������ */
//
	/* 86 */		{	/*"tama/knife.png"*/DUMMY_IMG,				1,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },/* �����~���i�C�t(��) */
	/* 87 */		{	/*"tama/knife_core16.png"*/DUMMY_IMG,		1,	16,  iyx(16,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },/* �S�����i�C�t(��) */

	/*103 */		{	"zako/homing16.png",						0,	16,  iyx(16,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* �U�R �U���e */

#endif
};


static IMAGE_RESOURCE my_resource[] =
{	/*�V���b�g */
	/*	0 REIMU */	{	"jiki/shot_re.png", 			0,	 1,  iyx( 1,   1), PRIORITY_01_SHOT,   0, 4, 0 },	/* "plasma.png" */
	/*	1 MARISA */ {	"jiki/shot_ma.png", 			0,	 1,  iyx( 1,   1), PRIORITY_01_SHOT,   0, 4, 0 },	/* "plasma_ma.png" */
	/*	2 REMILIA */{	"jiki/shot_oz.png", 			0,	 1,  iyx( 1,   1), PRIORITY_01_SHOT,   0, 4, 0 },	/* "plasma_oz.png" */
	/*	3 CIRNO */	{	"jiki/shot_ci.png", 			0,	 1,  iyx( 1,   1), PRIORITY_01_SHOT,   0, 6, 0 },	/* "plasma_ci.png" */
	/*	4 YUYUKO */ {	"jiki/shot_yu.png", 			0,	 1,  iyx( 1,   1), PRIORITY_01_SHOT,   0, 8, 0 },
//
	/*	5 REIMU */	{	"jiki/needle_re.png",			1,	 2,  iyx( 2,   1), PRIORITY_01_SHOT,   1, 6, 0 },	// �j�e
	/*	6 MARISA */ {	"jiki/needle_ma.png",			0,	 1,  iyx( 1,   1), PRIORITY_01_SHOT,   0, 8, 0 },	// �X�e
	/*	7 REMILIA */{	"jiki/needle_oz.png",			1,	 5,  iyx( 5,   1), PRIORITY_01_SHOT,   3,12/*8*/, 0 },	// ��������e
	/*	8 CIRNO */	{	"jiki/needle_ci.png",			0,	 1,  iyx( 1,   1), PRIORITY_01_SHOT,   0,16, 0 },	// �E�F�C�u�e
	/*	9 YUYUKO */ {	"jiki/needle_yu.png",			0,	 1,  iyx( 1,   1), PRIORITY_01_SHOT,   0, 8, 0 },	// �s���N���e
//
	/* 10 REIMU */	{	"jiki/homing_re.png",			0,	 6,  iyx( 6,   1), PRIORITY_01_SHOT,   0,16, 0 },	/* "tshoot.png", */
	/* 11 MARISA */ {	"jiki/homing_ma.png",			0,	 6,  iyx( 6,   1), PRIORITY_01_SHOT,   0,16, 0 },	/* "tshoot-ma.png" */
	/* 12 REMILIA */{	"jiki/homing_oz.png",			0,	 6,  iyx( 6,   1), PRIORITY_01_SHOT,   0,16, 0 },	/* "tshoot-oz.png" */
	/* 13 CIRNO */	{	"jiki/homing_ci.png",			0,	 6,  iyx( 6,   1), PRIORITY_01_SHOT,   0,16, 0 },	/* "tshoot-ci.png" */
	/* 14 YUYUKO */ {	"jiki/homing_yu.png",			0,	 6,  iyx( 6,   1), PRIORITY_01_SHOT,   0,16, 0 },	// ���e
//	/*���� */
	/* 15 REIMU */	{	"jiki/jiki_re.png", 			0,	12,  iyx(12,   1), PRIORITY_02_PLAYER, 0,16, 0 },	// "ship-med.png",		0,	 11,
	/* 16 MARISA */ {	"jiki/jiki_ma.png", 			0,	12,  iyx(12,   1), PRIORITY_02_PLAYER, 0,16, 0 },	// "ship-med-ma.png",	0,	 11,
	/* 17 REMILIA */{	"jiki/jiki_oz.png", 			0,	12,  iyx(12,   1), PRIORITY_02_PLAYER, 0,16, 0 },	// "ship-med-oz.png",	0,	 11,
	/* 18 CIRNO */	{	"jiki/jiki_ci.png", 			0,	12,  iyx(12,   1), PRIORITY_02_PLAYER, 0,16, 0 },	// "ship-med-ci.png",	0,	 11,
	/* 19 YUYUKO */ {	"jiki/jiki_yu.png", 			0,	12,  iyx(12,   1), PRIORITY_02_PLAYER, 0,16, 0 },
//	/*�����蔻�� */
	/* 20 REIMU */	{	"jiki/core_re.png", 			0,	 1,  iyx( 1,   1), PRIORITY_03_ENEMY,  0, 1/*2*/, 0 },	// "core.png",
	/* 21 MARISA */ {	"jiki/core_ma.png", 			0,	 1,  iyx( 1,   1), PRIORITY_03_ENEMY,  0, 2/*4*/, 0 },	// "core-ma.png",
	/* 22 REMILIA */{	"jiki/core_oz.png", 			0,	 1,  iyx( 1,   1), PRIORITY_03_ENEMY,  0, 2/*4*/, 0 },	// "core-oz.png",
	/* 23 CIRNO */	{	"jiki/core_ci.png", 			0,	 1,  iyx( 1,   1), PRIORITY_03_ENEMY,  0, 1/*3*/, 0 },	// "core-ci.png",
	/* 24 YUYUKO */ {	"jiki/core_yu.png", 			0,	 1,  iyx( 1,   1), PRIORITY_03_ENEMY,  0, 2/*4*/, 0 },
//	/* �I�v�V�����p�f�� */
	/* 25 REIMU */	{	"jiki/option1_re.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "option.png" */
	/* 26 MARISA */ {	"jiki/option1_ma.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "mari_op.png" */
	/* 27 REMILIA */{	"jiki/option1_oz.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "oze_op1.png" */
	/* 28 CIRNO */	{	"jiki/option1_ci.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "ci_op1.png" */
	/* 29 YUYUKO */ {	"jiki/option1_yu.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },
//
	/* 30 REIMU */	{	"jiki/option2_re.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* [�N���`�F�b�N�Ŏg���̂ŗL�閼�O��] */
	/* 31 MARISA */ {	"jiki/option2_ma.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* [�N���`�F�b�N�Ŏg���̂ŗL�閼�O��] */
	/* 32 REMILIA */{	"jiki/option2_oz.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "oze_op2.png" */
	/* 33 CIRNO */	{	"jiki/option2_ci.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "ci_op2.png" */
	/* 34 YUYUKO */ {	"jiki/option2_yu.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },
//
	/* 35 REIMU */	{	"jiki/option3_re.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* [�N���`�F�b�N�Ŏg���̂ŗL�閼�O��] */
	/* 36 MARISA */ {	"jiki/option3_ma.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* [�N���`�F�b�N�Ŏg���̂ŗL�閼�O��] */
	/* 37 REMILIA */{	"jiki/option3_oz.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "oze_op3.png" */
	/* 38 CIRNO */	{	"jiki/option3_ci.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "ci_op3.png" */
	/* 39 YUYUKO */ {	"jiki/option3_yu.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },
//
	/* 40 REIMU */	{	"jiki/option4_re.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* [�N���`�F�b�N�Ŏg���̂ŗL�閼�O��] */
	/* 41 MARISA */ {	"jiki/option4_ma.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* [�N���`�F�b�N�Ŏg���̂ŗL�閼�O��] */
	/* 42 REMILIA */{	"jiki/option4_oz.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "oze_op4.png" */
	/* 43 CIRNO */	{	"jiki/option4_ci.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },	/* "ci_op4.png" */
	/* 44 YUYUKO */ {	"jiki/option4_yu.png",			0,	 8,  iyx( 8,   1), PRIORITY_02_PLAYER, 0, 0, 0 },

//	/* �{���p�f�� */
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
	/* 55 REIMU */	{	"jiki/bomber1_re.png",			1,	32,  iyx(16,   2), PRIORITY_03_ENEMY,  0, 2, 0 },	/* [�N���`�F�b�N�Ŏg���̂ŗL�閼�O��] */
	/* 56 MARISA */ {	"jiki/bomber3_ma.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5,16, 0 },	/* "star_shield_green.png" */
	/* 57 REMILIA */{	"jiki/bomber3_oz.png",			1,	 4,  iyx( 4,   1), PRIORITY_01_SHOT,   3,16, 0 },	/* "fire_wind_r.png" */ 	/*0a */
	/* 58 CIRNO */	{	"jiki/bomber3_ci.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5,32, 0 },	/* "cirno_shield_green.png" */
	/* 59 YUYUKO */ {	"jiki/bomber3_yu.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5, 4, 0 },
//
	/* 60 REIMU */	{	"jiki/bomber1_re.png",			1,	32,  iyx(16,   2), PRIORITY_03_ENEMY,  0, 2, 0 },	/* [�N���`�F�b�N�Ŏg���̂ŗL�閼�O��] */
	/* 61 MARISA */ {	"jiki/bomber4_ma.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5, 8, 0 },	/* "star_shields_blue.png" */
	/* 62 REMILIA */{	"jiki/bomber4_oz.png",			1,	 4,  iyx( 4,   1), PRIORITY_01_SHOT,   3,16, 0 },	/* "fire_wind_u.png" */ 	/*0a */
	/* 63 CIRNO */	{	"jiki/bomber4_ci.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5,24, 0 },	/* "cirno_shields_blue.png" */
	/* 64 YUYUKO */ {	"jiki/bomber4_yu.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5, 4, 0 },
//
	/* 65 REIMU */	{	"jiki/bomber1_re.png",			1,	32,  iyx(16,   2), PRIORITY_03_ENEMY,  0, 2, 0 },	/* [�N���`�F�b�N�Ŏg���̂ŗL�閼�O��] */
	/* 66 MARISA */ {	"jiki/bomber5_ma.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5, 8, 0 },	/* "star_shield_red.png" */
	/* 67 REMILIA */{	"jiki/bomber4_oz.png",			1,	 4,  iyx( 4,   1), PRIORITY_01_SHOT,   3, 2, 0 },	/* [�N���`�F�b�N�Ŏg���̂ŗL�閼�O��] */	/*0a */
	/* 68 CIRNO */	{	"jiki/bomber5_ci.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5,24, 0 },	/* "cirno_shield_red.png" */
	/* 69 YUYUKO */ {	"jiki/bomber5_yu.png",			0,	 8,  iyx( 8,   1), PRIORITY_01_SHOT,   0,80, 0 },	/* �H�X�q�{���̐�{�� */
//
	/* 70 REIMU */	{	"jiki/bomber1_re.png",			1,	32,  iyx(16,   2), PRIORITY_03_ENEMY,  0, 2, 0 },	/* [�N���`�F�b�N�Ŏg���̂ŗL�閼�O��] */
	/* 71 MARISA */ {	"jiki/bomber6_ma.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5, 8, 0 },	/* "star_shields_green.png" */
	/* 72 REMILIA */{	"jiki/bomber4_oz.png",			1,	 4,  iyx( 4,   1), PRIORITY_01_SHOT,   3, 2, 0 },	/* [�N���`�F�b�N�Ŏg���̂ŗL�閼�O��] */	/*0a */
	/* 73 CIRNO */	{	"jiki/bomber6_ci.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5,24, 0 },	/* "cirno_shields_green.png" */
	/* 74 YUYUKO */ {	"jiki/bomber4_yu.png",			0,	 3,  iyx( 3,   1), PRIORITY_01_SHOT,   5, 2, 0 },	/* [�N���`�F�b�N�Ŏg���̂ŗL�閼�O��] */
//
	/* 75 */		{	"jiki/bomber_slow.png", 		1,	 4,  iyx( 4,   1), PRIORITY_01_SHOT,   0, 0, 0 },
	/* 76 */		{	"panel/bonus_items.png",		0,	 8,  iyx( 8,   1), PRIORITY_04_ITEM,   0,12, 0 },
//
#if 0
	/* 77 */	//	{	"tama/kugel.png",				0,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },
	/* 78 */	//	{	"tama/kugel2.png",				0,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },
	/* 79 */		{	"tama/bullet_beam16.png",		0,	16,  iyx(16,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* �j�e (�G�e) */
	/* 80 */		{	"tama/bullet_maru16.png",		0,	16,  iyx(16,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* ����(��) (�G�e) */	/* �Ԋ�(��) (�G�e) */
//
	/* 81 */		{	"tama/bullet_ming32.png",		0,	32,  iyx(32,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* �䂩���e (�G�e) */
	/* 82 */		{	"tama/jippou32.png",			0,	32,  iyx(32,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* ���͋[�e (�G�e) */
//
					/* �ǉ��\�� */																				/* �N�i�C�e (�G�e) */
//
	/* 83 */		{	"tama/oodama08.png",			1,	 8,  iyx( 8,   1), PRIORITY_05_BULLETS,	  0, 4, 0 },/* ���(����...)	����(��) PRIORITY_03_ENEMY �́A�����蔻�蕔�� */
	/* 83 */	//	{	"tama/bigkugel2.png",			0,	 1,  iyx( 1,   1), PRIORITY_03_ENEMY,	  0, 2, 0 },/* ����(��)  �����蔻�蕔�� */
	/* 84 */	//	{	"tama/bigkugel1.png",			1,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },/* ���(��) �\������ */
	/* 85 */	//	{	"tama/new_bigkugel.png",		1,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },/* ���(��) �\������ */
//
	/* 86 */		{	"tama/knife.png",				1,	 1,  iyx( 1,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },/* �����~���i�C�t(��) */
	/* 87 */		{	"tama/knife_core16.png",		1,	16,  iyx(16,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },/* �S�����i�C�t(��) */

	/*103 */		{	"zako/homing16.png",			0,	16,  iyx(16,   1), PRIORITY_05_BULLETS,	  0, 2, 0 },	/* �U�R �U���e */

#endif
//
	/* 88 */		{	"zako/mahoujin_0.png",			1,	 2,  iyx( 2,   1), PRIORITY_01_SHOT,   0, 0, 0 },/*0a */ /* �G�̌��̃A�� */
	/* 89 */		{	"zako/tikei_bgpanel1.png",		0,	 1,  iyx( 1,   1), PRIORITY_00_BG,	  0, 0, 0 },	/* �n�`�R�A�p�l�� */
	/* 90 */		{	"zako/tikei_bgpanel2.png",		0,	 1,  iyx( 1,   1), PRIORITY_00_BG,	  0, 0, 0 },	/* �n�`�p�l���_ */
	/* 91 */		{	"zako/tikei_grounder08.png",	0,	 8,  iyx( 8,   1), PRIORITY_01_SHOT,   2,16, 0 },	/* �}���z�[�� */
//
	/* 92 */		{	"zako/obake08.png", 			0,	 8,  iyx( 8,   1), PRIORITY_03_ENEMY,  0, 8, 0 },	/* �U�R */
	/* 93 */		{	"zako/yukari8x4.png",			0,	32,  iyx( 8,   4), PRIORITY_03_ENEMY,  0, 2, 0 },	/* �U�R */
	/* 94 */		{	"zako/aka_kedama08.png",		0,	 8,  iyx( 8,   1), PRIORITY_03_ENEMY,  4, 6, 0 },	/* �U�R 4 1 */
	/* 95 */		{	"zako/niji_kedama16.png",		0,	16,  iyx(16,   1), PRIORITY_03_ENEMY,  1, 8, 0 },	/* �U�R */
//
	/* 96 */		{	"zako/midoori_kedama16.png",	0,	16,  iyx(16,   1), PRIORITY_03_ENEMY,  4, 4, 0 },	/* �U�R 4 1 */
	/* 97 */		{	"zako/kedama16.png",			0,	16,  iyx(16,   1), PRIORITY_03_ENEMY,  5, 4, 0 },	/* �U�R */
	/* 98 */		{	"zako/inyou1_16.png",			0,	16,  iyx(16,   1), PRIORITY_03_ENEMY,  0,40, 0 },	/* �U�R */
	/* 99 */		{	"zako/tatsumaki16.png", 		0,	16,  iyx(16,   1), PRIORITY_03_ENEMY,  2,16, 0 },	/* �U�R */
//
	/*100 */		{	"zako/great_fairy02.png",		0,	 2,  iyx( 2,   1), PRIORITY_03_ENEMY,  0,16, 0 },	/* �U�R(���U�R) - */
	/*101 */		{	"zako/aka_meido08.png", 		0,	 8,  iyx( 8,   1), PRIORITY_03_ENEMY,  0,24, 0 },	/* �U�R */
	/*102 */		{	"zako/ao_yousei4x6.png", 		0,	24,  iyx( 4,   6), PRIORITY_03_ENEMY,  0,12, 0 },	/* �U�R "zako/ao_yousei24.png" */
//	/* ������ */
	/*104 */		{	"effect/tr_blue.png",			0,	 6,  iyx( 6,   1), PRIORITY_04_ITEM,   1, 0, 0 },
	/*105 */		{	"effect/tr_red.png",			0,	 6,  iyx( 6,   1), PRIORITY_04_ITEM,   1, 0, 0 },
	/*106 */		{	"effect/tr_green.png",			0,	 6,  iyx( 6,   1), PRIORITY_04_ITEM,   1, 0, 0 },
//	/* �U�R���Ŕ��� */
	/*107 */		{	"effect/bakuha05.png",			1,	 5,  iyx( 5,   1), PRIORITY_04_ITEM,   3, 0, 0 },
	/*108 */		{	"effect/bakuha06.png",			1,	 5,  iyx( 5,   1), PRIORITY_04_ITEM,   3, 0, 0 },
	/*109 */		{	"effect/bakuha07.png",			1,	 5,  iyx( 5,   1), PRIORITY_04_ITEM,   3, 0, 0 },
//	/* �Ή����� */
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
	�����蔻�� collision
--------------------------------------------------------- */

/*---------------------------------------------------------
	��`/�~ �����蔻��
	-------------------------------------------------------
	�I�u�W�F�N�g���m�̓����蔻��
	��܂��ɋ�`�Ŕ��ʂ�����A�߂����Ȃ�~�̏Փ˔���
--------------------------------------------------------- */

#if (1)
static /*BOOL */int collision_hit( /*obj_t */SPRITE *obj1, /*obj_t */SPRITE *obj2 )
{
	/* --- ����L���ȃI�u�W�F�N�g�łȂ�������I�� */
//	if (obj2->m_Hit256R == 0/*.0 */ /*FALSE */)
	if (256 > obj2->m_Hit256R /*FALSE */)
	{
		return (0/*FALSE */);	/* not hit. �������ĂȂ� */
	}
//	if (obj1->m_Hit256R == 0/*.0 */ /*FALSE */)
	if (256 > obj1->m_Hit256R /*FALSE */)
	{
		return (0/*FALSE */);	/* not hit. �������ĂȂ� */
	}
//
	/*flo at */int dr, dx, dy;
	dr = (obj2->m_Hit256R + obj1->m_Hit256R);
	dx = ((obj2->x256+obj2->w128) - (obj1->x256+obj1->w128));
	dy = ((obj2->y256+obj2->h128) - (obj1->y256+obj1->h128));
	/* check collision, delta x position. ��`����i�w���j */
	if (dx < 0/*.0 */)	{	dx = (-dx); }	/*dx = abs(dx); */
	if (dx > dr)	{	return (0/*FALSE */);	}	/* not hit, because not near the dx. �w�̍������傫���̂ŁA�������ĂȂ� */

	/* check collision, delta y position. ��`����i�x���j */
	if (dy < 0/*.0 */)	{	dy = (-dy); }	/*dy = abs(dy); */
	if (dy > dr)	{	return (0/*FALSE */);	}	/*	not hit, because not near the dy. �x�̍������傫���̂ŁA�������ĂȂ� */

	/* check collision, circle delta round distance.  �~�̔��a�Ŕ��� */
	dr *= dr;
	dx *= dx;
	dy *= dy;
	dx += dy;
	if (dr < dx)/*if ( (dr^2) < ( (dx^2) + (dy^2) )) */
	{
		return (0/*FALSE */);	/* not hit. �������ĂȂ� */
	}
	return (1/*TRUE */); /* hit. �������� */
}
#endif


/*---------------------------------------------------------
	�X�v���C�g�̂����蔻��
--------------------------------------------------------- */

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
					#if 1
					/* 4. ��`/�~�����蔻��`�F�b�N   ��܂��ɋ�`�Ŕ��ʂ�����A�߂����Ȃ�~�̏Փ˔��� */
					if (collision_hit(s,tocheck))	/* ��`/�~�����蔻�� */
					{
						return (s);/*�������� */
					}
					#endif
				}
			}
		}
		s = s->next;/*�� */
	}
	return (NULL);/*�������ĂȂ� */
}

/*---------------------------------------------------------
	�X�v���C�g ���X�g�ɓo�^���ꂽ�X�v���C�g�������B
--------------------------------------------------------- */

static void sprite_remove_one(SPRITE *src)
{
	SPRITE *s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	SPRITE *p = NULL;
	SPRITE *n = NULL;
	while (NULL != s)/* �X�v���C�g ���X�g�̏I���܂ŒT�� */
	{
		n = s->next;
		if (s == src)/*���������H */
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
			return;/* ���� */
		}
		p = s;
		s = n;
	}
	CHECKPOINT;/* �ُ� */
	error(ERR_WARN, "sprite not found in list");
}

/*---------------------------------------------------------
	�X�v���C�g ���X�g�ɓo�^���ꂽ�X�v���C�g��S�������B
--------------------------------------------------------- */

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
--------------------------------------------------------- */

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
				/*�t�]�A�j���֎~�ɕύX */
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
					//	else		/*�t�]�A�j���֎~�ɕύX */
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
				if ( 1000 < (s->timeover_ticks) )/*16�b */	/*(1024-1) < */ /*(0x0400 & (s->ticks)) */
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

/*---------------------------------------------------------
	�X�v���C�g��`�悷��B
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
			/* �A���t�@�l���ς�����ꍇ�̂� */
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
	�X�v���C�g��\������B
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
	s = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	while (NULL != s)
	{
		#if (1==USE_DEBUG)
		if (s->type != SP_DELETE )
		{
			debug_count1++;/*�L�� �X�v���C�g �J�E���g */
			//	max 	debug_count2/debug_count1			/*	 */
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
				debug_count2++;/*�L�� �X�v���C�g �J�E���g */
			}
		}
		#endif
		#if 0
		if (s->type != SP_DELETE ) /* �폜�ς݂͔�΂� */	//	SP_DELETE�� 0 �ɂȂ����̂ŗv��Ȃ��Ȃ����D
		#endif
		{
			if (s->type & type) /* type����v����ꍇ�̂� */
			{
				#if 1
				if (0==(s->type & SP_GROUP_BULLETS)) /* �Ƃ肠�����e�͕`���Ȃ� */
				#endif
				{
					if ((s->flags&SP_FLAG_VISIBLE)) /* �\���\�ȏꍇ�̂� */
					{
						sprite_draw000(s);
					}
				}
			}
		}
		s=s->next;/*�� */
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
	SPRITE *obj;/* �V�K�쐬����X�v���C�g */
	SPRITE *s;
	SPRITE *tail_obj/*=NULL */; /* �ǉ����郊�X�g�̌��ʒu�B�ؒf����ꍇ�͐ؒf�ʒu�B */
	obj 		= mmalloc(sizeof(SPRITE));
	s			= sprite_list000_head;	// �X�v���C�g���X�g�̐擪�B�Ăяo��
	tail_obj	= NULL;
	/*---------------------------------------------------------
		�X�v���C�g�̕\���v���C�I���e�B�[�𖞂����ʒu�𒲂ׂāA�����ɑ}������B
	--------------------------------------------------------- */
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
	--------------------------------------------------------- */
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

/* clouds�G�̏ꍇ�A�t�@�C���� */
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
	�摜�L���b�V���ɍڂ���(�Q�[�����ɉ摜�W�J����Ə�����������������
	�Q�[���ɂȂ�Ȃ��B�����ŗ\�߃L���b�V���ɍڂ��Ƃ�)
--------------------------------------------------------- */

/*static */ void load_bg2_chache(char *filename, int use_alpha)
{
	SDL_Surface *tmp;
	tmp = loadbmp0(filename, /*0 */use_alpha, 1);
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
