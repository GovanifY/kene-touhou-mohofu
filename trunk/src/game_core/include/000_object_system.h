
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _OBJECT_SYSTEM_H_
#define _OBJECT_SYSTEM_H_

/*---------------------------------------------------------
	スプライトオブジェクト マネージャ
--------------------------------------------------------- */
/*
寿命:   s32 JYUMYOU
1[秒]                                   60           >                64 [frame]
1[分]                                   60 x 60      >           64 x 64 [frame]
1[時間] 0x034BC0 ==   216000            60 x 60 x 60 >      64 x 64 x 64 [frame] ==   262144 0x040000
2.5[日] 0xC5C100 == 12960000 2.5 x 24 x 60 x 60 x 60 > 64 x 64 x 64 x 64 [frame] == 16777216 0x01000000
-------------------------------------------------
もし、毎フレーム減算するカウンタがあるとして、
signed int だと、初期値が 0x01ff ffff だった場合、2.5[日]経過しても、カウンタの値は0x01-- ----。
つまり最上位bit は符号として、最上位bitを除いた7bitは自由に使える。(カウンタ取り出すとき0x00ffffffでマスクするとして)
*/

#define JYUMYOU_MUGEN	(16777216)
#define JYUMYOU_1MIN	byou60(60)
#define JYUMYOU_NASI	(0)
/*
JYUMYOU_MUGEN:	(寿命無限==寿命、約2.5[日])プレイヤーやオプション等、自動で消えては困るもの。
JYUMYOU_1MIN:	(寿命、約1[分])
JYUMYOU_NASI:	(寿命、なし。無しになると、自動で消去されるので注意。「毎フレーム寿命=2に設定しとけば消えない。(寿命=1だと消えちゃうかも)」)

*/

/* 弾の生成時間(標準) */
#define JYUMYOU_TAMA_HASSEI 	(JYUMYOU_1MIN)
/* ザコの生成時間(標準) */
#define JYUMYOU_ZAKO_HASSEI 	(JYUMYOU_1MIN)
/* 画面外ならザコ消す場合の、リミット時間 */
//#define JYUMYOU_ZAKO_CLIP 		(2048)				/* 2048/60 = 34.1333333333333333333333333333333[秒] */

/*
	生成時間を基点とした動きの場合、(後でシステムが変わるかもしれないから)
	上記(JYUMYOU_TAMA_HASSEI / JYUMYOU_ZAKO_HASSEI)を基準としておく。
*/


/*---------------------------------------------------------
	プライオリティー(優先順位)
	-------------------------------------------------------
	プライオリティーには、描画プライオリティー以外に、
	あたり判定プライオリティー(==あたり判定を領域別に判別する機能)があるので注意。
	-------------------------------------------------------
--------------------------------------------------------- */
/*
	ここは種類別ではなくて、機能別に分類してください。
*/
#define SP_GROUP_SHOT_ZAKO					(0x0000)/*0x0100*/
//		OBJ_Z00_MAHOU_JIN													/* === Gu魔方陣面 === */
#define OBJ_Z01_JIKI_GET_ITEM				(0x0100)/*0x0100*/				/* === Guプレイヤー面 === */
#define OBJ_Z02_TEKI						(0x0200)/*0x0400*/				/* === Gu敵面 === */
#define OBJ_Z03_ITEM						(0x0400)/*(0x2000)0x1000*/		/* === GuITEM面 === */
#define OBJ_Z04_TAMA						(0x0800)/*(0x1000)0x0800*/		/* === Gu敵弾面 === */
#define OBJ_Z05_FRONT						(0x1000)/*(0x8000)*/			/* === Guフロント面 === */	/* === Guパネル面 === */

#define SP_GROUP_ALL_SDL_CORE_TYPE			(0x1f00)/* game_core動作 */
#define SP_GROUP_SUB_TYPE_128				(0x007f)


/*---------------------------------------------------------
	オブジェバンクシステム
--------------------------------------------------------- */

/* 管理するオブジェクトバンク */
enum
{
	OBJ_BANK_00_TAMA = 0,		// 敵弾用バンク 		//	※1.敵弾/アイテム用テクスチャは共用
	OBJ_BANK_01_ITEM,			// アイテム用バンク 	//	※1.敵弾/アイテム用テクスチャは共用
	OBJ_BANK_02_FRONT_BANK0,	// フロント面用バンク	//	※2.フロント/パネル用テクスチャは共用
//	OBJ_BANK_07_FRONT_BANK1,	// 用バンク
//	OBJ_BANK_07_FRONT_BANK2,	// 用バンク
	OBJ_BANK_03_TITLE_dummy,	/*[予定]*/	// タイトル画面用バンク //	※2.フロント/パネル用テクスチャは共用
//	OBJ_BANK_09_EFFECT, 		/*[予定]*/	// 用バンク
//
	OBJ_BANK_01_REIMU_A,		// 自機用バンク(霊夢A、霊符)
	OBJ_BANK_02_REIMU_B,		// 自機用バンク(霊夢B、夢符)
	OBJ_BANK_03_MARISA_A,		// 自機用バンク(魔理沙_A、魔符)
	OBJ_BANK_04_MARISA_B,		// 自機用バンク(魔理沙_B、恋符)
	OBJ_BANK_05_REMILIA,		// 自機用バンク(レミリア)
	OBJ_BANK_06_YUYUKO, 		// 自機用バンク(幽々子)
	OBJ_BANK_07_CIRNO_A,		// 自機用バンク(チルノA、氷符)
	OBJ_BANK_08_CIRNO_Q,		// 自機用バンク(チルノ⑨、⑨符)
//
	OBJ_BANK_21_BOSS_STAGE1,	// ボス用バンク(ステージ 1、アリス)
	OBJ_BANK_22_BOSS_STAGE2,	// ボス用バンク(ステージ 2、未定)
	OBJ_BANK_23_BOSS_STAGE3,	// ボス用バンク(ステージ 3、輝夜)
	OBJ_BANK_24_BOSS_STAGE4,	// ボス用バンク(ステージ 4、文)
	OBJ_BANK_25_BOSS_STAGE5,	// ボス用バンク(ステージ 5、パチュリー)
	OBJ_BANK_26_BOSS_STAGE6,	// ボス用バンク(ステージ 6、咲夜)
	OBJ_BANK_11_ZAKO_STAGE1,	// ザコ用バンク(ステージ 1-6, extra, phantasm)		// OBJ_BANK_27_BOSS_STAGE7_dummy,
	OBJ_BANK_28_BOSS_STAGE8,	// ボス用バンク(ステージ 8、エキストラ)
	OBJ_BANK_29_BOSS_STAGE9,	// ボス用バンク(ステージ 9、ファンタズム)
//
//	OBJ_BANK_11_ZAKO_STAGE1,
//	OBJ_BANK_12_ZAKO_STAGE2,
//	OBJ_BANK_13_ZAKO_STAGE3,
//	OBJ_BANK_14_ZAKO_STAGE4,
//	OBJ_BANK_15_ZAKO_STAGE5,
//	OBJ_BANK_16_ZAKO_STAGE6,
//	OBJ_BANK_17_ZAKO_STAGE7_dummy,
//	OBJ_BANK_18_ZAKO_STAGE8,
//
	OBJ_BANK_MAX
};
#define 	OBJ_BANK_SIZE			(8*8)
#define 	OBJ_BANK_SIZE_00_TAMA	(OBJ_BANK_00_TAMA*OBJ_BANK_SIZE)
#define 	OBJ_BANK_SIZE_01_ITEM	(OBJ_BANK_01_ITEM*OBJ_BANK_SIZE)

/*---------------------------------------------------------
	スプライト
--------------------------------------------------------- */

typedef struct /*_point256*/
{
	u16 x;	/*	union POINT */	/* x 位置 */
	u16 y;	/*	union POINT */	/* y 位置 */
} POINT_u16;
/* u16座標 */

typedef struct /*_point256*/
{
	int x256;	/*	union POINT */	/* x表示位置  (256固定小数点形式) / akt. Position */
	int y256;	/*	union POINT */	/* y表示位置  (256固定小数点形式) / akt. Position */
} POINT256;
/* t256座標 */

typedef struct /*_point_vector256*/
{
	int x256;	/*	union POINT */	/* x表示位置  (256固定小数点形式) / akt. Position */
	int y256;	/*	union POINT */	/* y表示位置  (256固定小数点形式) / akt. Position */
	int vx256;	/*	union POINT_VECTOR */	/* 共用regist_vector(); */	/*union BULLET_VECTOR_DATA*/
	int vy256;	/*	union POINT_VECTOR */	/* 共用regist_vector(); */	/*union BULLET_VECTOR_DATA*/
} POINT_VECTOR256;
/* t256座標 + t256ベクトル */

/*
	強制傾きoffの特殊機能:
	m_zoom_y256 に M_ZOOM_Y256_NO_TILT を設定しておくと、
	rotationCCW1024(描画用角度) を無視して、回転無し、強制1.0倍で描画する。
*/
#define M_ZOOM_Y256_NO_TILT (0)
/*
	アリスの人形が軌跡は回転弾なのにグラは回転していなかったり、
	パチェの炎のアニメーション(火符「アグニシャイン」)や
	チルノやルーミアの丸弾や大玉弾が回転していなかったり、その他色々使用している。
	本家も描画用設定角度をシンプルに無視する何らかの機能があるのでは?と思い実装してみた。
	基本的に丸弾は回転させると無駄に遅くなる。
*/


#if 1
// union 共用
	#define base_hp 					tairyoku_ti
	#define base_time_out				tairyoku_ti/*bakuhatsu*/
//	#define base_score					user01
	#define base_score					kougeki_ti
	#define atari_hantei				kougeki_ti
	#define ATARI_HANTEI_OFF			(0)
	#define ATARI_HANTEI_TAOSENAI		(0x3fffffff)
	//#define base_state				user01
#endif
//
/* スプライトの使い方

分類            自機           自弾        ボス         中ボス    ザコ       敵                  敵弾            爆発
tairyoku_ti     --             --          [特殊]     [zako      zako]      base_hp             base_time_out   base_time_out
                --             --           体力        体力      体力        体力                時間            時間
----------------------------------------------------------------------------------------------
kougeki_ti     (1)            kougeki_ti                                   base_score             --              --
                無敵時強さ     武器強さ    スコア      スコア     スコア      スコア              --              --
------------
攻撃値とスコアは同じ物なので、プレーヤーがボスに体当たりされて受けるダメージはボスのスコアと同じ量。
スコア0ならばプレーヤーは触ってもダメージを受けないので、プレーヤーは死なない。
*/

#define tmp_angleCCW65536 tmp_angleCCW1024
typedef struct _obj_tag_
{
	int cx256;						/*	union POINT */			/* center x 表示 中心座標位置  (256固定小数点形式) / akt. Position */
	int cy256;						/*	union POINT */			/* center y 表示 中心座標位置  (256固定小数点形式) / akt. Position */
	int vx256;						/*	union POINT_VECTOR */	/* 共用regist_vector(); */	/*union BULLET_VECTOR_DATA*/
	int vy256;						/*	union POINT_VECTOR */	/* 共用regist_vector(); */	/*union BULLET_VECTOR_DATA*/
//[4==1*4]
	int rotationCCW1024;			/* 描画用角度(下が0度で左回り(反時計回り), 一周は1024分割, 0-1023度) */
	int tmp_angleCCW1024;			/* 保持用角度(下が0度で左回り(反時計回り), 一周は1024分割, 0-1023度) */
	s32 jyumyou;					/* 寿命 */
	int obj_type_set;				/* [デバッグ中/検索出来るように変な名前にしとく] 種類及び使用可否 / Sprite-Type, ( enum OBJ_TYPE), 0 = remove. */
//[8==2*4]
	u32 color32;					/* Gu color AGBR8888 MAKE32RGBA(RED, GREEN, BLUE, ALPHA)で各要素 0 から 255 (0x00 から 0xff)まで。 */
	int m_Hit256R;					/* あたり判定用 */
	int m_zoom_x256;				/* 表示拡大率x (画像の横の拡大率t256()形式で指定) */
	int m_zoom_y256;				/* 表示拡大率y (画像の縦の拡大率t256()形式で指定) */
//[12==3*4]
	int kougeki_ti; 				/* 攻撃値 / score / あたり判定 */		/* 敵スコア */
	int tairyoku_ti;				/* user00 ENEMY_BASE base; */			//int health;	int time_out;		/* user03 */
	/* とりあえず移行用(2)追加弾、基本機能 */
	int user_data12;
	int for_align_dummy_22;/* .align 合わせ用。.align合ってた方が実行速度は速いし、プログラムも簡略化出来るので常に小さい。使ってないダミー。 */
//[16==4*4]
	/* とりあえず移行用(1)従来互換(弾) */
	int user_data00;
	int user_data01;
	int user_data02;/*user_data02==(r35-弾幕システム用途でレイヤー管理しているので重要)*/
	int user_data03;
//[20==5*4]
	int user_data04;
	int user_data05;
	int user_data06;
	int user_data07;
//[24==6*4]
	void (*callback_move01)(struct _obj_tag_ *src); 			/* Custom-Move-Routine */
	void (*callback_move02_r35_dummy)(struct _obj_tag_ *src);	/* Custom-Tamakesi-Routine / Zako Anime */
	void (*callback_move03)(struct _obj_tag_ *c, struct _obj_tag_ *src);
	void (*callback_move04)(struct _obj_tag_ *src); 			/* Custom-loser-Routine (やられた後にボーナスを出すとか、違う爆発するとか) / 自機オプションのアニメーションコールバック */
//[28==7*4]
// 描画用
	struct _obj_tag_ *target_obj;		/* とりあえず移行用(3)移行したらなくなる。 */
	/*u16*/u16/*int*/ tx;	//	u16 u0;//tx = u0;		int tx; 	/* u テクスチャ座標 */
	/*u16*/u16/*int*/ ty;	//	u16 v0;//ty = v0;		int ty; 	/* v テクスチャ座標 */
	/*u16*/u16/*int*/ w;	//	u16 u1;//w = (u1-u0);	int w;
	/*u16*/u16/*int*/ h;	//	u16 v1;//h = (v1-v0);	int h;
	int for_align_dummy_11;/* .align 合わせ用。.align合ってた方が実行速度は速いし、プログラムも簡略化出来るので常に小さい。使ってないダミー。 */
//[32==8*4]
} OBJ;


//	int user01; 					/* user01 ENEMY_BASE base; */	//int score;	int state;			/* user02 */
/*
kougeki_ti: 	攻撃値:
プレイヤーobj:			無敵期間に雑魚に体当たりする場合の攻撃値。(1)
プレイヤーショット: 	ショットの強さ。
雑魚obj:				雑魚がプレイヤーに体当たりする場合の攻撃値。及び倒した場合に得られるスコア。
ボスobj:				雑魚がプレイヤーに体当たりする場合の攻撃値。及び倒した場合に得られるスコア。
演出obj:				都合上, 常に(0)を保持。(???)

*/

#if (1)
/*
	-------------------------------------------------------
	ゲームでは何かアクションが起こった場合、ＯＢＪ(キャラ)によって対処の方法が異なる。
	例えばＯＢＪ(キャラ)がぶつかった場合。
	自機と弾がぶつかった場合、自機が死ぬ(ダメージ受ける)。
	自機とアイテムがぶつかった場合、アイテムが死ぬ(アイテム取る)。
	これらの関係は、ぶつかる処までは同じ。で、ぶつかってからの処理が違う。
	-------------------------------------------------------
	処理が違う部分は、コールバックという方法で吸収する。
	コールバックに予め処理を登録しておくと、ぶつかった場合予め登録しておいた、
	コールバックルーチン(コールバックされる処理)が呼ばれる。
	-------------------------------------------------------
	コールバックの種類として、ぶつかった場合以外にも、
	移動する場合。やられた場合。もコールバックになっている。
	-------------------------------------------------------
	コールバックは便利だけど、原理的に遅い。だから珠にしか使用しない処理には、良い方法だが、
	煩雑に呼ばれる処理には使うと遅くなる。
	-------------------------------------------------------
*/
/* callback_move01: 移動する場合の処理。 */
	#define callback_mover			callback_move01

/* callback_move02: r35現在使ってないダミー。 */
//	#define callback_boss_tamakesi	callback_move02/* 共用 */
//	#define callback_zako_anime 	callback_move02/* 共用 */

/* callback_move03: ぶつかった場合の処理。 */
	#define callback_hit_teki		callback_move03

/* callback_move04: やられた場合の処理。 */
	#define callback_loser			callback_move04
#endif


/*	struct _sprite *target_obj;
	用途は(
		player用誘導弾、
		後ろの魔方陣"妖怪2"(赤)
	)
	ほんとは複雑にキャストすれば要らないんだけどバグでそうだし。
 */



/*---------------------------------------------------------

---------------------------------------------------------*/

extern OBJ *obj_add_A00_tama_error(void);	/* [A00弾領域]にobjを登録する。(==弾専用) */
extern OBJ *obj_add_A01_teki_error(void);	/* [A01敵領域]にobjを登録する。(==敵専用) */
/* 任意の領域に強制的に登録する。[A02固定領域]と[A03パネル領域]はこの方法で登録する。 */
/* 内部管理値のオフセットを素直に指定する。例えば専用固定スプライトの場合は固定値までのオフセットOBJ_HEAD_02_0x0900_KOTEIを足す。 */
extern OBJ *obj_add_Ann_direct(unsigned int register_object_absolute_direct_number);	/* 強制登録用。危険なので注意して使う。*/

/* [A00弾領域]と[A01敵領域]と[A02固定領域]と[A03パネル領域]のOBJを全消去。 */
extern void obj_cleanup_all(void);

/* [A00弾領域]と[A01敵領域]と[A02固定領域]のOBJを移動処理する。[A03パネル領域]のobjは移動しない。 */
extern void obj_area_move_A00_A01_A02(void);

/* [A00弾領域]のobjとのあたり判定チェック。 */
extern OBJ *obj_collision_check_00_tama(OBJ *tocheck, int type);/*弾専用、typeには弾かアイテムかを指定する。*/
/* [A01敵領域]のobjとのあたり判定チェック。 */
extern OBJ *obj_collision_check_01_teki(OBJ *tocheck);/*敵専用*/	//, int type);

extern void gamen_gai_nara_zako_osimai(OBJ *src);/* 画面外ならおしまい */
extern void check_tukaima_time_out(OBJ *src);/* */

/* 使用中であるが退避したい場合。画面外にスプライトを移動させ、無効にする。 */
extern void sprite_kotei_obj_r36_taihi(OBJ *src);

#endif/* _OBJECT_SYSTEM_H_ */




#ifndef _OBJ_AREA_H_
#define _OBJ_AREA_H_

#if 1
/*---------------------------------------------------------
	敵弾管理システム
---------------------------------------------------------*/

	/* 描画プライオリティーが自機テクスチャーのもの */
/* -- プライオリティー＃６．自機を描画 */
//#define OBJ_111JIKI_POOL_MAX FIX_OBJ_07_JIKI_OPTION3

/*
 参考: kouma_tr013/東方紅魔郷/マニュアル/html/faq.html
Ｑ２３　アイテムの表示限界は？

　画面内５１２個までです。
　敵弾の数が６４０個なので、全てがボーナスアイテムになるとは限りませんが、
　殆どの場合はスコアパターンに影響するようなことはありません。

*/

enum/* [A00]エリア#00: [A00弾領域] */
{
/* 弾の最大数は OBJ_POOL_00_TAMA_1024_MAX (==1024) それ以上登録しようとしても登録されない。 */
//	OBJ_POOL_00_TAMA_1024_MAX = (1<<9),/*(旧仕様512のメモ)*/
	OBJ_POOL_00_TAMA_1024_MAX = (1<<10),/* 弾登録プログラムの都合上 2^n である必要があります。(1024==(1<<10) ) */
//	OBJ_POOL_00_TAMA_1024_MAX = (1<<11),/* 弾登録プログラムの都合上 2^n である必要があります。(2048==(1<<11) ) */
};


enum/* [A01]エリア#01: [A01敵領域] */
{
	TEKI_OBJ_00_BOSS_HONTAI = 0,/*<使用中>[r36]*/
/* 敵(自機、オプション、自弾等、弾以外の全てのスプライト含む)の最大数は OBJ_POOL_01_TEKI_0256_MAX (==256) それ以上登録しようとしても登録されない。 */
	OBJ_POOL_01_TEKI_0256_MAX = (1<<8),/* 敵登録プログラムの都合上 2^n である必要があります。(256==(1<<8) ) */
};


enum/* [A02]エリア#02: [A02固定領域] */
{
	/* 描画プライオリティーが自機テクスチャーのもの */
	FIX_OBJ_00_PLAYER = 0,		/*<使用中>[r32変更済み] obj_add_Ann_direct(OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER); */
	FIX_OBJ_01_JIKI_MARU,		/*<使用中>[r32変更済み] obj_add_Ann_direct(OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_01_JIKI_MARU); */
	FIX_OBJ_02_BOMBER_HONTAI,	/*<使用中>[r36] ボンバーの親 */
	FIX_O_BJ_03_SEND2,			/*[予定]*/
	FIX_OBJ_04_JIKI_OPTION0,	/*<使用中>[r32変更済み] obj_add_Ann_direct(OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_04_JIKI_OPTION0+jj); */
	FIX_OBJ_05_JIKI_OPTION1,	/*<使用中>[r32変更済み]*/
	FIX_OBJ_06_JIKI_OPTION2,	/*<使用中>[r32変更済み]*/
	FIX_OBJ_07_JIKI_OPTION3,	/*<使用中>[r32変更済み]*/
//
	/* 描画プライオリティーがfrontテクスチャーのもの */
//	xxx_FIX_OBJ_08_BOSS,		/* <あたり判定の都合上出来ない> */	/*<使用中>[r32変更済み] obj_add_Ann_direct(OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_08_BOSS); */
//	FIX_O_BJ_09_,				/*[予定]*/
//	FIX_O_BJ_10_,				/*[予定]*/
//	FIX_OBJ_11_EFFECT01,				/*<使用中>*/
//	FIX_O_BJ_12_SUPPORT0,		/*[予定]*/		/*<使用中>*/
//	FIX_O_BJ_13_SUPPORT1,		/*[予定]*/		/*<使用中>*/
//	FIX_O_BJ_14_SUPPORT2,		/*[予定]*/		/*<使用中>*/
//	FIX_O_BJ_15_SUPPORT3,		/*[予定]*/		/*<使用中>*/
	FIX_OBJ_08_EFFECT01,		/*[予定]*/		/*<使用中>*/
	FIX_OBJ_09_EFFECT02,		/*[予定]*/		/*<使用中>*/
	FIX_OBJ_10_EFFECT03,		/*[予定]*/		/*<使用中>*/
	FIX_OBJ_11_EFFECT04,		/*[予定]*/		/*<使用中>*/
	FIX_OBJ_12_EFFECT05,		/*[予定]*/		/*<使用中>*/
	FIX_OBJ_13, 	/*[予定]*/
	FIX_OBJ_14, 	/*[予定]*/
	FIX_OBJ_15_JIKI_TEISOKU_EFFECT, 	/*[予定]*/				/*<使用中>*/
	OBJ_POOL_02_KOTEI_0016_MAX/*MAX*/,/* 専用自動登録プログラムが無いので 2^n に限定する必要性はない。 */
};



enum/* [A03]エリア#03: [A03パネル領域] */
{
	PANEL_OBJ_00_S00 = 0,	/* スコア10桁目 */
	PANEL_OBJ_01_S01,		/* スコア9桁目 */
	PANEL_OBJ_02_S02,		/* スコア8桁目 */
	PANEL_OBJ_03_S03,		/* スコア7桁目 */
	PANEL_OBJ_04_S04,		/* スコア6桁目 */
	PANEL_OBJ_05_S05,		/* スコア5桁目 */
	PANEL_OBJ_06_S06,		/* スコア4桁目 */
	PANEL_OBJ_07_S07,		/* スコア3桁目 */
	PANEL_OBJ_08_S08,		/* スコア2桁目 */
	PANEL_OBJ_09_S09,		/* スコア1桁目 */
	PANEL_OBJ_0a_P00,		/* パワー1桁目 */
	PANEL_OBJ_0b_P01,		/* パワー小数点1桁目 */
	PANEL_OBJ_0c_P02,		/* パワー小数点2桁目 */
	PANEL_OBJ_0d_Time,		/* "Time:" */
	PANEL_OBJ_0e_P0p,		/* パワー'.' */
	PANEL_OBJ_0f_NAN_IDO,	/* 難易度表示 "Easy", "Normal", "Hard", "Lunatic", */
	//
	PANEL_OBJ_10_H00,		/* ハイスコア10桁目 */
	PANEL_OBJ_11_H01,		/* ハイスコア9桁目 */
	PANEL_OBJ_12_H02,		/* ハイスコア8桁目 */
	PANEL_OBJ_13_H03,		/* ハイスコア7桁目 */
	PANEL_OBJ_14_H04,		/* ハイスコア6桁目 */
	PANEL_OBJ_15_H05,		/* ハイスコア5桁目 */
	PANEL_OBJ_16_H06,		/* ハイスコア4桁目 */
	PANEL_OBJ_17_H07,		/* ハイスコア3桁目 */
	PANEL_OBJ_18_H08,		/* ハイスコア2桁目 */
	PANEL_OBJ_19_H09,		/* ハイスコア1桁目 */
	PANEL_OBJ_1a_fps00, 	/* fps2桁目 */
	PANEL_OBJ_1b_fps01, 	/* fps1桁目 */
	PANEL_OBJ_1c_fps02, 	/* fps小数点1桁目 */
	PANEL_OBJ_1d_fps03, 	/* fps小数点2桁目 */
	PANEL_OBJ_1e_fps0p, 	/* fps '.' */
	PANEL_OBJ_1f_fps_str,	/* "fps" */
	//
	PANEL_OBJ_20_N00,		/* 残りガッツ8桁目 */
	PANEL_OBJ_21_N01,		/* 残りガッツ7桁目 */
	PANEL_OBJ_22_N02,		/* 残りガッツ6桁目 */
	PANEL_OBJ_23_N03,		/* 残りガッツ5桁目 */
	PANEL_OBJ_24_N04,		/* 残りガッツ4桁目 */
	PANEL_OBJ_25_N05,		/* 残りガッツ3桁目 */
	PANEL_OBJ_26_N06,		/* 残りガッツ2桁目 */
	PANEL_OBJ_27_N07,		/* 残りガッツ1桁目 */
	PANEL_OBJ_28_B00,		/* ボム8桁目 */
	PANEL_OBJ_29_B01,		/* ボム7桁目 */
	PANEL_OBJ_2a_B02,		/* ボム6桁目 */
	PANEL_OBJ_2b_B03,		/* ボム5桁目 */
	PANEL_OBJ_2c_B04,		/* ボム4桁目 */
	PANEL_OBJ_2d_B05,		/* ボム3桁目 */
	PANEL_OBJ_2e_B06,		/* ボム2桁目 */
	PANEL_OBJ_2f_B07,		/* ボム1桁目 */
	//
	PANEL_OBJ_30_J00,		/* 残り時間3桁目 */
	PANEL_OBJ_31_J01,		/* 残り時間2桁目 */
	PANEL_OBJ_32_J02,		/* 残り時間1桁目 */
	PANEL_OBJ_33_G03,		/* グレイズ5桁目 */
	PANEL_OBJ_34_G04,		/* グレイズ4桁目 */
	PANEL_OBJ_35_G05,		/* グレイズ3桁目 */
	PANEL_OBJ_36_G06,		/* グレイズ2桁目 */
	PANEL_OBJ_37_G07,		/* グレイズ1桁目 */
	//
	/* パネル用スプライトは OBJ_POOL_03_PANEL_0056_MAX (==256) それ以上登録しようとしても登録されない。 */
//	OBJ_POOL_03_PANEL_0056_MAX = (256),/*(旧仕様のメモ)*/
	OBJ_POOL_03_PANEL_0056_MAX/* MAX */,/* 専用自動登録プログラムが無いので 2^n に限定する必要性はない。 */
};

#if 0
[A00]エリア#00: [A00弾領域]
[A01]エリア#01: [A01敵領域]
[A02]エリア#02: [A02固定領域]
[A03]エリア#03: [A03パネル領域]
スプライトオブジェクト相対オフセットアドレス。(the sprites object struct, relative address.)
0x0000 +-------------------------------+ OBJ_HEAD_00_0x0000_TAMA
       | [A00]弾専用スプライト(2048)   | OBJ_POOL_00_TAMA_1024_MAX (need, 2^n)
0x0800 +-------------------------------+ OBJ_HEAD_01_0x0800_TEKI
       | [A01]敵専用スプライト(256)    | OBJ_POOL_01_TEKI_0256_MAX (need, 2^n)
0x0900 +-------------------------------+ OBJ_HEAD_02_0x0900_KOTEI
       | [A02]自機等固定スプライト(16) | OBJ_POOL_02_KOTEI_0016_MAX (not need, 2^n)
0x0910 +-------------------------------+ OBJ_HEAD_03_0x0910_PANEL
       | [A03]パネル用スプライト(56)   | OBJ_POOL_03_PANEL_0056_MAX (not need, 2^n)
0x0948 +-------------------------------+ OBJ_LAST_99_0x0948_MAX
全スプライトオブジェクト OBJ_LAST_99_0x0948_MAX
//
「弾専用スプライトオブジェクト」 及び 「敵専用スプライトオブジェクト」 は、
自動登録プログラムの都合上、必ず 2^n である必要があります。
//
「固定スプライトオブジェクト」 及び 「パネル用スプライトオブジェクト」 は、
アドレス固定なので、自動登録プログラムが無い為、2^nサイズである必要はありません。
#endif
	/* 全スプライト */
#define OBJ_HEAD_00_0x0000_TAMA 	(0) 													/* 弾専用スプライトの先頭 */
#define OBJ_HEAD_01_0x0800_TEKI 	(OBJ_HEAD_00_0x0000_TAMA +OBJ_POOL_00_TAMA_1024_MAX)	/* 敵専用スプライトの先頭 */
#define OBJ_HEAD_02_0x0900_KOTEI	(OBJ_HEAD_01_0x0800_TEKI +OBJ_POOL_01_TEKI_0256_MAX)	/* 自機等固定スプライトの先頭 */
#define OBJ_HEAD_03_0x0910_PANEL	(OBJ_HEAD_02_0x0900_KOTEI+OBJ_POOL_02_KOTEI_0016_MAX)	/* パネル用スプライトの先頭 */
#define OBJ_LAST_99_0x0948_MAX		(OBJ_HEAD_03_0x0910_PANEL+OBJ_POOL_03_PANEL_0056_MAX)	/* 全スプライトの数 */
extern OBJ obj99[OBJ_LAST_99_0x0948_MAX];	/* 全スプライト */

enum/*(OBJの種類)*/
{
	OBJECT_TYPE_00_TAMA = 0,/* 弾専用スプライト */
	OBJECT_TYPE_01_TEKI,/* 敵専用スプライト */
	OBJECT_TYPE_02_KOTEI,/* 自機等固定スプライト */
	OBJECT_TYPE_03_PANEL,/* パネル用スプライト */
	OBJECT_TYPE_04_MAX/* 全スプライト */
};
//global OBJ obj99[OBJ_HEAD_00_0x0000_TAMA+OBJ_POOL_00_TAMA_1024_MAX];	/* 弾専用スプライトのリスト構造 */
//global OBJ obj99[OBJ_HEAD_01_0x0800_TEKI+OBJ_POOL_01_TEKI_0256_MAX];	/* 敵専用スプライトのリスト構造 */
//global OBJ obj99[OBJ_HEAD_02_0x0900_KOTEI+OBJ_POOL_02_KOTEI_0016_MAX]; /* 自機等固定スプライト */
//global OBJ obj99[OBJ_HEAD_03_0x0910_PANEL+OBJ_POOL_03_PANEL_0056_MAX]; /* パネル用スプライト */

/*---------------------------------------------------------
	オブジェバンクシステム
--------------------------------------------------------- */
extern void reflect_sprite_spec444(OBJ *sss, unsigned int bank_offset); /* オブジェクトのバンク設定を反映させる。 */
#endif

#endif /* _OBJ_AREA_H_ */
