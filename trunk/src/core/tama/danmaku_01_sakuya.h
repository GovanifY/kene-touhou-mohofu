
/*---------------------------------------------------------
 東方模倣風  ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	咲夜の弾幕を生成します。
---------------------------------------------------------*/


/*---------------------------------------------------------
	紅5面中ボス 咲夜 第一攻撃にちょっとだけ似た弾幕
	-------------------------------------------------------
	この弾幕の本質は、
	１．比較的速い(というか中速度)の青ナイフ弾の壁をくぐりながら、
	異常に遅いかつ微加速なので長く留まって避け難い赤クナイ弾を避ける事。
	２．青ナイフ弾と赤クナイ弾の速度が違うので、重なって避け難い場合が発生する事。
	３．しかしながら、重なりはある程度の時間を観察していれば避けられる。
	４．(３．)に矛盾して、観察時間が短い(こういう場合も必ず発生する)と運が悪いと避けられない。
	-------------------------------------------------------
	■ 青ナイフ
		青ナイフは曲がった壁にならないと魅力は無い。
		比較的速い速度だが、速過ぎると魅力は無い。微妙にいらいらする遅さも必要。
	■ 赤クナイ
		赤クナイにはいらいらする遅さが必要。
		画面下で避けていた場合に、「画面下あたりでのみ急激に速度が変化する」。
		こういう加速度で調整する必要あり。
		前記２－４は主に画面下あたりで発生する現象。
		画面中あたりでは、その分敵に近いので反射神経は要るが、前記２－４の現象は発生しにくい。
	-------------------------------------------------------
	■ 赤クナイ
		なんか知らんが原作やり直したら「減速弾」だった。
		「加速弾」＋「減速弾」って事かなぁ？
	-------------------------------------------------------
	原作と違い、そもそも上に行けないから、上避けは出来ない。
	(pspの縦解像度では上に行くスペースがない)
	下避けもpspの縦解像度が全然足りないから、本家風のチョン避けは無理。
	ある程度左右に動き回って避けて貰う事を想定している。(だから弾速遅い)
	それに併せて密着弾はあえてぜんぜん違う。(別変数とるのが面倒だった事もある)
	-------------------------------------------------------
	青ナイフの弾速はこれくらいかも？赤クナイの弾速は速過ぎる。
	赤クナイ弾の周期は原作はだいぶずれてくるので変えたいんだけど、
	「スペカシステム二重化」しないと上手くいかないかも？。
	-------------------------------------------------------
	「スペカシステム二重化」：2種類の異なったスペカが同時に撃てるスペカシステム。
	そういうものにするべきかしないべきか検討中。(パチェ)
	-------------------------------------------------------
	まだ禊(脅し)なので、(プレイヤーにこんな処で)死んでもらっては困る。
	-------------------------------------------------------
	青ナイフspeed256	speed_offset	/	赤クナイspeed256	speed_offset
	t256(2.5)			(0) 			/	(0) 				(4) 			// 良いんだけど本物より難しい。
	t256(2.0)			(0) 			/	(0) 				(2) 			// 赤クナイの初速遅すぎるかなあ。// 青ナイフと赤クナイがずれすぎ。本物より難しい。
	t256(1.75)			(0) 			/	(0) 				(1)
---------------------------------------------------------*/



//#define ADJUST3916	((65536/20)+(640))
#define ADJUST3916	(4000)
static void danmaku_create_01_sakuya_misogi(SPRITE *src)
{
//	DANMAKU_0256_TIME	DANMAKU_0128_TIME
	static int aaa_angle65536;
	static int tama_aaa_interval;		// 青ナイフ
//	static int tama_bbb_interval;		// 赤クナイ
	if (127 == (src->boss_base_danmaku_time_out))/* 初期値にリセット。 */
	{
		#if 1
		/* 青ナイフ: 右上側から撃ち始める。右側なので90度ずらして、bullet_regist_angle()の補正も行う。 */
	//	src->tmp_angleCCW65536	= (65536+(65536/4)-((65536*8)/20))+(ADJUST3916);				// 青ナイフ
	//	src->tmp_angleCCW65536	= (((65536*2)/20))+(ADJUST3916);				// 青ナイフ 	bullet_regist_angle();で下を基準に、時計回りに(2/20)から撃ち始める。
		src->tmp_angleCCW65536	= (((65536*1)/18))+(ADJUST3916);				// 青ナイフ 	bullet_regist_angle();で下を基準に、時計回りに(2/20)から撃ち始める。
		// 本物は打ち始めの青ナイフの端が右上60度くらいから打ち始め、
		// 打ち終わりは、左上60度くらいの位置になる。
		#endif
		#if 1
		/* 赤クナイ: */
	//	aaa_angle65536			= (65536/2);		// 赤クナイ
		aaa_angle65536			= (65536/4)-(0);		// 赤クナイ (右側から)
		#endif
		tama_aaa_interval = 0;	// 青ナイフ
//		tama_bbb_interval = 0;	// 赤クナイ
	}
//	if (DANMAKU_0096_TIME > (src->boss_base_danmaku_time_out))/* 暫く何もしない。 */
	if (100 > (src->boss_base_danmaku_time_out))/* 暫く何もしない。 */
	{
		tama_aaa_interval--;
		if (0 >= tama_aaa_interval)
	//	if (0==((src->boss_base_danmaku_time_out)&0x07))
		{
			tama_aaa_interval = (12);
			/*	青ナイフ */
			#if (1)
		//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
			#endif
		//	src->tmp_angleCCW1024 -= (int)(256/5)+(10); /*CCW*/
		//	src->tmp_angleCCW1024 -= (int)(1024/20)+(10); /*CCW*/
		//	src->tmp_angleCCW65536 -= (int)(65536/20)+(640); /*CCW*/
		//	src->tmp_angleCCW65536 -= (int)(65536/20)+(640)+(128); /*CCW*/	/* 3276== (65536/20) */
			src->tmp_angleCCW65536 -= (ADJUST3916); /*CCW*/
			mask65536(src->tmp_angleCCW65536);
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
			obj_send1->cx256					= (src->cx256); 					/* 弾源x256 */
			obj_send1->cy256					= (src->cy256); 					/* 弾源y256 */
		//	br.BULLET_REGIST_speed256			= (t256(2.5));						/* 弾速 (t256(2.5)参考。紅ノーマル5面) */
		//	br.BULLET_REGIST_speed256			= (t256(2.0));						/* 弾速 pspは縦解像度がないのでとにかく遅くしないと。 */
//			br.BULLET_REGIST_speed256			= (t256(1.75)); 					/* 弾速 pspは縦解像度がないのでとにかく遅くしないと。 */
			br.BULLET_REGIST_speed256			= (t256(1.00)); 					/* 弾速 pspは縦解像度がないのでとにかく遅くしないと。 */
			br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW65536));		/* 弾源角度65536 */ 		/* [螺旋状弾] */
		//	br.BULLET_REGIST_div_angle1024		= (int)(256/5);
		//	br.BULLET_REGIST_div_angle1024		= (int)(1024/20);
		//	br.BULLET_REGIST_div_angle65536 	= (int)(65536/20);					/* 分割角度([360/360]度を20分割) */
			br.BULLET_REGIST_div_angle65536 	= (int)(65536/18);					/* 分割角度([360/360]度を20分割)[打ち終わりが左斜め上になる] */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [青ナイフ弾] */
			br.BULLET_REGIST_n_way				= (8);								/* [8way] */
		//	br.BULLET_REGIST_speed_offset		= t256(0);/*てすと*/
		//	br.BULLET_REGIST_speed_offset		= t256(1);/*加速してみるてすと*/
			br.BULLET_REGIST_speed_offset		= t256(6);/*加速してみるてすと*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
			bullet_regist_angle();

		//	if (0==((src->boss_base_danmaku_time_out)&0x1f))/* reset (1/32) */
		//	{
		//	//	aaa_angle1024	= (src->tmp_angleCCW1024)-(int)((1024/2));
		//		aaa_angle65536	= (src->tmp_angleCCW65536)-(int)((65536/2));
		//	}
		}
	}
//	/* 赤クナイの方が遅いので、青ナイフより先に撃ち始める事にする。 */
//	if (DANMAKU_0032_TIME > (src->boss_base_danmaku_time_out))/* 暫く何もしない。 */
	if (41 > (src->boss_base_danmaku_time_out))/* 暫く何もしない。 */
//	if ((128-40) < (src->boss_base_danmaku_time_out))/* 最後は暫く何もしない。 */
	{
	//	else/* こっちの弾は本当はずれて出る感じにしたい。 */
//	//	if ((10-1)>((((u8)src->boss_base_danmaku_time_out)^0x20)&0x2f))/* x 10set */
//		if ((10-1)>(((src->boss_base_danmaku_time_out)	  )&0x0f))/* x 10set */
		/* 4回に一回、計10set */
		if ((0)==(((src->boss_base_danmaku_time_out)	  )&0x03))/* x 10set */
		{
			/* こっちの赤クナイは全然ダメ． */
		//	aaa_angle1024 -= (int)(256/5);
		//	aaa_angle1024 -= (int)(1024/20);
			aaa_angle65536 -= (int)(65536/20);
		//	if (0 > (int)aaa_angle65536)	{	aaa_angle65536 = (65536/2); 	}
		//	mask65536(aaa_angle65536);
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
			obj_send1->cx256					= (src->cx256); 					/* 弾源x256 */
			obj_send1->cy256					= (src->cy256); 					/* 弾源y256 */
		//	br.BULLET_REGIST_speed256			= (t256(2.0));						/* 弾速 速過ぎる */
		//	br.BULLET_REGIST_speed256			= (t256(1.0));						/* 弾速 (出始めは遅い) */
		//	br.BULLET_REGIST_speed256			= (t256(0.5));						/* 弾速 (出始めは遅い) */
		//	br.BULLET_REGIST_speed256			= (t256(0.25)); 					/* 弾速 (出始めは遅い) */
		//	br.BULLET_REGIST_angle65536 		= (((aaa_angle65536)+(65536*3/4))&(65536-1));	/* 弾源角度65536 */ 	/*下CCW*/
			br.BULLET_REGIST_angle65536 		= (((aaa_angle65536))&(65536-1));	/* 弾源角度65536 */ 	/*下CCW*/
			#if 1
		//	br.BULLET_REGIST_div_angle65536 	= (int)((difficulty+difficulty+1+1/*3*/)<<(6)); 	/* [密着弾]の分割角度 */
		//	br.BULLET_REGIST_div_angle65536 	= (int)((difficulty+1/*3*/)<<(6+1));		/* [密着弾]の分割角度(r31) */
		//	br.BULLET_REGIST_div_angle65536 	= (int)((difficulty+1/*3*/)<<(6+1+1));		/* [密着弾]の分割角度(r32) */
		//	static const u16 bbb_bbb[4] = {(u16)(128), (u16)(512-64), (u16)(512), (u16)(512+64)};
			br.BULLET_REGIST_div_angle65536 	= (int)(512-64);		/* [密着弾]の分割角度(r32) */
			#endif
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;			/* [赤クナイ弾] */
			br.BULLET_REGIST_n_way				= (4);								/* [4way] */
//-------------------------
	// ■  [加速弾]
		//	br.BULLET_REGIST_speed256			= (0);								/* 弾速 (出始めは遅い) */
//			br.BULLET_REGIST_speed256			= (t256(0.25)); 							/* 弾速 (出始めは遅い) */
		//	br.BULLET_REGIST_speed256			= (t256(3.25)); 							/* 弾速 (出始めは遅い) */
		//	br.BULLET_REGIST_speed256			= (t256(3.00)); 							/* 弾速 (出始めは遅い) */
			br.BULLET_REGIST_speed256			= (t256(2.00)); 							/* 弾速 (出始めは遅い) */

		//	br.BULLET_REGIST_speed_offset		= t256(3/*0*/);/* 加速してみる てすと*/
		//	br.BULLET_REGIST_speed_offset		= t256(8);/* 微加速？(加速弾な事は間違いない) */
		//	br.BULLET_REGIST_speed_offset		= t256(4);/* 微加速？(加速弾な事は間違いない) */
//			br.BULLET_REGIST_speed_offset		= t256(2);/* 微加速？(加速弾な事は間違いない) */
		//	br.BULLET_REGIST_speed_offset		= t256(1);/* 微加速？(加速弾な事は間違いない) */
			br.BULLET_REGIST_speed_offset		= t256(-1);/* 減速？(加速弾な事は間違いない) */
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
			bullet_regist_angle();
//-------------------------
	// ■  [減速弾]
		//	br.BULLET_REGIST_speed256			= (t256(0.25)); 							/* 弾速 (出始めは遅い) */
		//	br.BULLET_REGIST_speed256			= (t256(1.25)); 							/* 弾速 (出始めは遅い) */
			br.BULLET_REGIST_speed256			= (t256(2.25)); 							/* 弾速 (出始めは遅い) */
			br.BULLET_REGIST_speed_offset		= t256(-1);/* 減速？(加速弾な事は間違いない) */
			bullet_regist_angle();
		}
	}
}

/*---------------------------------------------------------
	紅5面中ボス 咲夜「奇術：ミスディレクション」にちょっとだけ似た弾幕
	-------------------------------------------------------
---------------------------------------------------------*/
static void danmaku_create_22_sakuya_miss_direction(SPRITE *src)
{
	if (256-64< src->boss_base_danmaku_time_out)
	{;}
	else
	if (256-64-48< src->boss_base_danmaku_time_out)
	{
		/* 赤クナイ弾 */
		if (0==((src->boss_base_danmaku_time_out)&0x03))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			#endif
			SPRITE *zzz_player;
			zzz_player = &obj00[FIX_OBJ_00_PLAYER];
			tmp_angleCCW65536_src_nerai(zzz_player, src);/*自機狙い角作成*/
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
			obj_send1->cx256					= (src->cx256); 					/* 弾源x256 */
			obj_send1->cy256					= (src->cy256); 					/* 弾源y256 */
		//	br.BULLET_REGIST_speed256			= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* 弾速 */
			br.BULLET_REGIST_speed256			= (t256(2.0)-((src->boss_base_danmaku_time_out)));	/* 弾速 */
		//	#define TORIAEZU_000 (0)/* 謎の調整値 */
			#define TORIAEZU_000 (int)(6*64)/* 謎の調整値 */
			br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW65536)) + TORIAEZU_000;		/* 弾源角度1024 */	/* 自機狙い弾 */
			br.BULLET_REGIST_div_angle65536 	= (int)(65536/24);					/* 分割角度 */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;			/* [赤クナイ弾] */
			br.BULLET_REGIST_n_way				= (24); 							/* [24way] */
			br.BULLET_REGIST_speed_offset		= t256(1);/*てすと*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
			bullet_regist_angle();
		}
	}
	else
//	if (256-192< src->boss_base_danmaku_time_out)
	if (48< src->boss_base_danmaku_time_out)
	{;}
	else
//	if (256-192< src->boss_base_danmaku_time_out)
	{
		/* 青ナイフ */
		if (0==((src->boss_base_danmaku_time_out)&0x0f))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			#endif
			SPRITE *zzz_player;
			zzz_player = &obj00[FIX_OBJ_00_PLAYER];
			tmp_angleCCW65536_src_nerai(zzz_player, src);/*自機ねらい角作成*/
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
			obj_send1->cx256					= (src->cx256); 					/* 弾源x256 */
			obj_send1->cy256					= (src->cy256); 					/* 弾源y256 */
		//	#define TORIAEZU_111 (0)/* 謎の調整値 */
			#define TORIAEZU_111 (int)(4*64)/* 謎の調整値 */
			br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW65536))-(int)(65536*6/(7*4)) + TORIAEZU_111;		/* 弾源角度1024 */
			br.BULLET_REGIST_div_angle65536 	= (int)(65536/(7*4));				/* 分割角度([90/360]度を7分割==28分割) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [青ナイフ弾] */
			br.BULLET_REGIST_n_way				= (11); 							/* [11way] */
			br.BULLET_REGIST_speed_offset		= t256(0);/*てすと*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
	// 弾1
			br.BULLET_REGIST_speed256			= (t256(2.0));						/* 弾速 */
			bullet_regist_angle();
	// 弾2
			br.BULLET_REGIST_speed256			= (t256(1.5));						/* 弾速 */
			bullet_regist_angle();
		}
	}
}
/*---------------------------------------------------------
	紅5面中ボス 咲夜「奇術：ミスディレクション(1/2)」にちょっとだけ似た弾幕
	-------------------------------------------------------
	自機狙い、全方向24分割弾
	-------------------------------------------------------
	[注：現在bu llet_create_n_way_dan_sa_type()の自機狙いはワザト狙わない奇数弾なので、動かなければ当たりませんが、
	そのうちここは偶数弾にする予定なので、動かなければ当たるようになる予定です]
---------------------------------------------------------*/
//DANMAKU_0048_TIME
static void danmaku_create_02_24nerai(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x03))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		SPRITE *zzz_player;
		zzz_player = &obj00[FIX_OBJ_00_PLAYER];
		tmp_angleCCW65536_src_nerai(zzz_player, src);/*自機狙い角作成*/
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
		obj_send1->cx256					= (src->cx256); 					/* 弾源x256 */
		obj_send1->cy256					= (src->cy256); 					/* 弾源y256 */
	//	br.BULLET_REGIST_speed256			= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* 弾速 */
		br.BULLET_REGIST_speed256			= (t256(2.0)-((src->boss_base_danmaku_time_out)));	/* 弾速 */
	//	#define TORIAEZU_000 (0)/* 謎の調整値 */
		#define TORIAEZU_000 (int)(6*64)/* 謎の調整値 */
		br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW65536)) + TORIAEZU_000;		/* 弾源角度1024 */	/* 自機狙い弾 */
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/24);					/* 分割角度 */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;			/* [赤クナイ弾] */
		br.BULLET_REGIST_n_way				= (24); 							/* [24way] */
		br.BULLET_REGIST_speed_offset		= t256(1);/*てすと*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
		bullet_regist_angle();
	}
}


/*---------------------------------------------------------
	紅5面中ボス 咲夜 「奇術：ミスディレクション(2/2)」にちょっとだけ似た弾幕
	-------------------------------------------------------
	自機狙い、11弾(中心と、左右に5弾ずつ狙い)
	-------------------------------------------------------
	[注：現在bu llet_create_n_way_dan_sa_type()の自機狙いはワザト狙わない奇数弾なので、動かなければ当たりませんが、
	そのうちここは偶数弾にする予定なので、動かなければ当たるようになる予定です]
	-------------------------------------------------------
	原作やりなおしたら、ここは３回撃つ。
---------------------------------------------------------*/
//DANMAKU_0064_TIME
static void danmaku_create_03_11nife(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x07))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		SPRITE *zzz_player;
		zzz_player = &obj00[FIX_OBJ_00_PLAYER];
		tmp_angleCCW65536_src_nerai(zzz_player, src);/*自機ねらい角作成*/
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
		obj_send1->cx256					= (src->cx256); 					/* 弾源x256 */
		obj_send1->cy256					= (src->cy256); 					/* 弾源y256 */
		br.BULLET_REGIST_speed256			= (t256(2.0));						/* 弾速 */
	//	#define TORIAEZU_111 (0)/* 謎の調整値 */
		#define TORIAEZU_111 (int)(4*64)/* 謎の調整値 */
		br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW65536))-(int)(65536*6/(7*4)) + TORIAEZU_111;		/* 弾源角度1024 */
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/(7*4));				/* 分割角度([90/360]度を7分割==28分割) */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [青ナイフ弾] */
		br.BULLET_REGIST_n_way				= (11); 							/* [11way] */
		br.BULLET_REGIST_speed_offset		= t256(0);/*てすと*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
		bullet_regist_angle();
	}
}


/*---------------------------------------------------------
	紅5面ボス 咲夜 「通常攻撃1(1/2)」にちょっとだけ似た弾幕(予定)
	-------------------------------------------------------
	24way =(右12way)+(左12way)
	-------------------------------------------------------

---------------------------------------------------------*/
static void danmaku_create_04_pink_hearts(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x0f))/* 本物は 8 弾(128==0x80==danmaku_time_out) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		obj_send1->cy256						= (src->cy256); 					/* 弾源y256 */	/* 本物は発弾位置の offset 用 */
		obj_send1->tmp_angleCCW65536 = (0);/* 下向き */
	//
		//	br.BULLET_REGIST_div_angle1024		= (0);								/* ダミー分割角度(未使用) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_06_YUKARI; 		/* [ピンクナイフ弾] */
			br.BULLET_REGIST_n_way				= (1);								/* [1way] */
			br.BULLET_REGIST_speed_offset		= t256(1/*0*/);/*てすと*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
		//
		int jj;
		jj=0;
		int i;
		for (i=0; i<(13*12)/*144*/; i+= (13) )/*12方向*/							/* 角度(128[(90+a)/360度]を 10 分割) */
		{
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
			obj_send1->cx256					= src->cx256+t256(4.0); 			/* 本物は発弾位置の x offset あり */
			br.BULLET_REGIST_speed256			= ((160-i)<<(1+1/*+1*/));				/* 弾速 */
			br.BULLET_REGIST_angle65536 		= (jj/*(0+i+i)&(1024-1)*/);
			bullet_regist_angle();
			//
			obj_send1->cx256					= src->cx256-t256(4.0); 			/* 本物は発弾位置の x offset あり */
			br.BULLET_REGIST_speed256			= ((160-i)<<(1+1/*+1*/));				/* 弾速 */
			br.BULLET_REGIST_angle65536 		= ((0-jj)&(65536-1));
			bullet_regist_angle();
			//
			jj += (13*2*64);
		}
	}
}


/*---------------------------------------------------------
	紅5面ボス 咲夜 「通常攻撃1(2/2)」にちょっとだけ似た弾幕(予定)
	-------------------------------------------------------
	32方向に分割、偶数は低速弾、奇数は高速弾。
	16方向を2回登録する。
	-------------------------------------------------------
	32方向	角度(1024[360/360度]を 32 分割)1024 == 32 x 32
---------------------------------------------------------*/
static void danmaku_create_05_32way_dual(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x3f))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		//
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
			obj_send1->cx256					= (src->cx256); 					/* 弾源x256 */
			obj_send1->cy256					= (src->cy256); 					/* 弾源y256 */
		//
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [青ナイフ弾] */
			br.BULLET_REGIST_n_way				= (16); 							/* [16way] */
			br.BULLET_REGIST_speed_offset		= t256(1/*0*/);/*てすと*/
			br.BULLET_REGIST_div_angle65536 	= (int)(65536/(16));		/* 分割角度(65536[360/360度]を 16 分割) */	/* 1周をn分割した角度 */
//			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		//
			br.BULLET_REGIST_speed256			= (t256(0.75)); 		/* 偶数は低速弾 */
			br.BULLET_REGIST_angle65536 		= (0);					/* 弾源角度65536 */
			bullet_regist_angle();
		//
			br.BULLET_REGIST_speed256			= (t256(1.25)); 		/* 奇数は高速弾 */
			br.BULLET_REGIST_angle65536 		= (65536/32);			/* 弾源角度65536 */
			bullet_regist_angle();
	}
}


/*---------------------------------------------------------
	幻在「クロックコープス(1/2)」
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	幻在「クロックコープス(2/2)」
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	幻象「ルナクロック(1/2)」
	-------------------------------------------------------
	32way 固定連弾	x 4回
---------------------------------------------------------*/
static void danmaku_create_06_luna_clock_32way(SPRITE *src)
{
	if (0x40==((src->boss_base_danmaku_time_out)&0xcf))/* 4回 */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		br.BULLET_REGIST_speed256			= (t256(1.0));				/* 弾速 */
		br.BULLET_REGIST_speed_offset		= t256(1/*0*/);/*てすと*/
		br.BULLET_REGIST_angle65536 		= (0);						/* 発射中心角度 / 特殊機能(自機狙い/他) */
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/(32));		/* 分割角度(1024[360/360度]を 32 分割) */	/* 1周をn分割した角度 */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_CAP16_05_TUTU_SIROI;	/* [青白実包弾] */	/* 弾グラ */
		br.BULLET_REGIST_n_way				= (32); 					/* [32way] */		/* 発弾数 */
//		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_angle();
	}
}


/*---------------------------------------------------------
	幻象「ルナクロック(2/2)」
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	「通常攻撃3(1/2)」
	-------------------------------------------------------
	拡散型全方位ナイフ弾
	80分割弾 == (8way[hi to 0]+8way[slow to 0]) x 5way(5分割)
	-------------------------------------------------------
	本物は画面上と画面左と画面右の端でナイフが一度だけ反射する。
---------------------------------------------------------*/
static void danmaku_create_07_80way_dual_five(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x3f))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
			obj_send1->cx256					= (src->cx256); 	/* 弾源x256 */
			obj_send1->cy256					= (src->cy256); 	/* 弾源y256 */
	//		br.BULLET_REGIST_div_angle1024		= (0);									/* ダミー分割角度(未使用) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;				/* [青ナイフ弾] */
			br.BULLET_REGIST_n_way				= (1);									/* [1way] */
			br.BULLET_REGIST_speed_offset		= t256(1/*0*/);/*てすと*/
//			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
	//
		int ii; 	/* 1周を65536分割した角度 */
		int j;		/* カウンタ */
		j=0;
		for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)	12.8==(1024/80) 80分割 [5分割] */	/* 204.8==(1024/5) 角度(1024[72/360度]を 5 分割) */
		{
			j++;
			j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
			br.BULLET_REGIST_speed256			= (t256(3.0)-((j)<<5)-((j&1)<<7));		/*奇数は高速弾*/
			br.BULLET_REGIST_angle65536 		= (((0-ii))&(65536-1)); 			/* 角度 */
			bullet_regist_angle();
		}
	}
}


/*---------------------------------------------------------
	メイド秘技「操りドール」
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	パーフェクトフリーズ(枠つき12丸弾を全弾加速させる)
	-------------------------------------------------------
	加速弾(正確には等加速弾)なのに、加速弾に見えないで
	等速弾(正確には等速度弾、加速==0)に見えちゃう問題がある。

---------------------------------------------------------*/
#if 1
static void exchange_damnaku_sakuya_saikasoku(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_bullet_obj_type;/* 調べるタイプを受け取る */
//	int jj = 0;
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &obj44[ii];
	//	if (check_type == (s->type) )	/* 矢印の青弾か赤弾なら */
		if ((BULLET_OODAMA32_02_KIIRO & 0xfff8) == (s->type & 0xfff8) ) 	/* 枠つき12丸弾なら */
		{
//			jj++;
		//	jj &= 0x07; 	// 2 3 4 5	6 7 8 9
//			jj &= 0x03; 	// 3 4 5 6
		//	s_change_meirin_yajirusi_one(s);
		//	s->tra65536 					= t256(6);			/* 調整加速弾 */
		//	s->tra65536 					= (jj<<8)+t256(2);	/* (2 ... 9) 調整加速弾 */
//			s->tra65536 					= (jj<<8)+t256(3);	/* (3 ... 6) 調整加速弾 */
			s->tra65536 					= t256(3);			/* (3 ... 6) 調整加速弾 */
			/* (1は遅すぎたので除外) 9 だと速過ぎるかも */
		}
		if ((BULLET_KNIFE20_04_AOI & 0xfff8) == (s->type & 0xfff8) )	/* 青ナイフ弾なら */
		{
//			s->tra65536 					= (jj<<8)+t256(3);	/* (3 ... 6) 調整加速弾 */
			s->tra65536 					= t256(2);			/* (3 ... 6) 調整加速弾 */
		}
	}
}
#endif

/*---------------------------------------------------------
	パーフェクトフリーズ(枠つき12丸弾を全弾停止。現在の座標を新基点座標に変更する。向きを変える)
	-------------------------------------------------------

---------------------------------------------------------*/
#if 1
static void exchange_damnaku_sakuya_tomeru(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_bullet_obj_type;/* 調べるタイプを受け取る */
//	int add_angle = 0;
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &obj44[ii];
	//	if (check_type == (s->type) )	/* 矢印の青弾か赤弾なら */
		if ((BULLET_OODAMA32_01_AKA & 0xfff8) == (s->type & 0xfff8) )	/* [赤大弾]なら */
		{
		//	s_change_meirin_yajirusi_one(s);
			/* 弾の現在座標を、新基点座標とする。 */
			s->tx256 = (s->cx256);		/* 基点座標にセット */
			s->ty256 = (s->cy256);		/* 基点座標にセット */
			s->radius256 = t256(0); 	/* 基点座標になったので半径は 0 にする。 */
			/*	*/
			s->tra65536 					= t256(0);		/* 調整加速弾 */
			s->speed65536					= t256(0);		/* 弾速 */
			#if 0
		//	s->rotationCCW1024				= (ra_nd()&((1024)-1)); 			/* 向き */	/* 向きは乱数でない気がする */
			#else
			/* 少なくとも乱数よりは本物に近い。 が、全然違う。 */
			/* 加算固定値(KOTEITI_7777)以外の要因として、弾幕リストのサイズでループする為、弾幕リストのサイズを */
			/* 現在の 1024 から、本物と同じ 640 に減らせば、似る可能性はある。 */
//			add_angle += (KOTEITI_7777);
//			s->rotationCCW1024				= (add_angle>>6);			/* 向き */
			#endif
			s->type 						= (BULLET_OODAMA32_02_KIIRO);
		}
	}
}
#endif
#if 1
static SPRITE obj_work;

static void exchange_damnaku_sakuya_tuika(void)
{
	voice_play(VOICE16_BOSS_KYUPIN, TRACK04_TEKIDAN);
//
	br.BULLET_REGIST_speed256			= (t256(0));			/* 弾速 */
	br.BULLET_REGIST_div_angle65536 	= (int)(65536/(48));		/* 分割角度(1024[360/360度]を 48 分割) */	/* 1周をn分割した角度 */
	br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [青ナイフ弾] */
	br.BULLET_REGIST_n_way				= (1);/* [nway] */
	br.BULLET_REGIST_speed_offset		= t256(0);/*てすと*/
//
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
	obj_work.cx256					= (obj_send1->cx256);				/* 弾源x256 */
	obj_work.cy256					= (obj_send1->cy256);				/* 弾源y256 */
//	/*const*/ SPRITE *obj_work_ptr = &obj_work;

//	int check_type;
//	check_type = br.BULLET_REGIST_bullet_obj_type;/* 調べるタイプを受け取る */
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &obj44[ii];
	//	if (check_type == (s->type) )	/* 矢印の青弾か赤弾なら */
		if ((BULLET_OODAMA32_02_KIIRO & 0xfff8) == (s->type & 0xfff8) ) 	/* [黄大弾]なら */
		{
		//	s_change_meirin_yajirusi_one(s);
			/* 弾の現在座標にナイフ弾を登録する。 */
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
			obj_send1->cx256					= (s->cx256);				/* 弾源x256 */
			obj_send1->cy256					= (s->cy256);				/* 弾源y256 */
			//
		//	tmp_angleCCW65536_src_nerai(obj_player, obj_send1);/*自機ねらい角作成*/
			tmp_angleCCW65536_src_nerai(&obj_work, s);/*自機ねらい角作成*/
			//
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
			br.BULLET_REGIST_angle65536 		= s->tmp_angleCCW65536;/*自機ねらい角*/
			bullet_regist_angle();
		}
	}
}
#endif

/*
	幻幽「ジャック・ザ・ルドビレ」の研究。
	このスペカは始めに[赤大弾]をランダムっぽくばら撒く。
	このばら撒きは、何て事のないいつもの東方ばら撒きなんだけど。
	どういう仕組みなのかさっぱりわからんので、研究してみる。
	まず、ばら撒いた処で適当にスクショ撮影。
	画面に2/3位しか弾が入らない。
	ここでチマチマ数を数えると20弾～26弾くらい。
	画面半分程で16～18弾くらいなので、多分全部で32弾～36弾なのでは？と推測。
	32弾、36弾。どっちもありそうなので何度か数えてみる。よく判らないが32弾っぽい。
	今ゲームフィールドが384x448, 咲夜さんの座標が291x49,
	判る限りの座標を目視(画像中心座標)＋お絵かきソフトで計測する事にした。
	大弾なので、お絵かきソフトで内接円になるように四角を描く。そのあと対角線に線を描いて中心座標を割り出す。
	----------------
	番号,画像中心座標,	相対座標
	 1弾目が 94x 87,	-(291- 94)x 87-49,	==(-197,+38)
	 2弾目が171x 69,	-(291-171)x 69-49,	==(-120,+20)
	 3弾目が192x 65,	-(291-192)x 65-49,	==(- 99,+16)
	 4弾目が151x119,	-(291-151)x119-49,	==(-140,+70)
	 5弾目が181x116,	-(291-181)x116-49,	==(-110,+67)
	 6弾目が221x 80,	-(291-221)x 80-49,	==(- 70,+31)
	 7弾目が175x139,	-(291-175)x139-49,	==(-116,+90)
	 8弾目が159x153,	-(291-159)x153-49,	==(-132,+104)
	 9弾目が210x152,	-(291-210)x152-49,	==(- 81,+103)
	10弾目が258x119,	-(291-258)x119-49,	==(- 33,+70)
	11弾目が255x152,	-(291-255)x152-49,	==(- 36,+103)
	12弾目が215x178,	-(291-215)x178-49,	==(- 76,+129)
	13弾目が254x170,	-(291-254)x170-49,	==(- 37,+121)
	14弾目が206x236,	-(291-206)x236-49,	==(- 85,+187)
	15弾目が233x273,	-(291-233)x273-49,	==(- 58,+224)
	16弾目が280x255,	-(291-280)x255-49,	==(- 11,+206)
	------------------この辺が半分っぽい。
	ここまで測定。
	相対座標から角度を割り出してみる。よくわかんないけど、
	tan(197/38)で角度が出るのかな？
//							(x/y)									tan(x/y)	たぶん角度
	 1弾目が(-197,+38)		5.18421052631578947368421052631579		0.0907292755019070511126536298905576
	 2弾目が(-120,+20)		6										0.105104235265676462511502380139882
	 3弾目が(- 99,+16)		6.1875									0.108414028737611378659694904675177
	 4弾目が(-140,+70)		2										0.0349207694917477305004026257737253
	 5弾目が(-110,+67)		1.64179104477611940298507462686567		0.0286625046177796138852122880504812
	 6弾目が(- 70,+31)		2.25806451612903225806451612903226		0.0394310774284007674631847355929523
	 7弾目が(-116,+90)		1.28888888888888888888888888888889		0.0224991500955683949518891752039385
	 8弾目が(-132,+104) 	1.26923076923076923076923076923077		0.0221558801384605992607822428428949
	 9弾目が(- 81,+103) 	0.786407766990291262135922330097087 	0.0137262667569302851091532667488299
	10弾目が(- 33,+70)		0.471428571428571428571428571428571 	0.00822816644161695626063183252739627
	11弾目が(- 36,+103) 	0.349514563106796116504854368932039 	0.00610025557803738263689086699115253
	12弾目が(- 76,+129) 	0.589147286821705426356589147286822 	0.010282922344805178445884641481058
	13弾目が(- 37,+121) 	0.305785123966942148760330578512397 	0.00533700788847012290939546575203758
	14弾目が(- 85,+187) 	0.454545454545454545454545454545455 	0.00793348122027169612990511645481441
	15弾目が(- 58,+224) 	0.258928571428571428571428571428571 	0.00451918686372985895525414665154542
	16弾目が(- 11,+206) 	0.0533980582524271844660194174757282	0.000931972200504819213720633933431873
//
	Windowsの電卓を使ってみる。
	Windowsの電卓でtan(1)は0.0174550649282175857651288952197278だそうな。
	tan(1)は(45度)の筈だから、ラジアン(１周が2πの単位系)として
	(2*pai)/360==tan(1)？？？。よくわからんな。
	どういうtanなのかよくわからない。
	tan(99)==-6.31375151467504309897946424476819()
	tan(90)==エラー無効な値です。
	tan(89)==57.2899616307594246872781475371126
	tan(88)==28.6362532829156035507565093209464
	tan(55)==1.4281480067421145021606184849985
//
わかった。arc_tan()じゃないと駄目だ。	Windowsの電卓は、tan(360度の角度)だわ。
	----------------
とにかく円状に広がらないので、BULLET_REGIST_speed_offsetに差がかなりあるのかも？
*/
/*---------------------------------------------------------
	幻幽「ジャック・ザ・ルドビレ」になるかもしれないテスト
---------------------------------------------------------*/
static void danmaku_create_14_sakuya_jack_oodama32(SPRITE *src)
{
	if (96==(src->boss_base_danmaku_time_out))	// 192== (64*3)
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
			obj_send1->cx256						= (src->cx256); 	/* 弾源x256 */
			obj_send1->cy256						= (src->cy256); 	/* 弾源y256 */
	//		br.BULLET_REGIST_div_angle65536 		= (0);									/* ダミー分割角度(未使用) */
			br.BULLET_REGIST_bullet_obj_type		= BULLET_OODAMA32_01_AKA;				/* [赤大弾] */
			br.BULLET_REGIST_n_way					= (1);									/* [1way] */
		//	br.BULLET_REGIST_regist_type			= REGIST_TYPE_00_MULTI_VECTOR;		/* 現在これしかないが要る */
	//
		int ii; 	/* ii */
	//	int jj; 	/* jj */
		int kk; 	/* カウンタ */
	//	int mm; 	/* カウンタ */
		kk=0;
	//	mm=0;
	//	jj=(1024-960);
		const u32 ra_nd32 = ra_nd();
		for (ii=0; ii<(32); ii ++)/* 32弾ばら撒く */
		{
			/* 960==30720/32,  30720 == 32768 -1024 -1024 */
			/* 936 = 936.228571428571428571428571428571 == (32768/(32+2左右の余裕分))	*/
			//	jj += (960);/* (=936)< 1024 [180度(32768)より若干狭い範囲にばら撒く] */
			//	kk += 386;
				kk += ra_nd32;
			//	kk += 42857142;
			//	mm += 42857142;
			//	br.BULLET_REGIST_speed_offset			= t256(0);/*てすと*/
			//	br.BULLET_REGIST_speed_offset			= t256(8);/*てすと*/
			//	br.BULLET_REGIST_speed_offset			= t256(12);/*てすと*/
				br.BULLET_REGIST_speed_offset			= (t256(4)+((kk&0x07)<<8));/*てすと*/
			//	br.BULLET_REGIST_speed256			= ((kk)&(512-1))+t256(0.25);	/*奇数は高速弾*/
				br.BULLET_REGIST_speed256			= ((kk)&(1024-1))+t256(0.25);	/*奇数は高速弾*/
				br.BULLET_REGIST_angle65536 		= ((kk)&(32768-1))+(int)(65536*3/4);			/* 角度 */
			//	br.BULLET_REGIST_angle65536 		= ((mm)&(1024-1))+jj+(int)(65536*3/4);			/* 角度 */
				bullet_regist_angle();	/* 角度弾として登録 */
		}
	}
	else
	if (64==(src->boss_base_danmaku_time_out))	// 192== (64*3)
	{
//		// 48 フレーム経過後、全てストップさせ、白くし、カウントリセット
		exchange_damnaku_sakuya_tomeru();/*全てストップ*/
	}
	else
	if (56==(src->boss_base_danmaku_time_out))	// 192== (64*3)
	{
		obj_send1->cx256					= t256(0);					/* 弾源x256 */
		obj_send1->cy256					= t256(256);				/* 弾源y256 */
		exchange_damnaku_sakuya_tuika();/*ナイフ追加*/
	}
	else
	if (48==(src->boss_base_danmaku_time_out))	// 192== (64*3)
	{
		obj_send1->cx256					= t256(GAME_WIDTH); 		/* 弾源x256 */
		obj_send1->cy256					= t256(256);				/* 弾源y256 */
		exchange_damnaku_sakuya_tuika();/*ナイフ追加*/
	}
	else
	if (40==(src->boss_base_danmaku_time_out))	// 192== (64*3)
	{
		SPRITE *zzz_player;
		zzz_player = &obj00[FIX_OBJ_00_PLAYER];
		obj_send1->cx256					= zzz_player->cx256;			/* 弾源x256 */
		obj_send1->cy256					= zzz_player->cy256;			/* 弾源y256 */
		exchange_damnaku_sakuya_tuika();/*ナイフ追加*/
	}
	else
	if (32==(src->boss_base_danmaku_time_out))	// 192== (64*3)
	{
		exchange_damnaku_sakuya_saikasoku();/*再加速*/
	}

}
