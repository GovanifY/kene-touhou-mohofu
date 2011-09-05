
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	レミリア っぽいカードを定義します。
---------------------------------------------------------*/

local void remilia_oyasumi_kougeki(OBJ *src)
{
	// ■  [大玉弾]
//	HATSUDAN_01_speed256			= (t256(3.5));						/* 弾速 */
//	HATSUDAN_01_speed256			= (t256(2.25)); 					/* 弾速 */
	HATSUDAN_01_speed256			= (t256(6.00)); 					/* 弾速 */
//	HATSUDAN_02_speed_offset		= -t256(1);/* 減速弾？ */
	HATSUDAN_02_speed_offset		= -t256(4);/* 減速弾？ */
	HATSUDAN_03_angle65536			= (16384)+(((REG_10_BOSS_SPELL_TIMER)&0x40)<<8);/* 自機狙い角 */
	HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_1000_EFFECT_MIDDLE)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	HATSUDAN_05_bullet_obj_type 	= BULLET_OODAMA32_01_AKA;			/* [赤大玉弾] */
	hatudan_system_regist_single();
	// ■  [中玉弾]
	HATSUDAN_01_speed256			= (t256(4.00)); 					/* 弾速 */
	HATSUDAN_02_speed_offset		= -t256(2);/* 減速弾？ */
	HATSUDAN_05_bullet_obj_type 	= (BULLET_MARU10_BASE + TAMA_IRO_01_AKA);			/* [赤大中弾] */
	u32 ii;
	for (ii=0; ii<8; ii++)
	{
		HATSUDAN_03_angle65536		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/8)-1)) - ((65536/16)-1);/* 自機狙い角 */
		hatudan_system_regist_single();
	}
	// ■  [小玉弾]
	HATSUDAN_01_speed256			= (t256(2.00)); 					/* 弾速 */
	HATSUDAN_02_speed_offset		= -t256(1);/* 減速弾？ */
	HATSUDAN_05_bullet_obj_type 	= (BULLET_MINI8_BASE + TAMA_IRO_01_AKA);				/* [赤大中弾] */
	for (ii=0; ii<(u32)(0x02<<(REG_0f_GAME_DIFFICULTY)); ii++)/*(16)*/
	{
		HATSUDAN_03_angle65536		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/4)-1)) - ((65536/8)-1);/* 自機狙い角 */
		hatudan_system_regist_single();
	}
	#if (0)
//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
	bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
	#endif
}


local void remilia_shot00(OBJ *src)
{
	// ■  [大玉弾]
//	HATSUDAN_01_speed256			= (t256(3.5));						/* 弾速 */
//	HATSUDAN_01_speed256			= (t256(2.25)); 					/* 弾速 */
	HATSUDAN_01_speed256			= (t256(6.00)); 					/* 弾速 */
//	HATSUDAN_02_speed_offset		= -t256(1);/* 減速弾？ */
	HATSUDAN_02_speed_offset		= -t256(4);/* 減速弾？ */
	HATSUDAN_03_angle65536			= src->tmp_angleCCW65536;/* 自機狙い角 */
	HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_1000_EFFECT_MIDDLE)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	HATSUDAN_05_bullet_obj_type 	= BULLET_OODAMA32_01_AKA;			/* [赤大玉弾] */
	hatudan_system_regist_single();
	// ■  [中玉弾]
	HATSUDAN_01_speed256			= (t256(4.00)); 					/* 弾速 */
	HATSUDAN_02_speed_offset		= -t256(2);/* 減速弾？ */
	HATSUDAN_05_bullet_obj_type 	= (BULLET_MARU10_BASE + TAMA_IRO_01_AKA);			/* [赤大中弾] */
	u32 ii;
	for (ii=0; ii<8; ii++)
	{
		HATSUDAN_03_angle65536		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/8)-1)) - ((65536/16)-1);/* 自機狙い角 */
		hatudan_system_regist_single();
	}
	// ■  [小玉弾]
	HATSUDAN_01_speed256			= (t256(2.00)); 					/* 弾速 */
	HATSUDAN_02_speed_offset		= -t256(1);/* 減速弾？ */
	HATSUDAN_05_bullet_obj_type 	= (BULLET_MINI8_BASE + TAMA_IRO_01_AKA);				/* [赤大中弾] */
	for (ii=0; ii<(u32)(0x02<<(REG_0f_GAME_DIFFICULTY)); ii++)/*(16)*/
	{
		HATSUDAN_03_angle65536		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/4)-1)) - ((65536/8)-1);/* 自機狙い角 */
		hatudan_system_regist_single();
	}
	#if (1)
//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
	bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
	#endif
}


/*---------------------------------------------------------
	レミリア 通常攻撃2 っぽいもの
	-------------------------------------------------------
	1秒撃ったら、3秒休む(移動)。

---- ---- ---- ----
0000 0011 1111 1111
---------------------------------------------------------*/

local void spell_create_1f_remilia_kougeki2(OBJ *src)
{
	if (0x80==((REG_10_BOSS_SPELL_TIMER)&0x80))/* 後半 */
	{
	//	if (0xc0==((REG_10_BOSS_SPELL_TIMER)&0xc0))
		if (0xc0==((REG_10_BOSS_SPELL_TIMER)&0xc7))/* ８回に一回 */
		{
			#if 0
			/* 撃つターン 8弾 */
					if (0x0000==((REG_10_BOSS_SPELL_TIMER)&0x30))	{	remilia_shot00(src);	}	/* 自機狙い撃ち */
			else	if (0x0100==((REG_10_BOSS_SPELL_TIMER)&0x30))	{	remilia_shot00(src);	}	/* CW 撃ち */
			else	if (0x0200==((REG_10_BOSS_SPELL_TIMER)&0x30))	{	remilia_shot00(src);	}	/* CCW 撃ち */
			else/*	if (0x0300==((REG_10_BOSS_SPELL_TIMER)&0x30)) */	{	remilia_shot00(src);	}	/* 自機狙い撃ち */	/* 一気撃ち */
			#endif
			if (0==(((REG_10_BOSS_SPELL_TIMER)+0x100)&0x200))
			{
				calculate_jikinerai();/* 自機狙い角作成 */
				src->tmp_angleCCW65536		= HATSUDAN_03_angle65536;/* 自機狙い角 */
			}
			else
			{
				src->tmp_angleCCW65536		+= (((REG_10_BOSS_SPELL_TIMER)&0x20)<<5)-( ((65536/24)-1));/* */
			}
			remilia_shot00(src);
		}
	}
	/* 本当は休みだけど、あまりに簡単なので攻撃 */
	else
	{
/*
8421
1111 f	- - - -
1110 e	- - - -
1101 d	- - - -
1100 c	- - - L
1011 b	- - - -
1010 a	- - - -
1001 9	- - - -
1000 8	- - - L
0111 7	- - - -
0110 6	- - H -
0101 5	- - - -
0100 4	- - H L
0011 3	- - - -
0010 2	- N H -
0001 1	- - - -
0000 0	e N H L
*/
		/* 休み */
		const u8 aaa[4] =
		{
			0xff,	/* easy */
			0x0d,	/* NORMAL */
			0x09,	/* HARD */
			0x03,	/* LUNATIC */
		};
		if (0==((REG_10_BOSS_SPELL_TIMER) & aaa[(REG_0f_GAME_DIFFICULTY)] ))
		{
			remilia_oyasumi_kougeki(src);
		}
	}
}

/*---------------------------------------------------------





大弾＆中弾＆小弾乱射
冥符「紅色の冥界」
128way螺旋状高速ナイフ弾
赤針弾ばら撒き→９WAY赤大弾連射→全方位12way炎弾連射（ループ）
紅符「スカーレットシュート」
---------------------------------------------------------*/









/*---------------------------------------------------------
 	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。(8回に1回発弾する)
	REG_0a_REG2 	保持角度。

	-------------------------------------------------------
128way螺旋状高速ナイフ弾
	拡散型全方位ナイフ弾
	80分割弾 == (8way[hi to 0]+8way[slow to 0]) x 5way(5分割)
	-------------------------------------------------------

---------------------------------------------------------*/
//local void spell_create_07_80way_dual_five(OBJ *src)
local void spell_create_4d_remilia_rasen_knife(OBJ *src)/* 鈴仙でなくて螺旋です。 */
{
//	if (0==((REG_10_BOSS_SPELL_TIMER)&0x01))
	{
				HATSUDAN_02_speed_offset		= t256(1/*0*/);/*(テスト)*/
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
				HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI+1;				/* [akaナイフ弾] */
				HATSUDAN_03_angle65536			= REG_0a_REG2;			/* 角度 */
		{	int ii; 	/* 1周を65536分割した角度 */
			for (ii=0; ii<(4); ii += (1) ) /* [4分割] */	/* 角度(65536[90/360度]を 4 分割) */
			{
				HATSUDAN_03_angle65536 += (65536/4);			/* 角度 */
				mask65536(HATSUDAN_03_angle65536);
				//
				HATSUDAN_01_speed256			= (t256(2.00) ); 	/* 速弾 t256(3.00) */
				hatudan_system_regist_single();/* (r33-) */
				//
				HATSUDAN_01_speed256			= (t256(2.40) ); 	/* 高速弾 t256(3.14) */
				hatudan_system_regist_single();/* (r33-) */
			}
		}
				HATSUDAN_03_angle65536 -= (65536/128);					/* 次の角度 */
				REG_0a_REG2 		= HATSUDAN_03_angle65536;			/* 角度を保持 */
	}
		#if (0)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
}


/*---------------------------------------------------------
	呪詛「ブラド・ツェペシュの呪い」
	-------------------------------------------------------
	「レッドマジック」
	-------------------------------------------------------
	珠置き共通サブ
	-------------------------------------------------------
	C言語の場合は共通サブ。
	スクリプト化した場合は、コピペする。
---------------------------------------------------------*/
#define TAMA_DATA_10_TAMA_OKI_KANKAKU	src->hatudan_register_user_data10
local void remilia_common_tama_oki_sub(OBJ *src)
{
		src->hatudan_register_speed65536	+= (1<<8);			/* 弾速 */
	//
	//	if (0==(src->jyumyou&0x0f))/*16回に1回弾を置く*/
	//	if (0==(src->jyumyou&0x07))/*8回に1回弾を置く*/
		TAMA_DATA_10_TAMA_OKI_KANKAKU--;
		if (0 > TAMA_DATA_10_TAMA_OKI_KANKAKU)
		{
			TAMA_DATA_10_TAMA_OKI_KANKAKU = REG_09_REG1;//[定数1]n回に一回弾を置く。
			//
		//	REG_02_DEST_X	= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
		//	REG_03_DEST_Y	= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
			set_REG_DEST_XY(src);
			REG_00_SRC_X	= (REG_0b_REG3);
			REG_01_SRC_Y	= (REG_0c_REG4);
			tmp_angleCCW65536_src_nerai();//_bbb(zzz_player, dest);
		//
			/* 弾源x256 y256 弾の中心から発弾。 */
		//	set_REG_DEST_XY(src);
			HATSUDAN_01_speed256			= t256(0);	/* 弾速 */
			HATSUDAN_02_speed_offset		= t256(0);/*(テスト)*/
		//	HATSUDAN_03_angle65536			= (0);	/* 基準角度 */
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_02)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_MINI8_BASE + TAMA_IRO_01_AKA);		/* [赤弾] */
			hatudan_system_regist_single();/* (r33-) */
		}
}

/*---------------------------------------------------------
	呪詛「ブラド・ツェペシュの呪い」
	-------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local void brp_tamaoki_danmaku_01_callback(OBJ *src)/* 珠置き */
{
	/*(225==1+224==1+7*32)*/
//	if ((HATUDAN_ITI_NO_JIKAN-224) < src->jyumyou)/* 発弾エフェクト後から[224]カウント未満の弾 */
	if ((HATUDAN_ITI_NO_JIKAN-225) < src->jyumyou)/* 発弾エフェクト後から[225]カウント未満の弾 */
	{
		if (0==(src->hatudan_register_spec_data & TAMA_SPEC_KAITEN_HOUKOU_BIT))/*TAMA_SPEC_AKA_AO_KUNAI_BIT*/
		{	/*CW(時計回り)*/
		//	src->rotationCCW1024 -= (6);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
			src->rotationCCW1024 -= (3);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		}
		else
		{	/*CCW(反時計回り)*/
		//	src->rotationCCW1024 += (6);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
			src->rotationCCW1024 += (3);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		}
		remilia_common_tama_oki_sub(src);
	}
	else	/* 発弾後 225 カウント以上は弾を消す。 */
	{
		/* (通常弾へ変身する) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	}
//	danmaku_00_standard_angle_mover(src);/*(角度弾移動+画面外弾消し)*/
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
}
/*---------------------------------------------------------
	「レッドマジック」
	-------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local void red_magic_tamaoki_danmaku_01_callback(OBJ *src)/* 珠置き */
{
	/*(225==1+224==1+7*32)*/
//	if ((HATUDAN_ITI_NO_JIKAN-224) < src->jyumyou)/* 発弾エフェクト後から[224]カウント未満の弾 */
	if ((HATUDAN_ITI_NO_JIKAN-225) < src->jyumyou)/* 発弾エフェクト後から[225]カウント未満の弾 */
	{
		if (0!=(src->hatudan_register_spec_data & TAMA_SPEC_KAITEN_HOUKOU_BIT))/*TAMA_SPEC_AKA_AO_KUNAI_BIT*/
		{
			/*(左右の壁で反射する。)*/
			if (src->cx256 < t256(32))
			{
				/*(一度しか反射しない)*/
			//	src->hatudan_register_spec_data = (DANMAKU_LAYER_01)|(TAMA_SPEC_KAITEN_HOUKOU_BIT)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
			//	src->rotationCCW1024 -= (1024/2);/* 180/360 度 曲げてみるテスト */
				goto red_magic_hansya_syori;
			}
			else
			if (src->cx256 > t256(32)+t256(320) )
			{
			red_magic_hansya_syori:
				/*(一度しか反射しない)*/
				src->hatudan_register_spec_data = (DANMAKU_LAYER_01)|(0)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
			//	src->rotationCCW1024 -= (1024/2);/* 180/360 度 曲げてみるテスト */
				src->rotationCCW1024 = (1024)-(src->rotationCCW1024);	/* 反転[※1] */
			}
		}
		remilia_common_tama_oki_sub(src);
	}
	else	/* 発弾後 225 カウント以上は弾を消す。 */
	{
		/* (通常弾へ変身する) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	}
//	danmaku_00_standard_angle_mover(src);/*(角度弾移動+画面外弾消し)*/
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
}

/*---------------------------------------------------------
	[弾幕グループ(2)セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local void remilia_tahane_danmaku_02_callback(OBJ *src)/* 珠跳ね */
{
	if ((HATUDAN_ITI_NO_JIKAN-128) == src->jyumyou)/* 発弾エフェクト後から[128]カウント の弾 */
	{
		src->hatudan_register_speed65536 = (1<<16); 		/* 弾速 */
		/* (通常弾へ変身する) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	}
//	danmaku_00_standard_angle_mover(src);/*(角度弾移動+画面外弾消し)*/
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
}
/*---------------------------------------------------------
	[初期化セクション]	呪詛「ブラド・ツェペシュの呪い」
	-------------------------------------------------------
	[定数1]n回に一回弾を置く。
	0easy:			16
	1normal:		12
	2hard:			8
	3lunatic:		4
---------------------------------------------------------*/
local void spell_init_4e_remilia_vlad_tepes(OBJ *src)
{
//	TAMA_DATA_10_TAMA_OKI_KANKAKU	= (0);// カウンタ
	REG_09_REG1 	= ( (16)-((REG_0f_GAME_DIFFICULTY)<<2));//[定数1]n回に一回弾を置く。
//	REG_09_REG1 	= (t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<6));//[定数1]雨の速度
//	REG_0a_REG2 	= ((1024/2)+(1024/24)+(REG_0f_GAME_DIFFICULTY<<3));//[定数2]赤青クナイが曲がる角度
	card.danmaku_callback[1] = brp_tamaoki_danmaku_01_callback;/*(珠置き用)*/
	card.danmaku_callback[2] = remilia_tahane_danmaku_02_callback;/*(珠跳ね用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}
/*---------------------------------------------------------
	[初期化セクション]	「レッドマジック」
	-------------------------------------------------------
	[定数1]n回に一回弾を置く。
	0easy:			16
	1normal:		12
	2hard:			8
	3lunatic:		4
---------------------------------------------------------*/
local void spell_init_4f_remilia_red_magic(OBJ *src)
{
//	TAMA_DATA_10_TAMA_OKI_KANKAKU	= (0);// カウンタ
	REG_09_REG1 	= ( (16)-((REG_0f_GAME_DIFFICULTY)<<2));//[定数1]n回に一回弾を置く。
//	REG_09_REG1 	= (t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<6));//[定数1]雨の速度
//	REG_0a_REG2 	= ((1024/2)+(1024/24)+(REG_0f_GAME_DIFFICULTY<<3));//[定数2]赤青クナイが曲がる角度
	//
	REG_0d_REG5 	= const_init_nan_ido_table	[tama_const_H09_NUMS_REMI_MAGIC 	+(REG_0f_GAME_DIFFICULTY)];
	REG_0e_REG6 	= const_init_nan_ido_table	[tama_const_H10_DIVS_REMI_MAGIC 	+(REG_0f_GAME_DIFFICULTY)];
//
	card.danmaku_callback[1] = red_magic_tamaoki_danmaku_01_callback;/*(珠置き用)*/
	card.danmaku_callback[2] = remilia_tahane_danmaku_02_callback;/*(珠跳ね用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/


}

/*---------------------------------------------------------
	[発弾セクション]	呪詛「ブラド・ツェペシュの呪い」
	-------------------------------------------------------
	現在(r35)のスペカシステムの場合、仕様上
	スペカがまたがるレジスタ値を保持できない。
	(スペカが始まるタイミングで強制的にレジスタが0クリアされてしまう為)
	-------------------------------------------------------
	使用レジスタ:
	REG_0b_REG3 	発弾後、集まるX座標(t256形式)
	REG_0c_REG4 	発弾後、集まるY座標(t256形式)
---------------------------------------------------------*/
local void spell_create_4e_remilia_vlad_tepes(OBJ *src)
{
	if ((SPELL_TIME_0256-1)==((REG_10_BOSS_SPELL_TIMER) ))/*(一番始め)*/
	{
		HATSUDAN_01_speed256			= t256(1.0);	/* 弾速 */
		HATSUDAN_02_speed_offset		= t256(5);/*(テスト)*/
		HATSUDAN_03_angle65536			= 0;	/* 基準角度 */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|((ra_nd()) & TAMA_SPEC_KAITEN_HOUKOU_BIT)|(0)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI;		/* [青ナイフ弾] */
		HATSUDAN_06_n_way				= (13); 	/* [13way] */		/* 発弾数 */
		HATSUDAN_07_div_angle65536		= (int)(65536/(13));	/* 分割角度(65536[360/360度]を 13 分割) */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		u32 aaa = ra_nd();
		REG_0b_REG3 = ((aaa    )&0xff00) + t256(((480-128-32-256)/2));
		REG_0c_REG4 = ((aaa<<8 )&0xff00) + t256(((272-256)/2));
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
}


/*---------------------------------------------------------
	[発弾セクション]	「レッドマジック」
	-------------------------------------------------------
	現在(r35)のスペカシステムの場合、仕様上
	スペカがまたがるレジスタ値を保持できない。
	(スペカが始まるタイミングで強制的にレジスタが0クリアされてしまう為)
	-------------------------------------------------------
	使用レジスタ:
	REG_0b_REG3 	発弾後、集まるX座標(t256形式)
	REG_0c_REG4 	発弾後、集まるY座標(t256形式)
---------------------------------------------------------*/
local void spell_create_4f_remilia_red_magic(OBJ *src)
{
	if ((SPELL_TIME_0256-1)==((REG_10_BOSS_SPELL_TIMER) ))/*(一番始め)*/
	{
		HATSUDAN_01_speed256			= t256(1.0);	/* 弾速 */
		HATSUDAN_02_speed_offset		= t256(5);/*(テスト)*/
	//	HATSUDAN_03_angle65536			= (65536/4);	/* 基準角度(水平に撃つ。左右が基準０度) */
		HATSUDAN_03_angle65536			= ((ra_nd())&0x1fff);	/* 基準角度(?) */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_KAITEN_HOUKOU_BIT)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_OODAMA32_01_AKA); 	/* [紫大弾] */
		HATSUDAN_06_n_way				= REG_0d_REG5;//(10); 	/* [10way] */		/* 発弾数 */
		HATSUDAN_07_div_angle65536		= REG_0e_REG6;//(int)(65536/(10));	/* 分割角度(65536[360/360度]を 10 分割) */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		u32 aaa = ra_nd();
		REG_0b_REG3 = ((aaa    )&0xff00) + t256(((480-128-32-256)/2));
		REG_0c_REG4 = ((aaa<<8 )&0xff00) + t256(((272-256)/2));
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
}
