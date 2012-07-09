/*
	SMPEG - SDL MPEG Player Library
	Copyright (C) 1999	Loki Entertainment Software

	- Modified by Michel Darricau from eProcess <mdarricau@eprocess.fr>  for popcorn -

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
	Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/* A class based on the MPEG stream class, used to parse and play audio */

//using namespace std;

#include "MPEGaudio.h"
#include "MPEGstream.h"

#ifndef smpeg_SetError_bbb
	#define smpeg_SetError_bbb(aaa)
#endif

bool MPEGaudio:: WantedSpec(PSPL_AUDIO_SPEC *wanted)
{
	wanted->freq = frequencies[version][frequency];
	#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	wanted->format = AUDIO_S16LSB;
	#else
	wanted->format = AUDIO_S16MSB;
	#endif
	if ( outputstereo )
	{
		wanted->channels = 2;
	}
	else
	{
		wanted->channels = 1;
	}
	wanted->samples 	= 4096;
	wanted->callback	= Play_MPEGaudioSDL;
	wanted->userdata	= this;
	return (true);
}

MPEGaudio:: MPEGaudio(MPEGstream *stream, bool initSDL) : sdl_audio(initSDL)
{
	/* Initialize MPEG audio */
	mpeg = stream;
	initialize();

	/* Just be paranoid.  If all goes well, this will be set to true */
	valid_stream = false;

	/* Analyze the MPEG audio stream */
	if ( loadheader() )
	{
		PSPL_AUDIO_SPEC wanted;
		WantedSpec(&wanted);

		/* Calculate the samples per frame */
		samplesperframe = 32 * wanted.channels;
		if ( layer == 3 )
		{
			samplesperframe *= 18;
			if ( version == 0 )
			{
				samplesperframe *= 2;
			}
		}
		else
		{
			samplesperframe *= SCALEBLOCK;
			if ( layer == 2 )
			{
				samplesperframe *= 3;
			}
		}
		if ( sdl_audio )
		{
			/* ÇƒÇ¢Ç§Ç©ÅAÇ±Ç±Ç≈SDL audio ( PSPL_OpenAudio() )åƒÇŒÇÍÇÈÇ∆, SDL mixer ìIÇ…ÇÕìsçáÇ™à´Ç¢ */
			/* Open the audio, get actual audio hardware format and convert */
			bool audio_active;
			PSPL_AUDIO_SPEC actual;
			audio_active = (PSPL_OpenAudio(&wanted, &actual) == 0);
			if ( audio_active )
			{
				ActualSpec(&actual);
				valid_stream = true;
			}
			else
			{
				smpeg_SetError_bbb(SDL_GetError());
			}
			PSPL_PauseAudio(0);
		}
		else 	/* The stream is always valid if we don't initialize SDL */
		{
			valid_stream = true;
		}
		//volume = (SDL_MIX_MAXVOLUME);//Volume(100);
	}

	/* For using system timestamp */
	{
		int i;
		for (i=0; i<N_TIMESTAMPS; i++)
		{	timestamp[i] = -1;}
	}
}

MPEGaudio:: ~MPEGaudio()
{
#ifdef THREADED_AUDIO
	/* Stop the decode thread */
	StopDecoding();
#endif

	/* Remove ourselves from the mixer hooks */
	Stop();
	if ( sdl_audio )
	{
		/* Close up the audio so others may play */
		PSPL_CloseAudio();//Ç§Ç‹Ç≠Ç¢Ç©Ç»Ç¢
	}
}


void MPEGaudio:: ActualSpec(const PSPL_AUDIO_SPEC *actual)
{
	/* Splay can optimize some of the conversion */
	if ( actual->channels == 1 && outputstereo )
	{
		forcetomonoflag = true;
	}
	if ( actual->channels == 2 && !outputstereo )
	{
		forcetostereoflag = true;
		samplesperframe *= 2;
	}
	/* FIXME: Create an audio conversion block */
	if ( (actual->freq/100) == ((frequencies[version][frequency]/2)/100) )
	{
		downfrequency = 1;
	}
	else if ( actual->freq != frequencies[version][frequency] )
	{
		#ifdef VERBOSE_WARNINGS
		error(0/*ERR_FATAL*/, "Warning: wrong audio frequency (wanted %d, got %d)\n",
				frequencies[version][frequency], actual->freq);
		#else
		;
		#endif
	}
	if ( actual->format != AUDIO_S16SYS )
	{
		if ( (actual->format^0x1000) == AUDIO_S16SYS )
		{
			swapendianflag = true;
		}
		#ifdef PSP_VIRTUAL_CONSOLE_IO
		else
		{
			error(0/*ERR_FATAL*/, "Warning: incorrect audio format\n");
		}
		#endif
	}
	rate_in_s = ((/*dou ble*/float)((actual->format&0xff)/8)*actual->channels*actual->freq);
	stereo = ((actual->channels-1) > 0);
}

#ifdef THREADED_AUDIO
void MPEGaudio:: StartDecoding(void)
{
	decoding = true;
	/* Create the ring buffer to hold audio */
	if ( ! ring )
	{
		ring = new MPEG_ring(samplesperframe*2);
	}
	if ( ! decode_thread )
	{
		decode_thread = SDL_CreateThread(Decode_MPEGaudio, this);
	}
}
void MPEGaudio:: StopDecoding(void)
{
	decoding = false;
	if ( decode_thread )
	{
		force_exit = true;
		if ( ring ) ring->ReleaseThreads();
		SDL_WaitThread(decode_thread, NULL);
		decode_thread = NULL;
	}
	if ( ring )
	{
		delete ring;
		ring = NULL;
	}
}
#endif

#include <psptypes.h>
#include <psprtc.h>
//------------------------------------------------------------
//	psp_ticker
//------------------------------------------------------------

static u64 psp_ticker(void)
{
	u64 current_ticks;
	sceRtcGetCurrentTick(&current_ticks);
	return (current_ticks);
}
static /*cycles_t*/u32 SDL_GetTicks_bbb/*osd_cycles*/(void)
{
//	return ((/*cycles_t*/u32)(psp_ticker()/1000));
	return ((/*cycles_t*/u32)(psp_ticker() /*/1000*/ ));
}

/* MPEG actions */
/*dou ble*/float MPEGaudio:: Time(void)
{
	/*dou ble*/float now;

	if ( frag_time )
	{
		now = (play_time + (/*dou ble*/float)( ((u32)SDL_GetTicks_bbb() - (u32)frag_time) )/1000.0);
	}
	else
	{
		now = play_time;
	}
	return (now);
}
void MPEGaudio:: Play(void)
{
	ResetPause();
	if ( valid_stream )
	{
		#ifdef THREADED_AUDIO
		StartDecoding();
		#endif
		playing = true;
	}
}
void MPEGaudio:: Stop(void)
{
	if ( valid_stream )
	{
		if ( sdl_audio )
		{	PSPL_LockAudio();}

		playing = false;

		if ( sdl_audio )
		{	PSPL_UnlockAudio();}
	}
	ResetPause();
}
void MPEGaudio:: Rewind(void)
{
	Stop();
	#ifdef THREADED_AUDIO
	/* Stop the decode thread */
	StopDecoding();
	#endif
	clearrawdata();
	decodedframe = 0;
	currentframe = 0;
	frags_playing = 0;
}
void MPEGaudio:: ResetSynchro(/*dou ble*/float time)
{
	play_time = time;
	frag_time = 0;

	/* Reinit the timestamp FIFO */
	int i;
	for (i=0; i<N_TIMESTAMPS; i++)
	{	timestamp[i] = -1;}
}
void MPEGaudio:: Skip(float seconds)
{
	/* Called only when there is no timestamp info in the MPEG */
	printf("Audio: Skipping %f seconds...\n", seconds);
	while (seconds > 0)
	{
		seconds -= (float) samplesperframe / ((float) frequencies[version][frequency]*(1+inputstereo));
		if (!loadheader()) break;
	}
}
//void MPEGaudio::Volume(int vol)
//{
//	if ( (vol >= 0) && (vol <= 100) )
//	{
//		volume = (vol*SDL_MIX_MAXVOLUME)/100;
//	}
//}
	/* Michel Darricau from eProcess <mdarricau@eprocess.fr>  conflict name in popcorn */
MPEGstatus MPEGaudio:: GetStatus(void)
{
	if ( valid_stream )
	{
		/* Has decoding stopped because of end of stream? */
		if ( mpeg->eof() && (decodedframe <= currentframe) )
		{
			return (MPEG_STOPPED);
		}
		/* Have we been told to play? */
		if ( playing )
		{
			return (MPEG_PLAYING);
		}
		else
		{
			return (MPEG_STOPPED);
		}
	}
	else
	{
		return (MPEG_ERROR);
	}
}

bool MPEGaudio:: GetAudioInfo(MPEG_AudioInfo *info)
{
	if ( info )
	{
		info->mpegversion	= version;
		info->mode			= mode;
		info->frequency 	= frequencies[version][frequency];
		info->layer 		= layer;
		info->bitrate		= bitrate[version][layer-1][bitrateindex];
		info->current_frame = currentframe;
	}
	return (true);
}
bool MPEGaudio:: fillbuffer(int size)
{
	bitindex=0;
	_buffer_pos = mpeg->pos;
	return (mpeg->copy_data(_buffer, size) > 0);
};

void MPEGaudio:: sync(void)
{
	bitindex = (bitindex+7)&0xfffffff8;
}

bool MPEGaudio:: issync(void)
{
	return ((bitindex&7) != 0);
}

int MPEGaudio::getbyte(void)
{
	int r = (unsigned char)_buffer[bitindex>>3];
	bitindex += 8;
	return (r);
}

int MPEGaudio::getbit(void)
{
	register int r = ((_buffer[bitindex>>3]>>(7-(bitindex&7)))&1);
	bitindex++;
	return (r);
}

int MPEGaudio::getbits8(void)
{
	register unsigned short a;
	{
		int offset = (bitindex>>3);
		a = (((unsigned char)_buffer[offset])<<8) | ((unsigned char)_buffer[offset+1]);
	}
	a <<= (bitindex&7);
	bitindex += 8;
	return ((int)((unsigned int)(a>>8)));
}

int MPEGaudio::getbits9(int bits)
{
	register unsigned short a;
	{
		int offset = bitindex>>3;
		a = (((unsigned char)_buffer[offset])<<8) | ((unsigned char)_buffer[offset+1]);
	}
	a <<= (bitindex&7);
	bitindex += bits;
	return ((int)((unsigned int)(a>>(16-bits))));
}
