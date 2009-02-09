#include "main.h"
#include <psppower.h>

extern SDL_Surface *screen;
/*
typedef struct SDL_Surface {
        Uint32 flags;                           // サーフェスのフラグ
        SDL_PixelFormat *format;                // ピクセル  フォーマット 
        int w, h;                               // サーフェスの幅と高さ
        Uint16 pitch;                           // サーフェスのスキャンラインの長さ(バイト)
        void *pixels;                           // 実際のピクセルデータへのポインタ
        SDL_Rect clip_rect;                     // サーフェスのクリッピング領域
        // Reference count -- used when freeing surface
        int refcount;                           // ほとんど読込

	// This structure also contains private fields not shown here
} SDL_Surface;
*/
extern int keyboard[];
extern KEYCONFIG keyconfig;
GAMESTATE state;
GAMESTATE laststate;
PSP_HEAP_SIZE_MAX();
int score;
int lives;
int volume=2; /* 0-3 */
int difficulty=DIFF_MEDIUM;

char mods[20][20];
int modcount=0;


//void wait(){
//        int i,j;
//        for (i=0; i<10000; i++) {
//                for(j=0; j<2000; j++)
//			j++;
//	}
//}


void readModDirs( ) {
	//fprintf(stdout,"readDataDirs\n");
  DIR *dp;
  struct dirent *dir;
  if ( (dp = opendir(".")) == NULL ) {
    fprintf(stderr, "Can't open directory: ");
    exit(1);
  }
  while ((dir = readdir(dp)) != NULL) {
	if (dir->d_name[0]!='_') continue;
    //fprintf(stdout,"%s\n", dir->d_name);
    strcpy(mods[modcount],dir->d_name);
    modcount++;
  }
  closedir(dp);
}


void chooseModDir() {
	int i;
	int selected=0;
	SceCtrlData cpad;
	memset(moddir,0,20);

	readModDirs();
	while (strlen(moddir)<2) {
		pspDebugScreenClear();
printF("\n \
 _  __ ___  __   _  ___  \n \
| |/ /| __||  \\ | || __| \n \
| ' < | _| | |\\\\| || _|  \n \
|_|\\_\\|___||_| \\__||___| \n\n"); 		//意味の無い改変。気付いてる人いるのかな？

		printF("\t\t version 0.9\n\n\n");
		printF("Please choose the mod:\n\n");
		for (i=0; i<modcount; i++) {
			if (i==selected)
				printF(">>> ");
			else
				printF("    ");
			printF("%s\n",mods[i]);		


		}
		while (1) {
		sceCtrlReadBufferPositive(&cpad, 1);
		if ((cpad.Buttons & PSP_CTRL_UP) || (cpad.Ly <= 0x30)){
			if (selected>0) selected--;
			break;

		}
		if ((cpad.Buttons & PSP_CTRL_DOWN) || (cpad.Ly >= 0xD0)){
			if (selected<modcount-1) selected++;
			break;

		}
		
		 if (cpad.Buttons & PSP_CTRL_CROSS) {
			strcpy(moddir,mods[selected]);
			sceKernelDelayThread(100000);		//これを入れないと何故か終了する。
			break;
		}
		}
		for(i=0; i<8; i++){
			sceDisplayWaitVblankStart();/*vsync*/
		}//wait();
	}
	pspDebugScreenClear();
}


/* Exit callback */
static int exit_callback(int arg1, int arg2, void *common)
{
	newstate(ST_GAME_QUIT,0,1);//*****追加：更なる231氏のアドバイスによりこの行を追加:090103
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
    if(thid >= 0)
    {
        sceKernelStartThread(thid, 0, 0);
    }
    return thid;
}

int main(int argc, char *argv[])
{
	SetupCallbacks();			//幼稚園氏のコピペ
	scePowerSetClockFrequency(333,333,166);
	pspDebugScreenInit();
	printF("Starting K.E.T.M.\n");
	
	if(ini_load()==-1)	//090110
	{
		chooseModDir();
		keyconfig.si= KEY_BOMB;		//□
		keyconfig.ma= KEY_CANCEL;	//○
		keyconfig.ba= KEY_SHOT;		//×
		keyconfig.sa= KEY_SC_SHOT;	//△
		keyconfig.u = KEY_UP;		//↑
		keyconfig.d = KEY_DOWN;		//↓
		keyconfig.l = KEY_LEFT;		//←
		keyconfig.r = KEY_RIGHT;	//→
		keyconfig.rt= KEY_SLOW;		//R
		keyconfig.lt= KEY_BOMB;		//L
		keyconfig.sl= KEY_NONE;		//SELECT
		keyconfig.st= KEY_PAUSE;	//START
	}

	game_init(argc, argv);
	playMusic("intro");
	while (state.mainstate!=ST_GAME_QUIT) {
		keyboard_poll();

		switch(state.mainstate) {
			case ST_START_INTRO:
				if(state.newstate) {
					startintro_init();
					state.newstate=0;
				}
				startintro_work();
				break;
			case ST_INTRO:
				if(state.newstate) {
					intro_init();
					state.newstate=0;
				}
				intro_work();
				break;
			case ST_GAME_PLAY:
				if(state.newstate) {
					playChunk(1);
					playMusic("stage1");
					thegame_init();
					state.newstate=0;
				}
				thegame_work();
				break;
			case ST_GAME_OVER:
				if(state.newstate) {
					//playChunk(4);		//***090123		コメントアウト
					stopMusic();
					gameover_init();
					playMusic("intro");
					state.newstate=0;
				}
				gameover_work();
                                //newstate(ST_INTRO,0,1);
				break;
			case ST_MENU:
				if(state.newstate) {
					menu_init();
					state.newstate=0;
				}
				menu_work();
				break;
			case ST_SHOW_HCLIST:
				if(state.newstate) {
					hsc_show_init();
					state.newstate=0;
				}
				hsc_show_work();
				break;
			case ST_ENTRY_HCLIST:
				if(state.newstate) {
					hsc_entry_init();
					state.newstate=0;
				}
				hsc_entry_work();
				break;
			case ST_KEY_CONFIG: 
				if(state.newstate) { 
					key_config_init(); 
					state.newstate=0; 
				} 
				key_config_work(); 
				break;
			case ST_PLAYER_SELECT:
				if(state.newstate) { 
					player_opt_init(); 
					state.newstate=0; 
				}
				player_opt_work();
		}
		
		//fps_show();
		SDL_Flip(screen);
		//SDL_UpdateRect(screen, 0, 0, 0, 0);
		fps_newframe();
	}

	/* TODO: Free everything (memory, SDL_Surfaces, Joysticks...) */

	hsc_save();

	//fprintf(stdout,"Thank you for playing\n");
	SDL_Quit();			//231氏の助言を受けて加えるもhome終了バグは解決せず。知識不足。
	sceKernelExitGame();
	return(0);
}

