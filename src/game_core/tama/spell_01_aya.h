
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	射命丸 文のカードを定義します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	文	通常攻撃1
	-------------------------------------------------------
		赤(12)弾
		青(12)弾
		紫(12)弾
	-------------------------------------------------------
	とりあえず、まったく似てない。
	というか、まったく違う。
	「通常攻撃1」はよくわかんない。ので、色々変えちゃうと思う。
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。
	REG_09_REG1 	aaa_angle65536
	REG_0a_REG2 	bbb_angle65536
	REG_0b_REG3 	だんだん大きくなる円の半径。
	REG_0c_REG4 	音用カウンタ。(48回に1回) 発音。
	-------------------------------------------------------
	REG_0d_REG5 	x位置/退避/復旧
	REG_0e_REG6 	y位置/退避/復旧
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_15_aya_misogi1)
{
//	if ((0x08)==((REG_10_BOSS_SPELL_TIMER)&0x0f))/* (16回に1回)(16回毎に発弾) */
	count_up_limit_NUM(REG_NUM_08_REG0, 16);//	/* (16回に1回)(16回毎に発弾) */
	if (1==REG_08_REG0) /* (16回に1回) 発弾 */
	{
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
//
	//	#define NUM_12_OF_CIRCLE_ONE	(12)/* 一周(原作は12弾) */
		#define NUM_12_OF_CIRCLE_ONE	(9)
	//	const unsigned int REG_0b_REG3 = (((~(REG_10_BOSS_SPELL_TIMER))>>3)&0x1f);/* テキトーに大きくしてみる。 */
		count_up_limit_NUM(REG_NUM_0b_REG3, 24);//		/*(r34, 32)*/
		//------------------
		#if (0)//
		// 退避
		REG_0d_REG5 = REG_02_DEST_X;
		REG_0e_REG6 = REG_03_DEST_Y;
		//------------------
		#endif
		unsigned int jj;
		for (jj=0; jj<(65536); jj+=(65536/NUM_12_OF_CIRCLE_ONE))/* 一周 */
		{
		//	REG_02_DEST_X += (sin65536((jj))*(16)); 			/* 弾源x256 */
		//	REG_03_DEST_Y += (cos65536((jj))*(16)); 			/* 弾源y256 */
			#if (1)//
		//	REG_02_DEST_X += ((si n65536((jj))*((16+(REG_0b_REG3)))));/*fps_factor*/	/* 弾源x256 */	/* CCWの場合 */
		//	REG_03_DEST_Y += ((co s65536((jj))*((16+(REG_0b_REG3)))));/*fps_factor*/	/* 弾源y256 */
		//	REG_02_DEST_X += ((si n1024((deg65536to1024(jj)))*((16+(REG_0b_REG3)))));/*fps_factor*/ 	/* 弾源x256 */	/* CCWの場合 */
		//	REG_03_DEST_Y += ((co s1024((deg65536to1024(jj)))*((16+(REG_0b_REG3)))));/*fps_factor*/ 	/* 弾源y256 */
			{
				int sin_value_t256; 	//	sin_value_t256 = 0;
				int cos_value_t256; 	//	cos_value_t256 = 0;
				int256_sincos1024( (deg65536to1024(jj)), &sin_value_t256, &cos_value_t256); 	/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
				REG_02_DEST_X += ((sin_value_t256*((12+(REG_0b_REG3)))));/*fps_factor*/ 	/*(r34, 16)*/
				REG_03_DEST_Y += ((cos_value_t256*((12+(REG_0b_REG3)))));/*fps_factor*/ 	/*(r34, 16)*/
			}
			#else/*(ダメ)*/
			//------------------
			HATSUDAN_01_speed256	= ((12+(REG_0b_REG3))<<8);
			HATSUDAN_03_angle65536	= (((jj)));
			sincos256();/*(破壊レジスタ多いので注意)*/
		//	REG_09_REG1 -= ( ((REG_03_DEST_Y))); 	/* 弾速 */
		//	h->center.y256 = (REG_02_DEST_X);/*fps_factor*/
			//------------------
			//------------------
			// 復旧
			REG_02_DEST_X = REG_0d_REG5 + (REG_03_DEST_Y);
			REG_03_DEST_Y = REG_0e_REG6 + (REG_02_DEST_X);
			//------------------
			#endif
			unsigned int ii;
			for (ii=0; ii<(65536); ii+=(65536/2))/* 一周 */
			{
			// ゆっくり
				// 順回り(下CCWだから、左回りCCW)
				HATSUDAN_01_speed256			= (t256(0.75)); 							/* 弾速 */		/*3.5 2.5 2.0*/
				HATSUDAN_02_speed_offset		= t256(0);/*(テスト)*/
				HATSUDAN_03_angle65536			= (((REG_09_REG1)+ii)&(65536-1));			/* 発射中心角度 / 特殊機能(自機狙い/他) */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_03_AOI);// 				/* [枠付き青丸弾] */
				hatudan_system_regist_single();/* (r33-) */
			// 速い
				// 逆回り(下CCWだから、右回りCW)
				HATSUDAN_01_speed256			= (t256(0.50)); 							/* 弾速 */		/*3.5 2.5 2.0*/
				HATSUDAN_02_speed_offset		= t256(1);/*(テスト)*/
				HATSUDAN_03_angle65536			= (((REG_0a_REG2)+ii)&(65536-1));			/* 発射中心角度 / 特殊機能(自機狙い/他) */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_01_AKA);// 				/* [枠付き 丸弾] */
				hatudan_system_regist_single();/* (r33-) */
			}
		}
		// 回転量
		REG_09_REG1 += (int)(	   (65536/(22)));/* ROTATE_ANGLE */				/*(1024/(6*8))*/		/* 角度(1024[360/360度]を 48分割) */
		REG_0a_REG2 += (int)(65536-(65536/(15)));/* ROTATE_ANGLE */				/*(1024/(6*8))*/		/* 角度(1024[360/360度]を 48分割) */
	// 音用
	//	if ((0x10)==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* (32回に1回発音) */
	//	if ((0x40)==((REG_10_BOSS_SPELL_TIMER)&0x7f))/* (128回に1回発音) */
	//	count_up_limit_NUM(REG_NUM_0c_REG4, 8);//	/* (8回に1回) ((16x8)==128回に1回発音) */
		count_up_limit_NUM(REG_NUM_0c_REG4, 3);//	/* (2回に1回) ((16x3)==48回に1回発音) */
		if (1==REG_0c_REG4) /* (48回に1回) 発音 */
		{
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
			#endif
		}
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
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。
	REG_09_REG1 	aaa_angle65536
	REG_0a_REG2 	bbb_angle65536
	REG_0c_REG4 	音用カウンタ。(40回に1回) 発音。
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_23_aya_misogi2)
{
//	if ((0x04)==((REG_10_BOSS_SPELL_TIMER)&0x07))/* (8回に1回 発弾) */
	count_up_limit_NUM(REG_NUM_08_REG0, 8);//	/* (8回に1回 発弾) */
	if (1==REG_08_REG0) /* (8回に1回) 発弾 */
	{
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
//
	//	#define NUM_12_OF_CIRCLE_ONE	(12)/* 一周(原作は12弾) */
		#define NUM_12_OF_CIRCLE_ONE	(9)
		unsigned int jj;
		for (jj=0; jj<(65536);	jj += (65536/NUM_12_OF_CIRCLE_ONE))/* 一周 */
		{
		//	REG_02_DEST_X += (sin65536((jj))*(16)); 			/* 弾源x256 */
		//	REG_03_DEST_Y += (cos65536((jj))*(16)); 			/* 弾源y256 */
			#if (0)//
			REG_02_DEST_X += ((si n1024((deg65536to1024(jj)))*(16)));/*fps_factor*/ 	/* 弾源x256 */	/* CCWの場合 */
			REG_03_DEST_Y += ((co s1024((deg65536to1024(jj)))*(16)));/*fps_factor*/ 	/* 弾源y256 */
			#else
			{
				int sin_value_t256; 	//	sin_value_t256 = 0;
				int cos_value_t256; 	//	cos_value_t256 = 0;
				int256_sincos1024( (deg65536to1024(jj)), &sin_value_t256, &cos_value_t256); 	/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
				REG_02_DEST_X += ((sin_value_t256*(12)));/*fps_factor*/ 	/*(r34, 16)*/
				REG_03_DEST_Y += ((cos_value_t256*(12)));/*fps_factor*/ 	/*(r34, 16)*/
			}
			#endif
		// [ゆっくり]
			// 順回り(下CCWだから、左回りCCW)
			HATSUDAN_01_speed256		= (t256(0.75)); 									/* 弾速 */		/*3.5 2.5 2.0*/
			HATSUDAN_02_speed_offset	= t256(0);/*(テスト)*/
			HATSUDAN_03_angle65536		= (( (REG_09_REG1))&(65536-1)); 				/* 発射中心角度 / 特殊機能(自機狙い/他) */
			HATSUDAN_05_bullet_obj_type = (BULLET_WAKU12_BASE + TAMA_IRO_03_AOI);// 						/* [枠付き青丸弾] */
			hatudan_system_regist_single();/* (r33-) */
		// [速い]
			// 逆回り(下CCWだから、右回りCW)
			HATSUDAN_01_speed256		= (t256(0.50)); 									/* 弾速 */		/*3.5 2.5 2.0*/
			HATSUDAN_02_speed_offset	= t256(1);/*(テスト)*/
			HATSUDAN_03_angle65536		= (( (REG_0a_REG2))&(65536-1)); 				/* 発射中心角度 / 特殊機能(自機狙い/他) */
			HATSUDAN_05_bullet_obj_type = (BULLET_WAKU12_BASE + TAMA_IRO_01_AKA);// 						/* [枠付き 丸弾] */
			hatudan_system_regist_single();/* (r33-) */
		}
		// 回転量
	//	REG_09_REG1 += (((int)( 	(65536/(22)))+((int)(65536/(2)))));/* ROTATE_ANGLE */				/*(1024/(6*8))*/		/* 角度(1024[360/360度]を 48分割) */
	//	REG_0a_REG2 += (((int)(65536-(65536/(15)))+((int)(65536/(2)))));/* ROTATE_ANGLE */ 			/*(1024/(6*8))*/		/* 角度(1024[360/360度]を 48分割) */
	//	REG_09_REG1 += (((int)( 	(65536/(44)))+((int)(65536/(2)))));/* ROTATE_ANGLE */				/*(1024/(6*8))*/		/* 角度(1024[360/360度]を 48分割) */
	//	REG_0a_REG2 += (((int)(65536-(65536/(30)))+((int)(65536/(2)))));/* ROTATE_ANGLE */ 			/*(1024/(6*8))*/		/* 角度(1024[360/360度]を 48分割) */
		// 回転量が 22分割(但し半周毎, 実質44分割)の場合.
		REG_09_REG1 += ((int)(		  (65536/(44)) + (65536/(2))   ));		/* 角度(65536[360/360度]を 22分割、但し半周毎に描く ==	((65536/44)+(65536/2)) ) */
		// 回転量が 15分割(但し半周毎, 実質30分割)の場合.
		REG_0a_REG2 += ((int)( 65536-( (65536/(30)) + (65536/(2)) ) )); 	/* 角度(65536[360/360度]を 15分割、但し半周毎に描く == -((65536/30)+(65536/2)) ) */
	// 音用
	//	if ((0x10)==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* (32回に1回) */
	//	if ((0x40)==((REG_10_BOSS_SPELL_TIMER)&0x7f))/* (128回に1回) */
		count_up_limit_NUM(REG_NUM_0c_REG4, 5);//	/* (5回に1回) ((8x5)==40回に1回発音) */
		if (1==REG_0c_REG4) /* (40回に1回) 発音 */
		{
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
			#endif
		}
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
	同じカードでも左右トリミングしてカットすると、目の錯覚で細く見える。
	同じカードでも上下トリミングしてカットすると、目の錯覚で太く見える。
	この場合(psp)両方同時に働くから、目の錯覚で極端に太く見える。
	-------------------------------------------------------
	だからといって、角度を狭めたり、全体を縮小して調整すると、余計無理ゲに近づく気がする。
	個人的には、「横避(よ)け」が一番やりたかったんだか、pspの画面狭いから、現状(r32)横避(よ)けは無理ゲっぽい。
	横避(よ)けは無理ゲっぽいが、横避(さ)けはできる。
	-------------------------------------------------------
	横避(よ)け:横に張り付きながら、文の体力を削る攻略法。(誘導系以外を想定。誘導系以外で攻撃できる)
	横避(さ)け:単に弾を避(さ)けるだけ、攻撃できない。誘導系なら攻撃できるかも知れないが想定外(現状の模倣風では出来ない)。
	このカードは単に弾を避(さ)けるだけなら簡単。
	-------------------------------------------------------
	現状(r32)では攻略もヘッタくれもなく直ぐ次に変わる。
---------------------------------------------------------*/

/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(aya_danmaku_01_callback)/* 岐符「天の八衢」 */
{
	if (((32*18)-HATUDAN_FRAME64)==((REG_0a_REG2) ))/* 約0.33[秒](==20[frame])停止 */
	{
		/* ここでは効果音なし */
		/*(弾幕グループ(1)を全弾停止。向きを変える。)*/
		{
			src->hatudan_register_tra65536					= t256(0);		/* 調整加速弾 */
			src->hatudan_register_speed65536				= t256(0);		/* 弾速 */
		}
	}
	else
	if (((32*18)-HATUDAN_FRAME64-20)==((REG_0a_REG2) ))/* 止まってから20[frame]後 */ /*(32*17)*/
	{
		/* 移動。 再加速。 */
			/*(弾幕グループ(1)を全弾加速させる。)
				-------------------------------------------------------
				加速弾(正確には等加速弾)なのに、加速弾に見えないで、
				等速弾(正確には等速度弾、加速==0)に見えちゃう問題がある。
			*/
		{
			#if 1
			/* 擬似乱数==加算固定値(KOTEITI_AYA_1234) */
		//	#define KOTEITI_AYA_1234 (1234<<6)
		//	#define KOTEITI_AYA_1234 (78976)
			#define KOTEITI_AYA_1234 (7777)
			REG_09_REG1 += (KOTEITI_AYA_1234);
		//	src->rotationCCW1024				= (ra_nd()&((1024)-1)); 	/* 向き */	/* 向きは乱数でない気がする */
		//	src->rotationCCW1024				= (deg65536to1024(REG_09_REG1));			/* 向き */
		//	src->rotationCCW1024				= ((((deg65536to1024(REG_09_REG1))&(512-1))+(256*(4-1)))&(1024-1)); 		/* 向き(下(180/360[度])にしか落ちない) */
			src->rotationCCW1024				= ((((deg65536to1024(REG_09_REG1))&(256-1))+(128*(8-1)))&(1024-1)); 		/* 向き(下( 90/360[度])にしか落ちない) */
			#endif
			src->hatudan_register_tra65536			= t256(1);	/* (1) 調整加速弾 */
		/* チルノ(パーフェクトフリーズ)の場合は先に変身するが、文(天の八衢)の場合は後で変身する。 */
			src->obj_type_set							= (BULLET_MARU10_BASE + TAMA_IRO_03_AOI);	/* 青丸弾 */
			reflect_sprite_spec(src, OBJ_BANK_SIZE_00_TAMA); 	/* 弾グラと弾あたり判定を変更する。 */
			/* (通常弾へ変身する) */
			src->hatudan_register_spec_data 		= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		}
		/* 止まった時よりも、動き始める時に効果音鳴らした方がそれっぽい。 */
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
	//	bullet_play_04_auto(VOICE11_BOSS_KIRARIN);		/* 効果音: きらりん */
		voice_play(VOICE11_BOSS_KIRARIN, TRACK03_IVENT_DAN);		/* 効果音: きらりん */
		voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);			/* 効果音: きらりん */
		/* 大事な事なので２回言いました。 */
		#endif
	}
//	danmaku_00_standard_angle_mover(src);/*(角度弾移動+画面外弾消し)*/
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
}


/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_22_aya_yatimata)
{
	card.danmaku_callback[1] = aya_danmaku_01_callback;/*(岐符「天の八衢」用。)*/
//	card.danmaku_callback[2] = NULL;/*(未使用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}
/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。
//	REG_09_REG1 	add_angle
	REG_0a_REG2 	ボスタイマー値、コールバック側に連絡用。(とりあえず)
//	REG_0b_REG3 	再加速の速度用に一時使用。
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_22_aya_yatimata)
{
	REG_0a_REG2 = (REG_10_BOSS_SPELL_TIMER);/*(とりあえず)*/
	if ((32*20)-1==((REG_10_BOSS_SPELL_TIMER) ))/* 約1.0[秒](==64[frame])全弾を展開 */
	{
		/* 弾生成 */
			HATSUDAN_01_speed256				= (t256(0.5));			/* 弾速(pspの画面は狭い) */
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_WAKU12_BASE + TAMA_IRO_00_SIRO);		/* 白弾 */
		//
		unsigned int jj;
		for (jj=0; jj<6; jj++)/* ６層。 */
		{
		//	HATSUDAN_02_speed_offset			= t256(-5); 		/* (-5) (-3)調整減速弾 */	/* この方式になるか検討中 */
		//	HATSUDAN_02_speed_offset			= t256(-jj);/*(テスト)*/
		//	HATSUDAN_02_speed_offset			= t256(jj<<2);/* x4倍 てすと*/
			HATSUDAN_02_speed_offset			= (jj<<(2+8));/* x4倍 てすと*/
		//	HATSUDAN_03_angle65536				= (65536/16);			/* 向き */
			HATSUDAN_03_angle65536				= (65536/2);							/* [破壊] */	/* 真上向き */
			HATSUDAN_06_n_way					= (32); 								/* [破壊] */	/* [32way] */	/* 32方向弾 */
			HATSUDAN_07_div_angle65536			= /*(1792);*/((65536-(65536/8))/32);	/* [破壊] */	/* 分割角度(弾がこない範囲を除き32分割) */	/* (ra_nd16>>4) == 乱数(0-4095) */
			hatudan_system_regist_n_way();/* (r33-) */
		}
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
	}
}


/*---------------------------------------------------------
	塞符「山神渡御」(normal)
	塞符「天上天下の照國」(luna)
	-------------------------------------------------------
	ぜんぜん作ってない...
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。
	REG_09_REG1 	aaa
	REG_0a_REG2 	bbb
	REG_0b_REG3 	ggg
	REG_0c_REG4 	zzz
---------------------------------------------------------*/
// 4096 == 65536/16
// 3855.05882352941176470588235294118 == 65536/17
//	240.941176470588235294117647058824 == ((65536/16)-(16636/17)) 差分
local OBJ_CALL_FUNC(spell_create_26_aya_saifu)
{
	if ((SPELL_TIME_9999-1)==((REG_10_BOSS_SPELL_TIMER) ))/* 初期化 */
	{
		REG_0c_REG4 = (64);/* 64から始める。 */
		REG_0b_REG3 = 0;
	}
	REG_0b_REG3--;
	if (0 > REG_0b_REG3)
	{
		REG_0b_REG3 = REG_0c_REG4;
	//	#define min_24zzz (26)/* (r33) */
	//	#define min_24zzz (40)/* (r34)若干弾切れする。 */
		#define min_24zzz (44)/* (r34) */
		REG_0c_REG4 -= (REG_0f_GAME_DIFFICULTY);
		if (min_24zzz > REG_0c_REG4)		{	REG_0c_REG4 = min_24zzz;		}
		#undef min_24zzz
	{
//		HATSUDAN_01_speed256			= (t256(1.00)); 			/* 弾速 */
		HATSUDAN_01_speed256			= (t256(0.50)); 			/* 弾速 */
//		HATSUDAN_02_speed_offset		= t256(0);/*(テスト)*/
		HATSUDAN_02_speed_offset		= t256(1);/*(テスト)*/
		HATSUDAN_03_angle65536			= (0);				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		{	const unsigned int cc_type[4] =
			{
				(BULLET_MINI8_BASE + TAMA_IRO_05_MIDORI),
				(BULLET_MINI8_BASE + TAMA_IRO_00_SIRO), 		// 塞符「山神渡御」(normal)
				(BULLET_KOME_BASE  + TAMA_IRO_06_KI_IRO),
				(BULLET_KOME_BASE  + TAMA_IRO_06_KI_IRO),		// 塞符「天上天下の照國」(luna)
			};
			HATSUDAN_05_bullet_obj_type = cc_type[(REG_0f_GAME_DIFFICULTY)];	/* [ 弾] */
		}
		HATSUDAN_06_n_way				= (16*5);			/*(6)*/ 	/* 発弾数 */
		HATSUDAN_07_div_angle65536		= (int)(65536/(16*5));	/* 分割角度(65536[360/360度]を 8 分割) */	/* 1周をn分割した角度 */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
	}
	}
//	if ((0x02)==((REG_10_BOSS_SPELL_TIMER)&0x03))/* (2回に1回)(8回毎に発弾) */
	if ((0x04)==((REG_10_BOSS_SPELL_TIMER)&0x07))/* (2回に1回)(8回毎に発弾) */
	{
		REG_0a_REG2 += (77);
		REG_09_REG1++;
		REG_09_REG1 &= (0x07);
		if (0==REG_09_REG1) 	{	REG_09_REG1++;	};
//		HATSUDAN_01_speed256			= (t256(1.00)); 	/* 弾速 */
		HATSUDAN_01_speed256			= (t256(0.50)); 	/* 弾速 */
//		HATSUDAN_02_speed_offset		= t256(0);/*(テスト)*/
		HATSUDAN_02_speed_offset		= t256(1);/*(テスト)*/
//		HATSUDAN_03_angle65536			= ((65536/2));				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		{	const unsigned int ba_type[4] =
			{
				(BULLET_KOME_BASE	 + TAMA_IRO_03_AOI),
				(BULLET_KOME_BASE	 + TAMA_IRO_03_AOI),	// 塞符「山神渡御」(normal)
				(BULLET_UROKO14_BASE + TAMA_IRO_06_KI_IRO),
				(BULLET_UROKO14_BASE + TAMA_IRO_06_KI_IRO), // 塞符「天上天下の照國」(luna)
			};
			HATSUDAN_05_bullet_obj_type = ba_type[(REG_0f_GAME_DIFFICULTY)];			/* [ 弾] */
		}
	//	HATSUDAN_06_n_way					= (17); 			/*(6)*/ 	/* 発弾数 */
		HATSUDAN_06_n_way					= (16); 			/*(8)*/ 	/* 発弾数 */
	//	HATSUDAN_07_div_angle65536			= (int)(65536/(16))-(((si n1024(( ra_nd() & REG_10_BOSS_SPELL_TIMER)))&0xff));	/*(int)(1024/(6))*/ 	/* 分割角度(1024[360/360度]を 6 分割) */	/* 1周をn分割した角度 */
		HATSUDAN_07_div_angle65536			= (int)(65536/(16/*8*/));	/* 分割角度(65536[360/360度]を 8 分割) */	/* 1周をn分割した角度 */
//
//		HATSUDAN_03_angle65536				= (int)((65536*1/4))+(REG_10_BOSS_SPELL_TIMER&0xff);				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		HATSUDAN_03_angle65536				= ((REG_0a_REG2));				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x08))
		{
//			HATSUDAN_03_angle65536			= (int)((65536*3/4))+(REG_10_BOSS_SPELL_TIMER&0xff);				/* 発射中心角度 / 特殊機能(自機狙い/他) */
			HATSUDAN_03_angle65536			= -(REG_0a_REG2);				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		}
		hatudan_system_regist_katayori_n_way();/* (r33-) */
	}
}


/*---------------------------------------------------------
	文	危険な種弾カード
	-------------------------------------------------------
	テキトー
---------------------------------------------------------*/
//tern OBJ_CALL_FUNC(add_zako_aya_doll);/* 椛弾 */
extern OBJ_CALL_FUNC(add_zako_aya_doll);/* 文人形カード */
local OBJ_CALL_FUNC(spell_create_0f_aya_doll)
{
	if (50==((REG_10_BOSS_SPELL_TIMER) ))
	{
		add_zako_aya_doll(src);
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	}
}

#if 0
/*---------------------------------------------------------
	大玉弾
---------------------------------------------------------*/

local OBJ_CALL_FUNC(bullet_create_aya_kougeki_03)
{
	s_bullet_create_aya_oodama3(src, /* 仕様変更 */(t256(4.0)+((REG_0f_GAME_DIFFICULTY)<<7))/*, 10*/);
}


/*---------------------------------------------------------
	危険な種弾
---------------------------------------------------------*/

local OBJ_CALL_FUNC(bullet_create_aya_kougeki_02)
{
	add_zako_aya_doll(src);
};
#endif


/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
local OBJ_CALL_FUNC(spell_init_1d_amefuri_test)
{
	REG_09_REG1 	= (t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<6));//[定数1]雨の速度
//	REG_0a_REG2 	= ((1024/2)+(1024/24)+(REG_0f_GAME_DIFFICULTY<<3));//[定数2]赤青クナイが曲がる角度
//	card.danmaku_callback[1] = meirin_danmaku_02_aka_ao_kunai_callback;/*(赤青クナイ用)*/
	card.danmaku_callback[2] = common_danmaku_01_amefuri_callback;/*(雨用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}
#else
	#define spell_init_1d_amefuri_test spell_init_12_common_amefuri
#endif
/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------

---------------------------------------------------------*/
#define spell_create_1d_amefuri_test spell_create_common_amefuri

