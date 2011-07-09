
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#include "gu_draw_screen.h"

/*---------------------------------------------------------
	�e�N�X�`�������[�h����
---------------------------------------------------------*/

/*---------------------------------------------------------
	����: ���� swizzle �ϊ����[�`���́A
		���� 16dots ����(8, 4, 2, 1)�̏ꍇ�ƁA
		������ 8dots ����(7, 6, 5, 4, 3, 2, 1)�̏ꍇ�́A
	�ϊ��ł��Ȃ��B
	�������Ȃ���A���������ꍇ��sizzle���Ȃ����A
	���邢�͕���16dots�ɂ���swizzle��mask�`�悵�������`�悪�����B
---------------------------------------------------------*/

//#define USE_T128_SWIZZLE 0
//#if 1//(1==USE_COLOR_16_AND_32)
#if (1==USE_COLOR_16_AND_32)
static void convert_swizzle
(
		  u8	*out_data,		/* �o�͉摜�̐擪�|�C���^ */
	const u8	*in_data,		/* ���͉摜�̐擪�|�C���^ */
/* (�Ⴆ�΁A512x512�e�N�X�`���[�̏ꍇ) */
	const u32	width_x_size,	/* ��������f�[�^�[�T�C�Y */	/*max: 512*4 */
	/* (�Ⴆ�΁AARGB8888 �Ȃ� 512*4==width_x_size) */
	/* (�Ⴆ�΁AARGB1555 �Ȃ� 512*2==width_x_size) */
	const u32	height			/* ���� */		/*max: 512*/
	/* (�Ⴆ�΁A512==height) */
)
{
	#if (1==USE_SWIZZLE)
	const u8	*src_y = in_data;
	u32 		*dst  = (u32*)out_data;
	u8 block_y;
	for (block_y = 0; block_y < /*height_blocks*/(height >>3/* /8 */); block_y++)/*max: 64-1*/
	{
		const u8	*src_x = src_y;
		u8 block_x;
		for (block_x = 0; block_x < /*width_blocks*/(width_x_size  >>4/* /16 */); block_x++)/*max: 32-1*/
		{
			const u32	*src = (u32*)src_x;
			u8 j;
			for (j = 0; j < 8; j++)/*max: 8-1*/
			{
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				src += /*src_pitch*/((width_x_size-16)>>2/* /4 */);
			}
			src_x += 16;
		}
		src_y += /*src_row*/(width_x_size <<3/* *8 */);
	}
	#else
	u32 		*src = (u32*)in_data;
	u32 		*dst = (u32*)out_data;
	u8 block_y;
	for (block_y = 0; block_y < /*height_blocks*/(height >>3/* /8 */); block_y++)/*max: 64-1*/
	{
		u8 block_x;
		for (block_x = 0; block_x < /*width_blocks*/(width_x_size  >>4/* /16 */); block_x++)/*max: 32-1*/
		{
			u8 j;
			for (j = 0; j < 8; j++)/*max: 8-1*/
			{
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
			}
		}
	}
	#endif
}
#endif

/*static*/global void TGameTexture_Load_Surface( int num )	/*, char *file_name*/
{
	/* ----- �e�N�X�`���ԍ����s����������I�� */
//	if (num < 0) return;
//	if (num > TEXTURE_MAX) return;

	/* ----- ���Ƀe�N�X�`�������������� */
	if (NULL != my_resource[num].my_texture)
	{
		#if (1==USE_SDL_image)
		SDL_FreeSurface(my_resource[num].my_texture);
		#else
		png_free_my_image(my_resource[num].my_texture);
		#endif
		my_resource[num].my_texture = NULL;
	}

	/* ----- �e�N�X�`���[�̓ǂݍ��� */
	#if (1==USE_SDL_image)
	my_resource[num].my_texture = IMG_Load(my_resource[num].file_name);
//	my_resource[num].my_texture = SDL_LoadBMP(my_resource[num].file_name);
	#else
	my_resource[num].my_texture = png_load_my_image(my_resource[num].file_name);//
	#endif
	if (NULL == my_resource[num].my_texture)
	{
	//	my_resource[num].my_texture = NULL;
		#if (1)
		CHECKPOINT;
		error(ERR_FATAL, (char*)"texture:cant load image %s:", my_resource[num].file_name);/*" %s"*/ /*,SDL_GetError()*/
		#endif
		return;
	}

	/* --- PSP �����Ƀe�N�X�`�������H���� */

	/* --- PSP�� 16byte align �ɖ����� �s��������(�x��?) �炵���B */
	/* --- ������ memalign(); ���g���A���������E�����킹��B */
	void *nonalign;
//	nonalign = 0;
	nonalign = (my_resource[num].my_texture)->MY_DIB_DATA;

	int mymap_128;
	//	mymap_128 = (my_resource[num].my_texture)->texture_width;
		mymap_128 = my_resource[num].texture_width;
		#define my_map_TW128 mymap_128
		#define my_map_TH128 mymap_128

	unsigned int msize;
//	msize = 0;
	#if (1==USE_SDL_image)
	msize = ((my_resource[num].my_texture)->w * (my_resource[num].my_texture)->h)
		 * (my_resource[num].my_texture)->format->BytesPerPixel;
	#else
	msize = (/*w*/my_map_TW128/*512*/ * /*h*/my_map_TH128/*512*/) * /*bpp*/4;
	#endif

#if (0==USE_32BIT_DRAW_MODE)
/*16bit mode*/
#if (1==USE_COLOR_16_AND_32)
	/* --- PSP �ŊȈ�16�r�b�g�F���w�肵���ꍇ��16bit�ɗ��Ƃ� */
	#if (1==USE_SDL_image)
	if (my_texture[num]->format->BytesPerPixel == 4)	/* PSP16bit mode && DIB==32bit color */
	#else
	if (/*BytesPerPixel*/4 == 4)	/* PSP16bit mode && DIB==32bit color */
	#endif
	//if (0)
	{
		u16 *pixdst;
		u32 *pixsrc;
		/* --- 16bit�F�� ���F���ĕێ� */
		(my_resource[num].my_texture)->pixels = (void*)memalign(16, (msize>>1)/*(msize / 2)*/);
		#if (1==USE_SDL_image)
		msize = ((my_resource[num].my_texture)->w * (my_resource[num].my_texture)->h);
		#else
		msize = (my_map_TW128 * my_map_TH128);
		#endif
		pixdst = (u16 *)/*convert_works*/gulist/*pclass->bitmap[num]->pixels*/;
		pixsrc = (u32 *)nonalign;

		trans_format8888to5650(pixsrc, pixdst, (msize));

	//	unsigned short *pixsrc2;
	//	pixsrc2 = (unsigned short *)pclass->bitmap[num]->pixels;
	//	pixdst = (unsigned short *)gulist/*convert_works*/;
	//	for (loop=0; loop<msize; loop++)
	//	{
	//		(*pixdst) = (*pixsrc2);
	//		pixdst++;
	//		pixsrc2++;
	//	}
		#if (1==USE_SDL_image)
		my_texture[num]->format->BytesPerPixel =  2;
		my_texture[num]->format->BitsPerPixel  = 16;
		my_texture[num]->format->Rmask = /*PSP_SCREEN_FORMAT_RMASK*/RMASK16;/*RMASK16*/
		my_texture[num]->format->Gmask = /*PSP_SCREEN_FORMAT_GMASK*/GMASK16;/*GMASK16*/
		my_texture[num]->format->Bmask = /*PSP_SCREEN_FORMAT_BMASK*/BMASK16;/*BMASK16*/
		my_texture[num]->format->Amask = /*PSP_SCREEN_FORMAT_AMASK*/AMASK16;/*AMASK16*/
		#endif
	// 16bit�F
		convert_swizzle(
			(u8*)((my_resource[num].my_texture)->MY_DIB_DATA),
			(const u8*)gulist/*convert_works*/,
			/* texture width x 2 */  my_map_TW128/*512*/*(2), 	/* short ������ 2�{ */
			/* texture height	 */  my_map_TH128/*512*/);
		// 512*2 because swizzle operates in bytes, and each pixel in a 16-bit texture is 2 bytes
	}
	else/* 32bit mode */
#endif
#endif
	{
		/* --- 32bit�F �ʏ탂�[�h */
		((my_resource[num].my_texture)->MY_DIB_DATA) = (void*)memalign(16, msize);
		memcpy(/*convert_works*/gulist/*pclass->bitmap[num]->pixels*/, nonalign, msize);
		#if (1)
	// 32bit�F
		convert_swizzle(
			(u8*)((my_resource[num].my_texture)->MY_DIB_DATA),
			(const u8*)gulist/*convert_works*/,
			/* texture width x 4 */  my_map_TW128/*512*/*(4), 	/* int ������ 4�{ */
			/* texture height	 */  my_map_TH128/*512*/);
		// 512*2 because swizzle operates in bytes, and each pixel in a 16-bit texture is 2 bytes
		#endif
	}
	free(nonalign);
}

