/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2FSDL_error_c.h

psp - リビジョン 440Subversion リポジトリ一覧: pspps2ps2wareps3ps3warepspwareリビジョン:(root)/trunk/SDL/src/SDL_error_c.h
詳細表示 - 最終更新日時 - ログを見る -
*/

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_error_c.h,v 1.6 2004/07/18 22:57:40 slouken Exp $";
#endif

/* This file defines a structure that carries language-independent
   error messages
*/

#ifndef _PSPL_error_c_h
#define _PSPL_error_c_h

#define ERR_MAX_STRLEN		128
#define ERR_MAX_ARGS		5

typedef struct SDL_error
{
	/* This is a numeric value corresponding to the current error */
	int error;

	/* This is a key used to index into a language hashtable containing
	   internationalized versions of the SDL error messages.  If the key
	   is not in the hashtable, or no hashtable is available, the key is
	   used directly as an error message format string.
	*/
	unsigned char key[ERR_MAX_STRLEN];

	/* These are the arguments for the error functions */
	int argc;
	union {
		void *value_ptr;
#if 0	/* What is a character anyway?	(UNICODE issues) */
		unsigned char value_c;
#endif
		int value_i;
		double value_f;
		unsigned char buf[ERR_MAX_STRLEN];
	} args[ERR_MAX_ARGS];
} SDL_error;

#endif /* _PSPL_error_c_h */
