
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
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
 "game_main.h" �̌�ŁA"kaiwa_system.h" �� ���ڃC���N���[�h���܂��B
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

/* 0:�g��Ȃ��B1:�g���B 1:�L�[�R���t�B�O�g���B0:�L�[�R���t�B�O�@�\OFF */
//#define USE_KEY_CONFIG (0)
#define USE_KEY_CONFIG (1)

/* 0:�g��Ȃ��B1:�g���B  [1:�L�[�R���t�B�O�g���ꍇ��] 1:�����L�[�̃L�[�R���t�B�O���g���B0:�����L�[�̃L�[�R���t�B�O�@�\OFF */
#define USE_KEY_CONFIG_ALLOW (0)

/* 0:���Ȃ��B 1:����B	�R���e�B�j���[�����ꍇ�A�X�R�A�����L���O */
#define USE_CONTINUED_RANKING	(0)

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


/* 0:���Ȃ��B 1:����B �p�[�T�[(�ݒ�ǂݍ��ݎ��̎�����)�ɂ��Ẵf�o�b�O�@�\�B */
#define USE_PARTH_DEBUG (0)






#define USE_32BIT_DRAW_MODE (0)

/* 0:���Ȃ��B 1:����B easy�̏ꍇ�A5�ʏI����BAD END�B */
//#define US E_EASY_BADEND (1)


/* 0:���Ȃ��B 1:����B ��蒆(r34)�̃e�X�g�B */
#define USE_GU_TACHIE_TEST (0)


#include "000_support.h"	/* (��ԍŏ��ɃC���N���[�h) */

/*---------------------------------------------------------
	�g�p�ݒ�(���ɃC���N���[�h)
	�{�V�X�e�����ǂ̂悤�Ɏg�����ʂ̐ݒ�
---------------------------------------------------------*/

/* �F�ݒ� */
#if (0==USE_32BIT_DRAW_MODE)
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
#else
		/* �F32bit���[�h */
		#define SDL_GU_PSM_0000 		GU_PSM_8888
		/* 5-6-5-0 */
		#define PSP_DEPTH16 			(32)
		#define SDL_5551_15 			(32)
		#define PSP_SCREEN_FORMAT_RMASK (0x000000ff)
		#define PSP_SCREEN_FORMAT_GMASK (0x0000ff00)
		#define PSP_SCREEN_FORMAT_BMASK (0x00ff0000)
		#define PSP_SCREEN_FORMAT_AMASK (0xff000000)
		#define PSP_SCREEN_FORMAT_LMASK (0xfefefefe)
#endif




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



enum /*_game_rank_*/
{
	RANK_EASY = 0,
	RANK_NORMAL,	/*=1*/
	RANK_HARD,		/*=2*/
	RANK_LUNATIC,	/*=3*/
	RANK_MAX		/* �����N�̍ő吔==(�ō������N+1) */
};

#include "000_my_math.h"
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
extern void story_mode_start(void);
extern void yume_no_kiroku_start(void);
extern void key_config_start(void);
extern void music_room_start(void);
extern void title_menu_start(void);/*for pause.c ... */
extern void pause_menu_start(void);
extern void ask_continue_menu_start(void);
//

#define pause_out_call_func  	return_call_func/* �|�[�Y���̖߂�� */
#define menu_out_call_func		return_call_func/* ���@�I�����̖߂�� / �I�v�V�������j���[�̖߂�� */

/*
	return_call_func
	�� �p�r1: pause_out_call_func : ����game_core��1�����Ȃ����ǁAgame_core�������ɂȂ�\��ł��̂ŁA������game_core�֖߂邩��ێ��B
	�� �p�r2: menu_out_call_func  : ���j���[�V�X�e���Ń��j���[���o�b�N����ꍇ�ɉ����̃��j���[�֖߂邩��ێ��B
*/
#endif

#if 1/*(���j���[�֘A)*/
/* �L�����Z������炵�ă��j���[�ɋ������A */
extern void menu_cancel_and_voice(void);
#endif

/*---------------------------------------------------------
	�X�v���C�g
---------------------------------------------------------*/

#include "000_sprite_system.h"
#include "000_gu_obj_type.h"
//#include "bullet_system.h"

//extern SPRITE *ob j_player;
extern SPRITE *global_obj_boss;/*(r35���݂̎d�l�ł́A�����蔻��̓s����v��B)*/




/*---------------------------------------------------------
	�e�V�X�e��
		�J�[�hCPU
		���e�V�X�e��
		���[�U�[�V�X�e��
---------------------------------------------------------*/

#include "000_tama_system.h"	/*(r33����ǉ�)*/

/*---------------------------------------------------------
	�X�y�������V�X�e��(�J�[�h�𐶐�)
---------------------------------------------------------*/

	/* �e��x256 �e��y256 �{�X���S���甭�e�B */
//#define set_REG_DEST_XY(aaa) {REG_02_SEND1_BOSS_X_LOCATE = (aaa->cx256);	REG_03_SEND1_BOSS_Y_LOCATE = (aaa->cy256); }
extern void set_REG_DEST_XY(SPRITE *src);
#if 0/*���ƂŗL���ɂ���*/
extern void tmp_angleCCW65536_src_nerai(void);
extern void calculate_jikinerai(void);

extern void spell_cpu_douchuu_init(void);
extern void card_maikai_init(SPRITE *src);
extern void card_generate(SPRITE *src);
#endif

/*---------------------------------------------------------
	�Q�[���R�A�T�|�[�g
---------------------------------------------------------*/

#include "000_core_support.h"

/*---------------------------------------------------------
	�Q�[���R�A�V�X�e��
---------------------------------------------------------*/

#include "000_game_core.h"


/* Gu Render */
#include "000_gu_render.h"


/* ��b�V�X�e������Ă΂����� */
#if 1
extern void called_from_kaiwa_system_boss_load(int boss_number);
#endif

#else
	include error!!
error! "���̃t�@�C���͉��x���C���N���[�h���܂���B"
#endif /* _MY_GAME_MAIN_H_ */
