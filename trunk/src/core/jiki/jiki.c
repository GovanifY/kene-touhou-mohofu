
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	各プレイヤー(霊夢 ＆ 魔理沙 ＆ レミリア ＆ チルノ ＆ 幽々子)
	REIMU(A/B) MARISA(A/B/C) REMILIA CIRNO YUYUKO
	-------------------------------------------------------
---------------------------------------------------------*/

#include "jiki_local.h"

#include "kanji_system.h"

/*---------------------------------------------------------

---------------------------------------------------------*/

global PLAYER_SPEC pd;

/*---------------------------------------------------------
	共通オブジェクト
---------------------------------------------------------*/
#if 1

//global SPRITE *obj00[FIX_OBJ_00_PLAYER];
//static SPRITE *obj_maru;		/* ○ */
//static SPRITE *option[4];

global SPRITE *global_obj_boss; //obj00[FIX_OBJ_08_BOSS]

global SPRITE *obj_send1;
//global SPRITE *obj_effect[8]; /* [□]（スペルカードのグラフィック） */

//#define TEISOKU_EFFECT_00_OBJ (6)
//#define TEISOKU_EFFECT_01_OBJ (7)

#endif


	/*---------------------------------------------------------
		各プレイヤーの性能差
	---------------------------------------------------------*/
/* 霊夢 特殊能力：喰らいボムの受付時間が長い */
/* チルノ 特殊能力：⑨ */

/*static*/global const u8 player_fix_status[BASE_MAX] =
{/* REIMU(A/B) MARISA(A/B)	REMILIA YUYUKO CIRNO(A/Q) */
//	/* BASE_OPT_SHOT_INTERVAL オプションショットの更新間隔 / option shot interval. */
//	  14,  13,	11,   8,	8,	31,   8,   9,	 /* WEAPON_L0 */
//	  12,  11,	10,   8,	8,	28,   8,   9,	 /* WEAPON_L1 */
//	  10,	9,	 9,   8,	8,	25,   8,   9,	 /* WEAPON_L2 */
//	   8,	7,	 8,   8,	8,	22,   8,   9,	 /* WEAPON_L3 */
//	   6,	5,	 7,   8,	8,	19,   8,   9,	 /* WEAPON_L4 */
//	   4,	3,	 6,   8,	8,	16,   8,   9,	 /* WEAPON_L5 */
//
	  14,  13,	11,   4,	8,	31,   8,   9,	 /* WEAPON_L0 */
	  12,  11,	10,   4,	8,	28,   8,   9,	 /* WEAPON_L1 */
	  10,	9,	 9,   4,	8,	25,   8,   9,	 /* WEAPON_L2 */
	   8,	7,	 8,   4,	8,	22,   8,   9,	 /* WEAPON_L3 */
	   6,	5,	 7,   4,	8,	19,   7,   8,	 /* WEAPON_L4 */
	   4,	3,	 6,   3,	8,	16,   5,   6,	 /* WEAPON_L5 */
//	 /* BASE_SPEED_ANIME	   プレイヤーのアニメーション速度 */
	   8,	8,	 2,   2,	3,	 4,   1,   1,	 /* BASE_SPEED_ANIME	   プレイヤーのアニメーション速度 */
//	  16,  16,	 8,   8,	4,	12,   9,   9,	 /* BASE_HIT_BOMB_DEC_WAIT	   喰らいボムの受付時間 / hit_bomb_wait. */
//	  16-16,  16-16,	 16-8,	 16-8,	16-4,	16-12,	 16-9,	 16-9,	 /* BASE_HIT_BOMB_DEC_WAIT 喰らいボムの受け付け猶予減少時間 / hit_bomb_wait. */
//	  0,  0,	 8,   8,	12,  4,   7,   7,	 /* BASE_HIT_BOMB_DEC_WAIT 喰らいボムの受け付け猶予減少時間 / hit_bomb_wait.(値が小さい程、受け付け時間が長い) */
	  0,  0,	 8,   8,	12,  4,   3,   2,	 /* BASE_HIT_BOMB_DEC_WAIT 喰らいボムの受け付け猶予減少時間 / hit_bomb_wait.(値が小さい程、受け付け時間が長い) */
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
	   0,	0,	 0,   0,	0,	 1,   1,   1,	 /* BASE_OPT_ANIM_TYPE	   0==回転, 1==パターン */
	   2,	2,	 2,   2,	2,	12,   3,   3,	 /* BASE_OPT_ANIM_SPEED    回転/パターン速度 */
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
	プレイヤー武器のあたり判定
---------------------------------------------------------*/

/*static*/global void player_weapon_colision_check(SPRITE *shot, int erase_shot_type)
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
			tekidan_obj->jyumyou = JYUMYOU_NASI;	/* 敵弾が消滅 */
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
				shot->jyumyou = JYUMYOU_NASI;		/* 自弾が消滅 */
				/* ショットが敵に当たった場合、打ち込み点を加算。(無敵キャラでも撃ち込み点で稼げる) */
				player_dummy_add_score(score(10));
			}
		}
	}
}


/*---------------------------------------------------------
	敵を探す子関数
---------------------------------------------------------*/

/*static*/global SPRITE *search_enemy_by_sprite(void)
{
	int ii;
	for (ii=0; ii<SPRITE_333POOL_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *h;
		h = &obj33[ii];
	//
		if (
			#if 1
		//	(SP_DELETE != h->type ) && /* 削除済みは飛ばす */
			(JYUMYOU_NASI <= h->jyumyou) && /* 削除済みは飛ばす */
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
				#if 1/*Gu(中心座標)*/
				if ((h->cx256 > 0) && (h->cx256 < t256(GAME_WIDTH ) ) &&
					(h->cy256 > 0) && (h->cy256 < t256(GAME_HEIGHT) ))	/* 表示画面内の敵 */
				#endif
				{
					return (h);/*見つけたよ*/
				}
			}
		}
	}
	return (&obj00[FIX_OBJ_00_PLAYER]);/*見つからなかった*/
}


/*---------------------------------------------------------
	スペカ発動時の立ち絵
---------------------------------------------------------*/

static void player_move_tachie(SPRITE *src) 	/* [***100113 追加 */
{
	if ((128-1) < pd.bomber_time)/* 残り時間が 128 以上なら、移動して待機 */
	{
		/* 画面外から出現、停止。しばし待つ。 */
		if ( t256(100) > src->cx256 )
		{
			src->cx256 += t256(5.0);
		}
	}
	else
	if ((1/*-1*/) < pd.bomber_time)/* 拡大 */
	{
		/* (24+(1))  、ここでの pd.bomber_time は、最大0x7fなので。 */
		src->color32		= (pd.bomber_time<<(24+(1)))|0x00ffffff;
		#if (1==USE_ZOOM_XY)
		src->m_zoom_x256	+= (10);
		src->m_zoom_y256	+= (10);
		#else
		src->m_zoom_xy256	+= (10);
		#endif
	}
	else
	{
		src->jyumyou = JYUMYOU_NASI;	/* 時間制限でおしまい */
	}
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

/*static*/ int weapon_level_offset; /* 装備した武器レベル(番号)の８倍にプレイヤー値を足した値をオフセットとして保持 */
/*
	weapon_level_offset == player番号 + (武器レベル x 8)
	weapon_level_offset =  select_player + (weapon_level<<3);
*/


/*---------------------------------------------------------
	プレイヤー、オプションの移動(霊夢、魔理沙、仮幽々子)
---------------------------------------------------------*/

//#define BASE_OPT_SHOT_ANIME	(PLAYERS8*3)
//	 12,  12,	6,	 6,    3,  yuuyko(12),	 CIRNO(3),	 CIRNO(3),	/* オプションショットのアニメーション速度 */

/*---------------------------------------------------------
	プレイヤー、オプションの定義
---------------------------------------------------------*/
/*static*/extern void player_move_option(SPRITE *src);

/*---new add-----*/


/*---------------------------------------------------------
	プレイヤー立ち絵生成
	(幽々子)[幽々子ボムの扇本体]
---------------------------------------------------------*/

static void player_create_bomber_tachie(void/*SPRITE *src*/)
{
	SPRITE *h;
	h					= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */	/*(?)*/
	{
		h->m_Hit256R			= JIKI_ATARI_ITEM_80;
//		h->flags				|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
		h->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);
		h->callback_mover		= player_move_tachie;
		h->type 				= (JIKI_BOMBER_00|SP_GROUP_SHOT_ZAKO);	/*SP_GROUP_JIKI_GET_ITEM*/ /*JI KI_SHOT_00*/	/* ボスの直接攻撃は禁止 */	/*(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) ボスに無効*/
		/* 立ち絵初期位置 */
		h->cx256				= -t256(150);	// (200)	/*0*/  /*(src->cx256) */
		h->cy256				=  t256(150);	// (100)	/*GAME_HEIGHT+1*/ /*(src->cy256)*/
	//
		/* 描画用角度 */
	//	h->rotationCCW1024		= (256);
		h->rotationCCW1024		= (0);
	//
	/*???*/ 	h->base_weapon_strength 	= (1/*8*1*/);		/* 扇本体 の強さ */
	}
}






enum /*_player_state_*/
{
	//...
	LIMIT_m255_CONTINUE 		= (-255),
	LIMIT_m127_MIN				= (-127),
	LIMIT_m16_GAME_OUT			= (-16),
	LIMIT_m1_KURAI_BOMB_UKETUKE = (-1),
	LIMIT_0_ZERO	= (0),
	LIMIT_1_ONE 	= (1),
	LIMIT_128		= (128),
	LIMIT_256		= (256),
	LIMIT_512		= (512),
	LIMIT_65535 	= (65535),
};

/* 0==特殊イベント発生していない。 */
static signed int pds_status_timer; /* 特殊イベント発生中タイマー */
//static int pd.special_ivent_type; /* 特殊イベントの種類 */


extern void reimu_create_bomber_kekkai_parrent(SPRITE *src);				/* 霊夢専用 ボムの親 */
extern void marisa_create_bomber_master_spark_parrent(SPRITE *src); 		/* 魔理沙 B専用 ボムの親 */
extern void yuyuko_create_bomber_gyastry_dream_parrent(SPRITE *src);
extern void marisa_create_bomber_star_dust_levarie_parrent(SPRITE *src);	/* 魔理沙 A専用 低速ボムの親 */
extern void remilia_create_bomber_cross_shot_parrent(SPRITE *src);			/* レミリアボムの親 */
extern void yuyuko_create_bomber_meifu_parrent(SPRITE *src);				/* 幽々子専用 低速ボムの親 */
extern void marisa_create_bomber_homing_parrent(SPRITE *src);				/* 低速ボムの親 */
extern void reimu_create_bomber_homing_shot_parrent(SPRITE *src);			/* 霊夢専用 低速ボムの親 */
/*---------------------------------------------------------
	プレイヤーキー操作
---------------------------------------------------------*/

//static int /*bomb_wait*/d->bomber_time;		/* ボムの有効時間 */	//次のボムを出せるまでの時間

extern /*global*/short my_analog_x; /* アナログ量、補正済み */
extern /*global*/short my_analog_y; /* アナログ量、補正済み */
extern void set_bg_alpha(int set_bg_alpha);
extern void register_main_shot(SPRITE *s1);


//enum /*_player_state_*/
//{
//	PLAYER_STATE_00_NORMAL,
//	PLAYER_STATE_01_KURAI_BOMB_UKETUKE, 	/* [***090125		追加:PLAYER_STATE_01_KURAI_BOMB_UKETUKE */
//	PLAYER_STATE_02_SAVE_01,
//	PLAYER_STATE_03_SAVE_02,
//	PLAYER_STATE_04_GAME_OUT,
//};

static void player_keycontrol(SPRITE *s1)
{
	/*---------------------------------------------------------
		プレイヤーボム関連処理
	---------------------------------------------------------*/
//	if (STATE_FLAG_06_IS_SCRIPT==(pd.state_flag & STATE_FLAG_06_IS_SCRIPT))
//	{
//		;	/* 会話イベント中はボム関連の処理はしない */
//	}
//	else
		/* シナリオ時には誘導弾を追加しない */
	if (0==(pd.state_flag & STATE_FLAG_06_IS_SCRIPT))
	{
	//	if (d->bomber_time>0)
		if (0 < /*bomb_wait*/pd.bomber_time)		/* ボムウェイト処理 */
		{
			//	d->bomber_time-=fps_fa_ctor;
			/*bomb_wait*/pd.bomber_time--/*-=fps_fa_ctor*/;/**/
			if (2 > /*bomb_wait*/pd.bomber_time)
			{
				set_bg_alpha(255);/* 画面を明るくする */
		//	}
		//	else
		//	if (/*bomb_wait*/pd.bomber_time < 190)
		//	{
				pd.state_flag &= (~STATE_FLAG_02_BOMB_AUTO_GET_ITEM);	/* ボムによる自動収集は終わり */
			}
		}
		else
		{
			pd.bomber_time = 0;
		//}
		//if (/*bomb_wait*/d->bomber_time<=0)
		//{
			if (my_pad & PSP_KEY_BOMB_CANCEL)
			{
				if (0 < pd.bombs)
				{
					pd.bombs--;
					pd.used_bomber++;	/* ボム使用回数 */
					voice_play(VOICE07_BOMB, TRACK02_ALEART_IVENT);/*テキトー*/
					set_bg_alpha(50);/* 画面を暗くする */
//					set_bg_alpha(100);/* 画面を暗くする */
//					set_bg_alpha(127);/* 画面を暗くする */
					/* 立ち絵 */
					player_create_bomber_tachie(/*obj00[FIX_OBJ_00_PLAYER]*/);
					//
					static /*const*/ void (*regist_call_table[/*16*/(PLAYERS8*2)])(SPRITE *src) =
					{
						reimu_create_bomber_homing_shot_parrent,				reimu_create_bomber_kekkai_parrent, 		/* 霊夢 A */
						reimu_create_bomber_kekkai_parrent, 					marisa_create_bomber_homing_parrent,		/* 霊夢 B */
						marisa_create_bomber_star_dust_levarie_parrent, 		marisa_create_bomber_homing_parrent,		/* 魔理沙 A */
						marisa_create_bomber_master_spark_parrent,				marisa_create_bomber_homing_parrent,		/* 魔理沙 B */
						remilia_create_bomber_cross_shot_parrent,				marisa_create_bomber_homing_parrent,		/* レミリア */
						yuyuko_create_bomber_gyastry_dream_parrent, 			yuyuko_create_bomber_meifu_parrent, 		/* 幽々子 */
						marisa_create_bomber_star_dust_levarie_parrent, 		reimu_create_bomber_homing_shot_parrent,	/* チルノ A */
						yuyuko_create_bomber_gyastry_dream_parrent, 			marisa_create_bomber_homing_parrent,		/* チルノ Q */
					};
						int index_aaa;
						index_aaa = ((select_player)+(select_player)+((my_pad & PSP_KEY_SLOW)?1:0));
					{
						(regist_call_table[(index_aaa)])(&obj00[FIX_OBJ_00_PLAYER]);
					}
					s1->color32 				= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	/* 半透明 */
					{
						SPRITE *zzz_obj_maru;
						zzz_obj_maru = &obj00[FIX_OBJ_01_JIKI_MARU];
						zzz_obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	/* 半透明 */
					}
//					if (PLAYER_STATE_01_KURAI_BOMB_UKETUKE==pd.special_ivent_type)/* 喰らいボム受付中 */
					if (LIMIT_0_ZERO > pds_status_timer)/* 喰らいボム受付中 */
					{
						pd.use_kurai_bomb++;/* 喰らいボム成功 */
					}
//					pds_status_timer = LIMIT_128;// pd.special_ivent_type	= PLAYER_STATE_03_SAVE_02;
					pd.bomber_time		= (PD_BOMBER_JYUMYOU180);//pd.bomber_time_table[(index_aaa)];
					//	#define USER_BOMOUT_WAIT (30)
					pds_status_timer	= pd.bomber_time + USER_BOMOUT_WAIT;/*実質無敵時間*/
					/*	実質無敵時間 == (設定無敵時間 + USER_BOMOUT_WAIT)。 	実質無敵時間 == 設定無敵時間 だと、わかりにくいので若干の余裕が必要。 */
				//
					kanji_window_clear_line(0); 	/* 漢字ウィンドウの1行目(==0)の内容を消す。 */
					set_cursor(0, 0);				/* カーソルを1行目(==0)へ移動 */
					{
						msg_time = pd.bomber_time;	/*(60*5)*/
						typedef struct
						{
							const char *spell_str_name; 	/* スペカ名称 */
						} aaa_RESOURCE;
						static const aaa_RESOURCE my_aaa_resource[(PLAYERS8*2)] =
						{
//	 "feeddccbbaa99887766554433221100",'\0' 	 "feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{"霊符「夢想封印」" 				"\n"},	{"祓符「夢想境界」" 				"\n"},//霊符「夢想結界・円」
	{"夢符「夢想結界」" 				"\n"},	{"降符「夢想調伏」" 				"\n"},//夢符「夢想結界・斬」
	{"魔符「スターダストレヴァリエ」"	"\n"},	{"閃符「未作成攻撃」"				"\n"},
	{"恋符「マスタースパーク」" 		"\n"},	{"虹符「未作成攻撃」"				"\n"},
	{"紅符「不夜城ブレッド」"			"\n"},	{"民盟「アクマクロス」" 			"\n"},
	{"死符「ギャストリドリーム」"		"\n"},	{"幻蝶「冥土の行灯」"				"\n"},//冥符「未作成攻撃」	/* dream cast. ghastly dream(妖々夢). ghastly dream(霊夢). */
	{"霰符「アイシクルストライク」" 	"\n"},	{"氷符「オンザロック」" 			"\n"},	/* 霰(あられ): 雨が凍ったり、溶けかけた雪が再び凍って降る。 */
	{"最強「アタイの拳」"				"\n"},	{"最雹「九文キック」"				"\n"},	/* 霙符 霜踏 霙(みぞれ): 雨と雪が混ざって降る。 */
						};
						print_kanji000(my_aaa_resource[index_aaa].spell_str_name, /*int color_type*/7, /*int wait*/0);
					}
				}
			}
		}
	}


	#if 0/* 今まで検討したボム時間(主なもの) */
//	/* 低速ボム */			pd.bomber_time = 200/*320*/ /*400*/ /*800*/;			/* ボムの有効時間(設定無敵時間)[※１仕様が変わったので半分にした] */
//	/* 霊夢 */				pd.bomber_time = 180/*200*/;							/* ボムの有効時間(設定無敵時間) */	/* こちらはボス有効(攻撃型) */
//	/* 魔理沙	チルノ */	pd.bomber_time = 100/*32*/ /*100*/; 					/* ボムの有効時間(設定無敵時間) */
//	/* 扇 有効時間 */		pd.bomber_time = 255/*255==((16*4*2*2)-1)*/;			/* ボムの有効時間(設定無敵時間) */
//	レミリアボム			pd.bomber_time = 0x7e	/* 255==0xff==8発	254==0xfe==7発==0xbf=192 4発==0x7f 150==0x96*/;
	static const u8 pd.bomber_time_table[/*16*/(PLAYERS8*2)] =
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
//
//	if (PLAYER_STATE_01_KURAI_BOMB_UKETUKE==pd.special_ivent_type) return;/* 喰らいボム受付中 */
	if (LIMIT_0_ZERO > pds_status_timer)	{	return; 	}	/* 喰らいボム受付中 */
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
	/* 高速モード(通常時) */								/* 高速モード(ボム発動時) */
	#define SPEED_HIGH_REIMU_A		(0x02f1)	/* t256(2.94) 2.94140625		  t256(3.0), */
	#define SPEED_HIGH_REIMU_B		(0x02f1)	/* t256(2.94) 2.94140625		  t256(3.0), */
	#define SPEED_HIGH_MARISA_A 	(0x03a3)	/* t256(3.64) 3.63671875		  t256(2.0), */
	#define SPEED_HIGH_MARISA_B 	(0x03a3)	/* t256(3.64) 3.63671875		  t256(2.0), */
	#define SPEED_HIGH_REMILIA		(0x0347)	/* t256(3.28) 3.27734375		  t256(5.0), */
	#define SPEED_HIGH_YUYUKO		(0x02c7)	/* t256(2.78) 2.77734375		  t256(5.0), */
	#define SPEED_HIGH_CIRNO_A		(0x0373)	/* t256(3.45) 3.44921875		  t256(4.5), */ 			/* ⑨だから低速の方が速い */
	#define SPEED_HIGH_CIRNO_Q		(0x0373)	/* t256(3.45) 3.44921875		  t256(4.5), */ 			/* ⑨だから低速の方が速い */
														/* 低速モード(通常時) */								/* 低速モード(ボム発動時) */
	#define SPEED_LOW_REIMU_A		(0x0178)	/* t256(1.47) 1.46875000 t256(2.0) t256(2.0), */
	#define SPEED_LOW_REIMU_B		(0x0178)	/* t256(1.47) 1.46875000 t256(2.0) t256(2.0), */
	#define SPEED_LOW_MARISA_A		(0x0233)	/* t256(2.20) 2.19921875 t256(2.5) t256(2.0), */
	#define SPEED_LOW_MARISA_B		(0x0233)	/* t256(2.20) 2.19921875 t256(2.5) t256(2.0), */
	#define SPEED_LOW_REMILIA		(0x01d6)	/* t256(1.84) 1.83593750 t256(2.5) t256(4.0), */
	#define SPEED_LOW_YUYUKO		(0x0161)	/* t256(1.38) 1.37890625 t256(2.0) t256(4.5), */
	#define SPEED_LOW_CIRNO_A		(0x0269)	/* t256(2.41) 2.41015625 t256(7.0) t256(5.0), */ /* 0x0700 ⑨だから低速の方が速い 0x0400==文高速時==幻想郷最速？ */
	#define SPEED_LOW_CIRNO_Q		(0x0700)	/* t256(2.41) 2.41015625 t256(7.0) t256(5.0), */ /* 0x0700 ⑨だから低速の方が速い 0x0400==文高速時==幻想郷最速？ */

	#define PLAYER_WIDTH			(50)	/*	固定サイズ */
	#define PLAYER_HEIGHT			(50)	/*	固定サイズ */

	static const signed /*int*/short player_speed256[(PLAYERS8*4)] =
	{	/* 高速モード(通常時) */								/* 高速モード(ボム発動時) */
		SPEED_HIGH_REIMU_A, 		/* REIMU_A */
		SPEED_HIGH_REIMU_B, 		/* REIMU_B */
		SPEED_HIGH_MARISA_A,		/* MARISA_A */
		SPEED_HIGH_MARISA_B,		/* MARISA_B */
		SPEED_HIGH_REMILIA, 		/* REMILIA */
		SPEED_HIGH_YUYUKO,			/* YUYUKO */
		SPEED_HIGH_CIRNO_A, 		/* CIRNO_A */
		SPEED_HIGH_CIRNO_Q, 		/* CIRNO_Q */
		/* 低速モード(通常時) */								/* 低速モード(ボム発動時) */
		SPEED_LOW_REIMU_A,			/* REIMU_A */
		SPEED_LOW_REIMU_B,			/* REIMU_B */
		SPEED_LOW_MARISA_A, 		/* MARISA_A */
		SPEED_LOW_MARISA_B, 		/* MARISA_B */
		SPEED_LOW_REMILIA,			/* REMILIA */
		SPEED_LOW_YUYUKO,			/* YUYUKO */
		SPEED_LOW_CIRNO_A,			/* CIRNO_A */
		SPEED_LOW_CIRNO_Q,			/* CIRNO_Q */
	#if 0/*[マスタースパーク中は、速度半分。]*/
		/* 高速モード(通常時) */								/* 高速モード(ボム発動時) */
		SPEED_HIGH_REIMU_A, 		/* REIMU_A */
		SPEED_HIGH_REIMU_B, 		/* REIMU_B */
		SPEED_HIGH_MARISA_A,		/* MARISA_A */
		(SPEED_HIGH_MARISA_B>>1),	/* MARISA_B */
		SPEED_HIGH_REMILIA, 		/* REMILIA */
		SPEED_HIGH_YUYUKO,			/* YUYUKO */
		SPEED_HIGH_CIRNO_A, 		/* CIRNO_A */
		SPEED_HIGH_CIRNO_Q, 		/* CIRNO_Q */
		/* 低速モード(通常時) */								/* 低速モード(ボム発動時) */
		SPEED_LOW_REIMU_A,			/* REIMU_A */
		SPEED_LOW_REIMU_B,			/* REIMU_B */
		SPEED_LOW_MARISA_A, 		/* MARISA_A */
		(SPEED_LOW_MARISA_B>>1),	/* MARISA_B */
		SPEED_LOW_REMILIA,			/* REMILIA */
		SPEED_LOW_YUYUKO,			/* YUYUKO */
		SPEED_LOW_CIRNO_A,			/* CIRNO_A */
		SPEED_LOW_CIRNO_Q,			/* CIRNO_Q */
	#endif
	};
	/* 自機速度を決める。 */
	unsigned int is_slow;
	is_slow = (my_pad & PSP_KEY_SLOW);
	/*const*/ signed int my_speed = player_speed256[select_player + ((is_slow)?(PLAYERS8):(0))];
	/* 移動量を決める。(移動量 = 自機速度 x アナログキー、デジタルの場合は予めアナログキー移動量に変換してある) */
	#if 1/*[マスタースパーク中は、速度半分。]*/
	if (
		(MARISA_B==select_player) &&	/* 魔理沙B(恋符) */
		(0<pd.bomber_time)				/* マスタースパーク中 */
	)
		#if 0
	{
		;/* マスタースパーク中はショットが撃てないらしい */
	}
	if (is_master_spark)
		#endif
	{	(my_speed >>= 1);	}
	#endif
	short	aaa_my_analog_x = (((my_speed)*(my_analog_x))>>8);
	short	aaa_my_analog_y = (((my_speed)*(my_analog_y))>>8);
	/* 斜めを考慮して移動する。 */
	s1->cx256 += ((((signed int)(jiki_move_length[((my_pad&0xf0)>>4)][0]))*(aaa_my_analog_x))>>8);	/*fps_factor*/
	s1->cy256 += ((((signed int)(jiki_move_length[((my_pad&0xf0)>>4)][1]))*(aaa_my_analog_y))>>8);	/*fps_factor*/
	/* 画面外に、はみだしたら修正。 */
	#if 1/*Gu(中心座標)*/
//		 if (s1->cx256 < t256(0))				{	s1->cx256 = t256(0);				}/* 左チェック */
//	else if (s1->cx256 > t256(GAME_WIDTH)  )	{	s1->cx256 = t256(GAME_WIDTH);		}/* 右チェック */
//		 if (s1->cy256 < t256(0))				{	s1->cy256 = t256(0);				}/* 上チェック */
//	else if (s1->cy256 > t256(GAME_HEIGHT) )	{	s1->cy256 = t256(GAME_HEIGHT);		}/* 下チェック */
	/* psp は、弾幕するにはいくらなんでも上下が狭すぎる。上下が喰み出すのはゲーム性と、ゲーム画面設計の両方の問題で意図的である。
		ゲーム画面設計上は左右は喰み出しても良いのだが、模倣風では喰み出さない事にする。
		これは左右の壁で反射する弾処理が、喰み出し可だと遅くなりそうなので都合上の問題。
		咲夜さんの上避けは何時になったら出来るのやら。 */
		 if (s1->cx256 < t256(PLAYER_WIDTH/2))					{	s1->cx256 = t256(PLAYER_WIDTH/2);					}/* 左チェック(喰み出さない) */
	else if (s1->cx256 > t256(GAME_WIDTH-(PLAYER_WIDTH/2))	)	{	s1->cx256 = t256(GAME_WIDTH-(PLAYER_WIDTH/2));		}/* 右チェック(喰み出さない) */
//		 if (s1->cy256 < t256(PLAYER_HEIGHT/2)) 				{	s1->cy256 = t256((PLAYER_HEIGHT/2));				}/* 上チェック(喰み出さない) */
//	else if (s1->cy256 > t256(GAME_HEIGHT-(PLAYER_HEIGHT/2)) )	{	s1->cy256 = t256(GAME_HEIGHT-(PLAYER_HEIGHT/2));	}/* 下チェック(喰み出さない) */
		 if (s1->cy256 < t256(0))								{	s1->cy256 = t256(0);								}/* 上チェック(上が喰み出すか、喰み出さないかまだ仕様が決まってない) */
	else if (s1->cy256 > t256(GAME_HEIGHT)) 					{	s1->cy256 = t256(GAME_HEIGHT);						}/* 下チェック(下が喰み出すのは意図的) */
	#endif
	/* コア移動 */
	{
		SPRITE *s2;
	//	s2 = obj_maru;
		s2 = &obj00[FIX_OBJ_01_JIKI_MARU];
		#if 1/*Gu(中心座標)*/
		s2->cx256 = s1->cx256;	/* 中心座標なのでコアオフセットはない */
		s2->cy256 = s1->cy256;	/* 中心座標なのでコアオフセットはない */
		#endif
//		if (is_slow)	{	s2->type		&= (~(0x08));/*s2->flags		|= ( (SP_FLAG_VISIBLE));*/	}	/* ○、表示 */
//		else			{	s2->type		|= ( (0x08));/*s2->flags		&= (~(SP_FLAG_VISIBLE));*/	}	/* ○、非表示 */
		/* 低速モード用エフェクトの表示 */
		{
			SPRITE *obj_tei_soku;
//			obj_tei_soku					= obj_effect[TEISOKU_EFFECT_00_OBJ];
			obj_tei_soku					= &obj00[FIX_OBJ_15_JIKI_TEISOKU_EFFECT];
			if (is_slow)
			{
//				SPRITE *zzz_obj_maru;
//				zzz_obj_maru				= &obj00[FIX_OBJ_01_JIKI_MARU];
//				obj_tei_soku->type			= (SPELL_SQUERE_);
				obj_tei_soku->type			= (SPELL_SQUERE_);
//				obj_tei_soku->flags 		|= (/*SP_FLAG_COLISION_CHECK*/SP_FLAG_VISIBLE/*|SP_FLAG_VISIBLE*/);
//				obj_tei_soku->flags 		|= ( (SP_FLAG_VISIBLE));		/* 低速モード用エフェクト、表示 */
				obj_tei_soku->cx256 		= (s1->cx256);	/* 中心座標なのでコアオフセットはない */
				obj_tei_soku->cy256 		= (s1->cy256);	/* 中心座標なのでコアオフセットはない */
				#if 1
				/* 描画用角度(下が0度で左回り(反時計回り)) */
				obj_tei_soku->rotationCCW1024 += (10);
				#endif
			}
			else
			{
				obj_tei_soku->type			= (SP_DUMMY_MUTEKI);/*(S P_GROUP_ETC_DUMMY_REMILIA);*/
			//	obj_tei_soku->type			= (CORE_HIDE_10_REIMU_A);
//				obj_tei_soku->flags 		&= (~(SP_FLAG_VISIBLE));		/* 低速モード用エフェクト、非表示 */
			//	obj_tei_soku->type			= (SPELL_SQUERE_);
//				obj_tei_soku->type			= FIX_OBJ_OFF;	/* 固定objの場合の単なるOFF */	/* まだ固定objになってない */
			}
		}
	}
	/* MAX時のアイテム自動収集 */
	if (my_pad & PSP_KEY_UP/*PSP_CTRL_UP*/) 	/* 註：斜め上でも回収可能 */ /*&& (s1->y>0)*/
	{
		#if 1
			/* 全キャラ: アイテム上部収集がいつでも可能 */
		#else
		if ((pd.weapon_power==MAX_POWER_IS_128) 	// 128[***090123 変更 /*max==MAX_POWER_IS_128==「129段階」*/
			/* 魔理沙 特殊能力：アイテム上部収集がいつでも可能 */
			|| (MARISA_A==select_player)
			|| (MARISA_B==select_player)
		//	|| (MARISA_C==select_player)
		)	/* 魔理沙は常に上部自動収集が可能 */
		#endif
		{
			if (s1->cy256 < PLAYER_SPLIT_LINE256/*t256(50)*/)/* [FULLパワー時のアイテム引き寄せライン] */
			{
				pd.state_flag |= STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM; /* 上部自動収集可能 */
			}
		}
	}
	else if (my_pad & PSP_KEY_DOWN/*PSP_CTRL_DOWN*/)	/* 註：斜め下でもやめる */ /*&& (s1->y<screen->h*-s1->h)*/
	{
		//if (pd.state_flag & (STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM /*| STATE_FLAG_02_BOMB_AUTO_GET_ITEM*/) )/*???たぶん*/
		{
			pd.state_flag &= (~STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM);	/* 上部自動収集不可 */
		}
	}
//
		/*---------------------------------------------------------
			プレイヤーショット関連処理
		---------------------------------------------------------*/
	/* シナリオスクリプトモードではショットボタン入力無効 */
	if (0==(pd.state_flag & STATE_FLAG_06_IS_SCRIPT))
	{
		if (my_pad & PSP_KEY_SHOT_OK)
		{
			pd.state_flag |= STATE_FLAG_15_KEY_SHOT;	/* on */
			register_main_shot(s1);
		}
	}
	#if 0/*デバッグ用*/
	p d_my_score	= (weapon_level_offset);/*デバッグ*/
	p d_graze_point = (weapon_level_offset);/*デバッグ*/
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
				auto_aniime &= 3;
					 if (4==auto_anime_frame)	{	now_anime_frame = (auto_anime_frame + auto_aniime); }	/* 中心1-4 */
				else if (4<auto_anime_frame)	{	now_anime_frame = (auto_anime_frame + (3) + (8) );	}	/* 右側1-4 */	/* +(8) == オプションインターリーブ */
				else							{	now_anime_frame = (auto_anime_frame );				}	/* 左側1-4 */
				s1->type = (JIKI_PLAYER_00_LEFT)+(now_anime_frame);
			}
			#else
			{
				static int auto_aniime;
				auto_aniime++;
				auto_aniime &= 3;
				s1->type = (JIKI_PLAYER_00_LEFT)+(auto_anime_frame );
					 if (4==auto_anime_frame)	{	s1->type += ( auto_aniime); }	/* 中心1-4 */
				else if (4<auto_anime_frame)	{	s1->type += ( 3+8); 		}	/* 右側1-4 */
			//	else							{								}	/* 左側1-4 */
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
/* [pd.weapon_powerが変更された場合に必ず行う後チェック] */
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
	u8 pds_weapon;	pds_weapon = pd.weapon_power;
		 if (pds_weapon < ( 8)) 	{	weapon_level_offset = (WEAPON_L0<<3);	}	/* WEAPON_L0(P000-P008) */	/* 6*/
	else if (pds_weapon < ( 16))	{	weapon_level_offset = (WEAPON_L1<<3);	}	/* WEAPON_L1(P008-P015) */	/*11*/
	else if (pds_weapon < ( 32))	{	weapon_level_offset = (WEAPON_L2<<3);	}	/* WEAPON_L2(P016-P031) */	/*61*/
	else if (pds_weapon < ( 64))	{	weapon_level_offset = (WEAPON_L3<<3);	}	/* WEAPON_L3(P032-P063) */	/*86*/
	else if (pds_weapon < (128))	{	weapon_level_offset = (WEAPON_L4<<3);	}	/* WEAPON_L4(P064-P127) */
	else							{	weapon_level_offset = (WEAPON_L5<<3);	}	/* WEAPON_L5(P128)		*/	/*max==P128==「129段階」*/
	weapon_level_offset += select_player;
	#endif
	#if 1//1968181
	u8 pds_weapon;	pds_weapon = pd.weapon_power;
	/* 0000 0001 1111 1--- */
	/* 紅、調べたら丁度2倍っぽい。 */
	{	/* 丁度2倍だとこんな感じで簡略化できる。 */
		weapon_level_offset = (select_player/*0*/);
		weapon_level_offset += (WEAPON_L5<<3);
		int jj;
		for (jj=(0); jj<(5); jj++)
		{
			if (pds_weapon & ((0x80)>>jj))
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
			SPRITE *zzz_obj_option;
			zzz_obj_option = &obj00[FIX_OBJ_04_JIKI_OPTION0+jj];
			if ( (pds_weapon > ((0x08)<<jj)/*jjj_tbl[jj]*/))	/* 丁度2倍だとこんな感じで簡略化できる。 */
					{	zzz_obj_option->flags	|= ( (SP_FLAG_OPTION_VISIBLE)); 	}	// 可視フラグのOn(可視)
			else	{	zzz_obj_option->flags	&= (~(SP_FLAG_OPTION_VISIBLE)); 	zzz_obj_option->cy256=t256(GAME_HEIGHT+1+16);	}	// 可視フラグのOFF(不可視)
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
static void player_add_power_type(SPRITE *src, SPRITE *ttt)
{
	{	const u8 aaa[4] = { 1, 8, 127, 1/*チルノ用*/};
		const int add_power = aaa[/*((ttt_type))*/((NULL==ttt)?(0/*3 チルノ用*/):((ttt->type)&0x03))];
		int previous_power; previous_power = pd.weapon_power;		/* アイテム取得する直前のパワー値 */
		pd.weapon_power += add_power;	/* パワー値、加算 */
		/* パワー値、超えてたら修正 */
		if ((MAX_POWER_IS_128-1) < pd.weapon_power )/*127==128段階目*/
		{
			pd.weapon_power = MAX_POWER_IS_128;/* 最大値 */
		}
		int add_score_point;
		if (MAX_POWER_IS_128 == pd.weapon_power/*previous_power*/) /* maxの場合 */
		{
			if (previous_power != pd.weapon_power)/* 直前がMAXじゃなかったのに、今回MAXになった場合 */
			{
				#if 1
				/* 特殊機能 */
				bullets_to_hosi();/* 弾全部、星アイテムにする */
				#endif
			}
			pd.chain_point += add_power;	/* チェイン、加算 */
			/* チェイン、超えてたら修正 */
			if (31 < pd.chain_point)
			{
				pd.chain_point = 31;/* 最大値 */
			}
			/* (ノーミスボーナス)ウェポンボーナス得点計算 */
			add_score_point = (/*score_type*/(pd.chain_point));
			#if 1
			/* チェインはボムMAXでないと実質、貯まらない */
				#if 0
		//	if (NULL==ttt)/* チルノなら */
				#else
			/* レミリア & 幽々子 & チルノは、ボム消費型(基本的にあまり貯めないでどんどん使う==割と直ぐ貯まる)にしようかと。 */
			if ((REMILIA-1) < select_player)/* レミリア & 幽々子 & チルノなら */
				#endif
			{
				if ((8-1) < pd.bombs)	{	;	}/* 既に最大値(8)ならば、なにもせず。 */
				else
				{
					pd.bombs++; 	/* ボム追加 */
					pd.weapon_power = (MAX_POWER_IS_128>>1);/* 最大値の半分 */
				}
			}
			#endif
		}
		else	/* maxでない場合、得点計算 */
		{
			/* (MAX時以外の) [P]は10点 */
			/* (MAX時以外の) [P中]は80点 */
			add_score_point = ((add_power/*-1*/)/*SCORE_10*/);
		}
		if (NULL!=ttt)/* チルノ以外なら */
		{
			bonus_info_score_nodel(ttt, add_score_point);
			/* */ttt->jyumyou = JYUMYOU_NASI;/* おしまい */
		}
	}
	check_weapon_level();/* [pd.weapon_powerが変更された場合に必ず行う後チェック] */
//
	voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*テキトー*/
}

static void player_colision_check_item(SPRITE *src)/*, int mask*/	/*,SP_GROUP_ITEMS*/
{
	/* 自機がアイテムにあたった場合 */
	SPRITE *ttt;	//対象
	ttt = sprite_collision_check_444(src, SP_GROUP_ITEMS);	/* 弾幕専用(アイテム) */	/*mask*/
	if (NULL != ttt)
	{
		switch (ttt->type)
		{
		case SP_ITEM_00_P001:	//	player_add_power(src, ttt, 1);		break;	// ウェポンアイテム[小p]
		case SP_ITEM_01_P008:	//	player_add_power(src, ttt, 8);		break;	// ウェポンアイテム[大P]	// [***090123		追加
		case SP_ITEM_02_P128:	//	player_add_power(src, ttt, 127);	break;	// ウェポンアイテム[F]		// [***090123		追加
									player_add_power_type(src, ttt);	break;	// ウェポンアイテム[小p][大P][F]
		//
		case SP_ITEM_04_BOMB:
			//player_add_bomb(t);		// [*****本来はコメントアウトしないよ
			if ((8-1) < pd.bombs)	{	goto add_10000pts;	}/* 既に最大値(8)ならば、10000+ [pts] */
			pd.bombs++;
			#if 1/*バグfix?*/
			ttt->jyumyou = JYUMYOU_NASI;/* おしまい */
			#endif
			voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*テキトー*/
			break;

		case SP_ITEM_03_1UP:
			if ((8-1) < pd.zanki)	{	goto add_10000pts;	}/* 既に最大値(8)ならば、10000+ [pts] */
			pd.zanki++;
			#if 1/*バグfix?*/
			ttt->jyumyou = JYUMYOU_NASI;/* おしまい */
			#endif
			/*effect_sound_number=*/voice_play(VOICE06_EXTEND, TRACK03_SHORT_MUSIC);/*テキトー*/
			break;

		case SP_ITEM_07_SPECIAL:
			break;
		#if 0
		/* 星点は、あたり判定なし */
		case S P_ITEM_05_HOSI:		/*not_break;*/
			ttt->jyumyou = JYUMYOU_NASI;/* 星点のみ特別処理 */
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
				if (PLAYER_SPLIT_LINE256 < src->cy256)	/* PLAYER_SPLIT_LINE256 未満の場合は、PLAYER_SPLIT_LINE256までの距離におおじて減点 */
				{
				//	add_score_point -= ((ttt->cy256-PLAYER_SPLIT_LINE256)>>(4+8));
				//	add_score_point = ((/*0x1f-(3)*/(SCORE_9000)-((ttt->cy256-PLAYER_SPLIT_LINE256)>>(3+8)))/*&0x1f*/);
					add_score_point = (SCORE_9000) - ((ttt->cy256-PLAYER_SPLIT_LINE256)>>(3+8));
				}
				else
				{
					add_10000pts:/* 強引に得点10000+ [pts] */
				//	add_score_point = (SCORE_10000);	/* 基本点 10000[pts] */
					add_score_point = (SCORE_10000+difficulty); 	/* 基本点 10000[pts](easy), 11000(normal), 12000(hard), 512000(lunatic). */
				}
				//player_dummy_add_score(score(1000));		// [***090123		変更
			//
				bonus_info_score_nodel(ttt, add_score_point);
				/* */ttt->jyumyou = JYUMYOU_NASI;/* おしまい */
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
static void player_explode(SPRITE *s1);
static void player_set_pichuun(void)
{
//	#if 0//(0==US E_ATARI_DE BUG)/* 当たり判定デバッグ時は無敵。 */
//	/* 通常時(デバッグ以外は死ぬ(喰らいボムモードへ)) */
//	pd.special_ivent_type		= PLAYER_STATE_01_KURAI_BOMB_UKETUKE;						/* 喰らいボム入力受け付けモードへ移行する。 */
//	pds_status_timer	= player_fix_status[BASE_HIT_BOMB_DEC_WAIT+select_player];			/* 喰らいボムの入力受け付け時間を設定 */
//	pds_status_timer	= (-1);/*とりあえず*/
//	pds_status_timer	= (-1);/*とりあえず*/	/* デバッグ(喰らいボム時間、全部霊夢並みに長い)*/
//	#else
	pds_status_timer	= (-1)-player_fix_status[BASE_HIT_BOMB_DEC_WAIT+select_player]; 	/* 喰らいボムの入力受け付け時間を設定 */
//	#endif
	voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* 自機死に音は、なるべく重ねない */
	{
		SPRITE *zzz_player;
		zzz_player = &obj00[FIX_OBJ_00_PLAYER];
		player_explode(zzz_player);/*プレイヤー爆発*/
	}
}



/*---------------------------------------------------------
	アイテム以外のプレイヤーのあたり判定。
	-------------------------------------------------------
	「敵弾によるグレイズ」
	「敵弾による死亡」
	「敵(雑魚＆ボス)による死亡」
---------------------------------------------------------*/
	//	player_colision_check_graze(s1);	/* グレイズの当たり判定 */
	//	player_colision_check_enemy(s1);	/* 雑魚＆ボスの当たり判定 */

#define TEST_FIX_GRAZE048	(t256(128.0))
#define TEST_FIX_DEATH001	(t256(1.0))
#define TEST_FIX_TEKI001	(t256(1.0))

static void player_colision_check_all(SPRITE *src)
{
	//static void player_colision_check_graze(SPRITE *src)
	/*---------------------------------------------------------
		プレイヤーと敵弾のあたり判定2(グレイズ専用)
		-------------------------------------------------------
			src 	プレイヤー
			ttt 	敵弾
	---------------------------------------------------------*/
	#if (1)
	src->m_Hit256R		= TEST_FIX_GRAZE048;/* てすと、グレイズ判定。 */
	#endif
	SPRITE *ttt;	/* 敵弾 */
	ttt = NULL;
	ttt = sprite_collision_check_444(src, SP_GROUP_BULLETS);/* 弾幕専用(弾幕) */
	if (NULL != ttt)
	{
		/* プレイヤーに敵弾があたった場合はグレイズする */
		if (0==(ttt->flags & SP_FLAG_GRAZE)) /* グレイズ済? */
		{
			ttt->flags |= SP_FLAG_GRAZE;/*グレイズ済*/
			pd.graze_point++;/*グレイズ数*/
			// チルノの場合、グレイズでパワーアップ。
			if ( (CIRNO_A-1) < (select_player) )/* 6:CIRNO_A or 7:CIRNO_Q */
			{
				player_add_power_type(src, /*ttt*/NULL);
			}
			else
			{
				player_dummy_add_score((score(100)+score(200)*difficulty));/*score(500)*/
			}
				/* グレイズ音 */
				voice_play(VOICE09_GRAZE, TRACK07_GRAZE);/*テキトー*/
		}
#if 1
		/* プレイヤースプライトが弾に触れているか */
		{
			SPRITE *uuu;	/* uuu */
			uuu = NULL;
	#if (0)
			SPRITE *zzz_obj_maru;
			zzz_obj_maru = &obj00[FIX_OBJ_01_JIKI_MARU];
			/* (グレイズ中ならコアと敵弾のあたり判定をする) */
//			#if (0)
//			/* コアで判定(廃止) */
//			uuu = sprite_collision_check_444(zzz_obj_maru, SP_GROUP_BULLETS);/* 弾幕専用(弾幕) */
//			#else
			/* 自機で判定 */
			int chache_m_Hit256R;
			chache_m_Hit256R	= src->m_Hit256R;/* 自機のあたり判定サイズ */
			src->m_Hit256R		= zzz_obj_maru->m_Hit256R;/* ○のあたり判定サイズ */
			uuu = sprite_collision_check_444(src, SP_GROUP_BULLETS);/* 弾幕専用(弾幕) */
			src->m_Hit256R		= chache_m_Hit256R;/* 戻す */
//			#endif
	#else
			src->m_Hit256R		= TEST_FIX_DEATH001;/* てすと、死亡判定。 */
			/* 自機で判定 */
			uuu = sprite_collision_check_444(src, SP_GROUP_BULLETS);/* 弾幕専用(弾幕) */
			src->m_Hit256R		= TEST_FIX_GRAZE048;/* てすと、グレイズ判定。 */
	#endif
			if (NULL != uuu)	{	player_set_pichuun();	}
		}
#endif
	}

	#if (1)
	src->m_Hit256R		= TEST_FIX_TEKI001;/* てすと、敵判定。(敵との当たり判定で死ぬ) */
	#endif
	/*---------------------------------------------------------
		プレイヤーと敵のあたり判定3(敵専用)
		-------------------------------------------------------
			src 	プレイヤー
			ttt 	敵
		-------------------------------------------------------
		自機が敵(ボス/中型/ザコ)に体当たりされた場合
	---------------------------------------------------------*/
	//	static void player_colision_check_enemy(SPRITE *src)
	{
		SPRITE *ttt;	/* 敵自体(ボス/中型/ザコ) */
		/*SP_GROUP_ENEMYS*/  // (SP_GROUP_TEKI/*|SP_GROUP_BOSS*/);
		ttt = sprite_collision_check_SDL_teki(src);// (SP_GROUP_TEKI)  /*SP_GROUP_ENEMYS*/);/*敵専用*/
		if (NULL != ttt)	{	player_set_pichuun();	}
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
	pd.state_flag		|= STATE_FLAG_14_GAME_LOOP_QUIT;
	#if (0000)
	option[0]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* オプションを消す */
	option[1]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* オプションを消す */
	option[2]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* オプションを消す */
	option[3]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* オプションを消す */
	#endif/*0000*/
//
//	pds_status_timer	= (150);	/* 150	 120では若干短い 100 */
//	pd.special_ivent_type	= PLAYER_STATE_04_GAME_OUT;/* GAME_OUT中 */
	pds_status_timer	= (LIMIT_m16_GAME_OUT); 	/* とりあえず */	/* 150	 120では若干短い 100 */
}
	#if (00)
	obj00[FIX_OBJ_00_PLAYER]->flags &= (~(SP_FLAG_VISIBLE));	/* 自機、非表示 */
	obj_maru->flags 	&= (~(SP_FLAG_VISIBLE));	/* ○、非表示 */
//	obj_maru->alpha 	= 0x00; 		// ○を消すために入れたけど意味無かったかもしれない。	// ゲームオーバー時の○の表示処理
	#endif

	/* オプションが非表示の場合、何もしない。 */
//	if (0==(src->flags & ( SP_FLAG_OPTION_VISIBLE)))	{	return; 	}


/*---------------------------------------------------------
	プレイヤー復活処理
---------------------------------------------------------*/
/* ステージデーター読み込み後の無敵設定 */
global void player_load_stage_muteki(void)
{
//	pds_status_timer	= (40); 						// 無敵時間 		/*120*/ /*150-120*/
//	pd.special_ivent_type	= PLAYER_STATE_02_SAVE_01;	// 無敵状態？
	pds_status_timer	= (LIMIT_512);					// 初期無敵時間 		/*120*/ /*150-120*/
}
/* ステージ中に復活する場合の無敵設定 */
static void player_few_muteki(void)
{
//	pds_status_timer	= (40); 						// 無敵時間 		/*120*/ /*150-120*/
//	pd.special_ivent_type	= PLAYER_STATE_02_SAVE_01;	// 無敵状態？
	pds_status_timer	= (LIMIT_m127_MIN); 			// 無敵時間 		/*120*/ /*150-120*/
}

static void player_fukkatsu_bbb(SPRITE *s1)
{
		/* プレイヤー位置の初期化 */
		#if 1/*Gu(中心座標)*/
		s1->cx256 = t256(GAME_WIDTH/2);
		s1->cy256 = t256(220/*GAME_HEIGHT*/);/*240*/
		#endif
	player_few_muteki();/* ステージ開始時のみ若干の無敵状態にセット */
	#if (00)
	/*obj00[FIX_OBJ_00_PLAYER]*/s1->flags			|= ( (SP_FLAG_VISIBLE));			/* 自機、表示 */
	#endif
	/*obj00[FIX_OBJ_00_PLAYER]*/s1->color32 		= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);						/* 自機、半透明 */	/*	s1->alpha			= 0x50;*/
//
	{
		SPRITE *zzz_obj_maru;
		zzz_obj_maru = &obj00[FIX_OBJ_01_JIKI_MARU];
	//	obj_maru->flags |= ( (SP_FLAG_VISIBLE));						/* ○、表示 */
		zzz_obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);								/* ○、半透明 */	/*	obj_maru->alpha = 0x50;*/
	}
	// チルノの場合、「ボム無し」からスタート.
	if ( (CIRNO_A-1) < (select_player) )/* 6:CIRNO_A or 7:CIRNO_Q */
	{
		pd.bombs			= (0);	/* ボム所持数の初期化 */
	}
	else/* チルノ以外 */
	{
		pd.bombs			= option_config[OPTION_CONFIG_01_BOMB]; 	/* ボム所持数の初期化 */	/*player_fix_status[BASE_BOMBS+select_player]*/  /*3*/
	}
	pd.chain_point		= (0);										/* ノーミスボーナスの初期化 */	// pd.chain_point(ノーミスボーナス)の初期化 // どれだけ連続(ノーミス)でwepon_upを取ったか
//
	check_weapon_level();/* [pd.weapon_powerが変更された場合に必ず行う後チェック] */
}

/*---------------------------------------------------------
	プレイヤーやられ処理
---------------------------------------------------------*/

static void player_explode(SPRITE *s1)
{
	/* アイテムを吐き出す */	//	[***090124ちょっと追加
	{
		if ((0==pd.zanki)
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
		//	item_create(s1, (((30-1) > pd.weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), (7)/*5*/, ITEM_MOVE_FLAG_06_RAND_XY);
		//	item_create(s1, (((16) > pd.weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), (6), ITEM_MOVE_FLAG_06_RAND_XY);/* いくら下手でも、多少多いかな? */
		//	item_create(s1, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
			#else
			/* 多少修正してみる */
			item_create(s1, (((16) > pd.weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), (3), ITEM_MOVE_FLAG_06_RAND_XY);/* 下手な人ががんばれば3つ取れる事が前提 */
			item_create(s1, SP_ITEM_00_P001, (5), ITEM_MOVE_FLAG_06_RAND_XY);/* P001をばらまく(おまけ、下手な人は、がんばっても同時には取れないという事が前提) */
			#endif
		}
		//
		/* レミリア 特殊能力：抱え落ちすると必ずボムが出る */
		if (REMILIA==select_player) 	/* レミリアの場合 */
		{
			/* 抱え落ちすると */
			if (0 != pd.bombs)
			{
				/* 使ってない分の持ってるボムを吐き出す */
				item_create(s1, SP_ITEM_04_BOMB, pd.bombs, ITEM_MOVE_FLAG_06_RAND_XY);
			}
		}
	}
//		pd.explode		= 0;
//	if (0==pd.explode)
	{

	//	bakuhatsu_add_type(t->cx256+t256(5),t->cy256+t256(5),/*0,*/BAKUHATSU_MINI00);
	//	t->jyumyou = JYUMYOU_NASI;/* おしまい */			/* あたった敵爆発 */
	//	pd.explode=0;
	//	pd.bonus=0;
		//	voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* 自機死に音は、なるべく重ねない */ 	// [***090127	場所を移動する。// 変更元
		/* 爆発エフェクト */

		#if 0/* あたった敵は倒せない敵かもしれないので、ここで必ず消すのはまずい気がする */
		/* あたった敵爆発 */
		bakuhatsu_add_type(pd.enemy->cx256+t256( 5), pd.enemy->cy256+t256( 5), /*0,*/ BAKUHATSU_MINI00);
		pd.enemy->type	= SP_DELETE;/*???*/
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
		/*	*/
		pd.bomber_time	= 0;			/* ←ここが無いせいで魔理沙(&チルノ)ボム撃てなくなってた */
		#if 1
	//	pd.bomber_time = 0;/*都合上*/
		set_bg_alpha(255);/* 画面を明るくする */
		#endif

	//	pd.state_flag		= STATE_FLAG_00_NONE;/*???*/
		pd.state_flag		&= (~(	STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM | 	/* 終わり */
									STATE_FLAG_02_BOMB_AUTO_GET_ITEM |			/* 終わり */
									STATE_FLAG_03_SCORE_AUTO_GET_ITEM));		/* 終わり */

	}
}

/*---------------------------------------------------------
	プレイヤー移動、特殊処理
	-------------------------------------------------------
	やられ中、復活中、ボム中、GAME_OUT中などの特殊処理
	-------------------------------------------------------
	予定:タイマー再設計。

	０以外の場合タイマーは自動で1減算(デクリメント)。
	pds_status_timer--;
	pds_status_timer &= 0xff; // 何らかの値でマスク
//
	負値(-255):
		復活中							(方向キー入力無効。ボム低速等キー入力、無効)
	負値(-127): LIMIT_m127_MIN
		ゲームオーバーデモ				(方向キー入力無効。ボム低速等キー入力、無効。敵あたり無し。弾あたり無し。)
	負値(-16): LIMIT_m16_GAME_OUT		PLAYER_STATE_04_GAME_OUT判定(-256)。
		喰らいボム受付					(方向キー入力無効。ボム低速等キー入力、有効。敵あたり無し。弾あたり無し。)
		マイナス一定値までは喰らいボム受付時間。弾にあたった場合、キャラ別で初期値(-240とか-200とか)が違う。
	負値(-1): LIMIT_m1_KURAI_BOMB_UKETUKE
		弾にあたった場合、(値を)ワープ
	ZERO(0):	LIMIT_0_ZERO			通常時(pspはZEROレジスタがあるため0比較は速い)
		通常時							(方向キー入力有効。ボム低速等キー入力、有効。敵あたり判定有り。弾あたり判定有り)
	ZERO(0):	LIMIT_0_ZERO			通常時(pspはZEROレジスタがあるため0比較は速い)
	ITI(1): 	LIMIT_1_ONE 			スクリプト動作時(イベント中)
		復活中に付き無敵。				(方向キー入力有効。ボム低速等キー入力、有効。敵あたり無し。弾あたり無し。)
	128?(128):	LIMIT_128				プレイヤー復活時間。(固定?)
		復活中に付き無敵。				(方向キー入力有効。ボム低速等キー入力、有効。敵あたり無し。弾あたり無し。)
	255:
		ゲーム開始時に付き無敵。		(方向キー入力有効。ボム低速等キー入力、有効。敵あたり無し。弾あたり無し。)
	512:
---------------------------------------------------------*/
//void render_continue(void/*int now_max_continue*/);
/*static*/ int	now_max_continue;
static void player_move_special_ivent(SPRITE *s1)
{
	pds_status_timer--;
	#if 0
//	pds_status_timer = (signed int)(pds_status_timer & (0x1ff)); // 何らかの値でマスク
	#else
	if (LIMIT_m255_CONTINUE > pds_status_timer) {	pds_status_timer += (512); }
//	if (-511 > pds_status_timer) {	pds_status_timer += 1024; }/*デバッグ検証用*/
	#endif
//
	if (LIMIT_m127_MIN > pds_status_timer)
	{
		//復活中の処理
		/* 拡大してたら縮小 */
		#if (1==USE_ZOOM_XY)
		/* X のみ縮小。Y は変更せず。 */
		if (t256(1.0) < s1->m_zoom_x256)
		{
			s1->m_zoom_x256 -= (5);
		//	s1->m_zoom_y256 -= (5);
		}
		else
		{
			s1->m_zoom_x256 = (t256(1.0));
		//	s1->m_zoom_y256 = (t256(1.0));
		}
		#else
		if (t256(1.0) < s1->m_zoom_xy256)
		{
			s1->m_zoom_xy256 -= (5);
		}
		else
		{
			s1->m_zoom_xy256 = (t256(1.0));
		}
		#endif
	}
	else
	if (LIMIT_m127_MIN == pds_status_timer)
	{
		//ゲームオーバーデモ
//		case PLAYER_STATE_04_GAME_OUT:/* GAME_OUT中 */
//			pd.special_ivent_type	= 0;/*???*/
			now_max_continue--;
			if (
					(0 < now_max_continue)
				#if (0)/* (0==DE BUG) */
				/* まだ無効 */
				&& ( (6) != player_now_stage)	/* 最終面はコンティニュー出来ない。 */
				#endif
				)
			{
				pd.use_continue++;	/* コンティニュー回数 */
			//	render_continue(/*now_max_continue*/);
				main_call_func = ask_continue_menu_start;
			}
			else
			{
				main_call_func = gameover_start;
			}
//			break;
	}
	else
	if (LIMIT_m16_GAME_OUT > pds_status_timer)
	{
		;/* [なにもしない] */
		/* (ミス確定してから)ゲームアウトになるまでの猶予時間 */
		/* (自分のアイテムが飛び散る時間) */
	}
	else
	if (LIMIT_m16_GAME_OUT == pds_status_timer) /* プレイヤー、ガッツ減少処理 */
	{
		/* 現在オプション増えすぎて、後半重くなるバグがある。 */
		#if (0000)
		option[0]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* オプションを消す */
		option[1]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* オプションを消す */
		option[2]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* オプションを消す */
		option[3]->flags	&= (~SP_FLAG_OPTION_VISIBLE);	/* オプションを消す */
		#endif/*0000*/
		pd.count_miss++;	/* ミス回数 */
		/* 残チャンス減らす */
		pd.zanki--;
		if (0 > pd.zanki)	/* 残チャンスがなければ */
		{
			player_loop_quit();
		}
		else
		{
		/*	MAX(128)->死ぬ->(112)->死ぬ->(96)->死ぬ->(80)->死ぬ->(64)	(常に16減で一定)	*/
			pd.weapon_power -= (16);	/* 武器レベルの低下(紅、調べたら難易度に関係なくP16減だった) */
			if (0 > pd.weapon_power)	{	pd.weapon_power = 0;	}
			player_fukkatsu_bbb(s1);
			#if (1==USE_ZOOM_XY)
			s1->m_zoom_x256 = t256(1.0)+(120*5);/* 拡大 */
			s1->m_zoom_y256 = t256(1.0);//+(120*5);/* 拡大 */
			#else
			s1->m_zoom_xy256 = t256(1.0)+(120*5);/* 拡大 */
			#endif
		}
	}
	else
	if (LIMIT_m1_KURAI_BOMB_UKETUKE > pds_status_timer)
	{
		;/* [なにもしない] */
		/* 喰らいボム受付の猶予時間 */
		//喰らいボム受付中の処理
//		case PLAYER_STATE_01_KURAI_BOMB_UKETUKE:/* 喰らいボム受付中 */			// [***090125		追加
//			/* 死亡確定 */
//			pd.state_flag |= STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL;		/* キー入力無効(1) */
//			break;
	}
	else
	/*
		(LIMIT_1_ONE-1)はつまり 現状 値(0) だけど、意味は(LIMIT_1_ONE-1)という意味。
		(LIMIT_1_ONEから 1 引いたもの、この 値(-1) は(player_move_special_ivent();開始時の)pds_status_timer--;を補正する分。)
		(結果的に(現在は)LIMIT_0_ZEROと同じだが、意味はLIMIT_0_ZEROと同じではない。)
		LIMIT_1_ONEは固定値(1)ではない、現状 値(1) だけど将来は判らない。
		*/
	if ((LIMIT_1_ONE-1) == pds_status_timer)
	{
		/* 半透明を通常に戻す */
		s1->color32 		= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);					/*	s1->alpha			= 0xff;*/
		{
			SPRITE *zzz_obj_maru;
			zzz_obj_maru = &obj00[FIX_OBJ_01_JIKI_MARU];
			zzz_obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/* ○、 */	/*	zzz_obj_maru->alpha = 0xff;*/
		}
	}
//	else
//	if ((LIMIT_128-1) == pds_status_timer)
//	{
//		case PLAYER_STATE_03_SAVE_02:/*プレイヤー復活中２*/
//			pd.special_ivent_type	= PLAYER_STATE_00_NORMAL;
//				s1->m_zoom_x256 			= t256(1.0)+100;	 /* サイズ */
//			break;
//	}
}


/*---------------------------------------------------------
	プレイヤー移動、メイン処理
	-------------------------------------------------------
	ボスでボム中無敵
	ボム中は、ボス / 中ザコ敵 にあたって死ぬ
	スクリプト動作中 は、敵あたり判定はない
	キー入力無効中(==復活中) は、敵あたり判定はない
	ボム中は、 ボス/ザコ敵 にあたって死なない
	喰らいボム受付中 は、敵あたり判定はない
---------------------------------------------------------*/

static void any_player_move(SPRITE *s1)
{
	pd.state_flag &= (~STATE_FLAG_15_KEY_SHOT); /* off */
	if (0!=(pd.state_flag & (STATE_FLAG_06_IS_SCRIPT)))
	{
		pds_status_timer = (LIMIT_1_ONE);	/* スクリプト動作中 は 1 */
	}
	/* LIMIT_m16_GAME_OUTより大きい場合、移動可能、ボタン受け付け可能 */
	if (LIMIT_m16_GAME_OUT < pds_status_timer)
	{
		player_keycontrol(s1);/* 移動＋ボタン処理 */
	}
	/* 通常時 又は 例外時の処理。(例外時の処理は分離しとく) */
	if (0 == pds_status_timer)	/* 通常時の場合 */
	{
		/* プレイヤーと、「敵弾によるグレイズ」/「敵弾による死亡」/「敵(雑魚＆ボス)による死亡」の当たり判定 */
		player_colision_check_all(s1);
	}
	else	/* 例外時(特殊処理)の場合 */
	{
		player_move_special_ivent(s1);	/* 特殊処理 */
	}
	/* アイテムは常に取れる */
	player_colision_check_item(s1); 		/* アイテムの当たり判定 */
}

//		bullets_to_hosi();/* 弾全部、星アイテムにする */
//廃止	if (0==(pd.state_flag & STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL))	/* キー入力有効(0) */
//		if (0 <= pds_status_timer)	/* 正値の場合、移動可能 */

	#if 0/*デバッグ*/
	if (0 <= pds_status_timer)	/* 正値の場合 */
	{
		p d_my_score	= pds_status_timer;
		p d_graze_point = 0;
	}
	else
	{
		p d_my_score	= -pds_status_timer;
		p d_graze_point = 11;
	}
	#endif


/*---------------------------------------------------------
	プレイヤー初期化()
---------------------------------------------------------*/
#if (1==USE_EXTEND_CHECK)
extern void player_init_extend_score(void);
#endif
//extern int zanki;
global void player_continue_value(void)
{
	pd.zanki				= (1+option_config[OPTION_CONFIG_00_PLAYER]);	/*pd.base_zanki*/	/*((zanki&0x03)+2)*/ /*player_fix_status[BASE_LIVES+select_player]*/
	pd.game_score			= score(0);
	pd.state_flag			&= (~STATE_FLAG_14_GAME_LOOP_QUIT); 	/* 復帰 */
	#if (1==USE_EXTEND_CHECK)
	player_init_extend_score();
	#endif
	player_fukkatsu_bbb(&obj00[FIX_OBJ_00_PLAYER]);/* オプションを追加より後 */

	/*
		[コンティニュースタート時]及び、[新規スタート時]の特別処理。(つまりゲーム中復活時[以外の]場合)
		------------------------------------------------------------
		これらの場合は、選択後に無敵になり、
		かつ直ぐに動ける(方向キー入力有効...)ように細工する。
		------------------------------------------------------------
		(ゲーム中復活時の場合は一定時間動けない。)
	*/
	/* 拡大率を1.0倍にする。 */
//	s1->m_zoom_x256 = (t256(1.0));
	/* (無敵時間で)直ぐに動ける。 */
	pds_status_timer = (LIMIT_m255_CONTINUE);
}

/*---------------------------------------------------------
	プレイヤー、オプションの追加
	霊夢	魔理沙	幽々子 レミリア ＆ チルノ
---------------------------------------------------------*/

static void option_create(void) /* レミリア ＆ チルノ [***090220 追加 */
{
	SPRITE *zzz_player;
	zzz_player = &obj00[FIX_OBJ_00_PLAYER];
	unsigned int jj;
	for (jj=0; jj<4; jj++)
	{
		SPRITE *h;
		h						= sprite_add_direct(FIX_OBJ_04_JIKI_OPTION0/*+OPTION_C1*/+jj);	/* 必ず登録できる。  */
		h->jyumyou				= JYUMYOU_MUGEN;/* 時間で自動消去しない */
		{
		//	h->type 				= (JIKI_OPTION_00_00/*|S P_MUTEKI*/)+kk; kk += (16);/* オプションインターリーブ */ /*8*/
			h->type 				= (JIKI_OPTION_00_00/*|S P_MUTEKI*/)+(jj<<4); /* オプションインターリーブ */ /*8*/
			{
				static const /*int*/s16 aaa_tbl[4] =
				{
					cv1024r(360-(45*5)),	/*cv1024r(45*1)*/ /*1024*1/8*/ /*;*/ /*チルノ*/
					cv1024r(360-(45*7)),	/*cv1024r(45*5)*/ /*1024*5/8*/ /*;*/ /*チルノ*/
					cv1024r(360-(45*1)),	/*cv1024r(45*3)*/ /*1024*3/8*/ /*;*/ /*チルノ*/
					cv1024r(360-(45*3)) 	/*cv1024r(45*7)*/ /*1024*7/8*/ /*;*/ /*チルノ*/
				};
				h->PL_OPTION_DATA_angleCCW1024/*REMILIA_angle1024*/ = aaa_tbl[jj];
			}
				h->PL_OPTION_DATA_opt_anime_add_id		= (OPTION_C1+jj);

		//
			#if 1/*Gu(中心座標)*/
			h->cx256					= (zzz_player->cx256);
			h->cy256					= (zzz_player->cy256);
			#endif
			h->m_Hit256R			= TAMA_ATARI_BULLET_DUMMY;
		//	h->flags				|= (SP_FLAG_OPTION_VISIBLE);
			h->flags				&= (~(SP_FLAG_OPTION_VISIBLE)); 	/* 可視フラグのOFF(不可視) */
			h->flags				&= (~(SP_FLAG_COLISION_CHECK)); 	/* あたり判定のOFF(無敵) */

	/*???*/ h->base_weapon_strength 				= (1/*8*5*/);/* [***090214 追加 */
			{
			//
				h->callback_mover		= player_move_option;
			//
			//	h->PL_OPTION_DATA_next					= obj00[FIX_OBJ_00_PLAYER];
			//	h->P L_OPTION_DATA_state2				= 0;
				h->PL_OPTION_DATA_offset_x256			= t256(0);
				h->PL_OPTION_DATA_offset_y256			= t256(0);
			//	h->PL_OPTION_DATA_ccc_angle512			= cv1024r((0));
				h->PL_OPTION_DATA_slow_count			= 0;
			}
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
extern void sprite_panel_init(void);
global void player_init(void)
{
	select_jiki_load_surface();
//
	sprite_all_cleanup();/* 全obj消去 */
//
	SPRITE *s1;
	s1					= sprite_add_direct(FIX_OBJ_00_PLAYER); 	/* 必ず登録できる。  */
	s1->jyumyou 		= JYUMYOU_MUGEN;/* 時間で自動消去しない */
	{
		static const s32 graze_atari[8] =
		{
			t256(16.0), 	/* 霊夢A */ 	//	BASE_CORE_ATARI_0a,/*6*/		/* 霊夢A */ 	/*	5 */	BASE_CORE_ATARI_0a,/*6*/
			t256(16.0), 	/* 霊夢B */ 	//	BASE_CORE_ATARI_0b,/*6*/		/* 霊夢B */ 	/*	5 */	BASE_CORE_ATARI_0a,/*6*/
			t256(32.0), 	/* 魔理沙A */	//	BASE_CORE_ATARI_0c,/*8*/		/* 魔理沙A */	/*	6 */	BASE_CORE_ATARI_0b,/*8*/
			t256(32.0), 	/* 魔理沙B */	//	BASE_CORE_ATARI_0d,/*8*/		/* 魔理沙B */	/*	6 */	BASE_CORE_ATARI_0b,/*8*/
			t256(16.0), 	/* レミリア */	//	BASE_CORE_ATARI_0e,/*8*/		/* 魔理沙C */	/*	7 */	BASE_CORE_ATARI_0c,/*12*/
			t256(48.0), 	/* 幽々子 */	//	BASE_CORE_ATARI_0f,/*12*/		/* レミリア */	/*	9 */	BASE_CORE_ATARI_0e,/*8*/
			t256(48.0), 	/* チルノA */	//	BASE_CORE_ATARI_0g,/*16*/		/* チルノ */	/*	8 */	BASE_CORE_ATARI_0d,/*16*/
			t256(48.0), 	/* チルノQ */	//	BASE_CORE_ATARI_0h,/*8*/		/* 幽々子 */	/*	6 */	BASE_CORE_ATARI_0b,/*8*/
		};
	//	s1->m_Hit256R		= JIKI_ATARI_ITEM_16;/* t256(16.0); グレイズ判定 */
		s1->m_Hit256R		= ((graze_atari[select_player]));/* グレイズ判定 */
	}
	s1->type			= (SP_GROUP_JIKI_GET_ITEM);
	s1->flags			|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	s1->callback_mover	= any_player_move;
	#if 1/*Gu(中心座標)*/
	s1->cx256			= (t256(GAME_WIDTH/2));
	s1->cy256			= (t256(GAME_HEIGHT));
	#endif
//
	/* プレイヤー、生死判定用コア(obj_maru == ○)の追加 */

	SPRITE *zzz_obj_maru;
	zzz_obj_maru				= sprite_add_direct(FIX_OBJ_01_JIKI_MARU);	/* 必ず登録できる。  */
	zzz_obj_maru->jyumyou		= JYUMYOU_MUGEN;/* 時間で自動消去しない */
//	zzz_obj_maru->type			= (JIKI_CORE_00_REIMU_A+select_player);
//	zzz_obj_maru->type			= (SPELL_SQUERE_);
	zzz_obj_maru->type			= (SP_DUMMY_MUTEKI);/*(S P_GROUP_ETC_DUMMY_REMILIA);*/
	{
		static const s32 base_core_atari[8] =
		{
			t256(0.75), 	/* 霊夢A */ 	//	BASE_CORE_ATARI_0a,/*6*/		/* 霊夢A */ 	/*	5 */	BASE_CORE_ATARI_0a,/*6*/
			t256(0.75), 	/* 霊夢B */ 	//	BASE_CORE_ATARI_0b,/*6*/		/* 霊夢B */ 	/*	5 */	BASE_CORE_ATARI_0a,/*6*/
			t256(1.50), 	/* 魔理沙A */	//	BASE_CORE_ATARI_0c,/*8*/		/* 魔理沙A */	/*	6 */	BASE_CORE_ATARI_0b,/*8*/
			t256(1.50), 	/* 魔理沙B */	//	BASE_CORE_ATARI_0d,/*8*/		/* 魔理沙B */	/*	6 */	BASE_CORE_ATARI_0b,/*8*/
			t256(1.75), 	/* レミリア */	//	BASE_CORE_ATARI_0e,/*8*/		/* 魔理沙C */	/*	7 */	BASE_CORE_ATARI_0c,/*12*/
			t256(2.00), 	/* 幽々子 */	//	BASE_CORE_ATARI_0f,/*12*/		/* レミリア */	/*	9 */	BASE_CORE_ATARI_0e,/*8*/
			t256(0.90), 	/* チルノA */	//	BASE_CORE_ATARI_0g,/*16*/		/* チルノ */	/*	8 */	BASE_CORE_ATARI_0d,/*16*/
			t256(0.25), 	/* チルノQ */	//	BASE_CORE_ATARI_0h,/*8*/		/* 幽々子 */	/*	6 */	BASE_CORE_ATARI_0b,/*8*/
		};
		//	if (NULL!=h)/* 登録できた場合のみ */	/* 強制登録 */
		zzz_obj_maru->m_Hit256R 		= ((base_core_atari[select_player]));
	}

//
//	#if (0==US E_BOSS_COMMON_MALLOC)
//	obj00[FIX_OBJ_08_BOSS]				= NULL;
//	#else
//----[BOSS]
	/* あたり判定の都合上 */
	{	SPRITE *h;
		h								= sprite_add_gu_error();	/* 敵obj番号の0番を必ず取得できる。 */
	//	h	あたり判定の都合上できない							= sprite_add_direct(FIX_OBJ_08_BOSS);	/* 必ず登録できる。  */
	//	if (NULL!=h)/* 登録できた場合のみ */	/* 強制登録 */		/* 仕様バグ(?) */
	//	h = &obj00[FIX_OBJ_08_BOSS];
		sprite_initialize_position(h);
		h->jyumyou				= JYUMYOU_MUGEN;/* 時間で自動消去しない */
		global_obj_boss 			= h;
	}
//	#endif
	{
		boss_effect_sprite_add();
		boss_effect_initialize_position();
	}

//
	pd.use_continue 		= 0;	/* コンティニュー回数 */
	pd.count_miss			= 0;	/* ミス回数 */
	pd.used_bomber			= 0;	/* ボム使用回数 */
	pd.use_kurai_bomb		= 0;	/* 喰らいボム使用回数 */
//	pd.count_bonus			= 0;	/* スペルカードボーナス回数 */
//
	pd.bomber_time			= 0;	/*==bomb_wait*/
	pd.graze_point			= 0;
//
	pd.state_flag			= STATE_FLAG_00_NONE;
//	if (MARISA==select_player)		{	pd.state_flag		|= STATE_FLAG_04_IS_MARISA; 	}	/* 魔理沙は常に自動収集 */
//


//
	boss_hp_dec_by_frame	= 0;/*ボス攻撃減少値、フレーム単位*/
//
	weapon_level_offset 	= select_player + (/*0==武器レベル*/0<<3);
	/* 練習モードの場合はフルパワーで始める(その代わりクリア後のイベントが見れない) */
	pd.weapon_power 		= (0==practice_mode)?(0):(MAX_POWER_IS_128);
//
	/* オプションを追加 */
	option_create();
//
	now_max_continue		= DEFAULT_MAX_CONTINUE;/*(3)*/
//
	#if 1
	player_continue_value();
	#else
//	p d_zanki				= /*pd.base_zanki*/ player_fix_status[BASE_LIVES+select_player];
//	p d_my_score			= score(0);
//	player_fukkatsu_bbb(obj00[FIX_OBJ_00_PLAYER]);/* オプションを追加より後 */
	#endif
//読み込み無敵で再設定されるので無意味//	pds_status_timer	= (LIMIT_512);					// 初期登場時、無敵時間
	#if (1==USE_ZOOM_XY)
	s1->m_zoom_x256 = t256(1.0);/*	初期登場時、拡大率 x 1.0[倍] */
	s1->m_zoom_y256 = t256(1.0);/*	初期登場時、拡大率 x 1.0[倍] */
	#else
	s1->m_zoom_xy256 = t256(1.0);/* 初期登場時、拡大率 x 1.0[倍] */
	#endif
	sprite_panel_init();
}


/*---------------------------------------------------------
	敵やられ
	ボス倒した場合の処理
	(プレイヤー側含む)
	-------------------------------------------------------
???謎コメント???	ボスを倒したらすぐ呼ばれる(プレイヤーを無敵にする為)
???謎コメント???	(フラグは仕様上時間待ちがある為、現在この用途には使えない)
---------------------------------------------------------*/
global void set_clear_pdc_special_ivent(void)
{
	#if (0)
//	pds_status_timer	= /*pd.bomber_time*/(255) + USER_BOMOUT_WAIT;/*(30)*/	/*実質無敵時間*/
	#else
//	pds_status_timer	= (6);/* 6[フレーム] ボス倒してから次(シナリオ)に進むまでの待ち時間 */
//	pds_status_timer	= (60);/* 60[フレーム] ボス倒してから、ボスが画面外に退避するまでの待ち時間 */
//	pds_status_timer	= (9999);/* 退避後に退避内で指定。	60[フレーム] ボス倒してから、ボスが画面外に退避するまでの待ち時間 */
//	pds_status_timer	= (600);/* 退避後に退避内で指定。	60[フレーム] ボス倒してから、ボスが画面外に退避するまでの待ち時間 */
	#endif
//	pds_status_timer	= 40/*120*/ /*150-120*/;									// 無敵時間
//	pd.special_ivent_type	= PLAYER_STATE_02_SAVE_01;	// 無敵状態？
//	pd.special_ivent_type	= PLAYER_STATE_03_SAVE_02;	/* 稀に、うまくいかない */
//	pds_status_timer	= (LIMIT_m127_MIN); 				// 無敵時間 		/*120*/ /*150-120*/
	pds_status_timer	= (LIMIT_65535);					// 無敵時間 		/*120*/ /*150-120*/
}


/*---------------------------------------------------------
	レーザー
---------------------------------------------------------*/
global void add_laser_off(SPRITE *src)
{
	pd.laser_mode = (0);/* off */
}
global void add_laser_on(SPRITE *src)
{
	pd.laser_mode = (1);/* on */
}
