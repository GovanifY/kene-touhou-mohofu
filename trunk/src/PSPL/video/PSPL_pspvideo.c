
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	/src/SDL231/video/SDL_pspvideo.c
	date:		2011/02/04
	source: 	http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2Fpsp%2FSDL_pspvideo.c
	revision:	2366(2366以下では最新)
--------------------------------------------------------- */

/*
psp - リビジョン 2366Subversion リポジトリ一覧: pspps2ps2wareps3ps3warepspwareリビジョン:(root)/trunk/SDL/src/video/psp/SDL_pspvideo.c
リビジョン 1689 - 詳細表示 - 前のリビジョンとの差分 - 最終更新日時 - ログを見る -
*/

#include <psptypes.h>
#include <pspge.h>
#include <pspkernel.h>
#include <psputils.h>
#include <pspdisplay.h>
#include <pspgu.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "./../include/PSPL.h"			/* 取り敢えず(仮) */
#include "./../include/PSPL_error.h"	/* 取り敢えず(仮) */
#include "./../include/PSPL_video.h"	/* 取り敢えず(仮) */
#include "./PSPL_sysvideo.h"
#include "./PSPL_pixels_c.h"
#include "./PSPL_events_c.h"

#include "./PSPL_pspvideo.h"



#ifndef SDL_OutOfMemory_bbb
	#define SDL_OutOfMemory_bbb(aaa)
#endif

#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb( ... )
#endif


#define SCREEN_480_WIDTH	(480)
#define SCREEN_272_HEIGHT	(272)

#define IS_SWSURFACE(flags) 	((flags & SDL_HWSURFACE) == SDL_SWSURFACE)
#define IS_HWSURFACE(flags) 	((flags & SDL_HWSURFACE) == SDL_HWSURFACE)


/* Private display data */
struct SDL_PrivateVideoData
{
	void *vram_base;
	/* Current display's pixel format. */
	int pixel_format;
	/* The current draw frame for double-buffered displays. */
	int frame;
	/* Byte offset of the start of the second frame. */
	unsigned int frame_offset;
	/* Screen format */
	int psm;
	/* Texture format */
	int tpsm;
	/* Pointer to the CLUT. */
//	void *g u_palette;
	/* Dimensions for stretched blit */
//	SDL_Rect h w_rect;
	SDL_Rect sw_rect;
};

	/* * * */
	/* Data private to this driver */
static	struct SDL_PrivateVideoData *psp_hidden_video;



//static unsigned int list[(4096)] __attribute__((aligned(16)));
//static unsigned int list[4096] __attribute__((aligned(16)));
/*---------------------------------------------------------
	共通ワーク
---------------------------------------------------------*/

//	/*static*/global	unsigned int __attribute__((aligned(16))) gulist[PACKET_SIZE];
//extern	unsigned int __attribute__((aligned(16))) gulist[(512*512)];

//struct texVertex
//{
//	unsigned short u, v;
//	short x, y, z;
//};
//
//struct rectVertex
//{
//	short x, y, z;
//};


/* PSP driver bootstrap functions */



	/* Initialize the native video subsystem, filling 'vformat' with the
	   "best" display pixel format, returning 0 or -1 if there's an error.
	 */
//	int (*VideoInit)(_THIS, SDL_PixelFormat *vformat);
/*static*/extern int SDLVIDEO_PSP_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	/* Default for pspsdk is 8888 ABGR */
	vformat->BitsPerPixel	= 32;
	vformat->BytesPerPixel	= 4;

	this->info.wm_available = 0;
	this->info.hw_available = 1;
	this->info.blit_fill	= 0; /* todo: fixme */
	this->info.blit_hw		= 1;
	this->info.blit_hw_CC	= 1;
	this->info.blit_hw_A	= 0; /* todo: implement me */
	return (0);
}



/* return a suitable psm value for a particular bpp */
static inline int psm_value(int bpp)
{
	if (bpp == 15)
	{	return (GU_PSM_5551);}
	else
	if (bpp == 16)
	{	return (GU_PSM_5650);}
	else
	{
		return (GU_PSM_8888);
	}
}

/*
vidmem handling from Holger Waechtler's pspgl
*/
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
	unsigned int start = ((unsigned int)sceGeEdramGetAddr());
	unsigned int adr = start;
	unsigned int i;

	/* round the size up to the nearest 16 bytes and all hwsurfaces are safe to
	   use as textures */
	if (size % 16 != 0)
	{	size += 16 - (size % 16);	}

	for (i=0; i<vidmem_map_len; i++)
	{
		if (vidmem_map[i].ptr != NULL)
		{
			unsigned int new_adr = (unsigned int) vidmem_map[i].ptr;
			if (size <= new_adr - adr)
			{	return vidmem_map_insert_new(i, adr, size);
			}
			adr = new_adr + vidmem_map[i].len;
		}
	}

	if (adr + size > start + sceGeEdramGetSize())
	{	return (NULL);		}
	return vidmem_map_insert_new(vidmem_map_len, adr, size);
}


static void  vidmem_free (void * ptr)
{
	int i;
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
/*static*/extern int SDLVIDEO_PSP_AllocHWSurface(_THIS, SDL_Surface *surface)
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

	/* Frees a previously allocated video surface */
//	void (*FreeHWSurface)(_THIS, SDL_Surface *surface);
/*static*/extern void SDLVIDEO_PSP_FreeHWSurface(_THIS, SDL_Surface *surface)
{
	vidmem_free(surface->pixels);
	surface->pixels = NULL;
}

/* We need to wait for vertical retrace on page flipped displays */
	/* Returns a readable/writable surface */
//	int (*LockHWSurface)(_THIS, SDL_Surface *surface);
/*static*/extern int SDLVIDEO_PSP_LockHWSurface(_THIS, SDL_Surface *surface)
{
	return (0);
}

	/* Returns a readable/writable surface */
//	void (*UnlockHWSurface)(_THIS, SDL_Surface *surface);
/*static*/extern void SDLVIDEO_PSP_UnlockHWSurface(_THIS, SDL_Surface *surface)
{
	/* Flush video RAM */
	sceKernelDcacheWritebackAll();
}

	/* Set the requested video mode, returning a surface which will be
	   set to the SDL_VideoSurface.  The width and height will already
	   be verified by List Modes(), and the video subsystem is free to
	   set the mode to a supported bit depth different from the one
	   specified -- the desired bpp will be emulated with a shadow
	   surface if necessary.  If a new mode is returned, this function
	   should take care of cleaning up the current mode.
	 */
//廃止済		SDL_Surface *(*SetVideoMode)(_THIS, SDL_Surface *current, int width, int height, int bpp, u32 flags);
/*static*/extern SDL_Surface *SDLVIDEO_PSP_SetVideoMode480x272(_THIS, SDL_Surface *current, /*int width_480, int height_272,*/ int bpp, u32 flags)
{
	int disp_pitch = (512);
	int draw_pitch = (512);
	int pixel_format;
	u32 Amask;
	u32 Rmask;
	u32 Gmask;
	u32 Bmask;

//	if (IS_HWSURFACE(flags)
//	// &&	(width != SCREEN_480_WIDTH || height != SCREEN_272_HEIGHT)
//	)
//	{
//		SDL_SetError_bbb("Couldn't find requested mode");
//		return (NULL);
//	}

	switch(bpp)
	{
	case 8: /* indexed, uses a shadow buffer for hwsurfaces */
		disp_pitch *= 4;
		Amask = 0;
		Rmask = 0;
		Gmask = 0;
		Bmask = 0;
		pixel_format = PSP_DISPLAY_PIXEL_FORMAT_8888;
		break;
	case 15: /* 5-5-5-1 */
		disp_pitch *= 2;
		draw_pitch *= 2;
		Amask = 0x00008000;
		Rmask = 0x0000001f;
		Gmask = 0x000003e0;
		Bmask = 0x00007c00;
		pixel_format = PSP_DISPLAY_PIXEL_FORMAT_5551;
		break;
	case 16: /* 5-6-5 */
		disp_pitch *= 2;
		draw_pitch *= 2;
		Amask = 0;
		Rmask = 0x0000001f;
		Gmask = 0x000007e0;
		Bmask = 0x0000f800;
		pixel_format = PSP_DISPLAY_PIXEL_FORMAT_565;
		break;
	case 32: /* 8-8-8-8 */
		disp_pitch *= 4;
		draw_pitch *= 4;
		Amask = 0xff000000;
		Rmask = 0x000000ff;
		Gmask = 0x0000ff00;
		Bmask = 0x00ff0000;
		pixel_format = PSP_DISPLAY_PIXEL_FORMAT_8888;
		break;
	default:
		SDL_SetError_bbb("Couldn't find requested mode");
		return (NULL);
	}
	if ( ! SDL_ReallocFormat(current, bpp, Rmask, Gmask, Bmask, 0/*Amask*/) )
	{
		SDL_SetError_bbb("Couldn't allocate color format");
		return (NULL);
	}

	current->flags 	= flags | SDL_FULLSCREEN;
	current->w = SCREEN_480_WIDTH;/*width*/
	current->h = SCREEN_272_HEIGHT;/*height*/

	/*this->*/psp_hidden_video->psm 			= psm_value(bpp);
	/*this->*/psp_hidden_video->tpsm			= /*this->*/psp_hidden_video->psm;
	/*this->*/psp_hidden_video->pixel_format	= pixel_format;
	/*this->*/psp_hidden_video->frame			= 0;
	/*this->*/psp_hidden_video->frame_offset	= 0;

	/* allocate display buffer */
	/*this->*/psp_hidden_video->vram_base = vidmem_alloc(disp_pitch * SCREEN_272_HEIGHT);

	sceDisplaySetMode(0, SCREEN_480_WIDTH, SCREEN_272_HEIGHT);
	sceDisplaySetFrameBuf(/*this->*/psp_hidden_video->vram_base, 512, pixel_format, PSP_DISPLAY_SETBUF_NEXTFRAME);

	if (IS_HWSURFACE(flags))
	{
		current->pitch = draw_pitch;
		if (flags & SDL_DOUBLEBUF)
		{
			/*this->*/psp_hidden_video->frame_offset = disp_pitch * SCREEN_272_HEIGHT/*height*/;
			/* Set the draw buffer to the second frame. */
			/*this->*/psp_hidden_video->frame = 1;
			/* allocate drawbuffer */
			vidmem_alloc(disp_pitch * SCREEN_272_HEIGHT/*height*/);
		}
		if (bpp == 8)
		{
			/* create a shadow surface */
			current->pixels = memalign(16, current->pitch * SCREEN_272_HEIGHT/*height*/);
			/* can't hwaccel 8bpp hwsurface */
			this->info.blit_fill 	= 0;
			this->info.blit_hw 		= 0;
		}
		else
		{
			current->pixels = (void *) ((u32) /*this->*/psp_hidden_video->vram_base + /*this->*/psp_hidden_video->frame_offset);
			current->flags |= SDL_PREALLOC; 		/* so SDL doesn't free ->pixels */
		}
	}
	else
	if (IS_SWSURFACE(flags))
	{
	//	/*this->*/psp_hidden_video->h w_rect.w = 480;
	//	/*this->*/psp_hidden_video->h w_rect.h = 272;
	//	/*this->*/psp_hidden_video->h w_rect.x = 0;
	//	/*this->*/psp_hidden_video->h w_rect.y = 0;

		/*this->*/psp_hidden_video->sw_rect.x = 0;
		/*this->*/psp_hidden_video->sw_rect.y = 0;
		/*this->*/psp_hidden_video->sw_rect.w = SCREEN_480_WIDTH;//width;
		/*this->*/psp_hidden_video->sw_rect.h = SCREEN_272_HEIGHT;//height;

		current->pitch 	= /*(width > 512) ? (width) * (bpp/8) :*/ (512) * (bpp/8);
		current->pixels = memalign(16, current->pitch * SCREEN_272_HEIGHT/*height*/);

//		this->UpdateRects = NULL;//(not support)//PSP_GuUpdateRects;
	}
//廃止済	PSP_GuInit(this, current, bpp);
	/* We're done */
	return (current);
}


/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
	/* Reverse the effects VideoInit() -- called if VideoInit() fails
	   or if the application is shutting down the video subsystem.
	*/
//	void (*VideoQuit)(_THIS);
/*static*/extern void SDLVIDEO_PSP_VideoQuit(_THIS)
{
//	sceGuTerm();/*???*/
//
	vidmem_free(/*this->*/psp_hidden_video->vram_base);
	if (this->screen->flags & SDL_DOUBLEBUF)
	{
		vidmem_free(
//			/*this->*/psp_hidden_video->vram_base + /*this->*/psp_hidden_video->frame_offset
			(void *) ((u32) /*this->*/psp_hidden_video->vram_base + /*this->*/psp_hidden_video->frame_offset)	/*(たぶん)*/
		);
	}
}
// #src/PSPL/video/PSPL_pspvideo.c: In function 'SDLVIDEO_PSP_VideoQuit':
// #src/PSPL/video/PSPL_pspvideo.c:440: error: pointer of type 'void *' used in arithmetic

	/* The function used to dispose of this structure */
//	void (*free)(_THIS);
/*static*/extern void SDLVIDEO_PSP_DeleteDevice(SDL_VideoDevice *device)
{
//	if (device)
	{
//		if (/*device->*/psp_hidden_video)
		{
			free(/*device->*/psp_hidden_video);
		}
		free(device);
	}
}

/*static*/extern SDL_VideoDevice *PSP_CreateDevice(int devindex)
{
	/* Initialize all variables that we clean on shutdown */
	SDL_VideoDevice *this;
	this = (SDL_VideoDevice *)malloc(sizeof(SDL_VideoDevice));
	if ( this )
	{
		memset(this, 0, (sizeof *this));
		/*this->*/psp_hidden_video =
			(struct SDL_PrivateVideoData *)malloc((sizeof */*this->*/psp_hidden_video));
	}
	if ( (this == NULL) || (/*this->*/psp_hidden_video == NULL) )
	{
		SDL_OutOfMemory_bbb();
		if ( this )
		{
			free(this);
		}
		return (0);
	}
	memset(/*this->*/psp_hidden_video, 0, (sizeof */*this->*/psp_hidden_video));

	/* Set the function pointers */
//	this->VideoInit 			= SDLVIDEO_PSP_VideoInit;
//	this->SetVideoMode			= SDLVIDEO_PSP_SetVideoMode;

//	this->VideoQuit 			= SDLVIDEO_PSP_VideoQuit;
//	this->AllocHWSurface		= SDLVIDEO_PSP_AllocHWSurface;

//	this->LockHWSurface 		= SDLVIDEO_PSP_LockHWSurface;
//	this->UnlockHWSurface		= SDLVIDEO_PSP_UnlockHWSurface;
//	this->FreeHWSurface 		= SDLVIDEO_PSP_FreeHWSurface;

//	this->free					= SDLVIDEO_PSP_DeleteDevice;
	return (this);
}

//static int PSP_Available(void)
//{
//	return (1);
//}

//	/*global*/ VideoBootStrap PSP_bootstrap =
//{
//	#define PSPVID_DRIVER_NAME "psp"
//	PSPVID_DRIVER_NAME,
//	"PSP video driver",
//	PSP_Available,
//		PSP_CreateDevice
//};
