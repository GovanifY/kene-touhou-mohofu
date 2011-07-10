
#if (1)
#include "boss.h"//#include "game_main.h"

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
	弾を全弾書き換え方式に移行するのでここは無くなる予定。
	-------------------------------------------------------
	ボス共通規格使用データー:
---------------------------------------------------------*/

// バグ対策。 user_data02 良くワカンナイけど空けとく。()
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
		/* 変身する。 */
		{
			src->type			= BULLET_KNIFE20_07_MIDORI;/* (青→緑ナイフに変身) */
			reflect_sprite_spec444(src, OBJ_BANK_SIZE_00_TAMA);
			#if 1/* わざと狙わない */
			u32 ra_nd32 = ra_nd();
			SPRITE *zzz_player;
			zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		//	set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
			#endif
			REG_00_SRC_X	= (zzz_player->cx256)+((ra_nd32<<8)&0x1fff)-t256(16);
			REG_01_SRC_Y	= (zzz_player->cy256)+((ra_nd32  )&0x0fff)-t256(8);
			REG_02_DEST_X	= ((src->cx256));
			REG_03_DEST_Y	= ((src->cy256));
			tmp_angleCCW65536_src_nerai();
			src->tmp_angleCCW1024 = (deg65536to1024(HATSUDAN_03_angle65536));		/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
			mask1024(src->tmp_angleCCW1024);/* ねんのため */
			src->rotationCCW1024 = ((src->tmp_angleCCW1024));	/* 表示角度 */
		}
		#if (0)//
		src->vx256 = ((si n1024((src->tmp_angleCCW1024)) )<<1);/*fps_factor*/	/* 緑ナイフの2nd狙い速度 */ /* 速度 */	/* CCWの場合 */
		src->vy256 = ((co s1024((src->tmp_angleCCW1024)) )<<1);/*fps_factor*/	/* 緑ナイフの2nd狙い速度 */ /* 速度 */
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (src->tmp_angleCCW1024), &sin_value_t256, &cos_value_t256);
			src->vx256 = ((sin_value_t256 )<<1);/*fps_factor*/
			src->vy256 = ((cos_value_t256 )<<1);/*fps_factor*/
		}
		#endif
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

/*
	-------------------------------------------------------
	ToDo:
	弾システムに移行する為、廃止予定。
	カードスクリプトシステムが稼動する為には、発弾を一ヶ所に統合しないと
	速度的な面でこのままの形態では無理があるので、
	(ここの発弾処理のせいで他の弾が処理落ちする)
	ここの発弾処理を hatudan_system_regist_single に置きかえる必要がある。
---------------------------------------------------------*/

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
	for (ii=0; ii<65536; ii += tmp_step[(REG_0f_GAME_DIFFICULTY)] ) 	/* プレイヤーの周りに8弾配置する */
	{
		SPRITE *h;
		h					= obj_add_00_tama_error();
		if (NULL != h)
		{
			#if (0)//
			h->cx256 = (tmp_x256) + ((si n1024((deg65536to1024(ii))))<<7);/*fps_factor*/	/* CCWの場合 */
			h->cy256 = (tmp_y256) + ((co s1024((deg65536to1024(ii))))<<7);/*fps_factor*/
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (deg65536to1024(ii)), &sin_value_t256, &cos_value_t256); 	/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
				h->cx256 = (tmp_x256) + ((sin_value_t256)<<7);/*fps_factor*/
				h->cy256 = (tmp_y256) + ((cos_value_t256)<<7);/*fps_factor*/
			}
			#endif
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
#endif
