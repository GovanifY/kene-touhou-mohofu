
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	パチュリー・ノーレッジのカードを定義します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	赤弾
---------------------------------------------------------*/
local void test_pache_16_shot_AKA(OBJ *src)
{
//	REG_02_DEST_X	= ((src->cx256));
//	REG_03_DEST_Y	= ((src->cy256));
	calculate_jikinerai();
	//
	HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	HATSUDAN_05_bullet_obj_type 		= (BULLET_MARU10_BASE + TAMA_IRO_01_AKA);		/* [赤丸弾] */
	HATSUDAN_06_n_way					= (16); 										/* [16way] */	/* 発弾数 */
	HATSUDAN_07_div_angle65536			= (int)(65536/(16));	/* 分割角度(1024[360/360度]を 16 分割) */	/* 1周をn分割した角度 */
	//
	unsigned int jj;
	for (jj=0; jj<(4); jj++)/* 4弾 */
	{
		HATSUDAN_02_speed_offset		= (3+jj)<<8;/*(テスト)*/
		HATSUDAN_01_speed256			= (t256(1.00)+(jj<<7)); 		/* 弾速 */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
	}
}

/*---------------------------------------------------------
	青弾
---------------------------------------------------------*/
local void test_pache_16_shot_AOI(OBJ *src)
{
	HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	HATSUDAN_05_bullet_obj_type 		= (BULLET_MARU10_BASE + TAMA_IRO_03_AOI);		/* [青丸弾] */
//	HATSUDAN_06_n_way					= (16); 										/* [16way] */	/* 発弾数 */
//	HATSUDAN_07_div_angle65536			= (int)(65536/(16));	/* 分割角度(1024[360/360度]を 16 分割) */	/* 1周をn分割した角度 */
	HATSUDAN_06_n_way					= (8);											/* [8way] */	/* 発弾数 */
	HATSUDAN_07_div_angle65536			= (int)(65536/(8)); /* 分割角度(1024[360/360度]を 8 分割) */	/* 1周をn分割した角度 */
	//
	unsigned int ii;
	for (ii=0; ii<(65536); ii+=(65536/4))/* 45 */
	{
		HATSUDAN_03_angle65536 = (lz[0].laser_angle65536+ii);
		//
		REG_0b_REG3 = ((HATSUDAN_03_angle65536));
		// ベクトル弾登録。 regist vector.
		sincos65536_NUM(REG_NUM_0b_REG3, REG_NUM_0c_REG4);
		REG_02_DEST_X	 = (((REG_0c_REG4)*(REG_09_REG1))>>8);
		REG_03_DEST_Y	 = (((REG_0d_REG5)*(REG_09_REG1))>>8);
		//
		REG_02_DEST_X	+= ((src->cx256));
		REG_03_DEST_Y	+= ((src->cy256));
		//
		calculate_jikinerai();
		//
	//	REG_0c_REG4 = ((HATSUDAN_03_angle65536) );
	//	//
	//	HATSUDAN_03_angle65536 = (REG_0c_REG4 );
	//	REG_02_DEST_X	= ((src->cx256));
	//	REG_03_DEST_Y	= ((src->cy256));
		/* 弾源x256 y256 弾の中心から発弾。 */
	//	set_REG_DEST_XY(src);	/*(親の位置から発弾)*/
		unsigned int jj;
		for (jj=0; jj<(4); jj++)/* 4弾 */
		{
			HATSUDAN_02_speed_offset	= (3+jj)<<8;/*(テスト)*/
			HATSUDAN_01_speed256		= (t256(1.00)+(jj<<7)); 		/* 弾速 */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}
/*---------------------------------------------------------
	魔方陣の位置から(赤丸弾)自機狙い弾(2回)
---------------------------------------------------------*/
extern void add_zako_pache_laser(OBJ *src);/* アリス人形カード */
local void spell_create_42_pache_laser1(OBJ *src)
{
	static int laser_muki=0;
	static int laser_count=0;
	laser_count--;
	if (0>laser_count)
	{
		laser_count = (150);
		laser_muki ^= 1;
		if (0==(laser_muki&1))
				{	lz[0].laser_angle65536 = (0+(65536/6)); 		}	/* +1/8周(60/360度) */
		else	{	lz[0].laser_angle65536 = (65536-(65536/6)); 	}	/* -1/8周(60/360度) */
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

	if ((65)>((REG_10_BOSS_SPELL_TIMER)&0xff))/* 5回 3回 */
	{
		if ((0)==((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			test_pache_16_shot_AKA(src);
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
			#endif
		}
	}
	#if 0
	/*0x280*/if ((640-1)==((REG_10_BOSS_SPELL_TIMER) ))
	{
		add_zako_pache_laser(src);
		test_pache_16_shot_AKA(src);
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
	/*0x200*/if ((640-128)==((REG_10_BOSS_SPELL_TIMER) ))
	{
		add_zako_pache_laser(src);
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
	#endif
}


/*---------------------------------------------------------
	レーザー付けると難しくなりすぎそう。
	(原作のパチェレーザーも十分難しいけど)
---------------------------------------------------------*/

/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/

local void spell_init_43_pache_laser2(OBJ *src)
{
	REG_09_REG1 	= (t256(64));//[定数1]発弾位置までの距離。
}
/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	?。
	REG_09_REG1 	[定数1]発弾位置までの距離。
//	REG_0a_REG2 	z
//	REG_0b_REG3 	難易度別定数。
---------------------------------------------------------*/
local void spell_create_43_pache_laser2(OBJ *src)
{
	static int laser_muki=0;
	static int laser_count=0;
	laser_count--;
	if (0>laser_count)
	{
		laser_count = (150);
		laser_muki ^= 1;
		if (0==(laser_muki&1))
				{	lz[0].laser_angle65536 = (0+(65536/6)); 		}	/* +1/8周(60/360度) */
		else	{	lz[0].laser_angle65536 = (65536-(65536/6)); 	}	/* -1/8周(60/360度) */
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
	//
//	if ((65)>((REG_10_BOSS_SPELL_TIMER)&0xff))/* 9回 5回 3回 */
	if ((65)>((REG_10_BOSS_SPELL_TIMER)&0xff))/* 9回 5回 3回 */
	{
		if ((0)==((REG_10_BOSS_SPELL_TIMER)&0x07))
		{
			test_pache_16_shot_AOI(src);
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
			#endif
		}
	}
}
	#if 0
	/*0x280*/if ((640-1)==((REG_10_BOSS_SPELL_TIMER) ))
	{
		add_zako_pache_laser(src);
		test_pache_16_shot_AKA(src);
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
	/*0x200*/if ((640-128)==((REG_10_BOSS_SPELL_TIMER) ))
	{
		add_zako_pache_laser(src);
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
	#endif


/*---------------------------------------------------------
	[パチェ] "　　　　火符「アグニシャイン」" No.15
	-------------------------------------------------------
	アグニシャインの炎弾は雑魚にした方が現実的かも？。
---------------------------------------------------------*/

/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
#define aaa_TAMA_DATA_10_TAMA_OKI_KANKAKU	src->hatudan_register_user_data10
local void aaa_tamaoki_danmaku_01_callback(OBJ *src)/* 珠置き */
{
	/*(225==1+224==1+7*32)*/
//	if ((HATUDAN_ITI_NO_JIKAN-224) < src->jyumyou)/* 発弾エフェクト後から[224]カウント未満の弾 */
//	if ((HATUDAN_ITI_NO_JIKAN-225) < src->jyumyou)/* 発弾エフェクト後から[225]カウント未満の弾 */
	if ((HATUDAN_ITI_NO_JIKAN-320) < src->jyumyou)/* 発弾エフェクト後から[225]カウント未満の弾 */
	{
		if (0==(src->hatudan_register_spec_data & TAMA_SPEC_KAITEN_HOUKOU_BIT))/*TAMA_SPEC_AKA_AO_KUNAI_BIT*/
		{	/*CW(時計回り)*/
		//	src->rotationCCW1024 -= (6);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		//	src->rotationCCW1024 -= (3);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
			src->rotationCCW1024 -= (6);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		}
		else
		{	/*CCW(反時計回り)*/
		//	src->rotationCCW1024 += (6);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		//	src->rotationCCW1024 += (3);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
			src->rotationCCW1024 += (6);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		}
//		src->hatudan_register_speed65536	+= (1<<8);			/* 弾速 */
		src->hatudan_register_speed65536	+= (512);		/*(加速)*/	/* 弾速 */
	//
	//	if (0==(src->jyumyou&0x0f))/*16回に1回弾を置く*/
	//	if (0==(src->jyumyou&0x07))/*8回に1回弾を置く*/
		aaa_TAMA_DATA_10_TAMA_OKI_KANKAKU--;
		if (0 > aaa_TAMA_DATA_10_TAMA_OKI_KANKAKU)
		{
			aaa_TAMA_DATA_10_TAMA_OKI_KANKAKU = REG_09_REG1;//[定数1]n回に一回弾を置く。

			#if 0
			//
		//	REG_02_DEST_X	= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
		//	REG_03_DEST_Y	= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
			set_REG_DEST_XY(src);
			REG_00_SRC_X	= (REG_0c_REG4);
			REG_01_SRC_Y	= (REG_0d_REG5);
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
			#endif
		}
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
	[初期化セクション]
	-------------------------------------------------------
	[定数1]n回に一回弾を置く。
	0easy:			16
	1normal:		12
	2hard:			8
	3lunatic:		4
---------------------------------------------------------*/
local void spell_init_2e_pache_agni_shine_1(OBJ *src)
{
//	aaa_TAMA_DATA_10_TAMA_OKI_KANKAKU	= (0);// カウンタ
	REG_09_REG1 	= ( (16)-((REG_0f_GAME_DIFFICULTY)<<2));//[定数1]n回に一回弾を置く。
//	REG_09_REG1 	= (t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<6));//[定数1]雨の速度
//	REG_0a_REG2 	= ((1024/2)+(1024/24)+(REG_0f_GAME_DIFFICULTY<<3));//[定数2]赤青クナイが曲がる角度
	card.danmaku_callback[1] = aaa_tamaoki_danmaku_01_callback;/*(珠置き用)*/
//	card.danmaku_callback[2] = aaa_tahane_danmaku_02_callback;/*(珠跳ね用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}

/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	現在(r35)のスペカシステムの場合、仕様上
	スペカがまたがるレジスタ値を保持できない。
	(スペカが始まるタイミングで強制的にレジスタが0クリアされてしまう為)
	-------------------------------------------------------
	使用レジスタ:
	REG_0c_REG4 	発弾後、集まるX座標(t256形式)
	REG_0d_REG5 	発弾後、集まるY座標(t256形式)
---------------------------------------------------------*/
local void spell_create_2e_pache_agni_shine_1(OBJ *src)
{
//	if ((SPELL_TIME_0256-1)==((REG_10_BOSS_SPELL_TIMER) ))/*(一番始め)*/
//	if (0== (REG_10_BOSS_SPELL_TIMER & 0x7f) ) /* 128回に1回なら撃つ */
	if (0x0f > (REG_10_BOSS_SPELL_TIMER & 0x7f) ) /* 128回に1回なら撃つ */
	{
	//	HATSUDAN_01_speed256			= t256(1.0);	/* 弾速 */
	//	HATSUDAN_02_speed_offset		= t256(5);/*(テスト)*/
		HATSUDAN_01_speed256			= t256(0.75);	/* 弾速 */
		HATSUDAN_02_speed_offset		= t256(1.5);/*(テスト)*/
		HATSUDAN_03_angle65536			= 0;	/* 基準角度 */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|((ra_nd()) & TAMA_SPEC_KAITEN_HOUKOU_BIT)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_UROKO14_BASE + TAMA_IRO_01_AKA);			/* [赤鱗弾] */
		HATSUDAN_06_n_way				= (13); 	/* [13way] */		/* 発弾数 */
		HATSUDAN_07_div_angle65536		= (int)(65536/(13));	/* 分割角度(65536[360/360度]を 18 分割) */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		u32 aaa = ra_nd();
		REG_0c_REG4 = ((aaa    )&0xff00) + t256(((480-128-32-256)/2));
		REG_0d_REG5 = ((aaa<<8 )&0xff00) + t256(((272-256)/2));
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
}
	#if 0/*(メモ)*/
	/*"超劣化アグニシャイン"*/
	if (0== (REG_10_BOSS_SPELL_TIMER & 0x01) ) /* 2回に1回なら撃つ */
	{
		HATSUDAN_01_speed256			= (t256(1.0));		/* 弾速 */
		HATSUDAN_02_speed_offset		= (t256(1.5));/*(テスト)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_UROKO14_BASE + TAMA_IRO_01_AKA);			/* [赤鱗弾] */
		REG_0a_REG2 = ra_nd();
		REG_0b_REG3 = REG_0a_REG2 & 0xff;
		//
		HATSUDAN_03_angle65536			= (REG_0a_REG2);
		hatudan_system_regist_single();
	}
	#endif


/*---------------------------------------------------------
	[パチェ] "　水符「プリンセスウンディネ」" No.16
	-------------------------------------------------------
	このカードの嫌な所は、中弾のあたり判定が(見た感じより)でかい事。
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
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。
	REG_09_REG1 	temp00_angle
---------------------------------------------------------*/

local void spell_create_2f_pache_princess_undine(OBJ *src)
{
	if (64 > ((REG_10_BOSS_SPELL_TIMER) ))
	{
		;
	}
	else
	if (128 > ((REG_10_BOSS_SPELL_TIMER) ))
	{
		if (0==((REG_10_BOSS_SPELL_TIMER)&(16-1)))
		{
			/* 自機狙い 角を作成 */
			REG_02_DEST_X	= ((src->cx256));
			REG_03_DEST_Y	= ((src->cy256));
			calculate_jikinerai();
		//	REG_09_REG1 = HATSUDAN_03_angle65536;
			/* 自機狙い 3way 白laser弾 */
		//	HATSUDAN_03_angle65536			= REG_09_REG1;
		//	HATSUDAN_01_speed256			= (t256(0.75)); 	/* 弾速 */
		//	HATSUDAN_01_speed256			= (t256(1.50)); 	/* 弾速 */
			HATSUDAN_01_speed256			= (t256(1.00)); 	/* 弾速 */
			HATSUDAN_02_speed_offset		= (t256(3));/*(テスト)*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_OODAMA32_00_AOI);//			/* [青弾] */
			HATSUDAN_06_n_way				= (11); 	/* [11way] */				/* 発弾数 */
			HATSUDAN_07_div_angle65536		= (int)(65536/(24)); /* 分割角度(65536[360/360度]を 16 分割) */ /* 1周をn分割した角度 */
			hatudan_system_regist_n_way();/* (r33-) */
		}
	}
	else
	if (250 > ((REG_10_BOSS_SPELL_TIMER) ))
	{
		if (0==((REG_10_BOSS_SPELL_TIMER)&(2-1)))
		{
			/* 自機狙い 3way 白laser弾 */
			//
		//	HATSUDAN_01_speed256			= (t256(0.75)); 	/* 弾速 */
		//	HATSUDAN_01_speed256			= (t256(1.50)); 	/* 弾速 */
			HATSUDAN_01_speed256			= (t256(4.00)); 	/* 弾速 */
			HATSUDAN_02_speed_offset		= (t256(3));/*(テスト)*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_HARI32_00_AOI);// 				/* [青針弾] */
			/* 1レーザーの長さ */
			#define pache_02_LASER_LENGTH (4)
			int jj;
			jj = 0;// for (jj=0; jj<(128*pache_02_LASER_LENGTH); jj+=128)
			{
				HATSUDAN_03_angle65536		= REG_09_REG1;
				HATSUDAN_06_n_way				= (3);	/* [3way] */	/* 発弾数 */
				/* 全部のスキマ(隙間全体)、隙間が閉じる時のtime_outは128だから。 */
				#define pache_00_SUKIMA_ZENBU		((128/*time_out*/)<<4)/*(4096)*/
				/* プレイヤーがくぐるスキマ */
				#define pache_01_SUKIMA_KUGURU		(1280)
				HATSUDAN_07_div_angle65536		= jj+((REG_10_BOSS_SPELL_TIMER)<<4)-(pache_00_SUKIMA_ZENBU-pache_01_SUKIMA_KUGURU);
				//(int)(65536/(16));	/* 分割角度(65536[360/360度]を n 分割) */	/* 1周をn分割した角度 */
				hatudan_system_regist_n_way();/* (r33-) */
			}
			if (0==((REG_10_BOSS_SPELL_TIMER)&(32-1)))
			{
				/* 自機狙わない 16way(?)17way(???) 青丸弾 */
				HATSUDAN_01_speed256			= (t256(0.50)); 					/* 弾速 */
				HATSUDAN_02_speed_offset		= (t256(3));/*(テスト)*/
				HATSUDAN_03_angle65536			= REG_09_REG1 + (65536/2);
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_MARU10_BASE + TAMA_IRO_03_AOI);// 			/* [青丸弾] */
				HATSUDAN_06_n_way				= (17); 	/* [17way] */				/* 発弾数 */
				HATSUDAN_07_div_angle65536		= (int)(65536/(19));	/* 分割角度(65536[360/360度]を 19 分割) */
				hatudan_system_regist_n_way();/* (r33-) */
			}
		}
	}
	else
	if (254 == ((REG_10_BOSS_SPELL_TIMER) ))
	{
		/* 自機狙い 角を作成 */
		REG_02_DEST_X	= ((src->cx256));
		REG_03_DEST_Y	= ((src->cy256));
		calculate_jikinerai();
		REG_09_REG1 = HATSUDAN_03_angle65536;
	}
}


/*---------------------------------------------------------
[パチェ] "　　　　木符「シルフィホルン」" No.17
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	乱数?。
	REG_09_REG1 	[定数1]雨の速度
	REG_0a_REG2 	?。
	REG_0b_REG3 	?。
//
	REG_0c_REG4 	一時使用マスク。
//	REG_0d_REG5 	?。
//	REG_0e_REG6 	?。
//	REG_0f_REG7 	難易度。

---------------------------------------------------------*/


/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
	交差弾。反時計回り。(0x0000)
	交差弾。時計回り。(0x0100)
---------------------------------------------------------*/
local void pache_sh1_danmaku_01_callback(OBJ *src)/* シルフィホルン */
{
	#if 1
	if ((HATUDAN_ITI_NO_JIKAN-64) < src->jyumyou)/* 発弾エフェクト時は無効 */
	{
		return;
	}
	#endif
	#if 1
	if ((HATUDAN_ITI_NO_JIKAN-64) == src->jyumyou)/* 発弾エフェクト後から64カウント経過した弾 */
	{
		angle_to_vector(src);
	//	src->vx256	= -t256(1.0);
	//	src->vy256	=  t256(1.0);
	//	src->hatudan_register_tra65536 = (0);
	}
	#endif
//	src->cx256 &= (0x0001ffff);
	if (t256(32) > (src->cx256) )
	{
		src->cx256 += t256(320);/*(320==480-128-32)*/
	}
//	move_vector_gamen_sita(src);/*(ベクトル移動+画面下消去)*/


	REG_08_REG0 += (31);
	REG_0c_REG4 = REG_08_REG0;
	REG_0c_REG4 &= 0xff;

	// ベクトル弾移動。move vector.
//	src->cx256 -= (t256(1.0));	/*fps_factor*/
//	src->cy256 += (t256(1.0));	/*fps_factor*/
//	src->cx256 += (src->vx256); 	/*fps_factor*/
//	src->cy256 += (src->vy256); 	/*fps_factor*/
//	src->cx256 += (((src->vx256)*(REG_0c_REG4))>>8);	/*fps_factor*/
//	src->cy256 += (((src->vy256)*(REG_0c_REG4))>>8);	/*fps_factor*/
	src->cx256 += (((src->vx256)*(REG_0c_REG4))>>7);/*特殊*/	/* 弾速(###x256倍) */	/*fps_factor*/
	src->cy256 += (((src->vy256)*(REG_0c_REG4))>>7);/*特殊*/	/* 弾速(###x256倍) */	/*fps_factor*/
//	src->cx256 += 1;//(t256( 0.1)); 	/*fps_factor*/
//	src->cy256 += 1;//(t256(-0.1)); 	/*fps_factor*/
//	src->vy256 += ((src->hatudan_register_tra65536)<<8);	/* 加減速調整 */
//	src->vy256 += ((src->hatudan_register_tra65536) );	/* 加減速調整 */


	// 消去判定
	#if 1
	// 画面外は消す。
//	hatudan_system_B_gamen_gai_tama_kesu(src);/*(通常弾用)*/
	/* 画面外の場合は弾を消す。 */
	if (
//	(0 < (rect_clip.bullet_clip_min.x256)-(src->cx256) ) ||
//	(0 > (rect_clip.bullet_clip_max.x256)-(src->cx256) ) ||
//	(0 < (rect_clip.bullet_clip_min.y256)-(src->cy256) ) ||
//	(0 > (rect_clip.bullet_clip_max.y256)-(src->cy256) ) )	/*(画面下のみ)*/
//	( (src->cy256) > (rect_clip.bullet_clip_max.y256) ) )	/*(画面下のみ)*/
	( (src->cy256) > (t256(272)) ) )	/*(画面下のみ)*/
	{
		src->jyumyou = JYUMYOU_NASI;
	}
	#endif
}


/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/

local void spell_init_30_pache_sylphy_horn_1(OBJ *src)
{
	REG_09_REG1 	= (t256(1.0));// [定数1]雨の速度
	card.danmaku_callback[1] = pache_sh1_danmaku_01_callback;/*(枠付き丸弾。交差弾。? 回り。)*/
//	card.danmaku_callback[2] = NULL;/*(未使用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}

/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
---------------------------------------------------------*/
local void spell_create_30_pache_sylphy_horn_1(OBJ *src)
{
//	sta tic int REG_08_REG0;
	REG_08_REG0 += (REG_10_BOSS_SPELL_TIMER<<4);
	/*"超劣化シルフィホルン"*/
//	if (0==(REG_10_BOSS_SPELL_TIMER & 0x0f))
	if (0==(REG_10_BOSS_SPELL_TIMER & 0x07))
	{
		const u32 ra_nd32 = (ra_nd()/*&0xffff*/);
//		REG_0b_REG3 = rand32 & 0xff;
		REG_08_REG0 += ra_nd32;
		REG_02_DEST_X	= t256(16)+(ra_nd32 & 0xff00)+(ra_nd32 & 0x3f00);	/* 弾源x256 */
//		REG_03_DEST_Y	+= -t256(32)+((ra_nd32>>8)&0x3f00); 				/* 弾源y256 */
//		REG_02_DEST_X	= ((rand32) & (0x0001ffff) );/*(512-1==0x01ff)*/
		REG_03_DEST_Y	= (t256(-8));/*(画面外から降る)*/
	//	REG_03_DEST_Y	= (t256(32));/*(画面外から降る)*/
//
//		HATSUDAN_01_speed256			= t256(1.0);		/* 弾速 */
//		HATSUDAN_01_speed256			= t256(12.0);		/* 弾速 */
//		HATSUDAN_01_speed256			= t256(33.0);		/* 弾速 */
//		HATSUDAN_01_speed256			= t256(128.0);		/* 弾速 */
		HATSUDAN_01_speed256/*特殊(無視？)*/	= t256(1.0);		/* 弾速(###x256倍) */
//		HATSUDAN_02_speed_offset		= t256(3.0);/*(テスト)*/
	//	HATSUDAN_02_speed_offset		= t256(1.5);/*(テスト)*/
		HATSUDAN_02_speed_offset		= (0);/*(テスト)*/
//		HATSUDAN_03_angle65536			= (REG_0a_REG2);
//		HATSUDAN_03_angle65536			= (65536)-(65536/8);//+(REG_0b_REG3);
		HATSUDAN_03_angle65536			= (65536)-(65536/16)-(ra_nd32 & 0x0fff);//+(REG_0b_REG3);
//		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_KOME_BASE + TAMA_IRO_05_MIDORI);			/* [緑米弾] */
		REG_0a_REG2 = ra_nd();
		REG_0b_REG3 = REG_0a_REG2 & 0xff;
		hatudan_system_regist_single();
	}
}

/*---------------------------------------------------------
[パチェ] "　　土符「レイジィトリリトン」" No.18
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
	交差弾。反時計回り。(0x0000)
	交差弾。時計回り。(0x0100)
---------------------------------------------------------*/
local void pache_rt1_danmaku_01_callback(OBJ *src)/* マーキュリポイズン */
{
	int aaa_jikan;
	aaa_jikan = ((src->jyumyou) & 0x3f);/* 64で割った余り */
	if (0==aaa_jikan)/* 0なら左右にぶれる。 */
	{
		src->rotationCCW1024 += ((ra_nd()&0xff)-128);/* 90/360 度(+-45/360) 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		src->hatudan_register_speed65536 = (t256(1.00)<<8); 	/* 弾速 */
	}
	else
	/* 32以上は減速 */
//	if ((32) < aaa_jikan)/* 発弾エフェクト後から[64]カウント未満の弾 */
	{
		src->hatudan_register_speed65536	-= (t256(0.02)<<8); 		/* 弾速 */
		src->hatudan_register_speed65536	= psp_max( (0), (src->hatudan_register_speed65536) );
	}
	danmaku_00_standard_angle_mover(src);/*(角度弾移動+画面外弾消し)*/
//	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
	// 画面外は消す。
//	hatudan_system_B_gamen_gai_tama_kesu(src);/*(通常弾用)*/
}

/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/

local void spell_init_31_pache_rage_tririton_1(OBJ *src)
{
	card.danmaku_callback[1] = pache_rt1_danmaku_01_callback;/*(枠付き丸弾。交差弾。? 回り。)*/
//	card.danmaku_callback[2] = NULL;/*(未使用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}

/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
---------------------------------------------------------*/
local void spell_create_31_pache_rage_tririton_1(OBJ *src)
{
	// 最初の発射 (64フレーム==約1秒)ばら撒く。
	if (((64))<(REG_10_BOSS_SPELL_TIMER)) // 192== (64*3)
	{
		HATSUDAN_01_speed256			= (t256(2.00) );						/* 弾速 */
		HATSUDAN_02_speed_offset		= (t256(3));/*(テスト)*/
		HATSUDAN_03_angle65536			= (ra_nd() &(0x7fff)) + (65536/4)+(65536/2);/*(下半分)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_06_KI_IRO);//			/* [黄弾] */
	//	HATSUDAN_06_n_way				= (1);	/* [1way] */				/* 発弾数 */
	//	HATSUDAN_07_div_angle65536		= (int)(65536/(24));	/* 分割角度(65536[360/360度]を 19 分割) */
		hatudan_system_regist_single();
	}
}

	#if 0//(1==PACHE_TEST)
	{
		/* 自機狙い 3way 白laser弾 */
		HATSUDAN_03_angle65536			= ((REG_10_BOSS_SPELL_TIMER));
	}
	if ((0)==((REG_10_BOSS_SPELL_TIMER)&4)) // 4回に1回チェック
	{
		exchange_damnaku_pache_sin_sokudo_maru_dan();
	}
	#endif


/*---------------------------------------------------------
[パチェ] "　　金符「メタルファティーグ」" No.19
	-------------------------------------------------------
---------------------------------------------------------*/


/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
	交差弾。反時計回り。(0x0000)
	交差弾。時計回り。(0x0100)
---------------------------------------------------------*/
local void pache_mt1_danmaku_01_callback(OBJ *src)/* シルバードラゴン */
{
	if ((HATUDAN_ITI_NO_JIKAN-32) < src->jyumyou)/* 発弾エフェクト後から[64]カウント未満の弾 */
	{
	//	src->hatudan_register_speed65536	-= (t256(0.02)<<8); 		/* 弾速 */
		src->hatudan_register_speed65536	-= (t256(0.03)<<8); 		/* 弾速 */
		src->hatudan_register_speed65536	= psp_max( (0), (src->hatudan_register_speed65536) );
	}
	else
	if ((HATUDAN_ITI_NO_JIKAN-64) < src->jyumyou)/* 発弾エフェクト後から[64]...[128]カウント未満の弾 */
	{
		/*(親は消える)*/
		src->jyumyou = JYUMYOU_NASI;/*(弾を消す)*/
		//
		/* 弾源x256 y256 弾の中心から発弾。 */
		set_REG_DEST_XY(src);	/*(親の位置から発弾)*/
		//
		HATSUDAN_01_speed256			= t256(1.00);		/* 弾速 */
	//	HATSUDAN_02_speed_offset		= (1<<(2+8));/* x4倍 てすと*/
		HATSUDAN_02_speed_offset		= t256(3+1);/*(テスト)*/
		HATSUDAN_03_angle65536			= (ra_nd() &(0x7fff)) + (65536/4)+(65536/2);/*(下半分)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_OODAMA32_02_KIIRO);	/* [黄色 大玉弾] */
		HATSUDAN_06_n_way				= (8);							/* [8way] */		/* 発弾数 */
		HATSUDAN_07_div_angle65536		= (int)(65536/(8)); 	/* 分割角度(65536[360/360度]を 8 分割) */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
	}
//	danmaku_00_standard_angle_mover(src);/*(角度弾移動+画面外弾消し)*/
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
}

/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/

local void spell_init_32_pache_metal_fatigue(OBJ *src)
{
	card.danmaku_callback[1] = pache_mt1_danmaku_01_callback;/*(黄色 大玉弾。)*/
//	card.danmaku_callback[2] = NULL;/*(未使用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}

/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。(kankaku)
//	REG_09_REG1 	カウンタ。(2種類)
//	REG_0a_REG2 	弾色(交差弾)。
//	REG_0b_REG3 	弾角度(交差弾)。	弾角度(連弾)。
---------------------------------------------------------*/

local void spell_create_32_pache_metal_fatigue(OBJ *src)
{
	if ((SPELL_TIME_0128-1)==((REG_10_BOSS_SPELL_TIMER) ))/*(一番始め)*/
	{
		HATSUDAN_01_speed256			= t256(2.50);		/* 弾速 */
	//	HATSUDAN_02_speed_offset		= (0<<(2+8));/* x4倍 てすと*/
		HATSUDAN_02_speed_offset		= t256(3+0);/*(テスト)*/
		HATSUDAN_03_angle65536			= (0);/*(真下)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_OODAMA32_02_KIIRO);	/* [黄色 大玉弾] */
		HATSUDAN_06_n_way				= (8);							/* [8way] */		/* 発弾数 */
		HATSUDAN_07_div_angle65536		= (int)(65536/(8)); 	/* 分割角度(65536[360/360度]を 8 分割) */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		//
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
	}
}

/*---------------------------------------------------------
[パチェ] "　　火符「アグニシャイン上級」" No.20
	-------------------------------------------------------
---------------------------------------------------------*/
#if (0)/*(とりあえず)*/
local void spell_create_33_pache_agni_shine_2(OBJ *src)
{
}
#else
	#define spell_create_33_pache_agni_shine_2		spell_create_2e_pache_agni_shine_1
	#define spell_init_33_pache_agni_shine_2		spell_init_2e_pache_agni_shine_1
#endif


/*---------------------------------------------------------
[パチェ] "　　木符「シルフィホルン上級」" No.21
	-------------------------------------------------------
---------------------------------------------------------*/
#if (0)/*(とりあえず)*/
local void spell_create_34_pache_sylphy_horn_2(OBJ *src)
{
}
#else
	#define spell_create_34_pache_sylphy_horn_2 	spell_create_30_pache_sylphy_horn_1
	#define spell_init_34_pache_sylphy_horn_2		spell_init_30_pache_sylphy_horn_1
#endif


/*---------------------------------------------------------
[パチェ] "土符「レイジィトリリトン上級」" No.22
	-------------------------------------------------------
	てすと
---------------------------------------------------------*/
#if 0
local void spell_init_31_pache_rage_tririton_1(OBJ *src)
{
	card.danmaku_callback[1] = pache_rt1_danmaku_01_callback;/*(枠付き丸弾。交差弾。? 回り。)*/
//	card.danmaku_callback[2] = NULL;/*(未使用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}
#endif
#if (1)/*(とりあえず)*/
local void spell_create_35_pache_rage_tririton_2(OBJ *src)
{
	// 最初の発射 (64フレーム==約1秒)ばら撒く。
//	if (((64))<(REG_10_BOSS_SPELL_TIMER)) // 192== (64*3)
	{
		HATSUDAN_01_speed256			= (t256(2.00) );						/* 弾速 */
		HATSUDAN_02_speed_offset		= (t256(3));/*(テスト)*/
		HATSUDAN_03_angle65536			= (ra_nd() &(0x7fff)) + (65536/4)+(65536/2);/*(下半分)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_06_KI_IRO);//			/* [黄弾] */
	//	HATSUDAN_06_n_way				= (1);	/* [1way] */				/* 発弾数 */
	//	HATSUDAN_07_div_angle65536		= (int)(65536/(24));	/* 分割角度(65536[360/360度]を 19 分割) */
		hatudan_system_regist_single();
	}
}
#else
	#define spell_create_35_pache_rage_tririton_2	spell_create_31_pache_rage_tririton_1
//	#define spell_init_35_pache_rage_tririton_2 	spell_init_31_pache_rage_tririton_1
#endif

/*---------------------------------------------------------
[パチェ] "　火符「アグニレイディアンス」" No.23
	-------------------------------------------------------
	"　火符「アグニレイディアンス」" No.23 ==
		"　　火符「アグニシャイン上級」" No.20 +
		"赤大玉"
---------------------------------------------------------*/
/*(とりあえず)*/
local void spell_create_36_pache_agni_radiance(OBJ *src)
{
	if (0== (REG_10_BOSS_SPELL_TIMER & 0x01) ) /* 2回に1回なら撃つ */
	{
		HATSUDAN_01_speed256			= (t256(2.00) );						/* 弾速 */
		HATSUDAN_02_speed_offset		= (t256(4));/*(テスト)*/
		HATSUDAN_03_angle65536			= (ra_nd());
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 	= BULLET_OODAMA32_01_AKA;			/* [赤大玉弾] */
		hatudan_system_regist_single();
	}
	/*"超劣化アグニシャイン"*/
	if (0== (REG_10_BOSS_SPELL_TIMER & 0x01) ) /* 2回に1回なら撃つ */
	{
		HATSUDAN_02_speed_offset		= t256(1.5);/*(テスト)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_UROKO14_BASE + TAMA_IRO_01_AKA);			/* [赤鱗弾] */
		HATSUDAN_01_speed256			= (t256(1.0));		/* 弾速 */
		REG_0a_REG2 = ra_nd();
		REG_0b_REG3 = REG_0a_REG2 & 0xff;
		//
		HATSUDAN_03_angle65536			= (REG_0a_REG2);
		hatudan_system_regist_single();
	}
}


/*---------------------------------------------------------
[パチェ] "　　　水符「ベリーインレイク」" No.24
	-------------------------------------------------------
	仕様:
		"　水符「プリンセスウンディネ」" No.16
		の上級版だがこちらの方が簡単。
	-------------------------------------------------------
	仕様:
		LunaticはHardに比べて動きが激しい。
---------------------------------------------------------*/
#if (0)/*(とりあえず)*/
local void spell_create_37_pache_bury_in_lake(OBJ *src)
{
}
#else
	#define spell_create_37_pache_bury_in_lake		spell_create_2f_pache_princess_undine
#endif

/*---------------------------------------------------------
[パチェ] "　　　木符「グリーンストーム」" No.25
	-------------------------------------------------------
	シルフィホルン上級の上級版。
---------------------------------------------------------*/
#if (0)/*(とりあえず)*/
local void spell_create_38_pache_green_storm(OBJ *src)
{
}
#else
	#define spell_create_38_pache_green_storm		spell_create_30_pache_sylphy_horn_1
	#define spell_init_38_pache_green_storm 		spell_init_30_pache_sylphy_horn_1
#endif


/*---------------------------------------------------------
[パチェ] "　　土符「トリリトンシェイク」" No.26
	-------------------------------------------------------
	"　　土符「トリリトンシェイク」" No.26 ==
		"土符「レイジィトリリトン上級」" No.22 +
		"七方向大玉弾。自機狙い。"
---------------------------------------------------------*/
#if 0
local void spell_init_31_pache_rage_tririton_1(OBJ *src)
{
	card.danmaku_callback[1] = pache_rt1_danmaku_01_callback;/*(枠付き丸弾。交差弾。? 回り。)*/
//	card.danmaku_callback[2] = NULL;/*(未使用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}
#endif
local void spell_create_39_pache_tririton_shake(OBJ *src)
{
//	count_up_limit_NUM(REG_NUM_08_REG0, 192);// 	/*	繰り返しカウンタ。 */
	// 最初の発射 (64フレーム==約1秒)ばら撒く。
	if (((64))<(REG_10_BOSS_SPELL_TIMER)) // 192== (64*3)
	{
		HATSUDAN_01_speed256			= (t256(2.00) );						/* 弾速 */
		HATSUDAN_02_speed_offset		= (t256(3));/*(テスト)*/
		HATSUDAN_03_angle65536			= (ra_nd() &(0x7fff)) + (65536/4)+(65536/2);/*(下半分)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_06_KI_IRO);//			/* [黄弾] */
	//	HATSUDAN_06_n_way				= (1);	/* [1way] */				/* 発弾数 */
	//	HATSUDAN_07_div_angle65536		= (int)(65536/(24));	/* 分割角度(65536[360/360度]を 19 分割) */
		hatudan_system_regist_single();
	}
	/*"七方向大玉弾。自機狙い。"*/
	if (0== (REG_10_BOSS_SPELL_TIMER & 0xf) ) /* 16回に1回なら撃つ */
	{
		#if 1
	//	REG_02_DEST_X	= ((src->cx256));
	//	REG_03_DEST_Y	= ((src->cy256));
		calculate_jikinerai();/* 自機狙い角作成 */
	//	REG_0a_REG2 = HATSUDAN_03_angle65536;/* 自機狙い角 */
		#endif
		HATSUDAN_01_speed256			= (t256(2.0));		/* 弾速 */
	//	HATSUDAN_02_speed_offset		= t256(1.5);/*(テスト)*/
		HATSUDAN_02_speed_offset		= t256(5);/*(テスト)*/
	//	HATSUDAN_03_angle65536			= (REG_0a_REG2);/* 自機狙い角 */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)標準弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_OODAMA32_02_KIIRO);		/* [黄色 大玉弾] */
		HATSUDAN_06_n_way				= (7);	/* [7way] */				/* 発弾数 */
		HATSUDAN_07_div_angle65536		= (int)(65536/(7)); /* 分割角度(65536[360/360度]を 7 分割) */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
}


/*---------------------------------------------------------
[パチェ] "　　　金符「シルバードラゴン」" No.27
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
	交差弾。反時計回り。(0x0000)
	交差弾。時計回り。(0x0100)
---------------------------------------------------------*/
local void pache_mt2_danmaku_01_callback(OBJ *src)/* シルバードラゴン */
{
	if ((HATUDAN_ITI_NO_JIKAN-32) < src->jyumyou)/* 発弾エフェクト後から[64]カウント未満の弾 */
	{
	//	src->hatudan_register_speed65536	-= (t256(0.02)<<8); 		/* 弾速 */
		src->hatudan_register_speed65536	-= (t256(0.05)<<8); 		/* 弾速 */
		src->hatudan_register_speed65536	= psp_max( (0), (src->hatudan_register_speed65536) );
	}
	else
	if ((HATUDAN_ITI_NO_JIKAN-64) < src->jyumyou)/* 発弾エフェクト後から[64]...[128]カウント未満の弾 */
	{
		if (0==(ra_nd()&1))
		{
			src->rotationCCW1024 -= (13);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		}
		else
		{
			src->rotationCCW1024 += (777);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		}
		src->hatudan_register_speed65536	+= (1<<8);			/* 弾速 */
	}
	else
	if ((HATUDAN_ITI_NO_JIKAN-128) < src->jyumyou)/* 発弾エフェクト後から[64]...[128]カウント未満の弾 */
	{
	//	src->hatudan_register_speed65536	+= (t256(0.01)<<8); 		/* 弾速 */
		src->hatudan_register_speed65536	+= (1<<8);			/* 弾速 */
	}
	else	/* 発弾後 256 カウント以上は弾を消す。 */
//	if ((HATUDAN_ITI_NO_JIKAN-256) > src->jyumyou/*(寿命なので自動で減る)*/)/* 発弾エフェクト後から[256]カウント以上経過した弾 */
	{
		/* (通常弾へ変身する) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	}
//	danmaku_00_standard_angle_mover(src);/*(角度弾移動+画面外弾消し)*/
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
}

/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/

local void spell_init_3a_pache_silver_dragon(OBJ *src)
{
	card.danmaku_callback[1] = pache_mt2_danmaku_01_callback;/*(白弾。)*/
//	card.danmaku_callback[2] = NULL;/*(未使用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}

/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。(kankaku)
//	REG_09_REG1 	カウンタ。(2種類)
//	REG_0a_REG2 	弾色(交差弾)。
//	REG_0b_REG3 	弾角度(交差弾)。	弾角度(連弾)。
---------------------------------------------------------*/

local void spell_create_3a_pache_silver_dragon(OBJ *src)
{
	if (30==REG_10_BOSS_SPELL_TIMER)
	{
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_00_SIRO);			/* 白弾 */	/* [中丸白色弾は無いので] */
		HATSUDAN_06_n_way				= (8);							/* [8way] */		/* 発弾数 */
		HATSUDAN_07_div_angle65536		= (int)(65536/(8)); 	/* 分割角度(65536[360/360度]を 8 分割) */
		unsigned int jj;
		for (jj=0; jj<(5); jj++)/* 5層 */
		{
		//	HATSUDAN_02_speed_offset		= (jj<<(2+8));/* x4倍 てすと*/
			HATSUDAN_02_speed_offset		= (3+jj)<<8;/*(テスト)*/
			HATSUDAN_01_speed256			= (t256(1.00)+(jj<<7)); 		/* 弾速 */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
		//
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
	}
}


/*---------------------------------------------------------
	[パチェ] "火＆土符「ラーヴァクロムレク」" No.28
	-------------------------------------------------------
	"火＆土符「ラーヴァクロムレク」" No.28 ==
		"　　土符「レイジィトリリトン」" No.18 +
		"　　　　火符「アグニシャイン」" No.15
	-------------------------------------------------------

	使用レジスタ
	REG_08_REG0 	カウンタ。

---------------------------------------------------------*/
#if 0
local void spell_init_31_pache_rage_tririton_1(OBJ *src)
{
	card.danmaku_callback[1] = pache_rt1_danmaku_01_callback;/*(枠付き丸弾。交差弾。? 回り。)*/
	card.danmaku_callback[2] = NULL;/*(未使用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}
#endif
local void spell_create_3b_pache_lava_cromlech(OBJ *src)
{
//	count_up_limit_NUM(REG_NUM_08_REG0, 192);// 	/*	繰り返しカウンタ。 */
	// 最初の発射 (64フレーム==約1秒)ばら撒く。
	if (((64))<(REG_10_BOSS_SPELL_TIMER)) // 192== (64*3)
	{
		HATSUDAN_01_speed256			= (t256(2.0) ); 					/* 弾速 */
		HATSUDAN_02_speed_offset		= (t256(3));/*(テスト)*/
		HATSUDAN_03_angle65536			= (ra_nd() &(0x7fff)) + (65536/4)+(65536/2);/*(下半分)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_06_KI_IRO);//			/* [黄弾] */
		hatudan_system_regist_single();
	}
	/*"超劣化アグニシャイン"*/
	if (0== (REG_10_BOSS_SPELL_TIMER & 0x01) ) /* 2回に1回なら撃つ */
	{
		HATSUDAN_01_speed256			= (t256(1.0));		/* 弾速 */
		HATSUDAN_02_speed_offset		= t256(1.5);/*(テスト)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_UROKO14_BASE + TAMA_IRO_01_AKA);			/* [赤鱗弾] */
		REG_0a_REG2 = ra_nd();
		REG_0b_REG3 = REG_0a_REG2 & 0xff;
		//
		HATSUDAN_03_angle65536			= (REG_0a_REG2);
		hatudan_system_regist_single();
	}
}


/*---------------------------------------------------------
	[パチェ] "木＆火符「フォレストブレイズ」" No.29
	-------------------------------------------------------
---------------------------------------------------------*/
#if (0)/*(未作成)*/
local void spell_create_3c_pache_forest_blaze(OBJ *src)
{
}
#else
	#define spell_create_3c_pache_forest_blaze		spell_create_1f_koakuma
#endif


/*---------------------------------------------------------
	[パチェ] "　水＆木符「ウォーターエルフ」" No.30
	-------------------------------------------------------
	正直よくわからん。
	パチェっぽい？。
	-------------------------------------------------------
	うーん。難しくなってる。
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	角度。
	REG_09_REG1 	角度保存用。
---------------------------------------------------------*/
local void spell_create_3d_pache_water_elf(OBJ *src)
{
	/* 水符 自機狙い 24方向全方位、水色弾 BULLET_MARU10_04_MIZU_IRO */
	/* 木符 自機狙い 20方向全方位、緑色弾 BULLET_MARU10_05_MIDORI */
	/* 木符 自機狙い 10方向半周、青中玉弾 BULLET_OODAMA32_00_AOI */
//	sta tic int REG_08_REG0;
	REG_08_REG0 += (REG_10_BOSS_SPELL_TIMER<<4);
	if (0==((REG_10_BOSS_SPELL_TIMER)&(16-1)))
	{
	//	int REG_09_REG1;
		/* 自機狙い 角を作成 */
		REG_02_DEST_X	= ((src->cx256));
		REG_03_DEST_Y	= ((src->cy256));
		calculate_jikinerai();
		REG_09_REG1 = HATSUDAN_03_angle65536;/* hatudan_system_regist_katayori_n_way()は角度破壊するので、まず保存。 */
		REG_08_REG0 += (REG_09_REG1);
		//	HATSUDAN_01_speed256				= (t256(0.75) );		/* 弾速 */
		//	HATSUDAN_01_speed256				= (t256(1.50) );		/* 弾速 */
	//		HATSUDAN_01_speed256				= (t256(1.00) );		/* 弾速 */
			HATSUDAN_01_speed256				= (t256(1.00) );		/* 弾速 */
			HATSUDAN_02_speed_offset			= t256(3);/*(テスト)*/
		if (32>((REG_10_BOSS_SPELL_TIMER)&(128-1)))
		{
			/* 木符 自機狙い 10方向半周、青中玉弾 BULLET_OODAMA32_00_AOI */
			HATSUDAN_03_angle65536				= REG_09_REG1;/*[角度、設定(復旧)]*/
			//
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_OODAMA32_00_AOI);//				/* [青弾] */
			HATSUDAN_06_n_way					= (10); 	/* [10way] */					/* 発弾数 */
			HATSUDAN_07_div_angle65536			= (int)(65536/(20));	/* 分割角度(65536[360/360度]を 20 分割) */
			hatudan_system_regist_n_way();/* (r33-) */
		}
			HATSUDAN_02_speed_offset			= t256(1);/*(テスト)*/
		if (0x0a90&(REG_08_REG0))
		{
			/* 木符 自機狙い 20方向全方位、緑色弾 BULLET_MARU10_05_MIDORI */
			HATSUDAN_03_angle65536				= REG_09_REG1;/*[角度、復旧]*/
			HATSUDAN_05_bullet_obj_type 		= (BULLET_MARU10_BASE + TAMA_IRO_05_MIDORI);//				/* [緑弾] */
			HATSUDAN_06_n_way					= (20); 	/* [20way] */					/* 発弾数 */
			HATSUDAN_07_div_angle65536			= (int)(65536/(20));	/* 分割角度(65536[360/360度]を 20 分割) */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
		if (0x0670&(REG_08_REG0))
		{
			/* 水符 自機狙い 24方向全方位、水色弾 BULLET_MARU10_04_MIZU_IRO */
			HATSUDAN_03_angle65536				= REG_09_REG1;/*[角度、復旧]*/
			HATSUDAN_05_bullet_obj_type 		= (BULLET_MARU10_BASE + TAMA_IRO_04_MIZU_IRO);//				/* [水色弾] */
			HATSUDAN_06_n_way					= (24); 	/* [24way] */					/* 発弾数 */
			HATSUDAN_07_div_angle65536			= (int)(65536/(24));	/* 分割角度(65536[360/360度]を 24 分割) */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}


/*---------------------------------------------------------
	[パチェ] "金＆水符「マーキュリポイズン」" No.31
	-------------------------------------------------------
	あんま似てないけど、少しだけパチェっぽい。
---------------------------------------------------------*/

/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
	交差弾。反時計回り。(0x0000)
	交差弾。時計回り。(0x0100)
---------------------------------------------------------*/
local void pache_mp1_danmaku_01_callback(OBJ *src)/* マーキュリポイズン */
{
//	if (cnt < 80)/* 80未満は減速 */
	if ((HATUDAN_ITI_NO_JIKAN-64) < src->jyumyou)/* 発弾エフェクト後から[64]カウント未満の弾 */
	{
		src->hatudan_register_speed65536	-= (t256(0.02)<<8); 		/* 弾速 */
		src->hatudan_register_speed65536	= psp_max( (0), (src->hatudan_register_speed65536) );
	}
	else
//	if (cnt == 80)/* 80なら左右にぶれる。 */
//	if ((HATUDAN_ITI_NO_JIKAN-80) == src->jyumyou)/* 発弾エフェクト後から[80]カウント未満の弾 */
	if ((HATUDAN_ITI_NO_JIKAN-96) < src->jyumyou)/* 発弾エフェクト後から[64]...[96]カウント未満の弾 */
	{
		#if 0
		s32 ang = bullet_get_angle(eb);
		int add_angle;
		add_angle = ((A_ROUND65536)/4 + deg360to_rad(rand()%10));
		bullet_set_angle(eb, ang - add_angle );
		bullet_set_angle(eb, ang + add_angle );
		#else
		if (src->hatudan_register_spec_data & TAMA_SPEC_AO_MIZU_MERCURY_BIT)
		{
			src->rotationCCW1024 -= (1);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		}
		else
		{
			src->rotationCCW1024 += (1);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		}
		#endif
		src->hatudan_register_speed65536	+= (t256(0.01)<<8); 		/* 弾速 */
	}
	else	/* 81から 100未満は加速 */
//	if (cnt < 200)/* 81から 200未満は加速(但し100で弾強制消去) */
	if ((HATUDAN_ITI_NO_JIKAN-256) < src->jyumyou)/* 発弾エフェクト後から[96]...[256]カウント未満の弾 */
	{
		if (0==(ra_nd()&1))
		{
			goto test_aaa;
		}
	//	bullet_set_speed(eb, (spd256+t256(0.015) ) );/*(加速)*/
		if (src->hatudan_register_spec_data & TAMA_SPEC_AO_MIZU_MERCURY_BIT)
		{
			src->rotationCCW1024 -= (1);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		}
		else
		{
			src->rotationCCW1024 += (1);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		}
	//	src->hatudan_register_speed65536	+= (t256(0.01)<<8); 		/* 弾速 */
		src->hatudan_register_speed65536	-= (512);		/* 減速 */
	test_aaa:
		;
	}
	else	/* 発弾後 256 カウント以上は弾を消す。 */
//	if ((HATUDAN_ITI_NO_JIKAN-256) > src->jyumyou/*(寿命なので自動で減る)*/)/* 発弾エフェクト後から[256]カウント以上経過した弾 */
	{
		/* (通常弾へ変身する) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	}
//	danmaku_00_standard_angle_mover(src);/*(角度弾移動+画面外弾消し)*/
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
}

/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/

local void spell_init_3e_pache_mercury_poison(OBJ *src)
{
	card.danmaku_callback[1] = pache_mp1_danmaku_01_callback;/*(枠付き丸弾。交差弾。反時計回り/時計回り。)*/
//	card.danmaku_callback[2] = NULL;/*(未使用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}

/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。(kankaku)
	REG_09_REG1 	カウンタ。(2種類)
//	REG_0a_REG2 	弾色(交差弾)。
//	REG_0b_REG3 	弾角度(交差弾)。	弾角度(連弾)。

---------------------------------------------------------*/
#define MERCURY_POISON_KANKAKU_030 (30)
#define MERCURY_POISON_KANKAKU_015 (15)
local void spell_create_3e_pache_mercury_poison(OBJ *src)
{
	REG_08_REG0--;
	if (0>REG_08_REG0)
	{
		REG_08_REG0 = MERCURY_POISON_KANKAKU_030;
		#if 0
		if (0==cg_game_difficulty)
		{
			;
		}
//		if (1==cg_game_difficulty)
		else
		{
			if (enemy.hp < (enemy.maxHp*2/3))	{REG_08_REG0 = (MERCURY_POISON_KANKAKU_015);}
		}
		#endif
	//	static unsigned int REG_09_REG1 = (0);
		REG_09_REG1 ^= 1;
		if (0==REG_09_REG1)
		{
		//	br.BULLET_REGIST_color			= ((8)+(0*5));/*(水色?)*/
			HATSUDAN_05_bullet_obj_type = BULLET_MARU10_BASE + TAMA_IRO_03_AOI; 		/* [中丸青色弾] [中丸水色弾] */
		}
		else
		{
		//	br.BULLET_REGIST_color			= ((8)+(1*5));/*(黄色?)*/
			HATSUDAN_05_bullet_obj_type = BULLET_MARU10_BASE + TAMA_IRO_06_KI_IRO;		/* [中丸青色弾] [中丸水色弾] */
		}
		HATSUDAN_02_speed_offset		= t256(1);/*(テスト)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|((REG_09_REG1)<<8)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_06_n_way				= (18); 	/* [18way] */		/* 発弾数 */
		HATSUDAN_07_div_angle65536		= (int)(65536/(18));	/* 分割角度(65536[360/360度]を 18 分割) */
		{
			unsigned int random_angle_offset;
		//	random_angle_offset = deg360to_rad((rand() % 5)); // 910.222222222222222222222222222222
			random_angle_offset = (ra_nd()&0x0fff);// 0xe38 == 3640 == 65536/18
			HATSUDAN_03_angle65536		= random_angle_offset;	/* 基準角度 */
	//		HATSUDAN_01_speed256		= t256(3.0) + jj*t256(0.3);
			HATSUDAN_01_speed256		= t256(2.0);	/* 弾速 */
	//		bullet_set_till_time(100);/*(強制的に消える時間)*/
			hatudan_system_regist_katayori_n_way();/* (r33-) */
			HATSUDAN_03_angle65536		= random_angle_offset;	/* 基準角度 */
			HATSUDAN_01_speed256		= t256(2.3);	/* 弾速 */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
		//
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
	}
}

/*---------------------------------------------------------
	[パチェ] "土＆金符「エメラルドメガリス」" No.32
	-------------------------------------------------------
	テキトーに作ってみた。
	全然違うんだけど、パチェっぽいといえば、パチェっぽい。
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local void spell_init_3f_pache_emerald_megalith(OBJ *src)
{
	/* easyは1方向, hardは1方向, lunatic は3方向 */
	REG_0b_REG3 = (((3==REG_0f_GAME_DIFFICULTY)?3:1));
}
/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	?。
//	REG_09_REG1 	z
//	REG_0a_REG2 	z
	REG_0b_REG3 	難易度別定数。
---------------------------------------------------------*/
local void spell_create_3f_pache_emerald_megalith(OBJ *src)
{
	REG_08_REG0 += (REG_10_BOSS_SPELL_TIMER<<4);
	// ランダム緑枠弾
	if (0== ((REG_10_BOSS_SPELL_TIMER)&(4-1) ))
	{
	//	REG_08_REG0 = HATSUDAN_03_angle65536;
		/* 緑枠弾 */
		HATSUDAN_03_angle65536			= ((REG_08_REG0&(32768-1))-16384);
		//
	//	HATSUDAN_01_speed256			= (t256(0.75) );		/* 弾速 */
	//	HATSUDAN_01_speed256			= (t256(1.50) );		/* 弾速 */
		HATSUDAN_01_speed256			= (t256(0.75)+(REG_08_REG0&0x007f)+((REG_0f_GAME_DIFFICULTY)<<4) ); /* 弾速 */
		HATSUDAN_02_speed_offset		= ((REG_08_REG0>>4)&0x07ff);/*(テスト)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_05_MIDORI);//		/* [緑枠弾] */
		HATSUDAN_06_n_way				= (2);	/* [2way] */								/* 発弾数 */
		HATSUDAN_07_div_angle65536		= (int)(65536/(24)); /* 分割角度(65536[360/360度]を 24 分割) */
		hatudan_system_regist_n_way();/* (r33-) */
	}
	// ランダム直下落下、緑大玉弾
	if (0== ((REG_10_BOSS_SPELL_TIMER)&(16-1) ))
	{
		const u32 ra_nd32 = (ra_nd()/*&0xffff*/);
		REG_08_REG0 += ra_nd32;
		REG_02_DEST_X	= t256(16)+(ra_nd32&0xff00)+(ra_nd32&0x3f00);	/* 弾源x256 */
		REG_03_DEST_Y	+= -t256(32)+((ra_nd32>>8)&0x3f00); 			/* 弾源y256 */
		/* 自機狙い 3way 白laser弾 */
		/* 自機狙わない 16way(?)17way(???) 青丸弾 */
		HATSUDAN_01_speed256				= (t256(0.50) );			/* 弾速 */
		HATSUDAN_02_speed_offset			= t256(8);/*(テスト)*/
		HATSUDAN_03_angle65536				= (0);
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 		= (BULLET_OODAMA32_03_MIDORI);//				/* [緑大玉弾] */
		/* easyは1方向, hardは1方向, lunatic は3方向 */
		if (3==REG_0f_GAME_DIFFICULTY)
		HATSUDAN_06_n_way					= (REG_0b_REG3);	/* [1-3way] */		/* 発弾数 */
		HATSUDAN_07_div_angle65536			= (int)(65536/(19));	/* 分割角度(65536[360/360度]を 19 分割) */
		hatudan_system_regist_n_way();/* (r33-) */
	}
	// 効果音
	if (0== ((REG_10_BOSS_SPELL_TIMER)&(32-1) ))
	{
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
	}
}
