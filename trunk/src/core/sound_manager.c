
/*---------------------------------------------------------
	オーディオ マネージャー
---------------------------------------------------------*/

#include "game_main.h"

/*---------------------------------------------------------

---------------------------------------------------------*/
/*大量のアイテム収集した場合に処理落ちが酷い*/
#define USE_VSYNC_SOUND	(0)
//#define USE_VSYNC_SOUND 	(1)

/* --- 曲の管理最大数 (最大読み込みファイル数) */
//#define MAX_MUSIC_FILES 8/*32*/

/* --- 効果音の管理最大数 (最大読み込みファイル数)MAX_VOICE_FILES=>USE_VOICE_FILES	*/
#define MAX_VOICE_FILES 16/*64*/

/*---------------------------------------------------------

---------------------------------------------------------*/

/* ----- 曲の数(読み込みファイル数) */
#define USE_MUSIC_FILES 14 /**/

/* ----- 効果音の数(読み込みファイル数) */
#define USE_VOICE_FILES 15 /*いくつか追加*/

/* ----- 曲のトラック */
static Mix_Music *music_track;

/* ----- 効果音のトラック */
static Mix_Chunk *voice_track[USE_VOICE_FILES];

#define NOT_USE_TRACK (-1)/*未使用フラグ*/

#define MAX_VOICE_TRACK 8/*仮想トラック数*/

#if (1==USE_VSYNC_SOUND)
static int request_voice[MAX_VOICE_TRACK];
#endif

/* ----- サウンド有効フラグ */
static int use_audio = 0;/*使用不可能*/


/*---------------------------------------------------------
	曲の一時停止
---------------------------------------------------------*/

//void pause_music(void)
//{
//	if ( 0==use_audio ) return;
//	Mix_PauseMusic();
//}

/*---------------------------------------------------------
	一時停止した曲の演奏再開
---------------------------------------------------------*/

//void resume_music(void)
//{
//	if ( 0==use_audio ) return;
//	Mix_ResumeMusic();
//}

/*---------------------------------------------------------
	曲のフェードアウト
---------------------------------------------------------*/

//void fadeout_music(void)
//{
//	if ( 0==use_audio ) return;
//	Mix_FadeOutMusic(1280);
//}

/*---------------------------------------------------------
	それまでの曲の演奏停止
---------------------------------------------------------*/

void stop_music(void)
{
//	if ( 0==use_audio ) return;
	if ( music_track != NULL )
	{
		//if ( Mix_PlayingMusic() )
		{	Mix_HaltMusic();	}
		Mix_FreeMusic(music_track);
		music_track = NULL;
	}
}

/*---------------------------------------------------------
	曲の演奏
---------------------------------------------------------*/

void play_music(int num)
{
	if ( 0==use_audio ) return;
	/* 範囲チェック */
//
	static const char *music_file_name[USE_MUSIC_FILES] =
	{
		"intro",	/*	0 */
		"stage1",	/*	1 */
		"stage2",	/*	2 */
		"stage3",	/*	3 */
		"stage4",	/*	4 */
		"stage5",	/*	5 */
		"stage6",	/*	6 */
		"stage7",	/*	7 */
		"menu2",	/*	8 */
		"boss1",	/*	9 */
		"boss2",	/* 10 */
		"boss3",	/* 11 */
		"boss4",	/* 12 */
		"boss5",	/* 13 */
	//	"boss6",	/* 14 */
	//	"boss7",	/* 15 */
	};		// いろいろ追加
	/* ----- それまでの演奏停止 */
	//stop_music();
	//void stop_music(void)
	{
	//	if ( 0==use_audio ) return;
		if ( music_track != NULL )
		{
			//if ( Mix_PlayingMusic() )
			{	Mix_HaltMusic();	}
			Mix_FreeMusic(music_track);
			music_track = NULL;
		}
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
				#if 0
				//ps pDebugScreenPrintf("Couldn't load: %s\n", name);
				//sc eKernelDelayThread(5000000);	// 確認用待ち時間
				#endif
				//use_audio = 0;/*使用不可能*/
				return;
			}
		//	strcpy(name, data_dir);
			strcpy(name, DIRECTRY_NAME_DATA "/sounds/");
			strcat(name, music_file_name[num]);
			strcat(name, (char *)&name_extention[aaa][0] );
			music_track = Mix_LoadMUS(name);
		}
		while ( NULL == music_track );
	}
	Mix_PlayMusic(music_track, -1);
}

/*---------------------------------------------------------
	オーディオシステムの終了処理
---------------------------------------------------------*/

void exit_audio(void)
{
	if ( 0==use_audio ) return;
	/* 曲の解放 */
	//if ( Mix_PlayingMusic() )
	{	Mix_HaltMusic();	}
	Mix_FreeMusic(music_track);
	music_track = NULL;
	/* 効果音の解放 */
	{int i;
		for (i=0; i<USE_VOICE_FILES; i++)
		{
			if ( voice_track[i] )
			{
				Mix_FreeChunk(voice_track[i]);
			}
		}
	}
	/* ミキサー(オーディオシステム)の解放 */
	Mix_CloseAudio();
}

/*---------------------------------------------------------
	効果音のリクエスト
	効果音を予約。トラックは適当に決める。
---------------------------------------------------------*/
#if 1
void play_voice_auto_track(int req)
{
	if ( 0==use_audio ) return;
	#if (0==USE_VSYNC_SOUND)
	//チャンネルが8までしか用意されていないので8以降はチャンネル自由
	if (req>=8) Mix_PlayChannel(-1, voice_track[req], 0);
	else Mix_PlayChannel(req, voice_track[req], 0);
	#else
	if (req<8)/*  0-7 まではトラック指定 */
	{
			if (NOT_USE_TRACK != request_voice[req])/*使用中?*/
			{
				if (req==request_voice[req])/* 既に予約されている */
				{
					return;
				}
				goto search_request;	/* 空き検索 */
			}
			else
			{
				request_voice[req] = req;/* 効果音 発音 予約 */
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
			if (NOT_USE_TRACK != request_voice[i])/*使用中?*/
			{
				if (req==request_voice[i])/* 既に予約されている */
				{
					return;
				}
			}
			else
			{
				request_voice[i] = req;/* 効果音 発音 予約 */
			}
		}
	}
	#endif
}
#endif

/*---------------------------------------------------------
	V-SYNCタイミングで、発声できる用に
	トラック指定で効果音を予約しておく
---------------------------------------------------------*/

#if 0
void voice_play(int req, int play_track )
{
	if ( 0==use_audio ) return;
	/* 範囲チェック */
//
	/* 効果音 予約 */
	request_voice[play_track] = req;
}
#endif

/*---------------------------------------------------------
	V-SYNCタイミングで、あらかじめ予約しておいた効果音を
	まとめて再生する
---------------------------------------------------------*/

#if (1==USE_VSYNC_SOUND)
void voice_play_vbl(void)
{
	#if (1==USE_VSYNC_SOUND)
	if ( 0==use_audio ) return;
	int i;
	for (i=0; i<MAX_VOICE_TRACK; i++)
	{
		if (NOT_USE_TRACK != request_voice[i])/*使用中?*/
		{
			/* 効果音 発音 */
			Mix_PlayChannel(i/*track*/, voice_track[request_voice[i/*track*/]], 0);
			request_voice[i] = NOT_USE_TRACK;/*未使用*/
		}
	}
	#endif
}
#endif

/*---------------------------------------------------------
	効果音のミキシング音量を変える
---------------------------------------------------------*/

void set_voice_volume(int volume)
{
	#ifdef ENABLE_PSP
	#else
	int i;
	for (i=0; i<USE_VOICE_FILES; i++)
	{
		Mix_VolumeChunk(voice_track[i], volume);
	}
	#endif
}

/*---------------------------------------------------------
	曲のミキシング音量を変える
---------------------------------------------------------*/

void set_music_volume(int volume)
{
	Mix_VolumeMusic(volume);
}

/*---------------------------------------------------------
	効果音の初期化、読み込み処理
---------------------------------------------------------*/

static void voice_load(void)
{
	static const char *voice_file_name[USE_VOICE_FILES] =
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
						/* 15 */
	};		//いろいろ追加
	int i;
	char name[64/*52*/];
	for ( i=0; i<USE_VOICE_FILES; i++ )
	{
	//	strcpy(name, data_dir);
		strcpy(name, DIRECTRY_NAME_DATA "/sounds/");
		strcat(name, voice_file_name[i]);
		if ( NULL == (voice_track[i] = Mix_LoadWAV(name)) )
		{
			//ps pDebugScreenPrintf("Couldn't load: %s\n", name);
			use_audio = 0;/*使用不可能*/
//			return;
		}
	}
}

/*---------------------------------------------------------
	オーディオシステムの初期化処理
---------------------------------------------------------*/

void init_audio(void)
{
	//use_audio = 0;/*使用不可能*/
	#if 1
	#if (1==USE_VSYNC_SOUND)
	/* 初期化(実は無くても実害はない) */
	int i;
	for (i=0; i<MAX_VOICE_TRACK; i++)
	{
		request_voice[i] = NOT_USE_TRACK;/* 効果音 発音 予約 */
	}
	#endif
	#endif
	if ( SDL_InitSubSystem(SDL_INIT_AUDIO) < 0 )
	{
		//ps pDebugScreenPrintf( "Unable to initialize SDL_AUDIO: %s\n", SDL_GetError());
		return;
	}

	const int audio_rate		= 44100;	// 22050
	const Uint16 audio_format	= AUDIO_S16;
	const int audio_channels	= 2;
	const int audio_buffers 	= 1024;
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0)
	{
		//ps pDebugScreenPrintf( "Couldn't open audio: %s\n", SDL_GetError());
		return;
	}
	//else
	//{
		/*	サウンドカードに、要求スペックを確認し、
			サウンドカードに機能が無い場合はスペック自体を修正する。
			(pspでは必ず要求スペックがあるので必要ない) */
		//Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	//}
	use_audio = 1;/*使用可能*/
	voice_load();
}
