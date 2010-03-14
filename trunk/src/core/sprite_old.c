
/*---------------------------------------------------------
	スプライト マネージャ
--------------------------------------------------------- */

#include "game_main.h"

/*---------------------------------------------------------
	リソース resource
--------------------------------------------------------- */
static u8 my_bullet_resource_atari_hankei[] =
{
#if 1
	/* [有る名前で] */
	//#define DU MMY_IMG "te ki/bu llet_maru16.png"
//	#define DU MMY_IMG	"ji ki/co re_re.png"

	#if 0
	/* 77 */	//	   "teki/kugel.png",						   /*0,*/	 2 ,
	/* 78 */	//	   "teki/kugel2.png",						   /*0,*/	 2 ,
	/* 79 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/	 2 , /* 針弾 (敵弾) */
	/* 80 */		   /* "teki/bullet_maru16.png"*/			   /*0,*/	 2 , /* 白丸(小) (敵弾) */	  /* 赤丸(中) (敵弾) */
//
	/* 81 */		   /* "teki/bullet_ming32.png"*/			   /*0,*/	 2 , /* ゆかりん弾 (敵弾) */
	/* 82 */		   /* "teki/jippou32.png"*/ 				   /*0,*/	 2 , /* 白模擬弾 (敵弾) */
//
					  /* 追加予定 */											 /* クナイ弾 (敵弾) */
//
	/* 83 */		   /* "teki/oodama08.png"*/ 				   /*1,*/	 4 ,/* 大弾(黒青赤...)	  黒玉(輪) PRIORITY_03_ENEMY は、あたり判定部分 */
	/* 83 */	//	   "teki/bigkugel2.png",					   /*0,*/	 2 ,/* 黒玉(輪)   あたり判定部分 */
	/* 84 */	//	   "teki/bigkugel1.png",					   /*1,*/	 2 ,/* 大弾(青) 表示部分 */
	/* 85 */	//	   "teki/new_bigkugel.png", 				   /*1,*/	 2 ,/* 大弾(赤) 表示部分 */
//
	/* 86 */		   /* "teki/knife.png"*/					   /*1,*/	 2 ,/* 垂直降下ナイフ(赤) */
	/* 87 */		   /* "teki/knife_core16.png"*/ 			   /*1,*/	 2 ,/* 全方向ナイフ(青) */
	#else
	/*	0 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (2), /* 2 */
	/*	1 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (4), /* 4 */
	#endif
	/* 103 */		   /*"teki/homing16.png"*/					   /*0,*/  (2), /* ザコ 誘導弾 */
//
	/*	3 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (16), /* 4 */ 	  //  JIKI_ATARI_ITEM_16,
	/*	4 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (80), /* 4 */ 	  //  JIKI_ATARI_ITEM_80,/* 扇本体 */
//
	/*	5 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (2),   /* re a */  //  BASE_BOMBER_ATARI_0a,/*4*/
	/*	5 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (2),   /* re b */  //  BASE_BOMBER_ATARI_0a,/*4*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (16),   /* ma a */  //  BASE_BOMBER_ATARI_0b,/*16*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (16),   /* ma b */  //  BASE_BOMBER_ATARI_0b,/*16*/
	/*	7 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (16),   /* oz / */  //  BASE_BOMBER_ATARI_0c,/*16*/
	/*	9 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (4),   /* yu / */  //  BASE_BOMBER_ATARI_0e,/*4*/
	/*	8 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (32),   /* ci a */  //  BASE_BOMBER_ATARI_0d,/*32*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (32),   /* ci q */  //  BASE_BOMBER_ATARI_0b,/*16*/
//
	/*	5 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (4),  /* re a */  //  BASE_SHOT_ATARI_0a,/*4*/
	/*	5 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (4),  /* re b */  //  BASE_SHOT_ATARI_0a,/*4*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (4),  /* ma a */  //  BASE_SHOT_ATARI_0b,/*4*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (4),  /* ma b */  //  BASE_SHOT_ATARI_0b,/*4*/
	/*	7 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (4),  /* oz / */  //  BASE_SHOT_ATARI_0c,/*4*/
	/*	9 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (8),  /* yu / */  //  BASE_SHOT_ATARI_0e,/*8*/
	/*	8 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (6),  /* ci a */  //  BASE_SHOT_ATARI_0d,/*6*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (6),  /* ci q */  //  BASE_SHOT_ATARI_0b,/*4*/
//
	/*	5 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (6),   /* re a */  //  BASE_NEEDLE_ATARI_0a,/*6*/
	/*	5 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (6),   /* re b */  //  BASE_NEEDLE_ATARI_0a,/*6*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (8),   /* ma a */  //  BASE_NEEDLE_ATARI_0b,/*8*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (8),   /* ma b */  //  BASE_NEEDLE_ATARI_0b,/*8*/
	/*	7 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (12),   /* oz / */  //  BASE_NEEDLE_ATARI_0c,/*12*/
	/*	9 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (8),   /* yu / */  //  BASE_NEEDLE_ATARI_0e,/*8*/
	/*	8 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (16),   /* ci a */  //  BASE_NEEDLE_ATARI_0d,/*16*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/ (16),   /* ci q */  //  BASE_NEEDLE_ATARI_0b,/*8*/
//
	/*	5 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (1),  /* re a */  //  BASE_CORE_ATARI_0a,/*6*/
	/*	5 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (1),  /* re b */  //  BASE_CORE_ATARI_0a,/*6*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (2),  /* ma a */  //  BASE_CORE_ATARI_0b,/*8*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (2),  /* ma b */  //  BASE_CORE_ATARI_0b,/*8*/
	/*	7 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (2),  /* oz / */  //  BASE_CORE_ATARI_0c,/*12*/
	/*	9 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (2),  /* yu / */  //  BASE_CORE_ATARI_0e,/*8*/
	/*	8 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (1),  /* ci a */  //  BASE_CORE_ATARI_0d,/*16*/
	/*	6 */		   /* "teki/bullet_beam16.png"*/			   /*0,*/  (1),  /* ci q */  //  BASE_CORE_ATARI_0b,/*8*/
#endif
};

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
	if (dx < 0/* .0 */) {	dx = (-dx); }	/* dx = abs(dx); */
	if (dx > dr)	{	return (0/* FALSE */);	}	/* not hit, because not near the dx. Ｘの差分が大きいので、当たってない */

	/* check collision, delta y position. 矩形判定（Ｙ軸） */
	if (dy < 0/* .0 */) {	dy = (-dy); }	/* dy = abs(dy); */
	if (dy > dr)	{	return (0/* FALSE */);	}	/*	not hit, because not near the dy. Ｙの差分が大きいので、当たってない */

	/* check collision, circle delta round distance.  円の半径で判定 */
	dr *= dr;
	dx *= dx;
	dy *= dy;
	dx += dy;
	if (dr < dx)/* if ( (dr^2) < ( (dx^2) + (dy^2) )) */
	{
		return (0/* FALSE */);	/* not hit. 当たってない */
	}
	return (1/* TRUE */); /* hit! 当たった */
}
#endif


/*---------------------------------------------------------
	(敵専用)スプライトのあたり判定
--------------------------------------------------------- */

SPRITE *sprite_list000_head = NULL; 	/* スプライトのリスト構造 */

//SPRITE sprite_pool[SPRITE_POOL_MAX];	/* スプライトのリスト構造 */

SPRITE *sprite_collision_check_SDL_teki(SPRITE *tocheck)/*, (SP_GROUP_TEKI)int type*/
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
						( (SP_FLAG_COLISION_CHECK|SP_FLAG_VISIBLE) == (s->flags&(SP_FLAG_COLISION_CHECK|SP_FLAG_VISIBLE)))
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
extern SPRITE *sprite_list000_head;
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
//			if (0==(s->flags&SP_FLAG_NOT_CACHE))
//			{
//				unloadbmp_by_surface(s->sprite_bmp);
//			}
			#if 0
		//	if ((s->flags&SP_FLAG_FREE_SURFACE))
			if ((s->flags&(SP_FLAG_FREE_SURFACE|SP_FLAG_NOT_CACHE)))
			{
				/* KETMでここは無効になってるが、
				ここが無効になってる限り(点数表示用フォントで)
				延々メインメモリを消費しちゃうので、
				開放してみるテスト */
				/* ここでハングアップ */
				SDL_FreeSurface(s->sprite_bmp);
			}
			#endif
		//	#if (1==USE_ZUKEI_ATARI_HANTEI)
		//	sprite_remove_colision_map(s->colision_bmp);
		//	#endif /* (1==USE_ZUKEI_ATARI_HANTEI) */
			if (NULL != s->data)
			{
				free (s->data);
			}
			free (s);
			return;/* 正常 */
		}
		p = s;
		s = n;
	}
	CHECKPOINT;/* 異常 */
	error(ERR_WARN, "sprite not found in list");
}

/*---------------------------------------------------------
	スプライト リストに登録されたスプライトを全部消す。
--------------------------------------------------------- */

//void sprite_remove_all222(int type){}
void sprite_remove_all_SDL(int type)
{
	SPRITE *s = sprite_list000_head;/* スプライト リストの先頭 から探す */
	SPRITE *n = NULL;
	while (NULL != s)
	{
		n = s->next;
		if (s->type & type) /* typeが一致する場合のみ */
		{
			sprite_remove_one_SDL(s);
		}
		s = n;
	}
}

//void sprite_remove_all_type(int type)
//{
//	SPRITE *s = sprite;
//	SPRITE *n = NULL;
//	while (NULL != s)
//	{
//		n = s->next;
//		if (s->type == type)
//		{
//			sprite_remove(s);
//		}
//		s = n;
//	}
//}

/*---------------------------------------------------------
	スプライトを一フレーム分動作させる。
--------------------------------------------------------- */

//void sprite_work222(int type){}
void sprite_work_SDL(int type)
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
			if (s->type & type) /* typeが一致する場合のみ */
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
	if ( ST_WORK_MENU != (psp_loop&0xff00) )/* メニュー以外の場合、(ポーズメニューで時間が経つ場合に消えると困るので) */
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

--------------------------------------------------------- */

SPRITE *sprite_add_gu(int image_resource_num)
{
	int m_Hit256R;		m_Hit256R		= ((my_bullet_resource_atari_hankei[image_resource_num])<<8);
//
	//int priority; 		priority		= image_resource_ptr->priority;
//
	SPRITE *obj;/* 新規作成するスプライト */
	SPRITE *s;
	SPRITE *tail_obj/* =NULL */; /* 追加するリストの候補位置。切断する場合は切断位置。 */
	obj 		= mmalloc(sizeof(SPRITE));
//	memset(obj, 0, sizeof(SPRITE));
	s			= sprite_list000_head;	/* スプライトリストの先頭。呼び出し */
	tail_obj	= NULL;
	/*---------------------------------------------------------
		スプライトの表示プライオリティーを満たす位置を調べて、そこに挿入する。
	--------------------------------------------------------- */
	while (
		(NULL != s) /*&&*/				/* リストの頭までしらべたなら終わり */
	//	(s->priority < priority)	/* 表示プライオリティーを満たす位置まで来たら終わり */
		)
	{
		tail_obj	= s;
		s			= s->next;
	}
	obj->next		= s;	/* 新規作成したスプライトの最後に連結 */
	if (NULL == tail_obj)/* 先頭なら(リストの切断がなければ) */
	{
		/* 先頭の場合は->nextが無いので */
		sprite_list000_head = obj;		/* スプライトリストの先頭を書き換える。 */
	}
	else	//if (NULL != tail_obj)
	{
		tail_obj->next		= obj;	/* リストに挿入 */
	}
	/*---------------------------------------------------------
		スプライトの設定をする。
	--------------------------------------------------------- */
	#if 0
	{	/* この機能(個別のスプライトに番号を割り当て番号で判別する)は大した意味がないので無くした。
			(個別のスプライトを判別したい場合は、ポインタで判別すればこの機能は不要なので) */
		static int current_id	= 0;
		obj->id 				= current_id;
		current_id++;
	}
	#endif

	/*set_atari_hankei*/ //image_resource_ptr->atari_hankei;
	obj->m_Hit256R	= m_Hit256R;

//	obj->frames 	= total_frames;
//	obj->priority	= priority;
	obj->flags		= 0;//set_flags; /* 0;	if (nocache) s->flags |= SP_FLAG_NOT_CACHE; */
	obj->color32	= 0xffffffff;		/*	obj->alpha		= 0xff;*/

	obj->m_zoom_x256			= 256;	/* 表示拡大率 256 == [ x 1.00 ] */
	#if (0==USE_MEM_CLEAR)/* [メモリゼロクリアー機能]を使わない場合は、初期化が必要。 */
//	obj->yx_an im_frame = 0x00;
//	obj->anim_count 	= 0;
	obj->m_angleCCW512	= 0;			/* 描画用角度(下が0度で左回り(反時計回り)) */	/* 0 == 傾かない。下が0度 */
//
	#if 000/* 自動的に消える機能 */
	obj->timeover_ticks 		= 0;
	#endif
//
	obj->vx256					= t256(0);
	obj->vy256					= t256(0);
//
	obj->data					= NULL;
	obj->callback_mover 		= NULL;
	obj->callback_loser 		= NULL;
	obj->callback_hit_enemy 	= NULL;
	#endif	/* (0==USE_MEM_CLEAR) */
	return (obj);
}

