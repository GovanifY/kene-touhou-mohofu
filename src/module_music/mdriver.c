
#include "mohou_mikmod_internals.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
	-------------------------------------------------------
	$Id: mdriver.c, v 1.6 2001/09/11 18:49:18 slouken Exp $
	These routines are used to access the available soundcard drivers.
	-------------------------------------------------------
	なんかドライバーという単位で、サウンドカードを登録する機能みたいだけど、
	模倣風は一つしか使わないんだし、もっと簡略化出来る気がする。
	-------------------------------------------------------
	-------------------------------------------------------
	$Id: drv_nos.c, v 1.4 2001/09/11 18:49:18 slouken Exp $
	Driver for no output
	-------------------------------------------------------
	Written by Jean-Paul Mikkers <mikmak@via.nl>
	-------------------------------------------------------
	-------------------------------------------------------
	Name:
		DRV_SDL.C
	Description:
		Mikmod driver for output using the Simple DirectMedia Layer.
---------------------------------------------------------*/

//#include <unistd.h>
//#include <pwd.h>
//#include <sys/stat.h>
#include <string.h>

/* Macro to define a missing driver, yet allowing binaries to dynamically link
	with the library without missing symbol errors */
//#define MISSING(a) MOD_MUSIC_DRIVER a = { NULL, NULL, NULL, 0, 0 }
//#define ZEROLEN 32768
//static	s8 *zerobuf = NULL;


/* ex:set ts=4: */
/* The following variable should not be changed! */
//MOD_MUSIC_API extern MOD_MUSIC_DRIVER* md_driver; /* Current driver in use. */

//static	MOD_MUSIC_DRIVER *firstdriver	= NULL;

extern	MOD_MUSIC_MODULE *pf; /* modfile being played */

//		u16 m d_device		= 0;
//		u16 m d_mixfreq		= 44100;44100only
//		u16 mm_d_mode		= DMODE_STEREO | DMODE_16BITS | DMODE_SURROUND | DMODE_SOFT_MUSIC | DMODE_SOFT_SNDFX;
//		u8 m d_pansep		= 128;		/* 128 == 100% (full left/right) */
		#if (1==USE_MIK_MOD_REVERB)
		u8 mod_config_reverb 	= 3; /* 3==(m d_reverb 6)Reverb */
		#endif /* (1==USE_MIK_MOD_REVERB) */
//		u8 m d_volume		= 128;		/* Global sound volume (0-128) */
//		u8 m d_musicvolume	= 128;		/* volume of song */
//		u8 m d_sndfxvolume	= 128;		/* volume of sound effects */
		u16 md_bpm			= 125;

/* Do not modify the numchn variables yourself!  use MD_SetVoices() */
		u8 md_numchn 	= 0;
		u8 md_sngchn 	= 0;
		u8 md_sfxchn 	= 0;
		//
//		u8 md_hardchn	= 0;
		u8 md_softchn	= 0;
		//
		void (*md_player)(void) = Player_HandleTick;
static	MM_BOOL  isplaying	= 0;
//static	MM_BOOL  initialized	= 0;
static	u8 *sfxinfo;
static	int sfxpool;

static	MM_SAMPLE **md_sample = NULL;







void Voice_SetVolume_internal(s8 voice, u16 vol)
{
	if ((voice<0)||(voice>=md_numchn)) return;
	/* range checks */
//	if (m d_musicvolume>128)	{	m d_musicvolume = 128;}
//	if (m d_sndfxvolume>128)	{	m d_sndfxvolume = 128;}
//	if (m d_volume>128) m d_volume = 128;

//	u32 tmp;
//	tmp = (u32)vol * /*(u32)m d_volume **/ ((voice<md_sngchn)?(u32)m d_musicvolume:(u32)m d_sndfxvolume);
//	MOD_VIRTUAL_MIXER_VoiceSetVolume(voice, (tmp>>7) ); 	//tmp / /*16384*/128UL
	MOD_VIRTUAL_MIXER_VoiceSetVolume(voice, ((u32)vol) );
}

void Voice_SetFrequency_internal(s8 voice, u32 frq)
{
	if ((voice<0)||(voice>=md_numchn)) return;
	if ((md_sample[voice])&&(md_sample[voice]->divfactor))
	{	frq /= md_sample[voice]->divfactor;}
	MOD_VIRTUAL_MIXER_VoiceSetFrequency(voice, frq);
}

void Voice_SetPanning_internal(s8 voice, u32 pan)
{
	if ((voice<0)||(voice>=md_numchn)) return;
#if (1==USE_PAN_SURROUND)
	if (pan!=PAN_SURROUND)
#endif //(1==USE_PAN_SURROUND)
//	{
//	//	if (m d_pansep>128) 	{m d_pansep = 128;}
//	//	if (mm_d_mode & DM ODE_PAN_REVERSE) 	{pan = 255-pan;}
//	//	pan = (((s16)(pan-128)*m d_pansep)/128)+128;
//		pan = (((s16)(pan-128) ) )+128;
//	}
	MOD_VIRTUAL_MIXER_VoiceSetPanning(voice, pan);
}

void Voice_Play_internal(s8 voice, MM_SAMPLE* s, u32 start)
{
	u32	repend;
	if ((voice<0)||(voice>=md_numchn))	{return;}
	md_sample[voice] = s;
	repend = s->loopend;
	if (s->flags&SF_LOOP)
	{	/* repend can't be bigger than size */
		if (repend>s->length)	{repend = s->length;}}
	MOD_VIRTUAL_MIXER_VoicePlay(voice, s->handle, start, s->length, s->loopstart, repend, s->flags);
}

void Voice_Stop_internal(s8 voice)
{
	if ((voice<0)||(voice>=md_numchn))	{return;}
	if (voice>=md_sngchn)
		/* It is a sound effects channel, so flag the voice as non-critical! */
	{	sfxinfo[voice-md_sngchn] = 0;}
	MOD_VIRTUAL_MIXER_VoiceStop(voice);
}

MM_BOOL Voice_Stopped_internal(s8 voice)
{
	if ((voice<0)||(voice>=md_numchn))	{return (0);}
	return (MOD_VIRTUAL_MIXER_VoiceStopped(voice));
}

//	MOD_MUSIC_API extern struct MOD_MUSIC_DRIVER_t drv_nos; 		/* no sound */
//	MOD_MUSIC_API extern struct MOD_MUSIC_DRIVER_t drv_sdl; 		/* Simple Direct Media */

//extern void MusicMod_Exit_internal(void);
static void MusicMod_Exit_internal(void)
{
	MusicMod_DisableOutput_internal();
	/*md_driver->Exit*/MOD_VIRTUAL_MIXER_Exit();
	md_numchn		= md_sfxchn = md_sngchn = 0;
	/*(終了する場合)*/
//	md_driver		= &drv_nos;/*(絶対ある筈のドライバ)*/
//	md_driver		= &drv_sdl;

	if (sfxinfo)	{free(sfxinfo);}
	if (md_sample)	{free(md_sample);}
	md_sample		= NULL;
	sfxinfo 		= NULL;

//	initialized = 0;
}
#if 1/*模倣風で使用してい */
static MM_BOOL smod_music_init(void/*MM_s8 *cmdline*/)
{
	mod_music_critical = 1;
	/* if m d_device==0, try to find a device number */
//	if (!m d_device)
//	{
//		m d_device = 1/*t*/;
//	}
	#if 0
	メモ
	mmm_driver.Init==static MM_BOOL NS_Init(void)
	{
	//	zerobuf = (s8*)mod_music_malloc(ZEROLEN);
		return (MOD_VIRTUAL_MIXER_Init());
	}
	mmm_driver.Init==static MM_BOOL SDRV_Init(void)
	{
		//mm_d_mode |= DMODE_SOFT_MUSIC | DMODE_SOFT_SNDFX;
		return (MOD_VIRTUAL_MIXER_Init());
	}
	#endif
//	if (mmm_driver.Init())/*(2回init???よくわかんない)	*/
	if (MOD_VIRTUAL_MIXER_Init())/*(2回init???よくわかんない)  */
	{
		/*md_driver->Exit*/MOD_VIRTUAL_MIXER_Exit();
		MusicMod_Exit_internal();
		if (mod_music_error_handler)	{mod_music_error_handler();}
		return (1);
	}
//	initialized 		= 1;
	mod_music_critical	= 0;
	return (0);
}

/*global*/MM_BOOL MusicMod_Init(void/*MM_s8 *cmdline*/)
{
	MM_BOOL result;

	MUTEX_LOCK(vars);
	MUTEX_LOCK(lists);
	result	= smod_music_init(/*cmdline*/);
	MUTEX_UNLOCK(lists);
	MUTEX_UNLOCK(vars);

	return (result);
}
#endif/*模倣風で使用していない*/


#if 1/*模倣風で使用してい */
void MusicMod_Exit(void)
{
	MUTEX_LOCK(vars);
	MUTEX_LOCK(lists);
	MusicMod_Exit_internal();
	MUTEX_UNLOCK(lists);
	MUTEX_UNLOCK(vars);
}
#endif/*模倣風で使用していない*/

//extern MM_BOOL MusicMod_EnableOutput_internal(void);
static MM_BOOL MusicMod_EnableOutput_internal(void)
{
	mod_music_critical = 1;
	if (!isplaying)
	{
		if (MOD_VIRTUAL_MIXER_PlayStart())	{	return (1);}
		isplaying = 1;
	}
	mod_music_critical = 0;
	return (0);
}

/*
	Limits the number of hardware voices to the specified amount.
	This function should only be used by the low-level drivers.
*/
static void LimitSoftVoices(int limit)
{
	if (/*(mm_d_mode & DMODE_SOFT_SNDFX) &&*/ (md_sfxchn>limit))	{md_sfxchn = limit;}
	if (/*(mm_d_mode & DMODE_SOFT_MUSIC) &&*/ (md_sngchn>limit))	{md_sngchn = limit;}
/*	if (mm_d_mode & DMODE_SOFT_SNDFX)*/
			{	md_softchn = md_sfxchn;}
/*	else	{	md_softchn = 0;}*/
/*	if (mm_d_mode & DMODE_SOFT_MUSIC)*/ 	{md_softchn += md_sngchn;}
	//
	{
		int t = 0;
		while (md_softchn>limit)
		{
			if (++t & 1)
					{	if (/*(mm_d_mode & DMODE_SOFT_SNDFX) &&*/ (md_sfxchn>4))	{	md_sfxchn--;	}	}
			else	{	if (/*(mm_d_mode & DMODE_SOFT_MUSIC) &&*/ (md_sngchn>8))	{	md_sngchn--;	}	}
		//	if (0==(mm_d_mode & DMODE_SOFT_SNDFX))
		//			{	md_softchn = md_sfxchn;}
		/*	else*/	{	md_softchn = 0;}
		//	if (0==(mm_d_mode & DMODE_SOFT_MUSIC))
		//	{	md_softchn += md_sngchn;}
		}
	}
	md_numchn = /*md_hardchn+*/md_softchn;
}

/* If either parameter is -1, the current set value will be retained. */
MM_BOOL MusicMod_SetNumVoices_internal(int music, int sfx)
{
	MM_BOOL resume	= 0;
	int t, oldchn	= 0;

	if ((!music)&&(!sfx))	{return (1);}
	mod_music_critical = 1;
	if (isplaying)
	{
		MusicMod_DisableOutput_internal();
		oldchn	= md_numchn;
		resume	= 1;
	}

	if (sfxinfo)	{free(sfxinfo);}
	if (md_sample)	{free(md_sample);}
	md_sample	= NULL;
	sfxinfo 	= NULL;

	if (music!=-1)	{md_sngchn = music;}
	if (sfx!=-1)	{md_sfxchn = sfx;}
	md_numchn = md_sngchn + md_sfxchn;

//	LimitHardVoices(0/*md_driver->HardVoiceLimit*/);/*(???)*/
	LimitSoftVoices(255/*md_driver->SoftVoiceLimit*/);

	if (MOD_VIRTUAL_MIXER_SetNumVoices())
	{
		MusicMod_Exit_internal();
		if (mod_music_error_number)
		{	if (mod_music_error_handler!=NULL)	{mod_music_error_handler();}}
		md_numchn = md_softchn = /*md_hardchn =*/ md_sfxchn = md_sngchn = 0;
		return (1);
	}

	if (md_sngchn+md_sfxchn)
	{	md_sample = (MM_SAMPLE**)mod_music_calloc( (md_sngchn+md_sfxchn), sizeof(MM_SAMPLE*));}
	if (md_sfxchn)
	{	sfxinfo = (u8 *)mod_music_calloc(md_sfxchn, sizeof(u8));}

	/* make sure the player doesn't start with garbage */
	for (t=oldchn; t<md_numchn; t++)	{Voice_Stop_internal(t);}
	sfxpool 	= 0;
	if (resume) 	{MusicMod_EnableOutput_internal();}
	mod_music_critical	= 0;
	return (0);
}

#if 1/*模倣風で使用してい mplayer.c */
MM_BOOL MusicMod_EnableOutput(void)
{
	MM_BOOL result;
	MUTEX_LOCK(vars);
	result = MusicMod_EnableOutput_internal();
	MUTEX_UNLOCK(vars);
	return (result);
}
#endif/*模倣風で使用していない*/

void MusicMod_DisableOutput_internal(void)
{
	if (isplaying
//	&& (0!=m d_device)	/*&& md_driver*/	/*???*/
	)
	{
		isplaying = 0;
		MOD_VIRTUAL_MIXER_PlayStop();
	}
}

//extern MM_BOOL MusicMod_Active_internal(void);
static MM_BOOL MusicMod_Active_internal(void)
{
	return (isplaying);
}

#if 1/*模倣風で使用してい mplayer.c */
MM_BOOL MusicMod_Active(void)
{
	MM_BOOL result;
	MUTEX_LOCK(vars);
	result = MusicMod_Active_internal();
	MUTEX_UNLOCK(vars);
	return (result);
}
#endif/*模倣風で使用していない*/

/*
	Plays a sound effects sample.
	Picks a voice from the number of voices allocated for
	use as sound effects (loops through voices, skipping all active criticals).
	Returns the voice that the sound is being played on.
*/
s8 Sample_Play_internal(MM_SAMPLE *s, u32 start, u8 flags)
{
	int orig = sfxpool;/* for cases where all channels are critical */
	if (!md_sfxchn) 	{	return (-1);}
	if (s->volume>64)	{	s->volume = 64;}
//
	/* check the first location after sfxpool */
	int c;
	do
	{
		c = sfxpool+md_sngchn;
		if (sfxinfo[sfxpool]&SFX_CRITICAL)
		{
			if (MOD_VIRTUAL_MIXER_VoiceStopped(c))
			{
				goto bbb;
			}
		}
		else
		{
			goto bbb;
		}
		sfxpool++;
		if (sfxpool>=md_sfxchn) sfxpool = 0;
	} while (sfxpool!=orig);
	return (-1);
bbb:
	sfxinfo[sfxpool] = flags;
	Voice_Play_internal(c, s, start);
	MOD_VIRTUAL_MIXER_VoiceSetVolume(c, s->volume<<2);
	Voice_SetPanning_internal(c, s->panning);
	MOD_VIRTUAL_MIXER_VoiceSetFrequency(c, s->speed);
	sfxpool++;
	if (sfxpool>=md_sfxchn) sfxpool=0;
	return (c);
}

/*---------- MT-safe stuff */

#ifdef HAVE_PTHREAD
	#define INIT_MUTEX(name) pthread_mutex_t mod_music_mutex_##name=PTHREAD_MUTEX_INITIALIZER
#elif defined(__OS2__)||defined(__EMX__)
	#define INIT_MUTEX(name) HMTX mod_music_mutex_##name
#elif defined(WIN32)
	#define INIT_MUTEX(name) HANDLE mod_music_mutex_##name
#else
	#define INIT_MUTEX(name)
#endif

INIT_MUTEX(vars);
INIT_MUTEX(lists);

/* ex:set ts=4: */

/* ex:set ts=4: */
