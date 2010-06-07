
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�C���N���[�h�t�@�C���ɂ���
---------------------------------------------------------*/

#if (0)

�t�@�C�����̐擪�� "000_" �Ŏn�܂镨�� "game_main.h" �ȊO����C���N���[�h���Ȃ��ł��������B

����� "000_" �Ŏn�܂镨�� ����P�ɕ������Ă��邾���ŁA�������Ă���Ӗ��͂��܂肠��܂���B
����ɂ����Ȃ�Ȃ��Ȃ�΁A�K���ɐ������ēZ�߂�\��ł��B
(000_audio.h �Ƃ� 000_support.h �Ƃ��́A����ł����S�ɋ@�\�ŕ�������Ă��܂��B
�����͓Z�߂�Ɣ���ɂ����Ȃ�̂œZ�߂܂���B)

��{�I�� "game_main.h" ���C���N���[�h���āA����ȊO�̓C���N���[�h���܂���B
��O�͂���܂��B

�ȉ���O
---------------------------------------------------------
�� �V�i���I�֘A�͍l�����Ȃ̂ŁA�V�i���I�֘A���g���ꍇ(��Ɋ����`��)
 "game_main.h" �̌�ŁA"scenario_script.h" �� ���ڃC���N���[�h���܂��B
---------------------------------------------------------
�� �t�@�C���֘A���g���\�肪����܂��B�t�@�C���֘A���g���ꍇ�A
 "game_main.h" �̌�ŁA"111_my_file.h" �� ���ڃC���N���[�h���܂��B
---------------------------------------------------------
�� �����̏ꍇ�A�������ʏ��������邩������Ȃ��̂ŁA
 "game_main.h" �̑���� "douchu.h" �� ���ڃC���N���[�h���܂��B
---------------------------------------------------------
�� �{�X�̏ꍇ�A�{�X���ʏ��������邩������Ȃ��̂ŁA
 "game_main.h" �̑���� "boss.h" �� ���ڃC���N���[�h���܂��B
---------------------------------------------------------
�� �`��֘A�ꍇ�A�d�l�������ς茈�܂��ĂȂ��̂ŗ�O�ł��B
---------------------------------------------------------

#endif /* (0) */

/*---------------------------------------------------------
	��{�ݒ�
	�{�Q�[���̊�{�ݒ�����̃t�@�C���ɏ����܂��B
---------------------------------------------------------*/

#ifndef _MY_GAME_MAIN_H_
#define _MY_GAME_MAIN_H_

/*	�C���N���[�h�t�@�C�����̂̊�{�ݒ�	*/

/* 0:���Ȃ��B 1:����B [�������[���N���A�[�@�\]
	1:�Ȃ�Amy_calloc()�����ۂ� �m�ۂ�����������0�N���A�[���܂��B0�Ȃ炵�܂���B
	��ʂɒe���o���ꍇ�ǂ��炪����������܂���(0:�������o�O��\���A1:�x��������x�͍���)���A
	1:�̕��������ȈӖ��Ńo�O�}���ł��܂��B */
//#define USE_MEM_CLEAR (0)
#define USE_MEM_CLEAR (1)

/*�p�~*/	/* 0:�g��Ȃ��B1:�g���B 1:�G���f�B���O�f�o�b�O�@�\�B0:���̋@�\OFF */
/*�p�~*/	//#define US E_ENDING_DEBUG (1)
/*�p�~*/	//#define MA X_STAGE6_FOR_CHECK (6/*5*/)

/* 0:�g��Ȃ��B1:�g���B 1:�L�[�R���t�B�O�g���B0:�L�[�R���t�B�O�@�\OFF */
//#define USE_KEY_CONFIG (0)
#define USE_KEY_CONFIG (1)

/* 0:�g��Ȃ��B1:�g���B  [1:�L�[�R���t�B�O�g���ꍇ��] 1:�����L�[�̃L�[�R���t�B�O���g���B0:�����L�[�̃L�[�R���t�B�O�@�\OFF */
#define USE_KEY_CONFIG_ALLOW (0)

/* 0:���Ȃ��B 1:����B	�R���e�B�j���[�����ꍇ�A�X�R�A�����L���O */
#define USE_CONTINUED_RANKING (1)

/* 0:���Ȃ��B 1:����B �G�N�X�e���h�`�F�b�N */
//#define USE_EXTEND_CHECK (0)
#define USE_EXTEND_CHECK (1)

/* 0:���Ȃ��B 1:����B �J���X�g(�X�R�A�J�E���^�[ �X�g�b�v)�`�F�b�N */
//#define USE_MAX_SCORE_COUNTER_STOP_CHECK (0)
#define USE_MAX_SCORE_COUNTER_STOP_CHECK (0)
/*
�J���X�g�� 99,9999,9990pts (99���_)�ł����A�`���b�g���炢�̒�������J���X�g�������ɂȂ��B
(�O���C�Y��10���̈ʂ܂ŉ҂�����A���낢��������Ă݂�����...)
�A���X�F�J���X�g�Ȃ�Ė�����I
*/



/* 0:���Ȃ��B 1:����B ��ver r31�ł� 0 �Œ�B���� 1 �ɂł��Ȃ��B�p�~�\��B */
#define USE_SIN_TABLE (0)

/* 0:���Ȃ��B 1:����B �p�[�T�[(�ݒ�ǂݍ��ݎ��̎�����)�ɂ��Ẵf�o�b�O�@�\�B */
#define USE_PARTH_DEBUG (0)


#include "000_support.h"	/* (��ԍŏ��ɃC���N���[�h) */

/*---------------------------------------------------------
	�g�p�ݒ�(���ɃC���N���[�h)
	�{�V�X�e�����ǂ̂悤�Ɏg�����ʂ̐ݒ�
---------------------------------------------------------*/

/* �F�ݒ� */
#define USE_PSP_5551	0
//#define USE_PSP_5551	1
#if (1==USE_PSP_5551)
	#define SDL_GU_PSM_0000 		GU_PSM_5551
	/*(PSPSDK�̏ꍇ5-5-5-1)*/
	/*(psp��SDL�ł͓���ȑ��삵�Ȃ�����5-5-5-0) */
	/* 5-5-5-1*/
	#define PSP_DEPTH16 			(16)
	#define SDL_5551_15 			(15)
	#define PSP_SCREEN_FORMAT_RMASK (0x001f)
	#define PSP_SCREEN_FORMAT_GMASK (0x03e0)
	#define PSP_SCREEN_FORMAT_BMASK (0x7c00)
	#define PSP_SCREEN_FORMAT_AMASK (0x8000)
	#define PSP_SCREEN_FORMAT_LMASK (0x7bde)
#else
	#define SDL_GU_PSM_0000 		GU_PSM_5650
	/* 5-6-5-0 */
	#define PSP_DEPTH16 			(16)
	#define SDL_5551_15 			(16)
	#define PSP_SCREEN_FORMAT_RMASK (0x001f)
	#define PSP_SCREEN_FORMAT_GMASK (0x07e0)
	#define PSP_SCREEN_FORMAT_BMASK (0xf800)
	#define PSP_SCREEN_FORMAT_AMASK (0x0000)
	#define PSP_SCREEN_FORMAT_LMASK (0xf7de)
#endif

enum
{
	SDL_00_VIEW_SCREEN		= 0,
	SDL_01_BACK_SCREEN,
//	SDL_02_TEX_SCREEN,
//	SDL_03_000_SCREEN,
	SDL_99_MAX_SCREEN,
};

//extern SDL_Surface *screen;
extern SDL_Surface *sdl_screen[SDL_99_MAX_SCREEN];/*(4)*/

//1231101(Gu) 1229917(SDL)
//#define US E_GU			(1)

//(1==US E_GU)
//	#define SD L_VRAM_SCREEN	vr am_screen
	#define SDL_PSP_BUFFER_WIDTH512 PSP_BUFFER_WIDTH512
	#if 1
		/* �\�t�g�E�F�A�[�� Z�\�[�g */
		#define USE_ZBUFFER 	(0)
	#else
		/* �n�[�h�E�F�A�[�Ń\�[�g */
		#define USE_ZBUFFER 	(1)
	#endif

#if 0
// SDL */
	//#define SD L_VRAM_SCREEN		sdl_screen[SDL_00_VIEW_SCREEN]
	//#define SD L_PSP_BUFFER_WIDTH512	PSP_WIDTH480
#endif


enum /*_errlevel*/
{
	ERR_FATAL=0,
	ERR_WARN,
//	ERR_INFO,
//	ERR_DEBUG,
};
#if 0
	#define CHECKPOINT	error(ERR_DEBUG, "Checkpoint file:%s line:%d function:%s",__FILE__,__LINE__,__FUNCTION__)
#else
	#define CHECKPOINT	;
#endif


#if 0
enum PspCtrlButtons
{
	PSP_CTRL_SELECT 	= 0x00000001,	/* Select button. */
	PSP_CTRL_START		= 0x00000008,	/* Start button. */
	PSP_CTRL_UP 		= 0x00000010,	/* Up D-Pad button. */
	PSP_CTRL_RIGHT		= 0x00000020,	/* Right D-Pad button. */
	PSP_CTRL_DOWN		= 0x00000040,	/* Down D-Pad button. */
	PSP_CTRL_LEFT		= 0x00000080,	/* Left D-Pad button. */
	PSP_CTRL_LTRIGGER	= 0x00000100,	/* Left trigger. */
	PSP_CTRL_RTRIGGER	= 0x00000200,	/* Right trigger. */
	PSP_CTRL_TRIANGLE	= 0x00001000,	/* Triangle button. */
	PSP_CTRL_CIRCLE 	= 0x00002000,	/* Circle button. */
	PSP_CTRL_CROSS		= 0x00004000,	/* Cross button. */
	PSP_CTRL_SQUARE 	= 0x00008000,	/* Square button. */
	PSP_CTRL_HOME		= 0x00010000,	/* Home button. In user mode this bit is set if the exit dialog is visible. */
	PSP_CTRL_HOLD		= 0x00020000,	/* Hold button. */
	PSP_CTRL_NOTE		= 0x00800000,	/* Music Note button. */
	PSP_CTRL_SCREEN 	= 0x00400000,	/* Screen button. */
	PSP_CTRL_VOLUP		= 0x00100000,	/* Volume up button. */
	PSP_CTRL_VOLDOWN	= 0x00200000,	/* Volume down button. */
	PSP_CTRL_WLAN_UP	= 0x00040000,	/* Wlan switch up. */
	PSP_CTRL_REMOTE 	= 0x00080000,	/* Remote hold position. */
	PSP_CTRL_DISC		= 0x01000000,	/* Disc present. */
	PSP_CTRL_MS 		= 0x02000000,	/* Memory stick present. */
};
#endif

#define PSP_KEY_NONE			0
#define PSP_KEY_SELECT			PSP_CTRL_SELECT
#define PSP_KEY_PAUSE			PSP_CTRL_START
#define PSP_KEY_UP				PSP_CTRL_UP
#define PSP_KEY_RIGHT			PSP_CTRL_RIGHT
#define PSP_KEY_DOWN			PSP_CTRL_DOWN
#define PSP_KEY_LEFT			PSP_CTRL_LEFT
#define PSP_KEY_SYSTEM			PSP_CTRL_LTRIGGER
#define PSP_KEY_SLOW			PSP_CTRL_RTRIGGER
#define PSP_KEY_SNAP_SHOT		PSP_CTRL_TRIANGLE
#define PSP_KEY_OPTION			PSP_CTRL_CIRCLE
#define PSP_KEY_SHOT_OK 		PSP_CTRL_CROSS
#define PSP_KEY_BOMB_CANCEL 	PSP_CTRL_SQUARE

enum
{
	KEY_NUM00_SELECT = 0,	/* SELECT */
	KEY_NUM01_START,		/* START */
	KEY_NUM02_UP,			/* �� */
	KEY_NUM03_RIGHT,		/* �E */
	KEY_NUM04_DOWN, 		/* �� */
	KEY_NUM05_LEFT, 		/* �� */
	KEY_NUM06_L_TRIG,		/* L */
	KEY_NUM07_R_TRIG,		/* R */
	KEY_NUM08_TRIANGLE, 	/* �� */
	KEY_NUM09_CIRCLE,		/* �� */
	KEY_NUM10_CROSS,		/* �~ */
	KEY_NUM11_SQUARE,		/* �� */
	KEY_NUM12_MAX			/* �ő吔 */
};

extern u32 my_pad;		/*�������*/
extern u32 my_pad_alter; /*�O�����*/


//#define IS_KEY BOARD_PULLED ((0==my_pad)&&(my_pad^my_pad_alter))/* �����L�[�𗣂��ꂽ�� */
/*	�L�[�𗣂����u�Ԃ��m��Ƃ���B�������u�Ԃ��m�肾�ƁA�����ꂽ�Ԃ������삷�镨�������ɂ���Ɠs���������B */
/*
	r31:����ł̓L�[���������u�Ԃ��m��̂��̂������̂ŁA���������Ƃ���̓L�[���������u�Ԃ��m��ɏC�������B
*/
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
extern void common_load_init(void);
extern void stage_clear_result_screen_start(void);/* for game_core.c ??stage_clear.c */
//
extern void stage_first_init(void);/* for game_core.c select_player.c */
extern void shooting_game_core_work(void);/* for pause.c ask_continue.c */
extern void difficulty_select_menu_start(void);
extern void gameover_start(void);/* for ask_continue.c */
extern void name_entry_start(void);/* for game_over.c */
//
extern void stage_select_menu_start(void);
extern void option_menu_start(void);
extern void story_script_start(void);
extern void yume_no_kiroku_start(void);
extern void key_config_start(void);
extern void music_room_start(void);
extern void title_menu_start(void);/*for pause.c ... */
extern void pause_menu_start(void);
extern void ask_continue_menu_start(void);
//

#define pause_out_call_func 	return_call_func/* �|�[�Y���̖߂�� */
#define menu_out_call_func		return_call_func/* ���@�I�����̖߂�� */

extern void (*main_call_func)(void);/* ���C���R�[���o�b�N */
extern void (*return_call_func)(void);/* �|�[�Y���A��̖߂��R�[���o�b�N */
/*
	return_call_func
	�� �p�r1: pause_out_call_func : ����game_core��1�����Ȃ����ǁAgame_core�������ɂȂ�\��ł��̂ŁA������game_core�֖߂邩��ێ��B
	�� �p�r2: menu_out_call_func  : ���j���[�V�X�e���Ń��j���[���o�b�N����ꍇ�ɉ����̃��j���[�֖߂邩��ێ��B
*/
#endif

/*---------------------------------------------------------
	�R���t�B�O
---------------------------------------------------------*/

extern int pad_config[KEY_NUM12_MAX];
//extern int ke yconfig[KEY_NUM12_MAX];

/*---------------------------------------------------------
	�X�v���C�g
---------------------------------------------------------*/

#include "000_sprite_system.h"
//#include "bullet_system.h"

extern SPRITE *obj_player;
extern SPRITE *obj_boss;
extern SPRITE *obj_send1;

extern int player_now_stage;

extern int difficulty;

extern int msg_time;/* ���b�Z�[�W(���Ή�)�\������ */

//#include "font.h"
//#include "menu.h"
//#ifndef _FONT_H_
//#define _FONT_H_

/*---------------------------------------------------------
	�t�H���g
---------------------------------------------------------*/

enum FONTS
{
	FONT10W,	/*	8x10xWHITE �� (mini) */
	FONT16R,	/* 16x16xRED   �g */
	FONT16W,	/* 16x16xWHITE �� */
	FONT_MAX
};

extern void font_init(void);

/* �T�[�t�F�C�X�����Ȃ��ŁA���ډ�ʂɕ\�� */
extern void font_print_screen_xy(char *text, int fontnr, int x, int y);

/* �V�K�T�[�t�F�C�X���쐬���A������������_�����O */
extern SDL_Surface *font_render(char *text,int fontnr);

/* */
extern void font_print(char *text, int fontnr, int x, int y);

//#endif /* _FONT_H_ */


/* �L�����Z������炵�ă��j���[�ɋ������A */
extern void menu_cancel_and_voice(void);

#include "000_audio.h"

typedef struct
{
	char name[16/*4*/];
//
	u32 score;
	int player;
	int final_stage;	/* ���B�X�e�[�W */
	int difficulty; 	/* ��Փx */
//
	int use_continue;	/* �R���e�B�j���[�� */
	int count_miss; 	/* �~�X�� */
	int used_bomber;	/* �{���g�p�� */
	int count_bonus;	/* �X�y���J�[�h�{�[�i�X�� */
} SCORE_FORMAT;

#define DIRECTRY_NAME_DATA_STR			"data"
/* 'data' �̕������� 4 ���� */
#define DIRECTRY_NAME_DATA_LENGTH		(4)

#define DIRECTRY_NAME_TEXT_STR			"/text/"
/* '/text/' �̕������� 6 ���� */
#define DIRECTRY_NAME_TEXT_LENGTH		(6)

		/* 'data' �̕������� 4 ���� */
	//	#define DIRECTRY_NAME_DATA_LENGTH		(4) 	// game_main.h �Œ�`(�ς��\�������邩��)
		/* '/text/' �̕������� 6 ���� */
	//	#define DIRECTRY_NAME_TEXT_LENGTH		(6)

#define DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR			".txt"




#include "000_player.h"
#include "000_bonus.h"
#include "000_load_stage.h"
#include "000_danmaku.h"
#include "000_bullet_object.h"

//#ifndef _HIGH_SCORE_H_
//#define _HIGH_SCORE_H_

#define MAX_8_SAVE_PLAYERS	(8)
#define MAX_5_RANKING		(5)

extern SCORE_FORMAT high_score_table[MAX_8_SAVE_PLAYERS][MAX_5_RANKING/*5*/];

//#endif /* _HIGH_SCORE_H_ */

/* �I�v�V�������j���[�Őݒ肷��ݒ�l */

enum /*_option_config_*/
{
	OPTION_CONFIG_00_PLAYER = 0,
	OPTION_CONFIG_01_BOMB,
	OPTION_CONFIG_02_BGM,
	OPTION_CONFIG_03_SOUND,
	OPTION_CONFIG_04_CURRENT_DIFFICULTY,
	OPTION_CONFIG_05_CURRENT_PLAYER,
	OPTION_CONFIG_06_ANALOG,
	OPTION_CONFIG_07_OPEN,
	OPTION_CONFIG_08_MAX	/* �ő吔 */
};

extern int option_config[OPTION_CONFIG_08_MAX]; 	/* 8 */ 	// ����=�{�^���ϐ�

enum /*_bg_resource_*/
{
	BG_TYPE_00_title_bg = 0,
	BG_TYPE_01_name_regist,
	BG_TYPE_02_key_config,
	BG_TYPE_03_music_room,
//	BG_TYPE_xx_loading,
	BG_TYPE_99_MAX_HAIKEI,
};
extern void load_SDL_bg(int bg_type_number);


/* Gu Render */
#include "000_gu_render.h"
#if 1

extern int draw_side_panel;/* �p�l���\��on(0�ȊO)/off(0) */

extern int draw_boss_hp_value;	/* �{�Xhp�`��l */
extern int boss_life_value; 	/* �{�Xhp�̗͒l / �{�X�����w�T�C�Y�`��l */
#endif

extern void error(int errorlevel, char *msg, ...);
extern SDL_Surface *load_chache_bmp(char *filename);//, int use_alpha, int use_chache);

extern void unloadbmp_by_surface(SDL_Surface *s);

extern void *my_calloc(size_t size);


extern int tiny_strcmp(char *aaa, const char *bbb);/* MIPS R4000�n�ɂ��킹�āA�œK�����Ă݂܂����B */

extern char get_stage_chr(int i);

#if 1
extern void script_boss_load(int boss_number);
extern void script_boss_start(void/*int i*/);
#endif

extern void display_vidinfo(void);


extern void psp_pause_filter(void);/* ���z�X�N���[���ɂ������G�t�F�N�g�������� */
extern void psp_clear_screen(void);/* ���z�X�N���[�������ŏ��� */
//extern void psp_push_screen(void);/* ���z�X�N���[����ޔ� */
//extern void psp_pop_screen(void);/* ���z�X�N���[���𕜊� */
#define psp_push_screen(aaa) psp_move_screen( SDL_00_VIEW_SCREEN, SDL_01_BACK_SCREEN )
#define psp_pop_screen(aaa)  psp_move_screen( SDL_01_BACK_SCREEN, SDL_00_VIEW_SCREEN )
extern void psp_move_screen(int src_screen_number, int dst_screen_number );


#else
	include error!!
#endif /* _MY_GAME_MAIN_H_ */
