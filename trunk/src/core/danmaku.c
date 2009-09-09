
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
	BOSS99_DATA *data = (BOSS99_DATA *)t->data;
	#if 1
		data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256					  ,p->x256-t->x256				  ));//2082417				/* 簡略版(弾と自分が大きさが同じならずれない、違うとその分誤差になる) */
	//	data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128	)),p->x256-t->x256-((p->w128+t->w128))));/*???(original)*/	/* 奇数弾の場合に自機狙い(?) なんか左にずれる */
/// 	data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128			)),p->x256-t->x256-((p->w128	 ))));
//		data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128	)),p->x256-t->x256-((p->w128-t->w128))));/**/				/* 奇数弾の場合に自機狙い */
	//	data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128-t->h128	)),p->x256-t->x256+((p->w128-t->w128))));/**/				/* 偶数弾の場合に自機狙い */
	//	data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256+((p->h128			)),p->x256-t->x256-((p->w128	 ))));
	//	data->boss_base.tmp_angleCCW512 =  (atan_512(p->y256-t->y256-((p->h128			)),p->x256-t->x256-((p->w128	 ))));
	#endif
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

static void danmaku_create_01_misogi(SPRITE *s/*, int nextstate*/ /*, int anim_frame*/)
{
	static int aaa_angle512;
	BOSS99_DATA *data = (BOSS99_DATA *)s->data;
//
	if (0==((data->boss_base.danmaku_time_out)&0x07))
	{
		play_voice_auto_track(VOICE13_SAKUYA_SHOT02);
/*CW*/
//		data->boss_base.tmp_angleCCW512 += (int)(128/5)+/*2*/(5)/*(difficulty<<2)*/; // 	/*3*/2+(difficulty);
/*CCW*/
		data->boss_base.tmp_angleCCW512 -= (int)(128/5)+/*2*/(5)/*(difficulty<<2)*/; // 	/*3*/2+(difficulty);
		mask512(data->boss_base.tmp_angleCCW512);
	//	bullet_create_aka_lines(s);
	//	bullet_create_aka_maru_jikinerai(s, t256(2.5));
	//	bullet_create_aka_maru_jikinerai(s, t256(2.0));
	//	bullet_create_aka_maru_jikinerai(s, t256(3.5));
		bullet_create_n_way_dan_sa_type(s,
			(t256(2.5)),							/* 弾速 */
			(data->boss_base.tmp_angleCCW512),
			(int)(128/5),
			BULLET_KNIFE20_04_AOI,
			(8));/* [螺旋状8way青ナイフ弾] */
		if (0==((data->boss_base.danmaku_time_out)&0x1f))/* reset (1/32) */
		{
			aaa_angle512=(data->boss_base.tmp_angleCCW512)-(int)((512/2));
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
			bullet_create_n_way_dan_sa_type(s,
				(t256(2.0)),					/* 弾速 */
				(aaa_angle512),
				(int)(difficulty+1/*3*/),		/* 密着弾の角度 */
				BULLET_KUNAI12_00_AKA,
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

//static void enemy_boss05_more_angle(SPRITE *s, int speed256, int angle4096/*, int a_angle4096, int kaiten_houkou*/)
static void danmaku_create_02_24nerai(SPRITE *s)
{
	BOSS99_DATA *data = (BOSS99_DATA *)s->data;
	if (0==((data->boss_base.danmaku_time_out)&0x03))
	{
		play_voice_auto_track(VOICE11_SAKUYA_E_SHOT00);
	//
	int aaa_angle512;
			tmp_angle_jikinerai512(player, s);/*自機ねらい角作成*/
			aaa_angle512=(data->boss_base.tmp_angleCCW512);
		{
			bullet_create_n_way_dan_sa_type(s,
				(t256(2.0)+((data->boss_base.danmaku_time_out)<<2)/*+aa*/), 	/* 弾速 */
				(aaa_angle512/*+aa*/),
				(int)(512/24),		/* 角度 */
				BULLET_KUNAI12_00_AKA,
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

//static void enemy_boss05_more_angle(SPRITE *s, int speed256, int angle4096/*, int a_angle4096, int kaiten_houkou*/)
static void danmaku_create_03_11nife(SPRITE *s)
{
	BOSS99_DATA *data = (BOSS99_DATA *)s->data;
	if (0==((data->boss_base.danmaku_time_out)&0x07))
	{
		play_voice_auto_track(VOICE11_SAKUYA_E_SHOT00);
	//
	int aaa_angle512;
			tmp_angle_jikinerai512(player, s);/*自機ねらい角作成*/
			aaa_angle512=(data->boss_base.tmp_angleCCW512);
		{
			bullet_create_n_way_dan_sa_type(s,
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
#if 1
//static void enemy_boss05_more_angle(SPRITE *s, int speed256, int angle4096/*, int a_angle4096, int kaiten_houkou*/)
static void danmaku_create_04_pink_hearts(SPRITE *s)
{
	BOSS99_DATA *data = (BOSS99_DATA *)s->data;
	if (0==((data->boss_base.danmaku_time_out)&0x0f))/* 本物は 8 弾(128==0x80==danmaku_time_out) */
	{
		play_voice_auto_track(VOICE11_SAKUYA_E_SHOT00);
	//
		int i;
		for (i=0; i<(13*12)/*144*/; i+= 13 )/*12方向*/		/* 角度(128[(90+a)/360度]を 10 分割) */
		{
			bullet_create_n_way_dan_sa_type(s,/* 本物は発弾位置の x offset あり */
				((160-i)<<(1+1)),		/* 弾速 */
				(i/*(0+i)&(512-1)*/),
				(int)(512/(7*4)),		/* ダミー角度(未使用) */
				BULLET_KNIFE20_06_YUKARI,
				(1));/* [1wayピンクナイフ弾] */
			bullet_create_n_way_dan_sa_type(s,
				((160-i)<<(1+1)),		/* 弾速 */
				((0-i)&(512-1)),
				(int)(512/(7*4)),		/* ダミー角度(未使用) */
				BULLET_KNIFE20_06_YUKARI,
				(1));/* [1wayピンクナイフ弾] */
		}
	}
}
#endif
/*---------------------------------------------------------
	紅5面ボス 咲夜 「通常攻撃1(2/2)」にちょっとだけ似た弾幕(予定)
	-------------------------------------------------------
	-------------------------------------------------------

---------------------------------------------------------*/
#if 1
//static void enemy_boss05_more_angle(SPRITE *s, int speed256, int angle4096/*, int a_angle4096, int kaiten_houkou*/)
static void danmaku_create_05_32way_dual(SPRITE *s)
{
	BOSS99_DATA *data = (BOSS99_DATA *)s->data;
	if (0==((data->boss_base.danmaku_time_out)&0x3f))
	{
		play_voice_auto_track(VOICE11_SAKUYA_E_SHOT00);
	//
		int i;
		for (i=0; i<(512); i+= 16 )/*32方向*/	/* 角度(512[360/360度]を 32 分割) */
		{
			bullet_create_n_way_dan_sa_type(s,
				(t256(2.0)-((i&16)<<4)),	/*奇数は高速弾*/
				(i/*(0+i)&(512-1)*/),
				(int)(512/(7*4)),		/* ダミー角度(未使用) */
				BULLET_KNIFE20_04_AOI,
				(1));/* [1way青ナイフ弾] */
		}
	}
}
#endif

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
	32way 固定連弾  x 4回
---------------------------------------------------------*/
#if 1
static void danmaku_create_06_luna_clock_32way(SPRITE *s)
{
	BOSS99_DATA *data = (BOSS99_DATA *)s->data;
	if (0x40==((data->boss_base.danmaku_time_out)&0xcf))/* 4回 */
	{
		play_voice_auto_track(VOICE11_SAKUYA_E_SHOT00);
	//
		bullet_create_n_way_dan_sa_type(s,
			(t256(2.0)),			/* 弾速 */
			(0),					/* 発射中心角度 / 特殊機能(自機狙い/他) */
			(int)(512/(32)),		/* 角度(512[360/360度]を 32 分割) */	/* 1周をn分割した角度 */
			BULLET_CAP16_05_SIROI,	/* 弾グラ */
			(32));/* [32way青白米弾] */ 	/* 発弾数 */
	}
}
#endif
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
#if 1
static void danmaku_create_07_80way_dual_five(SPRITE *s)
{
	BOSS99_DATA *data = (BOSS99_DATA *)s->data;
	if (0==((data->boss_base.danmaku_time_out)&0x3f))
	{
		play_voice_auto_track(VOICE11_SAKUYA_E_SHOT00);
	//
		int ii;
		int j;	/* カウンタ */
		j=0;
		for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)  6.4==(512/80) 80分割 [5分割] */	/* 102.4==(512/5) 角度(512[72/360度]を 5 分割) */
		{
			j++;
			j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
			bullet_create_n_way_dan_sa_type(s,
				(t256(3.0)-((j)<<5)-((j&1)<<7)),	/*奇数は高速弾*/
				(((0-ii)>>7)&(512-1)),	/* CW */
				(int)(512/(7*4)),		/* ダミー角度(未使用) */
				BULLET_KNIFE20_04_AOI,
				(1));/* [1way青ナイフ弾] */
		}
	}
}
#endif
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

//static void enemy_boss05_more_angle(SPRITE *s, int speed256, int angle4096/*, int a_angle4096, int kaiten_houkou*/)
static void danmaku_create_08_sentakki(SPRITE *s)
{
	BOSS99_DATA *data = (BOSS99_DATA *)s->data;
	if (0==((data->boss_base.danmaku_time_out)&0x03))
	{
		play_voice_auto_track(VOICE11_SAKUYA_E_SHOT00);
	//
		data->boss_base.tmp_angleCCW512 -= (int)(512/24);
		mask512(data->boss_base.tmp_angleCCW512);
		{
			bullet_create_n_way_dan_sa_type(s,
				(t256(2.0)+((data->boss_base.danmaku_time_out)<<2)/*+aa*/), 	/* 弾速 */
				(data->boss_base.tmp_angleCCW512/*+aa*/),
				(int)(512/24),		/* 角度 */
				BULLET_UROKO14_00_AOI,
				(8));/* [8way青鱗弾] */
			bullet_create_n_way_dan_sa_type(s,
				(t256(2.0)+((data->boss_base.danmaku_time_out)<<2)/*+aa*/), 	/* 弾速 */
				((-data->boss_base.tmp_angleCCW512/*+aa*/)&(512-1)),
				(int)(512/24),		/* 角度 */
				BULLET_UROKO14_04_MIZUIRO,
				(8));/* [8way水鱗弾] */
		}
	}
}
/*---------------------------------------------------------
	共通弾幕生成部
	-------------------------------------------------------
---------------------------------------------------------*/
/*static*/ void danmaku_generator(SPRITE *s)
{
	BOSS99_DATA *data=(BOSS99_DATA *)s->data;
	if (DANMAKU_00 != data->boss_base.danmaku_type) 	/* 弾幕生成は必要？ */
	{
		data->boss_base.danmaku_time_out -= 1/*fps_fa ctor*/;	/* 時間経過する  */
		if (0 < data->boss_base.danmaku_time_out)				/* 時間切れ？ */
		{;}
		else
		{
			data->boss_base.danmaku_type = DANMAKU_00;	/* 弾幕生成終了 */
		}
		/* 弾幕生成中 */
		switch (data->boss_base.danmaku_type)
		{
	//	case DANMAKU_00:									break;	/* 弾幕生成しない */
//	咲夜
		case DANMAKU_01:	danmaku_create_01_misogi(s);			break;	/* 禊弾幕 / 紅5面中ボス 咲夜 「通常攻撃」 */
		case DANMAKU_02:	danmaku_create_02_24nerai(s);			break;	/* 24弾幕 (奇術「ミスディレクションもどき(1/2)」) */
		case DANMAKU_03:	danmaku_create_03_11nife(s);			break;	/* 11弾幕 (奇術「ミスディレクションもどき(2/2)」) */
		case DANMAKU_04:	danmaku_create_04_pink_hearts(s);		break;	/* 紅5面ボス 咲夜 「通常攻撃1(1/2)」にちょっとだけ似た弾幕(予定) */
		case DANMAKU_05:	danmaku_create_05_32way_dual(s);		break;	/* 紅5面ボス 咲夜 「通常攻撃1/2(2/2)」にちょっとだけ似た弾幕(予定) */
		case DANMAKU_06:	danmaku_create_06_luna_clock_32way(s);	break;	/* 紅5面ボス 咲夜 幻象「ルナクロック(1/2)」にちょっとだけ似た弾幕(予定) */
		case DANMAKU_07:	danmaku_create_07_80way_dual_five(s);	break;	/* 紅5面ボス 咲夜 「通常攻撃3(1/2)」にちょっとだけ似た弾幕(予定) */
//	アリス(仮)
		case DANMAKU_08:	danmaku_create_08_sentakki(s);			break;	/* 洗濯機弾幕 */
		case DANMAKU_09:	danmaku_create_04_pink_hearts(s);		break;	/* 咲夜 テスト */
		case DANMAKU_10:	danmaku_create_08_sentakki(s);			break;	/* 洗濯機弾幕 */
		case DANMAKU_11:	danmaku_create_05_32way_dual(s);		break;	/* 咲夜 テスト */
		case DANMAKU_12:	danmaku_create_08_sentakki(s);			break;	/* 洗濯機弾幕 */
		case DANMAKU_13:	danmaku_create_06_luna_clock_32way(s);	break;	/* 咲夜 テスト */
		case DANMAKU_14:	danmaku_create_08_sentakki(s);			break;	/* 洗濯機弾幕 */
		case DANMAKU_15:	danmaku_create_07_80way_dual_five(s);	break;	/* 咲夜 テスト */
		case DANMAKU_16:	danmaku_create_08_sentakki(s);			break;	/* 洗濯機弾幕 */
		}
	}
}

