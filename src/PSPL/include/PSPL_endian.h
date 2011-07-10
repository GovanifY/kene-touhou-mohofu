
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_endian.h,v 1.15 2005/03/30 12:38:03 pmandin Exp $";
#endif

#ifdef _SDL_H
error! "このプログラムは SDL.h がインクルードされていると、コンパイルできません。"
#endif

#ifdef _SDL_endian_h
error! "このプログラムは SDL_endian.h がインクルードされていると、コンパイルできません。"
#endif

/* Functions for reading and writing endian-specific values */

#ifndef _PSPL_endian_h
#define _PSPL_endian_h

/* These functions read and write data of the specified endianness,
   dynamically translating to the host machine endianness.

   e.g.: If you want to read a 16 bit value on big-endian machine from
	 an open file containing little endian values, you would use:
		value = SDL_ReadLE16(rp);
	 Note that the read/write functions use SDL_RWops pointers
	 instead of FILE pointers.  This allows you to read and write
	 endian values from large chunks of memory as well as files
	 and other data sources.
*/

#include <stdio.h>

#include "PSPL_types.h"
#include "PSPL_rwops.h"
#include "PSPL_byteorder.h"

//#inc lude "beg in_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Use inline functions for compilers that support them, and static
   functions for those that do not.  Because these functions become
   static for compilers that do not support inline functions, this
   header should only be included in files that actually use them.
*/

#if (0)/*(unused)*/
static __inline__ u16 SDL_Swap16(u16 x)
{
	return ((x<<8)|(x>>8));
}
static __inline__ u32 SDL_Swap32(u32 x)
{
	return ((x<<24)|((x<<8)&0x00ff0000)|((x>>8)&0x0000ff00)|(x>>24));
}
#endif


/* Byteswap item from the specified endianness to the native endianness */
//#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	#define SDL_SwapLE16(X) 	(X)
	#define SDL_SwapLE32(X) 	(X)
	#define SDL_SwapLE64(X) 	(X)
	//#define SDL_SwapBE16(X) 	SDL_Swap16(X)/*(unused)*/
	//#define SDL_SwapBE32(X) 	SDL_Swap32(X)/*(unused)*/
//#else
//	//#define SDL_SwapLE16(X) 	SDL_Swap16(X)
//	//#define SDL_SwapLE32(X) 	SDL_Swap32(X)
//	#define SDL_SwapBE16(X) 	(X)
//	#define SDL_SwapBE32(X) 	(X)
//#endif

/* Read an item of the specified endianness and return in native format */
extern u16 /*SD LCALL*/ SDL_ReadLE16(SDL_RWops *src);
extern u16 /*SD LCALL*/ SDL_ReadBE16(SDL_RWops *src);
extern u32 /*SD LCALL*/ SDL_ReadLE32(SDL_RWops *src);
extern u32 /*SD LCALL*/ SDL_ReadBE32(SDL_RWops *src);

/* Write an item of native format to the specified endianness */
extern int /*SD LCALL*/ SDL_WriteLE16(SDL_RWops *dst, u16 value);
extern int /*SD LCALL*/ SDL_WriteBE16(SDL_RWops *dst, u16 value);
extern int /*SD LCALL*/ SDL_WriteLE32(SDL_RWops *dst, u32 value);
extern int /*SD LCALL*/ SDL_WriteBE32(SDL_RWops *dst, u32 value);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
//#inc lude "clo se_code.h"

#endif /* _PSPL_endian_h */
