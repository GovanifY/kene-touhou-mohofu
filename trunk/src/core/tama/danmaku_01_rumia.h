
/*---------------------------------------------------------
 東方模倣風  ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ルーミアの弾幕を生成します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	紅1面ボス ルーミア 夜符「ナイトバード」にちょっとだけ似た弾幕(予定)
	-------------------------------------------------------
	5弾目が自機狙いの 32分割16方向 弓状弾。
	-------------------------------------------------------
0000 0000 0x00==(time_out)&0x11 青(0x00==((time_out)&0x20))
0001 0000						青(0x00==((time_out)&0x20))
0010 0000 0x00==(time_out)&0x11 緑
0011 0000						緑
0100 0000 0x00==(time_out)&0x11 青(0x00==((time_out)&0x20))
0101 0000						青(0x00==((time_out)&0x20))
0110 0000 0x00==(time_out)&0x11 緑
0111 0000						緑
//
1000 0000 start
---------------------------------------------------------*/
static void danmaku_create_08_rumia_night_bird_zako(SPRITE *src)
{
	static int aaa_angle65536;
//	const int src_boss_base_danmaku_time_out = src->boss_base_danmaku_time_out;
//	if (128==((src->boss_base_danmaku_time_out)))
	if (0x10==((src->boss_base_danmaku_time_out)&0x10))
	{
		#if 1
		SPRITE *zzz_player;
		zzz_player = &obj00[FIX_OBJ_00_PLAYER];
		tmp_angleCCW65536_src_nerai(zzz_player, src);/*自機ねらい角作成*/
		aaa_angle65536 = src->tmp_angleCCW65536;/*自機ねらい角*/
		#else
		aaa_angle65536 = (0);/* テスト下向き */
		#endif
	//	aaa_angle65536 += (const int)(65536-(int)((65536*5)/(32)));/* 5弾目が自機狙い */
		aaa_angle65536 += (55296);/* 5弾目が自機狙い(65536-(5*(65536/32))) */
		mask65536(aaa_angle65536);
		/* 角度(65536[360/360度]を 32分割) */
	}
	else
	if (0x00==((src->boss_base_danmaku_time_out)&0x10))/* 16回 */
	{
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
		obj_send1->cy256						= (src->cy256); 	/* 弾源y256 */		/* 発弾位置の offset 用 */
		{
			int tama_color;/* 弾色 */
			int bbb;/* 方向 */
			int bbb_angle65536;/* 方向 */
			int iii;
				iii = ((src->boss_base_danmaku_time_out)&0x0f);
			if (0x00==((src->boss_base_danmaku_time_out)&0x20))
					{	bbb=-1;tama_color=0;/* 枠つき青弾 */bbb_angle65536=65536-aaa_angle65536;}
			else	{	bbb= 1;tama_color=1;/* 枠つき緑弾 */bbb_angle65536=aaa_angle65536;}
			mask65536(bbb_angle65536);
		//	obj_send1->cx256 = src->cx256+((16-(iii))<<(3+8))*bbb;/* 本物は発弾位置の x offset あり */
			obj_send1->cx256 = src->cx256+((16-(iii))<<(3+7))*bbb;/* 本物は発弾位置の x offset あり */
			br.BULLET_REGIST_speed256			= (t256(1.5));								/* 弾速 */
			br.BULLET_REGIST_angle65536 		= (bbb_angle65536); /* 発射中心角度 / 特殊機能(自機狙い/他) */
	//		br.BULLET_REGIST_div_angle1024		= (0);										/* ダミー分割角度(未使用) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU12_02_AOI+tama_color;			/* [枠つき青弾] [枠つき緑弾] */ 	/*BULLET_MARU12_03_AOI*/
			br.BULLET_REGIST_n_way				= (1);										/* [1way] */
			br.BULLET_REGIST_speed_offset		= t256(0);/*てすと*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
			bullet_regist_angle();
		}
		/* 次の弾の角度 */
		aaa_angle65536 += (int)(65536/(32));/* 角度(65536[360/360度]を 32分割) */
		mask65536(aaa_angle65536);
	}
}
/*---------------------------------------------------------
	紅1面ボス ルーミア 夜符「ナイトバード」にちょっとだけ似た弾幕(予定)
	-------------------------------------------------------
	5弾目が自機狙いの 32分割16方向 弓状弾。
	-------------------------------------------------------
0000 0000 0x00==(time_out)&0x11 青(0x00==((time_out)&0x20))
0001 0000						青(0x00==((time_out)&0x20))
0010 0000 0x00==(time_out)&0x11 緑
0011 0000						緑
0100 0000 0x00==(time_out)&0x11 青(0x00==((time_out)&0x20))
0101 0000						青(0x00==((time_out)&0x20))
0110 0000 0x00==(time_out)&0x11 緑
0111 0000						緑
//
1000 0000 start
---------------------------------------------------------*/
static void danmaku_create_28_rumia_night_bird(SPRITE *src)
{
	static int aaa_angle65536;
//	const int src_boss_base_danmaku_time_out = src->boss_base_danmaku_time_out;
//	if (128==((src->boss_base_danmaku_time_out)))
	if (0x10==((src->boss_base_danmaku_time_out)&0x10))
	{
		#if 1
		SPRITE *zzz_player;
		zzz_player = &obj00[FIX_OBJ_00_PLAYER];
		tmp_angleCCW65536_src_nerai(zzz_player, src);/*自機ねらい角作成*/
		aaa_angle65536 = src->tmp_angleCCW65536;/*自機ねらい角*/
		#else
		aaa_angle65536 = (0);/* テスト下向き */
		#endif
	//	aaa_angle65536 += (const int)(65536-(int)((65536*5)/(32)));/* 5弾目が自機狙い */
		aaa_angle65536 += (55296);/* 5弾目が自機狙い(65536-(5*(65536/32))) */
		mask65536(aaa_angle65536);
		/* 角度(65536[360/360度]を 32分割) */
	}
	else
	if (0x00==((src->boss_base_danmaku_time_out)&0x10))/* 16回 */
	{
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
		obj_send1->cy256						= (src->cy256); 	/* 弾源y256 */		/* 発弾位置の offset 用 */
		{
			int tama_color;/* 弾色 */
			int bbb;/* 方向 */
			int bbb_angle65536;/* 方向 */
			int iii;
				iii = ((src->boss_base_danmaku_time_out)&0x0f);
			if (0x00==((src->boss_base_danmaku_time_out)&0x20))
					{	bbb=-1;tama_color=0;/* 枠つき青弾 */bbb_angle65536=65536-aaa_angle65536;}
			else	{	bbb= 1;tama_color=1;/* 枠つき緑弾 */bbb_angle65536=aaa_angle65536;}
			mask65536(bbb_angle65536);
		//	obj_send1->cx256 = src->cx256+((16-(iii))<<(3+8))*bbb;/* 本物は発弾位置の x offset あり */
			obj_send1->cx256 = src->cx256+((16-(iii))<<(3+7))*bbb;/* 本物は発弾位置の x offset あり */
			br.BULLET_REGIST_speed256			= (t256(1.5));								/* 弾速 */
			br.BULLET_REGIST_angle65536 		= (bbb_angle65536); /* 発射中心角度 / 特殊機能(自機狙い/他) */
	//		br.BULLET_REGIST_div_angle1024		= (0);										/* ダミー分割角度(未使用) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU12_02_AOI+tama_color;			/* [枠つき青弾] [枠つき緑弾] */ 	/*BULLET_MARU12_03_AOI*/
			br.BULLET_REGIST_n_way				= (1);										/* [1way] */
			br.BULLET_REGIST_speed_offset		= t256(0);/*てすと*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
			bullet_regist_angle();
		}
		/* 次の弾の角度 */
		aaa_angle65536 += (int)(65536/(32));/* 角度(65536[360/360度]を 32分割) */
		mask65536(aaa_angle65536);
	}
}
/*---------------------------------------------------------
	紅1面ボス ルーミア 闇符「ディマーケイション」にちょっとだけ似た弾幕(予定)
	-------------------------------------------------------
	ルーミア交差弾:
	交差弾にも良く見ると種類があるような気がしてきた。
	闇符「ディマーケイション」の交差弾は。
		1. 一定距離に飛び出す。(通常減速弾)
		2. 弾停止。
		3. 突然 (弾グラが)90度回転する。
		4.弾グラが傾きながら弾が広がる。
	-------------------------------------------------------
	16
	-------------------------------------------------------
	0010 01--	0x027f == 64*10-1
	0010 00--	0x023f == 64* 9-1
				...
	0000 00--	0x003f == 64* 1-1
---------------------------------------------------------*/
static void danmaku_create_29_rumia_demarcation(SPRITE *src)
{
	unsigned int tama_color;/* 弾色 */
	tama_color = (0);
	if ((0x3f)==((src->boss_base_danmaku_time_out)&0x3f))
	{
		unsigned int tama_index;
		tama_index = 9-((src->boss_base_danmaku_time_out)>>6);
		if (3>tama_index)
		{
			unsigned int ttt[4]=
			{
				BULLET_KOME_03_AOI, 	/* 青弾 */
				BULLET_KOME_05_MIDORI,	/* 緑弾 */
				BULLET_KOME_01_AKA, 	/* 赤弾 */
				0
			};
			tama_color = ttt[tama_index];
		}
	}
	if ((0) != tama_color)
	{
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
		//
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
		obj_send1->cx256						= (src->cx256); 	/* 弾源x256 */
		obj_send1->cy256						= (src->cy256); 	/* 弾源y256 */
		{
			br.BULLET_REGIST_speed256			= (t256(1.5));								/* 弾速 */
			br.BULLET_REGIST_angle65536 		= (0);					/* 発射中心角度 / 特殊機能(自機狙い/他) */
			br.BULLET_REGIST_div_angle65536 	= (65536/16);			/* 分割角度 */
			br.BULLET_REGIST_bullet_obj_type	= tama_color;			/* [枠つき青弾] [枠つき緑弾] */
			br.BULLET_REGIST_n_way				= (16); 									/* [16way] */
			br.BULLET_REGIST_speed_offset		= t256(0);/*てすと*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
			bullet_regist_angle();
		}
	}
}
