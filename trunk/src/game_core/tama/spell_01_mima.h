
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	魅魔のカードを定義します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	魅魔様の向きによって、発弾位置を修正する。
	-------------------------------------------------------
	発弾位置は魅魔様中心位置からの差分なので、
	上に12[pixel]
	右に22[pixel]
	左に22[pixel]
	とする。
	パターン0-3の場合、右向き(+22)。
	パターン4-7の場合、左向き(-22)。

    左に22[pixel]    右に22[pixel]
    +--------- 50 --------+
    |                     |
    | ()               () | -- ---(発弾位置)
    |  |  -22  |  22   |  |
    |          |          | -12 (上に12[pixel])
    |          |          |
    50         + -------  | -- ---(魅魔様中心位置)
    |                     |
    |                     |
    |                     |
    |                     |
    |                     |
    +--------- 50 --------+
    魅魔様は 50x50[pixel]とする。
---------------------------------------------------------*/

local void mima_set_REG_DEST_XY(OBJ *src)
{
	/* 弾源 発弾 位置を修正 */
	REG_02_DEST_X += ((0==(src->obj_type_set&4))?(t256(22)):-(t256(22)));/* 右移動中は右側から撃つ */
	REG_03_DEST_Y -= t256(12);
}


/*---------------------------------------------------------
	魅魔、花てすとカード
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。
	REG_09_REG1 	adj_hari65536	開始地点
---------------------------------------------------------*/
/*---------------------------------------------------------
	#001 角度弾の移動を行う(通常弾用)
---------------------------------------------------------*/

static void mima_danmaku_01_callback(OBJ *src)
{
	#if (1)
	if ((HATUDAN_ITI_NO_JIKAN-64) < src->jyumyou)/* 発弾エフェクト時は無効 */
	{
		return;
	}
	#endif
	#if (1)
	if ((HATUDAN_ITI_NO_JIKAN-512) == src->jyumyou)/* 発弾エフェクト後から512カウント経過した弾 */
	{
		/* (通常弾へ変身する) */
		src->hatudan_register_spec_data 	= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
	}
	#endif
	//
//	danmaku_00_standard_angle_mover(src);/*(角度弾移動+画面外弾消し)*/
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
}
local void mima_boss01_nway_fire(OBJ *src)
{
	REG_02_DEST_X	= ((src->cx256));
	REG_03_DEST_Y	= ((src->cy256));
	calculate_jikinerai();/* 自機狙い角作成 */
	REG_09_REG1 = (HATSUDAN_03_angle65536);
//
	const int speed256 = (/*0x200*/0x100)+(ra_nd()&0xff)-((3-(REG_0f_GAME_DIFFICULTY))<<6);/* 難易度別で速度低下 */
	mima_set_REG_DEST_XY(src);	/* 向きで撃つ位置を変える。 */
//
//	HATSUDAN_02_speed_offset			= t256(1/*0*/);/*(テスト)*/
	HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
	HATSUDAN_05_bullet_obj_type 		= BULLET_HARI32_00_AOI + ((REG_10_BOSS_SPELL_TIMER)>>5);	/* [水色針弾] */	/*hari_no_iro*/
//
	int ii65536;
	for (ii65536=(REG_09_REG1/*0*/); ii65536<(REG_09_REG1+65536-(20*64)); ii65536+=(65536/5) )
	{
		int jj1024;
		int kk65536;
		kk65536 = (0);
		for (jj1024 = (0); jj1024<(1024/2); jj1024 += (32) )
		{
			{
				int sin_value_t256; 		// sin_value_t256 = 0;
				int cos_value_t256; 		// cos_value_t256 = 0;
				int256_sincos1024( (((jj1024)/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
				HATSUDAN_01_speed256		= (int)(speed256 + ((sin_value_t256))); 	/* 弾速 */
			}
			#if 0
		//	HATSUDAN_02_speed_offset	= -t256(HATSUDAN_01_speed256>>(6));/* (魅魔様lunaticで)これでも良い気もするけど。 */
			HATSUDAN_02_speed_offset	= -(HATSUDAN_01_speed256<<(8-6));/* (魅魔様lunaticで)これでも良い気もするけど。 */
			#endif
		//	HATSUDAN_02_speed_offset	= -t256(HATSUDAN_01_speed256>>(7));/* (パチェがあるから)少し簡単にしとく。 */
			HATSUDAN_02_speed_offset	= -(HATSUDAN_01_speed256<<(8-7));/* (パチェがあるから)少し簡単にしとく。 */
			HATSUDAN_03_angle65536		= ((ii65536)+(kk65536));				/* 角度 */
			hatudan_system_regist_single();
			kk65536 += ((65536/16)/5);
		}
	}
}

/*---------------------------------------------------------
	魅魔、花てすとカード
	-------------------------------------------------------
	封魔禄では4方向バラマキっぽいのを撃ってくるが、
	アレンジして5方向。花映塚っぽい奴に。
	-------------------------------------------------------
	テキトー
-dd 1 0000 0x10 == (xxx)&0x1f
-11 1 0000 luna
-10 1 0000 luna hard
-01 1 0000 luna hard norm
-00 1 0000 luna hard norm easy
---------------------------------------------------------*/
local void spell_init_0c_hana_test(OBJ *src)
{
	card.danmaku_callback[1] = mima_danmaku_01_callback;/*(。)*/
//	card.danmaku_callback[2] = NULL;/*(0)*/
//	card.danmaku_callback[3] = NULL;/*(0)*/
}

local void spell_create_0c_hana_test(OBJ *src)
{
//	if (0x40==((REG_10_BOSS_SPELL_TIMER)&0xcf))/* 4回 */
//	if (0x10==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* (16回に1回)(128なら計8回) */
	if (0x10==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* (16回に1回)(128なら計4回) */
	{
		if (((unsigned)REG_0f_GAME_DIFFICULTY) >= (((unsigned)REG_10_BOSS_SPELL_TIMER)>>5))
		{
			mima_boss01_nway_fire(src);
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		//	voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);/* テキトー */
			bullet_play_04_auto(VOICE11_BOSS_KIRARIN);
			#endif
		}
	}
}

/*---------------------------------------------------------
	魅魔、接近カード
	-------------------------------------------------------
	ここで下方うねり4方向、重力弾っぽい奴(予定)
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。
	REG_0e_REG6 	src_shot_angle65536 開始地点
---------------------------------------------------------*/
local void spell_create_0d_mima_sekkin(OBJ *src)
{
//	if (0x40==((REG_10_BOSS_SPELL_TIMER)&0xcf))/* 4回 */
//	if (0x10==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* (32回に1回)(128なら計 回) */
	if (0x04==((REG_10_BOSS_SPELL_TIMER)&0x07))/* (8回に1回) */
	{
	//	if (difficulty >= ((REG_10_BOSS_SPELL_TIMER)>>5))
		{
		//	REG_0e_REG6 -= (16);		/* cv1024r(10)*/
		//	REG_0e_REG6 -= (16*64); 		/* cv1024r(10)*/
			REG_0e_REG6 -= (1820);		/* cv1024r(10)*/	// 1820.44444444444444444444444444444 == 65536/36
		//	REG_0e_REG6 = ((src->shot_angle65536));/*(1024to65536)*/
		// (魅魔様専用品を使う。標準のではダメ) set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
			mima_set_REG_DEST_XY(src);	/* 向きで撃つ位置を変える。 */
			shot_common_gin_tama(src);/*(共通部分)*/
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		//	voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);/* テキトー */
			bullet_play_04_auto(VOICE11_BOSS_KIRARIN);
			#endif
		}
	}
}

#if 0
/*---------------------------------------------------------
	魅魔、トゲカード(秋 静葉、葉符「狂いの落葉」っぽい？)
	-------------------------------------------------------
	テキトー
	[0] f e d
	[c] b a 9
	[8] 7 6 5
	[4] 3 2 1
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。
	REG_09_REG1 	kaiten_aaa
	REG_0a_REG2 	kaiten_bbb
---------------------------------------------------------*/
local void spell_create_24_mima_toge(OBJ *src)
{
	if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x03))
	{
		REG_09_REG1 -= (10);
		HATSUDAN_02_speed_offset		= t256(1/*0*/);/*(テスト)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_HARI32_00_AOI);				/* [水色針弾] */
		//
		if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			HATSUDAN_01_speed256		= (t256(2.0) ); 	/* 弾速 */
			HATSUDAN_06_n_way			= (1);									/* [1way] */
			HATSUDAN_07_div_angle65536	= (int)(65536/24);						/* 分割角度 */
		}
		else
		if (0x0c==((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			HATSUDAN_01_speed256		= (t256(1.75) );	/* 弾速 */
			HATSUDAN_06_n_way			= (2);									/* [2way] */
			HATSUDAN_07_div_angle65536	= (int)(65536/24);						/* 分割角度 */
		}
		else
		if (0x08==((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			HATSUDAN_01_speed256		= (t256(1.5) ); 	/* 弾速 */
			HATSUDAN_06_n_way				= (2);									/* [2way] */
			HATSUDAN_07_div_angle65536		= (int)(65536/12);						/* 分割角度 */
		//	REG_0a_REG2 -= ((int)(65536/48)/*+(int)(65536/24)*/);
			REG_0a_REG2 -= ((int)(65536/24));
		}
		//
	//	REG_0a_REG2 -= ((int)(65536/48)/*+(int)(65536/24)*/);
		REG_0a_REG2 -= ((int)(65536/48));
		mask65536(REG_0a_REG2);
		HATSUDAN_03_angle65536				= (((REG_09_REG1+REG_0a_REG2))&(65536-1));	/* 角度 */
		//
		if (0x04!=((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
	}
	if ((SPELL_TIME_0640-1)==((REG_10_BOSS_SPELL_TIMER) ))
	{
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE17_BOSS_TAME01);
		#endif
	}
}
#endif

#if 1
/*---------------------------------------------------------
	魅魔、トゲカード(秋 静葉、葉符「狂いの落葉」っぽい？)
	-------------------------------------------------------
	テキトー
	f e d [c] 1100
	b a 9 [8] 1000
	7 6 5 [4] 0100
	3 2 1 [0] 0000
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。
	REG_09_REG1 	kaiten_aaa
	REG_0a_REG2 	kakudo_111
---------------------------------------------------------*/
local void spell_create_24_mima_toge(OBJ *src)
{
	if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x33))
	{
		if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x3f))
		{	/* ちょっとアリス風味 */
			REG_0a_REG2 += (7);
			{
				int sin_value_t256; 		// sin_value_t256 = 0;
				int cos_value_t256; 		// cos_value_t256 = 0;
				int256_sincos1024( (((REG_0a_REG2)/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
				REG_09_REG1 -= (( (sin_value_t256))<<6);
			}
			#if (1)
			voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);
		//	bullet_play_04_auto(VOICE11_BOSS_KIRARIN);
			#endif
		}
		HATSUDAN_01_speed256				= (t256(1.5));		/* 弾速 */
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		HATSUDAN_05_bullet_obj_type 		= (BULLET_HARI32_00_AOI);				/* [水色針弾] */
		HATSUDAN_06_n_way					= (5);									/* [5way] */
		HATSUDAN_07_div_angle65536			= (int)(65536/5);						/* 分割角度 */
		//
		unsigned int aaa = ((REG_10_BOSS_SPELL_TIMER)&0x0f);
		unsigned int bbb = ((0x0c-aaa)<<8);
		HATSUDAN_02_speed_offset			= (t256(1)+(aaa<<8));/*(テスト)*/
		//
		HATSUDAN_03_angle65536				= (((REG_09_REG1+bbb))&(65536-1));		/* 角度 */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		HATSUDAN_03_angle65536				= (((REG_09_REG1-bbb))&(65536-1));		/* 角度 */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
	}
}
#endif
