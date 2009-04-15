#include "main.h"
#include <psppower.h>

extern SDL_Surface *screen;


#if 0
	/* makefile に -Dmain=SDL_main がある場合  */
#else
	/* makefile に -Dmain=SDL_main がない場合  */

//============================================================
//	PSP module info section
//============================================================

PSP_MODULE_INFO("KENE", 0x0000, 0, 4);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);
PSP_MAIN_THREAD_STACK_SIZE_KB(32);

#endif

extern KEYCONFIG keyconfig;
GAMESTATE state;
GAMESTATE laststate;
//PSP_HEAP_SIZE_MAX();
//int score;
//int lives;
//int vol ume=2; /* 0-3 */
int difficulty=RANK_NORMAL;



/* Exit callback */
static int exit_callback(int arg1, int arg2, void *common)
{
	newstate(ST_GAME_QUIT,0,1);//[***** 追加:090103
	hsc_save();
	sceKernelExitGame();
	return 0;
}

/* Callback thread */
static int CallbackThread(SceSize args, void *argp)
{
	int cbid;
	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

/* Sets up the callback thread and returns its thread id */
static int SetupCallbacks(void)
{
	int thid = 0;
	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if (thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}
	return thid;
}

int main(int argc, char *argv[])
{
	SetupCallbacks();/* [HOME]キーで終了するためのコールバックを登録 */

	game_init(/*argc, argv*/);
	play_music(BGM_00_intro);
//
	scePowerSetClockFrequency(333,333,166);
	while (state.mainstate!=ST_GAME_QUIT)
	{
		keyboard_poll();
		switch (state.mainstate)
		{
	//	case ST_START_INTRO:	if (state.newstate) {	startintro_init();		state.newstate=0;	}	startintro_work();	break;
	//	case ST_INTRO:			if (state.newstate) {	intro_init();			state.newstate=0;	}	intro_work();		break;
		case ST_MENU:			if (state.newstate) {	menu_init();			state.newstate=0;	}	menu_work();		break;
		case ST_GAME_PLAY:		if (state.newstate) {	thegame_init(); 		state.newstate=0;	}	thegame_work(); 	break;
		case ST_GAME_OVER:		if (state.newstate) {	gameover_init();		state.newstate=0;	}	gameover_work();	break;	/*newstate(ST_INTRO,0,1);*/
		case ST_SHOW_HCLIST:	if (state.newstate) {	hsc_show_init();		state.newstate=0;	}	hsc_show_work();	break;
		case ST_ENTRY_HCLIST:	if (state.newstate) {	hsc_entry_init();		state.newstate=0;	}	hsc_entry_work();	break;
		case ST_KEY_CONFIG: 	if (state.newstate) {	key_config_init();		state.newstate=0;	}	key_config_work();	break;
		case ST_PLAYER_SELECT:	if (state.newstate) {	player_opt_init();		state.newstate=0;	}	player_opt_work();	break;
		case ST_STORY:			if (state.newstate) {	story_init();			state.newstate=0;	}	story_work();		break;
		}
		//voice_play_vbl();
		//fps_show();
		SDL_Flip(screen);
		//SDL_UpdateRect(screen, 0, 0, 0, 0);
		fps_newframe();
	}
	scePowerSetClockFrequency(222,222,111);
//
	/* TODO: Free everything (memory, SDL_Surfaces, Joysticks...) */

	hsc_save();

	//fprintf(stdout,"Thank you for playing\n");
	SDL_Quit(); 	//[***** 追加:090103
	sceKernelExitGame();
	return(0);
}
