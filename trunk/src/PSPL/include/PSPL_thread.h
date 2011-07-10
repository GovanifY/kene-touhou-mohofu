
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_thread.h,v 1.7 2004/08/20 18:57:01 slouken Exp $";
#endif

#ifdef _SDL_H
error! "このプログラムは SDL.h がインクルードされていると、コンパイルできません。"
#endif

#ifdef _SDL_thread_h
error! "このプログラムは SDL_thread.h がインクルードされていると、コンパイルできません。"
#endif

#ifndef _PSPL_thread_h
#define _PSPL_thread_h

/* Header for the SDL thread management routines

	These are independent of the other SDL routines.
*/

#include "PSPL_main.h"
#include "PSPL_types.h"

/* Thread synchronization primitives */
#include "PSPL_mutex.h"

//#inc lude "beg in_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* The SDL thread structure, defined in SDL_thread.c */
struct SDL_Thread;
typedef struct SDL_Thread SDL_Thread;

/* Create a thread */
extern SDL_Thread * /*SD LCALL*/ SDL_CreateThread(int (/*SD LCALL*/ *fn)(void *), void *data);

/* Get the 32-bit thread identifier for the current thread */
extern u32 /*SD LCALL*/ SDL_ThreadID(void);

/* Get the 32-bit thread identifier for the specified thread,
   equivalent to SDL_ThreadID() if the specified thread is NULL.
 */
extern u32 /*SD LCALL*/ SDL_GetThreadID(SDL_Thread *thread);

/* Wait for a thread to finish.
   The return code for the thread function is placed in the area
   pointed to by 'status', if 'status' is not NULL.
 */
extern void /*SD LCALL*/ SDL_WaitThread(SDL_Thread *thread, int *status);

/* Forcefully kill a thread without worrying about its state */
extern void /*SD LCALL*/ SDL_KillThread(SDL_Thread *thread);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
//#inc lude "clo se_code.h"

#endif /* _PSPL_thread_h */
