
#include "bullet_object.h"

/*---------------------------------------------------------
	弾幕生成
	-------------------------------------------------------
	★弾幕はシステムでサポートする構想があります。(構想中)
	システムでサポートされると、どのボスが、どの弾幕でも撃てるようになります。
---------------------------------------------------------*/

/* BOSS_BASE */

/*---------------------------------------------------------
	自機狙い弾の角度を計算
---------------------------------------------------------*/
/*static*/ void tmp_angle_jikinerai512(SPRITE *p, SPRITE *t)
{
//	BOSS99_DATA *data = (BOSS99_DATA *)t->data;
//
	#if 1
		/* Gu化完了するまで、どうせズレるので簡略版 */
	//	data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256					  ,p->x256-t->x256				  ));						/* 簡略版(弾と自分が大きさが同じならずれない、違うとその分誤差になる) */
		t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+(16*256)			  ,p->x256-t->x256+(16*256) 	  ));						/* ズル版(テキトー、ズレる) */
	#else
		/* Gu化完了したらこっち */
		t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128)), p->x256-t->x256+((p->w128-t->w128))));/**/ 			/* 偶数弾の場合に自機狙い */
	#endif
}
	#if 0
	//	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256 				  ,p->x256-t->x256				  ));//2082417				/* 簡略版(弾と自分が大きさが同じならずれない、違うとその分誤差になる) */
	//	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128	)),p->x256-t->x256-((p->w128+t->w128))));/*???(original)*/	/* 奇数弾の場合に自機狙い(?) なんか左にずれる */
/// 	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128			)),p->x256-t->x256-((p->w128	 ))));
//		t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128	)),p->x256-t->x256-((p->w128-t->w128))));/**/				/* 奇数弾の場合に自機狙い */
		t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128	)),p->x256-t->x256+((p->w128-t->w128))));/**/				/* 偶数弾の場合に自機狙い */
	//	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128			)),p->x256-t->x256-((p->w128	 ))));
	//	t->tmp_angleCCW512 =  (atan_512(p->y256-t->y256-((p->h128			)),p->x256-t->x256-((p->w128	 ))));
	#endif

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
static void danmaku_create_01_sakuya_misogi(SPRITE *src/*, int nextstate*/ /*, int anim_frame*/)
{
	static int aaa_angle512;
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
//
	if (0==((data->boss_base.danmaku_time_out)&0x07))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE13_SAKUYA_SHOT02);
		#else
	//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif

/*CW*/
//		data->boss_base.tmp_angleCCW512 += (int)(128/5)+/*2*/(5)/*(difficulty<<2)*/; // 	/*3*/2+(difficulty);
/*CCW*/
		src->tmp_angleCCW512 -= (int)(128/5)+/*2*/(5)/*(difficulty<<2)*/; //	/*3*/2+(difficulty);
		mask512(src->tmp_angleCCW512);
	//	bullet_create_aka_lines(s);
	//	bullet_create_aka_maru_jikinerai(src, t256(2.5));
	//	bullet_create_aka_maru_jikinerai(src, t256(2.0));
	//	bullet_create_aka_maru_jikinerai(src, t256(3.5));
		bullet_create_n_way_dan_sa_type(src,
			(t256(2.5)),							/* 弾速 */
			(src->tmp_angleCCW512),
			(int)(128/5),
			BULLET_KNIFE20_04_AOI,
			(8));/* [螺旋状8way青ナイフ弾] */
		if (0==((data->boss_base.danmaku_time_out)&0x1f))/* reset (1/32) */
		{
			aaa_angle512=(src->tmp_angleCCW512)-(int)((512/2));
		}
	}
	else
//	if ((10-1)>((((u8)data->boss_base.danmaku_time_out)^0x20)&0x2f))/* x 10way */
	if ((10-1)>(((data->boss_base.danmaku_time_out)    )&0x0f))/* x 10way */
	{
	//	if (/*(64)*/(16)/*(64-(difficulty<<4))*/ < data->danmaku_time_out)
		{
			aaa_angle512 -= (int)(128/5);
			mask512(aaa_angle512);
			bullet_create_n_way_dan_sa_type(src,
				(t256(2.0)),					/* 弾速 */
				(aaa_angle512),
				(int)(difficulty+1/*3*/),		/* 密着弾の角度 */
				BULLET_KUNAI12_01_AKA,
				(4));/* [4way密着赤クナイ弾] */
		}
	}
}

/*---------------------------------------------------------
	紅5面中ボス 咲夜「奇術：ミスディレクション(1/2)」にちょっとだけ似た弾幕
	-------------------------------------------------------
	自機狙い、全方向24分割弾
	-------------------------------------------------------
	[注：現在bullet_create_n_way_dan_sa_type()の自機狙いはワザト狙わない奇数弾なので、動かなければ当たりませんが、
	そのうちここは偶数弾にする予定なので、動かなければ当たるようになる予定です]
---------------------------------------------------------*/
static void danmaku_create_02_24nerai(SPRITE *src)
{
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x03))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
	int aaa_angle512;
			tmp_angle_jikinerai512(player, src);/*自機ねらい角作成*/
			aaa_angle512=(src->tmp_angleCCW512);
		{
			bullet_create_n_way_dan_sa_type(src,
				(t256(2.0)+((data->boss_base.danmaku_time_out)<<2)),	/* 弾速 */
				(aaa_angle512/*+aa*/),
				(int)(512/24),		/* 角度 */
				BULLET_KUNAI12_01_AKA,
				(24));/* [24way赤クナイ弾] */
		}
	}
}

/*---------------------------------------------------------
	紅5面中ボス 咲夜 「奇術：ミスディレクション(2/2)」にちょっとだけ似た弾幕
	-------------------------------------------------------
	自機狙い、11弾(中心と、左右に5弾ずつ狙い)
	-------------------------------------------------------
	[注：現在bullet_create_n_way_dan_sa_type()の自機狙いはワザト狙わない奇数弾なので、動かなければ当たりませんが、
	そのうちここは偶数弾にする予定なので、動かなければ当たるようになる予定です]
---------------------------------------------------------*/
static void danmaku_create_03_11nife(SPRITE *src)
{
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x07))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
	int aaa_angle512;
			tmp_angle_jikinerai512(player, src);/*自機ねらい角作成*/
			aaa_angle512=(src->tmp_angleCCW512);
		{
			bullet_create_n_way_dan_sa_type(src,
				(t256(2.0)),			/* 弾速 */
				(aaa_angle512),
				(int)(512/(7*4)),		/* 角度([90/360]度を7分割) */
				BULLET_KNIFE20_04_AOI,
				(11));/* [11way青ナイフ弾] */
		}
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
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x0f))/* 本物は 8 弾(128==0x80==danmaku_time_out) */
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		dummy_obj->w128 = src->w128;
		dummy_obj->h128 = src->h128;
		dummy_obj->y256 = src->y256;	/* 本物は発弾位置の offset 用 */
//		dummy_obj->tmp_angleCCW512 = (0);/* 下向き */
	//
		int i;
		for (i=0; i<(13*12)/*144*/; i+= 13 )/*12方向*/		/* 角度(128[(90+a)/360度]を 10 分割) */
		{
			dummy_obj->x256 = src->x256+t256(4);/* 本物は発弾位置の x offset あり */
			bullet_create_n_way_dan_sa_type(dummy_obj/*src*/,
				((160-i)<<(1+1)),					/* 弾速 */
				(i/*(0+i)&(512-1)*/),
				(0),								/* ダミー角度(未使用) */
				BULLET_KNIFE20_06_YUKARI,			/* [ピンクナイフ弾] */
				(1));								/* [1way] */
			dummy_obj->x256 = src->x256-t256(4);	/* 本物は発弾位置の x offset あり */
			bullet_create_n_way_dan_sa_type(dummy_obj/*src*/,
				((160-i)<<(1+1)),					/* 弾速 */
				((0-i)&(512-1)),
				(0),								/* ダミー角度(未使用) */
				BULLET_KNIFE20_06_YUKARI,			/* [ピンクナイフ弾] */
				(1));								/* [1way] */
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
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x3f))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		int i;
		for (i=0; i<(512); i+= 16 )/*32方向*/	/* 角度(512[360/360度]を 32 分割) */
		{
			bullet_create_n_way_dan_sa_type(src,
				(t256(2.0)-((i&16)<<4)),	/*奇数は高速弾*/
				(i/*(0+i)&(512-1)*/),
				(0),						/* ダミー角度(未使用) */
				BULLET_KNIFE20_04_AOI,		/* [青ナイフ弾] */
				(1));						/* [1way] */
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
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0x40==((data->boss_base.danmaku_time_out)&0xcf))/* 4回 */
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		bullet_create_n_way_dan_sa_type(src,
			(t256(2.0)),			/* 弾速 */
			(0),					/* 発射中心角度 / 特殊機能(自機狙い/他) */
			(int)(512/(32)),		/* 角度(512[360/360度]を 32 分割) */	/* 1周をn分割した角度 */
			BULLET_CAP16_05_SIROI,	/* 弾グラ */
			(32));/* [32way青白米弾] */ 	/* 発弾数 */
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
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x3f))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		int ii; 	/* 1周を65536分割した角度 */
		int j;		/* カウンタ */
		j=0;
		for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)	6.4==(512/80) 80分割 [5分割] */ 	/* 102.4==(512/5) 角度(512[72/360度]を 5 分割) */
		{
			j++;
			j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
			bullet_create_n_way_dan_sa_type(src,
				(t256(3.0)-((j)<<5)-((j&1)<<7)),	/*奇数は高速弾*/
				(((0-ii)>>7)&(512-1)),	/* CW */
				(0),								/* ダミー角度(未使用) */
				BULLET_KNIFE20_04_AOI,
				(1));/* [1way青ナイフ弾] */
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
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (0==((data->boss_base.danmaku_time_out)&0x03))
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		src->tmp_angleCCW512 -= (int)(512/24);
		mask512(src->tmp_angleCCW512);
		{
			bullet_create_n_way_dan_sa_type(src,
				(t256(2.0)+((data->boss_base.danmaku_time_out)<<2)),	/* 弾速 */
				(src->tmp_angleCCW512),
				(int)(512/24),		/* 角度 */
				BULLET_UROKO14_00_AOI,
				(8));/* [8way青鱗弾] */
			bullet_create_n_way_dan_sa_type(src,
				(t256(2.0)+((data->boss_base.danmaku_time_out)<<2)),	/* 弾速 */
				((-src->tmp_angleCCW512)&(512-1)),
				(int)(512/24),		/* 角度 */
				BULLET_UROKO14_04_MIZUIRO,
				(8));/* [8way水鱗弾] */
		}
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
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
	if (128==((data->boss_base.danmaku_time_out)))
	{
		tmp_angle_jikinerai512(player, src);/*自機ねらい角作成*/
	}
	else
	if (0x40==((data->boss_base.danmaku_time_out)&0xc7))/* 8回(修正1-2-3-4-5-6-7弾:0wayは撃たないので撃つのは7回) */
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE13_SAKUYA_SHOT02);
		#else
	//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
		#endif
	//
		{
			bullet_create_n_way_dan_sa_type(src,
				(t256(2.5)),	/* 弾速 */
				(src->tmp_angleCCW512),
				(int)(512/(64)),		/* 角度(512[360/360度]を 64 分割) */	/* 1周をn分割した角度 */
				BULLET_KOME_01_AOI, 	/* 青米弾 になってるのは現在 氷弾 のグラが無いから */
				((/*16*/7-((data->boss_base.danmaku_time_out)>>3))&7)+1);/* [nway青米弾] */
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
	紅3面中ボス 紅美鈴	通常攻撃1(1/2)	16方位偏り全方位ばら撒き弾
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
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
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
//	if (128==((data->boss_base.danmaku_time_out)))
	if (0x10==((data->boss_base.danmaku_time_out)&0x10))
	{
		tmp_angle_jikinerai512(player, src);/*自機ねらい角作成*/
		aaa_angle512 = src->tmp_angleCCW512;/*自機ねらい角*/
		aaa_angle512 -= (int)(512*(5)/(32));/* 5弾目が自機狙い */
		/* 角度(512[360/360度]を 32分割) */
	}
	else
	if (0x00==((data->boss_base.danmaku_time_out)&0x10))/* 16回 */
	{
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
		#else
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
		#endif
		dummy_obj->w128 = src->w128;	/* 発弾位置の offset 用 */
		dummy_obj->h128 = src->h128;	/* 発弾位置の offset 用 */
		dummy_obj->y256 = src->y256;	/* 発弾位置の offset 用 */
		{
		int iii;
			iii = (data->boss_base.danmaku_time_out);
		int aaa;/* 弾色 */
		int bbb;/* 方向 */
			if (0x00==((data->boss_base.danmaku_time_out)&0x20))
					{	bbb=-1;aaa=1;/* 枠つき青弾 */}
			else	{	bbb= 1;aaa=0;/* 枠つき緑弾 */}
		//	dummy_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+8))*bbb;/* 本物は発弾位置の x offset あり */
			dummy_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+7))*bbb;/* 本物は発弾位置の x offset あり */
			bullet_create_n_way_dan_sa_type(dummy_obj/*src*/,
				(t256(1.5)),							/* 弾速 */
				((/*0+*/(aaa_angle512)*bbb)&(512-1)),
				(0),									/* ダミー角度(未使用) */
				BULLET_MARU12_02_MIDORI+aaa,			/*BULLET_MARU12_03_AOI*/
				(1));									/* [1way枠つき緑弾]  [1way枠つき青弾] */
		}
		/* 次の弾の角度 */
		aaa_angle512 += (512/(32));/* 角度(512[360/360度]を 32分割) */
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
	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
//	if (128==((data->boss_base.danmaku_time_out)))
	if (0x10==((data->boss_base.danmaku_time_out)&0x10))
	{
		data_wait2_start = (20-((difficulty)<<2))/*8*/ /*10*/;
		data_wait2 = 10;
		tmp_angle_jikinerai512(player, src);/*自機ねらい角作成*/
		aaa_angle512 = src->tmp_angleCCW512;/*自機ねらい角*/
		aaa_angle512 -= (int)(512*(5)/(32));/* 5弾目が自機狙い */
		/* 角度(512[360/360度]を 32分割) */
	}
	else
//	if (0x00==((data->boss_base.danmaku_time_out)&0x10))/* 16回 */
	{
		data_wait2--;
		if (1 > data_wait2)
		{
			data_wait2 = data_wait2_start;
			#if (0==USE_DESIGN_TRACK)
			play_voice_auto_track(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
			#else
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
			#endif
			dummy_obj->w128 = src->w128;	/* 発弾位置の offset 用 */
			dummy_obj->h128 = src->h128;	/* 発弾位置の offset 用 */
			dummy_obj->y256 = src->y256;	/* 発弾位置の offset 用 */
			{
			int iii;
				iii = (data->boss_base.danmaku_time_out);
			int aaa;/* 弾色 */
			int bbb;/* 方向 */
				if (0x00==((data->boss_base.danmaku_time_out)&0x20))
						{	bbb=-1;aaa=1;/* 枠つき青弾 */}
				else	{	bbb= 1;aaa=0;/* 枠つき緑弾 */}
			//	dummy_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+8))*bbb;/* 本物は発弾位置の x offset あり */
				dummy_obj->x256 = src->x256+((16-(iii&0x0f))<<(3+7))*bbb;/* 本物は発弾位置の x offset あり */
				bullet_create_n_way_dan_sa_type(dummy_obj/*src*/,
					(t256(1.5)),							/* 弾速 */
					((/*0+*/(aaa_angle512)*bbb)&(512-1)),
					(0),									/* ダミー角度(未使用) */
					BULLET_MARU12_02_MIDORI+aaa,			/*BULLET_MARU12_03_AOI*/
					(1));									/* [1way枠つき緑弾]  [1way枠つき青弾] */
			}
			/* 次の弾の角度 */
		aaa_angle512 += (512/(32));/* 角度(512[360/360度]を 32分割) */
		}
	}
}

/*---------------------------------------------------------
	共通弾幕生成部
	-------------------------------------------------------

---------------------------------------------------------*/
/*static*/ void danmaku_generator(SPRITE *src)
{
	BOSS99_DATA *data=(BOSS99_DATA *)src->data;
	if (DANMAKU_00 != data->boss_base.danmaku_type) 	/* 弾幕生成は必要？ */
	{
		data->boss_base.danmaku_time_out -= 1/*fps_fa ctor*/;	/* 時間経過する  */
		if (0 < data->boss_base.danmaku_time_out)				/* 時間切れ？ */
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
				danmaku_create_99_mitei,			/* 0a */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 0b */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 0c */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 0d */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 0e */	/* 未定弾幕 */
				danmaku_create_99_mitei,			/* 0f */	/* 未定弾幕 */
			//													/* [チルノ(仮)] */
				danmaku_create_09_cirno_misogi, 	/* 10 */	/* チルノ禊弾幕 */
				danmaku_create_10_tengu_shot,		/* 11 */	/* 差分氏の妖怪1(天狗様?)が撃つ弾幕(予定) */
				danmaku_create_99_mitei,			/* 12 */
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
			(danmaku_create_bbb[(data->boss_base.danmaku_type/*-1*/)])(src);
		}
		else
		{
			data->boss_base.danmaku_type = DANMAKU_00;	/* 弾幕生成終了 */
		}
	}
}
