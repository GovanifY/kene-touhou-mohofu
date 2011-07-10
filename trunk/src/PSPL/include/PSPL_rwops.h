
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_rwops.h,v 1.8 2004/08/20 18:57:01 slouken Exp $";
#endif

#ifdef _SDL_H
error! "このプログラムは SDL.h がインクルードされていると、コンパイルできません。"
#endif

#ifdef _SDL_RWops_h
error! "このプログラムは SDL_RWops.h がインクルードされていると、コンパイルできません。"
#endif

/* This file provides a general interface for SDL to read and write
   data sources.  It can easily be extended to files, memory, etc.
*/

#ifndef _PSPL_RWops_h
#define _PSPL_RWops_h

#include <stdio.h>

#include "PSPL_types.h"

//#inc lude "beg in_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* This is the read/write operation structure -- very basic */

typedef struct SDL_RWops
{
	/* Seek to 'offset' relative to whence, one of stdio's whence values:
		SEEK_SET, SEEK_CUR, SEEK_END
	   Returns the final offset in the data source.
	 */
	int (/*SD LCALL*/ *seek)(struct SDL_RWops *context, int offset, int whence);

	/* Read up to 'num' objects each of size 'objsize' from the data
	   source to the area pointed at by 'ptr'.
	   Returns the number of objects read, or -1 if the read failed.
	 */
	int (/*SD LCALL*/ *read)(struct SDL_RWops *context, void *ptr, int size, int maxnum);

	/* Write exactly 'num' objects each of size 'objsize' from the area
	   pointed at by 'ptr' to data source.
	   Returns 'num', or -1 if the write failed.
	 */
	int (/*SD LCALL*/ *write)(struct SDL_RWops *context, const void *ptr, int size, int num);

	/* Close and free an allocated SDL_FSops structure */
	int (/*SD LCALL*/ *close)(struct SDL_RWops *context);

	u32 type;
	union
	{
		struct
		{
			int autoclose;
			FILE *fp;
		} stdio;
		struct
		{
			u8 *base;
			u8 *here;
			u8 *stop;
		} mem;
		struct
		{
			void *data1;
		} unknown;
	} hidden;
} SDL_RWops;


/* Functions to create SDL_RWops structures from various data sources */

extern SDL_RWops * /*SD LCALL*/ SDL_RWFromFile(const char *file, const char *mode);

extern SDL_RWops * /*SD LCALL*/ SDL_RWFromFP(FILE *fp, int autoclose);

extern SDL_RWops * /*SD LCALL*/ SDL_RWFromMem(void *mem, int size);
extern SDL_RWops * /*SD LCALL*/ SDL_RWFromConstMem(const void *mem, int size);

extern SDL_RWops * /*SD LCALL*/ SDL_AllocRW(void);
extern void /*SD LCALL*/ SDL_FreeRW(SDL_RWops *area);

/* Macros to easily read and write from an SDL_RWops structure */
#define SDL_RWseek(ctx, offset, whence)	(ctx)->seek(ctx, offset, whence)
#define SDL_RWtell(ctx)			(ctx)->seek(ctx, 0, SEEK_CUR)
#define SDL_RWread(ctx, ptr, size, n)	(ctx)->read(ctx, ptr, size, n)
#define SDL_RWwrite(ctx, ptr, size, n)	(ctx)->write(ctx, ptr, size, n)
#define SDL_RWclose(ctx)		(ctx)->close(ctx)


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
//#inc lude "clo se_code.h"

#endif /* _PSPL_RWops_h */
