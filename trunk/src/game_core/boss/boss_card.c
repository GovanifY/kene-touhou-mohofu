
#include "boss.h"


/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�J�[�h�Ǘ��V�X�e��
	-------------------------------------------------------
	�J�[�h�̓V�X�e���ŃT�|�[�g���Ă���̂ŁA�ǂ̃{�X���A�ǂ̃J�[�h�ł����Ă܂��B
	-------------------------------------------------------
	ToDo:
	�� �U���̊Ǘ�
	�� ����̊Ǘ�(���\����)
	�� �G��(���G��)���J�[�h�����Ă�@�\�́A�p�~���������ǂ������m��Ȃ��B
	����(���G�������G���J�[�h�@�\)��p�~���āA����ɓ����Ƀ{�X���o��������(�܂蒆-�{�X)
	�̕����A�V�X�e�����������肵�ėǂ��C������B��������ׂɂ͐F�X������������B
	����� dat �̋K�i���ƁA�����͏o���Ȃ��̂ŁA��� dat �̋K�i��ς��āA
	dat ����{�X�̉�b���N������悤�ɂ���K�v������B
	�v���C���[�ɋ����ĉ�b���Ⴄ�̂ŁA���̕ӂ̋K�i����͂܂�����B(����V�i���I�Ƃ�)
	-------------------------------------------------------
	�� �G��(���G��)���J�[�h�����Ă�@�\�́A�p�~���āA
	�G��(���G��)���J�[�h�����������ꍇ�́A�u�J�[�h�����Ă�U�R�v�Ƃ�������V�݂��Ă�����g��
	�V�X�e���ł̓T�|�[�g���Ȃ������ǂ��Ǝv���B�̂ł������������ŉ����\��B
	-------------------------------------------------------
	GAME_X_OFFSET �Ή��B
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
//tern void sakuya_03_keitai(SPRITE *src);/* ���� */
extern void sakuya_02_keitai(SPRITE *src);
//tern void sakuya_01_keitai(SPRITE *src);/* ���� */

// �p�`�F 5��

//���[�U�[
extern void add_laser_off(SPRITE *src);
extern void add_laser_on(SPRITE *src);

// �� 4��
extern void aya_05_keitai(SPRITE *src);
//tern void aya_04_keitai(SPRITE *src);/* ���� */
//tern void aya_03_keitai(SPRITE *src);/* ���� */
//tern void aya_02_keitai(SPRITE *src);/* ���� */
//tern void aya_01_keitai(SPRITE *src);/* ���� */

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

/*(��b���̌`��)*/
extern void kaiwa_00_keitai(SPRITE *src);
//extern void common_99_keitai(SPRITE *src);/* ���j��ɉ�ʊO�Ƀ{�X�������� */


/* ���ʕ� */
global void init_00_boss_clip000(SPRITE *h);/* �W���^�C�v */
global void init_00_boss_clip111(SPRITE *h);/* ��ɍL���^�C�v */




//------------ �J�[�h�֘A
#include "card_address.h"
#if (1)
	/* ���݂̌`�ԂŌ��Ă�J�[�h�ԍ����J�[�h�V�X�e���ɐݒ�B */
	/* AYA�T�`�Ԃ̏ꍇ�B(r35-)�̃V�X�e���Œʏ�e�𗘗p����ׂɁA�J�[�h�V�X�e���̏��������K�v�B */
	/* ���������āA�ǂ�Ȍ`�Ԃł�(r35�ȍ~��)�K���X�y�J�ɂ���K�v������B */
	#define init_48_r34_gokan_kinou 	card_maikai_init_and_get_spell_number
	#if (1)
		#define SPELL_r34_GOKAN_KINOU		SPELL_48_r34_gokan_kinou/*(�K�v)*/
	#else
	//	#define SPELL_r34_GOKAN_KINOU		SPELL_00/*(�_��)*/
	#endif
#endif
//
// int card.limit_health;	/* �K��l�ȉ��ɂȂ�� �J�[�h���[�h���� */
// int card.boss_state; 	/* ���l�ɂȂ�΃{�X �J�[�h���[�h�ɓ���Ȃ� */
// int card.mode;			/* �J�[�h���[�h */
// int card.boss_timer; 	/* ���p */	// ��������
// int card.number; 		/* ���p */	// �J�[�h�ԍ�

global CARD_SYSTEM_GLOBAL_CLASS card;
/*global*/ static int card_syoji_maisuu;		/* ���p */	// �J�[�h�ԍ��ő���E�l



typedef struct
{
	int spell_life; 							/* �J�[�h�ɓo�^���ꂽ���̗� */
	int spell_limit_time;						/* �J�[�h�̐������ԁB(�J�[�h�ɓo�^���ꂽ��莞��) */
//
	const char *spell_str_name; 				/* �J�[�h���� */
	int card_number;							/* �J�[�h�̎�� */
//
	void (*spell_init_callback)(SPRITE *sss);			/* �������ړ����� */
	void (*boss_move_keitai_callback)(SPRITE *sss); 	/* �{�X�ړ��`�ԑI������ */
} CARD_ADDRESS_RESOURCE;
//	void (*spell_yuudou_callback)(SPRITE *sss); 		/* �J�[�h�U���ړ����� */
//�p�~	void (*spell_tamakesi04_callback)(void);		/* �J�[�h�e��ʊO����(�e���� / �e���� / ...) */

	/* ���O�̓e�L�g�[�ł� */

/* �J�[�h����(==�P��[�t���[��])�́A64�{(==�P��[��1�b])���K�i */
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
/*---------------------------------------------------------
	�X���u�A�C�V�N���t�H�[���v�����Ȃ��ŃJ�[�h�����B
---------------------------------------------------------*/

static CARD_ADDRESS_RESOURCE my_card_resource[CARD_ADDRESS_MAX] =
{									// �ő唼�p��30�����B
// �`���m �G�L�X�g���X�e�[�W		"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},	/* 73728==8192*9.0 */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},	/* 73728==8192*9.0 */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},	/* 73728==8192*9.0 */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},	/* 73728==8192*9.0 */
//	3.5
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_10_cirno, 					NULL,						boss_move_02_xy_hidouki,				},	/* "���`��: " */	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_10_cirno, 					NULL,						boss_move_02_xy_hidouki,				},	/* "���`��: " */	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_10_cirno, 					NULL,						boss_move_02_xy_hidouki,				},	/* "���`��: " */	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_10_cirno, 					NULL,						boss_move_02_xy_hidouki,				},	/* "���`��: " */	/* ����̍U����(�蓮�ݒ�)  500 160	*/
//	3.0
	{	s_hp(24),	s_time(20), 	"�@�@�X���u�A�C�V�N���t�H�[���v" "\n",	SPELL_1b_cirno_icecle_fall, 		NULL,						boss_move_01_not_move,					},	/* "���`��: " */
	{	s_hp(24),	s_time(20), 	"�@�@�X���u�A�C�V�N���t�H�[���v" "\n",	SPELL_1b_cirno_icecle_fall, 		NULL,						boss_move_01_not_move,					},	/* "���`��: " */
	{	s_hp(24),	s_time(20), 	"�@�@�X���u�A�C�V�N���t�H�[���v" "\n",	SPELL_1b_cirno_icecle_fall, 		NULL,						boss_move_01_not_move,					},	/* "���`��: " */
	{	s_hp(24),	s_time(20), 	"�@�@�X���u�A�C�V�N���t�H�[���v" "\n",	SPELL_1b_cirno_icecle_fall, 		NULL,						boss_move_01_not_move,					},	/* "���`��: " */
//	2.25
	{	 s_hp(8),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,				},	/* "��O�`��: " */
	{	 s_hp(8),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,				},	/* "��O�`��: " */
	{	 s_hp(8),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,				},	/* "��O�`��: " */
	{	 s_hp(8),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,				},	/* "��O�`��: " */
//	2.0
	{	s_hp(16),	s_time(20), 	"�@�����u�p�[�t�F�N�g�t���[�Y�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,				},	/* "��l�`��: " */
	{	s_hp(16),	s_time(20), 	"�@�����u�p�[�t�F�N�g�t���[�Y�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,				},	/* "��l�`��: " */
	{	s_hp(16),	s_time(20), 	"�@�����u�p�[�t�F�N�g�t���[�Y�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,				},	/* "��l�`��: " */
	{	s_hp(16),	s_time(20), 	"�@�����u�p�[�t�F�N�g�t���[�Y�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,				},	/* "��l�`��: " */
//	1.5
	{	s_hp(32),	s_time(20), 	"�ᕄ�u�_�C�A�����h�u���U�[�h�v" "\n",	SPELL_12_diamond_blizzard,			NULL,						boss_move_02_xy_hidouki,				},	/* "��܌`��: " */
	{	s_hp(32),	s_time(20), 	"�ᕄ�u�_�C�A�����h�u���U�[�h�v" "\n",	SPELL_12_diamond_blizzard,			NULL,						boss_move_02_xy_hidouki,				},	/* "��܌`��: " */
	{	s_hp(32),	s_time(20), 	"�ᕄ�u�_�C�A�����h�u���U�[�h�v" "\n",	SPELL_12_diamond_blizzard,			NULL,						boss_move_02_xy_hidouki,				},	/* "��܌`��: " */
	{	s_hp(32),	s_time(20), 	"�ᕄ�u�_�C�A�����h�u���U�[�h�v" "\n",	SPELL_12_diamond_blizzard,			NULL,						boss_move_02_xy_hidouki,				},	/* "��܌`��: " */
//	0.0
	// ���easy�͒Z����ɒi�K�����Ȃ��B
// ��� 6�� 						"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*2.0),	s_time(900),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},	/* 73728==8192*9.0 */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},	/* 73728==8192*9.0 */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},	/* 73728==8192*9.0 */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},	/* 73728==8192*9.0 */
//	9.5
	{	s_hp(6),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_01_sakuya_misogi_normal,		NULL,						boss_move_04_xy_douki_differential32,	},	/* "���`��: �ʏ�U��(���E�ɓ������S�e����)"		*/	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	s_hp(10),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_01_sakuya_misogi_normal,		NULL,						boss_move_04_xy_douki_differential32,	},	/* "���`��: �ʏ�U��(���E�ɓ������S�e����)"		*/	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	s_hp(10),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_01_sakuya_misogi_normal,		NULL,						boss_move_04_xy_douki_differential32,	},	/* "���`��: �ʏ�U��(���E�ɓ������S�e����)"		*/	/* ����̍U����(�蓮�ݒ�)  500 160	*/
	{	s_hp(10),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_2d_sakuya_misogi_lunatic, 	NULL,						boss_move_04_xy_douki_differential32,	},	/* "���`��: �ʏ�U��(���E�ɓ������S�e����)"		*/	/* ����̍U����(�蓮�ݒ�)  500 160	*/
//	9.0
	{	s_hp(3),	s_time(10), 	"�@�@��p�u�~�X�f�B���N�V�����v" "\n",	SPELL_0d_sakuya_miss_direction, 	NULL,						sakuya_02_keitai,						},	/* "���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)"	*/
	{	s_hp(4),	s_time(10), 	"�@�@��p�u�~�X�f�B���N�V�����v" "\n",	SPELL_0d_sakuya_miss_direction, 	NULL,						sakuya_02_keitai,						},	/* "���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)"	*/
	{	s_hp(5),	s_time(10), 	"��p�u���f�~�X�f�B���N�V�����v" "\n",	SPELL_0d_sakuya_miss_direction, 	NULL,						sakuya_02_keitai,						},	/* "���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)"	*/
	{	s_hp(5),	s_time(10), 	"��p�u���f�~�X�f�B���N�V�����v" "\n",	SPELL_0d_sakuya_miss_direction, 	NULL,						sakuya_02_keitai,						},	/* "���`��: ��p�u�~�X�f�B���N�V�����v(�S���ʁA���܂�)"	*/
//	8.25	// bloody==�����炯�́B staind �X�e�C���h==�X�e���h==(�F��)�Ă��t��(�Ďc��)��B
	{	 s_hp(3),	s_time(20), 	NULL,/* "�ʏ�U��3" "\n"*/				SPELL_2c_sakuya_blue_red,			NULL,						boss_move_04_xy_douki_differential32,	},	/* "��O�`��: " " �@�@�@�@���ہu�y�b�p�[�i�C�t�v"		*/
	{	 s_hp(4),	s_time(20), 	NULL,/* "�ʏ�U��3" "\n"*/				SPELL_2c_sakuya_blue_red,			NULL,						boss_move_04_xy_douki_differential32,	},	/* "��O�`��: " " �@�@�@�@���J�u�X�e���h�i�C�t�v"		*/
	{	 s_hp(4),	s_time(20), 	NULL,/* "�ʏ�U��3" "\n"*/				SPELL_2c_sakuya_blue_red,			NULL,						boss_move_04_xy_douki_differential32,	},	/* "��O�`��: " " �@�@�܌��J�u�u���b�f�B�i�C�t�v"		*/
	{	 s_hp(4),	s_time(20), 	NULL,/* "�ʏ�U��3" "\n"*/				SPELL_2c_sakuya_blue_red,			NULL,						boss_move_04_xy_douki_differential32,	},	/* "��O�`��: " " �@�@�����u�p�[�t�F�N�g�i�C�t�v"		*/
//	8.0
	{	s_hp(6),	s_time(20), 	NULL,/* "�ʏ�U��"���Βe */ 			SPELL_2a_sakuya_baramaki1,			NULL,						sakuya_04_keitai,						},	/* "��l�`��: �����w����"	*/
	{	s_hp(9),	s_time(20), 	NULL,/* "�ʏ�U��"���Βe */ 			SPELL_2a_sakuya_baramaki1,			NULL,						sakuya_04_keitai,						},	/* "��l�`��: �����w����"	*/
	{	s_hp(9),	s_time(20), 	NULL,/* "�ʏ�U��"���Βe */ 			SPELL_2a_sakuya_baramaki1,			NULL,						sakuya_04_keitai,						},	/* "��l�`��: �����w����"	*/
	{	s_hp(9),	s_time(20), 	NULL,/* "�ʏ�U��"���Βe */ 			SPELL_2a_sakuya_baramaki1,			NULL,						sakuya_04_keitai,						},	/* "��l�`��: �����w����"	*/
//	7.5 	// meek==�f���B
	{	s_hp(6),	s_time(20), 	"���f�u�u���h�c�F�y�V���݂̓��v" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,				},	/* sakuya_06_keitai"��܌`��: (���F�}�X�J�b�g�eA)"	"�@�@����u�f���W�����X�~�[�N�v"	*/
	{	s_hp(9),	s_time(20), 	"���f�u�u���h�c�F�y�V���̎􂢁v" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,				},	/* sakuya_06_keitai"��܌`��: (���F�}�X�J�b�g�eA)"	"�@����u�f���W�����X���[���h�v"	*/
	{	s_hp(9),	s_time(20), 	"�j�f�u�u���h�c�F�y�V���̏j���v" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,				},	/* sakuya_06_keitai"��܌`��: (���F�}�X�J�b�g�eA)"	"�@�@���ہu�f���W�����X�^�C���v"	*/
	{	s_hp(9),	s_time(20), 	"���f�u�u���h�c�F�y�V���̋����v" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,				},	/* sakuya_06_keitai"��܌`��: (���F�}�X�J�b�g�eA)"	"�@�����u�f���W�����X�X�g�[���v"	*/
	//
	{	s_hp(6),	s_time(20), 	"���f�u�u���h�c�F�y�V���݂̓��v" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,				},	/* sakuya_06_keitai"��Z�`��: (���F�}�X�J�b�g�eB)"					*/
	{	s_hp(9),	s_time(20), 	"���f�u�u���h�c�F�y�V���̎􂢁v" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,				},	/* sakuya_06_keitai"��Z�`��: (���F�}�X�J�b�g�eB)"					*/
	{	s_hp(9),	s_time(20), 	"�j�f�u�u���h�c�F�y�V���̏j���v" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,				},	/* sakuya_06_keitai"��Z�`��: (���F�}�X�J�b�g�eB)"					*/
	{	s_hp(9),	s_time(20), 	"���f�u�u���h�c�F�y�V���̋����v" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,				},	/* sakuya_06_keitai"��Z�`��: (���F�}�X�J�b�g�eB)"					*/
//	6.5 	// �W���b�N�E�U�E���b�p�[==Jack the Ripper==�؂�􂫃W���b�N(�E�l�S)19���I(1888�N)�Ɏ���(?)/���ۖ������B���[���h�q�[���[�Y�B ���b�N==�X�v���b�^�[�n�E�X
	{	s_hp(12),	s_time(20), 	"�@��u�W���b�N�E�K�[���b�N�v" "\n",	SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,				},	/* "�掵�`��: (���U�����w)�ǉ��v�撆"				*/
	{	s_hp(16),	s_time(20), 	"��p�u�W���b�N�E�U�E���b�p�[�v" "\n",	SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,				},	/* "�掵�`��: (���U�����w)�ǉ��v�撆"				*/
	{	s_hp(16),	s_time(20), 	"��p�u�W���b�N�E�U�E�r�[���Y�v" "\n",	SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,				},	/* "�掵�`��: (���U�����w)�ǉ��v�撆"				*/
	{	s_hp(16),	s_time(20), 	"����u�W���b�N�E�f�E�}���}�L�v" "\n",	SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,				},	/* "�掵�`��: (���U�����w)�ǉ��v�撆"				*/
//
	{	s_hp(8),	s_time(20), 	NULL,									SPELL_2b_sakuya_baramaki2,			NULL,						sakuya_08_keitai,						},	/* "��l�`��: �����w����"	*/
	{	s_hp(12),	s_time(20), 	NULL,									SPELL_2b_sakuya_baramaki2,			NULL,						sakuya_08_keitai,						},	/* "��l�`��: �����w����"	*/
	{	s_hp(12),	s_time(20), 	NULL,									SPELL_2b_sakuya_baramaki2,			NULL,						sakuya_08_keitai,						},	/* "��l�`��: �����w����"	*/
	{	s_hp(12),	s_time(20), 	NULL,									SPELL_2b_sakuya_baramaki2,			NULL,						sakuya_08_keitai,						},	/* "��l�`��: �����w����"	*/
//	4.5
	{	s_hp(8),	s_time(20), 	"�@���z�u�t�F�X�e�B�o���i�C�t�v" "\n",	SPELL_r34_GOKAN_KINOU,				init_48_r34_gokan_kinou,	sakuya_09_keitai,						},	/* "���`��: �ŏI�`��(����1)"					*/
	{	s_hp(12),	s_time(20), 	"�@�����u�t�F�X�e�B�o���i�C�t�v" "\n",	SPELL_r34_GOKAN_KINOU,				init_48_r34_gokan_kinou,	sakuya_09_keitai,						},	/* "���`��: �ŏI�`��(����1)"					*/
	{	s_hp(12),	s_time(20), 	"�@���t�u�t�F�X�e�B�o���i�C�t�v" "\n",	SPELL_r34_GOKAN_KINOU,				init_48_r34_gokan_kinou,	sakuya_09_keitai,						},	/* "���`��: �ŏI�`��(����1)"					*/
	{	s_hp(12),	s_time(20), 	"�@���f�u�t�F�X�e�B�o���i�C�t�v" "\n",	SPELL_r34_GOKAN_KINOU,				init_48_r34_gokan_kinou,	sakuya_09_keitai,						},	/* "���`��: �ŏI�`��(����1)"					*/
//	2.5
	{	s_hp(8),	s_time(20), 	NULL,									SPELL_0e_remilia_00,				NULL,						sakuya_10_keitai,						},	/* "��10�`��: �ŏI�`��(����2)"	*/
	{	s_hp(12),	s_time(20), 	NULL,									SPELL_0e_remilia_00,				NULL,						sakuya_10_keitai,						},	/* "��10�`��: �ŏI�`��(����2)"	*/
	{	s_hp(12),	s_time(20), 	NULL,									SPELL_0e_remilia_00,				NULL,						sakuya_10_keitai,						},	/* "��10�`��: �ŏI�`��(����2)"	*/
	{	s_hp(12),	s_time(20), 	NULL,									SPELL_0e_remilia_00,				NULL,						sakuya_10_keitai,						},	/* "��10�`��: �ŏI�`��(����2)"	*/
//	0.5
	{	 s_hp(8),	s_time(20), 	"�@�@���C�h�閧�u�c�s�s�׎蓖�v" "\n",	SPELL_0e_remilia_00,				NULL,						sakuya_11_keitai,						},	/* "��11�`��: �ŏI�`��(����3)"					*/	// (easy)���͂����ɂ͂��Ȃ�
	{	 s_hp(8),	s_time(20), 	"�@�@���C�h��Z�u�́I�́I�́I�v" "\n",	SPELL_0e_remilia_00,				NULL,						sakuya_11_keitai,						},	/* "��11�`��: �ŏI�`��(����3)"					*/
	{	 s_hp(8),	s_time(20), 	"�@���C�h���u�����Ď��ʂ��v" "\n",	SPELL_0e_remilia_00,				NULL,						sakuya_11_keitai,						},	/* "��11�`��: �ŏI�`��(����3)"					*/
	{	 s_hp(8),	s_time(20), 	"�@�@���C�h���X�u���������{�X�v" "\n",	SPELL_0e_remilia_00,				NULL,						sakuya_11_keitai,						},	/* "��11�`��: �ŏI�`��(����3)"					*/
//	0.25
//	{	s_hp(16),	s_time(20), 	"�@�@�@�@��p�u���e�X�gE008�v" "\n",	SPELL_00,							NULL,						common_00_keitai,						},	/* "�攪�`��: (���~�߃i�C�t)�ǉ��v�撆" 		*/
//	{	s_hp(24),	s_time(20), 	"�@�@�@�@��p�u���e�X�gN008�v" "\n",	SPELL_00,							NULL,						common_00_keitai,						},	/* "�攪�`��: (���~�߃i�C�t)�ǉ��v�撆" 		*/
//	{	s_hp(24),	s_time(20), 	"�@�@�@�@��p�u���e�X�gH008�v" "\n",	SPELL_00,							NULL,						common_00_keitai,						},	/* "�攪�`��: (���~�߃i�C�t)�ǉ��v�撆" 		*/
//	{	s_hp(24),	s_time(20), 	"�@�@�@�@��p�u���e�X�gL008�v" "\n",	SPELL_00,							NULL,						common_00_keitai,						},	/* "�攪�`��: (���~�߃i�C�t)�ǉ��v�撆" 		*/

	// �p�`�Feasy�͒Z�����1�i�K���Ȃ��B
// A(�아)
// �p�`�FA 5��						"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*2.00), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
//	5.0
	{	s_hp(10),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
//	4.0
	{	s_hp(10),	s_time(30), 	"�@�@�@�@�Ε��u�A�O�j�V���C���v" "\n",	SPELL_2e_pache_agni_shine_1,		add_laser_off,				boss_move_03_x_douki,					},
	{	s_hp(16),	s_time(30), 	"�@�@�@�@�Ε��u�A�O�j�V���C���v" "\n",	SPELL_2e_pache_agni_shine_1,		add_laser_off,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�@�@�Ε��u�A�O�j�V���C���㋉�v" "\n",	SPELL_33_pache_agni_shine_2,		add_laser_off,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�@�@�Ε��u�A�O�j�V���C���㋉�v" "\n",	SPELL_33_pache_agni_shine_2,		add_laser_off,				boss_move_02_xy_hidouki,				},
//	3.0
	{	s_hp(10),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
//	2.0
	{	s_hp(10),	s_time(30), 	"�@�@�y���u���C�W�B�g�����g���v" "\n",	SPELL_31_pache_rage_tririton_1, 	add_laser_off,				boss_move_01_not_move,					},/*(�e�X�g)*/
	{	s_hp(16),	s_time(30), 	"�y���u���C�W�B�g�����g���㋉�v" "\n",	SPELL_35_pache_rage_tririton_2, 	add_laser_off,				boss_move_01_not_move,					},/*(�e�X�g)*/
	{	s_hp(16),	s_time(30), 	"�@�@�y���u�g�����g���V�F�C�N�v" "\n",	SPELL_39_pache_tririton_shake,		add_laser_off,				boss_move_01_not_move,					},/*(�e�X�g)*/
	{	s_hp(16),	s_time(30), 	"�@�@�y���u�g�����g���V�F�C�N�v" "\n",	SPELL_39_pache_tririton_shake,		add_laser_off,				boss_move_01_not_move,					},/*(�e�X�g)*/
//	1.0
	{	s_hp(10),	s_time(30), 	"�΁��y���u���[���@�N�������N�v" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�΁��y���u���[���@�N�������N�v" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�΁��y���u���[���@�N�������N�v" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�΁��y���u���[���@�N�������N�v" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						boss_move_02_xy_hidouki,				},
//	0.0
	{	s_hp(10),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
//	0.0
	{	s_hp(10),	s_time(30), 	"�؁��Ε��u�t�H���X�g�u���C�Y�v" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						boss_move_01_not_move,					},	// (easy)���͂����ɂ͂��Ȃ�
	{	s_hp(16),	s_time(30), 	"�؁��Ε��u�t�H���X�g�u���C�Y�v" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�؁��Ε��u�t�H���X�g�u���C�Y�v" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�؁��Ε��u�t�H���X�g�u���C�Y�v" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						boss_move_01_not_move,					},

// B(����)
// �p�`�FB 5��						"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*2.00), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
//	5.0
	{	s_hp(10),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
//	4.0
	{	s_hp(10),	s_time(30), 	"�@�����u�v�����Z�X�E���f�B�l�v" "\n",	SPELL_2f_pache_princess_undine, 	add_laser_off,				boss_move_04_xy_douki_differential32,	},
	{	s_hp(16),	s_time(30), 	"�@�����u�v�����Z�X�E���f�B�l�v" "\n",	SPELL_2f_pache_princess_undine, 	add_laser_off,				boss_move_04_xy_douki_differential32,	},
	{	s_hp(16),	s_time(30), 	"�@�@�@�����u�x���[�C�����C�N�v" "\n",	SPELL_37_pache_bury_in_lake,		add_laser_off,				boss_move_04_xy_douki_differential32,	},
	{	s_hp(16),	s_time(30), 	"�@�@�@�����u�x���[�C�����C�N�v" "\n",	SPELL_37_pache_bury_in_lake,		add_laser_off,				boss_move_02_xy_hidouki,				},
//	3.0
	{	s_hp(10),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
//	2.0
	{	s_hp(10),	s_time(30), 	"�@�@�ؕ��u�V���t�B�z�����㋉�v" "\n",	SPELL_34_pache_sylphy_horn_2,		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�@�@�ؕ��u�V���t�B�z�����㋉�v" "\n",	SPELL_34_pache_sylphy_horn_2,		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�@�@�@�ؕ��u�O���[���X�g�[���v" "\n",	SPELL_38_pache_green_storm, 		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�@�@�@�ؕ��u�O���[���X�g�[���v" "\n",	SPELL_38_pache_green_storm, 		add_laser_off,				boss_move_01_not_move,					},
//	1.0
	{	s_hp(10),	s_time(30), 	"�@�����ؕ��u�E�H�[�^�[�G���t�v" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�@�����ؕ��u�E�H�[�^�[�G���t�v" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�@�����ؕ��u�E�H�[�^�[�G���t�v" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�@�����ؕ��u�E�H�[�^�[�G���t�v" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,				},
//	0.0
	{	s_hp(10),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
//	0.0
	{	s_hp(10),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},	// (easy)���͂����ɂ͂��Ȃ�
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},

// C(����)
// �p�`�FC 5��						"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*2.00), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
//	5.0
	{	s_hp(10),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
//	4.0
	{	s_hp(10),	s_time(30), 	"�@�@�@�@�ؕ��u�V���t�B�z�����v" "\n",	SPELL_30_pache_sylphy_horn_1,		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�@�@�@�@�ؕ��u�V���t�B�z�����v" "\n",	SPELL_30_pache_sylphy_horn_1,		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�@�@�ؕ��u�V���t�B�z�����㋉�v" "\n",	SPELL_34_pache_sylphy_horn_2,		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�@�@�ؕ��u�V���t�B�z�����㋉�v" "\n",	SPELL_34_pache_sylphy_horn_2,		add_laser_off,				boss_move_01_not_move,					},
//	3.0
	{	s_hp(10),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
//	2.0
	{	s_hp(10),	s_time(30), 	"�@�@�@�@�Ε��u�A�O�j�V���C���v" "\n",	SPELL_2e_pache_agni_shine_1,		add_laser_off,				boss_move_03_x_douki,					},
	{	s_hp(16),	s_time(30), 	"�@�@�Ε��u�A�O�j�V���C���㋉�v" "\n",	SPELL_33_pache_agni_shine_2,		add_laser_off,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�@�Ε��u�A�O�j���C�f�B�A���X�v" "\n",	SPELL_36_pache_agni_radiance,		add_laser_off,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�@�Ε��u�A�O�j���C�f�B�A���X�v" "\n",	SPELL_36_pache_agni_radiance,		add_laser_off,				boss_move_02_xy_hidouki,				},
//	1.0
	{	s_hp(10),	s_time(30), 	"�؁��Ε��u�t�H���X�g�u���C�Y�v" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�؁��Ε��u�t�H���X�g�u���C�Y�v" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�؁��Ε��u�t�H���X�g�u���C�Y�v" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�؁��Ε��u�t�H���X�g�u���C�Y�v" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						boss_move_01_not_move,					},
//	0.0
	{	s_hp(10),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
//	0.0
	{	s_hp(10),	s_time(30), 	"�΁��y���u���[���@�N�������N�v" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						boss_move_02_xy_hidouki,				},	// (easy)���͂����ɂ͂��Ȃ�
	{	s_hp(16),	s_time(30), 	"�΁��y���u���[���@�N�������N�v" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�΁��y���u���[���@�N�������N�v" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�΁��y���u���[���@�N�������N�v" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						boss_move_02_xy_hidouki,				},

// D(����)
// �p�`�FD 5��						"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*2.00), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
//	5.0
	{	s_hp(10),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
//	4.0
	{	s_hp(10),	s_time(30), 	"�@�@�y���u���C�W�B�g�����g���v" "\n",	SPELL_31_pache_rage_tririton_1, 	add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�@�@�y���u���C�W�B�g�����g���v" "\n",	SPELL_31_pache_rage_tririton_1, 	add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�y���u���C�W�B�g�����g���㋉�v" "\n",	SPELL_35_pache_rage_tririton_2, 	add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�y���u���C�W�B�g�����g���㋉�v" "\n",	SPELL_35_pache_rage_tririton_2, 	add_laser_off,				boss_move_01_not_move,					},
//	3.0
	{	s_hp(10),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
//	2.0
	{	s_hp(10),	s_time(30), 	"�@�@�����u���^���t�@�e�B�[�O�v" "\n",	SPELL_32_pache_metal_fatigue,		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�@�@�����u���^���t�@�e�B�[�O�v" "\n",	SPELL_32_pache_metal_fatigue,		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�@�@�@�����u�V���o�[�h���S���v" "\n",	SPELL_3a_pache_silver_dragon,		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"�@�@�@�����u�V���o�[�h���S���v" "\n",	SPELL_3a_pache_silver_dragon,		add_laser_off,				boss_move_01_not_move,					},
//	1.0
	{	s_hp(10),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�y�������u�G�������h���K���X�v" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
//	0.0
	{	s_hp(10),	s_time(30), 	"�@�����ؕ��u�E�H�[�^�[�G���t�v" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�@�����ؕ��u�E�H�[�^�[�G���t�v" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�@�����ؕ��u�E�H�[�^�[�G���t�v" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"�@�����ؕ��u�E�H�[�^�[�G���t�v" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,				},
//	0.0
	{	s_hp(10),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},	// (easy)���͂����ɂ͂��Ȃ�
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"���������u�}�[�L�����|�C�Y���v" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},

//	{	s_hp(10),	s_time(99), 	" �@�����u�T�C�����g���쐬E06�v" "\n",	SPELL_00,							NULL,						boss_move_01_not_move,					},
//	{	s_hp(16),	s_time(99), 	" �@�����u�T�C�����g���쐬N06�v" "\n",	SPELL_00,							NULL,						boss_move_01_not_move,					},
//	{	s_hp(16),	s_time(99), 	" �@�����u�T�C�����g���쐬H06�v" "\n",	SPELL_00,							NULL,						boss_move_01_not_move,					},
//	{	s_hp(16),	s_time(99), 	" �@�����u�T�C�����g���쐬L06�v" "\n",	SPELL_00,							NULL,						boss_move_01_not_move,					},

//	{	s_hp(10),	s_time(99), 	" �@�X���u�p�`�����[���쐬E07�v" "\n",	SPELL_00,							NULL,						boss_move_01_not_move,					},
//	{	s_hp(16),	s_time(99), 	" �@�X���u�p�`�����[���쐬N07�v" "\n",	SPELL_00,							NULL,						boss_move_01_not_move,					},
//	{	s_hp(16),	s_time(99), 	" �@�X���u�p�`�����[���쐬H07�v" "\n",	SPELL_00,							NULL,						boss_move_01_not_move,					},
//	{	s_hp(16),	s_time(99), 	" �@�X���u�p�`�����[���쐬L07�v" "\n",	SPELL_00,							NULL,						boss_move_01_not_move,					},

//	SPELL_15_aya_misogi
//	SPELL_25_houka_kenran			/*aya_02_keitai*/
//	SPELL_23_meirin_magaru_kunai	/*aya_01_keitai*/

// �� 4��							"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	//																											/*�{�X�o��O�̏�����[��b�̑O]*/
	{s_hp(32*3),	s_time(600),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							NULL,						kaiwa_00_keitai,						},/* 32768=32768.0 */
	{s_hp(32*3),	s_time(600),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							NULL,						kaiwa_00_keitai,						},/* 32768=32768.0 */
	{s_hp(32*3),	s_time(600),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							NULL,						kaiwa_00_keitai,						},/* 32768=32768.0 */
	{s_hp(32*3),	s_time(600),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							NULL,						kaiwa_00_keitai,						},/* 32768=32768.0 */
	//	4.0 																									/*�{�X�퓬�O�̏�����[��b�̌�]*/
	{	s_hp(12),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_20_aya_misogi1,				init_00_boss_clip000,		boss_move_03_x_douki,					},/*(4)*/
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_20_aya_misogi1,				init_00_boss_clip000,		boss_move_03_x_douki,					},/*(4)*/
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_20_aya_misogi1,				init_00_boss_clip000,		boss_move_03_x_douki,					},/*(4)*/
	{	s_hp(16),	s_time(20), 	NULL,/* "�ʏ�U��1" "\n"*/				SPELL_20_aya_misogi1,				init_00_boss_clip000,		boss_move_03_x_douki,					},/*(4)*/
	//	3.5
	{	s_hp(8),	s_time(30), 	"�@�@�@�@�@�@�@�򕄁u�V�̔��ˁv" "\n",	SPELL_22_ame_no_yatimata,			init_00_boss_clip111,		boss_move_01_not_move,					},/*(4)*/
	{	s_hp(8),	s_time(30), 	"�@�@�@�@�@�@�@�򕄁u�V�̔��ˁv" "\n",	SPELL_22_ame_no_yatimata,			init_00_boss_clip111,		boss_move_01_not_move,					},/*(4)*/
	{	s_hp(8),	s_time(30), 	"�@�@�@�@�@�@�@�򕄁u�V�̔��ˁv" "\n",	SPELL_22_ame_no_yatimata,			init_00_boss_clip111,		boss_move_01_not_move,					},/*(4)*/
	{	s_hp(8),	s_time(30), 	"�@�@�@�@�@�@�@�򕄁u�V�̔��ˁv" "\n",	SPELL_22_ame_no_yatimata,			init_00_boss_clip111,		boss_move_01_not_move,					},/*(4)*/
	//	3.0
	{	s_hp(10),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_21_aya_misogi2,				init_00_boss_clip000,		boss_move_03_x_douki,					},/*(5)"�򕄁u���쐬�J�[�h001�v"*/
	{	s_hp(12),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_21_aya_misogi2,				init_00_boss_clip000,		boss_move_03_x_douki,					},/*(5)"�򕄁u���쐬�J�[�h001�v"*/
	{	s_hp(12),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_21_aya_misogi2,				init_00_boss_clip000,		boss_move_03_x_douki,					},/*(5)"�򕄁u���쐬�J�[�h001�v"*/
	{	s_hp(12),	s_time(20), 	NULL,/* "�ʏ�U��2" "\n"*/				SPELL_21_aya_misogi2,				init_00_boss_clip000,		boss_move_03_x_douki,					},/*(5)"�򕄁u���쐬�J�[�h001�v"*/
	//	2.5
	{	 s_hp(4),	s_time(40), 	"�@�@�@�@�@�@�E���u�n����D�]�v" "\n",	SPELL_24_aya_doll,					NULL,						boss_move_03_x_douki,					},/*(4)"�򕄁u���쐬�J�[�h002�v"*/
	{	 s_hp(4),	s_time(40), 	"�@�@�@�@�@�����u�n���卪��Q�v" "\n",	SPELL_24_aya_doll,					NULL,						boss_move_03_x_douki,					},/*(4)"�򕄁u���쐬�J�[�h002�v"*/
	{	 s_hp(4),	s_time(40), 	"�@�@�@�@�@�@�ߏ�u�R�����Q�v" "\n",	SPELL_24_aya_doll,					NULL,						boss_move_03_x_douki,					},/*(4)"�򕄁u���쐬�J�[�h002�v"*/
	{	 s_hp(4),	s_time(40), 	"�@�@�@�@�@��ʁu�����\\���ۉ߁v" "\n", SPELL_24_aya_doll,					NULL,						boss_move_03_x_douki,					},/*(4)"�򕄁u���쐬�J�[�h002�v"*/
	//	2.0
	{	s_hp(16),	s_time(100),	"�@�@�@�@�@�@�@�����u�����R�l�v" "\n",	SPELL_r34_GOKAN_KINOU,				init_48_r34_gokan_kinou,	aya_05_keitai,							},/*(5)"�򕄁u���쐬�J�[�h003�v"*/
	{	s_hp(16),	s_time(100),	"�@�@�@�@�@�@�����u�����R�R�l�v" "\n",	SPELL_r34_GOKAN_KINOU,				init_48_r34_gokan_kinou,	aya_05_keitai,							},/*(5)"�򕄁u���쐬�J�[�h003�v"*/
	{	s_hp(16),	s_time(100),	"�@�@�@�@�@�@�����u�����X�W�l�v" "\n",	SPELL_r34_GOKAN_KINOU,				init_48_r34_gokan_kinou,	aya_05_keitai,							},/*(5)"�򕄁u���쐬�J�[�h003�v"*/
	{	s_hp(16),	s_time(100),	"�@�@�@�@�@�@�����u�������蒆�v" "\n",	SPELL_r34_GOKAN_KINOU,				init_48_r34_gokan_kinou,	aya_05_keitai,							},/*(5)"�򕄁u���쐬�J�[�h003�v"*/
	//	1.5
	{	s_hp(8),	s_time(99), 	"�@�@�@�@�@�@�H���u�����̒��ׁv" "\n",	SPELL_1d_amefuri_test,				init_00_boss_clip111,		boss_move_01_not_move,					},/*(4)"�򕄁u���쐬�J�[�h004�v"*/
	{	s_hp(8),	s_time(99), 	"�@�@�@�@�@�@�H���u����̒��ׁv" "\n",	SPELL_1d_amefuri_test,				init_00_boss_clip111,		boss_move_01_not_move,					},/*(4)"�򕄁u���쐬�J�[�h004�v"*/
	{	s_hp(8),	s_time(99), 	"�@�@�@�@�@�@�H���u�Ӌ|�̒��ׁv" "\n",	SPELL_1d_amefuri_test,				init_00_boss_clip111,		boss_move_01_not_move,					},/*(4)"�򕄁u���쐬�J�[�h004�v"*/
	{	s_hp(8),	s_time(99), 	"�@�@�@�@�ǐՁu�R�����{�̒��ׁv" "\n",	SPELL_1d_amefuri_test,				init_00_boss_clip111,		boss_move_01_not_move,					},/*(4)"�򕄁u���쐬�J�[�h004�v"*/
	//	1.0
	{	s_hp(8),	s_time(99), 	"�@�@�ؕ��u���남�ڂ��F�؈�ࣁv" "\n",	SPELL_25_houka_kenran,				init_00_boss_clip000,		boss_move_01_not_move,					},/*(5)*/
	{	s_hp(8),	s_time(99), 	"�@�@�@�@�@�@�@�Ǖ��u�R�_�n��v" "\n",	SPELL_26_aya_saifu, 				init_00_boss_clip000,		boss_move_01_not_move,					},/*(5)*/
	{	s_hp(8),	s_time(99), 	"�@�@�@�@�Ǖ��u�V��V���̏ƚ��v" "\n",	SPELL_26_aya_saifu, 				init_00_boss_clip000,		boss_move_01_not_move,					},/*(5)*/
	{	s_hp(8),	s_time(99), 	"�@�@�@�@�Ǖ��u�B��Ƒ��̏ƚ��v" "\n",	SPELL_26_aya_saifu, 				init_00_boss_clip000,		boss_move_01_not_move,					},/*(5)*/
	//	0.5
//	{	s_hp(16),	s_time(99), 	"�@�@�@�򕄁u���쐬�J�[�hE006�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,							},/*(5)*/
//	{	s_hp(16),	s_time(99), 	"�@�@�@�򕄁u���쐬�J�[�hN006�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,							},/*(5)*/
//	{	s_hp(16),	s_time(99), 	"�@�@�@�򕄁u���쐬�J�[�hH006�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,							},/*(5)*/
//	{	s_hp(16),	s_time(99), 	"�@�@�@�򕄁u���쐬�J�[�hL006�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,							},/*(5)*/
	//	0.0
//	{	s_hp(64),	s_time(99), 	"�@�@�@�򕄁u���쐬�J�[�hE007�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,							},/*(5)*/
//	{	s_hp(64),	s_time(99), 	"�@�@�@�򕄁u���쐬�J�[�hN007�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,							},/*(5)*/
//	{	s_hp(64),	s_time(99), 	"�@�@�@�򕄁u���쐬�J�[�hH007�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,							},/*(5)*/
//	{	s_hp(64),	s_time(99), 	"�@�@�@�򕄁u���쐬�J�[�hL007�v" "\n",	SPELL_00,							NULL,						aya_05_keitai,							},/*(5)*/

//	SPELL_1e_kaguya01		�{�X�s���A�� 1�`��
//	SPELL_1f_kaguya04		�{�X�s���A�� 4�`��
	// 3�ʃ{�X�Aeasy�͂��������ȒP�ɁB���͂���Ȃ�ɁB
// �P�� 3�� 						"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*2.0),	s_time(500),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 28672==8192*3.5 */
	{s_hp(32*3.0),	s_time(500),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 28672==8192*3.5 */
	{s_hp(32*3.0),	s_time(500),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 28672==8192*3.5 */
	{s_hp(32*3.0),	s_time(500),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 28672==8192*3.5 */
	// 3.5
	{	s_hp(8),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_1e_kaguya01,					boss_init_kaguya,			kaguya_01_keitai,						}, // ����(5)�҂�e�A�ԁA�B���@�_���A�e(8)��18�Z�b�g(�b)���炢(�n�߂�5�b�͏o���Ȃ�)�B(�P����Ԋu�ňړ��A�I���㒆���ֈړ�)
	{	s_hp(12),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_1e_kaguya01,					boss_init_kaguya,			kaguya_01_keitai,						}, // ����(5)�҂�e�A�ԁA�B���@�_���A�e(8)��18�Z�b�g(�b)���炢(�n�߂�5�b�͏o���Ȃ�)�B(�P����Ԋu�ňړ��A�I���㒆���ֈړ�)
	{	s_hp(12),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_1e_kaguya01,					boss_init_kaguya,			kaguya_01_keitai,						}, // ����(5)�҂�e�A�ԁA�B���@�_���A�e(8)��18�Z�b�g(�b)���炢(�n�߂�5�b�͏o���Ȃ�)�B(�P����Ԋu�ňړ��A�I���㒆���ֈړ�)
	{	s_hp(12),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_1e_kaguya01,					boss_init_kaguya,			kaguya_01_keitai,						}, // ����(5)�҂�e�A�ԁA�B���@�_���A�e(8)��18�Z�b�g(�b)���炢(�n�߂�5�b�͏o���Ȃ�)�B(�P����Ԋu�ňړ��A�I���㒆���ֈړ�)
	// 3.0
	{	s_hp(8),	s_time(40), 	" �@�@�@�@�@�@���u���쐬E01�v" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,						},// 1.������̋�	(�P�钆���Œ�A�Ԏ��Ή��̊ےe�A�Z���[�U�[)4�Z�b�gx
	{	s_hp(12),	s_time(40), 	" �@�@�@�@�@�@���u���쐬N01�v" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,						},// 1.������̋�	(�P�钆���Œ�A�Ԏ��Ή��̊ےe�A�Z���[�U�[)4�Z�b�gx
	{	s_hp(12),	s_time(40), 	" �@�@�@�@�@�@���u���쐬H01�v" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,						},// 1.������̋�	(�P�钆���Œ�A�Ԏ��Ή��̊ےe�A�Z���[�U�[)4�Z�b�gx
	{	s_hp(12),	s_time(40), 	" �@�@�@�@�@�@���u���쐬L01�v" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,						},// 1.������̋�	(�P�钆���Œ�A�Ԏ��Ή��̊ےe�A�Z���[�U�[)4�Z�b�gx
	// 2.5
	{	s_hp(8),	s_time(40), 	" �@�@�@�@�@�@���u���쐬E02�v" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,						},// 2.���̌�΂̔�
	{	s_hp(12),	s_time(40), 	" �@�@�@�@�@�@���u���쐬N02�v" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,						},// 2.���̌�΂̔�
	{	s_hp(12),	s_time(40), 	" �@�@�@�@�@�@���u���쐬H02�v" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,						},// 2.���̌�΂̔�
	{	s_hp(12),	s_time(40), 	" �@�@�@�@�@�@���u���쐬L02�v" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,						},// 2.���̌�΂̔�
	// 2.0
	{	s_hp(8),	s_time(40), 	" �@�@�@�@�@�@���u���쐬E03�v" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,						},// 3.�Αl�̔��
	{	s_hp(12),	s_time(40), 	" �@�@�@�@�@�@���u���쐬N03�v" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,						},// 3.�Αl�̔��
	{	s_hp(12),	s_time(40), 	" �@�@�@�@�@�@���u���쐬H03�v" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,						},// 3.�Αl�̔��
	{	s_hp(12),	s_time(40), 	" �@�@�@�@�@�@���u���쐬L03�v" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,						},// 3.�Αl�̔��
	// 1.5
	{	s_hp(8),	s_time(40), 	" �@�@�@�@�@�@���u���쐬E04�v" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,						},// 4.���̎q���L
	{	s_hp(12),	s_time(40), 	" �@�@�@�@�@�@���u���쐬N04�v" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,						},// 4.���̎q���L
	{	s_hp(12),	s_time(40), 	" �@�@�@�@�@�@���u���쐬H04�v" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,						},// 4.���̎q���L
	{	s_hp(12),	s_time(40), 	" �@�@�@�@�@�@���u���쐬L04�v" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,						},// 4.���̎q���L
	// 1.0
	{	s_hp(8),	s_time(40), 	" �@�@�@�@�@�@���u���쐬E05�v" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,					},// 5.�H���̋ʂ̎}
	{	s_hp(12),	s_time(40), 	" �@�@�@�@�@�@���u���쐬N05�v" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,					},// 5.�H���̋ʂ̎}
	{	s_hp(12),	s_time(40), 	" �@�@�@�@�@�@���u���쐬H05�v" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,					},// 5.�H���̋ʂ̎}
	{	s_hp(12),	s_time(40), 	" �@�@�@�@�@�@���u���쐬L05�v" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,					},// 5.�H���̋ʂ̎}
	// 0.5
	{	s_hp(8),	s_time(40), 	" �@�@�@���u�H���̋ʂ̎}E06�v" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,					},//
	{	s_hp(12),	s_time(40), 	" �@�@�@���u�H���̋ʂ̎}N06�v" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,					},//
	{	s_hp(12),	s_time(40), 	" �@�@�@���u�H���̋ʂ̎}H06�v" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,					},//
	{	s_hp(12),	s_time(40), 	" �@�@�@���u�H���̋ʂ̎}L06�v" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,					},//
	// 2�ʃ{�X�Ȃ̂ŁA�K���ɈՂ����B
// ���� 2�� 						"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*1.4),	s_time(400),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 20480==8192*2.5 */
	{s_hp(32*2),	s_time(400),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 20480==8192*2.5 */
	{s_hp(32*2),	s_time(400),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 20480==8192*2.5 */
	{s_hp(32*2),	s_time(400),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 20480==8192*2.5 */
	// 2.000 (12==32*0.375) // easy(3) �₽��Z���͓̂����Ŏ���Ńp���[�_�E����z�肵�Ă���B�p���[�_�E���Ȃ̂Œe�͂��Ȃ����A�̗͂��ɒ[�ɏ��Ȃ��̂Ŏ��̒i�K�ɑ����ڍs�����e�����ɂȂ���B
	{	 s_hp(3),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_1a_mima_toge, 				NULL,						mima_01_keitai, 						},
	{	 s_hp(6),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_1a_mima_toge, 				NULL,						mima_01_keitai, 						},
	{	 s_hp(6),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_1a_mima_toge, 				NULL,						mima_01_keitai, 						},
	{	 s_hp(6),	s_time(20), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_1a_mima_toge, 				NULL,						mima_01_keitai, 						},
	// 2.--
	{	 s_hp(5),	s_time(20), 	" �@�@�@����u���쐬�J�[�hE01�v" "\n",	SPELL_27_hosigata_test, 			NULL,						mima_01_keitai, 						},	/* �ʏ�^�C�v */
	{	 s_hp(6),	s_time(20), 	" �@�@�@����u���쐬�J�[�hN01�v" "\n",	SPELL_27_hosigata_test, 			NULL,						mima_01_keitai, 						},	/* �ʏ�^�C�v */
	{	 s_hp(6),	s_time(20), 	" �@�@�@����u���쐬�J�[�hH01�v" "\n",	SPELL_27_hosigata_test, 			NULL,						mima_01_keitai, 						},	/* �ʏ�^�C�v */
	{	 s_hp(6),	s_time(20), 	" �@�@�@����u���쐬�J�[�hL01�v" "\n",	SPELL_27_hosigata_test, 			NULL,						mima_01_keitai, 						},	/* �ʏ�^�C�v */
	// 1.625
	{	s_hp(10),	s_time(40), 	"�@�@�ŕ��u�f�B�}�[�P�C�V�����v" "\n",	SPELL_29_rumia_demarcation, 		NULL/*init_00_boss_clip111*/,	boss_move_01_not_move,				},	/* ��L�^�C�v */	// "�@�@ �ŕ��u�f�B�}�[�P�C�V�����v"
	{	s_hp(18),	s_time(40), 	"�@�@�ŕ��u�f�B�}�[�P�C�V�����v" "\n",	SPELL_29_rumia_demarcation, 		NULL/*init_00_boss_clip111*/,	boss_move_01_not_move,				},	/* ��L�^�C�v */
	{	s_hp(18),	s_time(40), 	"�@�@�ŕ��u�f�B�}�[�P�C�V�����v" "\n",	SPELL_29_rumia_demarcation, 		NULL/*init_00_boss_clip111*/,	boss_move_01_not_move,				},	/* ��L�^�C�v */
	{	s_hp(18),	s_time(40), 	"�@�@�ŕ��u�f�B�}�[�P�C�V�����v" "\n",	SPELL_29_rumia_demarcation, 		NULL/*init_00_boss_clip111*/,	boss_move_01_not_move,				},	/* ��L�^�C�v */
	// 1.00
	{	s_hp(12),	s_time(40), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_23_meirin_magaru_kunai,		init_00_boss_clip111,		boss_move_01_not_move,					},	/* ��L�^�C�v */
	{	s_hp(10),	s_time(40), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_23_meirin_magaru_kunai,		init_00_boss_clip111,		boss_move_01_not_move,					},	/* ��L�^�C�v */
	{	s_hp(10),	s_time(40), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_23_meirin_magaru_kunai,		init_00_boss_clip111,		boss_move_01_not_move,					},	/* ��L�^�C�v */
	{	s_hp(10),	s_time(40), 	NULL,/* "�ʏ�U��" "\n"*/				SPELL_23_meirin_magaru_kunai,		init_00_boss_clip111,		boss_move_01_not_move,					},	/* ��L�^�C�v */
	// 1.--
	{	 s_hp(6),	s_time(40), 	"�@�@�@�@�@�@�@�ؕ��u�F�؈�ࣁv" "\n",	SPELL_25_houka_kenran,				init_00_boss_clip000,		boss_move_01_not_move,					},	/* �ʏ�^�C�v */
	{	 s_hp(8),	s_time(40), 	"�@�@�@�@�@�@�@�ؕ��u�F�؈�ࣁv" "\n",	SPELL_25_houka_kenran,				init_00_boss_clip000,		boss_move_01_not_move,					},	/* �ʏ�^�C�v */
	{	 s_hp(8),	s_time(40), 	"�@�@�@�@�@�@�@�O���u�F�O��ࣁv" "\n",	SPELL_25_houka_kenran,				init_00_boss_clip000,		boss_move_01_not_move,					},	/* �ʏ�^�C�v */
	{	 s_hp(8),	s_time(40), 	"�@�@�@�@�@�@�@�����u�F�،����v" "\n",	SPELL_25_houka_kenran,				init_00_boss_clip000,		boss_move_01_not_move,					},	/* �ʏ�^�C�v */
	// 1.--
	{	 s_hp(6),	s_time(60), 	" �@�@�@����u���쐬�J�[�hE05�v" "\n",	SPELL_18_hana_test, 				NULL,						boss_move_02_xy_hidouki,				},
	{	 s_hp(8),	s_time(60), 	" �@�@�@����u���쐬�J�[�hN05�v" "\n",	SPELL_18_hana_test, 				NULL,						boss_move_02_xy_hidouki,				},
	{	 s_hp(8),	s_time(60), 	" �@�@�@����u���쐬�J�[�hH05�v" "\n",	SPELL_18_hana_test, 				NULL,						boss_move_02_xy_hidouki,				},
	{	 s_hp(8),	s_time(60), 	" �@�@�@����u���쐬�J�[�hL05�v" "\n",	SPELL_18_hana_test, 				NULL,						boss_move_02_xy_hidouki,				},
	// 1.0
	{	s_hp(32),	s_time(60), 	" �@�@�@����u�Ă��ƃJ�[�hE06�v" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},// �s���㗈�Ȃ��B
	{	s_hp(32),	s_time(60), 	" �@�@�@����u�Ă��ƃJ�[�hN06�v" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},// �s���㗈�Ȃ��B
	{	s_hp(32),	s_time(60), 	" �@�@�@����u�Ă��ƃJ�[�hH06�v" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},// �s���㗈�Ȃ��B
	{	s_hp(32),	s_time(60), 	" �@�@�@����u�Ă��ƃJ�[�hL06�v" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},// �s���㗈�Ȃ��B
	// 1�ʃ{�X�Ȃ̂ŁA(���Ȃ��Ƃ� easy�Anormal �������)����o���Ȃ��B
// �A���X 1��						"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{s_hp(32*1.125), s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 16384==8192*2 */	/* "�ʏ�U��"�̎���(?) */ /* "�ʏ�U��"�̃��C�t */
	{s_hp(32*1.25),  s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 16384==8192*2 */	/* "�ʏ�U��"�̎���(?) */ /* "�ʏ�U��"�̃��C�t */
	{s_hp(32*1.50),  s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 16384==8192*2 */	/* "�ʏ�U��"�̎���(?) */ /* "�ʏ�U��"�̃��C�t */
	{s_hp(32*1.50),  s_time(300),	NULL,/* "�`�ԕύX" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 16384==8192*2 */	/* "�ʏ�U��"�̎���(?) */ /* "�ʏ�U��"�̃��C�t */
	// 1.25 easy(10) ������[F]�o��̂ŁA�p���[�͖��Ȃ��B�܂��n�߂ɒe���͊ȒP�Ƃ������Ɋ���Ă��炤�ׁA���߁B(easy�̓v���C���[�Ɏ���ł�����Ă͍���̂ŁA�{�X�Ɛ�����C�ɂ�����̂����)
	{	 s_hp(10),	 s_time(40),	NULL,/* "�ʏ�U��" "\n"*/				SPELL_14_alice_youmu300,			NULL,						boss_move_01_not_move,					},// ���炩�ɊȒP�Ȓe��
	{	  s_hp(6),	 s_time(40),	NULL,/* "�ʏ�U��" "\n"*/				SPELL_14_alice_youmu300,			NULL,						boss_move_01_not_move,					},
	{	  s_hp(5),	 s_time(40),	NULL,/* "�ʏ�U��" "\n"*/				SPELL_13_alice_suwako,				NULL,						boss_move_01_not_move,					},
	{	  s_hp(4),	 s_time(40),	NULL,/* "�ʏ�U��" "\n"*/				SPELL_13_alice_suwako,				NULL,						boss_move_01_not_move,					},
	// 1.--
	{	 s_hp(12),	 s_time(40),	"�@�@�@�@�@���Áu�g�̖��@Lv.2�v" "\n",	SPELL_15_alice_aka_2nd, 			NULL,						boss_move_01_not_move,					},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 s_hp(14),	 s_time(40),	" �@�@�@�@���Áu�g�̖��@Lv.16�v" "\n",	SPELL_15_alice_aka_2nd, 			NULL,						boss_move_01_not_move,					},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 s_hp(12),	 s_time(40),	"�@�@�@�@���Áu�g�̖��@Lv.256�v" "\n",	SPELL_15_alice_aka_2nd, 			NULL,						boss_move_04_xy_douki_differential32,	},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 s_hp(10),	 s_time(40),	"�@�@�@���Áu�g�̖��@Lv.65536�v" "\n",	SPELL_15_alice_aka_2nd, 			NULL,						boss_move_04_xy_douki_differential32,	},/* "�����u�M���̕������l�`�v"�̃��C�t */
	// 1.-- easy(4) �u�p�[�t�F�N�g�t���[�Y�v�͈��̎��̗�������ׁA���S�Ҍ����ł͖����B�����ĕs�K�v�ɒZ������B
	{	  s_hp(4),	 s_time(20),	" �@�@ ����u�J�[�h��̂Ă��Ɓv" "\n",	SPELL_44_ruiz3_test,				NULL,						boss_move_02_xy_hidouki,				},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	  s_hp(6),	 s_time(20),	"�@�����u�p�[�t�F�N�g�t���[�Y�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,				},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	  s_hp(8),	 s_time(20),	"�@�����u�p�[�e�B�N���t���[�N�v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,				},/* "�����u�M���̕������l�`�v"�̃��C�t */
	{	 s_hp(12),	 s_time(20),	"�@�����u�u���U�[�h�t���[�Y���v" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,				},/* "�����u�M���̕������l�`�v"�̃��C�t */
	// 1.00
	{	 s_hp(7),	 s_time(60),	"�@�����u�����̃}�g�����[�V�J�v" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " �@�����u�����̃}�g�����[�V�J�v"�����u�����̉~�J�l�`�v */ 	//(r33p)boss_move_04_xy_douki_differential
	{	 s_hp(8),	 s_time(60),	"�@�����u�M���̃}�g�����[�V�J�v" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " �@�����u�M���̃}�g�����[�V�J�v"�����u�����̉~�J�l�`�v */ 	//(r33p)boss_move_04_xy_douki_differential
	{	 s_hp(8),	 s_time(60),	"�@�����u�����̃}�g�����[�V�J�v" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " �@�����u�����̃}�g�����[�V�J�v"�����u�����̉~�J�l�`�v */ 	//(r33p)boss_move_04_xy_douki_differential
	{	 s_hp(8),	 s_time(60),	"�@�����u���Q�̃}�g�����[�V�J�v" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " �@�����u���Q�̃}�g�����[�V�J�v"�����u�����̉~�J�l�`�v */ 	//(r33p)boss_move_04_xy_douki_differential
	// 0.--
	{	 s_hp(8),	 s_time(60),	NULL,/* "�ʏ�U��z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " �@�@�@�@�����u�W����̈ĎR�q�v"�����u�����̉~�J�l�`�v */
	{	 s_hp(8),	 s_time(60),	NULL,/* "�ʏ�U��z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " �@�@�@�@�@�����u���̋@�I�l�`�v"�����u�����̉~�J�l�`�v */
	{	 s_hp(8),	 s_time(60),	NULL,/* "�ʏ�U��z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " �@�@�@�@�@�@�����u�Â̗������v"�����u�����̉~�J�l�`�v */
	{	 s_hp(8),	 s_time(60),	NULL,/* "�ʏ�U��z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " �@�����u��̃N���b�V���_�~�[�v"�����u�����̉~�J�l�`�v */
//									"eeddccbbaa99887766554433221100",'\n\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	#if (1)/*(r35�����ƃn���O�A�b�v����H)*/
	{	 s_hp(8),	 s_time(60),	NULL,/* "�ʏ�U��z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " �@�@�@�@�����u�W����̈ĎR�q�v"�����u�����̉~�J�l�`�v */
	{	 s_hp(8),	 s_time(60),	NULL,/* "�ʏ�U��z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " �@�@�@�@�@�����u���̋@�I�l�`�v"�����u�����̉~�J�l�`�v */
	{	 s_hp(8),	 s_time(60),	NULL,/* "�ʏ�U��z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " �@�@�@�@�@�@�����u�Â̗������v"�����u�����̉~�J�l�`�v */
	{	 s_hp(8),	 s_time(60),	NULL,/* "�ʏ�U��z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " �@�����u��̃N���b�V���_�~�[�v"�����u�����̉~�J�l�`�v */
	#endif
};
//	" �@�@�@�����u�����̕������l�`�v" "\n"
//	" �@�����u�����̃I�����A�\\�l�`�v" "\n" " �@�����u�����̃I�����A���l�`�v"...���݂̕����ł͖����ł��B
//	{	 (1024*16),  s_time(40),	"�@�����u���쐬�̈����ɐl�`L1�v" "\n",	SPELL_00,							NULL,						alice_04_keitai,						},/* �����u�����̉~�J�l�`�v */
//	{	(20000*4),	 s_time(20),	"�����u���쐬�̈ɑ������l�`L2�v" "\n",	SPELL_00,							NULL,						boss_move_02_xy_hidouki,				},/* �����u�󑐂̐l�`�Ă��v */
//	{	 (5000*4),	 s_time(20),	"�@�����u���쐬�̉p�g���l�`L3�v" "\n",	SPELL_00,							NULL,						alice_02_keitai,						},
//	{	 (5000*4),	 s_time(20),	"�@�����u���쐬�̘I�����l�`L4�v" "\n",	SPELL_00,							NULL,						alice_02_keitai,						},
//	{	 (5000*4),	 s_time(80),	"�@�@�����u���쐬�̐����l�`L5�v" "\n",	SPELL_00,							NULL,						alice_02_keitai,						},
//	{	 (5000*4),	 s_time(80),	"�@�����u���쐬�̔����`�l�`L6�v" "\n",	SPELL_00,							NULL,						alice_02_keitai,						},
//	{	 (5000*4),	 s_time(80),	"�@�@�����u���쐬�̔g���l�`L7�v" "\n",	SPELL_00,							NULL,						alice_02_keitai,						},
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
	for (ii=0; ii<length; ii++)/* �S�����ׂ�B */
	{
		s->BOSS_DATA_05_move_jyumyou	= 0xff;
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
	�J�[�h�V�X�e���̃{�X�`�Ԃɓo�^���ꂽ�J�[�h�ԍ����擾���A
//	�����ɃJ�[�h�̎��Ԑ؂��ݒ肷��B
	�����ɃX�y���̏��������s���B

---------------------------------------------------------*/

global void card_maikai_init_and_get_spell_number(SPRITE *src)
{
	card.card_number	= my_card_resource[(card.number)].card_number;	/* �J�[�h���Z�b�g */
	//
	card_maikai_init(src);		/* �J�[�h�̐������Ԃ�ݒ�(�\�߃J�[�h���Ƃɐݒ肳��Ă���W�����Ԃɐݒ�) */
}


/*---------------------------------------------------------
	�{�X�`�ԕύX���̋��ʃ��[�`��
	�J�[�h���j��A�C�e���o���B
---------------------------------------------------------*/

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
		�J�[�h���̕\��
		�J�[�h�w�i�́A�܂��������ǁA�����I�Ɋg���\��B
	---------------------------------------------------------*/
	//static void draw_card_name(void)
	{
		kanji_window_clear_line(ML_LINE_04);	/* �����E�B���h�E��4�s��(==3)�̓��e�������B */
		set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (ML_LINE_04)*(KANJI_FONT_18_HEIGHT_P2));	/* �J�[�\����4�s��(==3)�ֈړ� */
	//
		if (NULL != my_card_resource[(card.number)].spell_str_name)
		{
			/* �J�[�h�w�i������ꍇ */
			cb.callback_gu_draw_haikei = cb.callback_gu_draw_haikei_supeka;
			//
			// ""�J�[�h�̕����\���ʒu�B
			{
			//	ml_font[(ML_LINE_04)].timer 	= ML_ON;	/*"�\��ON"*/
				ml_font[(ML_LINE_04)].timer 	= byou60(5);	/* �� 5 �b */
				ml_font[(ML_LINE_04)].x 		= (96);/* X�ʒu */
				ml_font[(ML_LINE_04)].y 		= (12);/* Y�ʒu */
			//	ml_font[(ML_LINE_04)].render	= NULL;
			//	ml_font[(ML_LINE_04)].str		= my_font_text;/*(�f�o�b�O)*/	/*"�\��on"*/
			}
			ml_font[(0)].haikei 		= (ML_HAIKEI_01_BOSS_SPELL_CARD);/* [��/�{�X�J�[�h�p�w�i]�{�X�J�[�h�p����Ӕw�ion */
			cg.msg_time = byou60(5);	/* �� 5 �b */
			strcpy(my_font_text, my_card_resource[(card.number)].spell_str_name);
			kanji_color(/*int color_type*/(7)|STR_CODE_NO_ENTER);	/* ���s���Ȃ� */
			kanji_draw();
		}
		else
		{
			/* �J�[�h�w�i�������ꍇ */
			cb.callback_gu_draw_haikei = cb.callback_gu_draw_haikei_modosu;
		}
	}
	#endif
	/*--- BOSS ���ʂ��Ēl�� 0 �ɂ���B------------*/
	src->BOSS_DATA_05_move_jyumyou = (0);
//	src->BOSS_DATA_05_boss_base_state777 = (0);/* ���ʂ��� �l�� 0 �ɂ��� */
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
	//	limit_max_hp_dec_boss_by_flame = (((test_draw_boss_hp_value>>(6))+3)<<3);
	//	limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>3)+(3<<3);
	//	limit_max_hp_dec_boss_by_flame = (((test_draw_boss_hp_value>>(6))+1)<<4);
	//	limit_max_hp_dec_boss_by_flame = (((test_draw_boss_hp_value>>(6)))<<4)+(1<<4);
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
	SPRITE *obj_boss;
	obj_boss			= global_obj_boss;
//	obj_boss			= �����蔻��̓s���㖳��&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
	{
		int limit_max_hp_dec_boss_by_flame;
		u8 test_draw_boss_hp_value;
	//	test_draw_boss_hp_value = (obj_boss->base_hp>>5) & 0xff;	/* 8192/32 == 256 (r32) */
		test_draw_boss_hp_value = (obj_boss->base_hp>>7) & 0xff;	/* 32768/32 == 256 (r33) */
		#if 1
	//	limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>2) | (0x0f);
	//	limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>2) | (0x08);
		limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value) | (0x10);
		#endif
		//
		if (limit_max_hp_dec_boss_by_flame < card.boss_hp_dec_by_frame)
		{
			card.boss_hp_dec_by_frame = limit_max_hp_dec_boss_by_flame;
		}
	}
//
	obj_boss->base_hp -= card.boss_hp_dec_by_frame;
	card.boss_hp_dec_by_frame = 0;/* �g�����̂ŏ���(�t���[���P��) */
	if (card.limit_health >= obj_boss->base_hp) 	/* �K��l�ȉ��ɂȂ�΃J�[�h���[�h���� */
	{
		card.mode			= (CARD_BOSS_MODE_00_OFF);
		boss_destroy_check_type(obj_boss, DESTROY_CHECK_00_WIN_BOSS);
		cb.callback_gu_draw_haikei = cb.callback_gu_draw_haikei_modosu;
	}
}


/*---------------------------------------------------------
	�J�[�h�����ɐi�߂�B
---------------------------------------------------------*/
static void card_incliment(void)
{
			card.number += (4)/*1*/;
}

/* �o����x���W */
//#define BOSS_XP256		(t256(GAME_WIDTH/2))	/* ���S���W�Ȃ̂� */
#define BOSS_XP256			(t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH/2))	/* ���S���W�Ȃ̂� */

/*---------------------------------------------------------
	[�J�[�h�V�X�e�����Ɉړ��\��]	�J�[�h�o�^
---------------------------------------------------------*/

/*global*/global/*static*/ void card_boss_move_generate_check_regist(SPRITE *src)
{
	if ( /*off*/(0)==card.mode)/*(���f)*/
	{
		card.mode		= (CARD_BOSS_MODE_01_IDO_JYUNNBI);/*on*/	/* �Ƃ肠���� */
		/* [(�Ƃ肠����)�J�[�h�U���݂̂Ɏd�l�ύX]�����̂ŁA�Ō㌂�Ă�J�[�h���Ȃ��Ȃ����ꍇ�ɍU��������ׁB */
		{
			int aaa;	/* ���ݑ̗� ���� ���������J�[�h�� �������̗͒l */
			aaa = card.limit_health - (my_card_resource[(card.number+(4)/*1*/)].spell_life);	/*1000 500*/
		//	card.limit_health -= 1000/*500*/;
			/* �{�X���J�[�h�����Ă���̗͂�����ꍇ */
			if (0 < aaa)	/* �J�[�h���Ă�B */
			{
				card.limit_health = aaa;
			//	if (card_syoji_maisuu < card.number)
				if (0 == card_syoji_maisuu )
				{
					/* �`�ԕύX���Ȃ��A�A�C�e���f���Ȃ� */
				//	card.number -= (4);
				//	src->callback_loser 			= lo se_boss;
				}
				else/* �`�ԕύX������ */
				{
					card_incliment();
					card_syoji_maisuu--;
					#if 0//(1==TEST_ZAKO_HIDE)/* �{�X���e���󂯂� */
					zako_all_timeup();/* �U�R�^�C���A�E�g(�t�F�C�h�A�E�g���������ֈڍs) */
					#endif
					/* �A�C�e���f�� */
					if (NULL != src->callback_loser)
					{
						(src->callback_loser)(src); 	/* sakuya_put_items(src); */
					}
				}
			}
			else	/* �J�[�h���ĂȂ��B */
			{
				card.limit_health = (0);
			//	card.mode		= (0);/*off*/
			}
		}
		/*---------------------------------------------------------
			�J�[�h�`�F�b�N
		---------------------------------------------------------*/
		if ( /*off*/(0)==card.boss_timer)
		{
			card.limit_health = (0);
		}
	}
	else
	if (CARD_BOSS_MODE_01_IDO_JYUNNBI == card.mode)
	{
		card.mode			= (CARD_BOSS_MODE_02_TAIHI);/*on*/


		REG_10_BOSS_SPELL_TIMER = (0);	/* �J�[�h�����������I�Ɏ~�߂�B */
		bullets_to_hosi();		/* ���Ă̓G�e���Ahosi�A�C�e���ɕς��� */
		/* �^���t�߂ɑޔ� */
	//	src->BOSS_DATA_04_toutatu_wariai256 			= t256(  0);/* �������ς݂̕K�v���� */
		src->BOSS_DATA_04_toutatu_wariai256 			= t256(1.0);/* �������ς݂̕K�v���� */
	//	src->BOSS_DATA_00_target_x256			= t256(153);
		src->BOSS_DATA_00_target_x256			= BOSS_XP256; //t256(0);
	//	src->BOSS_DATA_01_target_y256			= src->cy256;
		src->BOSS_DATA_01_target_y256			= t256(16.0); //t256(0);

		#if 1/* (�V[�J�[�h�n�܂�O�ɏ�����]) ��0�`�Ԃ���A�K���ĂԔ��B */
		/* �J�[�h������ */
		if (NULL != my_card_resource[(card.number)].spell_init_callback)
		{
			(my_card_resource[(card.number)].spell_init_callback)(src);
		}
		#endif
	}
	else
	if (CARD_BOSS_MODE_02_TAIHI==card.mode) /* ���e�ʒu�܂ňړ����B */
	{
	//	src->BOSS_DATA_04_toutatu_wariai256 -= (1); 	/* [��	4 �b]== 4.2666==(256/60[flame]) */
	//	src->BOSS_DATA_04_toutatu_wariai256 -= (1*4);		/* [��	1 �b]== 1.0666==(256/(4*60)[flame]) */
//		src->BOSS_DATA_04_toutatu_wariai256 -= (1*(4-1));	/* [��	1 �b]== 1.0666==(256/(4*60)[flame]) */
		#if 1/*(r32)*/
	//	boss_yuudou_idou_nomi(src);/*(r32)*/
		boss_yuudou_hiritu_keisan(src);
	//	src->BOSS_DATA_04_toutatu_wariai256 -= (1); /* [��	4 �b]== 4.2666==(256/60[flame]) */
		src->BOSS_DATA_04_toutatu_wariai256 -= (1*4);	/* [��	1 �b]== 1.0666==(256/(4*60)[flame]) */
		if (0 > src->BOSS_DATA_04_toutatu_wariai256 )	/* �قډ�ʒ��S�t�߂܂ŁA�ړ������B */
		{
			src->BOSS_DATA_04_toutatu_wariai256 = (0);
			card.mode		= (CARD_BOSS_MODE_03_HATUDAN);/*on*/
		}
		#endif/*(r32)*/
		#if 0/*(r32p)*/
		boss_yuudou_idou_nomi(src);/*(r32p)*/
		boss_yuudou_hiritu_keisan(src);
	//	src->BOSS_DATA_04_toutatu_wariai256 -= (1); /* [��	4 �b]== 4.2666==(256/60[flame]) */
	//	src->BOSS_DATA_04_toutatu_wariai256 -= (1*4);	/* [��	1 �b]== 1.0666==(256/(4*60)[flame]) */
	//	if (0 > src->BOSS_DATA_04_toutatu_wariai256 )	/* �قډ�ʒ��S�t�߂܂ŁA�ړ������B */
		if ((0==src->BOSS_DATA_04_toutatu_wariai256))
		{
	//		src->BOSS_DATA_04_toutatu_wariai256 = (0);
			card.mode		= (CARD_BOSS_MODE_03_HATUDAN);/*on*/
		}
		#endif/*(r32p)*/
	}
	else/* [(�Ƃ肠����)�J�[�h�U���݂̂Ɏd�l�ύX]*/
	/*
		(�������̈ړ��Ȃǂ��J�[�h����)
		(�J�[�h�����Ȃ����������)
	*/
	/* �{�X�s�� */
	if (CARD_BOSS_MODE_03_HATUDAN==card.mode)/*(�J�[�h��)*/
	{
		(my_card_resource[(card.number)].boss_move_keitai_callback)(src);/*(�{�X�ړ��`�Ԗ��ɁA�{�X�ړ����������s����)*/
	}
}


/*---------------------------------------------------------
	[�J�[�h�V�X�e�����Ɉړ��\��]
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
	set_REG_DEST_XY(tama);/* ���e�ʒu ���Wxy */
	bakuhatsu_add_type_ccc(BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/* ��Ɏ��s������������ */
//
	/* ��ƕ������������R�[�h������������B */
	{
		card.boss_hp_dec_by_frame += /*w->*/tama->base_weapon_strength; /* �U�����đ̗͌��炷(����������) */
	}
}

//static int kaiwa_syuuryou;	/* �{�X�퓬�O�̉�b�I����҂� */
global /*static*/ void kaiwa_00_keitai(SPRITE *src)
{
//	if (0!=kaiwa_syuuryou)	/* �{�X�퓬�O�̉�b�I����҂� */
	if ( ((cg.state_flag) & STATE_FLAG_05_IS_BOSS) )	/* �{�X�퓬�O�̉�b�I��? */
	{
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);
	//	common_boss_init_2nd(src);	/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
		/*---------------------------------------------------------
			�{�X�̋��ʁA�Q��ڏ��������[�`��(�U���\)
		---------------------------------------------------------*/
	//	/*extern*/static void common_boss_init_2nd(SPRITE *src)
		{
			/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
			src->callback_hit_teki = s_callback_hit_boss;	/* �R�[���o�b�N�o�^ */
			/* card common init */
			card.mode		= (CARD_BOSS_MODE_01_IDO_JYUNNBI);/*on*/
		//	card.mode		= (0);/*off*/
		}
	//	/*���Ԑ����J�E���g�L����*/
	//	data->boss_base.state001++/* = ST_02*/;
		card_incliment();
	}
}
//global void called_from_kaiwa_system_boss_start(void)
//{
//	cg.state_flag |= STATE_FLAG_05_IS_BOSS; 	/* �{�X�퓬�O�̉�b�I����ݒ� */
//	kaiwa_syuuryou = (1);
//}
/*---------------------------------------------------------
	���ʃ{�X�ޔ�(���j��ɉ�ʊO�Ƀ{�X��������)
	[�J�[�h�V�X�e�����Ɉړ��\��]
---------------------------------------------------------*/

global /*static*/ void common_99_keitai(SPRITE *src)
{
	if (0 > (src->cy256+t256(50.0)))/* +t256(50.0) �{�X�O���̍ő�T�C�Y(50[dot]) */
	{
		bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
		src->callback_mover 	= NULL; 		/* �����܂� */
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
		if ( (int)(GAME_X_OFFSET*256+(GAME_320_WIDTH*256/2)) > src->cx256)	/* ��ʔ����̈ʒu */
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
	�J�[�h���I���܂ő҂B(�p�~)
	-------------------------------------------------------

---------------------------------------------------------*/
#if 0
	/*static*/global void card_state_check_holding(SPRITE *src)
	{
		if (SPELL_00 == card.card_number)
		{
			src->BOSS_DATA_05_boss_base_state777++;
		}
	}
#endif


/*---------------------------------------------------------
	�{�X�̋��ʁA�P��ڏ��������[�`��(����A�U���s��)
---------------------------------------------------------*/


/*---------------------------------------------------------
	�e�͈̔͂��u�W���v�ɐݒ�
---------------------------------------------------------*/
global void set_default_bullet_clip(void)/* call from load_stage.c */
{
	rect_clip.bullet_clip_min.x256 = t256(GAME_X_OFFSET);
	rect_clip.bullet_clip_max.x256 = t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH);
	rect_clip.bullet_clip_min.y256 = t256(-32);
	rect_clip.bullet_clip_max.y256 = t256(GAME_HEIGHT);
}


/*---------------------------------------------------------
	�e�͈̔͂��u��Ɖ��̍L�͈́v�ɐݒ�
---------------------------------------------------------*/
global void set_aya_bullet_clip(void)/* call from load_stage.c */
{
	rect_clip.bullet_clip_min.x256 = t256(GAME_X_OFFSET)						+ t256(-100);
	rect_clip.bullet_clip_max.x256 = t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH) + t256( 100);
	rect_clip.bullet_clip_min.y256 = t256(-256);/* ���� */
	rect_clip.bullet_clip_max.y256 = t256(GAME_HEIGHT);//+t256(100)
}

global void init_00_boss_clip000(SPRITE *h)/* �W���^�C�v */
{
	set_default_bullet_clip();	/* �e�͈̔͂�W���ɐݒ� */
}
global void init_00_boss_clip111(SPRITE *h)/* ��ɍL���^�C�v */
{
	set_aya_bullet_clip();	/* �e�͈̔͂�W���ɐݒ� */
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
//	#if (0==U SE_BOSS_COMMON_MALLOC)
//	h									= obj_add_01_teki_error();
//	obj_boss							= h;/*�P��{�l*/
//	#else
//	SPRITE *obj_boss;
//	obj_boss = �����蔻��̓s���㖳��&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
//	SPRITE *h;
//	h = �����蔻��̓s���㖳��&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
//	#endif


/*static*/extern  void boss_effect_reset(void);
extern void stage_boss_load_texture(void);



// src/core/douchu/boss.h �̏��������Q�Ƃ��鎖�B
global void called_from_kaiwa_system_boss_load(int boss_number)
{
	{
		/* boss_rect_init */
		rect_clip.boss_clip_min.x256	= t256(GAME_X_OFFSET)+t256( 			0)+t256(24);
		rect_clip.boss_clip_max.x256	= t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH)-t256(24);
		rect_clip.boss_clip_min.y256	= t256(0);
		rect_clip.boss_clip_max.y256	= t256(96);
	}
//
	int sss;
	sss = boss_number;
	sss &= (8-1);
//
	//----[BOSS]
	SPRITE *h;
	h					= global_obj_boss;/*�P��{�l*/
//	h					= �����蔻��̓s���㖳��&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* �����o�^ */		/* �d�l�o�O(?) */
	{
	/*(�Ē�`�̕K�v����)*/	h->type 						= BOSS_00_11;
		h->callback_mover				= common_boss_move;
		h->callback_hit_teki			= NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
	//
		#if 0/* �������ς݂̕K�v���� */
		h->vx256						= t256( 0);
		h->vy256						= t256( 0);
		#endif
	//
		h->BOSS_DATA_03_kougeki_anime_count 		= (0);	/* �U���A�j���[�V�����p�J�E���^ / 0�ȉ��Ȃ�ړ��A�j���[�V���� */
		h->BOSS_DATA_05_boss_base_state777			= (0);	/*ST_00*/	/*�����l��0�ɂ���B*/
	//	h->BOSS_DATA_05_boss_base_state777			= (-1); /*ST_00*/
	//
	//------------ �J�[�h�֘A
		{
			static const u16/*u8*/ aaa[8+8] =/* �J�[�h�A�h���X��256��ނ𒴂����̂ŁAu16 */
			{
			/* �{�X�J�n�J�[�h�ԍ�(easy) */
				CARD_ADDRESS_00E_chrno_000, 	/* �`���m(?) */ 	/* �G�L�X�g���p(boss0) */
				CARD_ADDRESS_00E_alice_000, 	/* �A���X */
				CARD_ADDRESS_00E_mima_000,		/* ���� */
				CARD_ADDRESS_00E_kaguya_000,	/* �P�� */
			//
				CARD_ADDRESS_00E_aya_000,		/* �� */
				CARD_ADDRESS_00E_pacheA_000,	/* �p�`�F */
				CARD_ADDRESS_00E_sakuya_000,	/* ��� */
				CARD_ADDRESS_00E_pacheA_000,	/* �t����(?) */ 	/* �t�@���^�Y���p(boss7) */
			/* �{�X�J�[�h������ */
				(6),//(0-0),													/* �`���m(?) */ 	/* �G�L�X�g���p(boss0) */
				(5),//(CARD_ADDRESS_16E_alice_ggg-CARD_ADDRESS_00E_alice_000),		/* �A���X */		/*CARD_ADDRESS_19_alice_jjj;*/
				(5),//(CARD_ADDRESS_17E_mima_hhh-CARD_ADDRESS_00E_mima_000),		/* ���� */
				(6),//(CARD_ADDRESS_17E_kaguya_hhh-CARD_ADDRESS_00E_kaguya_000),	/* �P�� */
			//
				(6),//(CARD_ADDRESS_19E_aya_jjj-CARD_ADDRESS_00E_aya_000),			/* �� */
				(8),//(CARD_ADDRESS_17E_pache_hhh-CARD_ADDRESS_00E_pache_000),		/* �p�`�F */
				(9),//(CARD_ADDRESS_11E_sakuya_kkk-CARD_ADDRESS_00E_sakuya_000),	/* ��� */
				(5),//(0-0),													/* �t����(?) */ 	/* �t�@���^�Y���p(boss7) */
			};
			card.number 			= aaa[sss  ] + ((cg.game_difficulty)&0x03)
				+ ((5!=sss)?(0):(((cg_game_select_player)&(4-1))<<(2+3)));/* ��Փx(2bit==4�i�K)�A�J�[�h(3bit==8�i�K) */
			card_syoji_maisuu		= aaa[sss+8];
		}
		{
	/*(�Ē�`�̕K�v����)*/	h->base_hp				= (my_card_resource[(card.number)].spell_life); 		/* �S�̗̂̑� */
			card.limit_health = (h->base_hp) - (my_card_resource[(card.number + (4)/*1*/)].spell_life); 	/* �ʏ�U��(����U��)�̍U���� */
			//
			card.boss_timer = (((my_card_resource[(card.number)].spell_limit_time)));	/* 75*64==75[count] 	��99[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
			card.mode		= (CARD_BOSS_MODE_01_IDO_JYUNNBI);/*on*/	/* ����H */
			/*???*/
		//	kaiwa_syuuryou = (0);
		}
		#if 1
	//------------ �J�[�h�֘A
		card.card_number			= SPELL_00; 	/* �J�[�h�����I���t���O */
		card.number_temporaly	= (0);			/*(SPELL_08_rumia-1)*/ /*0*/
		#endif
	//
	}
	boss_effect_reset();
	// �{�X�e�N�X�`���ǂݍ���
	stage_boss_load_texture();
}
