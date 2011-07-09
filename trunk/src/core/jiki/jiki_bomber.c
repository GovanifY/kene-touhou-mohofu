
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	各プレイヤー(霊夢 ＆ 魔理沙 ＆ レミリア ＆ チルノ ＆ 幽々子)
	REIMU(A/B) MARISA(A/B/C) REMILIA CIRNO YUYUKO
	-------------------------------------------------------
---------------------------------------------------------*/

#include "jiki_local.h"

/*---------------------------------------------------------
	player_bomber.h
---------------------------------------------------------*/

//typedef struct
//{
//	int speed256;
//	int state;
//	int time_out;			/* 設定時間内に見つけられない場合はおしまい */
//	SPRITE *target_obj; 	/* int target_id */
//} PL_HOMING_DATA; 		/* == PL_HLASER_DATA */

#define YUUDOU_BOMBER_speed256					user_data01
#define YUUDOU_BOMBER_reimu_musou_id			user_data02
//efine PL_HOMING_time_out						YUUDOU_BOMBER_reimu_musou_id		/* 設定時間内に見つけられない場合はおしまい */
//efine PL_HOMING_KODOMO_DATA_color256			user_data04

//	int angleCCW1024;
//	int nr; 				/* 右か左(ホーミング), nr==number */


//typedef struct
//{
#define PL_HOMING_KODOMO_DATA_check_x256		user_data00
#define PL_HOMING_KODOMO_DATA_check_y256		user_data01
/*???*/#define PL_HOMING_KODOMO_DATA_time_out	user_data02 		/* 設定時間内に見つけられない場合はおしまい */
//#define PL_HOMING_KODOMO_DATA_color256		user_data04
//	SPRITE *target_obj; 	/* int target_id */
//} PL_HOMING_KODOMO_DATA;	/* == PL_HLASER_DATA */

//typedef struct
//{
#define PL_KEKKAI_DATA_angleCCW1024 			user_data00
#define PL_KEKKAI_DATA_add_r1024				user_data01
#define PL_KEKKAI_DATA_radius					user_data02 		/* 半径(radius) */
//} PL_KEKKAI_DATA;



/*
colision_check あたり判定 -------------------------------------------------------------------------
*/

/*---------------------------------------------------------
	プレイヤー、高速時ボム。画面外はみ出しチェック。あたり判定チェック。
---------------------------------------------------------*/
static void player_bomber_out_colision_check(SPRITE *src)
{
	#if 1/*Gu(中心座標)*/
	if ((src->cx256 < t256(0))||(src->cx256 > t256(GAME_WIDTH))||
		(src->cy256 < t256(0))||(src->cy256 > t256(GAME_HEIGHT)))
	#endif
	{
		src->jyumyou = JYUMYOU_NASI;/* 画面外に出たらおしまい */
	}
	else
	{
		player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_01_BOMB);/* ボムで敵を倒すあたり判定 */
	}
}



/*
move ショット/ボム移動 -------------------------------------------------------------------------
*/

/*---------------------------------------------------------
	プレイヤー、高速時ボムの移動(霊夢)

	時計回り ＆ 反時計回り ＆ スクエア結界(霊夢B) 兼用
---------------------------------------------------------*/

static void reimu_move_rotate_kekkai(SPRITE *src) /* 霊夢 */
{
	src->PL_KEKKAI_DATA_angleCCW1024 += (src->PL_KEKKAI_DATA_add_r1024);	/*fps_factor*/	/*...*/
	mask1024(src->PL_KEKKAI_DATA_angleCCW1024);
	#if 1
	/* 描画用角度(下が0度で左回り(反時計回り)) */
	src->rotationCCW1024 = (src->PL_KEKKAI_DATA_angleCCW1024);
	#endif
//
	SPRITE *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	#if 1/*Gu(中心座標)*/
	src->cx256 = zzz_player->cx256 + sin1024((src->PL_KEKKAI_DATA_angleCCW1024))*src->PL_KEKKAI_DATA_radius;	/* CCWの場合 */
	src->cy256 = zzz_player->cy256 + cos1024((src->PL_KEKKAI_DATA_angleCCW1024))*src->PL_KEKKAI_DATA_radius;
	#endif
	/* スクエア結界(霊夢B) */
	if (REIMU_A != (cg_game_select_player))
	{
		if ( 0 < src->PL_KEKKAI_DATA_add_r1024)
		{
			if (zzz_player->cx256 < src->cx256)
					{	src->cx256 = zzz_player->cx256 + ((src->PL_KEKKAI_DATA_radius)<<8); 	}
			else	{	src->cx256 = zzz_player->cx256 - ((src->PL_KEKKAI_DATA_radius)<<8); 	}
		}
		else
		{
			if (zzz_player->cy256 < src->cy256)
					{	src->cy256 = zzz_player->cy256 + ((src->PL_KEKKAI_DATA_radius)<<8); 	}
			else	{	src->cy256 = zzz_player->cy256 - ((src->PL_KEKKAI_DATA_radius)<<8); 	}
		}
	}
//	if (REIMU==(cg_game_select_player))
	{
		if (src->PL_KEKKAI_DATA_radius<255) 	{	src->PL_KEKKAI_DATA_radius += (ra_nd()&1);	}	/* 少しずつ広がるようにした */
		else									{	src->jyumyou = JYUMYOU_NASI;
													cg.state_flag &= (~STATE_FLAG_02_BOMB_AUTO_GET_ITEM);	/* ボムによる自動収集は終わり */
												}	/* 回転が広がったらおしまい */
	//	src->alpha				= ((int)(255-src->PL_KEKKAI_DATA_radius))&(256-1);	/* だんだん消えるようにした */
	//	src->color32			= ((((255-src->PL_KEKKAI_DATA_radius))&(0xff))<<24)|0x00ffffff; /* だんだん消えるようにした */
		src->color32			= ((((255-src->PL_KEKKAI_DATA_radius))&(0xff))<<24)|0x00ffffff; /* だんだん消えるようにした */
	}
//	else //if (YUYUKO==(cg_game_select_player))
//	{

//		if (pd_bomber_time<=0)	{	src->jyumyou = JYUMYOU_NASI;}/* 時間制限でおしまい */
//	}
	if ( (ra_nd()&0xff) < cg.bomber_time)
	{
		if (0==((src->PL_KEKKAI_DATA_radius)&7))
		{
			//player_weapon_colision_check_kekkai(src, src->PL_KEKKAI_DATA_radius);
			bullets_to_hosi();/* 弾全部、星アイテムにする */
		}
	}
	player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_01_BOMB);/* ボムで敵を倒すあたり判定 */
}


/*---------------------------------------------------------
	プレイヤー、高速時ボムの移動(魔理沙B マスタースパーク専用)
---------------------------------------------------------*/
#define MASKER_SPARK_LENGTH144 (144)
static void marisa_move_master_spark(SPRITE *src)
{
	SPRITE *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
//	src->cx256 += src->vx256;	/*fps_factor*/
//	src->cy256 += src->vy256;	/*fps_factor*/
	src->cx256 = zzz_player->cx256; 			/*fps_factor*/
	src->cy256 = zzz_player->cy256-t256(MASKER_SPARK_LENGTH144);	/*fps_factor*/
	{
		static const u16 maspa_kaiten[4] =
		{
			(18),
			(1024-24),
			(10),
			(1024-6),
			/* 回転速度 */
		};
		/* 表示角度(下が0度で左回り(反時計回り)) */
		src->tmp_angleCCW1024	+= maspa_kaiten[((src->type)& (0x03))];
		mask1024((src->tmp_angleCCW1024));
		src->rotationCCW1024 = ((sin1024((src->tmp_angleCCW1024)))>>2);
	}
	/* 時間でおしまい */
	if (0 >= cg.bomber_time)	{ src->jyumyou = JYUMYOU_NASI;}/* 時間制限でおしまい */
	else
	if (0x40 > cg.bomber_time)	/* 0x3f ... 0x01 */
	{
		src->color32			= ((((cg.bomber_time))/*&(0x3f)*/)<<(24+2))|0x00ffffff; /* だんだん消えるようにした */
		/* (この値の範囲なら)実はマスク必要ない。 */
	}
	player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_01_BOMB);/* ボムで敵を倒すあたり判定 */
//	player_bomber_out_colision_check(src);/* 画面外に出たらおしまい & ボムで敵を倒すあたり判定 */
}

/*---------------------------------------------------------
	プレイヤー、高速時ボムの移動(魔理沙、チルノ、仮幽々子(違う感じにしたい) )
---------------------------------------------------------*/

static void marisa_yuyuko_move_levarie_gyastry_dream(SPRITE *src)
{
	src->cx256 += src->vx256;	/*fps_factor*/
	src->cy256 += src->vy256;	/*fps_factor*/
	#if 1
	/* YUYUKO MARISA_A 以外 */
	if (
	//	(CIRNO_A==(cg_game_select_player)) || (CIRNO_Q==(cg_game_select_player)) ||
		((CIRNO_A-1)<(cg_game_select_player)) //||
	//	(MARISA_B==(cg_game_select_player))/* マスパ(もどき) */
	)
	#endif
	{	/*チルノ用(暫定的)*/	/*1.5*(d->speed);*/ 	/*fps_factor*/
		src->cy256 -= (abs((src->vx256+(src->vx256>>1) )));
		src->cy256 -= (abs((src->vy256+(src->vy256>>1) )));
		src->cy256 -= t256(1.0);
	}
	{
		static const u8 aaa_sss[16] =
		{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
//			0, 0,  4, 16, 0, 0, 18, 18, /* 回転速度 */	/* YUYUKO			以外は 回転 する。 */
			0, 0, 20, 16, 0, 0, 18, 18, /* 回転速度 */	/* YUYUKO			以外は 回転 する。 */
//			0, 0,  2,  0, 0, 0,  3,  3, /* 拡大速度 */	/* YUYUKO  MARISA_B 以外は 拡大 する。 */
			0, 0,  0,  0, 0, 0,  3,  3, /* 拡大速度 */	/* CIRNO は 拡大 する。 */
		};
		/* 表示角度(下が0度で左回り(反時計回り)) */
		src->rotationCCW1024	+= aaa_sss[(cg_game_select_player)];
		mask1024((src->rotationCCW1024));
		if ( t256(4.0) > src->m_zoom_x256)
		{
			src->m_zoom_x256 += aaa_sss[(cg_game_select_player)+(PLAYERS8)];
		//	src->m_zoom_y256 += aaa_sss[(cg_game_select_player)+(PLAYERS8)];
			src->m_zoom_y256 = src->m_zoom_x256;
		}
	}
	player_bomber_out_colision_check(src);
}

/*---------------------------------------------------------
	プレイヤー、高速時ボムの移動(レミリア)
---------------------------------------------------------*/

static void remilia_move_burn_fire(SPRITE *src)
{
	#if 1
	// 加速
	// x1.10
//	src->vx256 = ((src->vx256 * t256(1.1))>>8);//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
//	src->vy256 = ((src->vx256 * t256(1.1))>>8);//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
	// x1.125
	src->vx256 += ((src->vx256)>>3);//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
	src->vy256 += ((src->vy256)>>3);//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
	// x1.0625
//	src->vx256 += ((src->vx256)>>4);//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
//	src->vy256 += ((src->vy256)>>4);//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
	/* 描画が重すぎるので加速性能、若干速く( x1.0625→ x1.125)する */
	//
	#endif
	src->cx256 += src->vx256;	//	src->vx256;//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
	src->cy256 += src->vy256;	//	src->vy256;//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
	//
	player_bomber_out_colision_check(src);
}

/*---------------------------------------------------------
	誘導弾の誘導移動サブルーチン
---------------------------------------------------------*/

static void yuudou_idou(SPRITE *src)
{
	SPRITE *target; 	/* 目標 */
	target = src->target_obj;
	/* 目標が画面内ならば目標に向かう */
	if (//(NULL != target) &&	/* 重複してるので排除 */
	//	(target->flags&SP_FLAG_VISIBLE) &&
		(target->cx256 > t256(0)) &&
		(target->cx256 < (t256(GAME_WIDTH)) ) &&
		(target->cy256 > t256(0)) &&
		(target->cy256 < (t256(GAME_HEIGHT)) )) 	/*Gu(中心座標)*/
	{
		#if (1)/*誘導(000)*/
		{int ta1024;
		#if 1
		/* CCWの場合 */
		//	ta1024 = at an_1024(target->cy256-src->cy256,target->cx256-src->cx256);
			ta1024 = atan_65536(target->cy256-src->cy256,target->cx256-src->cx256);
			ta1024 >>= (6);
			#if 1
			/* 遅れ誘導 */
			/* KETMのは考え方が複雑なので破棄、0度からの差分で考え180度を境に分岐すれば同等(1周は360度しかないので) */
		//	const int sabun_aaa = (src->tmp_angleCCW1024-src->rotationCCW1024);
			const int sabun_aaa1024 = (ta1024-src->rotationCCW1024);
			if (0 > sabun_aaa1024/*(src->tmp_angleCCW1024-src->rotationCCW1024)*/ )/* 0レジスタ+共通最適化 */
			{	/* 目標角度 < 制御角度 */
				if ( (1024/2) > sabun_aaa1024/*(src->tmp_angleCCW1024-src->rotationCCW1024)*/ )/* 共通最適化 */
						{	src->rotationCCW1024 -= 10; }	/* 180度以内なら近いほうから回る */
				#if 0	/* 省略して問題ない？ */
				else	{	src->rotationCCW1024 += 10; }	/* 180度以上なら反対周りのほうが近い */
				#endif
			}
			else	/* 目標角度 > 制御角度 */
			{
				if ( (1024/2) > -(sabun_aaa1024)/*(src->rotationCCW1024-src->tmp_angleCCW1024)*/ )
						{	src->rotationCCW1024 += 10; }	/* 180度以内なら近いほうから回る */
				#if 0	/* 省略して問題ない？ */
				else	{	src->rotationCCW1024 -= 10; }	/* 180度以上なら反対周りのほうが近い */
				#endif
			}
			#else
			/* 直接誘導 */
			src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/ = ta1024;
			#endif
		#endif
		}
		mask1024(src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/);
		#endif/*誘導(000)*/
	}
	else	/* 目標が画面外ならば新しい目標を探す。 */
	{
		/* Ziel wurde anderweitig vernichtet, neues Ziel suchen */
		src->target_obj = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];		/* 画面外に逃げられた */
	}
}

/* ------------------------------------------------------------------------ */

/*---------------------------------------------------------
	プレイヤー、誘導弾の移動(親)制御
---------------------------------------------------------*/

enum
{
	HOMING_096_TIME_OUT = 96,
	HOMING_128_TIME_OUT = 128,
};
static void marisa_move_parrent_hlaser(SPRITE *src)
{
	/* 他の誘導ボムが、既に倒したか？ */
	if (&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER] == src->target_obj) /* 見つからない(他の誘導ボムが倒してしまった場合) */
	{
		/* ターゲット検索、敵を探す。 */
		src->target_obj = search_teki_from_obj();
	}
	else	/* まだターゲットが生きてる */
	{
		if (HOMING_096_TIME_OUT < src->jyumyou)
		{
			/* 一定時間減速 */
			src->YUUDOU_BOMBER_speed256 -= t256(0.5);/*減速*/
			// 止まると困る
			if ( 0 > src->YUUDOU_BOMBER_speed256 )
			{
				src->YUUDOU_BOMBER_speed256 = t256(0.75);	/* 最小値 */
			}
		}
		else
		{
			/* 一定時間を超えたら加速 */
			src->YUUDOU_BOMBER_speed256 += t256(0.5);/*加速*/
			// 速過ぎるのも困る
		//	if ( t256(12.0) < src->YUUDOU_BOMBER_speed256)
			if ( t256(16.0) < src->YUUDOU_BOMBER_speed256)
			{
				src->YUUDOU_BOMBER_speed256 = t256(16.0);	/* 最大値 */
			}
		}
		//
		yuudou_idou(src);
	}
	#if 1
	/* (多分必要ないけど)念の為マスク */
	mask1024(src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/);
	#endif
	#if 1
	/* 表示用 */
//	src->rotationCCW1024	= src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/;
	#endif
	src->cx256 += ((sin1024((src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/))*src->YUUDOU_BOMBER_speed256)>>8);/*fps_factor*/	/* CCWの場合 */
	src->cy256 += ((cos1024((src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/))*src->YUUDOU_BOMBER_speed256)>>8);/*fps_factor*/
	player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_00_SHOT);
//
//	src->jyumyou--;/*fps_factor*/
//	if (0 > src->jyumyou ) /* 時間切れ？ */
//	{
//		src->jyumyou = JYUMYOU_NASI;		/* 時間切れなのでおしまい */
//	}
}

/*---------------------------------------------------------
	霊夢専用 低速ボム
	霊符霊夢
---------------------------------------------------------*/

static void move_reimu_musou_fuuin(SPRITE *src)
{
	/* 他の誘導ボムが、既に倒したか？ */
	if (&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER] == src->target_obj) /* 見つからない(他の誘導ボムが倒してしまった場合) */
	{
		/* ターゲット検索、敵を探す。 */
		src->target_obj = search_teki_from_obj();
	}
	else	/* まだターゲットが生きてる */
	{
		if ( t256(3.0) > src->YUUDOU_BOMBER_speed256)
		{
			src->YUUDOU_BOMBER_speed256 += /*(int)*/t256(0.5);/*加速*/
		}
		//
		yuudou_idou(src);
	}
// ボムは最大 7 個(8)
//	if ((cg.bomber_time&0x0f)==(src->YUUDOU_BOMBER_reimu_musou_id&0x0f))
	if ((cg.bomber_time&0x07)==(src->YUUDOU_BOMBER_reimu_musou_id&0x07))
	{
		src->vx256 = ((sin1024((src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/))*src->YUUDOU_BOMBER_speed256)>>8);/*fps_factor*/ /* CCWの場合 */
		src->vy256 = ((cos1024((src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/))*src->YUUDOU_BOMBER_speed256)>>8);/*fps_factor*/
	}
		src->cx256 += (src->vx256);/*fps_factor*/
		src->cy256 += (src->vy256);/*fps_factor*/
	player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_01_BOMB/*PLAYER_WEAPON_TYPE_00_SHOT*/);
//
	if (0 >= cg.bomber_time)	/* 時間切れの場合 */
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
		src->base_weapon_strength--;/* 時間切れの場合、体力を減らす */
	}
//
	if (0 > src->base_weapon_strength)	{	src->jyumyou = JYUMYOU_NASI;	}	/* 体力切れでおしまい */
}
/*---------------------------------------------------------
	プレイヤー、誘導弾の移動(子供)
---------------------------------------------------------*/

static void marisa_move_kodomo_hlaser(SPRITE *src)
{
	#if 1
	SPRITE *oya;
	oya = src->target_obj;
	if (
		//	(SP_DELETE == oya->type) || 			/* 親が消去済みならば子も消去 */
			(JYUMYOU_NASI > oya->jyumyou) ||			/* 親が消去済みならば子も消去 */
		//	((SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL) != oya->type)			/* (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)以外は親でないので消去 */
			(0==(SP_GROUP_SHOT_SPECIAL & oya->type))	/* (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)以外は親でないので消去 */
		)
	{
		src->jyumyou = JYUMYOU_NASI;/*おしまい*/
	}
//
	#if 1
	/* 表示用 */
	src->rotationCCW1024	= oya->rotationCCW1024;/*src->PL_HOMING_KODOMO_DATA_angleCCW1024*/
	#endif
	src->cx256 = oya->cx256;
	src->cy256 = oya->cy256;
//	src->PL_HOMING_KODOMO_DATA_color256 = ((oya->PL_HOMING_KODOMO_DATA_color256*t256(0.96))>>8)&0xff;
//	src->PL_HOMING_KODOMO_DATA_color256 = ((oya->PL_HOMING_KODOMO_DATA_color256*t256(0.90))>>8)&0xff;
//	src->PL_HOMING_KODOMO_DATA_color256 = ((oya->PL_HOMING_KODOMO_DATA_color256 - 0x10)&0xff);
//	src->color32 = (src->PL_HOMING_KODOMO_DATA_color256<<24)|(0x00ffffff);
	#if (1)/*???*/
	/* 数フレーム動かない場合は、自動消去する */
	if ((src->PL_HOMING_KODOMO_DATA_check_x256 != src->cx256) ||
		(src->PL_HOMING_KODOMO_DATA_check_y256 != src->cy256))
	{
		src->PL_HOMING_KODOMO_DATA_time_out = 8;/* 動いたら自動消去しない */
	}
	src->PL_HOMING_KODOMO_DATA_time_out--;
	if ( 0 > src->PL_HOMING_KODOMO_DATA_time_out)
	{
		src->jyumyou = JYUMYOU_NASI;/*おしまい*/
	}
	#endif
	src->PL_HOMING_KODOMO_DATA_check_x256 = src->cx256;
	src->PL_HOMING_KODOMO_DATA_check_y256 = src->cy256;
	#endif
}





/*
add1-------------------------------------------------------------------------
*/
/* (幽々子低速)十字炎ボムの炎の部分 */
/*		r_or_l	[xxx_r] l=2, [xxx_l] r=0, u=3, d=1	*/
static void yuyuko_add_meifu(SPRITE *src)
{
	int angCCW1024;
	for (angCCW1024=0; angCCW1024<(1024); angCCW1024+=(256)/*(20)*/)
	{
		int i;
		for (i=0; i<10; i+=1)
		{
			SPRITE *h;
			h = obj_add_01_teki_error();
			if (NULL!=h)/* 登録できた場合のみ */
			{
				//
				h->m_Hit256R		= JIKI_ATARI_ITEM_16;
//				h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
				h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL);/* ボスに有効 */
				h->callback_mover	= remilia_move_burn_fire;
				h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x64);		/*	h->alpha			= 0x64 100;*/
				h->base_weapon_strength 	= (8*(5));/* 仕様変更した */	/*9*/ /*10*/
				{
			u32 my_rand;
				my_rand = ra_nd();
					if (YUYUKO==(cg_game_select_player))
					{
						h->cx256			= (u16)((my_rand	)) + t256((GAME_WIDTH -256)/2);
						h->cy256			= (u16)((my_rand>>16)) + t256((GAME_HEIGHT-256)/2);
					}
					else
					{
						#if 1/*Gu(中心座標)*/
						h->cx256			= src->cx256;
						h->cy256			= src->cy256+t256(15);
						#endif
					}
				//	int rnd_spd 		= (ra_nd() & 0xff/*angCCW512*/)+256+((angCCW512+32+64) & 0x40);
				//	int aaa_spd 		= (((angCCW512+32+64) & 0x40));
				//	int rnd_spd 		= (ra_nd() & 0xff/*angCCW512*/)+256+(aaa_spd+aaa_spd);
			int rnd_spd 		= (my_rand & 0xff/*angCCW512*/)+256+(0x40+0x40);
			int ang_aaa_1024;
				ang_aaa_1024 = angCCW1024;
				ang_aaa_1024 += ((my_rand>>8) & (0x7f));
				ang_aaa_1024 += ((my_rand	) & (0x3f));
			//	ang_aaa_1024 -= (0x3f);
			//	ang_aaa_1024 -= (0x1f);
				ang_aaa_1024 -= (0x5f);
				mask1024(ang_aaa_1024);
				#if 1
				/* 描画用角度(下が0度で左回り(反時計回り)) */
				h->rotationCCW1024		= ang_aaa_1024;
				#endif
				h->vx256		= ((sin1024((ang_aaa_1024))*(rnd_spd))>>8);/*fps_factor*/ /* CCWの場合 */
				h->vy256		= ((cos1024((ang_aaa_1024))*(rnd_spd))>>8);/*fps_factor*/
				}
			}
		}
	}
}
/* 十字ショットボムの炎の部分 */
/*		r_or_l	[xxx_r] l=2, [xxx_l] r=0, u=3, d=1	*/
static void remilia_add_burn_shot(SPRITE *src)
{
	{
		int i;
		for (i=0; i<((4*3)-1); i++)
		{
			SPRITE *h;
			h = obj_add_01_teki_error();
			if (NULL!=h)/* 登録できた場合のみ */
			{
				//
				h->m_Hit256R		= JIKI_ATARI_ITEM_16;
//				h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
//				h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL)/*ボスに有効*/;
				h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_01|SP_GROUP_SHOT_SPECIAL)/*ボスに有効*/ /*SP_PL_HLASER*/;
				h->callback_mover	= remilia_move_burn_fire;
//				h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x64);		/*	h->alpha			= 0x64 100;*/
				h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	h->alpha			= 0x64 100;*/
				h->base_weapon_strength 	= (8*(5));/* 仕様変更した */	/*9*/ /*10*/
				#if 1/*Gu(中心座標)*/
				h->cx256			= src->cx256;
				h->cy256			= src->cy256+t256(15);
				#endif
				{
				//	int rnd_spd 		= (ra_nd() & 0xff/*angCCW512*/)+256+((angCCW512+32+64) & 0x40);
				//	int aaa_spd 		= (((angCCW512+32+64) & 0x40));
				//	int rnd_spd 		= (ra_nd() & 0xff/*angCCW512*/)+256+(aaa_spd+aaa_spd);
			//	u32 rnd_spd 		= (ra_nd() & 0xff/*angCCW512*/) | 0x0100;
				#if 1
				/* 描画用角度(下が0度で左回り(反時計回り)) */
//				h->rotationCCW1024		= ang_aaa_1024;
				h->rotationCCW1024		= (0);
				#endif
			//	h->vx256		= (( ((src->vx256))*(rnd_spd))>>8);/*fps_factor*/ /* CCWの場合 */
			//	h->vy256		= (( ((src->vy256))*(rnd_spd))>>8);/*fps_factor*/
				h->vx256		= (src->vx256) ;/*fps_factor*/ /* CCWの場合 */
				h->vy256		= (src->vy256) ;/*fps_factor*/
				u32 rnd_spd 		= (ra_nd() ) ;
				if (rnd_spd&0x01)	{	h->vx256 = - h->vx256;	}
				if (rnd_spd&0x02)	{	h->vy256 = - h->vy256;	}
				}
			}
		}
	}
}

#define BASE_STD_BOMB_STRENGTH		(PLAYERS8*0)
#define BASE_LOW_BOMB_STRENGTH		(PLAYERS8*1)

/*static*/static const u8 player_fix_status[(PLAYERS8*2)/*BASE_MAX*/] =
{/* REIMU(A/B) MARISA(A/B)	REMILIA YUYUKO CIRNO(A/Q) */
//	 /* BASE_SPEED_ANIME	   プレイヤーのアニメーション速度 */
//	  40,  40,	40,  40,   24,	48,   8,   8,	 /* BASE_STD_BOMB_STRENGTH 通常ボムの強さ / standard bomb strength. */
//	 192, 192, 255, 255,   96, 160,  96,  96,	 /* BASE_LOW_BOMB_STRENGTH 低速ボムの強さ / lower bomb strength. */
	#if 0
	  48,  40,	40,  40,   32,	48,   8,   9,	 /* BASE_STD_BOMB_STRENGTH 通常ボムの強さ / standard bomb strength. */
	 160, 192, 240, 255,  127, 160,  96, 127,	 /* BASE_LOW_BOMB_STRENGTH 低速ボムの強さ / lower bomb strength. */
	#else
	  48,  40,	40,  40,   32,	48, 240, 255,	 /* BASE_STD_BOMB_STRENGTH 通常ボムの強さ / standard bomb strength. */
	 160, 192, 240, 255,  127, 160, 240, 255,	 /* BASE_LOW_BOMB_STRENGTH 低速ボムの強さ / lower bomb strength. */
//	 (255), 192, 240, 255,	127, 160,  (244),  96,	 /* BASE_LOW_BOMB_STRENGTH 低速ボムの強さ / lower bomb strength. */
	#endif
//
};
//	  40,	 40,	5*8,	 40,	 24,	6*8,	 1*8,	1*8,	/* BASE_STD_BOMB_STRENGTH 通常ボムの強さ / standard bomb strength. */
//	 30*8,	30*8,  48*8,	48*8,	12*8,  24*8,	12*8,  12*8,	/* BASE_LOW_BOMB_STRENGTH 低速ボムの強さ / lower bomb strength. */

/*
レミリアボム: 通常ボムの強さ:[旧==8] [新==4]
旧 3回==((256/64)-1) [24==(8)*3]
新 7回==((256/32)-1) [21==(3)*7]
REIMU		5,[新==5] [旧==3]
MARISA		5,
REMILIA 	3,[新==3] [旧==8]
YUYUKO		6,[新==6] [旧==4]
CIRNO		1,[新==1] [旧==4]
	通常ボムの強さ / standard bomb strength.
*/
/*---------------------------------------------------------
	霊夢 A 専用 低速ボム追加
	霊符霊夢
	ボム:夢想封印
	ホーミング光弾を7発出す。敵がいない時はしばらくその場に停滞。
	威力は恋符についで高く、無敵時間も長い。光弾の軌跡、炸裂に弾消し。
---------------------------------------------------------*/

static void reimu_add_reifu_musofuuin(SPRITE *src)
{
//	int j;
//	j = (ra_nd()&(2-1));//for (j=0;j<2;j++)
	{
		//#define hlaser_NUM_OF_ENEMIES (24)
	//	int ii;
		static int musou_id;
		musou_id++;
		musou_id &= 0x07;
		{
			SPRITE *h;
			h						= obj_add_01_teki_error();
			if (NULL!=h)/* 登録できた場合のみ */
			{
				h->m_Hit256R		= JIKI_ATARI_ITEM_16;
	//			h->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)/*ボスに有効*/ /*SP_PL_HLASER*/;
				h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_01|SP_GROUP_SHOT_SPECIAL)/*ボスに有効*/ /*SP_PL_HLASER*/;
				#if 1/*Gu(中心座標)*/
			//	h->cx256			= src->cx256 - t256(5);
			//	h->cy256			= src->cy256 + t256(15);
				h->cx256			= src->cx256;
				h->cy256			= src->cy256;
				#endif
			//	h->flags			|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
//				h->flags			&= (~(SP_FLAG_VISIBLE));	/* 非表示 */
	//			if (0==i)
				{
					h->callback_mover	= move_reimu_musou_fuuin;
	//				h->rotationCCW1024	= cv1024r(0);
	//				h->rotationCCW1024	= (0/*j&1*/)?(cv1024r(	0+10)):(cv1024r(360-10));
	//				h->rotationCCW1024	= (i<<6);
					/* 180 ... 0 (1024/128==8) */
					h->rotationCCW1024	= ( (((int)cg.bomber_time)<<3)+(512));/* 射出する方向 */
					#if 0
					/* (多分必要ないけど)念の為マスク */
					mask1024(src->rotationCCW1024/*src->YUUDOU_BOMBER_angleCCW1024*/);
					#endif
					h->YUUDOU_BOMBER_speed256				= t256(1.2);
					h->base_weapon_strength 				= player_fix_status[BASE_LOW_BOMB_STRENGTH+(cg_game_select_player)]/*6*/;/* 低速ボムの強さ */
					h->jyumyou								= byou60(20);	/* 20秒 テキトー */ 	// (100);	/*(200)*/
					h->YUUDOU_BOMBER_reimu_musou_id 		= (musou_id);	/*(ii)(200)*/
					h->target_obj							= &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];			/* h->target_obj == obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];みつからない */
				}
			}
		}
	}
}

/*
add2-------------------------------------------------------------------------
*/
/*---------------------------------------------------------
	プレイヤー誘導弾生成
---------------------------------------------------------*/

static void marisa_add_teisoku_yuudoudan(SPRITE *src)
{
	int j;	j=(ra_nd()&(2-1));//for (j=0;j<2;j++)
	{
		SPRITE *h_old;	h_old = NULL;
		SPRITE *h;		h = NULL;
		//#define hlaser_NUM_OF_ENEMIES (24)
		//#define hlaser_NUM_OF_ENEMIES (12)		/* 半分にしてみる */
		#define hlaser_NUM_OF_ENEMIES (8)			/* 8枚位で十分かも(?) */
		unsigned int i;
		unsigned int tmp_color32;
		tmp_color32 = 0xffffffff;
		for (i=0; i<hlaser_NUM_OF_ENEMIES; i++)
		#undef hlaser_NUM_OF_ENEMIES
		{
			h_old = h;
			h					= obj_add_01_teki_error();
	//	if (NULL!=h)/* 登録できた場合のみ */	/* 強制登録 */
		if (NULL==h)return;/* 登録できた場合のみ */
/* うーん... */
			h->m_Hit256R		= JIKI_ATARI_ITEM_16;
//			h->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)/*ボスに有効*/ /*SP_PL_HLASER*/;
			h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_01|SP_GROUP_SHOT_SPECIAL)/*ボスに有効*/; /*SP_PL_HLASER*/
			#if 1/*Gu(中心座標)*/
		//	h->cx256			= src->cx256 - t256(5);
		//	h->cy256			= src->cy256 + t256(15);
			h->cx256			= src->cx256;
			h->cy256			= src->cy256;
			#endif
		//	h->flags			|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
//			h->flags			&= (~(SP_FLAG_VISIBLE));	/* 非表示 */
//				h->PL_HOMING_KODOMO_DATA_color256		= (t256(1.0));	/* 明るさ(アルファ初期値) */
			h->color32			= tmp_color32;
//			tmp_color32 -= 0x10000000;
//			tmp_color32 -= 0x20000000;
			tmp_color32 -= 0x20202020;
			if (0==i)
			{
				h->callback_mover	= marisa_move_parrent_hlaser;
//				h->rotationCCW1024						= cv1024r(0);
				h->rotationCCW1024						= (j&1)?(cv1024r(  0+10)):(cv1024r(360-10));
				h->YUUDOU_BOMBER_speed256				= t256(4.0);//t256(12.0);
				h->base_weapon_strength 				= player_fix_status[BASE_LOW_BOMB_STRENGTH+(cg_game_select_player)];/*6*/ /* 低速ボムの強さ */
				h->jyumyou								= (HOMING_128_TIME_OUT);	/*100(200)*/
				h->target_obj							= &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];	/* (h->target_obj==obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER])==見つからない */
			}
			else
			{
				h->callback_mover	= marisa_move_kodomo_hlaser/*NULL*/;
				h->target_obj		= h_old;
				h->PL_HOMING_KODOMO_DATA_time_out		= 8;
			}
		}
	}
}

//(r31)
//	 0	1  2  3  4	5  6  7  8	9 10 11 [移動処理順→]
//	↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓
//	 p	0  1  2  3	4  5  6  7	8  9 10
// 結果的に 全部重なるバグ。

//(r32)
//	11	10	9  8  7  6	5  4  3  2	1  0 [移動処理順→]
//	↓	↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓
//	10	 9	8  7  6  5	4  3  2  1	0  p

/* tuika 追加系 ------------------------------------------------------------------------ */

/*---------------------------------------------------------
	(霊夢)夢想封印の追加[ボム追加本体]
---------------------------------------------------------*/

static void reimu_tuika_musofuuin_shot(SPRITE *src)
{
	SPRITE *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	//
//	if (0x3f==(((int)pd_bomber_time)&0x3f))
//	if (0x0f==(((int)pd_bomber_time)&0x0f))
	if (0x0f==((src->jyumyou)&0x0f))/* 波動を撃つタイミング */
	{
		/* シナリオ時には誘導弾を追加しない */
		if (0==(cg.state_flag & STATE_FLAG_06_IS_SCRIPT))
		{
			reimu_add_reifu_musofuuin(zzz_player);/* 波動発動(早口言葉?) */
		}
	}
	#if 1/*Gu(中心座標)*/
	src->cx256 = zzz_player->cx256;
	src->cy256 = zzz_player->cy256;
	#endif
/* 時間制限でおしまい */	//if (0 >= cg.bomber_time)	{ src->jyumyou = JYUMYOU_NASI;}/* 時間制限でおしまい */
}

/*---------------------------------------------------------
	魔理沙、チルノ等、低速ボムの追加[ボム追加本体]
---------------------------------------------------------*/
static void marisa_tuika_bomber_teisoku_yuudoudan(SPRITE *src)
{
	SPRITE *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	//
//	if (0==(((int)pd_bomber_time)&0x3f))
//	if (0==(((int)pd_bomber_time)&0x0f))/*(r31)*/
	if (0==((src->jyumyou)&0x07))/*(r32)追加量を2倍にしてみる。*/
	{
		/* シナリオ時には誘導弾を追加しない */
		if (0==(cg.state_flag & STATE_FLAG_06_IS_SCRIPT))
		{
			marisa_add_teisoku_yuudoudan(zzz_player);
		}
	}
/* 時間制限でおしまい */	//	if (0 >= cg.bomber_time)	{	src->jyumyou = JYUMYOU_NASI;}/* 時間制限でおしまい */
}

/*---------------------------------------------------------
	レミリアボムの十字ショットの追加[ボム追加本体]
---------------------------------------------------------*/

static void remilia_tuika_cross_shot(SPRITE *src)
{
//	if (0x3f==(((int)pd_bomber_time)&0x3f))
//	if (0x0f==(((int)pd_bomber_time)&0x0f))
//	if (0x0f==(((int)pd_bomber_time)&0x0f))/* 波動を撃つタイミング */
	if (0x07==((src->jyumyou)&0x07))/* 波動を撃つタイミング */
	{
		/* シナリオ時には誘導弾を追加しない */
		if (0==(cg.state_flag & STATE_FLAG_06_IS_SCRIPT))
		{
			remilia_add_burn_shot(src);/* 波動発動(早口言葉?) */
		}
	}
	#if 1/*Gu(中心座標)*/
	/* 親移動 */
	src->cx256 += src->vx256;
	src->cy256 += src->vy256;
	#endif
/* 時間制限でおしまい */	//	if (0 >= cg.bomber_time)	{ src->jyumyou = JYUMYOU_NASI;}
}

/*---------------------------------------------------------
	(幽々子低速)レミリアボムの十字炎の追加[ボム追加本体]
---------------------------------------------------------*/

static void yuyuko_tuika_meifu(SPRITE *src)
{
	SPRITE *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	//
//	if (0x3f==(((int)pd_bomber_time)&0x3f))
//	if (0x0f==(((int)pd_bomber_time)&0x0f))
	if (0x0f==((src->jyumyou)&0x0f))/* 波動を撃つタイミング */
	{
		/* シナリオ時には誘導弾を追加しない */
		if (0==(cg.state_flag & STATE_FLAG_06_IS_SCRIPT))
		{
			yuyuko_add_meifu(zzz_player);/* 波動発動(早口言葉?) */
		}
	}
	#if 1/*Gu(中心座標)*/
	src->cx256 = zzz_player->cx256;
	src->cy256 = zzz_player->cy256;
	#endif
/* 時間制限でおしまい */	//	if (0 >= cg.bomber_time)	{ src->jyumyou = JYUMYOU_NASI;}/* 時間制限でおしまい */
}



/*
parrent 親系 -------------------------------------------------------------------------
*/
/*---------------------------------------------------------
	プレイヤーシールド生成の親(霊夢)
---------------------------------------------------------*/

/*static*/global void reimu_create_bomber_kekkai_parrent(SPRITE *src)		/* 霊夢 */		//シールドの追加
{
	cg.state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/* ボムによる自動収集可能 */
	int ii;//	int i;	/* 半象限ずつ */
	for (ii=0; ii<(8); ii++)//	for (i=0; i<360; i+=45)
	{
		SPRITE *h;
		h				= obj_add_01_teki_error();
	//	if (NULL!=h)/* 登録できた場合のみ */
		if (NULL==h)return;/* 登録できた場合のみ */
		{
			h->m_Hit256R			= TAMA_ATARI_04;
			if (0 == (ii&1))// if (i%90 == 0)
			{
				h->PL_KEKKAI_DATA_add_r1024 	= ( 46);							/* 時計回り */
				h->PL_KEKKAI_DATA_radius		= (16); 	/*(48)*/	//d->radius=38; 	/* 半径初期値 */	/* 少しずつ広がるようにした */
			}
			else		/* 1つだと不安なので2つ作った。 */
			{
				h->PL_KEKKAI_DATA_add_r1024 	= (-34);							/* 反時計回り */
				h->PL_KEKKAI_DATA_radius		= (0);		/*(45)*/	//d->radius=35; 	/* 半径初期値 */	/* 少しずつ広がるようにした */
			}
			/* 霊夢 */
			h->type 				= (ii&(4-1))+(/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL);/* ボスに有効 */
			h->callback_mover			= reimu_move_rotate_kekkai; 		/*	時計回り ＆ 反時計回り ＆ スクエア結界(霊夢B) 兼用 */
//			h->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
			h->PL_KEKKAI_DATA_angleCCW1024		= (ii<<7);	/* (1024/8) */
			/* 出始めはプレイヤー座標 */
			{
				SPRITE *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
				h->cx256					= zzz_player->cx256;
				h->cy256					= zzz_player->cy256;
			}
			h->base_weapon_strength 	= (8*1);	/*5*/
		}
	}
}
/*---------------------------------------------------------
	マスタースパーク、ボム生成の親
	(魔理沙B 専用)
---------------------------------------------------------*/

/*static*/global void marisa_create_bomber_master_spark_parrent(SPRITE *src)
{
	cg.state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/
	int iii;
	for (iii=0; iii<(3)/*16*/; iii++)
	{
		SPRITE *h;
		h					= obj_add_01_teki_error();
	//	if (NULL!=h)/* 登録できた場合のみ */
		if (NULL==h)return;/* 登録できた場合のみ */ 	/*うーん？？？*/
		{
			const static u32 color_mask32[(4)] =
			{	//		   RED	 GREEN BLUE  ALPHA
				MAKE32RGBA(0xff, 0x00, 0x00, 0x3f), 	// (赤)
				MAKE32RGBA(0x00, 0xff, 0x00, 0x3f), 	// (緑)
				MAKE32RGBA(0x00, 0x00, 0xff, 0x3f), 	// (青)
				MAKE32RGBA(0xff, 0xff, 0xff, 0x3f), 	// (白)
			};
			h->color32					= (color_mask32[iii]);
			h->m_Hit256R				= ((128)<<8);
			h->type 					= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL);/* ボスに有効 */
//			h->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
			h->callback_mover			= marisa_move_master_spark;
			h->tmp_angleCCW1024 		= ((iii)<<8);
			h->m_zoom_x256				= (256*4);	/* 描画用ボムの初期サイズ / size of bomb at first. */
			h->m_zoom_y256				= (256*4);	/* 描画用ボムの初期サイズ / size of bomb at first. */
			h->base_weapon_strength 	= player_fix_status[BASE_STD_BOMB_STRENGTH+(cg_game_select_player)];

			h->vx256					= (0);	/*fps_factor*/	/* CCWの場合 */
			h->vy256					= (0);	/*fps_factor*/
			#if 1/* 省略可能かも？ */
			h->cx256					= src->cx256;
			h->cy256					= src->cy256-t256(MASKER_SPARK_LENGTH144);
			#endif
//			/* 描画用角度(下が0度で左回り(反時計回り)) */
//			h->rotationCCW1024		= (0);
		}
	}
}

/*---------------------------------------------------------
	ギャストリドリーム
	プレイヤーボム生成の親
	(幽々子 ＆ チルノ)
---------------------------------------------------------*/

/*static*/global void yuyuko_create_bomber_gyastry_dream_parrent(SPRITE *src)
{
	cg.state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/
	int jjj;		jjj=0;
	int angCCW1024; angCCW1024 = (0);/* 0 チルノ用(暫定的)*/
	int iii;
	for (iii=0; iii<(18*3)/*16*/; iii++)
	{
		SPRITE *h;
		h					= obj_add_01_teki_error();
	//
	//	if (NULL!=h)/* 登録できた場合のみ */
		if (NULL==h)return;/* 登録できた場合のみ */ 	/*うーん？？？*/
		{
			const static u8 base_bomber_atari[8] =
			{
				  (2),	//	BASE_BOMBER_ATARI_0a,/*4*/		/* 霊夢A */ 	/*	5 */	/*0,*/	 /* re a */  //  BASE_BOMBER_ATARI_0a,/*4*/
				  (2),	//	BASE_BOMBER_ATARI_0b,/*4*/		/* 霊夢B */ 	/*	5 */	/*0,*/	 /* re b */  //  BASE_BOMBER_ATARI_0a,/*4*/
				 (16),	//	BASE_BOMBER_ATARI_0c,/*16*/ 	/* 魔理沙A */	/*	6 */	/*0,*/	 /* ma a */  //  BASE_BOMBER_ATARI_0b,/*16*/
				 (16),	//	BASE_BOMBER_ATARI_0d,/*16*/ 	/* 魔理沙B */	/*	6 */	/*0,*/	 /* ma b */  //  BASE_BOMBER_ATARI_0b,/*16*/
				 (16),	//	BASE_BOMBER_ATARI_0e,/*16*/ 	/* 魔理沙C */	/*	7 */	/*0,*/	 /* oz / */  //  BASE_BOMBER_ATARI_0c,/*16*/
				  (4),	//	BASE_BOMBER_ATARI_0f,/*16*/ 	/* レミリア */	/*	9 */	/*0,*/	 /* yu / */  //  BASE_BOMBER_ATARI_0e,/*4*/
				 (32),	//	BASE_BOMBER_ATARI_0g,/*32*/ 	/* チルノ */	/*	8 */	/*0,*/	 /* ci a */  //  BASE_BOMBER_ATARI_0d,/*32*/
				 (32),	//	BASE_BOMBER_ATARI_0h,/*4*/		/* 幽々子 */	/*	6 */	/*0,*/	 /* ci q */  //  BASE_BOMBER_ATARI_0b,/*16*/
			};
			h->m_Hit256R			= ((base_bomber_atari[(cg_game_select_player)])<<8);
			//
			jjj++;
			jjj &= 0x03;	//	if (4==jjj) 	{	jjj=0;	}
			h->type 		= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL);/* ボスに有効 */
			h->type += jjj;
	//

//			h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
			h->callback_mover	= marisa_yuyuko_move_levarie_gyastry_dream;
			#if 0
			{
				static const u16 player_fix_status_ggg[8] =
				{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
					256, 256, 256, 256, 32, 256, 32, 32,	/* 描画用ボムの初期サイズ / size of bomb at first. */
				};
				h->m_zoom_x256		= player_fix_status_ggg[(cg_game_select_player)];/*8*/ /* 64 == (1/4) */
			}
			#else
			if (YUYUKO!=(cg_game_select_player))
			{
				h->m_zoom_x256		= 1+(ra_nd()&(64-1));	/* 描画用ボムの初期サイズ / size of bomb at first. */
				h->m_zoom_y256		= 1+(ra_nd()&(64-1));	/* 描画用ボムの初期サイズ / size of bomb at first. */
			}
			#endif

			h->base_weapon_strength 	= player_fix_status[BASE_STD_BOMB_STRENGTH+(cg_game_select_player)];
			/* MARISAは強いが拡散してしまうのであたらない。 */
			/* チルノは当てやすいのでその分弱くしないと強すぎちゃう。 */
			int d_speed256;
			#if 0
			if (0==(iii&(2-1)/*%2*/))
			{			/*d->*/d_speed256	= t256(1.0);	c->alpha	= 150;	}
			else	{	/*d->*/d_speed256	= t256(1.2);	c->alpha	= 180;	}
			#else
			/*d->*/d_speed256	= t256(1.0)+(iii<<3);
			#endif
			h->vx256			= ((sin1024((angCCW1024))*(d_speed256))>>8);/*fps_factor*/	/* CCWの場合 */
			h->vy256			= ((cos1024((angCCW1024))*(d_speed256))>>8);/*fps_factor*/
	//
			#if 1/*Gu(中心座標)*/
			h->cx256			= src->cx256;
			h->cy256			= src->cy256;
			#endif
			/* ギャストリドリーム(もどき)。スターダストレヴァリエと違い近接性能は無い。 */
			if (YUYUKO==(cg_game_select_player))	/* 幽々子 */
			{
				h->cx256		+= ((h->vx256)<<5);
				h->cy256		+= ((h->vy256)<<5);
	//			/* 幽々子 強すぎるからな～(効果弱いが効果あるのも出来るようにシステム見直したほうがいいかも) */
			}
			#if 1
			/* 描画用角度(下が0度で左回り(反時計回り)) */
			h->rotationCCW1024		= angCCW1024;
			#endif
		//	angCCW1024 += (int)((1024)/16); // 22.5度/360, 2xπ/16 /* 0.392699081698724154810 */
			angCCW1024 += (int)((1024)/18); // 20.0度/360, 2xπ/18 /* 0.349065850398865915384 */
		}
	}
}


/*---------------------------------------------------------
	魔符「スターダストレヴァリエ」
	-------------------------------------------------------
	[プレイヤーシールド]ボム生成の親(魔理沙 A)
	-------------------------------------------------------
	スターダストレヴァリエ(3x8==計24個星が出る)
	は８方向に星が出る。星の色は、決まっている。

		左上	上		右上
		(赤0)	(青2)	(緑1)

		左				右
		(緑1)			(青2)

		左下	下		右下
		(青2)	(赤0)	(緑1)

	赤==0
	緑==1
	青==2

	-------------------------------------------------------
	一方向では、３つ星が出る。画像の左上座標を(x,y)とすると、

	大星=半透明、座標(x,y)
	中星=半透明、座標(x,y)
	小星=不透明、座標(x+64,y+64)	// 64 は テキトーな一定値。
									// pspの場合は、画面が狭いので48が妥当？
	---------------------------------------------------------*/

/*static*/global void marisa_create_bomber_star_dust_levarie_parrent(SPRITE *src)
{
	cg.state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/
//
	int angCCW1024; angCCW1024 = (0);/* 0 チルノ用(暫定的)*/
	int iii;
	for (iii=0; iii<(8)/*16*/; iii++)/*８方向*/
	{
		int kkk;
		for (kkk=0; kkk<(3)/*16*/; kkk++)/*3サイズ*/
		{
			SPRITE *h;
			h					= obj_add_01_teki_error();
		//	if (NULL!=h)/* 登録できた場合のみ */
			if (NULL==h)return;/* 登録できた場合のみ */ 	/*うーん？？？*/
			{
			//	h->m_Hit256R			= ((16)<<8);//((base_bomber_atari[(cg_game_select_player)])<<8);
				h->m_Hit256R			= ((32)<<8);//((base_bomber_atari[(cg_game_select_player)])<<8);
				const static u32 color_mask32[8] =
				{	//		   RED	 GREEN BLUE  ALPHA
					MAKE32RGBA(0xff, 0x00, 0x00, 0x7f), 	//	  下(赤0)
					MAKE32RGBA(0x00, 0xff, 0x00, 0x7f), 	//	右下(緑1)
					MAKE32RGBA(0x00, 0x00, 0xff, 0x7f), 	//	右	(青2)
					MAKE32RGBA(0x00, 0xff, 0x00, 0x7f), 	//	右上(緑1)
					MAKE32RGBA(0x00, 0x00, 0xff, 0x7f), 	//	  上(青2)
					MAKE32RGBA(0xff, 0x00, 0x00, 0x7f), 	//	左上(赤0)
					MAKE32RGBA(0x00, 0xff, 0x00, 0x7f), 	//	左	(緑1)
					MAKE32RGBA(0x00, 0x00, 0xff, 0x7f), 	//	左下(青2)
				};
				h->color32					= (color_mask32[iii]);
				h->type 					= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL);/*ボスに有効*/
		//
	//			h->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
				h->callback_mover			= marisa_yuyuko_move_levarie_gyastry_dream;
				h->m_zoom_x256				= (0x100<<(kkk));	/* 描画用ボムの初期サイズ / size of bomb at first. */
				h->m_zoom_y256				= (0x100<<(kkk));	/* 描画用ボムの初期サイズ / size of bomb at first. */
				h->base_weapon_strength 	= player_fix_status[BASE_STD_BOMB_STRENGTH+(cg_game_select_player)];
				{
//					const int d_speed256	= t256(1.5);	/* 速度一定 */
					const int d_speed256	= t256(1.5);	/* 速度一定 */
					h->vx256			= ((sin1024((angCCW1024))*(d_speed256))>>8);/*fps_factor*/	/* CCWの場合 */
					h->vy256			= ((cos1024((angCCW1024))*(d_speed256))>>8);/*fps_factor*/
				}//
				#if 1/*Gu(中心座標)*/
				{
				//	const int aaa111	= ((0==kkk)?(t256(48)):(0));	/* 速度一定 */
					h->cx256			= src->cx256;//+(aaa111);
					h->cy256			= src->cy256;//+(aaa111);
				}
				if (0==kkk)
				{
					h->cx256			+= (t256(48));
					h->cy256			+= (t256(48));
					h->color32			|= (MAKE32RGBA(0, 0, 0, 0xff)); 	/*	h->alpha			= 0xff 255;*/
				}
				#endif
				#if 1
				/* 描画用角度(下が0度で左回り(反時計回り)) */
				h->rotationCCW1024		= angCCW1024;
				#endif
			}
		}
		angCCW1024 += (int)((1024)/8);	// 45.0度/360, 64
	}
}

/*---------------------------------------------------------
	プレイヤーシールド生成(レミリア)の親[レミリアボムの十字ショット本体]
---------------------------------------------------------*/

/*static*/global void remilia_create_bomber_cross_shot_parrent(SPRITE *src)
{
	cg.state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/
	SPRITE *h;
	h						= obj_add_01_teki_error();
	if (NULL!=h)/* 登録できた場合のみ */	/* うーん... */
	{
		h->jyumyou			= (PD_BOMBER_JYUMYOU180);
//		h->m_Hit256R		= JIKI_ATARI_ITEM_16;
//		h->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
//		h->flags			&= (~(SP_FLAG_VISIBLE));	/* 非表示 */
//		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0xdc);		/*	h->alpha			= 0xdc;*/	/* 非表示 */
		h->callback_mover	= remilia_tuika_cross_shot;
		h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_01|SP_GROUP_SHOT_SPECIAL);/* ボスに有効 */
	//	#if 1/*Gu(中心座標)*/
	//	h->cx256			= t256(GAME_NOT_VISIBLE480);//(src->cx256); /* 非表示 */
//	//	h->cy256			= (src->cy256); /* 非表示 */
	//	#endif
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		#if 1/*Gu(中心座標)*/
		/*開始時の座標*/
		h->cx256 = zzz_player->cx256;
		h->cy256 = zzz_player->cy256;
		#endif
		{
//			SPRITE *obj_boss;
//			obj_boss = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
			int int_angle1024;
			int int_aaa;
		//	int_angle1024 = at an_1024(global_obj_boss->cy256-zzz_player->cy256, global_obj_boss->cx256-zzz_player->cx256);
			int_angle1024 = atan_65536(global_obj_boss->cy256-zzz_player->cy256, global_obj_boss->cx256-zzz_player->cx256);
			int_angle1024 >>= (6);
			int_aaa = sin1024((int_angle1024)); 	h->vx256	 = int_aaa+int_aaa;/*fps_factor*/ /* CCWの場合 */
			int_aaa = cos1024((int_angle1024)); 	h->vy256	 = int_aaa+int_aaa;/*fps_factor*/
			#if 1
			/* 描画用グラ回転 */
		//	h->rotationCCW1024	= int_angle1024;
			/* 描画用グラ回転(ショットは上が正面なので、回転角を計算)[180/360度]回す。 */
			int_angle1024 += cv1024r((180));
			mask1024(int_angle1024);
			h->rotationCCW1024	= int_angle1024;/* 下が正面の角度が描画用角度 */
			#endif
		}
	}
}

/*---------------------------------------------------------
	魔理沙、チルノ等、低速ボムの親
---------------------------------------------------------*/
/*static*/global void marisa_create_bomber_homing_parrent(SPRITE *src)
{
	cg.state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/* ボムによる自動収集可能 */
//
	SPRITE *h;
	h						= obj_add_01_teki_error();
	if (NULL!=h)/* 登録できた場合のみ */	/* うーん... */
	{
		h->jyumyou			= (PD_BOMBER_JYUMYOU180);
//		h->m_Hit256R		= JIKI_ATARI_ITEM_16;
	//	h->flags			|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
//		h->flags			&= (~(SP_FLAG_VISIBLE));	/* 非表示 */
//		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0xdc);	/*	c->alpha			= 0x80 0xdc;*/	/* 非表示 */
		h->callback_mover	= marisa_tuika_bomber_teisoku_yuudoudan;
		h->type 			= (/*表示しない*/SP_DUMMY_MUTEKI/*S P_GROUP_ETC_DUMMY_SLOW_BOMB*/ /*SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL*/);/*ボスに有効*/	/* 低速ボムもボスに有効とする */
		#if 1/*Gu(中心座標)*/
		h->cx256			= t256(GAME_NOT_VISIBLE480);//src->cx256;	/* 非表示 */
//		h->cy256			= src->cy256;	/* 非表示 */
		#endif
//
		#if 1
		/* ボム中＆復活中、有効にすると、重複する。 */
		/* 特殊機能 */
		bullets_to_hosi();/* 弾全部、星アイテムにする */
		#endif
	}
}
/*---------------------------------------------------------
	霊夢 A 専用 低速ボムの親
---------------------------------------------------------*/
/*static*/global void reimu_create_bomber_homing_shot_parrent(SPRITE *src)	/* reimu_move_add_bomber_hlaser */
{
	cg.state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/* ボムによる自動収集可能 */
	SPRITE *h;
	h						= obj_add_01_teki_error();
	if (NULL!=h)/* 登録できた場合のみ */	/* うーん... */
	{
		h->jyumyou			= (PD_BOMBER_JYUMYOU180);
//		h->m_Hit256R		= JIKI_ATARI_ITEM_16;
//		h->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
//		h->flags			&= (~(SP_FLAG_VISIBLE));	/* 非表示 */
//		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0xdc);		/*	h->alpha			= 0xdc;*/	/* 非表示 */
		h->callback_mover	= reimu_tuika_musofuuin_shot;
		h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_01|SP_GROUP_SHOT_SPECIAL);/* ボスに有効 */
	//	#if 1/*Gu(中心座標)*/
	//	h->cx256			= t256(GAME_NOT_VISIBLE480);//(src->cx256); /* 非表示 */
//	//	h->cy256			= (src->cy256); /* 非表示 */
	//	#endif
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		#if 1/*Gu(中心座標)*/
		/*開始時の座標*/
		h->cx256 = zzz_player->cx256;
		h->cy256 = zzz_player->cy256;
		#endif
		{
//			SPRITE *obj_boss;
//			obj_boss = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
			#if 1
			/* 描画用グラ回転 */
		//	h->rotationCCW1024	= int_angle1024;
			/* 描画用グラ回転(ショットは上が正面なので、回転角を計算)[180/360度]回す。 */
			h->rotationCCW1024	= 0;/* 下が正面の角度が描画用角度 */
			#endif
		}
	}
	#if 0
	/* ボム中＆復活中、有効にすると、重複する。 */
	/* 特殊機能 */
	bullets_to_hosi();/* 弾全部、星アイテムにする */
	#endif
}
/*---------------------------------------------------------
	幽々子専用 低速ボムの親 	[(幽々子低速)ボムの十字炎本体]
---------------------------------------------------------*/
/*static*/global void yuyuko_create_bomber_meifu_parrent(SPRITE *src)
{
	cg.state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/* ボムによる自動収集可能 */
	SPRITE *h;
	h						= obj_add_01_teki_error();
	if (NULL!=h)/* 登録できた場合のみ */	/* うーん... */
	{
		h->jyumyou			= (PD_BOMBER_JYUMYOU180);
//		h->m_Hit256R		= JIKI_ATARI_ITEM_16;
//		h->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
//		h->flags			&= (~(SP_FLAG_VISIBLE));	/* 非表示 */
//		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0xdc);		/*	h->alpha			= 0xdc;*/	/* 非表示 */
		h->callback_mover	= yuyuko_tuika_meifu;
		h->type 			= (/*表示しない*/SP_DUMMY_MUTEKI/*S P_GROUP_ETC_DUMMY_REMILIA*/ /*SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO*/); /* ボスの直接攻撃は禁止 */
		#if 1/*Gu(中心座標)*/
		h->cx256			= t256(GAME_NOT_VISIBLE480);//(src->cx256); /* 非表示 */
//		h->cy256			= (src->cy256); /* 非表示 */
		#endif
	}
}
