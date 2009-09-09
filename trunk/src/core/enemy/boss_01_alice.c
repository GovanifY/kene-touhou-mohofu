
#include "bullet_object.h"

/*---------------------------------------------------------
	アリス
	-------------------------------------------------------
	ToDo:
	ボスタイマー対応中
---------------------------------------------------------*/

typedef struct /*_boss01_data*/
{
	BOSS_BASE boss_base;

//------------ 移動関連
	int state8; 		/*	*/
	int state1; 		/* 形態 */
	int move_type;		/*	移動タイプ */
	int vx; 		/*	*/
	int vy; 		/*	*/
} BOSS01_DATA;

typedef struct /*_boss01_data*/
{
	ENEMY_BASE base;
//
	SPRITE *sakuya_obj; 		/* 継承 */
	int bwait;			/* 弾発射までの待ち時間。 */
	int br_angle512;	/* 弾の角度 */
	int identity_bit;	/* 固有ビット番号 */
//
	int fix_angle512;	/* てすと固定値 */
	int state222; 		/* 形態 */
} DOLL_DATA;


static int b01level;
static unsigned int alice_anime_count;

//----[ZAKO]

static unsigned int b01_live_flags;
static int bb_angle512;

static SPRITE *obj_doll[8]; 	/* 人形達 */

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

---------------------------------------------------------*/


enum
{
/*0*/	ALICE_01_KEITAI = 0,	/* 第一形態: アリス登場 */
/*0*/	ALICE_02_KEITAI,		/* 第二形態: 全員で攻撃 */
/*1*/	ALICE_03_KEITAI,		/* 第三形態: フォーメション2 */
/*2*/	ALICE_04_KEITAI,		/* 第四形態: アリス単体 */
};
enum
{
	ST_00 = 0,
	ST_01,
	ST_02,
	ST_03,
	ST_04,
	ST_05,
	ST_06,
	ST_07,
	ST_08,
	ST_09,
	ST_10,
	ST_11,
	ST_12,
	ST_13,
};

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_doll(SPRITE *s)
{
	explosion_add_type(s->x256+((s->w128)),s->y256+((s->h128)),/*0,*/EXPLOSION_FIRE08);
//
	item_create_for_boss(s, ITEM_CREATE_MODE_02);
//
	DOLL_DATA *data = (DOLL_DATA *)s->data;
	/* 普段の処理を速く(R4000系が得意な0比較を多用する)して、ここは(滅多に来ないので)遅くします */
	b01_live_flags &= (~(data->identity_bit));
}
/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
static int cc512;
static void move_doll01(SPRITE *s)
{
	DOLL_DATA *data = (DOLL_DATA *)s->data;
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
		/*CCW*/
			/*obj_doll[jj]*/s->x256 = data->sakuya_obj->x256 + ((sin512((aa_angle512))*radius));
			/*obj_doll[jj]*/s->y256 = data->sakuya_obj->y256 + ((cos512((aa_angle512))*radius));
		}
	}
	if (ST_03< ((BOSS01_DATA *)data->sakuya_obj->data)->state1 )
	{
		if (0==(ra_nd()&(0xff)))
		{
			/* ショット */
		//	b01_fire_flags &= (~(doll_data->identity_bit));/* off */
			bullet_create_n_way_dan_sa_type(s,
				(t256(1.5)+(difficulty<<6)),
				ANGLE_JIKI_NERAI_DAN,
				(int)(512/64),
				BULLET_CAP16_04_SIROI,
				(4+difficulty)/*(4-difficulty)*/);
		}
	}
	#endif
	#if 1
	/* アリスを倒すと皆破壊される。 */
	if (0==b01_live_flags)
	{
		s->type = SP_DELETE;
	}
	#endif
}
/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_doll02(SPRITE *s)
{
	cc512++;
	DOLL_DATA *data = (DOLL_DATA *)s->data;
	switch (data->state222)
	{
	case 0:
		/*obj_doll[jj]*/s->x256 = data->sakuya_obj->x256 + (((data->fix_angle512))<<7);
		/*obj_doll[jj]*/s->y256 = data->sakuya_obj->y256 - t256(16);
		if (0==(b01_live_flags & FLG_MINI_DOLL))
		{
			data->state222++;
		}
		break;
	case 1:
		/* 下過ぎ(下過ぎると上が空きすぎるし、下に近すぎる) */
		if (t256(28)/*offset*/ < s->y256)
		{
			s->y256 -= t256(1.414)/**fps_fa ctor*/;
		}
		/* 適正範囲内(t256(25)ぐらい)、次へ */
		if (t256(22)/*offset*/ < s->y256)
		{
			data->state222++;
		}
		/* 上過ぎ(上過ぎると弾が落ちてこない) */
		else
		{
			s->y256 += t256(1.414)/**fps_fa ctor*/;
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
			bullet_create_jyuryoku_dan(
				s,
				256,//t256(1+b01level)/*(3+b01level)*/ /*(4+doll_data->level)*/,
//				(data->br_angle512&127)+256+64,// /*deg512_2rad*/( (doll_data->br_angle512&255/*%180*/)+deg_360_to_512(90) ),
				(data->br_angle512&127)-256-64,// /*deg512_2rad*/( (doll_data->br_angle512&255/*%180*/)+deg_360_to_512(90) ),
			//	((ra_nd()&0x03)+1)//t256(0.04)/*10*/
				((ra_nd()&0x03)+2)//t256(0.04)
			);
		}
		break;
	}
	#if 1
	/* アリスを倒すと皆破壊される。 */
	if (0==b01_live_flags)
	{
		s->type = SP_DELETE;
	}
	#endif
}



/*---------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------
	旧人員の配置メモ
	[1][0][2]
	[4][3][5]
	-------------------------------------------------------
	s = ボス本体 boss sprite
	t = プレイヤーの弾 player's weapon
---------------------------------------------------------*/

static void callback_enemy_boss01_hitbyweapon_dummy(SPRITE *s, SPRITE *t/*, int angle*/)
{	/* 完全に姿を現すまで攻撃は、すべて無効とする。 */
}

static void callback_enemy_boss01_hitbyweapon(SPRITE *s, SPRITE *t/*, int angle*/)
{
	explosion_add_type(t->x256,t->y256,/*0,*/EXPLOSION_MINI00);/*先に実行した方が速い*/
//
	BOSS01_DATA *data	= (BOSS01_DATA *)s->data;
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
	//parsys_add(NULL,100,0,s->x,s->y,30,0,0,50,PIXELATE,NULL);


/*---------------------------------------------------------

---------------------------------------------------------*/



/*---------------------------------------------------------

---------------------------------------------------------*/
static int vvv256;
static void alice_animation(SPRITE *s)
{
	vvv256++/*vvv256+=vvv256*/;
	if (vvv256> t256(1) )
	{	vvv256= t256(1);}
//
	if (alice_anime_count)	/* 攻撃アニメーション */
	{
		alice_anime_count--;
		if ((32)>alice_anime_count) 	{	s->anim_frame = 0x23;}
		else
		if ((40)>alice_anime_count) 	{	s->anim_frame = 0x22;}
		else
										{	s->anim_frame = 0x21;}
	}
	else	/* 移動アニメーション */
	{
		BOSS01_DATA *data=(BOSS01_DATA *)s->data;
		int aaa;
		aaa = (data->vx>0)?(0x00):(0x10);	/*左右*/
		if ((16)>vvv256)		{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else
		if ((200)>vvv256)		{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else
		if ((224)>vvv256)		{	aaa+=2;}	/*128.0==t256(0.5)*/
		else
		if ((240)>vvv256)		{	aaa+=1;}	/*179.2==t256(0.7)*/
	//	else						{	aaa+=0;}
		s->anim_frame = aaa;
	}
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_alice(SPRITE *s)
{
	static int my_wait;
	static int firewait1;
	static int firewait2;
	static int firewait3;
//
	my_wait -= (1)/*fps_fa ctor*/;
//
	BOSS01_DATA *data=(BOSS01_DATA *)s->data;
	switch (data->state1)
	{
	case ST_00:
	//	x256		= (GAME_WIDTH*256/2); // /*t256*/256*(GAME_WIDTH/2-(/*obj_doll[0]->w*/62/2+/*obj_doll[-1]->w*/60/2+/*obj_doll[1]->w*/53/2)/2);/*???*/
	//	x256		= ( 256*( (GAME_WIDTH)-(62/2)-(60/2)-(53/2) ) )/(2);/*???*/  /*obj_doll[0]->w*/ /*obj_doll[-1]->w*/ /*obj_doll[1]->w*/
		s->x256 	= t256( 146)+t256(62/2);
		s->y256 	= t256(-100);
		data->state1++/* = ST_01*/;
		firewait1	= 45;
		firewait2	= 4;
		firewait3	= 0;
		break;
	case ST_01:/*下へ移動*/
		s->y256 += t256(2)/**fps_fa ctor*/;
		if (s->y256 >= t256(40/*40*/) )
		{
			data->state1++/* = ST_02*/;
		//	((PLAYER_DATA *)player->data)->bo ssmode=B05_BE FORE_LOAD;
			((PLAYER_DATA *)player->data)->state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
		}
		break;
		/*姿を現す*/
	case ST_02:
	//	if (B00_NONE/*B01_BA TTLE*/==(((PLAYER_DATA *)player->data)->bo ssmode))
		if ( ((((PLAYER_DATA *)player->data)->state_flag) & STATE_FLAG_05_IS_BOSS) )
		{
			my_wait = 40;
			data->state1++/* = ST_03*/;
		}
		break;
	case ST_03: 	/*wait*/
		if (my_wait <= 0)
		{
			/* プレイヤー弾受け付け、コールバックを登録 */
			((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss01_hitbyweapon;
			data->state1++/* = ST_03*/;
		}
		break;
/*完全に姿を現す*/
	case ST_04: 	/* 移動中 */
		if (my_wait<=0)
		{	/* 移動方向を決める */
			data->state8++;
			data->state8 &= (8-1);
			{
				enum
				{
					PPP_00_VX = 0,			/* x ベクトル移動量 */
					PPP_01_VY,				/* y ベクトル移動量 */
					PPP_02_WAIT_DIV_2,		/* ウェイトカウンタの半分量 */
					PPP_03_IS_RESET_ANIME,	/* アニメーションリセット 0:しない 1:する 2:特別(弾幕撃ち) */
				};
				s8 ppp[8][4] =
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
				data->vx	= ppp[data->state8][PPP_00_VX];
				data->vy	= ppp[data->state8][PPP_01_VY];
				my_wait 	= ppp[data->state8][PPP_02_WAIT_DIV_2]; 	/* 50*4 60 移動量 */
				my_wait 	+= my_wait;
				if (0!=ppp[data->state8][PPP_03_IS_RESET_ANIME])
				{
					if (2==ppp[data->state8][PPP_03_IS_RESET_ANIME])	/* 攻撃アニメーション */
					{
						alice_anime_count = 48;
						data->boss_base.danmaku_test++;
						data->boss_base.danmaku_type		= data->boss_base.danmaku_test/*DANMAKU_01*/;	/* 禊弾幕をセット */
						data->boss_base.danmaku_time_out	= (DANMAKU_01_SET_TIME+DANMAKU_01_SET_TIME);	/* 禊弾幕の発生時間 x 2 */
					}
					vvv256=1;
				}
			}
		}
		break;
	}
	/* alice move */
	{
		s->x256 += (data->vx)*vvv256;
			 if (s->x256< t256( 		0)+t256(24) )	{	s->x256=t256(		  0)+t256(24);}
		else if (s->x256> t256(GAME_WIDTH)-t256(24) )	{	s->x256=t256(GAME_WIDTH)-t256(24);}
	//
		s->y256 += (data->vy)*vvv256;
			 if (s->y256 < t256(0)	)	{	s->y256 = t256(0);	}
		else if (s->y256 > t256(96) )	{	s->y256 = t256(96); }
	}
	alice_animation(s);
//	move_all_doll(s);
//
	danmaku_generator(s);	/* 弾幕生成 */
//
//
	/*---------------------------------------------------------
		ダメージチェック
	---------------------------------------------------------*/
//
	{
	}
//
	{
	//	if (0 == data->boss_base.boss_life)
		{
		//	if (FLG_ALL_CAST==(b01_live_flags&FLG_ALL_CAST))/*全員倒した？*/
			if (data->boss_base.boss_health < 1)/*全員倒した？*/
			{
				item_create_for_boss(s, ITEM_CREATE_MODE_01);
			//
				/* アリスを倒すと皆破壊される。 */
				#if 1
				b01_live_flags		= 0/*FLG_ALL_CAST*/;
				#endif
			//
				player_set_destoroy_boss();
				explosion_add_circle(s, 1); /* player_set_destoroy_boss(); B02_BOSS_DESTROY が必要 */
				s->type 	= SP_DELETE;
			}
		}
	}
}
	/* コールバック登録 */
//	((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon = callback_enemy_boss01_hitbyweapon_dummy; /* ダミーコールバック登録 */

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_boss_alice(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	b01level			= lv+1;
	alice_anime_count	= 0;
//

//----[ZAKO]
//	bb_angle512 	= 128-43/*CCW*/ /*CW 43*/;/*512/12==42.66*/
	bb_angle512 	= 128/*CCW*/ /*CW 43*/;/*512/12==42.66*/

	b01_live_flags	= (FLG_ALL_CAST);
//
//----[BOSS]

	SPRITE *sakuya;
	sakuya							= sprite_add_res(BASE_BOSS_ALICE_PNG); /*"boss01-mo.png"60x42, offset*/
	sakuya->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	sakuya->anim_frame				= 0;
	sakuya->type 					= SP_BOSS/*SP_BOSS01*/;
	sakuya->callback_mover			= move_alice;

	{
		BOSS01_DATA *data;
		data								= mmalloc(sizeof(BOSS01_DATA));
		sakuya->data 						= data;
		//
		data->boss_base.boss_health 		= (1023);	// アリス本体のHP。もう少し高くてもいいかも。// [***090305	変更
		data->boss_base.boss_life			= (0);
		data->boss_base.score				= adjust_score_by_difficulty(score( 100000));	/*	10万 x 1人 (計100万==(1人x10万)+(6人x5万)+(2人x30万)) */
//		data->boss_base.score				= score(1000)*(difficulty+1);
		data->state1						= ST_00;
		data->state8						= 0;
		data->move_type 					= ALICE_01_KEITAI;
		data->vx = ( 0);
		data->vy = ( 0);

//		data->boss_base.boss_timer			= 40*64;		/* 40*64==40[count] 	約40[秒(64/60)](単位は秒ではない) */
		data->boss_base.boss_timer			= (99-24)*64;	/* 75*64==75[count] 	約75[秒(64/60)](単位は秒ではない) */
		//
		#if 1
//------------ 弾幕関連
		data->boss_base.danmaku_type		= 0;
		data->boss_base.danmaku_time_out	= 0;
		data->boss_base.danmaku_test		= (DANMAKU_08-1)/*0*/;
		#endif
//
		((PLAYER_DATA *)player->data)->boss = sakuya;
		/* コールバック登録 */
		((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon = callback_enemy_boss01_hitbyweapon_dummy; /* ダミーコールバック登録 */
	}

//----[ZAKO]
	int i;
	/* 85.33==512/6 170.66== 512/3 */	/*171*/
	#define AA_OFS85 (85)
	int jj_angle512;
	jj_angle512= -(int)(AA_OFS85/2);	/*0*/
	for (i=0; i<8; i++)
	{
		obj_doll[i] 						= sprite_add_res(BASE_BOSS_ALICE_DOLLS_PNG/*doll_res_num[i]*/);
//		obj_doll[i]->anim_speed 			= 0;
		obj_doll[i]->type					= SP_ZAKO/*SP_BOSS01*/;
		obj_doll[i]->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);

		obj_doll[i]->callback_loser 		= lose_doll;

		DOLL_DATA *data;
		data								= mmalloc(sizeof(DOLL_DATA));
		data->sakuya_obj					= sakuya;
		obj_doll[i]->data					= data;
		data->identity_bit					= (FLG_DOLL0<<i);		/* 固有ビット番号 */
//		data->health_flag					= 0;
		data->bwait 						= 0;
	//	br1_angle512						= deg_360_to_512(0);
	//	br2_angle512						= deg_360_to_512(0);
//		data->br_angle512					= deg_360_to_512(0);/*CW*/
		data->br_angle512					= (0);/*CCW*/
		data->base.health 					= (1023)/*(200)*/ /*((2==i)?(b01_04[difficulty]):(b01_health[i]))*/;
	//	data->base.score					= score( 500)*(difficulty+1);
		if (1<i)
		{
			obj_doll[i]->callback_mover 	= move_doll01;
			obj_doll[i]->anim_frame 		= 0x20+((i&1)<<4);		/* ミニ人形 / 紅い娘の人形 */
			data->base.score				= adjust_score_by_difficulty(score(  50000));	/*	 5万 x 6人 */
		}
		else
		{
			obj_doll[i]->callback_mover 	= move_doll02;
			obj_doll[i]->anim_frame 		= (i<<4);				/*  0:藍い娘の人形(旧左上) / 1:碧の娘の人形(旧右上) */
			data->base.score				= adjust_score_by_difficulty(score( 300000));	/*	30万 x 2人 */
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


//	((PLAYER_DATA *)player->data)->bo ssmode = B00_NONE/*B01_BA TTLE*/;
