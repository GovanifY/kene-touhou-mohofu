/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2FSDL_bmp.c

psp - ���r�W���� 440Subversion ���|�W�g���ꗗ: pspps2ps2wareps3ps3warepspware���r�W����:(root)/trunk/SDL/src/video/SDL_bmp.c
�ڍו\�� - �ŏI�X�V���� - ���O������ -
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
 "@(#) $Id: PSPL_bmp.c,v 1.6 2004/12/14 06:20:49 slouken Exp $";
#endif

#ifndef DISABLE_FILE

/*
   Code to load and save surfaces in Windows BMP format.

   Why support BMP format?	Well, it's a native format for Windows, and
   most image processing programs can read and write it.  It would be nice
   to be able to have at least one image format that we can natively load
   and save, and since PNG is so complex that it would bloat the library,
   BMP is a good alternative.

   This code currently supports Win32 DIBs in uncompressed 8 and 24 bpp.
*/
#include <pspkernel.h>

#include <stdio.h>
#include <string.h>

#include "./../include/PSPL_error.h"	/* ��芸����(��) */
#include "./../include/PSPL_video.h"	/* ��芸����(��) */
#include "./../include/PSPL_endian.h"

#ifndef SDL_Error_bbb
	#define SDL_Error_bbb(aaa)
#endif

#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb( ... )
#endif

/* Compression encodings for BMP files */
#ifndef BI_RGB
	#define BI_RGB			0
	#define BI_RLE8 		1
	#define BI_RLE4 		2
	#define BI_BITFIELDS	3
#endif

#if (00)
SDL_Surface * SDL_LoadBMP_RW(SDL_RWops *src, int freesrc)
{
	int was_error;
	long fp_offset;
	int bmpPitch;
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
//	SDL_ClearError();
	if ( SDL_RWread(src, magic, 1, 2) != 2 )
	{
		SDL_Error_bbb(SDL_EFREAD);
		was_error = 1;
		goto done;
	}
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
	bfSize			= SDL_ReadLE32(src);
	bfReserved1 	= SDL_ReadLE16(src);
	bfReserved2 	= SDL_ReadLE16(src);
	bfOffBits		= SDL_ReadLE32(src);

	/* Read the Win32 BITMAPINFOHEADER */
	biSize	  = SDL_ReadLE32(src);
	if ( biSize == 12 )
	{
		biWidth 		= (u32)SDL_ReadLE16(src);
		biHeight		= (u32)SDL_ReadLE16(src);
		biPlanes		= SDL_ReadLE16(src);
		biBitCount		= SDL_ReadLE16(src);
		biCompression	= BI_RGB;
		biSizeImage 	= (0);
		biXPelsPerMeter = (0);
		biYPelsPerMeter = (0);
		biClrUsed		= (0);
		biClrImportant	= (0);
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
	if ( strcmp(SDL_GetError(), "") != 0 )
	{
		was_error = 1;
		goto done;
	}

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

	/* We don't support any BMP compression right now */
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
	case BI_BITFIELDS:
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
	default:
		SDL_SetError_bbb("Compressed BMP files not supported");
		was_error = 1;
		goto done;
	}

	/* Create a compatible surface, note that the colors are RGB ordered */
	surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
			biWidth, biHeight, biBitCount, Rmask, Gmask, Bmask, 0);
	if ( surface == NULL )
	{
		was_error = 1;
		goto done;
	}

	/* Load the palette, if any */
	palette = (surface->format)->palette;
	if ( palette )
	{
		if ( biClrUsed == 0 )
		{
			biClrUsed = 1 << biBitCount;
		}
		if ( biSize == 12 )
		{
			for (i=0; i<(int)biClrUsed; i++)
			{
				SDL_RWread(src, &palette->colors[i].b, 1, 1);
				SDL_RWread(src, &palette->colors[i].g, 1, 1);
				SDL_RWread(src, &palette->colors[i].r, 1, 1);
				palette->colors[i].unused = 0;
			}
		}
		else
		{
			for (i=0; i<(int)biClrUsed; i++)
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
	bits = (u8 *)surface->pixels+(surface->h*surface->pitch);
	switch (ExpandBMP)
	{
	case 1:
		bmpPitch = (biWidth + 7) >> 3;
		pad  = (((bmpPitch)%4) ? (4-((bmpPitch)%4)) : 0);
		break;
	case 4:
		bmpPitch = (biWidth + 1) >> 1;
		pad  = (((bmpPitch)%4) ? (4-((bmpPitch)%4)) : 0);
		break;
	default:
		pad  = ((surface->pitch%4) ?
				(4-(surface->pitch%4)) : 0);
		break;
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
						if ( !SDL_RWread(src, &pixel, 1, 1) )
						{
							SDL_SetError_bbb("Error reading from BMP");
							was_error = 1;
							goto done;
						}
					}
					*(bits+i) = (pixel>>shift);
					pixel <<= ExpandBMP;
				}
			}
			break;
		default:
			if ( SDL_RWread(src, bits, 1, surface->pitch)
							 != surface->pitch )
			{
				SDL_Error_bbb(SDL_EFREAD);
				was_error = 1;
				goto done;
			}
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
	if ( freesrc && src )
	{
		SDL_RWclose(src);
	}
	return (surface);
}
#endif /*(00)*/



/*---------------------------------------------------------
	psp�̕\���� vram ����SDL���(���z�X�N���[��)(SDL_00_VIEW_SCREEN)�ɋ����R�s�[
	-------------------------------------------------------
	16bit mode(BGR565) ��p
---------------------------------------------------------*/

/*---------------------------------------------------------
	�ݒ�
---------------------------------------------------------*/

/* 0 == �ʏ�B[�W��] */
/* 1 == 2�i���̏z���𗘗p�����덷�ŏ��������s���B[��I] */
#define USE_GOSA_SAISYOU	(0)
/* 1 == �ϊ��� gulist ���g�p����B[�W��] */
/* 0 == �ϊ��� gulist ���g�p���Ȃ��B[�f�o�b�O�p] */
#define USE_GU_WORK 		(1)

/*---------------------------------------------------------

---------------------------------------------------------*/

#if (1==USE_GU_WORK)
	extern	unsigned int __attribute__((aligned(16))) gulist[(512*512)];
#endif /*(1==USE_GU_WORK)*/


#define PSP_512_WIDE   (512)
#define PSP_480_WIDE   (480)
#define PSP_272_HEIGHT (272)
//	u16 BM_magic16;/*(.align�������̂Ŗ���)*/
typedef struct
{
	u32 bfSize;
	u16 bfReserved1;
	u16 bfReserved2;
	u32 bfOffBits;
	//
	u32 biSize;
	u32 biWidth;
	u32 biHeight;
	u16 biPlanes;
	u16 biBitCount;
	u32 biCompression;
	u32 biSizeImage;
	u32 biXPelsPerMeter;
	u32 biYPelsPerMeter;
	u32 biClrUsed;
	u32 biClrImportant;
	//
} BMP_HEADER;
static BMP_HEADER bmp_header;
//int SDL_SaveBMP_RW(SDL_Surface *saveme, SDL_RWops *dst, int freedst)
int PSPL_save_vram16_to_file(const char *save_bmp_file_name)
{
	/* Make sure we have somewhere to save */
//	SDL_RWops *dst;
//	dst = SDL_RWFromFile(save_bmp_file_name, "wb");
	SceUID fd;
	fd = sceIoOpen(save_bmp_file_name, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
//	if ( dst )	/* file���쐬�o������ */
	if (0!=fd ) /* file���쐬�o������ */
	{
		#define WIN_BH_14 (14)
		#define WIN_BI_40 (40)
		/* The Win32 BMP file header (14 bytes) */
		/* The Win32 BITMAPINFOHEADER struct (40 bytes) */
		/* (14bytes.)
		2 bytes.	'BM'
		4 bytes.	bfSize
		2 bytes.	(0) == bfReserved1
		2 bytes.	(0) == bfReserved2
		4 bytes.	bfOffBits
		*/
		/* (40bytes)
		*/
		/* Write the BMP file header values */
		//	long fp_offset; 	fp_offset	= SDL_RWtell(dst);
		//	SDL_ClearError();
		//	{
		//		const char	 const_magic_BM_str[2] = { 'B','M' };
		//		SDL_RWwrite(dst, const_magic_BM_str, 2, 1);
		//	}
		/* Set the BMP file header values */
		/* The Win32 BMP file header (14 bytes) */
		//	u32 bfSize; bfSize			= (0);		/* We'll write this when we're done */
		//	u16 bfReserved1;	bfReserved1 	= (0);
		//	u16 bfReserved2;	bfReserved2 	= (0);
		//	u32 bfOffBits;		bfOffBits		= (0);		/* We'll write this when we're done */
	//bmp_header.BM 			= (u16)(((u16)'B')|(((u16)'M')<<8));											//	/*	SDL_WriteLE16(dst,*/ sceIoWrite(fd, (int)v32, 2);	// $42=='B' $4D=='M'  //$01,$02
	{
		u16 valueBM = (u16)(((u16)'B')|(((u16)'M')<<8));
		sceIoWrite(fd, &valueBM, 2);
	}
bmp_header.bfSize		= (u32)((WIN_BI_40+WIN_BH_14)+(PSP_480_WIDE * PSP_272_HEIGHT * 3))/*bfSize*/;	//	/*	SDL_WriteLE32(dst,*/ sceIoWrite(fd, (int)v32, 4);	// $36 $FA $05 $00	// ���Ƃł����Ƃ����l�������̂łƂ肠����0�����Ƃ��B
bmp_header.bfReserved1	= (u16)(0)/*bfReserved1*/;														//	/*	SDL_WriteLE16(dst,*/ sceIoWrite(fd, (int)v32, 2);	// $00 $00
bmp_header.bfReserved2	= (u16)(0)/*bfReserved2*/;														//	/*	SDL_WriteLE16(dst,*/ sceIoWrite(fd, (int)v32, 2);	// $00 $00
bmp_header.bfOffBits	= (u32)(WIN_BI_40+WIN_BH_14)/*bfOffBits*/;										//	/*	SDL_WriteLE32(dst,*/ sceIoWrite(fd, (int)v32, 4);	// $36 $00 $00 $00	// ���Ƃł����Ƃ����l�������̂łƂ肠����0�����Ƃ��B

		/* Set the BMP info values */
		/* The Win32 BITMAPINFOHEADER struct (40 bytes) */
	//	u32 biSize; 			biSize			= (40);/*(bi�̃T�C�Y==40bytes==&H28)*/
	//	s32 biWidth;			biWidth 		= (surface->w);(480)
	//	s32 biHeight;			biHeight		= (surface->h);(272)
	//	u16 biBitCount; 		biBitCount		= (surface->format->BitsPerPixel);(24)
	//	u32 biSizeImage;		biSizeImage 	= ((surface->h) * (surface->pitch));
	//	u16 biPlanes;			biPlanes		= (1);
	//	u32 biCompression;		biCompression	= BI_RGB;
	//	s32 biXPelsPerMeter;	biXPelsPerMeter = (0);
	//	s32 biYPelsPerMeter;	biYPelsPerMeter = (0);
	//
	//	u32 biClrUsed;
	//	if ( surface->format->palette )
	//	{
	//		biClrUsed = surface->format->palette->ncolors;
	//	}
	//	else
	//	{
	//		biClrUsed = (0);
	//	}
	//	u32 biClrImportant; biClrImportant = (0);

		/* Write the BMP info values */
bmp_header.biSize			= (u32)(WIN_BI_40)/*biSize*/;								//	/*	SDL_WriteLE32(dst,*/ sceIoWrite(fd, (int)v32, 4);	// $28 $00 $00 $00	// $0e,$0f,$10,$11
bmp_header.biWidth			= (u32)(PSP_480_WIDE)/*biWidth*/;							//	/*	SDL_WriteLE32(dst,*/ sceIoWrite(fd, (int)v32, 4);	// $E0 $01 $00 $00	// $12,$13,$14,$15
bmp_header.biHeight 		= (u32)(PSP_272_HEIGHT)/*biHeight*/;						//	/*	SDL_WriteLE32(dst,*/ sceIoWrite(fd, (int)v32, 4);	// $10 $01 $00 $00	// $16,$17,$18,$19
bmp_header.biPlanes 		= (u16)(1)/*biPlanes*/; 									//	/*	SDL_WriteLE16(dst,*/ sceIoWrite(fd, (int)v32, 2);	// $01 $00			// $1a,$1b
bmp_header.biBitCount		= (u16)(24)/*biBitCount*/;									//	/*	SDL_WriteLE16(dst,*/ sceIoWrite(fd, (int)v32, 2);	// $18 $00			// $1c,$1d
bmp_header.biCompression	= (u32)(BI_RGB)/*biCompression*/;							//	/*	SDL_WriteLE32(dst,*/ sceIoWrite(fd, (int)v32, 4);	// $00 $00 $00 $00	// $1e,$1f,$20,$21 === (0)
bmp_header.biSizeImage		= (u32)(PSP_480_WIDE * PSP_272_HEIGHT * 3)/*biSizeImage*/;	//	/*	SDL_WriteLE32(dst,*/ sceIoWrite(fd, (int)v32, 4);	// $00 $FA $05 $00	//&hfa0500== 391680==480*272*3	// $22,$23,$24,$25 //00 60 06 00 == &h 00 06 60 00 == 417792 == 272 * 512 * 3[bytes]
bmp_header.biXPelsPerMeter	= (u32)(0)/*biXPelsPerMeter*/;								//	/*	SDL_WriteLE32(dst,*/ sceIoWrite(fd, (int)v32, 4);	// $00 $00 $00 $00	// $26,
bmp_header.biYPelsPerMeter	= (u32)(0)/*biYPelsPerMeter*/;								//	/*	SDL_WriteLE32(dst,*/ sceIoWrite(fd, (int)v32, 4);	// $00 $00 $00 $00
bmp_header.biClrUsed		= (u32)(0)/*biClrUsed*/;									//	/*	SDL_WriteLE32(dst,*/ sceIoWrite(fd, (int)v32, 4);	// $00 $00 $00 $00
bmp_header.biClrImportant	= (u32)(0)/*biClrImportant*/;								//	/*	SDL_WriteLE32(dst,*/ sceIoWrite(fd, (int)v32, 4);	// $00 $00 $00 $00
	sceIoWrite(fd, &bmp_header, sizeof(bmp_header));
#if 1/*(�e�X�g)*/
	//	/* Write the palette (in BGR color order) */
	//	/* Write the bitmap offset */
	//	bfOffBits = SDL_RWtell(dst)-fp_offset;
	//	if ( SDL_RWseek(dst, fp_offset+10, SEEK_SET) < 0 )
	//	{
	//		SDL_Error_bbb(SDL_EFSEEK);
	//	}
	//	SDL_WriteLE32(dst, bfOffBits);
	//	if ( SDL_RWseek(dst, fp_offset+bfOffBits, SEEK_SET) < 0 )
	//	{
	//		SDL_Error_bbb(SDL_EFSEEK);
	//	}
		{	/*(�`��)*/
			#define WRITE_1_LINE_LENGTH 	((PSP_480_WIDE) * (3))
			#if (1==USE_GU_WORK)
			#else
		//	u8 line_buffer[(WRITE_1_LINE_LENGTH*1)];/*(16bit�F(psp_BGR565) �� 24bit�F(win_BGR888)�ϊ��p�o�b�t�@1�s���B[.align�����ĂȂ��̂Ńn���O�A�b�v����])*/
		//	u8 line_buffer[(WRITE_1_LINE_LENGTH*4)];/*(16bit�F(psp_BGR565) �� 24bit�F(win_BGR888)�ϊ��p�o�b�t�@4�s���B)*/
			u8 line_buffer[(WRITE_1_LINE_LENGTH*16)];/*(16bit�F(psp_BGR565) �� 24bit�F(win_BGR888)�ϊ��p�o�b�t�@16�s���B)*/
			#endif
			u16 *src;
			src = (u16 *)((u32) 0x44000000);/*VRAM*/	/*(psp��VRAM�擪)*/
			src += ((PSP_512_WIDE*PSP_272_HEIGHT)); 		/*(�摜�̉����ړ�)*/
			src -= ((PSP_512_WIDE));						/*(�]���J�n���C�����ړ��B��ԉ��̃��C���̐擪�ʒu)*/
			{
				unsigned int jj;
				for (jj=0; jj<(PSP_272_HEIGHT); jj++)
				{
					u8/*16*/ *dest;
				//	if(0==(jj&3))/*(4�s�Ɉ��Z�b�g)*/
					if(0==(jj&0x0f))/*(16�s�Ɉ��Z�b�g[psp�c==272==(16*17)])*/
					{
					//	dest /*= render_image*/ = (u8/*16*/ *)surface->pixels;
						#if (1==USE_GU_WORK)
						dest /*= render_image*/ = (u8/*16*/ *)&gulist[0];
						#else
						dest /*= render_image*/ = (u8/*16*/ *)&line_buffer[0];
						#endif
					}
					unsigned int ii;
					for (ii=0; ii<(PSP_480_WIDE); ii++)
					{
						// psp_BGR565 == bbbb bggg gggr rrrr
						// win_BGR888 == bbbb b--- gggg gg-- rrrr r---
						u16 pix_data;
						pix_data = *src;
						src++;
					#if (1==USE_GOSA_SAISYOU)
						/*(���ʂɃn�C�N�I���B�e�B�[)*/
						#define MY_GET_R_HQ(aaa) ((u8)((((pix_data&0x001f)<<(3)))&0xf8)+((((pix_data&0x001f)>>(2)))&0x07))
						#define MY_GET_G_HQ(aaa) ((u8)((((pix_data&0x07e0)>>(3)))&0xfc)+((((pix_data&0x07e0)>>(9)))&0x03))
						#define MY_GET_B_HQ(aaa) ((u8)((((pix_data&0xf800)>>(8)))&0xf8)+((((pix_data&0xf800)>>(13)))&0x07))
					#else
						/*(����)*/
						#define MY_GET_R_HQ(aaa) ((u8)(((pix_data&0x001f)<<(3)))&0xf8)
						#define MY_GET_G_HQ(aaa) ((u8)(((pix_data&0x07e0)>>(3)))&0xfc)
						#define MY_GET_B_HQ(aaa) ((u8)(((pix_data&0xf800)>>(8)))&0xf8)
					#endif
						*dest = MY_GET_B_HQ(pix_data);		dest++;
						*dest = MY_GET_G_HQ(pix_data);		dest++;
						*dest = MY_GET_R_HQ(pix_data);		dest++;
					}
					#if (0)
					src += ((PSP_512_WIDE-PSP_480_WIDE));	/*(��ʊO���̎c��𑫂�)*/
					src -= ((PSP_512_WIDE));				/*(�`��J�n�ʒu�ɖ߂�)*/
					src -= ((PSP_512_WIDE));				/*(��s��Ɉړ�)*/
					#else
					src -= ((PSP_512_WIDE+PSP_480_WIDE));	/*(��s��Ɉړ�)+(�`�������߂�)*/
					#endif
					/*(.align�������Ă��Ȃ��ƃn���O�A�b�v����̂ŁA.align���킹�̈�4�s�Ɉ�񏑂�����)*/
					/*(.align�������Ă��Ȃ��ƃn���O�A�b�v����̂ŁA.align���킹�̈�16�s�Ɉ�񏑂�����)*/
				//	if(0x03==(jj&3))/*(4�s�Ɉ�񏑂�����)*/
					if(0x0f==(jj&0x0f))/*(16�s�Ɉ�񏑂�����[psp�c==272==(16*17)])*/
					{
						#if (1==USE_GU_WORK)
					//	SDL_RWwrite(dst, gulist/*bits*/, (1), (WRITE_1_LINE_LENGTH) );		/* (1�s�o��[.align�����ĂȂ��̂Ńn���O�A�b�v����]) */
					//	sceIoWrite(fd, gulist/*bits*/, (WRITE_1_LINE_LENGTH)*4 );			/* (4�s�o��) */
						sceIoWrite(fd, gulist/*bits*/, (WRITE_1_LINE_LENGTH)*16 );			/* (16�s�o��) */
						#else
					//	SDL_RWwrite(dst, line_buffer/*bits*/, (1), (WRITE_1_LINE_LENGTH) ); 	/* (1�s�o��[.align�����ĂȂ��̂Ńn���O�A�b�v����]) */
					//	sceIoWrite(fd, line_buffer/*bits*/, (WRITE_1_LINE_LENGTH)*4 );			/* (4�s�o��) */
						sceIoWrite(fd, line_buffer/*bits*/, (WRITE_1_LINE_LENGTH)*16 ); 		/* (16�s�o��) */
						#endif
					}
				}
			}
		}
#endif
		//
		//	/* Write the BMP file size */
		//	bfSize = SDL_RWtell(dst)-fp_offset;
		//	if ( SDL_RWseek(dst, fp_offset+2, SEEK_SET) < 0 )
		//	{
		//		SDL_Error_bbb(SDL_EFSEEK);
		//	}
		//	SDL_WriteLE32(dst, bfSize);
		//	if ( SDL_RWseek(dst, fp_offset+bfSize, SEEK_SET) < 0 )
		//	{
		//		SDL_Error_bbb(SDL_EFSEEK);
		//	}

	//	SDL_RWclose(dst);
		sceIoClose(fd);
	}
//	return ((strcmp(SDL_GetError(), "") == 0) ? (0/*successful*/) : (-1/*error*/));
	return (0);
}
/*
psp��.align�������ĂȂ��ꍇ�n���O�A�b�v���邪�A���̃n���O�A�b�v�͎኱�s�v�c�ȋ��������B
�O��Ƃ���.align�������ĂȂ��ꍇ�A.align�������ĂȂ������� data�� 0 �� padding �����̂ŁA
(0��padding����Ă���̂͂��܂��܂�������Ȃ�)
����ȃf�[�^�[�Ƃ��Ă͎g���Ȃ��B(���̃f�[�^�[���T�C�Y��������)
���̃��[�`���ł́A.align�������ĂȂ��u�ԂɃn���O�A�b�v����̂ł͂Ȃ��A
BMP�f�[�^�[(0��padding����T�C�Y���傫���Ȃ��Ă���̂Ŏg���Ȃ�)��S�ď����I�������Ƀn���O�A�b�v����B
-------------------------------------------
�܂�A
.align�ᔽ�ł�psp��cpu���͍̂\�킸������i�߂�B
������.align�ᔽ����x��������ƁA�u.align�ᔽ���������Ă��Ȃ��v�Ƃ�����Ԃ���u.align�ᔽ�����������v�Ƃ�����ԂɂȂ�B
�u.align�ᔽ�����������v��Ԃ��ǂ����́A���[�U�[�Ƃ͊֌W�����u�ʃ^�X�N�v������������B
�u�ʃ^�X�N�v�Łu.align�ᔽ�����������v��Ԃ��m�F���ꂽ��A�u�ʃ^�X�N�v�������I��psp�̓d����؂�B
*/
#endif /* ENABLE_FILE */
