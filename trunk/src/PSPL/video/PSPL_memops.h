
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	/src/SDL231/video/SDL_memops.h
	date:		2011/02/04
	source: 	http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2FSDL_memops.h
	revision:	440(2366以下では最新)
--------------------------------------------------------- */

/*
psp - リビジョン 440Subversion リポジトリ一覧: pspps2ps2wareps3ps3warepspwareリビジョン:(root)/trunk/SDL/src/video/SDL_memops.h
詳細表示 - 最終更新日時 - ログを見る -
*/

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_memops.h,v 1.5 2004/01/04 16:49:21 slouken Exp $";
#endif

#ifndef _PSPL_memops_h
#define _PSPL_memops_h

/* System dependent optimized memory manipulation routines:
*/
#include <string.h>

#if defined(__GNUC__) && defined(i386)
/* Thanks to Brennan "Bas" Underwood, for the inspiration. :)
 */
#define SDL_memcpy(dst, src, len)				\
do {											\
	int u0, u1, u2; 							\
	__asm__ __volatile__ (						\
		"cld\n\t"								\
		"rep ; movsl\n\t"						\
		"testb $2,%b4\n\t"						\
		"je 1f\n\t" 							\
		"movsw\n"								\
		"1:\ttestb $1,%b4\n\t"					\
		"je 2f\n\t" 							\
		"movsb\n"								\
		"2:"									\
		: "=&c" (u0), "=&D" (u1), "=&S" (u2)	\
		: "0" ((unsigned)(len)/4), "q" (len), "1" (dst),"2" (src) \
		: "memory" );							\
} while (0)

#define SDL_memcpy4(dst, src, len)			\
do {										\
	int ecx, edi, esi;						\
	__asm__ __volatile__ (					\
		"cld\n\t"							\
		"rep ; movsl"						\
		: "=&c" (ecx), "=&D" (edi), "=&S" (esi) 	\
		: "0" ((unsigned)(len)), "1" (dst), "2" (src)	\
		: "memory" );						\
} while (0)

#define SDL_revcpy(dst, src, len)			\
do {										\
	int u0, u1, u2; 						\
	char *dstp = (char *)(dst); 			\
	char *srcp = (char *)(src); 			\
	int n = (len);							\
	if ( n >= 4 )	{						\
	__asm__ __volatile__ (					\
		"std\n\t"							\
		"rep ; movsl\n\t"					\
		: "=&c" (u0), "=&D" (u1), "=&S" (u2)	\
		: "0" (n >> 2), 						\
		  "1" (dstp+(n-4)), "2" (srcp+(n-4))	\
		: "memory" );							\
	}											\
	switch (n & 3)	{						\
		case 3: dstp[2] = srcp[2];			\
		case 2: dstp[1] = srcp[1];			\
		case 1: dstp[0] = srcp[0];			\
			break;							\
		default:							\
			break;							\
	}										\
} while (0)

#define SDL_memmove(dst, src, len)			\
do {										\
	if ( (dst) < (src) )	{				\
		SDL_memcpy((dst), (src), (len));	\
	} else {								\
		SDL_revcpy((dst), (src), (len));	\
	}										\
} while (0)

#define SDL_memset4(dst, val, len)			\
do {										\
	int u0, u1, u2; 						\
	__asm__ __volatile__ (					\
		"cld\n\t"							\
		"rep ; stosl\n\t"					\
		: "=&D" (u0), "=&a" (u1), "=&c" (u2)			\
		: "0" (dst), "1" (val), "2" ((u32)(len)) 	\
		: "memory" );						\
} while (0)

#endif /* GNU C and x86 */

/* If there are no optimized versions, define the normal versions */
#ifndef SDL_memcpy
#define SDL_memcpy(dst, src, len)		memcpy(dst, src, len)
#endif

#ifndef SDL_memcpy4
#define SDL_memcpy4(dst, src, len)		memcpy(dst, src, (len) << 2)
#endif

#ifndef SDL_revcpy
#define SDL_revcpy(dst, src, len)		memmove(dst, src, len)
#endif

#ifndef SDL_memset4
#define SDL_memset4(dst, val, len)		\
do {									\
	unsigned _count = (len);			\
	unsigned _n = (_count + 3) / 4; 	\
	u32 *_p = (u32 *)(dst);		\
	u32 _val = (val);				\
	switch (_count % 4) 	{			\
	case 0: do {	*_p++ = _val;		\
	case 3:  *_p++ = _val;				\
	case 2:  *_p++ = _val;				\
	case 1:  *_p++ = _val;				\
		} while ( --_n );				\
	}									\
} while (0)
#endif

#endif /* _PSPL_memops_h */

