/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2FSDL_pixels_c.h
psp - ���r�W���� 440Subversion ���|�W�g���ꗗ: pspps2ps2wareps3ps3warepspware���r�W����:(root)/trunk/SDL/src/video/SDL_pixels_c.h
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
 "@(#) $Id: PSPL_pixels_c.h,v 1.5 2004/01/04 16:49:21 slouken Exp $";
#endif

/* Useful functions and variables from SDL_pixel.c */

#include "./PSPL_blit.h"

/* Pixel format functions */
extern SDL_PixelFormat *SDL_AllocFormat(int bpp,
		u32 Rmask, u32 Gmask, u32 Bmask, u32 Amask);
extern SDL_PixelFormat *SDL_ReallocFormat(SDL_Surface *surface, int bpp,
		u32 Rmask, u32 Gmask, u32 Bmask, u32 Amask);
extern void SDL_FormatChanged(SDL_Surface *surface);
extern void SDL_FreeFormat(SDL_PixelFormat *format);

/* Blit mapping functions */
extern SDL_BlitMap *SDL_AllocBlitMap(void);
extern void SDL_InvalidateMap(SDL_BlitMap *map);
extern int SDL_MapSurface (SDL_Surface *src, SDL_Surface *dst);
extern void SDL_FreeBlitMap(SDL_BlitMap *map);

/* Miscellaneous functions */
extern u16 SDL_CalculatePitch(SDL_Surface *surface);
extern void SDL_DitherColors(SDL_Color *colors, int bpp);
extern u8 SDL_FindColor(SDL_Palette *pal, u8 r, u8 g, u8 b);
extern void SDL_ApplyGamma(u16 *gamma, SDL_Color *colors, SDL_Color *output, int ncolors);

