
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"ルーミア", 				"GFAIRY",
		"妖怪2"(赤),				"GFAIRY",
		"妖怪1"(青),(暫定的)		"GFAIRY",
	-------------------------------------------------------
	差分氏の妖怪、これは廃止して "妖怪2" に吸収予定。
---------------------------------------------------------*/

#if 0/*メモ*/
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif

#ifndef time_out
	#define time_out	user_data03
#endif
//#define speed256	user_data03
//#define state 	user_data04

//typedef struct
//{
//	ENEMY_BASE base;
//	BO SS_BASE boss_base;
//------------ 移動関連
//	int st ate1;
//	int time_out;			/* 行動wait */
#define YOKAI1_DATA_repeat				user_data04 	/* 繰り返し回数 */
#define YOKAI1_DATA_start_limit_y256	user_data05 	/* 登場y座標 */
#define yokai_type						user_data06 	/* */
#define YOKAI1_DATA_start_danmaku		user_data07 	/* ???() */

//
#define YOKAI1_DATA_s2					target_obj		/* SPRITE *s2;*/
//} YOKAI1_DATA;
//	int wait2;	/* 攻撃wait */

enum /* _yokai_type_ */
{
	YOKAI_TYPE_00_RUMIA = 0,	/* 0 ルーミア */
	YOKAI_TYPE_01_YOUKAI02, 	/* 1 "妖怪2"(赤) */
	YOKAI_TYPE_02_YOUKAI01, 	/* 2 "妖怪1"(青)差分氏っぽいの */
};

enum
{
	SS00 = 0,
	SS01,
	SS02,
	SS03,
	SS04,
};


/*---------------------------------------------------------
	s = ボス本体 boss sprite
	t = プレイヤーの弾 player's weapon
---------------------------------------------------------*/

static void callback_hit_chuu_boss(SPRITE *src/*敵自体*/, SPRITE *tama /*自弾*/)
{
	bakuhatsu_add_type_ddd(tama, BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);
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
			item_type = (SP_GROUP_ITEMS|(item_tbl[difficulty]));
		}
		else
		{
			item_type = SP_ITEM_05_TENSU;	/*点数を出す*/
		}
		item_create(src, item_type, 7, ITEM_MOVE_FLAG_06_RAND_XY);	/* ちらばる */
		player_add_score( src->base_score);
//
		bakuhatsu_add_circle(src, 0);
	//	src->type						= SP_DELETE;
		/* 倒した場合背後の魔方陣が消えているが、逃がした場合背後の魔方陣がある。 */
		src->YOKAI1_DATA_s2->type		= SP_DELETE;	/* 倒した場合、背後の魔方陣を消す。 */
		src->boss_base_state001 		= SS04; 		/* 上へ退場 */
		/* コールバック登録 */
		src->callback_hit_enemy = NULL; 	/* ダミーコールバック登録 */
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

#if 0
	;/* 8偶数弾 */
	data->wait2--;
	if (0 > data->wait2)
	{
		/* 0: 20 == 20-(0*4) */
		/* 1: 16 == 20-(1*4) */
		/* 2: 12 == 20-(2*4) */
		/* 3:  8 == 20-(3*4) */
		data->wait2 = ((20-1)-((difficulty)<<2));/*(8-1)*/ /*(10-1)*/
		voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*テキトー*/
		obj_send1->x256 					= src->x256;
		obj_send1->y256 					= src->y256;
		//
		br.BULLET_REGIST_speed256			= (t256(3.0)+((difficulty)<<6));
		br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_03_MIDORI;
		br.BULLET_REGIST_n_way				= (8);
		//
		br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
		br.BULLET_REGIST_div_angle1024		= (int)(1024/24);
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_vector();
	}
#endif

#if 0
	;/* 7奇数弾 */
	data->wait2--;
	if (0 > data->wait2)
	{
		data->wait2 = ((20-1)-((difficulty)<<2));/*(8-1)*/ /*(10-1)*/
		voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*テキトー*/
		obj_send1->x256 					= src->x256;
		obj_send1->y256 					= src->y256;
		//
		br.BULLET_REGIST_speed256			= t256(3.25)+((difficulty)<<6);
		br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_04_MIZUIRO;
		br.BULLET_REGIST_n_way				= (7);
		//
		br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
		br.BULLET_REGIST_div_angle1024		= (int)(1024/24);
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_vector();
	}
#endif

static void move_chuu_boss(SPRITE *src)
{
	src->time_out--;
	switch (src->boss_base_state001)
	{
	case SS00:	/* 上から登場 */
		src->YOKAI1_DATA_s2->y256	+= t256(2.0);	/*fps_factor*/
		src->y256					+= t256(2.0);	/*fps_factor*/
		if (src->YOKAI1_DATA_start_limit_y256 < src->y256)	{	src->time_out = 30; src->boss_base_state001 = SS03; }
		break;
//---------
	case SS01:	/* 弾幕セット */
			/*src->YOKAI1_DATA_wait2 = (8-1) (10-1)*/
			src->boss_base_state001++;/* = SS02;*/
			;/* 8偶数弾 */
			;/* 7奇数弾 */
			src->boss_base_danmaku_type 	= src->YOKAI1_DATA_start_danmaku;	/*DANMAKU_01*/	/* 禊弾幕をセット */		/* "妖怪2"&"ルーミア"専用 */
			danmaku_set_time_out(src);		/* 弾幕の時間切れを設定 */
		break;
	case SS02:	/* 禊弾幕が終わるまで待機 */
		danmaku_state_check_holding(src);	/*, SA07*/ /*, SAKUYA_ANIME_04_CENTER_A*/
		break;
	case SS03:	/* しばし待つ */
		if (0 > src->time_out)
		{
			src->time_out = 100/*150*/;
			/* 規定の繰り返し回数こなしたら退場 */
			src->YOKAI1_DATA_repeat--;
			if ((0) != src->YOKAI1_DATA_repeat) 	{	src->boss_base_state001 	= SS01; 		}/* 繰り返し */
			else									{	src->boss_base_state001++;/* = SS04*/		}/* 上へ退場 */
		}
		break;
//---------
	case SS04:	/* 上へ退場 */
		/* 倒した場合背後の魔方陣が消えているが、逃がした場合背後の魔方陣がある。 */
		if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* チェックは実装方式依存。 */
		{
			src->YOKAI1_DATA_s2->y256	-= t256(1.5);	/*fps_factor*/
		}
		src->y256						-= t256(1.5);	/*fps_factor*/
		#if 0/*SDL(左隅座標)*/
		if ( -((src->h128+src->h128)) > src->y256)
		#endif
		#if 0/*Gu(中心座標)*/
		if ( 0 > src->y256)
		#endif
		#if 0/*現在、左上基点なので、本来はこう*/
		if ( 0 > (src->y256+((src->height_size256/*縦のサイズ*/))))
		#endif
		#if 0/*Gu(中心座標基点になったら、本来はこう。*/
		if ( 0 > (src->y256+((src->height_harf_size256/*縦の半分サイズ*/))))
		#endif
		if ( 0 > (src->y256+t256(55.0)))/* 55ドットとして(左上基点、縦のサイズ) */
	//	if ( 0 > src->y256)/* 簡略版 */
		{
			src->type = SP_DELETE;
			if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* チェックは実装方式依存。 */
			{
				src->YOKAI1_DATA_s2->type = SP_DELETE;
			}
		}
		break;
	}
	if (SP_DELETE != src->type)
	{
		if (YOKAI_TYPE_00_RUMIA == src->yokai_type)
		{
			src->type			= TEKI_00_BOSS11+((src->time_out&0x30)>>4);
		}
		else
		{
			src->type			= TEKI_54_CHOU1+((src->time_out&0x10)>>4);
		}
	}
//
	if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* チェックは実装方式依存。 */
	{
		src->YOKAI1_DATA_s2->m_angleCCW1024 += 3;
		mask1024(src->YOKAI1_DATA_s2->m_angleCCW1024);
	}
//
	danmaku_generator(src); /* 弾幕生成 */
}

static void move_youkai1(SPRITE *src)
{
	src->time_out--;
	switch (src->boss_base_state001)
	{
	case SS00:	/* 上から登場 */
		src->YOKAI1_DATA_s2->y256	+= t256(2.0);	/*fps_factor*/
		src->y256					+= t256(2.0);	/*fps_factor*/
		if (src->YOKAI1_DATA_start_limit_y256 < src->y256)	{	src->time_out=30;	src->boss_base_state001 = SS03; }
		break;
//---------
	case SS01:
		if (0 > src->time_out)	{	src->time_out = 30; src->boss_base_state001=SS03;	/*src->YOKAI1_DATA_wait2=(8-1) (10-1);*/	}
		else
		{
		//	static int YOKAI1_DATA_wait2	= (10-1);/*0*/
			static int YOKAI1_DATA_wait2	= 0;		/* 攻撃wait */
			YOKAI1_DATA_wait2--;
			if (0 > YOKAI1_DATA_wait2)
			{
				/* 0: 20 == 20-(0*4) */
				/* 1: 16 == 20-(1*4) */
				/* 2: 12 == 20-(2*4) */
				/* 3:  8 == 20-(3*4) */
				YOKAI1_DATA_wait2 = ((20-1)-((difficulty)<<2));/*(8-1)*/ /*(10-1)*/
				voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*テキトー*/
				obj_send1->x256 = src->x256;
				obj_send1->y256 = src->y256;
				#if (0) // 1975405
				if (src->YOKAI1_DATA_repeat&1)
				{
					br.BULLET_REGIST_speed256			= (t256(3.25)+((difficulty)<<6));
					br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_04_MIZUIRO;
					br.BULLET_REGIST_n_way				= (7);
				}
				else
				{
					br.BULLET_REGIST_speed256			= (t256(3.0)+((difficulty)<<6));
					br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_03_MIDORI;
					br.BULLET_REGIST_n_way				= (8);
				}
				#else // 1975357
				{	const int aaa = (src->YOKAI1_DATA_repeat&1);
					br.BULLET_REGIST_speed256			= (t256(3.0)+((difficulty)<<6))+(aaa<<(8-2));
					br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_03_MIDORI+(aaa);
					br.BULLET_REGIST_n_way				= (7)+(aaa);
				}
				#endif
				br.BULLET_REGIST_angle1024				= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_div_angle1024			= (int)(1024/24);
				br.BULLET_REGIST_regist_type			= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
		break;
	case SS02:	/* 禊弾幕が終わるまで待機 */
		src->boss_base_state001++;
		break;
	case SS03:	/* しばし待つ */
		if (0 > src->time_out)
		{
			src->time_out = 100/*150*/;
			/* 規定の繰り返し回数こなしたら退場 */
			src->YOKAI1_DATA_repeat--;
			if ((0) != src->YOKAI1_DATA_repeat) 	{	src->boss_base_state001 = SS01; 			}/* 繰り返し */
			else									{	src->boss_base_state001++;/* = SS04*/		}/* 上へ退場 */
		}
		break;
//---------
	case SS04:	/* 上へ退場 */
		/* 倒した場合背後の魔方陣が消えているが、逃がした場合背後の魔方陣がある。 */
		if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* チェックは実装方式依存。 */
		{
			src->YOKAI1_DATA_s2->y256	-= t256(1.5);	/*fps_factor*/
		}
		src->y256						-= t256(1.5);	/*fps_factor*/
		#if 0/*SDL(左隅座標)*/
		if ( -((src->h128+src->h128)) > src->y256)
		#else/*Gu(中心座標)*/
		if ( 0 > src->y256)
		#endif
		{
			src->type					= SP_DELETE;
			if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* チェックは実装方式依存。 */
			{
				src->YOKAI1_DATA_s2->type	= SP_DELETE;
			}
		}
		break;
	}
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_54_CHOU1+((src->time_out&0x10)>>4);
	}
//
	if (SP_DELETE != src->YOKAI1_DATA_s2->type)/* チェックは実装方式依存。 */
	{
		src->YOKAI1_DATA_s2->m_angleCCW1024 += 3;
		mask1024(src->YOKAI1_DATA_s2->m_angleCCW1024);
	}
}



/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

/*
 (46-24)/2 == 11
*/
	//	s1->x256				= ((l->user_x)*t256(35.0))+t256(40.0);
	//	s1->x256				= ((l->user_x)*t256(32.0))+t256(48.0);
	//	s1->x256				= ((l->user_x)<<(8+5))+t256(48.0);


/*

 64=1*16+48
 80=2*16+48
112=4*16+48
160=7*16+48

*/

global void add_chuu_boss(STAGE_DATA *l)
{
	SPRITE *s1;
	s1									= sprite_add_gu_error();
	if (NULL!=s1/*h*/)/* 登録できた場合のみ */
	{
		SPRITE *s2; 		/* 後ろの魔方陣 */
		s2								= sprite_add_gu_error();
		if (NULL!=s2/*h*/)/* 登録できた場合のみ */
		{
			/* 0ttd dddd
				'0': force only zero.
				't': yokai type. (0-3)
				'd': danmaku type. (0-31)
			*/
			int my_yokai_type;
			my_yokai_type = (((l->user_255_code)>>5)&(4-1));
		//
			obj_send1->x256 			= ((l->user_x)<<(8));
			obj_send1->y256 			= t256(-30.0);
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
					MAKE32RGBA(0x5f, 0xff, 0xaf, 0x7f), 	/* 紫っぽく */
				};
				s2->color32 				= color_table[my_yokai_type];
			}
			s2->x256					= obj_send1->x256;/*+((obj_send1->w128-s2->w128))*/ /*-t256((8.0))*/
			s2->y256					= obj_send1->y256;/*+((obj_send1->h128-s2->h128))*/ /*-t256((8.0))*/
		//
			s1->m_Hit256R				= ZAKO_ATARI16_PNG;
			s1->yokai_type				= my_yokai_type;
			if (YOKAI_TYPE_00_RUMIA == (my_yokai_type))/*ルーミア*/
			{
				s1->type				= TEKI_00_BOSS11;
				s2->x256				-= t256(10);/* 11.50 == (46-23)/2 */
				s2->y256				-= t256( 4);/*	5.50 == (46-35)/2 */
			}
			else	/*妖怪2*/
			{
				s1->type				= TEKI_54_CHOU1;
			}
			s1->flags					|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			if (YOKAI_TYPE_02_YOUKAI01 == (my_yokai_type))/*差分氏*/
			{
				s1->callback_mover			= move_youkai1;/*差分氏*/
			}
			else
			{
				s1->callback_mover			= move_chuu_boss;
			}
		//	s1->callback_loser			= lose_youkai1;
			s1->callback_hit_enemy		= callback_hit_chuu_boss;	/* コールバック登録 */
			s1->x256					= obj_send1->x256;
			s1->y256					= obj_send1->y256;
		//
			s1->base_hp 				= (8*200)+(2/*di fficulty*/<<(4+3));	/*ルーミア*/		/* easyでも存在感を印象づける為に 200 は必要 */ 	// 50+150*di fficulty;
		//	s1->base_hp 				= (8*160)+(1/*di fficulty*/<<(4+3-1));	/*妖怪2*/			/* easyでも存在感を印象づける為に 200 は必要 */ 	// 50+150*di fficulty;
			s1->base_score				= score(100)+score(100)*difficulty;
		//
			s1->boss_base_state001				= SS00;
			s1->YOKAI1_DATA_repeat				= (2+2+1);
			s1->YOKAI1_DATA_start_limit_y256	= ((l->user_y)<<(8));/* t256(50.0) */
			s1->YOKAI1_DATA_s2					= s2;
		//	s1->YOKAI1_DATA_wait2				= (10-1);/*0*/
		//	s1->time_out						= 30;
			/* "妖怪2"&"ルーミア"専用 */
			{
				#if 1
			//------------ 弾幕関連
				s1->boss_base_danmaku_type			= 0;
			//	s1->YOKAI1_DATA_start_danmaku		= ((((l->user_255_code)&0x1f)+0x10/*とりあえずr27互換*/)&0x1f); 	/* "妖怪2"&"ルーミア"専用 */
				s1->YOKAI1_DATA_start_danmaku		= ((l->user_255_code)&0x1f);	/* "妖怪2"&"ルーミア"専用 */
				#endif
			}
		}
		else
		{
			s1->type				= SP_DELETE;/* 登録できなかった場合 */
		}
	}
}
