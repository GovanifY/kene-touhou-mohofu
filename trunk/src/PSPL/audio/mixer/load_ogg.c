
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
---------------------------------------------------------*/

/*
	SDL_mixer:	An audio mixer library based on the SDL library
	Copyright (C) 1997-2004 Sam Lantinga

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

	This is the source needed to decode an Ogg Vorbis into a waveform.
	This file by Vaclav Slavik (vaclav.slavik@matfyz.cz).
*/

/* $Id: load_ogg.c,v 1.3 2004/01/04 17:37:04 slouken Exp $ */
#ifdef _SDL_MIXER_H
error! "このプログラムは SDL_mixer.h がインクルードされていると、コンパイルできません。"
#endif
#define _SDL_MIXER_H

#if defined(OGG_MUSIC)

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <string.h>

#if defined(USE_TREMOR)
	#include <tremor/ivorbisfile.h>
#else
	#include <vorbis/vorbisfile.h>
#endif

#include "./../../include/PSPL_audio.h"
#include "000SDL_mixer.h"//#include <SD L/SD L_mixer.h>//#include "SD L_mixer.h"

//#include "SDL_SDL_mutex.h"//#include <SD L/SDL_mutex.h>//#include "SD L_mutex.h"
	//#include "SDL_SDL_endian.h"//#include <SD L/SDL_endian.h>//#include "SD L_endian.h"
	//#include "SDL_SDL_timer.h"//#include <SD L/SDL_timer.h>//#include "SD L_timer.h"

//#include "000SDL_mixer.h"//#include <SD L/SD L_mixer.h>//#include "SD L_mixer.h"
	//#include "SDL_SDL_rwops.h"

#include "load_ogg.h"

#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb(aaa)
#endif

//static /*size_t*/void sdl_read_func_ void(void *ptr, size_t size, size_t nmemb, void *datasource)
static size_t sdl_read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
//	/*return*/ SDL_RWread_ void((SDL_RWops*)datasource, ptr, size, nmemb);
	return SDL_RWread((SDL_RWops*)datasource, ptr, size, nmemb);
}

static int sdl_seek_func(void *datasource, ogg_int64_t offset, int whence)
{
	return SDL_RWseek((SDL_RWops*)datasource, (int)offset, whence);
}

static int sdl_close_func_freesrc(void *datasource)
{
	return SDL_RWclose((SDL_RWops*)datasource);
}

static int sdl_close_func_nofreesrc(void *datasource)
{
	return SDL_RWseek((SDL_RWops*)datasource, 0, SEEK_SET);
}

static long sdl_tell_func(void *datasource)
{
	return SDL_RWtell((SDL_RWops*)datasource);
}


/* don't call this directly; use Mix_LoadWAV_RW() for now. */
PSPL_AUDIO_SPEC *Mix_LoadOGG_RW(
	SDL_RWops *src,
	int freesrc,
	PSPL_AUDIO_SPEC *spec,
	u8 **audio_buf,
	u32 *audio_len)
{
	OggVorbis_File vf;
	ov_callbacks callbacks;
	vorbis_info *info;
	u8 *buf;
	int bitstream = -1;
	long samplesize;
	long samples;
	int read;
	int to_read;
	int must_close = 1;
	int was_error = 1;

	if ( (!src) || (!audio_buf) || (!audio_len) )	/* sanity checks. */
	{	goto done;}

//	callbacks.read_func_ void 	= sdl_read_func_ void;
	callbacks.read_func 		= sdl_read_func;
	callbacks.seek_func 		= sdl_seek_func;
	callbacks.tell_func 		= sdl_tell_func;
	callbacks.close_func 		= freesrc ? sdl_close_func_freesrc : sdl_close_func_nofreesrc;

	if (ov_open_callbacks(src, &vf, NULL, 0, callbacks) != 0)
	{
		SDL_SetError_bbb("OGG bitstream is not valid Vorbis stream!");
		goto done;
	}

	must_close = 0;

	info = ov_info(&vf, -1);

	*audio_buf = NULL;
	*audio_len = 0;
	memset(spec, '\0', sizeof (PSPL_AUDIO_SPEC));

	spec->format		= AUDIO_S16;
//(2)pspステレオ再生固定	spec->channels		= info->channels;
	spec->freq			= info->rate;
	spec->samples		= 4096; 	/* buffer size */

	samples 			= (long)ov_pcm_total(&vf, -1);

	*audio_len			= spec->size_1024/*(とりあえずテスト)*/ = (samples) * (2)/*spec->channels*/ * (2);// (2)pspステレオ再生固定
	*audio_buf			= malloc(*audio_len);
	if (*audio_buf == NULL)
	{	goto done;}

	buf = *audio_buf;
	to_read = *audio_len;
	#if defined(USE_TREMOR)
	for (read = ov_read(&vf, (char *)buf, to_read, &bitstream);
	#else
	for (read = ov_read(&vf, (char *)buf, to_read, 0/*LE*/, 2/*16bit*/, 1/*signed*/, &bitstream);
	#endif
		read > 0;
		#if defined(USE_TREMOR)
		read = ov_read(&vf, (char *)buf, to_read, &bitstream))
		#else
		read = ov_read(&vf, (char *)buf, to_read, 0, 2, 1, &bitstream))
		#endif
	{
		if (read == OV_HOLE || read == OV_EBADLINK)
		{	break; /* error */}

		to_read -= read;
		buf += read;
	}

	ov_clear(&vf);
	was_error = 0;

	/* Don't return a buffer that isn't a multiple of samplesize */
	samplesize = ((spec->format & 0xff)/8) * (2)/*spec->channels*/;// (2)pspステレオ再生固定
	*audio_len &= ~(samplesize-1);

done:
	if (src && must_close)
	{
		if (freesrc)
		{	SDL_RWclose(src);}
		else
		{	SDL_RWseek(src, 0, SEEK_SET);}
	}

	if ( was_error )
	{	spec = NULL;}

	return (spec);
} /* Mix_LoadOGG_RW */

/* end of load_ogg.c ... */

#endif

