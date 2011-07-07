
#include "boss.h"

/*static*/extern void add_zako_sakuya_doll_01_laser(SPRITE *src);/* レーザー */
/*static*/extern void add_zako_sakuya_doll_02_star(SPRITE *src);/* 子供魔方陣(星型を描く) */
/*static*/extern void add_zako_sakuya_doll_03_360knife(SPRITE *src);/* 子供魔方陣(360ナイフを撃つ) */
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
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define boss_time_out		user_data03 	/* 制限時間 */
	#define boss_base_state777	user_data03 	/* 制限時間(boss_time_outと同じ) */
#endif

//	int st ate1;							/* 行動 */
//	int move_type;		/*	移動タイプ */	/* 形態 */
//


#define sakuya_data_move_angle1024		user_data04 	/* 文の角度 */
#define sakuya_data_wait1				user_data05 	/* 文の角度 */
#define sakuya_data_wait2_256			user_data06 	/* 文の角度 */

//	int length_s_p256;	/* 咲夜とプレイヤーとの距離 */

//
/*---------------------------------------------------------
	咲夜背後の魔方陣についてメモ
	-------------------------------------------------------
	5面中-ボスで出てくる時は、大きさ60x60(たぶん64x64obj)ぐらい。
	5面ボスで出てくる時は、大きさ125x125(たぶん128x128obj)ぐらい。
	模倣風は基本的に縮小率75%なので、咲夜背後の魔方陣は
	48x48[dot](2倍拡大時は96x96[dot])ぐらいが妥当かな？
---------------------------------------------------------*/


/*--------------------------------------*/
/*--------------------------------------*/
/*--------------------------------------*/

#if 1
/*---------------------------------------------------------
	[スペカシステム内に移動予定]	スペカ撃破後アイテム出す
---------------------------------------------------------*/

static void sakuya_put_items(SPRITE *src)
{
	common_boss_put_items(src);
//
//別で設定済み	src->boss_base_state777 	= 0;/* 共通して 値を 0 にする */
//???	src->sakuya_data_wait1		= 80;/* 廃止 */
	src->sakuya_data_wait2_256	= t256(-100);

}
#endif



/*---------------------------------------------------------
	何がなんだか解からなくなるので、名前をつけよう
---------------------------------------------------------*/

#define SAKUYA_LIMIT_MIN_Y (t256(8.0))
#define SAKUYA_LIMIT_MAX_Y (t256(GAME_HEIGHT-32))

/* 咲夜さんの誘導ポイント座標 */

#define SAKUYA_POINT_X_LEFT_OUT 	(t256(GAME_WIDTH*0/4))
#define SAKUYA_POINT_X_LEFT_MID 	(t256(GAME_WIDTH*1/4))
#define SAKUYA_POINT_X_MID			(t256(GAME_WIDTH*2/4))/* -src->w/2 */
#define SAKUYA_POINT_X_RIGHT_MID	(t256(GAME_WIDTH*3/4))
#define SAKUYA_POINT_X_RIGHT_OUT	(t256(GAME_WIDTH*4/4))/* -src->w */
#define SAKUYA_POINT_X_MARGIN		(t256(16.0))

#define SAKUYA_POINT_Y_CENTER		(t256(50.0))




/*---------------------------------------------------------
	SPELL_CARD_11_sakuya_kkk	第11形態: 最終形態(その3)
---------------------------------------------------------*/

global void sakuya_11_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if (0==seisei_count)
		{
			add_zako_sakuya_doll_01_laser(src);/* 子供魔方陣追加 */
			/* hard or lunatic */
			if (1 < (cg_game_difficulty))
			{
				add_zako_sakuya_doll_02_star(src);/* 子供魔方陣追加 */
				/* lunatic only */
				if (2 < (cg_game_difficulty))
				{
					add_zako_sakuya_doll_03_360knife(src);/* 子供魔方陣追加 */
				}
			}
		}
		seisei_count++;
		seisei_count &= ((0x00ff) );	/* 256 == 0x100 == ( 4 x 64). [ 4秒ぐらい] */
	}
//
	src->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	src->alpha = 0xff;*/
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
	if (SPELL_00==src->boss_base_spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
//		alice_anime_count = 48;
		spell_card_get_spell_number(src);	/* スペカシステムから、現在の形態で撃つ弾幕番号を手に入れ、標準のboss_base_spell_time_out時間を設定。 */
		src->vvv256 = t256(1.0);
	}
//
	{
		static int seisei_count;
		if (0==seisei_count)
		{
			add_zako_sakuya_doll_02_star(src);/* 子供魔方陣追加 */
		//	add_zako_sakuya_doll_03_360knife(src);/* 子供魔方陣追加 */
		}
		seisei_count++;
		seisei_count &= 0x00ff; 	/* 256 == 0x100 == ( 4 x 64). [ 4秒ぐらい] */
	}
//
	if (src->sakuya_data_wait2_256 < t256(0.0) )
	{
		src->sakuya_data_wait2_256	= t256(0.0);
//???		src->sakuya_data_wait1		= 120;
		#if 0
		/* スペルカードシステムでは対応できない。 */
		sp ell_card_number = SPELL_CARD_09_sakuya_iii/*4*/;
		#endif
		#if 0/*???????????*/
		sp ell_card_number += (4);/* よくわかんない？？？*/
		#endif
//???		src->boss_base_state777 = SG02;
		//	[SAKUYA_ANIME_03_CENTER_A;
	}
	else
	{
		src->sakuya_data_wait2_256 -= t256(0.03);
		if (src->color32 > 0xf9000000)	/*	if (src->alpha > 0xf9) */	/* (src->alpha>0xff) */
		{
			src->color32 = 0xffffffff;	/*	src->alpha = 0xff;*/
		}
		else /*if (src->alpha<0xff)*/
		{
			src->color32 += 0x05000000; /*	src->alpha += 0x05;*/
		}
		src->cx256 += ((sin1024((src->tmp_angleCCW1024))*(src->sakuya_data_wait2_256))>>8);/*fps_factor*/		/* CCWの場合 */
		src->cy256 += ((cos1024((src->tmp_angleCCW1024))*(src->sakuya_data_wait2_256))>>8);/*fps_factor*/
	}
	//	[SAKUYA_ANIME_12_HATUDAN_START)
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------

---------------------------------------------------------*/
#if 0
static int sakuya_kyori(SPRITE *src)
{
	int rrr256;
	//	距離を求めるのに２乗してルートを取るのはルート演算が遅いので、工夫が必要かも。
	//	予めテーブル化しておくか、代替演算(CODIC)で求めるか、共通関数にしてCPUのfloat変換命令を最小に留めるか、どれか。
	//	src->length_s_p = sqrt((obj_player->x-src->x)*(obj_player->x-src->x)+(obj_player->y-src->y)*(obj_player->y-src->y));
	{	/* sqrt()遅いので簡易版。というか距離には拘らない事にした */
		int xxx256;
		int yyy256;
		xxx256 = abs((int)obj_player->cx256-(int)src->cx256);/*xxx256+=t256(10);*/ /* abs()は展開してくれるから大丈夫 */
		yyy256 = abs((int)obj_player->cy256-(int)src->cy256);/*yyy256+=t256(10);*/
		if (xxx256>yyy256)	{rrr256 = (xxx256/*+(yyy256>>1)*/);}/*少し難しくする*/
		else				{rrr256 = (yyy256/*+(xxx256>>1)*/);}/*少し難しくする*/
	}
	if (t256(40.0) > rrr256)	{	rrr256 = t256(40.0);}	/* 32 10 */
	else
	if (t256(128.0) < rrr256)	{	rrr256 = t256(128.0);}	/* 128 80 */
	return (rrr256);
}
#endif


/*---------------------------------------------------------
	SPELL_CARD_09_sakuya_iii	第九形態: 幻葬「フェスティバルナイフ」 最終形態(その1)
---------------------------------------------------------*/

global void sakuya_09_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if (0==seisei_count)
		{
		/*(テスト)*/	add_zako_sakuya_doll_02_star(src);/* 子供魔方陣追加 */
		//	add_zako_sakuya_doll_03_360knife(src);/* 子供魔方陣追加 */
		}
		seisei_count++;
		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8秒ぐらい] */
	}
//
	src->boss_base_state777++;

	if (80 > src->boss_base_state777)
	{
	//case SG00:
	//	src->sakuya_data_wait1--;/*fps_factor*/
	//	break;
	}
	else
	//	if (0 > src->sakuya_data_wait1) /* 待機時間が終わったら */
	if (81 > src->boss_base_state777)
	{
		//	src->boss_base_state777++;/* = SG01*/
//++		pd->bo ssmode = B00_NONE/*B01_BA TTLE*/;
			obj_send1->cx256	= (t256(GAME_WIDTH/2)); 		/* 弾源x256 */
			obj_send1->cy256	= (t256(30));					/* 弾源y256 */
			tmp_angleCCW1024_src_nerai(obj_send1, src);
			src->sakuya_data_move_angle1024 = src->tmp_angleCCW1024;
	}
	else
	if (1024 > src->boss_base_state777)
	{
	//case SG01:
		if ( (src->cx256 < t256(GAME_WIDTH/2+30) ) &&
			 (src->cx256 > t256(GAME_WIDTH/2-10) ) &&
			 (src->cy256 < t256(50.0)) ) /* 真ん中に来たら */
		{
			src->boss_base_state777 = (1024+1024-1)-(20);// src->boss_base_state777++;/* = SG02*/
		//	src->sakuya_data_wait1		= (20);
			src->sakuya_data_wait2_256	= t256(0.0);
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
	if ((1024+1024 ) > src->boss_base_state777)
	{
		;/* 何もしない(待機時間) */
//	case SG02:
//		{
//				src->sakuya_data_wait1--;/*fps_factor*/
//			if (0 > src->sakuya_data_wait1) 		{	src->boss_base_state777++;/* = SG03*/
//											//	[SAKUYA_ANIME_12_HATUDAN_START;
//			}		/* 待機時間が終わったら */
//		}
//		break;
	}
	else
	if ((1024+1024 +1) > src->boss_base_state777)
	{
//	case SG03:
		//	[SAKUYA_ANIME_15_HATUDAN_END)
		{
				bullet_create_sakuya_ryoute_knife(src);
		//	src->sakuya_data_wait1 = 40;
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
	if ((1024+1024 +1+40) > src->boss_base_state777)
	{
	//	else
		{
			/* CCWの場合 */
			src->cx256 += ((sin1024((src->sakuya_data_move_angle1024))*t256(3.5))>>8);/*fps_factor*/
			src->cy256 += ((cos1024((src->sakuya_data_move_angle1024))*t256(3.5))>>8);/*fps_factor*/
		}
	}
	else
	{
	//case SG04:
	//		src->sakuya_data_wait1--;/*fps_factor*/
	//	if (0 > src->sakuya_data_wait1)
		{
		//	src->sakuya_data_wait1	= (1024+1024-1)-(120)-((3-difficulty)*50);
		//	src->boss_base_state777 = SG02;
			src->boss_base_state777 = (1024+1024-1)-(120)-((3-(cg_game_difficulty))*50);
		}
	//	break;
	}
				SPRITE *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	#if 1/*Gu(中心座標)*/
	if ((zzz_player->cx256 < src->cx256 + t256(25)) &&	/* ???????????? */
		(zzz_player->cx256 > src->cx256 - t256(25)))
	#endif
	{	src->sakuya_data_wait2_256 += t256(1.0);/*fps_factor*/}
	if (src->sakuya_data_wait2_256 > t256(20.0)+((3-(cg_game_difficulty))*t256(30)))
	{
		//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
	//	src->length_s_p256 = sakuya_kyori(src);
		{
	//		src->cx256				= tmp1_256;/* 何故か咲夜移動？？？ */
	//		src->cy256				= tmp2_256;/* 何故か咲夜移動？？？ */
	//		src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);			/*	src->alpha			= 0x00;*/
			src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x80);			/*	src->alpha			= 0x00;*/	/* 半透明 */
	//		//	[SAKUYA_ANIME_15_HATUDAN_END;//?????
			src->tmp_angleCCW1024	= 0;//(ii<<7);
			src->sakuya_data_wait2_256			= /*speed256*/t256(2.0) + ((cg_game_difficulty)<<8);
			#if 0/*???*/
			/* スペルカードシステムでは対応できない。 */
			sp ell_card_number		= SPELL_CARD_10_sakuya_jjj;/*5*/ /*???*/
			#endif
		}
		bullet_crate_sakuya_kurukuru(src);/*,*/ /*1+difficulty,*/ /*(src->length_s_p256)*/
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

global void sakuya_06_keitai(SPRITE *src)
{
	if (0==src->boss_base_state777)
	{
//	{
//		static int seisei_count;
//		if (0==seisei_count)
		{
			add_zako_sakuya_doll_01_laser(src);/* レーザー追加 */
		}
//		seisei_count++;
//		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8秒ぐらい] */
//	}
		src->sakuya_data_wait1		= (3);	/* 3回撃ったら */
		src->sakuya_data_wait2_256	= t256(50.0);
		src->boss_base_state777 = 1;
	}
	else
	{
		src->sakuya_data_wait2_256 -= t256(1.0);/*fps_factor*/
		if (src->sakuya_data_wait2_256 < t256(0.0))
		{
			src->sakuya_data_wait2_256	= t256(30.0);
			/* (黄色葡萄弾) */
			#if 1
			tmp_angleCCW1024_jiki_nerai(src);/* 自機狙い角作成 */
			#endif
			/* CCWの場合 */
			bullet_crate_sakuya_linear_yuudou(src); 	/* 直線誘導ナイフ */
			//bullet_create_oodama0(src, t256(2), ANGLE_JIKINERAI_DAN, 40, 0, t256(src->w/2), t256(src->h/2));
			src->sakuya_data_wait1--;/*fps_factor*/
			if (0 > src->sakuya_data_wait1) 	/* 3回撃ったら */
			{
				src->sakuya_data_wait1 = (6-(cg_game_difficulty));	/* 5回撃ったら */	/*(20) 20回だと見ないうちに次の形態になってる */
				if (0==(ra_nd()&(2-1)/*%2*/))
						{	src->target_x256 = t256(100);				}
				else	{	src->target_x256 = t256(GAME_WIDTH-100);	}
				src->vvv256 = t256(1.00);
			}
		}
	}
	#if 1
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	#endif
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


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
		//	add_zako_sakuya_doll_02_star(src);/* 子供魔方陣追加 */
	//	/* ピンクハート */	add_zako_sakuya_doll_03_360knife(src);/* 子供魔方陣追加 */
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
		//	add_zako_sakuya_doll_02_star(src);/* 子供魔方陣追加 */
	//	/* ピンクハート */	add_zako_sakuya_doll_03_360knife(src);/* 子供魔方陣追加 */
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
	if (SPELL_00==src->boss_base_spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		spell_card_get_spell_number(src);	/* スペカシステムから、現在の形態で撃つ弾幕番号を手に入れ、標準のboss_base_spell_time_out時間を設定。 */
	//
	}
	if (/*(256-64)*/(255/*192+64*/) == src->boss_base_spell_time_out) /* 反転移動方向セット */
	{
		{
			/* 第二形態では使って無いから利用(共用)する */
			#define sakuya_data_sayuu_mode sakuya_data_wait2_256
			{
				src->sakuya_data_sayuu_mode ^= (2-1);/* 反転 */
				if (/*0==*/src->sakuya_data_sayuu_mode & (2-1))
						{	src->target_x256 = SAKUYA_POINT_X_MID-(t256(128));	}//96==128*0.75
				else	{	src->target_x256 = SAKUYA_POINT_X_MID+(t256(128));	}//
			}
			src->vvv256 = t256(1.00);
		}
	}
	//
	else
//	if (/*(256-256)*/(59)/*((251+64)-256)*/ == src->boss_base_spell_time_out) /* 真中へワープ */
	if (/*(256-256)*/(59)/*((251+64)-256)*/ == src->boss_base_spell_time_out) /* 真中へワープ */
	{
		src->target_x256 = src->cx256 = t256(GAME_WIDTH/2); /* 真中へワープ */
		src->vvv256 = t256(1.00);
	}
	else
	if (/*(256-256)*/(58)/*((250+64)-256)*/ == src->boss_base_spell_time_out) /* 真中へワープ */
//	if (/*(256-256)*/(0) == src->boss_base_spell_time_out) /* 真中へワープ */
//	if (/*(256-192)*/(64) == src->boss_base_spell_time_out) /* 真中へワープ */
//	if (/*(256-180)*/(76) == src->boss_base_spell_time_out) /* 真中へワープ */
	{
		src->target_x256 = src->cx256 = t256(GAME_WIDTH)-src->cx256;			/* 反対側へワープ */
		src->vvv256 = t256(1.00);
	}

	#if 1
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
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
	if (SPELL_00==src->boss_base_spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		spell_card_get_spell_number(src);	/* スペカシステムから、現在の形態で撃つ弾幕番号を手に入れ、標準のboss_base_spell_time_out時間を設定。 */
		src->vvv256 = t256(1.0);
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
	h->callback_loser				= sakuya_put_items;

//	h->sakuya_data_wait1						= (0);/*現在sakuya_02_keitai()で初期化するからたぶん不要*/
//	h->sakuya_data_wait2_256					= t256(0.0);/*現在 sakuya_02_keitai()でビットを使い、sakuya_03_keitai()で初期化するからたぶん不要*/
//	h->sakuya_data_move_angle1024				= (0);/*現在sakuya_09_keitai()で初期化するから不要*/
	//-------------------------
	obj_send1->type 				= SP_DUMMY_MUTEKI;
	obj_send1->cx256				= (0);
	obj_send1->cy256				= (0);
}
#endif

