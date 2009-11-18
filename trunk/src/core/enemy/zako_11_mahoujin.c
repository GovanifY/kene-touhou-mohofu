
#include "bullet_object.h"

/*---------------------------------------------------------
	"魔方陣2"		消える魔方陣
	"魔方陣1"		ばらまき攻撃魔方陣
	-------------------------------------------------------
	地形に関連する敵
---------------------------------------------------------*/

/* 特殊使用 */
#define MAHOU_TEKI_wait1		vx256
#define MAHOU_TEKI_wait2		vy256
#define MAHOU_TEKI_time_out 	tmp_angleCCW512

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
//	case SP_GROUNDER:
	if (rand_percent(30))	{	item_create(src,						SP_ITEM_00_P001,	1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );}
	if (rand_percent(30))	{	item_create(src,/*->x+10, c->y-5,*/ 	SP_ITEM_00_P001,	1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );}
	if (rand_percent(30))	{	item_create(src,/*->x+ 5, c->y+5,*/ 	SP_ITEM_06_TENSU,	1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );}
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
	if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->MAHOU_TEKI_time_out)
	{
		bullet_create_n_way_dan_sa_type(src,
			(t256(2.5)),			/* 弾速 */
			(0),					/* 下向き(上下弾) */
			(int)(512/(2)), 		/* 角度(512[360/360度]を 2 分割) */ 	/* 1周をn分割した角度 */
			BULLET_KOME_02_AKA, 	/* 赤米弾 */
			(2));/* [2way赤米弾] */
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
			src->MAHOU_TEKI_wait1 = 8/*5*/;/*プログラムが速くなってる分遅くする*/
			src->MAHOU_TEKI_wait2--;
			#if (0==USE_DESIGN_TRACK)
			play_voice_auto_track(VOICE10_MAGICF_E_SHOT);
			#else
			voice_play(VOICE10_MAGICF_E_SHOT, TRACK06_ALEART_IVENT_02);/*テキトー*/
			#endif
			/* 速度は 1.0-3.0までの無段階乱数に変更 */
			{	int angle512;
				angle512=(ra_nd()&(512-1))/*%360*/;
				bullet_create_tomari2_dan(src,
					/*t256(sp)*/((ra_nd()&(512-1))+256)/*speed256*/,
					/*degtorad*/(angle512),
					t256(0.04), 	/* 減速定数 */
					ANGLE_NO_SHOT_DAN);
			}
		}
	}
}

/*---------------------------------------------------------
	敵攻撃("魔方陣1")のスペカ
---------------------------------------------------------*/

static void attack_kougeki_mahoujin03(SPRITE *src)
{
	if (
			(src->y256 >= player->y256)
	 || 	(src->y256 > t256(100))
	)
	{
	//	src->MAHOU_TEKI_time_out++;/* = ST01;*/
		src->MAHOU_TEKI_time_out = MAHOU_TIME_LIMIT_01_ADJ; /* 消える */
		bullet_create_aka_maru_jikinerai(src, t256(1) );
	}
//	if (src->MAHOU_TEKI_wait2<(difficulty*7)+10) // [***090128 変更 */
	{
		src->MAHOU_TEKI_wait1--;			/* ばらまき間隔カウンタ */
		if (src->MAHOU_TEKI_wait1 <= 0) 	/* ばらまき間隔カウンタ */
		{
			src->MAHOU_TEKI_wait1 = (25-(difficulty*5)); /* ばらまき間隔カウンタ */
			#if (0==USE_DESIGN_TRACK)
			play_voice_auto_track(VOICE10_MAGICF_E_SHOT);
			#else
			voice_play(VOICE10_MAGICF_E_SHOT, TRACK06_ALEART_IVENT_02);/*テキトー*/
			#endif
			/* 速度は 1.0-4.0までの乱数(ただし上方域に偏差)に変更 */
			bullet_create_tomari2_dan(src,
				((ra_nd()&(1024-1))|(0x100))/*t256(sp)*/,
				/*deg512_2rad*/((ra_nd()&(512-1)))/*de gtorad(ra_nd()%360)*/,
				t256(0.03), 	/* 減速定数 */
				ANGLE_NO_SHOT_DAN);
		}
	}
}

/*---------------------------------------------------------
	円状攻撃のスペカ
---------------------------------------------------------*/

static void attack_kougeki_mahoujin04(SPRITE *src)
{
	/* 攻撃開始フレームから16フレーム経過後に 1回だけ攻撃 */
	//if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->MAHOU_TEKI_time_out)
	//if (0 == (src->MAHOU_TEKI_time_out&0x07))
	{
		bullet_create_tomari2_dan(src,
			(t256(2.5)),			/* 弾速 */
			((src->MAHOU_TEKI_time_out)+((src->MAHOU_TEKI_time_out&0x07)<<6)),					/* 向き */
	//		(int)(512/(7)), 		/* 角度(512[360/360度]を 7 分割) */ 	/* 1周をn分割した角度 */
	//		BULLET_KOME_02_AKA, 	/* 赤米弾 */
	//		(7));/* [7way赤米弾] */
				t256(0.03), 	/* 減速定数 */
				ANGLE_NO_SHOT_DAN);
	}
}

/*---------------------------------------------------------
	円状攻撃のスペカ
---------------------------------------------------------*/

static void attack_kougeki_mahoujin05(SPRITE *src)
{
	/* 攻撃開始フレームから16フレーム経過後に 1回だけ攻撃 */
	//if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->MAHOU_TEKI_time_out)
	//if (0 == (src->MAHOU_TEKI_time_out&0x07))
	{
		bullet_create_tomari2_dan(src,
			(t256(2.5)),			/* 弾速 */
			((-src->MAHOU_TEKI_time_out)+((src->MAHOU_TEKI_time_out&0x07)<<6)),					/* 向き */
	//		(int)(512/(7)), 		/* 角度(512[360/360度]を 7 分割) */ 	/* 1周をn分割した角度 */
	//		BULLET_KOME_02_AKA, 	/* 赤米弾 */
	//		(7));/* [7way赤米弾] */
				t256(0.03), 	/* 減速定数 */
				ANGLE_NO_SHOT_DAN);
	}
}


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
		#if 0
		if (SP_MUTEKI == src->type)
		{			attack_kougeki_mahoujin02(src); }	/* ("魔方陣2") 消える魔方陣 */
		else
	//	if (SP_ZAKO == src->type)
		{			attack_kougeki_mahoujin03(src); }	/* ("魔方陣1") ばらまき攻撃魔方陣 */
		#endif

static void move_kougeki_mahoujin(SPRITE *src)
{
	src->MAHOU_TEKI_time_out--;
	/* 出現 */
	if (MAHOU_TIME_LIMIT_02_ADJ < src->MAHOU_TEKI_time_out)
	{
		src->color32 += 0x03000000; 		/*	src->alpha += 0x03;*/ /*4*/ /**fps_fa ctor*/
	}
	/* 攻撃 */
	else
	if (MAHOU_TIME_LIMIT_01_ADJ < src->MAHOU_TEKI_time_out)
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
	if ( MAHOU_TIME_LIMIT_03_EPS < src->MAHOU_TEKI_time_out )	/*	if ( 0x04 0x00 > (unsigned int)(src->alpha))*/
	{
		src->color32 -= 0x03000000; 	/*	src->alpha += 0x03;*/ /*4*/ /**fps_fa ctor*/
	}
	/* おしまい */
	else
	{
	//	src->color32 = 0x00ffffff;		/*	src->alpha = 0x00;*/
		src->type = SP_DELETE;
	}
//
	src->m_angleCCW512++;
	mask512(src->m_angleCCW512);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_enemy_mahoujin(STAGE_DATA *l)
{
	short	xxx;				xxx 		= l->user_x;
	short	yyy;				yyy 		= l->user_y;
	char	i_code; 			i_code		= l->user_i_code;
	char	user_1_moji;	user_1_moji 	= l->user_1_moji;
//	short speed256; 	speed256	= l->scroll_speed256;
//	data->speed256					= (speed256);
	SPRITE *s;		//	s			= NULL;
	s = NULL;
//
	switch (i_code)
	{
		/* ばらまき攻撃魔方陣
		anim_speed: 1 速い
		anim_speed: 3 遅すぎ
		*/
	case CTYPE_11_MAHOUJIN_A:		/* ばらまき攻撃魔方陣 */	//	"魔方陣1",	/*	"GROUNDER",*/
		s							= sprite_add_res(BASE_TIKEI_GROUNDER08_PNG);	//s->anim_speed =  2/*3*/ /*(5-difficulty)*/ /*1*/;/*9"grounder.png"*/
//		s							= sprite_add_res(BASE_TIKEI_GROUNDER08_PNG);	s->anim_speed	=  3 /*(5-difficulty)*/ /*2*/;/*9"grounder.png"*/
		s->type 					= SP_ZAKO/*SP_TIKEI*/ /*SP_GROUNDER*/;
		s->callback_hit_enemy		= callback_hit_zako;
		s->callback_loser			= lose_kougeki_mahoujin;
//		s->MAHOU_TEKI_wait1 		= 100;	// [***090124		追加
	//	s->MAHOU_TEKI_wait2 		= 0;/* 未使用  */
		break;
	case CTYPE_12_MAHOUJIN_B:		/* 消える魔方陣 */	//	"魔方陣2",	/*	"MAGICF",*/
		s							= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);	//s->anim_speed =  2/*3*/ /*(5-difficulty)*/ /*1*/;/*9"grounder.png"*/
		s->type 					= TEKI_51_MAHOJIN1/*SP_TIKEI*/ /*SP_GROUNDER*/;
//		s->type 					= SP_MUTEKI;
		/*data->base.*/s->base_health	= 999999;/* 倒せない */
	//	s->callback_hit_enemy		= callback_hit_zako;
//		s->MAHOU_TEKI_wait1 		= 10/*+90*/;/* */
	//	s->MAHOU_TEKI_wait2 		= 5/*16+(difficulty<<3)*/ /*10+(7*difficulty)*/;/* 最大五回 */
		break;
	}
	s->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	s->callback_mover				= move_kougeki_mahoujin;
	/* KETMの相対座標指定は廃止。画面座標指定にする */
	s->x256 						= ((xxx)<<8)|(user_1_moji&0x07);/* こっそりインターリーブ */
	s->y256 						= ((yyy)<<8);	/*-((s->h128+s->h128))*/
	/*data->base.*/s->base_score	= score(200*2);
	/*data->base.*/s->base_health	+= 25+(difficulty<<3/*4*/);//+(difficulty*15);
//
	s->MAHOU_TEKI_time_out			= MAHOU_TIME_LIMIT_00_MAX;/* SS00 または ST00 */

	s->MAHOU_TEKI_wait1 			= 10/*+90*/;/* */
	s->MAHOU_TEKI_wait2 			= 5/*16+(difficulty<<3)*/ /*10+(7*difficulty)*/;/* 最大五回 */
	s->color32						= 0x00ffffff;	/*	s->alpha				= 0x02 0x00;*/
//	s->an im_frame					= 0;
}
