
#include "bullet_object.h"
#include "scenario_script.h"

/*---------------------------------------------------------
	���o�p�e�L�X�g(�p��)
	not a real enemy - just a line of text
	-------------------------------------------------------

---------------------------------------------------------*/
#if 000
typedef struct
{
#if 1
//	ENEMY_BASE b;		// [***090410		???�ǉ�???
#endif
	int state;
	int wait;
} GAME_IMAGE_DATA;
#endif
/*---------------------------------------------------------

---------------------------------------------------------*/
#if 000
static void move_game_image(SPRITE *src)
{
	GAME_IMAGE_DATA *data = (GAME_IMAGE_DATA *)src->data;
	switch (data->state)
	{
	case 0:
		src->alpha += 0x03/*fps_fa ctor*/;
		if (0xfa < src->alpha)
		{
			src->alpha	= 0xff;
			data->wait	= 100;
			data->state = 1;
		}
		break;
	case 1:
		data->wait -= 1/*fps_fa ctor*/;
		if (data->wait<1)
		{
			data->state = 2;
		}
		break;
	case 2:
		src->alpha -= 0x08/*fps_fa ctor*/;
		if (src->alpha < 0x32)
		{
			src->alpha = 0;
			src->type = SP_DELETE;
		}
		break;
	}
}
#endif
/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/



/*---------------------------------------------------------
	add common
---------------------------------------------------------*/
	#if (000)
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
	s->flags			&= (~(SP_FLAG_COLISION_CHECK));		/* �����蔻���OFF(���G) */
	s->type 			= SP_MENU_TEXT/*SP_MUTEKI*/;	/* �d�l�ύX */

	if (0==l->user_x)	/* �ȗ����̓Z���^�����O */
	{
		s->x256 		= t256(GAME_WIDTH/2)-((tmp->w)<<7)/*t256(GAME_WIDTH)*/;
	}
	else	/* �\�����W�w�� */
	{
		s->x256 		= ((l->user_x)<<8);
	}
		s->y256 		= ((l->user_y)<<8);
	s->alpha			= 0x00;
	s->callback_mover	= move_game_image;
	GAME_IMAGE_DATA *data;
	data				= mmalloc(sizeof(GAME_IMAGE_DATA));
	s->data 			= data;
#if 1
//	/*data->b.*/s->base_score		= score(200*2); 		// [***090410		???�ǉ�???
//	/*data->b.*/s->base_health		= 25+(difficulty<<2);	// [***090410		???�ǉ�???
#endif
	data->wait			= 0;
	data->state 		= 0;
}
#endif
/*---------------------------------------------------------
	'T' kanji Text string.
---------------------------------------------------------*/
//l->user_string,l->user_y
//char *text,
//int ypos

/* �Q�[�����C�x���g���b�Z�[�W�\�� */
void add_enemy_kanji_string(STAGE_DATA *l)
{
	char *text;
	text=l->user_string;
//
	#if (000)
	{	SDL_Surface *tmp;
		tmp 				= font_render(text,FONT04);
		add_common(l, tmp);
	}
	#endif
	msg_time = (60*5);
	print_kanji(/*SDL_Surface *drawmap,*/ /*SDL_Rect *rect*/0, text, /*int color_type*/7, /*int wait*/0);
}

/*---------------------------------------------------------
	'P' [***090313	�ǉ�	 Picture
	�����Ń��[�h����Ɗm���ɏ������������
---------------------------------------------------------*/
	#if (000)
	/* �Q�[���������������Ȃ��悤�ɗ\�߉摜�L���b�V���ɋl�߂�����Ȃ̂ŁAP�R�}���h���Q�[���������������Ȃ��悤�ɍ��͓̂���B
	(�e�ʊJ�n�O�ɂ��̃X�e�[�W�Ŏg��P�R�}���h�摜���p�X�^�b�N�ɋl�߂�Ώo���Ȃ������Ȃ�) */

/*l->user_string,l->user_x,l->user_y*/
//char *filename,
//int xpos,
//int ypos
void add_enemy_load_picture(STAGE_DATA *l)
{
	#if (000)
	SDL_Surface *surface;
	{
		char fn[128/*64 50*/];
	//	strcpy(fn, data_dir);	strcat(fn, "/");
		strcpy(fn, DIRECTRY_NAME_DATA "/");
		char *filename;
		filename		= l->user_string;
		strcat(fn,filename);
		surface 		= IMG_Load(fn);/*�����Ń��[�h����Ɗm���ɏ������������*/
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
	#endif
}
	#endif
