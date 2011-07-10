
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	/src/SDL231/video/SDL_RLEaccel.c
	date:		2011/02/04
	source: 	http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2FSDL_RLEaccel.c
	revision:	440(2366�ȉ��ł͍ŐV)
--------------------------------------------------------- */

/*
psp - ���r�W���� 440Subversion ���|�W�g���ꗗ: pspps2ps2wareps3ps3warepspware���r�W����:(root)/trunk/SDL/src/video/SDL_RLEaccel.c
�ڍו\�� - �ŏI�X�V���� - ���O������ -
*/

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_RLEaccel.c,v 1.11 2004/08/21 13:10:58 slouken Exp $";
#endif


/*
 * RLE encoding for software colorkey and alpha-channel acceleration
 *
 * Original version by Sam Lantinga
 *
 * Mattias Engdegard (Yorick): Rewrite. New encoding format, encoder and
 * decoder. Added per-surface alpha blitter. Added per-pixel alpha
 * format, encoder and blitter.
 *
 * Many thanks to Xark and johns for hints, benchmarks and useful comments
 * leading to this code.
 *
 * Welcome to Macro Mayhem.
 */

/*
 * The encoding translates the image data to a stream of segments of the form
 *
 * <skip> <run> <data>
 *
 * where <skip> is the number of transparent pixels to skip,
 *	   <run>  is the number of opaque pixels to blit,
 * and	 <data> are the pixels themselves.
 *
 * This basic structure is used both for colorkeyed surfaces, used for simple
 * binary transparency and for per-surface alpha blending, and for surfaces
 * with per-pixel alpha. The details differ, however:
 *
 * Encoding of colorkeyed surfaces:
 *
 *	 Encoded pixels always have the same format as the target surface.
 *	 <skip> and <run> are unsigned 8 bit integers, except for 32 bit depth
 *	 where they are 16 bit. This makes the pixel data aligned at all times.
 *	 Segments never wrap around from one scan line to the next.
 *
 *	 The end of the sequence is marked by a zero <skip>,<run> pair at the *
 *	 beginning of a line.
 *
 * Encoding of surfaces with per-pixel alpha:
 *
 *	 The sequence begins with a struct RLEDestFormat describing the target
 *	 pixel format, to provide reliable un-encoding.
 *
 *	 Each scan line is encoded twice: First all completely opaque pixels,
 *	 encoded in the target format as described above, and then all
 *	 partially transparent (translucent) pixels (where 1 <= alpha <= 254),
 *	 in the following 32-bit format:
 *
 *	 For 32-bit targets, each pixel has the target RGB format but with
 *	 the alpha value occupying the highest 8 bits. The <skip> and <run>
 *	 counts are 16 bit.
 *
 *	 For 16-bit targets, each pixel has the target RGB format, but with
 *	 the middle component (usually green) shifted 16 steps to the left,
 *	 and the hole filled with the 5 most significant bits of the alpha value.
 *	 i.e. if the target has the format	 rrrrrggggggbbbbb,
 *	 the encoded pixel will be 00000gggggg00000rrrrr0aaaaabbbbb.
 *	 The <skip> and <run> counts are 8 bit for the opaque lines, 16 bit
 *	 for the translucent lines. Two padding bytes may be inserted
 *	 before each translucent line to keep them 32-bit aligned.
 *
 *	 The end of the sequence is marked by a zero <skip>,<run> pair at the
 *	 beginning of an opaque line.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "PSPL_types.h"	/* ��芸����(��) */
#include "./../include/PSPL_video.h"	/* ��芸����(��) */
#include "./../include/PSPL_error.h"	/* ��芸����(��) */
#include "PSPL_sysvideo.h"
#include "PSPL_blit.h"
#include "PSPL_memops.h"
#include "PSPL_RLEaccel_c.h"

#ifndef SDL_OutOfMemory_bbb
	#define SDL_OutOfMemory_bbb(aaa)
#endif

//#ifndef MAX
//	#define MAX(a, b) ((a) > (b) ? (a) : (b))
//#endif
//#ifndef MIN
//	#define MIN(a, b) ((a) < (b) ? (a) : (b))
//#endif
#ifndef MIN
	#define MIN 	__builtin_allegrex_min
#endif

#define PIXEL_COPY(to, from, len, bpp)									\
do {																	\
	if (bpp == 4)														\
			{	SDL_memcpy4(to, from, (unsigned)(len)); 			}	\
	else	{	SDL_memcpy( to, from, (unsigned)(len) * (bpp)); 	}	\
} while (0)

/*
 * Various colorkey blit methods, for opaque and per-surface alpha
 */

#define OPAQUE_BLIT(to, from, length, bpp, alpha)		\
	PIXEL_COPY(to, from, length, bpp)
/*
 * For 32bpp pixels on the form 0x00rrggbb:
 * If we treat the middle component separately, we can process the two
 * remaining in parallel. This is safe to do because of the gap to the left
 * of each component, so the bits from the multiplication don't collide.
 * This can be used for any RGB permutation of course.
 */
#define ALPHA_BLIT32_888(to, from, length, bpp, alpha)	\
	do {												\
		int i;												\
		u32 *src = (u32 *)(from); 					\
		u32 *dst = (u32 *)(to);						\
		for (i=0; i<(int)(length); i++) 					\
		{													\
			u32 s = *src++;								\
			u32 d = *dst;								\
			u32 s1 = s & 0xff00ff;						\
			u32 d1 = d & 0xff00ff;						\
			d1 = (d1 + ((s1 - d1) * alpha >> 8)) & 0xff00ff;	\
			s &= 0xff00;										\
			d &= 0xff00;										\
			d = (d + ((s - d) * alpha >> 8)) & 0xff00;			\
			*dst++ = d1 | d;									\
		}														\
	} while (0)

/*
 * For 16bpp pixels we can go a step further: put the middle component
 * in the high 16 bits of a 32 bit word, and process all three RGB
 * components at the same time. Since the smallest gap is here just
 * 5 bits, we have to scale alpha down to 5 bits as well.
 */
#define ALPHA_BLIT16_565(to, from, length, bpp, alpha)	\
	do {									\
	int i;									\
	u16 *src = (u16 *)(from); 		\
	u16 *dst = (u16 *)(to);			\
	u32 ALPHA = alpha >> 3;				\
	for (i=0; i<(int)(length); i++) 	{	\
		u32 s = *src++;					\
		u32 d = *dst;					\
		s = (s | s << 16) & 0x07e0f81f; 	\
		d = (d | d << 16) & 0x07e0f81f; 	\
		d += (s - d) * ALPHA >> 5;			\
		d &= 0x07e0f81f;					\
		*dst++ = d | d >> 16;				\
	}										\
	} while (0)

#define ALPHA_BLIT16_555(to, from, length, bpp, alpha)	\
	do {									\
	int i;									\
	u16 *src = (u16 *)(from); 		\
	u16 *dst = (u16 *)(to);			\
	u32 ALPHA = alpha >> 3;				\
	for (i=0; i<(int)(length); i++) 	{	\
		u32 s = *src++;					\
		u32 d = *dst;					\
		s = (s | s << 16) & 0x03e07c1f; 	\
		d = (d | d << 16) & 0x03e07c1f; 	\
		d += (s - d) * ALPHA >> 5;			\
		d &= 0x03e07c1f;					\
		*dst++ = d | d >> 16;				\
	}										\
	} while (0)

/*
 * The general slow catch-all function, for remaining depths and formats
 */
#define ALPHA_BLIT_ANY(to, from, length, bpp, alpha)			\
	do {									\
	int i;									\
	u8 *src = from;						\
	u8 *dst = to;						\
	for (i=0; i<(int)(length); i++) 	{	\
		u32 s, d;						\
		unsigned rs, gs, bs, rd, gd, bd;	\
		switch(bpp) 	{						\
		case 2: 							\
		s = *(u16 *)src; 				\
		d = *(u16 *)dst; 				\
		break;								\
		case 3: 							\
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)	{		\
			s = (src[0] << 16) | (src[1] << 8) | src[2];	\
			d = (dst[0] << 16) | (dst[1] << 8) | dst[2];	\
		} else {						\
			s = (src[2] << 16) | (src[1] << 8) | src[0];	\
			d = (dst[2] << 16) | (dst[1] << 8) | dst[0];	\
		}								\
		break;							\
		case 4: 						\
		s = *(u32 *)src; 			\
		d = *(u32 *)dst; 			\
		break;							\
		}								\
		RGB_FROM_PIXEL(s, fmt, rs, gs, bs); 			\
		RGB_FROM_PIXEL(d, fmt, rd, gd, bd); 			\
		rd += (rs - rd) * alpha >> 8;					\
		gd += (gs - gd) * alpha >> 8;					\
		bd += (bs - bd) * alpha >> 8;					\
		PIXEL_FROM_RGB(d, fmt, rd, gd, bd); 			\
		switch(bpp) 	{							\
		case 2: 								\
		*(u16 *)dst = d; 					\
		break;								\
		case 3: 							\
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)	{			\
			dst[0] = d >> 16;				\
			dst[1] = d >> 8;				\
			dst[2] = d; 					\
		} else {							\
			dst[0] = d; 					\
			dst[1] = d >> 8;				\
			dst[2] = d >> 16;				\
		}									\
		break;								\
		case 4: 							\
		*(u32 *)dst = d; 				\
		break;								\
		}									\
		src += bpp; 						\
		dst += bpp; 						\
	}										\
	} while (0)



#define CHOOSE_BLIT(blitter, alpha, fmt)					\
	do {													\
	if (alpha == 255)										\
	{														\
		switch(fmt->BytesPerPixel)							\
		{													\
		case 1: blitter(1, u8, OPAQUE_BLIT); break;		\
		case 2: blitter(2, u8, OPAQUE_BLIT); break;		\
		case 3: blitter(3, u8, OPAQUE_BLIT); break;		\
		case 4: blitter(4, u16, OPAQUE_BLIT); break; 	\
		}													\
	}														\
	else													\
	{														\
		switch(fmt->BytesPerPixel)							\
		{													\
		case 1: 											\
			/* No 8bpp alpha blitting */					\
			break;														\
		case 2: 														\
			switch(fmt->Rmask | fmt->Gmask | fmt->Bmask)				\
			{															\
			case 0xffff:												\
				if (fmt->Gmask == 0x07e0								\
				 || fmt->Rmask == 0x07e0								\
				 || fmt->Bmask == 0x07e0)								\
				{														\
					blitter(2, u8, ALPHA_BLIT16_565);				\
				}														\
				else													\
				{	goto general16; 	}								\
				break;													\
			case 0x7fff:												\
				if (fmt->Gmask == 0x03e0								\
				 || fmt->Rmask == 0x03e0								\
				 || fmt->Bmask == 0x03e0)								\
				{														\
					blitter(2, u8, ALPHA_BLIT16_555);				\
				break;													\
				}														\
			/*	else */ 												\
			/*	{	goto general16; 	} */							\
			/* fallthrough */											\
			default:													\
			general16:													\
				blitter(2, u8, ALPHA_BLIT_ANY);						\
			}															\
			break;														\
		case 3: 														\
			blitter(3, u8, ALPHA_BLIT_ANY);							\
			break;														\
		case 4: 														\
			if ((fmt->Rmask | fmt->Gmask | fmt->Bmask) == 0x00ffffff	\
			 && (fmt->Gmask == 0xff00									\
			  || fmt->Rmask == 0xff00									\
			  || fmt->Bmask == 0xff00)) 								\
			{															\
				blitter(4, u16, ALPHA_BLIT32_888);					\
			}															\
			else														\
			{															\
				blitter(4, u16, ALPHA_BLIT_ANY); 					\
			}															\
			break;														\
		}																\
	}																	\
	} while (0)


/*
 * This takes care of the case when the surface is clipped on the left and/or
 * right. Top clipping has already been taken care of.
 */
static void RLEClipBlit(
	int w, u8 *srcbuf, SDL_Surface *dst,
	u8 *dstbuf, SDL_Rect *srcrect, unsigned alpha)
{
	SDL_PixelFormat *fmt = dst->format;
	#define RLECLIPBLIT(bpp, Type, do_blit) 			\
	do {												\
		int linecount = srcrect->h; 					\
		int ofs = 0;									\
		int left = srcrect->x;							\
		int right = left + srcrect->w;					\
		dstbuf -= left * bpp;							\
		for (;;)	{									\
			int run;									\
			ofs += *(Type *)srcbuf; 					\
			run = ((Type *)srcbuf)[1];					\
			srcbuf += 2 * sizeof(Type); 				\
			if (run)	{								\
			/* clip to left and right borders */		\
			if (ofs < right)	{						\
				int start = 0;							\
				int len = run;							\
				int startcol;							\
				if (left - ofs > 0) 	{				\
				start = left - ofs; 					\
				len -= start;							\
				if (len <= 0)							\
					goto nocopy ## bpp ## do_blit;		\
				}										\
				startcol = ofs + start; 				\
				if (len > right - startcol) 			\
				len = right - startcol; 				\
				do_blit(dstbuf + startcol * bpp, srcbuf + start * bpp,	\
					len, bpp, alpha);					\
			}											\
			nocopy ## bpp ## do_blit:					\
			srcbuf += run * bpp;						\
			ofs += run; 								\
			} else if (!ofs)							\
			break;										\
			if (ofs == w)	{							\
			ofs = 0;									\
			dstbuf += dst->pitch;						\
			if (!--linecount)							\
				break;									\
			}											\
		}												\
	} while (0)

	CHOOSE_BLIT(RLECLIPBLIT, alpha, fmt);

	#undef RLECLIPBLIT
}


/* blit a colorkeyed RLE surface */
int SDL_RLEBlit(
	SDL_Surface *src, SDL_Rect *srcrect,
	SDL_Surface *dst, SDL_Rect *dstrect)
{
	u8 *dstbuf;
	u8 *srcbuf;
	int x, y;
	int w = src->w;
	unsigned alpha;

	/* Lock the destination if necessary */
	if ( SDL_MUSTLOCK(dst) )
	{
		if ( SDL_LockSurface(dst) < 0 )
		{
			return (-1);
		}
	}

	/* Set up the source and destination pointers */
	x = dstrect->x;
	y = dstrect->y;
	dstbuf = (u8 *)dst->pixels + y * dst->pitch + x * src->format->BytesPerPixel;
	srcbuf = (u8 *)src->map->sw_data->aux_data;

	{
		/* skip lines at the top if neccessary */
		int vskip = srcrect->y;
		int ofs = 0;
		if (vskip)
		{

#define RLESKIP(bpp, Type)					\
		for (;;)	{							\
			int run;						\
			ofs += *(Type *)srcbuf; 		\
			run = ((Type *)srcbuf)[1];		\
			srcbuf += sizeof(Type) * 2; 	\
			if (run)	{						\
			srcbuf += run * bpp;			\
			ofs += run; 					\
			} else if (!ofs)				\
			goto done;						\
			if (ofs == w)	{					\
			ofs = 0;						\
			if (!--vskip)					\
				break;						\
			}								\
		}

		switch(src->format->BytesPerPixel)
		{
		case 1: RLESKIP(1, u8); break;
		case 2: RLESKIP(2, u8); break;
		case 3: RLESKIP(3, u8); break;
		case 4: RLESKIP(4, u16); break;
		}

#undef RLESKIP

		}
	}

	alpha = (src->flags & SDL_SRCALPHA) == SDL_SRCALPHA ? src->format->alpha : 255;
	/* if left or right edge clipping needed, call clip blit */
	if ( srcrect->x || srcrect->w != src->w )
	{
		RLEClipBlit(w, srcbuf, dst, dstbuf, srcrect, alpha);
	}
	else
	{
		SDL_PixelFormat *fmt = src->format;

#define RLEBLIT(bpp, Type, do_blit) 					\
		do {											\
		int linecount = srcrect->h; 					\
		int ofs = 0;									\
		for (;;)	{										\
			unsigned run;								\
			ofs += *(Type *)srcbuf; 					\
			run = ((Type *)srcbuf)[1];					\
			srcbuf += 2 * sizeof(Type); 				\
			if (run)	{									\
			do_blit(dstbuf + ofs * bpp, srcbuf, run, bpp, alpha);\
			srcbuf += run * bpp;						\
			ofs += run; 								\
			} else if (!ofs)							\
			break;										\
			if (ofs == w)	{								\
			ofs = 0;									\
			dstbuf += dst->pitch;						\
			if (!--linecount)							\
				break;									\
			}											\
		}												\
		} while (0)

		CHOOSE_BLIT(RLEBLIT, alpha, fmt);

#undef RLEBLIT
	}

done:
	/* Unlock the destination if necessary */
	if ( SDL_MUSTLOCK(dst) )
	{
		SDL_UnlockSurface(dst);
	}
	return (0);
}

#undef OPAQUE_BLIT

/*
 * Per-pixel blitting macros for translucent pixels:
 * These use the same techniques as the per-surface blitting macros
 */

/*
 * For 32bpp pixels, we have made sure the alpha is stored in the top
 * 8 bits, so proceed as usual
 */
#define BLIT_TRANSL_888(src, dst)				\
	do {										\
		u32 s = src; 						\
		u32 d = dst; 						\
		unsigned alpha = s >> 24;				\
		u32 s1 = s & 0xff00ff;				\
		u32 d1 = d & 0xff00ff;				\
		d1 = (d1 + ((s1 - d1) * alpha >> 8)) & 0xff00ff;		\
		s &= 0xff00;							\
		d &= 0xff00;							\
		d = (d + ((s - d) * alpha >> 8)) & 0xff00;				\
		dst = d1 | d;							\
	} while (0)

/*
 * For 16bpp pixels, we have stored the 5 most significant alpha bits in
 * bits 5-10. As before, we can process all 3 RGB components at the same time.
 */
#define BLIT_TRANSL_565(src, dst)			\
	do {									\
		u32 s = src; 					\
		u32 d = dst; 					\
		unsigned alpha = (s & 0x3e0) >> 5;	\
		s &= 0x07e0f81f;					\
		d = (d | d << 16) & 0x07e0f81f; 	\
		d += (s - d) * alpha >> 5;			\
		d &= 0x07e0f81f;					\
		dst = d | d >> 16;					\
	} while (0)

#define BLIT_TRANSL_555(src, dst)			\
	do {									\
		u32 s = src; 					\
		u32 d = dst; 					\
		unsigned alpha = (s & 0x3e0) >> 5;	\
		s &= 0x03e07c1f;					\
		d = (d | d << 16) & 0x03e07c1f; 	\
		d += (s - d) * alpha >> 5;			\
		d &= 0x03e07c1f;					\
		dst = d | d >> 16;					\
	} while (0)

/* used to save the destination format in the encoding. Designed to be
   macro-compatible with SDL_PixelFormat but without the unneeded fields */
typedef struct
{
	u8  BytesPerPixel;
	u8  Rloss;
	u8  Gloss;
	u8  Bloss;
	u8  Rshift;
	u8  Gshift;
	u8  Bshift;
	u8  Ashift;
	u32 Rmask;
	u32 Gmask;
	u32 Bmask;
	u32 Amask;
} RLEDestFormat;


/* blit a pixel-alpha RLE surface clipped at the right and/or left edges */
static void RLEAlphaClipBlit(int w, u8 *srcbuf, SDL_Surface *dst, u8 *dstbuf, SDL_Rect *srcrect)
{
	SDL_PixelFormat *df = dst->format;
	/*
	 * clipped blitter: Ptype is the destination pixel type,
	 * Ctype the translucent count type, and do_blend the macro
	 * to blend one pixel.
	 */
	#define RLEALPHACLIPBLIT(Ptype, Ctype, do_blend)				\
		do {														\
			int linecount = srcrect->h; 							\
			int left = srcrect->x;									\
			int right = left + srcrect->w;							\
			dstbuf -= left * sizeof(Ptype); 						\
			do {													\
				int ofs = 0;										\
				/* blit opaque pixels on one line */				\
				do {												\
					unsigned run;									\
					ofs += ((Ctype *)srcbuf)[0];					\
					run = ((Ctype *)srcbuf)[1]; 					\
					srcbuf += 2 * sizeof(Ctype);					\
					if (run)	{										\
						/* clip to left and right borders */		\
						int cofs = ofs; 							\
						int crun = run; 							\
						if (left - cofs > 0)	{						\
							crun -= left - cofs;					\
							cofs = left;							\
						}											\
						if (crun > right - cofs)					\
						{	crun = right - cofs;	}				\
						if (crun > 0)								\
						{	PIXEL_COPY(dstbuf + cofs * sizeof(Ptype),		\
								srcbuf + (cofs - ofs) * sizeof(Ptype),		\
								(unsigned)crun, sizeof(Ptype));}	\
						srcbuf += run * sizeof(Ptype);				\
						ofs += run; 								\
					} else if (!ofs)								\
					{	return; 	}								\
				} while (ofs < w);									\
				/* skip padding if necessary */ 					\
				if (sizeof(Ptype) == 2) 							\
				{	srcbuf += (unsigned long)srcbuf & 2;	}		\
				/* blit translucent pixels on the same line */		\
				ofs = 0;											\
				do {												\
					unsigned run;									\
					ofs += ((u16 *)srcbuf)[0];					\
					run = ((u16 *)srcbuf)[1];					\
					srcbuf += 4;									\
					if (run)	{										\
						/* clip to left and right borders */		\
						int cofs = ofs; 							\
						int crun = run; 							\
						if (left - cofs > 0)	{						\
							crun -= left - cofs;					\
							cofs = left;							\
						}											\
						if (crun > right - cofs)					\
						{	crun = right - cofs;	}				\
						if (crun > 0)	{								\
							Ptype *dst = (Ptype *)dstbuf + cofs;				\
							u32 *src = (u32 *)srcbuf + (cofs - ofs);		\
							int i;									\
							for (i = 0; i < crun; i++)				\
							{	do_blend(src[i], dst[i]);	}		\
						}											\
						srcbuf += run * 4;							\
						ofs += run; 								\
					}												\
				} while (ofs < w);									\
				dstbuf += dst->pitch;								\
			} while (--linecount);									\
		} while (0)

	switch(df->BytesPerPixel)
	{
	case 2:
		if (df->Gmask == 0x07e0 || df->Rmask == 0x07e0
			|| df->Bmask == 0x07e0)
		{	RLEALPHACLIPBLIT(u16, u8, BLIT_TRANSL_565);	}
		else
		{	RLEALPHACLIPBLIT(u16, u8, BLIT_TRANSL_555);	}
		break;
	case 4:
		RLEALPHACLIPBLIT(u32, u16, BLIT_TRANSL_888);
		break;
	}
}


/* blit a pixel-alpha RLE surface */
int SDL_RLEAlphaBlit(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect)
{
	int x, y;
	int w = src->w;
	u8 *srcbuf, *dstbuf;
	SDL_PixelFormat *df = dst->format;

	/* Lock the destination if necessary */
	if ( SDL_MUSTLOCK(dst) )
	{
		if ( SDL_LockSurface(dst) < 0 )
		{
			return (-1);
		}
	}

	x = dstrect->x;
	y = dstrect->y;
	dstbuf = (u8 *)dst->pixels + y * dst->pitch + x * df->BytesPerPixel;
	srcbuf = (u8 *)src->map->sw_data->aux_data + sizeof(RLEDestFormat);

	{
		/* skip lines at the top if necessary */
		int vskip = srcrect->y;
		if (vskip)
		{
			int ofs;
			if (df->BytesPerPixel == 2)
			{
				/* the 16/32 interleaved format */
				do {
					/* skip opaque line */
					ofs = 0;
					do {
						int run;
						ofs += srcbuf[0];
						run = srcbuf[1];
						srcbuf += 2;
						if (run)
						{
							srcbuf += 2 * run;
							ofs += run;
						}
						else
						if (!ofs)
						{	goto done;
						}
					} while (ofs < w);

					/* skip padding */
					srcbuf += (unsigned long)srcbuf & 2;

					/* skip translucent line */
					ofs = 0;
					do {
						int run;
						ofs += ((u16 *)srcbuf)[0];
						run = ((u16 *)srcbuf)[1];
						srcbuf += 4 * (run + 1);
						ofs += run;
					} while (ofs < w);
				} while (--vskip);
			}
			else
			{
				/* the 32/32 interleaved format */
				vskip <<= 1;	/* opaque and translucent have same format */
				do {
					ofs = 0;
					do {
						int run;
						ofs += ((u16 *)srcbuf)[0];
						run = ((u16 *)srcbuf)[1];
						srcbuf += 4;
						if (run)
						{
							srcbuf += 4 * run;
							ofs += run;
						}
						else
						if (!ofs)
						{	goto done;
						}
					} while (ofs < w);
				} while (--vskip);
			}
		}
	}

	/* if left or right edge clipping needed, call clip blit */
	if (srcrect->x || srcrect->w != src->w)
	{
		RLEAlphaClipBlit(w, srcbuf, dst, dstbuf, srcrect);
	}
	else
	{
		/*
		 * non-clipped blitter. Ptype is the destination pixel type,
		 * Ctype the translucent count type, and do_blend the
		 * macro to blend one pixel.
		 */
		#define RLEALPHABLIT(Ptype, Ctype, do_blend)				\
			do {													\
				int linecount = srcrect->h; 						\
				do {												\
					int ofs = 0;									\
					/* blit opaque pixels on one line */			\
					do {											\
						unsigned run;								\
						ofs += ((Ctype *)srcbuf)[0];				\
						run = ((Ctype *)srcbuf)[1]; 				\
						srcbuf += 2 * sizeof(Ctype);				\
						if (run)	{									\
							PIXEL_COPY(dstbuf + ofs * sizeof(Ptype), srcbuf,\
							   run, sizeof(Ptype)); 				\
							srcbuf += run * sizeof(Ptype);			\
							ofs += run; 							\
						} else if (!ofs)							\
						{	goto done;	}							\
					} while (ofs < w);								\
					/* skip padding if necessary */ 				\
					if (sizeof(Ptype) == 2) 						\
					{	srcbuf += (unsigned long)srcbuf & 2;	}	\
					/* blit translucent pixels on the same line */	\
					ofs = 0;										\
					do {											\
						unsigned run;								\
						ofs += ((u16 *)srcbuf)[0];				\
						run = ((u16 *)srcbuf)[1];				\
						srcbuf += 4;								\
						if (run)	{									\
							Ptype *dst = (Ptype *)dstbuf + ofs; 	\
							unsigned i; 							\
							for (i = 0; i < run; i++)	{				\
								u32 src = *(u32 *)srcbuf; 	\
								do_blend(src, *dst);				\
								srcbuf += 4;						\
								dst++;								\
							}										\
							ofs += run; 							\
						}											\
					} while (ofs < w);								\
					dstbuf += dst->pitch;							\
				} while (--linecount);								\
			} while (0)
		switch(df->BytesPerPixel)
		{
		case 2:
			if (df->Gmask == 0x07e0 || df->Rmask == 0x07e0
			   || df->Bmask == 0x07e0)
				RLEALPHABLIT(u16, u8, BLIT_TRANSL_565);
			else
				RLEALPHABLIT(u16, u8, BLIT_TRANSL_555);
			break;
		case 4:
			RLEALPHABLIT(u32, u16, BLIT_TRANSL_888);
			break;
		}
	}
done:
	/* Unlock the destination if necessary */
	if ( SDL_MUSTLOCK(dst) )
	{
		SDL_UnlockSurface(dst);
	}
	return (0);
}

/*
 * Auxiliary functions:
 * The encoding functions take 32bpp rgb + a, and
 * return the number of bytes copied to the destination.
 * The decoding functions copy to 32bpp rgb + a, and
 * return the number of bytes copied from the source.
 * These are only used in the encoder and un-RLE code and are therefore not
 * highly optimised.
 */

/* encode 32bpp rgb + a into 16bpp rgb, losing alpha */
static int copy_opaque_16(
	void *dst, u32 *src,
	int n,
	SDL_PixelFormat *sfmt,
	SDL_PixelFormat *dfmt)
{
	int i;
	u16 *d = dst;
	for (i=0; i<n; i++)
	{
		unsigned r, g, b;
		RGB_FROM_PIXEL(*src, sfmt, r, g, b);
		PIXEL_FROM_RGB(*d, dfmt, r, g, b);
		src++;
		d++;
	}
	return (n * 2);
}

/* decode opaque pixels from 16bpp to 32bpp rgb + a */
static int uncopy_opaque_16(
	u32 *dst, void *src,
	int n,
	RLEDestFormat *sfmt,
	SDL_PixelFormat *dfmt)
{
	int i;
	u16 *s = src;
	unsigned alpha = dfmt->Amask ? 255 : 0;
	for (i=0; i<n; i++)
	{
		unsigned r, g, b;
		RGB_FROM_PIXEL(*s, sfmt, r, g, b);
		PIXEL_FROM_RGBA(*dst, dfmt, r, g, b, alpha);
		s++;
		dst++;
	}
	return (n * 2);
}



/* encode 32bpp rgb + a into 32bpp G0RAB format for blitting into 565 */
static int copy_transl_565(
	void *dst, u32 *src,
	int n,
	SDL_PixelFormat *sfmt,
	SDL_PixelFormat *dfmt)
{
	int i;
	u32 *d = dst;
	for (i=0; i<n; i++)
	{
		unsigned r, g, b, a;
		u16 pix;
		RGBA_FROM_8888(*src, sfmt, r, g, b, a);
		PIXEL_FROM_RGB(pix, dfmt, r, g, b);
		*d = ((pix & 0x7e0) << 16) | (pix & 0xf81f) | ((a << 2) & 0x7e0);
		src++;
		d++;
	}
	return n * 4;
}

/* encode 32bpp rgb + a into 32bpp G0RAB format for blitting into 555 */
static int copy_transl_555(
	void *dst, u32 *src,
	int n,
	SDL_PixelFormat *sfmt,
	SDL_PixelFormat *dfmt)
{
	int i;
	u32 *d = dst;
	for (i=0; i<n; i++)
	{
		unsigned r, g, b, a;
		u16 pix;
		RGBA_FROM_8888(*src, sfmt, r, g, b, a);
		PIXEL_FROM_RGB(pix, dfmt, r, g, b);
		*d = ((pix & 0x3e0) << 16) | (pix & 0xfc1f) | ((a << 2) & 0x3e0);
		src++;
		d++;
	}
	return n * 4;
}

/* decode translucent pixels from 32bpp GORAB to 32bpp rgb + a */
static int uncopy_transl_16(
	u32 *dst, void *src,
	int n,
	RLEDestFormat *sfmt,
	SDL_PixelFormat *dfmt)
{
	int i;
	u32 *s = src;
	for (i=0; i<n; i++)
	{
		unsigned r, g, b, a;
		u32 pix = *s++;
		a = (pix & 0x3e0) >> 2;
		pix = (pix & ~0x3e0) | pix >> 16;
		RGB_FROM_PIXEL(pix, sfmt, r, g, b);
		PIXEL_FROM_RGBA(*dst, dfmt, r, g, b, a);
		dst++;
	}
	return n * 4;
}

/* encode 32bpp rgba into 32bpp rgba, keeping alpha (dual purpose) */
static int copy_32(
	void *dst, u32 *src,
	int n,
	SDL_PixelFormat *sfmt,
	SDL_PixelFormat *dfmt)
{
	int i;
	u32 *d = dst;
	for (i=0; i<n; i++)
	{
		unsigned r, g, b, a;
		u32 pixel;
		RGBA_FROM_8888(*src, sfmt, r, g, b, a);
		PIXEL_FROM_RGB(pixel, dfmt, r, g, b);
		*d++ = pixel | a << 24;
		src++;
	}
	return n * 4;
}

/* decode 32bpp rgba into 32bpp rgba, keeping alpha (dual purpose) */
static int uncopy_32(
	u32 *dst, void *src,
	int n,
	RLEDestFormat *sfmt,
	SDL_PixelFormat *dfmt)
{
	int i;
	u32 *s = src;
	for (i=0; i<n; i++)
	{
		unsigned r, g, b, a;
		u32 pixel = *s++;
		RGB_FROM_PIXEL(pixel, sfmt, r, g, b);
		a = pixel >> 24;
		PIXEL_FROM_RGBA(*dst, dfmt, r, g, b, a);
		dst++;
	}
	return n * 4;
}

#define ISOPAQUE(pixel, fmt) ((((pixel) & fmt->Amask) >> fmt->Ashift) == 255)

#define ISTRANSL(pixel, fmt)	\
	((unsigned)((((pixel) & fmt->Amask) >> fmt->Ashift) - 1U) < 254U)

/* convert surface to be quickly alpha-blittable onto dest, if possible */
static int RLEAlphaSurface(SDL_Surface *surface)
{
	SDL_Surface *dest;
	SDL_PixelFormat *df;
	int maxsize = 0;
	int max_opaque_run;
	int max_transl_run = 65535;
	unsigned masksum;
	u8 *rlebuf, *dst;
	int (*copy_opaque)(void *, u32 *, int, SDL_PixelFormat *, SDL_PixelFormat *);
	int (*copy_transl)(void *, u32 *, int, SDL_PixelFormat *, SDL_PixelFormat *);

	dest = surface->map->dst;
	if (!dest)
		return (-1);
	df = dest->format;
	if (surface->format->BitsPerPixel != 32)
		return (-1);		  /* only 32bpp source supported */

	/* find out whether the destination is one we support,
	   and determine the max size of the encoded result */
	masksum = df->Rmask | df->Gmask | df->Bmask;
	switch(df->BytesPerPixel)
	{
	case 2:
		/* 16bpp: only support 565 and 555 formats */
		switch(masksum)
		{
		case 0xffff:
			if (df->Gmask == 0x07e0
			   || df->Rmask == 0x07e0 || df->Bmask == 0x07e0)
			{
				copy_opaque = copy_opaque_16;
				copy_transl = copy_transl_565;
			}
			else
			{	return (-1);	}
			break;
		case 0x7fff:
			if (df->Gmask == 0x03e0
			   || df->Rmask == 0x03e0 || df->Bmask == 0x03e0)
			{
				copy_opaque = copy_opaque_16;
				copy_transl = copy_transl_555;
			}
			else
			{	return (-1);	}
			break;
		default:
			return (-1);
		}
		max_opaque_run = 255;	/* runs stored as bytes */

		/* worst case is alternating opaque and translucent pixels,
		   with room for alignment padding between lines */
		maxsize = surface->h * (2 + (4 + 2) * (surface->w + 1)) + 2;
		break;
	case 4:
		if (masksum != 0x00ffffff)
			return (-1);	  /* requires unused high byte */
		copy_opaque = copy_32;
		copy_transl = copy_32;
		max_opaque_run = 255;	/* runs stored as short ints */

		/* worst case is alternating opaque and translucent pixels */
		maxsize = surface->h * 2 * 4 * (surface->w + 1) + 4;
		break;
	default:
		return (-1);		  /* anything else unsupported right now */
	}
	maxsize += sizeof(RLEDestFormat);
	rlebuf = (u8 *)malloc(maxsize);
	if (!rlebuf)
	{
		SDL_OutOfMemory_bbb();
		return (-1);
	}
	{
		/* save the destination format so we can undo the encoding later */
		RLEDestFormat *r = (RLEDestFormat *)rlebuf;
		r->BytesPerPixel = df->BytesPerPixel;
		r->Rloss = df->Rloss;
		r->Gloss = df->Gloss;
		r->Bloss = df->Bloss;
		r->Rshift = df->Rshift;
		r->Gshift = df->Gshift;
		r->Bshift = df->Bshift;
		r->Ashift = df->Ashift;
		r->Rmask = df->Rmask;
		r->Gmask = df->Gmask;
		r->Bmask = df->Bmask;
		r->Amask = df->Amask;
	}
	dst = rlebuf + sizeof(RLEDestFormat);

	/* Do the actual encoding */
	{
		int x, y;
		int h = surface->h, w = surface->w;
		SDL_PixelFormat *sf = surface->format;
		u32 *src = (u32 *)surface->pixels;
		u8 *lastline = dst;	/* end of last non-blank line */

		/* opaque counts are 8 or 16 bits, depending on target depth */
		#define ADD_OPAQUE_COUNTS(n, m) 	\
		if (df->BytesPerPixel == 4) 	{		\
			((u16 *)dst)[0] = n; 	\
			((u16 *)dst)[1] = m; 	\
			dst += 4;			\
		} else {				\
			dst[0] = n; 		\
			dst[1] = m; 		\
			dst += 2;			\
		}

		/* translucent counts are always 16 bit */
		#define ADD_TRANSL_COUNTS(n, m)\
		(((u16 *)dst)[0] = n, ((u16 *)dst)[1] = m, dst += 4)

		for (y = 0; y < h; y++)
		{
			int runstart, skipstart;
			int blankline = 0;
			/* First encode all opaque pixels of a scan line */
			x = 0;
			do {
				int run, skip, len;
				skipstart = x;
				while (x < w && !ISOPAQUE(src[x], sf))
					x++;
				runstart = x;
				while (x < w && ISOPAQUE(src[x], sf))
					x++;
				skip = runstart - skipstart;
				if (skip == w)
					blankline = 1;
				run = x - runstart;
				while (skip > max_opaque_run)
				{
					ADD_OPAQUE_COUNTS(max_opaque_run, 0);
					skip -= max_opaque_run;
				}
				len = MIN(run, max_opaque_run);
				ADD_OPAQUE_COUNTS(skip, len);
				dst += copy_opaque(dst, src + runstart, len, sf, df);
				runstart += len;
				run -= len;
				while (run)
				{
					len = MIN(run, max_opaque_run);
					ADD_OPAQUE_COUNTS(0, len);
					dst += copy_opaque(dst, src + runstart, len, sf, df);
					runstart += len;
					run -= len;
				}
			} while (x < w);
			/* Make sure the next output address is 32-bit aligned */
			dst += (unsigned long)dst & 2;
			/* Next, encode all translucent pixels of the same scan line */
			x = 0;
			do {
				int run, skip, len;
				skipstart = x;
				while (x < w && !ISTRANSL(src[x], sf))
					x++;
				runstart = x;
				while (x < w && ISTRANSL(src[x], sf))
					x++;
				skip = runstart - skipstart;
				blankline &= (skip == w);
				run = x - runstart;
				while (skip > max_transl_run)
				{
					ADD_TRANSL_COUNTS(max_transl_run, 0);
					skip -= max_transl_run;
				}
				len = MIN(run, max_transl_run);
				ADD_TRANSL_COUNTS(skip, len);
				dst += copy_transl(dst, src + runstart, len, sf, df);
				runstart += len;
				run -= len;
				while (run)
				{
					len = MIN(run, max_transl_run);
					ADD_TRANSL_COUNTS(0, len);
					dst += copy_transl(dst, src + runstart, len, sf, df);
					runstart += len;
					run -= len;
				}
				if (!blankline)
					lastline = dst;
			} while (x < w);
			src += surface->pitch >> 2;
		}
		dst = lastline;  /* back up past trailing blank lines */
		ADD_OPAQUE_COUNTS(0, 0);
	}

	#undef ADD_OPAQUE_COUNTS
	#undef ADD_TRANSL_COUNTS

	/* Now that we have it encoded, release the original pixels */
	if ((surface->flags & SDL_PREALLOC) != SDL_PREALLOC
	   && (surface->flags & SDL_HWSURFACE) != SDL_HWSURFACE)
	{
		free( surface->pixels );
		surface->pixels = NULL;
	}

	/* realloc the buffer to release unused memory */
	{
		u8 *p = realloc(rlebuf, dst - rlebuf);
		if (!p)
			p = rlebuf;
		surface->map->sw_data->aux_data = p;
	}
	return (0);
}

static u32 getpix_8(u8 *srcbuf)
{
	return *srcbuf;
}

static u32 getpix_16(u8 *srcbuf)
{
	return *(u16 *)srcbuf;
}

static u32 getpix_24(u8 *srcbuf)
{
	if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
		return srcbuf[0] + (srcbuf[1] << 8) + (srcbuf[2] << 16);
	else
		return (srcbuf[0] << 16) + (srcbuf[1] << 8) + srcbuf[2];
}

static u32 getpix_32(u8 *srcbuf)
{
	return *(u32 *)srcbuf;
}

typedef u32 (*getpix_func)(u8 *);


static int RLEColorkeySurface(SDL_Surface *surface)
{
	u8 *rlebuf, *dst;
	int maxn;
	int y;
	u8 *srcbuf, *curbuf, *lastline;
	int maxsize = 0;
	int skip, run;
	int bpp = surface->format->BytesPerPixel;
	getpix_func getpix;
	u32 ckey, rgbmask;
	int w, h;

	/* calculate the worst case size for the compressed surface */
	switch(bpp)
	{
	case 1:
		/* worst case is alternating opaque and transparent pixels,
		   starting with an opaque pixel */
		maxsize = surface->h * 3 * (surface->w / 2 + 1) + 2;
		break;
	case 2:
	case 3:
		/* worst case is solid runs, at most 255 pixels wide */
		maxsize = surface->h * (2 * (surface->w / 255 + 1) + surface->w * bpp) + 2;
		break;
	case 4:
		/* worst case is solid runs, at most 65535 pixels wide */
		maxsize = surface->h * (4 * (surface->w / 65535 + 1) + surface->w * 4) + 4;
		break;
	}

	rlebuf = (u8 *)malloc(maxsize);
	if ( rlebuf == NULL )
	{
		SDL_OutOfMemory_bbb();
		return (-1);
	}

	/* Set up the conversion */
	srcbuf		= (u8 *)surface->pixels;
	curbuf		= srcbuf;
	maxn		= bpp == 4 ? 65535 : 255;
	skip		= run = 0;
	dst 		= rlebuf;
	rgbmask 	= ~surface->format->Amask;
	ckey		= surface->format->colorkey & rgbmask;
	lastline	= dst;
	{
		static getpix_func getpixes[4] =
		{
			getpix_8,
			getpix_16,
			getpix_24,
			getpix_32
		};
		getpix = getpixes[bpp - 1];
	}
	w = surface->w;
	h = surface->h;

#define ADD_COUNTS(n, m)			\
	if (bpp == 4)					\
	{								\
		((u16 *)dst)[0] = n; 	\
		((u16 *)dst)[1] = m; 	\
		dst += 4;					\
	}						\
	else					\
	{						\
		dst[0] = n; 		\
		dst[1] = m; 		\
		dst += 2;			\
	}

	for (y=0; y<h; y++)
	{
		int x = 0;
		int blankline = 0;
		do {
			int run, skip, len;
			int runstart;
			int skipstart = x;

			/* find run of transparent, then opaque pixels */
			while (x < w && (getpix(srcbuf + x * bpp) & rgbmask) == ckey)
			{	x++;	}
			runstart = x;
			while (x < w && (getpix(srcbuf + x * bpp) & rgbmask) != ckey)
			{	x++;	}
			skip = runstart - skipstart;
			if (skip == w)
			{	blankline = 1;	}
			run = x - runstart;

			/* encode segment */
			while (skip > maxn)
			{
				ADD_COUNTS(maxn, 0);
				skip -= maxn;
			}
			len = MIN(run, maxn);
			ADD_COUNTS(skip, len);
			memcpy(dst, srcbuf + runstart * bpp, len * bpp);
			dst += len * bpp;
			run -= len;
			runstart += len;
			while (run)
			{
				len = MIN(run, maxn);
				ADD_COUNTS(0, len);
				memcpy(dst, srcbuf + runstart * bpp, len * bpp);
				dst += len * bpp;
				runstart += len;
				run -= len;
			}
			if (!blankline)
			{	lastline = dst; 	}
		} while (x < w);
		srcbuf += surface->pitch;
	}
	dst = lastline;  /* back up bast trailing blank lines */
	ADD_COUNTS(0, 0);
	#undef ADD_COUNTS

	/* Now that we have it encoded, release the original pixels */
	if ((surface->flags & SDL_PREALLOC) != SDL_PREALLOC
		&& (surface->flags & SDL_HWSURFACE) != SDL_HWSURFACE)
	{
		free( surface->pixels );
		surface->pixels = NULL;
	}

	/* realloc the buffer to release unused memory */
	{
		/* If realloc returns NULL, the original block is left intact */
		u8 *p = realloc(rlebuf, dst - rlebuf);
		if (!p)
		p = rlebuf;
		surface->map->sw_data->aux_data = p;
	}
	return (0);
}

int SDL_RLESurface(SDL_Surface *surface)
{
	int retcode;
	/* Clear any previous RLE conversion */
	if ( (surface->flags & SDL_RLEACCEL) == SDL_RLEACCEL )
	{
		SDL_UnRLESurface(surface, 1);
	}

	/* We don't support RLE encoding of bitmaps */
	if ( surface->format->BitsPerPixel < 8 )
	{
		return (-1);
	}

	/* Lock the surface if it's in hardware */
	if ( SDL_MUSTLOCK(surface) )
	{
		if ( SDL_LockSurface(surface) < 0 )
		{
			return (-1);
		}
	}

	/* Encode */
	if ((surface->flags & SDL_SRCCOLORKEY) == SDL_SRCCOLORKEY)
	{
		retcode = RLEColorkeySurface(surface);
	}
	else
	{
		if ((surface->flags & SDL_SRCALPHA) == SDL_SRCALPHA
		   && surface->format->Amask != 0)
		{	retcode = RLEAlphaSurface(surface); 	}
		else
		{	retcode = -1;	}	/* no RLE for per-surface alpha sans ckey */
	}

	/* Unlock the surface if it's in hardware */
	if ( SDL_MUSTLOCK(surface) )
	{
		SDL_UnlockSurface(surface);
	}
	if (retcode < 0)
	{	return (-1);
	}
	/* The surface is now accelerated */
	surface->flags |= SDL_RLEACCEL;
	return (0);
}

/*
 * Un-RLE a surface with pixel alpha
 * This may not give back exactly the image before RLE-encoding; all
 * completely transparent pixels will be lost, and colour and alpha depth
 * may have been reduced (when encoding for 16bpp targets).
 */
static SDL_bool UnRLEAlpha(SDL_Surface *surface)
{
	u8 *srcbuf;
	u32 *dst;
	SDL_PixelFormat *sf = surface->format;
	RLEDestFormat *df = surface->map->sw_data->aux_data;
	int (*uncopy_opaque)(u32 *, void *, int, RLEDestFormat *, SDL_PixelFormat *);
	int (*uncopy_transl)(u32 *, void *, int, RLEDestFormat *, SDL_PixelFormat *);
	int w = surface->w;
	int bpp = df->BytesPerPixel;

	if (bpp == 2)
	{
		uncopy_opaque = uncopy_opaque_16;
		uncopy_transl = uncopy_transl_16;
	}
	else
	{
		uncopy_opaque = uncopy_transl = uncopy_32;
	}

	surface->pixels = malloc(surface->h * surface->pitch);
	if ( !surface->pixels )
	{
		return (SDL_FALSE);
	}
	/* fill background with transparent pixels */
	memset(surface->pixels, 0, surface->h * surface->pitch);

	dst = surface->pixels;
	srcbuf = (u8 *)(df + 1);
	for (;;)
	{
		/* copy opaque pixels */
		int ofs = 0;
		do {
			unsigned run;
			if (bpp == 2)
			{
				ofs += srcbuf[0];
				run = srcbuf[1];
				srcbuf += 2;
			}
			else
			{
				ofs += ((u16 *)srcbuf)[0];
				run = ((u16 *)srcbuf)[1];
				srcbuf += 4;
			}
			if (run)
			{
				srcbuf += uncopy_opaque(dst + ofs, srcbuf, run, df, sf);
				ofs += run;
			}
			else
			if (!ofs)
			{
				return (SDL_TRUE);
			}
		} while (ofs < w);
		/* skip padding if needed */
		if (bpp == 2)
		{	srcbuf += (unsigned long)srcbuf & 2;
		}
		/* copy translucent pixels */
		ofs = 0;
		do {
			unsigned run;
			ofs += ((u16 *)srcbuf)[0];
			run = ((u16 *)srcbuf)[1];
			srcbuf += 4;
			if (run)
			{
				srcbuf += uncopy_transl(dst + ofs, srcbuf, run, df, sf);
				ofs += run;
			}
		} while (ofs < w);
		dst += surface->pitch >> 2;
	}
	/* Make the compiler happy */
	return (SDL_TRUE);
}

void SDL_UnRLESurface(SDL_Surface *surface, int recode)
{
	if ( (surface->flags & SDL_RLEACCEL) == SDL_RLEACCEL )
	{
		surface->flags &= ~SDL_RLEACCEL;
		if (recode && (surface->flags & SDL_PREALLOC) != SDL_PREALLOC
		   && (surface->flags & SDL_HWSURFACE) != SDL_HWSURFACE)
		{
			if ((surface->flags & SDL_SRCCOLORKEY) == SDL_SRCCOLORKEY)
			{
				SDL_Rect full;
				unsigned alpha_flag;
				/* re-create the original surface */
				surface->pixels = malloc(surface->h * surface->pitch);
				if ( !surface->pixels )
				{
					/* Oh crap... */
					surface->flags |= SDL_RLEACCEL;
					return;
				}
				/* fill it with the background colour */
				SDL_FillRect(surface, NULL, surface->format->colorkey);
				/* now render the encoded surface */
				full.x = full.y = 0;
				full.w = surface->w;
				full.h = surface->h;
				alpha_flag = surface->flags & SDL_SRCALPHA;
				surface->flags &= ~SDL_SRCALPHA; /* opaque blit */
				SDL_RLEBlit(surface, &full, surface, &full);
				surface->flags |= alpha_flag;
			}
			else
			{
				if ( !UnRLEAlpha(surface) )
				{
					/* Oh crap... */
					surface->flags |= SDL_RLEACCEL;
					return;
				}
			}
		}
		if ( surface->map && surface->map->sw_data->aux_data )
		{
			free(surface->map->sw_data->aux_data);
			surface->map->sw_data->aux_data = NULL;
		}
	}
}
