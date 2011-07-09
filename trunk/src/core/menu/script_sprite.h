
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#ifndef _SCRIPT_SPRITE_00_H_
#define _SCRIPT_SPRITE_00_H_

typedef struct /*_script_sprite*/
{
	int cx256;			/* objの中心座標, 精度確保用(256固定小数点形式) */	//	int/*int*/ x;		/* x座標だよ */
	int cy256;			/* objの中心座標, 精度確保用(256固定小数点形式) */	//	int/*int*/ y;		/* y座標だよ */
//[4]
	int w;				/* objの幅 */			//	int cw; 	/* 中心座標(幅) */
	int h;				/* objの高さ */ 		//	int ch; 	/* 中心座標(高さ) */
//
	int target_x256;	/* 目標座標 */
	int target_y256;	/* 目標座標 */
//[8]
	int set_speed;		/* 移動速度 */		//	int move_wait;	/* 動きがあったときの制御用 */
	int angle1024;		/* 正確な方向 */	//	int r_course;	/* 大体の方向 */
//[12]
	SDL_Surface *img;		/* SDL 画像 Images. NULLの場合は画像が無い。 */
//	UINT16 *render_image;	/* 画像data */
//	int used;
//
//	u8 alpha255;		/* alpha値 */
//	u8 flags;			/* 0:非表示, 1:表示, 2:tachie_window(2nd screen)に表示	ここでは表示させるかどうかだけ */
} SCRIPT_SPRITE;
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
	/*#define*/ SCRIPT_SPRITE_00_TACHIE_RIGHT = 0,	//; (30&1)/*(-2)*/	/*(==0)*/
	//#define SPRITE_tachie_m (31)/*(-1)*/		//	/*(==1)*/
	/*#define*/ SCRIPT_SPRITE_01_TACHIE_LEFT_,		//	(29&1)/*(-3)*/	/*(==1)*/
	/*#define*/ SCRIPT_SPRITE_99_MAX/*最大値*/		//	(2/*32*/)
};

//#define SPR ITE_tachie_l (29&1)/*-3*/
//#define SPR ITE_tachie_r (30&1)/*-2*/
//#define SPR ITE_tachie_m (31)/*-1*/
//#define SCRIPT_TACHIE_OBJ_MAX		(2/*32*/)

//static SCRIPT_SPRITE *tachie_r;
//static SCRIPT_SPRITE *tachie_l;
//static SCRIPT_SPRITE *std_obj[SCRIPT_TACHIE_OBJ_MAX/*32*/ /*20*/];		/* 汎用スプライト */
/*static*/extern SCRIPT_SPRITE standard_script_sprite[SCRIPT_SPRITE_99_MAX/*32*/ /*20*/];


#endif /* _SCRIPT_SPRITE_00_H_ */
