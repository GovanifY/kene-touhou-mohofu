
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風  〜 Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	子供魔方陣 弾幕
	-------------------------------------------------------
	B.星型描くぜ
	-------------------------------------------------------
	(無駄に複雑だったので、
		A.レーザーを撃つ機能と、
		B.星型を描く機能と、
		C.360ナイフを撃つ機能と、
	を分離させた)
---------------------------------------------------------*/
#if 0/*メモ*/
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif
//	ボス共通規格と同じ(boss.hインクルードしてもしなくても対応)
#ifndef time_out
	#define time_out			user_data03 	/* 制限時間 */
#endif
//
#define my_angle1024		tmp_angleCCW1024	/* 保持角度[星型を描く場合に使う角度] */

#define doll_vx256			vx256
#define doll_vy256			vy256


/*---------------------------------------------------------
	子供魔方陣 移動
---------------------------------------------------------*/

static SPRITE hosi_position_obj;/* 星型描いてる座標位置を保持。 */
/*static*/extern void bullet_crate_sakuya_hosi_gata(SPRITE *src);
static void move_doll02(SPRITE *src)
{
	check_boss_option_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
//
	#if 1
	/* 魔方陣アニメーション */
	src->m_angleCCW1024--;/* 右回り */
	mask1024(src->m_angleCCW1024);
	#endif
//
	if ((64*8) < src->time_out)
	{
		/* 星型を描くよ */
		if (0==((src->time_out)&0x0f))
		{
			src->my_angle1024 += (1024*2/5);
			mask1024(src->my_angle1024);
			#define DOLL_SPEED10		(4)/*8*/
			src->doll_vx256 = ((sin1024((src->my_angle1024))*/*t256*/(DOLL_SPEED10))/*>>8*/);/*fps_factor*/ 			/* CCWの場合 */
			src->doll_vy256 = ((cos1024((src->my_angle1024))*/*t256*/(DOLL_SPEED10))/*>>8*/);/*fps_factor*/
			#undef DOLL_SPEED10
		}
		else
		{
			br.BULLET_REGIST_hosi_gata_angle1024		= ((src->my_angle1024))+(((src->time_out)&0x0f)<<4/*3*/);
			br.BULLET_REGIST_hosi_gata_time_out 		= ((src->time_out)&0x1ff);	//((src->time_out)-(64*8))
			br.BULLET_REGIST_hosi_gata_add_speed256 	= (((src->time_out>>2)&0x3)+4); 	/* 加加速度(か-かそくど) */
		//	br.BULLET_REGIST_hosi_gata_add_speed256 	= (((src->time_out )&0xf)|0x03);	/* 加加速度(か-かそくど) */
/*
0123 3333 4444	0+4
4567 7777 5555	1+4
89ab bbbb 6666	2+4
cdef ffff 7777	3+4
*/
			/* CCWの場合 */
			bullet_crate_sakuya_hosi_gata(&hosi_position_obj);
		}
		// 動作
		hosi_position_obj.x256 += (src->doll_vx256);/*fps_factor*/
		hosi_position_obj.y256 += (src->doll_vy256);/*fps_factor*/
	}
//
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

/*static*/ void add_zako_sakuya_doll_02_star(SPRITE *src)
{
	SPRITE *h;
	h								= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		h->m_Hit256R				= ZAKO_ATARI16_PNG;
	//	h->type 					= TEKI_51_MAHOJIN1;
		h->type 					= BOSS_16_YOUSEI11;
		h->base_hp					= (9999);/* 倒せない */
		h->flags					= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//	h->color32					= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);
	//	h->color32					= MAKE32RGBA(0x88, 0x33, 0xff, 0x80); /* 紅っぽく */
		h->color32					= MAKE32RGBA(0xff, 0x33, 0x66, 0x80); /* 紅っぽく */
		h->callback_mover			= move_doll02;
	//
		/* 子供魔方陣、配置位置 */
		#if 1
		h->x256 					= (src->x256);
		h->y256 					= (src->y256)-t256(16);/*咲夜 上方に配置*/
		#endif
		/* 弾を撃ち始める位置(星型描き始める位置) */
		hosi_position_obj.x256		= (src->x256)+(ra_nd()&0xfff);
		hosi_position_obj.y256		= (src->y256)+(ra_nd()&0xfff);/* 咲夜 下方から描く */
	//
		h->time_out 				= ((64*8)+(5*16)+1); 	/* 制限時間 */

		/* 星型を描く準備 */
		h->my_angle1024 			= (0);
	//	h->doll_vx256				= t256(1.0);	/*-t256(0.125)*/ /*t256(1.0)*/
	//	h->doll_vy256				= t256(0.0);	/* t256(2.0)*/ /*t256(0.0)*/
	}
}
