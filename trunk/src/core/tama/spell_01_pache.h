
/*---------------------------------------------------------
 東方模倣風 ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	パチュリー・ノーレッジの弾幕を生成します。
---------------------------------------------------------*/

static void test_pache_16_shot_AKA(SPRITE *src)
{
	set_sub_jikinerai(src);
	//
	br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(16));	/* 分割角度(1024[360/360度]を 16 分割) */	/* 1周をn分割した角度 */
	br.BULLET_REGIST_06_n_way					= (16); 	/* [16way] */	/* 発弾数 */
	//
	br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
	br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_MARU10_01_AKA;//BULLET_WAKU12_01_AKA;//							/* [赤丸弾] */
//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;/* 非傾き弾に設定する(r33-) */
//	br.BULLET_REGIST_01_speed_offset			= t256(1);/*(テスト)*/
//	br.BULLET_REGIST_01_speed_offset			= t256(6);/*(テスト)*/
	unsigned int jj;
	for (jj=0; jj<(4);	jj ++)/* 4弾 */
	{
		br.BULLET_REGIST_01_speed_offset		= (3+jj)<<8;/*(テスト)*/
		br.BULLET_REGIST_00_speed256			= (t256(1.00)+(jj<<7)); 		/* 弾速 */
		tama_system_regist_katayori_n_way();/* (r33-) */
	}
}
static void test_pache_16_shot_AOI(SPRITE *src)
{
	set_sub_jikinerai(src);
	//
	br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(16));	/* 分割角度(1024[360/360度]を 16 分割) */	/* 1周をn分割した角度 */
	br.BULLET_REGIST_06_n_way					= (16); 	/* [16way] */	/* 発弾数 */
	//
	br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
	br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_MARU10_03_AOI;//BULLET_WAKU12_01_AKA;//							/* [赤丸弾] */
//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;/* 非傾き弾に設定する(r33-) */
//	br.BULLET_REGIST_01_speed_offset			= t256(1);/*(テスト)*/
//	br.BULLET_REGIST_01_speed_offset			= t256(6);/*(テスト)*/
	unsigned int jj;
	for (jj=0; jj<(4);	jj ++)/* 4弾 */
	{
		br.BULLET_REGIST_01_speed_offset		= (3+jj)<<8;/*(テスト)*/
		br.BULLET_REGIST_00_speed256			= (t256(1.00)+(jj<<7)); 		/* 弾速 */
		tama_system_regist_katayori_n_way();/* (r33-) */
	}
}
/*
魔方陣の位置から(赤丸弾)自機狙い弾(2回)

*/
extern void add_zako_pache_laser(SPRITE *src);/* アリス人形弾幕 */
static void spell_create_42_pache_laser1(SPRITE *src)
{
	static int laser_muki=0;
	static int laser_count=0;
	laser_count--;
	if (0>laser_count)
	{
		laser_count = (150);
		laser_muki ^= 1;
		if (0==(laser_muki&1))
		{
			lz[0].laser_angle65536 = (0+(65536/6)); 		/* 1/8周(60/360度) */
		}
		else
		{
			lz[0].laser_angle65536 = (65536-(65536/6)); 	/* -1/8周(60/360度) */
		}
	}
	else
	if (100<laser_count)
	{
		;/*停止*/
	}
	else
	{
		;/* 移動 **/
		if (0==(laser_muki&1))
		{
			lz[0].laser_angle65536 -= (100);
		}
		else
		{
			lz[0].laser_angle65536 += (100);
		}
	}

	if ((65)>((src->boss_base_spell_time_out)&0xff))/* 5回 3回 */
	{
		if ((0)==((src->boss_base_spell_time_out)&0x0f))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			#endif
			test_pache_16_shot_AKA(src);
		}
	}
	#if 0
	/*0x280*/if ((640-1)==((src->boss_base_spell_time_out) ))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_pache_laser(src);
		test_pache_16_shot_AKA(src);
	}
	/*0x200*/if ((640-128)==((src->boss_base_spell_time_out) ))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_pache_laser(src);
	}
	#endif
}
static void spell_create_43_pache_laser2(SPRITE *src)
{
	static int laser_muki=0;
	static int laser_count=0;
	laser_count--;
	if (0>laser_count)
	{
		laser_count = (150);
		laser_muki ^= 1;
		if (0==(laser_muki&1))
		{
			lz[0].laser_angle65536 = (0+(65536/6)); 		/* 1/8周(60/360度) */
		}
		else
		{
			lz[0].laser_angle65536 = (65536-(65536/6)); 	/* -1/8周(60/360度) */
		}
	}
	else
	if (100<laser_count)
	{
		;/*停止*/
	}
	else
	{
		;/* 移動 **/
		if (0==(laser_muki&1))
		{
			lz[0].laser_angle65536 -= (100);
		}
		else
		{
			lz[0].laser_angle65536 += (100);
		}
	}

	if ((65)>((src->boss_base_spell_time_out)&0xff))/* 9回 5回 3回 */
	{
		if ((0)==((src->boss_base_spell_time_out)&0x07))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			#endif
			test_pache_16_shot_AOI(src);
		}
	}
	#if 0
	/*0x280*/if ((640-1)==((src->boss_base_spell_time_out) ))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_pache_laser(src);
		test_pache_16_shot_AKA(src);
	}
	/*0x200*/if ((640-128)==((src->boss_base_spell_time_out) ))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_pache_laser(src);
	}
	#endif
}


/*---------------------------------------------------------
	[パチェ] "　　　　火符「アグニシャイン」" No.15
	-------------------------------------------------------
	アグニシャインの炎弾は雑魚にした方が現実的かも？。
---------------------------------------------------------*/
static void spell_create_2e_pache_agni_shine_1(SPRITE *src)
{
}


/*---------------------------------------------------------
	[パチェ] "　水符「プリンセスウンディネ」" No.16
	-------------------------------------------------------
	この弾幕の嫌な所は、中弾のあたり判定が(見た感じより)でかい事。
	(小弾のあたり判定は小さいのに)
	だから中弾は見た目より危険。
	なんだけど、現在の模倣風に中弾無いしなー。
	丸弾は(大玉弾、ミニ弾、小弾、枠小弾)しか無い。
	中弾はあたり判定も大きいし、とりあえず雑魚に変更しようかな？
	-------------------------------------------------------
	レーザー音、間隔0.5sぐらい(6回)計3.0s?(8回?)
	0.3sぐらい休み
	後追い弾、間隔0.3sぐらい(6回)計2.0s?
	2.0sぐらい休み
	----
	8.0s==60x8==480(?)
	6.0s==60x6==360(?)
	----
	という風に感じるのだが、実時間は1 loop 4.0sぐらいっぽい。
---------------------------------------------------------*/

static void spell_create_2f_pache_princess_undine(SPRITE *src)
{
	static int temp00_angle;
	if (64 > ((src->boss_base_spell_time_out) ))
	{
		;
	}
	else
	if (128 > ((src->boss_base_spell_time_out) ))
	{
		if (0==((src->boss_base_spell_time_out)&(16-1)))
		{
			/* 自機狙い 角を作成 */
			set_sub_jikinerai(src);
		//	temp00_angle = br.BULLET_REGIST_02_angle65536;
			/* 自機狙い 3way 白laser弾 */
		//	br.BULLET_REGIST_02_angle65536				= temp00_angle;
			//
		//	br.BULLET_REGIST_00_speed256				= (t256(0.75) );		/* 弾速 */
		//	br.BULLET_REGIST_00_speed256				= (t256(1.50) );		/* 弾速 */
			br.BULLET_REGIST_00_speed256				= (t256(1.00) );		/* 弾速 */
		//	br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(テスト)*/
			br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(テスト)*/
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_OODAMA32_00_AOI;// 						/* [青弾] */
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;/* 非傾き弾に設定する(r33-) */
			br.BULLET_REGIST_06_n_way					= (11); 	/* [3way] */	/* 発弾数 */
			br.BULLET_REGIST_07_div_angle65536 			= (int)(65536/(24)); /* 分割角度(65536[360/360度]を 16 分割) */	/* 1周をn分割した角度 */
			tama_system_regist_n_way();/* (r33-) */
		}
	}
	else
	if (250 > ((src->boss_base_spell_time_out) ))
	{
		if (0==((src->boss_base_spell_time_out)&(2-1)))
		{
			/* 自機狙い 3way 白laser弾 */
			//
		//	br.BULLET_REGIST_00_speed256				= (t256(0.75) );		/* 弾速 */
		//	br.BULLET_REGIST_00_speed256				= (t256(1.50) );		/* 弾速 */
			br.BULLET_REGIST_00_speed256				= (t256(4.00) );		/* 弾速 */
		//	br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(テスト)*/
			br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(テスト)*/
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_HARI32_00_AOI;//							/* [青針弾] */
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* 通常弾に設定する(r33-) */
			/* 1レーザーの長さ */
			#define pache_02_LASER_LENGTH (4)
			int jj;
		jj=0;// for (jj=0; jj<(128*pache_02_LASER_LENGTH); jj+=128)
			{
				br.BULLET_REGIST_02_angle65536		= temp00_angle;
				br.BULLET_REGIST_06_n_way				= (3);	/* [3way] */	/* 発弾数 */
				/* 全部のスキマ(隙間全体)、隙間が閉じる時のtime_outは128だから。 */
				#define pache_00_SUKIMA_ZENBU		((128/*time_out*/)<<4)/*(4096)*/
				/* プレイヤーがくぐるスキマ */
				#define pache_01_SUKIMA_KUGURU		(1280)
				br.BULLET_REGIST_07_div_angle65536	= jj+((src->boss_base_spell_time_out)<<4)-(pache_00_SUKIMA_ZENBU-pache_01_SUKIMA_KUGURU) ;//(int)(65536/(16));	/* 分割角度(65536[360/360度]を 16 分割) */	/* 1周をn分割した角度 */
				tama_system_regist_n_way();/* (r33-) */
			}
			if (0==((src->boss_base_spell_time_out)&(32-1)))
			{
				/* 自機狙わない 16way(?)17way(???) 青丸弾 */
				br.BULLET_REGIST_00_speed256				= (t256(0.50) );		/* 弾速 */
				br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(テスト)*/
				br.BULLET_REGIST_02_angle65536				= temp00_angle + (65536/2);
				br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_MARU10_03_AOI;//							/* [青丸弾] */
			//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;/* 非傾き弾に設定する(r33-) */
				br.BULLET_REGIST_06_n_way					= (17); 	/* [17way] */	/* 発弾数 */
				br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(19));	/* 分割角度(65536[360/360度]を 24 分割) */	/* 1周をn分割した角度 */
				tama_system_regist_n_way();/* (r33-) */
			}
		}
	}
	else
	if (254 == ((src->boss_base_spell_time_out) ))
	{
		/* 自機狙い 角を作成 */
		set_sub_jikinerai(src);
		temp00_angle = br.BULLET_REGIST_02_angle65536;
	}
}
//			br.BULLET_REGIST_01_speed_offset		= t256(1);/*(テスト)*/
//			br.BULLET_REGIST_01_speed_offset		= t256(6);/*(テスト)*/


/*---------------------------------------------------------
[パチェ] "　　　　木符「シルフィホルン」" No.17
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_30_pache_sylphy_horn_1(SPRITE *src)
{
}


/*---------------------------------------------------------
[パチェ] "　　土符「レイジィトリリトン」" No.18
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_31_pache_rage_tririton_1(SPRITE *src)
{
}


/*---------------------------------------------------------
[パチェ] "　　金符「メタルファティーグ」" No.19
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_32_pache_metal_fatigue(SPRITE *src)
{
}


/*---------------------------------------------------------
[パチェ] "　　火符「アグニシャイン上級」" No.20
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_33_pache_agni_shine_2(SPRITE *src)
{
}


/*---------------------------------------------------------
[パチェ] "　　木符「シルフィホルン上級」" No.21
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_34_pache_sylphy_horn_2(SPRITE *src)
{
}


/*---------------------------------------------------------
[パチェ] "土符「レイジィトリリトン上級」" No.22
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_35_pache_rage_tririton_2(SPRITE *src)
{
}


/*---------------------------------------------------------
[パチェ] "　火符「アグニレイディアンス」" No.23
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_36_pache_agni_radiance(SPRITE *src)
{
}


/*---------------------------------------------------------
[パチェ] "　　　水符「ベリーインレイク」" No.24
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_37_pache_bury_in_lake(SPRITE *src)
{
}


/*---------------------------------------------------------
[パチェ] "　　　木符「グリーンストーム」" No.25
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_38_pache_green_storm(SPRITE *src)
{
}


/*---------------------------------------------------------
[パチェ] "　　土符「トリリトンシェイク」" No.26
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_39_pache_tririton_shake(SPRITE *src)
{
}


/*---------------------------------------------------------
[パチェ] "　　　金符「シルバードラゴン」" No.27
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_3a_pache_silver_dragon(SPRITE *src)
{
}


/*---------------------------------------------------------
	[パチェ] "火＆土符「ラーヴァクロムレク」" No.28
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_3b_pache_lava_cromlech(SPRITE *src)
{
}


/*---------------------------------------------------------
	[パチェ] "木＆火符「フォレストブレイズ」" No.29
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_3c_pache_forest_blaze(SPRITE *src)
{
}


/*---------------------------------------------------------
	[パチェ] "　水＆木符「ウォーターエルフ」" No.30
	-------------------------------------------------------
	正直よくわからん。
	パチェっぽい？。
---------------------------------------------------------*/
static void spell_create_3d_pache_water_elf(SPRITE *src)
{
	/* 水符 自機狙い 24方向全方位、水色弾 BULLET_MARU10_04_MIZU_IRO */
	/* 木符 自機狙い 20方向全方位、緑色弾 BULLET_MARU10_05_MIDORI */
	/* 木符 自機狙い 10方向半周、青中玉弾 BULLET_OODAMA32_00_AOI */
	static int temp11_angle;
	temp11_angle += (src->boss_base_spell_time_out<<4);
	if (0==((src->boss_base_spell_time_out)&(16-1)))
	{
		int temp00_angle;
		/* 自機狙い 角を作成 */
		set_sub_jikinerai(src);
		temp00_angle = br.BULLET_REGIST_02_angle65536;
		temp11_angle += (temp00_angle);
		if (32>((src->boss_base_spell_time_out)&(128-1)))
		{
			/* 木符 自機狙い 10方向半周、青中玉弾 BULLET_OODAMA32_00_AOI */
			br.BULLET_REGIST_02_angle65536				= temp00_angle;
			//
		//	br.BULLET_REGIST_00_speed256				= (t256(0.75) );		/* 弾速 */
		//	br.BULLET_REGIST_00_speed256				= (t256(1.50) );		/* 弾速 */
			br.BULLET_REGIST_00_speed256				= (t256(1.00) );		/* 弾速 */
		//	br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(テスト)*/
			br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(テスト)*/
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_OODAMA32_00_AOI;// 						/* [青弾] */
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;/* 非傾き弾に設定する(r33-) */
			br.BULLET_REGIST_06_n_way					= (10); 	/* [10way] */	/* 発弾数 */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(20));	/* 分割角度(65536[360/360度]を 20 分割) */	/* 1周をn分割した角度 */
			tama_system_regist_n_way();/* (r33-) */
		}
		if (0x0a90&(temp11_angle))
		{
			/* 木符 自機狙い 20方向全方位、緑色弾 BULLET_MARU10_05_MIDORI */
			br.BULLET_REGIST_02_angle65536				= temp00_angle;
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_MARU10_05_MIDORI;//							/* [青弾] */
			br.BULLET_REGIST_06_n_way					= (20); 	/* [20way] */	/* 発弾数 */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(20));	/* 分割角度(65536[360/360度]を 20 分割) */	/* 1周をn分割した角度 */
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
		if (0x0670&(temp11_angle))
		{
			/* 水符 自機狙い 24方向全方位、水色弾 BULLET_MARU10_04_MIZU_IRO */
			br.BULLET_REGIST_02_angle65536				= temp00_angle;
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_MARU10_04_MIZU_IRO;//							/* [青弾] */
			br.BULLET_REGIST_06_n_way					= (24); 	/* [24] */	/* 発弾数 */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(24));	/* 分割角度(65536[360/360度]を 24 分割) */	/* 1周をn分割した角度 */
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}


/*---------------------------------------------------------
	[パチェ] "金＆水符「マーキュリポイズン」" No.31
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_3e_pache_mercury_poison(SPRITE *src)
{
}


/*---------------------------------------------------------
	[パチェ] "土＆金符「エメラルドメガリス」" No.32
	-------------------------------------------------------
	テキトーに作ってみた。
	全然違うんだけど、パチェっぽいといえば、パチェっぽい。
---------------------------------------------------------*/
static void spell_create_3f_pache_emerald_megalith(SPRITE *src)
{
	static int temp00_angle;
	temp00_angle += (src->boss_base_spell_time_out<<4);

	// ランダム緑枠弾
	if (0== ((src->boss_base_spell_time_out)&(4-1) ))
	{
		obj_send1->cx256					= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
		obj_send1->cy256					= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
		if (0==((src->boss_base_spell_time_out)&(4-1)))
		{
		//	temp00_angle = br.BULLET_REGIST_02_angle65536;
			/* 緑枠弾 */
			br.BULLET_REGIST_02_angle65536				= ((temp00_angle&(32768-1))-16384);
			//
		//	br.BULLET_REGIST_00_speed256				= (t256(0.75) );		/* 弾速 */
		//	br.BULLET_REGIST_00_speed256				= (t256(1.50) );		/* 弾速 */
			br.BULLET_REGIST_00_speed256				= (t256(0.75)+(temp00_angle&0x007f)+((cg_game_difficulty)<<4) );		/* 弾速 */
		//	br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(テスト)*/
			br.BULLET_REGIST_01_speed_offset			= ((temp00_angle>>4)&0x07ff);/*(テスト)*/
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_05_MIDORI;//							/* [緑枠弾] */
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;/* 非傾き弾に設定する(r33-) */
			br.BULLET_REGIST_06_n_way					= (2);	/* [3way] */	/* 発弾数 */
			br.BULLET_REGIST_07_div_angle65536 			= (int)(65536/(24)); /* 分割角度(65536[360/360度]を 16 分割) */	/* 1周をn分割した角度 */
			tama_system_regist_n_way();/* (r33-) */
		}
	}
	// ランダム直下落下、緑大玉弾
	if (0== ((src->boss_base_spell_time_out)&(16-1) ))
	{
		const u32 ra_nd32 = (ra_nd()/*&0xffff*/);
		temp00_angle += ra_nd32;
		obj_send1->cx256							= t256(16)+(ra_nd32&0xff00)+(ra_nd32&0x3f00);			/* 弾源x256 */
		obj_send1->cy256							= (src->cy256) -t256(32)+((ra_nd32>>8)&0x3f00); 		/* 弾源y256 */
		/* 自機狙い 3way 白laser弾 */
		/* 自機狙わない 16way(?)17way(???) 青丸弾 */
		br.BULLET_REGIST_00_speed256				= (t256(0.50) );		/* 弾速 */
		br.BULLET_REGIST_01_speed_offset			= (8)<<8;/*(テスト)*/
		br.BULLET_REGIST_02_angle65536				= (0);
		br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
		br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_OODAMA32_03_MIDORI;//							/* [緑大玉弾] */
	//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;/* 非傾き弾に設定する(r33-) */
		/* easyは1方向,  lunatic は3方向 */
		br.BULLET_REGIST_06_n_way					= (1+((cg_game_difficulty)>>1));	/* [1way] */	/* 発弾数 */
		br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(19));	/* 分割角度(65536[360/360度]を 24 分割) */	/* 1周をn分割した角度 */
		tama_system_regist_n_way();/* (r33-) */
	}
}
