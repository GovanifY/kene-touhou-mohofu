
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	/src/SDL231/video/SDL_RLEaccel_c.h
	date:		2011/02/04
	source: 	http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2FSDL_RLEaccel_c.h
	revision:	440(2366�ȉ��ł͍ŐV)
--------------------------------------------------------- */

/*
psp - ���r�W���� 440Subversion ���|�W�g���ꗗ: pspps2ps2wareps3ps3warepspware���r�W����:(root)/trunk/SDL/src/video/SDL_RLEaccel_c.h
�ڍו\�� - �ŏI�X�V���� - ���O������ -
*/

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_RLEaccel_c.h,v 1.5 2004/01/04 16:49:21 slouken Exp $";
#endif

/* Useful functions and variables from SDL_RLEaccel.c */

extern int SDL_RLESurface(SDL_Surface *surface);
extern int SDL_RLEBlit(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect);
extern int SDL_RLEAlphaBlit(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect);
extern void SDL_UnRLESurface(SDL_Surface *surface, int recode);

