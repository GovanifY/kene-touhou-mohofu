
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
	-------------------------------------------------------
	$Id: mikmod.h,v 1.9 2004/07/18 18:41:33 slouken Exp $
	MusicMod sound library include file
---------------------------------------------------------*/

//#ifdef _SDL_MIXER_H
//error! "このプログラムは SDL_mixer.h がインクルードされていると、コンパイルできません。"
//#endif
//#define _SDL_MIXER_H

#ifndef _MOHOUFU_MIK_MOD_H_
#define _MOHOUFU_MIK_MOD_H_

#ifndef u8
	#define u8 unsigned char
#endif
#ifndef u16
	#define u16 unsigned short
#endif
#ifndef u32
	#define u32 unsigned int
#endif

#ifndef s8
	#define s8 signed char
#endif
#ifndef s16
	#define s16 signed short
#endif
#ifndef s32
	#define s32 signed int
#endif
#ifndef MM_BOOL
	#define MM_BOOL int
#endif

//#include "SD L.h"//#include "_SD L.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * ---------- Compiler magic for shared libraries
 */
//#if defined(WIN32) && 0 /* We're not building a shared library */
//	#ifdef DLL_EXPORTS
//		#define MOD_MUSIC_API __declspec(dllexport)
//	#else
//		#define MOD_MUSIC_API __declspec(dllimport)
//	#endif
//#else
	#define MOD_MUSIC_API
//#endif

/*
 *	---------- Library version
 */

#define LIB_MOHOU_MOD_VERSION_MAJOR 	3
#define LIB_MOHOU_MOD_VERSION_MINOR 	1
#define LIB_MOHOU_MOD_REVISION			8

#define LIB_MOHOU_MOD_VERSION \
	((LIB_MOHOU_MOD_VERSION_MAJOR<<16)| \
	 (LIB_MOHOU_MOD_VERSION_MINOR<< 8)| \
	 (LIB_MOHOU_MOD_REVISION))

MOD_MUSIC_API extern long MusicMod_GetVersion(void);

/*
 *	---------- Platform independent-type definitions
 */

//#ifdef WIN32
//	#define WIN32_LEAN_AND_MEAN
//	#include <windows.h>
//	#include <io.h>
//	#include <mmsystem.h>
//#endif

//#if defined(__OS2__)||defined(__EMX__)
//	#define INCL_DOSSEMAPHORES
//	#include <os2.h>
//#else
//	#ifndef WIN32
	//	typedef char MM_CHAR;
		#ifndef MM_CHAR
			#define MM_CHAR char
		#endif
//	#endif
//#endif

/*@DOES_NOT_HAVE_SIGNED@*/

//#if defined(__alpha) || defined(_LP64)
/* 64 bit architectures */
//typedef signed char		s8;			/* 1 byte, signed */
//typedef unsigned char 	u8;			/* 1 byte, unsigned */
//typedef signed short		s16; 		/* 2 bytes, signed */
//typedef unsigned short	u16; 		/* 2 bytes, unsigned */
//typedef signed int		s32; 		/* 4 bytes, signed */
//typedef unsigned int		u32; 		/* 4 bytes, unsigned */
//typedef int				MM_BOOL;			/* 0=false, <>0 true */
//#else
/* 32 bit architectures */

//typedef signed char		s8;			/* 1 byte, signed */
//#ifndef u8
//	typedef unsigned char	u8;			/* 1 byte, unsigned */
//#endif
//typedef signed short		s16; 		/* 2 bytes, signed */
//typedef unsigned short		u16; 		/* 2 bytes, unsigned */
//typedef signed long		s32; 		/* 4 bytes, signed */
//#if !defined(__OS2__)&&!defined(__EMX__)&&!defined(WIN32)
//typedef unsigned long 	u32; 		/* 4 bytes, unsigned */
//typedef int				MM_BOOL;			/* 0=false, <>0 true */
//#endif
//#endif

/*
 *	---------- Error codes
 */

enum
{
	MOD_MUSIC_ERROR_OPENING_FILE = 1,
	MOD_MUSIC_ERROR_OUT_OF_MEMORY,
	MOD_MUSIC_ERROR_DYNAMIC_LINKING,

	MOD_MUSIC_ERROR_SAMPLE_TOO_BIG,
	MOD_MUSIC_ERROR_OUT_OF_HANDLES,
	MOD_MUSIC_ERROR_UNKNOWN_WAVE_TYPE,

	MOD_MUSIC_ERROR_LOADING_PATTERN,
	MOD_MUSIC_ERROR_LOADING_TRACK,
	MOD_MUSIC_ERROR_LOADING_HEADER,
	MOD_MUSIC_ERROR_LOADING_SAMPLEINFO,
	MOD_MUSIC_ERROR_NOT_A_MOD_MUSIC_MODULE,
	MOD_MUSIC_ERROR_NOT_A_STREAM,
	MOD_MUSIC_ERROR_MED_SYNTHSAMPLES,
	MOD_MUSIC_ERROR_ITPACK_INVALID_DATA,

	MOD_MUSIC_ERROR_DETECTING_DEVICE,
	MOD_MUSIC_ERROR_INVALID_DEVICE,
	MOD_MUSIC_ERROR_INITIALIZING_MIXER,
	MOD_MUSIC_ERROR_OPENING_AUDIO,
	MOD_MUSIC_ERROR_8BIT_ONLY,
	MOD_MUSIC_ERROR_16BIT_ONLY,
	MOD_MUSIC_ERROR_STEREO_ONLY,
	MOD_MUSIC_ERROR_ULAW,
	MOD_MUSIC_ERROR_NON_BLOCK,

	MOD_MUSIC_ERROR_AF_AUDIO_PORT,

	MOD_MUSIC_ERROR_AIX_CONFIG_INIT,
	MOD_MUSIC_ERROR_AIX_CONFIG_CONTROL,
	MOD_MUSIC_ERROR_AIX_CONFIG_START,

	MOD_MUSIC_ERROR_GUS_SETTINGS,
	MOD_MUSIC_ERROR_GUS_RESET,
	MOD_MUSIC_ERROR_GUS_TIMER,

	MOD_MUSIC_ERROR_HP_SETSAMPLESIZE,
	MOD_MUSIC_ERROR_HP_SETSPEED,
	MOD_MUSIC_ERROR_HP_CHANNELS,
	MOD_MUSIC_ERROR_HP_AUDIO_OUTPUT,
	MOD_MUSIC_ERROR_HP_AUDIO_DESC,
	MOD_MUSIC_ERROR_HP_BUFFERSIZE,

	MOD_MUSIC_ERROR_OSS_SETFRAGMENT,
	MOD_MUSIC_ERROR_OSS_SETSAMPLESIZE,
	MOD_MUSIC_ERROR_OSS_SETSTEREO,
	MOD_MUSIC_ERROR_OSS_SETSPEED,

	MOD_MUSIC_ERROR_SGI_SPEED,
	MOD_MUSIC_ERROR_SGI_16BIT,
	MOD_MUSIC_ERROR_SGI_8BIT,
	MOD_MUSIC_ERROR_SGI_STEREO,
	MOD_MUSIC_ERROR_SGI_MONO,

	MOD_MUSIC_ERROR_SUN_INIT,

	MOD_MUSIC_ERROR_OS2_MIXSETUP,
	MOD_MUSIC_ERROR_OS2_SEMAPHORE,
	MOD_MUSIC_ERROR_OS2_TIMER,
	MOD_MUSIC_ERROR_OS2_THREAD,

	MOD_MUSIC_ERROR_DS_PRIORITY,
	MOD_MUSIC_ERROR_DS_BUFFER,
	MOD_MUSIC_ERROR_DS_FORMAT,
	MOD_MUSIC_ERROR_DS_NOTIFY,
	MOD_MUSIC_ERROR_DS_EVENT,
	MOD_MUSIC_ERROR_DS_THREAD,
	MOD_MUSIC_ERROR_DS_UPDATE,

	MOD_MUSIC_ERROR_WINMM_HANDLE,
	MOD_MUSIC_ERROR_WINMM_ALLOCATED,
	MOD_MUSIC_ERROR_WINMM_DEVICEID,
	MOD_MUSIC_ERROR_WINMM_FORMAT,
	MOD_MUSIC_ERROR_WINMM_UNKNOWN,

	MOD_MUSIC_ERROR_MAC_SPEED,
	MOD_MUSIC_ERROR_MAC_START,

	MOD_MUSIC_ERROR_MAX
};

/*
 *	---------- Error handling
 */

typedef void (MusicMod_handler)(void);
typedef MusicMod_handler *MusicMod_handler_t;

MOD_MUSIC_API extern int  MusicMod_errno;
MOD_MUSIC_API extern MM_BOOL MusicMod_critical;
MOD_MUSIC_API extern char *MusicMod_strerror(int);

MOD_MUSIC_API extern MusicMod_handler_t MusicMod_RegisterErrorHandler(MusicMod_handler_t);

/*
 *	---------- Library initialization and core functions
 */

MOD_MUSIC_API extern void		MusicMod_RegisterAllDrivers(void);
MOD_MUSIC_API extern void		MusicMod_UnregisterAllDrivers(void);

MOD_MUSIC_API extern MM_CHAR*	MusicMod_InfoDriver(void);

MOD_MUSIC_API extern int		MusicMod_DriverFromAlias(MM_CHAR*);

MOD_MUSIC_API extern MM_BOOL	MusicMod_Init(void/*MM_CHAR*cmdline*/);
MOD_MUSIC_API extern void		MusicMod_Exit(void);
MOD_MUSIC_API extern MM_BOOL	MusicMod_Reset(MM_CHAR*);
MOD_MUSIC_API extern MM_BOOL	MusicMod_SetNumVoices(int,int);
MOD_MUSIC_API extern MM_BOOL	MusicMod_Active(void);
MOD_MUSIC_API extern MM_BOOL	MusicMod_EnableOutput(void);
MOD_MUSIC_API extern void		MusicMod_DisableOutput(void);
MOD_MUSIC_API extern void		MusicMod_Update(void);

MOD_MUSIC_API extern MM_BOOL	MusicMod_InitThreads(void);
MOD_MUSIC_API extern void		MusicMod_Lock(void);
MOD_MUSIC_API extern void		MusicMod_Unlock(void);

/*
 *	---------- Reader, Writer
 */

/* support FILE read functions */
typedef struct MF_READER_t
{
	MM_BOOL 	(*Seek )(struct MF_READER_t*, int, int);
	int 		(*Tell )(struct MF_READER_t*);
	MM_BOOL 	(*Read )(struct MF_READER_t*, void*, size_t);
	int 		(*Get  )(struct MF_READER_t*);
	MM_BOOL 	(*Eof  )(struct MF_READER_t*);
} MF_READER;

#ifdef USE_MIKMOD_ALONE
/* not support FILE write functions */
typedef struct MF_WRITER_t
{
	MM_BOOL 	(*Seek )(struct MF_WRITER_t*, int, int);
	int 		(*Tell )(struct MF_WRITER_t*);
	MM_BOOL 	(*Write)(struct MF_WRITER_t*, void*, size_t);
	MM_BOOL 	(*Put  )(struct MF_WRITER_t*, int);
} MF_WRITER;
#endif /* USE_MIKMOD_ALONE */

/*
 *	---------- Samples
 */

/* Sample playback should not be interrupted */
#define SFX_CRITICAL 1

/* Sample format [loading and in-memory] flags: */
#define SF_16BITS		0x0001
#define SF_STEREO		0x0002
#define SF_SIGNED		0x0004
#define SF_BIG_ENDIAN	0x0008
#define SF_DELTA		0x0010
#define SF_ITPACKED 	0x0020

#define SF_FORMATMASK	0x003F

/* General Playback flags */

#define SF_LOOP 		0x0100
#define SF_BIDI 		0x0200
#define SF_REVERSE		0x0400
#define SF_SUSTAIN		0x0800

#define SF_PLAYBACKMASK 0x0C00

/* Module-only Playback Flags */

#define SF_OWNPAN		0x1000
#define SF_UST_LOOP 	0x2000

#define SF_EXTRAPLAYBACKMASK	0x3000


/* Panning constants */
/* 0: do not use Dolby Surround */
#define USE_PAN_SURROUND 0

#define PAN_LEFT		0
#define PAN_HALFLEFT	64
#define PAN_CENTER		128
#define PAN_HALFRIGHT	192
#define PAN_RIGHT		255
#if (1==USE_PAN_SURROUND)
	#define PAN_SURROUND 512 /* panning value for Dolby Surround */
#endif //(USE_PAN_SURROUND)

typedef struct MM_SAMPLE_t
{
	s16	panning;	/* panning (0-255 or PAN_SURROUND) */
	u32	speed;		/* Base playing speed/frequency of note */
	u8	volume; 	/* volume 0-64 */
	u16	inflags;	/* sample format on disk */
	u16	flags;		/* sample format in memory */
	u32	length; 	/* length of sample (in samples!) */
	u32	loopstart;	/* repeat position (relative to start, in samples) */
	u32	loopend;	/* repeat end */
	u32	susbegin;	/* sustain loop begin (in samples) \  Not Supported */
	u32	susend; 	/* sustain loop end 			   /	  Yet! */

	/* Variables used by the module player only! (ignored for sound effects) */
	u8  globvol; 	/* global volume */
	u8  vibflags;	/* autovibrato flag stuffs */
	u8  vibtype; 	/* Vibratos moved from INSTRUMENT to SAMPLE */
	u8  vibsweep;
	u8  vibdepth;
	u8  vibrate;
	MM_CHAR*  samplename;	/* name of the sample */

	/* Values used internally only */
	u16	avibpos;	/* autovibrato pos [player use] */
	u8	divfactor;	/* for sample scaling, maintains proper period slides */
	u32	seekpos;	/* seek position in file */
	s16	handle; 	/* sample handle used by individual drivers */
} MM_SAMPLE;

/* Sample functions */

MOD_MUSIC_API extern MM_SAMPLE	*Sample_Load(MM_CHAR*);
MOD_MUSIC_API extern MM_SAMPLE	*Sample_LoadFP(FILE*);
MOD_MUSIC_API extern MM_SAMPLE	*Sample_LoadGeneric(MF_READER*);
MOD_MUSIC_API extern void		Sample_Free(MM_SAMPLE*);
MOD_MUSIC_API extern s8		Sample_Play(MM_SAMPLE*,u32, u8);

MOD_MUSIC_API extern void		Voice_SetVolume(s8, u16);
MOD_MUSIC_API extern u16 	Voice_GetVolume(s8);
MOD_MUSIC_API extern void		Voice_SetFrequency(s8, u32);
MOD_MUSIC_API extern u32 	Voice_GetFrequency(s8);
MOD_MUSIC_API extern void		Voice_SetPanning(s8, u32);
MOD_MUSIC_API extern u32 	Voice_GetPanning(s8);
MOD_MUSIC_API extern void		Voice_Play(s8, MM_SAMPLE*, u32);
MOD_MUSIC_API extern void		Voice_Stop(s8);
MOD_MUSIC_API extern MM_BOOL	Voice_Stopped(s8);
MOD_MUSIC_API extern s32 	Voice_GetPosition(s8);
MOD_MUSIC_API extern u32 	Voice_RealVolume(s8);

/*
 *	---------- Internal module representation (UniMod)
 */

/*
	Instrument definition - for information only, the only field which may be
	of use in user programs is the name field
*/

/* Instrument note count */
#define INSTNOTES 120

/* Envelope point */
typedef struct ENVPT_t
{
	s16 pos;
	s16 val;
} ENVPT;

/* Envelope point count */
#define ENVPOINTS 32

/* Instrument structure */
typedef struct MM_INSTRUMENT_t
{
	MM_CHAR* insname;

	u8 flags;
	u16 samplenumber[INSTNOTES];
	u8 samplenote[INSTNOTES];

	u8 nnatype;
	u8 dca;				/* duplicate check action */
	u8 dct;				/* duplicate check type */
	u8 globvol;
	u16 volfade;
	s16 panning; 		/* instrument-based panning var */

	u8 pitpansep;		/* pitch pan separation (0 to 255) */
	u8 pitpancenter; 	/* pitch pan center (0 to 119) */
	u8 rvolvar;			/* random volume varations (0 - 100%) */
	u8 rpanvar;			/* random panning varations (0 - 100%) */

	/* volume envelope */
	u8 volflg;			/* bit 0: on 1: sustain 2: loop */
	u8 volpts;
	u8 volsusbeg;
	u8 volsusend;
	u8 volbeg;
	u8 volend;
	struct ENVPT_t volenv[ENVPOINTS];
	/* panning envelope */
	u8 panflg;			/* bit 0: on 1: sustain 2: loop */
	u8 panpts;
	u8 pansusbeg;
	u8 pansusend;
	u8 panbeg;
	u8 panend;
	struct ENVPT_t panenv[ENVPOINTS];
	/* pitch envelope */
	u8 pitflg;			/* bit 0: on 1: sustain 2: loop */
	u8 pitpts;
	u8 pitsusbeg;
	u8 pitsusend;
	u8 pitbeg;
	u8 pitend;
	struct ENVPT_t pitenv[ENVPOINTS];
} MM_INSTRUMENT;

struct MP_CONTROL_t;
struct MP_VOICE_t;

/*
	Module definition
*/

/* maximum master channels supported */
#define UF_MAXCHAN	64

/* Module flags */
#define UF_XMPERIODS	0x0001	/* XM periods / finetuning */
#define UF_LINEAR		0x0002	/* LINEAR periods (UF_XMPERIODS must be set) */
#define UF_INST 		0x0004	/* Instruments are used */
#define UF_NNA			0x0008	/* IT: NNA used, set numvoices rather than numchn */
#define UF_S3MSLIDES	0x0010	/* uses old S3M volume slides */
#define UF_BGSLIDES 	0x0020	/* continue volume slides in the background */
#define UF_HIGHBPM		0x0040	/* MED: can use >255 bpm */
#define UF_NOWRAP		0x0080	/* XM-type (i.e. illogical) pattern break semantics */
#define UF_ARPMEM		0x0100	/* IT: need arpeggio memory */
#define UF_FT2QUIRKS	0x0200	/* emulate some FT2 replay quirks */
#define UF_PANNING		0x0400	/* module uses panning effects or have
								   non-tracker default initial panning */

typedef struct MOD_MUSIC_MODULE_t
{
	/* general module information */
//廃止		MM_CHAR*			string_songname;	/* 曲名文字列: name of the song */
//廃止		MM_CHAR*			string_modtype; 	/* (MikMODが判別した)モジュール名文字列: string type of module loaded */
//廃止		MM_CHAR*			string_comment; 	/* 曲のコメント文字列: module comments */

		u16			flags;			/* See module flags above */
		u8			numchn; 		/* number of module channels */
		u8			numvoices;		/* max # voices used for full NNA playback */
		u16			numpos; 		/* number of positions in this song */
		u16			numpat; 		/* number of patterns in this song */
		u16			numins; 		/* number of instruments */
		u16			numsmp; 		/* number of samples */
struct	MM_INSTRUMENT_t *instruments;	/* all instruments */
struct	MM_SAMPLE_t 	*samples;		/* all samples */
		u8			realchn;		/* real number of channels used */
		u8			totalchn;		/* total number of channels used (incl NNAs) */

	/* playback settings */
		u16			reppos; 		/* restart position */
		u8			initspeed;		/* initial song speed */
		u16			inittempo;		/* initial song tempo */
		u8			initvolume; 	/* initial global volume (0 - 128) */
		u16			panning[UF_MAXCHAN];	/* 64 panning positions */
		u8			chanvol[UF_MAXCHAN];	/* 64 channel positions */
		u16			bpm;			/* current beats-per-minute speed */
		u16			sngspd; 		/* current song speed */
		s16			volume; 		/* song volume (0-128) (or user volume) */

		MM_BOOL 		extspd; 		/* extended speed flag (default enabled) */
		MM_BOOL 		panflag;		/* panning flag (default enabled) */
		MM_BOOL 		wrap;			/* wrap module ? (default disabled) */
		MM_BOOL 		loop;			/* allow module to loop ? (default enabled) */
		MM_BOOL 		fadeout;		/* volume fade out during last pattern */

		u16			patpos; 		/* current row number */
		s16			sngpos; 		/* current song position */
		u32			sngtime;		/* current song time in 2^-10 seconds */

		s16			relspd; 		/* relative speed factor */

	/* internal module representation */
		u16			numtrk; 		/* number of tracks */
		u8** 		tracks; 		/* array of numtrk pointers to tracks */
		u16* 		patterns;		/* array of Patterns */
		u16* 		pattrows;		/* array of number of rows for each pattern */
		u16* 		positions;		/* all positions */

		MM_BOOL 		forbid; 		/* if true, no player update! */
		u16			numrow; 		/* number of rows on current pattern */
		u16			vbtick; 		/* tick counter (counts from 0 to sngspd) */
		u16			sngremainder;	/* used for song time computation */

struct MP_CONTROL_t 	*control;		/* Effects Channel info (size pf->numchn) */
struct MP_VOICE_t		*voice; 		/* Audio Voice information (size md_numchn) */

		u8			globalslide;	/* global volume slide rate */
		u8			pat_repcrazy;	/* module has just looped to position -1 */
		u16			patbrk; 		/* position where to start a new pattern */
		u8			patdly; 		/* patterndelay counter (command memory) */
		u8			patdly2;		/* patterndelay counter (real one) */
		s16			posjmp; 		/* flag to indicate a jump is needed... */
//		u16			bpmlimit;		/* threshold to detect bpm or speed values */
} MOD_MUSIC_MODULE;

/*
 *	---------- Module loaders
 */

struct MOD_MUSIC_INTERNAL_LOADER_t;

MOD_MUSIC_API extern MM_CHAR*	 MusicMod_InfoLoader(void);
MOD_MUSIC_API extern void	 MusicMod_RegisterAllLoaders(void);
MOD_MUSIC_API extern void	 MusicMod_UnregisterAllLoaders(void);
MOD_MUSIC_API extern void	 MusicMod_RegisterLoader(struct MOD_MUSIC_INTERNAL_LOADER_t*);

MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_669;	/* 669 and Extended-669 (by Tran/Renaissance) */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_amf;	/* DMP Advanced Module Format (by Otto Chrons) */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_dsm;	/* DSIK internal module format */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_far;	/* Farandole Composer (by Daniel Potter) */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_gdm;	/* General DigiMusic (by Edward Schlunder) */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_it;	/* Impulse Tracker (by Jeffrey Lim) */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_imf;	/* Imago Orpheus (by Lutz Roeder) */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_med;	/* Amiga MED modules (by Teijo Kinnunen) */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_m15;	/* Soundtracker 15-instrument */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_mod;	/* Standard 31-instrument Module loader */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_mtm;	/* Multi-Tracker Module (by Renaissance) */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_okt;	/* Amiga Oktalyzer */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_stm;	/* ScreamTracker 2 (by Future Crew) */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_stx;	/* STMIK 0.2 (by Future Crew) */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_s3m;	/* ScreamTracker 3 (by Future Crew) */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_ult;	/* UltraTracker (by MAS) */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_uni;	/* MusicMod and APlayer internal module format */
MOD_MUSIC_API extern struct MOD_MUSIC_INTERNAL_LOADER_t load_xm;	/* FastTracker 2 (by Triton) */

/*
 *	---------- Module player
 */

/* SDL_RWops compatability */
//#if 0//def US E_RWOPS
//	#include "_SDL_rwops.h"
//	MOD_MUSIC_API extern MOD_MUSIC_MODULE* Player_LoadRW(SDL_RWops*, int, MM_BOOL);
//#endif /* US E_RWOPS */
/* End SDL_RWops compatability */

MOD_MUSIC_API extern MOD_MUSIC_MODULE* Player_Load(MM_CHAR*, int, MM_BOOL);
//MOD_MUSIC_API /*extern*/static MOD_MUSIC_MODULE* s_Player_LoadFP(FILE*, int, MM_BOOL);
//MOD_MUSIC_API /*extern*/static MOD_MUSIC_MODULE* s_Player_LoadGeneric(MF_READER*, int, MM_BOOL);	// LIB_MOHOU_MOD_MUSIC not supported.

#ifdef USE_MIKMOD_ALONE
//MOD_MUSIC_API extern MM_CHAR*  Player_LoadTitle(MM_CHAR*);
#endif /* USE_MIKMOD_ALONE */

MOD_MUSIC_API extern void		Player_Free(MOD_MUSIC_MODULE*);
MOD_MUSIC_API extern void		Player_Start(MOD_MUSIC_MODULE*);
MOD_MUSIC_API extern MM_BOOL	Player_Active(void);
MOD_MUSIC_API extern void		Player_Stop(void);
MOD_MUSIC_API extern void		Player_TogglePause(void);
MOD_MUSIC_API extern MM_BOOL	Player_Paused(void);
MOD_MUSIC_API extern void		Player_NextPosition(void);
MOD_MUSIC_API extern void		Player_PrevPosition(void);
MOD_MUSIC_API extern void		Player_SetPosition(u16);
MOD_MUSIC_API extern MM_BOOL	Player_Muted(u8);
MOD_MUSIC_API extern void		Player_SetVolume(s16);
//MOD_MUSIC_API extern MOD_MUSIC_MODULE* Pl ayer_GetModule(void);
MOD_MUSIC_API extern void		Player_SetSpeed(u16);
MOD_MUSIC_API extern void		Player_SetTempo(u16);
MOD_MUSIC_API extern void		Player_Unmute(s32, ...);
MOD_MUSIC_API extern void		Player_Mute(s32, ...);
MOD_MUSIC_API extern void		Player_ToggleMute(s32, ...);
MOD_MUSIC_API extern int		Player_GetChannelVoice(u8);
MOD_MUSIC_API extern u16 	Player_GetChannelPeriod(u8);
MOD_MUSIC_API extern void		Player_SetSynchroValue(int);
MOD_MUSIC_API extern int		Player_GetSynchroValue(void);

typedef void (MusicMod_player)(void);
typedef MusicMod_player *MusicMod_player_t;

MOD_MUSIC_API extern MusicMod_player_t MusicMod_RegisterPlayer(MusicMod_player_t);

#define MUTE_EXCLUSIVE	32000
#define MUTE_INCLUSIVE	32001

/*
 *	---------- Drivers
 */

enum
{
	MD_MUSIC = 0,
	MD_SNDFX
};

enum
{
	MD_HARDWARE = 0,
	MD_SOFTWARE
};

/* Mixing flags */

/* These ones take effect only after MusicMod_Init or MusicMod_Reset */
#define DMODE_16BITS		0x0001	/* enable 16 bit output */
#define DMODE_STEREO		0x0002	/* enable stereo output */
#define DMODE_SOFT_SNDFX	0x0004	/* Process sound effects via software mixer */
#define DMODE_SOFT_MUSIC	0x0008	/* Process music via software mixer */
//#define DMODE_HQMIXER 	0x0010	/* Use high-quality (slower) software mixer */
/* These take effect immediately. */
#define DMODE_SURROUND		0x0100	/* enable surround sound */
#define DMODE_INTERP		0x0200	/* enable interpolation */
//#define DM ODE_PAN_REVERSE	0x0400	/* reverse panning output., for stereo */

#define mm_d_mode	(DMODE_STEREO | DMODE_16BITS | DMODE_SURROUND | DMODE_SOFT_MUSIC | DMODE_SOFT_SNDFX)

//struct MOD_MUSIC_INTERNAL_SAMPLE_LOAD;
struct MOD_MUSIC_INTERNAL_SAMPLE_LOAD_t;

/*
標準 0: リバーブを使用しない。
	 1: リバーブを使用する。
*/
	#define USE_MIK_MOD_REVERB (0)
//	#define USE_MIK_MOD_REVERB (1)


/* These variables can be changed at ANY time and results will be immediate */
//MOD_MUSIC_API extern u8 m d_volume;		/* global sound volume (0-128) */
//MOD_MUSIC_API extern u8 m d_musicvolume;	/* volume of song */
//MOD_MUSIC_API extern u8 m d_sndfxvolume;	/* volume of sound effects */
#if (1==USE_MIK_MOD_REVERB)
MOD_MUSIC_API extern u8 mod_config_reverb;			/* 0 = none;  7 = chaos */
#endif /* (1==USE_MIK_MOD_REVERB) */
//MOD_MUSIC_API extern u8 m d_pansep;		/* 0 = mono;  128 == 100% (full left/right) */

/* The variables below can be changed at any time, but changes will not be
   implemented until MusicMod_Reset is called. A call to MusicMod_Reset may result
   in a skip or pop in audio (depending on the soundcard driver and the settings
   changed). */
//MOD_MUSIC_API extern u16 m d_device;			/* device */
//MOD_MUSIC_API extern u16 m d_mixfreq;	==44100 	/* 44100[Hz] only mixing frequency */
//const MOD_MUSIC_API extern u16 mm_d_mode;	/* mode. See DMODE_? flags above */


/*---------- Virtual channel mixer interface (for user-supplied drivers only) */

MOD_MUSIC_API extern MM_BOOL	MOD_VIRTUAL_MIXER_Init(void);
MOD_MUSIC_API extern void		MOD_VIRTUAL_MIXER_Exit(void);
MOD_MUSIC_API extern MM_BOOL	MOD_VIRTUAL_MIXER_SetNumVoices(void);
MOD_MUSIC_API extern u32 	MOD_VIRTUAL_MIXER_SampleSpace_soft_ware(void/*int*/);
MOD_MUSIC_API extern u32 	MOD_VIRTUAL_MIXER_SampleLength_soft_ware(/*int,*/ MM_SAMPLE *s);

MOD_MUSIC_API extern MM_BOOL	MOD_VIRTUAL_MIXER_PlayStart(void);
MOD_MUSIC_API extern void		MOD_VIRTUAL_MIXER_PlayStop(void);

MOD_MUSIC_API extern s16 	MOD_VIRTUAL_MIXER_SampleLoad_soft_ware(struct MOD_MUSIC_INTERNAL_SAMPLE_LOAD_t *s);/*, int*/
MOD_MUSIC_API extern void		MOD_VIRTUAL_MIXER_SampleUnload(s16);

MOD_MUSIC_API extern u32 	MOD_VIRTUAL_MIXER_WriteBytes(s8*, u32);
MOD_MUSIC_API extern u32 	MOD_VIRTUAL_MIXER_SilenceBytes(s8*, u32);

MOD_MUSIC_API extern void		MOD_VIRTUAL_MIXER_VoiceSetVolume(u8, u16);
MOD_MUSIC_API extern u16 	MOD_VIRTUAL_MIXER_VoiceGetVolume(u8);
MOD_MUSIC_API extern void		MOD_VIRTUAL_MIXER_VoiceSetFrequency(u8, u32);
MOD_MUSIC_API extern u32 	MOD_VIRTUAL_MIXER_VoiceGetFrequency(u8);
MOD_MUSIC_API extern void		MOD_VIRTUAL_MIXER_VoiceSetPanning(u8,u32);
MOD_MUSIC_API extern u32 	MOD_VIRTUAL_MIXER_VoiceGetPanning(u8);
MOD_MUSIC_API extern void		MOD_VIRTUAL_MIXER_VoicePlay(u8, s16, u32, u32, u32, u32, u16);

MOD_MUSIC_API extern void		MOD_VIRTUAL_MIXER_VoiceStop(u8);
MOD_MUSIC_API extern MM_BOOL	MOD_VIRTUAL_MIXER_VoiceStopped(u8);
MOD_MUSIC_API extern s32 	MOD_VIRTUAL_MIXER_VoiceGetPosition(u8);
MOD_MUSIC_API extern u32 	MOD_VIRTUAL_MIXER_VoiceRealVolume(u8);

#ifdef __cplusplus
}
#endif

#endif

/* ex:set ts=4: */
