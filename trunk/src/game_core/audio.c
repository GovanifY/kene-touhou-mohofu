
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	オーディオ マネージャー
---------------------------------------------------------*/

//	#include "./../SDL_mixer/music_mixer.h"/*何故かこちら*/
	#include "./../PSPL/audio/mixer/music_mixer.h"/*何故かこちら*/

//#ifdef ENABLE_PSP
	//# /* カスタムライブラリを使う */
//	#include <SDL/SDL.h>//#include "SDL.h"
//	#include "SDL_image.h"
	//#include <SDL/SDL_mixer.h>//#include "SD L_mixer.h"
	//#include "./../SDL_mixer/SDL_SDL_audio.h"
	//#include "./../SDL_mixer/000SDL_mixer.h"
	/*???*/
//#else
	//# /* 標準ライブラリを使う */
//	#include <SDL/SDL.h>
//	#include <SDL/SDL_image.h>
	//#include <SDL/SDL_mixer.h>/*#include "SD L_mixer.h"*/
	//#include "./../SDL_mixer/SDL_SDL_audio.h"
	//#include "./../SDL_mixer/000SDL_mixer.h"/*何故かこちら*/
	//#include "./../SDL_mixer/music_mixer_external.h"/*何故かこちら*/
//	#include "./../SDL_mixer/music_mixer.h"/*何故かこちら*/
//#endif

#if (1==USE_DESIGN_TRACK)
	/* 原理上 処理落ち は少なくなるが、きちんと効果音設計(トラック設定)しないとまともに鳴らない。 */
	#define USE_VSYNC_SOUND 	(1)
#else
	/* 大量のアイテム収集した場合に処理落ちが酷い。 */
	#define USE_VSYNC_SOUND 	(0)
#endif

/* --- 曲の管理最大数 (最大読み込みファイル数) */
//#define MAX_MUSIC_FILES 26/*32*/

/* --- 効果音の管理最大数 (最大読み込みファイル数)MAX_VOICE_ALLOC_FILES=>VOICE18_MAX_FILES */
//#define MAX_VOICE_ALLOC_FILES 18/*64*/

/*---------------------------------------------------------

---------------------------------------------------------*/


/* ----- 効果音の数(読み込みファイル数) */
//#define VOICE18_MAX_FILES 15 /*いくつか追加*/

/* ----- 曲のトラック */
static Mix_Music *music_track;

/* ----- 効果音のトラック */
static Mix_Chunk *voice_track[VOICE18_MAX_FILES];

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

//global void pause_music(void)
//{
//	if (0==use_audio) return;
//	Mix_PauseMusic();
//}


/*---------------------------------------------------------
	一時停止した曲の演奏再開
---------------------------------------------------------*/

//global void resume_music(void)
//{
//	if (0==use_audio) return;
//	Mix_ResumeMusic();
//}


/*---------------------------------------------------------
	曲のフェードアウト
---------------------------------------------------------*/

//global void fadeout_music(void)
//{
//	if (0==use_audio) return;
//	Mix_FadeOutMusic(1280);
//}


/*---------------------------------------------------------
	それまでの曲の演奏停止(廃止/統合)	play_music_num(BGM_00_stop)を使う
---------------------------------------------------------*/

//global void stop_music(void)
//{
//	//if (0==use_audio) return;
//	if (NULL != music_track)
//	{
//		//if (Mix_PlayingMusic() )
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
#define CUSTOM_MIXER (1)
#if (0==CUSTOM_MIXER)
	#define aaa_Mix_PlayMusic	Mix_PlayMusic
	#define sdl_mixer_play_channel(aaa,bbb) 	Mix_PlayChannelTimed(aaa,bbb,0,-1)
	//#define sdl_mixer_play_channel(aaa,bbb)	Mix_PlayChannel(aaa,bbb,0)
#else
	extern void sdl_mixer_play_music(Mix_Music *music_track);
	#define aaa_Mix_PlayMusic(aaa,bbb)	sdl_mixer_play_music(aaa)
	extern int sdl_mixer_play_channel(int which, Mix_Chunk *chunk);
#endif

global void play_music_num(int num)
{
	if (0==use_audio) return;
	/* 範囲チェック */
//
	/* ----- それまでの演奏停止 */
	//stop_music();
	//void stop_music(void)
	{
	//	if (0==use_audio) return;
		if (NULL != music_track)
		{
			//if (Mix_PlayingMusic() )
			{	Mix_HaltMusic();	}
			Mix_FreeMusic(music_track);
			music_track = NULL;
		}
	}
	/* ----- 設定で曲再生OFFなら再生しない */
	if (0==option_config[OPTION_CONFIG_02_BGM])
	{
		return;
	}
	/* ----- BGM 演奏停止コマンド */
	if (0==num) return;
	num--;
	/* ----- BGM 演奏開始 */
	char name[64/*72*/];
	{
		static const char *music_file_name[USE_28_MUSIC_FILES] =
		{
		/*	0 ----- BGM 演奏停止コマンド */
		/*	1 */	"stage1",	/* 1面道中 */
		/*	2 */	"stage2",	/* 2面道中 */
		/*	3 */	"stage3",	/* 3面道中 */
		/*	4 */	"stage4",	/* 4面道中 */
		/*	5 */	"stage5",	/* 5面道中 */
		/*	6 */	"stage6",	/* 6面道中 */
		/*	7 */	"stage7",	/* エンディング(道中) */
		/*	8 */	"stage8",	/* エキストラステージ道中 */
		/*	9 */	"stage9",	/* ファンタズムステージ道中 */
		/* 10 */	"menu4",	/* 上海紅茶館 */
		/* 11 */	"boss1",	/* 1面ボス (アリス) */
		/* 12 */	"boss2",	/* 2面ボス (魅魔) */
		/* 13 */	"boss3",	/* 3面ボス (輝夜) */
		/* 14 */	"boss4",	/* 4面ボス (文) */
		/* 15 */	"boss5",	/* 5面ボス (パチェ) */
		/* 16 */	"boss6",	/* 6面ボス (咲夜) */
		/* 17 */	"boss7",	/* チルノ エキストラステージ1ボス */
		/* 18 */	"boss8",	/* アリス エキストラステージ2ボス */
		/* 19 */	"boss9",	/* 魔理沙 エキストラステージ3ボス */
		/* 20 */	"boss10",	/* 妖夢 隠しボス */
		/* 21 */	"boss11",	/* レミリア 隠しボス */
		/* 22 */	"boss0",	/* フラン 隠しボス */
		/* 23 */	"boss12",	/* 博麗 霊夢 隠しボス */
		/* 23 */	"boss13",	/* ルーミア 隠しボス */
		/* 24 */	"menu3",	/* エンディング(シナリオ) */
		/* 25 */	"menu2",	/* キーコンフィグ */
		/* 26 */	"menu1",	/* タイトル画面 */
		/* 27 */	"stage10",	/* 未使用曲です。 */
		};
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
			strcpy(name, DIRECTRY_NAME_DATA_STR "/sounds/");
			strcat(name, music_file_name[num]);
			strcat(name, ((char *)&name_extention[aaa][0]) );
			music_track = Mix_LoadMUS(name);
		}
		while (NULL == music_track);
	}
	aaa_Mix_PlayMusic(music_track, -1);
}


/*---------------------------------------------------------
	効果音のリクエスト
	効果音を予約。トラックは適当に決める。
---------------------------------------------------------*/
#if (0==USE_DESIGN_TRACK)
global void play_voice_auto_track(int req)
{
	if (0==use_audio)	{	return; 	}
	#if (0==USE_VSYNC_SOUND)
	//チャンネルが8までしか用意されていないので8以降はチャンネル自由
	if (req>=8) 	{	sdl_mixer_play_channel(-1,	voice_track[req]);	}
	else			{	sdl_mixer_play_channel(req, voice_track[req]);	}
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
		unsigned int i;
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
global void voice_play(int req, int play_track)
{
	if (0==use_audio)	{	return; 	}
	/* 範囲チェック */
//
	/* 効果音 予約 */
	request_voice[play_track]	= req;
}
#endif

/*---------------------------------------------------------
	4-7 トラックを順番に発音
---------------------------------------------------------*/

#if (1==USE_DESIGN_TRACK)
global void bullet_play_04_auto(int req)
{
	if (0==use_audio)	{	return; 	}
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

/*
	グレイズ発音用の特殊処理:
	グレイズが確実になるように「グレイズ専用トラック」を用意したが、
	用意しただけでは、グレイズは確実に鳴らない。
	//
	グレイズ専用トラックは 1 track 確保した。
	何故 1 track かというと、グレイズ音が重なると変な音になってグレイズ音に聞こえない。
	そういう訳で、実験したら 1 track 以外ありえなかった。
	//
	グレイズ専用トラックは 1 track しかないので、「再入可能処理」を特別に作る必要がある。
	これは、「グレイズ音が人間が鳴ったと認識できる最低長さ」を保持している。
	//
	例えば仮に「グレイズ音」の音の「長さ」（なっている時間）が１秒とする。
	この時 0.5 秒グレイズ音を鳴らして、もう一度始めからグレイズ音を鳴らす。
	この場合 2 回グレイズ音が鳴ったと聞こえる。
	（但し、データーの作り方が悪いとグレイズ音が 1回 にしか聞こえない）
	//
	0.5 秒 と言うのは 30 フレーム。この時間をもっと短くしてみる。
	最低でも 4 - 5 flame に設定しないと音がまともに鳴らない。
	（但し、データーの作り方次第でこの辺は色々変わる。）
*/
//#define SAITEI_GRAZE_WAV_NAGASA	(5)
#define SAITEI_GRAZE_WAV_NAGASA 	(4)

/*---------------------------------------------------------
	グレイズ音を発音
---------------------------------------------------------*/
static int graze_wav_sync_timer;/* グレイズ音が鳴り中時間1 */
//static int graze_wav_pending;/* グレイズ音が鳴り中 の状態 */

#if (1==USE_DESIGN_TRACK)
global void voice_play_graze(void)
{
	if (0==graze_wav_sync_timer)
	{
		graze_wav_sync_timer	= SAITEI_GRAZE_WAV_NAGASA;
	}
}
#endif



/*------------------------------------------------------
	V-SYNCタイミングで、あらかじめ予約しておいた効果音を
	まとめて再生する
---------------------------------------------------------*/

#if (1==USE_VSYNC_SOUND)
global void voice_play_vbl(void)
{
	if (0==use_audio)	{	return; 	}
	/* ----- 設定で効果音再生OFFなら再生しない */
	if (0==option_config[OPTION_CONFIG_03_SOUND])
	{
		return;
	}
	#if (1==USE_VSYNC_SOUND)
		{
			if (0 < graze_wav_sync_timer)/* 前回のグレイズ音が鳴り中か？ */
			{
				graze_wav_sync_timer--;/* グレイズ音が鳴り中時間1を進める。 */
				if ((SAITEI_GRAZE_WAV_NAGASA-1)==graze_wav_sync_timer)/*(初回のみ発音)*/
				//
			//	if (0==graze_wav_sync_timer)
			//	{	/*(グレイズ再入発音が終わりの場合)*/
			//		graze_wav_pending = (0);/*(グレイズ再入発音許可)*/
			//	}
			//	else
			//	if ((SAITEI_GRAZE_WAV_NAGASA-1)==graze_wav_sync_timer)
			//	{	/*(グレイズ再入発音が始めの場合)*/
			//		if (0==graze_wav_pending)/*(SAITEI_GRAZE_WAV_NAGASA 時間経過しないと再入出来ない)*/
					{
			//			graze_wav_pending = (1);/*(グレイズ再入発音禁止)*/
						/*(グレイズ発音処理)*/
						voice_play(VOICE09_GRAZE, TRACK07_GRAZE_AUDIO_DRIVER_INTERNAL);
					}
			//	}
			}
		}
		#if 1
	/*
		妖々夢の発弾音とか、
		大体 0.025[sec] 位でループしてるみたい
		0.025[sec]==40frame??
		//
		1flame	(0.0166[sec]==60flameでループさせても、本物とかけ離れたビームみたいな変な音になる。
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
	{
		unsigned int i;
		for (i=0; i<MAX_VOICE_TRACK; i++)
		{
			if (NOT_USE_TRACK != request_voice[i])/*使用中?*/
			{
				/* 効果音 発音 */
				sdl_mixer_play_channel(i/*track*/, voice_track[request_voice[i/*track*/]]);
				request_voice[i] = NOT_USE_TRACK;/*未使用*/
			}
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
				sdl_mixer_play_channel(i/*track*/, voice_track[request_voice[i/*track*/]]);
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
global void set_voice_volume(int volume)
{
	#ifdef ENABLE_PSP
	#else
	int i;
	for (i=0; i<VOICE18_MAX_FILES; i++)
	{
		Mix_VolumeChunk(voice_track[i], volume);
	}
	#endif
}
#endif

/*---------------------------------------------------------
	曲のミキシング音量を変える
---------------------------------------------------------*/

global void set_music_volume(int volume)
{
	Mix_VolumeMusic(volume);
}


/*---------------------------------------------------------
	オーディオシステムの終了処理
---------------------------------------------------------*/

global void exit_audio(void)
{
	if (0==use_audio)	{	return; 	}
	/* 曲の解放 */
//	if (Mix_PlayingMusic() )
	{	Mix_HaltMusic();	}
	Mix_FreeMusic(music_track);
	music_track = NULL;
	/* 効果音の解放 */
	{
		unsigned int i;
		for (i=0; i<VOICE18_MAX_FILES; i++)
		{
			if (voice_track[i])
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
	static const char *voice_file_name[VOICE18_MAX_FILES] =
	{
		"se_a.wav", 	/*	0 */	//	"shot.wav", 	/*	0 */	"ショット",
		"se_b.wav", 	/*	1 */	//	"hit.wav",		/*	1 */	"メニュー決定",
		"se_c.wav", 	/*	2 */	//	"foedst.wav",	/*	2 */	"メニュー選択",
		"se_d.wav", 	/*	3 */	//	"bossdst.wav",	/*	3 */	"ボス退場",
	//
		"se_e.wav", 	/*	4 */	//	"shipdst.wav",	/*	4 */	"ピチューン",
		"se_f.wav", 	/*	5 */	//	"bonus.wav",	/*	5 */	"アイテム取得",
		"se_g.wav", 	/*	6 */	//	"extend.wav",	/*	6 */	"増えた",
		"se_h.wav", 	/*	7 */	//	"bomb.wav", 	/*	7 */	"自分カード",
	//
		"se_i.wav", 	/*	8 */	//	"death.wav",	/*	8 */	"ザコ倒した",
		"se_j.wav", 	/*	9 */	//	"graze.wav",	/*	9 */	"グレイズ",
		"se_p.wav", 	/* 10 */	//	"hit.wav",		/* 15 */	"時間制限",
		"se_l.wav", 	/* 11 */	//	"e_shot00.wav", /* 11 */	"キラリン☆",
	//
		"se_k.wav", 	/* 12 */	//	"e_shot.wav",	/* 10 */	"ボスショット1",
		"se_m.wav", 	/* 13 */	//	"b2_shot.wav",	/* 12 */	"ボスショット2",
		"se_n.wav", 	/* 14 */	//	"_shot.wav",	/* 13 */	"ボスショット3",
		"se_o.wav", 	/* 15 */	//	"e_shot01.wav", /* 14 */	"ボスショット4",
		"se_q.wav", 	/* 16 */	//	"qupiin.wav"	/* 16 */	"ボスショット5",
	//
		"se_r.wav", 	/* 17 */	//	"wiin.wav"		/* 17 */	"ボス溜め1",
	};					/* 18 */	// MAX
	unsigned int i;
	char name[64];/*52*/
	for (i=0; i<VOICE18_MAX_FILES; i++)
	{
		strcpy(name, DIRECTRY_NAME_DATA_STR "/sounds/");
		strcat(name, voice_file_name[i]);
	//	voice_track[i] = Mix_LoadWAV(name);
		voice_track[i] = Mix_LoadWAV_RW(SDL_RWFromFile(name, "rb"), 1);
		if (NULL == voice_track[i])
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

global void init_audio(void)
{
	//use_audio = 0;/*使用不可能*/
	#if (1)
	#if (1==USE_VSYNC_SOUND)
	/* 初期化(実は無くても実害はない) */
	{
		unsigned int i;
		for (i=0; i<MAX_VOICE_TRACK; i++)
		{
			request_voice[i] = NOT_USE_TRACK;/* 効果音 発音 予約 */
		}
	}
	graze_wav_sync_timer	= (0);/* 初期化(実は無くても実害はない) */
//	graze_wav_pending		= (0);/* 初期化(実は無くても実害はない) */
	#endif
	#endif
//
	const int audio_rate		= 44100;	// 22050
	const u16 audio_format		= AUDIO_S16;
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
