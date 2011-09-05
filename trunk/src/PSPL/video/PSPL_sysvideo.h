/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2FSDL_sysvideo.h
psp - ƒŠƒrƒWƒ‡ƒ“ 448Subversion ƒŠƒ|ƒWƒgƒŠˆê——: pspps2ps2wareps3ps3warepspwareƒŠƒrƒWƒ‡ƒ“:(root)/trunk/SDL/src/video/SDL_sysvideo.h
ƒŠƒrƒWƒ‡ƒ“ 441 - Ú×•\Ž¦ - ‘O‚ÌƒŠƒrƒWƒ‡ƒ“‚Æ‚Ì·•ª - ÅIXV“úŽž - ƒƒO‚ðŒ©‚é -
*/

/*---------------------------------------------------------
	“Œ•û–Í•í•— ` Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_sysvideo.h,v 1.24 2005/04/17 10:40:41 icculus Exp $";
#endif

#ifndef _PSPL_sysvideo_h
#define _PSPL_sysvideo_h

//#include "./../include/PSPL_mouse.h"
#define SDL_PROTOTYPES_ONLY
//#include "./../include/PSPL_syswm.h"
#undef SDL_PROTOTYPES_ONLY

/* This file prototypes the video driver implementation.
   This is designed to be easily converted to C++ in the future.
 */

/* The SDL video driver */
typedef struct SDL_VideoDevice SDL_VideoDevice;

/* Define the SDL video driver structure */
#define _THIS   SDL_VideoDevice *_this
#ifndef _STATUS
#define _STATUS SDL_status *status
#endif

	/* ----------------------------- */
	/* The name of this video driver */
//	const char *name_dummy;

	/* ----------------------------- */
	/* Initialization/Query functions */


	/* List the available video modes for the given pixel format, sorted
	   from largest to smallest.
	 */
//”pŽ~Ï	SDL_Rect **(*List Modes)(_THIS, SDL_PixelFormat *format, u32 flags);

	/* Toggle the fullscreen mode */
//	int (*Tog gleFullScreen)(_THIS, int on);

	/* Create a YUV video surface (possibly overlay) of the given
	   format.  The hardware should be able to perform at least 2x
	   scaling on display.
	 */
//”pŽ~Ï		SDL_Overlay *(*CreateYUVOverlay)(_THIS, int width, int height, u32 format, SDL_Surface *display);

	/* Sets the color entries { firstcolor .. (firstcolor+ncolors-1) }
	   of the physical palette to those in 'colors'. If the device is
	   using a software palette (SDL_HWPALETTE not set), then the
	   changes are reflected in the logical palette of the screen
	   as well.
	   The return value is 1 if all entries could be set properly
	   or 0 otherwise.
	*/
//”pŽ~Ï		int (*SetColors)(_THIS, int firstcolor, int ncolors, SDL_Color *colors);


	/* * * */
	/* Hardware acceleration functions */

	/* Sets the hardware accelerated blit function, if any, based
	   on the current flags of the surface (colorkey, alpha, etc.)
	 */
//”pŽ~Ï		int (*Check HW Blit)(_THIS, SDL_Surface *src, SDL_Surface *dst);

	/* Fills a surface rectangle with the given color */
//”pŽ~Ï		int (*Fill HW Rect)(_THIS, SDL_Surface *dst, SDL_Rect *rect, u32 color);

	/* Sets video mem colorkey and accelerated blit function */
//”pŽ~Ï		int (*Set HW ColorKey)(_THIS, SDL_Surface *surface, u32 key);

	/* Sets per surface hardware alpha value */
//”pŽ~Ï		int (*Set HW Alpha)(_THIS, SDL_Surface *surface, u8 value);

	/* Performs hardware flipping */
//”pŽ~Ï	int (*Flip HW Surface)(_THIS, SDL_Surface *surface);


//”pŽ~		int is_3 2bit;

	/* * * */
	/* Gamma support */
//”pŽ~	u16 *gamma;

	/* Set the gamma correction directly (emulated with gamma ramps) */
//??		int (*SetGamma)(_THIS, float red, float green, float blue);

	/* Get the gamma correction directly (emulated with gamma ramps) */
//??		int (*GetGamma)(_THIS, float *red, float *green, float *blue);

	/* Set the gamma ramp */
//??		int (*SetGammaRamp)(_THIS, u16 *ramp);

	/* Get the gamma ramp */
//??		int (*GetGammaRamp)(_THIS, u16 *ramp);

struct SDL_VideoDevice
{
	/* This pointer should exist in the native video subsystem and should
	   point to an appropriate update function for the current video mode
	 */
	void (*UpdateRects)(_THIS, int numrects, SDL_Rect *rects);

	/* Information about the video hardware */
	SDL_VideoInfo info;

	/* The pixel format used when SDL_CreateRGBSurface creates SDL_HWSURFACEs with alpha */
	SDL_PixelFormat* displayformatalphapixel;

	/* * * */
	/* Data common to all drivers */
	SDL_Surface *screen;
	SDL_Surface *shadow;
	SDL_Surface *visible;
//int dummy2;//”pŽ~	SDL_Palette *physpal;   /* physical palette, if != logical palette */
//int dummy1;//”pŽ~	SDL_Color *gammacols;   /* gamma-corrected colours, or NULL */
//	int offset_x;
//	int offset_y;

	/* Driver information flags */
//	int handles_any_size;   /* Driver handles any size video mode */



	/* * * */
};
#undef _THIS

//typedef struct VideoBootStrap
//{
//	const char *name;
//	const char *desc;
//	int (*available)(void);
//	SDL_VideoDevice *(*create)(int devindex);
//} VideoBootStrap;

//#ifdef ENABLE_PSP
//extern VideoBootStrap PSP_bootstrap;
//#endif

/* This is the current video device */
extern SDL_VideoDevice *current_video;

#define SDL_VideoSurface		(current_video->screen)
#define SDL_ShadowSurface		(current_video->shadow)
#define SDL_PublicSurface		(current_video->visible)

#endif /* _PSPL_sysvideo_h */
