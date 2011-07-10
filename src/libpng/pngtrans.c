
/* pngtrans.c - transforms the data in a row (used by both readers and writers)
 *
 * libpng  1.2.8 - December 3, 2004
 * For conditions of distribution and use, see copyright notice in png.h
 * Copyright (c) 1998-2004 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 */

#define PNG_INTERNAL
#include "my_png.h"

#if defined(PNG_READ_BGR_SUPPORTED) || defined(PNG_WRITE_BGR_SUPPORTED)
/* turn on BGR-to-RGB mapping */
void PNGAPI
png_set_bgr(png_structp png_ptr)
{
	png_debug(1, "in png_set_bgr\n");
	png_ptr->transformations |= PNG_BGR;
}
#endif

#if defined(PNG_READ_SWAP_SUPPORTED) || defined(PNG_WRITE_SWAP_SUPPORTED)
/* turn on 16 bit byte swapping */
void PNGAPI
png_set_swap(png_structp png_ptr)
{
	png_debug(1, "in png_set_swap\n");
	if (png_ptr->bit_depth == 16)
		png_ptr->transformations |= PNG_SWAP_BYTES;
}
#endif

#if defined(PNG_READ_PACK_SUPPORTED) || defined(PNG_WRITE_PACK_SUPPORTED)
/* turn on pixel packing */
void PNGAPI
png_set_packing(png_structp png_ptr)
{
	png_debug(1, "in png_set_packing\n");
	if (png_ptr->bit_depth < 8)
	{
		png_ptr->transformations |= PNG_PACK;
		png_ptr->usr_bit_depth = 8;
	}
}
#endif

#if defined(PNG_READ_PACKSWAP_SUPPORTED)||defined(PNG_WRITE_PACKSWAP_SUPPORTED)
/* turn on packed pixel swapping */
void PNGAPI
png_set_packswap(png_structp png_ptr)
{
	png_debug(1, "in png_set_packswap\n");
	if (png_ptr->bit_depth < 8)
		png_ptr->transformations |= PNG_PACKSWAP;
}
#endif

#if defined(PNG_READ_SHIFT_SUPPORTED) || defined(PNG_WRITE_SHIFT_SUPPORTED)
void PNGAPI
png_set_shift(png_structp png_ptr, png_color_8p true_bits)
{
	png_debug(1, "in png_set_shift\n");
	png_ptr->transformations |= PNG_SHIFT;
	png_ptr->shift = *true_bits;
}
#endif

#if defined(PNG_READ_INTERLACING_SUPPORTED) || \
	 defined(PNG_WRITE_INTERLACING_SUPPORTED)
int PNGAPI
png_set_interlace_handling(png_structp png_ptr)
{
	png_debug(1, "in png_set_interlace handling\n");
	if (png_ptr->interlaced)
	{
		png_ptr->transformations |= PNG_INTERLACE;
		return (7);
	}

	return (1);
}
#endif

#if defined(PNG_READ_FILLER_SUPPORTED) || defined(PNG_WRITE_FILLER_SUPPORTED)
/* Add a filler byte on read, or remove a filler or alpha byte on write.
 * The filler type has changed in v0.95 to allow future 2-byte fillers
 * for 48-bit input data, as well as to avoid problems with some compilers
 * that don't like bytes as parameters.
 */
void PNGAPI
png_set_filler(png_structp png_ptr, png_uint_32 filler, int filler_loc)
{
	png_debug(1, "in png_set_filler\n");
	png_ptr->transformations |= PNG_FILLER;
	png_ptr->filler = (png_byte)filler;
	if (filler_loc == PNG_FILLER_AFTER)
		png_ptr->flags |= PNG_FLAG_FILLER_AFTER;
	else
		png_ptr->flags &= ~PNG_FLAG_FILLER_AFTER;

	/* This should probably go in the "do_read_filler" routine.
	 * I attempted to do that in libpng-1.0.1a but that caused problems
	 * so I restored it in libpng-1.0.2a
	*/

	if (png_ptr->color_type == PNG_COLOR_TYPE_RGB)
	{
		png_ptr->usr_channels = 4;
	}

	/* Also I added this in libpng-1.0.2a (what happens when we expand
	 * a less-than-8-bit grayscale to GA? */

	if (png_ptr->color_type == PNG_COLOR_TYPE_GRAY && png_ptr->bit_depth >= 8)
	{
		png_ptr->usr_channels = 2;
	}
}

#if !defined(PNG_1_0_X)
/* Added to libpng-1.2.7 */
void PNGAPI
png_set_add_alpha(png_structp png_ptr, png_uint_32 filler, int filler_loc)
{
	png_debug(1, "in png_set_add_alpha\n");
	png_set_filler(png_ptr, filler, filler_loc);
	png_ptr->transformations |= PNG_ADD_ALPHA;
}
#endif

#endif

#if defined(PNG_READ_SWAP_ALPHA_SUPPORTED) || \
	 defined(PNG_WRITE_SWAP_ALPHA_SUPPORTED)
void PNGAPI
png_set_swap_alpha(png_structp png_ptr)
{
	png_debug(1, "in png_set_swap_alpha\n");
	png_ptr->transformations |= PNG_SWAP_ALPHA;
}
#endif

#if defined(PNG_READ_INVERT_ALPHA_SUPPORTED) || \
	 defined(PNG_WRITE_INVERT_ALPHA_SUPPORTED)
void PNGAPI
png_set_invert_alpha(png_structp png_ptr)
{
	png_debug(1, "in png_set_invert_alpha\n");
	png_ptr->transformations |= PNG_INVERT_ALPHA;
}
#endif

#if defined(PNG_READ_INVERT_SUPPORTED) || defined(PNG_WRITE_INVERT_SUPPORTED)
void PNGAPI
png_set_invert_mono(png_structp png_ptr)
{
	png_debug(1, "in png_set_invert_mono\n");
	png_ptr->transformations |= PNG_INVERT_MONO;
}

/* invert monochrome grayscale data */
void /* PRIVATE */
png_do_invert(png_row_infop row_info, png_bytep row)
{
	png_debug(1, "in png_do_invert\n");
  /* This test removed from libpng version 1.0.13 and 1.2.0:
	*	 if (row_info->bit_depth == 1 &&
	*/
#if defined(PNG_USELESS_TESTS_SUPPORTED)
	if (row == NULL || row_info == NULL)
	  return;
#endif
	if (row_info->color_type == PNG_COLOR_TYPE_GRAY)
	{
		png_bytep rp = row;
		png_uint_32 i;
		png_uint_32 istop = row_info->rowbytes;

		for (i = 0; i < istop; i++)
		{
			*rp = (png_byte)(~(*rp));
			rp++;
		}
	}
	else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA &&
		row_info->bit_depth == 8)
	{
		png_bytep rp = row;
		png_uint_32 i;
		png_uint_32 istop = row_info->rowbytes;

		for (i = 0; i < istop; i+=2)
		{
			*rp = (png_byte)(~(*rp));
			rp+=2;
		}
	}
	else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA &&
		row_info->bit_depth == 16)
	{
		png_bytep rp = row;
		png_uint_32 i;
		png_uint_32 istop = row_info->rowbytes;

		for (i = 0; i < istop; i+=4)
		{
			*rp = (png_byte)(~(*rp));
			*(rp+1) = (png_byte)(~(*(rp+1)));
			rp+=4;
		}
	}
}
#endif

#if defined(PNG_READ_SWAP_SUPPORTED) || defined(PNG_WRITE_SWAP_SUPPORTED)
/* swaps byte order on 16 bit depth images */
void /* PRIVATE */
png_do_swap(png_row_infop row_info, png_bytep row)
{
	png_debug(1, "in png_do_swap\n");
	if (
#if defined(PNG_USELESS_TESTS_SUPPORTED)
		 row != NULL && row_info != NULL &&
#endif
		 row_info->bit_depth == 16)
	{
		png_bytep rp = row;
		png_uint_32 i;
		png_uint_32 istop= row_info->width * row_info->channels;

		for (i = 0; i < istop; i++, rp += 2)
		{
			png_byte t = *rp;
			*rp = *(rp + 1);
			*(rp + 1) = t;
		}
	}
}
#endif

#if defined(PNG_READ_PACKSWAP_SUPPORTED)||defined(PNG_WRITE_PACKSWAP_SUPPORTED)
static const png_byte swap_table_1bpp[256] =
{
	0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
	0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8, 0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
	0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4, 0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
	0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
	0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2, 0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
	0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea, 0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
	0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
	0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee, 0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
	0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1, 0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
	0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9, 0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
	0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5, 0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
	0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed, 0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
	0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3, 0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
	0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb, 0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
	0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7, 0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
	0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef, 0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
};

static const png_byte swap_table_2bpp[256] =
{
	0x00, 0x40, 0x80, 0xc0, 0x10, 0x50, 0x90, 0xd0, 0x20, 0x60, 0xa0, 0xe0, 0x30, 0x70, 0xb0, 0xf0,
	0x04, 0x44, 0x84, 0xc4, 0x14, 0x54, 0x94, 0xd4, 0x24, 0x64, 0xa4, 0xe4, 0x34, 0x74, 0xb4, 0xf4,
	0x08, 0x48, 0x88, 0xc8, 0x18, 0x58, 0x98, 0xd8, 0x28, 0x68, 0xa8, 0xe8, 0x38, 0x78, 0xb8, 0xf8,
	0x0c, 0x4c, 0x8c, 0xcc, 0x1c, 0x5c, 0x9c, 0xdc, 0x2c, 0x6c, 0xac, 0xec, 0x3c, 0x7c, 0xbc, 0xfc,
	0x01, 0x41, 0x81, 0xc1, 0x11, 0x51, 0x91, 0xd1, 0x21, 0x61, 0xa1, 0xe1, 0x31, 0x71, 0xb1, 0xf1,
	0x05, 0x45, 0x85, 0xc5, 0x15, 0x55, 0x95, 0xd5, 0x25, 0x65, 0xa5, 0xe5, 0x35, 0x75, 0xb5, 0xf5,
	0x09, 0x49, 0x89, 0xc9, 0x19, 0x59, 0x99, 0xd9, 0x29, 0x69, 0xa9, 0xe9, 0x39, 0x79, 0xb9, 0xf9,
	0x0d, 0x4d, 0x8d, 0xcd, 0x1d, 0x5d, 0x9d, 0xdd, 0x2d, 0x6d, 0xad, 0xed, 0x3d, 0x7d, 0xbd, 0xfd,
	0x02, 0x42, 0x82, 0xc2, 0x12, 0x52, 0x92, 0xd2, 0x22, 0x62, 0xa2, 0xe2, 0x32, 0x72, 0xb2, 0xf2,
	0x06, 0x46, 0x86, 0xc6, 0x16, 0x56, 0x96, 0xd6, 0x26, 0x66, 0xa6, 0xe6, 0x36, 0x76, 0xb6, 0xf6,
	0x0a, 0x4a, 0x8a, 0xca, 0x1a, 0x5a, 0x9a, 0xda, 0x2a, 0x6a, 0xaa, 0xea, 0x3a, 0x7a, 0xba, 0xfa,
	0x0e, 0x4e, 0x8e, 0xce, 0x1e, 0x5e, 0x9e, 0xde, 0x2e, 0x6e, 0xae, 0xee, 0x3e, 0x7e, 0xbe, 0xfe,
	0x03, 0x43, 0x83, 0xc3, 0x13, 0x53, 0x93, 0xd3, 0x23, 0x63, 0xa3, 0xe3, 0x33, 0x73, 0xb3, 0xf3,
	0x07, 0x47, 0x87, 0xc7, 0x17, 0x57, 0x97, 0xd7, 0x27, 0x67, 0xa7, 0xe7, 0x37, 0x77, 0xb7, 0xf7,
	0x0b, 0x4b, 0x8b, 0xcb, 0x1b, 0x5b, 0x9b, 0xdb, 0x2b, 0x6b, 0xab, 0xeb, 0x3b, 0x7b, 0xbb, 0xfb,
	0x0f, 0x4f, 0x8f, 0xcf, 0x1f, 0x5f, 0x9f, 0xdf, 0x2f, 0x6f, 0xaf, 0xef, 0x3f, 0x7f, 0xbf, 0xff
};

static const png_byte swap_table_4bpp[256] =
{
	0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0,
	0x01, 0x11, 0x21, 0x31, 0x41, 0x51, 0x61, 0x71, 0x81, 0x91, 0xa1, 0xb1, 0xc1, 0xd1, 0xe1, 0xf1,
	0x02, 0x12, 0x22, 0x32, 0x42, 0x52, 0x62, 0x72, 0x82, 0x92, 0xa2, 0xb2, 0xc2, 0xd2, 0xe2, 0xf2,
	0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73, 0x83, 0x93, 0xa3, 0xb3, 0xc3, 0xd3, 0xe3, 0xf3,
	0x04, 0x14, 0x24, 0x34, 0x44, 0x54, 0x64, 0x74, 0x84, 0x94, 0xa4, 0xb4, 0xc4, 0xd4, 0xe4, 0xf4,
	0x05, 0x15, 0x25, 0x35, 0x45, 0x55, 0x65, 0x75, 0x85, 0x95, 0xa5, 0xb5, 0xc5, 0xd5, 0xe5, 0xf5,
	0x06, 0x16, 0x26, 0x36, 0x46, 0x56, 0x66, 0x76, 0x86, 0x96, 0xa6, 0xb6, 0xc6, 0xd6, 0xe6, 0xf6,
	0x07, 0x17, 0x27, 0x37, 0x47, 0x57, 0x67, 0x77, 0x87, 0x97, 0xa7, 0xb7, 0xc7, 0xd7, 0xe7, 0xf7,
	0x08, 0x18, 0x28, 0x38, 0x48, 0x58, 0x68, 0x78, 0x88, 0x98, 0xa8, 0xb8, 0xc8, 0xd8, 0xe8, 0xf8,
	0x09, 0x19, 0x29, 0x39, 0x49, 0x59, 0x69, 0x79, 0x89, 0x99, 0xa9, 0xb9, 0xc9, 0xd9, 0xe9, 0xf9,
	0x0a, 0x1a, 0x2a, 0x3a, 0x4a, 0x5a, 0x6a, 0x7a, 0x8a, 0x9a, 0xaa, 0xba, 0xca, 0xda, 0xea, 0xfa,
	0x0b, 0x1b, 0x2b, 0x3b, 0x4b, 0x5b, 0x6b, 0x7b, 0x8b, 0x9b, 0xab, 0xbb, 0xcb, 0xdb, 0xeb, 0xfb,
	0x0c, 0x1c, 0x2c, 0x3c, 0x4c, 0x5c, 0x6c, 0x7c, 0x8c, 0x9c, 0xac, 0xbc, 0xcc, 0xdc, 0xec, 0xfc,
	0x0d, 0x1d, 0x2d, 0x3d, 0x4d, 0x5d, 0x6d, 0x7d, 0x8d, 0x9d, 0xad, 0xbd, 0xcd, 0xdd, 0xed, 0xfd,
	0x0e, 0x1e, 0x2e, 0x3e, 0x4e, 0x5e, 0x6e, 0x7e, 0x8e, 0x9e, 0xae, 0xbe, 0xce, 0xde, 0xee, 0xfe,
	0x0f, 0x1f, 0x2f, 0x3f, 0x4f, 0x5f, 0x6f, 0x7f, 0x8f, 0x9f, 0xaf, 0xbf, 0xcf, 0xdf, 0xef, 0xff
};

/* swaps pixel packing order within bytes */
void /* PRIVATE */
png_do_packswap(png_row_infop row_info, png_bytep row)
{
	png_debug(1, "in png_do_packswap\n");
	if (
#if defined(PNG_USELESS_TESTS_SUPPORTED)
		 row != NULL && row_info != NULL &&
#endif
		 row_info->bit_depth < 8)
	{
		png_bytep rp, end, table;
		end = row + row_info->rowbytes;
		if (row_info->bit_depth == 1)		{	table = (png_bytep)swap_table_1bpp;	}
		else if (row_info->bit_depth == 2)	{	table = (png_bytep)swap_table_2bpp;	}
		else if (row_info->bit_depth == 4)	{	table = (png_bytep)swap_table_4bpp;	}
		else
		{
			return;
		}
		for (rp = row; rp < end; rp++)
		{
			*rp = table[*rp];
		}
	}
}
#endif /* PNG_READ_PACKSWAP_SUPPORTED or PNG_WRITE_PACKSWAP_SUPPORTED */

#if defined(PNG_WRITE_FILLER_SUPPORTED) || \
	 defined(PNG_READ_STRIP_ALPHA_SUPPORTED)
/* remove filler or alpha byte(s) */
void /* PRIVATE */
png_do_strip_filler(png_row_infop row_info, png_bytep row, png_uint_32 flags)
{
	png_debug(1, "in png_do_strip_filler\n");
#if defined(PNG_USELESS_TESTS_SUPPORTED)
	if (row != NULL && row_info != NULL)
#endif
	{
		png_bytep sp=row;
		png_bytep dp=row;
		png_uint_32 row_width=row_info->width;
		png_uint_32 i;

		if ((row_info->color_type == PNG_COLOR_TYPE_RGB ||
			(row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA &&
			(flags & PNG_FLAG_STRIP_ALPHA))) &&
			row_info->channels == 4)
		{
			if (row_info->bit_depth == 8)
			{
				/* This converts from RGBX or RGBA to RGB */
				if (flags & PNG_FLAG_FILLER_AFTER)
				{
					dp+=3; sp+=4;
					for (i = 1; i < row_width; i++)
					{
						*dp++ = *sp++;
						*dp++ = *sp++;
						*dp++ = *sp++;
						sp++;
					}
				}
				/* This converts from XRGB or ARGB to RGB */
				else
				{
					for (i = 0; i < row_width; i++)
					{
						sp++;
						*dp++ = *sp++;
						*dp++ = *sp++;
						*dp++ = *sp++;
					}
				}
				row_info->pixel_depth = 24;
				row_info->rowbytes = row_width * 3;
			}
			else /* if (row_info->bit_depth == 16) */
			{
				if (flags & PNG_FLAG_FILLER_AFTER)
				{
					/* This converts from RRGGBBXX or RRGGBBAA to RRGGBB */
					sp += 8; dp += 6;
					for (i = 1; i < row_width; i++)
					{
						/* This could be (although png_memcpy is probably slower):
						png_memcpy(dp, sp, 6);
						sp += 8;
						dp += 6;
						*/

						*dp++ = *sp++;
						*dp++ = *sp++;
						*dp++ = *sp++;
						*dp++ = *sp++;
						*dp++ = *sp++;
						*dp++ = *sp++;
						sp += 2;
					}
				}
				else
				{
					/* This converts from XXRRGGBB or AARRGGBB to RRGGBB */
					for (i = 0; i < row_width; i++)
					{
						/* This could be (although png_memcpy is probably slower):
						png_memcpy(dp, sp, 6);
						sp += 8;
						dp += 6;
						*/

						sp+=2;
						*dp++ = *sp++;
						*dp++ = *sp++;
						*dp++ = *sp++;
						*dp++ = *sp++;
						*dp++ = *sp++;
						*dp++ = *sp++;
					}
				}
				row_info->pixel_depth = 48;
				row_info->rowbytes = row_width * 6;
			}
			row_info->channels = 3;
		}
		else if ((row_info->color_type == PNG_COLOR_TYPE_GRAY ||
			(row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA &&
			(flags & PNG_FLAG_STRIP_ALPHA))) &&
			 row_info->channels == 2)
		{
			if (row_info->bit_depth == 8)
			{
				/* This converts from GX or GA to G */
				if (flags & PNG_FLAG_FILLER_AFTER)
				{
					for (i = 0; i < row_width; i++)
					{
						*dp++ = *sp++;
						sp++;
					}
				}
				/* This converts from XG or AG to G */
				else
				{
					for (i = 0; i < row_width; i++)
					{
						sp++;
						*dp++ = *sp++;
					}
				}
				row_info->pixel_depth = 8;
				row_info->rowbytes = row_width;
			}
			else /* if (row_info->bit_depth == 16) */
			{
				if (flags & PNG_FLAG_FILLER_AFTER)
				{
					/* This converts from GGXX or GGAA to GG */
					sp += 4; dp += 2;
					for (i = 1; i < row_width; i++)
					{
						*dp++ = *sp++;
						*dp++ = *sp++;
						sp += 2;
					}
				}
				else
				{
					/* This converts from XXGG or AAGG to GG */
					for (i = 0; i < row_width; i++)
					{
						sp += 2;
						*dp++ = *sp++;
						*dp++ = *sp++;
					}
				}
				row_info->pixel_depth = 16;
				row_info->rowbytes = row_width * 2;
			}
			row_info->channels = 1;
		}
		if (flags & PNG_FLAG_STRIP_ALPHA)
		  row_info->color_type &= ~PNG_COLOR_MASK_ALPHA;
	}
}
#endif

#if defined(PNG_READ_BGR_SUPPORTED) || defined(PNG_WRITE_BGR_SUPPORTED)
/* swaps red and blue bytes within a pixel */
void /* PRIVATE */
png_do_bgr(png_row_infop row_info, png_bytep row)
{
	png_debug(1, "in png_do_bgr\n");
	if (
#if defined(PNG_USELESS_TESTS_SUPPORTED)
		 row != NULL && row_info != NULL &&
#endif
		 (row_info->color_type & PNG_COLOR_MASK_COLOR))
	{
		png_uint_32 row_width = row_info->width;
		if (row_info->bit_depth == 8)
		{
			if (row_info->color_type == PNG_COLOR_TYPE_RGB)
			{
				png_bytep rp;
				png_uint_32 i;

				for (i = 0, rp = row; i < row_width; i++, rp += 3)
				{
					png_byte save = *rp;
					*rp = *(rp + 2);
					*(rp + 2) = save;
				}
			}
			else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
			{
				png_bytep rp;
				png_uint_32 i;

				for (i = 0, rp = row; i < row_width; i++, rp += 4)
				{
					png_byte save = *rp;
					*rp = *(rp + 2);
					*(rp + 2) = save;
				}
			}
		}
		else if (row_info->bit_depth == 16)
		{
			if (row_info->color_type == PNG_COLOR_TYPE_RGB)
			{
				png_bytep rp;
				png_uint_32 i;

				for (i = 0, rp = row; i < row_width; i++, rp += 6)
				{
					png_byte save = *rp;
					*rp = *(rp + 4);
					*(rp + 4) = save;
					save = *(rp + 1);
					*(rp + 1) = *(rp + 5);
					*(rp + 5) = save;
				}
			}
			else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
			{
				png_bytep rp;
				png_uint_32 i;

				for (i = 0, rp = row; i < row_width; i++, rp += 8)
				{
					png_byte save = *rp;
					*rp = *(rp + 4);
					*(rp + 4) = save;
					save = *(rp + 1);
					*(rp + 1) = *(rp + 5);
					*(rp + 5) = save;
				}
			}
		}
	}
}
#endif /* PNG_READ_BGR_SUPPORTED or PNG_WRITE_BGR_SUPPORTED */

#if defined(PNG_READ_USER_TRANSFORM_SUPPORTED) || \
	 defined(PNG_WRITE_USER_TRANSFORM_SUPPORTED) || \
	 defined(PNG_LEGACY_SUPPORTED)
void PNGAPI
png_set_user_transform_info(png_structp png_ptr, png_voidp
	user_transform_ptr, int user_transform_depth, int user_transform_channels)
{
	png_debug(1, "in png_set_user_transform_info\n");
#if defined(PNG_USER_TRANSFORM_PTR_SUPPORTED)
	png_ptr->user_transform_ptr = user_transform_ptr;
	png_ptr->user_transform_depth = (png_byte)user_transform_depth;
	png_ptr->user_transform_channels = (png_byte)user_transform_channels;
#else
	if (user_transform_ptr || user_transform_depth || user_transform_channels)
		png_warning(png_ptr,
			"This version of libpng does not support user transform info");
#endif
}
#endif

/* This function returns a pointer to the user_transform_ptr associated with
 * the user transform functions.  The application should free any memory
 * associated with this pointer before png_write_destroy and png_read_destroy
 * are called.
 */
png_voidp PNGAPI
png_get_user_transform_ptr(png_structp png_ptr)
{
#if defined(PNG_USER_TRANSFORM_PTR_SUPPORTED)
	return ((png_voidp)png_ptr->user_transform_ptr);
#else
	if (png_ptr)
		return (NULL);
	return (NULL);
#endif
}
