
/* 中ボスがカード撃つ */
#include "./../boss/boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ザコは纏める予定。
---------------------------------------------------------*/


#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 			user_data00 	/* 目標x座標 */
	#define target_y256 			user_data01 	/* 目標y座標 */
	#define toutatu_wariai256		user_data02 	/* 目標座標への到達割合 */
	#define kougeki_anime_count 	user_data03 	/* 攻撃アニメーション用カウンタ */
	#define boss_time_out			user_data04 	/* 制限時間 */
	#define boss_base_state777		user_data04 	/* 制限時間(boss_time_outと同じ) */
#endif

//#define zako_pattern_base 			user_data07 		/* パターンの基準位置。アニメする場合のベース */
#define zako_anime_rotate_angle1024 	user_data07/*zako_anime_base*/	/* アニメ回転角度1024 */

/*---------------------------------------------------------
	攻撃方法
---------------------------------------------------------*/
enum
{
	ZAKO_SPEKA_00_hai_kedama1 = 0,// 灰毛玉のカード1
	ZAKO_SPEKA_01_hai_kedama2,// 灰毛玉のカード2
	ZAKO_SPEKA_02_hai_kedama3,// 灰毛玉のカード3
	ZAKO_SPEKA_03_hai_kedama4,// 灰毛玉のカード4
	ZAKO_SPEKA_04_hai_kedama5,// 灰毛玉のカード5
	ZAKO_SPEKA_05_hai_kedama6,// 灰毛玉のカード6
	ZAKO_SPEKA_06_hai_kedama7,// 灰毛玉のカード7
	ZAKO_SPEKA_07_hai_kedama8,// 灰毛玉のカード8
	//
	ZAKO_SPEKA_08_mahou_nerai,
	ZAKO_SPEKA_09_inyou1,
	ZAKO_SPEKA_0a_midori1,
	ZAKO_SPEKA_0b_meido2,
	ZAKO_SPEKA_0c_aka_kedama1,// 赤毛玉のカード1
	ZAKO_SPEKA_0d_yukari,
	ZAKO_SPEKA_0e_obake1,
	ZAKO_SPEKA_0f_mahou_common,
	//
	ZAKO_SPEKA_10_aaa,// 未使用
	ZAKO_SPEKA_11_mahou_tate,
	ZAKO_SPEKA_12_mahou_random,
	ZAKO_SPEKA_13_aaa,// 未使用
	ZAKO_SPEKA_14_mahou_qqq,//	4:円状攻撃のカード(小左)
	ZAKO_SPEKA_15_mahou_qqq,//	5:円状攻撃のカード(中右)
	ZAKO_SPEKA_16_mahou_qqq,//	6:円状攻撃のカード(大左)
	ZAKO_SPEKA_17_mahou_qqq,//	4:円状攻撃のカード(小左)
	//
	ZAKO_SPEKA_18_aaa,// 未使用
	ZAKO_SPEKA_19_midori2,
	ZAKO_SPEKA_1a_meido3,
	ZAKO_SPEKA_1b_ao2,
	ZAKO_SPEKA_1c_ao1,
	ZAKO_SPEKA_1d_karasu,
	ZAKO_SPEKA_1e_kedama1,
	ZAKO_SPEKA_1f_ao3_rendan,
	//
	ZAKO_SPEKA_99_MAX,
};
/*---------------------------------------------------------
	中-ボス敵やられ
---------------------------------------------------------*/
/*global*/static void item_create_15_rumia(OBJ *src)
{
	const u8 item_tbl[4] =
	{
		(SP_ITEM_02_P128&0xff), 	/* [F] */
		(SP_ITEM_01_P008&0xff), 	/* [P]大 */
		(SP_ITEM_00_P001&0xff), 	/* [p]小 */
		(SP_ITEM_05_TENSU&0xff),	/* [点] */
	};
	item_create_flags_num(src, /*item_type*/(OBJ_Z03_ITEM|(item_tbl[(cg.game_difficulty)])), (7), (ITEM_MOVE_FLAG_02_WAIT|ITEM_MOVE_FLAG_06_RAND_XY) );
	/* ちらばる */
}
/*global*/static void item_create_14_yokai(OBJ *src)
{
	item_create_flags_num(src, SP_ITEM_05_TENSU, (7), (ITEM_MOVE_FLAG_02_WAIT|ITEM_MOVE_FLAG_06_RAND_XY) );
	/* ちらばる */	/* 点数を出す */
}
/*---------------------------------------------------------

---------------------------------------------------------*/


/*global*/static/*static*/ void item_create_00_item(OBJ *src)	{	item_create_mono(src, SP_ITEM_00_P001  );}
/*global*/static/*static*/ void item_create_01_item(OBJ *src)	{	item_create_mono(src, SP_ITEM_01_P008  );}
/*global*/static/*static*/ void item_create_02_item(OBJ *src)	{	item_create_mono(src, SP_ITEM_02_P128  );}
/*global*/static/*static*/ void item_create_03_item(OBJ *src)	{	item_create_mono(src, SP_ITEM_03_1UP   );}
/*global*/static/*static*/ void item_create_04_item(OBJ *src)	{	item_create_mono(src, SP_ITEM_04_BOMB  );}
/*global*/static/*static*/ void item_create_05_item(OBJ *src)	{	item_create_mono(src, SP_ITEM_05_TENSU );}//static void item_create_018_meido4
/*global*/static/*static*/ void item_create_06_item(OBJ *src)	{	item_create_mono(src, SP_ITEM_06_HOSI  );}
#define item_create_07_item item_create_random_table

//static void item_create_018_meido4
//	h->callback_loser		= item_create_05_item;//item_create_018_meido4;
//	h->callback_loser		= item_create_04_item;//item_create_003_tatsumaki1;/*old*/


/*---------------------------------------------------------
	ザコ敵やられ
---------------------------------------------------------*/

/*global*/static/*static*/ void item_create_000_kougeki_mahoujin(OBJ *src)
{
	if (rand_percent(30))	{	item_create_mono(src, SP_ITEM_00_P001  );}
	if (rand_percent(30))	{	item_create_mono(src, SP_ITEM_00_P001  );}
	if (rand_percent(30))	{	item_create_mono(src, SP_ITEM_05_TENSU );}
}

static void zako_shot_supeka(OBJ *src, u8 zako_supeka_type);/*(宣言)*/
/*global*/static/*static*/ void item_create_002_inyou_zako(OBJ *src)
{
//	if (0==(ra_nd()&(16-1)))	/*確率上げた。[1/16]←[1/20]*/ /*%20*/
	if (0==(ra_nd()&( 8-1)))	/*確率上げた。[1/8]←[1/20]*/ /*%20*/
	{
		zako_shot_supeka(src, ZAKO_SPEKA_09_inyou1);
	}
	else
	{
		item_create_mono(src, (SP_ITEM_00_P001+(ra_nd()&1)) );
	}
}
/*50%(SP_ITEM_00_P001 or SP_ITEM_01_P008)*/
//66%==SP_ITEM_04_BOMB or 33%==SP_ITEM_01_P008	 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),


//static void item_create_003_tatsumaki1(OBJ *src)
//{
//	destoroy++;
//	if ((NUM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
//	{
//		destoroy = 0;
//		if (rand_percent(50/*10*/))
//		{
//			item_create(src, (SP_ITEM_04_BOMB), 1, ITEM_MOVE_FLAG_06_RAND_XY);
//		}
//	}
//}
/*50%(SP_ITEM_01_P008 or SP_ITEM_04_BOMB) (SP_ITEM_01_P008+(ra_nd()&1)) */
//66%==SP_ITEM_04_BOMB or 33%==SP_ITEM_01_P008 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),


//static void item_create_017_meido3(OBJ *src)
//{
//	destoroy[static_last]++;
//	if ((NUM_OF_ENEMIES-1) < destoroy[static_last]/*all_destoroy*/)
//	{
//		destoroy[static_last] = 0;
//		item_create(src, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
//	}
//}
//		if (0==(ra_nd()&(512-1)))	/* 1/512 ← 1/600 の確率確率でアイテム出す */	/*%600*/
//		{
//			item_create(src, SP_ITEM_00_P001, 1, ITEM_MOVE_FLAG_06_RAND_XY);
//		}


/*---------------------------------------------------------
	ザコの攻撃方法は纏めるべき
---------------------------------------------------------*/


/*---------------------------------------------------------
	道中専用止まり弾
	-------------------------------------------------------
	全弾書き換え方式に移行するのでここは無くなる予定。
---------------------------------------------------------*/
/*---------------------------------------------------------
	ベクトル弾(基本的には、角度変化しない弾)
---------------------------------------------------------*/

#define BASE_add_speed256			user_data00 	/* 加減速 */
#define TOKUSYU_DAN_DATA_delta256	user_data01 	/* 加減速調整 */




#define TOMARI_DAN_LIMIT_00_MAX 	(512+384)	/* 適当に大きく1024==(512+64+最大停止時間) */
#define TOMARI_DAN_LIMIT_01_512 	(512)
#define TOMARI_DAN_LIMIT_02_032 	(32+0+0)

static void bullet_move_douchu_tomari_dan(OBJ *src)
{
	if (TOMARI_DAN_LIMIT_01_512 < src->jyumyou)/* 513 ... MAX */
	{
		/* 止まる */
	//	src->BASE_add_speed256 -= (1);
		src->BASE_add_speed256 -= (src->TOKUSYU_DAN_DATA_delta256);
		if (src->BASE_add_speed256 < t256(0))
		{
			src->BASE_add_speed256 = t256(0);
			src->jyumyou = (TOMARI_DAN_LIMIT_01_512);
		}
	}
	else
	if (TOMARI_DAN_LIMIT_02_032 < src->jyumyou)/* 33 ... 512 */
	{
		;	/* しばし停止 */
	}
	else/* 0 ... 32 */
	{
		/* 消える */
		src->color32 -= 0x10000000; 		/*	src->alpha -= 0x10; */
		if (0x20000000 > src->color32)		/*	( 0x20 > src->alpha)	*/
		{
			src->color32 = 0x00ffffff;		/*	src->alpha = 0x00;	*/
			src->jyumyou = JYUMYOU_NASI;
		}
	}
	//
	src->cy256 += t256(1.0);	/* 道中専用弾なのでスクロールする */
	//
//	move_bullet_vector(src);
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

}

/*---------------------------------------------------------
	弾を登録する。
	-------------------------------------------------------
	弾発動時のエフェクト等の関係上、発弾部分は全部纏める必要がある。
	-------------------------------------------------------
	ベクトル弾と角度弾を統合するかもしれない。
	-------------------------------------------------------
	ToDo:
	弾システムに移行する為、廃止予定。
	カードスクリプトシステムが稼動する為には、発弾を一ヶ所に統合しないと
	速度的な面でこのままの形態では無理があるので、
	(ここの発弾処理のせいで他の弾が処理落ちする)
	ここの発弾処理を hatudan_system_regist_single に置きかえる必要がある。
---------------------------------------------------------*/
/* ベクトル特殊弾(仮、移行形態) */

#define hatudan_system_kousadan_angle65536 tmp_angleCCW1024
static void angle_to_vector(OBJ *src)/*(角度をX軸、Y軸のベクトル速度へ変換する)*/
{
		REG_0b_REG3 = ((src->hatudan_system_kousadan_angle65536));/*(交差弾用に発弾時の角度が65536であるので使う。)*/
			#if (0)//
			REG_02_DEST_X	= ((si n1024(((REG_0b_REG3)>>6))*(REG_09_REG1))>>8);	/* CCWの場合 */
			REG_03_DEST_Y	= ((co s1024(((REG_0b_REG3)>>6))*(REG_09_REG1))>>8);
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (REG_0b_REG3>>6), &sin_value_t256, &cos_value_t256);
				/*	spell_register[register_number_value]	*/	REG_0c_REG4 = (sin_value_t256);
				/*	spell_register[register_number_value+1] */	REG_0d_REG5 = (cos_value_t256);
				REG_02_DEST_X	= (((REG_0c_REG4)*(REG_09_REG1))>>8);
				REG_03_DEST_Y	= (((REG_0d_REG5)*(REG_09_REG1))>>8);
			}
			#endif
		src->vx256	= (REG_02_DEST_X);/*fps_factor*/
		src->vy256	= (REG_03_DEST_Y);/*fps_factor*/
}
static void bullet_regist_douchu_vector_tomari_dan(OBJ *src)
{
	OBJ *h;
	h										= obj_add_A00_tama_error();/* 発弾登録 */
	if (NULL != h)/* 登録できた場合 */
	{
		h->cx256							= REG_02_DEST_X;/* 発弾位置 座標x */
		h->cy256							= REG_03_DEST_Y;/* 発弾位置 座標y */
		h->obj_type_set 							= (HATSUDAN_05_bullet_obj_type);
		reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
		//
		mask65536(HATSUDAN_03_angle65536);
		h->hatudan_system_kousadan_angle65536	= (HATSUDAN_03_angle65536);
		h->m_Hit256R						= TAMA_ATARI_MARU16_PNG;
		//
		/*angle512							= angle_jikinerai512_auto(obj_player, src, angle512);*/
		REG_09_REG1 						= (HATSUDAN_01_speed256);
		angle_to_vector(h);
		//
		/*user_data02==(r35-弾幕システム用途でレイヤー管理しているので重要)*/
		h->user_data02						= (0);/*(r35レガシーシステム側で、使わなくても、弾幕システム側で使うのでここで設定する必要がある。)*/
		//
		/* とりあえず、テキトー */
	//	h->BASE_add_speed256				= (t256(1.00)); 	/* 加速/減速、なし */
		h->BASE_add_speed256				= 1+(t256(1.00));	/* 加速 弾 */
		//
		h->TOKUSYU_DAN_DATA_delta256		= (HATSUDAN_02_speed_offset);
		h->jyumyou							= (TOMARI_DAN_LIMIT_00_MAX);
		h->callback_mover					= bullet_move_douchu_tomari_dan;
	}
}


/*---------------------------------------------------------
	ザコの攻撃方法は纏めるべき
---------------------------------------------------------*/
typedef struct /*aa*/
{
	int bullet_obj_type;	// HATSUDAN_05_bullet_obj_type
//	int aaa;				// aaa
	u16 kankaku;
	u16 speed256;			// HATSUDAN_01_speed256
	u16 n_way;				// HATSUDAN_06_n_way
	u16 div_angle65536; 	// HATSUDAN_07_div_angle65536
} ZAKO_SPELL_RESOURCE;
static ZAKO_SPELL_RESOURCE kougeki_tbl[(ZAKO_SPEKA_99_MAX)] =
{
	/* 0:紅っぽい毛玉(狭角度 奇数弾) t256(2.5) */
	{(BULLET_KOME_BASE),						/*(0),*/ 0x3f, t256(1.25), (5), (u16)(65536/12)},	/* ZAKO_SPEKA_00_hai_kedama1[e] (u32)(1024/12):広角なので簡単 */
	{(BULLET_KOME_BASE),						/*(0),*/ 0x1f, t256(1.25), (3), (u16)(65536/16)},	/* ZAKO_SPEKA_01_hai_kedama2[n] 0x0f:普通に避けれる間隔 */
	{(BULLET_KOME_BASE),						/*(0),*/ 0x0f, t256(1.25), (3), (u16)(65536/20)},	/* ZAKO_SPEKA_02_hai_kedama3[h] 0x0f:普通に避けれる間隔 */
	{(BULLET_KOME_BASE),						/*(0),*/ 0x07, t256(1.25), (5), (u16)(65536/24)},	/* ZAKO_SPEKA_03_hai_kedama4[l] (u32)(1024/24):普通の狭角 */	/* 1:妖っぽい毛玉(90度 4方向) (無駄弾なので難易度は低い筈) */
	{(BULLET_KOME_BASE),						/*(0),*/ 0x3f, t256(1.25), (8), (u16)(65536/ 8)},	/* ZAKO_SPEKA_04_hai_kedama5[e] 8:見せ弾(無駄弾なので難易度は低い筈) */
	{(BULLET_KOME_BASE),						/*(0),*/ 0x1f, t256(1.25), (8), (u16)(65536/ 8)},	/* ZAKO_SPEKA_05_hai_kedama6[n] 0x1f:普通に避けれる間隔 */
	{(BULLET_KOME_BASE),						/*(0),*/ 0x07, t256(1.25), (4), (u16)(65536/ 4)},	/* ZAKO_SPEKA_06_hai_kedama7[h] */
	{(BULLET_KOME_BASE),						/*(0),*/ 0x03, t256(1.25), (4), (u16)(65536/ 4)},	/* ZAKO_SPEKA_07_hai_kedama8[l] 0x03:連続弾っぽく */
	//
	{(BULLET_MINI8_BASE + TAMA_IRO_03_AOI), 	/*(0),*/ 0x03, t256(0.5),	(1), (u16)(65536/ 1)},	/* ZAKO_SPEKA_08_mahou_nerai */ 		// 6 -	// [魔方陣 3/囲妖怪1:ランダムばらまき攻撃+自機狙い("魔方陣1")のカード]
	{(BULLET_HARI32_01_AKA),					/*(0),*/ 0x03, t256(2.5),	(5), (u16)(65536/24)},	/* ZAKO_SPEKA_09_inyou1 */				// 6 7	// [針弾]
	{(BULLET_MINI8_BASE + TAMA_IRO_01_AKA), 	/*(0),*/ 0x03, t256(1.375), (1), (u16)(65536/ 1)},	/* ZAKO_SPEKA_0a_midori1 */ 			// 6 -	// [緑毛玉1/メイド1 のカード]
	{(BULLET_HARI32_00_AOI),					/*(0),*/ 0x03, t256(2.5),	(1), (u16)(65536/ 1)},	/* ZAKO_SPEKA_0b_meido2 */				// 6 -	// [ メイド2のカード]
	{(BULLET_MINI8_BASE + TAMA_IRO_07_DAI_DAI), /*(0),*/ 0x03, t256(1.25),	(1), (u16)(65536/ 1)},	/* ZAKO_SPEKA_0c_aka_kedama1 */ // 6 -	// [ 赤毛玉1のカード]
	{(BULLET_MINI8_BASE + TAMA_IRO_07_DAI_DAI), /*(0),*/ 0x03, t256(0.75),	(1), (u16)(65536/ 1)},	/* ZAKO_SPEKA_0d_yukari */				// 6 -	// [ 紫編隊2のカード]
	{(BULLET_KNIFE20_04_AOI),					/*(0),*/ 0x03, t256(0.625), (4), (u16)(65536/18)},	/* ZAKO_SPEKA_0e_obake1 */				// . 7	// [おばけ1 のカード]
	{(BULLET_MINI8_BASE + TAMA_IRO_01_AKA), 	/*(0),*/ 0x03, t256(1),/*?*/(4), (u16)(65536/ 7)},	/* ZAKO_SPEKA_0f_mahou_common */		//		// [魔方陣 とりあえず共通部分]
	//
	{(0),										/*(0),*/ 0x03, 0,			(4), (u16)(65536/ 4)},	/* ZAKO_SPEKA_10_aaa-*/
	{(BULLET_KOME_BASE + TAMA_IRO_01_AKA),		/*(0),*/ 0x01, t256(1.25),	(2), (u16)(65536/ 2)},	/* ZAKO_SPEKA_11_mahou_tate */			// 6 7	// [魔方陣 1:縦攻撃のカード]
	{(BULLET_MINI8_BASE + TAMA_IRO_01_AKA), 	/*(0),*/ 0x03, t256(2.0),	(4), (u16)(65536/ 4)},	/* ZAKO_SPEKA_12_mahou_random */		// . .	// [魔方陣 2:ランダムばらまき攻撃("魔方陣2")のカード]
	{(0),										/*(0),*/ 0x03, 0,			(4), (u16)(65536/ 4)},	/* ZAKO_SPEKA_13_aaa-*/
	{(BULLET_MINI8_BASE + TAMA_IRO_01_AKA), 	/*(0),*/ 0x03, t256(1),/*?*/(4), (u16)(65536/ 7)},	/* ZAKO_SPEKA_14_mahou_qqq */		//		// [魔方陣 とりあえず共通部分]
	{(BULLET_MINI8_BASE + TAMA_IRO_01_AKA), 	/*(0),*/ 0x03, t256(1),/*?*/(4), (u16)(65536/ 7)},	/* ZAKO_SPEKA_15_mahou_qqq */		//		// [魔方陣 とりあえず共通部分]
	{(BULLET_MINI8_BASE + TAMA_IRO_01_AKA), 	/*(0),*/ 0x03, t256(1),/*?*/(4), (u16)(65536/ 7)},	/* ZAKO_SPEKA_16_mahou_qqq */		//		// [魔方陣 とりあえず共通部分]
	{(BULLET_MINI8_BASE + TAMA_IRO_01_AKA), 	/*(0),*/ 0x03, t256(1),/*?*/(4), (u16)(65536/ 7)},	/* ZAKO_SPEKA_0f_mahou_common */		//		// [魔方陣 とりあえず共通部分]
	//
	{(0),										/*(0),*/ 0x03, 0,			(4), (u16)(65536/ 4)},	/* ZAKO_SPEKA_18_aaa-*/
	{(BULLET_MINI8_BASE + TAMA_IRO_01_AKA), 	/*(0),*/ 0x03, t256(1.5),	(4), (u16)(65536/48)},	/* ZAKO_SPEKA_19_midori2 */ 			// . 7	// [緑毛玉2 のカード]
	{(BULLET_UROKO14_BASE + TAMA_IRO_01_AKA),	/*(0),*/ 0x03, t256(1.0),	(4), (u16)(65536/24)},	/* ZAKO_SPEKA_1a_meido3 */				// . 7	// [ メイド3のカード]
	{(BULLET_UROKO14_BASE + TAMA_IRO_03_AOI),	/*(0),*/ 0x03, t256(1.0),	(4), (u16)(65536/24)},	/* ZAKO_SPEKA_1b_ao2 */ 				// . 7	// [ 青妖精1/青妖精4のカード(B)]
	{(0),										/*(0),*/ 0x03, t256(1.0),	(8), (u16)(65536/24)},	/* ZAKO_SPEKA_1c_ao1 */ 				// 6 7	// [ 青妖精1/青妖精4のカード(A)]
	{(0),										/*(0),*/ 0x03, t256(1.0),	(8), (u16)(65536/ 8)},	/* ZAKO_SPEKA_1d_karasu */				// 6 7	// [ 青妖精3/烏1のカード]
	{(0),										/*(0),*/ 0x03, t256(0.5),	(4), (u16)(65536/24)},	/* ZAKO_SPEKA_1e_kedama1 */ 			// . 7	// [ 毛玉1のカード]
	{(0),										/*(0),*/ 0x03, t256(1.25),	(7), (u16)(65536/ 4)},	/* ZAKO_SPEKA_1f_ao3_rendan */			// 6 -	// [ 青妖精2のカード]
	//
};
//label_ZAKO_SPEKA_0d_yukari:
//	/*[r35要半分速]*/HATSUDAN_01_speed256				+= t256(2.5);				/* 弾速 */	/*高速弾*/	/*t256(3+teki_rank)+(0(teki_rank<<6))*/
//	/*[r35要半分速]*/HATSUDAN_01_speed256				+= t256((cg.game_difficulty))+t256(1.5);		/* 弾速 */
//仕様上ダメ(rank拡張)	/*[r35半分速]*/HATSUDAN_01_speed256 				+= ((cg.game_difficulty)<<7);		/* 弾速 */
//label_ZAKO_SPEKA_1d_karasu:	// [ 青妖精3/烏1のカード]
//	/*[r35要半分速]*/HATSUDAN_01_speed256				= (t256(2.0)+(((cg.game_difficulty))<<6));
//仕様上ダメ(rank拡張)	/*[r35半分速]*/HATSUDAN_01_speed256 				+= (((cg.game_difficulty))<<5);

//label_ZAKO_SPEKA_0d_yukari:	// [ 紫編隊2のカード]
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_MINI8_BASE + TAMA_IRO_07_DAI_DAI);	/* [赤弾] */ /* 弾グラ */
//		HATSUDAN_06_n_way								= (1);							/* [1way] */
//	//	HATSUDAN_07_div_angle65536						= (0);							/* ダミー角度(未使用) */
//		goto COMMON_JIKINERAI;

static void zako_shot_supeka(OBJ *src, u8 zako_supeka_type)
{
	#if 0/*(てすつ)*/
	if (0==(cg.game_difficulty))
	{
		return;
	}
	#endif
	const void *aaa[(ZAKO_SPEKA_99_MAX)] =
	{
		&&label_ZAKO_SPEKA_18_haikedama,	// ZAKO_SPEKA_00_hai_kedama1 灰毛玉 のカード1
		&&label_ZAKO_SPEKA_18_haikedama,	// ZAKO_SPEKA_01_hai_kedama2 灰毛玉 のカード2
		&&label_ZAKO_SPEKA_18_haikedama,	// ZAKO_SPEKA_02_hai_kedama3 灰毛玉 のカード3
		&&label_ZAKO_SPEKA_18_haikedama,	// ZAKO_SPEKA_03_hai_kedama4 灰毛玉 のカード4
		&&label_ZAKO_SPEKA_18_haikedama,	// ZAKO_SPEKA_04_hai_kedama5 灰毛玉 のカード5
		&&label_ZAKO_SPEKA_18_haikedama,	// ZAKO_SPEKA_05_hai_kedama6 灰毛玉 のカード6
		&&label_ZAKO_SPEKA_18_haikedama,	// ZAKO_SPEKA_06_hai_kedama7 灰毛玉 のカード7
		&&label_ZAKO_SPEKA_18_haikedama,	// ZAKO_SPEKA_07_hai_kedama8 灰毛玉 のカード8
		//
		&&COMMON_JIKINERAI, 				// ZAKO_SPEKA_08_mahou_nerai	魔方陣 3:ランダムばらまき攻撃+自機狙い("魔方陣1")のカード
		&&COMMON_JIKINERAI, 				// ZAKO_SPEKA_09_inyou1 		陰陽玉1 のカード [針弾]
		&&COMMON_JIKINERAI, 				// ZAKO_SPEKA_0a_midori1		緑毛玉1 のカード
		&&COMMON_JIKINERAI, 				// ZAKO_SPEKA_0b_meido2 		のカード
		&&COMMON_JIKINERAI, 				// ZAKO_SPEKA_0c_aka_kedama1	赤毛玉 のカード
		&&COMMON_JIKINERAI, 				// ZAKO_SPEKA_0d_yukari 	のカード
		&&label_ZAKO_SPEKA_0e_obake1,		// ZAKO_SPEKA_0e_obake1 おばけ1 のカード
		&&label_ZAKO_SPEKA_0f_mahou_common, // ZAKO_SPEKA_0f_mahou_common 魔方陣 とりあえず共通部分
		//
		&&COMMON_JIKINERAI, 				// mahoujin00 ZAKO_SPEKA_10_aaa [未使用]のカード	/* 攻撃しない */
		&&label_ZAKO_SPEKA_11_mahou_tate,	// mahoujin01 ZAKO_SPEKA_11_mahou_tate 魔方陣 1:縦攻撃のカード
		&&label_ZAKO_SPEKA_12_mahou_random, // mahoujin02 ZAKO_SPEKA_12_mahou_random 魔方陣 2:ランダムばらまき攻撃("魔方陣2")のカード	/* ばらまき #01(差分氏、旧"魔方陣2") */
		&&COMMON_JIKINERAI, 				// mahoujin03 ZAKO_SPEKA_13_aaa [未使用]のカード	/* ばらまき #02(差分氏、旧"魔方陣1") */
		&&label_ZAKO_SPEKA_14_mahou_qqq,	// mahoujin04 ZAKO_SPEKA_14_mahou_qqq [未使用]のカード	/* 8way花弾(中) */
		&&label_ZAKO_SPEKA_15_mahou_qqq,	// mahoujin05 ZAKO_SPEKA_15_mahou_qqq [未使用]のカード	/* 8way花弾(中) */
		&&label_ZAKO_SPEKA_16_mahou_qqq,	// mahoujin06 ZAKO_SPEKA_16_mahou_qqq [未使用]のカード	/* 8way花弾(中) */
		&&label_ZAKO_SPEKA_14_mahou_qqq,	// mahoujin04 ZAKO_SPEKA_17_mahou_qqq [未使用]のカード		/* 8way花弾 */
		//
		&&COMMON_JIKINERAI, 				// ZAKO_SPEKA_18_aaa [未使用]のカード
		&&label_ZAKO_SPEKA_19_midori2,		// ZAKO_SPEKA_19_midori2 緑毛玉2 のカード
		&&label_ZAKO_SPEKA_1a_meido3,		// ZAKO_SPEKA_1a_meido3 	のカード
		&&label_ZAKO_SPEKA_1b_ao2,			// ZAKO_SPEKA_1b_ao2		のカード
		&&label_ZAKO_SPEKA_1c_ao1,			// ZAKO_SPEKA_1c_ao1		のカード
		&&label_ZAKO_SPEKA_1d_karasu,		// ZAKO_SPEKA_1d_karasu 	のカード
		&&label_ZAKO_SPEKA_1e_kedama1,		// ZAKO_SPEKA_1e_kedama1	のカード
		&&label_ZAKO_SPEKA_1f_ao3_rendan,	// ZAKO_SPEKA_1f_ao3_rendan のカード
		//
	};
	zako_supeka_type &= 0x1f;/*(念の為)*/
	//
	HATSUDAN_05_bullet_obj_type 			= kougeki_tbl[(zako_supeka_type)].bullet_obj_type;
	//
	HATSUDAN_01_speed256					= ((kougeki_tbl[(zako_supeka_type)].speed256)+((cg.game_rank)>>8));
	HATSUDAN_06_n_way						= kougeki_tbl[(zako_supeka_type)].n_way;
	HATSUDAN_07_div_angle65536				= ((kougeki_tbl[(zako_supeka_type)].div_angle65536) );
	//
	set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
	//
	goto *aaa[(zako_supeka_type)/*& 0x1f*/];
label_ZAKO_SPEKA_18_haikedama:
	//static void zako_spell_01_hai_kedama(OBJ *src, u8 zako_supeka_type)
	{
		//	zako_supeka_type -= (ZAKO_SPEKA_18_hai_kedama);
		//	zako_supeka_type &= (0x07);
		if (0==(src->jyumyou & kougeki_tbl[(zako_supeka_type & 0x07)].kankaku))/*0x0f*/
		{
		//	/*[r35要半分速]*/HATSUDAN_01_speed256	+= ((((cg_game_di fficulty))<<6));/* [2.5 - 3.25] */		// (t256(2.0)+((difficulty)<<6)),/* [2.0 - 2.75] */
		//	/*[r35要半分速]*/HATSUDAN_01_speed256	+= ((((zako_supeka_type)&(4-1))<<6));/* [2.5 - 3.25] */ 	// (t256(2.0)+((difficulty)<<6)),/* [2.0 - 2.75] */
			/*[r35半分速]*/HATSUDAN_01_speed256 	+= (((zako_supeka_type & 0x03)<<5));/* [2.5 - 3.25] */		// (t256(2.0)+((difficulty)<<6)),/* [2.0 - 2.75] */
			HATSUDAN_05_bullet_obj_type 			= BULLET_KOME_BASE + (src->cx256 & 0x07);
			goto COMMON_JIKINERAI;
		}
		else
		{
			goto EXIT_ZAKO_SPEKA;
		}
	}
		goto EXIT_ZAKO_SPEKA;
//-------------------------
label_ZAKO_SPEKA_14_mahou_qqq:
		//	HATSUDAN_01_speed256						= (t256(1.5)+(cos1024((src->jyumyou<<4)))); 				/* 弾速 */
/*...*/ //	HATSUDAN_01_speed256						= (t256(1.5)+(sin1024((src->jyumyou<<4)+255))); 			/* 弾速 */
		//	attack_kougeki_mahoujin_common(src);
		goto label_muki_mahou_right;
label_ZAKO_SPEKA_15_mahou_qqq:
		//	HATSUDAN_01_speed256						= (t256(1.0)+(cos1024((src->jyumyou<<3))<<2));					/* 弾速 */
/*...*/ //	HATSUDAN_01_speed256						= (t256(1.0)+(sin1024((src->jyumyou<<3)+255)<<2));				/* 弾速 */
		goto label_muki_mahou_left;
label_ZAKO_SPEKA_16_mahou_qqq:
		//	HATSUDAN_01_speed256						= (t256(1.0)+(cos1024((src->jyumyou<<2))<<2));					/* 弾速 */
/*...*/ //	HATSUDAN_01_speed256						= (t256(1.0)+(sin1024((src->jyumyou<<2)+255)<<2));				/* 弾速 */
		goto label_muki_mahou_right;
//-------------------------
label_muki_mahou_left:
		HATSUDAN_03_angle65536							= (((-(src->jyumyou+src->jyumyou)+((src->jyumyou&0x07)<<7)))<<6);	/* 向き */
		goto label_ZAKO_SPEKA_0f_mahou_common;
label_muki_mahou_right:
		HATSUDAN_03_angle65536							= ((( (src->jyumyou+src->jyumyou)+((src->jyumyou&0x07)<<7)))<<6);	/* 向き */
	//	goto label_ZAKO_SPEKA_0f_mahou_common;
label_ZAKO_SPEKA_0f_mahou_common:	// [魔方陣 とりあえず共通部分]
	//	HATSUDAN_03_angle65536							= (int)(65536/(7)); 						/* 角度(一周を 7 分割) */	/* 1周をn分割した角度 */
		HATSUDAN_02_speed_offset						= (1);//t256(0.03); 						/* 減速定数 */
		goto COMMON_LEGACY_DIRECT;

label_ZAKO_SPEKA_12_mahou_random:	// [魔方陣 2:ランダムばらまき攻撃("魔方陣2")のカード]
		/* 速度は 1.0-3.0までの無段階乱数に変更 */
		HATSUDAN_01_speed256							+= ((ra_nd()&(512-1))); 	/* 弾速 */
		HATSUDAN_03_angle65536							= ((ra_nd()&(65536-1)));					/* 角度 */
		HATSUDAN_02_speed_offset						= (2);//10.24==t256(0.04);					/* 減速定数 */
		goto COMMON_LEGACY_DIRECT;
//------------------------- 固定弾
label_ZAKO_SPEKA_11_mahou_tate: 	// [魔方陣	1:縦攻撃のカード]
		HATSUDAN_03_angle65536							= (int)(65536/(4)); 		/* 右向き(上下弾)、偶数弾なので結果的に上下弾。 */
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_KOME_BASE + TAMA_IRO_01_AKA); 	/* 赤米弾 */
//		HATSUDAN_06_n_way								= (2);						/* [2way] */
//		HATSUDAN_07_div_angle65536						= (int)(65536/(2)); 		/* 角度(一周を 2 分割) */	/* 1周をn分割した角度 */
		goto COMMON_MULTI_VECTER;

label_ZAKO_SPEKA_0e_obake1: 		// [おばけ1 のカード]	/*(仕様上easyは無理)*/
		#if 0
		if (0==(cg.game_difficulty))	{	return; 	}	/*(仕様上easyは無理)*/
		#endif
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_KNIFE20_04_AOI);		/* [青ナイフ弾] */
		HATSUDAN_06_n_way								= ((cg.game_difficulty)+(cg.game_difficulty)-1);	/* [- 1 3 5way] */
//		HATSUDAN_07_div_angle65536						= (int)(65536/(18));			/* 分割角度 */
		goto COMMON_MULTI_VECTER;
//------------------------- 自機狙い
label_ZAKO_SPEKA_19_midori2:	// [緑毛玉2 のカード]
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_MINI8_BASE + TAMA_IRO_01_AKA);	/* [赤弾] */ /* 弾グラ */
		HATSUDAN_06_n_way								= (1+(cg.game_difficulty)); 	/* [1 2 3 4way] */
//		HATSUDAN_07_div_angle65536						= (65536/48);					/* 分割角度 */
//	//	HATSUDAN_07_div_angle65536						= (65536/(48*64));					/* 分割角度 */
		goto COMMON_JIKINERAI;

label_ZAKO_SPEKA_1a_meido3: 	// [ メイド3のカード]
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_UROKO14_BASE + TAMA_IRO_01_AKA);
		HATSUDAN_06_n_way								= ((cg.game_difficulty)<<1);	/*[0 2 4 6way]*/
//		HATSUDAN_07_div_angle65536						= (int)(65536/24);
		goto COMMON_JIKINERAI;

label_ZAKO_SPEKA_1b_ao2:	// [ 青妖精1/青妖精4のカード(B)]
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_UROKO14_BASE + TAMA_IRO_03_AOI);		/* [青弾] */
		HATSUDAN_06_n_way								= ((cg.game_difficulty));						/* [0 1 2 3way] */	/*1*/
		HATSUDAN_07_div_angle65536						-= ((cg.game_difficulty)<<6);	/* 分割角度 */
		goto COMMON_JIKINERAI;

label_ZAKO_SPEKA_1c_ao1:	// [ 青妖精1/青妖精4のカード(A)]
		HATSUDAN_05_bullet_obj_type 					= BULLET_KUNAI12_BASE + ((ra_nd())&7);			/* [弾グラ] */
//		HATSUDAN_06_n_way								= (8);											/* 8way弾 */
		HATSUDAN_07_div_angle65536						-= ((cg.game_difficulty)<<6);	/* 分割角度 */
		goto COMMON_JIKINERAI;

label_ZAKO_SPEKA_1d_karasu: 	// [ 青妖精3/烏1のカード]
		HATSUDAN_05_bullet_obj_type 					= (BULLET_KUNAI12_BASE) + ((ra_nd())&7);
//		HATSUDAN_06_n_way								= (8);
//		HATSUDAN_07_div_angle65536						= (int)(65536/(8));
		goto COMMON_JIKINERAI;

label_ZAKO_SPEKA_1e_kedama1:		// [ 毛玉1のカード]
		HATSUDAN_05_bullet_obj_type 					= BULLET_KOME_BASE + (src->cx256&0x07);
		HATSUDAN_06_n_way								= (3+(cg.game_difficulty)+(cg.game_difficulty));	/* [3 5 7 9way] */
//		HATSUDAN_07_div_angle65536						= (int)(65536/24);
		goto COMMON_JIKINERAI;
//------------------------- 連弾
label_ZAKO_SPEKA_1f_ao3_rendan: 	// [ 青妖精2のカード]
		{	/* ミニ弾の連弾 */
		//	#define KKKKK (0)
			unsigned int KKKKK;
			for (KKKKK=0; KKKKK<6/*4*/; KKKKK++)
			{
				static const u16 bunkatsu65536_tbl[4] =
				{((int)(65536/8)), ((int)(65536/12)), ((int)(65536/16)), ((int)(65536/24))};
		//	/*[r35要半分速]*/HATSUDAN_01_speed256		= (t256(2.5)+(KKKKK<<6));					/* 弾速 */
			/*[r35半分速]*/HATSUDAN_01_speed256 		= (t256(1.25)+(KKKKK<<5));					/* 弾速 */
				HATSUDAN_05_bullet_obj_type 			= ((BULLET_MINI8_BASE + TAMA_IRO_01_AKA)+(/*0*/6-KKKKK/*とりあえず*/)); /* [ 弾] */
				HATSUDAN_06_n_way						= (7);									/* [7way] */
				HATSUDAN_07_div_angle65536				= (bunkatsu65536_tbl[((cg.game_difficulty))]);		/* 分割角度 */
				/* 自機狙い弾を撃つ。 */
				calculate_jikinerai();
			//	bullet_regist_multi_vector_direct();
				HATSUDAN_02_speed_offset		= t256(3/*6*/);/*(テスト)*/
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
				hatudan_system_regist_n_way();/* (r33-) */
			}
		}
		goto EXIT_ZAKO_SPEKA;
//-------------------------

	COMMON_LEGACY_DIRECT:
		#if (1)/*(これを共通化するかは未定)*/
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_MINI8_BASE + TAMA_IRO_01_AKA);			/* 弾グラ */
		#endif
		bullet_regist_douchu_vector_tomari_dan(src);/*(雑魚ではここ以外無い)*/
		goto EXIT_ZAKO_SPEKA;

	COMMON_JIKINERAI:
		calculate_jikinerai();
		/*(not_break;)*/
	COMMON_MULTI_VECTER:
	//	HATSUDAN_02_speed_offset		= t256(3/*6*/);/*(テスト)*/
		HATSUDAN_02_speed_offset		= (2);/*(とりあえず)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		hatudan_system_regist_n_way();/* (r33-) */
		//	goto EXIT_ZAKO_SPEKA;
	EXIT_ZAKO_SPEKA:
			;
}


/*---------------------------------------------------------
	ザコ移動共通
---------------------------------------------------------*/
	#define target_x256 			user_data00
	#define target_y256 			user_data01
	#define radius256				user_data02
static void zako_move_type11_radius(OBJ *src)
{
	#if (0)//
	src->cx256 = (src->target_x256) + ((si n1024((src->tmp_angleCCW1024))*(src->radius256))>>8);/*fps_factor*/	/* CCWの場合 */
	src->cy256 = (src->target_y256) + ((co s1024((src->tmp_angleCCW1024))*(src->radius256))>>8);/*fps_factor*/
	#else
	{
		int sin_value_t256; 	//	sin_value_t256 = 0;
		int cos_value_t256; 	//	cos_value_t256 = 0;
		int256_sincos1024( (src->tmp_angleCCW1024), &sin_value_t256, &cos_value_t256);
		src->cx256 = (src->target_x256) + ((sin_value_t256*(src->radius256))>>8);/*fps_factor*/
		src->cy256 = (src->target_y256) + ((cos_value_t256*(src->radius256))>>8);/*fps_factor*/
	}
	#endif
}
	#define speed256				user_data04 	/* 速度 */
	#define kaisu_nnn				user_data05 	/* 繰り返した回数のカウンタ */

/*---------------------------------------------------------

---------------------------------------------------------*/
static void zako_move_vector(OBJ *src)
{
	#if (0)//
	src->cx256 += ((si n1024((src->tmp_angleCCW1024))*(src->speed256))>>8);/*fps_factor*/	/* CCWの場合 */
	src->cy256 += ((co s1024((src->tmp_angleCCW1024))*(src->speed256))>>8);/*fps_factor*/
	#else
	{
		int sin_value_t256; 	//	sin_value_t256 = 0;
		int cos_value_t256; 	//	cos_value_t256 = 0;
		int256_sincos1024( (src->tmp_angleCCW1024), &sin_value_t256, &cos_value_t256);
		src->cx256 += ((sin_value_t256*(src->speed256))>>8);/*fps_factor*/
		src->cy256 += ((cos_value_t256*(src->speed256))>>8);/*fps_factor*/
	}
	#endif
}

/*---------------------------------------------------------
	ザコアニメ共通
---------------------------------------------------------*/
/* 0000 0100 0000 0000 = 0x0400 = 1024 */
/* 0000 0011 1--- ---- */
static void zako_anime_type_99_rotate(OBJ *src)
{
	src->rotationCCW1024 += (src->zako_anime_rotate_angle1024);/*グラ回転*/
	mask1024(src->rotationCCW1024);
}
static void zako_anime_type01(OBJ *src)
{
	if (0==(src->zako_anime_rotate_angle1024))
	{
	//	src->obj_type_set				= ((src->obj_type_set)&(~0x07)) + ((((src->tmp_angleCCW1024)&(1024-1)))>>7);
		src->obj_type_set				= ((src->obj_type_set)&(~0x07)) + ((((src->tmp_angleCCW1024)&(0x0380)))>>7);
	}
	else
	{
		zako_anime_type_99_rotate(src);
	}
}
static void zako_anime_type02(OBJ *src)
{
	if (0==(src->zako_anime_rotate_angle1024))
	{
		src->obj_type_set				= ((src->obj_type_set)&(~0x03)) + ((src->jyumyou>>2)&(4-1));
	}
	else
	{
		zako_anime_type_99_rotate(src);
	}
}
static void zako_anime_type05(OBJ *src)
{
	if (0==(src->zako_anime_rotate_angle1024))
	{
		src->obj_type_set				= ((src->obj_type_set)&(~0x03)) + ((src->jyumyou>>4)&(4-1));
	}
	else
	{
		zako_anime_type_99_rotate(src);
	}
}
static void zako_anime_type03(OBJ *src)
{
	if (0==(src->zako_anime_rotate_angle1024))
	{
		src->obj_type_set				= ((src->obj_type_set)&(~0x01)) + ((src->jyumyou&0x10)>>4);
	}
	else
	{
		zako_anime_type_99_rotate(src);
	}
}
static void zako_anime_type04(OBJ *src)
{
	src->rotationCCW1024 += (src->zako_anime_rotate_angle1024);/*グラ回転*/
	mask1024(src->rotationCCW1024);
}


/*---------------------------------------------------------

---------------------------------------------------------*/
	#define MAHOU_TEKI_wait1		user_data04
	#define MAHOU_TEKI_wait2		user_data05
	#define start_y256				user_data06
#include "zako_000_mahoujin.h"			/* 魔方陣[A/B] */

	#define limit_y256				user_data05 	/* 仮想地面 */
#include "zako_002_inyou1.h"			/* 竜巻 陰陽玉 */

//	#define speed256				user_data04 	/* 速度 */
	#define vx_r					user_data05 	/* 回転方向(回転量) */
	#define px256					user_data06 	/* 仮想x位置(反転用) */
#include "zako_003_tatsumaki1.h"		/* 竜巻 陰陽玉 */

//	#define target_x256 			user_data00
//	#define target_y256 			user_data01
//	#define radius256				user_data02
	/* 敵の向き tmp_angleCCW1024 */
#include "zako_004_kakomi1.h"			/* 妖怪 */
#include "zako_005_aka_kedama.h"		/* 妖怪 */


//	#define speed256				user_data04
	#define max_y256				user_data05
#include "zako_007_yukari2.h"			/* 毛玉 */
	#define target_x256 			user_data00
	#define target_y256 			user_data01
#include "zako_008_midori_kedama1.h"	/* 毛玉 */
	#define wait1					user_data04 	/* 上下に動いたカウンタ */
//	#define kaisu_nnn				user_data05 	/* 繰り返した回数のカウンタ */
#include "zako_009_midori_kedama2.h"	/* 毛玉 */

	#define target_x256 			user_data00
	#define target_y256 			user_data01
//	#define speed256				user_data04
#include "zako_010_kedama1.h"			/* 毛玉 */
	#define USE_X_HOUKOU (1)
/*(灰毛玉)*/	#define kougeki_type			user_data05
#include "zako_011_kedama2.h"			/* 毛玉 */


	/* 敵の向き tmp_angleCCW1024 */
	#define gra_anime_type			user_data04 	/* グラタイプ */
#include "zako_012_obake1.h"			/* [C妖精]その他ザコ */
//	#define target_x256 			user_data00
//	#define target_y256 			user_data01
//	#define radius256				user_data02
	#define flag1					user_data05
#include "zako_013_obake2.h"			/* [C妖精]その他ザコ */
	#define USE_X_HOUKOU_karasu (0)
	#define KARASU3_houkou			user_data05
#include "zako_014_karasu1.h"			/* [C妖精]その他ザコ */


//	#define speed256				user_data04
#include "zako_015_meido1.h"			/* [B妖精]中妖精 */
//	#define speed256				user_data04
#include "zako_016_meido2.h"			/* [B妖精]中妖精 */
	#define clip_left256			user_data00
	#define clip_right256			user_data01
	#define turnspeed1024			user_data03
//	#define speed256				user_data04
//	#define kaisu_nnn				user_data05 	/* ターンする回数 */
#include "zako_017_meido3.h"			/* [B妖精]中妖精 */
//	#define speed256				user_data04
#include "zako_018_meido4.h"			/* [B妖精]中妖精 */

	/* 敵の向き tmp_angleCCW1024 */
//	#define speed256				user_data04 	/* 速度 */
#include "zako_019_ao_yousei1.h"		/* [A妖精]小妖精 */
//	#define kaisu_nnn				user_data05 	/* 弾を撃つ回数 */
#include "zako_020_ao_yousei2.h"		/* [A妖精]小妖精 */
	#define USE_X_HOUKOU_ao3 (0)
	#define KARASU3_houkou			user_data05
#include "zako_021_ao_yousei3.h"		/* [A妖精]小妖精 */
	/* 敵の向き tmp_angleCCW1024 */
//	#define speed256				user_data04 	/* 速度 */
#include "zako_022_ao_yousei4.h"		/* [A妖精]小妖精 */


/*---------------------------------------------------------
	倒した場合のアイテム指定
	中ボスと共用する必要がある。
---------------------------------------------------------*/

static void regist_items_common(GAME_COMMAND *l, OBJ *src)
{
	void (*aaa[(16)])(OBJ *src) =
	{
		item_create_00_item,			item_create_01_item,				item_create_02_item,			item_create_03_item,
		item_create_04_item,			item_create_05_item,				item_create_06_item,			item_create_07_item,
		item_create_random_table,		item_create_000_kougeki_mahoujin,	item_create_002_inyou_zako, 	item_create_random_table,
		item_create_random_table,		item_create_random_table,			item_create_14_yokai,			item_create_15_rumia,
	};
	src->callback_loser 	= aaa[(((l->user_item8))&0x0f)];			/* 「ザコがやられた場合の処理」に、「ランダムテーブルからアイテム生成」を設定 */
}
/*
src->jyumyou		 ---a bc--
src->yx_an im_frame  yyyy xxxx
src->yx_an im_frame  --1a --bc
*/

/*---------------------------------------------------------
	倒した場合のアイテム指定
	中ボス/ボスと共用する必要がある。
	-------------------------------------------------------
	(r36-)の仕様上、スコアとザコのあたり判定は同じ物。
	したがって DATで スコア 0 に設定しておくと、無敵ザコになる。
---------------------------------------------------------*/

/*global*/static void regist_settei_common(GAME_COMMAND *l, OBJ *src)
{
	regist_items_common(l, src);/* 中ボスと共用する必要がある。 */
	src->m_Hit256R			= ZAKO_ATARI16_PNG; 	/* グラからあたり判定を決める。 */
	//
	src->cx256				= ((l->user_x)<<8); 	/* 設定ファイルから出現x座標を決める。 */
	src->cy256				= ((l->user_y)<<8); 	/* 設定ファイルから出現y座標を決める。 */
	src->base_hp			= ((l->user_hp));		/* 設定ファイルから体力を決める。 */
	src->base_score 		= ((l->user_score));	/* 設定ファイルから獲得スコアを決める。(あたり判定共用) */
//
	src->obj_type_set		= (OBJ_Z02_TEKI)|((l->user_select_gazou)&0x3f); /* 設定ファイルから画像を決める。 */
	/*
		DATは人間が書くので、間違ってたりする。
		パターンアニメーション専用のザコキャラに、
		回転系グラを指定した場合、
		パターンアニメーション出来ないので、
		バグったキャラになってしまう。
		そこでそういう場合は、強制的に回転系に補正する。
	*/
	if (11<((l->user_select_gazou)&0x0f))/*(12, 13, 14, 15) */
	{
		src->zako_anime_rotate_angle1024 = (10);	/* 回転する速さ */
	}
}
//	(r36) src->base_score === src->atari_hantei 	= (1/*スコア兼用*/);	/* デフォルトはあたり判定あり */


/*---------------------------------------------------------
	ザコ登録の共通ルーチン
	-------------------------------------------------------
---------------------------------------------------------*/

typedef struct
{
	void (*aaa_init_callback)(GAME_COMMAND *l, OBJ *src);	/* 初期化 処理 */
	void (*aaa_move_callback)(OBJ *sss);					/* 移動 処理 */
} SPELL_aaa_RESOURCE;

global void game_command_07_regist_zako(GAME_COMMAND *l)
{
	if (0 != cg.chuu_boss_mode)
	{
		/* 中-ボスがいる場合には雑魚追加しない。 */
		return;/* error */
	}
	if (23 < l->user_255_code)
	{
		return;/* error */
	}
	/* 中間コード形式のコマンドから各関数に分岐する */
	/* tekiの生成を番号で管理(load_stage.c の ctype_name[]に対応している) */
	SPELL_aaa_RESOURCE aaa_resource[(23)] =
	{
	/* 魔方陣 */
		{regist_zako_000_mahoujin1, 		move_kougeki_mahoujin}, 	/* "魔方陣1"	TEKI_RED_BOOK	 MAHOUJIN A */	/* ばらまき攻撃魔方陣 */
		{regist_zako_001_mahoujin2, 		move_kougeki_mahoujin}, 	/* "魔方陣2"	TEKI_MAHOJIN	 MAHOUJIN B */	/* 消える魔方陣 */
	/* 竜巻 陰陽玉 */
		{regist_zako_002_inyou1,			move_inyou1},				/* "陰陽玉1"	TEKI_INYOU_DAI */
		{regist_zako_003_tatsumaki1,		move_tatsumaki1},			/* "竜巻1"		TEKI_TATSUMAKI */
	/* 妖怪 */
		{regist_zako_004_kakomi1,			move_kakomi1},				/* "囲妖怪1"	TEKI_CHOU1 */
		{regist_zako_005_aka_kedama1,		move_aka_kedama1_1st},		/* "赤毛玉1"	TEKI_CHEN */	/* 橙 */
	/* 毛玉 */
		{regist_zako_006_inseki1,			move_obake1},				/* "隕石1"		TEKI_NIJI_HOSI */	/* その他ザコ */
		{regist_zako_007_yukari2,			move_yukari2},				/* "紫編隊2"	TEKI_NIJI_HOSI */	/* その他ザコ */
		{regist_zako_008_midori_kedama1,	move_midori_kedama1},		/* "緑毛玉1"	TEKI_AKA_KEDAMA */
		{regist_zako_009_midori_kedama2,	move_midori_kedama2},		/* "緑毛玉2"	TEKI_AKA_KEDAMA */
		{regist_zako_010_kedama1,			move_kedama1},				/* "毛玉1"		TEKI_HAI_KEDAMA */
		{regist_zako_011_kedama2,			move_kedama2},				/* "毛玉2"		TEKI_HAI_KEDAMA */
	/* [C妖精]その他ザコ */
		{regist_zako_012_obake1,			move_obake1},				/* "おばけ1"	TEKI_YOUSEI2_5 */
		{regist_zako_013_obake2,			move_obake2},				/* "おばけ2"	TEKI_YOUSEI2_5	"虹毛玉1" */
		{regist_zako_014_karasu1,			move_karasu1},				/* "烏1"		TEKI_YOUSEI2_1 */
	/* [B妖精]中妖精 */
		{regist_zako_015_meido1,			move_meido1},				/* "メイド1"	TEKI_YOUSEI3_1 */
		{regist_zako_016_meido2,			move_meido2},				/* "メイド2"	TEKI_YOUSEI3_1 */
		{regist_zako_017_meido3,			move_meido3},				/* "メイド3"	TEKI_YOUSEI3_1 */
		{regist_zako_018_meido4,			move_meido4},				/* "メイド4"	TEKI_YOUSEI3_1 */
	/* [A妖精]小妖精 */
		{regist_zako_019_ao_yousei1,		move_ao_yousei1},			/* "青妖精1"	TEKI_YOUSEI1_1 */
		{regist_zako_020_ao_yousei2,		move_ao_yousei2},			/* "青妖精2"	TEKI_YOUSEI1_1 */
		{regist_zako_021_ao_yousei3,		move_ao_yousei3},			/* "青妖精3"	TEKI_YOUSEI1_1 */
		{regist_zako_022_ao_yousei4,		move_ao_yousei4},			/* "青妖精4"	TEKI_YOUSEI1_1 */ //(r33では未使用)
	};
	OBJ *h;
	h = obj_add_A01_teki_error();/*(ザコ敵のobjを新規作成する。)*/
	if (NULL != h)/* 登録できた場合のみ(登録に失敗した場合は、画面内のザコ敵数が多すぎるのでこれ以上登録できない。) */
	{
		// 標準共通登録処理。
		regist_settei_common(l, h);/* 中ボスと共用する必要がある。 */
		h->callback_hit_teki	= callback_hit_zako;		/* 「ザコに自弾があたった場合の処理」に、初期値として標準の処理を設定しておく。 */
		//
		h->callback_mover = aaa_resource[ (int)(l->user_255_code) ].aaa_move_callback;	/* 中間コード形式のコマンド番号から移動処理を決める。 */
		(aaa_resource[ (int)(l->user_255_code) ].aaa_init_callback)(l, h);				/* 中間コード形式のコマンド番号から各関数に分岐する。 */
	}
}
#include "chuu_boss_111_douchu.h"/*(道中の中ボス)*/
#include "boss_000_douchu.h"/*(道中のボス)*/
