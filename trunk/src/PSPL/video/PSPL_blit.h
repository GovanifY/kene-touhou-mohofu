/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2FSDL_blit.h
psp - ���r�W���� 441Subversion ���|�W�g���ꗗ: pspps2ps2wareps3ps3warepspware���r�W����:(root)/trunk/SDL/src/video/SDL_blit.h
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
 "@(#) $Id: PSPL_blit.h,v 1.11 2005/04/17 10:19:22 icculus Exp $";
#endif

#ifndef _PSPL_blit_h
#define _PSPL_blit_h

#include "./../include/PSPL_endian.h"

/* The structure passed to the low level blit functions */
typedef struct
{
	u8 *s_pixels;
	int s_width;
	int s_height;
	int s_skip;
	u8 *d_pixels;
	int d_width;
	int d_height;
	int d_skip;
	void *aux_data;
	SDL_PixelFormat *src;
	u8 *table;
	SDL_PixelFormat *dst;
} SDL_BlitInfo;

/* The type definition for the low level blit functions */
typedef void (*SDL_loblit)(SDL_BlitInfo *info);

/* This is the private info structure for software accelerated blits */
struct private_swaccel
{
	SDL_loblit blit;
	void *aux_data;
};

/* Blit mapping definition */
typedef struct SDL_BlitMap
{
	SDL_Surface *dst;
	int identity;
	u8 *table;
	SDL_blit hw_xxx_blit;
	SDL_blit sw_blit;
	struct private_hwaccel *hw_data;
	struct private_swaccel *sw_data;

	/* the version count matches the destination; mismatch indicates
	   an invalid mapping */
	unsigned int format_version;
} SDL_BlitMap;


/* Functions found in SDL_blit.c */
extern int SDL_CalculateBlit(SDL_Surface *surface);

/* Functions found in SDL_blit_{0,1,N,A}.c */
extern SDL_loblit SDL_CalculateBlit0(SDL_Surface *surface, int complex);
extern SDL_loblit SDL_CalculateBlit1(SDL_Surface *surface, int complex);
extern SDL_loblit SDL_CalculateBlitN(SDL_Surface *surface, int complex);
extern SDL_loblit SDL_CalculateAlphaBlit(SDL_Surface *surface, int complex);

/*
 * Useful macros for blitting routines
 */

#define FORMAT_EQUAL(A, B)						  \
	((A)->BitsPerPixel == (B)->BitsPerPixel 			 \
	 && ((A)->Rmask == (B)->Rmask) && ((A)->Amask == (B)->Amask))

/* Load pixel of the specified format from a buffer and get its R-G-B values */
/* FIXME: rescale values to 0..255 here? */
#define RGB_FROM_PIXEL(pixel, fmt, r, g, b) 			 \
{									   \
	r = (((pixel&fmt->Rmask)>>fmt->Rshift)<<fmt->Rloss);		\
	g = (((pixel&fmt->Gmask)>>fmt->Gshift)<<fmt->Gloss);		\
	b = (((pixel&fmt->Bmask)>>fmt->Bshift)<<fmt->Bloss);		\
}
#define RGB_FROM_RGB565(pixel, r, g, b) 				\
{													\
	r = (((pixel&0xF800)>>11)<<3);					\
	g = (((pixel&0x07E0)>>5)<<2);					\
	b = ((pixel&0x001F)<<3);							\
}
#define RGB_FROM_RGB555(pixel, r, g, b) 				\
{													\
	r = (((pixel&0x7C00)>>10)<<3);					\
	g = (((pixel&0x03E0)>>5)<<3);					\
	b = ((pixel&0x001F)<<3);							\
}
#define RGB_FROM_RGB888(pixel, r, g, b) 				\
{													\
	r = ((pixel&0xff0000)>>16); 					\
	g = ((pixel&0xff00)>>8);						\
	b = (pixel&0xff);								\
}
#define RETRIEVE_RGB_PIXEL(buf, bpp, pixel) 			\
do {											\
	switch (bpp)	{								\
		case 2: 								\
			pixel = *((u16 *)(buf)); 		\
		break;									\
												\
		case 3: {								\
			u8 *B = (u8 *)(buf);				\
			if (SDL_BYTEORDER == SDL_LIL_ENDIAN)	{			\
				pixel = B[0] + (B[1] << 8) + (B[2] << 16);	\
			} else {							\
				pixel = (B[0] << 16) + (B[1] << 8) + B[2];	\
			}								\
		}									\
		break;								\
											\
		case 4: 							\
			pixel = *((u32 *)(buf)); 		\
		break;								\
											\
		default:							\
			pixel = 0;	/* appease gcc */			\
		break;								\
	}										\
} while (0)

#define DISEMBLE_RGB(buf, bpp, fmt, pixel, r, g, b) 		\
do {											\
	switch (bpp)	{								\
		case 2: 								\
			pixel = *((u16 *)(buf)); 		\
		break;									\
												\
		case 3: {								\
			u8 *B = (u8 *)buf;					\
			if (SDL_BYTEORDER == SDL_LIL_ENDIAN)	{			\
				pixel = B[0] + (B[1] << 8) + (B[2] << 16);	\
			} else {							\
				pixel = (B[0] << 16) + (B[1] << 8) + B[2];	\
			}								\
		}									\
		break;								\
											\
		case 4: 								\
			pixel = *((u32 *)(buf)); 		\
		break;									\
												\
		default:								\
			pixel = 0;		/* prevent gcc from complaining */	\
		break;									\
	}											\
	RGB_FROM_PIXEL(pixel, fmt, r, g, b);					\
} while (0)

/* Assemble R-G-B values into a specified pixel format and store them */
#define PIXEL_FROM_RGB(pixel, fmt, r, g, b) 		\
{													\
	pixel = ((r>>fmt->Rloss)<<fmt->Rshift)| 		\
		((g>>fmt->Gloss)<<fmt->Gshift)| 			\
		((b>>fmt->Bloss)<<fmt->Bshift); 			\
}
#define RGB565_FROM_RGB(pixel, r, g, b) 				\
{														\
	pixel = ((r>>3)<<11)|((g>>2)<<5)|(b>>3);			\
}
#define RGB555_FROM_RGB(pixel, r, g, b) 				\
{														\
	pixel = ((r>>3)<<10)|((g>>3)<<5)|(b>>3);			\
}
#define RGB888_FROM_RGB(pixel, r, g, b) 				\
{														\
	pixel = (r<<16)|(g<<8)|b;							\
}
#define ASSEMBLE_RGB(buf, bpp, fmt, r, g, b)		\
{											\
	switch (bpp)	{							\
		case 2: {							\
			u16 pixel;					\
											\
			PIXEL_FROM_RGB(pixel, fmt, r, g, b);	\
			*((u16 *)(buf)) = pixel; 			\
		}									\
		break;								\
											\
		case 3: {							\
			if (SDL_BYTEORDER == SDL_LIL_ENDIAN)	{	\
				*((buf)+fmt->Rshift/8) = r; 		\
				*((buf)+fmt->Gshift/8) = g; 		\
				*((buf)+fmt->Bshift/8) = b; 		\
			} else {								\
				*((buf)+2-fmt->Rshift/8) = r;		\
				*((buf)+2-fmt->Gshift/8) = g;		\
				*((buf)+2-fmt->Bshift/8) = b;		\
			}							\
		}								\
		break;							\
										\
		case 4: {							\
			u32 pixel;					\
											\
			PIXEL_FROM_RGB(pixel, fmt, r, g, b);	\
			*((u32 *)(buf)) = pixel; 			\
		}								\
		break;							\
	}									\
}
#define ASSEMBLE_RGB_AMASK(buf, bpp, fmt, r, g, b, Amask)	\
{											\
	switch (bpp)	{							\
		case 2: {							\
			u16 *bufp;					\
			u16 pixel;					\
										\
			bufp = (u16 *)buf;				\
			PIXEL_FROM_RGB(pixel, fmt, r, g, b);	\
			*bufp = pixel | (*bufp & Amask);		\
		}								\
		break;							\
										\
		case 3: {							\
			if (SDL_BYTEORDER == SDL_LIL_ENDIAN)	{	\
				*((buf)+fmt->Rshift/8) = r; 		\
				*((buf)+fmt->Gshift/8) = g; 		\
				*((buf)+fmt->Bshift/8) = b; 		\
			} else {								\
				*((buf)+2-fmt->Rshift/8) = r;		\
				*((buf)+2-fmt->Gshift/8) = g;		\
				*((buf)+2-fmt->Bshift/8) = b;		\
			}							\
		}								\
		break;							\
										\
		case 4: {							\
			u32 *bufp;					\
			u32 pixel;					\
										\
			bufp = (u32 *)buf;				\
			PIXEL_FROM_RGB(pixel, fmt, r, g, b);		\
			*bufp = pixel | (*bufp & Amask);		\
		}								\
		break;							\
	}									\
}

/* FIXME: Should we rescale alpha into 0..255 here? */
#define RGBA_FROM_PIXEL(pixel, fmt, r, g, b, a) 			\
{										\
	r = ((pixel&fmt->Rmask)>>fmt->Rshift)<<fmt->Rloss;			\
	g = ((pixel&fmt->Gmask)>>fmt->Gshift)<<fmt->Gloss;			\
	b = ((pixel&fmt->Bmask)>>fmt->Bshift)<<fmt->Bloss;			\
	a = ((pixel&fmt->Amask)>>fmt->Ashift)<<fmt->Aloss;			\
}
#define RGBA_FROM_8888(pixel, fmt, r, g, b, a)		\
{											\
	r = (pixel&fmt->Rmask)>>fmt->Rshift;	\
	g = (pixel&fmt->Gmask)>>fmt->Gshift;	\
	b = (pixel&fmt->Bmask)>>fmt->Bshift;	\
	a = (pixel&fmt->Amask)>>fmt->Ashift;	\
}
#define RGBA_FROM_RGBA8888(pixel, r, g, b, a)	\
{												\
	r = (pixel>>24);							\
	g = ((pixel>>16)&0xff); 					\
	b = ((pixel>>8)&0xff);						\
	a = (pixel&0xff);							\
}
#define RGBA_FROM_ARGB8888(pixel, r, g, b, a)	\
{												\
	r = ((pixel>>16)&0xff); 					\
	g = ((pixel>>8)&0xff);						\
	b = (pixel&0xff);							\
	a = (pixel>>24);							\
}
#define RGBA_FROM_ABGR8888(pixel, r, g, b, a)	\
{												\
	r = (pixel&0xff);							\
	g = ((pixel>>8)&0xff);						\
	b = ((pixel>>16)&0xff); 					\
	a = (pixel>>24);							\
}
#define DISEMBLE_RGBA(buf, bpp, fmt, pixel, r, g, b, a) 	\
do {											\
	switch (bpp)	{								\
		case 2: 								\
			pixel = *((u16 *)(buf)); 		\
		break;									\
												\
		case 3: {/* FIXME: broken code (no alpha) */		\
			u8 *b = (u8 *)buf;						\
			if (SDL_BYTEORDER == SDL_LIL_ENDIAN)	{			\
				pixel = b[0] + (b[1] << 8) + (b[2] << 16);	\
			} else {							\
				pixel = (b[0] << 16) + (b[1] << 8) + b[2];	\
			}									\
		}										\
		break;									\
												\
		case 4: 								\
			pixel = *((u32 *)(buf)); 		\
		break;									\
												\
		default:								\
			pixel = 0; /* stop gcc complaints */	\
		break;									\
	}											\
	RGBA_FROM_PIXEL(pixel, fmt, r, g, b, a);	\
	pixel &= ~fmt->Amask;						\
} while (0)

/* FIXME: this isn't correct, especially for Alpha (maximum != 255) */
#define PIXEL_FROM_RGBA(pixel, fmt, r, g, b, a) 	\
{												\
	pixel = ((r>>fmt->Rloss)<<fmt->Rshift)| 		\
		((g>>fmt->Gloss)<<fmt->Gshift)| 			\
		((b>>fmt->Bloss)<<fmt->Bshift)| 			\
		((a>>fmt->Aloss)<<fmt->Ashift); 			\
}
#define ASSEMBLE_RGBA(buf, bpp, fmt, r, g, b, a)	\
{											\
	switch (bpp)	{							\
		case 2: {							\
			u16 pixel;					\
											\
			PIXEL_FROM_RGBA(pixel, fmt, r, g, b, a);	\
			*((u16 *)(buf)) = pixel; 			\
		}								\
		break;							\
										\
		case 3: { /* FIXME: broken code (no alpha) */	\
			if (SDL_BYTEORDER == SDL_LIL_ENDIAN)	{		\
				*((buf)+fmt->Rshift/8) = r; 		\
				*((buf)+fmt->Gshift/8) = g; 		\
				*((buf)+fmt->Bshift/8) = b; 		\
			} else {					\
				*((buf)+2-fmt->Rshift/8) = r;		\
				*((buf)+2-fmt->Gshift/8) = g;		\
				*((buf)+2-fmt->Bshift/8) = b;		\
			}							\
		}								\
		break;							\
										\
		case 4: {							\
			u32 pixel;					\
										\
			PIXEL_FROM_RGBA(pixel, fmt, r, g, b, a);	\
			*((u32 *)(buf)) = pixel; 			\
		}								\
		break;							\
	}									\
}

/* Blend the RGB values of two pixels based on a source alpha value */
#define ALPHA_BLEND(sR, sG, sB, A, dR, dG, dB)		\
do {							\
	dR = (((sR-dR)*(A))>>8)+dR; 		\
	dG = (((sG-dG)*(A))>>8)+dG; 		\
	dB = (((sB-dB)*(A))>>8)+dB; 		\
} while (0)

/* Blend the RGB values of two pixels based on a source alpha value */
#define ACCURATE_ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB)	\
do {							\
	unsigned tR, tG, tB, tA;	\
	tA = 255 - sA;						\
	tR = 1 + (sR * sA) + (dR * tA); 	\
	dR = (tR + (tR >> 8)) >> 8; 		\
	tG = 1 + (sG * sA) + (dG * tA); 	\
	dG = (tG + (tG >> 8)) >> 8; 		\
	tB = 1 + (sB * sA) + (dB * tA); 	\
	dB = (tB + (tB >> 8)) >> 8; 		\
} while (0)


/* This is a very useful loop for optimizing blitters */
#if defined(_MSC_VER) && (_MSC_VER == 1300)
/* There's a bug in the Visual C++ 7 optimizer when compiling this code */
#else
#define USE_DUFFS_LOOP
#endif
#ifdef USE_DUFFS_LOOP

/* 8-times unrolled loop */
#define DUFFS_LOOP8(pixel_copy_increment, width)	\
{ int n = ((width+7)>>3/*/8*/); 							\
	switch (width & 7)	{							\
	case 0: do {	pixel_copy_increment;			\
	case 7:  pixel_copy_increment;				\
	case 6:  pixel_copy_increment;				\
	case 5:  pixel_copy_increment;				\
	case 4:  pixel_copy_increment;				\
	case 3:  pixel_copy_increment;				\
	case 2:  pixel_copy_increment;				\
	case 1:  pixel_copy_increment;				\
		} while ( --n > 0 );					\
	}											\
}

/* 4-times unrolled loop */
#define DUFFS_LOOP4(pixel_copy_increment, width)	\
{ int n = ((width+3)>>2/*/4*/); 						\
	switch (width & 3)	{						\
	case 0: do {	pixel_copy_increment;			\
	case 3:  pixel_copy_increment;				\
	case 2:  pixel_copy_increment;				\
	case 1:  pixel_copy_increment;				\
		} while ( --n > 0 );					\
	}											\
}

/* 2 - times unrolled loop */
#define DUFFS_LOOP_DOUBLE2(pixel_copy_increment,		\
				double_pixel_copy_increment, width) 	\
{ int n, w = width; 								\
	if ( w & 1 )	{									\
		pixel_copy_increment;						\
		w--;										\
	}											\
	if ( w > 0 )	{							\
		n = (( w + 2)>>2/*/4*/);						\
		switch( w & 2 ) 	{						\
		case 0: do {	double_pixel_copy_increment;		\
		case 2: 	 double_pixel_copy_increment;			\
			} while ( --n > 0 );							\
		}								\
	}									\
}

/* 2 - times unrolled loop 4 pixels */
#define DUFFS_LOOP_QUATRO2(pixel_copy_increment,			\
				double_pixel_copy_increment,				\
				quatro_pixel_copy_increment, width) 		\
{ int n, w = width; 										\
	if (w & 1)	{								\
	  pixel_copy_increment; 					\
	  w--;										\
	}											\
	if (w & 2)	{								\
	  double_pixel_copy_increment;				\
	  w -= 2;									\
	}											\
	if ( w > 0 )	{								\
		n = (( w + 7 )>>3/*/8*/);						\
		switch( w & 4 ) 	{						\
		case 0: do {	quatro_pixel_copy_increment;	\
		case 4: 	 quatro_pixel_copy_increment;		\
			} while ( --n > 0 );						\
		}								\
	}									\
}

/* Use the 8-times version of the loop by default */
#define DUFFS_LOOP(pixel_copy_increment, width) 		 \
	DUFFS_LOOP8(pixel_copy_increment, width)

#else

/* Don't use Duff's device to unroll loops */
#define DUFFS_LOOP_DOUBLE2(pixel_copy_increment,			\
			 double_pixel_copy_increment, width)			\
{ int n = width;								\
	if ( n & 1 )	{								\
	pixel_copy_increment;						\
	n--;										\
	}											\
	n=n>>1; 									\
	for (; n > 0; --n)	{						\
	double_pixel_copy_increment;				\
	}											\
}

/* Don't use Duff's device to unroll loops */
#define DUFFS_LOOP_QUATRO2(pixel_copy_increment,		\
				double_pixel_copy_increment,			\
				quatro_pixel_copy_increment, width) 	\
{ int n = width;									\
	if (n & 1)	{									\
	  pixel_copy_increment; 						\
	  n--;											\
	}												\
	if (n & 2)	{									\
	  double_pixel_copy_increment;					\
	  n -= 2;										\
	}												\
	n=n>>2; 										\
	for (; n > 0; --n)	{							\
	  quatro_pixel_copy_increment;					\
	}												\
}

/* Don't use Duff's device to unroll loops */
#define DUFFS_LOOP(pixel_copy_increment, width) 	\
{ int n;											\
	for ( n=width; n > 0; --n ) 	{					\
		pixel_copy_increment;						\
	}												\
}
#define DUFFS_LOOP8(pixel_copy_increment, width)	DUFFS_LOOP(pixel_copy_increment, width)
#define DUFFS_LOOP4(pixel_copy_increment, width)	DUFFS_LOOP(pixel_copy_increment, width)

#endif /* USE_DUFFS_LOOP */

/* Prevent Visual C++ 6.0 from printing out stupid warnings */
#if defined(_MSC_VER) && (_MSC_VER >= 600)
#pragma warning(disable: 4550)
#endif

#endif /* _PSPL_blit_h */

