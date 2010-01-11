
/*---------------------------------------------------------
	オーディオ マネージャー
---------------------------------------------------------*/

#include "game_main.h"

/*---------------------------------------------------------

---------------------------------------------------------*/

#if (1==USE_DESIGN_TRACK)
	/* 原理上 処理落ち は少なくなるが、きちんと効果音設計(トラック設定)しないとまともに鳴らない。 */
	#define USE_VSYNC_SOUND 	(1)
#else
	/* 大量のアイテム収集した場合に処理落ちが酷い。 */
	#define USE_VSYNC_SOUND 	(0)
#endif

/* --- 曲の管理最大数 (最大読み込みファイル数) */
//#define MAX_MUSIC_FILES 8/*32*/

/* --- 効果音の管理最大数 (最大読み込みファイル数)MAX_VOICE_ALLOC_FILES=>VOICE16_MAX_FILES	*/
//#define MAX_VOICE_ALLOC_FILES 16/*64*/

/*---------------------------------------------------------

---------------------------------------------------------*/


/* ----- 効果音の数(読み込みファイル数) */
//#define VOICE16_MAX_FILES 15 /*いくつか追加*/

/* ----- 曲のトラック */
static Mix_Music *music_track;

/* ----- 効果音のトラック */
static Mix_Chunk *voice_track[VOICE16_MAX_FILES];

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
	それまでの曲の演奏停止(廃止/統合)	play_music_num(BGM_00_stop)を使う
---------------------------------------------------------*/

//void stop_music(void)
//{
//	//if ( 0==use_audio ) return;
//	if ( music_track != NULL )
//	{
//		//if ( Mix_PlayingMusic() )
//		{	Mix_HaltMusic();	}
//		Mix_FreeMusic(music_track);
//		music_track = NULL;
//	}
//}


/*---------------------------------------------------------
	曲の演奏
	-------------------------------------------------------
	0==num	でそれまでの曲の演奏停止
	1～ 	で曲の演奏
---------------------------------------------------------*/

void play_music_num(int num)
{
	if ( 0==use_audio ) return;
	/* 範囲チェック */
//
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
	/* ----- 設定で曲再生OFFなら再生しない	*/
	if (0==option_config[OPTION_CONFIG_02_BGM])
	{
		return;
	}
	/* ----- BGM 演奏停止コマンド */
	if ( 0==num ) return;
	num--;
	/* ----- BGM 演奏開始 */
	char name[64/*72*/];
	{
		static const char *music_file_name[USE_MUSIC_FILES] =
		{
		/*	0 ----- BGM 演奏停止コマンド */
		/*	1 */	"stage1",	/* 1面道中 */
		/*	2 */	"stage2",	/* 2面道中 */
		/*	3 */	"stage3",	/* 3面道中 */
		/*	4 */	"stage4",	/* 4面道中 */
		/*	5 */	"stage5",	/* 5面道中 */
		/*	6 */	"stage6",	/* 6面道中 */
		/*	7 */	"stage7",	/* エンディング(道中) */
		/*	8 */	"stage8",	/* エキストラステージ1道中 */
		/*  9 */	"stage9",	/* エキストラステージ2道中 */
		/* 10 */	"boss1",	/* 1面ボス */
		/* 11 */	"boss2",	/* 2面ボス */
		/* 12 */	"boss3",	/* 3面ボス */
		/* 13 */	"boss4",	/* 4面ボス */
		/* 14 */	"boss5",	/* 5面ボス */
		/* 15 */	"boss6",	/* 6面ボス */
		/* 16 */	"menu2",	/* エンディング(シナリオ) */
		/* 17 */	"boss8",	/* エキストラステージ1ボス */
		/* 18 */	"boss9",	/* エキストラステージ2ボス */
		/* 19 */	"menu3",	/* キーコンフィグ */
		/* 20 */	"menu1",	/* タイトル画面 */
		};		// いろいろ追加
		const char *name_extention[] =
		{
			NULL, ".mod", ".s3m", ".xm", ".it", ".mp3", ".wav", ".ogg", 	/* 正順 */
//			NULL, ".ogg", ".wav", ".mp3", ".it", ".xm", ".s3m", ".mod", 	/* 逆順 */
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
	効果音のリクエスト
	効果音を予約。トラックは適当に決める。
---------------------------------------------------------*/
#if (0==USE_DESIGN_TRACK)
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

#if (1==USE_DESIGN_TRACK)
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
	4-7 トラックを順番に発音
---------------------------------------------------------*/

#if (1==USE_DESIGN_TRACK)
void bullet_play_04_auto(int req)
{
	if ( 0==use_audio ) return;
	/* 範囲チェック */
//
	/* 効果音 予約 */
	static u8 aaa_play_track;
	aaa_play_track++;
	aaa_play_track &= 0x03;
	aaa_play_track |= 0x04;
	request_voice[aaa_play_track] = req;
}
#endif

/*---------------------------------------------------------
	V-SYNCタイミングで、あらかじめ予約しておいた効果音を
	まとめて再生する
---------------------------------------------------------*/

#if (1==USE_VSYNC_SOUND)
void voice_play_vbl(void)
{
	if ( 0==use_audio ) return;
	/* ----- 設定で効果音再生OFFなら再生しない	*/
	if (0==option_config[OPTION_CONFIG_03_SOUND])
	{
		return;
	}
	#if (1==USE_VSYNC_SOUND)
		#if 1
	/*
		妖々夢の発弾音とか、
		大体 0.025[sec] 位でループしてるみたい
		0.025[sec]==40frame??
		//
		1flame	(0.0166[sec]==60frameでループさせても、本物とかけ離れたビームみたいな変な音になる。
		実際はびしゃびしゃした雨みたいな音)
		0.05[sec]マシンガンみたいな音(速過ぎる？)
		6flame	0.10[sec]マシンガンみたいな音(遅すぎる！)

		1flame	0.0166[sec]ビームみたいな変な音(速過ぎる！)
		2flame	0.0333[sec]
		3flame	0.0500[sec]
		4flame	0.0666[sec]マシンガンみたいな音(これぐらい？)
		5flame	0.0833[sec]マシンガンみたいな音(多分遅い)
		6flame	0.1000[sec]マシンガンみたいな音(遅すぎる！)
		//
		仮に本物と同じ音を使ったとしても、同じ効果音を出すためには、
		効果音の発音方法も同じじゃないと、同じ音は鳴らない。
		本物の効果音の発音方法は、さっぱり解からない。
	*/
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
		#else
	/*
	紅魔郷の場合、あんまり速く鳴らないように、
	コントロールしてるようで、やっぱシステムじゃなくて
	個別に(大体こんな感じで)制御してるのかも
	*/
	{
		static int i=0;
		i++;
		i &= 7;
	//	for (i=0; i<MAX_VOICE_TRACK; i++)
		{
			if (NOT_USE_TRACK != request_voice[i])/*使用中?*/
			{
				/* 効果音 発音 */
				Mix_PlayChannel(i/*track*/, voice_track[request_voice[i/*track*/]], 0);
				request_voice[i] = NOT_USE_TRACK;/*未使用*/
			}
		}
	}
		#endif
	#endif
}
#endif

/*---------------------------------------------------------
	効果音のミキシング音量を変える
---------------------------------------------------------*/
#if 0
void set_voice_volume(int volume)
{
	#ifdef ENABLE_PSP
	#else
	int i;
	for (i=0; i<VOICE16_MAX_FILES; i++)
	{
		Mix_VolumeChunk(voice_track[i], volume);
	}
	#endif
}
#endif

/*---------------------------------------------------------
	曲のミキシング音量を変える
---------------------------------------------------------*/

void set_music_volume(int volume)
{
	Mix_VolumeMusic(volume);
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
	{
		int i;
		for (i=0; i<VOICE16_MAX_FILES; i++)
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
	効果音の初期化、読み込み処理
---------------------------------------------------------*/

static void voice_load(void)
{
	static const char *voice_file_name[VOICE16_MAX_FILES] =
	{
		"se_a.wav", 	/*	0 */	//	"shot.wav", 	/*	0 */
		"se_b.wav", 	/*	1 */	//	"hit.wav",		/*	1 */
		"se_c.wav", 	/*	2 */	//	"foedst.wav",	/*	2 */
		"se_d.wav", 	/*	3 */	//	"bossdst.wav",	/*	3 */
		"se_e.wav", 	/*	4 */	//	"shipdst.wav",	/*	4 */
		"se_f.wav", 	/*	5 */	//	"bonus.wav",	/*	5 */
		"se_g.wav", 	/*	6 */	//	"extend.wav",	/*	6 */
		"se_h.wav", 	/*	7 */	//	"bomb.wav", 	/*	7 */
	//
		"se_i.wav", 	/*	8 */	//	"death.wav",	/*	8 */
		"se_j.wav", 	/*	9 */	//	"graze.wav",	/*	9 */
		"se_k.wav", 	/* 10 */	//	"e_shot.wav",	/* 10 */
		"se_l.wav", 	/* 11 */	//	"e_shot00.wav", /* 11 */
		"se_m.wav", 	/* 12 */	//	"b2_shot.wav",	/* 12 */
		"se_n.wav", 	/* 13 */	//	"_shot.wav",	/* 13 */
		"se_o.wav", 	/* 14 */	//	"e_shot01.wav", /* 14 */
		"se_p.wav", 	/* 15 */	//	"hit.wav",		/* 15 */
	};		//いろいろ追加
	int i;
	char name[64/*52*/];
	for (i=0; i<VOICE16_MAX_FILES; i++)
	{
	//	strcpy(name, data_dir);
		strcpy(name, DIRECTRY_NAME_DATA "/sounds/");
		strcat(name, voice_file_name[i]);
		voice_track[i] = Mix_LoadWAV(name);
		if ( NULL == voice_track[i] )
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
//
	const int audio_rate		= 44100;	// 22050
	const Uint16 audio_format	= AUDIO_S16;
	const int audio_channels	= 2;
	const int audio_buffers 	= 1024; 	//512
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


