
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	大妖精のカードを定義します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	紅2面中-ボス 大妖精 通常攻撃1(1/3)
	-------------------------------------------------------
緑クナイ弾
下から始めて、時計回り、2段づつ
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_1a_dai_yousei_midori)
{
//	if (64==(src->boss_spe ll_timer))
	{
		REG_09_REG1 = 0;
		REG_0a_REG2 = 0;
	}
}
/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。
	REG_09_REG1 	aaa_tmp_angleCCW65536
	REG_0a_REG2 	bbb
	REG_0b_REG3 	i (難易度別に残り回数を設定。)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_1a_dai_yousei_midori)
{
//	if ((64-(48))<((src->boss_spe ll_timer)))
	count_up_limit_NUM(REG_NUM_08_REG0, 64);
	if (16<(REG_08_REG0))
	{
	//	if (0x00==((src->boss_spe ll_timer)&(2-1)))/*(2回に1回)*/
		{
				HATSUDAN_02_speed_offset		= t256(1);/*(テスト)*/
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_KUNAI12_BASE + TAMA_IRO_05_MIDORI);	/* [ 弾] */
		//	u32 i;
		//	for (i=0; i<(1+(unsigned)REG_0f_GAME_DIFFICULTY); i++)/*(2段づつ)*/
			REG_0b_REG3 = REG_0f_GAME_DIFFICULTY;
		loop:
			{
				HATSUDAN_01_speed256			= (t256(0.6)+(REG_0a_REG2<<2)+(REG_0b_REG3<<6));			/* 弾速 */
				HATSUDAN_03_angle65536			= (REG_09_REG1);
				hatudan_system_regist_single();
			}
			REG_0b_REG3--;/*(残り回数減らす。)*/
			if (0 < REG_0b_REG3)/*(残り回数があればループ。)*/
		//	if (0 != REG_0b_REG3)/*(残り回数があればループ。)*/
			{
				goto loop;
			}
			REG_09_REG1 -= (65536/48);
			REG_0a_REG2++;
		}
	}
}


/*---------------------------------------------------------
	紅2面中-ボス 大妖精 通常攻撃1(2/3)
	-------------------------------------------------------
赤クナイ弾
下から始めて、反時計回り、2段づつ
メモ:Lunaticも一応同じ48方向弾。(4ヶ所から出て2段?3段に整列?)
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_1b_dai_yousei_aka)
{
//	if (64==(src->boss_spe ll_timer))
	{
		REG_09_REG1 = 0;
		REG_0a_REG2 = 0;
	}
}
/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。
	REG_09_REG1 	aaa_tmp_angleCCW65536
	REG_0a_REG2 	bbb
	REG_0b_REG3 	i (難易度別に残り回数を設定。)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_1b_dai_yousei_aka)
{
//	if ((64-(48))<((src->boss_spe ll_timer)))
	count_up_limit_NUM(REG_NUM_08_REG0, 64);
	if (16<(REG_08_REG0))
	{
	//	if (0x00==((src->boss_spe ll_timer)&(2-1)))/*(2回に1回)*/
		{
				HATSUDAN_02_speed_offset		= t256(1);/*(テスト)*/
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);	/* [ 弾] */
		//	u32 i;
		//	for (i=0; i<(1+(unsigned)REG_0f_GAME_DIFFICULTY); i++)/*(2段づつ)*/
			REG_0b_REG3 = REG_0f_GAME_DIFFICULTY;
		loop:
			{
				HATSUDAN_01_speed256			= (t256(0.6)+(REG_0a_REG2<<2)+(REG_0b_REG3<<6));			/* 弾速 */
				HATSUDAN_03_angle65536			= (REG_09_REG1);
				hatudan_system_regist_single();
			}
			REG_0b_REG3--;/*(残り回数減らす。)*/
			if (0 < REG_0b_REG3)/*(残り回数があればループ。)*/
		//	if (0 != REG_0b_REG3)/*(残り回数があればループ。)*/
			{
				goto loop;
			}
			REG_09_REG1 += (65536/48);
			REG_0a_REG2++;
		}
	}
}

/*---------------------------------------------------------
	紅4面中-ボス 小悪魔 通常攻撃1
	-------------------------------------------------------
	紫大玉弾。8方向。1/24ずつ反時計回りに回転。
	青クナイ弾。バラマキ。
	-------------------------------------------------------
	紫大玉弾。は無いので代わりに青大玉弾で。
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。
	REG_09_REG1 	角度1調整用。
	REG_0a_REG2 	乱数。
	REG_0b_REG3 	乱数加算角(テキトー)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_1f_koakuma)
{
//	if (64==(REG_10_BOSS_SPELL_TIMER)) /* 始めに初期化 */
	count_up_limit_NUM(REG_NUM_08_REG0, 192);// 	/*	繰り返しカウンタ。 */
	if (1==REG_08_REG0) /* 始めに初期化 */
	{
		REG_09_REG1 = 0;
		REG_0a_REG2 = 0;
		REG_0b_REG3 = 0;
		#if (1)
		AUDIO_18_voice_number	= VOICE17_BOSS_TAME01;	//	"ボス溜め1",		/* r	うぃーん音 */
		AUDIO_19_voice_truck	= TRACK03_IVENT_DAN;
		cpu_voice_play();
		#endif
	}
	if (0== (REG_08_REG0 & 0x0f) ) /* 16回に1回なら撃つ */
	{
		HATSUDAN_01_speed256			= (t256(1.0));			/* 弾速 */
	//	HATSUDAN_02_speed_offset		= t256(1);/*(テスト)*/
		HATSUDAN_02_speed_offset		= t256(5);/*(テスト)*/
		HATSUDAN_03_angle65536			= (REG_09_REG1);
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_OODAMA32_00_AOI); 	/* [青大玉弾] 紫無いから */
		HATSUDAN_06_n_way				= (8);							/* 発弾数 */
		HATSUDAN_07_div_angle65536		= (int)(65536/(8)); 			/* 分割角度(65536[360/360度]を 8 分割) */	/* 1周をn分割した角度 */
		hatudan_system_regist_katayori_n_way();
		REG_09_REG1 += ((65536/24)-256);	/* 平均(1周/24)を足す。(+-256乱数を足す) */
		REG_09_REG1 += (REG_0b_REG3);		/* 乱数加算角 */
	}
	if (0== (REG_08_REG0 & 0x01) ) /* 2回に1回なら撃つ */
	{
		HATSUDAN_02_speed_offset		= t256(1.5);/*(テスト)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_KUNAI12_BASE + TAMA_IRO_03_AOI);			/* [白米弾] */
		HATSUDAN_01_speed256			= (t256(1.0));		/* 弾速 */
		REG_0a_REG2 = ra_nd();
		REG_0b_REG3 = REG_0a_REG2 & 0x01ff;/*(0 ... 511)*/
		//
		HATSUDAN_03_angle65536			= (REG_0a_REG2);
		hatudan_system_regist_single();
	}
}
