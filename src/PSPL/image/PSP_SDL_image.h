/*
    SDL_image:  An example image loading library for use with SDL
    Copyright (C) 1999-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* $Id: PSPL_image.h,v 1.17 2004/01/04 17:33:01 slouken Exp $ */

/* A simple library to load images of various formats as SDL surfaces */

#ifndef _SDL_IMAGE_H
#define _SDL_IMAGE_H

#include "./../include/PSPL.h"//#include <SDL/SDL.h>//#include "SD L.h"
//#include "SD L_version.h"
////#inc lude "beg in_code.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Printable format: "%d.%d.%d", MAJOR, MINOR, PATCHLEVEL
*/
//#define SDL_IMAGE_MAJOR_VERSION	1
//#define SDL_IMAGE_MINOR_VERSION	2
//#define SDL_IMAGE_PATCHLEVEL	4

/* This macro can be used to fill a version structure with the compile-time
 * version of the SDL_image library.
 */
//#define SDL_IMAGE_VERSION(X)						//
//{													//
//	(X)->major = SDL_IMAGE_MAJOR_VERSION;			//
//	(X)->minor = SDL_IMAGE_MINOR_VERSION;			//
//	(X)->patch = SDL_IMAGE_PATCHLEVEL;				//
//}

/* This function gets the version of the dynamically linked SDL_image library.
   it should NOT be used to fill a version structure, instead you should
   use the SDL_IMAGE_VERSION() macro.
 */
//extern const SDL_version * /*SD LCALL*/ IMG_Linked_Version(void);

/* Load an image from an SDL data source.
   The 'type' may be one of: "BMP", "GIF", "PNG", etc.

   If the image format supports a transparent pixel, SDL will set the
   colorkey for the surface.  You can enable RLE acceleration on the
   surface afterwards by calling:
	SDL_SetColorKey(image, SDL_RLEACCEL, image->format->colorkey);
 */
extern SDL_Surface * /*SD LCALL*/ IMG_LoadTyped_RW(SDL_RWops *src, int freesrc, char *type);
/* Convenience functions */
extern SDL_Surface * /*SD LCALL*/ IMG_Load(const char *file);
//extern SDL_Surface * /*SD LCALL*/ IMG_Load_RW(SDL_RWops *src, int freesrc);

/* Invert the alpha of a surface for use with OpenGL
   This function is now a no-op, and only provided for backwards compatibility.
*/
//extern int /*SD LCALL*/ IMG_InvertAlpha(int on);

/* Functions to detect a file type, given a seekable source */
extern int /*SD LCALL*/ IMG_isBMP(SDL_RWops *src);
extern int /*SD LCALL*/ IMG_isPNM(SDL_RWops *src);
extern int /*SD LCALL*/ IMG_isXPM(SDL_RWops *src);
extern int /*SD LCALL*/ IMG_isXCF(SDL_RWops *src);
extern int /*SD LCALL*/ IMG_isPCX(SDL_RWops *src);
extern int /*SD LCALL*/ IMG_isGIF(SDL_RWops *src);
extern int /*SD LCALL*/ IMG_isJPG(SDL_RWops *src);
extern int /*SD LCALL*/ IMG_isTIF(SDL_RWops *src);
extern int /*SD LCALL*/ IMG_isPNG(SDL_RWops *src);
extern int /*SD LCALL*/ IMG_isLBM(SDL_RWops *src);

/* Individual loading functions */
extern SDL_Surface * /*SD LCALL*/ IMG_LoadBMP_RW(SDL_RWops *src);
extern SDL_Surface * /*SD LCALL*/ IMG_LoadPNM_RW(SDL_RWops *src);
extern SDL_Surface * /*SD LCALL*/ IMG_LoadXPM_RW(SDL_RWops *src);
extern SDL_Surface * /*SD LCALL*/ IMG_LoadXCF_RW(SDL_RWops *src);
extern SDL_Surface * /*SD LCALL*/ IMG_LoadPCX_RW(SDL_RWops *src);
extern SDL_Surface * /*SD LCALL*/ IMG_LoadGIF_RW(SDL_RWops *src);
extern SDL_Surface * /*SD LCALL*/ IMG_LoadJPG_RW(SDL_RWops *src);
extern SDL_Surface * /*SD LCALL*/ IMG_LoadTIF_RW(SDL_RWops *src);
extern SDL_Surface * /*SD LCALL*/ IMG_LoadPNG_RW(SDL_RWops *src);
extern SDL_Surface * /*SD LCALL*/ IMG_LoadTGA_RW(SDL_RWops *src);
extern SDL_Surface * /*SD LCALL*/ IMG_LoadLBM_RW(SDL_RWops *src);

extern SDL_Surface * /*SD LCALL*/ IMG_ReadXPMFromArray(char **xpm);

/* We'll use SDL for reporting errors */
#define IMG_SetError	SDL_SetError
#define IMG_GetError	SDL_GetError

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
////#inc lude "clo se_code.h"

#endif /* _SDL_IMAGE_H */
