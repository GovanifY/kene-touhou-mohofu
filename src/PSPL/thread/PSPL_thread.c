/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fthread%2FSDL_thread.c


psp - リビジョン 440Subversion リポジトリ一覧: pspps2ps2wareps3ps3warepspwareリビジョン:(root)/trunk/SDL/src/thread/SDL_thread.c
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
 "@(#) $Id: PSPL_thread.c,v 1.8 2004/01/04 16:49:18 slouken Exp $";
#endif

/* System independent thread management routines for SDL */

#include <psptypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../include/PSPL_error.h"	/* 取り敢えず(仮) */
#include "./../include/PSPL_thread.h"	/* 取り敢えず(仮) */

//#include "PSPL_error.h"
#include "./../include/PSPL_mutex.h"	/* 取り敢えず(仮) */
//#include "PSPL_thread.h"
#include "./PSPL_thread_c.h"
#include "./PSPL_systhread.h"

#ifndef SDL_OutOfMemory_bbb
	#define SDL_OutOfMemory_bbb(aaa)
#endif

#define ARRAY_CHUNKSIZE 32
/* The array of threads currently active in the application
   (except the main thread)
   The manipulation of an array here is safer than using a linked list.
*/
static int SDL_maxthreads = 0;
static int SDL_numthreads = 0;
static SDL_Thread **SDL_Threads = NULL;
static SDL_mutex *thread_lock = NULL;
//int _creating_thread_lock = 0;

/* This should never be called...
   If this is called by SDL_Quit(), we don't know whether or not we should
   clean up threads here.  If any threads are still running after this call,
   they will no longer have access to any per-thread data.
 */
void SDL_ThreadsQuit(void)
{
	SDL_mutex *mutex;
	mutex = thread_lock;
	thread_lock = NULL;
	if ( mutex != NULL )
	{
		SDL_DestroyMutex(mutex);
	}
}

int SDL_ThreadsInit(void)
{
	/* Set the thread lock creation flag so that we can reuse an
	   existing lock on the system - since this mutex never gets
	   destroyed (see SDL_ThreadsQuit()), we want to reuse it.
	*/
//	_creating_thread_lock = 1;
	thread_lock = SDL_CreateMutex();
//	_creating_thread_lock = 0;
	int retval;
	retval = 0;
	if ( thread_lock == NULL )
	{
		retval = -1;
	}
	return (retval);
}

/* Routines for manipulating the thread list */


static void SDL_DelThread(SDL_Thread *thread)
{
	if ( thread_lock )
	{
		SDL_mutexP(thread_lock);
		int i;
		for (i=0; i<SDL_numthreads; i++)
		{
			if ( thread == SDL_Threads[i] )
			{
				break;
			}
		}
		if ( i < SDL_numthreads )
		{
			if ( --SDL_numthreads > 0 )
			{
				while ( i < SDL_numthreads )
				{
					SDL_Threads[i] = SDL_Threads[i+1];
					i++;
				}
			}
			else
			{
				SDL_maxthreads = 0;
				free(SDL_Threads);
				SDL_Threads = NULL;
			}
			#ifdef DEBUG_THREADS
			printf("Deleting thread (%d left - %d max)\n", SDL_numthreads, SDL_maxthreads);
			#endif
		}
		SDL_mutexV(thread_lock);
	}
}
static void SDL_AddThread(SDL_Thread *thread)
{
	SDL_Thread **threads;
	/* WARNING:
	   If the very first threads are created simultaneously, then
	   there could be a race condition causing memory corruption.
	   In practice, this isn't a problem because by definition there
	   is only one thread running the first time this is called.
	*/
	if ( thread_lock == NULL )
	{
		if ( SDL_ThreadsInit() < 0 )
		{
			return;
		}
	}
	SDL_mutexP(thread_lock);

	/* Expand the list of threads, if necessary */
	#ifdef DEBUG_THREADS
	printf("Adding thread (%d already - %d max)\n", SDL_numthreads, SDL_maxthreads);
	#endif
	if ( SDL_numthreads == SDL_maxthreads )
	{
		threads=(SDL_Thread **)malloc((SDL_maxthreads+ARRAY_CHUNKSIZE) * (sizeof *threads));
		if ( threads == NULL )
		{
			SDL_OutOfMemory_bbb();
			goto done;
		}
		memcpy(threads, SDL_Threads, SDL_numthreads*(sizeof *threads));
		SDL_maxthreads += ARRAY_CHUNKSIZE;
		if ( SDL_Threads )
		{
			free(SDL_Threads);
		}
		SDL_Threads = threads;
	}
	SDL_Threads[SDL_numthreads++] = thread;
done:
	SDL_mutexV(thread_lock);
}

#if (1)
void SDL_WaitThread(SDL_Thread *thread, int *status)
{
	if ( thread )
	{
		SDL_SYS_WaitThread(thread);
		if ( status )
		{
			*status = thread->status;
		}
		SDL_DelThread(thread);
		free(thread);
	}
}
#endif /*(1)*/


/* Arguments and callback to setup and run the user thread function */
typedef struct
{
	int (*func)(void *);
	void *data;
	SDL_Thread *info;
	SDL_sem *wait;
} thread_args;
#if (1)
void SDL_RunThread(void *data)
{
	/* Perform any system-dependent setup
	   - this function cannot fail, and cannot use SDL_SetError_bbb()
	 */
	SDL_SYS_SetupThread();
	{
		int (*userfunc)(void *);
		void *userdata;
		int *statusloc;
		thread_args *args;
		/* Get the thread id */
		{
			args = (thread_args *)data;
			args->info->threadid = SDL_ThreadID();

			/* Figure out what function to run */
			userfunc = args->func;
			userdata = args->data;
			statusloc = &args->info->status;

			/* Wake up the parent thread */
			SDL_SemPost(args->wait);
		}
		/* Run the function */
		*statusloc = userfunc(userdata);
	}
}
#endif /*(1)*/

#if (1)
SDL_Thread *SDL_CreateThread(int (*fn)(void *), void *data)
{
	/* Allocate memory for the thread info structure */
	SDL_Thread *thread;
	thread = (SDL_Thread *)malloc(sizeof(*thread));
	if ( thread == NULL )
	{
		SDL_OutOfMemory_bbb();
		return (NULL);
	}
	memset(thread, 0, (sizeof *thread));
	thread->status = -1;
	/* Set up the arguments for the thread */
	{
		thread_args *args;
		args = (thread_args *)malloc(sizeof(*args));
		if ( args == NULL )
		{
			SDL_OutOfMemory_bbb();
			free(thread);
			return (NULL);
		}
		args->func = fn;
		args->data = data;
		args->info = thread;
		args->wait = SDL_CreateSemaphore(0);
		if ( args->wait == NULL )
		{
			free(thread);
			free(args);
			return (NULL);
		}
		/* Add the thread to the list of available threads */
		SDL_AddThread(thread);
		/* Create the thread and go! */
		{
			int ret;
			ret = SDL_SYS_CreateThread(thread, args);
			if ( ret >= 0 )
			{
				/* Wait for the thread function to use arguments */
				SDL_SemWait(args->wait);
			}
			else
			{
				/* Oops, failed.  Gotta free everything */
				SDL_DelThread(thread);
				free(thread);
				thread = NULL;
			}
		}
		SDL_DestroySemaphore(args->wait);
		free(args);
	}
	/* Everything is running now */
	return (thread);
}
#endif /*(1)*/


#if (00)
/* The default (non-thread-safe) global error variable */
static SDL_error SDL_global_error;
/* Routine to get the thread-specific error variable */
SDL_error *SDL_GetErrBuf(void)
{
	SDL_error *errbuf;
	errbuf = &SDL_global_error;
	if ( SDL_Threads )
	{
		u32 this_thread;
		this_thread = SDL_ThreadID();
		SDL_mutexP(thread_lock);
		{
			int i;
			for (i=0; i<SDL_numthreads; i++)
			{
				if ( this_thread == SDL_Threads[i]->threadid )
				{
					errbuf = &SDL_Threads[i]->errbuf;
					break;
				}
			}
		}
		SDL_mutexV(thread_lock);
	}
	return (errbuf);
}
#endif /*(00)*/

#if (00)
u32 SDL_GetThreadID(SDL_Thread *thread)
{
	u32 id;
	if ( thread )
	{
		id = thread->threadid;
	}
	else
	{
		id = SDL_ThreadID();
	}
	return (id);
}
#endif /*(00)*/

#if (00)
void SDL_KillThread(SDL_Thread *thread)
{
	if ( thread )
	{
		SDL_SYS_KillThread(thread);
		SDL_WaitThread(thread, NULL);
	}
}
#endif /*(00)*/
