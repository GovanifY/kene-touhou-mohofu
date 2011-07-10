
#include "boss.h"

/*static*/extern void add_zako_sakuya_doll_01_laser(SPRITE *src);/* レーザー */
 /* 珠を置く */
/*static*/extern void add_zako_sakuya_doll_04_360knife(SPRITE *src);/* 子供魔方陣(360ナイフを撃つ) */
/*static*/extern void add_zako_sakuya_baramaki1(SPRITE *src);/* 咲夜 ばら撒き1弾 オプション */
/*static*/extern void add_zako_sakuya_baramaki2(SPRITE *src);/* 咲夜 ばら撒き2弾 オプション */

/*static*/extern void bullet_create_sakuya_ryoute_knife(SPRITE *src);	/* 咲夜 両手ナイフカード */
/*static*/extern void bullet_crate_sakuya_linear_yuudou(SPRITE *src);	/* 咲夜 直線誘導ナイフカード */
/*static*/extern void bullet_crate_sakuya_kurukuru(SPRITE *src);		/* 咲夜 くるくるナイフカード */

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	十六夜 咲夜
	Izayoi Sakuya.
	-------------------------------------------------------
	ToDo:
	移動と弾の処理を分離しよう。でないと訳わからん。
	ボスタイマー対応中
	-------------------------------------------------------
	★ボスタイマーはシステムでサポート予定です。(作業中)
	-------------------------------------------------------
	GAME_X_OFFSET 対応。
---------------------------------------------------------*/

/*---------------------------------------------------------
	咲夜背後の魔方陣についてメモ
	-------------------------------------------------------
	5面中-ボスで出てくる時は、大きさ60x60(たぶん64x64obj)ぐらい。
	5面ボスで出てくる時は、大きさ125x125(たぶん128x128obj)ぐらい。
	模倣風は基本的に縮小率75%なので、咲夜背後の魔方陣は
	48x48[dot](2倍拡大時は96x96[dot])ぐらいが妥当かな？
---------------------------------------------------------*/

/*---------------------------------------------------------
	動かないでカードを撃つ。
---------------------------------------------------------*/

/*---------------------------------------------------------
	第11形態: 最終形態(その3)
---------------------------------------------------------*/

global void sakuya_11_keitai(SPRITE *src)
{
	if (SPELL_00==card.card_number) 	/* カード生成終了ならカード生成 */
	{
		card_maikai_init_and_get_spell_number(src); 	/* 現在の形態で撃てるカード番号をカードシステムに設定。 */
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);
	}
//
	int iii;
	iii = ra_nd();
	if (0==(iii&0x07))
	{
		src->vx256 = (((signed int)((iii>> 8)&0xff))-127);
		src->vy256 = (((signed int)((iii>>16)&0xff))-127);
	}
}


/*---------------------------------------------------------
	第10形態: 最終形態(その2)
---------------------------------------------------------*/

global void sakuya_10_keitai(SPRITE *src)
{
	if (SPELL_00==card.card_number) 	/* カード生成終了ならカード生成 */
	{
		card_maikai_init_and_get_spell_number(src); 	/* 現在の形態で撃てるカード番号をカードシステムに設定。 */
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);
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
		#if (0)//
		src->cx256 -= ((si n1024((src->tmp_angleCCW1024))*(t256(0.03)))>>8);/*fps_factor*/		/* CCWの場合 */
		src->cy256 -= ((co s1024((src->tmp_angleCCW1024))*(t256(0.03)))>>8);/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (src->tmp_angleCCW1024), &sin_value_t256, &cos_value_t256);
			src->cx256 -= ((sin_value_t256*(t256(0.03)))>>8);/*fps_factor*/
			src->cy256 -= ((cos_value_t256*(t256(0.03)))>>8);/*fps_factor*/
		}
		#endif
	}
	//	[SAKUYA_ANIME_12_HATUDAN_START)
}


/*---------------------------------------------------------
	第九形態: 幻葬「フェスティバルナイフ」 最終形態(その1)
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
	src->BOSS_DATA_05_boss_base_state777++;
	if (80 > src->BOSS_DATA_05_boss_base_state777) /* [A] 何もしない(待機時間) */
	{
		;/* 何もしない(待機時間) */
	}
	else
	if (81 > src->BOSS_DATA_05_boss_base_state777) /* [B] 待機時間が終わったら移動準備 */
	{
		//	src->BOSS_DATA_05_boss_base_state777++;/* = SG01*/
//++		pd->bo ssmode = B00_NONE/*B01_BA TTLE*/;
		//(r35でカードの場合のみ省略可能)	REG_02_DEST_X	= ((src->cx256));
		//(r35でカードの場合のみ省略可能)	REG_03_DEST_Y	= ((src->cy256));
		//(r35でカードの場合のみ省略可能)	set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
			REG_00_SRC_X	= (t256(GAME_X_OFFSET+(GAME_320_WIDTH/2))); 		/* 弾源x256 */
			REG_01_SRC_Y	= (t256(30));					/* 弾源y256 */
			tmp_angleCCW65536_src_nerai();
			src->BOSS_DATA_06_sakuya_data_move_angle1024 = (deg65536to1024(HATSUDAN_03_angle65536));	/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
	}
	else
	if (1024 > src->BOSS_DATA_05_boss_base_state777) /* [C] 真ん中に来るまで移動 */
	{
	//case SG01:
		if ( (src->cx256 < t256(GAME_X_OFFSET+(GAME_320_WIDTH/2)+20) ) &&
			 (src->cx256 > t256(GAME_X_OFFSET+(GAME_320_WIDTH/2)-20) ) &&
			 (src->cy256 < t256(50.0)) ) /* 真ん中に来たら */
		{
			src->BOSS_DATA_05_boss_base_state777 = (1024+1024-1)-(20);// src->BOSS_DATA_05_boss_base_state777++;/* = SG02*/
			src->BOSS_DATA_07_sakuya_data_common_wait256	= t256(0.0);
			//	[SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			#if (0)//
			src->cx256 += ((si n1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.0)))>>8);/*fps_factor*/	/* CCWの場合 */
			src->cx256 += ((si n1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.0)))>>8);/*fps_factor*/	/* CCWの場合 */
			src->cy256 += ((co s1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.0)))>>8);/*fps_factor*/
			src->cy256 += ((co s1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.0)))>>8);/*fps_factor*/
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (src->BOSS_DATA_06_sakuya_data_move_angle1024), &sin_value_t256, &cos_value_t256);
				src->cx256 += ((sin_value_t256*(t256(3.0)))>>8);/*fps_factor*/
				src->cy256 += ((cos_value_t256*(t256(3.0)))>>8);/*fps_factor*/
			}
			#endif
		}
	//	break;
	}
	else
	if ((1024+1024 ) > src->BOSS_DATA_05_boss_base_state777) /* [D] 何もしない(待機時間) */
	{
		;/* 何もしない(待機時間) */
	}
	else
	if ((1024+1024 +1) > src->BOSS_DATA_05_boss_base_state777) /* [E] 両手ナイフで攻撃 */
	{
//	case SG03:
		//	[SAKUYA_ANIME_15_HATUDAN_END)
		{
			bullet_create_sakuya_ryoute_knife(src);
			if (src->cy256 > t256(150))
		//	{	src->BOSS_DATA_06_sakuya_data_move_angle1024 = cv1024r(   210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;} 	/* CWの場合 */
		//	{	src->BOSS_DATA_06_sakuya_data_move_angle1024 = cv1024r(360-210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}	/* CCWの場合 */
			{	src->BOSS_DATA_06_sakuya_data_move_angle1024 = cv1024r(360-210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
			else
			{	src->BOSS_DATA_06_sakuya_data_move_angle1024 = ((ra_nd()&(1024-1)));}
			#if 1
			/*念の為*/
			mask1024(src->BOSS_DATA_06_sakuya_data_move_angle1024);
			#endif
			//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
		//	src->BOSS_DATA_05_boss_base_state777++;/* = SG04*/
		}
	//	break;
	}
	else
	if ((1024+1024 +1+40) > src->BOSS_DATA_05_boss_base_state777) /* [F]移動 */
	{
		#if (0)//
		src->cx256 += ((si n1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.5)))>>8);/*fps_factor*/	/* CCWの場合 */
		src->cy256 += ((co s1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.5)))>>8);/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (src->BOSS_DATA_06_sakuya_data_move_angle1024), &sin_value_t256, &cos_value_t256);
			src->cx256 += ((sin_value_t256*(t256(3.5)))>>8);/*fps_factor*/
			src->cy256 += ((cos_value_t256*(t256(3.5)))>>8);/*fps_factor*/
		}
		#endif
	}
	else /* [G] 待機時間作成し、[D]に戻る。 */
	{
	//case SG04:
		//	src->BOSS_DATA_05_boss_base_state777 = SG02;
		//	src->BOSS_DATA_05_boss_base_state777 = (1024+1024-1)-(120)-((3-(cg_game_di fficulty))*50);
		// difficulty(0) = (1927)-((3)*50) == 1777 == (271)c
		// difficulty(1) = (1927)-((2)*50) == 1827 == (221)c
		// difficulty(2) = (1927)-((1)*50) == 1877 == (171)c
		// difficulty(3) = (1927)-((0)*50) == 1927 == (121)c
		//	src->BOSS_DATA_05_boss_base_state777 = (1024+1024-1)-(120)-((3-(cg_game_di fficulty))*64);
		//	src->BOSS_DATA_05_boss_base_state777 = (1024+1024-1)-(120)-(3*64)+(cg_game_di fficulty*64);
		//	src->BOSS_DATA_05_boss_base_state777 = (1024+1024-1)-(120)-(3*64)+(cg_game_di fficulty<<6);
			src->BOSS_DATA_05_boss_base_state777 = (1024+1024-1)-(120)-(3*64)+(3<<6);
	//	break;
	}
				SPRITE *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	#if 1/*Gu(中心座標)*/
	if ((zzz_player->cx256 < src->cx256 + t256(25)) &&	/* ???????????? */
		(zzz_player->cx256 > src->cx256 - t256(25)))
	#endif
	{
	//	src->BOSS_DATA_07_sakuya_data_common_wait256 += (1+2+cg_game_di fficulty);/*fps_factor*/}
		src->BOSS_DATA_07_sakuya_data_common_wait256 += (1+2+3);/*fps_factor*/}
		// difficulty(0) = 20+((3)*(30)) == 110(r33)  42(r34)==(128/3)
		// difficulty(1) = 20+((2)*(30)) ==  80(r33)  32(r34)==(128/4)
		// difficulty(2) = 20+((1)*(30)) ==  50(r33)  25(r34)==(128/5)
		// difficulty(3) = 20+((0)*(30)) ==  20(r33)  21(r34)==(128/6)
//	if (src->BOSS_DATA_07_sakuya_data_common_wait256 > 20+((3-(cg_game_di fficulty))*(30)))
	if (src->BOSS_DATA_07_sakuya_data_common_wait256 > 128)
	{
		src->BOSS_DATA_07_sakuya_data_common_wait256		= 0;
	//	src->BOSS_DATA_07_sakuya_data_common_wait256		= 0;/*speed256*/(2) + ((cg_game_di fficulty));
		//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
		{
	//		src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);			/*	src->alpha			= 0x00;*/
			src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x80);			/*	src->alpha			= 0x00;*/	/* 半透明 */
	//		//	[SAKUYA_ANIME_15_HATUDAN_END;//?????
			src->tmp_angleCCW1024	= 0;//(ii<<7);
		}
		bullet_crate_sakuya_kurukuru(src);/*,*/ /*1+difficulty,*/
	}
}

/*---------------------------------------------------------
	第八形態: (時止めナイフ)追加計画中
	第七形態: (分散魔方陣)追加計画中
//
	第七形態: 幻幽「ジャック・ザ・ルドビレ」になるかもしれないテスト
	<<<てすと>>>
	動きは標準非同期移動を使う。(r32)。
---------------------------------------------------------*/

/*---------------------------------------------------------
	第六形態: (黄色葡萄弾)
	第五形態: (黄色葡萄弾)
---------------------------------------------------------*/
	//	/* ピンクハート */	add_zako_sakuya_doll_04_360knife(src);/* 子供魔方陣追加 */
	//	/* ピンクハート */	add_zako_sakuya_doll_04_360knife(src);/* 子供魔方陣追加 */

/*---------------------------------------------------------
	第08形態: 魅魔 魔方陣生成(生成するだけ)
---------------------------------------------------------*/

extern void core_boss_move_05_xy_douki_differential64(SPRITE *src);
extern void boss_move_04_xy_douki_differential32(SPRITE *src);
global void sakuya_08_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if (0==seisei_count)
		{
			add_zako_sakuya_baramaki2(src);/* 子供魔方陣追加 */
		}
		seisei_count++;
		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8秒ぐらい] */
	//
	}
//
	boss_move_04_xy_douki_differential32(src);
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
			add_zako_sakuya_baramaki1(src);/* 子供魔方陣追加 */
		}
		seisei_count++;
		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8秒ぐらい] */
	}
//
	boss_move_04_xy_douki_differential32(src);
}


/*---------------------------------------------------------
	時雨「バーティカルナイフ」
	第03形態: 垂直ナイフが落ちてくるよ
	通常攻撃3: 青赤ナイフ
---------------------------------------------------------*/
/*
	boss_move.c へ
*/


/*---------------------------------------------------------
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
	/* カードを撃ってない場合に追加 */
	if (SPELL_00==card.card_number) 	/* カード生成終了ならカード生成 */
	{
		card_maikai_init_and_get_spell_number(src); 	/* 現在の形態で撃てるカード番号をカードシステムに設定。 */
	//
	}
	if (/*(256-64)*/(255/*192+64*/) == REG_10_BOSS_SPELL_TIMER) /* 反転移動方向セット */
	{
		{
			/* 第二形態では使って無いから利用(共用)する */
			#define sakuya_data_sayuu_mode BOSS_DATA_07_sakuya_data_common_wait256
			{	/* 咲夜さんの誘導ポイント座標 */
				#define SAKUYA_POINT_X_MID			(t256(GAME_X_OFFSET+(GAME_320_WIDTH/2)))
				src->sakuya_data_sayuu_mode ^= (2-1);/* 反転 */
				if (/*0==*/src->sakuya_data_sayuu_mode & (2-1))
						{	src->BOSS_DATA_00_target_x256 = SAKUYA_POINT_X_MID-(t256(128)); }//96==128*0.75
				else	{	src->BOSS_DATA_00_target_x256 = SAKUYA_POINT_X_MID+(t256(128)); }//
			}
			src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
		}
	}
	//
	else
//	if (/*(256-256)*/(59)/*((251+64)-256)*/ == REG_10_BOSS_SPELL_TIMER) /* 真中へワープ */
	if (/*(256-256)*/(59)/*((251+64)-256)*/ == REG_10_BOSS_SPELL_TIMER) /* 真中へワープ */
	{
		src->BOSS_DATA_00_target_x256 = src->cx256 = t256(GAME_X_OFFSET+(GAME_320_WIDTH/2)); /* 真中へワープ */
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
	}
	else
	if (/*(256-256)*/(58)/*((250+64)-256)*/ == REG_10_BOSS_SPELL_TIMER) /* 真中へワープ */
//	if (/*(256-256)*/(0) == REG_10_BOSS_SPELL_TIMER) /* 真中へワープ */
//	if (/*(256-192)*/(64) == REG_10_BOSS_SPELL_TIMER) /* 真中へワープ */
//	if (/*(256-180)*/(76) == REG_10_BOSS_SPELL_TIMER) /* 真中へワープ */
	{
	//	src->BOSS_DATA_00_target_x256 = src->cx256 = t256(GAME_X_OFFSET) + ( t256(GAME_320_WIDTH) - ( (src->cx256) - t256(GAME_X_OFFSET) ) );	/* 反対側へワープ */
		src->BOSS_DATA_00_target_x256 = src->cx256 = t256(GAME_X_OFFSET) + t256(GAME_320_WIDTH) - (src->cx256) + t256(GAME_X_OFFSET);			/* 反対側へワープ */
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
	}

	#if 1
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
	#endif
}




/*---------------------------------------------------------
	第一形態: 通常攻撃(左右に動いて禊弾撃ち)
	通常攻撃(禊カード、原作とはあえて若干違う)
---------------------------------------------------------*/
/*
	boss_move.c へ
*/

