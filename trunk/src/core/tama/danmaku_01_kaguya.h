
/*---------------------------------------------------------
 東方模倣風  ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	輝夜の弾幕を生成します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	輝夜
	-------------------------------------------------------
	無理やり追加
---------------------------------------------------------*/

/*---------------------------------------------------------
	4面専用
---------------------------------------------------------*/

static void kaguya_uroko_dan_seisei(SPRITE *src)
{
	/* 4面の場合---- */
	const Uint16 my_ra_nd = ra_nd();
	if (my_ra_nd & 0x8000)		/* 50%の確率で */
	{
		int kakudo65536;
	//	kakudo65536 = ((255-32)<<6);
	//	kakudo65536 = ((256-32)<<6);
	//	kakudo65536 = (((1024/4)-(1024/32))<<6);
		kakudo65536 = ((65536/4)-(65536/32));
		kakudo65536 += ((my_ra_nd)&((/*64<<6*/4096)-1));
//
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
		obj_send1->cx256 					= src->cx256;
		obj_send1->cy256 					= src->cy256;
		br.BULLET_REGIST_angle65536 		= (kakudo65536);
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/24);				/* 分割角度([360/360]度を24分割) */
		br.BULLET_REGIST_n_way				= (24);
		br.BULLET_REGIST_speed_offset		= t256(0);		/* (-5) (-3)調整減速弾 */	/* この方式になるか検討中 */
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
//
	static const void *JumpTable[(8)] =
	{
		&&OP0x00, &&OP0x01, &&OP0x02, &&OP0x03,
		&&OP0x04, &&OP0x05, &&OP0x06, &&OP0x07,
	};
	//	goto ((src->base_hp)&(0xe000) )
		goto *JumpTable[((src->base_hp>>13)&(0x07))];
	//	switch (src->boss_base_bo ss_life)
		{
		OP0x03:/*not_break;*/
			br.BULLET_REGIST_speed256		= t256(0.6);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_03_MIDORI; 	bullet_regist_angle();
			br.BULLET_REGIST_speed256		= t256(1.4);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_05_KIIRO;		bullet_regist_angle();
		OP0x02:/*not_break;*/
		OP0x04:/*not_break;*/
			br.BULLET_REGIST_speed256		= t256(1.0);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_00_AOI;		bullet_regist_angle();
			br.BULLET_REGIST_speed256		= t256(1.8);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_04_MIZUIRO;	bullet_regist_angle();
		OP0x05:/*not_break;*/
		OP0x01:/*not_break;*/
			br.BULLET_REGIST_speed256		= t256(1.2);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_02_YUKARI; 	bullet_regist_angle();
			br.BULLET_REGIST_speed256		= t256(0.8);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_01_AKA;		bullet_regist_angle();
			br.BULLET_REGIST_speed256		= t256(2.0);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_01_AKA;		bullet_regist_angle();
		OP0x00:
		OP0x06:
		OP0x07:
			;
		}
	}
}


/*---------------------------------------------------------
	左右ショット
---------------------------------------------------------*/

static void kaguya_hari_shot(SPRITE *src)
{
	static int hari=0;
	hari--;
	hari &= 32;
	voice_play(VOICE11_BOSS_KIRARIN/*VOICE14_BOSS_KOUGEKI_01*/, TRACK04_TEKIDAN);/*テキトー*/
//
	int hari_no_iro;
	hari_no_iro = BULLET_HARI32_00_AOI;
	/* 4面の場合---- */
	{
		unsigned short iro_tbl[8] =
		{
/**/		BULLET_HARI32_01_AKA,
/*1*/		BULLET_HARI32_01_AKA,
/*2*/		BULLET_HARI32_03_DAIDAI,
/*3*/		BULLET_HARI32_02_KIIRO,
/*4*/		BULLET_HARI32_00_AOI,
/*5*/		BULLET_HARI32_00_AOI,
/**/		BULLET_HARI32_00_AOI,
/**/		BULLET_HARI32_00_AOI,
		};
		hari_no_iro = iro_tbl[(((src->base_hp)&(0xe000) )>>(13))];
	//	hari_no_iro = iro_tbl[(src->boss_base_bo ss_life)];
	}
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
		obj_send1->cx256 					= (src->cx256);
		obj_send1->cy256 					= (src->cy256);
	//	br.BULLET_REGIST_div_angle1024		= (0);				/* ダミー分割角度(未使用) */
		br.BULLET_REGIST_bullet_obj_type	= hari_no_iro;		/* [ 弾] */
		br.BULLET_REGIST_n_way				= (1);				/* [1way] */
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
//
	int angle1024;
	int spd;
	spd = (0x180)+(ra_nd() & 0x00ff);
	for (
			angle1024 = (0);	/*cv1024r(0)*/
		//	angle1024 < /*=*/cv1024r(360/*180*/);
			angle1024 < ((4/*difficulty*/+1)<<10);	//	/*=*/ cv1024r(360/*180*/)
		//	angle512 += (/*22*/21+hari-difficulty/*cv1024r(20)*/)		/* 23本(360) ← 8本(180) */
			angle1024 += (/*44*/36+hari+hari-(difficulty<<2) /*cv1024r(20)*/)	/* 最低 == 10 == 48本(360)	*/
		)
	{
		br.BULLET_REGIST_speed256			= (int)(spd);				/* 弾速 */	/*6*/	/*t256(2.5)*/
		br.BULLET_REGIST_angle1024			= angle1024;
		bullet_regist_vector();
		spd += (64>>3/*difficulty*/);
	}
}

/*---------------------------------------------------------
	左右ショット
---------------------------------------------------------*/

static void kaguya_hari_dan_seisei(SPRITE *src)
{
//	if (SPELL_CARD_00_kaguya_000 < sp ell_card_number)
	{
		static int fire_wait1;
		fire_wait1--;
		if (0 > fire_wait1)
		{
			fire_wait1 = (45*4);
			kaguya_hari_shot(src);/*enemy_boss04_fire(2);*/
		}
	}
}


/*---------------------------------------------------------
	4面の場合跳ねる珠
---------------------------------------------------------*/

static int fire_wait3;
static int bomb_n;
static void kaguya_pong_boll(SPRITE *src)
{
	/* 4面の場合跳ねる珠 */
	{
		/* 体力ゲージが 3本以下になったら、跳ねる珠を落とし始める。 */
		//switch ( (src->base_hp)&0xe000 )
		if (src->base_hp < (3*(8192)))
		//if (src->boss_base_bo ss_life < 2)
		{
			fire_wait3--;
			if (fire_wait3 < 0)
			{
			//	fire_wait3 = ((src->base_hp)>>(2+3));
				fire_wait3 = ((src->base_hp)>>(7)); 	/* 3*(8192)>>7 == 192 */
				//
				#define LIMIT_LOW50 (/*50*/64)
				if (fire_wait3 < (LIMIT_LOW50))
				{	fire_wait3 = (LIMIT_LOW50);}
			//
				bomb_n++;
				if (31 < bomb_n)	{	bomb_n = 31;	}
			//	1 ... 31
				br.BULLET_REGIST_regist_type				= REGIST_TYPE_01_HAZUMI;
				// 341.333333333333333333333333333333 ==  1024 / 3
				int jj;
			//	jj = (100)*(11-(int)((bomb_n*(0x100/3))>>8));
				jj = (1024*3)/(bomb_n+(2*3));
				int ii;
			//	for (ii=0; i <= (int)((bomb_n*(0x100/3))>>8); i++)
				for (ii=0; ii < (1024); ii+=jj)
				{
				/* 中心座標なので、オフセットなし==ボス中心から弾出す。 */
					obj_send1->cx256 							= (src->cx256);
					obj_send1->cy256 							= (src->cy256);
					br.BULLET_REGIST_speed256					= t256(0.05)+(difficulty<<6);			/* 速度 t256(5.0) */
					br.BULLET_REGIST_angle1024					= (ii); 				/* 角度 */
					br.BULLET_REGIST_jyuryoku_dan_delta256		= (t256(0.07)); 						/* delta256 */	/*17.92==t256(0.07)*/
					br.BULLET_REGIST_jyuryoku_dan_bound_counts	= (difficulty); 						/* bound_counts (2) */
					bullet_regist_vector();
				}
			}
		}
	}
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
extern void add_zako_kaguya_houmotsu(SPRITE *src);
global void boss_init_kaguya(SPRITE *h)
{
	fire_wait3			= 0;
	bomb_n				= 0;
//	h->callback_mover	= move_kaguya;
	add_zako_kaguya_houmotsu(h);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
static void danmaku_create_18_kaguya01(SPRITE *src)
{
//	if (50==((src->boss_base_danmaku_time_out) ))
	if ((64-10)==((src->boss_base_danmaku_time_out) ))
	{
		kaguya_uroko_dan_seisei(src);
	}
	kaguya_hari_dan_seisei(src);
}
static void danmaku_create_19_kaguya04(SPRITE *src)
{
//	if (50==((src->boss_base_danmaku_time_out) ))
	if ((64-10)==((src->boss_base_danmaku_time_out) ))
	{
		kaguya_uroko_dan_seisei(src);
	}
	kaguya_hari_dan_seisei(src);
	kaguya_pong_boll(src);
}



/*---------------------------------------------------------
	輝夜、蓬莱の玉の枝もどき
	神宝「蓬莱の玉の枝 - 夢色の郷 -」
	-------------------------------------------------------
	ぜんぜん作ってない...
//	１．輝夜が直接撃つ弾は16方向弾→17方向弾→16方向弾へジョジョに変化すｕｒｒｒｒｒｒｒｒｒｒｒｒｒｒｒｙ
	１．輝夜が直接撃つ弾は8方向弾が2セット。(?)
		セット別に回転方向が違う(?)。
	２．弓状にならんだオプションから始めだけ全方位(48方向)弾発射。
	３．弓状にならんだオプションから画面左右端に反転色米弾発射し、
		画面左右端で通常色米弾のプレイヤー狙い弾に変わる。
---------------------------------------------------------*/
// 4096 == 65536/16
// 3855.05882352941176470588235294118 == 65536/17
//	240.941176470588235294117647058824 == ((65536/16)-(16636/17)) 差分
static void danmaku_create_23_kaguya_tamanoe(SPRITE *src)
{
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
		br.BULLET_REGIST_bullet_obj_type	= (BULLET_MARU10_01_AKA-1)+aaa; 								/* [ 弾] */
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
