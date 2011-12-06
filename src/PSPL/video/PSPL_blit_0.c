/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2FSDL_blit_0.c

psp - リビジョン 441Subversion リポジトリ一覧: pspps2ps2wareps3ps3warepspwareリビジョン:(root)/trunk/SDL/src/video/SDL_blit_0.c
リビジョン 440 - 詳細表示 - 前のリビジョンとの差分 - 最終更新日時 - ログを見る -
*/

#define USE_MY_DIV (1)
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_blit_0.c,v 1.8 2005/05/16 05:55:17 slouken Exp $";
#endif

#include <stdio.h>
#include <string.h>

//#include "SDL_types.h"	/* 取り敢えず(仮) */
#include "./../include/PSPL_video.h"	/* 取り敢えず(仮) */
#include "PSPL_blit.h"

/* Functions to blit from bitmaps to other surfaces */

static void BlitBto1(SDL_BlitInfo *info)
{
	int c;
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
	#if (1==USE_MY_DIV)
	srcskip += width - ((width+7)>>3);	/*(widthは実は必ずunsigned。)*/
	#else/*(original)*/
	srcskip 	+= width-(width+7)/8;
	#endif
	if ( map )
	{
		while ( height-- )
		{
			u8 byte = 0, bit;
			for (c=0; c<width; c++)
			{
				if ( (c&7) == 0 )
				{
					byte = *src++;
				}
				bit = (byte&0x80)>>7;
				if ( 1 )
				{
					*dst = map[bit];
				}
				dst++;
				byte <<= 1;
			}
			src += srcskip;
			dst += dstskip;
		}
	}
	else
	{
		while ( height-- )
		{
			u8 byte = 0, bit;
			for (c=0; c<width; c++)
			{
				if ( (c&7) == 0 )
				{
					byte = *src++;
				}
				bit = (byte&0x80)>>7;
				if ( 1 )
				{
					*dst = bit;
				}
				dst++;
				byte <<= 1;
			}
			src += srcskip;
			dst += dstskip;
		}
	}
}
static void BlitBto2(SDL_BlitInfo *info)
{
	int c;
	int width, height;
	u8 *src;
	u16 *map, *dst;
	int srcskip, dstskip;

	/* Set up some basic variables */
	width		= info->d_width;
	height		= info->d_height;
	src 		= info->s_pixels;
	srcskip 	= info->s_skip;
	dst 		= (u16 *)info->d_pixels;
	dstskip 	= info->d_skip/2;
	map 		= (u16 *)info->table;
	#if (1==USE_MY_DIV)
	srcskip += width - ((width+7)>>3);	/*(widthは実は必ずunsigned。)*/
	#else/*(original)*/
	srcskip 	+= width-(width+7)/8;
	#endif
	while ( height-- )
	{
		u8 byte = 0, bit;
		for (c=0; c<width; c++)
		{
			if ( (c&7) == 0 )
			{
				byte = *src++;
			}
			bit = (byte&0x80)>>7;
			if ( 1 )
			{
				*dst = map[bit];
			}
			byte <<= 1;
			dst++;
		}
		src += srcskip;
		dst += dstskip;
	}
}
static void BlitBto3(SDL_BlitInfo *info)
{
	int c, o;
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
	#if (1==USE_MY_DIV)
	srcskip += width - ((width+7)>>3);	/*(widthは実は必ずunsigned。)*/
	#else/*(original)*/
	srcskip 	+= width-(width+7)/8;
	#endif
	while ( height-- )
	{
		u8 byte = 0, bit;
		for (c=0; c<width; c++)
		{
			if ( (c&7) == 0 )
			{
				byte = *src++;
			}
			bit = (byte&0x80)>>7;
			if ( 1 )
			{
				o = bit * 4;
				dst[0] = map[o++];
				dst[1] = map[o++];
				dst[2] = map[o++];
			}
			byte <<= 1;
			dst += 3;
		}
		src += srcskip;
		dst += dstskip;
	}
}
static void BlitBto4(SDL_BlitInfo *info)
{
	int width, height;
	u8 *src;
	u32 *map, *dst;
	int srcskip, dstskip;
	int c;

	/* Set up some basic variables */
	width		= info->d_width;
	height		= info->d_height;
	src 		= info->s_pixels;
	srcskip 	= info->s_skip;
	dst 		= (u32 *)info->d_pixels;
	dstskip 	= info->d_skip/4;
	map 		= (u32 *)info->table;
	#if (1==USE_MY_DIV)
	srcskip += width - ((width+7)>>3);	/*(widthは実は必ずunsigned。)*/
	#else/*(original)*/
	srcskip 	+= width-(width+7)/8;
	#endif
	while ( height-- )
	{
		u8 byte = 0, bit;
		for (c=0; c<width; c++)
		{
			if ( (c&7) == 0 )
			{
				byte = *src++;
			}
			bit = (byte&0x80)>>7;
			if ( 1 )
			{
				*dst = map[bit];
			}
			byte <<= 1;
			dst++;
		}
		src += srcskip;
		dst += dstskip;
	}
}

static void BlitBto1Key(SDL_BlitInfo *info)
{
	int width		= info->d_width;
	int height		= info->d_height;
	u8 *src 		= info->s_pixels;
	u8 *dst 		= info->d_pixels;
	int srcskip 	= info->s_skip;
	int dstskip 	= info->d_skip;
	u32 ckey		= info->src->colorkey;
	u8 *palmap		= info->table;
	int c;
	/* Set up some basic variables */
	#if (1==USE_MY_DIV)
	srcskip += width - ((width+7)>>3);	/*(widthは実は必ずunsigned。)*/
	#else/*(original)*/
	srcskip 	+= width-(width+7)/8;
	#endif
	if ( palmap )
	{
		while ( height-- )
		{
			u8	byte = 0, bit;
			for (c=0; c<width; c++)
			{
				if ( (c&7) == 0 )
				{
					byte = *src++;
				}
				bit = (byte&0x80)>>7;
				if ( bit != ckey )
				{
					*dst = palmap[bit];
				}
				dst++;
				byte <<= 1;
			}
			src += srcskip;
			dst += dstskip;
		}
	}
	else
	{
		while ( height-- )
		{
			u8	byte = 0, bit;
			for (c=0; c<width; c++)
			{
				if ( (c&7) == 0 )
				{
					byte = *src++;
				}
				bit = (byte&0x80)>>7;
				if ( bit != ckey )
				{
					*dst = bit;
				}
				dst++;
				byte <<= 1;
			}
			src += srcskip;
			dst += dstskip;
		}
	}
}

static void BlitBto2Key(SDL_BlitInfo *info)
{
	int width		= info->d_width;
	int height		= info->d_height;
	u8 *src 		= info->s_pixels;
	u16 *dstp		= (u16 *)info->d_pixels;
	int srcskip 	= info->s_skip;
	int dstskip 	= info->d_skip;
	u32 ckey		= info->src->colorkey;
	u8 *palmap		= info->table;
	int c;
	/* Set up some basic variables */
	#if (1==USE_MY_DIV)
	srcskip += width - ((width+7)>>3);	/*(widthは実は必ずunsigned。)*/
	#else/*(original)*/
	srcskip 	+= width-(width+7)/8;
	#endif
	#if (1==USE_MY_DIV)
	dstskip >>= 1;	/*(widthは実は必ずunsigned。)*/
	#else/*(original)*/
	dstskip /= 2;
	#endif
	while ( height-- )
	{
		u8 byte = 0, bit;
		for (c=0; c<width; c++)
		{
			if ( (c&7) == 0 )
			{
				byte = *src++;
			}
			bit = (byte&0x80)>>7;
			if ( bit != ckey )
			{
				*dstp=((u16 *)palmap)[bit];
			}
			byte <<= 1;
			dstp++;
		}
		src += srcskip;
		dstp += dstskip;
	}
}

static void BlitBto3Key(SDL_BlitInfo *info)
{
	int width		= info->d_width;
	int height		= info->d_height;
	u8 *src 		= info->s_pixels;
	u8 *dst 		= info->d_pixels;
	int srcskip 	= info->s_skip;
	int dstskip 	= info->d_skip;
	u32 ckey		= info->src->colorkey;
	u8 *palmap		= info->table;
	int c;
	/* Set up some basic variables */
	#if (1==USE_MY_DIV)
	srcskip += width - ((width+7)>>3);	/*(widthは実は必ずunsigned。)*/
	#else/*(original)*/
	srcskip 	+= width-(width+7)/8;
	#endif
	while ( height-- )
	{
		u8	byte = 0, bit;
		for (c=0; c<width; c++)
		{
			if ( (c&7) == 0 )
			{
				byte = *src++;
			}
			bit = (byte&0x80)>>7;
			if ( bit != ckey )
			{
				memcpy(dst, &palmap[bit*4], 3);
			}
			byte <<= 1;
			dst += 3;
		}
		src += srcskip;
		dst += dstskip;
	}
}

static void BlitBto4Key(SDL_BlitInfo *info)
{
	int width		= info->d_width;
	int height		= info->d_height;
	u8 *src 		= info->s_pixels;
	u32 *dstp		= (u32 *)info->d_pixels;
	int srcskip 	= info->s_skip;
	int dstskip 	= info->d_skip;
	u32 ckey		= info->src->colorkey;
	u8 *palmap		= info->table;
	int c;
	/* Set up some basic variables */
	#if (1==USE_MY_DIV)
	srcskip += width - ((width+7)>>3);	/*(widthは実は必ずunsigned。)*/
	#else/*(original)*/
	srcskip 	+= width-(width+7)/8;
	#endif
	#if (1==USE_MY_DIV)
	dstskip >>= 2;	/*(widthは実は必ずunsigned。)*/
	#else/*(original)*/
	dstskip /= 4;
	#endif
	while ( height-- )
	{
		u8 byte = 0, bit;
		for (c=0; c<width; c++)
		{
			if ( (c&7) == 0 )
			{
				byte = *src++;
			}
			bit = (byte&0x80)>>7;
			if ( bit != ckey )
			{
				*dstp=((u32 *)palmap)[bit];
			}
			byte <<= 1;
			dstp++;
		}
		src += srcskip;
		dstp += dstskip;
	}
}

static void BlitBtoNAlpha(SDL_BlitInfo *info)
{
	int width		= info->d_width;
	int height		= info->d_height;
	u8 *src 		= info->s_pixels;
	u8 *dst 		= info->d_pixels;
	int srcskip 	= info->s_skip;
	int dstskip 	= info->d_skip;
	const SDL_Color *srcpal = info->src->palette->colors;
	SDL_PixelFormat *dstfmt = info->dst;
	int  dstbpp;
	int c;
	const int A 	= info->src->alpha;
	/* Set up some basic variables */
	dstbpp			= dstfmt->BytesPerPixel;
	#if (1==USE_MY_DIV)
	srcskip += width - ((width+7)>>3);	/*(widthは実は必ずunsigned。)*/
	#else/*(original)*/
	srcskip 	+= width-(width+7)/8;
	#endif
	while ( height-- )
	{
		u8 byte = 0, bit;
		for (c=0; c<width; c++)
		{
			if ( (c&7) == 0 )
			{
				byte = *src++;
			}
			bit = (byte&0x80)>>7;
			if ( 1 )
			{
				u32 pixel;
				unsigned sR, sG, sB;
				unsigned dR, dG, dB;
				sR = srcpal[bit].r;
				sG = srcpal[bit].g;
				sB = srcpal[bit].b;
				DISEMBLE_RGB(dst, dstbpp, dstfmt, pixel, dR, dG, dB);
				ALPHA_BLEND(sR, sG, sB, A, dR, dG, dB);
				ASSEMBLE_RGB(dst, dstbpp, dstfmt, dR, dG, dB);
			}
			byte <<= 1;
			dst += dstbpp;
		}
		src += srcskip;
		dst += dstskip;
	}
}

static void BlitBtoNAlphaKey(SDL_BlitInfo *info)
{
	int width		= info->d_width;
	int height		= info->d_height;
	u8 *src 		= info->s_pixels;
	u8 *dst 		= info->d_pixels;
	int srcskip 	= info->s_skip;
	int dstskip 	= info->d_skip;
	SDL_PixelFormat *srcfmt = info->src;
	SDL_PixelFormat *dstfmt = info->dst;
	const SDL_Color *srcpal = srcfmt->palette->colors;
	int dstbpp;
	int c;
	const int A 	= srcfmt->alpha;
	u32 ckey		= srcfmt->colorkey;
	/* Set up some basic variables */
	dstbpp			= dstfmt->BytesPerPixel;
	#if (1==USE_MY_DIV)
	srcskip += width - ((width+7)>>3);	/*(widthは実は必ずunsigned。)*/
	#else/*(original)*/
	srcskip 	+= width-(width+7)/8;
	#endif
	while ( height-- )
	{
		u8	byte = 0, bit;
		for (c=0; c<width; c++)
		{
			if ( (c&7) == 0 )
			{
				byte = *src++;
			}
			bit = (byte&0x80)>>7;
			if ( bit != ckey )
			{
				int sR, sG, sB;
				int dR, dG, dB;
				u32 pixel;
				sR = srcpal[bit].r;
				sG = srcpal[bit].g;
				sB = srcpal[bit].b;
				DISEMBLE_RGB(dst, dstbpp, dstfmt, pixel, dR, dG, dB);
				ALPHA_BLEND(sR, sG, sB, A, dR, dG, dB);
				ASSEMBLE_RGB(dst, dstbpp, dstfmt, dR, dG, dB);
			}
			byte <<= 1;
			dst += dstbpp;
		}
		src += srcskip;
		dst += dstskip;
	}
}
#if (0)
static SDL_loblit bitmap_blit[] =
{
	NULL,
	BlitBto1,
	BlitBto2,
	BlitBto3,
	BlitBto4
};
static SDL_loblit colorkey_blit[] =
{
	NULL,
	BlitBto1Key,
	BlitBto2Key,
	BlitBto3Key,
	BlitBto4Key
};
SDL_loblit SDL_CalculateBlit0(SDL_Surface *surface, int blit_index)
{
	int which;
	if ( surface->format->BitsPerPixel != 1 )
	{
		/* We don't support sub 8-bit packed pixel modes */
		return (NULL);
	}
	if ( surface->map->dst->format->BitsPerPixel < 8 )
	{
		which = 0;
	}
	else
	{
		which = surface->map->dst->format->BytesPerPixel;
	}
	switch(blit_index)
	{
	case 0: return bitmap_blit[which];							 /* copy */
	case 1: return colorkey_blit[which];						 /* colorkey */
	case 2: return which >= 2 ? BlitBtoNAlpha : NULL;			 /* alpha */
	case 4: return which >= 2 ? BlitBtoNAlphaKey : NULL;		 /* alpha + colorkey */
	}
	return (NULL);
}
#else
SDL_loblit SDL_CalculateBlit0(SDL_Surface *surface, int blit_index)
{
	if ( surface->format->BitsPerPixel != 1 )
	{
		/* We don't support sub 8-bit packed pixel modes */
		return (NULL);
	}
	if ( surface->map->dst->format->BitsPerPixel < 8 )
	{
		/* We don't support. */
		return (NULL);	//which = 0;
	}
//	else
//	{
	int which;
		which = surface->map->dst->format->BytesPerPixel;
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
			static SDL_loblit bitmap_blit[(4)] =
			{
			//	NULL,
				BlitBto1,
				BlitBto2,
				BlitBto3,
				BlitBto4
			};
			return (bitmap_blit[which]);							 /* copy */
		}
	case 1:
		{
			static SDL_loblit colorkey_blit[(4)] =
			{
			//	NULL,
				BlitBto1Key,
				BlitBto2Key,
				BlitBto3Key,
				BlitBto4Key
			};
				return (colorkey_blit[which]);							 /* colorkey */
		}
	case 2: 	return ((which >= (2-1) ) ? (BlitBtoNAlpha	 ) : (NULL));	 /* alpha */
	case 4: 	return ((which >= (2-1) ) ? (BlitBtoNAlphaKey) : (NULL));	 /* alpha + colorkey */
	}
	return (NULL);
}
#endif
