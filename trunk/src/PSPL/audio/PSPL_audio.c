
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_audio.c, v 1.26 2004/09/17 13:20:10 slouken Exp $";
#endif

/* Allow access to a raw mixing buffer */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <malloc.h>


//#include <time.h>
//#include <sys/time.h>
#include <pspthreadman.h>/*sceKernelDelayThreadCB()*/

#include "./../include/PSPL.h"	/* 取り敢えず(仮) */
#include "./../include/PSPL_audio.h"
#include "./../include/PSPL_timer.h"
#include "./../include/PSPL_error.h"	/* 取り敢えず(仮) */
#include "PSPL_audio_c.h"
//#include "PS PL_audi ome m.h"
#include "PSPL_sysaudio.h"
#include "PSPL_pspaudio.h"

#ifndef SDL_Error_bbb
	#define SDL_Error_bbb(aaa)
#endif

#ifndef SDL_OutOfMemory_bbb
	#define SDL_OutOfMemory_bbb(aaa)
#endif

#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb( ... )
#endif

//#ifndef SDL_Delay_bbb
//	#define SDL_Delay_bbb(aaa)
//#endif

/*
	u32 でオーバーフローするので、1024で[us](micro seconds.)を1024で割った値を入力値とする。
*/
static void SDL_Delay_us_bbb_ms1024(u32 ms1024_usec)/* 1[msec] == 1000[usec] */
{
	const u32 max_delay = (0xffffffffUL);//	(* 1000 / 1000)
	if (ms1024_usec > max_delay)
	{	ms1024_usec = max_delay;}
	sceKernelDelayThreadCB(ms1024_usec);// ms1024  /* ms x 1000*/
}


SDL_AudioDevice *current_audio = NULL;

///#include "PSPL_audiomem.h"


/* (詳細がよくわかんない) */
/*
	フォーマットに合わせて、発音バッファのサイズを計算して決める。
*/
static void ssss_SDL_CalculateAudioSpec(PSPL_AUDIO_SPEC *spec)
{
//	switch (spec->format)
//	{
//	case AUDIO_U8:		spec->silence_dummy = 0x80; 	break;
//	default:			spec->silence_dummy = 0x00; 	break;
//	}
	spec->size_1024 = ((spec->format&0xff)/8);
	spec->size_1024 *= (2);//spec->channels_is_load_wave_only;
	spec->size_1024 *= spec->samples;
//	spec->size_1024 = (1024);/*psp*/
}

/* The general mixing thread function */
/*static*/extern void PSPSDLAUDIO_ThreadInit(_THIS);
/*static*/extern u8 *PSPSDLAUDIO_GetAudioBuf(_THIS);
/*static*/extern void PSPSDLAUDIO_PlayAudio(_THIS);
int SDL_RunAudio(void *audiop)
{
	SDL_AudioDevice 	*audio = (SDL_AudioDevice *)audiop;
	u8	*stream;
	int 	stream_len;
	void	*udata;
	void	(*fill)(void *userdata, u8 *stream, int len);
//	int 	silence;

	/* Perform any thread setup */
//	if ( audio->ThreadInit )
	{
	//	audio->ThreadInit(audio);
		PSPSDLAUDIO_ThreadInit(audio);
	}
	audio->threadid = SDL_ThreadID();

	/* Set up the mixing function */
	fill  = audio->spec.callback;
	udata = audio->spec.userdata;

	if ( audio->convert.needed )
	{
//		if ( audio->convert.src_format == AUDIO_U8 )
//		{
//			silence = 0x80;
//		}
//		else
//		{
//			silence = 0;
//		}
		stream_len = audio->convert.len;
	}
	else
	{
//		silence = audio->spec.silence;
		stream_len = audio->spec.size_1024;
	}
	stream = audio->fake_stream;


	/* Loop, filling the audio buffers */
	while ( audio->enabled )
	{
		/* Wait for new current buffer to finish playing */
		if ( stream == audio->fake_stream )
		{
			// 0x1_4490_9900 == ????  == 44100 x1000 x1000
			SDL_Delay_us_bbb_ms1024((((u64)audio->spec.samples*1000*1000)>>10)/audio->spec.freq);
		}
	//	else
	//	{
	//	//	/* This function waits until it is possible to write a full sound buffer */
	//	//	static void PSPAUD_WaitAudio(_THIS)
	//	//	{
	//	//		/* Because we block when sending audio, there's no need for this function to do anything. */
	//	//	}
	//	//	audio->WaitAudio(audio);
	//	}

		/* Fill the current buffer with sound */
		if ( audio->convert.needed )
		{
			if ( audio->convert.buf )
			{
				stream = audio->convert.buf;
			}
			else
			{
				continue;
			}
		}
		else
		{
		//	stream = audio->GetAudioBuf(audio);
			stream = PSPSDLAUDIO_GetAudioBuf(audio);
			if ( stream == NULL )
			{
				stream = audio->fake_stream;
			}
		}
		memset(stream, 0/*silence*/, stream_len);

		if ( ! audio->paused )
		{
			SDL_mutexP(audio->mixer_lock);
			(*fill)(udata, stream, stream_len);
			SDL_mutexV(audio->mixer_lock);
		}

		/* Convert the audio if necessary */
		if ( audio->convert.needed )
		{
			PSPL_ConvertAudio(&audio->convert);
		//	stream = audio->GetAudioBuf(audio);
			stream = PSPSDLAUDIO_GetAudioBuf(audio);
			if ( stream == NULL )
			{
				stream = audio->fake_stream;
			}
			memcpy(stream, audio->convert.buf, audio->convert.len_cvt);
		}

		/* Ready current buffer for play and change current buffer */
		if ( stream != audio->fake_stream )
		{
		//	audio->PlayAudio(audio);
			PSPSDLAUDIO_PlayAudio(audio);
		}
	}
	/* Wait for the audio to drain.. */
//	if ( NULL/*audio->WaitDone*/ )
//	{
//		audio->WaitDone(audio);
//	}
	return (0);
}

//static void SDL_Loc kAud io_Default(SDL_AudioDevice *audio)
static void PSPSDLAUDIO_LockAudio(_THIS)
{
	if ( this->thread && (SDL_ThreadID() == this->threadid) )
	{
		return;
	}
	SDL_mutexP(this->mixer_lock);
}

//static void SDL_Unl ockAud io_Default(SDL_AudioDevice *audio)
static void PSPSDLAUDIO_UnlockAudio(_THIS)
{
	if ( this->thread && (SDL_ThreadID() == this->threadid) )
	{
		return;
	}
	SDL_mutexV(this->mixer_lock);
}


/* Allocate memory that will be shared between threads (freed on exit) */
//static void *s_SDL_AllocAudioMem(int size)
//{
//	void *chunk;
//	chunk = malloc(size);
//	return ((void *)chunk);
//}
//
//static void s_SDL_FreeAudioMem(void *chunk)
//{
//	free(chunk);
//}
#define s_SDL_AllocAudioMem 	malloc
#define s_SDL_FreeAudioMem		free

/*static*/extern int PSPSDLAUDIO_OpenAudio(_THIS, PSPL_AUDIO_SPEC *spec);
int PSPL_OpenAudio(PSPL_AUDIO_SPEC *desired, PSPL_AUDIO_SPEC *obtained)
{
	SDL_AudioDevice *audio;
	/* Start up the audio driver, if necessary */
//	if ( ! current_audio )
//	{
//		if ( (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) ||
//			 (current_audio == NULL) )
//		{
//			return (-1);
//		}
//	}
	audio = current_audio;

	if (audio->opened)
	{
		SDL_SetError_bbb("Audio device is already opened");
		return (-1);
	}

	/* Verify some parameters */
	if ( desired->callback == NULL )
	{
		SDL_SetError_bbb("PSPL_OpenAudio() passed a NULL callback");
		return (-1);
	}
	#if 0
	switch ( desired->channels ) //(2)pspステレオ再生固定
	{
	case 1: /* Mono */
	case 2: /* Stereo */
//	case 4: /* surround */
//	case 6: /* surround with center and lfe */
		break;
	default:
		SDL_SetError_bbb("1 (mono) and 2 (stereo) channels supported");
		return (-1);
	}
	#endif

#if defined(macintosh) || (defined(__riscos__) && !defined(DISABLE_THREADS))
	/* FIXME: Need to implement PPC interrupt asm for PSPL_LockAudio() */
#else
	#if defined(__MINT__) && !defined(ENABLE_THREADS)
		/* Uses interrupt driven audio, without thread */
	#else
		/* Create a semaphore for locking the sound buffers */
		audio->mixer_lock = SDL_CreateMutex();
		if ( audio->mixer_lock == NULL )
		{
			SDL_SetError_bbb("Couldn't create mixer lock");
			// ここで開放は禁止 	PSPL_CloseAudio();
			return (-1);
		}
	#endif /* __MINT__ */
#endif /* macintosh */

	/* Calculate the silence and size of the audio specification */
	ssss_SDL_CalculateAudioSpec(desired);

	/* Open the audio subsystem */
	memcpy(&audio->spec, desired, sizeof(audio->spec));
	audio->convert.needed = 0;
	audio->enabled = 1;
	audio->paused  = 1;

	//#ifndef ENA BLE_AHI
	/* AmigaOS opens audio inside the main loop */
//	audio->opened = audio->OpenAudio(audio, &audio->spec) + 1;
	audio->opened = PSPSDLAUDIO_OpenAudio(audio, &audio->spec) + 1;

	if ( ! audio->opened )
	{
	// ここで開放は禁止 	PSPL_CloseAudio();
		return (-1);
	}
	//#endif

	/* If the audio driver changes the buffer size, accept it */
	if ( audio->spec.samples != desired->samples )
	{
		desired->samples = audio->spec.samples;
		ssss_SDL_CalculateAudioSpec(desired);
	}

	/* Allocate a fake audio memory buffer */
	audio->fake_stream = s_SDL_AllocAudioMem(audio->spec.size_1024);
	if ( audio->fake_stream == NULL )
	{
	// ここで開放は禁止 	PSPL_CloseAudio();
		SDL_OutOfMemory_bbb();
		return (-1);
	}

	/* See if we need to do any conversion */
	if ( obtained != NULL )
	{
		memcpy(obtained, &audio->spec, sizeof(audio->spec));
	}
	else
	if (		desired->freq != audio->spec.freq
			||	desired->format != audio->spec.format
		//??????//	||	desired->channels != audio->spec.channels  //(2)pspステレオ再生固定
	)
	{
		/* Build an audio conversion block */
		if (
			PSPL_BuildAudioCVT(
				&audio->convert,
				//--変換元
				desired->format,
				(2),//	desired->channels, //(2)pspステレオ再生固定
				desired->freq,
				//--変換先
				audio->spec.format,
				#ifdef _OLD_SDL_LINK_MODE_/*(旧互換機能)*/
				(2),//	audio->spec.channels, //(2)pspステレオ再生固定
				#endif
				audio->spec.freq) < 0 )
		{
		// ここで開放は禁止 	PSPL_CloseAudio();
			return (-1);
		}
		if ( audio->convert.needed )
		{
			audio->convert.len = desired->size_1024;
			audio->convert.buf = (u8 *)s_SDL_AllocAudioMem(audio->convert.len * audio->convert.len_mult);
			if ( audio->convert.buf == NULL )
			{
			// ここで開放は禁止 	PSPL_CloseAudio();
				SDL_OutOfMemory_bbb();
				return (-1);
			}
		}
	}

	//#ifndef ENA BLE_AHI
	/* Start the audio thread if necessary */
	switch (audio->opened)
	{
	case  1:
		/* Start the audio thread */
		audio->thread = SDL_CreateThread(SDL_RunAudio, audio);
		if ( audio->thread == NULL )
		{
		// ここで開放は禁止 	PSPL_CloseAudio();
			SDL_SetError_bbb("Couldn't create audio thread");
			return (-1);
		}
		break;
	default:
		/* The audio is now playing */
		break;
	}
	//#endif
	return (0);
}

void PSPL_PauseAudio(int pause_on)
{
	SDL_AudioDevice *audio = current_audio;
	if ( audio )
	{
		audio->paused = pause_on;
	}
}

void PSPL_LockAudio(void)
{
	SDL_AudioDevice *audio = current_audio;
	/* Obtain a lock on the mixing buffers */
	if ( audio /*&& audio->LockAudio*/ )
	{
	//	audio->LockAudio(audio);
		PSPSDLAUDIO_LockAudio(audio);
	}
}

void PSPL_UnlockAudio(void)
{
	SDL_AudioDevice *audio = current_audio;
	/* Release lock on the mixing buffers */
	if ( audio /*&& audio->UnlockAudio*/ )
	{
	//	audio->UnlockAudio(audio);
		PSPSDLAUDIO_UnlockAudio(audio);
	}
}

//void PSPL_CloseAudio(void)
//{
//ここで開放は禁止		SDL_QuitSubSystem(SDL_INIT_AUDIO);
//}
/*static*/extern void PSPSDLAUDIO_DeleteDevice(_THIS);
/*static*/extern void PSPSDLAUDIO_CloseAudio(_THIS);
/*only exit once*/void PSPL_AudioQuit(void)
{
	SDL_AudioDevice *audio = current_audio;
	if ( audio )
	{
		audio->enabled = 0;
		if ( audio->thread != NULL )
		{
			SDL_WaitThread(audio->thread, NULL);
		}
		if ( audio->mixer_lock != NULL )
		{
			SDL_DestroyMutex(audio->mixer_lock);
		}
		if ( audio->fake_stream != NULL )
		{
			s_SDL_FreeAudioMem(audio->fake_stream);
		}
		if ( audio->convert.needed )
		{
			s_SDL_FreeAudioMem(audio->convert.buf);
		}
		//#ifndef ENA BLE_AHI
		if ( audio->opened )
		{
		//	audio->CloseAudio(audio);
			PSPSDLAUDIO_CloseAudio(audio);
			audio->opened = 0;
		}
		//#endif
		/* Free the driver data */
	//	audio->free(audio);
		PSPSDLAUDIO_DeleteDevice(audio);
		current_audio = NULL;
	}
}

/*static*/extern SDL_AudioDevice *PSPAUD_CreateDevice(int devindex);
/*only boot once*/void PSPL_AudioInit(void/*const char *driver_name_dummy*/)
{
	/* Select the proper audio driver */
	SDL_AudioDevice *audio;
	{
		/* Available audio drivers */
	//	static AudioBootStrap *bootstrap[] =
	//	{
	//	//	#ifdef ENABLE_PSP
	//		&PSPAUD_bootstrap,
	//	//	#endif
	//		NULL
	//	};
	//	audio = bootstrap[0/*i*/]->create(0/*index*/);
		audio = PSPAUD_CreateDevice(0/*index*/);
		current_audio = audio;
	//	if ( current_audio )
	//	{
//????? 		current_audio->name = bootstrap[0/*i*/]->name;
	//		if ( !current_audio->LockAudio && !current_audio->UnlockAudio )
	//		{
	//			current_audio->LockAudio	= PSPSDLAUDIO_LockAudio;
	//			current_audio->UnlockAudio	= PSPSDLAUDIO_UnlockAudio;
	//		}
	//	}
	}
//	return (0);
}
