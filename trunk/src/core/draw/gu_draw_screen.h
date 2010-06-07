
/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#ifndef _GU_DRAW_SCREEN_H_
#define _GU_DRAW_SCREEN_H_



extern void TGameTexture_Load_Surface( int num );
//static void TGameTexture_Load_Surface(int num);





/*	*/
//#if (1==USE_16BIT_BLIT)
//	#define USE_COLOR_16_AND_32 0
//#endif
	#define USE_COLOR_16_AND_32 1



//#define USE_SDL_image 1
#define USE_SDL_image 0
#if (1==USE_SDL_image)
	//#include "_SDL_image.h"//#include "_SDL_image.h"
	#define MY_DIB_SURFACE	SDL_Surface
	#define MY_DIB_DATA 	pixels
	#define MY_DIB_WK512	unused1
#else
	#include "graphics00.h"
	#define MY_DIB_SURFACE	my_image
//	#define MY_DIB_DATA 	data
	#define MY_DIB_DATA 	pixels
	#define MY_DIB_WK512	wk512
#endif

//#define USE_SWIZZLE 0
#define USE_SWIZZLE 1

//#define USE_16BIT_BLIT 0
#define USE_16BIT_BLIT 1

//#define USE_BLIT_COLOR_BIT 32
#define USE_BLIT_COLOR_BIT 16

//#define TW128 128/* 弾テクスチャの幅 */
//#define TH128 128/* 弾テクスチャの高さ */
//#define WK512 512/* 弾テクスチャを描画する場合に、描画ワークエリアの幅 */

/* 大きなスプライトを描画するときの分割サイズ */
#define SLICE_64_SIZE	(64)

/* 1:頂点カラーを使う。 */
#define USE_VCOLOR 1/*1*/

/* --- GU 描画のためのパケットサイズ */
#define PACKET_SIZE  (262144)

/* gulist設定が小さすぎると都合が悪い */
#if (PACKET_SIZE<(512*512)) /* shere short16==(512*512/2*short) and int32 ==(512*512*int) */
	#undef	PACKET_SIZE
	#define PACKET_SIZE  (512*512)
#endif

//#define UNKNOWN_MY_FLAGS (GU_COLOR_5650)

	#if (1==USE_VCOLOR)
		/* 頂点カラーを使う場合。一般的。 */
		#if (16==USE_BLIT_COLOR_BIT)
			/* 15/16bit描画 */
//		//	#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
//		//	#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
//			#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
//
		//	#define TEXTURE_FLAGS4444		(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS5650		(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS5650_C32	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		//	#define TEXTURE_FLAGS4444		(GU_TEXTURE_16BIT | GU_COLOR_5551 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS4444		(GU_TEXTURE_16BIT | GU_COLOR_4444 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		#else
			/* 32bit描画 */
			#define TEXTURE_FLAGS5650	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		#endif
	#else
		/* 頂点カラーを使わない場合。「GU_COLOR」を付けると動かない。 */
		//	#define TEXTURE_FLAGS		(GU_TEXTURE_16BIT | 				GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS		(									GU_VERTEX_16BIT | GU_TRANSFORM_2D)
	#endif

// /* Vertex Declarations Begin */
//#define GU_TEXTURE_SHIFT(n)	((n)<<0)
//#define GU_TEXTURE_8BIT		GU_TEXTURE_SHIFT(1) 	0x01
//#define GU_TEXTURE_16BIT		GU_TEXTURE_SHIFT(2) 	0x02
//#define GU_TEXTURE_32BITF 	GU_TEXTURE_SHIFT(3) 	0x03
//#define GU_TEXTURE_BITS		GU_TEXTURE_SHIFT(3) 	0x03

//	*	- GU_COLOR_5650 - 16-bit color (R5G6B5A0)
//	*	- GU_COLOR_5551 - 16-bit color (R5G5B5A1)
//	*	- GU_COLOR_4444 - 16-bit color (R4G4B4A4)
//	*	- GU_COLOR_8888 - 32-bit color (R8G8B8A8)

//#define GU_COLOR_SHIFT(n) ((n)<<2)
//#define GU_COLOR_RES1 	GU_COLOR_SHIFT(1)	0x04
//#define GU_COLOR_RES2 	GU_COLOR_SHIFT(2)	0x08
//#define GU_COLOR_RES3 	GU_COLOR_SHIFT(3)	0x0c
//#define GU_COLOR_5650 	GU_COLOR_SHIFT(4)	0x10
//#define GU_COLOR_5551 	GU_COLOR_SHIFT(5)	0x14
//#define GU_COLOR_4444 	GU_COLOR_SHIFT(6)	0x18
//#define GU_COLOR_8888 	GU_COLOR_SHIFT(7)	0x1c
//#define GU_COLOR_BITS 	GU_COLOR_SHIFT(7)	0x1c





/*static*/extern	unsigned int __attribute__((aligned(16))) gulist[PACKET_SIZE];















/* 管理するテクスチャー(メモリに同時にロードする可能性のあるテクスチャー) */
enum
{
	TEX_00_BACK_GROUND = 0, 	/* [256x256]3D背景1 */
//	TEX_01_BACK_TEXTURE,		/* 背景障害物 */
	TEX_02_MAHOUJIN,			/* [128x128]魔方陣 */
	TEX_03_JIKI,				/* [256x256]自弾/自機 */
	TEX_04_TEKI,				/* ボス/ザコ敵 */
//	TEX_05_ITEM,				/* アイテム/漢字スコア */
	TEX_06_BULLET,				/* [128x128]敵弾 */
	TEX_07_FRONT,				/* [256x256]自機当たり表示/爆発/[コンティニュー文字(bank00)/メニュー文字(bank01)/メニュー文字(bank02)] */
//	TEX_08_SCORE_PANEL, 		/* [TEX_07_FRONTと融合する可能性が高い(256x512テクスチャ？)] スコアパネル/スコアフォント文字 */
//	TEX_09_TACHIE,				/* 立ち絵 */
//	TEX_10_MESSAGE, 			/* [TEX_07_FRONTと融合する可能性が高い(512x512テクスチャ？)]メッセージ固定文字 */
	TEXTURE_MAX 				/* --- 管理する最大テクスチャー数 */
};

/*---------------------------------------------------------
	スクリーン管理
---------------------------------------------------------*/

/* テクスチャをスプライトとして使う場合の管理テーブル書式 */
typedef struct
{
	unsigned char	u;	/* xテクスチャ位置、座標 */
	unsigned char	v;	/* yテクスチャ位置、座標 */
//
	unsigned char	w;	/* width 幅 */
	unsigned char	h;	/* height 高さ */
} VIRTUAL_OBJ_STATE;	/* スプライト一つ分(の大きさを管理) */

/* テクスチャごとに管理 */
typedef struct
{
	MY_DIB_SURFACE		*my_texture;		/* テクスチャ画像 */
	int 				texture_width;		/* テクスチャ幅 */
	int 				texture_height; 	/* テクスチャ高さ */
	int 				buffer_width;		/* 512 固定？ */
//
	int 				color_format;		/* 変換済み画像形式 */
	/*	読み込み時にARGB8888から画像変換を行うが、
		どういう形式に画像変換するか指定する。
		16bit形式でも ABGR0565 とか ABGR1555 とか ABGR4444 がある。
	 */
	int 				hh;/*予備*/
	VIRTUAL_OBJ_STATE	*object_table_head; 	/* スプライト管理テーブルの先頭 */
	char				*file_name;/* ファイル名 */
} MY_TEXTURE_RESOURCE;

/*static*/extern MY_TEXTURE_RESOURCE	my_resource[TEXTURE_MAX];


























#endif /* _GU_DRAW_SCREEN_H_ */

