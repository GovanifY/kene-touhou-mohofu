
#include "boss.h"

/*static*/extern void add_zako_aya_doll(SPRITE *src);/* 椛弾 */

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	射命丸 文
	Syameimaru Aya.
	-------------------------------------------------------
	-------------------------------------------------------
	自分で思ったよりも第2形態の攻撃パターンがカオスになってしまった。
	もっと丁寧に作り直した方がいいかも。
	天狗の速度を難易度ごとに変えてみた。
//
	差分氏の乱数大弾(文用の揺らぐ大弾)について私見(231)
	乱数大弾は (easy＋高速弾) 辺りまでなら面白いのですが、
	試行錯誤した結果、
	hard, lunatic あたりの弾幕シューを作るとして、
	弾密度をあげられないので、東方っぽくならないです。
	そういう理由で止めました。
---------------------------------------------------------*/


/* 1なのは随分古い仕様。現在はシステムで ketm 方式の CONTROLLER は無い。
がそれに代わる東方風 方式の CONTROLLER は作るかもしれない。
( CONTROLLER ってのは、ボスの周りふわふわして弾出すアレ。) */
#define USE_CONTROLLER (0)


/*---------------------------------------------------------
	天狗用の大玉弾(青)。乱数で怪しげな動きをする。
	-------------------------------------------------------
	見た目＞＞あたり判定です。
--------------------------------------------------------*/

#define bullet_create_aya_kougeki_23	s_bullet_create_aya_oodama3
#define bullet_create_aya_kougeki_22	s_bullet_create_aya_oodama3
#define bullet_create_aya_kougeki_21	s_bullet_create_aya_oodama3
static void s_bullet_create_aya_oodama3(SPRITE *src)
{
	send1_xy(src);	/* 弾源x256 y256 中心から発弾。 */
	//
	br.BULLET_REGIST_06_n_way					= (12); 				/* [12way] */
	br.BULLET_REGIST_07_div_angle65536			= (65536/12);			/* 分割角度 */	/* 30度づつ一周(12==360/30だから12方向) */
	//
//	br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
	br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
//
	tmp_angleCCW65536_jiki_nerai(src);
	br.BULLET_REGIST_02_angle65536				= (src->tmp_angleCCW65536);
	const u32 aaa								= ((u32)((((src->tmp_angleCCW65536)>>8)&(0x03))));
	br.BULLET_REGIST_04_bullet_obj_type 		= (BULLET_OODAMA32_00_AOI+(aaa));	/* 弾グラ */
//	br.BULLET_REGIST_00_speed256				= speed256; 			/* 弾速 */
	br.BULLET_REGIST_00_speed256				= (t256(4.0)-((cg_game_difficulty)<<7));			/* 弾速 */
	br.BULLET_REGIST_01_speed_offset			= t256(-2);/* 減速弾？ */
	tama_system_regist_katayori_n_way();/* (r33-) */
}
	#if 0
			if (0==aaa) 	{		br.BULLET_REGIST_00_speed256			= (t256(2.0))+(difficulty<<8);	}//= (src->aya_speed)-t256(2.0);						}
	else	if (1==aaa) 	{		br.BULLET_REGIST_00_speed256			= (t256(2.0))+(difficulty<<7);	}//= ((src->aya_speed)>>1); 						}
	else					{		br.BULLET_REGIST_00_speed256			= (t256(4.0))+(difficulty<<7);	}//= /*仕様変更*/(t256(4.0)+(difficulty<<7));/*, 10*/	}
	#endif
//							{		br.BULLET_REGIST_00_speed256			= (t256(3.0))+(difficulty<<7);	}





/*---------------------------------------------------------
	?弾
---------------------------------------------------------*/

static void bullet_create_aya_kougeki_01(SPRITE *src)
{
	send1_xy(src);	/* 弾源x256 y256 中心から発弾。 */
//	h->aya_speed								= t256(4.0)+(difficulty<<8);
//	br.BULLET_REGIST_00_speed256				= (src->aya_speed)-t256(2.0);				/* 弾速 */	/*t256(5.0)*/
	br.BULLET_REGIST_00_speed256				= (t256(2.0)+((cg_game_difficulty)<<8));				/* 弾速 */	/*t256(5.0)*/
	br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN;
//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
	br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_00_SIRO + (ra_nd()&0x0f); 	/* 弾グラ */	/* 弾に毒塗ってある設定 */
	br.BULLET_REGIST_06_n_way					= (5);										/* [5way] */
	br.BULLET_REGIST_07_VECTOR_div_angle1024	= cv1024r(30);//cv1024r(360-(30));			/* CCWの場合 */
	bullet_regist_multi_vector_direct();
}
/*---------------------------------------------------------
	?弾
---------------------------------------------------------*/

static void bullet_create_aya_kougeki_00(SPRITE *src)
{
	voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);/*テキトー*/
//
	send1_xy(src);	/* 弾源x256 y256 中心から発弾。 */
	br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN;
//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/24);
	/* 毎回 */
	{
		unsigned int aaa;
		aaa = ((cg_game_difficulty)<<6);/*src->aya_speed*/
		aaa += (t256(4.5)); 		/* 3.5だとeasyが難しすぎる。 t256(3.5) */
		unsigned int jj;
		for (jj=(0); jj<(8+(cg_game_difficulty)); jj++ )
		{
			aaa -= (t256(0.125));
			/* 弾に毒塗ってある設定 */
			br.BULLET_REGIST_00_speed256			= (aaa);										/* 弾速 */
			br.BULLET_REGIST_04_bullet_obj_type = BULLET_KOME_00_SIRO + (ra_nd()&0x0f); 	/* 弾グラ */
			br.BULLET_REGIST_06_n_way				= (11-jj);										/* [10way] */
			bullet_regist_multi_vector_direct();
		}
	}
}


/*---------------------------------------------------------
	ボス行動、第 5形態
	移動攻撃パターン2(高速移動)
---------------------------------------------------------*/

enum
{
	SS00 = 0,	/* 退避 */
	SS01,		/* 移動 */
	SS02,		/* 攻撃 */
};

global void aya_05_keitai(SPRITE *src)
{
	if (SS00 ==src->boss_base_state777) 	/* 退避 */
	{
	//	ca se SS00: /* 不定:初期位置情報の取得->SS02へ */
	//	ca se SS02: /* 不定:初期位置へ戻る->SS03へ */
		src->target_x256 = t256((GAME_WIDTH-50)/2);
	//	src->target_y256 = t256(20.0);
		src->target_y256 = t256(30.0); /* */
	//	if (/*150*/200 > src->toutatu_wariai256 )	/* 時間で */
		if (t256(31.0) > src->cy256)
		{
			src->toutatu_wariai256 = t256(1.00);
			src->boss_base_state777++;/* = SS03*/
			bullet_create_aya_kougeki_00(src);	/* 攻撃 */
		}
	}
	else
	if (SS01 ==src->boss_base_state777) 	/* 移動 */
	{
	//	ca se SS03: /* 初期位置:大弾3つ->SS02, SS02, SS04 */
		if (/*150*/240 > src->toutatu_wariai256 )	/* 時間で */
		{
			src->toutatu_wariai256 = t256(1.00);
			src->boss_base_state777++;
			bullet_create_aya_kougeki_23(src);	/* 攻撃 */
		//	int angle_jikinerai_1024;
		//	angle_jikinerai_1024 = src->tmp_angleCCW1024;
					/*t256(5.0)*/ /* 弾速 */		/*仕様変更*/	// t256(/*1.5*/5.0/*5.0*/)
			u8 zzz_aaa;
			{
				const u8 sst[8] = { 0, 0, 0, 1, 1, 1, 2, 2 }; /* (1/3)の確率で分岐する。 */
				zzz_aaa = sst[((ra_nd()&(8-1)))];	/* SS02, SS02, SS04 */
			}
			if (zzz_aaa == 0)
			{
				SPRITE *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
			//	src->target_x256 = t256(GAME_WIDTH/2); /* 真中へワープ */
				src->target_x256 = zzz_player->cx256;
				src->target_y256 = zzz_player->cy256;
				#define KYORI_AAA (t256(128.0)+((cg_game_difficulty)<<(8+4/*5*/)))
				if (src->target_y256 > (signed)KYORI_AAA)
				{
					src->target_y256 = KYORI_AAA;
				}
				#undef KYORI_AAA
			}
			else
			if (zzz_aaa == 1)
			{
				src->target_x256 = t256(0); /* */
				src->cy256 += t256(6.0);/*fps_factor*/	/* よくわかんない(???) */
			}
			else
		//	if (zzz_aaa == 2)
			{
				src->target_x256 = t256((GAME_WIDTH-50)); /* */
				src->cy256 += t256(6.0);/*fps_factor*/	/* よくわかんない(???) */
			}
		}
	}
	else
//	if (SS02 ==src->boss_base_state777) 	/* 攻撃 */
	{
//-------- 分岐 [SS02]
//-------- 分岐 [SS02]
//-------- [攻撃A]
//-------- 分岐 [SS04]
//		//	/* (1/2)の確率で[攻撃A]へ分岐する。 */
//		//	if (0==(ra_nd()&(2-1)))
//			/* (1/4)の確率で[攻撃A]へ分岐する。 */
//			{
//				/* (1/2)の確率で[攻撃A]へ分岐しない。 */
//			}
//	ca se SS02: /* 左移動中->SS05へ */
//	ca se SS02: /* 右移動中->SS04へ */
//	ca se SS05: /* 左:待機->SS06へ */
//	ca se SS03: /* 左:前へ -> SS11 */
//	ca se SS04: /* プレイヤー位置付近移動中 -> プレイヤー位置付近:大弾3つ -> SS00へ */
//		sakuya_anime00(src);//		sakuya_anime08(src);
		/* ayaの稼動範囲 */
		if (/*150*/220 > src->toutatu_wariai256 )	/* 時間で */
		{
			src->toutatu_wariai256 = t256(1.00);
		//	src->boss_base_state777++;	//	sakuya_wait_state_bbb(src/*,SS06*/);
			src->boss_base_state777 = SS00; /* SS00へ */
			//ca se SS06: /* 左:右回転攻撃->SS00, SS03 */
				u8 zzz_aaa;
			{
				const u8 sst[8] = { 0, 0, 0, 1, 1, 1, 2, 2 }; /* (1/3)の確率で分岐する。 */
				zzz_aaa = sst[((ra_nd()&(8-1)))];	/* SS02, SS02, SS04 */
			}
			if (zzz_aaa == 0)
			{
				bullet_create_aya_kougeki_21(src);	/* 攻撃 */
				#if (1==USE_CONTROLLER)
				eee_boss02_sr_add1(src);/* 左:右回転攻撃 */
				eee_boss02_sr_add2(src);/* 右:左回転攻撃 */
				#endif //(1==USE_CONTROLLER)
			}
			else
			if (zzz_aaa == 1)
			{
				bullet_create_aya_kougeki_01(src);	/* 攻撃 */
				src->target_y256 = t256(GAME_HEIGHT/3); /* */
			}
			else
		//	if (zzz_aaa == 2)
			{
				bullet_create_aya_kougeki_22(src);	/* 攻撃 */
			}
		//	break;
		}
	}
	#if 1
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
	#endif
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}



/*---------------------------------------------------------
	ボス行動、第 4形態
	移動パターン1(低速移動)
	-------------------------------------------------------
	ボス行動、第 3形態
	移動パターン0(低速移動)
---------------------------------------------------------*/

/*---------------------------------------------------------
	ボス行動、第 2形態
	移動パターン2(華符「芳華絢爛」のテスト)
	-------------------------------------------------------
	ボス行動、第 1形態
	移動パターン(「通常攻撃3」のテスト)
---------------------------------------------------------*/

