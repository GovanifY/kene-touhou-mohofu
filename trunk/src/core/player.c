
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	各プレイヤー(霊夢 ＆ 魔理沙 ＆ レミリア ＆ チルノ ＆ 幽々子)
	REIMU(A/B) MARISA(A/B/C) REMILIA CIRNO YUYUKO
	-------------------------------------------------------
---------------------------------------------------------*/

#include "kanji_system.h"

extern int select_player;

enum	/* _player_option_type_ */
{
	OPTION_C1 = 0,
	OPTION_C2,
	OPTION_C3,
	OPTION_C4,
};
/*---------------------------------------------------------
	共通オブジェクト
---------------------------------------------------------*/
#if 1

global SPRITE *obj_player;
static SPRITE *obj_maru;		/* ○ */

static SPRITE *option[4];

global SPRITE *obj_boss;

global SPRITE *obj_send1;

#endif


global int player_now_stage;	/* */
global int pd_bomber_time;		/* Use Gu */

//
global int pd_state_flag;		/* 設定フラグ */	// [***090116		追加
global int pd_weapon_power; 	/* 0x00-0x80  (0-128 の129段階==本家と同じ)   max==128==「129段階」*/
global int pd_chain_point;
//
global u32 pd_my_score; 		/* スコア得点 */
global u32 pd_graze_point;		/* グレイズ得点 */
global int pd_bombs;			/* ボム数 */
global int pd_zanki;			/* 残りチャンス */
//
global int pd_use_continue; 	/* コンティニュー回数 */
global int pd_count_miss;		/* ミス回数 */
global int pd_used_bomber;		/* ボム使用回数 */
//obal int pd_count_bonus;		/* スペルカードボーナス回数 */
//
global int pd_use_kurai_bomb;	/* 喰らいボム使用回数 */


enum /*_player_state_*/
{
	PLAYER_STATE_00_NORMAL,
	PLAYER_STATE_01_HIT_BOMB,	/* [***090125		追加:PLAYER_STATE_01_HIT_BOMB */
	PLAYER_STATE_02_SAVE_01,
	PLAYER_STATE_03_SAVE_02,
	PLAYER_STATE_04_GAME_OUT,
};

//typedef struct
//{
//	int speed256;
//	int state;
//	int time_out;			/* 設定時間内に見つけられない場合はおしまい */
//	SPRITE *target_obj; 	/* int target_id */
//} PL_HOMING_DATA; 		/* == PL_HLASER_DATA */

#define PL_HOMING_DATA_speed256 				user_data00
#define PL_HOMING_DATA_state					user_data01
#define PL_HOMING_DATA_time_out 				user_data02 		/* 設定時間内に見つけられない場合はおしまい */

//	int angleCCW1024;
//	int nr; 				/* 右か左(ホーミング), nr==number */


//typedef struct
//{
#define PL_HOMING_KODOMO_DATA_check_x256		user_data00
#define PL_HOMING_KODOMO_DATA_check_y256		user_data01
#define PL_HOMING_KODOMO_DATA_time_out			user_data02 		/* 設定時間内に見つけられない場合はおしまい */
//	SPRITE *target_obj; 	/* int target_id */
//} PL_HOMING_KODOMO_DATA;	/* == PL_HLASER_DATA */

//typedef struct
//{
#define PL_KEKKAI_DATA_angleCCW1024 			user_data00
#define PL_KEKKAI_DATA_add_r1024				user_data01
#define PL_KEKKAI_DATA_radius					user_data02 		/* 半径(radius) */
//} PL_KEKKAI_DATA;

//typedef struct
//{
#define PL_OPTION_DATA_angleCCW1024 			user_data00 		/* 下が０として左回り(逆回り)の角度 */
#define PL_OPTION_DATA_anime_wait				user_data01 		/* アニメーション速度のカウンタ */
#define PL_OPTION_DATA_opt_anime_add_id 		user_data02 		/* アニメーション方向の加算値 / オプションID / (レミリア & チルノ)x offset */	/* pos */
#define PL_OPTION_DATA_opt_shot_interval		user_data03 		/* オプションが発射するショットの更新間隔 */
//
	#if 1
	/* レミリア & チルノ */
//	int st ate1;					/* 共通なので、ここには要らない */
#define PL_OPTION_DATA_state2					user_data04
#define PL_OPTION_DATA_offset_x256				user_data05 		/* 円の半径と兼用 */
#define PL_OPTION_DATA_offset_y256				user_data06
#define PL_OPTION_DATA_slow_count				user_data07 		/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
	#endif
//} PL_OPTION_DATA;

#define OPTION_ID_01_LL 	(0x1dff)/*旧-1 0  c 1 1101 (0x01ff) 0001 1111 1111 */
#define OPTION_ID_02_LM 	(0x1cff)/*旧-1 0  c 1 1100 (0x02ff) 0010 1111 1111 */
#define OPTION_ID_03_RM 	(0x1401)/*旧+1 1  4 1 0100 (0x0401) 0100 0000 0001 */
#define OPTION_ID_04_RR 	(0x1501)/*旧+1 1  4 1 0101 (0x0701) 0111 0000 0001 */

/*
	0	2	4	6	8	a	c	e
		   10  11  00  01  10  11
		   -4				4
*/
	/*---------------------------------------------------------
		各プレイヤーの性能差
	---------------------------------------------------------*/
/* 霊夢 特殊能力：喰らいボムの受付時間が長い */
/* チルノ 特殊能力：⑨ */

#define PLAYERS8					(8)/*5*/

#define BASE_OPT_SHOT_INTERVAL0 	(PLAYERS8*0)
#define BASE_OPT_SHOT_INTERVAL1 	(PLAYERS8*1)
#define BASE_OPT_SHOT_INTERVAL2 	(PLAYERS8*2)
#define BASE_OPT_SHOT_INTERVAL3 	(PLAYERS8*3)
#define BASE_OPT_SHOT_INTERVAL4 	(PLAYERS8*4)
#define BASE_OPT_SHOT_INTERVAL5 	(PLAYERS8*5)
//
#define BASE_SPEED_ANIME			(PLAYERS8*6)
#define BASE_HIT_BOMB_WAIT			(PLAYERS8*7)
#define BASE_STD_BOMB_STRENGTH		(PLAYERS8*8)
#define BASE_LOW_BOMB_STRENGTH		(PLAYERS8*9)
//
#define BASE_OPT_ANIM_TYPE			(PLAYERS8*10)
#define BASE_OPT_ANIM_SPEED 		(PLAYERS8*11)
//
#define BASE_MAX					(PLAYERS8*12)		/* 最大数 */

static const u8 player_fix_status[BASE_MAX] =
{/* REIMU(A/B) MARISA(A/B)	REMILIA YUYUKO CIRNO(A/Q) */
//
//	  14,  13,	11,   8,	8,	31,   8,   9,	 /* WEAPON_L0 / BASE_OPT_SHOT_INTERVAL オプションショットの更新間隔 / option shot interval. */
//	  12,  11,	10,   8,	8,	28,   8,   9,	 /* WEAPON_L1 / BASE_OPT_SHOT_INTERVAL オプションショットの更新間隔 / option shot interval. */
//	  10,	9,	 9,   8,	8,	25,   8,   9,	 /* WEAPON_L2 / BASE_OPT_SHOT_INTERVAL オプションショットの更新間隔 / option shot interval. */
//	   8,	7,	 8,   8,	8,	22,   8,   9,	 /* WEAPON_L3 / BASE_OPT_SHOT_INTERVAL オプションショットの更新間隔 / option shot interval. */
//	   6,	5,	 7,   8,	8,	19,   8,   9,	 /* WEAPON_L4 / BASE_OPT_SHOT_INTERVAL オプションショットの更新間隔 / option shot interval. */
//	   4,	3,	 6,   8,	8,	16,   8,   9,	 /* WEAPON_L5 / BASE_OPT_SHOT_INTERVAL オプションショットの更新間隔 / option shot interval. */
//
	  14,  13,	11,   4,	8,	31,   8,   9,	 /* WEAPON_L0 / BASE_OPT_SHOT_INTERVAL オプションショットの更新間隔 / option shot interval. */
	  12,  11,	10,   4,	8,	28,   8,   9,	 /* WEAPON_L1 / BASE_OPT_SHOT_INTERVAL オプションショットの更新間隔 / option shot interval. */
	  10,	9,	 9,   4,	8,	25,   8,   9,	 /* WEAPON_L2 / BASE_OPT_SHOT_INTERVAL オプションショットの更新間隔 / option shot interval. */
	   8,	7,	 8,   4,	8,	22,   8,   9,	 /* WEAPON_L3 / BASE_OPT_SHOT_INTERVAL オプションショットの更新間隔 / option shot interval. */
	   6,	5,	 7,   4,	8,	19,   8,   9,	 /* WEAPON_L4 / BASE_OPT_SHOT_INTERVAL オプションショットの更新間隔 / option shot interval. */
	   4,	3,	 6,   3,	8,	16,   8,   9,	 /* WEAPON_L5 / BASE_OPT_SHOT_INTERVAL オプションショットの更新間隔 / option shot interval. */
//
	   8,	8,	 2,   2,	3,	 4,   1,   1,	 /* BASE_SPEED_ANIME	   プレイヤーのアニメーション速度 */
	  16,  16,	 8,   8,	4,	12,   9,   9,	 /* BASE_HIT_BOMB_WAIT	   喰らいボムの受付時間 / hit_bomb_wait. */
	  40,  40,	40,  40,   24,	48,   8,   8,	 /* BASE_STD_BOMB_STRENGTH 通常ボムの強さ / standard bomb strength. */
	 192, 192, 255, 255,   96, 160,  96,  96,	 /* BASE_LOW_BOMB_STRENGTH 低速ボムの強さ / lower bomb strength. */
//
	   0,	0,	 0,   0,	0,	 1,   1,   1,	 /* BASE_OPT_ANIM_TYPE	   0==回転, 1==パターン */
	   2,	2,	 2,   2,	2,	12,   3,   3,	 /* BASE_OPT_ANIM_SPEED    回転/パターン速度 */
};
//	  5*8,	 5*8,	5*8,	 5*8,	 3*8,	6*8,	 1*8,	1*8,	/* BASE_STD_BOMB_STRENGTH 通常ボムの強さ / standard bomb strength. */
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

static int pd_player_status;
static int pd_save_timer;

/*---------------------------------------------------------
	プレイヤー武器のあたり判定
---------------------------------------------------------*/

enum /*_player_state_*/
{
	PLAYER_WEAPON_TYPE_00_SHOT = 0, 	/* ショット系 */	/* 自弾が敵にあたった場合に自弾が消滅する */	/* 敵弾にあたると敵弾を消さない */
	PLAYER_WEAPON_TYPE_01_BOMB, 		/* ボム系 */		/* 自弾が敵にあたった場合に自弾が消滅しない */	/* 敵弾にあたると敵弾を消す */
};

static void player_weapon_colision_check(SPRITE *shot, int erase_shot_type)
{
//	SPRITE *shot;	/* 自弾 */
	SPRITE *tt; 	/* 一時使用のテンポラリ(敵スプライト、または、敵弾スプライト) */
	#define teki_obj		tt
	#define tekidan_obj 	tt
	/* 敵弾にあたった場合に敵弾を消す(ボム系のみ) */
	if (/*PLAYER_WEAPON_TYPE_01_BOMB==*/erase_shot_type/*erase_bullets*/)	/* ボム系のみ */
	{
		/* 自弾にあたったのは敵弾なのか調べる． */
		tekidan_obj = sprite_collision_check_444(shot, SP_GROUP_BULLETS);/* 弾幕専用(弾幕) */
		if (NULL != tekidan_obj)		/* 敵弾に当たったら */
		{
			tekidan_obj->type = SP_DELETE;	/* 敵弾が消滅 */
			voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
		}
	}
	/* 敵にあたった場合に敵を消す */
	{
		/* 自弾にあたったのは敵自体なのか調べる． */
		teki_obj = sprite_collision_check_SDL_teki(shot);//, (SP_GROUP_TEKI));	/*|SP_GROUP_BOSS*/	/*SP_GROUP_ENEMYS*/
		if (NULL != teki_obj)			/* 敵自体に当たったら */
		{
			if (NULL != (teki_obj->callback_hit_enemy)) 	/*	*/
			{
				(teki_obj->callback_hit_enemy)(teki_obj/*敵自体*/, shot/*自弾*/);
			}
			/* 自弾が敵にあたった場合に自弾が消滅する */
			if (/*erase_player_tama*/PLAYER_WEAPON_TYPE_00_SHOT == erase_shot_type) 	/* ショット系のみ */
			{
			//	/* 自弾消滅方法が特殊な場合 */
			//	if (NULL != (shot->callback_loser))
			//	{
			//		(shot->callback_loser)(shot/*自弾*/);
			//	}
				shot->type = SP_DELETE; 		/* 自弾が消滅 */
				/* ショットが敵に当たった場合、打ち込み点を加算。(無敵キャラでも撃ち込み点で稼げる) */
				player_add_score(score(10));
			}
		}
	}
}


/*---------------------------------------------------------
	プレイヤー、針弾、ショット弾の移動
	霊夢 ＆ 魔理沙 ＆ レミリア ＆ チルノ ＆ 幽々子
---------------------------------------------------------*/

static void player_move_shot(SPRITE *src)
{
	src->x256 += src->vx256;		/*fps_factor*/
	src->y256 += src->vy256;		/*fps_factor*/
	if (REMILIA==select_player) 	/* レミリアはゆらゆら */
	{	u16 rand_int;
		rand_int = ra_nd();
		src->x256 -= (rand_int&0x0200);/*0x0100*/
		src->x256 += (rand_int&0x0100);/*0x0080*/
		src->x256 += (rand_int&0x0100);/*0x0080*/
	}
	else
	if (YUYUKO==select_player)		/* 幽々子はそれる */
	{

		src->vx256 += ((src->vx256*pd_weapon_power)>>11);
	}
	else
//	if (REIMU==select_player)		/* 霊夢の回転ショット */
	if ((REIMU_B+1) > select_player)		/* 霊夢の回転ショット */
	{
		if (((JIKI_SHOT_01)|SP_GROUP_SHOT_SPECIAL)==src->type)
		{
			/* 描画用グラ回転 */
			src->m_angleCCW1024 += (32);	/* cv1024r(10) */
			mask1024(src->m_angleCCW1024);
		}
	}
	if (src->y256 < t256(-10))
	{
		src->type = SP_DELETE;
	}
	else
	{
		player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_00_SHOT);
	}
}


/*---------------------------------------------------------
	敵を探す子関数
---------------------------------------------------------*/
extern SPRITE *sprite_list000_head;
static SPRITE *search_enemy_by_sprite(void)
{
	/* Suche Enemy-Sprite, das noch nicht verfolgt wird, liefert SPRITE-ID oder -1 */
	SPRITE *h = sprite_list000_head;/* スプライト リストの先頭 から探す */
	while (NULL != h)/* スプライト リストの最後まで調べる */
	{
		if (
			#if 1
			(SP_DELETE != h->type ) && /* 削除済みは飛ばす */
			#endif
			(0 != (h->type & (SP_GROUP_TEKI/*|SP_GROUP_BOSS*/))/*SP_GROUP_ENEMYS*/) 	/* プレイヤーにとっての敵(ザコやボス) */
		)
		{
//			if (
#if 0
//				(式) && 						/* 同一フラグが無い敵(別の誘導弾が見つけていない敵) */
#endif
//				(h->flags & SP_FLAG_VISIBLE)										/* 使用中の敵 */
//			)
			{
				#if 0/*SDL(左隅座標)*/
				if ((h->x256 > 0) && (h->x256 < t256(GAME_WIDTH )-((h->w128+h->w128))) &&
					(h->y256 > 0) && (h->y256 < t256(GAME_HEIGHT)-((h->h128+h->h128)))) 	/* 表示画面内の敵 */
				#else/*Gu(中心座標)*/
				if ((h->x256 > 0) && (h->x256 < t256(GAME_WIDTH ) ) &&
					(h->y256 > 0) && (h->y256 < t256(GAME_HEIGHT) ))	/* 表示画面内の敵 */
				#endif
				{
					return (h);/*見つけたよ*/
				}
			}
		}
		h = h->next;/*次*/
	}
	return (obj_player);/*見つからなかった*/
}


/*---------------------------------------------------------
	プレイヤー、誘導弾の移動(親)制御
---------------------------------------------------------*/

static void player_move_parrent_hlaser(SPRITE *src)
{
	if (0==src->PL_HOMING_DATA_state)
	{
		/* 回転しながら正面を向く。 */
		#if 1
	//	/* CCWの場合 */
	//	mask1024(src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/);
		if ( (1024/2) < src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/)
				{	src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/ -= cv1024r(/*360-*/(8));/*fps_factor*/ }	/* 右回りに回転させる。 */
		else	{	src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/ += cv1024r(/*360-*/(8));/*fps_factor*/ }	/* 左回りに回転させる。 */
		mask1024(src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/);
		#endif
		//
		if ( t256(1.0) < src->PL_HOMING_DATA_speed256 )
		{
			src->PL_HOMING_DATA_speed256 -= /*(int)*/t256(0.5);/*減速*/
		}
		//
		#if 1
		/* CCWの場合 */
		if ((src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/ >/*=*/ cv1024r((180-8/*4*/)/*266*/)) &&
			(src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/ </*=*/ cv1024r((180+8/*4*/)/*274*/))) /* ほぼ正面(180)を向いたら */
		{
			src->PL_HOMING_DATA_state++;// = SH_01; 					/* 次へ */
			src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/ = cv1024r((180)); /* 向きを正面(180)に固定する */
		}
		#endif
	}
	else
	{
		/* 他の誘導ボムが、既に倒したか？ */
		if (obj_player == src->target_obj)	/* 見つからない(他の誘導ボムが倒してしまった場合) */
		{
			/* ターゲット検索、敵を探す。 */
			src->target_obj = search_enemy_by_sprite();
		}
		else	/* まだターゲットが生きてる */
		{
			if ( t256(12.0) > src->PL_HOMING_DATA_speed256)
			{
				src->PL_HOMING_DATA_speed256 += /*(int)*/t256(0.5);/*加速*/
			}
			SPRITE *target; 	/* Target */
			target = src->target_obj;
			#if 0/*SDL(左隅座標)*/
			if (//(NULL != target) &&	/* 重複してるので排除 */
				(target->flags&SP_FLAG_VISIBLE) &&
				(target->x256 > t256(0)) &&
				(target->x256 < (t256(GAME_WIDTH))-((target->w128+target->w128))) &&
				(target->y256 > t256(0)) &&
				(target->y256 < (t256(GAME_HEIGHT))-((target->h128+target->h128))))
			#else/*Gu(中心座標)*/
			if (//(NULL != target) &&	/* 重複してるので排除 */
//				(target->flags&SP_FLAG_VISIBLE) &&
				(target->x256 > t256(0)) &&
				(target->x256 < (t256(GAME_WIDTH)) ) &&
				(target->y256 > t256(0)) &&
				(target->y256 < (t256(GAME_HEIGHT)) ))
			#endif
			{
				#if (1)/*誘導(000)*/
				{int ta1024;
				#if 1
				/* CCWの場合 */
					ta1024 = atan_1024(target->y256-src->y256,target->x256-src->x256);
					#if 0
					/* 遅れ誘導 */
					/* KETMのは考え方が複雑なので破棄、0度からの差分で考え180度を境に分岐すれば同等(1周は360度しかないので) */
					#else
					/* 直接誘導 */
					src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/ = ta1024;
					#endif
				#endif
				}
				mask1024(src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/);
				#endif/*誘導(000)*/
			}
			else
			{
				src->target_obj = obj_player;		/* 画面外に逃げられた */
			}
		}
	}
	#if 1
	/* (多分必要ないけど)念の為マスク */
	mask1024(src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/);
	#endif
	#if 1
	/* 表示用 */
//	src->m_angleCCW1024 	= src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/;
	#endif
	src->x256 += ((sin1024((src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/))*src->PL_HOMING_DATA_speed256)>>8);/*fps_factor*/	/* CCWの場合 */
	src->y256 += ((cos1024((src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/))*src->PL_HOMING_DATA_speed256)>>8);/*fps_factor*/
	player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_00_SHOT);
//
	src->PL_HOMING_DATA_time_out--;/*fps_factor*/
	if (0 > src->PL_HOMING_DATA_time_out ) /* 時間切れ？ */
	{
		src->type = SP_DELETE;		/* 時間切れなのでおしまい */
	}
}


/*---------------------------------------------------------
	プレイヤー、誘導弾の移動(子供)
---------------------------------------------------------*/

static void player_move_kodomo_hlaser(SPRITE *src)
{
	#if 1
	SPRITE *oya;
	oya = src->target_obj;
	if (	(SP_DELETE == oya->type) || 			/* 親が消去済みならば子も消去 */
		//	((SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL) != oya->type)			/* (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)以外は親でないので消去 */
			(0==(SP_GROUP_SHOT_SPECIAL & oya->type))	/* (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)以外は親でないので消去 */
		)
	{
		src->type = SP_DELETE;/*おしまい*/
	}
//
	#if 1
	/* 表示用 */
	src->m_angleCCW1024 	= oya->m_angleCCW1024;/*src->PL_HOMING_KODOMO_DATA_angleCCW1024*/
	#endif
	src->x256 = oya->x256;
	src->y256 = oya->y256;

	/* 数フレーム動かない場合は、自動消去する */
	if ((src->PL_HOMING_KODOMO_DATA_check_x256 != src->x256) ||
		(src->PL_HOMING_KODOMO_DATA_check_y256 != src->y256))
	{
		src->PL_HOMING_KODOMO_DATA_time_out = 8;/* 動いたら自動消去しない */
	}
	src->PL_HOMING_KODOMO_DATA_time_out--;
	if ( 0 > src->PL_HOMING_KODOMO_DATA_time_out)
	{
		src->type = SP_DELETE;/*おしまい*/
	}
	src->PL_HOMING_KODOMO_DATA_check_x256 = src->x256;
	src->PL_HOMING_KODOMO_DATA_check_y256 = src->y256;
	#endif
}


/*---------------------------------------------------------
	霊夢専用 低速ボム
---------------------------------------------------------*/

static void move_reimu_musou_fuuin(SPRITE *src)
{
	/* 他の誘導ボムが、既に倒したか？ */
	if (obj_player == src->target_obj)	/* 見つからない(他の誘導ボムが倒してしまった場合) */
	{
		/* ターゲット検索、敵を探す。 */
		src->target_obj = search_enemy_by_sprite();
	}
	else	/* まだターゲットが生きてる */
	{
		if ( t256(3.0) > src->PL_HOMING_DATA_speed256)
		{
			src->PL_HOMING_DATA_speed256 += /*(int)*/t256(0.5);/*加速*/
		}
		SPRITE *target;
		target = src->target_obj;
		#if 1/*Gu(中心座標)*/
		if (
//			(target->flags&SP_FLAG_VISIBLE) &&
			(target->x256 > t256(0)) &&
			(target->x256 < (t256(GAME_WIDTH)) ) &&
			(target->y256 > t256(0)) &&
			(target->y256 < (t256(GAME_HEIGHT)) ))
		#endif
		{
			int ta1024;
		/* CCWの場合 */
			ta1024 = (atan_1024(target->y256-src->y256, target->x256-src->x256));
			#if 1
			/* 遅れ誘導 */
			/* KETMのは考え方が複雑なので破棄、0度からの差分で考え180度を境に分岐すれば同等(1周は360度しかないので) */
		//	const int sabun_aaa = (src->tmp_angleCCW1024-src->m_angleCCW1024);
			const int sabun_aaa1024 = (ta1024-src->m_angleCCW1024);
			if (0 > sabun_aaa1024/*(src->tmp_angleCCW1024-src->m_angleCCW1024)*/ )/* 0レジスタ+共通最適化 */
			{	/* 目標角度 < 制御角度 */
				if ( (1024/2) > sabun_aaa1024/*(src->tmp_angleCCW1024-src->m_angleCCW1024)*/ )/* 共通最適化 */
						{	src->m_angleCCW1024 -= 10;	}	/* 180度以内なら近いほうから回る */
				#if 0	/* 省略して問題ない？ */
				else	{	src->m_angleCCW1024 += 10;	}	/* 180度以上なら反対周りのほうが近い */
				#endif
			}
			else	/* 目標角度 > 制御角度 */
			{
				if ( (1024/2) > -(sabun_aaa1024)/*(src->m_angleCCW1024-src->tmp_angleCCW1024)*/ )
						{	src->m_angleCCW1024 += 10;	}	/* 180度以内なら近いほうから回る */
				#if 0	/* 省略して問題ない？ */
				else	{	src->m_angleCCW1024 -= 10;	}	/* 180度以上なら反対周りのほうが近い */
				#endif
			}
			#else
			/* 直接誘導 */
			src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/ = ta1024;
			#endif
			mask1024(src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/);
		}
		else
		{
			/* Ziel wurde anderweitig vernichtet, neues Ziel suchen */
			src->target_obj = obj_player;		/* 画面外に逃げられた */
		}
	}
//
#define PL_HOMING_DATA_reimu_musou_id PL_HOMING_DATA_time_out
	if((pd_bomber_time&0x0f)==(src->PL_HOMING_DATA_reimu_musou_id&0x0f))
	{
		src->vx256 = ((sin1024((src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/))*src->PL_HOMING_DATA_speed256)>>8);/*fps_factor*/	/* CCWの場合 */
		src->vy256 = ((cos1024((src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/))*src->PL_HOMING_DATA_speed256)>>8);/*fps_factor*/
	}
		src->x256 += (src->vx256);/*fps_factor*/
		src->y256 += (src->vy256);/*fps_factor*/
	player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_01_BOMB/*PLAYER_WEAPON_TYPE_00_SHOT*/);
//
	if (0 >= pd_bomber_time)	/* 時間切れの場合 */
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
		src->base_weapon_strength--;/* 時間切れの場合、体力を減らす */
	}
//
	if (0 > src->base_weapon_strength)	{	src->type = SP_DELETE;	}	/* 体力切れでおしまい */
}


/*---------------------------------------------------------
	プレイヤー、高速時ボムの移動(霊夢)

	時計回り ＆ 反時計回り 兼用
---------------------------------------------------------*/

static void player_move_kekkai(SPRITE *src) /* 霊夢 */
{
	src->PL_KEKKAI_DATA_angleCCW1024 += (src->PL_KEKKAI_DATA_add_r1024);	/*fps_factor*/	/*...*/
	mask1024(src->PL_KEKKAI_DATA_angleCCW1024);
	#if 1
	/* 描画用角度(下が0度で左回り(反時計回り)) */
	src->m_angleCCW1024 = (src->PL_KEKKAI_DATA_angleCCW1024);
	#endif
//
	#if 0/*SDL(左隅座標)*/
	src->x256 = obj_player->x256 + sin1024((src->PL_KEKKAI_DATA_angleCCW1024))*src->PL_KEKKAI_DATA_radius+((obj_player->w128-src->w128));	/* CCWの場合 */
	src->y256 = obj_player->y256 + cos1024((src->PL_KEKKAI_DATA_angleCCW1024))*src->PL_KEKKAI_DATA_radius+((obj_player->h128-src->h128));
	#else/*Gu(中心座標)*/
	src->x256 = obj_player->x256 + sin1024((src->PL_KEKKAI_DATA_angleCCW1024))*src->PL_KEKKAI_DATA_radius;	/* CCWの場合 */
	src->y256 = obj_player->y256 + cos1024((src->PL_KEKKAI_DATA_angleCCW1024))*src->PL_KEKKAI_DATA_radius;
	#endif
//	if (REIMU==select_player)
	{
		if (src->PL_KEKKAI_DATA_radius<255) 	{	src->PL_KEKKAI_DATA_radius += (ra_nd()&1);	}	/* 少しずつ広がるようにした */
		else									{	src->type = SP_DELETE;						}	/* 回転が広がったらおしまい */
	//	src->alpha				= ((int)(255-src->PL_KEKKAI_DATA_radius))&(256-1);	/* だんだん消えるようにした */
	//	src->color32			= ((((255-src->PL_KEKKAI_DATA_radius))&(0xff))<<24)|0x00ffffff; /* だんだん消えるようにした */
		src->color32			= ((((255-src->PL_KEKKAI_DATA_radius))&(0xff))<<24)|0x00ffffff; /* だんだん消えるようにした */
	}
//	else //if (YUYUKO==select_player)
//	{

//		if (pd_bomber_time<=0)	{	src->type = SP_DELETE;}/* 時間制限でおしまい */
//	}
	if ( (ra_nd()&0xff) < pd_bomber_time)
	{
		if(0==((src->PL_KEKKAI_DATA_radius)&7))
		{
			//player_weapon_colision_check_kekkai(src, src->PL_KEKKAI_DATA_radius);
			bullets_to_hosi();/* 弾全部、星アイテムにする */
		}
	}
	player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_01_BOMB);/* ボムで敵を倒すあたり判定 */
}


/*---------------------------------------------------------
	プレイヤー、高速時ボム。画面外はみ出しチェック。あたり判定チェック。
---------------------------------------------------------*/
static void player_bomber_out_colision_check(SPRITE *src)
{
	#if 0/*SDL(左隅座標)*/
	if ((src->x256+((src->w128+src->w128)) < t256(0))||(src->x256 > t256(GAME_WIDTH))||
		(src->y256+((src->h128+src->h128)) < t256(0))||(src->y256 > t256(GAME_HEIGHT)))
	#else/*Gu(中心座標)*/
	if ((src->x256 < t256(0))||(src->x256 > t256(GAME_WIDTH))||
		(src->y256 < t256(0))||(src->y256 > t256(GAME_HEIGHT)))
	#endif
	{
		src->type = SP_DELETE;/* 画面外に出たらおしまい */
	}
	else
	{
		player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_01_BOMB);/* ボムで敵を倒すあたり判定 */
	}
}

/*---------------------------------------------------------
	プレイヤー、高速時ボムの移動(魔理沙、チルノ、仮幽々子(違う感じにしたい) )
---------------------------------------------------------*/

static void player_move_levarie(SPRITE *src)
{
	src->x256 += src->vx256;	/*fps_factor*/
	src->y256 += src->vy256;	/*fps_factor*/
	#if 1
	/* YUYUKO MARISA_A 以外 */
	if (
	//	(CIRNO_A==select_player) || (CIRNO_Q==select_player) ||
		((CIRNO_A-1)<select_player) ||
		(MARISA_B==select_player))/* マスパ(もどき) */
	#endif
	{	/*チルノ用(暫定的)*/	/*1.5*(d->speed);*/ 	/*fps_factor*/
		src->y256 -= (abs((src->vx256+(src->vx256>>1) )));
		src->y256 -= (abs((src->vy256+(src->vy256>>1) )));
		src->y256 -= t256(1.0);
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
		src->m_angleCCW1024 	+= aaa_sss[select_player];
		mask1024((src->m_angleCCW1024));
		if ( t256(4.0) > src->m_zoom_x256)
		{
			src->m_zoom_x256 += aaa_sss[select_player+(PLAYERS8)];
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
	src->x256 += src->vx256;	//	src->vx256;//co_s1024((data->angle1024))*p->speed;/*fps_factor*/
	src->y256 += src->vy256;	//	src->vy256;//si_n1024((data->angle1024))*p->speed;/*fps_factor*/
//	if ((src->x256+((src->w128+src->w128)) < t256(0))||(src->x256 > t256(GAME_WIDTH))||
//		(src->y256+((src->h128+src->h128)) < t256(0)))
//
	player_bomber_out_colision_check(src);
}

/* 十字炎ボムの炎の部分 */
/*		r_or_l	[xxx_r] l=2, [xxx_l] r=0, u=3, d=1	*/
static void remilia_add_burn_fire(SPRITE *src/*, int ggg*/ /*r_or_l*/)	/* [***090221 追加 */
{
	int angCCW1024;
	for (angCCW1024=0; angCCW1024<(1024); angCCW1024+=(256)/*(20)*/)
	{
		int i;
		for (i=0; i<10; i+=1)
		{
			SPRITE *h;
			h = sprite_add_gu_error();
			if (NULL!=h)/* 登録できた場合のみ */
			{
				//
				h->m_Hit256R		= JIKI_ATARI_ITEM_16;
//				h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
				h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL)/*ボスに有効*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/
				h->callback_mover	= remilia_move_burn_fire;
				h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x64);		/*	h->alpha			= 0x64 100;*/
				h->base_weapon_strength 	= (8*(5));/* 仕様変更した */	/*9*/ /*10*/
				{
			u32 my_rand;
				my_rand = ra_nd();
					if (YUYUKO==select_player)
					{
						h->x256 			= (u16)((my_rand	)) + t256((GAME_WIDTH -256)/2);
						h->y256 			= (u16)((my_rand>>16)) + t256((GAME_HEIGHT-256)/2);
					}
					else
					{
						#if 1/*Gu(中心座標)*/
						h->x256 			= src->x256;
						h->y256 			= src->y256+t256(15);
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
				h->m_angleCCW1024		= ang_aaa_1024;
				#endif
				h->vx256		= ((sin1024((ang_aaa_1024))*(rnd_spd))>>8);/*fps_factor*/ /* CCWの場合 */
				h->vy256		= ((cos1024((ang_aaa_1024))*(rnd_spd))>>8);/*fps_factor*/
				}
			}
		}
	}
}


/*---------------------------------------------------------
	スペカ発動時の立ち絵
---------------------------------------------------------*/

static void player_move_tachie(SPRITE *src) 	/* [***100113 追加 */
{
	if ((128-1) < pd_bomber_time)/* 残り時間が 128 以上なら、移動して待機 */
	{
		/* 画面外から出現、停止。しばし待つ。 */
		if ( t256(100) > src->x256 )
		{
			src->x256 += t256(5.0);
		}
	}
	else
	if ((1/*-1*/) < pd_bomber_time)/* 拡大 */
	{
		/* (24+(1))  、ここでの pd_bomber_time は、最大0x7fなので。 */
		src->color32		= (pd_bomber_time<<(24+(1)))|0x00ffffff;
		src->m_zoom_x256	+= (10);
	}
	else
	{
		src->type = SP_DELETE;	/* 時間制限でおしまい */
	}
}


/*---------------------------------------------------------
	レミリアボムの十字炎
---------------------------------------------------------*/

static void player_move_add_cross_red(SPRITE *src)	/* [***090220 追加 */
{
	//
//	if (0x3f==(((int)pd_bomber_time)&0x3f))
//	if (0x0f==(((int)pd_bomber_time)&0x0f))
	if (0x0f==(((int)pd_bomber_time)&0x0f))/* 波動を撃つタイミング */
	{
		remilia_add_burn_fire(obj_player);/* 波動発動(早口言葉?) */
	}
	#if 1/*Gu(中心座標)*/
	src->x256 = obj_player->x256;
	src->y256 = obj_player->y256;
	#endif

	if (0 >= pd_bomber_time)	{ src->type = SP_DELETE;}/* 時間制限でおしまい */
}

/* ====================================================================================== */
/* ====================================================================================== */
/* ====================================================================================== */

/*---------------------------------------------------------
	プレイヤー弾(ショット/針兼用)生成、子関数
	プレイヤー、針弾(霊夢)の追加	 [***090220 追加
		(霊夢)		[針弾]、
		(魔理沙)	[森弾]、
		(レミリア)	[コウモリ弾]、
		(幽々子)	[蝶弾] 、
		(仮チルノ)	[丸氷弾]、
---------------------------------------------------------*/

static int weapon_level_offset; /* 装備した武器レベル(番号)の８倍にプレイヤー値を足した値をオフセットとして保持 */
/*
	weapon_level_offset == player番号 + (武器レベル x 8)
	weapon_level_offset =  select_player + (weapon_level<<3);
*/

enum /*_weapon_type_*/	/* 武器レベル */
{
	WEAPON_L0 = 0,		/* WEAPON_L0(P000-P008) */
	WEAPON_L1,			/* WEAPON_L1(P008-P015) */
	WEAPON_L2,			/* WEAPON_L2(P016-P031) */
	WEAPON_L3,			/* WEAPON_L3(P032-P063) */
	WEAPON_L4,			/* WEAPON_L4(P064-P127) */
	WEAPON_L5,			/* WEAPON_L5(P128)		*/
	WEAPON_L_MAX		/* 最大数 */
};


enum
{
//	/* (誘導ショット) */
	REIMU_SHOT_TYPE_00 = 0,
	/* (非誘導ショット) */
	REIMU_SHOT_TYPE_01,
	REIMU_SHOT_TYPE_02,
	REIMU_SHOT_TYPE_03,
	REIMU_SHOT_TYPE_04,
	REIMU_SHOT_TYPE_05,
	REIMU_SHOT_TYPE_06,
	REIMU_SHOT_TYPE_07,
//	/* 非誘導ショット(幽々子) */
	YUYUKO_SHOT_TYPE_08,
	YUYUKO_SHOT_TYPE_09,
	YUYUKO_SHOT_TYPE_0a,
	YUYUKO_SHOT_TYPE_0b,
//	/* 針(サブショット) */
	NEEDLE_ANGLE_270,
	NEEDLE_ANGLE_263,	/* レミリア 用 */
	NEEDLE_ANGLE_277,	/* レミリア 用 */
	NEEDLE_ANGLE_271,	/* 幽々子 用 */
//
	REIMU_SHOT_TYPE_MAX 	/* 最大数 */	//	NEEDLE_MAX
};

enum
{
	REI00_x_offset=0,
	REI01_y_offset,
	REI02_vx256,
	REI03_status,	/* 2==霊夢の回転ショット */
	REI04_MAX		/* 最大数 */
};
#define TEST_OFFSET (8)

static void player_register_shot_object(SPRITE *src, int /*set_*/shot_type)
{
	SPRITE *h;/* shot */
	h					= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		static const s16 r_tbl[REIMU_SHOT_TYPE_MAX][REI04_MAX] =
		{	/*x_offset256	y_offset256  vx256 status */
	//	/* 誘導ショット(霊夢 ＆ 魔理沙) */
	/*00*/	{t256(	0), t256(0/*+25*/),  (	0), 0x01},	 /*shot_regist_triple_fuda*/ /*(-24)==(4*(- 6))*/
	//	/* 非誘導ショット(霊夢) */
	/*01*/	{t256(TEST_OFFSET+	0), t256( 20),	(  0), 0x00},	/* 中 まっすぐ */
	/*02*/	{t256(TEST_OFFSET  -5), t256( 20),	(  0), 0x00},	/* 左 まっすぐ */
	/*03*/	{t256(TEST_OFFSET  +5), t256( 20),	(  0), 0x00},	/* 右 まっすぐ */
	/*04*/	{t256(TEST_OFFSET  -5), t256(+20),	(-20), 0x00},	/* 左 左上 */
	/*05*/	{t256(TEST_OFFSET  +5), t256(+20),	( 20), 0x00},	/* 右 右上 */
	/*06*/	{t256(TEST_OFFSET  -5), t256(+20),	(-35), 0x00},	/* 左 左斜上 */
	/*07*/	{t256(TEST_OFFSET  +5), t256(+20),	( 35), 0x00},	/* 右 右斜上 */
	//	/* 非誘導ショット(幽々子) */
	/*08*/	{t256(TEST_OFFSET -15), t256(-25),	(-25), 0x00},	/* 幽々子 */					/*(-32)==(4*(- 8))*/
	/*09*/	{t256(TEST_OFFSET +15), t256(-25),	(+25), 0x00},	/* 幽々子 */					/*( 32)==(4*(  8))*/
	/*0a*/	{t256(TEST_OFFSET -25), t256(-25),	(-50), 0x00},	/* 幽々子 */					/*(-64)==(4*(-16))*/
	/*0b*/	{t256(TEST_OFFSET +25), t256(-25),	(+50), 0x00},	/* 幽々子 */					/*( 64)==(4*( 16))*/
	//	/* 針(サブショット) */		/* CCWの場合 */
			{t256(	0), t256( -7),	(	0), 0x01},//	cv1024r((180)), 				/* 魔理沙 A/B 霊夢 レミリア ＆ チルノ */		/*t256( 0), t256( 0),*/
			{t256(	0), t256( -7),	(-108), 0x01},//	cv1024r((180-25)),/*263=270-7*/ /* レミリア ＆ チルノ */ /*t256( 0), t256( 0),*/
			{t256(	0), t256( -7),	( 108), 0x01},//	cv1024r((180+25)),/*277=270+7*/ /* レミリア ＆ チルノ */ /*t256( 0), t256( 0),*/
			{t256(	0), t256(-47),	(	5), 0x01},//	cv1024r((181)), 				/* 幽々子専用 */	/*-48*/ /*t256( 0), t256(-40),*/
		};
	//	int shot_type;
		int shot_status;/*針?*/
		shot_status = ((r_tbl[shot_type][REI03_status]));

		int is_needle;/*針?*/
	//	is_needle = (shot_type <= NEEDLE_ANGLE_270)?(1/*針*/):(0/*弾*/);
		is_needle = ((shot_status)&0x01);
	//
		#define OFFS_IS_NEEDLE (select_player+(is_needle<<3))
		const static u8 base_shot_atari[16] =
		{
		/*0,*/	(4),	/* re a */		//	BASE_SHOT_ATARI_0a,/*4*/		/* 霊夢A */ 	/*	5 */ BASE_SHOT_ATARI_0a,/*4*/
		/*0,*/	(4),	/* re b */		//	BASE_SHOT_ATARI_0b,/*4*/		/* 霊夢B */ 	/*	5 */ BASE_SHOT_ATARI_0a,/*4*/
		/*0,*/	(4),	/* ma a */		//	BASE_SHOT_ATARI_0c,/*4*/		/* 魔理沙A */	/*	6 */ BASE_SHOT_ATARI_0b,/*4*/
		/*0,*/	(4),	/* ma b */		//	BASE_SHOT_ATARI_0d,/*4*/		/* 魔理沙B */	/*	6 */ BASE_SHOT_ATARI_0b,/*4*/
		/*0,*/	(4),	/* oz / */		//	BASE_SHOT_ATARI_0e,/*4*/		/* 魔理沙C */	/*	7 */ BASE_SHOT_ATARI_0c,/*4*/
		/*0,*/	(8),	/* yu / */		//	BASE_SHOT_ATARI_0f,/*4*/		/* レミリア */	/*	9 */ BASE_SHOT_ATARI_0e,/*8*/
		/*0,*/	(6),	/* ci a */		//	BASE_SHOT_ATARI_0g,/*6*/		/* チルノ */	/*	8 */ BASE_SHOT_ATARI_0d,/*6*/
		/*0,*/	(6),	/* ci q */		//	BASE_SHOT_ATARI_0h,/*8*/		/* 幽々子 */	/*	6 */ BASE_SHOT_ATARI_0b,/*4*/
////
		/*0,*/	(6),	/* re a */		//	BASE_NEEDLE_ATARI_0a,/*6*/		/* 霊夢A */ 	/*	5 */  BASE_NEEDLE_ATARI_0a,/*6*/
		/*0,*/	(6),	/* re b */		//	BASE_NEEDLE_ATARI_0b,/*6*/		/* 霊夢B */ 	/*	5 */  BASE_NEEDLE_ATARI_0a,/*6*/
		/*0,*/	(8),	/* ma a */		//	BASE_NEEDLE_ATARI_0c,/*8*/		/* 魔理沙A */	/*	6 */  BASE_NEEDLE_ATARI_0b,/*8*/
		/*0,*/	(8),	/* ma b */		//	BASE_NEEDLE_ATARI_0d,/*8*/		/* 魔理沙B */	/*	6 */  BASE_NEEDLE_ATARI_0b,/*8*/
		/*0,*/ (12),	/* oz / */		//	BASE_NEEDLE_ATARI_0e,/*8*/		/* 魔理沙C */	/*	7 */  BASE_NEEDLE_ATARI_0c,/*12*/
		/*0,*/	(8),	/* yu / */		//	BASE_NEEDLE_ATARI_0f,/*12*/ 	/* レミリア */	/*	9 */  BASE_NEEDLE_ATARI_0e,/*8*/
		/*0,*/ (16),	/* ci a */		//	BASE_NEEDLE_ATARI_0g,/*16*/ 	/* チルノ */	/*	8 */  BASE_NEEDLE_ATARI_0d,/*16*/
		/*0,*/ (16),	/* ci q */		//	BASE_NEEDLE_ATARI_0h,/*8*/		/* 幽々子 */	/*	6 */  BASE_NEEDLE_ATARI_0b,/*8*/
		};
		h->m_Hit256R			= ((base_shot_atari[(OFFS_IS_NEEDLE)])<<8);/* あたり判定の大きさ */

	//	h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JI KI_SHOT_02|SP_GROUP_SHOT_SPECIAL)-shot_status;
		#if 1
	//	else
	//	if(0x01== shot_status)	/* 針 */
		{
			h->type 			= (JIKI_SHOT_07|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/	/*SP_PL_FIREBALL*/
		}
	//
		if (0x00 == shot_status)	/* 針以外 */
		{
			if (REIMU_B+1 > select_player)	/* 霊夢? */
			{
			//	if(0x02== shot_status)	/* 霊夢の回転ショット */
				if (REIMU_SHOT_TYPE_00 != shot_type)/* 自動ショット以外？ */
				{
					h->type 			= (JIKI_SHOT_01|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/
					/* 描画用グラ回転 */
					h->m_angleCCW1024	= (ra_nd() & (1024-1));/* 出だしの角度はランダムでないと(レーザーみたいな)変な画面になる */
				}
			/*	else	{	h->type = 自動ショットは、針と同じ; 	}	*/
			}
			else
			{
				h->type 			= (JIKI_SHOT_02|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/
			}
		}
	//	else
		#endif
		#if 0/*SDL(左隅座標)*/
		h->x256 			= (src->x256)+((src->w128-src->w128))	+ r_tbl[shot_type][REI00_x_offset]; /*+ x_offs*/
		h->y256 			= (src->y256)-((src->h128+src->h128))	+ r_tbl[shot_type][REI01_y_offset];/*(20)*/
		#else/*Gu(中心座標)*/
		h->x256 			= (src->x256) + r_tbl[shot_type][REI00_x_offset]; /*+ x_offs*/
		h->y256 			= (src->y256) + r_tbl[shot_type][REI01_y_offset];/*(20)*/
		#endif
		h->callback_mover	= player_move_shot;
//		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x96);	/*	h->alpha		= 0x96; 150 半透明にする。 */	/*shot_regist_triplestar*/
	//
		#define CCC_STRENGTH_L1 	(PLAYERS8* 0)/* WEAPON_L0 */
		#define CCC_STRENGTH_L2 	(PLAYERS8* 1)/* WEAPON_L1 */
		#define CCC_STRENGTH_L3 	(PLAYERS8* 2)/* WEAPON_L2 */
		#define CCC_STRENGTH_L4 	(PLAYERS8* 3)/* WEAPON_L3 */
		#define CCC_STRENGTH_L5 	(PLAYERS8* 4)/* WEAPON_L4 */
		#define CCC_STRENGTH_L6 	(PLAYERS8* 5)/* WEAPON_L5 */
		#define CCC_SPEED			(PLAYERS8* 6)
		#define DDD_SPEED			(PLAYERS8* 7)
	//
		#define DDD_STRENGTH_L1 	(PLAYERS8* 8)/* WEAPON_L0 */
		#define DDD_STRENGTH_L2 	(PLAYERS8* 9)/* WEAPON_L1 */
		#define DDD_STRENGTH_L3 	(PLAYERS8*10)/* WEAPON_L2 */
		#define DDD_STRENGTH_L4 	(PLAYERS8*11)/* WEAPON_L3 */
		#define DDD_STRENGTH_L5 	(PLAYERS8*12)/* WEAPON_L4 */
		#define DDD_STRENGTH_L6 	(PLAYERS8*13)/* WEAPON_L5 */
	//	#define DDD_MAX 			(PLAYERS8*2)		/* 最大数 */
		#define CCC_MAX 			(PLAYERS8*14)		/* 最大数 */
		static const u8/*u8*/ ccc_tbl[CCC_MAX] =
		{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
	//		 5*8, 5*8,	5*8, 5*8,  5*8,16*8,  5*8, 5*8, 	/* WEAPON_L0  strength ショットの強さ */ /* (L1ショットは、弾数が少ないので 5以下にしない) */
	//		 2*8, 3*8,	3*8, 2*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L1  strength ショットの強さ */
	//		 2*8, 3*8,	3*8, 1*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L2  strength ショットの強さ */
	//		 2*8, 3*8,	3*8, 1*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L3  strength ショットの強さ */
	//		 2*8, 3*8,	3*8, 1*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L4  strength ショットの強さ */
	//		 2*8, 3*8,	3*8, 1*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L5  strength ショットの強さ */
	//
			 5*8, 5*8,	5*8, 5*8,  5*8,16*8,  5*8, 5*8, 	/* WEAPON_L0  strength ショットの強さ */ /* (L1ショットは、弾数が少ないので 5以下にしない) */
			 2*8, 3*8,	3*8, 3*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L1  strength ショットの強さ */
			 2*8, 3*8,	3*8, 3*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L2  strength ショットの強さ */
			 2*8, 3*8,	3*8, 3*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L3  strength ショットの強さ */
			 2*8, 3*8,	3*8, 3*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L4  strength ショットの強さ */
			 2*8, 3*8,	3*8, 3*8,  4*8,16*8,  5*8, 5*8, 	/* WEAPON_L5  strength ショットの強さ */

			16,14, 10,12,  9, 4,  8, 8, 	/* speed	ショットの速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */
			18,18, 10,10,  3, 4,  3, 3, 	/* speed	針の速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */
	//
			 4*8, 4*8,	4*8,20*8,  6*8, 8*8,  6*8, 6*8, 	/* WEAPON_L0  strength 針の強さ */	/*REMILIA, CIRNO,  6 5 強すぎる*/
			 4*8, 4*8,	4*8,20*8,  6*8, 8*8,  6*8, 6*8, 	/* WEAPON_L1  strength 針の強さ */	/*REMILIA, CIRNO,  6 5 強すぎる*/
			 4*8, 4*8,	4*8,20*8,  6*8, 8*8,  6*8, 6*8, 	/* WEAPON_L2  strength 針の強さ */	/*REMILIA, CIRNO,  6 5 強すぎる*/
			 4*8, 4*8,	4*8,20*8,  6*8, 8*8,  6*8, 6*8, 	/* WEAPON_L3  strength 針の強さ */	/*REMILIA, CIRNO,  6 5 強すぎる*/
			 4*8, 4*8,	4*8,20*8,  6*8, 8*8,  6*8, 6*8, 	/* WEAPON_L4  strength 針の強さ */	/*REMILIA, CIRNO,  6 5 強すぎる*/
			 4*8, 4*8,	4*8,20*8,  6*8, 8*8,  6*8, 6*8, 	/* WEAPON_L5  strength 針の強さ */	/*REMILIA, CIRNO,  6 5 強すぎる*/
	/* [***20090822 REIMU 3 <- 2 (とりあえず針弱すぎるので) */
		};

	//		 2, 3,	3, 2,  4,16, 5, 5,	/* strength ショットの強さ */
	//		16,14, 10,12,  9, 4, 8, 8,	/* speed	ショットの速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */
		//};
	//		 2, 2, 3, 3, 4, 3, 3, 3,	/* strength ショットの強さ */
	//		15,15,12,12,18,12,12,12,	/* speed	ショットの速さ	*/
	/* 極端に機体の性能が偏るのは良くない。(強くて速いのはずるい) */
		//static const u8 ddd_tbl[DDD_MAX] =
		//{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
	//		 3, 3, 4, 4, 6, 8, 6, 6,	/* strength 針の強さ */ 	/*REMILIA, CIRNO,  6 5 強すぎる*/
	/* 霊夢(強くしてみる [***20090930 ) */
	//		 4, 4, 4,10, 6, 8, 6, 6,	/* strength 針の強さ */ 	/*REMILIA, CIRNO,  6 5 強すぎる*/
	//		 4, 4, 4,25, 6, 8, 6, 6,	/* strength 針の強さ */ 	/*REMILIA, CIRNO,  6 5 強すぎる*/
	//		18,18,10,15, 3, 4, 3, 3,	/* speed	針の速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */


		h->base_weapon_strength = ccc_tbl[CCC_STRENGTH_L1+(weapon_level_offset)+(is_needle<<(4+2))];/* 64 == 8[players]*8[line] */
	//	h->base_weapon_strength = ddd_tbl[DDD_STRENGTH_L1+(weapon_level_offset)];

	//
	//	int ok;
	//	ok=0;
		{
			#if 0
		//	if (1==r_tbl[shot_type][REI03_yudo])	/* pspは0レジスタがあるので0と比較したほうが速い */
			if (0 != r_tbl[shot_type][REI03_yudo])/* 自動ショットか？ */
			#else
		//	if ( (REIMU_SHOT_TYPE_01) > shot_type)/* 自動ショットか？ */
			if ( (REIMU_SHOT_TYPE_00) == shot_type)/* 自動ショットか？ */
			#endif
			{
				SPRITE *target;
				target = search_enemy_by_sprite();
				if (target != obj_player)
				{
					int int_angle1024;
					int_angle1024	= atan_1024(target->y256-obj_player->y256, target->x256-obj_player->x256);
				//	if (511 < int_angle1024 )/* 上方180-360度[/360度]の領域のみ追尾する */
					if ( /*(384)*/(256*3) < (( int_angle1024+128+256)&(1024-1)) )/* 上方225-315度[/360度]の領域のみ追尾する */	/* CCWの場合 */
					{
						h->vx256	 = sin1024((int_angle1024))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+select_player]*/);/*fps_factor*/ /* CCWの場合 */
						h->vy256	 = cos1024((int_angle1024))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+select_player]*/);/*fps_factor*/
						#if 1
						/* 描画用グラ回転 */
					//	h->m_angleCCW1024	= int_angle1024;
						/* 描画用グラ回転(ショットは上が正面なので、回転角を計算)[180/360度]回す。 */
						int_angle1024 += cv1024r((180));
						mask1024(int_angle1024);
						h->m_angleCCW1024	= int_angle1024;/* 下が正面の角度が描画用角度 */
						#endif
					//	ok=1;/* 自動追尾可能 */
						return;
					}
				}
			}
		}
	//
	//	if (0==ok)/* 自動追尾不可？ */
		{	/*固有の設定角度を使う*/
			h->vx256 = ((/*int_vx*/r_tbl[shot_type][REI02_vx256]))*/*p->speed*/(ccc_tbl[CCC_SPEED+OFFS_IS_NEEDLE]);/*fps_factor*/
			h->vy256 = -(((int)ccc_tbl[CCC_SPEED+OFFS_IS_NEEDLE])<<8);/*fps_factor*/ /*p->speed*/
		}
	}
}


/*---------------------------------------------------------
	プレイヤー弾(ショットのみ)生成
---------------------------------------------------------*/

/* 霊夢A (誘導巫女) 自動ショット */

static void shot_regist_re_yudou(SPRITE *s) /* 霊夢A 魔理沙A */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_00);/* 自動ショット */
}

static void shot_regist_yu_gggg(SPRITE *s)	/* 幽々子 */
{
	/*shot_regist_needle*/player_register_shot_object(s, NEEDLE_ANGLE_271);
}

static void shot_regist_ma_gggg(SPRITE *s)	/* 魔理沙A/B */ 	/* 兼(現在ダミー用) */
{
	/*shot_regist_needle*/player_register_shot_object(s, NEEDLE_ANGLE_270);
}

static void shot_regist_ci_gggg(SPRITE *s)	/* レミリア ＆ チルノ */
{
	/*shot_regist_needle*/player_register_shot_object(s, NEEDLE_ANGLE_277 );
	/*shot_regist_needle*/player_register_shot_object(s, NEEDLE_ANGLE_263 );
}
static void shot_regist_ma_lazer(SPRITE *s) /* 魔理沙B レーザー */
{
	{
		static int count123;count123++;
		if (count123&0x20)	{	return; }
	}
	shot_regist_ma_gggg(s);
}

/*---------------------------------------------------------
	プレイヤー弾(ショットのみ)生成
---------------------------------------------------------*/

/* 霊夢A/B 回転ショット */	/* 通常ショット */
static void shot_regist_1(SPRITE *s)	/* 霊夢 */	/* 霊夢 ＆ 魔理沙 ＆ チルノ ＆ レミリア */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* 通常ショット */
}
static void shot_regist_2(SPRITE *s)	/* 霊夢 */	/* 霊夢 ＆ 魔理沙 ＆ チルノ ＆ レミリア */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* 通常ショット */
}
static void shot_regist_3(SPRITE *s)	/* 霊夢 */	/* 魔理沙 ＆ チルノ */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* 通常ショット */
}
static void shot_regist_4(SPRITE *s)	/* 霊夢 */	/* 魔理沙 */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* 通常ショット */
}

static void shot_regist_5(SPRITE *s)	/* 霊夢B */ /* 魔理沙 */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_06); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_07); 	/* 通常ショット */
}

/* 霊夢B (針巫女) */

/* 魔理沙A(ミサイル魔理沙) */
/* 魔理沙B(レーザー魔理沙) */


static void shot_regist_chou(SPRITE *s) 		/* 幽々子 */
{
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_08);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_09);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_0a);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_0b);
}


/*---------------------------------------------------------
	プレイヤー、オプションの移動制御
---------------------------------------------------------*/
static void re_ma_yu_control_option(SPRITE *src) /* 霊夢、魔理沙	仮幽々子 */
{
	mask1024(src->PL_OPTION_DATA_angleCCW1024);
	{
	#define SSS_LOCATE			(PLAYERS8*0)
	#define SSS_SPEED			(PLAYERS8*1)
	#define SSS_MAX 			(PLAYERS8*2)		/* 最大数 */
		static const u8 sss_tbl[(SSS_MAX)] =
		{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
			14*16,8*16,14*16,14*16, 6*16, 6*16, 6*16, 6*16, /* SSS_LOCATE (YUYUKO==select_player)?(3):(7); */
			14,14,14,14, 6, 6, 14, 14,	/* SSS_SPEED  (YUYUKO==select_player)?(3):(7); */
		};
		int vv1024;
		int ss1024;
	//	vv1024 = (YUYUKO==select_player)?(3):(7);	/*ra d2deg512(0.1) == 8.1*/
		vv1024 = sss_tbl[SSS_LOCATE+select_player];
		ss1024 = sss_tbl[SSS_SPEED+select_player];
		if (my_pad & PSP_KEY_SLOW)		/* 押すと前方に移動 */
		{
			if ((OPTION_ID_03_RM/*1*/&0xff)==(src->PL_OPTION_DATA_opt_anime_add_id&0xff)/*pos*/) /* 1: 右側系のオプション(OPTION_ID_03_RM, OPTION_ID_04_RR ) */
			{
				if (src->PL_OPTION_DATA_angleCCW1024 < /*(288)*/(256*2+16*2 -16*2-64*2+(14<<4)+((src->PL_OPTION_DATA_opt_anime_add_id&0x100)>>1)-(vv1024) )/*272 (256+16)*/ )/*中位(右)位置*/
				{
					src->PL_OPTION_DATA_angleCCW1024 += ss1024;
				}
			}
			else	/* 0: 左側系のオプション */
			{
				if (src->PL_OPTION_DATA_angleCCW1024 > /*(224)*/(256*2-16*2 +16*2+64*2-(14<<4)-((src->PL_OPTION_DATA_opt_anime_add_id&0x100)>>1)+(vv1024) )/*240 (256-16)*/ )/*中位(左)位置*/
				{
					src->PL_OPTION_DATA_angleCCW1024 -= ss1024;
				}
			}
		}
		else	/* 放すと後方に移動 */
		{
			if ( (OPTION_ID_03_RM/*1*/&0xff)==(src->PL_OPTION_DATA_opt_anime_add_id&0xff)/*pos*/) /* 1: 右側系のオプション */
			{
				if (src->PL_OPTION_DATA_angleCCW1024 > 64*2+16*2 +8*2+32*2-64*2+((src->PL_OPTION_DATA_opt_anime_add_id&0x100)>>1) )/*最小(右)位置*/
				{
					src->PL_OPTION_DATA_angleCCW1024 -= ss1024;
				}
			}
			else	/* 0: 左側系のオプション */
			{
				if (src->PL_OPTION_DATA_angleCCW1024 < 448*2-16*2 -8*2-32*2+64*2-((src->PL_OPTION_DATA_opt_anime_add_id&0x100)>>1) )/*最大(左)位置*/
				{
					src->PL_OPTION_DATA_angleCCW1024 += ss1024;
				}
			}
		}
	}
	#if 1
	mask1024(src->PL_OPTION_DATA_angleCCW1024);/*ねんのため*/
	#endif
//	h->x256 = obj_player->x256+cos512((src->PL_OPTION_DATA_angle512))*(16)/*src->PL_OPTION_DATA_length*/-((h->w)>>1)+((obj_player->w-12-1)*src->PL_OPTION_DATA_opt_anime_add_id)+6;
//	h->x256 = obj_player->x256+cos512((src->PL_OPTION_DATA_angle512))*(16)/*src->PL_OPTION_DATA_length*/+((/*obj_player->w*/24-12-1)*src->PL_OPTION_DATA_opt_anime_add_id)+6-8;
//	h->x256 = obj_player->x256+cos512((src->PL_OPTION_DATA_angle512))*(16)/*src->PL_OPTION_DATA_length*/+((/*obj_player->w*/24+11+11)*src->PL_OPTION_DATA_opt_anime_add_id)-11-8;
// -1 : 		 -11 -8
//	1 : +24+11+11-11 -8
//	h->x256 = obj_player->x256+cos512((src->PL_OPTION_DATA_angle512))*(16)/*src->PL_OPTION_DATA_length*/+((/*obj_player->w*/24-6-6)*src->PL_OPTION_DATA_opt_anime_add_id)+6-8;
// -1 : 		 +6 -8
//	1 : +24 -6 -6 +6 -8
//	h->x256 = obj_player->x256+cos512((src->PL_OPTION_DATA_angle512))*(16)/*src->PL_OPTION_DATA_length*/+((/*obj_player->w*/(24)-6-6)*src->PL_OPTION_DATA_opt_anime_add_id)+6-((h->w)>>1)/*(8)*/;
//
	//	h->x256 = obj_player->x256+cos512((src->PL_OPTION_DATA_angle512))*(16)/*src->PL_OPTION_DATA_length*/+(((int)(src->PL_OPTION_DATA_pos+1)*obj_player->w-h->w)/2)-src->PL_OPTION_DATA_pos*6;
	//	h->x256 = obj_player->x256+cos512((src->PL_OPTION_DATA_angle512))*(16)/*src->PL_OPTION_DATA_length*/+(((int)(src->PL_OPTION_DATA_opt_anime_add_id+1)*/*player->w*/(24)-h->w)/2)-src->PL_OPTION_DATA_opt_anime_add_id*6; 		//2598187
	//	h->x256 = obj_player->x256+cos512((src->PL_OPTION_DATA_angle512))*(16)/*src->PL_OPTION_DATA_length*/+ (((24/*obj_player->w*/ /2)-6)*(src->PL_OPTION_DATA_opt_anime_add_id)) -((h->w)>>1) +(24/*player->w*/ /2); //2598219

		/* -1: 6*256  1: 18*256 */
	//	h->x256 = obj_player->x256+cos512((src->PL_OPTION_DATA_angle512))*(16)/*src->PL_OPTION_DATA_length*/+ (((6)*(2+src->PL_OPTION_DATA_opt_anime_add_id))<<8) -((h->w128)); //2598187 2079061

	{int hankei;
	//	hankei = (YUYUKO==select_player)?(32):(16);
		hankei = (YUYUKO==select_player)?(32):(24);
		/* CCWの場合 */
		#if 0/*SDL(左隅座標)*/
		src->x256 = obj_player->x256+sin1024((src->PL_OPTION_DATA_angleCCW1024))*(hankei)/*src->PL_OPTION_DATA_length*/+ (18*256)-(((src->PL_OPTION_DATA_opt_anime_add_id&2)<<8)*6) -((h->w128)); // 2079061 (4オプション対応)
		int aaa;
		aaa = cos1024((src->PL_OPTION_DATA_angleCCW1024))*(hankei)/*src->PL_OPTION_DATA_length*/+((obj_player->h128-h->h128))-t256(5.0);
		aaa = (REIMU_A==select_player)?(-aaa):(aaa);
		src->y256 = obj_player->y256+aaa;
		#else/*Gu(中心座標)*/
	//	src->x256 = obj_player->x256+sin1024((src->PL_OPTION_DATA_angleCCW1024))*(hankei)/*src->PL_OPTION_DATA_length*/+ (18*256)-(((src->PL_OPTION_DATA_opt_anime_add_id&2)<<8)*6);			// 2079061 (4オプション対応)
		src->x256 = obj_player->x256+sin1024((src->PL_OPTION_DATA_angleCCW1024))*(20/*hankei*/)/*src->PL_OPTION_DATA_length*/+ (18*256)-(((src->PL_OPTION_DATA_opt_anime_add_id&2)<<8)*6);	// 2079061 (4オプション対応)
		int aaa;
	//	aaa = cos1024((src->PL_OPTION_DATA_angleCCW1024))*(hankei)/*src->PL_OPTION_DATA_length*/ -t256(5.0);
	//	aaa = (REIMU_A==select_player)?(-aaa):(aaa);
	//	h->y256 = obj_player->y256+aaa;
		aaa = cos1024((src->PL_OPTION_DATA_angleCCW1024))*(hankei)/*src->PL_OPTION_DATA_length*/;
		aaa = (REIMU_A==select_player)?(-aaa):(aaa);
		src->y256 = obj_player->y256+aaa +t256(10.0);
		#endif
	}
}

/*---------------------------------------------------------
	プレイヤー、オプションの移動(霊夢、魔理沙、仮幽々子)
---------------------------------------------------------*/

/*
離す: [51]	[461=256+128+64+16-3]
押す: [  ]	[314=256	+64   -6]
*/

//#define BASE_OPT_SHOT_ANIME	(PLAYERS8*3)
//	 12,  12,	6,	 6,    3,  yuuyko(12),	 CIRNO(3),	 CIRNO(3),	/* オプションショットのアニメーション速度 */

#define animate_option_oz_ti animate_option_all
#define animate_option_re_ma_yu animate_option_all
//static void animate_option_oz_ti(SPRITE *src)
//static void animate_option_re_ma_yu(SPRITE *src)
static void animate_option_all(SPRITE *src)
{
#if 1
	if (0==player_fix_status[BASE_OPT_ANIM_TYPE+select_player])
	{
		/* 描画用グラ回転 */
		/* 描画用角度(下が0度で左回り(反時計回り)) */
		src->m_angleCCW1024 += (2+2);/*...とりあえず*/
		mask1024(src->m_angleCCW1024);
	}
	else
//	if (/*YUYUKO==select_player*/)
//	if ((CIRNO_A==select_player)||(CIRNO_Q==select_player))
	{
		src->PL_OPTION_DATA_anime_wait--;
		if (src->PL_OPTION_DATA_anime_wait<1)
		{
//			src->PL_OPTION_DATA_anime_wait=/*(anime_speed)*/(12);/*(12)*/ /*2*/ /*陰陽だまはゆっくり回るよ*/	//yuyuko
//			src->PL_OPTION_DATA_anime_wait=/*(anime_speed)*/(3);/*12*/ /*2*/ /*陰陽だまはゆっくり回るよ*/		//cirno
			src->PL_OPTION_DATA_anime_wait=/*(anime_speed)*/(player_fix_status[BASE_OPT_ANIM_SPEED+select_player]);/*12*/ /*2*/ /*陰陽だまはゆっくり回るよ*/
		#if 0
		//	src->type=((src->type+(src->PL_OPTION_DATA_opt_anime_add_id)+(src->PL_OPTION_DATA_opt_anime_add_id)-1/*src->PL_OPTION_DATA_pos*/)&(8-1)/*%8*/);
		//	src->type=((src->type+(src->PL_OPTION_DATA_opt_anime_add_id))&(0xfff7)/*%8*/);
			src->type=((src->type+(1))&(0xfff7)/*%8*/);
		//	if (src->type<0)		{	src->type=7;}
		#endif
		#if 0
		//	src->an im_frame=((src->an im_frame+(src->PL_OPTION_DATA_opt_anime_add_id)+(src->PL_OPTION_DATA_opt_anime_add_id)-1/*src->PL_OPTION_DATA_pos*/)&(8-1)/*%8*/);
			src->an im_frame=((src->an im_frame+(src->PL_OPTION_DATA_opt_anime_add_id))&(8-1)/*%8*/);
		//	if (src->an im_frame<0) 	{	src->an im_frame=7;}
		#endif
			#if 0
			src->an im_frame++;
			src->an im_frame &= (8-1);//if (src->an im_frame==8)	{src->an im_frame=0;}
			#endif

		#if 1
		//	src->type = ((src->type) & (0xfff8))|((src->type+1) & (8-1));//if (src->an im_frame==8) 	{src->an im_frame=0;}
			src->type++;
			src->type &= (0xfff7);//if (src->type==8)	{src->type=0;}
		#endif
		}
	}
#endif
}


/*---------------------------------------------------------
	霊夢 	魔理沙	仮幽々子
---------------------------------------------------------*/

static void re_ma_yu_move_option(SPRITE *src)
{
	/* オプションが非表示の場合、何もしない。 */
	if (0==(src->flags & ( SP_FLAG_OPTION_VISIBLE)))	{	return; 	}
//
	// アニメーション
	animate_option_re_ma_yu(src);
//
	if (pd_state_flag & STATE_FLAG_15_KEY_SHOT)
	{
		{
			{
				src->PL_OPTION_DATA_opt_shot_interval--;
				if (src->PL_OPTION_DATA_opt_shot_interval<0)
				{
					src->PL_OPTION_DATA_opt_shot_interval=player_fix_status[BASE_OPT_SHOT_INTERVAL0+(weapon_level_offset)];/* (霊夢 魔理沙	仮幽々子)オプションのショット間隔 */
					#if 0//1975613
					static /*const*/ void (*ggg[PLAYERS8])(SPRITE *sss) =
					{
						/*REIMU_A*/ 	shot_regist_re_yudou,	/* 霊夢 A(誘導赤札) */
						/*REIMU_B*/ 	shot_regist_ma_gggg,	/* 霊夢 B(針) */	/*re_bbbb*/
						/*MARISA_A*/	shot_regist_re_yudou,	/* 魔理沙 A(誘導森弾) */
						/*MARISA_B*/	shot_regist_ma_lazer,	/* 魔理沙 B(きまぐれレーザー) */
						/*REMILIA*/ 	shot_regist_ma_gggg,	/* 現在ダミー */
						/*YUYUKO*/		shot_regist_yu_gggg,	/* 幽々子(蝶) */
						/*CIRNO_A*/ 	shot_regist_ma_gggg,	/* 現在ダミー */
						/*CIRNO_Q*/ 	shot_regist_ma_gggg,	/* 現在ダミー */
					};
					(ggg[select_player])(src);
					#else//1975597
					switch (select_player)
					{
					case MARISA_A:	//	shot_regist_re_yudou(src);	break;	/* 魔理沙 A(誘導森弾) */
					case REIMU_A:		shot_regist_re_yudou(src);	break;	/* 霊夢(誘導赤札) */
					case YUYUKO:		shot_regist_yu_gggg(src);	break;	/* 幽々子(蝶) */
					case MARISA_B:		shot_regist_ma_lazer(src);	break;	/* 魔理沙B(きまぐれレーザー) */
						#if 0
				//	case REIMU_B:		shot_regist_ma_gggg(src);	break;	/* 霊夢(針) */		/*re_bbbb*/
						#endif
					default:			shot_regist_ma_gggg(src);	break;	/* 魔理沙A */
					}
					#endif
				}
			}
		}
	}
	re_ma_yu_control_option(src);
}

/*---------------------------------------------------------
	プレイヤー、オプションの定義
---------------------------------------------------------*/

/* [***090128 [***090220	追加:レミリア用オプション.c3,c4 */

/*---------------------------------------------------------
	プレイヤー、オプションの移動(レミリア ＆ チルノ)
---------------------------------------------------------*/

enum	/* _remilia_move_state_ */
{
	RS00 = 0,
	RS01,
	RS02,
};
enum	/* _remilia_formation_ */
{
	FORMATION_00 = 0,
	FORMATION_01,
	FORMATION_02,
	FORMATION_03,
};
/*
	目標:st ate1
	FORMATION_00: レミリアの直前に四つ / チルノの直後に四人
	FORMATION_01: レミリアの周りを回るよ
	FORMATION_02: 画面後方から支援するよ (y=250の位置に散らばる)
	FORMATION_03: 減速時用(好きな位置に置けるよ) (レミリアに重なるように。slowを押すと段階的にoption停止)
*/

static void oz_ci_move_option(SPRITE *src) /* レミリア ＆ チルノ	[***090220 追加 */
{
	/* オプションが非表示の場合、何もしない。 */
	if (0==(src->flags & ( SP_FLAG_OPTION_VISIBLE)))	{	return; }
//
	/* アニメーション */
	animate_option_oz_ti(src);
//
	static int state_zz1=0;
	/* 攻撃支援 */
	if (pd_state_flag & STATE_FLAG_15_KEY_SHOT)
	{
		{
			{
				src->PL_OPTION_DATA_opt_shot_interval--;
				if (src->PL_OPTION_DATA_opt_shot_interval<0)
				{
				//	src->PL_OPTION_DATA_opt_shot_interval=(5+3);/* (レミリア ＆ チルノ)オプションのショット間隔 */
					src->PL_OPTION_DATA_opt_shot_interval=player_fix_status[BASE_OPT_SHOT_INTERVAL0+(weapon_level_offset)];/* (霊夢 魔理沙	仮幽々子)オプションのショット間隔 */
					switch (src->PL_OPTION_DATA_opt_anime_add_id)
					{
					case OPTION_C1:
					case OPTION_C2:
/*
 WEAPON_L0(P000-P008)
 WEAPON_L1(P008-P015)
 WEAPON_L2(P016-P031)
 WEAPON_L3(P032-P063)
 WEAPON_L4(P064-P127)
 WEAPON_L5(P128)
*/
					//	if (pd_weapon_power > (100-1))	/* 旧システムの調整値 100 */
						if (pd_weapon_power > (96)) 	/* システムが変わったので修正してみる(96==128-32) */
						{
							/*shot_regist_needle*/shot_regist_ci_gggg(src); /*t256( 0), t256( 0),*/
				//			src->PL_OPTION_DATA_opt_shot_interval=(7+3);
						}
						else
						{
							/*shot_regist_needle*/shot_regist_ma_gggg(src); /*t256( 0), t256( 0),*/
				//			src->PL_OPTION_DATA_opt_shot_interval=(5+3);
						}
						break;
					case OPTION_C3:
					case OPTION_C4:
						/*shot_regist_needle*/shot_regist_ma_gggg(src); /*t256( 0), t256( 0),*/
				//		src->PL_OPTION_DATA_opt_shot_interval=(5+3);
						break;
					}
				}
			}
		}
	}
	//
	int slow_flag;
	slow_flag=0;
	/* 低速移動 */
	if (my_pad & PSP_KEY_SLOW)
	{
		if (REMILIA==select_player)
		{
			slow_flag=1;
		}
	//	else
	//	{
	//		/* チルノ */
	//	}
	}
	int player_offs_x256;
	int player_offs_y256;
	#if 0/*SDL(左隅座標)*/
	player_offs_x256 = obj_player->x256+((obj_player->w128-src->w128));
	player_offs_y256 = obj_player->y256+((obj_player->h128-src->h128));
	#else/*Gu(中心座標)*/
	player_offs_x256 = obj_player->x256;
	player_offs_y256 = obj_player->y256;
	#endif
//	if (1==slow_flag)	/* pspは0レジスタがあるので0と比較したほうが速い */
	if (0!=slow_flag)	/* 少なくとも低速移動でオプションが止められるのはレミリアのみ(チルノには合わない) */
	{
		/* レミリア用 */
		src->PL_OPTION_DATA_slow_count += (1<<2);
		if (( state_zz1==FORMATION_03) && /* FORMATION_03: 減速時用(好きな位置に置けるよ) */
			(src->PL_OPTION_DATA_slow_count < ((src->PL_OPTION_DATA_opt_anime_add_id+1)<<(6+2))/* *30 64*/))
		{
			src->x256 = player_offs_x256;
			src->y256 = player_offs_y256;
		}
		else if ( state_zz1==FORMATION_02) /* FORMATION_02: 画面後方から支援するよ */
		{
			switch (src->PL_OPTION_DATA_opt_anime_add_id)
			{
			#if 0/*SDL(左隅座標)*/
			case OPTION_C1: if (src->x256 < t256(170)-((src->w128+src->w128)))	{src->x256 += t256(2.0);} break;	/*fps_factor*/
			case OPTION_C2: if (src->x256 > t256(210))							{src->x256 -= t256(2.0);} break;	/*fps_factor*/
			case OPTION_C3: if (src->x256 < t256(100)-((src->w128+src->w128)))	{src->x256 += t256(4.0);} break;	/*fps_factor*/
			case OPTION_C4: if (src->x256 > t256(280))							{src->x256 -= t256(4.0);} break;	/*fps_factor*/
			#else/*Gu(中心座標)*/
			case OPTION_C1: if (src->x256 < t256(170))	{src->x256 += t256(2.0);} break;	/*fps_factor*/
			case OPTION_C2: if (src->x256 > t256(210))	{src->x256 -= t256(2.0);} break;	/*fps_factor*/
			case OPTION_C3: if (src->x256 < t256(100))	{src->x256 += t256(4.0);} break;	/*fps_factor*/
			case OPTION_C4: if (src->x256 > t256(280))	{src->x256 -= t256(4.0);} break;	/*fps_factor*/
			#endif
			}
		}
	}
	else
	{
		/* レミリア用 */
		if (src->PL_OPTION_DATA_slow_count)
		{
			src->PL_OPTION_DATA_slow_count=0;
			src->PL_OPTION_DATA_state2=0;
		}
	//
		/* フォーメーション変更 */
		/* OPTION_C1 の場合のみキー入力受け付け */
		if (OPTION_C1==src->PL_OPTION_DATA_opt_anime_add_id) //opt_anime_add_id==1によるflag管理
		{
		//	if (my_pad & PSP_KEY_OPTION)
		/* (オプションキーキーを離した瞬間なら) */
			if ((0==(my_pad & PSP_KEY_OPTION))) 	/* オプションキーが離されている */
			{
				if ( (my_pad^my_pad_alter) & PSP_KEY_OPTION)	/* オプションキーの状態が変わった(トグル、押したまたは離した場合) */
				{
	#if 0/*デバッグ用*/
	/*	★「ボスと相打ちするとハングアップ」バグ(～r26)対策 */
	/*(ボス倒す時に喰らいボム状態?だとstartすら効かなくなるバグがある。がオプションキーは効く)*/
	/* パネルのスコア欄にmove_typeを、グレイズ欄にstate_zz1を表示させる。っていうか書き換えちゃう。 */
	/*
	my_score0000460960 == 0xb410
	0x1
		STATE_FLAG_05_IS_BOSS						(0x0010)
	0x4
		ST ATE_FLAG_11_IS_BOSS_DESTROY				(0x0400)
	0xb
		STATE_FLAG_13_DRAW_BOSS_GAUGE				(0x1000)
		STATE_FLAG_14_GAME_LOOP_QUIT				(0x2000)
		STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL 			(0x8000)
	 */
	pd_my_score 	= pd_state_flag/*spell_card_number*/;
	pd_graze_point	= state_zz1;
	#endif
					state_zz1++;
					if ( state_zz1==(FORMATION_03+1) )
					{	 state_zz1=FORMATION_00;}
				//
					src->PL_OPTION_DATA_state2=0;					/* OPTION_C1 の state2 */
					option[OPTION_C2]->PL_OPTION_DATA_state2=0; 	/* OPTION_C2 の state2 */
					option[OPTION_C3]->PL_OPTION_DATA_state2=0; 	/* OPTION_C3 の state2 */
					option[OPTION_C4]->PL_OPTION_DATA_state2=0; 	/* OPTION_C4 の state2 */
				}
			}
		}
		switch (src->PL_OPTION_DATA_state2)
		{
		case RS00:	/* 初期化 */
			{
				enum
				{
					FORMATION_01_ADD_R = 0, 		/* 0 */
					FORMATION_01_RADIUS_R,			/* 1 */
					FORMATION_02_LOCATE_X,			/* 2 */
					FORMATION_02_LOCATE_Y,			/* 3 */
					FORMATION_00_LOCATE_Y_REMILIA,	/* 4 */ 	/* == 4 == (旧2) == REMILIA プレイヤー番号に合わせている事に注意 */
					FORMATION_00_LOCATE_X,			/* 5 */
					FORMATION_00_LOCATE_Y_CIRNO_A,	/* 6 */ 	/* == 6 == (旧3) == CIRNO	プレイヤー番号に合わせている事に注意 */
					FORMATION_00_LOCATE_Y_CIRNO_Q,	/* 7 */ 	/* == 7 == (旧3) == CIRNO	プレイヤー番号に合わせている事に注意 */
					FORMATION_MAX					/* 8 */ 	/* 最大数 */
				};
				//	{	t256(15),  t256(-15),	t256(25),  t256(-25) }, 				/* FORMATION_00:			旧FORMATION_00_LOCATE_X */
				//	{	t256(15),  t256( 15),	t256(20),  t256( 20) }, 				/* FORMATION_00:			旧FORMATION_00_LOCATE_Y */
//				//	{	( 1*115+10),  ( 2*115+10),	( 0*115+10),  ( 3*115+10) },		/* FORMATION_02:			画面後方から支援するよ(wideth380dot) */
				static const s16 fff[FORMATION_MAX][4] =
				{
					{	( 1),  (  2),	(-3),  ( -6) }, 								/* 0 FORMATION_01_ADD_R:		回転方向、角度加算値 */
					{	(20),  ( 30),	(40),  ( 50) }, 								/* 1 FORMATION_01_RADIUS_R: 回転半径 */
					{	( 1*104+20),  ( 2*104+20),	( 0*104+20),  ( 3*104+20) },		/* 2 FORMATION_02_LOCATE_X: 画面後方から支援するよ(wideth352dot) */
					{  (240),  (240),  (220),  (220) }, 								/* 3 FORMATION_02_LOCATE_Y: 画面後方から支援するよ */
					{	t256(12-33),  t256( 12-33), t256(26-33),  t256( 26-33) },		/* 4 FORMATION_00_LOCATE_Y_REMILIA: レミリアの直前に四つ FORMATION_00_LOCATE_Y_REMILIA */	/* 2: レミリア用 */ 	/* 前方配置 */
					{	t256(14+12-11-1),	t256(14-12-1), t256(14+28-11-1),  t256(14-28-1) },	/* 5 FORMATION_00_LOCATE_X: レミリアの直前に四つ FORMATION_00_LOCATE_X */
					{	t256(20),  t256( 20),	t256(25),  t256( 25) }, 				/* 6 FORMATION_00_LOCATE_Y_CIRNO_A: 	  チルノの直後に四人 FORMATION_00_LOCATE_Y_CIRNO */ /* 3: チルノ用 */
					{	t256(20),  t256( 20),	t256(25),  t256( 25) }, 				/* 7 FORMATION_00_LOCATE_Y_CIRNO_Q: 	  チルノの直後に四人 FORMATION_00_LOCATE_Y_CIRNO */ /* 3: チルノ用 */
				};
				switch ( state_zz1)
				{
				case FORMATION_00:	/* FORMATION_00: レミリアの直前に四つ / チルノの直後に四人 */
					src->PL_OPTION_DATA_offset_x256=((fff[FORMATION_00_LOCATE_X][(src->PL_OPTION_DATA_opt_anime_add_id)]));
					src->PL_OPTION_DATA_offset_y256=((fff[select_player/*FORMATION_00_LOCATE_Y*/][(src->PL_OPTION_DATA_opt_anime_add_id)]));	/* プレイヤー番号に合わせている事に注意 */
					/* レミリア用 */
				//	if (REMILIA==select_player)
				//	{
				//		src->PL_OPTION_DATA_offset_y256 -= t256(32);/* 前方配置 */
				//	}
					break;
				case FORMATION_01:	/* FORMATION_01: レミリアの周りを回るよ */
					src->PL_OPTION_DATA_offset_x256 = fff[FORMATION_01_RADIUS_R][(src->PL_OPTION_DATA_opt_anime_add_id)];/*40*/ /* 円の半径と兼用 */
					src->PL_OPTION_DATA_offset_y256 = fff[FORMATION_01_ADD_R ][(src->PL_OPTION_DATA_opt_anime_add_id)]; 		/* 円の回転方向、角度加算値と兼用 */
					break;
				case FORMATION_02:	/* FORMATION_02: 画面後方から支援するよ */
					src->PL_OPTION_DATA_offset_x256=((fff[FORMATION_02_LOCATE_X][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);
					src->PL_OPTION_DATA_offset_y256=((fff[FORMATION_02_LOCATE_Y][(src->PL_OPTION_DATA_opt_anime_add_id)])<<8);/*240*/
					break;
				case FORMATION_03:	/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
					break;
				}
			}
			src->PL_OPTION_DATA_state2=1;	/* 次へ */
			break;
		case RS01:	/* 目標地点へ移動中 */
			#define oz_offset_r 	(src->PL_OPTION_DATA_offset_x256)	/* FORMATION_01: 円の半径と兼用 */
			#define oz_offset_add_r (src->PL_OPTION_DATA_offset_y256)	/* FORMATION_01: 円の回転方向、角度加算値と兼用 */
			{
			int delta_y256=0;				/* delta_y : y座標における目標地点と現在地の差 */
			int delta_x256=0;				/* delta_x : x座標における目標地点と現在地の差 */
				switch ( state_zz1)
				{
				case FORMATION_00:	/* FORMATION_00: レミリアの直前に四つ / チルノの直後に四人 */
					delta_y256 = player_offs_y256-src->y256 + src->PL_OPTION_DATA_offset_y256;
					delta_x256 = player_offs_x256-src->x256 + src->PL_OPTION_DATA_offset_x256;
					break;
				case FORMATION_01:	/* FORMATION_01: レミリアの周りを回るよ */
#if 0
					/* レミリア用 */
					if (REMILIA==select_player)
					{
						/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024 += oz_offset_add_r;	/*5==ra d2deg512(0.05)*/	/*fps_factor*/
						mask1024(/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024);
					}
#endif
					delta_x256 = player_offs_x256-src->x256 + (sin1024((src->PL_OPTION_DATA_angleCCW1024/*REMILIA_angle512*/))*oz_offset_r);		/* CCWの場合 */
					delta_y256 = player_offs_y256-src->y256 + (cos1024((src->PL_OPTION_DATA_angleCCW1024/*REMILIA_angle512*/))*oz_offset_r);
					break;
				case FORMATION_02:	/* FORMATION_02: 画面後方から支援するよ */
					#if 0/*SDL(左隅座標)*/
					delta_y256 = (-((src->h128))-src->y256+src->PL_OPTION_DATA_offset_y256);/*240*/
					delta_x256 = (-((src->w128))-src->x256+src->PL_OPTION_DATA_offset_x256);
					#else/*Gu(中心座標)*/
					delta_y256 = (-src->y256+src->PL_OPTION_DATA_offset_y256);/*240*/
					delta_x256 = (-src->x256+src->PL_OPTION_DATA_offset_x256);
					#endif
					break;
				case FORMATION_03:	/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
					delta_y256 = player_offs_y256-src->y256;
					delta_x256 = player_offs_x256-src->x256;
					break;
				}
				{	/* 目標地点に移動する */
					int ccc_angle1024;
					ccc_angle1024 = atan_1024(((delta_y256)>>8), ((delta_x256)>>8));
					src->x256 += ((sin1024(( ccc_angle1024))*/*10*/((REMILIA==select_player)?t256(8.0):t256(1.0)))>>8);/*fps_factor*/	/* CCWの場合 */
					src->y256 += ((cos1024(( ccc_angle1024))*/*10*/((REMILIA==select_player)?t256(8.0):t256(1.0)))>>8);/*fps_factor*/
				}
#if 1
				/* レミリア用 */
				if (REMILIA==select_player)
				{
					/* 目標付近までたどり着いたら */
					if (( t256(-5) < delta_y256) &&
						(delta_y256 < t256(5)) )
					{
						src->PL_OPTION_DATA_state2=2;	/* 次へ */
					}
				}
#endif
			}
			break;
#if 1
		case RS02:	/* 位置の維持 & 配置完了flag */
			switch ( state_zz1)
			{
			case FORMATION_00:	/* FORMATION_00: レミリアの直前に四つ / チルノの直後に四人 */
				src->x256 = player_offs_x256 + src->PL_OPTION_DATA_offset_x256;
				src->y256 = player_offs_y256 + src->PL_OPTION_DATA_offset_y256;
				break;
			case FORMATION_01:	/* FORMATION_01: レミリアの周りを回るよ */
				/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024 += oz_offset_add_r;	/*5==ra d2deg512(0.05)*/	/*fps_factor*/
				mask1024(/*REMILIA_angle512*/src->PL_OPTION_DATA_angleCCW1024);
				src->x256 = player_offs_x256 + sin1024((src->PL_OPTION_DATA_angleCCW1024/*REMILIA_angle512*/))*oz_offset_r; /* CCWの場合 */
				src->y256 = player_offs_y256 + cos1024((src->PL_OPTION_DATA_angleCCW1024/*REMILIA_angle512*/))*oz_offset_r;
				break;
			case FORMATION_02:	/* FORMATION_02: 画面後方から支援するよ */
				src->x256 = src->PL_OPTION_DATA_offset_x256;
				src->y256 = src->PL_OPTION_DATA_offset_y256;
				break;
			case FORMATION_03:	/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
				src->x256 = player_offs_x256;
				src->y256 = player_offs_y256;
				break;
			}
#endif
			break;
		}
	}
}
//	if (src->flags&SP_FLAG_VISIBLE) /* 表示されていれば上記の処理をするよ。 */	{	}
//


/*---new add-----*/

/* ====================================================================================== */
/* ====================================================================================== */
/* ====================================================================================== */

/*---------------------------------------------------------
	プレイヤーシールド生成(霊夢)
---------------------------------------------------------*/

static void player_create_bomber_kekkai_parrent(SPRITE *src)		/* 霊夢 */		//シールドの追加
{
	pd_state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/
	int ii;//	int i;	/* 半象限ずつ */
	for (ii=0; ii<(8); ii++)//	for (i=0;i<360/*i<=359*/;i+=45)
	{
		SPRITE *h;
		h				= sprite_add_gu_error();
	//	if (NULL!=h)/* 登録できた場合のみ */
		if (NULL==h)return;/* 登録できた場合のみ */
		{
			h->m_Hit256R			= TAMA_ATARI_04;
			if (0 == (ii&1))// if (i%90 == 0)
			{
				h->PL_KEKKAI_DATA_add_r1024 	= ( 46);							/* 時計回り */
				h->PL_KEKKAI_DATA_radius		= (5);		/*(48)*/	//d->radius=38; 	/*半径初期値*/	/* 少しずつ広がるようにした */
			}
			else		/* 1つだと不安なので2つ作った。 */
			{
				h->PL_KEKKAI_DATA_add_r1024 	= (-34);							/* 反時計回り */
				h->PL_KEKKAI_DATA_radius		= (0);		/*(45)*/	//d->radius=35; 	/*半径初期値*/	/* 少しずつ広がるようにした */
			}
	//		if (YUYUKO==select_player)
	//		{	/* 幽々子 */
	//			h->PL_KEKKAI_DATA_radius		+= 45/*45*/;		/*半径初期値*/
	//			h->type 		= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO)/*ボスに無効*/;
	//		}
	//		else // if (REIMU==select_player)
			{	/* 霊夢 */
				h->type 				= (ii&(4-1))+(/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL)/*ボスに有効*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/
			}
			h->callback_mover			= player_move_kekkai;			/* 時計回り＆反時計回り */
		//	h->callback_mover			= player_move_kekkai/*2*/;		/* 反時計回り */
//			h->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
			h->PL_KEKKAI_DATA_angleCCW1024		= (ii*cv1024r((45)));	/*i*/
			/* 出始めはプレイヤー座標 */
			h->x256 					= obj_player->x256;
			h->y256 					= obj_player->y256;
			h->base_weapon_strength 	= (8*1);	/*5*/	/* [***090214 追加 */
		}
	}
}

/*---------------------------------------------------------
	[プレイヤーシールド]ボム生成
	(魔理沙 ＆ チルノ＆ 幽々子)
---------------------------------------------------------*/

static void player_create_bomber_levarie_parrent(SPRITE *src)
{
	pd_state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/
	int jjj;		jjj=0;
	int angCCW1024; angCCW1024 = (0);/* 0 チルノ用(暫定的)*/
	int iii;
	for (iii=0; iii<(18*3)/*16*/; iii++)
	{
		SPRITE *h;
		h					= sprite_add_gu_error();
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
			h->m_Hit256R			= ((base_bomber_atari[select_player])<<8);
			//
			jjj++;
			jjj &= 0x03;	//	if (4==jjj) 	{	jjj=0;	}
	//		h->type 		= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_ZAKO)/*ボスに無効*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)*/ /*ボスに有効*/
			h->type 		= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL)/*ボスに有効*/ /*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/;
			h->type += jjj;
	//

//			h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
			h->callback_mover	= player_move_levarie;
			#if 0
			{
				static const u16 player_fix_status_ggg[8] =
				{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
					256, 256, 256, 256, 32, 256, 32, 32,	/* 描画用ボムの初期サイズ / size of bomb at first. */
				};
				h->m_zoom_x256		= player_fix_status_ggg[select_player]/*8*/;/* 64 == (1/4) */
			}
			#else
			if (YUYUKO!=select_player)
			{
				h->m_zoom_x256		= 1+(ra_nd()&(64-1));	/* 描画用ボムの初期サイズ / size of bomb at first. */
			}
			#endif

			h->base_weapon_strength 	= player_fix_status[BASE_STD_BOMB_STRENGTH+select_player] /*k_strength*/ /*5*/ /*15-k-k*/;/* [***090214 追加 */
			/* MARISAは強いが拡散してしまうのであたらない。  */
			/* チルノは当てやすいのでその分弱くしないと強すぎちゃう。  */
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
			#if 0/*SDL(左隅座標)*/
			h->x256 			= src->x256+((src->w128-h->w128));
			h->y256 			= src->y256+((src->h128-h->h128));
			#else/*Gu(中心座標)*/
			h->x256 			= src->x256;
			h->y256 			= src->y256;
			#endif
			/* ギャストドリーム(もどき)。スターダストレヴァリエと違い近接性能は無い。 */
			if (YUYUKO==select_player)	/* 幽々子 */
			{
				h->x256 		+= ((h->vx256)<<5);
				h->y256 		+= ((h->vy256)<<5);
	//			/* 幽々子 強すぎるからな～(効果弱いが効果あるのも出来るようにシステム見直したほうがいいかも) */
			}
			#if 1
			/* 描画用角度(下が0度で左回り(反時計回り)) */
			h->m_angleCCW1024		= angCCW1024;
			#endif
		//	angCCW1024 += (int)((1024)/16); // 22.5度/360, 2xπ/16 /* 0.392699081698724154810 */
			angCCW1024 += (int)((1024)/18); // 20.0度/360, 2xπ/18 /* 0.349065850398865915384 */
		}
	}
}


/*---------------------------------------------------------
	魔符「スターダストレヴァリエ」
	-------------------------------------------------------
	[プレイヤーシールド]ボム生成(魔理沙 A)
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

static void player_create_bomber_star_dust_levarie_parrent(SPRITE *src)
{
	pd_state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/
//
	int angCCW1024; angCCW1024 = (0);/* 0 チルノ用(暫定的)*/
	int iii;
	for (iii=0; iii<(8)/*16*/; iii++)/*８方向*/
	{
		int kkk;
		for (kkk=0; kkk<(3)/*16*/; kkk++)/*3サイズ*/
		{
			SPRITE *h;
			h					= sprite_add_gu_error();
		//	if (NULL!=h)/* 登録できた場合のみ */
			if (NULL==h)return;/* 登録できた場合のみ */ 	/*うーん？？？*/
			{
			//	h->m_Hit256R			= ((16)<<8);//((base_bomber_atari[select_player])<<8);
				h->m_Hit256R			= ((32)<<8);//((base_bomber_atari[select_player])<<8);
				const static u32 color_mask32[8] =
				{	//         RED   GREEN BLUE  ALPHA
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
		//		h->type 					= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_ZAKO)/*ボスに無効*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)*/ /*ボスに有効*/
				h->type 					= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_04|SP_GROUP_SHOT_SPECIAL)/*ボスに有効*/ /*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/;
		//
	//			h->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
				h->callback_mover			= player_move_levarie;
				h->m_zoom_x256				= (0x100<<(kkk));	/* 描画用ボムの初期サイズ / size of bomb at first. */
				h->base_weapon_strength 	= player_fix_status[BASE_STD_BOMB_STRENGTH+select_player] /*k_strength*/ /*5*/ /*15-k-k*/;/* [***090214 追加 */
				{
//					const int d_speed256	= t256(1.5);	/* 速度一定 */
					const int d_speed256	= t256(1.5);	/* 速度一定 */
					h->vx256			= ((sin1024((angCCW1024))*(d_speed256))>>8);/*fps_factor*/	/* CCWの場合 */
					h->vy256			= ((cos1024((angCCW1024))*(d_speed256))>>8);/*fps_factor*/
				}//
				#if 1/*Gu(中心座標)*/
				{
				//	const int aaa111	= ((0==kkk)?(t256(48)):(0));	/* 速度一定 */
					h->x256 			= src->x256;//+(aaa111);
					h->y256 			= src->y256;//+(aaa111);
				}
				if (0==kkk)
				{
					h->x256 			+= (t256(48));
					h->y256 			+= (t256(48));
					h->color32			|= (MAKE32RGBA(0, 0, 0, 0xff)); 	/*	h->alpha			= 0xff 255;*/
				}
				#endif
				#if 1
				/* 描画用角度(下が0度で左回り(反時計回り)) */
				h->m_angleCCW1024		= angCCW1024;
				#endif
			}
		}
		angCCW1024 += (int)((1024)/8);	// 45.0度/360, 64
	}
}
/*---------------------------------------------------------
	プレイヤー立ち絵生成
	(幽々子)[幽々子ボムの扇本体]
---------------------------------------------------------*/

static void player_create_bomber_tachie(SPRITE *src)
{
	SPRITE *h;
	h					= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */	/*(?)*/
	{
		h->m_Hit256R			= JIKI_ATARI_ITEM_80;
//		h->flags				|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
		h->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	h->alpha			= 0xff;*/
		h->callback_mover		= player_move_tachie;
		h->type 				= (JIKI_BOMBER_00|SP_GROUP_SHOT_ZAKO);	/*SP_GROUP_JIKI_GET_ITEM*/ /*JI KI_SHOT_00*/	/* ボスの直接攻撃は禁止 */	/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/
		/* 立ち絵初期位置 */
		h->x256 				= -t256(150);	// (200)	/*0*/  /*(src->x256)+((src->w128-h->w128))*/;
		h->y256 				=  t256(150);	// (100)	/*GAME_HEIGHT+1*/ /*(src->y256)+((src->h128-h->h128))*/;
	//
		/* 描画用角度 */
	//	h->m_angleCCW1024		= (256);
		h->m_angleCCW1024		= (0);
	//
	/*???*/ 	h->base_weapon_strength 	= (1/*8*1*/);		/* 扇本体 の強さ */
	//
	}
}

/*---------------------------------------------------------
	プレイヤーシールド生成(レミリア)[レミリアボムの十字炎本体]
---------------------------------------------------------*/

static void player_create_bomber_cross_red_parrent(SPRITE *src) /* レミリア */ /* [***090220 追加 */
{
	pd_state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/
	SPRITE *h;
	h						= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */	/* うーん... */
	{
		h->m_Hit256R		= JIKI_ATARI_ITEM_16;
//		h->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
//		h->flags			&= (~(SP_FLAG_VISIBLE));	/*非表示*/
//		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0xdc);		/*	h->alpha			= 0xdc;*/	/*非表示*/
		h->callback_mover	= player_move_add_cross_red;
	//	h->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO);/* ボスの直接攻撃は禁止 */		/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/
		h->type 			= (/*表示しない*/SP_GROUP_ETC_DUMMY_REMILIA/*SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO*/);/* ボスの直接攻撃は禁止 */		/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/
		#if 0/*SDL(左隅座標)*/
		h->x256 			= (src->x256)+((src->w128-h->w128));
		h->y256 			= (src->y256)+((src->h128-h->h128));
		#else/*Gu(中心座標)*/
		h->x256 			= t256(GAME_NOT_VISIBLE480);//(src->x256);	/*非表示*/
//		h->y256 			= (src->y256);	/*非表示*/
		#endif
	}
}



/*---------------------------------------------------------
	プレイヤー誘導弾生成
---------------------------------------------------------*/

static void player_add_hlaser(SPRITE *src)
{
	int j;	j=(ra_nd()&(2-1));//for (j=0;j<2;j++)
	{
		SPRITE *h_old;	h_old = NULL;
		SPRITE *h;		h = NULL;
		//#define hlaser_NUM_OF_ENEMIES (24)
		#define hlaser_NUM_OF_ENEMIES (12)		/* [***090128		半分にしてみる */
		int i;
		for (i=0; i<hlaser_NUM_OF_ENEMIES; i++)
		#undef hlaser_NUM_OF_ENEMIES
		{
			h_old = h;
			h					= sprite_add_gu_error();
	//	if (NULL!=h)/* 登録できた場合のみ */	/* 強制登録 */
		if (NULL==h)return;/* 登録できた場合のみ */
/* うーん... */
			h->m_Hit256R		= JIKI_ATARI_ITEM_16;
//			h->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)/*ボスに有効*/ /*SP_PL_HLASER*/;
			h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_01|SP_GROUP_SHOT_SPECIAL)/*ボスに有効*/ /*SP_PL_HLASER*/;
			#if 0/*SDL(左隅座標)*/
			h->x256 			= (src->x256) + ((src->w128)) - t256(5);
			h->y256 			= (src->y256) + ((src->h128)) + t256(15);
			#else/*Gu(中心座標)*/
		//	h->x256 			= src->x256 - t256(5);
		//	h->y256 			= src->y256 + t256(15);
			h->x256 			= src->x256;
			h->y256 			= src->y256;
			#endif
		//	h->flags			|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
//			h->flags			&= (~(SP_FLAG_VISIBLE));	/*非表示*/
			if (0==i)
			{
				h->callback_mover	= player_move_parrent_hlaser;
//				h->m_angleCCW1024						= cv1024r(0);
				h->m_angleCCW1024						= (j&1)?(cv1024r(  0+10)):(cv1024r(360-10));
				h->PL_HOMING_DATA_speed256				= t256(12.0);
				h->base_weapon_strength 				= player_fix_status[BASE_LOW_BOMB_STRENGTH+select_player]/*6*/;/*低速ボムの強さ*/
				h->PL_HOMING_DATA_time_out/*range*/ 	= (100);	/*(200)*/
				h->PL_HOMING_DATA_state 				= 0;
				h->target_obj							= obj_player;	/* (h->target_obj==obj_player)==見つからない */
			}
			else
			{
				h->callback_mover	= player_move_kodomo_hlaser/*NULL*/;
				h->target_obj		= h_old;
				h->PL_HOMING_KODOMO_DATA_time_out		= 8;
			}
		}
	}
}

static void player_move_add_bomber_hlaser(SPRITE *src)
{
//	if (0==(((int)pd_bomber_time)&0x3f))
	if (0==(((int)pd_bomber_time)&0x0f))
	{
		/* シナリオ時には誘導弾を追加しない */
		if (0==(pd_state_flag & STATE_FLAG_06_IS_SCRIPT))
		{
			player_add_hlaser(obj_player);
		}
	}
//
	if (0 >= pd_bomber_time)	{	src->type = SP_DELETE;}/* 時間制限でおしまい */
}

/* 低速ボムの親 */
static void player_create_bomber_homing_parrent(SPRITE *src)	/* player_move_add_bomber_hlaser */
{
	pd_state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/* ボムによる自動収集可能 */
//
	SPRITE *h;
	h						= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */	/* うーん... */
	{
		h->m_Hit256R		= JIKI_ATARI_ITEM_16;
	//	h->flags			|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
//		h->flags			&= (~(SP_FLAG_VISIBLE));	/*非表示*/
//		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0xdc);	/*	c->alpha			= 0x80 0xdc;*/	/*非表示*/
		h->callback_mover	= player_move_add_bomber_hlaser;
	//	h->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)/*ボスに有効*/; /* 低速ボムもボスに有効とする */	/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/
		h->type 			= (/*表示しない*/SP_GROUP_ETC_DUMMY_SLOW_BOMB/*SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL*/)/*ボスに有効*/;	/* 低速ボムもボスに有効とする */	/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/
		#if 1/*Gu(中心座標)*/
		h->x256 			= t256(GAME_NOT_VISIBLE480);//src->x256;	/*非表示*/
//		h->y256 			= src->y256;	/*非表示*/
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
	霊夢専用 低速ボム
---------------------------------------------------------*/

static void reimu_add_hlaser(SPRITE *src)
{
//	int j;
//	j = (ra_nd()&(2-1));//for (j=0;j<2;j++)
	{
		//#define hlaser_NUM_OF_ENEMIES (24)
//		#define hlaser_NUM_OF_ENEMIES (12)		/* [***090128		半分にしてみる */
		int i;
	//	for (i=0; i<(12)/*hlaser_NUM_OF_ENEMIES*/; i++)
		for (i=0; i<(16)/*hlaser_NUM_OF_ENEMIES*/; i++)
		{
			SPRITE *h;
			h						= sprite_add_gu_error();
			if (NULL!=h)/* 登録できた場合のみ */
			{
				h->m_Hit256R		= JIKI_ATARI_ITEM_16;
	//			h->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_SPECIAL)/*ボスに有効*/ /*SP_PL_HLASER*/;
				h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_01|SP_GROUP_SHOT_SPECIAL)/*ボスに有効*/ /*SP_PL_HLASER*/;
				#if 0/*SDL(左隅座標)*/
				h->x256 			= (src->x256) + ((src->w128)) - t256(5);
				h->y256 			= (src->y256) + ((src->h128)) + t256(15);
				#else/*Gu(中心座標)*/
			//	h->x256 			= src->x256 - t256(5);
			//	h->y256 			= src->y256 + t256(15);
				h->x256 			= src->x256;
				h->y256 			= src->y256;
				#endif
			//	h->flags			|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
//				h->flags			&= (~(SP_FLAG_VISIBLE));	/*非表示*/
	//			if (0==i)
				{
					h->callback_mover	= move_reimu_musou_fuuin;
	//				h->m_angleCCW1024	= cv1024r(0);
	//				h->m_angleCCW1024	= (0/*j&1*/)?(cv1024r(	0+10)):(cv1024r(360-10));
					h->m_angleCCW1024	= (i<<6);
					#if 0
					/* (多分必要ないけど)念の為マスク */
					mask1024(src->m_angleCCW1024/*src->PL_HOMING_DATA_angleCCW1024*/);
					#endif
					h->PL_HOMING_DATA_speed256				= t256(1.2);
					h->base_weapon_strength 				= player_fix_status[BASE_LOW_BOMB_STRENGTH+select_player]/*6*/;/*低速ボムの強さ*/
				//	h->PL_HOMING_DATA_time_out/*range*/ 	= (100);	/*(200)*/
					h->PL_HOMING_DATA_reimu_musou_id		= (i);	/*(200)*/
					h->PL_HOMING_DATA_state 				= (0);
					h->target_obj							= obj_player;			/* h->target_obj == obj_player;みつからない */
				}
			}
		}
	}
}


/*---------------------------------------------------------
	霊夢専用 低速ボム
---------------------------------------------------------*/

/* 低速ボムの親 */
static void reimu_create_bomber_homing_parrent(SPRITE *src) 	/* reimu_move_add_bomber_hlaser */
{
	pd_state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/* ボムによる自動収集可能 */
//
	reimu_add_hlaser(obj_player);
//
	#if 1
	/* ボム中＆復活中、有効にすると、重複する。 */
	/* 特殊機能 */
	bullets_to_hosi();/* 弾全部、星アイテムにする */
	#endif
}


/*---------------------------------------------------------
	プレイヤーキー操作
---------------------------------------------------------*/

//static int /*bomb_wait*/d->bomber_time;		/* ボムの有効時間 */	//次のボムを出せるまでの時間

extern /*global*/short my_analog_x; /* アナログ量、補正済み */
extern /*global*/short my_analog_y; /* アナログ量、補正済み */
extern void set_bg_alpha(int set_bg_alpha);

static void player_keycontrol(SPRITE *s1)
{
	/*---------------------------------------------------------
		プレイヤーボム関連処理
	---------------------------------------------------------*/
//	if (STATE_FLAG_06_IS_SCRIPT==(pd_state_flag & STATE_FLAG_06_IS_SCRIPT))
//	{
//		;	/* 会話イベント中はボム関連の処理はしない */
//	}
//	else
		/* シナリオ時には誘導弾を追加しない */
	if (0==(pd_state_flag & STATE_FLAG_06_IS_SCRIPT))
	{
	//	if (d->bomber_time>0)
		if (0 < /*bomb_wait*/pd_bomber_time)		/* ボムウェイト処理 */
		{
			//	d->bomber_time-=fps_fa_ctor;
			/*bomb_wait*/pd_bomber_time--/*-=fps_fa_ctor*/;/**/
			if (2 > /*bomb_wait*/pd_bomber_time)
			{
				set_bg_alpha(255);/* 画面を明るくする */
		//	}
		//	else
		//	if (/*bomb_wait*/pd_bomber_time < 190)
		//	{
				pd_state_flag &= (~STATE_FLAG_02_BOMB_AUTO_GET_ITEM);	/* ボムによる自動収集は終わり */
			}
		}
		else
		{
			pd_bomber_time = 0;
		//}
		//if (/*bomb_wait*/d->bomber_time<=0)
		//{
			if (my_pad & PSP_KEY_BOMB_CANCEL)
			{
				if (0 < pd_bombs)
				{
					pd_bombs--;
					pd_used_bomber++;	/* ボム使用回数 */
					voice_play(VOICE07_BOMB, TRACK02_ALEART_IVENT);/*テキトー*/
					set_bg_alpha(50);/* 画面を暗くする */
//					set_bg_alpha(100);/* 画面を暗くする */
//					set_bg_alpha(127);/* 画面を暗くする */
					/* 立ち絵 */
					player_create_bomber_tachie(obj_player);
					//
					static /*const*/ void (*regist_call_table[/*16*/(PLAYERS8*2)])(SPRITE *src) =
					{
						player_create_bomber_kekkai_parrent,					reimu_create_bomber_homing_parrent, 		/* 霊夢 A */
						player_create_bomber_kekkai_parrent,					player_create_bomber_homing_parrent,		/* 霊夢 B */
						player_create_bomber_star_dust_levarie_parrent, 		player_create_bomber_homing_parrent,		/* 魔理沙 A */
						player_create_bomber_levarie_parrent,					player_create_bomber_homing_parrent,		/* 魔理沙 B */
						player_create_bomber_cross_red_parrent, 				player_create_bomber_homing_parrent,		/* レミリア */
						player_create_bomber_levarie_parrent,					player_create_bomber_cross_red_parrent, 	/* 幽々子 */
						player_create_bomber_levarie_parrent,					player_create_bomber_homing_parrent,		/* チルノ A */
						player_create_bomber_levarie_parrent,					player_create_bomber_homing_parrent,		/* チルノ Q */
					};
						int index_aaa;
						index_aaa = ((select_player)+(select_player)+((my_pad & PSP_KEY_SLOW)?1:0));
					{
						(regist_call_table[(index_aaa)])(obj_player);
					}
//	/* 低速ボム */			pd_bomber_time = 200/*320*/ /*400*/ /*800*/;	/* ボムの有効時間(設定無敵時間)[※１仕様が変わったので半分にした] */
//	/* 霊夢 */				pd_bomber_time = 180/*200*/;	/* ボムの有効時間(設定無敵時間) */	/* こちらはボス有効(攻撃型) */
//	/* 魔理沙	チルノ */	pd_bomber_time = 100/*32*/ /*100*/; 		/* ボムの有効時間(設定無敵時間) */
//	/* 扇 有効時間 */		pd_bomber_time = 255/*255==((16*4*2*2)-1)*/;			/* ボムの有効時間(設定無敵時間) */
//	レミリアボム			pd_bomber_time = 0x7e	/* 255==0xff==8発	254==0xfe==7発==0xbf=192 4発==0x7f 150==0x96*/;

					#if 0
					static /*const*/ u8 pd_bomber_time_table[/*16*/(PLAYERS8*2)] =
					{
						180,		180,		/* 180 200 霊夢 A */
						180,		200,		/* 180 200 霊夢 B */
						100,		180,		/* 100 200 魔理沙 A */
						100,		200,		/* 100 200 魔理沙 B */
						0x7e,		255,		/* 0x7e 200レミリア */
						180,		180,		/* 255 200 幽々子 */
						100,		180,		/* 100 200 チルノ A */
						100,		255,		/* 100 200 チルノ Q */
					};
					#endif
					pd_bomber_time		= 180;//pd_bomber_time_table[(index_aaa)];
					//	#define USER_BOMOUT_WAIT (30)
					pd_save_timer		= pd_bomber_time + USER_BOMOUT_WAIT;/*実質無敵時間*/
					/*	実質無敵時間 == (設定無敵時間 + USER_BOMOUT_WAIT)。 	実質無敵時間 == 設定無敵時間 だと、わかりにくいので若干の余裕が必要。 */
					s1->color32 		= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	/*	s1->alpha			= 0x50;*/	/* 半透明 */
					obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	/*	obj_maru->alpha 	= 0x50;*/	/* 半透明 */
					if (PLAYER_STATE_01_HIT_BOMB==pd_player_status)/* 喰らいボム受付中 */
					{
						pd_use_kurai_bomb++;/* 喰らいボム成功 */
					}
					pd_player_status	= PLAYER_STATE_03_SAVE_02;
				//
					kanji_window_clear(); 	/* 漢字ウィンドウの内容を消す。 */
					home_cursor();			/* カーソルをホームポジションへ移動 */
					{
						msg_time = pd_bomber_time;	/*(60*5)*/
						typedef struct
						{
							const char *spell_str_name; 	/* スペカ名称 */
						} aaa_RESOURCE;
						static const aaa_RESOURCE my_aaa_resource[/*16*/(PLAYERS8*2)/*(PLAYERS8*2)*/] =
						{
//	 "feeddccbbaa99887766554433221100",'\0' 	 "feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{"霊符「夢想結界・円」" 			"\n"},	{"祓符「夢想封印・直」" 			"\n"},//霊符「夢想結界・円」
	{"夢符「夢想結界・斬」" 			"\n"},	{"魂符「未作成攻撃」"				"\n"},//夢符「夢想結界・斬」
	{"魔符「スターダストレヴァリエ」"	"\n"},	{"閃符「未作成攻撃」"				"\n"},
	{"恋符「マスタースパーク」" 		"\n"},	{"虹符「未作成攻撃」"				"\n"},
	{"紅符「不夜城デビル」" 			"\n"},	{"民盟「アクマクロス」" 			"\n"},
	{"死符「ギャストリドリーム」"		"\n"},	{"冥符「未作成攻撃」"				"\n"},	/* dream cast. ghastly dream(妖々夢). ghastly dream(霊夢). */
	{"霰符「アイスビュレット」" 		"\n"},	{"氷符「オンザロック」" 			"\n"},	/* 霰(あられ): 雨が凍ったり、溶けかけた雪が再び凍って降る。 */
	{"最強「アタイの拳」"				"\n"},	{"最強「アタイのキック」"			"\n"},	/* 霙符 霜踏 霙(みぞれ): 雨と雪が混ざって降る。 */
						};
						print_kanji000(/*SDL_Surface *drawmap,*/ /*SDL_Rect *rect*/ /*0,*/
							my_aaa_resource[index_aaa].spell_str_name, //	aaa_str[(spell_card_number&15)],
							/*int color_type*/7, /*int wait*/0
						);
					}
				}
			}
		}
	}
//
	if (PLAYER_STATE_01_HIT_BOMB==pd_player_status) return;/* 喰らいボム受付中 */
//


	/*---------------------------------------------------------
		プレイヤー移動関連処理
	---------------------------------------------------------*/
	/*
		256固定小数点で自機の移動量
		1.0dot	 == 256 == t256(1.0),			縦横の場合
		約0.7dot == 181 == t256(1.0/√2.0)		斜めの場合
	*/
	static const s16 jiki_move_length[16][2] =
	{
	/*LDRU*/ /* y x */
	// 斜め移動が速過ぎるのを修正。
	/*0000*/ {	  0,	0},
	/*0001*/ {	  0, -256},/*0 U*/
	/*0010*/ {	256,	0},/*2 R*/
	/*0011*/ {	181, -181},/*1 UR*/
	/*0100*/ {	  0,  256},/*4 D*/
	/*0101*/ {	  0,	0},
	/*0110*/ {	181,  181},/*3 DR*/
	/*0111*/ {	  0,	0},
	/*1000*/ { -256,	0},/*6 L*/
	/*1001*/ { -181, -181},/*7 UL*/
	/*1010*/ {	  0,	0},
	/*1011*/ {	  0,	0},
	/*1100*/ { -181,  181},/*5 DL*/
	/*1101*/ {	  0,	0},
	/*1110*/ {	  0,	0},
	/*1111*/ {	  0,	0},
	};
/* 花映塚会話何たらサイト http://ambriel.hp.infoseek.co.jp/th9/  基本性能比較 移動速度(高速モード)
														[51200/xxxF]
200/50		50F 文										0x0400[1024.000000000000000000000000000000]3.63636363636363636363636363636364
200/55		55F [魔理沙] ミスティア (メディ,  四季) 	0x03a3[ 930.909090909090909090909090909091]3.63636363636363636363636363636364
200/58		58F [チルノ]								0x0373[ 882.758620689655172413793103448276]3.44827586206896551724137931034483
200/61		61F (鈴仙,	ルナサ) てゐ					0x0347[ 839.344262295081967213114754098361]3.27868852459016393442622950819672		[REMILIA]
200/65		65F リリカ									0x0314[ 787.692307692307692307692307692308]3.07692307692307692307692307692308
200/67		67F 小町									0x02fc[ 764.179104477611940298507462686567]2.98507462686567164179104477611940
200/68		68F [霊夢] 咲夜 							0x02f1[ 752.941176470588235294117647058824]2.94117647058823529411764705882353
200/72		72F メルラン								0x02c7[ 711.111111111111111111111111111111]2.77777777777777777777777777777778		[YUYUKO]
200/91		91F 幽香									0x0233[ 562.637362637362637362637362637363]2.19780219780219780219780219780220

吸霊移動速度(低速モード)
200/79		79F 	文									0x0288[ 648.101265822784810126582278481013]2.53164556962025316455696202531646
200/80		80F 	ミスティア							0x0280[ 640.000000000000000000000000000000]2.50000000000000000000000000000000
200/83		83F 	[チルノ]							0x0269[ 616.867469879518072289156626506024]2.40963855421686746987951807228916
200/85		85F 	咲夜								0x025a[ 602.352941176470588235294117647059]2.35294117647058823529411764705882
200/91		91F 	[魔理沙]							0x0233[ 562.637362637362637362637362637363]2.19780219780219780219780219780220
200/109 	109F	(鈴仙,	ルナサ) 					0x01d6[ 469.724770642201834862385321100917]1.83486238532110091743119266055046		[REMILIA]
200/114 	114F	(メディ,  四季) 					0x01c1[ 449.122807017543859649122807017544]1.75438596491228070175438596491228
200/124 	124F	リリカ 小町 						0x019d[ 412.903225806451612903225806451613]1.61290322580645161290322580645161
200/130 	130F	妖夢								0x018a[ 393.846153846153846153846153846154]1.53846153846153846153846153846154
200/136 	136F	メルラン [霊夢] 					0x0178[ 376.470588235294117647058823529412]1.47058823529411764705882352941176
200/145 	145F	てゐ								0x0161[ 353.103448275862068965517241379310]1.37931034482758620689655172413793		[YUYUKO]
200/148 	148F	幽香								0x015a[ 345.945945945945945945945945945946]1.35135135135135135135135135135135

*/
//#define BASE_SPEED_MINIMUM		(PLAYERS8*4)
//#define BASE_SPEED_MAXIMUM		(PLAYERS8*5)
//	 2,  3,  4,  7,  3, 	/* 最低速度 player_speed_minimum */
//	 4,  5,  4,  7,  3, 	/* 最高速度 player_speed_maximum */
	static const signed /*int*/short player_speed256[(PLAYERS8*4)] =
	{
	/* 高速モード(通常時) */								/* 高速モード(ボム発動時) */
	/* REIMU_A */	0x02f1,/* t256(2.94) 2.94140625 		  t256(3.0), */
	/* REIMU_B */	0x02f1,/* t256(2.94) 2.94140625 		  t256(3.0), */
	/* MARISA_A */	0x03a3,/* t256(3.64) 3.63671875 		  t256(2.0), */
	/* MARISA_B */	0x03a3,/* t256(3.64) 3.63671875 		  t256(2.0), */
	/* REMILIA */	0x0347,/* t256(3.28) 3.27734375 		  t256(5.0), */
	/* YUYUKO */	0x02c7,/* t256(2.78) 2.77734375 		  t256(5.0), */
	/* CIRNO_A */	0x0373,/* t256(3.45) 3.44921875 		  t256(4.5), */ 			/* ⑨だから低速の方が速い */
	/* CIRNO_Q */	0x0373,/* t256(3.45) 3.44921875 		  t256(4.5), */ 			/* ⑨だから低速の方が速い */
	/* 低速モード(通常時) */								/* 低速モード(ボム発動時) */
	/* REIMU_A */	0x0178,/* t256(1.47) 1.46875000 t256(2.0) t256(2.0), */
	/* REIMU_B */	0x0178,/* t256(1.47) 1.46875000 t256(2.0) t256(2.0), */
	/* MARISA_A */	0x0233,/* t256(2.20) 2.19921875 t256(2.5) t256(2.0), */
	/* MARISA_B */	0x0233,/* t256(2.20) 2.19921875 t256(2.5) t256(2.0), */
	/* REMILIA */	0x01d6,/* t256(1.84) 1.83593750 t256(2.5) t256(4.0), */
	/* YUYUKO */	0x0161,/* t256(1.38) 1.37890625 t256(2.0) t256(4.5), */
	/* CIRNO_A */	0x0269,/* t256(2.41) 2.41015625 t256(7.0) t256(5.0), */ /* 0x0700 ⑨だから低速の方が速い 0x0400==文高速時==幻想郷最速？ */
	/* CIRNO_Q */	0x0700,/* t256(2.41) 2.41015625 t256(7.0) t256(5.0), */ /* 0x0700 ⑨だから低速の方が速い 0x0400==文高速時==幻想郷最速？ */
	};
	/* 自機速度を決める。 */
	int is_slow;
	is_slow = (my_pad & PSP_KEY_SLOW);
	/*const*/ signed int my_speed = player_speed256[select_player + ((is_slow)?(PLAYERS8):(0))];
	/* 移動量を決める。(移動量 = 自機速度 x アナログキー、デジタルの場合は予めアナログキー移動量に変換してある) */
	short	aaa_my_analog_x = (((my_speed)*(my_analog_x))>>8);
	short	aaa_my_analog_y = (((my_speed)*(my_analog_y))>>8);
	/* 斜めを考慮して移動する。 */
	s1->x256 += ((((signed int)(jiki_move_length[((my_pad&0xf0)>>4)][0]))*(aaa_my_analog_x))>>8);	/*fps_factor*/
	s1->y256 += ((((signed int)(jiki_move_length[((my_pad&0xf0)>>4)][1]))*(aaa_my_analog_y))>>8);	/*fps_factor*/
	/* 画面外に、はみだしたら修正。 */
	#if 0/*SDL(左隅座標)*/
		 if (s1->x256 < t256(0))									{	s1->x256 = t256(0); 									}/* 左チェック */
	else if (s1->x256 > t256(GAME_WIDTH) -((s1->w128+s1->w128)))	{	s1->x256 = t256(GAME_WIDTH) -((s1->w128+s1->w128)); 	}/* 右チェック */
		 if (s1->y256 < t256(0))									{	s1->y256 = t256(0); 									}/* 上チェック */
	else if (s1->y256 > t256(GAME_HEIGHT)-((s1->h128+s1->h128)))	{	s1->y256 = t256(GAME_HEIGHT)-((s1->h128+s1->h128)); 	}/* 下チェック */
	#else/*Gu(中心座標)*/
//		 if (s1->x256 < t256(0))				{	s1->x256 = t256(0); 			}/* 左チェック */
//	else if (s1->x256 > t256(GAME_WIDTH)  ) 	{	s1->x256 = t256(GAME_WIDTH);	}/* 右チェック */
//		 if (s1->y256 < t256(0))				{	s1->y256 = t256(0); 			}/* 上チェック */
//	else if (s1->y256 > t256(GAME_HEIGHT) ) 	{	s1->y256 = t256(GAME_HEIGHT);	}/* 下チェック */
		 if (s1->x256 < t256(0))				{	s1->x256 = t256(0); 			}/* 左チェック */
	else if (s1->x256 > t256(GAME_WIDTH-31)  )	{	s1->x256 = t256(GAME_WIDTH-31); 	}/* 右チェック */
		 if (s1->y256 < t256(0/*+16*/)) 		{	s1->y256 = t256(0/*+16*/);			}/* 上チェック */
	else if (s1->y256 > t256(GAME_HEIGHT-16) )	{	s1->y256 = t256(GAME_HEIGHT-16);	}/* 下チェック */
	#endif
	/* コア移動 */
	{
		SPRITE *s2;
		s2 = obj_maru;
		#if 0/*SDL(左隅座標)*/
		s2->x256 = s1->x256+((s1->w128-s2->w128))	-(t256( 1));	/* -(t256( 1)) とりあえず */
		s2->y256 = s1->y256-((s2->h128+s2->h128))	+(t256(20));	/* +(t256(20)) とりあえず */
		#else/*Gu(中心座標)*/
	//	s2->x256 = s1->x256 -(t256( 1));	/* -(t256( 1)) とりあえず */
	//	s2->y256 = s1->y256 +(t256(20));	/* +(t256(20)) とりあえず */
	//	s2->x256 = (is_slow)?(s1->x256 +(t256((31-5)/2))):(t256(GAME_NOT_VISIBLE480));	/* -(t256( 1)) とりあえず */	/* コアオフセット */
		s2->x256 = s1->x256 +(t256((31-5)/2));	/* -(t256( 1)) とりあえず */	/* コアオフセット */
		s2->y256 = s1->y256 +(t256((31-5)/2));	/* +(t256(20)) とりあえず */	/* コアオフセット */
		#endif
		if (is_slow)	{	s2->type		&= (~(0x08));/*s2->flags		|= ( (SP_FLAG_VISIBLE));*/	}	/* ○、表示 */
		else			{	s2->type		|= ( (0x08));/*s2->flags		&= (~(SP_FLAG_VISIBLE));*/	}	/* ○、非表示 */
	}
	/* MAX時のアイテム自動収集 */
	if (my_pad & PSP_KEY_UP/*PSP_CTRL_UP*/) 	/* 註：斜め上でも回収可能 */ /*&& (s1->y>0)*/
	{
		#if 1
			/* 全キャラ: アイテム上部収集がいつでも可能 */
		#else
		if ((pd_weapon_power==MAX_POWER_IS_128)  //128[***090123 変更 /*max==MAX_POWER_IS_128==「129段階」*/
			/* 魔理沙 特殊能力：アイテム上部収集がいつでも可能 */
			|| (MARISA_A==select_player)
			|| (MARISA_B==select_player)
		//	|| (MARISA_C==select_player)
		)	/* 魔理沙は常に上部自動収集が可能 */
		#endif
		{
			if (s1->y256 < PLAYER_SPLIT_LINE256/*t256(50)*/)/* [FULLパワー時のアイテム引き寄せライン] */
			{
				pd_state_flag |= STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM; /* 上部自動収集可能 */
			}
		}
	}
	else if (my_pad & PSP_KEY_DOWN/*PSP_CTRL_DOWN*/)	/* 註：斜め下でもやめる */ /*&& (s1->y<screen->h*-s1->h)*/
	{
		//if (pd_state_flag & (STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM /*| STATE_FLAG_02_BOMB_AUTO_GET_ITEM*/) )/*???たぶん*/
		{
			pd_state_flag &= (~STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM);	/* 上部自動収集不可 */
		}
	}
//
		/*---------------------------------------------------------
			プレイヤーショット関連処理
		---------------------------------------------------------*/
	/* シナリオスクリプトモードではショットボタン入力無効 */
	if (0==(pd_state_flag & STATE_FLAG_06_IS_SCRIPT))
	{
		if (my_pad & PSP_KEY_SHOT_OK)
		{
			pd_state_flag |= STATE_FLAG_15_KEY_SHOT;	/* on */
			//
			if (
				(MARISA_B==select_player) &&	/* 魔理沙B(恋符) */
				(0<pd_bomber_time)				/* マスタースパーク中 */
			)
			{
				;/* マスタースパーク中はショットが撃てないらしい */
			}
			else
			{
				{static int weapon_interval=0;
					/*pd_*/weapon_interval--/*=fps_fa_ctor*/;
					if (/*pd_*/weapon_interval < 1 )
					{
						static const u8 weapon_tbl[(WEAPON_L_MAX)*(PLAYERS8)] =
						{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */    /* レミリア強すぎるので調整(+5) */
								 5, 5,	6, 6,  7+5,  9+8,  9,  9,	/* WEAPON_L0(P000-P008) */
								 5, 5,	6, 6,  6+5,  8+8,  9,  9,	/* WEAPON_L1(P008-P015) */
								 5, 5,	5, 5,  7+5,  7+8,  9,  9,	/* WEAPON_L2(P016-P031) */
								 5, 5,	5, 5,  6+5,  6+8,  9,  9,	/* WEAPON_L3(P032-P063) */
								 5, 5,	5, 5,  5+5,  5+8,  9,  9,	/* WEAPON_L4(P064-P127) */
								 5, 5,	5, 5,  5+5,  5+8,  9,  9,	/* WEAPON_L5(P128)		*/
						};
						/*pd_*/weapon_interval = weapon_tbl[(weapon_level_offset)];
//						/*pd_*/weapon_interval = weapon_tbl[(weapon_List<<3)+select_player];
//
						voice_play(VOICE00_SHOT, TRACK00_BULLETS);
						static /*const*/ void (*bbb[(WEAPON_L_MAX)*(PLAYERS8)])(SPRITE *sss) =
						{
	/*REIMU_A*/ 	/*REIMU_B*/ 	/*MARISA_A*/	/*MARISA_B*/	/*REMILIA*/ 	/*YUYUKO 無段階成長 */	/*CIRNO_A*/ 	/*CIRNO_Q*/
	shot_regist_1,	shot_regist_1,	shot_regist_1,	shot_regist_1,	shot_regist_1,	shot_regist_chou,		shot_regist_2,	shot_regist_2,	/* WEAPON_L0:(P000-P008) */
	shot_regist_1,	shot_regist_1,	shot_regist_2,	shot_regist_2,	shot_regist_1,	shot_regist_chou,		shot_regist_2,	shot_regist_2,	/* WEAPON_L1:(P008-P015) */
	shot_regist_2,	shot_regist_2,	shot_regist_2,	shot_regist_3,	shot_regist_2,	shot_regist_chou,		shot_regist_2,	shot_regist_3,	/* WEAPON_L2:(P016-P031) */
	shot_regist_3,	shot_regist_3,	shot_regist_3,	shot_regist_4,	shot_regist_2,	shot_regist_chou,		shot_regist_3,	shot_regist_3,	/* WEAPON_L3:(P032-P063) */
	shot_regist_3,	shot_regist_4,	shot_regist_3,	shot_regist_4,	shot_regist_2,	shot_regist_chou,		shot_regist_3,	shot_regist_3,	/* WEAPON_L4:(P064-P127) */
	shot_regist_4,	shot_regist_5,	shot_regist_3,	shot_regist_5,	shot_regist_2,	shot_regist_chou,		shot_regist_3,	shot_regist_3,	/* WEAPON_L5:(P128) 	 */
						};
						(bbb[(weapon_level_offset)])(s1);
					}
				}
			}
		}
	}
	#if 0/*デバッグ用*/
	pd_my_score 	= (weapon_level_offset);/*デバッグ*/
	pd_graze_point	= (weapon_level_offset);/*デバッグ*/
	#endif

	/*---------------------------------------------------------
		プレイヤーアニメーション関連処理
	---------------------------------------------------------*/
	{static int anime_delay;
		anime_delay -= 1;	/*fps_fa_ctor*/
		if (0 > anime_delay)
		{
			anime_delay = player_fix_status[BASE_SPEED_ANIME+select_player];/*2*/
			//
			static int auto_anime_frame = (4);/*5*/
				 if (my_pad & PSP_KEY_LEFT/*PSP_CTRL_LEFT*/)	{	if ( (0) < auto_anime_frame )	{auto_anime_frame--;} }
			else if (my_pad & PSP_KEY_RIGHT/*PSP_CTRL_RIGHT*/)	{	if ( (8) > auto_anime_frame )	{auto_anime_frame++;} }
			else
			{
				if (auto_anime_frame>4/*5*/) auto_anime_frame--;
				if (auto_anime_frame<4/*5*/) auto_anime_frame++;
			}
			#if 1
			{
				int now_anime_frame;
				static int auto_aniime;
				auto_aniime++;
				auto_aniime&=3;
					 if (4==auto_anime_frame)	{	now_anime_frame = (auto_anime_frame + auto_aniime); }	/* 中心1-4 */
				else if (4<auto_anime_frame)	{	now_anime_frame = (auto_anime_frame + (3) + (8) );	}	/* 右側1-4 */	/* +(8) == オプションインターリーブ */
				else							{	now_anime_frame = (auto_anime_frame );				}	/* 左側1-4 */
				s1->type = (JIKI_PLAYER_00_LEFT)+(now_anime_frame);
			}
			#else
			{
				static int auto_aniime;
				auto_aniime++;
				auto_aniime&=3;
				s1->type = (JIKI_PLAYER_00_LEFT)+(auto_anime_frame );
					 if (4==auto_anime_frame)	{	s1->type += ( auto_aniime); }	/* 中心1-4 */
				else if (4<auto_anime_frame)	{	s1->type += ( 3+8); 		}	/* 右側1-4 */
			//	else							{					}	/* 左側1-4 */
			}
			#endif
		}
	}
/*
0 1 2 3 4 5 6  7  8  :: auto_anime_frame[0 to 8]
0 1 2 3 4 8 9 10 11  :: now_anime_frame [0 to 11]
		5
		6
		7
*/
}

/*---------------------------------------------------------
	武器レベルが変更された場合に、武器やオプションの状態をチェックして変更
---------------------------------------------------------*/
/* [pd_weapon_powerが変更された場合に必ず行う後チェック] */
static void check_weapon_level(void)
{
	/*---------------------------------------------------------
		武器の段階を決める
	---------------------------------------------------------*/
	//weaponの段階から今の装備を決める		//リストを見て装備の変更
	// [***090123	最大128へ。
	// [***100123	最大129へ。
	// (0-128の129段階に修正)
	#if 0//1968661
	/* いろいろ頑張ってみたが、元の調整(6,11,61,86)は誰得(?)なので変える(2倍を超える値の調整は意味無い気ががする) */
	u8 pd_weapon;	pd_weapon = pd_weapon_power;
		 if (pd_weapon < (	8)) 	{	weapon_level_offset = (WEAPON_L0<<3);	}	/* WEAPON_L0(P000-P008) */	/* 6*/
	else if (pd_weapon < ( 16)) 	{	weapon_level_offset = (WEAPON_L1<<3);	}	/* WEAPON_L1(P008-P015) */	/*11*/
	else if (pd_weapon < ( 32)) 	{	weapon_level_offset = (WEAPON_L2<<3);	}	/* WEAPON_L2(P016-P031) */	/*61*/
	else if (pd_weapon < ( 64)) 	{	weapon_level_offset = (WEAPON_L3<<3);	}	/* WEAPON_L3(P032-P063) */	/*86*/
	else if (pd_weapon < (128)) 	{	weapon_level_offset = (WEAPON_L4<<3);	}	/* WEAPON_L4(P064-P127) */
	else							{	weapon_level_offset = (WEAPON_L5<<3);	}	/* WEAPON_L5(P128)		*/	/*max==P128==「129段階」*/
	weapon_level_offset += select_player;
	#endif
	#if 1//1968181
	u8 pd_weapon;	pd_weapon = pd_weapon_power;
	/* 0000 0001 1111 1--- */
	/* 紅、調べたら丁度2倍っぽい。 */
	{	/* 丁度2倍だとこんな感じで簡略化できる。 */
		weapon_level_offset = (select_player/*0*/);
		weapon_level_offset += (WEAPON_L5<<3);
		int jj;
		for (jj=(0); jj<(5); jj++)
		{
			if (pd_weapon & ((0x80)>>jj))
			{
				goto loop_end;//break;
			}
			weapon_level_offset -= (0x08);
		}
		loop_end:;
	}/*
0000 0000 0000 0000
		  5432 1 0
		  0123 4
*/
	#endif
//
	/*---------------------------------------------------------
		プレイヤーオプションチェック(必要であればオプションをセット)
	---------------------------------------------------------*/
	#if 1
	{
		/* いろいろ頑張ってみたが、元の調整(35,55,80,108)は誰得(?)なので変える(2倍を超える値の調整は意味無い気ががする) */
	//	static const u8 jjj_tbl[4] =
	//	{
	//		( 8),		//(35-1),
	//		(16),		//(55-1),
	//		(32),		//(80-1),
	//		(64)		//(108-1),
	//	};
		int jj;
		for (jj=(0); jj<(4); jj++)/* オプションは4つ */
		{
			if ( (pd_weapon > ((0x08)<<jj)/*jjj_tbl[jj]*/)) 	/* 丁度2倍だとこんな感じで簡略化できる。 */
					{	option[jj]->flags	|= ( (SP_FLAG_OPTION_VISIBLE)); 	}	// 可視フラグのOn(可視)
			else	{	option[jj]->flags	&= (~(SP_FLAG_OPTION_VISIBLE)); 	option[jj]->y256=t256(GAME_HEIGHT+1);	}	// 可視フラグのOFF(不可視)
		}
	}
	#endif
}
/*
メモ
;-------------------
霊夢 霊符	ショット（ホーミングアミュレット）		ボム （夢想封印）

L0	1way(まっすぐ中)								P000-P007			(直進1way)
オプション2つ追加									P008-P015
L1	2way(右上、左上)								P016-	28			(間隔が狭い2way)

L3	3way(まっすぐ中、右斜上、左斜上)				P032-	43			(間隔が広い3way)

Lx	3way(まっすぐ中、右斜上、左斜上)				P064-	113 	124 (間隔が広い3way)

124 オプション3方向

Lx	4way(まっすぐ右、まっすぐ左、右斜上、左斜上)			MAX(128)	(間隔が広い4way)

MAX(128)->死ぬ->(112)->死ぬ->(96)->死ぬ->(80)->死ぬ->(64)
;-------------------
霊夢 夢符	ショット（パスウェイジョンニードル）	ボム （封魔陣）

;-------------------
魔理沙 魔符 ショット（マジックミサイル）			ボム （スターダストレヴァリエ）

;-------------------
魔理沙 恋符 ショット（イリュージョンレーザー）		ボム （マスタースパーク）

;-------------------
*/

/*---------------------------------------------------------
	プレイヤーとアイテムのあたり判定1(アイテム専用)
	-------------------------------------------------------
		src 	プレイヤー
		ttt 	アイテム
---------------------------------------------------------*/
static void player_add_power(SPRITE *src, SPRITE *ttt, int add_power)
{
	int prev_power; prev_power = pd_weapon_power;		/* アイテム取得する直前のパワー値 */
	pd_weapon_power += add_power;	/* パワー値、加算 */
	/* パワー値、超えてたら修正 */
	if ((MAX_POWER_IS_128-1) < pd_weapon_power )/*127==128段階目*/
	{
		pd_weapon_power = MAX_POWER_IS_128;/* 最大値 */
	}
	int add_score_point;
	if (MAX_POWER_IS_128 == pd_weapon_power/*prev_power*/) /* maxの場合 */
	{
		pd_chain_point += add_power;	/* チェイン、加算 */
		/* チェイン、超えてたら修正 */
		if (31 < pd_chain_point)
		{
			pd_chain_point = 31;/* 最大値 */
		}
		/* (ノーミスボーナス)ウェポンボーナス得点計算 */
		add_score_point = (/*score_type*/(pd_chain_point));
	}
	else	/* maxでない場合、得点計算 */
	{
		/* (MAX時以外の) [P]は10点 */
		/* (MAX時以外の) [P中]は80点 */
		add_score_point = ((add_power/*-1*/)/*SCORE_10*/);
	}
	bonus_info_score_nodel(ttt, add_score_point);/* */ttt->type = SP_DELETE;/* おしまい */
	check_weapon_level();/* [pd_weapon_powerが変更された場合に必ず行う後チェック] */
//
	voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*テキトー*/
}

static void player_colision_check_item(SPRITE *src/*, int mask*/)
{
	/* 自機がアイテムにあたった場合 */
	SPRITE *ttt;	//対象
	ttt = sprite_collision_check_444(src, SP_GROUP_ITEMS);	/* 弾幕専用(アイテム) */	/*mask*/
	if (NULL != ttt)
	{

		switch (ttt->type)
		{
		case SP_ITEM_00_P001:	player_add_power(src, ttt, 1);		break;	// ウェポンアイテム(小P)
		case SP_ITEM_01_P008:	player_add_power(src, ttt, 8);		break;	// ウェポンアイテム(中P)	// [***090123		追加
		case SP_ITEM_02_P128:	player_add_power(src, ttt, 127);	break;	// ウェポンアイテム(F)		// [***090123		追加
		//
		case SP_ITEM_04_BOMB:
			//player_add_bomb(t);		// [*****本来はコメントアウトしないよ
			if ((8-1) < pd_bombs)	{	goto add_10000pts;	}/* 既に最大値(8)ならば、10000+ [pts] */
			pd_bombs++;
			#if 1/*バグfix?*/
			ttt->type = SP_DELETE;/* おしまい */
			#endif
			voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*テキトー*/
			break;

		case SP_ITEM_03_1UP:
			if ((8-1) < pd_zanki)	{	goto add_10000pts;	}/* 既に最大値(8)ならば、10000+ [pts] */
			pd_zanki++;
			#if 1/*バグfix?*/
			ttt->type = SP_DELETE;/* おしまい */
			#endif
			/*effect_sound_number=*/voice_play(VOICE06_EXTEND, TRACK03_SHORT_MUSIC);/*テキトー*/
			break;

		case SP_ITEM_07_SPECIAL:
			break;
		#if 0
		/* 星点は、あたり判定なし */
		case S P_ITEM_05_HOSI:		/*not_break;*/
			ttt->type = SP_DELETE;/* 星点のみ特別処理 */
//
			voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*テキトー*/
			break;
		#endif
		case SP_ITEM_05_TENSU:
			{
				/* PLAYER_SPLIT_LINE256 より上で取ると 10000pts. ... 下で取ると(?)約100pts. */
				/* (大体90pts、非常に難しいが、がんばれば(消える直前の3ライン)70ptsまで可能らしい) */
			//
				int add_score_point;
				if (PLAYER_SPLIT_LINE256 < src->y256)	/* PLAYER_SPLIT_LINE256 未満の場合は、PLAYER_SPLIT_LINE256までの距離におおじて減点 */
				{
				//	add_score_point -= ((ttt->y256-PLAYER_SPLIT_LINE256)>>(4+8));
				//	add_score_point = ((/*0x1f-(3)*/(SCORE_9000)-((ttt->y256-PLAYER_SPLIT_LINE256)>>(3+8)))/*&0x1f*/);
					add_score_point = (SCORE_9000) - ((ttt->y256-PLAYER_SPLIT_LINE256)>>(3+8));
				}
				else
				{
					add_10000pts:/* 強引に得点10000+ [pts] */
				//	add_score_point = (SCORE_10000);	/* 基本点 10000[pts] */
					add_score_point = (SCORE_10000+difficulty); 	/* 基本点 10000[pts](easy), 11000(normal), 12000(hard), 512000(lunatic). */
				}
				//player_add_score(score(1000));		// [***090123		変更
			//
				bonus_info_score_nodel(ttt, add_score_point);/* */ttt->type = SP_DELETE;/* おしまい */
//
			}
			voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*テキトー*/
			break;
		}
	}
}


/*---------------------------------------------------------
	ぴちゅーん処理(サブ)
	-------------------------------------------------------
	プレイヤーが 敵グループ または 敵弾 に当たった場合、
	喰らいボム開始の為の、ぴちゅーん処理を行う。
---------------------------------------------------------*/
static void player_set_pichuun(void)
{
	pd_player_status	= PLAYER_STATE_01_HIT_BOMB;
	pd_save_timer		= player_fix_status[BASE_HIT_BOMB_WAIT+select_player];/*0*/
	voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* 自機死に音は、なるべく重ねない */
}


/*---------------------------------------------------------
	プレイヤーと敵のあたり判定3(敵専用)
	-------------------------------------------------------
		src 	プレイヤー
		ttt 	敵
	-------------------------------------------------------
	自機が敵(ボス/中型/ザコ)に体当たりされた場合
---------------------------------------------------------*/

static void player_colision_check_enemy(SPRITE *src)//, int player_hit_enemy_group_mask)
{
	SPRITE *ttt;	/* 敵自体(ボス/中型/ザコ) */
	ttt = sprite_collision_check_SDL_teki(src);//, (SP_GROUP_TEKI)/*player_hit_enemy_group_mask*/ /*SP_GROUP_ENEMYS*/);/*敵専用*/
	if (NULL != ttt)	{	player_set_pichuun();	}
}


/*---------------------------------------------------------
	プレイヤーと敵弾のあたり判定2(グレイズ専用)
	-------------------------------------------------------
		src 	プレイヤー
		ttt 	敵弾
---------------------------------------------------------*/

static void player_colision_check_graze(SPRITE *src)
{
	SPRITE *ttt;	/* 敵弾 */
	ttt = sprite_collision_check_444(src, SP_GROUP_BULLETS);/* 弾幕専用(弾幕) */
	if (NULL != ttt)
	{
		/* プレイヤーに敵弾があたった場合はグレイズする */
		if (0==(ttt->flags & SP_FLAG_GRAZE)) /* グレイズ済? */
		{
			ttt->flags |= SP_FLAG_GRAZE;/*グレイズ済*/
			pd_graze_point++;/*グレイズ数*/
			player_add_score((score(100)+score(200)*difficulty));/*score(500)*/
			voice_play(VOICE09_GRAZE, TRACK07_GRAZE);/*テキトー*/
		}
		/* プレイヤースプライトが弾に触れているか */
		{
			/* (グレイズ中ならコアと敵弾のあたり判定をする) */
			#if 1
			/* コアで判定 */
			ttt = sprite_collision_check_444(obj_maru, SP_GROUP_BULLETS);/* 弾幕専用(弾幕) */
			#else
			/* 自機で判定 */
			int chache_m_Hit256R;
			chache_m_Hit256R	= src->m_Hit256R;/* 自機のあたり判定サイズ */
			src->m_Hit256R		= obj_maru->m_Hit256R;/* ○のあたり判定サイズ */
			ttt = sprite_collision_check_444(src, SP_GROUP_BULLETS);/* 弾幕専用(弾幕) */
			src->m_Hit256R		= chache_m_Hit256R;/* 戻す */
			#endif
			if (NULL != ttt)	{	player_set_pichuun();	}
		}
	}
}


/*---------------------------------------------------------
	プレイヤー移動
---------------------------------------------------------*/

/*---------------------------------------------------------
	プレイヤーループを抜ける処理
---------------------------------------------------------*/
global void player_loop_quit(void)
{
	pd_state_flag		|= STATE_FLAG_14_GAME_LOOP_QUIT;
	#if (0000)
	option[0]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* オプションを消す */
	option[1]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* オプションを消す */
	option[2]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* オプションを消す */
	option[3]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* オプションを消す */
	#endif/*0000*/
//
	pd_save_timer		= (150);	/* 150	 120では若干短い 100 */
	pd_player_status	= PLAYER_STATE_04_GAME_OUT;/* GAME_OUT中 */
}
	#if (00)
	obj_player->flags	&= (~(SP_FLAG_VISIBLE));	/* 自機、非表示 */
	obj_maru->flags 	&= (~(SP_FLAG_VISIBLE));	/* ○、非表示 */
//	obj_maru->alpha 	= 0x00; 		// ○を消すために入れたけど意味無かったかもしれない。	// ゲームオーバー時の○の表示処理
	#endif

	/* オプションが非表示の場合、何もしない。 */
//	if (0==(src->flags & ( SP_FLAG_OPTION_VISIBLE)))	{	return; 	}


/*---------------------------------------------------------
	プレイヤー復活処理
---------------------------------------------------------*/
global void player_few_muteki(void)
{
	pd_save_timer		= (40); 					// 無敵時間 		/*120*/ /*150-120*/
	pd_player_status	= PLAYER_STATE_02_SAVE_01;	// 無敵状態？
}

static void player_fukkatsu(SPRITE *s1)
{
	player_few_muteki();/* ステージ開始時のみ若干の無敵状態にセット */
	#if (00)
	obj_player->flags			|= ( (SP_FLAG_VISIBLE));			/* 自機、表示 */
	#endif
	obj_player->color32 		= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);						/* 自機、半透明 */	/*	s1->alpha			= 0x50;*/
//
//	obj_maru->flags |= ( (SP_FLAG_VISIBLE));						/* ○、表示 */
	obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);								/* ○、半透明 */	/*	obj_maru->alpha = 0x50;*/
	pd_bombs			= option_config[OPTION_CONFIG_01_BOMB]; 	/* ボム所持数の初期化 */	/*player_fix_status[BASE_BOMBS+select_player]*/  /*3*/
	pd_chain_point		= (0);										/* ノーミスボーナスの初期化 */	// pd_chain_point(ノーミスボーナス)の初期化 // どれだけ連続(ノーミス)でwepon_upを取ったか
//
	check_weapon_level();/* [pd_weapon_powerが変更された場合に必ず行う後チェック] */
}

/*---------------------------------------------------------
	プレイヤーやられ処理
---------------------------------------------------------*/

static void player_explode(SPRITE *s1)
{
	/* アイテムを吐き出す */	//	[***090124ちょっと追加
	{
		if ((0==pd_zanki)
			/*&&(0==difficulty)*/	/* (紅、調べたら難易度に関係なく[F]が出る)	*/
		)
		{
			/* コンティニューの場合(GAME_OUT)easy の場合 */
			/* 無駄に8個吐かせる  */
			/* コンティニューの場合easy の場合： (キャッチ出来る数で点数は違うけど、どれか１つキャッチすれば POWER は同じだから) */
		//	item_create(s1, (0==difficulty)?(SP_ITEM_02_P128):(SP_ITEM_01_P008), 8, ITEM_MOVE_FLAG_06_RAND_XY);
			item_create(s1, (SP_ITEM_02_P128), 8, ITEM_MOVE_FLAG_06_RAND_XY);
		}
		else
		{
			/* コンティニューの場合normal以上の場合： FIRE_POWER_G (8) x (8) == 64/128 (全部集めて50%) 。 */
			/* コンティニューの場合(GAME_OUT)normal以上の場合 */
			/* コンティニュー以外の場合 */
			/* 極端に弱くて死んだ場合の救済策:
				  上手い人は(一面)序盤で FULLPOWER になると思うし、
				仮に凡ミスで死んだとしてもボム回収などで POWER を維持するので、
				極端に弱くて死ぬ事はほぼない。
				  極端に弱くて死ぬのは結構下手だと思うので、若干の救済策が必要かと思う。
				その場合 FIRE_POWER_G (8) x (7+1) == 64/128 (全部集めて50%) にした。
			*/
			#if (0)
			/*
				パワーアップシステムを若干見直したので、(30-1) -> (16) へ基準を厳しくした。
 WEAPON_L0(P000-P008)
 WEAPON_L1(P008-P015)
 WEAPON_L2(P016-P031)
 WEAPON_L3(P032-P063)
 WEAPON_L4(P064-P127)
 WEAPON_L5(P128)
					WEAPON_L1以下で死んだ場合のみ救済策があり、
					全部取るとWEAPON_L3になるので、妥当だと思う。が、多少多いかな?
					WEAPON_L1[P15] -> 死ぬ -> 全部取る[P8]x6[個] -> [P15+P8x6]==WEAPON_L3[P63]
					WEAPON_L1[P15] -> 死ぬ -> １つ取る[P8]x1[個] -> [P15+P8x1]==WEAPON_L2[P23]
					WEAPON_L0[P00] -> 死ぬ -> 全部取る[P8]x6[個] -> [P00+P8x6]==WEAPON_L3[P48]
					WEAPON_L0[P00] -> 死ぬ -> １つ取る[P8]x1[個] -> [P00+P8x1]==WEAPON_L1[P08]
					WEAPON_L0[P00] -> 死ぬ -> ３つ取る[P8]x3[個] -> [P00+P8x3]==WEAPON_L2[P24]
					WEAPON_L2[P16] -> 死ぬ -> 全部取る[P1]x6[個] -> [P16+P1x6]==WEAPON_L2[P22]
					死んで半分(3つ)取ると必ずWEAPON_L2以上になる。
			*/
		//	item_create(s1, (((30-1) > pd_weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), (7)/*5*/, ITEM_MOVE_FLAG_06_RAND_XY);
		//	item_create(s1, (((16) > pd_weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), (6), ITEM_MOVE_FLAG_06_RAND_XY);/* いくら下手でも、多少多いかな? */
		//	item_create(s1, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
			#else
			/* 多少修正してみる */
			item_create(s1, (((16) > pd_weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), (3), ITEM_MOVE_FLAG_06_RAND_XY);/* 下手な人ががんばれば3つ取れる事が前提 */
			item_create(s1, SP_ITEM_00_P001, (5), ITEM_MOVE_FLAG_06_RAND_XY);/* P001をばらまく(おまけ、下手な人は、がんばっても同時には取れないという事が前提) */
			#endif
		}
		//
		/* レミリア 特殊能力：抱え落ちすると必ずボムが出る */
		if (REMILIA==select_player) 	/* レミリアの場合 */
		{
			/* 抱え落ちすると */
			if (0 != pd_bombs)
			{
				/* 使ってない分の持ってるボムを吐き出す */
				item_create(s1, SP_ITEM_04_BOMB, pd_bombs, ITEM_MOVE_FLAG_06_RAND_XY);
			}
		}
	}
//		pd_explode		= 0;
//	if (0==pd_explode)
	{

	//	bakuhatsu_add_type(t->x256+t256(5),t->y256+t256(5),/*0,*/BAKUHATSU_MINI00);
	//	t->type = SP_DELETE;/* おしまい */			/* あたった敵爆発 */
	//	pd_explode=0;
	//	pd_bonus=0;
		//	voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* 自機死に音は、なるべく重ねない */ 	// [***090127	場所を移動する。// 変更元
		/* 爆発エフェクト */

		#if 0/* あたった敵は倒せない敵かもしれないので、ここで必ず消すのはまずい気がする */
		/* あたった敵爆発 */
		bakuhatsu_add_type(pd_enemy->x256+t256( 5), pd_enemy->y256+t256( 5), /*0,*/ BAKUHATSU_MINI00);
		pd_enemy->type	= SP_DELETE;/*???*/
		#endif
		/* 自分爆発 */
		#if 1
		{
			int jj;
			for (jj=0; jj<(4); jj++)
			{
				bakuhatsu_add_type_ddd(s1, BAKUHATSU_MINI00);
			}
		}
		#endif
		/* プレイヤー位置の初期化 */
		#if 0/*SDL(左隅座標)*/
		s1->x256 = t256(GAME_WIDTH/2)-((s1->w128));
		s1->y256 = t256(220/*GAME_HEIGHT*/);/*240*/
		#else/*Gu(中心座標)*/
		s1->x256 = t256(GAME_WIDTH/2);
		s1->y256 = t256(220/*GAME_HEIGHT*/);/*240*/
		#endif
		/*	*/
		pd_bomber_time	= 0;			/* ←ここが無いせいで魔理沙(&チルノ)ボム撃てなくなってた */
		#if 1
	//	pd_bomber_time = 0;/*都合上*/
		set_bg_alpha(255);/* 画面を明るくする */
		#endif

	//	pd_state_flag		= STATE_FLAG_00_NONE;/*???*/
		pd_state_flag		&= (~(	STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM | 	/* 終わり */
									STATE_FLAG_02_BOMB_AUTO_GET_ITEM |			/* 終わり */
									STATE_FLAG_03_SCORE_AUTO_GET_ITEM));		/* 終わり */

		/* 現在オプション増えすぎて、後半重くなるバグがある。 */
		#if (0000)
		option[0]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* オプションを消す */
		option[1]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* オプションを消す */
		option[2]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* オプションを消す */
		option[3]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* オプションを消す */
		#endif/*0000*/
		pd_count_miss++;	/* ミス回数 */
		/* 残チャンス減らす */
		pd_zanki--;
		if (0 > pd_zanki)	/* 残チャンスがなければ */
		{
			player_loop_quit();
		}
		else
		{
			//
		//	if (pd_player_speed > player_fix_status[BASE_SPEED_MINIMUM+select_player]/*pd_player_speed_minimum*/)			//player_speed_minimum以下にならないように
		//	{	pd_player_speed--;}
		//	pd_weapon_power -= (difficulty*4)+12;		/* 武器レベルの低下 */
		//	pd_weapon_power -= ((difficulty+1)<<4); /* 武器レベルの低下(easy=16,32,48,64) */
			pd_weapon_power -= (16);	/* 武器レベルの低下(紅、調べたら難易度に関係なくP16減だった) */
			if (0 > pd_weapon_power)	{	pd_weapon_power = 0;	}
			player_fukkatsu(s1);
		}
	}
}

/*---------------------------------------------------------
	プレイヤー移動、特殊処理
	-------------------------------------------------------
	やられ中、復活中、ボム中、GAME_OUT中などの特殊処理
---------------------------------------------------------*/
//void render_continue(void/*int now_max_continue*/);
/*static*/ int	now_max_continue;
static void player_move_other(SPRITE *s1)
{
	pd_save_timer--;
	if (2 > pd_save_timer)/* 次の状態に移行 */
	{
		switch (pd_player_status)
		{
		case PLAYER_STATE_01_HIT_BOMB:/* 喰らいボム受付中 */			// [***090125		追加
			/* 死亡確定 */
				pd_state_flag |= STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL;		/* キー入力無効(1) */
			player_explode(s1);/*プレイヤー爆発*/
			break;

		case PLAYER_STATE_02_SAVE_01:/*プレイヤー復活中１(画面外から復活地点へ点滅しながら移動中)*/
		//ちょっとした待ち時間
		//	if (s1->y256 < t256(220)/*240*/)	//ちょっと前に出てくる
			{
				pd_state_flag		&= (~STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL);	/* キー入力有効(0) */
				pd_save_timer		= (120);
				pd_player_status	= PLAYER_STATE_03_SAVE_02;
			}
		//	else
		//	{
		//		s1->y256 -= t256(1)/*fps_fa_ctor*/;
		//	}
			break;/*??? [***090210 追加 */

		case PLAYER_STATE_03_SAVE_02:/*プレイヤー復活中２*/
			pd_player_status	= PLAYER_STATE_00_NORMAL;
			s1->color32 		= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);					/*	s1->alpha			= 0xff;*/
			obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/* ○、 */	/*	obj_maru->alpha = 0xff;*/
			break;

		case PLAYER_STATE_04_GAME_OUT:/* GAME_OUT中 */
			now_max_continue--;
			if (
					(0 < now_max_continue)
				#if (0)/* (0==DEBUG) */
				/* まだ無効 */
				&& ( (6) != player_now_stage)	/* 最終面はコンティニュー出来ない。 */
				#endif
				)
			{
				pd_use_continue++;	/* コンティニュー回数 */
			//	render_continue(/*now_max_continue*/);
				main_call_func = ask_continue_menu_start;
			}
			else
			{
				main_call_func = gameover_start;
			}
			break;
		}
	}
}


/*---------------------------------------------------------
	プレイヤー移動、メイン処理
---------------------------------------------------------*/
#define USE_BOSS_NOT_BOMBER 0/* 原作風(ボスでボム中無敵)(まだ作ってないので0 に出来ない) */
//#define USE_BOSS_NOT_BOMBER 1/* ボスでボムゲ禁止(ボスでボム中死ぬ) */
/*
	現在 0 にすると、ゲーム的に支障があるので出来ない。(まだ作ってないので0 に出来ない)
	支障というのは、現在の方式ではボムが複数ある場合に、重なりボムで異常な攻撃力になってしまう為。
//
	これを解決するには。
	ボスに攻撃した場合は、直接ライフを減少させない。
	ボスの最大攻撃減少値をフレーム単位で管理する。
//
	まだ作ってない。
*/

static void any_player_move(SPRITE *s1)
{
	pd_state_flag &= (~STATE_FLAG_15_KEY_SHOT); /* off */
	if (0==(pd_state_flag & STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL))	/* キー入力有効(0) */
	{
		player_keycontrol(s1);
	}
	#if (1==USE_BOSS_NOT_BOMBER)
	int player_hit_enemy_group;
	//player_hit_enemy_group = (SP_GROUP_TEKI);
	//player_hit_enemy_group = (SP_GROUP_BOSS);
	player_hit_enemy_group = (SP_GROUP_TEKI);
	#endif
	if (0 != pd_save_timer)/* 特殊処理 */
	{
//		bullets_to_hosi();/* 弾全部、星アイテムにする */
		player_move_other(s1);
	//
		#if (1==USE_BOSS_NOT_BOMBER)
		if (0==(pd_state_flag & (STATE_FLAG_05_IS_BOSS)))
		{	/* 道中 */
			/* ボム中は、 ザコ敵 にあたって死なない */
			player_hit_enemy_group = (0);
		}
	//	else
	//	{	/* ボス戦闘時 */
	//		/* ボム中は、ボス にあたって死ぬ */
	//		player_hit_enemy_group = (SP_GROUP_TEKI/*SP_GROUP_BOSS*/);
	//	}
		#endif
	}
	else		/*通常処理*/
	{
//	case PLAYER_STATE_00_NORMAL:
		player_colision_check_graze(s1);
//		player_hit_enemy_group = (SP_GROUP_TEKI/*|SP_GROUP_BOSS*/);/* 雑魚＆ボス */
	}
	/* ボム中は、ボス / 中ザコ敵 にあたって死ぬ */
	/* スクリプト動作中 は、敵あたり判定はない */
	/* キー入力無効中(==復活中) は、敵あたり判定はない */
	if (0==(pd_state_flag & (STATE_FLAG_06_IS_SCRIPT|STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL)))
	{
		/* 喰らいボム受付中 は、敵あたり判定はない */
		if (PLAYER_STATE_01_HIT_BOMB != pd_player_status)
		{
			#if (1==USE_BOSS_NOT_BOMBER)
			if (0 != player_hit_enemy_group)/*(???)*/			/* ボム中は、 ザコ敵 にあたって死なない */
			#else /*(0==USE_BOSS_NOT_BOMBER)*/
			if (0 != pd_save_timer) 	{;} 	else/*(???)*/			/* ボム中は、 ボス/ザコ敵 にあたって死なない */
			#endif
			{
				player_colision_check_enemy(s1);/*SP_GROUP_ENEMYS*/  // 	/, /*(SP_GROUP_TEKI)*/player_hit_enemy_group
			}
		}
	}
	/* アイテムは常に取れる */
	player_colision_check_item(s1/*,SP_GROUP_ITEMS*/);
}


/*---------------------------------------------------------
	プレイヤー初期化()
---------------------------------------------------------*/
#if (1==USE_EXTEND_CHECK)
extern void player_init_extend_score(void);
#endif
//extern int zanki;
global void player_continue_value(void)
{
	pd_zanki				= (1+option_config[OPTION_CONFIG_00_PLAYER]);	/*pd_base_zanki*/	/*((zanki&0x03)+2)*/ /*player_fix_status[BASE_LIVES+select_player]*/
	pd_my_score 			= score(0);
	pd_state_flag			&= (~STATE_FLAG_14_GAME_LOOP_QUIT); 	/* 復帰 */
	#if (1==USE_EXTEND_CHECK)
	player_init_extend_score();
	#endif
	player_fukkatsu(obj_player);/* オプションを追加より後 */
}

/*---------------------------------------------------------
	プレイヤー、オプションの追加
	霊夢	魔理沙	幽々子
---------------------------------------------------------*/

static void option_create_re_ma_yu( SPRITE *h,int jj)
{
			h->type 				= (JIKI_OPTION_00_00/*|S P_MUTEKI*/);
		//
			h->callback_mover		= re_ma_yu_move_option;
		//	h->PL_OPTION_DATA_length			= 11;
			static const int iii_tbl[4] =
			{
				OPTION_ID_02_LM,	/*-1*/ /*;*/ /*0*/
				OPTION_ID_03_RM,	/* 1*/ /*;*/
				OPTION_ID_01_LL,	/*-1*/ /*;*/ /*0*/
				OPTION_ID_04_RR 	/* 1*/ /*;*/
			};
			h->PL_OPTION_DATA_opt_anime_add_id/*pos*/			= iii_tbl[jj]/*-1*/;/*0*/
			static const /*int*/s16 ccc_tbl[4] =
			{
				cv1024r((270)), 	/*deg_360_to_512(180)*/ /*;*/ /*-ra d2deg512(M_PI)*/
				cv1024r(( 90)), 	/*deg_360_to_512(  0)*/ /*;*/
				cv1024r((270)), 	/*deg_360_to_512(180)*/ /*;*/ /*-ra d2deg512(M_PI)*/
				cv1024r(( 90))		/*deg_360_to_512(  0)*/ /*;*/
			};
			h->PL_OPTION_DATA_angleCCW1024						= ccc_tbl[jj];
}

static void option_create_oz_ci( SPRITE *h,int jj)	/* レミリア ＆ チルノ [***090220 追加 */
{
//			h->type 				= (JIKI_OPTION_00_00/*|S P_MUTEKI*/)+kk; kk += (16);/* オプションインターリーブ */ /*8*/
			h->type 				= (JIKI_OPTION_00_00/*|S P_MUTEKI*/)+(jj<<4); /* オプションインターリーブ */ /*8*/
		//
			h->callback_mover		= oz_ci_move_option;
			static const /*int*/s16 aaa_tbl[4] =
			{
				cv1024r(360-(45*5)),	/*cv1024r(45*1)*/ /*1024*1/8*/ /*;*/ /*チルノ*/
				cv1024r(360-(45*7)),	/*cv1024r(45*5)*/ /*1024*5/8*/ /*;*/ /*チルノ*/
				cv1024r(360-(45*1)),	/*cv1024r(45*3)*/ /*1024*3/8*/ /*;*/ /*チルノ*/
				cv1024r(360-(45*3)) 	/*cv1024r(45*7)*/ /*1024*7/8*/ /*;*/ /*チルノ*/
			};
			h->PL_OPTION_DATA_angleCCW1024/*REMILIA_angle1024*/ = aaa_tbl[jj];
//
		//	h->PL_OPTION_DATA_next					= obj_player;
			h->PL_OPTION_DATA_state2				= 0;
			h->PL_OPTION_DATA_offset_x256			= t256(0);
			h->PL_OPTION_DATA_offset_y256			= t256(0);
			h->PL_OPTION_DATA_opt_anime_add_id		= (OPTION_C1+jj);
		//	h->PL_OPTION_DATA_ccc_angle512			= cv1024r((0));
			h->PL_OPTION_DATA_slow_count			= 0;
}
static void option_create(void) /* レミリア ＆ チルノ [***090220 追加 */
{
	int jj;
	for (jj=0; jj<4; jj++)
	{
		SPRITE *h;
		h						= sprite_add_gu_error();
	//	if (NULL!=h)/* 登録できた場合のみ */	/* 強制登録 */		/* 仕様バグ(?) */
		{
			option[(OPTION_C1+jj)]	= h;

			#if 0/*SDL(左隅座標)*/
			h->x256 					= obj_player->x256+((src->w128-h->w128));
			h->y256 					= obj_player->y256+((src->h128-h->h128))+t256(20);
			h->x256 					= obj_player->x256+((src->w128-h->w128))-t256(11)/*h->PL_OPTION_DATA_length*/;
			h->y256 					= obj_player->y256+((src->h128-h->h128));
			#else/*Gu(中心座標)*/
		//	h->x256 					= obj_player->x256;
		//	h->y256 					= obj_player->y256+t256(20);
		//	h->x256 					= obj_player->x256-t256(11)/*h->PL_OPTION_DATA_length*/;
		//	h->y256 					= obj_player->y256;
		//	h->x256 					= obj_player->x256/*+t256((31-5)/2)*/;	/* ダミーオフセット */
		//	h->y256 					= obj_player->y256/*+t256((31-5)/2)*/;	/* ダミーオフセット */
			h->x256 					= obj_player->x256/*(???)*/;
			h->y256 					= obj_player->y256/*(???)*/;
			#endif

			h->m_Hit256R			= TAMA_ATARI_BULLET_DUMMY;
		//	h->flags				|= (SP_FLAG_OPTION_VISIBLE);
			h->flags				&= (~(SP_FLAG_OPTION_VISIBLE)); 	/* 可視フラグのOFF(不可視) */
			h->flags				&= (~(SP_FLAG_COLISION_CHECK)); 	/* あたり判定のOFF(無敵) */

	/*???*/ h->base_weapon_strength 				= (1/*8*5*/);/* [***090214 追加 */
			h->PL_OPTION_DATA_opt_shot_interval 	= 0;
		//	h->PL_OPTION_DATA_state 				= 0;
		//	h->PL_OPTION_DATA_state1				= 0;///
			h->PL_OPTION_DATA_anime_wait			= 0;
			/* レミリアのオプションは半透明っぽい */
			if (REMILIA==select_player)
			{
			//	h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x96);	/*	h->alpha			= 0x96; */	/* 明る過ぎる */
				h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x80);	/*	h->alpha			= 0x80;*/	/* 半透明 */
			//	h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	/*	h->alpha			= 0x50;*/	/* 暗ら過ぎる */
			}

			switch (select_player)
			{
			case CIRNO_A:/*とりあえず*/
			case CIRNO_Q:/*とりあえず*/
			case REMILIA:		option_create_oz_ci(h,jj);		break;/* [***090220 追加 */
			default:
			#if 0
			case YUYUKO:/*とりあえず*/
			case REIMU_A:/**/
			case REIMU_B:/**/
			case MARISA_A:
			case MARISA_B:
			#endif
								option_create_re_ma_yu(h,jj);	break;
			}
		}
	}
}
/*---------------------------------------------------------
	プレイヤー初期化
---------------------------------------------------------*/
extern void sprite_initialize_position(SPRITE *h);
extern int boss_hp_dec_by_frame;/*ボス攻撃減少値、フレーム単位*/
extern int practice_mode;
extern void select_jiki_load_surface(void);
extern void boss_effect_sprite_add(void);
global void player_init(void)
{
	select_jiki_load_surface();
	obj_player					= sprite_add_gu_error();		/* [***090220 追加 */
//	if (NULL!=h)/* 登録できた場合のみ */	/* 強制登録 */		/* 仕様バグ(?) */
	obj_player->m_Hit256R		= JIKI_ATARI_ITEM_16;
	obj_player->type			= (SP_GROUP_JIKI_GET_ITEM);
	obj_player->flags			|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	obj_player->callback_mover	= any_player_move;
	#if 1/*Gu(中心座標)*/
	obj_player->x256			= (t256(GAME_WIDTH/2));
	obj_player->y256			= (t256(GAME_HEIGHT));
	#endif
//
	/* プレイヤー、生死判定用コア(obj_maru == ○)の追加 */
	{
	//	SPRITE *s2;
	//	s2 = obj_maru;
		obj_maru					= sprite_add_gu_error();
	//	if (NULL!=h)/* 登録できた場合のみ */	/* 強制登録 */		/* 仕様バグ(?) */
		{
			obj_maru->type				= (JIKI_CORE_00_REIMU_A+select_player);
			{
				const static u8 base_core_atari[8] =
				{
					(1),	/* 霊夢A */ 	//	BASE_CORE_ATARI_0a,/*6*/		/* 霊夢A */ 	/*	5 */	BASE_CORE_ATARI_0a,/*6*/
					(1),	/* 霊夢B */ 	//	BASE_CORE_ATARI_0b,/*6*/		/* 霊夢B */ 	/*	5 */	BASE_CORE_ATARI_0a,/*6*/
					(2),	/* 魔理沙A */	//	BASE_CORE_ATARI_0c,/*8*/		/* 魔理沙A */	/*	6 */	BASE_CORE_ATARI_0b,/*8*/
					(2),	/* 魔理沙B */	//	BASE_CORE_ATARI_0d,/*8*/		/* 魔理沙B */	/*	6 */	BASE_CORE_ATARI_0b,/*8*/
					(2),	/* レミリア */	//	BASE_CORE_ATARI_0e,/*8*/		/* 魔理沙C */	/*	7 */	BASE_CORE_ATARI_0c,/*12*/
					(2),	/* 幽々子 */	//	BASE_CORE_ATARI_0f,/*12*/		/* レミリア */	/*	9 */	BASE_CORE_ATARI_0e,/*8*/
					(1),	/* チルノA */	//	BASE_CORE_ATARI_0g,/*16*/		/* チルノ */	/*	8 */	BASE_CORE_ATARI_0d,/*16*/
					(1),	/* チルノQ */	//	BASE_CORE_ATARI_0h,/*8*/		/* 幽々子 */	/*	6 */	BASE_CORE_ATARI_0b,/*8*/
				};
				//	if (NULL!=h)/* 登録できた場合のみ */	/* 強制登録 */
				obj_maru->m_Hit256R 		= ((base_core_atari[select_player])<<8);
			}
		}
	}
//
//	#if (0==US E_BOSS_COMMON_MALLOC)
//	obj_boss				= NULL;
//	#else
//----[BOSS]
	{ 	SPRITE *h;
		h								= sprite_add_gu_error();
	//	if (NULL!=h)/* 登録できた場合のみ */	/* 強制登録 */		/* 仕様バグ(?) */
		obj_boss = h;
		sprite_initialize_position(h);
	}
//	#endif
	{
		boss_effect_sprite_add();
		boss_effect_initialize_position();
	}

//
	pd_use_continue 		= 0;	/* コンティニュー回数 */
	pd_count_miss			= 0;	/* ミス回数 */
	pd_used_bomber			= 0;	/* ボム使用回数 */
	pd_use_kurai_bomb		= 0;	/* 喰らいボム使用回数 */
//	pd_count_bonus			= 0;	/* スペルカードボーナス回数 */
//
	pd_bomber_time			= 0;	/*==bomb_wait*/
	pd_graze_point			= 0;
//
	pd_state_flag			= STATE_FLAG_00_NONE;
//	if (MARISA==select_player)		{	pd_state_flag		|= STATE_FLAG_04_IS_MARISA; 	}	/* 魔理沙は常に自動収集 */
//


//
	boss_hp_dec_by_frame	= 0;/*ボス攻撃減少値、フレーム単位*/
//
	weapon_level_offset 	= select_player + (/*0==武器レベル*/0<<3);
	/* 練習モードの場合はフルパワーで始める(その代わりクリア後のイベントが見れない) */
	pd_weapon_power 		= (0==practice_mode)?(0):(MAX_POWER_IS_128);
//
	/* オプションを追加 */
	option_create();
//
	now_max_continue		= DEFAULT_MAX_CONTINUE;/*(3)*/
//
	#if 1
	player_continue_value();
	#else
//	pd_zanki				= /*pd_base_zanki*/ player_fix_status[BASE_LIVES+select_player];
//	pd_my_score 			= score(0);
//	player_fukkatsu(obj_player);/* オプションを追加より後 */
	#endif
}

/*---------------------------------------------------------
	敵やられ
	ボス倒した場合の処理
	(プレイヤー側含む)
	-------------------------------------------------------
???謎コメント???	ボスを倒したらすぐ呼ばれる(プレイヤーを無敵にする為)
???謎コメント???	(フラグは仕様上時間待ちがある為、現在この用途には使えない)
---------------------------------------------------------*/
//extern void player_loop_quit(void);
extern int	now_max_continue;

global /*static*/ void lose_boss(SPRITE *src)
{
	bakuhatsu_add_circle(src, 1);	/* pl ayer_set_destoroy_boss(); B02_BOSS_DESTROY が必要 */
//
	{
	//	pd_bo ssmode = B02_BOSS_DESTROY;
//		pd_state_flag |= ST ATE_FLAG_11_IS_BOSS_DESTROY;
		#if 1
		/* バグでアイテム出ない(???) */
	//	if (pd_state_flag&STATE_FLAG_09_IS_WIN_BOSS)		/* 攻撃で倒した場合のみ */
		#endif
		{
		//	pd_state_flag		&= (~STATE_FLAG_09_IS_WIN_BOSS);	/* 終わり */
			/* 相打ちの場合、強制喰らい復活、キー入力有効(0) */
			pd_state_flag		&= (~(STATE_FLAG_09_IS_WIN_BOSS|STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL));	/* 終わり */

			if (0 >= spell_card_boss_timer)
			{
				;/* 時間切れの場合はボーナスアイテムと得点なし。 */
			}
			else
			{
				/* ボーナスアイテムを出す */
// 咲夜 		item_create(src, S P_ITEM_05_HOSI/*SP_ITEM_05_TENSU*/, 15, IT EM_MOVE_FLAG_01_COLLECT);/*星点を出す*/
// 輝夜 		item_create_for_boss(src, ITEM_CREATE_MODE_01);// 輝夜 文 パチェ アリス
				item_create_for_boss(src, ITEM_CREATE_MODE_01);
			//
				#if 1
				/* ボスの得点加算 */
				player_add_score(src->base_score);/* 輝夜のスコア */
				#endif
			}
		}
	}
	//		data->boss_base.boss_health = 0;/* なんかバグるので追加 */
	/* コールバック登録 */
	src->callback_hit_enemy 	= NULL; 	/* ダミーコールバック登録 */
	#if 0
//	src->callback_mover 		= NULL; 		共通ボス退避(撃破後に画面外にボスが逃げる)
//	src->type					= SP_DELETE;	共通ボス退避(撃破後に画面外にボスが逃げる)
	#else
	src->callback_mover 		= common_99_keitai;/* 共通ボス退避(撃破後に画面外にボスが逃げる) */
	#endif
//
//
	#if (0)
	pd_save_timer		= /*pd_bomber_time*/255 + USER_BOMOUT_WAIT/*(30)*/;/*実質無敵時間*/
	#else
//	pd_save_timer		= (6);/* 6[フレーム] ボス倒してから次(シナリオ)に進むまでの待ち時間 */
//	pd_save_timer		= (60);/* 60[フレーム] ボス倒してから、ボスが画面外に退避するまでの待ち時間 */
	pd_save_timer		= (9999);/* 退避後に退避内で指定。	60[フレーム] ボス倒してから、ボスが画面外に退避するまでの待ち時間 */
	#endif
//	pd_save_timer		= 40/*120*/ /*150-120*/;									// 無敵時間
	pd_player_status	= PLAYER_STATE_02_SAVE_01;	// 無敵状態？
//	pd_player_status	= PLAYER_STATE_03_SAVE_02;	/* 稀に、うまくいかない */
//
	pd_bomber_time		= 0;
	#if 1
//	pd_bomber_time = 0;/*都合上*/
	set_bg_alpha(255);/* 画面を明るくする */
	#endif

	#if 1
	src->base_hp = 0;
	boss_life_value = 0;
	#endif
	#if 1
	draw_boss_hp_value	= 0;/* よくわかんない */	/* 輝夜/アリスを倒すと皆破壊される。 */
	#endif
//	s1->color32 		= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	//	s1->alpha		= 0x50; 	/* 半透明 */
//	obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	//	obj_maru->alpha = 0x50; 	/* 半透明 */
	boss_effect_initialize_position();

	/*---------------------------------------------------------
		ボス特殊処理
	---------------------------------------------------------*/
	//	if (B02_BOSS_DESTROY==pd_bo ssmode) //ボスを倒したときの処理
	//	if (/*ST ATE_FLAG_11_IS_BOSS_DESTROY==*/ (pd_state_flag & ST ATE_FLAG_11_IS_BOSS_DESTROY))
	//void boss_destroy_aaa(void)	/* ボスを倒した直後、「ボス後イベント」前の処理 */
	{
		bullets_to_hosi();/* 弾全部、星アイテムにする */
		// TIME_20_DBWAITフレーム待ってから実行。ボスを倒した時に画面に表示されている弾を全て消す処理のために必要。
		play_music_num(BGM_00_stop);
//
		voice_play(VOICE03_BOSS_HAKAI, TRACK03_SHORT_MUSIC/*TRACK02_ALEART_IVENT*/);		// [***090313		追加
//		voice_play(VOICE03_BOSS_HAKAI, TRACK01_EXPLODE);/*予備(うるさい)*/
//
		{
			pd_state_flag &= (~(STATE_FLAG_13_DRAW_BOSS_GAUGE));/*off*/
//			pd_state_flag		&= (~(STATE_FLAG_03_SCORE_AUTO_GET_ITEM));		/* 終わり */
			if (0==practice_mode)/* 練習モードではボス後イベントは見れないよ。 */
			{
			//	pd_bo ssmode	= B07_AFTER_LOAD;
				pd_state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
			}
			else/* 練習モードの場合、終了する */
			{
				#if 1/* この２つのセットで自動的に終了(GAME OVER)する */
				now_max_continue = 1;	/* コンティニューさせない */
				player_loop_quit();
				#endif
			}
		}
	}
//
}
