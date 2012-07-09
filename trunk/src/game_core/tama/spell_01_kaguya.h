
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	輝夜のカードを定義します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	輝夜
	-------------------------------------------------------
	無理やり追加
---------------------------------------------------------*/

/*---------------------------------------------------------
	4面専用
---------------------------------------------------------*/

local OBJ_CALL_FUNC(kaguya_uroko_dan_seisei)
{
	/* 4面の場合---- */
	const u16 my_ra_nd = ra_nd();
	if (my_ra_nd & 0x8000)		/* 50%の確率で */
	{
		s32 kakudo65536;
	//	kakudo65536 = ((255-32)<<6);
	//	kakudo65536 = ((256-32)<<6);
	//	kakudo65536 = (((1024/4)-(1024/32))<<6);
		kakudo65536 = ((65536/4)-(65536/32));
		kakudo65536 += ((my_ra_nd)&((/*64<<6*/4096)-1));
		HATSUDAN_02_speed_offset			= t256(0);			/* (-5) (-3)調整減速弾 */	/* この方式になるか検討中 */
		HATSUDAN_03_angle65536				= (kakudo65536);
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		HATSUDAN_06_n_way					= (24);
		HATSUDAN_07_div_angle65536			= (int)(65536/24);				/* 分割角度([360/360]度を24分割) */
//
	static const void *JumpTable[(8)] =
	{
		&&OP0x00, &&OP0x01, &&OP0x02, &&OP0x03,
		&&OP0x04, &&OP0x05, &&OP0x06, &&OP0x07,
	};
		goto *JumpTable[((src->base_hp>>13)&(0x07))];
		{
		OP0x03:/*not_break;*/
			HATSUDAN_01_speed256	= t256(0.6);	HATSUDAN_05_bullet_obj_type = (BULLET_UROKO14_BASE + TAMA_IRO_05_MIDORI);	hatudan_system_regist_katayori_n_way();
			HATSUDAN_01_speed256	= t256(1.4);	HATSUDAN_05_bullet_obj_type = (BULLET_UROKO14_BASE + TAMA_IRO_06_KI_IRO);	hatudan_system_regist_katayori_n_way();
		OP0x02:/*not_break;*/
		OP0x04:/*not_break;*/
			HATSUDAN_01_speed256	= t256(1.0);	HATSUDAN_05_bullet_obj_type = (BULLET_UROKO14_BASE + TAMA_IRO_03_AOI);		hatudan_system_regist_katayori_n_way();
			HATSUDAN_01_speed256	= t256(1.8);	HATSUDAN_05_bullet_obj_type = (BULLET_UROKO14_BASE + TAMA_IRO_04_MIZU_IRO); hatudan_system_regist_katayori_n_way();
		OP0x05:/*not_break;*/
		OP0x01:/*not_break;*/
			HATSUDAN_01_speed256	= t256(1.2);	HATSUDAN_05_bullet_obj_type = (BULLET_UROKO14_BASE + TAMA_IRO_02_YUKARI);	hatudan_system_regist_katayori_n_way();
			HATSUDAN_01_speed256	= t256(0.8);	HATSUDAN_05_bullet_obj_type = (BULLET_UROKO14_BASE + TAMA_IRO_01_AKA);		hatudan_system_regist_katayori_n_way();
			HATSUDAN_01_speed256	= t256(2.0);	HATSUDAN_05_bullet_obj_type = (BULLET_UROKO14_BASE + TAMA_IRO_01_AKA);		hatudan_system_regist_katayori_n_way();
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

local OBJ_CALL_FUNC(kaguya_hari_shot)
{
	static int hari=0;
	hari--;
	hari &= 32;
//
	int hari_no_iro;
	hari_no_iro = BULLET_HARI32_00_AOI;
	{	/* 4面の場合---- */
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
	//	hari_no_iro = iro_tbl[(src->boss_base_bo ss_life)];
	//	hari_no_iro = iro_tbl[(((src->base_hp)&(0x0000e000) )>>(13))];/*(r32)*/
		hari_no_iro = iro_tbl[(((src->base_hp)&(0x0001c000) )>>(14))];/*(r33)*/
	//											0x0001c000 == 114688 == 3.5*32*1024
	}
		HATSUDAN_02_speed_offset		= t256(0);/*(テスト)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
		HATSUDAN_05_bullet_obj_type 	= hari_no_iro;		/* [ 弾] */
	//
	int jj;
	int spd;/*(弾速)*/
	spd = t256(1.5) + /*(t256(0.0 ... 0.99)の乱数)*/(ra_nd() & 0x00ff);
	for (
			jj = (0);
			jj < ((4/*difficulty*/+1)<<16);
		//	angle512 += (/*22*/21+hari-difficulty/*cv1024r(20)*/)		/* 23本(360) ← 8本(180) */
			jj += ((/*44*/36<<6)+((hari)<<7)-((REG_0f_GAME_DIFFICULTY)<<(8)) /*cv1024r(20)*/) /* 最低 == 10 == 48本(360)	*/
		)
	{
		HATSUDAN_01_speed256			= (int)(spd);				/* 弾速 */	/*6*/	/*t256(2.5)*/
		HATSUDAN_03_angle65536			= (jj);
		hatudan_system_regist_single();
		spd += (64>>3/*difficulty*/);
	}
	//
	AUDIO_18_voice_number	= VOICE11_BOSS_KIRARIN;
	AUDIO_19_voice_truck	= TRACK04_TEKIDAN;/* テキトー */
	cpu_voice_play();
}

/*---------------------------------------------------------
	左右ショット
---------------------------------------------------------*/

local OBJ_CALL_FUNC(kaguya_hari_dan_seisei)
{
//	if (CARD_ADDRESS_00_kaguya_000 < sp ell_card_number)
	{
		static int fire_wait1;
		fire_wait1--;
		if (0 > fire_wait1)
		{
			fire_wait1 = (45*4);
			kaguya_hari_shot(src);
		}
	}
}


/*---------------------------------------------------------
	4面の場合跳ねる珠
---------------------------------------------------------*/

static int fire_wait3;
static int bomb_aaa;
local OBJ_CALL_FUNC(kaguya_pong_boll)
{
	/* 4面の場合跳ねる珠 */
	{
		/* 体力ゲージが 3本以下になったら、跳ねる珠を落とし始める。 */
		//sw itch ( (src->base_hp)&0xe000 )
		//if (src->boss_base_bo ss_life < 2)
	//	if (src->base_hp < (3*(8192)))/*(r32)*/
		if (src->base_hp < (3*(1024*32)))/*(r33)*/
		{
			fire_wait3--;
			if (fire_wait3 < 0)
			{
			//	fire_wait3 = ((src->base_hp)>>(2+3));
			//	fire_wait3 = ((src->base_hp)>>(7)); 	/* 3*(8192)>>7 == 192 */
				fire_wait3 = ((src->base_hp)>>(7+2));	/* 3*(32768)>>9 == 192 */
				//
				#define LIMIT_LOW50 (/*50*/64)
				if (fire_wait3 < (LIMIT_LOW50))
				{	fire_wait3 = (LIMIT_LOW50);}
			//	1 ... 40
					#if 0	/*古い*/
					HATSUDAN_01_speed256					= t256(0.05)+((REG_0f_GAME_DIFFICULTY)<<6); 	/* 速度 t256(5.0) */
					HATSUDAN_05_bullet_obj_type 			= BULLET_MINI8_BASE + ( TAMA_IRO_04_MIZU_IRO +(REG_0f_GAME_DIFFICULTY));
				//	HATSUDAN_06_n_way						= (8);
					HATSUDAN_07_VECTOR_leg acy_dan_delta256 = (t256(0.07)); 								/*17.92==t256(0.07)*/
			//	bomb_aaa += 9;
			//	if (448 < bomb_aaa) 	{	bomb_aaa = 448; 	}	/* 512-448 == 64 == 1024/16...方向 / 最大16方向の場合。 */
			//	if (433 < bomb_aaa) 	{	bomb_aaa = 433; 	}	/* 512-433 == 79 == 1024/13...方向 / 最大13方向の場合。 */
			//	if (427 < bomb_aaa) 	{	bomb_aaa = 427; 	}	/* 512-427 == 85 == 1024/12...方向 / 最大12方向の場合。 */
			//	if (419 < bomb_aaa) 	{	bomb_aaa = 419; 	}	/* 512-419 == 93 == 1024/11...方向 / 最大11方向の場合。 */
				int ii;
			//	for (ii=0; ii<(1024); ii+=(512-bomb_aaa))	/* 精々最大16方向ぐらい出ないとくぐって避けれない。 */
				{
					HATSUDAN_03_angle65536				= ((ii)<<6);	/* 角度 */
					bullet_regist_leg acy_vector_direct();
				}
					#else	/*新しい*/
					HATSUDAN_01_speed256					= t256(0.05)+((REG_0f_GAME_DIFFICULTY)<<6); 	/* 速度 t256(5.0) */
					HATSUDAN_02_speed_offset				= (t256(0.07)); 								/*17.92==t256(0.07)*/
					HATSUDAN_04_tama_spec					= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
					HATSUDAN_05_bullet_obj_type 			= BULLET_MINI8_BASE + ( TAMA_IRO_04_MIZU_IRO +(REG_0f_GAME_DIFFICULTY));
				bomb_aaa += (9*64);
				if ((419*64) < bomb_aaa)	{	bomb_aaa = (419*64);	}	/* 512-419 == 93 == 1024/11...方向 / 最大11方向の場合。 */
				int ii;
				for (ii=0; ii<(65536); ii+=((65536/2)-bomb_aaa))	/* 16方向より増やすと、くぐって避けるのが難しくなりすぎるので最大でも16方向。 */
				{
					HATSUDAN_03_angle65536				= (ii); 		/* 角度 */
					hatudan_system_regist_single();
				}
					#endif
			}
		}
	}
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
global OBJ_CALL_FUNC(boss_init_kaguya)
{
	init_set_dolls_kaguya_T01(src);/*(使い魔システム)*/
	fire_wait3			= 0;
	bomb_aaa			= 0;
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_18_kaguya01)
{
//	if (50==((REG_10_BOSS_SPELL_TIMER) ))
	if ((64-10)==((REG_10_BOSS_SPELL_TIMER) ))
	{
		kaguya_uroko_dan_seisei(src);
	}
	kaguya_hari_dan_seisei(src);
}
local OBJ_CALL_FUNC(spell_create_19_kaguya04)
{
//	if (50==((REG_10_BOSS_SPELL_TIMER) ))
	if ((64-10)==((REG_10_BOSS_SPELL_TIMER) ))
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
//	１．輝夜が直接撃つ弾は16方向弾→17方向弾→16方向弾へジョジョに変化する。
	１．輝夜が直接撃つ弾は8方向弾が2セット。(?)
		セット別に回転方向が違う(?)。
	２．弓状にならんだオプションから始めだけ全方位(48方向)弾発射。
	３．弓状にならんだオプションから画面左右端に反転色米弾発射し、
		画面左右端で通常色米弾のプレイヤー狙い弾に変わる。
	-------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	珠の色(1 ... 7)。	//	static u32 REG_09_REG1;
	REG_09_REG1 	角度?				//	static u32 REG_08_REG0;
---------------------------------------------------------*/
// 4096 == 65536/16
// 3855.05882352941176470588235294118 == 65536/17
//	240.941176470588235294117647058824 == ((65536/16)-(16636/17)) 差分
local OBJ_CALL_FUNC(spell_create_23_kaguya_tamanoe)
{
//	if ((0x02)==((REG_10_BOSS_SPELL_TIMER)&0x03))/* (2回に1回)(8回毎に発弾) */
	if ((0x04)==((REG_10_BOSS_SPELL_TIMER)&0x07))/* (2回に1回)(8回毎に発弾) */
	{
	#if (1)//[-r39]変数化するとスペカ切り替え時にリセットされる。
		static u32 kaguya_REG_09_REG1;
		static u32 kaguya_REG_08_REG0;
	#else//[r40]変数化するとスペカ切り替え時にリセットされる。
		#define kaguya_REG_09_REG1 REG_09_REG1
		#define kaguya_REG_08_REG0 REG_08_REG0
	#endif
		kaguya_REG_08_REG0++;
		kaguya_REG_08_REG0 &= (0x07);
		if (0==kaguya_REG_08_REG0)	{	kaguya_REG_08_REG0++;	};
		//
//		HATSUDAN_01_speed256			= (t256(1.00)); 			/* 弾速 */
		HATSUDAN_01_speed256			= (t256(0.50)); 			/* 弾速 */
//		HATSUDAN_02_speed_offset		= t256(0);/*(テスト)*/
		HATSUDAN_02_speed_offset		= t256(1);/*(テスト)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_MARU10_BASE) + kaguya_REG_08_REG0;	/* [中丸弾] */
	//	HATSUDAN_06_n_way				= (17); 						/* [17way] */	/* 発弾数 */
		HATSUDAN_06_n_way				= (16/*8*/);					/* [16way] */	/* 発弾数 */
	//	HATSUDAN_07_div_angle65536		= (int)(65536/(16))-(((si n1024(( ra_nd() & REG_10_BOSS_SPELL_TIMER)))&0xff));	/*(int)(1024/(6))*/ 	/* 分割角度(1024[360/360度]を 6 分割) */	/* 1周をn分割した角度 */
		HATSUDAN_07_div_angle65536		= (int)(65536/(16/*8*/));		/* 1周を16分割した角度 */
//
//		HATSUDAN_03_angle65536			= ((65536/2));					/* 発射中心角度 / 特殊機能(自機狙い/他) */
//		HATSUDAN_03_angle65536			= (int)((65536*1/4))+(REG_10_BOSS_SPELL_TIMER&0xff);				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		kaguya_REG_09_REG1 += (77);
		HATSUDAN_03_angle65536			= ((kaguya_REG_09_REG1));						/* 発射中心角度 / 特殊機能(自機狙い/他) */
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x08))
		{
//			HATSUDAN_03_angle65536		= (int)((65536*3/4))+(REG_10_BOSS_SPELL_TIMER&0xff);				/* 発射中心角度 / 特殊機能(自機狙い/他) */
			HATSUDAN_03_angle65536		= -(kaguya_REG_09_REG1);				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		}
		hatudan_system_regist_katayori_n_way();/* (r33-) */
	}
	/*(r40てすと)*/
//	if ((64-10)==((REG_10_BOSS_SPELL_TIMER) ))
//	{
//		kaguya_hari_dan_seisei(src);
//	}
}



/*---------------------------------------------------------
	蓬莱の玉の枝もどき
	-------------------------------------------------------
	せっかく模倣風で輝夜が出るのだから、
	蓬莱の玉の枝もどきだけ作りたい気がする。
	レーザーも要らないし。
	以下メモ。
//
	赤紫青水緑黄橙の順で7色繰り返しで出てくる。画面中央上部の輝夜からは全方位結構ゆっくり速度丸弾。
	ほぼ等速だか微妙に加速してる気もする。(気のせいかも)
	輝夜は上下に揺れているが、射出位置は揺れないで固定。
	全方位弾(開幕時は16方向全方位弾)だが少しずつ(ランダムっぽい角度で)弾数が増える。
	(つまり分割角度が減る)
	17方向全方位弾迄、増えると今度は16方向全方位弾まで減る(多分)。それの繰り返し。
//
	画面中央下部(というかほぼ画面中心)に魔方陣が半分ずつ重なって7つ。扇状配置。
	開幕時だけ、全方位米弾撃ってくる。
	下部に魔方陣からは左右に反転色、米弾。角魔方陣からは同時に弾が出る。
	弾速は比較的ゆっくりだが、輝夜丸弾より速い。
	こちらもほぼ等速だか微妙に加速してる気もする。(気のせいかも)
	色の順序は左側は上と同じ。右側は逆順。
	サインっぽい感じでうねって射出。7色撃つとしばし休む。
	右はangle=90[度]+(sin(周期++)*微角度定数)。
	左はangle=270[度]+(sin(周期++)*微角度定数)。みたいにすればいいのかも？
	(微角度定数は+-11.25度(360/(2^5))ぐらい？なら>>4とかでいいかな？)
	反転色、米弾は、左右の壁にあたると色が同色で反転し(例えば赤反転色米弾が赤米弾になるという意味)、
	自機狙い弾になる。
	-------------------------------------------------------
	大体こんな感じだと思うけど、間違ってるかもしれない。
	以上を満たせば、かなり細部の動きが違っても、似た感じにならないかなあ？
---------------------------------------------------------*/
