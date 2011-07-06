
/*---------------------------------------------------------
 東方模倣風  ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	魅魔の弾幕を生成します。
---------------------------------------------------------*/


/*---------------------------------------------------------
	魅魔、花てすと弾幕
---------------------------------------------------------*/

static void mima_boss01_nway_fire(SPRITE *src)
{
	static int adj_hari65536;	/* 開始地点 */
	SPRITE *zzz_player;
	zzz_player = &obj00[FIX_OBJ_00_PLAYER];
	tmp_angleCCW65536_src_nerai(zzz_player, src);/*自機狙い角作成*/
	adj_hari65536 = (src->tmp_angleCCW65536);
//
	const int speed256 = (/*0x200*/0x100)+(ra_nd()&0xff)-((3-difficulty)<<6);/* 難易度別で速度低下 */
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
//	const int src__x256 = (src->cx256)+((0<src->vx256)?(t256(16)):(0));/* 右移動中は右側から撃つ */
	const int src__x256 = (src->cx256)+((0<src->vx256)?(t256(8)):(-8));/* 右移動中は右側から撃つ */
//
		//	br.BULLET_REGIST_div_angle65536 	= (0);									/* ダミー分割角度(未使用) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_HARI32_00_AOI+((src->boss_base_danmaku_time_out)>>5);				/* [水色針弾] */	/*hari_no_iro*/
			br.BULLET_REGIST_n_way				= (1);									/* [1way] */
//			br.BULLET_REGIST_speed_offset		= t256(1/*0*/);/*てすと*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
//
	int ii65536;
	for (ii65536=(adj_hari65536/*0*/); ii65536<(adj_hari65536+65536-(20*64)); ii65536+=(65536/5) )
	{
		int jj1024;
		int kk65536;
		kk65536 = (0);
		for (jj1024 = (0); jj1024<(1024/2); jj1024 += (32) )
		{
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
			obj_send1->cx256 					= (src__x256);							/* 右移動中は右側から撃つ */
			obj_send1->cy256 					= (src->cy256);							/* 弾源y256 */
			br.BULLET_REGIST_speed256			= (int)(speed256 + sin1024((jj1024)));	/* 弾速 */
			#if 0
		//	br.BULLET_REGIST_speed_offset		= -t256(br.BULLET_REGIST_speed256>>6);/* (魅魔様lunaticで)これでも良い気もするけど。 */
			br.BULLET_REGIST_speed_offset		= -(br.BULLET_REGIST_speed256<<(8-6));/* (魅魔様lunaticで)これでも良い気もするけど。 */
			#endif
		//	br.BULLET_REGIST_speed_offset		= -t256(br.BULLET_REGIST_speed256>>7);/* (パチェがあるから)少し簡単にしとく。 */
			br.BULLET_REGIST_speed_offset		= -(br.BULLET_REGIST_speed256<<(8-7));/* (パチェがあるから)少し簡単にしとく。 */
			br.BULLET_REGIST_angle65536 		= ((ii65536)+(kk65536));				/* 角度 */
			bullet_regist_angle();
			kk65536 += ((65536/16)/5);
		}
	}
}

/*---------------------------------------------------------
	魅魔、花てすと弾幕
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
static void danmaku_create_0c_hana_test(SPRITE *src)
{
//	if (0x40==((src->boss_base_danmaku_time_out)&0xcf))/* 4回 */
//	if (0x10==((src->boss_base_danmaku_time_out)&0x1f))/* (16回に1回)(128なら計8回) */
	if (0x10==((src->boss_base_danmaku_time_out)&0x1f))/* (16回に1回)(128なら計4回) */
	{
		if (difficulty >= ((src->boss_base_danmaku_time_out)>>5))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		//	bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			bullet_play_04_auto(VOICE11_BOSS_KIRARIN);
		//	voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);/*テキトー*/
			#endif
		//
			mima_boss01_nway_fire(src);/*enemy_boss04_fire(2);*/
		}
	}
}

/*---------------------------------------------------------
	魅魔、接近弾幕
	-------------------------------------------------------
	ここで下方うねり4方向、重力弾っぽい奴(予定)
---------------------------------------------------------*/
static void danmaku_create_0d_mima_sekkin(SPRITE *src)
{
	static int src_shot_angle1024;	/* 開始地点 */
//	if (0x40==((src->boss_base_danmaku_time_out)&0xcf))/* 4回 */
//	if (0x10==((src->boss_base_danmaku_time_out)&0x1f))/* (32回に1回)(128なら計 回) */
	if (0x04==((src->boss_base_danmaku_time_out)&0x07))/* (8回に1回) */
	{
	//	if (difficulty >= ((src->boss_base_danmaku_time_out)>>5))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		//	bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			bullet_play_04_auto(VOICE11_BOSS_KIRARIN);
		//	voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);/*テキトー*/
			#endif
		//
		//	mima_boss01_nway_fire(src);/*enemy_boss04_fire(2);*/
			{
				src_shot_angle1024 -= (16); 		/* cv1024r(10)*/
//
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
				obj_send1->cx256 						= (src->cx256);
				obj_send1->cy256 						= (src->cy256);
				br.BULLET_REGIST_speed256				= t256(1.0);//t256(1+difficulty)/*(3+difficulty)*/ /*(4+difficulty)*/;
//				br.BULLET_REGIST_angle1024				= (src->shot_angle1024&(256-1))+512+128;// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
				br.BULLET_REGIST_angle1024				= (src_shot_angle1024&(256-1))-512-128;// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
			//	br.BULLET_REGIST_jyuryoku_dan_delta256	= ((ra_nd()&0x03)+1);//t256(0.04)/*10*/
				br.BULLET_REGIST_jyuryoku_dan_delta256	= ((ra_nd()&0x03)+2);//t256(0.04)
				br.BULLET_REGIST_bullet_obj_type		= (BULLET_MARU8_03_AOI);	/* 弾グラ */
				br.BULLET_REGIST_regist_type			= REGIST_TYPE_02_GRAVITY02;
				bullet_regist_vector();
			}
		}
	}
}

#if 0
/*---------------------------------------------------------
	魅魔、トゲ弾幕(秋 静葉、葉符「狂いの落葉」っぽい？)
	-------------------------------------------------------
	テキトー
	[0] f e d
	[c] b a 9
	[8] 7 6 5
	[4] 3 2 1
---------------------------------------------------------*/
static void danmaku_create_24_mima_toge(SPRITE *src)
{
	if ((DANMAKU_0640_TIME-1)==((src->boss_base_danmaku_time_out) ))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE17_BOSS_TAME01);
		#endif
	}
	if (0x00==((src->boss_base_danmaku_time_out)&0x03))
	{
		static unsigned int kaiten_aaa;
		static unsigned int kaiten_bbb;
		kaiten_aaa -= (10);
	//	kaitensa &= (1024-1);
		//
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
		obj_send1->cx256 					= (src->cx256);							/* 弾源x256 */
		obj_send1->cy256 					= (src->cy256);							/* 弾源y256 */
		//
		//
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		br.BULLET_REGIST_speed_offset		= t256(1/*0*/);/*てすと*/
		br.BULLET_REGIST_bullet_obj_type	= BULLET_HARI32_00_AOI; 				/* [水色針弾] */
		//
		if (0x00==((src->boss_base_danmaku_time_out)&0x0f))
		{
			br.BULLET_REGIST_div_angle65536 	= (int)(65536/24);						/* 分割角度 */
			br.BULLET_REGIST_n_way				= (1);									/* [1way] */
			br.BULLET_REGIST_speed256			= (t256(2.0) ); 	/* 弾速 */
		}
		else
		if (0x0c==((src->boss_base_danmaku_time_out)&0x0f))
		{
			br.BULLET_REGIST_div_angle65536 	= (int)(65536/24);						/* 分割角度 */
			br.BULLET_REGIST_n_way				= (2);									/* [2way] */
			br.BULLET_REGIST_speed256			= (t256(1.75) );	/* 弾速 */
		}
		else
		if (0x08==((src->boss_base_danmaku_time_out)&0x0f))
		{
			br.BULLET_REGIST_div_angle65536 	= (int)(65536/12);						/* 分割角度 */
			br.BULLET_REGIST_n_way				= (2);									/* [2way] */
			br.BULLET_REGIST_speed256			= (t256(1.5) ); 	/* 弾速 */
		//	kaiten_bbb -= ((int)(65536/48)/*+(int)(65536/24)*/);
			kaiten_bbb -= ((int)(65536/24));
		}
		//
	//	kaiten_bbb -= ((int)(65536/48)/*+(int)(65536/24)*/);
		kaiten_bbb -= ((int)(65536/48));
		mask65536(kaiten_bbb);
		br.BULLET_REGIST_angle65536 		= (((kaiten_aaa+kaiten_bbb))&(65536-1));								/* 角度 */
		//
		if (0x04!=((src->boss_base_danmaku_time_out)&0x0f))
		{
			bullet_regist_angle();
		}
	}
}
#endif

#if 1
/*---------------------------------------------------------
	魅魔、トゲ弾幕(秋 静葉、葉符「狂いの落葉」っぽい？)
	-------------------------------------------------------
	テキトー
	f e d [c] 1100
	b a 9 [8] 1000
	7 6 5 [4] 0100
	3 2 1 [0] 0000
---------------------------------------------------------*/
static void danmaku_create_24_mima_toge(SPRITE *src)
{
	if (0x00==((src->boss_base_danmaku_time_out)&0x33))
	{
		static unsigned int kaiten_aaa;
		if (0x00==((src->boss_base_danmaku_time_out)&0x3f))
		{	/* ちょっとアリス風味 */
			static unsigned int kakudo_111;
			kakudo_111 += (7);
			kaiten_aaa -= ((sin1024(kakudo_111))<<6);
			#if (1)
			voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);
		//	bullet_play_04_auto(VOICE11_BOSS_KIRARIN);
			#endif
		}
	//	kaitensa &= (1024-1);
		//
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
		obj_send1->cx256 					= (src->cx256);							/* 弾源x256 */
		obj_send1->cy256 					= (src->cy256);							/* 弾源y256 */
		//
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		br.BULLET_REGIST_bullet_obj_type	= BULLET_HARI32_00_AOI; 				/* [水色針弾] */
		br.BULLET_REGIST_n_way				= (5);									/* [1way] */
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/5);						/* 分割角度 */
		//
		unsigned int aaa = ((src->boss_base_danmaku_time_out)&0x0f);
		unsigned int bbb = ((0x0c-aaa)<<8);
		br.BULLET_REGIST_speed256			= (t256(1.5) ); 	/* 弾速 */
		br.BULLET_REGIST_speed_offset		= (t256(1)+(aaa<<8));/*てすと*/
		//
		br.BULLET_REGIST_angle65536 		= (((kaiten_aaa+bbb))&(65536-1));								/* 角度 */
		bullet_regist_angle();
		br.BULLET_REGIST_angle65536 		= (((kaiten_aaa-bbb))&(65536-1));								/* 角度 */
		bullet_regist_angle();
	}
}
#endif
