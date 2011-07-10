
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_types.h,v 1.13 2004/07/21 04:53:26 slouken Exp $";
#endif

#ifdef _SDL_H
error! "このプログラムは SDL.h がインクルードされていると、コンパイルできません。"
#endif

#ifdef _SDL_types_h
error! "このプログラムは SDL_types.h がインクルードされていると、コンパイルできません。"
#endif

/* General data types used by the SDL library */

#ifndef _PSPL_types_h
#define _PSPL_types_h

/* The number of elements in a table */
#define SDL_TABLESIZE(table)	(sizeof(table)/sizeof(table[0]))

/* Basic data types */
typedef enum
{
	SDL_FALSE = 0,
	SDL_TRUE  = 1
} SDL_bool;

//#ifdef H_MMBASIC /* mmbasic.h (Tru64 MME) */
//	/* Some of the basic types are already defined in mmbasic.h */
//typedef signed char	s8;
//typedef signed short	s16;
//typedef signed int	s32;
//#else
#ifndef u8
	#define 	u8		unsigned char
#endif
#ifndef s8
	#define 	s8		signed char
#endif
#ifndef u16
	#define 	u16 	unsigned short
#endif
#ifndef s16
	#define 	s16 	signed short
#endif
#ifndef u32
	#define 	u32 	unsigned int
#endif
#ifndef s32
	#define 	s32 	signed int
#endif

//#endif

/* Figure out how to support 64-bit datatypes */
#if !defined(__STRICT_ANSI__)
#ifdef __osf__ /* Tru64 */
#define SDL_HAS_64BIT_TYPE	long
#elif defined(__GNUC__) || defined(__MWERKS__) || defined(__SUNPRO_C) || defined(__DECC)
#define SDL_HAS_64BIT_TYPE	long long
#elif defined(_MSC_VER) /* VC++ */
#define SDL_HAS_64BIT_TYPE	__int64
#endif
#endif /* !__STRICT_ANSI__ */

/* The 64-bit type isn't available on EPOC/Symbian OS */
#ifdef __SYMBIAN32__
#undef SDL_HAS_64BIT_TYPE
#endif

/* The 64-bit datatype isn't supported on all platforms */
#ifdef SDL_HAS_64BIT_TYPE
#ifndef H_MMBASIC
typedef unsigned SDL_HAS_64BIT_TYPE Uint64;
#endif
typedef SDL_HAS_64BIT_TYPE Sint64;
#else
/* This is really just a hack to prevent the compiler from complaining */
typedef struct
{
	u32 hi;
	u32 lo;
} Uint64, Sint64;
#endif

/* Make sure the types really have the right sizes */
#define SDL_COMPILE_TIME_ASSERT(name, x)		   \
	   typedef int SDL_dummy_ ## name[(x) * 2 - 1]

SDL_COMPILE_TIME_ASSERT(uint8, sizeof(u8) == 1);
SDL_COMPILE_TIME_ASSERT(sint8, sizeof(s8) == 1);
SDL_COMPILE_TIME_ASSERT(uint16, sizeof(u16) == 2);
SDL_COMPILE_TIME_ASSERT(sint16, sizeof(s16) == 2);
SDL_COMPILE_TIME_ASSERT(uint32, sizeof(u32) == 4);
SDL_COMPILE_TIME_ASSERT(sint32, sizeof(s32) == 4);
SDL_COMPILE_TIME_ASSERT(uint64, sizeof(Uint64) == 8);
SDL_COMPILE_TIME_ASSERT(sint64, sizeof(Sint64) == 8);

/* Check to make sure enums are the size of ints, for structure packing.
   For both Watcom C/C++ and Borland C/C++ the compiler option that makes
   enums having the size of an int must be enabled.
   This is "-b" for Borland C/C++ and "-ei" for Watcom C/C++ (v11).
*/
/* Enable enums always int in CodeWarrior (for MPW use "-enum int") */
#ifdef __MWERKS__
#pragma enumsalwaysint on
#endif

typedef enum
{
	DUMMY_ENUM_VALUE
} SDL_DUMMY_ENUM;

SDL_COMPILE_TIME_ASSERT(enum, sizeof(SDL_DUMMY_ENUM) == sizeof(int));

#undef SDL_COMPILE_TIME_ASSERT

/* General keyboard/mouse state definitions */
//enum { SDL_PRESSED = 0x01, SDL_RELEASED = 0x00 };

#endif
