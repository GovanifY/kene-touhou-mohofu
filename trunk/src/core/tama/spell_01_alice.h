
/*---------------------------------------------------------
 東方模倣風 ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	アリスの弾幕を生成します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	アリス(仮)	赤の魔法(2段階目)のようなもの。
	-------------------------------------------------------
	移動を制限する弾幕。
	とりあえずアリスだから、赤の魔法の2段階目。
---------------------------------------------------------*/
static void spell_create_0e_aka_2nd(SPRITE *src)
{
	static int os2x;
	if (0==((src->boss_base_spell_time_out)&0x03))
	{
		obj_send1->cx256					= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
		obj_send1->cy256					= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
		if (0==((src->boss_base_spell_time_out)&0x0f))
		{
			br.BULLET_REGIST_00_speed256				= (t256(1.0));							/* 弾速 */
			br.BULLET_REGIST_01_speed_offset			= t256(1/*6*/);/*(テスト)*/
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_01_AKA;					/* [赤米弾] */
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)標準弾 */
			br.BULLET_REGIST_06_n_way					= (8);									/* [8way] */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/48);						/* 分割角度(1024/(6*8) ) １周を8分割した領域をさらに6分割した角度 */
			{
				/* tama_system_regist_katayori_n_wayの仕様が単純な為、補正する */
				#define BA_HOSEI48	((65536/(48/8))/2)	/* 一周 65536系で 48分割弾 x 8 way の真中(つまり/2)付近 */
				const int tmp_kakudo =
					(65536/(4*2)) + 	/* (65536/(4*2)) == まず(90/360)度の半分加える。(後で2倍になる) */	/* 弾が絶対にこない範囲 */
				//	(65536/(8*2)) + 	/* (65536/(8*2)) == まず(45/360)度の半分加える。(後で2倍になる) */	/* 弾が絶対にこない範囲 */
				//	(65536/128) +	/* まず(2.8125/360)度加える。(左右なので2倍==5.625度になる) */	/* 弾が絶対にこない範囲 */
					((256+sin1024((src->boss_base_spell_time_out<<2)&(1024-1)))<<4);				/* 次に揺らぎ分を加える。 */
				/* 右側 */
				br.BULLET_REGIST_02_angle65536		= (( tmp_kakudo-BA_HOSEI48)&(65536-1)); 		/* 角度(下CCWなので正方向==右側) */
				tama_system_regist_katayori_n_way();/* (r33-) */
			//
				/* 左側 */
				br.BULLET_REGIST_02_angle65536		= ((-tmp_kakudo-BA_HOSEI48)&(65536-1)); 		/* 角度(下CCWなので負方向==左側) */
				tama_system_regist_katayori_n_way();/* (r33-) */
			}
		}
		if (0x00==((src->boss_base_spell_time_out)&0x1f))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			#endif
		//
			SPRITE *zzz_player;
			zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
			tmp_angleCCW65536_src_nerai(zzz_player, obj_send1);/* 自機狙い角作成 */
			src->tmp_angleCCW65536		= obj_send1->tmp_angleCCW65536;/* 自機狙い角 */
			os2x = (src->cx256)+(ra_nd()&0x1fff)-t256((32)/2);							/* 弾源x256 */
		}
		//	br.BULLET_REGIST_00_speed256				= (t256(1.1));							/* 弾速 */
			br.BULLET_REGIST_00_speed256				= (t256(1.0)+((0x1f-((src->boss_base_spell_time_out)&0x1f))));	/* 弾速 */ /* 後で撃つほうが速く */
			br.BULLET_REGIST_01_speed_offset			= t256(3/*6*/);/*(テスト)*/
			br.BULLET_REGIST_02_angle65536				= (src->tmp_angleCCW65536);/* 自機狙い角 */
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_UROKO14_01_AKA;				/* [赤鱗弾] */
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)標準弾 */
		//
		{
			obj_send1->cx256							= os2x; 						/* 弾源x256 */
			/* 自機狙い */
			tama_system_regist_single();
		}
	}
}


/*---------------------------------------------------------
	アリス normal通常攻撃3(の一部)
	-------------------------------------------------------
	なんか全然まったく似てない。
	妖々夢風のつもりで創っていたが怪綺談風になった。
	何を言ってるのかわからないと思うが、わたしにもわからないや。
	恐ろしいものの片鱗を以下略。
	psp解像度が全然違うから、無理に似せようとしない方が良いかも？
---------------------------------------------------------*/

static void spell_create_17_alice_nejiri10sec(SPRITE *src)
{
	if ((64*1)<(src->boss_base_spell_time_out))
	{
		/* [CW 時計回り] */
		if (0==((src->boss_base_spell_time_out)&0x03))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			#endif
		//
			src->tmp_angleCCW65536 -= (int)(65536/24);
			#if 0/*1885624*/
			src->tmp_angleCCW65536 += (int)(ra_nd() & ((1<<difficulty)-1) );
			#else
			{static const u8 masked_tbl[4] = { 0x00, 0x0f, 0x3f, 0xff };
				src->tmp_angleCCW65536 += (int)(ra_nd() & (masked_tbl[(cg_game_difficulty)]) );
			}
			#endif
			mask65536(src->tmp_angleCCW65536);
		//
				obj_send1->cx256						= (src->cx256); 						/* 弾源x256 */
				obj_send1->cy256						= (src->cy256); 						/* 弾源y256 */
				//
				br.BULLET_REGIST_00_speed256				= (t256(1.0));							/* 弾速 */
				br.BULLET_REGIST_01_speed_offset			= t256(1);/*(テスト)*/
			//	br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
				br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_03_AOI; 				/* [枠付き青丸弾] */
			//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)標準弾 */
				br.BULLET_REGIST_06_n_way					= (6);									/* [6way] */
				br.BULLET_REGIST_07_div_angle65536			= (int)(65536/48);						/* 分割角度(1024/(6*8) ) １周を8分割した領域をさらに6分割した角度 */
		//
			{
				br.BULLET_REGIST_02_angle65536				= (src->tmp_angleCCW65536); 							/* 角度 */
				tama_system_regist_katayori_n_way();/* (r33-) */
			//
				br.BULLET_REGIST_02_angle65536				= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));					/* 角度 */
				tama_system_regist_katayori_n_way();/* (r33-) */
			}
		}
	}
	else
	if ((64*2)<(src->boss_base_spell_time_out))
	{
		/*[CCW 反時計回り]*/
		if (0==((src->boss_base_spell_time_out)&0x03))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			#endif
		//
			src->tmp_angleCCW65536 += (int)(65536/24);
			#if 0/*1885624*/
			src->tmp_angleCCW65536 -= (int)(ra_nd() & ((1<<difficulty)-1) );
			#else
			{static const u8 masked_tbl[4] = { 0x00, 0x0f, 0x3f, 0xff };
				src->tmp_angleCCW65536 -= (int)(ra_nd() & (masked_tbl[(cg_game_difficulty)]) );
			}
			#endif
			mask65536(src->tmp_angleCCW65536);
		//
				obj_send1->cx256						= (src->cx256); 						/* 弾源x256 */
				obj_send1->cy256						= (src->cy256); 						/* 弾源y256 */
				//
				br.BULLET_REGIST_00_speed256				= (t256(1.0));							/* 弾速 */
				br.BULLET_REGIST_01_speed_offset			= t256(1);/*(テスト)*/
			//	br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
				br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_01_AKA; 				/* [枠付き赤丸弾] */
			//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)標準弾 */
				br.BULLET_REGIST_06_n_way					= (6);									/* [6way] */
				br.BULLET_REGIST_07_div_angle65536			= (int)(65536/48);						/* 分割角度(1024/(6*8) ) １周を8分割した領域をさらに6分割した角度 */
		//
			{
				br.BULLET_REGIST_02_angle65536				= (src->tmp_angleCCW65536); 							/* 角度 */
				tama_system_regist_katayori_n_way();/* (r33-) */
			//
				br.BULLET_REGIST_02_angle65536				= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));					/* 角度 */
				tama_system_regist_katayori_n_way();/* (r33-) */
			}
		}
	}
}


/*---------------------------------------------------------
	アリス	アリス人形弾幕
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
extern void add_zako_alice_doll(SPRITE *src);/* アリス人形弾幕 */
static void spell_create_0b_alice_doll(SPRITE *src)
{
	if (50==((src->boss_base_spell_time_out) ))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_alice_doll(src);
	}
}

/*---------------------------------------------------------
	計7つに分裂する。
	-------------------------------------------------------


---------------------------------------------------------*/
#if 0
static void exchange_damnaku_alice_7_bunretu(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_04_bullet_obj_type;/* 調べるタイプを受け取る */
	int jj = 0;
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
	//	if (check_type == (s->type) )	/* 色に関係なく鱗弾[ ... ]なら */
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
extern void add_zako_alice_doll_type_b(SPRITE *src);/* アリス人形弾幕 */
static void spell_create_1e_alice_doll(SPRITE *src)
{
	if (250==((src->boss_base_spell_time_out) ))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_alice_doll_type_b(src);
	}
	if (0x00==((src->boss_base_spell_time_out)&0x3f))
	{
		/* 弾分裂 */
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
//		exchange_damnaku_arice_7_bunretu();
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
	アリス	諏訪子っぽい(?)弾幕
	-------------------------------------------------------
	テキトー
---------------------------------------------------------*/

static void spell_create_25_alice_suwako(SPRITE *src)
{
//	if ((0) ==((src->boss_base_spell_time_out)&0x01))/* 2回に1回 */
	{
		static int jjj;
	//	if ((0) ==((src->boss_base_spell_time_out)&0x07))/*  8に1回 */
		if ((0) ==((src->boss_base_spell_time_out)&0x0f))/* 16に1回 */
		{
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			jjj -= (int)(65536*8/256);
		}
	//	jjj -= (65536/256); 	/* 1周を256分割した角度 */
	//	jjj -= (65536/512); 	/* 1周を256分割した角度の半分。 */
		jjj -= (200);	/*	テキトー 1周を256分割した角度の半分。 */
		//
			obj_send1->cx256					= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
			obj_send1->cy256					= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
		/* 弾生成 */
		{
			br.BULLET_REGIST_00_speed256				= (t256(1.0) ); 					/* 弾速(pspの画面は狭い) */
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)標準弾 */
			br.BULLET_REGIST_06_n_way					= (8);								/* [8way] */			/* 8方向弾 */
			br.BULLET_REGIST_07_div_angle65536			= (65536/8);						/* 分割角度(弾がこない範囲を除き32分割) */	/* (ra_nd16>>4) == 乱数(0-4095) */
			//
			/* 0 [...赤] 150 [...青+赤] 180 [...青] 255 */
			if (0==((src->boss_base_spell_time_out)&1))
			{
				if ((128+((cg_game_difficulty)<<2))>(src->boss_base_spell_time_out))
				{
					br.BULLET_REGIST_01_speed_offset			= t256(2);					/* 調整減速弾 */	/* この方式になるか検討中 */
					br.BULLET_REGIST_02_angle65536				= (jjj);					/* 向き */
					br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_01_AKA;		/* 赤米弾 */
					tama_system_regist_katayori_n_way();/* (r33-) */
				}
			}
			else
			{
				if ((128-((cg_game_difficulty)<<2))<(src->boss_base_spell_time_out))
				{
					br.BULLET_REGIST_01_speed_offset			= t256(1);					/* 調整減速弾 */	/* この方式になるか検討中 */
					br.BULLET_REGIST_02_angle65536				= (65536-jjj); 				/* 向き */
					br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_03_AOI;		/* 青米弾 */
					tama_system_regist_katayori_n_way();/* (r33-) */
				}
			}
		}
	}

}
