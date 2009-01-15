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

//*****************************
//keyconfig用	***20080110-0115
int key_pos[12];		//数字=ボタン変数 並びは"key_bg.png"
int keys2[11]={KEY_NONE,KEY_SHOT,KEY_BOMB,KEY_SLOW,KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT,KEY_PAUSE,KEY_CANCEL,KEY_SC_SHOT};		//数字=機能定数 並びは"keylist.png"
SDL_Surface *key_sf[11];		//機能リストの画像"keylist.png"
SDL_Surface *key_bg;
SDL_Surface *key_set;
SPRITE *keyicon;
SPRITE *keyicon2;
SDL_Rect rect_k;
int select_key;
int select_key2;
int select_key_dir;		//menu画面内の状態
int bg_alpha;
int bg_level;			//menu画面の状態
//******************************
/*
	typedef struct {
		int u;	//上
		int d;	//下
		int l;	//左
		int r;	//右
		int ba;	//×
		int ma;	//○
		int sa;	//△
		int si;	//□
		int rt;	//R
		int lt;	//L
		int sl;	//SELECT
		int st;	//START
	} KEYCONFIG;
*/
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
		case K_CONFIG:
			key_config_init();
			break;
		case MEN_PLAYERS:
			player_opt_init();
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
		case K_CONFIG:
			key_config_work();
			break;
		case MEN_PLAYERS:
			player_opt_work();
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
	char *m03="KEY CONFIG";
	char *m04="BACK TO MAIN MENU";

	char *o[8];

	o[0]=m00;
	o[1]=m01;
	o[2]=m02;
	o[3]=m03;
	o[4]=m04;
	o[5]=NULL;
	
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
			case 3:	/* Key Config */
				newstate(ST_MENU,K_CONFIG,1);
				break;
			case 4:	/* Back to Game */
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
		ini_save();
	}
}


//***090110
//キーコンフィグ追加。
//これを追加するためにplayer.cやsupport.cのkeyconfig配列の指定方法も変更した。
//基本は絶対値指定(機能固定値)でsupport.cのkeypollの中のkeyconfigだけ配列指定番号に変数（keyconfig.xxx）を取る
//またkeypollに入る値は、0と1だけのままだとボタン設定上問題が出るのでビットで管理するようにした
void key_config_init()
{
	char *options_key[]={"NONE", "SHOT", "BOMB", "SLOW", "UP", "DOWN", "LEFT", "RIGHT", "PAUSE", "CANCEL", "SC SHOT",NULL};
	key_pos[0]=keyconfig.ma;
	key_pos[1]=keyconfig.ba;
	key_pos[2]=keyconfig.si;
	key_pos[3]=keyconfig.sa;
	key_pos[4]=keyconfig.u;
	key_pos[5]=keyconfig.d;
	key_pos[6]=keyconfig.l;
	key_pos[7]=keyconfig.r;
	key_pos[8]=keyconfig.rt;
	key_pos[9]=keyconfig.lt;
	key_pos[10]=keyconfig.sl;
	key_pos[11]=keyconfig.st;
	playMusic("stage5");
	int i=0;
	select_key=0;
	select_key_dir=0;
	bg_level=0;
	
	bg_alpha=0;
	keyicon=sprite_add_file("key_icon.png" , 12, PR_FRONT0);
	key_bg=loadbmp("key_bg.png");
	key_set=loadbmp("keylist.png");
	SDL_SetColorKey(key_set,SDL_SRCCOLORKEY,0x00000000);
	
	keyicon->type=SP_MENUTEXT;
	keyicon->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	keyicon->x=68;
	keyicon->y=49;
	keyicon->aktframe=0;
	keyicon->anim_speed=2;
	
	while(options_key[i]!=NULL) {
		key_sf[i]=font_render(options_key[i],FONT03);
		i++;
	}
	//SDL_SetAlpha(key_bg, SDL_SRCALPHA, 0);
	
	keyboard_clear();
}

void key_config_work()
{
	static int key_shot;		//存在確認用。1で存在。0で存在しない。存在しないとメニューを抜けれない。
	static int key_down;
	static int key_up;
	static int bg_wait;
	int i;
	
	SDL_BlitSurface(key_bg,NULL,screen,NULL);
	switch(bg_level)
	{
		case 0:
			if(bg_alpha<255)
				bg_alpha+=6*fps_factor;
			else
			{
				bg_alpha=255;
				bg_level=1;
				bg_wait=0;
			}
			SDL_SetAlpha(key_bg, SDL_SRCALPHA, bg_alpha);
			break;
		case 1:
			switch(select_key_dir)
			{
				case 0:		//第一層
					if(bg_wait==0)
					{
						if(keyboard[KEY_UP])		//上ボタン
						{
							if(select_key==0)
								select_key=13;
							else
								select_key--;
							bg_wait=10;
						}
						if(keyboard[KEY_DOWN])		//下ボタン
						{
							if(select_key==13)
								select_key=0;
							else
								select_key++;
							bg_wait=10;
						}
						if(keyboard[KEY_LEFT])		//左
						{
							select_key=0;
						}
						if(keyboard[KEY_RIGHT])		//右
						{
							select_key=13;
						}
						
						if(keyboard[KEY_CANCEL])		//キャンセルボタン
						{
							playChunk(4);
							keyicon->type=-1;
							bg_level=2;
							//key_bg=loadbmp("moon.jpg");
							bg_alpha=255;
							playMusic("intro");
						}
						
						if(keyboard[KEY_SHOT])		//ショットボタン
						{
							playChunk(2);
							if(select_key==13)		//OKを選んだ場合
							{
								key_shot=0;
								key_down=0;
								key_up=0;
								for(i=0;i<12;i++)	//ショットボタン&↑&↓があるか確認
								{
									if(key_pos[i]==KEY_SHOT)
										key_shot++;
									if(key_pos[i]==KEY_UP)
										key_up++;
									if(key_pos[i]==KEY_DOWN)
										key_down++;
								}
								if((key_shot!=0)&&((key_up!=0)&&(key_down!=0)))
								{
									keyicon->type=-1;
									bg_level=2;
									//key_bg=loadbmp("moon.jpg");
									bg_alpha=255;
									playMusic("intro");
									
									//最終的に代入される物
									keyconfig.ma=key_pos[0];
									keyconfig.ba=key_pos[1];
									keyconfig.si=key_pos[2];
									keyconfig.sa=key_pos[3];
									keyconfig.u= key_pos[4];
									keyconfig.d= key_pos[5];
									keyconfig.l= key_pos[6];
									keyconfig.r= key_pos[7];
									keyconfig.rt=key_pos[8];
									keyconfig.lt=key_pos[9];
									keyconfig.sl=key_pos[10];
									keyconfig.st=key_pos[11];
									ini_save();
								}
								else
									pspDebugScreenPrintf("damedayo\n");
							}
							else if(select_key==12)		//resetを選んだ場合
							{
								playChunk(7);
								bg_wait=10;
								key_pos[0]= KEY_CANCEL;
								key_pos[1]= KEY_SHOT;
								key_pos[2]= KEY_BOMB;
								key_pos[3]= KEY_SC_SHOT;
								key_pos[4]= KEY_UP;
								key_pos[5]= KEY_DOWN;
								key_pos[6]= KEY_LEFT;
								key_pos[7]= KEY_RIGHT;
								key_pos[8]= KEY_SLOW;
								key_pos[9]= KEY_NONE;
								key_pos[10]=KEY_NONE;
								key_pos[11]=KEY_PAUSE;
								select_key=13;
							}
							else		//キーの選択
							{
								select_key_dir=1;
							}
						}
					}
					else
						bg_wait--;
					
					keyicon->y=49+(select_key*15);
					
					break;
					
				case 1:		//第二層準備
					for(i=0;i<11;i++)
					{
						if(keys2[i]==key_pos[select_key])		//選択した所に何が入っているのか調べる
						{
							break;
						}
					}
					keyicon2=sprite_add_file("key_icon.png" , 12, PR_FRONT0);
					bg_alpha=0;
					keyicon2->type=SP_MENUTEXT;
					keyicon2->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
					keyicon2->x=295;
					keyicon2->y=40+(i*19);
					keyicon2->aktframe=0;
					keyicon2->anim_speed=1;
					select_key2=i;
					select_key_dir=2;
					break;
					
				case 2:
					if(bg_alpha<255)
						bg_alpha+=12*fps_factor;
					else
					{
						bg_alpha=255;
						select_key_dir=3;
					}
					
					rect_k.x=310;
					rect_k.y=36;
					SDL_SetAlpha(key_set, SDL_SRCALPHA, bg_alpha);
					SDL_BlitSurface(key_set,NULL,screen,&rect_k);
					break;
					
				case 3:		//第二層
					if(bg_wait==0)
					{
						if(keyboard[KEY_UP])
						{
							if(select_key2==0)
								select_key2=10;
							else
								select_key2--;
							bg_wait=10;
						}
						if(keyboard[KEY_DOWN])
						{
							if(select_key2==10)
								select_key2=0;
							else
								select_key2++;
							bg_wait=10;
						}
						if(keyboard[KEY_LEFT])
						{
							select_key2=0;
						}
						if(keyboard[KEY_RIGHT])
						{
							select_key2=10;
						}
						if(keyboard[KEY_CANCEL])
						{
							bg_wait=20;
							select_key_dir=0;
							keyicon2->type=-1;
						}
						if(keyboard[KEY_SHOT])
						{
							playChunk(2);
							bg_wait=20;
							for(i=0;i<12;i++)
							{
								if(key_pos[i]==keys2[select_key2])
									break;
							}
							key_pos[select_key]=keys2[select_key2];
							
							select_key_dir=0;
							keyicon2->type=-1;
						}
					}
					else
						bg_wait--;
					
					rect_k.x=310;
					rect_k.y=36;
					SDL_BlitSurface(key_set,NULL,screen,&rect_k);

					keyicon2->y=40+(select_key2*19);
					break;
			}
			for(i=0;i<12;i++)
			{
				kp_search(key_pos[i], i);
			}
			
			break;
		case 2:
			if(bg_alpha<255)
				bg_alpha+=8*fps_factor;
			else
			{
				//unloadbmp_by_surface(key_bg);
				bg_alpha=255;
				newstate(ST_INTRO,1,1);
				for(i=0;i<12;i++)
					SDL_FreeSurface(key_sf[i]);
			}
			//SDL_SetAlpha(key_bg, SDL_SRCALPHA, bg_alpha);
			break;
	}
	
	if(bg_level!=2)
	{
		sprite_work(SP_SHOW_MENUTEXT);
		sprite_display(SP_SHOW_MENUTEXT);
	}
}

void kp_search(int btn, int num)
{
	int i;
	for(i=0;i<11;i++)
		if(keys2[i]==btn)
			break;
	if(i==11)
		i=0;
	rect_k.x=150;
	rect_k.y=49+(num*15);
	SDL_BlitSurface(key_sf[i],NULL,screen,&rect_k);
}

void player_opt_init()
{
	
}

void player_opt_work()
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
			if(keyboard[KEY_DOWN]) {
				playChunk(2);
				if(m->active_item==m->nopt-1)
					m->active_item=0;
				else
					m->active_item++;
				w=10/fps_factor;
				m->timeout=-1;
			}
			if(keyboard[KEY_UP]) {
				playChunk(2);
				if(!m->active_item)
					m->active_item=m->nopt-1;
				else
					m->active_item--;
				w=10/fps_factor;;
				m->timeout=-1;
			}
			if((state.substate==MEN_PAUSE)&&(keyboard[KEY_PAUSE])) {		//ポーズメニューの場合はstartボタンでポーズ解除
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

		if(keyboard[KEY_SHOT]) {
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
