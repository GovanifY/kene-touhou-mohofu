
#include "boss.h"


/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�X�y���J�[�h�Ǘ��V�X�e��
	-------------------------------------------------------
	�X�y���J�[�h�̓V�X�e���ŃT�|�[�g���Ă���̂ŁA�ǂ̃{�X���A�ǂ̃X�y���J�[�h�ł����Ă܂��B
	-------------------------------------------------------
	ToDo:
	�� �U���̊Ǘ�
	�� ����̊Ǘ�(���\����)
	�� �G��(���G��)���X�y���J�[�h�����Ă�@�\�́A�p�~���������ǂ������m��Ȃ��B
	����(���G�������G���X�y���J�[�h�@�\)��p�~���āA����ɓ����Ƀ{�X���o��������(�܂蒆-�{�X)
	�̕����A�V�X�e�����������肵�ėǂ��C������B��������ׂɂ͐F�X������������B
	����� dat �̋K�i���ƁA�����͏o���Ȃ��̂ŁA��� dat �̋K�i��ς��āA
	dat ����{�X�̃X�N���v�g���N������悤�ɂ���K�v������B
	�v���C���[�ɋ����ăX�N���v�g���Ⴄ�̂ŁA���̕ӂ̋K�i����͂܂�����B(����V�i���I�Ƃ�)
	-------------------------------------------------------
	�� �G��(���G��)���X�y���J�[�h�����Ă�@�\�́A�p�~���āA
	�G��(���G��)���X�y���J�[�h�����������ꍇ�́A�u�X�y���J�[�h�����Ă�U�R�v�Ƃ�������V�݂��Ă�����g��
	�V�X�e���ł̓T�|�[�g���Ȃ������ǂ��Ǝv���B�̂ł������������ŉ����\��B
	-------------------------------------------------------
---------------------------------------------------------*/

#include "kanji_system.h"

/*---------------------------------------------------------
	�{�X�ǉ�
---------------------------------------------------------*/

// �`���m EX1��
extern void cirno_02_keitai(SPRITE *src);
// ��� 6��
extern void sakuya_11_keitai(SPRITE *src);
extern void sakuya_10_keitai(SPRITE *src);
extern void sakuya_09_keitai(SPRITE *src);
extern void sakuya_08_keitai(SPRITE *src);
extern void sakuya_07_keitai(SPRITE *src);
extern void sakuya_06_keitai(SPRITE *src);
extern void sakuya_04_keitai(SPRITE *src);
extern void sakuya_03_keitai(SPRITE *src);
extern void sakuya_02_keitai(SPRITE *src);
//tern void sakuya_01_keitai(SPRITE *src);/* ���� */
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
//tern void aya_04_keitai(SPRITE *src);/* ���� */
//tern void aya_03_keitai(SPRITE *src);/* ���� */
//tern void aya_02_keitai(SPRITE *src);/* ���� */
//tern void aya_01_keitai(SPRITE *src);/* ���� */
extern void boss_init_aya(SPRITE *src);

// �P�� 3��
extern void add_zako_kaguya_houmotsu(SPRITE *src);
extern void add_zako_kaguya_dolls02(SPRITE *src);
extern void add_zako_kaguya_dolls01(SPRITE *src);
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
extern void boss_move_05_xy_douki_differential64(SPRITE *src);
extern void boss_move_04_xy_douki_differential32(SPRITE *src);
extern void boss_move_03_x_douki(SPRITE *src);
extern void boss_move_02_xy_hidouki(SPRITE *src);
extern void boss_move_01_not_move(SPRITE *src);


extern void common_00_keitai(SPRITE *src);
//extern void common_99_keitai(SPRITE *src);/* ���j��ɉ�ʊO�Ƀ{�X�������� */


/* ���ʕ� */
global void init_00_boss_clip000(SPRITE *h);/* �W���^�C�v */
global void init_00_boss_clip111(SPRITE *h);/* ��ɍL���^�C�v */




//------------ �X�y���J�[�h�֘A
#include "spell_card_value.h"
//
// int spell_card.limit_health; 	/* �K��l�ȉ��ɂȂ�΃X�y���J�[�h���[�h���� */
// int spell_card.boss_state;		/* ���l�ɂȂ�΃{�X�X�y���J�[�h���[�h�ɓ���Ȃ� */
// int spell_card.mode; 			/* �X�y���J�[�h���[�h */
// int spell_card.boss_timer;		/* ���p */	// ��������

// int spell_card.number;			/* ���p */	// �X�y���J�[�h�ԍ�

global SPELL_CARD_GLOBAL_CLASS spell_card;
/*global*/ static int spell_card_syoji_maisuu;				/* ���p */	// �X�y���J�[�h�ԍ��ő���E�l



typedef struct
{
	int spell_life; 							/* �X�y���J�[�h�ɓo�^���ꂽ���̗� */
	int spell_limit_time;						/* �X�y���J�[�h�̐������ԁB(�X�y���J�[�h�ɓo�^���ꂽ��莞��) */
//
	const char *spell_str_name; 				/* �X�y���J�[�h���� */
	int spell_type; 							/* �X�y���J�[�h�ɓo�^���ꂽ�e�� */
//
	void (*spell_init_callback)(SPRITE *sss);	/* �������ړ����� */
	void (*spell_move02_callback)(SPRITE *sss);	/* �X�y���J�[�h�ړ����� */
//	void (*spell_yuudou_callback)(SPRITE *sss);	/* �X�y���J�[�h�U���ړ����� */
//	void (*spell_tama_callback)(SPRITE *sss);	/* �X�y���J�[�h�e��ʊO����(�e���� / �e���� / ...) */
} SPELL_CARD_RESOURCE;

	/* ���O�̓e�L�g�[�ł� */

/* �X�y������(==�P��[�t���[��])�́A64�{(==�P��[��1�b])���K�i */
#define s_time(aaa) (((int)(aaa))<<6)

/* �{�X�̗͒P�ʂ́A1024�{��P�ʂɂ��Ă݂� */
#define s_hp(aaa) (((int)(aaa))<<10)

#if 0
	// 32�ŃQ�[�W1�{(r32)
	32768==1024*32==s_hp(32)
//
311296==32768*9.5
294912==32768*9.0
262144==32768*8.0
237568==32768*7.25
229376==32768*7.0
196608==32768*6.0
163840==32768*5.0
147456==32768*4.5
131072==32768*4.0
114688==32768*3.5
98304==32768*3.0
81920==32768*2.5
65536==32768*2.0
49152==32768*1.5
#endif

static SPELL_CARD_RESOURCE my_spell_card_resource[SPELL_CARD_MAX] =
{									// �ő唼�p��30�����B

// �`���m �G�L�X�g���X�e�[�W		"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
//	3.5
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_10_cirno, 					NULL,						boss_move_02_xy_hidouki,	},	/* "���`��: " */	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_10_cirno, 					NULL,						boss_move_02_xy_hidouki,	},	/* "���`��: " */	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_10_cirno, 					NULL,						boss_move_02_xy_hidouki,	},	/* "���`��: " */	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_10_cirno, 					NULL,						boss_move_02_xy_hidouki,	},	/* "���`��: " */	/* ����̍U����(�蓮�ݒ�)  500 160	*/
//	3.0
	{	s_hp(24),	s_time(20), 	"�@�@�X���u�A�C�V�N���t�H�[���v" "\n",	SPELL_1b_cirno_icecle_fall, 		NULL,						cirno_02_keitai,			},	/* "���`��: " */
	{	s_hp(24),	s_time(20), 	"�@�@�X���u�A�C�V�N���t�H�[���v" "\n",	SPELL_1b_cirno_icecle_fall, 		NULL,						cirno_02_keitai,			},	/* "���`��: " */
	{	s_hp(24),	s_time(20), 	"�@�@�X���u�A�C�V�N���t�H�[���v" "\n",	SPELL_1b_cirno_icecle_fall, 		NULL,						cirno_02_keitai,			},	/* "���`��: " */
	{	s_hp(24),	s_time(20), 	"�@�@�X���u�A�C�V�N���t�H�[���v" "\n",	SPELL_1b_cirno_icecle_fall, 		NULL,						cirno_02_keitai,			},	/* "���`��: " */
//	2.25
	{	 s_hp(8),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,	},	/* "��O�`��: " */
	{	 s_hp(8),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,	},	/* "��O�`��: " */
	{	 s_hp(8),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,	},	/* "��O�`��: " */
	{	 s_hp(8),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,	},	/* "��O�`��: " */
//	2.0
	{	s_hp(16),	s_time(20), 	"�@�����u�p�[�t�F�N�g�t���[�Y�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,	},	/* "��l�`��: " */
	{	s_hp(16),	s_time(20), 	"�@�����u�p�[�t�F�N�g�t���[�Y�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,	},	/* "��l�`��: " */
	{	s_hp(16),	s_time(20), 	"�@�����u�p�[�t�F�N�g�t���[�Y�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,	},	/* "��l�`��: " */
	{	s_hp(16),	s_time(20), 	"�@�����u�p�[�t�F�N�g�t���[�Y�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,	},	/* "��l�`��: " */
//	1.5
	{	s_hp(32),	s_time(20), 	"�ᕄ�u�_�C�A�����h�u���U�[�h�v" "\n",	SPELL_12_diamond_blizzard,			NULL,						boss_move_02_xy_hidouki,	},	/* "��܌`��: " */
	{	s_hp(32),	s_time(20), 	"�ᕄ�u�_�C�A�����h�u���U�[�h�v" "\n",	SPELL_12_diamond_blizzard,			NULL,						boss_move_02_xy_hidouki,	},	/* "��܌`��: " */
	{	s_hp(32),	s_time(20), 	"�ᕄ�u�_�C�A�����h�u���U�[�h�v" "\n",	SPELL_12_diamond_blizzard,			NULL,						boss_move_02_xy_hidouki,	},	/* "��܌`��: " */
	{	s_hp(32),	s_time(20), 	"�ᕄ�u�_�C�A�����h�u���U�[�h�v" "\n",	SPELL_12_diamond_blizzard,			NULL,						boss_move_02_xy_hidouki,	},	/* "��܌`��: " */
//	0.0
	// ���easy�͒Z����ɒi�K�����Ȃ��B
// ��� 6�� 						"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*4.5),	s_time(900),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{s_hp(32*6.5),	s_time(900),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{s_hp(32*6.5),	s_time(900),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{s_hp(32*6.5),	s_time(900),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
//	9.5
	{	s_hp(10),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_01_sakuya_misogi_normal,		boss_init_sakuya,			boss_move_04_xy_douki_differential32,	},	/* "���`��: �ʏ�U��(���E�ɓ������S�e����)"		*/	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	s_hp(12),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_01_sakuya_misogi_normal,		boss_init_sakuya,			boss_move_04_xy_douki_differential32,	},	/* "���`��: �ʏ�U��(���E�ɓ������S�e����)"		*/	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	s_hp(12),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_01_sakuya_misogi_normal,		boss_init_sakuya,			boss_move_04_xy_douki_differential32,	},	/* "���`��: �ʏ�U��(���E�ɓ������S�e����)"		*/	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	s_hp(12),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_2d_sakuya_misogi_lunatic, 	boss_init_sakuya,			boss_move_04_xy_douki_differential32,	},	/* "���`��: �ʏ�U��(���E�ɓ������S�e����)"		*/	/* ����̍U����(�蓮�ݒ�)  500 160	*/
//	9.0
	{	s_hp(10),	s_time(20), 	"�@�@��p�u�~�X�f�B���N�V�����v" "\n",	SPELL_0d_sakuya_miss_direction, 	NULL,						sakuya_02_keitai,			},	/* "���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)"	*/
	{	s_hp(16),	s_time(20), 	"�@�@��p�u�~�X�f�B���N�V�����v" "\n",	SPELL_0d_sakuya_miss_direction, 	NULL,						sakuya_02_keitai,			},	/* "���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)"	*/
	{	s_hp(16),	s_time(20), 	"��p�u���f�~�X�f�B���N�V�����v" "\n",	SPELL_0d_sakuya_miss_direction, 	NULL,						sakuya_02_keitai,			},	/* "���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)"	*/
	{	s_hp(16),	s_time(20), 	"��p�u���f�~�X�f�B���N�V�����v" "\n",	SPELL_0d_sakuya_miss_direction, 	NULL,						sakuya_02_keitai,			},	/* "���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)"	*/
//	8.25	// bloody==�����炯�́B staind �X�e�C���h==�X�e���h==(�F��)�Ă��t��(�Ďc��)��B
	{	 s_hp(8),	s_time(20), 	NULL,/* "�ʏ�U��3" "\n"*/				SPELL_2c_sakuya_blue_red,			NULL,						sakuya_03_keitai,			},	/* "��O�`��: " " �@�@�@�@���ہu�y�b�p�[�i�C�t�v"		*/
	{	 s_hp(8),	s_time(20), 	NULL,/* "�ʏ�U��3" "\n"*/				SPELL_2c_sakuya_blue_red,			NULL,						sakuya_03_keitai,			},	/* "��O�`��: " " �@�@�@�@���J�u�X�e���h�i�C�t�v"		*/
	{	 s_hp(8),	s_time(20), 	NULL,/* "�ʏ�U��3" "\n"*/				SPELL_2c_sakuya_blue_red,			NULL,						sakuya_03_keitai,			},	/* "��O�`��: " " �@�@�܌��J�u�u���b�f�B�i�C�t�v"		*/
	{	 s_hp(8),	s_time(20), 	NULL,/* "�ʏ�U��3" "\n"*/				SPELL_2c_sakuya_blue_red,			NULL,						sakuya_03_keitai,			},	/* "��O�`��: " " �@�@�����u�p�[�t�F�N�g�i�C�t�v"		*/
//	8.0
	{	s_hp(20),	s_time(20), 	NULL,									SPELL_2a_sakuya_baramaki1,			NULL,						sakuya_04_keitai,			},	/* "��l�`��: �����w����"	*/
	{	s_hp(32),	s_time(20), 	NULL,									SPELL_2a_sakuya_baramaki1,			NULL,						sakuya_04_keitai,			},	/* "��l�`��: �����w����"	*/
	{	s_hp(32),	s_time(20), 	NULL,									SPELL_2a_sakuya_baramaki1,			NULL,						sakuya_04_keitai,			},	/* "��l�`��: �����w����"	*/
	{	s_hp(32),	s_time(20), 	NULL,									SPELL_2a_sakuya_baramaki1,			NULL,						sakuya_04_keitai,			},	/* "��l�`��: �����w����"	*/
//	7.5 	// meek==�f���B
	{	s_hp(8),	s_time(20), 	"�@�@�@�@��p�u���e�X�gE005�v" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,	},	/* sakuya_06_keitai"��܌`��: (���F�}�X�J�b�g�eA)"	"�@�@����u�f���W�����X�~�[�N�v"	*/
	{	s_hp(12),	s_time(20), 	"�@�@�@�@��p�u���e�X�gN005�v" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,	},	/* sakuya_06_keitai"��܌`��: (���F�}�X�J�b�g�eA)"	"�@����u�f���W�����X���[���h�v"	*/
	{	s_hp(12),	s_time(20), 	"�@�@�@�@��p�u���e�X�gH005�v" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,	},	/* sakuya_06_keitai"��܌`��: (���F�}�X�J�b�g�eA)"	"�@�@���ہu�f���W�����X�^�C���v"	*/
	{	s_hp(12),	s_time(20), 	"�@�@�@�@��p�u���e�X�gL005�v" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,	},	/* sakuya_06_keitai"��܌`��: (���F�}�X�J�b�g�eA)"	"�@�����u�f���W�����X�X�g�[���v"	*/
	//
	{	s_hp(8),	s_time(20), 	"�@�@�@�@��p�u���e�X�gE006�v" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,	},	/* sakuya_06_keitai"��Z�`��: (���F�}�X�J�b�g�eB)"					*/
	{	s_hp(12),	s_time(20), 	"�@�@�@�@��p�u���e�X�gN006�v" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,	},	/* sakuya_06_keitai"��Z�`��: (���F�}�X�J�b�g�eB)"					*/
	{	s_hp(12),	s_time(20), 	"�@�@�@�@��p�u���e�X�gH006�v" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,	},	/* sakuya_06_keitai"��Z�`��: (���F�}�X�J�b�g�eB)"					*/
	{	s_hp(12),	s_time(20), 	"�@�@�@�@��p�u���e�X�gL006�v" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,	},	/* sakuya_06_keitai"��Z�`��: (���F�}�X�J�b�g�eB)"					*/
//	6.5 	// �W���b�N�E�U�E���b�p�[==Jack the Ripper==�؂�􂫃W���b�N(�E�l�S)19���I(1888�N)�Ɏ���(?)/���ۖ������B���[���h�q�[���[�Y�B ���b�N==�X�v���b�^�[�n�E�X
	{	s_hp(16),	s_time(20), 	"�@��u�W���b�N�E�K�[���b�N�v" "\n",	SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,	},	/* "�掵�`��: (���U�����w)�ǉ��v�撆"				*/
	{	s_hp(24),	s_time(20), 	"��p�u�W���b�N�E�U�E���b�p�[�v" "\n",	SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,	},	/* "�掵�`��: (���U�����w)�ǉ��v�撆"				*/
	{	s_hp(24),	s_time(20), 	"��p�u�W���b�N�E�U�E�r�[���Y�v" "\n",	SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,	},	/* "�掵�`��: (���U�����w)�ǉ��v�撆"				*/
	{	s_hp(24),	s_time(20), 	"����u�W���b�N�E�f�E�}���}�L�v" "\n",	SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,	},	/* "�掵�`��: (���U�����w)�ǉ��v�撆"				*/
//
//	{	s_hp(16),	s_time(20), 	"�@�@�@�@��p�u���e�X�gE008�v" "\n",	SPELL_00,							NULL,						common_00_keitai,			},	/* "�攪�`��: (���~�߃i�C�t)�ǉ��v�撆" 		*/
//	{	s_hp(24),	s_time(20), 	"�@�@�@�@��p�u���e�X�gN008�v" "\n",	SPELL_00,							NULL,						common_00_keitai,			},	/* "�攪�`��: (���~�߃i�C�t)�ǉ��v�撆" 		*/
//	{	s_hp(24),	s_time(20), 	"�@�@�@�@��p�u���e�X�gH008�v" "\n",	SPELL_00,							NULL,						common_00_keitai,			},	/* "�攪�`��: (���~�߃i�C�t)�ǉ��v�撆" 		*/
//	{	s_hp(24),	s_time(20), 	"�@�@�@�@��p�u���e�X�gL008�v" "\n",	SPELL_00,							NULL,						common_00_keitai,			},	/* "�攪�`��: (���~�߃i�C�t)�ǉ��v�撆" 		*/
	{	s_hp(16),	s_time(20), 	NULL,									SPELL_2b_sakuya_baramaki2,			NULL,						sakuya_08_keitai,			},	/* "��l�`��: �����w����"	*/
	{	s_hp(24),	s_time(20), 	NULL,									SPELL_2b_sakuya_baramaki2,			NULL,						sakuya_08_keitai,			},	/* "��l�`��: �����w����"	*/
	{	s_hp(24),	s_time(20), 	NULL,									SPELL_2b_sakuya_baramaki2,			NULL,						sakuya_08_keitai,			},	/* "��l�`��: �����w����"	*/
	{	s_hp(24),	s_time(20), 	NULL,									SPELL_2b_sakuya_baramaki2,			NULL,						sakuya_08_keitai,			},	/* "��l�`��: �����w����"	*/
//	4.5
	{	s_hp(16),	s_time(20), 	"�@���z�u�t�F�X�e�B�o���i�C�t�v" "\n",	SPELL_00,							NULL,						sakuya_09_keitai,			},	/* "���`��: �ŏI�`��(����1)"					*/
	{	s_hp(24),	s_time(20), 	"�@�����u�t�F�X�e�B�o���i�C�t�v" "\n",	SPELL_00,							NULL,						sakuya_09_keitai,			},	/* "���`��: �ŏI�`��(����1)"					*/
	{	s_hp(24),	s_time(20), 	"�@���I�u�t�F�X�e�B�o���i�C�t�v" "\n",	SPELL_00,							NULL,						sakuya_09_keitai,			},	/* "���`��: �ŏI�`��(����1)"					*/
	{	s_hp(24),	s_time(20), 	"�@���f�u�t�F�X�e�B�o���i�C�t�v" "\n",	SPELL_00,							NULL,						sakuya_09_keitai,			},	/* "���`��: �ŏI�`��(����1)"					*/
//	2.5
	{	s_hp(20),	s_time(20), 	NULL,									SPELL_0e_remilia_00,				NULL,						sakuya_10_keitai,			},	/* "��10�`��: �ŏI�`��(����2)"	*/
	{	s_hp(24),	s_time(20), 	NULL,									SPELL_0e_remilia_00,				NULL,						sakuya_10_keitai,			},	/* "��10�`��: �ŏI�`��(����2)"	*/
	{	s_hp(24),	s_time(20), 	NULL,									SPELL_0e_remilia_00,				NULL,						sakuya_10_keitai,			},	/* "��10�`��: �ŏI�`��(����2)"	*/
	{	s_hp(24),	s_time(20), 	NULL,									SPELL_0e_remilia_00,				NULL,						sakuya_10_keitai,			},	/* "��10�`��: �ŏI�`��(����2)"	*/
//	0.5
	{	 s_hp(8),	s_time(20), 	"�@�@���C�h�閧�u�c�s�s�׎蓖�v" "\n",	SPELL_47_sakuya_meek,				NULL,						sakuya_11_keitai,			},	/* "��11�`��: �ŏI�`��(����3)"					*/	// (easy)���͂����ɂ͂��Ȃ�
	{	 s_hp(8),	s_time(20), 	"�@�@���C�h��Z�u�́I�́I�́I�v" "\n",	SPELL_47_sakuya_meek,				NULL,						sakuya_11_keitai,			},	/* "��11�`��: �ŏI�`��(����3)"					*/
	{	 s_hp(8),	s_time(20), 	"�@���C�h���u�����Ď��ʂ��v" "\n",	SPELL_47_sakuya_meek,				NULL,						sakuya_11_keitai,			},	/* "��11�`��: �ŏI�`��(����3)"					*/
	{	 s_hp(8),	s_time(20), 	"�@�@���C�h���X�u���������{�X�v" "\n",	SPELL_47_sakuya_meek,				NULL,						sakuya_11_keitai,			},	/* "��11�`��: �ŏI�`��(����3)"					*/
//	0.25

	// �p�`�Feasy�͒Z�����1�i�K���Ȃ��B
// A(�아)
// �p�`�FA 5��						"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*2.00), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
//	5.0
	{	s_hp(10),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki/*boss_move_01_not_move*/,	},/*boss_init_pache*/
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki/*boss_move_01_not_move*/,	},/*boss_init_pache*/
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki/*boss_move_01_not_move*/,	},/*boss_init_pache*/
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki/*boss_move_01_not_move*/,	},/*boss_init_pache*/
//	4.0
	{	s_hp(10),	s_time(30), 	"�@�@�@�@�Ε��u�A�O�j�V���C���v" "\n",	SPELL_2e_pache_agni_shine_1,		add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�@�@�@�@�Ε��u�A�O�j�V���C���v" "\n",	SPELL_2e_pache_agni_shine_1,		add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�@�@�Ε��u�A�O�j�V���C���㋉�v" "\n",	SPELL_33_pache_agni_shine_2,		add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�@�@�Ε��u�A�O�j�V���C���㋉�v" "\n",	SPELL_33_pache_agni_shine_2,		add_laser_off,				pache_01_keitai,			},
//	3.0
	{	s_hp(10),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,	},
//	2.0
	{	s_hp(10),	s_time(30), 	"�@�@�y���u���C�W�B�g�����g���v" "\n",	SPELL_31_pache_rage_tririton_1, 	add_laser_off,				/*boss_move_01_not_move*/pache_01_keitai/*pache_01_keitai*/,			},/*(�e�X�g)*/
	{	s_hp(16),	s_time(30), 	"�y���u���C�W�B�g�����g���㋉�v" "\n",	SPELL_35_pache_rage_tririton_2, 	add_laser_off,				/*boss_move_01_not_move*/pache_01_keitai/*pache_01_keitai*/,			},/*(�e�X�g)*/
	{	s_hp(16),	s_time(30), 	"�@�@�y���u�g�����g���V�F�C�N�v" "\n",	SPELL_39_pache_tririton_shake,		add_laser_off,				/*boss_move_01_not_move*/pache_01_keitai/*pache_01_keitai*/,			},/*(�e�X�g)*/
	{	s_hp(16),	s_time(30), 	"�@�@�y���u�g�����g���V�F�C�N�v" "\n",	SPELL_39_pache_tririton_shake,		add_laser_off,				/*boss_move_01_not_move*/pache_01_keitai/*pache_01_keitai*/,			},/*(�e�X�g)*/
//	1.0
	{	s_hp(10),	s_time(30), 	"�΁��y���u���[���@�N�������N�v" "\n",	SPELL_3b_pache_lava_cromlech,		add_zako_pache_dolls,		pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"�΁��y���u���[���@�N�������N�v" "\n",	SPELL_3b_pache_lava_cromlech,		add_zako_pache_dolls,		pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"�΁��y���u���[���@�N�������N�v" "\n",	SPELL_3b_pache_lava_cromlech,		add_zako_pache_dolls,		pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"�΁��y���u���[���@�N�������N�v" "\n",	SPELL_3b_pache_lava_cromlech,		add_zako_pache_dolls,		pache_04_keitai,			},
//	0.0
	{	s_hp(10),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		add_zako_pache_dolls,		pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		add_zako_pache_dolls,		pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		add_zako_pache_dolls,		pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		add_zako_pache_dolls,		pache_04_keitai,			},
//	0.0
	{	s_hp(10),	s_time(30), 	"�؁��Ε��u�t�H���X�g�u���C�Y�v" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						pache_04_keitai,			},	// (easy)���͂����ɂ͂��Ȃ�
	{	s_hp(16),	s_time(30), 	"�؁��Ε��u�t�H���X�g�u���C�Y�v" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"�؁��Ε��u�t�H���X�g�u���C�Y�v" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"�؁��Ε��u�t�H���X�g�u���C�Y�v" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						pache_04_keitai,			},

// B(����)
// �p�`�FB 5��						"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*2.00), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
//	5.0
	{	s_hp(10),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,	},/*boss_init_pache*/
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,	},/*boss_init_pache*/
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,	},/*boss_init_pache*/
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,	},/*boss_init_pache*/
//	4.0
	{	s_hp(10),	s_time(30), 	"�@�����u�v�����Z�X�E���f�B�l�v" "\n",	SPELL_2f_pache_princess_undine, 	add_laser_off,				boss_move_04_xy_douki_differential32,		},
	{	s_hp(16),	s_time(30), 	"�@�����u�v�����Z�X�E���f�B�l�v" "\n",	SPELL_2f_pache_princess_undine, 	add_laser_off,				boss_move_04_xy_douki_differential32,		},
	{	s_hp(16),	s_time(30), 	"�@�@�@�����u�x���[�C�����C�N�v" "\n",	SPELL_37_pache_bury_in_lake,		add_laser_off,				boss_move_04_xy_douki_differential32,		},
	{	s_hp(16),	s_time(30), 	"�@�@�@�����u�x���[�C�����C�N�v" "\n",	SPELL_37_pache_bury_in_lake,		add_laser_off,				boss_move_04_xy_douki_differential32,		},
//	3.0
	{	s_hp(10),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,	},
//	2.0
	{	s_hp(10),	s_time(30), 	"�@�@�ؕ��u�V���t�B�z�����㋉�v" "\n",	SPELL_34_pache_sylphy_horn_2,		add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�@�@�ؕ��u�V���t�B�z�����㋉�v" "\n",	SPELL_34_pache_sylphy_horn_2,		add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�@�@�@�ؕ��u�O���[���X�g�[���v" "\n",	SPELL_38_pache_green_storm, 		add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�@�@�@�ؕ��u�O���[���X�g�[���v" "\n",	SPELL_38_pache_green_storm, 		add_laser_off,				pache_01_keitai,			},
//	1.0
	{	s_hp(10),	s_time(30), 	"�@�����ؕ��u�E�H�[�^�[�G���t�v" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	"�@�����ؕ��u�E�H�[�^�[�G���t�v" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	"�@�����ؕ��u�E�H�[�^�[�G���t�v" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	"�@�����ؕ��u�E�H�[�^�[�G���t�v" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,	},
//	0.0
	{	s_hp(10),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		add_zako_pache_dolls,		pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		add_zako_pache_dolls,		pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		add_zako_pache_dolls,		pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		add_zako_pache_dolls,		pache_04_keitai,			},
//	0.0
	{	s_hp(10),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,	},	// (easy)���͂����ɂ͂��Ȃ�
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,	},

// C(����)
// �p�`�FC 5��						"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*2.00), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
//	5.0
	{	s_hp(10),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,	},/*boss_init_pache*/
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,	},/*boss_init_pache*/
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,	},/*boss_init_pache*/
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,	},/*boss_init_pache*/
//	4.0
	{	s_hp(10),	s_time(30), 	"�@�@�@�@�ؕ��u�V���t�B�z�����v" "\n",	SPELL_30_pache_sylphy_horn_1,		add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�@�@�@�@�ؕ��u�V���t�B�z�����v" "\n",	SPELL_30_pache_sylphy_horn_1,		add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�@�@�ؕ��u�V���t�B�z�����㋉�v" "\n",	SPELL_34_pache_sylphy_horn_2,		add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�@�@�ؕ��u�V���t�B�z�����㋉�v" "\n",	SPELL_34_pache_sylphy_horn_2,		add_laser_off,				pache_01_keitai,			},
//	3.0
	{	s_hp(10),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,	},
//	2.0
	{	s_hp(10),	s_time(30), 	"�@�@�@�@�Ε��u�A�O�j�V���C���v" "\n",	SPELL_2e_pache_agni_shine_1,		add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�@�@�Ε��u�A�O�j�V���C���㋉�v" "\n",	SPELL_33_pache_agni_shine_2,		add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�@�Ε��u�A�O�j���C�f�B�A���X�v" "\n",	SPELL_36_pache_agni_radiance,		add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�@�Ε��u�A�O�j���C�f�B�A���X�v" "\n",	SPELL_36_pache_agni_radiance,		add_laser_off,				pache_01_keitai,			},
//	1.0
	{	s_hp(10),	s_time(30), 	"�؁��Ε��u�t�H���X�g�u���C�Y�v" "\n",	SPELL_3c_pache_forest_blaze,		add_zako_pache_dolls,		pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"�؁��Ε��u�t�H���X�g�u���C�Y�v" "\n",	SPELL_3c_pache_forest_blaze,		add_zako_pache_dolls,		pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"�؁��Ε��u�t�H���X�g�u���C�Y�v" "\n",	SPELL_3c_pache_forest_blaze,		add_zako_pache_dolls,		pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"�؁��Ε��u�t�H���X�g�u���C�Y�v" "\n",	SPELL_3c_pache_forest_blaze,		add_zako_pache_dolls,		pache_04_keitai,			},
//	0.0
	{	s_hp(10),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	add_zako_pache_dolls,		boss_move_02_xy_hidouki,			},
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	add_zako_pache_dolls,		boss_move_02_xy_hidouki,			},
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	add_zako_pache_dolls,		boss_move_02_xy_hidouki,			},
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	add_zako_pache_dolls,		boss_move_02_xy_hidouki,			},
//	0.0
	{	s_hp(10),	s_time(30), 	"�΁��y���u���[���@�N�������N�v" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						pache_04_keitai,			},	// (easy)���͂����ɂ͂��Ȃ�
	{	s_hp(16),	s_time(30), 	"�΁��y���u���[���@�N�������N�v" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"�΁��y���u���[���@�N�������N�v" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"�΁��y���u���[���@�N�������N�v" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						pache_04_keitai,			},

// D(����)
// �p�`�FD 5��						"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*2.00), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
//	5.0
	{	s_hp(10),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,	},/*boss_init_pache*/
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,	},/*boss_init_pache*/
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,	},/*boss_init_pache*/
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,	},/*boss_init_pache*/
//	4.0
	{	s_hp(10),	s_time(30), 	"�@�@�y���u���C�W�B�g�����g���v" "\n",	SPELL_00,							add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�@�@�y���u���C�W�B�g�����g���v" "\n",	SPELL_00,							add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�y���u���C�W�B�g�����g���㋉�v" "\n",	SPELL_00,							add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�y���u���C�W�B�g�����g���㋉�v" "\n",	SPELL_00,							add_laser_off,				pache_01_keitai,			},
//	3.0
	{	s_hp(10),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,	},
//	2.0
	{	s_hp(10),	s_time(30), 	"�@�@�����u���^���t�@�e�B�[�O�v" "\n",	SPELL_00,							add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�@�@�����u���^���t�@�e�B�[�O�v" "\n",	SPELL_00,							add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�@�@�@�����u�V���o�[�h���S���v" "\n",	SPELL_00,							add_laser_off,				pache_01_keitai,			},
	{	s_hp(16),	s_time(30), 	"�@�@�@�����u�V���o�[�h���S���v" "\n",	SPELL_00,							add_laser_off,				pache_01_keitai,			},
//	1.0
	{	s_hp(10),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,	},
//	0.0
	{	s_hp(10),	s_time(30), 	"�@�����ؕ��u�E�H�[�^�[�G���t�v" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	"�@�����ؕ��u�E�H�[�^�[�G���t�v" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	"�@�����ؕ��u�E�H�[�^�[�G���t�v" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,	},
	{	s_hp(16),	s_time(30), 	"�@�����ؕ��u�E�H�[�^�[�G���t�v" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,	},
//	0.0
	{	s_hp(10),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_00,							NULL,						pache_04_keitai,			},	// (easy)���͂����ɂ͂��Ȃ�
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_00,							NULL,						pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_00,							NULL,						pache_04_keitai,			},
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_00,							NULL,						pache_04_keitai,			},

//	{	s_hp(10),	s_time(99), 	" �@�����u�T�C�����g���쐬E06�v" "\n",	SPELL_00,							NULL,						pache_04_keitai,			},
//	{	s_hp(16),	s_time(99), 	" �@�����u�T�C�����g���쐬N06�v" "\n",	SPELL_00,							NULL,						pache_04_keitai,			},
//	{	s_hp(16),	s_time(99), 	" �@�����u�T�C�����g���쐬H06�v" "\n",	SPELL_00,							NULL,						pache_04_keitai,			},
//	{	s_hp(16),	s_time(99), 	" �@�����u�T�C�����g���쐬L06�v" "\n",	SPELL_00,							NULL,						pache_04_keitai,			},

//	{	s_hp(10),	s_time(99), 	" �@�X���u�p�`�����[���쐬E07�v" "\n",	SPELL_00,							NULL,						pache_04_keitai,			},
//	{	s_hp(16),	s_time(99), 	" �@�X���u�p�`�����[���쐬N07�v" "\n",	SPELL_00,							NULL,						pache_04_keitai,			},
//	{	s_hp(16),	s_time(99), 	" �@�X���u�p�`�����[���쐬H07�v" "\n",	SPELL_00,							NULL,						pache_04_keitai,			},
//	{	s_hp(16),	s_time(99), 	" �@�X���u�p�`�����[���쐬L07�v" "\n",	SPELL_00,							NULL,						pache_04_keitai,			},

//	spell_card.spell_type	= SPELL_15_aya_misogi;		/* �e�����Z�b�g */
//	spell_card.spell_type	= SPELL_25_houka_kenran;		/* �e�����Z�b�g */	/*aya_02_keitai*/
//	spell_card.spell_type	= SPELL_23_aya_merin_test;	/* �e�����Z�b�g */	/*aya_01_keitai*/

// �� 4��							"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	//																											/*�{�X�o��O�̏�����[��b�̑O]*/
	{s_hp(32*3),	s_time(600),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							NULL,						common_00_keitai,			},/* 32768=32768.0 */
	{s_hp(32*3),	s_time(600),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							NULL,						common_00_keitai,			},/* 32768=32768.0 */
	{s_hp(32*3),	s_time(600),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							NULL,						common_00_keitai,			},/* 32768=32768.0 */
	{s_hp(32*3),	s_time(600),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							NULL,						common_00_keitai,			},/* 32768=32768.0 */
	//	4.0 																									/*�{�X�퓬�O�̏�����[��b�̌�]*/
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_20_aya_misogi1,				init_00_boss_clip000,		boss_move_03_x_douki,		},/*(4)*/
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_20_aya_misogi1,				init_00_boss_clip000,		boss_move_03_x_douki,		},/*(4)*/
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_20_aya_misogi1,				init_00_boss_clip000,		boss_move_03_x_douki,		},/*(4)*/
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_20_aya_misogi1,				init_00_boss_clip000,		boss_move_03_x_douki,		},/*(4)*/
	//	3.5
	{	s_hp(8),	s_time(30), 	"�@�@�@�@�@�@�@�򕄁u�V�̔��ˁv" "\n",	SPELL_22_ame_no_yatimata,			init_00_boss_clip111,		boss_move_01_not_move,		},/*(4)*/
	{	s_hp(8),	s_time(30), 	"�@�@�@�@�@�@�@�򕄁u�V�̔��ˁv" "\n",	SPELL_22_ame_no_yatimata,			init_00_boss_clip111,		boss_move_01_not_move,		},/*(4)*/
	{	s_hp(8),	s_time(30), 	"�@�@�@�@�@�@�@�򕄁u�V�̔��ˁv" "\n",	SPELL_22_ame_no_yatimata,			init_00_boss_clip111,		boss_move_01_not_move,		},/*(4)*/
	{	s_hp(8),	s_time(30), 	"�@�@�@�@�@�@�@�򕄁u�V�̔��ˁv" "\n",	SPELL_22_ame_no_yatimata,			init_00_boss_clip111,		boss_move_01_not_move,		},/*(4)*/
	//	3.0
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_21_aya_misogi2,				init_00_boss_clip000,		boss_move_03_x_douki,		},/*(5)"�򕄁u���쐬�X�y���J�[�h001�v"*/
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_21_aya_misogi2,				init_00_boss_clip000,		boss_move_03_x_douki,		},/*(5)"�򕄁u���쐬�X�y���J�[�h001�v"*/
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_21_aya_misogi2,				init_00_boss_clip000,		boss_move_03_x_douki,		},/*(5)"�򕄁u���쐬�X�y���J�[�h001�v"*/
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_21_aya_misogi2,				init_00_boss_clip000,		boss_move_03_x_douki,		},/*(5)"�򕄁u���쐬�X�y���J�[�h001�v"*/
	//	2.5
	{	 s_hp(4),	s_time(40), 	"�@�@�@�@�@�@�E���u�n����D�]�v" "\n",	SPELL_24_aya_doll,					NULL,						boss_move_03_x_douki,		},/*(4)"�򕄁u���쐬�X�y���J�[�h002�v"*/
	{	 s_hp(4),	s_time(40), 	"�@�@�@�@�@�����u�n���卪��Q�v" "\n",	SPELL_24_aya_doll,					NULL,						boss_move_03_x_douki,		},/*(4)"�򕄁u���쐬�X�y���J�[�h002�v"*/
	{	 s_hp(4),	s_time(40), 	"�@�@�@�@�@�@�ߏ�u�R�����Q�v" "\n",	SPELL_24_aya_doll,					NULL,						boss_move_03_x_douki,		},/*(4)"�򕄁u���쐬�X�y���J�[�h002�v"*/
	{	 s_hp(4),	s_time(40), 	"�@�@�@�@�@��ʁu�����\\���ۉ߁v" "\n", SPELL_24_aya_doll,					NULL,						boss_move_03_x_douki,		},/*(4)"�򕄁u���쐬�X�y���J�[�h002�v"*/
	//	2.0
	{	s_hp(16),	s_time(100),	"�@�@�@�@�@�@�@�����u�����R�l�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,				},/*(5)"�򕄁u���쐬�X�y���J�[�h003�v"*/
	{	s_hp(16),	s_time(100),	"�@�@�@�@�@�@�����u�����R�R�l�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,				},/*(5)"�򕄁u���쐬�X�y���J�[�h003�v"*/
	{	s_hp(16),	s_time(100),	"�@�@�@�@�@�@�����u�����X�W�l�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,				},/*(5)"�򕄁u���쐬�X�y���J�[�h003�v"*/
	{	s_hp(16),	s_time(100),	"�@�@�@�@�@�@�����u�������蒆�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,				},/*(5)"�򕄁u���쐬�X�y���J�[�h003�v"*/
	//	1.5
	{	s_hp(8),	s_time(99), 	"�@�@�@�@�@�@�H���u�����̒��ׁv" "\n",	SPELL_1d_amefuri_test,				init_00_boss_clip111,		boss_move_01_not_move,		},/*(4)"�򕄁u���쐬�X�y���J�[�h004�v"*/
	{	s_hp(8),	s_time(99), 	"�@�@�@�@�@�@�H���u����̒��ׁv" "\n",	SPELL_1d_amefuri_test,				init_00_boss_clip111,		boss_move_01_not_move,		},/*(4)"�򕄁u���쐬�X�y���J�[�h004�v"*/
	{	s_hp(8),	s_time(99), 	"�@�@�@�@�@�@�H���u�Ӌ|�̒��ׁv" "\n",	SPELL_1d_amefuri_test,				init_00_boss_clip111,		boss_move_01_not_move,		},/*(4)"�򕄁u���쐬�X�y���J�[�h004�v"*/
	{	s_hp(8),	s_time(99), 	"�@�@�@�@�ǐՁu�R�����{�̒��ׁv" "\n",	SPELL_1d_amefuri_test,				init_00_boss_clip111,		boss_move_01_not_move,		},/*(4)"�򕄁u���쐬�X�y���J�[�h004�v"*/
	//	1.0
	{	s_hp(8),	s_time(99), 	"�@�@�ؕ��u���남�ڂ��F�؈�ࣁv" "\n",	SPELL_25_houka_kenran,				init_00_boss_clip000,		boss_move_01_not_move,		},/*(5)*/
	{	s_hp(8),	s_time(99), 	"�@�@�@�@�@�@�@�Ǖ��u�R�_�n��v" "\n",	SPELL_26_aya_saifu, 				init_00_boss_clip000,		boss_move_01_not_move,		},/*(5)*/
	{	s_hp(8),	s_time(99), 	"�@�@�@�@�Ǖ��u�V��V���̏ƚ��v" "\n",	SPELL_26_aya_saifu, 				init_00_boss_clip000,		boss_move_01_not_move,		},/*(5)*/
	{	s_hp(8),	s_time(99), 	"�@�@�@�@�Ǖ��u�B��Ƒ��̏ƚ��v" "\n",	SPELL_26_aya_saifu, 				init_00_boss_clip000,		boss_move_01_not_move,		},/*(5)*/
	//	0.5
//	{	s_hp(16),	s_time(99), 	"�@�@�@�򕄁u���쐬�X�y�JE006�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,				},/*(5)*/
//	{	s_hp(16),	s_time(99), 	"�@�@�@�򕄁u���쐬�X�y�JN006�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,				},/*(5)*/
//	{	s_hp(16),	s_time(99), 	"�@�@�@�򕄁u���쐬�X�y�JH006�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,				},/*(5)*/
//	{	s_hp(16),	s_time(99), 	"�@�@�@�򕄁u���쐬�X�y�JL006�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,				},/*(5)*/
	//	0.0
//	{	s_hp(64),	s_time(99), 	"�@�@�@�򕄁u���쐬�X�y�JE007�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,				},/*(5)*/
//	{	s_hp(64),	s_time(99), 	"�@�@�@�򕄁u���쐬�X�y�JN007�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,				},/*(5)*/
//	{	s_hp(64),	s_time(99), 	"�@�@�@�򕄁u���쐬�X�y�JH007�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,				},/*(5)*/
//	{	s_hp(64),	s_time(99), 	"�@�@�@�򕄁u���쐬�X�y�JL007�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,				},/*(5)*/

//	spell_card.spell_type	= SPELL_1e_kaguya01;		/* �e�����Z�b�g */	�{�X�s���A�� 1�`��
//	spell_card.spell_type	= SPELL_1f_kaguya04;		/* �e�����Z�b�g */	�{�X�s���A�� 4�`��
	// 3�ʃ{�X�Aeasy�͂��������ȒP�ɁB���͂���Ȃ�ɁB
// �P�� 3�� 						"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*2.5),	s_time(500),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 28672==8192*3.5 */
	{s_hp(32*3.5),	s_time(500),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 28672==8192*3.5 */
	{s_hp(32*3.5),	s_time(500),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 28672==8192*3.5 */
	{s_hp(32*3.5),	s_time(500),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 28672==8192*3.5 */
	// 3.5
	{	s_hp(10),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_1e_kaguya01,					boss_init_kaguya,			kaguya_01_keitai,			}, // ����(5)�҂�e�A�ԁA�B���@�_���A�e(8)��18�Z�b�g(�b)���炢(�n�߂�5�b�͏o���Ȃ�)�B(�P����Ԋu�ňړ��A�I���㒆���ֈړ�)
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_1e_kaguya01,					boss_init_kaguya,			kaguya_01_keitai,			}, // ����(5)�҂�e�A�ԁA�B���@�_���A�e(8)��18�Z�b�g(�b)���炢(�n�߂�5�b�͏o���Ȃ�)�B(�P����Ԋu�ňړ��A�I���㒆���ֈړ�)
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_1e_kaguya01,					boss_init_kaguya,			kaguya_01_keitai,			}, // ����(5)�҂�e�A�ԁA�B���@�_���A�e(8)��18�Z�b�g(�b)���炢(�n�߂�5�b�͏o���Ȃ�)�B(�P����Ԋu�ňړ��A�I���㒆���ֈړ�)
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_1e_kaguya01,					boss_init_kaguya,			kaguya_01_keitai,			}, // ����(5)�҂�e�A�ԁA�B���@�_���A�e(8)��18�Z�b�g(�b)���炢(�n�߂�5�b�͏o���Ȃ�)�B(�P����Ԋu�ňړ��A�I���㒆���ֈړ�)
	// 3.0
	{	s_hp(10),	s_time(40), 	" �@�@�@�@�@�@���u���쐬E01�v" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 1.������̋�	(�P�钆���Œ�A�Ԏ��Ή��̊ےe�A�Z���[�U�[)4�Z�b�gx
	{	s_hp(16),	s_time(40), 	" �@�@�@�@�@�@���u���쐬N01�v" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 1.������̋�	(�P�钆���Œ�A�Ԏ��Ή��̊ےe�A�Z���[�U�[)4�Z�b�gx
	{	s_hp(16),	s_time(40), 	" �@�@�@�@�@�@���u���쐬H01�v" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 1.������̋�	(�P�钆���Œ�A�Ԏ��Ή��̊ےe�A�Z���[�U�[)4�Z�b�gx
	{	s_hp(16),	s_time(40), 	" �@�@�@�@�@�@���u���쐬L01�v" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 1.������̋�	(�P�钆���Œ�A�Ԏ��Ή��̊ےe�A�Z���[�U�[)4�Z�b�gx
	// 2.5
	{	s_hp(10),	s_time(40), 	" �@�@�@�@�@�@���u���쐬E02�v" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 2.���̌�΂̔�
	{	s_hp(16),	s_time(40), 	" �@�@�@�@�@�@���u���쐬N02�v" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 2.���̌�΂̔�
	{	s_hp(16),	s_time(40), 	" �@�@�@�@�@�@���u���쐬H02�v" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 2.���̌�΂̔�
	{	s_hp(16),	s_time(40), 	" �@�@�@�@�@�@���u���쐬L02�v" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 2.���̌�΂̔�
	// 2.0
	{	s_hp(10),	s_time(40), 	" �@�@�@�@�@�@���u���쐬E03�v" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,			},// 3.�Αl�̔��
	{	s_hp(16),	s_time(40), 	" �@�@�@�@�@�@���u���쐬N03�v" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,			},// 3.�Αl�̔��
	{	s_hp(16),	s_time(40), 	" �@�@�@�@�@�@���u���쐬H03�v" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,			},// 3.�Αl�̔��
	{	s_hp(16),	s_time(40), 	" �@�@�@�@�@�@���u���쐬L03�v" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,			},// 3.�Αl�̔��
	// 1.5
	{	s_hp(10),	s_time(40), 	" �@�@�@�@�@�@���u���쐬E04�v" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,			},// 4.���̎q���L
	{	s_hp(16),	s_time(40), 	" �@�@�@�@�@�@���u���쐬N04�v" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,			},// 4.���̎q���L
	{	s_hp(16),	s_time(40), 	" �@�@�@�@�@�@���u���쐬H04�v" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,			},// 4.���̎q���L
	{	s_hp(16),	s_time(40), 	" �@�@�@�@�@�@���u���쐬L04�v" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,			},// 4.���̎q���L
	// 1.0
	{	s_hp(10),	s_time(40), 	" �@�@�@�@�@�@���u���쐬E05�v" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,		},// 5.�H���̋ʂ̎}
	{	s_hp(16),	s_time(40), 	" �@�@�@�@�@�@���u���쐬N05�v" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,		},// 5.�H���̋ʂ̎}
	{	s_hp(16),	s_time(40), 	" �@�@�@�@�@�@���u���쐬H05�v" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,		},// 5.�H���̋ʂ̎}
	{	s_hp(16),	s_time(40), 	" �@�@�@�@�@�@���u���쐬L05�v" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,		},// 5.�H���̋ʂ̎}
	// 0.5
	{	s_hp(10),	s_time(40), 	" �@�@�@���u�H���̋ʂ̎}E06�v" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,		},//
	{	s_hp(16),	s_time(40), 	" �@�@�@���u�H���̋ʂ̎}N06�v" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,		},//
	{	s_hp(16),	s_time(40), 	" �@�@�@���u�H���̋ʂ̎}H06�v" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,		},//
	{	s_hp(16),	s_time(40), 	" �@�@�@���u�H���̋ʂ̎}L06�v" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,		},//
	// 2�ʃ{�X�Ȃ̂ŁA�K���ɈՂ����B
// ���� 2�� 						"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*1.4),	s_time(400),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 20480==8192*2.5 */
	{s_hp(32*2),	s_time(400),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 20480==8192*2.5 */
	{s_hp(32*2),	s_time(400),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 20480==8192*2.5 */
	{s_hp(32*2),	s_time(400),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 20480==8192*2.5 */
	// 2.000 (12==32*0.375) // easy(3) �₽��Z���͓̂����Ŏ���Ńp���[�_�E�������肵�Ă���B�p���[�_�E���Ȃ̂Œe�͂��Ȃ����A�̗͂��ɒ[�ɏ��Ȃ��̂Ŏ��̒i�K�ɑ����ڍs�����e�����ɂȂ���B
	{	 s_hp(3),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_1a_mima_toge, 				NULL,						mima_01_keitai, 			},
	{	 s_hp(6),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_1a_mima_toge, 				NULL,						mima_01_keitai, 			},
	{	 s_hp(6),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_1a_mima_toge, 				NULL,						mima_01_keitai, 			},
	{	 s_hp(6),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_1a_mima_toge, 				NULL,						mima_01_keitai, 			},
	// 2.--
	{	 s_hp(4),	s_time(20), 	" �@�@�@����u���쐬�X�y�JE01�v" "\n",	SPELL_18_hana_test, 				NULL,						mima_01_keitai, 			},	/* �ʏ�^�C�v */
	{	 s_hp(6),	s_time(20), 	" �@�@�@����u���쐬�X�y�JN01�v" "\n",	SPELL_18_hana_test, 				NULL,						mima_01_keitai, 			},	/* �ʏ�^�C�v */
	{	 s_hp(6),	s_time(20), 	" �@�@�@����u���쐬�X�y�JH01�v" "\n",	SPELL_18_hana_test, 				NULL,						mima_01_keitai, 			},	/* �ʏ�^�C�v */
	{	 s_hp(6),	s_time(20), 	" �@�@�@����u���쐬�X�y�JL01�v" "\n",	SPELL_18_hana_test, 				NULL,						mima_01_keitai, 			},	/* �ʏ�^�C�v */
	// 1.625
	{	 s_hp(8),	s_time(40), 	"�@�@�ŕ��u�f�B�}�[�P�C�V�����v" "\n",	SPELL_29_rumia_demarcation, 		init_00_boss_clip111,		boss_move_01_not_move,		},	/* ��L�^�C�v */	// "�@�@ �ŕ��u�f�B�}�[�P�C�V�����v"
	{	s_hp(18),	s_time(40), 	"�@�@�ŕ��u�f�B�}�[�P�C�V�����v" "\n",	SPELL_29_rumia_demarcation, 		init_00_boss_clip111,		boss_move_01_not_move,		},	/* ��L�^�C�v */
	{	s_hp(18),	s_time(40), 	"�@�@�ŕ��u�f�B�}�[�P�C�V�����v" "\n",	SPELL_29_rumia_demarcation, 		init_00_boss_clip111,		boss_move_01_not_move,		},	/* ��L�^�C�v */
	{	s_hp(18),	s_time(40), 	"�@�@�ŕ��u�f�B�}�[�P�C�V�����v" "\n",	SPELL_29_rumia_demarcation, 		init_00_boss_clip111,		boss_move_01_not_move,		},	/* ��L�^�C�v */
	// 1.00
	{	s_hp(12),	s_time(40), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_23_aya_merin_test,			NULL,						boss_move_01_not_move,		},	/* ��L�^�C�v */
	{	s_hp(10),	s_time(40), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_23_aya_merin_test,			NULL,						boss_move_01_not_move,		},	/* ��L�^�C�v */
	{	s_hp(10),	s_time(40), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_23_aya_merin_test,			NULL,						boss_move_01_not_move,		},	/* ��L�^�C�v */
	{	s_hp(10),	s_time(40), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_23_aya_merin_test,			NULL,						boss_move_01_not_move,		},	/* ��L�^�C�v */
	// 1.--
	{	 s_hp(6),	s_time(40), 	"�@�@�@�@�@�@�@�ؕ��u�F�؈�ࣁv" "\n",	SPELL_25_houka_kenran,				init_00_boss_clip000,		boss_move_01_not_move,		},	/* �ʏ�^�C�v */
	{	 s_hp(8),	s_time(40), 	"�@�@�@�@�@�@�@�ؕ��u�F�؈�ࣁv" "\n",	SPELL_25_houka_kenran,				init_00_boss_clip000,		boss_move_01_not_move,		},	/* �ʏ�^�C�v */
	{	 s_hp(8),	s_time(40), 	"�@�@�@�@�@�@�@�O���u�F�O��ࣁv" "\n",	SPELL_25_houka_kenran,				init_00_boss_clip000,		boss_move_01_not_move,		},	/* �ʏ�^�C�v */
	{	 s_hp(8),	s_time(40), 	"�@�@�@�@�@�@�@�����u�F�،����v" "\n",	SPELL_25_houka_kenran,				init_00_boss_clip000,		boss_move_01_not_move,		},	/* �ʏ�^�C�v */
	// 1.--
	{	 s_hp(6),	s_time(60), 	" �@�@�@����u���쐬�X�y�JE05�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,	},
	{	 s_hp(8),	s_time(60), 	" �@�@�@����u���쐬�X�y�JN05�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,	},
	{	 s_hp(8),	s_time(60), 	" �@�@�@����u���쐬�X�y�JH05�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,	},
	{	 s_hp(8),	s_time(60), 	" �@�@�@����u���쐬�X�y�JL05�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,	},
	// 1.0
	{	s_hp(32),	s_time(60), 	" �@�@�@����u�Ă��ƃX�y�JE06�v" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,	},// �s���㗈�Ȃ��B
	{	s_hp(32),	s_time(60), 	" �@�@�@����u�Ă��ƃX�y�JN06�v" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,	},// �s���㗈�Ȃ��B
	{	s_hp(32),	s_time(60), 	" �@�@�@����u�Ă��ƃX�y�JH06�v" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,	},// �s���㗈�Ȃ��B
	{	s_hp(32),	s_time(60), 	" �@�@�@����u�Ă��ƃX�y�JL06�v" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,	},// �s���㗈�Ȃ��B
	// 1�ʃ{�X�Ȃ̂ŁA(���Ȃ��Ƃ� easy�Anormal �������)����o���Ȃ��B
// �A���X 1��						"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*1.125), s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 16384==8192*2 */	/* "�ʏ�U��"�̎���(?) */ /* "�ʏ�U��"�̃��C�t */
	{s_hp(32*1.25),  s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 16384==8192*2 */	/* "�ʏ�U��"�̎���(?) */ /* "�ʏ�U��"�̃��C�t */
	{s_hp(32*1.50),  s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 16384==8192*2 */	/* "�ʏ�U��"�̎���(?) */ /* "�ʏ�U��"�̃��C�t */
	{s_hp(32*1.50),  s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		common_00_keitai,			},/* 16384==8192*2 */	/* "�ʏ�U��"�̎���(?) */ /* "�ʏ�U��"�̃��C�t */
	// 1.25 easy(10) ������[F]�o��̂ŁA�p���[�͖��Ȃ��B�܂��n�߂ɒe���͊ȒP�Ƃ������Ɋ���Ă��炤�ׁA���߁B(easy�̓v���C���[�Ɏ���ł�����Ă͍���̂ŁA�{�X�Ɛ�����C�ɂ�����̂����)
	{	 s_hp(10),	 s_time(40),	NULL,/* "�ʏ�U��" "\n"*/				SPELL_14_alice_youmu300,			NULL,						boss_move_01_not_move,		},// ���炩�ɊȒP�Ȓe��
	{	  s_hp(6),	 s_time(40),	NULL,/* "�ʏ�U��" "\n"*/				SPELL_14_alice_youmu300,			NULL,						boss_move_01_not_move,		},
	{	  s_hp(5),	 s_time(40),	NULL,/* "�ʏ�U��" "\n"*/				SPELL_13_alice_suwako,				NULL,						boss_move_01_not_move,		},
	{	  s_hp(4),	 s_time(40),	NULL,/* "�ʏ�U��" "\n"*/				SPELL_13_alice_suwako,				NULL,						boss_move_01_not_move,		},
	// 1.--
	{	 s_hp(12),	 s_time(40),	"�@�@�@�@�@���Áu�g�̖��@Lv.2�v" "\n",	SPELL_15_alice_aka_2nd, 			NULL,						boss_move_01_not_move,		},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 s_hp(14),	 s_time(40),	" �@�@�@�@���Áu�g�̖��@Lv.16�v" "\n",	SPELL_15_alice_aka_2nd, 			NULL,						boss_move_01_not_move,		},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 s_hp(12),	 s_time(40),	"�@�@�@�@���Áu�g�̖��@Lv.256�v" "\n",	SPELL_15_alice_aka_2nd, 			NULL,						boss_move_04_xy_douki_differential32,	},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 s_hp(10),	 s_time(40),	"�@�@�@���Áu�g�̖��@Lv.65536�v" "\n",	SPELL_15_alice_aka_2nd, 			NULL,						boss_move_04_xy_douki_differential32,	},/* "�����u�M���̕������l�`�v"�̃��C�t */
	// 1.-- easy(4) �u�p�[�t�F�N�g�t���[�Y�v�͈��̎��̗�������ׁA���S�Ҍ����ł͖����B�����ĕs�K�v�ɒZ������B
	{	  s_hp(4),	 s_time(20),	"  �@�����u�p�[�V�����t���[�Y�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,	},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	  s_hp(6),	 s_time(20),	"�@�����u�p�[�t�F�N�g�t���[�Y�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,	},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	  s_hp(8),	 s_time(20),	"�@�����u�p�[�e�B�N���t���[�N�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,	},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 s_hp(12),	 s_time(20),	"�@�����u�u���U�[�h�t���[�Y���v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,	},/* "�����u�M���̕������l�`�v"�̃��C�t */
	// 1.00
	{	 s_hp(7),	 s_time(60),	"�@�����u�����̃}�g�����[�V�J�v" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki/*boss_move_04_xy_douki_differential32*/,	},/* " �@�����u�����̃}�g�����[�V�J�v"�����u�����̉~�J�l�`�v */ 	//(r33p)boss_move_04_xy_douki_differential
	{	 s_hp(8),	 s_time(60),	"�@�����u�M���̃}�g�����[�V�J�v" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki/*boss_move_04_xy_douki_differential32*/,	},/* " �@�����u�M���̃}�g�����[�V�J�v"�����u�����̉~�J�l�`�v */ 	//(r33p)boss_move_04_xy_douki_differential
	{	 s_hp(8),	 s_time(60),	"�@�����u�����̃}�g�����[�V�J�v" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki/*boss_move_04_xy_douki_differential32*/,	},/* " �@�����u�����̃}�g�����[�V�J�v"�����u�����̉~�J�l�`�v */ 	//(r33p)boss_move_04_xy_douki_differential
	{	 s_hp(8),	 s_time(60),	"�@�����u���Q�̃}�g�����[�V�J�v" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki/*boss_move_04_xy_douki_differential32*/,	},/* " �@�����u���Q�̃}�g�����[�V�J�v"�����u�����̉~�J�l�`�v */ 	//(r33p)boss_move_04_xy_douki_differential
	// 0.--
	{	 s_hp(8),	 s_time(60),	NULL,/* "�ʏ�U��z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,	},/* " �@�@�@�@�����u�W����̈ĎR�q�v"�����u�����̉~�J�l�`�v */
	{	 s_hp(8),	 s_time(60),	NULL,/* "�ʏ�U��z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,	},/* " �@�@�@�@�@�����u���̋@�I�l�`�v"�����u�����̉~�J�l�`�v */
	{	 s_hp(8),	 s_time(60),	NULL,/* "�ʏ�U��z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,	},/* " �@�@�@�@�@�@�����u�Â̗������v"�����u�����̉~�J�l�`�v */
	{	 s_hp(8),	 s_time(60),	NULL,/* "�ʏ�U��z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,	},/* " �@�����u��̃N���b�V���_�~�[�v"�����u�����̉~�J�l�`�v */
//									"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
};
//	" �@�@�@�����u�����̕������l�`�v" "\n"
//	" �@�����u�����̃I�����A�\\�l�`�v" "\n" " �@�����u�����̃I�����A���l�`�v"...���݂̕����ł͖����ł��B
//	{	 (1024*16),  s_time(40),	"�@�����u���쐬�̈����ɐl�`L1�v" "\n",	SPELL_00,							NULL,						alice_04_keitai,			},/* �����u�����̉~�J�l�`�v */
//	{	(20000*4),	 s_time(20),	"�����u���쐬�̈ɑ������l�`L2�v" "\n",	SPELL_00,							NULL,						boss_move_02_xy_hidouki,	},/* �����u�󑐂̐l�`�Ă��v */
//	{	 (5000*4),	 s_time(20),	"�@�����u���쐬�̉p�g���l�`L3�v" "\n",	SPELL_00,							NULL,						alice_02_keitai,			},
//	{	 (5000*4),	 s_time(20),	"�@�����u���쐬�̘I�����l�`L4�v" "\n",	SPELL_00,							NULL,						alice_02_keitai,			},
//	{	 (5000*4),	 s_time(80),	"�@�@�����u���쐬�̐����l�`L5�v" "\n",	SPELL_00,							NULL,						alice_02_keitai,			},
//	{	 (5000*4),	 s_time(80),	"�@�����u���쐬�̔����`�l�`L6�v" "\n",	SPELL_00,							NULL,						alice_02_keitai,			},
//	{	 (5000*4),	 s_time(80),	"�@�@�����u���쐬�̔g���l�`L7�v" "\n",	SPELL_00,							NULL,						alice_02_keitai,			},
	/*
	�}�g�����[�V�J�́A�I�����l�`�ł����̂��ȁH�l�`�Ƃ�������薯�|�i�̎����B
	�ŋ߂͖G���}�g�����[�V�J�̎��v�����{���炠��炵���B
	//
	���{�̋C�ے��͕���17m�ȏ�̕����u�䕗�v�ƒ�`�B
	���ۋC�ۋ@��(WMO)�ł͕���33m�ȏ�̂��̂��utyphoon�v�ƒ�`�B����ȉ��́ustorm�v�B
	*/


/*---------------------------------------------------------
	�X�v���C�g ���X�g�ɓo�^���ꂽ�X�v���C�g��S�� �B
---------------------------------------------------------*/
#define TEST_ZAKO_HIDE (0)
#if (1==TEST_ZAKO_HIDE)/* �{�X���e���󂯂� */
static void sprite_sethide_all(SPRITE *s, unsigned int length)
{
	unsigned int ii;
	for (ii=0; ii<length; ii++ )/* �S�����ׂ�B */
	{
		s->boss_time_out	= 0xff;
		#if (1)/* �f�o�b�O�Ă��� */
		s->cx256		= (-1);
		#endif
		s++;
	}
}
static void zako_all_timeup(void)/*int ty pe*/
{
	SPRITE *s;
	s = &obj99[OBJ_HEAD_01_TEKI+0];
	sprite_sethide_all(s, OBJ_POOL_01_TEKI_MAX);
}
#endif

/*---------------------------------------------------------
	�X�y���J�[�h�V�X�e���̃{�X�`�Ԃɓo�^���ꂽ�e���ԍ����擾���A
	�����ɒe���̎��Ԑ؂��ݒ肷��B
---------------------------------------------------------*/

global void spell_card_get_spell_number(SPRITE *src)
{
	spell_card.spell_type	= my_spell_card_resource[(spell_card.number)].spell_type;	/* �e�����Z�b�g */
	spell_set_time_out(src);		/* �e���̐������Ԃ�ݒ�(�\�ߒe�����Ƃɐݒ肳��Ă���W�����Ԃɐݒ�) */
}


/*---------------------------------------------------------
	�{�X�`�ԕύX���̋��ʃ��[�`��
	�X�y���J�[�h���j��A�C�e���o���B
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
		�X�y���J�[�h���̕\��
		�X�y���J�[�h�w�i�́A�܂��������ǁA�����I�Ɋg���\��B
	---------------------------------------------------------*/
	//static void draw_spell_card_name(void)
	{
		kanji_window_clear_line(1); 	/* �����E�B���h�E��2�s��(==1)�̓��e�������B */
		set_cursor(0, 1);				/* �J�[�\����2�s��(==1)�ֈړ� */
	//
		if (NULL != my_spell_card_resource[(spell_card.number)].spell_str_name)
		{
			/* �X�y���J�[�h�w�i������ꍇ */
			callback_gu_draw_haikei = callback_gu_draw_haikei_supeka;
			//
			cg.msg_time = byou60(5);	/* �� 5 �b */
			print_kanji000(
				my_spell_card_resource[(spell_card.number)].spell_str_name,
				/*int color_type*/(7)|STR_CODE_NO_ENTER,	/* ���s���Ȃ� */
				/*int wait*/(0)
			);
		}
		else
		{
			/* �X�y���J�[�h�w�i�������ꍇ */
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
	-------------------------------------------------------
	ToDo:
	���|��A�ŋ߂̓��C�t�͂���ς胊�j�A�Ɍ����ĂāA
	�\������log�Ȃ񂶂�ˁH���ċC�����Ă����B
	�����������ɂ��悤���ȁB�Q�[���o�����X�Ƃ��F�X�ς�邩��B
---------------------------------------------------------*/
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

//0 ... 255 == draw_boss_hp_value 0001 1111 1110 0000;	>>=(2+3);  0000 0000 1111 1111; 0xff;
/*
	4096/60 == 68.2666666666666666666666666666667
	4096/256 == 16
*/
//#define LIMIT_MAX_HP_DEC_BOSS_BY_FLAME (16)
//#define LIMIT_MAX_HP_DEC_BOSS_BY_FLAME (48)
//	int LIMIT_MAX_HP_DEC_BOSS_BY_FLAME[8] = { (1), (2), (4), (8),  (16), (24), (32), (48), };


global void boss_hp_frame_check(void)
{
//	SPRITE *obj_boss;
//	obj_boss = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
	{
		int limit_max_hp_dec_boss_by_flame;
		u8 test_draw_boss_hp_value;
	//	test_draw_boss_hp_value = (global_obj_boss->base_hp>>5) & 0xff; 	/* 8192/32 == 256 (r32) */
		test_draw_boss_hp_value = (global_obj_boss->base_hp>>7) & 0xff; 	/* 32768/32 == 256 (r33) */
		#if 1
	//	limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>2) | (0x0f);
	//	limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>2) | (0x08);
		limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value) | (0x10);
		#endif
		//
		if (limit_max_hp_dec_boss_by_flame < spell_card.boss_hp_dec_by_frame)
		{
			spell_card.boss_hp_dec_by_frame = limit_max_hp_dec_boss_by_flame;
		}
	}
//
	global_obj_boss->base_hp -= spell_card.boss_hp_dec_by_frame;
	spell_card.boss_hp_dec_by_frame = 0;/* �g�����̂ŏ���(�t���[���P��) */
	if (spell_card.limit_health >= global_obj_boss->base_hp)		/* �K��l�ȉ��ɂȂ�΃X�y���J�[�h���[�h���� */
	{
		spell_card.mode 			= (SPELL_CARD_MODE_00_OFF);
		boss_destroy_check_type(global_obj_boss, DESTROY_CHECK_00_WIN_BOSS);
		callback_gu_draw_haikei = callback_gu_draw_haikei_modosu;
	}
}


// ����̃V�i���I�X�N���v�g�K�i���ƃX�y���J�[�h�V�X�e���Ƒ����������B
// �d���Ȃ��̂ŁA�����Ƀt���O�őΉ�������B
#define USE_OLD_SCRIPT_SYSTEM (1)
//#define USE_OLD_SCRIPT_SYSTEM (0)

/*---------------------------------------------------------
	�X�y���J�[�h�����ɐi�߂�B
---------------------------------------------------------*/
static void spell_card_incliment(void)
{
			spell_card.number += (4)/*1*/;
}

/*---------------------------------------------------------
	[�X�y���J�[�h�V�X�e�����Ɉړ��\��]	�X�y���J�[�h�o�^
---------------------------------------------------------*/
#if (1==USE_OLD_SCRIPT_SYSTEM)
	// ����̃V�i���I�X�N���v�g�K�i���ƃX�y���J�[�h�V�X�e���Ƒ����������B
	// �d���Ȃ��̂ŁA�����Ƀt���O�őΉ�������B
static int speka_first_move_flag;
#endif /* (1==USE_OLD_SCRIPT_SYSTEM) */

/*global*/global/*static*/ void check_regist_generate_spell_card(SPRITE *src)
{
	#if (1==USE_OLD_SCRIPT_SYSTEM)
	// ����̃V�i���I�X�N���v�g�K�i���ƃX�y���J�[�h�V�X�e���Ƒ����������B
	// �d���Ȃ��̂ŁA�����Ƀt���O�őΉ�������B
	int exec_speka;
	exec_speka = 0;
	#endif /* (1==USE_OLD_SCRIPT_SYSTEM) */
//
	if (0/*off*/==spell_card.mode)
	{
	//	if (1)
		{
			int aaa;	/* ���ݑ̗� ���� ���������X�y���J�[�h�� �������̗͒l */
			aaa = spell_card.limit_health - (my_spell_card_resource[(spell_card.number+(4)/*1*/)].spell_life);	/*1000 500*/
		//	spell_card.limit_health -= 1000/*500*/;
			/* �{�X���X�y���J�[�h�����Ă���̗͂�����ꍇ */
			if (0 < aaa)
			{
				spell_card.limit_health = aaa;
			//	if (b_health_alter_low1024 < (data->boss_base.boss_health & (1024-1)))/* �`�ԕύX������ */
				spell_card.mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/
				spell_card_incliment();
			//	if (spell_card_syoji_maisuu < spell_card.number)
				if (0 == spell_card_syoji_maisuu )
				{
					/* �`�ԕύX���Ȃ��A�A�C�e���f���Ȃ� */
					spell_card.number -= (4);
				//	src->callback_loser 			= lose_boss;
				}
				else
		//		if (0==sakuya->base_hp)
				{
					spell_card_syoji_maisuu--;
					#if 0//(1==TEST_ZAKO_HIDE)/* �{�X���e���󂯂� */
					zako_all_timeup();/* �U�R�^�C���A�E�g(�t�F�C�h�A�E�g���������ֈڍs) */
					#endif
		//			if (0 != (data->boss_base.boss_life))/* �`�ԕύX������ */
					/* �A�C�e���f�� */
					if (NULL != src->callback_loser)
					{
		//				data->boss_base.boss_life--;
						(src->callback_loser)(src); 	/* sakuya_put_items(src); */
					}
				}
				#if 0/* (��[�X�y���J�[�h�I������珉����]) (�`�ԕύX����O�ɕK���������������̂ł��̈ʒu�͎~�߂�B)*/
				/* �X�y���J�[�h������ */
				if (NULL != my_spell_card_resource[(spell_card.number)].spell_init_callback)
				{
					(my_spell_card_resource[(spell_card.number)].spell_init_callback)(src);
				}
				#endif
			}
			else
			{
				spell_card.limit_health = (0);
			//	spell_card.mode 		= (0);/*off*/
				spell_card.mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/	/* �Ƃ肠���� */
				/* [(�Ƃ肠����)�X�y���J�[�h�U���݂̂Ɏd�l�ύX]�����̂ŁA�Ō㌂�Ă�X�y���J�[�h���Ȃ��Ȃ����ꍇ�ɍU��������ׁB */
			}
		}
		/*---------------------------------------------------------
			�X�y���J�[�h�`�F�b�N
		---------------------------------------------------------*/
		if (0/*off*/==spell_card.boss_timer)
		{
			spell_card.limit_health = 0;
			spell_card.mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/
		}
	}
	else
	if (SPELL_CARD_MODE_01_IDO_JYUNNBI == spell_card.mode)
	{
		#if (1==USE_OLD_SCRIPT_SYSTEM)
		speka_first_move_flag = 0;
		#endif /* (1==USE_OLD_SCRIPT_SYSTEM) */
		src->boss_spell_timer = (0);	/* �X�y���e�������������I�Ɏ~�߂�B */
		bullets_to_hosi();		/* ���Ă̓G�e���Ahosi�A�C�e���ɕς��� */
		/* �^���t�߂ɑޔ� */
	//	src->toutatu_wariai256				= t256(  0);/* �������ς݂̕K�v���� */
		src->toutatu_wariai256				= t256(1.0);/* �������ς݂̕K�v���� */
	//	src->target_x256			= t256(153);
		src->target_x256			= BOSS_XP256; //t256(0);
	//	src->target_y256			= src->cy256;
		src->target_y256			= t256(16.0); //t256(0);
		#if 1/* (�V[�X�y���J�[�h�n�܂�O�ɏ�����]) ��0�`�Ԃ���A�K���ĂԔ��B */
		/* �X�y���J�[�h������ */
		if (NULL != my_spell_card_resource[(spell_card.number)].spell_init_callback)
		{
			(my_spell_card_resource[(spell_card.number)].spell_init_callback)(src);
		}
		#endif
		spell_card.mode 			= (SPELL_CARD_MODE_02_TAIHI);/*on*/
	}
	else
	if (SPELL_CARD_MODE_02_TAIHI==spell_card.mode)	/* ���e�ʒu�܂ňړ����B */
	{
	//	src->toutatu_wariai256 -= (1);		/* [��	4 �b]== 4.2666==(256/60[flame]) */
	//	src->toutatu_wariai256 -= (1*4);		/* [��	1 �b]== 1.0666==(256/(4*60)[flame]) */
//		src->toutatu_wariai256 -= (1*(4-1));	/* [��	1 �b]== 1.0666==(256/(4*60)[flame]) */
		#if 1/*(r32)*/
	//	boss_yuudou_idou_nomi(src);/*(r32)*/
		boss_yuudou_hiritu_keisan(src);
	//	src->toutatu_wariai256 -= (1);	/* [��	4 �b]== 4.2666==(256/60[flame]) */
		src->toutatu_wariai256 -= (1*4);	/* [��	1 �b]== 1.0666==(256/(4*60)[flame]) */
		if (0 > src->toutatu_wariai256 )	/* �قډ�ʒ��S�t�߂܂ŁA�ړ������B */
		{
			src->toutatu_wariai256 = (0);
			spell_card.mode 		= (SPELL_CARD_MODE_03_HATUDAN);/*on*/
		}
		#endif/*(r32)*/
		#if 0/*(r32p)*/
		boss_yuudou_idou_nomi(src);/*(r32p)*/
		boss_yuudou_hiritu_keisan(src);
	//	src->toutatu_wariai256 -= (1);	/* [��	4 �b]== 4.2666==(256/60[flame]) */
	//	src->toutatu_wariai256 -= (1*4);	/* [��	1 �b]== 1.0666==(256/(4*60)[flame]) */
	//	if (0 > src->toutatu_wariai256 )	/* �قډ�ʒ��S�t�߂܂ŁA�ړ������B */
		if ((0==src->toutatu_wariai256))
		{
	//		src->toutatu_wariai256 = (0);
			spell_card.mode 		= (SPELL_CARD_MODE_03_HATUDAN);/*on*/
		}
		#endif/*(r32p)*/
	}
	/*---------------------------------------------------------
		[�X�y���J�[�h�V�X�e�����Ɉړ��\��]	�X�y���J�[�h����
	---------------------------------------------------------*/
	else/* [(�Ƃ肠����)�X�y���J�[�h�U���݂̂Ɏd�l�ύX]*/
	/*
		����V�i���I����{�X���R���g���[���o���Ȃ��̂ł��̎d�l�͎b��I�B
		(�������̈ړ��Ȃǂ��X�y���J�[�h����)
		(�X�y���J�[�h�����Ȃ����������)
	*/
	/* �{�X�s�� */
	if (SPELL_CARD_MODE_03_HATUDAN==spell_card.mode)
	#if (1==USE_OLD_SCRIPT_SYSTEM)
	{
		exec_speka = 1;
		//(my_spell_card_resource[(spell_card.number)].spell_move02_callback)(src);
	}
	if (exec_speka + speka_first_move_flag)
	#endif /* (1==USE_OLD_SCRIPT_SYSTEM) */
	{
		(my_spell_card_resource[(spell_card.number)].spell_move02_callback)(src);
	}
}


/*---------------------------------------------------------
	[�X�y���J�[�h�V�X�e�����Ɉړ��\��]
---------------------------------------------------------*/


/*---------------------------------------------------------
	�{�X���U�������ꍇ�̋��ʃ��[�`��
	-------------------------------------------------------
	SPRITE *src;	�{�X�G����
	SPRITE *tama;	���e
---------------------------------------------------------*/

/*static*/static/*global*/	void s_callback_hit_boss(SPRITE *src, SPRITE *tama)
{
	/* �{�X & ��-�{�X�Ɏ��e�����������ꍇ�̉ΉԃG�t�F�N�g��o�^(����Gu�������Ȃ��̂ŕ`�悵�ĂȂ�) */
	bakuhatsu_add_type_ddd(tama, BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/* ��Ɏ��s������������ */
//
	/* ��ƕ������������R�[�h������������B */
	{
		spell_card.boss_hp_dec_by_frame += /*w->*/tama->base_weapon_strength;	/* �U�����đ̗͌��炷(����������) */
	}
}

/*global*/static /*static*/ void common_03_keitai(SPRITE *src)
{
//		if ( ((cg.state_flag) & STATE_FLAG_05_IS_BOSS) )	/* ��b�I��? */
		{
			src->toutatu_wariai256 = t256(1.0);
		//	common_boss_init_2nd(src);	/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
			/*---------------------------------------------------------
				�{�X�̋��ʁA�Q��ڏ��������[�`��(�U���\)
			---------------------------------------------------------*/
		//	/*extern*/static void common_boss_init_2nd(SPRITE *src)
			{
				/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
				src->callback_hit_teki = s_callback_hit_boss;	/* �R�[���o�b�N�o�^ */
				/* spell_card common init */
				spell_card.mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/
			//	spell_card.mode 		= (0);/*off*/
			}
		//	/*���Ԑ����J�E���g�L����*/
		//	data->boss_base.state001++/* = ST_02*/;
			spell_card_incliment();
		}
}
/*global*/static /*static*/ void common_02_keitai(SPRITE *src)
{
	//	if ( ((cg.state_flag) & STATE_FLAG_05_IS_BOSS) )	/* ��b�I��? */
	//	{
	//		my_spell_card_resource[(spell_card.number)].spell_move02_callback = common_03_keitai;
	//	}
}
global /*static*/ void common_00_keitai(SPRITE *src)
{
//	if (0 == src->toutatu_wariai256)
	if (0 > src->boss_base_state777)
	{
		src->boss_base_state777 = (0);
	//	src->toutatu_wariai256 = t256(1.0);
	}
	else	/* ��b�I����҂� */
	{
		my_spell_card_resource[(spell_card.number)].spell_move02_callback = common_02_keitai;
	}
}
global void script_boss_start(void)
{
		my_spell_card_resource[(spell_card.number)].spell_move02_callback = common_03_keitai;
}


/*---------------------------------------------------------
	���ʃ{�X�ޔ�(���j��ɉ�ʊO�Ƀ{�X��������)
	[�X�y���J�[�h�V�X�e�����Ɉړ��\��]
---------------------------------------------------------*/

global /*static*/ void common_99_keitai(SPRITE *src)
{
	if (0 > (src->cy256+t256(50.0)))/* +t256(50.0) �{�X�O���̍ő�T�C�Y(50[dot]) */
	{
		bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
		src->callback_mover		= NULL; 		/* �����܂� */
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
			src->cx256 += t256(1.0);	/* �E��ɑޔ� */
		}
		else
		{
			src->cx256 -= t256(1.0);	/* ����ɑޔ� */
		}
	}
}


/*---------------------------------------------------------
	�e�����I���܂ő҂B(�p�~)
	-------------------------------------------------------

---------------------------------------------------------*/
#if 0
	/*static*/global void danmaku_state_check_holding(SPRITE *src)
	{
		if (SPELL_00 == spell_card.spell_type)
		{
			src->boss_base_state777++;
		}
	}
#endif


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
global void game_command_03_regist_boss(GAME_COMMAND *l)
{
		cg.state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
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
	sss = boss_number;
	sss &= (8-1);
//
	//----[BOSS]
	SPRITE *h;
//	#if (0==U SE_BOSS_COMMON_MALLOC)
//	h									= obj_add_01_teki_error();
//	obj_boss							= h;/*�P��{�l*/
//	#else
	h									= global_obj_boss;/*�P��{�l*/
//	SPRITE *obj_boss;
//	obj_boss = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
//	SPRITE *h;
//	h = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];

//	#endif
//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* �����o�^ */		/* �d�l�o�O(?) */
	{
		h->m_Hit256R					= ZAKO_ATARI16_PNG;
		h->flags						|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
		h->type 						= BOSS_00_BOSS11;
		h->callback_mover				= common_boss_move;
		h->callback_loser				= common_boss_put_items;
		h->callback_hit_teki			= NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
	//
		#if 0/* �������ς݂̕K�v���� */
		h->vx256						= t256( 0);
		h->vy256						= t256( 0);
		#endif
		h->cx256						= BOSS_XP256;/*t256(GAME_WIDTH/2)*/
		h->cy256						= t256(-100);
	//
		h->kougeki_anime_count			= (0);	/* �U���A�j���[�V�����p�J�E���^ / 0�ȉ��Ȃ�ړ��A�j���[�V���� */
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
	//------------ �X�y���J�[�h�֘A
		{
			static const u16/*u8*/ aaa[8+8] =/* �X�y�J�̎�ނ�256��ނ𒴂����̂ŁAu16 */
			{
			/* �{�X�J�n�X�y���J�[�h�ԍ�(easy) */
				SPELL_CARD_00E_chrno_000,	/* �`���m(?) */ 	/* �G�L�X�g���p(boss0) */
				SPELL_CARD_00E_alice_000,	/* �A���X */
				SPELL_CARD_00E_mima_000,	/* ���� */
				SPELL_CARD_00E_kaguya_000,	/* �P�� */
			//
				SPELL_CARD_00E_aya_000, 	/* �� */
				SPELL_CARD_00E_pacheA_000,	/* �p�`�F */
				SPELL_CARD_00E_sakuya_000,	/* ��� */
				SPELL_CARD_00E_pacheA_000,	/* �t����(?) */ 	/* �t�@���^�Y���p(boss7) */
			/* �{�X�X�y���J�[�h������ */
				(6),//(0-0),													/* �`���m(?) */ 	/* �G�L�X�g���p(boss0) */
				(5),//(SPELL_CARD_16E_alice_ggg-SPELL_CARD_00E_alice_000),		/* �A���X */		/*SPELL_CARD_19_alice_jjj;*/
				(5),//(SPELL_CARD_17E_mima_hhh-SPELL_CARD_00E_mima_000),		/* ���� */
				(6),//(SPELL_CARD_17E_kaguya_hhh-SPELL_CARD_00E_kaguya_000),	/* �P�� */
			//
				(6),//(SPELL_CARD_19E_aya_jjj-SPELL_CARD_00E_aya_000),			/* �� */
				(8),//(SPELL_CARD_17E_pache_hhh-SPELL_CARD_00E_pache_000),		/* �p�`�F */
				(9),//(SPELL_CARD_11E_sakuya_kkk-SPELL_CARD_00E_sakuya_000),	/* ��� */
				(5),//(0-0),													/* �t����(?) */ 	/* �t�@���^�Y���p(boss7) */
			//
			};
			spell_card.number				= aaa[sss  ] + ((cg_game_difficulty)&0x03)
				+ ((5!=sss)?(0):(((cg_game_select_player)&(4-1))<<(2+3)));/* ��Փx(2bit==4�i�K)�A�X�y���J�[�h(3bit==8�i�K) */
			spell_card_syoji_maisuu 		= aaa[sss+8];
		}
		/* spell_card common init */
	//	{
	//	//	spell_card_limit_health 	= 0;
	//	//	spell_card_mode 			= 0;/*off*/
	//	}
		{
			h->base_hp				= (my_spell_card_resource[(spell_card.number)].spell_life); 		/* �S�̗̂̑� */
			spell_card.limit_health = (h->base_hp) - (my_spell_card_resource[(spell_card.number + (4)/*1*/)].spell_life);		/* �ʏ�U��(����U��)�̍U���� */
			//
			spell_card.boss_timer	= (((my_spell_card_resource[(spell_card.number)].spell_limit_time)));	/* 75*64==75[count] 	��99[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
			spell_card.mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/	/* ����H */
			/*???*/
			my_spell_card_resource[(spell_card.number)].spell_move02_callback = common_00_keitai;
		}
		#if 1
	//------------ �e���֘A
		spell_card.spell_type			= SPELL_00; 	/* �e�������I���t���O */
		spell_card.number_temporaly 	= (0);			/*(SPELL_08_rumia-1)*/ /*0*/
		#endif
	//
	}
	{
		boss_effect_reset();
	}
		// �{�X�e�N�X�`���ǂݍ���
		stage_boss_load_texture();
	#if (1==USE_OLD_SCRIPT_SYSTEM)
	// ����̃V�i���I�X�N���v�g�K�i���ƃX�y���J�[�h�V�X�e���Ƒ����������B
	// �d���Ȃ��̂ŁA�����Ƀt���O�őΉ�������B
	speka_first_move_flag = 1;
	#endif /* (1==USE_OLD_SCRIPT_SYSTEM) */
}
