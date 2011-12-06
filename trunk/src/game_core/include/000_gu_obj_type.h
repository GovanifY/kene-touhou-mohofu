
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/



/* === Gu�����w�� === */		/* --- �܂�����ĂȂ� --- */

//enum	/* �� �����w�ʃG�t�F�N�g */
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

/* === Gu�v���C���[�� === */
enum/* �I�v�V�����ƃC���^�[���[�u���Ă�̂́A(�I�v�V�����A�j��)�v���O�����̓s�� */
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

//	�v���C���[�A�j���[�V����
// [1]	���ֈړ��̃A�j���[�V����	[���S������4������3������2������1������1������1��...]
	JIKI_PLAYER_00_LEFT,	/* ����1 */ 	/* �ł��� */
	JIKI_PLAYER_01_LEFT,	/* ����2 */
	JIKI_PLAYER_02_LEFT,	/* ����3 */
	JIKI_PLAYER_03_LEFT,	/* ����4 */ 	/* ����荶 */
//	�J��Ԃ��A�j���[�V����		[���S1�����S2�����S3�����S4�����S1�����S2�����S3�����S4��...]
	JIKI_PLAYER_04_MID, 	/* ���S1 */
	JIKI_PLAYER_05_MID, 	/* ���S2 */
	JIKI_PLAYER_06_MID, 	/* ���S3 */
	JIKI_PLAYER_07_MID, 	/* ���S4 */

// [6]
	JIKI_OPTION_01_00,
	JIKI_OPTION_01_01,
	JIKI_OPTION_01_02,
	JIKI_OPTION_01_03,
	JIKI_OPTION_01_04,
	JIKI_OPTION_01_05,
	JIKI_OPTION_01_06,
	JIKI_OPTION_01_07,

// [2]	�E�ֈړ��̃A�j���[�V����	[���S���E��1���E��2���E��3���E��4���E��4���E��4��...]
	JIKI_PLAYER_08_RIGHT,	/* �E��1 */ 	/* �����E */
	JIKI_PLAYER_09_RIGHT,	/* �E��2 */
	JIKI_PLAYER_10_RIGHT,	/* �E��3 */
	JIKI_PLAYER_11_RIGHT,	/* �E��4 */ 	/* �ł��E */
	JIKI_ETC_00,			/* --- ���g�p --- */
	JIKI_ETC_01,			/* --- ���g�p --- */
	JIKI_ETC_02,			/* --- ���g�p --- */
	JIKI_ETC_03,			/* --- ���g�p --- */

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
	JIKI_BOMBER_00, 		/* �{���o�[�������̗����G */
	JIKI_BOMBER_01, 		/* ��88�e / �U���{��(�ᑬ�{��) */
	JIKI_BOMBER_02, 		/* --- ���g�p --- / ���e16�� / ���E�� / ����1 */
	JIKI_BOMBER_03, 		/* --- ���g�p --- / ���e16�� / ���E�� / ����2 */
	JIKI_BOMBER_04, 		/* ���D1 / ��1 / �\����1 / ��1 / �` */
	JIKI_BOMBER_05, 		/* ���D2 / ��2 / �\����2 / ��2 / �� */
	JIKI_BOMBER_06, 		/* ���D3 / ��3 / �\����3 / ��3 / �m */
	JIKI_BOMBER_07, 		/* ���D4 / ��4 / �\����4 / ��4 / �H */

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
	JIKI_SHOT_00,			/* �ԎDC /	/  / ����3 / */
	JIKI_SHOT_01,			/* �ԎDB /	/  / ����4 / */
	JIKI_SHOT_02,			/* �ԎDA / ���� / ���� / �X / �� */
	JIKI_SHOT_03,			/* / ��������e1 / ���D1 */
	JIKI_SHOT_04,			/* / ��������e2 / ���D2 */
	JIKI_SHOT_05,			/* / ��������e3 / ���D3 */
	JIKI_SHOT_06,			/* / ��������e4 / ���D4 */
	JIKI_SHOT_07,			/* / ��������e5 / �j�e / �X�e / �E�F�C�u�e / �s���N���e */

};

/* === Gu�G�� === */		/* --- �܂�����ĂȂ� --- */
	/* �� �����w�ʃG�t�F�N�g */

/* �G���ʋK�i(�{�X/���{�X/�U�R/�{�X�I�v�V����/���̑�) */
enum /*_teki_common_kikaku_*/
{
/*
xx:���g�p�B
ok:�ϊ��ς݁B
*/
/*r34	r33*/
/*00xx	*/	TEKI_00_00		/* �d��0[�E��1] */		= (OBJ_Z02_TEKI),
/*01xx	*/	TEKI_01_01, 	/* �d��0[�E��2] */
/*02xx	*/	TEKI_02_02, 	/* �d��0[�E��3] */
/*03xx	*/	TEKI_03_03, 	/* �d��0[�E��4] */
/*04xx	*/	TEKI_04_04, 	/* �d��0[����1] */
/*05xx	*/	TEKI_05_05, 	/* �d��0[����2] */
/*06xx	*/	TEKI_06_06, 	/* �d��0[����3] */
/*07xx	*/	TEKI_07_07, 	/* �d��0[����4] */
//
/*08xx	*/	TEKI_08_08, 	/* �d��0[�U��1] */
/*09xx	*/	TEKI_09_09, 	/* �d��0[�U��2] */
/*10	*/	TEKI_10_0a, 	/* �d��0[�U��3] */
/*11	*/	TEKI_11_0b, 	/* �d��0[�U��4] */
/*12	*/	TEKI_12_0c, 	/* �d��0[����1] */
/*13	*/	TEKI_13_0d, 	/* �d��0[����2] */
/*14	*/	TEKI_14_0e, 	/* �d��0[����3] */
/*15	*/	TEKI_15_0f, 	/* �d��0[����4] */
//
/*16ok	*/	TEKI_16_10, 	/* �d��1[�E����1] */
/*17xx	*/	TEKI_17_11, 	/* �d��1[�E����2] */
/*18xx	*/	TEKI_18_12, 	/* �d��1[�E����3] */
/*19xx	*/	TEKI_19_13, 	/* �d��1[�E����4] */
/*20xx	*/	TEKI_20_14, 	/* �d��1[���ʌ���1] */	/* BOSS_20_YOUSEI1_SYOUMEN */
/*21xx	*/	TEKI_21_15, 	/* �d��1[���ʌ���2] */
/*22xx	*/	TEKI_22_16, 	/* �d��1[���ʌ���3] */
/*23xx	*/	TEKI_23_17, 	/* �d��1[���ʌ���4] */
//
/*24	*/	TEKI_24_18, 	/* �d��1[������1] */
/*25	*/	TEKI_25_19, 	/* �d��1[������2] */
/*26	*/	TEKI_26_1a, 	/* �d��1[������3] */
/*27	*/	TEKI_27_1b, 	/* �d��1[������4] */
/*28ok	*/	TEKI_28_1c, 	/* �d��1[����1] */
/*29ok	*/	TEKI_29_1d, 	/* �d��1[����2] */
/*30ok	*/	TEKI_30_1e, 	/* �d��1[����3] */
/*31xx	*/	TEKI_31_1f, 	/* �d��1[����4] */
//
/*32ok	*/	TEKI_32_20, 	/* �d��2[�E����1] */
/*33	*/	TEKI_33_21, 	/* �d��2[�E����2] */
/*34	*/	TEKI_34_22, 	/* �d��2[�E����3] */
/*35	*/	TEKI_35_23, 	/* �d��2[�E����4] */
/*36ok	*/	TEKI_36_24, 	/* �d��2[���ʌ���1] */
/*37	*/	TEKI_37_25, 	/* �d��2[���ʌ���2] */
/*38	*/	TEKI_38_26, 	/* �d��2[���ʌ���3] */
/*39	*/	TEKI_39_27, 	/* �d��2[���ʌ���4] */
//
/*40	*/	TEKI_40_28, 	/* �d��2[������1] */
/*41	*/	TEKI_41_29, 	/* �d��2[������2] */
/*42	*/	TEKI_42_2a, 	/* �d��2[������3] */
/*43	*/	TEKI_43_2b, 	/* �d��2[������4] */
/*44ok	*/	TEKI_44_2c, 	/* �d��2[����1] */
/*45xx	*/	TEKI_45_2d, 	/* �d��2[����2] */
/*46ok	*/	TEKI_46_2e, 	/* �d��2[����3] */
/*47ok	*/	TEKI_47_2f, 	/* �d��2[����4] */
//
/*48ok	*/	TEKI_48_30, 	/* �d��3[�E����1] */
/*49	*/	TEKI_49_31, 	/* �d��3[�E����2] */
/*50	*/	TEKI_50_32, 	/* �d��3[�E����3] */
/*51	*/	TEKI_51_33, 	/* �d��3[�E����4] */
/*52	*/	TEKI_52_34, 	/* �d��3[���ʌ���1] */
/*53	*/	TEKI_53_35, 	/* �d��3[���ʌ���2] */
/*54	*/	TEKI_54_36, 	/* �d��3[���ʌ���3] */
/*55	*/	TEKI_55_37, 	/* �d��3[���ʌ���4] */
//
/*56	*/	TEKI_56_38, 	/* �d��3[������1] */
/*57	*/	TEKI_57_39, 	/* �d��3[������2] */
/*58	*/	TEKI_58_3a, 	/* �d��3[������3] */
/*59	*/	TEKI_59_3b, 	/* �d��3[������4] */
/*60ok	*/	TEKI_60_3c, 	/* �d��3[����1] */
/*61ok	*/	TEKI_61_3d, 	/* �d��3[����2] */
/*62ok	*/	TEKI_62_3e, 	/* �d��3[����3] */
/*63	*/	TEKI_63_3f, 	/* �d��3[����4] */
};
#if (1)
	/* �{�X/�{�X�I�v�V�����p�ݒ� */
	#define 	BOSS_00_11				TEKI_00_00/* �{�X & ��-�{�X[�E��1] */
//	#define 	BOSS_01_12				TEKI_01_01/* �{�X & ��-�{�X[�E��2] */
//	#define 	BOSS_02_13				TEKI_02_02/* �{�X & ��-�{�X[�E��3] */
//	#define 	BOSS_03_14				TEKI_03_03/* �{�X & ��-�{�X[�E��4] */
	#define 	BOSS_04_21				TEKI_04_04/* �{�X & ��-�{�X[����1] */
//	#define 	BOSS_05_22				TEKI_05_05/* �{�X & ��-�{�X[����2] */
//	#define 	BOSS_06_23				TEKI_06_06/* �{�X & ��-�{�X[����3] */
//	#define 	BOSS_07_24				TEKI_07_07/* �{�X & ��-�{�X[����4] */

//	#define 	BOSS_08_31				TEKI_08_08/* �{�X & ��-�{�X[�U��1] */
	#define 	BOSS_09_32				TEKI_09_09/* �{�X & ��-�{�X[�U��2] */
	#define 	BOSS_10_33				TEKI_10_0a/* �{�X & ��-�{�X[�U��3] */
//	#define 	BOSS_11_34				TEKI_11_0b/* �{�X & ��-�{�X[�U��4] */
//	#define 	BOSS_12_41				TEKI_12_0c/* �{�X & ��-�{�X[����1] */
//	#define 	BOSS_13_42				TEKI_13_0d/* �{�X & ��-�{�X[����2] */
//	#define 	BOSS_14_43				TEKI_14_0e/* �{�X & ��-�{�X[����3] */
//	#define 	BOSS_15_44				TEKI_15_0f/* �{�X & ��-�{�X[����4] */

	#define BOSS_20_YOUSEI1_SYOUMEN		TEKI_20_14
	#define ZAKO_YOUSEI_01				TEKI_16_10

	/* �U�R�p�ݒ� */
//	#define ZAKO_12_EFFECT1_resurved	TEKI_12_0c		/* r29���� ���g�p */
//	#define ZAKO_13_EFFECT2_resurved	TEKI_13_0d		/* r29���� ���g�p */
//	#define ZAKO_14_EFFECT3_resurved	TEKI_14_0e		/* r29���� ���g�p */
//	#define ZAKO_15_EFFECT4_resurved	TEKI_15_0f		/* r29���� ���g�p */

	#define ZAKO_28_MAHOJIN 			TEKI_28_1c		/* ���̖����w */


	#define ZAKO_63_HOMING_STUFF		TEKI_63_3f
#endif


/* === GuITEM�� === */
enum /*sprite_type*/
{
	/* �A�C�e�� */
	SP_ITEM_00_P001 	/* [p]�� */ 		= (OBJ_Z03_ITEM), 	/* Bonus items */
	SP_ITEM_01_P008,	/* [P]�� */
	SP_ITEM_02_P128,	/* [F] */
	SP_ITEM_03_1UP, 	/* [1up] */
	SP_ITEM_04_BOMB,	/* [B] */
	SP_ITEM_05_TENSU,	/* [�_] */
	SP_ITEM_06_HOSI,	/* [��] */
	SP_ITEM_07_SPECIAL,
};


/* === Gu�e�� === */

// �O���[�v�e�̏ꍇ�A�ʂɐF�w�肵�Ȃ��B
// �F�w�肷��ꍇ�́A���L���ʐF�w����g���B
enum
{
	TAMA_IRO_00_SIRO = 0,	// ��
	TAMA_IRO_01_AKA,		// ��
	TAMA_IRO_02_YUKARI, 	// ��
	TAMA_IRO_03_AOI,		// ��
	TAMA_IRO_04_MIZU_IRO,	// ��
	TAMA_IRO_05_MIDORI, 	// ��
	TAMA_IRO_06_KI_IRO, 	// ��
	TAMA_IRO_07_DAI_DAI,	// ��
};
enum
{
//1 ���ےe(�O���[�v�e)
	BULLET_MARU10_BASE/*[���_]*/			= (OBJ_Z04_TAMA),
	BULLET_MARU10_01_,// ��
	BULLET_MARU10_02_,// ��
	BULLET_MARU10_03_,// ��
	BULLET_MARU10_04_,// ��
	BULLET_MARU10_05_,// ��
	BULLET_MARU10_06_,// ��
	BULLET_MARU10_07_,// ��
//2 �Ēe(�O���[�v�e)
	BULLET_KOME_BASE,// ��
	BULLET_KOME_01_,// ��
	BULLET_KOME_02_,// ��
	BULLET_KOME_03_,// ��
	BULLET_KOME_04_,// ��
	BULLET_KOME_05_,// ��
	BULLET_KOME_06_,// ��
	BULLET_KOME_07_,// ��
//3 �~�j�e(�O���[�v�e)
	BULLET_MINI8_BASE,// ��
	BULLET_MINI8_01_,// ��
	BULLET_MINI8_02_,// ��
	BULLET_MINI8_03_,// ��
	BULLET_MINI8_04_,// ��
	BULLET_MINI8_05_,// ��
	BULLET_MINI8_06_,// ��
	BULLET_MINI8_07_,// ��
//4 �g�t�����ےe(�O���[�v�e)
	BULLET_WAKU12_BASE,// ��	/*12*/	[�`���m]
	BULLET_WAKU12_01_,/// ��	/*12*/	[�`���m]
	BULLET_WAKU12_02_,//KURO_AKA,		// [��] /*12*/	���~���A�p�̍��g�Ԓe			//	BULLET_CAP16_04_KOME_SIROI, 	/* [���Ēe] */
	BULLET_WAKU12_03_,// �� 	/*12*/	[�`���m]
	BULLET_WAKU12_04_,// �� 	//(r35) BULLET_CAP16_04_TUTU_SIROI, 	// [��] /*12*/						/* [������e] */
	BULLET_WAKU12_05_,// �� 	/*12*/	[�`���m]
	BULLET_WAKU12_06_,// �� 	/*12*/	[�`���m]	//	BULLET_MINI8_12_dummy,
	BULLET_WAKU12_07_,// �� 	/*12*/	[�`���m]	//	BULLET_MINI8_13_dummy,
//5 �ӂ�����e(�ؒe) (�O���[�v�e)
	BULLET_UROKO14_BASE,// ��
	BULLET_UROKO14_01_,// ��
	BULLET_UROKO14_02_,// ��
	BULLET_UROKO14_03_,// ��
	BULLET_UROKO14_04_,// ��
	BULLET_UROKO14_05_,// ��
	BULLET_UROKO14_06_,// ��
	BULLET_UROKO14_07_,// ��
//6 �N�i�C�e(�O���[�v�e)
	BULLET_KUNAI12_BASE,// ��
	BULLET_KUNAI12_01_,// ��
	BULLET_KUNAI12_02_,// ��
	BULLET_KUNAI12_03_,// ��
	BULLET_KUNAI12_04_,// ��
	BULLET_KUNAI12_05_,// ��
	BULLET_KUNAI12_06_,// ��
	BULLET_KUNAI12_07_,// ��
//7 �D�e / �i�C�t�e
	BULLET_OFUDA12_00_AOI,
	BULLET_OFUDA12_01_AKA,
	BULLET_OFUDA12_02_YUKARI,
	BULLET_CAP16_04_TUTU_SIROI, 	//(r35) BULLET_OFUDA12_03_MIDORI,
	BULLET_KNIFE20_04_AOI,
	BULLET_KNIFE20_05_AKA,
	BULLET_KNIFE20_06_YUKARI,
	BULLET_KNIFE20_07_MIDORI,
//8 �j�e / ��ʒe
	BULLET_HARI32_00_AOI,
	BULLET_HARI32_01_AKA,
	BULLET_HARI32_02_KIIRO,
	BULLET_HARI32_03_DAIDAI,
	BULLET_OODAMA32_00_AOI, 	/* �F */
	BULLET_OODAMA32_01_AKA, 	/* �ԐF */
	BULLET_OODAMA32_02_KIIRO,	/* ���F */
	BULLET_OODAMA32_03_MIDORI,	/* �ΐF */
};
#define BULLET_00_HOSI_TEN BULLET_MARU10_BASE


/* === Gu�t�����g�� === */		/* --- �܂�����ĂȂ� --- */

enum	/* �� �t�����g�ʃG�t�F�N�g */
{	/* �����̂����蔻��ʒu�\���p�R�A(�t�����g�ʃG�t�F�N�g) */
	zzz_JIKI_CORE_00_REIMU_A	= (OBJ_Z05_FRONT),
	zzz_JIKI_CORE_01_REIMU_B,
	zzz_JIKI_CORE_02_MARISA_A,
	zzz_JIKI_CORE_03_MARISA_B,
	zzz_JIKI_CORE_04_MARISA_C,
	zzz_JIKI_CORE_05_REMILIA,
	zzz_JIKI_CORE_06_CHIRNO,
	zzz_JIKI_CORE_07_YUYUKO,
//	/* CORE�B���p */
	zzz_CORE_HIDE_10_REIMU_A,
	zzz_CORE_HIDE_11_REIMU_B,
	zzz_CORE_HIDE_12_MARISA_A,
	zzz_CORE_HIDE_13_MARISA_B,
	zzz_CORE_HIDE_14_MARISA_C,
	zzz_CORE_HIDE_15_REMILIA,
	zzz_CORE_HIDE_16_CHIRNO,
	zzz_CORE_HIDE_17_YUYUKO,
//	/* ������ / �U�R���Ŕ���(�t�����g�ʃG�t�F�N�g) */
	BAKUHA00,	/* ������(���F?�A���݃_�~�[) */
	BAKUHA01,	/* ������(��)BASE_TR_BLUE_PNG */
	BAKUHA02,	/* ������(��)BASE_TR_RED_PNG */
	BAKUHA03,	/* ������(��)BASE_TR_GREEN_PNG */
	BAKUHA04,	/* �U�R���Ŕ���(���F�A���݃_�~�[) */
	BAKUHA05,	/* �U�R���Ŕ���(��)BASE_BAKUHA05_PNG */
	BAKUHA06,	/* �U�R���Ŕ���(��)BASE_BAKUHA06_PNG */
	BAKUHA07,	/* �U�R���Ŕ���(��)BASE_BAKUHA07_PNG */
//	/* �Ή�����(�t�����g�ʃG�t�F�N�g) */
	BAKUHA08,	/* �Ή����� */
	BAKUHA09,	/* �Ή����� */
	BAKUHA0a,	/* �Ή����� */
	BAKUHA0b,	/* �Ή����� */
	BAKUHA0c,	/* �Ή����� */
	BAKUHA0d,	/* �Ή����� */
	BAKUHA0e,	/* �Ή����� */
	BAKUHA0f,	/* �Ή����� */
//0x20
	SPELL_SQUERE_,	/* [��] x 5 */
	PANEL_STR_EXTRA,//
	SP_FRONT_YUKI,// // SPELL_LOGO_,/* Border Power of Spiritual */ 	//	SPELL_TACHIE_,
	PANEL_STR_MAX,
	PANEL_STR_EASY,
	PANEL_STR_NORMAL,
	PANEL_STR_HARD,
	PANEL_STR_LUNATIC,
//0x28
	PANEL_STR_0_roman,	//[r39]PANEL_STR_EXTRA,
	PANEL_STR_1_roman,	//[r39]PANEL_STR_TIME,
	PANEL_STR_2_roman,	//[r39]PANEL_STR_fps,
	PANEL_STR_3_roman,	//[r39]PANEL_STR_0_roman,
	PANEL_STR_4_roman,	//[r39]PANEL_STR_1_roman,
	PANEL_STR_5_roman,	//[r39]PANEL_STR_2_roman,
	PANEL_STR_6_roman,	//[r39]PANEL_STR_3_roman,
	PANEL_STR_7_roman,	//[r39]PANEL_STR_4_roman,
//0x30
	PANEL_STR_8_roman,	//[r39]PANEL_STR_5_roman,
	PANEL_STR_9_roman,	//[r39]PANEL_STR_6_roman,
	SP_DUMMY_MUTEKI,	//[r39]PANEL_STR_7_roman,
	PANEL_STR_TIME, 	//PANEL_STR_8_roman,
	PANEL_STR_fps,		//PANEL_STR_9_roman,
	PANEL_STR_ene_my,	//PANEL_STR_ene_my,
	PANEL_STR_0_kanji,
	PANEL_STR_1_kanji,
//0x38
	PANEL_STR_2_kanji,
	PANEL_STR_3_kanji,
	PANEL_STR_4_kanji,
	PANEL_STR_5_kanji,
	PANEL_STR_6_kanji,
	PANEL_STR_7_kanji,
	PANEL_STR_8_kanji,
	PANEL_STR_9_kanji,
//0x40
};
/*
	SP_DUMMY_MUTEKI == FRONT �ʂȂ̂ł����蔻��Ȃ��B���ꂩ��\������Ȃ��B
	SP_FRONT_YUKI == FRONT �ʂȂ̂ł����蔻��Ȃ��B(�Ƃ肠��������)��݂����ȕ\���B
*/


/* Border Power of Spiritual. ��̂ŗ͂𔭊����鋫�E. ��̂ł̗͂̋��E */
/* Border Power of Spiritus.  �𐸂ŗ͂𔭊����鋫�E. �𐸂ł̗͂̋��E */
/*
Spiritus  ��
Spiritual ���
��̂̋��E��
Border Power of Spiritual. ��̂̃p���[�ɐڂ��Ă��������B
Border Power of Spiritus. �𐸂̃p���[�ɐڂ��Ă��������B
*/
