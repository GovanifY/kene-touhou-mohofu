
/*---------------------------------------------------------
 東方模倣風 ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	その他の弾幕を生成します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	差分氏の妖怪1(天狗様?)が撃つ弾幕
	-------------------------------------------------------
	28休み → 100奇数弾 緑 7方向 BULLET_UROKO14_05_MIDORI		speed256	= t256(3.25)+((difficulty)<<6);
	28休み → 100偶数弾 青 8方向 BULLET_UROKO14_04_MIZUIRO		speed256	= (t256(3.0)+((difficulty)<<6));
---------------------------------------------------------*/
static void spell_create_11_tengu_shot(SPRITE *src)
{
	int aaa;
	if (128 > ((src->boss_spell_timer)))
	{/* 偶数弾 青 8方向 */
		aaa = 1;
	}
	else
	{/* 奇数弾 緑 7方向 */
		aaa = 0;
	}
	/* 初めの 28 flameはお休み */
	if (100 > ((src->boss_spell_timer)&0x7f))
	{
	//	static int YOKAI1_DATA_wait2	= (10-1);/*0*/
		static int YOKAI1_DATA_wait2	= 0;		/* 攻撃wait */
		YOKAI1_DATA_wait2--;
		if (0 > YOKAI1_DATA_wait2)
		{
			/* 0: 20 == 20-(0*4) */
			/* 1: 16 == 20-(1*4) */
			/* 2: 12 == 20-(2*4) */
			/* 3:  8 == 20-(3*4) */
			YOKAI1_DATA_wait2 = ((20-1)-(((cg_game_difficulty))<<2));/*(8-1)*/ /*(10-1)*/
			voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/* テキトー */
			{
				br.BULLET_REGIST_00_speed256			= (t256(3.0)+(((cg_game_difficulty))<<6))+(aaa<<(8-2));
				br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_UROKO14_04_MIZUIRO+(aaa);
				br.BULLET_REGIST_06_n_way				= (7)+(aaa);
			}
			br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN;
		//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
			br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/24);
			bullet_regist_multi_vector_send1_xy_src(src);	/* 弾源x256 y256 中心から発弾。 */
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

/* [CW 時計回り] */
static void spell_create_20_sonota_debug_cw_ao/*CW*/(SPRITE *src)
{
	if (0==((src->boss_spell_timer)&0x03))
	{
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
		src->tmp_angleCCW65536 -= (int)(65536/24);
		#if 0
		src->tmp_angleCCW65536 += (int)(ra_nd() & ((1<<difficulty)-1) );
		#else
		{static const u8 masked_tbl[4] = { 0x00, 0x0f, 0x3f, 0xff };
			src->tmp_angleCCW65536 += (int)(ra_nd() & (masked_tbl[(cg_game_difficulty)]) );
		}
		#endif
		mask65536(src->tmp_angleCCW65536);
		//
			br.BULLET_REGIST_00_speed256				= (t256(1.0));	/* 弾速 */
			br.BULLET_REGIST_01_speed_offset			= t256(1);/*(テスト)*/
		//	br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
			br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_03_AOI; 				/* [枠付き青丸弾] */
			br.BULLET_REGIST_06_n_way					= (6);									/* [6way] */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/48);						/* 分割角度(1024/(6*8) ) １周を8分割した領域をさらに6分割した角度 */
		{
			br.BULLET_REGIST_02_angle65536				= (src->tmp_angleCCW65536); 							/* 角度 */
			tama_system_regist_katayori_n_way();/* (r33-) */
		//
			br.BULLET_REGIST_02_angle65536				= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));		/* 角度 */
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}

/*[CCW 反時計回り]*/
static void spell_create_21_sonota_debug_ccw_aka/*CCW*/(SPRITE *src)
{
	if (0==((src->boss_spell_timer)&0x03))
	{
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
		src->tmp_angleCCW65536 += (int)(65536/24);
		#if 0
		src->tmp_angleCCW65536 -= (int)(ra_nd() & ((1<<difficulty)-1) );
		#else
		{static const u8 masked_tbl[4] = { 0x00, 0x0f, 0x3f, 0xff };
			src->tmp_angleCCW65536 -= (int)(ra_nd() & (masked_tbl[(cg_game_difficulty)]) );
		}
		#endif
		mask65536(src->tmp_angleCCW65536);
		//
			br.BULLET_REGIST_00_speed256				= (t256(1.0));	/* 弾速 */
			br.BULLET_REGIST_01_speed_offset			= t256(1);/*(テスト)*/
		//	br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
			br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_01_AKA; 				/* [枠付き赤丸弾] */
			br.BULLET_REGIST_06_n_way					= (6);									/* [6way] */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/48);						/* 分割角度(1024/(6*8) ) １周を8分割した領域をさらに6分割した角度 */
		{
			br.BULLET_REGIST_02_angle65536				= (src->tmp_angleCCW65536); 							/* 角度 */
			tama_system_regist_katayori_n_way();/* (r33-) */
		//
			br.BULLET_REGIST_02_angle65536				= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));		/* 角度 */
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}
