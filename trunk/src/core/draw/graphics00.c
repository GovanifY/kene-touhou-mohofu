
/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

//#include <stdlib.h>
//#include <malloc.h>
//#include <pspdisplay.h>
//#include <psputils.h>
//#include <pspgu.h>
//#include <psptypes.h>
//#include <psptypes.h>

#include "game_main.h"

#include <png.h>

#include "graphics00.h"

//#include <malloc.h>

/*---------------------------------------------------------
	将来的にSDL使わなくなる予定なので、
	グラフィック読み込みライブラリ
---------------------------------------------------------*/

static int getNextPower2(int width)
{
	int b = width;
	int n;
	for (n=0; b!=0; n++)
	{
		b >>= 1;
	}
	b = (1 << n);
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

#include "111_my_file.h"
/* for png and gif read from memory */
//typedef struct my_img_buffer_
//{
static unsigned char *png_buf_data;
static unsigned long png_buf_length;
static unsigned long png_buf_offset;
//} my_img_buffer;

//static my_img_buffer	png_buf;

	#if 0
// PNG メモリリード関数
static void png_default_read_data(png_structp png_ptr, png_bytep dest_data, png_size_t length)
{
//	FILE *io_ptr = (my_img_buffer *)png_get_io_ptr(png_ptr);
//	if ( (png_buf->offset + length) > (png_buf->length) )
	#if 0
	if ( (png_buf->offset ) > (png_buf->length) )
	{
	//	png_error(png_ptr,"read error");
	//	error(ERR_FATAL, "%s png read error", my_fopen_file_name );
		error(ERR_FATAL, "%d + %d > %d png read error", (png_buf->offset), length, (png_buf->length) );
	//	(213284)==(205092)+(8192) > 205439 png read error
	//	(??????)==(213284)+(8192) > 205439 png read error
	}
	#endif
//	memcpy(dest_ptr, png_buf_data, length);
//	png_buf_offset	+= length;
//	png_buf_data	+= length;
		FILE *io_ptr;
//	io_ptr = (SDL_RWops *)png_get_io_ptr(png_ptr);
	io_ptr = (FILE *)png_get_io_ptr(png_ptr);
//	SDL_RWread(io_ptr, dest_ptr, length, 1);
	fread( dest_data, 1, length, io_ptr);
}
	#endif // 0

#if 1

static void /* PRIVATE */ png_default_read_data(png_structp png_ptr, png_bytep dest_data, png_size_t length)
{
	int check;
//	png_byte *n_data;

	/* Check if dest_data really is near. If so, use usual code. */
//	n_data = (png_byte *) (dest_data);
//	if ((png_bytep)n_data == dest_data)
	{
//		check = fread(n_data, 1, length, /*io_ptr*/((png_FILE_p) (png_ptr->io_ptr)));
		check = fread((png_byte *) (dest_data), 1, length, /*io_ptr*/((png_FILE_p) (png_ptr->io_ptr)));
	}

//1948209
	if ((png_uint_32)check != (png_uint_32)length)
	{	png_error(png_ptr, "read Error");}
}
#endif

global my_image* png_load_my_image(const char *filename)
{
	strcpy(my_fopen_file_name, filename);
//
//	my_image			*aaa_image = (my_image*)malloc(sizeof(my_image));
	my_image			aaa_image;
//	if (!aaa_image) return (NULL);
//
	char *malloc_buf;
	malloc_buf = my_fopen(/*my_fopen_file_name, "r"*/);
	if ( NULL == /* fp =*/malloc_buf )
	{
		return (NULL);
	}
//	my_fclose (/*fp*/); 	//fclose(fp);

#define PNG_BYTES_TO_CHECK08 (8) // PNGの先頭ヘッダサイズ

	png_buf_data	= (unsigned char*)/*data*/malloc_buf;
	png_buf_length	= /*length*/my_file_now_size();
	// PNGチェック
	if (png_sig_cmp((png_bytep)png_buf_data, 0, PNG_BYTES_TO_CHECK08) != 0)
	{
		return (NULL);//	return HEAD_ERR;
	}
	png_buf_offset	= PNG_BYTES_TO_CHECK08;
	png_buf_data	+= PNG_BYTES_TO_CHECK08;
	png_structp 	png_ptr;
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
//		free(aaa_image);
//		fclose(fp);
		return (NULL);
	}


//
	FILE			*fp;
	fp = fopen(filename, "rb");
	if (NULL == fp)
	{
		return (NULL);
	}
//
	#if 0
	png_set_error_fn(png_ptr, (png_voidp) NULL, (png_error_ptr) NULL, user_warning_fn);
	#endif // 0
	png_infop		info_ptr;
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
//		free(aaa_image);
		goto error_and_free;
	//	fclose(fp);
	//	png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
	//	return (NULL);
	}
	//#define PNG_NO_STDIO/*psp*/
	#if !defined(PNG_NO_STDIO)
//	png_init_io(png_ptr, fp);
	#else
	#endif
	#if 1
	png_set_read_fn(png_ptr, (void*)fp/*&png_buf*/, (png_rw_ptr)png_default_read_data);
	#endif
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
//		free(aaa_image);
		goto error_and_free;
	//	fclose(fp);
	//	png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
	//	return (NULL);
	}
//	aaa_image.imageWidth	 = width;
//	aaa_image.imageHeight	 = height;
	aaa_image.texture_width  = getNextPower2(width);
	aaa_image.texture_height = getNextPower2(height);
	png_set_strip_16(png_ptr);
	png_set_packing(png_ptr);
	if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_gray_1_2_4_to_8(png_ptr);
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
	png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
	aaa_image.pixels = (/*Color*/u32*) memalign(16, aaa_image.texture_width * aaa_image.texture_height * sizeof(/*Color*/u32));
	if (!aaa_image.pixels)
	{
//		free(aaa_image);
		goto error_and_free;
	//	fclose(fp);
	//	png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
	//	return (NULL);
	}
	{
		u32 *line;
		line = (u32*)malloc(width * 4);
		if (!line)
		{
			free(aaa_image.pixels);
//			free(aaa_image);
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
					aaa_image.pixels[x + y * aaa_image.texture_width] = color;
				}
			}
		}
		free(line);
	}
	png_read_end(png_ptr, info_ptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
//
	fclose(fp);
	my_fclose (/*fp*/); 	//fclose(fp);

//
	my_image			*bbb_image = (my_image*)malloc(sizeof(my_image));
	if (!bbb_image) return (NULL);
// copy
	memcpy(bbb_image, &aaa_image, sizeof(/*struct*/ my_image));

//
	return (bbb_image);
//
error_and_free:
	png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
//
	fclose(fp);
	my_fclose (/*fp*/); 	//fclose(fp);
	return (NULL);
}

global void png_free_my_image(my_image	*image)
{
	free(image->pixels);
	free(image);
}

