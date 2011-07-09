
#include "boss.h"

/*static*/extern void add_zako_sakuya_doll_01_laser(SPRITE *src);/* レーザー */
/*static*/extern void add_zako_sakuya_doll_02_tama_oki(SPRITE *src);/* 珠を置く */
/*static*/extern void add_zako_sakuya_doll_03_star(SPRITE *src);/* 子供魔方陣(星型を描く) */
/*static*/extern void add_zako_sakuya_doll_04_360knife(SPRITE *src);/* 子供魔方陣(360ナイフを撃つ) */
/*static*/extern void add_zako_sakuya_baramaki1(SPRITE *src);/* 咲夜 ばら撒き1弾 オプション */
/*static*/extern void add_zako_sakuya_baramaki2(SPRITE *src);/* 咲夜 ばら撒き2弾 オプション */

/*static*/extern void bullet_create_sakuya_ryoute_knife(SPRITE *src);	/* 咲夜 両手ナイフ弾幕 */
/*static*/extern void bullet_crate_sakuya_linear_yuudou(SPRITE *src);	/* 咲夜 直線誘導ナイフ弾幕 */
/*static*/extern void bullet_crate_sakuya_kurukuru(SPRITE *src);		/* 咲夜 くるくるナイフ弾幕 */

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	十六夜 咲夜
	Izayoi Sakuya.
	-------------------------------------------------------
	ToDo:
	移動と弾幕を分離しよう。でないと訳わからん。
	ボスタイマー対応中
	-------------------------------------------------------
	★ボスタイマーはシステムでサポート予定です。(作業中)
	★弾幕はシステムでサポートする構想があります。(構想中)
	システムでサポートされると、どのボスが、どの弾幕でも撃てるようになります。
---------------------------------------------------------*/
#if 0/* めも：咲夜は、共通部分で使ってる。 */
/* ボス共通規格 */
	#define target_x256 			user_data00 	/* 目標x座標 */
	#define target_y256 			user_data01 	/* 目標y座標 */
	#define toutatu_wariai256		user_data02 	/* 目標座標への到達割合 */
	#define kougeki_anime_count 	user_data03 	/* 攻撃アニメーション用カウンタ */
	#define boss_time_out			user_data04 	/* 制限時間 */
	#define boss_base_state777		user_data04 	/* 制限時間(boss_time_outと同じ) */
//
	#define boss_spell_timer		user_data05 	/* スペル時間 */
#endif

#define sakuya_data_move_angle1024		user_data06 	/* 咲夜の角度 */
#define sakuya_data_common_wait256		user_data07 	/* 咲夜の */


/*---------------------------------------------------------
	咲夜背後の魔方陣についてメモ
	-------------------------------------------------------
	5面中-ボスで出てくる時は、大きさ60x60(たぶん64x64obj)ぐらい。
	5面ボスで出てくる時は、大きさ125x125(たぶん128x128obj)ぐらい。
	模倣風は基本的に縮小率75%なので、咲夜背後の魔方陣は
	48x48[dot](2倍拡大時は96x96[dot])ぐらいが妥当かな？
---------------------------------------------------------*/

/*---------------------------------------------------------
	動かないでスペカ撃つ。
---------------------------------------------------------*/

/*---------------------------------------------------------
	SPELL_CARD_11_sakuya_kkk	第11形態: 最終形態(その3)
---------------------------------------------------------*/

global void sakuya_11_keitai(SPRITE *src)
{
	if (SPELL_00==spell_card.spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		spell_card_get_spell_number(src);	/* スペカシステムから、現在の形態で撃つ弾幕番号を手に入れ、標準のboss_base_spell_time_out時間を設定。 */
		src->toutatu_wariai256 = t256(1.0);
	}
//
	int iii;
	iii = ra_nd();
	if (0==(iii&0x07))
	{
		src->vx256 = (((signed int)((iii>> 8)&0xff))-127);
		src->vy256 = (((signed int)((iii>>16)&0xff))-127);
	}
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	SPELL_CARD_10_sakuya_jjj	第10形態: 最終形態(その2)
---------------------------------------------------------*/

global void sakuya_10_keitai(SPRITE *src)
{
	if (SPELL_00==spell_card.spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		spell_card_get_spell_number(src);	/* スペカシステムから、現在の形態で撃つ弾幕番号を手に入れ、標準のboss_base_spell_time_out時間を設定。 */
		src->toutatu_wariai256 = t256(1.0);
	}
//
	{
		static int seisei_count;
		if (0==seisei_count)
		{
			add_zako_sakuya_doll_03_star(src);/* 子供魔方陣追加 */
		//	add_zako_sakuya_doll_04_360knife(src);/* 子供魔方陣追加 */
		}
		seisei_count++;
		seisei_count &= 0x00ff; 	/* 256 == 0x100 == ( 4 x 64). [ 4秒ぐらい] */
	}
//
	{
		if (src->color32 > 0xf9000000)	/*	if (src->alpha > 0xf9) */	/* (src->alpha>0xff) */
		{
			src->color32 = 0xffffffff;	/*	src->alpha = 0xff;*/
		}
		else /*if (src->alpha<0xff)*/
		{
			src->color32 += 0x05000000; /*	src->alpha += 0x05;*/
		}
		src->cx256 -= ((sin1024((src->tmp_angleCCW1024))*(t256(0.03)))>>8);/*fps_factor*/		/* CCWの場合 */
		src->cy256 -= ((cos1024((src->tmp_angleCCW1024))*(t256(0.03)))>>8);/*fps_factor*/
	}
	//	[SAKUYA_ANIME_12_HATUDAN_START)
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	SPELL_CARD_09_sakuya_iii	第九形態: 幻葬「フェスティバルナイフ」 最終形態(その1)
---------------------------------------------------------*/
/*
0
	[A] 何もしない(待機時間)
80
	[B] 待機時間が終わったら移動準備
81
	[C] 真ん中に来るまで移動 //case SG01:
1024
	[D] 何もしない(待機時間)
2048 ==(1024+1024 )
	[E] 両手ナイフで攻撃
2049 ==(1024+1024+1)
	[F]移動
2089 ==(1024+10024+1+40)
	[G] 待機時間作成し、戻る。
*/
global void sakuya_09_keitai(SPRITE *src)
{
	src->boss_base_state777++;
	if (80 > src->boss_base_state777) /* [A] 何もしない(待機時間) */
	{
		;/* 何もしない(待機時間) */
	}
	else
	if (81 > src->boss_base_state777) /* [B] 待機時間が終わったら移動準備 */
	{
		//	src->boss_base_state777++;/* = SG01*/
//++		pd->bo ssmode = B00_NONE/*B01_BA TTLE*/;
		//	send1_xy(src);	/* 弾源x256 y256 中心から発弾。 */
			obj_send1->cx256	= (t256(GAME_WIDTH/2)); 		/* 弾源x256 */
			obj_send1->cy256	= (t256(30));					/* 弾源y256 */
			tmp_angleCCW65536_src_nerai(obj_send1, src);
			src->sakuya_data_move_angle1024 = ((src->tmp_angleCCW65536)>>6);		/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
	}
	else
	if (1024 > src->boss_base_state777) /* [C] 真ん中に来るまで移動 */
	{
	//case SG01:
		if ( (src->cx256 < t256((GAME_WIDTH/2)+20) ) &&
			 (src->cx256 > t256((GAME_WIDTH/2)-20) ) &&
			 (src->cy256 < t256(50.0)) ) /* 真ん中に来たら */
		{
			src->boss_base_state777 = (1024+1024-1)-(20);// src->boss_base_state777++;/* = SG02*/
			src->sakuya_data_common_wait256 	= t256(0.0);
			//	[SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			src->cx256 += ((sin1024((src->sakuya_data_move_angle1024))*t256(3.0))>>8);/*fps_factor*/	/* CCWの場合 */
			src->cy256 += ((cos1024((src->sakuya_data_move_angle1024))*t256(3.0))>>8);/*fps_factor*/
		}
	//	break;
	}
	else
	if ((1024+1024 ) > src->boss_base_state777) /* [D] 何もしない(待機時間) */
	{
		;/* 何もしない(待機時間) */
	}
	else
	if ((1024+1024 +1) > src->boss_base_state777) /* [E] 両手ナイフで攻撃 */
	{
//	case SG03:
		//	[SAKUYA_ANIME_15_HATUDAN_END)
		{
			bullet_create_sakuya_ryoute_knife(src);
			if (src->cy256 > t256(150))
		//	{	src->sakuya_data_move_angle1024 = cv1024r(	  210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;} 	/* CWの場合 */
		//	{	src->sakuya_data_move_angle1024 = cv1024r(360-210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;} 	/* CCWの場合 */
			{	src->sakuya_data_move_angle1024 = cv1024r(360-210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
			else
			{	src->sakuya_data_move_angle1024 = ((ra_nd()&(1024-1)));}
			#if 1
			/*念の為*/
			mask1024(src->sakuya_data_move_angle1024);
			#endif
			//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
		//	src->boss_base_state777++;/* = SG04*/
		}
	//	break;
	}
	else
	if ((1024+1024 +1+40) > src->boss_base_state777) /* [F]移動 */
	{
		/* CCWの場合 */
		src->cx256 += ((sin1024((src->sakuya_data_move_angle1024))*t256(3.5))>>8);/*fps_factor*/
		src->cy256 += ((cos1024((src->sakuya_data_move_angle1024))*t256(3.5))>>8);/*fps_factor*/
	}
	else /* [G] 待機時間作成し、[D]に戻る。 */
	{
	//case SG04:
		//	src->boss_base_state777 = SG02;
		//	src->boss_base_state777 = (1024+1024-1)-(120)-((3-(cg_game_difficulty))*50);
		// difficulty(0) = (1927)-((3)*50) == 1777 == (271)c
		// difficulty(1) = (1927)-((2)*50) == 1827 == (221)c
		// difficulty(2) = (1927)-((1)*50) == 1877 == (171)c
		// difficulty(3) = (1927)-((0)*50) == 1927 == (121)c
		//	src->boss_base_state777 = (1024+1024-1)-(120)-((3-(cg_game_difficulty))*64);
		//	src->boss_base_state777 = (1024+1024-1)-(120)-(3*64)+(cg_game_difficulty*64);
			src->boss_base_state777 = (1024+1024-1)-(120)-(3*64)+(cg_game_difficulty<<6);
	//	break;
	}
				SPRITE *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	#if 1/*Gu(中心座標)*/
	if ((zzz_player->cx256 < src->cx256 + t256(25)) &&	/* ???????????? */
		(zzz_player->cx256 > src->cx256 - t256(25)))
	#endif
	{	src->sakuya_data_common_wait256 += (1+2+cg_game_difficulty);/*fps_factor*/}
		// difficulty(0) = 20+((3)*(30)) == 110(r33)  42(r34)==(128/3)
		// difficulty(1) = 20+((2)*(30)) ==  80(r33)  32(r34)==(128/4)
		// difficulty(2) = 20+((1)*(30)) ==  50(r33)  25(r34)==(128/5)
		// difficulty(3) = 20+((0)*(30)) ==  20(r33)  21(r34)==(128/6)
//	if (src->sakuya_data_common_wait256 > 20+((3-(cg_game_difficulty))*(30)))
	if (src->sakuya_data_common_wait256 > 128)
	{
		src->sakuya_data_common_wait256 		= 0;
	//	src->sakuya_data_common_wait256 		= 0;/*speed256*/(2) + ((cg_game_difficulty));
		//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
		{
	//		src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);			/*	src->alpha			= 0x00;*/
			src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x80);			/*	src->alpha			= 0x00;*/	/* 半透明 */
	//		//	[SAKUYA_ANIME_15_HATUDAN_END;//?????
			src->tmp_angleCCW1024	= 0;//(ii<<7);
		}
		bullet_crate_sakuya_kurukuru(src);/*,*/ /*1+difficulty,*/
	}
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}

/*---------------------------------------------------------
	SPELL_CARD_08_sakuya_hhh	第八形態: (時止めナイフ)追加計画中
	SPELL_CARD_07_sakuya_ggg	第七形態: (分散魔方陣)追加計画中
//
	第七形態: 幻幽「ジャック・ザ・ルドビレ」になるかもしれないテスト
	<<<てすと>>>
	動きは標準非同期移動を使う。(r32)。
---------------------------------------------------------*/

/*---------------------------------------------------------
	SPELL_CARD_06_sakuya_fff	第六形態: (黄色葡萄弾)
	SPELL_CARD_05_sakuya_eee	第五形態: (黄色葡萄弾)
---------------------------------------------------------*/

/*---------------------------------------------------------
	第08形態: 魅魔 魔方陣生成(生成するだけ)
---------------------------------------------------------*/

extern void core_boss_move_05_xy_douki_differential64(SPRITE *src);
extern void core_boss_move_04_xy_douki_differential32(SPRITE *src);
global void sakuya_08_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if (0==seisei_count)
		{
		//	add_zako_sakuya_doll_03_star(src);/* 子供魔方陣追加 */
	//	/* ピンクハート */	add_zako_sakuya_doll_04_360knife(src);/* 子供魔方陣追加 */
			add_zako_sakuya_baramaki2(src);/* 子供魔方陣追加 */
		}
		seisei_count++;
		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8秒ぐらい] */
	//
	}
//
	core_boss_move_04_xy_douki_differential32(src);
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	第04形態: 魅魔 魔方陣生成(生成するだけ)
---------------------------------------------------------*/

global void sakuya_04_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if (0==seisei_count)
		{
		//	add_zako_sakuya_doll_03_star(src);/* 子供魔方陣追加 */
	//	/* ピンクハート */	add_zako_sakuya_doll_04_360knife(src);/* 子供魔方陣追加 */
			add_zako_sakuya_baramaki1(src);/* 子供魔方陣追加 */
		}
		seisei_count++;
		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8秒ぐらい] */
	}
//
	core_boss_move_04_xy_douki_differential32(src);
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	SPELL_CARD_03_sakuya_ccc
	時雨「バーティカルナイフ」
	第03形態: 垂直ナイフが落ちてくるよ
	通常攻撃3: 青赤ナイフ
---------------------------------------------------------*/

global void sakuya_03_keitai(SPRITE *src)
{
	core_boss_move_04_xy_douki_differential32(src);
//	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
	/* 反射弾のてすと */
//	bullet_angle_all_gamen_gai_nara_hansya();
	bullet_angle_all_gamen_gai_nara_hansya_gensoku();
}

/*---------------------------------------------------------
	SPELL_CARD_02_sakuya_bbb
	第02形態: 奇術「ミスディレクション」(全方位、豆まき)
---------------------------------------------------------*/

/*
		64		256-64		256-0
	//	真中へワープ
		128 				256-64
	// なにもしない
		192 				256-128
	// 反転移動方向セット
		256 				256-192
	// なにもしない
		0		256 		256-256
	//	反対側へワープ
-------------------------

256 	真中へワープ

250 	反対側へワープ
*/
global void sakuya_02_keitai(SPRITE *src)
{
	/* 弾幕を撃ってない場合に追加 */
	if (SPELL_00==spell_card.spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		spell_card_get_spell_number(src);	/* スペカシステムから、現在の形態で撃つ弾幕番号を手に入れ、標準のboss_base_spell_time_out時間を設定。 */
	//
	}
	if (/*(256-64)*/(255/*192+64*/) == src->boss_spell_timer) /* 反転移動方向セット */
	{
		{
			/* 第二形態では使って無いから利用(共用)する */
			#define sakuya_data_sayuu_mode sakuya_data_common_wait256
			{	/* 咲夜さんの誘導ポイント座標 */
				#define SAKUYA_POINT_X_MID			(t256(GAME_WIDTH*2/4))
				src->sakuya_data_sayuu_mode ^= (2-1);/* 反転 */
				if (/*0==*/src->sakuya_data_sayuu_mode & (2-1))
						{	src->target_x256 = SAKUYA_POINT_X_MID-(t256(128));	}//96==128*0.75
				else	{	src->target_x256 = SAKUYA_POINT_X_MID+(t256(128));	}//
			}
			src->toutatu_wariai256 = t256(1.00);
		}
	}
	//
	else
//	if (/*(256-256)*/(59)/*((251+64)-256)*/ == src->boss_spell_timer) /* 真中へワープ */
	if (/*(256-256)*/(59)/*((251+64)-256)*/ == src->boss_spell_timer) /* 真中へワープ */
	{
		src->target_x256 = src->cx256 = t256(GAME_WIDTH/2); /* 真中へワープ */
		src->toutatu_wariai256 = t256(1.00);
	}
	else
	if (/*(256-256)*/(58)/*((250+64)-256)*/ == src->boss_spell_timer) /* 真中へワープ */
//	if (/*(256-256)*/(0) == src->boss_spell_timer) /* 真中へワープ */
//	if (/*(256-192)*/(64) == src->boss_spell_timer) /* 真中へワープ */
//	if (/*(256-180)*/(76) == src->boss_spell_timer) /* 真中へワープ */
	{
		src->target_x256 = src->cx256 = t256(GAME_WIDTH)-src->cx256;			/* 反対側へワープ */
		src->toutatu_wariai256 = t256(1.00);
	}

	#if 1
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
	#endif
//
	/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
	if (RANK_NORMAL < (cg_game_difficulty))/* hard or lunatic */
	{
		#if 0
		/* 反射弾のてすと */
		/*難しすぎる*/bullet_angle_all_gamen_gai_nara_hansya();
		#else/* やってみたけど、ちょっと難しすぎるので急遽簡単にする */
		/*少し難しすぎる。あと一押し簡単にしたいな(r32時点)*/bullet_angle_all_gamen_gai_nara_hansya_gensoku();
		#endif
	}
	else
	{
		bullet_angle_all_gamen_gai_nara_kesu();
	}
}


/*---------------------------------------------------------
	氷符「アイシクルフォール」動かないでスペカ撃つ。
---------------------------------------------------------*/

global void cirno_02_keitai(SPRITE *src)
{
	if (SPELL_00==spell_card.spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		spell_card_get_spell_number(src);	/* スペカシステムから、現在の形態で撃つ弾幕番号を手に入れ、標準のboss_base_spell_time_out時間を設定。 */
		src->toutatu_wariai256 = t256(1.0);
	}
//
//	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
		#if 0
		/* 反射弾のてすと */
		/*難しすぎる*/bullet_angle_all_gamen_gai_nara_hansya();
		#else/* やってみたけど、ちょっと難しすぎるので急遽簡単にする */
		/*少し難しすぎる。あと一押し簡単にしたいな(r32時点)*/bullet_angle_all_gamen_gai_nara_hansya_gensoku();
		#endif
}

/*---------------------------------------------------------
	SPELL_CARD_01_sakuya_aaa
	第一形態: 通常攻撃(左右に動いて禊弾撃ち)
	通常攻撃(禊弾幕、原作とはあえて若干違う)
---------------------------------------------------------*/
/*
	boss_move.c へ
*/


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
#if 1
global void boss_init_sakuya(SPRITE *h)
{
	h->callback_loser				= common_boss_put_items;

//	h->sakuya_data_common_wait256					= t256(0.0);/*現在 sakuya_02_keitai()でビットを使い、sakuya_03_keitai()で初期化するからたぶん不要*/
//	h->sakuya_data_move_angle1024				= (0);/*現在sakuya_09_keitai()で初期化するから不要*/
	//-------------------------
	obj_send1->type 				= SP_DUMMY_MUTEKI;
//	send1_xy(src);	/* 弾源x256 y256 中心から発弾。 */
	obj_send1->cx256				= (0);
	obj_send1->cy256				= (0);
}
#endif

