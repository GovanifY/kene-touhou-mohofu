
/*

http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fthread%2FSDL_thread_c.h

psp - ���r�W���� 440Subversion ���|�W�g���ꗗ: pspps2ps2wareps3ps3warepspware���r�W����:(root)/trunk/SDL/src/thread/SDL_thread_c.h
�ڍו\�� - �ŏI�X�V���� - ���O������ -

http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fthread%2Fpsp%2FSDL_systhread_c.h
http://svn.pspdev.org/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fthread%2Fpsp%2FSDL_systhread_c.h

psp - ���r�W���� 440Subversion ���|�W�g���ꗗ: pspps2ps2wareps3ps3warepspware���r�W����:(root)/trunk/SDL/src/thread/SDL_systhread.h
�ڍו\�� - �ŏI�X�V���� - ���O������ -
*/

/*
Subversion ���|�W�g���ꗗ psp
(root)/trunk/SDL/src/thread/psp/SDL_systhread_c.h - ���r�W���� 472
*/

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_thread_c.h,v 1.5 2004/01/04 16:49:18 slouken Exp $";
#endif

#ifndef _PSPL_thread_c_h
#define _PSPL_thread_c_h

#include "./PSPL_error_c.h"
//#include "./PSPL_systhread_c.h"

/* PSP port contributed by Marcus R. Brown <mrbrown@ocgnet.org>. */

#include <pspkerneltypes.h>

typedef SceUID SYS_ThreadHandle;

/* This is the system-independent thread info structure */
struct SDL_Thread
{
	u32 threadid;
	SYS_ThreadHandle handle;
	int status;
	SDL_error errbuf;
	void *data;
};

/* This is the function called to run a thread */
extern void SDL_RunThread(void *data);

/* Routine to get the thread-specific error variable */
extern SDL_error *SDL_GetErrBuf(void);

#endif /* _PSPL_thread_c_h */
