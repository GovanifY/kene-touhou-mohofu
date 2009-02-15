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

/* --- BGM �̊Ǘ��ő吔 (�ő�ǂݍ��݃t�@�C����) */
//#define MAX_MUSIC_FILES 8/*32*/

/* --- SE �̊Ǘ��ő吔 (�ő�ǂݍ��݃t�@�C����)MAX_VOICE_FILES=>USE_SE_FILES  */
#define MAX_VOICE_FILES 16/*64*/

/* ----- BGM �g���b�N */
static Mix_Music *BGM_track;


/* ----- BGM �̐�(�ǂݍ��݃t�@�C����) */
#define USE_BGM_FILES 7 /**/

/* ----- SE �̐�(�ǂݍ��݃t�@�C����) */
#define USE_SE_FILES 15 /*�������ǉ�*/

/* ----- SE �g���b�N */
static Mix_Chunk *SE_track[USE_SE_FILES];

//static int chunkFlag[USE_SE_FILES];

/* ----- �T�E���h�L���t���O */
static int useAudio = 0;


#define NOT_USE_TRACK (-1)/*���g�p�t���O*/

#define MAX_VOICE_TRACK 8/*���z�g���b�N��*/

#if (1==USE_VBL)
static int res_se[MAX_VOICE_TRACK];
#endif

/* ---------------------------------------- */
/* --- BGM �̃��N�G�X�g 					*/
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
	/* --- �͈̓`�F�b�N */
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
};		//���낢��ǉ�
	/* ----- ����܂ł̉��t��~ */
	//Mix_FreeMusic(BGM_track); 	//��������Ȃ��Ɠr���ŉ��y����Ȃ��Ȃ���ۂ�->�d�l�ł͂Ȃ������H
	if ( BGM_track != NULL )
	{
		//if ( Mix_PlayingMusic() )
		{	Mix_HaltMusic();}
		Mix_FreeMusic(BGM_track);
		BGM_track = NULL;
	}
	/* ----- BGM ���t�J�n */
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
				sceKernelDelayThread(5000000);	//�m�F�p�҂�����
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
/* --- �T�E���h�̉��						*/
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
/* --- SE �̃��N�G�X�g						*/
/* ---------------------------------------- */
#if 1
/* voice ��\��B�g���b�N�͓K���Ɍ��߂�B  */
void playChunk(int req)
{
	if ( !useAudio ) return;
	#if (0==USE_VBL)
	//�`�����l����8�܂ł����p�ӂ���Ă��Ȃ��̂�8�ȍ~�̓`�����l�����R
	if (req>=8) Mix_PlayChannel(-1, SE_track[req], 0);
	else Mix_PlayChannel(req, SE_track[req], 0);
	#else
	if (req<8)/*  0-7 �܂ł̓g���b�N�w�� */
	{
			if (NOT_USE_TRACK != res_se[req])/*�g�p��?*/
			{
				if (req==res_se[req])/* ���ɗ\�񂳂�Ă��� */
				{
					return;
				}
				goto search_request;	/* �󂫌��� */
			}
			else
			{
				res_se[req]=req;/* SE ���� �\�� */
			}
	}
	else
	{
		/* �󂫌��� */
		search_request:
		;
		int i;
		for (i=0; i<MAX_VOICE_TRACK; i++)
		{
			if (NOT_USE_TRACK != res_se[i])/*�g�p��?*/
			{
				if (req==res_se[i])/* ���ɗ\�񂳂�Ă��� */
				{
					return;
				}
			}
			else
			{
				res_se[i]=req;/* SE ���� �\�� */
			}
		}
	}
	#endif
}
#endif
#if 0
/* �g���b�N�w��� voice ��\�� */
void voice_play(int req, int play_track )
{
	if ( !useAudio ) return;
	/* --- �͈̓`�F�b�N */

	/* ----- SE �\�� */
	res_se[play_track] = req;
}
#endif

#if (1==USE_VBL)
/* �\�� voice ���Đ� */
void voice_play_vbl(void)
{
	#if (1==USE_VBL)
	if ( !useAudio ) return;
	int i;
	for (i=0; i<MAX_VOICE_TRACK; i++)
	{
		if (NOT_USE_TRACK != res_se[i])/*�g�p��?*/
		{
			/* ----- SE ���� */
			Mix_PlayChannel(i/*track*/, SE_track[res_se[i/*track*/]], 0);
			res_se[i] = NOT_USE_TRACK;/*���g�p*/
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
};		//���낢��ǉ�
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
	//useAudio = 0;/*�g�p�s�\*/
	#if 1
	#if (1==USE_VBL)
	/* ������(���͖����Ă����Q�͂Ȃ�) */
	int i;
	for (i=0; i<MAX_VOICE_TRACK; i++)
	{
		res_se[i] = NOT_USE_TRACK;/* SE ���� �\�� */
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
		/*	�T�E���h�J�[�h�ɁA�v���X�y�b�N���m�F���A
			�T�E���h�J�[�h�ɋ@�\�������ꍇ�̓X�y�b�N���̂��C������B
			(psp�ł͕K���v���X�y�b�N������̂ŕK�v�Ȃ�) */
		//Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	//}
	useAudio = 1;/*�g�p�\*/
	voice_load();
}
