
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

/*
    SDL_mixer:	An audio mixer library based on the SDL library
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* $Id: SD L_mixer.h,v 1.30 2004/01/04 17:37:04 slouken Exp $ */

#ifndef _MOHOUFU_MIXER_H_
#define _MOHOUFU_MIXER_H_

//typedef struct SDL_version
//{
//	u8 major;
//	u8 minor;
//	u8 patch;
//} SDL_version;

//#include <SD L/SDL_types.h>//#include "SDL_types.h"
//#include <SD L/SDL_rwops.h>//#include "SDL_rwops.h"
//#include <SD L/SDL_audio.h>//#include "SDL_audio.h"
//#include <SD L/SDL_byteorder.h>//#include "SDL_byteorder.h"
//#include <SD L/SDL_version.h>//#include "SDL_version.h"
//#include <SD L/begin_code.h>////#inc lude "beg in_code.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Good default values for a PC soundcard */
//#define MIX_DEFAULT_FREQUENCY	44100/*22050*/
//#if SDL_BYTEORDER == SDL_LIL_ENDIAN
//	#define MIX_DEFAULT_FORMAT		AUDIO_S16LSB
//#else
//	#define MIX_DEFAULT_FORMAT		AUDIO_S16MSB
//#endif
//#define MIX_DEFAULT_CHANNELS	2
#define MIX_MAX_VOLUME			128 	/* Volume of a chunk */

#if 000
/* The different fading types supported */
typedef enum
{
	MIX_NO_FADING,
	MIX_FADING_OUT,
	MIX_FADING_IN
} Mix_Fading;

#endif


#include "./music_mixer.h"

/* We'll use SDL for reporting errors */
#define Mix_SetError	SDL_SetError
#define Mix_GetError	SDL_GetError






/* Customize mohou SDL Mixer setting. */

#define USE_SWAP_FORMAT (0)
#define USE_SURROUND	(0)




/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
//#include <SD L/close_code.h>////#inc lude "clo se_code.h"

#endif /* _MOHOUFU_MIXER_H_ */

