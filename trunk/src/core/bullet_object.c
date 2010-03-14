
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
	弾幕リストをシンプルな構造にする方が先。(data、mallocしない)
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

#include "game_main.h"
#include "bullet_object.h"

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
#define BASE_add_speed256	user_data07 	/* 加速度 */




/* 応用弾(速度変化する) */
//typedef struct
//{
#define TOMARI_DAN_DATA_speed256	user_data00 	/* 速度 */
#define TOMARI_DAN_DATA_delta256	user_data01 	/* 加速度 */
//
#define TOMARI_DAN_DATA_init_vx256	user_data02 	/* 設定初期速度 */
#define TOMARI_DAN_DATA_init_vy256	user_data03 	/* 設定初期速度 */
//
	//int next_angle512;/*ST2*/ /* tmp_angleCCW512 */
	//int state;
//} TOMARI_DAN_DATA;
	//int time_out; 	/* timer */


/* 基本弾(速度、角度、ともに変化しない) */
//typedef struct
//{
//	int speed256;			/* sum256 */		/* 速度 */
#define GRAVITY_BULLET_DATA_delta256	user_data01 	/* 加速度(y軸成分のみ) */		/* gra256 */
//} GRAVITY_BULLET_DATA;


//typedef struct
//{
//	int speed256;			/* sum256 */		/* 速度 */
#define HAZUMI_DAN_DATA_delta256		user_data01 	/* 加速度(y軸成分のみ) */		/* gra256 */
#define HAZUMI_DAN_DATA_bound_counts	user_data02 	/* 跳ねる予定の回数(0で終わり) */
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
	どうも、本家には角度弾は無いっぽい。
	(本家の弾幕はこの方式以外で実装してるっぽい)
	角度変化するものは、数フレームごと、角度変化計算しなおしてるっぽい。
---------------------------------------------------------*/

/* 応用弾 */

//typedef struct
//{
//	int aaa_angle512;	/*union BULLET_ANGLE_DATA*/
#define KURUKURU_KNIFE_DATA_aaa_speed256				user_data01 	/*union BULLET_ANGLE_DATA*/
//
	//int time_out;
#define KURUKURU_KNIFE_DATA_speed_2nd_256			user_data00
#define KURUKURU_KNIFE_DATA_height					user_data02
#define KURUKURU_KNIFE_DATA_target					user_data03
//	int p_angle512;/*表示用角度*/
//} KURUKURU_KNIFE_DATA;

//typedef struct
//{
#define EVEN_KNIFE_DATA_angle512					user_data00 	/*union BULLET_ANGLE_DATA*/
#define EVEN_KNIFE_DATA_speed256					user_data01 	/*union BULLET_ANGLE_DATA*/
//
	//int time_out; // act

#define EVEN_KNIFE_DATA_d_angle512					user_data02
//	int length256;	t256(128)
#define EVEN_KNIFE_DATA_x256						user_data03
#define EVEN_KNIFE_DATA_y256						user_data04
#define EVEN_KNIFE_DATA_state						user_data05
#define EVEN_KNIFE_DATA_wait1						user_data06 	/* fire */
//	int p_angle512;/*表示用角度*/
#define EVEN_KNIFE_DATA_l_or_r						user_data07 	/* -1==l, 1==r */
//} EVEN_KNIFE_DATA;


/* 交差弾 実験 */
//typedef struct
//{
//	int angle512;
#define BOSS02_ICE_DATA_state						user_data00
#define BOSS02_ICE_DATA_speed256					user_data01
//
	//int time_out;
//	int add_angle512;
#define BOSS02_ICE_DATA_next_2nd_state				user_data02 /*ちょっと実験*/
//} BOSS02_ICE_DATA;

//typedef struct
//{
//	//int time_out; 	/* timer */
//} MOMIJI_DATA;


//typedef struct
//{
//	SPRITE *hyouji;
	//int time_out; 	/*OLD_BIG_BULLET_DATA*/ 	/* int wait_bg;*/
//#define OODAMA_OYA_DATA_ransu512					user_data00 	/*OLD_BIG_BULLET_DATA*/
//#define OODAMA_OYA_DATA_add_speed256				user_data01 	/*NEW_BIG_BULLET_DATA*/
//} OODAMA_OYA_DATA;

#if 0
typedef struct
{
	//int time_out;
	int check_x256;
	int check_y256;
//
//	int *sx256p;
//	int *sy256p;
//	int *type;
//	int sw;
//	int sh;
} OODAMA_KO_DATA;
#endif



//typedef struct
//{
#define BULLET_SP1_DATA_angle512				user_data00
#define BULLET_SP1_DATA_y_sum256				user_data01
#define BULLET_SP1_DATA_player_x256 			user_data02
#define BULLET_SP1_DATA_player_y256 			user_data03
//} BULLET_SP1_DATA;
//	int x_sa256;
//	int y_sa256;

//	int speed256;
//
//	int gra256; 	/* gra256 */
//	int sum256;
//	//int time_out; 	/* timer */

//	void/*BO SS99 _DATA*/ *sd;
//	int *sd_angle512;
//	int angle2_512;
//	int r_or_l; 	//	/*+1==R*/	/*-1==L*/

//typedef struct
//{
#define BULLET_SP2_DATA_angle512			user_data00
#define BULLET_SP2_DATA_speed256			user_data01 							/* 速度 */
//	int delta256;		/* t256(0.015) gra256 */			/* 加速度 */
//
//	void/*BOSS05_MAHO_DATA *sd;
//	int  *sd_nnn;
//
//	int state;
#define BULLET_SP2_DATA_star_remain_time			user_data02
//} BULLET_SP2_DATA;


/*---------------------------------------------------------
	画面外ならおしまい
---------------------------------------------------------*/
void gamen_gai_nara_osimai(SPRITE *src)
{
		if (((src->x256 < t256(0))||(src->x256 > t256(GAME_WIDTH))||
			 (src->y256 < t256(0))||(src->y256 > t256(GAME_HEIGHT))))		/* 画面外に出たら */
		{
			src->type = SP_DELETE;		/* おしまい */
		}
}
//	if ((src->x256 < t256(0))||(src->x256 > t256(GAME_WIDTH))||
//		(src->y256 < t256(0))||(src->y256 > t256(GAME_HEIGHT)))
//	{
//		src->type = SP_DELETE;/*画面外にでたらおしまい*/
//	}
/*---------------------------------------------------------
	速度値、角度値、をベクトル量に変換する
---------------------------------------------------------*/

static void callback_2nd_regist(SPRITE *src);
static void regist_vector(SPRITE *src, int speed256, int angle512)
{
	src->vx256	= ((sin512((angle512))*(speed256))>>8); 	/*fps_factor*/
	src->vy256	= ((cos512((angle512))*(speed256))>>8); 	/*fps_factor*/
//
	#if 0
	/* 弾速が遅すぎる場合に修正する */
	if (( (8) > abs(src->vx256)) ) { src->vx256 = (0<src->vx256)?( (8)):( (-8)); }
	if (( (8) > abs(src->vy256)) ) { src->vy256 = (0<src->vy256)?( (8)):( (-8)); }
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
	src->BASE_add_speed256		= (t256(1.00)); 	/* 加速/減速、なし */
}

/*---------------------------------------------------------
	基本量移動、画面外は消える
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
	gamen_gai_nara_osimai(src);/* 画面外ならおしまい */

	src->BULLET_DATA_2nd_count--;		/*fps_factor*/
	if (0 > src->BULLET_DATA_2nd_count)
	{
		if (NULL != src->callback_2nd_regist)
		{
			(src->callback_2nd_regist)(src);
			src->callback_2nd_regist = NULL;
		}
	}
}


/*---------------------------------------------------------
	咲夜用の黄色マスカット弾(ナイフ)
---------------------------------------------------------*/

#if 1
static int angle_jikinerai512_auto(SPRITE *p, SPRITE *t, int angle512);
static void move_bullet_sakuya_sp1(SPRITE *src)
{
//	BULLET_SP1_DATA *data = (BULLET_SP1_DATA *)src->data;
	src->base_time_out--;	//	data->timer++;
	if (0 > src->base_time_out)//	if (data->timer > 40)
	{
	//	else
		{
			src->vx256 = ((sin512((src->BULLET_SP1_DATA_angle512)) ) );/*fps_factor*/
			src->vy256 = ((cos512((src->BULLET_SP1_DATA_angle512)) ) );/*fps_factor*/
		}
	}
	else
	if (0+2048 > src->base_time_out)//	if (data->timer > 40)
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
			src->BULLET_SP1_DATA_y_sum256--;
		}
	//	if (240/*1*/ < data->y_sum256)
		{
			int x_sa256 = (src->x256 - src->BULLET_SP1_DATA_player_x256);
			int y_sa256 = (src->y256 - src->BULLET_SP1_DATA_player_y256);
			if (t256(2.0) > abs(x_sa256))
			{
				if (t256(2.0) > abs(y_sa256))
				{
					src->base_time_out = 0;
				}
			}
			int aaa_x256 = ((x_sa256 * src->BULLET_SP1_DATA_y_sum256)>>8);	/*fps_factor*/
			int aaa_y256 = ((y_sa256 * src->BULLET_SP1_DATA_y_sum256)>>8);	/*fps_factor*/
			src->x256 = src->BULLET_SP1_DATA_player_x256 + (aaa_x256);		/*fps_factor*/
			src->y256 = src->BULLET_SP1_DATA_player_y256 + (aaa_y256);		/*fps_factor*/
		}
	}
	else
	if (0+2048==src->base_time_out) //else if (data->timer==40)
	{
	//	src->yx_an im_frame = (1);
	//	src->type			= BULLET_MARU8_00_AKA+(1);
		src->type			= BULLET_KNIFE20_07_MIDORI;/* (青→緑ナイフに変身) */
		src->BULLET_SP1_DATA_angle512 = ANGLE_JIKI_NERAI_DAN;
		src->BULLET_SP1_DATA_angle512 = angle_jikinerai512_auto(player, src, src->BULLET_SP1_DATA_angle512);
		mask512(src->BULLET_SP1_DATA_angle512);
		src->m_angleCCW512 = src->BULLET_SP1_DATA_angle512;
	//	src->BULLET_SP1_DATA_angle2_512 = (*(data->sd_angle512))/*(data->sdata->angle512)*/;
//		src->BULLET_SP1_DATA_angle2_512 = (s->tmp_angleCCW512)/*(data->sdata->angle512)*/;
		src->vx256 = (0);	/*fps_factor*/
		src->vy256 = (0);	/*fps_factor*/
		/* とりあえず、仮対応。強制的に設定しちゃう */
		src->BULLET_SP1_DATA_player_x256 = (player->x256+t256(32/2)/*幅の半分*/) ;/*fps_factor*/
		src->BULLET_SP1_DATA_player_y256 = (player->y256+t256(16/2)/*高さの半分*/) ;/*fps_factor*/
//		src->BULLET_SP1_DATA_flag_first = 1;
		src->BULLET_SP1_DATA_y_sum256 = t256(1.0);	/* (2.5==5*0.5) */
	}
//	else
//	{
/* CCWの場合 */
//	}
//
	move_bullet_vector(src);
}
#endif



/*---------------------------------------------------------
	咲夜用の魔方陣用
---------------------------------------------------------*/

static void move_bullet_sakuya_sp2(SPRITE *src)
{
	{
	//	BULLET_SP2_DATA *data = (BULLET_SP2_DATA *)src->data;
	//	if (2==data->sdata->nnn/*%(4-difficulty)*/)/* ステートが1の時配置して、ステートが2になったら動く */
	//	if (2==(*(data->sd_nnn))/*%(4-difficulty)*/)/* ステートが1の時配置して、ステートが2になったら動く */
	//	if (1==(*(data->sd_nnn))/*%(4-difficulty)*/)/* ステートが0の時配置して、ステートが1になったら動く */
		src->BULLET_SP2_DATA_star_remain_time--;
		if (0 > (src->BULLET_SP2_DATA_star_remain_time))/* 動く */
		{
			src->BULLET_SP2_DATA_star_remain_time = 0;
			src->BULLET_SP2_DATA_speed256 += 4; // (3.84) == t256(0.015);
			src->vx256 = ((sin512((src->BULLET_SP2_DATA_angle512))*(src->BULLET_SP2_DATA_speed256))>>8);/*fps_factor*/	/* CCWの場合 */
			src->vy256 = ((cos512((src->BULLET_SP2_DATA_angle512))*(src->BULLET_SP2_DATA_speed256))>>8);/*fps_factor*/
		}
	}
	//move_bullet_vector(src);
	src->x256 += (src->vx256);		/*fps_factor*/
	src->y256 += (src->vy256);		/*fps_factor*/
	gamen_gai_nara_osimai(src);/* 画面外ならおしまい */
}

/*---------------------------------------------------------
	自機狙い弾の角度を計算
---------------------------------------------------------*/
static int angle_jikinerai512(SPRITE *p, SPRITE *t)
{
	#if 1
	return (atan_512((p->y256-t->y256), (p->x256-t->x256)));
	/* 偶数弾の場合に自機狙い(簡略版) 弾と自分が大きさが同じならずれない、違うとその分誤差になる */
	#endif
}
static int angle_jikinerai512_auto(SPRITE *p, SPRITE *t, int angle512)
{
	if (ANGLE_JIKI_NERAI_DAN==angle512) 	{	angle512	= angle_jikinerai512(p,t);}
	return (angle512);
}


/*---------------------------------------------------------

---------------------------------------------------------*/
#define TOMARI_DAN_next_angle512 tmp_angleCCW512
#define TOMARI_DAN_LIMIT_00_640 (512+128)
#define TOMARI_DAN_LIMIT_00_576 (512+64)
#define TOMARI_DAN_LIMIT_01_512 (512+0)
#define TOMARI_DAN_LIMIT_02_000 (0+0)
static void bullet_create_oodama22(
	SPRITE *src//,
//	int speed256,
//	int angle512//,
//	int ransu512//,
//	int add_speed256/*, int xoffs256, int yoffs256*/
);
//atic void enemy_stop_bullet_move(SPRITE *src)
static void enemy_stop_bullet2_move(SPRITE *src)
{
//	TOMARI_DAN_DATA *data = (TOMARI_DAN_DATA *)src->data;
	/*data->*/src->base_time_out--;
	if (TOMARI_DAN_LIMIT_00_640 < /*data->*/src->base_time_out)
	{
//	case 0: 	/* 止まる */
		src->TOMARI_DAN_DATA_speed256 -= (src->TOMARI_DAN_DATA_delta256);
		/*data->v*/src->vx256 = (((src->TOMARI_DAN_DATA_init_vx256)*src->TOMARI_DAN_DATA_speed256)>>8);/*fps_factor*/
		/*data->v*/src->vy256 = (((src->TOMARI_DAN_DATA_init_vy256)*src->TOMARI_DAN_DATA_speed256)>>8);/*fps_factor*/
		if (src->TOMARI_DAN_DATA_speed256 < t256(0))
		{
			src->TOMARI_DAN_DATA_speed256 = t256(0);
		//	data->state = 1;
			/*data->*/src->base_time_out=(TOMARI_DAN_LIMIT_00_640);
		}
	}
	else
	if (TOMARI_DAN_LIMIT_00_576 < /*data->*/src->base_time_out)
	{
//		break;
//	case 1: 	/* しばし停止 */
//		if (64 > /*data->*/src->base_time_out)
//		{
//			data->state = 2;
//		}
//		break;
//	case 2: 	/* 撃つ(大弾に変身) */
	//	src->an im_frame =						((/*data->*/src->base_time_out>>3)/*&0x07*/);	/*警告(変身するので)*/
		src->type		= BULLET_MARU8_00_AKA+	((/*data->*/src->base_time_out>>3)&0x07);		/*警告(変身するので)*/
	}
	else
	if (TOMARI_DAN_LIMIT_01_512 < /*data->*/src->base_time_out)
	{
	//	data->timer = 0;
	//	data->state = 3;
		if (ANGLE_NO_SHOT_DAN != src->TOMARI_DAN_next_angle512/*data->next_angle512*/)
		{
			bullet_create_oodama22(src//,
			//	t256(1.0/*0.0*/),
			//	(src->TOMARI_DAN_next_angle512)/*data->next_angle512*/ //,
				//0//,
				//(src->TOMARI_DAN_DATA_delta256/**7*/)/*, 0, 0*/
			);
			/*data->*/src->base_time_out=(TOMARI_DAN_LIMIT_02_000);
		}
		else
		{
			/*data->*/src->base_time_out=(TOMARI_DAN_LIMIT_01_512);
		}
	}
	else
	if (TOMARI_DAN_LIMIT_02_000 < /*data->*/src->base_time_out)
	{
		;	/* しばし停止 */
	}
	else
	{
//		break;
//	case 3: 	/* 消える */
		src->color32 -= 0x10000000; 		/*	src->alpha -= 0x10; */
		if ( 0x20000000 > src->color32) 	/*	( 0x20 > src->alpha)	*/
		{
			src->color32 = 0x00ffffff;		/*	src->alpha = 0x00;	*/
			src->type = SP_DELETE;
		}
//		break;
	}
	/* 特別に撃たない弾、の場合 */
	if (ANGLE_NO_SHOT_DAN == src->TOMARI_DAN_next_angle512/*data->next_angle512*/)
	{	/* スクロールする */
		src->y256 += t256(1);	//	my_adjust_y(s,pd);
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
//	src->HAZUMI_DAN_DATA_angle512	= atan_512(sin512((src->HAZUMI_DAN_DATA_angle512)),-cos512((src->HAZUMI_DAN_DATA_angle512)));
//}
static void bullet_move_hazumi_dan(SPRITE *src)
{
//	HAZUMI_DAN_DATA *data = (HAZUMI_DAN_DATA *)src->data;
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
		//	src->an im_frame = 4+(src->HAZUMI_DAN_DATA_bound_counts);		// [***090116		若干変更
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
//	GRAVITY_BULLET_DATA *data = (GRAVITY_BULLET_DATA *)src->data;
	src->vy256 += src->GRAVITY_BULLET_DATA_delta256;
//	data->speed256 += data->delta256;
//	src->y256 += data->speed256;
	move_bullet_vector(src);
}

/*---------------------------------------------------------
	咲夜
	プレイヤーの周りをくるくる回ってから、自分狙いのナイフ。
	青→緑に色が変わる。
---------------------------------------------------------*/
#define knife_tmp_angle512 tmp_angleCCW512

static void bullet_move_kurukuru_knife(SPRITE *src)
{
//	KURUKURU_KNIFE_DATA *data = (KURUKURU_KNIFE_DATA *)src->data;
	if (0==src->KURUKURU_KNIFE_DATA_target)
	{
//		mono_angle_move(s,(BULLET_ANGLE_DATA *)d);

	//	src->KURUKURU_KNIFE_DATA_aaa_speed256 -= t256(0.02);
	//	src->KURUKURU_KNIFE_DATA_aaa_speed256 -= t256(0.02*2);/*2倍にしたので辻褄合わせ*/
		src->KURUKURU_KNIFE_DATA_aaa_speed256 -= t256(0.04);	/* ナイフ速度、減速定数 */
	//	src->KURUKURU_KNIFE_DATA_aaa_speed256 -= 1/*t256(0.04)*/; /* ナイフ速度、減速定数 */
	//	if (s->an im_frame==17) {	src->an im_frame = 0;}
	//	else					{	src->an im_frame++;}
		/*data->p_angle512*/src->m_angleCCW512 += 32;
		mask512(src->m_angleCCW512/*data->p_angle512*/);/*ねんのため*/
		if (src->KURUKURU_KNIFE_DATA_aaa_speed256 <= t256(0) )
		{
			src->knife_tmp_angle512/*data->aaa_angle512*/ = atan_512(
				#if 1/*Gu(中心座標)*/
				(player->y256)-(src->y256)+(src->KURUKURU_KNIFE_DATA_height*256),
				(player->x256)-(src->x256)
				#endif
			);
			src->KURUKURU_KNIFE_DATA_aaa_speed256 = src->KURUKURU_KNIFE_DATA_speed_2nd_256;
			src->KURUKURU_KNIFE_DATA_target=1;
			src->type			= BULLET_KNIFE20_07_MIDORI;/* (青→緑ナイフに変身) */
		//	src->an im_frame=/*17-*/(deg_512_to_360(data->angle512)/20)%18;
		//	src->an im_frame=/*17-*/(deg_512_to_360(data->angle512)/20)%16;
			mask512(src->knife_tmp_angle512/*data->aaa_angle512*/);/*ねんのため*/
		//	src->an im_frame=/*17-*/((data->angle512)>>5);
			src->m_angleCCW512/*data->p_angle512*/=/*17-*/((src->knife_tmp_angle512/*data->aaa_angle512*/));
		}
		/*data->*/src->vx256 = ((sin512((src->knife_tmp_angle512/*data->aaa_angle512*/))*(src->KURUKURU_KNIFE_DATA_aaa_speed256))>>8);/*fps_factor*/
		/*data->*/src->vy256 = ((cos512((src->knife_tmp_angle512/*data->aaa_angle512*/))*(src->KURUKURU_KNIFE_DATA_aaa_speed256))>>8);/*fps_factor*/
	}
	src->x256 += (/*data->*/src->vx256);		/*fps_factor*/
	src->y256 += (/*data->*/src->vy256);		/*fps_factor*/
//
	/*data->*/src->base_time_out--;
	if (/*data->*/src->base_time_out<0)
	{
		if (((src->x256 < t256(0))||(src->x256 > t256(GAME_WIDTH))||
			 (src->y256 < t256(0))||(src->y256 > t256(GAME_HEIGHT))))
		{
			src->type = SP_DELETE;/* 画面外に出たらおしまい */
		}
	}
}

/*---------------------------------------------------------

	-------------------------------------------------------
	s->x256 	ナイフのx座標
	data->x256	ノードのx座標
	s->w/2		ナイフの横幅の半分
	ナイフのx座標＝ノードのx座標＋co_s512((data->angle512))×length－ナイフの横幅の半分
---------------------------------------------------------*/
#define ryoute_knife_length256 t256(128)
//static void enemy_evenr_knife_move(SPRITE *src)
static void enemy_even_knife_move(SPRITE *src)
{
//	EVEN_KNIFE_DATA *data = (EVEN_KNIFE_DATA *)src->data;
	switch (src->EVEN_KNIFE_DATA_state)
	{
	case 0:
		if (100 < src->EVEN_KNIFE_DATA_wait1)
		{
			src->EVEN_KNIFE_DATA_wait1 = 0;
			src->EVEN_KNIFE_DATA_state = 1;
		}
		src->EVEN_KNIFE_DATA_angle512 -= (src->EVEN_KNIFE_DATA_d_angle512*src->EVEN_KNIFE_DATA_l_or_r) ;/*fps_factor*/
		mask512(src->EVEN_KNIFE_DATA_angle512);
		/* 描画用角度(下が0度で左回り(反時計回り)) */
		src->m_angleCCW512 = (src->EVEN_KNIFE_DATA_angle512+((int)(512*1/4)*src->EVEN_KNIFE_DATA_l_or_r));
		mask512(src->m_angleCCW512);/*必ず要る*/
	//	src->x256		= data->x256-((cos512((data->angle512))*data->length256)>>8)-((src->w128));
	//	src->y256		= data->y256-((sin512((data->angle512))*data->length256)>>8)-((src->h128));
	//	src->x256		= data->x256-((sin512((data->angle512))*ryoute_knife_length256/*data->length256*/)>>8)-((src->w128));
	//	src->y256		= data->y256-((cos512((data->angle512))*ryoute_knife_length256/*data->length256*/)>>8)-((src->h128));
		#if 1/*Gu(中心座標)*/
		src->x256		= src->EVEN_KNIFE_DATA_x256-((sin512((src->EVEN_KNIFE_DATA_angle512)))<<7 );
		src->y256		= src->EVEN_KNIFE_DATA_y256-((cos512((src->EVEN_KNIFE_DATA_angle512)))<<7 );
		#endif
		break;

	case 1:
		if (50 < src->EVEN_KNIFE_DATA_wait1)
		{
			src->EVEN_KNIFE_DATA_wait1 = 0;
			src->EVEN_KNIFE_DATA_state = 0;
		}
		//#if 1
		//data->speed=20/*speed*/;/*よくわかんないが止まっちゃう*/
		//#endif
	//	mono_angle_move(src,(BULLET_ANGLE_DATA *)data);
		src->vx256 = ((sin512((src->EVEN_KNIFE_DATA_angle512))*src->EVEN_KNIFE_DATA_speed256)>>8);/*fps_factor*/
		src->vy256 = ((cos512((src->EVEN_KNIFE_DATA_angle512))*src->EVEN_KNIFE_DATA_speed256)>>8);/*fps_factor*/
		src->x256 += (src->vx256);/*fps_factor*/
		src->y256 += (src->vy256);/*fps_factor*/
		break;
	}
	if (10 < src->EVEN_KNIFE_DATA_wait1)
	{
		src->EVEN_KNIFE_DATA_wait1 = 0;
		#if 1
		send1_obj->x256 = src->x256;
		send1_obj->y256 = src->y256;
		send1_obj->BULLET_REGIST_speed256			=	(t256(2.5)/*t256(5.0)*/);
		send1_obj->BULLET_REGIST_angle512			=	(src->EVEN_KNIFE_DATA_angle512+((int)(512*3/12)*src->EVEN_KNIFE_DATA_l_or_r));
	//	send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(18));			/* ダミー分割角度(1wayなので使用していない) */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;		/* [青ナイフ弾] */
		send1_obj->BULLET_REGIST_n_way				=	(1);	/* [1way] */
		bullet_regist_basic();
		//
		send1_obj->BULLET_REGIST_speed256			=	(t256(2.2)/*t256(4.0)*/);
		send1_obj->BULLET_REGIST_angle512			=	(src->EVEN_KNIFE_DATA_angle512+((int)(512*4/12)*src->EVEN_KNIFE_DATA_l_or_r));
	//	send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(18)),		/* ダミー分割角度(1wayなので使用していない) */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;		/* [青ナイフ弾] */
		send1_obj->BULLET_REGIST_n_way				=	(1);	/* [1way] */
		bullet_regist_basic();
//
		send1_obj->BULLET_REGIST_speed256			=	(t256(2.0)/*t256(3.0)*/);
		send1_obj->BULLET_REGIST_angle512			=	(src->EVEN_KNIFE_DATA_angle512+((int)(512*5/12)*src->EVEN_KNIFE_DATA_l_or_r));
	//	send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(18)),		/* ダミー分割角度(1wayなので使用していない) */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;		/* [青ナイフ弾] */
		send1_obj->BULLET_REGIST_n_way				=	(1);	/* [1way] */
		bullet_regist_basic();
//
		if (1<difficulty)
		{
		send1_obj->BULLET_REGIST_speed256			=	(t256(1.8)/*t256(2.0)*/);
		send1_obj->BULLET_REGIST_angle512			=	(src->EVEN_KNIFE_DATA_angle512+((int)(512*6/12)*src->EVEN_KNIFE_DATA_l_or_r));
	//	send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(18));		/* ダミー分割角度(1wayなので使用していない) */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_04_AOI;		/* [青ナイフ弾] */
		send1_obj->BULLET_REGIST_n_way				=	(1);	/* [1way] */
		bullet_regist_basic();
		}
		#else
		bullet_create_sakuya_knife(src, t256(5.0), src->EVEN_KNIFE_DATA_angle512+(int)(512*3/12), src->EVEN_KNIFE_DATA_p_angle512); //+M_PI/2
		bullet_create_sakuya_knife(src, t256(4.0), src->EVEN_KNIFE_DATA_angle512+(int)(512*4/12), src->EVEN_KNIFE_DATA_p_angle512);
		bullet_create_sakuya_knife(src, t256(3.0), src->EVEN_KNIFE_DATA_angle512+(int)(512*5/12), src->EVEN_KNIFE_DATA_p_angle512);
		if (1<difficulty)
		{	bullet_create_sakuya_knife(src, t256(2.0), src->EVEN_KNIFE_DATA_angle512+(int)(512*6/12), src->EVEN_KNIFE_DATA_p_angle512);}
		#endif
	}
	src->EVEN_KNIFE_DATA_wait1++;
	/*data->*/src->base_time_out--;
	if ( /*data->*/src->base_time_out < 0 )
	{
		src->type = SP_DELETE;
	}
}

enum
{
	KS00 = 0,
//	KS03_type2,
	KS01,
	KS02,
//	KS04,
};
/*---------------------------------------------------------

---------------------------------------------------------*/
			/* 描画用角度(下が0度で左回り(反時計回り)) */
#define ice_angle512		m_angleCCW512
#define ice_add_angle512	tmp_angleCCW512

static void enemy_boss02_ice_move(SPRITE *src)
{
//	BOSS02_ICE_DATA *data = (BOSS02_ICE_DATA *)src->data;
	switch (src->BOSS02_ICE_DATA_state)
	{
	case KS00:	/* 円状運動 */
		/*data->*/src->base_time_out--;/*fps_factor*/
		if (/*data->*/src->base_time_out <= 0)
		{
			src->BOSS02_ICE_DATA_state = src->BOSS02_ICE_DATA_next_2nd_state;
		//	src->BOSS02_ICE_DATA_state++/* = KS01*/;
		}
		src->ice_angle512 += src->ice_add_angle512;/*fps_factor*/
//
		mask512(src->ice_angle512);
		src->vx256 = ((sin512((src->ice_angle512))*src->BOSS02_ICE_DATA_speed256)>>8);/*fps_factor*/
		src->vy256 = ((cos512((src->ice_angle512))*src->BOSS02_ICE_DATA_speed256)>>8);/*fps_factor*/
		break;
	case KS01:	/* バラバラ移動 */
		src->BOSS02_ICE_DATA_state++/* = KS02*/;
		{
			#if 0
			const short sss[4] =
			{
				t256(2.5),		/* easy:   2 [すごく簡単に変更]  2 これが避けれないって事は無いと思うケド */
				t256(3.0),		/* normal: 4 [すごく簡単に変更]  3 */
				t256(3.5),		/* hard:   6 [すごく簡単に変更]  5 落ち着いてよく見れば避けれます */
				t256(4.0)		/* luna:  12 [すごく簡単に変更] 10 落ち着いてよく見れば避けれますが、たまに失敗するwww */
			};
			src->BOSS02_ICE_DATA_speed256 = sss[difficulty]/*t256(12)*/;
			#else
		//	src->BOSS02_ICE_DATA_speed256 = t256(2.5)+(difficulty<<7);	/* (difficulty x 128) */
			src->BOSS02_ICE_DATA_speed256 = t256(2.5)+(difficulty<<6);	/* (difficulty x 64) */ 	/* 弾数が増えて難しくなったので遅くする */
			#endif
		}
//		src->ice_angle512	= (ra_nd()&(128-1)/*%90*/)+deg_360_to_512(45);
		src->ice_angle512	= (ra_nd()&(128-1)/*%90*/)-deg_360_to_512CCW(45);
//
		mask512(src->ice_angle512);
		src->vx256 = ((sin512((src->ice_angle512))*src->BOSS02_ICE_DATA_speed256)>>8);/*fps_factor*/
		src->vy256 = ((cos512((src->ice_angle512))*src->BOSS02_ICE_DATA_speed256)>>8);/*fps_factor*/
		break;
//	case KS02:	/* 何もしない(等速直線移動) */
//		break;
	}
//
	src->x256 += (src->vx256);/*fps_factor*/
	src->y256 += (src->vy256);/*fps_factor*/
//	move_bullet_vector(src);
	if ((src->x256 < t256(-100))||(src->x256 > t256(100)+t256(GAME_WIDTH))||
		(src->y256 < t256(-100))||(src->y256 > t256(100)+t256(GAME_HEIGHT) ) )
	{
		src->type = SP_DELETE;
	}
}


/*---------------------------------------------------------
	輝夜、最終形態で投げてくるかなり無茶な弾。
	-------------------------------------------------------
	delta256	フレーム毎に下方向にどれだけ加速するか
---------------------------------------------------------*/

// [***090114	追加
void bullet_create_hazumi_dan(SPRITE *src, int speed256, int angle512, int delta256, int bound_counts)
{
	SPRITE *h;			/*丸弾８(赤ＲＧＢ緑若黄青)*/
	h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_MARU16_PNG);/*緑黄弾*/
	h->type 			= BULLET_MARU8_00_AKA+(4+bound_counts);/*S P_BULLET*/ /*SP_LASER*/
	h->callback_mover	= bullet_move_hazumi_dan;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->an im_frame		= 0;//(4+bound_counts);/*とりあえず*/
	#if 1/*Gu(中心座標)*/
	h->x256 			= src->x256;
	h->y256 			= src->y256;
	#endif
//	HAZUMI_DAN_DATA *data;
//	data				= mmal loc(sizeof(HAZUMI_DAN_DATA));
//	h->data 			= data;
	/*angle512			= angle_jikinerai512_auto(player, src, angle512);*/
	mask512(angle512);
//	h->HAZUMI_DAN_DATA_angle512 	= (angle512);
//	h->HAZUMI_DAN_DATA_speed256 	= (speed256);
	regist_vector(h, speed256, angle512);

	h->HAZUMI_DAN_DATA_delta256 	= (delta256);
//	h->HAZUMI_DAN_DATA_sum256		= t256(0);
	h->HAZUMI_DAN_DATA_bound_counts = bound_counts;
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
/*bullet_create_sakuya_no_rot_knife*/
void bullet_create_jyuryoku_dan000(
	SPRITE *src,
	int speed256,
	int angle512,
	int delta256,
	int bullet_obj_type 	/* 弾グラ */	/*BULLET_MARU8_00_AKA+(7)*/ 	/*(BULLET_KNIFE20_05_AKA)*/
)
{
	SPRITE *h;			/*丸弾８(赤ＲＧＢ緑若黄青)*/
	h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_MARU16_PNG);/*青弾*/
	h->type 			= (bullet_obj_type);/*BULLET_MARU8_07_AOI*/  /*S P_BULLET*/ /*SP_LASER*/
	h->callback_mover	= bullet_move_gravity;	/*enemy_fall_knife_move*/
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->an im_frame		= 0;//(7)/*0*/;/*とりあえず*/
	#if 1/*Gu(中心座標)*/
	h->x256 			= src->x256;
	h->y256 			= src->y256;
	#endif
//	/*FALL_KNIFE_DATA*/GRAVITY_BULLET_DATA *data;
//	data				= mmal loc(sizeof(GRAVITY_BULLET_DATA/*FALL_KNIFE_DATA*/));
//	h->data 			= data;
//
	/*angle512			= angle_jikinerai512_auto(player, src, angle512);*/
	mask512(angle512);
//	data->angle512		= (angle512);
//	data->speed256		= (speed256);
	regist_vector(h, speed256, angle512);
	h->GRAVITY_BULLET_DATA_delta256 	= (delta256);
//	data->speed256		= t256(0);
}


/*---------------------------------------------------------
	咲夜用の黄色マスカット弾
	-------------------------------------------------------
	この弾は他の弾が速いのであえて遅くする。
---------------------------------------------------------*/

/*static*/ //void enemy_sp1_bullet_create(SPRITE *src, int speed256, int angle512, int gra256, int r_or_l, int *sd_angle512)

void sakuya_sp1_bullet_create_bbb(SPRITE *src)
{
	int j;
	for (j=(0/*k*/); j<(8/*-k*/); j++)
	{
		//static void sakuya_sp1_bullet_create(/*-1==L*/ /*, s_data_angle512*/ );/*1.26 == 2.1*0.6*//*, int *sd_angle512*/)
		SPRITE *h;			/*丸弾８(赤ＲＧＢ緑若黄青)*/
		h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_MARU16_PNG);	//	h->priority 	= PRIORITY_03_ENEMY;
		const unsigned int aaa = ((ra_nd()&(4-1)));
	//	h->type 			= BULLET_MARU8_00_AKA+4+(aaa);/*S P_BULLET*/ /*SP_LASER*/
		h->type 			= BULLET_KNIFE20_04_AOI+(aaa);/*S P_BULLET*/ /*SP_LASER*/
		h->callback_mover	= move_bullet_sakuya_sp1;
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//		h->an im_frame		= 0;// /*4*/(aaa)/*0*/;/*とりあえず*/
//		h->an im_speed		= 0;
		#if 1/*Gu(中心座標)*/
		h->x256 			= src->x256;
		h->y256 			= src->y256;
		#endif
		h->base_time_out	= 2048+20+(40+1);		/*data->timer		= 0;*/

		int angle512		= src->tmp_angleCCW512+((j-4)*42)/*deg_360_to_512(30)*/;
		mask512(angle512);
		h->m_angleCCW512 = angle512;
		h->vx256 = ((sin512((angle512))*t256(1.0))>>8);/*fps_factor*/
		h->vy256 = ((cos512((angle512))*t256(1.0))>>8);/*fps_factor*/

	//	BULLET_SP1_DATA *data;
	//	data				= mmal loc(sizeof(BULLET_SP1_DATA));
	//	h->data 			= data;
		h->BULLET_SP1_DATA_y_sum256 	= t256(1.0);	/* (2.5==5*0.5) */


//
	#if 1
	/* とりあえず、基本版 */
//	h->callback_2nd_regist	= callback_2nd_regist;
//
	/* とりあえず、テキトー */
	h->BULLET_DATA_2nd_count	= (999/*30*/);			/* 30カウントしたら半分速 */
	h->BASE_add_speed256		= (t256(1.00)); 	/* 加速/減速、なし */
	#endif
	}
}

#if 1/*魔方陣用*/
/*---------------------------------------------------------
	咲夜の魔方陣用
	angle512	512度指定
---------------------------------------------------------*/

/*static*/ void enemy_sp2_bullet_create01(SPRITE *src, int angle512, int star_remain_time/**sd_nnn*/)
{		/*丸弾８(赤ＲＧＢ緑若黄青)*/
	SPRITE *h;
	h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_MARU16_PNG);	//	h->priority 	= PRIORITY_03_ENEMY;
	h->type 			= BULLET_MARU8_00_AKA+(0);/*S P_BULLET*/
	h->callback_mover	= move_bullet_sakuya_sp2;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->an im_frame		= 0;//(0);
//	h->an im_speed		= 0;
	#if 1/*Gu(中心座標)*/
	h->x256 			= src->x256;
	h->y256 			= src->y256;
	#endif
	h->vx256			= (0);
	h->vy256			= (0);

//	BULLET_SP2_DATA *data;
//	data				= mmal loc(sizeof(BULLET_SP2_DATA));
//	h->data 			= data;
//	data->sd			= (BOSS05_MAHO_DATA *)s->data;
	h->BULLET_SP2_DATA_star_remain_time 	= star_remain_time;
	h->BULLET_SP2_DATA_angle512 	= (angle512);
	h->BULLET_SP2_DATA_speed256 	= t256(0);

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
---------------------------------------------------------*/
// bullet_create_tomari_dan
void bullet_create_tomari2_dan(
	SPRITE *src,
	int speed256,	/* 弾速 */
	int angle512,	/* 角度 */
	int delta256,	/* 減速定数 */
	int next_angle512
)
{
	SPRITE *h;
//	h					= sprite_add_only_bullet(TAMA_TYPE_KUGEL_PNG);/*小赤弾*/
	h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_MARU16_PNG);		/*丸弾８(赤ＲＧＢ緑若黄青)*/
	h->type 			= BULLET_MARU8_00_AKA+(0);/*S P_BULLET*/
	h->callback_mover	= enemy_stop_bullet2_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->an im_frame		= 0;//(0);
	#if 1/*Gu(中心座標)*/
	h->x256 			= src->x256;
	h->y256 			= src->y256;
	#endif
//	TOMARI_DAN_DATA *data;
//	data				= mmal loc(sizeof(TOMARI_DAN_DATA));
//	h->data 			= data;
	angle512			= angle_jikinerai512_auto(player, src, angle512);
//	data->angle512		= (angle512);
//	data->speed256		= (speed256);
	h->TOMARI_DAN_DATA_speed256 	= (speed256);
	regist_vector(h, t256(1.0)/*speed256*/, angle512);
	h->TOMARI_DAN_DATA_init_vx256	= /*data->*/h->vx256;
	h->TOMARI_DAN_DATA_init_vy256	= /*data->*/h->vy256;
//
	//data->state		= 0;
	h->TOMARI_DAN_DATA_delta256 	= (delta256);
	if (ANGLE_NO_SHOT_DAN==next_angle512)/* 特別に撃たない弾、の場合 */
	{
;// 	/*data->*/h->base_time_out	= 800+64;
	}
	else
	{
;// 	/*data->*/h->base_time_out	= 100+64;
		mask512(next_angle512);
	}
	/*data->next_angle512*/h->TOMARI_DAN_next_angle512 = (next_angle512);
	/*data->*/h->base_time_out	= 1024;/* 適当に大きく1024==(512+64+最大停止時間) */
}


/*---------------------------------------------------------
	くるくるナイフ(青→緑)
	-------------------------------------------------------
	全方向ナイフ(青)
	狙いナイフ(緑)
---------------------------------------------------------*/

void bullet_create_sakuya_kurukuru_knife(void)
//	SPRITE *src,
//	int speed256,	/* 弾速 */
//	int angle512,	/* 角度 */
//	int height		/* (?) */
//)
{/* x, y*/
	SPRITE *h;
	h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_KNIFE18_PNG);/*全方向ナイフ(青)*/
	h->type 			= /*BULLET_KNIFE20_07_MIDORI*/BULLET_KNIFE20_04_AOI;/*BULLET_KNIFE20_04_AOI*/	/* (青→緑ナイフに変身) */		/*S P_BULLET*/ /*SP_LASER*/
	h->callback_mover	= bullet_move_kurukuru_knife;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->an im_frame		= 0;//0;
//	h->x256 			= send1_obj->x256;
//	h->y256 			= send1_obj->y256/*x*/;
	#if 1/*Gu(中心座標)*/
	h->x256 			= send1_obj->x256;
	h->y256 			= send1_obj->y256;
	#endif
	/*data->*/h->base_time_out		= 300;
	h->knife_tmp_angle512/*data->aaa_angle512*/ 	= (send1_obj->BULLET_REGIST_angle512);
//
//	KURUKURU_KNIFE_DATA *data;
//	data				= mmal loc(sizeof(KURUKURU_KNIFE_DATA));
//	h->data 			= data;
	{
		int tmp_speed256;
		tmp_speed256 = (send1_obj->BULLET_REGIST_speed256);
		h->KURUKURU_KNIFE_DATA_aaa_speed256 	= (tmp_speed256/*+tmp_speed256*/);	/* 青ナイフの投げ初速度 */
		h->KURUKURU_KNIFE_DATA_speed_2nd_256	= (tmp_speed256+tmp_speed256);		/* 緑ナイフの2nd狙い速度 */
	}
	h->KURUKURU_KNIFE_DATA_target			= 0;
	h->KURUKURU_KNIFE_DATA_height			= (send1_obj->BULLET_REGIST_sakuya_kurukurku_knife_height);
}


/*---------------------------------------------------------
	両手ナイフ(青)
	-------------------------------------------------------
	length	enemy-player間の距離/√2
	r_or_l	0==右	1==左
---------------------------------------------------------*/

void bullet_create_sakuya_ryoute_knife(SPRITE *src)
//, int speed256/*, int dummy_length256*/, int r_or_l)/*0==右*/ 	/*1==左*/
{
int speed256;
	speed256	= t256(10);
	int r_or_l;
//	for (r_or_l=0; r_or_l<2; r_or_l++)
	for (r_or_l=-1; r_or_l<2; r_or_l+=2)
	{
		SPRITE *h;
		h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_KNIFE18_PNG);/*全方向ナイフ(青)*/ /*h->use_alpha=0 ????*/
		h->type 			= BULLET_KNIFE20_04_AOI;/*S P_BULLET*/ /*SP_LASER*/
	//
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	//	h->an im_frame		= 0;//0;
		#if 1/*Gu(中心座標)*/
		h->x256 			= src->x256;
		h->y256 			= src->y256;
		#endif
		/*data->*/h->base_time_out		= (150+difficulty*50);
	//
//		EVEN_KNIFE_DATA *data;
//		data				= mmal loc(sizeof(EVEN_KNIFE_DATA));
//		h->data 			= data;
	//
		int tmp512;
		tmp512 = angle_jikinerai512(player,src);/*???(original)*/ /* 奇数弾の場合に自機狙い */
		h->callback_mover	= enemy_even_knife_move;
		#if 0
	//	if (1==r_or_l)	/* pspは0レジスタがあるので0と比較したほうが速い */
		if (0!=r_or_l)
		{
	//		h->callback_mover	= enemy_evenl_knife_move;
			tmp512 += (64)/*M_PI*2*1/8*/;
			data->l_or_r			= -1;
		}
		else
	//	if (0==r_or_l)
		{
	//		h->callback_mover	= enemy_evenr_knife_move;
			tmp512 -= (64)/*M_PI*2*1/8*/;
			data->l_or_r			= 1;
		}
		#else
		h->EVEN_KNIFE_DATA_l_or_r			= r_or_l;
		tmp512 -= (64*r_or_l)/*M_PI*2*1/8*/;
		#endif
		mask512(tmp512);
	//	if (0==length256)	{length256=1*256;}
	//	data->length256 	= length256;
	//	data->x256			= /*s->x256+((s->w128))*/h->x256+((cos512((tmp512))*length256)>>8);
	//	data->y256			= /*s->y256+((s->h128))*/h->y256+((sin512((tmp512))*length256)>>8);
	//	data->x256			= /*s->x256+((s->w128))*/h->x256+((sin512((tmp512))*ryoute_knife_length256/*length256*/)>>8);
	//	data->y256			= /*s->y256+((s->h128))*/h->y256+((cos512((tmp512))*ryoute_knife_length256/*length256*/)>>8);
		h->EVEN_KNIFE_DATA_x256 		= /*s->x256+((s->w128))*/h->x256+((sin512((tmp512)) )<<7 );
		h->EVEN_KNIFE_DATA_y256 		= /*s->y256+((s->h128))*/h->y256+((cos512((tmp512)) )<<7 );
		h->EVEN_KNIFE_DATA_angle512 	= /*ra d2deg512*/(/*deg512_2rad*/(tmp512));
	//	h->EVEN_KNIFE_DATA_d_angle512	= ra d2deg512(speed/(M_PI*2*length));	//⊿angle≒任意/(2π×length/√2)
	//	int tmp_512;
	//	tmp_512=speed/(/*M_PI*2*/512*length);	//⊿angle≒任意/(2π×length/√2)
		h->EVEN_KNIFE_DATA_d_angle512	= 1;//	  /*ra d2deg512*/(/*deg512_2rad*/(tmp_512));
	//
		h->EVEN_KNIFE_DATA_speed256 	= /*10*/t256(10)/*8+difficulty*/ /*speed*/;/*よくわかんないが止まっちゃう*/
		h->EVEN_KNIFE_DATA_state		= 0;
		h->EVEN_KNIFE_DATA_wait1		= 0;
	}
}


/*---------------------------------------------------------
	多方向、等速直進弾を追加する
	-------------------------------------------------------

---------------------------------------------------------*/
/*(int)(512/24)*/
void bullet_regist_basic(void)
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
{
	send1_obj->BULLET_REGIST_angle512			= angle_jikinerai512_auto(player, send1_obj/*src*/, send1_obj->BULLET_REGIST_angle512); /*ANGLE_JIKI_NERAI_DAN???(original)*/ /* 奇数弾の場合に自機狙い */
	int i_angle512;
	i_angle512			= send1_obj->BULLET_REGIST_angle512-((send1_obj->BULLET_REGIST_n_way+2)*((send1_obj->BULLET_REGIST_div_angle512>>1)/*(int)(512/48)*/))/*ra d2deg512((M_PI*2)/48))*/; /* 48分割弾 */ // /* π/24 0.13089969389957471827 */
	/* 念の為マスク */
	mask512(i_angle512);
//
	int i;
	for (i=0; i<send1_obj->BULLET_REGIST_n_way; i++)
	{
		SPRITE *h;
		// frameはフレーム数-1 /*char *filename, int frame,*/
		h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_DUMMY/*_MING32_PNG*/);
		h->type 			= (send1_obj->BULLET_REGIST_bullet_obj_type);/*BULLET_UROKO14_01_AKA+*/ /*S P_BULLET*/
		h->callback_mover	= move_bullet_vector;
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		#if 1/*Gu(中心座標)*/
		h->x256 			= /*src*/send1_obj->x256;
		h->y256 			= /*src*/send1_obj->y256;
		#endif
//		BULLET_VECTOR_DATA *data;
//		data				= mmal loc(sizeof(BULLET_VECTOR_DATA));
//		h->data 			= data;
		i_angle512			+= send1_obj->BULLET_REGIST_div_angle512;	/*(2)*((int)(512/48))*/ /* π/12 0.26179938779914943654 */
		mask512(i_angle512);
	//	data->angle512		= /*deg512_2rad*/(i_angle512);	//
	//	data->speed256		= speed256;
		#if 1
		/* 描画用角度(下が0度で左回り(反時計回り)) */
	//	h->m_angleCCW512		= i_angle512;
	//	int aaa512; 	aaa512 = (/*127*/255-i_angle512);	mask512((aaa512));
//		int aaa512; 	aaa512 = (/*127*/127-i_angle512);	mask512((aaa512));
		int aaa512; 	aaa512 = (i_angle512);	mask512((aaa512));
		h->m_angleCCW512		= aaa512;
		#endif
		regist_vector(h, send1_obj->BULLET_REGIST_speed256, i_angle512);
	}
}

#if 0
/*---------------------------------------------------------
	統べての弾幕を共通化
	-------------------------------------------------------

---------------------------------------------------------*/

BULLET_STATUS bullet_resource[BULLET_RES_MAX] =
{
	{	&send1_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&send1_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&send1_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&send1_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&send1_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&send1_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&send1_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&send1_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
};
void bullet_create_resource(int type)
{

}
#endif

/*---------------------------------------------------------
	ノーマル弾
---------------------------------------------------------*/
/*bullet_create_maru8_frame*/
/*---------------------------------------------------------
	出現位置オフセット弾を作成する
	針弾を作成する
---------------------------------------------------------*/
void bullet_create_offset_dan_type000(
	SPRITE *src,			/*	*/
	int speed256,			/* 弾速 */
	int angle512,			/* 発射中心角度 / 特殊機能(自機狙い/他) */
	int bullet_obj_type 	/* 弾グラ */
)
	/*, int x_offset256, int y_offset256*/
{
		send1_obj->x256 = src->x256;
		send1_obj->y256 = src->y256;
		send1_obj->BULLET_REGIST_speed256			=	speed256;				/* 弾速 */
		send1_obj->BULLET_REGIST_angle512			=	angle512;
	//	send1_obj->BULLET_REGIST_div_angle512		=	(0);				/* ダミー角度(未使用) */
		send1_obj->BULLET_REGIST_bullet_obj_type	=	bullet_obj_type;	/* [赤弾] */
		send1_obj->BULLET_REGIST_n_way				=	(1);				/* [1way] */
		bullet_regist_basic();
}


/*---------------------------------------------------------
	プレイヤー狙い弾(赤)を作成する
---------------------------------------------------------*/
void bullet_create_aka_maru_jikinerai(
	SPRITE *src,
	int speed256
)
{
	int bullet_obj_type;	bullet_obj_type 	=	BULLET_MARU8_00_AKA;	/* 弾グラ */
	int angle512;			angle512			=	ANGLE_JIKI_NERAI_DAN;	/* 自機狙い弾 */
//
	bullet_create_offset_dan_type000(src, speed256, angle512, bullet_obj_type);
}


/*---------------------------------------------------------
	咲夜用の大弾(赤)。
	-------------------------------------------------------
	見た目＞＞あたり判定です。
---------------------------------------------------------*/

static void bullet_create_oodama22(
	SPRITE *src
//,
//	int speed256,
//	int angle512//,
//	int ransu512//,
	//int add_speed256
)/*, int xoffs256, int yoffs256*/
{
//	angle512			= angle_jikinerai512_auto(player, src, angle512);
//	mask512(angle512);
//
	int angle512;
	angle512			= (src->TOMARI_DAN_next_angle512);
//
	SPRITE *h;
	h					= sprite_add_only_bullet(TAMA_TYPE_OODAMA_08_PNG);	 /* 大弾(赤) 表示部分*/
//	h->priority 		= PRIORITY_03_ENEMY;
//	unsigned int aaa = (((angle512>>6)&(0x05))|(2));
	unsigned int aaa = (((angle512>>6)&(0x03)));
	h->type 			= BULLET_OODAMA32_00_SIROI+(aaa);/*S P_BULLET*/ /*SP_BIGBULLET*/
	h->callback_mover	= move_bullet_vector;	/* (要加速機能) */	/*move_bullet_oodama2_standard*/
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->an im_frame		= 0;//(aaa);/*(2) ((rand()&(0x05))|(2))*/
	#if 1/*Gu(中心座標)*/
	h->x256 			= src->x256;
	h->y256 			= src->y256;
	#endif
//	OODAMA_OYA_DATA *data;
//	data				= mmal loc(sizeof(OODAMA_OYA_DATA));
//	h->data 			= data;
//	h->OODAMA_OYA_DATA_add_speed256 = add_speed256;
//
//	h->OODAMA_OYA_DATA_angle512 	= (angle512);
//	h->OODAMA_OYA_DATA_speed256 	= speed256;
	int speed256;
	speed256 = (t256(1.00));
	regist_vector(h, speed256, angle512);
	h->BULLET_DATA_2nd_count	= (300);			/* 30カウントしたら半分速 */
	h->BASE_add_speed256		= (t256(1.05) );	/* 加速 */ /*add_speed256*/
}

/*---------------------------------------------------------
	天狗用
	ICE弾
	-------------------------------------------------------
---------------------------------------------------------*/

/*static*/ void bullet_create_aya_ice(SPRITE *src)
{
	int iii;
	int j;
	int rand_add_angle;
	rand_add_angle = (ra_nd()&(8-1));
	static const s8 step_tbl[(4)] =
	{
		(512/24),	/* easy */
		(512/28),	/* normal */
		(512/30),	/* hard */
		(512/32),	/* lunatic */
	};
	for (j=0; j</*2*/(1+difficulty); j++)
	{
		for (iii=(0); iii<(512/*-64*/); iii+=step_tbl[(difficulty)]/*(16)*/ )/* (512-32)(30<<4) (1<<4) ICE_NU M_OF_ENEMIES  */
		{
			SPRITE		*h;
			h					= sprite_add_only_bullet(TAMA_TYPE_BULLET_JIPPOU32_PNG);
			h->type 			= BULLET_KUNAI12_01_AKA;/*BULLET_CAP16_04_SIROI*/	/*S P_BULLET*/ /*S P_BO SS02ICE*/
			h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
			h->callback_mover	= enemy_boss02_ice_move;

		//	h->x256 			= src->x256 + ((0==j)?(t256(-19)):(t256(81)));
		//	h->x256 			= src->x256 + ((0==j)?(t256(-64)):(t256(64)));
		{	s16 aaa[4]			=	{t256(-64),t256(64),t256(-32),t256(32)};
			h->x256 			= src->x256 + aaa[(j)];
		}	h->y256 			= src->y256 /*+ t256(135 138)*/;
			/*data->*/h->base_time_out		= (120)/*100*/;
//
//			BOSS02_ICE_DATA *data;
//			data				= mmal loc(sizeof(BOSS02_ICE_DATA));
//			h->data 			= data;
//
		//	h->ice_add_angle512 = (j == 0)?(deg_360_to_512(+5)):(deg_360_to_512(-5));
			h->ice_add_angle512 = rand_add_angle-j;
			h->ice_angle512 	= (iii)/* (i<<4) deg_360_to_512(90) */;
//
			h->BOSS02_ICE_DATA_state		= 0;
		//	h->BOSS02_ICE_DATA_next_2nd_state	= KS01; 	/* バラバラ移動 */
			h->BOSS02_ICE_DATA_next_2nd_state	= KS02; 	/* 何もしない(等速直線移動) */
			h->BOSS02_ICE_DATA_speed256 	= t256(1.2);	/* (固定速度) */	/* (t256(2.5)+(difficulty<<6)) (t256(4.0)) */	/* (difficulty x 64) */
		}
		rand_add_angle = -rand_add_angle;
	}
}

