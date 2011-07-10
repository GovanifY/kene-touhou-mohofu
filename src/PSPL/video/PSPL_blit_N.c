/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2FSDL_blit_N.c


psp - リビジョン 441Subversion リポジトリ一覧: pspps2ps2wareps3ps3warepspwareリビジョン:(root)/trunk/SDL/src/video/SDL_blit_N.c
リビジョン 440 - 詳細表示 - 前のリビジョンとの差分 - 最終更新日時 - ログを見る -
*/
#define USE_MY_DIV (1)
//1575915 1576139
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_blit_N.c,v 1.11 2005/04/20 05:57:39 icculus Exp $";
#endif

#include <stdio.h>

//#include "PSPL_types.h"	/* 取り敢えず(仮) */
#include "./../include/PSPL_video.h"	/* 取り敢えず(仮) */
#include "PSPL_blit.h"
//#include "PSPL_by teorder.h"
//#include "PSPL_cp uinfo.h"

/* Functions to blit from N-bit surfaces to other surfaces */

/* This is now endian dependent */
#if ( SDL_BYTEORDER == SDL_LIL_ENDIAN )
#define HI	1
#define LO	0
#else /* ( SDL_BYTEORDER == SDL_BIG_ENDIAN ) */
#define HI	0
#define LO	1
#endif

#ifdef USE_ASMBLIT

/* Heheheh, we coerce Hermes into using SDL blit information */
#define X86_ASSEMBLER
#define HermesConverterInterface	SDL_BlitInfo
#define HermesClearInterface		void
#define STACKCALL

#include "HeadMMX.h"
#include "HeadX86.h"

#else

/* Special optimized blit for RGB 8-8-8 --> RGB 3-3-2 */
#define RGB888_RGB332(dst, src) \
{ \
	dst = (((src)&0x00E00000)>>16)| \
		  (((src)&0x0000E000)>>11)| \
		  (((src)&0x000000C0)>>6); \
}
static void Blit_RGB888_index8(SDL_BlitInfo *info)
{
	#ifndef USE_DUFFS_LOOP
	int c;
	#endif
	int width, height;
	u32 *src;
	const u8 *map;
	u8 *dst;
	int srcskip, dstskip;

	/* Set up some basic variables */
	width		= info->d_width;
	height		= info->d_height;
	src 		= (u32 *)info->s_pixels;
	srcskip 	= info->s_skip/4;
	dst 		= info->d_pixels;
	dstskip 	= info->d_skip;
	map 		= info->table;

	if ( map == NULL )
	{
		while ( height-- )
		{
			#ifdef USE_DUFFS_LOOP
			DUFFS_LOOP(
				RGB888_RGB332(*dst++, *src);
				, width);
			#else
			/* translate aligned. */
			for (c=(width/4); c; --c )
			{
				/* Pack RGB into 8bit pixel */
				++src;	RGB888_RGB332(*dst++, *src);
				++src;	RGB888_RGB332(*dst++, *src);
				++src;	RGB888_RGB332(*dst++, *src);
				++src;	/*(none??)*/
			}
			/* translate aligned remain. */
			switch ( width & 3 )
			{
			case 3: 	RGB888_RGB332(*dst++, *src);		++src;	/*not_break;*/
			case 2: 	RGB888_RGB332(*dst++, *src);		++src;	/*not_break;*/
			case 1: 	RGB888_RGB332(*dst++, *src);		++src;	/*not_break;*/
			}
			#endif /* USE_DUFFS_LOOP */
			src += srcskip;
			dst += dstskip;
		}
	}
	else
	{
		int pixel;
		while ( height-- )
		{
			#ifdef USE_DUFFS_LOOP
			DUFFS_LOOP(
				RGB888_RGB332(pixel, *src);
				*dst++ = map[pixel];
				++src;
				, width);
			#else
			for (c=(width/4); c; --c )
			{
				/* Pack RGB into 8bit pixel */
				RGB888_RGB332(pixel, *src); 	*dst++ = map[pixel];		++src;
				RGB888_RGB332(pixel, *src); 	*dst++ = map[pixel];		++src;
				RGB888_RGB332(pixel, *src); 	*dst++ = map[pixel];		++src;
				RGB888_RGB332(pixel, *src); 	*dst++ = map[pixel];		++src;
			}
			switch ( width & 3 )
			{
			case 3: 	RGB888_RGB332(pixel, *src); 	*dst++ = map[pixel];		++src;	/*not_break;*/
			case 2: 	RGB888_RGB332(pixel, *src); 	*dst++ = map[pixel];		++src;	/*not_break;*/
			case 1: 	RGB888_RGB332(pixel, *src); 	*dst++ = map[pixel];		++src;	/*not_break;*/
			}
			#endif /* USE_DUFFS_LOOP */
			src += srcskip;
			dst += dstskip;
		}
	}
}







#endif /* USE_ASMBLIT */





/* Special optimized blit for RGB 8-8-8 --> RGB 3-3-2 */
#ifndef RGB888_RGB332
#define RGB888_RGB332(dst, src) 	{ \
	dst = (((src)&0x00E00000)>>16)| \
		  (((src)&0x0000E000)>>11)| \
		  (((src)&0x000000C0)>>6); \
}
#endif
static void Blit_RGB888_index8_map(SDL_BlitInfo *info)
{
#ifndef USE_DUFFS_LOOP
	int c;
#endif
	int pixel;
	int width, height;
	u32 *src;
	const u8 *map;
	u8 *dst;
	int srcskip, dstskip;

	/* Set up some basic variables */
	width		= info->d_width;
	height		= info->d_height;
	src 		= (u32 *)info->s_pixels;
	srcskip 	= info->s_skip/4;
	dst 		= info->d_pixels;
	dstskip 	= info->d_skip;
	map 		= info->table;

#ifdef USE_DUFFS_LOOP
	while ( height-- )
	{
		DUFFS_LOOP(
			RGB888_RGB332(pixel, *src);
			*dst++ = map[pixel];
			++src;
		, width);
		src += srcskip;
		dst += dstskip;
	}
#else
	while ( height-- )
	{
		for ( c=width/4; c; --c )
		{
			/* Pack RGB into 8bit pixel */
			RGB888_RGB332(pixel, *src); 		*dst++ = map[pixel];			++src;
			RGB888_RGB332(pixel, *src); 		*dst++ = map[pixel];			++src;
			RGB888_RGB332(pixel, *src); 		*dst++ = map[pixel];			++src;
			RGB888_RGB332(pixel, *src); 		*dst++ = map[pixel];			++src;
		}
		switch ( width & 3 )
		{
		case 3: 			RGB888_RGB332(pixel, *src); 			*dst++ = map[pixel];				++src;
		case 2: 			RGB888_RGB332(pixel, *src); 			*dst++ = map[pixel];				++src;
		case 1: 			RGB888_RGB332(pixel, *src); 			*dst++ = map[pixel];				++src;
		}
		src += srcskip;
		dst += dstskip;
	}
#endif /* USE_DUFFS_LOOP */
}
static void BlitNto1(SDL_BlitInfo *info)
{
	#ifndef USE_DUFFS_LOOP
	int c;
	#endif
	int width, height;
	u8 *src;
	const u8 *map;
	u8 *dst;
	int srcskip, dstskip;
	int srcbpp;
	u32 pixel;
	int  sR, sG, sB;
	SDL_PixelFormat *srcfmt;

	/* Set up some basic variables */
	width		= info->d_width;
	height		= info->d_height;
	src 		= info->s_pixels;
	srcskip 	= info->s_skip;
	dst 		= info->d_pixels;
	dstskip 	= info->d_skip;
	map 		= info->table;
	srcfmt		= info->src;
	srcbpp		= srcfmt->BytesPerPixel;

	if ( map == NULL )
	{
		while ( height-- )
		{
			#ifdef USE_DUFFS_LOOP
			DUFFS_LOOP(
				DISEMBLE_RGB(src, srcbpp, srcfmt, pixel,
								sR, sG, sB);
				if ( 1 )
				{
					/* Pack RGB into 8bit pixel */
					*dst = ((sR>>5)<<(3+2))|
						((sG>>5)<<(2)) |
						((sB>>6)<<(0)) ;
				}
				dst++;
				src += srcbpp;
				, width);
			#else
			for ( c=width; c; --c )
			{
				DISEMBLE_RGB(src, srcbpp, srcfmt, pixel,
								sR, sG, sB);
				if ( 1 )
				{
					/* Pack RGB into 8bit pixel */
					*dst = ((sR>>5)<<(3+2))|
						((sG>>5)<<(2)) |
						((sB>>6)<<(0)) ;
				}
				dst++;
				src += srcbpp;
			}
			#endif
			src += srcskip;
			dst += dstskip;
		}
	}
	else
	{
		while ( height-- )
		{
			#ifdef USE_DUFFS_LOOP
			DUFFS_LOOP(
				DISEMBLE_RGB(src, srcbpp, srcfmt, pixel,
								sR, sG, sB);
				if ( 1 )
				{
					/* Pack RGB into 8bit pixel */
					*dst = map[((sR>>5)<<(3+2))|
						   ((sG>>5)<<(2))  |
						   ((sB>>6)<<(0))  ];
				}
				dst++;
				src += srcbpp;
			, width);
			#else
			for ( c=width; c; --c )
			{
				DISEMBLE_RGB(src, srcbpp, srcfmt, pixel,
								sR, sG, sB);
				if ( 1 )
				{
					/* Pack RGB into 8bit pixel */
					*dst = map[((sR>>5)<<(3+2))|
						   ((sG>>5)<<(2))  |
						   ((sB>>6)<<(0))  ];
				}
				dst++;
				src += srcbpp;
			}
			#endif /* USE_DUFFS_LOOP */
			src += srcskip;
			dst += dstskip;
		}
	}
}
static void BlitNtoN(SDL_BlitInfo *info)
{
	int width		= info->d_width;
	int height		= info->d_height;
	u8 *src		= info->s_pixels;
	int srcskip 	= info->s_skip;
	u8 *dst		= info->d_pixels;
	int dstskip 	= info->d_skip;
	SDL_PixelFormat *srcfmt 	= info->src;
	int srcbpp					= srcfmt->BytesPerPixel;
	SDL_PixelFormat *dstfmt 	= info->dst;
	int dstbpp					= dstfmt->BytesPerPixel;
	unsigned alpha				= dstfmt->Amask ? srcfmt->alpha : 0;

	while ( height-- )
	{
		DUFFS_LOOP(
		{
			u32 pixel;
			unsigned sR;
			unsigned sG;
			unsigned sB;
			DISEMBLE_RGB(src, srcbpp, srcfmt, pixel, sR, sG, sB);
			ASSEMBLE_RGBA(dst, dstbpp, dstfmt, sR, sG, sB, alpha);
			dst += dstbpp;
			src += srcbpp;
		},
		width);
		src += srcskip;
		dst += dstskip;
	}
}



static void BlitNto1Key(SDL_BlitInfo *info)
{
	int width		= info->d_width;
	int height		= info->d_height;
	u8 *src		= info->s_pixels;
	int srcskip 	= info->s_skip;
	u8 *dst		= info->d_pixels;
	int dstskip 	= info->d_skip;
	SDL_PixelFormat *srcfmt 	= info->src;
	const u8 *palmap 		= info->table;
	u32 ckey 	= srcfmt->colorkey;
	u32 rgbmask	= ~srcfmt->Amask;
	int srcbpp;
	u32 pixel;
	u8  sR, sG, sB;

	/* Set up some basic variables */
	srcbpp = srcfmt->BytesPerPixel;
	ckey &= rgbmask;

	if ( palmap == NULL )
	{
		while ( height-- )
		{
			DUFFS_LOOP(
			{
				DISEMBLE_RGB(src, srcbpp, srcfmt, pixel,
								sR, sG, sB);
				if ( (pixel & rgbmask) != ckey )
				{
					/* Pack RGB into 8bit pixel */
					*dst = ((sR>>5)<<(3+2))|
						((sG>>5)<<(2)) |
						((sB>>6)<<(0)) ;
				}
				dst++;
				src += srcbpp;
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
				DISEMBLE_RGB(src, srcbpp, srcfmt, pixel,
								sR, sG, sB);
				if ( (pixel & rgbmask) != ckey )
				{
					/* Pack RGB into 8bit pixel */
					*dst = palmap[((sR>>5)<<(3+2))|
							((sG>>5)<<(2))	|
							((sB>>6)<<(0))	];
				}
				dst++;
				src += srcbpp;
				},
				width);
			src += srcskip;
			dst += dstskip;
		}
	}
}

static void Blit2to2Key(SDL_BlitInfo *info)
{
	int width		= info->d_width;
	int height		= info->d_height;
	u16 *srcp	= (u16 *)info->s_pixels;
	int srcskip 	= info->s_skip;
	u16 *dstp	= (u16 *)info->d_pixels;
	int dstskip 	= info->d_skip;
	u32 ckey 	= info->src->colorkey;
	u32 rgbmask	= ~info->src->Amask;

	/* Set up some basic variables */
	#if (1==USE_MY_DIV)
	srcskip >>= 1;						/*(srcskipは実は必ずunsigned。)*/
	dstskip >>= 1;						/*(dstskipは実は必ずunsigned。)*/
	#else/*(original)*/
	srcskip /= 2;
	dstskip /= 2;
	#endif
	ckey &= rgbmask;

	while ( height-- )
	{
		DUFFS_LOOP(
			{
			if ( (*srcp & rgbmask) != ckey )
			{
				*dstp = *srcp;
			}
			dstp++;
			srcp++;
			},
			width);
		srcp += srcskip;
		dstp += dstskip;
	}
}

static void BlitNtoNKey(SDL_BlitInfo *info)
{
	int width		= info->d_width;
	int height		= info->d_height;
	u8 *src		= info->s_pixels;
	int srcskip 	= info->s_skip;
	u8 *dst		= info->d_pixels;
	int dstskip 	= info->d_skip;
	u32 ckey 	= info->src->colorkey;
	SDL_PixelFormat *srcfmt 	= info->src;
	SDL_PixelFormat *dstfmt 	= info->dst;
	int srcbpp		= srcfmt->BytesPerPixel;
	int dstbpp		= dstfmt->BytesPerPixel;
	unsigned alpha = dstfmt->Amask ? srcfmt->alpha : 0;

	while ( height-- )
	{
		DUFFS_LOOP(
			{
				u32 pixel;
				unsigned sR;
				unsigned sG;
				unsigned sB;
				RETRIEVE_RGB_PIXEL(src, srcbpp, pixel);
				if ( pixel != ckey )
				{
					RGB_FROM_PIXEL(pixel, srcfmt, sR, sG, sB);
					ASSEMBLE_RGBA(dst, dstbpp, dstfmt,
							  sR, sG, sB, alpha);
				}
				dst += dstbpp;
				src += srcbpp;
			},
			width);
		src += srcskip;
		dst += dstskip;
	}
}

static void BlitNtoNKeyCopyAlpha(SDL_BlitInfo *info)
{
	int width		= info->d_width;
	int height		= info->d_height;
	u8 *src		= info->s_pixels;
	int srcskip 	= info->s_skip;
	u8 *dst		= info->d_pixels;
	int dstskip 	= info->d_skip;
	u32 ckey 	= info->src->colorkey;
	SDL_PixelFormat *srcfmt 	= info->src;
	SDL_PixelFormat *dstfmt 	= info->dst;
	u32 rgbmask	= ~srcfmt->Amask;

	u8 srcbpp;
	u8 dstbpp;
	u32 pixel;
	u8  sR, sG, sB, sA;

	/* Set up some basic variables */
	srcbpp = srcfmt->BytesPerPixel;
	dstbpp = dstfmt->BytesPerPixel;
	ckey &= rgbmask;

	/* FIXME: should map alpha to [0..255] correctly! */
	while ( height-- )
	{
		DUFFS_LOOP(
			{
				DISEMBLE_RGBA(src, srcbpp, srcfmt, pixel,
					  sR, sG, sB, sA);
				if ( (pixel & rgbmask) != ckey )
				{
					  ASSEMBLE_RGBA(dst, dstbpp, dstfmt,
							sR, sG, sB, sA);
				}
				dst += dstbpp;
				src += srcbpp;
			},
			width);
		src += srcskip;
		dst += dstskip;
	}
}

/* Mask matches table, or table entry is zero */
#define MASKOK(x, y) (((x) == (y)) || ((y) == 0x00000000))

SDL_loblit SDL_CalculateBlitN(SDL_Surface *surface, int blit_index)
{
	struct private_swaccel *sdata;
	SDL_PixelFormat *srcfmt;
	SDL_PixelFormat *dstfmt;
	/* Set up data for choosing the blit */
	sdata = surface->map->sw_data;
	srcfmt = surface->format;
	dstfmt = surface->map->dst->format;
	//
	if ( blit_index & 2 )
	{
		/* alpha or alpha+colorkey */
		return SDL_CalculateAlphaBlit(surface, blit_index);
	}
	/* We don't support destinations less than 8-bits */
	if ( dstfmt->BitsPerPixel < 8 )
	{
		return (NULL);
	}
	//
	if (blit_index == 1)
	{
		/* colorkey blit: Here we don't have too many options, mostly
		   because RLE is the preferred fast way to deal with this.
		   If a particular case turns out to be useful we'll add it. */
		if (srcfmt->BytesPerPixel == 2
		   && surface->map->identity)
		{	return (Blit2to2Key);
		}
		else
		if (dstfmt->BytesPerPixel == 1)
		{	return (BlitNto1Key);
		}
		else
		{
			if (srcfmt->Amask && dstfmt->Amask)
			{
				return BlitNtoNKeyCopyAlpha;
			}
			else
			{
				return BlitNtoNKey;
			}
		}
	}
	//
	SDL_loblit blitfun;
	blitfun = NULL;
	if ( dstfmt->BitsPerPixel == 8 )
	{
		/* We assume 8-bit destinations are palettized */
		if ( (srcfmt->BytesPerPixel == 4) &&
			 (srcfmt->Rmask == 0x00FF0000) &&
			 (srcfmt->Gmask == 0x0000FF00) &&
			 (srcfmt->Bmask == 0x000000FF) )
		{
			if ( surface->map->table )
			{
				blitfun = Blit_RGB888_index8_map;
			}
			else
			{
				#ifdef USE_ASMBLIT
				sdata->aux_data = ConvertX86p32_8RGB332;
				blitfun = ConvertX86;
				#else
				blitfun = Blit_RGB888_index8;
				#endif
			}
		}
		else
		{
			blitfun = BlitNto1;
		}
	}
	else
	{
		/* Now the meat, choose the blitter we want */
	//	int a_need = 0;
	//	if (dstfmt->Amask)
	//	{	a_need = srcfmt->Amask ? 4/*COPY_ALPHA*/ : 2/*SET_ALPHA*/;	}
		//
		sdata->aux_data = NULL/*table[which].aux_data*/;
		blitfun = BlitNtoN;//table[which].blitfunc;
	//	if ((a_need == 4/*COPY_ALPHA*/) && (blitfun == BlitNtoN))
	//	{	blitfun = BlitNtoNCopyAlpha;
	//	}
	}
	#ifdef DEBUG_ASM
		#ifdef USE_ASMBLIT
			if ( blitfun == ConvertMMX )	{	fprintf(stderr, "Using mmx blit\n");	}
	else	if ( blitfun == ConvertX86 )	{	fprintf(stderr, "Using asm blit\n");	}
	else
		#endif
			if ( (blitfun == SDL_BlitNtoN) || (blitfun == SDL_BlitNto1) )
											{	fprintf(stderr, "Using C blit\n");				}
	else									{	fprintf(stderr, "Using optimized C blit\n");	}
	#endif /* DEBUG_ASM */
	return (blitfun);
}
