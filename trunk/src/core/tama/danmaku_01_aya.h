
/*---------------------------------------------------------
 東方模倣風  ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	射命丸 文の弾幕を生成します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	文	通常攻撃1(とりあえず通常攻撃2のコピペ)
	-------------------------------------------------------
		赤(12)弾
		青(12)弾
		紫(12)弾
	-------------------------------------------------------
	とりあえず、まったく似てない。
	というか、まったく違う。
	「通常攻撃1」はよくわかんない。ので、色々変えちゃうと思う。
---------------------------------------------------------*/

static void danmaku_create_15_aya_misogi1(SPRITE *src)
{
//	if ((0x10)==((src->boss_base_danmaku_time_out)&0x1f))/* (16回に1回)(128なら計8回) */
	if ((0x40)==((src->boss_base_danmaku_time_out)&0x7f))/* (16回に1回)(128なら計8回) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
#if 0/*全方位弾*/
		{
			tmp_angleCCW65536_src_nerai(obj_player, src);/*自機狙い角作成*/
			br.BULLET_REGIST_speed256			= (t256(1.75)); 									/* 弾速 */	/* 3.5 2.5 2.0*/
			br.BULLET_REGIST_angle65536 		= (src->tmp_angleCCW65536); 								/* 自機狙い弾 */
			br.BULLET_REGIST_div_angle65536 	= (int)(65536/(6*8))/* HOUGA_12_RED_DIV_ANGLE */;	/*(int)(1024/(48))*/	/* 分割角度(1024[360/360度]を 48 分割) */	/* 1周をn分割した角度 */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KOME_01_AOI;//BULLET_KOME_02_AKA;								/* [赤色米弾] */
			br.BULLET_REGIST_n_way				= (6*8)/* HOUGA_08_RED_NUMS */; 		/*(48)*/				/* [48way] */	/* 発弾数 */
			br.BULLET_REGIST_speed_offset		= t256(0);/*てすと*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
			bullet_regist_angle();
		}
#endif
	}
//	if ((0x02)==((src->boss_base_danmaku_time_out)&0x03))/* (2回に1回)(8回毎に発弾) */
//	if ((0x08)==((src->boss_base_danmaku_time_out)&0x0f))/* (2回に1回)(8回毎に発弾) */
	if ((0x08)==((src->boss_base_danmaku_time_out)&0x0f))/* (2回に1回)(8回毎に発弾) */
	{
		static int aaa_angle65536;
		static int bbb_angle65536;
//		br.BULLET_REGIST_speed256			= (t256(1.75)); 										/* 弾速 */		/*3.5 2.5 2.0*/
//		br.BULLET_REGIST_speed256			= (t256(0.75)); 										/* 弾速 */		/*3.5 2.5 2.0*/
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/(2))/* HOUGA_04_YEL_DIV_ANGLE */; 		/*(int)(1024/(6))*/ 	/* 分割角度(1024[360/360度]を 6 分割) */	/* 1周をn分割した角度 */
		br.BULLET_REGIST_n_way				= (2)/* HOUGA_00_YEL_NUMS */;				/*(6)*/ 				/* [1way] */	/* 発弾数 */
//		br.BULLET_REGIST_speed_offset		= t256(0);/*てすと*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
//
	//	#define NUM_12_OF_CIRCLE_ONE	(12)/* 一周(原作は12弾) */
		#define NUM_12_OF_CIRCLE_ONE	(9)
	//	const unsigned int aaa = (((~(src->boss_base_danmaku_time_out))>>3)&0x1f);/* テキトーに大きくしてみる。 */
		const unsigned int aaa = (((~(src->boss_base_danmaku_time_out))>>3)&0x1f);/* テキトーに大きくしてみる。 */
		unsigned int jj;
		for (jj=0; jj<(65536);	jj += (65536/NUM_12_OF_CIRCLE_ONE))/* 一周 */
		{
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
		//	obj_send1->cx256 					= (src->cx256)+t256(25.0)+(sin65536((jj))*16);				/* 弾源x256 */
		//	obj_send1->cy256 					= (src->cy256)+t256(16.0)+(cos65536((jj))*16);				/* 弾源y256 */
		//	obj_send1->cx256 					= (src->cx256)+t256(25.0)+(sin65536((jj))*(16+(aaa)));				/* 弾源x256 */
		//	obj_send1->cy256 					= (src->cy256)+t256(16.0)+(cos65536((jj))*(16+(aaa)));				/* 弾源y256 */
			obj_send1->cx256 					= (src->cx256) +(sin65536((jj))*(16+(aaa)));				/* 弾源x256 */
			obj_send1->cy256 					= (src->cy256) +(cos65536((jj))*(16+(aaa)));				/* 弾源y256 */
		//ゆっくり
			// 順回り(下CCWだから、左回りCCW)
			br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU12_02_AOI;//BULLET_KOME_05_KIIRO;								/* [黄色米弾] */
			br.BULLET_REGIST_angle65536 		= (( (aaa_angle65536))&(65536-1));				/* 発射中心角度 / 特殊機能(自機狙い/他) */
			br.BULLET_REGIST_speed256			= (t256(0.75)); 										/* 弾速 */		/*3.5 2.5 2.0*/
			br.BULLET_REGIST_speed_offset		= t256(0);/*てすと*/
			bullet_regist_angle();
		//速い
			// 逆回り(下CCWだから、右回りCW)
			br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU12_01_AKA;//BULLET_KOME_05_KIIRO;								/* [黄色米弾] */
			br.BULLET_REGIST_angle65536 		= (( (bbb_angle65536))&(65536-1));				/* 発射中心角度 / 特殊機能(自機狙い/他) */
			br.BULLET_REGIST_speed256			= (t256(0.50)); 										/* 弾速 */		/*3.5 2.5 2.0*/
			br.BULLET_REGIST_speed_offset		= t256(1);/*てすと*/
			bullet_regist_angle();
		}
		// 回転量
		aaa_angle65536 += (int)(	  (65536/(22)));/* HOUGA_16_YEL_ROTATE_ANGLE */ 						/*(1024/(6*8))*/		/* 角度(1024[360/360度]を 48分割) */
		bbb_angle65536 += (int)(65536-(65536/(15)));/* HOUGA_16_YEL_ROTATE_ANGLE */ 						/*(1024/(6*8))*/		/* 角度(1024[360/360度]を 48分割) */
	}
}


/*---------------------------------------------------------
	文	通常攻撃2
	-------------------------------------------------------
	normal
		赤(12)弾	CW弾速速い								右下45度から開始
		青(12)弾	CCW弾速遅い(青弾の2.5倍くらい？)		左下45度から開始
	-------------------------------------------------------
	luna
		赤(12)弾	CW弾速速い								右下45度から開始
		青(12)弾	CCW弾速遅い(青弾の1.5倍くらい？)		左下45度から開始
	-------------------------------------------------------
	???。
---------------------------------------------------------*/

static void danmaku_create_23_aya_misogi2(SPRITE *src)
{
//	if ((0x10)==((src->boss_base_danmaku_time_out)&0x1f))/* (16回に1回)(128なら計8回) */
	if ((0x40)==((src->boss_base_danmaku_time_out)&0x7f))/* (16回に1回)(128なら計8回) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	}
//	if ((0x02)==((src->boss_base_danmaku_time_out)&0x03))/* (2回に1回)(8回毎に発弾) */
//	if ((0x08)==((src->boss_base_danmaku_time_out)&0x0f))/* (2回に1回)(8回毎に発弾) */
	if ((0x08)==((src->boss_base_danmaku_time_out)&0x0f))/* (2回に1回)(8回毎に発弾) */
	{
		static int aaa_angle65536;
		static int bbb_angle65536;
//		br.BULLET_REGIST_speed256			= (t256(1.75)); 										/* 弾速 */		/*3.5 2.5 2.0*/
//		br.BULLET_REGIST_speed256			= (t256(0.75)); 										/* 弾速 */		/*3.5 2.5 2.0*/
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/(2))/* HOUGA_04_YEL_DIV_ANGLE */; 		/*(int)(1024/(6))*/ 	/* 分割角度(1024[360/360度]を 6 分割) */	/* 1周をn分割した角度 */
		br.BULLET_REGIST_n_way				= (2)/* HOUGA_00_YEL_NUMS */;				/*(6)*/ 				/* [1way] */	/* 発弾数 */
//		br.BULLET_REGIST_speed_offset		= t256(0);/*てすと*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
//
	//	#define NUM_12_OF_CIRCLE_ONE	(12)/* 一周(原作は12弾) */
		#define NUM_12_OF_CIRCLE_ONE	(9)
		unsigned int jj;
		for (jj=0; jj<(65536);	jj += (65536/NUM_12_OF_CIRCLE_ONE))/* 一周 */
		{
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
		//	obj_send1->cx256 					= (src->cx256)+t256(25.0)+(sin65536((jj))*16);				/* 弾源x256 */
		//	obj_send1->cy256 					= (src->cy256)+t256(16.0)+(cos65536((jj))*16);				/* 弾源y256 */
			obj_send1->cx256 					= (src->cx256) +(sin65536((jj))*16);				/* 弾源x256 */
			obj_send1->cy256 					= (src->cy256) +(cos65536((jj))*16);				/* 弾源y256 */
		// [ゆっくり]
			// 順回り(下CCWだから、左回りCCW)
			br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU12_02_AOI;//BULLET_KOME_05_KIIRO;								/* [黄色米弾] */
			br.BULLET_REGIST_angle65536 		= (( (aaa_angle65536))&(65536-1));							/* 発射中心角度 / 特殊機能(自機狙い/他) */
			br.BULLET_REGIST_speed256			= (t256(0.75)); 											/* 弾速 */		/*3.5 2.5 2.0*/
			br.BULLET_REGIST_speed_offset		= t256(0);/*てすと*/
			bullet_regist_angle();
		// [速い]
			// 逆回り(下CCWだから、右回りCW)
			br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU12_01_AKA;//BULLET_KOME_05_KIIRO;								/* [黄色米弾] */
			br.BULLET_REGIST_angle65536 		= (( (bbb_angle65536))&(65536-1));							/* 発射中心角度 / 特殊機能(自機狙い/他) */
			br.BULLET_REGIST_speed256			= (t256(0.50)); 											/* 弾速 */		/*3.5 2.5 2.0*/
			br.BULLET_REGIST_speed_offset		= t256(1);/*てすと*/
			bullet_regist_angle();
		}
		// 回転量
		aaa_angle65536 += (int)(	  (65536/(22)));/* HOUGA_16_YEL_ROTATE_ANGLE */ 						/*(1024/(6*8))*/		/* 角度(1024[360/360度]を 48分割) */
		bbb_angle65536 += (int)(65536-(65536/(15)));/* HOUGA_16_YEL_ROTATE_ANGLE */ 						/*(1024/(6*8))*/		/* 角度(1024[360/360度]を 48分割) */
	}
}













/*---------------------------------------------------------
	文	岐符「天の八衢」
	-------------------------------------------------------
	６層x32方向弾
	-------------------------------------------------------
	初期展開時、下側左右22.5/360度[1周/16分割]の範囲は弾がこない。

		 /|X
		/ | X
	   /  |  X
	  /   |   X
	 /	  |    X
	/	  | 	X
	22.5  | 22.5
	弾がこない範囲 == [1周/16分割] + [1周/16分割]
	原作のスクショから測るとこう。
	-------------------------------------------------------
	だけど、pspだと角度が広く感じる。
	上下切れてる上に(ゲームフィールドが)横広の画面だから、錯視が働くんだろうな。
	錯視(目の錯覚):
	同じ人が縦縞の服と横縞の服を着た場合、縦縞の服の方が(全体が)細く見える。
	同じ弾幕でも左右トリミングしてカットすると、目の錯覚で細く見える。
	同じ弾幕でも上下トリミングしてカットすると、目の錯覚で太く見える。
	この場合(psp)両方同時に働くから、目の錯覚で極端に太く見える。
	-------------------------------------------------------
	だからといって、角度を狭めたり、全体を縮小して調整すると、余計無理ゲに近づく気がする。
	個人的には、「横避(よ)け」が一番やりたかったんだか、pspの画面狭いから、現状(r32)横避(よ)けは無理ゲっぽい。
	横避(よ)けは無理ゲっぽいが、横避(さ)けはできる。
	-------------------------------------------------------
	横避(よ)け:横に張り付きながら、文の体力を削る攻略法。(誘導系以外を想定。誘導系以外で攻撃できる)
	横避(さ)け:単に弾を避(さ)けるだけ、攻撃できない。誘導系なら攻撃できるかも知れないが想定外(現状の模倣風では出来ない)。
	この弾幕は単に弾を避(さ)けるだけなら簡単。
	-------------------------------------------------------
	現状(r32)では攻略もヘッタくれもなく直ぐ次に変わる。
---------------------------------------------------------*/

/*---------------------------------------------------------
	文	岐符「天の八衢」(枠つき12丸弾を全弾停止。現在の座標を新基点座標に変更する。向きを変える)
	-------------------------------------------------------
---------------------------------------------------------*/

static void exchange_damnaku_aya_tomeru(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_bullet_obj_type;/* 調べるタイプを受け取る */
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &obj44[ii];
	//	if (check_type == (s->type) )	/* 矢印の青弾か赤弾なら */
		if (is_tama_grouip08(BULLET_MARU12_00_SIRO) == is_tama_grouip08(s->type) )	/* 枠つき12丸弾なら */
		{
			s->tra65536 					= t256(0);		/* 調整加速弾 */
			s->speed65536					= t256(0);		/* 弾速 */
		}
	}
}


/*---------------------------------------------------------
	文	岐符「天の八衢」(枠つき12丸弾を全弾加速させる)
	-------------------------------------------------------
	加速弾(正確には等加速弾)なのに、加速弾に見えないで
	等速弾(正確には等速度弾、加速==0)に見えちゃう問題がある。

---------------------------------------------------------*/

static void exchange_damnaku_aya_saikasoku(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_bullet_obj_type;/* 調べるタイプを受け取る */
//	int jj = 0;
	int add_angle = 0;
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &obj44[ii];
	//	if (check_type == (s->type) )	/* 矢印の青弾か赤弾なら */
		if (is_tama_grouip08(BULLET_MARU12_00_SIRO) == is_tama_grouip08(s->type) )	/* 枠つき12丸弾なら */
		{
		//	s_change_meirin_yajirusi_one(s);
			/* 弾の現在座標を、新基点座標とする。 */
			s->tx256 = (s->cx256);		/* 基点座標にセット */
			s->ty256 = (s->cy256);		/* 基点座標にセット */
			s->radius256 = t256(0); 	/* 基点座標になったので半径は 0 にする。 */
			/*	*/
			#if 0
		//	s->rotationCCW1024				= (ra_nd()&((1024)-1)); 			/* 向き */	/* 向きは乱数でない気がする */
			#else
			/* 少なくとも乱数よりは本物に近い。 が、全然違う。 */
			/* 加算固定値(KOTEITI_7777)以外の要因として、弾幕リストのサイズでループする為、弾幕リストのサイズを */
			/* 現在の 1024 から、本物と同じ 640 に減らせば、似る可能性はある。 */
		//	#define KOTEITI_1234 (1234<<6)
			#define KOTEITI_1234 (78976)
			add_angle += (KOTEITI_1234);
		//	s->rotationCCW1024				= (add_angle>>6);			/* 向き */
		//	s->rotationCCW1024				= ((((add_angle>>6)&(512-1))+(256*(4-1)))&(1024-1));			/* 向き(下(180/360[度])にしか落ちない) */
			s->rotationCCW1024				= ((((add_angle>>6)&(256-1))+(128*(8-1)))&(1024-1));			/* 向き(下( 90/360[度])にしか落ちない) */
			#endif
			s->tra65536 					= t256(1);	/* (1) 調整加速弾 */
		//	チルノ(パーフェクトフリーズ)の場合は先に変身するが、文(天の八衢)の場合は後で変身する。
			s->type 						= (BULLET_MARU10_03_AOI);	/* 青丸弾 */
			reflect_sprite_spec444(s, OBJ_BANK_SIZE_00_TAMA);	/* 弾グラと弾あたり判定を変更する。 */
		}
	}
}

static void danmaku_create_22_aya_yatimata(SPRITE *src)
{
	if ((32*20)-1==((src->boss_base_danmaku_time_out) ))/* 約1.0[秒](==64flame)弾幕展開 */
	{
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		//
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
	//		obj_send1->cx256 					= (src->cx256)+t256(25.0);				/* 弾源x256 */
	//		obj_send1->cy256 					= (src->cy256)+t256(16.0);				/* 弾源y256 */
	//		obj_send1->cx256 					= (src->cx256)+t256(16.0);				/* 弾源x256 */
	//		obj_send1->cy256 					= (src->cy256)+t256(16.0);				/* 弾源y256 */
			obj_send1->cx256 					= (src->cx256);				/* 弾源x256 */
			obj_send1->cy256 					= (src->cy256);				/* 弾源y256 */
		//	obj_send1->rotationCCW1024			= (src->rotationCCW1024);				/* 弾源y256 */
		//
		/* 弾生成 */
		unsigned int j;
		for (j=0; j<6; j++ )/* ６層。 */
		{
			br.BULLET_REGIST_speed256				= (t256(0.5) ); 			/* 弾速(pspの画面は狭い) */

			br.BULLET_REGIST_angle65536 			= (65536/16)/* */;			/* 向き */
			br.BULLET_REGIST_div_angle65536 		= ((65536-(65536/8))/32);				/* 分割角度(弾がこない範囲を除き32分割) */	/* (ra_nd16>>4) == 乱数(0-4095) */
			br.BULLET_REGIST_bullet_obj_type		= BULLET_MARU12_00_SIRO;	/* 青丸弾 */

			br.BULLET_REGIST_n_way					= (32); 		/* [32way] */			/* 32方向弾 */

		//	br.BULLET_REGIST_speed_offset			= t256(-5); 		/* (-5) (-3)調整減速弾 */	/* この方式になるか検討中 */
		//	br.BULLET_REGIST_speed_offset			= t256(-j);/*てすと*/
		//	br.BULLET_REGIST_speed_offset			= t256(j<<2);/* x4倍 てすと*/
			br.BULLET_REGIST_speed_offset			= (j<<(2+8));/* x4倍 てすと*/
		//	br.BULLET_REGIST_regist_type			= REGIST_TYPE_00_MULTI_VECTOR;		/* 現在これしかないが要る */
			bullet_regist_angle();	/* 角度弾として登録 */
		}
	}
	else
	if ((32*18)==((src->boss_base_danmaku_time_out) ))/* 約0.33[秒](==20flame)停止 */
	{
		/* ここでは効果音なし */
		/* 全弾停止 */
		exchange_damnaku_aya_tomeru();/*全てストップ*/
	}
	else
	if (/*(32*17)*/((32*18)-20)==((src->boss_base_danmaku_time_out) ))/* 止まってから20flame後 */
	{
		/* 止まった時よりも、動き始める時に効果音鳴らした方がそれっぽい。 */
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
	//	bullet_play_04_auto(VOICE11_BOSS_KIRARIN);		/* 効果音: きらりん */
		voice_play(VOICE11_BOSS_KIRARIN, TRACK03_SHORT_MUSIC);		/* 効果音: きらりん */
		voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);			/* 効果音: きらりん */
		/* 大事な事なので２回言いました。 */
		#endif
		/* 移動 */
		exchange_damnaku_aya_saikasoku();/*再加速*/
	}

}



/*---------------------------------------------------------
	塞符c「山神渡御」(normal)
	塞符「天上天下の照國」(luna)
	-------------------------------------------------------
	ぜんぜん作ってない...
---------------------------------------------------------*/
// 4096 == 65536/16
// 3855.05882352941176470588235294118 == 65536/17
//	240.941176470588235294117647058824 == ((65536/16)-(16636/17)) 差分
static void danmaku_create_26_aya_saifu(SPRITE *src)
{
	static unsigned int zzz;
	static	 signed int ggg;
	if ((DANMAKU_9999_TIME-1)==((src->boss_base_danmaku_time_out) ))/* 初期化 */
	{
		zzz = 64;
		ggg = 0;
	}
	ggg--;
	if (0 > ggg)
	{
		ggg = zzz;
		#define min_24zzz (26)
		zzz -= difficulty;
		if (min_24zzz > zzz)		{	zzz = min_24zzz;		}
		#undef min_24zzz
	{
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
		obj_send1->cx256 					= (src->cx256);				/* 弾源x256 */
		obj_send1->cy256 					= (src->cy256);				/* 弾源y256 */
	//	obj_send1->cx256 					= (src->cx256)+t256(25.0);				/* 弾源x256 */
	//	obj_send1->cy256 					= (src->cy256)+t256(16.0);				/* 弾源y256 */
		//
//		br.BULLET_REGIST_speed256			= (t256(1.00)); 										/* 弾速 */
		br.BULLET_REGIST_speed256			= (t256(0.50)); 										/* 弾速 */
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/(16*5));	/* 分割角度(65536[360/360度]を 8 分割) */	/* 1周をn分割した角度 */
		br.BULLET_REGIST_angle65536 		= ((0));				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		{	const unsigned int cc_type[4] =
			{
				BULLET_MARU8_05_MIDORI,
				BULLET_MARU8_00_SIRO, 			// 塞符「山神渡御」(normal)
				BULLET_KOME_06_KI_IRO,
				BULLET_KOME_06_KI_IRO,			// 塞符「天上天下の照國」(luna)
			};
			br.BULLET_REGIST_bullet_obj_type	= cc_type[difficulty];								/* [ 弾] */
		}
		br.BULLET_REGIST_n_way				= (16*5);			/*(6)*/ 				/* [1way] */	/* 発弾数 */
//		br.BULLET_REGIST_speed_offset		= t256(0);/*てすと*/
		br.BULLET_REGIST_speed_offset		= t256(1);/*てすと*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
		bullet_regist_angle();
	}
	}
//	if ((0x02)==((src->boss_base_danmaku_time_out)&0x03))/* (2回に1回)(8回毎に発弾) */
	if ((0x04)==((src->boss_base_danmaku_time_out)&0x07))/* (2回に1回)(8回毎に発弾) */
	{
		static unsigned int bbb;
		bbb += 77;
		static unsigned int aaa;
		aaa++;
		aaa &= (0x07);
		if (0==aaa) 	{	aaa++;	};
		//
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
		obj_send1->cx256 					= (src->cx256);				/* 弾源x256 */
		obj_send1->cy256 					= (src->cy256);				/* 弾源y256 */
	//	obj_send1->cx256 					= (src->cx256)+t256(25.0);				/* 弾源x256 */
	//	obj_send1->cy256 					= (src->cy256)+t256(16.0);				/* 弾源y256 */
		//
//		br.BULLET_REGIST_speed256			= (t256(1.00)); 										/* 弾速 */
		br.BULLET_REGIST_speed256			= (t256(0.50)); 										/* 弾速 */
	//	br.BULLET_REGIST_div_angle65536 	= (int)(65536/(16))-(((sin1024(( ra_nd() & src->boss_base_danmaku_time_out)))&0xff));		/*(int)(1024/(6))*/ 	/* 分割角度(1024[360/360度]を 6 分割) */	/* 1周をn分割した角度 */
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/(16/*8*/));	/* 分割角度(65536[360/360度]を 8 分割) */	/* 1周をn分割した角度 */
//		br.BULLET_REGIST_angle65536 		= ((65536/2));				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		{	const unsigned int ba_type[4] =
			{
				BULLET_KOME_03_AOI,
				BULLET_KOME_03_AOI, 		// 塞符「山神渡御」(normal)
				BULLET_UROKO14_05_KIIRO,
				BULLET_UROKO14_05_KIIRO,	// 塞符「天上天下の照國」(luna)
			};
			br.BULLET_REGIST_bullet_obj_type	= ba_type[difficulty];								/* [ 弾] */
		}
	//	br.BULLET_REGIST_n_way				= (17); 			/*(6)*/ 				/* [1way] */	/* 発弾数 */
		br.BULLET_REGIST_n_way				= (16/*8*/);			/*(6)*/ 				/* [1way] */	/* 発弾数 */
//		br.BULLET_REGIST_speed_offset		= t256(0);/*てすと*/
		br.BULLET_REGIST_speed_offset		= t256(1);/*てすと*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
//
//		br.BULLET_REGIST_angle65536 		= (int)((65536*1/4))+(src->boss_base_danmaku_time_out&0xff);				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		br.BULLET_REGIST_angle65536 		= ((bbb));				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		if (0==((src->boss_base_danmaku_time_out)&0x08))
		{
//			br.BULLET_REGIST_angle65536 		= (int)((65536*3/4))+(src->boss_base_danmaku_time_out&0xff);				/* 発射中心角度 / 特殊機能(自機狙い/他) */
			br.BULLET_REGIST_angle65536 		= -(bbb);				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		}
		bullet_regist_angle();
	}
}

/*---------------------------------------------------------
	文	危険な種弾	人形弾幕
	-------------------------------------------------------
	テキトー
---------------------------------------------------------*/
extern void add_zako_aya_doll(SPRITE *src);/* 文人形弾幕 */
static void danmaku_create_0f_aya_doll(SPRITE *src)
{
	if (50==((src->boss_base_danmaku_time_out) ))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_aya_doll(src);
	}
}

#if 0
/*---------------------------------------------------------
	大玉弾
---------------------------------------------------------*/

static void bullet_create_aya_kougeki_03(SPRITE *src)
{
	s_bullet_create_aya_oodama3(src, /*仕様変更*/(t256(4.0)+(difficulty<<7))/*, 10*/);
}


/*---------------------------------------------------------
	危険な種弾
---------------------------------------------------------*/

static void bullet_create_aya_kougeki_02(SPRITE *src)
{
	add_zako_aya_doll(src);
};
#endif

/*---------------------------------------------------------
	雨降り弾
	-------------------------------------------------------
---------------------------------------------------------*/

static void danmaku_create_2a_amefuri_test(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x03))	// 4カウントに1回上に8way弾を撃つ
	{
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
		obj_send1->cx256 						= (src->cx256);
		obj_send1->cy256 						= (src->cy256);
	//	obj_send1->cx256 						= (src->cx256)+t256(25.0);
	//	obj_send1->cy256 						= (src->cy256)+t256(50.0);
		//
		unsigned int j;
		for (j=(0); j<(8); j++)
		{
			/* ショット */
		//	b05_fire_flags &= (~(doll_data->identity_bit));/* off */
			br.BULLET_REGIST_speed256				= (t256(1.5)+(difficulty<<6));
			br.BULLET_REGIST_angle1024				= (1024/4)+(1024/8)+(j<<5)+((src->boss_base_danmaku_time_out>>2)&0x3f);
//			br.BULLET_REGIST_div_angle1024			= (int)(1024/64);
//	//		br.BULLET_REGIST_bullet_obj_type		= BULLET_CAP16_04_KOME_SIROI;			/* [青白米弾] */	/* 弾グラ */
	//		br.BULLET_REGIST_n_way					= (8);					/*(4-difficulty)*/
//	//		br.BULLET_REGIST_regist_type			= REGIST_TYPE_02_GRAVITY02;
			br.BULLET_REGIST_jyuryoku_dan_delta256	= ((ra_nd()&0x03)+2);//t256(0.04)
			br.BULLET_REGIST_bullet_obj_type		= (BULLET_KOME_03_AOI); 	/* 弾グラ */
			br.BULLET_REGIST_regist_type			= REGIST_TYPE_02_GRAVITY02;
			bullet_regist_vector();
		}
	}
}
