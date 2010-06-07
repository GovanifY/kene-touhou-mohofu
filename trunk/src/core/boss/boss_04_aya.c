
#include "boss.h"

/*static*/extern void add_zako_aya_doll(SPRITE *src);/* 椛弾 */

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	射命丸 文
	Syameimaru Aya.
	-------------------------------------------------------
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
#if 0/* めも：あやは、共通部分で使ってる。 */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif


//#de fine an imation_wait			user_data02 	/* アニメーション用 */


//	int st ate1;		/* 形態ステート */	/* 弾幕ステート */
#define aya_angle1024			user_data04 	/* 文の角度 */
//
#define wait1					user_data05 	/* 移動待ち */
//
#define aya_speed				user_data06 	/* 難度別速度 */
#define ice_number				user_data07 	/* ICE弾幕番号 */


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
	//	if (DT02 < src->ice_number) 	{	src->ice_number = DT02; }
		src->boss_base_state001 = 0;
		src->wait1	= 40;
	//	player_add_score(src->boss_base_score);
	}
}


/*---------------------------------------------------------
	天狗用の大弾(青)。乱数で怪しげな動きをする。
	-------------------------------------------------------
	見た目＞＞あたり判定です。
	速度は、[速度に比例するものの意味の良く判らない固定値]から、
	単純に弾速へと仕様変更した。
---------------------------------------------------------*/

/**/
static void s_bullet_create_aya_oodama3(SPRITE *src, int speed256)
{
			obj_send1->x256 					= (src->x256);
			obj_send1->y256 					= (src->y256);
//
			br.BULLET_REGIST_speed256			= speed256; 		/* 弾速 */
			br.BULLET_REGIST_n_way				= (1);				/* [1way] */
//			br.BULLET_REGIST_div_angle1024		= (0);				/* ダミー角度(未使用) */
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
//
	tmp_angleCCW1024_jikinerai(obj_player, src);
	{	int angle1024;
		for (angle1024=(src->tmp_angleCCW1024); angle1024<(src->tmp_angleCCW1024+1024); angle1024 += cv1024r((30)) )/* 30度づつ一周(12==360/30だから12方向) */
		{
			br.BULLET_REGIST_angle1024			= angle1024;
			br.BULLET_REGIST_bullet_obj_type	= (BULLET_OODAMA32_00_SIROI+((u32)(((angle1024>>7)&(0x03)))));	/* 弾グラ */
			bullet_regist_vector();
		}
	}
}
/*---------------------------------------------------------

---------------------------------------------------------*/
static int src_animation_wait;
static void sakuya_wait_state(SPRITE *src/*, int next_set_state*/)
{
	if (0 < src->wait1)
	{
		src->wait1--;/*fps_factor*/
		src_animation_wait++;/*fps_factor*/
		if (7 < src_animation_wait)
		{
			src_animation_wait = 0;
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
		s_bullet_create_aya_oodama3(src, /*仕様変更*/(t256(4.0)+(difficulty<<7))/*, 10*/);
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
		src_animation_wait++;/*fps_factor*/
		if (7 < src_animation_wait)
		{
			src_animation_wait = 0;
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
		src_animation_wait++;/*fps_factor*/
		if (7 < src_animation_wait)
		{
			src_animation_wait = 0;
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
	ST3_00 = 0,
	ST3_01,
	ST3_02,
	ST3_04,
	ST3_05,
	ST3_06,
	ST3_07,
	ST3_08,
	ST3_09,
	ST3_10,
	ST3_11,
	ST3_12,
};enum
{
	ST4_00 = 0,
	ST4_01,
	ST4_02,
	ST4_04,
	ST4_05,
	ST4_06,
	ST4_07,
	ST4_08,
	ST4_09,
	ST4_10,
	ST4_11,
	ST4_12,
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
};


/*---------------------------------------------------------
	アイス弾
---------------------------------------------------------*/

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
	移動パターン(「通常攻撃3」のテスト)
---------------------------------------------------------*/

/*static*/ void aya_01_keitai(SPRITE *src)
{
	/* 弾幕を撃ってない場合に追加 */
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		src->boss_base_danmaku_type 	= DANMAKU_12_aya_merin_test;//DANMAKU_08_rumia; 	/*DANMAKU_01*/	/* 禊弾幕をセット */
	//	danmaku_set_time_out(src);		/* 弾幕の制限時間を設定(予め弾幕ごとに設定されている標準時間に設定) */
		src->boss_base_danmaku_time_out = ((60*10)/*DANMAKU_01_SET_TIME*6*10*/);			/* */
//		vvv256 = t256(1.0);
	}
}


/*---------------------------------------------------------
	移動パターン2(華符「芳華絢爛」のテスト)
---------------------------------------------------------*/

/*static*/ void aya_02_keitai(SPRITE *src)
{
	/* 弾幕を撃ってない場合に追加 */
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		src->boss_base_danmaku_type 	= DANMAKU_0a_houka_kenran;		/*DANMAKU_01*/	/* 禊弾幕をセット */
	//	danmaku_set_time_out(src);		/* 弾幕の時間切れを設定 */
		src->boss_base_danmaku_time_out = ((60*10)/*DANMAKU_01_SET_TIME*6*10*/);			/* */
//		vvv256 = t256(1.0);
	}
}
/*---------------------------------------------------------
	文に美鈴っぽい弾幕撃たせてみるテスト
---------------------------------------------------------*/

static void bullet_create_kougeki_01(SPRITE *src)
{
	src->boss_base_danmaku_type 	= DANMAKU_12_aya_merin_test;					/* 禊弾幕をセット */
	danmaku_set_time_out(src);		/* 弾幕の時間切れを設定 */
}
/*---------------------------------------------------------
	移動パターン0(低速移動)
---------------------------------------------------------*/
/* 1なのは随分古い仕様。現在はシステムで ketm 方式の CONTROLLER は無い。
がそれに代わる東方風 方式の CONTROLLER は作るかもしれない。
( CONTROLLER ってのは、ボスの周りふわふわして弾出すアレ。) */
#define USE_CONTROLLER (0)
/*static*/ void aya_03_keitai(SPRITE *src)
{
	switch (src->boss_base_state001)
	{
	case ST3_00: /* y-pos erreicht: rundumschuss */
		{
			src->boss_base_danmaku_time_out = ((0)/*DANMAKU_01_SET_TIME*6*10*/);	/* 弾幕を止める */
		}
		#if (1==USE_CONTROLLER)
		enemy_boss02_sr_add1(src);
		#endif //(1==USE_CONTROLLER)
		bullet_create_kougeki_01(src); /* 攻撃 */

		bullet_create_kougeki_02(src); /* 攻撃 */
		src->wait1 = 20;
		src->boss_base_state001++;/* = ST3_01*/
		break;
//--------
	case ST3_01: /* 弾幕が終わるまで待つ */
		danmaku_state_check_holding(src);/*,ST3_02*/
		break;

	case ST3_02: /* 左へ移動、画面端まで。 */
		sakuya_anime00(src);
		src->x256 -= t256(2.0);/*fps_factor*/
		if (src->x256 < t256(0.0))
		{
			src->boss_base_state001++/* = ST3_04*/;
			src->wait1 = 20;
			bullet_create_kougeki_02(src); /* 攻撃 */
		}
		break;
//--------
	case ST3_04: /* 待つ */
		sakuya_wait_state(src/*,ST3_05*/);
		break;

	case ST3_05: /* 右へ移動、画面中心まで。 */
		sakuya_anime08(src);
		src->x256 += t256(2.0);/*fps_factor*/
		if (src->x256 > BOSS_XP256)
		{
			src->boss_base_state001++;/* = ST3_06*/
			#if (1==USE_CONTROLLER)
			enemy_boss02_sr_add1(src);
			#endif //(1==USE_CONTROLLER)
			src->wait1 = 100;
			bullet_create_kougeki_02(src); /* 攻撃 */
		}
		break;
//--------
	case ST3_06: /* 待つ */
		sakuya_wait_state(src/*,ST3_07*/);
		break;

	case ST3_07:
		src->boss_base_state001++;/* = ST3_08*/
		src->wait1 = 100;
	//	bullet_create_kougeki_01(); /* 攻撃 */
		break;
//--------
	case ST3_08: /* 待つ */
		sakuya_wait_state(src/*,ST3_09*/);
		break;

	case ST3_09: /* 右へ移動、画面端まで。 */
		sakuya_anime08(src);
		src->x256 += t256(2.0);/*fps_factor*/
		#if 0/*SDL(左隅座標)*/
		if (src->x256 > (t256(GAME_WIDTH-50)-((src->w128+src->w128))))
		#else/*Gu(中心座標)*/
		if (src->x256 > t256(GAME_WIDTH-50) )
		#endif
		{
			src->boss_base_state001++;/* = ST3_10*/
			src->wait1 = 20;
			bullet_create_kougeki_02(src); /* 攻撃 */
		}
		break;
//--------
	case ST3_10: /* 待つ */
		sakuya_wait_state(src/*,ST3_11*/);
		break;

	case ST3_11: /* 左へ移動、画面中心まで。 */
		sakuya_anime00(src);
		src->x256 -= t256(2.0);/*fps_factor*/
		if (src->x256 < BOSS_XP256)
		{
		//	src->boss_base_state001++;/* = ST3_00*/
			src->boss_base_state001 = ST3_00;
		}
		break;
	}
}


/*---------------------------------------------------------
	移動パターン1(低速移動)
---------------------------------------------------------*/

/*static*/ void aya_04_keitai(SPRITE *src)
{
	switch (src->boss_base_state001)
	{
	case ST4_00: /* y-pos erreicht: rundumschuss */
		{
			src->boss_base_danmaku_time_out = ((0)/*DANMAKU_01_SET_TIME*6*10*/);	/* 弾幕を止める */
		}
		#if (1==USE_CONTROLLER)
		enemy_boss02_sr_add1(src);
		#endif //(1==USE_CONTROLLER)
	//	bullet_create_kougeki_01(src); /* 攻撃 */

		bullet_create_kougeki_02(src); /* 攻撃 */
		src->wait1 = 20;
		src->boss_base_state001++;/* = ST4_01*/
		break;
//--------
	case ST4_01: /* 待つ */
		sakuya_wait_state(src);/*,ST4_02*/
		break;

	case ST4_02: /* 左へ移動、画面端まで。 */
		sakuya_anime00(src);
		src->x256 -= t256(2.0);/*fps_factor*/
		if (src->x256 < t256(0.0))
		{
			src->boss_base_state001++/* = ST4_04*/;
			src->wait1 = 20;
			bullet_create_kougeki_02(src); /* 攻撃 */
		}
		break;
//--------
	case ST4_04: /* 待つ */
		sakuya_wait_state(src/*,ST4_05*/);
		break;

	case ST4_05: /* 右へ移動、画面中心まで。 */
		sakuya_anime08(src);
		src->x256 += t256(2.0);/*fps_factor*/
		if (src->x256 > BOSS_XP256)
		{
			src->boss_base_state001++;/* = ST4_06*/
			#if (1==USE_CONTROLLER)
			enemy_boss02_sr_add1(src);
			#endif //(1==USE_CONTROLLER)
			src->wait1 = 100;
			bullet_create_kougeki_02(src); /* 攻撃 */
		}
		break;
//--------
	case ST4_06: /* 待つ */
		sakuya_wait_state(src/*,ST4_07*/);
		break;

	case ST4_07:
		src->boss_base_state001++;/* = ST4_08*/
		src->wait1 = 100;
	//	bullet_create_kougeki_01(); /* 攻撃 */
		break;
//--------
	case ST4_08: /* 待つ */
		sakuya_wait_state(src/*,ST4_09*/);
		break;

	case ST4_09: /* 右へ移動、画面端まで。 */
		sakuya_anime08(src);
		src->x256 += t256(2.0);/*fps_factor*/
		#if 0/*SDL(左隅座標)*/
		if (src->x256 > (t256(GAME_WIDTH-50)-((src->w128+src->w128))))
		#else/*Gu(中心座標)*/
		if (src->x256 > t256(GAME_WIDTH-50) )
		#endif
		{
			src->boss_base_state001++;/* = ST4_10*/
			src->wait1 = 20;
			bullet_create_kougeki_02(src); /* 攻撃 */
		}
		break;
//--------
	case ST4_10: /* 待つ */
		sakuya_wait_state(src/*,ST4_11*/);
		break;

	case ST4_11: /* 左へ移動、画面中心まで。 */
		sakuya_anime00(src);
		src->x256 -= t256(2.0);/*fps_factor*/
		if (src->x256 < BOSS_XP256)
		{
		//	src->boss_base_state001++;/* = ST4_00*/
			src->boss_base_state001 = ST4_00;
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
		obj_send1->x256 = t256((GAME_WIDTH-50)/2);
		obj_send1->y256 = t256(20.0);
		tmp_angleCCW1024_jikinerai(obj_send1, src);
		src->aya_angle1024 = src->tmp_angleCCW1024;
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
				obj_send1->x256 					= (src->x256);
				obj_send1->y256 					= (src->y256);
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_div_angle1024		= (int)(1024/24);
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
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
						br.BULLET_REGIST_speed256			= (aaa);									/* 弾速 */
						int tama_graphics;
						tama_graphics = (ra_nd()&0x0f);
						if (0==tama_graphics)	{ tama_graphics = 0x08; }	/* 0 は(星点なので)避ける。 */
						br.BULLET_REGIST_bullet_obj_type	= (BULLET_KOME_01_AOI-1) + (tama_graphics); 	/* 弾グラ */	/*BULLET_KUNAI12_00_AOI*/
						br.BULLET_REGIST_n_way				= (11-jj);								/* [10way] */
						bullet_regist_vector();
					}
				}
			}
		}
		else
		{
#if 1
/* CCWの場合 */
			src->x256 += ((sin1024((src->aya_angle1024))*t256(6.0))>>8);/*fps_factor*/
			src->y256 += ((cos1024((src->aya_angle1024))*t256(6.0))>>8);/*fps_factor*/
#endif
			src->boss_base_state001--/* = SS00*/;
		}
		break;

	case SS02:	/* 初期位置:大弾3つ->SS03, SS04, SS08 */
		src->wait1--;/*fps_factor*/
		if (0 > src->wait1)
		{
			src->wait1 = 5/*10*/;
			s_bullet_create_aya_oodama3(src, (src->aya_speed)-t256(2.0) );
			int angle_jikinerai_1024;
			angle_jikinerai_1024 = src->tmp_angleCCW1024;
					/*t256(5.0)*/ /* 弾速 */		/*仕様変更*/	// t256(/*1.5*/5.0/*5.0*/)
			{
				const u8 sst[8] = { SS03, SS03, SS03, SS04, SS04, SS04, SS08, SS08 }; /* (1/3)の確率で分岐する。 */
				src->boss_base_state001 = sst[((ra_nd()&(8-1)))];	/* SS03, SS04, SS08 */
			}
			if (src->boss_base_state001 == SS08)
			{
				src->aya_angle1024 = angle_jikinerai_1024;
			}
			else
			{
				src->y256 += t256(6.0);/*fps_factor*/
			}
		}
		break;

//-------- 分岐 [SS03]
	case SS03: /* 左移動中->SS05へ */
		sakuya_anime00(src);
		src->x256 -= (src->aya_speed);/*fps_factor*/	 /*(t256(4.0)+(difficulty<<8))*/
		if (0 > src->x256)
		{
			src->boss_base_state001=SS05/*SS05*/;
		}
		break;
//-------- 分岐 [SS04]
	case SS04: /* 右移動中->SS08へ */
		sakuya_anime08(src);
		src->x256 += (src->aya_speed);/*fps_factor*/	/*(t256(4.0)+(difficulty<<8))*/
		#if 0/*SDL(左隅座標)*/
		if (src->x256 > (t256((GAME_WIDTH-50))-((src->w128+src->w128))) )
		#else/*Gu(中心座標)*/
		if (src->x256 > t256((GAME_WIDTH-50)) )
		#endif
		{
			src->boss_base_state001++/*SS05*/;
		}
		break;
//--------

		//	src->x256 += t256(1.0); 	// [***090114		追加
		//	src->x256 -= t256(1.0); 	// [***090114		追加


	case SS05: /* 左:待機->SS06へ */
		sakuya_wait_state(src/*,SS06*/);
		break;

	case SS06: /* 左:右回転攻撃->SS00, SS07 */
		#if (1==USE_CONTROLLER)
		enemy_boss02_sr_add1(src);/* 左:右回転攻撃 */
		enemy_boss02_sr_add2(src);/* 右:左回転攻撃 */
		#endif //(1==USE_CONTROLLER)

	//	/* (1/2)の確率で[攻撃A]へ分岐する。 */
	//	if (0==(ra_nd()&(2-1)))
		/* (1/4)の確率で[攻撃A]へ分岐する。 */
		if (0!=(ra_nd()&(4-1)))
		{
			/* (1/2)の確率で[攻撃A]へ分岐しない。 */
		//	src->boss_base_state001 = SS00;
			src->boss_base_state001 = SS00;
		}
		else
		{	src->boss_base_state001++/* = SS07*/;}
		break;
//-------- [攻撃A]
	case SS07: /* 左:前へ -> SS11 */
		src->y256 += (t256(1.0)+(src->aya_speed));/*fps_factor*/	/*(t256(4.0)+(difficulty<<8))*/
		if (src->y256 > t256(GAME_HEIGHT/3) )
		{
		//	src->boss_base_state001++;/* = SS11*/
			{
		//		-cv1024r((30)),
			/* CCWの場合 */
				obj_send1->x256 					= (src->x256);
				obj_send1->y256 					= (src->y256);
				br.BULLET_REGIST_speed256			= (src->aya_speed)-t256(2.0);			/* 弾速 */	/*t256(5.0)*/
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_div_angle1024		= cv1024r(360-(30));
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KOME_01_AOI+(ra_nd()&0x0f);	/* 弾グラ */	/* 弾に毒塗ってある設定 */
				br.BULLET_REGIST_n_way				= (5);									/* [5way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
			src->boss_base_state001 = SS00;
		}
		break;
//-------- 分岐 [SS08]
	case SS08: /* プレイヤー位置付近移動中 -> プレイヤー位置付近:大弾3つ -> SS00へ */
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
			s_bullet_create_aya_oodama3(src, ((src->aya_speed)>>1));
			/* 弾速 */	/*t256(3.0)*/	/*仕様変更*/	//t256(/*1.3*/3.0/*3.0*/),
			//	,20/*ransu512*/
			/* 大弾の追加 */
			src->boss_base_state001 = SS00; /* SS00へ */
		//	break;
		}
		else
		{
#if 1
			src->x256 += ((sin1024((src->aya_angle1024))*(src->aya_speed)/*(t256(4.0)+(difficulty<<8))*/)>>8);/*fps_factor*/	/* CCWの場合 */
			src->y256 += ((cos1024((src->aya_angle1024))*(src->aya_speed)/*(t256(4.0)+(difficulty<<8))*/)>>8);/*fps_factor*/
#endif
		}
		break;
	}
}


/*---------------------------------------------------------
	ボス行動
---------------------------------------------------------*/

static void move_aya(SPRITE *src)
{
//	ch eck_regist_spell_card(src);	/* スペカ登録可能なら登録 */
//	sp ell_card_generator222(src);	/* スペカ生成 */
	check_regist_generate_spell_card(src);	/* スペカ登録可能なら登録 / スペカ生成 */
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
	move_boss_effect(src);		/* 回エフェクト */
	danmaku_generator(src); 	/* 弾幕生成 */
//
	bullet_angle_all_hamidasi_check();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void boss_init_aya(SPRITE *h)
{
	h->callback_mover			= move_aya;
	h->callback_loser			= aya_put_items;

	h->wait1					= (50);
	src_animation_wait			= (0);
	h->aya_speed				= t256(4.0)+(difficulty<<8);
	h->ice_number				= (0);

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
//
//	bullet_clip_min.x256 = t256(-32);
//	bullet_clip_max.x256 = t256(0)+t256(GAME_WIDTH);
//	bullet_clip_min.y256 = t256(-256);
//	bullet_clip_max.y256 = t256(0)+t256(GAME_HEIGHT);
	bullet_clip_min.x256 = t256(-100);
	bullet_clip_max.x256 = t256(100)+t256(GAME_WIDTH);
	bullet_clip_min.y256 = t256(-256);
	bullet_clip_max.y256 = t256(100)+t256(GAME_HEIGHT);
}
