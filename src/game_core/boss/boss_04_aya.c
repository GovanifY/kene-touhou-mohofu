
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
	差分氏：自分で思ったよりも第2形態の攻撃パターンがカオスになってしまった。
	差分氏：もっと丁寧に作り直した方がいいかも。
	差分氏：天狗の速度を難易度ごとに変えてみた。
---------------------------------------------------------*/


/* 1なのは随分古い仕様。現在はシステムで ketm 方式の CONTROLLER は無い。
がそれに代わる東方風 方式の CONTROLLER は作るかもしれない。
( CONTROLLER ってのは、ボスの周りふわふわして弾出すアレ。) */
#define USE_CONTROLLER (0)



	#if 0
			if (0==aaa) 	{		HATSUDAN_01_speed256		= (t256(2.0))+(difficulty<<8);	}// = (src->aya_speed)-t256(2.0);						}
	else	if (1==aaa) 	{		HATSUDAN_01_speed256		= (t256(2.0))+(difficulty<<7);	}// = ((src->aya_speed)>>1);							}
	else					{		HATSUDAN_01_speed256		= (t256(4.0))+(difficulty<<7);	}// = /*仕様変更*/(t256(4.0)+(difficulty<<7));/*, 10*/	}
//							{		HATSUDAN_01_speed256		= (t256(3.0))+(difficulty<<7);	}
	#endif


/*---------------------------------------------------------
	天狗用の大玉弾(青)。乱数で怪しげな動きをする。
	-------------------------------------------------------
	見た目＞＞あたり判定です。
--------------------------------------------------------*/
enum
{
	AYA_KOUGEKI_TYPE_00_NONE = (0),
	AYA_KOUGEKI_TYPE_01_RENDAN, 	// (1)
	AYA_KOUGEKI_TYPE_02_FUDADAN,	// (2)
	AYA_KOUGEKI_TYPE_03_OODAMA, 	// (3)
};
#define AYA_KOUGEKI_TYPE_03_OODAMA_23	AYA_KOUGEKI_TYPE_03_OODAMA
#define AYA_KOUGEKI_TYPE_03_OODAMA_22	AYA_KOUGEKI_TYPE_03_OODAMA
#define AYA_KOUGEKI_TYPE_03_OODAMA_21	AYA_KOUGEKI_TYPE_03_OODAMA

/*---------------------------------------------------------
	攻撃タイプ
---------------------------------------------------------*/
static int kougeki_type;

static void aya_kougeki_all(SPRITE *src)
{
	//(r35でカードの場合のみ省略可能)	set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
	set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
	calculate_jikinerai();
	#if 0/*(r35のシステムでは、省略可能)*/
	HATSUDAN_03_angle65536				= (HATSUDAN_03_angle65536); 	/* [破壊] */
	#endif
	//
//	if (AYA_KOUGEKI_TYPE_00_NONE==kougeki_type)
//	{
//	}
//	else
	if (AYA_KOUGEKI_TYPE_01_RENDAN==kougeki_type)	/* 攻撃タイプ00:	米連弾 */
	{	voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);/*テキトー*/
		HATSUDAN_02_speed_offset			= t256(1);/* ？ */
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		{	/* 毎回 */
			s32 aaa;
			aaa = ((REG_0f_GAME_DIFFICULTY)<<6);/*src->aya_speed*/
			aaa += (t256(4.5)); 		/* 3.5だとeasyが難しすぎる。 t256(3.5) */
			const s32 ggg_angle 				= (HATSUDAN_03_angle65536); 	/* [破壊] */
			unsigned int jj;
			for (jj=(0); jj<(8+((unsigned)REG_0f_GAME_DIFFICULTY)); jj++)
			{
				aaa -= (t256(0.125));
				/* 弾に毒塗ってある設定 */
				HATSUDAN_01_speed256			= (aaa);									/* 弾速 */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_KOME_BASE) + (ra_nd()&0x0f);		/* 弾グラ */
				HATSUDAN_03_angle65536			= (ggg_angle);		/* [破壊] */
				HATSUDAN_06_n_way				= (11-jj);			/* [破壊] */			/* [10way] */
				HATSUDAN_07_div_angle65536		= (int)(65536/24);	/* [破壊] */
				hatudan_system_regist_n_way();/* (r33-) */
			}
		}
	}
	else
	if (AYA_KOUGEKI_TYPE_02_FUDADAN==kougeki_type)	/* 攻撃タイプ01:	後追い札弾 */
	{	/*(札弾は意図的に遅くする)*/
	//	HATSUDAN_01_speed256			= (t256(2.0)+((REG_0f_GAME_DIFFICULTY)<<8));	/* 弾速 */	/*t256(5.0)*/
		HATSUDAN_01_speed256			= (t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<4));	/* 弾速 */	/*t256(5.0)*/
		HATSUDAN_02_speed_offset		= t256(1);/* ？ */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
	//	HATSUDAN_05_bullet_obj_type 	= BULLET_KOME_BASE + (ra_nd()&0x0f);	/* 弾グラ */	/* 弾に毒塗ってある設定 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_OFUDA12_00_AOI) + (src->BOSS_DATA_06_aya_atooidan_type);	/* 弾グラ */	/* 弾に毒塗ってある設定 */
		HATSUDAN_06_n_way				= (5);			/* [破壊] */	/* [5way] */
		HATSUDAN_07_div_angle65536		= (65536/12);	/* [破壊] */	/* 12分割 == (30/360)[度] */
		hatudan_system_regist_n_way();/* (r33-) */
	}
	else
//	if (AYA_KOUGEKI_TYPE_03_OODAMA==kougeki_type)	/* 攻撃タイプ02:	大玉弾 */
	{	/*(連弾より若干遅くする)*/
	//	HATSUDAN_01_speed256			= speed256; 			/* 弾速 */
	//	HATSUDAN_01_speed256			= (t256(4.0)-((REG_0f_GAME_DIFFICULTY)<<7));			/* 弾速 */
		HATSUDAN_01_speed256			= (t256(3.5)-((REG_0f_GAME_DIFFICULTY)<<6));			/* 弾速 */
		HATSUDAN_02_speed_offset		= t256(-2);/* 減速弾？ */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		const s32 aaa					= ((u32)((((HATSUDAN_03_angle65536)>>8)&(0x03))));
		HATSUDAN_05_bullet_obj_type 	= (BULLET_OODAMA32_00_AOI+(aaa));	/* 弾グラ */
		HATSUDAN_06_n_way				= (12); 						/* [12way] */
		HATSUDAN_07_div_angle65536		= (65536/12);					/* 12分割 == (30/360)[度] */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
	}
}

/*---------------------------------------------------------
	ボス行動、第 5形態
	移動攻撃パターン2(高速移動)
---------------------------------------------------------*/

#define SS_TAN_I_JIKAN (4096)
/*
---0
退避
---1
---2
---3

*/

global void aya_05_keitai(SPRITE *src)
{
	const u8 sst[8] = { 0, 0, 0, 1, 1, 1, 2, 2 }; /* (1/3)の確率で分岐する。 */
	src->BOSS_DATA_05_boss_base_state777++;
	if ( (SS_TAN_I_JIKAN*1) > src->BOSS_DATA_05_boss_base_state777) 	/* [A]退避 */
	{
	//	ca se SS00: /* 不定:初期位置情報の取得->SS02へ */
	//	ca se SS02: /* 不定:初期位置へ戻る->SS03へ */
		src->BOSS_DATA_00_target_x256 = t256((GAME_WIDTH-50)/2);
	//	src->BOSS_DATA_01_target_y256 = t256(20.0);
		src->BOSS_DATA_01_target_y256 = t256(30.0); /* */
	//	if (/*150*/200 > src->BOSS_DATA_04_toutatu_wariai256 )	/* 時間で */
		if (t256(31.0) > src->cy256)
		{
			src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
			src->BOSS_DATA_05_boss_base_state777 = (SS_TAN_I_JIKAN*1);
			kougeki_type = AYA_KOUGEKI_TYPE_01_RENDAN;	/* 攻撃 */
		}
	}
	else
	if ( (SS_TAN_I_JIKAN*2) > src->BOSS_DATA_05_boss_base_state777) /* [B]移動 */
	{
	//	ca se SS03: /* 初期位置:大弾3つ->SS02, SS02, SS04 */
		if (/*150*/240 > src->BOSS_DATA_04_toutatu_wariai256 )	/* 時間で */
		{
			src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
			src->BOSS_DATA_05_boss_base_state777 = (SS_TAN_I_JIKAN*3);
			kougeki_type = AYA_KOUGEKI_TYPE_03_OODAMA_23;	/* 攻撃 */
					/*t256(5.0)*/ /* 弾速 */		/*仕様変更*/	// t256(/*1.5*/5.0/*5.0*/)
			{
				src->BOSS_DATA_06_aya_atooidan_type = sst[((ra_nd()&(8-1)))];	/* SS02, SS02, SS04 */
			}
			if (0 == src->BOSS_DATA_06_aya_atooidan_type)
			{
				SPRITE *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
			//	src->BOSS_DATA_00_target_x256 = t256(GAME_WIDTH/2); /* 真中へワープ */
				src->BOSS_DATA_00_target_x256 = zzz_player->cx256;
				src->BOSS_DATA_01_target_y256 = zzz_player->cy256;
			//	#define KYORI_AAA (t256(128.0)+((cg_game_di fficulty)<<(8+4/*5*/)))
				#define KYORI_AAA (t256(128.0)+((3)<<(8+4/*5*/)))
				if (src->BOSS_DATA_01_target_y256 > (signed)KYORI_AAA)
				{
					src->BOSS_DATA_01_target_y256 = KYORI_AAA;
				}
				#undef KYORI_AAA
			}
			else
			if (1 == src->BOSS_DATA_06_aya_atooidan_type)
			{
				src->BOSS_DATA_00_target_x256 = t256(0); /* */
				src->cy256 += t256(6.0);/*fps_factor*/	/* よくわかんない(???) */
			}
			else
		//	if (2 == src->BOSS_DATA_06_aya_atooidan_type)
			{
				src->BOSS_DATA_00_target_x256 = t256((GAME_WIDTH-50)); /* */
				src->cy256 += t256(6.0);/*fps_factor*/	/* よくわかんない(???) */
			}
		}
	}
	else
//	if (SS02 ==src->BOSS_DATA_05_boss_base_state777)	/* 攻撃 */
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
		if (/*150*/220 > src->BOSS_DATA_04_toutatu_wariai256 )	/* 時間で */
		{
			src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
		//	src->BOSS_DATA_05_boss_base_state777++; //	sakuya_wait_state_bbb(src/*,SS06*/);
			src->BOSS_DATA_05_boss_base_state777 = (SS_TAN_I_JIKAN*0); /* SS00へ */
			//ca se SS06: /* 左:右回転攻撃->SS00, SS03 */
				u8 zzz_aaa;
			{
				zzz_aaa = sst[((ra_nd()&(8-1)))];	/* SS02, SS02, SS04 */
			}
			if (zzz_aaa == 0)
			{
				kougeki_type = AYA_KOUGEKI_TYPE_02_FUDADAN; 	/* 攻撃 */
				src->BOSS_DATA_01_target_y256 = t256(GAME_HEIGHT/3); /* */
			}
			else
			#if (1==USE_CONTROLLER)
			if (zzz_aaa == 1)
			{
				kougeki_type = AYA_KOUGEKI_TYPE_03_OODAMA_21;	/* 攻撃 */
				#if (1==USE_CONTROLLER)
				eee_boss02_sr_add1(src);/* 左:右回転攻撃 */
				eee_boss02_sr_add2(src);/* 右:左回転攻撃 */
				#endif //(1==USE_CONTROLLER)
			}
			else
			#endif //(1==USE_CONTROLLER)
		//	if (zzz_aaa == 2)
			{
				kougeki_type = AYA_KOUGEKI_TYPE_03_OODAMA_22;	/* 攻撃 */
			}
		}
	}
	if (AYA_KOUGEKI_TYPE_00_NONE!=kougeki_type)
	{
		aya_kougeki_all(src);
		kougeki_type = AYA_KOUGEKI_TYPE_00_NONE;
	}
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
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

