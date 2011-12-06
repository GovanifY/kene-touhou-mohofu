/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2FSDL_pixels.c
psp - ���r�W���� 441Subversion ���|�W�g���ꗗ: pspps2ps2wareps3ps3warepspware���r�W����:(root)/trunk/SDL/src/video/SDL_pixels.c
���r�W���� 440 - �ڍו\�� - �O�̃��r�W�����Ƃ̍��� - �ŏI�X�V���� - ���O������ -
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
 "@(#) $Id: PSPL_pixels.c,v 1.14 2005/05/16 05:34:58 slouken Exp $";
#endif

/* General (mostly internal) pixel/color manipulation routines for SDL */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "../include/PSPL_error.h"	/* ��芸����(��) */
#include "../include/PSPL_video.h"	/* ��芸����(��) */
#include "PSPL_sysvideo.h"
#include "PSPL_blit.h"
#include "PSPL_pixels_c.h"

#if (1==USE_RLEACCEL)
	#include "PSPL_RLEaccel_c.h"
#endif

#ifndef SDL_OutOfMemory_bbb
	#define SDL_OutOfMemory_bbb(aaa)
#endif

/* Helper functions */
/*
 * Allocate a pixel format structure and fill it according to the given info.
 */
SDL_PixelFormat *SDL_AllocFormat(int bpp, u32 Rmask, u32 Gmask, u32 Bmask, u32 Amask)
{
	SDL_PixelFormat *format;
	u32 mask;

	/* Allocate an empty pixel format structure */
//	format = mal loc((sizeof(*format)));
	format = memalign(16, (sizeof(*format)));
	if ( format == NULL )
	{
		SDL_OutOfMemory_bbb();
		return (NULL);
	}
	memset(format, 0, sizeof(*format));
	format->alpha = SDL_ALPHA_OPAQUE;

	/* Set up the format */
	format->BitsPerPixel = bpp;
	format->BytesPerPixel = (bpp+7)/8;
	if ( Rmask || Bmask || Gmask )	/* Packed pixels with custom mask */
	{
		format->Amask = Amask;
		format->Bmask = Bmask;
		format->Gmask = Gmask;
		format->Rmask = Rmask;
		//
		format->Ashift	= 0;
		format->Bshift	= 0;
		format->Gshift	= 0;
		format->Rshift	= 0;
		//
		format->Aloss	= 8;
		format->Bloss	= 8;
		format->Gloss	= 8;
		format->Rloss	= 8;
		//
		format->palette = NULL;
		if (Amask)	{	for (mask=Amask; !(mask&0x01); mask>>=1)	{	format->Ashift++;}
						for (		   ;  (mask&0x01); mask>>=1)	{	format->Aloss--;}	}
		if (Bmask)	{	for (mask=Bmask; !(mask&0x01); mask>>=1)	{	format->Bshift++;}
						for (		   ;  (mask&0x01); mask>>=1)	{	format->Bloss--;}	}
		if (Gmask)	{	for (mask=Gmask; !(mask&0x01); mask>>=1)	{	format->Gshift++;}
						for (		   ;  (mask&0x01); mask>>=1)	{	format->Gloss--;}	}
		if (Rmask)	{	for (mask=Rmask; !(mask&0x01); mask>>=1)	{	format->Rshift++;}
						for (		   ;  (mask&0x01); mask>>=1)	{	format->Rloss--;}	}
	}
	else
	if ( bpp > 8 )	/* Packed pixels with standard mask */
	{
		format->Bmask = ((0xff>>format->Bloss)<<format->Bshift);
		format->Gmask = ((0xff>>format->Gloss)<<format->Gshift);
		format->Rmask = ((0xff>>format->Rloss)<<format->Rshift);
		/* R-G-B */
		if ( bpp > 24 ) 	{	bpp = 24;	}
		int bpp_div3;
		bpp_div3 = bpp;
		bpp_div3 *= (86);/*(int)(256/3)*/
		bpp_div3 >>= 8;/*(8==(24*86)/256)*/
		format->Bshift = 0;
		format->Gshift = (bpp_div3);
		format->Rshift = ((bpp_div3)+(bpp%3))+(bpp_div3);
		format->Bloss = 8-(bpp_div3);
		format->Gloss = 8-(bpp_div3)-(bpp%3);
		format->Rloss = 8-(bpp_div3);
	}
	else	/* Palettized formats have no mask info */
	{
		format->Amask	= 0;
		format->Bmask	= 0;
		format->Gmask	= 0;
		format->Rmask	= 0;
		format->Ashift	= 0;
		format->Bshift	= 0;
		format->Gshift	= 0;
		format->Rshift	= 0;
		format->Aloss	= 8;
		format->Bloss	= 8;
		format->Gloss	= 8;
		format->Rloss	= 8;
	}
	if ( bpp <= 8 ) 	/* Palettized mode */
	{
		int ncolors = 1<<bpp;
		#ifdef DEBUG_PALETTE
		fprintf(stderr,"bpp=%d ncolors=%d\n",bpp,ncolors);
		#endif
//		format->palette = (SDL_Palette *)mal loc((sizeof(SDL_Palette)));
		format->palette = (SDL_Palette *)memalign(16, (sizeof(SDL_Palette)));
		if ( format->palette == NULL )
		{
			SDL_FreeFormat(format);
			SDL_OutOfMemory_bbb();
			return (NULL);
		}
		(format->palette)->ncolors = ncolors;
//		(format->palette)->colors = (SDL_Color *)mal loc(((format->palette)->ncolors*sizeof(SDL_Color)));
		(format->palette)->colors = (SDL_Color *)memalign(16, ((format->palette)->ncolors*sizeof(SDL_Color)));
		if ( (format->palette)->colors == NULL )
		{
			SDL_FreeFormat(format);
			SDL_OutOfMemory_bbb();
			return (NULL);
		}
		if ( Rmask || Bmask || Gmask )
		{
			/* create palette according to masks */
			int i;
			int Rm=0,Gm=0,Bm=0;
			int Rw=0,Gw=0,Bw=0;
			#ifdef ENABLE_PALETTE_ALPHA
			int Am=0,Aw=0;
			#endif
			if (Rmask)	{	Rw=8-format->Rloss; 	for (i=format->Rloss;i>0;i-=Rw) 	{	Rm|=1<<i;	}	}
			if (Gmask)	{	Gw=8-format->Gloss; 	for (i=format->Gloss;i>0;i-=Gw) 	{	Gm|=1<<i;	}	}
			if (Bmask)	{	Bw=8-format->Bloss; 	for (i=format->Bloss;i>0;i-=Bw) 	{	Bm|=1<<i;	}	}
			#ifdef DEBUG_PALETTE
			fprintf(stderr,"Rw=%d Rm=0x%02X\n",Rw,Rm);
			fprintf(stderr,"Gw=%d Gm=0x%02X\n",Gw,Gm);
			fprintf(stderr,"Bw=%d Bm=0x%02X\n",Bw,Bm);
			#endif
		#ifdef ENABLE_PALETTE_ALPHA
			if (Amask)	{	Aw=8-format->Aloss; 	for (i=format->Aloss;i>0;i-=Aw) 	{	Am|=1<<i;	}	}
			#ifdef DEBUG_PALETTE
			fprintf(stderr,"Aw=%d Am=0x%02X\n",Aw,Am);
			#endif
		#endif
			for (i=0; i < ncolors; i++)
			{
				int r,g,b;
				r=(i&Rmask)>>format->Rshift;	r=(r<<format->Rloss)|((r*Rm)>>Rw);		format->palette->colors[i].r=r;
				g=(i&Gmask)>>format->Gshift;	g=(g<<format->Gloss)|((g*Gm)>>Gw);		format->palette->colors[i].g=g;
				b=(i&Bmask)>>format->Bshift;	b=(b<<format->Bloss)|((b*Bm)>>Bw);		format->palette->colors[i].b=b;
		#ifdef ENABLE_PALETTE_ALPHA
				a = (i&Amask)>>format->Ashift;
				a = (a<<format->Aloss)|((a*Am)>>Aw);
				format->palette->colors[i].a/*unused*/ = a;
		#else
				format->palette->colors[i].a/*unused*/ = (0);
		#endif
			}
		}
		else
		if ( ncolors == 2 )
		{
			/* Create a black and white bitmap palette */
			format->palette->colors[0].r = 0xff;
			format->palette->colors[0].g = 0xff;
			format->palette->colors[0].b = 0xff;
			format->palette->colors[1].r = 0x00;
			format->palette->colors[1].g = 0x00;
			format->palette->colors[1].b = 0x00;
		}
		else
		{
			/* Create an empty palette */
			memset((format->palette)->colors, 0, (format->palette)->ncolors*sizeof(SDL_Color) );
		}
	}
	return (format);
}


SDL_PixelFormat *SDL_ReallocFormat(
	SDL_Surface *surface, int bpp,
	u32 Rmask, u32 Gmask, u32 Bmask, u32 Amask)
{
	if ( surface->format )
	{
		SDL_FreeFormat(surface->format);
		SDL_FormatChanged(surface);
	}
	surface->format = SDL_AllocFormat(bpp, Rmask, Gmask, Bmask, Amask);
	return surface->format;
}

/*
 * Change any previous mappings from/to the new surface format
 */
void SDL_FormatChanged(SDL_Surface *surface)
{
	static int format_version = 0;
	format_version++;
	if ( format_version < 0 )	/* It wrapped... */
	{
		format_version = 1;
	}
	surface->format_version = format_version;
	SDL_InvalidateMap(surface->map);
}
/*
 * Free a previously allocated format structure
 */
void SDL_FreeFormat(SDL_PixelFormat *format)
{
	if ( format )
	{
		if ( format->palette )
		{
			if ( format->palette->colors )
			{
				free(format->palette->colors);
			}
			free(format->palette);
		}
		free(format);
	}
}
/*
 * Calculate an 8-bit (3 red, 3 green, 2 blue) dithered palette of colors
 */
void SDL_DitherColors(SDL_Color *colors, int bpp)
{
	int i;
	if (bpp != 8)
	{	return;  /* only 8bpp supported right now */
	}
	for (i = 0; i < 256; i++)
	{
		int r, g, b;
		/* map each bit field to the full [0, 255] interval,
		   so 0 is mapped to (0, 0, 0) and 255 to (255, 255, 255) */
		r = i & 0xe0;
		r |= r >> 3 | r >> 6;
		colors[i].r = r;
		g = (i << 3) & 0xe0;
		g |= g >> 3 | g >> 6;
		colors[i].g = g;
		b = i & 0x3;
		b |= b << 2;
		b |= b << 4;
		colors[i].b = b;
	}
}
/*
 * Calculate the pad-aligned scanline width of a surface
 */
u16 SDL_CalculatePitch(SDL_Surface *surface)
{
	/* Surface should be 4-byte aligned for speed */
	u16 pitch;
	pitch = surface->w*surface->format->BytesPerPixel;
	switch (surface->format->BitsPerPixel)
	{
	case 1: 	pitch = (pitch+7)/8;		break;
	case 4: 	pitch = (pitch+1)/2;		break;
	default:								break;
	}
	pitch = (pitch + 3) & ~3;		/* 4-byte aligning */
	return (pitch);
}
/*
 * Match an RGB value to a particular palette index
 */
u8 SDL_FindColor(SDL_Palette *pal, u8 r, u8 g, u8 b)
{
	/* Do colorspace distance matching */
	u8 pixel = 0;
	unsigned int smallest;
	smallest = ~0;
	{
		int i;
		for (i=0; i<pal->ncolors; i++)
		{
			unsigned int distance;
			{
				int rd;
				int gd;
				int bd;
				rd = pal->colors[i].r - r;
				gd = pal->colors[i].g - g;
				bd = pal->colors[i].b - b;
				distance = (rd*rd)+(gd*gd)+(bd*bd);
			}
			if ( distance < smallest )
			{
				pixel = i;
				if ( distance == 0 )	/* Perfect match! */
				{
					break;
				}
				smallest = distance;
			}
		}
	}
	return (pixel);
}

/* Find the opaque pixel value corresponding to an RGB triple */
u32 SDL_MapRGB(SDL_PixelFormat *format, u8 r, u8 g, u8 b)
{
	if ( format->palette == NULL )
	{
		return (r >> format->Rloss) << format->Rshift
			   | (g >> format->Gloss) << format->Gshift
			   | (b >> format->Bloss) << format->Bshift
			   | format->Amask;
	}
	else
	{
		return SDL_FindColor(format->palette, r, g, b);
	}
}

/* Find the pixel value corresponding to an RGBA quadruple */
u32 SDL_MapRGBA(SDL_PixelFormat *format, u8 r, u8 g, u8 b, u8 a)
{
	if ( format->palette == NULL )
	{
		return (r >> format->Rloss) << format->Rshift
			| (g >> format->Gloss) << format->Gshift
			| (b >> format->Bloss) << format->Bshift
			| ((a >> format->Aloss) << format->Ashift & format->Amask);
	}
	else
	{
		return SDL_FindColor(format->palette, r, g, b);
	}
}

void SDL_GetRGBA(u32 pixel, SDL_PixelFormat *fmt,
		 u8 *r, u8 *g, u8 *b, u8 *a)
{
	if ( fmt->palette == NULL )
	{
		/*
		 * This makes sure that the result is mapped to the
		 * interval [0..255], and the maximum value for each
		 * component is 255. This is important to make sure
		 * that white is indeed reported as (255, 255, 255),
		 * and that opaque alpha is 255.
		 * This only works for RGB bit fields at least 4 bit
		 * wide, which is almost always the case.
		 */
		unsigned v;
		v = (pixel & fmt->Rmask) >> fmt->Rshift;		*r = (v << fmt->Rloss) + (v >> (8 - (fmt->Rloss << 1)));
		v = (pixel & fmt->Gmask) >> fmt->Gshift;		*g = (v << fmt->Gloss) + (v >> (8 - (fmt->Gloss << 1)));
		v = (pixel & fmt->Bmask) >> fmt->Bshift;		*b = (v << fmt->Bloss) + (v >> (8 - (fmt->Bloss << 1)));
		if (fmt->Amask)
		{
			v = (pixel & fmt->Amask) >> fmt->Ashift;	*a = (v << fmt->Aloss) + (v >> (8 - (fmt->Aloss << 1)));
		}
		else
		{
			*a = SDL_ALPHA_OPAQUE;
		}
	}
	else
	{
		*r = fmt->palette->colors[pixel].r;
		*g = fmt->palette->colors[pixel].g;
		*b = fmt->palette->colors[pixel].b;
		*a = SDL_ALPHA_OPAQUE;
	}
}

void SDL_GetRGB(u32 pixel, SDL_PixelFormat *fmt, u8 *r, u8 *g, u8 *b)
{
	if ( fmt->palette == NULL )
	{
		/* the note for SDL_GetRGBA above applies here too */
		unsigned v;
		v = (pixel & fmt->Rmask) >> fmt->Rshift;	*r = (v << fmt->Rloss) + (v >> (8 - (fmt->Rloss << 1)));
		v = (pixel & fmt->Gmask) >> fmt->Gshift;	*g = (v << fmt->Gloss) + (v >> (8 - (fmt->Gloss << 1)));
		v = (pixel & fmt->Bmask) >> fmt->Bshift;	*b = (v << fmt->Bloss) + (v >> (8 - (fmt->Bloss << 1)));
	}
	else
	{
		*r = fmt->palette->colors[pixel].r;
		*g = fmt->palette->colors[pixel].g;
		*b = fmt->palette->colors[pixel].b;
	}
}

/* Apply gamma to a set of colors - this is easy. :) */
void SDL_ApplyGamma(
	u16 *gamma, SDL_Color *colors, SDL_Color *output,
	int ncolors)
{
	int i;
	for (i=0; i<ncolors; i++)
	{
		output[i].r = gamma[0*256 + colors[i].r] >> 8;
		output[i].g = gamma[1*256 + colors[i].g] >> 8;
		output[i].b = gamma[2*256 + colors[i].b] >> 8;
	}
}

/* Map from Palette to Palette */
static u8 *Map1to1(SDL_Palette *src, SDL_Palette *dst, int *identical)
{
	u8 *map;
	if (identical)
	{
		if ( src->ncolors <= dst->ncolors )
		{
			/* If an identical palette, no need to map */
			if (0 == memcmp(src->colors, dst->colors, (src->ncolors * sizeof(SDL_Color))))
			{
				*identical = 1;
				return (NULL);
			}
		}
		*identical = 0;
	}
//	map = (u8 *)mal loc((src->ncolors));
	map = (u8 *)memalign(16, (src->ncolors));
	if ( map == NULL )
	{
		SDL_OutOfMemory_bbb();
		return (NULL);
	}
	{
		int i;
		for (i=0; i<src->ncolors; i++)
		{
			map[i] = SDL_FindColor(dst,
				src->colors[i].r, src->colors[i].g, src->colors[i].b);
		}
	}
	return (map);
}
/* Map from Palette to BitField */
static u8 *Map1toN(SDL_Palette *src, SDL_PixelFormat *dst)
{
	u8 *map;
	int  bpp;
	unsigned alpha;

	bpp = ((dst->BytesPerPixel == 3) ? 4 : dst->BytesPerPixel);
//	map = (u8 *)mal loc((src->ncolors*bpp));
	map = (u8 *)memalign(16, (src->ncolors*bpp));
	if ( map == NULL )
	{
		SDL_OutOfMemory_bbb();
		return (NULL);
	}
	alpha = dst->Amask ? SDL_ALPHA_OPAQUE : 0;
	/* We memory copy to the pixel map so the endianness is preserved */
	{
		int i;
		for (i=0; i<src->ncolors; i++)
		{
			ASSEMBLE_RGBA(&map[i*bpp],
				dst->BytesPerPixel, dst,
				src->colors[i].r,
				src->colors[i].g,
				src->colors[i].b,
				alpha);
		}
	}
	return (map);
}
/* Map from BitField to Dithered-Palette to Palette */
static u8 *MapNto1(SDL_PixelFormat *src, SDL_Palette *dst, int *identical)
{
	/* Generate a 256 color dither palette */
	SDL_Palette dithered;
	SDL_Color colors[256];

	/* SDL_DitherColors does not initialize the 'unused' component of colors,
	   but Map1to1 compares it against dst, so we should initialize it. */
	memset(colors, 0, sizeof(colors) );

	dithered.ncolors = 256;
	SDL_DitherColors(colors, 8);
	dithered.colors = colors;
	return (Map1to1(&dithered, dst, identical));
}

SDL_BlitMap *SDL_AllocBlitMap(void)
{
	SDL_BlitMap *map;
	/* Allocate the empty map */
//	map = (SDL_BlitMap *)mal loc((sizeof(*map)));
	map = (SDL_BlitMap *)memalign(16, (sizeof(*map)));
	if ( map == NULL )
	{
		SDL_OutOfMemory_bbb();
		return (NULL);
	}
	memset(map, 0, sizeof(*map));

	/* Allocate the software blit data */
//	map->sw_data = (struct private_swaccel *)mal loc((sizeof(*map->sw_data)));
	map->sw_data = (struct private_swaccel *)memalign(16, (sizeof(*map->sw_data)));
	if ( map->sw_data == NULL )
	{
		SDL_FreeBlitMap(map);
		SDL_OutOfMemory_bbb();
		return (NULL);
	}
	memset(map->sw_data, 0, sizeof(*map->sw_data));

	/* It's ready to go */
	return (map);
}
void SDL_InvalidateMap(SDL_BlitMap *map)
{
	if ( ! map )
	{
		return;
	}
	map->dst = NULL;
	map->format_version = (unsigned int)-1;
	if ( map->table )
	{
		free(map->table);
		map->table = NULL;
	}
}
int SDL_MapSurface (SDL_Surface *src, SDL_Surface *dst)
{
	SDL_PixelFormat *srcfmt;
	SDL_PixelFormat *dstfmt;
	SDL_BlitMap *map;

	/* Clear out any previous mapping */
	map = src->map;
	#if (1==USE_RLEACCEL)
	if ( (src->flags & SDL_RLEACCEL) == SDL_RLEACCEL )
	{
		SDL_UnRLESurface(src, 1);
	}
	#endif /*(USE_RLEACCEL)*/
	SDL_InvalidateMap(map);

	/* Figure out what kind of mapping we're doing */
	map->identity = 0;
	srcfmt = src->format;
	dstfmt = dst->format;
	switch (srcfmt->BytesPerPixel)
	{
	case 1:
		switch (dstfmt->BytesPerPixel)
		{
		case 1:
			/* Palette --> Palette */
			/* If both SDL_HWSURFACE, assume have same palette */
			if ( ((src->flags & SDL_HWSURFACE) == SDL_HWSURFACE) &&
				 ((dst->flags & SDL_HWSURFACE) == SDL_HWSURFACE) )
			{
				map->identity = 1;
			}
			else
			{
				map->table = Map1to1(srcfmt->palette,
					dstfmt->palette, &map->identity);
			}
			if ( ! map->identity )
			{
				if ( map->table == NULL )
				{
					return (-1);
				}
			}
			if (srcfmt->BitsPerPixel!=dstfmt->BitsPerPixel)
			{	map->identity = 0;
			}
			break;
		default:
			/* Palette --> BitField */
			map->table = Map1toN(srcfmt->palette, dstfmt);
			if ( map->table == NULL )
			{
				return (-1);
			}
			break;
		}
		break;
	default:
		switch (dstfmt->BytesPerPixel)
		{
		case 1:
			/* BitField --> Palette */
			map->table = MapNto1(srcfmt,
					dstfmt->palette, &map->identity);
			if ( ! map->identity )
			{
				if ( map->table == NULL )
				{
					return (-1);
				}
			}
			map->identity = 0;			/* Don't optimize to copy */
			break;
		default:
			/* BitField --> BitField */
			if ( FORMAT_EQUAL(srcfmt, dstfmt) )
			{	map->identity = 1;	}
			break;
		}
		break;
	}
	map->dst = dst;
	map->format_version = dst->format_version;

	/* Choose your blitters wisely */
	return (SDL_CalculateBlit(src));
}
void SDL_FreeBlitMap(SDL_BlitMap *map)
{
	if ( map )
	{
		SDL_InvalidateMap(map);
		if ( map->sw_data != NULL )
		{
			free(map->sw_data);
		}
		free(map);
	}
}
