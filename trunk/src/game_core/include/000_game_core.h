
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _GAME_CORE_
#define _GAME_CORE_
/*---------------------------------------------------------
	�Q�[�������N
---------------------------------------------------------*/

enum /*_game_rank_*/
{
	RANK_EASY = 0,
	RANK_NORMAL,	/*=1*/
	RANK_HARD,		/*=2*/
	RANK_LUNATIC,	/*=3*/
	RANK_MAX		/* �����N�̍ő吔==(�ō������N+1) */
};

/*---------------------------------------------------------
	�R�[���o�b�N
---------------------------------------------------------*/
#if 1/*�P���R�[���o�b�N����*/
extern MAIN_CALL_FUNC(common_load_init);
extern MAIN_CALL_FUNC(stage_clear_result_screen_start);/* for game_core.c ??stage_clear.c */
//
extern MAIN_CALL_FUNC(stage_first_init);/* for game_core.c select_player.c */
extern MAIN_CALL_FUNC(shooting_game_core_work);/* for pause.c ask_continue.c */
extern MAIN_CALL_FUNC(gameover_start);/* for ask_continue.c */
extern MAIN_CALL_FUNC(name_entry_start);/* for game_over.c */
//
extern MAIN_CALL_FUNC(stage_select_menu_start);
extern MAIN_CALL_FUNC(option_menu_start);
extern MAIN_CALL_FUNC(story_mode_start);
extern MAIN_CALL_FUNC(yume_no_kiroku_start);
extern MAIN_CALL_FUNC(key_config_start);
extern MAIN_CALL_FUNC(music_room_start);
extern MAIN_CALL_FUNC(title_menu_start);/*for pause.c ... */
extern MAIN_CALL_FUNC(pause_menu_start);
extern MAIN_CALL_FUNC(ask_continue_menu_start);
extern MAIN_CALL_FUNC(rank_select_menu_start);
//

#define pause_out_call_func 	return_call_func/* �|�[�Y���̖߂�� */
#define menu_out_call_func		return_call_func/* ���@�I�����̖߂�� / �I�v�V�������j���[�̖߂�� */

/*
	return_call_func
	�� �p�r1: pause_out_call_func : �|�[�Y���̖߂���ێ��B
	�� �p�r2: menu_out_call_func  : ���j���[�V�X�e���Ń��j���[���o�b�N����(�߂�)�ꍇ�ɉ����̃��j���[�֖߂邩��ێ��B
*/
#endif

/*---------------------------------------------------------
	���j���[�֘A
---------------------------------------------------------*/

#if 1/*(���j���[�֘A)*/
/* �L�����Z������炵�ă��j���[�ɋ������A */
extern void menu_cancel_and_voice(void);
#endif


#endif /* _GAME_CORE_ */

#ifndef _JIKI_H_
#define _JIKI_H_

// > �yPSP�zPSP�œ�����肽���z8�y�e���z
// > 200 �F���������񁗂��������ς��B�F2009/01/27(��) 18:41:00 ID:xQgI7hCU
// > ���낻��S�A�C�e�������ė~�����ȁB
// > ����FULL�p���[���̃A�C�e�������񂹃��C���͂���������Ɖ��̕��������Ǝv���B
// > ��̓I�ɂ͂��̕ӂ聫
// > http://www4.uploader.jp/user/isisuke/images/isisuke_uljp00081.bmp
// ���̈ӌ��͂킽�������l�Ɏv�����̂ŁA�݂�Ȃ������v�����ɈႢ�Ȃ��B
// ���̉摜��484x283[pixel]�ŁAy==63[line]�ɐԐ��������Ă������B
// psp�̉𑜓x�́A 480x272[pixel]�Ȃ̂� x/272=63/283, x=(63*272)/283 == 60.55[line]
// ����/���쎁	50(49)[line] �����シ����B
// �X��8��200�� ��60[line] (���o�I��)
// �����ŃL�����̍����������邵�A64(63)�������Ɖ�72(71)���炢�ł������̂ł́H�Ƃ������64�ɂ��Ď������Ă݂�B
// ���ʁF����ς����������������B8[pixel]�Ƃ��킸16[pixel]���炢���������B�Ăю������Ă݂�B
// ���ʁF����Ȃ��񂩂ȁH�Ƃ����킯��80(79)[pixel]�Ɍ���B����8[pixel]��88(87)�ł����������ˁB
// �{�Ƃ̊��o��厖�ɂ���Ȃ炱��Ȃ��񂾂Ǝv���B
//---------------
//�u�A�C�e�������񂹃��C���v�ȉ��Ŏ��Ɓu�A�C�e�������񂹃��C���v����322���C���܂ŋ����Ō��_�B
//
//�v���C�t�B�[���h�� 384x448 �Ȃ̂ŁA448-322 == 126
//�u�A�C�e�������񂹃��C���v�͑����ォ��128�h�b�g���ȁH
//
//(B�A�C�e���̑傫����16x16[pixel]�Ȃ̂ŁAB�A�C�e���̏�������� 448+(16/2)���C���ōs������A
//�Ō��B�A�C�e���̔��肪����̂��AB�A�C�e���̂x�����S��456[���C��]�ɂ����ꍇ�B
//456-322 == 134, 134-128 == 6. ����6[���C��]�͂����������@���S����ʉ����ɍs���Ȃ��������h�b�g(5[pixel]?)����B
//�c��1[pixel]�͕s�����Ŕ��肵������? )
//---------------
// �������䗦�Ȃ�A
// (�͕핗�A�C�e�������񂹃��C��)/(�{�ƃA�C�e�������񂹃��C��) == (�͕핗�Q�[���t�B�[���h�c)/(�{�ƃQ�[���t�B�[���h�c)
// ����������B(�͕핗�A�C�e�������񂹃��C��)���m�肽���̂�����ڍs���āA
// (�͕핗�A�C�e�������񂹃��C��) == (�{�ƃA�C�e�������񂹃��C��) * (�͕핗�Q�[���t�B�[���h�c)/(�{�ƃQ�[���t�B�[���h�c)
// (�͕핗�A�C�e�������񂹃��C��) == 128 * 272/448 == 77.7142857142857142857142857142857 =:= 78[pixel] �P���Ȕ䗦�v�Z�̏ꍇ�B
// http://hossy.info/game/toho/k_score.php �����̉摜���t�Z���Čv�Z����� 77 [pixel] ������B
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
/*(���荞�݃V�X�e��(����)�p�Ɋm��)*/
#define II_SYSTEM_TASK_NUMBER							(0x000f)
//--------
/* �v���C���[���㕔�ֈړ����A�蓮�ŃA�C�e�����W�s���ꍇ��on�B */
#define JIKI_FLAG_0x0010_JYOU_BU_SYUU_SYUU				(0x0010)
/* �{�������������ۂɁA�����ŃA�C�e�����W���s���ꍇ��on�B */
#define JIKI_FLAG_0x0020_BOMBER_SYUU_SYUU				(0x0020)
/* �{�X�|����ɓ_��(���A�C�e��)��{�X���o���A�C�e���������Ŏ��W����B���̏ꍇ��on�B */
#define JIKI_FLAG_0x0040_BOSS_GO_ITEM_JIDOU_SYUU_SYUU	(0x0040)
//-------
/* �� [�v���C���[�̃I�v�V�������V���b�g�����ꍇ�̒ʐM�p�t���O] */
#define JIKI_FLAG_0x0080_SHOT_KEY_SEND_FOR_OPTION		(0x0080)
/* 1==�I�v�V�������V���b�g�����B */
/* 0==�I�v�V�������V���b�g�������Ȃ��B */
//-------


	#define SCENE_FLAG_0x0000_KAIWA_TATAKAU 	(0x0000)/*(����)*/
	#define SCENE_FLAG_0x0100_KAIWA_LOAD		(0x0100)/*(����)*/
	#define SCENE_FLAG_0x0200_KAIWA_MODE		(0x0200)/*(����)*/
	#define SCENE_FLAG_0x0300_KAIWA_END 		(0x0300)/*(����)*/
	#define SCENE_FLAG_0x0300_KAIWA_MASK		(0x0300)/*(����)*/
	#define SCENE_NUMBER_0x0400_DOUCHUU 		(0x0400)
	#define SCENE_NUMBER_0x0800_BOSS_TATAKAU	(0x0800)
	#define SCENE_NUMBER_MASK					(0xff00)

#define USE_r36_SCENE_FLAG (1)
//#define USE_r36_SCENE_FLAG	(0)
#if (1==USE_r36_SCENE_FLAG)
	/*
	SCENE_FLAG:
0000: 0x0000: �X�e�[�W�^�C�g���A�����B
0001: 0x0100: �X�e�[�W�^�C�g���A��b�ǂ݂��݁B"0"
0010: 0x0200: �X�e�[�W�^�C�g���A��b���[�h�B
0011: 0x0300: �X�e�[�W�^�C�g���A��b���[�h�����܂��w���B
0100: 0x0400: �{�X�퓬�O�A�����A�{�X�w���B				[���ꏈ��on:�����ǉ�]
0101: 0x0500: �{�X�퓬�O�A��b�ǂ݂��݁B"1" 			[���ꏈ��on:��b�ǂ݂���]
0110: 0x0600: �{�X�퓬�O�A��b���[�h�B					[���ꏈ��on:��b���[�h]
0111: 0x0700: �{�X�퓬�O�A��b���[�h�����܂��w���B		[���ꏈ��on:��b���[�h�����܂��w��]
//
1000: 0x0800: �{�X�퓬���B								[���ꏈ��off:�Ȃ�]
1001: 0x0900: �{�X�퓬��Agood��b�ǂ݂��݁B"2"(or"3")	[���ꏈ��on:��b�ǂ݂���]
1010: 0x0a00: �{�X�퓬��Agood��b���[�h�B				[���ꏈ��on:��b���[�h]
1011: 0x0b00: �{�X�퓬��Agood��b�����܂��w���B		[���ꏈ��on:��b���[�h�����܂��w��]
1100: 0x0c00: �{�X�퓬���B(�_�~�[�B���̏�Ԃɂ͐�΂ɂȂ�Ȃ��B���̏�Ԃ��ƃ{�X�̗̓Q�[�W���`���Ȃ�)
1101: 0x0d00: �{�X�퓬��Abad��b�ǂ݂��݁B"3"
1110: 0x0e00: �{�X�퓬��Abad��b���[�h�B
1111: 0x0f00: �{�X�퓬��Abad��b���[�h�����܂��w���B
	  0x1000:
	  0x8000:�v���C���[���[�v�𔲂��鏈��(�Ƃ肠����)
*/
//	#define SCENE_FLAG_TEST_IS_BOSS 			(0x0800)
//	#define SCENE_NUMBER_MASK_IS_BOSS			(0x0d00)


	#define IS_SCENE(aaa)		((cg.state_flag & SCENE_NUMBER_MASK))
	#define NEXT_SCENE			cg.state_flag += 0x0100;

//
	#define IS_SCENE_DOUCHU_TUIKA		(SCENE_NUMBER_0x0400_DOUCHUU>=(cg.state_flag & (SCENE_NUMBER_MASK)))
	#define IS_SCENE_KAIWA_TATAKAU		(SCENE_FLAG_0x0000_KAIWA_TATAKAU	==(cg.state_flag & (SCENE_FLAG_0x0300_KAIWA_MASK)))
	#define IS_SCENE_KAIWA_LOAD 		(SCENE_FLAG_0x0100_KAIWA_LOAD		==(cg.state_flag & (SCENE_FLAG_0x0300_KAIWA_MASK)))
	#define IS_SCENE_KAIWA_MODE 		(SCENE_FLAG_0x0200_KAIWA_MODE		==(cg.state_flag & (SCENE_FLAG_0x0300_KAIWA_MASK)))
	#define IS_SCENE_END_KAIWA_MODE 	(SCENE_FLAG_0x0300_KAIWA_END		==(cg.state_flag & (SCENE_FLAG_0x0300_KAIWA_MASK)))
#else
	#define STATE_FLAG_0x0100_IS_LOAD_KAIWA_TXT 			(0x0100)
	/* on */

	/* �Q�[�����A��b���[�h�� on �ɂȂ�B�Q�[�����ĊJ������ off �ɂȂ�B*/
	#define STATE_FLAG_0x0200_IS_KAIWA_MODE 				(0x0200)
	/* on */

	#define STATE_FLAG_0x0300_END_KAIWA_MODE				(0x0400)
	/* on */

	/* �{�X�Ɛ키�ۂ�on(�{�X�O�C�x���g�ł�OFF) */
	#define STATE_FLAG_0x0800_IS_BOSS						(0x0800)

	/* �{�X��|�����ꍇ��on(���Ԑ؂�ł�OFF) */
	//#define STATE_FLAG_09_IS_WIN_BOSS 					(0x0400)
	/* on */
//-------
	#define STATE_FLAG_13_GAME_TERMINATE					(0x1000)/* result��ɃQ�[���I�� */
	#define STATE_FLAG_14_DOUCHU_TUIKA						(0x2000)/* �����ŃR�}���h�ǉ����������s���ꍇon */
	#define STATE_FLAG_15_DRAW_BOSS_GAUGE					(0x4000)
//-------
	#define IS_SCENE_DOUCHU_TUIKA		(cg.state_flag & STATE_FLAG_14_DOUCHU_TUIKA)
	#define IS_SCENE_KAIWA_TATAKAU		(0==(cg.state_flag & STATE_FLAG_0x0200_IS_KAIWA_MODE))
	#define IS_SCENE_KAIWA_LOAD 		(cg.state_flag & STATE_FLAG_0x0100_IS_LOAD_KAIWA_TXT)
	#define IS_SCENE_KAIWA_MODE 		(cg.state_flag & STATE_FLAG_0x0200_IS_KAIWA_MODE)
	#define IS_SCENE_END_KAIWA_MODE 	(cg.state_flag & STATE_FLAG_0x0300_END_KAIWA_MODE)
#endif


/*(�g�p�\�肠��B������)*/
#define USE_BOSS_JIKANGIRE	(0)
/* 1:�g���B 0:�g��Ȃ��B�X�y�J���Ԑ؂�̏ꍇ�A�C�e���o���Ȃ��@�\�B�����؁B */


#if (1==USE_BOSS_JIKANGIRE)/*(�g�p�\�肠��B������)*/
	/*(���e����spellcpu�ŏ���)*/
	#define JIKI_FLAG_16_0x8000_BOSS_JIKAN_GIRE 			(0x8000)
#endif


//#define STATE_FLAG_0123_AUTO_GET_ITEM (JIKI_FLAG_0x0010_JYOU_BU_SYUU_SYUU|JIKI_FLAG_0x0020_BOMBER_SYUU_SYUU|JIKI_FLAG_0x0040_BOSS_GO_ITEM_JIDOU_SYUU_SYUU)



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

#define USE_PAD_STRUCT (1)
/*(0: ���� / GAME_CORE_GLOBAL_CALLBACK�ɓ����)*/
/*(1: �Ɨ� / GAME_CORE_GLOBAL_CALLBACK�ɓ���Ȃ�)*/

/*(0: �A�i���O�ʂ��g��Ȃ�[���v���C�Ή�����Ȃ�A�i���O�ʂ��g��Ȃ������Ó��ȋC������B1byte�t�H�[�}�b�g(�㉺���E,�`�a�b[Slow])�ɏo����B����Ƀ�������Ń��������O�X���k�ł���B(���k���Ȃ���L�^/�Đ�)])*/
/*(1: �A�i���O�ʂ��g��[�A�i���O�̏ꍇ���������O�X���k�������Ȃ��̂ŏ����Ƀ�������H���B0.5M[bytes]����������H�����A���邩�s��])*/
//r39�Ŕp�~�ς�#define USE_ANA LOG_VALUE (0)
/*[1/60sec]==4[byte]*/
/*[1sec]==60*4[byte]*/
/*[1min]==60*60*4[byte]*/
/*[30min]==30*60*60*4[byte]==432000==421.875k[Bytes]==0.4119873046875M[Bytes]*/
/*(�͕핗�̓N���A�܂�30���ȏォ����)*/
typedef struct _game_core_global_callback_
{
	void (*main_call_func)(void);/* ���C���R�[���o�b�N */
	void (*return_call_func)(void);/* �|�[�Y���A��̖߂��R�[���o�b�N */
	/* �J�[�h���łȂ��ʏ펞�̃v���O�������w�� */
	void *callback_gu_draw_haikei_modosu;	/* (�J�[�h������ʏ펞��)�߂��p�B(==�ʏ펞) */
	/* �J�[�h���̃v���O�������w�� */
	void *callback_gu_draw_haikei_supeka;	/* �J�[�h���p */
//
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
//extern S_DL_Surface *screen;
/*extern*/ SDL_Surface *sdl_screen[SDL_99_MAX_SCREEN];/*(4)*/
//
	//
//	u16 *show_frame;	/*(�\�����)*/
//	u16 *draw_frame;	/*(�`����)*/
//	u16 *work_frame;	/*(�ꎞ��Ɨp���)*/
//	u16 *tex_frame; 	/*(�L���b�V���摜�p���)*/
	//
	#if (0==USE_PAD_STRUCT)
	/*(����)*/
	u32 pad_data;						/*�������*/				//	extern	u32 cg_m y_pad;
	u32 pad_data_alter; 				/*�O�����*/				//	extern	u32 cg_m y_pad_alter;
	#endif/*(USE_PAD_STRUCT)*/
} GAME_CORE_GLOBAL_CALLBACK;
extern GAME_CORE_GLOBAL_CALLBACK cb;

	#if (1==USE_PAD_STRUCT)/*(r34)*/
	/*(�Ɨ�)*/
	/* �A���C�����g�֌W(???) (s16)�� GAME_CORE_GLOBAL_CLASS�ɓ����ƍI�������Ȃ� */
	/* �Ӑ}�I�ɓ���Ȃ����� */
typedef struct _psp_pad_global_class_
{
	u32 pad_data;						/*�������*/				//	extern	u32 cg_m y_pad;
	u32 pad_data_alter; 				/*�O�����*/				//	extern	u32 cg_m y_pad_alter;
} PSP_PAD_GLOBAL_CLASS;
extern PSP_PAD_GLOBAL_CLASS psp_pad;
	#else/*(0==USE_PAD_STRUCT)*/
		/*(����)*/
		#define psp_pad cb/*_global_pad_*/
	#endif/*(USE_PAD_STRUCT)*/

typedef struct _game_core_global_class_
{
	u32 state_flag; 	/* �ݒ�t���O(�W) */
	int weapon_power;	/* 0x00-0x80  (0-128 ��129�i�K==�{�ƂƓ���)   max==128==�u129�i�K�v*/
	int chain_point;
	s32 bomber_time;	/* Use Gu */  // /* bomb_wait */ /* �{���̗L������ */
//[0x10 4]
	u32 game_score; 	/* �X�R�A���_ */
	u32 graze_point;	/* �O���C�Y���_ */
	int bombs;			/* �{���� */
	int zanki;			/* �c��`�����X */
//[0x20 8]
	s32 game_difficulty;/* (psp��max,min���߂��g���Ă���̂� signed int �̕K�v������ ) */
	s32 game_rank;		/* �����N�ϓ��V�X�e���̃Q�[�������N */
	unsigned int jiki_weapon_level; 	/* (r35u2)�����̂� */		//	/*u8*/unsigned int jiki_weapon_level_offset;/* �����������탌�x��(�ԍ�)�̂W�{�Ƀv���C���[�l�𑫂����l���I�t�Z�b�g�Ƃ��ĕێ� */
	unsigned int conv_bg_alpha;
//[0x30 12]
	int current_bg0_y_scroll_speed256;	/* bg0�̃X�N���[���A���ݑ��x */
	/*(�`��p)*/
	int draw_boss_hp_value; /* �{�Xhp�`��l */ ///??????????
	#if (1)/*(���Ή�)*/
	/* �b�� SDL �t�H���g */
//	int dummy111;/*�����ƍI�����삵�Ȃ�*/
//	�b�� [���C�������_�[�Ɉڍs�\��B]
	int msg_time;/* ���b�Z�[�W(���Ή�)�\������ */	/* ����ӃE�B���h�E�\������(��) */
	#endif
//	/* �W�v�V�X�e��(player_data) */
	int player_data_use_continue;	/* �R���e�B�j���[�� */
//[0x40 16]
	int player_data_count_miss; 	/* �~�X�� */
	int player_data_used_bomber;	/* �{���g�p�� */
	int player_data_use_kurai_bomb; /* ��炢�{�������� */
	int player_data_count_bonus;	/* �J�[�h�{�[�i�X�� */
//[0x48 18]
//	���̏ꏊ�� 4[byte]���E�ł̓_���ŁA���̂�32[byte]���E�ɍ��킹�Ȃ��Ɛ��퓮�삵�Ȃ��B(4[byte]���E�łُ͈퓮�삷��)
	s8 game_now_stage;				/* ���݃X�e�[�W�ԍ� */
	s8 game_continue_stage; 		/* ���݃R���e�B�j���[����X�e�[�W�ԍ���ێ� */
	s8 game_now_max_continue;		/* �R���e�B�j���[�\�ȉ� */
	s8 game_practice_mode;			/* ���K���[�h */
//
	s8 side_panel_draw_flag;/* �p�l���\��on(0�ȊO)/off(0) */
	s8 chuu_boss_mode;
	s8 dummy_status;				/* [���g�p]�_�~�[ */
	u8 player_option_mode;/* �����ɓ���Ȃ��������������H�A�Ƃ肠���� */
//	int dummy2222;/*�����Ɖ��̂��I�����삵�Ȃ�*/
//	int dummy3333;/*�����Ɖ��̂��I�����삵�Ȃ�*/
	#if (1)/*(���Ή�)*/
	/* �b�� SDL �t�H���g */
	int PSPL_font_x;
	int PSPL_font_y;
	#endif
//[0x50 20]
//[0x60 24]
} GAME_CORE_GLOBAL_CLASS;
extern GAME_CORE_GLOBAL_CLASS cg;
// /* �\��2(.align���킹�ŕK�v) */
//	s8 dr aw_flag_ka iwa_screen;		/* ��b��ʂ̉�b�p����ӃE�B���h�E�\���t���O */



//
//	u8 laser_mode;	/* �Ă���(���[�U�[���[�h 0:off, 1:on) */
	/* �W�v�V�X�e���ȊO�̕ێ���� */
//	u8 game_select_player;

/* �Ӑ}�I�ɓ���Ȃ����� */
extern unsigned int cg_game_select_player;/* : (�����͂Ƃ�����)����(r33)�� GAME_CORE_GLOBAL_CLASS�ɓ���Ȃ������ǂ����ۂ��B */



#if 1
//
//extern int bo ss_life_value;	/* �{�Xhp�̗͒l / �{�X�����w�T�C�Y�`��l */
#endif


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
#define ITEM_MOVE_FLAG_02_WAIT			0x02
//
#define ITEM_MOVE_FLAG_02_RAND_X		0x04
#define ITEM_MOVE_FLAG_04_RAND_Y		0x08
#define ITEM_MOVE_FLAG_06_RAND_XY		(ITEM_MOVE_FLAG_02_RAND_X|ITEM_MOVE_FLAG_04_RAND_Y)

#define ITEM_CREATE_MODE_01 	((4*8/*ITEM_MAX*/)*0)
#define ITEM_CREATE_MODE_02 	((4*8/*ITEM_MAX*/)*1)

/*(�A�C�e���쐬�n)*/
//�p�~extern void item_create(			OBJ *src, int type, int num, int up_flags);
extern void item_create_mono(			OBJ *src, int type);
extern void item_create_flags_num(		OBJ *src, int type, int num_of_creates, int set_item_flags);
extern void item_create_for_boss(		OBJ *src, int item_create_mode);
extern void item_create_random_table(	OBJ *src);

/*()*/
extern void bonus_info_score_nodel( 		OBJ *src, int score_type);		/*int x, int y*/
extern void bonus_info_any_score_nodel( 	OBJ *src, u32 score_num_pts);	/*int x, int y*/

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
//#define  MAX_PARA1_36		/*64*/(64-20-8-4+32)/*(64-(4*5))*/

typedef struct _stage_command_
{
	u32 		v_time; 				/* ����(0<v_time)�̏ꍇ 1/60 �P�ʂ̏o�����ԃJ�E���^�B(-1)�̏ꍇ�o���ς݁B(0�̏ꍇerror�������Ȃ��B) */
	struct _stage_command_ *next;
	s32 		user_locate_x;			/* [pixel�P��] locate_x */
	s32 		user_locate_y;			/* [pixel�P��] locate_y */
	s32 		user_angle1024;			/* [deg1024�P��] vector_x */
	s32 		user_speed256;			/* [t256�P��] vector_y */
//(16[Bytes] == (4x4) )
	u8			user_i_code;			/* intermediate code ���ԃR�[�h */
	u8			user_255_code;			/* 'E'�Ƃ�'T'�Ƃ�	*/
	u8			user_item8; 			/* item */
	u8			user_select_gazou;		/* �A�j��/�摜��� */
	int 		user_hp;				/* hp */
	int 		user_score; 			/* score */
	int 		user_kougeki_type;		/* �G���U������ꍇ�̃J�[�h�̎�� */
//(20[Bytes] == (4x5) )
//	char		user_string[MAX_PARA1_36/*(64)*/];/* strings */
//(64[Bytes] == (4x4)+(4)+(44) )
} GAME_COMMAND; 	/* .align 64 [bytes] */
//	u16 	dummy_scroll_speed256;	/* para3 scroll speed256 */


/* ���ԃR�[�h�`���̃R�}���h */
/* �Q�[�����́A���ԃR�[�h�`���̃R�}���h�������\����͂͂��Ȃ��B(�\����͂�load_stage.c�ōs��) */

enum /*_game_command_code_*/
{
/*���ԃR�[�h*/
/*0x07*/		GC_CODE_00_ZAKO = 0,				/* �G �U�R */
/*0x06*/		GC_CODE_01_CHUU_BOSS_COMMON,	/* �G �U�R��-�{�X(�X�N ���v�g�N�����Ȃ�) */
/*0x05*/		GC_CODE_02_BOSS_COMMON, 		/* �G (�{�X / ��-�{�X)�p �X�N ���v�g�N�� */
/*0x02*/		GC_CODE_03_BG_CONTROL,			/* �w�i�R���g���[���B(�X�N���[�����x��) */
//
/*0x01*/		GC_CODE_04_ALL_CLEAR,			/* �Q�[�� �S�X�e�[�W �N���A�[ */
/*0x01*/		GC_CODE_05_RESURVED,			/* �\�� */
/*0x01*/		GC_CODE_06_RESURVED,			/* �\�� */
/*0x01*/		GC_CODE_07_RESURVED,			/* �\�� */
//
/*0x00*/	//	GC_CODE_00_SJIS_TEXT = 0,		/* �����\��[�ԕ���؂�Ƃ��Ă��K�v] */
/*0x03*/	//	GC_CODE_03_CHECK_SECRET_BONUS,	/* �B���{�[�i�X�`�F�b�N "�o��" */
/*0x04*/	//	GC_CODE_04_BEGIN_SECRET_BONUS,	/* �B���{�[�i�X�`�F�b�N "�m�F" */
			//
/*0x08*/	/* �g���\ */
/*...*/ 	/* ... */
/*0x2f*/	/* �g���\ */
/*0x30*/	GC_CODE_MAX,		/* 0x30 == '0' == �J�[�h�R�}���h (== ���x�R�}���h�A�p�~) */
};

#endif /* _LOAD_STAGE_FILE_H_ */


#ifndef _BOSS_COMMON_H_
#define _BOSS_COMMON_H_

/* �J�[�h�V�X�e�� */

/* �J�[�h�̏������B�J�[�h���ς��Ɩ���s���K�v������B */
extern OBJ_CALL_FUNC(card_maikai_init); 						/*(���񏉊���)*/
extern OBJ_CALL_FUNC(card_maikai_init_and_get_spell_number);	/*(���񏉊���)+(���݌��ׂ��ԍ����J�[�h�V�X�e������擾)*/


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
extern OBJ_CALL_FUNC(bakuhatsu_add_rect);
extern void bakuhatsu_add_circle(OBJ/**/ *src, int mode);
extern OBJ_CALL_FUNC(bakuhatsu_add_zako04);

extern void callback_hit_zako(OBJ/**/ *src, OBJ/**/ *tama);

#endif /* _ATARI_HANTEI_H_ */
