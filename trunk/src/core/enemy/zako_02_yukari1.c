
#include "bullet_object.h"

/*---------------------------------------------------------
		"紫編隊1",		"MING",
	-------------------------------------------------------
	10機ばらばらに、落ちてくるザコ敵
	自機狙い赤単弾を連射してくる
	-------------------------------------------------------
	道中に必要か悩んだが、元と似た仕様の編隊飛行敵は必要ない。
	単純に落ちてくる敵に変更する。
	-------------------------------------------------------
	10機三角錐状に続がり、編隊飛行してくるザコ敵
	ming
	rwingx_curverに似てる
---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
//	int angle512;
	int time_out;
	int enemy_rank;
} YUKARI1_DATA;
static int destoroy;

#define NUM_OF_ENEMIES (10)

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_yukari1(SPRITE *src)
{
	int put_item;	put_item = 99;
//	case SP_ZAKO_02_YUKARI1:
	if (rand_percent(30))
	{
		if (rand_percent(50))	{	put_item = (SP_ITEM_00_P001&0xff);}
		else					{	put_item = (SP_ITEM_06_TENSU&0xff);}
	}
//
	if (99 != put_item)
	{
		item_create(src, (put_item|SP_ITEM_00_P001), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
	}
//
	destoroy++;
	if ( (NUM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
	{
		destoroy = 0;
		item_create(/*zzz*/ src, SP_ITEM_00_P001, 1, (ITEM_MOVE_FLAG_06_RAND_XY) );
	}
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_yukari1(SPRITE *src)
{
	YUKARI1_DATA *data = (YUKARI1_DATA *)src->data;
	data->time_out--;
	if (0==((data->time_out)&0x3f))
	{
		if (data->enemy_rank>0)
		{	bullet_create_aka_maru_jikinerai(src, t256(data->enemy_rank+2) );}
	}
//
	src->x256 += (src->vx256)/**fps_fa ctor*/;
	src->y256 += (src->vy256)/**fps_fa ctor*/;
	if ((src->y256 > t256(GAME_HEIGHT)))
	{
		src->type = SP_DELETE;	/* おしまい */
	}
//
	src->m_angleCCW512+=5;/*グラ回転*/
	mask512(src->m_angleCCW512);
//	src->anim_frame=(deg_512_to_360(src->m_angleCCW512+deg_360_to_512(270))/10)%36;
//	src->anim_frame = ((((src->m_angleCCW512/*+deg_360_to_512(270)*/)&(512-1))*(36/2))>>8);
//	src->anim_frame = ((((src->m_angleCCW512/*+deg_360_to_512(270)*/)&(512-1))*(32/2))>>8);
//	src->anim_frame = ((((src->m_angleCCW512/*+deg_360_to_512(270)*/)&(512-1)))>>4);
	src->yx_anim_frame = ( ((src->m_angleCCW512>>3)&(0x30)) | ((src->m_angleCCW512>>4)&(0x07)) );

/* "yukari8x4.png"
src->m_angleCCW512		 a bcde ----
src->yx_anim_frame	  yyyy xxxx
src->yx_anim_frame	  --ab -cde
*/
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_yukari1(STAGE_DATA *l)/*int lv*/
{
	int lv;	lv	= l->user_y;
//
	destoroy = 0;
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s						= sprite_add_res(BASE_YUKARI32_PNG);	//s->anim_speed=0;/*36"mi ng.png"*/
		s->type 				= SP_ZAKO/*_02_YUKARI1*/;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_yukari1;
		s->callback_loser		= lose_yukari1;
		s->callback_hit_enemy	= callback_hit_zako;
		s->anim_frame			= 0;
#if 1
/* CCWの場合 */
		s->m_angleCCW512		= deg_360_to_512CCW(0);
#endif
		/*data->base.*/s->base_score		= score(25*2);
		/*data->base.*/s->base_health		= 1+(difficulty<<2);
//
		{
			/* 初期位置 */
		u32 rrrr;
			rrrr = (ra_nd());
			s->x256 = ((rrrr)&0xffff) + t256((GAME_WIDTH-256)/2);
			s->y256 = -t256(30);
		//
			s->vx256 = ((rrrr)&0x01ff);/*右方向*/
			/* 画面右側の場合、左方向へ進む */
			if ( t256((GAME_WIDTH)/2) < s->x256)
			{
				s->vx256 = (-(s->vx256));/*左方向*/
			}
			s->vy256 = ((rrrr>>16)&0x01ff) + t256(1.00);/*下方向*/
		}
//
		YUKARI1_DATA *data;
		data					= mmalloc(sizeof(YUKARI1_DATA));
		s->data 				= data;
		data->time_out			= 0x3ff;
		data->enemy_rank		= lv;
	}
}
#undef NUM_OF_ENEMIES
