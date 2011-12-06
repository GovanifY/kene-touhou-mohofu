
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ののじっぽいカードを生成しようと思ったけど無理でした。
---------------------------------------------------------*/

/*---------------------------------------------------------
	紅2面中-ボス 大妖精 通常攻撃1(1/3)
	-------------------------------------------------------
	緑クナイ弾
	下から始めて、時計回り、2段づつ
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。
	REG_09_REG1 	角度調整用。
	REG_0a_REG2 	速度調整用。
	REG_0b_REG3 	加算角(テキトー)
	REG_0c_REG4 	加算角調整値(seed)を保持。(テキトー)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_1e_momiji_no_nonoji)
{
//	if (64==(REG_10_BOSS_SPELL_TIMER)) /* 始めに初期化 */
	count_up_limit_NUM(REG_NUM_08_REG0, 192);// 	/*	繰り返しカウンタ。 */
	if (1==REG_08_REG0) /* 始めに初期化 */
	{
		REG_09_REG1 = 0;
		REG_0a_REG2 = 0;
	}
//	if ((64-(48))<((REG_10_BOSS_SPELL_TIMER)))
	if (48 > REG_08_REG0) /* 48以下なら撃つ */
	{
	//	if (0x00==((REG_10_BOSS_SPELL_TIMER)&(2-1)))/*(2回に1回)*/
		{
				HATSUDAN_02_speed_offset		= t256(1);/*(テスト)*/
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_OODAMA32_00_AOI); 	/* [青大玉弾] 白いの無いから */
			u32 i;
			for (i=0; i<(1+(unsigned)REG_0f_GAME_DIFFICULTY); i++)/*(2段づつ)*/
			{
			//	HATSUDAN_01_speed256			= (t256(0.6)+(REG_0a_REG2<<2)+(i<<6));			/* 弾速 */
				HATSUDAN_01_speed256			= (t256(0.6)+(REG_0a_REG2	)+(i<<6));			/* 弾速 */
				HATSUDAN_03_angle65536			= (REG_09_REG1);
				hatudan_system_regist_single();
			}
			REG_09_REG1 += (65536/48);	/* 下から撃ち始める。 */
			REG_0a_REG2++;
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
			#endif
		}
	}
	if (192 > REG_08_REG0) /* 192以下なら撃つ */
	{
		if (96 < REG_08_REG0) /* 96以上なら撃つ */
		{
			HATSUDAN_02_speed_offset		= t256(1);/*(テスト)*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_KOME_BASE + TAMA_IRO_00_SIRO);			/* [白米弾] */
			HATSUDAN_01_speed256			= (t256(0.6)+(REG_0a_REG2));		/* 弾速 */
			HATSUDAN_03_angle65536			= (REG_09_REG1);
			REG_0c_REG4 += REG_0b_REG3; /* テキトー? */
			REG_0b_REG3 = REG_0c_REG4;
			REG_0b_REG3 = (REG_0b_REG3 >> (5)); 	/*( /=32 こういうカード命令が必要。)*/
		//	count_up_limit_NUM(REG_NUM_0b_REG3, 1365);//	/* 1365.333333 == (65536/48) 最大加算角度制限してみる。 */
			REG_09_REG1 -= 1365; /* (192-96)/48 ==2 で２周回る筈。テキトー? */
			REG_09_REG1 -= REG_0b_REG3; /* テキトー? */
			hatudan_system_regist_single();
		}
	}
}
