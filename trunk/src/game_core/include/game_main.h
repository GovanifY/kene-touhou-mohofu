
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�C���N���[�h�t�@�C���ɂ���
---------------------------------------------------------*/

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


/* 0:���Ȃ��B 1:����B �����p�B�ꕔ�Ή����Ă��Ȃ��B�L���ɂ���ƂƂĂ��x���B */
#define USE_32BIT_DRAW_MODE (0)

/* 0:���Ȃ��B 1:����B easy�̏ꍇ�A5�ʏI����BAD END�B */
//#define US E_EASY_BADEND (1)


/* 0:���Ȃ��B 1:����B (r36)����0�Œ�B����̕�������1�͖����Ȃ̂ŁA���Ǘ\��B */
#define USE_AFTER_LOAD_STAGE (0)


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
		/*(PSPSDK�̏ꍇ5-5-5-1�B���R�n�[�h�E�F�A�[�A���t�@�͎g����)*/
		/*(psp��SDL�ł͓���ȑ��삵�Ȃ�����5-5-5-0�B�n�[�h�E�F�A�[�A���t�@�͋����I�Ɏg���Ȃ�)*/
		/* SDL�ƌ݊��������� 5-5-5-1 ���[�h�� 5-5-5-0 ���[�h�Ƃ����g���Ȃ��̂�
		 5-5-5-1 ���[�h�̈Ӗ����܂�Ŗ����B
		(�n�[�h�E�F�A�[�A���t�@������̂Ɏg���Ȃ�)
		(���x��5-6-5-0 ���[�h�Ɠ����ŉ摜�������Ȃ�B)
		(5-6-5-0 ���[�h�ɂ��������܂��B) */
	//	[SDL�̃o�O�Ɣ����B/SDL/video/SDL_pspvideo.c�ŋ����I��A=0�ɂ��Ă���B]
	//	SDL_ReallocFormat(aaa, video_bpp, Rmask, Gmask, Bmask, 0/*Amask*/);�������B
	//	[�����炭���x���x�߂����̂ŋ����I��A�Ȃ��ɂ����Ǝv����])
		/* 5-5-5-1 */
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


/*---------------------------------------------------------
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
	�e��}�N����
---------------------------------------------------------*/
#include "000_support.h"/* (����B��ԍŏ��ɃC���N���[�h) */

/*---------------------------------------------------------
	��{�֐�
---------------------------------------------------------*/
#include "000_my_math.h"

/*---------------------------------------------------------
	��{�J�[�h�X�N���v�g�p���ߌ^
---------------------------------------------------------*/
#define CPU_FUNC(aaa)  void aaa(void)

/*---------------------------------------------------------
	��{�R�[���o�b�N ("000_object_system.h"���O�ɃC���N���[�h)
---------------------------------------------------------*/
#define MAIN_CALL_FUNC(aaa) void aaa(void)
#define OBJ_CALL_FUNC(aaa)  void aaa(OBJ/**/ *src)

/*---------------------------------------------------------
	�X�v���C�g �I�u�W�F�N�g
---------------------------------------------------------*/

#include "000_object_system.h"
#include "000_gu_obj_type.h"/*("000_object_system.h"����ɃC���N���[�h)*/

/*---------------------------------------------------------
	�e�V�X�e��
	�J�[�hCPU
	���e�V�X�e��
	���[�U�[�V�X�e��
	���C���[�V�X�e��
---------------------------------------------------------*/

#include "000_tama_system.h"/*("000_object_system.h"����ɃC���N���[�h)*/

/*---------------------------------------------------------
	�Q�[���R�A�T�|�[�g
---------------------------------------------------------*/

#include "000_core_support.h"/*("000_tama_system.h"����ɃC���N���[�h)*/

/*---------------------------------------------------------
	�Q�[���R�A�V�X�e��
---------------------------------------------------------*/

#include "000_game_core.h"


/*---------------------------------------------------------
	�n�[�h�E�F�A�[�����_�����O�T�|�[�g
---------------------------------------------------------*/

#include "000_gu_render.h"



#else
	include error!!
error! "���̃t�@�C���͉��x���C���N���[�h���܂���B"
#endif /* _MY_GAME_MAIN_H_ */
