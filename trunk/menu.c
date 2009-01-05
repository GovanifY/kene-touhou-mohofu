#include "menu.h"

extern int keyboard[];
extern SDL_Surface *screen;
extern GAMESTATE state;
extern int volume;
extern int difficulty;
extern KEYCONFIG keyconfig;
extern double fps_factor;
extern int fps_display;
extern Uint32 videoflags;
extern int fps_method;

MENU startmenu;
MENU pausemenu;
MENU optionmenu;
MENU volumemenu;
MENU difficultymenu;
MENU player_sl;

/*
typedef struct {
	SPRITE *opts[20][5];
	SDL_Surface *opts_surface[20];		//画像化した各項目のアドレス
	SDL_Surface *bg;
	int active_item;
	int select_finish;
	int nopt;		//オプションの項目数を数えてる
	int alpha;
	int alphadir;		//各項目のα値の状態。フェードin or out or done
	int fadeout;
	int timeout;
} MENU;

typedef struct _sprite {
	int id;				// スプライトID
	SDL_Surface *img;		// Images
	COLMAP_CACHE *cmap;		// Zeiger auf Col-Map-Cache entry
	int type;			// Sprite-Typ, (siehe enum SPRITE_TYPE), -1=remove
	Uint8 priority;			// Prioritaet (0: unten, 9 oben)
	Uint8 frames;			// Anzahl Frames
	Sint8 anim_speed;		// Geschw. der Animation (negative Werte: Anim l舫ft r・kw舐ts
	Uint8 alpha;			// 255: opak, -  0: durchsichtig
	Uint8 flags;			// siehe unten (SP_FLAG...)
	int aktframe;			// akt. Frame
	double anim_count;
	int w,h;			// Breite, He
	int ticks;			// number of ticks since creation
	double x,y;			// akt. Position
	void *data;			// Zeiger auf Strukur mit zus舩zlichen Daten
	void (*mover)(struct _sprite *);// Custom-Move-Routine
	struct _sprite *next;
} SPRITE;

typedef struct {	//たぶん矩形データ用
	int i0;
	int i1;
	int i2;
	int i3;
} MENU_DATA;
*/

Uint32 pause_start_time=0;

void menusystem_init()
{
	startmenu.active_item=0;
	pausemenu.active_item=0;
	optionmenu.active_item=0;
	volumemenu.active_item=0;
	difficultymenu.active_item=0;
	player_sl.active_item=0;
}

void menu_init()
{
	switch(state.substate) {
		case MEN_START:
			startmenu_init();
			break;
		case MEN_PAUSE:
			pausemenu_init();
			break;
		case MEN_OPTION:
			optionmenu_init();
			break;
		case MEN_VOLUME:
			volumemenu_init();
			break;
		case MEN_DIFF:
			difficultymenu_init();
			break;
		case MEN_PLAYERS:
			player_sl_init();
			break;
	}
}

void menu_work()
{
	switch(state.substate) {
		playChunk(2);
		case MEN_START:
			startmenu_work();
			break;
		case MEN_PAUSE:
			pausemenu_work();
			break;
		case MEN_OPTION:
			optionmenu_work();
			break;
		case MEN_VOLUME:
			volumemenu_work();
			break;
		case MEN_DIFF:
			difficultymenu_work();
			break;
		case MEN_PLAYERS:
			player_sl_work();
			break;
	}
}
	
	
void startmenu_init()
{
	char *options[]={"NEW GAME", "STORY", "OPTIONS", "QUIT GAME", NULL};		//ちょっと変更
	genericmenu_init(options,&startmenu,0,500);
}

void startmenu_work()
{


	if(!startmenu.select_finish) {
		genericmenu_work(&startmenu);
	} else {
		if(startmenu.timeout==-3) {
			/* Timeout, go on and show HighScore List */
			newstate(ST_SHOW_HCLIST,0,1);
		} else {
			startmenu.select_finish=0;
			switch(startmenu.active_item) {

				case 0: /* New Game */
					newstate(ST_GAME_PLAY,0,1);
					break;

				case 1:	/* Options */
					newstate(ST_SHOW_HCLIST,0,1);
					break;

				case 2: /* Hiscore */
					newstate(ST_MENU,MEN_OPTION,1);
					break;

				case 3: /* Quit */
					newstate(ST_GAME_QUIT,0,1);
					break;
			}
		}
	}

}

void pausemenu_init()
{
	char *options[]={"CONTINUE GAME", "QUIT GAME", NULL};
	genericmenu_init(options,&pausemenu,1,-1);
	pause_start_time=SDL_GetTicks();
}

void pausemenu_work()
{
	if(!pausemenu.select_finish) {
		genericmenu_work(&pausemenu);
	} else {
		pausemenu.select_finish=0;
		switch(pausemenu.active_item) {
			case 0: /* Continue Game */
				newstate(ST_GAME_PLAY,0,0);
				adjust_start_time(pause_start_time);
				break;
			case 1:	/* Quit Game */
				 stopMusic();
				sprite_remove_all(SP_SHOW_ALL);
				//parsys_remove_all();
				newstate(ST_INTRO,0,1);
				playMusic("intro");
				break;
		}
	}
}

void optionmenu_init()
{
	char *m00="FX VOLUME";
	char *m01="DIFFICULTY";
	char *m02="HIGH SCORE";		//変更
	char *m03="BACK TO MAIN MENU";

	char *o[8];

	o[0]=m00;
	o[1]=m01;
	o[2]=m02;
	o[3]=m03;
	o[4]=NULL;
	
	genericmenu_init(o,&optionmenu,0,-1);
}

void optionmenu_work()
{
	if(!optionmenu.select_finish) {
		genericmenu_work(&optionmenu);
	} else {
		optionmenu.select_finish=0;
		switch(optionmenu.active_item) {
			playChunk(2);
			case 0: /* change Sound-FX Volume  */
				newstate(ST_MENU,MEN_VOLUME,1);
				break;
			case 1:	/* difficulty */
				newstate(ST_MENU,MEN_DIFF,1);
				break;
			case 2:	/* Hiscore */
				newstate(ST_SHOW_HCLIST,0,1);
				break;
			case 3:	/* Back to Game */
				newstate(ST_MENU,MEN_START,1);
				break;
		}
	}
}

void volumemenu_init()
{
	char *options[]={"0","1","2","3",NULL};
	genericmenu_init(options,&volumemenu,0,-1);
	volumemenu.active_item=volume;
}

void volumemenu_work()
{
	if(!volumemenu.select_finish) {
		genericmenu_work(&volumemenu);
	} else {
		volumemenu.select_finish=0;
		volume=volumemenu.active_item;
		setChunkVolume(volume*40);
		newstate(ST_MENU,MEN_OPTION,1);
	}
}

void difficultymenu_init()
{
	char *options[]={"EASY", "MEDIUM", "HARD",NULL};
	genericmenu_init(options,&difficultymenu,0,-1);
	difficultymenu.active_item=difficulty;
}

void difficultymenu_work()
{
	if(!difficultymenu.select_finish) {
		genericmenu_work(&difficultymenu);
	} else {
		difficultymenu.select_finish=0;
		difficulty=difficultymenu.active_item;
		newstate(ST_MENU,MEN_OPTION,1);
	}
}



void player_sl_init()
{
	
	
}

void player_sl_work()
{
	
}

void genericmenu_init(char *options[], MENU *m, int fadeout, int timeout)
{
	int i,j;
	MENU_DATA *d;

	for(i=0;i<20;i++) {
		m->opts_surface[i]=NULL;
		for(j=0;j<5;j++) { m->opts[i][j]=NULL; }
	}
	m->bg=NULL;
	m->nopt=0;
	m->select_finish=0;		//ここまでMENUの初期化

	while(options[m->nopt]!=NULL) {
		// m->opts_surface[m->nopt]=font_render(options[m->nopt],FONT01);
		m->opts_surface[m->nopt]=font_render(options[m->nopt],FONT06);
		for(i=0;i<5;i++) {			//各スプライトに情報を与えてていく
			m->opts[m->nopt][i]=sprite_add(m->opts_surface[m->nopt],1,i+5,1);
			d=mmalloc(sizeof(MENU_DATA));
			m->opts[m->nopt][i]->data=d;
			m->opts[m->nopt][i]->type=SP_MENUTEXT;
			m->opts[m->nopt][i]->alpha=0;
			m->opts[m->nopt][i]->x=0;
			m->opts[m->nopt][i]->y=0;
		}
		m->nopt++;
	}
	for(i=0;i<m->nopt;i++) {
		for(j=0;j<5;j++){
			d=(MENU_DATA *)m->opts[i][j]->data;
			d->i0=(screen->w/2)-(m->opts[i][j]->w/2);
			d->i1=(screen->h/2+40)-((m->opts[i][j]->h+5)*m->nopt/2)+i*(m->opts[i][j]->h+5);
			d->i2=0;
			d->i3=0;
		}
	}

	m->bg=SDL_CreateRGBSurface(SDL_HWSURFACE,screen->w,screen->h,
		screen->format->BitsPerPixel,
		screen->format->Rmask,
		screen->format->Gmask,
		screen->format->Bmask,
		screen->format->Amask);
	SDL_BlitSurface(screen,NULL,m->bg,NULL);

	m->alpha=0;
	m->alphadir=0;
	m->fadeout=fadeout;
	m->timeout=timeout;

	keyboard_clear();

}

void genericmenu_work(MENU *m)
{
	static int w=10;
	static double angle=0;
	int i,j;
	MENU_DATA *d;


	if(!m->select_finish) {

		if(m->timeout>0)
			m->timeout-=fps_factor;
			// m->timeout-=1;

		if(m->fadeout) {
			SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
			SDL_SetAlpha(m->bg,SDL_SRCALPHA,255-(m->alpha/2));
		}
		SDL_BlitSurface(m->bg,NULL,screen,NULL);

		if((w<=0) && (m->alphadir<2)) {
			if(keyboard[keyconfig.d]) {
				playChunk(2);
				if(m->active_item==m->nopt-1)
					m->active_item=0;
				else
					m->active_item++;
				w=10/fps_factor;
				m->timeout=-1;
			}
			if(keyboard[keyconfig.u]) {
				playChunk(2);
				if(!m->active_item)
					m->active_item=m->nopt-1;
				else
					m->active_item--;
				w=10/fps_factor;;
				m->timeout=-1;
			}
			if((state.substate==MEN_PAUSE)&&(keyboard[keyconfig.e])) {
				playChunk(2);
				pausemenu.active_item=0;
				m->alphadir=2;
				m->timeout=-1;
			}
		} else w--;

		switch(m->alphadir) {
			case 0: /* Increasing alpha, fade menu in */
				m->alpha+=8*fps_factor;
				if(m->alpha>=255) {
					m->alphadir=1;
					m->alpha=255;
				}
				break;
			/* case 1: fadein complete */
			case 2:
				/* selection done, decreasing alpha, fade menu out */
				m->alpha-=8*fps_factor;
				if(m->alpha<=0) {
					m->alphadir=3;
					m->alpha=0;
				}
				break;
			case 3:
				/* fadout fininshed, menu done */
				SDL_SetAlpha(m->bg,SDL_SRCALPHA,255);
				SDL_BlitSurface(m->bg,NULL,screen,NULL);
				//SDL_FreeSurface(m->bg);
				for(i=0;i<m->nopt;i++) {
					for(j=0;j<5;j++) {
						m->opts[i][j]->type=-1;
					}
					//SDL_FreeSurface(m->opts_surface[i]);
				}
				if(m->timeout==-2) m->timeout=-3;
				m->select_finish=1;
				w=10/fps_factor;
				return;
		}

		angle+=15*fps_factor;
		// angle%=360;
		if(angle>=360)
			angle-=360;

		for(i=0;i<m->nopt;i++) {
			d=(MENU_DATA *)m->opts[i][0]->data;
			if(i==m->active_item) {
				d->i2=12;
			} else {
				if(d->i2 && (!d->i3)) {
					d->i2--;
					d->i3=3;
				}
			}
			if(d->i3)
				d->i3--;
			m->opts[i][0]->x=d->i0+(cos(degtorad(angle))*(double)d->i2);
			// m->opts[i][0]->y=d->i1+(sin(degtorad(angle))*(double)d->i2);
			m->opts[i][0]->y=d->i1;




			for(j=4;j>0;j--) {
				m->opts[i][j]->alpha=m->alpha;
				m->opts[i][j]->x=m->opts[i][j-1]->x;
				m->opts[i][j]->y=m->opts[i][j-1]->y;
				if((m->opts[i][j]->x>0)&&(m->opts[i][j]->y>0))
					m->opts[i][j]->flags|=SP_FLAG_VISIBLE;
			}
		}
		sprite_work(SP_SHOW_MENUTEXT);
		sprite_display(SP_SHOW_MENUTEXT);

		if(keyboard[keyconfig.f]) {
			playChunk(2);
			m->alphadir=2;
			m->timeout=-1;
		}
		if(m->timeout==0) {
			m->alphadir=2;
			m->timeout=-2;
		}
	}
	//parsys_display();
}
