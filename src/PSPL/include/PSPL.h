
/*---------------------------------------------------------
	ìåï˚ñÕïÌïó Å` Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL.h,v 1.8 2004/01/04 16:49:07 slouken Exp $";
#endif

#ifdef _SDL_H
error! "Ç±ÇÃÉvÉçÉOÉâÉÄÇÕ SDL.h Ç™ÉCÉìÉNÉãÅ[ÉhÇ≥ÇÍÇƒÇ¢ÇÈÇ∆ÅAÉRÉìÉpÉCÉãÇ≈Ç´Ç‹ÇπÇÒÅB"
#endif

//#define SDLCALL 	/**/
//#define DECLSPEC	/**/

/* Main include header for the SDL library */

#ifndef _PSPL_H
#define _PSPL_H

#include "./PSPL_main.h"
#include "./PSPL_types.h"
//#include "./PSPL_getenv.h"
#include "./PSPL_error.h"
#include "./PSPL_rwops.h"
#include "./PSPL_timer.h"
#include "./PSPL_audio.h"
//#include "./PSPL_cdrom.h"
//#include "./PSPL_joystick.h"
#include "./PSPL_events.h"
#include "./PSPL_video.h"
#include "./PSPL_byteorder.h"
//#include "./PSPL_version.h"

//#inc lude "beg in_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* As of version 0.5, SDL is loaded dynamically into the application */

/* These are the flags which may be passed to SDL_Init() -- you should
   specify the subsystems which you will be using in your application.
*/
// îpé~#define	SDL_IN IT_TIMER		0x00000001
#define SDL_INIT_AUDIO			0x00000010
#define SDL_INIT_VIDEO			0x00000020
// îpé~#define SDL_IN IT_CDR OM		0x00000100
// îpé~#define SDL_IN IT_JOY STICK	0x00000200
#define SDL_INIT_NOPARACHUTE	0x00100000	/* Don't catch fatal signals */
#define SDL_INIT_EVENTTHREAD	0x01000000	/* Not supported on all OS's */
#define SDL_INIT_EVERYTHING 	0x0000FFFF

/* This function loads the SDL dynamically linked library and initializes
 * the subsystems specified by 'flags' (and those satisfying dependencies)
 * Unless the SDL_INIT_NOPARACHUTE flag is set, it will install cleanup
 * signal handlers for some commonly ignored fatal signals (like SIGSEGV)
 */
extern int /*SD LCALL*/ SDL_Init(u32 flags);

/* This function initializes specific SDL subsystems */
//îpé~extern int /*SD LCALL*/ SDL_InitSubSystem(u32 flags);

/* This function cleans up specific SDL subsystems */
//îpé~extern void /*SD LCALL*/ SDL_QuitSubSystem(u32 flags);

/* This function returns mask of the specified subsystems which have
   been initialized.
   If 'flags' is 0, it returns a mask of all initialized subsystems.
*/
//îpé~extern u32 /*SD LCALL*/ SDL_WasInit(u32 flags);

/* This function cleans up all initialized subsystems and unloads the
 * dynamically linked library.  You should call it upon all exit conditions.
 */
extern void /*SD LCALL*/ SDL_Quit(void);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
//#inc lude "clo se_code.h"

#endif /* _PSPL_H */
