/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fevents%2FSDL_events_c.h

psp - ���r�W���� 440Subversion ���|�W�g���ꗗ: pspps2ps2wareps3ps3warepspware���r�W����:(root)/trunk/SDL/src/events/SDL_events_c.h
�ڍו\�� - �ŏI�X�V���� - ���O������ -
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
 "@(#) $Id: PSPL_events_c.h,v 1.6 2004/01/04 16:49:17 slouken Exp $";
#endif

/* Useful functions and variables from SDL_events.c */
#include "./../include/PSPL_events.h"

/* Start and stop the event processing loop */

extern void SDL_QuitInterrupt(void);

/* Event handler init routines */
extern int  SDL_AppActiveInit(void);
extern int  SDL_QuitInit(void);

/* Internal event queueing functions
   (from SDL_active.c, SDL_mouse.c, SDL_keyboard.c, SDL_quit.c, SDL_events.c)
 */
extern int SDL_PrivateAppActive(u8 gain, u8 state);
//�p�~��	extern int SDL_PrivateResize(int w, int h);
extern int SDL_PrivateExpose(void);
extern int SDL_PrivateQuit(void);


extern int SDL_TranslateUNICODE;
