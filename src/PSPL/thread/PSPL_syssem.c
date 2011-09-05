/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fthread%2Fpsp%2FSDL_syssem.c

psp - リビジョン 1119Subversion リポジトリ一覧: pspps2ps2wareps3ps3warepspwareリビジョン:(root)/trunk/SDL/src/thread/psp/SDL_syssem.c
リビジョン 567 - 詳細表示 - 前のリビジョンとの差分 - 最終更新日時 - ログを見る -
*/

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

/* PSP port contributed by Marcus R. Brown <mrbrown@ocgnet.org>. */

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_syssem.c,v 1.6 2004/01/04 16:49:19 slouken Exp $";
#endif

/* Semaphore functions for the PSP. */

#include <pspkerneltypes.h>
#include <pspthreadman.h>
#include <pspkerror.h>

#include <stdio.h>
#include <stdlib.h>

#include "./../include/PSPL_error.h"	/* 取り敢えず(仮) */
#include "./../include/PSPL_thread.h"	/* 取り敢えず(仮) */

#ifndef SDL_OutOfMemory_bbb
	#define SDL_OutOfMemory_bbb(aaa)
#endif

#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb( ... )
#endif

struct SDL_semaphore
{
	SceUID  semid;
};


/* TODO: This routine is a bit overloaded.
 * If the timeout is 0 then just poll the semaphore; if it's SDL_MUTEX_MAXWAIT, pass
 * NULL to sceKernelWaitSema() so that it waits indefinitely; and if the timeout
 * is specified, convert it to microseconds. */
static int s_PSPL_SemWaitTimeout(SDL_sem *sem/*, u32 timeout*/)
{
	if (sem == NULL)
	{
		SDL_SetError_bbb("Passed a NULL sem");
		return (0);
	}
//
	u32 *pTimeout;
//	if (timeout == 0)
//	{	unsigned int res_aaa;
//		res_aaa = sceKernelPollSema(sem->semid, 1);
//		if (res_aaa < 0)
//		{
//			return SDL_MUTEX_TIMEDOUT;
//		}
//		return (0);
//	}
//	else
//	if (timeout == SDL_MUTEX_MAXWAIT)
	{
		pTimeout = NULL;
	}
//	else
//	{
//		timeout *= 1000;	/* Convert to microseconds. */
//		pTimeout = &timeout;
//	}
//
	unsigned int res_bbb;
	res_bbb = sceKernelWaitSema(sem->semid, 1, pTimeout);	/* セマフォロック */
	switch (res_bbb)
	{
	case SCE_KERNEL_ERROR_OK:
		return (0);
	case SCE_KERNEL_ERROR_WAIT_TIMEOUT:
		return SDL_MUTEX_TIMEDOUT;
	default:
		SDL_SetError_bbb("WaitForSingleObject() failed");
		return (-1);
	}
}

#if (00)
//int SDL_SemTryWait(SDL_sem *sem)
//{
//	return s_PSPL_SemWaitTimeout(sem/*, 0*/);
//}
#endif /*(00)*/

int SDL_SemWait(SDL_sem *sem)
{
	return s_PSPL_SemWaitTimeout(sem/*, SDL_MUTEX_MAXWAIT*/);
}

#if (00)
/* Returns the current count of the semaphore */
u32 SDL_SemValue(SDL_sem *sem)
{
	if (sem == NULL)
	{
		SDL_SetError_bbb("Passed a NULL sem");
		return (0);
	}
//
	SceKernelSemaInfo info;
	if (sceKernelReferSemaStatus(sem->semid, &info) >= 0)
	{
		return info.currentCount;
	}
	return (0);
}
#endif /*(00)*/

int SDL_SemPost(SDL_sem *sem)
{
	if (sem == NULL)
	{
		SDL_SetError_bbb("Passed a NULL sem");
		return (-1);
	}
//
	int res;
	res = sceKernelSignalSema(sem->semid, 1);
	if (res < 0)
	{
		SDL_SetError_bbb("sceKernelSignalSema() failed");	/* セマフォアンロック */
		return (-1);
	}
	return (0);
}


/* Free the semaphore */
void SDL_DestroySemaphore(SDL_sem *sem)
{
	if (sem != NULL)
	{
		if (sem->semid > 0)
		{
			sceKernelDeleteSema(sem->semid);	/* セマフォ削除 */
			sem->semid = 0;
		}
		free(sem);
	}
}

/* Create a semaphore */
SDL_sem *SDL_CreateSemaphore(u32 initial_value)
{
	SDL_sem *sem;
	sem = (SDL_sem *) malloc(sizeof(*sem));
	if (sem != NULL)
	{
		/* TODO: Figure out the limit on the maximum value. */
		sem->semid = sceKernelCreateSema("SDL sema", 0, initial_value, 255, NULL);	/* セマフォ生成 */
		if (sem->semid < 0)
		{
			SDL_SetError_bbb("Couldn't create semaphore");
			free(sem);
			sem = NULL;
		}
	}
	else
	{
		SDL_OutOfMemory_bbb();
	}
	return (sem);
}

/* vim: ts=4 sw=4
 */

/*
http://www23.atwiki.jp/amorphophallus/pages/38.html
排他制御（mutex）

セマフォは同期されたカウンタである。PSPにはmutexタイプはないが、セマフォはそれをシミュレートできる。

sceKernelCreateSema と sceKernelDeleteSema でそれぞれ 生成 と 削除 ができる
sceKernelWaitSema と sceKernelSignalSema でそれぞれ ロック と アンロック ができる

*/

