
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
	-------------------------------------------------------
	$Id: mikmod_internals.h, v 1.10 2004/07/18 18:41:33 slouken Exp $
	MusicMod sound library internal definitions.
---------------------------------------------------------*/


#ifndef _MIKMOD_INTERNALS_H
#define _MIKMOD_INTERNALS_H

//#ifdef HAVE_MALLOC_H
#include <malloc.h>
//#endif

#include <stdarg.h>
//#if defined(__OS2__)||defined(__EMX__)||defined(WIN32)
//	#define strcasecmp(s, t) stricmp(s, t)
//#endif

//#include "_mik mod_build.h"
#include "mohou_mikmod.h"



//#if defined(WIN32) && !defined(__STDC__)
//	#define __STDC__ 1
//#endif
//
//#if defined(WIN32) && defined(_MSC_VER)
//	#pragma warning(disable: 4018 4244)
//#endif

//#ifdef macintosh
//	#ifndef __MWERKS__
//		#define __STDC__=1
//	#endif
//	static char *strdup(const char *str)
//	{
//		char *newstr;
//		newstr = (char *)malloc(strlen(str)+1);
//		if ( newstr != NULL )
//		{
//			strcpy(newstr, str);
//		}
//		return (newstr);
//	}
//#endif

//#ifdef WIN32
//	#ifndef __STDC__
//		#define __STDC__
//	#endif
//	#pragma warning(disable:4761)
//#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*---------- More type definitions */

/* s32LONG: 64bit, signed */
//#if defined(__alpha) || defined(_LP64)
//	typedef long		s32LONG;
//	//#define NATIVE2_64BIT_INT
//#elif defined(__WATCOMC__)
//	typedef __int64 	s32LONG;
//#elif defined(WIN32) && !defined(__MWERKS__)
//	typedef LONGLONG	s32LONG;
//#else
//	typedef long long	s32LONG;
//#endif

/*---------- Error handling */

#define mod_music_error_number	MusicMod_errno
#define mod_music_critical		MusicMod_critical
extern MusicMod_handler_t		mod_music_error_handler;

/*---------- Memory allocation */

//extern void *mod_music_calloc(size_t, size_t);
//extern void *mod_music_malloc(size_t);
extern void *module_music_calloc(size_t);
//#define mod_music_malloc(aaa) 	mod_music_calloc(1, aaa)
#define mod_music_malloc(aaa)		module_music_calloc(aaa)
#define mod_music_calloc(aaa, bbb)	module_music_calloc((aaa)*(bbb))
#define mod_music_free(p)			{ if (p) free(p); p = NULL; }

/*---------- MT stuff */

//#ifdef HAVE_PTHREAD
//	#if defined(__OpenBSD__) && !defined(_POSIX_THREADS)
//		#define _POSIX_THREADS
//	#endif
//	#include <pthread.h>
//	#define DECLARE_MUTEX(name) 	extern pthread_mutex_t mod_music_mutex_##name
//	#define MUTEX_LOCK(name)		pthread_mutex_lock(&mod_music_mutex_##name)
//	#define MUTEX_UNLOCK(name)		pthread_mutex_unlock(&mod_music_mutex_##name)
//#elif defined(__OS2__)||defined(__EMX__)
//	#define DECLARE_MUTEX(name) 	extern HMTX mod_music_mutex_##name
//	#define MUTEX_LOCK(name)		if (mod_music_mutex_##name) DosRequestMutexSem(mod_music_mutex_##name, SEM_INDEFINITE_WAIT)
//	#define MUTEX_UNLOCK(name)		if (mod_music_mutex_##name) DosReleaseMutexSem(mod_music_mutex_##name)
//#elif defined(WIN32)
//	#include <windows.h>
//	#define DECLARE_MUTEX(name) 	extern HANDLE mod_music_mutex_##name
//	#define MUTEX_LOCK(name)		if (mod_music_mutex_##name) WaitForSingleObject(mod_music_mutex_##name, INFINITE)
//	#define MUTEX_UNLOCK(name)		if (mod_music_mutex_##name) ReleaseMutex(mod_music_mutex_##name)
//#else
	#define DECLARE_MUTEX(name)
	#define MUTEX_LOCK(name)
	#define MUTEX_UNLOCK(name)
	/*
	基本的にpspの発音方式(sceAudioOutputPannedBlockingの場合)は、非同期(別スレッド生成)で動くが lockは原理的に要らない。
	(pspのSDLはsceAudioOutputPannedBlockingを使い発音している。)
	mikmod(とsmpeg) は lock(SDLのmutex) しないで正常に動くのに、なんで ogg(libtremor) はダメなんだろ？？？
	(libtremorは何が干渉してるのか未解明。libtremorはファイル開けっ放しで読むのかなあ？その辺かも？)
	*/
//#endif

DECLARE_MUTEX(lists);
DECLARE_MUTEX(vars);

/*---------- Portable file I/O */
#define MYMOD_psp_FILE_system		(0)
#define MYMOD_stdio_FILE_system 	(1)
#define MYMOD_SDL_rw_FILE_system	(2)

/* (設定)まだ作り中なので、設定できない。 */
#define USE_MYMOD_FILE_MODE 	MYMOD_stdio_FILE_system

#if (MYMOD_psp_FILE_system==USE_MYMOD_FILE_MODE)
	/* fd を使う */
	#define MYMOD_FILE		SceUID
	#define MYMOD_FILE_ptr	SceUID
	#define MYMOD_fclose	sceIoClose
	#define MYMOD_fopen 	sceIoOpen
	#define MYMOD_attrib	PSP_O_RDONLY, 0777
	#define s_MYMOD_Player_LoadFP s_Player_LoadFD
#endif

#if (MYMOD_stdio_FILE_system==USE_MYMOD_FILE_MODE)
	/* FILE *fp を使う */
	#define MYMOD_FILE		FILE
	#define MYMOD_FILE_ptr	FILE*
	#define MYMOD_fclose	fclose
	#define MYMOD_fopen 	fopen
	#define MYMOD_attrib	"rb"/*attrib*/
	#define s_MYMOD_Player_LoadFP s_Player_LoadFP
#endif

#if (MYMOD_SDL_rw_FILE_system==USE_MYMOD_FILE_MODE)
	//def US E_RWOPS
	/* SDL の RW を使う */
	#define MYMOD_FILE		SDL_RWops
	#define MYMOD_FILE_ptr	FILE*
	#define MYMOD_fclose	fclose
	#define MYMOD_fopen 	fopen
	#define MYMOD_attrib	"rb"/*attrib*/
	#define s_MYMOD_Player_LoadFP s_Player_LoadRW
#endif

/* SDL_RWops compatability */
//#if 0//def US E_RWOPS
//	extern MF_READER *mod_music_new_rwops_reader(MYMOD_FILE *fp_rw);
//	extern void mod_music_delete_rwops_reader(MF_READER*);
//#endif /* US E_RWOPS */
/* End SDL_RWops compatability */



extern MM_BOOL mod_music_FileExists(MM_s8 *fname);

	#define mod_music_fseek(x, y, z)				x->Seek(x, y, z)
#if (MYMOD_psp_FILE_system==USE_MYMOD_FILE_MODE)
	#define mod_music_file_seek_set(x, y)		mod_music_fseek(x, y, PSP_SEEK_SET)
	#define mod_music_file_seek_cur(x, y)		mod_music_fseek(x, y, PSP_SEEK_CUR)
#else
	#define mod_music_file_seek_set(x, y)		mod_music_fseek(x, y, SEEK_SET)
	#define mod_music_file_seek_cur(x, y)		mod_music_fseek(x, y, SEEK_CUR)
#endif
	#define mod_music_file_seek_rewind(x)		mod_music_file_seek_set(x, 0)
	#define mod_music_ftell(x)					x->Tell(x)

	#define mod_music_eof(x)					x->Eof(x)

extern void mod_music_iobase_setcur(MF_READER*);
extern void mod_music_iobase_revert(void);
//extern FILE *mod_music_fopen(MM_s8 *file_name, MM_s8 *attribute);
//extern FILE *mod_music_fopen(MM_s8 *file_name, const char *attribute);
//extern FILE *mod_music_fopen_FILE(MM_s8 *file_name);
extern MYMOD_FILE_ptr mod_music_fopen_FILE(MM_s8 *file_name);

/* support FILE read functions */
#define mod_music_file_read_byte(x) 	x->Get(x)
//efine mod_music_file_read_s8(x)	(s8)(mod_music_file_read_byte(x))
//efine mod_music_file_read_u8(x)	(u8)(mod_music_file_read_byte(x))
#define music_fgetc()					(mod_music_file_read_byte(mmff.mod_FILE_reader))
//efine music_fgetc()					mmff.mod_FILE_reader->Get(mmff.mod_FILE_reader)

//efine mod_music_file_read_s8(mmff.mod_FILE_reader) (s8)(mod_music_file_read_byte(x))
//efine mod_music_file_read_u8(mmff.mod_FILE_reader) (u8)(mod_music_file_read_byte(x))
//efine mod_music_file_read_u16LE(/**/mmff.mod_FILE_reader)
#define music_fget16()					(mod_music_file_read_u16LE(/**/mmff.mod_FILE_reader))
#define music_fget32()					(mod_music_file_read_u32LE(/**/mmff.mod_FILE_reader))




//#define mod_music_file_read_s8_mod_FILE_reader()	(s8)mmff.mod_FILE_reader->Get(mmff.mod_FILE_reader)
//#define mod_music_file_read_u8_mod_FILE_reader()	(u8)mmff.mod_FILE_reader->Get(mmff.mod_FILE_reader)

#define mod_music_file_read_multi_s8(x, y, z) x->Read(x, (void *)y, z)
#define mod_music_file_read_multi_u8(x, y, z) x->Read(x, (void *)y, z)

extern MF_READER *mod_music_new_file_reader(FILE* fp);
extern void mod_music_delete_file_reader(MF_READER*);


//

#if (0)//def USE_MIKMOD_ALONE
/* not support FILE write functions */

#define mod_music_write_s8(x, y) y->Put(y, (int)x)
#define mod_music_write_u8(x, y) y->Put(y, (int)x)

#define mod_music_write_multi_s8(x, y, z) x->Write(x, (void *)y, z)
#define mod_music_write_multi_u8(x, y, z) x->Write(x, (void *)y, z)

extern MF_WRITER *mod_music_new_file_writer(FILE *fp);
extern void mod_music_delete_file_writer(MF_WRITER*);

extern void mod_music_write_string(MF_WRITER *writer, MM_s8 *data);

extern void mod_music_write_s16BE(MF_WRITER *writer, s16 data);
extern void mod_music_write_s16LE(MF_WRITER *writer, s16 data);
extern void mod_music_write_u16BE(MF_WRITER *writer, u16 data);
extern void mod_music_write_u16LE(MF_WRITER *writer, u16 data);

extern void mod_music_write_s32BE(MF_WRITER *writer, s32 data);
extern void mod_music_write_s32LE(MF_WRITER *writer, s32 data);
extern void mod_music_write_u32BE(MF_WRITER *writer, u32 data);
extern void mod_music_write_u32LE(MF_WRITER *writer, u32 data);

extern void mod_music_write_multi_s16BE(MF_WRITER *writer, s16 *buffer, int number);
extern void mod_music_write_multi_s16LE(MF_WRITER *writer, s16 *buffer, int number);
extern void mod_music_write_multi_u16BE(MF_WRITER *writer, u16 *buffer, int number);
extern void mod_music_write_multi_u16LE(MF_WRITER *writer, u16 *buffer, int number);

extern void mod_music_write_multi_s32BE(MF_WRITER *writer, s32 *buffer, int number);
extern void mod_music_write_multi_s32LE(MF_WRITER *writer, s32 *buffer, int number);
extern void mod_music_write_multi_u32BE(MF_WRITER *writer, u32 *buffer, int number);
extern void mod_music_write_multi_u32LE(MF_WRITER *writer, u32 *buffer, int number);
#endif /* USE_MIKMOD_ALONE */

/*---------- Samples */

/*
	This is a handle of sorts attached to any sample registered with SL_RegisterSample();.
	Generally, this only need be used or changed by the loaders and drivers of mikmod.
*/
typedef struct MOD_MUSIC_INTERNAL_SAMPLE_LOAD_t
{
	struct MOD_MUSIC_INTERNAL_SAMPLE_LOAD_t *next;

	u32  length;		/* length of sample (in samples!) */
	u32  loopstart; 	/* repeat position (relative to start, in samples) */
	u32  loopend;		/* repeat end */
	u16  infmt;
	u16  outfmt;
	int 	 scalefactor;
	MM_SAMPLE	*sample;
	struct MF_READER_t	*sample_FILE_reader;
} MOD_MUSIC_INTERNAL_SAMPLE_LOAD;

/*---------- Sample and waves loading interface */

extern void 	 SL_HalveSample(MOD_MUSIC_INTERNAL_SAMPLE_LOAD*, int);
extern void 	 SL_Sample8to16(MOD_MUSIC_INTERNAL_SAMPLE_LOAD*);
extern void 	 SL_Sample16to8(MOD_MUSIC_INTERNAL_SAMPLE_LOAD*);
extern void 	 SL_SampleSigned(MOD_MUSIC_INTERNAL_SAMPLE_LOAD*);
extern void 	 SL_SampleUnsigned(MOD_MUSIC_INTERNAL_SAMPLE_LOAD*);
extern MM_BOOL	 SL_LoadSamples(void);
extern MOD_MUSIC_INTERNAL_SAMPLE_LOAD* SL_RegisterSample(MF_READER*, MM_SAMPLE*, int);
extern MM_BOOL	 SL_Load(void*, MOD_MUSIC_INTERNAL_SAMPLE_LOAD*, u32);
extern MM_BOOL	 SL_Init(MOD_MUSIC_INTERNAL_SAMPLE_LOAD*);
extern void 	 SL_Exit(MOD_MUSIC_INTERNAL_SAMPLE_LOAD*);

/*---------- Internal module representation (UniMod) interface */

/* number of notes in an octave */
#define OCTAVE 12

extern void 	UniSetRow(u8*);
extern u8	UniGetByte(void);
extern u8*	UniFindRow(u8*, u16);
extern void 	UniReset(void);
extern void 	UniWriteByte(u8);
extern void 	UniWriteWord(u16);
extern void 	UniNewline(void);
extern u8*	UniDup(void);
extern MM_BOOL	UniInit(void);
extern void 	UniCleanup(void);
extern void 	UniEffect(u16, u16);
#define UniInstrument(x)	UniEffect(U231_ICODE_0x02_INSTRUMENT, x)
#define UniNote(x)			UniEffect(U231_ICODE_0x01_NOTE, x)
extern void 	UniPTEffect(u8, u8);
extern void 	UniVolEffect(u16, u8);

/*---------- Module Commands(中間コード) */
enum
{
	U231_ICODE_0x00_dummy = 0,
	/* Simple note */
	U231_ICODE_0x01_NOTE /*= 1*/,
	/* Instrument change */
	U231_ICODE_0x02_INSTRUMENT,
	/* UltraTracker effects */
	U231_ICODE_0x03_ULTEFFECT9, /* Sample fine offset */
	/* Impulse Tracker effects */
	U231_ICODE_0x04_ITEFFECTG,	/* porta to note */
	U231_ICODE_0x05_ITEFFECTH,	/* vibrato */
	U231_ICODE_0x06_ITEFFECTI,	/* tremor (xy not incremented) */
	U231_ICODE_0x07_ITEFFECTM,	/* set channel volume */
	U231_ICODE_0x08_ITEFFECTN,	/* slide / fineslide channel volume */
	U231_ICODE_0x09_ITEFFECTP,	/* slide / fineslide channel panning */
	U231_ICODE_0x0a_ITEFFECTT,	/* slide tempo */
	U231_ICODE_0x0b_ITEFFECTU,	/* fine vibrato */
	U231_ICODE_0x0c_ITEFFECTW,	/* slide / fineslide global volume */
	U231_ICODE_0x0d_ITEFFECTY,	/* panbrello */
	U231_ICODE_0x0e_ITEFFECTZ,	/* resonant filters */
	U231_ICODE_0x0f_ITEFFECTS0,
	/* Protracker effects */
	U231_ICODE_0x10_PTEFFECT0,	/* arpeggio */
	U231_ICODE_0x11_PTEFFECT1,	/* porta up */
	U231_ICODE_0x12_PTEFFECT2,	/* porta down */
	U231_ICODE_0x13_PTEFFECT3,	/* porta to note */
	U231_ICODE_0x14_PTEFFECT4,	/* vibrato */
	U231_ICODE_0x15_PTEFFECT5,	/* dual effect 3+A */
	U231_ICODE_0x16_PTEFFECT6,	/* dual effect 4+A */
	U231_ICODE_0x17_PTEFFECT7,	/* tremolo */
	U231_ICODE_0x18_PTEFFECT8,	/* pan */
	U231_ICODE_0x19_PTEFFECT9,	/* sample offset */
	U231_ICODE_0x1a_PTEFFECTA,	/* volume slide */
	U231_ICODE_0x1b_PTEFFECTB,	/* pattern jump */
	U231_ICODE_0x1c_PTEFFECTC,	/* set volume */
	U231_ICODE_0x1d_PTEFFECTD,	/* pattern break */
	U231_ICODE_0x1e_PTEFFECTE,	/* extended effects */
	U231_ICODE_0x1f_PTEFFECTF,	/* set speed */
	/* Fast Tracker effects */
	U231_ICODE_0x20_KEYFADE,		/* note fade */
	U231_ICODE_0x21_VOLEFFECTS, /* volume column effects */
	U231_ICODE_0x22_KEYOFF, 	/* note off */		//廃止XM4	U231_ICODE_0x14_XMEFFECT4,	/* vibrato */
	U231_ICODE_0x23_XMEFFECTA,	/* volume slide */
	U231_ICODE_0x24_XMEFFECTE1, /* fine porta up */
	U231_ICODE_0x25_XMEFFECTE2, /* fine porta down */
	U231_ICODE_0x26_XMEFFECTEA, /* fine volume slide up */
	U231_ICODE_0x27_XMEFFECTEB, /* fine volume slide down */
	U231_ICODE_0x28_XMEFFECTG,	/* set global volume */
	U231_ICODE_0x29_XMEFFECTH,	/* global volume slide */
	U231_ICODE_0x2a_XMEFFECTL,	/* set envelope position */
	U231_ICODE_0x2b_XMEFFECTP,	/* pan slide */
	U231_ICODE_0x2c_XMEFFECTX1, /* extra fine porta up */
	U231_ICODE_0x2d_XMEFFECTX2, /* extra fine porta down */
	U231_ICODE_0x2e_MEDSPEED,	/* OctaMED effects */	//	U231_ICODE_0x2e_resurved,
	U231_ICODE_0x2f_XMEFFECTZ,	/* XM Z synchro */
	/* Scream Tracker effects */
	U231_ICODE_0x30_S3MEFFECTA, /* set speed */
	U231_ICODE_0x31_S3MEFFECTD, /* volume slide */
	U231_ICODE_0x32_S3MEFFECTE, /* porta down */
	U231_ICODE_0x33_S3MEFFECTF, /* porta up */
	U231_ICODE_0x34_S3MEFFECTI, /* tremor */
	U231_ICODE_0x35_S3MEFFECTQ, /* retrig */
	U231_ICODE_0x36_S3MEFFECTR, /* tremolo */
	U231_ICODE_0x37_S3MEFFECTT, /* set tempo */
	U231_ICODE_0x38_S3MEFFECTU, /* fine vibrato */
	U231_ICODE_0x39_MEDEFFECTF1,	/* play note twice */			/* OctaMED effects */
	U231_ICODE_0x3a_MEDEFFECTF2,	/* delay note */				/* OctaMED effects */	//	U231_ICODE_0x3a_resurved,
	U231_ICODE_0x3b_MEDEFFECTF3,	/* play note three times */ 	/* OctaMED effects */	//	U231_ICODE_0x3b_resurved,
	U231_ICODE_0x3c_MAX 			/* Max number(LAST) */
};
// 廃止(XM_EFFECT_04 と ProTracker_EFFECT_04は、全く同じなので統合した。)
#define U231_ICODE_0x14_XMEFFECT4 U231_ICODE_0x14_PTEFFECT4




/* IT resonant filter information */

#define UF_MAXMACRO 	0x10
#define UF_MAXFILTER	0x100

#define FILT_CUT		0x80
#define FILT_RESONANT	0x81

typedef struct //FILTER_t
{
	u8 filter;
	u8 inf;
} FILTER;

/*---------- Instruments */

/* Instrument format flags */
#define IF_OWNPAN		1
#define IF_PITCHPAN 	2

/* Envelope flags: */
#define EF_ON			1
#define EF_SUSTAIN		2
#define EF_LOOP 		4
#define EF_VOLENV		8

/* New Note Action Flags */
#define NNA_CUT 		0
#define NNA_CONTINUE	1
#define NNA_OFF 		2
#define NNA_FADE		3

#define NNA_MASK		3

#define DCT_OFF 		0
#define DCT_NOTE		1
#define DCT_SAMPLE		2
#define DCT_INST		3

#define DCA_CUT 		0
#define DCA_OFF 		1
#define DCA_FADE		2

#define KEY_KICK		0
#define KEY_OFF 		1
#define KEY_FADE		2
#define KEY_KILL		(KEY_OFF|KEY_FADE)

#define KICK_ABSENT 	0
#define KICK_NOTE		1
#define KICK_KEYOFF 	2
#define KICK_ENV		4

#define AV_IT			1	/* IT vs. XM vibrato info */

/*---------- Playing */

#define POS_NONE		(-2)	/* no loop position defined */

#define LAST_PATTERN	(u16)(-1) /* special ``end of song'' pattern */

typedef struct /*ENVPR _tag*/
{
	u8			flg;		/* envelope flag */
	u8			pts;		/* number of envelope points */
	u8			susbeg; 	/* envelope sustain index begin */
	u8			susend; 	/* envelope sustain index end */
	u8			beg;		/* envelope loop begin */
	u8			end;		/* envelope loop end */
	s16 		p;			/* current envelope counter */
	u16 		a;			/* envelope index a */
	u16 		b;			/* envelope index b */
	MM_ENVPT	*env;		/* envelope points */
} MM_ENVPR;


struct MP_VOICE_t;
typedef struct MP_CONTROL_t
{
	MM_INSTRUMENT	*i;
	MM_SAMPLE		*s;
	u8		sample; 		/* which sample number */
	u8		note;			/* the audible note as heard, direct rep of period */
	s16 	outvolume;		/* output volume (vol + sampcol + instvol) */
	s8		chanvol;		/* channel's "global" volume */
	u16 	fadevol;		/* fading volume rate */
	s16 	panning;		/* panning position */
	u8		kick;			/* if true = sample has to be restarted */
	u16 	period; 		/* period to play the sample at */
	u8		nna;			/* New note action type + master/slave flags */

	u8		volflg; 		/* volume envelope settings */
	u8		panflg; 		/* panning envelope settings */
	u8		pitflg; 		/* pitch envelope settings */

	u8		keyoff; 		/* if true = fade out and stuff */
	s16 	handle; 		/* which sample-handle */
	u8		notedelay;		/* (used for note delay) */
	s32 	start;			/* The starting byte index in the sample */

	struct MP_VOICE_t		*slave; 		/* Audio Slave of current effects control channel */

	u8		slavechn;		/* Audio Slave of current effects control channel */
	u8		muted;			/* if set, channel not played */
	u16 	ultoffset;		/* fine sample offset memory */
	u8		anote;			/* the note that indexes the audible */
	u8		oldnote;
	s16 	ownper;
	s16 	ownvol;
	u8		dca;			/* duplicate check action */
	u8		dct;			/* duplicate check type */
	u8* 	row;			/* row currently playing on this channel */
	s8		retrig; 		/* retrig value (0 means don't retrig) */
	u32 	speed;			/* what mm_fine_tune to use */
	s16 	volume; 		/* amiga volume (0 t/m 64) to play the sample at */

	s16 	tmp_volume; 	/* tmp volume */
	u16 	tmp_period; 	/* tmp period */
	u16 	wanted_period;	/* period to slide to (with effect 3 or 5) */

	u8		arpmem; 		/* arpeggio command memory */
	u8		pansspd;		/* panslide speed */
	u16 	slidespeed;
	u16 	portspeed;		/* noteslide speed (toneportamento) */

	u8		s3m_tremor; 	/* s3m tremor (effect I) counter */
	u8		s3m_tremor_on_off_time; 	/* s3m tremor ontime/offtime */
	u8		s3m_volume_slide;	/* last used volslide */
	s8		sliding;
	u8		s3m_retrig_speed;	/* last used retrig speed */
	u8		s3m_retrig_slide;	/* last used retrig slide */

	u8		glissando;		/* glissando (0 means off) */
	u8		wavecontrol;

	s8		vibpos; 		/* current vibrato position */
	u8		vibspd; 		/* "" speed */
	u8		vibdepth;		/* "" depth */

	s8		trmpos; 		/* current tremolo position */
	u8		trmspd; 		/* "" speed */
	u8		trmdepth;		/* "" depth */

	u8		fslideupspd;
	u8		fslidednspd;
	u8		fportupspd; 	/* fx E1 (extra fine portamento up) data */
	u8		fportdnspd; 	/* fx E2 (extra fine portamento dn) data */
	u8		ffportupspd;	/* fx X1 (extra fine portamento up) data */
	u8		ffportdnspd;	/* fx X2 (extra fine portamento dn) data */

	u32 	hioffset;		/* last used high order of sample offset */
	u16 	soffset;		/* last used low order of sample-offset (effect 9) */

	u8		sseffect;		/* last used Sxx effect */
	u8		ssdata; 		/* last used Sxx data info */
	u8		chanvolslide;	/* last used channel volume slide */

	u8		panbwave;		/* current panbrello waveform */
	u8		panbpos;		/* current panbrello position */
	s8		panbspd;		/* "" speed */
	u8		panbdepth;		/* "" depth */

	u16 	newsamp;		/* set to 1 upon a sample / inst change */
	u8		voleffect;		/* Volume Column Effect Memory as used by IT */
	u8		voldata;		/* Volume Column Data Memory */

	s16 	pat_reppos; 	/* patternloop position */
	u16 	pat_repcnt; 	/* times to loop */
} MP_CONTROL;

/* Used by NNA only player (audio control.	AUDTMP is used for full effects
	control). */
typedef struct MP_VOICE_t
{
	MM_INSTRUMENT	*i;
	MM_SAMPLE		*s;
	u8			sample; 		/* which instrument number */

	u8			note;			/* the audible note (as heard, direct rep of period) */
	s16 		volume; 		/* output volume (vol + sampcol + instvol) */
	s8			chanvol;		/* channel's "global" volume */
	u16 		fadevol;		/* fading volume rate */
	s16 		panning;		/* panning position */
	u8			kick;			/* if true = sample has to be restarted */
	u16 		period; 		/* period to play the sample at */
	u8			nna;			/* New note action type + master/slave flags */
	u8			volflg; 		/* volume envelope settings */
	u8			panflg; 		/* panning envelope settings */
	u8			pitflg; 		/* pitch envelope settings */
	u8			keyoff; 		/* if true = fade out and stuff */
	s16 		handle; 		/* which sample-handle */
	s32 		start;			/* The start byte index in the sample */

/* Below here is info NOT in MP_CONTROL!! */
	MM_ENVPR	venv;
	MM_ENVPR	penv;
	MM_ENVPR	cenv;

	u16 		avibpos;		/* autovibrato pos */
	u16 		aswppos;		/* autovibrato sweep pos */

	u32 		totalvol;		/* total volume of channel (before global mixings) */

	MM_BOOL 	mflag;
	s16 		masterchn;
	u16 		masterperiod;

	struct MP_CONTROL_t *master;		/* index of "master" effects channel */
} MP_VOICE;

/*---------- Loaders */

typedef struct MOD_MUSIC_INTERNAL_LOADER_t
{
	struct MOD_MUSIC_INTERNAL_LOADER_t	*next;
	const char	*type;		// MM_s8*	type;
	const char	*version;	// MM_s8*	version;
	MM_BOOL 	(*Init)(void);
	MM_BOOL 	(*Test)(void);
	MM_BOOL 	(*Load)(MM_BOOL);
	void		(*Cleanup)(void);
//曲名廃止	MM_s8*		(*LoadTitle)(void); //	曲名(但しシフトjisコード対象外)
} MOD_MUSIC_INTERNAL_LOADER;

/* internal loader variables */

extern u16	mm_fine_tune[16];
extern MOD_MUSIC_MODULE  	mmoo;						/* static unimod loading space */
//static /*extern*/ u16 npertab[7*OCTAVE];			/* used only by the original MOD loaders */

extern u8*	mm_position_look_up;					/* lookup table for pattern jumps after
													   blank pattern removal */
extern u16* origpositions;

/* (psp) .alignの関係で構造体にいれとく
	レアケースかもしんないけど、pspのGCCで、(とりあえず今使ってるのは4.3.3)
	global 変数の.alignが4byte境界に合ってないと(他が)おかしくなる不具合があるっぽい。
	実際に(他のglobal変数に)異常値を書き込むので困った。
*/
typedef struct /*MOD_MUSIC_INTERNAL_MIDI_FILTER_tag*/
{
	// file read write support.
	MF_READER		*mod_FILE_reader;
//
//struct MOD_MUSIC_INTERNAL_MIDI_FILTER_tag mma_midi;
	//
	s8				re_map[UF_MAXCHAN]; 	/* 64, for removing empty channels */
	// mm midi.
	/*u8*/u32		mm_midi_active_macro;			/* active midi macro number for Sxx */
	/*u8*/u32		mm_midi_position_look_up_counter;
	#if (1==USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)
	MM_BOOL 		mm_midi_use_resonant_filters;	/* resonant filters in use */
	#endif /*(USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)*/
} MOD_MUSIC_INTERNAL_MIDI_FILTER;
extern MOD_MUSIC_INTERNAL_MIDI_FILTER mmff;/* 構造体にいれときゃタブンOK */
//
#if (1==USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)
extern u8	mm_filter_macros[UF_MAXMACRO];	/* midi macros settings */
extern FILTER	filtersettings[UF_MAXFILTER];	/* computed filter settings */
#endif /*(USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)*/

extern int* 	noteindex;

/*---------- Internal loader interface */

#ifdef LINK_IT
extern MM_BOOL	ReadComment(u16);
#endif // LINK_IT
//extern MM_BOOL	ReadLinedComment(u16, u16);
extern MM_BOOL	AllocPositions(int);
extern MM_BOOL	AllocPatterns(void);
extern MM_BOOL	AllocTracks(void);
extern MM_BOOL	AllocInstruments(void);
extern MM_BOOL	AllocSamples(void);
extern MM_s8* DupStr(MM_s8*, u16, MM_BOOL);

/* loader utility functions */
extern int* 	AllocLinear(void);
extern void 	FreeLinear(void);
extern int		speed_to_mm_fine_tune(u32, int);
extern void 	S3MIT_ProcessCmd(u8 cmd, u8 inf, u8 flags);/*unsigned int*/ /*MM_BOOL*/
extern void 	S3MIT_CreateOrders(MM_BOOL);

/* flags for S3MIT_ProcessCmd */
#define S3MIT_OLDSTYLE	1	/* behave as old scream tracker */
#define S3MIT_IT		2	/* behave as impulse tracker */
#define S3MIT_SCREAM	4	/* enforce scream tracker specific limits */

/* used to convert c4spd to linear XM periods (IT and IMF loaders). */
extern u16	getlinearperiod(u16, u32);
extern u32	getfrequency(u8, u32);

/* loader shared data */
#define STM_NTRACKERS	3
extern MM_s8 *STM_Signatures[STM_NTRACKERS];

/*---------- Player interface */

extern MM_BOOL	Player_Init(MOD_MUSIC_MODULE*);
extern void 	Player_Exit(MOD_MUSIC_MODULE*);
extern void 	Player_HandleTick(void);

/*---------- Drivers */

/* max. number of handles a driver has to provide. (not strict) */
#define MAXSAMPLEHANDLES	384

/* These variables can be changed at ANY time and results will be immediate */
extern u16	md_bpm; 	 /* current song / hardware BPM rate */

/* Variables below can be changed via MD_SetNumVoices at any time. However, a
	call to MD_SetNumVoicess while the driver is active will cause the sound to
	skip slightly. */
extern u8	md_numchn;	 /* number of song + sound effects voices */
extern u8	md_sngchn;	 /* number of song voices */
extern u8	md_sfxchn;	 /* number of sound effects voices */
extern u8	md_hardchn;  /* number of hardware mixed voices */
extern u8	md_softchn;  /* number of software mixed voices */

/* This is for use by the hardware drivers only.  It points to the registered
	tickhandler function. */
extern void (*md_player)(void);


/* uLaw conversion */
extern void unsignedtoulaw(char *, int);

/* Parameter extraction helper */
extern MM_s8	*MD_GetAtom(MM_s8*, MM_s8*, MM_BOOL);

#if defined(unix) || defined(__APPLE__) && defined(__MACH__)
/* POSIX helper functions */
extern MM_BOOL	MD_Access(MM_s8 *);
extern MM_BOOL	MD_DropPrivileges(void);
#endif


/*---------- Prototypes for non-MT safe versions of some public functions */


extern void mod_music_registerloader(/*struct*/ MOD_MUSIC_INTERNAL_LOADER*);
extern void mod_music_unregisterloaders(void);


extern void MusicMod_DisableOutput_internal(void);/*mloader.c(?)*/
extern MM_BOOL MusicMod_SetNumVoices_internal(int, int);/*mloader.c(?)*/
extern void Player_Exit_internal(MOD_MUSIC_MODULE*);/*mloader.c(?)*/

extern void Player_Stop_internal(void);/*mplayer.c(?)*/
extern MM_BOOL Player_Paused_internal(void);/*mplayer.c(?)*/

extern void Sample_Free_internal(MM_SAMPLE*);

extern void Voice_Play_internal(s8, MM_SAMPLE*, u32);
extern void Voice_SetFrequency_internal(s8, u32);
extern void Voice_SetPanning_internal(s8, u32);
extern void Voice_SetVolume_internal(s8, u16);
extern void Voice_Stop_internal(s8);
extern MM_BOOL Voice_Stopped_internal(s8);




//#undef psp_max
#ifndef psp_max
	#define psp_max 	__builtin_allegrex_max
#endif
//#undef psp_min
#ifndef psp_min
	//#define psp_min(a, b) (((a)<(b)) ? (a) : (b))
	#define psp_min 	__builtin_allegrex_min
#endif

#if 1/*(てすと)*/
extern void memset32(int *dest, int c, int count);
extern void memset32_uncached(int *dest, int c, int count);
extern void memcpy32(int *dest, int *src, int count); // 32bit word count
#endif/*(てすと)*/


#ifdef __cplusplus
}
#endif

#endif

/* ex:set ts=4: */
