
/*---------------------------------------------------------
	ìåï˚ñÕïÌïó Å` Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_audio_c.h,v 1.5 2004/01/04 16:49:11 slouken Exp $";
#endif

/* Functions and variables exported from PSPL_audio.c for PSPL_sysaudio.c */

/* Functions to get a list of "close" audio formats */
extern u16 SDL_FirstAudioFormat(u16 format);
extern u16 SDL_NextAudioFormat(void);

/* Function to calculate the size and silence for a PSPL_AUDIO_SPEC */
extern void SDL_CalculateAudioSpec(PSPL_AUDIO_SPEC *spec);

/* The actual mixing thread function */
extern int SDL_RunAudio(void *audiop);
