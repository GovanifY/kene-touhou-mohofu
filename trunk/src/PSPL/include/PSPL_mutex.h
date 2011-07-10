
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_mutex.h,v 1.7 2004/01/04 16:49:07 slouken Exp $";
#endif

#ifdef _SDL_H
error! "このプログラムは SDL.h がインクルードされていると、コンパイルできません。"
#endif

#ifdef _SDL_mutex_h
error! "このプログラムは SDL_mutex.h がインクルードされていると、コンパイルできません。"
#endif

#ifndef _PSPL_mutex_h
#define _PSPL_mutex_h

/* Functions to provide thread synchronization primitives

	These are independent of the other SDL routines.
*/

#include "PSPL_main.h"
#include "PSPL_types.h"

//#inc lude "beg in_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Synchronization functions which can time out return this value
   if they time out.
*/
#define SDL_MUTEX_TIMEDOUT	1

/* This is the timeout value which corresponds to never time out */
#define SDL_MUTEX_MAXWAIT	(~(u32)0)


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Mutex functions					       */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* The SDL mutex structure, defined in SDL_mutex.c */
struct SDL_mutex;
typedef struct SDL_mutex SDL_mutex;

/* Create a mutex, initialized unlocked */
extern SDL_mutex * /*SD LCALL*/ SDL_CreateMutex(void);

/* Lock the mutex  (Returns 0, or -1 on error) */
#define SDL_LockMutex(m)	SDL_mutexP(m)
extern int /*SD LCALL*/ SDL_mutexP(SDL_mutex *mutex);

/* Unlock the mutex  (Returns 0, or -1 on error)
   It is an error to unlock a mutex that has not been locked by
   the current thread, and doing so results in undefined behavior.
 */
#define SDL_UnlockMutex(m)	SDL_mutexV(m)
extern int /*SD LCALL*/ SDL_mutexV(SDL_mutex *mutex);

/* Destroy a mutex */
extern void /*SD LCALL*/ SDL_DestroyMutex(SDL_mutex *mutex);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Semaphore functions					   */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* The SDL semaphore structure, defined in SDL_sem.c */
struct SDL_semaphore;
typedef struct SDL_semaphore SDL_sem;

/* Create a semaphore, initialized with value, returns NULL on failure. */
extern SDL_sem * /*SD LCALL*/ SDL_CreateSemaphore(u32 initial_value);

/* Destroy a semaphore */
extern void /*SD LCALL*/ SDL_DestroySemaphore(SDL_sem *sem);

/* This function suspends the calling thread until the semaphore pointed
 * to by sem has a positive count. It then atomically decreases the semaphore
 * count.
 */
extern int /*SD LCALL*/ SDL_SemWait(SDL_sem *sem);

/* Non-blocking variant of SDL_SemWait(), returns 0 if the wait succeeds,
   SDL_MUTEX_TIMEDOUT if the wait would block, and -1 on error.
*/
extern int /*SD LCALL*/ SDL_SemTryWait(SDL_sem *sem);

/* Variant of SDL_SemWait() with a timeout in milliseconds, returns 0 if
   the wait succeeds, SDL_MUTEX_TIMEDOUT if the wait does not succeed in
   the allotted time, and -1 on error.
   On some platforms this function is implemented by looping with a delay
   of 1 ms, and so should be avoided if possible.
*/
extern int /*SD LCALL*/ SDL_SemWaitTimeout(SDL_sem *sem, u32 ms);

/* Atomically increases the semaphore's count (not blocking), returns 0,
   or -1 on error.
 */
extern int /*SD LCALL*/ SDL_SemPost(SDL_sem *sem);

/* Returns the current count of the semaphore */
extern u32 /*SD LCALL*/ SDL_SemValue(SDL_sem *sem);


/* ------------------------------------------ */
/* Condition variable functions				  */
/* ------------------------------------------ */

/* The SDL condition variable structure, defined in SDL_cond.c */
struct SDL_cond;
typedef struct SDL_cond SDL_cond;

/* Create a condition variable */
extern SDL_cond * /*SD LCALL*/ SDL_CreateCond(void);

/* Destroy a condition variable */
extern void /*SD LCALL*/ SDL_DestroyCond(SDL_cond *cond);

/* Restart one of the threads that are waiting on the condition variable,
   returns 0 or -1 on error.
 */
extern int /*SD LCALL*/ SDL_CondSignal(SDL_cond *cond);

/* Restart all threads that are waiting on the condition variable,
   returns 0 or -1 on error.
 */
extern int /*SD LCALL*/ SDL_CondBroadcast(SDL_cond *cond);

/* Wait on the condition variable, unlocking the provided mutex.
   The mutex must be locked before entering this function!
   Returns 0 when it is signaled, or -1 on error.
 */
extern int /*SD LCALL*/ SDL_CondWait(SDL_cond *cond, SDL_mutex *mut);

/* Waits for at most 'ms' milliseconds, and returns 0 if the condition
   variable is signaled, SDL_MUTEX_TIMEDOUT if the condition is not
   signaled in the allotted time, and -1 on error.
   On some platforms this function is implemented by looping with a delay
   of 1 ms, and so should be avoided if possible.
*/
extern int /*SD LCALL*/ SDL_CondWaitTimeout(SDL_cond *cond, SDL_mutex *mutex, u32 ms);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
//#inc lude "clo se_code.h"

#endif /* _PSPL_mutex_h */
