
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_video.h,v 1.19 2005/01/02 05:11:16 slouken Exp $";
#endif

#ifdef _SDL_H
error! "このプログラムは SDL.h がインクルードされていると、コンパイルできません。"
#endif

#ifdef _SDL_video_h
error! "このプログラムは SDL_video.h がインクルードされていると、コンパイルできません。"
#endif

/* Header file for access to the SDL raw framebuffer window */

#ifndef _PSPL_video_h
#define _PSPL_video_h

#include <stdio.h>

#include "PSPL_types.h"
#include "PSPL_mutex.h"
#include "PSPL_rwops.h"

//#inc lude "beg in_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Transparency definitions: These define alpha as the opacity of a surface */
#define SDL_ALPHA_OPAQUE 255
#define SDL_ALPHA_TRANSPARENT 0

/* Useful data types */
typedef struct SDL_Rect
{
	s16 x, y;
	u16 w, h;
} SDL_Rect;

typedef struct SDL_Color
{
	u8 r;
	u8 g;
	u8 b;
	u8 unused;
} SDL_Color;
#define SDL_Colour SDL_Color

typedef struct SDL_Palette
{
	int 	  ncolors;
	SDL_Color *colors;
} SDL_Palette;

/* Everything in the pixel format structure is read-only */
typedef struct SDL_PixelFormat
{
	SDL_Palette *palette;
	u8  BitsPerPixel;
	u8  BytesPerPixel;
	u8  Rloss;
	u8  Gloss;
	u8  Bloss;
	u8  Aloss;
	u8  Rshift;
	u8  Gshift;
	u8  Bshift;
	u8  Ashift;
	u32 Rmask;
	u32 Gmask;
	u32 Bmask;
	u32 Amask;

	/* RGB color key information */
	u32 colorkey;
	/* Alpha value information (per-surface alpha) */
	u8  alpha;
} SDL_PixelFormat;

/* This structure should be treated as read-only, except for 'pixels',
   which, if not NULL, contains the raw pixel data for the surface.
*/
typedef struct SDL_Surface
{
	u32 flags;				/* Read-only */
	SDL_PixelFormat *format;		/* Read-only */
	int w, h;				/* Read-only */
	u16 pitch;				/* Read-only */
	void *pixels;				/* Read-write */
	int offset; 			/* Private */

	/* Hardware-specific surface info */
	struct private_hwdata *hwdata;

	/* clipping information */
	SDL_Rect clip_rect; 		/* Read-only */
	u32 unused1; 			/* for binary compatibility */

	/* Allow recursive locks */
	u32 locked;				/* Private */

	/* info for fast blit mapping to other surfaces */
	struct SDL_BlitMap *map;		/* Private */

	/* format version, bumped at every change to invalidate blit maps */
	unsigned int format_version;		/* Private */

	/* Reference count -- used when freeing surface */
	int refcount;				/* Read-mostly */
} SDL_Surface;

/* These are the currently supported flags for the SDL_surface */
/* Available for SDL_CreateRGBSurface() or SDL_SetVideoMode() */
#define SDL_SWSURFACE		0x00000000	/* Surface is in system memory */
#define SDL_HWSURFACE		0x00000001	/* Surface is in video memory */
#define SDL_ASYNCBLIT		0x00000004	/* Use asynchronous blits if possible */
/* Available for SDL_SetVideoMode() */
#define SDL_ANYFORMAT		0x10000000	/* Allow any video depth/pixel-format */
#define SDL_HWPALETTE		0x20000000	/* Surface has exclusive palette */
#define SDL_DOUBLEBUF		0x40000000	/* Set up double-buffered video mode */
#define SDL_FULLSCREEN		0x80000000	/* Surface is a full screen display */
//#define SDL_OPENGL			0x00000002	/* Create an OpenGL rendering context */
//#define SDL_OPENGLBLIT		0x0000000A	/* Create an OpenGL rendering context and use it for blitting */
#define SDL_RESIZABLE		0x00000010	/* This video mode may be resized */
#define SDL_NOFRAME 		0x00000020	/* No window caption or edge frame */
/* Used internally (read-only) */
#define SDL_HWACCEL 		0x00000100	/* Blit uses hardware acceleration */
#define SDL_SRCCOLORKEY 	0x00001000	/* Blit uses a source color key */
#define SDL_RLEACCELOK		0x00002000	/* Private flag */
#define SDL_RLEACCEL		0x00004000	/* Surface is RLE encoded */
#define SDL_SRCALPHA		0x00010000	/* Blit uses source alpha blending */
#define SDL_PREALLOC		0x01000000	/* Surface uses preallocated memory */

/* Evaluates to true if the surface needs to be locked before access */
#define SDL_MUSTLOCK(surface)	\
  (surface->offset ||		\
  ((surface->flags & (SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_RLEACCEL)) != 0))

/* typedef for private surface blitting functions */
typedef int (*SDL_blit)(struct SDL_Surface *src, SDL_Rect *srcrect,
			struct SDL_Surface *dst, SDL_Rect *dstrect);


/* Useful for determining the video hardware capabilities */
typedef struct SDL_VideoInfo
{
	u32 hw_available :1; /* Flag: Can you create hardware surfaces? */
	u32 wm_available :1; /* Flag: Can you talk to a window manager? */
	u32 UnusedBits1	:6;
	u32 UnusedBits2	:1;
	u32 blit_hw		:1; /* Flag: Accelerated blits HW --> HW */
	u32 blit_hw_CC	:1; /* Flag: Accelerated blits with Colorkey */
	u32 blit_hw_A	:1; /* Flag: Accelerated blits with Alpha */
	u32 blit_sw		:1; /* Flag: Accelerated blits SW --> HW */
	u32 blit_sw_CC	:1; /* Flag: Accelerated blits with Colorkey */
	u32 blit_sw_A	:1; /* Flag: Accelerated blits with Alpha */
	u32 blit_fill	:1; /* Flag: Accelerated color fill */
	u32 UnusedBits3	:16;
	u32 video_mem;	/* The total amount of video memory (in K) */
	SDL_PixelFormat *vfmt;	/* Value: The format of the video surface */
} SDL_VideoInfo;


/* The most common video overlay formats.
   For an explanation of these pixel formats, see:
	http://www.webartz.com/fourcc/indexyuv.htm

   For information on the relationship between color spaces, see:
   http://www.neuro.sfc.keio.ac.jp/~aly/polygon/info/color-space-faq.html
 */
#define SDL_YV12_OVERLAY  0x32315659	/* Planar mode: Y + V + U  (3 planes) */
#define SDL_IYUV_OVERLAY  0x56555949	/* Planar mode: Y + U + V  (3 planes) */
#define SDL_YUY2_OVERLAY  0x32595559	/* Packed mode: Y0+U0+Y1+V0 (1 plane) */
#define SDL_UYVY_OVERLAY  0x59565955	/* Packed mode: U0+Y0+V0+Y1 (1 plane) */
#define SDL_YVYU_OVERLAY  0x55595659	/* Packed mode: Y0+V0+Y1+U0 (1 plane) */

/* The YUV hardware video overlay */
typedef struct SDL_Overlay
{
	u32 format;				/* Read-only */
	int w, h;					/* Read-only */
	int planes; 				/* Read-only */
	u16 *pitches;			/* Read-only */
	u8 **pixels; 			/* Read-write */

	/* Hardware-specific surface info */
	struct private_yuvhwfuncs *hwfuncs;
	struct private_yuvhwdata *hwdata;

	/* Special flags */
	u32 hw_overlay :1;	/* Flag: This overlay hardware accelerated? */
	u32 UnusedBits :31;
} SDL_Overlay;


/* flags for SDL_SetPalette() */
#define SDL_LOGPAL 0x01
#define SDL_PHYSPAL 0x02

/* Function prototypes */

/*
 * This function returns a pointer to the current display surface.
 * If SDL is doing format conversion on the display surface, this
 * function returns the publicly visible surface, not the real video
 * surface.
 */
extern SDL_Surface * /*SD LCALL*/ SDL_GetVideoSurface(void);

/*
 * This function returns a read-only pointer to information about the
 * video hardware.	If this is called before SDL_SetVideoMode(), the 'vfmt'
 * member of the returned structure will contain the pixel format of the
 * "best" video mode.
 */
extern const SDL_VideoInfo * /*SD LCALL*/ SDL_GetVideoInfo(void);

/*
 * Check to see if a particular video mode is supported.
 * It returns 0 if the requested mode is not supported under any bit depth,
 * or returns the bits-per-pixel of the closest available mode with the
 * given width and height.	If this bits-per-pixel is different from the
 * one used when setting the video mode, SDL_SetVideoMode() will succeed,
 * but will emulate the requested bits-per-pixel with a shadow surface.
 *
 * The arguments to SDL_VideoModeOK() are the same ones you would pass to
 * SDL_SetVideoMode()
 */
extern int /*SD LCALL*/ SDL_VideoModeOK(int width, int height, int bpp, u32 flags);

/*
 * Return a pointer to an array of available screen dimensions for the
 * given format and video flags, sorted largest to smallest.  Returns
 * NULL if there are no dimensions available for a particular format,
 * or (SDL_Rect **)-1 if any dimension is okay for the given format.
 *
 * If 'format' is NULL, the mode list will be for the format given
 * by SDL_GetVideoInfo()->vfmt
 */
//廃止済	extern SDL_Rect ** /*SD LCALL*/ SDL_List Modes(SDL_PixelFormat *format, u32 flags);

/*
 * Set up a video mode with the specified width, height and bits-per-pixel.
 *
 * If 'bpp' is 0, it is treated as the current display bits per pixel.
 *
 * If SDL_ANYFORMAT is set in 'flags', the SDL library will try to set the
 * requested bits-per-pixel, but will return whatever video pixel format is
 * available.  The default is to emulate the requested pixel format if it
 * is not natively available.
 *
 * If SDL_HWSURFACE is set in 'flags', the video surface will be placed in
 * video memory, if possible, and you may have to call SDL_LockSurface()
 * in order to access the raw framebuffer.	Otherwise, the video surface
 * will be created in system memory.
 *
 * If SDL_ASYNCBLIT is set in 'flags', SDL will try to perform rectangle
 * updates asynchronously, but you must always lock before accessing pixels.
 * SDL will wait for updates to complete before returning from the lock.
 *
 * If SDL_HWPALETTE is set in 'flags', the SDL library will guarantee
 * that the colors set by SDL_SetColors() will be the colors you get.
 * Otherwise, in 8-bit mode, SDL_SetColors() may not be able to set all
 * of the colors exactly the way they are requested, and you should look
 * at the video surface structure to determine the actual palette.
 * If SDL cannot guarantee that the colors you request can be set,
 * i.e. if the colormap is shared, then the video surface may be created
 * under emulation in system memory, overriding the SDL_HWSURFACE flag.
 *
 * If SDL_FULLSCREEN is set in 'flags', the SDL library will try to set
 * a fullscreen video mode.  The default is to create a windowed mode
 * if the current graphics system has a window manager.
 * If the SDL library is able to set a fullscreen video mode, this flag
 * will be set in the surface that is returned.
 *
 * If SDL_DOUBLEBUF is set in 'flags', the SDL library will try to set up
 * two surfaces in video memory and swap between them when you call
 * SDL_Flip().	This is usually slower than the normal single-buffering
 * scheme, but prevents "tearing" artifacts caused by modifying video
 * memory while the monitor is refreshing.	It should only be used by
 * applications that redraw the entire screen on every update.
 *
 * If SDL_RESIZABLE is set in 'flags', the SDL library will allow the
 * window manager, if any, to resize the window at runtime.  When this
 * occurs, SDL will send a SDL_VIDEORESIZE event to you application,
 * and you must respond to the event by re-calling SDL_SetVideoMode()
 * with the requested size (or another size that suits the application).
 *
 * If SDL_NOFRAME is set in 'flags', the SDL library will create a window
 * without any title bar or frame decoration.  Fullscreen video modes have
 * this flag set automatically.
 *
 * This function returns the video framebuffer surface, or NULL if it fails.
 *
 * If you rely on functionality provided by certain video flags, check the
 * flags of the returned surface to make sure that functionality is available.
 * SDL will fall back to reduced functionality if the exact flags you wanted
 * are not available.
 */
extern SDL_Surface * /*SD LCALL*/ SDL_SetVideoMode(int width, int height, int bpp, u32 flags);

/*
 * Makes sure the given list of rectangles is updated on the given screen.
 * If 'x' and 'y' and 'w' and 'h' are all 0, SDL_UpdateRect will update the entire
 * screen.
 * These functions should not be called while 'screen' is locked.
 */
extern void /*SD LCALL*/ SDL_UpdateRects(SDL_Surface *screen, int numrects, SDL_Rect *rects);
extern void /*SD LCALL*/ SDL_UpdateRect(SDL_Surface *screen, s32 x, s32 y, u32 w, u32 h);

/*
 * On hardware that supports double-buffering, this function sets up a flip
 * and returns.  The hardware will wait for vertical retrace, and then swap
 * video buffers before the next video surface blit or lock will return.
 * On hardware that doesn not support double-buffering, this is equivalent
 * to calling SDL_UpdateRect(screen, 0, 0, 0, 0);
 * The SDL_DOUBLEBUF flag must have been passed to SDL_SetVideoMode() when
 * setting the video mode for this function to perform hardware flipping.
 * This function returns 0 if successful, or -1 if there was an error.
 */
extern int /*SD LCALL*/ SDL_Flip(SDL_Surface *screen);

/*
 * Set the gamma correction for each of the color channels.
 * The gamma values range (approximately) between 0.1 and 10.0
 *
 * If this function isn't supported directly by the hardware, it will
 * be emulated using gamma ramps, if available.  If successful, this
 * function returns 0, otherwise it returns -1.
 */
extern int /*SD LCALL*/ SDL_SetGamma(float red, float green, float blue);

/*
 * Set the gamma translation table for the red, green, and blue channels
 * of the video hardware.  Each table is an array of 256 16-bit quantities,
 * representing a mapping between the input and output for that channel.
 * The input is the index into the array, and the output is the 16-bit
 * gamma value at that index, scaled to the output color precision.
 *
 * You may pass NULL for any of the channels to leave it unchanged.
 * If the call succeeds, it will return 0.	If the display driver or
 * hardware does not support gamma translation, or otherwise fails,
 * this function will return -1.
 */
extern int /*SD LCALL*/ SDL_SetGammaRamp(const u16 *red, const u16 *green, const u16 *blue);

/*
 * Retrieve the current values of the gamma translation tables.
 *
 * You must pass in valid pointers to arrays of 256 16-bit quantities.
 * Any of the pointers may be NULL to ignore that channel.
 * If the call succeeds, it will return 0.	If the display driver or
 * hardware does not support gamma translation, or otherwise fails,
 * this function will return -1.
 */
extern int /*SD LCALL*/ SDL_GetGammaRamp(u16 *red, u16 *green, u16 *blue);

/*
 * Sets a portion of the colormap for the given 8-bit surface.	If 'surface'
 * is not a palettized surface, this function does nothing, returning 0.
 * If all of the colors were set as passed to SDL_SetColors(), it will
 * return 1.  If not all the color entries were set exactly as given,
 * it will return 0, and you should look at the surface palette to
 * determine the actual color palette.
 *
 * When 'surface' is the surface associated with the current display, the
 * display colormap will be updated with the requested colors.	If
 * SDL_HWPALETTE was set in SDL_SetVideoMode() flags, SDL_SetColors()
 * will always return 1, and the palette is guaranteed to be set the way
 * you desire, even if the window colormap has to be warped or run under
 * emulation.
 */
extern int /*SD LCALL*/ SDL_SetColors(SDL_Surface *surface,
			SDL_Color *colors, int firstcolor, int ncolors);

/*
 * Sets a portion of the colormap for a given 8-bit surface.
 * 'flags' is one or both of:
 * SDL_LOGPAL  -- set logical palette, which controls how blits are mapped
 *		to/from the surface,
 * SDL_PHYSPAL -- set physical palette, which controls how pixels look on
 *		the screen
 * Only screens have physical palettes. Separate change of physical/logical
 * palettes is only possible if the screen has SDL_HWPALETTE set.
 *
 * The return value is 1 if all colours could be set as requested, and 0
 * otherwise.
 *
 * SDL_SetColors() is equivalent to calling this function with
 *	   flags = (SDL_LOGPAL|SDL_PHYSPAL).
 */
extern int /*SD LCALL*/ SDL_SetPalette(SDL_Surface *surface, int flags,
				   SDL_Color *colors, int firstcolor,
				   int ncolors);

/*
 * Maps an RGB triple to an opaque pixel value for a given pixel format
 */
extern u32 /*SD LCALL*/ SDL_MapRGB
			(SDL_PixelFormat *format, u8 r, u8 g, u8 b);

/*
 * Maps an RGBA quadruple to a pixel value for a given pixel format
 */
extern u32 /*SD LCALL*/ SDL_MapRGBA(SDL_PixelFormat *format,
				   u8 r, u8 g, u8 b, u8 a);

/*
 * Maps a pixel value into the RGB components for a given pixel format
 */
extern void /*SD LCALL*/ SDL_GetRGB(u32 pixel, SDL_PixelFormat *fmt,
	u8 *r, u8 *g, u8 *b);

/*
 * Maps a pixel value into the RGBA components for a given pixel format
 */
extern void /*SD LCALL*/ SDL_GetRGBA(u32 pixel, SDL_PixelFormat *fmt,
	u8 *r, u8 *g, u8 *b, u8 *a);

/*
 * Allocate and free an RGB surface (must be called after SDL_SetVideoMode)
 * If the depth is 4 or 8 bits, an empty palette is allocated for the surface.
 * If the depth is greater than 8 bits, the pixel format is set using the
 * flags '[RGB]mask'.
 * If the function runs out of memory, it will return NULL.
 *
 * The 'flags' tell what kind of surface to create.
 * SDL_SWSURFACE means that the surface should be created in system memory.
 * SDL_HWSURFACE means that the surface should be created in video memory,
 * with the same format as the display surface.  This is useful for surfaces
 * that will not change much, to take advantage of hardware acceleration
 * when being blitted to the display surface.
 * SDL_ASYNCBLIT means that SDL will try to perform asynchronous blits with
 * this surface, but you must always lock it before accessing the pixels.
 * SDL will wait for current blits to finish before returning from the lock.
 * SDL_SRCCOLORKEY indicates that the surface will be used for colorkey blits.
 * If the hardware supports acceleration of colorkey blits between
 * two surfaces in video memory, SDL will try to place the surface in
 * video memory. If this isn't possible or if there is no hardware
 * acceleration available, the surface will be placed in system memory.
 * SDL_SRCALPHA means that the surface will be used for alpha blits and
 * if the hardware supports hardware acceleration of alpha blits between
 * two surfaces in video memory, to place the surface in video memory
 * if possible, otherwise it will be placed in system memory.
 * If the surface is created in video memory, blits will be _much_ faster,
 * but the surface format must be identical to the video surface format,
 * and the only way to access the pixels member of the surface is to use
 * the SDL_LockSurface() and SDL_UnlockSurface() calls.
 * If the requested surface actually resides in video memory, SDL_HWSURFACE
 * will be set in the flags member of the returned surface.  If for some
 * reason the surface could not be placed in video memory, it will not have
 * the SDL_HWSURFACE flag set, and will be created in system memory instead.
 */
#define SDL_AllocSurface	SDL_CreateRGBSurface
extern SDL_Surface * /*SD LCALL*/ SDL_CreateRGBSurface(
	u32 flags,
	int width, int height, int depth,
	u32 Rmask, u32 Gmask, u32 Bmask, u32 Amask);
extern SDL_Surface * /*SD LCALL*/ SDL_CreateRGBSurfaceFrom(
	void *pixels,
	int width, int height, int depth, int pitch,
	u32 Rmask, u32 Gmask, u32 Bmask, u32 Amask);
extern void /*SD LCALL*/ SDL_FreeSurface(SDL_Surface *surface);

/*
 * SDL_LockSurface() sets up a surface for directly accessing the pixels.
 * Between calls to SDL_LockSurface()/SDL_UnlockSurface(), you can write
 * to and read from 'surface->pixels', using the pixel format stored in
 * 'surface->format'.  Once you are done accessing the surface, you should
 * use SDL_UnlockSurface() to release it.
 *
 * Not all surfaces require locking.  If SDL_MUSTLOCK(surface) evaluates
 * to 0, then you can read and write to the surface at any time, and the
 * pixel format of the surface will not change.  In particular, if the
 * SDL_HWSURFACE flag is not given when calling SDL_SetVideoMode(), you
 * will not need to lock the display surface before accessing it.
 *
 * No operating system or library calls should be made between lock/unlock
 * pairs, as critical system locks may be held during this time.
 *
 * SDL_LockSurface() returns 0, or -1 if the surface couldn't be locked.
 */
extern int /*SD LCALL*/ SDL_LockSurface(SDL_Surface *surface);
extern void /*SD LCALL*/ SDL_UnlockSurface(SDL_Surface *surface);

/*
 * Load a surface from a seekable SDL data source (memory or file.)
 * If 'freesrc' is non-zero, the source will be closed after being read.
 * Returns the new surface, or NULL if there was an error.
 * The new surface should be freed with SDL_FreeSurface().
 */
extern SDL_Surface * /*SD LCALL*/ SDL_LoadBMP_RW(SDL_RWops *src, int freesrc);

/* Convenience macro -- load a surface from a file */
#define SDL_LoadBMP(file)	SDL_LoadBMP_RW(SDL_RWFromFile(file, "rb"), 1)

/*
 * Save a surface to a seekable SDL data source (memory or file.)
 * If 'freedst' is non-zero, the source will be closed after being written.
 * Returns 0 if successful or -1 if there was an error.
------------
extern int PSPL_save_vram16_to_file(const char *save_bmp_file_name);
------------
pspのvramをファイルに保存
16bitモード時専用 オフセット0 のみサポート。
(32bitモードはサポートしない)
PSPのVRAM オフセット0(0x44000000)から 512x272 の領域のうち、
480x272 を 24bit 無圧縮 Windows BMP 形式で保存。
------------
PSPのVRAMの使用方法を勝手に決め打ちして固定しているので、
表示画面等切り替えていても一切考慮しないので注意。
とはいえ単に正確な画面が保存されないだけでエラーにはならない。
------------
現在内部で Create surface で 24bit BMP分メモリ確保するので、遅い。
 */
//extern int /*SD LCALL*/ SDL_SaveBMP_RW(SDL_Surface *surface, SDL_RWops *dst, int freedst);
//extern int /*SD LCALL*/ PSPL_SaveBMP(SDL_Surface *surface, const char *save_bmp_file_name/*SDL_RWops *dst*/ );
extern int /*SD LCALL*/ PSPL_save_vram16_to_file(const char *save_bmp_file_name);
/* Convenience macro -- save a surface to a file */
//#define SDL_SaveBMP(surface, file) SDL_SaveBMP_RW(surface, SDL_RWFromFile(file, "wb"), 1)

/*
 * Sets the color key (transparent pixel) in a blittable surface.
 * If 'flag' is SDL_SRCCOLORKEY (optionally OR'd with SDL_RLEACCEL),
 * 'key' will be the transparent pixel in the source image of a blit.
 * SDL_RLEACCEL requests RLE acceleration for the surface if present,
 * and removes RLE acceleration if absent.
 * If 'flag' is 0, this function clears any current color key.
 * This function returns 0, or -1 if there was an error.
 */
extern int /*SD LCALL*/ SDL_SetColorKey
			(SDL_Surface *surface, u32 flag, u32 key);

/*
 * This function sets the alpha value for the entire surface, as opposed to
 * using the alpha component of each pixel. This value measures the range
 * of transparency of the surface, 0 being completely transparent to 255
 * being completely opaque. An 'alpha' value of 255 causes blits to be
 * opaque, the source pixels copied to the destination (the default). Note
 * that per-surface alpha can be combined with colorkey transparency.
 *
 * If 'flag' is 0, alpha blending is disabled for the surface.
 * If 'flag' is SDL_SRCALPHA, alpha blending is enabled for the surface.
 * OR:ing the flag with SDL_RLEACCEL requests RLE acceleration for the
 * surface; if SDL_RLEACCEL is not specified, the RLE accel will be removed.
 *
 * The 'alpha' parameter is ignored for surfaces that have an alpha channel.
 */
extern int /*SD LCALL*/ SDL_SetAlpha(SDL_Surface *surface, u32 flag, u8 alpha);

/*
 * Sets the clipping rectangle for the destination surface in a blit.
 *
 * If the clip rectangle is NULL, clipping will be disabled.
 * If the clip rectangle doesn't intersect the surface, the function will
 * return SDL_FALSE and blits will be completely clipped.  Otherwise the
 * function returns SDL_TRUE and blits to the surface will be clipped to
 * the intersection of the surface area and the clipping rectangle.
 *
 * Note that blits are automatically clipped to the edges of the source
 * and destination surfaces.
 */
extern SDL_bool /*SD LCALL*/ SDL_SetClipRect(SDL_Surface *surface, const SDL_Rect *rect);

/*
 * Gets the clipping rectangle for the destination surface in a blit.
 * 'rect' must be a pointer to a valid rectangle which will be filled
 * with the correct values.
 */
extern void /*SD LCALL*/ SDL_GetClipRect(SDL_Surface *surface, SDL_Rect *rect);

/*
 * Creates a new surface of the specified format, and then copies and maps
 * the given surface to it so the blit of the converted surface will be as
 * fast as possible.  If this function fails, it returns NULL.
 *
 * The 'flags' parameter is passed to SDL_CreateRGBSurface() and has those
 * semantics.  You can also pass SDL_RLEACCEL in the flags parameter and
 * SDL will try to RLE accelerate colorkey and alpha blits in the resulting
 * surface.
 *
 * This function is used internally by SDL_DisplayFormat().
 */
extern SDL_Surface * /*SD LCALL*/ SDL_ConvertSurface
			(SDL_Surface *src, SDL_PixelFormat *fmt, u32 flags);

/*
 * This performs a fast blit from the source surface to the destination
 * surface.  It assumes that the source and destination rectangles are
 * the same size.  If either 'srcrect' or 'dstrect' are NULL, the entire
 * surface (src or dst) is copied.	The final blit rectangles are saved
 * in 'srcrect' and 'dstrect' after all clipping is performed.
 * If the blit is successful, it returns 0, otherwise it returns -1.
 *
 * The blit function should not be called on a locked surface.
 *
 * The blit semantics for surfaces with and without alpha and colorkey
 * are defined as follows:
 *
 * RGBA->RGB:
 *	   SDL_SRCALPHA set:
 *	alpha-blend (using alpha-channel).
 *	SDL_SRCCOLORKEY ignored.
 *	   SDL_SRCALPHA not set:
 *	copy RGB.
 *	if SDL_SRCCOLORKEY set, only copy the pixels matching the
 *	RGB values of the source colour key, ignoring alpha in the
 *	comparison.
 *
 * RGB->RGBA:
 *	   SDL_SRCALPHA set:
 *	alpha-blend (using the source per-surface alpha value);
 *	set destination alpha to opaque.
 *	   SDL_SRCALPHA not set:
 *	copy RGB, set destination alpha to source per-surface alpha value.
 *	   both:
 *	if SDL_SRCCOLORKEY set, only copy the pixels matching the
 *	source colour key.
 *
 * RGBA->RGBA:
 *	   SDL_SRCALPHA set:
 *	alpha-blend (using the source alpha channel) the RGB values;
 *	leave destination alpha untouched. [Note: is this correct?]
 *	SDL_SRCCOLORKEY ignored.
 *	   SDL_SRCALPHA not set:
 *	copy all of RGBA to the destination.
 *	if SDL_SRCCOLORKEY set, only copy the pixels matching the
 *	RGB values of the source colour key, ignoring alpha in the
 *	comparison.
 *
 * RGB->RGB:
 *	   SDL_SRCALPHA set:
 *	alpha-blend (using the source per-surface alpha value).
 *	   SDL_SRCALPHA not set:
 *	copy RGB.
 *	   both:
 *	if SDL_SRCCOLORKEY set, only copy the pixels matching the
 *	source colour key.
 *
 * If either of the surfaces were in video memory, and the blit returns -2,
 * the video memory was lost, so it should be reloaded with artwork and
 * re-blitted:
	while ( PSPL_UpperBlit(image, imgrect, screen, dstrect) == -2 )
	{
		while ( SDL_LockSurface(image) < 0 )
			Sleep(10);
		-- Write image pixels to image->pixels --
		SDL_UnlockSurface(image);
	}
 * This happens under DirectX 5.0 when the system switches away from your
 * fullscreen application.	The lock will also fail until you have access
 * to the video memory again.
 */
/* You should call PSPL_UpperBlit() unless you know exactly how SDL
   blitting works internally and how to use the other blit functions.
*/
#define PSPL_UpperBlit SDL_UpperBlit

/* This is the public blit function, PSPL_UpperBlit(), and it performs
   rectangle validation and clipping before passing it to SDL_LowerBlit()
*/
extern int /*SD LCALL*/ SDL_UpperBlit
			(SDL_Surface *src, SDL_Rect *srcrect,
			 SDL_Surface *dst, SDL_Rect *dstrect);
/* This is a semi-private blit function and it performs low-level surface
   blitting only.
*/
extern int /*SD LCALL*/ SDL_LowerBlit
			(SDL_Surface *src, SDL_Rect *srcrect,
			 SDL_Surface *dst, SDL_Rect *dstrect);

/*
 * This function performs a fast fill of the given rectangle with 'color'
 * The given rectangle is clipped to the destination surface clip area
 * and the final fill rectangle is saved in the passed in pointer.
 * If 'dstrect' is NULL, the whole surface will be filled with 'color'
 * The color should be a pixel of the format used by the surface, and
 * can be generated by the SDL_MapRGB() function.
 * This function returns 0 on success, or -1 on error.
 */
extern int /*SD LCALL*/ SDL_FillRect
		(SDL_Surface *dst, SDL_Rect *dstrect, u32 color);

/*
 * This function takes a surface and copies it to a new surface of the
 * pixel format and colors of the video framebuffer, suitable for fast
 * blitting onto the display surface.  It calls SDL_ConvertSurface()
 *
 * If you want to take advantage of hardware colorkey or alpha blit
 * acceleration, you should set the colorkey and alpha value before
 * calling this function.
 *
 * If the conversion fails or runs out of memory, it returns NULL
 */
extern SDL_Surface * /*SD LCALL*/ SDL_DisplayFormat(SDL_Surface *surface);

/*
 * This function takes a surface and copies it to a new surface of the
 * pixel format and colors of the video framebuffer (if possible),
 * suitable for fast alpha blitting onto the display surface.
 * The new surface will always have an alpha channel.
 *
 * If you want to take advantage of hardware colorkey or alpha blit
 * acceleration, you should set the colorkey and alpha value before
 * calling this function.
 *
 * If the conversion fails or runs out of memory, it returns NULL
 */
extern SDL_Surface * /*SD LCALL*/ SDL_DisplayFormatAlpha(SDL_Surface *surface);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* YUV video surface overlay functions					   */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* This function creates a video output overlay
   Calling the returned surface an overlay is something of a misnomer because
   the contents of the display surface underneath the area where the overlay
   is shown is undefined - it may be overwritten with the converted YUV data.
*/
extern SDL_Overlay * /*SD LCALL*/ SDL_CreateYUVOverlay(int width, int height,
				u32 format, SDL_Surface *display);

/* Lock an overlay for direct access, and unlock it when you are done */
extern int /*SD LCALL*/ SDL_LockYUVOverlay(SDL_Overlay *overlay);
extern void /*SD LCALL*/ SDL_UnlockYUVOverlay(SDL_Overlay *overlay);

/* Blit a video overlay to the display surface.
   The contents of the video surface underneath the blit destination are
   not defined.
   The width and height of the destination rectangle may be different from
   that of the overlay, but currently only 2x scaling is supported.
*/
extern int /*SD LCALL*/ SDL_DisplayYUVOverlay(SDL_Overlay *overlay, SDL_Rect *dstrect);

/* Free a video overlay */
extern void /*SD LCALL*/ SDL_FreeYUVOverlay(SDL_Overlay *overlay);



/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
//#inc lude "clo se_code.h"

#endif /* _PSPL_video_h */
