
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
---------------------------------------------------------*/

/*
	SDL_mixer:	An audio mixer library based on the SDL library
	Copyright (C) 1997-2009 Sam Lantinga

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


	# ------
	# libmad (& smpeg)が遅い訳:
	# 実際動かしてみて、あれ？何でこんなに遅いんだよっ！ってのには理由があります。
	# libmad自体は遅くない(たぶん)。さらに smpeg も遅くないのかも知れない(やってないから判らない)。
	# ------
	# SDLでの libmad (& smpeg)の対応は、曲を読み込む時に mp3ファイルを fopen しっぱなしにして、
	# mp3のデコード時は fseek でmp3ファイルを実際に読みに行って、曲が終わったら fclose するというもの。
	# ------
	# PC(ms-dos/windows/linux/...) では、mp3ファイル自体がファイルキャッシュ、つまり実際はメインメモリ上にあるので
	# この対応でも何の問題もない。
	# しかし psp では、ファイル全部がメインメモリに無く、そういう仕組みになってないので煩雑に「本当にメモステに読みにいって」遅いのです。
	# (現状 r31 あまりに遅いので /src/SDL_mixer/music_mad.h MAD_INPUT_BUFFER_SIZE 一回の読み込み量を減らしてます。
	# 単純に減らせば良い物ではなく、値を減らせば読み込み回数は当然増えるというデメリットがあります。)
	# ------
	# そゆー訳で /src/SDL_mixer/music_mad.c を改造すれば良い訳です。
	# ------
	# で、とりあえず改造したんだけど、遅い。。。ただし、ぶちぶち音が途切れるのは直りました。
	# ------

*/
#ifdef _SDL_MIXER_H
error! "このプログラムは SDL_mixer.h がインクルードされていると、コンパイルできません。"
#endif
#define _SDL_MIXER_H

#ifdef MP3_MAD_MUSIC



#include <pspiofilemgr.h>

#include <malloc.h>
#include <string.h>


#include "./../../include/PSPL_audio.h"
#include "./000SDL_mixer.h"//#define MIX_MAX_VOLUME			128 	/* Volume of a chunk */

#include "./music_mad.h"





#if 1
static unsigned long file_size;
static unsigned long file_seek;
//static char *my_buf;
//static void *malloc_buf;
static char *malloc_buf;

/*static*/static/*global*/ void *my_fopen(/*void*/const char *my_fopen_file_name/*const char *my_fopen_file_name*/ , const char *dummy)
{
	SceUID fd;
	fd = sceIoOpen((char *)my_fopen_file_name, PSP_O_RDONLY, 0777);
	if (0 == fd)
	{
		goto error111;
	}
	file_size = sceIoLseek32(fd, 0, PSP_SEEK_END);
	file_seek = 0;

	malloc_buf = malloc(file_size);
	if (NULL == malloc_buf)
	{
		sceIoClose(fd);
		goto error111;
	}
	sceIoLseek32(fd, 0, PSP_SEEK_SET);
	sceIoRead( fd, malloc_buf, file_size);
	sceIoClose(fd);
//	my_buf = malloc_buf;
//
	return (malloc_buf);
error111:
	return (NULL);
}
/*static*/static/*global*/ void my_fclose(/*void*/void *wfp/*void *wfp*/)
{
	free(malloc_buf);
}
static int my_fread(void *dummy, unsigned char *read_start, int dummy333, int read_size)
{
	int readed_size;
	readed_size = 0;
//	int aaa;
//	aaa = read_start;
//	if (file_size < aaa )	return (0)/*NULL*/;
//	file_seek = read_start;
//	aaa += read_size;
//	if (file_size < aaa )	{ aaa = file_size; }
//	readed_size = aaa;
//
//	mp3_mad->rw_file_pos	= &malloc_buf[file_seek];
//
//	if (file_size < file_seek)	return (0)/*NULL*/;
	int i;
	i = 0;
	{
	loop:
		;
		if (file_size <= file_seek)
		{
			goto quit;
		}
		read_start[i] = malloc_buf[file_seek];
		i++;
		file_seek++;
		readed_size++;
		read_size--;
		if (0 != read_size) 	{	goto loop;	}
	}

quit:
	return (readed_size);
}
static void my_fseek(void *dummy, int dummy222, int dummy333)
{
	file_seek = 0;
}
	#define my_SDL_fopen	my_fopen
	#define my_SDL_fclose	my_fclose
	#define my_SDL_fread 	my_fread
	#define my_SDL_fseek	my_fseek
#else
//	fopen?		()
//	fclose? 	()
//	fread?		読む(SDL_RWread)					fread(ctx, ptr, size, n)
//	fseek?		指定した場所に移動(SDL_RWseek)		fseek(ctx, offset, whence)

	#define my_SDL_fopen	SDL_RWFromFile
	#define my_SDL_fclose	SDL_FreeRW
	#define my_SDL_fread 	SDL_RWread
	#define my_SDL_fseek	SDL_RWseek
#endif

static mad_data *s_mad_openFileRW(SDL_RWops *sdl_fp_rw, PSPL_AUDIO_SPEC *mixer)
{
	mad_data *mp3_mad;
	mp3_mad = (mad_data *)malloc(sizeof(mad_data));
	if (mp3_mad)
	{
		mp3_mad->rw_file_pos_dummy	= sdl_fp_rw;
		mp3_mad->freerw 		= SDL_FALSE;
		mad_stream_init(&mp3_mad->stream);
		mad_frame_init(&mp3_mad->frame);
		mad_synth_init(&mp3_mad->synth);
		mp3_mad->frames_read	= 0;
		mad_timer_reset(&mp3_mad->next_frame_start);
		mp3_mad->volume 		= MIX_MAX_VOLUME;
		mp3_mad->status 		= 0;
		mp3_mad->output_begin	= 0;
		mp3_mad->output_end 	= 0;
		mp3_mad->mixer			= *mixer;
	}
	return (mp3_mad);
}


mad_data *mad_openFile(const char *filename, PSPL_AUDIO_SPEC *mixer)
{
	SDL_RWops *sdl_fp_rw;
	sdl_fp_rw = my_SDL_fopen(filename, "rb");
	if (sdl_fp_rw == NULL)
	{
		return (NULL);
	}
	//
	mad_data *mp3_mad;
	mp3_mad = s_mad_openFileRW(sdl_fp_rw, mixer);
	if (mp3_mad == NULL)
	{
		my_SDL_fclose(sdl_fp_rw);
		return (NULL);
	}
	mp3_mad->freerw = SDL_TRUE;
	return (mp3_mad);
}


void mad_closeFile(mad_data *mp3_mad)
{
	mad_stream_finish(&mp3_mad->stream);
	mad_frame_finish(&mp3_mad->frame);
	mad_synth_finish(&mp3_mad->synth);

	if (mp3_mad->freerw)
	{
		my_SDL_fclose(mp3_mad->rw_file_pos_dummy);
	}
	free(mp3_mad);
}

/* Starts the playback. */
void mad_start(mad_data *mp3_mad)
{
	mp3_mad->status |= MS_playing;
}

/* Stops the playback. */
void mad_stop(mad_data *mp3_mad)
{
	mp3_mad->status &= ~MS_playing;
}

/* Returns true if the playing is engaged, false otherwise. */
int mad_isPlaying(mad_data *mp3_mad)
{
	return ((mp3_mad->status & MS_playing) != 0);
}

/* Reads the next frame from the file.	Returns true on success or false on failure. */
static int read_next_frame(mad_data *mp3_mad)
{
	if (mp3_mad->stream.buffer == NULL ||
			mp3_mad->stream.error == MAD_ERROR_BUFLEN)
	{
		size_t read_size;
		size_t remaining;
		unsigned char *read_start;

		/* There might be some bytes in the buffer left over from last time. */
		/*	If so, move them down and read more bytes following them. */
		if (mp3_mad->stream.next_frame != NULL)
		{
			remaining = mp3_mad->stream.bufend - mp3_mad->stream.next_frame;
			memmove(mp3_mad->input_buffer, mp3_mad->stream.next_frame, remaining);
			read_start = mp3_mad->input_buffer + remaining;
			read_size = MAD_INPUT_BUFFER_SIZE - remaining;
		}
		else
		{
			read_size	= MAD_INPUT_BUFFER_SIZE;
			read_start	= mp3_mad->input_buffer;
			remaining	= 0;
		}

		/* Now read additional bytes from the input file. */
		read_size = /*SDL_RWread*/my_SDL_fread(mp3_mad->rw_file_pos_dummy, read_start, 1, read_size);

		if (read_size <= 0)
		{
			if ((mp3_mad->status & (MS_input_eof | MS_input_error)) == 0)
			{
				if (read_size == 0)
				{
					mp3_mad->status |= MS_input_eof;
				}
				else
				{
					mp3_mad->status |= MS_input_error;
				}

				/* At the end of the file, we must stuff MAD_BUFFER_GUARD number of 0 bytes. */
				memset(read_start + read_size, 0, MAD_BUFFER_GUARD);
				read_size += MAD_BUFFER_GUARD;
			}
		}

		/* Now feed those bytes into the libmad stream. */
		mad_stream_buffer(&mp3_mad->stream, mp3_mad->input_buffer, read_size + remaining);
		mp3_mad->stream.error = MAD_ERROR_NONE;
	}

	/* Now ask libmad to extract a frame from the data we just put in its buffer. */
	if (mad_frame_decode(&mp3_mad->frame, &mp3_mad->stream))
	{
		if (MAD_RECOVERABLE(mp3_mad->stream.error))
		{
			return (0);
		}
		else
		if (mp3_mad->stream.error == MAD_ERROR_BUFLEN)
		{
			return (0);
		}
		else
		{
			mp3_mad->status |= MS_decode_error;
			return (0);
		}
	}

	mp3_mad->frames_read++;
	mad_timer_add(&mp3_mad->next_frame_start, mp3_mad->frame.header.duration);
	return (1);
}

#if 1
/* Scale a MAD sample to 16 bits for output. */
static signed int scale(mad_fixed_t sample)
{
	/* round */
	sample += (1L << (MAD_F_FRACBITS - 16));

	/* clip */
	if (sample >= MAD_F_ONE)
	{	sample = MAD_F_ONE - 1;}
	else if (sample < -MAD_F_ONE)
	{	sample = -MAD_F_ONE;}

	/* quantize */
	return (sample >> (MAD_F_FRACBITS + 1 - 16));
}
#endif

/* Once the frame has been read, copies its samples into the output buffer. */
static void decode_frame(mad_data *mp3_mad)
{
//	int ret;
	mad_synth_frame(&mp3_mad->synth, &mp3_mad->frame);
	//

	struct mad_pcm *pcm;
	unsigned char *out;

	pcm = &mp3_mad->synth.pcm;
	out = mp3_mad->output_buffer + mp3_mad->output_end;
	signed short *out16;
	out16 = (void*)out;
	if ((mp3_mad->status & MS_cvt_decoded) == 0)
	{
		mp3_mad->status |= MS_cvt_decoded;

		/* The first frame determines some key properties of the stream.
			 In particular, it tells us enough to set up the convert
			 structure now. */
		PSPL_BuildAudioCVT(
			&mp3_mad->cvt,
			//--変換元
			AUDIO_S16,
			pcm->channels,
			mp3_mad->frame.header.samplerate,
			//--変換先
			mp3_mad->mixer.format,
			#ifdef _OLD_SDL_LINK_MODE_/*(旧互換機能)*/
			(2),//	mp3_mad->mixer.channels, //(2)pspステレオ再生固定
			#endif
			mp3_mad->mixer.freq);
	}

	/* pcm->samplerate contains the sampling frequency */

	unsigned int nchannels;
	nchannels	= pcm->channels;
	unsigned int nsamples;
	nsamples	= pcm->length;
	mad_fixed_t const *left_ch;
	mad_fixed_t const *right_ch;
	left_ch 	= pcm->samples[0];
	right_ch	= pcm->samples[1];

	/* output sample(s) in 16-bit signed little-endian PCM */
	if (nchannels == 2) 	/* stereo render */
	{
		while (nsamples--)
		{
			signed short sample;		//	signed int sample;
			sample = scale(*left_ch++);
		/*	*out = ((sample >> 0) & 0xff);*/	out++;
		/*	*out = ((sample >> 8) & 0xff);*/	out++;
			*out16 = (sample & 0xffff); 		out16++;
			sample = scale(*right_ch++);
		/*	*out = ((sample >> 0) & 0xff);	*/	out++;
		/*	*out = ((sample >> 8) & 0xff);	*/	out++;
			*out16 = (sample & 0xffff); 		out16++;
		}
	}
	else	/* monoral render */
	{
		while (nsamples--)
		{
			signed short sample;		//	signed int sample;
			sample = scale(*left_ch++);
		/*	*out = ((sample >> 0) & 0xff);*/	out++;
		/*	*out = ((sample >> 8) & 0xff);*/	out++;
			*out16 = (sample & 0xffff); 		out16++;
		}
	}
	mp3_mad->output_end = out - mp3_mad->output_buffer;
	/*assert(mp3_mad->output_end <= MAD_OUT PUT_BUFFER_SIZE);*/
}

int mad_getSamples(mad_data *mp3_mad, u8 *stream, int len)
{
	int bytes_remaining;
	int num_bytes;
	u8 *out;

	if ((mp3_mad->status & MS_playing) == 0)
	{
		/* We're not supposed to be playing, so send silence instead. */
		memset(stream, 0, len);
		return (0);/*0???*/
	}

	out = stream;
	bytes_remaining = len;
	while (bytes_remaining > 0)
	{
		if (mp3_mad->output_end == mp3_mad->output_begin)
		{
			/* We need to get a new frame. */
			mp3_mad->output_begin = 0;
			mp3_mad->output_end = 0;
			if (!read_next_frame(mp3_mad))
			{
				if ((mp3_mad->status & MS_error_flags) != 0)
				{
					/* Couldn't read a frame; either an error condition or end-of-file.  Stop. */
					memset(out, 0, bytes_remaining);
					mp3_mad->status &= ~MS_playing;
					return bytes_remaining;
				}
			}
			else
			{
				decode_frame(mp3_mad);

				/* Now convert the frame data to the appropriate format for output. */
				mp3_mad->cvt.buf = mp3_mad->output_buffer;
				mp3_mad->cvt.len = mp3_mad->output_end;

				mp3_mad->output_end = (int)(mp3_mad->output_end * mp3_mad->cvt.len_ratio);
				/*assert(mp3_mad->output_end <= MAD_OUT PUT_BUFFER_SIZE);*/
				PSPL_ConvertAudio(&mp3_mad->cvt);
			}
		}

		num_bytes = mp3_mad->output_end - mp3_mad->output_begin;
		if (bytes_remaining < num_bytes)
		{
			num_bytes = bytes_remaining;
		}

		if (mp3_mad->volume == MIX_MAX_VOLUME)
		{
			memcpy(out, mp3_mad->output_buffer + mp3_mad->output_begin, num_bytes);
		}
		else
		{
			PSPL_MixAudio(out, mp3_mad->output_buffer + mp3_mad->output_begin, num_bytes, mp3_mad->volume);
		}
		out += num_bytes;
		mp3_mad->output_begin += num_bytes;
		bytes_remaining -= num_bytes;
	}
	return (0);
}

void mad_seek_int(mad_data *mp3_mad, /*dou ble*/int position)
{
	mad_timer_t target;
	int int_part;

	int_part = (int)position;
//	mad_timer_set(&target, int_part, (int)((position - int_part) * 1000000), 1000000);
//	mad_timer_set(&target, int_part, (int)((position - int_part) * 1000), 1000);
	mad_timer_set(&target, int_part, (int)((position - int_part) ), 1);

	if (mad_timer_compare(mp3_mad->next_frame_start, target) > 0)
	{
		/* In order to seek backwards in a VBR file, we have to rewind and
			 start again from the beginning.  This isn't necessary if the
			 file happens to be CBR, of course; in that case we could seek
			 directly to the frame we want.  But I leave that little
			 optimization for the future developer who discovers she really
			 needs it. */
		mp3_mad->frames_read	= 0;
		mad_timer_reset(&mp3_mad->next_frame_start);
		mp3_mad->status &= ~MS_error_flags;
		mp3_mad->output_begin	= 0;
		mp3_mad->output_end 	= 0;

		/*SDL_RWseek*/my_SDL_fseek(mp3_mad->rw_file_pos_dummy, 0, /*RW_SEEK_SET*/SEEK_SET);
	}

	/* Now we have to skip frames until we come to the right one.
		 Again, only truly necessary if the file is VBR. */
	while (mad_timer_compare(mp3_mad->next_frame_start, target) < 0)
	{
		if (!read_next_frame(mp3_mad))
		{
			if ((mp3_mad->status & MS_error_flags) != 0)
			{
				/* Couldn't read a frame; either an error condition or end-of-file.  Stop. */
				mp3_mad->status &= ~MS_playing;
				return;
			}
		}
	}

	/* Here we are, at the beginning of the frame that contains the
		 target time.  Ehh, I say that's close enough.	If we wanted to,
		 we could get more precise by decoding the frame now and counting
		 the appropriate number of samples out of it. */
}

void mad_setVolume(mad_data *mp3_mad, unsigned int volume)
{
	mp3_mad->volume = volume;
}


#endif /* MP3_MAD_MUSIC */
