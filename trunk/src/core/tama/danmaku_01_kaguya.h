
/*---------------------------------------------------------
 東方模倣風  ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	かぐやの弾幕を生成します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	かぐや
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
		obj_send1->x256 					= src->x256;
		obj_send1->y256 					= src->y256;
		br.BULLET_REGIST_angle65536 		= (kakudo65536);
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/24);				/* 分割角度([360/360]度を24分割) */
		br.BULLET_REGIST_n_way				= (24);
		br.BULLET_REGIST_speed_offset		= (0);		/* (-5) (-3)調整減速弾 */	/* この方式になるか検討中 */
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
//
		switch ((src->base_hp)&(0xe000) )
	//	switch (src->boss_base_bo ss_life)
		{
		case (3<<13):/*not_break;*/
			br.BULLET_REGIST_speed256		= t256(0.6);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_03_MIDORI; 	bullet_regist_angle();
			br.BULLET_REGIST_speed256		= t256(1.4);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_05_KIIRO;		bullet_regist_angle();
		case (2<<13):/*not_break;*/
		case (4<<13):/*not_break;*/
			br.BULLET_REGIST_speed256		= t256(1.0);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_00_AOI;		bullet_regist_angle();
			br.BULLET_REGIST_speed256		= t256(1.8);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_04_MIZUIRO;	bullet_regist_angle();
		case (5<<13):/*not_break;*/
		case (1<<13):/*not_break;*/
			br.BULLET_REGIST_speed256		= t256(1.2);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_02_YUKARI; 	bullet_regist_angle();
			br.BULLET_REGIST_speed256		= t256(0.8);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_01_AKA;		bullet_regist_angle();
			br.BULLET_REGIST_speed256		= t256(2.0);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_01_AKA;		bullet_regist_angle();
			break;
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
		obj_send1->x256 					= (src->x256);
		obj_send1->y256 					= (src->y256);
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
//	if (SPELL_CARD_00_kaguya_000 < spell_card_number)
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
				fire_wait3 = ((src->base_hp)>>(7));		/* 3*(8192)>>7 == 192 */
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
					obj_send1->x256 							= (src->x256);
					obj_send1->y256 							= (src->y256);
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
//
	bullet_clip_min.x256 = t256(-32);
	bullet_clip_max.x256 = t256(0)+t256(GAME_WIDTH);
	bullet_clip_min.y256 = t256(-32);
	bullet_clip_max.y256 = t256(0)+t256(GAME_HEIGHT);
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
