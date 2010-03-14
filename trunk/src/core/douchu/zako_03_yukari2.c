
#include "douchu.h"

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

//#define tx256		user_data00
//#define ty256		user_data01
//#define time_out	user_data02
#define speed256	user_data03
#define state		user_data04

//2#define enemy_rank	user_data05
#define max_y256	user_data05


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_yukari2(SPRITE *src)
{
	switch (src->state)
	{
	case 0: /* nach unten */
		if (src->y256 >= src->max_y256)
		{
			src->state=1;
			src->tmp_angleCCW512 = atan_512(t256(0)-src->y256,t256(GAME_WIDTH/2)-src->x256);
			if (t256(1.5/*2.0*/) < src->speed256)
			{	/*減速*/
				src->speed256 -= t256(0.2);
			//	src->speed256 = t256(2)/*3+difficulty*/;								// [***090201		変更
			}
			/* CCWの場合 */
			src->vx256=((sin512((src->tmp_angleCCW512))*src->speed256)>>8);
			src->vy256=((cos512((src->tmp_angleCCW512))*src->speed256)>>8);
		}
		if (ENEMY_LAST_SHOT_LINE256 > src->y256)	/* このラインより下からは敵が撃たない */
		{
			//if (src->enemy_rank)
			{
			//	if (0==(ra_nd()%(105-(src->enemy_rank-2+difficulty)*10)))	// [***090126		若干変更
			//	if (0==(ra_nd()%(11-(src->enemy_rank-2+difficulty) ))) 	// [***090126		若干変更
			//	if (0==(ra_nd()%(16-(src->enemy_rank-2+difficulty) ))) 	// [***090126		若干変更
			//	if ((src->enemy_rank-2+difficulty) >= (ra_nd()&(16-1)))	// [***090126		若干変更
				if ((/*src->enemy_rank*/2-2+difficulty) >= (ra_nd()&(64-1)))	// [***090126		若干変更
				{
					bullet_create_aka_maru_jikinerai(src, t256(1)+t256(difficulty)+(/*src->enemy_rank*/2<<6)/*t256(src->enemy_rank/3)*/ );
				}
			}
		}
		break;
	case 1:
		gamen_gai_nara_osimai(src);/* 画面外ならおしまい */
		break;
	}
	/*似てるがちょっと違う--以下rwingx.cと同じ*/
	src->x256 += (src->vx256);/*fps_factor*/
	src->y256 += (src->vy256);/*fps_factor*/
//
	src->m_angleCCW512 += 5;/*グラ回転*/
	mask512(src->m_angleCCW512);
}


#if 0
	case 0: 	/* 右へ移動中 */
		if (src->x >= src->clip_right)
		{	src->state=1;}
		break;
	case 1: 	/* 右周りで回転中 */
		src->tmp_angleCCW512 -= src->turnspeed512;/*fps_factor*/
		if (src->tmp_angleCCW512 <= deg_360_to_512(180))
		{
			src->tmp_angleCCW512 = deg_360_to_512(180);
			src->state=2;
			if (0 < src->enemy_rank)
			{	bullet_create_aka_maru_jikinerai(s, t256(3)+(src->enemy_rank<<7) );}
		}
		break;
	case 2: 	/* 左へ移動中 */
		if (src->x <= src->clip_left)
		{	src->state=3;}
		break;
	case 3: 	/* 左周りで回転中 */
		src->tmp_angleCCW512 += src->turnspeed512;/*fps_factor*/
		if (src->tmp_angleCCW512 >= deg_360_to_512(360) )
		{
			src->tmp_angleCCW512 = deg_360_to_512(0);
			src->state=0/*4*/;
			if (0 < src->enemy_rank)
			{	bullet_create_aka_maru_jikinerai(s, t256(3)+(src->enemy_rank<<7) );}
		}
		break;

	case 8: 	/* 右へ移動中 */
		if (src->x > GAME_WIDTH)
		{	src->flags &= (~(SP_FLAG_VISIBLE));}
		break;
#endif

/*---------------------------------------------------------
	自機狙い弾の角度を計算
---------------------------------------------------------*/
static int common_angle_nerai512(SPRITE *p, SPRITE *t)
{
	#if 1
	return (atan_512((p->y256-t->y256), (p->x256-t->x256)));
	/* 偶数弾の場合に自機狙い(簡略版) 弾と自分が大きさが同じならずれない、違うとその分誤差になる */
	#endif
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_yukari2(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_61_NIJI_HOSI;
	add_zako_common(l, h);
	h->callback_mover		= move_yukari2;
//
	h->max_y256 			= (t256(GAME_HEIGHT)-((t256(24)/*s->h128+s->h128*/))-t256(60));
	h->tmp_angleCCW512 		= common_angle_nerai512(player,h);
//	h->tmp_angleCCW512		= atan_512(
//		(t256(GAME_HEIGHT)-((t256(24)/*s->h128+s->h128*/))-t256(60))-h->y256,
//		t256(GAME_WIDTH/2)-h->x256);
	h->speed256 			= (t256(2.5/*3.0*/)+((3/*di fficulty*/)<<4) ); /*4*/ /*始めだけは速い*/
	/* CCWの場合 */
	h->vx256=((sin512((h->tmp_angleCCW512))*h->speed256)>>8);
	h->vy256=((cos512((h->tmp_angleCCW512))*h->speed256)>>8);
//
	h->state 			= 0;
	//h->enemy_rank		= 2/*enemy_rank*/;
}
