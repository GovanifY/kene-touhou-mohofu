/*
�y���zPSP�Œe��STG��肽���z��肽���z15�y���z
722 �F231�F2009/09/08(��) 23:17:49 ID:qnDIO3of
�͕핗�̗\��ɂ���
�X�y�J�͎����\�肪����܂��B�͕핗��������z�肵�Ă���X�y�J�V�X�e���͈ȉ��̂悤�Ȃ��̂ł��B

�{�X �� spell_card.c �� danmaku.c �� bullet_object.c �Ƃ����o�H���l���Ă��܂��B
���̂悤�ɕ����Ă���̂́A���݂܂�����܂��񂪁A
���U�R �� danmaku.c �� bullet_object.c �Ƃ����o�H���l���Ă��邩��ł��B

spell_card.c �̓X�y�J���Ǘ�����V�X�e���ł��B

�{�X���X�y�J�����Ă���̗͂�����ꍇ�B�X�y�J�ɓo�^���ꂽ���̗͂ƈ��������ɃX�y�J�������܂��B
�X�y�J�����Ă�`�����X�������Ă��A�X�y�J�ɓo�^���ꂽ���̗͂ɖ����Ȃ��ꍇ�̓X�y�J�������܂���B(���Ă܂���)

�{�X���X�y�J�����Ɠ����Ƀ{�X�̗̑͂���X�y�J����������āA�X�y�J���[�h�ɂȂ�܂��B
�X�y�J���[�h���Ƀ{�X���U������ƃ{�X�̗̑͂͌��炳�Ȃ��ŁA�X�y�J(spell_card.c)���Ǘ����Ă���
�X�y�J���̗̑�(�X�y�J�Q�[�W���\��)�����炵�܂��B
�X�y�J���[�h���ɃX�y�J�ɓo�^���ꂽ��莞�Ԃ��o�߂��邩�A�X�y�J(spell_card.c)���Ǘ����Ă���
�X�y�J���̗̑�(�X�y�J�Q�[�W)�������Ȃ�ƒʏ탂�[�h�ɕ��A���܂��B
���̂Ƃ��X�y�J���̗̑�(�X�y�J�Q�[�W)�͋����I�� 0 �ɂȂ�܂��B

���̂悤�ȃX�y�J�V�X�e����z�肵�Ă���ׁA�X�y�J�V�X�e���ɑ̗͂����炳����悤�Ƀ{�X������������K�v������܂��B
�X�y�J�V�X�e�������̂����̂ƃ{�X�̉����ɂ͎��Ԃ��|����(���Ԃ񎟉�(�㌎��2009-09-27�\��)�ɂ͊Ԃɍ����܂���)�̂�
�C���ɂ��҂����������B

��L�ȊO�̈Ă̎����\��͂���܂��񂪁A�����Ɨǂ��Ă�����΂����ɏ����Ă�����
�Q�l�ɂ��܂����A�ǂ��ĂȂ�Ύ������邩������܂���B

*/

#include "bullet_object.h"
#include "scenario_script.h"

/*---------------------------------------------------------
	�X�y�J�Ǘ��V�X�e��
	-------------------------------------------------------
	���X�y�J�̓V�X�e���ŃT�|�[�g����\�z������܂��B(�\�z��)
	�V�X�e���ŃT�|�[�g�����ƁA�ǂ̃{�X���A�ǂ̃X�y�J�ł����Ă�悤�ɂȂ�܂��B
	-------------------------------------------------------
	ToDo:
	���U���̊Ǘ�
	������̊Ǘ�(���\����)
---------------------------------------------------------*/
// ��� 6��
extern void sakuya_11_keitai(SPRITE *src);
extern void sakuya_10_keitai(SPRITE *src);
extern void sakuya_09_keitai(SPRITE *src);
extern void sakuya_06_keitai(SPRITE *src);
extern void sakuya_04_keitai(SPRITE *src);
extern void sakuya_03_keitai(SPRITE *src);
extern void sakuya_02_keitai(SPRITE *src);
extern void sakuya_01_keitai(SPRITE *src);

// �p�`�F 5��
//tern void pache_05_keitai(SPRITE *src);
extern void pache_04_keitai(SPRITE *src);
//tern void pache_03_keitai(SPRITE *src);
//tern void pache_02_keitai(SPRITE *src);
extern void pache_01_keitai(SPRITE *src);

// �� 4��
extern void aya_05_keitai(SPRITE *src);
extern void aya_04_keitai(SPRITE *src);
//tern void aya_03_keitai(SPRITE *src);
extern void aya_02_keitai(SPRITE *src);
extern void aya_01_keitai(SPRITE *src);

// �P�� 3��
extern void add_zako_kaguya_houmotsu(SPRITE *src);
//tern void kaguya_06_keitai(SPRITE *src);
//tern void kaguya_05_keitai(SPRITE *src);
extern void kaguya_04_keitai(SPRITE *src);
//tern void kaguya_03_keitai(SPRITE *src);
//tern void kaguya_02_keitai(SPRITE *src);
extern void kaguya_01_keitai(SPRITE *src);

// ���� 2��
extern void add_zako_mima_dolls(SPRITE *src);
extern void mima_01_keitai(SPRITE *src);

// �A���X 1��
extern void alice_02_keitai(SPRITE *src);/* �����u�M���̕������l�`�v^^); */
extern void alice_01_keitai(SPRITE *src);/* �ʏ�U�� */

// ���ʌ`��
extern void init_00_keitai(SPRITE *src);	/* �_�~�[ */
extern void common_00_keitai(SPRITE *src);
//extern void common_99_keitai(SPRITE *src);/* ���j��ɉ�ʊO�Ƀ{�X�������� */


//------------ �X�y�J�֘A
/*extern*/ int spell_card_limit_health; 	/* �K��l�ȉ��ɂȂ�΃X�y�J���[�h���� */
/*extern*/ int spell_card_boss_state;		/* ���l�ɂȂ�΃{�X�X�y�J���[�h�ɓ���Ȃ� */
/*extern*/ int spell_card_mode; 			/* �X�y�J���[�h */
/*extern*/ int spell_card_boss_timer;		/* ���p */	// ��������

/*extern*/ int spell_card_number;			/* ���p */	// �X�y�J�ԍ�
/*extern*/ int spell_card_max;				/* ���p */	// �X�y�J�ԍ��ő���E�l

typedef struct
{
	int spell_life; 	/* �X�y�J�ɓo�^���ꂽ���̗� */
	int spell_time; 	/* �X�y�J�ɓo�^���ꂽ��莞�� */
	void (*spell_init_callback)(SPRITE *sss);	/* �������ړ����� */
	void (*spell_move_callback)(SPRITE *sss);	/* �X�y�J�ړ����� */
	const char *spell_str_name; 	/* �X�y�J���� */
} SPELL_CARD_RESOURCE;

//	{	 500,	  20,		"�X���u�J����000�v" },/*NULL*/		/* �X�y�J�������Ȃ� */
	/* ���O�̓e�L�g�[�ł� */

/* �X�y������(==�P��[�t���[��])�́A64�{(==�P��[��1�b])���K�i */
#define s_time(aaa) ((aaa)<<6)

/*static*/ SPELL_CARD_RESOURCE my_111resource[SPELL_CARD_MAX] =
{																				//	�ő唼�p��31�����B
// ��� 6�� 																	"feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{	73728,	  s_time(200),	init_00_keitai, 			common_00_keitai,	NULL/*"�`�ԕύX" "\n"*/	 				  },/* 73728==8192*9 8192==1024*8 */
	{	 5000,	  s_time(20),	init_00_keitai, 			sakuya_01_keitai,	NULL/*"�ʏ�U��" "\n"*/ 				  },	  /*  "���`��: �ʏ�U��(���E�ɓ������S�e����)"	  */  /* ����̍U����(�蓮�ݒ�)  500 160  */
	{	 5000,	  s_time(20),	init_00_keitai, 			sakuya_02_keitai,	" �@�@��p�u�~�X�f�B���N�V�����v" "\n"	  },	  /*  "���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)"  */
	{	 5000,	  s_time(20),	init_00_keitai, 			sakuya_03_keitai,	" �@�@���J�u�o�[�e�B�J���i�C�t�v" "\n"	  },	  /*  "��O�`��: (�����i�C�t�������Ă����)"		  */
	{	 3000,	  s_time(20),	init_00_keitai, 			sakuya_04_keitai,	" �@�����u�A�����~�e�b�h���C�h�v" "\n"	  },	  /*  "��l�`��: �����w����"						  */
	{	10000,	  s_time(20),	init_00_keitai, 			sakuya_06_keitai,	" �@����u�f���W�����X���[���h�v" "\n"	  },	  /*  "��܌`��: (���F�}�X�J�b�g�e)"				  */
//	{	 5000,	  s_time(20),	init_00_keitai, 			common_00_keitai,	"�@�@�@�@�@��p�u���e�X�g007�v" "\n"	  },	  /*  "�掵�`��: (���U�����w)�ǉ��v�撆"			  */
//	{	 5000,	  s_time(20),	init_00_keitai, 			common_00_keitai,	"�@�@�@�@�@��p�u���e�X�g008�v" "\n"	  },	  /*  "�攪�`��: (���~�߃i�C�t)�ǉ��v�撆"			  */
	{	25000,	  s_time(20),	init_00_keitai, 			sakuya_09_keitai,	" �@�����u�t�F�X�e�B�o���i�C�t�v" "\n"	  },	  /*  "���`��: �ŏI�`��(����1)"					  */
	{	20000,	  s_time(20),	init_00_keitai, 			sakuya_10_keitai,	" �@�@�@�@�����u�`�F�b�N���C�h�v" "\n"	  },	  /*  "��10�`��: �ŏI�`��(����2)"					  */
	{	90000,	  s_time(20),	init_00_keitai, 			sakuya_11_keitai,	" �@�@���C�h��Z�u�t�@�C�i�����v" "\n"	  },	  /*  "��11�`��: �ŏI�`��(����3)"					  */

// �p�`�F 5��																	"feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{	65536,	  s_time(250),	init_00_keitai, 			common_00_keitai,	NULL/*"�`�ԕύX" "\n"*/	 				  },/* 65536==8192*8 3069 150 120 ����̍U����(�蓮�ݒ�) */
	{	 5000,	  s_time(20),	init_00_keitai, 			pache_01_keitai,	NULL/*"�ʏ�U��" "\n"*/ 				  },  /* ����̍U����(�蓮�ݒ�) */
	{	10000,	  s_time(40),	init_00_keitai, 			pache_01_keitai,	"�@�@�Ε��u�p�`�����[���쐬000�v" "\n"	  },
	{	10000,	  s_time(20),	init_00_keitai, 			pache_04_keitai,	"�@�@�����u�p�`�����[���쐬001�v" "\n"	  },
	{	10000,	  s_time(40),	init_00_keitai, 			pache_04_keitai,	"�@�@�ؕ��u�V���t�B�[���쐬002�v" "\n"	  },
	{	10000,	  s_time(40),	init_00_keitai, 			pache_04_keitai,	"�@�@�����u�p�`�����[���쐬003�v" "\n"	  },
	{	10000,	  s_time(40),	init_00_keitai, 			pache_04_keitai,	"�@�@�y���u�p�`�����[���쐬004�v" "\n"	  },
	{	10000,	  s_time(40),	init_00_keitai, 			pache_04_keitai,	"�@�@�����u�p�`�����[���쐬005�v" "\n"	  },
//	{	10000,	  s_time(40),	init_00_keitai, 			pache_04_keitai,	"�@�@�����u�T�C�����g���쐬006�v" "\n"	  },
//	{	10000,	  s_time(40),	init_00_keitai, 			pache_04_keitai,	"�@�@�X���u�p�`�����[���쐬007�v" "\n"	  },

// �� 4��																		"feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{	40960,	  s_time(150),	init_00_keitai, 			common_00_keitai,	NULL/*"�`�ԕύX" "\n"*/	 				  },/* 40960==8192*5 49152==8192*6 */
	{	 5000,	  s_time(20),	init_00_keitai, 			aya_01_keitai,		NULL/*"�ʏ�U��" "\n"*/ 				  },/*4*/	  /* ����̍U����(�蓮�ݒ�) 500 120  */
	{	 5000,	  s_time(30),	init_00_keitai, 			aya_02_keitai,		"�@�@�@ �ؕ��u�F�؈�ࣁv�̃e�X�g" "\n"	  },/*4*/
	{	 5000,	  s_time(20),	init_00_keitai, 			aya_04_keitai,		"�@�@�@�@�@�@ �H���u����̒��ׁv" "\n"	  },/*5"�򕄁u���쐬�X�y�J001�v"*/
	{	 8000,	  s_time(40),	init_00_keitai, 			aya_04_keitai,		"�@�@�@ �����u�n���卪���]��Q�v" "\n"	  },/*4"�򕄁u���쐬�X�y�J002�v"*/
	{	17000,	  s_time(100),	init_00_keitai, 			aya_05_keitai,		"�@�@�@�@�@�@ �����u�����R�R�l�v" "\n"	  },/*5"�򕄁u���쐬�X�y�J003�v"*/
	{	10000,	  s_time(40),	init_00_keitai, 			aya_05_keitai,		" �@�@�@ �򕄁u���쐬�X�y�J004�v" "\n"	  },/*4"�򕄁u���쐬�X�y�J004�v"*/
	{	10000,	  s_time(40),	init_00_keitai, 			aya_05_keitai,		" �@�@�@ �򕄁u���쐬�X�y�J005�v" "\n"	  },/*5*/
	{	10000,	  s_time(40),	init_00_keitai, 			aya_05_keitai,		" �@�@�@ �򕄁u���쐬�X�y�J006�v" "\n"	  },/*5*/
	{	10000,	  s_time(40),	init_00_keitai, 			aya_05_keitai,		" �@�@�@ �򕄁u���쐬�X�y�J007�v" "\n"	  },/*5*/

// �P�� 3�� 																	"feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{	40960,	  s_time(250),	add_zako_kaguya_houmotsu,	common_00_keitai,	NULL/*"�`�ԕύX" "\n"*/	 				  },/* 40960==8192*5 */
	{	 5000,	  s_time(23),	add_zako_kaguya_houmotsu,	kaguya_01_keitai,	NULL/*"�ʏ�U��" "\n"*/ 				  },  /* ����̍U����(�蓮�ݒ�) 500 99	*/	  // ����(5)�҂�e�A�ԁA�B���@�_���A�e(8)��18�Z�b�g(�b)���炢(�n�߂�5�b�͏o���Ȃ�)�B(�P����Ԋu�ňړ��A�I���㒆���ֈړ�)
	{	 5000,	  s_time(40),	add_zako_kaguya_houmotsu,	kaguya_01_keitai,	"�@�@�@�@�@�@�@���u���쐬001�v" "\n"	  },// 1.������̋�   (�P�钆���Œ�A�Ԏ��Ή��̊ےe�A�Z���[�U�[)4�Z�b�gx
	{	 5000,	  s_time(40),	add_zako_kaguya_houmotsu,	kaguya_01_keitai,	"�@�@�@�@�@�@�@���u���쐬002�v" "\n"	  },// 2.���̌�΂̔�
	{	 7000,	  s_time(40),	add_zako_mima_dolls,		kaguya_04_keitai,	"�@�@�@�@�@�@�@���u���쐬003�v" "\n"	  },// 3.�Αl�̔��
	{	 7000,	  s_time(40),	add_zako_mima_dolls,		kaguya_04_keitai,	"�@�@�@�@�@�@�@���u���쐬004�v" "\n"	  },// 4.���̎q���L
	{	 7000,	  s_time(40),	add_zako_mima_dolls,		kaguya_04_keitai,	"�@�@�@�@�@�@�@���u���쐬005�v" "\n"	  },// 5.�H���̋ʂ̎}
	{	 7000,	  s_time(40),	add_zako_mima_dolls,		kaguya_04_keitai,	"�@�@�@�@�@�@�@���u���쐬006�v" "\n"	  },//

// ���� 2�� 																	"feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{	57344,	  s_time(200),	init_00_keitai, 			common_00_keitai,	NULL/*"�`�ԕύX" "\n"*/	 				  },/* 57344==8192*7 24576==8192*3 */
	{	 8192,	  s_time(20),	add_zako_mima_dolls,		mima_01_keitai, 	NULL/*"�ʏ�U��" "\n"*/ 				  },  /* ����̍U����(�蓮�ݒ�) 500 40	*/
	{	 8192,	  s_time(20),	add_zako_mima_dolls,		mima_01_keitai, 	"�@�@�@�@����u���쐬�X�y�J001�v" "\n"	  },
	{	 8192,	  s_time(40),	add_zako_mima_dolls,		mima_01_keitai, 	"�@�@�@�@����u���쐬�X�y�J002�v" "\n"	  },
	{	 8192,	  s_time(40),	add_zako_mima_dolls,		mima_01_keitai, 	"�@�@�@�@����u���쐬�X�y�J003�v" "\n"	  },
	{	 8192,	  s_time(40),	add_zako_mima_dolls,		mima_01_keitai, 	"�@�@�@�@����u���쐬�X�y�J004�v" "\n"	  },
	{	 8192,	  s_time(40),	add_zako_mima_dolls,		mima_01_keitai, 	"�@�@�@�@����u���쐬�X�y�J005�v" "\n"	  },
	{	 8192,	  s_time(40),	add_zako_mima_dolls,		mima_01_keitai, 	"�@�@�@�@����u���쐬�X�y�J006�v" "\n"	  },

// �A���X 1��																	"feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{	12288,	  s_time(100),	init_00_keitai, 			common_00_keitai,	NULL/*"�`�ԕύX" "\n"*/	 				  },/* 12288==8192*1.5 16384==8192*2 */   /* "�ʏ�U��"�̎���(?) */ /* "�ʏ�U��"�̃��C�t */
	{	 2000,	  s_time(10),	init_00_keitai, 			alice_01_keitai,	NULL/*"�ʏ�U��" "\n"*/ 				  },  /* ����̍U����(�蓮�ݒ�) 1023 75 */
	{	10000,	  s_time(20),	init_00_keitai, 			alice_02_keitai,	" �@�����u�M���̃}�g�����[�V�J�v" "\n"	  },/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 5000,	  s_time(40),	init_00_keitai, 			alice_02_keitai,	"�@�����u���쐬�̈����ɐl�`001�v" "\n"	  },/* �����u�����̉~�J�l�`�v */
	{	20000,	  s_time(20),	init_00_keitai, 			alice_02_keitai,	"�����u���쐬�̈ɑ������l�`002�v" "\n"	  },/* �����u�󑐂̐l�`�Ă��v */
//	{	 5000,	  s_time(20),	init_00_keitai, 			alice_02_keitai,	"�@�����u���쐬�̉p�g���l�`003�v" "\n"	  },
//	{	 5000,	  s_time(20),	init_00_keitai, 			alice_02_keitai,	"�@�����u���쐬�̘I�����l�`004�v" "\n"	  },
//	{	 5000,	  s_time(80),	init_00_keitai, 			alice_02_keitai,	"�@�@�����u���쐬�̐����l�`005�v" "\n"	  },
//	{	 5000,	  s_time(80),	init_00_keitai, 			alice_02_keitai,	"�@�����u���쐬�̔����`�l�`006�v" "\n"	  },
//	{	 5000,	  s_time(80),	init_00_keitai, 			alice_02_keitai,	"�@�@�����u���쐬�̔g���l�`007�v" "\n"	  },
//																				"feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
//
	/*
	�}�g�����[�V�J�́A�I�����l�`�ł����̂��ȁH�l�`�Ƃ�������薯�|�i�̎����B
	�ŋ߂͖G���}�g�����[�V�J�̎��v�����{���炠��炵���B
	//
	���{�̋C�ے��͕���17m�ȏ�̕����u�䕗�v�ƒ�`�B
	���ۋC�ۋ@��(WMO)�ł͕���33m�ȏ�̂��̂��utyphoon�v�ƒ�`�B����ȉ��́ustorm�v�B
	*/

};

/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]	�X�y�J����
---------------------------------------------------------*/

/*static*/ void spell_card_generator222(SPRITE *src)
{
	(my_111resource[(spell_card_number)].spell_move_callback)(src);
	/*---------------------------------------------------------
		�X�y�J�`�F�b�N
	---------------------------------------------------------*/
	{
		if (0/*off*/==spell_card_mode)
		{
			if (0/*off*/==spell_card_boss_timer)
			{
				spell_card_limit_health = 0;
				spell_card_mode 		= (1);/*on*/
			}
		}
	}
}

/*---------------------------------------------------------
	�X�y�J���̕\��
---------------------------------------------------------*/
static void draw_spell_card_name(void)
{
	script_message_window_clear();/*�X�N���v�g���b�Z�[�W��ʂ�����*/
	msg_time = (60*5);
	print_kanji000(/*SDL_Surface *drawmap,*/ /*SDL_Rect *rect*/ /*0,*/
		my_111resource[(spell_card_number)].spell_str_name, //	aaa_str[(spell_card_number&15)],
		/*int color_type*/(7)|STR_CODE_NO_ENTER,	/* ���s���Ȃ� */
		/*int wait*/(0)
	);
}


/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]	�X�y�J���j��A�C�e���o��
---------------------------------------------------------*/
//	#define alice_put_items common_boss_put_items
//	#define mima_put_items common_boss_put_items
//	#define kaguya_put_items common_boss_put_items
//	#define pache_put_items common_boss_put_items

/*---------------------------------------------------------
	�{�X�`�ԕύX���̋��ʃ��[�`��
---------------------------------------------------------*/
extern void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;				/* �w�i�E�B���h�E�\���t���O */
extern void *callback_gu_draw_haikei_supeka;
extern void *callback_gu_draw_haikei_modosu;
//
//extern void draw_spell_card_name(void);/* spell_card.c */
void common_boss_put_items(SPRITE *src)
{
//++	pd_bo ssmode=B04_CHANGE;
	bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
//
	callback_gu_draw_haikei = callback_gu_draw_haikei_supeka;
//
	voice_play(VOICE07_BOMB, TRACK02_ALEART_IVENT);/*�e�L�g�[*/
	#if (1)
	draw_spell_card_name();
	#endif

}


/*---------------------------------------------------------
	�{�X���U�������ꍇ�̃t���[���`�F�b�N
	-------------------------------------------------------
---------------------------------------------------------*/
/*
	4096/60 == 68.2666666666666666666666666666667
	4096/256 == 16
*/
//#define LIMIT_MAX_HP_DEC_BOSS_BY_FLAME (16)
#define LIMIT_MAX_HP_DEC_BOSS_BY_FLAME (48)
extern int boss_hp_dec_by_frame;/*�{�X�U�������l�A�t���[���P��*/
void boss_hp_frame_check(void)
{
	if (LIMIT_MAX_HP_DEC_BOSS_BY_FLAME < boss_hp_dec_by_frame)
	{
		boss_hp_dec_by_frame = LIMIT_MAX_HP_DEC_BOSS_BY_FLAME;
	}
//
		pd_boss->base_hp -= boss_hp_dec_by_frame;
		boss_hp_dec_by_frame = 0;/*�g�����̂ŏ���(�t���[���P��)*/
		if (spell_card_limit_health >= /*data->boss_base.boss*/pd_boss->base_hp)		/* �K��l�ȉ��ɂȂ�΃X�y�J���[�h���� */
		{
			spell_card_mode 			= 0/*off*/;
			boss_destroy_check_type(pd_boss, DESTROY_CHECK_00_WIN_BOSS);
			callback_gu_draw_haikei = callback_gu_draw_haikei_modosu;
		}

}
/*---------------------------------------------------------

---------------------------------------------------------*/
#if 0
/*extern*/ void create_spell_card(SPRITE *src, int spell_card_type)
{
//
	int aaa;	/* ���ݑ̗� ���� ���������X�y�J�� �������̗͒l */
	aaa = ((/*data->boss_base.boss*/src->base_hp) - (my_111resource[spell_card_type].spell_life));
	/* �{�X���X�y�J�����Ă���̗͂�����ꍇ */
	if ( 0 < aaa )
	{
		spell_card_limit_health = aaa;
		spell_card_mode 		= (1)/*on*/;
	}
}
#endif

/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]	�X�y�J�o�^
---------------------------------------------------------*/

/*static*/ void regist_spell_card222(SPRITE *src)
{
//	spell_card_limit_health -= 1000/*500*/;
	spell_card_limit_health -= (my_111resource[(spell_card_number+1)].spell_life);	/*1000 500*/
	/* �{�X���X�y�J�����Ă���̗͂�����ꍇ */
	if (0 < spell_card_limit_health)
	{
	//	if (b_health_alter_low1024 < (data->boss_base.boss_health & (1024-1)))/* �`�ԕύX������ */
		spell_card_mode 		= (1);/*on*/
		spell_card_number++;
		if (spell_card_max < spell_card_number)
		{
			/* �`�ԕύX���Ȃ��A�A�C�e���f���Ȃ� */
			spell_card_number--;
		//	src->callback_loser 			= lose_boss;
		}
		else
//		if (0==sakuya->base_hp)
		{
//			if (0 != (data->boss_base.boss_life))/* �`�ԕύX������ */
			/* �A�C�e���f�� */
			if (NULL != src->callback_loser)
			{
//				data->boss_base.boss_life--;
				(src->callback_loser)(src); 	/* sakuya_put_items(src); */
			}
		}
		/* �X�y���J�[�h������ */
		(my_111resource[(spell_card_number)].spell_init_callback)(src);
	}
	else
	{
		spell_card_limit_health = (0);
		spell_card_mode 		= (0);/*off*/
	}
}


/*---------------------------------------------------------
	�{�X�ړ������̋��ʃ��[�`��
	-------------------------------------------------------
�� �ړ��͈͂̓o�^�A�@�\�F
	���炩���߃{�X�ړ��͈͂�o�^���Ă����B
�� �͈͓��Ȃ�Έړ��A�@�\�F
	�ړ��͈͓��Ȃ�΁A�{�X���ړ�����B
	�ړ��͈͊O�Ȃ�΁A�{�X�͈ړ����Ȃ��B
�� �ړ�����A�@�\�F
	�{�X���ړ����Ȃ��ꍇ�́A�u�ړ��ł��Ȃ������t���O�v���n�m�ɂȂ�B
---------------------------------------------------------*/
/*extern*/ int boss_hamidasi;			/* �u�ړ��ł��Ȃ������t���O�v(�g�p�O�Ɏ蓮��OFF==0�ɂ��Ƃ�) */
/*extern*/ POINT256 boss_clip_min;		/* �{�X�ړ��͈�(�ŏ��l) */
/*extern*/ POINT256 boss_clip_max;		/* �{�X�ړ��͈�(�ő�l) */
void boss_move_clip_rect(SPRITE *src)
{
	src->x256 += (src->vx256);
		 if (src->x256 < boss_clip_min.x256 )	{	src->x256 = boss_clip_min.x256; 	boss_hamidasi=1;	}
	else if (src->x256 > boss_clip_max.x256 )	{	src->x256 = boss_clip_max.x256; 	boss_hamidasi=1;	}
//
	src->y256 += (src->vy256);
		 if (src->y256 < boss_clip_min.y256 )	{	src->y256 = boss_clip_min.y256; 	boss_hamidasi=1;	}
	else if (src->y256 > boss_clip_max.y256 )	{	src->y256 = boss_clip_max.y256; 	boss_hamidasi=1;	}
}

/*---------------------------------------------------------
	�{�X�̋��ʁA�P��ڏ��������[�`��(����A�U���s��)
---------------------------------------------------------*/

#if (0==USE_BOSS_COMMON_MALLOC)
/*extern*/ void spell_card_boss_init_regist(SPRITE *src)
#else
/*extern*/ void spell_card_boss_init_regist_void(void/*SPRITE *src*/)
#endif
{
#if (0==USE_BOSS_COMMON_MALLOC)
	pd_boss = src;/*�P��{�l*/
#else
	SPRITE *src;
	src = pd_boss;/*�P��{�l*/
#endif
//
	src->x256						= BOSS_XP256;/*t256(GAME_WIDTH/2)-(sakuya->w128)*/
	src->y256						= t256(-100);
//
	src->callback_hit_enemy 		= NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
//
	/* spell_card common init */
//	{
//	//	spell_card_limit_health 	= 0;
//	//	spell_card_mode 			= 0;/*off*/
//	}
		#if 1
	//------------ �e���֘A
		src->boss_base_danmaku_type 	= 0;
		src->boss_base_danmaku_time_out = 0;
		src->boss_base_danmaku_test 	= 0;	/*(DANMAKU_08_rumia-1)*/ /*0*/
		#endif

// ��� 	sakuya->base_hp 					= ((8*1024)-1)*(9); 	/* 8192==1024*8 */
// �p�`�F	sakuya->base_hp 					= ((8*1024)-1)*(3); 	// �p�`�F�{�̂�HP�B�������������Ă����������B// [***090305	�ύX
// �P�� 	sakuya->base_hp 					= ((8*1024)-1)*5;/*5 4test*/
// ���� 	sakuya->base_hp 					= 8*(((difficulty<<6/*[x64]*/)+ 200+1024) );/*test*/
// ��		sakuya->base_hp 					= ((8*1024)-1)*6;/*6 4test*/
// �A���X	sakuya->base_hp 					= (((8*1024)-1)*2); 	// �A���X�{�̂�HP�B�������������Ă����������B// [***090305	�ύX
//
	src->base_hp			= (my_111resource[(spell_card_number)].spell_life); 		/* �S�̗̂̑� */
	spell_card_limit_health = (src->base_hp) - (my_111resource[(spell_card_number+1)].spell_life);		/* �ʏ�U��(����U��)�̍U���� */
//
	spell_card_boss_timer	= (((my_111resource[(spell_card_number)].spell_time))); 	/* 75*64==75[count] 	��99[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
	spell_card_mode 		= (1);/*on*/	/* ����H */
	/* boss_rect_init */
	boss_clip_min.x256	= t256( 		0)+t256(24);
	boss_clip_max.x256	= t256(GAME_WIDTH)-t256(24);
	boss_clip_min.y256	= t256(0);
	boss_clip_max.y256	= t256(96);
	boss_effect_init();

}

/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]
---------------------------------------------------------*/

/*static*/ void init_00_keitai(SPRITE *src)
{
	/* ���݃_�~�[ */
}

/*static*/ void common_00_keitai(SPRITE *src)
{
	if (t256(15.0) > src->y256) 	/* ���ֈړ� */		/*t256(40.0)*/	/*t256(30.0)*/
	{
		src->y256 += t256(1.0); 	/*fps_factor*/	/*t256(2.0)*/
	}
	else
	if (t256(16.0) > src->y256) 	/* ���ֈړ� */		/*t256(40.0)*/	/*t256(30.0)*/
	{
		src->y256 += t256(1.0); 	/*fps_factor*/	/*t256(2.0)*/
		pd_state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
	}
	else	/* ��b�I����҂� */
	{
		if ( ((pd_state_flag) & STATE_FLAG_05_IS_BOSS) )	/* ��b�I��? */
		{
		//	common_boss_init_2nd(src);	/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
			/*---------------------------------------------------------
				�{�X�̋��ʁA�Q��ڏ��������[�`��(�U���\)
			---------------------------------------------------------*/
		//	/*extern*/static void common_boss_init_2nd(SPRITE *src)
			{
				/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
				src->callback_hit_enemy = callback_hit_boss;	/* �R�[���o�b�N�o�^ */
				/* spell_card common init */
				spell_card_mode 		= (1);/*on*/
			}
		//	/*���Ԑ����J�E���g�L����*/
		//	data->boss_base.state001++/* = ST_02*/;
			spell_card_number++;
		}
	}
}


/*---------------------------------------------------------
	���ʃ{�X�ޔ�(���j��ɉ�ʊO�Ƀ{�X��������)
	[�X�y�J�V�X�e�����Ɉړ��\��]
---------------------------------------------------------*/

/*static*/ void common_99_keitai(SPRITE *src)
{
	if (0 > (src->y256+t256(50.0)))/* +t256(50.0) �{�X�O���̍ő�T�C�Y(50[dot]) */
	{
		src->callback_mover 		= NULL; 		/* �����܂� */
		src->type					= SP_DELETE;	/* �����܂� */
	//	pd_save_timer		= (6);/* 6[�t���[��] �{�X�|���Ă��玟(�V�i���I)�ɐi�ނ܂ł̑҂����� */
	}
	else
	{
		src->y256 -= t256(0.75);					/* ��ɑޔ� */			/* t256(0.75) �ޔ𑬓x 0.75[dot/flame] */	/*fps_factor*/
		if ( (int)(GAME_WIDTH*256/2) > src->x256)	/* ��ʔ����̈ʒu */
		{
			src->x256 += t256(1.0); 	/* �E��ɑޔ� */
		}
		else
		{
			src->x256 -= t256(1.0); 	/* ����ɑޔ� */
		}
	}
}
