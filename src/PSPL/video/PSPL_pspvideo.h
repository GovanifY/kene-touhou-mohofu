/*

http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2Fpsp%2FSDL_pspvideo.h
psp - リビジョン 1464Subversion リポジトリ一覧: pspps2ps2wareps3ps3warepspwareリビジョン:(root)/trunk/SDL/src/video/psp/SDL_pspvideo.h
リビジョン 1444 - 詳細表示 - 前のリビジョンとの差分 - 最終更新日時 - ログを見る -
*/

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

/*
    PSP port contributed by:
    Marcus R. Brown <mrbrown@ocgnet.org>
    Jim Paris <jim@jtan.com>
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_nullvideo.h,v 1.4 2004/01/04 16:49:24 slouken Exp $";
#endif

#ifndef _PSPL_pspvideo_h
#define _PSPL_pspvideo_h

//#include "./../include/PSPL_mouse.h"
  #include "./PSPL_sysvideo.h"
//#include "PSPL_mutex.h"

/* Hidden "this" pointer for the video functions */
#define _THIS   SDL_VideoDevice *this


#endif /* _PSPL_pspvideo_h */
