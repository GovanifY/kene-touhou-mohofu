
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_byteorder.h,v 1.9 2004/11/12 22:09:17 slouken Exp $";
#endif

#ifdef _SDL_H
error! "このプログラムは SDL.h がインクルードされていると、コンパイルできません。"
#endif

#ifdef _SDL_byteorder_h
error! "このプログラムは SDL_byteorder.h がインクルードされていると、コンパイルできません。"
#endif

/* Macros for determining the byte-order of this platform */

#ifndef _PSPL_byteorder_h
#define _PSPL_byteorder_h

/* The two types of endianness */
#define SDL_LIL_ENDIAN	1234
#define SDL_BIG_ENDIAN	4321

/* Pardon the mess, I'm trying to determine the endianness of this host.
   I'm doing it by preprocessor defines rather than some sort of configure
   script so that application code can use this too.  The "right" way would
   be to dynamically generate this file on install, but that's a lot of work.
 */
#if (defined(__i386__) || defined(__i386)) || \
     defined(__ia64__) || defined(WIN32) || \
    (defined(__alpha__) || defined(__alpha)) || \
     defined(__arm__) || \
    (defined(__mips__) && defined(__MIPSEL__)) || \
     defined(__SYMBIAN32__) || \
     defined(__x86_64__) || \
     defined(__LITTLE_ENDIAN__)
#define SDL_BYTEORDER	SDL_LIL_ENDIAN
#else
#define SDL_BYTEORDER	SDL_BIG_ENDIAN
#endif

#endif /* _PSPL_byteorder_h */
