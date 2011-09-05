/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fthread%2Fpsp%2FSDL_systhread.c
psp - ���r�W���� 2315Subversion ���|�W�g���ꗗ: pspps2ps2wareps3ps3warepspware���r�W����:(root)/trunk/SDL/src/thread/psp/SDL_systhread.c
���r�W���� 1221 - �ڍו\�� - �O�̃��r�W�����Ƃ̍��� - �ŏI�X�V���� - ���O������ -
*/

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

/* PSP port contributed by Marcus R. Brown <mrbrown@ocgnet.org>. */

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_systhread.c,v 1.5 2004/01/04 16:49:19 slouken Exp $";
#endif

/* PSP thread management routines for SDL */

#include <pspkerneltypes.h>
#include <pspthreadman.h>

#include <stdio.h>
#include <stdlib.h>

#include "./../include/PSPL_error.h"	/* ��芸����(��) */
#include "./../include/PSPL_thread.h"	/* ��芸����(��) */
#include "./PSPL_systhread.h"

#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb( ... )
#endif

static int ThreadEntry(SceSize args, void *argp)
{
	SDL_RunThread(*(void **) argp);
	return (0);
}

int SDL_SYS_CreateThread(SDL_Thread *thread, void *args)
{
	SceKernelThreadInfo status;
	int priority = 32;

	/* Set priority of new thread to the same as the current thread */
	status.size = sizeof(SceKernelThreadInfo);
	if (sceKernelReferThreadStatus(sceKernelGetThreadId(), &status) == 0)
	{
		priority = status.currentPriority;
	}

	thread->handle = sceKernelCreateThread("SDL thread", ThreadEntry,
					priority, 0x8000,
					PSP_THREAD_ATTR_VFPU, NULL);
	if (thread->handle < 0)
	{
		SDL_SetError_bbb("sceKernelCreateThread() failed");
		return (-1);
	}

	sceKernelStartThread(thread->handle, 4, &args);
	return (0);
}

void SDL_SYS_SetupThread(void)
{
	/* Do nothing. */
}

u32 SDL_ThreadID(void)
{
	return (u32) sceKernelGetThreadId();
}

void SDL_SYS_WaitThread(SDL_Thread *thread)
{
	sceKernelWaitThreadEnd(thread->handle, NULL);
	sceKernelDeleteThread(thread->handle);
}

void SDL_SYS_KillThread(SDL_Thread *thread)
{
	sceKernelTerminateDeleteThread(thread->handle);
}

/* vim: ts=4 sw=4
 */
