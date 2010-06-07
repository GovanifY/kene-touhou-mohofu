
#include "boss.h"

/*static*/extern void add_zako_sakuya_doll_01_laser(SPRITE *src);/* レーザー */
/*static*/extern void add_zako_sakuya_doll_02_star(SPRITE *src);/* 子供魔方陣(星型を描く) */
/*static*/extern void add_zako_sakuya_doll_03_360knife(SPRITE *src);/* 子供魔方陣(360ナイフを撃つ) */

/*static*/extern void add_zako_sakuya_baramaki(SPRITE *src);/* 咲夜 ばら撒き弾 オプション */

/*static*/extern void bullet_create_sakuya_ryoute_knife(SPRITE *src);

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
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
#if 0/* めも：さくやは、共通部分で使ってる。 */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256		 		user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif
//	int st ate1;							/* 行動 */
//	int move_type;		/*	移動タイプ */	/* 形態 */
//

#define move_angle1024		user_data04 	/* 文の角度 */
#define wait1				user_data05 	/* 文の角度 */
#define wait2_256			user_data06 	/* 文の角度 */
//#define wa it3			user_data07 	/* 文の角度 */


#define move_vx256			move_angle1024


//	int length_s_p256;	/* 咲夜とプレイヤーとの距離 */

//
/*---------------------------------------------------------

---------------------------------------------------------*/
/*
	SAKUYA_ANIME_xx_dummy
	は中抜きパターンなので、
	プログラムで指定しない。
	(指定すると意味が判らなくなるから指定しない。)
*/
//enum
//{
//	SAKUYA_ANIME_00_LEFT = 0,
//	SAKUYA_ANIME_01_dummy,
//	SAKUYA_ANIME_02_dummy,							//	SAKUYA_ANIME_03_,
//	SAKUYA_ANIME_03_CENTER_A,
//
//	SAKUYA_ANIME_04_CENTER_B,
//	SAKUYA_ANIME_05_dummy,
//	SAKUYA_ANIME_06_dummy,							//	SAKUYA_ANIME_08_,
//	SAKUYA_ANIME_07_RIGHT,
//
//	SAKUYA_ANIME_08_HIKARU_START,	/*= 0x10*/		//	SAKUYA_ANIME_10_,/*0x11*/
//	SAKUYA_ANIME_09_dummy,			/*0x12*/		//	SAKUYA_ANIME_12_,/*0x13*/
//	SAKUYA_ANIME_10_dummy,			/*0x14*/
//	SAKUYA_ANIME_11_HIKARU_END, 	/*0x15*/
//
//	SAKUYA_ANIME_12_HATUDAN_START,	/*0x16*/
//	SAKUYA_ANIME_13_dummy,			/*0x17*/
//	SAKUYA_ANIME_14_dummy,			/*0x18*/
//	SAKUYA_ANIME_15_HATUDAN_END,	/*0x19*/
//};

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
	SE00_ss = 0,
	SE00,
	SE01,
	SE02,
};


enum
{
	SD00 = 0,
	SD01,
	SD02,
	SD03,
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
	SC08,
	SC09,
};
enum
{
	SB00 = 0,
	SB01,
	SB02,
	SB03,
	SB04,
	SB05,
	SB06,
	SB07,
	SB08,
	SB09,
};
enum
{
	SA00 = 0,
	SA01,
	SA02,
	SA03,
	SA04,
	SA05,
	SA06,
	SA07,
	SA08,
	SA09,
};

/*--------------------------------------*/
/*--------------------------------------*/
/*--------------------------------------*/


/*---------------------------------------------------------
	[スペカシステム内に移動予定]	スペカ撃破後アイテム出す
---------------------------------------------------------*/

static void sakuya_put_items(SPRITE *src)
{
	common_boss_put_items(src);
//
	src->boss_base_state001 	= 0;/* 共通して 値を 0 にする */
	src->wait1		= 80;
	src->wait2_256	= t256(-100);
//	src->wa it3 	= (0);//???

}


/*---------------------------------------------------------

---------------------------------------------------------*/

static void sakuya_wait_state(SPRITE *src)
{
	src->wait1--;/*fps_factor*/
	if (0 > src->wait1)
	{
		src->boss_base_state001++/* = nextstate*/;
	}
}



/*---------------------------------------------------------

---------------------------------------------------------*/

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
	while (1)
	{
		tmp888		= (((int)ra_nd()+(t256_floor(src->wait2_256)) )&(8-1)/*%8*/);
		src->wait2_256	+= t256(1.0);
		len256			+= t256(0.1);
	/* CCWの場合 */
		#if 0/*SDL(左隅座標)*/
		tmp1_256 = obj_player->x256 + ((obj_player->w128-src->w128)) + ((sin1024((tmp888<<7))*len256)>>8);
		tmp2_256 = obj_player->y256 + ((obj_player->h128-src->h128)) + ((cos1024((tmp888<<7))*len256)>>8);
		#else/*Gu(中心座標)*/
		tmp1_256 = obj_player->x256 + ((sin1024((tmp888<<7))*len256)>>8);
		tmp2_256 = obj_player->y256 + ((cos1024((tmp888<<7))*len256)>>8);
		#endif
		if ((( t256(30) < tmp1_256) && (tmp1_256 < t256(GAME_WIDTH-30))) &&
			(( t256(30) < tmp2_256) && (tmp2_256 < t256(GAME_HEIGHT-72))))
		{	break;	}
	}
	int tmp_x256;
	int tmp_y256;
	#if 0/*SDL(左隅座標)*/
	tmp_x256	= obj_player->x256 + ((obj_player->w128));
	tmp_y256	= obj_player->y256 + ((obj_player->h128));
	#else/*Gu(中心座標)*/
	tmp_x256	= obj_player->x256;
	tmp_y256	= obj_player->y256;
	#endif
	int ii;
	for (ii=0; ii<8; ii++)
	{
		if (tmp888==ii)
		{
			src->tmp_angleCCW1024	= (ii<<7);
			src->x256				= tmp1_256;
			src->y256				= tmp2_256;
//			src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);			/*	src->alpha			= 0x00;*/
			src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x80);			/*	src->alpha			= 0x00;*/	/* 半透明 */
//			//	[SAKUYA_ANIME_15_HATUDAN_END;//?????
			#if 0/*???*/
			/* スペルカードシステムでは対応できない。 */
			sp ell_card_number		= SPELL_CARD_10_sakuya_jjj/*5*/;/*???*/
			#endif
			src->wait2_256			= /*speed256*/t256(1.0)+(difficulty<<8);
		}
		else
		{
		//	dou ble tmp_s_x;
		//	dou ble tmp_s_y;
		//	tmp_s_x = s->x;
		//	tmp_s_y = s->y;
			/* プレイヤー狙い弾 */
			#if 1
			/* CCWの場合 */
			obj_send1->x256 	= (src->x256)	= tmp_x256 + ((sin1024((ii<<7))*len256)>>8);
			obj_send1->y256 	= (src->y256)	= tmp_y256 + ((cos1024((ii<<7))*len256)>>8);
		//	obj_send1->w128 	= src->w128;/*弾*/
		//	obj_send1->h128 	= src->h128;/*弾*/
			#endif
		//	/*src,*/
			br.BULLET_REGIST_speed256						= t256(1.0/*2.0*/); 				/* 速度 */
			br.BULLET_REGIST_angle1024						= (ii<<7)/*i*(1024)*1/8*/;			/* 角度 */
			br.BULLET_REGIST_sakuya_kurukurku_knife_height	= -30+(ii&(2-1)/*%2*/)*60;			/* -30 or 30 */
			br.BULLET_REGIST_regist_type					= REGIST_TYPE_04_KURU_KURU;
			bullet_regist_vector();

		//	s->x = tmp_s_x;
		//	s->y = tmp_s_y;
		}
	}
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

#define SAKUYA_POINT_Y_LEFT 		(t256(50.0))
#define SAKUYA_POINT_Y_MID			(t256(10.0))
#define SAKUYA_POINT_Y_RIGHT		(t256(50.0))
#define SAKUYA_POINT_Y_CENTER		(t256(50.0))
#define SAKUYA_POINT_Y_MARGIN		(t256(16.0))

#define SAKUYA_LIMIT_X_LEFT 		(SAKUYA_POINT_X_LEFT_OUT  + SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_LEFT 	(SAKUYA_POINT_X_MID 	  + SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_RIGHT	(SAKUYA_POINT_X_MID 	  - SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_RIGHT		(SAKUYA_POINT_X_RIGHT_OUT - SAKUYA_POINT_X_MARGIN)

#define atan_1024_SAKUYA_LEFT		atan_1024( (SAKUYA_POINT_Y_LEFT)	-src->y256, (SAKUYA_POINT_X_LEFT_OUT) -src->x256)
#define atan_1024_SAKUYA_MID		atan_1024( (SAKUYA_POINT_Y_MID) 	-src->y256, (SAKUYA_POINT_X_MID)	  -src->x256)
#define atan_1024_SAKUYA_RIGHT		atan_1024( (SAKUYA_POINT_Y_RIGHT)	-src->y256, (SAKUYA_POINT_X_RIGHT_OUT)-src->x256)
#define atan_1024_SAKUYA_CENTER 	atan_1024( (SAKUYA_POINT_Y_CENTER)	-src->y256, (SAKUYA_POINT_X_MID)	  -src->x256)

/* 咲夜さんの誘導ポイント名称 */
enum
{
	SAKUYA_POINT_00_LEFT=0, 	/*左下*/
	SAKUYA_POINT_01_MID_UP, 	/*中上*/
//	SAKUYA_POINT_02_CENTER, 	/*画面中心*/
	SAKUYA_POINT_03_RIGHT,		/*右下*/
};


/*---------------------------------------------------------
	move_aliceという名前は既に使ってるので注意
---------------------------------------------------------*/

static void sakuya_idou_sub(SPRITE *src, int speed256, int target_point_name)
{
	int b_move_angle1024;
	switch (target_point_name)
	{
	default:
	case SAKUYA_POINT_00_LEFT:		b_move_angle1024 = atan_1024_SAKUYA_LEFT;/*50*/ break;
	case SAKUYA_POINT_01_MID_UP:	b_move_angle1024 = atan_1024_SAKUYA_MID;		break;
//	case SAKUYA_POINT_02_CENTER:	b_move_angle1024 = atan_1024_SAKUYA_CENTER; 	break;
	case SAKUYA_POINT_03_RIGHT: 	b_move_angle1024 = atan_1024_SAKUYA_RIGHT;/**/	break;
	}
	src->x256 += ((sin1024((b_move_angle1024))*(speed256))>>8);/*fps_factor*/	/* CCWの場合 */
	src->y256 += ((cos1024((b_move_angle1024))*(speed256))>>8);/*fps_factor*/
	//
		 if (src->y256 < (SAKUYA_LIMIT_MIN_Y) ) 	{	src->y256 += t256(1.0); 	}/* はみ出したら修正 */
	else if (src->y256 > (SAKUYA_LIMIT_MAX_Y) ) 	{	src->y256 -= t256(1.0); 	}/* はみ出したら修正 */
}


	/*	256 == 0x100 == ( 4 x 64). [ 4秒ぐらい] */
	/*	512 == 0x200 == ( 8 x 64). [ 8秒ぐらい] */
	/*	768 == 0x300 == (12 x 64). [12秒ぐらい] */
	/* 1024 == 0x400 == (16 x 64). [16秒ぐらい] */


/*---------------------------------------------------------
	SPELL_CARD_11_sakuya_kkk	第10形態: 最終形態(その3)
---------------------------------------------------------*/

/*static*/ void sakuya_11_keitai(SPRITE *src)
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
	bullet_angle_all_hamidasi_check();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	SPELL_CARD_10_sakuya_jjj	第10形態: 最終形態(その2)
---------------------------------------------------------*/

/*static*/ void sakuya_10_keitai(SPRITE *src)
{
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
		spell_card_number++;
		src->boss_base_state001 = SG02;
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
		src->x256 += ((sin1024((src->tmp_angleCCW1024))*(src->wait2_256))>>8);/*fps_factor*/		/* CCWの場合 */
		src->y256 += ((cos1024((src->tmp_angleCCW1024))*(src->wait2_256))>>8);/*fps_factor*/
	}
	//	[SAKUYA_ANIME_12_HATUDAN_START)
	bullet_angle_all_hamidasi_check();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}

/*---------------------------------------------------------
	SPELL_CARD_09_sakuya_iii	第九形態: 最終形態(その1)
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
		xxx256 = abs((int)obj_player->x256-(int)src->x256);/*xxx256+=t256(10);*/ /* abs()は展開してくれるから大丈夫 */
		yyy256 = abs((int)obj_player->y256-(int)src->y256);/*yyy256+=t256(10);*/
		if (xxx256>yyy256)	{rrr256 = (xxx256/*+(yyy256>>1)*/);}/*少し難しくする*/
		else				{rrr256 = (yyy256/*+(xxx256>>1)*/);}/*少し難しくする*/
	}
	if (t256(40.0) > rrr256)	{	rrr256 = t256(40.0);}	/* 32 10 */
	else
	if (t256(128.0) < rrr256)	{	rrr256 = t256(128.0);}	/* 128 80 */
	return (rrr256);
}
#endif
/*static*/ void sakuya_09_keitai(SPRITE *src)
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
	switch (src->boss_base_state001)
	{
	case SG00:
			src->wait1--;/*fps_factor*/
		if (src->wait1<0)	/* 待機時間が終わったら */
		{
			src->boss_base_state001++/* = SG01*/;
//++		pd->bo ssmode=B00_NONE/*B01_BA TTLE*/;
			obj_send1->x256 	= (t256(GAME_WIDTH/2)); 		/* 弾源x256 */
			obj_send1->y256 	= (t256(30));					/* 弾源y256 */
			tmp_angleCCW1024_jikinerai(obj_send1, src);
			src->move_angle1024 = src->tmp_angleCCW1024;
		}
	//	else		{		}
		break;
	case SG01:
		if ( (src->x256 < t256(GAME_WIDTH/2+30) ) &&
			 (src->x256 > t256(GAME_WIDTH/2-10) ) &&
			 (src->y256 < t256(50.0)) ) /* 真ん中に来たら */
		{
			src->boss_base_state001++/* = SG02*/;
			src->wait1		= (20);
			src->wait2_256	= t256(0.0);
			//	[SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			src->x256 += ((sin1024((src->move_angle1024))*t256(3.0))>>8);/*fps_factor*/ 	/* CCWの場合 */
			src->y256 += ((cos1024((src->move_angle1024))*t256(3.0))>>8);/*fps_factor*/
		}
		break;
	case SG02:
		{
				src->wait1--;/*fps_factor*/
			if (src->wait1<0)			{	src->boss_base_state001++/* = SG03*/;
											//	[SAKUYA_ANIME_12_HATUDAN_START;
			}		/* 待機時間が終わったら */
		//	else if (src->wait1<6)		{	src->anxxime_frame=SAKUYA_ANIME_11_HIKARU_END;			}
		//	else if (src->wait1<12) 	{	src->anxxime_frame=SAKUYA_ANIME_13_;		}
		//	else if (src->wait1<18) 	{	src->anxxime_frame=SAKUYA_ANIME_12_;		}
		//	else if (src->wait1<24) 	{	src->anxxime_frame=SAKUYA_ANIME_11_;		}
		//	else if (src->wait1<30) 	{	src->anxxime_frame=SAKUYA_ANIME_10_;		}
		//	else if (src->wait1<36) 	{	src->anxxime_frame=SAKUYA_ANIME_08_HIKARU_START;		}
//			else if (src->wait1<36) 	{	src->anxxime_frame=aaa_tbl[src->wait1]; 				}
	//		else if (src->wait1>56) 	{	src->anxxime_frame=SAKUYA_ANIME_08_HIKARU_START;		}
//			else if (src->wait1<32) 	{	src->anxxime_frame=SAKUYA_ANIME_08_HIKARU_START+((src->wait1&0x38)>>3); }
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
			if (src->y256 > t256(150))
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
			src->boss_base_state001++/* = SG04*/;
		}
		break;
	case SG04:
			src->wait1--;/*fps_factor*/
		if (src->wait1<0)
		{
			src->wait1	= 120+(3-difficulty)*50;
			src->boss_base_state001 = SG02;
		}
		else
		{
			/* CCWの場合 */
			src->x256 += ((sin1024((src->move_angle1024))*t256(3.5))>>8);/*fps_factor*/
			src->y256 += ((cos1024((src->move_angle1024))*t256(3.5))>>8);/*fps_factor*/
		}
		break;
	}
	#if 0/*SDL(左隅座標)*/
	if ((obj_player->x256+((obj_player->w128)) < src->x256+((src->w128))+t256(25))&&
		(obj_player->x256+((obj_player->w128)) > src->x256+((src->w128))-t256(25)))
	#else/*Gu(中心座標)*/
	if ((obj_player->x256 < src->x256 +t256(25))&&
		(obj_player->x256 > src->x256 -t256(25)))
	#endif
	{	src->wait2_256 += t256(1.0);/*fps_factor*/}
	if (src->wait2_256 > t256(20.0)+((3-difficulty)*t256(30)))
	{
		//	[(SAKUYA_ANIME_12_HATUDAN_START+1);/*16???*/
	//	src->length_s_p256 = sakuya_kyori(src);
		enemy_boss06_knifes3(src/*,*/ /*1+difficulty,*/ /*(src->length_s_p256)*/ );
	}
	bullet_angle_all_hamidasi_check();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}
//	static const u8 aaa_tbl[40/*(6*6)*/] =
//	{
//		SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END,
//		SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,
//		SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,
//		SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,
//		SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,
//		SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,
//		0,0,0,0/* アライン用ダミーデーター。 dummy for align*/
//	};
//	static const u8 aaa_tbl[40/*(6*6)*/] =
//	{
//		SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END,
//		SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END, SAKUYA_ANIME_11_HIKARU_END,
//		SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,
//		SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,
//		SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,
//		SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,
//		SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,
//		SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,//32
//		SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,	SAKUYA_ANIME_08_HIKARU_START,
//		0,0,0,0/* アライン用ダミーデーター。 dummy for align*/
//	};


/*---------------------------------------------------------
	SPELL_CARD_08_sakuya_hhh	第八形態: (時止めナイフ)追加計画中
	SPELL_CARD_07_sakuya_ggg	第七形態: (分散魔方陣)追加計画中
---------------------------------------------------------*/


/*---------------------------------------------------------
	SPELL_CARD_06_sakuya_fff	第六形態: (黄色葡萄弾)
	SPELL_CARD_05_sakuya_eee	第五形態: (黄色葡萄弾)
---------------------------------------------------------*/
/*static*/extern void bullet_crate_sakuya_linear_yuudou(SPRITE *src);	/* 直線誘導ナイフ */

/*static*/ void sakuya_06_keitai(SPRITE *src)
{
	switch (src->boss_base_state001)
	{
	case SE00_ss:
//	{
//		static int seisei_count;
//		if(0==seisei_count)
		{
			add_zako_sakuya_doll_01_laser(src);/* レーザー追加 */
		}
//		seisei_count++;
//		seisei_count &= 0x01ff; 	/* 512 == 0x200 == ( 8 x 64). [ 8秒ぐらい] */
//	}
		src->boss_base_state001++;
		break;
	case SE00:
		#if 1/*使ってない？？*/
		tmp_angleCCW1024_jikinerai(obj_player, src);
		#endif
		src->wait2_256 -= t256(1.0);/*fps_factor*/
		if (src->wait2_256 < t256(0.0))
		{
			src->wait2_256	= t256(50.0);
			/* (黄色葡萄弾) */

/* CCWの場合 */
			bullet_crate_sakuya_linear_yuudou(src); 	/* 直線誘導ナイフ */
			//bullet_create_oodama0(src, t256(2), ANGLE_JIKINERAI_DAN, 40, 0, t256(src->w/2), t256(src->h/2));
			src->wait1--;/*fps_factor*/
			if (src->wait1<0)	/* 3回撃ったら */
			{
				src->wait1=20;
				if (0==(ra_nd()&(2-1)/*%2*/))
				{
					src->boss_base_state001++/* = SE01*/;
					//	[(SAKUYA_ANIME_03_CENTER_A-2);
				}
				else
				{
					src->boss_base_state001/*+=2*/ = SE02;
					//	[(SAKUYA_ANIME_03_CENTER_A+2);
				}
			}
		}
	//	[SAKUYA_ANIME_08_HIKARU_START)

		break;
	case SE01:
		src->wait1--;/*fps_factor*/
		if (src->wait1<0)
		{
			src->wait1		= (3);
			src->boss_base_state001--/* = SE00*/;
			src->wait2_256	= t256(30.0);
			//	[SAKUYA_ANIME_15_HATUDAN_END;
		}
		else
		{
			if (src->x256 < t256(100) )
			{
				src->boss_base_state001++/* = SE02*/;
				//	[SAKUYA_ANIME_07_RIGHT-1/*SAKUYA_ANIME_07_RIGHT-1*/;/*7 ???*/
			}
			else
			{	src->x256 -= t256(2.0);/*fps_factor*/}
		}
		break;
	case SE02:
		src->wait1--;/*fps_factor*/
		if (src->wait1<0)
		{
			src->wait1		= (3);
			src->boss_base_state001 = SE00;
			src->wait2_256	= t256(30.0);
			//	[SAKUYA_ANIME_15_HATUDAN_END;
		}
		else
		{
			#if 0/*SDL(左隅座標)*/
			if (src->x256 > t256(GAME_WIDTH-100)-((src->w128+src->w128)) )
			#else/*Gu(中心座標)*/
			if (src->x256 > t256(GAME_WIDTH-100) )
			#endif
			{
				src->boss_base_state001--/* = SE01*/;
				//	[SAKUYA_ANIME_00_LEFT+1;/*1 ???*/
			}
			else
			{	src->x256 += t256(2.0);/*fps_factor*/}
		}
		break;
	}
	bullet_angle_all_hamidasi_check();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	第四形態: 魅魔 魔方陣生成
---------------------------------------------------------*/

/*static*/ void sakuya_04_keitai(SPRITE *src)
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
	switch (src->boss_base_state001)
	{
	case SD00:
			add_zako_sakuya_baramaki(src);/* 子供魔方陣追加 */
		{	/* 戻す? */
			boss_clip_min.x256	= t256( 		0)+t256(24);
			boss_clip_max.x256	= t256(GAME_WIDTH)-t256(24);
		}
		src->boss_base_state001++;
		break;
	case SD01:
		sakuya_wait_state(src);/*アニメセット[SAKUYA_ANIME_12_HATUDAN_START]*/
		break;
	case SD02:
	//	if ( ( src->y > 50) && (src->x<GAME_WIDTH/2+30 || src->x>GAME_WIDTH/2-30))		/* 真ん中に来たら */
		if (( src->y256 > (SAKUYA_POINT_Y_CENTER-SAKUYA_POINT_Y_MARGIN) ) &&
			( src->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
			( src->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		/* 真ん中に来たら */
		{
			src->boss_base_state001++;
			src->wait1		= 10;
			src->wait2_256	= t256(0.0);
				//	[SAKUYA_ANIME_08_HIKARU_START;
		}
		else
		{
			src->move_angle1024 = atan_1024_SAKUYA_CENTER;/*atan_1024(50-src->y,GAME_WIDTH/2-src->w-src->x)*/
			src->x256 += ((sin1024((src->move_angle1024))*t256(3))>>8);/*fps_factor*/		/* CCWの場合 */
			src->y256 += ((cos1024((src->move_angle1024))*t256(3))>>8);/*fps_factor*/
			//	[SAKUYA_ANIME_15_HATUDAN_END)
		}
		break;
	case SD03:
		//	[SAKUYA_ANIME_11_HIKARU_END)
		{
			/* 子供魔方陣生成(2回生成しちゃうバグあり) */
			src->boss_base_state001++/* = SE00*/;/*dummy*/
		//	src->boss_base_state001 	=	0/* = SE00*/;
#if 0/*???*/
/* スペルカードシステムでは対応できない。 */
			sp ell_card_number++;		/* 次の形態へ */
#endif
			src->wait2_256	= t256(10.0);
			src->wait1		= (5);
		}
			//	[SAKUYA_ANIME_11_HIKARU_END)
		break;
	}
	bullet_angle_all_hamidasi_check();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	SPELL_CARD_03_sakuya_ccc
	時雨「バーティカルナイフ」
	第三形態: 垂直ナイフが落ちてくるよ
---------------------------------------------------------*/

/*static*/ void sakuya_03_keitai(SPRITE *src)
{
	switch (src->boss_base_state001)
	{
	case SC00:
			src->vx256 = t256(0.0); 	/* 移動停止指定 */
		{	/* 横の制限を変える */
			boss_clip_min.x256 = t256(0);			//t256(50);
			boss_clip_max.x256 = t256(GAME_WIDTH);	//t256(GAME_WIDTH)-t256(50)-((src->w128+src->w128));
		}
		src->boss_base_state001++;
		break;
	case SC01:
		sakuya_wait_state(src);/*アニメセット[SAKUYA_ANIME_03_CENTER_A]*/
		break;
	case SC02:
		if (src->x256 < (SAKUYA_LIMIT_X_LEFT)/*5*/ )	/* 左隅に来たら */	/* → || (src->y<5)があると無限ループになる*/
		{
			src->boss_base_state001++;
			src->wait1		= (10);
			src->wait2_256	= t256(0.0);
				//	??? アニメセット[SAKUYA_ANIME_00_LEFT;
		}
		else
		{
			sakuya_idou_sub(src, t256(3.0), SAKUYA_POINT_00_LEFT);
		}
		break;
	case SC03:
		sakuya_wait_state(src);/*アニメセット[(SAKUYA_ANIME_03_CENTER_A-1)]*/ /*???*/
		break;
	case SC04:
		if (src->x256 > (SAKUYA_LIMIT_X_RIGHT)/*GAME_WIDTH-src->w+3*/ ) /* 右端に来たら */
		{
			src->boss_base_state001++;
			src->wait2_256	= t256(0.0);
				//	??? アニメセット[SAKUYA_ANIME_07_RIGHT;
		}
		else
		{
			{	const short sss[(4)] =
				{
					t256(2*0.48),	/*	easy:まばらなので簡単*/ 						/* 1.2 */
					t256(2*0.75),	/*normal:まばらなので簡単*/ 						//	t256(0.48/*0.5*/ /*1.0*/ /*1.5*/),/*normal:塊なので大きく避ければ簡単*/
					t256(2*1.0),	/*	hard:たまに難しい時がある*/ 					/*2.0*/
					t256(2*18.5)	/*	luna:うへぇこのスキマくぐるんですかぁ？*/		/*6.5*/ /*12.5*/ /*8.5*/
					/*(luna:ボムらなくても気合避けでなんとかなる程度に難易度抑えたいのだがまだ高すぎるな)*/
				};
				src->wait2_256 += /*t256(4)*/sss[difficulty]/*t256(3*1.42)*/;
			}
			if (0==(((int)src->wait2_256)&(32-1-1)/*%21 t256(15*1.42)*/))/* 0x1e == (32-1-1) */
			{
				obj_send1->x256 					= (src->x256);
				obj_send1->y256 					= (src->y256);
				br.BULLET_REGIST_speed256			= /*(0.7)*/t256(1.75)/*(2.0)*/;
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
			sakuya_idou_sub(src, t256(5.0), SAKUYA_POINT_03_RIGHT); 	// SAKUYA_ANIME_07_RIGHT)
		}
		break;
	case SC05:
	//	if (( src->x256 < (SAKUYA_LIMIT_X_MID)/*GAME_WIDTH/2-src->w/2*/ ))	/* 真ん中に来たら */
		if (( src->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
			( src->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		/* 真ん中に来たら */
		{
			src->boss_base_state001++;
				//	??? アニメセット[SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			sakuya_idou_sub(src, t256(3.0), SAKUYA_POINT_01_MID_UP);		// (SAKUYA_ANIME_03_CENTER_A )
		}
		break;
	case SC06:
		if (src->x256 > (SAKUYA_LIMIT_X_RIGHT)/*GAME_WIDTH-src->w-3*/ ) /* 右端に来たら */ /* → || (src->y<5)があると無限ループになる */
		{
			src->boss_base_state001++;
			src->wait1		= (10);
			src->wait2_256	= t256(0.0);
				//	??? アニメセット[SAKUYA_ANIME_07_RIGHT;
		}
		else
		{
			sakuya_idou_sub(src, t256(3.0), SAKUYA_POINT_03_RIGHT); 	// SAKUYA_ANIME_07_RIGHT)

		}
		break;
	case SC07:
		if (src->x256 < (SAKUYA_LIMIT_X_LEFT)/*5*/) 	/* 左端に来たら */
		{
			src->boss_base_state001++;
			src->wait2_256	= t256(0.0);
				//	??? アニメセット[SAKUYA_ANIME_00_LEFT;
		}
		else
		{
			src->wait2_256 += t256(4.0)/*(3*1.42)*/;
			if (0x0000==((src->wait2_256)&(0x0f00))/*%(12*1.42)*/)
			{
				obj_send1->x256 					= (src->x256);
				obj_send1->y256 					= (src->y256);
				br.BULLET_REGIST_speed256			= t256(3.00);
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_05_AKA;
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_02_GRAVITY02;
#if 0
/* CWの場合 */
						br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)+cv1024r(180));			br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.03);	bullet_regist_vector();
				if (difficulty)
				{
						br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)+cv1024r(90));			br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.04);	bullet_regist_vector();
					if (1<difficulty)
					{	br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/));						br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.02);	bullet_regist_vector();}
				}
#else
/* CCWの場合 */
						br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)+cv1024r(360-180));		br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.03);	bullet_regist_vector();
				if (difficulty)
				{
						br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/)+cv1024r(360-90));		br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.04);	bullet_regist_vector();
					if (1<difficulty)
					{	br.BULLET_REGIST_angle1024	= (((t256_floor(src->wait2_256))&(256-1)/*%(180*1.42)*/));						br.BULLET_REGIST_jyuryoku_dan_delta256	= t256(0.02);	bullet_regist_vector();}
				}
#endif
				voice_play(VOICE12_SAKUYA_B2_SHOT, TRACK04_TEKIDAN);
			}
			sakuya_idou_sub(src, t256(5.0), SAKUYA_POINT_00_LEFT);		// SAKUYA_ANIME_00_LEFT)
		}
		break;
	case SC08:
	//	if (( src->x256 > (SAKUYA_LIMIT_X_MID)/*(GAME_WIDTH/2-src->w/2)*/ ))	/* 真ん中に来たら */
		if (( src->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
			( src->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		/* 真ん中に来たら */
		{
			src->boss_base_state001++;
			src->wait2_256	= t256(60.0);
				//	??? アニメセット[SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			sakuya_idou_sub(src, t256(3.0), SAKUYA_POINT_01_MID_UP);		// (SAKUYA_ANIME_03_CENTER_A )
		}
		break;
	case SC09:
		src->wait2_256 -= t256(1.0);
		if (src->wait2_256 < t256(0.0))
		{
		//	src->boss_base_state001++/* = SC02*/;
			src->boss_base_state001 = SC02;
				//	??? アニメセット[SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
//			src->move_angle1024 = atan_1024_SAKUYA_MID;
			if (0 == ((t256_floor(src->wait2_256))&(32-1)/*%20*/) )
			{
				int b_wait2_high;
				b_wait2_high=(((int)((t256_floor(src->wait2_256))>>5/*/20*/)));
			//	src->anxxime_frame++; src->anxxime_frame %= 7;//??? 9???
				//	??? アニメセット[		src->anxxime_frame++; src->anxxime_frame &= (8-1);
				{
//					obj_send1->x256 	=	(src->x256);
					obj_send1->y256 	=	(src->y256);
//
					#if 0
					/* CWの場合 */
						br.BULLET_REGIST_angle512 = (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(40)));
						br.BULLET_REGIST_angle512 = (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(30)));
					if (1<difficulty)
					{
						br.BULLET_REGIST_angle512 = (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(20)));
					}
					#endif

					/* CCWの場合 */
						/*src,*/
						br.BULLET_REGIST_speed256	= (b_wait2_high<<7/*8*/)+t256(2.0/*4.0*/);
//						br.BULLET_REGIST_angle512	= cv512r(/*360+*/90-30)+(b_wait2_high)*cv512r(40);/*廃止*/
						br.BULLET_REGIST_sakuya_kurukurku_knife_height = -80;
						br.BULLET_REGIST_regist_type					= REGIST_TYPE_04_KURU_KURU;

					obj_send1->x256 	=	(src->x256)+(ra_nd()&0xffff)-t256(128);/*とりあえず代替(br.BULLET_REGIST_angle512)*/
						bullet_regist_vector();
						/*src,*/
						br.BULLET_REGIST_speed256	= (b_wait2_high<<7/*8*/)+t256(1.5/*3.0*/);
//						br.BULLET_REGIST_angle512	= cv512r(/*360+*/90-30)+(b_wait2_high)*cv512r(30);/*廃止*/
						br.BULLET_REGIST_sakuya_kurukurku_knife_height = -60;
					obj_send1->x256 	=	(src->x256)+(ra_nd()&0xffff)-t256(128);/*とりあえず代替(br.BULLET_REGIST_angle512)*/
						bullet_regist_vector();
					if (1<difficulty)
					{
						/*src,*/
						br.BULLET_REGIST_speed256	= (b_wait2_high<<7/*8*/)+t256(1.0/*2.0*/);
//						br.BULLET_REGIST_angle512	= cv512r(/*360+*/90-30)+(b_wait2_high)*cv512r(20);/*廃止*/
						br.BULLET_REGIST_sakuya_kurukurku_knife_height = -40;
					obj_send1->x256 	=	(src->x256)+(ra_nd()&0xffff)-t256(128);/*とりあえず代替(br.BULLET_REGIST_angle512)*/
						bullet_regist_vector();
					}

				}
				//	??? アニメセット[SAKUYA_ANIME_15_HATUDAN_END;
			}
		}
	// [SAKUYA_ANIME_12_HATUDAN_START)
		break;
	}
	bullet_angle_all_hamidasi_check();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	SPELL_CARD_02_sakuya_bbb
	第二形態: 奇術「ミスディレクション」(全方位、豆まき)
---------------------------------------------------------*/

/*static*/ void sakuya_02_keitai(SPRITE *src)
{
	/* 第二形態では使って無いみたいだから利用する */
	#define sayuu_mode wait2_256
	switch (src->boss_base_state001)
	{
	case SB00:	/* 初期値 */
		src->wait1		= 0;/*とりあえず*/
		src->sayuu_mode = (ra_nd()&(2-1));
		src->boss_base_state001++;
//		src->vx256 = t256(0.0); 	/* 移動停止指定 */
		break;
/*----------------*/
	case SB01: /* 真中へワープ、移動方向セット */
		src->x256 = t256(GAME_WIDTH/2); /* 真中へワープ */
		{
			src->sayuu_mode ^= (2-1);/* 反転 */
			if (0==src->sayuu_mode)
					{	src->vx256 = t256(1.0);/*0.5*/		}
			else	{	src->vx256 = -t256(1.0);/*0.5*/ 	}
		}
		src->wait1		= (35);/* 移動量==待機時間[WAITSTATE] */	/* 弾撃たないでちょっと移動 */
		//	??? アニメセット[SAKUYA_ANIME_03_CENTER_A]
		src->boss_base_state001++;
		break;
	case SB02:	/* 弾撃たないでちょっと移動 */
		sakuya_wait_state(src);/*アニメセット[SAKUYA_ANIME_12_HATUDAN_START]*/
		//	??? アニメセット[SAKUYA_ANIME_12_HATUDAN_START] //	/* 弾撃ちアニメ待機 */
		break;
	case SB03:	/* 24弾幕をセット */
		//	enemy_boss06_mamemaki_state(src, SB07, SAKUYA_ANIME_15_HATUDAN_END);
			src->boss_base_danmaku_type 	= DANMAKU_02_sakuya;	/* 24弾幕をセット */
			danmaku_set_time_out(src);		/* 弾幕の時間切れを設定 */
			src->boss_base_state001++;
		break;
	case SB04:	/* 24弾幕が終わるまで弾撃ちながら移動 */
		danmaku_state_check_holding(src); /*, SAKUYA_ANIME_03_CENTER_A*/
		break;
/*----------------*/
	case SB05:
			src->vx256 = t256(0.0); 	/* 移動停止指定 */
			src->x256 = t256(GAME_WIDTH)-src->x256; 			/* 反対側へワープ */
		//	??? アニメセット[SAKUYA_ANIME_12_HATUDAN_START] //	/* 弾撃ちアニメ待機 */
			src->boss_base_state001++;
		break;
	case SB06:
			src->boss_base_danmaku_type 	= DANMAKU_03_sakuya;	/* 11弾幕をセット */
			danmaku_set_time_out(src);		/* 弾幕の時間切れを設定 */
			src->wait1			= (35);/* (SB12で)少し待機 */
		//	??? アニメセット[SAKUYA_ANIME_03_CENTER_A]
			src->boss_base_state001++;
		break;
	case SB07:	/* 11弾幕が終わるまで待機 */
		danmaku_state_check_holding(src); /*, SAKUYA_ANIME_03_CENTER_A*/
		break;
	case SB08:	/* (SB12で)少し待機 */
		sakuya_wait_state(src);/*アニメセット[SAKUYA_ANIME_03_CENTER_A]*/
		break;
	case SB09:
		src->boss_base_state001 = SB01;
		break;
	}
	bullet_angle_all_hamidasi_check();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	SPELL_CARD_01_sakuya_aaa
	第一形態: 通常攻撃(左右に動いて禊弾撃ち)
	通常攻撃(禊弾幕、原作とはあえて若干違う)
---------------------------------------------------------*/
static void misogi_set(SPRITE *src)
{
			src->vx256 = t256(0.0); 	/* 移動停止指定 */
			src->boss_base_state001++;
			#if 1/* 可変弾幕 */
			tmp_angleCCW65536_jikinerai(obj_player, src);/* 自機ねらい角作成 */
				#if 0
					/*CW*/			src->tmp_angleCCW1024	-= 256;
				#else
				//	/*CCW*/ 		src->tmp_angleCCW1024	+= (512-256);
					/*CCW*/ 		src->tmp_angleCCW65536	+= (32768-16384);
				#endif
			#else/* 固定弾幕 */
			//	/*CCW*/ 			src->tmp_angleCCW1024	= cv1024r((90));/* 右側から撃ち始める (256) */
				/*CCW*/ 			src->tmp_angleCCW65536	= cv65536r((90));/* 右側から撃ち始める (256) */
			#endif
			src->boss_base_danmaku_type 	= DANMAKU_01_sakuya;	/* 禊弾幕をセット */
			danmaku_set_time_out(src);		/* 弾幕の時間切れを設定 */
			/*[発弾アニメ指定セット]*/		// [SAKUYA_ANIME_12_HATUDAN_START]

}

/*static*/ void sakuya_01_keitai(SPRITE *src)
{
	static int my_yudo_point;
	switch (src->boss_base_state001)
	{
	/* 第一形態は画面中心(横x位置)から始まる */
/*----------------*/
	case SA00:/* 次の移動位置(画面左側)を決める */
		src->boss_base_state001++;
		src->vx256 = -t256(2.0/*3.0*/);/*fps_factor*/	/* 左方向へ移動指定 */
	//	my_yudo_point = SAKUYA_POINT_X_LEFT_MID;
		my_yudo_point = SAKUYA_POINT_X_MID-(ra_nd()&((128*256)-1));/* 画面左側の適当な位置 */	/*例:x256==t256(50.0)*/
		break;
	case SA01:	/* 左へ移動 */
		if (my_yudo_point > src->x256 ) 	/* 画面左側の適当な位置へ到達したら */
		{
			misogi_set(src);
		}
		break;
	case SA02:	/* 禊弾幕が終わるまで待機 */	/* 弾撃ちアニメ待機 */
		danmaku_state_check_holding(src);/*, SA03*/ /* SAKUYA_ANIME_03_CENTER_A */
		break;
	case SA03:
		src->vx256 = t256(3.0/*3.0*/);/*fps_factor*/	/* 真ん中へ移動 */
		src->boss_base_state001++;
		break;
	case SA04:	/* 真ん中に戻る */
		if (SAKUYA_POINT_X_MID < src->x256 )	/* 真ん中まで来たら */
		{
			src->vx256 = t256(0.0); 	/* 移動停止指定 */
			src->boss_base_state001++;
		}
		break;
/*----------------*/
	case SA05:/* 次の移動位置(画面右側)を決める */
			src->boss_base_state001++;
			src->vx256 = t256(2.0/*3.0*/);/*fps_factor*/	/* 右方向へ移動指定 */
		//	my_yudo_point = SAKUYA_POINT_X_RIGHT_MID;
			my_yudo_point = SAKUYA_POINT_X_MID+(ra_nd()&((128*256)-1));
		break;
	case SA06:	/* 右へ移動 */
		if ( my_yudo_point < src->x256 )	/* 画面右側の適当な位置へ到達したら */
		{
			misogi_set(src);
		}
		break;
	case SA07:	/* 禊弾幕が終わるまで待機 */	/* 弾撃ちアニメ待機 */
		danmaku_state_check_holding(src); /* [SAKUYA_ANIME_03_CENTER_A] */
		break;
	case SA08:
			/*[??? アニメ指定セット]*/		// [SAKUYA_ANIME_03_CENTER_A]
		src->vx256 = -t256(3.0/*3.0*/);/*fps_factor*/	/* 真ん中へ移動 */
		src->boss_base_state001++;
		break;
	case SA09:	/* 真ん中に戻る */
		if (SAKUYA_POINT_X_MID > src->x256 )	/* 真ん中に戻る */
		{
			src->vx256 = t256(0.0); 	/* 移動停止指定 */
			src->boss_base_state001 = SA00;
		}
		break;
	}
	bullet_angle_all_hamidasi_check();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}

	//	if (SPELL_CARD_04_sakuya_ddd != spell_card_number)	/* 第四形態: 魔方陣生成 */
	/* 魔方陣生成中はoff */


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void boss_init_sakuya(SPRITE *h)
{
	h->callback_loser				= sakuya_put_items;

	h->wait1						= (0);
	h->wait2_256					= t256(0.0);
	h->move_angle1024				= (0);
	//-------------------------
	obj_send1->type 				= SP_MUTEKI;
	obj_send1->x256 				= (0);
	obj_send1->y256 				= (0);
//
	//	bullet_clip_min.x256 = t256(-100);
	//	bullet_clip_max.x256 = t256(100)+t256(GAME_WIDTH);
	//	bullet_clip_min.y256 = t256(-256);/* あや */
	//	bullet_clip_max.y256 = t256(100)+t256(GAME_HEIGHT);
		bullet_clip_min.x256 = t256(-32);
		bullet_clip_max.x256 = t256(0)+t256(GAME_WIDTH);
		bullet_clip_min.y256 = t256(0);
		bullet_clip_max.y256 = t256(0)+t256(GAME_HEIGHT);
}
//	h->callback_mover				= move_alice;
//	h->callback_mover				= move_alice;

//	obj_send1						= sprite_add_res(BASE_BOSS_SAKUYA_PNG);
//	obj_send1->flags				&= (~(SP_FLAG_VISIBLE));
//	obj_send1->an im_frame			= SAKUYA_ANIME_03_CENTER_A;
//	obj_send1->callback_mover		= dummy_move;

//	h->anxxime_frame					= SAKUYA_ANIME_03_CENTER_A;
//	sakuya->wa it3						= 0;
