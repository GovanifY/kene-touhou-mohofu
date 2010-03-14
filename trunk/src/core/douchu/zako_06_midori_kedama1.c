
#include "douchu.h"

/*---------------------------------------------------------
	"緑毛玉1",		"XEV",
	-------------------------------------------------------
	緑毛玉	単体で突っ込んでくる。
---------------------------------------------------------*/

#define tx256	user_data00
#define ty256	user_data01
#define state	user_data02

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_midori_kedama1(SPRITE *src)
{
	int speed256 = t256(2.5/*0*/);
	switch (src->state)
	{
	case 0:
		speed256 = t256(2.5/*3*/);
		if ((src->x256 >= player->x256)||
			(src->y256 > t256(GAME_HEIGHT-48) ) )/*480-80*/
		{
			src->state	= 1;
			src->tx256	= t256(-100);
		//	src->ty256	= player->y256-t256(48);
			src->ty256	= player->y256-t256(48);
			{
				bullet_create_aka_maru_jikinerai(src, t256(2.75) );
				lose_random_item(src);
			}//t256(1)+((4/*data->enemy_rank*/)<<8)
		}
		break;/* ??? [***20090210	追加  */
	case 1:
		speed256 = t256(3.5/*6*/);
		gamen_gai_nara_osimai(src);/* 画面外ならおしまい */
		break;/* ??? [***20090409	追加  */
	}
	{	int angle512;
		angle512 = atan_512(src->ty256-src->y256,src->tx256-src->x256);
/* CCWの場合 */
		src->x256 += ((sin512(angle512)*speed256)>>8);/*fps_factor*/
		src->y256 += ((cos512(angle512)*speed256)>>8);/*fps_factor*/
	}
//
	src->m_angleCCW512 += 5;
	mask512(src->m_angleCCW512);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_midori_kedama1(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_60_AKA_KEDAMA;
	add_zako_common(l, h);
	h->callback_mover		= move_midori_kedama1;
//
	h->tx256				= player->x256;
//	h->ty256				= player->y256; 		/* 特攻禁止 */
	h->ty256				= ((player->y256)>>1);
	h->state				= 0;
}
//	int enemy_rank; 	enemy_rank	= l->user_y; 2 4
//	data->enemy_rank		= enemy_rank;


//	/*data->base.*/s->base_score		= score(5*2);
//	/*data->base.*/s->base_hp		= (8*(8*8))+(1/*di fficulty*/<<(2+3))/*(1+(di fficulty<<2))*/;/*やわらかすぎ*/

//	h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->callback_hit_enemy	= callback_hit_zako;
//	switch (ra_nd()&(4-1)/*%3*/)
//	{
//	case 0: h->x256 		=							  (ra_nd()&((32*256)-1))/*(ra_nd()%20)*/; break;
//	case 1: h->x256 		= t256(GAME_WIDTH/3)		 +(ra_nd()&((32*256)-1))/*(ra_nd()%20)*/; break;
//	case 2: h->x256 		= t256(GAME_WIDTH  )		 -(ra_nd()&((32*256)-1))/*(ra_nd()%20)*/; break;
//	case 3: h->x256 		= t256(GAME_WIDTH/2)+t256(10)+(ra_nd()&((32*256)-1))/*(ra_nd()%20)*/; break;
//	}
//	h->y256 				= (ra_nd()&((32*256)-1))/*(ra_nd()%20)*/-t256(50);

