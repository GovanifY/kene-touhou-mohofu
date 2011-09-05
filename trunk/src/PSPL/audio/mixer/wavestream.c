
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

	Sam Lantinga
	slouken@libsdl.org
*/

/* $Id: wavestream.c,v 1.19 2004/01/04 17:37:04 slouken Exp $ */

/* This file supports streaming WAV files, without volume adjustment */
#ifdef _SDL_MIXER_H
error! "このプログラムは SDL_mixer.h がインクルードされていると、コンパイルできません。"
#endif
#define _SDL_MIXER_H

#include <stdlib.h>
#include <malloc.h>

#include <string.h>

#include "./../../include/PSPL_audio.h"
#include "./000SDL_mixer.h"//#define MIX_MAX_VOLUME			128 	/* Volume of a chunk */

//#include <SD L/SDL_audio.h>//#include "SD L_audio.h"
//#include <SD L/SDL_mutex.h>//#include "SD L_mutex.h"
//#include <SD L/SDL_rwops.h>//#include "SD L_rwops.h"
#include "./../../include/PSPL_endian.h"//#include <SD L/SDL_endian.h>//#include "SD L_endian.h"

//#include <SD L/SD L_mixer.h>//#include "SD L_mixer.h"
#include "./wavestream.h"

#ifndef Mix_SetError_bbb
	#define Mix_SetError_bbb(...)
#endif

/*
	Taken with permission from SDL_wave.h, part of the SDL library,
	available at: http://www.libsdl.org/
	and placed under the same license as this mixer library.
*/

/* WAVE files are little-endian */

/*******************************************/
/* Define values for Microsoft WAVE format */
/*******************************************/
#define RIFF	0x46464952	/* "RIFF" */
#define WAVE	0x45564157	/* "WAVE" */
#define FACT	0x74636166	/* "fact" */
#define LIST	0x5453494c	/* "LIST" */
#define FMT 	0x20746D66	/* "fmt " */
#define DATA	0x61746164	/* "data" */
#define PCM_CODE	1
#define ADPCM_CODE	2
#define WAVE_MONO	1
#define WAVE_STEREO 2

/* Normally, these three chunks come consecutively in a WAVE file */
typedef struct WaveFMT
{
/* Not saved in the chunk we read:
	u32 	FMTchunk;
	u32 	fmtlen;
*/
	u16 	encoding;
	u16 	channels;		/* 1 = mono, 2 = stereo */
	u32 	frequency;		/* One of 11025, 22050, or 44100 Hz */
	u32 	byterate;		/* Average bytes per second */
	u16 	blockalign; 	/* Bytes per sample block */
	u16 	bitspersample;	/* One of 8, 12, 16, or 4 for ADPCM */
} WaveFMT;

/* The general chunk found in the WAVE file */
typedef struct Chunk
{
	u32 	magic;
	u32 	length;
	u8 		*data;		/* Data includes magic and length */
} Chunk;

/*********************************************/
/* Define values for AIFF (IFF audio) format */
/*********************************************/
#define FORM	0x4d524f46	/* "FORM" */
#define AIFF	0x46464941	/* "AIFF" */
#define SSND	0x444e5353	/* "SSND" */
#define COMM	0x4d4d4f43	/* "COMM" */


/* Currently we only support a single stream at a time */
static WAVStream *music = NULL;

/* This is the format of the audio mixer data */
static PSPL_AUDIO_SPEC mixer;
static unsigned int wavestream_volume = MIX_MAX_VOLUME;

/* Function to load the WAV/AIFF stream */
//static FILE *LoadWAVStream (const char *file, PSPL_AUDIO_SPEC *spec, long *start, long *stop);
//static FILE *LoadAIFFStream (const char *file, PSPL_AUDIO_SPEC *spec, long *start, long *stop);




/* Function to load the WAV stream */

static int WAV_ReadChunk(SDL_RWops *src, Chunk *chunk, int read_data)
{
	chunk->magic	= SDL_ReadLE32(src);
	chunk->length	= SDL_ReadLE32(src);
	if ( read_data )
	{
		chunk->data = (u8 *)malloc(chunk->length);
		if ( chunk->data == NULL )
		{
			Mix_SetError_bbb("Out of memory");
			return (-1);
		}
		if ( SDL_RWread(src, chunk->data, chunk->length, 1) != 1 )
		{
			Mix_SetError_bbb("Couldn't read chunk");
			free(chunk->data);
			return (-1);
		}
	}
	else
	{
		SDL_RWseek(src, chunk->length, SEEK_CUR);
	}
	return (chunk->length);
}
static FILE *LoadWAVStream (const char *file, PSPL_AUDIO_SPEC *spec, long *start, long *stop)
{
	int was_error;
	FILE *wavefp;
	SDL_RWops *src;
	Chunk chunk;
	int lenread;

	/* WAV magic header */
	u32 RIFFchunk;
	u32 wavelen;
	u32 WAVEmagic;

	/* FMT chunk */
	WaveFMT *format = NULL;

	/* Make sure we are passed a valid data source */
	was_error = 0;
	wavefp = fopen(file, "rb");
	src = NULL;
	if ( wavefp )
	{
		src = SDL_RWFromFP(wavefp, 0);
	}
	if ( src == NULL )
	{
		was_error = 1;
		goto done;
	}

	/* Check the magic header */
	RIFFchunk	= SDL_ReadLE32(src);
	wavelen 	= SDL_ReadLE32(src);
	WAVEmagic	= SDL_ReadLE32(src);
	if ( (RIFFchunk != RIFF) || (WAVEmagic != WAVE) )
	{
		Mix_SetError_bbb("Unrecognized file type (not WAVE)");
		was_error = 1;
		goto done;
	}

	/* Read the audio data format chunk */
	chunk.data = NULL;
	do {
		/* FIXME! Add this logic to PSPL_LoadWAV_RW() */
		if ( chunk.data )
		{
			free(chunk.data);
		}
		lenread = WAV_ReadChunk(src, &chunk, 1);
		if ( lenread < 0 )
		{
			was_error = 1;
			goto done;
		}
	} while ( (chunk.magic == FACT) || (chunk.magic == LIST) );

	/* Decode the audio data format */
	format = (WaveFMT *)chunk.data;
	if ( chunk.magic != FMT )
	{
		free(chunk.data);
		Mix_SetError_bbb("Complex WAVE files not supported");
		was_error = 1;
		goto done;
	}
	switch (SDL_SwapLE16(format->encoding))
	{
	case PCM_CODE:
		/* We can understand this */
		break;
	default:
		Mix_SetError_bbb("Unknown WAVE data format");
		was_error = 1;
		goto done;
	}
	memset(spec, 0, (sizeof *spec));
	spec->freq = SDL_SwapLE32(format->frequency);
	switch (SDL_SwapLE16(format->bitspersample))
	{
	case  8: 	spec->format = AUDIO_U8;		break;
	case 16:	spec->format = AUDIO_S16;		break;
	default:
		Mix_SetError_bbb("Unknown PCM data format");
		was_error = 1;
		goto done;
	}
	spec->channels_is_load_wave_only 	= (u8) SDL_SwapLE16(format->channels);
	spec->samples	= 4096; /* Good default buffer size */

	/* Set the file offset to the DATA chunk data */
	chunk.data = NULL;
	do
	{
		*start		= SDL_RWtell(src) + 2*sizeof(u32);
		lenread 	= WAV_ReadChunk(src, &chunk, 0);
		if ( lenread < 0 )
		{
			was_error = 1;
			goto done;
		}
	} while ( chunk.magic != DATA );
	*stop = SDL_RWtell(src);

done:
	if ( format != NULL )
	{
		free(format);
	}
	if ( src )
	{
		SDL_RWclose(src);
	}
	if ( 0!= was_error )
	{
		if ( wavefp )
		{
			fclose(wavefp);
			wavefp = NULL;
		}
	}
	return (wavefp);
}


#ifdef USE_WINDOWS_AIFF

/* Function to load the AIFF stream */

/* I couldn't get SANE_to_dou ble() to work, so I stole this from libsndfile.
 * I don't pretend to fully understand it.
 */

static u32 AIFF_SANE_to_u32(u8 *sanebuf)
{
	/* Negative number? */
	if (sanebuf[0] & 0x80)
	{	return (0);}

	/* Less than 1? */
	if (sanebuf[0] <= 0x3F)
	{	return (1);}

	/* Way too big? */
	if (sanebuf[0] > 0x40)
	{	return 0x4000000;}

	/* Still too big? */
	if (sanebuf[0] == 0x40 && sanebuf[1] > 0x1C)
	{	return 800000000;}

	return ((
		(sanebuf[2] << 23) |
		(sanebuf[3] << 15) |
		(sanebuf[4] <<	7) |
		(sanebuf[5] >>	1))
		 >> (29 - sanebuf[1]));
}
static FILE *LoadAIFFStream(
	const char *file, PSPL_AUDIO_SPEC *spec,
	long *start, long *stop)
{
	int was_error;
	int found_SSND;
	int found_COMM;
	FILE *wavefp;
	SDL_RWops *src;

	u32 chunk_type;
	u32 chunk_length;
	long next_chunk;

	/* AIFF magic header */
	u32 FORMchunk;
	u32 AIFFmagic;
	/* SSND chunk	 */
	u32 offset;
	u32 blocksize;
	/* COMM format chunk */
	u16 channels 		= 0;
	u32 numsamples 		= 0;
	u16 samplesize 		= 0;
	u8 sane_freq[10];
	u32 frequency = 0;

	/* Make sure we are passed a valid data source */
	was_error 	= 0;
	wavefp 		= fopen(file, "rb");
	src 		= NULL;
	if ( wavefp )
	{
		src = SDL_RWFromFP(wavefp, 0);
	}
	if ( src == NULL )
	{
		was_error = 1;
		goto done;
	}

	/* Check the magic header */
	FORMchunk		= SDL_ReadLE32(src);
	chunk_length	= SDL_ReadBE32(src);
	AIFFmagic		= SDL_ReadLE32(src);
	if ( (FORMchunk != FORM) || (AIFFmagic != AIFF) )
	{
		Mix_SetError_bbb("Unrecognized file type (not AIFF)");
		was_error = 1;
		goto done;
	}

	/* From what I understand of the specification, chunks may appear in
	 * any order, and we should just ignore unknown ones.
	 *
	 * TODO: Better sanity-checking. E.g. what happens if the AIFF file
	 *	 contains compressed sound data?
	 */

	found_SSND = 0;
	found_COMM = 0;

	do
	{
		chunk_type		= SDL_ReadLE32(src);
		chunk_length	= SDL_ReadBE32(src);
		next_chunk		= SDL_RWtell(src) + chunk_length;

		/* Paranoia to avoid infinite loops */
		if (chunk_length == 0)
		{	break;}

		switch (chunk_type)
		{
		case SSND:
			found_SSND	= 1;
			offset		= SDL_ReadBE32(src);
			blocksize	= SDL_ReadBE32(src);
			*start		= SDL_RWtell(src) + offset;
			break;

		case COMM:
			found_COMM	= 1;

			/* Read the audio data format chunk */
			channels	= SDL_ReadBE16(src);
			numsamples	= SDL_ReadBE32(src);
			samplesize	= SDL_ReadBE16(src);
			SDL_RWread(src, sane_freq, sizeof(sane_freq), 1);
			frequency	= AIFF_SANE_to_u32(sane_freq);
			break;

		default:
			break;
		}
	} while ( (!found_SSND || !found_COMM) &&
		(SDL_RWseek(src, next_chunk, SEEK_SET) != -1) );

	if (!found_SSND)
	{
		Mix_SetError_bbb("Bad AIFF file (no SSND chunk)");
		was_error = 1;
		goto done;
	}

	if (!found_COMM)
	{
		Mix_SetError_bbb("Bad AIFF file (no COMM chunk)");
		was_error = 1;
		goto done;
	}

	*stop = *start + channels * numsamples * (samplesize>>3);/*/8*/

	/* Decode the audio data format */
	memset(spec, 0, (sizeof *spec));
	spec->freq = frequency;
	switch (samplesize)
	{
	case 8: 	spec->format = AUDIO_S8;		break;
	case 16:	spec->format = AUDIO_S16MSB;	break;
	default:
		Mix_SetError_bbb("Unknown samplesize in data format");
		was_error = 1;
		goto done;
	}
	spec->channels	= (u8) channels;
	spec->samples	= 4096; /* Good default buffer size */

done:
	if ( src )
	{
		SDL_RWclose(src);
	}
	if ( was_error )
	{
		if ( wavefp )
		{
			fclose(wavefp);
			wavefp = NULL;
		}
	}
	return (wavefp);
}
#endif	/* USE_WINDOWS_AIFF */


/* Initialize the WAVStream player, with the given mixer settings
   This function returns 0, or -1 if there was an error.
 */
int WAVStream_Init(PSPL_AUDIO_SPEC *mixerfmt)
{
	mixer = *mixerfmt;
	return (0);
}

void WAVStream_SetVolume(int volume)
{
	wavestream_volume = volume;
}

/* Load a WAV stream from the given file */
extern int tiny_strcmp(/*const*/ char *aaa, const char *bbb);
WAVStream *WAVStream_LoadSong(const char *file, const char *magic)
{
	WAVStream *wave;
	PSPL_AUDIO_SPEC wavespec;

	if ( ! mixer.format )
	{
	Mix_SetError_bbb("WAV music output not started");
	return (NULL);
	}
	wave = (WAVStream *)malloc(sizeof *wave);
	if ( wave )
	{
		memset(wave, 0, (sizeof *wave));
		if ( tiny_strcmp((char *)magic, "RIFF") == 0 )
		{
			wave->wavefp = LoadWAVStream(file, &wavespec, &wave->start, &wave->stop);
		}
		#ifdef USE_WINDOWS_AIFF
		else
		if ( tiny_strcmp((char *)magic, "FORM") == 0 )
		{
			wave->wavefp = LoadAIFFStream(file, &wavespec, &wave->start, &wave->stop);
		}
		#endif
		else
		{
			Mix_SetError_bbb("Unknown WAVE format");
		}
		if ( wave->wavefp == NULL )
		{
			free(wave);
			return (NULL);
		}
		PSPL_BuildAudioCVT(
			&wave->cvt,
			//--変換元
			wavespec.format,
			wavespec.channels_is_load_wave_only,
			wavespec.freq,
			//--変換先
			mixer.format,
			#ifdef _OLD_SDL_LINK_MODE_/*(旧互換機能)*/
			(2),//	mixer.channels, //(2)pspステレオ再生固定
			#endif
			mixer.freq);
	}
	return (wave);
}

/* Start playback of a given WAV stream */
void WAVStream_Start(WAVStream *wave)
{
	clearerr(wave->wavefp);
	fseek(wave->wavefp, wave->start, SEEK_SET);
	music = wave;
}

/* Play some of a stream previously started with WAVStream_Start() */
void WAVStream_PlaySome(u8 *stream, int len)
{
	long pos;
	if ( music && ((pos=ftell(music->wavefp)) < music->stop) )
	{
		if ( music->cvt.needed )
		{
			int original_len;
			original_len=0;
			if (0!=music->cvt.len_ratio)
			{
				original_len=/*(int)*/(/*(dou ble)*/len/music->cvt.len_ratio);
			}
			if ( music->cvt.len != original_len )
			{
				int worksize;
				if ( music->cvt.buf != NULL )
				{
					free(music->cvt.buf);
				}
				worksize = original_len*music->cvt.len_mult;
				music->cvt.buf=(u8 *)malloc(worksize);
				if ( music->cvt.buf == NULL )
				{
					return;
				}
				music->cvt.len = original_len;
			}
			if ( (music->stop - pos) < original_len )
			{
				original_len = (music->stop - pos);
			}
			original_len = fread(music->cvt.buf,1,original_len,music->wavefp);
			/* At least at the time of writing, PSPL_ConvertAudio()
			   does byte-order swapping starting at the end of the
			   buffer. Thus, if we are reading 16-bit samples, we
			   had better make damn sure that we get an even
			   number of bytes, or we'll get garbage.
			 */
			if ( (music->cvt.src_format & 0x0010) && (original_len & 1) )
			{
				original_len--;
			}
			music->cvt.len = original_len;
			PSPL_ConvertAudio(&music->cvt);
			/*(!!!)*/
			PSPL_MixAudio(stream, music->cvt.buf, music->cvt.len_cvt, wavestream_volume);
		}
		else
		{
			if ( (music->stop - pos) < len )
			{
				len = (music->stop - pos);
			}
			fread(stream, len, 1, music->wavefp);
		}
	}
}

/* Stop playback of a stream previously started with WAVStream_Start() */
void WAVStream_Stop(void)
{
	music = NULL;
}

/* Close the given WAV stream */
void WAVStream_FreeSong(WAVStream *wave)
{
	if ( wave )
	{
		/* Clean up associated data */
		if ( wave->wavefp )
		{
			fclose(wave->wavefp);
		}
		if ( wave->cvt.buf )
		{
			free(wave->cvt.buf);
		}
		free(wave);
	}
}

/* Return non-zero if a stream is currently playing */
int WAVStream_Active(void)
{
	int active;
	active = 0;
	if ( music && (ftell(music->wavefp) < music->stop) )
	{
		active = 1;
	}
	return (active);
}
