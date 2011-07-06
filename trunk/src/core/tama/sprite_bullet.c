
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	スプライト マネージャ
	-------------------------------------------------------
--------------------------------------------------------- */
		#if (0)/*なんだかよくわからんなー*/
//	dx = ((obj2->cx256) - (obj1->cx256));
//	dy = ((obj2->cy256) - (obj1->cy256));
//	dx = ((obj2->cx256/*+(t256(15.0))*/) - (obj1->cx256+(obj1->w<<7)));
//	dy = ((obj2->cy256/*+(t256(15.0))*/) - (obj1->cy256+(obj1->h<<7)));
	// 本来の位置より右に２－３ドット、上に１－２ドットずれてる。
//	dx = ((obj2->cx256/*+(t256(15.0))*/) - (obj1->cx256+(obj1->w<<7)));
//	dy = ((obj2->cy256+(t256(4.0))) - (obj1->cy256+(obj1->h<<7)));/*何故か上にズレてるので補正してみる*/
	// 左右は判らないが、上にまだ１－２ドットずれてる。
//	dx = ((obj2->cx256+(t256(3.0))) - (obj1->cx256+(obj1->w<<7)));/*何故か右にズレてるので補正してみる*/
//	dy = ((obj2->cy256+(t256(6.0))) - (obj1->cy256+(obj1->h<<7)));/*何故か上にズレてるので補正してみる*/
	// 左右は判らないが、上にまだ１－２ドットずれてる。
//		dx = ((obj2->cx256+(t256(3.0))) - (obj1->cx256+(obj1->w<<7)));/*何故か右にズレてるので補正してみる*/
//		dy = ((obj2->cy256+(t256(9.0))) - (obj1->cy256+(obj1->h<<7)));/*何故か上にズレてるので補正してみる*/
		#endif
	//	#else
	//	/* デバッグモード */
	//	dx = ((obj2->cx256+(de bug_value1<<8)) - (obj1->cx256+(obj1->w<<7)));/*何故か右にズレてるので補正してみる*/
	//	dy = ((obj2->cy256+(de bug_value2<<8)) - (obj1->cy256+(obj1->h<<7)));/*何故か上にズレてるので補正してみる*/

/*---------------------------------------------------------
	あたり判定 collision
	-------------------------------------------------------
	「あたり判定」とは、objがあたっているかどうかの判定。
	「あたり判定」とは、名ばかりで、実は「あたっている場合」を判定すると遅い。
	-------------------------------------------------------
	「あたっている場合」を判定するのではなく、
	「あたっていない場合」を判定する方が効率が良い(速い)。
	-------------------------------------------------------
	なぜ効率が良いかと言うと、判定領域が複雑な場合、「分割処理」をして判定すれば良い。
	「あたっている場合」を判定する場合「分割処理が複雑」だが、
	「あたっていない場合」を判定する場合は「分割処理が簡単」。
	-------------------------------------------------------
	最後まで「あたっていない場合」以外ならば、つまりあたっているという事。
--------------------------------------------------------- */

/*---------------------------------------------------------
	レーザー あたり判定
	-------------------------------------------------------
	-------------------------------------------------------
	分割処理１．判定有効なオブジェクト
	分割処理２．領域大小判定(直線外はあたらない。)
	分割処理３．距離で判定(円内はあたらない。)
	-------------------------------------------------------
	レーザーの仕様:
	//	※1. レーザー発生源から一定距離(laser1->ar256)は当たらない。//※3.に統合
		※2. レーザーは一定の太さ(laser1->aw256)がある。太さの外側は当たらない。
		※3. レーザー発生源から、反対方向はあたらない。
		※4. レーザーは発生源から(laser1->angle65536)[n/65536度]傾いている。
.	-------------------------------------------------------
  .     ↓レーザー発生源
.   .  ---
 .    .     .
   . |   x   |半径(laser1->ar256)の円。円内はあたらない。(※1.)
      .     .
   /    ---   .こちらの領域は反対側なのであたらない。(※3.)
  /    / .     .
 /    /    .     .
     /一定の太さ(laser1->aw256)の直線。直線外はあたらない。(※2.)
    /         .
   /            .
  /               .こちらの領域は反対側(レーザー発生源から一定距離)なのであたらない。(※1. ＆ ※3.)
 /


     →あたり判定領域を回転させる。

--------------------------------------------------------- */
#if 0/* めも */
int collision_laser( /* obj_t */SPRITE *obj1, LASER *laser1)
{
//	分割処理１．判定有効なオブジェクト

//	判定領域を -(laser1->angle65536) 回転させる。(傾きと逆方向に回転させる)
	int sin_angle;
	int cos_angle;
	int center_x256;
	int center_y256;
	int sabun_x;
	int sabun_y;
	int rotation_angle1024;
	int hantei_x;
//	int hantei_y;
	/* レーザー発生源を原点とする。 */
	center_x256 = (laser1->cx256);
	center_y256 = (laser1->cy256);
	/* 原点からの差分ベクトル成分。 */
	sabun_x = (obj1->cx256)-(center_x256);
	sabun_y = (obj1->cy256)-(center_y256);
	/* (傾きと逆方向に回転させる) */
	rotation_angle1024 = -((laser1->angle65536)>>(6));
	/* 固定値 */
	sin_angle = (int)(int256_sin1024(/*rot_sin*/((/*OFFS_SIN1024+*/ 		  rotation_angle1024)&(1024-1)))/*<<8*/);
	cos_angle = (int)(int256_sin1024(/*rot_cos*/((	OFFS_COS1024+			  rotation_angle1024)&(1024-1)))/*<<8*/);
	#if 1
	/* [下CCW]下が 0度で反時計回りの角度系 */
	hantei_x = (((sabun_x256 * sin_angle) + (sabun_y256 * cos_angle))>>8);
	hantei_y = (((sabun_x256 * cos_angle) - (sabun_y256 * sin_angle))>>8);
	#endif
//	分割処理２．領域大小判定(直線外はあたらない。)
//	if ( (0) > hantei_y)/* レーザー発生源から、反対方向はあたらない。 */
	if ( (laser1->ar256) > hantei_y)/* レーザー発生源から、反対方向はあたらない。 */
	{
		return (0/* FALSE */);	/* not hit. 当たってない */
	}
	if ( (0+(laser1->aw256)) > hantei_x)/* 一定の太さ(laser1->aw256)の直線。直線外はあたらない。 */
	{
		return (0/* FALSE */);	/* not hit. 当たってない */
	}
	if ( (0-(laser1->aw256)) < hantei_x)/* 一定の太さ(laser1->aw256)の直線。直線外はあたらない。 */
	{
		return (0/* FALSE */);	/* not hit. 当たってない */
	}

//	分割処理３．距離で判定(円内はあたらない。)
//	if ( laser1->ar256 > ra256)
//	{
//		return (0/* FALSE */);	/* not hit. 当たってない */
//	}
	return (1/* TRUE */); /* hit! 当たった */
}

#endif

/*---------------------------------------------------------
	矩形/円 あたり判定
	-------------------------------------------------------
	オブジェクト同士の当たり判定
	大まかに矩形で判別した後、近そうなら円の衝突判定
	-------------------------------------------------------
	分割処理１．判定有効なオブジェクト
	分割処理２．矩形判定
	分割処理３．円の半径で判定
--------------------------------------------------------- */

#if (1)
static /* BOOL */int collision_hit( /* obj_t */SPRITE *obj1, /* obj_t */SPRITE *obj2 )
{
	#if (0)/* この機能は要らない(?)かも。 */
	/*--- 判定有効なオブジェクトでなかったら終了 */
//	if (obj2->m_Hit256R == 0/* .0 */ /* FALSE */)
	if ( (16) > obj2->m_Hit256R /* FALSE */)/*t256(1.00)t256(0.10)*/
	{
		return (0/* FALSE */);	/* not hit. 当たってない */
	}
//	if (obj1->m_Hit256R == 0/* .0 */ /* FALSE */)
	if ( (16) > obj1->m_Hit256R /* FALSE */)/*t256(1.00)t256(0.10)*/
	{
		return (0/* FALSE */);	/* not hit. 当たってない */
	}
	#endif
//
	int dr; 	/* 半径(==radius) */
	int dx; 	/* x差分(微小x, delta x) */
	int dy; 	/* y差分(微小y, delta x) */
	dr = (obj2->m_Hit256R + obj1->m_Hit256R);
	#if 1/*Gu(中心座標)*/
	dx = ((obj2->cx256) - (obj1->cx256));/* 中心同士で差を求める。 */
	dy = ((obj2->cy256) - (obj1->cy256));/* 中心同士で差を求める。 */
	#endif

	/* check collision, delta x position. 矩形判定（Ｘ軸） */
	if (dx < 0/* .0 */) 	{	dx = (-dx); 			}	/* dx = abs(dx); */
	if (dx > dr)			{	return (0/* FALSE */);	}	/* not hit, because not near the dx. Ｘの差分が大きいので、当たってない */

	/* check collision, delta y position. 矩形判定（Ｙ軸） */
	if (dy < 0/* .0 */) 	{	dy = (-dy); 			}	/* dy = abs(dy); */
	if (dy > dr)			{	return (0/* FALSE */);	}	/*	not hit, because not near the dy. Ｙの差分が大きいので、当たってない */

	/* check collision, circle delta round distance.  円の半径で判定 */
	dr *= dr;
	dx *= dx;
	dy *= dy;
	dx += dy;
	/* 当たり判定外なら */
	if (dr < dx)/* if ( (dr^2) < ( (dx^2) + (dy^2) )) */
	{
		return (0/* FALSE */);	/* not hit. 当たってない */
	}
	return (1/* TRUE */); /* hit! 当たった */
}
#endif


/*---------------------------------------------------------
	スプライトのあたり判定
	-------------------------------------------------------
	入力値: type
		SP_GROUP_BULLETS	(弾幕)
		SP_GROUP_ITEMS		(アイテム)
--------------------------------------------------------- */
/*---------------------------------------------------------
	(敵専用)スプライトのあたり判定
--------------------------------------------------------- */

global SPRITE obj44[SPRITE_444POOL_MAX];	/* 弾専用スプライトのリスト構造 */
global SPRITE obj33[SPRITE_333POOL_MAX];	/* 敵専用スプライトのリスト構造 */
global SPRITE obj00[SPRITE_111POOL_MAX];	/* 自機等固定スプライト  */
global SPRITE obj22[SPRITE_222POOL_MAX];	/* パネル用スプライト  */

static SPRITE *sprite_collision_check(SPRITE *tocheck, SPRITE *s, int type, unsigned int length)
{
	unsigned int ii;
	for (ii=0; ii<length; ii++ )/* 全部調べる。 */
	{
	//	if (SP_DELETE != s->type)/* 消去済みは飛ばす。 */
		if (JYUMYOU_NASI < s->jyumyou)/* 消去済みは飛ばす。 */
		{
			/* 1. 自分自身チェック */
			/* 判定対象(自分自身)は飛ばす(全く同じもの同士のあたり判定を取っても意味がないので取らない) / zu testendes Sprite ueberspringen */
			if (s != tocheck)	/* Is sprites my self?(not need self collisions.) */
			{
				/* 2. 判定種類チェック */
				if (
					//		(s->type != SP_DELETE ) && /* 削除済みは飛ばす */	/* SP_DELETEが 0 になったので要らなくなった． */
							(s->type & type)			/* typeが一致する場合のみ */	/* Passender Sprite-Type? */
					)	/* do collision only same type. */
				{
					/* 3. 判定フラグチェック */
					if (
							( (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/) == (s->flags&(SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/)))
							/* あたり判定があり、かつ、表示可能なもののみチェック */
						)	/* do collision only visible and, use collision check type. */
					{
						#if 1
						/* 4. 矩形/円あたり判定チェック   大まかに矩形で判別した後、近そうなら円の衝突判定 */
						if (collision_hit(s,tocheck))	/* 矩形/円あたり判定 */ 	/* hit collision rectangle to circle check. */
						{
							return (s);/* あたった */	/* hit collisioning! */
						}
						#endif
					}
				}
			}
		}
		s++;
	}
	return (NULL);/* あたってない */	/* no hit collisioning. */
}
/*static*/global SPRITE *sprite_collision_check_444(SPRITE *tocheck, int type)
{
	SPRITE *s;
	s = &obj44[0];
	return sprite_collision_check(tocheck, s, type, SPRITE_444POOL_MAX);
}
//	/*static*/global SPRITE *sprite_collision_check_SDL_teki(SPRITE *tocheck, int type)
/*static*/global SPRITE *sprite_collision_check_SDL_teki(SPRITE *tocheck)/*, (SP_GROUP_TEKI)int ty pe*/
{
	SPRITE *s;
	s = &obj33[0];
	return sprite_collision_check(tocheck, s, /*type*/(SP_GROUP_TEKI), SPRITE_333POOL_MAX);
}




/*---------------------------------------------------------
	スプライト リストに登録されたスプライトを全部消す。
--------------------------------------------------------- */
static void sprite_remove_all(SPRITE *s, unsigned int length)
{
	unsigned int ii;
	for (ii=0; ii<length; ii++ )/* 全部調べる。 */
	{
		s->jyumyou	= JYUMYOU_NASI;
		#if (1)/* デバッグてすと */
		s->cx256		= (-1);
		#endif
		s++;
	}
}
//extern void sprite_remove_all_SDL_void(void); 		/*int type*/ /*gu汎用(旧SDL)*/
//extern void sprite_remove_all_444_void(void); 		/*int type*/ /*弾幕専用*/

static void s_sprite_remove_all_444_void(void)/*int ty pe*/
{
	SPRITE *s;
	s = &obj44[0];
	sprite_remove_all(s, SPRITE_444POOL_MAX);
}
static void s_sprite_remove_all_SDL_void(void)/*int ty pe*/
{
	SPRITE *s;
	s = &obj33[0];
	sprite_remove_all(s, SPRITE_333POOL_MAX);
}
global void sprite_all_cleanup(void)
{
	s_sprite_remove_all_SDL_void();/*gu汎用*/	/*SP_GROUP_ALL_GAME_OBJS*/
	s_sprite_remove_all_444_void();/*弾幕専用*/ /*SP_GROUP_ALL_GAME_OBJS*/
//	sprite_remove_all222(SP_GROUP_ALL_GAME_OBJS);/*弾幕用*/
}

/*---------------------------------------------------------
	スプライトを一フレーム分動作させる。
--------------------------------------------------------- */
static void sprite_move_main(SPRITE *s, unsigned int length)
{
	unsigned int iii;
//	for (iii=0; iii<length; iii++ )/* 全部調べる。 */
	iii = length;/* レーザーの実装を簡単にする為に、逆順にする。 */
	{
		my_loop:
	//	#if 1/* 要らない気がする(てすとちう) */
	//	if (s->type != SP_DELETE ) /* 削除済みは飛ばす */
	//	#endif
		if (JYUMYOU_NASI < s->jyumyou)/* 消去済みは飛ばす。 */
		{
			s->jyumyou--;/* 寿命経過 */
		//	#if 0
		//	if (s->type != SP_DELETE ) /* 削除済みは飛ばす */	/* SP_DELETEが 0 になったので要らなくなった． */
		//	#endif
			{
				if (s->type & (SP_GROUP_ALL_SDL_CORE_TYPE)/*type*/) /* typeが一致する場合のみ */
				{
					if (NULL != s->callback_mover)
					{
						(s->callback_mover)(s);
					}
				}
			}
		}
		s--;//	s++;/* レーザーの実装を簡単にする為に、逆順にする。 */
		iii--;
		if (0 != iii)
		{
			goto my_loop;
		}
	}
}

static void s_sprite_move_main_Gu_444(void)
{
	SPRITE *s;
	s = &obj44[0+SPRITE_444POOL_MAX];
	sprite_move_main(s, SPRITE_444POOL_MAX);
	/* 道中の場合勝手に食み出しチェックを行い弾を消す(暫定的) */
	if (0==(pd.state_flag & STATE_FLAG_05_IS_BOSS))
	{
		bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
	}
}
//extern void sprite_move_main_SDL_222(void);			/*int type*/ /*gu汎用(旧SDL)*/
//extern void sprite_move_main_Gu_444(void);				/*int type*/ /*gu弾幕専用*/

static void s_sprite_move_main_SDL_222(void)
{
	SPRITE *s;
	s = &obj33[0+SPRITE_333POOL_MAX];
	sprite_move_main(s, SPRITE_333POOL_MAX);
}
static void s_sprite_move_main_SDL_111(void)
{
	SPRITE *s;
	s = &obj00[0+SPRITE_111POOL_MAX];
	sprite_move_main(s, SPRITE_111POOL_MAX);
}
global void sprite_move_all(void)/*int ty pe*/
{
		//controller_work();
//		sprite_work222(SP_GROUP_ALL_SDL_WORK_TYPE);/*弾幕用*/
//		sprite_work000(SP_GROUP_ALL_SDL_WORK_TYPE);
		s_sprite_move_main_SDL_111();/* 自機等固定オブジェクト */
		s_sprite_move_main_SDL_222();/*gu汎用*/ 	/*SP_GROUP_ALL_SDL_CORE_TYPE*/
		s_sprite_move_main_Gu_444();/*弾幕専用*/	/*SP_GROUP_ALL_SDL_CORE_TYPE*/

}
#if 1
/*---------------------------------------------------------
	画面外ならおしまい(個別、主に汎用objで使用)
---------------------------------------------------------*/
global void gamen_gai_nara_zako_osimai(SPRITE *src)
{
	if (
		(src->cx256 < t256(0)) ||
		(src->cx256 > t256(GAME_WIDTH)) ||
		(src->cy256 < t256(0)) ||
		(src->cy256 > t256(GAME_HEIGHT)) )		/* 画面外に出たら */
	{
		src->jyumyou = JYUMYOU_NASI;		/* おしまい */
	}
}
//	if ((src->cx256 < t256(0))||(src->cx256 > t256(GAME_WIDTH))||
//		(src->cy256 < t256(0))||(src->cy256 > t256(GAME_HEIGHT)))
//	{
//		src->jyumyou = JYUMYOU_NASI;/*画面外にでたらおしまい*/
//	}
/*---------------------------------------------------------
	画面外ならおしまい(個別、主に汎用objで使用)
---------------------------------------------------------*/
//global void gamen_gai_limit_30_nara_zako_osimai(SPRITE *src)
//{
//	if ((JYUMYOU_ZAKO_CLIP) > src->jyumyou) 		/* 離脱 */
//	{
//		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
//	}
//}
#endif


#if 1
/*---------------------------------------------------------
	画面外(弾幕)
---------------------------------------------------------*/

/* 検討中 */

/*extern*/global POINT256 bullet_clip_min;		/*	範囲(最小値) */
/*extern*/global POINT256 bullet_clip_max;		/*	範囲(最大値) */
#endif


#if 1
/*---------------------------------------------------------
	画面外ならおしまいのみ(弾幕)
---------------------------------------------------------*/

static void s_check_hamidasi_bullet_angle_one(SPRITE *src)
{
	#if 1/* 検討中 */
//	if ((src->cx256 < t256(-100))||(src->cx256 > t256(100)+t256(GAME_WIDTH))||
//		(src->cy256 < t256(-256))||(src->cy256 > t256(100)+t256(GAME_HEIGHT) ) )
	if (
		(src->cx256 < bullet_clip_min.x256)||
		(src->cx256 > bullet_clip_max.x256)||
		(src->cy256 < bullet_clip_min.y256)||
		(src->cy256 > bullet_clip_max.y256) )
	{
		src->jyumyou = JYUMYOU_NASI;
	}
	#endif
}

/*---------------------------------------------------------
	弾の喰み出しチェックを行う(毎フレーム行う必要はない)
---------------------------------------------------------*/

global void bullet_angle_all_gamen_gai_nara_kesu(void)
{
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &obj44[ii];
		s_check_hamidasi_bullet_angle_one(s);
	}
}
#endif


/*---------------------------------------------------------
	左右が画面外なら反射(弾幕)
---------------------------------------------------------*/

#if 1/* 角度弾規格(策定案) */
	#define tx256				vx256/* 基点座標x */
	#define ty256				vy256/* 基点座標y */
	//
	#define radius256			user_data01 	/* 半径 */
//	#define speed256			user_data02 	/* 加減速 */
	#define speed65536			user_data02 	/* 加減速 */
	#define tra65536			user_data03 	/* 加減速調整 */
	#define rotate1024			user_data04 	/* 回転角度調整 */
	#define bullet_status_mask	user_data05 	/* 画面外消去判定や反射機能 */
#endif

#if 1/* 角度弾規格B(仮策定案) */
	/* 仮 */
	#define SAYUU_HANSYA_BIT	0x0001
#endif



#if 0
/*---------------------------------------------------------
	左右が画面外なら反射のみ(弾幕)
---------------------------------------------------------*/
static void s_check_hamidasi_bullet_angle_sayuu_hansya(SPRITE *src)
{
	#if 1/* 検討中 */
	int test_flag;
	test_flag = 0;
	if (
		(src->cx256 < t256(0)/*bullet_clip_min.x256*/) )
	{
		if (0==(src->bullet_status_mask & SAYUU_HANSYA_BIT))
				{	test_flag = 1;			}
		else	{	src->jyumyou = JYUMYOU_NASI;	}
	}
	else
	if (
		(src->cx256 > t256(GAME_WIDTH)/*bullet_clip_max.x256*/) )
	{
		if (0==(src->bullet_status_mask & SAYUU_HANSYA_BIT))
				{	test_flag = 1;			}
		else	{	src->jyumyou = JYUMYOU_NASI;	}
	}
	if (0 != test_flag)
	{
			src->bullet_status_mask |= SAYUU_HANSYA_BIT;
		//	src->tx256 = - src->tx256;
		//	src->tx256 = t256(GAME_WIDTH+GAME_WIDTH)/*bullet_clip_max.x256 + bullet_clip_max.x256*/ - src->tx256;
			#if 1
			/* 現在の座標を新基点座標に変更し、半径0にする。 */
			src->tx256 = src->cx256;
			src->ty256 = src->cy256;
			src->radius256 = 0;
			#endif
		//	src->rotationCCW1024 += (1024/4);/* 90度回転 */ 		/* ダメ[※2] */
		//	src->rotationCCW1024 += (1024*3/4);/* -90度回転 */		/* ダメ[※2] */
			src->rotationCCW1024 = (1024)-(src->rotationCCW1024);	/* 反転[※1] */
			mask1024(src->rotationCCW1024);
	}
	if (
		(src->cy256 < - t256(100)/*bullet_clip_min.y256*/)||
		(src->cy256 >   t256(272)/*bullet_clip_max.y256*/)
	)
	{
		src->jyumyou = JYUMYOU_NASI;
	}
	#endif
}


/*---------------------------------------------------------
	弾の喰み出しチェックを行う(毎フレーム行う必要はない)
---------------------------------------------------------*/

global void bullet_angle_all_gamen_gai_nara_hansya(void)
{
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &obj44[ii];
		s_check_hamidasi_bullet_angle_sayuu_hansya(s);
	}
}

#endif


#if 1
/*---------------------------------------------------------
	左右が画面外なら反射減速(弾幕)
---------------------------------------------------------*/

static void s_check_hamidasi_bullet_angle_sayuu_hansya_gensoku(SPRITE *src)
{
	#if 1/* 検討中 */
//	if ((src->cx256 < t256(-100))||(src->cx256 > t256(100)+t256(GAME_WIDTH))||
//		(src->cy256 < t256(-256))||(src->cy256 > t256(100)+t256(GAME_HEIGHT) ) )
	int test_flag;
	test_flag = 0;
	if (
		(src->cx256 < t256(0)/*bullet_clip_min.x256*/) )
	{
		if (0==(src->bullet_status_mask & SAYUU_HANSYA_BIT))
				{	test_flag = 1;			}
		else	{	src->jyumyou = JYUMYOU_NASI;	}
	}
	else
	if (
		(src->cx256 > t256(GAME_WIDTH)/*bullet_clip_max.x256*/) )
	{
		if (0==(src->bullet_status_mask & SAYUU_HANSYA_BIT))
				{	test_flag = 1;			}
		else	{	src->jyumyou = JYUMYOU_NASI;	}
	}
	if (0!=test_flag)
	{
			src->bullet_status_mask |= SAYUU_HANSYA_BIT;
		//	src->tx256 = t256(GAME_WIDTH+GAME_WIDTH)/*bullet_clip_max.x256 + bullet_clip_max.x256*/ - src->tx256;
		//	src->tx256 = - src->tx256;
			#if 1
			/* 現在の座標を新基点座標に変更し、半径0にする。 */
			src->tx256 = src->cx256;
			src->ty256 = src->cy256;
			src->radius256 = 0;
			#endif
		//	src->rotationCCW1024 += (1024*3/4);/* -90度回転 */		/* ダメ[※2] */
		//	src->rotationCCW1024 += (1024/4);/* 90度回転 */ 		/* ダメ[※2] */
			src->rotationCCW1024 = (1024)-(src->rotationCCW1024);	/* 反転[※1] */
		//	src->speed256					= ((src->speed256)>>1);/*減速*/
			src->speed65536 				= ((src->speed65536)>>(1));/*減速*/
			mask1024(src->rotationCCW1024);
	}
	if (
		(src->cy256 < - t256(100)/*bullet_clip_min.y256*/)||
		(src->cy256 >   t256(272)/*bullet_clip_max.y256*/)
	)
	{
		src->jyumyou = JYUMYOU_NASI;
	}
	#endif
}
/*---------------------------------------------------------
	弾の喰み出しチェックを行う(毎フレーム行う必要はない)
---------------------------------------------------------*/

global void bullet_angle_all_gamen_gai_nara_hansya_gensoku(void)
{
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &obj44[ii];
		s_check_hamidasi_bullet_angle_sayuu_hansya_gensoku(s);
	}
}

#endif



/*---------------------------------------------------------
	共通ボス、初期化off
	スプライトを確保したまま退避させる。
---------------------------------------------------------*/
global void sprite_initialize_position(SPRITE *h)
{
	h->m_Hit256R					= ZAKO_ATARI16_PNG;
	h->flags				&= (~(SP_FLAG_OPTION_VISIBLE)); 	/* 可視フラグのOFF(不可視) */
	h->flags				&= (~(SP_FLAG_COLISION_CHECK)); 	/* あたり判定のOFF(無敵) */
	h->type 						= BOSS_00_BOSS11;
	h->callback_mover				= NULL;
	h->callback_loser				= NULL;
	h->callback_hit_enemy			= NULL; 	/* ダミーコールバック登録 */
	h->cx256 		= (t256(GAME_WIDTH/2));
	h->cy256 		= (t256(-256));
}

/*---------------------------------------------------------
	スプライトの初期化をする。
	-------------------------------------------------------
	リスト方式でないスプライトも初期化する為、
	外部からも呼ばれるので注意。
--------------------------------------------------------- */
//tern void sprite_initialize_gu(SPRITE *obj);
global void sprite_initialize_gu(SPRITE *obj)
{
	#if (0==USE_MEM_CLEAR)/* [メモリゼロクリアー機能]を使わない場合は、初期化が必要。 */
	#else
	memset(obj, 0, sizeof(SPRITE));
	#endif
//
	obj->flags		= 0;				/*set_flags*/
	obj->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	obj->alpha		= 0xff;*/
	#if 1/* 自動的に消える機能 */
	obj->jyumyou	= JYUMYOU_1MIN; 		/* 1分したら勝手に自動消去。 */
//	obj->timeover_ticks 		= 0;
	#endif
	#if (1==USE_ZOOM_XY)
	obj->m_zoom_x256			= t256(1.00);	/* 表示拡大率 256 == [ x 1.00 ] */
	obj->m_zoom_y256			= t256(1.00);	/* 表示拡大率 256 == [ x 1.00 ] */
	#else
	obj->m_zoom_xy256			= t256(1.00);	/* 表示拡大率 256 == [ x 1.00 ] */
	#endif
	#if (0==USE_MEM_CLEAR)/* [メモリゼロクリアー機能]を使わない場合は、初期化が必要。 */
	obj->rotationCCW1024		= 0;	/* 描画用角度(下が0度で左回り(反時計回り)) */	/* 0 == 傾かない。下が0度 */
//
	obj->vx256					= t256(0);
	obj->vy256					= t256(0);
//
	obj->callback_mover 		= NULL;
	obj->callback_loser 		= NULL;
	obj->callback_hit_enemy 	= NULL;
	#endif	/* (0==USE_MEM_CLEAR) */
}


/*---------------------------------------------------------
	gu移行中、弾専用スプライトをリストに追加する。
	gu移行中、汎用スプライトをリストに追加する。
	登録できない場合、NULLを返す。
--------------------------------------------------------- */

global SPRITE *sprite_add_444only_bullet_error(void)/*int image_resource_num*/
{
	#if 1
	/* private (この関数で)プライベートな変数 */
	static int register_num;	/* 登録できる可能性が高そうな位置を保持 */
	#endif
//	if (登録できない場合)	{	return(NULL);	}
//	int priority;		priority		= image_resource_ptr->priority;
	SPRITE *obj;/* 新規作成するスプライト */
	{
		int search_count;
		search_count = 0;
my_retry:
		search_count++;
		if ( (SPRITE_444POOL_MAX-1) < search_count)
		{	return(NULL);	}	/* (登録できない場合) */

		register_num++; 	/* 登録できる可能性が高そうな位置 */
		register_num &= (SPRITE_444POOL_MAX-1);
		obj = &obj44[register_num];
	//
	//	if (SP_DELETE != obj->type)/* 使用中ならもう一度探す。 */
		if (JYUMYOU_NASI < obj->jyumyou)/* 使用中ならもう一度探す。 */
		{
			goto my_retry;	/* 探す。 */
		}
	}
	sprite_initialize_gu(obj);			/* 新規作成したスプライトを初期化 */
	#if 1
	/* 標準初期化 */
	obj->flags			|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);	/* あたり判定あり */
	#endif
	return (obj);
}

global SPRITE *sprite_add_gu_error(void)
{
	#if 1
	/* private (この関数で)プライベートな変数 */
	static int register_num;	/* 登録できる可能性が高そうな位置を保持 */
	#endif
//	if (登録できない場合)	{	return(NULL);	}
//	int priority;		priority		= image_resource_ptr->priority;
	SPRITE *obj;/* 新規作成するスプライト */
	{
		int search_count;
		search_count = 0;
my_retry:
		search_count++;
		if ( (SPRITE_333POOL_MAX-1) < search_count)
		{	return(NULL);	}	/* (登録できない場合) */

		register_num++; 	/* 登録できる可能性が高そうな位置 */
		register_num &= (SPRITE_333POOL_MAX-1);
		obj = &obj33[register_num];
	//
	//	if (SP_DELETE != obj->type)/* 使用中ならもう一度探す。 */
		if (JYUMYOU_NASI < obj->jyumyou)/* 使用中ならもう一度探す。 */
		{
			goto my_retry;	/* 探す。 */
		}
	}
	sprite_initialize_gu(obj);			/* 新規作成したスプライトを初期化 */
	return (obj);
}


global SPRITE *sprite_add_direct(unsigned int register_number)
{
//	int priority;		priority		= image_resource_ptr->priority;
	SPRITE *obj;/* 新規作成するスプライト */
	obj = &obj00[register_number];
	sprite_initialize_gu(obj);			/* 新規作成したスプライトを初期化 */
	return (obj);
}

#define PPP (GAME_WIDTH)
global void sprite_panel_init(void)
{
	unsigned int ii;
	for (ii=0; ii<SPRITE_222POOL_MAX; ii++ )/* 全部 */
	{
		SPRITE *obj;/* 初期化するスプライト */
		obj = &obj22[ii];
		sprite_initialize_gu(obj);			/* スプライトを初期化 */
	}
	unsigned int kk;
	kk = ((PPP+5*8+4)<<8);
//	unsigned int jj;
//	for (jj=0; jj<(2); jj++ )/* */
	for (ii=0; ii<(10); ii++ )/* */
	{
		SPRITE *obj;/* 初期化するスプライト */
		/* スコア */
		obj = &obj22[ii+PANEL_OBJ_00_S00];
		obj->cx256 = (kk);
		obj->cy256 = ((6*8+7)<<8);
		/* ハイスコア */
		obj = &obj22[ii+PANEL_OBJ_10_H00];
		obj->cx256 = (kk);
		obj->cy256 = ((3*8+2)<<8);
		//
		kk += (8*256);
	}
}

/*---------------------------------------------------------
	r32:とりあえずバグあるのを無理やり回避(?)
--------------------------------------------------------- */
global void sprite_test_debug_init(void)
{
	//ボス出ない。よくわかんない(???)sprite_all_cleanup();/* 弾と敵は全部消す。 */
	//

}
