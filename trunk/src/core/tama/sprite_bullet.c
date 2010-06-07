
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	スプライト マネージャ
	-------------------------------------------------------
	模倣風では、リスト方式(～r30)では 500弾を超えたあたりから、
	(ゲームが止まったのかと思うくらい)物凄く酷い処理落ちが始まります。
	そこで、リスト方式は廃止します。
	//
	配列方式なら、毎回配列を全走査しても 1000弾 ぐらいまでなら、
	とりあえず処理落ちしませんでした。
	//
	r31現在:弾幕だけ配列方式でザコは、リスト方式なので色々処理落ちがあります。
	ザコと弾幕も巧く統合したいです。(あたり判定があるので、混ぜるという意味ではない。
	共通して使える仕組みにするという事)
--------------------------------------------------------- */

/*---------------------------------------------------------
	あたり判定 collision
--------------------------------------------------------- */

/*---------------------------------------------------------
	矩形/円 あたり判定
	-------------------------------------------------------
	オブジェクト同士の当たり判定
	大まかに矩形で判別した後、近そうなら円の衝突判定
--------------------------------------------------------- */

#if (1)
static /* BOOL */int collision_hit( /* obj_t */SPRITE *obj1, /* obj_t */SPRITE *obj2 )
{
	/*--- 判定有効なオブジェクトでなかったら終了 */
//	if (obj2->m_Hit256R == 0/* .0 */ /* FALSE */)
	if (256 > obj2->m_Hit256R /* FALSE */)
	{
		return (0/* FALSE */);	/* not hit. 当たってない */
	}
//	if (obj1->m_Hit256R == 0/* .0 */ /* FALSE */)
	if (256 > obj1->m_Hit256R /* FALSE */)
	{
		return (0/* FALSE */);	/* not hit. 当たってない */
	}
//
	/* flo at */int dr, dx, dy;
	dr = (obj2->m_Hit256R + obj1->m_Hit256R);
	#if 0/*SDL(左隅座標)*/
	dx = ((obj2->x256+obj2->w128) - (obj1->x256+obj1->w128));
	dy = ((obj2->y256+obj2->h128) - (obj1->y256+obj1->h128));
	#else/*Gu(中心座標)*/
	dx = ((obj2->x256) - (obj1->x256));
	dy = ((obj2->y256) - (obj1->y256));
	#endif
	/* check collision, delta x position. 矩形判定（Ｘ軸） */
	if (dx < 0/* .0 */) 	{	dx = (-dx); }	/* dx = abs(dx); */
	if (dx > dr)			{	return (0/* FALSE */);	}	/* not hit, because not near the dx. Ｘの差分が大きいので、当たってない */

	/* check collision, delta y position. 矩形判定（Ｙ軸） */
	if (dy < 0/* .0 */) 	{	dy = (-dy); }	/* dy = abs(dy); */
	if (dy > dr)			{	return (0/* FALSE */);	}	/*	not hit, because not near the dy. Ｙの差分が大きいので、当たってない */

	/* check collision, circle delta round distance.  円の半径で判定 */
	dr *= dr;
	dx *= dx;
	dy *= dy;
	dx += dy;
	// 当たり判定外なら
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

//	/*static*/global SPRITE *sp rite_list444_head = NULL;	/* スプライトのリスト構造 */

//global SPRITE sprite_pool[SPRITE_POOL_MAX];	/* スプライトのリスト構造 */


global SPRITE sprite_444pool[SPRITE_444POOL_MAX];	/* スプライトのリスト構造 */

/*static*/global SPRITE *sprite_collision_check_444(SPRITE *tocheck, int type)
{
//	SPRITE *s = sp rite_list444_head;/* スプライト リストの先頭 から探す */
//	while (NULL != s)
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &sprite_444pool[ii];
	//
		if (SP_DELETE != s->type)/* 消去済みは飛ばす。 */
		{
			/* 1. 自分自身チェック */
			/* 判定対象(自分自身)は飛ばす(全く同じもの同士のあたり判定を取っても意味がないので取らない) / zu testendes Sprite ueberspringen */
			if (s != tocheck)	/* Is sprites my self?(not need self collisions.) */
			{
				/* 2. 判定種類チェック */
				if (
					//		(s->type != SP_DELETE ) && /* 削除済みは飛ばす */	/* SP_DELETEが 0 になったので要らなくなった． */
							(s->type & type)			 /* typeが一致する場合のみ */	/* Passender Sprite-Type? */
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
//			s = s->next;/* 次 */
		}
	}
	return (NULL);/* あたってない */	/* no hit collisioning. */
}


/*---------------------------------------------------------
	スプライト リストに登録されたスプライトを消す。
--------------------------------------------------------- */
#include <malloc.h>/* free(); */

#if 0
//extern SPRITE *sp rite_list444_head;
static void sprite_remove_one_444(SPRITE *src)
{
	SPRITE *s = sp rite_list444_head;/* スプライト リストの先頭 から探す */
	SPRITE *p = NULL;
	SPRITE *n = NULL;
	while (NULL != s)/* スプライト リストの終わりまで探す */
	{
		n = s->next;
		if (s == src)/* 見つかった？ */
		{
			if (NULL == p)
			{
				sp rite_list444_head = n;	/* スプライトリストの先頭を書き換える。 */
			}
			else
			{
				p->next = n;
			}
			free (s);
			#if 0
			s = NULL;/* 念の為要る？ */
			#endif
			return;/* 正常 */
		}
		p = s;
		s = n;
	}
	CHECKPOINT;/* 異常 */
	error(ERR_WARN, (char*)"sprite not found in list");
}
#endif

/*---------------------------------------------------------
	スプライト リストに登録されたスプライトを全部消す。
--------------------------------------------------------- */

global void sprite_remove_all_444_void(void)/*int ty pe*/
{
	#if 0
	SPRITE *s = sp rite_list444_head;/* スプライト リストの先頭 から探す */
	SPRITE *n = NULL;
	while (NULL != s)
	{
		n = s->next;
		if (s->type & (SP_GROUP_ALL_GAME_OBJS)/*type*/) /* typeが一致する場合のみ */
		{
			sprite_remove_one_444(s);
		}
		s = n;
	}
	#else
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* 全部調べる。 */
	{
		sprite_444pool[ii].type 	= SP_DELETE;
	}
	#endif
}


/*---------------------------------------------------------
	スプライトを一フレーム分動作させる。
--------------------------------------------------------- */

global void sprite_move_main_Gu_444(void)/*int ty pe*/
{
//	SPRITE *s;
//	SPRITE *n;
//	s = sp rite_list444_head;/* スプライト リストの先頭 から探す */
//	while (NULL != s)
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &sprite_444pool[ii];
	//
		#if 1/* 要らない気がする(てすとちう) */
		if (s->type != SP_DELETE ) /* 削除済みは飛ばす */
		#endif
		{
//			n = s->next;
			#if 0
			if (s->type != SP_DELETE ) /* 削除済みは飛ばす */	/* SP_DELETEが 0 になったので要らなくなった． */
			#endif
			{
				if (s->type & (SP_GROUP_ALL_SDL_CORE_TYPE)/*type*/) /* typeが一致する場合のみ */
				{
					if (NULL != s->callback_mover)
					{
						(s->callback_mover)(s);
					}
				}
			}
//			s = n;
		}
	}
	/* 道中の場合勝手に食み出しチェックを行い弾を消す(暫定的) */
	if (0==(pd_state_flag & STATE_FLAG_05_IS_BOSS))
	{
		bullet_angle_all_hamidasi_check();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
	}
}









#if 1
/*---------------------------------------------------------
	画面外ならおしまい(個別、主に汎用objで使用)
---------------------------------------------------------*/
global void gamen_gai_nara_zako_osimai(SPRITE *src)
{
	if (
		(src->x256 < t256(0)) ||
		(src->x256 > t256(GAME_WIDTH)) ||
		(src->y256 < t256(0)) ||
		(src->y256 > t256(GAME_HEIGHT)) )		/* 画面外に出たら */
	{
		src->type = SP_DELETE;		/* おしまい */
	}
}
//	if ((src->x256 < t256(0))||(src->x256 > t256(GAME_WIDTH))||
//		(src->y256 < t256(0))||(src->y256 > t256(GAME_HEIGHT)))
//	{
//		src->type = SP_DELETE;/*画面外にでたらおしまい*/
//	}
#endif



#if 1
/*---------------------------------------------------------
	画面外ならおしまい(弾幕)
---------------------------------------------------------*/

/* 検討中 */

/*extern*/global POINT256 bullet_clip_min;		/*  範囲(最小値) */
/*extern*/global POINT256 bullet_clip_max;		/*  範囲(最大値) */

static void s_check_hamidasi_bullet_angle_one(SPRITE *src)
{
	#if 1/* 検討中 */
//	if ((src->x256 < t256(-100))||(src->x256 > t256(100)+t256(GAME_WIDTH))||
//		(src->y256 < t256(-256))||(src->y256 > t256(100)+t256(GAME_HEIGHT) ) )
	if (
		(src->x256 < bullet_clip_min.x256)||
		(src->x256 > bullet_clip_max.x256)||
		(src->y256 < bullet_clip_min.y256)||
		(src->y256 > bullet_clip_max.y256) )
	{
		src->type = SP_DELETE;
	}
	#endif
}

/*---------------------------------------------------------
	弾の喰み出しチェックを行う(毎フレーム行う必要はない)
---------------------------------------------------------*/

global void bullet_angle_all_hamidasi_check(void)
{
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &sprite_444pool[ii];
		s_check_hamidasi_bullet_angle_one(s);
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
		h->x256			= (t256(GAME_WIDTH/2));
		h->y256			= (t256(-256));

}

/*---------------------------------------------------------
	スプライトの初期化をする。
	-------------------------------------------------------
	リスト方式でないスプライトも初期化する為、
	外部からも呼ばれるので注意。
--------------------------------------------------------- */

global void sprite_initialize_gu(SPRITE *obj)
{
	#if (0==USE_MEM_CLEAR)/* [メモリゼロクリアー機能]を使わない場合は、初期化が必要。 */
	#else
	memset(obj, 0, sizeof(SPRITE));
	#endif
//
	obj->flags		= 0;				/*set_flags*/
	obj->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	obj->alpha		= 0xff;*/

	obj->m_zoom_x256			= 256;	/* 表示拡大率 256 == [ x 1.00 ] */
	#if (0==USE_MEM_CLEAR)/* [メモリゼロクリアー機能]を使わない場合は、初期化が必要。 */
	obj->m_angleCCW1024 		= 0;	/* 描画用角度(下が0度で左回り(反時計回り)) */	/* 0 == 傾かない。下が0度 */
//
	#if 000/* 自動的に消える機能 */
	obj->timeover_ticks 		= 0;
	#endif
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
	gu移行中、弾専用
	スプライトをリストに追加する。

	登録できない場合、NULLを返す。

--------------------------------------------------------- */

global SPRITE *sprite_add_only_bullet_error(void)/*int image_resource_num*/
{
	#if 1
	/* private (この関数で)プライベートな変数 */
	static int register_num;	/* 登録できる可能性が高そうな位置を保持 */
	#endif
//	if (登録できない場合)	{	return(NULL);	}
//
//	int priority;		priority		= image_resource_ptr->priority;
//
	SPRITE *obj;/* 新規作成するスプライト */
	int search_count;
	search_count = 0;
my_retry:
		search_count++;
		if ( (SPRITE_444POOL_MAX-1) < search_count)
		{	return(NULL);	}	/* (登録できない場合) */

		register_num++; 	/* 登録できる可能性が高そうな位置 */
		register_num &= (SPRITE_444POOL_MAX-1);
		obj = &sprite_444pool[register_num];
	//
		if (obj->type != SP_DELETE )/* 使用中 */
		{
			goto my_retry;	/* 探す。 */
		}
//




//	SPRITE *s;
//	SPRITE *tail_obj/* =NULL */;		/* 追加するリストの候補位置。切断する場合は切断位置。 */
//	obj 		= my_calloc(sizeof(SPRITE));
//	memset(obj, 0, sizeof(SPRITE));
//	s			= sp rite_list444_head; /* スプライトリストの先頭。呼び出し */
//	tail_obj	= NULL;
	/*---------------------------------------------------------
		スプライトの表示プライオリティーを満たす位置を調べて、そこに挿入する。
	--------------------------------------------------------- */
//	while (
//		(NULL != s) /*&&*/				/* リストの頭までしらべたなら終わり */
//		(s->priority < priority)		/* 表示プライオリティーを満たす位置まで来たら終わり */
//		)
//	{
//		tail_obj	= s;
//		s			= s->next;
//	}
//	obj->next		= s;				/* 新規作成したスプライトの最後に連結 */
//	if (NULL == tail_obj)				/* 先頭なら(リストの切断がなければ) */
//	{
//		/* 先頭の場合は->nextが無いので */
//		sp rite_list444_head = obj; 	/* スプライトリストの先頭を書き換える。 */
//	}
//	else	//if (NULL != tail_obj)
//	{
//		tail_obj->next		= obj;		/* リストに挿入 */
//	}
	sprite_initialize_gu(obj);			/* 新規作成したスプライトを初期化 */
	#if 1
	/* 標準初期化 */
	obj->flags			|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);	/* あたり判定あり */
	#endif
	return (obj);
}






/*---------------------------------------------------------
	スプライト マネージャ(old)
--------------------------------------------------------- */



/*---------------------------------------------------------
	(敵専用)スプライトのあたり判定
--------------------------------------------------------- */

/*static*/global  SPRITE *sprite_list000_head = NULL;	/* スプライトのリスト構造 */

//global SPRITE sprite_pool[SPRITE_POOL_MAX];	/* スプライトのリスト構造 */

/*static*/global  SPRITE *sprite_collision_check_SDL_teki(SPRITE *tocheck)/*, (SP_GROUP_TEKI)int ty pe*/
{
	SPRITE *s = sprite_list000_head;/* スプライト リストの先頭 から探す */
	while (NULL != s)
	{
		/* 1. 自分自身チェック */
		/* 判定対象(自分自身)は飛ばす(全く同じもの同士のあたり判定を取っても意味がないので取らない) / zu testendes Sprite ueberspringen */
		if (s != tocheck)	/* Is sprites my self?(not need self collisions.) */
		{
			/* 2. 判定種類チェック */
			if (
				//		(s->type != SP_DELETE ) && /* 削除済みは飛ばす */	/* SP_DELETEが 0 になったので要らなくなった． */
						(s->type & (SP_GROUP_TEKI)/*type*/) 		 /* typeが一致する場合のみ */	/* Passender Sprite-Type? */
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
		s = s->next;/* 次 */
	}
	return (NULL);/* あたってない */	/* no hit collisioning. */
}


/*---------------------------------------------------------
	スプライト リストに登録されたスプライトを消す。
--------------------------------------------------------- */
#include <malloc.h>/* free(); */

//extern SPRITE *sprite_list000_head;
static void sprite_remove_one_SDL(SPRITE *src)
{
	SPRITE *s = sprite_list000_head;/* スプライト リストの先頭 から探す */
	SPRITE *p = NULL;
	SPRITE *n = NULL;
	while (NULL != s)/* スプライト リストの終わりまで探す */
	{
		n = s->next;
		if (s == src)/* 見つかった？ */
		{
			if (NULL == p)
			{
				sprite_list000_head = n;	/* スプライトリストの先頭を書き換える。 */
			}
			else
			{
				p->next = n;
			}
			free (s);
			#if 0
			s = NULL;/* 念の為要る？ */
			#endif
			return;/* 正常 */
		}
		p = s;
		s = n;
	}
	CHECKPOINT;/* 異常 */
	error(ERR_WARN, (char*)"sprite not found in list");
}

/*---------------------------------------------------------
	スプライト リストに登録されたスプライトを全部消す。
--------------------------------------------------------- */

global void sprite_remove_all_SDL_void(void)/*int ty pe*/
{
	SPRITE *s = sprite_list000_head;/* スプライト リストの先頭 から探す */
	SPRITE *n = NULL;
	while (NULL != s)
	{
		n = s->next;
		if (s->type & (SP_GROUP_ALL_GAME_OBJS)/*type*/) /* typeが一致する場合のみ */
		{
			sprite_remove_one_SDL(s);
		}
		s = n;
	}
}

/*---------------------------------------------------------
	スプライトを一フレーム分動作させる。
--------------------------------------------------------- */

global void sprite_move_main_SDL_222(void)/*int ty pe*/
{
	SPRITE *s;
	SPRITE *n;
	s = sprite_list000_head;/* スプライト リストの先頭 から探す */
	while (NULL != s)
	{
		n = s->next;
		#if 0
		if (s->type != SP_DELETE ) /* 削除済みは飛ばす */	/* SP_DELETEが 0 になったので要らなくなった． */
		#endif
		{
			if (s->type & (SP_GROUP_ALL_SDL_CORE_TYPE)/*type*/) /* typeが一致する場合のみ */
			{
				if (NULL != s->callback_mover)
				{
					(s->callback_mover)(s);
				}
			}
		}
		s = n;
	}
	#if 1
	/* 使用済みスプライトを探して、使用済みスプライトがあれば消す */
	#if 00/* とりあえず現在未使用だが、メニューでobj使う場合は注意 */
//	if ( S T_WORK_MENU != (ps p_loop&0xff00) )/* メニュー以外の場合、(ポーズメニューで時間が経つ場合に消えると困るので) */
	if ( S T_INIT_MENU_ST_MENU_SUB_MAIN_MENU > (ps p_loop&0xff00) )/* メニュー以外の場合、(ポーズメニューで時間が経つ場合に消えると困るので) */
/* とりあえず */
	#endif
	{
		s = sprite_list000_head;/* スプライト リストの先頭 から探す */
		while (NULL != s)
		{
			n = s->next;
			#if 000/* 自動的に消える機能 */
			/* 一定時間での自動消去、許可フラグONの場合、 */
			if ( (SP_FLAG_TIME_OVER == (s->flags&SP_FLAG_TIME_OVER)) )
			{
				s->timeover_ticks++;
				if ( 1000 < (s->timeover_ticks) )/* 16秒 */ 	/* (1024-1) < */ /* (0x0400 & (s->ticks)) */
				{
				//	s->timeover_ticks = 0;
					s->type = SP_DELETE;	/* 自動消去にする。 */
				}
			}
			#endif
			if (SP_DELETE == s->type)	/* 消去？ */
			{
				sprite_remove_one_SDL(s);	/* 削除済みを消す */
			}
			s = n;
		}
	}
	#endif
}

/*---------------------------------------------------------
	gu移行中、汎用スプライトをリストに追加する。

	登録できない場合、NULLを返す。

--------------------------------------------------------- */
extern void sprite_initialize_gu(SPRITE *obj);
global SPRITE *sprite_add_gu_error(void)
{
//	if (登録できない場合)	{	return(NULL);	}
//
//	int priority;		priority		= image_resource_ptr->priority;
//
	SPRITE *obj;/* 新規作成するスプライト */
	SPRITE *s;
	SPRITE *tail_obj/* =NULL */;		/* 追加するリストの候補位置。切断する場合は切断位置。 */
	obj 		= my_calloc(sizeof(SPRITE));
//	memset(obj, 0, sizeof(SPRITE));
	s			= sprite_list000_head;	/* スプライトリストの先頭。呼び出し */
	tail_obj	= NULL;
	/*---------------------------------------------------------
		スプライトの表示プライオリティーを満たす位置を調べて、そこに挿入する。
	--------------------------------------------------------- */
	while (
		(NULL != s) /*&&*/				/* リストの頭までしらべたなら終わり */
//		(s->priority < priority)		/* 表示プライオリティーを満たす位置まで来たら終わり */
		)
	{
		tail_obj	= s;
		s			= s->next;
	}
	obj->next		= s;				/* 新規作成したスプライトの最後に連結 */
	if (NULL == tail_obj)				/* 先頭なら(リストの切断がなければ) */
	{
		/* 先頭の場合は->nextが無いので */
		sprite_list000_head = obj;		/* スプライトリストの先頭を書き換える。 */
	}
	else	//if (NULL != tail_obj)
	{
		tail_obj->next		= obj;		/* リストに挿入 */
	}
	sprite_initialize_gu(obj);			/* 新規作成したスプライトを初期化 */
	return (obj);
}

