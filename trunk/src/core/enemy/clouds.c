
#include "game_main.h"

/*---------------------------------------------------------
	演出用浮遊物
	-------------------------------------------------------

---------------------------------------------------------*/

/* clouds */
#define MAX_CLOUDS (8)
static SPRITE *w3[MAX_CLOUDS];
static int used_clouds;

/*---------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
	int speed_base256;
	int speed_rand256;
} CLOUDS_DATA;

/*---------------------------------------------------------

---------------------------------------------------------*/

static void clouds_mover(SPRITE *src)
{
	CLOUDS_DATA *d = (CLOUDS_DATA *)src->data;
	src->y256 += d->speed_base256/**fps_fa ctor*/;
	src->y256 += d->speed_rand256/**fps_fa ctor*/;
	if (src->y256 > t256(GAME_HEIGHT))
	{
		src->y256			-= (t256(GAME_HEIGHT)+((src->w128+src->w128)));
		d->speed_rand256	= (ra_nd()&((128)-1));
		src->x256			= (ra_nd()&((256*256)-1))+(ra_nd()&((128*256)-1))/*(ra_nd()%GAME_WIDTH)*/-((src->w128+src->w128));
		//c->alpha			= bg_alpha;
//		src->alpha			= 200;/*遅すぎる([60-50fps -> [40-30]fps)*/
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

//	int lev = player_now_stage;
//	sp rintf(filename,"wolke03_%d.png",lev);
//	sp rintf(filename,"wolke02_%d.png",lev);
//	sp rintf(filename,"wolke01_%d.png",lev);
//	strcpy(filename,render_filename(/*TYPE_02_WOLKE0Z_Z,*/ /*lev,*/ 2));
//	strcpy(filename,render_filename(/*TYPE_02_WOLKE0Z_Z,*/ /*lev,*/ 1));
/*static*/ void clouds_init(/*int lev*/)
{
//	int i;
//	char filename[32/*20*/];
//		char wol_type[MAX_CLOUDS]= { 3,3,3,3, 3,2,2,1 };
//	for (i=0;i<MAX_CLOUDS;i++)
//		strcpy(filename,render_filename(/*TYPE_02_WOLKE0Z_Z,*/ /*lev,*/ wol_type[i]));/*ここでロードすると確実に処置落ちするよ*/
}


/*---------------------------------------------------------
	ここでロードすると確実に処置落ちするよ
---------------------------------------------------------*/
//	char *filename,
//	int total_frames,
//	int x_divide_frames,
//	int y_divide_frames,
//	Uint8 priority,
//	int use_alpha,
//	int anime_speed

//	char *filename,			/*filename*/(l->user_string),
//	int total_frames,		1/*(l->user_x)*/,
//	int x_divide_frames,	1,
//	int y_divide_frames,	1,
//	Uint8 priority,			PRIORITY_00_BG/*P R_BACK0*/,
//	int use_alpha,			0,
//	int anime_speed			3

static char clouds_filename_work[32];

extern void load_bg2_chache(char *filename, int use_alpha);
/*l->user_string,l->user_x,l->user_y*/
/*static*/extern SPRITE *sprite_add_internal_res(IMAGE_RESOURCE *image_resource_ptr);
void add_clouds(STAGE_DATA *l)
{
	if ((MAX_CLOUDS-1) < used_clouds)
	{
		return;
	}
//
	// error check
	load_bg2_chache(/*filename*/(l->user_string), 0);
//
	static IMAGE_RESOURCE my_resource[1] =
	{
		{
			&clouds_filename_work[0],
			0,
			1/*(l->user_x)*/,
			iyx(1,1),
			PRIORITY_00_BG/*P R_BACK0*/,
			0, 0, 0
		}
	};
	strcpy(clouds_filename_work, /*filename*/(l->user_string));
	{
		w3[used_clouds] 				= sprite_add_internal_res((IMAGE_RESOURCE *)my_resource);
	//	w3[used_clouds] 				= spr ite_add_file(filename,1,PRIORITY_01_SHOT/*P R_BACK1*/);
	//	w3[used_clouds] 				= spr ite_add_file(filename,1,PRIORITY_01_SHOT/*P R_BACK2*/);
		w3[used_clouds]->x256			=  (ra_nd()&((256*256)-1))+(ra_nd()&((128*256)-1))/*(ra_nd()%GAME_WIDTH)*/ /*((w3[i]->w)<<8)*/;
		w3[used_clouds]->y256			= -64-(ra_nd()&((256*256)-1))/*(ra_nd()%GAME_HEIGHT)*/;
		w3[used_clouds]->flags			&= (~(SP_FLAG_COLISION_CHECK));/*あたり判定なし*/
		w3[used_clouds]->flags			|= (SP_FLAG_VISIBLE);
		w3[used_clouds]->type			= SP_MUTEKI;
		w3[used_clouds]->alpha			= /*0*/255/*bg_alpha*/;
	//	w3[used_clouds]->anim_frame 	= (l->user_x);	/*...*/
		w3[used_clouds]->callback_mover = clouds_mover;
		CLOUDS_DATA *d;
		d								= mmalloc(sizeof(CLOUDS_DATA));
		w3[used_clouds]->data			= d;
		d->speed_base256				= /*t256(0.5)-*/ (l->user_y);/*1*/
	//	d->speed_base256				= t256(1);/*2*/
	//	d->speed_base256				= t256(1.5);/*3*/
		d->speed_rand256				= (ra_nd()&((128)-1));
	}
	used_clouds++;
}

/*---------------------------------------------------------
	ステージ開始時、ゲームオーバー時に開放
---------------------------------------------------------*/

void clouds_destroy(void)
{
	//static void clouds_remove(void)
	{
		int i;
		i = used_clouds;
		while ( 0 < i )
		{
			i--;
			w3[i]->type = SP_DELETE;
		}
		used_clouds = 0;
	}
}

/*---------------------------------------------------------
	psp起動時に一度だけ初期化する
---------------------------------------------------------*/

void clouds_system_init(void)
{
	used_clouds = 0;
}
