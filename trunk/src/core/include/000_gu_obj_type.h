
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

/*(r33)*/
/* === Gu�G�� === */		/* --- �܂�����ĂȂ� --- */
enum	/* �� �����w�ʃG�t�F�N�g */
{
/*
xx:���g�p�B
ok:�ϊ��ς݁B
*/
/*r34	r33*/
/*00xx	*/	TEKI_00_BOSS11			/* �{�X & ��-�{�X[�E��1] */ 					= /*0x0400*/SP_GROUP_TEKI,
/*01xx	*/	TEKI_01_BOSS12, 		/* �{�X & ��-�{�X[�E��2] */
/*02xx	*/	TEKI_02_BOSS13, 		/* �{�X & ��-�{�X[�E��3] */
/*03xx	*/	TEKI_03_BOSS14, 		/* �{�X & ��-�{�X[�E��4] */
/*04xx	*/	TEKI_04_BOSS21, 		/* �{�X & ��-�{�X[����1] */
/*05xx	*/	TEKI_05_BOSS22, 		/* �{�X & ��-�{�X[����2] */
/*06xx	*/	TEKI_06_BOSS23, 		/* �{�X & ��-�{�X[����3] */
/*07xx	*/	TEKI_07_BOSS24, 		/* �{�X & ��-�{�X[����4] */
//
/*08xx	*/	TEKI_08_BOSS31, 		/* �{�X & ��-�{�X[�U��1] */
/*09xx	*/	TEKI_09_BOSS32, 		/* �{�X & ��-�{�X[�U��2] */
/*10	*/	TEKI_10_BOSS33, 		/* �{�X & ��-�{�X[�U��3] */
/*11	*/	TEKI_11_BOSS34, 		/* �{�X & ��-�{�X[�U��4] */
/*12	*/	TEKI_12_EFFECT1_resurved,	/* r29���� ���g�p */
/*13	*/	TEKI_13_EFFECT2_resurved,	/* r29���� ���g�p */
/*14	*/	TEKI_14_EFFECT3_resurved,	/* r29���� ���g�p */
/*15	*/	TEKI_15_EFFECT4_resurved,	/* r29���� ���g�p */
//
/*16ok	*/	TEKI_16_YOUSEI1_1,		/* �d��1[�E����1] */
/*17xx	*/	TEKI_17_YOUSEI1_2,		/* �d��1[�E����2] */
/*18xx	*/	TEKI_18_YOUSEI1_3,		/* �d��1[�E����3] */
/*19xx	*/	TEKI_19_YOUSEI1_4,		/* �d��1[�E����4] */
/*20xx	*/	TEKI_20_YOUSEI1_5,		/* �d��1[���ʌ���1] */ 	/* BOSS_16_YOUSEI11 */
/*21xx	*/	TEKI_21_YOUSEI1_6,		/* �d��1[���ʌ���2] */
/*22xx	*/	TEKI_22_YOUSEI1_7,		/* �d��1[���ʌ���3] */
/*23xx	*/	TEKI_23_YOUSEI1_8,		/* �d��1[���ʌ���4] */
//
/*24	*/	TEKI_24_YOUSEI1_9,		/* �d��1[������1] */
/*25	*/	TEKI_25_YOUSEI1_a,		/* �d��1[������2] */
/*26	*/	TEKI_26_YOUSEI1_b,		/* �d��1[������3] */
/*27	*/	TEKI_27_YOUSEI1_c,		/* �d��1[������4] */
/*28ok	*/	TEKI_28_MAHOJIN,		/* ���̖����w */
/*29ok	*/	TEKI_29_INYOU_DAI,
/*30ok	*/	TEKI_30_CHOU1,
/*31xx	*/	TEKI_31_CHOU2,
//
/*32ok	*/	TEKI_32_YOUSEI2_1,		/* �d��2[�E����1] */
/*33	*/	TEKI_33_YOUSEI2_2,		/* �d��2[�E����2] */
/*34	*/	TEKI_34_YOUSEI2_3,		/* �d��2[�E����3] */
/*35	*/	TEKI_35_YOUSEI2_4,		/* �d��2[�E����4] */
/*36ok	*/	TEKI_36_YOUSEI2_5,		/* �d��2[���ʌ���1] */
/*37	*/	TEKI_37_YOUSEI2_6,		/* �d��2[���ʌ���2] */
/*38	*/	TEKI_38_YOUSEI2_7,		/* �d��2[���ʌ���3] */
/*39	*/	TEKI_39_YOUSEI2_8,		/* �d��2[���ʌ���4] */
//
/*40	*/	TEKI_40_YOUSEI2_9,		/* �d��2[������1] */
/*41	*/	TEKI_41_YOUSEI2_a,		/* �d��2[������2] */
/*42	*/	TEKI_42_YOUSEI2_b,		/* �d��2[������3] */
/*43	*/	TEKI_43_YOUSEI2_c,		/* �d��2[������4] */
/*44ok	*/	TEKI_44_CHEN,
/*45xx	*/	TEKI_45_BLUE_BOOK,
/*46ok	*/	TEKI_46_RED_BOOK,
/*47ok	*/	TEKI_47_HAI_KEDAMA,
//
/*48ok	*/	TEKI_48_YOUSEI3_1,		/* �d��3[�E����1] */
/*49	*/	TEKI_49_YOUSEI3_2,		/* �d��3[�E����2] */
/*50	*/	TEKI_50_YOUSEI3_3,		/* �d��3[�E����3] */
/*51	*/	TEKI_51_YOUSEI3_4,		/* �d��3[�E����4] */
/*52	*/	TEKI_52_YOUSEI3_5,		/* �d��3[���ʌ���1] */
/*53	*/	TEKI_53_YOUSEI3_6,		/* �d��3[���ʌ���2] */
/*54	*/	TEKI_54_YOUSEI3_7,		/* �d��3[���ʌ���3] */
/*55	*/	TEKI_55_YOUSEI3_8,		/* �d��3[���ʌ���4] */
//
/*56	*/	TEKI_56_YOUSEI3_9,		/* �d��3[������1] */
/*57	*/	TEKI_57_YOUSEI3_a,		/* �d��3[������2] */
/*58	*/	TEKI_58_YOUSEI3_b,		/* �d��3[������3] */
/*59	*/	TEKI_59_YOUSEI3_c,		/* �d��3[������4] */
/*60ok	*/	TEKI_60_AKA_KEDAMA,
/*61ok	*/	TEKI_61_NIJI_HOSI,
/*62ok	*/	TEKI_62_TATSUMAKI,
/*63	*/	TEKI_63_HOMING_STUFF,
};
#define BOSS_00_BOSS11		TEKI_00_BOSS11
#define BOSS_16_YOUSEI11	TEKI_20_YOUSEI1_5

/* === Gu�����w�� === */		/* --- �܂�����ĂȂ� --- */

//enum	/* �� �����w�ʃG�t�F�N�g */
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

/* === Gu�t�����g�� === */		/* --- �܂�����ĂȂ� --- */

enum	/* �� �t�����g�ʃG�t�F�N�g */
{	/* �����̂����蔻��ʒu�\���p�R�A(�t�����g�ʃG�t�F�N�g) */
	zzz_JIKI_CORE_00_REIMU_A	= /*0x4000*/SP_GROUP07_FRONT,
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
//
	SPELL_SQUERE_,	/* [��] x 5 */
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
	SP_DUMMY_MUTEKI == FRONT �ʂȂ̂ł����蔻��Ȃ��B���ꂩ��\������Ȃ��B
	SP_FRONT_YUKI == FRONT �ʂȂ̂ł����蔻��Ȃ��B(�Ƃ肠��������)��݂����ȕ\���B
*/
//#define SP_DUMMY_MUTEKI S P_GROUP_ETC_DUMMY_REMILIA
//#define SP_FRONT_YUKI 	S P_GROUP_ETC_DUMMY_SLOW_BOMB

/* Border Power of Spiritual. ��̂ŗ͂𔭊����鋫�E. ��̂ł̗͂̋��E */
/* Border Power of Spiritus.  �𐸂ŗ͂𔭊����鋫�E. �𐸂ł̗͂̋��E */
/*
Spiritus  ��
Spiritual ���
��̂̋��E��
Border Power of Spiritual. ��̂̃p���[�ɐڂ��Ă��������B
Border Power of Spiritus. �𐸂̃p���[�ɐڂ��Ă��������B
*/

/* === Gu�v���C���[�� === */
enum/* �I�v�V�����ƃC���^�[���[�u���Ă�̂́A(�I�v�V�����A�j��)�v���O�����̓s�� */
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
	JIKI_BOMBER_01, 		/* �U���{��(�ᑬ�{��) */
	JIKI_BOMBER_02, 		/* --- ���g�p --- / ���E�� / ����1 */
	JIKI_BOMBER_03, 		/* --- ���g�p --- / ���E�� / ����2 */
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

/* === Gu�e���� === */

/* TAMA_GROUIP_08_MASK_0xfff8: 8 �e == 1 �O���[�v�Ƃ��ĊǗ�����ꍇ�ɁA�O���[�v��ʂ�����ׂ̃}�X�N�l */
#define TAMA_GROUIP_08_MASK_0xfff8 (0xfff8)
#define is_tama_grouip08(aaa) (aaa & TAMA_GROUIP_08_MASK_0xfff8)
enum
{
//1 ���ےe
	BULLET_00_HOSI_TEN			= SP_GROUP_BULLETS/*0x0800*/,
	BULLET_MARU10_01_AKA,			// ��
	BULLET_MARU10_02_YUKARI,		// ��
	BULLET_MARU10_03_AOI,			// ��
	BULLET_MARU10_04_MIZU_IRO,		// ��
	BULLET_MARU10_05_MIDORI,		// ��
	BULLET_MARU10_06_KI_IRO,		// ��
	BULLET_MARU10_07_DAI_DAI,		// ��
//2 �Ēe
	BULLET_KOME_00_SIRO,			// ��
	BULLET_KOME_01_AKA, 			// ��
	BULLET_KOME_02_YUKARI,			// ��
	BULLET_KOME_03_AOI, 			// ��
	BULLET_KOME_04_MIZU_IRO,		// ��
	BULLET_KOME_05_MIDORI,			// ��
	BULLET_KOME_06_KI_IRO,			// ��
	BULLET_KOME_07_DAI_DAI, 		// ��
//3 �~�j�e
	BULLET_MINI8_00_SIRO,			// ��
	BULLET_MINI8_01_AKA,			// ��
	BULLET_MINI8_02_YUKARI, 		// ��
	BULLET_MINI8_03_AOI,			// ��
	BULLET_MINI8_04_MIZU_IRO,		// ��
	BULLET_MINI8_05_MIDORI, 		// ��
	BULLET_MINI8_06_KI_IRO, 		// ��
	BULLET_MINI8_07_DAI_DAI,		// ��
//4 �g�t�����ےe
	BULLET_WAKU12_00_SIRO,			// ��	/*12*/	[�`���m]
	BULLET_WAKU12_01_AKA,			// ��	/*12*/	[�`���m]
	BULLET_WAKU12_02_KURO_AKA,		// [��] /*12*/	���~���A�p�̍��g�Ԓe			//	BULLET_CAP16_04_KOME_SIROI, 	/* [���Ēe] */
	BULLET_WAKU12_03_AOI,			// ��	/*12*/	[�`���m]
	BULLET_CAP16_04_TUTU_SIROI, 	// [��] /*12*/						/* [������e] */
	BULLET_WAKU12_05_MIDORI,		// ��	/*12*/	[�`���m]
	BULLET_WAKU12_06_KI_IRO,		// ��	/*12*/	[�`���m]	//	BULLET_MINI8_12_dummy,
	BULLET_WAKU12_07_DAIDAI,		// ��	/*12*/	[�`���m]	//	BULLET_MINI8_13_dummy,
//5 �ӂ�����e(�ؒe)
	BULLET_UROKO14_00_SIRO, 		// ��
	BULLET_UROKO14_01_AKA,			// ��
	BULLET_UROKO14_02_YUKARI,		// ��
	BULLET_UROKO14_03_AOI,			// ��
	BULLET_UROKO14_04_MIZUIRO,		// ��
	BULLET_UROKO14_05_MIDORI,		// ��
	BULLET_UROKO14_06_KI_IRO,		// ��
	BULLET_UROKO14_07_DAIDAI,		// ��
//6 �N�i�C�e
	BULLET_KUNAI12_00_SIRO, 		// ��
	BULLET_KUNAI12_01_AKA,			// ��
	BULLET_KUNAI12_02_YUKARI,		// ��
	BULLET_KUNAI12_03_AOI,			// ��
	BULLET_KUNAI12_04_MIZUIRO,		// ��
	BULLET_KUNAI12_05_MIDORI,		// ��
	BULLET_KUNAI12_06_KI_IRO,		// ��
	BULLET_KUNAI12_07_DAIDAI,		// ��
//7 �D�e / �i�C�t�e
	BULLET_OFUDA12_00_AOI,
	BULLET_OFUDA12_01_AKA,
	BULLET_OFUDA12_02_YUKARI,
	BULLET_OFUDA12_03_MIDORI,
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

/* === GuITEM�� === */
enum /*sprite_type*/
{
/*	�A�C�e�� */
	SP_ITEM_00_P001 	/* [p]�� */ 								= /*0x1000*/SP_GROUP_ITEMS, 	/* Bonus items */
	SP_ITEM_01_P008,	/* [P]�� */
	SP_ITEM_02_P128,	/* [F] */
	SP_ITEM_03_1UP, 	/* [1up] */
	SP_ITEM_04_BOMB,	/* [B] */
	SP_ITEM_05_TENSU,	/* [�_] */
	SP_ITEM_06_HOSI,	/* [��] */
	SP_ITEM_07_SPECIAL,
};

/* === GuFRONT�� === */
