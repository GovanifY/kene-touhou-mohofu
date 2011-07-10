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

/* $Id: IMG.c,v 1.15 2004/01/04 17:33:01 slouken Exp $ */

/* A simple library to load images of various formats as SDL surfaces */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "PSP_SDL_image.h"

#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb( ... )
#endif

#define ARRAYSIZE(a) (sizeof(a) / sizeof((a)[0]))


/* Table of image detection and loading functions */
static struct
{
	const char *type;							// その形式の名前です。
	int (*is)(SDL_RWops *src);					// その形式かどうか判定します。
	SDL_Surface *(*load)(SDL_RWops *src);		// その形式で SDL_Surface に読み込みます。
} supported[] =
{
	/* keep magicless formats first */
	#ifdef LOAD_TGA
	{ "TGA", NULL,		IMG_LoadTGA_RW },
	#endif
	#ifdef LOAD_BMP
	{ "BMP", IMG_isBMP, IMG_LoadBMP_RW },
	#endif
	#ifdef LOAD_PNM
	{ "PNM", IMG_isPNM, IMG_LoadPNM_RW }, /* P[BGP]M share code */
	#endif
	#ifdef LOAD_XPM
	{ "XPM", IMG_isXPM, IMG_LoadXPM_RW },
	#endif
	#ifdef LOAD_XCF
	{ "XCF", IMG_isXCF, IMG_LoadXCF_RW },
	#endif
	#ifdef LOAD_PCX
	{ "PCX", IMG_isPCX, IMG_LoadPCX_RW },
	#endif
	#ifdef LOAD_GIF
	{ "GIF", IMG_isGIF, IMG_LoadGIF_RW },
	#endif
//	模倣風では jpeg より png の使用頻度が高いので上にする。
	#ifdef LOAD_PNG
	{ "PNG", IMG_isPNG, IMG_LoadPNG_RW },
	#endif
//
	#ifdef LOAD_JPG
	{ "JPG", IMG_isJPG, IMG_LoadJPG_RW },
	#endif
	#ifdef LOAD_TIF
	{ "TIF", IMG_isTIF, IMG_LoadTIF_RW },
	#endif
	#ifdef LOAD_LBM
	{ "LBM", IMG_isLBM, IMG_LoadLBM_RW },
	#endif
//	#ifdef LOAD_PNG
//	{ "PNG", IMG_isPNG, IMG_LoadPNG_RW }
//	#endif
	/*
	補足:	IMG_isXXXの判定はいい加減といえば、いい加減なので
	ここの読み込み順に判定が依存している可能性があります。
	だから安直に並べ替えませんでした。
	-----
	つまり SDL_image はこの順序しかチェックしていないのだから、
	使用頻度が高い順に並べ替えるだけでエンバグ(無かったバグを付加すること)する
	可能性があると言う事です。
	-----
	ですが、模倣風では jpeg より png の使用頻度が高いので、
	そこだけ並べ替えました。
	*/
};

#if (0)
/* Portable case-insensitive string compare function */
/* src/PSPL/audio/mixer/music.c */
/* src/PSPL/image/IMG.c */

static int PSPL_string_equals(const char *str1, const char *str2)
{
	while ( (*str1) && (*str2) )
	{
		if ( tou pper((u8)*str1) !=
			 tou pper((u8)*str2) )
		{	break;}
		str1++;
		str2++;
	}
	return ((!*str1) && (!*str2));
}
#else
extern int PSPL_string_equals(const char *str1, const char *str2);
#endif

#if 1
/* Load an image from a file */
SDL_Surface *IMG_Load(const char *file)
{
	SDL_RWops *src = SDL_RWFromFile(file, "rb");
	if (NULL == src)
	{
		/* The error message has been set in SDL_RWFromFile */
		SDL_SetError_bbb("Passed a NULL data source");
		return (NULL);
	}
	/* Make sure there is something to do.. */
	/* See whether or not this data source can handle seeking */
	if (SDL_RWseek(src, 0, SEEK_CUR) < 0)
	{
		SDL_SetError_bbb("Can't seek in this data source");
		goto err_my_close;
	//	#if (1)//(freesrc)
	//	{	SDL_RWclose(src);}
	//	#endif
	//	return (NULL);
	}
	//	const int freesrc = (1);
	//	return (s_IMG_LoadTyped_RW(src, 1, type));
	/* Load an image from an SDL datasource, optionally specifying the type */
	//static SDL_Surface *s_IMG_LoadTyped_RW(SDL_RWops *src, int freesrc, char *type)
	/* Detect the type of image being loaded */
	char *type = strrchr(file, '.');
	if (type)
	{
		type++;
	}
	{
		int start;				start = SDL_RWtell(src);
		SDL_Surface *image; 	image = NULL;
		unsigned int i;
		for (i=0; i<ARRAYSIZE(supported); i++)
		{
			if (supported[i].is)
			{
				SDL_RWseek(src, start, SEEK_SET);
				if (!supported[i].is(src))
				{	continue;}
			}
			else
			{
				/* magicless format */
				if (!type
				   || !PSPL_string_equals(type, supported[i].type))
				{	continue;}
			}
			#ifdef DEBUG_IMGLIB
			#ifdef PSP_VIRTUAL_CONSOLE_IO
			error(0/*ERR_FATAL*/, (char*)"IMGLIB: Loading image as %s\n", supported[i].type);
			#endif
			#endif
			SDL_RWseek(src, start, SEEK_SET);
			image = supported[i].load(src);
			#if (1)//(freesrc)
			{	SDL_RWclose(src);}
			#endif
			return (image);/*(成功)*/
		}
	}
	SDL_SetError_bbb("Unsupported image format");
err_my_close:
	#if (1)//(freesrc)
	{	SDL_RWclose(src);	}
	#endif
	return (NULL);/*(失敗、このファイルは読み込めなかった)*/
}
#endif







#if 0/*(非サポート)*/
/* SDL過去バージョン互換関数 */
/* Load an image from an SDL datasource (for compatibility) */
SDL_Surface *IMG_Load_RW(SDL_RWops *src, int freesrc)
{
	return (s_IMG_LoadTyped_RW(src, freesrc, NULL));
}
#endif

#if 0/*(非サポート)*/
/* OpenGL用関数 */
/* Invert the alpha of a surface for use with OpenGL
   This function is a no-op and only kept for backwards compatibility.
 */
int IMG_InvertAlpha(int on)
{
	return (1);
}
#endif

#if 0/*(非サポート)*/
/* SDL IMAGEバージョンチェック関数 */
const SDL_version *IMG_Linked_Version(void)
{
	static SDL_version linked_version;
	SDL_IMAGE_VERSION(&linked_version);
	return (&linked_version);
}
#endif
