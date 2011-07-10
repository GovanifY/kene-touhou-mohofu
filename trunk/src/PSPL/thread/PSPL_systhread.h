/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fthread%2FSDL_systhread.h

psp - リビジョン 440Subversion リポジトリ一覧: pspps2ps2wareps3ps3warepspwareリビジョン:(root)/trunk/SDL/src/thread/SDL_systhread.h
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
 "@(#) $Id: PSPL_systhread.h,v 1.5 2004/01/04 16:49:18 slouken Exp $";
#endif

/* These are functions that need to be implemented by a port of SDL */

#include "./PSPL_thread_c.h"

/* This function creates a thread, passing args to SDL_RunThread(),
   saves a system-dependent thread id in thread->id, and returns 0
   on success.
*/
extern int SDL_SYS_CreateThread(SDL_Thread *thread, void *args);

/* This function does any necessary setup in the child thread */
extern void SDL_SYS_SetupThread(void);

/* This function waits for the thread to finish and frees any data
   allocated by SDL_SYS_CreateThread()
 */
extern void SDL_SYS_WaitThread(SDL_Thread *thread);

/* This function kills the thread and returns */
extern void SDL_SYS_KillThread(SDL_Thread *thread);

