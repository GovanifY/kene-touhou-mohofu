
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�X�y�J�Ǘ��V�X�e��
	-------------------------------------------------------
	�� �X�y�J�̓V�X�e���ŃT�|�[�g����\�z������܂��B(�\�z��)
	�V�X�e���ŃT�|�[�g�����ƁA�ǂ̃{�X���A�ǂ̃X�y�J�ł����Ă�悤�ɂȂ�܂��B
	-------------------------------------------------------
	ToDo:
	�� �U���̊Ǘ�
	�� ����̊Ǘ�(���\����)
	�� �G��(���G��)���X�y�J�����Ă�@�\�́A�p�~���������ǂ������m��Ȃ��B
	����(���G�������G���X�y�J�@�\)��p�~���āA����ɓ����Ƀ{�X���o��������(�܂蒆�{�X)
	�̕����A�V�X�e�����������肵�ėǂ��C������B��������ׂɂ͐F�X������������B
	����� dat �̋K�i���ƁA�����͏o���Ȃ��̂ŁA��� dat �̋K�i��ς��āA
	dat ����{�X�̃X�N���v�g���N������悤�ɂ���K�v������B
	�v���C���[�ɋ����ăX�N���v�g���Ⴄ�̂ŁA���̕ӂ̋K�i����͂܂�����B(����V�i���I�Ƃ�)
	-------------------------------------------------------
	�� �G��(���G��)���X�y�J�����Ă�@�\�́A�p�~���āA
	�G��(���G��)���X�y�J�����������ꍇ�́A�u�X�y�J�����Ă�U�R�v�Ƃ�������V�݂��Ă�����g��
	�V�X�e���ł̓T�|�[�g���Ȃ������ǂ��Ǝv���B�̂ł������������ŉ����\��B
	-------------------------------------------------------
	�܂�u�X�y�J�����Ă�U�R�v�Ɓu�����Ƀ{�X���o��������(�܂蒆�{�X)�v��2��ނ�V�ݗ\��B
	�������C���R�A���ꊷ�����ɂȂ鎖���m�肵�Ă���̂ŁA
	�� �����R�A(�G�����{�X(�܂蒆�{�X)���\)
	�� �{�X�R�A(�{�X�̂݉\�A�G���s��)
	�� ��b�R�A(�{�X��b���[�h�̂݉\�A�G���s��)
---------------------------------------------------------*/

#include "kanji_system.h"

/*---------------------------------------------------------
	�{�X�ǉ�
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
extern void boss_init_sakuya(SPRITE *src);

// �p�`�F 5��
extern void add_zako_pache_dolls(SPRITE *src);
//tern void pache_05_keitai(SPRITE *src);
extern void pache_04_keitai(SPRITE *src);
//tern void pache_03_keitai(SPRITE *src);
//tern void pache_02_keitai(SPRITE *src);
extern void pache_01_keitai(SPRITE *src);

// �� 4��
extern void aya_05_keitai(SPRITE *src);
extern void aya_04_keitai(SPRITE *src);
extern void aya_03_keitai(SPRITE *src);
extern void aya_02_keitai(SPRITE *src);
extern void aya_01_keitai(SPRITE *src);
extern void boss_init_aya(SPRITE *src);

// �P�� 3��
extern void add_zako_kaguya_houmotsu(SPRITE *src);
extern void add_zako_kaguya_dolls(SPRITE *src);
//tern void kaguya_06_keitai(SPRITE *src);
//tern void kaguya_05_keitai(SPRITE *src);
extern void kaguya_04_keitai(SPRITE *src);
//tern void kaguya_03_keitai(SPRITE *src);
//tern void kaguya_02_keitai(SPRITE *src);
extern void kaguya_01_keitai(SPRITE *src);
extern void boss_init_kaguya(SPRITE *src);

// ���� 2��
extern void add_zako_mima_dolls(SPRITE *src);
extern void mima_01_keitai(SPRITE *src);

// �A���X 1��
extern void alice_03_keitai(SPRITE *src);/* �����u�M���̕������l�`�v^^); */
extern void alice_02_keitai(SPRITE *src);/* ���o�u�p�[�t�F�N�g�t���[�Y�v */
extern void alice_01_keitai(SPRITE *src);/* �ʏ�U�� */

// ���ʌ`��
extern void init_00_keitai(SPRITE *src);	/* �_�~�[ */
extern void common_00_keitai(SPRITE *src);
//extern void common_99_keitai(SPRITE *src);/* ���j��ɉ�ʊO�Ƀ{�X�������� */


//------------ �X�y�J�֘A
global int spell_card_limit_health; 	/* �K��l�ȉ��ɂȂ�΃X�y�J���[�h���� */
global int spell_card_boss_state;		/* ���l�ɂȂ�΃{�X�X�y�J���[�h�ɓ���Ȃ� */
global int spell_card_mode; 			/* �X�y�J���[�h */
global int spell_card_boss_timer;		/* ���p */	// ��������

global int spell_card_number;			/* ���p */	// �X�y�J�ԍ�
global int spell_card_max;				/* ���p */	// �X�y�J�ԍ��ő���E�l

typedef struct
{
	int spell_limit_time;						/* �X�y�J�̐������ԁB(�X�y�J�ɓo�^���ꂽ��莞��) */
	int spell_repeat_time;						/* �X�y�J�̌J��Ԃ����� */
	void (*spell_init_callback)(SPRITE *sss);	/* �������ړ����� */
	void (*spell_move_callback)(SPRITE *sss);	/* �X�y�J�ړ����� */
	int spell_life; 							/* �X�y�J�ɓo�^���ꂽ���̗� */
	const char *spell_str_name; 				/* �X�y�J���� */
} SPELL_CARD_RESOURCE;

	/* ���O�̓e�L�g�[�ł� */

/* �X�y������(==�P��[�t���[��])�́A64�{(==�P��[��1�b])���K�i */
#define s_time(aaa) ((aaa)<<6)

#if 0
	8192==1024*8
//
73728==8192*9.0
65536==8192*8.0
57344==8192*7.0
49152==8192*6.0
40960==8192*5.0
36864==8192*4.5
32768==8192*4.0
28672==8192*3.5
24576==8192*3.0
20480==8192*2.5
16384==8192*2.0
12288==8192*1.5
#endif
static SPELL_CARD_RESOURCE my_spell_card_resource[SPELL_CARD_MAX] =
{																							  //	�ő唼�p��31�����B
// ��� 6�� 																				  "feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{	 s_time(300),  s_time(300),  init_00_keitai/*boss_init_sakuya*/,			 common_00_keitai,		73728,NULL/*"�`�ԕύX" "\n"*/					},/* 73728==8192*9.0 */
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 sakuya_01_keitai,		 4096,NULL/*"�ʏ�U��" "\n"*/					},		/*	"���`��: �ʏ�U��(���E�ɓ������S�e����)"		*/	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 sakuya_02_keitai,		 4096," �@�@��p�u�~�X�f�B���N�V�����v" "\n"	},		/*	"���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)"	*/
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 sakuya_03_keitai,		 4096," �@�@���J�u�o�[�e�B�J���i�C�t�v" "\n"	},		/*	"��O�`��: (�����i�C�t�������Ă����)"			*/
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 sakuya_04_keitai,		 4096," �@�����u�A�����~�e�b�h���C�h�v" "\n"	},		/*	"��l�`��: �����w����"							*/
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 sakuya_06_keitai,		 8192," �@����u�f���W�����X���[���h�v" "\n"	},		/*	"��܌`��: (���F�}�X�J�b�g�e)"					*/
//	{	 s_time(20),   s_time(20),	 init_00_keitai,			 common_00_keitai,		 5000,"�@�@�@�@�@��p�u���e�X�g007�v" "\n"	},		/*	"�掵�`��: (���U�����w)�ǉ��v�撆"				*/
//	{	 s_time(20),   s_time(20),	 init_00_keitai,			 common_00_keitai,		 5000,"�@�@�@�@�@��p�u���e�X�g008�v" "\n"	},		/*	"�攪�`��: (���~�߃i�C�t)�ǉ��v�撆"			*/
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 sakuya_09_keitai,		25000," �@�����u�t�F�X�e�B�o���i�C�t�v" "\n"	},		/*	"���`��: �ŏI�`��(����1)" 					*/
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 sakuya_10_keitai,		20000," �@�@�@�@�����u�`�F�b�N���C�h�v" "\n"	},		/*	"��10�`��: �ŏI�`��(����2)" 					*/
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 sakuya_11_keitai,		90000," �@�@���C�h��Z�u�t�@�C�i�����v" "\n"	},		/*	"��11�`��: �ŏI�`��(����3)" 					*/

// �p�`�F 5��																				  "feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{	 s_time(300),  s_time(300),  init_00_keitai,			 common_00_keitai,		40960,NULL/*"�`�ԕύX" "\n"*/					},/* 40960==8192*5.0 */
	{	 s_time(30),   s_time(30),	 init_00_keitai,			 pache_01_keitai,		 8192,NULL/*"�ʏ�U��" "\n"*/					},
	{	 s_time(30),   s_time(30),	 init_00_keitai,			 pache_01_keitai,		 8192,"�@�@�Ε��u�p�`�����[���쐬000�v" "\n"	},
	{	 s_time(30),   s_time(30),	 init_00_keitai,			 pache_01_keitai,		 8192,"�@�@�����u�p�`�����[���쐬001�v" "\n"	},
	{	 s_time(30),   s_time(30),	 add_zako_pache_dolls,		 pache_04_keitai,		 8192,"�@�@�ؕ��u�V���t�B�[���쐬002�v" "\n"	},
	{	 s_time(30),   s_time(30),	 add_zako_pache_dolls,		 pache_04_keitai,		 8192,"�@�@�����u�p�`�����[���쐬003�v" "\n"	},
	{	 s_time(30),   s_time(30),	 init_00_keitai,			 pache_04_keitai,		 8192,"�@�@�y���u�p�`�����[���쐬004�v" "\n"	},
	{	 s_time(30),   s_time(30),	 init_00_keitai,			 pache_04_keitai,		 8192,"�@�@�����u�p�`�����[���쐬005�v" "\n"	},
//	{	 s_time(99),   s_time(99),	 init_00_keitai,			 pache_04_keitai,		 8192,"�@�@�����u�T�C�����g���쐬006�v" "\n"	},
//	{	 s_time(99),   s_time(99),	 init_00_keitai,			 pache_04_keitai,		 8192,"�@�@�X���u�p�`�����[���쐬007�v" "\n"	},

// �� 4��																					  "feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{	 s_time(250),  s_time(250),  init_00_keitai/*boss_init_aya*/,			 common_00_keitai,		32768,NULL/*"�`�ԕύX" "\n"*/					},/* 32768=8192*4.0 */
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 aya_01_keitai, 		 4096,NULL/*"�ʏ�U��" "\n"*/					},/*4*/
	{	 s_time(30),   s_time(30),	 init_00_keitai,			 aya_02_keitai, 		 2048,"�@�@ �ؕ��u���남�ڂ��F�؈�ࣁv" "\n"	},/*4*/
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 aya_03_keitai, 		 2048,"�@�@�@�@�@�@ �H���u����̒��ׁv" "\n"	},/*5"�򕄁u���쐬�X�y�J001�v"*/
	{	 s_time(40),   s_time(40),	 init_00_keitai,			 aya_04_keitai, 		 4096,"�@�@�@ �����u�n���卪���]��Q�v" "\n"	},/*4"�򕄁u���쐬�X�y�J002�v"*/
	{	 s_time(100),  s_time(100),  init_00_keitai,			 aya_05_keitai, 		 8192,"�@�@�@�@�@�@ �����u�����R�R�l�v" "\n"	},/*5"�򕄁u���쐬�X�y�J003�v"*/
	{	 s_time(99),   s_time(99),	 init_00_keitai,			 aya_05_keitai, 		16384," �@�@�@ �򕄁u���쐬�X�y�J004�v" "\n"	},/*4"�򕄁u���쐬�X�y�J004�v"*/
	{	 s_time(99),   s_time(99),	 init_00_keitai,			 aya_05_keitai, 		16384," �@�@�@ �򕄁u���쐬�X�y�J005�v" "\n"	},/*5*/
	{	 s_time(99),   s_time(99),	 init_00_keitai,			 aya_05_keitai, 		16384," �@�@�@ �򕄁u���쐬�X�y�J006�v" "\n"	},/*5*/
	{	 s_time(99),   s_time(99),	 init_00_keitai,			 aya_05_keitai, 		16384," �@�@�@ �򕄁u���쐬�X�y�J007�v" "\n"	},/*5*/

// �P�� 3�� 																				  "feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{	 s_time(250),  s_time(250),  boss_init_kaguya,			 common_00_keitai,		28672,NULL/*"�`�ԕύX" "\n"*/					},/* 28672==8192*3.5 */
	{	 s_time(23),   s_time(23),	 add_zako_kaguya_houmotsu,	 kaguya_01_keitai,		 4096,NULL/*"�ʏ�U��" "\n"*/					}, // ����(5)�҂�e�A�ԁA�B���@�_���A�e(8)��18�Z�b�g(�b)���炢(�n�߂�5�b�͏o���Ȃ�)�B(�P����Ԋu�ňړ��A�I���㒆���ֈړ�)
	{	 s_time(40),   s_time(40),	 add_zako_kaguya_houmotsu,	 kaguya_01_keitai,		 4096,"�@�@�@�@�@�@�@���u���쐬001�v" "\n"	},// 1.������̋�	(�P�钆���Œ�A�Ԏ��Ή��̊ےe�A�Z���[�U�[)4�Z�b�gx
	{	 s_time(40),   s_time(40),	 add_zako_kaguya_houmotsu,	 kaguya_01_keitai,		 4096,"�@�@�@�@�@�@�@���u���쐬002�v" "\n"	},// 2.���̌�΂̔�
	{	 s_time(40),   s_time(40),	 add_zako_kaguya_dolls, 	 kaguya_04_keitai,		 4096,"�@�@�@�@�@�@�@���u���쐬003�v" "\n"	},// 3.�Αl�̔��
	{	 s_time(40),   s_time(40),	 add_zako_kaguya_dolls, 	 kaguya_04_keitai,		 4096,"�@�@�@�@�@�@�@���u���쐬004�v" "\n"	},// 4.���̎q���L
	{	 s_time(40),   s_time(40),	 add_zako_kaguya_dolls, 	 kaguya_04_keitai,		 4096,"�@�@�@�@�@�@�@���u���쐬005�v" "\n"	},// 5.�H���̋ʂ̎}
	{	 s_time(40),   s_time(40),	 add_zako_kaguya_dolls, 	 kaguya_04_keitai,		 4096,"�@�@�@�@�@�@�@���u���쐬006�v" "\n"	},//

// ���� 2�� 																				  "feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{	 s_time(200),  s_time(200),  init_00_keitai,			 common_00_keitai,		20480,NULL/*"�`�ԕύX" "\n"*/					},/* 20480==8192*2.5 */
	{	 s_time(20),   s_time(20),	 add_zako_mima_dolls,		 mima_01_keitai,		 8192,NULL/*"�ʏ�U��" "\n"*/					},
	{	 s_time(20),   s_time(20),	 add_zako_mima_dolls,		 mima_01_keitai,		 8192,"�@�@�@�@����u���쐬�X�y�J001�v" "\n"	},
	{	 s_time(40),   s_time(40),	 add_zako_mima_dolls,		 mima_01_keitai,		 8192,"�@�@�@�@����u���쐬�X�y�J002�v" "\n"	},
	{	 s_time(40),   s_time(40),	 add_zako_mima_dolls,		 mima_01_keitai,		 8192,"�@�@�@�@����u���쐬�X�y�J003�v" "\n"	},
	{	 s_time(40),   s_time(40),	 add_zako_mima_dolls,		 mima_01_keitai,		 8192,"�@�@�@�@����u���쐬�X�y�J004�v" "\n"	},
	{	 s_time(40),   s_time(40),	 add_zako_mima_dolls,		 mima_01_keitai,		 8192,"�@�@�@�@����u���쐬�X�y�J005�v" "\n"	},
	{	 s_time(40),   s_time(40),	 add_zako_mima_dolls,		 mima_01_keitai,		 8192,"�@�@�@�@����u���쐬�X�y�J006�v" "\n"	},

// �A���X 1��																				  "feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{	 s_time(120),  s_time(120),  init_00_keitai,			 common_00_keitai,		16384,NULL/*"�`�ԕύX" "\n"*/					},/* 16384==8192*2 */	/* "�ʏ�U��"�̎���(?) */ /* "�ʏ�U��"�̃��C�t */
	{	 s_time(10),   s_time(10),	 init_00_keitai,			 alice_01_keitai,		 4096,NULL/*"�ʏ�U��" "\n"*/					},
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 alice_02_keitai,		 4096," �@�����u�p�[�t�F�N�g�t���[�Y�v" "\n"	},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 alice_03_keitai,		 8100," �@�����u�M���̃}�g�����[�V�J�v" "\n"	},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 s_time(40),   s_time(40),	 init_00_keitai,			 alice_03_keitai,		 5000,"�@�����u���쐬�̈����ɐl�`001�v" "\n"	},/* �����u�����̉~�J�l�`�v */
//	{	 s_time(20),   s_time(20),	 init_00_keitai,			 alice_03_keitai,		20000,"�����u���쐬�̈ɑ������l�`002�v" "\n"	},/* �����u�󑐂̐l�`�Ă��v */
//	{	 s_time(20),   s_time(20),	 init_00_keitai,			 alice_02_keitai,		 5000,"�@�����u���쐬�̉p�g���l�`003�v" "\n"	},
//	{	 s_time(20),   s_time(20),	 init_00_keitai,			 alice_02_keitai,		 5000,"�@�����u���쐬�̘I�����l�`004�v" "\n"	},
//	{	 s_time(80),   s_time(80),	 init_00_keitai,			 alice_02_keitai,		 5000,"�@�@�����u���쐬�̐����l�`005�v" "\n"	},
//	{	 s_time(80),   s_time(80),	 init_00_keitai,			 alice_02_keitai,		 5000,"�@�����u���쐬�̔����`�l�`006�v" "\n"	},
//	{	 s_time(80),   s_time(80),	 init_00_keitai,			 alice_02_keitai,		 5000,"�@�@�����u���쐬�̔g���l�`007�v" "\n"	},
//																							  "feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
};
	/*
	�}�g�����[�V�J�́A�I�����l�`�ł����̂��ȁH�l�`�Ƃ�������薯�|�i�̎����B
	�ŋ߂͖G���}�g�����[�V�J�̎��v�����{���炠��炵���B
	//
	���{�̋C�ے��͕���17m�ȏ�̕����u�䕗�v�ƒ�`�B
	���ۋC�ۋ@��(WMO)�ł͕���33m�ȏ�̂��̂��utyphoon�v�ƒ�`�B����ȉ��́ustorm�v�B
	*/
		// ��� 	sakuya->base_hp 	= ((8*1024)-1)*(9); 	/* 8192==1024*8 */
		// �p�`�F	sakuya->base_hp 	= ((8*1024)-1)*(3); 	// �p�`�F�{�̂�HP�B�������������Ă����������B// [***090305	�ύX
		// �P�� 	sakuya->base_hp 	= ((8*1024)-1)*5;/*5 4test*/
		// ���� 	sakuya->base_hp 	= 8*(((difficulty<<6/*[x64]*/)+ 200+1024) );/*test*/
		// ��		sakuya->base_hp 	= ((8*1024)-1)*6;/*6 4test*/
		// �A���X	sakuya->base_hp 	= (((8*1024)-1)*2); 	// �A���X�{�̂�HP�B�������������Ă����������B// [***090305	�ύX
		//



/*---------------------------------------------------------
	�X�y�J���̕\��
---------------------------------------------------------*/
static void draw_spell_card_name(void)
{
	kanji_window_clear();	/* �����E�B���h�E�̓��e�������B */
	home_cursor();			/* �J�[�\�����z�[���|�W�V�����ֈړ� */
	msg_time = (60*5);
	print_kanji000(/*SDL_Surface *drawmap,*/ /*SDL_Rect *rect*/ /*0,*/
		my_spell_card_resource[(spell_card_number)].spell_str_name, //	aaa_str[(spell_card_number&15)],
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

//
//extern void draw_spell_card_name(void);/* spell_card.c */
global void common_boss_put_items(SPRITE *src)
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
	���C�t�����Ȃ��Ȃ�ƃ{�X���Ȃ��Ȃ����ȂȂ��̂́A���o�������ł��B
	�Ă��Ɓ[�ɍČ��H
---------------------------------------------------------*/
//0 ... 255 == draw_boss_hp_value 0001 1111 1110 0000;	>>=(2+3);  0000 0000 1111 1111; 0xff;
/*
	4096/60 == 68.2666666666666666666666666666667
	4096/256 == 16
*/
//#define LIMIT_MAX_HP_DEC_BOSS_BY_FLAME (16)
//#define LIMIT_MAX_HP_DEC_BOSS_BY_FLAME (48)
//	int LIMIT_MAX_HP_DEC_BOSS_BY_FLAME[8] = { (1), (2), (4), (8),  (16), (24), (32), (48), };

extern int boss_hp_dec_by_frame;/*�{�X�U�������l�A�t���[���P��*/
global void boss_hp_frame_check(void)
{
	{
		int limit_max_hp_dec_boss_by_flame;
		u8 test_draw_boss_hp_value;
		/* 8192/32 == 256 */
		test_draw_boss_hp_value = (obj_boss->base_hp>>5) & 0xff;
		#if 0
			 if (0x80 < test_draw_boss_hp_value)	{	limit_max_hp_dec_boss_by_flame = (48);	}	//128 ... 255
		else if (0x40 < test_draw_boss_hp_value)	{	limit_max_hp_dec_boss_by_flame = (32);	}	// 64 ... 127
		else if (0x20 < test_draw_boss_hp_value)	{	limit_max_hp_dec_boss_by_flame = (16);	}	// 32 ...  63
		else if (0x10 < test_draw_boss_hp_value)	{	limit_max_hp_dec_boss_by_flame = ( 4);	}	// 16 ...  31
		else										{	limit_max_hp_dec_boss_by_flame = ( 1);	}	//	0 ...  15
		#endif
		#if 0
			 if (200 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (40);	}	//201 ... 255
		else if (150 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (30);	}	//151 ... 200
		else if (100 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (20);	}	//101 ... 150
		else if ( 50 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (10);	}	// 51 ... 100
		else										{	limit_max_hp_dec_boss_by_flame = ( 5);	}	//	0 ...  50
		#endif
		#if 0
			 if (200 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (48);	}	//201 ... 255  ;  48 = 6  x8
		else if (150 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (40);	}	//151 ... 200  ;  40 = 5  x8
		else if (100 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (32);	}	//101 ... 150  ;  32 = 4  x8
		else if ( 50 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (24);	}	// 51 ... 100  ;  24 = 3  x8
		else										{	limit_max_hp_dec_boss_by_flame = (12);	}	//	0 ...  50  ;  12 = 1.5x8
		#endif
		#if 0
			 if (191 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (48);	}	//192 ... 255  ;  48 = 6  x8
		else if (127 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (40);	}	//128 ... 191  ;  40 = 5  x8
		else if ( 63 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (32);	}	// 64 ... 127  ;  32 = 4  x8
		else										{	limit_max_hp_dec_boss_by_flame = (24);	}	//	0 ...  63  ;  24 = 3  x8
		#endif
		#if 0
			 if (191 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (60);	}	//192 ... 255  ;  60 = 4  x16 -4
		else if (127 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (44);	}	//128 ... 191  ;  44 = 3  x16 -4
		else if ( 63 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (28);	}	// 64 ... 127  ;  28 = 2  x16 -4
		else										{	limit_max_hp_dec_boss_by_flame = (12);	}	//	0 ...  63  ;  12 = 1  x16 -4
		#endif
		#if 0
	//	limit_max_hp_dec_boss_by_flame = (((test_draw_boss_hp_value>>6)+3)<<3);
	//	limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>3)+(3<<3);
	//	limit_max_hp_dec_boss_by_flame = (((test_draw_boss_hp_value>>6)+1)<<4);
	//	limit_max_hp_dec_boss_by_flame = (((test_draw_boss_hp_value>>6))<<4)+(1<<4);
	//	limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>2)+(1<<4)-4;
		limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>2)+(16-4);
		#endif
		#if 1
	//	limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>2) | (0x0f);
		limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>2) | (0x08);
		#endif
		//
		if (limit_max_hp_dec_boss_by_flame < boss_hp_dec_by_frame)
		{
			boss_hp_dec_by_frame = limit_max_hp_dec_boss_by_flame;
		}
	}
//
	obj_boss->base_hp -= boss_hp_dec_by_frame;
	boss_hp_dec_by_frame = 0;/*�g�����̂ŏ���(�t���[���P��)*/
	if (spell_card_limit_health >= obj_boss->base_hp)		/* �K��l�ȉ��ɂȂ�΃X�y�J���[�h���� */
	{
		spell_card_mode 			= (SPELL_CARD_MODE_00_OFF);
		boss_destroy_check_type(obj_boss, DESTROY_CHECK_00_WIN_BOSS);
		callback_gu_draw_haikei = callback_gu_draw_haikei_modosu;
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
/*extern*/global int boss_hamidasi; 			/* �u�ړ��ł��Ȃ������t���O�v(�g�p�O�Ɏ蓮��OFF==0�ɂ��Ƃ�) */
/*extern*/global POINT256 boss_clip_min;		/* �{�X�ړ��͈�(�ŏ��l) */
/*extern*/global POINT256 boss_clip_max;		/* �{�X�ړ��͈�(�ő�l) */
global void boss_move_vx_vy(SPRITE *src)
{
	src->x256 += (src->vx256);
	src->y256 += (src->vy256);
}
global void boss_clip_rect(SPRITE *src)
{
		 if (src->x256 < boss_clip_min.x256 )	{	src->x256 = boss_clip_min.x256; 	boss_hamidasi=1;	}
	else if (src->x256 > boss_clip_max.x256 )	{	src->x256 = boss_clip_max.x256; 	boss_hamidasi=1;	}
//
		 if (src->y256 < boss_clip_min.y256 )	{	src->y256 = boss_clip_min.y256; 	boss_hamidasi=1;	}
	else if (src->y256 > boss_clip_max.y256 )	{	src->y256 = boss_clip_max.y256; 	boss_hamidasi=1;	}
}

// ����̃V�i���I�X�N���v�g�K�i���ƃX�y�J�V�X�e���Ƒ����������B
// �d���Ȃ��̂ŁA�����Ƀt���O�őΉ�������B
#define USE_OLD_SCRIPT_SYSTEM (1)


/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]	�X�y�J�o�^
---------------------------------------------------------*/
#if (1==USE_OLD_SCRIPT_SYSTEM)
	// ����̃V�i���I�X�N���v�g�K�i���ƃX�y�J�V�X�e���Ƒ����������B
	// �d���Ȃ��̂ŁA�����Ƀt���O�őΉ�������B
static int speka_first_move_flag;
#endif /* (1==USE_OLD_SCRIPT_SYSTEM) */

//global /*static*/ void re gist_spell_card222(SPRITE *src)
//global /*static*/ void ch eck_regist_spell_card(SPRITE *src)
global /*static*/ void check_regist_generate_spell_card(SPRITE *src)
{
	#if (1==USE_OLD_SCRIPT_SYSTEM)
	// ����̃V�i���I�X�N���v�g�K�i���ƃX�y�J�V�X�e���Ƒ����������B
	// �d���Ȃ��̂ŁA�����Ƀt���O�őΉ�������B
	int exec_speka;
	exec_speka = 0;
	#endif /* (1==USE_OLD_SCRIPT_SYSTEM) */
//
	if (0/*off*/==spell_card_mode)
	{
	//	if (1)
		{
			int aaa;	/* ���ݑ̗� ���� ���������X�y�J�� �������̗͒l */
			aaa = spell_card_limit_health - (my_spell_card_resource[(spell_card_number+1)].spell_life); 	/*1000 500*/
		//	spell_card_limit_health -= 1000/*500*/;
			/* �{�X���X�y�J�����Ă���̗͂�����ꍇ */
			if (0 < aaa)
			{
				spell_card_limit_health = aaa;
			//	if (b_health_alter_low1024 < (data->boss_base.boss_health & (1024-1)))/* �`�ԕύX������ */
				spell_card_mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/
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
				(my_spell_card_resource[(spell_card_number)].spell_init_callback)(src);
			}
			else
			{
				spell_card_limit_health = (0);
			//	spell_card_mode 		= (0);/*off*/
				spell_card_mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/	/* �Ƃ肠���� */
				/* [(�Ƃ肠����)�X�y�J�U���݂̂Ɏd�l�ύX]�����̂ŁA�Ō㌂�Ă�X�y�J���Ȃ��Ȃ����ꍇ�ɍU��������ׁB */
			}
		}
		/*---------------------------------------------------------
			�X�y�J�`�F�b�N
		---------------------------------------------------------*/
		if (0/*off*/==spell_card_boss_timer)
		{
			spell_card_limit_health = 0;
			spell_card_mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/
		}
	}
	else
	if (SPELL_CARD_MODE_01_IDO_JYUNNBI == spell_card_mode)
	{
		#if (1==USE_OLD_SCRIPT_SYSTEM)
		speka_first_move_flag = 0;
		#endif /* (1==USE_OLD_SCRIPT_SYSTEM) */
		src->boss_base_danmaku_time_out = (0);	/* �e�������������I�Ɏ~�߂�B */
		bullets_to_hosi();		/* ���Ă̓G�e���Ahosi�A�C�e���ɕς��� */
		/* �^���t�߂ɑޔ� */
	//	src->vvv256 				= t256(  0);/* �������ς݂̕K�v���� */
		src->vvv256 				= t256(1.0);/* �������ς݂̕K�v���� */
	//	src->target_x256			= t256(153);
		src->target_x256			= BOSS_XP256; //t256(0);
	//	src->target_y256			= src->y256;
		src->target_y256			= t256(16.0); //t256(0);
		spell_card_mode 			= (SPELL_CARD_MODE_02_TAIHI);/*on*/
	}
	else
	if (SPELL_CARD_MODE_02_TAIHI==spell_card_mode)	/* ���e�ʒu�܂ňړ����B */
	{
	//	alice_yuudou_move_only(src);
		alice_yuudou_calc(src);
	//	src->vvv256 -= (1); 	/* [��	4 �b]== 4.2666==(256/60[flame]) */
		src->vvv256 -= (1*4);	/* [��	1 �b]== 1.0666==(256/(4*60)[flame]) */
		if (0 > src->vvv256 )	/* �قډ�ʒ��S�t�߂܂ŁA�ړ������B */
		{
			src->vvv256 = (0);
			spell_card_mode 		= (SPELL_CARD_MODE_03_HATUDAN);/*on*/
		}
	}
	/*---------------------------------------------------------
		[�X�y�J�V�X�e�����Ɉړ��\��]	�X�y�J����
	---------------------------------------------------------*/
	else/* [(�Ƃ肠����)�X�y�J�U���݂̂Ɏd�l�ύX]*/
	/*
		����V�i���I����{�X���R���g���[���o���Ȃ��̂ł��̎d�l�͎b��I�B
		(�������̈ړ��Ȃǂ��X�y�J����)
		(�X�y�J�����Ȃ����������)
	*/
	/* �{�X�s�� */
	if (SPELL_CARD_MODE_03_HATUDAN==spell_card_mode)
	#if (1==USE_OLD_SCRIPT_SYSTEM)
	{
		exec_speka = 1;
		//(my_spell_card_resource[(spell_card_number)].spell_move_callback)(src);
	}
	if (exec_speka + speka_first_move_flag)
	#endif /* (1==USE_OLD_SCRIPT_SYSTEM) */
	{
		(my_spell_card_resource[(spell_card_number)].spell_move_callback)(src);
	}
}

	//	if (t256(/*(GAME_WIDTH/2)-32*/144) > src->x256) 	/* ��ʔ����̈ʒu���኱���B */
	//	{
	//		src->x256 += t256(1.0); 	/* �E �ɑޔ� */
	//	}
	//	else
	//	if (t256(/*(GAME_WIDTH/2)+32*/208) < src->x256) 	/* ��ʔ����̈ʒu���኱�E�B */
	//	{
	//		src->x256 -= t256(1.0); 	/* �� �ɑޔ� */
	//	}

/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]
---------------------------------------------------------*/

global /*static*/ void init_00_keitai(SPRITE *src)
{
	/* ���݃_�~�[ */
}
/*global*/static /*static*/ void common_03_keitai(SPRITE *src)
{
//		if ( ((pd_state_flag) & STATE_FLAG_05_IS_BOSS) )	/* ��b�I��? */
		{
			src->vvv256 = t256(1.0);
		//	common_boss_init_2nd(src);	/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
			/*---------------------------------------------------------
				�{�X�̋��ʁA�Q��ڏ��������[�`��(�U���\)
			---------------------------------------------------------*/
		//	/*extern*/static void common_boss_init_2nd(SPRITE *src)
			{
				/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
				src->callback_hit_enemy = callback_hit_boss;	/* �R�[���o�b�N�o�^ */
				/* spell_card common init */
				spell_card_mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/
			//	spell_card_mode 		= (0);/*off*/
			}
		//	/*���Ԑ����J�E���g�L����*/
		//	data->boss_base.state001++/* = ST_02*/;
			spell_card_number++;
		}
}
/*global*/static /*static*/ void common_02_keitai(SPRITE *src)
{
	//	if ( ((pd_state_flag) & STATE_FLAG_05_IS_BOSS) )	/* ��b�I��? */
	//	{
	//		my_spell_card_resource[(spell_card_number)].spell_move_callback = common_03_keitai;
	//	}
}
global /*static*/ void common_00_keitai(SPRITE *src)
{
//	if (0 == src->vvv256)
	if (0 > src->boss_base_state001)
	{
		src->boss_base_state001 = (0);
	//	src->vvv256 = t256(1.0);
	}
	else	/* ��b�I����҂� */
	{
		my_spell_card_resource[(spell_card_number)].spell_move_callback = common_02_keitai;
	}
}
global void script_boss_start(void)
{
		my_spell_card_resource[(spell_card_number)].spell_move_callback = common_03_keitai;
}


/*---------------------------------------------------------
	���ʃ{�X�ޔ�(���j��ɉ�ʊO�Ƀ{�X��������)
	[�X�y�J�V�X�e�����Ɉړ��\��]
---------------------------------------------------------*/

global /*static*/ void common_99_keitai(SPRITE *src)
{
	if (0 > (src->y256+t256(50.0)))/* +t256(50.0) �{�X�O���̍ő�T�C�Y(50[dot]) */
	{
		bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
		src->callback_mover 		= NULL; 		/* �����܂� */
	//	#if (0==US E_BOSS_COMMON_MALLOC)
	//	src->type					= SP_DELETE;	/* �����܂� */
	//	#else
		sprite_initialize_position(src);
	//	#endif

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

/*---------------------------------------------------------
	�e�����I���܂ő҂B
	-------------------------------------------------------

---------------------------------------------------------*/

/*static*/global void danmaku_state_check_holding(SPRITE *src)
{
	if (DANMAKU_00 == src->boss_base_danmaku_type)
	{
		src->boss_base_state001++;
	}
}




/*---------------------------------------------------------
	�{�X�̋��ʁA�P��ڏ��������[�`��(����A�U���s��)
---------------------------------------------------------*/



/*---------------------------------------------------------
	�e���͈̔͂�W���ɐݒ�
---------------------------------------------------------*/
global void set_default_bullet_clip(void)/* call from load_stage.c */
{
	//	bullet_clip_min.x256 = t256(-100);
	//	bullet_clip_max.x256 = t256(100)+t256(GAME_WIDTH);
	//	bullet_clip_min.y256 = t256(-256);/* ���� */
	//	bullet_clip_max.y256 = t256(100)+t256(GAME_HEIGHT);
		bullet_clip_min.x256 = t256(-32);
		bullet_clip_max.x256 = t256(0)+t256(GAME_WIDTH);
		bullet_clip_min.y256 = t256(-32);
		bullet_clip_max.y256 = t256(0)+t256(GAME_HEIGHT);
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
/* from game_core.c */
global void add_boss_common(STAGE_DATA *l)
{
		pd_state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
}
/*static*/extern  void boss_effect_reset(void);
extern void stage_boss_load_texture(void);
global void script_boss_load(int boss_number)
{
	{
		/* boss_rect_init */
		boss_clip_min.x256	= t256( 		0)+t256(24);
		boss_clip_max.x256	= t256(GAME_WIDTH)-t256(24);
		boss_clip_min.y256	= t256(0);
		boss_clip_max.y256	= t256(96);
	}
//
	int sss;
//	sss = l->user_255_code;
	sss = boss_number;
	sss &= (8-1);
//
	alice_anime_count	= 0;
	//----[BOSS]
	SPRITE *h;
//	#if (0==U SE_BOSS_COMMON_MALLOC)
//	h									= sprite_add_gu_error();
//	obj_boss							= h;/*�P��{�l*/
//	#else
	h									= obj_boss;/*�P��{�l*/
//	#endif
//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* �����o�^ */		/* �d�l�o�O(?) */
	{
		h->m_Hit256R					= ZAKO_ATARI16_PNG;
		h->flags						|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
		h->type 						= BOSS_00_BOSS11;
		h->callback_mover				= move_alice;
		h->callback_loser				= common_boss_put_items;
		h->callback_hit_enemy			= NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
	//
		#if 0/* �������ς݂̕K�v���� */
		h->vx256						= t256( 0);
		h->vy256						= t256( 0);
		#endif
		h->x256 						= BOSS_XP256;/*t256(GAME_WIDTH/2)-(sakuya->w128)*/
		h->y256 						= t256(-100);
	//
	//	h->boss_base_state001			= (0);	/*ST_00*/
		h->boss_base_state001			= (-1); /*ST_00*/
	//
		/* 3�ʂ̏ꍇ---- */ 	/* �S���ꏏ */
		/* 4�ʂ̏ꍇ---- */ 	/* �P��{�l */
		{
			static const unsigned int aaa_score_tbl[8/*8*/] =
			{
				score( 100000),/* �G�L�X�g���p(boss0) */
				score( 100000), 		/* �A���X */	/*	10�� x 1�l (�v100��==(1�lx10��)+(6�lx5��)+(2�lx30��)) */
				score( 500000), 		/* ���� */		/* 50�� (�v300��==6x50��) */
				score(2000000), 		/* �P�� */		/* 200�� (�v400��==(200��)+(5x40��)) */
				//
				score(2000000), 		/* �� */		/* 200�� */
				score( 500000), 		/* �p�`�F */	/*	50�� x 1�l ???(�v500��==(1�lx50��)+(6�lx5��)+(2�lx30��)) */
				score(5000000), 		/* ��� */		/* 500�� */
				score( 100000),/* �t�@���^�Y���p(boss7) */
			};
			h->base_score					= adjust_score_by_difficulty(aaa_score_tbl[sss]);
		}
	//------------ �X�y�J�֘A
		{
			static const u8 aaa[8+8] =
			{
				0,/* �G�L�X�g���p(boss0) */
				SPELL_CARD_00_alice_000,	/* �A���X */
				SPELL_CARD_00_mima_000, 	/* ���� */
				SPELL_CARD_00_kaguya_000,	/* �P�� */
			//
				SPELL_CARD_00_aya_000,		/* �� */
				SPELL_CARD_00_pache_000,	/* �p�`�F */
				SPELL_CARD_00_sakuya_000,	/* ��� */
				0,/* �t�@���^�Y���p(boss7) */
			//
				0,/* �G�L�X�g���p(boss0) */
				SPELL_CARD_14_alice_eee,	/* �A���X */ /*SPELL_CARD_19_alice_jjj;*/
				SPELL_CARD_17_mima_hhh, 	/* ���� */
				SPELL_CARD_17_kaguya_hhh,	/* �P�� */
			//
				SPELL_CARD_19_aya_jjj,		/* �� */
				SPELL_CARD_17_pache_hhh,	/* �p�`�F */
				SPELL_CARD_11_sakuya_kkk,	/* ��� */
				0,/* �t�@���^�Y���p(boss7) */
			//
			};
			spell_card_number				= aaa[sss];
			spell_card_max					= aaa[sss+8];
		}
		/* spell_card common init */
	//	{
	//	//	spell_card_limit_health 	= 0;
	//	//	spell_card_mode 			= 0;/*off*/
	//	}

		{
			h->base_hp				= (my_spell_card_resource[(spell_card_number)].spell_life); 		/* �S�̗̂̑� */
			spell_card_limit_health = (h->base_hp) - (my_spell_card_resource[(spell_card_number+1)].spell_life);		/* �ʏ�U��(����U��)�̍U���� */
			//
			spell_card_boss_timer	= (((my_spell_card_resource[(spell_card_number)].spell_limit_time)));	/* 75*64==75[count] 	��99[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
			spell_card_mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/	/* ����H */
			/*???*/
			my_spell_card_resource[(spell_card_number)].spell_move_callback = common_00_keitai;
		}
	//------------
//
		#if 1
	//------------ �e���֘A
		h->boss_base_danmaku_type		= DANMAKU_00;	/* �e�������I���t���O */
		h->boss_base_danmaku_test		= 0;	/*(DANMAKU_08_rumia-1)*/ /*0*/
		#endif
	//
	//------------ �e���͈͐ݒ� boss_init_aya(); boss_init_sakuya(); (���ꏉ��������O�̕K�v����)
		set_default_bullet_clip();	/* �e���͈̔͂�W���ɐݒ� */
		{	/* ���ꏉ����(�����͖{�������A���ʉ����Ĕp�~�\��) */
			void (*bbb[8])(SPRITE *h) =
			{
				NULL,				/* �G�L�X�g���p(boss0) */
				NULL,				/* �A���X */
				NULL,				/* ���� */
				NULL,				/* �P�� */
//
				boss_init_aya,		/* �� */
				NULL,				/* �p�`�F */
				boss_init_sakuya,	/* ��� */
				NULL,				/* �t�@���^�Y���p(boss7) */
			};
			if (NULL != bbb[ (int)(sss) ])
			{
				(*bbb[ (int)(sss) ])(h);	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
			}
		}
	}
	{
		boss_effect_reset();
	}
		// �{�X�e�N�X�`���ǂݍ���
		stage_boss_load_texture();
	#if (1==USE_OLD_SCRIPT_SYSTEM)
	// ����̃V�i���I�X�N���v�g�K�i���ƃX�y�J�V�X�e���Ƒ����������B
	// �d���Ȃ��̂ŁA�����Ƀt���O�őΉ�������B
	speka_first_move_flag = 1;
	#endif /* (1==USE_OLD_SCRIPT_SYSTEM) */

}

