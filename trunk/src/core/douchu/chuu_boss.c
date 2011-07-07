
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"ルーミア", 				"GFAIRY",
		"妖怪2"(赤),				"GFAIRY",
		"妖怪1"(青),(暫定的)		"GFAIRY",
		"妖怪3"(紫),(暫定的)		"GFAIRY",
	-------------------------------------------------------
	; (72==64[差分氏]+8[ルーミア弾])
	; (79==64[差分氏]+15[天狗弾])		(差分氏の妖怪互換)。
---------------------------------------------------------*/

#if 0/*メモ*/
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define boss_time_out		user_data03 	/* 制限時間 */
#endif

#define YOKAI1_DATA_repeat				user_data04 	/* 繰り返し回数 */
#define YOKAI1_DATA_start_limit_y256	user_data05 	/* 登場y座標 */
#define yokai_type						user_data06 	/* 妖怪の種類(0 ... 3) */
#define YOKAI1_DATA_start_danmaku		user_data07 	/* 開始する弾幕番号 */

//
#define YOKAI1_DATA_s2					target_obj		/* SPRITE *s2;*/

enum /* _yokai_type_ */
{
	YOKAI_TYPE_00_RUMIA = 0,	/* 0:00 ルーミア */
	YOKAI_TYPE_01_YOUKAI02, 	/* 1:32 "妖怪2"(赤) */
	YOKAI_TYPE_02_YOUKAI01, 	/* 2:64 "妖怪1"(青) */
	YOKAI_TYPE_03_YOUKAI03		/* 3:96 "妖怪3"(紫) */
};


/*---------------------------------------------------------
	SPRITE *src;	中-ボス敵自体
	SPRITE *tama;	自弾
---------------------------------------------------------*/

static void callback_hit_chuu_boss(SPRITE *src, SPRITE *tama)
{
	/* ボス & 中-ボスに自弾があたった場合の火花エフェクトを登録(現在Gu部分がないので描画してない) */
	bakuhatsu_add_type_ddd(tama, BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/* 先に実行した方が速い */
//
	src->base_hp -= tama->base_weapon_strength; 	/* 攻撃して体力減らす(強さ分引く) */
	if (0 >= src->base_hp)			/* ０か負値なら、倒した。 */
	{
		#if 1/*要るの？*/
		src->base_hp	= (9999);/* ２回は倒せない */
		#endif
		bullets_to_hosi();/* 弾全部、星アイテムにする */
	//
		int item_type;
		if (YOKAI_TYPE_00_RUMIA == src->yokai_type)
		{
			const u8 item_tbl[4] =
			{
				(SP_ITEM_02_P128&0xff), 	/* [F] */
				(SP_ITEM_01_P008&0xff), 	/* [P]大 */
				(SP_ITEM_00_P001&0xff), 	/* [p]小 */
				(SP_ITEM_05_TENSU&0xff),	/* [点] */
			};
			item_type = (SP_GROUP_ITEMS|(item_tbl[(cg_game_difficulty)]));
		}
		else
		{
			item_type = SP_ITEM_05_TENSU;	/* 点数を出す */
		}
		item_create(src, item_type, 7, ITEM_MOVE_FLAG_06_RAND_XY);	/* ちらばる */
		player_dummy_add_score(src->base_score);
//
		bakuhatsu_add_circle(src, 0);
	//	src->type						= SP_DELETE;
		/* 倒した場合背後の魔方陣が消えているが、逃がした場合背後の魔方陣がある。 */
		src->YOKAI1_DATA_s2->jyumyou		= JYUMYOU_NASI; /* 倒した場合、背後の魔方陣を消す。 */
		src->jyumyou						= (512-1);
		/* コールバック登録 */
		src->callback_hit_teki = NULL;	/* ダミーコールバック登録 */
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_chuu_boss(SPRITE *src)
{
	if (512 > src->jyumyou) 	// SS04:	/* 上へ退場 */
	{
		#if 1/* 現在の実装方式は、あんま良くない気もする。 */
		/* 倒した場合背後の魔方陣が消えているが、逃がした場合背後の魔方陣がある。 */
	//	if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* チェックは実装方式依存。 */
		if (JYUMYOU_NASI <= src->YOKAI1_DATA_s2->jyumyou)/* 背後の魔方陣があれば移動。 */
		{
			src->YOKAI1_DATA_s2->cy256	-= t256(1.5);	/*fps_factor*/
		}
		#endif
		src->cy256						-= t256(1.5);	/*fps_factor*/
		if ( 0 > (src->cy256+t256(55.0)))/* 55ドットとして(左上基点、縦のサイズ) */
	//	if ( 0 > src->cy256)/* 簡略版 */
		{
			src->jyumyou						= JYUMYOU_NASI;
		//	if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* チェックは実装方式依存。 */
			if (JYUMYOU_NASI <= src->YOKAI1_DATA_s2->jyumyou)/* 背後の魔方陣があれば消去。 */
			{
				src->YOKAI1_DATA_s2->jyumyou	= JYUMYOU_NASI;
			}
			/* 退場処理 */
			chu_boss_mode = 0;
			//hold_game_mode_off();
		}
	}
	else
	if (512+100 > src->jyumyou) 	// SS03:	/* しばし待つ */
	{
	//	if (0 > src->time_out)
		{
	//		src->time_out = 100/*150*/;
			/* 規定の繰り返し回数こなしたら退場 */
			src->YOKAI1_DATA_repeat--;
			if ((0) != src->YOKAI1_DATA_repeat) 	{	src->jyumyou = (512+100+512+100-1);/*SS01;*/		}/* 繰り返し */
			else									{	src->jyumyou = (512-1);/* = SS04*/					}/* 上へ退場 */
		}
	}
	else
	if (512+100+512 > src->jyumyou) 	// SS02:	/* 禊弾幕が終わるまで待機 */
	{
	//	danmaku_state_check_holding(src);
		if (SPELL_00 == src->boss_base_spell_type)
		{
			src->jyumyou = (512+100-1);
		}
	}
	else
	if (512+100+512+100 > src->jyumyou) 	// SS01:	/* 弾幕セット */			/* 8偶数弾 7奇数弾 */
	{
			src->jyumyou = (512+100+512-1);/* = SS02;*/
			src->boss_base_spell_type	= src->YOKAI1_DATA_start_danmaku;	/* 弾幕をセット */		/* "妖怪2"&"ルーミア"専用 */
			spell_set_time_out(src);		/* 弾幕の時間切れを設定 */
	}
	else
//	if (512+100+512+100+100 > src->jyumyou) // SS00:	/* 上から登場 */
	{
		src->YOKAI1_DATA_s2->cy256	+= t256(2.0);	/*fps_factor*/
		src->cy256					+= t256(2.0);	/*fps_factor*/
		if (src->YOKAI1_DATA_start_limit_y256 < src->cy256)
		{
			src->jyumyou = (512+30-1);		/*	src->time_out = 30; SS03;*/
		}
	}

//	if (SP_DELETE != src->type)
	if (JYUMYOU_NASI < src->jyumyou)/* あれば */
	/* アニメーション */
	{
		if (YOKAI_TYPE_00_RUMIA == src->yokai_type)
		{
			src->type			= TEKI_00_BOSS11+((src->jyumyou&0x30)>>4);
		}
		else
		{
			src->type			= TEKI_54_CHOU1+((src->jyumyou&0x10)>>4);
		}
	}
//	if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* チェックは実装方式依存。 */
	if (JYUMYOU_NASI < src->YOKAI1_DATA_s2->jyumyou)/* あれば */
	{
		src->YOKAI1_DATA_s2->rotationCCW1024 += (3);
		mask1024(src->YOKAI1_DATA_s2->rotationCCW1024);
	}
//
	if (512 > src->jyumyou)
	{
		;	/* 上へ退場 */
	}
	else
	{
		spell_generator(src); /* 弾幕生成 */
	}
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
global int chu_boss_mode;
global void game_command_04_regist_chuu_boss(GAME_COMMAND *l)
{
	if (0==chu_boss_mode)
	{
		chu_boss_mode = 1;
		//hold_game_mode_on();
	/* プライオリティー(表示優先順位)があるから、背後に表示させる為に、初めに後ろの魔方陣を確保。 */
	SPRITE *s2; 		/* 後ろの魔方陣 */
	s2								= obj_add_01_teki_error();
	if (NULL!=s2/*h*/)/* 登録できた場合のみ */
	{
		/* 後ろの魔方陣が確保出来たら本体を確保。 */
		SPRITE *s1; 	/* 本体 */
		s1									= obj_add_01_teki_error();
		if (NULL!=s1/*h*/)/* 登録できた場合のみ */
		{
			/* 0ttd dddd
				'0': force only zero.
				't': yokai type. (0-3)
				'd': danmaku type. (0-31)
			*/
			int my_yokai_type;
		//	my_yokai_type = (((l->user_255_code)>>5)&(4-1));
			my_yokai_type = (((l->user_255_code)   )&(4-1));
		//
			obj_send1->cx256			= ((l->user_x)<<(8));
			obj_send1->cy256			= t256(-30.0);
			/* 魔方陣生成 */
			s2->m_Hit256R				= ZAKO_ATARI02_PNG;
			s2->type					= TEKI_52_MAHOJIN;/*S P_M UTEKI*/
			s2->flags					|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			{
				const u32 color_table[4] =
				{	/*	s2->alpha			= 0x00;*/
					MAKE32RGBA(0xff, 0xff, 0xff, 0x7f), 	/* 灰っぽく */
					MAKE32RGBA(0xff, 0x3f, 0x3f, 0x7f), 	/* 赤っぽく */
					MAKE32RGBA(0x3f, 0x00, 0xff, 0x7f), 	/* 青っぽく */
					MAKE32RGBA(0x5f, 0xff, 0xaf, 0x7f), 	/* 紫っぽく(水色になった) */
				};
				s2->color32 				= color_table[my_yokai_type];
			}
			s2->cx256					= (obj_send1->cx256);
			s2->cy256					= (obj_send1->cy256);
		//
			s1->m_Hit256R				= ZAKO_ATARI16_PNG;
			s1->yokai_type				= my_yokai_type;

			if (YOKAI_TYPE_00_RUMIA == (my_yokai_type))/* ルーミア */
			{
				s1->type				= TEKI_00_BOSS11;
			}
			else	/* 妖怪2 */
			{
				s1->type				= TEKI_54_CHOU1;
			}
			s1->flags					|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			s1->callback_mover			= move_chuu_boss;
		//	s1->callback_loser			= lose_youkai1;
			s1->callback_hit_teki		= callback_hit_chuu_boss;	/* コールバック登録 */
			s1->cx256					= (obj_send1->cx256);
			s1->cy256					= (obj_send1->cy256);
		//
			/* easyでも存在感を印象づける為に 200 は必要 */ 	// 50+150*di fficulty;
			s1->base_hp 				= ((l->user_hp));		/* 設定ファイルから体力を決める。 */
			s1->base_score				= ((l->user_score));	/* 設定ファイルから獲得スコアを決める。 */
		//
			s1->YOKAI1_DATA_repeat				= (2+2+1);
			s1->YOKAI1_DATA_start_limit_y256	= ((l->user_y)<<(8));/* t256(50.0) */
			s1->YOKAI1_DATA_s2					= s2;
			{
				#if 1
			//------------ 弾幕関連
				s1->boss_base_spell_type			= 0;
			//	s1->YOKAI1_DATA_start_danmaku		= ((((l->user_255_code)&0x1f)+0x10/*とりあえずr27互換*/)&0x1f);
			//	s1->YOKAI1_DATA_start_danmaku		= ((l->user_255_code)&0x1f);
				s1->YOKAI1_DATA_start_danmaku		= ((l->user_kougeki_type)&0x3f);
				#endif
			}
		}
		else
		{
			/* 本体(s1)が登録できなかった場合、背後の魔方陣(s2)の登録もあきらめる。 */
			s2->jyumyou 			= JYUMYOU_NASI;/* 登録できなかった場合 */
		}
	}
	}
}
		//	s1->base_hp 				= 1856;//(8*200)+(2/*di fficulty*/<<(4+3)); 	/* ルーミア */
		//	s1->base_hp 				= (8*160)+(1/*di fficulty*/<<(4+3-1));	/* 妖怪2 */
		//	s1->base_score				= score(100)+score(100)*(cg_game_difficulty);

/*
 (46-24)/2 == 11
*/

/*
 64=1*16+48
 80=2*16+48
112=4*16+48
160=7*16+48
*/
