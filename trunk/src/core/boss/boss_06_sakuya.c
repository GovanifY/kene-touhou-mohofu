
#include "boss.h"

/*static*/extern void add_zako_sakuya_doll_01_laser(SPRITE *src);/* レーザー */
/*static*/extern void add_zako_sakuya_doll_02_star(SPRITE *src);/* 子供魔方陣(星型を描く) */
/*static*/extern void add_zako_sakuya_doll_03_360knife(SPRITE *src);/* 子供魔方陣(360ナイフを撃つ) */
/*static*/extern void add_zako_sakuya_baramaki(SPRITE *src);/* 咲夜 ばら撒き弾 オプション */

/*static*/extern void bullet_create_sakuya_ryoute_knife(SPRITE *src);

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
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


#define move_angle1024		user_data04 	/* 文の角度 */
#define wait1				user_data05 	/* 文の角度 */
#define wait2_256			user_data06 	/* 文の角度 */

//	int length_s_p256;	/* 咲夜とプレイヤーとの距離 */

//
/*---------------------------------------------------------

---------------------------------------------------------*/

enum
{
	SG00 = 0,
	SG01,
	SG02,
	SG03,
	SG04,
};

enum
{
	SE00 = 0,
	SE01,
	SE02,
};

enum
{
	SD00 = 0,
	SD01,
	SD02,
};
enum
{
	SC00 = 0,
	SC01,
	SC02,
	SC03,
	SC04,
	SC05,
	SC06,
	SC07,
};



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
	src->wait1		= 80;
	src->wait2_256	= t256(-100);

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
#define SAKUYA_POINT_X_MID			(t256(GAME_WIDTH*2/4))/*-src->w/2*/
#define SAKUYA_POINT_X_RIGHT_MID	(t256(GAME_WIDTH*3/4))
#define SAKUYA_POINT_X_RIGHT_OUT	(t256(GAME_WIDTH*4/4))/*-src->w  */
#define SAKUYA_POINT_X_MARGIN		(t256(16.0))

#define SAKUYA_POINT_Y_CENTER		(t256(50.0))




/*---------------------------------------------------------
	SPELL_CARD_11_sakuya_kkk	第10形態: 最終形態(その3)
---------------------------------------------------------*/

global void sakuya_11_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if(0==seisei_count)
		{
			add_zako_sakuya_doll_01_laser(src);/* 子供魔方陣追加 */
			/* hard or lunatic */
			if (1 < difficulty)
			{
				add_zako_sakuya_doll_02_star(src);/* 子供魔方陣追加 */
				/* lunatic only */
				if (2 < difficulty)
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
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* 弾幕生成終了なら弾幕生成 */
	{
//		alice_anime_count = 48;
		spell_card_get_danmaku_number(src); 	/* スペカシステムから、現在の形態で撃つ弾幕番号を手に入れ、標準のboss_base_danmaku_time_out時間を設定。 */
		src->vvv256 = t256(1.0);
	}
//
	{
		static int seisei_count;
		if(0==seisei_count)
		{
			add_zako_sakuya_doll_02_star(src);/* 子供魔方陣追加 */
		//	add_zako_sakuya_doll_03_360knife(src);/* 子供魔方陣追加 */
		}
		seisei_count++;
		seisei_count &= 0x00ff; 	/* 256 == 0x100 == ( 4 x 64). [ 4秒ぐらい] */
	}
//
	if (src->wait2_256 < t256(0.0) )
	{
		src->wait2_256	= t256(0.0);
		src->wait1		= 120;
		#if 0
		/* スペルカードシステムでは対応できない。 */
		sp ell_card_number = SPELL_CARD_09_sakuya_iii/*4*/;
		#endif
		#if 0/*???????????*/
		sp ell_card_number += (4);/*よくわかんない？？？*/
		#endif
		src->boss_base_state777 = SG02;
		//	[SAKUYA_ANIME_03_CENTER_A;
	}
	else
	{
		src->wait2_256 -= t256(0.03);
		if (src->color32 > 0xf9000000)	/*	if (src->alpha > 0xf9)	*/	/* (src->alpha>0xff)*/
		{
			src->color32 = 0xffffffff;	/*	src->alpha = 0xff;*/
		}
		else /*if (src->alpha<0xff)*/
		{
			src->color32 += 0x05000000; /*	src->alpha += 0x05;*/
		}
		src->cx256 += ((sin1024((src->tmp_angleCCW1024))*(src->wait2_256))>>8);/*fps_factor*/		/* CCWの場合 */
		src->cy256 += ((cos1024((src->tmp_angleCCW1024))*(src->wait2_256))>>8);/*fps_factor*/
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
#if 1
/* プレイヤーの周りに散らばり、時間が経つとプレイヤー狙い弾 */
static void enemy_boss06_knifes3(SPRITE *src)	/*,*/ /*dou ble speed,*/	/*int length222*/
{
	int len256;
	int tmp1_256;
	int tmp2_256;
	int tmp888;
	tmp888			= 0;
	/* ある程度遠くに出ないと認識しづらい */
	/* キャラ(コア)がでかいと避ける前に死ぬ。(ナイフが広がる前にあたる) */
	len256			= t256(/*70.0*/80.0) -((difficulty)<<8);	/*-length222*/	/*/2*/
	src->wait2_256	= t256(0.0);
				SPRITE *zzz_player;
				zzz_player = &obj00[FIX_OBJ_00_PLAYER];
	while (1)
	{
		tmp888		= (((int)ra_nd()+(t256_floor(src->wait2_256)) )&(8-1)/*%8*/);
		src->wait2_256	+= t256(1.0);
		len256			+= t256(0.1);
	/* CCWの場合 */
		#if 1/*Gu(中心座標)*/
		tmp1_256 = zzz_player->cx256 + ((sin1024((tmp888<<7))*len256)>>8);
		tmp2_256 = zzz_player->cy256 + ((cos1024((tmp888<<7))*len256)>>8);
		#endif
		if ((( t256(30) < tmp1_256) && (tmp1_256 < t256(GAME_WIDTH-30))) &&
			(( t256(30) < tmp2_256) && (tmp2_256 < t256(GAME_HEIGHT-72))))
		{	break;	}
	}
	int tmp_x256;
	int tmp_y256;
	#if 1/*Gu(中心座標)*/
	tmp_x256	= zzz_player->cx256;
	tmp_y256	= zzz_player->cy256;
	#endif
	int ii;
	for (ii=0; ii<8; ii++)
	{
		if (tmp888==ii)
		{
			src->cx256				= tmp1_256;
			src->cy256				= tmp2_256;
//			src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);			/*	src->alpha			= 0x00;*/
			src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x80);			/*	src->alpha			= 0x00;*/	/* 半透明 */
//			//	[SAKUYA_ANIME_15_HATUDAN_END;//?????
			src->tmp_angleCCW1024	= (ii<<7);
			src->wait2_256			= /*speed256*/t256(1.0)+(difficulty<<8);
			#if 0/*???*/
			/* スペルカードシステムでは対応できない。 */
			sp ell_card_number		= SPELL_CARD_10_sakuya_jjj;/*5*/ /*???*/
			#endif
		}
		else
		{
			/* プレイヤー狙い弾 */
			#if 1
			/* CCWの場合 */
			obj_send1->cx256 	= (src->cx256)	= tmp_x256 + ((sin1024((ii<<7))*len256)>>8);
			obj_send1->cy256 	= (src->cy256)	= tmp_y256 + ((cos1024((ii<<7))*len256)>>8);
			#endif
			br.BULLET_REGIST_speed256						= t256(1.0/*2.0*/); 				/* 速度 */
			br.BULLET_REGIST_angle1024						= (ii<<7);/*i*(1024)*1/8*/			/* 角度 */
//			br.BU LLET_REGIST_sakuya_kurukurku_knife_height = t256(32)-(ii&(2-1)/*%2*/)*64*256; 		/* -32 or 32 */
			br.BULLET_REGIST_regist_type					= REGIST_TYPE_04_KURU_KURU;
			bullet_regist_vector();
		}
	}
}
#endif


/*---------------------------------------------------------
	SPELL_CARD_09_sakuya_iii	第九形態: 最終形態(その1)
---------------------------------------------------------*/

global void sakuya_09_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if(0==seisei_count)
		{
		/*てすと*/	add_zako_sakuya_doll_02_star(src);/* 子供魔方陣追加 */
		//	add_zako_sakuya_doll_03_360knife(src);/* 子供魔方陣追加 */
		}
		seisei_count++;
		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8秒ぐらい] */
	}
//
	switch (src->boss_base_state777)
	{
	case SG00:
		src->wait1--;/*fps_factor*/
		if (0 > src->wait1) /* 待機時間が終わったら */
		{
			src->boss_base_state777++;/* = SG01*/
//++		pd->bo ssmode = B00_NONE/*B01_BA TTLE*/;
			obj_send1->cx256 	= (t256(GAME_WIDTH/2)); 		/* 弾源x256 */
			obj_send1->cy256 	= (t256(30));					/* 弾源y256 */
			tmp_angleCCW1024_src_nerai(obj_send1, src);
			src->move_angle1024 = src->tmp_angleCCW1024;
		}
		break;
	case SG01:
		if ( (src->cx256 < t256(GAME_WIDTH/2+30) ) &&
			 (src->cx256 > t256(GAME_WIDTH/2-10) ) &&
			 (src->cy256 < t256(50.0)) ) /* 真ん中に来たら */
		{
			src->boss_base_state777++;/* = SG02*/
			src->wait1		= (20);
			src->wait2_256	= t256(0.0);
			//	[SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			src->cx256 += ((sin1024((src->move_angle1024))*t256(3.0))>>8);/*fps_factor*/ 	/* CCWの場合 */
			src->cy256 += ((cos1024((src->move_angle1024))*t256(3.0))>>8);/*fps_factor*/
		}
		break;
	case SG02:
		{
				src->wait1--;/*fps_factor*/
			if (0 > src->wait1) 		{	src->boss_base_state777++;/* = SG03*/
											//	[SAKUYA_ANIME_12_HATUDAN_START;
			}		/* 待機時間が終わったら */
		}
		break;
	case SG03:
		//	[SAKUYA_ANIME_15_HATUDAN_END)
		{
			{
			//	int rrr256; 	//	/*src->length_s_p256*/rrr256 = sakuya_kyori(src);
		//		bullet_create_sakuya_even_knife(src, t256(10), /*src->length_s_p256*/ /*rrr256,*/ 0/*0==右*/);
		//		bullet_create_sakuya_even_knife(src, t256(10), /*src->length_s_p256*/ /*rrr256,*/ 1/*1==左*/);
				bullet_create_sakuya_ryoute_knife(src);
			}
			src->wait1=40;
			if (src->cy256 > t256(150))
		//	{	src->move_angle1024 = cv1024r(	  210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;} 	/* CWの場合 */
		//	{	src->move_angle1024 = cv1024r(360-210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;} 	/* CCWの場合 */
			{	src->move_angle1024 = cv1024r(360-210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
			else
			{	src->move_angle1024 = ((ra_nd()&(1024-1)));}
			#if 1
			/*念の為*/
			mask1024(src->move_angle1024);
			#endif
			//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
			src->boss_base_state777++;/* = SG04*/
		}
		break;
	case SG04:
			src->wait1--;/*fps_factor*/
		if (0 > src->wait1)
		{
			src->wait1	= 120+(3-difficulty)*50;
			src->boss_base_state777 = SG02;
		}
		else
		{
			/* CCWの場合 */
			src->cx256 += ((sin1024((src->move_angle1024))*t256(3.5))>>8);/*fps_factor*/
			src->cy256 += ((cos1024((src->move_angle1024))*t256(3.5))>>8);/*fps_factor*/
		}
		break;
	}
				SPRITE *zzz_player;
				zzz_player = &obj00[FIX_OBJ_00_PLAYER];
	#if 1/*Gu(中心座標)*/
	if ((zzz_player->cx256 < src->cx256 + t256(25)) &&  /* ???????????? */
		(zzz_player->cx256 > src->cx256 - t256(25)))
	#endif
	{	src->wait2_256 += t256(1.0);/*fps_factor*/}
	if (src->wait2_256 > t256(20.0)+((3-difficulty)*t256(30)))
	{
		//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
	//	src->length_s_p256 = sakuya_kyori(src);
		enemy_boss06_knifes3(src/*,*/ /*1+difficulty,*/ /*(src->length_s_p256)*/ );
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
/*static*/extern void bullet_crate_sakuya_linear_yuudou(SPRITE *src);	/* 直線誘導ナイフ */

global void sakuya_06_keitai(SPRITE *src)
{
	if (0==src->boss_base_state777)
	{
//	{
//		static int seisei_count;
//		if(0==seisei_count)
		{
			add_zako_sakuya_doll_01_laser(src);/* レーザー追加 */
		}
//		seisei_count++;
//		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8秒ぐらい] */
//	}
		src->wait1		= (3);	/* 3回撃ったら */
		src->wait2_256	= t256(50.0);
		src->boss_base_state777 = 1;
	}
	else
	{
		src->wait2_256 -= t256(1.0);/*fps_factor*/
		if (src->wait2_256 < t256(0.0))
		{
			src->wait2_256	= t256(30.0);
			/* (黄色葡萄弾) */
			#if 1
			tmp_angleCCW1024_jiki_nerai(src);/* 自機ねらい角作成 */
			#endif
			/* CCWの場合 */
			bullet_crate_sakuya_linear_yuudou(src); 	/* 直線誘導ナイフ */
			//bullet_create_oodama0(src, t256(2), ANGLE_JIKINERAI_DAN, 40, 0, t256(src->w/2), t256(src->h/2));
			src->wait1--;/*fps_factor*/
			if (0 > src->wait1) 	/* 3回撃ったら */
			{
				src->wait1 = (6-difficulty);	/* 5回撃ったら */	/*(20) 20回だと見ないうちに次の形態になってる */
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
	第四形態: 魅魔 魔方陣生成(生成するだけ)
---------------------------------------------------------*/

global void sakuya_04_keitai(SPRITE *src)
{
	{
		static int seisei_count;
		if(0==seisei_count)
		{
		//	add_zako_sakuya_doll_02_star(src);/* 子供魔方陣追加 */
			add_zako_sakuya_doll_03_360knife(src);/* 子供魔方陣追加 */
		}
		seisei_count++;
		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8秒ぐらい] */
	}
//
	switch (src->boss_base_state777)
	{
	case SD00:
		add_zako_sakuya_baramaki(src);/* 子供魔方陣追加 */
		//
		src->boss_base_state777++;
		src->target_x256 = SAKUYA_POINT_X_MID;/*SAKUYA_POINT_00_LEFT*/
		src->target_y256 = SAKUYA_POINT_Y_CENTER;/*SAKUYA_POINT_00_LEFT*/
		src->vvv256 = t256(1.00);
		break;
#if 0/* スペルカードシステムでは対応できない。 */
	case SD01:
		if (/*150*/200 > src->vvv256 )	/* 時間で */	/* 真ん中に来たら */
		{
			src->boss_base_state777++;/* = SE01*/ /*dummy*/
		//	src->boss_base_state777 	=	0/* = SE01*/;
		//	src->wait1		= (10);
//			src->wait1		= (5);/*???*/
		//	src->wait2_256	= t256(0.0);
//			src->wait2_256	= t256(10.0);/*???*/
				//	[SAKUYA_ANIME_08_HIKARU_START;
		//	[SAKUYA_ANIME_11_HIKARU_END)
			/* 子供魔方陣生成(2回生成しちゃうバグあり) */
			#if 0/*???*/
			/* スペルカードシステムでは対応できない。 */
			sp ell_card_number += (4);		/* 次の形態へ */
			#endif
			//	[SAKUYA_ANIME_11_HIKARU_END)
		}
		break;
#endif
	}
	#if 1
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	#endif
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	SPELL_CARD_03_sakuya_ccc
	時雨「バーティカルナイフ」
	第三形態: 垂直ナイフが落ちてくるよ
---------------------------------------------------------*/
static void sakuya_shot_111(SPRITE *src)
{
	{
	//	const short sss[(4)] =
	//	{
	//	t256(2*1.0),	//	t256(2*0.48),	/*	easy:まばらなので簡単*/ 						/* 1.2 */
	//	t256(2*1.0),	//	t256(2*0.75),	/*normal:まばらなので簡単*/ 						//	t256(0.48/*0.5*/ /*1.0*/ /*1.5*/),/*normal:塊なので大きく避ければ簡単*/
	//	t256(2*18.5),	//	t256(2*1.0),	/*	hard:たまに難しい時がある*/ 					/*2.0*/
	//	t256(2*18.5)	//	t256(2*18.5)	/*	luna:うへぇこのスキマくぐるんですかぁ？*/		/*6.5*/ /*12.5*/ /*8.5*/
	//		/*(r32)luna:コツが要るけどすごく簡単になった、その他(hard以下)は論外に簡単 */
	//		/*(r31)(luna:)ボムった方が早いけど、私的には(事故率10%以下で)気合避け可能。でも確率5-10%(10数回に一回)ぐらいは死ぬ。安定しないな。 */
	//		/*(-r30)(luna:ボムらなくても気合避けでなんとかなる程度に難易度抑えたいのだがまだ高すぎるな)*/
	//	};
		src->wait2_256 += t256(2*18.5);//	/*t256(4)*/sss[difficulty];/*t256(3*1.42)*/
	}
	if (0==(((int)src->wait2_256)&(32-1-1)/*%21 t256(15*1.42)*/))/* 0x1e == (32-1-1) */
	{
		obj_send1->cx256 					= (src->cx256);
		obj_send1->cy256 					= (src->cy256);
		br.BULLET_REGIST_speed256			= t256(1.75);/*(2.0)*/ /*(0.7)*/
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_05_AKA;
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_02_GRAVITY02;
		#if 0
		/* CWの場合 */
		br.BULLET_REGIST_angle1024	= ( cv1024r(360)-((t256_floor(src->wait2_256))&((1024/2)-1)/*%(180*1.42)*/));	br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.03);	bullet_regist_vector();
		br.BULLET_REGIST_angle1024	= ( cv1024r(270)-((t256_floor(src->wait2_256))&((1024/2)-1)/*%(180*1.42)*/));	br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.04);	bullet_regist_vector();
		br.BULLET_REGIST_angle1024	= ( cv1024r(180)-((t256_floor(src->wait2_256))&((1024/2)-1)/*%(180*1.42)*/));	br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.02);	bullet_regist_vector();
		#else
		/* CCWの場合 */
		br.BULLET_REGIST_angle1024	= (-cv1024r(360)-((t256_floor(src->wait2_256))&((1024/2)-1)/*%(180*1.42)*/));	br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.03);	bullet_regist_vector();
		br.BULLET_REGIST_angle1024	= (-cv1024r(270)-((t256_floor(src->wait2_256))&((1024/2)-1)/*%(180*1.42)*/));	br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.04);	bullet_regist_vector();
		br.BULLET_REGIST_angle1024	= (-cv1024r(180)-((t256_floor(src->wait2_256))&((1024/2)-1)/*%(180*1.42)*/));	br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.02);	bullet_regist_vector();
		#endif
		voice_play(VOICE12_SAKUYA_B2_SHOT, TRACK04_TEKIDAN);
	}
}
static void sakuya_shot_222(SPRITE *src)
{
	src->wait2_256 += t256(4.0);/*(3*1.42)*/
	if (0x0000==((src->wait2_256)&(0x0f00)))/*%(12*1.42)*/
	{
		obj_send1->cx256 					= (src->cx256);
		obj_send1->cy256 					= (src->cy256);
		br.BULLET_REGIST_speed256			= t256(3.00);
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_05_AKA;
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_02_GRAVITY02;
		#if 0
		/* CWの場合 */
				br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)+cv1024r(180)); 		br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.03);	bullet_regist_vector();
		if (difficulty)
		{
				br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)+cv1024r(90));			br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.04);	bullet_regist_vector();
			if (1<difficulty)
			{	br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/));						br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.02);	bullet_regist_vector();}
		}
		#else
		/* CCWの場合 */
				br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)+cv1024r(360-180)); 	br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.03);	bullet_regist_vector();
		if (difficulty)
		{
				br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)+cv1024r(360-90));		br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.04);	bullet_regist_vector();
			if (1<difficulty)
			{	br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/));						br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.02);	bullet_regist_vector();}
		}
		#endif
		voice_play(VOICE12_SAKUYA_B2_SHOT, TRACK04_TEKIDAN);
	}
}
static void sakuya_shot_333(SPRITE *src)
{
//	src->move_angle1024 = at an_1024_SAKUYA_MID;
//	if (0 == ((t256_floor(src->wait2_256))&(32-1)/*%20*/) )
	if (0 == (( (src->vvv256))&(32-1)/*%20*/) )
	{
//		int b_wait2_high=(((int)(( (src->vvv256))>>5/*/20*/)));
		{
//			obj_send1->cx256 	=	(src->cx256);
//			obj_send1->cy256 	=	(src->cy256);
			br.BULLET_REGIST_regist_type					= REGIST_TYPE_04_KURU_KURU;
			/* CCWの場合 */
			unsigned int ii;
		//	for (ii=0; ii<((4+difficulty)<<3); ii++)
		//	for (ii=0; ii<(32<<difficulty); ii++)
			for (ii=0; ii<(32*8); ii++)
			{
				const u32 ra_nd32 = ra_nd();
			//	obj_send1->cx256 	= (src->cx256)+((ra_nd32   )&0xffff)-t256(128);/*とりあえず代替(br.BULLET_REGIST_angle512)*/
				obj_send1->cx256 	= (src->cx256)+((((ra_nd32	)&0xffff)-t256(128))>>(3-difficulty));/*とりあえず代替(br.BULLET_REGIST_angle512)*/
				obj_send1->cy256 	= (src->cy256)+((ra_nd32<<8)&0x3fff)-t256(32);/*とりあえず代替(br.BULLET_REGIST_angle512)*/
//				br.BULLET_REGIST_angle512	= cv512r(/*360+*/90-30)+(b_wait2_high)*cv512r(20);/*廃止*/
//				br.BULLET_REGIST_angle512	= cv512r(/*360+*/90-30)+(b_wait2_high)*cv512r(40);/*廃止*/
//				br.BULLET_REGIST_angle512	= cv512r(/*360+*/90-30)+(b_wait2_high)*cv512r(30);/*廃止*/
//
			//	br.BULLET_REGIST_speed256	= (b_wait2_high<<7/*8*/)+t256(2.0)-(ii<<5);//t256(2.0/*4.0*/) t256(1.5/*3.0*/) t256(1.0/*2.0*/)
				br.BULLET_REGIST_speed256	= t256(2.0)-(ii);//t256(2.0/*4.0*/) t256(1.5/*3.0*/) t256(1.0/*2.0*/)
//				br.BU LLET_REGIST_sakuya_kurukurku_knife_height = t256(32)+(ii<<4);//t256(40) t256(60) t256(80);
				bullet_regist_vector();
			}
		}
	}
}
			#if 0
			/* CWの場合 */
				br.BULLET_REGIST_angle512 = (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(40)));
				br.BULLET_REGIST_angle512 = (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(30)));
			if (1<difficulty)
			{
				br.BULLET_REGIST_angle512 = (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(20)));
			}
			#endif
global void sakuya_03_keitai(SPRITE *src)
{
	switch (src->boss_base_state777)
	{
	case SC00:
	//	if (src->cx256 < (SAKUYA_LIMIT_X_LEFT)/*5*/ )	/* 左隅に来たら */	/* → || (src->y<5)があると無限ループになる*/
		{
			src->boss_base_state777++;
			src->target_x256 = SAKUYA_POINT_X_LEFT_OUT;/*SAKUYA_POINT_00_LEFT*/
			src->vvv256 = t256(1.00);
		}
		break;
	case SC01:
		if (/*150*/200 > src->vvv256 )	/* 時間で */	/* 左隅に来たら */
		{
			src->boss_base_state777++;
			src->target_x256 = SAKUYA_POINT_X_RIGHT_OUT;/*SAKUYA_POINT_03_RIGHT*/
			src->vvv256 = t256(1.00);
			src->wait2_256	= t256(0.0);/*(sakuya_shot_111用)弾幕基準時間*/
		}
		break;
	case SC02:
		if (/*150*/200 > src->vvv256 )	/* 時間で */	/* 右端に来たら */
		{
			src->boss_base_state777++;
			src->target_x256 = SAKUYA_POINT_X_MID;/*SAKUYA_POINT_01_MID_UP*/
			src->vvv256 = t256(1.00);
		}
		else
		{
			sakuya_shot_111(src);
		}
		break;
	case SC03:
		if (/*150*/200 > src->vvv256 )	/* 時間で */	/* 真ん中に来たら */
		{
			src->boss_base_state777++;
			src->target_x256 = SAKUYA_POINT_X_RIGHT_OUT;/*SAKUYA_POINT_03_RIGHT*/
			src->vvv256 = t256(1.00);
		}
		break;
	case SC04:
		if (/*150*/200 > src->vvv256 )	/* 時間で */	/* 右端に来たら */
		{
			src->boss_base_state777++;
			src->target_x256 = SAKUYA_POINT_X_LEFT_OUT;/*SAKUYA_POINT_00_LEFT*/
			src->vvv256 = t256(1.00);
	//		src->wait1		= (10);/*???廃止*/
			src->wait2_256	= t256(0.0);/*(sakuya_shot_222用)弾幕基準時間*/
		}
		break;
	case SC05:
		if (/*150*/200 > src->vvv256 )	/* 時間で */	/* 左端に来たら */
		{
			src->boss_base_state777++;
			src->target_x256 = SAKUYA_POINT_X_MID;/*SAKUYA_POINT_01_MID_UP*/
			src->vvv256 = t256(1.00);
		}
		else
		{
			sakuya_shot_222(src);
		}
		break;
	case SC06:
		if (/*150*/200 > src->vvv256 )	/* 時間で */	/* 真ん中に来たら */
		{
			src->boss_base_state777++;
			src->vvv256 = t256(1.00);
//			src->wait2_256	= t256(60.0);
		}
		break;
	case SC07:
//		src->wait2_256 -= t256(1.0);
//		if (src->wait2_256 < t256(0.0))
		if (/*150*/200 > src->vvv256 )	/* 時間で */
		{
		//	src->boss_base_state777++/* = SC00*/;
			src->boss_base_state777 = SC00;
		}
		else
		{
			sakuya_shot_333(src);
		}
		break;
	}
	#if 1
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	#endif
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	SPELL_CARD_02_sakuya_bbb
	第二形態: 奇術「ミスディレクション」(全方位、豆まき)
---------------------------------------------------------*/

/* 第一、第二形態では使って無いみたいだから利用する */
#define sayuu_mode wait2_256
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
*/
global void sakuya_02_keitai(SPRITE *src)
{
//	src->boss_base_state777++;
//	src->boss_base_state777 &= (256-1);
	/* 弾幕を撃ってない場合に追加 */
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		spell_card_get_danmaku_number(src); 	/* スペカシステムから、現在の形態で撃つ弾幕番号を手に入れ、標準のboss_base_danmaku_time_out時間を設定。 */
	//
		src->target_x256 = src->cx256 = t256(GAME_WIDTH/2); /* 真中へワープ */
		src->vvv256 = t256(1.00);
	}
	if ((256-64) == src->boss_base_danmaku_time_out) /* 反転移動方向セット */
	{
		{
			{
				src->sayuu_mode ^= (2-1);/* 反転 */
				if (/*0==*/src->sayuu_mode & (2-1))
						{	src->target_x256 = SAKUYA_POINT_X_MID-(t256(128));	}//96==128*0.75
				else	{	src->target_x256 = SAKUYA_POINT_X_MID+(t256(128));	}//
			}
			src->vvv256 = t256(1.00);
		}
		/* 24弾幕をセット */
		//	enemy_boss06_mamemaki_state(src, SB05, SAKUYA_ANIME_15_HATUDAN_END);
//		src->boss_base_danmaku_type 	= DANMAKU_02_sakuya;	/* 24弾幕をセット */
//		danmaku_set_time_out(src);		/* 弾幕の時間切れを設定 */	//	#define DANMAKU_0048_TIME	(48+1)
	}
	else
	if ((256-192) == src->boss_base_danmaku_time_out) /* 真中へワープ */
//	if ((256-180) == src->boss_base_danmaku_time_out) /* 真中へワープ */
	{
		src->target_x256 = src->cx256 = t256(GAME_WIDTH)-src->cx256;			/* 反対側へワープ */
		src->vvv256 = t256(1.0);
	}

	#if 1
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	#endif
	/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
	if (RANK_NORMAL < difficulty)/* hard or lunatic */
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
//boss_base_danmaku_time_out
//
//	if (58/*264-206*/ > src->boss_base_state777)
//	if (58/*264-206*/ == src->boss_base_state777)
//	if (0 == src->boss_base_state777)
//	{
//		/* 11弾幕をセット */
//	//	src->boss_base_danmaku_type 	= DANMAKU_03_sakuya;		/* 11弾幕をセット */
//		src->boss_base_danmaku_type 	= DANMAKU_1c_sakuya;		/* 奇術「ミスディレクション」をセット */
//		danmaku_set_time_out(src);		/* 弾幕の時間切れを設定 */	//#define DANMAKU_0064_TIME (64+1)
//	}
//	else
//	if (64 == src->boss_base_state777) /* 真中へワープ */
//	if (207-206 > src->boss_base_state777)	/* 真中へワープ、移動方向セット */
//	if (1 == src->boss_base_state777)		/* 真中へワープ、移動方向セット */
//	if (192 == src->boss_base_state777) 	/* 真中へワープ、移動方向セット */


/*---------------------------------------------------------
	SPELL_CARD_01_sakuya_aaa
	第一形態: 通常攻撃(左右に動いて禊弾撃ち)
	通常攻撃(禊弾幕、原作とはあえて若干違う)
---------------------------------------------------------*/


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
#if 1
global void boss_init_sakuya(SPRITE *h)
{
	h->callback_loser				= sakuya_put_items;

//	h->wait1						= (0);/*現在sakuya_02_keitai()で初期化するからたぶん不要*/
//	h->wait2_256					= t256(0.0);/*現在 sakuya_02_keitai()でビットを使い、sakuya_03_keitai()で初期化するからたぶん不要*/
//	h->move_angle1024				= (0);/*現在sakuya_09_keitai()で初期化するから不要*/
	//-------------------------
	obj_send1->type 				= SP_DUMMY_MUTEKI;
	obj_send1->cx256 				= (0);
	obj_send1->cy256 				= (0);
}
#endif
//	h->callback_mover				= move_alice;

//	obj_send1						= sprite_add_res(BASE_BOSS_SAKUYA_PNG);
//	obj_send1->flags				&= (~(SP_FLAG_VISIBLE));
//	obj_send1->callback_mover		= dummy_move;
