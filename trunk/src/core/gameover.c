#include "gameover.h"

extern SDL_Surface *screen;
extern GAMESTATE state;
extern HSC_LIST hsc_table[];
extern double fps_factor;
extern GAMESTATE state;
extern int lastscore;


static SDL_Surface *go_surface1;
static SDL_Surface *go_surface2;

//int go_ex1,go_ex2;
#include "player.h"

enum _gameover_states
{
	GO_INIT=0,
	GO_FADEINIT,
	GO_FADEIN1,
	GO_FADEIN2,
	GO_WAIT,
	GO_QUIT
};

void gamecore_term(void)
{
	bg_destroy();
	controller_remove_all();
	sprite_remove_all(SP_SHOW_ALL);
//	parsys_remove_all();
//	score_cleanup();
}
void gameover_init(void)
{
	gamecore_term();
//
	//stopMusic();
	play_music(BGM_00_intro);
//
				//	//playChunk(4); 	//[***090123		コメントアウト
				//	stopMusic();	コメントアウト
				//	play_music(BGM_00_intro);	コメントアウト

	go_surface1=font_render("GAME OVER",FONT05);
	{	char scoretext[32/*50*/];
		sprintf(scoretext,"SCORE: %d0",lastscore);
		go_surface2=font_render(scoretext,FONT05);
	}
	#if 1
	/* KETM互換なら ここで back buffer screen を clear screen すべき */
	psp_push_screen();
	#endif
	SDL_SetAlpha(screen,SDL_SRCALPHA,128);
	psp_push_screen();	//SDL_BlitSurface(screen,NULL,back_screen,NULL);
	SDL_SetAlpha(screen,SDL_SRCALPHA,255);
	newstate(ST_GAME_OVER,GO_FADEINIT,0);
}

static void gameover_display(double s1, double s2)
{
	SDL_Rect s,d;
	s.x=0;
	s.y=0;

	s.w=go_surface1->w;
	s.h=go_surface1->h;
	d.w=go_surface1->w*s1;
	d.h=go_surface1->h*s1;
	d.x=GAME_WIDTH/2-d.w/2; 		//[***090203
	d.y=GAME_HEIGHT/2-d.h/2-30;		//[***090203
	blit_scaled(go_surface1,&s,screen,&d);

	s.w=go_surface2->w;
	s.h=go_surface2->h;
	d.w=go_surface2->w*s2;
	d.h=go_surface2->h*s2;
	d.x=GAME_WIDTH/2-d.w/2; 		//[***090203
	d.y=GAME_HEIGHT/2-d.h/2+30;		//[***090203
	blit_scaled(go_surface2,&s,screen,&d);
}
void gameover_work(void)
{
//	SDL_Surface *tmpsurface;
//	SDL_Rect s,d;
	static int wait=0;
	static double go_size1, go_size2;

	if (state.mainstate!=ST_GAME_OVER || state.newstate==1) return;
	psp_pop_screen();	//SDL_BlitSurface(back_screen,NULL,screen,NULL);

	switch (state.substate)
	{
	case GO_FADEINIT:
		go_size1=0;
		go_size2=0;
		newstate(ST_GAME_OVER,GO_FADEIN1,0);
		break;

	case GO_FADEIN1:
		gameover_display(go_size1,go_size2);
		go_size1+=0.05*fps_factor;
		if (go_size1>=1.5) {
			newstate(ST_GAME_OVER,GO_FADEIN2,0);
		}
		break;

	case GO_FADEIN2:
		gameover_display(go_size1,go_size2);
		go_size2+=0.05*fps_factor;
		if (go_size2>=1.0) {
			wait=200;
			newstate(ST_GAME_OVER,GO_WAIT,0);
		}
		break;

	case GO_WAIT:
		gameover_display(go_size1,go_size2);
		wait-=fps_factor;
		if (wait<=0) {
			newstate(ST_GAME_OVER,GO_QUIT,0);
		}
		break;

	case GO_QUIT:
		//SDL_FreeSurface(go_surface1);
		//SDL_FreeSurface(go_surface2);
		//SDL_FreeSurface(back_screen);
		if (lastscore > hsc_table[4].score)
		{
			/* you made it! enter your name in the hiscore-list */
			newstate(ST_ENTRY_HCLIST,0,1);
		}
		else
		{
			/* you'd better play barbie */
			newstate(ST_MENU/*ST_INTRO*/,0,1);
		}
		break;
	}
}

