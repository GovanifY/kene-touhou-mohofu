/*
http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2FSDL.c
psp - ���r�W���� 440Subversion ���|�W�g���ꗗ: pspps2ps2wareps3ps3warepspware���r�W����:(root)/trunk/SDL/src/SDL.c
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
 "@(#) $Id: PSPL.c,v 1.9 2004/01/04 16:49:10 slouken Exp $";
#endif

/* Initialization code for SDL */

#include "./include/PSPL.h" 		/* ��芸����(��) */

/* Initialization/Cleanup routines */

#ifdef CHECK_LEAKS
int surfaces_allocated = 0;
#endif

/* Function prototypes */

/*only boot once*/extern void /*SD LCALL*/ PSPL_VideoInit(void);/*const char *driver_name, u32 flags*/
/*only exit once*/extern void /*SD LCALL*/ PSPL_VideoQuit(void);

/*only boot once*/extern void /*SD LCALL*/ PSPL_AudioInit(void);/*const char *driver_name*/
/*only exit once*/extern void /*SD LCALL*/ PSPL_AudioQuit(void);

/*---------------------------------------------------------
	�J�n����
---------------------------------------------------------*/

int SDL_Init(u32 dummy_flags)
{
//	SDL_ClearError();						/* Clear the error message */
	PSPL_VideoInit();/*only boot once*/ 	/* Initialize the video/event subsystem */
	PSPL_AudioInit();/*only boot once*/ 	/* Initialize the audio subsystem */
	return (0);/*(0==����������)*/
}


/*---------------------------------------------------------
	�I������
---------------------------------------------------------*/

void SDL_Quit(void)
{
	/* Quit all subsystems */
	/* Shut down requested initialized subsystems */
	PSPL_AudioQuit();/*only exit once*/
	PSPL_VideoQuit();/*only exit once*/
//
	#ifdef CHECK_LEAKS
	/* Print the number of surfaces not freed */
	if ( surfaces_allocated != 0 )
	{
		fprintf(stderr, "SDL Warning: %d SDL surfaces extant\n", surfaces_allocated);
	}
	#endif
}
