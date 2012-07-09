/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2FSDL_surface.c

psp - リビジョン 441Subversion リポジトリ一覧: pspps2ps2wareps3ps3warepspwareリビジョン:(root)/trunk/SDL/src/video/SDL_surface.c
リビジョン 440 - 詳細表示 - 前のリビジョンとの差分 - 最終更新日時 - ログを見る -
*/

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_surface.c,v 1.21 2005/04/17 10:40:41 icculus Exp $";
#endif

#include <psptypes.h>
#include <pspge.h>
#include <pspkernel.h>
#include <psputils.h>
#include <pspdisplay.h>
#include <pspgu.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "./../include/PSPL_error.h"	/* 取り敢えず(仮) */
#include "./../include/PSPL_video.h"	/* 取り敢えず(仮) */
#include "PSPL_sysvideo.h"
//#include "PSPL_cursor_c.h"
#include "PSPL_blit.h"

#include "PSPL_pixels_c.h"
#include "PSPL_memops.h"
//#include "PSPL_leaks.h"

#if (1==USE_RLEACCEL)
	#include "PSPL_RLEaccel_c.h"
#endif

#ifndef SDL_OutOfMemory_bbb
	#define SDL_OutOfMemory_bbb(aaa)
#endif

#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb( ... )
#endif

/* Public routines */
/*
 * Create an empty RGB surface of the appropriate depth
 */
/*static*/extern int SDLVIDEO_PSP_AllocHWSurface_aaa(/*_THIS,*/ SDL_Surface *surface);
SDL_Surface * SDL_CreateRGBSurface(
	u32 flags,
	int width, int height, int depth,
	u32 Rmask, u32 Gmask, u32 Bmask, u32 Amask)
{
	SDL_Surface *screen;
	SDL_Surface *surface;

	/* Make sure the size requested doesn't overflow our datatypes */
	/* Next time I write a library like SDL, I'll use int for size. :) */
//	if ( (16384 <= width) || (65536 <= height) )
	if ( (512 < width) || (512 < height) )
	{
		SDL_SetError_bbb("Width or height is too large");
		return (NULL);
	}

	/* Check to see if we desire the surface in video memory */
	screen = (pspl_screen);
//	if ( pspl_screen/*video*/ )/*(pspl_screenはある？)*/
//	{
//		/*(PSPL_VideoInit()、SDL_SetVideoMode()以降の場合)*/
//		screen = (pspl_screen);
//	}
//	else
//	{	/*(PSPL_VideoInit()、SDL_SetVideoMode()以前の場合)*/
//		screen = NULL;/*(PSPL_VideoInit()でpspl_screenを新規作成する場合)*/
//	}
	if (
			(NULL!=screen) &&
		(SDL_HWSURFACE == (screen->flags&SDL_HWSURFACE) ) )
	{
		if (0 != (flags&(SDL_SRCCOLORKEY|SDL_SRCALPHA)) )
		{
			flags |= SDL_HWSURFACE;
		}
		if (SDL_SRCCOLORKEY == (flags & SDL_SRCCOLORKEY) )
		{
			if ( 0== 1/*info.blit_hw_CC*/ )
			{
				flags &= ~SDL_HWSURFACE;
			}
		}
		if (SDL_SRCALPHA == (flags & SDL_SRCALPHA) )
		{
			if ( 0== 0/*info.blit_hw_A*/ )
			{
				flags &= ~SDL_HWSURFACE;
			}
		}
	}
	else
	{
		flags &= ~SDL_HWSURFACE;
	}

	/* Allocate the surface */
//	surface = (SDL_Surface *)mal loc(sizeof(*surface));
	surface = (SDL_Surface *)memalign(16, (sizeof(*surface)));
	if (NULL == surface)
	{
		SDL_OutOfMemory_bbb();
		return (NULL);
	}
	surface->flags = SDL_SWSURFACE;
	if (SDL_HWSURFACE == (flags & SDL_HWSURFACE))
	{
	//	if (0 (Amask) && (display_form_at_alpha_pixel))
	//	{
	//		0 depth = display_form_at_alpha_pixel->BitsPerPixel;
	//		0 Rmask = display_form_at_alpha_pixel->Rmask;
	//		0 Gmask = display_form_at_alpha_pixel->Gmask;
	//		0 Bmask = display_form_at_alpha_pixel->Bmask;
	//		0 Amask = display_form_at_alpha_pixel->Amask;
	//	}
	//	else
		{
			depth = screen->format->BitsPerPixel;
			Rmask = screen->format->Rmask;
			Gmask = screen->format->Gmask;
			Bmask = screen->format->Bmask;
			Amask = screen->format->Amask;
		}
	}
	surface->format = SDL_AllocFormat(depth, Rmask, Gmask, Bmask, Amask);
	if (NULL == surface->format)
	{
		free(surface);
		return (NULL);
	}
	if ( Amask )
	{
		surface->flags |= SDL_SRCALPHA;
	}
	surface->w			= width;
	surface->h			= height;
	surface->pitch		= SDL_CalculatePitch(surface);
	surface->pixels 	= NULL;
	surface->offset 	= 0;
	surface->hwdata 	= NULL;
	surface->locked 	= 0;
	surface->map		= NULL;
//	surface->unused1	= 0;
	SDL_SetClipRect(surface, NULL);
	SDL_FormatChanged(surface);

	/* Get the pixels */
	if (
		(SDL_SWSURFACE == (flags&SDL_HWSURFACE) ) ||
	//	(video->AllocHWSurface(this, surface) < 0)
		(SDLVIDEO_PSP_AllocHWSurface_aaa(/*this,*/ surface) < 0)
	)
	{
		if ( surface->w && surface->h )
		{
		//	surface->pixels = mal loc(surface->h*surface->pitch);
			surface->pixels = memalign(16, (surface->h*surface->pitch));
			if (NULL == surface->pixels)
			{
				SDL_FreeSurface(surface);
				SDL_OutOfMemory_bbb();
				return (NULL);
			}
			/* This is important for bitmaps */
			memset(surface->pixels, 0, surface->h*surface->pitch );
		}
	}

	/* Allocate an empty mapping */
	surface->map = SDL_AllocBlitMap();
	if (NULL == surface->map)
	{
		SDL_FreeSurface(surface);
		return (NULL);
	}

	/* The surface is ready to go */
	surface->refcount = 1;
	#if defined(CHECK_LEAKS)
	surfaces_allocated++;
	#endif
	return (surface);
}
SDL_Surface * SDL_CreateSurface(
	u32 flags,
	int width, int height)
{
	SDL_Surface *surface;
	surface = SDL_CreateRGBSurface(
		flags, width, height,
	//	((pspl_screen)->format->BytesPerPixel*8),
		(pspl_screen)->format->BitsPerPixel,
		(pspl_screen)->format->Rmask,
		(pspl_screen)->format->Gmask,
		(pspl_screen)->format->Bmask,
		(pspl_screen)->format->Amask);
	return (surface);
}
/*
 * Create an RGB surface from an existing memory buffer
 */
SDL_Surface * SDL_CreateRGBSurfaceFrom(
	void *pixels,
	int width, int height,
	int depth, int pitch,
	u32 Rmask, u32 Gmask, u32 Bmask, u32 Amask)
{
	SDL_Surface *surface;

	surface = SDL_CreateRGBSurface(
		SDL_SWSURFACE, 0, 0,
		depth,
		Rmask, Gmask, Bmask, Amask);
	if (NULL != surface)
	{
		surface->flags |= SDL_PREALLOC;
		surface->pixels = pixels;
		surface->w = width;
		surface->h = height;
		surface->pitch = pitch;
		SDL_SetClipRect(surface, NULL);
	}
	return (surface);
}
/*
 * Set the color key in a blittable surface
 */
int SDL_SetColorKey(SDL_Surface *surface, u32 flag, u32 key)
{
	/* Sanity check the flag as it gets passed in */
	if ( flag & SDL_SRCCOLORKEY )
	{
		#if (1==USE_RLEACCEL)
		if ( flag & (SDL_RLEACCEL|SDL_RLEACCELOK) )
		{
			flag = (SDL_SRCCOLORKEY | SDL_RLEACCELOK);
		}
		else
		#endif /*(USE_RLEACCEL)*/
		{
			flag = (SDL_SRCCOLORKEY);
		}
	}
	else
	{
		flag = (0);
	}

	/* Optimize away operations that don't change anything */
	#if (1==USE_RLEACCEL)
	if (flag == (surface->flags & (SDL_SRCCOLORKEY|SDL_RLEACCELOK)))
	{
		if (key == surface->format->colorkey)
		{
			return (0);
		}
	}
	/* UnRLE surfaces before we change the colorkey */
	if ( surface->flags & SDL_RLEACCEL )
	{
		SDL_UnRLESurface(surface, 1);
	}
	#endif /*(USE_RLEACCEL)*/
	if ( flag )
	{
		surface->flags |= SDL_SRCCOLORKEY;
		surface->format->colorkey = key;
		if (SDL_HWACCEL == (surface->flags & SDL_HWACCEL) )
		{
//廃止済				if ( (NULL == (video->Set HW ColorKey)) ||
//廃止済					 (0 > (video->Set HW ColorKey(this, surface, key)) ) )
			{
				surface->flags &= ~SDL_HWACCEL;
			}
		}
		#if (1==USE_RLEACCEL)
		if ( (flag & SDL_RLEACCELOK) )
		{
			surface->flags |= SDL_RLEACCELOK;
		}
		else
		{
			surface->flags &= ~SDL_RLEACCELOK;
		}
		#endif /*(USE_RLEACCEL)*/
	}
	else
	{
		#if (1==USE_RLEACCEL)
		surface->flags &= ~(SDL_SRCCOLORKEY|SDL_RLEACCELOK);
		#else
		surface->flags &= ~(SDL_SRCCOLORKEY/*|SDL_RLEACCELOK*/);
		#endif /*(USE_RLEACCEL)*/
		surface->format->colorkey = 0;
	}
	SDL_InvalidateMap(surface->map);
	return (0);
}
/* This function sets the alpha channel of a surface */
int SDL_SetAlpha (SDL_Surface *surface, u32 flag, u8 value)
{
	u32 oldflags = surface->flags;
	u32 oldalpha = surface->format->alpha;

	/* Sanity check the flag as it gets passed in */
	if ( flag & SDL_SRCALPHA )
	{
		#if (1==USE_RLEACCEL)
		if ( flag & (SDL_RLEACCEL|SDL_RLEACCELOK) )
		{
			flag = (SDL_SRCALPHA | SDL_RLEACCELOK);
		}
		else
		#endif /*(USE_RLEACCEL)*/
		{
			flag = (SDL_SRCALPHA);
		}
	}
	else
	{
		flag = (0);
	}

	/* Optimize away operations that don't change anything */
	#if (1==USE_RLEACCEL)
	if (flag == (surface->flags & (SDL_SRCALPHA|SDL_RLEACCELOK)))
	{
		if ((!flag) || (value == oldalpha))
		{
			return (0);
		}
	}
	if (!(flag & SDL_RLEACCELOK) && (surface->flags & SDL_RLEACCEL))
	{
		SDL_UnRLESurface(surface, 1);
	}
	#endif /*(USE_RLEACCEL)*/
	if ( flag )
	{
		surface->flags |= SDL_SRCALPHA;
		surface->format->alpha = value;
		if (SDL_HWACCEL == (surface->flags & SDL_HWACCEL) )
		{
//廃止済	if ( (video->Set HW Alpha == NULL) ||
//廃止済		 (video->Set HW Alpha(this, surface, value) < 0) )
			{
				surface->flags &= ~SDL_HWACCEL;
			}
		}
		#if (1==USE_RLEACCEL)
		if ( flag & SDL_RLEACCELOK )
		{
			surface->flags |= SDL_RLEACCELOK;
		}
		else
		{
			surface->flags &= ~SDL_RLEACCELOK;
		}
		#endif /*(USE_RLEACCEL)*/
	}
	else
	{
		surface->flags &= ~SDL_SRCALPHA;
		surface->format->alpha = SDL_ALPHA_OPAQUE;
	}
	/*
	 * The representation for software surfaces is independent of
	 * per-surface alpha, so no need to invalidate the blit mapping
	 * if just the alpha value was changed. (If either is 255, we still
	 * need to invalidate.)
	 */
	if ( (SDL_HWACCEL == (surface->flags & SDL_HWACCEL))
	   || (oldflags != surface->flags)
	   || (((oldalpha + 1) ^ (value + 1)) & 0x100))
		SDL_InvalidateMap(surface->map);
	return (0);
}
int SDL_SetAlphaChannel(SDL_Surface *surface, u8 value)
{
	int row, col;
	int offset;
	u8 *buf;
	if ( (0xff000000 != (surface->format->Amask) ) &&
		 (0x000000ff != (surface->format->Amask) ) )
	{
		SDL_SetError_bbb("Unsupported surface alpha mask format");
		return (-1);
	}
	#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	if (0xff000000 == (surface->format->Amask) )
			{	offset = 3; 	}
	else	{	offset = 0; 	}
	#else
	if (0xff000000 == (surface->format->Amask) )
			{	offset = 0; 	}
	else	{	offset = 3; 	}
	#endif /* Byte ordering */

	/* Quickly set the alpha channel of an RGBA or ARGB surface */
	if ( SDL_MUSTLOCK(surface) )
	{
		if (0 > SDL_LockSurface(surface))
		{
			return (-1);
		}
	}
	row = surface->h;
	while (row--)
	{
		col = surface->w;
		buf = (u8 *)surface->pixels + row * surface->pitch + offset;
		while (col--)
		{
			*buf = value;
			buf += 4;
		}
	}
	if ( SDL_MUSTLOCK(surface) )
	{
		SDL_UnlockSurface(surface);
	}
	return (0);
}

/*
 * A function to calculate the intersection of two rectangles:
 * return true if the rectangles intersect, false otherwise
 */

/* psp専用の命令(pspのcpuにしか無いアセンブラ命令)を活用する。(組み込み関数) */
#ifndef psp_min
	#define psp_min __builtin_allegrex_min
#endif
#ifndef psp_max
	#define psp_max __builtin_allegrex_max
#endif

static __inline__ SDL_bool SDL_IntersectRect(const SDL_Rect *A, const SDL_Rect *B, SDL_Rect *intersection)
{
	int Amin;
	int Amax;
	int Bmin;
	int Bmax;
	/* Horizontal intersection */
	Amin = A->x;	Amax = Amin + A->w;
	Bmin = B->x;	Bmax = Bmin + B->w;
//	if (Amin < Bmin)	{	Amin = Bmin;}
	Amin = psp_max(Amin, Bmin);
	intersection->x = Amin;
//	if (Amax > Bmax)	{	Amax = Bmax;}
	Amax = psp_min(Amax, Bmax);
//	intersection->w = ((Amax - Amin) > 0) ? (Amax - Amin) : (0);
	intersection->w = psp_max((Amax - Amin), 0);
//
	/* Vertical intersection */
	Amin = A->y;	Amax = Amin + A->h;
	Bmin = B->y;	Bmax = Bmin + B->h;
//	if (Amin < Bmin)	{	Amin = Bmin;}
	Amin = psp_max(Amin, Bmin);
	intersection->y = Amin;
//	if (Amax > Bmax)	{	Amax = Bmax;}
	Amax = psp_min(Amax, Bmax);
//	intersection->h = ((Amax - Amin) > 0) ? (Amax - Amin) : (0);
	intersection->h = psp_max((Amax - Amin), 0);
//
	return (intersection->w && intersection->h);
}
/*
 * Set the clipping rectangle for a blittable surface
 */
SDL_bool SDL_SetClipRect(SDL_Surface *surface, const SDL_Rect *rect)
{
	SDL_Rect full_rect;

	/* Don't do anything if there's no surface to act on */
	if ( ! surface )
	{
		return (SDL_FALSE);
	}

	/* Set up the full surface rectangle */
	full_rect.x = 0;
	full_rect.y = 0;
	full_rect.w = surface->w;
	full_rect.h = surface->h;

	/* Set the clipping rectangle */
	if ( ! rect )
	{
		surface->clip_rect = full_rect;
		return (1);
	}
	return SDL_IntersectRect(rect, &full_rect, &surface->clip_rect);
}
void SDL_GetClipRect(SDL_Surface *surface, SDL_Rect *rect)
{
	if ( surface && rect )
	{
		*rect = surface->clip_rect;
	}
}
/*
 * Set up a blit between two surfaces -- split into three parts:
 * The upper part, SDL_UpperBlit(), performs clipping and rectangle
 * verification.  The lower part is a pointer to a low level
 * accelerated blitting function.
 *
 * These parts are separated out and each used internally by this
 * library in the optimimum places.  They are exported so that if
 * you know exactly what you are doing, you can optimize your code
 * by calling the one(s) you need.
 */
int SDL_LowerBlit(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect)
{
	/* Check to make sure the blit mapping is valid */
	if ( (src->map->dst != dst) ||
		 (src->map->dst->format_version != src->map->format_version) )
	{
		if (0 > SDL_MapSurface(src, dst))
		{
			return (-1);
		}
	}
	//
	SDL_blit do_blit;
//	SDL_Rect hw_srcrect;
//	SDL_Rect hw_dstrect;
	/* Figure out which blitter to use */
//	if ( (src->flags & SDL_HWACCEL) == SDL_HWACCEL )
//	{
//		if ( src == (pspl_screen) )
//		{
//			hw_srcrect = *srcrect;
//			hw_srcrect.x += current_video->offset_x;
//			hw_srcrect.y += current_video->offset_y;
//			srcrect = &hw_srcrect;
//		}
//		if ( dst == (pspl_screen) )
//		{
//			hw_dstrect = *dstrect;
//			hw_dstrect.x += current_video->offset_x;
//			hw_dstrect.y += current_video->offset_y;
//			dstrect = &hw_dstrect;
//		}
//		do_blit = src->map->hw _blit;
//	}
//	else
	{
		do_blit = src->map->sw_blit;
	}
	return (do_blit(src, srcrect, dst, dstrect));
}


int SDL_UpperBlit(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect)
{
	SDL_Rect fulldst;
	int srcx, srcy, w, h;
	/* Make sure the surfaces aren't locked */
	if ( (! src) || (! dst) )
	{
		SDL_SetError_bbb("SDL_UpperBlit: passed a NULL surface");
		return (-1);
	}
	if ( (src->locked) || (dst->locked) )
	{
		SDL_SetError_bbb("Surfaces must not be locked during blit");
		return (-1);
	}

	/* If the destination rectangle is NULL, use the entire dest surface */
	if (NULL == dstrect)
	{
		fulldst.x = fulldst.y = 0;
		dstrect = &fulldst;
	}

	/* clip the source rectangle to the source surface */
	if (srcrect)
	{
		int maxw, maxh;
		srcx = srcrect->x;
		w = srcrect->w;
		if (0 > srcx)	{	w += srcx;	dstrect->x -= srcx; srcx = 0;	}
		maxw = src->w - srcx;
	//	if (w > maxw)		{	w = maxw;		}
		w = psp_min(w, maxw);
		srcy = srcrect->y;
		h = srcrect->h;
		if (0 > srcy)	{	h += srcy;	dstrect->y -= srcy; srcy = 0;	}
		maxh = src->h - srcy;
	//	if (h > maxh)		{	h = maxh;		}
		h = psp_min(h, maxh);
	}
	else
	{
		srcx = srcy = 0;
		w = src->w;
		h = src->h;
	}

	/* clip the destination rectangle against the clip rectangle */
	{
		SDL_Rect *clip = &dst->clip_rect;
		int dx, dy;
		dx = clip->x - dstrect->x;					if (0 < dx) 	{	w -= dx;	dstrect->x += dx;	srcx += dx; 	}
		dx = dstrect->x + w - clip->x - clip->w;	if (0 < dx) 	{	w -= dx;	}
		dy = clip->y - dstrect->y;					if (0 < dy) 	{	h -= dy;	dstrect->y += dy;	srcy += dy; 	}
		dy = dstrect->y + h - clip->y - clip->h;	if (0 < dy) 	{	h -= dy;	}
	}

	if ((0 < w) && (0 < h))
	{
		SDL_Rect sr;
		sr.x = srcx;
		sr.y = srcy;
		sr.w = dstrect->w = w;
		sr.h = dstrect->h = h;
		return SDL_LowerBlit(src, &sr, dst, dstrect);
	}
	dstrect->w = dstrect->h = 0;
	return (0);
}

//static int SDL_FillRect1(SDL_Surface *dst, SDL_Rect *dstrect, u32 color)
//{
//	/* FIXME: We have to worry about packing order.. *sigh* */
//	SDL_SetError_bbb("1-bpp rect fill not yet implemented");
//	return (-1);
//}

//static int SDL_FillRect4(SDL_Surface *dst, SDL_Rect *dstrect, u32 color)
//{
//	/* FIXME: We have to worry about packing order.. *sigh* */
//	SDL_SetError_bbb("4-bpp rect fill not yet implemented");
//	return (-1);
//}

/*
 * This function performs a fast fill of the given rectangle with 'color'
 */
#if 0
#define pSDL_memset4(dst, val, len) 	\
do {									\
	unsigned _count = (len);			\
	unsigned _n = (_count + 3) / 4; 	\
	u32 *_p = (u32 *)(dst); 	\
	u32 _val = (val);				\
	switch (_count /*&(4-1)*/% 4)	{			\
	case 0: do {	*_p++ = _val;		\
	case 3:  *_p++ = _val;				\
	case 2:  *_p++ = _val;				\
	case 1:  *_p++ = _val;				\
		} while ( --_n );				\
	}									\
} while (0)
#else

	#if 1/*(てすと)*/
	extern void memset32(int *dest, int c, int count);
	extern void memset32_uncached(int *dest, int c, int count);
	extern void memcpy32(int *dest, int *src, int count); // 32bit word count
	#endif/*(てすと)*/

	#define pSDL_memset4 memset32
#endif

int SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, u32 color)
{
	int x, y;
	u8 *row;
	/* This function doesn't work on 8 bpp > surfaces. */
//	if (8 > dst->format->BitsPerPixel)
//	{
//		switch (dst->format->BitsPerPixel)
//		{
//		case 1: 	return SDL_FillRect1(dst, dstrect, color);			break;
//		case 4: 	return SDL_FillRect4(dst, dstrect, color);			break;
//	//	default:	SDL_SetError_bbb("Fill rect on unsupported surface format");return (-1);	break;
//		}
//	}
	/* If 'dstrect' == NULL, then fill the whole surface */
	if ( dstrect )
	{
		/* Perform clipping */
		if ( !SDL_IntersectRect(dstrect, &dst->clip_rect, dstrect) )
		{
			return (0);
		}
	}
	else
	{
		dstrect = &dst->clip_rect;
	}
	/* Check for hardware acceleration */
//	if ( ((dst->flags & SDL_HWSURFACE) == SDL_HWSURFACE) &&
//					video->info.blit_fill )
//	{
//		SDL_Rect hhhdw_rect;
//		if ( dst == (pspl_screen) )
//		{
//			hhhdw_rect = *dstrect;
//			hhhdw_rect.x += current_video->offset_x;
//			hhhdw_rect.y += current_video->offset_y;
//			dstrect = &hhhdw_rect;
//		}
//		return (video->Fill HW Rect(this, dst, dstrect, color));//廃止済
//	}
	/* Perform software fill */
	if ( SDL_LockSurface(dst) != 0 )
	{
		return (-1);
	}
	row = (u8 *)dst->pixels+dstrect->y*dst->pitch+
			dstrect->x*dst->format->BytesPerPixel;
	if ( dst->format->palette || (color == 0) )
	{
		x = dstrect->w*dst->format->BytesPerPixel;
		if ( !color && !((u32)row&3) && !(x&3) && !(dst->pitch&3) )
		{
			int n = x >> 2;
			for ( y=dstrect->h; y; --y )
			{
				pSDL_memset4( (void*)row, 0, n);
				row += dst->pitch;
			}
		}
		else
		{
			//__PPC__
			{
				for (y = dstrect->h; y; y--)
				{
					memset(row, color, x);
					row += dst->pitch;
				}
			}
		}
	}
	else
	{
		switch (dst->format->BytesPerPixel)
		{
		case 2:
			for ( y=dstrect->h; y; --y )
			{
				u16 *pixels = (u16 *)row;
				u16 c = color;
				u32 cc = (u32)c << 16 | c;
				int n = dstrect->w;
				if ((u32)pixels & 3)
				{
					*pixels++ = c;
					n--;
				}
				if (n >> 1)
				{	pSDL_memset4( (void*)pixels, cc, n >> 1);
				}
				if (n & 1)
				{	pixels[n - 1] = c;
				}
				row += dst->pitch;
			}
			break;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{	color <<= 8;
			}
			for ( y=dstrect->h; y; --y )
			{
				u8 *pixels = row;
				for ( x=dstrect->w; x; --x )
				{
					memcpy(pixels, &color, 3);
					pixels += 3;
				}
				row += dst->pitch;
			}
			break;

		case 4:
			for (y = dstrect->h; y; --y)
			{
				pSDL_memset4( (void*)row, color, dstrect->w);
				row += dst->pitch;
			}
			break;
		}
	}
	SDL_UnlockSurface(dst);
	/* We're done! */
	return (0);
}

/*
 * Lock a surface to directly access the pixels
 */

/* We need to wait for vertical retrace on page flipped displays */


//	/*static*/extern int SDLVIDEO_PSP_LockHWSurface_aaa(/*_THIS,*/ SDL_Surface *surface);
int SDL_LockSurface(SDL_Surface *surface)
{
	if ( ! surface->locked )
	{
		/* Perform the lock */
		if ( surface->flags & (SDL_HWSURFACE|SDL_ASYNCBLIT) )
		{
		//	if ( video->LockHWSurface(this, surface) < 0 )
		//	if ( SDLVIDEO_PSP_LockHWSurface_aaa(/*this,*/ surface) < 0 )
		//	if ( (0) < 0 )
		//	if ( /*SDLVIDEO_PSP_LockHWSurface(this, surface)*/(0) < 0 )
		//	{
		//		return (-1);
		//	}
		}
		#if (1==USE_RLEACCEL)
		if ( surface->flags & SDL_RLEACCEL )
		{
			SDL_UnRLESurface(surface, 1);
			surface->flags |= SDL_RLEACCEL; /* save accel'd state */
		}
		#endif /*(USE_RLEACCEL)*/
		/* This needs to be done here in case pixels changes value */
		surface->pixels = (u8 *)surface->pixels + surface->offset;
	}

	/* Increment the surface lock count, for recursive locks */
	++surface->locked;
	/* Ready to go.. */
	return (0);
}
/*
 * Unlock a previously locked surface
 */
void SDL_UnlockSurface(SDL_Surface *surface)
{
	/* Only perform an unlock if we are locked */
	if ( ! surface->locked || (--surface->locked > 0) )
	{
		return;
	}

	/* Perform the unlock */
	surface->pixels = (u8 *)surface->pixels - surface->offset;

	/* Unlock hardware or accelerated surfaces */
	if ( surface->flags & (SDL_HWSURFACE|SDL_ASYNCBLIT) )
	{
	//	video->UnlockHWSurface(this, surface);
	//	SDLVIDEO_PSP_UnlockHWSurface_aaa(/*this,*/ surface);
		/* Flush video RAM */
		sceKernelDcacheWritebackAll();
	}
	#if (1==USE_RLEACCEL)
	else
	{
		/* Update RLE encoded surface with new data */
		if ( (surface->flags & SDL_RLEACCEL) == SDL_RLEACCEL )
		{
			surface->flags &= ~SDL_RLEACCEL; /* stop lying */
			SDL_RLESurface(surface);
		}
	}
	#endif /*(USE_RLEACCEL)*/
}

/*
 * Convert a surface into the specified pixel format.
 */
SDL_Surface * SDL_ConvertSurface(SDL_Surface *surface, SDL_PixelFormat *format, u32 flags)
{
	SDL_Surface *convert;
	u32 colorkey = 0;
	u8 alpha = 0;
	u32 surface_flags;
	SDL_Rect bounds;

	/* Check for empty destination palette! (results in empty image) */
	if (NULL != format->palette)
	{
		int i;
		for (i=0; i<(format->palette->ncolors); i++)
		{
			if ( (format->palette->colors[i].r != 0) ||
				 (format->palette->colors[i].g != 0) ||
				 (format->palette->colors[i].b != 0) )
			{	break;	}
		}
		if ( i == format->palette->ncolors )
		{
			SDL_SetError_bbb("Empty destination palette");
			return (NULL);
		}
	}

	/* Only create hw surfaces with alpha channel if hw alpha blits
	   are supported */
	if (format->Amask != 0 && (flags & SDL_HWSURFACE))
	{
		if (//(0==) ||
			0==0/*info.blit_hw_A*/)
		{	flags &= ~SDL_HWSURFACE;
		}
	}

	/* Create a new surface with the desired format */
	convert = SDL_CreateRGBSurface(flags,
				surface->w, surface->h, format->BitsPerPixel,
		format->Rmask, format->Gmask, format->Bmask, format->Amask);
	if (NULL == convert)
	{
		return (NULL);
	}

	/* Copy the palette if any */
	if ( format->palette && convert->format->palette )
	{
		memcpy(convert->format->palette->colors,
				format->palette->colors,
				format->palette->ncolors*sizeof(SDL_Color));
		convert->format->palette->ncolors = format->palette->ncolors;
	}

	/* Save the original surface color key and alpha */
	surface_flags = surface->flags;
	if ( (surface_flags & SDL_SRCCOLORKEY) == SDL_SRCCOLORKEY )
	{
		/* Convert colourkeyed surfaces to RGBA if requested */
		if ((flags & SDL_SRCCOLORKEY) != SDL_SRCCOLORKEY
		   && format->Amask)
		{
			surface_flags &= ~SDL_SRCCOLORKEY;
		}
		else
		{
			colorkey = surface->format->colorkey;
			SDL_SetColorKey(surface, 0, 0);
		}
	}
	if ( (surface_flags & SDL_SRCALPHA) == SDL_SRCALPHA )
	{
		/* Copy over the alpha channel to RGBA if requested */
		if ( format->Amask )
		{
			surface->flags &= ~SDL_SRCALPHA;
		}
		else
		{
			alpha = surface->format->alpha;
			SDL_SetAlpha(surface, 0, 0);
		}
	}

	/* Copy over the image data */
	bounds.x = 0;
	bounds.y = 0;
	bounds.w = surface->w;
	bounds.h = surface->h;
	SDL_LowerBlit(surface, &bounds, convert, &bounds);

	/* Clean up the original surface, and update converted surface */
	if ( convert != NULL )
	{
		SDL_SetClipRect(convert, &surface->clip_rect);
	}
	if ( (surface_flags & SDL_SRCCOLORKEY) == SDL_SRCCOLORKEY )
	{
		#if (1==USE_RLEACCEL)
		u32 cflags = surface_flags&(SDL_SRCCOLORKEY|SDL_RLEACCELOK);
		#else
		u32 cflags = surface_flags&(SDL_SRCCOLORKEY/*|SDL_RLEACCELOK*/);
		#endif /*(USE_RLEACCEL)*/
		if ( convert != NULL )
		{
			u8 keyR, keyG, keyB;
			SDL_GetRGB(colorkey, surface->format, &keyR, &keyG, &keyB);
			#if (1==USE_RLEACCEL)
			SDL_SetColorKey(convert, cflags|(flags&SDL_RLEACCELOK), SDL_MapRGB(convert->format, keyR, keyG, keyB));
			#else
			SDL_SetColorKey(convert, cflags/*|(flags&SDL_RLEACCELOK)*/, SDL_MapRGB(convert->format, keyR, keyG, keyB));
			#endif /*(USE_RLEACCEL)*/
		}
		SDL_SetColorKey(surface, cflags, colorkey);
	}
	if ( (surface_flags & SDL_SRCALPHA) == SDL_SRCALPHA )
	{
		#if (1==USE_RLEACCEL)
		u32 aflags = surface_flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
		#else
		u32 aflags = surface_flags&(SDL_SRCALPHA/*|SDL_RLEACCELOK*/);
		#endif /*(USE_RLEACCEL)*/
		if ( convert != NULL )
		{
			#if (1==USE_RLEACCEL)
			SDL_SetAlpha(convert, aflags|(flags&SDL_RLEACCELOK), alpha);
			#else
			SDL_SetAlpha(convert, aflags/*|(flags&SDL_RLEACCELOK)*/, alpha);
			#endif /*(USE_RLEACCEL)*/
		}
		if ( format->Amask )
		{
			surface->flags |= SDL_SRCALPHA;
		}
		else
		{
			SDL_SetAlpha(surface, aflags, alpha);
		}
	}
	/* We're ready to go! */
	return (convert);
}

/*
 * Free a surface created by the above function.
 */
/*static*/extern void SDLVIDEO_PSP_FreeHWSurface_aaa(/*_THIS,*/ SDL_Surface *surface);
void SDL_FreeSurface(SDL_Surface *surface)
{
	/* Free anything that's not NULL, and not the screen surface */
	if (
		(surface == NULL)
		 || (surface == (pspl_screen))
	)
	{
		return;
	}
	if ( --surface->refcount > 0 )
	{
		return;
	}
	while ( surface->locked > 0 )
	{
		SDL_UnlockSurface(surface);
	}
	#if (1==USE_RLEACCEL)
	if ( (surface->flags & SDL_RLEACCEL) == SDL_RLEACCEL )
	{
		SDL_UnRLESurface(surface, 0);
	}
	#endif /*(USE_RLEACCEL)*/
	if ( surface->format )
	{
		SDL_FreeFormat(surface->format);
		surface->format = NULL;
	}
	if ( surface->map != NULL )
	{
		SDL_FreeBlitMap(surface->map);
		surface->map = NULL;
	}
	if ( surface->hwdata )
	{
	//	video->FreeHWSurface(this, surface);
		SDLVIDEO_PSP_FreeHWSurface_aaa(/*this,*/ surface);
	}
	if ( surface->pixels &&
		 ((surface->flags & SDL_PREALLOC) != SDL_PREALLOC) )
	{
		free(surface->pixels);
	}
	free(surface);
#ifdef CHECK_LEAKS
	--surfaces_allocated;
#endif
}
