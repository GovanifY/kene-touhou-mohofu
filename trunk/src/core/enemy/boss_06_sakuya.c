
#include "bullet_object.h"

/*---------------------------------------------------------
	十六夜 咲夜
	PAD
	-------------------------------------------------------
	ToDo:
	移動と弾幕を分離しよう。でないと訳わからん。
	ボスタイマー対応中
	-------------------------------------------------------
	★ボスタイマーはシステムでサポート予定です。(作業中)
	★弾幕はシステムでサポートする構想があります。(構想中)
	システムでサポートされると、どのボスが、どの弾幕でも撃てるようになります。
---------------------------------------------------------*/

/*
C++みたいに 継承 させてみる。
が、よく考えたら現状globalのplayerにあるので
要らなかったのでOFF。でもあとでこの考えは使うかも。
 */
#define USE_KEISYOU 0
//#define USE_KEISYOU 1
/*
継承しない 2019729
継承させる 2019425(継承させた方が小さい)
*/

typedef struct /*_boss06_data*/
{
	BOSS_BASE boss_base;
//------------ 移動関連
	int state1; 							/* 行動 */
//	int move_type;		/*	移動タイプ */	/* 形態 */
//
	int wait1;
	int wait2_256;
	int wait3;
	int move_angle512;
//	int length_s_p256;	/* 咲夜とプレイヤーとの距離 */

} BOSS06_DATA;
//	int aaa_type;		/* 0: normal, 1: destroyed */
#define move_vx256 move_angle512


typedef struct /*_boss06_doll_data*/
{
	#if (1==USE_KEISYOU)
	SPRITE *sakuya_obj; 		/* 継承 */
	#endif
	int state_d01;	/* 行動 */
	int state_d02;	/* 形態 */
	int nnn;
	int hen_no_kazu;	/* 星型の辺の数をカウント(0辺から4辺までの計5辺==五角形) */
	int wait;
	int angle512;
//
	int sss;
	int jjj;
	int doll_vx;
	int doll_vy;
	int doll_point_x;			/* 目標座標 */
	int doll_point_y;			/* 目標座標 */
	int doll_point_angle512;
	int star_remain_time;
//
//	int tx256;
//	int ty256;
} BOSS06_DOLL_DATA;


#if 1
/* 構造体の外にある必然性がある */
	#define FLG_NO_DOLL 	(0)
	#define FLG_ALL_CAST	(1)
static int kodomo_num;
//static SPRITE *dummy_obj; /*dummy_obj*/
#endif


//----[ZAKO]

static SPRITE *obj_doll;		// 子供魔方陣のスプライト

/*---------------------------------------------------------

---------------------------------------------------------*/


enum
{
//--- 星型以前
	DS00 = 0,
	DS01,
	DS02,
	DS03,
};
enum
{
//--- 星型準備
	DB00 = 0,
//--- 星型以降
	DB01,
	DB02,
	DB03,
	DB04,
	DB05,
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
	SA10,
	SA11,
	SA12,
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
	SB10,
	SB11,
	SB12,
	SB13,
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
};
enum
{
	SD00 = 0,
	SD01,
	SD02,
};
enum
{
	SE00 = 0,
	SE01,
	SE02,
};
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
	SAKUYA_ANIME_00_LEFT = 0,
	SAKUYA_ANIME_01_,
	SAKUYA_ANIME_02_,
//	SAKUYA_ANIME_03_,
	SAKUYA_ANIME_04_CENTER_A,
//
	SAKUYA_ANIME_05_CENTER_B,
	SAKUYA_ANIME_06_,
	SAKUYA_ANIME_07_,
//	SAKUYA_ANIME_08_,
	SAKUYA_ANIME_09_RIGHT,
//
	SAKUYA_ANIME_09_ = 0x10,
//	SAKUYA_ANIME_10_,/*0x11*/
	SAKUYA_ANIME_11_,/*0x12*/
//	SAKUYA_ANIME_12_,/*0x13*/
	SAKUYA_ANIME_13_,/*0x14*/
	SAKUYA_ANIME_14_,/*0x15*/
//
	SAKUYA_ANIME_15_HATUDAN,/*0x16*/
	SAKUYA_ANIME_16_,/*0x17*/
	SAKUYA_ANIME_17_,/*0x18*/
	SAKUYA_ANIME_18_,/*0x19*/
};

/*---------------------------------------------------------
	子供魔方陣 移動
---------------------------------------------------------*/

#define DOLL_WAIT50 		(16/*16*/)
#define DOLL_SPEED10		(4/*8*/)
#define DOLL_SET03			(1)
#define DOLL_ADD05			(1)

#define DOLL_POINT_CX00 	(t256(GAME_WIDTH/2))
#define DOLL_POINT_CY00 	(t256(/*GAME_HEIGHT/4*/64))
#define DOLL_RADIUS_SIZE	(/*t256*/(90))

#define DOLL_HEN_MAX		(5)

//------------------------------------------

	/*
	星型線1 190, 48
	星型線2 248, 217
	星型線3 100, 113
	星型線4 280, 113
	星型線5 138, 217
	*/
/*
#define pX 0
#define pY 1
	u16 ppp[5][2] =
	{
		{t256(190), t256( 48)},
		{t256(248), t256(217)},
		{t256(100), t256(113)},
		{t256(280), t256(113)},
		{t256(138), t256(217)},
	};
*/

/*static*/extern void enemy_sp2_bullet_create01(SPRITE *src, int angle512, int star_remain_time/**sd_nnn*/);
static void move_doll02(SPRITE *src)
{
	BOSS06_DOLL_DATA *data = (BOSS06_DOLL_DATA *)src->data;
	data->star_remain_time--;
//
	switch (data->state_d02)
	{
//------------------------------------------
	case DB00:	/* 星型を描く準備 */
		data->state_d02++/* = DB01*/;
		data->doll_point_angle512=0;
		data->doll_vx = t256(1.0);
		data->doll_vy = t256(0.0);
		#if (1==USE_KEISYOU)
		src->x256 = (data->sakuya_obj->x256);		dummy_obj->x256 = src->x256;
		src->y256 = (data->sakuya_obj->y256);		dummy_obj->y256 = src->y256;
		#else
		src->x256 = (((((PLAYER_DATA *)player->data)->boss))->x256);		dummy_obj->x256 = src->x256;
		src->y256 = (((((PLAYER_DATA *)player->data)->boss))->y256);		dummy_obj->y256 = src->y256;
		#endif
		data->sss = 0;
		break;
//------------------------------------------
	case DB01:	/* 準備完了まで待つ */
		src->anim_frame = 0;
//			if (2==data->nnn/*%(4-difficulty)*/)/* %2 == (5-3:Lunatic) */
//			{	src->anim_frame = 1;}
//			else
//			{	src->anim_frame = 0;}
		#if (1==USE_KEISYOU)
		if (((BOSS06_DATA *)data->sakuya_obj->data)->state1==1)
		#else
		if (((BOSS06_DATA *)((((PLAYER_DATA *)player->data)->boss))->data)->state1==1)
		#endif
		{
			data->state_d02++ /*= DB02*/;
		}
		break;
//------------------------------------------
	case DB02:
		data->state_d02++ /*= DB03*/;
		data->state_d01 		= 0x00;
//		data->star_remain_time	= (64*5);
		data->wait				= DOLL_WAIT50;
		break;
	case DB03:	/* 一時停止 */
	//	hosi_wait(src); static void hosi_wait(SPRITE *src)
		{
		//	BOSS06_DOLL_DATA *data = (BOSS06_DOLL_DATA *)src->data;
		//	if (data->wait<0)	{	data->state_d02++/* = DB04*/;}	else	{	data->wait--;}
			data->wait--;	if (data->wait<0)	{	data->state_d02++/* = DB04*/;}
		}
		break;
//------------------------------------------
	case DB04:	/* 星型を描くよ */
//		data->doll_point_x=DOLL_POINT_CX00+sin512((data->doll_point_angle512))*DOLL_RADIUS_SIZE;
//		data->doll_point_y=DOLL_POINT_CY00+cos512((data->doll_point_angle512))*DOLL_RADIUS_SIZE;

		/*	*/
		dummy_obj->x256 += (data->doll_vx)/**fps_fa ctor*/;
		dummy_obj->y256 += (data->doll_vy)/**fps_fa ctor*/;
		data->sss++;
		if (
	//		(t256(32) > abs(data->doll_point_x-src->x256)) &&
	//		(t256(32) > abs(data->doll_point_y-src->y256))
				(16 < data->sss)
		)
		{
			data->sss = 0;
			data->doll_point_angle512 += (512*2/5);
			mask512(data->doll_point_angle512);
			{
			/* CCWの場合 */
		//	int d_angle512=atan_512(data->doll_point_y-src->y256-((src->h128)), data->doll_point_x-src->x256-((src->w128)));/*画面中心より上*/
			data->doll_vx = ((sin512((data->doll_point_angle512/*d_angle512*/))*/*t256*/(/*3*/DOLL_SPEED10/*+difficulty*/))/*>>8*/)/**fps_fa ctor*/;
			data->doll_vy = ((cos512((data->doll_point_angle512/*d_angle512*/))*/*t256*/(/*3*/DOLL_SPEED10/*+difficulty*/))/*>>8*/)/**fps_fa ctor*/;
			}

		//
			data->state_d02++/* = DB05*/;
		//
			data->hen_no_kazu--;
		//	if ((5-1) < data->hen_no_kazu)/* 5回==(5-1) */
			if (0 == data->hen_no_kazu)/* 5回==(5-1) */
			{
				data->hen_no_kazu = (DOLL_HEN_MAX);
			//	data->hen_no_kazu &= 1;
				data->star_remain_time	= (64*5);
				data->nnn++;	if (data->nnn>(/*4-0*/3/*difficulty*/) )	{data->nnn=0/*1*/ /*0*/;}
				#if (1==USE_KEISYOU)
				src->x256 = (data->sakuya_obj->x256);		dummy_obj->x256 = src->x256;
				src->y256 = (data->sakuya_obj->y256);		dummy_obj->y256 = src->y256;
				#else
				src->x256 = (((((PLAYER_DATA *)player->data)->boss))->x256);		dummy_obj->x256 = src->x256;
				src->y256 = (((((PLAYER_DATA *)player->data)->boss))->y256);		dummy_obj->y256 = src->y256;
				#endif
			//	data->tx256 = DOLL_POINT_CX00;
			//	data->ty256 = DOLL_POINT_CY00;
			//	dummy_obj->x256 = data->tx256;
			//	dummy_obj->y256 = data->ty256;
			}
		}
		else
		{
			data->state_d01+=/*5*/DOLL_ADD05/*3*/;
			if (0/*1*/==data->nnn)
			{
				if (0==(data->state_d01&/*7*/DOLL_SET03/*%5*/))
				{
					/* CWの場合 */
				//	enemy_sp2_bullet_create00(src, deg_360_to_512(3)+(( ((-data->state_d01*2/**data->hen_no_kazu*/)) +data->state_d01)>>3/*/5*/), t256(0.015), &(((BOSS06_DOLL_DATA *)src->data)->nnn) );
/* CCWの場合 */
					enemy_sp2_bullet_create01(dummy_obj,
						data->doll_point_angle512,	//	-deg_360_to_512CCW(3)+(( ((-data->state_d01*2/**data->hen_no_kazu*/)) +data->state_d01)>>3/*/5*/),
					//	t256(0.015),
						data->star_remain_time//	&(((BOSS06_DOLL_DATA *)src->data)->nnn)
					);
				}
			}
		}
		break;
	case DB05:
		data->jjj++;
		if ((3-difficulty) < data->jjj)
		{
			data->jjj = 0;
		//	bullet_create_360_knifes(src, t256(1.2) );
			/*---------------------------------------------------------
				360ナイフ
				魔方陣用
				全方向に青ナイフ弾を撃つ
			---------------------------------------------------------*/

			//static void bullet_create_360_knifes(SPRITE *src, int speed256)
			{
			//	int angle512;
			//	for (angle512=0; angle512<512; angle512+=32/*29*/)
			//	{
			//		bullet_create_sakuya_knife(src, (speed256), (angle512), /*i*/(angle512)/*anime_pattern*/);
			//	}
			bullet_create_n_way_dan_sa_type(src,
				(t256(1.2)/*(speed256)*/),
				(0/*aaa_angle512*/),
				(int)(512/(18)),		/* 角度([360/360]度を18分割) */
				BULLET_KNIFE20_06_YUKARI/*BULLET_KNIFE20_04_AOI*/,
				(18));/* [18wayピンクナイフ弾] */
			}
		}
		data->state_d02 = DB02;
		break;

//------------------------------------------
	}
	#if 1
	//if (data->c->type==SP_DELETE) 		/*これではうまくいかない場合がある*/
	if (0/*FLG_NO_DOLL*/==common_boss_flags)	/* 輝夜を倒すと皆破壊される。 */
	{
		src->type = SP_DELETE;
	}
	#endif
//
	#if 0/*魔方陣デバッグ用*/
	/* パネルのスコア欄にstate_d02を、グレイズ欄にstate_d01を表示させる。っていうか書き換えちゃう。 */
	((PLAYER_DATA *)player->data)->my_score 	= data->state_d02;
	((PLAYER_DATA *)player->data)->graze_point	= data->state_d01;
	#endif
}

static void move_doll01(SPRITE *src)
{
	BOSS06_DOLL_DATA *data = (BOSS06_DOLL_DATA *)src->data;
	//if (data->state_d02 < DB01)
	{
		#if (1==USE_KEISYOU)
		src->x256 = data->sakuya_obj->x256+((data->sakuya_obj->w128-src->w128));
		src->y256 = data->sakuya_obj->y256+((data->sakuya_obj->h128-src->h128));
		#else
		src->x256 = ((((PLAYER_DATA *)player->data)->boss))->x256+((((((PLAYER_DATA *)player->data)->boss))->w128-src->w128));
		src->y256 = ((((PLAYER_DATA *)player->data)->boss))->y256+((((((PLAYER_DATA *)player->data)->boss))->h128-src->h128));
		#endif
	//
		#if (1==USE_KEISYOU)
	//	if (((BOSS06_DATA *)data->sakuya_obj->data)->move_type==SPELL_CARD_09_sakuya_iii/*4*/ /*星型を描いてもいいよ*/)
		#else
	//	if (((BOSS06_DATA *)((((PLAYER_DATA *)player->data)->boss))->data)->move_type==SPELL_CARD_09_sakuya_iii/*4*/ /*星型を描いてもいいよ*/)
		if (spell_card_number==SPELL_CARD_09_sakuya_iii/*4*/ /*星型を描いてもいいよ*/)
		#endif
		{
			static int hosi_gata_kakuze_counter;
			hosi_gata_kakuze_counter--;
			if (hosi_gata_kakuze_counter < 0)	/*星型を描いてもいいよ*/
			{
				hosi_gata_kakuze_counter = (60*10);
			//	data->state_d02 = DB01;/*星型を描くぜ*/
			//	data->state_d02 = DB00;/*星型を描くぜ*/
				data->state_d02 = DB00;/*星型を描くぜ*/
				src->callback_mover = move_doll02;
			}
		}
	}
	switch (data->state_d02)
	{
//------------------------------------------
	case DS00:
		src->color32 += 0x03000000; 		/*	src->alpha += 0x03;*/
		if (0x96000000 < src->color32)		/*	(0x96 < src->alpha) */
		{
			src->color32		= 0x96ffffff;	/*	src->alpha		= 0x96;*/
			data->state_d02++/* = DS01*/;
			data->wait		= 50;
		}
		break;

	case DS01:
		data->wait -= 1/*fps_fa ctor*/;
		if (data->wait < 1)
		{
			data->state_d02++/* = DS02*/;
			data->state_d01 = 0x100;
		}
		break;
	case DS02:
		if (0==(data->state_d01&0x0f/*%10*/))
		{
#if 0
/* CWの場合 */
			bullet_create_tomari2_dan(src, (data->state_d01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512, 					  t256(0.02), data->angle512+(data->state_d01)-deg_360_to_512((180	)));	//ra d2deg512((data->state_d01)>>7/*/100*/)
			bullet_create_tomari2_dan(src, (data->state_d01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512+deg_360_to_512((120)), t256(0.02), data->angle512+(data->state_d01)-deg_360_to_512((180+120)));	//ra d2deg512((data->state_d01)>>7/*/100*/)
			bullet_create_tomari2_dan(src, (data->state_d01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512-deg_360_to_512((120)), t256(0.02), data->angle512+(data->state_d01)-deg_360_to_512((180-120)));	//ra d2deg512((data->state_d01)>>7/*/100*/)
#else
/* CCWの場合 */
			bullet_create_tomari2_dan(src, (data->state_d01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512, 						 t256(0.02), data->angle512+(data->state_d01)+deg_360_to_512CCW((180	)));	//ra d2deg512((data->state_d01)>>7/*/100*/)
			bullet_create_tomari2_dan(src, (data->state_d01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512-deg_360_to_512CCW((120)), t256(0.02), data->angle512+(data->state_d01)+deg_360_to_512CCW((180+120)));	//ra d2deg512((data->state_d01)>>7/*/100*/)
			bullet_create_tomari2_dan(src, (data->state_d01/*>>4*/<<3/*1/10*/)/**256*/, data->angle512+deg_360_to_512CCW((120)), t256(0.02), data->angle512+(data->state_d01)+deg_360_to_512CCW((180-120)));	//ra d2deg512((data->state_d01)>>7/*/100*/)
#endif
		}
		data->state_d01--;
		if (data->state_d01 < 1)
		{
			data->state_d02++/* = DS03*/;
			data->wait		= 15;
			data->state_d01 = 0x10;
		}
		break;
	case DS03:
		data->wait -= 1/*fps_fa ctor*/;
		if (data->wait < 1)
		{
			data->wait=12;
			#if (0==USE_DESIGN_TRACK)
			play_voice_auto_track(VOICE13_SAKUYA_SHOT02);
			#else
			voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
			#endif
			{int i;
				for (i=t256(5); i<t256(9); i+=t256(1))
				{
#if 0
/* CWの場合 */
					bullet_create_offset_dan_type000(src, i, data->angle512,					 /*t256(0), t256(0),*/ BULLET_HARI32_00_AOI);
					bullet_create_offset_dan_type000(src, i, data->angle512+deg_360_to_512(120), /*t256(0), t256(0),*/ BULLET_HARI32_00_AOI);
					bullet_create_offset_dan_type000(src, i, data->angle512-deg_360_to_512(120), /*t256(0), t256(0),*/ BULLET_HARI32_00_AOI);
#else
/* CCWの場合 */
					bullet_create_offset_dan_type000(src, i, data->angle512,						/*t256(0), t256(0),*/ BULLET_HARI32_00_AOI);
					bullet_create_offset_dan_type000(src, i, data->angle512-deg_360_to_512CCW(120), /*t256(0), t256(0),*/ BULLET_HARI32_00_AOI);
					bullet_create_offset_dan_type000(src, i, data->angle512+deg_360_to_512CCW(120), /*t256(0), t256(0),*/ BULLET_HARI32_00_AOI);
#endif
				}
			}
			/* deg_360_to_512(8.59) 8.59436717317284029518323968680208	PI*2/41.8879008478638666666666666666667 ra d2deg512(0.15)*/
			/* 12.2230999796235950864828297767852 == 512/41.8879008478638666666666666666667 */
#if 0
/* CWの場合 */
			data->angle512 += 12;
#else
/* CCWの場合 */
			data->angle512 -= 12;
#endif
			data->state_d01--;
		}
		if (data->state_d01<0x0)
		{
			data->state_d02--/* = DS02*/;
			data->state_d01 = 0x50;
		}
		break;

	}
	#if 1
	//if (data->c->type==SP_DELETE) 		/*これではうまくいかない場合がある*/
	if (0/*FLG_NO_DOLL*/==common_boss_flags)	/* 輝夜を倒すと皆破壊される。 */
	{
		src->type = SP_DELETE;
	}
	#endif
//
	#if 0/*魔方陣デバッグ用*/
	/* パネルのスコア欄にstate_d02を、グレイズ欄にstate_d01を表示させる。っていうか書き換えちゃう。 */
	((PLAYER_DATA *)player->data)->my_score 	= data->state_d02;
	((PLAYER_DATA *)player->data)->graze_point	= data->state_d01;
	#endif
}

static void move_doll00(SPRITE *src)
{
//	if (sp ell_card_number==SPELL_CARD_09_sakuya_iii/*4*/ /*星型を描いてもいいよ*/)
	if (0 != kodomo_num)
	{
	//	BOSS06_DOLL_DATA *data = (BOSS06_DOLL_DATA *)src->data;
	//	data->state_d02 = DS00;
		src->callback_mover = move_doll01;
	}
}

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
	//tama->anim_frame	= SAKUYA_ANIME_04_CENTER_A;//バグ(ここでプレイヤーショットのアニメパターンを弄るので、レミボムでハングアップする)
	src->anim_frame 	= SAKUYA_ANIME_04_CENTER_A;//咲夜のアニメパターンを弄る
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	data->state1		= 0;/* 共通して 値を 0 にする */
	data->wait1 		= 80;
	data->wait2_256 	= t256(-100);
	data->wait3 		= 0;//???

}
	#if (0)
	sp ell_card_number++;
	#endif
	#if (0)
//	src->base_health	= (1024-1); 	/* life_point消費しゲージ補充 */
	{
		u16 aaa_timer[10/*8*/] =
		{
			(1000*64),	/* ダミー */
			(1000*64),	/* 最終形態 */
			(40*64),
			(40*64),
			(40*64),
			(40*64),
			(40*64),
			(40*64),
			(40*64),
			(40*64),
		};
		spell_card_boss_timer		= aaa_timer[(data->boss_base.bo ss_life)];		/* 40*64==40[count] 	約40[秒(64/60)](単位は秒ではない) */
	}	/* ボスタイマー初期化 */
	// 40 60 [秒] ???	4096/*2500*/	/*3000*/;//???
	#endif

/*---------------------------------------------------------

---------------------------------------------------------*/



/*---------------------------------------------------------

---------------------------------------------------------*/

static void sakuya_wait_state(SPRITE *src, /*int nextstate,*/ int anim_frame)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	data->wait1 -= 1/*fps_fa ctor*/;
	if (0 < data->wait1)
	{;}
	else
	{
		data->state1++/* = nextstate*/;
		src->anim_frame = anim_frame;
	}
}


/*---------------------------------------------------------
	弾幕が終わるまで待つ。
	-------------------------------------------------------

---------------------------------------------------------*/

static void danmaku_state_check_holding(SPRITE *src)	/*, int nextstate*/ /*, int anim_frame*/
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	if (DANMAKU_00 == data->boss_base.danmaku_type)
	{
		data->state1++/* = nextstate*/;
	}
}


/*---------------------------------------------------------
	咲夜	中心→左右アニメサブ
	-------------------------------------------------------
	s->anim_frame = SAKUYA_ANIME_04_CENTER_A;
	data->wait3 = 0;
	で初期化されてる必要がある。
---------------------------------------------------------*/

static void sakuya_anime_to_sayuu(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	if (0 < data->move_vx256)
	{
		/* 右へ移動 */
		if (src->anim_frame==SAKUYA_ANIME_09_RIGHT)
		{
			data->state1++/*	= SB06*/;/*おしまい*/
		}
		else
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=3)
			{
				data->wait3=0;
				src->anim_frame++;/*???*/
			}
		}
	}
	else
	{
		/* 左へ移動 */
		if (src->anim_frame==SAKUYA_ANIME_00_LEFT)
		{
			data->state1++/*	= SB06*/;/*おしまい*/
		}
		else
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=3)
			{
				data->wait3=0;
				src->anim_frame--;/*???*/
			}
		}
	}
}


/*---------------------------------------------------------
	咲夜	弾撃ちアニメ待機
	-------------------------------------------------------
	s->anim_frame = SAKUYA_ANIME_15_HATUDAN;
	data->wait3 = 0;
	で初期化されてる必要がある。
---------------------------------------------------------*/

static void sakuya_tama_anime_wait3_0(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	if (src->anim_frame==SAKUYA_ANIME_18_)
	{
		data->state1++/*	= SB06*/;
	}
	else
	{
		data->wait3 += 1/*fps_fa ctor*/;
		if (data->wait3>=3)
		{
			data->wait3=0;
			src->anim_frame++;/*???*/
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss06_out(SPRITE *src)
{
	if (src->x256 < t256(50))												{	src->x256 = t256(50);}
	if (src->x256 > t256(GAME_WIDTH)-t256(50)-((src->w128+src->w128)))		{	src->x256 = t256(GAME_WIDTH)-t256(50)-((src->w128+src->w128));}
	if (src->y256 < t256(10))												{	src->y256 = t256(10);}
	if (src->y256 > t256(GAME_HEIGHT)-t256(120)+((src->h128+src->h128)))	{	src->y256 = t256(GAME_HEIGHT)-t256(120)+((src->h128+src->h128));}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

#if 1
/* プレイヤーの周りに散らばり、時間が経つとプレイヤー狙い弾 */
static void enemy_boss06_knifes3(SPRITE *src)	/*,*/ /*dou ble speed,*/	/*int length222*/
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	int len256;
	int tmp1_256;
	int tmp2_256;
	int tmp888;
	tmp888=0;
	/* ある程度遠くに出ないと認識しづらい */
	/* キャラ(コア)がでかいと避ける前に死ぬ。(ナイフが広がる前にあたる) */
	len256 = t256(/*70*/80) -((difficulty)<<8) /*-length222*/  /*/2*/;
	data->wait2_256 = t256(0);
	while (1)
	{
		tmp888 = (((int)ra_nd()+(t256_floor(data->wait2_256)) )&(8-1)/*%8*/);
		data->wait2_256 += t256(1);
		len256 += t256(0.1);
#if 0
/* CWの場合 */
		tmp1_256 = player->x256 + ((player->w128-src->w128)) + ((cos512((tmp888<<6))*len256)>>8);
		tmp2_256 = player->y256 + ((player->h128-src->h128)) + ((sin512((tmp888<<6))*len256)>>8);
#else
/* CCWの場合 */
		tmp1_256 = player->x256 + ((player->w128-src->w128)) + ((sin512((tmp888<<6))*len256)>>8);
		tmp2_256 = player->y256 + ((player->h128-src->h128)) + ((cos512((tmp888<<6))*len256)>>8);
#endif
		if ((( t256(30) < tmp1_256) && (tmp1_256 < t256(GAME_WIDTH-30)))&&
			(( t256(30) < tmp2_256) && (tmp2_256 < t256(GAME_HEIGHT-72))))
		{	break;}
	}
	int tmp_x256;
	int tmp_y256;
	tmp_x256 = player->x256 + ((player->w128));
	tmp_y256 = player->y256 + ((player->h128));
	int ii;
	for (ii=0; ii<8; ii++)
	{
		if (tmp888==ii)
		{
			src->tmp_angleCCW512	= (ii<<6);
			src->x256				= tmp1_256;
			src->y256				= tmp2_256;
			src->color32			= 0x00ffffff;			/*	src->alpha			= 0x00;*/
//			src->anim_frame 		= SAKUYA_ANIME_18_;//?????
#if 0/*???*/
/* スペルカードシステムでは対応できない。 */
			sp ell_card_number	= SPELL_CARD_10_sakuya_jjj/*5*/;/*???*/
#endif
			data->wait2_256 	= /*speed256*/t256(1)+(difficulty<<8);
		}
		else
		{
		//	dou ble tmp_s_x;
		//	dou ble tmp_s_y;
		//	tmp_s_x = s->x;
		//	tmp_s_y = s->y;
			/* プレイヤー狙い弾 */
#if 0
/* CWの場合 */
			src->x256		=	tmp_x256 + ((cos512((ii<<6))*len256)>>8);
			src->y256		=	tmp_y256 + ((sin512((ii<<6))*len256)>>8);
#else
/* CCWの場合 */
			src->x256		=	tmp_x256 + ((sin512((ii<<6))*len256)>>8);
			src->y256		=	tmp_y256 + ((cos512((ii<<6))*len256)>>8);
#endif
			bullet_create_sakuya_follow_knife1/*2*/(src,
				/*speed256*/t256(2.0),
				(ii<<6)/*i*M_PI*2*1/8*/,			/*angle512*/
				-30+(ii&(2-1)/*%2*/)*60);			/* -30 or 30 */
		//	s->x = tmp_s_x;
		//	s->y = tmp_s_y;
		}
	}
}
#endif


/*---------------------------------------------------------
	何がなんだか解からなくなるので、名前をつけよう
---------------------------------------------------------*/

#define SAKUYA_LIMIT_MIN_Y (t256(8))
#define SAKUYA_LIMIT_MAX_Y (t256(GAME_HEIGHT-32))

/* 咲夜さんの誘導ポイント座標 */

#define SAKUYA_POINT_X_LEFT_OUT 	(t256(GAME_WIDTH*0/4))
#define SAKUYA_POINT_X_LEFT_MID 	(t256(GAME_WIDTH*1/4))
#define SAKUYA_POINT_X_MID			(t256(GAME_WIDTH*2/4))/*-src->w/2*/
#define SAKUYA_POINT_X_RIGHT_MID	(t256(GAME_WIDTH*3/4))
#define SAKUYA_POINT_X_RIGHT_OUT	(t256(GAME_WIDTH*4/4))/*-src->w  */
#define SAKUYA_POINT_X_MARGIN		(t256(16))

#define SAKUYA_POINT_Y_LEFT 		(t256(50))
#define SAKUYA_POINT_Y_MID			(t256(10))
#define SAKUYA_POINT_Y_RIGHT		(t256(50))
#define SAKUYA_POINT_Y_CENTER		(t256(50))
#define SAKUYA_POINT_Y_MARGIN		(t256(16))

#define SAKUYA_LIMIT_X_LEFT 		(SAKUYA_POINT_X_LEFT_OUT +SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_LEFT 	(SAKUYA_POINT_X_MID  +SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_RIGHT	(SAKUYA_POINT_X_MID  -SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_RIGHT		(SAKUYA_POINT_X_RIGHT_OUT-SAKUYA_POINT_X_MARGIN)

#define atan_512_SAKUYA_LEFT		atan_512( (SAKUYA_POINT_Y_LEFT) 	-src->y256, (SAKUYA_POINT_X_LEFT_OUT) -src->x256)
#define atan_512_SAKUYA_MID 		atan_512( (SAKUYA_POINT_Y_MID)		-src->y256, (SAKUYA_POINT_X_MID)	  -src->x256)
#define atan_512_SAKUYA_RIGHT		atan_512( (SAKUYA_POINT_Y_RIGHT)	-src->y256, (SAKUYA_POINT_X_RIGHT_OUT)-src->x256)
#define atan_512_SAKUYA_CENTER		atan_512( (SAKUYA_POINT_Y_CENTER)	-src->y256, (SAKUYA_POINT_X_MID)	  -src->x256)

/* 咲夜さんの誘導ポイント名称 */
enum
{
	SAKUYA_POINT_00_LEFT=0, 	/*左下*/
	SAKUYA_POINT_01_MID_UP, 	/*中上*/
//	SAKUYA_POINT_02_CENTER, 	/*画面中心*/
	SAKUYA_POINT_03_RIGHT,		/*右下*/
};


/*---------------------------------------------------------
	move_sakuyaという名前は既に使ってるので注意
---------------------------------------------------------*/

static void sakuya_idou_sub(SPRITE *src, int speed256, int target_point_name)
{
	int b_move_angle512;
	switch (target_point_name)
	{
	case SAKUYA_POINT_00_LEFT:		b_move_angle512 = atan_512_SAKUYA_LEFT;/*50*/	break;
	case SAKUYA_POINT_01_MID_UP:	b_move_angle512 = atan_512_SAKUYA_MID;			break;
//	case SAKUYA_POINT_02_CENTER:	b_move_angle512 = atan_512_SAKUYA_CENTER;		break;
	case SAKUYA_POINT_03_RIGHT: 	b_move_angle512 = atan_512_SAKUYA_RIGHT;/**/	break;
	}
#if 0
/* CWの場合 */
	src->x256 += ((cos512((b_move_angle512))*(speed256))>>8)/**fps_fa ctor*/;
	src->y256 += ((sin512((b_move_angle512))*(speed256))>>8)/**fps_fa ctor*/;
#else
/* CCWの場合 */
	src->x256 += ((sin512((b_move_angle512))*(speed256))>>8)/**fps_fa ctor*/;
	src->y256 += ((cos512((b_move_angle512))*(speed256))>>8)/**fps_fa ctor*/;
#endif
		 if (src->y256 < (SAKUYA_LIMIT_MIN_Y) ) {	src->y256 += t256(1);	}/* はみ出したら修正 */
	else if (src->y256 > (SAKUYA_LIMIT_MAX_Y) ) {	src->y256 -= t256(1);	}/* はみ出したら修正 */
}


/*---------------------------------------------------------
	SPELL_CARD_01_sakuya_aaa	通常攻撃(禊弾幕、原作とはあえて若干違う)	第一形態: 左右に動いて小弾撃ち
---------------------------------------------------------*/

/*static*/ void sakuya_01_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
//	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	static int my_yudo_point;

/* 第一形態では使って無いみたいだから利用する */
//#define _mode wait2_256
	switch (data->state1)
	{
	case SA00:
			data->state1	= SA12; 	/* SA01*/
		break;

	/*完全に姿を現す*/
/*----------------*/
	case SA01:	/* 左へ移動 */
		//if (pd->bo ssmode==B01_BA TTLE)
	//	{
		if (my_yudo_point < src->x256 )/*t256(50)*/
		{
			src->x256 -= t256(2.0/*3.0*/)/*fps_fa ctor*/;
			if (SAKUYA_ANIME_00_LEFT < src->anim_frame)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (SAKUYA_ANIME_04_CENTER_A < data->wait3)
				{
					src->anim_frame--;
					data->wait3 = 0;
				}
			}
		}
		else	/* x が 50 以下になったら */
		{
			data->state1++/*	= SA02*/;
	#if 1/* 可変弾幕 */
			tmp_angle_jikinerai512(player, src);/* 自機ねらい角作成 */
		#if 0
		/*CW*/
			src->tmp_angleCCW512	-= 128;
		#else
		/*CCW*/
			src->tmp_angleCCW512	+= (256-128);
		#endif
	#else/* 固定弾幕 */
		/*CCW*/
			src->tmp_angleCCW512	= deg_360_to_512CCW((90));/* 右側から撃ち始める (128)*/
	#endif
			data->boss_base.danmaku_type		= DANMAKU_01_sakuya;	/* 禊弾幕をセット */
			data->boss_base.danmaku_time_out	= DANMAKU_01_SET_TIME;	/* 禊弾幕の発生時間 */
	//		src->anim_frame 		= SAKUYA_ANIME_04_CENTER_A;
			src->anim_frame 		= SAKUYA_ANIME_15_HATUDAN;
		}
	//	}
		break;
	case SA02:	/* 弾撃ちアニメ待機 */
		sakuya_tama_anime_wait3_0(src);
		break;
	case SA03:	/* 禊弾幕が終わるまで待機 */
		danmaku_state_check_holding(src/*, SA03*/ /*, SAKUYA_ANIME_04_CENTER_A*/);
		break;
	case SA04:
		src->anim_frame 		= SAKUYA_ANIME_04_CENTER_A;
		data->state1++/*	= SA05*/;
		break;
	case SA05:	/* 真ん中に戻る */
		if (SAKUYA_POINT_X_MID > src->x256 )
		{
			src->x256 += t256(3.0/*3.0*/)/*fps_fa ctor*/;
			if (SAKUYA_ANIME_09_RIGHT > src->anim_frame)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (SAKUYA_ANIME_04_CENTER_A < data->wait3)
				{
					src->anim_frame++;
					data->wait3 = 0;
				}
			}
		}
		else	/* 真ん中まで来たら */
		{
			data->state1++/*	= SA04*/;
		//	if (RANK_NORMAL<difficulty) 	{	bullet_create_aka_lines(s); 	}
		}
		break;
/*----------------*/
	case SA06:/* 移動位置を決める */
			data->state1++/*	= SA04*/;
		//	my_yudo_point = SAKUYA_POINT_X_RIGHT_MID;
			my_yudo_point = SAKUYA_POINT_X_MID+(ra_nd()&((128*256)-1));
		break;
/*----------------*/
	case SA07:	/* 右へ移動 */
		if ( my_yudo_point > src->x256/*+((src->w128+src->w128))*/ )/*t256(GAME_WIDTH-50)*/
		{
			src->x256 += t256(2.0/*3*/)/*fps_fa ctor*/;
		}
		else
		{
			data->state1++/*	= SA05*/;
	#if 1/* 可変弾幕 */
			tmp_angle_jikinerai512(player, src);/* 自機ねらい角作成 */
		#if 0
		/*CW*/
			src->tmp_angleCCW512	-= 128;
		#else
		/*CCW*/
			src->tmp_angleCCW512	+= (256-128);
		#endif
	#else/* 固定弾幕 */
		/*CCW*/
			src->tmp_angleCCW512	= deg_360_to_512CCW((90));/* 右側から撃ち始める (128)*/
	#endif
			data->boss_base.danmaku_type		= DANMAKU_01_sakuya;	/* 禊弾幕をセット */
			data->boss_base.danmaku_time_out	= DANMAKU_01_SET_TIME;	/* 禊弾幕の発生時間 */
		//	src->anim_frame 		= SAKUYA_ANIME_04_CENTER_A;
			src->anim_frame 		= SAKUYA_ANIME_15_HATUDAN;
		}
		break;
	case SA08:	/* 弾撃ちアニメ待機 */
		sakuya_tama_anime_wait3_0(src);
		break;
	case SA09:	/* 禊弾幕が終わるまで待機 */
		danmaku_state_check_holding(src/*, SA07*/ /*, SAKUYA_ANIME_04_CENTER_A*/);
		break;
	case SA10:
		src->anim_frame 		= SAKUYA_ANIME_04_CENTER_A;
		data->state1++/*	= SA05*/;
		break;
	case SA11:	/* 真ん中に戻る */
		if (SAKUYA_POINT_X_MID < src->x256 )
		{
			src->x256 -= t256(3.0/*3.0*/)/*fps_fa ctor*/;
			if (SAKUYA_ANIME_00_LEFT < src->anim_frame)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (SAKUYA_ANIME_04_CENTER_A < data->wait3)
				{
					src->anim_frame--;
					data->wait3 = 0;
				}
			}
		}
		else	/* 真ん中に戻る */
		{
			data->state1++/* = SA01*/;
		//	if (RANK_NORMAL<difficulty) 	{	bullet_create_aka_lines(s); 	}
		}
		break;
/*----------------*/
	case SA12:/* 移動位置を決める */
		data->state1	= SA01;
	//	my_yudo_point = SAKUYA_POINT_X_LEFT_MID;
		my_yudo_point = SAKUYA_POINT_X_MID-(ra_nd()&((128*256)-1));
		break;
/*----------------*/

	}
}


/*---------------------------------------------------------
	SPELL_CARD_02_sakuya_bbb	奇術「ミスディレクション」	第二形態: 全方位、豆まき
---------------------------------------------------------*/

/*static*/ void sakuya_02_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;

/* 第二形態では使って無いみたいだから利用する */
#define sayuu_mode wait2_256
	//data->wait2_256		ループ回数
	switch (data->state1)
	{
	case SB00:	/* 初期値 */
		data->wait1 		= 0;/*とりあえず*/
//		data->wait2_256 	= t256(0);
		data->sayuu_mode	= (ra_nd()&(2-1)/*%2*/);
		data->state1++/*	= SB01*/;
		break;

//	case SB00aaa:	/* 待機 */
//		sakuya_wait_state(src, SAKUYA_ANIME_04_CENTER_A);
	//		data->wait1 -= 1/*fps_fa ctor*/;
	//	if (data->wait1<0)
	//	{
	//		data->state1++/*	= SB01*/;
//++//		pd->bo ssmode	= B00_NONE/*B01_BA TTLE*/;
	//	}	//		else	{	}
//		break;

//-----------

	case SB01:
		src->x256 = t256(GAME_WIDTH/2); /* 真中へワープ */
		{
//			int ang512;
			data->sayuu_mode ^= (2-1);/* 反転 */
			if (0==data->sayuu_mode)
			{
				data->move_vx256 = t256(1.0/*0.5*/);
//				ang512=(ra_nd()&(256-1)/*%(180*1.42)*/);
//				if (ang512>deg_360_to_512(90))		{	ang512+=deg_360_to_512(90); 	src->anim_frame=(SAKUYA_ANIME_04_CENTER_A+1); }
//				else								{									src->anim_frame=(SAKUYA_ANIME_04_CENTER_A-1); }
//				ang512 -= deg_360_to_512(45);
			}
			else
			{
				data->move_vx256 = -t256(1.0/*0.5*/);
//				if (src->x256 > t256(GAME_WIDTH/2) )	{	ang512=deg_360_to_512(180); 	src->anim_frame=(SAKUYA_ANIME_04_CENTER_A+2); }
//				else									{	ang512=deg_360_to_512(0);		src->anim_frame=(SAKUYA_ANIME_04_CENTER_A-2); }
			}
//			data->move_angle512=/*deg512_2rad*/(ang512);
		}
//		data->wait1 		= 20+(3-difficulty)*10;
		data->wait1 		= (35);/* [WAITSTATE] */	/* 弾撃たないでちょっと移動 */
//		data->wait2_256 += t256(1);
		//
		src->anim_frame = SAKUYA_ANIME_04_CENTER_A;
		data->wait3=0;
		data->state1++/*	= SB02*/;
		break;

	case SB02:		/* 移動 */
		src->x256 += ((data->move_vx256))/**fps_fa ctor*/;	/* 移動 */	/* 弾撃たないでちょっと移動 */
		sakuya_anime_to_sayuu(src);
		break;
	case SB03:
		src->x256 += ((data->move_vx256))/**fps_fa ctor*/;	/* 移動 */	/* 弾撃たないでちょっと移動 */
		sakuya_wait_state(src/*, SB05*/, SAKUYA_ANIME_15_HATUDAN);
		break;
	case SB04:
		src->anim_frame = SAKUYA_ANIME_15_HATUDAN;
		data->wait3 = 0;
		data->state1++/*	= SB02*/;
		break;
	case SB05:	/* 弾撃ちアニメ待機 */
		sakuya_tama_anime_wait3_0(src);
		break;
	case SB06:
		//	enemy_boss06_mamemaki_state(src, SB07, SAKUYA_ANIME_18_);
			data->boss_base.danmaku_type		= DANMAKU_02_sakuya;	/* 24弾幕をセット */
			data->boss_base.danmaku_time_out	= DANMAKU_02_SET_TIME;	/* 24弾幕の発生時間 */
			data->state1++/*	= SB07*/;
		break;
	case SB07:	/* 24弾幕が終わるまで移動 */
		src->x256 += ((data->move_vx256))/**fps_fa ctor*/;	/* 移動 */	/* 弾撃ちながら移動 */
		danmaku_state_check_holding(src/*, SA07*/ /*, SAKUYA_ANIME_04_CENTER_A*/);
		break;
	case SB08:
			src->x256 = t256(GAME_WIDTH)-src->x256; 			/* 反対側へワープ */
			src->anim_frame = SAKUYA_ANIME_15_HATUDAN;
			data->wait3 = 0;
			data->state1++/*	= SB09*/;
		break;
	case SB09:	/* 弾撃ちアニメ待機 */
		sakuya_tama_anime_wait3_0(src);
		break;
	case SB10:
			data->boss_base.danmaku_type		= DANMAKU_03_sakuya;	/* 11弾幕をセット */
			data->boss_base.danmaku_time_out	= DANMAKU_03_SET_TIME;	/* 11弾幕の発生時間 */
			data->wait1 			= (35);/* (SB12で)少し待機 */
			src->anim_frame 		= SAKUYA_ANIME_04_CENTER_A;
			data->state1++/*	= SB07*/;
		break;
	case SB11:	/* 11弾幕が終わるまで待機 */
		danmaku_state_check_holding(src/*, SA07*/ /*, SAKUYA_ANIME_04_CENTER_A*/);
		break;
	case SB12:	/* (SB12で)少し待機 */
		sakuya_wait_state(src/*, SB05*/, SAKUYA_ANIME_04_CENTER_A);
		break;
	case SB13:
		data->state1	= SB01;
		break;
	}
	enemy_boss06_out(src);
}


/*---------------------------------------------------------
	SPELL_CARD_03_sakuya_ccc	時雨「バーティカルナイフ」		第三形態: 垂直ナイフが落ちてくるよ
---------------------------------------------------------*/

/*static*/ void sakuya_03_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	switch (data->state1)
	{
	case SC00:
		sakuya_wait_state(src, SAKUYA_ANIME_04_CENTER_A);
	//	data->wait1 -= 1/*fps_fa ctor*/;
	//	if (data->wait1<0)	/* 待機時間が終わったら */
	//	{
	//		data->state1++/*	= SC01*/;
//++//		pd->bossmode		= B00_NONE/*B01_BATTLE*/;
	//		src->anim_frame 	= SAKUYA_ANIME_04_CENTER_A;
	//	}
	//	else	{	}
		break;
	case SC01:
		if (src->x256 < (SAKUYA_LIMIT_X_LEFT)/*5*/ )	/* 左隅に来たら */	/* → || (src->y<5)があると無限ループになる*/
		{
			data->state1++/*	= SC02*/;
			data->wait1 	= 10;
			data->wait2_256 = t256(0);
			src->anim_frame = SAKUYA_ANIME_00_LEFT;
		}
		else
		{
			sakuya_idou_sub(src, t256(3), SAKUYA_POINT_00_LEFT);
			if (src->anim_frame>SAKUYA_ANIME_00_LEFT)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					src->anim_frame--;
					data->wait3=0;
				}
			}
		}
		break;
	case SC02:
		sakuya_wait_state(src/*, SC03*/, SAKUYA_ANIME_02_/*???*/);
		break;
	case SC03:
		if (src->x256 > (SAKUYA_LIMIT_X_RIGHT)/*GAME_WIDTH-src->w+3*/ ) /* 右端に来たら */
		{
			data->state1++/*	= SC04*/;
			data->wait2_256 = t256(0);
			src->anim_frame = SAKUYA_ANIME_09_RIGHT;
		}
		else
		{
			{	const short sss[4] =
				{
					t256(1.2),/*easy:まばらなので簡単*/
					t256(0.5/*1.0*/ /*1.5*/),/*normal:塊なので大きく避ければ簡単*/
					t256(1.0/*2.0*/),/*hard:たまに難しい時がある*/
					t256(18.5/*6.5*/ /*12.5*/ /*8.5*/) /*luna:うへぇこのスキマくぐるんですかぁ？*/
					/*(luna:ボムらなくても気合避けでなんとかなる程度に難易度抑えたいのだがまだ高すぎるな)*/
				};
				data->wait2_256 += /*t256(4)*/sss[difficulty]/*t256(3*1.42)*/;
			}
			if (0==(((int)data->wait2_256)&(16-1)/*%21 t256(15*1.42)*/))
			{
#if 0
/* CWの場合 */
				bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(2)/*(2)*/, (deg_360_to_512(360)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.03), BULLET_KNIFE20_05_AKA);
				//if (difficulty)
				{
					bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(2)/*(2)*/, (deg_360_to_512(270)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.04), BULLET_KNIFE20_05_AKA);
				//	if (difficulty>1)
					{	bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(2)/*(2)*/, (deg_360_to_512(180)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02), BULLET_KNIFE20_05_AKA);}
				}
#else
/* CCWの場合 */
				bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(2)/*(2)*/, (-deg_360_to_512CCW(360)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.03), BULLET_KNIFE20_05_AKA);
				//if (difficulty)
				{
					bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(2)/*(2)*/, (-deg_360_to_512CCW(270)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.04), BULLET_KNIFE20_05_AKA);
				//	if (difficulty>1)
					{	bullet_create_jyuryoku_dan000(src, /*(0.7)*/t256(2)/*(2)*/, (-deg_360_to_512CCW(180)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02), BULLET_KNIFE20_05_AKA);}
				}
#endif
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE12_SAKUYA_B2_SHOT);
				#else
				voice_play(VOICE12_SAKUYA_B2_SHOT, TRACK04_TEKIDAN);
				#endif
			}
			sakuya_idou_sub(src, t256(5), SAKUYA_POINT_03_RIGHT);
			if (src->anim_frame<SAKUYA_ANIME_09_RIGHT)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=7)
				{
					src->anim_frame++;
					data->wait3=0;
				}
			}
		}
		break;
	case SC04:
	//	if (( src->x256 < (SAKUYA_LIMIT_X_MID)/*GAME_WIDTH/2-src->w/2*/ ))	/* 真ん中に来たら */
		if (( src->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
			( src->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		/* 真ん中に来たら */
		{
			data->state1++/*	= SC05*/;
			src->anim_frame = SAKUYA_ANIME_04_CENTER_A;
		}
		else
		{
			sakuya_idou_sub(src, t256(3), SAKUYA_POINT_01_MID_UP);
			if (SAKUYA_ANIME_04_CENTER_A < src->anim_frame)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					src->anim_frame--;
					data->wait3=0;
				}
			}
		}
		break;
	case SC05:
		if (src->x256 > (SAKUYA_LIMIT_X_RIGHT)/*GAME_WIDTH-src->w-3*/ ) /* 右端に来たら */ /* → || (src->y<5)があると無限ループになる */
		{
			data->state1++/*	= SC06*/;
			data->wait1 	= 10;
			data->wait2_256 = 0;
			src->anim_frame = SAKUYA_ANIME_09_RIGHT;
		}
		else
		{
			sakuya_idou_sub(src, t256(3), SAKUYA_POINT_03_RIGHT);
			if (src->anim_frame<SAKUYA_ANIME_09_RIGHT)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					src->anim_frame++;
					data->wait3=0;
				}
			}
		}
		break;
	case SC06:
		if (src->x256 < (SAKUYA_LIMIT_X_LEFT)/*5*/) 	/* 左端に来たら */
		{
			data->state1++/*	= SC07*/;
			data->wait2_256 = 0;
			src->anim_frame = SAKUYA_ANIME_00_LEFT;
		}
		else
		{
			data->wait2_256 += t256(4)/*(3*1.42)*/;
			if (0x0000==((data->wait2_256)&(0x0f00))/*%(12*1.42)*/)
			{
#if 0
/* CWの場合 */
				bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512(180)), t256(0.03), BULLET_KNIFE20_05_AKA);
				if (difficulty)
				{
					bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512(90)), t256(0.04), BULLET_KNIFE20_05_AKA);
					if (difficulty>1)
					{	bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02), BULLET_KNIFE20_05_AKA);}
				}
#else
/* CCWの場合 */
				bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512CCW(360-180)), t256(0.03), BULLET_KNIFE20_05_AKA);
				if (difficulty)
				{
					bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512CCW(360-90)), t256(0.04), BULLET_KNIFE20_05_AKA);
					if (difficulty>1)
					{	bullet_create_jyuryoku_dan000(src, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02), BULLET_KNIFE20_05_AKA);}
				}
#endif
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE12_SAKUYA_B2_SHOT);
				#else
				voice_play(VOICE12_SAKUYA_B2_SHOT, TRACK04_TEKIDAN);
				#endif
			}
			sakuya_idou_sub(src, t256(5), SAKUYA_POINT_00_LEFT);
			if (src->anim_frame>SAKUYA_ANIME_00_LEFT)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=7)
				{
					src->anim_frame--;
					data->wait3=0;
				}
			}
		}
		break;
	case SC07:
	//	if (( src->x256 > (SAKUYA_LIMIT_X_MID)/*(GAME_WIDTH/2-src->w/2)*/ ))	/* 真ん中に来たら */
		if (( src->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
			( src->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		/* 真ん中に来たら */
		{
			data->state1++/*	= SC08*/;
			data->wait2_256 = t256(60);
			src->anim_frame = SAKUYA_ANIME_04_CENTER_A;
		}
		else
		{
			sakuya_idou_sub(src, t256(3), SAKUYA_POINT_01_MID_UP);
			if (SAKUYA_ANIME_04_CENTER_A > src->anim_frame)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					src->anim_frame++;
					data->wait3=0;
				}
			}
		}
		break;
	case SC08:
		data->wait2_256 -= t256(1);
		if (data->wait2_256 < 0)
		{
		//	data->state1++/*	= SC01*/;
			data->state1	= SC01;
			src->anim_frame = SAKUYA_ANIME_04_CENTER_A;
		}
		else
		{
//			data->move_angle512=atan_512_SAKUYA_MID;
			if (0 == ((t256_floor(data->wait2_256))&(32-1)/*%20*/) )
			{
				int b_wait2_high;
				b_wait2_high=(((int)((t256_floor(data->wait2_256))>>5/*/20*/)));
			//	src->anim_frame++;	src->anim_frame %= 7;//??? 9???
				src->anim_frame++;	src->anim_frame &= (8-1);
				{
#if 0
/* CWの場合 */
					bullet_create_sakuya_follow_knife1(src, (b_wait2_high<<8)+t256(4.0), (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(40))), -80);
					bullet_create_sakuya_follow_knife1(src, (b_wait2_high<<8)+t256(3.0), (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(30))), -60);
					if (difficulty>1)
					{
						bullet_create_sakuya_follow_knife1(src, (b_wait2_high<<8)+t256(2.0), (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(20))), -40);
					}
#else
/* CCWの場合 */
						bullet_create_sakuya_follow_knife1(src, (b_wait2_high<<8)+t256(4.0), deg_360_to_512CCW(/*360+*/90-30)+(b_wait2_high)*deg_360_to_512CCW(40), -80);
						bullet_create_sakuya_follow_knife1(src, (b_wait2_high<<8)+t256(3.0), deg_360_to_512CCW(/*360+*/90-30)+(b_wait2_high)*deg_360_to_512CCW(30), -60);
					if (difficulty>1)
					{
						bullet_create_sakuya_follow_knife1(src, (b_wait2_high<<8)+t256(2.0), deg_360_to_512CCW(/*360+*/90-30)+(b_wait2_high)*deg_360_to_512CCW(20), -40);
					}
#endif
				}
				src->anim_frame = SAKUYA_ANIME_18_;
			}
		}
		if (src->anim_frame>SAKUYA_ANIME_15_HATUDAN)
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=4)
			{
				src->anim_frame--;
				data->wait3=0;
			}
		}
		break;
//	case SC09:
//			data->state1	= SC01;
//		break;
	}
}


/*---------------------------------------------------------
	第四形態: 魔方陣生成
---------------------------------------------------------*/

/*static*/ void sakuya_04_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	switch (data->state1)
	{
	case SD00:
		sakuya_wait_state(src, SAKUYA_ANIME_15_HATUDAN);
	//	data->wait1 -= 1/*fps_fa ctor*/;
	//	if (data->wait1<0)	/* 待機時間が終わったら */
	//	{
	//		data->state1++/*	= SD01*/;
//++//		pd->bo ssmode=B00_NONE/*B01_BA TTLE*/;
//??//		data->boss_base.boss_timer += (20*64);			// + 20 [秒] ???	/* 16.666[秒] 1000 ???*/
	//		src->anim_frame=SAKUYA_ANIME_15_HATUDAN;
	//	}
	//	else	{	}
		break;
	case SD01:
	//	if ( ( src->y > 50) && (src->x<GAME_WIDTH/2+30 || src->x>GAME_WIDTH/2-30))		/* 真ん中に来たら */
		if (( src->y256 > (SAKUYA_POINT_Y_CENTER-SAKUYA_POINT_Y_MARGIN) ) &&
			( src->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
			( src->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		/* 真ん中に来たら */
		{
			data->state1++/*	= SD02*/;
			data->wait1 	= 10;
			data->wait2_256 = 0;
			src->anim_frame = SAKUYA_ANIME_09_;
		}
		else
		{
			data->move_angle512=atan_512_SAKUYA_CENTER/*atan_512(50-src->y,GAME_WIDTH/2-src->w-src->x)*/;
#if 0
/* CWの場合 */
			src->x256 += ((cos512((data->move_angle512))*t256(3))>>8)/**fps_fa ctor*/;
			src->y256 += ((sin512((data->move_angle512))*t256(3))>>8)/**fps_fa ctor*/;
#else
/* CCWの場合 */
			src->x256 += ((sin512((data->move_angle512))*t256(3))>>8)/**fps_fa ctor*/;
			src->y256 += ((cos512((data->move_angle512))*t256(3))>>8)/**fps_fa ctor*/;
#endif
			if (src->anim_frame<SAKUYA_ANIME_18_)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					src->anim_frame++;
					data->wait3=0;
				}
			}
		}
		break;
	case SD02:
		if (src->anim_frame==SAKUYA_ANIME_14_)
		{	/* 子供魔方陣生成(2回生成しちゃうバグあり) */
		//	kodomo_num++;
		//	if (1==kodomo_num)
		//	{
		//	}
			kodomo_num=1;
			data->state1++/*	= SE00*/;/*dummy*/
		//	data->state1		=	0/* = SE00*/;
#if 0/*???*/
/* スペルカードシステムでは対応できない。 */
			sp ell_card_number++;		/* 次の形態へ */
#endif
			data->wait2_256 	= t256(10);
			data->wait1 		= 5;
		}
		else
		{
			if (src->anim_frame<SAKUYA_ANIME_14_)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					src->anim_frame++;
					data->wait3=0;
				}
			}
		}
		break;
//	case SD03:/*dummy*/
//		break;
	}
}


/*---------------------------------------------------------
	enemy_boss06_knifes1
	アングルは角度指定で(-90)～(90)
	heightは"playerから"の高さ
---------------------------------------------------------*/

#if 0
static void enemy_boss06_knifes1(SPRITE *src, int speed256, int angle512, int height)
{
	/*	*/
	/*	*/
#if 0
/* CWの場合 */
//	bullet_create_sakuya_follow_knife1(src, speed256, /*deg_360_to_512*/(deg_360_to_512((180))-angle512), height);
#else
/* CCWの場合 */
//	bullet_create_sakuya_follow_knife1(src, speed256, /*deg_360_to_512*/(-deg_360_to_512CCW((180))-angle512), height);
	bullet_create_sakuya_follow_knife1(src, speed256, /*deg_360_to_512*/(/*deg_360_to_512CCW(360+90-(180))-*/angle512), height);
#endif
//	bullet_create_sakuya_follow_knife1(src, speed256, /*deg_360_to_512*/(angle512), height);
}
#endif


/*---------------------------------------------------------
	SPELL_CARD_05_sakuya_eee	第五形態: (黄色マスカット弾)
	SPELL_CARD_06_sakuya_fff	第六形態: (黄色マスカット弾)
---------------------------------------------------------*/
/*static*/extern void sakuya_sp1_bullet_create_bbb(SPRITE *src);

/*static*/ void sakuya_06_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	switch (data->state1)
	{
	case SE00:
		tmp_angle_jikinerai512(player, src);
		data->wait2_256 -= t256(1)/*fps_fa ctor*/;
		if (data->wait2_256<t256(0))
		{
			data->wait2_256=t256(50);
//			int *s_data_angle512;
		//	s_data_angle512 = &(((BOSS06_DATA *)src->data)->boss_base.tmp_angleCCW512);
//			s_data_angle512 = &(src->tmp_angleCCW512);
			/* (黄色マスカット弾) */

/* CCWの場合 */
			sakuya_sp1_bullet_create_bbb(src);
			//bullet_create_oodama0(src, t256(2), ANGLE_JIKINERAI_DAN, 40, 0, t256(src->w/2), t256(src->h/2));
			data->wait1 -= 1/*fps_fa ctor*/;
			if (data->wait1<0)	/* 3回撃ったら */
			{
				data->wait1=20;
				if (0==(ra_nd()&(2-1)/*%2*/))
				{
					data->state1++/*	= SE01*/;
					src->anim_frame=(SAKUYA_ANIME_04_CENTER_A-2);
				}
				else
				{
					data->state1/*+=2*/ = SE02;
					src->anim_frame=(SAKUYA_ANIME_04_CENTER_A+2);
				}
			}
		}
		if (src->anim_frame>SAKUYA_ANIME_09_)
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=3)
			{
				data->wait3=0;
				src->anim_frame--;
			}
		}
		break;
	case SE01:
		data->wait1 -= 1/*fps_fa ctor*/;
		if (data->wait1<0)
		{
			data->wait1 	= 3;
			data->state1--/*	= SE00*/;
			data->wait2_256=t256(30);
			src->anim_frame=SAKUYA_ANIME_18_;
		}
		else
		{
			if (src->x256 < t256(100) )
			{
				data->state1++/* = SE02*/;
				src->anim_frame=SAKUYA_ANIME_09_RIGHT-1/*SAKUYA_ANIME_08_RIGHT-1*/;/*7 ???*/
			}
			else
			{	src->x256 -= t256(2)/**fps_fa ctor*/;}
		}
		break;
	case SE02:
		data->wait1 -= 1/*fps_fa ctor*/;
		if (data->wait1<0)
		{
			data->wait1 	= 3;
			data->state1	= SE00;
			data->wait2_256 = t256(30);
			src->anim_frame = SAKUYA_ANIME_18_;
		}
		else
		{
			if (src->x256 > t256(GAME_WIDTH-100)-((src->w128+src->w128)) )
			{
				data->state1--/* = SE01*/;
				src->anim_frame=SAKUYA_ANIME_00_LEFT+1;/*1 ???*/
			}
			else
			{	src->x256 += t256(2)/**fps_fa ctor*/;}
		}
		break;
	}
}


/*---------------------------------------------------------
	SPELL_CARD_07_sakuya_ggg	第七形態: (分散魔方陣)追加計画中
	SPELL_CARD_08_sakuya_hhh	第八形態: (時止めナイフ)追加計画中
---------------------------------------------------------*/

/*---------------------------------------------------------
	SPELL_CARD_09_sakuya_iii	第九形態: 最終形態(その1)
---------------------------------------------------------*/
#if 0
static int sakuya_kyori(SPRITE *src)
{
	int rrr256;
	//	距離を求めるのに２乗してルートを取るのはルート演算が遅いので、工夫が必要かも。
	//	予めテーブル化しておくか、代替演算(CODIC)で求めるか、共通関数にしてCPUのfloat変換命令を最小に留めるか、どれか。
	//	data->length_s_p=sqrt((player->x-src->x)*(player->x-src->x)+(player->y-src->y)*(player->y-src->y));
	{	/* sqrt()遅いので簡易版。というか距離には拘らない事にした */
		int xxx256;
		int yyy256;
		xxx256=abs((int)player->x256-(int)src->x256);/*xxx256+=t256(10);*/ /* abs()は展開してくれるから大丈夫 */
		yyy256=abs((int)player->y256-(int)src->y256);/*yyy256+=t256(10);*/
		if (xxx256>yyy256)	{rrr256=(xxx256/*+(yyy256>>1)*/);}/*少し難しくする*/
		else				{rrr256=(yyy256/*+(xxx256>>1)*/);}/*少し難しくする*/
	}
	if (t256(40) > rrr256)	{	rrr256=t256(40);}	/* 32 10 */
	else
	if (t256(128) < rrr256) {	rrr256=t256(128);}	/* 128 80 */
	return (rrr256);
}
#endif
/*static*/ void sakuya_09_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	switch (data->state1)
	{
	case SG00:
			data->wait1 -= 1/*fps_fa ctor*/;
		if (data->wait1<0)	/* 待機時間が終わったら */
		{
			data->state1++/* = SG01*/;
//++		pd->bo ssmode=B00_NONE/*B01_BA TTLE*/;
			data->move_angle512=atan_512(t256(30)-(src->y256),t256(GAME_WIDTH/2)-((src->w128))-(src->x256));
		}
	//	else		{		}
		break;
	case SG01:
		if ( (src->x256 < t256(GAME_WIDTH/2+30) ) &&
			 (src->x256 > t256(GAME_WIDTH/2-10) ) &&
			 (src->y256 < t256(50)) )	/* 真ん中に来たら */
		{
			data->state1++/*	= SG02*/;
			data->wait1 	= 20;
			data->wait2_256 = 0;
			src->anim_frame=SAKUYA_ANIME_04_CENTER_A;
		}
		else
		{
			/* CWの場合 */
			/* CCWの場合 */
			src->x256 += ((sin512((data->move_angle512))*t256(3))>>8);/**fps_fa ctor*/
			src->y256 += ((cos512((data->move_angle512))*t256(3))>>8);/**fps_fa ctor*/
		}
		break;
	case SG02:
		{
				data->wait1 -= 1/*fps_fa ctor*/;
			if (data->wait1<0)			{	data->state1++/* = SG03*/;
											src->anim_frame=SAKUYA_ANIME_15_HATUDAN;	}		/* 待機時間が終わったら */
		//	else if (data->wait1<6) 	{	src->anim_frame=SAKUYA_ANIME_14_;			}
		//	else if (data->wait1<12)	{	src->anim_frame=SAKUYA_ANIME_13_;			}
		//	else if (data->wait1<18)	{	src->anim_frame=SAKUYA_ANIME_12_;			}
		//	else if (data->wait1<24)	{	src->anim_frame=SAKUYA_ANIME_11_;			}
		//	else if (data->wait1<30)	{	src->anim_frame=SAKUYA_ANIME_10_;			}
		//	else if (data->wait1<36)	{	src->anim_frame=SAKUYA_ANIME_09_;			}
//			else if (data->wait1<36)	{	src->anim_frame=aaa_tbl[data->wait1];		}
	//		else if (data->wait1>56)	{	src->anim_frame=SAKUYA_ANIME_09_;		}
			else if (data->wait1<32)	{	src->anim_frame=SAKUYA_ANIME_09_+((data->wait1&0x38)>>3);		}
		}
		break;
	case SG03:
		if (src->anim_frame==SAKUYA_ANIME_18_)
		{
			{
			//	int rrr256;
			//	/*data->length_s_p256*/rrr256 = sakuya_kyori(src);
		//		bullet_create_sakuya_even_knife(src, t256(10), /*data->length_s_p256*/ /*rrr256,*/ 0/*0==右*/);
		//		bullet_create_sakuya_even_knife(src, t256(10), /*data->length_s_p256*/ /*rrr256,*/ 1/*1==左*/);
				bullet_create_sakuya_even_knife_bbb(src);
			}
			data->wait1=40;
			if (src->y256 > t256(150))
			/* CWの場合 */
		//	{	data->move_angle512=deg_360_to_512(210) + (((ra_nd()&(2048-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
			/* CCWの場合 */
			{	data->move_angle512=deg_360_to_512CCW(360-210) + (((ra_nd()&(2048-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
			else
			{	data->move_angle512=/*deg512_2rad*/((ra_nd()&(512-1)))/*degtorad(ra_nd()%360)*/;}
			#if 1
			/*念の為*/
			mask512(data->move_angle512);
			#endif
			src->anim_frame = SAKUYA_ANIME_16_;/*16???*/
			data->state1++/*	= SG04*/;
		}
		else
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=3)
			{
				src->anim_frame++;
				data->wait3=0;
			}
		}
		break;
	case SG04:
			data->wait1 -= 1/*fps_fa ctor*/;
		if (data->wait1<0)
		{
			data->wait1 	= 120+(3-difficulty)*50;
			data->state1	= SG02;
		}
		else
		{
			/* CCWの場合 */
			src->x256 += ((sin512((data->move_angle512))*t256(3.5))>>8)/**fps_fa ctor*/;
			src->y256 += ((cos512((data->move_angle512))*t256(3.5))>>8)/**fps_fa ctor*/;
		}
		break;
	}
	if ((player->x256+((player->w128)) < src->x256+((src->w128))+t256(25))&&
		(player->x256+((player->w128)) > src->x256+((src->w128))-t256(25)))
	{	data->wait2_256 += t256(1)/*fps_fa ctor*/;}
	if (data->wait2_256 > t256(20)+(3-difficulty)*t256(30))
	{
		src->anim_frame=SAKUYA_ANIME_16_;/*16???*/
	//	data->length_s_p256 = sakuya_kyori(src);
		enemy_boss06_knifes3(src/*,*/ /*1+difficulty,*/ /*(data->length_s_p256)*/ );
	}
	enemy_boss06_out(src);
}
		//	const Uint8 aaa_tbl[40/*(6*6)*/] =
		//	{
		//		SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,
		//		SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,
		//		SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,
		//		SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,
		//		SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,
		//		SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,
		//		0,0,0,0/* アライン用ダミーデーター。 dummy for align*/
		//	};
//			const Uint8 aaa_tbl[40/*(6*6)*/] =
//			{
//	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,
//	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,
//	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,
//	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,
//	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,
//	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,
//	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,
//	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,//32
//	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,
//	0,0,0,0/* アライン用ダミーデーター。 dummy for align*/
//			};


/*---------------------------------------------------------
	SPELL_CARD_10_sakuya_jjj	第10形態: 最終形態(その2)
---------------------------------------------------------*/

/*static*/ void sakuya_10_keitai(SPRITE *src)
{
	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	if (data->wait2_256 < t256(0) )
	{
		data->wait2_256 = t256(0);
		data->wait1 	= 120;
#if 0
/* スペルカードシステムでは対応できない。 */
		sp ell_card_number = SPELL_CARD_09_sakuya_iii/*4*/;
#endif
		spell_card_number++;
		data->state1	= SG02;
		src->anim_frame = SAKUYA_ANIME_04_CENTER_A;
	}
	else
	{
		data->wait2_256 -= t256(0.03);
		if (src->color32 > 0xf9000000)	/*	if (src->alpha > 0xf9)	*/	/* (src->alpha>0xff)*/
		{
			src->color32 = 0xffffffff;	/*	src->alpha = 0xff;*/
		}
		else /*if (src->alpha<0xff)*/
		{
			src->color32 += 0x05000000; /*	src->alpha += 0x05;*/
		}
		/* CWの場合 */
		/* CCWの場合 */
		src->x256 += ((sin512((src->tmp_angleCCW512))*(data->wait2_256))>>8)/**fps_fa ctor*/;
		src->y256 += ((cos512((src->tmp_angleCCW512))*(data->wait2_256))>>8)/**fps_fa ctor*/;
	}
	if (src->anim_frame>SAKUYA_ANIME_15_HATUDAN)
	{
		data->wait3 += 1/*fps_fa ctor*/;
		if (data->wait3 >= 4)
		{
			src->anim_frame--;
			data->wait3 = 0;
		}
	}
	enemy_boss06_out(src);
}

/*---------------------------------------------------------
	SPELL_CARD_11_sakuya_kkk	第10形態: 最終形態(その3)
---------------------------------------------------------*/

/*static*/ void sakuya_11_keitai(SPRITE *src)
{
}

/*---------------------------------------------------------
	ボス行動
---------------------------------------------------------*/

static void move_sakuya(SPRITE *src)
{
	/* スペカ登録 */
	/* 魔方陣生成中はoff */
	{
	//	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
	//	if (SPELL_CARD_04_sakuya_ddd != spell_card_number)	/* 第四形態: 魔方陣生成 */
		{
			if (0/*off*/==spell_card_mode)
			{
				regist_spell_card222(src);
			}
		}
	}
	spell_card_generator222(src);	/* スペカ生成 */
	#if 1/* [スペカシステム内に移動予定] */
	#endif
	boss_effect(src);			/* 回エフェクト */
	danmaku_generator(src); 	/* 弾幕生成 */
//
	#if 0/*十六夜本人デバッグ用*/
	/* パネルのスコア欄にmove_typeを、グレイズ欄にstate1を表示させる。っていうか書き換えちゃう。 */
	((PLAYER_DATA *)player->data)->my_score 	= sp ell_card_number;
	((PLAYER_DATA *)player->data)->graze_point	= data->state1;
	#endif
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_boss_sakuya(STAGE_DATA *l)/*int lv*/
{
	boss_bgm_mode		= (l->user_y);
//
//----[ZAKO]
	common_boss_flags	= (FLG_ALL_CAST);
//
	kodomo_num	= 0;

//
//	dummy_obj								= sprite_add_res(BASE_BOSS_SAKUYA_PNG);
	dummy_obj->flags						&= (~(SP_FLAG_VISIBLE));
	dummy_obj->anim_frame					= SAKUYA_ANIME_04_CENTER_A;
	dummy_obj->type 						= SP_MUTEKI;
//	dummy_obj->callback_mover				= dummy_move;
	dummy_obj->x256 						= 0;
	dummy_obj->y256 						= 0;

//
//----[BOSS]
		SPRITE *sakuya;
	//	sakuya								= spr ite_add_file 0("boss/sakuya.png"/*"boss04.png"*/, 19, PRIORITY_03_ENEMY, 0); sakuya->anim_speed=0;
		sakuya								= sprite_add_res(BASE_BOSS_SAKUYA_PNG);
		sakuya->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
		sakuya->yx_anim_frame				= SAKUYA_ANIME_04_CENTER_A;
		sakuya->type						= SP_BOSS/*SP_BOSS06*/;
		sakuya->callback_mover				= move_sakuya;
		sakuya->callback_loser				= sakuya_put_items;
//
		sakuya->base_health 				= ((1024)-1)*9;/*test*/
		sakuya->base_score					= adjust_score_by_difficulty(score(5000000));	/* 500万 */
	//------------ スペカ関連
		spell_card_number					= SPELL_CARD_00_sakuya_000;
		spell_card_max						= SPELL_CARD_11_sakuya_kkk;
		spell_card_boss_init_regist(sakuya);
	{
		BOSS06_DATA *data;
		data								= mmalloc(sizeof(BOSS06_DATA));
		sakuya->data						= data;
		data->state1						= /*ST_00*/0/*SA00*/;
		data->wait1 						= 0;
		data->wait2_256 					= t256(0);
		data->wait3 						= 0;
	//	data->aaa_type						= 0;
		data->move_angle512 				= (0);
		#if 1
	//------------ 弾幕関連
		data->boss_base.danmaku_type		= 0;
		data->boss_base.danmaku_time_out	= 0;
		#endif
	}
	//----[ZAKO]
	//	create_doll(src);		/* 子供魔方陣生成(2回生成しちゃうバグあり) */

	/*---------------------------------------------------------
		子供魔方陣 生成 	子供魔方陣グラフィック生成
	---------------------------------------------------------*/

	//static void create_doll(SPRITE *sakuya)
	{
//		obj_doll							= sprite_add_res(BASE_MAHOUJIN_0_PNG);	//obj_doll->anim_speed	= 0;/*"boss04-lo.png"*/
		obj_doll							= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG); //obj_doll->anim_speed	= 0;/*"boss04-lo.png"*/
		obj_doll->type						= TEKI_51_MAHOJIN1;
//		obj_doll->type						= SP_MUTEKI;
		obj_doll->base_health				= 999999;/* 倒せない */
		obj_doll->flags 					|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
		obj_doll->anim_frame				= 0;
		obj_doll->color32					= 0x00ffffff;	/*	obj_doll->alpha 		= 0x00;*/
		obj_doll->callback_mover			= move_doll00;
		obj_doll->x256						= sakuya->x256+((sakuya->w128-obj_doll->w128));
		obj_doll->y256						= sakuya->y256+((sakuya->h128-obj_doll->h128));

		{
			BOSS06_DOLL_DATA *data;
			data							= mmalloc(sizeof(BOSS06_DOLL_DATA));
			obj_doll->data					= data;
			data->state_d02 				= 0;
			data->state_d01 				= 0x00;
			data->wait						= 0;
			data->angle512					= (0);
			#if (1==USE_KEISYOU)
			data->sakuya_obj				= sakuya;	/* 継承させる */
			#endif
			data->nnn						= 0;
			data->hen_no_kazu				= (DOLL_HEN_MAX);
		}
	}

}
//		spell_card_boss_timer				= (20*64);			/*1000*/	// + 20 [秒] ???	/* 16.666[秒] 1000 ???*/
//		spell_card_boss_timer				= (40*64);			/* 40*64==40[count] 	約40[秒(64/60)](単位は秒ではない) */
//		spell_card_boss_timer				= ((99-24)*64); 	/* 75*64==75[count] 	約75[秒(64/60)](単位は秒ではない) */
	/*score(5000)+score(4000)*difficulty*/

//		data->boss_base.bo ss_life			= ((/*5*/SPELL_CARD_10_sakuya_jjj));			/*5119==((5*1024)-1)*/
//		sakuya->base_health 				= ((/*5*/SPELL_CARD_10_sakuya_jjj*1024)-1); 	/*5119==((5*1024)-1)*/
//		sakuya->base_health 				= ((1024)-1);
