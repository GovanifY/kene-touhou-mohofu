
/*---------------------------------------------------------
	“Œ•û–Í•í•— ` Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	/src/SDL231/video/SDL_video.c
	date:		2011/02/04
	source: 	http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2FSDL_video.c&peg=2366
	revision:	448(2366ˆÈ‰º‚Å‚ÍÅV)
--------------------------------------------------------- */

/*
psp - ƒŠƒrƒWƒ‡ƒ“ 448Subversion ƒŠƒ|ƒWƒgƒŠˆê——: pspps2ps2wareps3ps3warepspwareƒŠƒrƒWƒ‡ƒ“:(root)/trunk/SDL/src/video/SDL_video.c @ 2366
ƒŠƒrƒWƒ‡ƒ“ 441 - Ú×•\¦ - ‘O‚ÌƒŠƒrƒWƒ‡ƒ“‚Æ‚Ì·•ª - ÅIXV“ú - ƒƒO‚ğŒ©‚é -
*/

/* The high-level video driver subsystem */

#include <psptypes.h>
#include <pspge.h>
#include <pspkernel.h>
#include <psputils.h>
#include <pspdisplay.h>
#include <pspgu.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../include/PSPL.h"			/* æ‚èŠ¸‚¦‚¸(‰¼) */
#include "./../include/PSPL_error.h" 	/* æ‚èŠ¸‚¦‚¸(‰¼) */
#include "./../include/PSPL_video.h" 	/* æ‚èŠ¸‚¦‚¸(‰¼) */
//#include "PSPL_events.h"
//#include "PSPL_mutex.h"
#include "PSPL_sysvideo.h"
//#include "PSPL_sysevents.h"
#include "PSPL_blit.h"
#include "PSPL_pixels_c.h"
#include "PSPL_events_c.h"
//#include "PSPL_cursor_c.h"

#include "PSPL_pspvideo.h"

#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb( ... )
#endif




SDL_VideoDevice *current_video = NULL;



/*
 * Get the current information about the video hardware
 */
const SDL_VideoInfo *SDL_GetVideoInfo(void)/* memo: call from SDL_surface.c */
{
	const SDL_VideoInfo *info;
	info = NULL;
	if ( current_video )
	{
		info = &current_video->info;
	}
	return (info);
}



/*
 * Return a pointer to an array of available screen dimensions for the
 * given format, sorted largest to smallest.  Returns NULL if there are
 * no dimensions available for a particular format, or (SDL_Rect **)-1
 * if any dimension is okay for the given format.  If 'format' is NULL,
 * the mode list will be for the format given by SDL_GetVideoInfo()->vfmt
 */
static const SDL_Rect RECT_480x272 =
{
	.x = 0,
	.y = 0,
	.w = 480,
	.h = 272
};
static const SDL_Rect *modelist[] =
{
	&RECT_480x272,
	NULL
};
static SDL_Rect **PSP_List_Modes(_THIS, SDL_PixelFormat *format, u32 flags)
{
	#define IS_SWSURFACE(flags) 	((flags & SDL_HWSURFACE) == SDL_SWSURFACE)
	/* x dimension should be a multiple of PSP_SLICE_SIZE */
	if (IS_SWSURFACE(flags))
	{	return ((SDL_Rect **)-1);	}

	switch(format->BitsPerPixel)
	{
	case 8: /* proxied by a shadow surface */
	case 15:
	case 16:
	case 32:
		return (SDL_Rect **)modelist;
	default:
		return (NULL);
	}
}
static SDL_Rect ** s_PSPL_List_Modes(SDL_PixelFormat *format, u32 flags)
{
//	SDL_VideoDevice *video = current_video;
	SDL_VideoDevice *this  = current_video;
	SDL_Rect **modes;
	modes = NULL;
	if ( SDL_VideoSurface )
	{
		if ( format == NULL )
		{
			format = SDL_VideoSurface->format;
		}
	//	modes = video->List Modes(this, format, flags);//”p~Ï
		modes = PSP_List_Modes(this, format, flags);
	}
	return (modes);
}
/*
 * Check to see if a particular video mode is supported.
 * It returns 0 if the requested mode is not supported under any bit depth,
 * or returns the bits-per-pixel of the closest available mode with the
 * given width and height.	If this bits-per-pixel is different from the
 * one used when setting the video mode, SDL_SetVideoMode() will succeed,
 * but will emulate the requested bits-per-pixel with a shadow surface.
 */
static u8 SDL_closest_depths[4][8] =
{
	/* 8 bit closest depth ordering */
	{ 0, 8, 16, 15, 32, 24, 0, 0 },
	/* 15,16 bit closest depth ordering */
	{ 0, 16, 15, 32, 24, 8, 0, 0 },
	/* 24 bit closest depth ordering */
	{ 0, 24, 32, 16, 15, 8, 0, 0 },
	/* 32 bit closest depth ordering */
	{ 0, 32, 16, 15, 24, 8, 0, 0 }
};


#ifdef macintosh /* MPW optimization bug? */
	#define NEGATIVE_ONE (0xffffffff)
#else
	#define NEGATIVE_ONE (-1)
#endif

static int s_PSPL_VideoModeOK(/*int width, int height,*/ int bpp, u32 flags)
{
	/* Currently 1 and 4 bpp are not supported */
	if ( bpp < 8 || bpp > 32 )
	{
		return (0);
	}
//	if ( (width <= 0) || (height <= 0) )
//	{
//		return (0);
//	}

	/* Search through the list valid of modes */
	SDL_PixelFormat format;
	memset(&format, 0, sizeof(format));
	int supported;
	supported = 0;
	unsigned int table;
//	table = (((bpp+7)/8)-1);
	table = (((bpp+7)>>3)-1);
	SDL_closest_depths[table][0] = bpp;
	SDL_closest_depths[table][7] = 0;
//
	int b;
	int i;
	for (b=0; !supported && SDL_closest_depths[table][b]; b++)
	{
		SDL_Rect **sizes;
		format.BitsPerPixel = SDL_closest_depths[table][b];
		sizes = s_PSPL_List_Modes(&format, flags);
		if ( sizes == (SDL_Rect **)0 )
		{
			/* No sizes supported at this bit-depth */
			continue;
		}
		else
		if (sizes == (SDL_Rect **)NEGATIVE_ONE)
		{
			/* Any size supported at this bit-depth */
			supported = 1;
			continue;
		}
//		else
//		if (current_video->handles_any_size)
//		{
//			/* Driver can center a smaller surface to simulate fullscreen */
//			for (i=0; sizes[i]; i++)
//			{
//			//	if ((sizes[i]->w >= width) && (sizes[i]->h >= height))
//				if ((sizes[i]->w >= /*width*/480) && (sizes[i]->h >= /*height*/272))
//				{
//					supported = 1; /* this mode can fit the centered window. */
//					break;
//				}
//			}
//		}
		else
		{
			for (i=0; sizes[i]; i++)
			{
			//	if ((sizes[i]->w == width) && (sizes[i]->h == height))
				if ((sizes[i]->w == /*width*/480) && (sizes[i]->h == /*height*/272))
				{
					supported = 1;
					break;
				}
			}
		}
	}
	if ( supported )
	{
		b--;
		return (SDL_closest_depths[table][b]);
	}
	else
	{
		return (0);
	}
}

/*
 * Get the closest non-emulated video mode to the one requested
 */
static int s_PSPL_GetVideoMode(int *BitsPerPixel, u32 flags)/*int *w, int *h,*/
{
	int table, b, i;
	int supported;
	int native_bpp;
	SDL_PixelFormat format;
	SDL_Rect **sizes;

	/* Check parameters */
	if ( (*BitsPerPixel) < 8 || (*BitsPerPixel) > 32 )
	{
		SDL_SetError_bbb("Invalid bits per pixel(range is {8...32})");
		return (0);
	}
//	if ((*w <= 0) || (*h <= 0))
//	{
//		SDL_SetError_bbb("Invalid width or height");
//		return (0);
//	}

	/* Try the original video mode, get the closest depth */
	native_bpp = s_PSPL_VideoModeOK(*BitsPerPixel, flags);/* *w, *h,*/
	if (native_bpp == (*BitsPerPixel))
	{
		return (1);
	}
	if (0 < native_bpp)
	{
		(*BitsPerPixel) = native_bpp;
		return (1);
	}

	/* No exact size match at any depth, look for closest match */
	memset(&format, 0, sizeof(format));
	supported = 0;
	table = ((*BitsPerPixel+7)/8)-1;
	SDL_closest_depths[table][0] = *BitsPerPixel;
	SDL_closest_depths[table][7] = SDL_VideoSurface->format->BitsPerPixel;
	for (b=0; !supported && SDL_closest_depths[table][b]; b++)
	{
		int best;
		format.BitsPerPixel = SDL_closest_depths[table][b];
		sizes = s_PSPL_List_Modes(&format, flags);
		if ( sizes == (SDL_Rect **)0 )
		{
			/* No sizes supported at this bit-depth */
			continue;
		}
		best = 0;
		for (i=0; sizes[i]; i++)
		{
			/* Mode with both dimensions bigger or equal than asked ? */
			if ((sizes[i]->w >= /* *w*/480) && (sizes[i]->h >= /* *h*/272))
			{
				/* Mode with any dimension smaller or equal than current best ? */
				if ((sizes[i]->w <= sizes[best]->w) || (sizes[i]->h <= sizes[best]->h))
				{
					best = i;
					supported = 1;
				}
			}
		}
		if (supported)
		{
//480		*w=sizes[best]->w;
//272		*h=sizes[best]->h;
			*BitsPerPixel = SDL_closest_depths[table][b];
		}
	}
	if ( ! supported )
	{
	//	SDL_SetError_bbb("No video mode large enough for %dx%d", *w, *h);
		SDL_SetError_bbb("No video mode large enough for 480x272" );
	}
	return (supported);
}

/*
 * Create a shadow surface suitable for fooling the app. :-)
 */
static void SDL_CreateShadowSurface(int depth)
{
	u32 Rmask, Gmask, Bmask;

	/* Allocate the shadow surface */
	if ( depth == (SDL_VideoSurface->format)->BitsPerPixel )
	{
		Rmask = (SDL_VideoSurface->format)->Rmask;
		Gmask = (SDL_VideoSurface->format)->Gmask;
		Bmask = (SDL_VideoSurface->format)->Bmask;
	}
	else
	{
		Rmask = Gmask = Bmask = 0;
	}
	SDL_ShadowSurface = SDL_CreateRGBSurface(SDL_SWSURFACE,
				SDL_VideoSurface->w, SDL_VideoSurface->h,
						depth, Rmask, Gmask, Bmask, 0);
	if ( SDL_ShadowSurface == NULL )
	{
		return;
	}

	/* 8-bit shadow surfaces report that they have exclusive palette */
	if ( SDL_ShadowSurface->format->palette )
	{
		SDL_ShadowSurface->flags |= SDL_HWPALETTE;
		if ( depth == (SDL_VideoSurface->format)->BitsPerPixel )
		{
			memcpy(SDL_ShadowSurface->format->palette->colors,
				SDL_VideoSurface->format->palette->colors,
				SDL_VideoSurface->format->palette->ncolors*
							sizeof(SDL_Color));
		}
		else
		{
			SDL_DitherColors(SDL_ShadowSurface->format->palette->colors, depth);
		}
	}

	/* If the video surface is resizable, the shadow should say so */
	if ( (SDL_VideoSurface->flags & SDL_RESIZABLE) == SDL_RESIZABLE )
	{
		SDL_ShadowSurface->flags |= SDL_RESIZABLE;
	}
	/* If the video surface has no frame, the shadow should say so */
	if ( (SDL_VideoSurface->flags & SDL_NOFRAME) == SDL_NOFRAME )
	{
		SDL_ShadowSurface->flags |= SDL_NOFRAME;
	}
	/* If the video surface is fullscreen, the shadow should say so */
	if ( (SDL_VideoSurface->flags & SDL_FULLSCREEN) == SDL_FULLSCREEN )
	{
		SDL_ShadowSurface->flags |= SDL_FULLSCREEN;
	}
	/* If the video surface is flippable, the shadow should say so */
	if ( (SDL_VideoSurface->flags & SDL_DOUBLEBUF) == SDL_DOUBLEBUF )
	{
		SDL_ShadowSurface->flags |= SDL_DOUBLEBUF;
	}
//	return;
}

/*
 * Set the requested video mode, allocating a shadow buffer if necessary.
 */
/*static*/extern SDL_Surface *SDLVIDEO_PSP_SetVideoMode480x272(_THIS, SDL_Surface *current, /*int width_480, int height_272,*/ int bpp, u32 flags);
SDL_Surface * SDL_SetVideoMode(int dummy_width, int dummy_height, int bpp, u32 flags)
{
	SDL_VideoDevice *video, *this;
	SDL_Surface *prev_mode, *mode;
	int video_bpp;

	/* Start up the video driver, if necessary..
	   WARNING: This is the only function protected this way!
	 */
	if ( ! current_video )
	{
		if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE) < 0 )
		{
			return (NULL);
		}
	}
	this = video = current_video;

	/* Default to the current video bpp */
	if ( bpp == 0 )
	{
		flags |= SDL_ANYFORMAT;
		bpp = SDL_VideoSurface->format->BitsPerPixel;
	}

	/* Get a good video mode, the closest one possible */
//	int video_w;	video_w = width;
//	int video_h;	video_h = height;
	video_bpp = bpp;
	if ( ! s_PSPL_GetVideoMode(/*&video_w, &video_h,*/ &video_bpp, flags) )
	{
		return (NULL);
	}

	/* Check the requested flags */
	/* There's no palette in > 8 bits-per-pixel mode */
	if ( video_bpp > 8 )
	{
		flags &= ~SDL_HWPALETTE;
	}
	#if 0
	if ( (flags&SDL_FULLSCREEN) != SDL_FULLSCREEN )
	{
		/* There's no windowed double-buffering */
		flags &= ~SDL_DOUBLEBUF;
	}
	#endif
	if ( (flags&SDL_DOUBLEBUF) == SDL_DOUBLEBUF )
	{
		/* Use hardware surfaces when double-buffering */
		flags |= SDL_HWSURFACE;
	}

	/* Clean up any previous video mode */
	if ( SDL_PublicSurface != NULL )
	{
		SDL_PublicSurface = NULL;
	}
	if ( SDL_ShadowSurface != NULL )
	{
		SDL_Surface *ready_to_go;
		ready_to_go = SDL_ShadowSurface;
		SDL_ShadowSurface = NULL;
		SDL_FreeSurface(ready_to_go);
	}
//”p~	if ( video->physpal )
//”p~	{
//”p~		free(video->physpal->colors);
//”p~		free(video->physpal);
//”p~		video->physpal = NULL;
//”p~	}
//”p~	if ( video->gammacols)
//”p~	{
//”p~		free(video->gammacols);
//”p~		video->gammacols = NULL;
//”p~	}

	/* Try to set the video mode, along with offset and clipping */
	prev_mode = SDL_VideoSurface;
	SDL_VideoSurface = NULL;	/* In case it's freed by driver */
//	mode = video->SetVideoMode(this, prev_mode, /*video_w*/480, /*video_h*/272, video_bpp, flags);
	mode = SDLVIDEO_PSP_SetVideoMode480x272(this, prev_mode, video_bpp, flags);
	if ( mode ) 	/* Prevent resize events from mode change */
	{
//”p~Ï	SDL_PrivateResize(mode->w, mode->h);
	}
	/*
	 * rcg11292000
	 * If you try to set an SDL_OPENGL surface, and fail to find a
	 * matching  visual, then the next call to SDL_SetVideoMode()
	 * will segfault, since  we no longer point to a dummy surface,
	 * but rather NULL.
	 * Sam 11/29/00
	 * WARNING, we need to make sure that the previous mode hasn't
	 * already been freed by the video driver.	What do we do in
	 * that case?  Should we call PSPL_VideoInit() again?
	 */
	SDL_VideoSurface = (mode != NULL) ? mode : prev_mode;

	if ( (mode != NULL)	)
	{
		/* Sanity check */
//		if ( (mode->w < /*width*/480) || (mode->h < /*height*/272) )
//		{
//			SDL_SetError_bbb("Video mode smaller than requested");
//			return (NULL);
//		}

		/* If we have a palettized surface, create a default palette */
		if ( mode->format->palette )
		{
			SDL_PixelFormat *vf = mode->format;
			SDL_DitherColors(vf->palette->colors, vf->BitsPerPixel);
//”p~Ï			video->SetColors(this, 0, vf->palette->ncolors, vf->palette->colors);
		}

		/* Clear the surface to black */
//”p~		video->offset_x 	= (0);
//”p~		video->offset_y 	= (0);
		mode->offset		= (0);
		SDL_SetClipRect(mode, NULL);
		#if 0/*(‚Æ‚è‚ ‚¦‚¸)*/
		SDL_ClearSurface(mode);// –{—ˆA‹N“®(‰æ–Êƒ‚[ƒhØ‚è‘Ö‚¦)‚É‰æ–ÊÁ‚·‹@”\B
		#endif
		/* Now adjust the offsets to match the desired mode */
	//	video->offset_x = ((/*mode->w*/480-/*width*/480)/2);
	//	video->offset_y = ((/*mode->h*/272-/*height*/272)/2);
//”p~		video->offset_x = (0);
//”p~		video->offset_y = (0);
		mode->offset = (0);//(video->offset_y * mode->pitch) + (video->offset_x * mode->format->BytesPerPixel);
		#ifdef DEBUG_VIDEO
		fprintf(stderr,
			"Requested mode: %dx%dx%d, obtained mode %dx%dx%d (offset %d)\n",
			width, height, bpp,
			mode->w, mode->h, mode->format->BitsPerPixel, mode->offset);
		#endif
		mode->w = /*width*/480;
		mode->h = /*height*/272;
		SDL_SetClipRect(mode, NULL);
	}

	/* If we failed setting a video mode, return NULL... (Uh Oh!) */
	if ( mode == NULL )
	{
		return (NULL);
	}

	/* If there is no window manager, set the SDL_NOFRAME flag */
	if ( ! video->info.wm_available )
	{
		mode->flags |= SDL_NOFRAME;
	}

	/* Create a shadow surface if necessary */
	/* There are three conditions under which we create a shadow surface:
		1.	We need a particular bits-per-pixel that we didn't get.
		2.	We need a hardware palette and didn't get one.
		3.	We need a software surface and got a hardware surface.
	*/
	if (
		 (
		 (	!(flags&SDL_ANYFORMAT) &&
			(SDL_VideoSurface->format->BitsPerPixel != bpp)) ||
		 (	 (flags&SDL_HWPALETTE) &&
				!(SDL_VideoSurface->flags&SDL_HWPALETTE)) ||
		/* If the surface is in hardware, video writes are visible
		   as soon as they are performed, so we need to buffer them
		 */
		 (	 ((flags&SDL_HWSURFACE) == SDL_SWSURFACE) &&
				(SDL_VideoSurface->flags&SDL_HWSURFACE)) ||
		 (	 (flags&SDL_DOUBLEBUF) &&
				(SDL_VideoSurface->flags&SDL_HWSURFACE) &&
				!(SDL_VideoSurface->flags&SDL_DOUBLEBUF))
		 ) )
	{
		SDL_CreateShadowSurface(bpp);
		if ( SDL_ShadowSurface == NULL )
		{
			SDL_SetError_bbb("Couldn't create shadow surface");
			return (NULL);
		}
		SDL_PublicSurface = SDL_ShadowSurface;
	}
	else
	{
		SDL_PublicSurface = SDL_VideoSurface;
	}
	video->info.vfmt = SDL_VideoSurface->format;

	/* We're done! */
	return (SDL_PublicSurface);
}





#if (1)/* call from src/core/menu/scenario_script.c */
/*
 * Convert a surface into the video pixel format.
 */
/*global*/ SDL_Surface *SDL_DisplayFormat(SDL_Surface *surface)
{
	u32 flags;
	#if (000)/* NOT support check. */
	if ( ! SDL_PublicSurface )
	{
		// SDL‚ÌƒrƒfƒIƒ‚[ƒh‚ªˆê“x‚àƒZƒbƒg‚³‚ê‚Ä‚È‚¢‚æB
		SDL_SetError_bbb("No video mode has been set");
		return (NULL);
	}
	#endif
	#if (000)/* NOT support get SDL_HWSURFACE. */
	/* Set the flags appropriate for copying to display surface */
	if (((SDL_PublicSurface->flags&SDL_HWSURFACE) == SDL_HWSURFACE) && current_video->info.blit_hw)
	{	flags = SDL_HWSURFACE;}
	else
	#endif
	{	flags = SDL_SWSURFACE;}
	#ifdef AUTORLE_DISPLAYFORMAT
	flags |= (surface->flags & (SDL_SRCCOLORKEY|SDL_SRCALPHA));
	flags |= SDL_RLEACCELOK;
	#else
	flags |= surface->flags & (SDL_SRCCOLORKEY|SDL_SRCALPHA|SDL_RLEACCELOK);
	#endif
	return (SDL_ConvertSurface(surface, SDL_PublicSurface->format, flags));
}
#endif


/*
 * Clean up the video subsystem
 */
extern void SDLVIDEO_PSP_VideoQuit(_THIS);
/*static*/extern void SDLVIDEO_PSP_DeleteDevice(SDL_VideoDevice *device);
void PSPL_VideoQuit(void)/*only exit once*/
{
	if ( current_video )
	{
		/* Halt event processing before doing anything else */
//”p~Ï	SDL_StopEventLoop();

		/* Clean up allocated window manager items */
		if ( SDL_PublicSurface )
		{
			SDL_PublicSurface = NULL;
		}
		/* Just in case... */

		{
		//	SDL_VideoDevice *video = current_video;
			SDL_VideoDevice *this  = current_video;
			/* Clean up the system video */
		//	video->VideoQuit(this);
			SDLVIDEO_PSP_VideoQuit(this);

			/* Free any lingering surfaces */
			{
				SDL_Surface *ready_to_go;
				ready_to_go 		= SDL_ShadowSurface;
				SDL_ShadowSurface	= NULL;
				SDL_FreeSurface(ready_to_go);
				if ( SDL_VideoSurface != NULL )
				{
					ready_to_go 		= SDL_VideoSurface;
					SDL_VideoSurface	= NULL;
					SDL_FreeSurface(ready_to_go);
				}
			}
			SDL_PublicSurface = NULL;

			/* Clean up miscellaneous memory */
//”p~			if ( video->physpal )
//”p~			{
//”p~				free(video->physpal->colors);
//”p~				free(video->physpal);
//”p~				video->physpal = NULL;
//”p~			}
//”p~			if ( video->gammacols ) 			{	free(video->gammacols); 	video->gammacols = NULL;	}
//”p~			if ( video->gamma ) 				{	free(video->gamma); 		video->gamma = NULL;		}
			/* Finish cleaning up video subsystem */
		//	video->free(this);
			SDLVIDEO_PSP_DeleteDevice(this);
			current_video = NULL;
		}
	}
//	return;
}

/*
 * Initialize the video and event subsystems -- determine native pixel format
 */
extern SDL_VideoDevice *PSP_CreateDevice(int devindex);
/*static*/extern int SDLVIDEO_PSP_VideoInit(_THIS, SDL_PixelFormat *vformat);
void PSPL_VideoInit(void)/*only boot once*/
{
	SDL_PixelFormat vformat;
	u32 video_flags;
//
	/* Toggle the event thread flags, based on OS requirements */
	#if defined(MUST_THREAD_EVENTS)
	flags |= SDL_INIT_EVENTTHREAD;
	#elif defined(CANT_THREAD_EVENTS)
	if ( (flags & SDL_INIT_EVENTTHREAD) == SDL_INIT_EVENTTHREAD )
	{
		SDL_SetError_bbb("OS doesn't support threaded events");
		return (-1);
	}
	#endif
//
	/* Select the proper video driver */
	SDL_VideoDevice *video;
	{
		/* Available video drivers */
//		static VideoBootStrap *bootstrap[] =
//		{
//		//	#ifdef ENABLE_PSP
//			&PSP_bootstrap,
//		//	#endif
//			NULL
//		};
	//	video = NULL;
	//	video = bootstrap[0/*i*/]->create(0/*index*/);
		video = PSP_CreateDevice(0/*index*/);
		current_video = video;
//????		current_video->name = bootstrap[0/*i*/]->name;
	}
	/* Do some basic variable initialization */
	video->screen		= NULL;
	video->shadow		= NULL;
	video->visible		= NULL;
//”p~	video->physpal		= NULL;
//”p~		video->gammacols	= NULL;
//”p~		video->gamma		= NULL;
//”p~	video->offset_x 	= 0;
//”p~	video->offset_y 	= 0;
	memset(&video->info, 0, (sizeof video->info));

	video->displayformatalphapixel		= NULL;

	/* Initialize the video subsystem */
	memset(&vformat, 0, sizeof(vformat));
//	video->VideoInit(video, &vformat);
	SDLVIDEO_PSP_VideoInit(video, &vformat);
//	if (  < 0 )
//	{
//		//–³‚µ	SDL_Vid eoQuit();
//		return (-1);
//	}

	/* Create a zero sized video surface of the appropriate format */
	video_flags = SDL_SWSURFACE;
	SDL_VideoSurface = SDL_CreateRGBSurface(video_flags, 0, 0,
				vformat.BitsPerPixel,
				vformat.Rmask, vformat.Gmask, vformat.Bmask, 0);
//	if ( SDL_VideoSurface == NULL )
//	{
//		//–³‚µ	SDL_Vid eoQuit();
//		return (-1);
//	}
	SDL_PublicSurface = NULL;		/* Until SDL_SetVideoMode() */

#if 0 /* Don't change the current palette - may be used by other programs.
	   * The application can't do anything with the display surface until
	   * a video mode has been set anyway. :)
	   */
	/* If we have a palettized surface, create a default palette */
	if ( SDL_VideoSurface->format->palette )
	{
		SDL_PixelFormat *vf = SDL_VideoSurface->format;
		SDL_DitherColors(vf->palette->colors, vf->BitsPerPixel);
		video->SetColors(video,
				 0, vf->palette->ncolors, vf->palette->colors);
	}
#endif
	video->info.vfmt = SDL_VideoSurface->format;


}
//#include "not_use_PSPL_video.c" // g—p‚µ‚È‚¢•”•ª‚ª‘½‚¢‚Ì‚Å•ªŠ„
