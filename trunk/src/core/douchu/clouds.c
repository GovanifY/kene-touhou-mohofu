
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

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
#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif
//typedef struct
//{
#define CLOUDS_DATA_speed_base256	user_data04
#define CLOUDS_DATA_speed_rand256	user_data05
//} CLOUDS_DATA;

/*---------------------------------------------------------

---------------------------------------------------------*/

static void clouds_mover(SPRITE *src)
{
	src->y256 += src->CLOUDS_DATA_speed_base256;/*fps_factor*/
	src->y256 += src->CLOUDS_DATA_speed_rand256;/*fps_factor*/
	if (src->y256 > t256(GAME_HEIGHT))
	{
		#if 0/*SDL(左隅座標)*/
		src->y256			-= (t256(GAME_HEIGHT)+((src->w128+src->w128)));
		src->CLOUDS_DATA_speed_rand256 = (ra_nd()&((128)-1));
		src->x256			= (ra_nd()&((256*256)-1))+(ra_nd()&((128*256)-1))-((src->w128+src->w128));	/*(ra_nd()%GAME_WIDTH)*/
		#else/*Gu(中心座標)*/
		src->y256			-= (t256(GAME_HEIGHT));
		src->CLOUDS_DATA_speed_rand256 = (ra_nd()&((128)-1));
		src->x256			= (ra_nd()&((256*256)-1))+(ra_nd()&((128*256)-1));							/*(ra_nd()%GAME_WIDTH)*/
		#endif
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
global /*static*/ void clouds_init(/*int lev*/)
{
//	int i;
//	char filename[32/*20*/];
//		char wol_type[MAX_CLOUDS]= { 3,3,3,3, 3,2,2,1 };
//	for (i=0;i<MAX_CLOUDS;i++)
//		strcpy(filename,render_filename(/*TYPE_02_WOLKE0Z_Z,*/ /*lev,*/ wol_type[i]));/*ここでロードすると確実に処理落ちするよ*/
}


/*---------------------------------------------------------
	ここでロードすると確実に処理落ちするよ
---------------------------------------------------------*/
//	char *filename,
//	int total_frames,
//	int x_divide_frames,
//	int y_divide_frames,
//	u8 priority,
//	int use_alpha,
//	int anime_speed

//	char *filename, 		/*filename*/(l->user_string),
//	int total_frames,		1/*(l->user_x)*/,
//	int x_divide_frames,	1,
//	int y_divide_frames,	1,
//	u8 priority,		PR IORITY_00_BG/*P R_BACK0*/,
//	int use_alpha,			0,
//	int anime_speed 		3

static char clouds_filename_work[32];

extern void load_bg2_chache(char *filename, int use_alpha);
/*l->user_string,l->user_x,l->user_y*/
// /*static*/extern SPRITE *sprite_add_internal_res(IM AGE_RESOURCE *image_resource_ptr);
global void add_clouds(STAGE_DATA *l)
{
	if ((MAX_CLOUDS-1) < used_clouds)
	{
		return;
	}
//
	// error check
	#if 000
	load_bg2_chache(/*filename*/(l->user_string), 0);
	#endif
//
	strcpy(clouds_filename_work, /*filename*/(l->user_string));
	{
		SPRITE *h;
		h						= sprite_add_gu_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			w3[used_clouds] 	= h;
			used_clouds++;
		//
			h->m_Hit256R		= (1/*test*/);
			h->x256 			=  (ra_nd()&((256*256)-1))+(ra_nd()&((128*256)-1)); 	/*(ra_nd()%GAME_WIDTH)*/ /*((w3[i]->w)<<8)*/
			h->y256 			= -64-(ra_nd()&((256*256)-1));							/*(ra_nd()%GAME_HEIGHT)*/
			h->flags			&= (~(SP_FLAG_COLISION_CHECK));/*あたり判定なし*/
//			h->flags			|= (SP_FLAG_VISIBLE);
			h->type 			= SP_MUTEKI;
			h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x3f);	/*bg_alpha*/	//	h->alpha	= /*0*/0xff/*bg_alpha*/;
			h->callback_mover	= clouds_mover;
			h->CLOUDS_DATA_speed_base256			= (l->user_y);	/*t256(0.5)-*/	/*1.0*/
		//	h->CLOUDS_DATA_speed_base256			= t256(1.0);/*2.0*/
		//	h->CLOUDS_DATA_speed_base256			= t256(1.5);/*3.0*/
			h->CLOUDS_DATA_speed_rand256			= (ra_nd()&((128)-1));
		}
	}
}
#if 000
	static const IM AGE_RESOURCE my_resource[1] =
	{
		{
			&clouds_filename_work[0],
			0,
			1/*(l->user_x)*/,
			iyx(1,1),
			PR IORITY_01_SHOT/*PR IORITY_00_BG*/ /*P R_BACK0*/,
			0, 0, 0
		}
	};
#endif

/*---------------------------------------------------------
	ステージ開始時、ゲームオーバー時に開放
---------------------------------------------------------*/

global void clouds_destroy(void)
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

global void clouds_system_init(void)
{
	used_clouds = 0;
}
