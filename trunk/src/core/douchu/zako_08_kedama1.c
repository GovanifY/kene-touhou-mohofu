
#include "douchu.h"

/*---------------------------------------------------------
		"毛玉1",	"BADGUY",
	-------------------------------------------------------
	本家毛玉
---------------------------------------------------------*/

#define tx256		user_data00
#define ty256		user_data01
#define time_out	user_data02
#define speed256	user_data03
#define state		user_data04

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
enum
{
	ST00 = 0,
//	ST01,
	ST02,
	ST03,
}
;
static void move_kedama1(SPRITE *src)
{
	src->time_out--;/*fps_factor*/
	switch (src->state)
	{
	case ST00:	/* 登場する */
		{	int angle512;
			angle512 = atan_512(src->ty256-src->y256-t256(64)/*+(20)*/,src->tx256-src->x256);
			src->vx256 = ((sin512(angle512)));
			src->vy256 = ((cos512(angle512)));
		}
		if ((src->x256 >= player->x256) ||
			(src->y256 > t256(GAME_HEIGHT/2) ) || /*(480-80)*/ /*(272-32)*/
			(0 > src->time_out)
		 )
		{
			src->vy256 = (-src->vy256);
			src->state++;/* = ST02*/
			src->time_out	= 64;
			/* 第二目標は、 */
			src->tx256		= t256(-100);
			src->ty256		= t256(0);/*((player->y256)&(128-1))*/
			src->speed256	= /*6*/t256(2.5/*5.0*/);/*fps_factor*/
		}
		break;/*??? [***090215 追加 */
	case ST02:
		if (0==(src->time_out & 0x0f))
		{
			if (ENEMY_LAST_SHOT_LINE256 > src->y256)	/* このラインより下からは敵が撃たない */
			{
				send1_obj->x256 = src->x256;
				send1_obj->y256 = src->y256;
				send1_obj->BULLET_REGIST_speed256			=		t256(1.0);	//t256(1.0)+t256((src->enemy_rank)>>1),
				send1_obj->BULLET_REGIST_angle512			=		ANGLE_JIKI_NERAI_DAN;
				send1_obj->BULLET_REGIST_div_angle512		=		(int)(512/24);
				send1_obj->BULLET_REGIST_bullet_obj_type	=		BULLET_KOME_01_AOI+(src->x256&0x07);
				send1_obj->BULLET_REGIST_n_way				=		(3+difficulty+difficulty) ;
				bullet_regist_basic();
			}
		}
		if (0 > src->time_out)
		{
			src->state++;
		}
		break;
	case ST03:/* 退場 */
		{	int angle512;
			angle512=atan_512(src->ty256-src->y256-t256(64)/*+(20)*/, src->tx256-src->x256);
			src->vx256 = ((sin512(angle512)));
			src->vy256 = ((cos512(angle512)));
		}
		gamen_gai_nara_osimai(src);/* 画面外ならおしまい */
		break;/*??? [***090215 追加 */
	}

	/* 移動する */
	src->x256+=(((src->vx256)*src->speed256)>>8);
	src->y256+=(((src->vy256)*src->speed256)>>8);
//
	src->m_angleCCW512 += 5;
	mask512(src->m_angleCCW512);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_kedama1(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_59_HAI_KEDAMA;
	add_zako_common(l, h);
	h->color32				= 0xaaffffff;		/*白っぽく */
	h->callback_mover		= move_kedama1;
//
	h->state				= ST00;
	h->time_out 			= 64;
	/* 第一目標はプレイヤーのいた位置 */
	h->tx256				= player->x256;
	h->ty256				= player->y256;
	h->vx256				= (0);
	h->vy256				= (0);
//	h->speed256 			= ((ra_nd()&((256*2)-1)))*(1+difficulty+enemy_rank/3);/*fps_factor*/
	h->speed256 			= ((ra_nd()&((256*2)-1))) + ((difficulty<<8)+ (/*enemy_rank*/5<<7/**86*2*/) );/*fps_factor*/


}
//	int enemy_rank	= l->user_y; 3 4 5


//	/*src->base.*/s->base_score = score(/*50*/5*2)*(enemy_rank+1);
//	/*src->base.*/s->base_hp		= (8*(8*8))+(1/*di fficulty*/<<(2+3))/*(1+(di fficulty<<2))*/;/*やわらかすぎ*/

//	h->callback_loser		= lose_random_item;
//	h->callback_hit_enemy	= callback_hit_zako;
//	h->y256 				= ((ra_nd()&((32*256)-1))-t256(80));//	(ra_nd()%40-90);

//	h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	switch (i&3/*i%5*/)
//	{
//	case 0: h->x256 		=				   (ra_nd()&((64*256)-1))/*ra_nd()%40*/; break;
//	case 1: h->x256 		= t256(GAME_WIDTH)-(ra_nd()&((64*256)-1))/*ra_nd()%40*/; break;
//	case 2: h->x256 		= t256(200)+	   (ra_nd()&((64*256)-1))/*ra_nd()%40*/; break;
//	case 3: h->x256 		= t256(90)+ 	   (ra_nd()&((64*256)-1))/*ra_nd()%40*/; break;
//	case 4: h->x256 		= t256(310)+	   (ra_nd()&((64*256)-1))/*ra_nd()%40*/; break;
//	}

