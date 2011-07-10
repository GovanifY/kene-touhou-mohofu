
#include "mohou_mikmod_internals.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
	-------------------------------------------------------
	$Id: mmerror.c,v 1.3 1999/12/28 18:51:11 hercules Exp $

	Error handling functions.
	Register an error handler with mod_music_RegisterErrorHandler() and you're all set.
	-------------------------------------------------------
	The global variables mod_music_error_number, and mod_music_critical are set before the error
	handler in called.	See below for the values of these variables.
---------------------------------------------------------*/

#if 0
MM_CHAR *mod_music_errmsg[MOD_MUSIC_ERROR_MAX+1] =
{
/* No error */

	"No error",

/* Generic errors */

	"Could not open requested file",
	"Out of memory",
	"Dynamic linking failed",

/* Sample errors */

	"Out of memory to load sample",
	"Out of sample handles to load sample",
	"Sample format not recognized",

/* Module errors */

	"Failure loading module pattern",
	"Failure loading module track",
	"Failure loading module header",
	"Failure loading sampleinfo",
	"Module format not recognized",
	"Module sample format not recognized",
	"Synthsounds not supported in MED files",
	"Compressed sample is invalid",

/* Driver errors: */

	"Sound device not detected",
	"Device number out of range",
	"Software mixer failure",
	"Could not open sound device",
	"This driver supports 8 bit linear output only",
	"This driver supports 16 bit linear output only",
	"This driver supports stereo output only",
	"This driver supports uLaw output (8 bit mono, 8 kHz) only",
	"Unable to set non-blocking mode for audio device",

/* AudioFile driver errors	*/

	"Cannot find suitable AudioFile audio port",

/* AIX driver errors */

	"Configuration (init step) of audio device failed",
	"Configuration (control step) of audio device failed",
	"Configuration (start step) of audio device failed",

/* ALSA driver errors */

/* EsounD driver errors */

/* Ultrasound driver errors */

	"Ultrasound driver only works in 16 bit stereo 44 KHz",
	"Ultrasound card could not be reset",
	"Could not start Ultrasound timer",

/* HP driver errors  */

	"Unable to select 16bit-linear sample format",
	"Could not select requested sample-rate",
	"Could not select requested number of channels",
	"Unable to select audio output",
	"Unable to get audio description",
	"Could not set transmission buffer size",

/* Open Sound System driver errors */

	"Could not set fragment size",
	"Could not set sample size",
	"Could not set mono/stereo setting",
	"Could not set sample rate",

/* SGI driver errors */

	"Unsupported sample rate",
	"Hardware does not support 16 bit sound",
	"Hardware does not support 8 bit sound",
	"Hardware does not support stereo sound",
	"Hardware does not support mono sound",

/* Sun driver errors */

	"Sound device initialization failed",

/* OS/2 drivers errors */

	"Could not set mixing parameters",
	"Could not create playback semaphores",
	"Could not create playback timer",
	"Could not create playback thread",

/* DirectSound driver errors */

	"Could not set playback priority",
	"Could not create playback buffers",
	"Could not set playback format",
	"Could not register callback",
	"Could not register event",
	"Could not create playback thread",
	"Could not initialize playback thread",

/* Windows Multimedia API driver errors */

	"Invalid device handle",
	"The resource is already allocated",
	"Invalid device identifier",
	"Unsupported output format",
	"Unknown error",

/* Macintosh driver errors */

	"Unsupported sample rate",
	"Could not start playback",

/* Invalid error */

	"Invalid error code"
};

char *MusicMod_strerror(int code)
{
	if ((code<0)||(code>MOD_MUSIC_ERROR_MAX))
	{
		code = MOD_MUSIC_ERROR_MAX+1;
	}
	return (mod_music_errmsg[code]);
}
#endif

/* User installed error callback */
MusicMod_handler_t mod_music_error_handler	= NULL;
int mod_music_error_number	= 0;
MM_BOOL mod_music_critical	= 0;

MusicMod_handler_t mod_music_registererrorhandler(MusicMod_handler_t proc)
{
	MusicMod_handler_t oldproc=mod_music_error_handler;
	mod_music_error_handler = proc;
	return (oldproc);
}

MusicMod_handler_t MusicMod_RegisterErrorHandler(MusicMod_handler_t proc)
{
	MusicMod_handler_t result;
	MUTEX_LOCK(vars);
	result = mod_music_registererrorhandler(proc);
	MUTEX_UNLOCK(vars);
	return (result);
}

/* ex:set ts=4: */
