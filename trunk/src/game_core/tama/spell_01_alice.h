
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	アリスのカードを定義します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	アリス(仮)	赤の魔法(2段階目)のようなもの。
	-------------------------------------------------------
	移動を制限するカード。
	とりあえずアリスだから、赤の魔法の2段階目。
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。(4回に1回発弾する)
	REG_0a_REG2 	自機狙い角を一時保持する。(現在の実装では HATSUDAN_03_angle65536 が破壊される仕様なので要るが、後で要らなくなるかも？)
	REG_0b_REG3 	[x座標一時保存]
	REG_0c_REG4 	[y座標一時保存]
	REG_0d_REG5 	難易度別定数1
	REG_0e_REG6 	難易度別定数2
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_0e_aka_2nd)
{
	REG_0d_REG5 	= const_init_nan_ido_table	[tama_const_H06_NUMS_ALICE_RED		+(REG_0f_GAME_DIFFICULTY)];
	REG_0e_REG6 	= const_init_nan_ido_table	[tama_const_H07_DIVS_ALICE_RED		+(REG_0f_GAME_DIFFICULTY)];/*(((REG_10_BOSS_SPELL_TIMER)&0x20)>>3)+*/
}
local OBJ_CALL_FUNC(spell_create_0e_aka_2nd)
{
	count_up_limit_NUM(REG_NUM_08_REG0, 4);//	if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
	if (0==(REG_08_REG0))
	{
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			REG_0b_REG3 = REG_02_DEST_X;//[ボスx座標一時保存]
			REG_0c_REG4 = REG_03_DEST_Y;//[ボスy座標一時保存]
			//------------------
			HATSUDAN_01_speed256	= (t256(1.0));
			HATSUDAN_03_angle65536	= deg1024to65536((((REG_10_BOSS_SPELL_TIMER<<2))));
			sincos256();/*(破壊レジスタ多いので注意)*/
		//	src->center.x256 = REG_03_DEST_Y;//sin_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
		//	src->center.y256 = REG_02_DEST_X;//cos_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
			//------------------
			//
		//[重複]	HATSUDAN_01_speed256				= (t256(1.0));							/* 弾速 */
			HATSUDAN_02_speed_offset			= t256(1);/*6(テスト)*/
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_KOME_BASE + TAMA_IRO_01_AKA); 				/* [赤米弾] */
			HATSUDAN_06_n_way					= (8);						/* [8way] */
			HATSUDAN_07_div_angle65536			= (int)(65536/48);			/* 分割角度(1024/(6*8) ) １周を8分割した領域をさらに6分割した角度 */
			{
				/* hatudan_system_regist_katayori_n_wayの仕様が単純な為、補正する */
				#define BA_HOSEI48	((65536/(48/8))/2)	/* 一周 65536系で 48分割弾 x 8 way の真中(つまり/2)付近 */
				const int tmp_kakudo =
					(65536/(4*2)) + 	/* (65536/(4*2)) == まず(90/360)度の半分加える。(後で2倍になる) */	/* 弾が絶対にこない範囲 */
				//	(65536/(8*2)) + 	/* (65536/(8*2)) == まず(45/360)度の半分加える。(後で2倍になる) */	/* 弾が絶対にこない範囲 */
				//	(65536/128) +		/* まず(2.8125/360)度加える。(左右なので2倍==5.625度になる) */		/* 弾が絶対にこない範囲 */
					((256 + (REG_03_DEST_Y/*sin_value_t256*/))<<4);					/* 次に揺らぎ分を加える。 */
				// 発射座標リストア。
				REG_02_DEST_X = REG_0b_REG3;//[ボスx座標復活]
				REG_03_DEST_Y = REG_0c_REG4;//[ボスy座標復活]
				/* 右側 */
				HATSUDAN_03_angle65536		= (( tmp_kakudo-BA_HOSEI48)&(65536-1)); 	/* 角度(下CCWなので正方向==右側) */
				hatudan_system_regist_katayori_n_way();/* (r33-) */
				/* 左側 */
				HATSUDAN_03_angle65536		= ((-tmp_kakudo-BA_HOSEI48)&(65536-1)); 	/* 角度(下CCWなので負方向==左側) */
				hatudan_system_regist_katayori_n_way();/* (r33-) */
			}
		}
		{
			if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x1f))
			{
			//(r35でカードの場合のみ省略可能)	set_REG_DEST_XY(src);
				calculate_jikinerai();/* 自機狙い角作成 */
				REG_0a_REG2 						= HATSUDAN_03_angle65536;/* 自機狙い角 */
				#if (1)
				cpu_bullet_play_15_auto();
				#endif
			}
			//	HATSUDAN_01_speed256			= (t256(1.1));							/* 弾速 */
				HATSUDAN_01_speed256			= (t256(1.0)+((0x1f-((REG_10_BOSS_SPELL_TIMER)&0x1f))));	/* 弾速 */ /* 後で撃つほうが速く */
				HATSUDAN_02_speed_offset		= t256(3/*6*/);/*(テスト)*/
				HATSUDAN_03_angle65536			= (REG_0a_REG2);/* 自機狙い角 */
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_2000_EFFECT_MINI)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_UROKO14_BASE + TAMA_IRO_01_AKA);				/* [赤鱗弾] */
			//
				HATSUDAN_06_n_way				= REG_0d_REG5;										/* [8way] */
				HATSUDAN_07_div_angle65536		= REG_0e_REG6;	/* 分割角度(1024/(6*8) ) １周を8分割した領域をさらに6分割した角度 */
			{
			//	os2x	発弾するx位置(乱数的に横にずらす)
				REG_02_DEST_X						+= (ra_nd()&0x1fff)-t256((32)/2);	/* 弾源x256 */
				/* 自機狙い */
			//	hatudan_system_regist_single();
				hatudan_system_regist_n_way();/* (r33-) */
			}
		}
	}
}


#if 1/*(r39)*/
/*---------------------------------------------------------
	アリス normal通常攻撃3(の一部)
	-------------------------------------------------------
	-------------------------------------------------------
	使用レジスタ
---------------------------------------------------------*/

/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
	交差弾。反時計回り。
	[弾幕グループ(2)セクション]
	-------------------------------------------------------
	交差弾。時計回り。
---------------------------------------------------------*/
local OBJ_CALL_FUNC(alice_aaa_danmaku_01_callback)
{
	/* 0-256 カウントまで */ 	/* [0]カウント==発弾位置 */
	if ((HATUDAN_ITI_NO_JIKAN-80) < src->jyumyou)/* 発弾エフェクト後から[0-79]カウント経過した弾 */
	{
		if (0==(src->hatudan_register_spec_data & TAMA_SPEC_KAITEN_HOUKOU_BIT))
		{
			src->rotationCCW1024++;/* 曲げてみるテスト */
		}
		else
		{
			src->rotationCCW1024--;/* 曲げてみるテスト */
		}
	}
	// (この方が解かりやすい？)
	else
//	if ((HATUDAN_ITI_NO_JIKAN-256) == src->jyumyou/*(寿命なので自動で減る)*/)/* 発弾エフェクト後から[256]カウント経過した弾 */
	{
		/* (通常弾へ変身する==画面内では弾は消えない) */
		src->hatudan_register_spec_data 	= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		#if (1)/*(デバッグ)*/
		src->obj_type_set						= (BULLET_MINI8_BASE + TAMA_IRO_01_AKA);		/* [ミニ赤丸弾] */
		reflect_sprite_spec(src, OBJ_BANK_SIZE_00_TAMA); 	/* 弾グラと弾あたり判定を変更する。 */
		#endif
	}
//	danmaku_00_standard_angle_mover(src);/*(角度弾移動+画面外弾消し)*/
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
}
/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_init_17_alice_nejiri10sec)
{
	card.danmaku_callback[1] = alice_aaa_danmaku_01_callback;/*(米弾。交差弾。反時計回り。) (米弾。交差弾。時計回り。)*/
//	card.danmaku_callback[2] = rumia_danmaku_02_callback;/*(丸弾。連弾。自機狙い。)*/
//	card.danmaku_callback[3] = NULL;
}

local OBJ_CALL_FUNC(spell_create_17_alice_nejiri10sec)
{
}
#endif

#if 0/*(r38)*/
/*---------------------------------------------------------
	アリス normal通常攻撃3(の一部)
	-------------------------------------------------------
	なんか全然まったく似てない。
	妖々夢風のつもりで創っていたが怪綺談風になった。
	何を言ってるのかわからないと思うが、わたしにもわからないや。
	恐ろしいものの片鱗を以下略。
	psp解像度が全然違うから、無理に似せようとしない方が良いかも？
	-------------------------------------------------------
	使用レジスタ
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_17_alice_nejiri10sec)
{
	if ((64*1)<(REG_10_BOSS_SPELL_TIMER))
	{
		/* [CW 時計回り] */
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
		{
			src->tmp_angleCCW65536 -= (int)(65536/24);
			#if 0
			src->tmp_angleCCW65536 += (int)(ra_nd() & ((1<<difficulty)-1) );
			#else
			{static const u8 masked_tbl[4] = { 0x00, 0x0f, 0x3f, 0xff };
				src->tmp_angleCCW65536 += (int)(ra_nd() & (masked_tbl[(REG_0f_GAME_DIFFICULTY)]) );
			}
			#endif
			mask65536(src->tmp_angleCCW65536);
			//
				HATSUDAN_01_speed256				= (t256(1.0));							/* 弾速 */
				HATSUDAN_02_speed_offset			= t256(1);/*(テスト)*/
				HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
				HATSUDAN_05_bullet_obj_type 		= (BULLET_WAKU12_BASE + TAMA_IRO_03_AOI);				/* [枠付き青丸弾] */
				HATSUDAN_06_n_way					= (6);									/* [6way] */
				HATSUDAN_07_div_angle65536			= (int)(65536/48);						/* 分割角度(1024/(6*8) ) １周を8分割した領域をさらに6分割した角度 */
		//
			{
				HATSUDAN_03_angle65536				= (src->tmp_angleCCW65536); 							/* 角度 */
				hatudan_system_regist_katayori_n_way();/* (r33-) */
			//
				HATSUDAN_03_angle65536				= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));					/* 角度 */
				hatudan_system_regist_katayori_n_way();/* (r33-) */
			}
			#if (1)
			cpu_bullet_play_15_auto();
			#endif
		}
	}
	else
	if ((64*2)<(REG_10_BOSS_SPELL_TIMER))
	{
		/*[CCW 反時計回り]*/
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
		{
			src->tmp_angleCCW65536 += (int)(65536/24);
			#if 0
			src->tmp_angleCCW65536 -= (int)(ra_nd() & ((1<<difficulty)-1) );
			#else
			{	static const u8 masked_tbl[4] = { 0x00, 0x0f, 0x3f, 0xff };
				src->tmp_angleCCW65536 -= (int)(ra_nd() & (masked_tbl[(REG_0f_GAME_DIFFICULTY)]) );
			}
			#endif
			mask65536(src->tmp_angleCCW65536);
			//
				HATSUDAN_01_speed256				= (t256(1.0));							/* 弾速 */
				HATSUDAN_02_speed_offset			= t256(1);/*(テスト)*/
				HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
				HATSUDAN_05_bullet_obj_type 		= (BULLET_WAKU12_BASE + TAMA_IRO_01_AKA);				/* [枠付き赤丸弾] */
				HATSUDAN_06_n_way					= (6);									/* [6way] */
				HATSUDAN_07_div_angle65536			= (int)(65536/48);						/* 分割角度(1024/(6*8) ) １周を8分割した領域をさらに6分割した角度 */
			{
				HATSUDAN_03_angle65536				= (src->tmp_angleCCW65536); 							/* 角度 */
				hatudan_system_regist_katayori_n_way();/* (r33-) */
			//
				HATSUDAN_03_angle65536				= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));					/* 角度 */
				hatudan_system_regist_katayori_n_way();/* (r33-) */
			}
			#if (1)
			cpu_bullet_play_15_auto();
			#endif
		}
	}
}
#endif

/*---------------------------------------------------------
	アリス	アリス人形カード
	-------------------------------------------------------
蒼符「博愛の仏蘭西人形 -easy-」
アリスの放った四体の人形が、二回分裂する7way弾を撃つ。
4 x 7 x 7 == 最大 196[弾](妖々夢)
3 x 7 x 7 == 最大 147[弾](模倣風)	//3 x 8 x 8 == 192
	-------------------------------------------------------
蒼符「博愛の仏蘭西人形」
アリスの放った六体の人形が、二回分裂する7way弾を撃つ。
6 x 7 x 7 == 最大 294[弾](妖々夢)
5 x 7 x 7 == 最大 245[弾](模倣風)	//5 x 8 x 8 == 320
	-------------------------------------------------------
蒼符「博愛の仏蘭西人形 -hard-」
アリスの放った八体の人形が、二回分裂する7way弾を撃つ。
8 x 7 x 7 == 最大 392[弾](妖々夢)
9 x 7 x 7 == 最大 441[弾](模倣風)	//7 x 8 x 8 == 448
	-------------------------------------------------------
蒼符「博愛のオルレアン人形」赤弾→緑弾
アリスの放った八体の人形が、三回分裂する7way弾を撃つ。
8 x 7 x 7 x 7 == 最大2744[弾](妖々夢)
2 x 7 x 7 x 7 == 最大 686[弾](模倣風)	//9 x 8 x 8 == 576
---------------------------------------------------------*/
extern OBJ_CALL_FUNC(add_zako_alice_doll_type_a);/* アリス人形カード */
local OBJ_CALL_FUNC(spell_create_0b_alice_zako_doll)
{
	if (50==((REG_10_BOSS_SPELL_TIMER) ))
	{
		add_zako_alice_doll_type_a(src);
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
}

/*---------------------------------------------------------
	計7つに分裂する。
	-------------------------------------------------------


---------------------------------------------------------*/
#if 0
local void exchange_damnaku_alice_7_bunretu(void)
{
	int jj = 0;
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_1024_MAX; ii++)/* 全部調べる。 */
	{
		OBJ *s;
		s = &obj99[OBJ_HEAD_00_0x0000_TAMA+ii];
		/* 色に関係なく鱗弾[ ... ]なら */
		if (((BULLET_UROKO14_03_AOI+1) & 0xfff8) == (s->type & 0xfff8) )	/* [赤鱗弾]なら */
		{
			jj++;
		//	jj &= 0x07; 	// 2 3 4 5	6 7 8 9
			jj &= 0x03; 	// 3 4 5 6
		//	s_change_meirin_yajirusi_one(s);
		//	s->tra65536 					= t256(6);		/* 調整加速弾 */
		//	s->tra65536 					= (jj<<8)+t256(2);	/* (2 ... 9) 調整加速弾 */
			s->tra65536 					= (jj<<8)+t256(3);	/* (3 ... 6) 調整加速弾 */
			/* (1は遅すぎたので除外) 9 だと速過ぎるかも */
		}
	}
}
#endif
extern OBJ_CALL_FUNC(add_zako_alice_doll_type_b);/* アリス人形カード */
local OBJ_CALL_FUNC(spell_create_1e_alice_tama_doll)
{
	if (250==((REG_10_BOSS_SPELL_TIMER) ))
	{
		add_zako_alice_doll_type_b(src);
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
	if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x3f))
	{
		/* 弾分裂 */
//		exchange_damnaku_arice_7_bunretu();
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
}
/*
// 蒼符「博愛の仏蘭西人形」
	各人形が1弾生成(青弾)
	外側に射出。
//
	180度反転する。
	７弾に分裂(青弾→白弾)
  A   A    A   A   --- 速度速い

		A------------- 速度中
		|
   A	|	 A	------ 速度遅い
		+
//
	180度反転する。
	７弾に分裂(白弾→赤弾)
  A   A    A   A   --- 速度速い

		A------------- 速度中
		|
   A	|	 A	------ 速度遅い
		+
// 蒼符「博愛のオルレアン人形」のみ
	180度反転する。
	７弾に分裂(赤弾→緑弾)
  A   A    A   A   --- 速度速い

		A------------- 速度中
		|
   A	|	 A	------ 速度遅い
		+
//

*/

/*---------------------------------------------------------
	アリス	諏訪子っぽい(?)カード
	-------------------------------------------------------
	テキトー
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	向き。

---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_25_alice_suwako)
{
//	if ((0) ==((REG_10_BOSS_SPELL_TIMER)&0x01))/* 2回に1回 */
	{
	//	if ((0) ==((REG_10_BOSS_SPELL_TIMER)&0x07))/*  8に1回 */
		if ((0) ==((REG_10_BOSS_SPELL_TIMER)&0x0f))/* 16に1回 */
		{
			REG_08_REG0 -= (int)(65536*8/256);
			cpu_bullet_play_15_auto();
		}
	//	REG_08_REG0 -= (65536/256); 	/* 1周を256分割した角度 */
	//	REG_08_REG0 -= (65536/512); 	/* 1周を256分割した角度の半分。 */
		REG_08_REG0 -= (200);	/*	テキトー 1周を256分割した角度の半分。 */
		/* 弾生成 */
		{
			HATSUDAN_01_speed256				= (t256(1.0) ); 					/* 弾速(pspの画面は狭い) */
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_06_n_way					= (8);								/* [8way] */			/* 8方向弾 */
			HATSUDAN_07_div_angle65536			= (65536/8);						/* 分割角度(弾がこない範囲を除き32分割) */	/* (ra_nd16>>4) == 乱数(0-4095) */
			//
			/* 0 [...赤] 150 [...青+赤] 180 [...青] 255 */
			if (0==((REG_10_BOSS_SPELL_TIMER)&1))
			{
				if ((s32)(128+((REG_0f_GAME_DIFFICULTY)<<2)) > (REG_10_BOSS_SPELL_TIMER))
				{
					HATSUDAN_02_speed_offset		= t256(2);					/* 調整減速弾 */	/* この方式になるか検討中 */
					HATSUDAN_03_angle65536			= (REG_08_REG0);			/* 向き */
					HATSUDAN_05_bullet_obj_type 	= (BULLET_KOME_BASE + TAMA_IRO_01_AKA); 	/* 赤米弾 */
					hatudan_system_regist_katayori_n_way();/* (r33-) */
				}
			}
			else
			{
				if ((s32)(128-((REG_0f_GAME_DIFFICULTY)<<2)) < (REG_10_BOSS_SPELL_TIMER))
				{
					HATSUDAN_02_speed_offset		= t256(1);					/* 調整減速弾 */	/* この方式になるか検討中 */
					HATSUDAN_03_angle65536			= (65536-REG_08_REG0);		/* 向き */
					HATSUDAN_05_bullet_obj_type 	= (BULLET_KOME_BASE + TAMA_IRO_03_AOI); 	/* 青米弾 */
					hatudan_system_regist_katayori_n_way();/* (r33-) */
				}
			}
		}
	}

}
