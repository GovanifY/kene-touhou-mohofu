
/*---------------------------------------------------------
 東方模倣風 ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ルーミアの弾幕を生成します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	紅1面ボス ルーミア 夜符「ナイトバード」にちょっとだけ似た弾幕(予定)
	-------------------------------------------------------
	5弾目が自機狙いの 32分割16方向 弓状弾。
	-------------------------------------------------------
---a aaaa [aaa]
0000 xxxx 16回 撃つ 		青 (0x00==((time_out)&0x20))
0001 0000 自機狙い角作成	青 (0x00==((time_out)&0x20))
0010 xxxx 16回 撃つ 		緑
0011 0000 自機狙い角作成	緑
0100 xxxx 16回 撃つ 		青 (0x00==((time_out)&0x20))
0101 0000 自機狙い角作成	青 (0x00==((time_out)&0x20))
0110 xxxx 16回 撃つ 		緑
0111 0000 自機狙い角作成	緑
//
1000 0000 start
---------------------------------------------------------*/
static void spell_create_08_rumia_night_bird(SPRITE *src)
{
	static int aaa_angle65536;
	if (0x10==((src->boss_spell_timer)&0x1f))/* 自機狙い角作成 */
	{
		#if 1
		tmp_angleCCW65536_jiki_nerai(src);/* 自機狙い角作成 */
		aaa_angle65536 = src->tmp_angleCCW65536;/* 自機狙い角 */
		#endif
		//	2048 == (65536)/(32)		: 1周を32分割した角度。
		//	8192 == (2048) * (4)		: 5弾目(4弾分の幅)。
		// 57344 == (65536) - (8192)	: 開始角度。(加算しないと誤差が出る。)
	//	aaa_angle65536 += (const int)(65536-(int)((65536*4)/(32)));/* 5弾目が自機狙い */
		if (0x00==((src->boss_spell_timer)&0x20))
				{	aaa_angle65536 += (57344);		}/* 5弾目(4弾分の幅)が自機狙い(65536-(4*(65536/32))) */
		else	{	aaa_angle65536 += (8192);		}/* 5弾目(4弾分の幅)が自機狙い(65536+(4*(65536/32))) */
		mask65536(aaa_angle65536);
		/* 角度(65536[360/360度]を 32分割) */
	}
	else
	if (0x00==((src->boss_spell_timer)&0x10))/* 16回 撃つ */
	{
		bullet_play_04_auto(VOICE14_SAKUYA_SHOT02);/*VOICE15_BOSS_KOUGEKI_01*/
		{
			int tama_color;/* 方向 */
			int iii;
				iii = ((src->boss_spell_timer)&0x0f);
			#if 0/* デバッグ(自機狙い弾の色を変える) */
			if (0x0b==iii)/* 0x0b==5弾目 (0x0fが1弾目だから0x0bが5弾目) */
			{
				tama_color = -3;/* 枠つき赤弾  色変える。 */
			}
			else
			#endif
			#if 1
			{
				if (0x00==((src->boss_spell_timer)&0x20))
						{	tama_color = -1;/* 枠つき青弾 */ }
				else	{	tama_color =  1;/* 枠つき緑弾 */ }
			}
			#endif
		//	br.BULLET_REGIST_00_speed256				= (t256(1.5));								/* 弾速 */
			br.BULLET_REGIST_00_speed256				= (t256(0.75))+((16-(iii))<<5); 			/* 弾速 */
			/* 弾速 0.75 - 2.75 倍 */
			br.BULLET_REGIST_01_speed_offset			= t256(0);/*(テスト)*/
			br.BULLET_REGIST_02_angle65536				= (aaa_angle65536); 	/* 発射中心角度 / 特殊機能(自機狙い/他) */
			br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_03_AOI+1+tama_color;			/* [枠付き青丸弾] [枠付き緑丸弾] */
			tama_system_regist_single();/* (r33-) */
		}
		/* 次の弾の角度 */
		//	 2048 == (65536)/(32)		: 1周を32分割した角度。
		//	63488 == (65536) - (2048)	: 減算の場合(加算しないと誤差が出る。)
		if (0x00==((src->boss_spell_timer)&0x20))
				{	aaa_angle65536 += (2048);		}/* 角度(65536[360/360度]を 32分割) (int)(65536/(32)); */
		else	{	aaa_angle65536 += (63488);		}/* 角度(65536[360/360度]を 32分割) (65536)-(int)(65536/(32)); */
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

static void s_check_aaa_kome_dan(SPRITE *src)/* 交差弾 */
{
	if (0==(src->flags&SP_FLAG_COLISION_CHECK))/* あたり判定の無い弾は消さない。(発弾エフェクト用) */
	{
		return;
	}
	//
	/* 0-32 カウントまで */
	if ((JYUMYOU_1MIN-64-32) < src->jyumyou)/* 発弾エフェクト後から0カウント経過した弾 */
	{
		;/* 発弾すぐは何もしない */
	}
	else
	/* 32-64 カウントまで */
	if ((JYUMYOU_1MIN-64-64) < src->jyumyou)/* 発弾エフェクト後から32カウント経過した弾 */
	{
		//src->rotationCCW1024 += (256);/* 90/360 度 曲げてみるテスト */
		if ((src->tama_system_tama_data) & TAMA_DATA_RUMIA_SAYUU_BIT)
		{
		//	src->rotationCCW1024 += (8);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
			src->rotationCCW1024 += (10);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		}
		else
		{
		//	src->rotationCCW1024 -= (8);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
			src->rotationCCW1024 -= (10);/* 90/360 度 曲げてみるテスト(32カウントかけて曲げる。256==8*32) */
		}
	}
	else
	/* 64-256 カウントまで */
	if ((JYUMYOU_1MIN-64-256) < src->jyumyou)/* 発弾エフェクト後から64カウント経過した弾 */
	{
		if ((src->tama_system_tama_data) & TAMA_DATA_RUMIA_SAYUU_BIT)
		{
			src->rotationCCW1024++;/* 曲げてみるテスト */
		}
		else
		{
			src->rotationCCW1024--;/* 曲げてみるテスト */
		}
	}
	#if 0
	/* 256- カウントまで */
//	else
	{
		;/* 一定以上時間がたったら、何もしない */
	}
	#endif
}

static void s_check_bbb_maru_dan(SPRITE *src)/* 連弾 */
{
	if (0==(src->flags&SP_FLAG_COLISION_CHECK))/* あたり判定の無い弾は消さない。(発弾エフェクト用) */
	{
		return;
	}
	/* 32 カウントなら */
	if ((JYUMYOU_1MIN-64-32) == src->jyumyou)/* 発弾エフェクト後から32カウント経過した弾 */
	{
		if ((src->tama_system_tama_data) & TAMA_DATA_RUMIA_DIMMER_RENDAN_BIT)
		{
			src->tama_system_tama_data = (TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
			tmp_angleCCW65536_jiki_nerai(src);/* 自機狙い角作成 */
			#if 0
			/*(ダメ)*/
		//	src->rotationCCW1024 = (0);//描画用角度((src->tmp_angleCCW65536)>>6);/* 自機狙い角 */
			#else
			/*(特別ケース、このまま(見た目)の角度で移動する)*/
			src->rotationCCW1024 = ((src->tmp_angleCCW65536)>>6);/* 自機狙い角 */
			/* ここの描画用角度は後で利用する。 */
			#endif
			src->tama_system_speed65536 			= (t256(2.0)<<8);			/* 弾速 */
		}
	}
}

static void dimmer_tama_check(void)
{
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
		if (is_tama_grouip08(BULLET_KOME_00_SIRO) == is_tama_grouip08(s->type) )	/* 米弾なら */
		{
			s_check_aaa_kome_dan(s);
		}
		else
		if (is_tama_grouip08(BULLET_WAKU12_00_SIRO) == is_tama_grouip08(s->type) )	/* 枠付き丸弾なら */
		{
			s_check_bbb_maru_dan(s);
		}
	}
}
			#if 0
			unsigned int ttt[4]=
			{
				BULLET_KOME_03_AOI, 	/* 青弾 */
				BULLET_KOME_05_MIDORI,	/* 緑弾 */
				BULLET_KOME_01_AKA, 	/* 赤弾 */
				0
			};
			tama_color = ttt[tama_index];
			#endif


/*---------------------------------------------------------
	自機狙い連弾
---------------------------------------------------------*/

static void dimmer_shot_02_rendan(SPRITE *src)
{
	unsigned int ii;
	for (ii=0; ii<8; ii++)
	{
		int aaa_angle65536;
		#if 1
		tmp_angleCCW65536_jiki_nerai(src);/* 自機狙い角作成 */
		aaa_angle65536 = src->tmp_angleCCW65536;/* 自機狙い角 */
		#endif
		//	2048 == (65536)/(32)		: 1周を32分割した角度。
		//	8192 == (2048) * (4)		: 5弾目(4弾分の幅)。
		// 57344 == (65536) - (8192)	: 開始角度。(加算しないと誤差が出る。)
	//	aaa_angle65536 += (const int)(65536-(int)((65536*4)/(32)));/* 5弾目が自機狙い */
		if (0x00==((src->boss_spell_timer)&0x20))
				{	aaa_angle65536 += (57344);		}/* 5弾目(4弾分の幅)が自機狙い(65536-(4*(65536/32))) */
		else	{	aaa_angle65536 += (8192);		}/* 5弾目(4弾分の幅)が自機狙い(65536+(4*(65536/32))) */
		mask65536(aaa_angle65536);
		/* 角度(65536[360/360度]を 32分割) */
		unsigned int jj;
		for (jj=0; jj<16; jj++)
		{
		//	br.BULLET_REGIST_00_speed256				= (t256(1.5));								/* 弾速 */
			br.BULLET_REGIST_00_speed256				= (t256(0.75))+((16-(jj))<<5);				/* 弾速 */
			/* 弾速 0.75 - 2.75 倍 */
			br.BULLET_REGIST_01_speed_offset			= (ii<<6);//t256(0);/*(テスト)*/
			br.BULLET_REGIST_02_angle65536				= (aaa_angle65536); 	/* 発射中心角度 / 特殊機能(自機狙い/他) */
			br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_RUMIA_DIMMER_RENDAN_BIT)|(TAMA_DATA_8000_NON_TILT);/* (r33-)非傾き弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_03_AOI; 			/* [枠付き青丸弾] [枠付き緑丸弾] */
			tama_system_regist_single();/* (r33-) */
			/* 次の弾の角度 */
			//	 2048 == (65536)/(32)		: 1周を32分割した角度。
			//	63488 == (65536) - (2048)	: 減算の場合(加算しないと誤差が出る。)
			if (0x00==((src->boss_spell_timer)&0x20))
					{	aaa_angle65536 += (2048);		}/* 角度(65536[360/360度]を 32分割) (int)(65536/(32)); */
			else	{	aaa_angle65536 += (63488);		}/* 角度(65536[360/360度]を 32分割) (65536)-(int)(65536/(32)); */
			mask65536(aaa_angle65536);
		}
	}
}


/*---------------------------------------------------------
	交差弾。 青弾 / 緑弾 / 赤弾
---------------------------------------------------------*/

static void dimmer_shot_01_kousadan(SPRITE *src, u8 tama_type)
		{
			int tama_kakudo = (((tama_type)&0x06)<<8);/* 弾色別に発弾角を変える。(((tama_type)&0x06)<<6) */
			br.BULLET_REGIST_00_speed256				= (t256(1.0));			/* 弾速 (t256(1.5)) */
			br.BULLET_REGIST_01_speed_offset			= t256(0);/*(テスト)*/
			br.BULLET_REGIST_02_angle65536				= (tama_kakudo);					/* 発射中心角度 / 特殊機能(自機狙い/他) */
		//	br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_RUMIA_SAYUU_BIT);/* 特殊 */
			br.BULLET_REGIST_03_tama_data				= /* 特殊 */(TAMA_DATA_RUMIA_SAYUU_BIT)|(TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_00_SIRO+tama_type;			/* [枠つき青弾] [枠つき緑弾] */
			br.BULLET_REGIST_06_n_way					= (32); 				/* [32way] */	/* (16)[16way] */
			br.BULLET_REGIST_07_div_angle65536			= (65536/32);			/* 分割角度 (65536/16) */
			tama_system_regist_katayori_n_way();/*(r33-)*/
			/* br.BULLET_REGIST_02_angle65536はtama_system_regist_katayori_n_way();で破壊される(仕様な)ので再設定が必要。 */
			br.BULLET_REGIST_02_angle65536				= (tama_kakudo);					/* 発射中心角度 / 特殊機能(自機狙い/他) */
		//	br.BULLET_REGIST_03_tama_data				= (0);/* 戻す(必ず戻す事。戻し忘れるとシステム自体正常動作しない) */
			br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
			tama_system_regist_katayori_n_way();
		}


/*---------------------------------------------------------

---------------------------------------------------------*/

static void spell_create_29_rumia_demarcation(SPRITE *src)
{
	dimmer_tama_check();/* ディマーケイション用弾チェック */
	u8 tama_type;/* 弾色 */
	tama_type = (0);
	if ((0x1f)==((src->boss_spell_timer)&0x1f))
	{
		#define P0x80		0x80
		#define YASUMI000	0x00
		const u8 ttt[16] =
		{
		/*0x00*/	P0x80,	/* 自機狙い連弾 */
		/*0x01*/	P0x80,	/* 自機狙い連弾 */
		/*0x02*/	P0x80,	/* 自機狙い連弾 */
		/*0x03*/	P0x80,	/* 自機狙い連弾 */
		/*0x04*/	P0x80,	/* 自機狙い連弾 */
		/*0x05*/	P0x80,	/* 自機狙い連弾 */
		/*0x06*/	YASUMI000,	/* 休み */
		/*0x07*/	YASUMI000,	/* 休み */
		/*0x08*/	YASUMI000,	/* 休み */
		/*0x09*/	(BULLET_KOME_01_AKA    - BULLET_KOME_00_SIRO),		/* 赤弾 */
		/*0x0a*/	YASUMI000,	/* 休み */
		/*0x0b*/	(BULLET_KOME_05_MIDORI - BULLET_KOME_00_SIRO),		/* 緑弾 */
		/*0x0c*/	YASUMI000,	/* 休み */
		/*0x0d*/	(BULLET_KOME_03_AOI    - BULLET_KOME_00_SIRO),		/* 青弾 */
		/*0x0e*/	YASUMI000,	/* 休み */
		/*0x0f*/	YASUMI000	/* 休み */
		};
		tama_type = ttt[(((src->boss_spell_timer)>>5)&0x0f)];
	}
	if ((0) == tama_type)	/* お休み */
	{
		;
	}
	else
	if (P0x80 == tama_type) 	/* 自機狙い連弾 */
	{
		bullet_play_04_auto(VOICE14_SAKUYA_SHOT02);/*VOICE15_BOSS_KOUGEKI_01*/
		dimmer_shot_02_rendan(src);
	}
	else		/* 交差弾。 青弾 / 緑弾 / 赤弾 */
	{
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		dimmer_shot_01_kousadan(src, tama_type);
	}
}
