#include "enemy.h"

#ifdef ENABLE_PSP
	//# /* カスタムライブラリを使う */
	#include "SDL_image.h"
#else
	//# /* 標準ライブラリを使う */
	#include <SDL/SDL_image.h>
#endif

extern double fps_factor;

/* not a real enemy - just a line of text */

typedef struct
{
#if 1
	ENEMY_BASE b;		//[***090410		???追加???
#endif
	int state;
	double speed;
	double destx;
	double desty;
	double wait;
} GAMETEXT_DATA;

typedef struct
{
#if 1
	ENEMY_BASE b;		//[***090410		???追加???
#endif
	int state;
	double wait;
} GAMEIMG_DATA;

static void enemy_gametext_mover(SPRITE *s)
{
	GAMETEXT_DATA *g=(GAMETEXT_DATA *)s->data;
//	SDL_Surface *sur;
	switch (g->state)
	{
	case 0:
		s->x-=g->speed*fps_factor;
		if (s->x<=g->destx)
		{
			g->wait=40;
			g->state=1;
		}
		break;
	case 1:
		g->wait-=fps_factor;
		if (g->wait<=0)
		{
			g->wait=20;
			g->state=2;
		}
		break;
	case 2:
		g->wait-=fps_factor;
		s->alpha=55+g->wait*10;
		if (g->wait<=0)
		{	g->state=3;}
		break;
	case 3:
		s->type=SP_DELETE;
		break;
	}
}

static void enemy_gameimg_mover(SPRITE *s)
{
	GAMEIMG_DATA *g=(GAMEIMG_DATA *)s->data;
	switch (g->state)
	{
	case 0:
		{int aaa;aaa = s->alpha; aaa += fps_factor*3;
		if (/*s->alpha*/aaa<=255)
		{
			s->alpha=255;
			g->wait=100;
			g->state=1;
		}else{s->alpha=aaa;}}
		break;
	case 1:
		g->wait-=fps_factor;
		if (g->wait<=0)
		{
			g->state=2;
		}
		break;
	case 2:
		s->alpha-=fps_factor;
		if (s->alpha<=0)
		{
			s->alpha=0;
			g->state=3;
		}
		break;
	case 3:
		s->type=SP_DELETE;
		break;
	}
}

/* ゲーム中イベントメッセージ(英語)表示 */
void enemy_gametext_add(char *text, int ypos)
{
	SDL_Surface *sur;
	sur 			= font_render(text,FONT04);
	SPRITE *s;
	s				= sprite_add(sur,1,PR_BONUS,1);
	s->flags		|=SP_FLAG_FREESURFACE|SP_FLAG_VISIBLE;
	s->type 		= SP_ETC;
	s->x			= GAME_WIDTH;		//ウィンドウ幅の変更
	s->y			= ypos;
	s->mover		= enemy_gametext_mover;
	GAMETEXT_DATA *data;
	data			= mmalloc(sizeof(GAMETEXT_DATA));
	s->data 		= data;
#if 1
	data->b.score	= score(200*2); 		//[***090410		???追加???
	data->b.health	= 25+(difficulty<<2);	//[***090410		???追加???
#endif
	data->destx 	= (GAME_WIDTH/2)-sur->w/2;		//ウィンドウ幅の変更
	data->desty 	= ypos;
	data->speed 	= 5;
	data->wait		= 0;
	data->state 	= 0;
}

void enemy_gameimg_add(char *filename,int xpos, int ypos)
{
	SDL_Surface *surface;
	SDL_Surface *tmp;
	char fn[64/*50*/];
	strcpy(fn,moddir);
	strcat(fn,"/");
	strcat(fn,filename);
	surface = IMG_Load(fn);
	surface = SDL_DisplayFormatAlpha(surface);
	tmp=SDL_CreateRGBSurface(
			SDL_SRCCOLORKEY|SDL_SWSURFACE,
			surface->w,
			surface->h,
			screen->format->BitsPerPixel,
			screen->format->Rmask,
			screen->format->Gmask,
			screen->format->Bmask,
			screen->format->Amask);
	SDL_SetColorKey(tmp,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	SDL_BlitSurface(surface,NULL,tmp,NULL);
	SDL_FreeSurface(surface);
//
	SPRITE *s;
	s			= sprite_add(tmp, 1, PR_BONUS, 1);
	s->flags	|=SP_FLAG_FREESURFACE|SP_FLAG_VISIBLE;
	s->type 	= SP_ETC;
	s->x		= xpos;
	s->y		= ypos;
	s->alpha	= 0;
	s->mover	= enemy_gameimg_mover;
	GAMEIMG_DATA/*GAMETEXT_DATA*/ *data;
	data		= mmalloc(sizeof(GAMEIMG_DATA));
	s->data 	= data;
#if 1
	data->b.score	= score(200*2); 		//[***090410		???追加???
	data->b.health	= 25+(difficulty<<2);	//[***090410		???追加???
#endif
	data->wait		= 0;
	data->state 	= 0;
}
