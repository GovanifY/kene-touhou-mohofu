
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
extern void sakuya_07_keitai(SPRITE *src);
extern void sakuya_06_keitai(SPRITE *src);
extern void sakuya_04_keitai(SPRITE *src);
extern void sakuya_03_keitai(SPRITE *src);
extern void sakuya_02_keitai(SPRITE *src);
//tern void sakuya_01_keitai(SPRITE *src);/*����*/
extern void boss_init_sakuya(SPRITE *src);

// �p�`�F 5��
//extern void add_zako_pache_laser(SPRITE *src);
extern void add_zako_pache_dolls(SPRITE *src);
//tern void pache_05_keitai(SPRITE *src);
extern void pache_04_keitai(SPRITE *src);
//tern void pache_03_keitai(SPRITE *src);
//tern void pache_02_keitai(SPRITE *src);
extern void pache_01_keitai(SPRITE *src);
extern void boss_init_pache(SPRITE *src);/* �p�`�F���[�U�[�p���ꏉ���� */

//���[�U�[
extern void add_laser_off(SPRITE *src);
extern void add_laser_on(SPRITE *src);

// �� 4��
extern void aya_05_keitai(SPRITE *src);
//tern void aya_04_keitai(SPRITE *src);/*����*/
//tern void aya_03_keitai(SPRITE *src);/*����*/
//tern void aya_02_keitai(SPRITE *src);/*����*/
//tern void aya_01_keitai(SPRITE *src);/*����*/
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

extern void mima_01_keitai(SPRITE *src);

// �A���X 1��


// ���ʌ`��
extern void boss_move_03_x_douki(SPRITE *src);
extern void boss_move_02_xy_hidouki(SPRITE *src);
extern void boss_move_01_not_move(SPRITE *src);


extern void common_00_keitai(SPRITE *src);
//extern void common_99_keitai(SPRITE *src);/* ���j��ɉ�ʊO�Ƀ{�X�������� */


/* ���ʕ� */
global void init_00_boss_clip000(SPRITE *h);/* �W���^�C�v */
global void init_00_boss_clip111(SPRITE *h);/* ��ɍL���^�C�v */




//------------ �X�y�J�֘A
#include "spell_card_value.h"

global int spell_card_limit_health; 	/* �K��l�ȉ��ɂȂ�΃X�y�J���[�h���� */
global int spell_card_boss_state;		/* ���l�ɂȂ�΃{�X�X�y�J���[�h�ɓ���Ȃ� */
global int spell_card_mode; 			/* �X�y�J���[�h */
global int spell_card_boss_timer;		/* ���p */	// ��������

global int spell_card_number;			/* ���p */	// �X�y�J�ԍ�
/*global*/ static int spell_card_syoji_maisuu;				/* ���p */	// �X�y�J�ԍ��ő���E�l

typedef struct
{
	int spell_life; 							/* �X�y�J�ɓo�^���ꂽ���̗� */
	int spell_limit_time;						/* �X�y�J�̐������ԁB(�X�y�J�ɓo�^���ꂽ��莞��) */
//
	const char *spell_str_name; 				/* �X�y�J���� */
	int danmaku_type;							/* �X�y�J�ɓo�^���ꂽ�e�� */
//
	void (*spell_init_callback)(SPRITE *sss);	/* �������ړ����� */
	void (*spell_move_callback)(SPRITE *sss);	/* �X�y�J�ړ����� */
} SPELL_CARD_RESOURCE;

	/* ���O�̓e�L�g�[�ł� */

/* �X�y������(==�P��[�t���[��])�́A64�{(==�P��[��1�b])���K�i */
#define s_time(aaa) ((aaa)<<6)

#if 0
	8192==1024*8
//
77824==8192*9.5
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
{									//	�ő唼�p��31�����B

// �`���m �G�L�X�g���X�e�[�W		"feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{	 (28672),	s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{	 (28672),	s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{	 (28672),	s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{	 (28672),	s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
//	3.5
	{	  (4096),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				DANMAKU_10_cirno,				NULL,						boss_move_02_xy_hidouki,	},	/* "���`��: " */	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	  (4096),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				DANMAKU_10_cirno,				NULL,						boss_move_02_xy_hidouki,	},	/* "���`��: " */	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	  (4096),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				DANMAKU_10_cirno,				NULL,						boss_move_02_xy_hidouki,	},	/* "���`��: " */	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	  (4096),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				DANMAKU_10_cirno,				NULL,						boss_move_02_xy_hidouki,	},	/* "���`��: " */	/* ����̍U����(�蓮�ݒ�)  500 160	*/
//	3.0
	{	  (6144),	s_time(20), 	" �@�@�X���u�A�C�V�N���t�H�[���v" "\n", DANMAKU_01_sakuya,				NULL,						boss_move_01_not_move,		},	/* "���`��: " */
	{	  (6144),	s_time(20), 	" �@�@�X���u�A�C�V�N���t�H�[���v" "\n", DANMAKU_01_sakuya,				NULL,						boss_move_01_not_move,		},	/* "���`��: " */
	{	  (6144),	s_time(20), 	" �@�@�X���u�A�C�V�N���t�H�[���v" "\n", DANMAKU_01_sakuya,				NULL,						boss_move_01_not_move,		},	/* "���`��: " */
	{	  (6144),	s_time(20), 	" �@�@�X���u�A�C�V�N���t�H�[���v" "\n", DANMAKU_01_sakuya,				NULL,						boss_move_01_not_move,		},	/* "���`��: " */
//	2.25
	{	  (2048),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				DANMAKU_0c_sakuya_jack32,		NULL,						boss_move_02_xy_hidouki,	},	/* "��O�`��: " */
	{	  (2048),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				DANMAKU_0c_sakuya_jack32,		NULL,						boss_move_02_xy_hidouki,	},	/* "��O�`��: " */
	{	  (2048),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				DANMAKU_0c_sakuya_jack32,		NULL,						boss_move_02_xy_hidouki,	},	/* "��O�`��: " */
	{	  (2048),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				DANMAKU_0c_sakuya_jack32,		NULL,						boss_move_02_xy_hidouki,	},	/* "��O�`��: " */
//	2.0
	{	  (4096),	s_time(20), 	" �@�����u�p�[�t�F�N�g�t���[�Y�v" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},	/* "��l�`��: " */
	{	  (4096),	s_time(20), 	" �@�����u�p�[�t�F�N�g�t���[�Y�v" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},	/* "��l�`��: " */
	{	  (4096),	s_time(20), 	" �@�����u�p�[�t�F�N�g�t���[�Y�v" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},	/* "��l�`��: " */
	{	  (4096),	s_time(20), 	" �@�����u�p�[�t�F�N�g�t���[�Y�v" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},	/* "��l�`��: " */
//	1.5
	{	  (8192),	s_time(20), 	" �ᕄ�u�_�C�A�����h�u���U�[�h�v" "\n", DANMAKU_12_diamond_blizzard,	NULL,						boss_move_02_xy_hidouki,	},	/* "��܌`��: " */
	{	  (8192),	s_time(20), 	" �ᕄ�u�_�C�A�����h�u���U�[�h�v" "\n", DANMAKU_12_diamond_blizzard,	NULL,						boss_move_02_xy_hidouki,	},	/* "��܌`��: " */
	{	  (8192),	s_time(20), 	" �ᕄ�u�_�C�A�����h�u���U�[�h�v" "\n", DANMAKU_12_diamond_blizzard,	NULL,						boss_move_02_xy_hidouki,	},	/* "��܌`��: " */
	{	  (8192),	s_time(20), 	" �ᕄ�u�_�C�A�����h�u���U�[�h�v" "\n", DANMAKU_12_diamond_blizzard,	NULL,						boss_move_02_xy_hidouki,	},	/* "��܌`��: " */
//	0.0

// ��� 6�� 						"feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{	 (77824),	s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{	 (77824),	s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{	 (77824),	s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{	 (77824),	s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
//	9.5
	{	 (4096),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_01_sakuya,				boss_init_sakuya,			boss_move_03_x_douki,		},	/* "���`��: �ʏ�U��(���E�ɓ������S�e����)"		*/	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	 (4096),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_01_sakuya,				boss_init_sakuya,			boss_move_03_x_douki,		},	/* "���`��: �ʏ�U��(���E�ɓ������S�e����)"		*/	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	 (4096),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_01_sakuya,				boss_init_sakuya,			boss_move_03_x_douki,		},	/* "���`��: �ʏ�U��(���E�ɓ������S�e����)"		*/	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	 (4096),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_01_sakuya,				boss_init_sakuya,			boss_move_03_x_douki,		},	/* "���`��: �ʏ�U��(���E�ɓ������S�e����)"		*/	/* ����̍U����(�蓮�ݒ�)  500 160	*/
//	9.0
	{	 (6144),	s_time(20), 	" �@�@��p�u�~�X�f�B���N�V�����v" "\n", DANMAKU_0d_sakuya_miss_direction,				NULL,						sakuya_02_keitai,			},	/* "���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)"	*/
	{	 (6144),	s_time(20), 	" �@�@��p�u�~�X�f�B���N�V�����v" "\n", DANMAKU_0d_sakuya_miss_direction,				NULL,						sakuya_02_keitai,			},	/* "���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)"	*/
	{	 (6144),	s_time(20), 	" ��p�u���f�~�X�f�B���N�V�����v" "\n", DANMAKU_0d_sakuya_miss_direction,				NULL,						sakuya_02_keitai,			},	/* "���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)"	*/
	{	 (6144),	s_time(20), 	" ��p�u���f�~�X�f�B���N�V�����v" "\n", DANMAKU_0d_sakuya_miss_direction,				NULL,						sakuya_02_keitai,			},	/* "���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)"	*/
//	8.25	// bloody==�����炯�́B staind �X�e�C���h==�X�e���h==(�F��)�Ă��t��(�Ďc��)��B
	{	 (2048),	s_time(20), 	" �@�@�@�@���ہu�y�b�p�[�i�C�t�v" "\n", DANMAKU_00, 					NULL,						sakuya_03_keitai,			},	/* "��O�`��: ���J�u�y�b�p�[�i�C�t�v(�����i�C�t�������Ă����)" 		*/
	{	 (2048),	s_time(20), 	" �@�@�@�@���J�u�X�e���h�i�C�t�v" "\n", DANMAKU_00, 					NULL,						sakuya_03_keitai,			},	/* "��O�`��: ���J�u�y�b�p�[�i�C�t�v(�����i�C�t�������Ă����)" 		*/
	{	 (2048),	s_time(20), 	" �@�@�܌��J�u�u���b�f�B�i�C�t�v" "\n", DANMAKU_00, 					NULL,						sakuya_03_keitai,			},	/* "��O�`��: ���J�u�y�b�p�[�i�C�t�v(�����i�C�t�������Ă����)" 		*/
	{	 (2048),	s_time(20), 	" �@�@�����u�p�[�t�F�N�g�i�C�t�v" "\n", DANMAKU_00, 					NULL,						sakuya_03_keitai,			},	/* "��O�`��: ���J�u�y�b�p�[�i�C�t�v(�����i�C�t�������Ă����)" 		*/
//	8.0
	{	 (4096),	s_time(20), 	" �@�@�@����u���~�e�b�h���C�h�v" "\n", DANMAKU_00, 					NULL,						sakuya_04_keitai,			},	/* "��l�`��: �����w����"							*/
	{	 (4096),	s_time(20), 	" �@�����u�A�����~�e�b�h���C�h�v" "\n", DANMAKU_00, 					NULL,						sakuya_04_keitai,			},	/* "��l�`��: �����w����"							*/
	{	 (4096),	s_time(20), 	" �@�@�����u�X�v���b�^�[���C�h�v" "\n", DANMAKU_00, 					NULL,						sakuya_04_keitai,			},	/* "��l�`��: �����w����"							*/
	{	 (4096),	s_time(20), 	" �@�@�@�@�@�@�����u�E�����C�h�v" "\n", DANMAKU_00, 					NULL,						sakuya_04_keitai,			},	/* "��l�`��: �����w����"							*/
//	7.5 	// meek==�f���B
	{	 (8192),	s_time(20), 	" �@�@����u�f���W�����X�~�[�N�v" "\n", DANMAKU_00, 					NULL,						sakuya_06_keitai,			},	/* "��܌`��: (���F�}�X�J�b�g�e)"					*/
	{	 (8192),	s_time(20), 	" �@����u�f���W�����X���[���h�v" "\n", DANMAKU_00, 					NULL,						sakuya_06_keitai,			},	/* "��܌`��: (���F�}�X�J�b�g�e)"					*/
	{	 (8192),	s_time(20), 	" �@�@���ہu�f���W�����X�^�C���v" "\n", DANMAKU_00, 					NULL,						sakuya_06_keitai,			},	/* "��܌`��: (���F�}�X�J�b�g�e)"					*/
	{	 (8192),	s_time(20), 	" �@�����u�f���W�����X�X�g�[���v" "\n", DANMAKU_00, 					NULL,						sakuya_06_keitai,			},	/* "��܌`��: (���F�}�X�J�b�g�e)"					*/
//	6.5 	// �W���b�N�E�U�E���b�p�[==Jack the Ripper==�؂�􂫃W���b�N(�E�l�S)19���I(1888�N)�Ɏ���/���ۖ������B���[���h�q�[���[�Y�B ���b�N==�X�v���b�^�[�n�E�X
	{	(16384),	s_time(20), 	"�@��u�W���b�N�E�K�E���b�p�v" "\n", DANMAKU_0c_sakuya_jack32,		NULL,						boss_move_02_xy_hidouki,	},	/* "�掵�`��: (���U�����w)�ǉ��v�撆"				*/
	{	(16384),	s_time(20), 	"��p�u�W���b�N�E�U�E���b�p�[�v" "\n", DANMAKU_0c_sakuya_jack32,		NULL,						boss_move_02_xy_hidouki,	},	/* "�掵�`��: (���U�����w)�ǉ��v�撆"				*/
	{	(16384),	s_time(20), 	"��p�u�W���b�N�E�U�E�r�[���Y�v" "\n", DANMAKU_0c_sakuya_jack32,		NULL,						boss_move_02_xy_hidouki,	},	/* "�掵�`��: (���U�����w)�ǉ��v�撆"				*/
	{	(16384),	s_time(20), 	"���u�W���b�N�E�f�E�}���}�L�v" "\n", DANMAKU_0c_sakuya_jack32,		NULL,						boss_move_02_xy_hidouki,	},	/* "�掵�`��: (���U�����w)�ǉ��v�撆"				*/
//
//	{	 (8192),	s_time(20), 	"�@�@�@�@�@��p�u���e�X�gE08�v" "\n", DANMAKU_00, 					NULL,						common_00_keitai,			},	/* "�攪�`��: (���~�߃i�C�t)�ǉ��v�撆" 		*/
//	{	 (8192),	s_time(20), 	"�@�@�@�@�@��p�u���e�X�gN08�v" "\n", DANMAKU_00, 					NULL,						common_00_keitai,			},	/* "�攪�`��: (���~�߃i�C�t)�ǉ��v�撆" 		*/
//	{	 (8192),	s_time(20), 	"�@�@�@�@�@��p�u���e�X�gH08�v" "\n", DANMAKU_00, 					NULL,						common_00_keitai,			},	/* "�攪�`��: (���~�߃i�C�t)�ǉ��v�撆" 		*/
//	{	 (8192),	s_time(20), 	"�@�@�@�@�@��p�u���e�X�gL08�v" "\n", DANMAKU_00, 					NULL,						common_00_keitai,			},	/* "�攪�`��: (���~�߃i�C�t)�ǉ��v�撆" 		*/
//	4.5
	{	(16384),	s_time(20), 	" �@���z�u�t�F�X�e�B�o���i�C�t�v" "\n", DANMAKU_00, 					NULL,						sakuya_09_keitai,			},	/* "���`��: �ŏI�`��(����1)"					*/
	{	(16384),	s_time(20), 	" �@�����u�t�F�X�e�B�o���i�C�t�v" "\n", DANMAKU_00, 					NULL,						sakuya_09_keitai,			},	/* "���`��: �ŏI�`��(����1)"					*/
	{	(16384),	s_time(20), 	" �@���I�u�t�F�X�e�B�o���i�C�t�v" "\n", DANMAKU_00, 					NULL,						sakuya_09_keitai,			},	/* "���`��: �ŏI�`��(����1)"					*/
	{	(16384),	s_time(20), 	" �@���f�u�t�F�X�e�B�o���i�C�t�v" "\n", DANMAKU_00, 					NULL,						sakuya_09_keitai,			},	/* "���`��: �ŏI�`��(����1)"					*/
//	2.5
	{	(16384),	s_time(20), 	NULL,									DANMAKU_0e_remilia_00,			NULL,						sakuya_10_keitai,			},	/* "��10�`��: �ŏI�`��(����2)"	" �@�@�@�@���E�u�`���b�g���C�h�v" "\n"				*/
	{	(16384),	s_time(20), 	NULL,									DANMAKU_0e_remilia_00,			NULL,						sakuya_10_keitai,			},	/* "��10�`��: �ŏI�`��(����2)"	" �@�@�@�@�����u�`�F�b�N���C�h�v" "\n"				*/
	{	(16384),	s_time(20), 	NULL,									DANMAKU_0e_remilia_00,			NULL,						sakuya_10_keitai,			},	/* "��10�`��: �ŏI�`��(����2)"	" �@�@�@�@�����u�E�b�J�����C�h�v" "\n"				*/
	{	(16384),	s_time(20), 	NULL,									DANMAKU_0e_remilia_00,			NULL,						sakuya_10_keitai,			},	/* "��10�`��: �ŏI�`��(����2)"	" �@�@�@�@�����u�E�b�g�����C�h�v" "\n"				*/
//	0.5
	{	 (2048),	s_time(20), 	" �@�@���C�h�閧�u�c�s�s�׎蓖�v" "\n", DANMAKU_00, 					NULL,						sakuya_11_keitai,			},	/* "��11�`��: �ŏI�`��(����3)"					*/
	{	 (2048),	s_time(20), 	" �@�@���C�h��Z�u�́I�́I�́I�v" "\n", DANMAKU_00, 					NULL,						sakuya_11_keitai,			},	/* "��11�`��: �ŏI�`��(����3)"					*/
	{	 (2048),	s_time(20), 	" �@���C�h���u�����Ď��ʂ��v" "\n", DANMAKU_00, 					NULL,						sakuya_11_keitai,			},	/* "��11�`��: �ŏI�`��(����3)"					*/
	{	 (2048),	s_time(20), 	" �@�@���C�h���X�u���������{�X�v" "\n", DANMAKU_00, 					NULL,						sakuya_11_keitai,			},	/* "��11�`��: �ŏI�`��(����3)"					*/
//	0.25

// �p�`�F 5��						"feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{	 (40960),	s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{	 (40960),	s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{	 (40960),	s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{	 (40960),	s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
//	5.0
	{	  (8192),	s_time(30), 	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_27_PACHE_LASER2,		add_laser_on,				boss_move_01_not_move,		},/*boss_init_pache*/
	{	  (8192),	s_time(30), 	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_27_PACHE_LASER2,		add_laser_on,				boss_move_01_not_move,		},/*boss_init_pache*/
	{	  (8192),	s_time(30), 	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_27_PACHE_LASER2,		add_laser_on,				boss_move_01_not_move,		},/*boss_init_pache*/
	{	  (8192),	s_time(30), 	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_27_PACHE_LASER2,		add_laser_on,				boss_move_01_not_move,		},/*boss_init_pache*/
//	4.0
	{	  (8192),	s_time(30), 	"�@�@�Ε��u�p�`�����[���쐬E00�v" "\n", DANMAKU_00, 					add_laser_off,				pache_01_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�Ε��u�p�`�����[���쐬N00�v" "\n", DANMAKU_00, 					add_laser_off,				pache_01_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�Ε��u�p�`�����[���쐬H00�v" "\n", DANMAKU_00, 					add_laser_off,				pache_01_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�Ε��u�p�`�����[���쐬L00�v" "\n", DANMAKU_00, 					add_laser_off,				pache_01_keitai,			},
//	3.0
	{	  (8192),	s_time(30), 	"�@�@�����u�p�`�����[���쐬E01�v" "\n", DANMAKU_00, 					NULL,						pache_01_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�����u�p�`�����[���쐬N01�v" "\n", DANMAKU_00, 					NULL,						pache_01_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�����u�p�`�����[���쐬H01�v" "\n", DANMAKU_00, 					NULL,						pache_01_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�����u�p�`�����[���쐬L01�v" "\n", DANMAKU_00, 					NULL,						pache_01_keitai,			},
//	2.0
	{	  (8192),	s_time(30), 	"�@�@�ؕ��u�V���t�B�[���쐬E02�v" "\n", DANMAKU_00, 					add_zako_pache_dolls,		pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�ؕ��u�V���t�B�[���쐬N02�v" "\n", DANMAKU_00, 					add_zako_pache_dolls,		pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�ؕ��u�V���t�B�[���쐬H02�v" "\n", DANMAKU_00, 					add_zako_pache_dolls,		pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�ؕ��u�V���t�B�[���쐬L02�v" "\n", DANMAKU_00, 					add_zako_pache_dolls,		pache_04_keitai,			},
//	1.0
	{	  (8192),	s_time(30), 	"�@�@�����u�p�`�����[���쐬E03�v" "\n", DANMAKU_00, 					add_zako_pache_dolls,		pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�����u�p�`�����[���쐬N03�v" "\n", DANMAKU_00, 					add_zako_pache_dolls,		pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�����u�p�`�����[���쐬H03�v" "\n", DANMAKU_00, 					add_zako_pache_dolls,		pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�����u�p�`�����[���쐬L03�v" "\n", DANMAKU_00, 					add_zako_pache_dolls,		pache_04_keitai,			},
//	0.0
	{	  (8192),	s_time(30), 	"�@�@�y���u�p�`�����[���쐬E04�v" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�y���u�p�`�����[���쐬N04�v" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�y���u�p�`�����[���쐬H04�v" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�y���u�p�`�����[���쐬L04�v" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
//	0.0
	{	  (8192),	s_time(30), 	"�@�@�����u�p�`�����[���쐬E05�v" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�����u�p�`�����[���쐬N05�v" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�����u�p�`�����[���쐬H05�v" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"�@�@�����u�p�`�����[���쐬L05�v" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},

//	{	  (8192),	s_time(99), 	"�@�@�����u�T�C�����g���쐬E06�v" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
//	{	  (8192),	s_time(99), 	"�@�@�����u�T�C�����g���쐬N06�v" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
//	{	  (8192),	s_time(99), 	"�@�@�����u�T�C�����g���쐬H06�v" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
//	{	  (8192),	s_time(99), 	"�@�@�����u�T�C�����g���쐬L06�v" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},

//	{	  (8192),	s_time(99), 	"�@�@�X���u�p�`�����[���쐬E07�v" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
//	{	  (8192),	s_time(99), 	"�@�@�X���u�p�`�����[���쐬N07�v" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
//	{	  (8192),	s_time(99), 	"�@�@�X���u�p�`�����[���쐬H07�v" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
//	{	  (8192),	s_time(99), 	"�@�@�X���u�p�`�����[���쐬L07�v" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},



//	src->boss_base_danmaku_type 	= DANMAKU_15_aya_misogi;		/* �e�����Z�b�g */
//	src->boss_base_danmaku_type 	= DANMAKU_25_houka_kenran;		/* �e�����Z�b�g */	/*aya_02_keitai*/
//	src->boss_base_danmaku_type 	= DANMAKU_23_aya_merin_test;	/* �e�����Z�b�g */	/*aya_01_keitai*/

// �� 4��							"feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	//																										/*�{�X�o��O�̏�����[��b�̑O]*/
	{	(32768),	s_time(250),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					NULL,						common_00_keitai,			},/* 32768=8192*4.0 */
	{	(32768),	s_time(250),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					NULL,						common_00_keitai,			},/* 32768=8192*4.0 */
	{	(32768),	s_time(250),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					NULL,						common_00_keitai,			},/* 32768=8192*4.0 */
	{	(32768),	s_time(250),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					NULL,						common_00_keitai,			},/* 32768=8192*4.0 */
	//	4.0 																								/*�{�X�퓬�O�̏�����[��b�̌�]*/
	{		 (4096),s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				DANMAKU_20_aya_misogi1, 		init_00_boss_clip000,		boss_move_03_x_douki,		},/*4*/
	{		 (4096),s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				DANMAKU_20_aya_misogi1, 		init_00_boss_clip000,		boss_move_03_x_douki,		},/*4*/
	{		 (4096),s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				DANMAKU_20_aya_misogi1, 		init_00_boss_clip000,		boss_move_03_x_douki,		},/*4*/
	{		 (4096),s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				DANMAKU_20_aya_misogi1, 		init_00_boss_clip000,		boss_move_03_x_douki,		},/*4*/
	//	3.5
	{		 (4096),s_time(30), 	" �@�@�@�@�@�@�@�򕄁u�V�̔��ˁv" "\n", DANMAKU_22_ame_no_yatimata, 	init_00_boss_clip111,		boss_move_01_not_move,		},/*4*/
	{		 (4096),s_time(30), 	" �@�@�@�@�@�@�@�򕄁u�V�̔��ˁv" "\n", DANMAKU_22_ame_no_yatimata, 	init_00_boss_clip111,		boss_move_01_not_move,		},/*4*/
	{		 (4096),s_time(30), 	" �@�@�@�@�@�@�@�򕄁u�V�̔��ˁv" "\n", DANMAKU_22_ame_no_yatimata, 	init_00_boss_clip111,		boss_move_01_not_move,		},/*4*/
	{		 (4096),s_time(30), 	" �@�@�@�@�@�@�@�򕄁u�V�̔��ˁv" "\n", DANMAKU_22_ame_no_yatimata, 	init_00_boss_clip111,		boss_move_01_not_move,		},/*4*/
	//	3.0
	{		 (4096),s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				DANMAKU_21_aya_misogi2, 		init_00_boss_clip000,		boss_move_03_x_douki,		},/*5"�򕄁u���쐬�X�y�J001�v"*/
	{		 (4096),s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				DANMAKU_21_aya_misogi2, 		init_00_boss_clip000,		boss_move_03_x_douki,		},/*5"�򕄁u���쐬�X�y�J001�v"*/
	{		 (4096),s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				DANMAKU_21_aya_misogi2, 		init_00_boss_clip000,		boss_move_03_x_douki,		},/*5"�򕄁u���쐬�X�y�J001�v"*/
	{		 (4096),s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				DANMAKU_21_aya_misogi2, 		init_00_boss_clip000,		boss_move_03_x_douki,		},/*5"�򕄁u���쐬�X�y�J001�v"*/
	//	2.5
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@ �E���u�n����D�]�v" "\n", DANMAKU_24_aya_doll,			NULL,						boss_move_03_x_douki,		},/*4"�򕄁u���쐬�X�y�J002�v"*/
	{		 (4096),s_time(40), 	"�@�@�@�@�@ �����u�n���卪��Q�v" "\n", DANMAKU_24_aya_doll,			NULL,						boss_move_03_x_douki,		},/*4"�򕄁u���쐬�X�y�J002�v"*/
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@ �ߏ�u�R�����Q�v" "\n", DANMAKU_24_aya_doll,			NULL,						boss_move_03_x_douki,		},/*4"�򕄁u���쐬�X�y�J002�v"*/
	{		 (4096),s_time(40), 	"�@�@�@�@�@ ��ʁu�����\\���ۉ߁v" "\n",DANMAKU_24_aya_doll,			NULL,						boss_move_03_x_douki,		},/*4"�򕄁u���쐬�X�y�J002�v"*/
	//	2.0
	{		 (4096),s_time(100),	"�@�@�@�@�@�@�@ �����u�����R�l�v" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5"�򕄁u���쐬�X�y�J003�v"*/
	{		 (4096),s_time(100),	"�@�@�@�@�@�@ �����u�����R�R�l�v" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5"�򕄁u���쐬�X�y�J003�v"*/
	{		 (4096),s_time(100),	"�@�@�@�@�@�@ �����u�����X�W�l�v" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5"�򕄁u���쐬�X�y�J003�v"*/
	{		 (4096),s_time(100),	"�@�@�@�@�@�@ �����u�������蒆�v" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5"�򕄁u���쐬�X�y�J003�v"*/
	//	1.5
	{		 (4096),s_time(99), 	"�@�@�@�@�@�@ �H���u�����̒��ׁv" "\n", DANMAKU_2a_amefuri_test,		init_00_boss_clip111,		boss_move_01_not_move,		},/*4"�򕄁u���쐬�X�y�J004�v"*/
	{		 (4096),s_time(99), 	"�@�@�@�@�@�@ �H���u����̒��ׁv" "\n", DANMAKU_2a_amefuri_test,		init_00_boss_clip111,		boss_move_01_not_move,		},/*4"�򕄁u���쐬�X�y�J004�v"*/
	{		 (4096),s_time(99), 	"�@�@�@�@�@�@ �H���u�Ӌ|�̒��ׁv" "\n", DANMAKU_2a_amefuri_test,		init_00_boss_clip111,		boss_move_01_not_move,		},/*4"�򕄁u���쐬�X�y�J004�v"*/
	{		 (4096),s_time(99), 	"�@�@�@�@ �ǐՁu�R�����{�̒��ׁv" "\n", DANMAKU_2a_amefuri_test,		init_00_boss_clip111,		boss_move_01_not_move,		},/*4"�򕄁u���쐬�X�y�J004�v"*/
	//	1.0
	{		 (4096),s_time(99), 	"�@�@ �ؕ��u���남�ڂ��F�؈�ࣁv" "\n", DANMAKU_25_houka_kenran,		init_00_boss_clip000,		boss_move_01_not_move,		},/*5*/
	{		 (4096),s_time(99), 	"�@�@�@�@�@�@�@ �Ǖ��u�R�_�n��v" "\n", DANMAKU_26_aya_saifu,			init_00_boss_clip000,		boss_move_01_not_move,		},/*5*/
	{		 (4096),s_time(99), 	"�@�@�@�@ �Ǖ��u�V��V���̏ƚ��v" "\n", DANMAKU_26_aya_saifu,			init_00_boss_clip000,		boss_move_01_not_move,		},/*5*/
	{		 (4096),s_time(99), 	"�@�@�@�@ �Ǖ��u�B��Ƒ��̏ƚ��v" "\n", DANMAKU_26_aya_saifu,			init_00_boss_clip000,		boss_move_01_not_move,		},/*5*/
	//	0.5
//	{		 (4096),s_time(99), 	" �@�@�@ �򕄁u���쐬�X�y�JE06�v" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5*/
//	{		 (4096),s_time(99), 	" �@�@�@ �򕄁u���쐬�X�y�JN06�v" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5*/
//	{		 (4096),s_time(99), 	" �@�@�@ �򕄁u���쐬�X�y�JH06�v" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5*/
//	{		 (4096),s_time(99), 	" �@�@�@ �򕄁u���쐬�X�y�JL06�v" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5*/
	//	0.0
//	{		(16384),s_time(99), 	" �@�@�@ �򕄁u���쐬�X�y�JE07�v" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5*/
//	{		(16384),s_time(99), 	" �@�@�@ �򕄁u���쐬�X�y�JN07�v" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5*/
//	{		(16384),s_time(99), 	" �@�@�@ �򕄁u���쐬�X�y�JH07�v" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5*/
//	{		(16384),s_time(99), 	" �@�@�@ �򕄁u���쐬�X�y�JL07�v" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5*/

//	src->boss_base_danmaku_type 	= DANMAKU_1e_kaguya01;		/* �e�����Z�b�g */	�{�X�s���A�� 1�`��
//	src->boss_base_danmaku_type 	= DANMAKU_1f_kaguya04;		/* �e�����Z�b�g */	�{�X�s���A�� 4�`��

// �P�� 3�� 						"feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{		(28672),s_time(250),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 28672==8192*3.5 */
	{		(28672),s_time(250),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 28672==8192*3.5 */
	{		(28672),s_time(250),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 28672==8192*3.5 */
	{		(28672),s_time(250),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 28672==8192*3.5 */
	// 3.5
	{		 (4096),s_time(23), 	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_1e_kaguya01,			boss_init_kaguya,			kaguya_01_keitai,			}, // ����(5)�҂�e�A�ԁA�B���@�_���A�e(8)��18�Z�b�g(�b)���炢(�n�߂�5�b�͏o���Ȃ�)�B(�P����Ԋu�ňړ��A�I���㒆���ֈړ�)
	{		 (4096),s_time(23), 	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_1e_kaguya01,			boss_init_kaguya,			kaguya_01_keitai,			}, // ����(5)�҂�e�A�ԁA�B���@�_���A�e(8)��18�Z�b�g(�b)���炢(�n�߂�5�b�͏o���Ȃ�)�B(�P����Ԋu�ňړ��A�I���㒆���ֈړ�)
	{		 (4096),s_time(23), 	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_1e_kaguya01,			boss_init_kaguya,			kaguya_01_keitai,			}, // ����(5)�҂�e�A�ԁA�B���@�_���A�e(8)��18�Z�b�g(�b)���炢(�n�߂�5�b�͏o���Ȃ�)�B(�P����Ԋu�ňړ��A�I���㒆���ֈړ�)
	{		 (4096),s_time(23), 	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_1e_kaguya01,			boss_init_kaguya,			kaguya_01_keitai,			}, // ����(5)�҂�e�A�ԁA�B���@�_���A�e(8)��18�Z�b�g(�b)���炢(�n�߂�5�b�͏o���Ȃ�)�B(�P����Ԋu�ňړ��A�I���㒆���ֈړ�)
	// 3.0
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬E01�v" "\n", DANMAKU_1e_kaguya01,			add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 1.������̋�	(�P�钆���Œ�A�Ԏ��Ή��̊ےe�A�Z���[�U�[)4�Z�b�gx
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬N01�v" "\n", DANMAKU_1e_kaguya01,			add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 1.������̋�	(�P�钆���Œ�A�Ԏ��Ή��̊ےe�A�Z���[�U�[)4�Z�b�gx
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬H01�v" "\n", DANMAKU_1e_kaguya01,			add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 1.������̋�	(�P�钆���Œ�A�Ԏ��Ή��̊ےe�A�Z���[�U�[)4�Z�b�gx
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬L01�v" "\n", DANMAKU_1e_kaguya01,			add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 1.������̋�	(�P�钆���Œ�A�Ԏ��Ή��̊ےe�A�Z���[�U�[)4�Z�b�gx
	// 2.5
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬E02�v" "\n", DANMAKU_1e_kaguya01,			add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 2.���̌�΂̔�
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬N02�v" "\n", DANMAKU_1e_kaguya01,			add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 2.���̌�΂̔�
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬H02�v" "\n", DANMAKU_1e_kaguya01,			add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 2.���̌�΂̔�
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬L02�v" "\n", DANMAKU_1e_kaguya01,			add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 2.���̌�΂̔�
	// 2.0
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬E03�v" "\n", DANMAKU_1f_kaguya04,			add_zako_kaguya_dolls,		kaguya_04_keitai,			},// 3.�Αl�̔��
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬N03�v" "\n", DANMAKU_1f_kaguya04,			add_zako_kaguya_dolls,		kaguya_04_keitai,			},// 3.�Αl�̔��
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬H03�v" "\n", DANMAKU_1f_kaguya04,			add_zako_kaguya_dolls,		kaguya_04_keitai,			},// 3.�Αl�̔��
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬L03�v" "\n", DANMAKU_1f_kaguya04,			add_zako_kaguya_dolls,		kaguya_04_keitai,			},// 3.�Αl�̔��
	// 1.5
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬E04�v" "\n", DANMAKU_1f_kaguya04,			add_zako_kaguya_dolls,		kaguya_04_keitai,			},// 4.���̎q���L
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬N04�v" "\n", DANMAKU_1f_kaguya04,			add_zako_kaguya_dolls,		kaguya_04_keitai,			},// 4.���̎q���L
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬H04�v" "\n", DANMAKU_1f_kaguya04,			add_zako_kaguya_dolls,		kaguya_04_keitai,			},// 4.���̎q���L
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬L04�v" "\n", DANMAKU_1f_kaguya04,			add_zako_kaguya_dolls,		kaguya_04_keitai,			},// 4.���̎q���L
	// 1.0
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬E05�v" "\n", DANMAKU_1c_kakuya_tamanoe,		add_zako_kaguya_dolls,		boss_move_01_not_move,		},// 5.�H���̋ʂ̎}
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬N05�v" "\n", DANMAKU_1c_kakuya_tamanoe,		add_zako_kaguya_dolls,		boss_move_01_not_move,		},// 5.�H���̋ʂ̎}
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬H05�v" "\n", DANMAKU_1c_kakuya_tamanoe,		add_zako_kaguya_dolls,		boss_move_01_not_move,		},// 5.�H���̋ʂ̎}
	{		 (4096),s_time(40), 	"�@�@�@�@�@�@�@���u���쐬L05�v" "\n", DANMAKU_1c_kakuya_tamanoe,		add_zako_kaguya_dolls,		boss_move_01_not_move,		},// 5.�H���̋ʂ̎}
	// 0.5
	{		 (4096),s_time(40), 	"�@�@�@�@���u�H���̋ʂ̎}E06�v" "\n", DANMAKU_1c_kakuya_tamanoe,		add_zako_kaguya_dolls,		boss_move_01_not_move,		},//
	{		 (4096),s_time(40), 	"�@�@�@�@���u�H���̋ʂ̎}N06�v" "\n", DANMAKU_1c_kakuya_tamanoe,		add_zako_kaguya_dolls,		boss_move_01_not_move,		},//
	{		 (4096),s_time(40), 	"�@�@�@�@���u�H���̋ʂ̎}H06�v" "\n", DANMAKU_1c_kakuya_tamanoe,		add_zako_kaguya_dolls,		boss_move_01_not_move,		},//
	{		 (4096),s_time(40), 	"�@�@�@�@���u�H���̋ʂ̎}L06�v" "\n", DANMAKU_1c_kakuya_tamanoe,		add_zako_kaguya_dolls,		boss_move_01_not_move,		},//

// ���� 2�� 						"feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{		(32768),s_time(200),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 20480==8192*2.5 */
	{		(32768),s_time(200),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 20480==8192*2.5 */
	{		(32768),s_time(200),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 20480==8192*2.5 */
	{		(32768),s_time(200),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 20480==8192*2.5 */
	// 4.0
	{		 (4096),s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_1a_mima_toge,			NULL,						mima_01_keitai, 			},
	{		 (4096),s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_1a_mima_toge,			NULL,						mima_01_keitai, 			},
	{		 (4096),s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_1a_mima_toge,			NULL,						mima_01_keitai, 			},
	{		 (4096),s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_1a_mima_toge,			NULL,						mima_01_keitai, 			},
	// 3.5
	{		 (4096),s_time(20), 	"�@�@�@�@����u���쐬�X�y�JE01�v" "\n", DANMAKU_18_hana_test,			NULL,						mima_01_keitai, 			},
	{		 (4096),s_time(20), 	"�@�@�@�@����u���쐬�X�y�JN01�v" "\n", DANMAKU_18_hana_test,			NULL,						mima_01_keitai, 			},
	{		 (4096),s_time(20), 	"�@�@�@�@����u���쐬�X�y�JH01�v" "\n", DANMAKU_18_hana_test,			NULL,						mima_01_keitai, 			},
	{		 (4096),s_time(20), 	"�@�@�@�@����u���쐬�X�y�JL01�v" "\n", DANMAKU_18_hana_test,			NULL,						mima_01_keitai, 			},
	// 3.0
	{		 (4096),s_time(40), 	"�@�@�@�@����u���쐬�X�y�JE02�v" "\n", DANMAKU_29_rumia_demarcation,	NULL,						boss_move_03_x_douki,		},//"�@�@ �ŕ��u�f�B�}�[�P�C�V�����v"
	{		 (4096),s_time(40), 	"�@�@�@�@����u���쐬�X�y�JN02�v" "\n", DANMAKU_29_rumia_demarcation,	NULL,						boss_move_03_x_douki,		},
	{		 (4096),s_time(40), 	"�@�@�@�@����u���쐬�X�y�JH02�v" "\n", DANMAKU_29_rumia_demarcation,	NULL,						boss_move_03_x_douki,		},
	{		 (4096),s_time(40), 	"�@�@�@�@����u���쐬�X�y�JL02�v" "\n", DANMAKU_29_rumia_demarcation,	NULL,						boss_move_03_x_douki,		},
	// 2.5
	{		 (4096),s_time(40), 	"�@�@�@�@����u���쐬�X�y�JE03�v" "\n", DANMAKU_23_aya_merin_test,		init_00_boss_clip111,		boss_move_01_not_move,		},
	{		 (4096),s_time(40), 	"�@�@�@�@����u���쐬�X�y�JN03�v" "\n", DANMAKU_23_aya_merin_test,		init_00_boss_clip111,		boss_move_01_not_move,		},
	{		 (4096),s_time(40), 	"�@�@�@�@����u���쐬�X�y�JH03�v" "\n", DANMAKU_23_aya_merin_test,		init_00_boss_clip111,		boss_move_01_not_move,		},
	{		 (4096),s_time(40), 	"�@�@�@�@����u���쐬�X�y�JL03�v" "\n", DANMAKU_23_aya_merin_test,		init_00_boss_clip111,		boss_move_01_not_move,		},
	// 2.0
	{		 (4096),s_time(40), 	"�@�@�@�@����u���쐬�X�y�JE04�v" "\n", DANMAKU_26_aya_saifu,			init_00_boss_clip000,		boss_move_01_not_move,		},
	{		 (4096),s_time(40), 	"�@�@�@�@����u���쐬�X�y�JN04�v" "\n", DANMAKU_26_aya_saifu,			init_00_boss_clip000,		boss_move_01_not_move,		},
	{		 (4096),s_time(40), 	"�@�@�@�@����u���쐬�X�y�JH04�v" "\n", DANMAKU_26_aya_saifu,			init_00_boss_clip000,		boss_move_01_not_move,		},
	{		 (4096),s_time(40), 	"�@�@�@�@����u���쐬�X�y�JL04�v" "\n", DANMAKU_26_aya_saifu,			init_00_boss_clip000,		boss_move_01_not_move,		},
	// 1.5
	{		 (4096),s_time(40), 	"�@�@�@�@����u���쐬�X�y�JE05�v" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},
	{		 (4096),s_time(40), 	"�@�@�@�@����u���쐬�X�y�JN05�v" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},
	{		 (4096),s_time(40), 	"�@�@�@�@����u���쐬�X�y�JH05�v" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},
	{		 (4096),s_time(40), 	"�@�@�@�@����u���쐬�X�y�JL05�v" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},
	// 1.0
	{		 (4096),s_time(40), 	"�@�@�@�@����u�Ă��ƃX�y�JE06�v" "\n", DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},// �s���㗈�Ȃ��B
	{		 (4096),s_time(40), 	"�@�@�@�@����u�Ă��ƃX�y�JN06�v" "\n", DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},// �s���㗈�Ȃ��B
	{		 (4096),s_time(40), 	"�@�@�@�@����u�Ă��ƃX�y�JH06�v" "\n", DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},// �s���㗈�Ȃ��B
	{		 (4096),s_time(40), 	"�@�@�@�@����u�Ă��ƃX�y�JL06�v" "\n", DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},// �s���㗈�Ȃ��B
/*
	�����l�͈ꉞ�u���@�����v�Ȃ񂾂���A�Y��Ȃ��ł�ˁI
*/

// �A���X 1��						"feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{	(24576),	 s_time(120),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 16384==8192*2 */	/* "�ʏ�U��"�̎���(?) */ /* "�ʏ�U��"�̃��C�t */
	{	(24576),	 s_time(120),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 16384==8192*2 */	/* "�ʏ�U��"�̎���(?) */ /* "�ʏ�U��"�̃��C�t */
	{	(24576),	 s_time(120),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 16384==8192*2 */	/* "�ʏ�U��"�̎���(?) */ /* "�ʏ�U��"�̃��C�t */
	{	(24576),	 s_time(120),	NULL,/* "�`�ԕύX" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 16384==8192*2 */	/* "�ʏ�U��"�̎���(?) */ /* "�ʏ�U��"�̃��C�t */
	// 3.0
	{	 (4096),	 s_time(10),	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_14_alice_youmu300,		NULL,						boss_move_01_not_move,		},
	{	 (4096),	 s_time(10),	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_14_alice_youmu300,		NULL,						boss_move_01_not_move,		},
	{	 (4096),	 s_time(10),	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_13_alice_suwako,		NULL,						boss_move_01_not_move,		},
	{	 (4096),	 s_time(10),	NULL,/* "�ʏ�U��" "\n"*/				DANMAKU_13_alice_suwako,		NULL,						boss_move_01_not_move,		},
	// 2.5
	{	 (3072),	 s_time(20),	" �@�@�@�@�@���Áu�g�̖��@Lv.2�v" "\n", DANMAKU_15_alice_aka_2nd,		NULL,						boss_move_01_not_move,		},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 (3072),	 s_time(20),	"  �@�@�@�@���Áu�g�̖��@Lv.16�v" "\n", DANMAKU_15_alice_aka_2nd,		NULL,						boss_move_01_not_move,		},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 (3072),	 s_time(20),	" �@�@�@�@���Áu�g�̖��@Lv.256�v" "\n", DANMAKU_15_alice_aka_2nd,		NULL,						boss_move_01_not_move,		},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 (3072),	 s_time(20),	" �@�@�@���Áu�g�̖��@Lv.65536�v" "\n", DANMAKU_15_alice_aka_2nd,		NULL,						boss_move_01_not_move,		},/* "�����u�M���̕������l�`�v"�̃��C�t */
	// 2.125
	{	 (3072),	 s_time(20),	"   �@�����u�p�[�V�����t���[�Y�v" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 (3072),	 s_time(20),	" �@�����u�p�[�t�F�N�g�t���[�Y�v" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 (3072),	 s_time(20),	" �@�����u�p�[�e�B�N���t���[�N�v" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 (3072),	 s_time(20),	" �@�����u�u���U�[�h�t���[�Y���v" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},/* "�����u�M���̕������l�`�v"�̃��C�t */
	// 1.75
	{	(12288),	 s_time(40),	" �@�����u�����̃}�g�����[�V�J�v" "\n", DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},/* " �@�����u�����̃}�g�����[�V�J�v"�����u�����̉~�J�l�`�v */
	{	(12288),	 s_time(40),	" �@�����u�M���̃}�g�����[�V�J�v" "\n", DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},/* " �@�����u�M���̃}�g�����[�V�J�v"�����u�����̉~�J�l�`�v */
	{	(12288),	 s_time(40),	" �@�����u�����̃}�g�����[�V�J�v" "\n", DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},/* " �@�����u�����̃}�g�����[�V�J�v"�����u�����̉~�J�l�`�v */
	{	(12288),	 s_time(40),	" �@�����u���Q�̃}�g�����[�V�J�v" "\n", DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},/* " �@�����u���Q�̃}�g�����[�V�J�v"�����u�����̉~�J�l�`�v */
	// 0.25
	{	 (3072),	 s_time(40),	NULL,/* "�ʏ�U��z" "\n"*/				DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},/* " �@�@�@�@�����u�W����̈ĎR�q�v"�����u�����̉~�J�l�`�v */
	{	 (3072),	 s_time(40),	NULL,/* "�ʏ�U��z" "\n"*/				DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},/* " �@�@�@�@�@�����u���̋@�I�l�`�v"�����u�����̉~�J�l�`�v */
	{	 (3072),	 s_time(40),	NULL,/* "�ʏ�U��z" "\n"*/				DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},/* " �@�@�@�@�@�@�����u�Â̗������v"�����u�����̉~�J�l�`�v */
	{	 (3072),	 s_time(40),	NULL,/* "�ʏ�U��z" "\n"*/				DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},/* " �@�����u��̃N���b�V���_�~�[�v"�����u�����̉~�J�l�`�v */
//									"feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
};
//	" �@�@�@�����u�����̕������l�`�v" "\n"
//	" �@�����u�����̃I�����A�\\�l�`�v" "\n" " �@�����u�����̃I�����A���l�`�v"...���݂̕����ł͖����ł��B
//	{	 (4096),	 s_time(40),	"�@�����u���쐬�̈����ɐl�`L01�v" "\n", DANMAKU_00, 					NULL,						alice_04_keitai,			},/* �����u�����̉~�J�l�`�v */
//	{	(20000),	 s_time(20),	"�����u���쐬�̈ɑ������l�`L02�v" "\n", DANMAKU_00, 					NULL,						boss_move_02_xy_hidouki,	},/* �����u�󑐂̐l�`�Ă��v */
//	{	 (5000),	 s_time(20),	"�@�����u���쐬�̉p�g���l�`L03�v" "\n", DANMAKU_00, 					NULL,						alice_02_keitai,			},
//	{	 (5000),	 s_time(20),	"�@�����u���쐬�̘I�����l�`L04�v" "\n", DANMAKU_00, 					NULL,						alice_02_keitai,			},
//	{	 (5000),	 s_time(80),	"�@�@�����u���쐬�̐����l�`L05�v" "\n", DANMAKU_00, 					NULL,						alice_02_keitai,			},
//	{	 (5000),	 s_time(80),	"�@�����u���쐬�̔����`�l�`L06�v" "\n", DANMAKU_00, 					NULL,						alice_02_keitai,			},
//	{	 (5000),	 s_time(80),	"�@�@�����u���쐬�̔g���l�`L07�v" "\n", DANMAKU_00, 					NULL,						alice_02_keitai,			},
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




/*---------------------------------------------------------
	�X�y���J�[�h�V�X�e���̃{�X�`�Ԃɓo�^���ꂽ�e���ԍ����擾���A
	�����ɒe���̎��Ԑ؂��ݒ肷��B
---------------------------------------------------------*/

global void spell_card_get_danmaku_number(SPRITE *src)
{
	src->boss_base_danmaku_type 	= my_spell_card_resource[(spell_card_number)].danmaku_type; 	/* �e�����Z�b�g */
	danmaku_set_time_out(src);		/* �e���̐������Ԃ�ݒ�(�\�ߒe�����Ƃɐݒ肳��Ă���W�����Ԃɐݒ�) */
}


/*---------------------------------------------------------
	�{�X�`�ԕύX���̋��ʃ��[�`��
	�X�y�J���j��A�C�e���o���B
---------------------------------------------------------*/
//	#define alice_put_items 	common_boss_put_items
//	#define aya_put_items		common_boss_put_items
//	#define mima_put_items		common_boss_put_items
//	#define kaguya_put_items	common_boss_put_items
//	#define pache_put_items 	common_boss_put_items
//	h->callback_mover			= move_aya;
//	h->callback_mover			= move_alice;
//	h->callback_loser			= aya_put_items;

//
global void common_boss_put_items(SPRITE *src)
{
//++	pd_bo ssmode=B04_CHANGE;
	bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
	#if (0)/*�����*/
	bakuhatsu_add_circle(src, 1);
	#endif
//
	voice_play(VOICE07_BOMB, TRACK02_ALEART_IVENT);/*�e�L�g�[*/
	#if (1)
	/*---------------------------------------------------------
		�X�y�J���̕\��
		�X�y�J�w�i�́A�܂��������ǁA�����I�Ɋg���\��B
	---------------------------------------------------------*/
	//static void draw_spell_card_name(void)
	{
		kanji_window_clear_line(1); 	/* �����E�B���h�E��2�s��(==1)�̓��e�������B */
		set_cursor(0, 1);				/* �J�[�\����2�s��(==1)�ֈړ� */
	//
		if (NULL != my_spell_card_resource[(spell_card_number)].spell_str_name)
		{
			/* �X�y�J�w�i������ꍇ */
			callback_gu_draw_haikei = callback_gu_draw_haikei_supeka;
			//
			msg_time = (60*5);
			print_kanji000(
				my_spell_card_resource[(spell_card_number)].spell_str_name,
				/*int color_type*/(7)|STR_CODE_NO_ENTER,	/* ���s���Ȃ� */
				/*int wait*/(0)
			);
		}
		else
		{
			/* �X�y�J�w�i�������ꍇ */
			callback_gu_draw_haikei = callback_gu_draw_haikei_modosu;
		}
	}
	#endif
	/*--- BOSS ���ʂ��Ēl�� 0 �ɂ���B------------*/
	src->boss_time_out = (0);
//	src->boss_base_state777 = (0);/* ���ʂ��� �l�� 0 �ɂ��� */
//	player_dummy_add_score(src->boss_base_score);
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
//	SPRITE *obj_boss;
//	obj_boss = &obj00[FIX_OBJ_08_BOSS];
	{
		int limit_max_hp_dec_boss_by_flame;
		u8 test_draw_boss_hp_value;
		/* 8192/32 == 256 */
		test_draw_boss_hp_value = (global_obj_boss->base_hp>>5) & 0xff;
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
	global_obj_boss->base_hp -= boss_hp_dec_by_frame;
	boss_hp_dec_by_frame = 0;/*�g�����̂ŏ���(�t���[���P��)*/
	if (spell_card_limit_health >= global_obj_boss->base_hp)		/* �K��l�ȉ��ɂȂ�΃X�y�J���[�h���� */
	{
		spell_card_mode 			= (SPELL_CARD_MODE_00_OFF);
		boss_destroy_check_type(global_obj_boss, DESTROY_CHECK_00_WIN_BOSS);
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
	src->cx256 += (src->vx256);
	src->cy256 += (src->vy256);
}
global void boss_clip_rect(SPRITE *src)
{
		 if (src->cx256 < boss_clip_min.x256 )	{	src->cx256 = boss_clip_min.x256; 	boss_hamidasi=1;	}
	else if (src->cx256 > boss_clip_max.x256 )	{	src->cx256 = boss_clip_max.x256; 	boss_hamidasi=1;	}
//
		 if (src->cy256 < boss_clip_min.y256 )	{	src->cy256 = boss_clip_min.y256; 	boss_hamidasi=1;	}
	else if (src->cy256 > boss_clip_max.y256 )	{	src->cy256 = boss_clip_max.y256; 	boss_hamidasi=1;	}
}

// ����̃V�i���I�X�N���v�g�K�i���ƃX�y�J�V�X�e���Ƒ����������B
// �d���Ȃ��̂ŁA�����Ƀt���O�őΉ�������B
#define USE_OLD_SCRIPT_SYSTEM (1)

/*---------------------------------------------------------
	�X�y�J�����ɐi�߂�B
---------------------------------------------------------*/
static void spell_card_incliment(void)
{
			spell_card_number += (4)/*1*/;
}

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
			aaa = spell_card_limit_health - (my_spell_card_resource[(spell_card_number+(4)/*1*/)].spell_life);	/*1000 500*/
		//	spell_card_limit_health -= 1000/*500*/;
			/* �{�X���X�y�J�����Ă���̗͂�����ꍇ */
			if (0 < aaa)
			{
				spell_card_limit_health = aaa;
			//	if (b_health_alter_low1024 < (data->boss_base.boss_health & (1024-1)))/* �`�ԕύX������ */
				spell_card_mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/
				spell_card_incliment();
			//	if (spell_card_syoji_maisuu < spell_card_number)
				if (0 == spell_card_syoji_maisuu )
				{
					/* �`�ԕύX���Ȃ��A�A�C�e���f���Ȃ� */
					spell_card_number -= (4);
				//	src->callback_loser 			= lose_boss;
				}
				else
		//		if (0==sakuya->base_hp)
				{
					spell_card_syoji_maisuu--;
		//			if (0 != (data->boss_base.boss_life))/* �`�ԕύX������ */
					/* �A�C�e���f�� */
					if (NULL != src->callback_loser)
					{
		//				data->boss_base.boss_life--;
						(src->callback_loser)(src); 	/* sakuya_put_items(src); */
					}
				}
				#if 0/* (��[�X�y�J�I������珉����]) (�`�ԕύX����O�ɕK���������������̂ł��̈ʒu�͎~�߂�B)*/
				/* �X�y���J�[�h������ */
				if (NULL != my_spell_card_resource[(spell_card_number)].spell_init_callback)
				{
					(my_spell_card_resource[(spell_card_number)].spell_init_callback)(src);
				}
				#endif
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
	//	src->target_y256			= src->cy256;
		src->target_y256			= t256(16.0); //t256(0);
		#if 1/* (�V[�X�y�J�n�܂�O�ɏ�����]) ��0�`�Ԃ���A�K���ĂԔ��B */
		/* �X�y���J�[�h������ */
		if (NULL != my_spell_card_resource[(spell_card_number)].spell_init_callback)
		{
			(my_spell_card_resource[(spell_card_number)].spell_init_callback)(src);
		}
		#endif
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

	//	if (t256(/*(GAME_WIDTH/2)-32*/144) > src->cx256) 	/* ��ʔ����̈ʒu���኱���B */
	//	{
	//		src->cx256 += t256(1.0); 	/* �E �ɑޔ� */
	//	}
	//	else
	//	if (t256(/*(GAME_WIDTH/2)+32*/208) < src->cx256) 	/* ��ʔ����̈ʒu���኱�E�B */
	//	{
	//		src->cx256 -= t256(1.0); 	/* �� �ɑޔ� */
	//	}

/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]
---------------------------------------------------------*/

//global /*static*/ void NULL(SPRITE *src)
//{
//	/* ���݃_�~�[ */
//}
/*global*/static /*static*/ void common_03_keitai(SPRITE *src)
{
//		if ( ((pd.state_flag) & STATE_FLAG_05_IS_BOSS) )	/* ��b�I��? */
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
			spell_card_incliment();
		}
}
/*global*/static /*static*/ void common_02_keitai(SPRITE *src)
{
	//	if ( ((pd.state_flag) & STATE_FLAG_05_IS_BOSS) )	/* ��b�I��? */
	//	{
	//		my_spell_card_resource[(spell_card_number)].spell_move_callback = common_03_keitai;
	//	}
}
global /*static*/ void common_00_keitai(SPRITE *src)
{
//	if (0 == src->vvv256)
	if (0 > src->boss_base_state777)
	{
		src->boss_base_state777 = (0);
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
	if (0 > (src->cy256+t256(50.0)))/* +t256(50.0) �{�X�O���̍ő�T�C�Y(50[dot]) */
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
		src->cy256 -= t256(0.75);					/* ��ɑޔ� */			/* t256(0.75) �ޔ𑬓x 0.75[dot/flame] */	/*fps_factor*/
		if ( (int)(GAME_WIDTH*256/2) > src->cx256)	/* ��ʔ����̈ʒu */
		{
			src->cx256 += t256(1.0); 	/* �E��ɑޔ� */
		}
		else
		{
			src->cx256 -= t256(1.0); 	/* ����ɑޔ� */
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
		src->boss_base_state777++;
	}
}




/*---------------------------------------------------------
	�{�X�̋��ʁA�P��ڏ��������[�`��(����A�U���s��)
---------------------------------------------------------*/


/*---------------------------------------------------------
	�e���͈̔͂��u�W���v�ɐݒ�
---------------------------------------------------------*/
global void set_default_bullet_clip(void)/* call from load_stage.c */
{
	bullet_clip_min.x256 = t256(-32);
	bullet_clip_max.x256 = t256(0)+t256(GAME_WIDTH);
	bullet_clip_min.y256 = t256(-32);
	bullet_clip_max.y256 = t256(0)+t256(GAME_HEIGHT);
}


/*---------------------------------------------------------
	�e���͈̔͂��u��Ɖ��̍L�͈́v�ɐݒ�
---------------------------------------------------------*/
global void set_aya_bullet_clip(void)/* call from load_stage.c */
{
	bullet_clip_min.x256 = t256(-100);
	bullet_clip_max.x256 = t256(100)+t256(GAME_WIDTH);
	bullet_clip_min.y256 = t256(-256);/* ���� */
	bullet_clip_max.y256 = t256(100)+t256(GAME_HEIGHT);
}
//global void set_default_bullet_clip(void);/* call from load_stage.c */
//global void set_aya_bullet_clip(void);/* */

global void init_00_boss_clip000(SPRITE *h)/* �W���^�C�v */
{
	set_default_bullet_clip();	/* �e���͈̔͂�W���ɐݒ� */
}
global void init_00_boss_clip111(SPRITE *h)/* ��ɍL���^�C�v */
{
	set_aya_bullet_clip();	/* �e���͈̔͂�W���ɐݒ� */
}

	//------------ ���ꏉ����
	#if 0
	/*	common_boss_init(); ����̕K�v������*/
	{
		/* boss_rect_init */
		boss_clip_min.x256	= t256(0.0);
		boss_clip_min.y256	= t256(0.0);
		boss_clip_max.x256	= t256((GAME_WIDTH-50));
		/*
		0	easy:	128 = 128+(32x0) (���܂�Ȃ�)
		1	normal: 160 = 128+(32x1) (���܂�Ȃ�)
		2	hard:	192 = 128+(32x2) (���܂�Ȃ�)
		3	luna:	224 = 128+(32x3) (���܂��)
		*/
	//	boss_clip_max.y256	= (t256(128.0)+(difficulty<<(8+5)));	/*t256(96.0)*/
		/* (���܂�Ȃ�)
		0	easy:	128 = 128+(16x0) (���܂�Ȃ�)
		1	normal: 144 = 128+(16x1) (���܂�Ȃ�)
		2	hard:	160 = 128+(16x2) (���܂�Ȃ�)
		3	luna:	176 = 128+(16x3) (���܂�Ȃ�)
		*/
		boss_clip_max.y256	= (t256(128.0)+(difficulty<<(8+4)));	/*t256(96.0)*/
	}
	#endif

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
/* from game_core.c */
global void add_boss_common(STAGE_DATA *l)
{
		pd.state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
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
	h									= global_obj_boss;/*�P��{�l*/


//	SPRITE *obj_boss;
//	obj_boss = &obj00[FIX_OBJ_08_BOSS];
//	SPRITE *h;
//	h = &obj00[FIX_OBJ_08_BOSS];

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
		h->cx256 						= BOSS_XP256;/*t256(GAME_WIDTH/2)*/
		h->cy256 						= t256(-100);
	//
	//	h->boss_base_state777			= (0);	/*ST_00*/
		h->boss_base_state777			= (-1); /*ST_00*/
	//
		/* 3�ʂ̏ꍇ---- */ 	/* �S���ꏏ */
		/* 4�ʂ̏ꍇ---- */ 	/* �P��{�l */
		{
			static const unsigned int aaa_score_tbl[8/*8*/] =
			{
				score(1000000), 		/* �`���m */	/* �G�L�X�g���p(boss0) */
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
			/* �{�X�J�n�X�y�J�ԍ�(easy) */
				SPELL_CARD_00E_chrno_000,	/* �`���m(?) */ 	/* �G�L�X�g���p(boss0) */
				SPELL_CARD_00E_alice_000,	/* �A���X */
				SPELL_CARD_00E_mima_000,	/* ���� */
				SPELL_CARD_00E_kaguya_000,	/* �P�� */
			//
				SPELL_CARD_00E_aya_000, 	/* �� */
				SPELL_CARD_00E_pache_000,	/* �p�`�F */
				SPELL_CARD_00E_sakuya_000,	/* ��� */
				SPELL_CARD_00E_pache_000,	/* �t����(?) */ 	/* �t�@���^�Y���p(boss7) */
			/* �{�X�X�y�J������ */
				(6),//(0-0),													/* �`���m(?) */ 	/* �G�L�X�g���p(boss0) */
				(5),//(SPELL_CARD_16E_alice_ggg-SPELL_CARD_00E_alice_000),		/* �A���X */		/*SPELL_CARD_19_alice_jjj;*/
				(5),//(SPELL_CARD_17E_mima_hhh-SPELL_CARD_00E_mima_000),		/* ���� */
				(6),//(SPELL_CARD_17E_kaguya_hhh-SPELL_CARD_00E_kaguya_000),	/* �P�� */
			//
				(6),//(SPELL_CARD_19E_aya_jjj-SPELL_CARD_00E_aya_000),			/* �� */
				(7),//(SPELL_CARD_17E_pache_hhh-SPELL_CARD_00E_pache_000),		/* �p�`�F */
				(9),//(SPELL_CARD_11E_sakuya_kkk-SPELL_CARD_00E_sakuya_000),	/* ��� */
				(5),//(0-0),													/* �t����(?) */ 	/* �t�@���^�Y���p(boss7) */
			//
			};
			spell_card_number				= aaa[sss  ]+(difficulty&0x03);
			spell_card_syoji_maisuu 		= aaa[sss+8];
		}
		/* spell_card common init */
	//	{
	//	//	spell_card_limit_health 	= 0;
	//	//	spell_card_mode 			= 0;/*off*/
	//	}
		{
			h->base_hp				= (my_spell_card_resource[(spell_card_number)].spell_life); 		/* �S�̗̂̑� */
			spell_card_limit_health = (h->base_hp) - (my_spell_card_resource[(spell_card_number + (4)/*1*/)].spell_life);		/* �ʏ�U��(����U��)�̍U���� */
			//
			spell_card_boss_timer	= (((my_spell_card_resource[(spell_card_number)].spell_limit_time)));	/* 75*64==75[count] 	��99[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
			spell_card_mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/	/* ����H */
			/*???*/
			my_spell_card_resource[(spell_card_number)].spell_move_callback = common_00_keitai;
		}
		#if 1
	//------------ �e���֘A
		h->boss_base_danmaku_type		= DANMAKU_00;	/* �e�������I���t���O */
		h->boss_base_danmaku_test		= 0;			/*(DANMAKU_08_rumia-1)*/ /*0*/
		#endif
	//
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
