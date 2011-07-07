
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
	if (128 > ((src->boss_base_spell_time_out)))
	{/* 偶数弾 青 8方向 */
		aaa = 1;
	}
	else
	{/* 奇数弾 緑 7方向 */
		aaa = 0;
	}
	/* 初めの 28 flameはお休み */
	if (100 > ((src->boss_base_spell_time_out)&0x7f))
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
			voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*テキトー*/

			obj_send1->cx256							= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
			obj_send1->cy256							= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
			{
				br.BULLET_REGIST_00_speed256			= (t256(3.0)+(((cg_game_difficulty))<<6))+(aaa<<(8-2));
				br.BULLET_REGIST_04_bullet_obj_type		= BULLET_UROKO14_04_MIZUIRO+(aaa);
				br.BULLET_REGIST_06_n_way				= (7)+(aaa);
			}
			br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN;
			br.BULLET_REGIST_05_regist_type				= REGIST_TYPE_00_MULTI_VECTOR;
			br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/24);
			bullet_regist_vector();
		}
	}

}

#if 0
{
	static int aaa_angle1024;
	static int data_wait2_start;/* 発弾間隔start */
	static int data_wait2;/* 発弾間隔 */

//	if (128==((src->boss_base_spell_time_out)))
	if (0x10==((src->boss_base_spell_time_out)&0x10))
	{
		data_wait2_start	= (20-(((cg_game_difficulty))<<2));	/*8*/ /*10*/
		data_wait2			= 10;
		tmp_angleCCW1024_jiki_nerai(src);/* 自機狙い角作成 */
		aaa_angle1024		= src->tmp_angleCCW1024;/* 自機狙い角 */
		aaa_angle1024		-= (int)(1024*(5)/(32));/* 5弾目が自機狙い */
		/* 角度(1024[360/360度]を 32分割) */
	}
	else
//	if (0x00==((src->boss_base_spell_time_out)&0x10))/* 16回 */
	{
		data_wait2--;
		if (1 > data_wait2)
		{
			data_wait2 = data_wait2_start;
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
			#endif
			obj_send1->cy256 						= (src->cy256);								/* 弾源y256 */	/* 発弾位置の offset 用 */
			{
			int iii;
				iii = (src->boss_base_spell_time_out);
			int tama_color;/* 弾色 */
			int bbb;/* 方向 */
				if (0x00==((src->boss_base_spell_time_out)&0x20))
						{	bbb=-1;tama_color=0;/* 枠つき青弾 */}
				else	{	bbb= 1;tama_color=1;/* 枠つき緑弾 */}
			//	obj_send1->cx256 = src->cx256+((16-(iii&0x0f))<<(3+8))*bbb;/* 本物は発弾位置の x offset あり */
				obj_send1->cx256 = src->cx256+((16-(iii&0x0f))<<(3+7))*bbb;/* 本物は発弾位置の x offset あり */
				br.BULLET_REGIST_00_speed256				= (t256(1.5));								/* 弾速 */
				br.BULLET_REGIST_02_VECTOR_angle1024		= ((/*0+*/(aaa_angle1024)*bbb)&(1024-1));	/* 弾源角度1024 */
		//		br.BULLET_REGIST_07_VECTOR_div_angle1024	= (0);										/* ダミー分割角度(未使用) */
				br.BULLET_REGIST_04_bullet_obj_type			= BULLET_WAKU12_03_AOI+tama_color;			/* [枠つき青弾] [枠つき緑弾] */ 	/*BULLET_MARU12_03_AOI*/
				br.BULLET_REGIST_06_n_way					= (1);										/* [1way] */
				br.BULLET_REGIST_05_regist_type				= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
			/* 次の弾の角度 */
			aaa_angle1024 += (int)(1024/(32));/* 角度(1024[360/360度]を 32分割) */
		}
	}
}
#endif

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
	if (0==((src->boss_base_spell_time_out)&0x03))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
			obj_send1->cx256						= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
			obj_send1->cy256						= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
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
			//
			br.BULLET_REGIST_00_speed256				= (t256(1.0));	/* 弾速 */
			br.BULLET_REGIST_01_speed_offset			= t256(1);/*(テスト)*/
		//	br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
			br.BULLET_REGIST_04_bullet_obj_type			= BULLET_WAKU12_03_AOI; 				/* [枠付き青丸弾] */
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)標準弾 */
			br.BULLET_REGIST_06_n_way					= (6);									/* [6way] */
			br.BULLET_REGIST_07_div_angle65536 			= (int)(65536/48);						/* 分割角度(1024/(6*8) ) １周を8分割した領域をさらに6分割した角度 */
		{
			br.BULLET_REGIST_02_angle65536 				= (src->tmp_angleCCW65536); 							/* 角度 */
			tama_system_regist_katayori_n_way();/* (r33-) */
		//
			br.BULLET_REGIST_02_angle65536 				= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));					/* 角度 */
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}

/*[CCW 反時計回り]*/
static void spell_create_21_sonota_debug_ccw_aka/*CCW*/(SPRITE *src)
{
	if (0==((src->boss_base_spell_time_out)&0x03))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
			obj_send1->cx256						= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
			obj_send1->cy256						= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
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
			//
			br.BULLET_REGIST_00_speed256				= (t256(1.0));	/* 弾速 */
			br.BULLET_REGIST_01_speed_offset			= t256(1);/*(テスト)*/
		//	br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_01_AKA; 				/* [枠付き赤丸弾] */
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)標準弾 */
			br.BULLET_REGIST_06_n_way					= (6);									/* [6way] */
			br.BULLET_REGIST_07_div_angle65536  		= (int)(65536/48);						/* 分割角度(1024/(6*8) ) １周を8分割した領域をさらに6分割した角度 */
	//
		{
			br.BULLET_REGIST_02_angle65536  			= (src->tmp_angleCCW65536); 							/* 角度 */
			tama_system_regist_katayori_n_way();/* (r33-) */
		//
			br.BULLET_REGIST_02_angle65536  			= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));					/* 角度 */
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}

