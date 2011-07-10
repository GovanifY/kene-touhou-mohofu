
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _JIKI_H_
#define _JIKI_H_

// > �yPSP�zPSP�œ�����肽���z8�y�e���z
// > 200 �F���������񁗂��������ς��B�F2009/01/27(��) 18:41:00 ID:xQgI7hCU
// > ���낻��S�A�C�e�������ė~�����ȁB
// > ����FULL�p���[���̃A�C�e�������񂹃��C���͂���������Ɖ��̕��������Ǝv���B
// > ��̓I�ɂ͂��̕ӂ聫
// > ttp://www4.uploader.jp/user/isisuke/images/isisuke_uljp00081.bmp
// ���̈ӌ��͂킽�������l�Ɏv�����̂ŁA�݂�Ȃ������v�����ɈႢ�Ȃ��B
// ���̉摜��484x283[dots]�ŁAy==63[line]�ɐԐ��������Ă������B
// psp�̉𑜓x�́A 480x272[dots]�Ȃ̂� x/272=63/283, x=(63*272)/283 == 60.55[line]
// ����/���쎁	50(49)[line] �����シ����B
// �X��8��200�� ��60[line] (���o�I��)
// �����ŃL�����̍����������邵�A64(63)�������Ɖ�72(71)���炢�ł������̂ł́H�Ƃ������64�ɂ��Ď������Ă݂�B
// ���ʁF����ς����������������B8[dots]�Ƃ��킸16[dots]���炢���������B�Ăю������Ă݂�B
// ���ʁF����Ȃ��񂩂ȁH�Ƃ����킯��80(79)[dots]�Ɍ���B����8[dots]��88(87)�ł����������ˁB
// �{�Ƃ̊��o��厖�ɂ���Ȃ炱��Ȃ��񂾂Ǝv���B
//---------------
//�u�A�C�e�������񂹃��C���v�ȉ��Ŏ��Ɓu�A�C�e�������񂹃��C���v����322���C���܂ŋ����Ō��_�B
//
//�v���C�t�B�[���h�� 384x448 �Ȃ̂ŁA448-322 == 126
//�u�A�C�e�������񂹃��C���v�͑����ォ��128�h�b�g���ȁH
//
//(B�A�C�e���̑傫����16x16[dot]�Ȃ̂ŁAB�A�C�e���̏�������� 448+(16/2)���C���ōs������A
//�Ō��B�A�C�e���̔��肪����̂��AB�A�C�e���̂x�����S��456[���C��]�ɂ����ꍇ�B
//456-322 == 134, 134-128 == 6. ����6[���C��]�͂����������@���S����ʉ����ɍs���Ȃ��������h�b�g(5[dot?])����B
//�c��1[dot]�͕s�����Ŕ��肵������? )
//---------------
// �������䗦�Ȃ�A
// (�͕핗�A�C�e�������񂹃��C��)/(�{�ƃA�C�e�������񂹃��C��) == (�͕핗�Q�[���t�B�[���h�c)/(�{�ƃQ�[���t�B�[���h�c)
// ����������B(�͕핗�A�C�e�������񂹃��C��)���m�肽���̂�����ڍs���āA
// (�͕핗�A�C�e�������񂹃��C��) == (�{�ƃA�C�e�������񂹃��C��) * (�͕핗�Q�[���t�B�[���h�c)/(�{�ƃQ�[���t�B�[���h�c)
// (�͕핗�A�C�e�������񂹃��C��) == 128 * 272/448 == 77.7142857142857142857142857142857 =:= 78[dots] �P���Ȕ䗦�v�Z�̏ꍇ�B
// http://hossy.info/game/toho/k_score.php �����̉摜���t�Z���Čv�Z����� 77 dots ������B
/* �A�C�e�������񂹃��C�� */
//#define ITEM_GET_BORDER_LINE256		(t256(77))/* �摜���牼�� */
//#define ITEM_GET_BORDER_LINE256		(t256(78))/* �{�ƂƓ��䗦�Ɖ��肵�t�Z */
#define ITEM_GET_BORDER_LINE256 		(t256(80))/* ���S���W�ŊǗ����ĂȂ��z(-r31) */
/* ����(r33) �v���C���o�Ƃ��Ă���3��(77, 78, 80)�̒��ł� (80)����Ԗ{�Ƃɋ߂��B(77, 78)�͏�߂���B
���܂艺�ɂ��Ă��ʔ����Ȃ�����A(80)���x���Ó��ȋC������B */

/* �G�������Ȃ����C�� (���̃��C����艺����͓G�������Ȃ�) */
#define ENEMY_LAST_SHOT_LINE256 	(t256(272-72))


enum /*_select_player_*/
{
	/*0*/	REIMU_A = 0,
	/*1*/	REIMU_B,
	/*2*/	MARISA_A,
	/*3*/	MARISA_B,
//
	/*4*/	REMILIA,
	/*5*/	YUYUKO,
	/*6*/	CIRNO_A,
	/*7*/	CIRNO_Q,
//
	MAX_08_PLAYER
};

/*(�S�t���O����������@�\)*/
#define JIKI_FLAG_0x00_ALL_OFF							(0x0000)
//--------
/*(���荞�݃V�X�e��(r36�V�K)�p�Ɋm��)*/
#define II_SYSTEM_TASK_NUMBER							(0x000f)
//--------
/* �v���C���[���㕔�ֈړ����A�蓮�ŃA�C�e�����W�s���ꍇ��on�B */
#define JIKI_FLAG_0x01_JYOU_BU_SYUU_SYUU				(0x0010)
/* �{�������������ۂɁA�����ŃA�C�e�����W���s���ꍇ��on�B */
#define JIKI_FLAG_0x02_BOMBER_SYUU_SYUU 				(0x0020)
/* �{�X�|����ɓ_��(���A�C�e��)��{�X���o���A�C�e���������Ŏ��W����B���̏ꍇ��on�B */
#define JIKI_FLAG_0x04_BOSS_GO_ITEM_JIDOU_SYUU_SYUU 	(0x0040)
//-------
/* �� [�v���C���[�̃I�v�V�������V���b�g�����ꍇ�̒ʐM�p�t���O] */
#define JIKI_FLAG_0x08_SHOT_KEY_SEND_FOR_OPTION 		(0x0080)
/* 1==�I�v�V�������V���b�g�����B */
/* 0==�I�v�V�������V���b�g�������Ȃ��B */
//-------
/* �Q�[�����A��b���[�h�� on �ɂȂ�B�Q�[�����ĊJ������ off �ɂȂ�B*/
#define STATE_FLAG_06_IS_KAIWA_MODE 					(0x0100)
/* on */
/* �{�X��|�����ꍇ��on(���Ԑ؂�ł�OFF) */
//#define STATE_FL AG_09_IS_WIN_BOSS					(0x0200)
/* on */
#define STATE_FLAG_10_IS_LOAD_KAIWA_TXT 				(0x0400)
/* on */
#define STATE_FLAG_12_END_KAIWA_MODE					(0x0800)
/* on */
//-------
#define STATE_FLAG_13_DRAW_BOSS_GAUGE					(0x1000)

#define STATE_FLAG_14_DOUCHU_TUIKA						(0x2000)/* �����ŃR�}���h�ǉ����������s���ꍇon */

/* �{�X�Ɛ키�ۂ�on(�{�X�O�C�x���g�ł�OFF) */
#define STATE_FLAG_05_IS_BOSS							(0x4000)

#define STATE_FLAG_16_GAME_TERMINATE					(0x8000)/* result��ɃQ�[���I�� */


//#define ST ATE_FLAG_11_IS_BOSS_DESTROY				(0x0400)/* �p�~ */

//#define STATE_FLAG_0123_AUTO_GET_ITEM (JIKI_FLAG_0x01_JYOU_BU_SYUU_SYUU|JIKI_FLAG_0x02_BOMBER_SYUU_SYUU|JIKI_FLAG_0x04_BOSS_GO_ITEM_JIDOU_SYUU_SYUU)
//#define ST ATE_FLAG_14_GAME_LOOP_QUIT 				(0x2000)
//#define ST ATE_FLAG_16_NOT_ALLOW_KEY_CONTROL			(0x8000)/* �p�~ */


//	/*extern*/global void *callback_gu_draw_haikei_modosu;	/* (�J�[�h������ʏ펞��)�߂��p */
//	/*extern*/global void *callback_gu_draw_haikei_supeka;	/* �J�[�h���p */
//	global void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;
//	extern void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;				/* �w�i�E�B���h�E�\���t���O */
enum
{
	SDL_00_VIEW_SCREEN		= 0,
	SDL_01_BACK_SCREEN,
//	SDL_02_TEX_SCREEN,
//	SDL_03_000_SCREEN,
	SDL_99_MAX_SCREEN,
};

typedef struct _game_core_global_callback_
{
	void (*main_call_func)(void);/* ���C���R�[���o�b�N */
	void (*return_call_func)(void);/* �|�[�Y���A��̖߂��R�[���o�b�N */
	/* �J�[�h���łȂ��ʏ펞�̃v���O�������w�� */
	void *callback_gu_draw_haikei_modosu;	/* (�J�[�h������ʏ펞��)�߂��p�B(==�ʏ펞) */
	/* �J�[�h���̃v���O�������w�� */
	void *callback_gu_draw_haikei_supeka;	/* �J�[�h���p */

	/* ���ݎg�p����v���O������ݒ� */
	void (*callback_gu_draw_haikei)(void);	/* �Q�[�����A�w�i�E�B���h�E�\���t���O */
/*
	������`�悷��`���̃e�N�X�`���[
*/
		#if (0==USE_32BIT_DRAW_MODE)
	u16 *kanji_window_screen_image; 					/* ���b�Z�[�W�E�B���h�E(�摜) */
		#else
	u32 *kanji_window_screen_image; 					/* ���b�Z�[�W�E�B���h�E(�摜) */
		#endif
//extern SDL_Surface *screen;
/*extern*/ SDL_Surface *sdl_screen[SDL_99_MAX_SCREEN];/*(4)*/

} GAME_CORE_GLOBAL_CALLBACK;
extern GAME_CORE_GLOBAL_CALLBACK cb;
typedef struct _game_core_global_class_
{
	int state_flag; 	/* �ݒ�t���O(�W) */
	int weapon_power;	/* 0x00-0x80  (0-128 ��129�i�K==�{�ƂƓ���)   max==128==�u129�i�K�v*/
	int chain_point;
	int bomber_time;	/* Use Gu */
//[0x10 4]
	u32 game_score; 	/* �X�R�A���_ */
	u32 graze_point;	/* �O���C�Y���_ */
	int bombs;			/* �{���� */
	int zanki;			/* �c��`�����X */
//[0x20 8]
	s32 game_difficulty;/* cg.game_difficulty: GAME_CORE_GLOBAL_CLASS�ɓ����Ƒ��x�ቺ����B(psp��max,min���߂��g���Ă���̂� signed int �̕K�v������ ) */
	s32 game_rank;		/* �����N�ϓ��V�X�e���̃Q�[�������N */
	/*u8*/unsigned int jiki_weapon_level_offset;/* �����������탌�x��(�ԍ�)�̂W�{�Ƀv���C���[�l�𑫂����l���I�t�Z�b�g�Ƃ��ĕێ� */
	unsigned int conv_bg_alpha;
//[0x30 12]
	int current_bg0_y_scroll_speed256;	/* bg0�̃X�N���[���A���ݑ��x */
	/*(�`��p)*/
	int draw_boss_hp_value; /* �{�Xhp�`��l */ ///??????????
	int dummy111;/*�����ƍI�����삵�Ȃ�*/
//	/* �W�v�V�X�e��(player_data) */
	int player_data_use_continue;	/* �R���e�B�j���[�� */
//[0x40 16]
	int player_data_count_miss; 	/* �~�X�� */
	int player_data_used_bomber;	/* �{���g�p�� */
	int player_data_use_kurai_bomb; /* ��炢�{�������� */
	int player_data_count_bonus;	/* �J�[�h�{�[�i�X�� */
//[0x48 18]
	#if (1)/*(���Ή�)*/
	/* �b�� SDL �t�H���g */
	int SDL_font_type;
	int SDL_font_x;
	int SDL_font_y;
//[0x60 24]
//	�b�� [���C�������_�[�Ɉڍs�\��B]
	int msg_time;/* ���b�Z�[�W(���Ή�)�\������ */	/* ����ӃE�B���h�E�\������(��) */
	#endif
//[0x50 20]
//	���̏ꏊ�� 4[byte]���E�ł̓_���ŁA���̂�32[byte]���E�ɍ��킹�Ȃ��Ɛ��퓮�삵�Ȃ��B(4[byte]���E�łُ͈퓮�삷��)
	s8 game_now_stage;				/* ���݃X�e�[�W�ԍ� */
	s8 game_continue_stage; 		/* ���݃R���e�B�j���[����X�e�[�W�ԍ���ێ� */
	s8 game_now_max_continue;		/* �R���e�B�j���[�\�ȉ� */
	s8 game_practice_mode;			/* ���K���[�h */
//
	s8 side_panel_draw_flag;/* �p�l���\��on(0�ȊO)/off(0) */
	s8 chuu_boss_mode;
	s8 dummy_status;				/* [���g�p]�_�~�[ */
	u8 player_option_mode;/* �����ɓ���Ȃ��������������H�A�Ƃ肠����1575659 */
	int dummy2222;/*�����Ɖ��̂��I�����삵�Ȃ�*/
	int dummy3333;/*�����Ɖ��̂��I�����삵�Ȃ�*/
} GAME_CORE_GLOBAL_CLASS;
extern GAME_CORE_GLOBAL_CLASS cg;
// /* �\��2(.align���킹�ŕK�v) */
//	s8 dr aw_flag_ka iwa_screen;		/* ��b��ʂ̉�b�p����ӃE�B���h�E�\���t���O */



//
//	u8 laser_mode;	/* �Ă���(���[�U�[���[�h 0:off, 1:on) */
	/* �W�v�V�X�e���ȊO�̕ێ���� */
//	u8 game_select_player;
//�x�߂���B	u8 game_difficulty/* = RANK_EASY*/; 	/*	RANK_NORMAL*/
//�x�߂���B	int game_difficulty;







/* �Ӑ}�I�ɓ���Ȃ����� */
extern unsigned int cg_game_select_player;/* cg.game_difficulty: (�����͂Ƃ�����)����(r33)�� GAME_CORE_GLOBAL_CLASS�ɓ���Ȃ������ǂ����ۂ��B */

	#if (1)/*(r34)*/
typedef struct _psp_pad_global_class_
{
	u32 pad_data;						/*�������*/
	u32 pad_data_alter; 				/*�O�����*/
	s16 analog_absolute_value_x;		/* �A�i���O�ʁA�␳�ς� */
	s16 analog_absolute_value_y;		/* �A�i���O�ʁA�␳�ς� */
} PSP_PAD_GLOBAL_CLASS;
extern PSP_PAD_GLOBAL_CLASS psp_pad;
	#endif

#if 1

	#if (0)/* �A���C�����g�֌W(???) (s16)�� GAME_CORE_GLOBAL_CLASS�ɓ����ƍI�������Ȃ� */
	/* �Ӑ}�I�ɓ���Ȃ����� */
	extern	u32 cg_m y_pad; 		/*�������*/
	extern	u32 cg_m y_pad_alter;	/*�O�����*/
	extern	s16 cg_analog_x;		/* �A�i���O�ʁA�␳�ς� */
	extern	s16 cg_analog_y;		/* �A�i���O�ʁA�␳�ς� */
	#endif
//
//??????????extern int draw_boss_hp_value;	/* �{�Xhp�`��l */
//extern int bo ss_life_value;	/* �{�Xhp�̗͒l / �{�X�����w�T�C�Y�`��l */
#endif
extern int boss_x256;
extern int boss_y256;


#define USE_HOLD_GAME_MODE	(0)
#if (1==USE_HOLD_GAME_MODE)
extern void hold_game_mode_on(void);/* �Q�[�����Ԃ̈ꎞ��~ */
extern void hold_game_mode_off(void);/* �Q�[�����Ԃ̓���J�n */
#endif

//#define IS_KEY BOARD_PULLED ((0==psp_pad.pad_data)&&((psp_pad.pad_data)^(psp_pad.pad_data_alter)))/* �����L�[�𗣂��ꂽ�� */
/*	�L�[�𗣂����u�Ԃ��m��Ƃ���B�������u�Ԃ��m�肾�ƁA�����ꂽ�Ԃ������삷�镨�������ɂ���Ɠs���������B */
/*
	r31:����ł̓L�[���������u�Ԃ��m��̂��̂������̂ŁA���������Ƃ���̓L�[���������u�Ԃ��m��ɏC�������B
*/

/* 127 ���Ǝv���Ă������ǁA128�݂���(�܂�129�i�K) */
//#define MAX_POWER_IS_128 (127)/* 0x00-0x7f  (0-127 ��128�i�K) */
//#define MAX_POWER_IS_128 (128)/* 0x00-0x80  (0-128 ��129�i�K) */
#define MAX_POWER_IS_128 (128)

//typedef struct
//{
//	int strength;
//} WEAPON_BASE;

//#define DEFAULT_MAX_CONTINUE	(3)
//#define DEFAULT_MAX_CONTINUE	(30)
#define DEFAULT_MAX_CONTINUE	(30)


#endif /* _JIKI_H_ */


#ifndef _BONUS_H_
#define _BONUS_H_

enum
{
	SCORE_76800 = 0,
				SCORE_10,	/* (MAX���ȊO��) [P]��10�_ */
							SCORE_20,	SCORE_30,
	SCORE_40,	SCORE_50,	SCORE_60,	SCORE_70,
	SCORE_80,	SCORE_90,	SCORE_100,	SCORE_200,
	SCORE_300,	SCORE_400,	SCORE_500,	SCORE_600,
	SCORE_700,	SCORE_800,	SCORE_900,	SCORE_1000,
	SCORE_2000, SCORE_3000, SCORE_4000, SCORE_5000,
	SCORE_6000, SCORE_7000, SCORE_8000, SCORE_9000,
	SCORE_10000,SCORE_11000,SCORE_12000,SCORE_51200/*(�ő�l)*/
};

#define ITEM_MOVE_FLAG_00_NONE			0x00
#define ITEM_MOVE_FLAG_01_COLLECT		0x01
#define ITEM_MOVE_FLAG_02_RAND_X		0x02
#define ITEM_MOVE_FLAG_04_RAND_Y		0x04
#define ITEM_MOVE_FLAG_06_RAND_XY		(ITEM_MOVE_FLAG_02_RAND_X|ITEM_MOVE_FLAG_04_RAND_Y)

#define ITEM_CREATE_MODE_01 	((4*8/*ITEM_MAX*/)*0)
#define ITEM_CREATE_MODE_02 	((4*8/*ITEM_MAX*/)*1)

/*(�A�C�e���쐬�n)*/
//�p�~extern void item_create(			SPRITE *src, int type, int num, int up_flags);
extern void item_create_mono(			SPRITE *src, int type);
extern void item_create_num(			SPRITE *src, int type, int num_of_creates );
extern void item_create_for_boss(		SPRITE *src, int item_create_mode);
extern void item_create_random_table(	SPRITE *src);

/*()*/
extern void bonus_info_score_nodel( 		SPRITE *src, int score_type);		/*int x, int y*/
extern void bonus_info_any_score_nodel( 	SPRITE *src, u32 score_num_pts);	/*int x, int y*/

/*(�e��S�Đ��_�ɕϊ�����)*/
extern void bullets_to_hosi(void);

/*(�X�R�A���Z����ꍇ�́A�J���X�g �`�F�b�N������̂ŁA�K���������g��)*/
extern void player_dummy_add_score(u32 score_num_pts);
extern void player_add_score(u32 score_num_pts);
#if (1==USE_EXTEND_CHECK)
extern void player_check_extend_score(void);/*(�G�N�X�e���h�`�F�b�N)*/
#endif /* (1==USE_EXTEND_CHECK) */

/*(��Փx�X�R�A�␳)*/
extern u32 adjust_score_by_difficulty(u32 convert_score);

#endif /* _BONUS_H_ */



#ifndef _LOAD_STAGE_FILE_H_
#define _LOAD_STAGE_FILE_H_

//#define  MAX_IVENT_ENTRY	512
#define  MAX_PARA1_36		/*64*/(64-20-8-4+32)/*(64-(4*5))*/

typedef struct _stage_command_
{
	u32 		v_time; 				/* ����(0<v_time)�̏ꍇ 1/60 �P�ʂ̏o�����ԃJ�E���^�B(-1)�̏ꍇ�o���ς݁B(0�̏ꍇerror�������Ȃ��B) */
	struct _stage_command_ *next;
	u32 		user_x; 				/* x */
	int 		user_y; 				/* y */
//(16[Bytes] == (4x4) )
	u8			user_i_code;			/* intermediate code ���ԃR�[�h */
	u8			user_255_code;			/* 'E'�Ƃ�'T'�Ƃ�	*/
	u8			user_item8; 			/* item */
	u8			user_select_gazou;		/* �A�j��/�摜��� */
	int 		user_hp;				/* hp */
	int 		user_score; 			/* score */
	int 		user_kougeki_type;		/* �G���U������ꍇ�̃J�[�h�̎�� */
//(20[Bytes] == (4x5) )
	char		user_string[MAX_PARA1_36/*(64)*/];/* strings */
//(64[Bytes] == (4x4)+(4)+(44) )
} GAME_COMMAND; 	/* .align 64 [bytes] */
//	u16 	dummy_scroll_speed256;	/* para3 scroll speed256 */


/* ���ԃR�[�h�`���̃R�}���h */
/* �Q�[�����́A���ԃR�[�h�`���̃R�}���h�������\����͂͂��Ȃ��B(�\����͂�load_stage.c�ōs��) */

enum /*_game_command_code_*/
{
/*���ԃR�[�h*/
/*0x00*/	GC_CODE_00_SJIS_TEXT = 0,		/* �����\��[�ԕ���؂�Ƃ��Ă��K�v] */
/*0x01*/	GC_CODE_01_ALL_CLEAR,			/* �Q�[�� �S�X�e�[�W �N���A�[ */
/*0x02*/	GC_CODE_02_BG_CONTROL,			/* �w�i�R���g���[���B(�X�N���[�����x��) */
/*0x03*/	GC_CODE_03_CHECK_SECRET_BONUS,	/* �B���{�[�i�X�`�F�b�N "�o��" */
/*0x04*/	GC_CODE_04_BEGIN_SECRET_BONUS,	/* �B���{�[�i�X�`�F�b�N "�m�F" */
/*0x05*/	GC_CODE_05_BOSS_COMMON, 		/* �G (�{�X / ��-�{�X)�p �X�N ���v�g�N�� */
/*0x06*/	GC_CODE_06_CHUU_BOSS_COMMON,	/* �G �U�R��-�{�X(�X�N ���v�g�N�����Ȃ�) */
/*0x07*/	GC_CODE_07_ZAKO,				/* �G �U�R */
			//
/*0x08*/	/* �g���\ */
/*...*/ 	/* ... */
/*0x2f*/	/* �g���\ */
/*0x30*/	GC_CODE_MAX,		/* 0x30 == '0' == �J�[�h�R�}���h (== ���x�R�}���h�A�p�~) */
};

#endif /* _LOAD_STAGE_FILE_H_ */


#ifndef _SPELL_SYSTEM_H_
#define _SPELL_SYSTEM_H_
/*
	�X�y���V�X�e��==�J�[�h�̊Ǘ��V�X�e���B(�J�[�h�V�X�e���Ƃ͈Ⴄ�̂Œ���)
*/
	/*---------------------------------------------------------
	�X�y���V�X�e��(���炩���߃J�[�h�̒�`�����Ă����V�X�e��)
	-------------------------------------------------------
	�͕핗�ł̓X�y���̓V�X�e���ŃT�|�[�g���Ă��܂��B
	�X�y���Ƃ��Ē�`(�J�[�h)���Ă����ƁA�ǂ̃{�X���A�ǂ̃X�y���ł����Ă�悤�ɂȂ�܂��B
	-------------------------------------------------------
	�܂��X�y���̓J�[�h�V�X�e���ɔԍ���o�^���Ă����ƁA�J�[�h�V�X�e���ŃX�y�����擾�ł��܂��B
	(�܂�J�[�h�V�X�e���o�R�ŃX�y�������Ă܂�)
	-------------------------------------------------------
	(r32)���݁A�G��(�͕핗��dat�̒�-�{�X)���X�y�������Ă܂����A
	���̋@�\�͏����Ȃ��Ȃ邩���m��܂���B
	�G��(�͕핗��dat�̒�-�{�X)���X�y�������Ă�@�\�́u���_�\�v�Ŏv�������̂ł����A
	���������@�\�����������Q�[���̑��x�������o����C�����邵�A
	�����������炱�������@�\�͖{�Ƃɖ����̂����Ǝv�����Ƃ�����܂��B�ǂ�����܂���B
	-------------------------------------------------------
	boss->boss_base_card_number �Ɍ��������X�y�����Z�b�g����ƁA�X�y�������J�n�B
	�X�y���������I������Ǝ����I�� boss->boss_base_card_number �� DANMAKU_00 �ɂȂ�B
	-------------------------------------------------------
	�X�y���������� boss->boss_base_spell_time_out ������ɖ��t���[�����Z����A0 �ɂȂ�ƃX�y�������I���B
	���̎��Ԃ̓X�y�����ƂɈႤ�W�����Ԃ��ݒ肳��Ă���B
	����̓J�[�h�̏��������Ŏ��������������B
	-------------------------------------------------------
	�J�[�h�̏������B
	�J�[�h���ς��Ɩ���s���K�v������B
	---------------------------------------------------------*/

/* �J�[�h�ԍ�(�e�X�y������ʂ���ׂ̔ԍ�) */
enum
{
	SPELL_00 = 0,						/* �X�y�������I���t���O�B */
//	/* �G���p */
	SPELL_01_sakuya_misogi_normal,		/* [���] �g5�ʒ�-�{�X �S�J�[�h / �g5�ʒ�-�{�X ��� �u�ʏ�U���v */
	SPELL_02_sakuya_old_test,			/* [���] �g5�ʒ�-�{�X 24�J�[�h (��p�u�~�X�f�B���N�V�������ǂ�(1/2)�v) */
	SPELL_03_sakuya_old_test,			/* [���] �g5�ʒ�-�{�X 11�J�[�h (��p�u�~�X�f�B���N�V�������ǂ�(2/2)�v) */
	SPELL_04_sakuya_pink_hearts,		/* [���] �g5�ʃ{�X [���] �u�ʏ�U��1(1/2)�v�ɂ�����Ƃ��������J�[�h(�\��) */
	SPELL_05_sakuya,					/* [���] �g5�ʃ{�X [���] �u�ʏ�U��1/2(2/2)�v�ɂ�����Ƃ��������J�[�h(�\��) */
	SPELL_06_sakuya,					/* [���] �g5�ʃ{�X [���] ���ہu���i�N���b�N(1/2)�v�ɂ�����Ƃ��������J�[�h(�\��) */
	SPELL_07_sakuya,					/* [���] �g5�ʃ{�X [���] �u�ʏ�U��3(1/2)�v�ɂ�����Ƃ��������J�[�h(�\��) */
//
	SPELL_08_rumia, 					/* [���[�~�A] �g1�ʃ{�X [���[�~�A] �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������J�[�h(�\��) */
	SPELL_09_alice, 					/* [�A���X] ���ۂ��H�e�L�g�[�J�[�h */
	SPELL_0a_dai_yousei_01, 			/* [��d��] �S�� 48�����A���v���A�΃N�i�C�e */
	SPELL_0b_dai_yousei_02, 			/* [��d��] �S�� 48�����A�����v���A�ԃN�i�C�e */
	SPELL_0c_sakuya_jack32, 			/* [���] ���H�u�W���b�N�E�U�E���h�r���v�ɂȂ邩������Ȃ��e�X�g */
	SPELL_0d_sakuya_miss_direction, 	/* [���] ��p�u�~�X�f�B���N�V�����v */
	SPELL_0e_remilia_00,				/* [���~���A] �e�X�g */
	SPELL_0f_tengu, 					/* [���̑�] �������̗d��1(�V��l?)�����J�[�h(�\��) */ 	/*???*/
//
	SPELL_10_cirno, 					/* [�`���m] �S�J�[�h */
	SPELL_11_perfect_freeze,			/* [�`���m] �p�[�t�F�N�g�t���[�Y */
	SPELL_12_diamond_blizzard,			/* [�`���m] �ᕄ�u�_�C�A�����h�u���U�[�h�v */
	SPELL_13_alice_suwako,				/* [�A���X] �z�K�q���ۂ�(?)�J�[�h */
	SPELL_14_alice_youmu300,			/* [�A���X] �d3�ʃ{�X  normal�ʏ�U��3(�̈ꕔ) */
	SPELL_15_alice_aka_2nd, 			/* [�A���X] ��EX�ʃ{�X �Ԃ̖��@2�i�K�ڂ��ǂ� */
	SPELL_16_alice_doll,				/* [�A���X] �d3�ʃ{�X [�A���X]�l�`�J�[�h(���ǂ�) */ // 40[sec]
	SPELL_17_alice_doll_02, 			/* [�A���X] �쐬�� */
//
	SPELL_18_hana_test, 				/* [����] �ԂĂ��ƃJ�[�h */
	SPELL_40_SONOTA_DEBUG_CW_AO,		/* [���̑�] �f�o�b�O�J�[�hCW�� */
	SPELL_41_SONOTA_DEBUG_CCW_AKA,		/* [���̑�] �f�o�b�O�J�[�hCCW�� */
	SPELL_1b_cirno_icecle_fall, 		/* [�`���m] �X���u�A�C�V�N���t�H�[���v */
	SPELL_44_ruiz3_test,				/* [���C�Y] �J�[�h��̂Ă��ƁB */
	SPELL_1d_amefuri_test,				/* [] �J�Ă��� */
	SPELL_1e_momiji_no_nonoji,			/* [��] �̂̂��J�[�h */
	SPELL_1f_koakuma,					/* [������] �Ă��Ɨp */
//	/* �{�X�p */
	SPELL_20_aya_misogi1,				/*	1[��] �̒ʏ�e���ۂ��J�[�h�������Ă݂�e�X�g1 */
	SPELL_21_aya_misogi2,				/*	2[��] �̒ʏ�e���ۂ��J�[�h�������Ă݂�e�X�g2 */
	SPELL_22_ame_no_yatimata,			/*	3[��] �򕄁u�V�̔��ˁv */
	SPELL_23_meirin_magaru_kunai,		/*	4[����] �ʏ�U�� */
	SPELL_24_aya_doll,					/*	5[��] �l�`�J�[�h */ //40[sec]
	SPELL_25_houka_kenran,				/*	6[�g����] �ؕ��u�F�؈�ࣁv�ɂ�����Ƃ��������J�[�h(�\��) */
	SPELL_26_aya_saifu, 				/*	7[��] �Ǖ� */
	SPELL_27_hosigata_test, 			/*	8 [���̑�] ���^�e�X�g */
//
	SPELL_28_remilia_tamaoki1,			/*	9[���] �g �ʃ{�X  ��݂����ۂ����̋ʒu�J�[�h(�\��) */
	SPELL_29_rumia_demarcation, 		/* 10[���[�~�A] �g1�ʃ{�X �ŕ��u�f�B�}�[�P�C�V�����v�ɂ�����Ƃ��������J�[�h(�\��) */
	SPELL_2a_sakuya_baramaki1,			/* 11[���] �g5�ʒ�-�{�X �΂�T��1 �J�[�h () */
	SPELL_2b_sakuya_baramaki2,			/* 12[���] �g5�ʒ�-�{�X �΂�T��2 �J�[�h () */
	SPELL_2c_sakuya_blue_red,			/* 13[���] �g5�ʒ�-�{�X �u�ʏ�U��3�v�ԃi�C�t */
	SPELL_2d_sakuya_misogi_lunatic, 	/* 14[���] �g5�ʒ�-�{�X �S�J�[�h / �g5�ʒ�-�{�X ��� �u�ʏ�U���vlunatic */
	SPELL_2e_pache_agni_shine_1,		/* [�p�`�F] "�@�@�@�@�Ε��u�A�O�j�V���C���v" No.15 */	// agni shine (�A�O�j�̋P��)�A (�A�O�j==�C���h�_�b�̉΂̐_)
	SPELL_2f_pache_princess_undine, 	/* [�p�`�F] "�@�����u�v�����Z�X�E���f�B�l�v" No.16 */	// princess undine (�E���f�B�l�̕P)�A(�E���f�B�l==�B���p�t�p���P���X�X�́u�d���̏��v�ɓo�ꂷ�鐅�̐��̖��O)
//
	SPELL_30_pache_sylphy_horn_1,		/* [�p�`�F] "�@�@�@�@�ؕ��u�V���t�B�z�����v" No.17 */	// sylphy horn (�V���t�B�̃z����)�A(�V���t�B==�����i�鐸��)�A(�z����==�y��==�p�J)
	SPELL_31_pache_rage_tririton_1, 	/* [�p�`�F] "�@�@�y���u���C�W�B�g�����g���v" No.18 */	// rage tririton (�{��g�����g��)�A (�g�����g��==���E��Y�X�g�[���w���W�̎O��)
	SPELL_32_pache_metal_fatigue,		/* [�p�`�F] "�@�@�����u���^���t�@�e�B�[�O�v" No.19 */	// metal fatigue (������J)
	SPELL_33_pache_agni_shine_2,		/* [�p�`�F] "�@�@�Ε��u�A�O�j�V���C���㋉�v" No.20 */
	SPELL_34_pache_sylphy_horn_2,		/* [�p�`�F] "�@�@�ؕ��u�V���t�B�z�����㋉�v" No.21 */
	SPELL_35_pache_rage_tririton_2, 	/* [�p�`�F] "�y���u���C�W�B�g�����g���㋉�v" No.22 */
	SPELL_36_pache_agni_radiance,		/* [�p�`�F] "�@�Ε��u�A�O�j���C�f�B�A���X�v" No.23 */	// agni radiance (�A�O�j�̋P��)�A (�A�O�j==�C���h�_�b�̉΂̐_)
	SPELL_37_pache_bury_in_lake,		/* [�p�`�F] "�@�@�@�����u�x���[�C�����C�N�v" No.24 */	// bury in lake (����)
//
	SPELL_38_pache_green_storm, 		/* [�p�`�F] "�@�@�@�ؕ��u�O���[���X�g�[���v" No.25 */	// green storm (�΂̗�)
	SPELL_39_pache_tririton_shake,		/* [�p�`�F] "�@�@�y���u�g�����g���V�F�C�N�v" No.26 */	// tririton shake (�h���g�����g��)�A (�g�����g��==���E��Y�X�g�[���w���W�̎O��)
	SPELL_3a_pache_silver_dragon,		/* [�p�`�F] "�@�@�@�����u�V���o�[�h���S���v" No.27 */	// silver dragon (�◃��)
	SPELL_3b_pache_lava_cromlech,		/* [�p�`�F] "�΁��y���u���[���@�N�������N�v" No.28 */	// lava cromlech (�n�����)
	SPELL_3c_pache_forest_blaze,		/* [�p�`�F] "�؁��Ε��u�t�H���X�g�u���C�Y�v" No.29 */	// forest blaze (�X�щ΍�)
	SPELL_3d_pache_water_elf,			/* [�p�`�F] "�@�����ؕ��u�E�H�[�^�[�G���t�v" No.30 */	// water (��) / elf (�d��)	(�M���O:���n��==���ɗn�����Ⴄ�d��)
	SPELL_3e_pache_mercury_poison,		/* [�p�`�F] "���������u�}�[�L�����|�C�Y���v" No.31 */	// mercury poison (���⒆��)
	SPELL_3f_pache_emerald_megalith,	/* [�p�`�F] "�y�������u�G�������h���K���X�v" No.32 */	// emerald megalith (����ȃG�������h��)
//
	SPELL_42_PACHE_LASER1,				/* [�p�`�F] �u�ʏ�U��1�v�̃��[�U�[���ǂ� */
	SPELL_43_PACHE_LASER2,				/* [�p�`�F] �u�ʏ�U��3�v�̃��[�U�[���ǂ� */
	SPELL_19_mima_sekkin,				/* [����] �ڋ߃J�[�h */
	SPELL_1a_mima_toge, 				/* [����] �g�Q�Ă��ƃJ�[�h */
	SPELL_1e_kaguya01,					/* [�P��] �ނ���ϊ�1 */
	SPELL_1f_kaguya04,					/* [�P��] �ނ���ϊ�2 */
	SPELL_1c_kakuya_tamanoe,			/* [�P��]�A�H���̋ʂ̎}���ǂ��B�쐬�� */
	SPELL_47_,							/* ���� */
//
	#if (1)
	SPELL_48_r34_gokan_kinou, 			/* [r34]r34�ȑO�̌݊��@�\�B(r35-)�V�X�e���ŕK���K�v�B */
	SPELL_49,							/* ���� */
	SPELL_4a,							/* ���� */
	SPELL_4b,							/* ���� */
	SPELL_4c,							/* ���� */
	SPELL_4d,							/* ���� */
	SPELL_4e,							/* ���� */
	SPELL_4f,							/* ���� */
	#endif
//
	SPELL_MAX,
};
//	SPELL_1f_silent_selena, 			/* [�p�`�F] �T�C�����g�Z���i �� �Ă��Ɨp */
//


/* HATSUDAN_03_angle65536 �� ���@�_���e�̊p�x���v�Z */
extern void tmp_angleCCW65536_src_nerai(void);

/* HATSUDAN_03_angle65536 �� ���@�_���e�̊p�x���v�Z */
extern void calculate_jikinerai(void);


/* �X�y����CPU���s���A�J�[�h���P�t���[����������B */
extern void card_generate(SPRITE *src);



#endif /* _SPELL_SYSTEM_H_ */


#ifndef _CARD_SYSTEM_H_
#define _CARD_SYSTEM_H_

//------------ �J�[�h�֘A

extern void create_card(SPRITE *src, int card_number);


//extern void ch eck_regist_card(SPRITE *src);/* �J�[�h�̍X�V�`�F�b�N */
extern void card_boss_move_generate_check_regist(SPRITE *src);	/* �J�[�h�o�^�\�Ȃ�o�^ / �J�[�h���� */


// extern void card_state_check_holding(SPRITE *src);/* �J�[�h���I���܂ő҂B */

enum
{
	CARD_BOSS_MODE_00_OFF = 0,			/* �J�[�h���g�p���Ȃ�(�ʏ�U����)�B(�J�[�h�����Ă邩�ǂ������f) */
	CARD_BOSS_MODE_01_IDO_JYUNNBI,		/* ���Ă�ꍇ�B���e�ʒu�܂ňړ� */
	CARD_BOSS_MODE_02_TAIHI,			/* �ޔ𒆁B */
	CARD_BOSS_MODE_03_HATUDAN,			/* ���e���B */
};



enum
{
	DANMAKU_LAYER_00 = 0,			//(0)/* �e���R���g���[�����Ȃ��ʏ�e(��ʊO�Œe����) */
	DANMAKU_LAYER_01,				//(1)/* �e���R���g���[���O���[�v(1)�e */
	DANMAKU_LAYER_02,				//(2)/* �e���R���g���[���O���[�v(2)�e */
	DANMAKU_LAYER_03,				//(3)/* �e���R���g���[���O���[�v(3)�e */
	DANMAKU_LAYER_04_MAX/* �e���R���g���[���O���[�v�ő吔 */
};

typedef struct /*_card_global_class_*/
{
	int mode;				/* �J�[�h���[�h */
	int limit_health;		/* �K��l�ȉ��ɂȂ�΃J�[�h���[�h���� */
	int boss_state; 		/* ���l�ɂȂ�΃{�X���J�[�h���[�h�ɓ���Ȃ� */
	int boss_timer; 		/* [���p]�������� */
	//
	int number; 			/* [���p]�J�[�h�ԍ� */
	int number_temporaly;	/* [�ꎞ�g�p]�J�[�h�ԍ� */
	int card_number;		/* */
	int boss_hp_dec_by_frame;/* �{�X�U�������l�A�t���[���P�� */
	//
	void (*danmaku_callback[(DANMAKU_LAYER_04_MAX)])(SPRITE *sss);	/*	�e���R�[���o�b�N�V�X�e��(�X�y���ϐg����) */
	// �u�ړ��ł��Ȃ������t���O�v(�g�p�O�Ɏ蓮��OFF==0�ɂ��Ƃ�)
	int/*u8*/ boss_hamidasi;			/* �u�ړ��ł��Ȃ������t���O�v(�g�p�O�Ɏ蓮��OFF==0�ɂ��Ƃ�) */
} CARD_SYSTEM_GLOBAL_CLASS;
extern CARD_SYSTEM_GLOBAL_CLASS card;

//	int dummy2;
/* �Ƃ肠���� */
#if 0
//	#define boss_base_resurved000				user_data10/*(r32)���݁A���g�p�B*/
//	#define bo ss_base_state001 				user_data10/*�p�~*/
	#define boss_base_spell_temporaly			user_data11
	#define boss_base_card_number				user_data12
	#define boss_base_spell_time_out			user_data13
#endif


//------------ "��"�݂����ȃ}�[�N�̃G�t�F�N�g
/*
�u"��"�݂����ȃ}�[�N�v�́A�{�X�������Ă�J�[�h�������ł��B
������{���́A�J�[�h�̖�������������A���������炳�Ȃ��Ⴂ���Ȃ��̂��ȁH
�ł��{�Ƃ����Ȃ��ĂȂ���ˁB
*/
/*static*/extern  void move_card_square_effect(SPRITE *src);
/*static*/extern  void boss_effect_initialize_position(void);

extern void danmaku_system_callback(void);
#endif /* _CARD_SYSTEM_H_ */


#ifndef _BOSS_COMMON_H_
#define _BOSS_COMMON_H_

/* �{�X�Ő錾���K�v�Ȃ���(�O���[�o��) */

enum
{
	DESTROY_CHECK_00_WIN_BOSS = 0,
	DESTROY_CHECK_01_IS_TIME_OUT,
};
/*	�� �U���̏ꍇ�̎��S���� 	�� ���Ԑ؂�̏ꍇ�̎��S���� */
/*static*/extern  void boss_destroy_check_type(SPRITE *src/*�G����*/, int check_type);

/* �J�[�h�V�X�e�� */

/* �J�[�h�̏������B�J�[�h���ς��Ɩ���s���K�v������B */
extern void card_maikai_init(SPRITE *src);						/*(���񏉊���)*/
extern void card_maikai_init_and_get_spell_number(SPRITE *src); /*(���񏉊���)+(���݌��ׂ��ԍ����J�[�h�V�X�e������擾)*/

// ���ʌ`��
extern void common_99_keitai(SPRITE *src);/* ���j��ɉ�ʊO�Ƀ{�X�������� */

#endif /* _BOSS_H_ */


#ifndef _ATARI_HANTEI_H_
#define _ATARI_HANTEI_H_

enum
{
	BAKUHATSU_MINI00 = 0,/* ������ */
	BAKUHATSU_MINI01,
	BAKUHATSU_MINI02,
	BAKUHATSU_MINI03,
//
	BAKUHATSU_ZAKO04,/* �U�R���Ŕ��� */
	BAKUHATSU_ZAKO05,
	BAKUHATSU_ZAKO06,
	BAKUHATSU_ZAKO07,
//
	BAKUHATSU_FIRE08,/* �Ή����� */
	BAKUHATSU_FIRE09,
	BAKUHATSU_FIRE10,
	BAKUHATSU_FIRE11,
//
	BAKUHATSU_MOVE12,/* �ړ����� */
	BAKUHATSU_MOVE13,
	BAKUHATSU_MOVE14,
	BAKUHATSU_MOVE15,
};
#define TAMA_ATARI_02					((2)<<8)
#define TAMA_ATARI_04					((4)<<8)

#define TAMA_ATARI_COMMON16_PNG 		TAMA_ATARI_02
#define TAMA_ATARI_MARU16_PNG			TAMA_ATARI_02
#define TAMA_ATARI_KNIFE18_PNG			TAMA_ATARI_02
#define TAMA_ATARI_JIPPOU32_PNG 		TAMA_ATARI_02

#define TAMA_ATARI_OODAMA_08_PNG		TAMA_ATARI_04



#define TAMA_ATARI_BULLET_DUMMY 		TAMA_ATARI_02
#define JIKI_ATARI_ITEM_16				((16)<<8)
#define JIKI_ATARI_ITEM_80				((80)<<8)


#define ZAKO_ATARI02_PNG				TAMA_ATARI_02
#define ZAKO_ATARI04_PNG				TAMA_ATARI_04
#define ZAKO_ATARI16_PNG				JIKI_ATARI_ITEM_16
#define ZAKO_ATARI_HOMING16_PNG 		JIKI_ATARI_ITEM_16


	/* ���e�G�t�F�N�g�̎��ԕ� */
	#define HATUDAN_FRAME64 	(64)

/**/extern int boss_hamidasi;		/* �u�ړ��ł��Ȃ������t���O�v(�g�p�O�Ɏ蓮��OFF==0�ɂ��Ƃ�) */
typedef struct /*_clip_class_*/
{
	POINT256 boss_clip_min; 	/* �{�X�ړ��͈�(�ŏ��l) */
	POINT256 boss_clip_max; 	/* �{�X�ړ��͈�(�ő�l) */

	POINT256 bullet_clip_min;	/* �e�͈̔�(�ŏ��l) */
	POINT256 bullet_clip_max;	/* �e�͈̔�(�ő�l) */
} RECT_CLIP_CLASS;
extern RECT_CLIP_CLASS rect_clip;

extern void bakuhatsu_add_type_ccc(int type);
extern void bakuhatsu_add_rect(SPRITE *src);
extern void bakuhatsu_add_circle(SPRITE *src, int mode);
extern void bakuhatsu_add_zako04(SPRITE *src);

extern void callback_hit_zako(SPRITE *src, SPRITE *t);

#endif /* _ATARI_HANTEI_H_ */

