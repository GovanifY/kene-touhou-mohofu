#include "bg.h"

extern double fps_factor;
extern SDL_Surface *screen;
extern SPRITE *player;

/*static*/ double bg_alpha;

int is_bg_add;	//[***090201		�ǉ�:�ǉ��t���O
int is_bg_end;	//[***090201		�ǉ�:�O�w�i�I���t���O
int is_bg_fin;	//[***090202		�ǉ�:�w�i�񃋁[�v�t���O
int n_bg;		//[***090209		�ǉ�:�ǉ��w�i�ԍ�

/* clouds */
static SPRITE *w1[5];
static SPRITE *w2[10];
static SPRITE *w3[20];

/* tile */
static SDL_Surface *btile1=NULL;
static SDL_Surface *btile2=NULL;		//[***090201		�ǉ�
static double btile_y;

static int akt_bgtype;

enum
{
	TYPE_00_BACKZ_A_Z=0,
	TYPE_01_BACKZ,
	TYPE_02_WOLKE0Z_Z,
};


/* ���x�̗v��ꏊ�ł� sp rintf() �͎g��Ȃ������ǂ� */ 		/* �����񏈗�(sp rintf)�͔��ɒx�� */
static char *render_filename(int type, int lev, int n_bg)
{
	char work_filename[32/*20*/];
	char *filename;
	filename=work_filename;
	switch (type)
	{
	case TYPE_00_BACKZ_A_Z:
	//	sp rintf(filename,"back%d_a-%d.jpg", lev, n_bg);
		strcpy(filename,"backZ_a-Z.jpg");
		filename[4]= ('0'+lev);
		filename[8]= ('0'+n_bg);
		break;
	case TYPE_01_BACKZ:
	//	sp rintf(filename,"back%d.jpg",lev);
		strcpy(filename,"backZ.jpg");
		filename[4]= ('0'+lev);
		break;
	case TYPE_02_WOLKE0Z_Z:
	//	sp rintf(filename,"wolke03_%d.png", lev, n_bg);/*�t��*/
		strcpy(filename,"wolke0Z_Z.png");
		filename[6]= ('0'+n_bg);
		filename[8]= ('0'+lev);
		break;
	}
	return filename;
}

void tile_add(/*int lev*/)		//[***090201		�ǉ�
{
	int lev = player_now_stage;
	btile2=loadbmp(/*filename*/render_filename(TYPE_00_BACKZ_A_Z, lev, n_bg));
	is_bg_add=1;		//�񖇖ڂ̔w�i�ǉ��t���O
}


static void tile_init(/*int lev*/)
{
	int lev = player_now_stage;
	//if (NULL==btile1)
	//{
		btile1=loadbmp(/*filename*/render_filename(TYPE_01_BACKZ, lev, 0));
	//}
	btile_y=GAME_WIDTH;
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
	if (c->y>GAME_HEIGHT)
	{
		c->y-=GAME_HEIGHT+c->w;
		d->speed_rand=(double)(rand()&(256-1))/256;
		c->x=rand()%GAME_WIDTH-c->w;
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
//	sp rintf(filename,"wolke03_%d.png",lev);
	strcpy(filename,render_filename(TYPE_02_WOLKE0Z_Z, lev, 3));
	for (i=0;i<5;i++)
	{
		w3[i]=sprite_add_file(filename,1,PR_BACK0);
		w3[i]->x=rand()%GAME_WIDTH-w3[i]->w;
		w3[i]->y=rand()%GAME_HEIGHT;
		w3[i]->flags&=~SP_FLAG_COLCHECK;
		w3[i]->flags|=SP_FLAG_VISIBLE;
		w3[i]->type=SP_ETC;
		w3[i]->mover=clouds_mover;
		d=mmalloc(sizeof(CLOUDS_DATA));
		w3[i]->data=d;
		w3[i]->alpha=bg_alpha;
		d->speed_base=1;
		d->speed_rand=(double)(rand()&(256-1))/256;
	}
//	sp rintf(filename,"wolke02_%d.png",lev);
	strcpy(filename,render_filename(TYPE_02_WOLKE0Z_Z, lev, 2));
	for (i=0;i<2;i++)
	{
		w2[i]=sprite_add_file(filename,1,PR_BACK1);
		w2[i]->x=rand()%GAME_WIDTH-w2[i]->w;
		w2[i]->y=rand()%GAME_HEIGHT;
		w2[i]->flags&=~SP_FLAG_COLCHECK;
		w2[i]->flags|=SP_FLAG_VISIBLE;
		w2[i]->type=SP_ETC;
		w2[i]->mover=clouds_mover;
		d=mmalloc(sizeof(CLOUDS_DATA));
		w2[i]->data=d;
		w2[i]->alpha=bg_alpha;
		d->speed_base=2;
		d->speed_rand=(double)(rand()&(256-1))/256;
	}
//	sp rintf(filename,"wolke01_%d.png",lev);
	strcpy(filename,render_filename(TYPE_02_WOLKE0Z_Z, lev, 1));
	for (i=0;i<1;i++)
	{
		w1[i]=sprite_add_file(filename,1,PR_BACK2);
		w1[i]->x=rand()%GAME_WIDTH-w1[i]->w;
		w1[i]->y=rand()%GAME_HEIGHT;
		w1[i]->flags&=~SP_FLAG_COLCHECK;
		w1[i]->flags|=SP_FLAG_VISIBLE;
		w1[i]->type=SP_ETC;
		w1[i]->mover=clouds_mover;
		d=mmalloc(sizeof(CLOUDS_DATA));
		w1[i]->data=d;
		w1[i]->alpha=bg_alpha;
		d->speed_base=3;
		d->speed_rand=(double)(rand()&(256-1))/256;
	}
}



void bg_init(int bg_type/*,int lev*/)
{
	if ((bg_type>=BG_LAST)||(bg_type<BG_BLACK))
	{	error(ERR_FATAL,"unknown BG_TYPE: %d",bg_type);}

	akt_bgtype = bg_type;
	bg_alpha=0;
	switch (bg_type)
	{
	case BG_BLACK:														break;
	case BG_CLOUDS: 	tile_init(/*lev*/); 	clouds_init(/*lev*/);	break;
	case BG_STARS:		/* stars_init(); */ 							break;
	case BG_TILE:		tile_init(/*lev*/); 							break;
	}
}

static void tile_remove(void)
{
	if (btile1!=NULL)
	{	unloadbmp_by_surface(btile1);}
	btile1=NULL;

	if (btile2!=NULL)
	{	unloadbmp_by_surface(btile2);}
	btile2=NULL;
}

static void tile_work(void)
{
	if (!is_bg_fin)	//�񃋁[�v�t���O�������ĂȂ��Ƃ�(�ʏ탋�[�g)
	{
		btile_y+=fps_factor;
		if (is_bg_end)		//�w�i�ǉ����߂��o�Ă���O�w�i���I�[�܂ŗ����Ƃ�
		{
			if (btile_y>btile2->h-1)
			{
				btile_y-=btile2->h;
			}
			else if (btile_y>GAME_HEIGHT && btile1!=NULL) //������
			{
				#if 0
				unloadbmp_by_surface(btile1);
				btile1=NULL;
				unloadbmp_by_surface(btile2);
				btile2=NULL;
				#else
				tile_remove();
				#endif
				btile1=loadbmp(/*filename*/render_filename(TYPE_00_BACKZ_A_Z, player_now_stage/*level*/, n_bg));
				is_bg_add=0;
				is_bg_end=0;
				n_bg++;
			}
		}
		else	//����Ȗ��߂��o�ĂȂ��Ƃ�or�w�i�ǉ����߂��o�Ă���O�w�i���I�[�܂ŗ��ĂȂ��Ƃ�
		{
			if (btile_y>btile1->h-1)	//btile_y���O�w�i�Ɠ��������ɂȂ����Ƃ�
			{
				if (is_bg_add)			//�w�i�ǉ����߂��o���Ƃ�
				{
					btile_y=0;
					is_bg_end=1;		//�O�w�i�I���t���O
				}
				else		//�ʏ폈��
				{
					btile_y-=btile1->h;
				}
			}
		}
	}
	else		//�񃋁[�v�̏ꍇ �����̏��@�\���ĂȂ���������Ȃ�
	{
		if (is_bg_end)
		{
			if (btile_y+fps_factor <= btile2->h-1)
			{
				btile_y+=fps_factor;
			}
		}
		else
		{
			if (btile_y+fps_factor <= btile1->h-1)
			{
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
	if (bg_alpha<255)
	{
		psp_clear_screen();
		SDL_SetAlpha(btile1,SDL_SRCALPHA,bg_alpha);
	}
	else
	{
		SDL_SetAlpha(btile1,SDL_SRCALPHA,255);
	}
	if (is_bg_add || is_bg_end)
	{
		SDL_SetAlpha(btile2,SDL_SRCALPHA,bg_alpha);
		r2.w=btile2->w;
		r2.h=btile2->h;
	}

	r.w=btile1->w;
	r.h=btile1->h;

	if (is_bg_end)
	{
		for (i=0;i<GAME_WIDTH;i+=btile2->w)
		{
			r2.x=i;
			r2.y=-btile2->h+btile_y;
			SDL_BlitSurface(btile2,NULL,screen,&r2);
		}
		for (i=0;i<GAME_WIDTH;i+=btile1->w)
		{
			for (j=btile_y;j<GAME_HEIGHT;j+=btile1->h)
			{
				r.x=i;
				r.y=j;
				SDL_BlitSurface(btile1,NULL,screen,&r);
			}
		}
	}
	else
	{
		for (i=0;i<GAME_WIDTH;i+=btile1->w)
		{
			for (j=-btile1->h;j<GAME_HEIGHT;j+=btile1->h)
			{
				r.x=i;
				r.y=j+btile_y;
				SDL_BlitSurface(btile1,NULL,screen,&r);
			}
		}
	}
}

void bg_work(void)
{
	if (akt_bgtype<0)
	{
		error(ERR_WARN,"bg_work with no bg_type\n[%d]==akt_bgtype",akt_bgtype );
		return;
	}

	if (bg_alpha<255)
	{	bg_alpha+=6*fps_factor;}
	else
	{	bg_alpha=255;}

	switch (akt_bgtype)
	{
	case BG_BLACK:
		psp_clear_screen();
		break;

	case BG_CLOUDS:
		// SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0xff));
		//psp_clear_screen();
		//SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,bg_alpha));
		tile_work();
		tile_display();
		break;

	case BG_STARS:
		psp_clear_screen();
		break;

	case BG_TILE:
		tile_work();
		tile_display();
		break;
	}
}

static void clouds_remove(void)
{
	int i;
	for (i=0;i<5;i++)
	{
		if (i<1) w1[i]->type=SP_DELETE;
		if (i<2) w2[i]->type=SP_DELETE;
		w3[i]->type=SP_DELETE;
	}
}


void bg_destroy(void)
{
	if (akt_bgtype<0)
	{
		error(ERR_WARN,"bg_destroy with no bg_type\n[%d]==akt_bgtype",akt_bgtype );
		return;
	}
	switch (akt_bgtype)
	{
	case BG_BLACK:								break;
	case BG_CLOUDS: 	tile_remove();	clouds_remove();		break;
	case BG_STARS:		/* stars_remove();*/	break;
	case BG_TILE:		tile_remove();			break;
	}
	akt_bgtype = BG_THROW/*-1*/;
}
