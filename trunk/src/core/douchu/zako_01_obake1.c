
#include "douchu.h"

/*---------------------------------------------------------
		"おばけ1",		"DRAGER",
	-------------------------------------------------------
	突撃おばけ、(かなりうろ覚え、おばけじゃなかったかも？)
	-------------------------------------------------------
仕様：
	■ datでscoreを指定する。
	■ datで体力を指定する。
	■ datで獲得アイテムを指定する。
	■ datで出現位置 x, y 座標を指定する。
	■ 雑魚の速度は難易度に関係なく固定。結構速め。(t256(2.0))
	■ 雑魚は自機狙いで突撃してくる。(途中で向きは変えません)
	■ 等間隔で弾を撃ってくる。(早く倒した方が有利)
		easy	撃たない。
		normal	1way弾。
		hard	3way弾。
		luna	5way弾。
	■ 弾の速度は、雑魚の速度より遅い。速度は難易度に関係なく固定。(t256(1.25))
	(画面に残るので早く倒した方が有利)
	-------------------------------------------------------
	つまりフラワーウイザラウェイの逆バージョンです。
	と言った方が分かりやすいかな？
---------------------------------------------------------*/

/*---------------------------------------------------------
	"隕石1", "紫 編 隊1",		"MING",
	-------------------------------------------------------
仕様：
	■ datでscoreを指定する。
	■ datで体力を指定する。
	■ datで獲得アイテムを指定する。
	■ datで出現位置 x, y 座標を指定する。



	単体でばらばらに、落ちてくるザコ敵
	自機狙い赤単弾を連射してくる
	-------------------------------------------------------
	道中に必要か悩んだが、元と似た仕様の編隊飛行敵は必要ない。
	単純に落ちてくる敵に変更する。
	-------------------------------------------------------
	10機三角錐状に続がり、編隊飛行してくるザコ敵
	ming
	rwingx_curverに似てる
---------------------------------------------------------*/

/* 敵の向き tmp_angleCCW512  */

#define time_out			user_data02 	/* 経過時間 */
#define gra_anime_type		user_data06 	/* グラタイプ */


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_obake1(SPRITE *src)
{
	src->time_out--;
	if (( 0 > src->time_out ))
	{
		gamen_gai_nara_osimai(src);/* 画面外ならおしまい */
	}
	if (0==((src->time_out)&0x3f))/* 等間隔(テキト－) */
	{
		if (ENEMY_LAST_SHOT_LINE256 > src->y256)	/* このラインより下からは敵が撃たない */
		{
			if (0<difficulty)
			{
				send1_obj->x256 = src->x256;
				send1_obj->y256 = src->y256;
				send1_obj->BULLET_REGIST_speed256			=	(t256(1.25));
				send1_obj->BULLET_REGIST_angle512			=	(src->tmp_angleCCW512); 	/*(ANGLE_JIKI_NERAI_DAN)*/
				send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(18));			/* 分割角度 */
				send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;		/* [青ナイフ弾] */
				send1_obj->BULLET_REGIST_n_way				=	(difficulty+difficulty-1);	/* [1 3 5way] */
				bullet_regist_basic();
			}
		}
	}
//
	{
		src->x256 += (src->vx256);/*fps_factor*/
		src->y256 += (src->vy256);/*fps_factor*/
	}
	if (SP_DELETE != src->type)
	{
		if (0==src->gra_anime_type)
		{
			/* offset無しは回転 */
			src->m_angleCCW512 += (5);/*グラ回転*/
			mask512(src->m_angleCCW512);
		}
		else
		{
			src->type			= TEKI_28_YOUSEI2_5+(((src->y256>>8)&0x03));
		}
	}
}

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

void add_zako_obake1(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_28_YOUSEI2_5;
	add_zako_common(l, h);
	h->callback_mover		= move_obake1;
	h->time_out 			= (60);
//
	h->tmp_angleCCW512 = common_angle_nerai512(player,h);
	h->vx256 = ((sin512((h->tmp_angleCCW512))));
	h->vy256 = ((cos512((h->tmp_angleCCW512))));
	h->vx256 += h->vx256;
	h->vy256 += h->vy256;
	h->gra_anime_type	= (1);	/* 0==回転アニメ、グラタイプ */
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_inseki1(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_61_NIJI_HOSI;
	add_zako_common(l, h);
	h->callback_mover		= move_obake1;
	h->time_out 		= (0x3ff);
//
	h->gra_anime_type	= (0);	/* 0==回転アニメ、グラタイプ */
	{
		/* 初期位置 */
		h->x256 = (((u32)l->user_x)<<8);//((rrrr)&0xffff) + t256((GAME_WIDTH-256)/2);
		h->y256 = -t256(30);
	//
	u32 rrrr;
		rrrr = (ra_nd());
		h->vx256 = ((rrrr)&0x01ff);/*右方向*/
		/* 画面右側の場合、左方向へ進む */
		if ( t256((GAME_WIDTH)/2) < h->x256)
		{
			h->vx256 = (-(h->vx256));/*左方向*/
		}
		h->vy256 = ((rrrr>>16)&0x01ff) + t256(1.00);/*下方向*/
	}
}
