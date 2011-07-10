
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_audiocvt.c,v 1.7 2004/12/13 08:49:17 slouken Exp $";
#endif

/* Functions for audio drivers to perform runtime conversion of audio format */

#include <stdio.h>

#include "./../include/PSPL_error.h"
#include "./../include/PSPL_audio.h"

#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb(aaa)
#endif

/* Toggle endianness */
static void s_SDL_ConvertEndian(PSPL_AUDIO_CONVERT *cvt, u16 format)
{
	int i;
	u8 *data, tmp;
	#ifdef DEBUG_CONVERT
	fprintf(stderr, "Converting audio endianness\n");
	#endif
	data = cvt->buf;
	for (i=cvt->len_cvt/2; i; i--)
	{
		tmp = data[0];
		data[0] = data[1];
		data[1] = tmp;
		data += 2;
	}
	format = (format ^ 0x1000);
	if ( cvt->filters[++cvt->filter_index] )
	{
		cvt->filters[cvt->filter_index](cvt, format);
	}
}

/* Toggle signed/unsigned */
static void s_SDL_ConvertSign(PSPL_AUDIO_CONVERT *cvt, u16 format)
{
	int i;
	u8 *data;
	#ifdef DEBUG_CONVERT
	fprintf(stderr, "Converting audio signedness\n");
	#endif
	data = cvt->buf;
	if ( (format & 0xff) == 16 )
	{
		if ( (format & 0x1000) != 0x1000 )	/* Little endian */
		{
			++data;
		}
		for (i=cvt->len_cvt/2; i; i--)
		{
			*data ^= 0x80;
			data += 2;
		}
	}
	else
	{
		for (i=cvt->len_cvt; i; i--)
		{
			*data++ ^= 0x80;
		}
	}
	format = (format ^ 0x8000);
	if ( cvt->filters[++cvt->filter_index] )
	{
		cvt->filters[cvt->filter_index](cvt, format);
	}
}

/* Convert 16-bit to 8-bit */
static void s_SDL_Convert8(PSPL_AUDIO_CONVERT *cvt, u16 format)
{
	int i;
	u8 *src, *dst;
	#ifdef DEBUG_CONVERT
	fprintf(stderr, "Converting to 8-bit\n");
	#endif
	src = cvt->buf;
	dst = cvt->buf;
	if ( (format & 0x1000) != 0x1000 )	/* Little endian */
	{
		src++;
	}
	for (i=cvt->len_cvt/2; i; i--)
	{
		*dst = *src;
		src += 2;
		dst += 1;
	}
	format = ((format & ~0x9010) | AUDIO_U8);
	cvt->len_cvt /= 2;
	if ( cvt->filters[++cvt->filter_index] )
	{
		cvt->filters[cvt->filter_index](cvt, format);
	}
}

/* Convert 8-bit to 16-bit - LSB */
static void s_SDL_Convert16LSB(PSPL_AUDIO_CONVERT *cvt, u16 format)
{
	int i;
	u8 *src, *dst;
	#ifdef DEBUG_CONVERT
	fprintf(stderr, "Converting to 16-bit LSB\n");
	#endif
	src = cvt->buf+cvt->len_cvt;
	dst = cvt->buf+cvt->len_cvt*2;
	for (i=cvt->len_cvt; i; i--)
	{
		src -= 1;
		dst -= 2;
		dst[1] = *src;
		dst[0] = 0;
	}
	format = ((format & ~0x0008) | AUDIO_U16LSB);
	cvt->len_cvt *= 2;
	if ( cvt->filters[++cvt->filter_index] )
	{
		cvt->filters[cvt->filter_index](cvt, format);
	}
}

/* Convert 8-bit to 16-bit - MSB */
static void s_SDL_Convert16MSB(PSPL_AUDIO_CONVERT *cvt, u16 format)
{
	int i;
	u8 *src, *dst;
	#ifdef DEBUG_CONVERT
	fprintf(stderr, "Converting to 16-bit MSB\n");
	#endif
	src = cvt->buf+cvt->len_cvt;
	dst = cvt->buf+cvt->len_cvt*2;
	for (i=cvt->len_cvt; i; i--)
	{
		src -= 1;
		dst -= 2;
		dst[0] = *src;
		dst[1] = 0;
	}
	format = ((format & ~0x0008) | AUDIO_U16MSB);
	cvt->len_cvt *= 2;
	if ( cvt->filters[++cvt->filter_index] )
	{
		cvt->filters[cvt->filter_index](cvt, format);
	}
}

/* Duplicate a mono channel to both stereo channels */
static void s_SDL_ConvertStereo(PSPL_AUDIO_CONVERT *cvt, u16 format)
{
	int i;
	#ifdef DEBUG_CONVERT
	fprintf(stderr, "Converting to stereo\n");
	#endif
	if ( (format & 0xff) == 16 )
	{
		u16 *src, *dst;
		src = (u16 *)(cvt->buf+cvt->len_cvt);
		dst = (u16 *)(cvt->buf+cvt->len_cvt*2);
		for (i=cvt->len_cvt/2; i; i--)
		{
			dst -= 2;
			src -= 1;
			dst[0] = src[0];
			dst[1] = src[0];
		}
	}
	else
	{
		u8 *src, *dst;
		src = cvt->buf+cvt->len_cvt;
		dst = cvt->buf+cvt->len_cvt*2;
		for (i=cvt->len_cvt; i; i--)
		{
			dst -= 2;
			src -= 1;
			dst[0] = src[0];
			dst[1] = src[0];
		}
	}
	cvt->len_cvt *= 2;
	if ( cvt->filters[++cvt->filter_index] )
	{
		cvt->filters[cvt->filter_index](cvt, format);
	}
}

/* Effectively mix right and left channels into a single channel */
static void s_SDL_ConvertMono(PSPL_AUDIO_CONVERT *cvt, u16 format)
{
	int i;
	s32 sample;
	#ifdef DEBUG_CONVERT
	fprintf(stderr, "Converting to mono\n");
	#endif
	switch (format&0x8018)
	{
	case AUDIO_U8:
		{
			u8 *src, *dst;
			src = cvt->buf;
			dst = cvt->buf;
			for (i=cvt->len_cvt/2; i; i--)
			{
				sample = src[0] + src[1];
				if ( sample > 255 )
				{
					*dst = 255;
				}
				else
				{
					*dst = sample;
				}
				src += 2;
				dst += 1;
			}
		}
		break;
	case AUDIO_S8:
		{
			s8 *src, *dst;
			src = (s8 *)cvt->buf;
			dst = (s8 *)cvt->buf;
			for (i=cvt->len_cvt/2; i; i--)
			{
				sample = src[0] + src[1];
				if ( sample > 127 )
				{
					*dst = 127;
				}
				else
				if ( sample < -128 )
				{
					*dst = -128;
				}
				else
				{
					*dst = sample;
				}
				src += 2;
				dst += 1;
			}
		}
		break;
	case AUDIO_U16:
		{
			u8 *src, *dst;
			src = cvt->buf;
			dst = cvt->buf;
			if ( (format & 0x1000) == 0x1000 )
			{
				for (i=cvt->len_cvt/4; i; i--)
				{
					sample = (u16)((src[0]<<8)|src[1])+
						 (u16)((src[2]<<8)|src[3]);
					if ( sample > 65535 )
					{
						dst[0] = 0xff;
						dst[1] = 0xff;
					}
					else
					{
						dst[1] = (sample&0xff);
						sample >>= 8;
						dst[0] = (sample&0xff);
					}
					src += 4;
					dst += 2;
				}
			}
			else
			{
				for (i=cvt->len_cvt/4; i; i--)
				{
					sample = (u16)((src[1]<<8)|src[0])+
						 (u16)((src[3]<<8)|src[2]);
					if ( sample > 65535 )
					{
						dst[0] = 0xff;
						dst[1] = 0xff;
					}
					else
					{
						dst[0] = (sample&0xff);
						sample >>= 8;
						dst[1] = (sample&0xff);
					}
					src += 4;
					dst += 2;
				}
			}
		}
		break;
	case AUDIO_S16:
	{
			u8 *src, *dst;
			src = cvt->buf;
			dst = cvt->buf;
			if ( (format & 0x1000) == 0x1000 )
			{
				for (i=cvt->len_cvt/4; i; i--)
				{
					sample = (s16)((src[0]<<8)|src[1])+
						 (s16)((src[2]<<8)|src[3]);
					if ( sample > 32767 )
					{
						dst[0] = 0x7F;
						dst[1] = 0xff;
					}
					else
					if ( sample < -32768 )
					{
						dst[0] = 0x80;
						dst[1] = 0x00;
					}
					else
					{
						dst[1] = (sample&0xff);
						sample >>= 8;
						dst[0] = (sample&0xff);
					}
					src += 4;
					dst += 2;
				}
			}
			else
			{
				for (i=cvt->len_cvt/4; i; i--)
				{
					sample = (s16)((src[1]<<8)|src[0])+
						 (s16)((src[3]<<8)|src[2]);
					if ( sample > 32767 )
					{
						dst[1] = 0x7F;
						dst[0] = 0xff;
					}
					else
					if ( sample < -32768 )
					{
						dst[1] = 0x80;
						dst[0] = 0x00;
					}
					else
					{
						dst[0] = (sample&0xff);
						sample >>= 8;
						dst[1] = (sample&0xff);
					}
					src += 4;
					dst += 2;
				}
			}
		}
		break;
	}
	cvt->len_cvt /= 2;
	if ( cvt->filters[++cvt->filter_index] )
	{
		cvt->filters[cvt->filter_index](cvt, format);
	}
}

/* Convert rate down by multiple of 2 */
static void s_SDL_RateDIV2(PSPL_AUDIO_CONVERT *cvt, u16 format)
{
	int i;
	u8 *src, *dst;
	#ifdef DEBUG_CONVERT
	fprintf(stderr, "Converting audio rate / 2\n");
	#endif
	src = cvt->buf;
	dst = cvt->buf;
	switch (format & 0xff)
	{
	case 8:
		for (i=cvt->len_cvt/2; i; i--)
		{
			dst[0] = src[0];
			src += 2;
			dst += 1;
		}
		break;
	case 16:
		for (i=cvt->len_cvt/4; i; i--)
		{
			dst[0] = src[0];
			dst[1] = src[1];
			src += 4;
			dst += 2;
		}
		break;
	}
	cvt->len_cvt /= 2;
	if ( cvt->filters[++cvt->filter_index] )
	{
		cvt->filters[cvt->filter_index](cvt, format);
	}
}


/* Convert rate down by multiple of 2, for stereo */
static void s_SDL_RateDIV2_c2(PSPL_AUDIO_CONVERT *cvt, u16 format)
{
	int i;
	u8 *src, *dst;
	#ifdef DEBUG_CONVERT
	fprintf(stderr, "Converting audio rate / 2\n");
	#endif
	src = cvt->buf;
	dst = cvt->buf;
	switch (format & 0xff)
	{
	case 8:
		for (i=cvt->len_cvt/4; i; i--)
		{
			dst[0] = src[0];
			dst[1] = src[1];
			src += 4;
			dst += 2;
		}
		break;
	case 16:
		for (i=cvt->len_cvt/8; i; i--)
		{
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
			dst[3] = src[3];
			src += 8;
			dst += 4;
		}
		break;
	}
	cvt->len_cvt /= 2;
	if ( cvt->filters[++cvt->filter_index] )
	{
		cvt->filters[cvt->filter_index](cvt, format);
	}
}

/* Convert rate up by multiple of 2 */
static void s_SDL_RateMUL2(PSPL_AUDIO_CONVERT *cvt, u16 format)
{
	int i;
	u8 *src, *dst;
	#ifdef DEBUG_CONVERT
	fprintf(stderr, "Converting audio rate * 2\n");
	#endif
	src = cvt->buf+cvt->len_cvt;
	dst = cvt->buf+cvt->len_cvt*2;
	switch (format & 0xff)
	{
	case 8:
		for (i=cvt->len_cvt; i; i--)
		{
			src -= 1;
			dst -= 2;
			dst[0] = src[0];
			dst[1] = src[0];
		}
		break;
	case 16:
		for (i=cvt->len_cvt/2; i; i--)
		{
			src -= 2;
			dst -= 4;
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[0];
			dst[3] = src[1];
		}
		break;
	}
	cvt->len_cvt *= 2;
	if ( cvt->filters[++cvt->filter_index] )
	{
		cvt->filters[cvt->filter_index](cvt, format);
	}
}


/* Convert rate up by multiple of 2, for stereo */
static void s_SDL_RateMUL2_c2(PSPL_AUDIO_CONVERT *cvt, u16 format)
{
	int i;
	u8 *src, *dst;
	#ifdef DEBUG_CONVERT
	fprintf(stderr, "Converting audio rate * 2\n");
	#endif
	src = cvt->buf+cvt->len_cvt;
	dst = cvt->buf+cvt->len_cvt*2;
	switch (format & 0xff)
	{
	case 8:
		for (i=cvt->len_cvt/2; i; i--)
		{
			src -= 2;
			dst -= 4;
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[0];
			dst[3] = src[1];
		}
		break;
	case 16:
		for (i=cvt->len_cvt/4; i; i--)
		{
			src -= 4;
			dst -= 8;
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
			dst[3] = src[3];
			dst[4] = src[0];
			dst[5] = src[1];
			dst[6] = src[2];
			dst[7] = src[3];
		}
		break;
	}
	cvt->len_cvt *= 2;
	if ( cvt->filters[++cvt->filter_index] )
	{
		cvt->filters[cvt->filter_index](cvt, format);
	}
}

/* Creates a set of audio filters to convert from one format to another.
   Returns -1 if the format conversion is not supported, or 1 if the
   audio filter is set up.
*/
int PSPL_BuildAudioCVT(
	PSPL_AUDIO_CONVERT *cvt,
	//--変換元
	u16 src_format,
	u8 src_channels,
	int src_rate,
	//--変換先
	u16 dst_format,
	//	(2),//	u8 dst_channels, //(2)pspステレオ再生固定
	int dst_rate)
{
/*	printf("Build format %04x->%04x, channels %u->%u, rate %d->%d\n",
		src_format, dst_format, src_channels, dst_channels, src_rate, dst_rate);*/
	/* Start off with no conversion necessary */
	cvt->needed 		= 0;
	cvt->filter_index	= 0;
	cvt->filters[0] 	= NULL;
	cvt->len_mult		= 1;
	cvt->len_ratio		= 1.0f;

	/* First filter:  Endian conversion from src to dst */
	if ( (src_format & 0x1000) != (dst_format & 0x1000)
		 && ((src_format & 0xff) != 8) )
	{
		cvt->filters[cvt->filter_index++] = s_SDL_ConvertEndian;
	}
	/* Second filter: Sign conversion -- signed/unsigned */
	if ( (src_format & 0x8000) != (dst_format & 0x8000) )
	{
		cvt->filters[cvt->filter_index++] = s_SDL_ConvertSign;
	}
	/* Next filter:  Convert 16 bit <--> 8 bit PCM */
	if ( (src_format & 0xff) != (dst_format & 0xff) )
	{
		switch (dst_format&0x10FF)
		{
		case AUDIO_U8:
			cvt->filters[cvt->filter_index++] = s_SDL_Convert8;
		//	cvt->len_ratio /= 2.0f;
			cvt->len_ratio *= 0.5f;
			break;
		case AUDIO_U16LSB:
			cvt->filters[cvt->filter_index++] = s_SDL_Convert16LSB;
			cvt->len_mult *= 2;
			cvt->len_ratio *= 2.0f;
			break;
		case AUDIO_U16MSB:
			cvt->filters[cvt->filter_index++] = s_SDL_Convert16MSB;
			cvt->len_mult *= 2;
			cvt->len_ratio *= 2.0f;
			break;
		}
	}

	/* Last filter:  Mono/Stereo conversion */
	if ( src_channels != (2)/*dst_channels*/ ) //(2)pspステレオ再生固定
	{
		if ( (src_channels == 1)
		//	&& (/*dst_channels*/(2) > 1)  //(2)pspステレオ再生固定
		)
		{
			cvt->filters[cvt->filter_index++] = s_SDL_ConvertStereo;
			cvt->len_mult	*= 2;
			src_channels	= 2;
			cvt->len_ratio	*= 2.0f;
		}
//		if ( (src_channels == 2) &&
//				(dst_channels == 6) )
//		{
//			cvt->filters[cvt->filter_index++] = SDL_ConvertSurround;
//			src_channels = 6;
//			cvt->len_mult *= 3;
//			cvt->len_ratio *= 3.0f;
//		}
//		if ( (src_channels == 2) &&
//				(dst_channels == 4) )
//		{
//			cvt->filters[cvt->filter_index++] = SDL_ConvertSurround_4;
//			src_channels = 4;
//			cvt->len_mult *= 2;
//			cvt->len_ratio *= 2.0f;
//		}
		while ( (src_channels*2) <= (2)/*dst_channels*/ )  //(2)pspステレオ再生固定
		{
			cvt->filters[cvt->filter_index++] = s_SDL_ConvertStereo;
			cvt->len_mult *= 2;
			src_channels *= 2;
			cvt->len_ratio *= 2.0f;
		}
//		if ( (src_channels == 6) &&
//				(dst_channels <= 2) )
//		{
//			cvt->filters[cvt->filter_index++] = SDL_ConvertStrip;
//			src_channels = 2;
//			cvt->len_ratio /= 3.0f;
//		}
//		if ( (src_channels == 6) &&
//				(dst_channels == 4) )
//		{
//			cvt->filters[cvt->filter_index++] = SDL_ConvertStrip_2;
//			src_channels = 4;
//			cvt->len_ratio /= 2.0f;
//		}
		/* This assumes that 4 channel audio is in the format:
			 Left {front/back} + Right {front/back}
		   so converting to L/R stereo works properly.
		 */
	//	while ( ((src_channels%2) == 0) &&
	//			((src_channels/2) >= (2)/*dst_channels*/ ) )  //(2)pspステレオ再生固定
		while ( ((src_channels&1) == 0) &&
				((src_channels>>1) >= (2)/*dst_channels*/ ) )  //(2)pspステレオ再生固定
		{
			cvt->filters[cvt->filter_index++]	= s_SDL_ConvertMono;
			src_channels	>>= 1;
		//	cvt->len_ratio	/= 2.0f;
			cvt->len_ratio	*= 0.5f;
		}
		if ( src_channels != (2)/*dst_channels*/ )	//(2)pspステレオ再生固定
		{
			/* Uh oh.. */;
		}
	}

	/* (周波数リアルタイム変換)Do rate conversion */
	#if (1==USE_AUTO_RATE_CONVERSION)
	/*(原版より、周波数自動リアルタイム変換機能は正常動作していない為、無効の様です。)*/
	cvt->rate_incr = 0.0f;
	#endif /*(1==USE_AUTO_RATE_CONVERSION)*/
	if ( (src_rate/100) != (dst_rate/100) )
	{
		u32 hi_rate, lo_rate;
		int len_mult;
		float ex_len_ratio;
		void (*rate_cvt)(PSPL_AUDIO_CONVERT *cvt, u16 format);

		if ( src_rate > dst_rate )
		{
			hi_rate = src_rate;
			lo_rate = dst_rate;
			switch (src_channels)
			{
			case 1: rate_cvt = s_SDL_RateDIV2;	  break;
			case 2: rate_cvt = s_SDL_RateDIV2_c2; break;
//			case 4: rate_cvt = s_SDL_RateDIV2_c4; break;
//			case 6: rate_cvt = s_SDL_RateDIV2_c6; break;
			default: return (-1);
			}
			len_mult = 1;
			ex_len_ratio = 0.5f;
		}
		else
		{
			hi_rate = dst_rate;
			lo_rate = src_rate;
			switch (src_channels)
			{
			case 1: rate_cvt = s_SDL_RateMUL2;	  break;
			case 2: rate_cvt = s_SDL_RateMUL2_c2; break;
//			case 4: rate_cvt = s_SDL_RateMUL2_c4; break;
//			case 6: rate_cvt = s_SDL_RateMUL2_c6; break;
			default: return (-1);
			}
			len_mult = 2;
			ex_len_ratio = 2.0f;
		}
		/* If hi_rate = lo_rate*2^x then conversion is easy */
		while ( ((lo_rate*2)/100) <= (hi_rate/100) )
		{
			cvt->filters[cvt->filter_index++] = rate_cvt;
			cvt->len_mult *= len_mult;
			lo_rate *= 2;
			cvt->len_ratio *= ex_len_ratio;
		}
		#if (1==USE_AUTO_RATE_CONVERSION)
		/*(原版より、周波数自動リアルタイム変換機能は正常動作していない為、無効の様です。)*/
		/* We may need a slow conversion here to finish up */
		if ( (lo_rate/100) != (hi_rate/100) )
		{
			#if 1
			/* The problem with this is that if the input buffer is
			   say 1K, and the conversion rate is say 1.1, then the
			   output buffer is 1.1K, which may not be an acceptable
			   buffer size for the audio driver (not a power of 2)
			*/
			/* For now, punt and hope the rate distortion isn't great.
			*/
			#else
			if ( src_rate < dst_rate )
			{
				cvt->rate_incr = (float)lo_rate/hi_rate;
				cvt->len_mult *= 2;
				cvt->len_ratio /= cvt->rate_incr;
			}
			else
			{
				cvt->rate_incr = (float)hi_rate/lo_rate;
				cvt->len_ratio *= cvt->rate_incr;
			}
			cvt->filters[cvt->filter_index++] = SDL_RateSLOW;
			#endif
		}
		#endif /*(1==USE_AUTO_RATE_CONVERSION)*/
	}
	/* Set up the filter information */
	if ( cvt->filter_index != 0 )
	{
		cvt->needed 	= 1;
		cvt->src_format = src_format;
		cvt->dst_format = dst_format;
		cvt->len		= 0;
		cvt->buf		= NULL;
		cvt->filters[cvt->filter_index] = NULL;
	}
	return (cvt->needed);
}


int PSPL_ConvertAudio(PSPL_AUDIO_CONVERT *cvt)
{
	/* Make sure there's data to convert */
	if ( cvt->buf == NULL )
	{
		SDL_SetError_bbb("No buffer allocated for conversion");
		return (-1);
	}
	/* Return okay if no conversion is necessary */
	cvt->len_cvt = cvt->len;
	if ( cvt->filters[0] == NULL )
	{
		return (0);
	}
	/* Set up the conversion and go! */
	cvt->filter_index = 0;
	cvt->filters[0](cvt, cvt->src_format);
	return (0);
}


