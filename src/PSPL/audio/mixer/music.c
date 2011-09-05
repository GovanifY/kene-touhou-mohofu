
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
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

/* $Id: music.c,v 1.58 2004/11/15 23:34:56 slouken Exp $ */
#ifdef _SDL_MIXER_H
error! "このプログラムは SDL_mixer.h がインクルードされていると、コンパイルできません。"
#endif
#define _SDL_MIXER_H

#include <stdlib.h>
#include <malloc.h>

#include <string.h>
//#include <ctype.h>/*isaln um tou pper*/

//#include "SDL_SDL_endian.h"//#include <SD L/SDL_endian.h>//#include "SD L_endian.h"
//#include "SDL_SDL_audio.h"//#include <SD L/SDL_audio.h>//#include "SD L_audio.h"
//#include "SDL_SDL_timer.h"//#include <SD L/SDL_timer.h>//#include "SD L_timer.h"

/* need new version. */
#include "./../../include/PSPL_audio.h"
#include "000SDL_mixer.h"//#include <SD L/SD L_mixer.h>//#include "SD L_mixer.h"

#ifndef Mix_SetError_bbb
	#define Mix_SetError_bbb(...)
#endif

#ifndef SDL_Delay_bbb
	#define SDL_Delay_bbb(aaa)
#endif

/* The music command hack is UNIX specific */
#ifndef unix
	#undef CMD_MUSIC
#endif

#if defined(CMD_MUSIC)
	#include "./music_cmd.h"
#endif

#if defined(WAV_MUSIC)
	#include "./wavestream.h"
#endif
	// || defined(LIB_MOHOU_MOD_MUSIC) LIB_MOHOU_MOD_MUSIC not supported.
#if defined(MOD_MUSIC)
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

#if defined(MID_MUSIC)
	#if defined(USE_TIMIDITY_MIDI)
		#include "./timidity.h"
	#endif
	#if defined(USE_NATIVE_MIDI)
		#include "./native_midi.h"
	#endif
	#if defined(USE_TIMIDITY_MIDI) && defined(USE_NATIVE_MIDI)
		#define MIDI_ELSE	else
	#else
		#define MIDI_ELSE
	#endif
#endif

#if defined(OGG_MUSIC)
	#include "./music_ogg.h"
#endif

#if defined(MP3_MUSIC)
	#include "./../smpeg/smpeg.h"
#endif

#if defined(MP3_MAD_MUSIC)
	#include "./music_mad.h"
#endif

#if defined(FLAC_MUSIC)
	#include "./music_flac.h"
#endif

#if defined(MP3_MUSIC) || defined(MP3_MAD_MUSIC)
	static PSPL_AUDIO_SPEC used_mixer;
#endif

static Mix_Music * volatile music_playing = NULL;
static unsigned int music_volume = MIX_MAX_VOLUME;

/* USE_PAUSE_MUSIC:0==なし==使わない。1==使う。曲の一時停止、再開機能。 */
#define USE_PAUSE_MUSIC (0)
#if (1==USE_PAUSE_MUSIC)
static int volatile my_music_pause_disable = 0;
#endif /* (1==USE_PAUSE_MUSIC) */

//static int volatile music_stopped = 0;/*[廃止]*/
//static int music_loops = 0;/*(???)[廃止]*/
static char *music_cmd = NULL;

#if (1==USE_SWAP_FORMAT)
static int music_swap8;
static int music_swap16;
#endif

typedef enum
{
	MUS_NONE,
	MUS_CMD,
	MUS_WAV,
	MUS_MOD,
	MUS_MID,
	MUS_OGG,
	MUS_MP3,
	MUS_MP3_MAD,
	MUS_FLAC
} Mix_MusicType;

struct the_Mix_Music_t
{
	Mix_MusicType type;
	union
	{
		#if defined(CMD_MUSIC)
		MusicCMD *cmd;
		#endif
		#if defined(WAV_MUSIC)
		WAVStream *wave;
		#endif
		// || defined(LIB_MOHOU_MOD_MUSIC) LIB_MOHOU_MOD_MUSIC not supported.
		#if defined(MOD_MUSIC)
		UNIMOD *module;
		#endif
	#if defined(MID_MUSIC)
		#if defined(USE_TIMIDITY_MIDI)
		MidiSong *midi;
		#endif
		#if defined(USE_NATIVE_MIDI)
		NativeMidiSong *nativemidi;
		#endif
	#endif
		#if defined(OGG_MUSIC)
		OGG_music *ogg;
		#endif
		#if defined(MP3_MUSIC)
		SMPEG *mp3;
		#endif
		#if defined(MP3_MAD_MUSIC)
		mad_data *mp3_mad;
		#endif
		#if defined(FLAC_MUSIC)
		FLAC_music *flac;
		#endif
	} data;
//	Mix_Fading fading;/*[廃止]フェード*/
//	int fade_step;/*[廃止]フェード*/
//	int fade_steps;/*[廃止]フェード*/
	int error;
};

#if defined(MID_MUSIC)
	#if defined(USE_TIMIDITY_MIDI)
		static int timidity_ok;
		static int samplesize;
	#endif
	#if defined(USE_NATIVE_MIDI)
		static int native_midi_ok;
	#endif
#endif

#if (1==USE_SURROUND)
/* Reference for converting mikmod output to 4/6 channels */
static int current_output_channels;
static u16 current_output_format;
#endif

/* Used to calculate fading steps */
//static int ms_per_step;/*[廃止]*/


/* Set the music volume */
static void music_internal_volume(int volume)
{
	switch (music_playing->type)
	{
	#if defined(CMD_MUSIC)
	case MUS_CMD:
		MusicCMD_SetVolume(volume);
		break;
	#endif
	#if defined(WAV_MUSIC)
	case MUS_WAV:
		WAVStream_SetVolume(volume);
		break;
	#endif
	// || defined(LIB_MOHOU_MOD_MUSIC) LIB_MOHOU_MOD_MUSIC not supported.
	#if defined(MOD_MUSIC)
	case MUS_MOD:
		Player_SetVolume((s16)volume);
		break;
	#endif
	#if defined(MID_MUSIC)
	case MUS_MID:
		#if defined(USE_NATIVE_MIDI)
		if ( native_midi_ok )
		{
			native_midi_setvolume(volume);
		}
		MIDI_ELSE
		#endif
		#if defined(USE_TIMIDITY_MIDI)
		if ( timidity_ok )
		{
			Timidity_SetVolume(volume);
		}
		#endif
		break;
	#endif
	#if defined(OGG_MUSIC)
	case MUS_OGG:
		OGG_setvolume(music_playing->data.ogg, volume);
		break;
	#endif
	#if defined(FLAC_MUSIC)
	case MUS_FLAC:
		FLAC_setvolume(music_playing->data.flac, volume);
		break;
	#endif
	#if defined(MP3_MUSIC)
	case MUS_MP3:
	//	SMPEG_setvolume(music_playing->data.mp3, (int)(((float)volume/(float)MIX_MAX_VOLUME)*100.0));
	//	SMPEG_setvolume(music_playing->data.mp3, (int)((volume>>1)*100.0));
		SMPEG_setvolume(music_playing->data.mp3, (int)(volume) );
		break;
	#endif
	#if defined(MP3_MAD_MUSIC)
	case MUS_MP3_MAD:
		mad_setVolume(music_playing->data.mp3_mad, volume);
		break;
	#endif
		default:
		/* Unknown music type?? */
		break;
	}
}


/* Set the music's initial volume */
static void music_internal_initialize_volume(void)
{
//	if ( music_playing->fading == MIX_FADING_IN )/*[廃止]フェード*/
//	{
//		music_internal_volume(0);
//	}
//	else
	{
		music_internal_volume(music_volume);
	}
}


int Mix_VolumeMusic(int volume)
{
	int prev_volume;

	prev_volume = music_volume;
	if ( volume < 0 )
	{
		return prev_volume;
	}
	if ( volume > SDL_MIX_MAXVOLUME )
	{
		volume = SDL_MIX_MAXVOLUME;
	}
	music_volume = volume;
	PSPL_LockAudio();
	if ( music_playing )
	{
		music_internal_volume(music_volume);
	}
	PSPL_UnlockAudio();
	return (prev_volume);
}


/* Halt playing of music */
static void music_internal_halt(void)
{
	switch (music_playing->type)
	{
	#if defined(CMD_MUSIC)
	case MUS_CMD:
		MusicCMD_Stop(music_playing->data.cmd);
		break;
	#endif
	#if defined(WAV_MUSIC)
	case MUS_WAV:
		WAVStream_Stop();
		break;
	#endif
	// || defined(LIB_MOHOU_MOD_MUSIC) LIB_MOHOU_MOD_MUSIC not supported.
	#if defined(MOD_MUSIC)
	case MUS_MOD:
		Player_Stop();
		break;
	#endif
	#if defined(MID_MUSIC)
	case MUS_MID:
		#if defined(USE_NATIVE_MIDI)
		if ( native_midi_ok )
		{
			native_midi_stop();
		} MIDI_ELSE
		#endif
		#if defined(USE_TIMIDITY_MIDI)
		if ( timidity_ok )
		{
			Timidity_Stop();
		}
		#endif
		break;
	#endif
	#if defined(OGG_MUSIC)
	case MUS_OGG:
		OGG_stop(music_playing->data.ogg);
		break;
	#endif
	#if defined(FLAC_MUSIC)
	case MUS_FLAC:
		FLAC_stop(music_playing->data.flac);
		break;
	#endif
	#if defined(MP3_MUSIC)
	case MUS_MP3:
		SMPEG_stop(music_playing->data.mp3);
		break;
	#endif
	#if defined(MP3_MAD_MUSIC)
	case MUS_MP3_MAD:
		mad_stop(music_playing->data.mp3_mad);
		break;
	#endif
	default:
		/* Unknown music type?? */
		return;
	}
//	music_playing->fading = MIX_NO_FADING;/*[廃止]フェード*/
	music_playing = NULL;
}


int Mix_HaltMusic(void)
{
	PSPL_LockAudio();
	if ( music_playing )
	{
		music_internal_halt();
	}
	PSPL_UnlockAudio();
	return (0);
}


/* Check the status of the music */
static int music_internal_playing(void)
{
	int playing = 1;
	switch (music_playing->type)
	{
	#if defined(CMD_MUSIC)
	case MUS_CMD:
		if (!MusicCMD_Active(music_playing->data.cmd))		{	playing = 0;	}
		break;
	#endif
	#if defined(WAV_MUSIC)
	case MUS_WAV:
		if ( !WAVStream_Active() )							{	playing = 0;	}
		break;
	#endif
	// || defined(LIB_MOHOU_MOD_MUSIC) LIB_MOHOU_MOD_MUSIC not supported.
	#if defined(MOD_MUSIC)
	case MUS_MOD:
		if ( !Player_Active() ) 							{	playing = 0;	}
		break;
	#endif
	#if defined(MID_MUSIC)
	case MUS_MID:
		#if defined(USE_NATIVE_MIDI)
		if ( native_midi_ok )
		{
			if ( !native_midi_active() )					{	playing = 0;	}
		} MIDI_ELSE
		#endif
		#if defined(USE_TIMIDITY_MIDI)
		if ( timidity_ok )
		{
			if ( !Timidity_Active() )						{	playing = 0;	}
		}
		#endif
		break;
	#endif
	#if defined(OGG_MUSIC)
	case MUS_OGG:
		if ( !OGG_playing(music_playing->data.ogg) )		{	playing = 0;	}
		break;
	#endif
	#if defined(FLAC_MUSIC)
	case MUS_FLAC:
		if ( !FLAC_playing(music_playing->data.flac) )		{	playing = 0;	}
		break;
	#endif
	#if defined(MP3_MUSIC)
	case MUS_MP3:
		if ( SMPEG_status(music_playing->data.mp3) != SMPEG_PLAYING )		{	playing = 0;	}
		break;
	#endif
	#if defined(MP3_MAD_MUSIC)
	case MUS_MP3_MAD:
		if (!mad_isPlaying(music_playing->data.mp3_mad))	{	playing = 0;	}
		break;
	#endif
	default:
		{	playing = 0;}
		break;
	}
	return (playing);
}

#if 00/*(r32)模倣風で使用していない*/
int Mix_PlayingMusic(void)
{
	int playing = 0;
	PSPL_LockAudio();
	if ( music_playing )
	{
		playing = music_internal_playing();
	}
	PSPL_UnlockAudio();
	return (playing);
}
#endif/*00*/

/* Set the playing music position */
#if defined(OGG_MUSIC)
extern void OGG_jump_to_time_int(OGG_music *music, /*dou ble*/int time);
#endif
/**/static int music_internal_position_int(/*dou ble*/int position)
{
	int retval = 0;
	switch (music_playing->type)
	{
	// || defined(LIB_MOHOU_MOD_MUSIC) LIB_MOHOU_MOD_MUSIC not supported.
	#if defined(MOD_MUSIC)
	case MUS_MOD:
		Player_SetPosition((u16)position);
		break;
	#endif
	#if defined(OGG_MUSIC)
	case MUS_OGG:
		OGG_jump_to_time_int(music_playing->data.ogg, position);
		break;
	#endif
	#if defined(MP3_MUSIC)
	case MUS_MP3:
		if ( position > 0/*.0*/ )
		{
			SMPEG_skip(music_playing->data.mp3, position);
		}
		else
		{
			SMPEG_rewind(music_playing->data.mp3);
			SMPEG_play(music_playing->data.mp3);
		}
		break;
	#endif
	#if defined(MP3_MAD_MUSIC)
	case MUS_MP3_MAD:
		mad_seek_int(music_playing->data.mp3_mad, position);
		break;
	#endif
	default:
		/* TODO: Implement this for other music backends */
		retval = -1;
		break;
	}
	return (retval);
}
#if 00/*模倣風で使用していない*/
/**/static int Mix_SetMusicPosition_int(/*dou ble*/int position)
{
	int retval;
	PSPL_LockAudio();
	if ( music_playing )
	{
		retval = music_internal_position_int(position);
		if ( retval < 0 )
		{
			Mix_SetError_bbb("Position not implemented for music type");
		}
	}
	else
	{
		Mix_SetError_bbb("Music isn't playing");
		retval = -1;
	}
	PSPL_UnlockAudio();
	return (retval);
}
#endif/*模倣風で使用していない*/

/* Play a music chunk.	Returns 0, or -1 if there was an error.
 */
static int music_internal_play_int(Mix_Music *music, /*dou ble*/int position)
{
	int retval = 0;

	/* Note the music we're playing */
	if ( music_playing )
	{
		music_internal_halt();
	}
	music_playing = music;

	/* Set the initial volume */
	if ( music->type != MUS_MOD )
	{
		music_internal_initialize_volume();
	}

	/* Set up for playback */
	switch (music->type)
	{
	#if defined(CMD_MUSIC)
	case MUS_CMD:
		MusicCMD_Start(music->data.cmd);
		break;
	#endif
	#if defined(WAV_MUSIC)
	case MUS_WAV:
		WAVStream_Start(music->data.wave);
		break;
	#endif
	// || defined(LIB_MOHOU_MOD_MUSIC) LIB_MOHOU_MOD_MUSIC not supported.
	#if defined(MOD_MUSIC)
	case MUS_MOD:
		Player_Start(music->data.module);
		/* Player_SetVolume() does nothing before Player_Start() */
		music_internal_initialize_volume();
		break;
	#endif
	#if defined(MID_MUSIC)
	case MUS_MID:
		#if defined(USE_NATIVE_MIDI)
		if ( native_midi_ok )
		{
			native_midi_start(music->data.nativemidi);
		} MIDI_ELSE
		#endif
		#if defined(USE_TIMIDITY_MIDI)
		if ( timidity_ok )
		{
			Timidity_Start(music->data.midi);
		}
		#endif
		break;
	#endif
	#if defined(OGG_MUSIC)
	case MUS_OGG:
		OGG_play(music->data.ogg);
		break;
	#endif
	#if defined(MP3_MUSIC)
	case MUS_MP3:
		SMPEG_enableaudio(music->data.mp3,1);
		#if (1==USE_MP3_VIDEO)
		SMPEG_enablevideo(music->data.mp3,0);
		#endif
		SMPEG_play(music_playing->data.mp3);
		break;
	#endif
	#if defined(MP3_MAD_MUSIC)
	case MUS_MP3_MAD:
		mad_start(music->data.mp3_mad);
		break;
	#endif
	default:
		Mix_SetError_bbb("Can't play unknown music type");
		retval = -1;
		break;
	}

	/* Set the playback position, note any errors if an offset is used */
	if ( retval == 0 )
	{
		if ( position > 0/*.0*/ )
		{
			if ( music_internal_position_int(position) < 0 )
			{
				Mix_SetError_bbb("Position not implemented for music type");
				retval = -1;
			}
		}
		else
		{
			music_internal_position_int(0/*.0*/);
		}
	}

	/* If the setup failed, we're not playing any music anymore */
	if ( retval < 0 )
	{
		music_playing = NULL;
	}
	return (retval);
}


#if 0/*[廃止]曲再生終了時のコールバック*/
/* Support for hooking when the music has finished */
static void (*music_finished_hook)(void) = NULL;
void Mix_HookMusicFinished(void (*music_finished)(void))
{
	PSPL_LockAudio();
	music_finished_hook = music_finished;
	PSPL_UnlockAudio();
}
#endif/*[廃止]曲再生終了時のコールバック*/



#if defined(MOD_MUSIC)
extern void MOD_playAudio(MOD_MUSIC_MODULE *music, u8 *stream, int len);
#endif
/* Mixing function */
void custom_music_mixer(void *udata, u8 *stream, int len)
{
	#if (1==USE_PAUSE_MUSIC)
	if ((0==my_music_pause_disable))
	#endif /* (1==USE_PAUSE_MUSIC) */
	{
		if (music_playing)/* music callbackがある場合(曲がロードされている場合のみ) */
		{
			/* Handle fading */
			#if 00/*[廃止]フェード*/
			if ( music_playing->fading != MIX_NO_FADING )
			{
				if ( music_playing->fade_step++ < music_playing->fade_steps )
				{
					int volume;
					int fade_step = music_playing->fade_step;
					int fade_steps = music_playing->fade_steps;
					if ( music_playing->fading == MIX_FADING_OUT )
					{
						volume = (music_volume * (fade_steps-fade_step)) / fade_steps;
					}
					else	/* Fading in */
					{
						volume = (music_volume * fade_step) / fade_steps;
					}
					music_internal_volume(volume);
				}
				else
				{
					if ( music_playing->fading == MIX_FADING_OUT )
					{
						music_internal_halt();/*おしまい?*/
					//	if ( music_finished_hook )	{	music_finished_hook();	}	/*[廃止]曲再生終了時のコールバック*/
						return;
					}
					music_playing->fading = MIX_NO_FADING;
				}
			}
			#endif
			/* Restart music if it has to loop */
			if ( !music_internal_playing() )
			{
				/* Restart music if it has to loop at a high level */
			//	if ( -1)/*???*/ 	//music_loops && --music_loops )/*(???)[廃止]*/
				{
					/* 永遠に曲を繰り返す。 */
				//	Mix_Fading current_fade 	= music_playing->fading;/*[廃止]フェード*/
					music_internal_play_int(music_playing, 0/*.0*/);
				//	music_playing->fading		= current_fade;/*[廃止]フェード*/
				}
			//	else		/* 曲の繰り返し回数を指定する。 */
			//	{
			//		music_internal_halt();/*おしまい?*/
			//		//	if ( music_finished_hook )	{	music_finished_hook();	}	/*[廃止]曲再生終了時のコールバック*/
			//		return;
			//	}
			}
			switch (music_playing->type)
			{
			#if defined(CMD_MUSIC)
			case MUS_CMD:
				/* The playing is done externally */
				break;
			#endif
			#if defined(WAV_MUSIC)
			case MUS_WAV:
				WAVStream_PlaySome(stream, len);
				break;
			#endif
			// || defined(LIB_MOHOU_MOD_MUSIC) LIB_MOHOU_MOD_MUSIC not supported.
			#if defined(MOD_MUSIC)
			case MUS_MOD:
				MOD_playAudio(NULL/*music_playing->data.module*/, stream, len);
				break;
			#endif
			#if defined(MID_MUSIC)
			#if defined(USE_TIMIDITY_MIDI)
			case MUS_MID:
				if ( timidity_ok )
				{
					int samples = len / samplesize;
					Timidity_PlaySome(stream, samples);
				}
				break;
			#endif
			#endif
			#if defined(OGG_MUSIC)
			case MUS_OGG:
				OGG_playAudio(music_playing->data.ogg, stream, len);
				break;
			#endif
			#if defined(MP3_MUSIC)
			case MUS_MP3:
				SMPEG_playAudio(music_playing->data.mp3, stream, len);
				break;
			#endif
			#if defined(MP3_MAD_MUSIC)
			case MUS_MP3_MAD:
					mad_getSamples(music_playing->data.mp3_mad, stream, len);
					break;
			#endif
			default:
				/* Unknown music type?? */
				break;
			}
		}
	}
}

/* Initialize the music players with a certain desired audio format */
int open_music(PSPL_AUDIO_SPEC *mixer)
{
	int music_error;
	music_error = 0;
	#if defined(WAV_MUSIC)
	if ( WAVStream_Init(mixer) < 0 )
	{
		music_error++;
	}
	#endif
	// || defined(LIB_MOHOU_MOD_MUSIC) LIB_MOHOU_MOD_MUSIC not supported.
	#if defined(MOD_MUSIC)
	/* Set the MusicMod music format */
	#if (1==USE_SWAP_FORMAT)
	music_swap8 	= 0;
	music_swap16	= 0;
	#endif
	#if (1==USE_SWAP_FORMAT)
	switch (mixer->format)
	{
	case AUDIO_U8:
	case AUDIO_S8:
		if ( mixer->format == AUDIO_S8 )
		{
			music_swap8 = 1;
		}
//		mm_d_mode = 0;
		break;
	case AUDIO_S16LSB:
	case AUDIO_S16MSB:
		/* See if we need to correct MusicMod mixing */
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		if ( mixer->format == AUDIO_S16MSB )
		#else
		if ( mixer->format == AUDIO_S16LSB )
		#endif
		{
			music_swap16 = 1;
		}
//		mm_d_mode = DMODE_16BITS;
		break;

	default:
		Mix_SetError_bbb("Unknown hardware audio format");
		music_error++;
	}
	#endif
	#if (1==USE_SURROUND)
	current_output_channels = mixer->channels;
	current_output_format	= mixer->format;
	#endif
	#if (000)
	if ( mixer->channels > 1 )
	{
		#if (1==USE_SURROUND)
			#define MAX_OUTPUT_CHANNELS 6
		#else
			#define MAX_OUTPUT_CHANNELS 2
		#endif
		if ( mixer->channels > MAX_OUTPUT_CHANNELS )
		{
			Mix_SetError_bbb("Hardware uses more channels than mixer");
			music_error++;
		}
//		mm_d_mode |= DMODE_STEREO;
	}
	#endif
//	m d_mixfreq 	= 44100/*mix er->freq*/;
//	m d_device		= 0;
//	m d_volume		= 128;/*96*/
//	m d_musicvolume = 128;
//	m d_sndfxvolume = 128;
//	m d_pansep		= 128;
	#if (1==USE_MIK_MOD_REVERB)
	mod_config_reverb		= 0;
	#endif /* (1==USE_MIK_MOD_REVERB) */

	// LIB_MOHOU_MOD_MUSIC not supported.
	#ifdef LIB_MOHOU_MOD_MUSIC
//	mm_d_mode |= DMODE_HQMIXER|DMODE_SOFT_MUSIC|DMODE_SURROUND;
	#endif
	#ifdef LIB_MOHOU_MOD_MUSIC
	if (!MusicMod_InfoDriver())
	#endif
//	MusicMod_RegisterAllDrivers();/*(r35-無し)*/
	#ifdef LIB_MOHOU_MOD_MUSIC
	if (!MusicMod_InfoLoader())
	#endif
	MusicMod_RegisterAllLoaders();
	if ( MusicMod_Init() )
	{
		Mix_SetError_bbb("%s", MusicMod_strerror(MusicMod_errno));
		music_error++;
	}
#endif
#if defined(MID_MUSIC)
#if defined(USE_TIMIDITY_MIDI)
	samplesize = mixer->size / mixer->samples;
	if ( Timidity_Init(44100/*mix er->freq*/, mixer->format,
				mixer->channels, mixer->samples) == 0 )
	{
		timidity_ok = 1;
	}
	else
	{
		timidity_ok = 0;
	}
#endif
#if defined(USE_NATIVE_MIDI)
#if defined(USE_TIMIDITY_MIDI)
	native_midi_ok = !timidity_ok;
	if ( native_midi_ok )
#endif
		native_midi_ok = native_midi_detect();
#endif
#endif
#if defined(OGG_MUSIC)
	if ( OGG_init(mixer) < 0 )
	{
		music_error++;
	}
#endif
#if defined(MP3_MUSIC) || defined(MP3_MAD_MUSIC)
	/* Keep a copy of the mixer */
	used_mixer = *mixer;
#endif
	music_playing = NULL;
//	music_stopped = 0;/*[廃止]*/
	if ( music_error )
	{
		return (-1);
	}
	Mix_VolumeMusic(SDL_MIX_MAXVOLUME);

	/* Calculate the number of ms for each callback */
//	ms_per_step = (int) (((float)mixer->samples * 1000.0) / mix er->freq);/*[廃止]*/

	return (0);
}

#if (000)
#if (1)
/* Portable case-insensitive string compare function */
/* src/PSPL/audio/mixer/music.c */
/* src/PSPL/image/IMG.c */
/*
	ファイル名しか使わないので、簡易版でも無問題。
	s-jisはisaln um()やtou pper()使ってる時点で端から非対応。
	下記の文字は変換されちゃうのでファイル名に使わない事。
	`{|}~ 変換前
	@[\]^ 変換後
*/
	/*static*/ int PSPL_string_equals(const char *str1, const char *str2)
	{
		#if 0
		while ( (*str1) && (*str2) )
		{
			if ( tou pper((u8)*str1) !=
				 tou pper((u8)*str2) )
			{	break;}
			str1++;
			str2++;
		}
		return ((!*str1) && (!*str2));
		#else
		/*(簡易版。もうちょっと仕様を削ればもっと簡略化できる。例えば引数1にNULLが来ないとか)*/
		{
			u8 s1;
			u8 s2;
		my_loop:
			/* 0110 6, 0100 4, 1101 1111 0xdf	*/
			s1 = (*str1) & (0xdf);/*(正確でないテキトー版大文字化)*/
			s2 = (*str2) & (0xdf);/*(正確でないテキトー版大文字化)*/
			if (0==s1)
			{
				if (0==s2)	{	goto my_quit;}
				else		{	goto my_false;}/*(長さが違うのでng!)*/
			}
			if (0==s2)
			{
				if (0==s1)	{	goto my_quit;}
				else		{	goto my_false;}/*(長さが違うのでng!)*/
			}
			if (s1!=s2) goto my_false;
			goto my_loop;
		}
		my_quit:
			return (1/*TRUE*/);/*ok!==*/
		my_false:
			return (0/*FALSE*/);/*NG!,*/
		#endif
	}


#else
extern int PSPL_string_equals(const char *str1, const char *str2);
#endif
#endif /*(000)*/

#if 1
/* Portable case-insensitive string compare function */
//#include <ctype.h>/*isaln um toupper*/
int PSPL_MIXER_string_equals(const char *str1, const char *str2)
{
//	while ( *str1 && *str2 )
	{
	loop_start:
		if (0==(*str1)) 	goto loop_break;
		if (0==(*str2)) 	goto loop_break;
		//
	//	if ( toupper((u8)*str1) !=
	//		 toupper((u8)*str2) )
		if (  ((*str1) & (0xdf)) != 	/*(正確でないテキトー版大文字化)*/
			  ((*str2) & (0xdf)) )		/*(正確でないテキトー版大文字化)*/
		{
			goto loop_break;
		}
		str1++;
		str2++;
		goto loop_start;
	loop_break:
		;
	}
	#if 0
//	return ( (!(*str1)) && (!(*str2)) );
	return ( (0==(*str1)) && (0==(*str2)) );
	#else
	if (0!=(*str1)) 	goto my_false;
	if (0!=(*str2)) 	goto my_false;
//	my_quit:
		return (1/*TRUE*/);/*ok!==*/
	my_false:
		return (0/*FALSE*/);/*NG!,*/
	#endif
}
#endif

/* Load a music file */
extern int tiny_strcmp(/*const*/ char *aaa, const char *bbb);
Mix_Music *Mix_LoadMUS(const char *file)
{
	u8 magic[5];
	u8 moremagic[9];
	Mix_Music *music;

	/* Figure out what kind of file this is */
	FILE *fp;
	fp = fopen(file, "rb");
	if ( (fp == NULL) || !fread(magic, 4, 1, fp) )
	{
		if ( fp != NULL )
		{
			fclose(fp);
		}
		Mix_SetError_bbb("Couldn't read from '%s'", file);
		return (NULL);
	}
	if (!fread(moremagic, 8, 1, fp))
	{
		Mix_SetError_bbb("Couldn't read from '%s'", file);
		return (NULL);
	}
	magic[4] = '\0';
	moremagic[8] = '\0';
	fclose(fp);

	/* Figure out the file extension, so we can determine the type */
	char *ext;
	ext = strrchr(file, '.');
	if ( ext ) ext++; /* skip the dot in the extension */

	/* Allocate memory for the music structure */
	music = (Mix_Music *)malloc(sizeof(Mix_Music));
	if ( music == NULL )
	{
		Mix_SetError_bbb("Out of memory");
		return (NULL);
	}
	music->error = 0;

	#if defined(CMD_MUSIC)
	if ( music_cmd )
	{
		music->type = MUS_CMD;
		music->data.cmd = MusicCMD_LoadSong(music_cmd, file);
		if ( music->data.cmd == NULL )
		{
			music->error = 1;
		}
	}
	else
	#endif
	#if defined(WAV_MUSIC)
	/* WAVE files have the magic four bytes "RIFF"
	   AIFF files have the magic 12 bytes "FORM" XXXX "AIFF"
	 */
	if ( (ext && PSPL_MIXER_string_equals(ext, "wav")) ||
		 (	(tiny_strcmp((char *)magic, "RIFF") == 0) &&
			(tiny_strcmp((char *)(moremagic+4), "WAVE") == 0)) ||
			(tiny_strcmp((char *)magic, "FORM") == 0) )
	{
		music->type = MUS_WAV;
		music->data.wave = WAVStream_LoadSong(file, (char *)magic);
		if ( music->data.wave == NULL )
		{
			Mix_SetError_bbb("Unable to load WAV file");
			music->error = 1;
		}
	}
	else
	#endif
	#if defined(MID_MUSIC)
	/* MIDI files have the magic four bytes "MThd" */
	if ( (ext && PSPL_MIXER_string_equals(ext, "mid")) ||
			#if 0
		 (ext && PSPL_MIXER_string_equals(ext, "midi")) ||
			#endif
		   tiny_strcmp((char *)magic, "MThd") == 0	||
		 ( tiny_strcmp((char *)magic, "RIFF") == 0	&&
		   tiny_strcmp((char *)(moremagic+4), "RMID") == 0 ) )
	{
		music->type = MUS_MID;
		#if defined(USE_NATIVE_MIDI)
		if ( native_midi_ok )
		{
			music->data.nativemidi = native_midi_loadsong((char *)file);
			if ( music->data.nativemidi == NULL )
			{
				Mix_SetError_bbb("%s", native_midi_error());
				music->error = 1;
			}
		}
		MIDI_ELSE
		#endif
		#if defined(USE_TIMIDITY_MIDI)
		if ( timidity_ok )
		{
			music->data.midi = Timidity_LoadSong((char *)file);
			if ( music->data.midi == NULL )
			{
				Mix_SetError_bbb("%s", Timidity_Error());
				music->error = 1;
			}
		}
		else
		{
			Mix_SetError_bbb("%s", Timidity_Error());
			music->error = 1;
		}
		#endif
	}
	else
	#endif
	#if defined(OGG_MUSIC)
	/* Ogg Vorbis files have the magic four bytes "OggS" */
	if (	(ext && PSPL_MIXER_string_equals(ext, "ogg")) ||/*"模倣風はこれしかない"*/
		 tiny_strcmp((char *)magic, "OggS") == 0 )
	{
		music->type = MUS_OGG;
		music->data.ogg = OGG_new(file);
		if ( music->data.ogg == NULL )
		{
			music->error = 1;
		}
	}
	else
	#endif
	#if defined(MP3_MUSIC)
	if (	(ext && PSPL_MIXER_string_equals(ext, "mp3")) ||/*"模倣風はこれしかない"*/
			#if 0
			(ext && PSPL_MIXER_string_equals(ext, "mpg")) ||
			(ext && PSPL_MIXER_string_equals(ext, "mpeg")) ||
			#endif
		 ((magic[0] == 0xff) && ((magic[1] & 0xF0) == 0xF0))
	)
	{
		SMPEG_Info info;
		music->type = MUS_MP3;
		music->data.mp3 = SMPEG_new(file, &info, 0);
		if (!info.has_audio)
		{
			Mix_SetError_bbb("MPEG file does not have any audio stream.");
			music->error = 1;
		}
		else
		{
			SMPEG_actualSpec(music->data.mp3, &used_mixer);
		}
	}
	else
	#endif
	#if defined(MP3_MAD_MUSIC)
	if (	(ext && PSPL_MIXER_string_equals(ext, "mp3")) ||/*"模倣風はこれしかない"*/
			#if 0
			(ext && PSPL_MIXER_string_equals(ext, "mpg")) ||
			(ext && PSPL_MIXER_string_equals(ext, "MPEG")) ||
			(ext && PSPL_MIXER_string_equals(ext, "mad")) ||
			#endif
		 ((magic[0] == 0xff) && ((magic[1] & 0xF0) == 0xF0))
	//	 || (st rncmp((char *)magic, "ID3", 3) == 0)
	)
	{
		music->type = MUS_MP3_MAD;
		music->data.mp3_mad = mad_openFile(file, &used_mixer);
		if (music->data.mp3_mad == 0)
		{
			Mix_SetError_bbb("Could not initialize MPEG stream.");
			music->error = 1;
		}
	} else
	#endif
	// || defined(LIB_MOHOU_MOD_MUSIC) LIB_MOHOU_MOD_MUSIC not supported.
	#if defined(MOD_MUSIC)
	if ( 1 )
	{
		music->type = MUS_MOD;
		music->data.module = MusicMod_LoadSong((char *)file, 64);
		if ( music->data.module == NULL )
		{
			Mix_SetError_bbb("%s", MusicMod_strerror(MusicMod_errno));
			music->error = 1;
		}
		else
		{
			/* Stop implicit looping, fade out and other flags. */
			music->data.module->extspd	= 1;
			music->data.module->panflag = 1;
			music->data.module->wrap	= 0;
			music->data.module->loop	= 0;
			#if 0 /* Don't set fade out by default - unfortunately there's no real way
					 to query the status of the song or set trigger actions.  Hum. */
			music->data.module->fadeout = 1;
			#endif
		}
	}
	else
	#endif
	{
		Mix_SetError_bbb("Unrecognized music format");
		music->error = 1;
	}
	if ( music->error )
	{
		free(music);
		music = NULL;
	}
	return (music);
}

/* Free a music chunk previously loaded */
void Mix_FreeMusic(Mix_Music *music)
{
	if ( music )
	{
		/* Stop the music if it's currently playing */
		PSPL_LockAudio();
		if ( music == music_playing )
		{
			#if 00/*[廃止]フェード*/
			/* Wait for any fade out to finish */
		//	while ( music->fading == MIX_FADING_OUT )/*[廃止]フェード*/
		//	{
		//		PSPL_UnlockAudio();
		//		SDL_Delay_bbb(100);
		//		PSPL_LockAudio();
		//	}
			#endif
			if ( music == music_playing )
			{
				music_internal_halt();
			}
		}
		PSPL_UnlockAudio();
		switch (music->type)
		{
		#if defined(CMD_MUSIC)
		case MUS_CMD:
			MusicCMD_FreeSong(music->data.cmd);
			break;
		#endif
		#if defined(WAV_MUSIC)
		case MUS_WAV:
			WAVStream_FreeSong(music->data.wave);
			break;
		#endif
		// || defined(LIB_MOHOU_MOD_MUSIC) LIB_MOHOU_MOD_MUSIC not supported.
		#if defined(MOD_MUSIC)
		case MUS_MOD:
			MusicMod_FreeSong(music->data.module);
			break;
		#endif
		#if defined(MID_MUSIC)
		case MUS_MID:
			#if defined(USE_NATIVE_MIDI)
			if ( native_midi_ok )
			{
				native_midi_freesong(music->data.nativemidi);
			}
			MIDI_ELSE
			#endif
			#if defined(USE_TIMIDITY_MIDI)
			if ( timidity_ok )
			{
				Timidity_FreeSong(music->data.midi);
			}
			#endif
			break;
		#endif
		#if defined(OGG_MUSIC)
		case MUS_OGG:
			OGG_delete(music->data.ogg);
			break;
		#endif
		#if defined(FLAC_MUSIC)
		case MUS_FLAC:
			FLAC_delete(music->data.flac);
			break;
		#endif
		#if defined(MP3_MUSIC)
		case MUS_MP3:
			SMPEG_delete(music->data.mp3);
			break;
		#endif
		#if defined(MP3_MAD_MUSIC)
		case MUS_MP3_MAD:
			mad_closeFile(music->data.mp3_mad);
			break;
		#endif
		default:
			/* Unknown music type?? */
			break;
		}
		free(music);
	}
}

/* Find out the music format of a mixer music, or the currently playing
   music, if 'music' is NULL.
*/
Mix_MusicType Mix_GetMusicType(const Mix_Music *music)
{
	Mix_MusicType type = MUS_NONE;
	if ( music )
	{
		type = music->type;
	}
	else
	{
		PSPL_LockAudio();
		if ( music_playing )
		{
			type = music_playing->type;
		}
		PSPL_UnlockAudio();
	}
	return (type);
}

#if 1/* 仕様変更(BGM即応する要望があったので改造中) */
int sdl_mixer_play_music(Mix_Music *music)
//	/**/static int Mix_FadeInMusicPos_int(Mix_Music *music, int loops, int ms, /*dou ble*/int position)
{
	int retval;
	/* Don't play null pointers :-) */
	if ( music == NULL )
	{
		Mix_SetError_bbb("music parameter was NULL");
		return (-1);
	}
	/* Setup the data */
	#if 00/*[廃止]フェード*/
//	if ( 0/*ms*/ )		{	music->fading = MIX_FADING_IN;	}
//	else				{	music->fading = MIX_NO_FADING;	}
//	music->fade_step = 0;	/*[廃止]フェード*/
//	music->fade_steps = (0);//(ms / ms_per_step);	/*[廃止]フェード*/
	#endif
	/* Play the puppy */
	PSPL_LockAudio();
	#if 00/*[廃止]フェード*/
	/* If the current music is fading out, wait for the fade to complete */
	while ( music_playing && (music_playing->fading == MIX_FADING_OUT) )
	{
		PSPL_UnlockAudio();
		SDL_Delay_bbb(100);
		PSPL_LockAudio();
	}
	#endif
	#if (1==USE_PAUSE_MUSIC)
	my_music_pause_disable = 0;
	#endif /* (1==USE_PAUSE_MUSIC) */
//	music_loops = -1;//loops;/*(???)[廃止]*/
	retval = music_internal_play_int(music, 0/*position*/);
	PSPL_UnlockAudio();
	return (retval);
}
#endif
#if 00/*模倣風で使用していない*/
int Mix_FadeInMusic(Mix_Music *music, int loops, int ms)
{
	return Mix_FadeInMusicPos_int(music, loops, ms, 0/*.0*/);
}
#endif/*模倣風で使用していない*/

#if 00/*模倣風で使用していない*/
int aaa_Mix_PlayMusic(Mix_Music *music, int loops)
{
	return Mix_FadeInMusicPos_int(music, loops, 0, 0/*.0*/);
}
#else
	#if 0
int sdl_mixer_play_music(Mix_Music *music)
{
	return Mix_FadeInMusicPos_int(music, -1, 0, 0/*.0*/);
}
	#endif
#endif/*模倣風で使用していない*/





#if 00/*模倣風で使用していない*/
/* Progressively stop the music */
int Mix_FadeOutMusic(int ms)
{
	int retval = 0;
	PSPL_LockAudio();
	if ( music_playing && (music_playing->fading == MIX_NO_FADING) )
	{
		music_playing->fading = MIX_FADING_OUT;
		music_playing->fade_step = 0;
		music_playing->fade_steps = /*(0);*/(ms / ms_per_step);
		retval = 1;
	}
	PSPL_UnlockAudio();
	return (retval);
}
#endif/*模倣風で使用していない*/

#if 00/*模倣風で使用していない*/
Mix_Fading Mix_FadingMusic(void)
{
	Mix_Fading fading = MIX_NO_FADING;
	PSPL_LockAudio();
	if ( music_playing )
	{
		fading = music_playing->fading;
	}
	PSPL_UnlockAudio();
	return (fading);
}
#endif/*模倣風で使用していない*/

#if 00/*模倣風で使用していない*/
/* Pause/Resume the music stream */
	#if (1==USE_PAUSE_MUSIC)
void Mix_PauseMusic(void)
{
	my_music_pause_disable = 1;
}

void Mix_ResumeMusic(void)
{
	my_music_pause_disable = 0;
}

int Mix_PausedMusic(void)
{
	return (0 != my_music_pause_disable);
}
	#endif /* (1==USE_PAUSE_MUSIC) */

void Mix_RewindMusic(void)
{
	Mix_SetMusicPosition_int(0/*.0*/);
}
#endif/*模倣風で使用していない*/


/* Set the external music playback command */
int Mix_SetMusicCMD(const char *command)
{
	Mix_HaltMusic();
	if ( music_cmd )
	{
		free(music_cmd);
		music_cmd = NULL;
	}
	if ( command )
	{
		music_cmd = (char *)malloc(strlen(command)+1);
		if ( music_cmd == NULL )
		{
			return (-1);
		}
		strcpy(music_cmd, command);
	}
	return (0);
}

#if 00/*模倣風で使用していない*/
	// LIB_MOHOU_MOD_MUSIC not supported.
#ifdef LIB_MOHOU_MOD_MUSIC
static int _pl_synchro_value;
void Player_SetSynchroValue(int i)
{
	error(0/*ERR_FATAL*/, (char*)"SDL_mixer: Player_SetSynchroValue is not supported.\n");
	_pl_synchro_value = i;
}

int Player_GetSynchroValue(void)
{
	error(0/*ERR_FATAL*/, (char*)"SDL_mixer: Player_GetSynchroValue is not supported.\n");
	return (_pl_synchro_value);
}
#endif
#endif/*模倣風で使用していない*/

#if 00/*模倣風で使用していない*/
int Mix_SetSynchroValue(int i)
{
	if ( music_playing && (!music_stopped) )
	{
		switch (music_playing->type)
		{
		#if defined(MOD_MUSIC) || defined(LIB_MOHOU_MOD_MUSIC)
		case MUS_MOD:
			if ( ! Player_Active() )
			{
				return (-1);
			}
			Player_SetSynchroValue(i);
			return (0);
			break;
		#endif
		default:
			return (-1);
			break;
		}
		return (-1);
	}
	return (-1);
}
#endif/*模倣風で使用していない*/

#if 00/*模倣風で使用していない*/
int Mix_GetSynchroValue(void)
{
	if ( music_playing && (!music_stopped) )
	{
		switch (music_playing->type)
		{
		#if defined(MOD_MUSIC) || defined(LIB_MOHOU_MOD_MUSIC)
		case MUS_MOD:
			if ( ! Player_Active() )
			{
				return (-1);
			}
			return Player_GetSynchroValue();
			break;
		#endif
		default:
			return (-1);
			break;
		}
		return (-1);
	}
	return (-1);
}
#endif/*模倣風で使用していない*/

/* Uninitialize the music players */
void close_music(void)
{
	Mix_HaltMusic();
	#if defined(CMD_MUSIC)
	Mix_SetMusicCMD(NULL);
	#endif
#if defined(MOD_MUSIC) || defined(LIB_MOHOU_MOD_MUSIC)
	MusicMod_Exit();
	#ifndef LIB_MOHOU_MOD_MUSIC
	MusicMod_UnregisterAllLoaders();
//	MusicMod_UnregisterAllDrivers();/*(r35-無し)*/
	#endif
#endif
}

#if 0 //def US E_RWOPS

#if 0/*(???)*/
Mix_Music *Mix_LoadMUS_RW(SDL_RWops *rw)
{
	u8 magic[5];	/* Apparently there is no way to check if the file is really a MOD, */
	/* or there are too many formats supported by MusicMod or MusicMod does */
	/* this check by itself. If someone implements other formats (e.g. MP3) */
	/* the check can be uncommented */
	Mix_Music *music;
	int start;

	/* Figure out what kind of file this is */
	start = SDL_RWtell(rw);
	if (SDL_RWread(rw, magic, 1, 4) != 4)
	{
		Mix_SetError_bbb("Couldn't read from RWops");
		return (NULL);
	}
	SDL_RWseek(rw, start, SEEK_SET);
	magic[4] = '\0';

	/* Allocate memory for the music structure */
	music = (Mix_Music *)malloc(sizeof(Mix_Music));
	if (music==NULL )
	{
		Mix_SetError_bbb("Out of memory");
		return (NULL);
	}
	music->error = 0;

	#if defined(OGG_MUSIC)
	/* Ogg Vorbis files have the magic four bytes "OggS" */
	if ( tiny_strcmp((char *)magic, "OggS") == 0 )
	{
		music->type = MUS_OGG;
		music->data.ogg = OGG_new_RW(rw);
		if ( music->data.ogg == NULL )
		{
			music->error = 1;
		}
	}
	else
	#endif
	#if 0/* 模倣風では非対応 ma d_openFileRW(); */
//	#if defined(MP3_MAD_MUSIC)
//	if ( ( magic[0] == 0xff && (magic[1] & 0xF0) == 0xF0) || ( st rncmp((char *)magic, "ID3", 3) == 0 ) )
//	{
//		music->type = MUS_MP3_MAD;
//		music->data.mp3_mad = ma d_openFileRW(rw, &used_mixer);
//		if (music->data.mp3_mad == 0)
//		{
//			Mix_SetError("Could not initialize MPEG stream.");
//			music->error = 1;
//		}
//	}
//	else
//	#endif
	#endif /* (0) */
	// || defined(LIB_MOHOU_MOD_MUSIC) LIB_MOHOU_MOD_MUSIC not supported.
	#if defined(MOD_MUSIC)
	if (1)
	{
		music->type = MUS_MOD;
		music->data.module = MusicMod_LoadSongRW(rw, 64);
		if (music->data.module==NULL)
		{
			Mix_SetError_bbb("%s",MusicMod_strerror(MusicMod_errno));
			music->error = 1;
		}
	}
	else
	#endif
	{
		Mix_SetError_bbb("Unrecognized music format");
		music->error = 1;
	}
	if (music->error)
	{
		free(music);
		music = NULL;
	}
	return (music);
}
#endif /*0*/

#endif /* US E_RWOPS */
