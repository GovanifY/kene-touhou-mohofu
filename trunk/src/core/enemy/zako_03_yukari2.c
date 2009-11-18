
#include "bullet_object.h"

/*---------------------------------------------------------
		"紫編隊2",		"GREETER",
	-------------------------------------------------------
	バグあり？？？
	-------------------------------------------------------
	５機一列に続がり、編隊飛行してくるザコ敵
	-------------------------------------------------------
	ming_greeter
	rwingx_curver(ming)に似てる
---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
//	int angle512;
	int speed256;
	int state;
	int enemy_rank;
//
	int max_y256;
} YUKARI2_DATA;
static int destoroy;

#define NUM_OF_ENEMIES (5)

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_yukari2(SPRITE *src)
{
	item_create(src, enemy_get_random_item(), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
//
	destoroy++;
	if ( (NUM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
	{
		destoroy = 0;
		if (rand_percent(30))
		{
			item_create(/*zzz*/ src, SP_ITEM_00_P001, 1, ITEM_MOVE_FLAG_06_RAND_XY);
		}
	}
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_yukari2(SPRITE *src)
{
	YUKARI2_DATA *data = (YUKARI2_DATA *)src->data;
	switch (data->state)
	{
	case 0: /* nach unten */
		if (src->y256 >= data->max_y256)
		{
			data->state=1;
			src->tmp_angleCCW512 = atan_512(t256(0)-src->y256,t256(GAME_WIDTH/2)-src->x256);
			if (t256(1.5/*2.0*/) < data->speed256)
			{	/*減速*/
				data->speed256 -= t256(0.2);
			//	data->speed256 = t256(2)/*3+difficulty*/;								// [***090201		変更
			}
			/* CCWの場合 */
			src->vx256=((sin512((src->tmp_angleCCW512))*data->speed256)>>8);
			src->vy256=((cos512((src->tmp_angleCCW512))*data->speed256)>>8);
		}
		if (ENEMY_LAST_SHOT_LINE256 > src->y256)	/* このラインより下からは敵が撃たない */
		{
			if (data->enemy_rank)
			{
			//	if (0==(ra_nd()%(105-(data->enemy_rank-2+difficulty)*10)))	// [***090126		若干変更
			//	if (0==(ra_nd()%(11-(data->enemy_rank-2+difficulty) ))) 	// [***090126		若干変更
			//	if (0==(ra_nd()%(16-(data->enemy_rank-2+difficulty) ))) 	// [***090126		若干変更
			//	if ((data->enemy_rank-2+difficulty) >= (ra_nd()&(16-1)))	// [***090126		若干変更
				if ((data->enemy_rank-2+difficulty) >= (ra_nd()&(64-1)))	// [***090126		若干変更
				{
					bullet_create_aka_maru_jikinerai(src, t256(1)+t256(difficulty)+(data->enemy_rank<<6)/*t256(data->enemy_rank/3)*/ );
				}
			}
		}
		break;
	case 1:
	//	if (src->y256 < -((src->h128+src->h128)) )
		if (src->y256 < -(t256(16)) )
		{
		//	src->flags	&= (~(SP_FLAG_VISIBLE));
			src->type = SP_DELETE;	/* おしまい */
		}
		break;
	}
	/*似てるがちょっと違う--以下rwingx.cと同じ*/
	src->x256+=(src->vx256)/**fps_fa ctor*/;
	src->y256+=(src->vy256)/**fps_fa ctor*/;
//
	src->m_angleCCW512 += 5;/*グラ回転*/
	mask512(src->m_angleCCW512);
//	src->an im_frame=(deg_512_to_360(src->tmp_angleCCW512+deg_360_to_512(270))/10)%36;
//	src->an im_frame = ((((src->tmp_angleCCW512/*+deg_360_to_512(270)*/)&(512-1))*(36/2))>>8);
//	src->an im_frame = ((((src->tmp_angleCCW512/*+deg_360_to_512(270)*/)&(512-1))*(32/2))>>8);
//	src->an im_frame = ((((src->tmp_angleCCW512/*+deg_360_to_512(270)*/)&(512-1)))>>4);
//	src->yx_an im_frame = ( ((src->tmp_angleCCW512>>3)&(0x30)) | ((src->tmp_angleCCW512>>4)&(0x07)) );
/* "yukari8x4.png"
src->tmp_angleCCW512	 a bcde ----
src->yx_an im_frame		  yyyy xxxx
src->yx_an im_frame		  --ab -cde
*/
}


#if 0
	case 0: 	/* 右へ移動中 */
		if (src->x >= data->clip_right)
		{	data->state=1;}
		break;
	case 1: 	/* 右周りで回転中 */
		src->tmp_angleCCW512 -= data->turnspeed512/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
		if (src->tmp_angleCCW512 <= deg_360_to_512(180))
		{
			src->tmp_angleCCW512 = deg_360_to_512(180);
			data->state=2;
			if (0 < data->enemy_rank)
			{	bullet_create_aka_maru_jikinerai(s, t256(3)+(data->enemy_rank<<7) );}
		}
		break;
	case 2: 	/* 左へ移動中 */
		if (src->x <= data->clip_left)
		{	data->state=3;}
		break;
	case 3: 	/* 左周りで回転中 */
		src->tmp_angleCCW512 += data->turnspeed512/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
		if (src->tmp_angleCCW512 >= deg_360_to_512(360) )
		{
			src->tmp_angleCCW512 = deg_360_to_512(0);
			data->state=0/*4*/;
			if (0 < data->enemy_rank)
			{	bullet_create_aka_maru_jikinerai(s, t256(3)+(data->enemy_rank<<7) );}
		}
		break;

	case 8: 	/* 右へ移動中 */
		if (src->x > GAME_WIDTH)
		{	src->flags &= (~(SP_FLAG_VISIBLE));}
		break;
#endif

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_yukari2(STAGE_DATA *l)/*int lv*/
{
	int enemy_rank; 	enemy_rank	= l->user_y;
//
	destoroy = 0;
	static int static_last=0;/* 登場位置の切り替え */
	static_last++;
	static_last &= 1;
//
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
//		s						= sp rite_add_res(BASE_YUKARI32_PNG);	//s->anim_speed=0;/*36"mi ng.png"*/
		s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);	//s->anim_speed=0;/*36"mi ng.png"*/
		s->type 				= /*SP_ZAKO*/TEKI_61_NIJI_HOSI/*_03_YUKARI2*/;
//		s->type 				= SP_ZAKO/*_03_YUKARI2*/;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_yukari2;
		s->callback_loser		= lose_yukari2;
		s->callback_hit_enemy	= callback_hit_zako;
		if (0==static_last) {	s->x256 = t256(0);								}	//右上から登場
		else				{	s->x256 = t256(GAME_WIDTH)-(t256(24)/*s->w128+s->w128*/); 	}	//左上から登場
		s->y256 				= -i*(t256(24)/*s->h128+s->h128*/);
		YUKARI2_DATA *data;
		data					= mmalloc(sizeof(YUKARI2_DATA));
		s->data 				= data;
//
		data->max_y256			= (t256(GAME_HEIGHT)-((t256(24)/*s->h128+s->h128*/))-t256(60));
		s->tmp_angleCCW512		= atan_512((t256(GAME_HEIGHT)-((t256(24)/*s->h128+s->h128*/))-t256(60))-s->y256,t256(GAME_WIDTH/2)-s->x256);
		data->speed256			= (t256(2.5/*3.0*/)+((difficulty)<<4) ) /*4*/;/*始めだけは速い*/
		/* CCWの場合 */
		s->vx256=((sin512((s->tmp_angleCCW512))*data->speed256)>>8);
		s->vy256=((cos512((s->tmp_angleCCW512))*data->speed256)>>8);
//
		data->state 			= 0;
		/*data->base.*/s->base_score		= score(5*2);
		/*data->base.*/s->base_health		= 1+(difficulty<<2);
		data->enemy_rank		= enemy_rank;
	}
}
#undef NUM_OF_ENEMIES
