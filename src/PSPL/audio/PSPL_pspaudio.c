
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

/* PSP port contributed by Marcus R. Brown <mrbrown@ocgnet.org>. */

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_diskaudio.c,v 1.5 2004/01/04 16:49:13 slouken Exp $";
#endif


#include <pspaudio.h>
#include <pspthreadman.h>



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "./../include/PSPL_audio.h"
#include "./../include/PSPL_error.h"
//#include "PS PL_aud iome m.h"
#include "PSPL_audio_c.h"
#include "./../include/PSPL_timer.h"
//#include "PS PL_aud iodev_c.h"

#include "PSPL_sysaudio.h"
#include "PSPL_pspaudio.h"

#ifndef SDL_OutOfMemory_bbb
	#define SDL_OutOfMemory_bbb(aaa)
#endif

#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb( ... )
#endif


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
static PSPL_MY_PSP_AUDIO_DATA *psp_hidden_audio;




/*---------------------------------------------------------
	オーディオ再生する。
---------------------------------------------------------*/

//	void (*PlayAudio)(_THIS);
/*static*/extern void PSPSDLAUDIO_PlayAudio(_THIS)
{
	u8 *mixbuf = /*this->*/psp_hidden_audio->mixbufs[/*this->*/psp_hidden_audio->next_buffer];
//	if (this->spec.channels == 1)
//	{
//		sceAudioOutputBlocking(/*this->*/psp_hidden_audio->channel, PSP_AUDIO_VOLUME_MAX, mixbuf);
//	}
//	else
	{
		sceAudioOutputPannedBlocking(/*this->*/psp_hidden_audio->channel, PSP_AUDIO_VOLUME_MAX, PSP_AUDIO_VOLUME_MAX, mixbuf);
	}

//	/*this->*/psp_hidden_audio->next_buffer = (/*this->*/psp_hidden_audio->next_buffer + 1) % NUM_BUFFERS;
	#if (1)
	/*this->*/psp_hidden_audio->next_buffer++;
	/*this->*/psp_hidden_audio->next_buffer &= ((2-1));
	#else
	/*this->*/psp_hidden_audio->next_buffer ^= (1);
	#endif
}

/*---------------------------------------------------------
	オーディオ再生で(再生中で無い)書きこんで良いバッファを取得。
---------------------------------------------------------*/

//	u8 *(*GetAudioBuf)(_THIS);
/*static*/extern u8 *PSPSDLAUDIO_GetAudioBuf(_THIS)
{
	return /*this->*/psp_hidden_audio->mixbufs[/*this->*/psp_hidden_audio->next_buffer];
}










/*(???)*/

/* Audio driver functions */
//	int  (*OpenAudio)(_THIS, PSPL_AUDIO_SPEC *spec);
/*static*/extern int PSPSDLAUDIO_OpenAudio(_THIS, PSPL_AUDIO_SPEC *spec)
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
	spec->freq = 44100;

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
	/*this->*/psp_hidden_audio->rawbuf = (u8 *) memalign(64, mixlen);
	if (/*this->*/psp_hidden_audio->rawbuf == NULL)
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
		/*this->*/psp_hidden_audio->channel = sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL, spec->samples, format);
	}
	if (/*this->*/psp_hidden_audio->channel < 0)
	{
		SDL_SetError_bbb("Couldn't reserve hardware channel");
		free(/*this->*/psp_hidden_audio->rawbuf);
		/*this->*/psp_hidden_audio->rawbuf = NULL;
		return (-1);
	}

	memset(/*this->*/psp_hidden_audio->rawbuf, 0, mixlen);
	{
		int i;
		for (i=0; i<NUM_BUFFERS; i++)
		{
			/*this->*/psp_hidden_audio->mixbufs[i] = &/*this->*/psp_hidden_audio->rawbuf[i * spec->size_1024];
		}
	}
	/*this->*/psp_hidden_audio->next_buffer = 0;
	return (0);
}


//	void (*ThreadInit)(_THIS);		/* Called by audio thread at start */
/*static*/extern void PSPSDLAUDIO_ThreadInit(_THIS)
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


/* This function waits until it is possible to write a full sound buffer */
//廃止	void (*WaitAudio)(_THIS);
//static void PSPSDLAUDIO_WaitAudio(_THIS)
//{
//	/* Because we block when sending audio, there's no need for this function to do anything. */
//}




//	void (*CloseAudio)(_THIS);
/*static*/extern void PSPSDLAUDIO_CloseAudio(_THIS)
{
	if (/*this->*/psp_hidden_audio->channel >= 0)
	{
		sceAudioChRelease(/*this->*/psp_hidden_audio->channel);
		/*this->*/psp_hidden_audio->channel = -1;
	}
	if (/*this->*/psp_hidden_audio->rawbuf != NULL)
	{
		free(/*this->*/psp_hidden_audio->rawbuf);
		/*this->*/psp_hidden_audio->rawbuf = NULL;
	}
}


/* Audio driver bootstrap functions */


	/* * * */
	/* The function used to dispose of this structure */
//	void (*free)(_THIS);
/*static*/extern void PSPSDLAUDIO_DeleteDevice(_THIS)
{
	free(/*this->*/psp_hidden_audio);
	free(this);
}

/*static*/extern SDL_AudioDevice *PSPAUD_CreateDevice(int devindex)
{
	SDL_AudioDevice *this;
	/* Initialize all variables that we clean on shutdown */
	this = (SDL_AudioDevice *)malloc(sizeof(SDL_AudioDevice));
	if ( this )
	{
		memset(this, 0, (sizeof *this));
		/*this->*/psp_hidden_audio =
			(PSPL_MY_PSP_AUDIO_DATA *)malloc((sizeof */*this->*/psp_hidden_audio));
	}
	if ( (this == NULL) || (/*this->*/psp_hidden_audio == NULL) )
	{
		SDL_OutOfMemory_bbb();
		if ( this )
		{
			free(this);
		}
		return (0);
	}
	memset(/*this->*/psp_hidden_audio, 0, (sizeof */*this->*/psp_hidden_audio));

	/* Set the function pointers */
//	this->OpenAudio 	= PSPSDLAUDIO_OpenAudio;
//	this->ThreadInit	= PSPSDLAUDIO_ThreadInit;
//廃止		this->WaitAudio 	= PSPSDLAUDIO_WaitAudio;
//	this->PlayAudio 	= PSPSDLAUDIO_PlayAudio;
//	this->GetAudioBuf	= PSPSDLAUDIO_GetAudioBuf;
//	this->CloseAudio	= PSPSDLAUDIO_CloseAudio;

//	this->free			= PSPSDLAUDIO_DeleteDevice;
	return (this);
}

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
