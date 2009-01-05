#include "fps.h"

Uint32 fps_gamestart;
Uint32 fps_lasttime;
Uint32 fps_frames;
double fps_factor;
int fps;
int fps_method;
int fps_display;

#define AVG_FRAMES 5 
double fps_factor_avg[AVG_FRAMES];
int fps_factor_avg_idx;

extern GAMESTATE state;
extern SDL_Surface *screen;
extern difficulty;
void fps_init()
{
	int i;

	fps_gamestart=SDL_GetTicks();
	fps_lasttime=fps_gamestart;
	fps_frames=0;
	fps_factor=1;
	fps_method=MAXFPS;
	fps_display=0;
	for(i=0;i<AVG_FRAMES;i++)
		fps_factor_avg[i]=1;
	fps_factor_avg_idx=0;
	
}

void fps_show()
{
/*
	char fpstxt[9];
	SDL_Surface *t;
	SDL_Rect r;
	static int fps_index=0;
	static int fps_array[50];
	int fps_avg;
	static int fps_avg_old=0;
	int i;

	if((fps_display) && (state.mainstate==ST_GAME_PLAY)) {
		if(fps_method==MAXFPS) {
			fps_array[fps_index]=fps;
			fps_avg=0;
			for(i=0;i<50;i++) {
				fps_avg+=fps_array[i];
			}
			fps_avg/=50;
			if(++fps_index>49) {
				fps_index=0;
				fps_avg_old=fps_avg;
			}
			sprintf(fpstxt,"%03d FPS",fps_avg_old);
		} else {
			sprintf(fpstxt,"- FIX -");
		}

		t=font_render(fpstxt,FONT07);
		r.x=screen->w-t->w;
		r.y=screen->h-t->h;
		r.w=t->w;
		r.h=t->h;
		SDL_BlitSurface(t,NULL,screen,&r);
		SDL_FreeSurface(t);
	}
*/
}

void toggle_fps()
{
	if(fps_method==MAXFPS) {
		fps_method=WAITFORFRAME;
		fps_factor=1;
	} else {
		fps_method=MAXFPS;
		fps_factor=1;
	}
}

void fps_newframe()
{
	static Uint32 nexttime=0;
	static Uint32 now;
	int i;

	fps_frames++;
	fps_lasttime=now;
	now=SDL_GetTicks();

		fps_factor_avg[fps_factor_avg_idx++]=(double)(now-fps_lasttime)/(TICK_INTERVAL-difficulty*3);
		if(fps_factor_avg_idx==AVG_FRAMES)
			fps_factor_avg_idx=0;
		fps_factor=0;
		for(i=0;i<AVG_FRAMES;i++)
			fps_factor+=fps_factor_avg[i];
		fps_factor/=AVG_FRAMES;
		fps=1/((double)(now-fps_lasttime)/1000);
			
}
