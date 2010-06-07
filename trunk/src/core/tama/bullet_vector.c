
#include "game_main.h"

/*---------------------------------------------------------
  東方模倣風  ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	移動量弾
---------------------------------------------------------*/

/*---------------------------------------------------------
	弾幕作成 / 爆発作成
	-------------------------------------------------------
	後で変化する弾を作るかも。
	ボスを倒した時に消えるようにしてある。
	追加した弾
	// [***090114	大弾の動きを変えてみた
	-------------------------------------------------------
	もちついて本家見直してみる。
	本家の弾幕に「変身機能なんて無い」。
	単に弾幕リスト見て、弾幕番号で書き換えてる気がしてきた。

	そもそも現状の方式では弾幕リスト書き換えは、
	出来ない事もないが、かなりめんどくさいので、
	弾幕リストをシンプルな構造にする方が先。(data、mal locしない)
	-------------------------------------------------------

■ 弾幕システム(研究中)。
 1.弾の変身:
 弾の変身は、今まで弾が変身するシステムかと思っていたが、本家の弾にそんな機能はない(たぶん)。
弾「が」変身するんじゃなくて、弾「を」変身させる。この違いは大きい。
自分から変身する弾は要らない。弾幕システムに弾の変身機能は持たせない。(外部から強制的に変身させる)

例えば模倣風咲夜の「くるくるナイフ」で説明すると、

現在の模倣風:
	咲夜が「くるくるナイフ弾」という特殊弾を投げる。
	「くるくるナイフ弾」は減速弾。
	速度が一定以下になると自分で(わざと狙わない)自機狙い弾に変身(緑ナイフ)する。
	自機狙い弾として移動(するがあくまで特殊弾)。

変更後:
	咲夜が「通常弾」を投げる。(例えば通常弾、青ナイフとまったく同じもの、
	このまま何もしなければ通常弾、青ナイフとまったく同じ。あたりまえだけど)
	「通常弾」はただの減速弾。
	一定フレーム(弾とは関係ない場所で計った時間で)経過すると、咲夜が「弾幕チェックイベント発生」させる。
	咲夜が「弾幕リストを検索」し、条件(例えば速度が一定以下の青ナイフ)に一致したものを変身させる。
	変身後もただの(わざと狙わない)自機狙い「通常弾」。
	(通常弾緑ナイフとまったく同じもの)
	-------------------------------------------------------
	条件は色々ある。例えば発生から一定時間経過した弾とか。

現在の模倣風は特殊弾が多すぎる。本家の弾には特殊弾は一つも無い(たぶん)。
こういう方式でないと、妖夢の大玉切りみたいのは遅くなりすぎて実装できない(たぶん)。
(弾がボスがちかずいてきたのを察知して弾けるのでは、画面上の全弾に毎フレームその処理が入るので速度が遅くなりすぎる。
ボスが「弾幕リストを検索」し、自分に距離が近い弾だけ切るのなら、一回で済むので十分速い。
その処理が万一とても遅かった場合でも1フレームなので全体的に問題が出ない)

 2.弾の基点:
 総ての弾には基点と発生時間があり、単純な関数で現在位置を一度で割り出している気がする。

---------------------------------------------------------*/

global BULLET_REGIST br;

#define callback_2nd_regist 	callback_loser	/* ２nd登録 */




/*---------------------------------------------------------
	とりあえず移行用(2)追加弾、基本機能
---------------------------------------------------------*/

#define BULLET_DATA_2nd_count	user_data10 	/* ２nd 変身 */
#define BULLET_DATA_2nd_speed	user_data11 	/* ２nd 速度 */
#define BULLET_DATA_2nd_vx256	user_data12 	/* ２nd vx256 */
#define BULLET_DATA_2nd_vy256	user_data13 	/* ２nd vy256 */



/*---------------------------------------------------------
	２nd 変身登録、基本版
---------------------------------------------------------*/

static void callback_2nd_regist(SPRITE *src)
{
	src->BULLET_DATA_2nd_count = 9999;				/* 3次変身しない */
	src->vx256	= (src->BULLET_DATA_2nd_vx256); 	/* ２nd vx256 */
	src->vy256	= (src->BULLET_DATA_2nd_vy256); 	/* ２nd vy256 */
}




/*---------------------------------------------------------
	ベクトル弾(角度変化しないものはこの方式の方が速い)
---------------------------------------------------------*/

/* 基本弾(速度、角度、ともに変化しない) */
#define BASE_add_speed256						user_data07 	/* 加速度 */


/* 応用弾(速度変化する) */
//typedef struct
//{
#define TOMARI_DAN_DATA_speed256				user_data00 	/* 速度 */
#define TOMARI_DAN_DATA_delta256				user_data01 	/* 加速度 */
//
#define TOMARI_DAN_DATA_init_vx256				user_data02 	/* 設定初期速度 */
#define TOMARI_DAN_DATA_init_vy256				user_data03 	/* 設定初期速度 */
//} TOMARI_DAN_DATA;


/* 基本弾(速度、角度、ともに変化しない) */
//typedef struct
//{
#define GRAVITY_BULLET_DATA_delta256			user_data01 	/* 加速度(y軸成分のみ) */
//} GRAVITY_BULLET_DATA;


//typedef struct
//{
#define HAZUMI_DAN_DATA_delta256				user_data01 	/* 加速度(y軸成分のみ) */
#define HAZUMI_DAN_DATA_bound_counts			user_data02 	/* 跳ねる予定の回数(0で終わり) */
//} HAZUMI_DAN_DATA;

/*---------------------------------------------------------
	ベクトル展開弾
	-------------------------------------------------------
	指定座標(発弾からの相対位置)まで比率で動き、
	そこからはベクトル弾
---------------------------------------------------------*/



/*---------------------------------------------------------
	角度弾(角度変化するもの)
	-------------------------------------------------------
	角度変化するもの(交差弾)は、数フレームごと、角度変化計算しなおしてるっぽい。(?)
---------------------------------------------------------*/

/* 応用弾 */

//typedef struct
//{
#define KURUKURU_KNIFE_DATA_speed_2nd_256		user_data00
#define KURUKURU_KNIFE_DATA_height				user_data02
//} KURUKURU_KNIFE_DATA;


//typedef struct
//{
#define BULLET_SP1_DATA_angle1024				user_data00
#define BULLET_YUUDOU_DATA_hiritu256			user_data01
#define target_x256 							user_data02
#define target_y256 							user_data03
//} BULLET_SP1_DATA;


#define BULLET_HOSI_GATA_DATA_angle1024 		user_data00
#define BULLET_HOSI_GATA_DATA_speed256			user_data01 		/* 速度 */
#define BULLET_HOSI_GATA_DATA_add_delta256		user_data02 		/* 加算、加速度 */
#define BULLET_HOSI_GATA_DATA_star_remain_time	user_data03


/*---------------------------------------------------------
	速度値、角度値、をベクトル量に変換する
---------------------------------------------------------*/

static void callback_2nd_regist(SPRITE *src);
static void regist_vector(SPRITE *src, int speed256, int angle1024)
{
	src->vx256	= ((sin1024((angle1024))*(speed256))>>8);	/*fps_factor*/
	src->vy256	= ((cos1024((angle1024))*(speed256))>>8);	/*fps_factor*/
//
	#if 0
	/* 弾速が遅すぎる場合に修正する */
	if (( (8) > abs(src->vx256)) )	{ src->vx256 = (0<src->vx256)?( (8)):( (-8)); }
	if (( (8) > abs(src->vy256)) )	{ src->vy256 = (0<src->vy256)?( (8)):( (-8)); }
	#endif
//
	/* とりあえず半分速にする */
	src->BULLET_DATA_2nd_vx256	= ((src->vx256)>>1);	/*fps_factor*/
	src->BULLET_DATA_2nd_vy256	= ((src->vy256)>>1);	/*fps_factor*/
//
	/* とりあえず、基本版 */
	src->callback_2nd_regist	= callback_2nd_regist;
//
	/* とりあえず、テキトー */
	src->BULLET_DATA_2nd_count	= (30); 			/* 30カウントしたら半分速 */
//	src->BASE_add_speed256		= (t256(1.00)); 	/* 加速/減速、なし */
	src->BASE_add_speed256		= 1+(t256(1.00));	/* 加速 弾 */

}

/*---------------------------------------------------------
	基本量移動、画面外は消える
	喰み出しチェック機能はないので別でやる事
---------------------------------------------------------*/

static void move_bullet_vector(SPRITE *src)
{
	if (t256(1.00)!=(src->BASE_add_speed256))
	{
		src->vx256 = ((src->vx256*src->BASE_add_speed256)>>8);/* 加速/減速 */
		src->vy256 = ((src->vy256*src->BASE_add_speed256)>>8);/* 加速/減速 */
	}
	src->x256 += (src->vx256);		/*fps_factor*/
	src->y256 += (src->vy256);		/*fps_factor*/
//
//	gamen_gai_nara_osimai(src);/* 画面外ならおしまい */

	#if 1/* (無くなる可能性が高い予定) */
	src->BULLET_DATA_2nd_count--;		/*fps_factor*/
	if (0 > src->BULLET_DATA_2nd_count)
	{
		if (NULL != src->callback_2nd_regist)
		{
			(src->callback_2nd_regist)(src);
			src->callback_2nd_regist = NULL;
		}
	}
	#endif
}


/*---------------------------------------------------------
	咲夜の直線誘導ナイフ弾
	-------------------------------------------------------
	弾幕書き換え方式に移行するのでここは無くなる予定。
---------------------------------------------------------*/

#if 1
//static int angle_jikinerai512_auto(SPRITE *p, SPRITE *t, int angle512);
static void s_tmp_angleCCW1024_jikinerai_auto(SPRITE *p, SPRITE *t/*, int angle512*/);
static void move_bullet_sakuya_linear_yuudou(SPRITE *src)
{
	src->base_time_out--;
	if (0 > src->base_time_out) 				/* 等速運動 */
	{
		/* 移動速度 t256(1.0) で等速移動 */
		src->vx256 = ((sin1024((src->BULLET_SP1_DATA_angle1024)) ));/*fps_factor*/
		src->vy256 = ((cos1024((src->BULLET_SP1_DATA_angle1024)) ));/*fps_factor*/
	}
	else
	if (0+2048 > src->base_time_out)			/* 誘導運動 */
	{
		const u8 m_tbl[4] =
		{
		//	0,1,3,7,	/* 遅い方が難しい */
			1,0,7,3,	/* ほかの攻撃との兼ね合いで、タイミングをとりづらい方が難しい */
		};
		/* 自分に集まる */
	//	if (0==(src->base_time_out & (7>>difficulty) ))
	//	if (0==(src->base_time_out & (7>>(3-difficulty)) ))/* 遅い方が難しい */
		if (0==(src->base_time_out & (m_tbl[difficulty]) ))
		{
			src->BULLET_YUUDOU_DATA_hiritu256--;
		}
	//	if (240/*1*/ < data->y_sum256)
		{	/* 差分 == (弾の現在座標 - 弾の誘導座標) */
			int x_sa256 = (src->x256 - src->target_x256);
			int y_sa256 = (src->y256 - src->target_y256);
			/* ある程度、誘導座標の近くまできたら */
			if (t256(2.0) > abs(x_sa256))
			{
				if (t256(2.0) > abs(y_sa256))
				{
					src->base_time_out = 0; 	/* 等速運動へ */
				}
			}
			/* 加算差分 == (弾の差分座標 * 誘導比率) */
			int aaa_x256 = ((x_sa256 * src->BULLET_YUUDOU_DATA_hiritu256)>>8);	/*fps_factor*/
			int aaa_y256 = ((y_sa256 * src->BULLET_YUUDOU_DATA_hiritu256)>>8);	/*fps_factor*/
			src->x256 = src->target_x256 + (aaa_x256);	/*fps_factor*/
			src->y256 = src->target_y256 + (aaa_y256);	/*fps_factor*/
		}
	}
	else
	if (0+2048==src->base_time_out) 			/* 狙い弾に変身する */
	{
	//	src->type			= BULLET_MARU8_00_AKA+(1);
		src->type			= BULLET_KNIFE20_07_MIDORI;/* (青→緑ナイフに変身) */
		src->BULLET_SP1_DATA_angle1024 = ANGLE_JIKI_NERAI_DAN;
		{
			src->tmp_angleCCW1024			= src->BULLET_SP1_DATA_angle1024;
			s_tmp_angleCCW1024_jikinerai_auto(obj_player, src);
			src->BULLET_SP1_DATA_angle1024	= src->tmp_angleCCW1024;
		}
		mask1024(src->BULLET_SP1_DATA_angle1024);
		src->m_angleCCW1024 = src->BULLET_SP1_DATA_angle1024;
		src->vx256 = (0);	/*fps_factor*/
		src->vy256 = (0);	/*fps_factor*/
		/* 誘導座標はプレイヤーの位置付近 */
		/* とりあえず、仮対応。強制的に設定しちゃう */
		src->target_x256	= (obj_player->x256+t256(32/2)/*幅の半分*/);		/*fps_factor*/
		src->target_y256	= (obj_player->y256+t256(16/2)/*高さの半分*/);	/*fps_factor*/
		src->BULLET_YUUDOU_DATA_hiritu256		= t256(1.0);	/* (2.5==5*0.5) */
	}
//
	move_bullet_vector(src);
}
#endif



/*---------------------------------------------------------
	咲夜の		B.星型を描く魔方陣用
	-------------------------------------------------------
	弾幕書き換え方式に移行するのでここは無くなる予定。
---------------------------------------------------------*/

static void move_bullet_sakuya_hosi_gata(SPRITE *src)
{
	{
	//	if (2==data->sdata->nnn/*%(4-difficulty)*/)/* ステートが1の時配置して、ステートが2になったら動く */
	//	if (2==(*(data->sd_nnn))/*%(4-difficulty)*/)/* ステートが1の時配置して、ステートが2になったら動く */
	//	if (1==(*(data->sd_nnn))/*%(4-difficulty)*/)/* ステートが0の時配置して、ステートが1になったら動く */
		src->BULLET_HOSI_GATA_DATA_star_remain_time--;
		if (0 > (src->BULLET_HOSI_GATA_DATA_star_remain_time))/* 動く */
		{
			src->BULLET_HOSI_GATA_DATA_star_remain_time = 0;
			src->BULLET_HOSI_GATA_DATA_speed256 += (src->BULLET_HOSI_GATA_DATA_add_delta256); //(4) (3.84) == t256(0.015);
			src->vx256 = ((sin1024((src->BULLET_HOSI_GATA_DATA_angle1024))*(src->BULLET_HOSI_GATA_DATA_speed256))>>8);/*fps_factor*/	/* CCWの場合 */
			src->vy256 = ((cos1024((src->BULLET_HOSI_GATA_DATA_angle1024))*(src->BULLET_HOSI_GATA_DATA_speed256))>>8);/*fps_factor*/
		}
	}
	//move_bullet_vector(src);
	src->x256 += (src->vx256);		/*fps_factor*/
	src->y256 += (src->vy256);		/*fps_factor*/
//	gamen_gai_nara_osimai(src);/* 画面外ならおしまい */
}

/*---------------------------------------------------------
	自機狙い弾の角度を計算
---------------------------------------------------------*/
static void s_tmp_angleCCW1024_jikinerai(SPRITE *p, SPRITE *t)
{
	#if 1
	t->tmp_angleCCW1024 =	(atan_1024((p->y256-t->y256), (p->x256-t->x256)));
	/* 偶数弾の場合に自機狙い(簡略版) 弾と自分が大きさが同じならずれない、違うとその分誤差になる */
	#endif
}
//static int angle_jikinerai1024_auto(SPRITE *p, SPRITE *t, int angle1024)
//{
//	if (ANGLE_JIKI_NERAI_DAN==angle1024)	{	angle1024	= angle_jikinerai1024(p,t);}
//	return (angle1024);
//}
static void s_tmp_angleCCW1024_jikinerai_auto(SPRITE *p, SPRITE *t/*, int angle1024*/)
{
	if (ANGLE_JIKI_NERAI_DAN==t->tmp_angleCCW1024/*angle1024*/)
	{
		s_tmp_angleCCW1024_jikinerai(p, t);
	//	angle1024 = t->tmp_angleCCW1024;
	}
//	return (angle1024);
}

/*---------------------------------------------------------

	-------------------------------------------------------
	弾幕書き換え方式に移行するのでここは無くなる予定。
---------------------------------------------------------*/
#define TOMARI_DAN_next_angle1024	user_data04/*tmp_angleCCW1024*/
#define TOMARI_DAN_LIMIT_00_640 	(512+128)
#define TOMARI_DAN_LIMIT_00_576 	(512+64)
#define TOMARI_DAN_LIMIT_01_512 	(512+0)
#define TOMARI_DAN_LIMIT_02_000 	(0+0)
//	int speed256,
//	int angle512//,
//	int ransu512//,
//	int add_speed256/*, int xoffs256, int yoffs256*/

static void bullet_create_oodama22(SPRITE *src);
//atic void enemy_stop_bullet_move(SPRITE *src)
static void enemy_stop_bullet2_move(SPRITE *src)
{
	src->base_time_out--;
	if (TOMARI_DAN_LIMIT_00_640 < src->base_time_out)
	{
		/* 止まる */
		src->TOMARI_DAN_DATA_speed256 -= (src->TOMARI_DAN_DATA_delta256);
		src->vx256 = (((src->TOMARI_DAN_DATA_init_vx256)*src->TOMARI_DAN_DATA_speed256)>>8);/*fps_factor*/
		src->vy256 = (((src->TOMARI_DAN_DATA_init_vy256)*src->TOMARI_DAN_DATA_speed256)>>8);/*fps_factor*/
		if (src->TOMARI_DAN_DATA_speed256 < t256(0))
		{
			src->TOMARI_DAN_DATA_speed256 = t256(0);
			src->base_time_out = (TOMARI_DAN_LIMIT_00_640);
		}
	}
	else
	if (TOMARI_DAN_LIMIT_00_576 < src->base_time_out)
	{
		/* 撃つ(大弾に変身) */
		src->type		= BULLET_MARU8_00_AKA + (( src->base_time_out>>3)&0x07);		/*警告(変身するので)*/
	}
	else
	if (TOMARI_DAN_LIMIT_01_512 < src->base_time_out)
	{
		if (ANGLE_NO_SHOT_DAN != src->TOMARI_DAN_next_angle1024 )
		{
			bullet_create_oodama22(src);
			src->base_time_out = (TOMARI_DAN_LIMIT_02_000);
		}
		else
		{
			src->base_time_out = (TOMARI_DAN_LIMIT_01_512);
		}
	}
	else
	if (TOMARI_DAN_LIMIT_02_000 < src->base_time_out)
	{
		;	/* しばし停止 */
	}
	else
	{
		/* 消える */
		src->color32 -= 0x10000000; 		/*	src->alpha -= 0x10; */
		if ( 0x20000000 > src->color32) 	/*	( 0x20 > src->alpha)	*/
		{
			src->color32 = 0x00ffffff;		/*	src->alpha = 0x00;	*/
			src->type = SP_DELETE;
		}
	}
	/* 特別に撃たない弾、の場合 */
	if (ANGLE_NO_SHOT_DAN == src->TOMARI_DAN_next_angle1024 )
	{	/* スクロールする */
		src->y256 += t256(1.0); 	//	my_adjust_y(s,pd);
	}
	move_bullet_vector(src);
}

/*---------------------------------------------------------
	輝夜、最終形態で投げてくるかなり無茶な弾。
	-------------------------------------------------------
	難しすぎるので、色々弱体化www。
	でも、まだ難しい。
---------------------------------------------------------*/

/* 90/360度回転 */
//static void rot_90_angle512(HAZUMI_DAN_DATA *d)
//{
//	src->HAZUMI_DAN_DATA_angle512	= at an_512(sin512((src->HAZUMI_DAN_DATA_angle512)),-cos512((src->HAZUMI_DAN_DATA_angle512)));
//}
static void bullet_move_hazumi_dan(SPRITE *src)
{
	if (/*src->HAZUMI_DAN_DATA_sum256*/src->vy256 < t256(15) )	/* 最大重力加速度 */
	{
		/*src->HAZUMI_DAN_DATA_sum256*/src->vy256 += src->HAZUMI_DAN_DATA_delta256; /* 補正値 */
	}
//	s->y256 += src->HAZUMI_DAN_DATA_sum256; 		/* 積算重力加速度 */
//	mono_angle_move(s,(BULLET_ANGLE_DATA *)d);
	src->y256 += src->vy256;
	src->x256 += src->vx256;

	/* 横の壁 */
	if ((src->x256 < t256(0))||(src->x256 > t256(GAME_WIDTH)))
	{
		#if 1
		src->vx256 = -(src->vx256);
	//	rot_90_angle512(d); 	/*	跳ね返る */
		#else
		src->type = SP_DELETE;		/*	消える */
		#endif
	}
	/* 床 */
	else if (src->y256 > t256(GAME_HEIGHT))
	{
		/* 設定跳ねる回数 */
		if (0!=src->HAZUMI_DAN_DATA_bound_counts)
		{
			src->HAZUMI_DAN_DATA_bound_counts--;	/* 跳ねる回数を減らす */
			src->type		= BULLET_MARU8_00_AKA+4+(src->HAZUMI_DAN_DATA_bound_counts);		// [***090116		若干変更
		//	src->HAZUMI_DAN_DATA_sum256 	= -src->HAZUMI_DAN_DATA_sum256; 	/* 重力加速度 */
			#if 0
			src->HAZUMI_DAN_DATA_speed256	= -src->HAZUMI_DAN_DATA_speed256;		/* 速度 */
			src->HAZUMI_DAN_DATA_speed256	+= src->HAZUMI_DAN_DATA_speed256;		/* 加速(して天井へ逃がす) */
		//	rot_90_angle512(d); 		// [***090116		追加
			#else
			src->vy256	= -(src->vy256);	/* 速度 */
			src->vy256	+= (src->vy256>>2); /* 1.25倍に 加速(して天井へ逃がす) */
			src->vx256	+= src->vx256;		/* 横に */
			#endif
		}
		else
		{
			src->type = SP_DELETE;		/*	消える */
		}
	}
	#if 1
	/* 天井 */
	else if (src->y256 < 0)
	{
		src->type = SP_DELETE;		/*	消える */
	}
	#endif
}

/*---------------------------------------------------------
	重力弾の移動
	垂直降下ナイフ(赤)
---------------------------------------------------------*/

static void bullet_move_gravity(SPRITE *src)
{
	src->vy256 += src->GRAVITY_BULLET_DATA_delta256;
//	data->speed256 += data->delta256;
//	src->y256 += data->speed256;
	move_bullet_vector(src);
}

/*---------------------------------------------------------
	咲夜
	プレイヤーの周りをくるくる回ってから、自分狙いのナイフ。
	青→緑に色が変わる。
	-------------------------------------------------------
	弾幕書き換え方式に移行するのでここは無くなる予定。
---------------------------------------------------------*/
#define knife_tmp_angle1024 user_data04/*tmp_angleCCW1024*/

static void bullet_move_kurukuru_knife(SPRITE *src)
{
	src->base_time_out--;
	if ( 0 > src->base_time_out)
	{
		if (((src->x256 < t256(0))||(src->x256 > t256(GAME_WIDTH))||
			 (src->y256 < t256(0))||(src->y256 > t256(GAME_HEIGHT))))
		{
			src->type = SP_DELETE;/* 画面外に出たらおしまい */
		}
	}
	else
	if (250 > src->base_time_out)
	{
		;
	}
	else
	if (250 == src->base_time_out)
	{
		;
	//	if (src->KURUKURU_KNIFE_DATA_aaa_speed256 <= t256(0) )
		{
		//	src->KURUKURU_KNIFE_DATA_aaa_speed256	= src->KURUKURU_KNIFE_DATA_speed_2nd_256;
		//	src->KURUKURU_KNIFE_DATA_target 		= 1;
			src->base_time_out						= (250);
			src->type								= BULLET_KNIFE20_07_MIDORI;/* (青→緑ナイフに変身) */
			src->tmp_angleCCW1024 = atan_1024(
				#if 1/*Gu(中心座標)*/
				(obj_player->y256)-(src->y256)+(src->KURUKURU_KNIFE_DATA_height*256),
				(obj_player->x256)-(src->x256)
				#endif
			);
			mask1024(src->tmp_angleCCW1024);/*ねんのため*/
			src->m_angleCCW1024 = ((src->tmp_angleCCW1024));	/* 表示角度 */
		}
		/*data->*/src->vx256 = ((sin1024((src->tmp_angleCCW1024))*(src->KURUKURU_KNIFE_DATA_speed_2nd_256))>>8);/*fps_factor*/
		/*data->*/src->vy256 = ((cos1024((src->tmp_angleCCW1024))*(src->KURUKURU_KNIFE_DATA_speed_2nd_256))>>8);/*fps_factor*/
	}
	else
//	if (0==src->KURUKURU_KNIFE_DATA_target)
	{
//		mono_angle_move(s,(BULLET_ANGLE_DATA *)d);

	//	src->KURUKURU_KNIFE_DATA_aaa_speed256 -= t256(0.02);
	//	src->KURUKURU_KNIFE_DATA_aaa_speed256 -= t256(0.02*2);/*2倍にしたので辻褄合わせ*/
//		src->KURUKURU_KNIFE_DATA_aaa_speed256 -= t256(0.04);	/* ナイフ速度、減速定数 */
	//	src->KURUKURU_KNIFE_DATA_aaa_speed256 -= 1/*t256(0.04)*/; /* ナイフ速度、減速定数 */
		src->m_angleCCW1024 += (64);
		mask1024(src->m_angleCCW1024);/*ねんのため*/
//		/*data->*/src->vx256 = ((sin512((src->knife_tmp_angle512/*data->aaa_angle512*/))*(src->KURUKURU_KNIFE_DATA_aaa_speed256))>>8);/*fps_factor*/
//		/*data->*/src->vy256 = ((cos512((src->knife_tmp_angle512/*data->aaa_angle512*/))*(src->KURUKURU_KNIFE_DATA_aaa_speed256))>>8);/*fps_factor*/
	}
	src->x256 += (src->vx256);		/*fps_factor*/
	src->y256 += (src->vy256);		/*fps_factor*/
//
}



/*---------------------------------------------------------
	重力弾。アリスの人形が撃つ弾。
	-------------------------------------------------------
	垂直降下ナイフ(赤)
	-------------------------------------------------------
	下方向に加速していく中弾。
	フレーム毎で計算しているのでかなり重力加速度を小さくしないと大変なことに。
	delta256	フレーム毎に下方向にどれだけ加速するか
---------------------------------------------------------*/
//SPRITE *src, int speed256, int angle512, int delta256, int bullet_obj_type);

//	SPRITE *src,
//	int br.BULLET_REGIST_speed256,
//	int br.BULLET_REGIST_angle1024,
//	int BULLET_REGIST_jyuryoku_dan_delta256,
//	int br.BULLET_REGIST_bullet_obj_type	/* 弾グラ */	/*BULLET_MARU8_00_AKA+(7)*/ 	/*(BULLET_KNIFE20_05_AKA)*/

/*bullet_create_sakuya_no_rot_knife*/
static void s_bullet_create_jyuryoku_dan(void)
{
	SPRITE *h;			/*丸弾８(赤ＲＧＢ緑若黄青)*/
	h					= sprite_add_only_bullet_error();/*青弾*/
	if (NULL != h)
	{
		h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
		h->type 			= (br.BULLET_REGIST_bullet_obj_type);/*BULLET_MARU8_07_AOI*/  /*S P_BULLET*/ /*SP_LASER*/
		h->callback_mover	= bullet_move_gravity;	/*enemy_fall_knife_move*/
		#if 1/*Gu(中心座標)*/
		h->x256 			= /*src*/obj_send1->x256;
		h->y256 			= /*src*/obj_send1->y256;
		#endif
	//
		/*angle512			= angle_jikinerai512_auto(obj_player, src, angle512);*/
		mask1024(br.BULLET_REGIST_angle1024);
	//	data->angle512		= (angle512);
	//	data->speed256		= (speed256);
		regist_vector(h, br.BULLET_REGIST_speed256, br.BULLET_REGIST_angle1024);
		h->GRAVITY_BULLET_DATA_delta256 	= (br.BULLET_REGIST_jyuryoku_dan_delta256);
	//	data->speed256		= t256(0);
	}
}


/*---------------------------------------------------------
	咲夜の直線誘導ナイフ
	-------------------------------------------------------
	この弾は他の弾が速いのであえて遅くする。
---------------------------------------------------------*/

global void bullet_crate_sakuya_linear_yuudou(SPRITE *src)
{
	int j;
	for (j=(0); j<(8); j++)
	{
		SPRITE *h;
		h						= sprite_add_only_bullet_error();
		if (NULL != h)
		{
			h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
			const unsigned int aaa = ((ra_nd()&(4-1))); 	/* ナイフの色はランダム */
		//	h->type 			= BULLET_MARU8_00_AKA+4+(aaa);/*S P_BULLET*/ /*SP_LASER*/
			h->type 			= BULLET_KNIFE20_04_AOI+(aaa);/*S P_BULLET*/ /*SP_LASER*/
			h->callback_mover	= move_bullet_sakuya_linear_yuudou;
			#if 1/*Gu(中心座標)*/
			h->x256 			= src->x256;
			h->y256 			= src->y256;
			#endif
			h->base_time_out	= 2048+20+(40+1);		/*data->timer		= 0;*/

			int angle1024		= src->tmp_angleCCW1024+((j-4)*84); 	/* 85.333 == cv1024r(30) */
			mask1024(angle1024);
			h->m_angleCCW1024 = angle1024;
			h->vx256 = ((sin1024((angle1024))*t256(1.0))>>8);/*fps_factor*/
			h->vy256 = ((cos1024((angle1024))*t256(1.0))>>8);/*fps_factor*/

			h->BULLET_YUUDOU_DATA_hiritu256 	= t256(1.0);	/* (2.5==5*0.5) */
		//
			#if 1
			/* とりあえず、基本版 */
		//	h->callback_2nd_regist	= callback_2nd_regist;
		//
			/* とりあえず、テキトー */
			h->BULLET_DATA_2nd_count	= (999/*30*/);		/* 30カウントしたら半分速 */
			h->BASE_add_speed256		= (t256(1.00)); 	/* 加速/減速、なし */
			#endif
		}
	}
}

#if 1/* 星型を描く魔方陣用 */
/*---------------------------------------------------------
	咲夜の		B.星型を描く魔方陣用
	-------------------------------------------------------
	弾幕書き換え方式に移行するのでここは無くなる予定。
---------------------------------------------------------*/

global /*static*/ void bullet_crate_sakuya_hosi_gata(SPRITE *src)
{		/*丸弾８(赤ＲＧＢ緑若黄青)*/
	SPRITE *h;
	h					= sprite_add_only_bullet_error();
	if (NULL != h)
	{
		h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
		h->type 			= BULLET_MARU8_00_AKA+(0);/*S P_BULLET*/
		h->callback_mover	= move_bullet_sakuya_hosi_gata;
		#if 1/*Gu(中心座標)*/
		h->x256 			= src->x256;
		h->y256 			= src->y256;
		#endif
		h->vx256			= (0);
		h->vy256			= (0);
		h->BULLET_HOSI_GATA_DATA_star_remain_time	= (br.BULLET_REGIST_hosi_gata_time_out);
		h->BULLET_HOSI_GATA_DATA_angle1024			= (br.BULLET_REGIST_hosi_gata_angle1024);
		h->BULLET_HOSI_GATA_DATA_speed256			= t256(0);
		h->BULLET_HOSI_GATA_DATA_add_delta256		= (br.BULLET_REGIST_hosi_gata_add_speed256);

	/*特殊*/
	//
		#if 1
		/* とりあえず、基本版 */
	//	h->callback_2nd_regist	= callback_2nd_regist;
	//
		/* とりあえず、テキトー */
		h->BULLET_DATA_2nd_count	= (999/*30*/);			/* 30カウントしたら半分速 */
		h->BASE_add_speed256		= (t256(1.00)); 	/* 加速/減速、なし */
		#endif
		/* とりあえず、基本版 */
		h->callback_2nd_regist	= NULL;
	}
}
//	data->state 		= 0;
//	data->delta256		= (delta256);

#endif



/*---------------------------------------------------------
	ばらまき攻撃魔方陣(通称マンホール)がばらまく、止まる弾
	-------------------------------------------------------
	咲夜がばらまく、止まる弾。大弾に変化する
	-------------------------------------------------------
	angle512	ANGLE_JIKINERAI_DAN (999) でプレイヤー狙い
	delta256	フレーム毎にspeedをどれだけ減速するか
	-------------------------------------------------------
	弾幕書き換え方式に移行するのでここは無くなる予定。
---------------------------------------------------------*/
	//SPRITE *src, int speed256, int angle512, int a256, int next_angle512);
//SPRITE *src,
//	int br.BULLET_REGIST_speed256,				/* 弾速 */
//	int br.BULLET_REGIST_angle1024, 			/* 角度 */
//	int br.BULLET_REGIST_tomari2_dan_delta256,	/* 減速定数 */
//	int br.BULLET_REGIST_tomari2_dan_next_angle1024


// bullet_create_tomari_dan
static void s_bullet_create_tomari2_dan(void)
{
	SPRITE *h;
	h					= sprite_add_only_bullet_error();	/*丸弾８(赤ＲＧＢ緑若黄青)*/	/*小赤弾 TAMA_TYPE_KUGEL_PNG */
	if (NULL != h)
	{
		h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
		h->type 			= BULLET_MARU8_00_AKA+(0);/*S P_BULLET*/
		h->callback_mover	= enemy_stop_bullet2_move;
		#if 1/*Gu(中心座標)*/
		h->x256 			= /*src*/obj_send1->x256;
		h->y256 			= /*src*/obj_send1->y256;
		#endif
		{
			/*src*/obj_send1->tmp_angleCCW1024	= br.BULLET_REGIST_angle1024;
			s_tmp_angleCCW1024_jikinerai_auto(obj_player, /*src*/obj_send1);
			br.BULLET_REGIST_angle1024				= /*src*/obj_send1->tmp_angleCCW1024;
		}
	//	data->angle512		= (angle512);
	//	data->speed256		= (speed256);
		h->TOMARI_DAN_DATA_speed256 	= (br.BULLET_REGIST_speed256);
		regist_vector(h, t256(1.0)/*speed256*/, br.BULLET_REGIST_angle1024);
		h->TOMARI_DAN_DATA_init_vx256	= /*data->*/h->vx256;
		h->TOMARI_DAN_DATA_init_vy256	= /*data->*/h->vy256;
	//
		//data->state		= 0;
		h->TOMARI_DAN_DATA_delta256 	= (br.BULLET_REGIST_tomari2_dan_delta256);
		if (ANGLE_NO_SHOT_DAN==br.BULLET_REGIST_tomari2_dan_next_angle1024)/* 特別に撃たない弾、の場合 */
		{
			;// 	h->base_time_out	= 800+64;
		}
		else
		{
			;// 	h->base_time_out	= 100+64;
			mask1024(br.BULLET_REGIST_tomari2_dan_next_angle1024);
		}
		h->TOMARI_DAN_next_angle1024	= (br.BULLET_REGIST_tomari2_dan_next_angle1024);
		h->base_time_out				= 1024;/* 適当に大きく1024==(512+64+最大停止時間) */
	}
}


/*---------------------------------------------------------
	くるくるナイフ(青→緑)
	-------------------------------------------------------
	全方向ナイフ(青)
	狙いナイフ(緑)
	-------------------------------------------------------
	弾幕書き換え方式に移行するのでここは無くなる予定。
---------------------------------------------------------*/
	//SPRITE *src, int speed256, int angle512, int height);

//	SPRITE *src,
//	int speed256,	/* 弾速 */
//	int angle512,	/* 角度 */
//	int height		/* (?) */
//)
static void s_bullet_create_sakuya_222kurukuru_knife(void)
{/* x, y*/
	SPRITE *h;
	h					= sprite_add_only_bullet_error();/*全方向ナイフ(青)*/
	if (NULL != h)
	{
		h->m_Hit256R		= TAMA_ATARI_KNIFE18_PNG;
		h->type 			= /*BULLET_KNIFE20_07_MIDORI*/BULLET_KNIFE20_04_AOI;/*BULLET_KNIFE20_04_AOI*/	/* (青→緑ナイフに変身) */		/*S P_BULLET*/ /*SP_LASER*/
		h->callback_mover	= bullet_move_kurukuru_knife;

		#if 1/*Gu(中心座標)*/
		h->x256 			= obj_send1->x256;
		h->y256 			= obj_send1->y256;/*x*/
		#endif
		h->base_time_out	= (300);
	//	h->knife_tmp_angle512/*data->aaa_angle512*/ 	= (br.BULLET_REGIST_angle512);
	//
		{
			int tmp_speed256;
			tmp_speed256 = (br.BULLET_REGIST_speed256);
	//		h->KURUKURU_KNIFE_DATA_aaa_speed256 	= (tmp_speed256/*+tmp_speed256*/);	/* 青ナイフの投げ初速度 */
			h->KURUKURU_KNIFE_DATA_speed_2nd_256	= (tmp_speed256+tmp_speed256);		/* 緑ナイフの2nd狙い速度 */
		}
		h->vx256			= (0);
		h->vy256			= (0);
	//	h->KURUKURU_KNIFE_DATA_target			= 0;
		h->KURUKURU_KNIFE_DATA_height			= (br.BULLET_REGIST_sakuya_kurukurku_knife_height);
	}
}

/*---------------------------------------------------------
	咲夜用の大弾(赤)。
	-------------------------------------------------------
	見た目＞＞あたり判定です。
	-------------------------------------------------------
	弾幕書き換え方式に移行するのでここは無くなる予定。
---------------------------------------------------------*/

static void bullet_create_oodama22(SPRITE *src)
{
	SPRITE *h;
	h					= sprite_add_only_bullet_error();  /* 大弾(赤) 表示部分*/
	if (NULL != h)
	{
	//	angle512			= angle_jikinerai512_auto(obj_player, src, angle512);
	//	mask512(angle512);
	//
		int angle1024;
		angle1024			= (src->TOMARI_DAN_next_angle1024);
	//
		h->m_Hit256R		= TAMA_ATARI_OODAMA_08_PNG;
		h->type 			= BULLET_OODAMA32_00_SIROI+((((angle1024>>7)&(0x03))));
		h->callback_mover	= move_bullet_vector;	/* (要加速機能) */
		#if 1/*Gu(中心座標)*/
		h->x256 			= src->x256;
		h->y256 			= src->y256;
		#endif
	//
		int speed256;
		speed256 = (t256(1.00));
		regist_vector(h, speed256, angle1024);
		h->BULLET_DATA_2nd_count	= (300);			/* 30カウントしたら半分速 */
		h->BASE_add_speed256		= (t256(1.05)); 	/* 加速 */ /*add_speed256*/
	}
}

/*---------------------------------------------------------
	-------------------------------------------------------
	-------------------------------------------------------
	-------------------------------------------------------
	-------------------------------------------------------
---------------------------------------------------------*/


/*---------------------------------------------------------
	輝夜、最終形態で投げてくるかなり無茶な弾。
	-------------------------------------------------------
	delta256	フレーム毎に下方向にどれだけ加速するか
---------------------------------------------------------*/
//extern void bullet_create_hazumi_dan(void);
//SPRITE *src, int speed256, int angle512, int delta256, int bound_counts);

// [***090114	追加
//	SPRITE *src,
//	int br.BULLET_REGIST_speed256,
//	int br.BULLET_REGIST_angle1024,
//	int BULLET_REGIST_jyuryoku_dan_delta256,
//	int BULLET_REGIST_jyuryoku_dan_bound_counts

static void s_bullet_create_hazumi_dan(void)
{
	SPRITE *h;			/*丸弾８(赤ＲＧＢ緑若黄青)*/
	h					= sprite_add_only_bullet_error();/*緑黄弾*/
	if (NULL != h)
	{
		h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
		h->type 			= BULLET_MARU8_00_AKA+(4+br.BULLET_REGIST_jyuryoku_dan_bound_counts);/*S P_BULLET*/ /*SP_LASER*/
		h->callback_mover	= bullet_move_hazumi_dan;
		#if 1/*Gu(中心座標)*/
		h->x256 			= /*src*/obj_send1->x256;
		h->y256 			= /*src*/obj_send1->y256;
		#endif
		/*angle512			= angle_jikinerai512_auto(obj_player, src, angle512);*/
		mask1024(br.BULLET_REGIST_angle1024);
	//	h->HAZUMI_DAN_DATA_angle512 	= (angle512);
	//	h->HAZUMI_DAN_DATA_speed256 	= (speed256);
		regist_vector(h, br.BULLET_REGIST_speed256, br.BULLET_REGIST_angle1024);

		h->HAZUMI_DAN_DATA_delta256 	= (br.BULLET_REGIST_jyuryoku_dan_delta256);
	//	h->HAZUMI_DAN_DATA_sum256		= t256(0);
		h->HAZUMI_DAN_DATA_bound_counts = br.BULLET_REGIST_jyuryoku_dan_bound_counts;
	}
}















/*---------------------------------------------------------
	多方向、等速直進弾を追加する
	-------------------------------------------------------

---------------------------------------------------------*/
/*(int)(512/24)*/

//	SPRITE *src,			/*	*/
//	int BULLET_REGIST_speed256, 		/* 弾速 */
//	int BULLET_REGIST_angle512, 		/* 発射中心角度 / 特殊機能(自機狙い/他) */
//	int BULLET_REGIST_div_angle512, 	/* 1周をn分割した角度 */
//	int BULLET_REGIST_bullet_obj_type,	/* 弾グラ */
//	int BULLET_REGIST_n_way 			/* 発弾数 */
	/*
		よく考えたら、ANGLE_JIKI_NERAI_KISUDAN は必要ない。
		ANGLE_JIKINERAI_DAN だけあれば良く、
		発弾数に偶数を指定すれば偶数弾。
		発弾数に奇数を指定すれば奇数弾。
		あたりまえだけど、気が付かなかったww。
	*/
static void s_bullet_regist_multi_angle(void)
{
	{
		obj_send1->tmp_angleCCW1024 		= br.BULLET_REGIST_angle1024;
		s_tmp_angleCCW1024_jikinerai_auto(obj_player, obj_send1/*src*/ ); /*ANGLE_JIKI_NERAI_DAN???(original)*/ /* 奇数弾の場合に自機狙い */
		br.BULLET_REGIST_angle1024	= obj_send1->tmp_angleCCW1024;
	}
	int i_angle1024;
	i_angle1024 		= br.BULLET_REGIST_angle1024-((br.BULLET_REGIST_n_way+2)*((br.BULLET_REGIST_div_angle1024>>1)/*(int)(1024/48)*/)); /* 48分割弾 */ // /* π/24 0.13089969389957471827 */
	/* 念の為マスク */
//	mask1024(i_angle1024);
//
	int i;
	for (i=0; i<br.BULLET_REGIST_n_way; i++)
	{
		SPRITE *h;
		// frameはフレーム数-1 /*char *filename, int frame,*/
		h					= sprite_add_only_bullet_error(); /*_MING32_PNG*/
		if (NULL != h)
		{
			h->m_Hit256R		= TAMA_ATARI_COMMON16_PNG;
			h->type 			= (br.BULLET_REGIST_bullet_obj_type);/*BULLET_UROKO14_01_AKA+*/ /*S P_BULLET*/
			h->callback_mover	= move_bullet_vector;
			#if 1/*Gu(中心座標)*/
			h->x256 			= /*src*/obj_send1->x256;
			h->y256 			= /*src*/obj_send1->y256;
			#endif
			i_angle1024 		+= br.BULLET_REGIST_div_angle1024;	/*(2)*((int)(1024/48))*/ /* π/12 0.26179938779914943654 */
			mask1024(i_angle1024);
			h->m_angleCCW1024		= i_angle1024;		/* 描画用角度(下が0度で左回り(反時計回り)) */
			regist_vector(h, br.BULLET_REGIST_speed256, i_angle1024);
		}
	}
}


/*---------------------------------------------------------
	弾を登録する。
	-------------------------------------------------------
	関数を(クラスっぽく)隠蔽。
	弾発動時のエフェクト等の関係上、発弾部分は全部纏める必要がある。
	-------------------------------------------------------
	ベクトル弾と角度弾を統合するかもしれない。
---------------------------------------------------------*/

global void bullet_regist_vector(void)
{
	#if 1
	switch (br.BULLET_REGIST_regist_type)
	{
	case REGIST_TYPE_00_MULTI_VECTOR:	s_bullet_regist_multi_angle();					break;
	case REGIST_TYPE_01_HAZUMI: 		s_bullet_create_hazumi_dan();					break;
	case REGIST_TYPE_02_GRAVITY02:		s_bullet_create_jyuryoku_dan(); 				break;
	case REGIST_TYPE_03_TOMARI: 		s_bullet_create_tomari2_dan();					break;
	case REGIST_TYPE_04_KURU_KURU:		s_bullet_create_sakuya_222kurukuru_knife(); 	break;
	}
	#else
	const void (*aaa_func[(8)])(void) =
	{
		s_bullet_regist_multi_angle,					/* REGIST_TYPE_00_MULTI_VECTOR	*/
		s_bullet_create_hazumi_dan, 					/* REGIST_TYPE_01_HAZUMI		*/
		s_bullet_create_jyuryoku_dan,					/* REGIST_TYPE_02_GRAVITY02 	*/
		s_bullet_create_tomari2_dan,					/* REGIST_TYPE_03_TOMARI		*/
//
		s_bullet_create_sakuya_222kurukuru_knife,		/* REGIST_TYPE_04_KURU_KURU 	*/
		NULL,
		NULL,
		NULL,
	};
	(*aaa_func[ (br.BULLET_REGIST_regist_type/*&0x07*/) ])();	/* 各関数に分岐する */
	#endif
}
