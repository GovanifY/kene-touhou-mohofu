/*
	SDL_image:	An example image loading library for use with SDL
	Copyright (C) 1999-2004 Sam Lantinga

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library; if not, write to the Free
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	Sam Lantinga
	slouken@libsdl.org
*/

/* $Id: IMG_bmp.c,v 1.4 2004/08/21 13:25:56 slouken Exp $ */

/* This is a BMP image file loading framework */

#include <stdio.h>
#include <string.h>

#include "./PSP_SDL_image.h"

#ifdef LOAD_BMP

/* See if an image is contained in a data source */
int IMG_isBMP(SDL_RWops *src)
{
	char magic[2];
	int is_BMP;
	is_BMP = 0;
	SDL_RWread(src, magic, 2, 1);
//	if ()
	{
	//	if ( st rncmp(magic, "BM", 2) == 0 )
		if (
				('B'==magic[0])
			&&	('M'==magic[1])
		)
		{
			is_BMP = 1;
		}
	}
	return (is_BMP);
}

//#include <SDL/SDL_error.h>//#include "SD L_error.h"
//#include <SDL/SDL_video.h>//#include "SD L_video.h"
//#include <SDL/SDL_endian.h>//#include "SD L_endian.h"

#ifndef SDL_Error_bbb
	#define SDL_Error_bbb(aaa)
#endif

#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb( ... )
#endif

#ifndef SDL_ClearError_bbb
	#define SDL_ClearError_bbb(aaa)
#endif

/* Compression encodings for BMP files */
#ifndef BI_RGB
	#define BI_RGB			0
	#define BI_RLE8 		1
	#define BI_RLE4 		2
	#define BI_BITFIELDS	3
#endif

static int readRlePixels(SDL_Surface * surface, SDL_RWops * src, int isRle8)
{
	/*
	| Sets the surface pixels from src.  A bmp image is upside down.
	*/
	int pitch = surface->pitch;
	int height = surface->h;
	u8 * bits = (u8 *)surface->pixels + ((height-1) * pitch);
	int ofs = 0;
	u8 ch;
	u8 needsPad;

	for (;;)
	{
		SDL_RWread(src, &ch, 1, 1);//if (0== ) return (1);
		/*
		| encoded mode starts with a run length, and then a byte
		| with two colour indexes to alternate between for the run
		*/
		if ( ch )
		{
			u8 pixel;
			SDL_RWread(src, &pixel, 1, 1);//if (0==) return (1);
			if ( isRle8 )	/* 256-color bitmap, compressed */
			{
				do {
					bits[ofs++] = pixel;
				} while (--ch);
			}
			else			/* 16-color bitmap, compressed */
			{
				u8 pixel0 = (pixel >> 4);
				u8 pixel1 = (pixel & 0x0f);
				for (;;)
				{
					bits[ofs++] = pixel0;		/* even count, high nibble */
					if (!--ch) break;
					bits[ofs++] = pixel1;		/* odd count, low nibble */
					if (!--ch) break;
				}
			}
		}
		else
		{
			/*
			| A leading zero is an escape; it may signal the end of the bitmap,
			| a cursor move, or some absolute data.
			| zero tag may be absolute mode or an escape
			*/
			SDL_RWread(src, &ch, 1, 1);//if (0==) return (1);
			switch (ch)
			{
			case 0: 							/* end of line */
				ofs = 0;
				bits -= pitch;					/* go to previous */
				break;
			case 1: 							/* end of bitmap */
				return (0); 					/* success! */
			case 2: 							/* delta */
				SDL_RWread(src, &ch, 1, 1);//if (0==) return (1);
				ofs += ch;
				SDL_RWread(src, &ch, 1, 1);//if (0==) return (1);
				bits -= (ch * pitch);
				break;
			default:						/* no compression */
				if (isRle8)
				{
					needsPad = ( ch & 1 );
					do {
						SDL_RWread(src, bits + ofs++, 1, 1);//if (0==) return (1);
					} while (--ch);
				}
				else
				{
					needsPad = ( ((ch+1)>>1) & 1 ); 	/* (ch+1)>>1: bytes size */
					for (;;)
					{
						u8 pixel;
						SDL_RWread(src, &pixel, 1, 1);//if (0==) return (1);
						bits[ofs++] = (pixel >> 4);
						if (!--ch) break;
						bits[ofs++] = (pixel & 0x0f);
						if (!--ch) break;
					}
				}
				/* pad at even boundary */
				if (needsPad)
				{
					SDL_RWread(src, &ch, 1, 1);//if (0==) return (1);
				}
				break;
			}
		}
	}
}

static SDL_Surface *LoadBMP_RW (SDL_RWops *src, int freesrc)
{
	int was_error;
	long fp_offset;
	int i, pad;
	SDL_Surface *surface;
	u32 Rmask;
	u32 Gmask;
	u32 Bmask;
	SDL_Palette *palette;
	u8 *bits;
	int ExpandBMP;

	/* The Win32 BMP file header (14 bytes) */
	char   magic[2];
	u32 bfSize;
	u16 bfReserved1;
	u16 bfReserved2;
	u32 bfOffBits;

	/* The Win32 BITMAPINFOHEADER struct (40 bytes) */
	u32 biSize;
	s32 biWidth;
	s32 biHeight;
	u16 biPlanes;
	u16 biBitCount;
	u32 biCompression;
	u32 biSizeImage;
	s32 biXPelsPerMeter;
	s32 biYPelsPerMeter;
	u32 biClrUsed;
	u32 biClrImportant;

	/* Make sure we are passed a valid data source */
	surface = NULL;
	was_error = 0;
	if ( src == NULL )
	{
		was_error = 1;
		goto done;
	}

	/* Read in the BMP file header */
	fp_offset = SDL_RWtell(src);
	SDL_ClearError_bbb();
	SDL_RWread(src, magic, 1, 2);
//	if (  != 2 )
//	{
//		SDL_Error_bbb(SDL_EFREAD);
//		was_error = 1;
//		goto done;
//	}
//	if ( st rncmp(magic, "BM", 2) != 0 )
	if (
			('B'==magic[0])
		&&	('M'==magic[1])
	)
	{
		SDL_SetError_bbb("File is not a Windows BMP file");
		was_error = 1;
		goto done;
	}
	bfSize		= SDL_ReadLE32(src);
	bfReserved1 = SDL_ReadLE16(src);
	bfReserved2 = SDL_ReadLE16(src);
	bfOffBits	= SDL_ReadLE32(src);

	/* Read the Win32 BITMAPINFOHEADER */
	biSize		= SDL_ReadLE32(src);
	if ( biSize == 12 )
	{
		biWidth 		= (u32)SDL_ReadLE16(src);
		biHeight		= (u32)SDL_ReadLE16(src);
		biPlanes		= SDL_ReadLE16(src);
		biBitCount		= SDL_ReadLE16(src);
		biCompression	= BI_RGB;
		biSizeImage 	= 0;
		biXPelsPerMeter = 0;
		biYPelsPerMeter = 0;
		biClrUsed		= 0;
		biClrImportant	= 0;
	}
	else
	{
		biWidth 		= SDL_ReadLE32(src);
		biHeight		= SDL_ReadLE32(src);
		biPlanes		= SDL_ReadLE16(src);
		biBitCount		= SDL_ReadLE16(src);
		biCompression	= SDL_ReadLE32(src);
		biSizeImage 	= SDL_ReadLE32(src);
		biXPelsPerMeter = SDL_ReadLE32(src);
		biYPelsPerMeter = SDL_ReadLE32(src);
		biClrUsed		= SDL_ReadLE32(src);
		biClrImportant	= SDL_ReadLE32(src);
	}

	/* Check for read error */
	//if ( strcmp(SDL_GetError(), "") != 0 )
	//{
	//	was_error = 1;
	//	goto done;
	//}

	/* Expand 1 and 4 bit bitmaps to 8 bits per pixel */
	switch (biBitCount)
	{
	case 1:
	case 4:
		ExpandBMP = biBitCount;
		biBitCount = 8;
		break;
	default:
		ExpandBMP = 0;
		break;
	}

	/* RLE4 and RLE8 BMP compression is supported */
	Rmask = Gmask = Bmask = 0;
	switch (biCompression)
	{
	case BI_RGB:
		/* If there are no masks, use the defaults */
		if ( bfOffBits == (14+biSize) )
		{
			/* Default values for the BMP format */
			switch (biBitCount)
			{
			case 15:
			case 16:
				Rmask = 0x7C00;
				Gmask = 0x03E0;
				Bmask = 0x001F;
				break;
			case 24:
				#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				Rmask = 0x000000FF;
				Gmask = 0x0000FF00;
				Bmask = 0x00FF0000;
				break;
				#endif
			case 32:
				Rmask = 0x00FF0000;
				Gmask = 0x0000FF00;
				Bmask = 0x000000FF;
				break;
			default:
				break;
			}
			break;
		}
		/* Fall through -- read the RGB masks */
	default:
		switch (biBitCount)
		{
		case 15:
		case 16:
		case 32:
			Rmask = SDL_ReadLE32(src);
			Gmask = SDL_ReadLE32(src);
			Bmask = SDL_ReadLE32(src);
			break;
		default:
			break;
		}
		break;
	}

	/* Create a compatible surface, note that the colors are RGB ordered */
	surface = SDL_CreateRGBSurface(SDL_SWSURFACE, biWidth, biHeight, biBitCount, Rmask, Gmask, Bmask, 0);
	if ( surface == NULL )
	{
		was_error = 1;
		goto done;
	}

	/* Load the palette, if any */
	palette = (surface->format)->palette;
	if ( palette )
	{
		/*
		| guich: always use 1<<bpp b/c some bitmaps can bring wrong information
		| for colorsUsed
		*/
	/*	if ( biClrUsed == 0 ) */
		{
			biClrUsed = (1 << biBitCount);
		}
		if ( biSize == 12 )
		{
			for (i=0; i<((int)biClrUsed); i++)
			{
				SDL_RWread(src, &palette->colors[i].b, 1, 1);
				SDL_RWread(src, &palette->colors[i].g, 1, 1);
				SDL_RWread(src, &palette->colors[i].r, 1, 1);
				palette->colors[i].unused = 0;
			}
		}
		else
		{
			for (i=0; i<((int)biClrUsed); i++)
			{
				SDL_RWread(src, &palette->colors[i].b, 1, 1);
				SDL_RWread(src, &palette->colors[i].g, 1, 1);
				SDL_RWread(src, &palette->colors[i].r, 1, 1);
				SDL_RWread(src, &palette->colors[i].unused, 1, 1);
			}
		}
		palette->ncolors = biClrUsed;
	}

	/* Read the surface pixels.  Note that the bmp image is upside down */
	if ( SDL_RWseek(src, fp_offset+bfOffBits, SEEK_SET) < 0 )
	{
		SDL_Error_bbb(SDL_EFSEEK);
		was_error = 1;
		goto done;
	}
	if ((biCompression == BI_RLE4) || (biCompression == BI_RLE8))
	{
		was_error = readRlePixels(surface, src, biCompression == BI_RLE8);
		if (was_error) SDL_SetError_bbb("Error reading from BMP");
		goto done;
	}
	bits = (u8 *)surface->pixels+(surface->h*surface->pitch);
	{
		int bmpPitch;
		switch (ExpandBMP)
		{
		case 1:
			bmpPitch = (biWidth + 7) >> 3;
			pad  = (((bmpPitch)&(4-1)) ? (4-((bmpPitch)&(4-1))) : 0);
			break;
		case 4:
			bmpPitch = (biWidth + 1) >> 1;
			pad  = (((bmpPitch)&(4-1)) ? (4-((bmpPitch)&(4-1))) : 0);
			break;
		default:
			pad  = ((surface->pitch%4) ? (4-(surface->pitch%4)) : 0);
			break;
		}
	}
	while ( bits > (u8 *)surface->pixels )
	{
		bits -= surface->pitch;
		switch (ExpandBMP)
		{
		case 1:
		case 4:
			{
				u8 pixel = 0;
				int   shift = (8-ExpandBMP);
				for (i=0; i<surface->w; i++)
				{
					if ( i%(8/ExpandBMP) == 0 )
					{
						SDL_RWread(src, &pixel, 1, 1);
					//	if (0==)
					//	{
					//		SDL_SetError_bbb("Error reading from BMP");
					//		was_error = 1;
					//		goto done;
					//	}
					}
					*(bits+i) = (pixel>>shift);
					pixel <<= ExpandBMP;
				}
			}
			break;
		default:
			SDL_RWread(src, bits, 1, surface->pitch);
		//	if (  != surface->pitch )
		//	{
		//		SDL_Error_bbb(SDL_EFREAD);
		//		was_error = 1;
		//		goto done;
		//	}
			#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			/* Byte-swap the pixels if needed. Note that the 24bpp
			   case has already been taken care of above. */
			switch(biBitCount)
			{
			case 15:
			case 16:
				{
					u16 *pix = (u16 *)bits;
					for (i=0; i<(surface->w); i++)
					{	pix[i] = SDL_Swap16(pix[i]);
					}
				}
				break;
			case 32:
				{
					u32 *pix = (u32 *)bits;
					for (i=0; i<(surface->w); i++)
					{	pix[i] = SDL_Swap32(pix[i]);
					}
				}
				break;
			}
			#endif
			break;
		}
		/* Skip padding bytes, ugh */
		if ( pad )
		{
			u8 padbyte;
			for (i=0; i<pad; i++)
			{
				SDL_RWread(src, &padbyte, 1, 1);
			}
		}
	}
done:
	if ( was_error )
	{
		if ( surface )
		{
			SDL_FreeSurface(surface);
		}
		surface = NULL;
	}
	if ( (freesrc) && (src) )
	{
		SDL_RWclose(src);
	}
	return (surface);
}

/* Load a BMP type image from an SDL datasource */
SDL_Surface *IMG_LoadBMP_RW(SDL_RWops *src)
{
	return (LoadBMP_RW(src, 0));
}

#else/*(�A���T�|�[�g�̏ꍇ)*/

/*(�A���T�|�[�g�̏ꍇ�̓����N���Ȃ��̂Ŏ����v��Ȃ�)*/
//	/* See if an image is contained in a data source */
//	int IMG_isBMP(SDL_RWops *src)
//	{
//		return (0);/*(���s�A���̃t�@�C����bmp�ł͂Ȃ�)*/
//	}
//	/* Load a BMP type image from an SDL datasource */
//	SDL_Surface *IMG_LoadBMP_RW(SDL_RWops *src)
//	{
//		return (NULL);/*(���s�A���̃t�@�C���͓ǂݍ��߂Ȃ�����)*/
//	}

#endif /* LOAD_BMP */
