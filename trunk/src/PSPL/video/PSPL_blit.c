
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	/src/SDL231/video/SDL_blit.c
	date:		2011/02/04
	source: 	http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2FSDL_blit.c
	revision:	441(2366以下では最新)
--------------------------------------------------------- */

/*
psp - リビジョン 441Subversion リポジトリ一覧: pspps2ps2wareps3ps3warepspwareリビジョン:(root)/trunk/SDL/src/video/SDL_blit.c
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
 "@(#) $Id: PSPL_blit.c,v 1.13 2005/04/17 10:40:41 icculus Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../include/PSPL_error.h"			/* 取り敢えず(仮) */
#include "./../include/PSPL_video.h"			/* 取り敢えず(仮) */
#include "./PSPL_sysvideo.h"
#include "./PSPL_blit.h"
#include "./PSPL_RLEaccel_c.h"
#include "./PSPL_pixels_c.h"
#include "./PSPL_memops.h"




#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb( ... )
#endif




/* The general purpose software blit routine */
static int SDL_SoftBlit(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect)
{
	int okay;
	int src_locked;
	int dst_locked;

	/* Everything is okay at the beginning...  */
	okay = 1;

	/* Lock the destination if it's in hardware */
	dst_locked = 0;
	if ( SDL_MUSTLOCK(dst) )
	{
		if ( SDL_LockSurface(dst) < 0 )
		{
			okay = 0;
		}
		else
		{
			dst_locked = 1;
		}
	}
	/* Lock the source if it's in hardware */
	src_locked = 0;
	if ( SDL_MUSTLOCK(src) )
	{
		if ( SDL_LockSurface(src) < 0 )
		{
			okay = 0;
		}
		else
		{
			src_locked = 1;
		}
	}

	/* Set up source and destination buffer pointers, and BLIT! */
	if ( okay  && srcrect->w && srcrect->h )
	{
		SDL_BlitInfo info;
		SDL_loblit RunBlit;

		/* Set up the blit information */
		info.s_pixels	= (u8 *)src->pixels +
				(u16)srcrect->y*src->pitch +
				(u16)srcrect->x*src->format->BytesPerPixel;
		info.s_width	= srcrect->w;
		info.s_height	= srcrect->h;
		info.s_skip 	= src->pitch-info.s_width*src->format->BytesPerPixel;
		info.d_pixels	= (u8 *)dst->pixels +
				(u16)dstrect->y*dst->pitch +
				(u16)dstrect->x*dst->format->BytesPerPixel;
		info.d_width	= dstrect->w;
		info.d_height	= dstrect->h;
		info.d_skip 	= dst->pitch-info.d_width*dst->format->BytesPerPixel;
		info.aux_data	= src->map->sw_data->aux_data;
		info.src		= src->format;
		info.table		= src->map->table;
		info.dst		= dst->format;
		RunBlit 		= src->map->sw_data->blit;

		/* Run the actual software blit */
		RunBlit(&info);
	}

	/* We need to unlock the surfaces if they're locked */
	if ( dst_locked )
	{
		SDL_UnlockSurface(dst);
	}
	if ( src_locked )
	{
		SDL_UnlockSurface(src);
	}
	/* Blit is done! */
	return (okay ? 0 : -1);
}

static void SDL_BlitCopy(SDL_BlitInfo *info)
{
	u8 *src, *dst;
	int w, h;
	int srcskip, dstskip;

	w = info->d_width*info->dst->BytesPerPixel;
	h = info->d_height;
	src = info->s_pixels;
	dst = info->d_pixels;
	srcskip = w+info->s_skip;
	dstskip = w+info->d_skip;
	while ( h-- )
	{
		SDL_memcpy(dst, src, w);/* normal */
		src += srcskip;
		dst += dstskip;
	}
}

static void SDL_BlitCopyOverlap(SDL_BlitInfo *info)
{
	u8 *src, *dst;
	int w, h;
	int srcskip, dstskip;
	w = info->d_width*info->dst->BytesPerPixel;
	h = info->d_height;
	src = info->s_pixels;
	dst = info->d_pixels;
	srcskip = w+info->s_skip;
	dstskip = w+info->d_skip;
	if ( dst < src )
	{
		while ( h-- )
		{
			SDL_memcpy(dst, src, w);
			src += srcskip;
			dst += dstskip;
		}
	}
	else
	{
		src += ((h-1) * srcskip);
		dst += ((h-1) * dstskip);
		while ( h-- )
		{
			SDL_revcpy(dst, src, w);
			src -= srcskip;
			dst -= dstskip;
		}
	}
}

/* Figure out which of many blit routines to set up on a surface */
int SDL_CalculateBlit(SDL_Surface *surface)
{
	int blit_index;
	/* Clean everything out to start */
	if ( (surface->flags & SDL_RLEACCEL) == SDL_RLEACCEL )
	{
		SDL_UnRLESurface(surface, 1);
	}
	surface->map->sw_blit = NULL;

	/* Figure out if an accelerated hardware blit is possible */
	surface->flags &= ~SDL_HWACCEL;

	/* Get the blit function index, based on surface mode */
	/* { 0 = nothing, 1 = colorkey, 2 = alpha, 3 = colorkey+alpha } */
	blit_index = 0;
	blit_index |= (!!(surface->flags & SDL_SRCCOLORKEY))	  << 0;
	if ( surface->flags & SDL_SRCALPHA
		 && (surface->format->alpha != SDL_ALPHA_OPAQUE
		 || surface->format->Amask) )
	{
		blit_index |= 2;
	}

	/* Check for special "identity" case -- copy blit */
	if ( surface->map->identity && blit_index == 0 )
	{
		surface->map->sw_data->blit = SDL_BlitCopy;

		/* Handle overlapping blits on the same surface */
		if ( surface == surface->map->dst )
		{
			surface->map->sw_data->blit = SDL_BlitCopyOverlap;
		}
	}
	else
	{
		if ( surface->format->BitsPerPixel < 8 )
		{
			surface->map->sw_data->blit =
				SDL_CalculateBlit0(surface, blit_index);
		}
		else
		{
			switch ( surface->format->BytesPerPixel )
			{
			case 1:
				surface->map->sw_data->blit =
					SDL_CalculateBlit1(surface, blit_index);
				break;
			case 2:
			case 3:
			case 4:
				surface->map->sw_data->blit =
					SDL_CalculateBlitN(surface, blit_index);
				break;
			default:
				surface->map->sw_data->blit = NULL;
				break;
			}
		}
	}
	/* Make sure we have a blit function */
	if ( surface->map->sw_data->blit == NULL )
	{
		SDL_InvalidateMap(surface->map);
		SDL_SetError_bbb("Blit combination not supported");
		return (-1);
	}

	/* Choose software blitting function */
	if (surface->flags & SDL_RLEACCELOK
	   && (surface->flags & SDL_HWACCEL) != SDL_HWACCEL)
	{
		if (surface->map->identity
		   && (blit_index == 1
			   || (blit_index == 3 && !surface->format->Amask)))
		{
			if ( SDL_RLESurface(surface) == 0 )
			{
				surface->map->sw_blit = SDL_RLEBlit;
			}
		}
		else
		if (blit_index == 2 && surface->format->Amask)
		{
			if ( SDL_RLESurface(surface) == 0 )
			{
				surface->map->sw_blit = SDL_RLEAlphaBlit;
			}
		}
	}
	if ( surface->map->sw_blit == NULL )
	{
		surface->map->sw_blit = SDL_SoftBlit;
	}
	return (0);
}
