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


extern int keyboard[];
extern KEYCONFIG keyconfig;
GAMESTATE state;
GAMESTATE laststate;
//PSP_HEAP_SIZE_MAX();
//int score;
//int lives;
//int vol ume=2; /* 0-3 */
int difficulty=DIFF_MEDIUM;

static char option_module_files[20][20];
static int modcount=0;


//void wait(){
//	int i,j;
//	for (i=0; i<10000; i++) {
//		for (j=0; j<2000; j++)
//			j++;
//	}
//}


void readModDirs(void)
{
	//fprintf(stdout,"readDataDirs\n");
	DIR *dp;
	struct dirent *dir;
	if ( (dp = opendir(".")) == NULL )
	{
		fprintf(stderr, "Can't open directory: ");
		exit(1);
	}
	while ((dir = readdir(dp)) != NULL)
	{
		if (dir->d_name[0]!='_') continue;
		//fprintf(stdout,"%s\n", dir->d_name);
		strcpy(option_module_files[modcount],dir->d_name);
		modcount++;
	}
	closedir(dp);
}

void chooseModDir(void)
{
	int i;
	int selected=0;
	SceCtrlData cpad;
	memset(moddir,0,20);

	readModDirs();
	while (strlen(moddir)<2)
	{
		pspDebugScreenClear();
		pspDebugScreenPrintf("\n"
			" _  __ ___  __   _  ___  \n"
			"| |/ /| __||  \\ | || __| \n"
			"| ' < | _| | |\\\\| || _|  \n"
			"|_|\\_\\|___||_| \\__||___| \n\n");
		//意味の無い改変。気付いてる人いるのかな？

		pspDebugScreenPrintf("\t\t version 0.9\n\n\n");
		pspDebugScreenPrintf("Please choose the mod:\n\n");
		for (i=0; i<modcount; i++)
		{
			if (i==selected)
				pspDebugScreenPrintf(">>> ");
			else
				pspDebugScreenPrintf("    ");
			pspDebugScreenPrintf("%s\n",option_module_files[i]);
		}
		while (1)
		{
			sceCtrlReadBufferPositive(&cpad, 1);
			if ((cpad.Buttons & PSP_CTRL_UP) || (cpad.Ly <= 0x30)) {
				if (selected>0) selected--;
				break;
			}
			else if ((cpad.Buttons & PSP_CTRL_DOWN) || (cpad.Ly >= 0xD0)) {
				if (selected<modcount-1) selected++;
				break;
			}
			if (cpad.Buttons & PSP_CTRL_CROSS) {
				strcpy(moddir,option_module_files[selected]);
				sceKernelDelayThread(100000);		//これを入れないと何故か終了する。
				break;
			}
		}
		for (i=0; i<8; i++)
		{
			sceDisplayWaitVblankStart();/*vsync*/
		}//wait();
	}
	pspDebugScreenClear();
}


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
	pspDebugScreenInit();
	pspDebugScreenPrintf("Starting K.E.T.M.\n");

	if (ini_load()==-1) //090110
	{
		chooseModDir();
		keyconfig.si= KEY_BOMB; 	//□
		keyconfig.ma= KEY_CANCEL;	//○
		keyconfig.ba= KEY_SHOT; 	//×
		keyconfig.sa= KEY_SC_SHOT;	//△
		keyconfig.u = KEY_UP;		//↑
		keyconfig.d = KEY_DOWN; 	//↓
		keyconfig.l = KEY_LEFT; 	//←
		keyconfig.r = KEY_RIGHT;	//→
		keyconfig.rt= KEY_SLOW; 	//R
		keyconfig.lt= KEY_BOMB; 	//L
		keyconfig.sl= KEY_NONE; 	//SELECT
		keyconfig.st= KEY_PAUSE;	//START
	}

	game_init(argc, argv);
	play_music(BGM_00_intro);
//
	scePowerSetClockFrequency(333,333,166);
	while (state.mainstate!=ST_GAME_QUIT)
	{
		keyboard_poll();
		switch (state.mainstate)
		{
		case ST_START_INTRO:	if (state.newstate) {	startintro_init();		state.newstate=0;	}	startintro_work();	break;
		case ST_INTRO:			if (state.newstate) {	intro_init();			state.newstate=0;	}	intro_work();		break;
		case ST_GAME_PLAY:		if (state.newstate) {	thegame_init(); 		state.newstate=0;	}	thegame_work(); 	break;
		case ST_GAME_OVER:		if (state.newstate) {	gameover_init();		state.newstate=0;	}	gameover_work();	break;	/*newstate(ST_INTRO,0,1);*/
		case ST_MENU:			if (state.newstate) {	menu_init();			state.newstate=0;	}	menu_work();		break;
		case ST_SHOW_HCLIST:	if (state.newstate) {	hsc_show_init();		state.newstate=0;	}	hsc_show_work();	break;
		case ST_ENTRY_HCLIST:	if (state.newstate) {	hsc_entry_init();		state.newstate=0;	}	hsc_entry_work();	break;
		case ST_KEY_CONFIG: 	if (state.newstate) {	key_config_init();		state.newstate=0;	}	key_config_work();	break;
		case ST_PLAYER_SELECT:	if (state.newstate) {	player_opt_init();		state.newstate=0;	}	player_opt_work();	break;
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
