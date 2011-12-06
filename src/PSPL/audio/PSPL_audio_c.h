
/*---------------------------------------------------------
	ìåï˚ñÕïÌïó Å` Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

/* Functions and variables exported from PSPL_audio.c for PSPL_sysaudio.c */

/* Functions to get a list of "close" audio formats */
extern u16 SDL_FirstAudioFormat(u16 format);
extern u16 SDL_NextAudioFormat(void);

/* Function to calculate the size and silence for a PSPL_AUDIO_SPEC */
extern void SDL_CalculateAudioSpec(PSPL_AUDIO_SPEC *spec);

/* The actual mixing thread function */
extern int SDL_RunAudio(void *audiop);
