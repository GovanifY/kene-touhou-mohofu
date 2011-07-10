
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_endian.c,v 1.5 2004/01/04 16:49:17 slouken Exp $";
#endif

/* Functions for dynamically reading and writing endian-specific values */

#include "./include/PSPL_endian.h"



/* bmp */
int SDL_WriteLE16(SDL_RWops *dst, u16 value)
{
	value = SDL_SwapLE16(value);
	return (SDL_RWwrite(dst, &value, (sizeof value), 1));
}


/* src/audio_mixer/wavestream.c */
/* src/audio_mixer/mixer.c */
/* src/PSPL/audio/PSPL_wave.c  ... */
/*static*/ u32 SDL_ReadLE32(SDL_RWops *src)
{
	u32 value;
	SDL_RWread(src, &value, (sizeof value), 1);
	return (SDL_SwapLE32(value));
}

int SDL_WriteLE32(SDL_RWops *dst, u32 value)
{
	value = SDL_SwapLE32(value);
	return (SDL_RWwrite(dst, &value, (sizeof value), 1));
}


