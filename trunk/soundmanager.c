/*
 * $Id: soundmanager.c,v 1.4 2003/02/09 07:34:16 kenta Exp $
 *
 * Copyright 2002 Kenta Cho. All rights reserved.
 */

/**
 * BGM/SE manager(using SDL_mixer).
 *
 * @version $Revision: 1.4 $
 */
#include <SDL/SDL.h>/*#include "SDL.h"*/
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <pspdebug.h>

#include <SDL/SDL_mixer.h>/*#include "SDL_mixer.h"*/
#include "soundmanager.h"
#include "support.h"

#define USE_VBL 0
/*
#define MUSIC_NUM 2
static char *musicFileName[MUSIC_NUM] =
{
	"stg0", "stg1",
};
static Mix_Music *BGM_track[MUSIC_NUM];
*/

/* --- BGM の管理最大数 (最大読み込みファイル数) */
//#define MAX_MUSIC_FILES 8/*32*/

/* --- SE の管理最大数 (最大読み込みファイル数)MAX_VOICE_FILES=>USE_SE_FILES  */
#define MAX_VOICE_FILES 16/*64*/

/* ----- BGM トラック */
static Mix_Music *BGM_track;


/* ----- BGM の数(読み込みファイル数) */
#define USE_BGM_FILES 7 /**/

/* ----- SE の数(読み込みファイル数) */
#define USE_SE_FILES 15 /*いくつか追加*/

/* ----- SE トラック */
static Mix_Chunk *SE_track[USE_SE_FILES];

//static int chunkFlag[USE_SE_FILES];

/* ----- サウンド有効フラグ */
static int useAudio = 0;


#define NOT_USE_TRACK (-1)/*未使用フラグ*/

#define MAX_VOICE_TRACK 8/*仮想トラック数*/

#if (1==USE_VBL)
static int res_se[MAX_VOICE_TRACK];
#endif

/* ---------------------------------------- */
/* --- BGM のリクエスト 					*/
/* ---------------------------------------- */
// Play/Stop the music/chunk.


//void pauseMusic(void)
//{
//	if ( !useAudio ) return;
//	Mix_PauseMusic();
//}
//
//void resumeMusic(void)
//{
//	if ( !useAudio ) return;
//	Mix_ResumeMusic();
//}
//
//void fadeMusic(void)
//{
//	if ( !useAudio ) return;
//	Mix_FadeOutMusic(1280);
//}


//void stopMusic(void)
//{
//	if ( !useAudio ) return;
//	if ( BGM_track != NULL )
//	{
//		//if ( Mix_PlayingMusic() )
//		{
//			Mix_HaltMusic();
//			Mix_FreeMusic(BGM_track);
//			BGM_track = NULL;
//		}
//	}
//}

//void playMusic(char *play_music_file_name)
void play_music(int num)
{
	if ( !useAudio ) return;
	/* --- 範囲チェック */
static const char *music_file_name[USE_BGM_FILES] =
{
	"intro",	/*	0 */
	"stage1",	/*	1 */
	"stage2",	/*	2 */
	"stage3",	/*	3 */
	"stage4",	/*	4 */
	"stage5",	/*	5 */
	"stage6",	/*	6 */
//	"stage7",	/*	7 */
//	"menu2",	/*	8 */
//	"boss1",	/*	9 */
//	"boss2",	/* 10 */
//	"boss3",	/* 11 */
//	"boss4",	/* 12 */
//	"boss5",	/* 13 */
//	"boss6",	/* 14 */
//	"boss7", 	/* 15 */
};		//いろいろ追加
	/* ----- それまでの演奏停止 */
	//Mix_FreeMusic(BGM_track); 	//これを入れないと途中で音楽が鳴らなくなるっぽい->仕様ではなかった？
	if ( BGM_track != NULL )
	{
		//if ( Mix_PlayingMusic() )
		{	Mix_HaltMusic();}
		Mix_FreeMusic(BGM_track);
		BGM_track = NULL;
	}
	/* ----- BGM 演奏開始 */
	char name[64/*72*/];
	{
		const char *name_extention[] =
		{
			NULL, ".mod", ".s3m", ".xm", ".it", ".mp3", ".wav", ".ogg",
		};
		int aaa;
		aaa = 8;
		do
		{
			aaa--;
			if (0 > aaa)
			{
				pspDebugScreenPrintf("Couldn't load: %s\n", name);
				sceKernelDelayThread(5000000);	//確認用待ち時間
				//useAudio = 0;
				return;
			}
			strcpy(name, moddir);
			strcat(name, "/sounds/");
			strcat(name, music_file_name[num]);
			strcat(name, (char *)&name_extention[aaa][0] );
			BGM_track = Mix_LoadMUS(name);
		}
		while ( NULL == BGM_track );
	}
	Mix_PlayMusic(BGM_track, -1);
}


/* ---------------------------------------- */
/* --- サウンドの解放						*/
/* ---------------------------------------- */

void closeSound(void)
{
	int i;
	if ( !useAudio ) return;
	if ( Mix_PlayingMusic() )
	{
		Mix_HaltMusic();
	}
/*
	for ( i=0 ; i<MUSIC_NUM ; i++ )
	{
		if ( BGM_track[i] )
		{
			Mix_FreeMusic(BGM_track[i]);
		}
	}
*/
	for ( i=0 ; i<USE_SE_FILES ; i++ )
	{
		if ( SE_track[i] )
		{
			Mix_FreeChunk(SE_track[i]);
		}
	}
	Mix_CloseAudio();
}

/* ---------------------------------------- */
/* --- SE のリクエスト						*/
/* ---------------------------------------- */
#if 1
/* voice を予約。トラックは適当に決める。  */
void playChunk(int req)
{
	if ( !useAudio ) return;
	#if (0==USE_VBL)
	//チャンネルが8までしか用意されていないので8以降はチャンネル自由
	if (req>=8) Mix_PlayChannel(-1, SE_track[req], 0);
	else Mix_PlayChannel(req, SE_track[req], 0);
	#else
	if (req<8)/*  0-7 まではトラック指定 */
	{
			if (NOT_USE_TRACK != res_se[req])/*使用中?*/
			{
				if (req==res_se[req])/* 既に予約されている */
				{
					return;
				}
				goto search_request;	/* 空き検索 */
			}
			else
			{
				res_se[req]=req;/* SE 発音 予約 */
			}
	}
	else
	{
		/* 空き検索 */
		search_request:
		;
		int i;
		for (i=0; i<MAX_VOICE_TRACK; i++)
		{
			if (NOT_USE_TRACK != res_se[i])/*使用中?*/
			{
				if (req==res_se[i])/* 既に予約されている */
				{
					return;
				}
			}
			else
			{
				res_se[i]=req;/* SE 発音 予約 */
			}
		}
	}
	#endif
}
#endif
#if 0
/* トラック指定で voice を予約 */
void voice_play(int req, int play_track )
{
	if ( !useAudio ) return;
	/* --- 範囲チェック */

	/* ----- SE 予約 */
	res_se[play_track] = req;
}
#endif

#if (1==USE_VBL)
/* 予約 voice を再生 */
void voice_play_vbl(void)
{
	#if (1==USE_VBL)
	if ( !useAudio ) return;
	int i;
	for (i=0; i<MAX_VOICE_TRACK; i++)
	{
		if (NOT_USE_TRACK != res_se[i])/*使用中?*/
		{
			/* ----- SE 発音 */
			Mix_PlayChannel(i/*track*/, SE_track[res_se[i/*track*/]], 0);
			res_se[i] = NOT_USE_TRACK;/*未使用*/
		}
	}
	#endif
}
#endif

void setChunkVolume(int volume)
{
	int i;
	for (i=0; i<USE_SE_FILES; i++)
	{	Mix_VolumeChunk(SE_track[i], volume);}
}





// Initialize the sound.

static void voice_load(void)
{
static const char *voice_file_name[USE_SE_FILES] =
{
	"shot.wav", 	/*	0 */
	"hit.wav",		/*	1 */
	"foedst.wav",	/*	2 */
	"bossdst.wav",	/*	3 */
	"shipdst.wav",	/*	4 */
	"bonus.wav",	/*	5 */
	"extend.wav",	/*	6 */
	"bomb.wav", 	/*	7 */
//
	"death.wav",	/*	8 */
	"graze.wav",	/*	9 */
	"e_shot.wav",	/* 10 */
	"e_shot00.wav", /* 11 */
	"b2_shot.wav",	/* 12 */
	"_shot.wav",	/* 13 */
	"e_shot01.wav"	/* 14 */
};		//いろいろ追加
	int i;
	char name[64/*52*/];
	for ( i=0; i<USE_SE_FILES; i++ )
	{
		strcpy(name, moddir);
		strcat(name, "/sounds/");
		strcat(name, voice_file_name[i]);
		if ( NULL == (SE_track[i] = Mix_LoadWAV(name)) )
		{
			pspDebugScreenPrintf("Couldn't load: %s\n", name);
			useAudio = 0;
			return;
		}
		//chunkFlag[i] = 0;
	}
}

void initSound(void)
{
	//useAudio = 0;/*使用不可能*/
	#if 1
	#if (1==USE_VBL)
	/* 初期化(実は無くても実害はない) */
	int i;
	for (i=0; i<MAX_VOICE_TRACK; i++)
	{
		res_se[i] = NOT_USE_TRACK;/* SE 発音 予約 */
	}
	#endif
	#endif
	if ( SDL_InitSubSystem(SDL_INIT_AUDIO) < 0 )
	{
		pspDebugScreenPrintf( "Unable to initialize SDL_AUDIO: %s\n", SDL_GetError());
		return;
	}

	const int audio_rate		= 44100;	// 22050
	const Uint16 audio_format	= AUDIO_S16;
	const int audio_channels	= 2;
	const int audio_buffers 	= 1024;
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0)
	{
		pspDebugScreenPrintf( "Couldn't open audio: %s\n", SDL_GetError());
		return;
	}
	//else
	//{
		/*	サウンドカードに、要求スペックを確認し、
			サウンドカードに機能が無い場合はスペック自体を修正する。
			(pspでは必ず要求スペックがあるので必要ない) */
		//Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	//}
	useAudio = 1;/*使用可能*/
	voice_load();
}
