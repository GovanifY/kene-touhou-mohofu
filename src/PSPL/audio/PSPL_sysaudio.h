
/*---------------------------------------------------------
	ìåï˚ñÕïÌïó Å` Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifndef _PSPL_sysaudio_h
#define _PSPL_sysaudio_h

#include "./../include/PSPL_mutex.h"
#include "./../include/PSPL_thread.h"

/* The SDL audio driver */
typedef struct SDL_AudioDevice SDL_AudioDevice;

/* Define the SDL audio driver structure */

#ifndef _STATUS
#define _STATUS SDL_status *status
#endif


	/* --- */
	/* The name of this audio driver */
//	const char *name_dummy;

	/* --- */
	/* Public driver functions */
//#define _THIS	SDL_AudioDevice *_this
//NULL	void (*WaitDone)(_THIS);

	/* --- */
	/* Lock / Unlock functions added for the Mac port */
//	void (*LockAudio)(_THIS);
//	void (*UnlockAudio)(_THIS);
//#undef _THIS




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
