
#include "bullet_object.h"

/*---------------------------------------------------------
	十六夜 咲夜
	PAD
	-------------------------------------------------------
	ToDo:
	移動と弾幕を分離しよう。でないと訳わからん。
	ボスタイマー対応中
	-------------------------------------------------------
	★ボスタイマーはシステムでサポート予定です。(作業中)
	★弾幕はシステムでサポートする構想があります。(構想中)
	システムでサポートされると、どのボスが、どの弾幕でも撃てるようになります。
---------------------------------------------------------*/

typedef struct /*_boss05_data*/
{
	BOSS_BASE boss_base;
//------------ 移動関連
	int state1; 							/* 行動 */
	int move_type;		/*	移動タイプ */	/* 形態 */
//
	int wait1;
	int wait2_256;
	int wait3;
//	int level;
	int move_angle512;
	int length_s_p256;	/* 咲夜とプレイヤーとの距離 */

} BOSS05_DATA;
//	int aaa_type;		/* 0: normal, 1: destroyed */
#define move_vx256 move_angle512


#if 1
/* 構造体の外にある必然性がある */
static int mahoujin_live;
	#define OFF_00_MAHOUJIN (0)
	#define ON_01_MAHOUJIN	(1)
static int mahoujin_num;
static SPRITE *dummy;	/*dummy_obj*/
#endif

enum
{
//--- 星型以前
	MH00 = 0,
	MH01,
	MH02,
	MH03,
//--- 星型準備
	MH04,
//--- 星型以降
	MH05,
	MH06,
	MH07,
	MH08,
	MH09,
	MH10,
	MH11,
	MH12,
	MH13,
	MH14,
	MH15,
	MH16,
	MH17,
	MH18,
	MH19,
	MH20,
//
	MH21,
};

enum
{
	SA00 = 0,
	SA01,
	SA02,
	SA03,
	SA04,
	SA05,
	SA06,
	SA06aaa,
	SA07,
	SA08,
	SA09,
	SA10,
	SA11,
	SA12,
};
enum
{
	SB00 = 0,
//	SB00aaa,
	SB01,
//	SB02,
//	SB02aaa,
	SB03,
	SB03aaa,
	SB03bbb,
	SB04,
	SB05,
	SB06,
	SB07,
	SB08,
	SB09,
	SB10,
	SB11,
	SB12,
};
enum
{
	SC00 = 0,
	SC01,
	SC02,
	SC03,
	SC04,
	SC05,
	SC06,
	SC07,
	SC08,
};
enum
{
	SD00 = 0,
	SD01,
	SD02,
	SD03,
	SD04,
	SD05,
};
enum
{
	SF00 = 0,
	SF01,
	SF02,
	SF03,
	SF04,
};

enum
{
/*0*/	SAKUYA_01_KEITAI = 0,	/* 第一形態: 左右に動いて小弾撃ち */
/*1*/	SAKUYA_02_KEITAI,		/* 第二形態: 全方位、豆まき */
/*2*/	SAKUYA_03_KEITAI,		/* 第三形態: 垂直ナイフが落ちてくるよ */
/*3*/	SAKUYA_04_KEITAI_A, 	/* 第四形態: (黄色マスカット弾) */
		SAKUYA_04_KEITAI_B, 	/* 第五形態: (黄色マスカット弾) */
//		SAKUYA_06_KEITAI,		/* 第六形態: (分散魔方陣)追加計画中 */
//		SAKUYA_07_KEITAI,		/* 第七形態: (時止めナイフ)追加計画中 */
//	最終形態
/*4*/	SAKUYA_08_KEITAI,		/* 第八形態: 最終形態(その1) */
/*5*/	SAKUYA_09_KEITAI,		/* 第九形態: 最終形態(その2) */
};

enum
{
	SAKUYA_ANIME_00_LEFT = 0,
	SAKUYA_ANIME_01_,
	SAKUYA_ANIME_02_,
	SAKUYA_ANIME_03_,
	SAKUYA_ANIME_04_CENTER_A,
//
	SAKUYA_ANIME_05_CENTER_B,
	SAKUYA_ANIME_06_,
	SAKUYA_ANIME_07_,
	SAKUYA_ANIME_08_,
	SAKUYA_ANIME_09_RIGHT,
//
	SAKUYA_ANIME_09_ = 0x10,
	SAKUYA_ANIME_10_,/*0x11*/
	SAKUYA_ANIME_11_,/*0x12*/
	SAKUYA_ANIME_12_,/*0x13*/
	SAKUYA_ANIME_13_,/*0x14*/
	SAKUYA_ANIME_14_,/*0x15*/
//
	SAKUYA_ANIME_15_HATUDAN,/*0x16*/
	SAKUYA_ANIME_16_,/*0x17*/
	SAKUYA_ANIME_17_,/*0x18*/
	SAKUYA_ANIME_18_,/*0x19*/
};

/*---------------------------------------------------------

	-------------------------------------------------------
	s = ボス本体 boss sprite
	t = プレイヤーの弾 player's weapon
---------------------------------------------------------*/

static void callback_enemy_boss05_hitbyweapon_dummy(SPRITE *s, SPRITE *t/*, int angle*/)
{	/* 完全に姿を現すまで攻撃は、すべて無効とする。 */
}

static void callback_enemy_boss05_hitbyweapon(SPRITE *s, SPRITE *t/*, int angle*/)
{
	explosion_add_type(t->x256,t->y256,/*0,*/EXPLOSION_MINI00);/*先に実行した方が速い*/
//
	BOSS05_DATA *data	= (BOSS05_DATA *)s->data;
	WEAPON_BASE *w		= (WEAPON_BASE *)t->data;
//
	{
		data->boss_base.boss_health -= w->strength;/* ボス攻撃して体力減らす */
		if (0 > data->boss_base.boss_health)
		{
			data->boss_base.boss_health = 0;
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static void sakuya_put_items(SPRITE *s/*, SPRITE *t*/)
{
	play_voice_auto_track(VOICE07_BOMB);
//++	((PLAYER_DATA *)player->data)->bo ssmode=B04_CHANGE;
	item_from_bullets(SP_ITEM_05_HOSI);
	item_create_for_boss(s, ITEM_CREATE_MODE_02);
	//t->anim_frame 	= SAKUYA_ANIME_04_CENTER_A;//バグ(ここでプレイヤーショットのアニメパターンを弄るので、レミボムでハングアップする)
	s->anim_frame		= SAKUYA_ANIME_04_CENTER_A;//咲夜のアニメパターンを弄る
	BOSS05_DATA *data = (BOSS05_DATA *)s->data;
	data->move_type++;
	data->state1		= 0;/* 共通して 値を 0 にする */
	data->wait1 		= 80;
	data->wait2_256 	= t256(-100);
	data->wait3 		= 0;//???
	data->boss_base.boss_health 	= (1024-1); 	/* life_point消費しゲージ補充 */
	{
		u16 aaa_timer[8] =
		{
			(1000*64),	/* ダミー */
			(1000*64),	/* 最終形態 */
			(40*64),
			(40*64),
			(40*64),
			(40*64),
			(40*64),
			(40*64),
		};
		data->boss_base.boss_timer		= aaa_timer[(data->boss_base.boss_life)];		/* 40*64==40[count] 	約40[秒(64/60)](単位は秒ではない) */
	}	/* ボスタイマー初期化 */
	// 40 60 [秒] ???	4096/*2500*/	/*3000*/;//???
}


/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/extern void enemy_sp2_bullet_create(SPRITE *s, int angle512, int gra256, int *sd_nnn);
/*static*/extern void enemy_sp1_bullet_create(SPRITE *s, int speed256, int angle512, int gra256, int r_or_l, int *sd_angle512);


/*---------------------------------------------------------

---------------------------------------------------------*/

static void sakuya_wait_state(SPRITE *s, /*int nextstate,*/ int anim_frame)
{
	BOSS05_DATA *data = (BOSS05_DATA *)s->data;
	data->wait1 -= 1/*fps_fa ctor*/;
	if (0 < data->wait1)
	{;}
	else
	{
		data->state1++/* = nextstate*/;
		s->anim_frame = anim_frame;
	}
}





/*---------------------------------------------------------
	弾幕が終わるまで待つ。
	-------------------------------------------------------

---------------------------------------------------------*/

static void danmaku_state_check_holding(SPRITE *s/*, int nextstate*/ /*, int anim_frame*/)
{
	BOSS05_DATA *data = (BOSS05_DATA *)s->data;
	if (DANMAKU_00 == data->boss_base.danmaku_type)
	{
		data->state1++/* = nextstate*/;
	}
}



/*---------------------------------------------------------
さくや
中心→左右アニメサブ

s->anim_frame = SAKUYA_ANIME_04_CENTER_A;
data->wait3 = 0;
で初期化されてる必要がある。
---------------------------------------------------------*/

static void sakuya_anime_to_sayuu(SPRITE *s)
{
	BOSS05_DATA *data=(BOSS05_DATA *)s->data;
	if (0 < data->move_vx256)
	{
		/* 右へ移動 */
		if (s->anim_frame==SAKUYA_ANIME_09_RIGHT)
		{
			data->state1++/*	= SB05*/;/*おしまい*/
		}
		else
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=3)
			{
				data->wait3=0;
				s->anim_frame++;/*???*/
			}
		}
	}
	else
	{
		/* 左へ移動 */
		if (s->anim_frame==SAKUYA_ANIME_00_LEFT)
		{
			data->state1++/*	= SB05*/;/*おしまい*/
		}
		else
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3>=3)
			{
				data->wait3=0;
				s->anim_frame--;/*???*/
			}
		}
	}
}
/*---------------------------------------------------------
さくや
弾撃ちアニメ待機

s->anim_frame = SAKUYA_ANIME_15_HATUDAN;
data->wait3 = 0;
で初期化されてる必要がある。
---------------------------------------------------------*/

static void sakuya_tama_anime_wait3_0(SPRITE *s)
{
	BOSS05_DATA *data=(BOSS05_DATA *)s->data;
	if (s->anim_frame==SAKUYA_ANIME_18_)
	{
		data->state1++/*	= SB05*/;
	}
	else
	{
		data->wait3 += 1/*fps_fa ctor*/;
		if (data->wait3>=3)
		{
			data->wait3=0;
			s->anim_frame++;/*???*/
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss05_out(SPRITE *s)
{
	if (s->x256 < t256(50)) 										{	s->x256 = t256(50);}
	if (s->x256 > t256(GAME_WIDTH)-t256(50)-((s->w128+s->w128)))	{	s->x256 = t256(GAME_WIDTH)-t256(50)-((s->w128+s->w128));}
	if (s->y256 < t256(10)) 										{	s->y256 = t256(10);}
	if (s->y256 > t256(GAME_HEIGHT)-t256(120)+((s->h128+s->h128)))	{	s->y256 = t256(GAME_HEIGHT)-t256(120)+((s->h128+s->h128));}
}



/*---------------------------------------------------------

---------------------------------------------------------*/

#if 1
/* プレイヤーの周りに散らばり、時間が経つとプレイヤー狙い弾 */
static void enemy_boss05_knifes3(SPRITE *s/*,*/ /*dou ble speed,*/	/*int length222*/)
{
	BOSS05_DATA *data=(BOSS05_DATA *)s->data;
	int len256;
	int tmp1_256;
	int tmp2_256;
	int tmp888;
	tmp888=0;
	/* ある程度遠くに出ないと認識しづらい */
	/* キャラ(コア)がでかいと避ける前に死ぬ。(ナイフが広がる前にあたる) */
	len256 = t256(/*70*/80) -((difficulty)<<8) /*-length222*/  /*/2*/;
	data->wait2_256 = t256(0);
	while (1)
	{
		tmp888 = (((int)ra_nd()+(t256_floor(data->wait2_256)) )&(8-1)/*%8*/);
		data->wait2_256 += t256(1);
		len256 += t256(0.1);
#if 0
/* CWの場合 */
		tmp1_256 = player->x256 + ((player->w128-s->w128)) + ((cos512((tmp888<<6))*len256)>>8);
		tmp2_256 = player->y256 + ((player->h128-s->h128)) + ((sin512((tmp888<<6))*len256)>>8);
#else
/* CCWの場合 */
		tmp1_256 = player->x256 + ((player->w128-s->w128)) + ((sin512((tmp888<<6))*len256)>>8);
		tmp2_256 = player->y256 + ((player->h128-s->h128)) + ((cos512((tmp888<<6))*len256)>>8);
#endif
		if ((( t256(30) < tmp1_256) && (tmp1_256 < t256(GAME_WIDTH-30)))&&
			(( t256(30) < tmp2_256) && (tmp2_256 < t256(GAME_HEIGHT-72))))
		{	break;}
	}
	int tmp_x256;
	int tmp_y256;
	tmp_x256 = player->x256 + ((player->w128));
	tmp_y256 = player->y256 + ((player->h128));
	int ii;
	for (ii=0; ii<8; ii++)
	{
		if (tmp888==ii)
		{
			data->boss_base.tmp_angleCCW512 	= (ii<<6);
			s->x256 			= tmp1_256;
			s->y256 			= tmp2_256;
			s->alpha			= 0;
//			s->anim_frame		= SAKUYA_ANIME_18_;//?????
			data->move_type 	= SAKUYA_09_KEITAI/*5*/;/*???*/
			data->wait2_256 	= /*speed256*/t256(1)+(difficulty<<8);
		}
		else
		{
		//	dou ble tmp_s_x;
		//	dou ble tmp_s_y;
		//	tmp_s_x = s->x;
		//	tmp_s_y = s->y;
			/* プレイヤー狙い弾 */
#if 0
/* CWの場合 */
			s->x256 		=	tmp_x256 + ((cos512((ii<<6))*len256)>>8);
			s->y256 		=	tmp_y256 + ((sin512((ii<<6))*len256)>>8);
#else
/* CCWの場合 */
			s->x256 		=	tmp_x256 + ((sin512((ii<<6))*len256)>>8);
			s->y256 		=	tmp_y256 + ((cos512((ii<<6))*len256)>>8);
#endif
			bullet_create_sakuya_follow_knife1/*2*/(s,
				/*speed256*/t256(2.0),
				(ii<<6)/*i*M_PI*2*1/8*/,							/*angle512*/
				-30+(ii&(2-1)/*%2*/)*60);							/* -30 or 30 */
		//	s->x = tmp_s_x;
		//	s->y = tmp_s_y;
		}
	}
}
#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

typedef struct /*_boss05_maho_data*/
{
	SPRITE *sakuya_obj; 		/* 継承 */
	int state_m01;	/* 行動 */
	int state_m02;	/* 形態 */
	int nnn;
	int mmm;
	int wait;
	int angle512;
//
	int sss;
	int jjj;
	int maho_vx;
	int maho_vy;
	int maho_point_x;			/* 目標座標 */
	int maho_point_y;			/* 目標座標 */
	int maho_point_angle512;
//
//	int tx256;
//	int ty256;
} BOSS05_MAHO_DATA;

static void hosi_wait(BOSS05_MAHO_DATA *d)
{
//	if (d->wait<0)	{	d->state_m02++/*	= MH09*/;}	else	{	d->wait--;}
	d->wait--;	if (d->wait<0)	{	d->state_m02++/*	= MH09*/;}
}



static void enemy_boss05_maho_move(SPRITE *s)
{
	/*
	星型線1 190, 48
	星型線2 248, 217
	星型線3 100, 113
	星型線4 280, 113
	星型線5 138, 217
	*/
/*
#define pX 0
#define pY 1
	u16 ppp[5][2] =
	{
		{t256(190), t256( 48)},
		{t256(248), t256(217)},
		{t256(100), t256(113)},
		{t256(280), t256(113)},
		{t256(138), t256(217)},
	};
*/

	BOSS05_MAHO_DATA *d = (BOSS05_MAHO_DATA *)s->data;
	if (d->state_m02 < MH05)
	{
		s->x256 = d->sakuya_obj->x256+((d->sakuya_obj->w128-s->w128));
		s->y256 = d->sakuya_obj->y256+((d->sakuya_obj->h128-s->h128));
		if (((BOSS05_DATA *)d->sakuya_obj->data)->move_type==SAKUYA_08_KEITAI/*4*/ /*星型を描いてもいいよ*/)
		{
		//	d->state_m02 = MH05;/*星型を描くぜ*/
			d->state_m02 = MH04;/*星型を描くぜ*/
		}
	}
	switch (d->state_m02)
	{
//------------------------------------------
	case MH00:
		s->alpha += 3;
		if (150 < s->alpha)
		{
			s->alpha		= 150;
			d->state_m02++/*	= MH01*/;
			d->wait 		= 50;
		}
		break;

	case MH01:
		d->wait -= 1/*fps_fa ctor*/;
		if (d->wait < 1)
		{
			d->state_m02++/*	= MH02*/;
			d->state_m01	= 0x100;
		}
		break;
	case MH02:
		if (0==(d->state_m01&0x0f/*%10*/))
		{
#if 0
/* CWの場合 */
			bullet_create_tomari2_dan(s, (d->state_m01/*>>4*/<<3/*1/10*/)/**256*/, d->angle512, 					  t256(0.02), d->angle512+(d->state_m01)-deg_360_to_512((180  )));	//ra d2deg512((d->state_m01)>>7/*/100*/)
			bullet_create_tomari2_dan(s, (d->state_m01/*>>4*/<<3/*1/10*/)/**256*/, d->angle512+deg_360_to_512((120)), t256(0.02), d->angle512+(d->state_m01)-deg_360_to_512((180+120)));	//ra d2deg512((d->state_m01)>>7/*/100*/)
			bullet_create_tomari2_dan(s, (d->state_m01/*>>4*/<<3/*1/10*/)/**256*/, d->angle512-deg_360_to_512((120)), t256(0.02), d->angle512+(d->state_m01)-deg_360_to_512((180-120)));	//ra d2deg512((d->state_m01)>>7/*/100*/)
#else
/* CCWの場合 */
			bullet_create_tomari2_dan(s, (d->state_m01/*>>4*/<<3/*1/10*/)/**256*/, d->angle512, 						 t256(0.02), d->angle512+(d->state_m01)+deg_360_to_512CCW((180	)));	//ra d2deg512((d->state_m01)>>7/*/100*/)
			bullet_create_tomari2_dan(s, (d->state_m01/*>>4*/<<3/*1/10*/)/**256*/, d->angle512-deg_360_to_512CCW((120)), t256(0.02), d->angle512+(d->state_m01)+deg_360_to_512CCW((180+120)));	//ra d2deg512((d->state_m01)>>7/*/100*/)
			bullet_create_tomari2_dan(s, (d->state_m01/*>>4*/<<3/*1/10*/)/**256*/, d->angle512+deg_360_to_512CCW((120)), t256(0.02), d->angle512+(d->state_m01)+deg_360_to_512CCW((180-120)));	//ra d2deg512((d->state_m01)>>7/*/100*/)
#endif
		}
		d->state_m01--;
		if (d->state_m01 < 1)
		{
			d->state_m02++/*	= MH03*/;
			d->wait 		= 15;
			d->state_m01	= 0x10;
		}
		break;
	case MH03:
		d->wait -= 1/*fps_fa ctor*/;
		if (d->wait < 1)
		{
			d->wait=12;
			play_voice_auto_track(VOICE13_SAKUYA_SHOT02);
			{int i;
				for (i=t256(5); i<t256(9); i+=t256(1))
				{
#if 0
/* CWの場合 */
					bullet_create_offset_dan_type(s, i, d->angle512,					 t256(0), t256(0), BULLET_HARI32_00_AOI);
					bullet_create_offset_dan_type(s, i, d->angle512+deg_360_to_512(120), t256(0), t256(0), BULLET_HARI32_00_AOI);
					bullet_create_offset_dan_type(s, i, d->angle512-deg_360_to_512(120), t256(0), t256(0), BULLET_HARI32_00_AOI);
#else
/* CCWの場合 */
					bullet_create_offset_dan_type(s, i, d->angle512,						t256(0), t256(0), BULLET_HARI32_00_AOI);
					bullet_create_offset_dan_type(s, i, d->angle512-deg_360_to_512CCW(120), t256(0), t256(0), BULLET_HARI32_00_AOI);
					bullet_create_offset_dan_type(s, i, d->angle512+deg_360_to_512CCW(120), t256(0), t256(0), BULLET_HARI32_00_AOI);
#endif
				}
			}
			/* deg_360_to_512(8.59) 8.59436717317284029518323968680208	PI*2/41.8879008478638666666666666666667 ra d2deg512(0.15)*/
			/* 12.2230999796235950864828297767852 == 512/41.8879008478638666666666666666667 */
#if 0
/* CWの場合 */
			d->angle512 += 12;
#else
/* CCWの場合 */
			d->angle512 -= 12;
#endif
			d->state_m01--;
		}
		if (d->state_m01<0x0)
		{
			d->state_m02--/*	= MH02*/;
			d->state_m01	= 0x50;
		}
		break;

//------------------------------------------

	/*
	星型線1 190, 48
	星型線2 248, 217
	星型線3 100, 113
	星型線4 280, 113
	星型線5 138, 217
	*/
#define MAHO_JIN_WAIT50 	(16/*16*/)
#define MAHO_JIN_SPEED10	(4/*8*/)
#define MAHO_JIN_SET03		(1)
#define MAHO_JIN_ADD05		(1)

#define MAHO_JIN_POINT_CX00 	(t256(GAME_WIDTH/2))
#define MAHO_JIN_POINT_CY00 	(t256(/*GAME_HEIGHT/4*/64))
#define MAHO_JIN_RADIUS_SIZE	(/*t256*/(90))

//------------------------------------------

	case MH04:	/* 星型を描く準備 */
		d->state_m02++/*	= MH05*/;
		d->maho_point_angle512=0;
		d->maho_vx = t256(1.0);
		d->maho_vy = t256(0.0);
		s->x256 = (d->sakuya_obj->x256);		dummy->x256 = s->x256;
		s->y256 = (d->sakuya_obj->y256);		dummy->y256 = s->y256;
		d->sss = 0;
		break;
//------------------------------------------
	case MH05:	/* 準備完了まで待つ */
		s->anim_frame = 0;
//			if (2==d->nnn/*%(4-difficulty)*/)/* %2 == (5-3:Lunatic) */
//			{	s->anim_frame = 1;}
//			else
//			{	s->anim_frame = 0;}
		if (((BOSS05_DATA *)d->sakuya_obj->data)->state1==1)
		{
			d->state_m02++/*	= MH06*/;
			d->state_m01	= 0x0;
		}
		break;
//------------------------------------------
	case MH06:	/* 星型を描くよ */
//		d->maho_point_x=MAHO_JIN_POINT_CX00+sin512((d->maho_point_angle512))*MAHO_JIN_RADIUS_SIZE;
//		d->maho_point_y=MAHO_JIN_POINT_CY00+cos512((d->maho_point_angle512))*MAHO_JIN_RADIUS_SIZE;

		/*	*/
		dummy->x256 += (d->maho_vx)/**fps_fa ctor*/;
		dummy->y256 += (d->maho_vy)/**fps_fa ctor*/;
		d->sss++;
		if (
	//		(t256(32) > abs(d->maho_point_x-s->x256)) &&
	//		(t256(32) > abs(d->maho_point_y-s->y256))
				(16 < d->sss)
		)
		{
			d->sss = 0;
			d->maho_point_angle512 += (512*2/5);
			mask512(d->maho_point_angle512);
			{
			/* CCWの場合 */
		//	int d_angle512=atan_512(d->maho_point_y-s->y256-((s->h128)), d->maho_point_x-s->x256-((s->w128)));/*画面中心より上*/
			d->maho_vx = ((sin512((d->maho_point_angle512/*d_angle512*/))*/*t256*/(/*3*/MAHO_JIN_SPEED10/*+difficulty*/))/*>>8*/)/**fps_fa ctor*/;
			d->maho_vy = ((cos512((d->maho_point_angle512/*d_angle512*/))*/*t256*/(/*3*/MAHO_JIN_SPEED10/*+difficulty*/))/*>>8*/)/**fps_fa ctor*/;
			}

		//
			d->state_m02++/*	= MH05*/;
		//
			d->mmm++;
			if (5 < d->mmm)
			{
				d->mmm = 0;
			//	d->mmm &= 1;
				d->nnn++;	if (d->nnn>(/*4-0*/3/*difficulty*/) )	{d->nnn=0/*1*/ /*0*/;}
				s->x256 = (d->sakuya_obj->x256);		dummy->x256 = s->x256;
				s->y256 = (d->sakuya_obj->y256);		dummy->y256 = s->y256;
			//	d->tx256 = MAHO_JIN_POINT_CX00;
			//	d->ty256 = MAHO_JIN_POINT_CY00;
			//	dummy->x256 = d->tx256;
			//	dummy->y256 = d->ty256;
			}
		//
			d->wait 		= MAHO_JIN_WAIT50;
			d->state_m01	= 0x0;
		}
		else
		{
			d->state_m01+=/*5*/MAHO_JIN_ADD05/*3*/;
			if (0/*1*/==d->nnn)
			{
				if (0==(d->state_m01&/*7*/MAHO_JIN_SET03/*%5*/))
				{
					/* CWの場合 */
				//	enemy_sp2_bullet_create(s, deg_360_to_512(3)+(( ((-d->state_m01*2/**d->mmm*/)) +d->state_m01)>>3/*/5*/), t256(0.015), &(((BOSS05_MAHO_DATA *)s->data)->nnn) );
/* CCWの場合 */
					enemy_sp2_bullet_create(dummy,
						d->maho_point_angle512, 	//	-deg_360_to_512CCW(3)+(( ((-d->state_m01*2/**d->mmm*/)) +d->state_m01)>>3/*/5*/),
						t256(0.015),
						&(((BOSS05_MAHO_DATA *)s->data)->nnn)
					);
				}
			}
		}
		break;
	case MH07:
		d->jjj++;
		if ((3-difficulty) < d->jjj)
		{
			d->jjj = 0;
		//	bullet_create_360_knifes(s, t256(1.2) );
			/*---------------------------------------------------------
				360ナイフ
				魔方陣用
				全方向に青ナイフ弾を撃つ
			---------------------------------------------------------*/

			//static void bullet_create_360_knifes(SPRITE *s, int speed256)
			{
			//	int angle512;
			//	for (angle512=0; angle512<512; angle512+=32/*29*/)
			//	{
			//		bullet_create_sakuya_knife(s, (speed256), (angle512), /*i*/(angle512)/*anime_pattern*/);
			//	}
			bullet_create_n_way_dan_sa_type(s,
				(t256(1.2)/*(speed256)*/),
				(0/*aaa_angle512*/),
				(int)(512/(18)),		/* 角度([360/360]度を18分割) */
				BULLET_KNIFE20_06_YUKARI/*BULLET_KNIFE20_04_AOI*/,
				(18));/* [18wayピンクナイフ弾] */
			}
		}
		d->state_m02++/*	= MH08*/;
		break;
	case MH08:	hosi_wait(d);	break;	/* 一時停止 */
	case MH09:
		d->state_m02	= MH06;
		d->state_m01	= 0x0;
		break;
//------------------------------------------

	}
	#if 0
	/*これではうまくいかない場合がある*/
	if (d->c->type==SP_DELETE)
	#else
	if (OFF_00_MAHOUJIN==mahoujin_live)
	#endif
	{
		s->type = SP_DELETE;
	}
	#if 0/*魔方陣デバッグ用*/
	/* パネルのスコア欄にstate_m02を、グレイズ欄にstate_m01を表示させる。っていうか書き換えちゃう。 */
	((PLAYER_DATA *)player->data)->score		= d->state_m02;
	((PLAYER_DATA *)player->data)->graze_point	= d->state_m01;
	#endif
}

static void enemy_boss05_maho_create(SPRITE *sakuya)	//魔方陣グラフィック生成
{
	SPRITE *s2; 		// 魔方陣グラフィックのスプライト
	s2					= sprite_add_res(BASE_MAHOUJIN_0_PNG);	//s2->anim_speed	= 0;/*"boss04-lo.png"*/
	s2->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	s2->anim_frame		= 0;
	s2->type			= SP_MUTEKI;
	BOSS05_MAHO_DATA *data;
	data				= mmalloc(sizeof(BOSS05_MAHO_DATA));
	s2->data			= data;
	data->state_m02 	= 0;
	data->state_m01 	= 0x0;
	data->wait			= 0;
	data->angle512		= (0);
	data->sakuya_obj	= sakuya;
	data->nnn			= 0;
	data->mmm			= 0;
	s2->alpha			= 0;
	s2->callback_mover	= enemy_boss05_maho_move;
	s2->x256			= sakuya->x256+((sakuya->w128-s2->w128));
	s2->y256			= sakuya->y256+((sakuya->h128-s2->h128));
}


/*---------------------------------------------------------

---------------------------------------------------------*/

#define SAKUYA_LIMIT_MIN_Y (t256(8))
#define SAKUYA_LIMIT_MAX_Y (t256(GAME_HEIGHT-32))

/* 咲夜さんの誘導ポイント座標 */

#define SAKUYA_POINT_X_LEFT_OUT 		(t256(GAME_WIDTH*0/4))
#define SAKUYA_POINT_X_LEFT_MID 		(t256(GAME_WIDTH*1/4))
#define SAKUYA_POINT_X_MID				(t256(GAME_WIDTH*2/4))/*-s->w/2*/
#define SAKUYA_POINT_X_RIGHT_MID		(t256(GAME_WIDTH*3/4))
#define SAKUYA_POINT_X_RIGHT_OUT		(t256(GAME_WIDTH*4/4))/*-s->w  */
#define SAKUYA_POINT_X_MARGIN			(t256(16))

#define SAKUYA_POINT_Y_LEFT 	(t256(50))
#define SAKUYA_POINT_Y_MID		(t256(10))
#define SAKUYA_POINT_Y_RIGHT	(t256(50))
#define SAKUYA_POINT_Y_CENTER	(t256(50))
#define SAKUYA_POINT_Y_MARGIN	(t256(16))

#define SAKUYA_LIMIT_X_LEFT 		(SAKUYA_POINT_X_LEFT_OUT +SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_LEFT 	(SAKUYA_POINT_X_MID  +SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_RIGHT	(SAKUYA_POINT_X_MID  -SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_RIGHT		(SAKUYA_POINT_X_RIGHT_OUT-SAKUYA_POINT_X_MARGIN)

#define atan_512_SAKUYA_LEFT	atan_512( (SAKUYA_POINT_Y_LEFT) 	-s->y256, (SAKUYA_POINT_X_LEFT_OUT) -s->x256)
#define atan_512_SAKUYA_MID 	atan_512( (SAKUYA_POINT_Y_MID)		-s->y256, (SAKUYA_POINT_X_MID)	-s->x256)
#define atan_512_SAKUYA_RIGHT	atan_512( (SAKUYA_POINT_Y_RIGHT)	-s->y256, (SAKUYA_POINT_X_RIGHT_OUT)-s->x256)
#define atan_512_SAKUYA_CENTER	atan_512( (SAKUYA_POINT_Y_CENTER)	-s->y256, (SAKUYA_POINT_X_MID)	-s->x256)


/* 咲夜さんの誘導ポイント名称 */
enum
{
	SAKUYA_POINT_00_LEFT=0, 	/*左下*/
	SAKUYA_POINT_01_MID_UP, 	/*中上*/
//	SAKUYA_POINT_02_CENTER, 	/*画面中心*/
	SAKUYA_POINT_03_RIGHT,		/*右下*/
};


/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_sakuya(SPRITE *s, int speed256, int target_point_name)
{
	int b_move_angle512;
	switch (target_point_name)
	{
	case SAKUYA_POINT_00_LEFT:		b_move_angle512 = atan_512_SAKUYA_LEFT;/*50*/		break;
	case SAKUYA_POINT_01_MID_UP:	b_move_angle512 = atan_512_SAKUYA_MID;				break;
//	case SAKUYA_POINT_02_CENTER:	b_move_angle512 = atan_512_SAKUYA_CENTER;		break;
	case SAKUYA_POINT_03_RIGHT: 	b_move_angle512 = atan_512_SAKUYA_RIGHT;/**/	break;
	}
#if 0
/* CWの場合 */
	s->x256 += ((cos512((b_move_angle512))*(speed256))>>8)/**fps_fa ctor*/;
	s->y256 += ((sin512((b_move_angle512))*(speed256))>>8)/**fps_fa ctor*/;
#else
/* CCWの場合 */
	s->x256 += ((sin512((b_move_angle512))*(speed256))>>8)/**fps_fa ctor*/;
	s->y256 += ((cos512((b_move_angle512))*(speed256))>>8)/**fps_fa ctor*/;
#endif
		 if (s->y256 < (SAKUYA_LIMIT_MIN_Y) )	{	s->y256 += t256(1); 	}/* はみ出したら修正 */
	else if (s->y256 > (SAKUYA_LIMIT_MAX_Y) )	{	s->y256 -= t256(1); 	}/* はみ出したら修正 */
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss05_move(SPRITE *s)
{
static int my_yudo_point;
	BOSS05_DATA *data=(BOSS05_DATA *)s->data;
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	switch (data->move_type)
	{
	case SAKUYA_01_KEITAI/*0*/: /* 第一形態: 左右に動いて小弾撃ち */
/* 第一形態では使って無いみたいだから利用する */
//#define _mode wait2_256
		switch (data->state1)
		{
		case SA00:	/* 下がる */
			s->y256 += t256(1)/*fps_fa ctor*/;
			if (t256(16/*20*/) < s->y256)	// y が t256(20) まで来たら
			{
			/* 登場時には弾を撃たない */
			//	bullet_create_aka_lines(s);
				s->anim_frame = SAKUYA_ANIME_04_CENTER_A/*0*/;
			//	pd->bo ssmode=B05_BE FORE_LOAD;
				pd->state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
				data->state1++/*	= SA01*/;
			}
			break;
		/*完全に姿を現す*/
		case SA01:	/* */
		//	if (pd->bo ssmode==B01_BA TTLE)
		//	if ((STATE_FLAG_05_IS_BOSS|0) == (pd->state_flag&(STATE_FLAG_05_IS_BOSS|STATE_FLAG_06_IS_SCRIPT)))
			if ( ((((PLAYER_DATA *)player->data)->state_flag) & STATE_FLAG_05_IS_BOSS) )
			{
				/* プレイヤー弾受け付け、コールバックを登録 */
				((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss05_hitbyweapon;
			//	/*時間制限カウント有効化*/
			//	data->state1++/*	= SA02*/;
				data->state1	= SA12;
			}
			break;
/*----------------*/
		case SA02:	/* 左へ移動 */
			//if (pd->bo ssmode==B01_BA TTLE)
		//	{
			if (my_yudo_point < s->x256 )/*t256(50)*/
			{
				s->x256 -= t256(2.0/*3.0*/)/*fps_fa ctor*/;
				if (SAKUYA_ANIME_00_LEFT < s->anim_frame)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (SAKUYA_ANIME_04_CENTER_A < data->wait3)
					{
						s->anim_frame--;
						data->wait3 = 0;
					}
				}
			}
			else	// x が 50 以下になったら
			{
				data->state1++/*	= SA03*/;
		#if 1/*可変弾幕*/
				tmp_angle_jikinerai512(player, s);/*自機ねらい角作成*/
			#if 0
			/*CW*/
				data->boss_base.tmp_angleCCW512 	-= 128;
			#else
			/*CCW*/
				data->boss_base.tmp_angleCCW512 	+= (256-128);
			#endif
		#else/*固定弾幕*/
			/*CCW*/
				data->boss_base.tmp_angleCCW512 	= deg_360_to_512CCW((90));/* 右側から撃ち始める (128)*/
		#endif
				data->boss_base.danmaku_type		= DANMAKU_01;			/* 禊弾幕をセット */
				data->boss_base.danmaku_time_out	= DANMAKU_01_SET_TIME;	/* 禊弾幕の発生時間 */
		//		s->anim_frame			= SAKUYA_ANIME_04_CENTER_A;
				s->anim_frame			= SAKUYA_ANIME_15_HATUDAN;
			}
		//	}
			break;
		case SA03:	/* 弾撃ちアニメ待機 */
			sakuya_tama_anime_wait3_0(s);
			break;
		case SA04:	/* 禊弾幕が終わるまで待機 */
			danmaku_state_check_holding(s/*, SA04*/ /*, SAKUYA_ANIME_04_CENTER_A*/);
			break;
		case SA05:
			s->anim_frame			= SAKUYA_ANIME_04_CENTER_A;
			data->state1++/*	= SA06*/;
			break;
		case SA06:	/* 真ん中に戻る */
			if (SAKUYA_POINT_X_MID > s->x256 )
			{
				s->x256 += t256(3.0/*3.0*/)/*fps_fa ctor*/;
				if (SAKUYA_ANIME_09_RIGHT > s->anim_frame)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (SAKUYA_ANIME_04_CENTER_A < data->wait3)
					{
						s->anim_frame++;
						data->wait3 = 0;
					}
				}
			}
			else	/* 真ん中まで来たら */
			{
				data->state1++/*	= SA05*/;
			//	if (RANK_NORMAL<difficulty) 	{	bullet_create_aka_lines(s); 	}
			}
			break;
/*----------------*/
		case SA06aaa:/* 移動位置を決める */
				data->state1++/*	= SA05*/;
			//	my_yudo_point = SAKUYA_POINT_X_RIGHT_MID;
				my_yudo_point = SAKUYA_POINT_X_MID+(ra_nd()&((128*256)-1));
			break;
/*----------------*/
		case SA07:	/* 右へ移動 */
			if ( my_yudo_point > s->x256/*+((s->w128+s->w128))*/ )/*t256(GAME_WIDTH-50)*/
			{
				s->x256 += t256(2.0/*3*/)/*fps_fa ctor*/;
			}
			else
			{
				data->state1++/*	= SA06*/;
		#if 1/*可変弾幕*/
				tmp_angle_jikinerai512(player, s);/*自機ねらい角作成*/
			#if 0
			/*CW*/
				data->boss_base.tmp_angleCCW512 	-= 128;
			#else
			/*CCW*/
				data->boss_base.tmp_angleCCW512 	+= (256-128);
			#endif
		#else/*固定弾幕*/
			/*CCW*/
				data->boss_base.tmp_angleCCW512 	= deg_360_to_512CCW((90));/* 右側から撃ち始める (128)*/
		#endif
				data->boss_base.danmaku_type		= DANMAKU_01;			/* 禊弾幕をセット */
				data->boss_base.danmaku_time_out	= DANMAKU_01_SET_TIME;	/* 禊弾幕の発生時間 */
			//	s->anim_frame			= SAKUYA_ANIME_04_CENTER_A;
				s->anim_frame			= SAKUYA_ANIME_15_HATUDAN;
			}
			break;
		case SA08:	/* 弾撃ちアニメ待機 */
			sakuya_tama_anime_wait3_0(s);
			break;
		case SA09:	/* 禊弾幕が終わるまで待機 */
			danmaku_state_check_holding(s/*, SA07*/ /*, SAKUYA_ANIME_04_CENTER_A*/);
			break;
		case SA10:
			s->anim_frame			= SAKUYA_ANIME_04_CENTER_A;
			data->state1++/*	= SA06*/;
			break;
		case SA11:	/* 真ん中に戻る */
			if (SAKUYA_POINT_X_MID < s->x256 )
			{
				s->x256 -= t256(3.0/*3.0*/)/*fps_fa ctor*/;
				if (SAKUYA_ANIME_00_LEFT < s->anim_frame)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (SAKUYA_ANIME_04_CENTER_A < data->wait3)
					{
						s->anim_frame--;
						data->wait3 = 0;
					}
				}
			}
			else	/* 真ん中に戻る */
			{
				data->state1++/* = SA02*/;
			//	if (RANK_NORMAL<difficulty) 	{	bullet_create_aka_lines(s); 	}
			}
			break;
/*----------------*/
		case SA12:/* 移動位置を決める */
			data->state1	= SA02;
		//	my_yudo_point = SAKUYA_POINT_X_LEFT_MID;
			my_yudo_point = SAKUYA_POINT_X_MID-(ra_nd()&((128*256)-1));
			break;
		}
		break;
	//

/* 第二形態では使って無いみたいだから利用する */
#define sayuu_mode wait2_256
	case SAKUYA_02_KEITAI/*1*/: // 第二形態: 全方位、豆まき
		//data->wait2_256		ループ回数
		switch (data->state1)
		{
		case SB00:	/* 初期値 */
			data->wait1 		= 0;/*とりあえず*/
//			data->wait2_256 	= t256(0);
			data->sayuu_mode	= (ra_nd()&(2-1)/*%2*/);
			data->state1++/*	= SB01*/;
			break;

//		case SB00aaa:	/* 待機 */
//			sakuya_wait_state(s, SAKUYA_ANIME_04_CENTER_A);
		//		data->wait1 -= 1/*fps_fa ctor*/;
		//	if (data->wait1<0)
		//	{
		//		data->state1++/*	= SB01*/;
//++	//		pd->bo ssmode	= B00_NONE/*B01_BA TTLE*/;
		//	}	//		else	{	}
//			break;

//-----------

		case SB01:
			s->x256 = t256(GAME_WIDTH/2);	/* 真中へワープ */
			{
//				int ang512;
				data->sayuu_mode ^= (2-1);/* 反転 */
				if (0==data->sayuu_mode)
				{
					data->move_vx256 = t256(1.0/*0.5*/);
//					ang512=(ra_nd()&(256-1)/*%(180*1.42)*/);
//					if (ang512>deg_360_to_512(90))		{	ang512+=deg_360_to_512(90); 	s->anim_frame=(SAKUYA_ANIME_04_CENTER_A+1); }
//					else								{									s->anim_frame=(SAKUYA_ANIME_04_CENTER_A-1); }
//					ang512 -= deg_360_to_512(45);
				}
				else
				{
					data->move_vx256 = -t256(1.0/*0.5*/);
//					if (s->x256 > t256(GAME_WIDTH/2) )	{	ang512=deg_360_to_512(180); 	s->anim_frame=(SAKUYA_ANIME_04_CENTER_A+2); }
//					else								{	ang512=deg_360_to_512(0);		s->anim_frame=(SAKUYA_ANIME_04_CENTER_A-2); }
				}
//				data->move_angle512=/*deg512_2rad*/(ang512);
			}
//			data->wait1 		= 20+(3-difficulty)*10;
			data->wait1 		= (35);/* [WAITSTATE] */	/* 弾撃たないでちょっと移動 */
//			data->wait2_256 += t256(1);
			//
			s->anim_frame = SAKUYA_ANIME_04_CENTER_A;
			data->wait3=0;
			data->state1++/*	= SB02*/;
			break;

		case SB03:
			// 移動
			s->x256 += ((data->move_vx256))/**fps_fa ctor*/;	// 移動 	/* 弾撃たないでちょっと移動 */
			sakuya_anime_to_sayuu(s);
			break;
		case SB03aaa:
			s->x256 += ((data->move_vx256))/**fps_fa ctor*/;	// 移動 	/* 弾撃たないでちょっと移動 */
			sakuya_wait_state(s/*, SB04*/, SAKUYA_ANIME_15_HATUDAN);
			break;
		case SB03bbb:
			s->anim_frame = SAKUYA_ANIME_15_HATUDAN;
			data->wait3 = 0;
			data->state1++/*	= SB03*/;
			break;
		case SB04:	/* 弾撃ちアニメ待機 */
			sakuya_tama_anime_wait3_0(s);
			break;
		case SB05:
			//	enemy_boss05_mamemaki_state(s, SB06, SAKUYA_ANIME_18_);
				data->boss_base.danmaku_type		= DANMAKU_02;			/* 24弾幕をセット */
				data->boss_base.danmaku_time_out	= DANMAKU_02_SET_TIME;	/* 24弾幕の発生時間 */
				data->state1++/*	= SB06*/;
			break;
		case SB06:	/* 24弾幕が終わるまで移動 */
			s->x256 += ((data->move_vx256))/**fps_fa ctor*/;	// 移動 	/* 弾撃ちながら移動 */
			danmaku_state_check_holding(s/*, SA07*/ /*, SAKUYA_ANIME_04_CENTER_A*/);
			break;
		case SB07:
				s->x256 = t256(GAME_WIDTH)-s->x256; 			/* 反対側へワープ */
				s->anim_frame = SAKUYA_ANIME_15_HATUDAN;
				data->wait3 = 0;
				data->state1++/*	= SB08*/;
			break;
		case SB08:	/* 弾撃ちアニメ待機 */
			sakuya_tama_anime_wait3_0(s);
			break;
		case SB09:
				data->boss_base.danmaku_type		= DANMAKU_03;			/* 11弾幕をセット */
				data->boss_base.danmaku_time_out	= DANMAKU_03_SET_TIME;	/* 11弾幕の発生時間 */
				data->wait1 			= (35);/* (SB11で)少し待機 */
				s->anim_frame			= SAKUYA_ANIME_04_CENTER_A;
				data->state1++/*	= SB06*/;
			break;
		case SB10:	/* 11弾幕が終わるまで待機 */
			danmaku_state_check_holding(s/*, SA07*/ /*, SAKUYA_ANIME_04_CENTER_A*/);
			break;
		case SB11:	/* (SB11で)少し待機 */
			sakuya_wait_state(s/*, SB04*/, SAKUYA_ANIME_04_CENTER_A);
			break;
		case SB12:
			data->state1	= SB01;
			break;
		}
		enemy_boss05_out(s);
		break;
	//

//
	case SAKUYA_03_KEITAI/*2*/: 	// 第三形態: 垂直ナイフが落ちてくるよ
		switch (data->state1)
		{
		case SC00:
			sakuya_wait_state(s, SAKUYA_ANIME_04_CENTER_A);
		//	data->wait1 -= 1/*fps_fa ctor*/;
		//	if (data->wait1<0)	// 待機時間が終わったら
		//	{
		//		data->state1++/*		= SC01*/;
//++	//		pd->bossmode		= B00_NONE/*B01_BATTLE*/;
		//		s->anim_frame		= SAKUYA_ANIME_04_CENTER_A;
		//	}
		//	else
		//	{
		//	}
			break;
		case SC01:
			if (s->x256 < (SAKUYA_LIMIT_X_LEFT)/*5*/ )	// 左隅に来たら 	/* → || (s->y<5)があると無限ループになる*/
			{
				data->state1++/*	= SC02*/;
				data->wait1 	= 10;
				data->wait2_256 = t256(0);
				s->anim_frame	= SAKUYA_ANIME_00_LEFT;
			}
			else
			{
				move_sakuya(s, t256(3), SAKUYA_POINT_00_LEFT);
				if (s->anim_frame>SAKUYA_ANIME_00_LEFT)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=3)
					{
						s->anim_frame--;
						data->wait3=0;
					}
				}
			}
			break;
		case SC02:
			sakuya_wait_state(s/*, SC03*/, SAKUYA_ANIME_02_/*???*/);
			break;
		case SC03:
			if (s->x256 > (SAKUYA_LIMIT_X_RIGHT)/*GAME_WIDTH-s->w+3*/ ) // 右端に来たら
			{
				data->state1++/*	= SC04*/;
				data->wait2_256 = t256(0);
				s->anim_frame	= SAKUYA_ANIME_09_RIGHT;
			}
			else
			{
				{	const short sss[4] =
					{
						t256(1.2),/*easy:まばらなので簡単*/
						t256(0.5/*1.0*/ /*1.5*/),/*normal:塊なので大きく避ければ簡単*/
						t256(1.0/*2.0*/),/*hard:たまに難しい時がある*/
						t256(18.5/*6.5*/ /*12.5*/ /*8.5*/) /*luna:うへぇこのスキマくぐるんですかぁ？*/
						/*(luna:ボムらなくても気合避けでなんとかなる程度に難易度抑えたいのだがまだ高すぎるな)*/
					};
					data->wait2_256 += /*t256(4)*/sss[difficulty]/*t256(3*1.42)*/;
				}
				if (0==(((int)data->wait2_256)&(16-1)/*%21 t256(15*1.42)*/))
				{
#if 0
/* CWの場合 */
					bullet_create_sakuya_no_rot_knife(s, /*(0.7)*/t256(2)/*(2)*/, (deg_360_to_512(360)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.03));
					//if (difficulty)
					{
						bullet_create_sakuya_no_rot_knife(s, /*(0.7)*/t256(2)/*(2)*/, (deg_360_to_512(270)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.04));
					//	if (difficulty>1)
						{	bullet_create_sakuya_no_rot_knife(s, /*(0.7)*/t256(2)/*(2)*/, (deg_360_to_512(180)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02));}
					}
#else
/* CCWの場合 */
					bullet_create_sakuya_no_rot_knife(s, /*(0.7)*/t256(2)/*(2)*/, (-deg_360_to_512CCW(360)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.03));
					//if (difficulty)
					{
						bullet_create_sakuya_no_rot_knife(s, /*(0.7)*/t256(2)/*(2)*/, (-deg_360_to_512CCW(270)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.04));
					//	if (difficulty>1)
						{	bullet_create_sakuya_no_rot_knife(s, /*(0.7)*/t256(2)/*(2)*/, (-deg_360_to_512CCW(180)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02));}
					}
#endif
					play_voice_auto_track(VOICE12_SAKUYA_B2_SHOT);
				}
				move_sakuya(s, t256(5), SAKUYA_POINT_03_RIGHT);
				if (s->anim_frame<SAKUYA_ANIME_09_RIGHT)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=7)
					{
						s->anim_frame++;
						data->wait3=0;
					}
				}
			}
			break;
		case SC04:
		//	if (( s->x256 < (SAKUYA_LIMIT_X_MID)/*GAME_WIDTH/2-s->w/2*/ ))	// 真ん中に来たら
			if (( s->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
				( s->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		// 真ん中に来たら
			{
				data->state1++/*	= SC05*/;
				s->anim_frame	= SAKUYA_ANIME_04_CENTER_A;
			}
			else
			{
				move_sakuya(s, t256(3), SAKUYA_POINT_01_MID_UP);
				if (SAKUYA_ANIME_04_CENTER_A < s->anim_frame)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=3)
					{
						s->anim_frame--;
						data->wait3=0;
					}
				}
			}
			break;
		case SC05:
			if (s->x256 > (SAKUYA_LIMIT_X_RIGHT)/*GAME_WIDTH-s->w-3*/ ) // 右端に来たら /* → || (s->y<5)があると無限ループになる */
			{
				data->state1++/*	= SC06*/;
				data->wait1 	= 10;
				data->wait2_256 = 0;
				s->anim_frame	= SAKUYA_ANIME_09_RIGHT;
			}
			else
			{
				move_sakuya(s, t256(3), SAKUYA_POINT_03_RIGHT);
				if (s->anim_frame<SAKUYA_ANIME_09_RIGHT)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=3)
					{
						s->anim_frame++;
						data->wait3=0;
					}
				}
			}
			break;
		case SC06:
			if (s->x256 < (SAKUYA_LIMIT_X_LEFT)/*5*/)	// 左端に来たら
			{
				data->state1++/*	= SC07*/;
				data->wait2_256 = 0;
				s->anim_frame	= SAKUYA_ANIME_00_LEFT;
			}
			else
			{
				data->wait2_256 += t256(4)/*(3*1.42)*/;
				if (0x0000==((data->wait2_256)&(0x0f00))/*%(12*1.42)*/)
				{
#if 0
/* CWの場合 */
					bullet_create_sakuya_no_rot_knife(s, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512(180)), t256(0.03));
					if (difficulty)
					{
						bullet_create_sakuya_no_rot_knife(s, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512(90)), t256(0.04));
						if (difficulty>1)
						{	bullet_create_sakuya_no_rot_knife(s, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02));}
					}
#else
/* CCWの場合 */
					bullet_create_sakuya_no_rot_knife(s, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512CCW(360-180)), t256(0.03));
					if (difficulty)
					{
						bullet_create_sakuya_no_rot_knife(s, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512CCW(360-90)), t256(0.04));
						if (difficulty>1)
						{	bullet_create_sakuya_no_rot_knife(s, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02));}
					}
#endif
					play_voice_auto_track(VOICE12_SAKUYA_B2_SHOT);
				}
				move_sakuya(s, t256(5), SAKUYA_POINT_00_LEFT);
				if (s->anim_frame>SAKUYA_ANIME_00_LEFT)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=7)
					{
						s->anim_frame--;
						data->wait3=0;
					}
				}
			}
			break;
		case SC07:
		//	if (( s->x256 > (SAKUYA_LIMIT_X_MID)/*(GAME_WIDTH/2-s->w/2)*/ ))	// 真ん中に来たら
			if (( s->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
				( s->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		// 真ん中に来たら
			{
				data->state1++/*	= SC08*/;
				data->wait2_256 = t256(60);
				s->anim_frame	= SAKUYA_ANIME_04_CENTER_A;
			}
			else
			{
				move_sakuya(s, t256(3), SAKUYA_POINT_01_MID_UP);
				if (SAKUYA_ANIME_04_CENTER_A > s->anim_frame)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=3)
					{
						s->anim_frame++;
						data->wait3=0;
					}
				}
			}
			break;
		case SC08:
			data->wait2_256 -= t256(1);
			if (data->wait2_256 < 0)
			{
			//	data->state1++/*	= SC01*/;
				data->state1	= SC01;
				s->anim_frame	= SAKUYA_ANIME_04_CENTER_A;
			}
			else
			{
//				data->move_angle512=atan_512_SAKUYA_MID;
				if (0 == ((t256_floor(data->wait2_256))&(32-1)/*%20*/) )
				{
					int b_wait2_high;
					b_wait2_high=(((int)((t256_floor(data->wait2_256))>>5/*/20*/)));
				//	s->anim_frame++;	s->anim_frame %= 7;//??? 9???
					s->anim_frame++;	s->anim_frame &= (8-1);
					{
#if 0
/* CWの場合 */
						bullet_create_sakuya_follow_knife1(s, (b_wait2_high<<8)+t256(4.0), (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(40))), -80);
						bullet_create_sakuya_follow_knife1(s, (b_wait2_high<<8)+t256(3.0), (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(30))), -60);
						if (difficulty>1)
						{
							bullet_create_sakuya_follow_knife1(s, (b_wait2_high<<8)+t256(2.0), (deg_360_to_512((180-30))+((b_wait2_high)*deg_360_to_512(20))), -40);
						}
#else
/* CCWの場合 */
							bullet_create_sakuya_follow_knife1(s, (b_wait2_high<<8)+t256(4.0), deg_360_to_512CCW(/*360+*/90-30)+(b_wait2_high)*deg_360_to_512CCW(40), -80);
							bullet_create_sakuya_follow_knife1(s, (b_wait2_high<<8)+t256(3.0), deg_360_to_512CCW(/*360+*/90-30)+(b_wait2_high)*deg_360_to_512CCW(30), -60);
						if (difficulty>1)
						{
							bullet_create_sakuya_follow_knife1(s, (b_wait2_high<<8)+t256(2.0), deg_360_to_512CCW(/*360+*/90-30)+(b_wait2_high)*deg_360_to_512CCW(20), -40);
						}
#endif
					}
					s->anim_frame = SAKUYA_ANIME_18_;
				}
			}
			if (s->anim_frame>SAKUYA_ANIME_15_HATUDAN)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=4)
				{
					s->anim_frame--;
					data->wait3=0;
				}
			}
			break;
	//	case SC09:
	//			data->state1	= SC01;
	//		break;
		}
		break;


/*---------------------------------------------------------
	enemy_boss05_knifes1
	アングルは角度指定で(-90)～(90)
	heightは"playerから"の高さ
---------------------------------------------------------*/

#if 0
static void enemy_boss05_knifes1(SPRITE *s, int speed256, int angle512, int height)
{
	/*	*/
	/*	*/
#if 0
/* CWの場合 */
//	bullet_create_sakuya_follow_knife1(s, speed256, /*deg_360_to_512*/(deg_360_to_512((180))-angle512), height);
#else
/* CCWの場合 */
//	bullet_create_sakuya_follow_knife1(s, speed256, /*deg_360_to_512*/(-deg_360_to_512CCW((180))-angle512), height);
	bullet_create_sakuya_follow_knife1(s, speed256, /*deg_360_to_512*/(/*deg_360_to_512CCW(360+90-(180))-*/angle512), height);
#endif
//	bullet_create_sakuya_follow_knife1(s, speed256, /*deg_360_to_512*/(angle512), height);
}
#endif
//------------------------
	//	あまりに短いので少し長くする
	case SAKUYA_04_KEITAI_A/*3*/:	// 第四形態: (黄色マスカット弾)
	case SAKUYA_04_KEITAI_B/*4*/:	// 第四形態: (黄色マスカット弾) 	/* 魔方陣生成(2回生成しちゃうバグあり) */
		switch (data->state1)
		{
		case SD00:
			sakuya_wait_state(s, SAKUYA_ANIME_15_HATUDAN);
		//	data->wait1 -= 1/*fps_fa ctor*/;
		//	if (data->wait1<0)	// 待機時間が終わったら
		//	{
		//		data->state1++/*	= SD01*/;
//++	//			pd->bo ssmode=B00_NONE/*B01_BA TTLE*/;
//???	//		data->boss_base.boss_timer += (20*64);			// + 20 [秒] ???	/* 16.666[秒] 1000 ???*/
		//		s->anim_frame=SAKUYA_ANIME_15_HATUDAN;
		//	}
		//	else	{	}
			break;
		case SD01:
		//	if ( ( s->y > 50) && (s->x<GAME_WIDTH/2+30 || s->x>GAME_WIDTH/2-30))		// 真ん中に来たら
			if (( s->y256 > (SAKUYA_POINT_Y_CENTER-SAKUYA_POINT_Y_MARGIN) ) &&
				( s->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
				( s->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		// 真ん中に来たら
			{
				data->state1++/*	= SD02*/;
				data->wait1 	= 10;
				data->wait2_256 = 0;
				s->anim_frame	= SAKUYA_ANIME_09_;
			}
			else
			{
				data->move_angle512=atan_512_SAKUYA_CENTER/*atan_512(50-s->y,GAME_WIDTH/2-s->w-s->x)*/;
#if 0
/* CWの場合 */
				s->x256 += ((cos512((data->move_angle512))*t256(3))>>8)/**fps_fa ctor*/;
				s->y256 += ((sin512((data->move_angle512))*t256(3))>>8)/**fps_fa ctor*/;
#else
/* CCWの場合 */
				s->x256 += ((sin512((data->move_angle512))*t256(3))>>8)/**fps_fa ctor*/;
				s->y256 += ((cos512((data->move_angle512))*t256(3))>>8)/**fps_fa ctor*/;
#endif
				if (s->anim_frame<SAKUYA_ANIME_18_)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=3)
					{
						s->anim_frame++;
						data->wait3=0;
					}
				}
			}
			break;
		case SD02:
			if (s->anim_frame==SAKUYA_ANIME_14_)
			{
				mahoujin_num++;
				if (1==mahoujin_num)
				{
					enemy_boss05_maho_create(s);		/* 魔方陣生成(2回生成しちゃうバグあり) */
				}
				data->state1++/*	= SD03*/;
				data->wait2_256 = t256(10);
				data->wait1 	= 5;
			}
			else
			{
				if (s->anim_frame<SAKUYA_ANIME_14_)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=3)
					{
						s->anim_frame++;
						data->wait3=0;
					}
				}
			}
			break;
//-------------
		case SD03:
			tmp_angle_jikinerai512(player, s);
			data->wait2_256 -= t256(1)/*fps_fa ctor*/;
			if (data->wait2_256<t256(0))
			{
				data->wait2_256=t256(50);
				int *s_data_angle512;
				s_data_angle512 = &(((BOSS05_DATA *)s->data)->boss_base.tmp_angleCCW512);
				/* (黄色マスカット弾) */
#if 0
/* CWの場合 */
				enemy_sp1_bullet_create(s, (t256(0.70)), data->boss_base.tmp_angleCCW512-1*42/*deg_360_to_512(30)*/, (t256(0.030)), 1/* 1==R*/, s_data_angle512 );/*0.72 == 1.2*0.6*/
				enemy_sp1_bullet_create(s, (t256(0.70)), data->boss_base.tmp_angleCCW512+1*42/*deg_360_to_512(30)*/, (t256(0.030)), -1/*-1==L*/, s_data_angle512 );/*0.72 == 1.2*0.6*/
				enemy_sp1_bullet_create(s, (t256(0.85)), data->boss_base.tmp_angleCCW512-2*42/*deg_360_to_512(30)*/, (t256(0.033)), 1/* 1==R*/, s_data_angle512 );/*0.9 == 1.5*0.6*/
				enemy_sp1_bullet_create(s, (t256(0.85)), data->boss_base.tmp_angleCCW512+2*42/*deg_360_to_512(30)*/, (t256(0.033)), -1/*-1==L*/, s_data_angle512 );/*0.9 == 1.5*0.6*/
				if (difficulty)
				{
					enemy_sp1_bullet_create(s, (t256(1.0)), data->boss_base.tmp_angleCCW512-3*42/*deg_360_to_512(30)*/, (t256(0.036)),	1/* 1==R*/, s_data_angle512 );/*1.08 == 1.8*0.6*/
					enemy_sp1_bullet_create(s, (t256(1.0)), data->boss_base.tmp_angleCCW512+3*42/*deg_360_to_512(30)*/, (t256(0.036)), -1/*-1==L*/, s_data_angle512 );/*1.08 == 1.8*0.6*/
					if (1<difficulty)
					{
						enemy_sp1_bullet_create(s, (t256(1.15)), data->boss_base.tmp_angleCCW512-4*42/*deg_360_to_512(30)*/, (t256(0.04)),	1/* 1==R*/, s_data_angle512 );/*1.26 == 2.1*0.6*/
						enemy_sp1_bullet_create(s, (t256(1.15)), data->boss_base.tmp_angleCCW512+4*42/*deg_360_to_512(30)*/, (t256(0.04)), -1/*-1==L*/, s_data_angle512 );/*1.26 == 2.1*0.6*/
					}
				}
#else
/* CCWの場合 */
				enemy_sp1_bullet_create(s, (t256(0.70)), data->boss_base.tmp_angleCCW512+1*42/*deg_360_to_512(30)*/, (t256(0.030)), 1/* 1==R*/, s_data_angle512 );/*0.72 == 1.2*0.6*/
				enemy_sp1_bullet_create(s, (t256(0.70)), data->boss_base.tmp_angleCCW512-1*42/*deg_360_to_512(30)*/, (t256(0.030)), -1/*-1==L*/, s_data_angle512 );/*0.72 == 1.2*0.6*/
				enemy_sp1_bullet_create(s, (t256(0.85)), data->boss_base.tmp_angleCCW512+2*42/*deg_360_to_512(30)*/, (t256(0.033)), 1/* 1==R*/, s_data_angle512 );/*0.9 == 1.5*0.6*/
				enemy_sp1_bullet_create(s, (t256(0.85)), data->boss_base.tmp_angleCCW512-2*42/*deg_360_to_512(30)*/, (t256(0.033)), -1/*-1==L*/, s_data_angle512 );/*0.9 == 1.5*0.6*/
				if (difficulty)
				{
					enemy_sp1_bullet_create(s, (t256(1.0)), data->boss_base.tmp_angleCCW512+3*42/*deg_360_to_512(30)*/, (t256(0.036)),	1/* 1==R*/, s_data_angle512 );/*1.08 == 1.8*0.6*/
					enemy_sp1_bullet_create(s, (t256(1.0)), data->boss_base.tmp_angleCCW512-3*42/*deg_360_to_512(30)*/, (t256(0.036)), -1/*-1==L*/, s_data_angle512 );/*1.08 == 1.8*0.6*/
					if (1<difficulty)
					{
						enemy_sp1_bullet_create(s, (t256(1.15)), data->boss_base.tmp_angleCCW512+4*42/*deg_360_to_512(30)*/, (t256(0.04)),	1/* 1==R*/, s_data_angle512 );/*1.26 == 2.1*0.6*/
						enemy_sp1_bullet_create(s, (t256(1.15)), data->boss_base.tmp_angleCCW512-4*42/*deg_360_to_512(30)*/, (t256(0.04)), -1/*-1==L*/, s_data_angle512 );/*1.26 == 2.1*0.6*/
					}
				}
#endif
				//bullet_create_oodama0(s, t256(2), ANGLE_JIKINERAI_DAN, 40, 0, t256(s->w/2), t256(s->h/2));
				data->wait1 -= 1/*fps_fa ctor*/;
				if (data->wait1<0)	//3回撃ったら
				{
					data->wait1=20;
					if (0==(ra_nd()&(2-1)/*%2*/))
					{
						data->state1++/*	= SD04*/;
						s->anim_frame=(SAKUYA_ANIME_04_CENTER_A-2);
					}
					else
					{
						data->state1/*+=2*/ = SD05;
						s->anim_frame=(SAKUYA_ANIME_04_CENTER_A+2);
					}
				}
			}
			if (s->anim_frame>SAKUYA_ANIME_09_)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					data->wait3=0;
					s->anim_frame--;
				}
			}
			break;
		case SD04:
			data->wait1 -= 1/*fps_fa ctor*/;
			if (data->wait1<0)
			{
				data->wait1 	= 3;
				data->state1--/*	= SD03*/;
				data->wait2_256=t256(30);
				s->anim_frame=SAKUYA_ANIME_18_;
			}
			else
			{
				if (s->x256 < t256(100) )
				{
					data->state1++/* = SD05*/;
					s->anim_frame=SAKUYA_ANIME_09_RIGHT-1/*SAKUYA_ANIME_08_RIGHT-1*/;/*7 ???*/
				}
				else
				{	s->x256 -= t256(2)/**fps_fa ctor*/;}
			}
			break;
		case SD05:
			data->wait1 -= 1/*fps_fa ctor*/;
			if (data->wait1<0)
			{
				data->wait1 	= 3;
				data->state1	= SD03;
				data->wait2_256 = t256(30);
				s->anim_frame	= SAKUYA_ANIME_18_;
			}
			else
			{
				if (s->x256 > t256(GAME_WIDTH-100)-((s->w128+s->w128)) )
				{
					data->state1--/* = SD04*/;
					s->anim_frame=SAKUYA_ANIME_00_LEFT+1;/*1 ???*/
				}
				else
				{	s->x256 += t256(2)/**fps_fa ctor*/;}
			}
			break;
		}
		break;
	//
	case SAKUYA_08_KEITAI/*5*/ /*4*/:	/* 第八形態: 最終形態(その1) */
#if 1
		switch (data->state1)
		{
		case SF00:
				data->wait1 -= 1/*fps_fa ctor*/;
			if (data->wait1<0)	// 待機時間が終わったら */
			{
				data->state1++/* = SF01*/;
//++			pd->bo ssmode=B00_NONE/*B01_BA TTLE*/;
				data->move_angle512=atan_512(t256(30)-(s->y256),t256(GAME_WIDTH/2)-((s->w128))-(s->x256));
			}
			else
			{
			}
			break;
		case SF01:
			if ( (s->x256 < t256(GAME_WIDTH/2+30) ) &&
				 (s->x256 > t256(GAME_WIDTH/2-10) ) &&
				 (s->y256 < t256(50)) ) 	// 真ん中に来たら */
			{
				data->state1++/*	= SF02*/;
				data->wait1 	= 20;
				data->wait2_256 = 0;
				s->anim_frame=SAKUYA_ANIME_04_CENTER_A;
			}
			else
			{
#if 0
/* CWの場合 */
				s->x256 += ((cos512((data->move_angle512))*t256(3))>>8);/**fps_fa ctor*/
				s->y256 += ((sin512((data->move_angle512))*t256(3))>>8);/**fps_fa ctor*/
#else
/* CCWの場合 */
				s->x256 += ((sin512((data->move_angle512))*t256(3))>>8);/**fps_fa ctor*/
				s->y256 += ((cos512((data->move_angle512))*t256(3))>>8);/**fps_fa ctor*/
#endif
			}
			break;
		case SF02:
			{
				const Uint8 aaa_tbl[40/*(6*6)*/] =
				{
					SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,	SAKUYA_ANIME_14_,
					SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,	SAKUYA_ANIME_13_,
					SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,	SAKUYA_ANIME_12_,
					SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,	SAKUYA_ANIME_11_,
					SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,	SAKUYA_ANIME_10_,
					SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,	SAKUYA_ANIME_09_,
					0,0,0,0/* アライン用ダミーデーター。 dummy for align*/
				};
					data->wait1 -= 1/*fps_fa ctor*/;
				if (data->wait1<0)			{	data->state1++/* = SF03*/; s->anim_frame=SAKUYA_ANIME_15_HATUDAN;	}		// 待機時間が終わったら */
			//	else if (data->wait1<6) 	{						s->anim_frame=SAKUYA_ANIME_14_; 	}
			//	else if (data->wait1<12)	{						s->anim_frame=SAKUYA_ANIME_13_; 	}
			//	else if (data->wait1<18)	{						s->anim_frame=SAKUYA_ANIME_12_; 	}
			//	else if (data->wait1<24)	{						s->anim_frame=SAKUYA_ANIME_11_; 	}
			//	else if (data->wait1<30)	{						s->anim_frame=SAKUYA_ANIME_10_; 	}
			//	else if (data->wait1<36)	{						s->anim_frame=SAKUYA_ANIME_09_; 	}
				else if (data->wait1<36)	{						s->anim_frame=aaa_tbl[data->wait1]; 	}
			}
			break;
		case SF03:
			if (s->anim_frame==SAKUYA_ANIME_18_)
			{
			//	距離を求めるのに２乗してルートを取るのはルート演算が遅いので、工夫が必要かも。
			//	予めテーブル化しておくか、代替演算(CODIC)で求めるか、共通関数にしてCPUのfloat変換命令を最小に留めるか、どれか。
			//	data->length_s_p=sqrt((player->x-s->x)*(player->x-s->x)+(player->y-s->y)*(player->y-s->y));
				{	/* sqrt()遅いので簡易版。というか距離には拘らない事にした */
					int xxx256;
					int yyy256;
					xxx256=abs(player->x256-s->x256);xxx256+=256;/* abs()は展開してくれるから大丈夫 */
					yyy256=abs(player->y256-s->y256);yyy256+=256;
					if (xxx256>yyy256)	{data->length_s_p256=(xxx256/*+(yyy256>>1)*/);}/*少し難しくする*/
					else				{data->length_s_p256=(yyy256/*+(xxx256>>1)*/);}/*少し難しくする*/
				}
#if 1
				bullet_create_sakuya_even_knife(s, t256(10), data->length_s_p256, 0/*0==右*/);
				#if 1
				bullet_create_sakuya_even_knife(s, t256(10), data->length_s_p256, 1/*1==左*/);
				#endif
#endif
				data->wait1=40;
				if (s->y256 > t256(150))
#if 0
/* CWの場合 */
				{	data->move_angle512=deg_360_to_512(210) + (((ra_nd()&(2048-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
#else
/* CCWの場合 */
				{	data->move_angle512=deg_360_to_512CCW(360-210) + (((ra_nd()&(2048-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
#endif
				else
				{	data->move_angle512=/*deg512_2rad*/((ra_nd()&(512-1)))/*degtorad(ra_nd()%360)*/;}
				#if 1
				/*念の為*/
				mask512(data->move_angle512);
				#endif
				s->anim_frame	= SAKUYA_ANIME_16_;/*16???*/
				data->state1++/*	= SF04*/;
			}
			else
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					s->anim_frame++;
					data->wait3=0;
				}
			}
			break;
		case SF04:
				data->wait1 -= 1/*fps_fa ctor*/;
			if (data->wait1<0)
			{
				data->wait1 	= 120+(3-difficulty)*50;
				data->state1	= SF02;
			}
			else
			{
#if 0
/* CWの場合 */
				s->x256 += ((cos512((data->move_angle512))*t256(3.5))>>8)/**fps_fa ctor*/;
				s->y256 += ((sin512((data->move_angle512))*t256(3.5))>>8)/**fps_fa ctor*/;
#else
/* CCWの場合 */
				s->x256 += ((sin512((data->move_angle512))*t256(3.5))>>8)/**fps_fa ctor*/;
				s->y256 += ((cos512((data->move_angle512))*t256(3.5))>>8)/**fps_fa ctor*/;
#endif
			}
			break;
		}
		if ((player->x256+((player->w128)) < s->x256+((s->w128))+t256(25))&&
			(player->x256+((player->w128)) > s->x256+((s->w128))-t256(25)))
		{	data->wait2_256 += t256(1)/*fps_fa ctor*/;}
		if (data->wait2_256 > t256(20)+(3-difficulty)*t256(30))
		{
			s->anim_frame=SAKUYA_ANIME_16_;/*16???*/
		//	距離を求めるのに２乗してルートを取るのはルート演算が遅いので、工夫が必要かも。
		//	予めテーブル化しておくか、代替演算(CODIC)で求めるか、共通関数にしてCPUのfloat変換命令を最小に留めるか、どれか。
		//	data->length_s_p=sqrt((player->x-s->x)*(player->x-s->x)+(player->y-s->y)*(player->y-s->y));
				{	/* sqrt()遅いので簡易版。というか距離には拘らない事にした */
					int xxx256;
					int yyy256;
					xxx256=abs((int)player->x256-(int)s->x256);xxx256+=256;/* abs()は展開してくれるから大丈夫 */
					yyy256=abs((int)player->y256-(int)s->y256);yyy256+=256;
					if (xxx256>yyy256)	{data->length_s_p256=(xxx256/*+(yyy256>>1)*/);}/*少し難しくする*/
					else				{data->length_s_p256=(yyy256/*+(xxx256>>1)*/);}/*少し難しくする*/
				}
			if (data->length_s_p256>t256(80))
			{	data->length_s_p256=t256(80);}
			enemy_boss05_knifes3(s/*,*/ /*1+difficulty,*/ /*(data->length_s_p256)*/ );
		}
		enemy_boss05_out(s);
#endif
		break;

	//
	case SAKUYA_09_KEITAI/*6*/ /*5*/:	/* 第九形態: 最終形態(その2) */
#if 1
		if (data->wait2_256 < t256(0) )
		{
			data->wait2_256 = t256(0);
			data->wait1 	= 120;
			data->move_type = SAKUYA_08_KEITAI/*4*/;
			data->state1	= SF02;
			s->anim_frame	= SAKUYA_ANIME_04_CENTER_A;
		}
		else
		{
			data->wait2_256 -= t256(0.03);
			if (s->alpha>249)//if (s->alpha>255)
			{
				s->alpha = 255;
			}
			else //if (s->alpha<255)
			{
				s->alpha += 5;
			}
#if 0
/* CWの場合 */
			s->x256 += ((cos512((data->boss_base.tmp_angleCCW512))*(data->wait2_256))>>8)/**fps_fa ctor*/;
			s->y256 += ((sin512((data->boss_base.tmp_angleCCW512))*(data->wait2_256))>>8)/**fps_fa ctor*/;
#else
/* CCWの場合 */
			s->x256 += ((sin512((data->boss_base.tmp_angleCCW512))*(data->wait2_256))>>8)/**fps_fa ctor*/;
			s->y256 += ((cos512((data->boss_base.tmp_angleCCW512))*(data->wait2_256))>>8)/**fps_fa ctor*/;
#endif
		}
		if (s->anim_frame>SAKUYA_ANIME_15_HATUDAN)
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3 >= 4)
			{
				s->anim_frame--;
				data->wait3 = 0;
			}
		}
		enemy_boss05_out(s);
#endif
		break;
	}
//
	danmaku_generator(s);	/* 弾幕生成 */
//
	/*---------------------------------------------------------
		ダメージチェック
	---------------------------------------------------------*/
//
	{
	//	if (data->boss_base.boss_health < (/*4*/SAKUYA_08_KEITAI-data->move_type)*1024)
		if (0==data->boss_base.boss_health)
		{
			if (0 != (data->boss_base.boss_life))/* 形態変更したら */
			{
				data->boss_base.boss_life--;
				sakuya_put_items(s/*,t*/);
			}
		}
	}
//
	{
		if (0 == data->boss_base.boss_life)
		{
			if (data->boss_base.boss_health < 1)/*全員倒した？*/
			{
				item_create(s, SP_ITEM_05_HOSI/*SP_ITEM_06_TENSU*/, 15, ITEM_MOVE_FLAG_01_COLLECT);/*星点を出す*/
			//
				player_add_score(data->boss_base.score);
			//
				player_set_destoroy_boss();
				explosion_add_circle(s, 1); 	/* player_set_destoroy_boss();B02_BOSS_DESTROY が必要 */
				s->type 	= SP_DELETE;
				mahoujin_live	= OFF_00_MAHOUJIN;
			}
		}
	}
//
	#if 0/*十六夜本人デバッグ用*/
	/* パネルのスコア欄にmove_typeを、グレイズ欄にstate1を表示させる。っていうか書き換えちゃう。 */
	((PLAYER_DATA *)player->data)->score		= data->move_type;
	((PLAYER_DATA *)player->data)->graze_point	= data->state1;
	#endif
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_boss_sakuya(STAGE_DATA *l)/*int lv*/
{
	mahoujin_live	= ON_01_MAHOUJIN;
	mahoujin_num	= 0;
//
	dummy							= sprite_add_res(BASE_BOSS_SAKUYA_PNG);
	dummy->flags					&= (~(SP_FLAG_VISIBLE));
	dummy->anim_frame				= SAKUYA_ANIME_04_CENTER_A;
	dummy->type 					= SP_MUTEKI;
//	dummy->callback_mover			= dummy_obj_move;
	dummy->x256 					= 0;
	dummy->y256 					= 0;

//
	SPRITE *s;
//	s								= spr ite_add_file 0("boss/sakuya.png"/*"boss04.png"*/, 19, PRIORITY_03_ENEMY, 0); s->anim_speed=0;
	s								= sprite_add_res(BASE_BOSS_SAKUYA_PNG);
	s->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	s->anim_frame					= SAKUYA_ANIME_04_CENTER_A;
	s->type 						= SP_BOSS/*SP_BOSS05*/;
	s->callback_mover				= enemy_boss05_move;
	s->x256 						= t256(GAME_WIDTH/2)/*(t256(GAME_WIDTH/2))-((s->w128))*/;
	s->y256 						= -((s->h128+s->h128));
//
	BOSS05_DATA *data;
	data							= mmalloc(sizeof(BOSS05_DATA));
	s->data 						= data;
//	data->boss_base.boss_health 	= ((/*5*/SAKUYA_09_KEITAI*1024)-1); 	/*5119==((5*1024)-1)*/
	data->boss_base.boss_health 	= ((1024)-1);
	data->boss_base.boss_life		= ((/*5*/SAKUYA_09_KEITAI));		/*5119==((5*1024)-1)*/

	/*score(5000)+score(4000)*difficulty*/
	data->boss_base.score			= adjust_score_by_difficulty(score(5000000));	/* 500万 */
	data->move_type 				= SAKUYA_01_KEITAI/*0*/;
	data->state1					= SA00;
	data->wait1 					= 0;
	data->wait2_256 				= t256(0);
	data->wait3 					= 0;
//	data->aaa_type					= 0;
//	data->level 					= (l->user_y);	/*lv*/
	data->move_angle512 			= (0);
//	data->boss_base.boss_timer		= (20*64);		/*1000*/	// + 20 [秒] ???	/* 16.666[秒] 1000 ???*/
	data->boss_base.boss_timer		= (40*64);	/* 40*64==40[count] 	約40[秒(64/60)](単位は秒ではない) */
	// 60 [秒] ???

	#if 1
//------------ 弾幕関連
	data->boss_base.danmaku_type				= 0;
	data->boss_base.danmaku_time_out			= 0;
	#endif
//
	((PLAYER_DATA *)player->data)->boss 	= s;
//	((PLAYER_DATA *)player->data)->bo ssmode=B00_NONE/*B01_BA TTLE*/;
	/* コールバック登録 */
	((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss05_hitbyweapon_dummy;	/* ダミーコールバック登録 */
}
