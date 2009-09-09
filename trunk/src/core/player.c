
/*---------------------------------------------------------
	各プレイヤー(霊夢 ＆ 魔理沙 ＆ レミリア ＆ チルノ ＆ 幽々子)
	REIMU MARISA REMILIA CIRNO YUYUKO
	-------------------------------------------------------
---------------------------------------------------------*/

#include "game_main.h"
#include "player.h"

#if 0
/* CWの場合 */
#else
/* CCWの場合 */
	/*とりあえず*/
//	#define deg_360_to_512 deg_360_to_512CCW
#endif

extern int select_player;

SPRITE *player;
int player_now_stage;		//

enum /*_player_state_*/
{
	PLAYER_STATE_00_NORMAL,
	PLAYER_STATE_01_HIT_BOMB,		// [***090125		追加:PLAYER_STATE_01_HIT_BOMB
	PLAYER_STATE_03_SAVE_01,
	PLAYER_STATE_04_SAVE_02,
	PLAYER_STATE_05_GAME_OUT,
};


typedef struct
{
	int strength;/*union WEAPON_BASE*/
	int vx256;	/* vector x */
	int vy256;	/* vector y */
} PL_SHOT_DATA;

#define PL_LEVARIE_DATA 	PL_SHOT_DATA/*PLAYER_BASE_STRUCT*/
#define OZ_BURN_FIRE_DATA	PL_SHOT_DATA/*PLAYER_BASE_STRUCT*/

typedef struct
{
	int strength;/*union WEAPON_BASE*/
	int angleCCW512;
	int speed256;
//
	int nr;
	int state;
	int time_out;	/* 設定時間内に見つけられない場合はおしまい */
	SPRITE *target_obj/*int target_id*/;
//	SPRITE *tg_sprite;/* ターゲット目標表示マーカー */	/*PL_HOMING_DATAのみ*/
} PL_HOMING_DATA;/*==PL_HLASER_DATA*/

typedef struct
{
	int strength;/*union WEAPON_BASE*/
//	int angle512;
//	int speed256;
//
//	int nr;
//	int state;
	int check_x256;
	int check_y256;

	int time_out;	/* 設定時間内に見つけられない場合はおしまい */
	SPRITE *target_obj/*int target_id*/;
//	SPRITE *tg_sprite;/* ターゲット目標表示マーカー */	/*PL_HOMING_DATAのみ*/
} PL_HOMING_KODOMO_DATA;/*==PL_HLASER_DATA*/

typedef struct
{
	int strength;/*union WEAPON_BASE*/	/* [***090214 追加 */
	int angleCCW512;
	int add_r512;
	int radius; 	/*半径(radius)*/
} PL_KEKKAI_DATA;

typedef struct
{
	int strength;/*union WEAPON_BASE*/	/* [***090214 追加 */
	int angleCCW512;				/* 下が０として左回り(逆回り)の角度 */
	int anime_wait; 				/* アニメーション速度のカウンタ */
	int opt_anime_add_id/*pos*/;	/* アニメーション方向の加算値 / オプションID / (レミ & チルノ)x offset */
	int opt_shot_interval;			/* オプションが発射するショットの更新間隔 */
//
	#if 1
	/* レミ & チルノ */
//	int state1;//共通なので、ここには要らない
	int state2;
	int offset_x256;		/* 円の半径と兼用 */
	int offset_y256;
//	int f_angle512; 	/* レミの回りを回る場合の、回転角度 */	/* FORMATION_01: レミの周りを回るよ */

	int slow_count; 			/*	*/				/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
//	int ccc_angle512;		/* 廃止 */
//	struct _sprite *next;	/* 廃止 */
	#endif
//	int length; 			/* 廃止 */
//	int state;				/* 廃止 */
//	int anime_speed;		/* 廃止 */	/* アニメーション速度の設定値 */
} PL_OPTION_DATA;
#define REMILIA_angle512		angleCCW512
#define REMILIA_OPTION_DATA 	PL_OPTION_DATA



//efine OPTION_ID_01_LL 	(0x01ff)/*旧-1 0  0001 (0x01ff) 0001 1111 1111 */
//efine OPTION_ID_02_LM 	(0x04ff)/*旧-1 0  0100 (0x02ff) 0010 1111 1111 */
//efine OPTION_ID_03_RM 	(0x0801)/*旧+1 1  1000 (0x0401) 0100 0000 0001 */
//efine OPTION_ID_04_RR 	(0x0d01)/*旧+1 1  1101 (0x0701) 0111 0000 0001 */

//efine OPTION_ID_01_LL 	(0x05ff)/*旧-1 0  4 0101 (0x01ff) 0001 1111 1111 */
//efine OPTION_ID_02_LM 	(0x04ff)/*旧-1 0  4 0100 (0x02ff) 0010 1111 1111 */
//efine OPTION_ID_03_RM 	(0x0c01)/*旧+1 1  c 1100 (0x0401) 0100 0000 0001 */
//efine OPTION_ID_04_RR 	(0x0d01)/*旧+1 1  c 1101 (0x0701) 0111 0000 0001 */

//#define OPTION_ID_01_LL	(0x0dff)/*旧-1 0  c 1101 (0x01ff) 0001 1111 1111 */
//#define OPTION_ID_02_LM	(0x0cff)/*旧-1 0  c 1100 (0x02ff) 0010 1111 1111 */
//#define OPTION_ID_03_RM	(0x0401)/*旧+1 1  4 0100 (0x0401) 0100 0000 0001 */
//#define OPTION_ID_04_RR	(0x0501)/*旧+1 1  4 0101 (0x0701) 0111 0000 0001 */

//#define OPTION_ID_01_LL 	(0x1dff)/*旧-1 0  c 1 1101 (0x01ff) 0001 1111 1111 */
//#define OPTION_ID_02_LM 	(0x1cff)/*旧-1 0  c 1 1100 (0x02ff) 0010 1111 1111 */
//#define OPTION_ID_03_RM 	(0x1401)/*旧+1 1  4 1 0100 (0x0401) 0100 0000 0001 */
//#define OPTION_ID_04_RR 	(0x1501)/*旧+1 1  4 1 0101 (0x0701) 0111 0000 0001 */

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

#define PLAYERS5				(5)

#define BASE_SPEED_ANIME		(PLAYERS5*0)
#define BASE_HIT_BOMB_WAIT		(PLAYERS5*1)
#define BASE_OPT_SHOT_INTERVAL	(PLAYERS5*2)
#define BASE_OPT_SHOT_ANIME 	(PLAYERS5*3)
#define BASE_STD_BOMB_STRENGTH	(PLAYERS5*4)
#define BASE_LOW_BOMB_STRENGTH	(PLAYERS5*5)
//
#define BASE_BOMBS				(PLAYERS5*6)
#define BASE_LIVES				(PLAYERS5*7)
#define BASE_MAX				(PLAYERS5*8)		/* 最大数 */
static Uint8 player_fix_status[BASE_MAX] =
{/* REIMU MARISA REMILIA CIRNO YUYUKO */
	  8,   2,	3,	 1,   4,	/* プレイヤーのアニメーション速度 */
	 16,   8,	4,	 9,  12,	/* 喰らいボムの受付時間 / hit_bomb_wait. */
	  4,   8,	9,	 9,  16,	/* オプションショットの更新間隔 / option shot interval. */
	 12,   6,	3,	 3,  12,	/* オプションショットのアニメーション速度 */
	  5,   5,	3,	 1,   6,	/* 通常ボムの強さ / standard bomb strength. */
	 30,  48,  12,	12,  24,	/* 低速ボムの強さ / lower bomb strength. */
//
	 3,  3,  3,  3,  4, 	/* 復活時のボム数 [初期ボム数](optionが充実したら無くなるかも) */
	 4,  4,  3,  9,  5, 	/* 開始時の残りチャンス [初期プレイヤー数](optionが充実したら無くなるかも) */
};
/*
レミボム: 通常ボムの強さ:[旧==8] [新==4]
旧 3回==((256/64)-1) [24==(8)*3]
新 7回==((256/32)-1) [21==(3)*7]
REIMU		5,[新==5] [旧==3]
MARISA		5,
REMILIA 	3,[新==3] [旧==8]
CIRNO		1,[新==1] [旧==4]
YUYUKO	    6,[新==6] [旧==4]
	通常ボムの強さ / standard bomb strength.
*/


/*---------------------------------------------------------
	プレイヤー、生死判定用コア、生死判定
---------------------------------------------------------*/

static int is_graze;	// player1が弾幕に触れたか？(グレイズ中か否か)
static int pd_player_status;
static int pd_save_timer;

static void player_move_core(SPRITE *s2)
{
	s2->x256 = player->x256+((player->w128-s2->w128))	-(t256(1));
	s2->y256 = player->y256-((s2->h128+s2->h128))		+(t256(20));
	// プレイヤースプライトが弾に触れているか
	if (is_graze)	// グレイズ中？
	{
		#if 0
		/* 復活中はアイテム以外当たり判定が無いのでここに来ない筈(きたらバグ) */
		if (PLAYER_STATE_00_NORMAL == pd_player_status)
		#endif
		{
			SPRITE *t;
			// (グレイズ中ならコアと敵弾のあたり判定をする)
			if (NULL != (t=sprite_collision_check(s2,SP_GROUP_BULLETS)))
			{
				play_voice_auto_track(VOICE04_SHIP_HAKAI);		// [***090127	変更先
				pd_player_status	= PLAYER_STATE_01_HIT_BOMB;
				pd_save_timer		= player_fix_status[BASE_HIT_BOMB_WAIT+select_player]/*0*/;
			//	((PLAYER_DATA *)player->data)->enemy = t;/* あたった敵を覚えとく */
				/* あたった敵は、必ず死ぬ訳では無いので、あたった敵の体力を減らすべき */
				/**/
			}
		}
	}
	is_graze=0;/* グレイズ中おわり */
}

/*---------------------------------------------------------
	プレイヤー武器のあたり判定
---------------------------------------------------------*/

static void hit_work_zako(SPRITE *s, SPRITE *player_tama)
{
	ENEMY_BASE *zako_data;
	zako_data	= (ENEMY_BASE *)s->data;
	WEAPON_BASE *player_tama_data = (WEAPON_BASE *)player_tama->data;
	zako_data->health -= player_tama_data->strength;	/* 強さ分引く */
	if (0 >= zako_data->health) 	/* ０か負値なら、倒した。 */
	{
		explosion_add_type(s->x256,s->y256+t256(5),/*0,*/EXPLOSION_ZAKO04/*EXPLOSION_MINI00*/);/*ザコ消滅爆発*/
		//parsys_add(spimg, 2,2, c->x,c->y, 10, angle, 10, 50, EXPLODE|DIFFSIZE, NULL);
//		int up_flags;	up_flags=(player_tama->type==(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO))?(ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY):(ITEM_MOVE_FLAG_06_RAND_XY);
		if (NULL != s->callback_loser)
		{
			(s->callback_loser)(s);
			s->callback_loser = NULL;
		}
		bonus_info_any_score_nodel(s, ((zako_data->score)*(/*pd->now_stage*/player_now_stage/*level*/)) );/*自動消去へ仕様変更*/s->type = SP_DELETE;
		play_voice_auto_track(VOICE08_DEATH);
	}
}

extern void enemy_zako_yokai1_nonshield_hitbyweapon(SPRITE *s, SPRITE *t/*, int angle*/);

	/* wenn flag erase_bullets gesetzt ist, werden auch gegnerische schuesse getroffen */
	/* wenn flag erase_player_tama gesetzt ist, wird der Schuss vernichtet */
//PLAYER_WEAPON_TYPE_00_SHOT /*int angle,*/ int 1==erase_player_tama, int 0==erase_bullets
//PLAYER_WEAPON_TYPE_01_BOMB /*int angle,*/ int 0==erase_player_tama, int 1==erase_bullets
enum /*_player_state_*/
{
	PLAYER_WEAPON_TYPE_00_SHOT = 0, 	/* ショット系 */	/* 自弾が敵にあたった場合に自弾が消滅する */	/* 敵弾にあたると敵弾を消さない */
	PLAYER_WEAPON_TYPE_01_BOMB, 		/* ボム系 */		/* 自弾が敵にあたった場合に自弾が消滅しない */	/* 敵弾にあたると敵弾を消す */
};
static void player_weapon_colision_check(SPRITE *s, int erase_shot_type)
{
//	SPRITE *s;		// 自弾
	SPRITE *tt; 	// 敵スプライト、または、敵弾スプライト
//	SDL_Surface *spimg;
//	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;

	/* 敵弾にあたった場合に敵弾を消す */
	if (/*PLAYER_WEAPON_TYPE_01_BOMB==*/erase_shot_type/*erase_bullets*/)	/* ボム系のみ */
	{
		if (NULL != (tt=sprite_collision_check(s,SP_GROUP_BULLETS)))		/* 敵弾？ */
		{
			tt->type = SP_DELETE;	/* 敵弾が消滅 */
			play_voice_auto_track(VOICE02_ZAKO_HAKAI);
			//spimg=sprite_getcurrimg(c);
			//parsys_add(spimg, 2,2, c->x,c->y, 10, angle, 10, 30, EXPLODE|DIFFSIZE, NULL);
			//SDL_FreeSurface(spimg);
		}
	}
	/* 敵にあたった場合に敵を消す */
	if (NULL != (tt=sprite_collision_check(s,(SP_GROUP_ZAKO|SP_GROUP_BOSS)/*SP_GROUP_ENEMYS*/)))	/* 敵？ */
	{
		switch (tt->type)
		{
		//ボスに当たったのがシールドだった場合はダメージ処理を行わない
		case SP_CHUU:	/*not_break;*/		// ボスと共用
		case SP_BOSS:
			#if (0)/*[debug]*/
			((PLAYER_DATA *)player->data)->my_score = s->type;
			#endif /*(0000)*/
		//	if ((SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) != s->type)
			if (0==(SP_GROUP_SHOT_BOSS & s->type))
			{
				;
			}
			else
			{
				(((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon)(tt, s/*,angle*/);
			}
			break;
		default:
			/* あたったのは本当にザコ類？ */
			if (SP_ZAKO==(tt->type&(SP_GROUP_ZAKO|SP_GROUP_BOSS)/*SP_GROUP_ENEMYS*/))
			{
				hit_work_zako(tt, s);/*本当にザコ類*/
			}
			break;
		}
		/* 自弾が敵にあたった場合に自弾が消滅する */
		if (/*erase_player_tama*/PLAYER_WEAPON_TYPE_00_SHOT == erase_shot_type) 	/* ショット系のみ */
		{
			s->type = SP_DELETE;	/* 自弾が消滅 */
		}
	}
}


/*---------------------------------------------------------
	プレイヤー、針弾の移動
	霊夢 ＆ 魔理沙 ＆ レミ ＆ チルノ ＆ 幽々子
---------------------------------------------------------*/

static void player_move_needle(SPRITE *s)
{
	PL_SHOT_DATA *data=(PL_SHOT_DATA *)s->data;
	s->x256 += data->vx256;//co_s512((data->angle512))*data->speed/**fps_fa_ctor*/;
	s->y256 += data->vy256;//si_n512((data->angle512))*data->speed/**fps_fa_ctor*/;
	if (REMILIA==select_player) 	/* レミはゆらゆら */
	{	int rand_int;
		rand_int = ra_nd();
		s->x256 -= (rand_int&0x0100);
		s->x256 += (rand_int&0x0080);
		s->x256 += (rand_int&0x0080);
	}
	else
	if (YUYUKO==select_player)		/* 幽々子はそれる */
	{
		PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
		data->vx256 += ((data->vx256*pd->weapon_power)>>11);
	}
	else
	if (REIMU==select_player)		/* 霊夢の回転ショット */
	{
		if (((JIKI_SHOT_01)|SP_GROUP_SHOT_BOSS)==s->type)
		{
			/* 描画用グラ回転 */
			s->m_angleCCW512 +=	16/*deg_360_to_512CCW((10))*/;
			mask512(s->m_angleCCW512);
		}
	}
	if (s->y256 < t256(-10))
	{
		s->type = SP_DELETE;
	}
	else
	{
		player_weapon_colision_check(s, PLAYER_WEAPON_TYPE_00_SHOT);
	}
}

/*---------------------------------------------------------
	プレイヤー、ショット弾の移動
	霊夢 ＆ 魔理沙 ＆ レミ ＆ チルノ ＆ 幽々子
---------------------------------------------------------*/
#define player_move_shot player_move_needle

/*---------------------------------------------------------
	敵を探す子関数
---------------------------------------------------------*/
extern SPRITE *sprite_list000_head;
static /*int*/SPRITE *search_enemy_by_sprite(void)
{
	/* Suche Enemy-Sprite, das noch nicht verfolgt wird, liefert SPRITE-ID oder -1 */
	SPRITE *s = sprite_list000_head;/* スプライト リストの先頭 から探す */
	while (NULL != s)/* スプライト リストの最後まで調べる */
	{
		if (
			#if 1
			(SP_DELETE != s->type ) && /* 削除済みは飛ばす */
			#endif
			(0 != (s->type & (SP_GROUP_ZAKO|SP_GROUP_BOSS))/*SP_GROUP_ENEMYS*/) 	/* プレイヤーにとっての敵(ザコやボス) */
		)
		{
			if (
#if 0
/* SP_FLAG_LOCK_TARGET この考えは欠陥があるな */
//				(0 == (s->flags & SP_FLAG_LOCK_TARGET)) &&							/* 同一フラグが無い敵(別の誘導弾が見つけていない敵) */
#endif
				(s->flags & SP_FLAG_VISIBLE)										/* 使用中の敵 */
			)
			{
				if ((s->x256 > 0) && (s->x256 < t256(GAME_WIDTH )-((s->w128+s->w128))) &&
					(s->y256 > 0) && (s->y256 < t256(GAME_HEIGHT)-((s->h128+s->h128)))) 	/* 表示画面内の敵 */
				{
					return (s)/*->id*/;/*見つけたよ*/
				}
			}
		}
		s = s->next;/*次*/
	}
	return (NULL)/*-1*/;/*見つからなかった*/
}

/*---------------------------------------------------------
	プレイヤー、誘導弾の移動
---------------------------------------------------------*/

enum
{
	SH_00 = 0,
//	SH_01,
	SH_02,
	SH_03,

};


static void player_move_parrent_hlaser(SPRITE *s)
{
	PL_HOMING_DATA *data;
	data=(PL_HOMING_DATA *)s->data;
//
	SPRITE *target;
	target = NULL;	/* Target */
//	SPRITE *tg;
//	tg=(SPRITE *)data->tg_sprite; /* Target-Fadenkreuz */	/* ターゲット目標表示マーカー */

	switch (data->state)
	{
	case SH_00: /* 回転しながら正面を向く。 / Raketen in Ausgangspos. bringen (Sprengkopf nach Norden) */
#if 0
/* CWの場合 */
		if (0==data->nr)	{	data->angle512+=deg_360_to_512((8))/**fps_fa_ctor*/; }	/* 右回りに回転させる。/ linke Rakete: rot. im UZ */
		else				{	data->angle512-=deg_360_to_512((8))/**fps_fa_ctor*/; }	/* 左回りに回転させる。/ rechte Rekete: rot. gegen UZ */
#else
/* CCWの場合 */
		if (0==data->nr)	{	data->angleCCW512-=deg_360_to_512CCW(/*360-*/(8))/**fps_fa_ctor*/; }	/* 右回りに回転させる。/ linke Rakete: rot. im UZ */
		else				{	data->angleCCW512+=deg_360_to_512CCW(/*360-*/(8))/**fps_fa_ctor*/; }	/* 左回りに回転させる。/ rechte Rekete: rot. gegen UZ */
#endif
		//
		mask512(data->angleCCW512);
		//
		if ( t256(1.0) < data->speed256 )
		{
			data->speed256 -= /*(int)*/t256(0.5);/*減速*/
		}
		//
#if 0
/* CWの場合 */
		if ((data->angle512>=deg_360_to_512((270-4)/*266*/))&&
			(data->angle512<=deg_360_to_512((270+4)/*274*/))) /* ほぼ正面(270)を向いたら */
		{
			/* Sprengkopf in Ausgangspos? */
			data->state = SH_02;						/* 次へ */
			data->angle512=deg_360_to_512((270)); /* 向きを正面(270)に固定する */
		}
#else
/* CCWの場合 */
		if ((data->angleCCW512>=deg_360_to_512CCW((180-4)/*266*/))&&
			(data->angleCCW512<=deg_360_to_512CCW((180+4)/*274*/))) /* ほぼ正面(180)を向いたら */
		{
			/* Sprengkopf in Ausgangspos? */
			data->state = SH_02;						/* 次へ */
			data->angleCCW512=deg_360_to_512CCW((180)); /* 向きを正面(180)に固定する */
		}
#endif
		break;
	case SH_02: /* 敵を探す。 / Ziel suchen */
		data->target_obj/*target_id*/ = search_enemy_by_sprite();
		if (NULL/*-1*/ != data->target_obj/*target_id*/)
		{
			target=data->target_obj/*sp rite_get_by_id (data->target_id)*/;
			if (NULL != target)
			{
				data->state = SH_03;						/* 次へ */
#if 0
/* SP_FLAG_LOCK_TARGET この考えは欠陥があるな */
				target->flags|=SP_FLAG_LOCK_TARGET; /* 「一つしかないですよ」フラグON */
#endif
			//	tg->flags|= SP_FLAG_VISIBLE;	/* ターゲット目標表示マーカー */
			}
		//	else
		//	{
		//	//	tg->flags&= (~(SP_FLAG_VISIBLE));	/* ターゲット目標表示マーカー */
		//	}
		}
		break;
	case SH_03: 			/* Ziel verfolgen */
		/* 他の誘導ボムが、既に倒したか？ */
		if (NULL/*-1*/ == /*data->target_id*/data->target_obj)	/* 見つからない(他の誘導ボムが倒してしまった場合) */
		{
			/* SP_FLAG_LOCK_TARGETを止めたのでここにくる */
			data->state = SH_02;	/* ターゲット検索からやり直す。 */
		}
		else	/* まだターゲットが生きてる */
		{
			if ( t256(12.0) > data->speed256)
			{
				data->speed256 += /*(int)*/t256(0.5);/*加速*/
			}
			target=data->target_obj/*sp rite_get_by_id (data->target_id)*/;
			if (//(NULL != target) &&	/* 重複してるので排除 */
				(target->flags&SP_FLAG_VISIBLE) &&
				(target->x256 > t256(0)) &&
				(target->x256 < (t256(GAME_WIDTH))-((target->w128+target->w128))) &&
				(target->y256 > t256(0)) &&
				(target->y256 < (t256(GAME_HEIGHT))-((target->h128+target->h128))))
			{
#if (1)/*誘導(000)*/
				{int ta512;
#if 0
/* CWの場合 */
					ta512=atan_512(target->y256-s->y256,target->x256-s->x256);
					if (data->angle512>ta512)
					{
							 if (data->angle512-ta512<deg_360_to_512((175)))	{	data->angle512-=deg_360_to_512((8))/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
						else if (data->angle512-ta512>deg_360_to_512((185)))	{	data->angle512+=deg_360_to_512((8))/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
					}
					else if (data->angle512<ta512)
					{
							 if (ta512-data->angle512<deg_360_to_512((175)))	{	data->angle512+=deg_360_to_512((8))/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
						else if (ta512-data->angle512>deg_360_to_512((185)))	{	data->angle512-=deg_360_to_512((8))/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
					}
#else
/* CCWの場合 */
//					ta512=atan_512(target->y256-s->y256,target->x256-s->x256);
					ta512=atan_512(target->y256-s->y256,target->x256-s->x256);
					#if 0
					/* 遅れ誘導 */
					/* 作ってない */
					if (data->angleCCW512>ta512)
					{
							 if (data->angleCCW512-ta512<deg_360_to_512CCW((175)))	{	data->angleCCW512+=deg_360_to_512CCW((8))/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
						else if (data->angleCCW512-ta512>deg_360_to_512CCW((185)))	{	data->angleCCW512-=deg_360_to_512CCW((8))/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
					}
					else if (data->angleCCW512<ta512)
					{
							 if (ta512-data->angleCCW512<deg_360_to_512CCW((175)))	{	data->angleCCW512-=deg_360_to_512CCW((8))/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
						else if (ta512-data->angleCCW512>deg_360_to_512CCW((185)))	{	data->angleCCW512+=deg_360_to_512CCW((8))/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
					}
					#else
					/* 直接誘導 */
					data->angleCCW512 = ta512;
					#endif
#endif
				}
				mask512(data->angleCCW512);
			//	tg->x=target->x+((target->w-tg->w)>>1)+ra_nd()%6-3; /* ターゲット目標表示マーカー */
			//	tg->y=target->y+((target->h-tg->h)>>1)+ra_nd()%6-3; /* ターゲット目標表示マーカー */
#endif/*誘導(000)*/
			}
			else
			{
				/* Ziel wurde anderweitig vernichtet, neues Ziel suchen */
				data->target_obj = NULL/*data->target_id=-1*/;		/* 画面外に逃げられたら */
				data->state = SH_02;	/* 戻る */
			//	tg->flags&= (~(SP_FLAG_VISIBLE));	/* ターゲット目標表示マーカー */
			}
		}
		break;
	}
	#if 1
	/* (多分必要ないけど)念の為マスク */
	mask512(data->angleCCW512);
	#endif
#if 0
/* CWの場合 */
	s->x256 += ((cos512((data->angle512))*data->speed256)>>8)/**fps_fa_ctor*/;
	s->y256 += ((sin512((data->angle512))*data->speed256)>>8)/**fps_fa_ctor*/;
#else
/* CCWの場合 */
	s->x256 += ((sin512((data->angleCCW512))*data->speed256)>>8)/**fps_fa_ctor*/;
	s->y256 += ((cos512((data->angleCCW512))*data->speed256)>>8)/**fps_fa_ctor*/;
#endif
	#if 0
	/* homing */
	//	{	int ddd_angle512;
	//		ddd_angle512 = data->angle512+deg_360_to_512((180));
	//		mask512(ddd_angle512);
	//		//parsys_add(NULL,20,0,s->x+((s->w)>>1),s->y+((s->h)>>1),20,ddd_dangle512,30,10,PIXELATE,NULL);
	//	}
		#if 0
		s->anim_frame=((data->angle512*20)/512) % 20;
		#else
		s->anim_frame=((data->angle512*10)>>8);
		#endif
	#endif
	player_weapon_colision_check(s, PLAYER_WEAPON_TYPE_00_SHOT);

	data->time_out--/*range-=fps_fa_ctor*/;
	if (data->time_out/*range*/<=0) /* 時間切れ？ / max. Reichweite erreicht? */
	{
		s->type = SP_DELETE;		/* 時間切れなのでおしまい */
#if 0
/* SP_FLAG_LOCK_TARGET この考えは欠陥があるな */
	//	tg->type = SP_DELETE;	/* ターゲット目標表示マーカー*/
		if (NULL != target) 	/* ターゲットがまだ生きてたら、 Target freigeben */
		{
			target->flags &= (~(SP_FLAG_LOCK_TARGET)); /* 弄ったフラグ戻しとこう。 */
		}
#endif
	}
}
		//	/* ...orz ありえねえ */ error(ERR_WARN, "homing: target_id==-1&&state==3 ->back to state 2 (this shouldn't happen)");

static void player_move_kodomo_hlaser(SPRITE *s)
{
	#if 1
	PL_HOMING_KODOMO_DATA *data=(PL_HOMING_KODOMO_DATA *)s->data;
	SPRITE *oya;
	oya = data->target_obj;
	if (	(SP_DELETE == oya->type) || 			/* 親が消去済みならば子も消去 */
		//	((SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_BOSS) != oya->type)			/* (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_BOSS)以外は親でないので消去 */
			(0==(SP_GROUP_SHOT_BOSS & oya->type))	/* (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_BOSS)以外は親でないので消去 */
		)
	{
		s->type = SP_DELETE;/*おしまい*/
	}
//
	s->x256 = oya->x256;
	s->y256 = oya->y256;

	/* 数フレーム動かない場合は、自動消去する */
	if ((data->check_x256 != s->x256) ||
		(data->check_y256 != s->y256))
	{
		data->time_out	= 8;/* 動いたら自動消去しない */
	}
	data->time_out--;
	if ( 1 > data->time_out)
	{
		s->type = SP_DELETE;/*おしまい*/
	}
	data->check_x256 = s->x256;
	data->check_y256 = s->y256;
	#endif
}

/*---------------------------------------------------------
	プレイヤー、高速時ボムの移動(霊夢)
---------------------------------------------------------*/

/*
	s->x		シールドのx座標
	player->x	プレイヤーのx座標
	player->w/2 プレイヤーの横幅の半分
	co_s512((data->angle512))	angleの角度をπに直したあとのコサイン。この場合は40°の倍数＋１０*fpsのコサイン。
	data->radius	円の半径40
	s->w/2		シールドの横幅の半分
	プレイヤーのx座標＋プレイヤーの横幅の半分＝プレイヤーの中心座標
	なので
	シールドのx座標＝プレイヤーのx座標＋プレイヤーの横幅の半分＋co s(radian((４０°*x)＋speed*fps))×40－シールドの横幅の半分
				   ＝プレイヤーの中心座標＋co s(radian((４０°*x)＋speed*fps))×40－シールドの横幅の半分
*/
// 時計回り ＆ 反時計回り 兼用
static void player_move_kekkai(SPRITE *s)	/* 霊夢 */
{
	PL_KEKKAI_DATA *data	= (PL_KEKKAI_DATA *)s->data;
	data->angleCCW512 += (data->add_r512/**fps_fa_ctor*/);/*...*/
	mask512(data->angleCCW512);// if (data->angle>360) {	data->angle-=360;}
	#if 1
	/* 描画用角度(下が0度で左回り(反時計回り)) */
	s->m_angleCCW512	= (data->angleCCW512);
	#endif
//
#if 0
	/* CWの場合 */
	s->x256 = player->x256 + cos512((data->angle512))*data->radius+((player->w128-s->w128));
	s->y256 = player->y256 + sin512((data->angle512))*data->radius+((player->h128-s->h128));
#else
	/* CCWの場合 */
	s->x256 = player->x256 + sin512((data->angleCCW512))*data->radius+((player->w128-s->w128));
	s->y256 = player->y256 + cos512((data->angleCCW512))*data->radius+((player->h128-s->h128));
#endif

//	if (REIMU==select_player)
	{
		if (data->radius<255)	{	data->radius+=(ra_nd()&1);	}	/* 少しずつ広がるようにした */
		else					{	s->type = SP_DELETE;		}	/* 回転が広がったらおしまい */
		s->alpha			= ((int)(255-data->radius))&(256-1);	/* だんだん消えるようにした */
	}
//	else //if (YUYUKO==select_player)
//	{
//		PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
//		if (pd->bomber_time<=0) {	s->type = SP_DELETE;}/* 時間制限でおしまい */
//	}
	player_weapon_colision_check(s, PLAYER_WEAPON_TYPE_01_BOMB);/* ボムで敵を倒すあたり判定 */
}



/*---------------------------------------------------------
	プレイヤー、高速時ボム。画面外はみ出しチェック。あたり判定チェック。
---------------------------------------------------------*/
static void player_bomber_out_colision_check(SPRITE *s)
{
	if ((s->x256+((s->w128+s->w128)) < t256(0))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256+((s->h128+s->h128)) < t256(0))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type = SP_DELETE;/* 画面外に出たらおしまい */
	}
	else
	{
		player_weapon_colision_check(s, PLAYER_WEAPON_TYPE_01_BOMB);/* ボムで敵を倒すあたり判定 */
	}
}

/*---------------------------------------------------------
	プレイヤー、高速時ボムの移動(魔理沙、チルノ、仮幽々子(違う感じにしたい) )
---------------------------------------------------------*/

static void player_move_levarie(SPRITE *s)
{
	PL_LEVARIE_DATA *data=(PL_LEVARIE_DATA *)s->data;
	s->x256 += data->vx256;//co_s512((data->angle512))*d->speed/**fps_fa_ctor*/;
	s->y256 += data->vy256;//si_n512((data->angle512))*d->speed/**fps_fa_ctor*/;
	if (CIRNO==select_player)
	{	/*1.5*(d->speed)*//**fps_fa_ctor*/;/*チルノ用(暫定的)*/
		s->y256 -= (abs((data->vx256+(data->vx256>>1) )));
		s->y256 -= (abs((data->vy256+(data->vy256>>1) )));
		s->y256 -= t256(1);
	}
	/* YUYUKO 以外は回転/拡大する。 */
	//if (YUYUKO != select_player)
	//if (0==(ra_nd()&0x40))/* きまぐれ回転/拡大 */
	{
		u8 aaa_sss[16] =
		{/* REIMU MARISA REMILIA CIRNO YUYUKO */
			0,	2,	0,	9, 0,	/* 回転速度 */
			0,	2,	0,	3, 0,	/* 拡大速度 */
			0,	0,	0,	0, 0, 0,
		};
		/*(暫定的)*/
	//	if (0==(ra_nd()&0x40))/* きまぐれ回転 */
		{	/* 表示角度(下が0度で左回り(反時計回り)) */
			s->m_angleCCW512		+= aaa_sss[select_player]/*10*/;
		}
		mask512((s->m_angleCCW512));
		if ( t256(4.0) > s->m_zoom_x256)
		{
		//	s->m_zoom_x256 += 1/*8*/;
			s->m_zoom_x256 += aaa_sss[select_player+(PLAYERS5)]/*8*/;
		}
	}
	player_bomber_out_colision_check(s);
}

/*---------------------------------------------------------
	プレイヤー、高速時ボムの移動(レミリア)
---------------------------------------------------------*/

static void remilia_move_burn_fire(SPRITE *s)
{
	OZ_BURN_FIRE_DATA *data=(OZ_BURN_FIRE_DATA *)s->data;
	#if 1
	// 加速
	// x1.10
//	data->vx256 = ((data->vx256 * t256(1.1))>>8);//co_s512((data->angle512))*p->speed/**fps_fa_ctor*/;
//	data->vy256 = ((data->vx256 * t256(1.1))>>8);//si_n512((data->angle512))*p->speed/**fps_fa_ctor*/;
	// x1.125
	data->vx256 += ((data->vx256)>>3);//co_s512((data->angle512))*p->speed/**fps_fa_ctor*/;
	data->vy256 += ((data->vy256)>>3);//si_n512((data->angle512))*p->speed/**fps_fa_ctor*/;
	// x1.0625
//	data->vx256 += ((data->vx256)>>4);//co_s512((data->angle512))*p->speed/**fps_fa_ctor*/;
//	data->vy256 += ((data->vy256)>>4);//si_n512((data->angle512))*p->speed/**fps_fa_ctor*/;
	/* 描画が重すぎるので加速性能、若干速く( x1.0625→ x1.125)する */
	//
	#endif
	s->x256 += data->vx256;//co_s512((data->angle512))*p->speed/**fps_fa_ctor*/;
	s->y256 += data->vy256;//si_n512((data->angle512))*p->speed/**fps_fa_ctor*/;
//	if ((s->x256+((s->w128+s->w128)) < t256(0))||(s->x256 > t256(GAME_WIDTH))||
//		(s->y256+((s->h128+s->h128)) < t256(0)))
//
	player_bomber_out_colision_check(s);
}

/* 十字炎ボムの炎の部分 */
/*		r_or_l	[xxx_r] l=2, [xxx_l] r=0 ,u=3 ,d=1	*/
static void remilia_add_burn_fire(SPRITE *s/*, int ggg*/ /*r_or_l*/)	/* [***090221 追加 */
{
		#if 0
	int hhh;
	hhh = BASE_BOMBER2_PNG_oz;
		#endif
	int angCCW512;
//	for (ggg=0; ggg<3; ggg++)
	for (angCCW512=0; angCCW512<512; angCCW512+=5/*10*/)
	{
		SPRITE *c;
		#if 0
		/*const*/ char *bbb_name[3] =
		{
			"bomber2_oz.png"/*"fire_wind_l.png"*/,	/*move left*/
			"bomber3_oz.png"/*"fire_wind_r.png"*/,	/*move right*/
			"bomber4_oz.png"/*"fire_wind_u.png"*/,
		};
		c = spr ite_add_file 0((char *)bbb_name[(ggg)], 4, PRIORITY_01_SHOT/*PRIORITY_02_PLAYER*/, 0);
		#endif
	//	c = sprite_add_res(hhh);hhh += PLAYERS5;
		c = sprite_add_bullet(JIKI_ATARI_ITEM_16);
		//
		c->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
		c->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_02|SP_GROUP_SHOT_BOSS)/*ボスに有効*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/
		c->anim_speed		= 3;
		c->anim_frame		= 0;
		c->callback_mover	= remilia_move_burn_fire;
		c->alpha			= 100;
		c->x256 			= s->x256+((s->w128-c->w128));
	//		 if (0==ggg/*2==r_or_l*/)	{	c->y			= s->y+15;}
	//	else if (1==ggg/*0==r_or_l*/)	{	c->y			= s->y+15;}
	//	else if (2==ggg/*3==r_or_l*/)	{	c->y			= s->y+15;	c->y -= s->h;}
	//
		c->y256 			= s->y256+t256(15);
//		if (2==ggg/*3==r_or_l*/)	{	c->y256 -= ((s->h128+s->h128));}
	//
		OZ_BURN_FIRE_DATA *data;
		data				= mmalloc(sizeof(OZ_BURN_FIRE_DATA));
		c->data 			= data;
	//
	//	data->speed 		= 3/*12*/;/*もうちょっと遅く(遅い方が長く残るので強い)*/
	//	data->strength		= 16/*9*/ /*10*/;/* やっぱ詐欺っぽいくらい強くていいや。当てるの難しいし */
		/* レミリアのボムは当てなければ減らないので(外すと減らない)若干強すぎるくらいでいい */
		/* ただ、将来あたり判定が設定できるようになったら、あたり判定は多少小さくしたい所 */

		data->strength		= 5/*9*/ /*10*/;/* 仕様変更した */
		{
		//	int rnd_spd 		= (ra_nd() & 0xff/*angCCW512*/)+256+((angCCW512+32+64) & 0x40);
			int aaa_spd 		= (((angCCW512+32+64) & 0x40));
			int rnd_spd 		= (ra_nd() & 0xff/*angCCW512*/)+256+(aaa_spd+aaa_spd);
#if 0
	/* CWの場合 */
			data->vx256 	= ((cos512((/*data->angle512*/ang512))*/*data->speed*/(rnd_spd)/**fps_fa_ctor*/)>>8);
			data->vy256 	= ((sin512((/*data->angle512*/ang512))*/*data->speed*/(rnd_spd)/**fps_fa_ctor*/)>>8);
#else
	/* CCWの場合 */
			data->vx256 	= ((sin512((/*data->angle512*/angCCW512))*/*data->speed*/(rnd_spd)/**fps_fa_ctor*/)>>8);
			data->vy256 	= ((cos512((/*data->angle512*/angCCW512))*/*data->speed*/(rnd_spd)/**fps_fa_ctor*/)>>8);
#endif
		}
		#if 1
		/* 描画用角度(下が0度で左回り(反時計回り)) */
		c->m_angleCCW512		= angCCW512;
		#endif
	}
}

/*---------------------------------------------------------
	幽々子ボムの扇
---------------------------------------------------------*/

static void player_move_add_oogi(SPRITE *s) /* [***090220 追加 */
{
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	if ((255-32) < pd->bomber_time)
	{
	//	s->m_zoom_x256		= ((1+255-pd->bomber_time)<<3);
		s->m_zoom_x256		= ((1+255-pd->bomber_time)<<4);
	}
	else
	{
		s->m_zoom_x256		-= (1);
	}
	if ((32) > pd->bomber_time)/* 残り時間が 1/8 未満なら、警告する */
	{
		s->m_zoom_x256		-= (8);
	}
	if ((0) > s->m_zoom_x256)
	{
	//	s->flags		&= (~SP_FLAG_VISIBLE);/*表示off*/
		s->type = SP_DELETE;	/* 扇縮小でおしまい */
	}

	#if 0
	data->oogi_x_offset256	= t256(11-100);
	s->x256 = player->x256+(data->oogi_x_offset256)/*+((player->w128-s->w128))*/;
	#else
	s->x256 = player->x256+(t256(11-100)/*oogi_x_offset256*/)/*+((player->w128-s->w128))*/;
	#endif
//	s->y256 = player->y256-t256(64)/*+((player->h128-s->h128))*/;		/* 幽々子が下側(601氏案、扇が消える時に変) */
	s->y256 = player->y256-t256((104/2))/*+((player->h128-s->h128))*/;	/* 幽々子が中心(妖々夢風、妖々夢の幽々子は扇の中心にいるよ) */
//
	#if 1/*要らないかも？*/
	if (1 > pd->bomber_time) { s->type = SP_DELETE;}/* 時間制限でおしまい */
	#endif
}

/*---------------------------------------------------------
	レミリアボムの十字炎
---------------------------------------------------------*/

static void player_move_add_cross_red(SPRITE *s)	/* [***090220 追加 */
{
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	//
//	if (0x3f==(((int)pd->bomber_time)&0x3f))
//	if (0x0f==(((int)pd->bomber_time)&0x0f))
	if (0x1f==(((int)pd->bomber_time)&0x1f))/*波動を撃つタイミング*/
	{
		remilia_add_burn_fire(s);/*波動発動*/
	}
	s->x256 = player->x256+((player->w128-s->w128));
	s->y256 = player->y256+((player->h128-s->h128));

	if (1 > pd->bomber_time)	{ s->type = SP_DELETE;}/* 時間制限でおしまい */
}


/*---------------------------------------------------------
	プレイヤー、針弾の追加(霊夢)
	[針弾]、(魔理沙)[森弾]、幽々子[蝶弾] (レミリア、仮チルノ)[コウモリ弾]
---------------------------------------------------------*/
enum
{
	NEEDLE_ANGLE_270=0,
	NEEDLE_ANGLE_263,	/* レミリア 用 */
	NEEDLE_ANGLE_277,	/* レミリア 用 */
	NEEDLE_ANGLE_271,	/* 幽々子 用 */
};
static void player_add_needle(SPRITE *s, int x_offs, int y_offs, int needle_type )
	// 霊夢、魔理沙、幽々子 レミ	/* [***090220 追加 */
{
	SPRITE *shot; //spr ite_add_file 0("fireball1.png",2,PRIORITY_02_PLAYER,1); shot->anim_speed=1;
//	shot					= sprite_add_res( BASE_NEEDLE_PNG+select_player );
	shot					= sprite_add_bullet( BASE_NEEDLE_ATARI_0a+select_player );
	//spr ite_add_file 0((char *)aaa_name[(k)],1,PRIORITY_02_PLAYER,0); shot->anim_speed=0;
	/*REMILIA*/  //spr ite_add_file 0("bat.png",5,PRIORITY_02_PLAYER,0); shot->anim_speed=3;
	shot->type				= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_SHOT_07|SP_GROUP_SHOT_BOSS)/*SP_PL_FIREBALL*/;
	shot->x256				= s->x256+((s->w128-shot->w128)) + x_offs;
	shot->y256				= s->y256-t256(7) + y_offs;
//	if (REIMU != select_player) //霊夢はアルファなし	霊夢もアルファあり(本家見直したらアルファあったから)
	{
		shot->alpha 		= 150;
	}
	shot->callback_mover	= player_move_needle;/*player_move_koumori*/
	shot->flags 			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
	PL_SHOT_DATA *data;
	data					= mmalloc(sizeof(PL_SHOT_DATA));
	shot->data				= data;
//
	#define DDD_STRENGTH		(PLAYERS5*0)
	#define DDD_SPEED			(PLAYERS5*1)
	#define DDD_MAX 			(PLAYERS5*2)		/* 最大数 */
	static Uint8 ddd_tbl[DDD_MAX] =
	{/* REIMU MARISA REMILIA CIRNO YUYUKO */
		 2, 4, 6, 6, 8, 	/* strength ショットの強さ */	/*REMILIA, CIRNO,  6 5 強すぎる*/
		18,10, 3, 3, 4, 	/* speed	ショットの速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */
	};
	data->strength			= ddd_tbl[DDD_STRENGTH+select_player];
//	p_speed 				= ddd_tbl[DDD_SPEED+select_player];
//	data->strength			= (4);MARISA	data->strength		= /*3*/2;REIMU	/*強すぎる*/
//	data->angle512			= bbb[needle_type]/*deg_360_to_512((270))*/;
//	data->speed 			= 10;	/*REMILIA, CIRNO,  3 8 強すぎる*/
//	data->vx256 			= cos512((/*angle512*/deg_360_to_512((270))))*/*p->speed*/(ddd_tbl[DDD_SPEED+select_player])/**fps_fa_ctor*/;
//	data->vy256 			= sin512((/*angle512*/deg_360_to_512((270))))*/*p->speed*/(ddd_tbl[DDD_SPEED+select_player])/**fps_fa_ctor*/;
#if 0
/* CWの場合 */
	{
		const signed short bbb512[4] =
		{
			deg_360_to_512((270)),
			deg_360_to_512((270-25)),/*263=270-7*/	/* レミリア 用 */
			deg_360_to_512((270+25)),/*277=270+7*/	/* レミリア 用 */
			deg_360_to_512((271)),					/* 幽々子 用 */
		};
		data->vx256 		= cos512((/*angle512*/bbb512[needle_type]))*/*p->speed*/(ddd_tbl[DDD_SPEED+select_player])/**fps_fa_ctor*/;
		data->vy256 		= sin512((/*angle512*/bbb512[needle_type]))*/*p->speed*/(ddd_tbl[DDD_SPEED+select_player])/**fps_fa_ctor*/;
	}
#else
/* CCWの場合 */
	{
		const signed short bbb512[4] =
		{
			deg_360_to_512CCW((180)),
			deg_360_to_512CCW((180-25)),/*263=270-7*/	/* レミリア 用 */
			deg_360_to_512CCW((180+25)),/*277=270+7*/	/* レミリア 用 */
			deg_360_to_512CCW((181)),					/* 幽々子 用 */
		};
		data->vx256 		= sin512((/*angle512*/bbb512[needle_type]))*/*p->speed*/(ddd_tbl[DDD_SPEED+select_player])/**fps_fa_ctor*/;
		data->vy256 		= cos512((/*angle512*/bbb512[needle_type]))*/*p->speed*/(ddd_tbl[DDD_SPEED+select_player])/**fps_fa_ctor*/;
	}
#endif
}

/*---------------------------------------------------------
	プレイヤー、オプションの移動制御
---------------------------------------------------------*/
static void reimu_marisa_control_option(SPRITE *s, PL_OPTION_DATA *data, PLAYER_DATA *pd) /* 霊夢、魔理沙 */
{
	mask512(data->angleCCW512);

	{int vv512;
		vv512 = (YUYUKO==select_player)?(3):(7);	/*ra d2deg512(0.1) == 8.1*/
		if (my_pad & PSP_KEY_SLOW)		/* 押すと前方に移動 */
		{
			if ((OPTION_ID_03_RM/*1*/&0xff)==(data->opt_anime_add_id&0xff)/*pos*/) /* 1: 右側系のオプション(OPTION_ID_03_RM, OPTION_ID_04_RR ) */
			{
				if (data->angleCCW512 < /*(288)*/(256+16	-16-64+(7<<4)+((data->opt_anime_add_id&0x100)>>2)-(vv512<<4) )/*272 (256+16)*/ )/*中位(右)位置*/
				{
					data->angleCCW512 += vv512;
				}
			}
			else	/* 0: 左側系のオプション */
			{
				if (data->angleCCW512 > /*(224)*/(256-16	+16+64-(7<<4)-((data->opt_anime_add_id&0x100)>>2)+(vv512<<4) )/*240 (256-16)*/ )/*中位(左)位置*/
				{
					data->angleCCW512 -= vv512;
				}
			}
		}
		else	/* 放すと後方に移動 */
		{
			if ( (OPTION_ID_03_RM/*1*/&0xff)==(data->opt_anime_add_id&0xff)/*pos*/) /* 1: 右側系のオプション */
			{
				if (data->angleCCW512 > 64+16 +8+32-64+((data->opt_anime_add_id&0x100)>>2) )/*最小(右)位置*/
				{
					data->angleCCW512 -= vv512;
				}
			}
			else	/* 0: 左側系のオプション */
			{
				if (data->angleCCW512 < 448-16 -8-32+64-((data->opt_anime_add_id&0x100)>>2) )/*最大(左)位置*/
				{
					data->angleCCW512 += vv512;
				}
			}
		}
	}
	#if (0000)
	if (0==(pd->state_flag&STATE_FLAG_08_OPTION_HIDE))	/* オプションが消えてれば */
	{
	//	s->type = SP_DELETE;/*おしまい*/
	//	s->type = SP_DELETE;/*おしまい*/
		s->flags			|= (SP_FLAG_VISIBLE);/*見せない*/
	}
	#endif/*0000*/
	#if 1
	mask512(data->angleCCW512);/*ねんのため*/
	#endif
//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/-((s->w)>>1)+((player->w-12-1)*data->opt_anime_add_id)+6;
//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+((/*player->w*/24-12-1)*data->opt_anime_add_id)+6-8;
//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+((/*player->w*/24+11+11)*data->opt_anime_add_id)-11-8;
// -1 : 		 -11 -8
//	1 : +24+11+11-11 -8
//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+((/*player->w*/24-6-6)*data->opt_anime_add_id)+6-8;
// -1 : 		 +6 -8
//	1 : +24 -6 -6 +6 -8
//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+((/*player->w*/(24)-6-6)*data->opt_anime_add_id)+6-((s->w)>>1)/*(8)*/;
//
	//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+(((int)(data->pos+1)*player->w-s->w)/2)-data->pos*6;
	//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+(((int)(data->opt_anime_add_id+1)*/*player->w*/(24)-s->w)/2)-data->opt_anime_add_id*6; 		//2598187
	//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+ (((24/*player->w*/ /2)-6)*(data->opt_anime_add_id)) -((s->w)>>1) +(24/*player->w*/ /2);	//2598219

		/* -1: 6*256  1: 18*256 */
	//	s->x256 = player->x256+cos512((data->angle512))*(16)/*data->length*/+ (((6)*(2+data->opt_anime_add_id))<<8) -((s->w128)); //2598187 2079061

	{int hankei;
		hankei = (YUYUKO==select_player)?(32):(16);
#if 0
/* CWの場合 */
		s->x256 = player->x256+cos512((data->angle512))*(hankei)/*data->length*/+ (18*256)-(((data->opt_anime_add_id&2)<<8)*6) -((s->w128)); // 2079061 (4オプション対応)
		s->y256 = player->y256+sin512((data->angle512))*(hankei)/*data->length*/+((player->h128-s->h128))-t256(5);
#else
/* CCWの場合 */
		s->x256 = player->x256+sin512((data->angleCCW512))*(hankei)/*data->length*/+ (18*256)-(((data->opt_anime_add_id&2)<<8)*6) -((s->w128)); // 2079061 (4オプション対応)
		s->y256 = player->y256+cos512((data->angleCCW512))*(hankei)/*data->length*/+((player->h128-s->h128))-t256(5);
#endif
	}
}

/*---------------------------------------------------------
	プレイヤー、オプションの移動(霊夢、魔理沙、仮幽々子)
---------------------------------------------------------*/

static void re_gggg(SPRITE *s, int pd_weapon)	/* 霊夢 */
{
	PL_OPTION_DATA *data=(PL_OPTION_DATA *)s->data;

//	player_add_needle(s, t256( 0), t256( 0), NEEDLE_ANGLE_270);
	/* 霊夢の場合前方集中させると強い。 */
	mask512(data->angleCCW512);
	{signed int diff256;
		diff256 = (data->angleCCW512);
	//	diff256 = 512-diff256;
		diff256 -= 256;
		#if (0)/*[debug]*/
		((PLAYER_DATA *)player->data)->my_score = data->angleCCW512;
		#endif /*(0000)*/
	//	player_add_needle(s,  ((diff256)<<6)+(((data->opt_anime_add_id)&0x0c00))-((0x0c00)>>1), t256( 0), NEEDLE_ANGLE_270);
	//	player_add_needle(s,  ((diff256)<<6)+(((data->opt_anime_add_id)&0x1e00))-((0x0800)), t256( 0), NEEDLE_ANGLE_270);
	//	player_add_needle(s, -((diff256)<<6)+(((data->opt_anime_add_id)&0x1e00))-((0x0800)), t256( 0), NEEDLE_ANGLE_270);
	//	player_add_needle(s, -((diff256)<<6)+(((data->opt_anime_add_id)&0x1e00))-((0x1000)), t256( 0), NEEDLE_ANGLE_270);
		player_add_needle(s, -((diff256)<<4)+(((data->opt_anime_add_id)&0x1e00))-((0x1000)), t256( 0), NEEDLE_ANGLE_270);
	}
}
/*
離す: [51]	[461=256+128+64+16-3]
押す: [  ]	[314=256	+64   -6]

*/

static void ma_gggg(SPRITE *s, int pd_weapon)	/* 魔理沙 */
{
	player_add_needle(s, t256( 0), t256( 0), NEEDLE_ANGLE_270);
}

static void yu_gggg(SPRITE *s, int pd_weapon)	/* 幽々子 */
{
	player_add_needle(s, t256(	0), t256(-40), NEEDLE_ANGLE_271);/*-48*/
}

static void re_ma_move_option(SPRITE *s)	//霊夢	魔理沙	仮幽々子 */
{
	/* オプションが非表示の場合、何もしない。 */
	if (0==(s->flags & ( SP_FLAG_VISIBLE))) 	{	return; }
//
	PL_OPTION_DATA *data=(PL_OPTION_DATA *)s->data;
	// アニメーション
	data->anime_wait--;
	if (data->anime_wait<1)
	{
		data->anime_wait=/*data->anime_speed;*/player_fix_status[BASE_OPT_SHOT_ANIME+select_player];/*12*/ /*2*/ /*陰陽だまはゆっくり回るよ*/
	//	s->anim_frame=((s->anim_frame+(data->opt_anime_add_id)+(data->opt_anime_add_id)-1/*data->pos*/)&(8-1)/*%8*/);
		s->anim_frame=((s->anim_frame+(data->opt_anime_add_id))&(8-1)/*%8*/);
	//	if (s->anim_frame<0)		{	s->anim_frame=7;}
	}
	//
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	if (pd->state_flag & STATE_FLAG_15_KEY_SHOT)
	{
		{
			{
				data->opt_shot_interval--;
				if (data->opt_shot_interval<0)
				{
					data->opt_shot_interval=player_fix_status[BASE_OPT_SHOT_INTERVAL+select_player];
					#if 0//2083393
					/*const*/static void (*ggg[PLAYERS5])(SPRITE *sss, int pd_weapon) =
					{
						/*REIMU*/		re_gggg,	// 霊夢
						/*MARISA*/		ma_gggg,	// 魔理沙
						/*REMILIA*/ 	re_gggg,	// 現在ダミー
						/*CIRNO*/		re_gggg,	// 現在ダミー
						/*YUYUKO*/		yu_gggg,	// 幽々子
					};
					(ggg[select_player])(s, pd->weapon_power);
					#else//2082433
					switch (select_player)
					{
					case REIMU: 	re_gggg(s, pd->weapon_power); break;	// 霊夢
					case MARISA:	ma_gggg(s, pd->weapon_power); break;	// 魔理沙
					case YUYUKO:	yu_gggg(s, pd->weapon_power); break;	// 幽々子
					}
					#endif
				}
			}
		}
	}
	reimu_marisa_control_option(s, data, pd);
}

/*---------------------------------------------------------
	プレイヤー、オプションの定義
---------------------------------------------------------*/

/* [***090128 [***090220	追加:レミリア用オプション.c3,c4 */

enum
{
	OPTION_C1 = 0,
	OPTION_C2,
	OPTION_C3,
	OPTION_C4,
};
static SPRITE *option[4];

/*---------------------------------------------------------
	プレイヤー、オプションの移動(レミリア)
---------------------------------------------------------*/

enum
{
	FORMATION_00 = 0,
	FORMATION_01,
	FORMATION_02,
	FORMATION_03,
};
/*
	目標:state1
	FORMATION_00: レミリアの直後に四つ
	FORMATION_01: レミリアの周りを回るよ
	FORMATION_02: 画面後方から支援するよ (y=250の位置に散らばる)
	FORMATION_03: 減速時用(好きな位置に置けるよ) (レミリアに重なるように。slowを押すと段階的にoption停止)
*/
static void oz_move_option(SPRITE *s)	/* レミリア 	[***090220 追加 */
{
	REMILIA_OPTION_DATA *data=(REMILIA_OPTION_DATA *)s->data;
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
//
	#if 0
	switch (data->opt_anime_add_id)
	{
	case OPTION_C1:
		break;
	case OPTION_C2:
		if (  (pd->weapon_power<=55-1)) 			{	s->flags &= (~(SP_FLAG_VISIBLE));	}
		else /*if (  (pd->weapon_power>55-1))*/ 	{	s->flags |=   (SP_FLAG_VISIBLE);	}
		break;
	case OPTION_C3:
		if (  (pd->weapon_power<=80-1)) 			{	s->flags &= (~(SP_FLAG_VISIBLE));	}
		else /*if (  (pd->weapon_power>80-1))*/ 	{	s->flags |=   (SP_FLAG_VISIBLE);	}
		break;
	case OPTION_C4:
		if (  (pd->weapon_power<=108-1))			{	s->flags &= (~(SP_FLAG_VISIBLE));	}
		else /*if (  (pd->weapon_power>108-1))*/	{	s->flags |=   (SP_FLAG_VISIBLE);	}
		break;
	}
	#endif
//
	/* オプションが非表示の場合、何もしない。 */
	if (0==(s->flags & ( SP_FLAG_VISIBLE))) 	{	return; }
//
	// !--------------------
//
	static int state1=0;
	if (s->flags&SP_FLAG_VISIBLE)	/* 表示されていれば上記の処理をするよ。 */
	{
		/* アニメーション */
		data->anime_wait--;
		if (data->anime_wait<1)
		{
			data->anime_wait=/*data->anime_speed;*/player_fix_status[BASE_OPT_SHOT_ANIME+select_player];/*12*/ /*2*/ /*陰陽だまはゆっくり回るよ*/
			s->anim_frame++;
			s->anim_frame &= (8-1);//if (s->anim_frame==8)	{s->anim_frame=0;}
		}
		/* 攻撃支援 */
		if (pd->state_flag & STATE_FLAG_15_KEY_SHOT)
		{
			{
				{
					data->opt_shot_interval--;
					if (data->opt_shot_interval<0)
					{
						switch (data->opt_anime_add_id)
						{
						case OPTION_C1:
						case OPTION_C2:
							if (pd->weapon_power > 100-1)
							{
								player_add_needle(s, t256( 0), t256( 0), NEEDLE_ANGLE_277 );
								player_add_needle(s, t256( 0), t256( 0), NEEDLE_ANGLE_263 );
								data->opt_shot_interval=7+3;
							}
							else
							{
								player_add_needle(s, t256( 0), t256( 0), NEEDLE_ANGLE_270 );
								data->opt_shot_interval=5+3;
							}
							break;
						case OPTION_C3:
						case OPTION_C4:
							player_add_needle(s, t256( 0), t256( 0), NEEDLE_ANGLE_270 );
							data->opt_shot_interval=5+3;
							break;
						}
					}
				}
			}
		}
		//
		int slow_flag;
		slow_flag=0;
		// 低速移動 */
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
		player_offs_x256 = player->x256+((player->w128-s->w128));
		player_offs_y256 = player->y256+((player->h128-s->h128));
	//	if (1==slow_flag)	/* pspは0レジスタがあるので0と比較したほうが速い */
		if (0!=slow_flag)	// 少なくとも低速移動でオプションが止められるのはレミのみ(チルノには合わない)
		{
			/*レミ用*/
			data->slow_count += (1<<2);
			if ((/*data->*/state1==FORMATION_03) && /* FORMATION_03: 減速時用(好きな位置に置けるよ) */
				(data->slow_count < ((data->opt_anime_add_id+1)<<(6+2))/* *30 64*/))
			{
				s->x256 = player_offs_x256;
				s->y256 = player_offs_y256;
			}
			else if (/*data->*/state1==FORMATION_02)	/* FORMATION_02: 画面後方から支援するよ */
			{
				switch (data->opt_anime_add_id)
				{
				case OPTION_C1: if (s->x256 < t256(170)-((s->w128+s->w128))) {s->x256 += t256(2)/**fps_fa_ctor*/;}	break;
				case OPTION_C2: if (s->x256 > t256(210))					{s->x256 -= t256(2)/**fps_fa_ctor*/;}	break;
				case OPTION_C3: if (s->x256 < t256(100)-((s->w128+s->w128))) {s->x256 += t256(4)/**fps_fa_ctor*/;}	break;
				case OPTION_C4: if (s->x256 > t256(280))					{s->x256 -= t256(4)/**fps_fa_ctor*/;}	break;
				}
			}
		}
		else
		{
			/*レミ用*/
			if (data->slow_count)
			{
				data->slow_count=0;
				data->state2=0;
			}
		//
			// フォーメーション変更
			/* OPTION_C1 の場合のみキー入力受け付け */
			if (OPTION_C1==data->opt_anime_add_id) //opt_anime_add_id==1によるflag管理
			{
			//	if (my_pad & PSP_KEY_OPTION)
			/* (オプションキーキーを離した瞬間なら) */
				if ((0==(my_pad & PSP_KEY_OPTION))) 	/* オプションキーが離されている */
				{
					if ( (my_pad^my_pad_alter) & PSP_KEY_OPTION)	/* オプションキーキーの状態が変わった(トグル、押したまたは離した場合) */
					{
						/*data->*/state1++;
						if (/*data->*/state1==(FORMATION_03+1) )
						{	/*data->*/state1=FORMATION_00;}
					//
						data->state2=0; 								/* OPTION_C1 の state2 */
						((REMILIA_OPTION_DATA *)option[OPTION_C2]->data)->state2=0; /* OPTION_C2 の state2 */
						((REMILIA_OPTION_DATA *)option[OPTION_C3]->data)->state2=0; /* OPTION_C3 の state2 */
						((REMILIA_OPTION_DATA *)option[OPTION_C4]->data)->state2=0; /* OPTION_C4 の state2 */
					}
				}
			}
			switch (data->state2)
			{
			case 0: 	//初期化
				{
					enum
					{
						FORMATION_00_LOCATE_X=0,
						FORMATION_00_LOCATE_Y,/* 1*/
						FORMATION_01_RADIUS_R,/* 2*/
						FORMATION_01_ADD_R,   /* 3*/
						FORMATION_02_LOCATE_X,/* 4*/
						FORMATION_02_LOCATE_Y,/* 5*/
						FORMATION_MAX		  /* 6*/		/* 最大数 */
					};
					const signed short fff[FORMATION_MAX][4] =
					{
						{	t256(15),  t256(-15),	t256(25),  t256(-25) }, 	/* FORMATION_00: レミリアの直後に四つ */
						{	t256(15),  t256( 15),	t256(20),  t256( 20) }, 	/* FORMATION_00: レミリアの直後に四つ */
						{	(20),  ( 30),	(40),  ( 50) }, 					/* FORMATION_01: 回転半径 */
						{	( 1),  (  2),	(-3),  ( -6) }, 					/* FORMATION_01: 回転方向、角度加算値 */
//					//	{	( 1*115+10),  ( 2*115+10),	( 0*115+10),  ( 3*115+10) },	/* FORMATION_02: 画面後方から支援するよ(wideth380dot) */
						{	( 1*104+20),  ( 2*104+20),	( 0*104+20),  ( 3*104+20) },	/* FORMATION_02: 画面後方から支援するよ(wideth352dot) */
						{  (240),  (240),  (220),  (220) }, 							/* FORMATION_02: 画面後方から支援するよ */
					};
					switch (/*data->*/state1)
					{
					case FORMATION_00:	/* FORMATION_00: レミリアの直後に四つ */
						data->offset_x256=((fff[FORMATION_00_LOCATE_X][(data->opt_anime_add_id)]));
						data->offset_y256=((fff[FORMATION_00_LOCATE_Y][(data->opt_anime_add_id)]));
						break;
					case FORMATION_01:	/* FORMATION_01: レミの周りを回るよ */
						data->offset_x256 = fff[FORMATION_01_RADIUS_R][(data->opt_anime_add_id)];/*40*/ /* 円の半径と兼用 */
						data->offset_y256 = fff[FORMATION_01_ADD_R ][(data->opt_anime_add_id)]; 		/* 円の回転方向、角度加算値と兼用 */
/// 					data->f_angle512=0/*l_angle512*/;
						break;
					case FORMATION_02:	/* FORMATION_02: 画面後方から支援するよ */
						data->offset_x256=((fff[FORMATION_02_LOCATE_X][(data->opt_anime_add_id)])<<8);
						data->offset_y256=((fff[FORMATION_02_LOCATE_Y][(data->opt_anime_add_id)])<<8);/*240*/
						break;
					case FORMATION_03:	/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
						break;
					}
				}
				data->state2=1; 	/* 次へ */
				break;
			case 1: 	// 目標地点へ移動中 */
				#define oz_offset_r 	(data->offset_x256) 	/* FORMATION_01: 円の半径と兼用 */
				#define oz_offset_add_r (data->offset_y256) 	/* FORMATION_01: 円の回転方向、角度加算値と兼用 */
				{
				int delta_y256=0;				/* delta_y : y座標における目標地点と現在地の差 */
				int delta_x256=0;				/* delta_x : x座標における目標地点と現在地の差 */
					switch (/*data->*/state1)
					{
					case FORMATION_00:	/* FORMATION_00: レミリアの直後に四つ */
						delta_y256 = player_offs_y256-s->y256 + data->offset_y256;
						delta_x256 = player_offs_x256-s->x256 + data->offset_x256;
						break;
					case FORMATION_01:	/* FORMATION_01: レミの周りを回るよ */						//static int l_angle512;
#if 0
						/*レミ用*/
						if (REMILIA==select_player)
						{
							/*l_angle512*/data->/*f_*/angle512 += oz_offset_add_r/*data->offset_y*/ /*5==ra d2deg512(0.05)*/ /**fps_fa_ctor*/;
							mask512(/*l_angle512*/data->/*f_*/angle512);	//if (l_angle>M_PI) 	{	l_angle-=2*M_PI;	}
						}
#endif
#if 0
/* CWの場合 */
						delta_x256 = player_offs_x256-s->x256 + (cos512((data->REMILIA_angle512/*+l_angle512*/))*oz_offset_r);
						delta_y256 = player_offs_y256-s->y256 + (sin512((data->REMILIA_angle512/*+l_angle512*/))*oz_offset_r);
#else
/* CCWの場合 */
						delta_x256 = player_offs_x256-s->x256 + (sin512((data->REMILIA_angle512/*+l_angle512*/))*oz_offset_r);
						delta_y256 = player_offs_y256-s->y256 + (cos512((data->REMILIA_angle512/*+l_angle512*/))*oz_offset_r);
#endif
						break;
					case FORMATION_02:	/* FORMATION_02: 画面後方から支援するよ */
						delta_y256 = (-((s->h128))-s->y256+data->offset_y256);/*240*/
						delta_x256 = (-((s->w128))-s->x256+data->offset_x256);
						break;
					case FORMATION_03:	/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
						delta_y256 = player_offs_y256-s->y256;
						delta_x256 = player_offs_x256-s->x256;
						break;
					}
					{	/* 目標地点に移動する */
						int ccc_angle512;
						/*data->*/ccc_angle512=atan_512(((delta_y256)>>8), ((delta_x256)>>8));
#if 0
/* CWの場合 */
						s->x256 += ((cos512((/*data->*/ccc_angle512))*/*10*/((REMILIA==select_player)?t256(8.0):t256(1.0)))>>8)/**fps_fa_ctor*/;
						s->y256 += ((sin512((/*data->*/ccc_angle512))*/*10*/((REMILIA==select_player)?t256(8.0):t256(1.0)))>>8)/**fps_fa_ctor*/;
#else
/* CCWの場合 */
						s->x256 += ((sin512((/*data->*/ccc_angle512))*/*10*/((REMILIA==select_player)?t256(8.0):t256(1.0)))>>8)/**fps_fa_ctor*/;
						s->y256 += ((cos512((/*data->*/ccc_angle512))*/*10*/((REMILIA==select_player)?t256(8.0):t256(1.0)))>>8)/**fps_fa_ctor*/;
#endif
					}
#if 1
					/*レミ用*/
					if (REMILIA==select_player)
					{
						/* 目標付近までたどり着いたら */
						if (( t256(-5) < delta_y256) &&
							(delta_y256 < t256(5)) )
						{
							data->state2=2; 	/* 次へ */
						}
					}
#endif
				}
				break;
#if 1
			case 2: 	/* 位置の維持 & 配置完了flag */
				switch (/*data->*/state1)
				{
				case FORMATION_00:	/* FORMATION_00: レミリアの直後に四つ */
					s->x256 = player_offs_x256 + data->offset_x256;
					s->y256 = player_offs_y256 + data->offset_y256;
					break;
				case FORMATION_01:	/* FORMATION_01: レミの周りを回るよ */
					/*l_angle512*/data->/*f_*/REMILIA_angle512 += oz_offset_add_r/*data->offset_y*/ /*5==ra d2deg512(0.05)*/ /**fps_fa_ctor*/;
					mask512(/*l_angle512*/data->/*f_*/REMILIA_angle512);	//if (l_angle>M_PI) 	{	l_angle-=2*M_PI;	}
#if 0
/* CWの場合 */
					s->x256 = player_offs_x256 + cos512((data->REMILIA_angle512/*+data->f_angle512*/ /*l_angle512*/))*oz_offset_r;
					s->y256 = player_offs_y256 + sin512((data->REMILIA_angle512/*+data->f_angle512*/ /*l_angle512*/))*oz_offset_r;
#else
/* CCWの場合 */
					s->x256 = player_offs_x256 + sin512((data->REMILIA_angle512/*+data->f_angle512*/ /*l_angle512*/))*oz_offset_r;
					s->y256 = player_offs_y256 + cos512((data->REMILIA_angle512/*+data->f_angle512*/ /*l_angle512*/))*oz_offset_r;
#endif
					break;
				case FORMATION_02:	/* FORMATION_02: 画面後方から支援するよ */
					s->x256 = data->offset_x256;
					s->y256 = data->offset_y256;
					break;
				case FORMATION_03:	/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
					s->x256 = player_offs_x256;
					s->y256 = player_offs_y256;
					break;
				}
#endif
				break;
			}
		}
	}
//
	#if (0000)
	if (0==(pd->state_flag&STATE_FLAG_08_OPTION_HIDE))	/* オプションが消えてれば */
	{
	//	s->type = SP_DELETE;/*おしまい*/
	//	s->type = SP_DELETE;/*おしまい*/
		s->flags			|= (SP_FLAG_VISIBLE);/*見せない*/
	}
	#endif/*0000*/
}

/*---------------------------------------------------------
	プレイヤー、オプションの追加
	霊夢	魔理沙	幽々子
---------------------------------------------------------*/

static void option_create_re_ma_yu(SPRITE *s)
{
	PL_OPTION_DATA *data;		/* 霊夢のを流用してます。 */
//
	option[OPTION_C1]					= sprite_add_res( BASE_OPTION1_PNG+select_player ); 	/*+ REIMU MARISA YUYUKO */
//	option[OPTION_C1]->flags			|= (SP_FLAG_VISIBLE);
	option[OPTION_C1]->flags			&= (~(SP_FLAG_VISIBLE));		/* 可視フラグのOFF(不可視) */
	option[OPTION_C1]->callback_mover	= re_ma_move_option;
	data								= mmalloc(sizeof(PL_OPTION_DATA));
	option[OPTION_C1]->data 			= data;
	data->strength						= 5;/* [***090214 追加 */
//	data->length						= 11;
	option[OPTION_C1]->x256 			= s->x256+((s->w128-option[OPTION_C1]->w128))-t256(11)/*data->length*/;
	option[OPTION_C1]->y256 			= s->y256+((s->h128-option[OPTION_C1]->h128));
	data->anime_wait					= 0;
//	data->anime_speed					= player_fix_status[BASE_OPT_SHOT_ANIME+select_player];/*12*/ /*2*/ /*陰陽だまはゆっくり回るよ*/
	data->opt_anime_add_id/*pos*/		= OPTION_ID_02_LM/*-1*/;/*0*/
//	data->angleCCW512					= deg_360_to_512((270))/*deg_360_to_512(180)*/;/*-ra d2deg512(M_PI)*/
	data->angleCCW512					= deg_360_to_512CCW((270))/*deg_360_to_512(180)*/;/*-ra d2deg512(M_PI)*/
//	data->state 						= 0;
	data->opt_shot_interval 			= 0;
	option[OPTION_C1]->anim_frame		= 0;
	option[OPTION_C1]->type 			= SP_MUTEKI;
//
	option[OPTION_C2]					= sprite_add_res( BASE_OPTION1_PNG+select_player/*+ REIMU MARISA YUYUKO */ );
//	option[OPTION_C2]->flags			|= (SP_FLAG_VISIBLE);
	option[OPTION_C2]->flags			&= (~(SP_FLAG_VISIBLE));		/* 可視フラグのOFF(不可視) */
	option[OPTION_C2]->callback_mover	= re_ma_move_option;
	data								= mmalloc(sizeof(PL_OPTION_DATA));
	option[OPTION_C2]->data 			= data;
	data->strength						= 5;/* [***090214 追加 */
//	data->length						= 11;
	option[OPTION_C2]->x256 			= s->x256+((s->w128-option[OPTION_C2]->w128))-t256(11)/*data->length*/;
	option[OPTION_C2]->y256 			= s->y256+((s->h128-option[OPTION_C2]->h128));
	data->anime_wait					= 0;
//	data->anime_speed					= player_fix_status[BASE_OPT_SHOT_ANIME+select_player];/*12*/ /*2*/;/*陰陽だまはゆっくり回るよ*/
	data->opt_anime_add_id/*pos*/		= OPTION_ID_03_RM/*1*/;
//	data->angleCCW512					= deg_360_to_512((90))/*deg_360_to_512(0)*/;
	data->angleCCW512					= deg_360_to_512CCW((90))/*deg_360_to_512(0)*/;
//	data->state 						= 0;
	data->opt_shot_interval 			= 0;
	option[OPTION_C2]->anim_frame		= 0;
	option[OPTION_C2]->type 			= SP_MUTEKI;
//
//
	option[OPTION_C3]					= sprite_add_res( BASE_OPTION1_PNG+select_player/*+ REIMU MARISA YUYUKO */ );
//	option[OPTION_C3]->flags			|= (SP_FLAG_VISIBLE);
	option[OPTION_C3]->flags			&= (~(SP_FLAG_VISIBLE));		/* 可視フラグのOFF(不可視) */
	option[OPTION_C3]->callback_mover	= re_ma_move_option;
	data								= mmalloc(sizeof(PL_OPTION_DATA));
	option[OPTION_C3]->data 			= data;
	data->strength						= 5;/* [***090214 追加 */
//	data->length						= 11;
	option[OPTION_C3]->x256 			= s->x256+((s->w128-option[OPTION_C3]->w128))-t256(11)/*data->length*/;
	option[OPTION_C3]->y256 			= s->y256+((s->h128-option[OPTION_C3]->h128));
	data->anime_wait					= 0;
//	data->anime_speed					= player_fix_status[BASE_OPT_SHOT_ANIME+select_player];/*12*/ /*2*/ /*陰陽だまはゆっくり回るよ*/
	data->opt_anime_add_id/*pos*/		= OPTION_ID_01_LL/*-1*/;/*0*/
//	data->angleCCW512					= deg_360_to_512((270))/*deg_360_to_512(180)*/;/*-ra d2deg512(M_PI)*/
	data->angleCCW512					= deg_360_to_512CCW((270))/*deg_360_to_512(180)*/;/*-ra d2deg512(M_PI)*/
//	data->state 						= 0;
	data->opt_shot_interval 			= 0;
	option[OPTION_C3]->anim_frame		= 0;
	option[OPTION_C3]->type 			= SP_MUTEKI;
//
	option[OPTION_C4]					= sprite_add_res( BASE_OPTION1_PNG+select_player/*+ REIMU MARISA YUYUKO */ );
//	option[OPTION_C4]->flags			|= (SP_FLAG_VISIBLE);
	option[OPTION_C4]->flags			&= (~(SP_FLAG_VISIBLE));		/* 可視フラグのOFF(不可視) */
	option[OPTION_C4]->callback_mover	= re_ma_move_option;
	data								= mmalloc(sizeof(PL_OPTION_DATA));
	option[OPTION_C4]->data 			= data;
	data->strength						= 5;/* [***090214 追加 */
//	data->length						= 11;
	option[OPTION_C4]->x256 			= s->x256+((s->w128-option[OPTION_C4]->w128))-t256(11)/*data->length*/;
	option[OPTION_C4]->y256 			= s->y256+((s->h128-option[OPTION_C4]->h128));
	data->anime_wait					= 0;
//	data->anime_speed					= player_fix_status[BASE_OPT_SHOT_ANIME+select_player];/*12*/ /*2*/;/*陰陽だまはゆっくり回るよ*/
	data->opt_anime_add_id/*pos*/		= OPTION_ID_04_RR/*1*/;
//	data->angleCCW512					= deg_360_to_512((90))/*deg_360_to_512(0)*/;
	data->angleCCW512					= deg_360_to_512CCW((90))/*deg_360_to_512(0)*/;
//	data->state 						= 0;
	data->opt_shot_interval 			= 0;
	option[OPTION_C4]->anim_frame		= 0;
	option[OPTION_C4]->type 			= SP_MUTEKI;
//
}

static void option_create_oz_ci(SPRITE *s)	//レミ	/* [***090220 追加 */
{
	REMILIA_OPTION_DATA *data;
	option[OPTION_C1]					= sprite_add_res( BASE_OPTION1_PNG+select_player/*REMILIA*/ );
//	option[OPTION_C1]->flags			|= (SP_FLAG_VISIBLE);
	option[OPTION_C1]->flags			&= (~(SP_FLAG_VISIBLE));		/* 可視フラグのOFF(不可視) */
	option[OPTION_C1]->type 			= SP_MUTEKI;
	option[OPTION_C1]->callback_mover	= oz_move_option;
	data								= mmalloc(sizeof(REMILIA_OPTION_DATA));
	option[OPTION_C1]->data 			= data;
	option[OPTION_C1]->anim_frame		= 0;
//	data->REMILIA_angle512				= deg_360_to_512((45*5))/*deg_360_to_512(45*1)*/ /*M_PI*2*1/8*/;/*チルノ*/
	data->REMILIA_angle512				= deg_360_to_512CCW(360-(45*5))/*deg_360_to_512(45*1)*/ /*M_PI*2*1/8*/;/*チルノ*/
//	data->next							= player;
//	data->state1						= 0;///
	data->state2						= 0;
	data->offset_x256					= t256(0);
	data->offset_y256					= t256(0);
	data->strength						= 5;
	data->anime_wait					= 0;
//	data->anime_speed					= 3;
	data->opt_shot_interval 			= 0;
	data->opt_anime_add_id				= OPTION_C1;
//	data->ccc_angle512					= deg_360_to_512((0));
	data->slow_count					= 0;
	option[OPTION_C1]->x256 			= s->x256+((s->w128-option[OPTION_C1]->w128));
	option[OPTION_C1]->y256 			= s->y256+((s->h128-option[OPTION_C1]->h128))+t256(20);
//
	option[OPTION_C2]					= sprite_add_res( BASE_OPTION2_PNG+select_player/*REMILIA*/ );
//	option[OPTION_C2]->flags			&= (~(SP_FLAG_VISIBLE));
	option[OPTION_C2]->flags			&= (~(SP_FLAG_VISIBLE));		/* 可視フラグのOFF(不可視) */
	option[OPTION_C2]->type 			= SP_MUTEKI;
	option[OPTION_C2]->callback_mover	= oz_move_option;
	data								= mmalloc(sizeof(REMILIA_OPTION_DATA));
	option[OPTION_C2]->data 			= data;
	option[OPTION_C2]->anim_frame		= 0;
//	data->REMILIA_angle512				= deg_360_to_512((45*7))/*deg_360_to_512(45*5)*/ /*M_PI*2*5/8*/;/*チルノ*/
	data->REMILIA_angle512				= deg_360_to_512CCW(360-(45*7))/*deg_360_to_512(45*5)*/ /*M_PI*2*5/8*/;/*チルノ*/
//	data->next							= c1;
//	data->state1						= ((REMILIA_OPTION_DATA *)c1->data)->state1;///
	data->state2						= 0;
	data->offset_x256					= t256(0);
	data->offset_y256					= t256(0);
	data->strength						= 5;
	data->anime_wait					= 0;
//	data->anime_speed					= 3;
	data->opt_shot_interval 			= 0;
	data->opt_anime_add_id				= OPTION_C2;
//	data->ccc_angle512					= deg_360_to_512((0));
	data->slow_count					= 0;
	option[OPTION_C2]->x256 			= s->x256+((s->w128-option[OPTION_C2]->w128));
	option[OPTION_C2]->y256 			= s->y256+((s->h128-option[OPTION_C2]->h128))+t256(20);
//
	option[OPTION_C3]					= sprite_add_res( BASE_OPTION3_PNG+select_player/*REMILIA*/ );
//	option[OPTION_C3]->flags			&= (~(SP_FLAG_VISIBLE));
	option[OPTION_C3]->flags			&= (~(SP_FLAG_VISIBLE));		/* 可視フラグのOFF(不可視) */
	option[OPTION_C3]->type 			= SP_MUTEKI;
	option[OPTION_C3]->callback_mover	= oz_move_option;
	data								= mmalloc(sizeof(REMILIA_OPTION_DATA));
	option[OPTION_C3]->data 			= data;
	option[OPTION_C3]->anim_frame		= 0;
//	data->REMILIA_angle512				= deg_360_to_512((45*1))/*deg_360_to_512(45*3)*/ /*M_PI*2*3/8*/;/*チルノ*/
	data->REMILIA_angle512				= deg_360_to_512CCW(360-(45*1))/*deg_360_to_512(45*3)*/ /*M_PI*2*3/8*/;/*チルノ*/
//	data->next							= c2;
//	data->state1						= ((REMILIA_OPTION_DATA *)c1->data)->state1;///
	data->state2						= 0;
	data->offset_x256					= t256(0);
	data->offset_y256					= t256(0);
	data->strength						= 5;
	data->anime_wait					= 0;
//	data->anime_speed					= 3;
	data->opt_shot_interval 			= 0;
	data->opt_anime_add_id				= OPTION_C3;
//	data->ccc_angle512					= deg_360_to_512((0));
	data->slow_count					= 0;
	option[OPTION_C3]->x256 			= s->x256+((s->w128-option[OPTION_C3]->w128));
	option[OPTION_C3]->y256 			= s->y256+((s->h128-option[OPTION_C3]->h128))+t256(20);
//
	option[OPTION_C4]					= sprite_add_res( BASE_OPTION4_PNG+select_player/*REMILIA*/ );
//	option[OPTION_C4]->flags			&= (~(SP_FLAG_VISIBLE));
	option[OPTION_C4]->flags			&= (~(SP_FLAG_VISIBLE));		/* 可視フラグのOFF(不可視) */
	option[OPTION_C4]->type 			= SP_MUTEKI;
	option[OPTION_C4]->callback_mover	= oz_move_option;
	data								= mmalloc(sizeof(REMILIA_OPTION_DATA));
	option[OPTION_C4]->data 			= data;
	option[OPTION_C4]->anim_frame		= 0;
//	data->REMILIA_angle512				= deg_360_to_512((45*3))/*deg_360_to_512(45*7)*/ /*M_PI*2*7/8*/;/*チルノ*/
	data->REMILIA_angle512				= deg_360_to_512CCW(360-(45*3))/*deg_360_to_512(45*7)*/ /*M_PI*2*7/8*/;/*チルノ*/
//	data->next							= c3;
//	data->state1						= ((REMILIA_OPTION_DATA *)c1->data)->state1;///
	data->state2						= 0;
	data->offset_x256					= t256(0);
	data->offset_y256					= t256(0);
	data->strength						= 5;
	data->anime_wait					= 0;
//	data->anime_speed					= 3;
	data->opt_shot_interval 			= 0;
	data->opt_anime_add_id				= OPTION_C4;
//	data->ccc_angle512					= deg_360_to_512((0));
	data->slow_count					= 0;
	option[OPTION_C4]->x256 			= s->x256+((s->w128-option[OPTION_C4]->w128));
	option[OPTION_C4]->y256 			= s->y256+((s->h128-option[OPTION_C4]->h128))+t256(20);
}

/*---new add-----*/


/*---------------------------------------------------------
	プレイヤー、生死判定用コアの追加
	○の追加
---------------------------------------------------------*/

static SPRITE *player_add_core(SPRITE *s1)
{
	SPRITE *s2;
	s2						= NULL;
	s2						= sprite_add_res( BASE_CORE_PNG+select_player );
	s2->anim_speed			= 0;
	s2->type				= SP_PLAYER_CORE;
	s2->x256				= (s1->x256)+((s1->w128-s2->w128));
	s2->y256				= (s1->y256)-((s2->h128+s2->h128))+t256(20);
	s2->callback_mover		= player_move_core;
	s2->flags				|= (SP_FLAG_VISIBLE);
	return s2;
}

/*---------------------------------------------------------
	プレイヤー弾(ショットのみ)生成、子関数
---------------------------------------------------------*/

enum
{
/* 誘導ショット */
	REIMU_SHOT_TYPE_91 = 0,
	REIMU_SHOT_TYPE_92,
	REIMU_SHOT_TYPE_93,
	REIMU_SHOT_TYPE_94,
	REIMU_SHOT_TYPE_95,
	REIMU_SHOT_TYPE_96,
/* 非誘導ショット */
	REIMU_SHOT_TYPE_10,
	REIMU_SHOT_TYPE_11,
	REIMU_SHOT_TYPE_12,
//
	REIMU_SHOT_TYPE_13,
	REIMU_SHOT_TYPE_14,
//
	MARISA_SHOT_TYPE_03,
	MARISA_SHOT_TYPE_04,
	//MARISA_SHOT_TYPE_05,
	//MARISA_SHOT_TYPE_06,
	MARISA_SHOT_TYPE_07,
	MARISA_SHOT_TYPE_08,
//
	YUYUKO_SHOT_TYPE_01,
	YUYUKO_SHOT_TYPE_02,
	YUYUKO_SHOT_TYPE_03,
	YUYUKO_SHOT_TYPE_04,
//
	REIMU_SHOT_TYPE_MAX 	/* 最大数 */
};

enum
{
	REI00_x_offset=0,
	REI01_y_offset,
	REI02_vx256,
	REI03_gra_type, 	/* 霊夢の回転ショット */
	REI04_MAX			/* 最大数 */
};

static void player_add_shot(SPRITE *player, int shot_type) //霊夢 ＆ 魔理沙 ＆ チルノ ＆ レミ */	/* [***090220 追加 */
{
	const Sint16 r_tbl[REIMU_SHOT_TYPE_MAX][REI04_MAX] =
	{	/*x_offset256	y_offset256  vx256 gra_type */
/* 誘導ショット(霊夢) */
//
/*03*/	{t256(-15), t256(+25),	(-20), 0},	/*player_add_triple_fuda*/	/*(-24)==(4*(- 6))*/
/*04*/	{t256(+15), t256(+25),	( 20), 0},	/*player_add_triple_fuda*/	/*( 24)==(4*(  6))*/
/*05*/	{t256(-15), t256(+25),	(-30), 0},	/*player_add_quad_fuda*/	/*(-40)==(4*(-10))*/
/*06*/	{t256(+15), t256(+25),	( 30), 0},	/*player_add_quad_fuda*/	/*( 40)==(4*( 10))*/
//
/*07*/	{t256(-15), t256(+25),	(-45), 0},	/*player_add_five_fuda*/	/*(-60)==(4*(-15))*/
/*08*/	{t256(+15), t256(+25),	( 45), 0},	/*player_add_five_fuda*/	/*( 60)==(4*( 15))*/

/* 非誘導ショット(霊夢) */

/*10*/	{t256(	0), t256( 20),	(  0), 0},	/*player_add_fuda*/ 		/*player_add_star*/
/*11*/	{t256( -5), t256( 20),	(  0), 0},	/*player_add_dou ble_fuda*/ /*player_add_dou ble_star*/
/*12*/	{t256( +5), t256( 20),	(  0), 0},	/*player_add_dou ble_fuda*/ /*player_add_dou ble_star*/
//
/*13*/	{t256( -5), t256(+20),	(-25), 1},	/*player_add_five_fuda*/	/*(-32)==(4*(- 8))*/
/*14*/	{t256( +5), t256(+20),	( 25), 1},	/*player_add_five_fuda*/	/*( 32)==(4*(  8))*/

/* 非誘導ショット(魔理沙) */

/*03*/	//256( -5), t256(+20),	(  0), 0},	/*player_add_triple_star*/	/*(-16)==(4*(- 4))*/	/* 魔理沙 まっすぐの方がよくない？ */
/*04*/	//256( +5), t256(+20),	(  0), 0},	/*player_add_triple_star*/	/*( 16)==(4*(  4))*/	/* 魔理沙 まっすぐの方がよくない？ */
/*03*/	{t256(	0), t256( 30),	( -8), 0},	/*player_add_triple_star*/	/*( -8)==(4*(- 2))*/	/* 魔理沙 中心部から出てあまり拡散しない */
/*04*/	{t256(	0), t256( 30),	(  8), 0},	/*player_add_triple_star*/	/*(  8)==(4*(  2))*/	/* 魔理沙 中心部から出てあまり拡散しない */
/*05*/	//256( -6), t256(+20),	( -4), 0},	/*player_add_quad_star*/	/*( -4)==(4*(- 1))*/	/*(-12)==(4*(- 3))*/
/*06*/	//256( +6), t256(+20),	(  4), 0},	/*player_add_quad_star*/	/*(  4)==(4*(  1))*/	/*( 12)==(4*(  3))*/
/*07*/	{t256(-15), t256(+25),	(  0), 0},	/*player_add_quad_star*/	/*(-28)==(4*(- 7))*/	/* 魔理沙 まっすぐの方がよくない？ */	/*t256(-10)*/
/*08*/	{t256(+15), t256(+25),	(  0), 0},	/*player_add_quad_star*/	/*( 28)==(4*(  7))*/	/* 魔理沙 まっすぐの方がよくない？ */	/*t256(+10)*/

/* 非誘導ショット(幽々子) */

/*01*/	{t256(-15), t256(-25),	(-25), 0},	/*幽々子*/					/*(-32)==(4*(- 8))*/
/*02*/	{t256(+15), t256(-25),	(+25), 0},	/*幽々子*/					/*( 32)==(4*(  8))*/
/*03*/	{t256(-25), t256(-25),	(-50), 0},	/*幽々子*/					/*(-64)==(4*(-16))*/
/*04*/	{t256(+25), t256(-25),	(+50), 0},	/*幽々子*/					/*( 64)==(4*( 16))*/
	};
	SPRITE *shot;
//	shot					= sprite_add_res( BASE_PLAZMA_PNG+select_player );
	shot					= sprite_add_bullet( BASE_SHOT_ATARI_0a+select_player );
//	shot->type				= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_SHOT_02|SP_GROUP_SHOT_BOSS)-r_tbl[shot_type][REI03_gra_type];
	#if 1
	if (0 == r_tbl[shot_type][REI03_gra_type])
	{
		shot->type				= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_SHOT_02|SP_GROUP_SHOT_BOSS);
	}
	else	/* 霊夢の回転ショット */
	{
		shot->type				= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_SHOT_01|SP_GROUP_SHOT_BOSS);
		/* 描画用グラ回転 */
		shot->m_angleCCW512 =	(ra_nd() & 0x1ff);/* 出だしの角度はランダムでないと(レーザーみたいな)変な画面になる */
	}
	#endif
	shot->x256				= (player->x256)+((player->w128-shot->w128))	+ r_tbl[shot_type][REI00_x_offset];
	shot->y256				= (player->y256)-((shot->h128+shot->h128))		+ r_tbl[shot_type][REI01_y_offset]/*20*/;
	shot->callback_mover	= player_move_shot;
	shot->flags 			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
	shot->alpha 			= 150;/*player_add_triplestar*/
	PL_SHOT_DATA *data;
	data					= mmalloc(sizeof(PL_SHOT_DATA));
	shot->data				= data;
//
	#define CCC_STRENGTH		(PLAYERS5*0)
	#define CCC_SPEED			(PLAYERS5*1)
	#define CCC_MAX 			(PLAYERS5*2)		/* 最大数 */
	static Uint8 ccc_tbl[CCC_MAX] =
	{/* REIMU MARISA REMILIA CIRNO YUYUKO */
		 2, 3, 4, 5,16, 	/* strength ショットの強さ */
		16,10, 9, 8, 4, 	/* speed	ショットの速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */
	};
//		 2, 3, 4, 3, 3, 	/* strength ショットの強さ */
//		15,12,18,12,12, 	/* speed	ショットの速さ	*/
/* 極端に機体の性能が偏るのは良くない。(強くて速いのはずるい) */
	data->strength	= ccc_tbl[CCC_STRENGTH+select_player];

//
	int int_angle512;
//	int ok;
//	ok=0;
	{
	//	if (1==r_tbl[shot_type][REI03_yudo])	/* pspは0レジスタがあるので0と比較したほうが速い */
		#if 0
		if (0 != r_tbl[shot_type][REI03_yudo])/* 自動ショットか？ */
		#else
		if ( (REIMU_SHOT_TYPE_10) > shot_type)/* 自動ショットか？ */
		#endif
		{
			SPRITE *target;
			target = search_enemy_by_sprite();
			if (target != NULL)
			{
				/*data->*/int_angle512	= atan_512(target->y256-player->y256,target->x256-player->x256);
			//	if (255 < /*data->*/int_angle512 )/* 上方180-360度[/360度]の領域のみ追尾する */
#if 0
/* CWの場合 */
				if ( /*(384)*/(128*3) < ((/*data->*/int_angle512+64)&(512-1)) )/* 上方225-315度[/360度]の領域のみ追尾する */
#else
/* CCWの場合 */
				if ( /*(384)*/(128*3) < ((/*data->*/int_angle512+64+128)&(512-1)) )/* 上方225-315度[/360度]の領域のみ追尾する */
#endif
				{
#if 0
/* CWの場合 */
					data->vx256 = cos512((/*angle512*/int_angle512))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+select_player]*/)/**fps_fa_ctor*/;
					data->vy256 = sin512((/*angle512*/int_angle512))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+select_player]*/)/**fps_fa_ctor*/;
#else
/* CCWの場合 */
					data->vx256 = sin512((/*angle512*/int_angle512))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+select_player]*/)/**fps_fa_ctor*/;
					data->vy256 = cos512((/*angle512*/int_angle512))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+select_player]*/)/**fps_fa_ctor*/;
#endif
					#if 1
					/* 描画用グラ回転 */
				//	shot->m_angleCCW512 	=	int_angle512;
					/* 描画用グラ回転(ショットは上が正面なので、回転角を計算)[180/360度]回す。 */
					int_angle512 += deg_360_to_512CCW((180));
					mask512(int_angle512);
					shot->m_angleCCW512 	=	int_angle512;/* 下が正面の角度が描画用角度 */
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
		data->vx256 = ((/*int_vx*/r_tbl[shot_type][REI02_vx256]))*/*p->speed*/(ccc_tbl[CCC_SPEED+select_player])/**fps_fa_ctor*/;
		data->vy256 = -(((int)ccc_tbl[CCC_SPEED+select_player])<<8);/**fps_fa_ctor*/
	}
//	data->speed 	= ccc_tbl[CCC_SPEED+select_player];
}

/*---------------------------------------------------------
	プレイヤー弾(ショットのみ)生成
---------------------------------------------------------*/

static void player_add_single_shot(SPRITE *player)	/* 霊夢 ＆ 魔理沙 ＆ チルノ ＆ レミ */	/* [***090220 追加 */
{
	player_add_shot(player,REIMU_SHOT_TYPE_10);
}
static void player_add_dual_shot(SPRITE *player)	/* 霊夢 ＆ 魔理沙 ＆ チルノ ＆ レミ */	/* [***090220 追加 */
{
	player_add_shot(player,REIMU_SHOT_TYPE_11);
	player_add_shot(player,REIMU_SHOT_TYPE_12);
}

static void player_add_triple_fuda(SPRITE *player)	/* 霊夢 */
{
	player_add_shot(player,REIMU_SHOT_TYPE_91);/* 自動ショット */
	player_add_shot(player,REIMU_SHOT_TYPE_10);
	player_add_shot(player,REIMU_SHOT_TYPE_92);/* 自動ショット */
}

static void player_add_quad_fuda(SPRITE *player)	/* 霊夢 */
{
	player_add_shot(player,REIMU_SHOT_TYPE_93);/* 自動ショット */
	player_add_shot(player,REIMU_SHOT_TYPE_11);
	player_add_shot(player,REIMU_SHOT_TYPE_12);
	player_add_shot(player,REIMU_SHOT_TYPE_94);/* 自動ショット */
}

static void player_add_five_fuda(SPRITE *player)	/* 霊夢 */
{
	player_add_shot(player,REIMU_SHOT_TYPE_95);/* 自動ショット */
	player_add_shot(player,REIMU_SHOT_TYPE_13);
	player_add_shot(player,REIMU_SHOT_TYPE_10);
	player_add_shot(player,REIMU_SHOT_TYPE_14);
	player_add_shot(player,REIMU_SHOT_TYPE_96);/* 自動ショット */
}

static void player_add_dual_star(SPRITE *player)	/* 魔理沙 */
{
	player_add_shot(player,MARISA_SHOT_TYPE_03);	/* 微拡散 */
	player_add_shot(player,MARISA_SHOT_TYPE_04);	/* 微拡散 */
}

static void player_add_triple_star(SPRITE *player)	/* 魔理沙 ＆ チルノ */
{
	player_add_shot(player,MARISA_SHOT_TYPE_03);	/* 微拡散 */	/* REIMU_SHOT_TYPE_11 まっすぐ */
	player_add_shot(player,REIMU_SHOT_TYPE_10);
	player_add_shot(player,MARISA_SHOT_TYPE_04);	/* 微拡散 */	/* REIMU_SHOT_TYPE_12 まっすぐ */
}

static void player_add_quad_star(SPRITE *player)	/* 魔理沙 */
{
	player_add_shot(player,REIMU_SHOT_TYPE_11); 	/* REIMU_SHOT_TYPE_11 まっすぐ */	/*MARISA_SHOT_TYPE_05*/
	player_add_shot(player,REIMU_SHOT_TYPE_12); 	/* REIMU_SHOT_TYPE_12 まっすぐ */	/*MARISA_SHOT_TYPE_06*/
	player_add_shot(player,MARISA_SHOT_TYPE_07);
	player_add_shot(player,MARISA_SHOT_TYPE_08);
}

static void player_add_chou(SPRITE *player) 		/* 幽々子 */
{
	player_add_shot(player,YUYUKO_SHOT_TYPE_01);
	player_add_shot(player,YUYUKO_SHOT_TYPE_02);
	player_add_shot(player,YUYUKO_SHOT_TYPE_03);
	player_add_shot(player,YUYUKO_SHOT_TYPE_04);
}

/*---------------------------------------------------------
	プレイヤーシールド生成(霊夢)
---------------------------------------------------------*/

static void player_create_bomber_kekkai_parrent(SPRITE *s)		/* 霊夢 */
{
	PLAYER_DATA *pd = (PLAYER_DATA *)s->data;
	pd->state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/
	int ii;//	int i;	/* 半象限ずつ */
	for (ii=0; ii<(8); ii++)//	for (i=0;i<360/*i<=359*/;i+=45)
	{
		SPRITE *c;
		PL_KEKKAI_DATA *d;
		if (0 == (ii&1))// if (i%90 == 0)
		{
		//	c				= spr ite_add_file 0("bomber1_re.png", 32/*36*/, PRIORITY_03_ENEMY/*PRIORITY_02_PLAYER*/, 1);/*"cshoot1.png"*/
		//	c				= sprite_add_res(BASE_BOMBER1_PNG);
			c				= sprite_add_bullet(TAMA_ATARI_04);
			d				= mmalloc(sizeof(PL_KEKKAI_DATA));
			c->data 		= d;
			d->add_r512 	= (23); 							/* 時計回り */
			d->radius		= 5/*(48)*/;	//d->radius=38; 	/*半径初期値*/	/* 少しずつ広がるようにした */
		}
		else		/* 1つだと不安なので2つ作った。 */
		{
		//	c				= spr ite_add_file 0("bomber2_re.png", 32/*36*/, PRIORITY_03_ENEMY/*PRIORITY_02_PLAYER*/, 1);/*"cshoot2r.png""cshoot2.png"*/
		//	c				= sprite_add_res(BASE_BOMBER2_PNG);
			c				= sprite_add_bullet(TAMA_ATARI_04);
			d				= mmalloc(sizeof(PL_KEKKAI_DATA));
			c->data 		= d;
			d->add_r512 	= (-17);							/* 反時計回り */
			d->radius		= 0/*(45)*/;	//d->radius=35; 	/*半径初期値*/	/* 少しずつ広がるようにした */
		}
//		if (YUYUKO==select_player)
//		{	/* 幽々子 */
//			d->radius		+= 45/*45*/;		/*半径初期値*/
//			c->type 		= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO)/*ボスに無効*/;
//		}
//		else // if (REIMU==select_player)
		{	/* 霊夢 */
			c->type 		= (ii&(4-1))+(/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_02|SP_GROUP_SHOT_BOSS)/*ボスに有効*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/
		}
		c->callback_mover	= player_move_kekkai;			/* 時計回り＆反時計回り */
	//	c->callback_mover	= player_move_kekkai/*2*/;		/* 反時計回り */
		c->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
	//	d->angle512 		= (ii*deg_360_to_512((45)))/*i*/;
//		d->angleCCW512		= (ii*deg_360_to_512((45)))/*i*/;
		d->angleCCW512		= (ii*deg_360_to_512CCW((45)))/*i*/;
		/* 出始めはプレイヤー座標 */
		c->x256 			= player->x256;//+co_s512(((ii*deg_360_to_512(45))/*i*/))*d->radius+((player->w-s->w)>>1);
		c->y256 			= player->y256;//+si_n512(((ii*deg_360_to_512(45))/*i*/))*d->radius+((player->h-s->h)>>1); /* 時計回り */
		d->strength 		= 1/*5*/;/* [***090214 追加 */
	}
}

/*---------------------------------------------------------
	[プレイヤーシールド]ボム生成
	(魔理沙 ＆ チルノ＆ 幽々子)
---------------------------------------------------------*/

static void player_create_bomber_levarie_parrent(SPRITE *s)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)s->data;
	pd->state_flag	|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/
	SPRITE *c;
	c = NULL;
	PL_LEVARIE_DATA *data;
	int j;		j=0;
		#if 0
	int hhh;	hhh = BASE_BOMBER1_PNG+select_player;
		#endif
	int angCCW512; angCCW512 = (0)/*0*/;/*チルノ用(暫定的)*/	//angCCW512 = ra d2deg512(((M_PI*2)/32))/*0*/;/*チルノ用(暫定的)*/
	int iii;
	for (iii=0; iii<(18*3)/*16*/; iii++)
	{
	//	c					= spr ite_add_file 0((char *)bbb_name[(k+j)], 3, PRIORITY_01_SHOT/*PRIORITY_02_PLAYER*/, 0);
	//	c					= sprite_add_res(hhh);hhh += PLAYERS5;
		c					= sprite_add_bullet(BASE_BOMBER_ATARI_0a+select_player);
		j++;if (6==j)
		{
			j=0;
		#if 0
			hhh = BASE_BOMBER1_PNG+select_player;
		#endif
		}
		c->anim_speed		= 5;
		c->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
		c->callback_mover	= player_move_levarie;
		#if 0
		{
			Uint16 player_fix_status_ggg[8] =
			{/* REIMU MARISA REMILIA CIRNO YUYUKO */
				256, 256, 32, 32, 256, 0,0,0,	/* 描画用ボムの初期サイズ / size of bomb at first. */
			};
			c->m_zoom_x256		= player_fix_status_ggg[select_player]/*8*/;/* 64 == (1/4) */
		}
		#else
		if (YUYUKO!=select_player)
		{
			c->m_zoom_x256		= 1+(ra_nd()&(64-1));
		}
		#endif

		data					= mmalloc(sizeof(PL_LEVARIE_DATA));
		c->data 			= data;
		data->strength		= player_fix_status[BASE_STD_BOMB_STRENGTH+select_player] /*k_strength*/ /*5*/ /*15-k-k*/;/* [***090214 追加 */
		/* MARISAは強いが拡散してしまうのであたらない。  */
		/* チルノは当てやすいのでその分弱くしないと強すぎちゃう。  */
//		data->angle512		= angCCW512;
		int d_speed256;
		#if 0
		if (0==(iii&(2-1)/*%2*/))
		{			/*d->*/d_speed256	= t256(1.0);	c->alpha	= 150;	}
		else	{	/*d->*/d_speed256	= t256(1.2);	c->alpha	= 180;	}
		#else
		/*d->*/d_speed256	= t256(1.0)+(iii<<3);
		#endif
#if 0
/* CWの場合 */
		data->vx256 		= ((cos512((/*angle512*/angCCW512))*/*p->speed*/(d_speed256))>>8)/**fps_fa_ctor*/;
		data->vy256 		= ((sin512((/*angle512*/angCCW512))*/*p->speed*/(d_speed256))>>8)/**fps_fa_ctor*/;
#else
/* CCWの場合 */
		data->vx256 		= ((sin512((/*angle512*/angCCW512))*/*p->speed*/(d_speed256))>>8)/**fps_fa_ctor*/;
		data->vy256 		= ((cos512((/*angle512*/angCCW512))*/*p->speed*/(d_speed256))>>8)/**fps_fa_ctor*/;
#endif
//
		c->x256 			= (s->x256)+((s->w128-c->w128));
		c->y256 			= (s->y256)+((s->h128-c->h128));
		/* ギャストドリーム(もどき)。スターダストレヴァリエと違い近接性能は無い。 */
		if (YUYUKO==select_player)	/* 幽々子 */
		{
			c->x256 		+= ((data->vx256)<<5);
			c->y256 		+= ((data->vy256)<<5);
		#if 0
			if (BASE_BOMBER5_PNG_yu==hhh)	{	hhh=BASE_BOMBER3_PNG_yu;}
		#endif
//			c->type 		= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_02|SP_GROUP_SHOT_ZAKO)/*ボスに無効*/;/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_BOSS)*/ /*ボスに有効*/
//			/* 幽々子 強すぎるからな～(効果弱いが効果あるのも出来るようにシステム見直したほうがいいかも) */
		}
//		else
		{
			c->type 		= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_BOMBER_02|SP_GROUP_SHOT_BOSS)/*ボスに有効*/ /*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/;
		}
		c->type += j;
		#if 1
		/* 描画用角度(下が0度で左回り(反時計回り)) */
		c->m_angleCCW512		= angCCW512;
		#endif
	//	angCCW512 += (int)((512)/16); // 22.5度/360, π/8 /* 0.392699081698724154810 */
		angCCW512 += (int)((512)/18); // 20.0度/360, π/9 /* 0.349065850398865915384 */
	}
}

/*---------------------------------------------------------
	プレイヤーシールド生成(幽々子)[幽々子ボムの扇本体]
---------------------------------------------------------*/

static void player_create_bomber_oogi_parrent(SPRITE *s)
{
//	PLAYER_DATA *pd 	= (PLAYER_DATA *)s->data;
//	pd->state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/	/*別(player_create_bomber_levarie_parrent)で設定済み*/
	SPRITE *c;
//	c					= spr ite_add_file 0("bomber1_oz.png", 10, PRIORITY_01_SHOT/*PRIORITY_02_PLAYER*/, 0);/*"cross_red.png"*/
//	c					= sprite_add_res(BASE_BOMBER5_PNG_yu);
	c					= sprite_add_bullet(JIKI_ATARI_ITEM_80);
	c->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
	c->anim_speed		= 0;
	c->anim_frame		= 0;
	c->alpha			= 255;
	c->callback_mover	= player_move_add_oogi;
	c->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JIKI_SHOT_00|SP_GROUP_SHOT_ZAKO);/* ボスの直接攻撃は禁止 */		/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/
	c->x256 			= 0/*(s->x256)+((s->w128-c->w128))*/;
	c->y256 			= 272/*(s->y256)+((s->h128-c->h128))*/;
//
	/* 描画用角度 */
//	c->m_angleCCW512		= (128);
	c->m_angleCCW512		= (0);
//
	PL_SHOT_DATA *data;
	data				= mmalloc(sizeof(PL_SHOT_DATA));
	c->data 			= data;
	data->strength		= 1;		/* 扇本体 の強さ */
}

/*---------------------------------------------------------
	プレイヤーシールド生成(レミリア)[レミリアボムの十字炎本体]
---------------------------------------------------------*/

static void player_create_bomber_cross_red_parrent(SPRITE *s) /* レミ */ /* [***090220 追加 */
{
	PLAYER_DATA *pd 	= (PLAYER_DATA *)s->data;
	pd->state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/
	SPRITE *c;
//	c					= spr ite_add_file 0("bomber1_oz.png", 10, PRIORITY_01_SHOT/*PRIORITY_02_PLAYER*/, 0);/*"cross_red.png"*/
//	c					= sprite_add_res(BASE_BOMBER1_PNG_oz);
	c					= sprite_add_bullet(JIKI_ATARI_ITEM_16);
	c->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
	c->flags			&= (~(SP_FLAG_VISIBLE));	/*非表示*/
	c->anim_speed		= 0;
	c->anim_frame		= 0;
	c->alpha			= 220;
	c->callback_mover	= player_move_add_cross_red;
//	c->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO);/* ボスの直接攻撃は禁止 */		/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/
	c->type 			= (/*表示しない*/SP_GROUP_ETC/*SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO*/);/* ボスの直接攻撃は禁止 */		/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/
	c->x256 			= (s->x256)+((s->w128-c->w128));
	c->y256 			= (s->y256)+((s->h128-c->h128));
}


/*---------------------------------------------------------
	プレイヤー、誘導弾の移動制御
---------------------------------------------------------*/

//#define hlaser_NUM_OF_ENEMIES (24)
#define hlaser_NUM_OF_ENEMIES (12)		/* [***090128		半分にしてみる */

/*---------------------------------------------------------
	プレイヤー誘導弾生成
---------------------------------------------------------*/

static void player_add_hlaser(SPRITE *src)
{
	int j;
	j=(ra_nd()&(2-1));//for (j=0;j<2;j++)
	{
		SPRITE *s_old;	s_old = NULL;
		SPRITE *s;		s = NULL;
		int i;
		for (i=0; i<hlaser_NUM_OF_ENEMIES; i++)
		{
			s_old = s;
		//	s				= sprite_add_res( BASE_TSHOOT_PNG+select_player );
			s				= sprite_add_bullet(JIKI_ATARI_ITEM_16);
			s->type 		= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_BOSS)/*ボスに有効*/ /*SP_PL_HLASER*/;
			s->x256 		= (src->x256) + ((src->w128)) - t256(5);
			s->y256 		= (src->y256) + ((src->h128)) + t256(15);
		//	s->flags		|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
			s->flags			&= (~(SP_FLAG_VISIBLE));	/*非表示*/
			s->anim_frame	= 5-((double)6.0/hlaser_NUM_OF_ENEMIES)*i;
			if (0==i)
			{
				s->callback_mover	= player_move_parrent_hlaser;
				PL_HOMING_DATA *data;
				data				= mmalloc(sizeof(PL_HOMING_DATA));
				s->data 			= data;
				data->nr			= j;
//				data->angle512		= deg_360_to_512((100));
//				data->angle512		= deg_360_to_512CCW(360-(100));
				data->angleCCW512	= deg_360_to_512CCW(0);
				data->speed256		= t256(12);
				data->strength		= player_fix_status[BASE_LOW_BOMB_STRENGTH+select_player]/*6*/;/*低速ボムの強さ*/
				data->time_out/*range*/ 	= 100/*200*/;
				data->state 		= 0;
				data->target_obj	= NULL; 	//b->target_obj/*target_id*/	= /*-1*/NULL;
			}
			else
			{
				s->callback_mover	= player_move_kodomo_hlaser/*NULL*/;
				PL_HOMING_KODOMO_DATA *data;
				data				= mmalloc(sizeof(PL_HOMING_KODOMO_DATA));
				s->data 			= data;
			//	s->data 			= NULL;
				data->target_obj	= s_old;
				data->time_out		= 8;
			}
		}
	}
}
#undef hlaser_NUM_OF_ENEMIES


static void player_move_add_bomber_hlaser(SPRITE *s)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
//	if (0==(((int)pd->bomber_time)&0x3f))
	if (0==(((int)pd->bomber_time)&0x0f))
	{
		/* シナリオ時には誘導弾を追加しない */
		if (0==(pd->state_flag & STATE_FLAG_06_IS_SCRIPT))
		{
			player_add_hlaser(player);
		}
	}
	#if 0
	s->anim_frame		= (((int)pd->bomber_time>>4)&0x03);/*"bomber_slow.png"*/
	s->x256 			= (player->x256)+((player->w128-s->w128));
	s->y256 			= (player->y256)+((player->h128-s->h128));
	#endif
//
	if (1 > pd->bomber_time) {	s->type = SP_DELETE;}/* 時間制限でおしまい */
}

/* 低速ボムの親 */
static void player_create_bomber_homing_parrent(SPRITE *s)
{
	PLAYER_DATA *pd 	= (PLAYER_DATA *)s->data;
	pd->state_flag		|= STATE_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/
//
	SPRITE *c;
//	c					= spr ite_add_file 0("bomber_slow.png", 4, PRIORITY_01_SHOT/*P R_BACK1*/, 1);	c->anim_speed		= 0;
//	c					= sprite_add_res(BASE_BOMBER_SLOW_PNG);
	c					= sprite_add_bullet(JIKI_ATARI_ITEM_16);
//	c->flags			|= (SP_FLAG_VISIBLE/*|SP_FLAG_TIME_OVER*/);
	c->flags			&= (~(SP_FLAG_VISIBLE));	/*非表示*/
	c->data 			= 0/*d*/;
	c->anim_frame		= 0;
	c->alpha			= 128/*220*/;
	c->callback_mover	= player_move_add_bomber_hlaser;
//	c->type 			= (SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_BOSS)/*ボスに有効*/;	/* 低速ボムもボスに有効とする */	/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/
	c->type 			= (/*表示しない*/SP_GROUP_ETC/*SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_BOSS*/)/*ボスに有効*/;	/* 低速ボムもボスに有効とする */	/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/
	c->x256 			= (s->x256)+((s->w128-c->w128));
	c->y256 			= (s->y256)+((s->h128-c->h128));
}


/*---------------------------------------------------------
	プレイヤーキー操作
---------------------------------------------------------*/

//static int /*bomb_wait*/d->bomber_time;		/* ボムの有効時間 */	//次のボムを出せるまでの時間
static int weapon_List; 	//どの武器を装備しているか

	enum /*_weapon_type_*/
	{
		WEAPON_L1=0,
		WEAPON_L2,
		WEAPON_L3,
		WEAPON_L4,
		WEAPON_L5,
	//	WEAPON_L6,
		WP_MAX		/* 最大数 */
	};

extern /*global*/short my_analog_x; /* アナログ量、補正済み */
extern /*global*/short my_analog_y; /* アナログ量、補正済み */
extern void set_bg_alpha(int set_bg_alpha);
static void player_keycontrol(SPRITE *s1)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)s1->data;
	//if player is invisible (state gameover) allow no keycontrol
	if ((0==(s1->flags&SP_FLAG_VISIBLE)))
	{
		return;
	}

	/*---------------------------------------------------------
		プレイヤーボム関連処理
	---------------------------------------------------------*/
//	if (STATE_FLAG_06_IS_SCRIPT==(pd->state_flag & STATE_FLAG_06_IS_SCRIPT))
//	{
//		;	/* 会話イベント中はボム関連の処理はしない */
//	}
//	else
	if (0==(pd->state_flag & STATE_FLAG_06_IS_SCRIPT))
	{
	//	if (d->bomber_time>0)
		if (0 < /*bomb_wait*/pd->bomber_time)		/* ボムウェイト処理 */
		{
			//	d->bomber_time-=fps_fa_ctor;
			/*bomb_wait*/pd->bomber_time--/*-=fps_fa_ctor*/;/**/
			if (2 > /*bomb_wait*/pd->bomber_time)
			{
				set_bg_alpha(255);/* 画面を明るくする */
		//	}
		//	else
		//	if (/*bomb_wait*/pd->bomber_time < 190)
		//	{
				pd->state_flag &= (~STATE_FLAG_02_BOMB_AUTO_GET_ITEM);	/* ボムによる自動収集は終わり */
			}
		}
		else
		{
			pd->bomber_time = 0;
		//	pd->ex tra_type=PLX_NONE;
		//}
		//if (/*bomb_wait*/d->bomber_time<=0)
		//{
			if (0 < pd->bombs)
			{
				if (my_pad & PSP_KEY_BOMB_CANCEL)
				{
					play_voice_auto_track(VOICE07_BOMB);
					set_bg_alpha(50);/* 画面を暗くする */
//					set_bg_alpha(100);/* 画面を暗くする */
//					set_bg_alpha(127);/* 画面を暗くする */
					/* 低速ボム */
					if (my_pad & PSP_KEY_SLOW)
					{
				//	case (REIMU*2+TEISOKU_MODE):/*とりあえず*/
				//	case (MARISA*2+TEISOKU_MODE):/*とりあえず*/
				//	case (REMILIA*2+TEISOKU_MODE):/*とりあえず*/
				//	case (CIRNO*2+TEISOKU_MODE):/*とりあえず*/
				//	case (YUYUKO*2+TEISOKU_MODE):/*とりあえず*/
						pd->bomber_time = 200/*320*/ /*400*/ /*800*/;	/* ボムの有効時間(設定無敵時間)[※１仕様が変わったので半分にした] */
						item_from_bullets(SP_ITEM_05_HOSI);/* 弾全部消す */
						player_create_bomber_homing_parrent(player);	/*player_move_add_bomber_hlaser*/
				//		break;
					}
					/* 通常ボム */
					else
					{
						switch ((select_player))
						{
					//	case (YUYUKO*2+TEISOKU_MODE):/*とりあえず*/
					//		pd->bomber_time = 300;			/* ボムの有効時間(設定無敵時間) */	/* こちらはボス無効(防御型) */
					//		goto common_kekkai;
						case (REIMU):
						//	/*bomb_wait*/d->bomber_time 	= 200;
							pd->bomber_time = 180/*200*/;	/* ボムの有効時間(設定無敵時間) */	/* こちらはボス有効(攻撃型) */
						//	pd_save_timer	= 200+30;		//設定無敵時間。たまにシールドをすり抜ける者が現れるので
						//	pd->ex tra_type = PLX_BOMB;
					//	common_kekkai:
							player_create_bomber_kekkai_parrent(player);		//シールドの追加
							break;
						case (CIRNO):/*とりあえず*/
						case (MARISA):
						case (YUYUKO):
						//	/*bomb_wait*/d->bomber_time 	= 100;
							pd->bomber_time = 32/*100*/;			/* ボムの有効時間(設定無敵時間) */
						//	pd_save_timer	= 100+30/*200 5*/;	// 設定無敵時間。つーかさすがにボム時間中無敵じゃないのはきつい
						//	pd->ex tra_type = PLX_BOMB; 	/*←ここが無いと(extra_tem有効時に)ボムが発動出来ないよ*/
							if (YUYUKO==select_player)
							{
								/* 扇 有効時間 */
								pd->bomber_time = 255/*255==((16*4*2*2)-1)*/;			/* ボムの有効時間(設定無敵時間) */
								player_create_bomber_oogi_parrent(player);
							}
							player_create_bomber_levarie_parrent(player);
							break;
					//	case (YUYUKO):
					//		/* 扇 有効時間 */
					//		pd->bomber_time = 255/*255==((16*4*2*2)-1)*/;			/* ボムの有効時間(設定無敵時間) */
					//		player_create_bomber_levarie_parrent(player);
					//		player_create_bomber_oogi_parrent(player);
					//		break;
						case (REMILIA): /* [***090220 追加 */
						//	/*bomb_wait*/d->bomber_time 	= 150/*300*/ /*400*/;	/*400(150+250)は明らかに長すぎる*/
							/* レミボムは強すぎるので、4発から3発に減らし、加速ボムにして当て難くした。 */
							/* ただしレミボムはボスに有効とし、当たれば強い。 */
						//	pd->bomber_time = 0xfe	/* 255==0xff==4発	254==0xfe==3発==0xbf=192 2発==0x7f 150==0x96*/;
							pd->bomber_time = 0x7e	/* 255==0xff==8発	254==0xfe==7発==0xbf=192 4発==0x7f 150==0x96*/;
						//	pd_save_timer	= 150+30;		//設定無敵時間。
						//	pd->ex tra_type = PLX_BOMB;
							player_create_bomber_cross_red_parrent(player);
							break;
						}
					}
					pd_save_timer		= pd->bomber_time + USER_BOMOUT_WAIT;/*実質無敵時間*/
				//	pd->ex tra_type 	= PLX_BOMB;
					s1->alpha			= 80;	/*半透明*/
					pd->core->alpha 	= 80;	/*半透明*/
					pd_player_status	= PLAYER_STATE_04_SAVE_02;
					pd->bombs--;
				}
			}
		}
	}
					/*	[※１] homing / hlaser は低速ボムになり武器使用中は無敵となったので、
						従来の有効時間では長すぎる。そこで有効時間を半分にした。*/
				//	#define USER_BOMOUT_WAIT (30)
					/*	[※２] 実質無敵時間 == (設定無敵時間 + USER_BOMOUT_WAIT)
						フレームスキップが入っていたり処理落ちしたりする関係上、
						グラが必ずしも描き終わっているとは限らない。
						もし描き終わっていない場合は、実質無敵時間 == 設定無敵時間 だと、
						画面にボムが表示されているのに判定で死ぬ。	*/
					/*	[※３]※２の余裕分を入れても、現在の状態ではボムの終わりが
						非常にわかりにくい。というか実質わからない。
						何らかの警告を組み入れる事が絶対必要。(だがまだ作ってない)
						(本家では色々警告がある) */


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
	static const signed /*int*/short jiki_move_length[16][2] =
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
	#if 0
	/* 高速モード(ボム発動時) */
	/* REIMU */ 	t256(3.0),
	/* MARISA */	t256(2.0),
	/* REMILIA */	t256(5.0),
	/* CIRNO */ 	t256(4.5),/*⑨だから低速の方が速い*/
	/* YUYUKO */	t256(5.0),
	/* 低速モード(ボム発動時) */
	/* REIMU */ 	t256(2.0),
	/* MARISA */	t256(2.0),
	/* REMILIA */	t256(4.0),
	/* CIRNO */ 	t256(5.0),/*⑨だから低速の方が速い*/
	/* YUYUKO */	t256(4.5),
		/*+*/
		/*((1 > pd->bomber_time)?(0):(PLAYERS5+PLAYERS5))*/
	#endif
/* 花映塚会話何たらサイト http://ambriel.hp.infoseek.co.jp/th9/  基本性能比較 移動速度(高速モード)
														[51200/xxxF]
200/50		50F 文										0x0400[1024.000000000000000000000000000000]3.63636363636363636363636363636364
200/55		55F [魔理沙] ミスティア (メディ , 四季) 	0x03a3[ 930.909090909090909090909090909091]3.63636363636363636363636363636364
200/58		58F [チルノ]								0x0373[ 882.758620689655172413793103448276]3.44827586206896551724137931034483
200/61		61F (鈴仙 , ルナサ) てゐ					0x0347[ 839.344262295081967213114754098361]3.27868852459016393442622950819672		[REMILIA]
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
200/109 	109F	(鈴仙 , ルナサ) 					0x01d6[ 469.724770642201834862385321100917]1.83486238532110091743119266055046		[REMILIA]
200/114 	114F	(メディ , 四季) 					0x01c1[ 449.122807017543859649122807017544]1.75438596491228070175438596491228
200/124 	124F	リリカ 小町 						0x019d[ 412.903225806451612903225806451613]1.61290322580645161290322580645161
200/130 	130F	妖夢								0x018a[ 393.846153846153846153846153846154]1.53846153846153846153846153846154
200/136 	136F	メルラン [霊夢] 					0x0178[ 376.470588235294117647058823529412]1.47058823529411764705882352941176
200/145 	145F	てゐ								0x0161[ 353.103448275862068965517241379310]1.37931034482758620689655172413793		[YUYUKO]
200/148 	148F	幽香								0x015a[ 345.945945945945945945945945945946]1.35135135135135135135135135135135

*/
//#define BASE_SPEED_MINIMUM		(PLAYERS5*4)
//#define BASE_SPEED_MAXIMUM		(PLAYERS5*5)
//	 2,  3,  4,  7,  3, 	/* 最低速度 player_speed_minimum */
//	 4,  5,  4,  7,  3, 	/* 最高速度 player_speed_maximum */
	static const signed /*int*/short player_speed256[(PLAYERS5*4)] =
	{
	/* 高速モード(通常時) */
	/* REIMU */ 	0x02f1,/* t256(2.94) 2.94140625 */
	/* MARISA */	0x03a3,/* t256(3.64) 3.63671875 */
	/* REMILIA */	0x0347,/* t256(3.28) 3.27734375 */
	/* CIRNO */ 	0x0373,/* t256(3.45) 3.44921875 */ /*⑨だから低速の方が速い*/
	/* YUYUKO */	0x02c7,/* t256(2.78) 2.77734375 */
	/* 低速モード(通常時) */
	/* REIMU */ 	0x0178,/* t256(1.47) 1.46875000  t256(2.0)*/
	/* MARISA */	0x0233,/* t256(2.20) 2.19921875  t256(2.5)*/
	/* REMILIA */	0x01d6,/* t256(1.84) 1.83593750  t256(2.5)*/
	/* CIRNO */ 	/*0x0700*/0x0269/*0x0269*/,/* t256(2.41) 2.41015625  t256(7.0)*/ /*⑨だから低速の方が速い 0x0400==文高速時==幻想郷最速？	*/
	/* YUYUKO */	0x0161,/* t256(1.38) 1.37890625  t256(2.0)*/
	};
	/*const*/ signed int my_speed = player_speed256[select_player+	((my_pad & PSP_KEY_SLOW)?(PLAYERS5):(0))];
	short	aaa_my_analog_x = (((my_speed)*(my_analog_x))>>8);
	short	aaa_my_analog_y = (((my_speed)*(my_analog_y))>>8);
	s1->x256 += ((((signed int)(jiki_move_length[((my_pad&0xf0)>>4)][0]))*(aaa_my_analog_x))>>8); /**fps_fa_ctor*/
	s1->y256 += ((((signed int)(jiki_move_length[((my_pad&0xf0)>>4)][1]))*(aaa_my_analog_y))>>8); /**fps_fa_ctor*/
	// はみだしたら修正。
		 if (s1->x256 < t256(0))									{	s1->x256 = t256(0); 									}
	else if (s1->x256 > t256(GAME_WIDTH) -((s1->w128+s1->w128)))	{	s1->x256 = t256(GAME_WIDTH) -((s1->w128+s1->w128)); 	}
		 if (s1->y256 < t256(0))									{	s1->y256 = t256(0); 									}
	else if (s1->y256 > t256(GAME_HEIGHT)-((s1->h128+s1->h128)))	{	s1->y256 = t256(GAME_HEIGHT)-((s1->h128+s1->h128)); 	}

// > 【PSP】PSPで東方やりたい奴8【弾幕】
// > 200 ：名無しさん＠お腹いっぱい。：2009/01/27(火) 18:41:00 ID:xQgI7hCU
// > そろそろSアイテム消して欲しいな。
// > あとFULLパワー時のアイテム引き寄せラインはもうちょっと下の方がいいと思う。
// > 具体的にはこの辺り↓
// > ttp://www4.uploader.jp/user/isisuke/images/isisuke_uljp00081.bmp
// この意見はわたしも同様に思ったので、みんながそう思ったに違いない。
// この画像は484x283[dots]で、y==63[line]に赤線が引いてあった。
// pspの解像度は、 480x272[dots]なので x/272=63/283 , x=(63*272)/283 == 60.55[line]
// 差分/製作氏	50(49)[line] 少し上すぎる。
// スレ8の200氏 約60[line] (感覚的に)
// ここでキャラの高さ分もあるし、64(63)かもっと下72(71)ぐらいでもいいのでは？という訳で64にして実験してみる。
// 結果：やっぱもう少し下がいい。8[dots]といわず16[dots]ぐらい下がいい。再び実験してみる。
// 結果：こんなもんかな？というわけで80(79)[dots]に決定。もう8[dots]下88(87)でもいいかもね。
// 本家の感覚を大事にするならこんなもんだと思う。
#define SPLIT_LINE (t256(80))
// http://hossy.info/game/toho/k_score.php ここの画像を逆算して計算すると 77 dots あたり。
//#define SPLIT_LINE (t256(77))

	/* MAX時のアイテム自動収集 */
	if (my_pad & PSP_KEY_UP/*PSP_CTRL_UP*/) 	/* 註：斜め上でも回収可能 */ /*&& (s1->y>0)*/
	{
		if ((pd->weapon_power==127)  //128[***090123 変更 /*max==127==「128段階」*/
			/* 魔理沙 特殊能力：アイテム上部収集がいつでも可能 */
			|| (MARISA==select_player)) 	/* 魔理沙は常に上部自動収集が可能 */
		{
			if (s1->y256 < SPLIT_LINE/*50*/)/**/
			{
				pd->state_flag |= STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM;	/* 上部自動収集可能 */
			}
		}
	}
	else if (my_pad & PSP_KEY_DOWN/*PSP_CTRL_DOWN*/)	/* 註：斜め下でもやめる */ /*&& (s1->y<screen->h*-s1->h)*/
	{
		//if (pd->state_flag & (STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM /*| STATE_FLAG_02_BOMB_AUTO_GET_ITEM*/) )/*???たぶん*/
		{
			pd->state_flag &= (~STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM); 	/* 上部自動収集不可 */
		}
	}
//
	/*---------------------------------------------------------
		プレイヤーショット関連処理
	---------------------------------------------------------*/
	if (0==(pd->state_flag & STATE_FLAG_06_IS_SCRIPT))
	{
		if (my_pad & PSP_KEY_SHOT_OK)
		{
			pd->state_flag |= STATE_FLAG_15_KEY_SHOT;	/* on */
			{static int weapon_interval=0;
				/*pd->*/weapon_interval--/*=fps_fa_ctor*/;
				if (/*pd->*/weapon_interval < 1 )
				{
					#define BASE_WEAPON_L1		(PLAYERS5*0)
					#define BASE_WEAPON_L2		(PLAYERS5*1)
					#define BASE_WEAPON_L3		(PLAYERS5*2)
					#define BASE_WEAPON_L4		(PLAYERS5*3)
					#define BASE_WEAPON_L5		(PLAYERS5*4)
					#define BASE_WEAPON_MAX 	(PLAYERS5*5)
					static Uint8 www_tbl[BASE_WEAPON_MAX] =
					{/* REIMU MARISA REMILIA CIRNO YUYUKO */	/* レミ強すぎるので調整(+5) */
						 5,  6,  7+5,  9,  9+8, 	/* WEAPON_L1 */
						 5,  6,  6+5,  9,  8+8, 	/* WEAPON_L2 */
						 5,  5,  7+5,  9,  7+8, 	/* WEAPON_L3 */
						 5,  5,  6+5,  9,  6+8, 	/* WEAPON_L4 */
						 5,  5,  5+5,  9,  5+8, 	/* WEAPON_L5 */
					};
					/*pd->*/weapon_interval = www_tbl[(PLAYERS5*weapon_List)+select_player];
//
					play_voice_auto_track(VOICE00_SHOT);
					/*const*/static void (*bbb[PLAYERS5][WP_MAX])(SPRITE *sss) =
					{
				/* WEAPON_L1: */			/* WEAPON_L2: */		/* WEAPON_L3: */			/* WEAPON_L4: */			/* WEAPON_L5: */
{	/*REIMU*/	player_add_single_shot, 	player_add_dual_shot,	player_add_triple_fuda, 	player_add_quad_fuda,		player_add_five_fuda,	},
{	/*MARISA*/	player_add_single_shot, 	player_add_dual_shot,	player_add_dual_star,		player_add_triple_star, 	player_add_quad_star,	},
{	/*REMILIA*/ player_add_single_shot, 	player_add_single_shot, player_add_dual_shot,		player_add_dual_shot,		player_add_dual_shot,	},
{	/*CIRNO*/	player_add_dual_shot,		player_add_dual_star,	player_add_dual_star,		player_add_triple_star, 	player_add_triple_star, },
{	/*YUYUKO*/	player_add_chou,			player_add_chou,		player_add_chou,			player_add_chou,			player_add_chou,		},	/* 無段階成長 */
					};
					(bbb[select_player][weapon_List])(s1);
				}
			}

		}
	}
	/*---------------------------------------------------------
		プレイヤーアニメーション関連処理
	---------------------------------------------------------*/
	{static int anim_delay;
		/*pd->*/anim_delay -= 1/*fps_fa_ctor*/;
		if (0 > /*pd->*/anim_delay)
		{
			/*pd->*/anim_delay = player_fix_status[BASE_SPEED_ANIME+select_player]/*2*/;
			//
			static int auto_anim_frame = 4/*5*/;
				 if (my_pad & PSP_KEY_LEFT/*PSP_CTRL_LEFT*/)	{	if (auto_anim_frame>0	)	 auto_anim_frame--; }
			else if (my_pad & PSP_KEY_RIGHT/*PSP_CTRL_RIGHT*/)	{	if (auto_anim_frame</*7*/8/*10*/) auto_anim_frame++; }
			else
			{
				if (auto_anim_frame>4/*5*/) auto_anim_frame--;
				if (auto_anim_frame<4/*5*/) auto_anim_frame++;
			}
			{
				int now_anim_frame;
				static int auto_aniime;
				auto_aniime++;
				auto_aniime&=3;
					 if (4==auto_anim_frame)	{	now_anim_frame = (auto_anim_frame + auto_aniime);	}
				else if (4<auto_anim_frame) 	{	now_anim_frame = (auto_anim_frame + 3); 			}
				else							{	now_anim_frame = (auto_anim_frame );				}
				s1->anim_frame = (now_anim_frame);
				s1->type = (SP_GROUP_JIKI_GET_ITEM)|(now_anim_frame);
			}
		}
	}
/*
0 1 2 3 4 5 6  7  8
0 1 2 3 4 8 9 10 11
		5
		6
		7
*/
}

/*---------------------------------------------------------
	武器レベルが変更された場合に、武器やオプションの状態をチェックして変更
---------------------------------------------------------*/
/* [pd->weapon_powerが変更された場合に必ず行う後チェック] */
static void check_weapon_level(PLAYER_DATA *pd)
{
	int pd_weapon;
	pd_weapon = pd->weapon_power;
	/*---------------------------------------------------------
		武器の段階を決める
	---------------------------------------------------------*/
	//weaponの段階から今の装備を決める		//リストを見て装備の変更
	// [***090123	最大128へ。
	// (0-128の129段階だったが、0-127の128段階に修正)
		 if (pd_weapon <= ( 5-1))	{	weapon_List=WEAPON_L1;	}
	else if (pd_weapon <= (10-1))	{	weapon_List=WEAPON_L2;	}
	else if (pd_weapon <= (60-1))	{	weapon_List=WEAPON_L3;	}
	else if (pd_weapon <= (85-1))	{	weapon_List=WEAPON_L4;	}
	else /*if (pd_weapon<=(128-1))*/{	weapon_List=WEAPON_L5;	}	/*max==127==「128段階」*/
	//else							{	weapon_List=WEAPON_L6;	}
/*
[0	1  2  3  4][5  6  7  8	9]10 11 12 13 14 15 -- WEAPON_L1 -- WEAPON_L2 -- WEAPON_L3
16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
32
48 49 50 51 52 53 54 55 56 57 58 59][60  --- WEAPON_L4
64
80 81 82 83 84][85 -- WEAPON_L5
96
112
--
128
//
[0	1  2  3  4	5  6  7][8	9 10 11 12 13 14 15]-- WEAPON_L1 -- WEAPON_L2
16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 -- WEAPON_L3 -- WEAPON_L3
32												-- WEAPON_L3 -- WEAPON_L3
48 49 50 51 52 53 54 55 56 57 58 59[60			-- WEAPON_L3 -- WEAPON_L4
64												-- WEAPON_L4 -- WEAPON_L4
80 81 82 83 84 85 86 87][						-- WEAPON_L4 -- WEAPON_L5
96												-- WEAPON_L5 -- WEAPON_L5
112 											-- WEAPON_L5 -- WEAPON_L5
*/
	/*---------------------------------------------------------
		プレイヤーオプションチェック(必要であればオプションをセット)
	---------------------------------------------------------*/
	#if 1
	{
		if ( (pd_weapon > (35-1)))
				{	option[OPTION_C1]->flags	|= ( (SP_FLAG_VISIBLE));	}	// 可視フラグのOn(可視)
		else	{	option[OPTION_C1]->flags	&= (~(SP_FLAG_VISIBLE));	}	// 可視フラグのOFF(不可視)
		if ( (pd_weapon > (55-1)))
				{	option[OPTION_C2]->flags	|= ( (SP_FLAG_VISIBLE));	}	// 可視フラグのOn(可視)
		else	{	option[OPTION_C2]->flags	&= (~(SP_FLAG_VISIBLE));	}	// 可視フラグのOFF(不可視)
		if ( (pd_weapon > (80-1)))
				{	option[OPTION_C3]->flags	|= ( (SP_FLAG_VISIBLE));	}	// 可視フラグのOn(可視)
		else	{	option[OPTION_C3]->flags	&= (~(SP_FLAG_VISIBLE));	}	// 可視フラグのOFF(不可視)
		if ( (pd_weapon > (108-1)))
				{	option[OPTION_C4]->flags	|= ( (SP_FLAG_VISIBLE));	}	// 可視フラグのOn(可視)
		else	{	option[OPTION_C4]->flags	&= (~(SP_FLAG_VISIBLE));	}	// 可視フラグのOFF(不可視)
	}
	#endif
}

/*---------------------------------------------------------
	クリアボーナス チェック
---------------------------------------------------------*/

void player_stage_clear(void)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
			player_add_score(adjust_score_by_difficulty((
			(player_now_stage * score(1000)) +	/* ステージ x 1000pts */
			(pd->weapon_power * score(100)) +	/* パワー	x  100pts */
			(pd->graze_point)					/* グレイズ x	10pts */
		)));
	//
	pd->graze_point = 0;/* 清算して消える */
}




/*---------------------------------------------------------
	プレイヤーとアイテムのあたり判定1(アイテム専用)
	-------------------------------------------------------
		s1		プレイヤー
		tt		アイテム
---------------------------------------------------------*/
/*static*/ int effect_sound_hosi;
static void player_add_power(SPRITE *s1, SPRITE *tt, int add_power)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)s1->data;
	int prev_power; prev_power = pd->weapon_power;		/* アイテム取得する直前のパワー値 */
	pd->weapon_power += add_power;	/* パワー値、加算 */
	/* パワー値、超えてたら修正 */
	if ((127-1) < pd->weapon_power )/*127==128段階目*/
	{
		pd->weapon_power = 127;/* 最大値 */
	}
	int add_score_point;
	if (127 == pd->weapon_power/*prev_power*/)	/* maxの場合 */
	{
		pd->chain_point += add_power;	/* チェイン、加算 */
		/* チェイン、超えてたら修正 */
		if (31 < pd->chain_point)
		{
			pd->chain_point = 31;/* 最大値 */
		}
		/* (ノーミスボーナス)ウェポンボーナス得点計算 */
		add_score_point = (/*score_type*/(pd->chain_point));
	}
	else	/* maxでない場合、得点計算 */
	{
		/* (MAX時以外の) [P]は10点 */
		/* (MAX時以外の) [P中]は80点 */
		add_score_point = ((add_power/*-1*/)/*SCORE_10*/);
	}
	bonus_info_score_nodel(tt, add_score_point);/* */tt->type = SP_DELETE;/* おしまい */
	check_weapon_level(pd);/* [pd->weapon_powerが変更された場合に必ず行う後チェック] */
	/*effect_sound_number=*/ effect_sound_hosi=1;/*play_voice_auto_track(VOICE05_BONUS);*/
}

static void player_colision_check_item(SPRITE *s1/*, int mask*/)
{
	/* 自機がアイテムにあたった場合 */
	SPRITE *tt; //対象
	if (NULL != (tt=sprite_collision_check(s1,SP_GROUP_ITEMS/*mask*/)))
	{
	//	int effect_sound_number;/* 効果音 */
	//	effect_sound_number = 99;/* 効果音なし */
		//
		PLAYER_DATA *pd = (PLAYER_DATA *)s1->data;
		switch (tt->type)
		{
		case SP_ITEM_00_P001:	player_add_power(s1, tt, 1);		break;	// ウェポンアイテム(小P)
		case SP_ITEM_01_P008:	player_add_power(s1, tt, 8);		break;	// ウェポンアイテム(中P)	// [***090123		追加
		case SP_ITEM_04_P128:	player_add_power(s1, tt, 127);		break;	// ウェポンアイテム(F)		// [***090123		追加
		//
		case SP_ITEM_02_BOMB:
			//player_add_bomb(t);		// [*****本来はコメントアウトしないよ
			if (8 < pd->bombs)	{	goto add_1000pts;	}/* 既に最大値ならば、1000pts */
			pd->bombs++;
			/*effect_sound_number=*/ effect_sound_hosi=1;/*play_voice_auto_track(VOICE05_BONUS);*/
			break;

		case SP_ITEM_03_1UP:
			if (8 < pd->zanki)	{	goto add_1000pts;	}/* 既に最大値ならば、1000pts */
			pd->zanki++;
			/*effect_sound_number=*/play_voice_auto_track(VOICE06_EXTEND);
			break;

		case SP_ITEM_07_SPECIAL:
			break;
		#if 0
		/* 星点は、あたり判定なし */
		case SP_ITEM_05_HOSI:		/*not_break;*/
			tt->type = SP_DELETE;/* 星点のみ特別処理 */
			/*effect_sound_number=*/ effect_sound_hosi=1;/*play_voice_auto_track(VOICE05_BONUS);*/
			break;
		#endif
add_1000pts:
		case SP_ITEM_06_TENSU:
			{	/* SPLIT_LINE より上で取ると 1000pts. ... 下で取ると約100pts. */
				/* (大体90pts、非常に難しいが、がんばれば(消える直前の3ライン)70ptsまで可能らしい) */
				int add_score_point;
				add_score_point = (SCORE_1000); 	/* 基本点 1000pts */
				if (SPLIT_LINE < s1->y256)	/* SPLIT_LINE 未満の場合は、SPLIT_LINEまでの距離におおじて減点 */
				{
					add_score_point -= ((tt->y256-SPLIT_LINE)>>(4+8));
				}
				//player_add_score(score(1000));		// [***090123		変更
				bonus_info_score_nodel(tt, add_score_point);/* */tt->type = SP_DELETE;/* おしまい */
				/*effect_sound_number=*/ effect_sound_hosi=1;/*play_voice_auto_track(VOICE05_BONUS);*/
			}
			break;
		}
	//	if (99 != effect_sound_number)/* 効果音あれば */
	//	{
	//		play_voice_auto_track(effect_sound_number);/* 効果音鳴らす */
	//	}
	}
}


/*---------------------------------------------------------
	プレイヤーと敵のあたり判定3(敵専用)
	-------------------------------------------------------
		s1		プレイヤー
		tt		敵
---------------------------------------------------------*/

static void player_colision_check_enemy(SPRITE *s1, int player_hit_enemy_group_mask)
{
	/* 自機が敵(ボス/中型/ザコ)に体当たりされた場合 */
	/* --- 敵 --- */
	//	case SP_BOSS:		/* 自機がボス敵に体当たりされた場合 */
	//	case SP_CHUU:		/* 自機が中型敵に体当たりされた場合 */
	//	case SP_ZAKO:		/* 自機がザコ敵に体当たりされた場合 */
	SPRITE *tt; //対象
	if (NULL != (tt=sprite_collision_check(s1, player_hit_enemy_group_mask/*SP_GROUP_ENEMYS*/)))	// 敵グループに当たった場合
	{
	//	int effect_sound_number;/* 効果音 */
	//	effect_sound_number = 99;/* 効果音なし */
		//
		pd_player_status	= PLAYER_STATE_01_HIT_BOMB; 	// [***090125		追加
		pd_save_timer		= player_fix_status[BASE_HIT_BOMB_WAIT+select_player]/*0*/;
	//	pd->enemy			= t;/* あたった敵を覚えとく */
		/* あたった敵は、必ず死ぬ訳では無いので、あたった敵の体力を減らすべき */
		/**/
		/*effect_sound_number=*/play_voice_auto_track(VOICE04_SHIP_HAKAI);		// [***090127	変更先
	//	if (99 != effect_sound_number)/* 効果音あれば */
	//	{
	//		play_voice_auto_track(effect_sound_number);/* 効果音鳴らす */
	//	}
	}
}


/*---------------------------------------------------------
	プレイヤーと敵弾のあたり判定2(グレイズ専用)
	-------------------------------------------------------
		s1		プレイヤー
		tt		敵弾
---------------------------------------------------------*/
static void player_colision_check_graze(SPRITE *s1/*, int mask*/)
{
	/* --- 敵弾 --- */
	//	case SP_BULLET: 	/*not_break;*/
	//	case SP_LASER:		/*not_break;*/
	//	case SP_BIGBULLET:	/*not_break;*/
	//	case SP_BOSS02ICE:	/*not_break;*/
//
	SPRITE *tt; //対象
	if (NULL != (tt=sprite_collision_check(s1,SP_GROUP_BULLETS/*mask*/)))
	{
	//	int effect_sound_number;/* 効果音 */
	//	effect_sound_number = 99;/* 効果音なし */
		//
		/* プレイヤーに敵弾があたった場合はグレイズする */
		is_graze = 1;/*グレイズ中*/
		if (0==(tt->flags & SP_FLAG_GRAZE)) /* グレイズ済? */
		{
			tt->flags |= SP_FLAG_GRAZE;/*グレイズ済*/
			((PLAYER_DATA *)s1->data)->graze_point++;/*グレイズ数*/
			player_add_score((score(100)+score(200)*difficulty));/*score(500)*/
			/*effect_sound_number=*/play_voice_auto_track(VOICE09_GRAZE);
		}
	//	if (99 != effect_sound_number)/* 効果音あれば */
	//	{
	//		play_voice_auto_track(effect_sound_number);/* 効果音鳴らす */
	//	}
	}
}

/*---------------------------------------------------------
	プレイヤー移動
---------------------------------------------------------*/

/*---------------------------------------------------------
	プレイヤーループを抜ける処理
---------------------------------------------------------*/
void player_loop_quit(void)
{
	SPRITE *s1;
	s1 = player;
	PLAYER_DATA *pd = (PLAYER_DATA *)s1->data;
	pd->state_flag		|= STATE_FLAG_14_GAME_LOOP_QUIT;
	#if (0000)
	pd->state_flag		&= (~STATE_FLAG_08_OPTION_HIDE);	/* オプションを消す */
	#endif/*0000*/
//
	pd_save_timer		= 150;	/* 150	 120では若干短い 100 */
	pd_player_status	= PLAYER_STATE_05_GAME_OUT;/* GAME_OUT中 */
	s1->flags			&= (~(SP_FLAG_VISIBLE));		// 可視フラグのOFF(不可視)
	pd->core->flags 	&= (~(SP_FLAG_VISIBLE));		// ○消し// ○も消す
//	pd->core->alpha 	= 0;			// ○を消すために入れたけど意味無かったかもしれない。	// ゲームオーバー時の○の表示処理
//
}

/*---------------------------------------------------------
	プレイヤー復活処理
---------------------------------------------------------*/

static void player_fukkatsu(SPRITE *s1)
{
	pd_save_timer		= 40/*120*/ /*150-120*/;									// 無敵時間
	pd_player_status	= PLAYER_STATE_03_SAVE_01;	// 無敵状態？

	s1->flags			|= (SP_FLAG_VISIBLE);		// 可視フラグのON(可視)
	s1->alpha			= 80;	/*半透明*/
	s1->anim_frame		= 5;
//
	PLAYER_DATA *pd = (PLAYER_DATA *)s1->data;
	pd->core->flags 	|= (SP_FLAG_VISIBLE);		// ○表示
	pd->core->alpha 	= 80;	/*半透明*/
	pd->bombs			= player_fix_status[BASE_BOMBS+select_player]/*3*/; 		// ボム所持数の初期化
	pd->chain_point 	= 0;	// pd->chain_point(ノーミスボーナス)の初期化 // どれだけ連続(ノーミス)でwepon_upを取ったか
//
	check_weapon_level(pd);/* [pd->weapon_powerが変更された場合に必ず行う後チェック] */
}

/*---------------------------------------------------------
	プレイヤーやられ処理
---------------------------------------------------------*/

static void player_explode(SPRITE *s1)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)s1->data;
	/* アイテムを吐き出す */	//	[***090124ちょっと追加
	{
		/* GAME_OUTでもアイテム吐くべき(最後だけ吐かないのは違和感がある) */
		/* 極端に弱くて死んだ場合の救済策: 上手い人は序盤で FULLPOWER になり、死んでもボム回収などで POWER を維持するので、 */
		/* 極端に弱くては死なない。 極端に弱くて死ぬのは結構下手だと思うので、その場合 FIRE_POWER_G (8) x 6 == 48/128 (全部集めて37%) にした。 */
		item_create(s1, (((30-1) > pd->weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), 5, ITEM_MOVE_FLAG_06_RAND_XY);
		item_create(s1, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
		//
		/* レミリア 特殊能力：抱え落ちすると必ずボムが出る */
		if (REMILIA==select_player) 	/* レミの場合 */
		{
			/* 抱え落ちすると */
			if (0 != pd->bombs)
			{
				/* 使ってない分の持ってるボムを吐き出す */
				item_create(s1, SP_ITEM_02_BOMB, pd->bombs, ITEM_MOVE_FLAG_06_RAND_XY);
			}
		}
	}
//		pd->explode 		= 0;
//	if (0==pd->explode)
	{
	//	spimg=sprite_getcurrimg(t); parsys_add(spimg, 2,2, t->x,t->y, 10, 270, 10, 30, EXPLODE|DIFFSIZE, NULL); 		SDL_FreeSurface(spimg);spimg = NULL;
	//	spimg=sprite_getcurrimg(s1);parsys_add(spimg, 2,2, t->x,t->y, 10,  90, 10, 30, EXPLODE|DIFFSIZE, &pd->explode); SDL_FreeSurface(spimg);spimg = NULL;

	//	explosion_add_type(t->x256+t256(5),t->y256+t256(5),/*0,*/EXPLOSION_MINI00);
	//	t->type = SP_DELETE;/* おしまい */			/* あたった敵爆発 */
	//	pd->explode=0;
	//	pd->bonus=0;

		//play_voice_auto_track(VOICE04_SHIP_HAKAI);	// [***090127	場所を移動する。
		/* 爆発エフェクト */

		#if 0/* あたった敵は倒せない敵かもしれないので、ここで必ず消すのはまずい気がする */
		/* あたった敵爆発 */
		explosion_add_type(pd->enemy->x256+t256( 5), pd->enemy->y256+t256( 5), /*0,*/ EXPLOSION_MINI00);
		pd->enemy->type 	= SP_DELETE;/*???*/
		#endif
		/* 自分爆発 */
		explosion_add_type(s1->x256+t256( 5), s1->y256+t256( 5), /*0,*/ EXPLOSION_MINI00);
		explosion_add_type(s1->x256+t256( 5), s1->y256+t256(20), /*0,*/ EXPLOSION_MINI00);
		explosion_add_type(s1->x256+t256(20), s1->y256+t256( 5), /*0,*/ EXPLOSION_MINI00);
		explosion_add_type(s1->x256+t256(20), s1->y256+t256(20), /*0,*/ EXPLOSION_MINI00);
		/* プレイヤー位置の初期化 */
		s1->x256 = t256(GAME_WIDTH/2)-((s1->w128));
		s1->y256 = t256(220/*GAME_HEIGHT*/);/*240*/
		/*	*/
	//	pd->ex tra_type 	= PLX_NONE; 廃止
		pd->bomber_time 	= 0;			/* ←ここが無いせいで魔理沙(&チルノ)ボム撃てなくなってた */
	//	pd->state_flag		= STATE_FLAG_00_NONE;/*???*/
		pd->state_flag		&= (~(	STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM | 	/* 終わり */
									STATE_FLAG_02_BOMB_AUTO_GET_ITEM |			/* 終わり */
									STATE_FLAG_03_SCORE_AUTO_GET_ITEM));		/* 終わり */

		/* 現在オプション増えすぎて、後半重くなるバグがある。 */
		#if (0000)
		pd->state_flag		&= (~STATE_FLAG_08_OPTION_HIDE);	/* オプションを消す */
		#endif/*0000*/
		/* 残チャンス減らす */
		pd->zanki--;
		if (0 > pd->zanki)	/* 残チャンスがなければ */
		{
			player_loop_quit();
		}
		else
		{
			//
		//	if (pd->player_speed > player_fix_status[BASE_SPEED_MINIMUM+select_player]/*pd->player_speed_minimum*/) 			//player_speed_minimum以下にならないように
		//	{	pd->player_speed--;}
			pd->weapon_power -= (difficulty*4)+12;	// 武器レベルの低下
			if ( pd->weapon_power<0)	{	pd->weapon_power=0;}
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
		PLAYER_DATA *pd = (PLAYER_DATA *)s1->data;
		switch (pd_player_status)
		{
		case PLAYER_STATE_01_HIT_BOMB:/* 喰らいボム受付中 */			// [***090125		追加
			/* 死亡確定 */
				pd->state_flag |= STATE_FLAG_16_NOT_KEY_CONTROL;		/* キー入力無効(1) */
			player_explode(s1);/*プレイヤー爆発*/
			break;

		case PLAYER_STATE_03_SAVE_01:/*プレイヤー復活中１(画面外から復活地点へ点滅しながら移動中)*/
		//ちょっとした待ち時間
		//	if (s1->y256 < t256(220)/*240*/)	//ちょっと前に出てくる
			{
				pd->state_flag		&= (~STATE_FLAG_16_NOT_KEY_CONTROL);	/* キー入力有効(0) */
				pd_save_timer		= 120;
				pd_player_status	= PLAYER_STATE_04_SAVE_02;
			}
		//	else
		//	{
		//		s1->y256 -= t256(1)/*fps_fa_ctor*/;
		//	}
			break;/*??? [***090210 追加 */

		case PLAYER_STATE_04_SAVE_02:/*プレイヤー復活中２*/
			pd_player_status	= PLAYER_STATE_00_NORMAL;
			s1->alpha			= 255;
			pd->core->alpha 	= 255;
			break;

		case PLAYER_STATE_05_GAME_OUT:/* GAME_OUT中 */
			now_max_continue--;
			if (0 < now_max_continue)
			{
			//	render_continue(/*now_max_continue*/);
				psp_loop=(ST_INIT_MENU|ST_MENU_SUB_ASK_CONTINUE);//newsta te(ST_GAME_OVER,0,1);
			}
			else
			{
				psp_loop=(ST_WORK_GAME_OVER|0);//newsta te(ST_GAME_OVER,0,1);
			}
			break;
		}
	}
}


/*---------------------------------------------------------
	プレイヤー移動、メイン処理
---------------------------------------------------------*/

static void player_move(SPRITE *s1)
{
	{// static int frame_aaa;
	//	frame_aaa++;
	//	frame_aaa &= (4-1);
	//	if (0==frame_aaa)
		{
		//	if (1==effect_sound_hosi)	/* pspは0レジスタがあるので0と比較したほうが速い */
			if (0 != effect_sound_hosi)
			{
				effect_sound_hosi = 0;
				play_voice_auto_track(VOICE05_BONUS);
			}
		}
	}
	PLAYER_DATA *pd = (PLAYER_DATA *)s1->data;
	pd->state_flag &= (~STATE_FLAG_15_KEY_SHOT);	/* off */
	if (0==(pd->state_flag & STATE_FLAG_16_NOT_KEY_CONTROL))	/* キー入力有効(0) */
	{
		player_keycontrol(s1);
	}
	int player_hit_enemy_group;
	//player_hit_enemy_group = SP_GROUP_ZAKO;
	//player_hit_enemy_group = SP_GROUP_BOSS;
	if (0 != pd_save_timer)/* 特殊処理 */
	{
		player_move_other(s1);
		player_hit_enemy_group = (SP_GROUP_BOSS);	/* ボム中は、ボス / 中ザコ敵 にあたって死ぬ */
	}
	else		/*通常処理*/
	{
//	case PLAYER_STATE_00_NORMAL:
		player_colision_check_graze(s1/*,SP_GROUP_BULLETS*/);
		player_hit_enemy_group = (SP_GROUP_BOSS|SP_GROUP_ZAKO);
	}
	/* ボム中は、ボス / 中ザコ敵 にあたって死ぬ */
	/* スクリプト動作中 は、敵あたり判定はない */
	/* キー入力無効中(==復活中) は、敵あたり判定はない */
	if (0==(pd->state_flag & (STATE_FLAG_06_IS_SCRIPT|STATE_FLAG_16_NOT_KEY_CONTROL)))
	{
		/* 喰らいボム受付中 は、敵あたり判定はない */
		if (PLAYER_STATE_01_HIT_BOMB != pd_player_status)
		{
			player_colision_check_enemy(s1, player_hit_enemy_group);/*SP_GROUP_ENEMYS*/
		}
	}
	/* アイテムは常に取れる */
	player_colision_check_item(s1/*,SP_GROUP_ITEMS*/);
}


/*---------------------------------------------------------
	プレイヤー初期化()
---------------------------------------------------------*/
void player_continue_value(void)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;

	pd->zanki				= /*pd_base_zanki*/ player_fix_status[BASE_LIVES+select_player];
	pd->my_score			= score(0);

	player_fukkatsu(player);
	pd->state_flag			&= (~STATE_FLAG_14_GAME_LOOP_QUIT); 	/* 復帰 */
}


/*---------------------------------------------------------
	プレイヤー初期化
---------------------------------------------------------*/
extern int practice_mode;
extern void select_jiki_load_surface(void);
void player_init(void)
{
	select_jiki_load_surface();
//	player					= sprite_add_res( BASE_SHIP_MED_PNG+select_player );		/* [***090220 追加 */
	player					= sprite_add_bullet( JIKI_ATARI_ITEM_16 );		/* [***090220 追加 */
	player->type			= (SP_GROUP_JIKI_GET_ITEM);
	player->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	player->callback_mover	= player_move;
	#if 1
	/* とりあえず、仮対応。強制的に設定しちゃう */
	player->w128			= (t256(32/2));
	player->h128			= (t256(16/2));
	#endif
	player->x256			= (t256(GAME_WIDTH/2));
	player->y256			= (t256(GAME_HEIGHT))-((player->h128+player->h128));
//	player->anim_frame		= 5;
	PLAYER_DATA *pd;
	pd						= mmalloc(sizeof(PLAYER_DATA));
	player->data			= pd;

	pd->bomber_time 		= 0;	/*==bomb_wait*/
	pd->graze_point 		= 0;
	pd->zanki				= /*pd_base_zanki*/ player_fix_status[BASE_LIVES+select_player];
	pd->my_score			= score(0);
	pd->state_flag			= STATE_FLAG_00_NONE;
//	if (MARISA==select_player)		{	pd->state_flag		|= STATE_FLAG_04_IS_MARISA; 	}	/* 魔理沙は常に自動収集 */
	pd->boss				= NULL;
	pd->core				= player_add_core(player);		/* ○の追加 */

	weapon_List 			= 0;
	/* 練習モードの場合はフルパワーで始める(その代わりクリア後のイベントが見れない) */
	pd->weapon_power		= (0==practice_mode)?(0):(127);

//
	/* オプションを追加 */
	switch (select_player)
	{
	case YUYUKO:/*とりあえず*/
	case REIMU:/**/
	case MARISA:		option_create_re_ma_yu(player); 	break;
	case CIRNO:/*とりあえず*/
	case REMILIA:		option_create_oz_ci(player);		break;/* [***090220 追加 */
	}
//
	player_fukkatsu(player);/* オプションを追加より後 */
	now_max_continue		= /*3*/DEFAULT_MAX_CONTINUE;
}


/*---------------------------------------------------------
	プレイヤー
---------------------------------------------------------*/
static void callback_enemy_boss00_hitbyweapon_dummy(SPRITE *s, SPRITE *t/*, int angle*/)
{
}
void player_set_destoroy_boss(void)
{
//	((PLAYER_DATA *)player->data)->bo ssmode=B02_BOSS_DESTROY;
	((PLAYER_DATA *)player->data)->state_flag |= STATE_FLAG_11_IS_BOSS_DESTROY;
//
	pd_save_timer		= /*pd->bomber_time*/255 + USER_BOMOUT_WAIT;/*実質無敵時間*/
//	pd->ex tra_type 	= PLX_BOMB;
//	s1->alpha			= 80;	/*半透明*/
//	pd->core->alpha 	= 80;	/*半透明*/
	pd_player_status	= PLAYER_STATE_04_SAVE_02;

	//		data->boss_base.boss_health = 0;/* なんかバグるので追加 */
	/* コールバック登録 */
	((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon = callback_enemy_boss00_hitbyweapon_dummy; /* ダミーコールバック登録 */
	((PLAYER_DATA *)player->data)->boss->type = SP_DELETE;
	((PLAYER_DATA *)player->data)->boss->callback_mover = NULL;
}
