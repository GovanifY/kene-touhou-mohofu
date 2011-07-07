
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _SPRITE_H_
#define _SPRITE_H_

/*---------------------------------------------------------
	スプライト マネージャ
--------------------------------------------------------- */
/*
寿命:   s32 JYUMYOU
1[秒]                                   60           >                64 [flame]
1[分]                                   60 x 60      >           64 x 64 [flame]
1[時間] 0x034BC0 ==   216000            60 x 60 x 60 >      64 x 64 x 64 [flame] ==   262144 0x040000
2.5[日] 0xC5C100 == 12960000 2.5 x 24 x 60 x 60 x 60 > 64 x 64 x 64 x 64 [flame] == 16777216 0x01000000
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


//#define SP_FLAG_COLISION_CHECK	(0x01)				/* あたり判定ありフラグ */
//#define SP_FLAG_GRAZE 			(0x10)				/* グレイズ済みかとうかのフラグ */
#define SP_FLAG_COLISION_CHECK		(0x8000)			/* あたり判定ありフラグ */
#define SP_FLAG_GRAZE				(0x0080)			/* グレイズ済みかとうかのフラグ */
#define SP_FLAG_OPTION_VISIBLE		(SP_FLAG_GRAZE) 	/* プレイヤーオプション有効フラグ */


/* Sprites */
//#define MAX_SPRITE 200

#define USER_BOMOUT_WAIT (30)

/* 使用済みだから消してくれフラグ */
//#define SP_DELETE 			(0)
/*	0 ならば判別処理が省略できる。 -1 だと一々判別処理が入る
	KETMでは判別しないとならない場所に判別処理が書いてない為、
	スプライトがおかしくなるバグが複数ヶ所にある。 */

/* 管理方式が違うので、単なるOFF */
#define FIX_OBJ_OFF 			(SP_DELETE)

/*
	ここは種類別ではなくて、機能別に分類してください。
*/
#define SP_GROUP_SHOT_ZAKO					(0x0000)/*0x0100*/
#define SP_GROUP_SHOT_SPECIAL				(0x0080)/*0x0100*/		/* 霊夢の回転ショット */		/* kodomo_hlaserの判別 */
//
#define SP_GROUP_JIKI_GET_ITEM				(0x0100)/*0x0100*/		/* === Guプレイヤー面 === */
//#define dummy_SP_GROUP_PLAYER_CORE		(0x0200)/*0x0100*/
#define SP_GROUP_TEKI						(0x0200)/*0x0400*/		/* === Gu敵面 === */
//#define SP_GROUP_BOSS 					(0x0800)/*0x0400*/		/* === Gu敵面 === */
//#define SP_GROUP_TEKI 					(0x0400)/*0x0400*/
//#define SP_GROUP_TEKI 					(SP_GROUP_TEKI|0)
//#define SP_GROUP_BOSS 					(SP_GROUP_TEKI|SP_GROUP_SHOT_SPECIAL)
#define SP_GROUP_BULLETS					(0x0400)/*(0x1000)0x0800*/		/* === Gu弾幕面 === */
#define SP_GROUP_ITEMS						(0x0800)/*(0x2000)0x1000*/
//#define SP_GROUP_PAUSE_S P_ME NU_TEXT 	(0x4000)/*0x2000*/		/*SDL*/
/*???*/#define SP_GROUP07_FRONT 				(0x1000)/*(0x8000)*/				/* === Guフロント面 === */

//#define SP_GROUP_ALL_SDL_TYPE (0xff00)
#define SP_GROUP_ALL_SDL_WORK_TYPE			(0x1f00)/* SDL動作 */
//#define SP_GROUP_ALL_SDL_CORE_TYPE		((0x1f00) & (~(SP_GROUP_PAUSE_S P_ME NU_TEXT|0)))/* game_core動作 */
#define SP_GROUP_ALL_SDL_CORE_TYPE			(0x1f00)/* game_core動作 */

//#define SP_GROUP_ALL_SDL_DRAW_TYPE		(SP_GROUP_ALL_SDL_WORK_TYPE & (~(SP_GROUP_TEKI|SP_GROUP_ITEMS|SP_GROUP_BULLETS)))/*SDL表示*/
/* 敵＆アイテム＆敵弾は少なくともSDLで描かない */
//#define SP_GROUP_ALL_SDL_DRAW_TYPE		(0x4000)/* SDL表示(現状SP_GROUP_PAUSE_S P_ME NU_TEXTのみSDL描画) */

//#define SP_GROUP_SUB_TYPE_256 			(0x00ff)
#define SP_GROUP_SUB_TYPE_128				(0x007f)
/* 以下あたり判定なし */
/*???*/ 	//#define SP_GROUP_MAHOU_JIN			(0x2000)/*(0x00010000)*/			/* === Gu魔方陣面 === */
/*???*/ 	//#define SP_GROUP_PANEL				(0x4000)/*(0x00020000)*/			/* === Guパネル面 === */
//#define SP_FLAG_COLISION_CHECK			(0x8000)/*(0x00020000)*/			/* === Guパネル面 === */

#define SP_GROUP_ALL_GAME_OBJS				(0x00ffff00)			/* ゲームコア終了時にかたづけるobj */
//#define SP_GROUP_SHOTS					(0x0200)
//#define SP_GROUP07_FRONT						(0x4000)
//#define SP_GROUP_ENEMYS					(0x0200)/*(0x0400)*/
/* 共用 */
//#define SP_GROUP_TEXTS					(SP_GROUP_PLAYER)/*(0x2000)*/

/* SP_GROUP07_FRONT == SP_GROUP07_FRONT */
//#define SP_GROUP07_FRONT						(SP_GROUP07_FRONT)/* フロント面 */

		//←ウェポンアイテム(中)	//ウェポンアイテム(強)→SP_ITEM_EXTRA_SHIELD,
		//→これらは低速ボムに吸収された。SP_ITEM_EXTRA_HOMING, SP_ITEM_EXTRA_HLASER,
/*	自分A */
//	S P_PLAYER_ITEM/*プレイヤーの本体(あたり判定がアイテム取得)*/			= /*0x0100*/SP_GROUP_JIKI_GET_ITEM, /* Unser Held */
/*	自弾 */
//	S P_SHOT_BOSS,/*プレイヤーのボスに有効弾*/							//	= /*0x0200*/SP_GROUP_SHOTS, 	/* Waffen des Helden */
//	S P_SHOT_ZAKO,/*プレイヤーのボスに無効弾*/
/*	自分B */
	//dummy_SP_PLAYER_CORE/*プレイヤーのコア(あたり判定が死亡判定)*/		= /*0x0100*/SP_GROUP_PLAYER_CORE,
/* ------------- */
/*	ザコ敵 (ボム中、体当たりでプレイヤー死なない) */
//	SP_ZAKO 																= /*0x0400*/SP_GROUP_TEKI,		/* Die Boesen */
/* ------------- */
/*	ボス敵 / 中ザコ敵 (ボム中、体当たりでプレイヤー死ぬ) */
//	S P_CHUU,// /*特殊敵[中型敵]*/ /*S P_ZA KO_YOKAI1*/ 					= /*0x0400*/SP_GROUP_BOSS,		/* [***090325 */
//	S P_BO SS,
/* ------------- */
/*	敵弾 */
	//dummy_SP_BULLET														= /*0x0800*/SP_GROUP_BULLETS,	/* ... und ihre Waffen */
/* ------------- */

//enum /*sprite_type*/
//{
/* ------------- */
/*	ポーズ中移動可能物 */
//	S P_ME NU_TEXT	/* 文字 */												= /*0x2000*/SP_GROUP_PAUSE_S P_ME NU_TEXT,
/*	その他 */
//	SP_MUTEKI																= /*0x4000*/SP_GROUP07_FRONT,		/* diverses */
//};

//#define SP_MUTEKI 	SP_GROUP07_FRONT

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
	OBJ_BANK_01_REIMU_A,	// 自機用バンク(霊夢A、霊符)
	OBJ_BANK_02_REIMU_B,	// 自機用バンク(霊夢B、夢符)
	OBJ_BANK_03_MARISA_A,	// 自機用バンク(魔理沙_A、魔符)
	OBJ_BANK_04_MARISA_B,	// 自機用バンク(魔理沙_B、恋符)
	OBJ_BANK_05_REMILIA,	// 自機用バンク(レミリア)
	OBJ_BANK_06_YUYUKO, 	// 自機用バンク(幽々子)
	OBJ_BANK_07_CIRNO_A,	// 自機用バンク(チルノA)
	OBJ_BANK_08_CIRNO_Q,	// 自機用バンク(チルノ⑨)
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


/*-------------------*/
/* 針弾とかあるから、現在図形判定しないと詐欺判定になる。 */
/* あたり判定無くしても速度は変わらないがメモリが節約できるので将来的にはこの辺も改良したい。 */
//#define USE_ZUKEI_ATARI_HANTEI (1)
//#define USE_ZUKEI_ATARI_HANTEI (0)

typedef struct /*_point256*/
{
	Uint16 x;	/*	union POINT */	/* x 位置 */
	Uint16 y;	/*	union POINT */	/* y 位置 */
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
	#define base_hp 					katasa
	#define base_weapon_strength		katasa
	#define base_time_out				katasa/*bakuhatsu*/
//	#define base_score					user01
	#define base_score					user_data13
	//#define base_state				user01
#endif
//
/* スプライトの使い方

分類		自機		自弾					敵					敵弾			爆発
katasa		--			base_weapon_strength	base_hp 			base_time_out	base_time_out
			--			武器強さ				体力				時間			時間
user_data13 	--			--						base_score			--				--
			--			--						スコア				--				--
*/

#define tmp_angleCCW65536 tmp_angleCCW1024
typedef struct _sprite
{
	int cx256;						/*	union POINT */			/* center x 表示 中心座標位置  (256固定小数点形式) / akt. Position */
	int cy256;						/*	union POINT */			/* center y 表示 中心座標位置  (256固定小数点形式) / akt. Position */
	int vx256;						/*	union POINT_VECTOR */	/* 共用regist_vector(); */	/*union BULLET_VECTOR_DATA*/
	int vy256;						/*	union POINT_VECTOR */	/* 共用regist_vector(); */	/*union BULLET_VECTOR_DATA*/
//[4]
	int rotationCCW1024;			/* 描画用角度(下が0度で左回り(反時計回り), 一周は1024分割, 0-1023度) */
	int tmp_angleCCW1024;			/* 保持用角度(下が0度で左回り(反時計回り), 一周は1024分割, 0-1023度) */
	int katasa; 					/* user00 ENEMY_BASE base; */	//int health;	int time_out;		/* user03 */
	int type;						/* 種類及び使用可否 / Sprite-Type, (siehe enum SPRITE_TYPE), 0 = remove. */
//[8]
	u32 color32;					/* Gu color AGBR8888 MAKE32RGBA(RED, GREEN, BLUE, ALPHA)で各要素 0 から 255 (0x00 から 0xff)まで。 */
	int m_Hit256R;					/* あたり判定用 */
	int m_zoom_x256;				/* 表示拡大率x (画像の横の拡大率t256()形式で指定) */
	int m_zoom_y256;				/* 表示拡大率y (画像の縦の拡大率t256()形式で指定) */
//[12]
	/* とりあえず移行用(1)従来互換(弾) */
	int user_data00;
	int user_data01;
	int user_data02;
	int user_data03;
//[16]
	int user_data04;
	int user_data05;
	int user_data06;
	int user_data07;
//[20]
	/* とりあえず移行用(2)追加弾、基本機能 */
	int user_data10;
	int user_data11;
	int user_data12;
	int user_data13;		/* 敵スコア */
//	int user01; 					/* user01 ENEMY_BASE base; */	//int score;	int state;			/* user02 */
//
	void (*callback_loser)(struct _sprite *src);	/* Custom-loser-Routine (やられた後にボーナスを出すとか、違う爆発するとか) / 自機オプションのアニメーションコールバック */
	void (*callback_mover)(struct _sprite *src);	/* Custom-Move-Routine */
	void (*callback_hit_teki)(struct _sprite *c, struct _sprite *src);	// [***090325		追加
	struct _sprite *target_obj; 		/* とりあえず移行用(3)移行したらなくなる。 */
//[20]
//
	/*u8*/u32 flags;					/* フラグセット / siehe unten (SP_FLAG...) */
	s32 jyumyou;	/* 寿命 */
//	u32 dummy_used;/*???*/
//	int dummy111;/* 使ってないダミー */
// 描画用
//	u16 u0;
//	u16 v0;
//	u16 u1;
//	u16 v1;
	/*u16*/u16/*int*/ tx;	//tx = u0;	int tx; 	/* u テクスチャ座標 */
	/*u16*/u16/*int*/ ty;	//ty = v0;	int ty; 	/* v テクスチャ座標 */
	/*u16*/u16/*int*/ w;	//w = (u1-u0);	int w;
	/*u16*/u16/*int*/ h;	//h = (v1-v0);	int h;
} SPRITE;
// 1858355 1858307
/*---------------------------------------------------------
	jyumyou 寿命 life_cycle sprite_time_out sprite_longevity
	-------------------------------------------------------
	名前は忘れたけど、KETMにあった考え方。
	一度は排除したが、これをもう一度復活させる。
	ただし、システムで強制的に値を加算する。
	スプライトが生成されると、値が 1。
	スプライトをフレーム処理する度に、1 加算される。
	-------------------------------------------------------
	pspは 0 レジスタがあるので、0 未満、
	つまり負数は 自動消去。
	-------------------------------------------------------
	int じゃなくて short にする案。
	0x7fff==32767 は正数だが、1加算すると signed short の場合、
	0x8000==32768==-32768==負数になるので、負数になったら自動消去される。
---------------------------------------------------------*/

/*	struct _sprite *target_obj;
	用途は(
		player用誘導弾、
		後ろの魔方陣"妖怪2"(赤)
	)
	ほんとは複雑にキャストすれば要らないんだけどバグでそうだし。
 */


//#ifndef GRP_SCREEN_H
//#define GRP_SCREEN_H
/*	@since		Jul.27.2005 GRP_SCREEN_H		画面表示管理 */



/*---------------------------------------------------------
	リソース resource
---------------------------------------------------------*/

extern SPRITE *obj_add_00_tama_error(void); 							/* gu移行中、弾専用 				int bullet_type_num */
extern SPRITE *obj_add_01_teki_error(void); 							/* gu移行中、汎用スプライト 		int bullet_type_num */
extern SPRITE *obj_add_nn_direct(unsigned int register_object_absolute_direct_number);	/* 強制登録用。危険なので注意して使う。*/
/* (r33)で名前だけでなく仕様が変わったので注意。 / 専用固定スプライトの場合は固定値までのオフセット
OBJ_HEAD_02_KOTEIを足す。 */

extern void sprite_all_cleanup(void);
extern void sprite_move_all(void);		/* スプライトオブジェクトの移動処理 */


extern SPRITE *obj_collision_check_01_teki(SPRITE *tocheck);/*敵専用*/	//, int type);
extern SPRITE *obj_collision_check_00_tama(SPRITE *tocheck, int type);/*gu弾幕専用*/

extern void gamen_gai_nara_zako_osimai(SPRITE *src);/* 画面外ならおしまい */
extern void check_boss_option_time_out(SPRITE *src);/* */

#if 1
/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
/*画面外なら弾を消す*/
extern void bullet_angle_all_gamen_gai_nara_kesu(void);

/*画面外なら弾を反射*/
extern void bullet_angle_all_gamen_gai_nara_hansya(void);		/* 反射弾のてすと */

/*画面外なら弾を反射減速*/
extern void bullet_angle_all_gamen_gai_nara_hansya_gensoku(void);	/* 反射減速弾のてすと */
#endif

/* 使用中であるが退避したい場合。画面外にスプライトを移動させ、無効にする。 */
extern void sprite_initialize_position(SPRITE *src);

//#if 1
//	/* ソフトウェアーで Zソート */
//	#define USE_ZBUFFER 	(0)
//#else
//	/* ハードウェアーでソート */
//	#define USE_ZBUFFER 	(1)
//#endif
//



#endif/* _SPRITE_H_ */



#ifndef _BULLET_SYSTEM_H_
#define _BULLET_SYSTEM_H_

/*---------------------------------------------------------
	敵弾管理システム
---------------------------------------------------------*/

//#if 000/* 移行中につき無効 */
//	/* MAX_POOL_BULLET: 2のn乗の必要あり(1024とか2048とか) */
//#define MAX_POOL_BULLET 1024
//extern TGameSprite bullet_pool[MAX_POOL_BULLET];
//
//extern void bullet_system_init(void);
//extern void bullet_system_exit(void);
//#endif /* 000 移行中につき無効 */




#if 1
/*---------------------------------------------------------
	新、敵弾管理システム(仮)
---------------------------------------------------------*/

enum
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
	PANEL_OBJ_30_J01,		/* 残り時間2桁目 */
	PANEL_OBJ_30_J02,		/* 残り時間1桁目 */
	PANEL_OBJ_31_G03,		/* グレイズ5桁目 */
	PANEL_OBJ_31_G04,		/* グレイズ4桁目 */
	PANEL_OBJ_31_G05,		/* グレイズ3桁目 */
	PANEL_OBJ_32_G06,		/* グレイズ2桁目 */
	PANEL_OBJ_33_G07,		/* グレイズ1桁目 */
	//
	OBJ_POOL_03_PANEL_MAX/* MAX */
};
//enum /*_common_my_obj_*/
//{
//	MY_O BJ_00_JIKI = 0,		/*[r32変更済み]*/
//	MY_O BJ_01_JIKI_MARU,		/**/		/*[r32変更済み]*/
//	MY_O BJ_02_JIKI_OPT01,		/**/		/*[r32変更済み]*/
//	MY_O BJ_03_JIKI_OPT02,		/**/		/*[r32変更済み]*/
//	MY_O BJ_04_JIKI_OPT03,		/**/		/*[r32変更済み]*/
//	MY_O BJ_05_JIKI_OPT04,		/**/		/*[r32変更済み]*/
//	MY_O BJ_06_SEND1,			/*[予定]*/
//	MY_O BJ_07_,				/*[予定]*/
//
//	MY_O BJ_08_BOSS,			/*[予定]*/
//	MY_O BJ_09_,				/*[予定]*/
//	MY_O BJ_0a_,				/*[予定]*/
//	MY_O BJ_0b_EFFECT01,		/*[予定]*/
//	MY_O BJ_0c_EFFECT02,		/*[予定]*/
//	MY_O BJ_0d_EFFECT03,		/*[予定]*/
//	MY_O BJ_0e_EFFECT04,		/*[予定]*/
//	MY_O BJ_0f_EFFECT05,		/*[予定]*/
//
//	MY_O BJ_99_MAX
//};
enum
{
	/* 描画プライオリティーが自機テクスチャーのもの */
	FIX_OBJ_00_PLAYER = 0,		/*<使用中>[r32変更済み] obj_add_nn_direct(OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER); */
	FIX_OBJ_01_JIKI_MARU,		/*<使用中>[r32変更済み] obj_add_nn_direct(OBJ_HEAD_02_KOTEI+FIX_OBJ_01_JIKI_MARU); */
	FIX_O_BJ_02_SEND1,			/*[予定]*/
	FIX_O_BJ_03_SEND2,			/*[予定]*/
	FIX_OBJ_04_JIKI_OPTION0,	/*<使用中>[r32変更済み] obj_add_nn_direct(OBJ_HEAD_02_KOTEI+FIX_OBJ_04_JIKI_OPTION0+jj); */
	FIX_OBJ_05_JIKI_OPTION1,	/*<使用中>[r32変更済み]*/
	FIX_OBJ_06_JIKI_OPTION2,	/*<使用中>[r32変更済み]*/
	FIX_OBJ_07_JIKI_OPTION3 ,	/*<使用中>[r32変更済み]*/
//
	/* 描画プライオリティーがfrontテクスチャーのもの */
//	xxx_FIX_OBJ_08_BOSS,		/* <あたり判定の都合上出来ない> */	/*<使用中>[r32変更済み] obj_add_nn_direct(OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS); */
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
	OBJ_POOL_02_KOTEI_MAX/*MAX*/
};
	/* 描画プライオリティーが自機テクスチャーのもの */
/* -- プライオリティー＃６．自機を描画 */
//#define SPRITE_111JIKI_POOL_MAX FIX_OBJ_07_JIKI_OPTION3

/*
 参考: kouma_tr013/東方紅魔郷/マニュアル/html/faq.html
Ｑ２３　アイテムの表示限界は？

　画面内５１２個までです。
　敵弾の数が６４０個なので、全てがボーナスアイテムになるとは限りませんが、
　殆どの場合はスコアパターンに影響するようなことはありません。

*/

/* 弾の最大数は OBJ_POOL_00_TAMA_MAX (==1024) それ以上登録しようとしても登録されない。 */
//#define OBJ_POOL_00_TAMA_MAX		(512)/* とりあえず */
#define OBJ_POOL_00_TAMA_MAX		(1024)/* 弾登録プログラムの都合上 2^n である必要があります。(1024==(1<<10) ) */
//extern SPRITE obj99[OBJ_HEAD_00_TAMA+OBJ_POOL_00_TAMA_MAX];		/* 弾専用スプライト */

/* 敵(自機、オプション、自弾等、弾以外の全てのスプライト含む)の最大数は OBJ_POOL_01_TEKI_MAX (==256) それ以上登録しようとしても登録されない。 */
#define OBJ_POOL_01_TEKI_MAX		(256)/* 敵登録プログラムの都合上 2^n である必要があります。(1024==(1<<10) ) */
//extern SPRITE obj99[OBJ_HEAD_01_TEKI+OBJ_POOL_01_TEKI_MAX];		/* 敵専用スプライト */

//extern SPRITE obj99[OBJ_HEAD_02_KOTEI+OBJ_POOL_02_KOTEI_MAX]; 	/* 敵専用スプライト */

/* パネル用スプライトは OBJ_POOL_03_PANEL_MAX (==256) それ以上登録しようとしても登録されない。 */
//#define OBJ_POOL_03_PANEL_MAX 	(256)/* 敵登録プログラムの都合上 2^n である必要があります。(1024==(1<<10) ) */
//extern SPRITE obj99[OBJ_HEAD_03_PANEL+OBJ_POOL_03_PANEL_MAX]; 	/* パネル用スプライト */





	/* 全スプライト */
#define OBJ_HEAD_00_TAMA	(0) 										/* 弾専用スプライト */
#define OBJ_HEAD_01_TEKI	(OBJ_HEAD_00_TAMA+OBJ_POOL_00_TAMA_MAX) 	/* 敵専用スプライト */
#define OBJ_HEAD_02_KOTEI	(OBJ_HEAD_01_TEKI+OBJ_POOL_01_TEKI_MAX) 	/* 自機等固定スプライト */
#define OBJ_HEAD_03_PANEL	(OBJ_HEAD_02_KOTEI+OBJ_POOL_02_KOTEI_MAX)	/* パネル用スプライト */
#define OBJ99_MAX			(OBJ_HEAD_03_PANEL+OBJ_POOL_03_PANEL_MAX)
extern SPRITE obj99[OBJ99_MAX]; 	/* 全スプライト */


/*---------------------------------------------------------
	オブジェバンクシステム
--------------------------------------------------------- */
extern void reflect_sprite_spec444(SPRITE *sss, unsigned int bank_offset);	/* オブジェクトのバンク設定を反映させる。 */
#endif

#endif /* _BULLET_SYSTEM_H_ */
