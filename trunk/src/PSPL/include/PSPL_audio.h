
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_audio.h,v 1.9 2004/08/20 18:57:01 slouken Exp $";
#endif

#ifdef _SDL_H
error! "このプログラムは SDL.h がインクルードされていると、コンパイルできません。"
#endif

#ifdef _SDL_audio_h
error! "このプログラムは SDL_audio.h がインクルードされていると、コンパイルできません。"
#endif

/* Access to the raw audio mixing buffer for the SDL library */

#ifndef _PSPL_audio_h
#define _PSPL_audio_h

#include <stdio.h>

#include "PSPL_main.h"
#include "PSPL_types.h"
#include "PSPL_error.h"
#include "PSPL_rwops.h"
#include "PSPL_byteorder.h"

//#inc lude "beg in_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* The calculated values in this structure are calculated by PSPL_OpenAudio() */
typedef struct //PSPL_AUDIO_SPEC_tag
{
		int 	freq;					/* DSP frequency -- samples per second */
		u16 	format; 				/* Audio data format */
/*2*/	u8		channels_is_load_wave_only; /*(".wav"読みこみ時は1か2。それ以外は必ず2.)*/		/* Number of channels: 1 mono, 2 stereo */
		u8		silence_dummy;			/* Audio buffer silence value (calculated) */
		u16 	samples;				/* Audio buffer size in samples (power of 2) */
		u16 	padding;				/* Necessary for some compile environments */
		u32 	size_1024;				/* Audio buffer size in bytes (calculated) */
		/* This function is called when the audio device needs more data.
		   'stream' is a pointer to the audio data buffer
		   'len' is the length of that buffer in bytes.
		   Once the callback returns, the buffer will no longer be valid.
		   Stereo samples are stored in a LRLRLR ordering.
		*/
		void	(/*SD LCALL*/ *callback)(void *userdata, u8 *stream, int len);
		void	*userdata;
} PSPL_AUDIO_SPEC;

/* Audio format flags (defaults to LSB byte order) */
#define AUDIO_U8		0x0008	/* Unsigned 8-bit samples */
#define AUDIO_S8		0x8008	/* Signed 8-bit samples */
#define AUDIO_U16LSB	0x0010	/* Unsigned 16-bit samples */
#define AUDIO_S16LSB	0x8010	/* Signed 16-bit samples */
#define AUDIO_U16MSB	0x1010	/* As above, but big-endian byte order */
#define AUDIO_S16MSB	0x9010	/* As above, but big-endian byte order */
#define AUDIO_U16		AUDIO_U16LSB
#define AUDIO_S16		AUDIO_S16LSB

/* Native audio byte ordering */
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define AUDIO_U16SYS	AUDIO_U16LSB
#define AUDIO_S16SYS	AUDIO_S16LSB
#else
#define AUDIO_U16SYS	AUDIO_U16MSB
#define AUDIO_S16SYS	AUDIO_S16MSB
#endif

/*(原版より、周波数自動リアルタイム変換機能は正常動作していない為、無効の様です。)*/
#define USE_AUTO_RATE_CONVERSION (0)

/* A structure to hold a set of audio conversion filters and buffers. */
typedef struct PSPL_AUDIO_CONVERT_tag
{
	int 	needed; 			/* Set to 1 if conversion possible. */
	u16 	src_format; 		/* Source audio format. */
	u16 	dst_format; 		/* Target audio format. */
	u8		*buf;				/* Buffer to hold entire audio data. */
	int 	len;				/* Length of original audio buffer. */
	int 	len_cvt;			/* Length of converted audio buffer. */
	int 	len_mult;			/* buffer must be len*len_mult big. */
	void (/*SD LCALL*/ *filters[10])(struct PSPL_AUDIO_CONVERT_tag *cvt, u16 format);	/* 周波数変換実行ルーチン */
	int 	filter_index;		/* Current audio conversion function. */
	float	len_ratio;			/* Given len, final size is len*len_ratio. */
	#if (1==USE_AUTO_RATE_CONVERSION)
	float	rate_incr;			/* Rate conversion increment. */
	#endif /*(1==USE_AUTO_RATE_CONVERSION)*/
} PSPL_AUDIO_CONVERT;


/* Function prototypes */

/*
 * This function opens the audio device with the desired parameters, and
 * returns 0 if successful, placing the actual hardware parameters in the
 * structure pointed to by 'obtained'.	If 'obtained' is NULL, the audio
 * data passed to the callback function will be guaranteed to be in the
 * requested format, and will be automatically converted to the hardware
 * audio format if necessary.  This function returns -1 if it failed
 * to open the audio device, or couldn't set up the audio thread.
 *
 * When filling in the desired audio spec structure,
 *	'desired->freq' should be the desired audio frequency in samples-per-second.
 *	'desired->format' should be the desired audio format.
 *	'desired->samples' is the desired size of the audio buffer, in samples.
 *	   This number should be a power of two, and may be adjusted by the audio
 *	   driver to a value more suitable for the hardware.  Good values seem to
 *	   range between 512 and 8096 inclusive, depending on the application and
 *	   CPU speed.  Smaller values yield faster response time, but can lead
 *	   to underflow if the application is doing heavy processing and cannot
 *	   fill the audio buffer in time.  A stereo sample consists of both right
 *	   and left channels in LR ordering.
 *	   Note that the number of samples is directly related to time by the
 *	   following formula:  ms = (samples*1000)/freq
 *	'desired->size' is the size in bytes of the audio buffer, and is
 *	   calculated by PSPL_OpenAudio().
 *	'desired->silence' is the value used to set the buffer to silence,
 *	   and is calculated by PSPL_OpenAudio().
 *	'desired->callback' should be set to a function that will be called
 *	   when the audio device is ready for more data.  It is passed a pointer
 *	   to the audio buffer, and the length in bytes of the audio buffer.
 *	   This function usually runs in a separate thread, and so you should
 *	   protect data structures that it accesses by calling PSPL_LockAudio()
 *	   and PSPL_UnlockAudio() in your code.
 *	'desired->userdata' is passed as the first parameter to your callback
 *	   function.
 *
 * The audio device starts out playing silence when it's opened, and should
 * be enabled for playing by calling PSPL_PauseAudio(0) when you are ready
 * for your audio callback function to be called.  Since the audio driver
 * may modify the requested size of the audio buffer, you should allocate
 * any local mixing buffers after you open the audio device.
 */
extern int /*SD LCALL*/ PSPL_OpenAudio(PSPL_AUDIO_SPEC *desired, PSPL_AUDIO_SPEC *obtained);

/*
 * This function pauses and unpauses the audio callback processing.
 * It should be called with a parameter of 0 after opening the audio
 * device to start playing sound.  This is so you can safely initialize
 * data for your callback function after opening the audio device.
 * Silence will be written to the audio device during the pause.
 */
extern void /*SD LCALL*/ PSPL_PauseAudio(int pause_on);

/*
 * This function loads a WAVE from the data source, automatically freeing
 * that source if 'freesrc' is non-zero.  For example, to load a WAVE file,
 * you could do:
 *	PSPL_LoadWAV_RW(SDL_RWFromFile("sample.wav", "rb"), 1, ...);
 *
 * If this function succeeds, it returns the given PSPL_AUDIO_SPEC,
 * filled with the audio data format of the wave data, and sets
 * 'audio_buf' to a malloc()'d buffer containing the audio data,
 * and sets 'audio_len' to the length of that audio buffer, in bytes.
 * You need to free the audio buffer with PSPL_FreeWAV() when you are
 * done with it.
 *
 * This function returns NULL and sets the SDL error message if the
 * wave file cannot be opened, uses an unknown data format, or is
 * corrupt.  Currently raw and MS-ADPCM WAVE files are supported.
 */
extern PSPL_AUDIO_SPEC * /*SD LCALL*/ PSPL_LoadWAV_RW(SDL_RWops *src, int freesrc, PSPL_AUDIO_SPEC *spec, u8 **audio_buf, u32 *audio_len);

/* Compatibility convenience function -- loads a WAV from a file */
#define PSPL_LoadWAV(file, spec, audio_buf, audio_len) \
	PSPL_LoadWAV_RW(SDL_RWFromFile(file, "rb"),1, spec,audio_buf,audio_len)

/*
 * This function frees data previously allocated with PSPL_LoadWAV_RW()
 */
extern void /*SD LCALL*/ PSPL_FreeWAV(u8 *audio_buf);

/*
 * This function takes a source format and rate and a destination format
 * and rate, and initializes the 'cvt' structure with information needed
 * by PSPL_ConvertAudio() to convert a buffer of audio data from one format
 * to the other.
 * This function returns 0, or -1 if there was an error.
 */
//extern int /*SD LCALL*/ SDL_BuildAudioCVT(
//	PSPL_AUDIO_CONVERT *cvt,
//	u16 src_format,
//	u8 src_channels,
//	int src_rate,
//	u16 dst_format,
//	u8 dst_channels,//(2)pspステレオ再生固定
//	int dst_rate);
extern int /*SD LCALL*/ PSPL_BuildAudioCVT(
	PSPL_AUDIO_CONVERT *cvt,
	//--変換元
	u16 src_format,
	u8 src_channels,
	int src_rate,
	//--変換先
	u16 dst_format,
//	u8 dst_channels,//(2)pspステレオ再生固定
	int dst_rate);

/* Once you have initialized the 'cvt' structure using SDL_BuildAudioCVT(),
 * created an audio buffer cvt->buf, and filled it with cvt->len bytes of
 * audio data in the source format, this function will convert it in-place
 * to the desired format.
 * The data conversion may expand the size of the audio data, so the buffer
 * cvt->buf should be allocated after the cvt structure is initialized by
 * SDL_BuildAudioCVT(), and should be cvt->len*cvt->len_mult bytes long.
 */
extern int /*SD LCALL*/ PSPL_ConvertAudio(PSPL_AUDIO_CONVERT *cvt);

/*
 * This takes two audio buffers of the playing audio format and mixes
 * them, performing addition, volume adjustment, and overflow clipping.
 * The volume ranges from 0 - 128, and should be set to SDL_MIX_MAXVOLUME
 * for full audio volume.  Note this does not change hardware volume.
 * This is provided for convenience -- you can mix your own audio data.
 */
#define SDL_MIX_MAXVOLUME 128
extern void /*SD LCALL*/ PSPL_MixAudio(u8 *dst, const u8 *src, u32 len, int volume);

/*
 * The lock manipulated by these functions protects the callback function.
 * During a LockAudio/UnlockAudio pair, you can be guaranteed that the
 * callback function is not running.  Do not call these from the callback
 * function or you will cause deadlock.
 */
extern void /*SD LCALL*/ PSPL_LockAudio(void);
extern void /*SD LCALL*/ PSPL_UnlockAudio(void);

/*
 * This function shuts down audio processing and closes the audio device.
 */
extern void /*SD LCALL*/ PSPL_CloseAudio(void);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
//#inc lude "clo se_code.h"

#endif /* _PSPL_audio_h */
