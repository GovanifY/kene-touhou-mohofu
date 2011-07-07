
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _TAMA_SYSTEM_H_
#define _TAMA_SYSTEM_H_

/*---------------------------------------------------------
	弾システム
---------------------------------------------------------*/

/*---------------------------------------------------------
	弾幕の受け渡し設定(角度弾の場合)
	-------------------------------------------------------
	角度弾の分解能は65536度に決定。
---------------------------------------------------------*/

typedef struct _bullet_regist_
{
	// 以下は、 弾の設定。
	int BULLET_REGIST_00_speed256;					/* 速度 */
	int BULLET_REGIST_01_speed_offset;				/* 加速度 */

	int BULLET_REGIST_02_angle65536;				/* 角度 */					/* <分解能65536度に決定> */
	int BULLET_REGIST_03_tama_data; 				/* 加角度(交差弾用) */

	int BULLET_REGIST_04_bullet_obj_type;			/* 弾グラフィックの種類 */
	int BULLET_REGIST_05_regist_type;				/* 登録方式(動きの種類) */	/* 仕様変更(r33) */
	// 以下は、 n_way弾の場合に必要な要素。
	int BULLET_REGIST_06_n_way; 					/* n way 弾の弾数 */
	int BULLET_REGIST_07_div_angle65536;			/* n way 弾の分割角度 */	/* <分解能65536度に決定> */
	// 以下は、 弾幕リストに登録する場合の設定。
//	int BULLET_REGIST_start_number; 				/* 開始番号 */
//	int BULLET_REGIST_max_size; 					/* サイズ */
} BULLET_REGIST;
extern BULLET_REGIST br;/* 旧システムから引き継ぐ */


/*---------------------------------------------------------
	弾幕の受け渡し設定(ベクトル弾の場合)
	-------------------------------------------------------
	☆ 現在(r33)ベクトル弾の分解能は1024度なので、注意。
	☆ 1024度だと精度不足なので65536度に変更するかもしれないが
	速度的な都合で現在(r33)未定。
	☆ 現在(r33)速度的な都合で1024度になっているが、
	速度的な都合が解消されれば65536度に変更したい。
---------------------------------------------------------*/

#define BULLET_REGIST_00_VECTOR_hosi_gata_time_out			BULLET_REGIST_00_speed256
#define BULLET_REGIST_02_VECTOR_hosi_gata_angle1024 		BULLET_REGIST_02_angle65536
#define BULLET_REGIST_07_VECTOR_hosi_gata_add_speed256		BULLET_REGIST_07_div_angle65536

#define BULLET_REGIST_06_VECTOR_jyuryoku_dan_bound_counts	BULLET_REGIST_06_n_way
#define BULLET_REGIST_07_VECTOR_jyuryoku_dan_delta256		BULLET_REGIST_07_div_angle65536

#define BULLET_REGIST_02_VECTOR_angle1024					BULLET_REGIST_02_angle65536
#define BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024	BULLET_REGIST_06_n_way
#define BULLET_REGIST_07_VECTOR_tomari_dan_delta256 		BULLET_REGIST_07_div_angle65536

#define BULLET_REGIST_07_VECTOR_div_angle1024				BULLET_REGIST_07_div_angle65536


/*---------------------------------------------------------
	登録種類(角度弾の場合)
---------------------------------------------------------*/

extern void tama_system_regist_single(void);			/* 単発 */
extern void tama_system_regist_katayori_n_way(void);	/* 偏り n way弾 */
extern void tama_system_regist_n_way(void); 			/* 通常 n way弾 */

/*---------------------------------------------------------
	登録種類(ベクトル弾の場合)
---------------------------------------------------------*/
extern void bullet_regist_vector(void);

/*---------------------------------------------------------
	ベクトル弾の種類、廃止予定(旧互換)
---------------------------------------------------------*/

enum
{
	REGIST_TYPE_00_MULTI_VECTOR,	/* 多方向弾 */
	REGIST_TYPE_01_HAZUMI,			/* 重力弾01 輝夜、最終形態で投げてくるかなり無茶な弾。 */
	REGIST_TYPE_02_GRAVITY02,		/* 重力弾02 */
	REGIST_TYPE_03_TOMARI,			/* 止まり弾 */
	REGIST_TYPE_99_MAX,
};
// (r33)廃止済み:	REGIST_TY PE_04_KURU_KURU,		/* くるくる弾 */

/*---------------------------------------------------------
	角度弾規格(策定案、仮運用中。)
	基点座標関連は特殊機能に振って変更するかも知れない。
---------------------------------------------------------*/

	#define tama_system_radius256				user_data00 	/* 半径 */
	#define tama_system_speed65536				user_data01 	/* 加減速 */	//	#define tama_system_speed256				user_data02 	/* 加減速 */
	#define tama_system_tra65536				user_data02 	/* 加減速調整 */
	#define tama_system_tama_data				user_data03 	/* 画面外消去判定や反射機能 */
	#define tama_system_hatsudan_counter		user_data04 	/* 発弾カウンタ */
//	#define tama_system_aaa 					user_data05 	/* 拡張予定(?) (r33現在未使用) / tama_system_add_rot ate1024 加角度、回転角度調整(交差弾用) t256形式() */
//	#define tama_system_bbb 					user_data06 	/* 拡張予定(?) (r33現在未使用) / */
//	#define tama_system_ccc 					user_data07 	/* 拡張予定(?) (r33現在未使用) / */

#if 1/* 角度弾規格B(仮策定案) */
	#define TAMA_DATA_0000_TILT 				(0x0000)/* 傾き弾(通常弾) */
	#define TAMA_DATA_8000_NON_TILT 			(0x8000)/* 非傾き弾 */
	/* (仮) */
	#define TAMA_DATA_SAYUU_HANSYA_BIT			(0x0001)
	#define TAMA_DATA_RUMIA_SAYUU_BIT			(0x0001)
	#define TAMA_DATA_RUMIA_DIMMER_RENDAN_BIT	(0x0002)
#endif

#if 0
enum
{
	TAMA_TYPE_00_ANGLE_TILT = 0,	/* 通常弾 */
	TAMA_TYPE_01_ANGLE_NON_TILT,	/* 傾かない弾(大玉弾、チルノ弾、等用) */
//	TAMA_TYPE_02_,					/* 弾 */
//	TAMA_TYPE_03_,					/* 弾 */
	/* ... */
	TAMA_TYPE_99_MAX/* 最大値 */
};
#else
	#define dummy_r33(aaa) ;
#endif




#endif /* _TAMA_SYSTEM_H_ */

/*---------------------------------------------------------
	レーザーシステム
---------------------------------------------------------*/

#ifndef _LASER_SYSTEM_H_
#define _LASER_SYSTEM_H_

/*
	パチェ「通常攻撃」
	フラン禁忌「レーヴァテイン」
	フラン禁弾「過去を刻む時計」
	これらのスペカを作るかどうかは別問題として、これらのレーザー弾に
	システムで対応できるように設計しとく必要がある。
*/
enum
{
	LASER_TYPE_00_OFF		= 0,//	LASER_TYPE_00_1_PACHE	= 0,	// 0==off, lz[0]==offの場合、特殊機能で全部off。
	LASER_TYPE_01_4_PACHE,	/* 4本、外側タイプ(パチェ「通常攻撃」用) */
	LASER_TYPE_02_1_FRAN,	/* 1本、タイプ	  (フラン禁忌「レーヴァテイン」用) */
	LASER_TYPE_03_4_FRAN,	/* 4本、内側タイプ(フラン禁弾「過去を刻む時計」用) */
	MAX_99_LASER_TYPE
};
enum
{
	LASER_00_ = 0,
	LASER_01_,
	MAX_99_LASER
	/*	2弾？==禁忌「レーヴァテイン」(「始め」と「最後か真中あたり」のみ判定があれば、残像全部に判定がなくても大して変わらない気がする)
		8弾？==禁忌「レーヴァテイン」
		最後でなくて、真中あたりにするのは難易度を調整する為。(最後の方が若干難しい)
		物が剣なので、実際は「ぎりぎりで当たってる」のだけど、その場合あたってない事にした方が「ぎりぎりで当たらなかった」という奴。
		２本要る理由は、高速移動キャラで、1本目ですり抜けても、２本目に当たる。
	 */
};
typedef struct _laser_
{
	int laser_type; 		/* レーザーの種類 / 又はoff指定. LASER_TYPE */
	int laser_angle65536;	/* レーザーの角度 */
} LASER;
extern LASER lz[MAX_99_LASER];
/* lz[0]==0;の場合、すべてoff。(レーザーモード 0:off, 1:on) */

#endif /* _LASER_SYSTEM_H_ */
