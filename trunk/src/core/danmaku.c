
#include "bullet_object.h"

/*---------------------------------------------------------
	弾幕生成
	-------------------------------------------------------
	★弾幕はシステムでサポートする構想があります。(構想中)
	システムでサポートされると、どのボスが、どの弾幕でも撃てるようになります。
---------------------------------------------------------*/


/*---------------------------------------------------------
	自機狙い弾の角度を計算
---------------------------------------------------------*/

/*static*/ void tmp_angle_jikinerai512(SPRITE *p, SPRITE *t)
{
	/* 簡略版(弾と自分が大きさが同じならずれない、違うとその分誤差になる) */
	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256, p->x256-t->x256));
}


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
	static int aaa_angle512;
//
	if (0==((src->boss_base_danmaku_time_out)&0x07))
	{
		#if (1)
	//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif
		src->tmp_angleCCW512 -= (int)(128/5)+(5);	/*CCW*/
		mask512(src->tmp_angleCCW512);
		send1_obj->x256 							= (src->x256);						/* 弾源x256 */
		send1_obj->y256 							= (src->y256);						/* 弾源y256 */
		send1_obj->BULLET_REGIST_speed256			= (t256(2.5));						/* 弾速 */
		send1_obj->BULLET_REGIST_angle512			= (src->tmp_angleCCW512);			/* 弾源角度512 */			/* [螺旋状弾] */
		send1_obj->BULLET_REGIST_div_angle512		= (int)(128/5);
		send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [青ナイフ弾] */
		send1_obj->BULLET_REGIST_n_way				= (8);								/* [8way] */
		bullet_regist_basic();

		if (0==((src->boss_base_danmaku_time_out)&0x1f))/* reset (1/32) */
		{
			aaa_angle512=(src->tmp_angleCCW512)-(int)((512/2));
		}
	}
	else
//	if ((10-1)>((((u8)src->boss_base_danmaku_time_out)^0x20)&0x2f))/* x 10way */
	if ((10-1)>(((src->boss_base_danmaku_time_out)	  )&0x0f))/* x 10way */
	{
		aaa_angle512 -= (int)(128/5);
		mask512(aaa_angle512);
		send1_obj->x256 							= (src->x256);						/* 弾源x256 */
		send1_obj->y256 							= (src->y256);						/* 弾源y256 */
		send1_obj->BULLET_REGIST_speed256			= (t256(2.0));						/* 弾速 */
		send1_obj->BULLET_REGIST_angle512			= (aaa_angle512);					/* 弾源角度512 */
		send1_obj->BULLET_REGIST_div_angle512		= (int)(difficulty+1/*3*/); 		/* [密着弾]の角度 */
		send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;			/* [赤クナイ弾] */
		send1_obj->BULLET_REGIST_n_way				= (4);								/* [4way] */
		bullet_regist_basic();
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
		tmp_angle_jikinerai512(player, src);/*自機狙い角作成*/
		send1_obj->x256 							= (src->x256);						/* 弾源x256 */
		send1_obj->y256 							= (src->y256);						/* 弾源y256 */
		send1_obj->BULLET_REGIST_speed256			= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* 弾速 */
		send1_obj->BULLET_REGIST_angle512			= (src->tmp_angleCCW512);			/* 弾源角度512 */	/* 自機狙い弾 */
		send1_obj->BULLET_REGIST_div_angle512		= (int)(512/24);					/* 角度 */
		send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;			/* [赤クナイ弾] */
		send1_obj->BULLET_REGIST_n_way				= (24); 							/* [24way] */
		bullet_regist_basic();
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
		tmp_angle_jikinerai512(player, src);/*自機ねらい角作成*/
		send1_obj->x256 							= (src->x256);						/* 弾源x256 */
		send1_obj->y256 							= (src->y256);						/* 弾源y256 */
		send1_obj->BULLET_REGIST_speed256			= (t256(2.0));						/* 弾速 */
		send1_obj->BULLET_REGIST_angle512			= (src->tmp_angleCCW512);			/* 弾源角度512 */
		send1_obj->BULLET_REGIST_div_angle512		= (int)(512/(7*4)); 				/* 角度([90/360]度を7分割) */
		send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [青ナイフ弾] */
		send1_obj->BULLET_REGIST_n_way				= (11); 							/* [11way] */
		bullet_regist_basic();
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
		send1_obj->y256 = (src->y256);		/* 弾源y256 */	/* 本物は発弾位置の offset 用 */
//		send1_obj->tmp_angleCCW512 = (0);/* 下向き */
	//
		int i;
		for (i=0; i<(13*12)/*144*/; i+= 13 )/*12方向*/		/* 角度(128[(90+a)/360度]を 10 分割) */
		{
			send1_obj->x256 = src->x256+t256(4.0);/* 本物は発弾位置の x offset あり */
			send1_obj->BULLET_REGIST_speed256			= ((160-i)<<(1+1+1));				/* 弾速 */
			send1_obj->BULLET_REGIST_angle512			= (i/*(0+i)&(512-1)*/);
	//		send1_obj->BULLET_REGIST_div_angle512		= (0);								/* ダミー角度(未使用) */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_06_YUKARI; 		/* [ピンクナイフ弾] */
			send1_obj->BULLET_REGIST_n_way				= (1);								/* [1way] */
			bullet_regist_basic();
//
			send1_obj->x256 = src->x256-t256(4.0);	/* 本物は発弾位置の x offset あり */
			send1_obj->BULLET_REGIST_speed256			= ((160-i)<<(1+1+1));				/* 弾速 */
			send1_obj->BULLET_REGIST_angle512			= ((0-i)&(512-1));
	//		send1_obj->BULLET_REGIST_div_angle512		= (0);								/* ダミー角度(未使用) */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_06_YUKARI; 		/* [ピンクナイフ弾] */
			send1_obj->BULLET_REGIST_n_way				= (1);								/* [1way] */
			bullet_regist_basic();
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
			send1_obj->x256 = (src->x256);		/* 弾源x256 */
			send1_obj->y256 = (src->y256);		/* 弾源y256 */
	//
		int i;
		for (i=0; i<(512); i+= 16 )/*32方向*/	/* 角度(512[360/360度]を 32 分割) */
		{
			send1_obj->BULLET_REGIST_speed256			= (t256(2.0)-((i&16)<<4));	/*奇数は高速弾*/
			send1_obj->BULLET_REGIST_angle512			= (i/*(0+i)&(512-1)*/); 	/* 弾源角度512 */
	//		send1_obj->BULLET_REGIST_div_angle512		= (0);						/* ダミー角度(未使用) */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;		/* [青ナイフ弾] */
			send1_obj->BULLET_REGIST_n_way				= (1);						/* [1way] */
			bullet_regist_basic();
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
		send1_obj->BULLET_REGIST_speed256			= (t256(2.0));				/* 弾速 */
		send1_obj->BULLET_REGIST_angle512			= (0);						/* 発射中心角度 / 特殊機能(自機狙い/他) */
		send1_obj->BULLET_REGIST_div_angle512		= (int)(512/(32));			/* 角度(512[360/360度]を 32 分割) */	/* 1周をn分割した角度 */
		send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_CAP16_05_SIROI;	/* [青白米弾] */	/* 弾グラ */
		send1_obj->BULLET_REGIST_n_way				= (32); 					/* [32way] */		/* 発弾数 */
		bullet_regist_basic();
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
			send1_obj->x256 = (src->x256);		/* 弾源x256 */
			send1_obj->y256 = (src->y256);		/* 弾源y256 */
	//
		int ii; 	/* 1周を65536分割した角度 */
		int j;		/* カウンタ */
		j=0;
		for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)	6.4==(512/80) 80分割 [5分割] */ 	/* 102.4==(512/5) 角度(512[72/360度]を 5 分割) */
		{
			j++;
			j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
			send1_obj->BULLET_REGIST_speed256			= (t256(3.0)-((j)<<5)-((j&1)<<7));	/*奇数は高速弾*/
			send1_obj->BULLET_REGIST_angle512			= (((0-ii)>>7)&(512-1));				/* 角度 */
	//		send1_obj->BULLET_REGIST_div_angle512		= (0);								/* ダミー角度(未使用) */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;				/* [青ナイフ弾] */
			send1_obj->BULLET_REGIST_n_way				= (1);								/* [1way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	メイド秘技「操りドール」
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	アリス(仮)	洗濯機弾幕
	-------------------------------------------------------
	テキトー
---------------------------------------------------------*/
static void danmaku_create_08_sentakki(SPRITE *src)
{

	if (0==((src->boss_base_danmaku_time_out)&0x03))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		src->tmp_angleCCW512 -= (int)(512/24);
		mask512(src->tmp_angleCCW512);
	//
			send1_obj->x256 = (src->x256);		/* 弾源x256 */
			send1_obj->y256 = (src->y256);		/* 弾源y256 */
	//
		{
			send1_obj->BULLET_REGIST_speed256			= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* 弾速 */
			send1_obj->BULLET_REGIST_angle512			= (src->tmp_angleCCW512);								/* 角度 */
			send1_obj->BULLET_REGIST_div_angle512		= (int)(512/24);						/* 分割角度 */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_00_AOI;				/* [青鱗弾] */
			send1_obj->BULLET_REGIST_n_way				= (8);									/* [8way] */
			bullet_regist_basic();
		//
			send1_obj->BULLET_REGIST_speed256			= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* 弾速 */
			send1_obj->BULLET_REGIST_angle512			= ((-src->tmp_angleCCW512)&(512-1));					/* 角度 */
			send1_obj->BULLET_REGIST_div_angle512		= (int)(512/24);						/* 分割角度 */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_04_MIZUIRO;			/* [水鱗弾] */
			send1_obj->BULLET_REGIST_n_way				= (8);									/* [8way] */
			bullet_regist_basic();
		}
	}
}


/*---------------------------------------------------------
	魅魔、花てすと弾幕
---------------------------------------------------------*/

static void enemy_boss01_nway_fire(SPRITE *src)
{
	static int adj_hari;	/* 開始地点 */
//	adj_hari = angle_jikinerai512(player, src);
	tmp_angle_jikinerai512(player, src);/*自機狙い角作成*/
	adj_hari = (src->tmp_angleCCW512);
//
	int speed256;
	speed256 = (0x200)+(ra_nd()&0xff);
//
	const int src__x256 = (src->x256)+((0<src->vx256)?(t256(16)):(0));/* 右移動中は右側から撃つ */
//
	int ii;
	for (ii=(adj_hari/*0*/); ii<(adj_hari+512-10); ii+=(512/5) )
	{
		int jj;
		int kk;
		for (jj=(0),kk=(0); jj<(256); jj+=(16),kk+=((512/16)/5) )
		{
			send1_obj->x256 = (src__x256);/* 右移動中は右側から撃つ */
			send1_obj->y256 = (src->y256);		/* 弾源y256 */
			send1_obj->BULLET_REGIST_speed256			= (int)(speed256 + sin512(jj)); 	/* 弾速 */
			send1_obj->BULLET_REGIST_angle512			= ((ii)+(kk));						/* 角度 */
		//	send1_obj->BULLET_REGIST_div_angle512		= (0);								/* ダミー角度(未使用) */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_HARI32_00_AOI; 			/* [水色針弾] */	/*hari_no_iro*/
			send1_obj->BULLET_REGIST_n_way				= (1);								/* [1way] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	魅魔、花てすと弾幕
	-------------------------------------------------------
	封魔禄では4方向バラマキっぽいのを撃ってくるが、
	アレンジして5方向。花映塚っぽい奴に。
	-------------------------------------------------------
	テキトー
-dd 1 0000 0x10 == (xxx)&0x1f
-11 1 0000 luna
-10 1 0000 luna hard
-01 1 0000 luna hard norm
-00 1 0000 luna hard norm easy
---------------------------------------------------------*/
static void danmaku_create_12_hana_test(SPRITE *src)
{
//	if (0x40==((src->boss_base_danmaku_time_out)&0xcf))/* 4回 */
//	if (0x10==((src->boss_base_danmaku_time_out)&0x1f))/* (16回に1回)(128なら計8回) */
	if (0x10==((src->boss_base_danmaku_time_out)&0x1f))/* (16回に1回)(128なら計4回) */
	{
		if (difficulty >= ((src->boss_base_danmaku_time_out)>>5))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		//	bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			bullet_play_04_auto(VOICE11_BOSS_KIRARIN);
		//	voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);/*テキトー*/
			#endif
		//
			enemy_boss01_nway_fire(src);/*enemy_boss04_fire(2);*/
		}
	}
//	else
//	if (0x00==((src->boss_base_danmaku_time_out)&0x1f))/* (16回に1回)(128なら計4回) */
	{
		/* ここで下方うねり4方向、重力弾っぽい奴(予定) */
	}
}


/*---------------------------------------------------------
	アリス(仮)	アリス人形弾幕
	-------------------------------------------------------
	テキトー
---------------------------------------------------------*/
extern void add_zako_alice_doll(SPRITE *src);/* アリス人形弾幕 */
static void danmaku_create_0b_alice_doll(SPRITE *src)
{

	if (50==((src->boss_base_danmaku_time_out) ))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_alice_doll(src);
	}
}

/*---------------------------------------------------------
	紅2面ボス チルノ	通常攻撃1(1/3)	自機狙い拡散弾(1-2-3-4-5-6-7-8弾)
	-------------------------------------------------------
	(1-2-3-4-5-6-7弾)かと思ってたけど、原作やり直したら、
	(1-2-3-4-5-6-7-8弾)だったので修正。
うーん
	原作やり直してよく見たら、
	(1-2-3-4-5-6-7弾)か 			(easyとか)
	(2-3-4-5-6-7-8弾)なんですね。	(lunaとか)
---------------------------------------------------------*/
static void danmaku_create_09_cirno_misogi(SPRITE *src)
{

	if (128==((src->boss_base_danmaku_time_out)))
	{
		tmp_angle_jikinerai512(player, src);/*自機ねらい角作成*/
	}
	else
	if (0x40==((src->boss_base_danmaku_time_out)&0xc7))/* 8回(修正1-2-3-4-5-6-7弾:0wayは撃たないので撃つのは7回) */
	{
		#if (1)
	//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif
	//
		{
			send1_obj->x256 = (src->x256);		/* 弾源x256 */
			send1_obj->y256 = (src->y256);		/* 弾源y256 */
		//
			send1_obj->BULLET_REGIST_speed256			= (t256(2.5));				/* 弾速 */
			send1_obj->BULLET_REGIST_angle512			= (src->tmp_angleCCW512);
			send1_obj->BULLET_REGIST_div_angle512		= (int)(512/(64));		/* 角度(512[360/360度]を 64 分割) */	/* 1周をn分割した角度 */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KOME_01_AOI;	/* [青米弾] になってるのは現在 氷弾 のグラが無いから */
			send1_obj->BULLET_REGIST_n_way				= ((/*16*/7-((src->boss_base_danmaku_time_out)>>3))&7)+(1);/* [nway] */
			bullet_regist_basic();
		}
	}
}

/*---------------------------------------------------------
	紅2面ボス チルノ	通常攻撃1(2/3)	自機狙い全方位弾
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif

/*---------------------------------------------------------
	紅2面ボス チルノ	通常攻撃1(3/3)	自機狙い収束弾
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
#endif


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
	時計回り、反時計回りの6way螺旋弾(黄色)		1回毎に(512/(6*8))[512/512度]回転
	48方位全方位弾(赤色)						8回毎に発弾
	-------------------------------------------------------
	芳華絢爛
	解像度とか速度とかの要因で、そのままでは pspで難しい気もする。
	Lunatic はこのままで、Hard 以下は細工して易しくする。
---------------------------------------------------------*/
#if 1
static void danmaku_create_11_houka_kenran(SPRITE *src)
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
	const static s16 houga_tbl[(H5*4)] =
	{/* easy				normal				hard				lunatic 			*/
		(4),				(8),				(5),				(6),					/* 註:3より4の方が簡単 */
		(short)(512/(4)),	(short)(512/(8)),	(short)(512/(5)),	(short)(512/(6)),		/* 註:3より4の方が簡単 */
		(short)(512/(4*8)), (short)(512/(8*8)), (short)(512/(5*8)), (short)(512/(6*8)), 	/* 註:3より4の方が簡単 */
//
		(4*8),				(4*8),				(5*8),				(6*8),					/* 註:3より4の方が簡単 */
		(short)(512/(4*8)), (short)(512/(4*8)), (short)(512/(5*8)), (short)(512/(6*8)), 	/* 註:3より4の方が簡単 */
	};
	static int aaa_angle512;

	//
			send1_obj->x256 = (src->x256);		/* 弾源x256 */		/* 弾源x256 */
			send1_obj->y256 = (src->y256);		/* 弾源y256 */		/* 弾源y256 */
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
			int aaa_angle512;
			tmp_angle_jikinerai512(player, src);/*自機狙い角作成*/
			aaa_angle512=(src->tmp_angleCCW512);
			send1_obj->BULLET_REGIST_speed256			= (t256(3.5));										/* 弾速 */	/*2.5 2.0*/
			send1_obj->BULLET_REGIST_angle512			= (aaa_angle512);									/* 自機狙い弾 */
			send1_obj->BULLET_REGIST_div_angle512		= houga_tbl[HOUGA_12_RED_DIV_ANGLE+difficulty]; 	/*(int)(512/(48))*/ 				/* 角度(512[360/360度]を 48 分割) */	/* 1周をn分割した角度 */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KOME_02_AKA;								/* [赤色米弾] */
			send1_obj->BULLET_REGIST_n_way				= houga_tbl[HOUGA_08_RED_NUMS+difficulty];			/*(48)*/								/* [48way] */	/* 発弾数 */
			bullet_regist_basic();
		}
	}
//	if ((0x02)==((src->boss_base_danmaku_time_out)&0x03))/* (2回に1回)(8回毎に発弾) */
	if ((0x08)==((src->boss_base_danmaku_time_out)&0x0f))/* (2回に1回)(8回毎に発弾) */
	{	// 順回り(下CCWだから、左回りCCW)
		send1_obj->BULLET_REGIST_speed256			= (t256(3.5));										/* 弾速 */		/*2.5 2.0*/
		send1_obj->BULLET_REGIST_angle512			= ((/*0+*/(aaa_angle512))&(512-1)); 				/* 発射中心角度 / 特殊機能(自機狙い/他) */
		send1_obj->BULLET_REGIST_div_angle512		= houga_tbl[HOUGA_04_YEL_DIV_ANGLE+difficulty]; 	/*(int)(512/(6))*/					/* 角度(512[360/360度]を 6 分割) */ 	/* 1周をn分割した角度 */
		send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KOME_05_KIIRO; 							/* [黄色米弾] */
		send1_obj->BULLET_REGIST_n_way				= houga_tbl[HOUGA_00_YEL_NUMS+difficulty];			/*(6)*/ 								/* [1way] */	/* 発弾数 */
		bullet_regist_basic();
		// 逆回り(下CCWだから、右回りCW)
		send1_obj->BULLET_REGIST_speed256			= (t256(3.5));										/* 弾速 */		/*2.5 2.0*/
		send1_obj->BULLET_REGIST_angle512			= ((512-(aaa_angle512))&(512-1));					/* 発射中心角度 / 特殊機能(自機狙い/他) */
		send1_obj->BULLET_REGIST_div_angle512		= houga_tbl[HOUGA_04_YEL_DIV_ANGLE+difficulty]; 	/*(int)(512/(6))*/					/* 角度(512[360/360度]を 6 分割) */ 	/* 1周をn分割した角度 */
		send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_KOME_05_KIIRO; 							/* [黄色米弾] */
		send1_obj->BULLET_REGIST_n_way				= houga_tbl[HOUGA_00_YEL_NUMS+difficulty];			/*(6)*/ 								/* [1way] */	/* 発弾数 */
		bullet_regist_basic();
		// 回転量
		aaa_angle512 += houga_tbl[HOUGA_16_YEL_ROTATE_ANGLE+difficulty];	/*(512/(6*8))*/ 				/* 角度(512[360/360度]を 48分割) */
	}
}
#endif

/*---------------------------------------------------------
	紅1面ボス ルーミア 夜符「ナイトバード」にちょっとだけ似た弾幕(予定)
	-------------------------------------------------------
	5弾目が自機狙いの 32分割16方向 弓状弾。
	-------------------------------------------------------
0000 0000 0x00==(time_out)&0x11 青(0x00==((time_out)&0x20))
0001 0000						青(0x00==((time_out)&0x20))
0010 0000 0x00==(time_out)&0x11 緑
0011 0000						緑
0100 0000 0x00==(time_out)&0x11 青(0x00==((time_out)&0x20))
0101 0000						青(0x00==((time_out)&0x20))
0110 0000 0x00==(time_out)&0x11 緑
0111 0000						緑
//
1000 0000 start
---------------------------------------------------------*/
static void danmaku_create_04_night_bird(SPRITE *src)
{
	static int aaa_angle512;

//	if (128==((src->boss_base_danmaku_time_out)))
	if (0x10==((src->boss_base_danmaku_time_out)&0x10))
	{
		tmp_angle_jikinerai512(player, src);/*自機ねらい角作成*/
		aaa_angle512 = src->tmp_angleCCW512;/*自機ねらい角*/
		aaa_angle512 -= (int)(512*(5)/(32));/* 5弾目が自機狙い */
		/* 角度(512[360/360度]を 32分割) */
	}
	else
	if (0x00==((src->boss_base_danmaku_time_out)&0x10))/* 16回 */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
		#endif
		send1_obj->y256 = (src->y256);		/* 弾源y256 */	/* 発弾位置の offset 用 */
		{
			int iii;
				iii = (src->boss_base_danmaku_time_out);
			int aaa;/* 弾色 */
			int bbb;/* 方向 */
			if (0x00==((src->boss_base_danmaku_time_out)&0x20))
					{	bbb=-1;aaa=1;/* 枠つき青弾 */}
			else	{	bbb= 1;aaa=0;/* 枠つき緑弾 */}
		//	send1_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+8))*bbb;/* 本物は発弾位置の x offset あり */
			send1_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+7))*bbb;/* 本物は発弾位置の x offset あり */
			send1_obj->BULLET_REGIST_speed256			= (t256(1.5));								/* 弾速 */
			send1_obj->BULLET_REGIST_angle512			= ((/*0+*/(aaa_angle512)*bbb)&(512-1)); 	/* 発射中心角度 / 特殊機能(自機狙い/他) */
	//		send1_obj->BULLET_REGIST_div_angle512		= (0);										/* ダミー角度(未使用) */
			send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_MARU12_02_MIDORI+aaa;				/* [枠つき緑弾]  [枠つき青弾] */	/*BULLET_MARU12_03_AOI*/
			send1_obj->BULLET_REGIST_n_way				= (1);										/* [1way] */
			bullet_regist_basic();
		}
		/* 次の弾の角度 */
		aaa_angle512 += (int)(512/(32));/* 角度(512[360/360度]を 32分割) */
	}
}

/*---------------------------------------------------------
	差分氏の妖怪1(天狗様?)が撃つ弾幕(予定)
	-------------------------------------------------------
	未作成
---------------------------------------------------------*/
static void danmaku_create_10_tengu_shot(SPRITE *src)
{
	static int aaa_angle512;
	static int data_wait2_start;/* 発弾間隔start */
	static int data_wait2;/* 発弾間隔 */

//	if (128==((src->boss_base_danmaku_time_out)))
	if (0x10==((src->boss_base_danmaku_time_out)&0x10))
	{
		data_wait2_start = (20-((difficulty)<<2))/*8*/ /*10*/;
		data_wait2 = 10;
		tmp_angle_jikinerai512(player, src);/*自機ねらい角作成*/
		aaa_angle512 = src->tmp_angleCCW512;/*自機ねらい角*/
		aaa_angle512 -= (int)(512*(5)/(32));/* 5弾目が自機狙い */
		/* 角度(512[360/360度]を 32分割) */
	}
	else
//	if (0x00==((src->boss_base_danmaku_time_out)&0x10))/* 16回 */
	{
		data_wait2--;
		if (1 > data_wait2)
		{
			data_wait2 = data_wait2_start;
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
			#endif
			send1_obj->y256 = (src->y256);		/* 弾源y256 */	/* 発弾位置の offset 用 */
			{
			int iii;
				iii = (src->boss_base_danmaku_time_out);
			int aaa;/* 弾色 */
			int bbb;/* 方向 */
				if (0x00==((src->boss_base_danmaku_time_out)&0x20))
						{	bbb=-1;aaa=1;/* 枠つき青弾 */}
				else	{	bbb= 1;aaa=0;/* 枠つき緑弾 */}
			//	send1_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+8))*bbb;/* 本物は発弾位置の x offset あり */
				send1_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+7))*bbb;/* 本物は発弾位置の x offset あり */
				send1_obj->BULLET_REGIST_speed256			= (t256(1.5));								/* 弾速 */
				send1_obj->BULLET_REGIST_angle512			= ((/*0+*/(aaa_angle512)*bbb)&(512-1)); 	/* 弾源角度512 */
		//		send1_obj->BULLET_REGIST_div_angle512		= (0);										/* ダミー角度(未使用) */
				send1_obj->BULLET_REGIST_bullet_obj_type	= BULLET_MARU12_02_MIDORI+aaa;				/* [枠つき緑弾]  [枠つき青弾] */	/*BULLET_MARU12_03_AOI*/
				send1_obj->BULLET_REGIST_n_way				= (1);										/* [1way] */
				bullet_regist_basic();
			}
			/* 次の弾の角度 */
			aaa_angle512 += (int)(512/(32));/* 角度(512[360/360度]を 32分割) */
		}
	}
}

/*---------------------------------------------------------
	文に美鈴っぽい弾幕撃たせてみるテスト(の筈だったんだけど、
	何故か当初の方向とは違う方向に……)
	-------------------------------------------------------
---------------------------------------------------------*/
#if 1
extern void bullet_create_aya_ice(SPRITE *src);

static void danmaku_create_12_aya_merin_test(SPRITE *src)
{
	if (0x10==((src->boss_base_danmaku_time_out)&0x1f))/* (16回に1回)(128なら計8回) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		bullet_create_aya_ice(src);
	}
}
#endif

/*---------------------------------------------------------
	共通弾幕生成部
	-------------------------------------------------------

---------------------------------------------------------*/
/*static*/ void danmaku_generator(SPRITE *src)
{
	if (DANMAKU_00 != src->boss_base_danmaku_type)	/* 弾幕生成は必要？ */
	{
		src->boss_base_danmaku_time_out--;			/* 時間経過する  */ 	/*fps_factor*/
		if (0 < src->boss_base_danmaku_time_out)	/* 時間切れ？ */
		{
			/* 弾幕生成中 */
			/*const*/static void (*danmaku_create_bbb[(DANMAKU_MAX/*16+1*/)])(SPRITE *sss) =
			{
				#define danmaku_create_99_mitei danmaku_create_09_cirno_misogi
				NULL,								/* 00 */	/* 弾幕生成しない */
			//													/* [咲夜] */
				danmaku_create_01_sakuya_misogi,	/* 01 */	/* 咲夜禊弾幕 / 紅5面中ボス 咲夜 「通常攻撃」 */
				danmaku_create_02_24nerai,			/* 02 */	/* 咲夜24弾幕 (奇術「ミスディレクションもどき(1/2)」) */
				danmaku_create_03_11nife,			/* 03 */	/* 咲夜11弾幕 (奇術「ミスディレクションもどき(2/2)」) */
				danmaku_create_04_pink_hearts,		/* 04 */	/* 紅5面ボス 咲夜 「通常攻撃1(1/2)」にちょっとだけ似た弾幕(予定) */
				danmaku_create_05_32way_dual,		/* 05 */	/* 紅5面ボス 咲夜 「通常攻撃1/2(2/2)」にちょっとだけ似た弾幕(予定) */
				danmaku_create_06_luna_clock_32way, /* 06 */	/* 紅5面ボス 咲夜 幻象「ルナクロック(1/2)」にちょっとだけ似た弾幕(予定) */
				danmaku_create_07_80way_dual_five,	/* 07 */	/* 紅5面ボス 咲夜 「通常攻撃3(1/2)」にちょっとだけ似た弾幕(予定) */
			//													/* [アリス(仮)] */
				danmaku_create_04_night_bird,		/* 08 */	/* 紅1面ボス ルーミア 夜符「ナイトバード」にちょっとだけ似た弾幕(予定) */
				danmaku_create_08_sentakki, 		/* 09 */	/* 洗濯機弾幕 */
				danmaku_create_11_houka_kenran, 	/* 0a */	/* 紅美鈴 華符「芳華絢爛」にちょっとだけ似た弾幕(予定) */
				danmaku_create_0b_alice_doll,		/* 0b */	/* アリス人形弾幕 */
				danmaku_create_12_hana_test,		/* 0c */	/* 魅魔、花てすと弾幕 */
				danmaku_create_99_mitei,			/* 0d */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 0e */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 0f */	/* 未定弾幕 */
			//													/* [チルノ(仮)] */
				danmaku_create_09_cirno_misogi, 	/* 10 */	/* チルノ禊弾幕 */
				danmaku_create_10_tengu_shot,		/* 11 */	/* 差分氏の妖怪1(天狗様?)が撃つ弾幕(予定) */
				danmaku_create_12_aya_merin_test,	/* 12 */	/* 文に美鈴っぽい弾幕撃たせてみるテスト */
				danmaku_create_99_mitei,			/* 13 */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 14 */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 15 */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 16 */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 17 */	/* 未定弾幕 */
			//
				danmaku_create_99_mitei,			/* 18 */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 19 */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 1a */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 1b */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 1c */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 1d */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 1e */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 1f */	/* 未定弾幕 */
			};
			(danmaku_create_bbb[(src->boss_base_danmaku_type/*-1*/)])(src);
		}
		else
		{
			src->boss_base_danmaku_type = DANMAKU_00;			/* 弾幕生成終了 */
		}
	}
}
