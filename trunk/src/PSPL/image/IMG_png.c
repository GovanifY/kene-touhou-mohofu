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

/* $Id: IMG_png.c,v 1.11 2004/05/16 18:16:58 slouken Exp $ */

/* This is a PNG image file loading framework */

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#include "PSP_SDL_image.h"

#ifdef LOAD_PNG

#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb( ... )
#endif

/*----------------------------------------------------------------------
		File: SDL_png.c
	 Purpose: A PNG loader and saver for the SDL library
	Revision:
  Created by: Philippe Lavoie			(2 November 1998)
			  lavoie@zeus.genie.uottawa.ca
 Modified by:

 Copyright notice:
			Copyright (C) 1998 Philippe Lavoie

			This library is free software; you can redistribute it and/or
			modify it under the terms of the GNU Library General Public
			License as published by the Free Software Foundation; either
			version 2 of the License, or (at your option) any later version.

			This library is distributed in the hope that it will be useful,
			but WITHOUT ANY WARRANTY; without even the implied warranty of
			MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the GNU
			Library General Public License for more details.

			You should have received a copy of the GNU Library General Public
			License along with this library; if not, write to the Free
			Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

	Comments: The load and save routine are basically the ones you can find
			 in the example.c file from the libpng distribution.

  Changes:
	5/17/99 - Modified to use the new SDL data sources - Sam Lantinga

---------------------------------------------------------------------- */

//#include "./../include/PSPL_endian.h"//#include <SDL/SDL_endian.h>//#include "SD L_endian.h"

#include "./../../libpng/my_png.h"//#include <png.h>

//#define PNG_BYTES_TO_CHECK08	(4)
#define PNG_BYTES_TO_CHECK08	(8)

/* See if an image is contained in a data source */
int IMG_isPNG(SDL_RWops *src)
{
	unsigned char buf[PNG_BYTES_TO_CHECK08];
	/* Read in the signature bytes */
	if (SDL_RWread(src, buf, 1, PNG_BYTES_TO_CHECK08) != PNG_BYTES_TO_CHECK08)
	{	return (0); 	}
	/* Compare the first PNG_BYTES_TO_CHECK08 bytes of the signature. */
//	return ( !png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK08));
	return ( !png_sig_cmp08(buf/*, (png_size_t)0, PNG_BYTES_TO_CHECK08*/));
}

/* read callback for libpng. */
/* Load a PNG type image from an SDL datasource */
static void png_read_data(png_structp ctx, png_bytep area, png_size_t size)
{
	SDL_RWops *src;
	src = (SDL_RWops *)png_get_io_ptr(ctx);
	SDL_RWread(src, area, size, 1);
}

/* Read the entire image.  If the image has an alpha channel or a tRNS
 * chunk, and you have called png_handle_alpha()[*], you will need to
 * initialize the image to the current image that PNG will be overlaying.
 * We set the num_rows again here, in case it was incorrectly set in
 * png_read_start_row() by a call to png_read_update_info() or
 * png_start_read_image() if png_set_interlace_handling() wasn't called
 * prior to either of these functions like it should have been.  You can
 * only call this function once.  If you desire to have an image for
 * each pass of a interlaced image, use png_read_rows() instead.
 *
 * [*] png_handle_alpha() does not exist yet, as of libpng version 1.2.8
 */
static void PNGAPI s_png_read_image(png_structp png_ptr, png_bytepp image)
{
	png_debug(1, "in png_read_image\n");
	int pass;
	#ifdef PNG_READ_INTERLACING_SUPPORTED
	pass = png_set_interlace_handling(png_ptr);
	#else
	if (png_ptr->interlaced)
	{	png_error(png_ptr, "Cannot read interlaced image -- interlace handler disabled.");  	}
	pass = 1;
	#endif
	png_uint_32 image_height;
	image_height = png_ptr->height;
	png_ptr->num_rows = image_height; /* Make sure this is set correctly */
	{
		int j;
		for (j=0; j<pass; j++)
		{
			png_bytepp rp;
			rp = image;
			png_uint_32 i;
			for (i=0; i<image_height; i++)
			{
				png_read_row(png_ptr, *rp, png_bytep_NULL);
				rp++;
			}
		}
	}
}
SDL_Surface *IMG_LoadPNG_RW(SDL_RWops *src)
{
	if ( !src )
	{
		/* The error message has been set in SDL_RWFromFile */
		return (NULL);
	}
	/* Initialize the data we will clean up when we're done */
	png_structp png_ptr;				png_ptr 		= NULL;
	png_infop info_ptr; 				info_ptr 		= NULL;
	png_bytep *volatile row_pointers;	row_pointers 	= NULL;
	SDL_Surface *volatile surface;		surface 		= NULL;
	/* Create the PNG loading context structure */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		SDL_SetError_bbb("Couldn't allocate memory for PNG file or incompatible PNG dll");
		goto done;
	}
	/* Allocate/initialize the memory for image information.  REQUIRED. */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		SDL_SetError_bbb("Couldn't create image information for PNG file");
		goto done;
	}
	#if 0
	/* Set error handling if you are using setjmp/longjmp method (this is
	 * the normal method of doing things with libpng).	REQUIRED unless you
	 * set up your own error handlers in png_create_read_struct() earlier.
	 */
	if ( setjmp(png_ptr->jmpbuf) )
	{
		SDL_SetError_bbb("Error reading the PNG file.");
		goto done;
	}
	#endif // 0
	/* Set up the input control */
	png_set_read_fn(png_ptr, src, png_read_data);
	/* Read PNG header info */
	png_read_info(png_ptr, info_ptr);
//
	int color_type;
	{
		png_uint_32 height;
		{
			volatile int ckey = -1;
			png_color_16 *transv;
			{
				png_uint_32 width;
				int bit_depth;
				int interlace_type;
				//
				png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
					&color_type, &interlace_type, int_p_NULL, int_p_NULL);
				/* tell libpng to strip 16 bit/color files down to 8 bits/color */
				png_set_strip_16(png_ptr);
				/* Extract multiple pixels with bit depths of 1, 2, and 4 from a single
				 * byte into separate bytes (useful for paletted and grayscale images).
				 */
				png_set_packing(png_ptr);
				/* scale greyscale values to the range 0..255 */
				if (color_type == PNG_COLOR_TYPE_GRAY)
				{	png_set_expand(png_ptr);}
				/* For images with a single "transparent colour", set colour key;
					if more than one index has transparency, or if partially transparent
					entries exist, use full alpha channel */
				if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
				{
					int num_trans;
					u8 *trans;
					png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &transv);
					if (color_type == PNG_COLOR_TYPE_PALETTE)
					{
						/* Check if all tRNS entries are opaque except one */
						int i;
						int t = -1;
						for (i=0; i<num_trans; i++)
						{
							if (0==trans[i])
							{
								if (t >= 0)
								{	break;}
								t = i;
							}
							else
							if (trans[i] != 255)
							{	break;}
						}
						//
						if (i == num_trans) 	/* exactly one transparent index */
						{
							ckey = t;
						}
						else	/* more than one transparent index, or translucency */
						{
							png_set_expand(png_ptr);
						}
					}
					else
					{	ckey = 0; }/* actual value will be set later */
				}
				if ( color_type == PNG_COLOR_TYPE_GRAY_ALPHA )
				{	png_set_gray_to_rgb(png_ptr);	}
				png_read_update_info(png_ptr, info_ptr);
				png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
					&color_type, &interlace_type, int_p_NULL, int_p_NULL);
				/* Allocate the SDL surface to hold the image */
				{
					u32 Rmask;
					u32 Gmask;
					u32 Bmask;
					u32 Amask;
					Rmask = Gmask = Bmask = Amask = 0 ;
					if ( color_type != PNG_COLOR_TYPE_PALETTE )
					{
						if ( SDL_BYTEORDER == SDL_LIL_ENDIAN )
						{
							Rmask = 0x000000ff;
							Gmask = 0x0000ff00;
							Bmask = 0x00ff0000;
							Amask = (info_ptr->channels == 4) ? 0xff000000 : 0;
						}
						else
						{
							int s = (info_ptr->channels == 4) ? 0 : 8;
							Rmask = 0xff000000 >> s;
							Gmask = 0x00ff0000 >> s;
							Bmask = 0x0000ff00 >> s;
							Amask = 0x000000ff >> s;
						}
					}
					surface = SDL_AllocSurface(SDL_SWSURFACE, width, height,
						bit_depth*info_ptr->channels, Rmask,Gmask,Bmask,Amask);
				}
			}
			if ( surface == NULL )
			{
				SDL_SetError_bbb("Out of memory");
				goto done;
			}
			if (ckey != -1)
			{
				if (color_type != PNG_COLOR_TYPE_PALETTE)
				{
					/* FIXME: Should these be truncated or shifted down? */
					ckey = SDL_MapRGB(surface->format,
						(u8)transv->red,
						(u8)transv->green,
						(u8)transv->blue);
				}
				SDL_SetColorKey(surface, SDL_SRCCOLORKEY, ckey);
			}
		}
		/* Create the array of pointers to image data */
		row_pointers = (png_bytep*) malloc(sizeof(png_bytep)*height);
		if ( (row_pointers == NULL) )
		{
			SDL_SetError_bbb("Out of memory");
			SDL_FreeSurface(surface);
			surface = NULL;
			goto done;
		}
		{	int row;
			for (row=0; row<((int)height); row++)
			{
				row_pointers[row] = (png_bytep)
					(u8 *)surface->pixels + row*surface->pitch;
			}
		}
	}
	/* Read the entire image in one go */
	s_png_read_image(png_ptr, row_pointers);
	// 1783346
	/* and we're done!	(png_read_end() can be omitted if no processing of
	 * post-IDAT text/time/etc. is desired)
	 * In some cases it can't read PNG's created by some popular programs (ACDSEE),
	 * we do not want to process comments, so we omit png_read_end

	png_read_end(png_ptr, info_ptr);
	*/

	/* Load the palette, if any */
	{
		SDL_Palette *palette;
		palette = surface->format->palette;
		if ( palette )
		{
			int i;
			if (color_type == PNG_COLOR_TYPE_GRAY)
			{
				palette->ncolors = 256;
				for (i=0; i<256; i++)
				{
					palette->colors[i].r = i;
					palette->colors[i].g = i;
					palette->colors[i].b = i;
				}
			}
			else
			if (info_ptr->num_palette > 0 )
			{
				palette->ncolors = info_ptr->num_palette;
				for (i=0; i<info_ptr->num_palette; i++)
				{
					palette->colors[i].b = info_ptr->palette[i].blue;
					palette->colors[i].g = info_ptr->palette[i].green;
					palette->colors[i].r = info_ptr->palette[i].red;
				}
			}
		}
	}
done:	/* Clean up and return */
	png_destroy_read_struct(&png_ptr, (info_ptr ? &info_ptr : png_infopp_NULL), png_infopp_NULL);
	if ( row_pointers )
	{
		free(row_pointers);
	}
	return (surface);
//error_and_free:
}

#else/*(アンサポートの場合)*/

/*(アンサポートの場合はリンクしないので実質要らない)*/
//	/* See if an image is contained in a data source */
//	int IMG_isPNG(SDL_RWops *src)
//	{
//		return (0);/*(失敗、このファイルはpngではない)*/
//	}
//	/* Load a PNG type image from an SDL datasource */
//	SDL_Surface *IMG_LoadPNG_RW(SDL_RWops *src)
//	{
//		return (NULL);/*(失敗、このファイルは読み込めなかった)*/
//	}

#endif /* LOAD_PNG */

#if (0) //ndef PNG_NO_SEQUENTIAL_READ_SUPPORTED
/*
	png 読み込み(公式サンプル)
	--------------------------
	取り敢えず使ってないけど、公式サンプルなので残しとく。

*/
#if defined(PNG_INFO_IMAGE_SUPPORTED)
void PNGAPI png_read_png(
	png_structp png_ptr,
	png_infop info_ptr,
	int transforms,
	voidp params)
{

	#if defined(PNG_READ_INVERT_ALPHA_SUPPORTED)
	/* invert the alpha channel from opacity to transparency
	 */
	if (transforms & PNG_TRANSFORM_INVERT_ALPHA)
	{	 png_set_invert_alpha(png_ptr); 	}
	#endif

	/* png_read_info() gives us all of the information from the
	 * PNG file before the first IDAT (image data chunk).
	 */
	png_read_info(png_ptr, info_ptr);
	if (info_ptr->height > PNG_UINT_32_MAX / png_sizeof(png_bytep))
	{	png_error(png_ptr,"Image is too high to process with png_read_png()");	}

	/* -------------- image transformations start here ------------------- */

	#if defined(PNG_READ_16_TO_8_SUPPORTED)
	/* tell libpng to strip 16 bit/color files down to 8 bits per color
	 */
	if (transforms & PNG_TRANSFORM_STRIP_16)
	{	 png_set_strip_16(png_ptr);}
	#endif

	#if defined(PNG_READ_STRIP_ALPHA_SUPPORTED)
	/* Strip alpha bytes from the input data without combining with
	 * the background (not recommended).
	 */
	if (transforms & PNG_TRANSFORM_STRIP_ALPHA)
	{	 png_set_strip_alpha(png_ptr);}
	#endif

	#if defined(PNG_READ_PACK_SUPPORTED) && !defined(PNG_READ_EXPAND_SUPPORTED)
	/* Extract multiple pixels with bit depths of 1, 2, or 4 from a single
	 * byte into separate bytes (useful for paletted and grayscale images).
	 */
	if (transforms & PNG_TRANSFORM_PACKING)
	{	 png_set_packing(png_ptr);	}
	#endif

	#if defined(PNG_READ_PACKSWAP_SUPPORTED)
	/* Change the order of packed pixels to least significant bit first
	 * (not useful if you are using png_set_packing).
	 */
	if (transforms & PNG_TRANSFORM_PACKSWAP)
	{	 png_set_packswap(png_ptr); 	}
	#endif

	#if defined(PNG_READ_EXPAND_SUPPORTED)
	/* Expand paletted colors into true RGB triplets
	 * Expand grayscale images to full 8 bits from 1, 2, or 4 bits/pixel
	 * Expand paletted or RGB images with transparency to full alpha
	 * channels so the data will be available as RGBA quartets.
	 */
	if (transforms & PNG_TRANSFORM_EXPAND)
	{	 if ((png_ptr->bit_depth < 8) ||
			  (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE) ||
			  (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)))
		{	png_set_expand(png_ptr);}
	}
	#endif

	/* We don't handle background color or gamma transformation or dithering.
	 */

	#if defined(PNG_READ_INVERT_SUPPORTED)
	/* invert monochrome files to have 0 as white and 1 as black
	 */
	if (transforms & PNG_TRANSFORM_INVERT_MONO)
	{	png_set_invert_mono(png_ptr);}
	#endif

	#if defined(PNG_READ_SHIFT_SUPPORTED)
	/* If you want to shift the pixel values from the range [0,255] or
	 * [0,65535] to the original [0,7] or [0,31], or whatever range the
	 * colors were originally in:
	 */
	if ((transforms & PNG_TRANSFORM_SHIFT)
		 && png_get_valid(png_ptr, info_ptr, PNG_INFO_sBIT))
	{
		png_color_8p sig_bit;
		png_get_sBIT(png_ptr, info_ptr, &sig_bit);
		png_set_shift(png_ptr, sig_bit);
	}
	#endif

	#if defined(PNG_READ_BGR_SUPPORTED)
	/* flip the RGB pixels to BGR (or RGBA to BGRA)
	 */
	if (transforms & PNG_TRANSFORM_BGR)
	{	png_set_bgr(png_ptr);}
	#endif

	#if defined(PNG_READ_SWAP_ALPHA_SUPPORTED)
	/* swap the RGBA or GA data to ARGB or AG (or BGRA to ABGR)
	 */
	if (transforms & PNG_TRANSFORM_SWAP_ALPHA)
	{	png_set_swap_alpha(png_ptr);}
	#endif

	#if defined(PNG_READ_SWAP_SUPPORTED)
	/* swap bytes of 16 bit files to least significant byte first
	 */
	if (transforms & PNG_TRANSFORM_SWAP_ENDIAN)
	{	png_set_swap(png_ptr);}
	#endif

	/* We don't handle adding filler bytes */

	/* Optional call to gamma correct and add the background to the palette
	 * and update info structure.  REQUIRED if you are expecting libpng to
	 * update the palette for you (i.e., you selected such a transform above).
	 */
	png_read_update_info(png_ptr, info_ptr);

	/* -------------- image transformations end here ------------------- */

	#ifdef PNG_FREE_ME_SUPPORTED
	png_free_data(png_ptr, info_ptr, PNG_FREE_ROWS, 0);
	#endif
	if (info_ptr->row_pointers == NULL)
	{
		{
			info_ptr->row_pointers = (png_bytepp)
				png_malloc(png_ptr, info_ptr->height * png_sizeof(png_bytep));
			#ifdef PNG_FREE_ME_SUPPORTED
			info_ptr->free_me |= PNG_FREE_ROWS;
			#endif
		}
		{
			int row;
			for (row=0; row<((int)info_ptr->height); row++)
			{
				info_ptr->row_pointers[row] = (png_bytep)
					png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
			}
		}
	}

	png_read_image(png_ptr, info_ptr->row_pointers);
	info_ptr->valid |= PNG_INFO_IDAT;

	/* read rest of file, and get additional chunks in info_ptr - REQUIRED */
	png_read_end(png_ptr, info_ptr);

	if (transforms == 0 || params == NULL)
	{	/* quiet compiler warnings */ return;}

}
#endif
#endif /* PNG_NO_SEQUENTIAL_READ_SUPPORTED */
