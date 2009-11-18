
#include "bullet_object.h"

/*---------------------------------------------------------
	射命丸 文
	天狗
	-------------------------------------------------------
	ToDo:
	ボスタイマー対応中
	-------------------------------------------------------
	自分で思ったよりも第2形態の攻撃パターンがカオスになってしまった。
	もっと丁寧に作り直した方がいいかも。
	[*** 090114 	天狗の速度を難易度ごとに変えてみた。
---------------------------------------------------------*/

typedef struct /*_boss02_data*/
{
	BOSS_BASE boss_base;
//------------ 移動関連
	int state1; 		/* 形態ステート */	/* 弾幕ステート */
	int aya_angle512;	/* 文の角度 */

	int wait1;			/* 移動待ち */
	int animation_wait; 		/* アニメーション用 */

	int n_wait3;
	int aya_speed;		/*	難度別速度 */
	int ice_number; 	/*	ICE弾幕番号 */
//
} BOSS02_DATA;

/*---------------------------------------------------------
	[スペカシステム内に移動予定]	スペカ撃破後アイテム出す
---------------------------------------------------------*/

static void aya_put_items(SPRITE *src)
{
	common_boss_put_items(src);
//
	bakuhatsu_add_circle(/*d*/src, 1);
//
	{
		BOSS02_DATA *data=(BOSS02_DATA *)src->data;
		data->ice_number++;
	//	if (DT02 < data->ice_number)	{	data->ice_number = DT02;	}
		data->state1	= 0;
		data->wait1 	= 40;
	//	player_add_score(data->boss_base.score);
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
//#define AYA_ANIME_LEFT_00 (0)
//#define AYA_ANIME_CENTER_04 (4)
//#define AYA_ANIME_RIGHT_08	(8)

static void sakuya_wait_state(SPRITE *src/*, int next_set_state*/)
{
	BOSS02_DATA *data = (BOSS02_DATA *)src->data;
	if (data->wait1 > 0)
	{
		data->wait1 -= 1/*fps_fa ctor*/;
		data->animation_wait += 1/*fps_fa ctor*/;
		if (7 < data->animation_wait)
		{
			data->animation_wait = 0;
		//	if (src->anim_frame<AYA_ANIME_CENTER_04)		{	src->anim_frame++;		}
		//	else if (src->anim_frame>AYA_ANIME_CENTER_04)	{	src->anim_frame--;		}
			u8 aaa = (src->anim_frame & 0x03);
			if (0!=(aaa))
			{
				aaa--;
				src->anim_frame = (src->anim_frame & 0xfc)|(aaa);
			}
		}
	}
	else
	{
		data->state1++/* = next_set_state*/;
/* CCWの場合 */
		dummy_obj->w128 = src->w128;
		dummy_obj->h128 = src->h128;
		dummy_obj->x256 = src->x256 + (src->w128);	/* 本物は発弾位置の offset 用 */
		dummy_obj->y256 = src->y256 + (src->h128);	/* 本物は発弾位置の offset 用 */

		bullet_create_oodama00(dummy_obj,
			t256(4)+(difficulty<<7),
		//	t256(1.0)+(difficulty<<4),	/*仕様変更*/	/*t256(4)+(difficulty<<7)*/
			ANGLE_JIKI_NERAI_DAN,
			10,
			0
		);
	}
}
/*---------------------------------------------------------

---------------------------------------------------------*/
static void sakuya_anime08(SPRITE *src)
{
	BOSS02_DATA *data = (BOSS02_DATA *)src->data;
			u8 aaa = (src->anim_frame & 0x03);
		//	if (src->anim_frame<AYA_ANIME_RIGHT_08)
			if (3!=aaa)
			{
				data->animation_wait += 1/*fps_fa ctor*/;
				if (7 < data->animation_wait)
				{
					data->animation_wait=0;
				//	src->anim_frame++;
					aaa++;
					src->anim_frame = /*(src->anim_frame & 0xfc)*/(0x00)|(aaa);
				}
			}
}
/*---------------------------------------------------------

---------------------------------------------------------*/
static void sakuya_anime00(SPRITE *src)
{
	BOSS02_DATA *data = (BOSS02_DATA *)src->data;
			u8 aaa = (src->anim_frame & 0x03);
		//	if (src->anim_frame>AYA_ANIME_LEFT_00)
			if (3!=aaa)
			{
				data->animation_wait += 1/*fps_fa ctor*/;
				if (7 < data->animation_wait)
				{
					data->animation_wait=0;
				//	src->anim_frame--;
					aaa++;
					src->anim_frame = /*(src->anim_frame & 0xfc)*/(0x10)|(aaa);
				}
			}
}
/*---------------------------------------------------------

---------------------------------------------------------*/
enum
{
	ST_00 = 0,
	ST_01,
	ST_02,
	ST_03,
	ST_04,
	ST_05,
	ST_06,
	ST_07,
	ST_08,
	ST_09,
	ST_10,
	ST_11,
	ST_12,
};
enum
{
	SS00 = 0,
	SS01,
	SS02,
	SS03,
	SS04,
	SS05,
	SS06,
	SS07,
	SS08,
	SS09,
	SS10,
	SS11,
	SS12,
};

/*---------------------------------------------------------

---------------------------------------------------------*/

static void bullet_create_aya_momiji(SPRITE *src)
{
#if 0
/* CWの場合 */
	bullet_create_momiji_dan(src, t256(3.5), deg_360_to_512((30*(3-2))) );
	bullet_create_momiji_dan(src, t256(4.0), deg_360_to_512((30*(3+0))) );
	bullet_create_momiji_dan(src, t256(3.5), deg_360_to_512((30*(3+2))) );
#else
/* CCWの場合 */
//	bullet_create_momiji_dan(src, t256(3.5), -deg_360_to_512CCW((30*(3-2))) );
//	bullet_create_momiji_dan(src, t256(4.0), -deg_360_to_512CCW((30*(3+0))) );
//	bullet_create_momiji_dan(src, t256(3.5), -deg_360_to_512CCW((30*(3+2))) );
//2016201
	bullet_create_momiji_dan(src, t256(3.5), deg_360_to_512CCW(360+90-(30*(3-2))) );
	bullet_create_momiji_dan(src, t256(4.0), deg_360_to_512CCW(360+90-(30*(3+0))) );
	bullet_create_momiji_dan(src, t256(3.5), deg_360_to_512CCW(360+90-(30*(3+2))) );
#endif
}

/*---------------------------------------------------------
	自機狙い弾の角度を計算
---------------------------------------------------------*/
static int angle_jikinerai512(SPRITE *p, SPRITE *t)
{
	#if 1
		return (atan_512(p->y256-t->y256				 ,p->x256-t->x256				  ));//2082417					/* 偶数弾の場合に自機狙い(簡略版) 弾と自分が大きさが同じならずれない、違うとその分誤差になる */
	//	return (atan_512(p->y256-t->y256+((p->h128-t->h128)),p->x256-t->x256-((p->w128+t->w128))));/*???(original)*/	/* 奇数弾の場合に自機狙い(?) なんか左にずれる */
//		return (atan_512(p->y256-t->y256+((p->h128-t->h128)),p->x256-t->x256-((p->w128-t->w128))));/**/ 				/* 奇数弾の場合に自機狙い */
	//	return (atan_512(p->y256-t->y256+((p->h128-t->h128)),p->x256-t->x256+((p->w128-t->w128))));/**/ 				/* 偶数弾の場合に自機狙い */
	//	return (atan_512(p->y256-t->y256+((p->h128	)),p->x256-t->x256-((p->w128	 ))));
	//	return (atan_512(p->y256-t->y256-((p->h128	)),p->x256-t->x256-((p->w128	 ))));
	#endif
}

/*---------------------------------------------------------

---------------------------------------------------------*/

extern void bullet_create_aya_ice(SPRITE *src);

static int bullet_create_oodama3(SPRITE *src, /*仕様変更*/int speed256, int ransu512)
{	int angle_jikinerai_512;
//	angle_jikinerai_512 = at an_512(player->y256-src->y256,player->x256-src->x256);
	angle_jikinerai_512 = angle_jikinerai512(player,src);
/* CCWの場合 */
		dummy_obj->w128 = src->w128;
		dummy_obj->h128 = src->h128;
		dummy_obj->x256 = src->x256 + ((src->w128*86*2)>>8);	/* 本物は発弾位置の offset 用 */
		dummy_obj->y256 = src->y256 + (src->h128);	/* 本物は発弾位置の offset 用 */
//		dummy_obj->tmp_angleCCW512 = (0);/* 下向き */
	bullet_create_oodama00(dummy_obj, speed256, angle_jikinerai_512+deg_360_to_512CCW((30)), ransu512, 0 ); /*t256(s->w/3), t256(s->h/2)*/
	bullet_create_oodama00(dummy_obj, speed256, angle_jikinerai_512 					   , ransu512, 0 ); /*t256(s->w/3), t256(s->h/2)*/
	bullet_create_oodama00(dummy_obj, speed256, angle_jikinerai_512-deg_360_to_512CCW((30)), ransu512, 0 ); /*t256(s->w/3), t256(s->h/2)*/
	return (angle_jikinerai_512);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void bullet_create_kougeki_01(SPRITE *src)
{	BOSS02_DATA *data=(BOSS02_DATA *)src->data;
//	if (DT00==data->ice_number)
//	if (2 < ((data->ice_number+1) & 0x03/*DT00*/))
	{
		const unsigned char aaa_bbb[8] =
		{
			0,0,1,0, 0,1,0,1,
		};
		if (0 != aaa_bbb[(data->ice_number&7)/*+8*/])
		{	bullet_create_aya_ice(src);}
	}
};
static void bullet_create_kougeki_02(SPRITE *src)
{	BOSS02_DATA *data=(BOSS02_DATA *)src->data;
//	if (DT01==data->ice_number)
	if (0!=(data->ice_number & 1/*DT01*/))
	{	bullet_create_aya_momiji(src);}
};
/*
000 0 --
001 1
010 2
011 3 --
100 4

*/

/*---------------------------------------------------------
	何がなんだか解からなくなるので、名前をつけよう
---------------------------------------------------------*/

/* 咲夜さんの誘導ポイント座標 */

#define SAKUYA_POINT_X_LEFT_OUT 	(t256(GAME_WIDTH*0/4))
#define SAKUYA_POINT_X_LEFT_MID 	(t256(GAME_WIDTH*1/4))
#define SAKUYA_POINT_X_MID			(t256(GAME_WIDTH*2/4))/*-src->w/2*/
#define SAKUYA_POINT_X_RIGHT_MID	(t256(GAME_WIDTH*3/4))
#define SAKUYA_POINT_X_RIGHT_OUT	(t256(GAME_WIDTH*4/4))/*-src->w  */
#define SAKUYA_POINT_X_MARGIN		(t256(25/*16*/))


#define SAKUYA_LIMIT_X_LEFT 		(SAKUYA_POINT_X_LEFT_OUT +SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_LEFT 	(SAKUYA_POINT_X_MID  +SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_RIGHT	(SAKUYA_POINT_X_MID  -SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_RIGHT		(SAKUYA_POINT_X_RIGHT_OUT-SAKUYA_POINT_X_MARGIN)

//(t256(GAME_WIDTH)-((src->w128+src->w128)))

/*---------------------------------------------------------
	移動パターン
---------------------------------------------------------*/
extern const u8 alice_danmaku_table[16];
/*static*/ void aya_01_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait -= (1)/*fps_fa ctor*/;
//
	BOSS02_DATA *data = (BOSS02_DATA *)src->data;
	{
		if (my_wait <= 0)
		{	/* 移動方向を決める */
			data->state1++;
			data->state1 &= (8-1);
			{
				enum
				{
					dummy_PPP_00_VX = 0,	/* x ベクトル移動量 */
					PPP_01_VY,				/* y ベクトル移動量 */
					PPP_02_WAIT_DIV_2,		/* ウェイトカウンタの半分量 */
					PPP_03_IS_RESET_ANIME,	/* アニメーションリセット 0:しない 1:する 2:特別(弾幕撃ち) */
				};
				s8 ppp[8][4] =
				{
					{( 2),(-1),(100),( 1),},	/*右上へ*/
					{( 0),( 0),( 50),( 2),},	/*wait*/
					{( 2),( 1),(100),( 1),},	/*右下へ*/
					{( 0),( 0),( 10),( 0),},	/*wait*/
					{(-2),(-1),(100),( 1),},	/*左上へ*/
					{( 0),( 0),( 50),( 2),},	/*wait*/
					{(-2),( 1),(100),( 1),},	/*左下へ*/
					{( 0),( 0),( 10),( 0),},	/*wait*/
				};
//				data->vx	= ppp[data->state1][PPP_00_VX];
//				data->vy	= ppp[data->state1][PPP_01_VY];
//				src->vx256	= (0);
//				src->vy256	= ((ppp[data->state1][PPP_01_VY])<<4);
				my_wait 	= ppp[data->state1][PPP_02_WAIT_DIV_2]; 	/* 50*4 60 移動量 */
				my_wait 	+= my_wait;
				if (0!=ppp[data->state1][PPP_03_IS_RESET_ANIME])
				{
					if (2==ppp[data->state1][PPP_03_IS_RESET_ANIME])	/* 攻撃アニメーション */
					{
//						alice_anime_count = 48;
						data->boss_base.danmaku_test++;
						data->boss_base.danmaku_test &= 0x07;
						data->boss_base.danmaku_type		= alice_danmaku_table[data->boss_base.danmaku_test];	/*DANMAKU_01*/	/* 禊弾幕をセット */
						data->boss_base.danmaku_time_out	= (DANMAKU_01_SET_TIME+DANMAKU_01_SET_TIME);	/* 禊弾幕の発生時間 x 2 */
					}
//					vvv256=1;
				}
			}
		}
	}
//	src->vx256 = (data->vx)*vvv256;
//	src->vy256 = (data->vy)*vvv256;
}



/*---------------------------------------------------------
	移動パターン1(低速移動)
---------------------------------------------------------*/

/*static*/ void aya_04_keitai(SPRITE *src)
{
	BOSS02_DATA *data = (BOSS02_DATA *)src->data;
	switch (data->state1)
	{
	case ST_00: /* y-pos erreicht: rundumschuss */
		#if (1==USE_CONTROLLER)
		enemy_boss02_sr_add1(src);
		#endif //(1==USE_CONTROLLER)
		bullet_create_kougeki_01(src);
		bullet_create_kougeki_02(src);
		data->wait1 = 20;
		data->state1++/* = ST_01*/;
		break;

	case ST_01: /* waiting */
		sakuya_wait_state(src/*,ST_02*/);
		break;

	case ST_02: /* nach links bis zum rand */
		if (src->x256 >= t256(0))
		{
			src->x256 -= t256(2)/**fps_fa ctor*/;
			sakuya_anime00(src);
		}
		else
		{
			data->state1++/* = ST_04*/;
		}
		break;
	case ST_03: /*	*/
		{
			data->state1++/* = ST_04*/;
			data->wait1 = 20;
			bullet_create_kougeki_02(src);
		}
		break;


	case ST_04: /* waiting */
		sakuya_wait_state(src/*,ST_05*/);
		break;

	case ST_05: /* nach rechts bis zur mitte */
		if (src->x256 < BOSS_XP256)
		{
			src->x256 += t256(2)/**fps_fa ctor*/;
			sakuya_anime08(src);
		}
		else
		{
			#if (1==USE_CONTROLLER)
			enemy_boss02_sr_add1(src);
			#endif //(1==USE_CONTROLLER)
			data->wait1 = 100;
			data->state1++/* = ST_06*/;
			bullet_create_kougeki_02(src);
		}
		break;

	case ST_06: /* waiting */
		sakuya_wait_state(src/*,ST_07*/);
		break;

	case ST_07:
		bullet_create_kougeki_01(src);
		data->wait1 = 100;
		data->state1++/* = ST_08*/;
		break;

	case ST_08: /* waiting */
		sakuya_wait_state(src/*,ST_09*/);
		break;

	case ST_09: /* nach rechts bis zum rand */
		if (src->x256 < (t256(GAME_WIDTH)-((src->w128+src->w128))))
		{
			src->x256 += t256(2)/**fps_fa ctor*/;
			sakuya_anime08(src);
		}
		else
		{
			data->wait1 = 20;
			data->state1++/* = ST_10*/;
			bullet_create_kougeki_02(src);
		}
		break;

	case ST_10: /* waiting */
		sakuya_wait_state(src/*,ST_11*/);
		break;

	case ST_11: /* nach links bis zur mitte */
		if (src->x256 > BOSS_XP256)
		{
			src->x256 -= t256(2)/**fps_fa ctor*/;
			sakuya_anime00(src);
		}
		else
		{	data->state1++/* = ST_00*/; }
		break;
	case ST_12: /*	*/
		{
			data->state1 = ST_00;
		}
		break;
	}
}
/*---------------------------------------------------------
	移動攻撃パターン2(高速移動)
---------------------------------------------------------*/

/*static*/ void aya_05_keitai(SPRITE *src)
{
	BOSS02_DATA *data = (BOSS02_DATA *)src->data;
	switch (data->state1)
	{
	case SS00:	/* 不定:初期位置情報の取得->SS01へ */
		dummy_obj->x256 = t256(GAME_WIDTH/2);
		dummy_obj->y256 = t256(20);
	//	data->aya_angle512 = at an_512(dummy_obj->y256-src->y256,dummy_obj->x256-src->x256-((src->w128+src->w128)));
		data->aya_angle512 = angle_jikinerai512(dummy_obj,src);
		data->state1++/* = SS01*/;
		break;

	case SS01:	/* 不定:初期位置へ戻る->SS02へ */
		if (src->y256 < t256(30))
		{
			data->state1++/* = SS02*/;
			data->wait1 += 10+(3-difficulty)*10;
/*
[0] 1  2 [0] 1	2 [0] 1  2 [0] 1  2 				/ 3回に一回
<0> 1  2  3 [0] 1  2  3 [0] 1  2  3 				/ 4回に一回
<0> 1  2  3  4	5  6  7  8	9 10 11 				/ 12回に一回以外
[-][-][-][-][+][+][+][+][-][-][-][-][+][+][+][+]
[0] 1  2 [0] 1	2 [0] 1 [0] 1  2 [0] 1	2 [0] 1 	/ 3回に一回
 0 [1] 2  3  0 [1] 2  3  0 [1] 2  3  0 [1] 2  3 	/ 4回に一回
 0	1  2  3  4	5  6  7  8	9 10 11 12 13 14 15 	/ 16回
*/
			{	static const Uint8 n_wait3_tbl[8/*16*/] =
				{ 1,2,0,1, 0,2,1,0, /*1,2,0,1, 0,2,1,0,*/ };
				/* 3回に一回   (0 == ((data->n_wait3)%3)) */
				data->n_wait3++;
				data->n_wait3 &= 0x07;
				if (n_wait3_tbl[data->n_wait3]&0x01)
				{
					/* 弾に毒塗ってある設定 */
					bullet_create_n_way_dan_sa_type(src, t256(5), ANGLE_JIKI_NERAI_DAN, (int)(512/24), BULLET_KOME_01_AOI+(ra_nd()&0x0f),	 8);	/*なるべく共通化*/
				}
				/* 4回に一回   (0 == ((data->n_wait3)%4)) && */
				/* 但し12回に一回以外  (0 != ((data->n_wait3)%12)) */
				if (n_wait3_tbl[data->n_wait3]&0x02)
				{
					/* 弾に毒塗ってある設定 */
					bullet_create_n_way_dan_sa_type(src, t256(4), ANGLE_JIKI_NERAI_DAN, (int)(512/24), BULLET_KOME_01_AOI+(ra_nd()&0x0f)/*BULLET_KUNAI12_00_AOI*/, 11); /*なるべく共通化*/
					bullet_create_n_way_dan_sa_type(src, t256(3), ANGLE_JIKI_NERAI_DAN, (int)(512/24), BULLET_KOME_01_AOI+(ra_nd()&0x0f)/*BULLET_KUNAI12_02_MIDORI*/, 11); /*なるべく共通化*/
				}
			}
		}
		else
		{
#if 1
/* CCWの場合 */
			src->x256 += ((sin512((data->aya_angle512))*t256(6))>>8)/**fps_fa ctor*/;
			src->y256 += ((cos512((data->aya_angle512))*t256(6))>>8)/**fps_fa ctor*/;
#endif
			data->state1--/* = SS00*/;
			#if 1/*差分氏互換*/
			data->n_wait3--;
			#endif
		}
		break;

	case SS02:	/* 初期位置:大弾3つ->SS03, SS04, SS05 */
		if (data->wait1<0)
		{	int angle_jikinerai_512;
			angle_jikinerai_512 = bullet_create_oodama3(src,	/*仕様変更*/t256(/*1.5*/5.0/*5.0*/), 1/*ransu512*/);
		//	data->state1 = (ra_nd()%3)+SS03;
			data->state1 = (((ra_nd()&(256-1))*3)>>8)+SS03;
			if (data->state1 == SS05)
			{
				data->aya_angle512 = angle_jikinerai_512;
			}
			else
			{
				src->y256 += t256(6)/**fps_fa ctor*/;
			}
		}
		else
		{
			data->wait1 -= 1/*fps_fa ctor*/;
		}
		break;

	case SS03:	/* 左移動中->SS06へ */
		if (src->x256 >= 0)
		{
			src->x256 -= /*(t256(4)+(difficulty<<8))*/(data->aya_speed)/**fps_fa ctor*/;
			sakuya_anime00(src);
		}
		else
		{
			src->x256 += t256(1);		// [***090114		追加
			data->wait1 = 10;
			data->state1 = SS06;
		}
		break;

	case SS04:	/* 右移動中->SS08へ */
		if (src->x256 < (t256(GAME_WIDTH)-((src->w128+src->w128))))
		{
			src->x256 += /*(t256(4)+(difficulty<<8))*/(data->aya_speed)/**fps_fa ctor*/;
			sakuya_anime08(src);
		}
		else
		{
			src->x256 -= t256(1);		// [***090114		追加
			data->wait1 = 10;
			data->state1 = SS08;
		}
		break;

	case SS05:	/* プレイヤー位置付近移動中->SS10へ */
		/* ayaの稼動範囲 */
		#if 0
		/* 画面を喰み出た場合に戻る(ボス当たりありにしたので復活時に踏まれて困る) */
		if (((src->x256 < t256(0))||(src->y256 < t256(0)))||
			((src->x256+((s->w128+src->w128)) > t256(GAME_WIDTH))||
		//	 (src->y256+((s->h128+src->h128)) > t256(GAME_HEIGHT))))
			 (src->y256+((s->h128+src->h128)) > t256(220))))/*(踏まれない)*/
		#else
		/* 稼動範囲を喰み出た場合に戻る(ボス当たりありにしたので復活時に踏まれて困る) */
		if (((src->x256 < t256(0))||(src->y256 < t256(0)))||
			((src->x256+((src->w128+src->w128)) > t256(GAME_WIDTH))||
		//	 (src->y256+((src->h128+src->h128)) > t256(GAME_HEIGHT))))
			 (src->y256+((src->h128+src->h128)) > (t256(128)+(difficulty<<(8+5)))  )))	/* difficulty x 32 */
		/*
			easy:	128(踏まれない)
			normal: 160(踏まれない)
			hard:	192(踏まれない)
			luna:	224(踏まれる)
		*/
		#endif
		{
			data->state1 = SS10;
		}
		else
		{
#if 1
/* CCWの場合 */
			src->x256 += ((sin512((data->aya_angle512))*(data->aya_speed)/*(t256(4)+(difficulty<<8))*/)>>8)/**fps_fa ctor*/;
			src->y256 += ((cos512((data->aya_angle512))*(data->aya_speed)/*(t256(4)+(difficulty<<8))*/)>>8)/**fps_fa ctor*/;
#endif
		}
		break;
//--------
	case SS06:	/* 左:待機->SS07へ */
		sakuya_wait_state(src/*,SS07*/);
		break;

	case SS07:	/* 左:右回転攻撃->SS00, SS11 */
		#if (1==USE_CONTROLLER)
		enemy_boss02_sr_add1(src);
		#endif //(1==USE_CONTROLLER)
		if (0==(ra_nd()&(2-1)))
		{
			data->state1 = SS00;/*	data->n_wait3++;*/
		}
		else
		{	data->state1 = SS11;}
		break;
//--------
	case SS08:	/* 右:待機->SS09へ */
		sakuya_wait_state(src/*,SS09*/);
		break;

	case SS09:	/* 右:左回転攻撃->ひとまずSS00へ */
		#if (1==USE_CONTROLLER)
		enemy_boss02_sr_add2(src);
		#endif //(1==USE_CONTROLLER)
		data->state1 = SS00;/*	data->n_wait3++;*/
		break;
//--------
	case SS10:	/* プレイヤー位置付近:大弾3つ->ひとまずSS00へ */
		bullet_create_oodama3(src,	/*仕様変更*/t256(/*1.3*/3.0/*3.0*/), 20/*ransu512*/);	/* 大弾の追加 */
		data->state1 = SS00;/*	data->n_wait3++;*/
		break;

	case SS11:	/* 左:前へ->SS12 */
		if (src->y256 > t256(GAME_HEIGHT/3) )
		{	data->state1++/* = SS12*/;}
		else
		{	src->y256 += (t256(1)+(data->aya_speed)/*(t256(4)+(difficulty<<8))*/)/**fps_fa ctor*/;}
		break;

	case SS12:
		{
			/* 弾に毒塗ってある設定 */
#if 0
/* CWの場合 */
//			bullet_create_n_way_dan_sa_type(src, t256(5), ANGLE_JIKI_NERAI_DAN, deg_360_to_512((30)), BULLET_KOME_01_AOI+(ra_nd()&0x0f),	 5);	/*なるべく共通化*/
#else
/* CCWの場合 */
//			bullet_create_n_way_dan_sa_type(src, t256(5), ANGLE_JIKI_NERAI_DAN, -deg_360_to_512CCW((30)), BULLET_KOME_01_AOI+(ra_nd()&0x0f),	 5);	/*なるべく共通化*/
			bullet_create_n_way_dan_sa_type(src, t256(5), ANGLE_JIKI_NERAI_DAN, deg_360_to_512CCW(360-(30)), BULLET_KOME_01_AOI+(ra_nd()&0x0f),  5);	/*なるべく共通化*/
#endif
		//	int angle_jikinerai_512;
		//	angle_jikinerai_512=at an_512(player->y256-src->y256,player->x256-src->x256);
		//	bullet_create_gg_dan(src, t256(5), angle_jikinerai_512+deg_360_to_512(60),	2/*state_hi*/, 0/*state_lo*/);		//いらない弾達。消した方がいいかも
		//	bullet_create_gg_dan(src, t256(5), angle_jikinerai_512+deg_360_to_512(30),	1/*state_hi*/, 0/*state_lo*/);
		//	bullet_create_aka_maru_jikinerai(src, t256(5));
		//	bullet_create_gg_dan(src, t256(5), angle_jikinerai_512-deg_360_to_512(30), -1/*state_hi*/, 0/*state_lo*/);
		//	bullet_create_gg_dan(src, t256(5), angle_jikinerai_512-deg_360_to_512(60), -2/*state_hi*/, 0/*state_lo*/);
		}
		data->state1 = SS00;/*	data->n_wait3++;*/
		break;
	}
}


/*---------------------------------------------------------
	ボス行動
---------------------------------------------------------*/

static void move_aya(SPRITE *src)
{
	/* スペカ登録 */
			if (0/*off*/==spell_card_mode)
			{
				regist_spell_card222(src);
			}
	spell_card_generator222(src);	/* スペカ生成 */
	#if 1/* [スペカシステム内に移動予定] */
	/*---------------------------------------------------------
		パチェ移動処理
	---------------------------------------------------------*/
	//
	//	enemy_boss04_setpos(src, xxx,yyy);
	//
	//boss_move96(src);
	#endif
//
	boss_effect(src);			/* 回エフェクト */
	danmaku_generator(src); 	/* 弾幕生成 */
//
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
	//	sakuya								= spr ite_add_file 0("boss/aya.png"/*"boss02_v2.png"*/,9,PRIORITY_03_ENEMY, 0); s->anim_speed=0;
	//	sakuya->base_health 				= (difficulty<<10/**1024*/)+3071+1024;
	//	sakuya->base_health 				= (1024-1);
	//	sakuya->base_health 				= (1024-1)*((difficulty)+3);
	//	sakuya->base_score					= score(2000)+score(1500)*difficulty;
	//	sakuya->base_health 				= ((1024)-1)*((difficulty)+3);/*test*/

void add_boss_aya(STAGE_DATA *l)/*int lv*/
{
	boss_bgm_mode		= (l->user_y);
//
//----[BOSS]
		SPRITE *sakuya;
		sakuya								= sprite_add_res(BASE_BOSS_AYA_PNG);
		sakuya->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
		sakuya->yx_anim_frame				= 0x00/*AYA_ANIME_CENTER_04*/;
		sakuya->type						= SP_BOSS/*SP_BOSS02*/;
		sakuya->callback_mover				= move_aya;
		sakuya->callback_loser				= aya_put_items;
	//
	//	sakuya->base_health 				= ((1024)-1)*4;/*test*/
		sakuya->base_health 				= ((1024)-1)*6;/*test*/
		sakuya->base_score					= adjust_score_by_difficulty(score(2000000));	/* 200万 */
	//------------ スペカ関連
		spell_card_number					= SPELL_CARD_00_aya_000;
		spell_card_max						= SPELL_CARD_19_aya_jjj;
		spell_card_boss_init_regist(sakuya);
	{
		BOSS02_DATA *data;
		data								= mmalloc(sizeof(BOSS02_DATA));
		sakuya->data						= data;
		data->state1						= (0)/*ST_13*/;/*0*/
		data->wait1 						= 50;
		data->animation_wait				= 0;
		data->n_wait3						= 0/*1*/;
		data->aya_speed 					= t256(4)+(difficulty<<8);
		data->ice_number					= (0);
		//
		#if 1
	//------------ 弾幕関連
		data->boss_base.danmaku_type		= 0;
		data->boss_base.danmaku_time_out	= 0;
		data->boss_base.danmaku_test		= 0;	/*(DANMAKU_08_rumia-1)*/ /*0*/
		#endif
	}
	//------------ 特殊初期化
	#if 0
	/*	common_boss_init(); より後の必要がある*/
	{
		/* boss_rect_init */
		boss_clip_min.x256	= t256(0);
		boss_clip_min.y256	= t256(0);
		boss_clip_max.x256	= t256(GAME_WIDTH);
		boss_clip_max.y256	= (t256(128)+(difficulty<<(8+5)));	/*t256(96)*/
	}
		/*
		0	easy:	128(踏まれない)
		1	normal: 160(踏まれない)
		2	hard:	192(踏まれない)
		3	luna:	224(踏まれる)
		*/
	#endif
}
