
#include "douchu.h"

/*---------------------------------------------------------
	東方模倣風  〜 Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"魔方陣2"		消える魔方陣
	"魔方陣1"		ばらまき攻撃魔方陣
	-------------------------------------------------------
	地形に関連する敵
---------------------------------------------------------*/
#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif

#define time_out				user_data03/*tmp_angleCCW1024*/
#define MAHOU_TEKI_wait1		user_data04
#define MAHOU_TEKI_wait2		user_data05
#define start_y256				user_data06


/* 時間設定 */

//#define MAHOU_TIME_LIMIT_00_MAX	(0x1ff)
//#define MAHOU_TIME_LIMIT_00_MAX	(0x17f)
#define MAHOU_TIME_LIMIT_00_MAX 	(0x13f)
#define MAHOU_TIME_LIMIT_01_ADJ 	((int)(0x0ff/3))
#define MAHOU_TIME_LIMIT_02_ADJ 	(MAHOU_TIME_LIMIT_00_MAX - MAHOU_TIME_LIMIT_01_ADJ)
#define MAHOU_TIME_LIMIT_03_EPS 	(0x04)

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_kougeki_mahoujin(SPRITE *src)
{
	if (rand_percent(30))	{	item_create(src, SP_ITEM_00_P001,	 1, (ITEM_MOVE_FLAG_06_RAND_XY) );}
	if (rand_percent(30))	{	item_create(src, SP_ITEM_00_P001,	 1, (ITEM_MOVE_FLAG_06_RAND_XY) );}
	if (rand_percent(30))	{	item_create(src, SP_ITEM_05_TENSU,	 1, (ITEM_MOVE_FLAG_06_RAND_XY) );}
}

/*---------------------------------------------------------
	敵攻撃しない
---------------------------------------------------------*/

static void attack_kougeki_mahoujin00(SPRITE *src)
{
	/* 攻撃しない */
}
/*---------------------------------------------------------
	縦攻撃のスペカ
---------------------------------------------------------*/

static void attack_kougeki_mahoujin01(SPRITE *src)
{
	/* 攻撃開始フレームから16フレーム経過後に 1回だけ攻撃 */
	if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->time_out)
	{
		obj_send1->x256 					= src->x256;
		obj_send1->y256 					= src->y256;
		br.BULLET_REGIST_speed256			= (t256(2.5));			/* 弾速 */
		br.BULLET_REGIST_angle1024			= (0);					/* 下向き(上下弾) */
		br.BULLET_REGIST_div_angle1024		= (int)(1024/(2));		/* 角度(一周を 2 分割) */	/* 1周をn分割した角度 */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KOME_02_AKA;	/* 赤米弾 */
		br.BULLET_REGIST_n_way				= (2);					/* [2way] */
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_vector();
	}
}

/*---------------------------------------------------------
	敵攻撃("魔方陣2")のスペカ
---------------------------------------------------------*/

static void attack_kougeki_mahoujin02(SPRITE *src)
{
	if (0 < src->MAHOU_TEKI_wait2)		// [***090128		変更
	{
		src->MAHOU_TEKI_wait1--;			/* ばらまき間隔カウンタ */
		if (src->MAHOU_TEKI_wait1 <= 0) 	/* ばらまき間隔カウンタ */
		{
			src->MAHOU_TEKI_wait1 = (8)/*5*/;/*プログラムが速くなってる分遅くする*/
			src->MAHOU_TEKI_wait2--;
			voice_play(VOICE10_MAGICF_E_SHOT, TRACK06_ALEART_IVENT_02);/*テキトー*/
			/* 速度は 1.0-3.0までの無段階乱数に変更 */
			obj_send1->x256 								= (src->x256);
			obj_send1->y256 								= (src->y256);
			br.BULLET_REGIST_speed256						= ((ra_nd()&(512-1))+256);		/* 弾速 */	/*t256(sp)*/
			br.BULLET_REGIST_angle1024						= (ra_nd()&(1024-1));			/* 角度 */
			br.BULLET_REGIST_tomari2_dan_delta256			= t256(0.04);					/* 減速定数 */
			br.BULLET_REGIST_tomari2_dan_next_angle1024 	= ANGLE_NO_SHOT_DAN;
			br.BULLET_REGIST_regist_type					= REGIST_TYPE_03_TOMARI;
			bullet_regist_vector();
		}
	}
}

/*---------------------------------------------------------
	敵攻撃("魔方陣1")のスペカ
---------------------------------------------------------*/

static void attack_kougeki_mahoujin03(SPRITE *src)
{
	if (
			(src->y256 > obj_player->y256)
	 || 	(src->y256 > t256(100))
	)
	{
		obj_send1->x256 					= (src->x256);
		obj_send1->y256 					= (src->y256);
	//	src->time_out++;/* = ST01;*/
		src->time_out						= MAHOU_TIME_LIMIT_01_ADJ; /* 消える */
		br.BULLET_REGIST_speed256			= t256(1.0);				/* 弾速 */
	//	br.BULLET_REGIST_speed256			= speed256; 				/* 弾速 */
		br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* 自機狙い弾 */	/* 発射中心角度 / 特殊機能(自機狙い/他) */
	//	br.BULLET_REGIST_div_angle1024		= (0);						/* ダミー角度(未使用) */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU8_00_AKA;		/* [赤弾] */ /* 弾グラ */
		br.BULLET_REGIST_n_way				= (1);						/* [1way] */
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_vector();
	}
//	if (src->MAHOU_TEKI_wait2<(difficulty*7)+10) // [***090128 変更 */
	{
		src->MAHOU_TEKI_wait1--;			/* ばらまき間隔カウンタ */
		if (src->MAHOU_TEKI_wait1 <= 0) 	/* ばらまき間隔カウンタ */
		{
			src->MAHOU_TEKI_wait1 = (25-(difficulty*5)); /* ばらまき間隔カウンタ */
			voice_play(VOICE10_MAGICF_E_SHOT, TRACK06_ALEART_IVENT_02);/*テキトー*/
			/* 速度は 1.0-4.0までの乱数(ただし上方域に偏差)に変更 */
			obj_send1->x256 								= (src->x256);
			obj_send1->y256 								= (src->y256);

			br.BULLET_REGIST_speed256						= ((ra_nd()&(1024-1))|(0x100)); 	/* 弾速 */	/*t256(sp)*/
			br.BULLET_REGIST_angle1024						= ((ra_nd()&(1024-1))); 			/* 角度 */
			br.BULLET_REGIST_tomari2_dan_delta256			= t256(0.03);	/* 減速定数 */
			br.BULLET_REGIST_tomari2_dan_next_angle1024 	= ANGLE_NO_SHOT_DAN;
			br.BULLET_REGIST_regist_type					= REGIST_TYPE_03_TOMARI;
			bullet_regist_vector();
		}
	}
}

/*---------------------------------------------------------
	円状攻撃のスペカ
---------------------------------------------------------*/

static void attack_kougeki_mahoujin04(SPRITE *src)
{
	/* 攻撃開始フレームから16フレーム経過後に 1回だけ攻撃 */
	//if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->time_out)
	//if (0 == (src->time_out&0x07))
	{
			obj_send1->x256 								= (src->x256);
			obj_send1->y256 								= (src->y256);
			br.BULLET_REGIST_speed256						= (t256(2.5));			/* 弾速 */
			br.BULLET_REGIST_angle1024						= ( (src->time_out+src->time_out)+((src->time_out&0x07)<<7));	/* 向き */
	//		br.BULLET_REGIST_angle1024						= (int)(1024/(7));		/* 角度(一周を 7 分割) */	/* 1周をn分割した角度 */
	//		BULLET_KOME_02_AKA; 	/* 赤米弾 */
	//		(7));/* [7way赤米弾] */
			br.BULLET_REGIST_tomari2_dan_delta256			= t256(0.03);	/* 減速定数 */
			br.BULLET_REGIST_tomari2_dan_next_angle1024 	= ANGLE_NO_SHOT_DAN;
			br.BULLET_REGIST_regist_type					= REGIST_TYPE_03_TOMARI;
			bullet_regist_vector();
	}
}

/*---------------------------------------------------------
	円状攻撃のスペカ
---------------------------------------------------------*/

static void attack_kougeki_mahoujin05(SPRITE *src)
{
	/* 攻撃開始フレームから16フレーム経過後に 1回だけ攻撃 */
	//if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->time_out)
	//if (0 == (src->time_out&0x07))
	{
			obj_send1->x256 								= (src->x256);
			obj_send1->y256 								= (src->y256);
			br.BULLET_REGIST_speed256						= (t256(2.5));			/* 弾速 */
			br.BULLET_REGIST_angle1024						= (-(src->time_out+src->time_out)+((src->time_out&0x07)<<7));	/* 向き */
	//		br.BULLET_REGIST_angle1024						= (int)(1024/(7)),		/* 角度(一周を 7 分割) */	/* 1周をn分割した角度 */
	//		BULLET_KOME_02_AKA, 	/* 赤米弾 */
	//		(7));/* [7way赤米弾] */
			br.BULLET_REGIST_tomari2_dan_delta256			= t256(0.03);	/* 減速定数 */
			br.BULLET_REGIST_tomari2_dan_next_angle1024 	= ANGLE_NO_SHOT_DAN;
			br.BULLET_REGIST_regist_type					= REGIST_TYPE_03_TOMARI;
			bullet_regist_vector();
	}
}


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
		#if 0
		if (S P_MUTEKI == src->type)
		{			attack_kougeki_mahoujin02(src); }	/* ("魔方陣2") 消える魔方陣 */
		else
	//	if (SP_ZAKO == src->type)
		{			attack_kougeki_mahoujin03(src); }	/* ("魔方陣1") ばらまき攻撃魔方陣 */
		#endif

static void move_kougeki_mahoujin(SPRITE *src)
{
	src->time_out--;
	/* 出現 */
	if (MAHOU_TIME_LIMIT_02_ADJ < src->time_out)
	{
		src->color32 += 0x03000000; 		/*	src->alpha += 0x03;*/ /*4*/ /*fps_factor*/
	}
	/* 攻撃 */
	else
	if (MAHOU_TIME_LIMIT_01_ADJ < src->time_out)
	{
		/* スペカ選択 */
		void (*aaa[8])(SPRITE *src) =
		{
			attack_kougeki_mahoujin00,		/* 攻撃しない */
			attack_kougeki_mahoujin01,		/* 縦弾 */
			attack_kougeki_mahoujin02,		/* ばらまき #01(差分氏、旧"魔方陣2") */
			attack_kougeki_mahoujin03,		/* ばらまき #02(差分氏、旧"魔方陣1") */
			attack_kougeki_mahoujin04,		/* 8way花弾 */
			attack_kougeki_mahoujin05,		/* 8way花弾 */
			attack_kougeki_mahoujin04,		/* 8way花弾 */
			attack_kougeki_mahoujin04,		/* 8way花弾 */
		};
		(*aaa[ (int)((src->x256)&0x07) ])(src); 	/* 各関数に分岐する */
	}
	/* 消える */
	else
	if ( MAHOU_TIME_LIMIT_03_EPS < src->time_out )	/*	if ( 0x04 0x00 > (unsigned int)(src->alpha))*/
	{
		src->color32 -= 0x03000000; 	/*	src->alpha += 0x03;*/ /*4*/ /*fps_factor*/
	}
	/* おしまい */
	else
	{
	//	src->color32 = 0x00ffffff;		/*	src->alpha = 0x00;*/
		src->type = SP_DELETE;
	}
//
	src->y256 = src->start_y256 + ((cos1024(((src->time_out<<5)&(1024-1))))<<2);

//	src->m_angleCCW1024++;
//	mask1024(src->m_angleCCW1024);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_enemy_mahoujin(STAGE_DATA *l)
{
	SPRITE *h;		//	h			= NULL;
	h									= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		if (CTYPE_19_MAHOUJIN_A==((char)l->user_i_code))
		{		/* ばらまき攻撃魔方陣 */	//	"魔方陣1",	/*	"GROUNDER",*/
			h->m_Hit256R				= ZAKO_ATARI04_PNG;
			h->type 					= TEKI_58_RED_BOOK; 	/*SP_ZAKO*/ /*SP_TIKEI*/ /*SP_GROUNDER*/
			h->callback_hit_enemy		= callback_hit_zako;
			h->callback_loser			= lose_kougeki_mahoujin;
	//		h->MAHOU_TEKI_wait1 		= 100;	// [***090124		追加
		//	h->MAHOU_TEKI_wait2 		= 0;/* 未使用  */
		}
		else	//CTYPE_20_MAHOUJIN_B:		/* 消える魔方陣 */	//	"魔方陣2",	/*	"MAGICF",*/
		{
			h->m_Hit256R				= ZAKO_ATARI16_PNG;
			h->type 					= TEKI_52_MAHOJIN;	/*SP_TIKEI*/ /*SP_GROUNDER*/
//			/*data->base.*/h->base_hp	= (9999);/* 倒せない */
		//	h->callback_hit_enemy		= callback_hit_zako;
	//		h->MAHOU_TEKI_wait1 		= 10/*+90*/;/* */
		//	h->MAHOU_TEKI_wait2 		= 5/*16+(difficulty<<3)*/ /*10+(7*difficulty)*/;/* 最大五回 */
		}
		h->callback_mover				= move_kougeki_mahoujin;
		/* KETMの相対座標指定は廃止。画面座標指定にする */
		h->x256 						= (((short)l->user_x)<<8)|(((char)l->user_255_code)&0x07);/* こっそりインターリーブ */
		h->start_y256					= (((short)l->user_y)<<8);
	//
		h->time_out 					= MAHOU_TIME_LIMIT_00_MAX;/* SS00 または ST00 */

		h->MAHOU_TEKI_wait1 			= (10); 	/*+90*/ /* */
		h->MAHOU_TEKI_wait2 			= (5);		/* 最大五回 */	/*16+(difficulty<<3)*/ /*10+(7*difficulty)*/
		h->color32						= 0x00ffffff;	/*	h->alpha				= 0x02 0x00;*/
	}
}
//		/*data->base.*/h->base_hp		+= (8*25)+(1/*di fficulty*/<<(3/*4*/+3));	//+(di fficulty*15)
//		/*data->base.*/h->base_score	= score(200*2);
