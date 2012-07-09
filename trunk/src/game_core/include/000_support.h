
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _SUPPORT_H_
#define _SUPPORT_H_

/*---------------------------------------------------------
	�T�|�[�g�ݒ�(��ԍŏ��ɃC���N���[�h)
	�{�V�X�e�����g����ł̊�{�I�ȃ}�N�����̃T�|�[�g�֘A
---------------------------------------------------------*/

// PSPSDK(standard)

#include <psptypes.h>
#include <pspaudio.h>
#include <pspctrl.h>
#include <pspdisplay.h>
//#include <pspgu.h>
//#include <pspgum.h>/* gum��gu����ɃC���N���[�h */
#include <pspiofilemgr.h>
#include <pspkernel.h>
#include <psppower.h>
#include <psprtc.h>

// C(standard)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>/*calloc*/

// PSPSDK(debug)

//#include "./../../debug/psp_debug.h"//#include <pspdebug.h>
//#include "psp_debug.h"//#include <pspdebug.h>
#ifdef ENABLE_PROFILE
	#include <pspprof.h>
#endif

// PSPSDK(optional)

//#include <pspsdk.h>
//#include <psputility.h>
//#include <pspmoduleinfo.h>
//#include <pspnet_apctl.h>
//#include <pspnet_inet.h>
//#include <pspnet_resolver.h>
//#include <netinet/in.h>
//#include "net_io.h"
//#include <pspuser.h>

// C(optional)

#include <unistd.h>
#include <stdarg.h>
#include <dirent.h>
#include <ctype.h>
#include <math.h>/*sin(),at an2()*/
//#include <signal.h>
//#include <sys/stat.h>


#ifdef ENABLE_PSP
	//# /* �J�X�^�����C�u�������g�� */
	#include "./../../PSPL/include/PSPL.h"//#include <SDL/SDL.h>//#include "SDL.h"
	#include "./../../PSPL/image/PSP_SDL_image.h"	//#include "SDL_image.h"
//	#include <SD L/SD L_mixer.h>//#include "SD L_mixer.h"
#else
	/* (r34)�A���T�|�[�g */
	//# /* �W�����C�u�������g�� */
	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>
//	#include <SDL/SDL_mixer.h>/*#include "SD L_mixer.h"*/
#endif

	//
	#ifndef offs_t
		#define offs_t unsigned int
	#endif

//	#ifndef TRUE
//		#define TRUE	(1)
//	#endif
//
//	#ifndef FALSE
//		#define FALSE	(0)
//	#endif

	#ifndef global
		/* static �Ȃ̂� global �Ȃ̂� ��������B */
		#define global /**/
	#endif

	#define my_TRUE 	(1)/**/
	#define my_FALSE	(0)/**/

	/* �f�o�b�O�������l�A���0. */
	#define TEST000 	(0)

#if (1)
/* RGBA5551:
	psp �ɂ� RGBA5551 (GU_PSM_5551) ���[�h������B������ A ��1�r�b�g�́A����������΃n�[�h�E�F�A�[��color-key�@�\�B
//
	������ SDL �ł� 5551 ���[�h�ɂ��Ă� A �� 1 �r�b�g�́A��������Ďg�������o���Ȃ��B(�܂苭���I��RGBA5550�ƂȂ�)
	(���ڃT�[�t�F�C�X�����������ꍇ��������)�uSDL�̂݁v(�̕`���)�g�p���Ă������ł́u�n�[�h�E�F�A�[��color-key�@�\�v�͎g�p�o���Ȃ��B
	 SDL �ł� 5551 ���[�h�̏ꍇ�Acolor-key��ʂɐݒ肵�A�u�\�t�g�E�F�A�[��color-key�@�\�v���g�p����B
//
	�ԈႢ//����APSPSDK �ł́u�n�[�h�E�F�A�[��color-key�@�\�v���g����BPSPSDK�ł�alpha-bit���g����B
	�ԈႢ//PSPSDK �ł́u�\�t�g�E�F�A�[��color-key�@�\�v�Ƃ������̂͑��݂��Ȃ��ׁA�t�Ɍ����΁A
	�ԈႢ//�K��������color-key��ݒ肵�Ȃ���΂Ȃ�Ȃ��B(PSPSDK�ł�alpha-bit==�����F==color-key)
//
	�ԈႢ//�܂�APSPSDK �ł� RGBA8888(GU_PSM_8888) �� RGBA5551(GU_PSM_5551) ���[�h�ł�color-key�@�\�����邪�A
	�ԈႢ//RGBA5650(GU_PSM_5650) ���[�h�ł�color-key�@�\�������
	�ԈႢ//(�n�[�h�E�F�A�[/�\�t�g�E�F�A�[�Ɋւ�炸�A�����x�͐ݒ�ł��邪color-key(�����F)��������������@���Ȃ�)

	PSPSDK �ł� sceGuBlendFunc(); �� color-key ���ݒ�ł���݂����B

void sceGuBlendFunc(int op, int src, int dest, unsigned int srcfix, unsigned int destfix);

���l:
�u�����h�֐���ݒ肵�܂��BDirectX�ň����Ƃ���́u�u�����h�t�@�N�^�[�v�ɊY�����܂��B
�`���Ɋ��ɑ��݂���F�Ə������ޗ\��̐F���r�A���Z���Ď��ۂɏ������ސF�����肵�܂��B

op�ɐݒ�ł���萔�͈ȉ��̒ʂ�

GU_ADD
GU_SUBTRACT
GU_REVERSE_SUBTRACT
GU_MIN
GU_MAX
GU_ABS
src��dest�ɐݒ�ł���萔�͈ȉ��̒ʂ�

GU_SRC_COLOR
GU_ONE_MINUS_SRC_COLOR
GU_SRC_ALPHA
GU_ONE_MINUS_SRC_ALPHA
GU_DST_ALPHA
GU_ONE_MINUS_DST_ALPHA
GU_DST_COLOR
GU_ONE_MINUS_DST_COLOR
GU_FIX
srcfix��destfix�ɐݒ肷��l�͂��ꂼ��src��dest��GU_FIX���g����ꍇ�̂ݓK�p����܂��B
R8G8B8A8�t�H�[�}�b�g�ŐF��ݒ肷�鎖�ɂȂ�܂����APSP�̓��g���G���f�B�A����CPU���g���Ă��邱�Ƃɒ��ӂ��Ă��������B


*/
	/* 16bit(15bit) RGBA5551 */
	/*
	15: abbb bbgg gggr rrrr
	15: -bbb b-gg gg-r rrr- (psp��SDL�ł�a���g���Ȃ��H�̂�)
	*/
	#define MAKECOL15A(r, g, b, a)	(((b & 0xf8) << 7) | ((g & 0xf8) << 2) | ((r & 0xf8) >> 3) | ((a & 0x80)<<8))
	#define MAKECOL15(r, g, b)		(((b & 0xf8) << 7) | ((g & 0xf8) << 2) | ((r & 0xf8) >> 3))
	#define GETR15F(col)			(((col << 3) & 0xf8) /*| ((col >>  2) & 0x07)*/)
	#define GETG15F(col)			(((col >> 2) & 0xf8) /*| ((col >>  7) & 0x07)*/)
	#define GETB15F(col)			(((col >> 7) & 0xf8) /*| ((col >> 12) & 0x07)*/)
	#define GETR15(col) 			(((col << 3) & 0xf8) | ((col >>  2) & 0x07))
	#define GETG15(col) 			(((col >> 2) & 0xf8) | ((col >>  7) & 0x07))
	#define GETB15(col) 			(((col >> 7) & 0xf8) | ((col >> 12) & 0x07))
#endif

#if (1)
	/* 16bit RGBA5650 */
	/*
	16: bbbb bggg gggr rrrr
	16: bbbb -ggg gg-r rrr-
	*/
	#define MAKECOL16A(r, g, b, a)	(((b & 0xf8) << 8) | ((g & 0xfc) << 3) | ((r & 0xf8) >> 3))
	#define MAKECOL16(r, g, b)		(((b & 0xf8) << 8) | ((g & 0xfc) << 3) | ((r & 0xf8) >> 3))
	#define GETR16F(col)			(((col << 3) & 0xf8) /*| ((col >>  2) & 0x07)*/)
	#define GETG16F(col)			(((col >> 3) & 0xfc) /*| ((col >>  8) & 0x03)*/)
	#define GETB16F(col)			(((col >> 8) & 0xf8) /*| ((col >> 13) & 0x07)*/)
	#define GETR16(col) 			(((col << 3) & 0xf8) | ((col >>  2) & 0x07))
	#define GETG16(col) 			(((col >> 3) & 0xf8) | ((col >>  8) & 0x07))
	#define GETB16(col) 			(((col >> 8) & 0xf8) | ((col >> 13) & 0x07))
#endif

#if (1)
	/* 32bit RGBA8888 */
	#define MAKE32RGBA(r, g, b, a)	(((a & 0xff) << 24) | ((b & 0xff) << 16) | ((g & 0xff) << 8) | (r & 0xff))
	#define GETR32(col) 			(((col		) & 0xff) )
	#define GETG32(col) 			(((col >>  8) & 0xff) )
	#define GETB32(col) 			(((col >> 16) & 0xff) )
	#define GETA32(col) 			(((col >> 24) & 0xff) )
#endif

/* PSP�̕\����ʗ̈� */
#ifndef PSP_BUFFER_WIDTH512
	#define PSP_BUFFER_WIDTH512 	(512)
#endif
#ifndef PSP_WIDTH480
	#define PSP_WIDTH480			(480)
#endif
#ifndef PSP_HEIGHT272
	#define PSP_HEIGHT272			(272)
#endif
#if 0/*(����)*/
�� psp�̏ꍇ�A�c�͘_�O�ő���Ȃ��B
�����ł����c�����薳���č����Ă�̂�
�c�̏㉺�ɘg��t����Ƃ����肦�Ȃ��B
���܂�ɏc�������ׂɁA���@���㉺�ɋ�ݏo����͎̂d�l�Ƃ���B
(�Ƃ������㉺�ɋ�ݏo������_���Ȃ���Ȃ����x���ŏc������Ȃ�)

�� psp�̏ꍇ�A���͂��܂�ɍL������B
(r39���_��windows�V���[�Y�Ɣ�r����2�{���炢�B
����Ɣ�r���Ă�1.5�{����(����̓A�X�y�N�g�䂪1:1�ł͂Ȃ��̂Œ���))

windows�V���[�Y��640x480�h�b�g(�h�b�g�����s�N�Z��)�̉�ʂ�`�����Ƃ���B
���̎����̒�����64cm�B�̏c������48cm�B�������Ƃ���B
(32�C���`���炢�̉�ʂ���?)
���̏ꍇ640[mm]x480[mm]�Ȃ̂�1�h�b�g�̑傫���́B���Ƃ�1�s�N�Z���̑傫���́A
�c��1[mm]x����1[mm]�̐����`�B

���x�͓����f�B�X�v���C�� pc-98x1���q���Bwindows�V���[�Y��480x400�h�b�g�`�����̂Ɠ����ʐς�
pc-98x1�ł� 640x400�s�N�Z�����f��B���̎����̒�����64cm�B�̏c������48cm�B
�c����������40cm�������Ȃ�windows�V���[�Y�Ɠ��������ǁA���̒�����48cm�B
���Ƃ�1�s�N�Z���̑傫���́A�c��1.2[mm]x����1[mm]�̏c�ɒ��������`�B�G�~���ł͍Č��ł��Ȃ��B
--------------------
�� windows�̏ꍇ�B384x448[pixel]�B�A�X�y�N�g���psp�Ɠ���1:1�Ȃ̂ŒP���ɏk������ƁA
384x448 == 233x272
384 == 233 x (1.64806866952789699570815450643777...)
448 == 272 x (1.64806866952789699570815450643777...)
psp�ŉ���233[pixel]�B�ł�psp�̃A�X�y�N�g���1:1����Ȃ��Đ��p�[�Z���g����Ă�B
�Ȃ�psp�͉��ɒ����B���ۂ�224[pixel]�Ƃ��B(�����Ƌt�Z���Ȃ��Ɛ��m�Ȓl���o�Ȃ�)
233 == 1.37339055793991416309012875536481 �{ (320/233) (�A�X�y�N�g�䂪1:1�Ȃ炱�������ǎ��ۂɑ��肷��ƈႤ)
224 == 1.42857142857142857142857142857143 �{ (320/224)
�݌v���psp�̓A�X�y�N�g�䂪��1:1�̂���Ȃ񂾂낤���ǁA���ۂɑ��肵�Ă݂��psp�͑S�R1:1����Ȃ��B
 windows�̏ꍇ�̏k���C���[�W�Ƃ��čl�����ꍇ�Ȃ�A
r39���_�Ō����ɂ͉���1.4�{���炢(1.4�{�ȏ�)�L���Ȃ��Ă�B

448/384����1.16666666666666666666666666666667�{
--------------------
�� ����̏ꍇ�B
����̏ꍇ�Ȃɂ�蒍�ӂ��Ȃ���΂Ȃ�Ȃ��̂��A�X�y�N�g��͖�4:3�Ȏ��B
windows�͖�4:4(==1:1)�B�c��1.2�{�����̂ŏc�̃h�b�g����1.2�{���Ȃ��ƂȂ�Ȃ��B(480==400x1.2)
����̏ꍇ�B384x368[pixel]�B�A�X�y�N�g�䂪1:1�Ɋ��Z����ƃQ�[����ʂ�384x441.6[pixel]�ł���B
windows�̏ꍇ�Ɋ��Z����384x441.6[pixel]�̖ʐς�`���ƌ������B����̉�ʂ�6.4[pixel]���������Ȃ̂�
���炭���ʐς����m��Ȃ��B(�f�B�X�v���C�̒�������ŃA�X�y�N�g�䂪4:3�ɂȂ�Ȃ����B)
windows�̏ꍇ�Ɋ��Z����
�c�̒����� 98.5714285714285714285714285714286% (==98.5%) ����͖w�Ǔ����ƌ��������Ӗ�����B

441.6/384����1.15�{
--------------------

psp�̃A�X�y�N�g���1:1�ɋ߂����ǁA�O���肵���琳�m��1:1����Ȃ������B
(�����psp�̉�ʂ͐��m��16:9�ł͖����ƌ������Œn�f�W�����k�����̂Ɣ����ɏc���䂪�Ⴄ�B(��̓���������))
(1:1�Ȃ�272x272�h�b�g����ʂɕ`���Ă��̂����ő���ƁA�c�Ɖ��̕����I�����������ɂȂ�B
psp�ł͏c�Ɖ��̒����������Ⴄ�B)
slice32                      128�Ȃ͕̂`��̊֌W��32�Ŋ���؂�鐔�̕��������B
+---+------------------------+-----------+
|32 |  ---  320[pixel] ---   | -- 128 -- | == 480[pixel]
|   |                        |           |
|   |                        |           |
|   |                        |           |
|   |                        |           |
|   |                        |           |
|   |                        |           |
|   |                        |           |
|   |                        |           |
|   |                        |           |
+---+------------------------+-----------+
(0) (32)                     (480-128)   (480)

#endif
/* �V���[�e�B���OGAME�����̕\����ʗ̈� */
//#define GAME_WIDTH			(320)/*(352)*/ /*(380)*/
#define PANEL_WIDTH 			(128)
#define GAME_X_OFFSET			(32)
#define GAME_320_WIDTH			(PSP_WIDTH480-PANEL_WIDTH-GAME_X_OFFSET)
#define GAME_WIDTH				(GAME_320_WIDTH+GAME_X_OFFSET)
#define GAME_HEIGHT 			(PSP_HEIGHT272)
//	GAME_X_OFFSET �Ή��B

/* x�ʒu��480�̏ꍇ(==481�h�b�g��) Scissor�������̂ŕK���\�����Ȃ� */
#define GAME_NOT_VISIBLE480 	(PSP_WIDTH480)

#if (1)
	/* �\�t�g�E�F�A�[�� Z�\�[�g */
	#define USE_ZBUFFER 	(0)
#else
	/* �n�[�h�E�F�A�[�Ń\�[�g */
	#define USE_ZBUFFER 	(1)
#endif


/* C:/cygwin/pspdev/psp/include/math.h �Ő錾����Ă���̂ŗv��Ȃ� */
//#ifndef M_PI
//	#define M_PI			(3.14159265358979323846)
//#endif

#ifndef T_GU_M_PI
	#define T_GU_M_PI		GU_PI
#endif

//#if 0
/* CW�̏ꍇ */
	/* �P����360�x�̒P�ʌn(deg360)���P�����Q�΂̒P�ʌn(radian)�֕ϊ��B�y�ыt�ϊ��B */
	#define deg360_2rad(x)		(((T_GU_M_PI*2)/(360.0))*(x))
	#define rad2deg360(x)		( (int)((x)*((360.0)/(T_GU_M_PI*2))+360)%360 )

	/* �P����512�x�̒P�ʌn(deg512)���P�����Q�΂̒P�ʌn(radian)�֕ϊ��B�y�ыt�ϊ��B */
//	#define deg512_2rad(x)		(((T_GU_M_PI*2)/(512.0))*(x))
	//#define rad2deg512(x) 	( (int)((x)*((512.0)/(T_GU_M_PI*2))+512)%512 )
	//#define rad2deg512(x) 	( (int)((x)*((512.0)/(T_GU_M_PI*2))+512)&(512-1) )
//	#define rad2deg512(x)		( (int)((x)*((512.0)/(T_GU_M_PI*2))/*+512*/)&(512-1) )

	/* �P����1024�x�̒P�ʌn(deg1024)���P�����Q�΂̒P�ʌn(radian)�֕ϊ��B�y�ыt�ϊ��B */
	#define deg1024_2rad(x) 	(((T_GU_M_PI*2)/(1024.0))*(x))
	//#define rad2deg1024(x)	( (int)((x)*((1024.0)/(T_GU_M_PI*2))+1024)%1024 )
	//#define rad2deg1024(x)	( (int)((x)*((1024.0)/(T_GU_M_PI*2))+1024)&(1024-1) )
	#define rad2deg1024(x)		( (int)((x)*((1024.0)/(T_GU_M_PI*2))/*+1024*/)&(1024-1) )

	/* �P����4096�x�̒P�ʌn(deg4096)���P�����Q�΂̒P�ʌn(radian)�֕ϊ��B�y�ыt�ϊ��B */
//	#define deg4096_2rad(x) 	(((T_GU_M_PI*2)/(4096.0))*(x))
	//#define rad2deg4096(x)	( (int)((x)*((4096.0)/(T_GU_M_PI*2))+4096)%4096 )
	//#define rad2deg4096(x)	( (int)((x)*((4096.0)/(T_GU_M_PI*2))+4096)&(4096-1) )
//	#define rad2deg4096(x)		( (int)((x)*((4096.0)/(T_GU_M_PI*2))/*+4096*/)&(4096-1) )

	/* �P����65536�x�̒P�ʌn(deg65536)���P�����Q�΂̒P�ʌn(radian)�֕ϊ��B�y�ыt�ϊ��B */
	#define deg65536_2rad(x)	(((T_GU_M_PI*2)/(65536.0))*(x))
	//#define rad2deg65536(x)	( (int)((x)*((65536.0)/(T_GU_M_PI*2))+65536)%65536 )
	//#define rad2deg65536(x)	( (int)((x)*((65536.0)/(T_GU_M_PI*2))+65536)&(65536-1) )
	#define rad2deg65536(x) 	( (int)((x)*((65536.0)/(T_GU_M_PI*2))/*+65536*/)&(65536-1) )

	/* �P����360�x�̒P�ʌn(deg360)���P����512�x�̒P�ʌn(deg512)�֕ϊ��B�y�ыt�ϊ��B */
//	#define deg_360_to_512(x)	((int)((x)*(512.0/360.0)))
//	#define deg_512_to_360(x)	((int)((x)*(360.0/512.0)))

	/* �P����360�x�̒P�ʌn(deg360)���P����1024�x�̒P�ʌn(deg1024)�֕ϊ��B�y�ыt�ϊ��B */
	#define deg_360_to_1024(x)	((int)((x)*(1024.0/360.0)))
	#define deg_1024_to_360(x)	((int)((x)*(360.0/1024.0)))

	/* �P����360�x�̒P�ʌn(deg360)���P����4096�x�̒P�ʌn(deg4096)�֕ϊ��B�y�ыt�ϊ��B */
	#define deg_360_to_4096(x)	((int)((x)*(4096.0/360.0)))
	#define deg_4096_to_360(x)	((int)((x)*(360.0/4096.0)))

	/* �P����360�x�̒P�ʌn(deg360)���P����65536�x�̒P�ʌn(deg65536)�֕ϊ��B�y�ыt�ϊ��B */
	#define deg360_to_65536(x)	((int)((x)*(65536.0/360.0)))
	#define deg65536_to_360(x)	((int)((x)*(360.0/65536.0)))

	/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
	/* �P����65536�x�̒P�ʌn(deg65536)���P����1024�x�̒P�ʂ̒P�ʌn(deg1024)�֕ϊ��B�y�ыt�ϊ��B */
	/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
	#define deg65536to1024(x)	((x)>>6)
	/* �u1����1024�����v����u1����65536�����v�֕ϊ�����B */
	#define deg1024to65536(x)	((x)<<6)

//#else
/* CCW�̏ꍇ */

	/* �P����360�x�̒P�ʌn(deg360)���P����512�x�̒P�ʌn(deg512)�֕ϊ��B */
//	#define cv512r(x)		((int)((x)*(512.0/360.0)))
	/* �P����360�x�̒P�ʌn(deg360)���P����1024�x�̒P�ʌn(deg1024)�֕ϊ��B */
	#define cv1024r(x)		((int)((x)*(1024.0/360.0)))
//#endif

/* �P�� �͈͓̔��ɃN���b�s���O */
#define mask256(aaa)		{aaa &= (256-1);}/* psp�ł� unsigned char �ɃL���X�g����̂ƃR�[�h�͑����ɂȂ�B */
//#define mask512(aaa)		{aaa &= (512-1);}/*�͕핗��r31���݁A������*/
#define mask1024(aaa)		{aaa &= (1024-1);}/*�W�����x�e*/
//#define mask4096(aaa) 	{aaa &= (4096-1);}/*�͕핗��r31���݁A������*/
#define mask65536(aaa)		{aaa &= (65536-1);}/*�����e*/	/* ��:���P */

/*  1/n�� �͈͓̔��ɃN���b�s���O�B
	(���m�ɂ� 1/(2^bbb)�� �͈͓̔��ɃN���b�s���O�B)
	(bbb=1�̏ꍇ1/2��, bbb=2�̏ꍇ1/4��)
*/
#define mask_harf65536(aaa,bbb) 	{aaa = ((aaa+((65536>>(bbb+1))))&((65536>>(bbb))-1))-((65536>>(bbb+1)));}/*�����e*/


/*
	��:���P���������Ă�����psp�ł́AGCC(4.xx�ȍ~�Ȃ�)�� and���Z�Ȃǂ��Ȃ��B
	�P���ɃV���[�g�ɂ��邾���B
	2byte �ɂȂ��Ă邾��������Apsp�ł͑������B
*/

/* �������X�R�A�ɕϊ��B�y�ыt�ϊ��B */
#define score(x)	((u32)(((u32)(x))/10))
#define score_r(x)	((u32)(((u32)(x))*10))

/* ������256�Œ菬���_�`���ɕϊ��B�y��dou ble�֋t�ϊ��B */
#define t256(x) 				((int)((x)*256))
//#define t256_to_dou ble(x)	(((dou ble)(x))*(1.0/256.0))
#define t256_floor(x)			((x)>>(8))

/* ������8�Œ菬���_�`���ɕϊ��B�y��dou ble�֋t�ϊ��B */
#define t8(x)					((short)((x)*8))
//#define t8_to_dou ble(x)		(((dou ble)(x))*(1.0/8.0))
#define t8_floor(x) 			((x)>>(3))

/* �b�P�ʂ̎��Ԃ� */
#define byou60(x)	((int)(x*60))
/* ��b�P�ʂ̎��Ԃ� */
#define byou64(x)	((int)(x*64))


/* ���W�A���o�ő΍�Bpsp�� dou ble�̒l��ێ����邾���ł��x���B(�Ⴆ�Ί֐��̈����Ƃ�)
int�Œl��ێ����āA�g���x�ɕϊ��A�t�ϊ����������A�����Ƒ����B
�����Ń��W�A���o�ł���ׂ̃}�N�� */
#if 0
	/* �P����512�x�̒P�ʌn�� at an2(y,x), cos(x), sin(x) */
	#define at an_512(y,x)	(rad2deg512(at an2((y),(x))))
	#define co_s512(x)		(cos(deg512_2rad((x))))
	#define si_n512(x)		(sin(deg512_2rad((x))))
	/* �ςȖ��O�ɂȂ��Ă�̂� cos( �Ƃ� sin( �Ō������邽�� */
#else
//	#define SINTABLE_SIZE512	512
//	#define SINTABLE_MASK512	(SINTABLE_SIZE512-1)
//	#define SINTABLE_SIZE1024	1024
//	#define SINTABLE_MASK1024	(SINTABLE_SIZE1024-1)
//	#define SINTABLE_SIZE65536	65536
//	#define SINTABLE_MASK65536	(SINTABLE_SIZE65536-1)

//	#define OFFS_SIN512 		(0)
//	#define OFFS_COS512 		((SINTABLE_SIZE512/4)-1)/*127 90*/
//	#define OFFS_SIN1024		(0)
//	#define OFFS_COS1024		((SINTABLE_SIZE1024/4)-1)/*255 90*/
//	#define OFFS_SIN65536		(0)
//	#define OFFS_COS65536		((SINTABLE_SIZE65536/4)-1)/*255 90*/
//	extern int at an_512(int y, int x);
//	extern int at an_1024(int y, int x);
//	extern int at an_65536(int y, int x);
	#if 1
		#include "psp_vfpu.h"
	//	#define cos512(x)		((int)(int256_si n1024((((x+x)+OFFS_COS512+OFFS_COS512)&(1024-1)))))
	//	#define sin512(x)		((int)(int256_si n1024((((x+x)						 )&(1024-1)))))
	//	#define co s1024(x)		((int)(int256_si n1024((((x)+OFFS_COS1024)&(1024-1)))))
	//	#define si n1024(x)		((int)(int256_si n1024((((x) 			)&(1024-1)))))
//		#define cos65536(x) 	((int)(int256_si n1024((((x)+OFFS_COS65536)&(65536-1))>>6)))
//		#define sin65536(x) 	((int)(int256_si n1024((((x) 			 )&(65536-1))>>6)))
	//	#define cos65536(x) 	((int)(int256_si n1024((((x)+OFFS_COS65536)&(65536-1))>>6)))
	//	#define sin65536(x) 	((int)(int256_si n1024((((x) 			 )&(65536-1))>>6)))
	//	#define cos65536(x) 	((int)(int256_sin65536((((x)+OFFS_COS65536)&(65536-1)) )))
	//	#define sin65536(x) 	((int)(int256_sin65536((((x)			 )&(65536-1)) )))
	#endif
#endif

/* rand()�o�ő΍�B�{�Ƃɂ̓��v���C�@�\������B
���v���C�@�\���Č�����ׂɂ͓����̑g�ݍ��݊֐�rand()�͎g���Ȃ��B
�����̒������O�̋[�������̊֐���p�ӂ���K�v������B
���v���C�J�n����Q�[���J�n��(�ʂ̎n��)�ɗ����n��̏�����������B */
/* �ςȖ��O�ɂȂ��Ă�̂� rand( �Ō������邽�� */
#if 0
	#define ra_nd(x)		(rand(x))
#else
	extern int ra_nd(void);
#endif


/*
	min, max �� psp_min, psp_max �ɂ����B
	���R�͂Q����B
	--------------
1.	C++�����N����ꍇ(���Ȃ�����)
	std::min
	std::max
	������ŋ�J����̂ŁA
	min, max �� psp_min, psp_max �ɂ����B
	���������͕핗�� C�R���p�C���̕��ŁA C++�R���p�C�� �g���ĂȂ����ǁB
	--------------
2.	psp_min, psp_max �̏ꍇ�A�����͕K�� signed int �Ȃ̂ŁA
	����𖾎����Ă����K�v������B
	--------------
	�܂��͕핗�G���W��PC�ɗ��p�\�����B����l���Ȃ��Ǝv�����ǁB
*/

/* psp��p�̖���(psp��cpu�ɂ��������A�Z���u������)�����p����B(�g�ݍ��݊֐�) */
#ifdef psp_min
	!print_error("include file�̏��������������ł��B")
#endif
#ifdef psp_max
	!print_error("include file�̏��������������ł��B")
#endif
/* �����Ǝv�����ǔO�̈׏��� */
//#undef min
//#undef max
#define psp_min __builtin_allegrex_min
#define psp_max __builtin_allegrex_max



#if (1)/*(�A�Z���u���֘A)*/
extern int tiny_strcmp(char *aaa, const char *bbb);/* MIPS R4000�n�ɂ��킹�āA�œK�����Ă݂܂����B */
/*
	�]���f�[�^�[����������p�ł��B
	�]���f�[�^�[�����Ȃ�(256bytes�ȉ��Ƃ�)�̏ꍇ�A�ᑬ�����܂��B
	(64bytes�P�ʂœ]�����Ă��܂��B)
*/
extern void memset32(int *dest, int c, int count);
extern void memset32_uncached(int *dest, int c, int count);
extern void memcpy32(int *dest, int *src, int count); // 32bit word count
#endif/*(�Ă���)*/



#endif /* _SUPPORT_H_ */
