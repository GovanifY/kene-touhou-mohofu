
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	将来的にSDL使わなくなる予定なので、
	グラフィック読み込みライブラリ
---------------------------------------------------------*/

//#include <stdlib.h>
//#include <malloc.h>
//#include <pspdisplay.h>
//#include <psputils.h>
//#include <pspgu.h>
//#include <psptypes.h>

#include "./../../libpng/my_png.h"//#include <png.h>
#include "graphics00.h"

/*
【ﾏﾀｰﾘ】PSP自作ソフト総合スレ ver.4【開発】
492 ：名無しさん＠お腹いっぱい。：2005/08/26(金) 19:24:17 ID:05X/tAqJ
>>491

psptoolchainだと built in function(組み込み関数)が使えるようです
int __builtin_allegrex_bitrev(int);
int __builtin_allegrex_wsbh(int);
int __builtin_allegrex_wsbw(int);

int __builtin_allegrex_clz(int);
int __builtin_allegrex_clo(int);
int __builtin_allegrex_ctz(int);
int __builtin_allegrex_cto(int);
int __builtin_allegrex_seb(char);
int __builtin_allegrex_seh(short);
int __builtin_allegrex_max(int, int);
int __builtin_allegrex_min(int, int);
void __builtin_allegrex_sync(void);

int __builtin_allegrex_rotr(int val, int shift_amount);
int __builtin_allegrex_rotr(int val, int shift_amount);
void __builtin_allegrex_cache(int cache_func, int address);
float __builtin_allegrex_sqrt_s(float);
int __builtin_allegrex_ceil_w_s(float);
int __builtin_allegrex_floor_w_s(float);
int __builtin_allegrex_round_w_s(float);
int __builtin_allegrex_trunc_w_s(float);

*/


/*---------------------------------------------------------
	Get Next Maximum, Power of 2.
	-------------------------------------------------------
	入力値のうち次の(2のn乗上)を超えない最大値を返します。
	-------------------------------------------------------
	例: 入力 31 出力 32 	// 0001 1111 -> 0010 0000
	例: 入力 32 出力 32 	// 0010 0000 -> 0010 0000
	例: 入力 33 出力 64 	// 0010 0001 -> 0100 0000
---------------------------------------------------------*/

static int round_up_to_power_of_2(int x)
{
	#if (1)
	/* alegrex(pspのcpu)対応の組み込み関数を使う */
	return (1 << (32 - __builtin_allegrex_clz(x - 1)));
	#else
	/* alegrex非対応の場合(pspのcpuに対応していない場合) */
	/* 万一コンパイルできないSDKの場合用 */
	{
		// とりあえず大体の大きさ判定(値が無くなるビットを調べる)
		int b = x;
		int n;
		for (n=0; b!=0; n++)	{	b >>= 1;	}
		// この時点で大体の大きさ(n bit)ぐらい。
		// 大体の大きさ(b)を計算
		b = (1 << n);
		// 特殊ケース(丁度)の場合、半分にする。
		if (b == (2 * x))	{	b >>= 1;	}
		return (b);
	}
	#endif
}

/*---------------------------------------------------------
---------------------------------------------------------*/

#include "111_my_file.h"
/* for png and gif read from memory */
//typedef struct my_img_buffer_
//{
static unsigned char *png_buf_data;
static unsigned long png_buf_length;
static unsigned long png_buf_offset;
//} my_img_buffer;

//static my_img_buffer	png_buf;
#define ERROR_C_MODULE_NAME "graphics00.c: "

#define USE_DEBUG_MESSAGE (0)
#if (1==USE_DEBUG_MESSAGE)
	#define my_err(aaa) 	error(ERR_FATAL, ERROR_C_MODULE_NAME aaa);
#else
	#define my_err(aaa) 	;
#endif


/*---------------------------------------------------------
	libpngからの読み込みコールバック。
	-------------------------------------------------------
	メモリキャッシュ上にあるpngファイルを
	libpngへファイルから読み込んでいる様なフォーマットで渡します。
---------------------------------------------------------*/

extern/*static*/ void my_file_read_to_mem(unsigned char* write_memory, int read_length);
static void /* PRIVATE */ png_file_chache_read_data(png_structp png_ptr_dummy, png_bytep dest_data, png_size_t length)
{
	my_file_read_to_mem(dest_data, length); 	// ファイルキャッシュから指定した長さ[bytes]転送
}


/*---------------------------------------------------------
	png読み込み。
	-------------------------------------------------------
	pngを一度で読み込みます。
---------------------------------------------------------*/

global my_image* png_load_my_image(const char *filename)
{
	strcpy(my_file_common_name, filename);
	//
	char *malloc_buf;
	malloc_buf = my_file_fopen(/*my_file_common_name, "r"*/);	// png読み込み後ファイルキャッシュへ配置
	if (NULL == /* fp =*/malloc_buf)
	{
		my_err("png ファイルがロードできるメモリが無い。");
//		fclose(fp);
		/*re turn (NULL);*/goto err_and_my_close;
	}
	//
	png_buf_data	= (unsigned char*)/*data*/malloc_buf;
	png_buf_length	= /*length*/my_file_now_size();
	// PNGチェック
	#define PNG_BYTES_TO_CHECK08 (8) // PNGの先頭ヘッダサイズ
//	if (0 != png_sig_cmp((png_bytep)png_buf_data, 0, PNG_BYTES_TO_CHECK08) )
	if (0 != png_sig_cmp08((png_bytep)png_buf_data/*, 0, PNG_BYTES_TO_CHECK08*/) )
	{
		my_err("signeture が合わないので png file では無い。");
//		fclose(fp);
		/*re turn (NULL);*/goto err_and_my_close;// 	re turn (HEAD_ERR);
	}
	png_buf_offset	= PNG_BYTES_TO_CHECK08;
	png_buf_data	+= PNG_BYTES_TO_CHECK08;
	png_structp 	png_ptr;
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		my_err("read struct 作成失敗にゃん。");
//		fclose(fp);
		/*re turn (NULL);*/goto err_and_my_close;
	}
	//

	#if 0
	png_set_error_fn(png_ptr, (png_voidp) NULL, (png_error_ptr) NULL, user_warning_fn);
	#endif // 0
	png_infop		info_ptr;
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		my_err("info struct 作成失敗だ。");
		goto error_and_free;
	//	fclose(fp);
	//	png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
	//	re turn (NULL);
	}
	//#define PNG_NO_STDIO/*psp*/
	#if !defined(PNG_NO_STDIO)
//	png_init_io(png_ptr, fp);
	#else
	#endif
	#if 1
//nofp		png_set_read_fn(png_ptr, (void*)fp/*&png_buf*/, (png_rw_ptr)png_file_chache_read_data);
	png_set_read_fn(png_ptr, /*not used dummy*/(void*)0/*&png_buf*/, (png_rw_ptr)png_file_chache_read_data);
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
		my_err("512x512 超えちゃダメ。");
		goto error_and_free;
	//	fclose(fp);
	//	png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
	//	re turn (NULL);
	}
//	my_image			*aaa_image = (my_image*)malloc(sizeof(my_image));
	my_image			aaa_image;
//	if (!aaa_image) re turn (NULL);

//	aaa_image.imageWidth	 = width;
//	aaa_image.imageHeight	 = height;
	aaa_image.texture_width  = round_up_to_power_of_2(width);
	aaa_image.texture_height = round_up_to_power_of_2(height);
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
		my_err("目盛が足りない。");
		goto error_and_free;
	//	fclose(fp);
	//	png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
	//	re turn (NULL);
	}
	{
		u32 *line;
		line = (u32*)malloc(width * 4);
		if (!line)
		{
			free(aaa_image.pixels);
//			free(aaa_image);
			my_err("酒盛が足りません。");
			goto error_and_free;
		//	fclose(fp);
		//	png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		//	re turn (NULL);
		}
		{
			u32 y;
			for (y=0; y<height; y++)
			{
				png_read_row(png_ptr, (u8*) line, png_bytep_NULL);
				u32 x;
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
//
	my_image			*bbb_image = (my_image*)malloc(sizeof(my_image));
	if (!bbb_image)
	{
		/*re turn (NULL);*/goto err_and_my_close;
	}
// copy
	memcpy(bbb_image, &aaa_image, sizeof(/*struct*/ my_image));
//
	my_file_fclose (/*fp*/);	//fclose(fp);	// ファイルキャッシュ破棄
	return (bbb_image);
//
error_and_free:
	png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
	//
err_and_my_close:
	my_file_fclose (/*fp*/);	//fclose(fp);	// ファイルキャッシュ破棄
	return (NULL);
}

global void png_free_my_image(my_image	*image)
{
	free(image->pixels);
	free(image);
}
