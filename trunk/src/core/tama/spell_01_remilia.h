
/*---------------------------------------------------------
 東方模倣風 〜 Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	レミリア っぽい弾幕を生成します。
---------------------------------------------------------*/

static void remilia_oyasumi_kougeki(SPRITE *src)
{
	#if (0)
//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
	bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
	#endif
//
	// ■  [大玉弾]
//	br.BULLET_REGIST_00_speed256			= (t256(3.5));						/* 弾速 */
//	br.BULLET_REGIST_00_speed256			= (t256(2.25)); 					/* 弾速 */
	br.BULLET_REGIST_00_speed256			= (t256(6.00)); 					/* 弾速 */
//	br.BULLET_REGIST_01_speed_offset		= -t256(1);/* 減速弾？ */
	br.BULLET_REGIST_01_speed_offset		= -t256(4);/* 減速弾？ */
	br.BULLET_REGIST_02_angle65536			= (16384)+(((src->boss_spell_timer)&0x40)<<8);/* 自機狙い角 */
	br.BULLET_REGIST_03_tama_data   		= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_OODAMA32_01_AKA;			/* [赤大玉弾] */
	tama_system_regist_single();
	// ■  [中玉弾]
	br.BULLET_REGIST_00_speed256			= (t256(4.00)); 					/* 弾速 */
	br.BULLET_REGIST_01_speed_offset		= -t256(2);/* 減速弾？ */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_MARU10_01_AKA; 			/* [赤大中弾] */
	u32 ii;
	for (ii=0; ii<8; ii++)
	{
		br.BULLET_REGIST_02_angle65536		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/8)-1)) - ((65536/16)-1);/* 自機狙い角 */
		tama_system_regist_single();
	}
	// ■  [小玉弾]
	br.BULLET_REGIST_00_speed256			= (t256(2.00)); 					/* 弾速 */
	br.BULLET_REGIST_01_speed_offset		= -t256(1);/* 減速弾？ */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_MINI8_01_AKA;				/* [赤大中弾] */
	for (ii=0; ii<(u32)(0x02<<(cg_game_difficulty)); ii++)/*(16)*/
	{
		br.BULLET_REGIST_02_angle65536		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/4)-1)) - ((65536/8)-1);/* 自機狙い角 */
		tama_system_regist_single();
	}
}


static void remilia_shot00(SPRITE *src)
{
	#if (1)
//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
	bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
	#endif
//
	// ■  [大玉弾]
//	br.BULLET_REGIST_00_speed256			= (t256(3.5));						/* 弾速 */
//	br.BULLET_REGIST_00_speed256			= (t256(2.25)); 					/* 弾速 */
	br.BULLET_REGIST_00_speed256			= (t256(6.00)); 					/* 弾速 */
//	br.BULLET_REGIST_01_speed_offset		= -t256(1);/* 減速弾？ */
	br.BULLET_REGIST_01_speed_offset		= -t256(4);/* 減速弾？ */
	br.BULLET_REGIST_02_angle65536			= src->tmp_angleCCW65536;/* 自機狙い角 */
	br.BULLET_REGIST_03_tama_data   		= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_OODAMA32_01_AKA;			/* [赤大玉弾] */
	tama_system_regist_single();
	// ■  [中玉弾]
	br.BULLET_REGIST_00_speed256			= (t256(4.00)); 					/* 弾速 */
	br.BULLET_REGIST_01_speed_offset		= -t256(2);/* 減速弾？ */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_MARU10_01_AKA; 			/* [赤大中弾] */
	u32 ii;
	for (ii=0; ii<8; ii++)
	{
		br.BULLET_REGIST_02_angle65536		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/8)-1)) - ((65536/16)-1);/* 自機狙い角 */
		tama_system_regist_single();
	}
	// ■  [小玉弾]
	br.BULLET_REGIST_00_speed256			= (t256(2.00)); 					/* 弾速 */
	br.BULLET_REGIST_01_speed_offset		= -t256(1);/* 減速弾？ */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_MINI8_01_AKA;				/* [赤大中弾] */
	for (ii=0; ii<(u32)(0x02<<(cg_game_difficulty)); ii++)/*(16)*/
	{
		br.BULLET_REGIST_02_angle65536		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/4)-1)) - ((65536/8)-1);/* 自機狙い角 */
		tama_system_regist_single();
	}
}


/*---------------------------------------------------------
	レミリア 通常攻撃2 っぽいもの
	-------------------------------------------------------
	1秒撃ったら、3秒休む(移動)。

---- ---- ---- ----
0000 0011 1111 1111
---------------------------------------------------------*/

static void spell_create_1f_remilia_kougeki2(SPRITE *src)
{
	if (0x80==((src->boss_spell_timer)&0x80))/* 後半 */
	{
	//	if (0xc0==((src->boss_spell_timer)&0xc0))
		if (0xc0==((src->boss_spell_timer)&0xc7))/* ８回に一回 */
		{
			#if 0
			/* 撃つターン 8弾 */
			if (0x0000==((src->boss_spell_timer)&0x30))
			{
				/* 自機狙い撃ち */
				remilia_shot00(src);
			}
			else
			if (0x0100==((src->boss_spell_timer)&0x30))
			{
				/* CW 撃ち */
				remilia_shot00(src);
			}
			else
			if (0x0200==((src->boss_spell_timer)&0x30))
			{
				/* CCW 撃ち */
				remilia_shot00(src);
			}
			else
		//	if (0x0300==((src->boss_spell_timer)&0x30))
			{
			//	/* 一気撃ち */
				/* 自機狙い撃ち */
				remilia_shot00(src);
			}
			#endif
			if (0==(((src->boss_spell_timer)+0x100)&0x200))
			{
				tmp_angleCCW65536_jiki_nerai(obj_send1);/* 自機狙い角作成 */
				src->tmp_angleCCW65536		= obj_send1->tmp_angleCCW65536;/* 自機狙い角 */
			}
			else
			{
				src->tmp_angleCCW65536		+= (((src->boss_spell_timer)&0x20)<<5)-( ((65536/24)-1));/* */
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
		if ( 0==((src->boss_spell_timer) & aaa[(cg_game_difficulty)] ))
		{
			remilia_oyasumi_kougeki(src);
		}
	}
}

/*---------------------------------------------------------
	レミリア
	-------------------------------------------------------


---------------------------------------------------------*/

static void spell_create_28_remilia_tama_oki(SPRITE *src)
{
	if (50==((src->boss_spell_timer) ))
	{
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_sakuya_doll_02_tama_oki(src);/* 珠置き追加 */
	}
}
