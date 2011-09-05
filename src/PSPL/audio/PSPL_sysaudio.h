
/*---------------------------------------------------------
	ìåï˚ñÕïÌïó Å` Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_sysaudio.h,v 1.21 2004/08/21 03:21:44 slouken Exp $";
#endif

#ifndef _PSPL_sysaudio_h
#define _PSPL_sysaudio_h

#include "./../include/PSPL_mutex.h"
#include "./../include/PSPL_thread.h"

/* The SDL audio driver */
typedef struct SDL_AudioDevice SDL_AudioDevice;

/* Define the SDL audio driver structure */
#define _THIS	SDL_AudioDevice *_this
#ifndef _STATUS
#define _STATUS SDL_status *status
#endif


	/* --- */
	/* The name of this audio driver */
//	const char *name_dummy;

	/* --- */
	/* Public driver functions */
//NULL	void (*WaitDone)(_THIS);

	/* --- */
	/* Lock / Unlock functions added for the Mac port */
//	void (*LockAudio)(_THIS);
//	void (*UnlockAudio)(_THIS);




	/* --- */
	/* Data private to this driver */
//int dummy;//	struct SD L_PrivateAudioData *hidden;

struct SDL_AudioDevice
{
	/* --- */
	/* The description of this audio driver */
	const char *desc;

	/* --- */
	/* Data common to all devices */

	/* The current audio specification (shared with audio thread) */
	PSPL_AUDIO_SPEC spec;

	/* An audio conversion block for audio format emulation */
	PSPL_AUDIO_CONVERT convert;

	/* Current state flags */
	int enabled;
	int paused;
	int opened;

	/* Fake audio buffer for when the audio hardware is busy */
	u8 *fake_stream;

	/* A semaphore for locking the mixing buffers */
	SDL_mutex *mixer_lock;

	/* A thread to feed the audio device */
	SDL_Thread *thread;
	u32 threadid;
};
#undef _THIS

//typedef struct AudioBootStrap
//{
//	const char *name;
//	const char *desc;
//	int (*available)(void);
//	SDL_AudioDevice *(*create)(int devindex);
//} AudioBootStrap;

//#ifdef ENABLE_PSP
//extern AudioBootStrap PSPAUD_bootstrap;
//#endif

/* This is the current audio device */
extern SDL_AudioDevice *current_audio;

#endif /* _PSPL_sysaudio_h */
