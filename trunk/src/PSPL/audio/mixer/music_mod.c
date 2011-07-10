
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

/*
	SDL_mixer:	An audio mixer library based on the SDL library
	Copyright (C) 1997-2009 Sam Lantinga

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

/* $Id: music_mod.c 4211 2008-12-08 00:27:32Z slouken $ */
#ifdef _SDL_MIXER_H
error! "このプログラムは SDL_mixer.h がインクルードされていると、コンパイルできません。"
#endif
#define _SDL_MIXER_H

#include <stdlib.h>
#include <malloc.h>

#include <string.h>
#include <ctype.h>

//#include <SD L/SDL_endian.h>//#include "SD L_endian.h"
//#include <SD L/SDL_audio.h>//#include "SD L_audio.h"
//#include <SD L/SDL_timer.h>//#include "SD L_timer.h"

/* need new version. */
#include "./../../include/PSPL_audio.h"
#include "./000SDL_mixer.h"//#include <SD L/SD L_mixer.h>//#include "SD L_mixer.h"

#ifndef Mix_SetError_bbb
	#define Mix_SetError_bbb(...)
#endif

#ifndef SDL_Delay_bbb
	#define SDL_Delay_bbb(aaa)
#endif



#if defined(MOD_MUSIC) || defined(LIB_MOHOU_MOD_MUSIC)
//	#include "./../mikmod/mohou_mikmod.h"// #include "mikmod.h"
	#include "./../../../module_music/mohou_mikmod.h"// #include "mikmod.h"
	#if defined(LIB_MOHOU_MOD_VERSION)		/* libmikmod 3.1.8 */
		#define UNIMOD					MOD_MUSIC_MODULE
		#define MusicMod_Init() 		MusicMod_Init(/*NULL*/)
		#define MusicMod_LoadSong(a,b)	Player_Load(a,b,0)
	//	#if 0 //def US E_RWOPS
	//		#ifndef LIB_MOHOU_MOD_MUSIC
	//			#define MusicMod_LoadSongRW(a,b)	Player_LoadRW(a,b,0)
	//		#endif
	//	#endif
		#define MusicMod_FreeSong		Player_Free
		extern int MusicMod_errno;
	#else								/* old MusicMod 3.0.3 */
		#define MusicMod_strerror(x)		mod_music_errmsg[x])
		#define MusicMod_errno			mod_music_error_number
	#endif
#endif



#if 0 //def US E_RWOPS

#if 00/*模倣風で使用していない*/
#ifdef (LIB_MOHOU_MOD_MUSIC)

typedef struct
{
	MF_READER mr;
	int offset;
	int eof;
	SDL_RWops *rw;
} LMM_MF_READER;

MM_BOOL LMM_Seek(struct MF_READER *mr, long to, int dir)
{
	LMM_MF_READER* lmmmr = (LMM_MF_READER*)mr;
	if ( dir == SEEK_SET )
	{
		to += lmmmr->offset;
	}
	return (SDL_RWseek(lmmmr->rw, to, dir) < lmmmr->offset);
}
long LMM_Tell(struct MF_READER *mr)
{
	LMM_MF_READER* lmmmr = (LMM_MF_READER*)mr;
	return (SDL_RWtell(lmmmr->rw)-lmmmr->offset);
}
MM_BOOL LMM_Read(struct MF_READER *mr, void *buf, size_t sz)
{
	LMM_MF_READER* lmmmr = (LMM_MF_READER*)mr;
	return (SDL_RWread(lmmmr->rw, buf, sz, 1));
}
int LMM_Get(struct MF_READER *mr)
{
	u8 c;
	LMM_MF_READER* lmmmr = (LMM_MF_READER*)mr;
	if (SDL_RWread(lmmmr->rw, &c, 1, 1))
	{
		return (c);
	}
	return (EOF);
}
MM_BOOL LMM_Eof(struct MF_READER *mr)
{
	int offset;
	LMM_MF_READER* lmmmr = (LMM_MF_READER*)mr;
	offset = LMM_Tell(mr);
	return (offset>=lmmmr->eof);
}
MOD_MUSIC_MODULE *MusicMod_LoadSongRW(SDL_RWops *rw, int maxchan)
{
	LMM_MF_READER lmmmr =
	{
		LMM_Seek,
		LMM_Tell,
		LMM_Read,
		LMM_Get,
		LMM_Eof,
		0,
		0,
		0/*rw*/
	};
	lmmmr.offset	= SDL_RWtell(rw);
	SDL_RWseek(rw, 0, SEEK_END);
	lmmmr.eof		= SDL_RWtell(rw);
	SDL_RWseek(rw, lmmmr.offset, SEEK_SET);
	lmmmr.rw		= rw;
	//
	MOD_MUSIC_MODULE *m;
	m				= Player_LoadGeneric((MF_READER*)&lmmmr, maxchan, 0);
	return (m);
}
#endif
#endif/*模倣風で使用していない*/


#endif /* US E_RWOPS */



//MOD_playAudio(music_playing->data.module, stream, len)
//int MOD_playAudio(MOD_MUSIC_MODULE *music, u8 *stream, int len)
void MOD_playAudio(MOD_MUSIC_MODULE *music, u8 *stream, int len)
{
	#if (1==USE_SURROUND)
	if (current_output_channels > 2)
	{
		int small_len = 2 * len / current_output_channels;
		MOD_VIRTUAL_MIXER_WriteBytes((s8 *)stream, small_len);
		/* and extend to len by copying channels */
		u8 *src, *dst;
		src = stream + small_len;
		dst = stream + len;
		int i;
		switch (current_output_format & 0xff)
		{
		case 8:
			for ( i=small_len/2; i; i-- )
			{
				src -= 2;
				dst -= current_output_channels;
				dst[0] = src[0];
				dst[1] = src[1];
				dst[2] = src[0];
				dst[3] = src[1];
				if (current_output_channels == 6)
				{
					dst[4] = src[0];
					dst[5] = src[1];
				}
			}
			break;
		case 16:
			for ( i=small_len/4; i; i-- )
			{
				src -= 4;
				dst -= 2 * current_output_channels;
				dst[0] = src[0];
				dst[1] = src[1];
				dst[2] = src[2];
				dst[3] = src[3];
				dst[4] = src[0];
				dst[5] = src[1];
				dst[6] = src[2];
				dst[7] = src[3];
				if (current_output_channels == 6)
				{
					dst[ 8] = src[0];
					dst[ 9] = src[1];
					dst[10] = src[2];
					dst[11] = src[3];
				}
			}
			break;
		}
	}
	else
	#endif
	{
		MOD_VIRTUAL_MIXER_WriteBytes((s8 *)stream, len);
	}
	#if (1==USE_SWAP_FORMAT)
	if ( music_swap8 )
	{
		u8 *dst;
		int i;
		dst = stream;
		for ( i=len; i; i-- )
		{
			*dst++ ^= 0x80;
		}
	}
	else
	if ( music_swap16 )
	{
		u8 *dst, tmp;
		int i;
		dst = stream;
		for ( i=(len/2); i; i-- )
		{
			tmp = dst[0];
			dst[0] = dst[1];
			dst[1] = tmp;
			dst += 2;
		}
	}
	#endif
//	return (0);
}
