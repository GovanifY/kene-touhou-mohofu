
/*---------------------------------------------------------
 東方模倣風  ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	咲夜の弾幕を生成します。
---------------------------------------------------------*/


/*---------------------------------------------------------
	紅5面中ボス 咲夜 第一攻撃にちょっとだけ似た弾幕
	-------------------------------------------------------
	本家は固定弾幕(?)だけど、これは狙い系。
	(仮に固定弾幕だったとしても)そもそも上に行けないから、上避けは出来ない。
	(pspの縦解像度では上に行くスペースがない)
	下避けもpspの縦解像度が全然足りないから、本家風のチョン避けは無理。
	ある程度左右に動き回って避けて貰う事を想定している。(だから弾速遅い)
	それに併せて密着弾はあえてぜんぜん違う。(別変数とるのが面倒だった事もある)
---------------------------------------------------------*/

static void danmaku_create_01_sakuya_misogi(SPRITE *src)
{
	static int aaa_angle65536;
//
	if (0==((src->boss_base_danmaku_time_out)&0x07))
	{
		#if (1)
	//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif
	//	src->tmp_angleCCW1024 -= (int)(256/5)+(10); /*CCW*/
	//	src->tmp_angleCCW1024 -= (int)(1024/20)+(10); /*CCW*/
		src->tmp_angleCCW65536 -= (int)(65536/20)+(640); /*CCW*/
		mask65536(src->tmp_angleCCW65536);
		obj_send1->x256 					= (src->x256);						/* 弾源x256 */
		obj_send1->y256 					= (src->y256);						/* 弾源y256 */
		br.BULLET_REGIST_speed256			= (t256(2.5));						/* 弾速 */
		br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW65536));		/* 弾源角度65536 */ 		/* [螺旋状弾] */
	//	br.BULLET_REGIST_div_angle1024		= (int)(256/5);
	//	br.BULLET_REGIST_div_angle1024		= (int)(1024/20);
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/20);					/* 分割角度([360/360]度を20分割) */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [青ナイフ弾] */
		br.BULLET_REGIST_n_way				= (8);								/* [8way] */
		br.BULLET_REGIST_speed_offset		= (0);/*てすと*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
		bullet_regist_angle();/* bullet_regist_vector(); より若干シンプルな為、引数に互換がない。 */

		if (0==((src->boss_base_danmaku_time_out)&0x1f))/* reset (1/32) */
		{
		//	aaa_angle1024	= (src->tmp_angleCCW1024)-(int)((1024/2));
			aaa_angle65536	= (src->tmp_angleCCW65536)-(int)((65536/2));
		}
	}
	else
//	if ((10-1)>((((u8)src->boss_base_danmaku_time_out)^0x20)&0x2f))/* x 10way */
	if ((10-1)>(((src->boss_base_danmaku_time_out)	  )&0x0f))/* x 10way */
	{
	//	aaa_angle1024 -= (int)(256/5);
	//	aaa_angle1024 -= (int)(1024/20);
		aaa_angle65536 -= (int)(65536/20);
		mask65536(aaa_angle65536);
		obj_send1->x256 					= (src->x256);						/* 弾源x256 */
		obj_send1->y256 					= (src->y256);						/* 弾源y256 */
		br.BULLET_REGIST_speed256			= (t256(2.0));						/* 弾速 */
		br.BULLET_REGIST_angle65536 		= ((aaa_angle65536));				/* 弾源角度65536 */
	//	br.BULLET_REGIST_div_angle65536 	= (int)((difficulty+difficulty+1+1/*3*/)<<(6)); 	/* [密着弾]の分割角度 */
		br.BULLET_REGIST_div_angle65536 	= (int)((difficulty+1/*3*/)<<(6+1));		/* [密着弾]の分割角度 */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;			/* [赤クナイ弾] */
		br.BULLET_REGIST_n_way				= (4);								/* [4way] */
		br.BULLET_REGIST_speed_offset		= (0);/*てすと*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
		bullet_regist_angle();/* bullet_regist_vector(); より若干シンプルな為、引数に互換がない。 */
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
static void danmaku_create_02_24nerai(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x03))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		tmp_angleCCW65536_jikinerai(obj_player, src);/*自機狙い角作成*/
		obj_send1->x256 					= (src->x256);						/* 弾源x256 */
		obj_send1->y256 					= (src->y256);						/* 弾源y256 */
	//	br.BULLET_REGIST_speed256			= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* 弾速 */
		br.BULLET_REGIST_speed256			= (t256(2.0)-((src->boss_base_danmaku_time_out)));	/* 弾速 */
	//	#define TORIAEZU_000 (0)/* 謎の調整値 */
		#define TORIAEZU_000 (int)(6*64)/* 謎の調整値 */
		br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW65536)) + TORIAEZU_000;		/* 弾源角度1024 */	/* 自機狙い弾 */
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/24);					/* 分割角度 */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;			/* [赤クナイ弾] */
		br.BULLET_REGIST_n_way				= (24); 							/* [24way] */
		br.BULLET_REGIST_speed_offset		= (1);/*てすと*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
		bullet_regist_angle();/* bullet_regist_vector(); より若干シンプルな為、引数に互換がない。 */
	}
}


/*---------------------------------------------------------
	紅5面中ボス 咲夜 「奇術：ミスディレクション(2/2)」にちょっとだけ似た弾幕
	-------------------------------------------------------
	自機狙い、11弾(中心と、左右に5弾ずつ狙い)
	-------------------------------------------------------
	[注：現在bu llet_create_n_way_dan_sa_type()の自機狙いはワザト狙わない奇数弾なので、動かなければ当たりませんが、
	そのうちここは偶数弾にする予定なので、動かなければ当たるようになる予定です]
---------------------------------------------------------*/
static void danmaku_create_03_11nife(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x07))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		tmp_angleCCW65536_jikinerai(obj_player, src);/*自機ねらい角作成*/
		obj_send1->x256 					= (src->x256);						/* 弾源x256 */
		obj_send1->y256 					= (src->y256);						/* 弾源y256 */
		br.BULLET_REGIST_speed256			= (t256(2.0));						/* 弾速 */
	//	#define TORIAEZU_111 (0)/* 謎の調整値 */
		#define TORIAEZU_111 (int)(4*64)/* 謎の調整値 */
		br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW65536))-(int)(65536*6/(7*4)) + TORIAEZU_111;		/* 弾源角度1024 */
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/(7*4));				/* 分割角度([90/360]度を7分割==28分割) */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [青ナイフ弾] */
		br.BULLET_REGIST_n_way				= (11); 							/* [11way] */
		br.BULLET_REGIST_speed_offset		= (0);/*てすと*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
		bullet_regist_angle();/* bullet_regist_vector(); より若干シンプルな為、引数に互換がない。 */
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
		obj_send1->y256 						= (src->y256);						/* 弾源y256 */	/* 本物は発弾位置の offset 用 */
		obj_send1->tmp_angleCCW65536 = (0);/* 下向き */
	//
		//	br.BULLET_REGIST_div_angle1024		= (0);								/* ダミー分割角度(未使用) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_06_YUKARI; 		/* [ピンクナイフ弾] */
			br.BULLET_REGIST_n_way				= (1);								/* [1way] */
			br.BULLET_REGIST_speed_offset		= (0);/*てすと*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
		//
		int jj;
		jj=0;
		int i;
		for (i=0; i<(13*12)/*144*/; i+= (13) )/*12方向*/							/* 角度(128[(90+a)/360度]を 10 分割) */
		{
			obj_send1->x256 					= src->x256+t256(4.0);				/* 本物は発弾位置の x offset あり */
			br.BULLET_REGIST_speed256			= ((160-i)<<(1+1+1));				/* 弾速 */
			br.BULLET_REGIST_angle65536			= (jj/*(0+i+i)&(1024-1)*/);
			bullet_regist_angle();/* bullet_regist_vector(); より若干シンプルな為、引数に互換がない。 */
			//
			obj_send1->x256 					= src->x256-t256(4.0);				/* 本物は発弾位置の x offset あり */
			br.BULLET_REGIST_speed256			= ((160-i)<<(1+1+1));				/* 弾速 */
			br.BULLET_REGIST_angle65536			= ((0-jj)&(65536-1));
			bullet_regist_angle();/* bullet_regist_vector(); より若干シンプルな為、引数に互換がない。 */
			//
			jj += (13*2*64);
		}
	}
}


/*---------------------------------------------------------
	紅5面ボス 咲夜 「通常攻撃1(2/2)」にちょっとだけ似た弾幕(予定)
	-------------------------------------------------------
	-------------------------------------------------------

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
			obj_send1->x256 					= (src->x256);						/* 弾源x256 */
			obj_send1->y256 					= (src->y256);						/* 弾源y256 */
		//
	//		br.BULLET_REGIST_div_angle1024		= (0);								/* ダミー分割角度(未使用) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [青ナイフ弾] */
			br.BULLET_REGIST_n_way				= (1);								/* [1way] */
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		//
		int i;
		for (i=0; i<(1024); i+= 32 )/*32方向*/	/* 角度(1024[360/360度]を 32 分割)1024 == 32 x 32 */
		{
			br.BULLET_REGIST_speed256			= (t256(2.0)-((i&32)<<3));			/*奇数は高速弾*/
			br.BULLET_REGIST_angle1024			= (i/*(0+i)&(1024-1)*/);			/* 弾源角度1024 */
			bullet_regist_vector();
		}
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
		br.BULLET_REGIST_speed256			= (t256(2.0));				/* 弾速 */
		br.BULLET_REGIST_angle1024			= (0);						/* 発射中心角度 / 特殊機能(自機狙い/他) */
		br.BULLET_REGIST_div_angle1024		= (int)(1024/(32)); 		/* 分割角度(1024[360/360度]を 32 分割) */	/* 1周をn分割した角度 */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_CAP16_05_TUTU_SIROI;	/* [青白実包弾] */	/* 弾グラ */
		br.BULLET_REGIST_n_way				= (32); 					/* [32way] */		/* 発弾数 */
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_vector();
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
			obj_send1->x256 					= (src->x256);		/* 弾源x256 */
			obj_send1->y256 					= (src->y256);		/* 弾源y256 */
	//		br.BULLET_REGIST_div_angle1024		= (0);									/* ダミー分割角度(未使用) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;				/* [青ナイフ弾] */
			br.BULLET_REGIST_n_way				= (1);									/* [1way] */
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
	//
		int ii; 	/* 1周を65536分割した角度 */
		int j;		/* カウンタ */
		j=0;
		for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)	12.8==(1024/80) 80分割 [5分割] */	/* 204.8==(1024/5) 角度(1024[72/360度]を 5 分割) */
		{
			j++;
			j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
			br.BULLET_REGIST_speed256			= (t256(3.0)-((j)<<5)-((j&1)<<7));		/*奇数は高速弾*/
			br.BULLET_REGIST_angle1024			= (((0-ii)>>6)&(1024-1));				/* 角度 */
			bullet_regist_vector();
		}
	}
}


/*---------------------------------------------------------
	メイド秘技「操りドール」
---------------------------------------------------------*/
#if 0
#endif
