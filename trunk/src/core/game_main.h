#ifndef _GAME_MAIN_H_
#define _GAME_MAIN_H_

#include "support.h"

/*---------------------------------------------------------
  macros
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

//1231101(Gu) 1229917(SDL)
#define USE_GU			(1)
//#define USE_GU			(0)
#if (1==USE_GU)
	#define SDL_VRAM_SCREEN 	vram_screen
	#define SDL_BUF_WIDTH512	BUF_WIDTH512
	#if 1
		/* �\�t�g�E�F�A�[�� Z�\�[�g */
		#define USE_ZBUFFER 	(0)
	#else
		/* �n�[�h�E�F�A�[�Ń\�[�g */
		#define USE_ZBUFFER 	(1)
	#endif
#else
	#define SDL_VRAM_SCREEN 	sdl_screen[SDL_00_VIEW_SCREEN]
	#define SDL_BUF_WIDTH512	PSP_WIDTH480
#endif

enum /*_errlevel*/
{
	ERR_FATAL=0,
	ERR_WARN,
//	ERR_INFO,
//	ERR_DEBUG,
};
#if 0
	#define CHECKPOINT error(ERR_DEBUG, "Checkpoint file:%s line:%d function:%s",__FILE__,__LINE__,__FUNCTION__)
#else
	#define CHECKPOINT ;
#endif
//	ST_INIT_PLAYER_SELECT				�z���B�Ȃ�
//	ST_INIT_GAME_OVER					�z���B�Ȃ�
//	ST_INIT_RESULT						�z���B�Ȃ�
//	ST_INIT_KEY_CONFIG					�z���B�Ȃ�
//	ST_INIT_STORY						�z���B�Ȃ�
//	ST_INTRO,
//	ST_START_INTRO,
//	ST_GAME_DEMO,

enum /*_state*/
{
	ST_PSP_QUIT 				= 0x0000,
	ST_INIT_GAME_PLAY_common	= 0x0100,
	ST_WORK_GAME_PLAY			= 0x0200,
	ST_INIT_MENU				= 0x0300,
	ST_WORK_MENU				= 0x0400,
	ST_WORK_PLAYER_SELECT		= 0x0500,
	ST_dummy					= 0x0600,/* �\�� ST_INIT_NAME_ENTRY */
	ST_WORK_NAME_ENTRY			= 0x0700,
//
	ST_WORK_STAGE_FIRST 		= 0x0800,
	ST_WORK_STAGE_CLEAR 		= 0x0900,
//
	ST_WORK_GAME_OVER			= 0x0a00,
	ST_WORK_RESULT				= 0x0b00,

	ST_WORK_STORY				= 0x0c00,		/* [***20090223 �ǉ�  */
	ST_WORK_OPTION_MENU 		= 0x0d00,
	ST_WORK_KEY_CONFIG			= 0x0e00,
	ST_WORK_MUSIC_ROOM			= 0x0f00,
};
		#if 0
		switch ((u8)(psp_loop>>8))
		{
		case (ST_INIT_GAME_PLAY_common>>8): common_load_init(); 		break;
		case (ST_WORK_GAME_PLAY>>8):		shooting_game_core_work();	break;
		case (ST_INIT_MENU>>8): 			all_menu_init();			break;
		case (ST_WORK_MENU>>8): 			all_menu_work();			break;
//		case (ST_INIT_PLAYER_SELECT>>8):	player_opt_init();			break;�z���B�Ȃ�
		case (ST_WORK_PLAYER_SELECT>>8):	player_opt_work();			break;
//		case (ST_INIT_NAME_ENTRY>>8):		name_entry_init();			break;�z���B�Ȃ�
		case (ST_WORK_NAME_ENTRY>>8):		name_entry_work();			break;
//
		case (ST_WORK_STAGE_FIRST>>8):		stage_first_init(); 		break;
		case (ST_WORK_STAGE_CLEAR>>8):		stage_clear_work(); 		break;
//
	//	case (ST_INIT_GAME_OVER>>8):		gameover_init();			break;�z���B�Ȃ�
		case (ST_WORK_GAME_OVER>>8):		gameover_work();			break;
	//	case (ST_INIT_RESULT>>8):			result_init();				break;�z���B�Ȃ�
		case (ST_WORK_RESULT>>8):			result_work();				break;
	//	case (ST_INIT_KEY_CONFIG>>8):		key_config_init();			break;�z���B�Ȃ�
		case (ST_WORK_KEY_CONFIG>>8):		key_config_work();			break;
	//	case (ST_INIT_STORY>>8):			story_init();				break;�z���B�Ȃ�
		case (ST_WORK_STORY>>8):			story_work();				break;
	//	case (ST_INTRO>>8): 				intro_init();				break;
	//	case (ST_INTRO>>8): 				intro_work();				break;
	//	case (ST_START_INTRO>>8):			startintro_init();			break;
	//	case (ST_START_INTRO>>8):			startintro_work();			break;
		}
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


#define IS_KEYBOARD_PULLED ((0==my_pad)&&(my_pad^my_pad_alter))/* �����L�[�𗣂��ꂽ�� */
/*	�L�[�𗣂����u�Ԃ��m��Ƃ���B�������u�Ԃ��m�肾�ƁA�����ꂽ�Ԃ������삷�镨�������ɂ���Ɠs���������B */

enum /*_game_rank_*/
{
	RANK_EASY = 0,
	RANK_NORMAL,	/*=1*/
	RANK_HARD,		/*=2*/
	RANK_LUNATIC,	/*=3*/
	RANK_MAX		/* �����N�̍ő吔==(�ō������N+1) */
};

extern int psp_loop;

extern int pad_config[KEY_NUM12_MAX];
//extern int ke yconfig[KEY_NUM12_MAX];


//extern SDL_Surface *screen;
extern SDL_Surface *sdl_screen[SDL_99_MAX_SCREEN];/*(4)*/

#include "sprite_system.h"
#include "bullet_system.h"

extern SPRITE *player;
extern SPRITE *send1_obj;

extern int player_now_stage;

extern int difficulty;

extern int msg_time;/* ���b�Z�[�W(���Ή�)�\������ */

#include "font.h"
#include "menu.h"
#include "player.h"
//#include "fps.h"
#include "audio.h"//#include "so und_manager.h"

//#include "sta rtint ro.h"
//#include "particle.h"

//#include "_resource.h"

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

//extern char moddir[20];
//extern char data_dir[20];
#define DIRECTRY_NAME_DATA			"data"
#define DIRECTRY_NAME_LENGTH		(4)


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

extern int option_config[OPTION_CONFIG_08_MAX]; 	/* 8 */ 	// ����=�{�^���ϐ� ���т� "bg/key_haikei_surface.png"


/* Gu */
#if 1
extern void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;				/* �w�i�E�B���h�E�\���t���O */
extern int draw_side_panel;/* �p�l���\��on(0�ȊO)/off(0) */

extern int draw_boss_hp_value;	/* �{�Xhp�`��l */
extern int boss_life_value; 	/* �{�X�����w�T�C�Y�`��l */
#endif

extern void error(int errorlevel, char *msg, ...);
extern SDL_Surface *loadbmp0(char *filename, int use_alpha, int use_chache);
extern SDL_Surface *loadbmp1(char *filename);
extern SDL_Surface *loadbmp2(char *filename);


extern void unloadbmp_by_surface(SDL_Surface *s);

extern void *mmalloc(size_t size);


extern int tiny_strcmp(char *aaa, const char *bbb);/* MIPS R4000�n�ɂ��킹�āA�œK�����Ă݂܂����B */

extern void display_vidinfo(void);


extern void psp_pause_filter(void);/* ���z�X�N���[���ɂ������G�t�F�N�g�������� */
extern void psp_clear_screen(void);/* ���z�X�N���[�������ŏ��� */
//extern void psp_push_screen(void);/* ���z�X�N���[����ޔ� */
//extern void psp_pop_screen(void);/* ���z�X�N���[���𕜊� */
#define psp_push_screen(aaa) psp_move_screen( SDL_00_VIEW_SCREEN, SDL_01_BACK_SCREEN )
#define psp_pop_screen(aaa)  psp_move_screen( SDL_01_BACK_SCREEN, SDL_00_VIEW_SCREEN )
extern void psp_move_screen(int src_screen_number, int dst_screen_number );

/* 0:���Ȃ��B 1:����B [�������[���N���A�[�@�\]
	1:�Ȃ�Ammalloc()�����ۂ� �m�ۂ�����������0�N���A�[���܂��B0�Ȃ炵�܂���B
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


/* 0:���Ȃ��B 1:����B �쐬���Ȃ̂�1�ɏo���Ȃ��B */
#define USE_BOSS_COMMON_MALLOC (0)
//#define USE_BOSS_COMMON_MALLOC (1)


#endif /* _GAME_MAIN_H_ */
