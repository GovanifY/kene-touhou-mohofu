
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	スプライト マネージャ
	-------------------------------------------------------
	GAME_X_OFFSET対応
--------------------------------------------------------- */

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
int collision_laser( /* obj_t */OBJ *obj1, LASER *laser1)
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
	rotation_angle1024 = -(deg65536to1024(laser1->angle65536));
	/* 固定値 */
//	sin_angle = (int)(int256_si n1024(/*rot_sin*/((/*OFFS_SIN1024+*/		  rotation_angle1024)&(1024-1)))/*<<8*/);
//	cos_angle = (int)(int256_si n1024(/*rot_cos*/(( OFFS_COS1024+			  rotation_angle1024)&(1024-1)))/*<<8*/);
	int256_sincos1024( (rotation_angle1024), &sin_angle, &cos_angle);
	#if 1
	/* [下CCW]下が 0度で反時計回りの角度系 */
	hantei_x = (((sabun_x256 * sin_angle) + (sabun_y256 * cos_angle))>>8);
	hantei_y = (((sabun_x256 * cos_angle) - (sabun_y256 * sin_angle))>>8);
	#endif
//	分割処理２．領域大小判定(直線外はあたらない。)
//	if ((0) > hantei_y)/* レーザー発生源から、反対方向はあたらない。 */
	if ((laser1->ar256) > hantei_y)/* レーザー発生源から、反対方向はあたらない。 */
	{
		return (0);/* FALSE */	/* not hit. 当たってない */
	}
	if ((0+(laser1->aw256)) > hantei_x)/* 一定の太さ(laser1->aw256)の直線。直線外はあたらない。 */
	{
		return (0);/* FALSE */	/* not hit. 当たってない */
	}
	if ((0-(laser1->aw256)) < hantei_x)/* 一定の太さ(laser1->aw256)の直線。直線外はあたらない。 */
	{
		return (0);/* FALSE */	/* not hit. 当たってない */
	}

//	分割処理３．距離で判定(円内はあたらない。)
//	if (laser1->ar256 > ra256)
//	{
//		return (0);/* FALSE */	/* not hit. 当たってない */
//	}
	return (1);/* TRUE */	/* hit! 当たった */
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
static /* BOOL */int collision_hit( /* obj_t */OBJ *obj1, /* obj_t */OBJ *obj2 )
{
	#if (0)/* この機能は要らない(?)かも。 */
	/*--- 判定有効なオブジェクトでなかったら終了 */
//	if (obj2->m_Hit256R == 0/* .0 */ /* FALSE */)
	if ((16) > obj2->m_Hit256R) 	/*t256(1.00)t256(0.10)*/
	{
		return (0);/* FALSE */	/* not hit. 当たってない */
	}
//	if (obj1->m_Hit256R == 0/* .0 */ /* FALSE */)
	if ((16) > obj1->m_Hit256R) 	/*t256(1.00)t256(0.10)*/
	{
		return (0);/* FALSE */	/* not hit. 当たってない */
	}
	#endif
//
	int dr; 	/* 半径(==radius) */
	int dx; 	/* x差分(微小x距離, delta x) */
	int dy; 	/* y差分(微小y距離, delta x) */
	dr = (obj2->m_Hit256R + obj1->m_Hit256R);
	/* 中心同士で差。つまり軸ごとの距離を求める。 */
	dx = ((obj2->cx256) - (obj1->cx256));/* 中心同士で差を求める。 */
	dy = ((obj2->cy256) - (obj1->cy256));/* 中心同士で差を求める。 */

	/* check collision, delta x position. 矩形判定（Ｘ軸） */
	if (dx < 0) 	{	dx = (-dx); 			}	/* dx = abs(dx); */
	if (dx > dr)	{	return (0);/* FALSE */	}	/* not hit, because not near the dx. Ｘの差分が大きいので、当たってない */

	/* check collision, delta y position. 矩形判定（Ｙ軸） */
	if (dy < 0) 	{	dy = (-dy); 			}	/* dy = abs(dy); */
	if (dy > dr)	{	return (0);/* FALSE */	}	/*	not hit, because not near the dy. Ｙの差分が大きいので、当たってない */

	/* check collision, circle delta round distance.  円の半径で判定 */
	dr *= dr;
	dx *= dx;
	dy *= dy;
	dx += dy;
	/* 当たり判定外なら */
	if (dr < dx)/* if ((dr^2) < ( (dx^2) + (dy^2) )) */
	{
		return (0);/* FALSE */	/* not hit. 当たってない */
	}
	return (1);/* TRUE */	/* hit! 当たった */
}
#endif


/*---------------------------------------------------------
	スプライトのあたり判定
	-------------------------------------------------------
	入力値: type
		OBJ_Z04_TAMA		(弾)
		OBJ_Z03_ITEM		(アイテム)
--------------------------------------------------------- */
/*---------------------------------------------------------
	(敵専用)スプライトのあたり判定
--------------------------------------------------------- */

global OBJ obj99[OBJ_LAST_99_0x0948_MAX]; 	/* 全スプライト */

static OBJ *sprite_collision_check(OBJ *tocheck, OBJ *s, int set_obj_type, unsigned int length)
{
	unsigned int ii;
	for (ii=0; ii<length; ii++)/* 全部調べる。 */
	{
		if (JYUMYOU_NASI < s->jyumyou)/* 消去済みは飛ばす。 */
		{
			/* 1. 自分自身チェック */
			/* 判定対象(自分自身)は飛ばす(全く同じもの同士のあたり判定を取っても意味がないので取らない) */
			if (s != tocheck)	/* Is sprites my self?(not need self collisions.) */
			{
				/* 2. 判定種類チェック */
				if (
							(s->obj_type_set & set_obj_type)			/* obj_type_set が一致する場合のみ */
					)	/* do collision only mach type. */
				{
					/* 3. 判定フラグチェック */
					if (
							( (ATARI_HANTEI_OFF) != (s->atari_hantei))/*(あたり判定があるもののみチェック)*/
						)	/* do collision only, use collision check type. */
					{
						/* 4. 矩形/円あたり判定チェック   大まかに矩形で判別した後、近そうなら円の衝突判定 */
						if (collision_hit(s, tocheck))	/* 矩形/円あたり判定 */ 	/* hit collision rectangle to circle check. */
						{
							return (s);/*(あたった)*/	/* hit collisioning! */
						}
					}
				}
			}
		}
		s++;
	}
	return (NULL);/*(あたってない)*/	/* no hit collisioning. */
}
/*static*/global OBJ *obj_collision_check_00_tama(OBJ *tocheck, int type)
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_00_0x0000_TAMA+0];
	return sprite_collision_check(tocheck, s, type, OBJ_POOL_00_TAMA_1024_MAX);
}
//	/*static*/global OBJ *obj_collision_check_01_teki(OBJ *tocheck, int type)
/*static*/global OBJ *obj_collision_check_01_teki(OBJ *tocheck)/*, (OBJ_Z02_TEKI)int ty pe*/
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_01_0x0800_TEKI+0];
	return sprite_collision_check(tocheck, s, /*type*/(OBJ_Z02_TEKI), OBJ_POOL_01_TEKI_0256_MAX);
}


/*---------------------------------------------------------
	スプライト リストに登録されたスプライトを全部消す。
--------------------------------------------------------- */
static void obj_remove_all(OBJ *s, unsigned int length)
{
	unsigned int ii;
	for (ii=0; ii<length; ii++)/* 全部調べる。 */
	{
		s->jyumyou	= JYUMYOU_NASI;
		#if (1)/* デバッグてすと */
		s->cx256		= (-1);
		#endif
		s++;
	}
}

#if 0
static void s_obj_remove_all_00_tama(void)	/* 弾専用 */
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_00_0x0000_TAMA+0];
	obj_remove_all(s, OBJ_POOL_00_TAMA_1024_MAX);
}
static void s_obj_remove_all_01_teki(void)	/* gu汎用(旧SDL) */
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_01_0x0800_TEKI+0];
	obj_remove_all(s, OBJ_POOL_01_TEKI_0256_MAX);
}
global void obj_cleanup_all(void)
{
	s_obj_remove_all_01_teki();/* gu汎用 */
	s_obj_remove_all_00_tama();/* 弾専用 */
}
#endif

#if 1/*(r36)*/

/* [A00弾領域]と[A01敵領域]と[A02固定領域]と[A03パネル領域]のOBJを全消去。 */
global void obj_cleanup_all(void)
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_00_0x0000_TAMA+0];
	obj_remove_all(s, (OBJ_POOL_00_TAMA_1024_MAX+OBJ_POOL_01_TEKI_0256_MAX+OBJ_POOL_02_KOTEI_0016_MAX+OBJ_POOL_03_PANEL_0056_MAX));
}
#endif

#if 0
global void obj_cleanup_A02_A03_kotei_panel(void)
{
}
#endif
/*---------------------------------------------------------
	スプライトを一フレーム分動作させる。
--------------------------------------------------------- */
static void sprite_move_main(OBJ *s, unsigned int length)
{
	unsigned int iii;
//	for (iii=0; iii<length; iii++)/* 全部調べる。 */
	iii = length;/* レーザーの実装を簡単にする為に、逆順にする。 */
	{
		my_loop:
		if (JYUMYOU_NASI < s->jyumyou)/* 消去済みは飛ばす。 */
		{
			s->jyumyou--;/* 寿命経過 */
			{
				#if 1/*???*/
				if (s->obj_type_set & (SP_GROUP_ALL_SDL_CORE_TYPE)) /* obj_type_setが一致する場合のみ */
				#endif
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
	#if(0)/*(ダメ)*/
extern void hatudan_effect_move_all(void);
global void obj_area_move_A00_A01_A02(void)
{
	hatudan_effect_move_all();
	/*
	[非動作]	弾専用スプライト(1024)
	[動作]		敵専用スプライト(256)
	[動作]		自機等固定スプライト(16)
	[非動作]	パネル用スプライト(56)
	*/
	OBJ *s;
	s = &obj99[OBJ_HEAD_03_0x0910_PANEL];
	sprite_move_main(s, (OBJ_POOL_01_TEKI_0256_MAX+OBJ_POOL_02_KOTEI_0016_MAX));
}
/*(r35, 将来的に弾の移動は無し)*/
	#endif
	//
	#if(1)/*(めもr33)*/
global void obj_area_move_A00_A01_A02(void)
{
	/*
	[動作]		弾専用スプライト(1024)[アイテムや星点や発弾エフェクトの動作]
	[動作]		敵専用スプライト(256)
	[動作]		自機等固定スプライト(16)
	[非動作]	パネル用スプライト(56)
	*/
	OBJ *s;
	s = &obj99[OBJ_HEAD_03_0x0910_PANEL];
	sprite_move_main(s, (OBJ_POOL_00_TAMA_1024_MAX+OBJ_POOL_01_TEKI_0256_MAX+OBJ_POOL_02_KOTEI_0016_MAX));
}
/*(r35, 将来的に弾の移動は無しになる可能性が高い。その場合 effect は別システムになる)*/
	#endif
	//
	#if 0/*(めもr32)*/
static void obj_move_main_00_tama(void) 	/* 弾専用スプライト(1024) gu弾専用 */
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_00_0x0000_TAMA+OBJ_POOL_00_TAMA_1024_MAX+0];
	sprite_move_main(s, OBJ_POOL_00_TAMA_1024_MAX);
}

static void obj_move_main_01_teki(void) /* 敵専用スプライト(256) gu汎用(旧SDL) */
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_01_0x0800_TEKI+OBJ_POOL_01_TEKI_0256_MAX+0];
	sprite_move_main(s, OBJ_POOL_01_TEKI_0256_MAX);
}
static void obj_move_main_02_kotei(void)/* 自機等固定スプライト(16) */
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_02_0x0900_KOTEI+OBJ_POOL_02_KOTEI_0016_MAX+0];
	sprite_move_main(s, OBJ_POOL_02_KOTEI_0016_MAX);
}
global void obj_area_move_A00_A01_A02(void)
{
	obj_move_main_02_kotei();/* 自機等固定スプライト(16) (自機等固定オブジェクト) */
	obj_move_main_01_teki();/* 敵専用スプライト(256) (gu汎用) */
	obj_move_main_00_tama();/* 弾専用スプライト(1024) (弾専用) */
}
	#endif

#if 1
/*---------------------------------------------------------
	画面外ならおしまい(個別、主に汎用objで使用)
---------------------------------------------------------*/
global void gamen_gai_nara_zako_osimai(OBJ *src)
{
	if (
		(src->cx256 < t256(GAME_X_OFFSET)) ||
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
//global void gamen_gai_limit_30_nara_zako_osimai(OBJ *src)
//{
//	if ((JYUMYOU_ZAKO_CLIP) > src->jyumyou) 		/* 離脱 */
//	{
//		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
//	}
//}
#endif



/*---------------------------------------------------------
	使用中であるが退避したい場合。画面外にスプライトを移動させ、無効にする。
	-------------------------------------------------------
	スプライトを確保したまま退避させる。
	ボスobj, effect obj で使用中。
---------------------------------------------------------*/
global void sprite_kotei_obj_r36_taihi(OBJ *h)
{
	h->jyumyou						= (JYUMYOU_MUGEN);/* 時間で自動消去しない */	/*(固定確保で判別されるフラグ)*/
	h->m_Hit256R					= ZAKO_ATARI16_PNG;
	h->atari_hantei 				= (ATARI_HANTEI_OFF/*スコア兼用*/); 	/* あたり判定のOFF(無敵) */
	h->callback_mover				= NULL;
	h->callback_loser				= NULL;
	h->callback_hit_teki			= NULL; 	/* ダミーコールバック登録 */
	h->cx256		= (t256( GAME_X_OFFSET + ((GAME_320_WIDTH)/2) ));
	h->cy256		= (t256(-256));
}
//???	h->obj_type_set 				= BOSS_00_11;

/*---------------------------------------------------------
	スプライトの初期化をする。
	-------------------------------------------------------
	リスト方式でないスプライトも初期化する為、
	外部からも呼ばれるので注意。
--------------------------------------------------------- */
//tern void sprite_initialize_gu(OBJ *obj);
global void sprite_initialize_gu(OBJ *obj)
{
	#if (0==USE_MEM_CLEAR)/* [メモリゼロクリアー機能]を使わない場合は、初期化が必要。 */
	#else
	memset(obj, 0, sizeof(OBJ));
	#endif
//
	obj->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	obj->alpha		= 0xff;*/
	#if 1/* 自動的に消える機能 */
	obj->jyumyou	= JYUMYOU_1MIN; 		/* 1分したら勝手に自動消去。 */
//	obj->timeover_ticks 		= 0;
	#endif
	obj->m_zoom_x256			= t256(1.00);	/* 表示拡大率 256 == [ x 1.00 ] */
	obj->m_zoom_y256			= t256(1.00);	/* 表示拡大率 256 == [ x 1.00 ] */
	#if (0==USE_MEM_CLEAR)/* [メモリゼロクリアー機能]を使わない場合は、初期化が必要。 */
	obj->rotationCCW1024		= 0;	/* 描画用角度(下が0度で左回り(反時計回り)) */	/* 0 == 傾かない。下が0度 */
//
	obj->vx256					= t256(0);
	obj->vy256					= t256(0);
//
	obj->callback_mover 		= NULL;
	obj->callback_loser 		= NULL;
	obj->callback_hit_teki		= NULL;
	#endif	/* (0==USE_MEM_CLEAR) */
}


/*---------------------------------------------------------
	gu移行中、弾専用スプライトをリストに追加する。
	gu移行中、汎用スプライトをリストに追加する。
	登録できない場合、NULLを返す。
--------------------------------------------------------- */

global OBJ *obj_add_Ann_direct(unsigned int direct_register_number)/*OBJ_HEAD_02_0x0900_KOTEI+*/
{
//	int priority;		priority		= image_resource_ptr->priority;
	OBJ *obj;/* 新規作成するスプライト */
	obj = &obj99[direct_register_number];
	sprite_initialize_gu(obj);			/* 新規作成したスプライトを初期化 */
	return (obj);
}

global OBJ *obj_add_A00_tama_error(void)
{
	#if 1
	/* private (この関数で)プライベートな変数 */
	static u16 register_num;	/* 登録できる可能性が高そうな位置を保持 */
	#endif
	OBJ *obj;/* 新規作成するスプライト */
	{
		u16 search_count;
		search_count = 0;
my_retry:
		search_count++;
		if ((OBJ_POOL_00_TAMA_1024_MAX-1) <= search_count)
		{
			#if (0)/*(デバッグ)*/
			/*(アリスはともかく文で出る!!何でだろ？)*/
			psp_fatal_error( (char*)
			//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
				"regist tama: %d 登録可能\最大弾数を、" "\\n"
				"超えています。", search_count);
			#endif
			return (NULL);
		}	/* (登録できない場合) */
		register_num++; 	/* 登録できる可能性が高そうな位置 */
		register_num &= (OBJ_POOL_00_TAMA_1024_MAX-1);
		obj = &obj99[OBJ_HEAD_00_0x0000_TAMA+register_num];
		if (JYUMYOU_NASI < obj->jyumyou)/* 使用中ならもう一度探す。 */
		{
			goto my_retry;	/* 探す。 */
		}
	}
	sprite_initialize_gu(obj);			/* 新規作成したスプライトを初期化 */
	#if 1
	/* 標準初期化 */
	obj->atari_hantei			|= (1/*(スコア兼用)*/); 	/* あたり判定あり */
	#endif
	return (obj);
}

/*
(r36)boss objは敵汎用objから固定確保するように仕様変更した。
*/

/*bossを固定確保する分*/
#define BOSS_OBJ_KOTEI_KAKUHO_BUN (1)
global OBJ *obj_add_A01_teki_error(void)
{
	#if 1
	/* private (この関数で)プライベートな変数 */
	static u32 register_num;	/* 登録できる可能性が高そうな位置を保持 */
	#endif
	OBJ *obj;/* 新規作成するスプライト */
	{
		u32 search_count;
		search_count = 0;
my_retry:
		search_count++;
		if ((OBJ_POOL_01_TEKI_0256_MAX-1-(BOSS_OBJ_KOTEI_KAKUHO_BUN)) < search_count)
		{	return (NULL);	}	/* (登録できない場合) */
		register_num++; 	/* 登録できる可能性が高そうな位置 */
		register_num &= (OBJ_POOL_01_TEKI_0256_MAX-1);
		obj = &obj99[OBJ_HEAD_01_0x0800_TEKI+register_num];
		if (JYUMYOU_NASI < obj->jyumyou)/* 使用中ならもう一度探す。 */
		{
			goto my_retry;	/* 探す。 */
		}
	}
	sprite_initialize_gu(obj);			/* 新規作成したスプライトを初期化 */
	return (obj);
}


#define PPP (GAME_WIDTH)
global void sprite_panel_init(void)
{
	unsigned int ii;
	for (ii=0; ii<OBJ_POOL_03_PANEL_0056_MAX; ii++)/* 全部 */
	{
		OBJ *obj;/* 初期化するスプライト */
		obj = &obj99[OBJ_HEAD_03_0x0910_PANEL+ii];
		sprite_initialize_gu(obj);			/* スプライトを初期化 */
	}
	unsigned int kk;
	kk = ((PPP+5*8+4)<<8);
//	unsigned int jj;
//	for (jj=0; jj<(2); jj++)/* */
	for (ii=0; ii<(10); ii++)/* */
	{
		OBJ *obj;/* 初期化するスプライト */
		/* スコア */
		obj = &obj99[OBJ_HEAD_03_0x0910_PANEL+ii+PANEL_OBJ_00_S00];
		obj->cx256 = (kk);
		obj->cy256 = ((6*8+7)<<8);
		/* ハイスコア */
		obj = &obj99[OBJ_HEAD_03_0x0910_PANEL+ii+PANEL_OBJ_10_H00];
		obj->cx256 = (kk);
		obj->cy256 = ((3*8+2)<<8);
		//
		kk += (8*256);
	}
}
