
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#ifndef _KAIWA_SPRITE_00_H_
#define _KAIWA_SPRITE_00_H_

typedef struct /* _kaiwa_sprite_tag_ */
{
	int cx256;			/* objの中心 x 座標, 精度確保用(256固定小数点形式) */
	int cy256;			/* objの中心 y 座標, 精度確保用(256固定小数点形式) */
	int target_x256;	/* 目標 x 座標 */
	int target_y256;	/* 目標 y 座標 */
	int origin_x256;	/* 原点 x 座標 */
	int origin_y256;	/* 原点 y 座標 */
	int offset_x256;	/* 原点からの差分 x 座標 */
	int offset_y256;	/* 原点からの差分 y 座標 */
//[4]
	int speed256;		/* 移動速度 */		//	int move_wait;	/* 動きがあったときの制御用 */
	int angle1024;		/* 正確な方向 */	//	int r_course;
	int w_bit;				/* objの幅(1<<bit) */
	int h_bit;				/* objの高さ(1<<bit) */
//[8]
	int move_flag;		/* 0:停止中。 0以外(仮に 1):移動中。 */
	SDL_Surface *img;	/* SDL 画像 Images. NULLの場合は画像が無い。 */
//[12]
} KAIWA_SPRITE;
//	u16 *render_image;	/* 画像data */
//	int used;
//
//	u8 alpha255;		/* alpha値 */
//	u8 flags;			/* 0:非表示, 1:表示, 2:tachie_window(2nd screen)に表示	ここでは表示させるかどうかだけ */
/*
-3 ==1 29
-2 ==0 30
-1 ==1 31
 0 ==0 32
 1 ==1
 2 ==0
 3 ==1
*/
enum
{
	KAIWA_SPRITE_00_TACHIE_RIGHT = 0,	//
	KAIWA_SPRITE_01_TACHIE_LEFT_,		//
	KAIWA_SPRITE_99_MAX/*最大値*/		//	(2/*32*/)	/*32*/ /*20*/
};
enum
{
	KAIWA_SPRITE_MOVE_FLAG_00_MOVE_COMPLETE = 0,
	KAIWA_SPRITE_MOVE_FLAG_01_MOVE_START,
	KAIWA_SPRITE_MOVE_FLAG_99_MAX/*最大値*/
};

/*static*/extern KAIWA_SPRITE kaiwa_sprite[KAIWA_SPRITE_99_MAX];	/* 汎用スプライト */


#endif /* _KAIWA_SPRITE_00_H_ */
