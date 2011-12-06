
#include "boss.h"

extern OBJ_CALL_FUNC(boss_move_00_card_settei_seisei);
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	魅魔
	Mitei Madakimattenai.
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	ボス行動、第 1形態
---------------------------------------------------------*/

global OBJ_CALL_FUNC(boss_move_10_mima_keitai)
{
	static int mima_zako_tuika_timer = 0;	/* 雑魚、追加タイマー。追加間隔をあまり短くしない。 */
	if (0 < mima_zako_tuika_timer)
	{
		mima_zako_tuika_timer--;
	}
	//
	static int mima_jikan;	/* カード追加タイマー。テキトー。 */
	mima_jikan--;
	/* カードを撃ってない場合に追加 */
//	if (SPELL_00==card.spell_used_number)	/* スペル生成終了ならカード生成 */
	if (0 > mima_jikan) 					/* スペル生成終了ならカード生成 */
	{
		const unsigned char aa_ra_nd = ra_nd();
		if (0==(aa_ra_nd&0x03))
		{
			if (0==mima_zako_tuika_timer)
			{
			//	mima_zako_tuika_timer = (128);
				mima_zako_tuika_timer = (256);
				init_set_dolls_mima_T04(src);/*(使い魔システム)*/
			}
		}
		mima_jikan = /*(123)*/(64) + (aa_ra_nd&0x3f);
		//
		src->BOSS_DATA_05_boss_base_state777++;
		src->BOSS_DATA_05_boss_base_state777 &= (8-1);
	//	if (0!=(src->BOSS_DATA_05_boss_base_state777))/*(r32)*/
		if (0!=(aa_ra_nd & 0xc0))/*(r33)*/
		{
			static const POINT_u16 hosi_no_zahyou[(8)] = /* 目標座標 */
			{	/* {(x),(y)}, */
				{(171),( 18)},	/* 南斗水鳥星へ */
				{(202),( 92)},	/* 北斗七星(5)へ */
				{(257),(107)},	/* 北斗七星(6)へ */
				{(339),( 74)},	/* 北斗七星(7)へ */
				{(	5),(116)},	/* 北斗七星(1)へ */
				{(	6),( 46)},	/* 北斗七星(2)へ */
				{(107),( 26)},	/* 北斗七星(3)へ */
				{(133),( 78)},	/* 北斗七星(4)へ */
			};
			/* 移動座標を決める */
		//	src->BOSS_DATA_00_target_x256 = ((hosi_no_zahyou[src->BOSS_DATA_05_boss_base_state777].x)<<8);
		//	src->BOSS_DATA_01_target_y256 = ((hosi_no_zahyou[src->BOSS_DATA_05_boss_base_state777].y)<<8);
			REG_02_DEST_X	= ((hosi_no_zahyou[src->BOSS_DATA_05_boss_base_state777].x)<<8);/*(t256()形式)*/
			REG_03_DEST_Y	= ((hosi_no_zahyou[src->BOSS_DATA_05_boss_base_state777].y)<<8);/*(t256()形式)*/
			boss_set_new_position(src);/*(誘導移動座標を設定)*/
			/* カードを決める */
			card_maikai_init_and_get_spell_number(src); 	/* 現在の形態で撃てるカード番号をカードシステムに設定。 */
		}
		else
		{
			/* 移動座標を決める */
			OBJ *zzz_player;
			zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
		//	src->BOSS_DATA_00_target_x256 = (zzz_player->center.x256);
		//	src->BOSS_DATA_01_target_y256 = (zzz_player->center.y256);
			REG_02_DEST_X	= (zzz_player->center.x256);/*(t256()形式)*/
			REG_03_DEST_Y	= (zzz_player->center.y256);/*(t256()形式)*/
			boss_set_new_position(src);/*(誘導移動座標を設定)*/
			#if 1
			/* スペルを決める */
			card.spell_used_number	= SPELL_19_mima_sekkin; 	/* カードをセット */
			card_maikai_init(src);/*(r35-, カードの初期化。カードが変わると毎回行う必要がある)*/
			#endif
		}
	//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* 攻撃アニメーション */
	}
//r36たぶん。	boss_re gacy_yuudou(src);
}


//#include "boss.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	蓬莱山 輝夜
	Houraisan Kaguya.
	-------------------------------------------------------
---------------------------------------------------------*/

static OBJ_CALL_FUNC(kaguya_common_shot_check)
{
	/* 移動方向を決める */
	src->BOSS_DATA_05_boss_base_state777++;
	src->BOSS_DATA_05_boss_base_state777 &= (4-1);
	#if (1)
	if ((/*(unsigned)*/REG_0f_GAME_DIFFICULTY) >= /*(unsigned)*/(src->BOSS_DATA_05_boss_base_state777) )/*(直前の論理ANDで必ずunsignedが保証されるからunsigned比較を使う)*/
	{
		card_maikai_init_and_get_spell_number(src);
	}
	#endif
}

/*---------------------------------------------------------
	ボス行動、第 4形態
	斜め移動
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。()
	REG_0e_REG6 	目標座標 x256
---------------------------------------------------------*/

global OBJ_CALL_FUNC(boss_move_12_kaguya_funya_idou)
{
	/* カードを撃ってない場合に追加 */
	if (SPELL_00==card.spell_used_number)	/* スペル生成終了ならカード生成 */
	{
		kaguya_common_shot_check(src);
		/* 気分で目標を決める。 */
		REG_0e_REG6 = ((ra_nd()&0xffff))+(t256(48));
	}
	REG_08_REG0++;
	if (8 < REG_08_REG0)
	{
		REG_08_REG0 = 0;
		/* 目標にちかづこうと努力。 */
		src->BOSS_DATA_00_target_x256 += ((src->BOSS_DATA_00_target_x256 < REG_0e_REG6) ? t256(8) : -t256(8) );
		src->BOSS_DATA_00_target_x256 &= 0xffff;
	{
		int sin_value_t256; 		//	sin_value_t256 = 0;
		int cos_value_t256; 		//	cos_value_t256 = 0;
		int256_sincos1024( (((src->BOSS_DATA_00_target_x256>>4)/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
		src->BOSS_DATA_01_target_y256 = (t256(25)+( (sin_value_t256)<<6));
	}
		REG_02_DEST_X	= (src->BOSS_DATA_00_target_x256);
		REG_03_DEST_Y	= (src->BOSS_DATA_01_target_y256);
		boss_set_new_position(src);/*(誘導移動座標を設定)*/
	//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);
	}
//r36たぶん。	boss_re gacy_yuudou(src);
}


/*---------------------------------------------------------
	ボス行動、第 1形態
	微上下移動
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。()
---------------------------------------------------------*/

global OBJ_CALL_FUNC(boss_move_11_kaguya_yureru)
{
	/* カードを撃ってない場合に追加 */
	if (SPELL_00==card.spell_used_number)	/* スペル生成終了ならカード生成 */
	{
		kaguya_common_shot_check(src);
	}
	REG_08_REG0 += (10);
//	mask1024(REG_08_REG0);
	{
		int sin_value_t256; 		//	sin_value_t256 = 0;
		int cos_value_t256; 		//	cos_value_t256 = 0;
		int256_sincos1024( ((((REG_08_REG0))/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
		src->center.y256 = (t256(25)+( ((sin_value_t256))<<2));
	}
//
}

//#include "boss.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	射命丸 文
	Syameimaru Aya.
	-------------------------------------------------------
	差分氏の作った攻撃をスペカにする。
	-------------------------------------------------------
	差分氏：自分で思ったよりも第2形態の攻撃パターンがカオスになってしまった。
	差分氏：もっと丁寧に作り直した方がいいかも。
	差分氏：天狗の速度を難易度ごとに変えてみた。
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。()
	REG_09_REG1 	攻撃タイプ(発弾部へ受け渡し用)
	REG_0a_REG2 	後追い弾の種類	aya_atooidan_type
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

static OBJ_CALL_FUNC(aya_kougeki_all)
{
	//(r35でカードの場合のみ省略可能)	set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
	set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
	calculate_jikinerai();
	#if 0/*(r35のシステムでは、省略可能)*/
	HATSUDAN_03_angle65536				= (HATSUDAN_03_angle65536); 	/* [破壊] */
	#endif
	//
//	if (AYA_KOUGEKI_TYPE_00_NONE==REG_09_REG1)
//	{
//	}
//	else
	if (AYA_KOUGEKI_TYPE_01_RENDAN==REG_09_REG1)	/* 攻撃タイプ00:	米連弾 */
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
	if (AYA_KOUGEKI_TYPE_02_FUDADAN==REG_09_REG1)	/* 攻撃タイプ01:	後追い札弾 */
	{	/*(札弾は意図的に遅くする)*/
	//	HATSUDAN_01_speed256			= (t256(2.0)+((REG_0f_GAME_DIFFICULTY)<<8));	/* 弾速 */	/*t256(5.0)*/
		HATSUDAN_01_speed256			= (t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<4));	/* 弾速 */	/*t256(5.0)*/
		HATSUDAN_02_speed_offset		= t256(1);/* ？ */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
	//	HATSUDAN_05_bullet_obj_type 	= BULLET_KOME_BASE + (ra_nd()&0x0f);		/* 弾グラ */	/* 弾に毒塗ってある設定 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_OFUDA12_00_AOI) + (REG_0a_REG2);	/* 弾グラ */	/* 弾に毒塗ってある設定 */
		HATSUDAN_06_n_way				= (5);			/* [破壊] */	/* [5way] */
		HATSUDAN_07_div_angle65536		= (65536/12);	/* [破壊] */	/* 12分割 == (30/360)[度] */
		hatudan_system_regist_n_way();/* (r33-) */
	}
	else
//	if (AYA_KOUGEKI_TYPE_03_OODAMA==REG_09_REG1)	/* 攻撃タイプ02:	大玉弾 */
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

/*(SS_TAN_I_JIKAN: need 2^n, 2のn乗で割り切れる必要あり)*/
#define SS_TAN_I_JIKAN (4096)
/*
---0
[A]退避座標設定
---1
[B]退避行動
---2
[C]攻撃移動
---3
[D]攻撃
---3+220 -> 0 へ。

*/
/*(文の幅50[pixel])*/
//#define AYA_OBJ_WIDTH50 (50)

/* 出現時x座標 */
//#define BOSS_XP256		(t256(GAME_WIDTH/2))	/* 中心座標なので */
#define BOSS_XP256			(t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH/2))	/* 中心座標なので */

global OBJ_CALL_FUNC(boss_move_13_aya_taifu)
{
	const u8 sst[8] = { 0, 0, 0, 1, 1, 1, 2, 2 }; /* (1/3)の確率で分岐する。 */
	REG_08_REG0++;
	if ((SS_TAN_I_JIKAN*1) > REG_08_REG0)	/* [A]退避座標設定 */
	{
		REG_08_REG0 = (SS_TAN_I_JIKAN*1);
	//	src->BOSS_DATA_00_target_x256 = t256(GAME_X_OFFSET)+t256((GAME_320_WIDTH-AYA_OBJ_WIDTH50)/2);
	//	src->BOSS_DATA_00_target_x256 = BOSS_XP256;
		src->BOSS_DATA_01_target_y256 = t256(30.0); /* t256(20.0) */
		/* 移動座標を決める */
		REG_02_DEST_X	= (src->BOSS_DATA_00_target_x256);/*(t256()形式)*/
		REG_03_DEST_Y	= (src->BOSS_DATA_01_target_y256);/*(t256()形式)*/
		boss_set_new_position(src);/*(誘導移動座標を設定)*/

	}
	if ((SS_TAN_I_JIKAN*2) > REG_08_REG0)	/* [B]退避行動 */
	{
	//	ca se SS00: /* 不定:初期位置情報の取得->SS02へ */
	//	ca se SS02: /* 不定:初期位置へ戻る->SS03へ */
	//	if (/*150*/200 > src->BOSS_DATA_04_toutatu_wariai256 )	/* 時間で */
		/*(攻撃条件[A])*/
		if (t256(31.0) > src->center.y256)
		{
//m 		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
			REG_08_REG0 = (SS_TAN_I_JIKAN*2);
			REG_09_REG1 = AYA_KOUGEKI_TYPE_01_RENDAN;	/* 攻撃 */
		}
	}
	else
	if ((SS_TAN_I_JIKAN*3) > REG_08_REG0) /* [C]攻撃移動 */
	{
	//	ca se SS03: /* 初期位置:大弾3つ->SS02, SS02, SS04 */
	//	if (/*150*/240 > src->BOSS_DATA_04_toutatu_wariai256 )	/* 時間で */
		/*(攻撃条件[B])*/
	//	if (/*150*/240+(SS_TAN_I_JIKAN*2) < REG_08_REG0 )	/* 時間で */
		if (/*150*/24-((REG_0f_GAME_DIFFICULTY)<<2)+(SS_TAN_I_JIKAN*2) < REG_08_REG0 )	/* 時間で */
		{
//m 		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
			REG_08_REG0 = (SS_TAN_I_JIKAN*3);
			REG_09_REG1 = AYA_KOUGEKI_TYPE_03_OODAMA_23;	/* 攻撃 */
					/*t256(5.0)*/ /* 弾速 */		/*仕様変更*/	// t256(/*1.5*/5.0/*5.0*/)
			{
				REG_0a_REG2 = sst[((ra_nd()&(8-1)))];	/* SS02, SS02, SS04 */
			}
			if (0 == REG_0a_REG2)
			{
				OBJ *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
			//	src->BOSS_DATA_00_target_x256 = BOSS_XP256; /* 真中へワープ */
				src->BOSS_DATA_00_target_x256 = zzz_player->center.x256;
				src->BOSS_DATA_01_target_y256 = zzz_player->center.y256;
			//	#define KYORI_AAA (t256(128.0)+((cg_game_di fficulty)<<(8+4/*5*/)))
				#define KYORI_AAA (t256(128.0)+((3)<<(8+4/*5*/)))
				if (src->BOSS_DATA_01_target_y256 > (signed)KYORI_AAA)
				{
					src->BOSS_DATA_01_target_y256 = KYORI_AAA;
				}
				#undef KYORI_AAA
			}
			else
			if (1 == REG_0a_REG2)
			{
				src->BOSS_DATA_00_target_x256 = t256(GAME_X_OFFSET); /* */
			//	src->center.y256 += t256(6.0);/*fps_factor*/	/* よくわかんない(???) */
				src->BOSS_DATA_01_target_y256 += t256(6.0);/*fps_factor*/	/* よくわかんない(???) */
			}
			else
		//	if (2 == REG_0a_REG2)
			{
			//	src->BOSS_DATA_00_target_x256 = t256((GAME_X_OFFSET+GAME_320_WIDTH-AYA_OBJ_WIDTH50)); /* */
				src->BOSS_DATA_00_target_x256 = t256((GAME_X_OFFSET+GAME_320_WIDTH)); /* */
			//	src->center.y256 += t256(6.0);/*fps_factor*/	/* よくわかんない(???) */
				src->BOSS_DATA_01_target_y256 += t256(6.0);/*fps_factor*/	/* よくわかんない(???) */
			}
		}
	}
	else
//	if (SS02 ==REG_08_REG0) 	/* [D]攻撃 */
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
	//	if (/*150*/220 > src->BOSS_DATA_04_toutatu_wariai256 )	/* 時間で */
		/*(攻撃条件[C])*/
	//	if (/*150*/220+(SS_TAN_I_JIKAN*3) < REG_08_REG0 )	/* 時間で */
		if (/*150*/22-((REG_0f_GAME_DIFFICULTY)<<2)+(SS_TAN_I_JIKAN*3) < REG_08_REG0 )	/* 時間で */
		{
//m 		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.00);
		//	REG_08_REG0++; //	sakuya_wait_state_bbb(src/*,SS06*/);
			REG_08_REG0 = (SS_TAN_I_JIKAN*0); /* SS00へ */
			//ca se SS06: /* 左:右回転攻撃->SS00, SS03 */
				u8 zzz_aaa;
			{
				zzz_aaa = sst[((ra_nd()&(8-1)))];	/* SS02, SS02, SS04 */
			}
			if (zzz_aaa == 0)
			{
				REG_09_REG1 = AYA_KOUGEKI_TYPE_02_FUDADAN;	/* 攻撃 */
				src->BOSS_DATA_01_target_y256 = t256(GAME_HEIGHT/3); /* */
			}
			else
			#if (1==USE_CONTROLLER)
			if (zzz_aaa == 1)
			{
				REG_09_REG1 = AYA_KOUGEKI_TYPE_03_OODAMA_21;	/* 攻撃 */
				#if (1==USE_CONTROLLER)
				eee_boss02_sr_add1(src);/* 左:右回転攻撃 */
				eee_boss02_sr_add2(src);/* 右:左回転攻撃 */
				#endif //(1==USE_CONTROLLER)
			}
			else
			#endif //(1==USE_CONTROLLER)
		//	if (zzz_aaa == 2)
			{
				REG_09_REG1 = AYA_KOUGEKI_TYPE_03_OODAMA_22;	/* 攻撃 */
			}
		}
	}
	if (AYA_KOUGEKI_TYPE_00_NONE!=REG_09_REG1)
	{
		aya_kougeki_all(src);
		REG_09_REG1 = AYA_KOUGEKI_TYPE_00_NONE;
		/* 移動座標を決める */
		REG_02_DEST_X	= (src->BOSS_DATA_00_target_x256);/*(t256()形式)*/
		REG_03_DEST_Y	= (src->BOSS_DATA_01_target_y256);/*(t256()形式)*/
		boss_set_new_position(src);/*(誘導移動座標を設定)*/
	}
//r36たぶん。	boss_re gacy_yuudou(src);
}
	/*(区切り時間の場合、座標設定)*/
//	if (0==(REG_08_REG0&(SS_TAN_I_JIKAN-1)))
//	{
//	}


//#include "boss.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	十六夜 咲夜
	Izayoi Sakuya.
	-------------------------------------------------------
	ToDo:
	移動と弾の処理を分離しよう。でないと訳わからん。
	ボスタイマー対応中
	-------------------------------------------------------
	★ボスタイマーはシステムでサポート予定です。(作業中)
---------------------------------------------------------*/


/*---------------------------------------------------------
	咲夜背後の魔方陣についてメモ
	-------------------------------------------------------
	5面中-ボスで出てくる時は、大きさ60x60(たぶん64x64obj)ぐらい。
	5面ボスで出てくる時は、大きさ125x125(たぶん128x128obj)ぐらい。
	模倣風は基本的に縮小率75%なので、咲夜背後の魔方陣は
	48x48[pixel](2倍拡大時は96x96[pixel])ぐらいが妥当かな？
---------------------------------------------------------*/



/*---------------------------------------------------------
	第10形態: 最終形態(その2)
---------------------------------------------------------*/

global OBJ_CALL_FUNC(boss_move_16_sakuya_nazo_keitai)
{
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
		src->center.x256 -= ((si n1024((src->tmp_angleCCW1024))*(t256(0.03)))>>8);/*fps_factor*/		/* CCWの場合 */
		src->center.y256 -= ((co s1024((src->tmp_angleCCW1024))*(t256(0.03)))>>8);/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (src->tmp_angleCCW1024), &sin_value_t256, &cos_value_t256);
			src->center.x256 -= ((sin_value_t256*(t256(0.03)))>>8);/*fps_factor*/
			src->center.y256 -= ((cos_value_t256*(t256(0.03)))>>8);/*fps_factor*/
		}
		#endif
	}
	//	[SAKUYA_ANIME_12_HATUDAN_START)
	/*(以下は将来的に共通化される予定)*/
	boss_move_00_card_settei_seisei(src);/*(カード生成終了ならカード設定し、生成。)*/
}


/*---------------------------------------------------------
	第九形態: 幻葬「フェスティバルナイフ」 最終形態(その1)
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。()
	REG_0c_REG4 	BO SS_DATA_07_sakuya_data_common_wait256
	-------------------------------------------------------
	REG_0e_REG6 	[発弾連絡レジスタ]
		0:	なし。
		1:	extern OBJ_CALL_FUNC(bullet_create_sakuya_ ryoute_knife); 咲夜 両手ナイフカード
		2:	extern OBJ_CALL_FUNC(bullet_create_sakuya_ kurukuru);     咲夜 くるくるナイフカード
	-------------------------------------------------------
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

global OBJ_CALL_FUNC(boss_move_15_sakuya_festival_knife)
{
	REG_08_REG0++;
	if (80 > REG_08_REG0) /* [A] 何もしない(待機時間) */
	{
		;/* 何もしない(待機時間) */
	}
	else
	if (81 > REG_08_REG0) /* [B] 待機時間が終わったら移動準備 */
	{
//++		pd->bo ssmode = B00_NONE/*B01_BA TTLE*/;
		//(r35でカードの場合のみ省略可能)	REG_02_DEST_X	= ((src->center.x256));
		//(r35でカードの場合のみ省略可能)	REG_03_DEST_Y	= ((src->center.y256));
		//(r35でカードの場合のみ省略可能)	set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
			REG_00_SRC_X	= (t256(GAME_X_OFFSET+(GAME_320_WIDTH/2))); 		/* 弾源x256 */
			REG_01_SRC_Y	= (t256(30));					/* 弾源y256 */
			tmp_angleCCW65536_src_nerai();
			src->BOSS_DATA_06_sakuya_data_move_angle1024 = (deg65536to1024(HATSUDAN_03_angle65536));	/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
	}
	else
	if (1024 > REG_08_REG0) /* [C] 真ん中に来るまで移動 */
	{
		if ( (src->center.x256 < t256(GAME_X_OFFSET+(GAME_320_WIDTH/2)+20) ) &&
			 (src->center.x256 > t256(GAME_X_OFFSET+(GAME_320_WIDTH/2)-20) ) &&
			 (src->center.y256 < t256(50.0)) ) /* 真ん中に来たら */
		{
			REG_08_REG0 = (1024+1024-1)-(20);// REG_08_REG0++;/* = SG02*/
			REG_0c_REG4 = 0;
			//	[SAKUYA_ANIME_03_CENTER_A;
		}
		else
		{
			#if (0)//
			src->center.x256 += ((si n1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.0)))>>8);/*fps_factor*/	/* CCWの場合 */
			src->center.x256 += ((si n1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.0)))>>8);/*fps_factor*/	/* CCWの場合 */
			src->center.y256 += ((co s1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.0)))>>8);/*fps_factor*/
			src->center.y256 += ((co s1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.0)))>>8);/*fps_factor*/
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (src->BOSS_DATA_06_sakuya_data_move_angle1024), &sin_value_t256, &cos_value_t256);
				src->center.x256 += ((sin_value_t256*(t256(3.0)))>>8);/*fps_factor*/
				src->center.y256 += ((cos_value_t256*(t256(3.0)))>>8);/*fps_factor*/
			}
			#endif
		}
	}
	else
	if ((1024+1024 ) > REG_08_REG0) /* [D] 何もしない(待機時間) */
	{
		;/* 何もしない(待機時間) */
	}
	else
	if ((1024+1024 +1) > REG_08_REG0) /* [E] 両手ナイフで攻撃 */
	{
		{
			//	[SAKUYA_ANIME_12_HATUDAN_START]
			//	[SAKUYA_ANIME_15_HATUDAN_END]
			REG_0e_REG6=(1);/*[咲夜 両手ナイフカード]*/
			if (src->center.y256 > t256(150))
		//	{	src->BOSS_DATA_06_sakuya_data_move_angle1024 = cv1024r(   210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;} 	/* CWの場合 */
		//	{	src->BOSS_DATA_06_sakuya_data_move_angle1024 = cv1024r(360-210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}	/* CCWの場合 */
			{	src->BOSS_DATA_06_sakuya_data_move_angle1024 = cv1024r(360-210) + (((ra_nd()&(4096-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
			else
			{	src->BOSS_DATA_06_sakuya_data_move_angle1024 = ((ra_nd()&(1024-1)));}
			#if 1
			/*念の為*/
			mask1024(src->BOSS_DATA_06_sakuya_data_move_angle1024);
			#endif
		}
	}
	else
	if ((1024+1024 +1+40) > REG_08_REG0) /* [F]移動 */
	{
		#if (0)//
		src->center.x256 += ((si n1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.5)))>>8);/*fps_factor*/	/* CCWの場合 */
		src->center.y256 += ((co s1024((src->BOSS_DATA_06_sakuya_data_move_angle1024))*(t256(3.5)))>>8);/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (src->BOSS_DATA_06_sakuya_data_move_angle1024), &sin_value_t256, &cos_value_t256);
			src->center.x256 += ((sin_value_t256*(t256(3.5)))>>8);/*fps_factor*/
			src->center.y256 += ((cos_value_t256*(t256(3.5)))>>8);/*fps_factor*/
		}
		#endif
	}
	else /* [G] 待機時間作成し、[D]に戻る。 */
	{
			REG_08_REG0 = (1024+1024-1)-(120)-(3*64)+(3<<6);
	}
	//
	OBJ *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
	#if 1/*Gu(中心座標)*/
	if ((zzz_player->center.x256 < src->center.x256 + t256(25)) &&	/* ???????????? */
		(zzz_player->center.x256 > src->center.x256 - t256(25)))
	#endif
	{
		REG_0c_REG4++;/*fps_factor*/
	}
	if (REG_0c_REG4 > 24)/*(21 r35u1)*/
	{
		REG_0c_REG4 	= 0;
		{
	//		src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);			/*	src->alpha			= 0x00;*/
			src->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x80);			/*	src->alpha			= 0x00;*/	/* 半透明 */
			src->tmp_angleCCW1024	= (0);
		}
		//	[SAKUYA_ANIME_12_HATUDAN_START]
		//	[SAKUYA_ANIME_15_HATUDAN_END]
		REG_0e_REG6=(2);/*[咲夜 くるくるナイフカード]*/
	}
	/*(以下は将来的に共通化される予定)*/
	boss_move_00_card_settei_seisei(src);/*(カード生成終了ならカード設定し、生成。)*/
//r36たぶん。	boss_re gacy_yuudou(src);
}


/*---------------------------------------------------------
	第02形態: 奇術「ミスディレクション」
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。()
//	REG_09_REG1 	BO SS_DATA_07_sakuya_data_common_wait256
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
/*(ゲーム画面のx座標中心位置)==(左端のパネル部分)+(ゲーム領域画面の半分)*/
#define SAKUYA_POINT_X_MID			(t256(GAME_X_OFFSET+(GAME_320_WIDTH/2)))

global OBJ_CALL_FUNC(boss_move_14_sakuya_miss_direction)
{
	if (/*(256-64)*/(255/*192+64*/) == REG_10_BOSS_SPELL_TIMER) /* 反転移動方向セット */
	{
		/* 第二形態では使って無いから利用(共用)する */
		#define sakuya_data_sayuu_mode BOSS_DATA_07_sakuya_data_common_wait256
		{	/* 咲夜さんの誘導ポイント座標 */
			src->sakuya_data_sayuu_mode ^= (2-1);/* 反転 */
			if (/*0==*/src->sakuya_data_sayuu_mode & (2-1))
					{	src->BOSS_DATA_00_target_x256 = SAKUYA_POINT_X_MID-(t256(128)); }//96==128*0.75
			else	{	src->BOSS_DATA_00_target_x256 = SAKUYA_POINT_X_MID+(t256(128)); }//
		}
		#if (1)
		/* 移動座標を決める */
	//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* 攻撃アニメーション */
		REG_02_DEST_X	= (src->BOSS_DATA_00_target_x256);/*(t256()形式)*/
		REG_03_DEST_Y	= (src->BOSS_DATA_01_target_y256);/*(t256()形式)*/
		boss_set_new_position(src);/*(誘導移動座標を設定)*/
		#endif
	}
	//
	else
//	if (/*(256-256)*/(59)/*((251+64)-256)*/ == REG_10_BOSS_SPELL_TIMER) /* 真中へワープ */
	if (/*(256-256)*/(59)/*((251+64)-256)*/ == REG_10_BOSS_SPELL_TIMER) /* 真中へワープ */
	{
		src->BOSS_DATA_00_target_x256 = src->center.x256 = SAKUYA_POINT_X_MID; /* 真中へワープ */
		#if (1)
		/* 移動座標を決める */
	//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* 攻撃アニメーション */
		REG_02_DEST_X	= (src->BOSS_DATA_00_target_x256);/*(t256()形式)*/
		REG_03_DEST_Y	= (src->BOSS_DATA_01_target_y256);/*(t256()形式)*/
		boss_set_new_position(src);/*(誘導移動座標を設定)*/
		#endif
	}
	else
	if (/*(256-256)*/(58)/*((250+64)-256)*/ == REG_10_BOSS_SPELL_TIMER) /* 反対側へワープ */
//	if (/*(256-256)*/(0) == REG_10_BOSS_SPELL_TIMER) /* 真中へワープ */
//	if (/*(256-192)*/(64) == REG_10_BOSS_SPELL_TIMER) /* 真中へワープ */
//	if (/*(256-180)*/(76) == REG_10_BOSS_SPELL_TIMER) /* 真中へワープ */
	{
	/*
		 反対側の座標==(左端のパネル部分)+(ゲーム領域画面)-(現在の座標-(左端のパネル部分));
		 反対側の座標==(左端のパネル部分)+(左端のパネル部分)+(ゲーム領域画面)-(現在の座標));
	*/
	//	src->BOSS_DATA_00_target_x256 = src->center.x256 = t256(GAME_X_OFFSET) + ( t256(GAME_320_WIDTH) - ( (src->center.x256) - t256(GAME_X_OFFSET) ) );	/* 反対側へワープ */
		src->BOSS_DATA_00_target_x256 = src->center.x256 = t256((GAME_X_OFFSET+GAME_X_OFFSET+GAME_320_WIDTH)) - (src->center.x256); 			/* 反対側へワープ */
		#if (1)
		/* 移動座標を決める */
	//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* 攻撃アニメーション */
		REG_02_DEST_X	= (src->BOSS_DATA_00_target_x256);/*(t256()形式)*/
		REG_03_DEST_Y	= (src->BOSS_DATA_01_target_y256);/*(t256()形式)*/
		boss_set_new_position(src);/*(誘導移動座標を設定)*/
		#endif
	}
	/*(以下は将来的に共通化される予定)*/
	boss_move_00_card_settei_seisei(src);/*(カード生成終了ならカード設定し、生成。)*/
//r36たぶん。	boss_re gacy_yuudou(src);
}

