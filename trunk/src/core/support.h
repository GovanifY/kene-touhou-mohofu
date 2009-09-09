#ifndef _SUPPORT_H_
#define _SUPPORT_H_

// PSPSDK(standard)

#include <psptypes.h>
#include <pspaudio.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspiofilemgr.h>
#include <pspkernel.h>
#include <psppower.h>
#include <psprtc.h>

// C(standard)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

// PSPSDK(debug)

#include <pspdebug.h>
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
#include <math.h>/*sin(),atan2()*/
//#include <signal.h>


#ifdef ENABLE_PSP
	//# /* �J�X�^�����C�u�������g�� */
	#include "SDL.h"
	#include "SDL_image.h"
	#include "SDL_mixer.h"
#else
	//# /* �W�����C�u�������g�� */
	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>
	#include <SDL/SDL_mixer.h>/*#include "SDL_mixer.h"*/
#endif

	//
	#ifndef UINT8
		#define UINT8 unsigned char
	#endif
	#ifndef UINT16
		#define UINT16 unsigned short
	#endif
	#ifndef UINT32
		#define UINT32 unsigned int
	#endif
	//
	#ifndef INT8
		#define INT8 signed char
	#endif
	#ifndef INT16
		#define INT16 signed short
	#endif
	#ifndef INT32
		#define INT32 signed int
	#endif
	//
	#ifndef offs_t
		#define offs_t unsigned int
	#endif

//	#ifndef TRUE
//		#define TRUE 1
//	#endif
//
//	#ifndef FALSE
//		#define FALSE 0
//	#endif




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
	#define MAKECOL15A(r, g, b, a)	(((b & 0xf8) << 7) | ((g & 0xf8) << 2) | ((r & 0xf8) >> 3)|((a & 0x80)<<8))
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
	#define GETR32(col) 			(((col      ) & 0xff) )
	#define GETG32(col) 			(((col >>  8) & 0xff) )
	#define GETB32(col) 			(((col >> 16) & 0xff) )
	#define GETA32(col) 			(((col >> 24) & 0xff) )
#endif

/* PSP�̕\����ʗ̈� */
#define BUF_WIDTH512	(512)
#define PSP_WIDTH480	(480)
#define PSP_HEIGHT272	(272)

/* �V���[�e�B���OGAME�����̕\����ʗ̈� */
#define GAME_WIDTH		(380)
#define GAME_HEIGHT 	(272)

#if 1
	/* �P���g�� */
	#define USE_ZOOM_XY 	0
#else
	/* �c���g�� */
	#define USE_ZOOM_XY 	1
#endif

/* C:/cygwin/pspdev/psp/include/math.h �Ő錾����Ă���̂ŗv��Ȃ� */
//#ifndef M_PI
//	#define M_PI			(3.14159265358979323846)
//#endif

//#define M_PI_H			(0.52359877559829887307)		/* �� �� 6 */

//#define degtorad(x)		(((T_GU_M_PI*2)/360.0)*(x))		/* 2�� �� 360 * X */
//#define radtodeg(x)		( (int)((x)*((360.0)/(T_GU_M_PI*2))+360)%360 )

#ifndef T_GU_M_PI
	#define T_GU_M_PI		GU_PI
#endif

/* �P����360�x�̒P�ʌn(deg360)���P�����Q�΂̒P�ʌn(radian)�֕ϊ��B�y�ыt�ϊ��B */
#define deg360_2rad(x)		(((T_GU_M_PI*2)/(360.0))*(x))
#define rad2deg360(x)		( (int)((x)*((360.0)/(T_GU_M_PI*2))+360)%360 )

/* �P����512�x�̒P�ʌn(deg512)���P�����Q�΂̒P�ʌn(radian)�֕ϊ��B�y�ыt�ϊ��B */
#define deg512_2rad(x)		(((T_GU_M_PI*2)/(512.0))*(x))
//#define rad2deg512(x) 	( (int)((x)*((512.0)/(T_GU_M_PI*2))+512)%512 )
//#define rad2deg512(x) 	( (int)((x)*((512.0)/(T_GU_M_PI*2))+512)&(512-1) )
#define rad2deg512(x)		( (int)((x)*((512.0)/(T_GU_M_PI*2))/*+512*/)&(512-1) )

/* �P����4096�x�̒P�ʌn(deg4096)���P�����Q�΂̒P�ʌn(radian)�֕ϊ��B�y�ыt�ϊ��B */
#define deg4096_2rad(x) 	(((T_GU_M_PI*2)/(4096.0))*(x))
//#define rad2deg4096(x)	( (int)((x)*((4096.0)/(T_GU_M_PI*2))+4096)%4096 )
//#define rad2deg4096(x)	( (int)((x)*((4096.0)/(T_GU_M_PI*2))+4096)&(4096-1) )
#define rad2deg4096(x)		( (int)((x)*((4096.0)/(T_GU_M_PI*2))/*+4096*/)&(4096-1) )

/* �P����65536�x�̒P�ʌn(deg65536)���P�����Q�΂̒P�ʌn(radian)�֕ϊ��B�y�ыt�ϊ��B */
#define deg65536_2rad(x)	(((T_GU_M_PI*2)/(65536.0))*(x))
//#define rad2deg65536(x)	( (int)((x)*((65536.0)/(T_GU_M_PI*2))+65536)%65536 )
//#define rad2deg65536(x)	( (int)((x)*((65536.0)/(T_GU_M_PI*2))+65536)&(65536-1) )
#define rad2deg65536(x) 	( (int)((x)*((65536.0)/(T_GU_M_PI*2))/*+65536*/)&(65536-1) )

/* �P����360�x�̒P�ʌn(deg360)���P����512�x�̒P�ʌn(deg512)�֕ϊ��B�y�ыt�ϊ��B */
#define deg_360_to_512(x)	((int)((x)*(512.0/360.0)))
#define deg_512_to_360(x)	((int)((x)*(360.0/512.0)))

/* �P����360�x�̒P�ʌn(deg360)���P����1024�x�̒P�ʌn(deg1024)�֕ϊ��B�y�ыt�ϊ��B */
#define deg_360_to_1024(x)	((int)((x)*(1024.0/360.0)))
#define deg_1024_to_360(x)	((int)((x)*(360.0/1024.0)))

/* �P����360�x�̒P�ʌn(deg360)���P����4096�x�̒P�ʌn(deg4096)�֕ϊ��B�y�ыt�ϊ��B */
#define deg_360_to_4096(x)	((int)((x)*(4096.0/360.0)))
#define deg_4096_to_360(x)	((int)((x)*(360.0/4096.0)))

/* �P����360�x�̒P�ʌn(deg360)���P����65536�x�̒P�ʌn(deg65536)�֕ϊ��B�y�ыt�ϊ��B */
#define deg360_to_65536(x)	((int)((x)*(65536.0/360.0)))
#define deg65536_to_360(x)	((int)((x)*(360.0/65536.0)))

/* �P���͈͓̔��ɃN���b�s���O */
#define mask512(aaa)	{aaa &= (512-1);}
#define mask4096(aaa)	{aaa &= (4096-1);}

/* �������X�R�A�ɕϊ��B�y�ыt�ϊ��B */
#define score(x)	((int)(((int)(x))/10))
#define score_r(x)	((int)(((int)(x))*10))

/* ������256�Œ菬���_�`���ɕϊ��B�y��dou ble�֋t�ϊ��B */
#define t256(x) 				((int)((x)*256))
//#define t256_to_dou ble(x)	(((dou ble)(x))*(1.0/256.0))
#define t256_floor(x)			((x)>>(8))

/* ������8�Œ菬���_�`���ɕϊ��B�y��dou ble�֋t�ϊ��B */
#define t8(x)					((short)((x)*8))
//#define t8_to_dou ble(x)		(((dou ble)(x))*(1.0/8.0))
#define t8_floor(x) 			((x)>>(3))


/* aaa%�̊m���� */
#define rand_percent(aaa) ((unsigned char)(ra_nd())<=(unsigned char)( (aaa*256)/100 ) ) /* aaa%�̊m���� */

/* ���W�A���o�ő΍�Bpsp�� dou ble�̒l��ێ����邾���ł��x���B(�Ⴆ�Ί֐��̈����Ƃ�)
int�Œl��ێ����āA�g���x�ɕϊ��A�t�ϊ����������A�����Ƒ����B
�����Ń��W�A���o�ł���ׂ̃}�N�� */
#if 0
	/* �P����512�x�̒P�ʌn�� atan2(y,x), cos(x), sin(x)  */
	#define atan_512(y,x)	(rad2deg512(atan2((y),(x))))
	#define co_s512(x)		(cos(deg512_2rad((x))))
	#define si_n512(x)		(sin(deg512_2rad((x))))
	/* �ςȖ��O�ɂȂ��Ă�̂� cos( �Ƃ� sin( �Ō������邽�� */
#else
	#define SINTABLE_SIZE	512
	#define SINTABLE_MASK	(SINTABLE_SIZE-1)

	#define OFFS_SIN 0
	#define OFFS_COS ((SINTABLE_SIZE/4)-1)/*127 90*/
	extern int atan_512(int y, int x);
	extern int sin_tbl512[SINTABLE_SIZE];
	#define cos512(x)		(sin_tbl512[(((x)+OFFS_COS)&(512-1))])
	#define sin512(x)		(sin_tbl512[(((x)		  )&(512-1))])
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





/* �L���b�V���Ȃ�VRAM  VRAM[y][x]�Ƃ��ăA�N�Z�X */
//#define	VRAM		((long(*)[BUF_WIDTH])(((char*)0x4000000)+0x40000000))


#endif /* _SUPPORT_H_ */
