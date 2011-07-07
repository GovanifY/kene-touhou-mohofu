
/*---------------------------------------------------------
 東方模倣風 ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	美鈴の弾幕を生成します。
---------------------------------------------------------*/


/*---------------------------------------------------------
	紅3面中-ボス 紅美鈴
	通常攻撃1(1/2)
	16方位偏り全方位ばら撒き弾
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	紅3面中-ボス 紅美鈴
	華符「芳華絢爛」にちょっとだけ似た弾幕(予定)
	時計回り、反時計回りの6way螺旋弾(黄色)		1回毎に(1024/(6*8))[1024/1024度]回転
	48方位全方位弾(赤色)						8回毎に発弾
	-------------------------------------------------------
	芳華絢爛
	解像度とか速度とかの要因で、そのままでは pspで難しい気もする。
	Lunatic はこのままで、Hard 以下は細工して易しくする。
---------------------------------------------------------*/
#if 1
static void spell_create_0a_houka_kenran(SPRITE *src)
{
//	if ((0x10)==((src->boss_base_spell_time_out)&0x1f))/* (16回に1回)(128なら計8回) */
	if ((0x40)==((src->boss_base_spell_time_out)&0x7f))/* (16回に1回)(128なら計8回) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);/* 本家はきこきこ音 */
		#endif
		set_sub_jikinerai(src);/* 1:いちいち作成するっぽい。 */
		br.BULLET_REGIST_00_speed256				= (t256(1.75)); 									/* 弾速 */	/* 3.5 2.5 2.0 */
		br.BULLET_REGIST_01_speed_offset			= t256(0);/*(テスト)*/
		br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
		br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_01_AKA;								/* [赤色米弾] */
	//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)標準弾 */
		br.BULLET_REGIST_06_n_way					= spell_bunkatsu_nums_table[b_HOUGA_08_RED_NUMS+(cg_game_difficulty)];			/*(48)*/				/* [48way] */	/* 発弾数 */
		br.BULLET_REGIST_07_div_angle65536			= spell_nan_ido_table[HOUGA_12_RED_DIV_ANGLE+(cg_game_difficulty)]; 	/*(int)(1024/(48))*/	/* 分割角度(1024[360/360度]を 48 分割) */	/* 1周をn分割した角度 */
		tama_system_regist_katayori_n_way();/* (r33-) */
	}
//	if ((0x02)==((src->boss_base_spell_time_out)&0x03))/* (2回に1回)(8回毎に発弾) */
	if ((0x08)==((src->boss_base_spell_time_out)&0x0f))/* (2回に1回)(8回毎に発弾) */
	{
		obj_send1->cx256							= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
		obj_send1->cy256							= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
		static int aaa_angle65536;
		br.BULLET_REGIST_00_speed256				= (t256(1.75)); 										/* 弾速 */		/*3.5 2.5 2.0*/
		br.BULLET_REGIST_01_speed_offset			= t256(0);/*(テスト)*/
		br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
		br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_06_KI_IRO;								/* [黄色米弾] */
	//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)標準弾 */
		br.BULLET_REGIST_06_n_way					= spell_bunkatsu_nums_table[b_HOUGA_00_YEL_NUMS+(cg_game_difficulty)];				/*(6)*/ 				/* [1way] */	/* 発弾数 */
		br.BULLET_REGIST_07_div_angle65536			= spell_nan_ido_table[HOUGA_04_YEL_DIV_ANGLE+(cg_game_difficulty)]; 	/*(int)(1024/(6))*/ 	/* 分割角度(1024[360/360度]を 6 分割) */	/* 1周をn分割した角度 */
		// 順回り(下CCWだから、左回りCCW)
		br.BULLET_REGIST_02_angle65536				= ((/*0+*/(aaa_angle65536))&(65536-1)); 				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		tama_system_regist_katayori_n_way();/* (r33-) */
		// 逆回り(下CCWだから、右回りCW)
		br.BULLET_REGIST_02_angle65536				= ((65536-(aaa_angle65536))&(65536-1)); 				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		tama_system_regist_katayori_n_way();/* (r33-) */
		// 回転量
		aaa_angle65536 += spell_nan_ido_table[HOUGA_16_YEL_ROTATE_ANGLE+(cg_game_difficulty)];			/*(1024/(6*8))*/		/* 角度(1024[360/360度]を 48分割) */
	}
}
#endif

#if 0
static const s8 step_tbl[(4+4)] =
{
	(1024/24),	/* easy */
	(1024/28),	/* normal */
	(1024/30),	/* hard */
	(1024/32),	/* lunatic */
//
	(24),	/* easy */
	(28),	/* normal */
	(30),	/* hard */
	(32),	/* lunatic */
};
#endif

/*---------------------------------------------------------
	文に美鈴っぽい弾幕撃たせてみるテスト(の筈だったんだけど、
	何故か当初の方向とは違う方向に……)
	-------------------------------------------------------
---------------------------------------------------------*/
#if 1

//extern void exchange_damnaku_check_type(void);
/*---------------------------------------------------------
normal? 紅クナイ	24方向?
hard	紅クナイ	35方向(36方向では無い)

---------------------------------------------------------*/

static void s_change_meirin_yajirusi_one(SPRITE *h)
{
	/* 現在の弾座標を、基点座標にする。 */
	h->tx256 = h->cx256;/*fps_factor*/
	h->ty256 = h->cy256;/*fps_factor*/
	//
//	h->type 						= (BULLET_KUNAI12_00_AOI+1);/* 青弾→赤弾に変身 */
//	h->tama_system_speed256 					= t256(0.5);		/* 速度 */	/* 初速(打ち出し速度) */
//	h->tama_system_speed256 					= t256(0.0)+((h->tama_system_radius256)>>5);	/* 半径が大きい程、初速が速い */	/* 初速(打ち出し速度) */
//	h->tama_system_speed256 					= t256(0.0)+((h->tama_system_radius256)>>6);	/* 半径が大きい程、初速が速い */	/* 初速(打ち出し速度) */
	h->tama_system_speed65536					= ((t256(0.0)+((h->tama_system_radius256)>>6))<<8); /* 半径が大きい程、初速が速い */	/* 初速(打ち出し速度) */
	h->tama_system_radius256					= t256(0);/* 半径 */
	h->tama_system_tra65536 					= t256(1.0);		/* 調整加速弾 */
	const int ao_aka_tbl[(2)]	=
	{
	//	 (1024/2)+(1024/32),/* 180/360ちょい回転 */ 	/* 青 */
	//	-(1024/2)-(1024/32),/* 180/360ちょい回転 */ 	/* 赤 */
		 (1024/2)+(1024/24),/* 180/360ちょい回転 */ 	/* 青 */
		-(1024/2)-(1024/24),/* 180/360ちょい回転 */ 	/* 赤 */
	};
//	h->rotationCCW1024				+= (1024/2)+(1024/16);/* 180/360ちょい回転 */
//	h->rotationCCW1024				+= (1024/2)+(1024/32);/* 180/360ちょい回転 */
	h->rotationCCW1024				+= ao_aka_tbl[(((br.BULLET_REGIST_04_bullet_obj_type)&2)>>1)];/* 180/360ちょい回転 */
	mask1024(h->rotationCCW1024);
}
//global void exchange_damnaku_check_type(void)
static void s_exchange_damnaku_check_type(void)
{
	int check_type;
	check_type = br.BULLET_REGIST_04_bullet_obj_type;/* 調べるタイプを受け取る */
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
		if (check_type == (s->type) )	/* 矢印の青弾か赤弾なら */
		{
			s_change_meirin_yajirusi_one(s);
		}
	}
}

static void spell_create_12_aya_merin_test(SPRITE *src)
{
/*
time
f	1111	青1 			[発弾]
e	1110
d	1101			赤1 	[発弾]
c	1100
b	1011	青2 			[変身]
a	1010
9	1001			赤2 	[変身]
8	1000
7
...
0
*/
	/* [発弾] */
	if (0xd0==((src->boss_base_spell_time_out)&0xdf))/* (256回に2回) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		obj_send1->cx256						= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
		obj_send1->cy256						= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
		unsigned int jj;
		for (jj=(0); jj<(6); jj++)
		{
			br.BULLET_REGIST_00_speed256				= (t256(4.0)-(jj<<6));				/* 弾速 */
			br.BULLET_REGIST_01_speed_offset			= -t256(5); 						/* (-5) (-3)調整減速弾 */	/* この方式になるか検討中 */
			br.BULLET_REGIST_02_angle65536				= (0/65536);						/* 下向き */
			int check_type = BULLET_KUNAI12_01_AKA+((src->boss_base_spell_time_out>>4)&2);	/* 赤青クナイ弾 +(0&1)*/
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= check_type;
		//未定br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)標準弾 */
			br.BULLET_REGIST_06_n_way					= (24);//step_tbl[(difficulty+4)];/*(32)*/
			br.BULLET_REGIST_07_div_angle65536			= (65536/24);				/* 分割角度([360/360]度を24分割) */ 	//step_tbl[(difficulty)];/*(32)*/
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
	}
	else
	/* [変身] */
//	if (0x90==((src->boss_base_spell_time_out)&0xdf))/* (256回に2回) (r32) */
	if (0x50==((src->boss_base_spell_time_out)&0xdf))/* (256回に2回) (r33)発弾エフェクト時間があるので64カウントずらす */
	{
		/* 変身可能な弾の条件を設定 */
		br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_KUNAI12_01_AKA+((src->boss_base_spell_time_out>>4)&2); /*+(0&1)*/
		/* 今画面にある弾を全部調べて、弾を変身させる。 */
		s_exchange_damnaku_check_type();
	}
	/* 雨 */
	if (0x30>((src->boss_base_spell_time_out)&0xff))/* (256回に ??回) */
	{
		if (0==((src->boss_base_spell_time_out)&0x03))	// 4カウントに1回上に8way弾を撃つ
		{
			obj_send1->cx256						= (src->cx256); 	/* 弾源x256 ボス中心から発弾。 */
			obj_send1->cy256						= (src->cy256); 	/* 弾源y256 ボス中心から発弾。 */
			//
			unsigned int jj;
			for (jj=(0); jj<(8); jj++)
			{
				/* ショット */
			//	b05_fire_flags &= (~(doll_data->identity_bit));/* off */
				br.BULLET_REGIST_00_speed256						= (t256(1.5)+((cg_game_difficulty)<<6));
				br.BULLET_REGIST_02_VECTOR_angle1024				= (1024/4)+(1024/8)+(jj<<5)+((src->boss_base_spell_time_out>>2)&0x3f);
				br.BULLET_REGIST_04_bullet_obj_type 				= (BULLET_KOME_00_SIRO);	/* 弾グラ */	/* [白米弾] */
				br.BULLET_REGIST_05_regist_type 					= REGIST_TYPE_02_GRAVITY02;
	//			br.BULLET_REGIST_06_n_way							= (8);
//				br.BULLET_REGIST_07_VECTOR_div_angle1024			= (int)(1024/64);
				br.BULLET_REGIST_07_VECTOR_jyuryoku_dan_delta256	= ((ra_nd()&0x03)+2);//t256(0.04)
				bullet_regist_vector();
			}
		}
	}
}
#endif

/*---------------------------------------------------------
	幻符「華想夢葛」は星型(5方向)に配置する。
	-------------------------------------------------------
---------------------------------------------------------*/
