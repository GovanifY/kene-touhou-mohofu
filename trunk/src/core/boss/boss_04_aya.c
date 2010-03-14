
#include "bullet_object.h"
/*static*/extern void add_zako_aya_doll(SPRITE *src);/* 椛弾 */

/*---------------------------------------------------------
	射命丸 文
	Syameimaru Aya.
	-------------------------------------------------------
	天狗
	ToDo:
	ボスタイマー対応中
	-------------------------------------------------------
	自分で思ったよりも第2形態の攻撃パターンがカオスになってしまった。
	もっと丁寧に作り直した方がいいかも。
	[*** 090114 	天狗の速度を難易度ごとに変えてみた。
//
	差分氏の乱数大弾(文用の揺らぐ大弾)について私見(231)
	乱数大弾は (easy＋高速弾) 辺りまでなら面白いのですが、
	試行錯誤した結果、
	hard, lunatic あたりの弾幕シューを作るとして、
	弾密度をあげられないので、東方っぽくならないです。
	そういう理由で止めました。
---------------------------------------------------------*/

//	int st ate1;		/* 形態ステート */	/* 弾幕ステート */
#define aya_angle512			user_data00 	/* 文の角度 */
//
#define wait1					user_data01 	/* 移動待ち */
#define animation_wait			user_data02 	/* アニメーション用 */
//

#define aya_speed				user_data03 	/* 難度別速度 */
#define ice_number				user_data04 	/* ICE弾幕番号 */


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
		src->ice_number++;
	//	if (DT02 < src->ice_number) {	src->ice_number = DT02; }
		src->boss_base_state001 = 0;
		src->wait1	= 40;
	//	player_add_score(src->boss_base_score);
	}
}

/*---------------------------------------------------------
	自機狙い弾の角度を計算
---------------------------------------------------------*/
static int angle_jikinerai512(SPRITE *p, SPRITE *t)
{
	#if 1
	return (atan_512((p->y256-t->y256), (p->x256-t->x256)));
	/* 偶数弾の場合に自機狙い(簡略版) 弾と自分が大きさが同じならずれない、違うとその分誤差になる */
	#endif
}


/*---------------------------------------------------------
	天狗用の大弾(青)。乱数で怪しげな動きをする。
	-------------------------------------------------------
	見た目＞＞あたり判定です。
	速度は、[速度に比例するものの意味の良く判らない固定値]から、
	単純に弾速へと仕様変更した。
---------------------------------------------------------*/

//extern void bullet_create_aya_ice(SPRITE *src);/*仕様変更*/
/**/
static int bullet_create_oodama3(SPRITE *src, int speed256)
{
	int angle_jikinerai_512;
	angle_jikinerai_512 = angle_jikinerai512(player,src);
	int angle512;
	for (angle512=(angle_jikinerai_512); angle512<(angle_jikinerai_512+512); angle512+=deg_360_to_512CCW((30)) )/* 30度づつ一周(12==360/30だから12方向) */
	{
	//	unsigned int aaa = (((angle512>>6)&(0x06))|(1));
		unsigned int aaa = (((angle512>>6)&(0x03)));
	//
		bullet_create_offset_dan_type000(
			src,
			speed256,
			angle512,
			(BULLET_OODAMA32_00_SIROI+(aaa)) 	/* 弾グラ */	/*bullet_obj_type*/
		);
	}
	return (angle_jikinerai_512);
}
/*---------------------------------------------------------

---------------------------------------------------------*/

static void sakuya_wait_state(SPRITE *src/*, int next_set_state*/)
{

	if (0 < src->wait1)
	{
		src->wait1--;/*fps_factor*/
		src->animation_wait++;/*fps_factor*/
		if (7 < src->animation_wait)
		{
			src->animation_wait = 0;
		//	if (src->an im_frame<AYA_ANIME_CENTER_04)		{	src->an im_frame++; 	}
		//	else if (src->an im_frame>AYA_ANIME_CENTER_04)	{	src->an im_frame--; 	}
			u8 aaa = (src->type/*an im_frame*/ & 0x03);
			if (0!=(aaa))
			{
				aaa--;
				src->type/*an im_frame*/ = (BOSS_00_BOSS11)+((src->type/*an im_frame*/ & 0xfc)|(aaa));
			}
		}
	}
	else
	{
		src->boss_base_state001++/* = next_set_state*/;
		bullet_create_oodama3(src, /*仕様変更*/(t256(4.0)+(difficulty<<7))/*, 10*/);
	}
}
/*---------------------------------------------------------

---------------------------------------------------------*/
static void sakuya_anime08(SPRITE *src)
{

			u8 aaa = (src->type/*an im_frame*/ & 0x03);
		//	if (src->an im_frame<AYA_ANIME_RIGHT_08)
			if (3!=aaa)
			{
				src->animation_wait++;/*fps_factor*/
				if (7 < src->animation_wait)
				{
					src->animation_wait=0;
				//	src->an im_frame++;
					aaa++;
					src->type/*an im_frame*/ = (BOSS_00_BOSS11)+((0x00)|(aaa));/*(src->an im_frame & 0xfc)*/
				}
			}
}
/*---------------------------------------------------------

---------------------------------------------------------*/
static void sakuya_anime00(SPRITE *src)
{

			u8 aaa = (src->type/*an im_frame*/ & 0x03);
		//	if (src->an im_frame>AYA_ANIME_LEFT_00)
			if (3!=aaa)
			{
				src->animation_wait++;/*fps_factor*/
				if (7 < src->animation_wait)
				{
					src->animation_wait=0;
				//	src->an im_frame--;
					aaa++;
					src->type/*an im_frame*/ = (BOSS_00_BOSS11)+((/*0x10*/0x04)|(aaa));/*(src->an im_frame & 0xfc)*/
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

static void bullet_create_kougeki_01(SPRITE *src)
{
//	if (DT00==data->ice_number)
//	if (2 < ((data->ice_number+1) & 0x03/*DT00*/))
	{
		const unsigned char aaa_bbb[8] =
		{
			0,0,1,0, 0,1,0,1,
		};
		if (0 != aaa_bbb[(src->ice_number&7)/*+8*/])
		{
			//bullet_create_aya_ice(src);
			/* 文に美鈴っぽい弾幕撃たせてみるテスト */
			src->boss_base_danmaku_type 	= DANMAKU_12_aya_merin_test;					/* 禊弾幕をセット */
			src->boss_base_danmaku_time_out = (DANMAKU_01_SET_TIME+DANMAKU_01_SET_TIME);	/* 禊弾幕の発生時間 x 2 */
		}
	}
};
static void bullet_create_kougeki_02(SPRITE *src)
{
//	if (DT01==src->ice_number)
	if (0!=(src->ice_number & 1/*DT01*/))
	{
		add_zako_aya_doll(src);
	}
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

#define SAKUYA_POINT_X_LEFT_OUT 	(t256((GAME_WIDTH-50)*0/4))
#define SAKUYA_POINT_X_LEFT_MID 	(t256((GAME_WIDTH-50)*1/4))
#define SAKUYA_POINT_X_MID			(t256((GAME_WIDTH-50)*2/4))/*-src->w/2*/
#define SAKUYA_POINT_X_RIGHT_MID	(t256((GAME_WIDTH-50)*3/4))
#define SAKUYA_POINT_X_RIGHT_OUT	(t256((GAME_WIDTH-50)*4/4))/*-src->w  */
#define SAKUYA_POINT_X_MARGIN		(t256(25/*16*/))


#define SAKUYA_LIMIT_X_LEFT 		(SAKUYA_POINT_X_LEFT_OUT +SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_LEFT 	(SAKUYA_POINT_X_MID  +SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_RIGHT	(SAKUYA_POINT_X_MID  -SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_RIGHT		(SAKUYA_POINT_X_RIGHT_OUT-SAKUYA_POINT_X_MARGIN)

//(t256(GAME_WIDTH)-((src->w128+src->w128)))


/*---------------------------------------------------------
	移動パターン(通常攻撃)
---------------------------------------------------------*/
#if 0
//extern const u8 aya_danmaku_table[16];
/*static*/static const u8 aya_danmaku_table[4/*16*/] =
{
/*	0:easy						1:normal					2:hard						3:lunatic */
	DANMAKU_10_cirno,			DANMAKU_08_rumia,			DANMAKU_08_rumia,			DANMAKU_08_rumia,	/* 1回目 */
//	DANMAKU_10_cirno,			DANMAKU_08_rumia,			DANMAKU_08_rumia,			DANMAKU_08_rumia,	/* 2回目 */
//	DANMAKU_08_rumia,			DANMAKU_10_cirno,			DANMAKU_08_rumia,			DANMAKU_08_rumia,	/* 3回目 */
//	DANMAKU_08_rumia,			DANMAKU_08_rumia,			DANMAKU_10_cirno,			DANMAKU_08_rumia,	/* 4回目 */
};
#endif
/*static*/ void aya_01_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait--;/*fps_factor*/
//

	{
		if (0 >= my_wait)
		{	/* 移動方向を決める */
			src->boss_base_state001++;
			src->boss_base_state001 &= (8-1);
			{
				enum
				{
			//		dummy_PPP_00_VX = 0,		/* x ベクトル移動量 */
			//		dummy_PPP_01_VY,			/* y ベクトル移動量 */
					PPP_02_WAIT_DIV_2 = 0,		/* ウェイトカウンタの半分量 */
					PPP_03_IS_RESET_ANIME,		/* アニメーションリセット 0:しない 1:する 2:特別(弾幕撃ち) */
					PPP_04_MAX/* 最大項目数 */
				};
				static const s8 ppp[8][PPP_04_MAX] =
				{
					{/*( 2),(-1),*/(100),( 1),},	/*右上へ*/
					{/*( 0),( 0),*/( 50),( 2),},	/*wait*/
					{/*( 2),( 1),*/(100),( 1),},	/*右下へ*/
					{/*( 0),( 0),*/( 10),( 0),},	/*wait*/
					{/*(-2),(-1),*/(100),( 1),},	/*左上へ*/
					{/*( 0),( 0),*/( 50),( 2),},	/*wait*/
					{/*(-2),( 1),*/(100),( 1),},	/*左下へ*/
					{/*( 0),( 0),*/( 10),( 0),},	/*wait*/
				};
//				data->vx	= ppp[src->boss_base_state001][PPP_00_VX];
//				data->vy	= ppp[src->boss_base_state001][PPP_01_VY];
//				src->vx256	= (0);
//				src->vy256	= ((ppp[src->boss_base_state001][PPP_01_VY])<<4);
				my_wait 	= ppp[src->boss_base_state001][PPP_02_WAIT_DIV_2];	/* 50*4 60 移動量 */
				my_wait 	+= my_wait;
				if (0!=ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME])
				{
					if (2==ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME]) /* 攻撃アニメーション */
					{
//						alice_anime_count = 48;
					//	src->boss_base_danmaku_test &= 0x0f;
						src->boss_base_danmaku_type 	= DANMAKU_08_rumia;//aya_danmaku_table[src->boss_base_danmaku_test];	/*DANMAKU_01*/	/* 禊弾幕をセット */
					//	src->boss_base_danmaku_test += 4;
						src->boss_base_danmaku_time_out = ((60*10)/*DANMAKU_01_SET_TIME*6*10*/);	/* 禊弾幕の発生時間 x 60 */
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
	移動パターン2(華符「芳華絢爛」のテスト)
---------------------------------------------------------*/
#if 0
//extern const u8 aya_danmaku_table[16];
/*static*/static const u8 aya_danmaku_table[16] =
{
/*	0:easy						1:normal					2:hard						3:lunatic */
	DANMAKU_08_rumia,			DANMAKU_08_rumia,			DANMAKU_08_rumia,			DANMAKU_0a_houka_kenran,	/* 1回目 */
	DANMAKU_10_cirno,			DANMAKU_08_rumia,			DANMAKU_0a_houka_kenran,	DANMAKU_08_rumia,			/* 2回目 */
	DANMAKU_08_rumia,			DANMAKU_10_cirno,			DANMAKU_10_cirno,			DANMAKU_0a_houka_kenran,	/* 3回目 */
	DANMAKU_0a_houka_kenran,	DANMAKU_0a_houka_kenran,	DANMAKU_0a_houka_kenran,	DANMAKU_0a_houka_kenran,	/* 4回目 */
};
#endif
/*static*/ void aya_02_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait--;/*fps_factor*/
//

	{
		if (my_wait <= 0)
		{	/* 移動方向を決める */
			src->boss_base_state001++;
			src->boss_base_state001 &= (8-1);
			{
				enum
				{
			//		dummy_PPP_00_VX = 0,		/* x ベクトル移動量 */
			//		dummy_PPP_01_VY,			/* y ベクトル移動量 */
					PPP_02_WAIT_DIV_2 = 0,		/* ウェイトカウンタの半分量 */
					PPP_03_IS_RESET_ANIME,		/* アニメーションリセット 0:しない 1:する 2:特別(弾幕撃ち) */
					PPP_04_MAX/* 最大項目数 */
				};
				static const s8 ppp[8][PPP_04_MAX] =
				{
					{/*( 2),(-1),*/(100),( 1),},	/*右上へ*/
					{/*( 0),( 0),*/( 50),( 2),},	/*wait*/
					{/*( 2),( 1),*/(100),( 1),},	/*右下へ*/
					{/*( 0),( 0),*/( 10),( 0),},	/*wait*/
					{/*(-2),(-1),*/(100),( 1),},	/*左上へ*/
					{/*( 0),( 0),*/( 50),( 2),},	/*wait*/
					{/*(-2),( 1),*/(100),( 1),},	/*左下へ*/
					{/*( 0),( 0),*/( 10),( 0),},	/*wait*/
				};
//				data->vx	= ppp[src->boss_base_state001][PPP_00_VX];
//				data->vy	= ppp[src->boss_base_state001][PPP_01_VY];
//				src->vx256	= (0);
//				src->vy256	= ((ppp[src->boss_base_state001][PPP_01_VY])<<4);
				my_wait 	= ppp[src->boss_base_state001][PPP_02_WAIT_DIV_2];	/* 50*4 60 移動量 */
				my_wait 	+= my_wait;
				if (0!=ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME])
				{
					if (2==ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME]) /* 攻撃アニメーション */
					{
//						alice_anime_count = 48;
					//	src->boss_base_danmaku_test &= 0x0f;
						src->boss_base_danmaku_type 	= DANMAKU_0a_houka_kenran;//aya_danmaku_table[src->boss_base_danmaku_test]; /*DANMAKU_01*/	/* 禊弾幕をセット */
					//	src->boss_base_danmaku_test += 4;
						src->boss_base_danmaku_time_out = ((60*10)/*DANMAKU_01_SET_TIME*6*10*/);	/* 禊弾幕の発生時間 x 60 */
					}
//					vvv256=1;
				}
			}
		}
	}
//	src->vx256 = (src->vx)*vvv256;
//	src->vy256 = (src->vy)*vvv256;
}


/*---------------------------------------------------------
	移動パターン1(低速移動)
---------------------------------------------------------*/

/*static*/ void aya_04_keitai(SPRITE *src)
{
	switch (src->boss_base_state001)
	{
	case ST_00: /* y-pos erreicht: rundumschuss */
		{
						src->boss_base_danmaku_time_out = ((0)/*DANMAKU_01_SET_TIME*6*10*/);	/* 弾幕を止める */
		}
		#if (1==USE_CONTROLLER)
		enemy_boss02_sr_add1(src);
		#endif //(1==USE_CONTROLLER)
		bullet_create_kougeki_01(src);
		bullet_create_kougeki_02(src);
		src->wait1 = 20;
		src->boss_base_state001++/* = ST_01*/;
		break;

	case ST_01: /* waiting */
		sakuya_wait_state(src/*,ST_02*/);
		break;

	case ST_02: /* nach links bis zum rand */
		if (src->x256 >= t256(0.0))
		{
			src->x256 -= t256(2.0);/*fps_factor*/
			sakuya_anime00(src);
		}
		else
		{
			src->boss_base_state001++/* = ST_04*/;
		}
		break;
	case ST_03: /*	*/
		{
			src->boss_base_state001++/* = ST_04*/;
			src->wait1 = 20;
			bullet_create_kougeki_02(src);
		}
		break;


	case ST_04: /* waiting */
		sakuya_wait_state(src/*,ST_05*/);
		break;

	case ST_05: /* nach rechts bis zur mitte */
		if (src->x256 < BOSS_XP256)
		{
			src->x256 += t256(2.0);/*fps_factor*/
			sakuya_anime08(src);
		}
		else
		{
			#if (1==USE_CONTROLLER)
			enemy_boss02_sr_add1(src);
			#endif //(1==USE_CONTROLLER)
			src->wait1 = 100;
			src->boss_base_state001++;/* = ST_06*/
			bullet_create_kougeki_02(src);
		}
		break;

	case ST_06: /* waiting */
		sakuya_wait_state(src/*,ST_07*/);
		break;

	case ST_07:
		bullet_create_kougeki_01(src);
		src->wait1 = 100;
		src->boss_base_state001++;/* = ST_08*/
		break;

	case ST_08: /* waiting */
		sakuya_wait_state(src/*,ST_09*/);
		break;

	case ST_09: /* nach rechts bis zum rand */
		#if 0/*SDL(左隅座標)*/
		if (src->x256 < (t256(GAME_WIDTH-50)-((src->w128+src->w128))))
		#else/*Gu(中心座標)*/
		if (src->x256 < t256(GAME_WIDTH-50) )
		#endif
		{
			src->x256 += t256(2.0);/*fps_factor*/
			sakuya_anime08(src);
		}
		else
		{
			src->wait1 = 20;
			src->boss_base_state001++;/* = ST_10*/
			bullet_create_kougeki_02(src);
		}
		break;

	case ST_10: /* waiting */
		sakuya_wait_state(src/*,ST_11*/);
		break;

	case ST_11: /* nach links bis zur mitte */
		if (src->x256 > BOSS_XP256)
		{
			src->x256 -= t256(2.0);/*fps_factor*/
			sakuya_anime00(src);
		}
		else
		{	src->boss_base_state001++;/* = ST_00*/ }
		break;
	case ST_12: /*	*/
		{
			src->boss_base_state001 = ST_00;
		}
		break;
	}
}


/*---------------------------------------------------------
	移動攻撃パターン2(高速移動)
---------------------------------------------------------*/

/*static*/ void aya_05_keitai(SPRITE *src)
{
	switch (src->boss_base_state001)
	{
	case SS00:	/* 不定:初期位置情報の取得->SS01へ */
		send1_obj->x256 = t256((GAME_WIDTH-50)/2);
		send1_obj->y256 = t256(20.0);
	//	src->aya_angle512 = at an_512(send1_obj->y256-src->y256,send1_obj->x256-src->x256-((src->w128+src->w128)));
		src->aya_angle512 = angle_jikinerai512(send1_obj, src);
		src->boss_base_state001++/* = SS01*/;
		break;

	case SS01:	/* 不定:初期位置へ戻る->SS02へ */
		if (src->y256 < t256(30.0))
		{
			src->boss_base_state001++/* = SS02*/;
			src->wait1 += 10;//+(3-3/*difficulty*/)*10
			{
				voice_play(VOICE11_BOSS_KIRARIN/*VOICE14_BOSS_KOUGEKI_01*/, TRACK04_TEKIDAN);/*テキトー*/
			//
				send1_obj->x256 = src->x256;
				send1_obj->y256 = src->y256;
				send1_obj->BULLET_REGIST_angle512			=	ANGLE_JIKI_NERAI_DAN;
				send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/24);
				/* 毎回 */
				{
					int aaa;
					aaa = (difficulty<<6);/*src->aya_speed*/
					aaa += (t256(4.5)); 		/* 3.5だとeasyが難しすぎる。 t256(3.5) */
					int jj;
					for (jj=(0); jj<(8+difficulty); jj++ )
					{
						aaa -= (t256(0.125));
						/* 弾に毒塗ってある設定 */
						send1_obj->BULLET_REGIST_speed256			=	(aaa);									/* 弾速 */
						send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KOME_01_AOI+(ra_nd()&0x0f);		/* 弾グラ */	/*BULLET_KUNAI12_00_AOI*/
						send1_obj->BULLET_REGIST_n_way				=	(11-jj);								/* [10way] */
						bullet_regist_basic();
					}
				}
			}
		}
		else
		{
#if 1
/* CCWの場合 */
			src->x256 += ((sin512((src->aya_angle512))*t256(6.0))>>8);/*fps_factor*/
			src->y256 += ((cos512((src->aya_angle512))*t256(6.0))>>8);/*fps_factor*/
#endif
			src->boss_base_state001--/* = SS00*/;
		}
		break;

	case SS02:	/* 初期位置:大弾3つ->SS03, SS04, SS05 */
		src->wait1--;/*fps_factor*/
		if (0 > src->wait1)
		{	int angle_jikinerai_512;
			angle_jikinerai_512 = bullet_create_oodama3(src, (src->aya_speed)-t256(2.0) );
					/*t256(5.0)*/ /* 弾速 */		/*仕様変更*/	// t256(/*1.5*/5.0/*5.0*/)
			//	,1/*ransu512*/
		//	src->boss_base_state001 = (ra_nd()%3)+SS03;
			src->boss_base_state001 = (((ra_nd()&(256-1))*3)>>8)+SS03;
			if (src->boss_base_state001 == SS05)
			{
				src->aya_angle512 = angle_jikinerai_512;
			}
			else
			{
				src->y256 += t256(6.0);/*fps_factor*/
			}
		}
		break;

	case SS03:	/* 左移動中->SS06へ */
		if (0 <= src->x256)
		{
			src->x256 -= (src->aya_speed);/*fps_factor*/ 	 /*(t256(4.0)+(difficulty<<8))*/
			sakuya_anime00(src);
		}
		else
		{
			src->x256 += t256(1.0); 	// [***090114		追加
			src->wait1 = 5/*10*/;
			src->boss_base_state001 = SS06;
		}
		break;

	case SS04:	/* 右移動中->SS08へ */
		#if 0/*SDL(左隅座標)*/
		if (src->x256 < (t256((GAME_WIDTH-50))-((src->w128+src->w128))) )
		#else/*Gu(中心座標)*/
		if (src->x256 < t256((GAME_WIDTH-50)) )
		#endif
		{
			src->x256 += (src->aya_speed);/*fps_factor*/	/*(t256(4.0)+(difficulty<<8))*/
			sakuya_anime08(src);
		}
		else
		{
			src->x256 -= t256(1.0); 	// [***090114		追加
			src->wait1 = 5/*10*/;
			src->boss_base_state001 = SS08;
		}
		break;

	case SS05:	/* プレイヤー位置付近移動中->SS10へ */
		/* ayaの稼動範囲 */
		#if 0
		/* 画面を喰み出た場合に戻る(ボス当たりありにしたので復活時に踏まれて困る) */
		if (((src->x256 < t256(0.0))||(src->y256 < t256(0.0)))||
			((src->x256+((s->w128+src->w128)) > t256((GAME_WIDTH-50)))||
		//	 (src->y256+((s->h128+src->h128)) > t256(GAME_HEIGHT))))
			 (src->y256+((s->h128+src->h128)) > t256(220.0))))/*(踏まれない)*/
		#endif
		/* 稼動範囲を喰み出た場合に戻る(ボス当たりありにしたので復活時に踏まれて困る) */
		#if 0/*SDL(左隅座標)*/
		if (((src->x256 < t256(0.0))||(src->y256 < t256(0.0)))||
			((src->x256+((src->w128+src->w128)) > t256((GAME_WIDTH-50)))||
		//	 (src->y256+((src->h128+src->h128)) > t256(GAME_HEIGHT))))
			 (src->y256+((src->h128+src->h128)) > (t256(128.0)+(difficulty<<(8+4/*5*/)))	))) 	/* difficulty x 16	 32 */
		#else/*Gu(中心座標)*/
		if (((src->x256 < t256(0.0))||(src->y256 < t256(0.0)))||
			((src->x256 > t256((GAME_WIDTH-50)))||
		//	 (src->y256 > t256(GAME_HEIGHT))))
			 (src->y256 > (t256(128.0)+(difficulty<<(8+4/*5*/)))	))) 	/* difficulty x 16	 32 */
		#endif
		/*
			easy:	128(踏まれない)
			normal: 160(踏まれない)
			hard:	192(踏まれない)
			luna:	224(踏まれる)
		*/
		{
			src->boss_base_state001 = SS10;
		}
		else
		{
#if 1
/* CCWの場合 */
			src->x256 += ((sin512((src->aya_angle512))*(src->aya_speed)/*(t256(4.0)+(difficulty<<8))*/)>>8);/*fps_factor*/
			src->y256 += ((cos512((src->aya_angle512))*(src->aya_speed)/*(t256(4.0)+(difficulty<<8))*/)>>8);/*fps_factor*/
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
			src->boss_base_state001 = SS00;
		}
		else
		{	src->boss_base_state001 = SS11;}
		break;
//--------
	case SS08:	/* 右:待機->SS09へ */
		sakuya_wait_state(src/*,SS09*/);
		break;

	case SS09:	/* 右:左回転攻撃->ひとまずSS00へ */
		#if (1==USE_CONTROLLER)
		enemy_boss02_sr_add2(src);
		#endif //(1==USE_CONTROLLER)
		src->boss_base_state001 = SS00;
		break;
//--------
	case SS10:	/* プレイヤー位置付近:大弾3つ->ひとまずSS00へ */
		bullet_create_oodama3(src, ((src->aya_speed)>>1));
		/* 弾速 */	/*t256(3.0)*/	/*仕様変更*/	//t256(/*1.3*/3.0/*3.0*/),
		//	,20/*ransu512*/
		/* 大弾の追加 */
		src->boss_base_state001 = SS00;
		break;

	case SS11:	/* 左:前へ->SS12 */
		if (src->y256 > t256(GAME_HEIGHT/3) )
		{	src->boss_base_state001++;/* = SS12*/}
		else
		{	src->y256 += (t256(1.0)+(src->aya_speed)/*(t256(4.0)+(difficulty<<8))*/);/*fps_factor*/}
		break;

	case SS12:
		{
	//		-deg_360_to_512CCW((30)),
		/* CCWの場合 */
			send1_obj->x256 = src->x256;
			send1_obj->y256 = src->y256;
			send1_obj->BULLET_REGIST_speed256			=	(src->aya_speed)-t256(2.0); 			/* 弾速 */	/*t256(5.0)*/
			send1_obj->BULLET_REGIST_angle512			=	ANGLE_JIKI_NERAI_DAN;
			send1_obj->BULLET_REGIST_div_angle512		=	deg_360_to_512CCW(360-(30));
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KOME_01_AOI+(ra_nd()&0x0f);		/* 弾グラ */	/* 弾に毒塗ってある設定 */
			send1_obj->BULLET_REGIST_n_way				=	(5) ;									/* [5way] */
			bullet_regist_basic();
		}
		src->boss_base_state001 = SS00;
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

void add_boss_aya(STAGE_DATA *l)
{
	//----[BOSS]
	SPRITE *h;
	#if (0==USE_BOSS_COMMON_MALLOC)
	h								= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	#else
	h								= pd_boss;/*輝夜本人*/
	#endif
	h->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	h->type 						= BOSS_00_BOSS11;
	h->callback_mover				= move_aya;
	h->callback_loser				= aya_put_items;
	//
	h->base_score					= adjust_score_by_difficulty(score(2000000));	/* 200万 */
	//------------ スペカ関連
	spell_card_number					= SPELL_CARD_00_aya_000;
	spell_card_max						= SPELL_CARD_19_aya_jjj;
	#if (0==USE_BOSS_COMMON_MALLOC)
	spell_card_boss_init_regist(h);
	#else
	spell_card_boss_init_regist_void();
	#endif
	{
	h->boss_base_state001			= (0)/*ST_13*/;/*0*/
	h->wait1						= 50;
	h->animation_wait				= 0;
	h->aya_speed					= t256(4.0)+(difficulty<<8);
	h->ice_number					= (0);
	}
	//------------ 特殊初期化
	#if 0
	/*	common_boss_init(); より後の必要がある*/
	{
		/* boss_rect_init */
		boss_clip_min.x256	= t256(0.0);
		boss_clip_min.y256	= t256(0.0);
		boss_clip_max.x256	= t256((GAME_WIDTH-50));
		/*
		0	easy:	128 = 128+(32x0) (踏まれない)
		1	normal: 160 = 128+(32x1) (踏まれない)
		2	hard:	192 = 128+(32x2) (踏まれない)
		3	luna:	224 = 128+(32x3) (踏まれる)
		*/
	//	boss_clip_max.y256	= (t256(128.0)+(difficulty<<(8+5)));	/*t256(96.0)*/
		/* (踏まれない)
		0	easy:	128 = 128+(16x0) (踏まれない)
		1	normal: 144 = 128+(16x1) (踏まれない)
		2	hard:	160 = 128+(16x2) (踏まれない)
		3	luna:	176 = 128+(16x3) (踏まれない)
		*/
		boss_clip_max.y256	= (t256(128.0)+(difficulty<<(8+4)));	/*t256(96.0)*/
	}
	#endif
}
