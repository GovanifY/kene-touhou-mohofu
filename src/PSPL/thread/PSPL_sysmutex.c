/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fthread%2Fgeneric%2FSDL_sysmutex.c


psp - リビジョン 440Subversion リポジトリ一覧: pspps2ps2wareps3ps3warepspwareリビジョン:(root)/trunk/SDL/src/thread/generic/SDL_sysmutex.c
詳細表示 - 最終更新日時 - ログを見る -
*/

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_sysmutex.c,v 1.5 2004/01/04 16:49:19 slouken Exp $";
#endif

/* An implementation of mutexes using semaphores */

#include <stdio.h>
#include <stdlib.h>

#include "./../include/PSPL_error.h"	/* 取り敢えず(仮) */
#include "./../include/PSPL_thread.h"	/* 取り敢えず(仮) */
//#include "./PSPL_systhread_c.h"

#ifndef SDL_OutOfMemory_bbb
	#define SDL_OutOfMemory_bbb(aaa)
#endif

#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb( ... )
#endif


struct SDL_mutex
{
	int recursive;
	u32 owner;
	SDL_sem *sem;
};

/* Create a mutex */
SDL_mutex *SDL_CreateMutex(void)
{
	SDL_mutex *mutex;
	/* Allocate mutex memory */
	mutex = (SDL_mutex *)malloc(sizeof(*mutex));
	if ( mutex )
	{
		/* Create the mutex semaphore, with initial value 1 */
		mutex->sem = SDL_CreateSemaphore(1);
		mutex->recursive = 0;
		mutex->owner = 0;
		if ( ! mutex->sem )
		{
			free(mutex);
			mutex = NULL;
		}
	}
	else
	{
		SDL_OutOfMemory_bbb();
	}
	return mutex;
}

/* Free the mutex */
void SDL_DestroyMutex(SDL_mutex *mutex)
{
	if ( mutex )
	{
		if ( mutex->sem )
		{
			SDL_DestroySemaphore(mutex->sem);
		}
		free(mutex);
	}
}

/* Lock the semaphore */
int SDL_mutexP(SDL_mutex *mutex)
{
#ifdef DISABLE_THREADS
	return (0);
#else
	u32 this_thread;

	if ( mutex == NULL )
	{
		SDL_SetError_bbb("Passed a NULL mutex");
		return (-1);
	}

	this_thread = SDL_ThreadID();
	if ( mutex->owner == this_thread )
	{
		++mutex->recursive;
	}
	else
	{
		/* The order of operations is important.
		   We set the locking thread id after we obtain the lock
		   so unlocks from other threads will fail.
		*/
		SDL_SemWait(mutex->sem);
		mutex->owner = this_thread;
		mutex->recursive = 0;
	}
	return (0);
#endif /* DISABLE_THREADS */
}

/* Unlock the mutex */
int SDL_mutexV(SDL_mutex *mutex)
{
#ifdef DISABLE_THREADS
	return (0);
#else
	if ( mutex == NULL )
	{
		SDL_SetError_bbb("Passed a NULL mutex");
		return (-1);
	}

	/* If we don't own the mutex, we can't unlock it */
	if ( SDL_ThreadID() != mutex->owner )
	{
		SDL_SetError_bbb("mutex not owned by this thread");
		return (-1);
	}

	if ( mutex->recursive )
	{
		--mutex->recursive;
	}
	else
	{
		/* The order of operations is important.
		   First reset the owner so another thread doesn't lock
		   the mutex and set the ownership before we reset it,
		   then release the lock semaphore.
		 */
		mutex->owner = 0;
		SDL_SemPost(mutex->sem);
	}
	return (0);
#endif /* DISABLE_THREADS */
}
