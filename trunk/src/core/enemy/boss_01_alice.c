
#include "bullet_object.h"

/*---------------------------------------------------------
	アリス・マーガトロイド
	Alice Margatroid.
	-------------------------------------------------------
	ToDo:
	ボスタイマー対応中
---------------------------------------------------------*/

/*
C++みたいに 継承 させてみる。
が、よく考えたら現状globalのplayerにあるので
要らなかったのでOFF。でもあとでこの考えは使うかも。
 */
#define USE_KEISYOU 0

typedef struct /*_boss01_data*/
{
	BOSS_BASE boss_base;
//------------ 移動関連
	int state1; 	/* 形態ステート */	/* 弾幕ステート */
	int vx; 		/*	*/
	int vy; 		/*	*/
} BOSS01_DATA;

typedef struct /*_boss01_data*/
{
//	ENEMY_BASE base;
//
	#if (1==USE_KEISYOU)
	SPRITE *sakuya_obj; /* 継承 */
	#endif
	int identity_bit;	/* 固有ビット番号 */
	int fix_angle512;	/* てすと固定値 */
//------------ doll02専用
	int state222;		/* 形態 */
	int br_angle512;	/* 弾の角度 */
	int bwait;			/* 弾発射までの待ち時間。 */
} DOLL_DATA;

static unsigned int alice_anime_count;

//----[ZAKO]

static int bb_angle512;

// FLG_FIRE1 弾フラグ。紅い娘の人形が壊れていて、かつ指定位置に着いた時。onになる。
#define FLG_FIRE1 0x01/* 藍い娘の人形(旧左上)が指定位置に付いたか */
#define FLG_FIRE2 0x02/* 碧の娘の人形(旧右上)が指定位置に付いたか */
//
#define FLG_DOLL0 0x01/* 藍い娘の人形(旧左上) */
#define FLG_DOLL1 0x02/* 碧の娘の人形(旧右上) */
//
#define FLG_DOLL2 0x04/* 紅い娘の人形(人形) */	/*key*/
#define FLG_DOLL3 0x08/* 旧左前の人形 */
//
#define FLG_DOLL4 0x10/* 旧右前の人形 */
#define FLG_DOLL5 0x20/* 旧右前の人形 */
#define FLG_DOLL6 0x40/* 旧右前の人形 */
#define FLG_DOLL7 0x80/* 旧右前の人形 */

#define FLG_MINI_DOLL	(FLG_DOLL2|FLG_DOLL3|FLG_DOLL4|FLG_DOLL5|FLG_DOLL6|FLG_DOLL7)
#define FLG_FIRE_DOLL	(FLG_DOLL0|FLG_DOLL1)
#define FLG_ALL_CAST	(FLG_FIRE_DOLL|FLG_MINI_DOLL)/*全員*/


/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_doll(SPRITE *src)
{
	send1_obj->x256 = src->x256+((src->w128));
	send1_obj->y256 = src->y256+((src->h128));
	bakuhatsu_add_type_ddd(send1_obj/*src->x256+((src->w128)),src->y256+((src->h128))*/,/*0,*/BAKUHATSU_FIRE08);
//
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
//
	DOLL_DATA *data = (DOLL_DATA *)src->data;
	/* 普段の処理を速く(R4000系が得意な0比較を多用する)して、ここは(滅多に来ないので)遅くします */
	common_boss_flags &= (~(data->identity_bit));
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static int cc512;
static void move_doll01(SPRITE *src)
{
	DOLL_DATA *data = (DOLL_DATA *)src->data;
	#if 1
	{
		bb_angle512++;
	int aa_angle512;
		aa_angle512 = bb_angle512;
	//int jj;
		/* 紅い娘の人形(人形) 旧左前の人形 旧右前の人形 */
	//	for (jj=2; jj<8; jj++)/*2 3 4 5 6 7*/
		{
//			aa_angle512 += (85)/*171*/;/* 85.33==512/6 170.66== 512/3 */
			aa_angle512 += data->fix_angle512;
			mask512(aa_angle512);
			int radius;
		//	radius = (90)/*45*/;
			{
				radius = ((sin512(cc512))>>2)+16;	/* 80==16+64 */
			}
		#if (1==USE_KEISYOU)
		/*CCW*/
			src->x256 = data->sakuya_obj->x256 + ((sin512((aa_angle512))*radius));
			src->y256 = data->sakuya_obj->y256 + ((cos512((aa_angle512))*radius));
		#else
		/*CCW*/
			src->x256 = (((PLAYER_DATA *)player->data)->boss)->x256 + ((sin512((aa_angle512))*radius));
			src->y256 = (((PLAYER_DATA *)player->data)->boss)->y256 + ((cos512((aa_angle512))*radius));
		#endif
		}
	}
	if (SPELL_CARD_00_alice_000 < spell_card_number)
	{
		if (0==(ra_nd()&(0xff)))
		{
			/* ショット */
		//	b01_fire_flags &= (~(doll_data->identity_bit));/* off */
			send1_obj->x256 = src->x256;
			send1_obj->y256 = src->y256;
			#if 1
			/* あとで要る */
	//		send1_obj->h128 = src->h128;
	//		send1_obj->w128 = src->w128;
			#endif
		//	bullet_create_n_way_dan_sa_type(src,
			send1_obj->BULLET_REGIST_speed256			=	(t256(1.5)+(difficulty<<6));
			send1_obj->BULLET_REGIST_angle512			=	ANGLE_JIKI_NERAI_DAN;
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/64);
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_CAP16_04_SIROI;
			send1_obj->BULLET_REGIST_n_way				=	(4+difficulty)/*(4-difficulty)*/ ;
			bullet_regist_basic();
		}
	}
	#endif
	#if 1
	/* アリスを倒すと皆破壊される。 */
	if (0==common_boss_flags)
	{
		src->type = SP_DELETE;
	}
	#endif
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_doll02(SPRITE *src)
{
	cc512++;
	DOLL_DATA *data = (DOLL_DATA *)src->data;
	switch (data->state222)
	{
	case 0:
		#if (1==USE_KEISYOU)
		src->x256 = data->sakuya_obj->x256 + (((data->fix_angle512))<<7);
		src->y256 = data->sakuya_obj->y256 - t256(16);
		#else
		src->x256 = (((PLAYER_DATA *)player->data)->boss)->x256 + (((data->fix_angle512))<<7);
		src->y256 = (((PLAYER_DATA *)player->data)->boss)->y256 - t256(16);
		#endif
		if (0==(common_boss_flags & FLG_MINI_DOLL))
		{
			data->state222++;
		}
		break;
	case 1:
		/* 下過ぎ(下過ぎると上が空きすぎるし、下に近すぎる) */
		if (t256(28)/*offset*/ < src->y256)
		{
			src->y256 -= t256(1.414)/**fps_fa ctor*/;
		}
		/* 適正範囲内(t256(25)ぐらい)、次へ */
		if (t256(22)/*offset*/ < src->y256)
		{
			data->state222++;
		}
		/* 上過ぎ(上過ぎると弾が落ちてこない) */
		else
		{
			src->y256 += t256(1.414)/**fps_fa ctor*/;
		}
		break;
	case 2:
		{	data->bwait--;}
		if (data->bwait <= 0)
		{
			data->bwait = 5+(3-difficulty)*5;
//			data->br_angle512 += deg_360_to_512(10);
//			data->br_angle512 -= deg_360_to_512CCW(10);
//			data->br_angle512 += 8;
			data->br_angle512 -= 8;
			bullet_create_jyuryoku_dan000(
				src,
				t256(1.0),//t256(1+difficulty)/*(3+difficulty)*/ /*(4+difficulty)*/,
//				(data->br_angle512&127)+256+64,// /*deg512_2rad*/( (doll_data->br_angle512&255/*%180*/)+deg_360_to_512(90) ),
				(data->br_angle512&127)-256-64,// /*deg512_2rad*/( (doll_data->br_angle512&255/*%180*/)+deg_360_to_512(90) ),
			//	((ra_nd()&0x03)+1)//t256(0.04)/*10*/
				((ra_nd()&0x03)+2),//t256(0.04)
				(BULLET_MARU8_00_AKA+(7))	/* 弾グラ */
			);
		}
		break;
	}
	#if 1
	/* アリスを倒すと皆破壊される。 */
	if (0==common_boss_flags)
	{
		src->type = SP_DELETE;
	}
	#endif
}

/*---------------------------------------------------------
	旧人員の配置メモ
	[1][0][2]
	[4][3][5]
---------------------------------------------------------*/

/*---------------------------------------------------------

---------------------------------------------------------*/

static int vvv256;
static void alice_animation(SPRITE *src)
{
	vvv256++/*vvv256+=vvv256*/;
	if (vvv256> t256(1.0) )
	{	vvv256= t256(1.0);}
//
	if (alice_anime_count)	/* 攻撃アニメーション */
	{
		alice_anime_count--;
		if ((32)>alice_anime_count) 	{	src->type = BOSS_09_BOSS32;/*src->an im_frame = 0x23;*/}
		else
		if ((40)>alice_anime_count) 	{	src->type = BOSS_10_BOSS33;/*src->an im_frame = 0x22;*/}
		else
										{	src->type = BOSS_09_BOSS32;/*src->an im_frame = 0x21;*/}
	}
	else	/* 移動アニメーション */
	{
	//	BOSS01_DATA *data=(BOSS01_DATA *)src->data;
		int aaa;
		aaa = (src->vx256>0)?(BOSS_00_BOSS11/*0x00*/):(BOSS_04_BOSS21/*0x10*/); /*左右*/
		if ((16)>vvv256)		{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else
		if ((200)>vvv256)		{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else
		if ((224)>vvv256)		{	aaa+=2;}	/*128.0==t256(0.5)*/
		else
		if ((240)>vvv256)		{	aaa+=1;}	/*179.2==t256(0.7)*/
	//	else					{	aaa+=0;}
		/*src->an im_frame*/src->type = aaa;
	}
}


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
extern const u8 alice_danmaku_table[16];

//		firewait1	= 45;
//		firewait2	= 4;
//		firewait3	= 0;
//		break;
//	case ST_01:
//	static int firewait1;
//	static int firewait2;
//	static int firewait3;
//
/*static*/ void alice_01_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait -= (1)/*fps_fa ctor*/;
//
	BOSS01_DATA *data = (BOSS01_DATA *)src->data;
	{
		if (my_wait <= 0)
		{	/* 移動方向を決める */
			data->state1++;
			data->state1 &= (8-1);
			{
				enum
				{
					PPP_00_VX = 0,			/* x ベクトル移動量 */
					PPP_01_VY,				/* y ベクトル移動量 */
					PPP_02_WAIT_DIV_2,		/* ウェイトカウンタの半分量 */
					PPP_03_IS_RESET_ANIME,	/* アニメーションリセット 0:しない 1:する 2:特別(弾幕撃ち) */
				};
				const s8 ppp[8][4] =
				{
					{( 2),(-1),(100),( 1),},	/*右上へ*/
					{( 0),( 0),( 50),( 2),},	/*wait*/
					{( 2),( 1),(100),( 1),},	/*右下へ*/
					{( 0),( 0),( 10),( 0),},	/*wait*/
					{(-2),(-1),(100),( 1),},	/*左上へ*/
					{( 0),( 0),( 50),( 2),},	/*wait*/
					{(-2),( 1),(100),( 1),},	/*左下へ*/
					{( 0),( 0),( 10),( 0),},	/*wait*/
				};
				data->vx	= ppp[data->state1][PPP_00_VX];
				data->vy	= ppp[data->state1][PPP_01_VY];
				my_wait 	= ppp[data->state1][PPP_02_WAIT_DIV_2]; 	/* 50*4 60 移動量 */
				my_wait 	+= my_wait;
				if (0!=ppp[data->state1][PPP_03_IS_RESET_ANIME])
				{
					if (2==ppp[data->state1][PPP_03_IS_RESET_ANIME])	/* 攻撃アニメーション */
					{
						alice_anime_count = 48;
						data->boss_base.danmaku_test++;
						data->boss_base.danmaku_test &= 0x07;
						data->boss_base.danmaku_type		= alice_danmaku_table[data->boss_base.danmaku_test];	/*DANMAKU_01*/	/* 禊弾幕をセット */
						data->boss_base.danmaku_time_out	= (DANMAKU_01_SET_TIME+DANMAKU_01_SET_TIME);	/* 禊弾幕の発生時間 x 2 */
					}
					vvv256=1;
				}
			}
		}
	}
	src->vx256 = (data->vx)*vvv256;
	src->vy256 = (data->vy)*vvv256;
}



/*---------------------------------------------------------
	ボス行動
---------------------------------------------------------*/

static void move_alice(SPRITE *src)
{
	/* スペカ登録 */
			if (0/*off*/==spell_card_mode)
			{
				regist_spell_card222(src);
			}
	spell_card_generator222(src);	/* スペカ生成 */
	#if 1/* [スペカシステム内に移動予定] */
	/*---------------------------------------------------------
		パチェ移動処理
	---------------------------------------------------------*/
	//
	//	enemy_boss04_setpos(src, xxx,yyy);
	//
	boss_move_clip_rect(src);
	#endif
//
	/*---------------------------------------------------------
		アリス移動処理
	---------------------------------------------------------*/
	alice_animation(src);
//	boss_move_clip_rect(src);
//	move_all_doll(src);
//
	boss_effect(src);			/* 回エフェクト */
	danmaku_generator(src); 	/* 弾幕生成 */
//
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
//		sakuya->base_score			= score(1000)*(difficulty+1);

void add_boss_alice(STAGE_DATA *l)/*int lv*/
{
	boss_bgm_mode		= (l->user_y);
//
	alice_anime_count	= 0;
//

//----[ZAKO]
//	bb_angle512 		= 128-43/*CCW*/ /*CW 43*/;/*512/12==42.66*/
	bb_angle512 		= 128/*CCW*/ /*CW 43*/;/*512/12==42.66*/

	common_boss_flags	= (FLG_ALL_CAST);

//----[BOSS]
		SPRITE *sakuya;
	//	sakuya								= sprite_add_res(BASE_BOSS_ALICE_PNG); /*"boss01-mo.png"60x42, offset*/
		sakuya								= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		sakuya->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//		sakuya->an im_frame 				= 0;
		sakuya->type						= BOSS_00_BOSS11;	/*SP_BOSS*/ 	/*SP_BOSS01*/
		sakuya->callback_mover				= move_alice;
		sakuya->callback_loser				= common_boss_put_items;
	//	sakuya->callback_loser				= lose_boss/*common_boss_put_items*/;/* スペカ無い */
//
		sakuya->base_health 				= (1023);	// アリス本体のHP。もう少し高くてもいいかも。// [***090305	変更
		sakuya->base_score					= adjust_score_by_difficulty(score( 100000));	/*	10万 x 1人 (計100万==(1人x10万)+(6人x5万)+(2人x30万)) */
	//------------ スペカ関連
		spell_card_number					= SPELL_CARD_00_alice_000;
		spell_card_max						= SPELL_CARD_19_alice_jjj;
		spell_card_boss_init_regist(sakuya);
	{
		BOSS01_DATA *data;
		data								= mmalloc(sizeof(BOSS01_DATA));
		sakuya->data						= data;
		//
		data->state1						= 0/*ST_00*/;
		data->vx = ( 0);
		data->vy = ( 0);
		//
		#if 1
	//------------ 弾幕関連
		data->boss_base.danmaku_type		= 0;
		data->boss_base.danmaku_time_out	= 0;
		data->boss_base.danmaku_test		= 0;	/*(DANMAKU_08_rumia-1)*/ /*0*/
		#endif
	}

//----[ZAKO]
	/* 85.33==512/6 170.66== 512/3 */	/*171*/
	#define AA_OFS85 (85)
	int jj_angle512;
	jj_angle512= -(int)(AA_OFS85/2);	/*0*/

	int i;
	for (i=0; i<8; i++)
	{
		/*static*/ SPRITE *obj_doll;	/* 人形達 */
	//	obj_doll						= sprite_add_res(BASE_BOSS_ALICE_DOLLS_PNG/*doll_res_num[i]*/);
		obj_doll						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
//		obj_doll->anim_speed			= 0;
//		obj_doll->type					= (BOSS_16_YOUSEI11+(i<<2));		/*SP_ZAKO*/ 	/*SP_BOSS01*/
		obj_doll->flags 				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//
		obj_doll->callback_loser		= lose_doll;
		obj_doll->callback_hit_enemy	= callback_hit_zako;
//
		DOLL_DATA *data;
		data								= mmalloc(sizeof(DOLL_DATA));
		#if (1==USE_KEISYOU)
		data->sakuya_obj					= sakuya;	/* 継承させる */
		#endif
		obj_doll->data					= data;
		data->identity_bit					= (FLG_DOLL0<<i);		/* 固有ビット番号 */
//		data->health_flag					= 0;
		data->bwait 						= 0;
	//	br1_angle512						= deg_360_to_512(0);
	//	br2_angle512						= deg_360_to_512(0);
//		data->br_angle512					= deg_360_to_512(0);/*CW*/
		data->br_angle512					= (0);/*CCW*/
		obj_doll->base_health			= (1024-1)/*(200)*/ /*((2==i)?(b01_04[difficulty]):(b01_health[i]))*/;
	//	obj_doll->base_score			= score( 500)*(difficulty+1);
		if (1<i)
		{
			obj_doll->callback_mover	= move_doll01;
			obj_doll->type/*an im_frame*/		= /*0x20*/8+BOSS_16_YOUSEI11+((i&1)<<2/*4*/);		/* ミニ人形 / 紅い娘の人形 */
			obj_doll->base_score		= adjust_score_by_difficulty(score(  50000));	/*	 5万 x 6人 */
		}
		else
		{
			obj_doll->callback_mover	= move_doll02;
			obj_doll->type/*an im_frame*/		= BOSS_16_YOUSEI11+(i<<2/*4*/); 			/*	0:藍い娘の人形(旧左上) / 1:碧の娘の人形(旧右上) */
			obj_doll->base_score		= adjust_score_by_difficulty(score( 300000));	/*	30万 x 2人 */
		}
		data->fix_angle512					= jj_angle512;
		jj_angle512 += (AA_OFS85);
		data->state222						= 0;
	}

}

//新スコア配分: 	藍い娘と碧の娘はなるべく倒そう。倍以上スコアが違うよ。
//		data->boss_base.score				= adjust_score_by_difficulty(score( 100000));	/*	10万 x 1人 (計100万==(1人x10万)+(6人x5万)+(2人x30万)) */
//		data->base.score					= adjust_score_by_difficulty(score( 300000));	/*	30万 x 2人 */
//		data->base.score					= adjust_score_by_difficulty(score(  50000));	/*	 5万 x 6人 */

//旧スコア配分
//		data->boss_base.score				= adjust_score_by_difficulty(score( 500000));	/*	50万 x 1人 (計100万==(1人x50万)+(5人x10万)) */
//		data->base.score					= adjust_score_by_difficulty(score( 100000));	/*	10万 x 5人 */
