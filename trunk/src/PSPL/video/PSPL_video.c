
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
	-------------------------------------------------------
	/src/SDL231/video/SDL_video.c
	date:		2011/02/04
	source: 	http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2FSDL_video.c&peg=2366
	revision:	448(2366以下では最新)
	-------------------------------------------------------
	/src/SDL231/video/SDL_pspvideo.c
	date:		2011/02/04
	source: 	http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2Fpsp%2FSDL_pspvideo.c
	revision:	2366(2366以下では最新)
	-------------------------------------------------------
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2Fpsp%2FSDL_pspvideo.h
---------------------------------------------------------*/

/*
psp - リビジョン 448Subversion リポジトリ一覧: pspps2ps2wareps3ps3warepspwareリビジョン:(root)/trunk/SDL/src/video/SDL_video.c @ 2366
リビジョン 441 - 詳細表示 - 前のリビジョンとの差分 - 最終更新日時 - ログを見る -
psp - リビジョン 2366Subversion リポジトリ一覧: pspps2ps2wareps3ps3warepspwareリビジョン:(root)/trunk/SDL/src/video/psp/SDL_pspvideo.c
リビジョン 1689 - 詳細表示 - 前のリビジョンとの差分 - 最終更新日時 - ログを見る -
psp - リビジョン 1464Subversion リポジトリ一覧: pspps2ps2wareps3ps3warepspwareリビジョン:(root)/trunk/SDL/src/video/psp/SDL_pspvideo.h
リビジョン 1444 - 詳細表示 - 前のリビジョンとの差分 - 最終更新日時 - ログを見る -
*/

/*
	PSP port contributed by:
	Marcus R. Brown <mrbrown@ocgnet.org>
	Jim Paris <jim@jtan.com>
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_nullvideo.h,v 1.4 2004/01/04 16:49:24 slouken Exp $";
#endif

/* The high-level video driver subsystem */

#include <psptypes.h>
#include <pspge.h>
//#include <pspkernel.h>
//#include <psputils.h>
#include <pspdisplay.h>
//#include <pspgu.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../include/PSPL.h"			/* 取り敢えず(仮) */
#include "./../include/PSPL_error.h"	/* 取り敢えず(仮) */
#include "./../include/PSPL_video.h"	/* 取り敢えず(仮) */
//#include "PSPL_events.h"
//#include "PSPL_mutex.h"
#include "PSPL_sysvideo.h"
//#include "PSPL_sysevents.h"
#include "PSPL_blit.h"
#include "PSPL_pixels_c.h"
#include "PSPL_events_c.h"
//#include "PSPL_cursor_c.h"

#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb( ... )
#endif


#define SCREEN_480_WIDTH	(480)
#define SCREEN_272_HEIGHT	(272)


	SDL_Surface *pspl_screen;
/*---------------------------------------------------------

---------------------------------------------------------*/


/* PSP driver bootstrap functions */
/* vidmem handling from Holger Waechtler's pspgl */
struct vidmem_chunk
{
	void *ptr;
	unsigned int len;
};


static struct vidmem_chunk *vidmem_map = NULL;
static unsigned int vidmem_map_len = 0;

static void *vidmem_map_insert_new (unsigned int idx, unsigned int adr, unsigned int size)
{
	void *tmp = realloc(vidmem_map, (vidmem_map_len + 1) * sizeof(vidmem_map[0]));
	if (!tmp)
	{	return (NULL);	}

	vidmem_map = tmp;
	memmove(&vidmem_map[idx+1], &vidmem_map[idx], (vidmem_map_len-idx) * sizeof(vidmem_map[0]));
	vidmem_map_len++;
	vidmem_map[idx].ptr = (void*) adr;
	vidmem_map[idx].len = size;

	return vidmem_map[idx].ptr;
}


static void* vidmem_alloc(unsigned int size)
{
	/*(16[bytes]境界へ .align 調整処理.)*/
	unsigned int i;
	/* round the size up to the nearest 16 bytes and
	 all hwsurfaces are safe to use as textures. */
	i = (size & (16-1));//(size % 16)
//	if (0 != i) 	{	size += 16 - i; 	}
	if (0 != i) 	{	size += 16; size -= i;	}
	/*(リストに挿入処理.)*/
	unsigned int start_addr;
	unsigned int temp_addr;
	start_addr	= ((unsigned int)sceGeEdramGetAddr());
	temp_addr	= start_addr;
	/*(残りvramのある限り調べる)*/
	for (i=0; i<vidmem_map_len; i++)
	{
		if (vidmem_map[i].ptr != NULL)
		{
			unsigned int new_addr;
			new_addr = ((unsigned int)vidmem_map[i].ptr);
			if (size <= new_addr - temp_addr)
			{
				goto my_insert_end;/*(その場所に挿入)*/
			}
			temp_addr = new_addr + vidmem_map[i].len;
		}
	}
	/*(失敗判定)*/
	if (temp_addr + size > start_addr + sceGeEdramGetSize())
	{return (NULL);/*(挿入できない)*/}
	/*(最後に挿入)*/
	i = vidmem_map_len;
my_insert_end:
	return (vidmem_map_insert_new(i, temp_addr, size));/* (挿入処理.) */
}


static void  vidmem_free(void * ptr)
{
	unsigned int i;
	for (i=0; i<vidmem_map_len; i++)
	{
		if (vidmem_map[i].ptr == ptr)
		{
			void *tmp;
			vidmem_map_len--;
			memmove(&vidmem_map[i], &vidmem_map[i+1], (vidmem_map_len-i) * sizeof(vidmem_map[0]));
			tmp = realloc(vidmem_map, vidmem_map_len * sizeof(vidmem_map[0]));
			if (tmp)
			{	vidmem_map = tmp;	}
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

#if 1/*(???)*/
/*
Anybody have a routine that will take a number and round it up to the next
power of two?

i.e.:
15 gets rounded up to 16 (2^4).
120 gets rounded up to 128 (2^7).
1000 gets rounded up to 1024 (2^10).

etc...
*/

static /*inline*/ int round_up_to_power_of_2(int x)
{
	#if (1)
	/* alegrex(pspのcpu)対応の組み込み関数を使う */
	return (1 << (32 - __builtin_allegrex_clz(x - 1)));
	#else
	/* alegrex非対応の場合 */
	/* 万一コンパイルできないSDKの場合用 */
	{
		int b = x;
		int n;
		for (n=0; b!=0; n++)
		{
			b >>= 1;
		}
		b = (1 << n);
		//
		if (b == (2 * x))
		{
			b >>= 1;
		}
		return (b);
	}
	#endif
}

	/* Allocates a surface in video memory */
//	int (*AllocHWSurface)(_THIS, SDL_Surface *surface);
/*static*/extern int SDLVIDEO_PSP_AllocHWSurface_aaa(/*_THIS,*/ SDL_Surface *surface)
{
	int pitch;
	pitch = round_up_to_power_of_2(surface->pitch);
	surface->pixels = vidmem_alloc(pitch * surface->h);
	if (!surface->pixels)
	{
		return (-1);/* 失敗 */
	}
	surface->pitch = pitch;
	surface->flags |= SDL_HWSURFACE;/* HW可能 */
	surface->hwdata = (void*)1; /* Hack to make SDL realize it's a HWSURFACE when freeing */
	return (0);/* 成功 */
}
#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

	/* Frees a previously allocated video surface */
//	void (*FreeHWSurface)(_THIS, SDL_Surface *surface);
/*static*/extern void SDLVIDEO_PSP_FreeHWSurface_aaa(/*_THIS,*/ SDL_Surface *surface)
{
	vidmem_free(surface->pixels);
	surface->pixels = NULL;
}






/*---------------------------------------------------------

---------------------------------------------------------*/


#if (1)/* call from src/core/menu/scenario_script.c */
/*
 * Convert a surface into the video pixel format.
 */
/*global*/ SDL_Surface *SDL_DisplayFormat(SDL_Surface *surface)
{
	u32 flags;
//	#if (000)/* NOT support check. */
//	if ( ! (pspl_screen) )
//	{
//		// SDLのビデオモードが一度もセットされてないよ。
//		SDL_SetError_bbb("No video mode has been set");
//		return (NULL);
//	}
//	#endif
//	#if (000)/* NOT support get SDL_HWSURFACE. */
//	/* Set the flags appropriate for copying to display surface */
//	if ((((pspl_screen)->flags&SDL_HWSURFACE) == SDL_HWSURFACE) && current_video->info.blit_hw)
//	{	flags = SDL_HWSURFACE;}
//	else
//	#endif
	{	flags = SDL_SWSURFACE;}
//	#ifdef AUTORLE_DISPLAYFORMAT
//	flags |= (surface->flags & (SDL_SRCCOLORKEY|SDL_SRCALPHA));
//	flags |= SDL_RLEACCELOK;
//	#else
	#if (1==USE_RLEACCEL)
		flags |= surface->flags & (SDL_SRCCOLORKEY|SDL_SRCALPHA|SDL_RLEACCELOK);
	#else
		flags |= surface->flags & (SDL_SRCCOLORKEY|SDL_SRCALPHA/*|SDL_RLEACCELOK*/);
	#endif /*(USE_RLEACCEL)*/
//	#endif
	return (SDL_ConvertSurface(surface, (pspl_screen)->format, flags));
}
#endif

	/* * * */
static void *psp_vram_base;
/*---------------------------------------------------------
	Clean up the video subsystem.
	Note:	If we are terminated, this could be called in the middle of
			another SDL video routine -- notably UpdateRects.
--------------------------------------------------------- */
/* Reverse the effects VideoInit() -- called if VideoInit() fails
   or if the application is shutting down the video subsystem. */
void PSPL_VideoQuit(void)/*only exit once*/
{
	/* Clean up allocated window manager items */
//	if ( (pspl_screen) )
//	{
//		(pspl_screen) = NULL;
//	}
	/* Just in case... */
	/* Clean up the system video */
//	sceGuTerm();/*???*/
	vidmem_free(psp_vram_base);
	/* Free any lingering surfaces */
	if ( (pspl_screen) != NULL )
	{
		SDL_Surface *ready_to_go;
		ready_to_go 		= (pspl_screen);
		(pspl_screen)	= NULL;
		SDL_FreeSurface(ready_to_go);
	}
	(pspl_screen) = NULL;
	/* Clean up miscellaneous memory */
	/* Finish cleaning up video subsystem */
//	video->free(this);
}


/*---------------------------------------------------------
	void PSPL_VideoInit(void)
	Initialize the video and event subsystems -- determine native pixel format.
	SDL_SetVideoMode
--------------------------------------------------------- */

//SDL_Surface * SDL_SetVideoMode(int dummy_width, int dummy_height, int video_bpp, u32 dummy_flags)
SDL_Surface * PSPL_video_init(
	int video_bpp_sdl,	/*sdl depth(15 or 16 or 32)*/
	int video_depth16,	/*psp depth(16 or 32)*/
	u32 Amask,
	u32 Bmask,
	u32 Gmask,
	u32 Rmask
)/*only boot once*/
// /*static*/extern int SDLVIDEO_PSP_VideoInit(_THIS, SDL_PixelFormat *vformat);
{
	/* Select the proper video driver */
	/* Initialize all variables that we clean on shutdown */
	memset(&pspl_screen, 0, (sizeof(pspl_screen)));

	/* Do some basic variable initialization */
	pspl_screen 						= NULL; 	/* Until SDL_SetVideoMode() */

	/* Initialize the video subsystem */

	//	video->VideoInit(video, &vformat);
	//	SDLVIDEO_PSP_VideoInit(video, &vformat);
		/* Initialize the native video subsystem, filling 'vformat' with the
		   "best" display pixel format, returning 0 or -1 if there's an error.
		 */
	// /*static*/extern int SDLVIDEO_PSP_VideoInit(_THIS, SDL_PixelFormat *vformat)
	#if (1)
	{
		SDL_PixelFormat vformat;
		memset(&vformat, 0, sizeof(vformat));
		vformat.BytesPerPixel		= 4;	/* Default for pspsdk is 8888 ABGR */
		vformat.BitsPerPixel		= /*video_depth16*/16/*32*/;	/* Default for pspsdk is 8888 ABGR */
	//	vformat.Amask= /*Amask*/0/*0*/;
	//	vformat.Bmask= /*Bmask*/0/*0*/;
	//	vformat.Gmask= /*Gmask*/0/*0*/;
	//	vformat.Rmask= /*Rmask*/0/*0*/;
		/* Create a zero sized video surface of the appropriate format */
		pspl_screen =
			SDL_CreateRGBSurface
			(
				/*video_flags =*/(SDL_SWSURFACE),
				(0)/*width*/,
				(0)/*height*/
			,
				vformat.BitsPerPixel,
				vformat.Rmask,
				vformat.Gmask,
				vformat.Bmask,
				0
			);
	}
	#endif
	#if (0)/*(ダメ)*/
	{
	//	SDL_PixelFormat vformat;
	//	memset(&vformat, 0, sizeof(vformat));
	//unused	vformat.BytesPerPixel		= 4;	/* Default for pspsdk is 8888 ABGR */
		/*vformat.*/(pspl_screen)->format->BitsPerPixel 	= /*video_depth16*/16/*32*/;	/* Default for pspsdk is 8888 ABGR */
		/*vformat.*/(pspl_screen)->format->Amask= /*Amask*/0/*0*/;
		/*vformat.*/(pspl_screen)->format->Bmask= /*Bmask*/0/*0*/;
		/*vformat.*/(pspl_screen)->format->Gmask= /*Gmask*/0/*0*/;
		/*vformat.*/(pspl_screen)->format->Rmask= /*Rmask*/0/*0*/;
		/* Create a zero sized video surface of the appropriate format */
		pspl_screen =
		//	SDL_CreateRGBSurface
			SDL_CreateSurface
			(
				/*video_flags =*/(SDL_SWSURFACE),
				(0)/*width*/,
				(0)/*height*/
			//,
			//	vformat.BitsPerPixel,
			//	vformat.Rmask,
			//	vformat.Gmask,
			//	vformat.Bmask,
			//	0
			);
	}
	#endif
//	info_vfmt = (pspl_screen)->format;

	/* Clean up any previous video mode */
//	if ( (pspl_screen) != NULL )
//	{
//		(pspl_screen) = NULL;
//	}
	/* Try to set the video mode, along with offset and clipping */
//	SDL_Surface *mode;
//	mode = (pspl_screen);
	//
	int disp_pitch;
	int pixel_format;
	disp_pitch = (512*2);
	{
	//	u32 Amask;
	//	u32 Bmask;
	//	u32 Gmask;
	//	u32 Rmask;
		if (15==video_bpp_sdl)//case 15: /* 5-5-5-1 */
		{
		//	Amask = 0x00008000;
		//	Bmask = 0x00007c00;
		//	Gmask = 0x000003e0;
		//	Rmask = 0x0000001f;
			pixel_format = PSP_DISPLAY_PIXEL_FORMAT_5551;
		}
		else
		if (16==video_bpp_sdl)//case 16: /* 5-6-5 */
		{
		//	Amask = 0;
		//	Bmask = 0x0000f800;
		//	Gmask = 0x000007e0;
		//	Rmask = 0x0000001f;
			pixel_format = PSP_DISPLAY_PIXEL_FORMAT_565;
		}
		else
	//	if (32==video_bpp_sdl)//case 32: /* 8-8-8-8 */
		{
			disp_pitch += disp_pitch;
		//	Amask = 0xff000000;
		//	Bmask = 0x00ff0000;
		//	Gmask = 0x0000ff00;
		//	Rmask = 0x000000ff;
			pixel_format = PSP_DISPLAY_PIXEL_FORMAT_8888;
		}
	}
	/* Now adjust the offsets to match the desired mode */
	//廃止		video->offset_x 		= (0);
	//廃止		video->offset_y 		= (0);
	SDL_ReallocFormat(pspl_screen, video_bpp_sdl, Rmask, Gmask, Bmask, 0/*Amask*/);
	pspl_screen->offset 	= (0);//(video->offset_y * pspl_screen->pitch) + (video->offset_x * pspl_screen->format->BytesPerPixel);
	pspl_screen->w		= SCREEN_480_WIDTH;/*width*/
	pspl_screen->h		= SCREEN_272_HEIGHT;/*height*/
	pspl_screen->pitch	= (disp_pitch);
	/* allocate display buffer */
	{
		u32 	psp_frame_offset;	/* Byte offset of the start of the second frame. */
		psp_frame_offset = (disp_pitch * SCREEN_272_HEIGHT);/*height*/
		psp_vram_base	= vidmem_alloc((psp_frame_offset+psp_frame_offset));	/* allocate dispbuffer + drawbuffer */
		pspl_screen->pixels 	= (void *) ((u32) psp_vram_base + psp_frame_offset);
	}
	sceDisplaySetMode(0, SCREEN_480_WIDTH, SCREEN_272_HEIGHT);
	sceDisplaySetFrameBuf(psp_vram_base, 512, pixel_format, PSP_DISPLAY_SETBUF_NEXTFRAME);
	{
		u32 flags;
		flags = (SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_PREALLOC );
		pspl_screen->flags	= flags;
		/* SDL_PREALLOC: so SDL doesn't free ->pixels */
	}
	/* Clear the surface to black */
//	SDL_SetClipRect(pspl_screen, NULL);
	#if 0/*(とりあえず)*/
	SDL_ClearSurface(pspl_screen);// 本来、起動時(画面モード切り替え時)に画面消す機能。
	#endif
//	info_vfmt = (pspl_screen)->format;
	return (pspl_screen);
}
//#include "not_use_PSPL_video.c" // 使用しない部分が多いので分割
