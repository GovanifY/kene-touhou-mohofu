
/*---------------------------------------------------------
 東方模倣風  ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	美鈴の弾幕を生成します。
---------------------------------------------------------*/


/*---------------------------------------------------------
	紅3面中ボス 紅美鈴
	通常攻撃1(1/2)
	16方位偏り全方位ばら撒き弾
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	紅3面中ボス 紅美鈴
	華符「芳華絢爛」にちょっとだけ似た弾幕(予定)
	時計回り、反時計回りの6way螺旋弾(黄色)		1回毎に(1024/(6*8))[1024/1024度]回転
	48方位全方位弾(赤色)						8回毎に発弾
	-------------------------------------------------------
	芳華絢爛
	解像度とか速度とかの要因で、そのままでは pspで難しい気もする。
	Lunatic はこのままで、Hard 以下は細工して易しくする。
---------------------------------------------------------*/
#if 1
static void danmaku_create_0a_houka_kenran(SPRITE *src)
{
	enum
	{
		H0=0,H1,H2,H3,H4,H5
	};
	#define HOUGA_00_YEL_NUMS			(H0*4)
	#define HOUGA_04_YEL_DIV_ANGLE		(H1*4)
	#define HOUGA_16_YEL_ROTATE_ANGLE	(H2*4)
//
	#define HOUGA_08_RED_NUMS			(H3*4)
	#define HOUGA_12_RED_DIV_ANGLE		(H4*4)
	const static s32/*s16 でいいかも？*/ houga_tbl[(H5*4)] =
	{/* easy					normal					hard					lunatic 			*/
		(4),					(8),					(5),					(6),					/* 註:3より4の方が簡単 */
		(int)(65536/(4)),		(int)(65536/(8)),		(int)(65536/(5)),		(int)(65536/(6)),		/* 註:3より4の方が簡単 */
		(int)(65536/(4*8)), 	(int)(65536/(8*8)), 	(int)(65536/(5*8)), 	(int)(65536/(6*8)), 	/* 註:3より4の方が簡単 */
//
		(4*8),					(4*8),					(5*8),					(6*8),					/* 註:3より4の方が簡単 */
		(int)(65536/(4*8)), 	(int)(65536/(4*8)), 	(int)(65536/(5*8)), 	(int)(65536/(6*8)), 	/* 註:3より4の方が簡単 */
	};

	//
			obj_send1->x256 					= (src->x256)+t256(25.0);				/* 弾源x256 */
			obj_send1->y256 					= (src->y256)+t256(16.0);				/* 弾源y256 */
//
//	if ((0x10)==((src->boss_base_danmaku_time_out)&0x1f))/* (16回に1回)(128なら計8回) */
	if ((0x40)==((src->boss_base_danmaku_time_out)&0x7f))/* (16回に1回)(128なら計8回) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		{
			tmp_angleCCW65536_jikinerai(obj_player, src);/*自機狙い角作成*/
			br.BULLET_REGIST_speed256			= (t256(1.75)); 									/* 弾速 */	/* 3.5 2.5 2.0*/
			br.BULLET_REGIST_angle65536 		= (src->tmp_angleCCW65536); 								/* 自機狙い弾 */
			br.BULLET_REGIST_div_angle65536 	= houga_tbl[HOUGA_12_RED_DIV_ANGLE+difficulty]; 	/*(int)(1024/(48))*/	/* 分割角度(1024[360/360度]を 48 分割) */	/* 1周をn分割した角度 */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KOME_02_AKA;								/* [赤色米弾] */
			br.BULLET_REGIST_n_way				= houga_tbl[HOUGA_08_RED_NUMS+difficulty];			/*(48)*/				/* [48way] */	/* 発弾数 */
			br.BULLET_REGIST_speed_offset		= (0);/*てすと*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
			bullet_regist_angle();/* bullet_regist_vector(); より若干シンプルな為、引数に互換がない。 */
		}
	}
//	if ((0x02)==((src->boss_base_danmaku_time_out)&0x03))/* (2回に1回)(8回毎に発弾) */
	if ((0x08)==((src->boss_base_danmaku_time_out)&0x0f))/* (2回に1回)(8回毎に発弾) */
	{
		static int aaa_angle65536;
		br.BULLET_REGIST_speed256			= (t256(1.75)); 										/* 弾速 */		/*3.5 2.5 2.0*/
		br.BULLET_REGIST_div_angle65536 	= houga_tbl[HOUGA_04_YEL_DIV_ANGLE+difficulty]; 		/*(int)(1024/(6))*/ 	/* 分割角度(1024[360/360度]を 6 分割) */	/* 1周をn分割した角度 */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KOME_05_KIIRO; 								/* [黄色米弾] */
		br.BULLET_REGIST_n_way				= houga_tbl[HOUGA_00_YEL_NUMS+difficulty];				/*(6)*/ 				/* [1way] */	/* 発弾数 */
		br.BULLET_REGIST_speed_offset		= (0);/*てすと*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*現在種類が無い*/
//
		// 順回り(下CCWだから、左回りCCW)
		br.BULLET_REGIST_angle65536 		= ((/*0+*/(aaa_angle65536))&(65536-1)); 				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		bullet_regist_angle();/* bullet_regist_vector(); より若干シンプルな為、引数に互換がない。 */
		// 逆回り(下CCWだから、右回りCW)
		br.BULLET_REGIST_angle65536 		= ((65536-(aaa_angle65536))&(65536-1)); 				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		bullet_regist_angle();/* bullet_regist_vector(); より若干シンプルな為、引数に互換がない。 */
		// 回転量
		aaa_angle65536 += houga_tbl[HOUGA_16_YEL_ROTATE_ANGLE+difficulty];							/*(1024/(6*8))*/		/* 角度(1024[360/360度]を 48分割) */
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

extern void exchange_damnaku_check_type(void);

static void danmaku_create_12_aya_merin_test(SPRITE *src)
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
	if (0xd0==((src->boss_base_danmaku_time_out)&0xdf))/* (256回に2回) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		int jj;
		for (jj=0; jj<(6); jj++)
		{
			br.BULLET_REGIST_speed256				= (t256(4.0)-(jj<<6));		/* 弾速 */
			br.BULLET_REGIST_angle65536 			= (0/65536);				/* 下向き */
			br.BULLET_REGIST_div_angle65536 		= (65536/24);				/* 分割角度([360/360]度を24分割) */ 	//step_tbl[(difficulty)];/*(32)*/
			int check_type = BULLET_KUNAI12_00_AOI+1-((src->boss_base_danmaku_time_out>>5)&1);	/*+(0&1)*/	/*(BULLET_KUNAI12_01_AKA-1)*/
			br.BULLET_REGIST_bullet_obj_type		= check_type;
			br.BULLET_REGIST_n_way					= (24);//step_tbl[(difficulty+4)];/*(32)*/
			br.BULLET_REGIST_speed_offset			= (-5); 		/* (-5) (-3)調整減速弾 */	/* この方式になるか検討中 */
		//	br.BULLET_REGIST_regist_type			= REGIST_TYPE_00_MULTI_VECTOR;		/* 現在これしかないが要る */
			{
				obj_send1->x256 					= (src->x256)+t256(25.0);				/* 弾源x256 */
				obj_send1->y256 					= (src->y256)+t256(16.0);				/* 弾源y256 */
			//	obj_send1->m_angleCCW1024			= (src->m_angleCCW1024);				/* 弾源y256 */
				bullet_regist_angle();	/* 角度弾として登録 */
			}
		}
	}
	else
	/* [変身] */
	if (0x90==((src->boss_base_danmaku_time_out)&0xdf))/* (256回に2回) */
	{
		/* 変身可能な弾の条件を設定 */
		br.BULLET_REGIST_bullet_obj_type		= BULLET_KUNAI12_00_AOI+1-((src->boss_base_danmaku_time_out>>5)&1); /*+(0&1)*/	/*(BULLET_KUNAI12_01_AKA-1)*/
		/* 今画面にある弾を全部調べて、弾を変身させる。 */
		exchange_damnaku_check_type();
	}
}
#endif
