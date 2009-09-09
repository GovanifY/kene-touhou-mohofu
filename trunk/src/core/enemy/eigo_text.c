
#include "bullet_object.h"

/*---------------------------------------------------------
	演出用テキスト(英語)
	not a real enemy - just a line of text
	-------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
#if 1
	ENEMY_BASE b;		// [***090410		???追加???
#endif
	int state;
	int wait;
} GAME_IMAGE_DATA;

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_game_image(SPRITE *s)
{
	GAME_IMAGE_DATA *g=(GAME_IMAGE_DATA *)s->data;
	switch (g->state)
	{
	case 0:
		s->alpha += 3/*fps_fa ctor*/;
		if (250 < s->alpha)
		{
			s->alpha=255;
			g->wait=100;
			g->state=1;
		}
		break;
	case 1:
		g->wait -= 1/*fps_fa ctor*/;
		if (g->wait<1)
		{
			g->state=2;
		}
		break;
	case 2:
		s->alpha -= 8/*fps_fa ctor*/;
		if (s->alpha < 50)
		{
			s->alpha=0;
			s->type=SP_DELETE;
		}
		break;
	}
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/



/*---------------------------------------------------------
	add common
---------------------------------------------------------*/

static void add_common(STAGE_DATA *l, SDL_Surface *tmp)
{
	static IMAGE_RESOURCE my_resource[1] =
	{
		{
			NULL,/*dummy*/
			0,/*dummy*/
			1,
			iyx(1,1),
			PRIORITY_04_ITEM,
			0, 0, 0
		}
	};
//
	SPRITE *s;
	s					= sprite_add_res_list(
		tmp,
//		1,
//		1,
//		1,
//		PRIORITY_04_ITEM,
		SP_FLAG_NOT_CACHE/*1*/,
//		0	/*anime_speed*/
		(IMAGE_RESOURCE *)my_resource
	);
	s->flags			|= (SP_FLAG_FREE_SURFACE|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
	s->type 			= SP_ETC;

	if (0==l->user_x)	/* 省略時はセンタリング */
	{
		s->x256 		= t256(GAME_WIDTH/2)-((tmp->w)<<7)/*t256(GAME_WIDTH)*/;
	}
	else	/* 表示座標指定 */
	{
		s->x256 		= ((l->user_x)<<8);
	}
		s->y256 		= ((l->user_y)<<8);
	s->alpha			= 0;
	s->callback_mover	= move_game_image;
	GAME_IMAGE_DATA *data;
	data				= mmalloc(sizeof(GAME_IMAGE_DATA));
	s->data 			= data;
#if 1
	data->b.score		= score(200*2); 		// [***090410		???追加???
	data->b.health		= 25+(difficulty<<2);	// [***090410		???追加???
#endif
	data->wait			= 0;
	data->state 		= 0;
}
/*---------------------------------------------------------
	'T' english Text
---------------------------------------------------------*/
//l->user_string,l->user_y
//char *text,
//int ypos

/* ゲーム中イベントメッセージ(英語)表示 */
void add_enemy_eigo_string(STAGE_DATA *l)
{
char *text;
	text=l->user_string;
//
	{	SDL_Surface *tmp;
		tmp 				= font_render(text,FONT04);
		add_common(l, tmp);
	}
}

/*---------------------------------------------------------
	'P' [***090313	追加	 Picture
	ここでロードすると確実に処置落ちするよ
---------------------------------------------------------*/

/*l->user_string,l->user_x,l->user_y*/
//char *filename,
//int xpos,
//int ypos
void add_enemy_load_picture(STAGE_DATA *l)
{
	SDL_Surface *surface;
	{
		char fn[128/*64 50*/];
	//	strcpy(fn, data_dir); 	strcat(fn, "/");
		strcpy(fn, DIRECTRY_NAME_DATA "/");
		char *filename;
		filename		= l->user_string;
		strcat(fn,filename);
		surface 		= IMG_Load(fn);/*ここでロードすると確実に処置落ちするよ*/
	}
	surface 			= SDL_DisplayFormatAlpha(surface);
//
	{	SDL_Surface *tmp;
		tmp=SDL_CreateRGBSurface(
			SDL_SRCCOLORKEY|SDL_SWSURFACE,
			surface->w,
			surface->h,
			sdl_screen[SDL_00_SCREEN]->format->BitsPerPixel,
			sdl_screen[SDL_00_SCREEN]->format->Rmask,
			sdl_screen[SDL_00_SCREEN]->format->Gmask,
			sdl_screen[SDL_00_SCREEN]->format->Bmask,
			sdl_screen[SDL_00_SCREEN]->format->Amask);
		SDL_SetColorKey(tmp,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
		SDL_BlitSurface(surface,NULL,tmp,NULL);
		SDL_FreeSurface(surface);
		add_common(l, tmp);
	}
}
