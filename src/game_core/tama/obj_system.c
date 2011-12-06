
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	スプライトオブジェクト システム
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
.   -------------------------------------------------------
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
int obj_collision_laser( /* obj_t */OBJ *obj1, LASER *laser1)
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
	center_x256 = (laser1->center.x256);
	center_y256 = (laser1->center.y256);
	/* 原点からの差分ベクトル成分。 */
	sabun_x = (obj1->center.x256)-(center_x256);
	sabun_y = (obj1->center.y256)-(center_y256);
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
static /* BOOL */int obj_collision_tama( /* obj_t */OBJ *obj1, /* obj_t */OBJ *obj2 )
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
	dx = ((obj2->center.x256) - (obj1->center.x256));/* 中心同士で差を求める。 */
	dy = ((obj2->center.y256) - (obj1->center.y256));/* 中心同士で差を求める。 */

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
	スプライトオブジェクトのあたり判定
	-------------------------------------------------------
	入力値: type
		OBJ_Z04_TAMA		(弾)
		OBJ_Z03_ITEM		(アイテム)
--------------------------------------------------------- */
/*---------------------------------------------------------
	(敵専用)スプライトオブジェクトのあたり判定
--------------------------------------------------------- */

global OBJ obj99[OBJ_LAST_99_0x0a48_MAX];	/* 全スプライトオブジェクト */

static OBJ *obj_collision_check(OBJ/**/ *tocheck, OBJ/**/ *s, int set_obj_type, unsigned int length)
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
						if (obj_collision_tama(s, tocheck)) 	/* 矩形/円あたり判定 */ 	/* hit collision rectangle to circle check. */
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
/*static*/global OBJ *obj_collision_check_00_tama(OBJ/**/ *tocheck, int type)
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_00_0x0000_TAMA+0];
	return obj_collision_check(tocheck, s, type, OBJ_POOL_00_TAMA_1024_MAX);
}
//	/*static*/global OBJ *obj_collision_check_A01_A02_teki(OBJ/**/ *tocheck, int type)
/*static*/global OBJ *obj_collision_check_A01_A02_teki(OBJ/**/ *tocheck)/*, (OBJ_Z02_TEKI)int ty pe*/
{
	OBJ *s;
//	s = &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+0];
//	return obj_collision_check(tocheck, s, /*type*/(OBJ_Z02_TEKI), (/*OBJ_POOL_01_TOUROKU_0256_MAX+*/OBJ_POOL_02_TEKI_FIX_0256_MAX));
	s = &obj99[OBJ_HEAD_01_0x0800_TOUROKU+0];
	return obj_collision_check(tocheck, s, /*type*/(OBJ_Z02_TEKI), (OBJ_POOL_01_TOUROKU_0256_MAX + OBJ_POOL_02_TEKI_FIX_0256_MAX));
}


/*---------------------------------------------------------
	スプライトオブジェクトを一フレーム分、移動する。
--------------------------------------------------------- */
//	for (iii=0; iii<length; iii++)/* 全部調べる。 */
	//
	#if(1)/*(めもr33)*/
	/*
	[移動]		弾専用スプライトオブジェクト(1024)[アイテムや星点や発弾エフェクトの移動]
	[移動]		固定敵専用スプライトオブジェクト(256)
	[移動]		敵専用スプライトオブジェクト(256)
	[移動]		自機等固定スプライトオブジェクト(16)
	[非移動]	パネル用スプライトオブジェクト(56)
	*/
global void obj_system_move_all(void)
{
	OBJ *s; 	s = &obj99[OBJ_HEAD_04_0x0a10_PANEL];
	unsigned int length;	length = (
			OBJ_POOL_00_TAMA_1024_MAX +
			OBJ_POOL_01_TOUROKU_0256_MAX +
			OBJ_POOL_02_TEKI_FIX_0256_MAX +
			OBJ_POOL_03_KOTEI_0016_MAX);
	unsigned int iii;
	iii = length;/* レーザーの実装を簡単にする為に、逆順にする。 */
	{
		my_loop:
		if (JYUMYOU_NASI < s->jyumyou)/* 消去済みは飛ばす。 */
		{
			s->jyumyou--;/* 寿命経過 */
			/*(移動処理があれば移動する。)*/
			if (NULL != s->callback_mover)
			{
				(s->callback_mover)(s);/*(移動処理)*/
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
/*(r35, 将来的に弾の移動は無しになる可能性が高い。その場合 effect は別システムになる)*/
	#endif
	//
	#if(0)/*(ダメ)*/
extern void hatudan_effect_move_all(void);
global void obj_system_move_all(void)
{
	hatudan_effect_move_all();
	/*
	[非動作]	弾専用スプライトオブジェクト(1024)
	[動作]		固定敵専用スプライトオブジェクト(256)
	[動作]		敵専用スプライトオブジェクト(256)
	[動作]		自機等固定スプライトオブジェクト(16)
	[非動作]	パネル用スプライトオブジェクト(56)
	*/
	OBJ *s;
	s = &obj99[OBJ_HEAD_04_0x0a10_PANEL];
	obj_system_move_all(s, (
		OBJ_POOL_01_TOUROKU_0256_MAX +
		OBJ_POOL_02_TEKI_FIX_0256_MAX +
		OBJ_POOL_03_KOTEI_0016_MAX));
}
/*(r35, 将来的に弾の移動は無し)*/
	#endif

#if 1
/*---------------------------------------------------------
	画面外ならおしまい(個別、主に汎用objで使用)
---------------------------------------------------------*/
global OBJ_CALL_FUNC(gamen_gai_nara_zako_osimai)
{
	if (
		(src->center.x256 < t256(GAME_X_OFFSET)) ||
		(src->center.x256 > t256(GAME_WIDTH)) ||
		(src->center.y256 < t256(0)) ||
		(src->center.y256 > t256(GAME_HEIGHT)) )		/* 画面外に出たら */
	{
		src->jyumyou = JYUMYOU_NASI;		/* おしまい */
	}
}
//	if ((src->center.x256 < t256(0))||(src->center.x256 > t256(GAME_WIDTH))||
//		(src->center.y256 < t256(0))||(src->center.y256 > t256(GAME_HEIGHT)))
//	{
//		src->jyumyou = JYUMYOU_NASI;/*画面外にでたらおしまい*/
//	}
/*---------------------------------------------------------
	画面外ならおしまい(個別、主に汎用objで使用)
---------------------------------------------------------*/
//global OBJ_CALL_FUNC(gamen_gai_limit_30_nara_zako_osimai)
//{
//	if ((JYUMYOU_ZAKO_CLIP) > src->jyumyou) 		/* 離脱 */
//	{
//		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
//	}
//}
#endif



/*---------------------------------------------------------
	使用中であるが退避したい場合。画面外にスプライトオブジェクトを移動させ、無効にする。
	-------------------------------------------------------
	スプライトオブジェクトを確保したまま退避させる。
	ボスobj, effect obj で使用中。
---------------------------------------------------------*/
global OBJ_CALL_FUNC(sprite_kotei_obj_r36_taihi)
{
	src->jyumyou				= (JYUMYOU_MUGEN);/* 時間で自動消去しない */	/*(固定確保で判別されるフラグ)*/
	src->m_Hit256R				= ZAKO_ATARI16_PNG;
	src->atari_hantei 			= (ATARI_HANTEI_OFF/*スコア兼用*/); 	/* あたり判定のOFF(無敵) */
	src->callback_mover			= NULL; 	/*(移動処理)*/
	src->callback_kougeki 		= NULL; 	/*(攻撃処理)*/
	src->callback_hit_teki		= NULL; 	/*(被弾時処理) ダミーコールバック登録 */
	src->callback_loser			= NULL; 	/*(死亡時処理)*/
	src->center.x256			= (t256( GAME_X_OFFSET + ((GAME_320_WIDTH)/2) ));
	src->center.y256			= (t256(-256));
}
//???	h->obj_type_set 	= BOSS_00_11;

/*---------------------------------------------------------
	スプライトオブジェクトの初期化をする。
	-------------------------------------------------------
	リスト方式でないスプライトオブジェクトも初期化する為、
	外部からも呼ばれるので注意。
--------------------------------------------------------- */
//tern OBJ_CALL_FUNC(obj_initialize_one);
global OBJ_CALL_FUNC(obj_initialize_one)
{
	#if (0==USE_MEM_CLEAR)/* [メモリゼロクリアー機能]を使わない場合は、初期化が必要。 */
	#else
	memset(src, 0, sizeof(OBJ));
	#endif
//
	src->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	obj->alpha		= 0xff;*/
	#if 1/* 自動的に消える機能 */
	src->jyumyou				= JYUMYOU_1MIN; 		/* 1分したら勝手に自動消去。 */
//	src->timeover_ticks 		= 0;
	#endif
	src->m_zoom.x256			= t256(1.00);	/* 表示拡大率 256 == [ x 1.00 ] */
	src->m_zoom.y256			= t256(1.00);	/* 表示拡大率 256 == [ x 1.00 ] */
//	src->m_zoom.y256			= M_ZOOM_Y256_NO_TILT;/* 特殊機能で傾かないようシステム拡張(r33)。 */
	/*([r39]m_zoomの初期値を変更テスト、[回転1.00倍モードから非回転、強制1.00倍モードへ])*/
	#if (0==USE_MEM_CLEAR)/* [メモリゼロクリアー機能]を使わない場合は、初期化が必要。 */
	src->rotationCCW1024		= 0;	/* 描画用角度(下が0度で左回り(反時計回り)) */	/* 0 == 傾かない。下が0度 */
//
	src->math_vector.x256		= t256(0);
	src->math_vector.y256		= t256(0);
//
	src->callback_mover 		= NULL;
	src->callback_kougeki		= NULL; 	/*(攻撃処理)*/
	src->callback_hit_teki		= NULL;
	src->callback_loser 		= NULL;
	#endif	/* (0==USE_MEM_CLEAR) */
}


/*---------------------------------------------------------
	gu移行中、弾専用スプライトオブジェクトをリストに追加する。
	gu移行中、汎用スプライトオブジェクトをリストに追加する。
	登録できない場合、NULLを返す。
--------------------------------------------------------- */

/*---------------------------------------------------------
	弾領域に敵弾を登録する。
--------------------------------------------------------- */

global OBJ *obj_regist_tama(void)
{
	#if 1
	/* private (この関数で)プライベートな変数 */
	static u32 register_num;/*(登録できる可能性が高そうな位置を保持)*/
	#endif
	OBJ *obj;/*(新規作成するスプライトオブジェクト)*/
	{
		u32 remain_count;/*(登録出来る残り数)*/
		remain_count = (OBJ_POOL_00_TAMA_1024_MAX-1);
my_retry:
		remain_count--;
		if (0 == remain_count)
		{
			#if (0)/*(デバッグ)*/
			/*(アリスはともかく文で出る!!何でだろ？)*/
			psp_fatal_error( (char*)
			//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
				"regist tama: %d 登録可能\最大弾数を、" "\\n"
				"超えています。", remain_count);
			#endif
			return (NULL);/*(登録できなかった)*/
		}
		register_num++; 	/*(登録できる可能性が高そうな位置)*/
		register_num &= (OBJ_POOL_00_TAMA_1024_MAX-1);
		obj = &obj99[OBJ_HEAD_00_0x0000_TAMA+register_num];
		if (JYUMYOU_NASI < (obj->jyumyou))/*(使用中ならもう一度探す。)*/
		{
			goto my_retry;/*(探す。)*/
		}
	}
	obj_initialize_one(obj);			/* 新規作成したスプライトオブジェクトを初期化 */
	#if 1
	/* 標準初期化 */
	obj->atari_hantei			|= (1/*(スコア兼用)*/); 	/* あたり判定あり */
	#endif
	return (obj);/*(登録できた)*/
}
/*---------------------------------------------------------
。
--------------------------------------------------------- */

/*
(r36)boss objは敵汎用objから固定確保するように仕様変更した。
*/
static u32 register_kotei_first;	/* 登録敵が、登録できる位置 */
static u32 register_kotei_max;		/* 登録敵が、登録できる位置 */

/*(固定敵を確保)*/
global void obj_touroku_push(void)
{
	register_kotei_first++;
	register_kotei_max--;
}
/*(固定敵を減らす)*/
global void obj_touroku_pop(void)
{
	register_kotei_first--;
	register_kotei_max++;
}
/*---------------------------------------------------------
	敵オブジェ登録領域に敵オブジェを登録する。
--------------------------------------------------------- */

/*(bossを固定確保する分)*/
//#define BOSS_OBJ_KOTEI_KAKUHO_BUN (1)
global OBJ *obj_regist_teki(void)
{
	#if 1
	/* private (この関数で)プライベートな変数 */
	static u32 register_num;/*(登録できる可能性が高そうな位置を保持)*/
	#endif
	OBJ *obj;/*(新規作成するスプライトオブジェクト)*/
	{
		u32 remain_count;/*(登録出来る残り数)*/
		remain_count = register_kotei_max;
my_retry:
		remain_count--;
		if (0 == remain_count)
		{
			return (NULL);/*(登録できなかった)*/
		}
		register_num++; 	/*(登録できる可能性が高そうな位置)*/
		if (register_num >= (OBJ_POOL_01_TOUROKU_0256_MAX-1))
		{
			register_num = register_kotei_first;
		}
		obj = &obj99[OBJ_HEAD_01_0x0800_TOUROKU+register_num];
		if (JYUMYOU_NASI < (obj->jyumyou))/*(使用中ならもう一度探す。)*/
		{
			goto my_retry;/*(探す。)*/
		}
	}
	obj_initialize_one(obj);		/* 新規作成したスプライトオブジェクトを初期化 */
	return (obj);/*(登録できた)*/
}
/*---------------------------------------------------------
	番号を指定して直接登録する。
--------------------------------------------------------- */

global OBJ *obj_regist_direct_number(unsigned int direct_register_number)
{
//	int priority;		priority		= image_resource_ptr->priority;
	OBJ *obj;/* 新規作成するスプライトオブジェクト */
	obj = &obj99[direct_register_number];
	obj_initialize_one(obj);			/* 新規作成したスプライトオブジェクトを初期化 */
	return (obj);
}

/*---------------------------------------------------------
	スプライトオブジェクト リストに登録されたスプライトオブジェクトを全部消す。
--------------------------------------------------------- */

/* [A00弾領域]と[A01固定敵領域]と[A02敵領域]と[A03固定領域]と[A04パネル領域]のOBJを全消去。 */
global void obj_system_cleanup_all(void)
{
	OBJ *sss;
	sss = &obj99[OBJ_HEAD_00_0x0000_TAMA+0];
	unsigned int length;
	length = (
		OBJ_POOL_00_TAMA_1024_MAX +/* 弾専用 */
		OBJ_POOL_01_TOUROKU_0256_MAX +/* gu汎用(旧SDL) */
		OBJ_POOL_02_TEKI_FIX_0256_MAX +/* gu汎用(旧SDL) */
		OBJ_POOL_03_KOTEI_0016_MAX +
		OBJ_POOL_04_PANEL_0056_MAX);
	{
	re_try_loop:
	//	obj_initialize_one(sss);
		sss->jyumyou		= JYUMYOU_NASI;
		#if (1)/* デバッグてすと */
		sss->center.x256	= (-1);
		#endif
		sss++;
		//
		length--;
		if (0 != length) goto re_try_loop;
	}

}

/*---------------------------------------------------------
	スプライトオブジェクト システム初期化。
--------------------------------------------------------- */

global void obj_system_init(void)
{
	/*(登録敵スプライトの初期化)*/
	register_kotei_first	= 0;
	register_kotei_max		= (OBJ_POOL_02_TEKI_FIX_0256_MAX-1/*-(BOSS_OBJ_KOTEI_KAKUHO_BUN)*/);
}

/*---------------------------------------------------------
	プレイヤーにとっての敵を探す
---------------------------------------------------------*/

global OBJ *obj_teki_search(void)
{
	int ii;
	for (ii=0; ii<OBJ_POOL_02_TEKI_FIX_0256_MAX; ii++)/* 全部調べる。 */
	{
		OBJ *h;
		h = &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+ii];
	//
		if (
			#if 1
			(JYUMYOU_NASI <= h->jyumyou) && /* 削除済みは飛ばす */
			#endif
			(0 != (h->obj_type_set & (OBJ_Z02_TEKI )) ) 	/* プレイヤーにとっての敵(ザコやボス) */
		)
		{
			{
				#if 1/* Gu(中心座標) */
				if ((h->center.x256 > 0) && (h->center.x256 < t256(GAME_WIDTH ) ) &&
					(h->center.y256 > 0) && (h->center.y256 < t256(GAME_HEIGHT) ))	/* 表示画面内の敵 */
				#endif
				{
					return (h);/* 見つけたよ */
				}
			}
		}
	}
	return (&obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER]);/* 見つからなかった */
}
