
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_audio_c.h,v 1.5 2004/01/04 16:49:11 slouken Exp $"
 "@(#) $Id: PSPL_sysaudio.h,v 1.21 2004/08/21 03:21:44 slouken Exp $"
 "@(#) $Id: PSPL_diskaudio.h,v 1.4 2004/01/04 16:49:13 slouken Exp $"
 "@(#) $Id: PSPL_audio.c, v 1.26 2004/09/17 13:20:10 slouken Exp $"
 "@(#) $Id: PSPL_diskaudio.c,v 1.5 2004/01/04 16:49:13 slouken Exp $";
#endif
/* Allow access to a raw mixing buffer */
/* PSP port contributed by Marcus R. Brown <mrbrown@ocgnet.org>. */

#include <pspthreadman.h>/*sceKernelDelayThreadCB()*/
#include <pspaudio.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

//#include <time.h>
//#include <sys/time.h>

#include "./../include/PSPL.h"	/* 取り敢えず(仮) */
#include "./../include/PSPL_audio.h"
#include "./../include/PSPL_timer.h"
#include "./../include/PSPL_error.h"	/* 取り敢えず(仮) */
#include "PSPL_audio_c.h"
//#include "PS PL_audi ome m.h"
//#include "PS PL_audi odev_c.h"
#include "PSPL_sysaudio.h"
//#include "PSPL_pspaudio.h"

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


#define NUM_BUFFERS 	(2)

/*---------------------------------------------------------
	オーディオ再生関係で使うデーター。
---------------------------------------------------------*/

typedef struct
{
	/* The raw allocated mixing buffer. */
	u8	*rawbuf;
	/* Individual mixing buffers. */
	u8	*mixbufs[NUM_BUFFERS];
	/* Index of the next available mixing buffer. */
	int 	 next_buffer;
	/* The hardware output channel. */
	int 	 channel;
} PSPL_MY_PSP_AUDIO_DATA;
	/* Data private to this driver */
//static PSPL_MY_PSP_AUDIO_DATA *psp_hidden_audio;
static PSPL_MY_PSP_AUDIO_DATA ppp __attribute__((aligned(64/*16*/))) ;/*(64byte境界に必要???)*/




////////////////////////////////





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
	spec->size_1024 *= (spec->samples);
//	spec->size_1024 = (1024);/*psp*/
}

/* Hidden "this" pointer for the video functions */




#define USE_DELAY_BBB (0)
#if (1==USE_DELAY_BBB)
/* u32 でオーバーフローするので、1024で[us](micro seconds.)を1024で割った値を入力値とする。 */
static void SDL_Delay_us_bbb_ms1024(u32 ms1024_usec)/* 1[msec] == 1000[usec] */
{
	const u32 max_delay = (0xffffffffUL);//	(* 1000 / 1000)
	if (ms1024_usec > max_delay)
	{	ms1024_usec = max_delay;}
	sceKernelDelayThreadCB(ms1024_usec);// ms1024  /* ms x 1000*/
}
#endif

/* The general mixing thread function */
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
	//	audio->ThreadInit(audio);
	//	PSPSDLAUDIO_ThreadInit(audio);
	//	void (*ThreadInit)(SDL_AudioDevice *this);		/* Called by audio thread at start */
	//	static  void PSPSDLAUDIO_ThreadInit(SDL_AudioDevice *this)
	{
		/* Increase the priority of this audio thread by 1 to put it
		   ahead of other SDL threads. */
		SceUID thid;
		SceKernelThreadInfo status;
		thid = sceKernelGetThreadId();
		status.size = sizeof(SceKernelThreadInfo);
		if (sceKernelReferThreadStatus(thid, &status) == 0)
		{
			sceKernelChangeThreadPriority(thid, status.currentPriority - 1);
		}
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
			#if (1==USE_DELAY_BBB)
			// 0x1_4490_9900 == ????  == 44100 x1000 x1000
		//		#if 0
		//	#define ms1024_usec 	(((u64)audio->spec.samples_is1024q*1000*1000)>>10)/audio->spec.freq
		//		#else
		//	#define ms1024_usec 	(u32)((((u64)1024*1000*1000)>>10)/44100)
		//		#endif
		//	SDL_Delay_us_bbb_ms1024();
			/*
				u32 でオーバーフローするので、1024で[us](micro seconds.)を1024で割った値を入力値とする。
			*/
		//	static void SDL_Delay_us_bbb_ms1024()/* 1[msec] == 1000[usec] */
		//	u32 ms1024_usec
		//	#define  max_delay	(u32)(0xffffffffUL) /*(* 1000 / 1000)*/
		//	if (ms1024_usec > max_delay)
		//	{	ms1024_usec = max_delay;}
		//	sceKernelDelayThreadCB( (u32)((((u64)1024*1000*1000)>>10)/44100)/*ms1024_usec*/);// ms1024	/* ms x 1000*/
		//	sceKernelDelayThreadCB( (u32)(((1000*1000))/44100)/*ms1024_usec*/);// ms1024  /* ms x 1000*/
			/* samples_is1024q (mixerの場合1024, oggの場合4096, waveの場合4096) */
			SDL_Delay_us_bbb_ms1024((((u64)audio->spec.samples*1000*1000)>>10)/audio->spec.freq);
			#else
			sceKernelDelayThreadCB( (u32)(((1000*1000))/44100)/*ms1024_usec*/);// ms1024  /* ms x 1000*/
			#endif
		}
	//	else
	//	{
	//	//	/* This function waits until it is possible to write a full sound buffer */
	//	//	static void PSPAUD_WaitAudio(SDL_AudioDevice *this)
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
		//	stream = s_PSPSDLAUDIO_GetAudioBuf(audio);
		//	u8 *(*GetAudioBuf)(SDL_AudioDevice *this);
		//	static u8 *s_PSPSDLAUDIO_GetAudioBuf(SDL_AudioDevice *this)
			/* オーディオ再生で(再生中で無い)書きこんで良いバッファを取得。 */
			stream = (u8*)/*this->*/ppp.mixbufs[/*this->*/ppp.next_buffer];
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
			/* オーディオ再生で(再生中で無い)書きこんで良いバッファを取得。 */
			stream = (u8*)/*this->*/ppp.mixbufs[/*this->*/ppp.next_buffer];
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
		//	void (*PlayAudio)(SDL_AudioDevice *this);
		//	PSPSDLAUDIO_PlayAudio(audio);
		//	static void PSPSDLAUDIO_PlayAudio(SDL_AudioDevice *this)
		//	if (this->spec.channels == 1)
		//	{
		//		sceAudioOutputBlocking(/*this->*/ppp.channel, PSP_AUDIO_VOLUME_MAX, mixbuf);
		//	}
		//	else	{	}
			/* オーディオ再生する。 */
			u8 *mixbuf = /*this->*/ppp.mixbufs[/*this->*/ppp.next_buffer];
			sceAudioOutputPannedBlocking(/*this->*/ppp.channel, PSP_AUDIO_VOLUME_MAX, PSP_AUDIO_VOLUME_MAX, mixbuf);
		//	/*this->*/ppp.next_buffer = (/*this->*/ppp.next_buffer + 1) % NUM_BUFFERS;
			#if (1)
			/*this->*/ppp.next_buffer++;
			/*this->*/ppp.next_buffer &= ((2-1));
			#else
			/*this->*/ppp.next_buffer ^= (1);
			#endif
		}
	}
	/* Wait for the audio to drain.. */
//	if ( NULL/*audio->WaitDone*/ )
//	{
//		audio->WaitDone(audio);
//	}
	return (0);
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
//#define s_SDL_AllocAudioMem 	malloc
//#define s_SDL_FreeAudioMem		free


/* Audio driver functions */
//	int  (*OpenAudio)(SDL_AudioDevice *this, PSPL_AUDIO_SPEC *spec);
static int PSPSDLAUDIO_OpenAudio(SDL_AudioDevice *this, PSPL_AUDIO_SPEC *spec)
{
	switch (spec->format & 0xff)
	{
	case 8:
	case 16:
		spec->format = AUDIO_S16LSB;
		break;
	default:
		SDL_SetError_bbb("Unsupported audio format");
		return (-1);
	}

	/* The sample count must be a multiple of 64. */
	spec->samples = PSP_AUDIO_SAMPLE_ALIGN(spec->samples);
	spec->freq = (44100);

	/* Update the fragment size as size in bytes. */
	//	s111_SDL_CalculateAudioSpec(spec);
	//	static void s111_SDL_CalculateAudioSpec(PSPL_AUDIO_SPEC *spec)
	{
	//	switch (spec->format)
	//	{
	//	case AUDIO_U8:		spec->silence_dummy = 0x80; 	break;
	//	default:			spec->silence_dummy = 0x00; 	break;
	//	}
		spec->size_1024 = (2);//((spec->format&0xff)/8);
		spec->size_1024 *= (2)/*spec->channels*/;
		spec->size_1024 *= spec->samples;
	//	spec->size_1024 = (1024);/*psp*/
	}

	/* Allocate the mixing buffer.	Its size and starting address must
	   be a multiple of 64 bytes.  Our sample count is already a multiple of
	   64, so spec->size should be a multiple of 64 as well. */
	int mixlen;
	mixlen = spec->size_1024 * NUM_BUFFERS;
	/*this->*/ppp.rawbuf = (u8 *) memalign(64, mixlen);
	if (/*this->*/ppp.rawbuf == NULL)
	{
		SDL_SetError_bbb("Couldn't allocate mixing buffer");
		return (-1);
	}

	{
		int format;
		/* Setup the hardware channel. */
	//	if (spec->channels == 1)
	//	{
	//		format = PSP_AUDIO_FORMAT_MONO;
	//	}
	//	else
		{
			format = PSP_AUDIO_FORMAT_STEREO;
		}
		/*this->*/ppp.channel = sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL, spec->samples, format);
	}
	if (/*this->*/ppp.channel < 0)
	{
		SDL_SetError_bbb("Couldn't reserve hardware channel");
		free(/*this->*/ppp.rawbuf);
		/*this->*/ppp.rawbuf = NULL;
		return (-1);
	}

	memset(/*this->*/ppp.rawbuf, 0, mixlen);
	{
		int i;
		for (i=0; i<NUM_BUFFERS; i++)
		{
			/*this->*/ppp.mixbufs[i] = &/*this->*/ppp.rawbuf[i * spec->size_1024];
		}
	}
	/*this->*/ppp.next_buffer = 0;
	return (0);
}
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
	audio->fake_stream = /*s_SDL_AllocAudioMem*/memalign(16, audio->spec.size_1024);
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
			audio->convert.buf = (u8 *)/*s_SDL_AllocAudioMem*/memalign(16, audio->convert.len * audio->convert.len_mult);
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

//void PSPL_CloseAudio(void)
//{
//ここで開放は禁止		SDL_QuitSubSystem(SDL_INIT_AUDIO);
//}

	/* The function used to dispose of this structure */

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
			/*s_SDL_FreeAudioMem*/free(audio->fake_stream);
		}
		if ( audio->convert.needed )
		{
			/*s_SDL_FreeAudioMem*/free(audio->convert.buf);
		}
		//#ifndef ENA BLE_AHI
		if ( audio->opened )
		{
		//	audio->CloseAudio(audio);
		//	PSPSDLAUDIO_CloseAudio(audio);
			if (/*audio->*/ppp.channel >= 0)
			{
				sceAudioChRelease(/*audio->*/ppp.channel);
				/*audio->*/ppp.channel = -1;
			}
			if (/*audio->*/ppp.rawbuf != NULL)
			{
				free(/*audio->*/ppp.rawbuf);
				/*audio->*/ppp.rawbuf = NULL;
			}
			audio->opened = 0;
		}
		//#endif
		/* Free the driver data */
	//	audio->free(audio);
	//	PSPSDLAUDIO_DeleteDevice(audio);
//		free(/*audio->*/psp_hidden_audio);
		free(audio);
		current_audio = NULL;
	}
}



/*only boot once*/void PSPL_AudioInit(void/*const char *driver_name_dummy*/)
{
	/* Initialize all variables that we clean on shutdown */
	current_audio = NULL;
	current_audio = (SDL_AudioDevice *)memalign(16, sizeof(SDL_AudioDevice));
	memset(current_audio, 0, (sizeof *current_audio));
//	/*current_audio->*/psp_hidden_audio = (PSPL_MY_PSP_AUDIO_DATA *)malloc((sizeof */*current_audio->*/psp_hidden_audio));
//	memset(/*current_audio->*/psp_hidden_audio, 0, (sizeof */*current_audio->*/psp_hidden_audio));
	memset(/*current_audio->*/&ppp, 0, (sizeof(/*current_audio->*/ppp)));
}
		/* Available audio drivers */
//	SDL_AudioDevice *audio;
	/* Select the proper audio driver */
	//	static AudioBootStrap *bootstrap[] =
	//	{
	//	//	#ifdef ENABLE_PSP
	//		&PSPAUD_bootstrap,
	//	//	#endif
	//		NULL
	//	};
	//	audio = bootstrap[0/*i*/]->create(0/*index*/);
	//	audio = PSPAUD_CreateDevice(0/*index*/);
	//	static SDL_AudioDevice *PSPAUD_CreateDevice(int devindex)

	/* Set the function pointers */
//	audio->OpenAudio 	= PSPSDLAUDIO_OpenAudio;
//	audio->ThreadInit	= PSPSDLAUDIO_ThreadInit;
//廃止		audio->WaitAudio 	= PSPSDLAUDIO_WaitAudio;
//	audio->PlayAudio 	= PSPSDLAUDIO_PlayAudio;
//	audio->GetAudioBuf	= PSPSDLAUDIO_GetAudioBuf;
//	audio->CloseAudio	= PSPSDLAUDIO_CloseAudio;

//	audio->free			= PSPSDLAUDIO_DeleteDevice;

	//	if ( current_audio )
	//	{
//????? 		current_audio->name = bootstrap[0/*i*/]->name;
	//		if ( !current_audio->LockAudio && !current_audio->UnlockAudio )
	//		{
	//			current_audio->LockAudio	= PSPSDLAUDIO_LockAudio;
	//			current_audio->UnlockAudio	= PSPSDLAUDIO_UnlockAudio;
	//		}
	//	}
//	return (0);


/*(???)*/

/* This function waits until it is possible to write a full sound buffer */
//廃止	void (*WaitAudio)(SDL_AudioDevice *this);
//static void PSPSDLAUDIO_WaitAudio(SDL_AudioDevice *this)
//{
//	/* Because we block when sending audio, there's no need for this function to do anything. */
//}

/* Audio driver bootstrap functions */

//static int PSPSDLAUDIO_Available(void)
//{
//	return (1);
//}

//AudioBootStrap PSPSDLAUDIO_bootstrap =
//{
//	/* The tag name used by PSP audio */
//	#define PSPSDLAUDIO_DRIVER_NAME		"psp"
//	PSPSDLAUDIO_DRIVER_NAME,
//	"PSP audio driver",
//	PSPSDLAUDIO_Available,
//		PSPSDLAUDIO_CreateDevice
//};
/* vim: ts=4 sw=4
 */

/*---------------------------------------------------------
	外部I/F
	-------------------------------------------------------
	外部から操作するインターフェース部分。
---------------------------------------------------------*/

/*---------------------------------------------------------
	ロック解除
	-------------------------------------------------------
	ロック解除して他からの書きこみを許可する。
---------------------------------------------------------*/
void PSPL_UnlockAudio(void)
{
	SDL_AudioDevice *audio = current_audio;
	/* Release lock on the mixing buffers */
	if ( audio ) /*&& audio->UnlockAudio*/
	{
	//	audio->UnlockAudio(audio);
	//	PSPSDLAUDIO_UnlockAudio(audio);
	//	static void SDL_Unl ockAud io_Default(SDL_AudioDevice *audio)
	//	static void PSPSDLAUDIO_UnlockAudio(SDL_AudioDevice *this)
		if (audio->thread
			 && (SDL_ThreadID() == audio->threadid) )
		{
			return;
		}
		SDL_mutexV(audio->mixer_lock);
	}
}

/*---------------------------------------------------------
	ロック
	-------------------------------------------------------
	ロックして他からの書きこみを禁止する。
---------------------------------------------------------*/
void PSPL_LockAudio(void)
{
	SDL_AudioDevice *audio = current_audio;
	/* Obtain a lock on the mixing buffers */
	if ( audio ) /*&& audio->LockAudio*/
	{
	//	audio->LockAudio(audio);
	//	PSPSDLAUDIO_LockAudio(audio);
	//	static void SDL_Loc kAud io_Default(SDL_AudioDevice *audio)
	//	static void PSPSDLAUDIO_LockAudio(SDL_AudioDevice *this)
		if (audio->thread
			&& (SDL_ThreadID() == audio->threadid) )
		{
			return;
		}
		SDL_mutexP(audio->mixer_lock);
	}
}

/*---------------------------------------------------------
	一時停止
	-------------------------------------------------------
	一時停止モードのon/off.
---------------------------------------------------------*/
void PSPL_PauseAudio(int pause_on)
{
	SDL_AudioDevice *audio = current_audio;
	if ( audio )
	{
		audio->paused = pause_on;
	}
}
