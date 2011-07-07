
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	咲夜のくるくるナイフ弾(青→緑)
	咲夜
	プレイヤーの周りをくるくる回ってから、自分狙いのナイフ。
	青→緑に色が変わる。
	-------------------------------------------------------
	全方向ナイフ(青)
	狙いナイフ(緑)
	-------------------------------------------------------
	プレイヤーの周りに散らばり、時間が経つとプレイヤー狙い弾
	-------------------------------------------------------
	弾幕書き換え方式に移行するのでここは無くなる予定。
---------------------------------------------------------*/

#define HATUDAN_TIME256 	(256)

static void bullet_move_kurukuru_knife(SPRITE *src)
{
	if (HATUDAN_TIME256 > src->jyumyou)
	{
		;
	}
	else
	if (HATUDAN_TIME256 == src->jyumyou)
	{
		{
			src->type			= BULLET_KNIFE20_07_MIDORI;/* (青→緑ナイフに変身) */
			#if 1/* わざと狙わない */
			u32 ra_nd32 = ra_nd();
			SPRITE *zzz_player;
			zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
			obj_send1->cx256	= (zzz_player->cx256)+((ra_nd32<<8)&0x1fff)-t256(16);
			obj_send1->cy256	= (zzz_player->cy256)+((ra_nd32  )&0x0fff)-t256(8);
			#endif
			tmp_angleCCW1024_src_nerai(obj_send1, src);
			mask1024(src->tmp_angleCCW1024);/* ねんのため */
			src->rotationCCW1024 = ((src->tmp_angleCCW1024));	/* 表示角度 */
		}
		src->vx256 = ((sin1024((src->tmp_angleCCW1024)) )<<1);/*fps_factor*/	/* 緑ナイフの2nd狙い速度 */ /* 速度 */
		src->vy256 = ((cos1024((src->tmp_angleCCW1024)) )<<1);/*fps_factor*/	/* 緑ナイフの2nd狙い速度 */ /* 速度 */
	}
	else
	{
		/* くるくるさせる */
		src->rotationCCW1024 += (64);
		mask1024(src->rotationCCW1024);/* ねんのため */
	}
	src->cx256 += (src->vx256); 	/*fps_factor*/
	src->cy256 += (src->vy256); 	/*fps_factor*/
}

global void bullet_crate_sakuya_kurukuru(SPRITE *src)
{
	SPRITE *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	const int tmp_x256		= zzz_player->cx256;
	const int tmp_y256		= zzz_player->cy256;
	/* ある程度遠くに出ないと認識しづらい */
	/* キャラ(コア)がでかいと避ける前に死ぬ。(ナイフが広がる前にあたる) */
	const int tmp_step[4]	=
	{
		(65536/8),
		(65536/10),
		(65536/12),
		(65536/16),
	};
	int ii;
	for (ii=0; ii<65536; ii += tmp_step[(cg_game_difficulty)] ) 	/* プレイヤーの周りに8弾配置する */
	{
		SPRITE *h;
		h					= obj_add_00_tama_error();
		if (NULL != h)
		{
			h->cx256			= tmp_x256 + ((sin1024((ii>>6)))<<7);
			h->cy256			= tmp_y256 + ((cos1024((ii>>6)))<<7);
			h->type 			= BULLET_KNIFE20_04_AOI;	/* (青→緑ナイフに変身) */
			reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
			h->m_Hit256R		= TAMA_ATARI_KNIFE18_PNG;
		//
			h->callback_mover	= bullet_move_kurukuru_knife;
			h->jyumyou			= (HATUDAN_TIME256+64);
		//
			h->vx256			= (0);
			h->vy256			= (0);
		}
	}
}
