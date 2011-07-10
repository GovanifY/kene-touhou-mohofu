
/*---------------------------------------------------------
 東方模倣風 ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ルーミアのカードを定義します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	紅1面ボス ルーミア 夜符「ナイトバード」にちょっとだけ似たカード(予定)
	-------------------------------------------------------
	5弾目が自機狙いの 32分割16方向 弓状弾。
	-------------------------------------------------------
---a aaaa [aaa]
0000 xxxx 16回 撃つ         青色 (0x00==((time_out)&0x20))
0001 0000 自機狙い角作成    青色 (0x00==((time_out)&0x20))
0010 xxxx 16回 撃つ         水色
0011 0000 自機狙い角作成    水色
0100 xxxx 16回 撃つ         青色 (0x00==((time_out)&0x20))
0101 0000 自機狙い角作成    青色 (0x00==((time_out)&0x20))
0110 xxxx 16回 撃つ         水色
0111 0000 自機狙い角作成    水色
//
1000 0000 start
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ1。
	REG_09_REG1 	カウンタ2。
	REG_0a_REG2 	弾の基準角度。aaa_angle65536
	REG_0b_REG3 	弾番号。0x0fが1弾目、0x0eが2弾目。
	-------------------------------------------------------
---------------------------------------------------------*/
local void spell_create_08_rumia_night_bird(SPRITE *src)
{
	count_up_limit_NUM(REG_NUM_08_REG0, 32);
//	if (0x10==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* 自機狙い角作成 */
	if (1==(REG_08_REG0))/* 自機狙い角作成 */
	{
		count_up_limit_NUM(REG_NUM_09_REG1, 2);/*(カウンタ2)*/
		#if 1
		REG_02_DEST_X	= ((src->cx256));
		REG_03_DEST_Y	= ((src->cy256));
		calculate_jikinerai();/* 自機狙い角作成 */
		REG_0a_REG2 = HATSUDAN_03_angle65536;/* 自機狙い角 */
		#endif
        //  2048 == (65536)/(32)        : 1周を32分割した角度。
        //  8192 == (2048) * (4)        : 5弾目(4弾分の幅)。
        // 57344 == (65536) - (8192)    : 開始角度。(加算しないと誤差が出る。)
    //  REG_0a_REG2 += (const int)(65536-(int)((65536*4)/(32)));/* 5弾目が自機狙い */
	//	if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x20))
		if (1==(REG_09_REG1))/*(カウンタ2)*/
				{	REG_0a_REG2 += (57344); 	}/* 5弾目(4弾分の幅)が自機狙い(65536-(4*(65536/32))) */
		else	{	REG_0a_REG2 += (8192);		}/* 5弾目(4弾分の幅)が自機狙い(65536+(4*(65536/32))) */
		mask65536(REG_0a_REG2);
		/* 角度(65536[360/360度]を 32分割) */
		REG_0b_REG3 = 0x10;
	}
//	else
//	if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x10))/* 16回 撃つ */
	if (15<(REG_08_REG0))/* (16 ... 31), 16回 撃つ */
	{
		{
			/* 方向 */
			REG_0b_REG3 -= (1);
		//	REG_0b_REG3 = ((REG_10_BOSS_SPELL_TIMER)&0x0f);
			#if (0)/* デバッグ(自機狙い弾の色を変える) */
			if (0x0b==REG_0b_REG3)/* 0x0b==5弾目 (0x0fが1弾目だから0x0bが5弾目) */
						{	HATSUDAN_05_bullet_obj_type = (BULLET_WAKU12_BASE + TAMA_IRO_01_AKA);		}/* 枠つき赤色弾 色変える。 */
			else
			#endif
			#if 1
			{
			//	if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x20))
				if (1==(REG_09_REG1))/*(カウンタ2)*/
						{	HATSUDAN_05_bullet_obj_type = (BULLET_WAKU12_BASE + TAMA_IRO_03_AOI);		}/* 枠つき青色弾 */
				else	{	HATSUDAN_05_bullet_obj_type = (BULLET_WAKU12_BASE + TAMA_IRO_04_MIZU_IRO);	}/* 枠つき水色弾 */
			}
			#endif
		//	HATSUDAN_01_speed256			= (t256(1.5));								/* 弾速 */
			HATSUDAN_01_speed256			= (t256(0.75))+((16-(REG_0b_REG3))<<5); 			/* 弾速 */
			/* 弾速 0.75 - 2.75 倍 */
			HATSUDAN_02_speed_offset		= t256(0);/*(テスト)*/
			HATSUDAN_03_angle65536			= (REG_0a_REG2);	/* 発射中心角度 / 特殊機能(自機狙い/他) */
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
			/* [枠付き青色弾] [枠付き水色弾] */
			hatudan_system_regist_single();/* (r33-) */
		}
		/* 次の弾の角度 */
		//	 2048 == (65536)/(32)		: 1周を32分割した角度。
		//	63488 == (65536) - (2048)	: 減算の場合(加算しないと誤差が出る。)
	//	if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x20))
		if (1==(REG_09_REG1))/*(カウンタ2)*/
				{	REG_0a_REG2 += (2048);		}/* 角度(65536[360/360度]を 32分割) (int)(65536/(32)); */
		else	{	REG_0a_REG2 += (63488); 	}/* 角度(65536[360/360度]を 32分割) (65536)-(int)(65536/(32)); */
		mask65536(REG_0a_REG2);
		//
		bullet_play_04_auto(VOICE14_SAKUYA_SHOT02);/*VOICE15_BOSS_KOUGEKI_01*/
	}
}


/*---------------------------------------------------------
	紅1面ボス ルーミア 闇符「ディマーケイション」にちょっとだけ似たカード(予定)
	-------------------------------------------------------
	ルーミア交差弾:
	交差弾にも良く見ると種類があるような気がしてきた。
	闇符「ディマーケイション」の交差弾は。
		1. 一定距離に飛び出す。(通常減速弾)
		2. 弾停止。
		3. 突然 (弾グラが)90度回転する。(極彩颱風みたいな感じで滑らかではあるが、すばやく回転)
		4.弾グラが傾きながら弾が広がる。
	-------------------------------------------------------
	16
	-------------------------------------------------------
	0010 01--	0x027f == 64*10-1
	0010 00--	0x023f == 64* 9-1
				...
	0000 00--	0x003f == 64* 1-1
	-------------------------------------------------------
640==0x280
512==0x200
	32*0		５
	32*1		６
	32*2		３
	32*3		４
	32*4		１
	32*5		２
	32*6		休み
	32*7		休み
	32*8		休み
	32*9			赤
	32*10		休み
	32*11			緑
	32*12		休み
	32*13			青
	32*14		休み
	32*15		休み

---------------------------------------------------------*/

/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
	交差弾。反時計回り。
	[弾幕グループ(2)セクション]
	-------------------------------------------------------
	交差弾。時計回り。
---------------------------------------------------------*/
local void rumia_danmaku_01_callback(SPRITE *src)
{
	/* 0-32 カウントまで */ 	/* [0]カウント==発弾位置 */
	if ((HATUDAN_ITI_NO_JIKAN-32) < src->jyumyou)/* 発弾エフェクト後から[0-31]カウント経過した弾 */
	{
		;/* 発弾後、すぐは何もしない==直進弾 */
	}
	else/* 32-64 カウントまで */
	if ((HATUDAN_ITI_NO_JIKAN-64) < src->jyumyou)/* 発弾エフェクト後から[32-63]カウント経過した弾 */
	{
		if (0==(src->hatudan_register_spec_data & TAMA_SPEC_KAITEN_HOUKOU_BIT))
		{
			/*(模倣風では下CCWなので、正方向が反時計回り)*/
		//	src->rotationCCW1024 += (8);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
			src->rotationCCW1024 += (10);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		}
		else
		{
			/*(模倣風では下CCWなので、負方向が時計回り)*/
		//	src->rotationCCW1024 -= (8);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
			src->rotationCCW1024 -= (10);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		}
	}
	else/* 64-256 カウントまで */
	if ((HATUDAN_ITI_NO_JIKAN-256) < src->jyumyou)/* 発弾エフェクト後から[64-255]カウント経過した弾 */
	{
		if (0==(src->hatudan_register_spec_data & TAMA_SPEC_KAITEN_HOUKOU_BIT))
		{
			src->rotationCCW1024++;/* 曲げてみるテスト */
		}
		else
		{
			src->rotationCCW1024--;/* 曲げてみるテスト */
		}
	}
	// (この方が解かりやすい？)
	else
//	if ((HATUDAN_ITI_NO_JIKAN-256) == src->jyumyou/*(寿命なので自動で減る)*/)/* 発弾エフェクト後から[256]カウント経過した弾 */
	{
		/* (通常弾へ変身する==画面内では弾は消えない) */
		src->hatudan_register_spec_data 	= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	}
//	danmaku_00_standard_angle_mover(src);/*(角度弾移動+画面外弾消し)*/
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
}


	#if (0)
	/* 発弾後 512 カウント以上は弾を消す。 */
	if ((HATUDAN_ITI_NO_JIKAN-512) > src->jyumyou/*(寿命なので自動で減る)*/)/* 発弾エフェクト後から[512]カウント以上経過した弾 */
	{
		/* (通常弾へ変身する) */
		src->hatudan_register_spec_data 	= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	}
	#endif


/*---------------------------------------------------------
	[弾幕グループ(2)セクション]
	-------------------------------------------------------
	連弾。自機狙い。
	-------------------------------------------------------
	自機を狙わない場合があるが、原因が良く解からない。
---------------------------------------------------------*/
local void rumia_danmaku_02_callback(SPRITE *src)/* 連弾 */
{
	/* 32 カウントなら */
	if ((HATUDAN_ITI_NO_JIKAN-32) == src->jyumyou)/* 発弾エフェクト後から[32]カウント経過した弾 */
	{
		REG_02_DEST_X	= ((src->cx256));
		REG_03_DEST_Y	= ((src->cy256));
		calculate_jikinerai();/* 自機狙い角作成 */
		#if 0
		/*(ダメ)*/
	//	src->rotationCCW1024 = (0);//描画用角度(deg65536to1024(HATSUDAN_03_angle65536));/* 自機狙い角 */
		#else
		/*(特別ケース、このまま(見た目)の角度で移動する)*/
		src->rotationCCW1024 = (deg65536to1024(HATSUDAN_03_angle65536));/* 自機狙い角 */
		/* ここの描画用角度は後で利用する。 */
		#endif
		//
		src->hatudan_register_speed65536	= (t256(2.0)<<8);			/* 弾速 */
		//
		/* (通常弾へ変身する) */
		src->hatudan_register_spec_data 	= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	}
//	danmaku_00_standard_angle_mover(src);/*(角度弾移動+画面外弾消し)*/
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
}


/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/

local void spell_init_29_rumia_demarcation(SPRITE *src)
{
//	card.danmaku_callback[0] = danmaku_00_standard_angle_mover;/*(通常弾用)*/
	card.danmaku_callback[1] = rumia_danmaku_01_callback;/*(米弾。交差弾。反時計回り。) (米弾。交差弾。時計回り。)*/
	card.danmaku_callback[2] = rumia_danmaku_02_callback;/*(丸弾。連弾。自機狙い。)*/
//	card.danmaku_callback[3] = NULL;
}
/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。(32回に1回発弾する)
	REG_09_REG1 	カウンタ。(16種類)
	REG_0a_REG2 	弾色(交差弾)。
	REG_0b_REG3 	弾角度(交差弾)。	弾角度(連弾)。

---------------------------------------------------------*/
// カードスクリプト語では「else」は無い。
local void spell_create_29_rumia_demarcation(SPRITE *src)
{
//	if ((0x1f)==((REG_10_BOSS_SPELL_TIMER)&0x1f))
	count_up_limit_NUM(REG_NUM_08_REG0, 32);
	if (0==(REG_08_REG0))
	{
		count_up_limit_NUM(REG_NUM_09_REG1, 16);//
		#if (1)/*(発弾座標を修正)*/
			REG_02_DEST_X	= ((src->cx256));
			REG_03_DEST_Y	= ((src->cy256));
		#endif
		if (2==REG_09_REG1)
		{
			REG_0a_REG2 = (TAMA_IRO_03_AOI);		/* 青弾 */
			goto kousadan;
		}
		if (4==REG_09_REG1)
		{
			REG_0a_REG2 = (TAMA_IRO_05_MIDORI); 	/* 緑弾 */
			goto kousadan;
		}
		if (6==REG_09_REG1)
		{
			REG_0a_REG2 = (TAMA_IRO_01_AKA);		/* 赤弾 */
			goto kousadan;
		}
		if (14<REG_09_REG1)/* 最後の方は時間が足りないので、変身処理が間に合わない。 */
		{
			goto oyasumi;
		}
		if (9<REG_09_REG1)		/* 自機狙い連弾 */
		{
		//	dimmer_shot_02_rendan(src);
			/*---------------------------------------------------------
				自機狙い連弾
			-------------------------------------------------------
			記憶だけで作りがちなので、原作でチェック。normalの場合、飛んでくる連弾は４回だった。
			---------------------------------------------------------*/
			//local void dimmer_shot_02_rendan(SPRITE *src)
			unsigned int ii;
			for (ii=0; ii<8; ii++)
			{
				#if 1
				REG_02_DEST_X	= ((src->cx256));
				REG_03_DEST_Y	= ((src->cy256));
				calculate_jikinerai();/* 自機狙い角作成 */
				REG_0b_REG3 = HATSUDAN_03_angle65536;/* 自機狙い角 */
				#endif
				//	2048 == (65536)/(32)		: 1周を32分割した角度。
				//	8192 == (2048) * (4)		: 5弾目(4弾分の幅)。
				// 57344 == (65536) - (8192)	: 開始角度。(加算しないと誤差が出る。)
			//	REG_0b_REG3 += (const int)(65536-(int)((65536*4)/(32)));/* 5弾目が自機狙い */
				if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x20))
						{	REG_0b_REG3 += (57344); 	}/* 5弾目(4弾分の幅)が自機狙い(65536-(4*(65536/32))) */
				else	{	REG_0b_REG3 += (8192);		}/* 5弾目(4弾分の幅)が自機狙い(65536+(4*(65536/32))) */
				mask65536(REG_0b_REG3);
				/* 角度(65536[360/360度]を 32分割) */
					HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_02)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
					HATSUDAN_05_bullet_obj_type 		= BULLET_WAKU12_BASE + TAMA_IRO_03_AOI; 		/* [枠付き青色弾] [枠付き水色弾] */
				unsigned int jj;
				for (jj=0; jj<16; jj++)
				{
				//	HATSUDAN_01_speed256				= (t256(1.5));								/* 弾速 */
					HATSUDAN_01_speed256				= (t256(0.75))+((16-(jj))<<5);				/* 弾速 */
					/* 弾速 0.75 - 2.75 倍 */
					HATSUDAN_02_speed_offset			= (ii<<6);//t256(0);/*(テスト)*/
					HATSUDAN_03_angle65536				= (REG_0b_REG3);	/* 発射中心角度 / 特殊機能(自機狙い/他) */
					hatudan_system_regist_single();/* (r33-) */
					/* 次の弾の角度 */
					//	 2048 == (65536)/(32)		: 1周を32分割した角度。
					//	63488 == (65536) - (2048)	: 減算の場合(加算しないと誤差が出る。)
					if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x20))
							{	REG_0b_REG3 += (2048);		}/* 角度(65536[360/360度]を 32分割) (int)(65536/(32)); */
					else	{	REG_0b_REG3 += (63488); 	}/* 角度(65536[360/360度]を 32分割) (65536)-(int)(65536/(32)); */
					mask65536(REG_0b_REG3);
				}
			}
			bullet_play_04_auto(VOICE14_SAKUYA_SHOT02);/*VOICE15_BOSS_KOUGEKI_01*/
		}
		goto oyasumi;
	kousadan:	/* 交差弾。 青弾 / 緑弾 / 赤弾 */
		{
		//	dimmer_shot_01_kousadan(src, REG_0a_REG2/* 弾色 */);
			/*---------------------------------------------------------
				交差弾。 青弾 / 緑弾 / 赤弾
			---------------------------------------------------------*/
		//	local void dimmer_shot_01_kousadan(SPRITE *src, u8 tama_type)
			REG_0b_REG3 = (((REG_0a_REG2)&0x06)<<8);/* 弾色別に発弾角を変える。(((REG_0a_REG2)&0x06)<<6) */
			HATSUDAN_01_speed256				= (t256(1.0));			/* 弾速 (t256(1.5)) */
			HATSUDAN_02_speed_offset			= t256(0);/*(テスト)*/
			HATSUDAN_03_angle65536				= (REG_0b_REG3);					/* 発射中心角度 / 特殊機能(自機狙い/他) */
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(0)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_05_bullet_obj_type 		= BULLET_KOME_BASE + (REG_0a_REG2); 		/* [枠つき青色弾] [枠つき水色弾] */
			HATSUDAN_06_n_way					= (32); 				/* (32) [32way] */
			HATSUDAN_07_div_angle65536			= (65536/32);			/* 分割角度 (65536/32) */
			hatudan_system_regist_katayori_n_way();/*(r33-)*/
			/* HATSUDAN_03_angle65536はhatudan_system_regist_katayori_n_way();で破壊される(仕様な)ので再設定が必要。 */
			HATSUDAN_03_angle65536				= (REG_0b_REG3);					/* 発射中心角度 / 特殊機能(自機狙い/他) */
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(TAMA_SPEC_KAITEN_HOUKOU_BIT)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			hatudan_system_regist_katayori_n_way();
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		}
	oyasumi:	/* お休み */
		;
	}

}
		//{
		//	#define P0x80		0x80
		//	#define YASUMI000	0x00
		//	const u8 ttt[16] =
		//	{
		//	/*0x0f*/0	YASUMI000,	/* 休み */
		//	/*0x0e*/1	YASUMI000,	/* 休み */
		//	/*0x0d*/2	(BULLET_KOME_03_AOI    - BULLET_KOME_00_SIRO),		/* 青弾 */
		//	/*0x0c*/3	YASUMI000,	/* 休み */
		//	/*0x0b*/4	(BULLET_KOME_05_MIDORI - BULLET_KOME_00_SIRO),		/* 緑弾 */
		//	/*0x0a*/5	YASUMI000,	/* 休み */
		//	/*0x09*/6	(BULLET_KOME_01_AKA    - BULLET_KOME_00_SIRO),		/* 赤弾 */
		//	/*0x08*/7	YASUMI000,	/* 休み */
		//	/*0x07*/8	YASUMI000,	/* 休み */
		//	/*0x06*/9	YASUMI000,	/* 休み */
		//	/*0x05*/10	P0x80,	/* 自機狙い連弾 */
		//	/*0x04*/	P0x80,	/* 自機狙い連弾 */
		//	/*0x03*/	P0x80,	/* 自機狙い連弾 */
		//	/*0x02*/	P0x80,	/* 自機狙い連弾 */
		//	/*0x01*/	P0x80,	/* 自機狙い連弾 */
		//	/*0x00*/	P0x80,	/* 自機狙い連弾 */
		//	};
		//	tama_type = ttt[((REG_09_REG1/*(REG_10_BOSS_SPELL_TIMER)>>5*/)&0x0f)];
		//}
