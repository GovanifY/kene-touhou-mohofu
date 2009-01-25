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
#include "SDL.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <pspdebug.h>

#include "SDL_mixer.h"
#include "soundmanager.h"
#include "support.h"
static int useAudio = 0;

#define MUSIC_NUM 2 

/*static char *musicFileName[MUSIC_NUM] = {
  "stg0", "stg1",
};
static Mix_Music *music[MUSIC_NUM];
*/


static Mix_Music *music;

#define CHUNK_NUM 11	//いくつか追加

static char *chunkFileName[CHUNK_NUM] = {
  "shot.wav", "hit.wav", "foedst.wav", "bossdst.wav", "shipdst.wav", "bonus.wav", "extend.wav", "bomb.wav", "death.wav", "graze.wav", "e_shot.wav"
};		//いろいろ追加
static Mix_Chunk *chunk[CHUNK_NUM];
static int chunkFlag[CHUNK_NUM];

void closeSound() {
  int i;
  if ( !useAudio ) return;
  if ( Mix_PlayingMusic() ) {
    Mix_HaltMusic();
  }
/*
  for ( i=0 ; i<MUSIC_NUM ; i++ ) {
    if ( music[i] ) {
      Mix_FreeMusic(music[i]);
    }
  }
*/
  for ( i=0 ; i<CHUNK_NUM ; i++ ) {
    if ( chunk[i] ) {
      Mix_FreeChunk(chunk[i]);
    }
  }
  Mix_CloseAudio();
}


// Initialize the sound.

static void loadSounds() {
  int i;
  char name[52];
/*
  for ( i=0 ; i<MUSIC_NUM ; i++ ) {
    strcpy(name, "sounds/");
    strcat(name, musicFileName[i]);
    strcat(name, ".s3m");
    if ( NULL == (music[i] = Mix_LoadMUS(name)) ) {

		//try mod
    	strcpy(name, "sounds/");
    	strcat(name, musicFileName[i]);
    	strcat(name, ".mod");
    	if ( NULL == (music[i] = Mix_LoadMUS(name)) ) {
    		strcpy(name, "sounds/");
    		strcat(name, musicFileName[i]);
    		strcat(name, ".xm");
    		if ( NULL == (music[i] = Mix_LoadMUS(name)) ) {
    			strcpy(name, "sounds/");
    			strcat(name, musicFileName[i]);
    			strcat(name, ".ogg");
    			if ( NULL == (music[i] = Mix_LoadMUS(name)) ) {
      				pspDebugScreenPrintf("Couldn't load: %s\n", name);
      				//useAudio = 0;
      				//return;
			}
		}
	}
    }
  } 
*/ 
  for ( i=0 ; i<CHUNK_NUM ; i++ ) {
    strcpy(name, moddir);
    strcat(name, "/sounds/");
    strcat(name, chunkFileName[i]);
    if ( NULL == (chunk[i] = Mix_LoadWAV(name)) ) {
      pspDebugScreenPrintf("Couldn't load: %s\n", name);
      useAudio = 0;
      return;
    }
    chunkFlag[i] = 0;
  }
}

void initSound() {
  int audio_rate;
  Uint16 audio_format;
  int audio_channels;
  int audio_buffers;

  if ( SDL_InitSubSystem(SDL_INIT_AUDIO) < 0 ) {
    pspDebugScreenPrintf( "Unable to initialize SDL_AUDIO: %s\n", SDL_GetError());
    return;
  }

  audio_rate = 44100;
  //audio_rate = 22050;
  audio_format = AUDIO_S16;
  audio_channels = 2;
  audio_buffers = 1024;
  
  if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0) {
    pspDebugScreenPrintf( "Couldn't open audio: %s\n", SDL_GetError());
    return;
  } else {
    Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
  }

  useAudio = 1;
  loadSounds();
}

// Play/Stop the music/chunk.

void playMusic(char *file) {
	//Mix_FreeMusic(music);		//これを入れないと途中で音楽が鳴らなくなるっぽい->仕様ではなかった？
	if ( !useAudio ) return;
	char name[72];
	if ( music!=NULL ) {
		//if ( Mix_PlayingMusic() )
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music=NULL;
	}
  
	strcpy(name, moddir);
	strcat(name, "/sounds/");
	strcat(name, file);
	strcat(name, ".ogg");		//ogg以外全て消した
	if ( NULL == (music = Mix_LoadMUS(name)) ) {
		pspDebugScreenPrintf("Couldn't load: %s\n", name);
		sceKernelDelayThread(5000000);	//確認用待ち時間
		//useAudio = 0;
		return;
	}
  Mix_PlayMusic(music,-1);
}

void pauseMusic() {
  if ( !useAudio ) return;
  Mix_PauseMusic();
}

void resumeMusic() {
  if ( !useAudio ) return;
  Mix_ResumeMusic();
}

void fadeMusic() {
  if ( !useAudio ) return;
  Mix_FadeOutMusic(1280);
}

void stopMusic() {
  if ( !useAudio ) return;
    if ( music!=NULL ) {
	  	if ( Mix_PlayingMusic() ){
    		Mix_HaltMusic();
      		Mix_FreeMusic(music);
			music=NULL;
		}
    }
}

void playChunk(int idx) {
  if ( !useAudio ) return;
  if(idx>=8) Mix_PlayChannel(-1, chunk[idx], 0);	//チャンネルが8までしか用意されていないので8以降はチャンネル自由
  else Mix_PlayChannel(idx, chunk[idx], 0);
}

void setChunkVolume(int volume) {
	int i;
	for (i=0; i<CHUNK_NUM; i++)
		Mix_VolumeChunk(chunk[i], volume);
}
