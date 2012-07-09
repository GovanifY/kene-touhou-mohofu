/* MPEG/WAVE Sound library

   (C) 1997 by Jung woo-jae */

// Mpegtoraw.cc
// Server which get mpeg format and put raw format.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>
#include "psp_vfpu.h"/* ok(mp3�Đ�����ok���ۂ�) */

#include <stdlib.h>
#include <string.h>

#ifdef DEBUG_SYSTEM
	#include <assert.h>
#endif

#include "MPEGaudio.h"
#include "MPEGstream.h"
#if defined(_WIN32)
	#include <windows.h>
#endif

#include <SDL/SDL_endian.h>//#include "SD L_endian.h"

#if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
	#define _KEY 0
#else
	#define _KEY 3
#endif

int MPEGaudio::getbits( int bits )
{
	union
	{
		char store[4];
		int current;
	} u;
	int bi;

	if ( ! bits )
	{	return (0);}

	u.current = 0;
	bi = (bitindex & 7);
	u.store[ _KEY ] = _buffer[ bitindex >> 3 ] << bi;
	bi = 8 - bi;
	bitindex += bi;

	while ( bits )
	{
		if ( ! bi )
		{
			u.store[ _KEY ] = _buffer[ bitindex >> 3 ];
			bitindex += 8;
			bi = 8;
		}

		if ( bits >= bi )
		{
			u.current <<= bi;
			bits -= bi;
			bi = 0;
		}
		else
		{
			u.current <<= bits;
			bi -= bits;
			bits = 0;
		}
	}
	bitindex -= bi;

	return ( u.current >> 8 );
}


// Convert mpeg to raw
// Mpeg headder class
void MPEGaudio::initialize()
{
	static bool initialized = false;

	last_speed			= 0;
	stereo				= true;
	forcetomonoflag 	= false;
	forcetostereoflag	= false;
	swapendianflag		= false;
	downfrequency		= 0;

	scalefactor 		= SCALE;
	calcbufferoffset	= 15;
	currentcalcbuffer	= 0;

	register REAL *s1;
	register REAL *s2;
	REAL *s3;
	REAL *s4;

	s1 = calcbufferL[0];
	s2 = calcbufferR[0];
	s3 = calcbufferL[1];
	s4 = calcbufferR[1];
	register int i;
	for (i=CALCBUFFERSIZE-1; i>=0; i--)
	{
		calcbufferL[0][i] = calcbufferL[1][i] =
		calcbufferR[0][i] = calcbufferR[1][i] = 0.0f;
	}

	if ( ! initialized )
	{
	//	#define MY_PI 3.14159265358979323846
		for (i=0;i<16;i++)	{	hcos_64[i] = (float)(1.0f / (2.0f * vfpu_cosf((/*dou ble*/float)((/*dou ble*/float)(i+i+1)*((/*dou ble*/float)(M_PI/64.0f))) )));}
		for (i=0;i< 8;i++)	{	hcos_32[i] = (float)(1.0f / (2.0f * vfpu_cosf((/*dou ble*/float)((/*dou ble*/float)(i+i+1)*((/*dou ble*/float)(M_PI/32.0f))) )));}
		for (i=0;i< 4;i++)	{	hcos_16[i] = (float)(1.0f / (2.0f * vfpu_cosf((/*dou ble*/float)((/*dou ble*/float)(i+i+1)*((/*dou ble*/float)(M_PI/16.0f))) )));}
		for (i=0;i< 2;i++)	{	hcos_8 [i] = (float)(1.0f / (2.0f * vfpu_cosf((/*dou ble*/float)((/*dou ble*/float)(i+i+1)*((/*dou ble*/float)(M_PI/ 8.0f))) )));}
							{	hcos_4	   = (float)(1.0f / (2.0f * vfpu_cosf((/*dou ble*/float)( 				   (1.0f) *((/*dou ble*/float)(M_PI/ 4.0f))) )));}
		initialized = true;
	}

	layer3initialize();

#ifdef THREADED_AUDIO
	decode_thread = NULL;
	ring = NULL;
#endif
	Rewind();
	ResetSynchro(0);
};


bool MPEGaudio::loadheader()
{
	register int c;
	bool flag;
	flag = false;
	do
	{
		if ( (c = mpeg->copy_byte()) < 0 )
		{	break;}
		if ( c == 0xff )
		{
			while ( ! flag )
			{
				if ( (c = mpeg->copy_byte()) < 0 )
				{
					flag = true;
					break;
				}
				if ( (c & 0xf0) == 0xf0 )
				{
					flag = true;
					break;
				}
				else if ( c != 0xff )
				{
					break;
				}
			}
		}
	} while ( ! flag );

	if ( c < 0 )
	{	return (false);}

	// Analyzing
	c &= 0xf;
	protection = c & 1;
	layer = 4 - ((c >> 1) & 3);
	version = (_mpegversion) ((c >> 3) ^ 1);

	c = mpeg->copy_byte() >> 1;
	padding = (c & 1);
	c >>= 1;
	frequency = (_frequency) (c&3);
	if (frequency == 3)
	{	return (false);}
	c >>= 2;
	bitrateindex = (int) c;
	if ( bitrateindex == 15 )
	{	return (false);}

	c = ((unsigned int)mpeg->copy_byte()) >> 4;
	extendedmode = c & 3;
	mode = (_mode) (c >> 2);


	// Making information

	inputstereo = (mode == single) ? 0 : 1;

	forcetomonoflag 	= (!stereo &&  inputstereo);
	forcetostereoflag 	= ( stereo && !inputstereo);

	if (forcetomonoflag)
			{	outputstereo = 0;}
	else	{	outputstereo = inputstereo;}

	channelbitrate = bitrateindex;
	if (inputstereo)
	{
		if (channelbitrate==4)
				{	channelbitrate = 1;}
		else	{	channelbitrate -= 4;}
	}

	if (channelbitrate==1 || channelbitrate==2)
			{	tableindex=0;}
	else	{	tableindex=1;}

	if (layer==1)
	{  subbandnumber = MAXSUBBAND;}
	else
	{
		if (!tableindex)
		{
			if (frequency==frequency32000)
					{	subbandnumber = 12; }
			else 	{	subbandnumber =  8;	}
		}
		else
		if (frequency==frequency48000||
			(channelbitrate>=3 && channelbitrate<=5))
				{	subbandnumber = 27;}
		else 	{	subbandnumber = 30;}
	}

	if (mode==single)		{	stereobound = 0;}
	else if (mode==joint)	{	stereobound = ((extendedmode+1)<<2);}
	else 					{	stereobound = subbandnumber;}
//
	if (stereobound>subbandnumber)	{	stereobound = subbandnumber;}

	// framesize & slots
	if (layer==1)
	{
		framesize=(12000*bitrate[version][0][bitrateindex])/
			  frequencies[version][frequency];
		if (frequency==frequency44100 && padding)	{framesize++;}
		framesize<<=2;
	}
	else
	{
		framesize = (144000*bitrate[version][layer-1][bitrateindex])/
			(frequencies[version][frequency]<<version);
		if (padding)	{framesize++;}
		if (layer==3)
		{
			if (version)
			{
				layer3slots = framesize-((mode==single)?9:17)
						 -(protection?0:2)
						 -4;
			}
			else
			{
				layer3slots = framesize-((mode==single)?17:32)
						 -(protection?0:2)
						 -4;
			}
		}
	}

	#ifdef DEBUG_AUDIO
	error(0/*ERR_FATAL*/, "MPEG %d audio layer %d (%d kbps), at %d Hz %s [%d]\n", version+1, layer,  bitrate[version][layer-1][bitrateindex], frequencies[version][frequency], (mode == single) ? "mono" : "stereo", framesize);
	#endif

	/* Fill the buffer with new data */
	if (!fillbuffer(framesize-4))
	{	return (false); 	}
//
	if (!protection)
	{
		getbyte();					  // CRC, Not check!!
		getbyte();
	}
	// Sam 7/17 - skip sequences of quickly varying frequencies
	int speed = frequencies[version][frequency];
	if ( speed != last_speed )
	{
		last_speed = speed;
		if ( rawdatawriteoffset )
		{
			++decodedframe;
			#ifndef THREADED_AUDIO
			++currentframe;
			#endif
		}
		return loadheader();
	}
	return (true);
}


bool MPEGaudio::run( int frames, /*dou ble*/float *timestamp)
{
	/*dou ble*/float last_timestamp = -1;
	int totFrames = frames;
	for ( ; frames; frames-- )
	{
		if ( loadheader() == false )
		{
			return (false);
		}
		if (frames == totFrames  && (timestamp != NULL))
		{
			if (last_timestamp != mpeg->timestamp)
			{
				if (mpeg->timestamp_pos <= _buffer_pos)
				{	last_timestamp = (*timestamp) = mpeg->timestamp;}
			}
			else
			{
				*timestamp = -1;
			}
		}
				if ( layer == 3 )	{extractlayer3();}
		else	if ( layer == 2 )	{extractlayer2();}
		else	if ( layer == 1 )	{extractlayer1();}

		/* Handle swapping data endianness */
		if ( swapendianflag )
		{
			s16 *p;
			p = rawdata+rawdatawriteoffset;
			while ( p > rawdata )
			{
				--p;
				*p = SDL_Swap16(*p);
			}
		}

		/* Handle expanding to stereo output */
		if ( forcetostereoflag )
		{
			s16 *in;
			s16 *out;
			in = rawdata+rawdatawriteoffset;
			rawdatawriteoffset *= 2;
			out = rawdata+rawdatawriteoffset;
			while ( in > rawdata )
			{
				--in;
				*(--out) = *in;
				*(--out) = *in;
			}
		}

		// Sam 10/5 - If there is no data, don't increment frames
		if ( rawdatawriteoffset )
		{
			++decodedframe;
			#ifndef THREADED_AUDIO
			++currentframe;
			#endif
		}
	}
	return (true);
}

#ifdef THREADED_AUDIO
int Decode_MPEGaudio(void *udata)
{
	MPEGaudio *audio = (MPEGaudio *)udata;
	/*dou ble*/float timestamp;

#if defined(_WIN32)
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
#endif

	audio->force_exit = false;
	while ( audio->decoding && ! audio->mpeg->eof() && !audio->force_exit )
	{
		audio->rawdata = (s16 *)audio->ring->NextWriteBuffer();
		if ( audio->rawdata )
		{
			audio->rawdatawriteoffset = 0;
			/* Sam 10/5/2000 - Added while to prevent empty buffer in ring */
			while ( audio->run(1, &timestamp) &&
					(audio->rawdatawriteoffset == 0) )
			{
				/* Keep looping */ ;
			}
			if ((u32)audio->rawdatawriteoffset*2 <= audio->ring->BufferSize())
			{
				audio->ring->WriteDone(audio->rawdatawriteoffset*2, timestamp);
			}
		}
	}
	audio->decoding = false;
	return (0);
}
#endif /* THREADED_AUDIO */


#include <psptypes.h>
#include <psprtc.h>
//------------------------------------------------------------
//	psp_ticker
//------------------------------------------------------------

static u64 psp_ticker(void)
{
	u64 current_ticks;
	sceRtcGetCurrentTick(&current_ticks);
	return current_ticks;
}
static /*cycles_t*/u32 SDL_GetTicks_bbb/*osd_cycles*/(void)
{
//	return (/*cycles_t*/u32)(psp_ticker()/1000);
	return (/*cycles_t*/u32)(psp_ticker() /*/1000*/ );
}

// Helper function for SDL audio
int Play_MPEGaudio(MPEGaudio *audio, u8 *stream, int render_len)
{
	//int volume;
	int mixed = 0;

		/* Michel Darricau from eProcess <mdarricau@eprocess.fr>  conflict name in popcorn */
	/* Bail if audio isn't playing */
	if ( audio->GetStatus() != MPEG_PLAYING )
	{
		return (0);
	}
	//volume = audio->volume;

	/* Increment the current play time (assuming fixed frag size) */
	switch (audio->frags_playing++)
	{
	// Vivien: Well... the theorical way seems good to me :-)
	case 0: 	/* The first audio buffer is being filled */
		break;
	case 1: 	/* The first audio buffer is starting playback */
		audio->frag_time = SDL_GetTicks_bbb();
		break;
	default:	/* A buffer has completed, filling a new one */
		audio->frag_time = SDL_GetTicks_bbb();
		audio->play_time += ((/*dou ble*/float)render_len)/((/*dou ble*/float)audio->rate_in_s);
		break;
	}

	/* Copy the audio data to output */
#ifdef THREADED_AUDIO
	u8 *rbuf;
	#ifdef DEBUG_SYSTEM
	assert(audio);
	assert(audio->ring);
	#endif
	s32 copylen;
	do
	{
	/* this is empirical, I don't realy know how to find out when
	   a certain piece of audio has finished playing or even if
	   the timestamps refer to the time when the frame starts
	   playing or then the frame ends playing, but as is works
	   quite right */
		copylen = audio->ring->NextReadBuffer(&rbuf);
		if ( copylen > render_len )
		{
			PSPL_MixAudio(stream, rbuf, render_len, (SDL_MIX_MAXVOLUME)/*volume*/);
			mixed += render_len;
			audio->ring->ReadSome(render_len);
			render_len = 0;
			int i;
			for (i=0; i < N_TIMESTAMPS -1; i++)
			{
				audio->timestamp[i] = audio->timestamp[i+1];
			}
			audio->timestamp[N_TIMESTAMPS-1] = audio->ring->ReadTimeStamp();
		}
		else
		{
			PSPL_MixAudio(stream, rbuf, copylen, (SDL_MIX_MAXVOLUME)/*volume*/);
			mixed += copylen;
			++audio->currentframe;
			audio->ring->ReadDone();
		//	error(0/*ERR_FATAL*/, "-");
			render_len -= copylen;
			stream += copylen;
		}
		if (audio->timestamp[0] != -1)
		{
			/*dou ble*/float timeshift = audio->Time() - audio->timestamp[0];
			/*dou ble*/float correction = 0;
			#ifdef DEBUG_SYSTEM
			assert(audio->timestamp >= 0);
			#endif
			if (fabs(timeshift) > 1.0f)
			{
				correction = -timeshift;
				#ifdef DEBUG_TIMESTAMP_SYNC
				error(0/*ERR_FATAL*/, "audio jump %f\n", timeshift);
				#endif
			}
			else
			{	correction = -timeshift/100;}
			#ifdef USE_TIMESTAMP_SYNC
			audio->play_time += ((/*dou ble*/float)correction);
			#endif
			#ifdef DEBUG_TIMESTAMP_SYNC
			error(0/*ERR_FATAL*/, "\raudio: time:%8.3f shift:%8.4f",
				audio->Time(), timeshift);
			#endif
			audio->timestamp[0] = -1;
		}
	}
	while (
		(copylen) &&
		(render_len > 0) &&
		((audio->currentframe < audio->decodedframe) || audio->decoding) &&
		(!audio->force_exit)
	);

#else
	/* The length is interpreted as being in samples */
	render_len /= 2;

	/* Copy in any saved data */
	if ( audio->rawdatawriteoffset >= audio->rawdatareadoffset)
	{
		s32 copylen;
		copylen = (audio->rawdatawriteoffset-audio->rawdatareadoffset);
		#ifdef DEBUG_SYSTEM
		assert(copylen >= 0);
		#endif
		if ( copylen >= render_len )
		{
			PSPL_MixAudio(stream, (u8 *)&audio->spillover[audio->rawdatareadoffset], (render_len+render_len), (SDL_MIX_MAXVOLUME)/*volume*/);
			mixed += (render_len+render_len);
			audio->rawdatareadoffset += render_len;
			goto finished_mixing;
		}
		PSPL_MixAudio(stream, (u8 *)&audio->spillover[audio->rawdatareadoffset], (copylen+copylen), (SDL_MIX_MAXVOLUME)/*volume*/);
		mixed	+= (copylen+copylen);
		render_len 	-= copylen;
		stream	+= (copylen+copylen);
	}

	/* Copy in any new data */
	audio->rawdata				= (s16 *)stream;
	audio->rawdatawriteoffset	= 0;
	audio->run(render_len/audio->samplesperframe);
	mixed	+= audio->rawdatawriteoffset*2;
	render_len 	-= audio->rawdatawriteoffset;
	stream	+= audio->rawdatawriteoffset*2;

	/* Write a save buffer for remainder */
	audio->rawdata				= audio->spillover;
	audio->rawdatawriteoffset	= 0;
	if ( audio->run(1) )
	{
		#ifdef DEBUG_SYSTEM
		assert(audio->rawdatawriteoffset > render_len);
		#endif
		PSPL_MixAudio(stream, (u8 *) audio->spillover, (render_len+render_len), (SDL_MIX_MAXVOLUME)/*volume*/);
		mixed += (render_len+render_len);
		audio->rawdatareadoffset = render_len;
	}
	else
	{
		audio->rawdatareadoffset = 0;
	}
finished_mixing:
#endif
	return (mixed);
}


void Play_MPEGaudioSDL(void *udata, u8 *stream, int len)
{
	MPEGaudio *audio = (MPEGaudio *)udata;
	Play_MPEGaudio(audio, stream, len);
}

// EOF
