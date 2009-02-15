#include "bg.h"

extern double fps_factor;
extern SDL_Surface *screen;
extern SPRITE *player;

/*static*/ double bg_alpha;

int is_bg_add;	//[***090201		追加:追加フラグ
int is_bg_end;	//[***090201		追加:前背景終了フラグ
int is_bg_fin;	//[***090202		追加:背景非ループフラグ
int n_bg;		//[***090209		追加:追加背景番号

/* clouds */
static SPRITE *w1[5];
static SPRITE *w2[10];
static SPRITE *w3[20];

/* tile */
static SDL_Surface *btile1=NULL;
static SDL_Surface *btile2=NULL;		//[***090201		追加
static double btile_y;

static int akt_bgtype;


void tile_add(/*int lev*/)		//[***090201		追加
{
	char filename[32/*20*/];
	int lev = player_now_stage;
	sprintf(filename,"back%d_a-%d.jpg",lev,n_bg);
	btile2=loadbmp(filename);
	is_bg_add=1;		//二枚目の背景追加フラグ
}


static void tile_init(/*int lev*/)
{
	char filename[32/*20*/];
	int lev = player_now_stage;
	sprintf(filename,"back%d.jpg",lev);
	//if (btile1==NULL) {
		btile1=loadbmp(filename);
	//}
	btile_y=WIDTH2;
}

typedef struct
{
	double speed_base;
	double speed_rand;
} CLOUDS_DATA;

static void clouds_mover(SPRITE *c)
{
	CLOUDS_DATA *d=(CLOUDS_DATA *)c->data;
	c->y+=d->speed_base*fps_factor;
	c->y+=d->speed_rand*fps_factor;
	if (c->y>HEIGHT) {
		c->y-=HEIGHT+c->w;
		d->speed_rand=(double)(rand()%100)/100;
		c->x=rand()%WIDTH2-c->w;
		//c->alpha=bg_alpha;
		c->alpha=200;
	}
}

static void clouds_init(/*int lev*/)
{
	int i;
	CLOUDS_DATA *d;
	char filename[32/*20*/];
	int lev = player_now_stage;
	sprintf(filename,"wolke03_%d.png",lev);

	for (i=0;i<5;i++) {
		w3[i]=sprite_add_file(filename,1,PR_BACK0);
		w3[i]->x=rand()%WIDTH2-w3[i]->w;
		w3[i]->y=rand()%HEIGHT;
		w3[i]->flags&=~SP_FLAG_COLCHECK;
		w3[i]->flags|=SP_FLAG_VISIBLE;
		w3[i]->type=SP_ETC;
		w3[i]->mover=clouds_mover;
		d=mmalloc(sizeof(CLOUDS_DATA));
		w3[i]->data=d;
		w3[i]->alpha=bg_alpha;
		d->speed_base=1;
		d->speed_rand=(double)(rand()%100)/100;
	}

	sprintf(filename,"wolke02_%d.png",lev);
	for (i=0;i<2;i++) {
		w2[i]=sprite_add_file(filename,1,PR_BACK1);
		w2[i]->x=rand()%WIDTH2-w2[i]->w;
		w2[i]->y=rand()%HEIGHT;
		w2[i]->flags&=~SP_FLAG_COLCHECK;
		w2[i]->flags|=SP_FLAG_VISIBLE;
		w2[i]->type=SP_ETC;
		w2[i]->mover=clouds_mover;
		d=mmalloc(sizeof(CLOUDS_DATA));
		w2[i]->data=d;
		w2[i]->alpha=bg_alpha;
		d->speed_base=2;
		d->speed_rand=(double)(rand()%100)/100;
	}

	sprintf(filename,"wolke01_%d.png",lev);
	for (i=0;i<1;i++) {
		w1[i]=sprite_add_file(filename,1,PR_BACK2);
		w1[i]->x=rand()%WIDTH2-w1[i]->w;
		w1[i]->y=rand()%HEIGHT;
		w1[i]->flags&=~SP_FLAG_COLCHECK;
		w1[i]->flags|=SP_FLAG_VISIBLE;
		w1[i]->type=SP_ETC;
		w1[i]->mover=clouds_mover;
		d=mmalloc(sizeof(CLOUDS_DATA));
		w1[i]->data=d;
		w1[i]->alpha=bg_alpha;
		d->speed_base=3;
		d->speed_rand=(double)(rand()%100)/100;
	}
}



void bg_init(int bg_type/*,int lev*/)
{
	if ((bg_type>=BG_LAST)||(bg_type<BG_BLACK))
		error(ERR_FATAL,"unknown BG_TYPE: %d",bg_type);

	akt_bgtype=bg_type;
	bg_alpha=0;
	switch (bg_type)
	{
	case BG_BLACK:												break;
	case BG_CLOUDS: 	tile_init(/*lev*/); 	clouds_init(/*lev*/);	break;
	case BG_STARS:		/* stars_init(); */ 					break;
	case BG_TILE:		tile_init(/*lev*/); 						break;
	}
}

static void tile_work(void)
{
	if (!is_bg_fin)	//非ループフラグが立ってないとき(通常ルート)
	{
		btile_y+=fps_factor;
		if (is_bg_end)		//背景追加命令が出てから前背景が終端まで来たとき
		{
			if (btile_y>btile2->h-1)
			{
				btile_y-=btile2->h;
			}
			else if (btile_y>HEIGHT && btile1!=NULL) //下準備
			{
				unloadbmp_by_surface(btile1);
				btile1=NULL;
				unloadbmp_by_surface(btile2);
				btile2=NULL;
				char filename[32/*20*/];
				sprintf(filename,"back%d_a-%d.jpg",player_now_stage/*level*/, n_bg);
				btile1=loadbmp(filename);
				is_bg_add=0;
				is_bg_end=0;
				n_bg++;
			}
		}
		else	//特殊な命令が出てないときor背景追加命令が出てから前背景が終端まで来てないとき
		{
			if (btile_y>btile1->h-1)	//btile_yが前背景と同じ高さになったとき
			{
				if (is_bg_add){		//背景追加命令が出たとき
					btile_y=0;
					is_bg_end=1;		//前背景終了フラグ
				}
				else{		//通常処理
					btile_y-=btile1->h;
				}
			}
		}
	}
	else{		//非ループの場合 ※今の所機能してないかもしれない
		if (is_bg_end){
			if (btile_y+fps_factor<=btile2->h-1){
				btile_y+=fps_factor;
			}
		}
		else{
			if (btile_y+fps_factor<=btile1->h-1){
				btile_y+=fps_factor;
			}
		}
	}
}
static void tile_display(void)
{
	int i,j;
	SDL_Rect r;
	SDL_Rect r2;
	if (bg_alpha<255) {
		SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
		SDL_SetAlpha(btile1,SDL_SRCALPHA,bg_alpha);
	} else
		SDL_SetAlpha(btile1,SDL_SRCALPHA,255);
	if (is_bg_add || is_bg_end){
		SDL_SetAlpha(btile2,SDL_SRCALPHA,bg_alpha);
		r2.w=btile2->w;
		r2.h=btile2->h;
	}

	r.w=btile1->w;
	r.h=btile1->h;

	if (is_bg_end)
	{
		for (i=0;i<WIDTH2;i+=btile2->w){
			r2.x=i;
			r2.y=-btile2->h+btile_y;
			SDL_BlitSurface(btile2,NULL,screen,&r2);
		}
		for (i=0;i<WIDTH2;i+=btile1->w) {
			for (j=btile_y;j<HEIGHT;j+=btile1->h) {
				r.x=i;
				r.y=j;
				SDL_BlitSurface(btile1,NULL,screen,&r);
			}
		}
	}
	else
	{
		for (i=0;i<WIDTH2;i+=btile1->w) {
			for (j=-btile1->h;j<HEIGHT;j+=btile1->h) {
				r.x=i;
				r.y=j+btile_y;
				SDL_BlitSurface(btile1,NULL,screen,&r);
			}
		}
	}
}

void bg_work()
{
	if (akt_bgtype<0) {
		error(ERR_WARN,"bg_work with no bg_type");
		return;
	}

	if (bg_alpha<255)
		bg_alpha+=6*fps_factor;
	else
		bg_alpha=255;

	switch (akt_bgtype) {

	case BG_BLACK:
		SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
		break;

	case BG_CLOUDS:
		// SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0xff));
		//SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
		//SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,bg_alpha));
		tile_work();
		tile_display();
		break;

	case BG_STARS:
		SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
		break;

	case BG_TILE:
		tile_work();
		tile_display();
		break;
	}
}

static void clouds_remove()
{
	int i;
	for (i=0;i<5;i++)
	{
		if (i<1) w1[i]->type=-1;
		if (i<2) w2[i]->type=-1;
		w3[i]->type=-1;
	}
}
static void tile_remove()
{
	if (btile1!=NULL)
	{	unloadbmp_by_surface(btile1);}
	btile1=NULL;

	if (btile2!=NULL)
	{	unloadbmp_by_surface(btile2);}
	btile2=NULL;
}

void bg_destroy()
{
	if (akt_bgtype<0)
	{
		error(ERR_WARN,"bg_destroy with no bg_type");
		return;
	}
	switch (akt_bgtype)
	{
	case BG_BLACK:								break;
	case BG_CLOUDS: 	clouds_remove();		break;
	case BG_STARS:		/* stars_remove();*/	break;
	case BG_TILE:		tile_remove();			break;
	}
	akt_bgtype=-1;
}
