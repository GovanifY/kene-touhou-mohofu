
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_main.h,v 1.12 2004/01/04 16:49:07 slouken Exp $";
#endif

#ifdef _SDL_H
error! "このプログラムは SDL.h がインクルードされていると、コンパイルできません。"
#endif

#ifdef _SDL_main_h
error! "このプログラムは SDL_main.h がインクルードされていると、コンパイルできません。"
#endif

#ifndef _PSPL_main_h
#define _PSPL_main_h

/* Redefine main() on Win32 and MacOS so that it is called by winmain.c */

#if defined(WIN32) || defined(_WIN32) || \
    (defined(__MWERKS__) && !defined(__BEOS__)) || \
    defined(macintosh) || defined(__APPLE__) || \
    defined(__SYMBIAN32__) || defined(QWS)

#ifdef __cplusplus
#define C_LINKAGE	"C"
#else
#define C_LINKAGE
#endif /* __cplusplus */

/* The application's main() function must be called with C linkage,
   and should be declared like this:
#ifdef __cplusplus
extern "C"
#endif
	int main(int argc, char *argv[])
	{
	}
 */
//#define main	SDL_main

/* The prototype for the application's main() function */
//extern C_LINKAGE int SDL_main(int argc, char *argv[]);


/* From the SDL library code -- needed for registering the app on Win32 */
#if defined(WIN32)
#include "PSPL_types.h"
//#inc lude "beg in_code.h"

#ifdef __cplusplus
extern "C" {
#endif

/* This should be called from your WinMain() function, if any */
extern void /*SD LCALL*/ SDL_SetModuleHandle(void *hInst);
/* This can also be called, but is no longer necessary */
extern int /*SD LCALL*/ SDL_RegisterApp(char *name, u32 style, void *hInst);

#ifdef __cplusplus
}
#endif
//#inc lude "clo se_code.h"
#endif

/* From the SDL library code -- needed for registering QuickDraw on MacOS */
#if defined(macintosh)
//#inc lude "beg in_code.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declaration so we don't need to include QuickDraw.h */
struct QDGlobals;

/* This should be called from your main() function, if any */
extern void /*SD LCALL*/ SDL_InitQuickDraw(struct QDGlobals *the_qd);

#ifdef __cplusplus
}
#endif
//#inc lude "clo se_code.h"
#endif

#endif /* Need to redefine main()? */

#endif /* _PSPL_main_h */
