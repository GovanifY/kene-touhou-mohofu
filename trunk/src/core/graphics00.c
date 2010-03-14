
/*---------------------------------------------------------
	将来的にSDL使わなくなる予定なので、
	グラフィック読み込みライブラリ
---------------------------------------------------------*/

#include <stdlib.h>
#include <malloc.h>
#include <pspdisplay.h>
#include <psputils.h>
#include <png.h>
#include <pspgu.h>
#include <psptypes.h>
#include <psptypes.h>

#include "graphics00.h"

static int getNextPower2(int width)
{
	int b = width;
	int n;
	for (n=0; b!=0; n++)
	{
		b >>= 1;
	}
	b = (1<<n);
//
	if (b == (2 * width))
	{
		b >>= 1;
	}
	return (b);
}


	#if 0
static void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg)	{	}
	#endif // 0

my_image* png_load_my_image(const char *filename)
{
	my_image			*image = (my_image*) malloc(sizeof(my_image));
	if (!image) return (NULL);

	FILE			*fp;
	if ((fp = fopen(filename, "rb")) == NULL) return (NULL);
	png_structp 	png_ptr;
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		free(image);
		fclose(fp);
		return (NULL);
	}
	#if 0
	png_set_error_fn(png_ptr, (png_voidp) NULL, (png_error_ptr) NULL, user_warning_fn);
	#endif // 0
	png_infop		info_ptr;
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		free(image);
		goto error_and_free;
	//	fclose(fp);
	//	png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
	//	return (NULL);
	}
	png_init_io(png_ptr, fp);

	png_set_sig_bytes(png_ptr, 0/*sig_read*/);	//unsigned int	sig_read = 0;
	png_read_info(png_ptr, info_ptr);
//
	png_uint_32 	width;
	png_uint_32 	height;
	int 			bit_depth;
	int 			color_type;
	int 			interlace_type;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, int_p_NULL, int_p_NULL);
	if (width > 512 || height > 512)
	{
		free(image);
		goto error_and_free;
	//	fclose(fp);
	//	png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
	//	return (NULL);
	}
//	image->imageWidth	 = width;
//	image->imageHeight	 = height;
	image->texture_width  = getNextPower2(width);
	image->texture_height = getNextPower2(height);
	png_set_strip_16(png_ptr);
	png_set_packing(png_ptr);
	if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_gray_1_2_4_to_8(png_ptr);
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
	png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
	image->pixels = (/*Color*/u32*) memalign(16, image->texture_width * image->texture_height * sizeof(/*Color*/u32));
	if (!image->pixels)
	{
		free(image);
		goto error_and_free;
	//	fclose(fp);
	//	png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
	//	return (NULL);
	}
	{
		u32 *line;
		line = (u32*) malloc(width * 4);
		if (!line)
		{
			free(image->pixels);
			free(image);
			goto error_and_free;
		//	fclose(fp);
		//	png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		//	return (NULL);
		}
		{
			int y;
			for (y=0; y<height; y++)
			{
				png_read_row(png_ptr, (u8*) line, png_bytep_NULL);
				int x;
				for (x=0; x<width; x++)
				{
					u32 color = line[x];
					image->pixels[x + y * image->texture_width] =  color;
				}
			}
		}
		free(line);
	}
	png_read_end(png_ptr, info_ptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
	fclose(fp);
	return (image);
error_and_free:
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return (NULL);
}

void png_free_my_image(my_image 	*image)
{
	free(image->pixels);
	free(image);
}

