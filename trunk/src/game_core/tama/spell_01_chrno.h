
/*---------------------------------------------------------
	東方模倣風 〜 Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	チルノのカードを定義します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	紅2面ボス チルノ	通常攻撃1(1/3)	自機狙い拡散弾(1-2-3-4-5-6-7-8弾)
	-------------------------------------------------------
	(1-2-3-4-5-6-7弾)かと思ってたけど、原作やり直したら、
	(1-2-3-4-5-6-7-8弾)だったので修正。
うーん
	原作やり直してよく見たら、
//	(1-2-3-4-5-6-7弾)か 			(easyとか)間違い？？？


	チルノはどうも「内部ランク」で違う感じの弾を撃ってくる。ので良く判らない。


	-------------------------------------------------------
	原作やり直してよく見たら、
	(2-3-4-5-6-7弾) 				(easy)
									(normal)
	(3-4-5-6-7-8弾)。				(hard)
	(3-4-5-6-7-8弾)。				(lunatic)
	-------------------------------------------------------
	始めの方の弾が速く、後の弾はゆっくり。

1100 0111 0xc7
0100 0000
--11 0111 0x37


	-------------------------------------------------------
0x38(56) --0
0x30(48) --1
0x28(40) --2 全方位青枠丸弾
0x20(32) --3 全方位青枠丸弾
0x18(24) --4
0x10(16) --5
0x08( 8) --6
0x00( 0) --7
	-------------------------------------------------------
	lunaticとかだと全方位青枠丸弾も同時に撃ってくる。(2回ぐらい)
	-------------------------------------------------------
	使用レジスタ
	REG_0c_REG4 	覚えとく角度。aaa_angle65536
---------------------------------------------------------*/

/*---------------------------------------------------------
	[初期化セクション]
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_10_cirno_misogi)
{
//	if ((64-1)==((src->boss_sp ell_timer)))
//	if ((SPELL_TIME_0060-1)==((src->boss_sp ell_timer)))
	{
		REG_02_DEST_X	= ((src->center.x256));
		REG_03_DEST_Y	= ((src->center.y256));
		calculate_jikinerai();/* 自機狙い角作成 */
	//	/*別変数でも良い?*/src->tmp_angleCCW65536
		REG_0a_REG2 = ((REG_0f_GAME_DIFFICULTY)>>1);
		REG_0c_REG4 = HATSUDAN_03_angle65536;	/* [角度をおぼえとく] */
		REG_0d_REG5 = const_init_nan_ido_bunkatu_nums_table [tama_const_H04_NUMS_CIRNO_BLUE 	+(REG_0f_GAME_DIFFICULTY)];
		REG_0e_REG6 = const_init_nan_ido_table				[tama_const_H05_DIVS_CIRNO_BLUE 	+(REG_0f_GAME_DIFFICULTY)];
	}
}
/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ1。
	REG_09_REG1 	周期カウンタ2。
	REG_0a_REG2 	難易度別定数3(E=0,N=0, H=1,L=1)
//	REG_0b_REG3 	---
	REG_0c_REG4 	覚えとく角度。aaa_angle65536
	REG_0d_REG5 	難易度別定数1
	REG_0e_REG6 	難易度別定数2
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_10_cirno_misogi)
{
	count_up_limit_NUM(REG_NUM_08_REG0, 8);
//	if (0x40==((src->boss_sp ell_timer)&0xc7))/* 8回(修正1-2-3-4-5-6-7弾:0wayは撃たないので撃つのは7回) */
//	if (0x00==((src->boss_sp ell_timer)&0x07))/* 8回(修正1-2-3-4-5-6-7弾:0wayは撃たないので撃つのは7回) */
	if (1==(REG_08_REG0))/* 8回毎に1回 */
	{
		count_up_limit_NUM(REG_NUM_09_REG1, 12);/*(0 ... 11 : カウンタ2)*/
		if (6 > REG_09_REG1)/*(0,1,2,3,4,5 の場合)*/
		{	/* 2.50, 2.375, 2.250, 2.125, 2.00, 1.875 :  (8-3)==0.125,  8==1.00, 7==0.50, 6==0.25, 5==0.125 */
			HATSUDAN_01_speed256			= (t256(2.5))-((REG_NUM_09_REG1)<<5);				/* 弾速(2.5) */
		//	HATSUDAN_02_speed_offset		= t256(6);/*(テスト)*/
			HATSUDAN_02_speed_offset		= t256(6);/*(テスト)*/
			HATSUDAN_03_angle65536			= REG_0c_REG4;//(/*別変数でも良い*/src->tmp_angleCCW65536); /* [破壊] */
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_KOME_BASE + TAMA_IRO_03_AOI); 	/* [青米弾] になってるのは現在 氷弾 のグラが無いから */
			HATSUDAN_06_n_way				= (2) + (REG_09_REG1) + (REG_0a_REG2);/* [nway] */	/* [破壊] */
		//	HATSUDAN_07_div_angle65536		= (int)(65536/(64));		/* [破壊] */		/* 分割角度(1024[360/360度]を 64 分割) */	/* 1周をn分割した角度 */
		//	HATSUDAN_07_div_angle65536		= (int)(65536/(80));		/* [破壊] */		/* 分割角度(1024[360/360度]を 80 分割) */	/* 1周をn分割した角度 */
			HATSUDAN_07_div_angle65536		= (int)(65536/(96));		/* [破壊] */		/* 分割角度(1024[360/360度]を 80 分割) */	/* 1周をn分割した角度 */
			hatudan_system_regist_n_way();/* (r33-) */
		}
		#if 1
		/* --10 -000 == 0011 0111 */
	//	if (0x20==((src->boss_sp ell_timer)&0x37))/* 2回 */
		if (9 < REG_09_REG1)/* 2回 */	/* (10) or (11) の場合。 */
		{
			if (0 != REG_0d_REG5)
			{
				// hard 18方向(1回)
				// Luna 36方向(2回)
				REG_02_DEST_X	= ((src->center.x256));
				REG_03_DEST_Y	= ((src->center.y256));
				calculate_jikinerai();/* 1:いちいち作成するっぽい。 */
				//
				HATSUDAN_01_speed256			= (t256(2.0));				/* 弾速(2.5) */
				HATSUDAN_02_speed_offset		= t256(1);/*(テスト)*/
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_03_AOI);	/* [枠付き青丸弾] */
				HATSUDAN_06_n_way				= REG_0d_REG5;//(36);/* [nway] */
				HATSUDAN_07_div_angle65536		= REG_0e_REG6;//(int)(65536/(36));		/* 分割角度(1024[360/360度]を 64 分割) */	/* 1周をn分割した角度 */
				hatudan_system_regist_katayori_n_way();/* (r33-) */
			}
		}
		#endif /*0000*/
		#if (1)
	//	voice_play(VOICE14_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_SAKUYA_SHOT02);
		#endif
	}
}


/*---------------------------------------------------------
	紅2面ボス チルノ	通常攻撃1(2/3)	自機狙い全方位弾
	-------------------------------------------------------
	[easy]10方向枠付き青弾。
	-------------------------------------------------------
	ただの全方位ばらまき。
	-------------------------------------------------------
	紅2面ボス チルノ	通常攻撃1(3/3)	自機狙い収束弾
	-------------------------------------------------------
	[easy]自機狙い収束18方向白米弾は、原作のスクショから
	弾の中心で138x138[pix]ぐらいまで広がるっぽい。
	70[pix]として、pspでは52[pix]ぐらい？(52.5==70*0.75)
	-------------------------------------------------------
	始めはただの全方位ばらまき。speedt256(2.0)ぐらい？
	70[pix]進んだら、全弾自機狙いに変化。
	すごいゆっくりspeedt256(0.5)ぐらい？
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	紅2面ボス チルノ	氷符「アイシクルフォール」
	-------------------------------------------------------
	上から順番に計11回撃つ。(初ループ目は2回目の位置から始めるので計10回、2ループ目からは計11回撃つ)
	3回目ぐらいが水平(より若干下)。
	10回目ぐらいが45度斜めぐらい(より若干下)。
	-------------------------------------------------------
	このカードではチルノは動かないので、画像解析してみる。
	http://code.google.com/p/kene-touhou-mohofu/htm/lab/20110704.html
	-------------------------------------------------------
	画像解析してみると、角度ムラがかなりある。
	わざとやってるのかもしれない。良く判らない。
	-------------------------------------------------------
	45/360度を8分割した角度らしい。(が近い)
	と言う事は1周を64分割した角度？？？
	-------------------------------------------------------
	1回0.5[秒]くらいかな？
	-------------------------------------------------------
	0.5[秒]が 32[カウント]として 10[回]だから、
	繰り返し1周期 == 320[カウント]？
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。(音専用)
	-------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(cirno_icecle_fall_danmaku_11_callback)/* 氷符「アイシクルフォール」 */
{
//	if (((32*18)-HATUDAN_FRAME64)==((REG_0a_REG2) ))/* 約0.33[秒](==20[frame])停止 */
	if ((HATUDAN_ITI_NO_JIKAN-64) == src->jyumyou)/* 発弾エフェクト後から[64]カウントの弾 */
	{
		if (0==(src->hatudan_register_spec_data & TAMA_SPEC_KAITEN_HOUKOU_BIT))/*TAMA_SPEC_AKA_AO_KUNAI_BIT*/
		{	/*CW(時計回り)*/
		//	src->rotationCCW1024 -= (6);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		//	src->rotationCCW1024 -= (3);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
			src->rotationCCW1024 -= (1024/4);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		}
		else
		{	/*CCW(反時計回り)*/
		//	src->rotationCCW1024 += (6);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		//	src->rotationCCW1024 += (3);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
			src->rotationCCW1024 += (1024/4);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		}
			src->hatudan_register_tra65536					= t256(1);		/* (1) 調整加速弾 */
			src->hatudan_register_speed65536				= t256(128);		/* 弾速 */
			/* (通常弾へ変身する) */
			src->hatudan_register_spec_data 		= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	}
//	danmaku_00_standard_angle_mover(src);/*(角度弾移動+画面外弾消し)*/
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
}

/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_init_1b_cirno_icecle_fall)
{
//	card.danmaku_callback[1] = danmaku_01_standard_angle_sayuu_hansya_mover;/*(通常弾用)*/	/*(画面外なら反射減速)*/
	card.danmaku_callback[1] = cirno_icecle_fall_danmaku_11_callback;/*(氷符「アイシクルフォール」用。)*/
//	card.danmaku_callback[2] = NULL;/*(未使用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}
/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_1b_cirno_icecle_fall)
{
//	if ((64-1)==((src->boss_sp ell_timer)))
//	if ((SPELL_TIME_0060-1)==((src->boss_sp ell_timer)))
//	{
//		REG_02_DEST_X	= ((src->center.x256));
//		REG_03_DEST_Y	= ((src->center.y256));
//		calculate_jikinerai();/* 自機狙い角作成 */
//		HATSUDAN_03_angle65536 = HATSUDAN_03_angle65536;	/* [角度をおぼえとく] */
//	}
//	else
//	if (0x40==((REG_10_BOSS_SPELL_TIMER)&0xc7))/* 8回(修正1-2-3-4-5-6-7弾:0wayは撃たないので撃つのは7回) */
//	if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x07))/* 8回(修正1-2-3-4-5-6-7弾:0wayは撃たないので撃つのは7回) */
	if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* 約0.5[秒]として、32回に1回。 */
	{
		//	HATSUDAN_02_speed_offset			= (jj<<(2+8));/* x4倍 てすと*/
			HATSUDAN_02_speed_offset			= (0);/* x4倍 てすと*/
		//	HATSUDAN_02_speed_offset			= t256(6);/*(テスト)*/
		//	HATSUDAN_01_speed256				= (t256(0.5));			/* 弾速(pspの画面は狭い) */
		//	HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_KOME_BASE + TAMA_IRO_03_AOI); 	/* [青米弾] になってるのは現在 氷弾 のグラが無いから */
		int jj;
		for (jj=0; jj<5; jj++)/* 一度に5弾撃つ。 */
		{
		//	HATSUDAN_01_speed256				= (t256(2.0)-(jj<<5));				/* 弾速(2.5) */
		//	HATSUDAN_01_speed256				= (t256(1.5)-(jj<<8));				/* 弾速(2.5) */
		//	HATSUDAN_01_speed256				= (t256(1.25)-(jj<<5)); 			/* 弾速(1.250 1.125 1.000, 0.875 0.750, 0.625 ) */
			HATSUDAN_01_speed256				= (t256(1.25)-(jj<<6)); 			/* 弾速(1.250 1.125 1.000, 0.875 0.750, 0.625 ) */
		//	HATSUDAN_03_angle65536				= (0);	/* 下向き */	/* [破壊] */
		//	HATSUDAN_06_n_way					= (1);/* [nway] */		/* [破壊] */
		//	HATSUDAN_07_div_angle65536			= (int)(65536/(5))+((REG_10_BOSS_SPELL_TIMER)<<7);	/* [破壊] */		/* 分割角度(1024[360/360度]を 64 分割) */	/* 1周をn分割した角度 */
			int aaa_angle;
			aaa_angle = (65536/2) + (int)(65536/5) + ((REG_10_BOSS_SPELL_TIMER)<<(10-5));/*n x 1024, 1024==65536/64[分割]。 */
			HATSUDAN_03_angle65536 = aaa_angle;
			/* HATSUDAN_03_angle65536 [破壊??] */
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(TAMA_SPEC_KAITEN_HOUKOU_BIT)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			hatudan_system_regist_single();/* (r33-) */
		//
			HATSUDAN_03_angle65536 = (65536-aaa_angle);/*(反対側を狙う)*/
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(0)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			/* HATSUDAN_03_angle65536 [破壊??] */
			hatudan_system_regist_single();/* (r33-) */
		}
		#if (1)
	//	voice_play(VOICE14_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_SAKUYA_SHOT02);
		#endif
	}
	if (0x20==((REG_10_BOSS_SPELL_TIMER)&0x37))/* 2回 */
	{
		if (0 < (REG_0f_GAME_DIFFICULTY) )
		{
			REG_02_DEST_X	= ((src->center.x256));
			REG_03_DEST_Y	= ((src->center.y256));
			calculate_jikinerai();/* 1:いちいち作成するっぽい。 */
			HATSUDAN_01_speed256			= (t256(1.0));				/* (t256(2.0)) 弾速(2.5) */
			HATSUDAN_02_speed_offset		= t256(1);/*(テスト)*/
			/* HATSUDAN_03_angle65536 [破壊] */
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_07_DAI_DAI);		/* [枠付き黄丸弾] */
			HATSUDAN_06_n_way				= (5);/* [nway] */	/* [破壊] */
			HATSUDAN_07_div_angle65536		= (int)(65536/(36));	/* [破壊] */		/* 分割角度(1024[360/360度]を 64 分割) */	/* 1周をn分割した角度 */
			hatudan_system_regist_n_way();/* (r33-) */
		}
	}
}




/*---------------------------------------------------------
	パーフェクトフリーズの研究
	-------------------------------------------------------
	本当はパーフェクトフリーズに乱数は一つも無いのでは？
	と個人的には考えている。
	-------------------------------------------------------
	とりあえず、自前の乱数生成関数(単に周期の長い関数)が、割と品質の良い乱数を出すので、
	何も小細工しなくても偏らないので特に問題なかった。
	でもどうせなら拘ってみる。
//
	まずチルノは毎フレーム弾をばら撒いているに決まっているので(この前提がおかしい可能性はあまり無いと思われる)。
	チルノがばら撒く弾量から、1秒間に60弾程度、1秒間に120弾程度、1秒間に180弾程度の中では、1秒間に120弾程度が近い。
	従って1フレーム2弾とする。
//
	パット見には判らないが、チルノの乱数弾はぐるぐる回りながら、偏って出てくる。らしいので。
	どれぐらいの速度で回るのなら、回って見えないかを実験してみる。(原作は言われなければ判らない程度)
	(16分割==)約1秒間(64フレーム)に4周(0.25秒で1周)だと、露骨に回っている感じ。なので、これでは明らかにおかしい。
	(8分割==)約1秒間(64フレーム)に8周(0.125秒で1周)だと、言われなければ判らない程度。
	(4分割==)約1秒間(64フレーム)に16周(0.0625秒で1周)だと、分割領域がおおざっぱすぎて、分割する意味がないのではないか？。
	1フレーム2弾と総弾量が決まっている為、周回数を増せばその分、分割領域が広くなり、分割する意味がなくなる。
	(分割数1は分割処理をしないのと同じだから)
	つまり、弾量が1フレーム2弾の場合、8分割に近いものしかありえないという事が判る。
//
	上記の実験から、1フレーム辺りは 8分の1程度の領域に 2弾が確定した。
	ただし、この領域は厳密に 8分の1 ではない。
	なぜなら、この領域が n分の1で割り切れると、回って見えやすい上にゲーム的に安全地帯が出来やすい。
	だから、この領域は厳密に n分の1 ではない。
	65536/8 == 8192。65536/10 == 6553.6。これらに近い値と思われる。
	8分の1(8192) > ? > 10分の1(6554) テキトーに KOTEITI_CHIRNO_7777 にしてみる。
	7777の場合 8.42689983284042689983284042689983分の1。(==65536/7777)
//
	上記実験が 8分割なので、2弾目の角度は、16 分割離れた先の、16 分割程度の乱数値(0-4095)が良いと思われる。
	( 8分割 == 16分割(固定値) + 16分割(乱数値) )
	-------------------------------------------------------
	★ 要修正:
	パーフェクトフリーズの後追い弾(青丸)はやり直したら全然違った。
	普通は止まって避けるから判らなかった。(っていうかワザワザ動く人居ないかと)
	なんか動きながら避けると、構造がわかる。
	ランダムとかじゃなくて、(８方向弾を)「三セット」(同時っぽく)撃って来るみたい。
	(自機狙わない)自機狙い偶数弾。
	初回時に自機狙い角a, b, cを作成(?)。
	aとbとcは独立。
---------------------------------------------------------*/

//#define KOTEITI_CHIRNO_7777	(7777<<6)
//#define KOTEITI_CHIRNO_7777	(497728)
#define KOTEITI_CHIRNO_7777 	(492013)
//#define KOTEITI_CHIRNO_7777	(7777)
/* 擬似乱数==加算固定値(KOTEITI_CHIRNO_7777) */



/*---------------------------------------------------------
	パーフェクトフリーズ
	1周期が約10[秒]とする。
0x280  0000 0010 1000 0000	>>=6  0x000a
	-------------------------------------------------------
64 x  0 	(640==0x0280) a
	ランダム発射
64 x  1 	(576==0x0240) 9
	ランダム発射
64 x  2 	(512==0x0200) 8
	ランダム発射
64 x  3 	(448==0x01c0) 7 減速開始
	//減速
64 x  4 	(384==0x0180) 6 停止
	//停止	// 自機依存による8方向発射
64 x  5 	(320==0x0140) 5 再加速
	//再加速
64 x  6 	(256==0x0100) 4
64 x  7 	(192==0x00c0) 3
64 x  8 	(128==0x0080) 2
64 x  9 	( 64==0x0040) 1
64 x 10 	(  0==0x0000) 0
---------------------------------------------------------*/

/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(cirno_danmaku_01_callback)/* パーフェクトフリーズ */
{
	if ((7*64)==(REG_0a_REG2)/*(とりあえず)*/)	/*減速開始*/
	{
		/*---------------------------------------------------------
			(全弾減速) (枠つき12丸弾を全弾減速させる。)
		---------------------------------------------------------*/
	//	src->hatudan_register_tra65536				= t256(-6); 		/* (-5.12==98%)調整減速弾 x 1.0 */
	//	src->hatudan_register_tra65536				= t256(-12);		/* (-5.12==98%)調整減速弾 x 2.0 */	/* -12==少し戻る */
		src->hatudan_register_tra65536				= t256(-9); 		/* (-5.12==98%)調整減速弾 x 1.5 */
	}
	else
//	if ((6*64)==(REG_0a_REG2)/*(とりあえず)*/)	/*全てストップ*/
//	if ((5*64)==(REG_0a_REG2)/*(とりあえず)*/)	/*全てストップ*/
	if ((4*64)==(REG_0a_REG2)/*(とりあえず)*/)	/*全てストップ*/
	{
		/*---------------------------------------------------------
			(全弾停止) (枠つき12丸弾を全弾停止。)
			弾の色を白くする。
			向きを変える。
		---------------------------------------------------------*/
			#if 0
		//	s->rotationCCW1024				= (ra_nd()&((1024)-1)); 			/* 向き */	/* 向きは乱数でない気がする */
			#else
			/* 少なくとも乱数よりは本物に近い。 が、全然違う。 */
			/* 加算固定値(KOTEITI_CHIRNO_7777)以外の要因として、全弾配列のサイズでループする為、全弾配列のサイズを */
			/* 現在の 1024 から、本物と同じ 640 に減らせば、似る可能性はある。 */
			REG_09_REG1 += (KOTEITI_CHIRNO_7777);
			src->rotationCCW1024			= (deg65536to1024(REG_09_REG1));	/* 向き */	/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
			#endif
		src->hatudan_register_tra65536	= t256(0.0);		/* 調整加速弾 */
		src->hatudan_register_speed65536	= t256(0.0);		/* 弾速 */
		src->obj_type_set						= (BULLET_WAKU12_BASE + TAMA_IRO_00_SIRO);
		reflect_sprite_spec(src, OBJ_BANK_SIZE_00_TAMA); 	/* 弾グラと弾あたり判定を変更する。 */
	}
	else
	if ((3*64)==(REG_0a_REG2)/*(とりあえず)*/)	/*再加速*/
	{
		/*---------------------------------------------------------
			(全弾加速) (枠つき12丸弾を全弾加速させる。)
			加速弾(正確には等加速弾)なのに、加速弾に見えないで
			等速弾(正確には等速度弾、加速==0)に見えちゃう問題がある。
		---------------------------------------------------------*/
		{
			#if 0
			REG_0b_REG3 += 0x0100;
		//	REG_0b_REG3 &= 0x0700;		// 2 3 4 5	6 7 8 9
			REG_0b_REG3 &= 0x0300;		// 3 4 5 6
		//	src->hatudan_register_tra65536	= t256(6);					/* 調整加速弾 */
		//	src->hatudan_register_tra65536	= (jj<<8)+t256(2);			/* (2 ... 9) 調整加速弾 */
	//		src->hatudan_register_tra65536	= (REG_0b_REG3)+t256(3.0);/*(r34)*/ 	/* (3 ... 6) 調整加速弾 */
			#endif
			/* (1は遅すぎたので除外) 9 だと速過ぎるかも */
			/*
				r35で弾幕システムの仕様を変更して、次の弾幕にかかる事が出来るようになった。(r34のシステムでは出来ない)
				(本物は加速が遅いので次の弾幕にかかる)
				したがって、もっとゆっくり加速する。
				r34のシステムではレイヤー区別が無く、弾の種類で判定している為、前の弾幕と次の弾幕の区別が出来ない。
				この為、遅れる事は許されない為に、弾幕の仕様を変更していた。
				r35のシステムでは弾幕別にレイヤーを分けて管理している。この為r34に比べ若干遅い。
				弾幕のレイヤー管理はr34は計1レイヤー管理する(==レイヤー管理無し)。r35は計4レイヤー管理する。
				r35では前の弾幕(レイヤー0)と次の弾幕(レイヤー1)はレイヤーが違うので同時に出現しても問題無い。
			*/
			#if (0)
			REG_0b_REG3 += (123);
			REG_0b_REG3 &= (0x01ff);/* 0 ... 2 */
			src->hatudan_register_tra65536	= (REG_0b_REG3)+t256(1.0);/*(r35)*/ 	/* (1 ... 3) 調整加速弾 */
			#else
			REG_0b_REG3 += (97);
			REG_0b_REG3 &= (0xff);/* 0 ... 1 */
			src->hatudan_register_tra65536	= (REG_0b_REG3)+t256(1.0);/*(r35)*/ 	/* (1 ... 2) 調整加速弾 */
			#endif
		}
		/* (通常弾へ変身する) */	/*(後追い弾は通常弾)*/
		src->hatudan_register_spec_data 	= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	}
//	danmaku_00_standard_angle_mover(src);/*(角度弾移動+画面外弾消し)*/
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
}

/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_13_perfect_freeze)
{
	card.danmaku_callback[1] = cirno_danmaku_01_callback;/*(パーフェクトフリーズ用。)*/
//	card.danmaku_callback[2] = NULL;/*(未使用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}
/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。
	REG_09_REG1 	add_angle
	REG_0a_REG2 	ボスタイマー値、コールバック側に連絡用。(とりあえず)
	REG_0b_REG3 	再加速の速度用に一時使用。
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_13_perfect_freeze)
{
	REG_0a_REG2 = (REG_10_BOSS_SPELL_TIMER);/*(とりあえず)*/
	//
	/* 最初のランダム発射 (64*3フレーム==約3秒)ばら撒く。 */
	if (((SPELL_TIME_64_640)-(64*3))<(REG_10_BOSS_SPELL_TIMER)) // 192== (64*3)
	{
		{
			/*
				原作では弾にどの色を割り当てるかという事には拘りがある。
				チルノは 5種類の色の弾を撃つが、撃つ弾の色の出る確率が均等ではなく偏っている。
				この色の偏りは、プログラムの都合等で偶然偏ったものではなく、作為的な演出である。
				ここではチルノが青(aho)なので、ゲーム的にチルノを目立たせるために青弾を少なくするという演出意図がある。
				(青弾が多いとチルノが何処にいるか判らなくなるというゲーム上の都合がある。)
				この色の偏りが、チルノらしさを演出しているので、ここの再現は実は重要だと思う。
				(ahoとかいう設定は関係なくて)弾の色の偏りが原作と違うと、おそらく「ゲーム的にチルノらしくならない」。
				チルノらしさ: 青 -> あお -> ao -> aho -> アホ -> baka -> baka==�H(花映塚公式説明書から)
				//
				そこで適当なスクショから弾の数を調べる。
				1(1).赤 27/141個	3/16個 == 3.06382978723404255319148936170213 = 27*16/141	(R+G+Bは白なので青が若干少ないがほぼ同値にする)
				2(3).青 21/141個	2/16個 == 2.38297872340425531914893617021277 = 21*16/141	(R+G+Bは白なので青が若干少ないがほぼ同値にする)
				3(5).緑 26/141個	3/16個 == 2.95035460992907801418439716312057 = 26*16/141	(R+G+Bは白なので青が若干少ないがほぼ同値にする)
				4(6).黄 34/141個	4/16個 == 3.85815602836879432624113475177305 = 34*16/141	黄すなわち(R+G)なので青が少ないという演出。
				5(7).橙 33/141個	4/16個 == 3.74468085106382978723404255319149 = 33*16/141	橙すなわち(R+R+G)なので青が少ないという演出。
				従って、下記の確率テーブルがある。
			*/
			const u8 p_tama_tbl[(16)] =
			{
				/* 確率を等しくする。 */
				/* 色が散らばるような配置を考える。 */
				 TAMA_IRO_01_AKA, TAMA_IRO_03_AOI,	  TAMA_IRO_05_MIDORI, TAMA_IRO_06_KI_IRO, TAMA_IRO_07_DAI_DAI,
				 TAMA_IRO_01_AKA, TAMA_IRO_03_AOI,	  TAMA_IRO_05_MIDORI, TAMA_IRO_06_KI_IRO, TAMA_IRO_07_DAI_DAI,
				 TAMA_IRO_01_AKA, TAMA_IRO_06_KI_IRO, TAMA_IRO_05_MIDORI, TAMA_IRO_06_KI_IRO, TAMA_IRO_07_DAI_DAI, TAMA_IRO_07_DAI_DAI,
			};
			REG_09_REG1 += (KOTEITI_CHIRNO_7777);	/* 固定値。8分の1 から 10分の1 程度の値で、n分の1で割り切れない値。 */
			const u16 ra_nd16 = (ra_nd()/*&0xffff*/);
		//	HATSUDAN_01_speed256			= (t256(1.9)+(ra_nd16>>7)); 		/* 弾速 */	/* (ra_nd16>>7) == 乱数(0-511) */
			HATSUDAN_01_speed256			= (t256(1.5)+(ra_nd16>>7)); 		/* 弾速 */	/* (ra_nd16>>7) == 乱数(0-511) */
		//	HATSUDAN_02_speed_offset		= t256(-5); 		/* (-5) (-3)調整減速弾 */	/* この方式になるか検討中 */
			HATSUDAN_02_speed_offset		= t256(0);/*(テスト)*/
			/*
				ここの弾速調整は、単純に速い方が難しかったり、遅い方が難しかったりという事は無い。
				あんまり速くし過ぎる(倍速ぐらい？)と、ただのエターナルミークになっちゃうのは判ると思うけど。
				//
				速い場合は、始めの弾を避けるのはその分難しくなるが、後半は弾数が減るので酷く簡単になる。
				遅い場合は、始めの弾を避けるのはその分簡単になるが、後半は弾数が増えるのでその分難しくなる。
			*/
		//	HATSUDAN_03_angle65536				= (0/65536);					/* 下向き */
		//	HATSUDAN_03_angle65536				= (ra_nd()&((32768)-1)) + ((REG_10_BOSS_SPELL_TIMER)<<(8+6));		/* 向き */
		//	HATSUDAN_03_angle65536				= (ra_nd()&((32768/32)-1)) + ((REG_10_BOSS_SPELL_TIMER)<<(8+4));		/* 向き */
		//	HATSUDAN_03_angle65536				= (ra_nd()&((65536)-1)) ;		/* 向き */
			HATSUDAN_03_angle65536				= (REG_09_REG1)/*8分割(0.125秒で1周)*/; 	/* 向き */
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(TAMA_SPEC_2000_EFFECT_MINI)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
			HATSUDAN_05_bullet_obj_type 		= BULLET_WAKU12_BASE + (p_tama_tbl[((REG_10_BOSS_SPELL_TIMER)&0x0f)]);
			#if 0
			HATSUDAN_06_n_way					= (2);										/* [2way] */	/* 1フレームに2弾 */
			#else
			HATSUDAN_06_n_way					= (1+(REG_0f_GAME_DIFFICULTY)); /* 模倣風なので、難易度調整 */		/* [2way] */	/* 1フレームに2弾 */
			#endif
			HATSUDAN_07_div_angle65536			= (65536/16)+(ra_nd16>>4);				/* 分割角度([360/360]度を16分割) */ 	/* (ra_nd16>>4) == 乱数(0-4095) */
			hatudan_system_regist_katayori_n_way(); /* (r33-) */
		}
		#if (1)
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x07))
		{
			cpu_bullet_play_15_auto();
		}
		#endif
	}
	else
//	if (((SPELL_TIME_64_640)-(64*4))<(REG_10_BOSS_SPELL_TIMER)) // 192== (64*3)
	if (((SPELL_TIME_64_640)-(64*4))<(REG_10_BOSS_SPELL_TIMER)) // 192== (64*3)
	{
		;/*何もしない*/
	}
	else
	/* 後追い弾発射 */
//	if (((SPELL_TIME_64_640)-(64*5))<(REG_10_BOSS_SPELL_TIMER)) // 192== (64*3)
	if (((SPELL_TIME_64_640)-((64*4))-(48)/*(64*6)*/)<(REG_10_BOSS_SPELL_TIMER)) // 192== (64*3)
	{
		/*
		Normalで適当にやってみた。
		後追い弾は、速度の違う弾(三種)を同時に出す。のセットを6回だった。
		*/
		if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x07)) /* 8[frame]に1回 */
		{
			if ((4+(REG_0f_GAME_DIFFICULTY))>((REG_10_BOSS_SPELL_TIMER)&0x07))
			{
				REG_02_DEST_X	= ((src->center.x256));
				REG_03_DEST_Y	= ((src->center.y256));
				calculate_jikinerai();/* 1:いちいち作成するっぽい。 */
				int aaa_angle65536;
				aaa_angle65536 = (HATSUDAN_03_angle65536);/* 自機狙い角 */
				HATSUDAN_02_speed_offset		= t256(0);/*(テスト)*/
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_2000_EFFECT_MINI)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_MARU10_BASE + TAMA_IRO_03_AOI);
				unsigned int jj;
				for (jj=0; jj<3; jj++)
				{
				//	HATSUDAN_01_speed256			= (t256(2.5));				/* 弾速 */
				//	HATSUDAN_01_speed256			= (t256(1.75)+(jj<<7)); 	/* 弾速 */
					HATSUDAN_01_speed256			= (t256(2.00)+(jj<<8)); 	/* 弾速 */
					HATSUDAN_03_angle65536			= (aaa_angle65536)+(jj<<8); 	/* [破壊] */		/* 自機狙い角 */
					HATSUDAN_06_n_way				= (8);			/* [破壊] */						/* [8way] */
					HATSUDAN_07_div_angle65536		= (65536/16);	/* [破壊] */						/* 分割角度([360/360]度を16分割) */
					hatudan_system_regist_n_way();		/* (r33-) */
				}
			}
		}
	}
}


/*---------------------------------------------------------
	雪符「ダイアモンドブリザード」
	-------------------------------------------------------
	(r35)巧くいってない。
	-------------------------------------------------------
	チルノの周りで氷弾をばら撒く
	微妙に減速弾っぽい。
		1.チルノの周り離れた位置から発弾。(ランダム?)
		2.ある程度まで広がる。
		3.微減速弾。
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。(音専用)
	REG_09_REG1 	angle_65536
	REG_0a_REG2 	sine   の値用。
	REG_0b_REG3 	cosine の値用。
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_16_diamond_blizzard)
{
//	if (0==((REG_10_BOSS_SPELL_TIMER)&0x1f))/*(32回に1回)*/
	/*(毎フレーム発弾)*/
	{
		REG_09_REG1 = (ra_nd()/*&0xffff*/);
		REG_0a_REG2 = REG_02_DEST_X;/*(ボスx座標保存, push stack)*/
		REG_0b_REG3 = REG_03_DEST_Y;/*(ボスy座標保存, push stack)*/
		#if /*雪符「ダイアモンドブリザード」*/1/*[廃止予定](-r39)*/
		/*(楕円の範囲で発弾)*/
	//	sincos65536_ZZZ();/*(ダメ???)*/
	HATSUDAN_03_angle65536 = REG_09_REG1;
//	HATSUDAN_01_speed256 = t256(1.0);
//	HATSUDAN_01_speed256 = (REG_09_REG1&0x7f00);
{
	#if (0)//
	REG_0a_REG2 	= (((si n1024((deg65536to1024(HATSUDAN_03_angle65536)))) ) );		/* CCWの場合 */
	REG_0b_REG3 	= (((co s1024((deg65536to1024(HATSUDAN_03_angle65536)))) ) );
	#else
	{
		int sin_value_t256; 		//	sin_value_t256 = 0;
		int cos_value_t256; 		//	cos_value_t256 = 0;
		int256_sincos1024( (deg65536to1024(HATSUDAN_03_angle65536)), &sin_value_t256, &cos_value_t256);
		REG_02_DEST_X	 = (((sin_value_t256)*(REG_09_REG1&0x7f00))>>8);
		REG_03_DEST_Y	 = (((cos_value_t256)*((REG_09_REG1>>8)&0x3f00))>>8);
	//	REG_02_DEST_X	 = (((cos_value_t256)*(HATSUDAN_01_speed256))>>8);/*fps_factor*/
	//	REG_03_DEST_Y	 = (((sin_value_t256)*(HATSUDAN_01_speed256))>>8);/*fps_factor*/
	}
	#endif
}
		#else/*(xxx-)*/
		//------------------
		HATSUDAN_03_angle65536 = REG_09_REG1;
	//	HATSUDAN_01_speed256 = t256(1.0);
		HATSUDAN_01_speed256 = (REG_09_REG1&0x7f00);
		sincos256();/*(破壊レジスタ多いので注意)*/
	//	radius_aaa = REG_03_DEST_Y;//sin_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
	//	src->math_vector.y256 = REG_02_DEST_X;//cos_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
		//------------------
		#endif
		REG_02_DEST_X += REG_0a_REG2;/*(ボスx座標加算, pull stack)*/
		REG_03_DEST_Y += REG_0b_REG3;/*(ボスy座標加算, pull stack)*/
		//
		HATSUDAN_01_speed256			= (t256(0.5));				/* 弾速(2.5) */
		HATSUDAN_02_speed_offset		= t256(2);/*(てすと)*/
		HATSUDAN_03_angle65536			= (REG_09_REG1);/*(何故かダメ???)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_KOME_BASE + TAMA_IRO_03_AOI); 	/* [青米弾] になってるのは現在 氷弾 のグラが無いから */
		hatudan_system_regist_single();
	}
	count_up_limit_NUM(REG_NUM_08_REG0, 32);//	/* (32回毎に発音) */
	if (1==REG_08_REG0) /* (32回に1回) 発音 */
	{
	//	cpu_bullet_play_15_auto();
		bullet_play_04_auto(VOICE16_BOSS_KYUPIN);
	}
}


			/*(矩形の範囲で発弾)*/
			#if 0
			REG_02_DEST_X	+= -t256(64)+(ra_nd32&0x7f00);				/* 弾源x256 */
			REG_03_DEST_Y	+= -t256(32)+((ra_nd32>>8)&0x3f00); 		/* 弾源y256 */
			#endif

/*
	最強「エターナルフォースブリザード」
	(Eternal Force Blizzard, EFB)

	エターナルフォースブリザード
		一瞬で相手の周囲の大気ごと氷結させる
		相手は死ぬ

	2ちゃんねるのニュース速報板のスレッド(通称VIP)で考え出された魔法(必殺技？)
	あまりのクォリティの高さに様々なAAが作られる。
	また、多くの閲覧者に自分も過去に同じような事を考えていたと言うトラウマを呼び覚まさせた。

*/
