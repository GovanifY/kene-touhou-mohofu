
#include "game_main.h"

/*---------------------------------------------------------
  東方模倣風 ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	移動量弾(ベクトル弾)
	-------------------------------------------------------
	Todo:
	現在の模倣風は特殊弾が多すぎる。「特殊弾の方式はpspでは遅すぎる」ので弾幕にするのは原理的に無理。
	特殊弾にしなくてもまったく同じ事が(複数の)通常弾(の組み合わせで)で実現できる。
	「複数の通常弾の組み合わせ」なら原理的に速いので弾幕にするのは原理的に可能。
	(単純な)通常弾を100種類ぐらいに増やしても大丈夫かもしれない。
	-------------------------------------------------------
	速度を考えると特殊弾の中でsw itch()case:させるのは論外(に遅い)。
	GCCはどの version / 最適化 でもsw itch()case:はR系はジャンプテーブルに展開しない。
	最低でもif ()か、ジャンプテーブルにする。
	VC++(MSC)ではジャンプテーブルを明示できないが、GCCはジャンプテーブルを作れる。
	VC++はその分sw itch()case:でdafault:が無い場合、ジャンプテーブルを生成する。(最適化による)
	-------------------------------------------------------
	要するに「弾が変身(状態変更)」させる方式は遅いので無理。
	「ボスが変身(状態変更)」させる方式は十分速い。
---------------------------------------------------------*/

/*---------------------------------------------------------
	弾幕作成 / 爆発作成
	-------------------------------------------------------
	後で変化する弾を作るかも。
	ボスを倒した時に消えるようにしてある。
	-------------------------------------------------------
	もちついて本家見直してみる。
	本家の弾幕に「変身機能なんて無い」。
	単に弾幕リスト見て、弾幕番号で書き換えてる気がしてきた。
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


/*---------------------------------------------------------

---------------------------------------------------------*/

global BULLET_REGIST br;

#define callback_2nd_regist 		callback_loser	/* ２nd登録 */

/*---------------------------------------------------------
	ベクトル弾(基本的には、角度変化しない弾)
---------------------------------------------------------*/

/* 基本弾(速度、角度、ともに変化しない) */
/* 応用弾(速度変化する) */

#define BASE_add_speed256			user_data00 	/* 加減速 */
#define TOKUSYU_DAN_DATA_delta256	user_data01 	/* 加減速調整 */
#define TOMARI_DAN_next_angle1024	user_data02 	/* 変身後の角度 */			/*tmp_angleCCW1024*/

/*---------------------------------------------------------
	とりあえず移行用(2)追加弾、基本機能
---------------------------------------------------------*/

#define BULLET_DATA_2nd_count		user_data04/*10*/	/* ２nd 変身 */
#define BULLET_DATA_2nd_speed		user_data05/*11*/	/* ２nd 速度 */
#define BULLET_DATA_2nd_vx256		user_data06/*12*/	/* ２nd vx256 */
#define BULLET_DATA_2nd_vy256		user_data07/*13*/	/* ２nd vy256 */

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
		#if (0)
	//	src->vx256 = ((src->vx256*src->BASE_add_speed256)>>8);/* 加速/減速 */	// 負の場合ダメ。
		#else
		const int vx256_speed = (src->vx256*src->BASE_add_speed256);
		if (0 < vx256_speed) /* 正の場合 */
		{
			src->vx256 = ((vx256_speed)>>8);/* 加速/減速 */
		}
		else /* 負の場合 */
		{
			src->vx256 = -((-vx256_speed)>>8);/* 加速/減速 */
		}
		#endif
		//
		#if (1)/* 悪影響がさしてないので... */
		src->vy256 = ((src->vy256*src->BASE_add_speed256)>>8);/* 加速/減速 */	// 負の場合ダメ。
		#else
		const int vy256_speed = (src->vy256*src->BASE_add_speed256);
		if (0 < vy256_speed) /* 正の場合 */
		{
			src->vy256 = ((vy256_speed)>>8);/* 加速/減速 */
		}
		else /* 負の場合 */
		{
			src->vy256 = -((-vy256_speed)>>8);/* 加速/減速 */
		}
		#endif
	}
	src->cx256 += (src->vx256); 	/*fps_factor*/
	src->cy256 += (src->vy256); 	/*fps_factor*/
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

	-------------------------------------------------------
	弾幕書き換え方式に移行するのでここは無くなる予定。
---------------------------------------------------------*/

#define TOMARI_DAN_LIMIT_00_MAX 	(512+512)	/* 適当に大きく1024==(512+64+最大停止時間) */
#define TOMARI_DAN_LIMIT_00_640 	(512+128)
#define TOMARI_DAN_LIMIT_00_576 	(512+64)
#define TOMARI_DAN_LIMIT_01_512 	(512+0)
#define TOMARI_DAN_LIMIT_02_000 	(32+0+0)

static void bullet_create_oodama22(SPRITE *src);
static void bullet_move_tomari_dan(SPRITE *src)
{
	if (TOMARI_DAN_LIMIT_00_640 < src->jyumyou)
	{
		/* 止まる */
	//	src->BASE_add_speed256 -= (1);
		src->BASE_add_speed256 -= (src->TOKUSYU_DAN_DATA_delta256);
		if (src->BASE_add_speed256 < t256(0))
		{
			src->BASE_add_speed256 = t256(0);
			src->jyumyou = (TOMARI_DAN_LIMIT_00_640);
		}
	}
	else
	if (TOMARI_DAN_LIMIT_00_576 < src->jyumyou)
	{
		/* 撃つ(大弾に変身) */
		src->type		= BULLET_MINI8_00_SIRO + (( src->jyumyou>>3)&0x07); 	/* 警告(変身するので) */
	}
	else
	if (TOMARI_DAN_LIMIT_01_512 < src->jyumyou)
	{
		if (ANGLE_NO_SHOT_DAN != src->TOMARI_DAN_next_angle1024 )
		{
			bullet_create_oodama22(src);
			src->jyumyou = (TOMARI_DAN_LIMIT_02_000);
		}
		else
		{
			src->jyumyou = (TOMARI_DAN_LIMIT_01_512);
		}
	}
	else
	if (TOMARI_DAN_LIMIT_02_000 < src->jyumyou)
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
			src->jyumyou = JYUMYOU_NASI;
		}
	}
	/* 特別に撃たない弾、の場合 */
	if (ANGLE_NO_SHOT_DAN == src->TOMARI_DAN_next_angle1024 )
	{	/* スクロールする */
		src->cy256 += t256(1.0);	//	my_adjust_y(s,pd);
	}
	move_bullet_vector(src);
}


/*---------------------------------------------------------
	重力弾の移動
	垂直降下ナイフ(赤)
---------------------------------------------------------*/

static void bullet_move_gravity(SPRITE *src)
{
	src->vy256 += src->TOKUSYU_DAN_DATA_delta256;
	move_bullet_vector(src);
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
	h					= obj_add_00_tama_error();	/* 大弾(赤) 表示部分*/
	if (NULL != h)
	{
		h->cx256			= src->cx256;
		h->cy256			= src->cy256;
	//	angle512			= angle_jikinerai512_auto(obj_player, src, angle512);
	//	mask512(angle512);
		int angle1024;
		angle1024			= (src->TOMARI_DAN_next_angle1024);
	//
		h->type 			= BULLET_OODAMA32_00_AOI+((((angle1024>>7)&(0x03))));
		reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
		h->m_Hit256R		= TAMA_ATARI_OODAMA_08_PNG;
	//
		h->callback_mover	= move_bullet_vector;	/* (要加速機能) */
	//
		int speed256;
		speed256 = (t256(1.00));
		regist_vector(h, speed256, angle1024);
		h->BULLET_DATA_2nd_count	= (300);			/* 30カウントしたら半分速 */
		h->BASE_add_speed256		= (t256(1.05)); 	/* 加速 */
	}
}


/*---------------------------------------------------------
	重力弾。アリスの人形が撃つ弾。
	輝夜、最終形態で投げてくるかなり無茶な弾。bullet_create_sakuya_no_rot_knife
	ばらまき攻撃魔方陣(通称マンホール)がばらまく、止まる弾
	咲夜がばらまく、止まる弾。大弾に変化する
	-------------------------------------------------------
	下方向に加速していく中弾。
	フレーム毎で計算しているのでかなり重力加速度を小さくしないと大変なことに。
	delta256	フレーム毎に下方向にどれだけ加速するか
	-------------------------------------------------------
	angle512	ANGLE_JIKINERAI_DAN (999) でプレイヤー狙い
	delta256	フレーム毎にspeedをどれだけ減速するか
	-------------------------------------------------------
	弾幕書き換え方式に移行するのでここは無くなる予定。
---------------------------------------------------------*/
//	int br.BULLET_REGIST_00_speed256,					/* 弾速 */
//	int br.BULLET_REGIST_02_VECTOR_angle1024,			/* 角度 */
//	int br.BULLET_REGIST_04_bullet_obj_type,				/* 弾グラ */	/*(BULLET_KNIFE20_05_AKA)*/
//	int br.BULLET_REGIST_06_VECTOR_HANERU_KAISUU  int bound_counts
//	int br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024
//	int br.BULLET_REGIST_07_VECTOR_legacy_dan_delta256, /* 減速定数 */
static void s_bullet_create_haneru_dan(SPRITE *h)
{
	h->callback_mover	= bullet_move_gravity;
}

static void s_bullet_create_tomari_dan(SPRITE *h)// bullet_create_tomari_dan
{
	if (ANGLE_NO_SHOT_DAN==br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024)/* 特別に撃たない弾、の場合 */
	{
		;// 	h->jyumyou	= 800+64;
	}
	else
	{
		;// 	h->jyumyou	= 100+64;
		mask1024(br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024);
	}
	h->TOMARI_DAN_next_angle1024	= (br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024);
	h->jyumyou						= (TOMARI_DAN_LIMIT_00_MAX);
	h->callback_mover				= bullet_move_tomari_dan;
}


static void s_bullet_regist_legacy_angle(void)
{
	SPRITE *h;
	h					= obj_add_00_tama_error();
	if (NULL != h)
	{
		h->cx256 = obj_send1->cx256;
		h->cy256 = obj_send1->cy256;
		//
		h->type 			= (br.BULLET_REGIST_04_bullet_obj_type);
		reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
		h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
		//
		/*angle512			= angle_jikinerai512_auto(obj_player, src, angle512);*/
		mask1024(br.BULLET_REGIST_02_VECTOR_angle1024);
		regist_vector(h, br.BULLET_REGIST_00_speed256, br.BULLET_REGIST_02_VECTOR_angle1024);
		//
		h->TOKUSYU_DAN_DATA_delta256 	= (br.BULLET_REGIST_07_VECTOR_legacy_dan_delta256);
		if (LEGACY_REGIST_TYPE_00_HANERU==((br.BULLET_REGIST_03_VECTOR_regist_type)&0x03))
		{
			s_bullet_create_haneru_dan(h);		//	LEGACY_REGIST_TYPE_00_HANERU:
		}
		else
		{
			s_bullet_create_tomari_dan(h);		//	LEGACY_REGIST_TYPE_01_TOMARI:
		}
	}
}


/*---------------------------------------------------------
	多方向、等速直進弾を追加する
	-------------------------------------------------------
	よく考えたら、ANGLE_JIKI_NERAI_KISUDAN は必要ない。
	ANGLE_JIKINERAI_DAN だけあれば良く、
	発弾数に偶数を指定すれば偶数弾。
	発弾数に奇数を指定すれば奇数弾。
	あたりまえだけど、気が付かなかったww。
	-------------------------------------------------------
way 	補正角度
1		-0/2
2		-1/2
3		-2/2
4		-3/2
5		-4/2
---------------------------------------------------------*/
//	int BULLET_REGIST_00_speed256,			/* 弾速 */
//	int BULLET_REGIST_02_angle512, 			/* 発射中心角度 / 特殊機能(自機狙い/他) */
//	int BULLET_REGIST_04_bullet_obj_type,	/* 弾グラ */
//	int BULLET_REGIST_06_n_way				/* 発弾数 */
//	int BULLET_REGIST_div_angle512, 		/* 1周をn分割した角度 */
static void s_bullet_regist_multi_angle(void)
{
	int i_angle1024;
	i_angle1024 		= br.BULLET_REGIST_02_VECTOR_angle1024;
//	i_angle1024 		-= ((br.BULLET_REGIST_06_n_way+2)*((br.BULLET_REGIST_07_VECTOR_div_angle1024>>1)/*(int)(1024/48)*/)); /* 48分割弾 */ // /* π/24 0.13089969389957471827 */
	i_angle1024 		-= ((br.BULLET_REGIST_06_n_way-1)*((br.BULLET_REGIST_07_VECTOR_div_angle1024>>1)/*(int)(1024/48)*/)); /* 48分割弾 */ // /* π/24 0.13089969389957471827 */
//	mask1024(i_angle1024);	/* 念の為マスク */
	int i;
	for (i=0; i<br.BULLET_REGIST_06_n_way; i++)
	{
		SPRITE *h;
		h					= obj_add_00_tama_error();
		if (NULL != h)
		{
			h->cx256 = obj_send1->cx256;
			h->cy256 = obj_send1->cy256;
			h->type 			= (br.BULLET_REGIST_04_bullet_obj_type);
			reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
			h->m_Hit256R		= TAMA_ATARI_COMMON16_PNG;
		//
			h->callback_mover	= move_bullet_vector;
			i_angle1024 		+= br.BULLET_REGIST_07_VECTOR_div_angle1024;
			mask1024(i_angle1024);
			h->rotationCCW1024		= i_angle1024;		/* 描画用角度(下が0度で左回り(反時計回り)) */
			regist_vector(h, br.BULLET_REGIST_00_speed256, i_angle1024);
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


	/*---------------------------------------------------------
		自機(src)狙い弾指定の場合、角度を計算
	---------------------------------------------------------*/
static void auto_jikinerai(void)
{
	if (ANGLE_JIKI_NERAI_DAN==br.BULLET_REGIST_02_VECTOR_angle1024)
	{
		tmp_angleCCW65536_jiki_nerai(obj_send1);
		br.BULLET_REGIST_02_VECTOR_angle1024	= ((obj_send1->tmp_angleCCW65536)>>6);
	}
}
global void bullet_regist_multi_vector_send1_xy_src(SPRITE *src)
{
	send1_xy(src);	/* 弾源x256 y256 中心から発弾。 */
	auto_jikinerai();
	s_bullet_regist_multi_angle();
}

global void bullet_regist_multi_vector_direct(void)
{
	auto_jikinerai();
	s_bullet_regist_multi_angle();
}
global void bullet_regist_legacy_vector_send1_xy_src(SPRITE *src)
{
	send1_xy(src);	/* 弾源x256 y256 中心から発弾。 */
	s_bullet_regist_legacy_angle();
}

global void bullet_regist_legacy_vector_direct(void)
{
	s_bullet_regist_legacy_angle();
}
