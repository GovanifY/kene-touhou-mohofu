/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2FSDL_blit_1.c

psp - リビジョン 440Subversion リポジトリ一覧: pspps2ps2wareps3ps3warepspwareリビジョン:(root)/trunk/SDL/src/video/SDL_blit_1.c
詳細表示 - 最終更新日時 - ログを見る -
*/

#define USE_MY_DIV (1)
#define USE_MY_DIV_LOOP (1)
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_blit_1.c,v 1.6 2004/01/04 16:49:21 slouken Exp $";
#endif

#include <stdio.h>

//#include "PSPL_types.h"	/* 取り敢えず(仮) */
#include "./../include/PSPL_video.h"	/* 取り敢えず(仮) */
#include "PSPL_blit.h"
#include "PSPL_sysvideo.h"
//#include "./../include/PSPL_endian.h"

/* Functions to blit from 8-bit surfaces to other surfaces */

static void Blit1to1(SDL_BlitInfo *info)
{
	int width, height;
	u8 *src, *map, *dst;
	int srcskip, dstskip;
	/* Set up some basic variables */
	width		= info->d_width;
	height		= info->d_height;
	src 		= info->s_pixels;
	srcskip 	= info->s_skip;
	dst 		= info->d_pixels;
	dstskip 	= info->d_skip;
	map 		= info->table;
	while ( height-- )
	{
		#ifdef USE_DUFFS_LOOP
		DUFFS_LOOP(
			{
			  *dst = map[*src];
			}
			dst++;
			src++;
		, width);
		#else
		{
			int c;
			for (c=width; c; c--)
			{
				*dst = map[*src];
				dst++;
				src++;
			}
		}
		#endif
		src += srcskip;
		dst += dstskip;
	}
}
/* This is now endian dependent */
#if ( SDL_BYTEORDER == SDL_LIL_ENDIAN )
	#define HI		1
	#define LO		0
#else /* ( SDL_BYTEORDER == SDL_BIG_ENDIAN ) */
	#define HI		0
	#define LO		1
#endif
static void Blit1to2(SDL_BlitInfo *info)
{
	#ifndef USE_DUFFS_LOOP
	int c;
	#endif
	int width, height;
	u8 *src, *dst;
	u16 *map;
	int srcskip, dstskip;
	/* Set up some basic variables */
	width		= info->d_width;
	height		= info->d_height;
	src 		= info->s_pixels;
	srcskip 	= info->s_skip;
	dst 		= info->d_pixels;
	dstskip 	= info->d_skip;
	map 		= (u16 *)info->table;
	#ifdef USE_DUFFS_LOOP
	while ( height-- )
	{
		DUFFS_LOOP(
		{
			*(u16 *)dst = map[*src++];
			dst += 2;
		},
		width);
		src += srcskip;
		dst += dstskip;
	}
	#else
	/* Memory align at 4-byte boundary, if necessary */
	if ( (long)dst & 0x03 )
	{
		/* Don't do anything if width is 0 */
		if ( width == 0 )
		{
			return;
		}
		--width;

		while ( height-- )
		{
			/* Perform copy alignment */
			*(u16 *)dst = map[*src++];
			dst += 2;

			/* Copy in 4 pixel chunks */
			#if (1==USE_MY_DIV_LOOP)
			for (c=(width>>2); c; c--)/*(widthは実は必ずunsigned。)*/
			#else/*(original)*/
			for (c=(width/4); c; c--)
			#endif
			{
				*(u32 *)dst =
					(map[src[HI]]<<16)|(map[src[LO]]);
				src += 2;
				dst += 4;
				*(u32 *)dst =
					(map[src[HI]]<<16)|(map[src[LO]]);
				src += 2;
				dst += 4;
			}
			/* Get any leftovers */
			switch (width & 3)
			{
			case 3:
				*(u16 *)dst = map[*src++];
				dst += 2;
			case 2:
				*(u32 *)dst =
				  (map[src[HI]]<<16)|(map[src[LO]]);
				src += 2;
				dst += 4;
				break;
			case 1:
				*(u16 *)dst = map[*src++];
				dst += 2;
				break;
			}
			src += srcskip;
			dst += dstskip;
		}
	}
	else
	{
		while ( height-- )
		{
			/* Copy in 4 pixel chunks */
			#if (1==USE_MY_DIV_LOOP)
			for (c=(width>>2); c; c--)/*(widthは実は必ずunsigned。)*/
			#else/*(original)*/
			for (c=(width/4); c; c--)
			#endif
			{
				*(u32 *)dst =
					(map[src[HI]]<<16)|(map[src[LO]]);
				src += 2;
				dst += 4;
				*(u32 *)dst =
					(map[src[HI]]<<16)|(map[src[LO]]);
				src += 2;
				dst += 4;
			}
			/* Get any leftovers */
			switch (width & 3)
			{
				case 3:
					*(u16 *)dst = map[*src++];
					dst += 2;
				case 2:
					*(u32 *)dst =
					  (map[src[HI]]<<16)|(map[src[LO]]);
					src += 2;
					dst += 4;
					break;
				case 1:
					*(u16 *)dst = map[*src++];
					dst += 2;
					break;
			}
			src += srcskip;
			dst += dstskip;
		}
	}
	#endif /* USE_DUFFS_LOOP */
}
static void Blit1to3(SDL_BlitInfo *info)
{
	int o;
	int width, height;
	u8 *src, *map, *dst;
	int srcskip, dstskip;
	/* Set up some basic variables */
	width		= info->d_width;
	height		= info->d_height;
	src 		= info->s_pixels;
	srcskip 	= info->s_skip;
	dst 		= info->d_pixels;
	dstskip 	= info->d_skip;
	map 		= info->table;
	while ( height-- )
	{
		#ifdef USE_DUFFS_LOOP
		DUFFS_LOOP(
			{
				o = *src * 4;
				dst[0] = map[o++];
				dst[1] = map[o++];
				dst[2] = map[o++];
			}
			src++;
			dst += 3;
		, width);
		#else
		{
			int c;
			for (c=width; c; c--)
			{
				o = *src * 4;
				dst[0] = map[o++];
				dst[1] = map[o++];
				dst[2] = map[o++];
				src++;
				dst += 3;
			}
		}
		#endif /* USE_DUFFS_LOOP */
		src += srcskip;
		dst += dstskip;
	}
}
static void Blit1to4(SDL_BlitInfo *info)
{
	int width, height;
	u8 *src;
	u32 *map, *dst;
	int srcskip, dstskip;
	/* Set up some basic variables */
	width		= info->d_width;
	height		= info->d_height;
	src 		= info->s_pixels;
	srcskip 	= info->s_skip;
	dst 		= (u32 *)info->d_pixels;
	dstskip 	= info->d_skip/4;
	map 		= (u32 *)info->table;
	while ( height-- )
	{
		#ifdef USE_DUFFS_LOOP
		DUFFS_LOOP(
			*dst++ = map[*src++];
		, width);
		#else
		{
			int c;
			#if (1==USE_MY_DIV_LOOP)
			for (c=(width>>2); c; c--)/*(widthは実は必ずunsigned。)*/
			#else/*(original)*/
			for (c=(width/4); c; c--)
			#endif
			{
				*dst++ = map[*src++];
				*dst++ = map[*src++];
				*dst++ = map[*src++];
				*dst++ = map[*src++];
			}
		}
		switch ( width & 3 )
		{
		case 3: 		*dst++ = map[*src++];
		case 2: 		*dst++ = map[*src++];
		case 1: 		*dst++ = map[*src++];
		}
		#endif /* USE_DUFFS_LOOP */
		src += srcskip;
		dst += dstskip;
	}
}

static void Blit1to1Key(SDL_BlitInfo *info)
{
	int width		= info->d_width;
	int height		= info->d_height;
	u8 *src		= info->s_pixels;
	int srcskip 	= info->s_skip;
	u8 *dst		= info->d_pixels;
	int dstskip 	= info->d_skip;
	u8 *palmap	= info->table;
	u32 ckey 	= info->src->colorkey;
	if ( palmap )
	{
		while ( height-- )
		{
			DUFFS_LOOP(
			{
				if ( *src != ckey )
				{
					*dst = palmap[*src];
				}
				dst++;
				src++;
			},
			width);
			src += srcskip;
			dst += dstskip;
		}
	}
	else
	{
		while ( height-- )
		{
			DUFFS_LOOP(
			{
				if ( *src != ckey )
				{
					*dst = *src;
				}
				dst++;
				src++;
			},
			width);
			src += srcskip;
			dst += dstskip;
		}
	}
}

static void Blit1to2Key(SDL_BlitInfo *info)
{
	int width		= info->d_width;
	int height		= info->d_height;
	u8 *src		= info->s_pixels;
	int srcskip 	= info->s_skip;
	u16 *dstp	= (u16 *)info->d_pixels;
	int dstskip 	= info->d_skip;
	u16 *palmap	= (u16 *)info->table;
	u32 ckey 	= info->src->colorkey;
	/* Set up some basic variables */
	#if (1==USE_MY_DIV)
	dstskip >>= 1;	/*(widthは実は必ずunsigned。)*/
	#else/*(original)*/
	dstskip /= 2;
	#endif
	while ( height-- )
	{
		DUFFS_LOOP(
		{
			if ( *src != ckey )
			{
				*dstp=palmap[*src];
			}
			src++;
			dstp++;
		},
		width);
		src += srcskip;
		dstp += dstskip;
	}
}

static void Blit1to3Key(SDL_BlitInfo *info)
{
	int width		= info->d_width;
	int height		= info->d_height;
	u8 *src		= info->s_pixels;
	int srcskip 	= info->s_skip;
	u8 *dst		= info->d_pixels;
	int dstskip 	= info->d_skip;
	u8 *palmap	= info->table;
	u32 ckey 	= info->src->colorkey;
	int o;
	while ( height-- )
	{
		DUFFS_LOOP(
		{
			if ( *src != ckey )
			{
				o = *src * 4;
				dst[0] = palmap[o++];
				dst[1] = palmap[o++];
				dst[2] = palmap[o++];
			}
			src++;
			dst += 3;
		},
		width);
		src += srcskip;
		dst += dstskip;
	}
}

static void Blit1to4Key(SDL_BlitInfo *info)
{
	int width		= info->d_width;
	int height		= info->d_height;
	u8 *src		= info->s_pixels;
	int srcskip 	= info->s_skip;
	u32 *dstp	= (u32 *)info->d_pixels;
	int dstskip 	= info->d_skip;
	u32 *palmap	= (u32 *)info->table;
	u32 ckey 	= info->src->colorkey;
	/* Set up some basic variables */
	#if (1==USE_MY_DIV)
	dstskip >>= 2;	/*(widthは実は必ずunsigned。)*/
	#else/*(original)*/
	dstskip /= 4;
	#endif
	while ( height-- )
	{
		DUFFS_LOOP(
		{
			if ( *src != ckey )
			{
				*dstp = palmap[*src];
			}
			src++;
			dstp++;
		},
		width);
		src += srcskip;
		dstp += dstskip;
	}
}

static void Blit1toNAlpha(SDL_BlitInfo *info)
{
	int width		= info->d_width;
	int height		= info->d_height;
	u8 *src		= info->s_pixels;
	int srcskip 	= info->s_skip;
	u8 *dst		= info->d_pixels;
	int dstskip 	= info->d_skip;
	SDL_PixelFormat *dstfmt = info->dst;
	const SDL_Color *srcpal = info->src->palette->colors;
	int dstbpp;
	const int A 	= info->src->alpha;
	/* Set up some basic variables */
	dstbpp = dstfmt->BytesPerPixel;
	while ( height-- )
	{
		int sR, sG, sB;
		int dR, dG, dB;
		DUFFS_LOOP4(
			{
				u32 pixel;
				sR = srcpal[*src].r;
				sG = srcpal[*src].g;
				sB = srcpal[*src].b;
				DISEMBLE_RGB(dst, dstbpp, dstfmt,
						 pixel, dR, dG, dB);
				ALPHA_BLEND(sR, sG, sB, A, dR, dG, dB);
				ASSEMBLE_RGB(dst, dstbpp, dstfmt, dR, dG, dB);
				src++;
				dst += dstbpp;
			},
			width);
		src += srcskip;
		dst += dstskip;
	}
}

static void Blit1toNAlphaKey(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	u8 *src = info->s_pixels;
	int srcskip = info->s_skip;
	u8 *dst = info->d_pixels;
	int dstskip = info->d_skip;
	SDL_PixelFormat *srcfmt = info->src;
	SDL_PixelFormat *dstfmt = info->dst;
	const SDL_Color *srcpal = info->src->palette->colors;
	u32 ckey = srcfmt->colorkey;
	int dstbpp;
	const int A = srcfmt->alpha;

	/* Set up some basic variables */
	dstbpp = dstfmt->BytesPerPixel;

	while ( height-- )
	{
		int sR, sG, sB;
		int dR, dG, dB;
		DUFFS_LOOP(
		{
			if ( *src != ckey )
			{
				u32 pixel;
				sR = srcpal[*src].r;
				sG = srcpal[*src].g;
				sB = srcpal[*src].b;
				DISEMBLE_RGB(dst, dstbpp, dstfmt,
							pixel, dR, dG, dB);
				ALPHA_BLEND(sR, sG, sB, A, dR, dG, dB);
				ASSEMBLE_RGB(dst, dstbpp, dstfmt, dR, dG, dB);
			}
			src++;
			dst += dstbpp;
		},
		width);
		src += srcskip;
		dst += dstskip;
	}
}


SDL_loblit SDL_CalculateBlit1(SDL_Surface *surface, int blit_index)
{
	SDL_PixelFormat *dstfmt;
	dstfmt = surface->map->dst->format;
	if ( dstfmt->BitsPerPixel < 8 )
	{
		/* We don't support. */
		return (NULL);	//which = 0;
	}
//	else
//	{
	int which;
		which = dstfmt->BytesPerPixel;
//	}
	#if (1)/* don't need? */
	if (0==which)
	{
		return (NULL);/*safty?*/
	}
	#endif
	which--;
	switch (blit_index)
	{
	case 0:
		{
			static SDL_loblit one_blit[(4)] =
			{
			//	NULL,
				Blit1to1,
				Blit1to2,
				Blit1to3,
				Blit1to4
			};
			return (one_blit[which]);	 /* copy */
		}
	case 1:
		{
			static SDL_loblit one_blitkey[(4)] =
			{
			//	NULL,
				Blit1to1Key,
				Blit1to2Key,
				Blit1to3Key,
				Blit1to4Key
			};
			return (one_blitkey[which]);	 /* colorkey */
		}
		/* Supporting 8bpp->8bpp alpha is doable but requires lots of
		   tables which consume space and takes time to precompute,
		   so is better left to the user */
	case 2: 	return ((which >= (2-1)) ? (Blit1toNAlpha	) : (NULL) );	 /* alpha */
	case 3: 	return ((which >= (2-1)) ? (Blit1toNAlphaKey) : (NULL) );	 /* alpha + colorkey */
	}
	return (NULL);
}
