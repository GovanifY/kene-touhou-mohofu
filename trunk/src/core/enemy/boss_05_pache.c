
#include "bullet_object.h"

/*---------------------------------------------------------
	パチュリー・ノーレッジ
	パチェ(予定)	(r27から新規追加)
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
	int state1; 		/* 形態ステート */
	int state8; 		/* 弾幕ステート */
//	int move_type;		/* 移動タイプ */
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


//static int b01level;
static unsigned int alice_anime_count;

//----[ZAKO]

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

#define FLG_MINI_DOLL	(FLG_DOLL4|FLG_DOLL5|FLG_DOLL6|FLG_DOLL7)
#define FLG_FIRE_DOLL	(FLG_DOLL0|FLG_DOLL1|FLG_DOLL2|FLG_DOLL3)
#define FLG_ALL_CAST	(FLG_FIRE_DOLL|FLG_MINI_DOLL)/*全員*/

/*---------------------------------------------------------

---------------------------------------------------------*/

enum
{
/*0*/	ALICE_01_KEITAI = 0,	/* 第一形態: パチェ登場 */
/*0*/	ALICE_02_KEITAI,		/* 第二形態: 全員で攻撃 */
/*1*/	ALICE_03_KEITAI,		/* 第三形態: フォーメション2 */
/*2*/	ALICE_04_KEITAI,		/* 第四形態: パチェ単体 */
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

static void lose_doll(SPRITE *src)
{
	dummy_obj->x256 = src->x256+((src->w128));
	dummy_obj->y256 = src->y256+((src->h128));
	bakuhatsu_add_type_ddd(dummy_obj/*src->x256+((src->w128)),src->y256+((src->h128))*/,/*0,*/BAKUHATSU_FIRE08);
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
			/*obj_doll[jj]*/src->x256 = data->sakuya_obj->x256 + ((sin512((aa_angle512))*radius));
			/*obj_doll[jj]*/src->y256 = data->sakuya_obj->y256 + ((cos512((aa_angle512))*radius));
		#else
		/*CCW*/
			/*obj_doll[jj]*/src->x256 = (((PLAYER_DATA *)player->data)->boss)->x256 + ((sin512((aa_angle512))*radius));
			/*obj_doll[jj]*/src->y256 = (((PLAYER_DATA *)player->data)->boss)->y256 + ((cos512((aa_angle512))*radius));
		#endif
		}
	}
	#if (1==USE_KEISYOU)
	if (ST_03 < ((BOSS01_DATA *)data->sakuya_obj->data)->state1 )
	#else
	if (ST_03 < ((BOSS01_DATA *)(((PLAYER_DATA *)player->data)->boss)->data)->state1 )
	#endif
	{
		if (0==(ra_nd()&(0xff)))
		{
			/* ショット */
		//	b05_fire_flags &= (~(doll_data->identity_bit));/* off */
			bullet_create_n_way_dan_sa_type(src,
				(t256(1.5)+(difficulty<<6)),
				ANGLE_JIKI_NERAI_DAN,
				(int)(512/64),
				BULLET_CAP16_04_SIROI,
				(4+difficulty)/*(4-difficulty)*/);
		}
	}
	#endif
	#if 1
	/* パチェを倒すと皆破壊される。 */
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
		/*obj_doll[jj]*/src->x256 = data->sakuya_obj->x256 + (((data->fix_angle512))<<7);
		/*obj_doll[jj]*/src->y256 = data->sakuya_obj->y256 - t256(16);
		#else
		/*obj_doll[jj]*/src->x256 = (((PLAYER_DATA *)player->data)->boss)->x256 + (((data->fix_angle512))<<7);
		/*obj_doll[jj]*/src->y256 = (((PLAYER_DATA *)player->data)->boss)->y256 - t256(16);
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
				256,//t256(1+b01level)/*(3+b01level)*/ /*(4+doll_data->level)*/,
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
	/* パチェを倒すと皆破壊される。 */
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
	if (vvv256> t256(1) )
	{	vvv256= t256(1);}
//
	if (alice_anime_count)	/* 攻撃アニメーション */
	{
		alice_anime_count--;
		if ((32)>alice_anime_count) 	{	src->anim_frame = 0x23;}
		else
		if ((40)>alice_anime_count) 	{	src->anim_frame = 0x22;}
		else
										{	src->anim_frame = 0x21;}
	}
	else	/* 移動アニメーション */
	{
		BOSS01_DATA *data=(BOSS01_DATA *)src->data;
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
		src->anim_frame = aaa;
	}
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_alice(SPRITE *src)
{
	static int my_wait;
//	static int firewait1;
//	static int firewait2;
//	static int firewait3;
//
	my_wait -= (1)/*fps_fa ctor*/;
//
	BOSS01_DATA *data = (BOSS01_DATA *)src->data;
	switch (data->state1)
	{
	case ST_00:
	//	x256		= (GAME_WIDTH*256/2); // /*t256*/256*(GAME_WIDTH/2-(/*obj_doll[0]->w*/62/2+/*obj_doll[-1]->w*/60/2+/*obj_doll[1]->w*/53/2)/2);/*???*/
	//	x256		= ( 256*( (GAME_WIDTH)-(62/2)-(60/2)-(53/2) ) )/(2);/*???*/  /*obj_doll[0]->w*/ /*obj_doll[-1]->w*/ /*obj_doll[1]->w*/
		src->x256	= t256( 146)+t256(62/2);
		src->y256	= t256(-100);
		data->state1++/* = ST_01*/;
//		firewait1	= 45;
//		firewait2	= 4;
//		firewait3	= 0;
		break;
	case ST_01:/*下へ移動*/
		src->y256 += t256(2)/**fps_fa ctor*/;
		if (src->y256 >= t256(40/*40*/) )
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
			src->callback_hit_enemy = callback_hit_boss; 	/* コールバック登録 */
			data->state1++/* = ST_04*/;
		}
		break;
/*完全に姿を現す*/
	case ST_04: 	/* 移動中 */
		if (my_wait<=0)
		{	/* 移動方向を決める */
			data->state8++;
			data->state8 &= (8-1);
			{
				{	u16 my_rand;
					my_rand = ra_nd();
					data->vx	= ((my_rand&0x02)-1)/*ppp[data->state8][PPP_00_VX]*/;
					data->vy	= (((my_rand>>8)&0x03)-2)/*ppp[data->state8][PPP_01_VY]*/;
						 if (src->x256< t256(		  0)+t256(48) ) 	{	data->vx =  (2);}
					else if (src->x256> t256(GAME_WIDTH)-t256(48) ) 	{	data->vx = -(2);}
				//	my_wait 	= ppp[data->state8]/*[PPP_02_WAIT_DIV_2]*/; 	/* 50*4 60 移動量 */
					my_wait 	= (my_rand & 0xff); 	/* 50*4 60 移動量 */	/*[PPP_02_WAIT_DIV_2]*/
				//	my_wait 	+= my_wait;
				//	if (0!=ppp[data->state8][PPP_03_IS_RESET_ANIME])
					{
					//	if (2==ppp[data->state8][PPP_03_IS_RESET_ANIME])	/* 攻撃アニメーション */
						if (0==(my_rand&0x04))
						{
							alice_anime_count = 48;
							data->boss_base.danmaku_test++;
							data->boss_base.danmaku_test &= 0x0f; 		/*test*/
							data->boss_base.danmaku_type		= data->boss_base.danmaku_test/*DANMAKU_01*/;	/* 禊弾幕をセット */
							data->boss_base.danmaku_time_out	= (DANMAKU_01_SET_TIME+DANMAKU_01_SET_TIME);	/* 禊弾幕の発生時間 x 2 */
						}
						vvv256=1;
					}
				}
			}
		}
		break;
	}
	/*---------------------------------------------------------
		パチェ移動処理
	---------------------------------------------------------*/
	src->vx256 = (data->vx)*vvv256;
	src->vy256 = (data->vy)*vvv256;
	alice_animation(src);
	boss_move96(src);
//	move_all_doll(src);
//
	danmaku_generator(src); 	/* 弾幕生成 */
//
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_boss_pache(STAGE_DATA *l)/*int lv*/
{
//	int lv;	lv	= l->user_y;
//	b01level			= lv+1;
	alice_anime_count	= 0;
//

//----[ZAKO]
//	bb_angle512 	= 128-43/*CCW*/ /*CW 43*/;/*512/12==42.66*/
	bb_angle512 	= 0;/*128*/ /*CCW*/ /*CW 43*/ /*512/12==42.66*/

	common_boss_flags	= (FLG_ALL_CAST);

//----[BOSS]
	SPRITE *sakuya;
	sakuya									= sprite_add_res(BASE_BOSS_PACHE_PNG); /*"boss05-mo.png"60x42, offset*/
	sakuya->flags							|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	sakuya->anim_frame						= 0;
	sakuya->type							= SP_BOSS/*SP_BOSS01*/;
	sakuya->callback_mover					= move_alice;
	sakuya->callback_loser					= lose_boss;
//	sakuya->callback_hit_enemy				= callback_hit_boss;
	sakuya->callback_hit_enemy				= NULL; 	/* ダミーコールバック登録 */
//
	{
		BOSS01_DATA *data;
		data								= mmalloc(sizeof(BOSS01_DATA));
		sakuya->data						= data;
		//
		/*data->boss_base.boss_*/sakuya->base_health		= ((1024-1)*3);	// パチェ本体のHP。もう少し高くてもいいかも。// [***090305	変更
//		data->boss_base.boss_life			= (0);
		/*data->boss_base.*/sakuya->base_score				= adjust_score_by_difficulty(score( 100000));	/*	10万 x 1人 (計100万==(1人x10万)+(6人x5万)+(2人x30万)) */
//		/*data->boss_base.*/sakuya->base_score				= score(1000)*(difficulty+1);
		data->state1						= ST_00;
		data->state8						= 0;
//		data->move_type 					= ALICE_01_KEITAI;
		data->vx = ( 0);
		data->vy = ( 0);

	//	/*data->boss_base.*/spell_card_boss_timer		= (40*64);			/* 40*64==40[count] 	約40[秒(64/60)](単位は秒ではない) */
	//	/*data->boss_base.*/spell_card_boss_timer		= ((99-24)*64);		/* 75*64==75[count] 	約75[秒(64/60)](単位は秒ではない) */
		/*data->boss_base.*/spell_card_boss_timer		= ((120)*64);		/* 75*64==75[count] 	約120[秒(64/60)](単位は秒ではない) */
		//
		#if 1
	//------------ 弾幕関連
		data->boss_base.danmaku_type		= 0;
		data->boss_base.danmaku_time_out	= 0;
		data->boss_base.danmaku_test		= (DANMAKU_08_rumia-1)/*0*/;
		#endif
	//------------ スペカ関連
		#if 1
		/* [スペカシステム内に移動予定] */
		/* 初回の登録作ってないので手動 */
		create_spell_card_init_dummy();
		spell_card_mode 		= 1/*on*/;
		#endif
//
		((PLAYER_DATA *)player->data)->boss = sakuya;
	}

//----[ZAKO]
	/* 85.33==512/6 170.66== 512/3 */	/*171*/
//	#define AA_OFS85 (85)
	#define AA_OFS85 (100)
	int jj_angle512;
	jj_angle512= -(int)(AA_OFS85/2);	/*0*/
	int i;
	for (i=0; i<8; i++)
	{
		obj_doll[i] 						= sprite_add_res(BASE_BOSS_PACHE_DOLLS_PNG/*doll_res_num[i]*/);
//		obj_doll[i]->anim_speed 			= 0;
		obj_doll[i]->type					= SP_ZAKO/*SP_BOSS05*/;
		obj_doll[i]->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//
		obj_doll[i]->callback_loser 		= lose_doll;
		obj_doll[i]->callback_hit_enemy 	= callback_hit_zako;
//
		DOLL_DATA *data;
		data								= mmalloc(sizeof(DOLL_DATA));
		#if (1==USE_KEISYOU)
		data->sakuya_obj					= sakuya;	/* 継承させる */
		#endif
		obj_doll[i]->data					= data;
		data->identity_bit					= (FLG_DOLL0<<i);		/* 固有ビット番号 */
//		data->health_flag					= 0;
		data->bwait 						= 0;
	//	br1_angle512						= deg_360_to_512(0);
	//	br2_angle512						= deg_360_to_512(0);
//		data->br_angle512					= deg_360_to_512(0);/*CW*/
		data->br_angle512					= (0);/*CCW*/
	//	/*data->base.*/obj_doll[i]->base_score					= score( 500)*(difficulty+1);
		if (3/*1*/<i)
		{
			obj_doll[i]->callback_mover 	= move_doll01;
			obj_doll[i]->anim_frame 		= 0x20+((i&1)<<4);		/* ミニ人形 / 紅い娘の人形 */
			/*data->base.*/obj_doll[i]->base_score				= adjust_score_by_difficulty(score(  50000));	/*	 5万 x 4人 */
			/*data->base.*/obj_doll[i]->base_health 			= ((128-1));/*すぐ死ぬ*/
		}
		else
		{
			obj_doll[i]->callback_mover 	= move_doll02;
			obj_doll[i]->anim_frame 		= (i<<4);				/*	0:藍い娘の人形(旧左上) / 1:碧の娘の人形(旧右上) */
			/*data->base.*/obj_doll[i]->base_score				= adjust_score_by_difficulty(score( 300000));	/*	30万 x 4人 */
			/*data->base.*/obj_doll[i]->base_health 			= ((1024-1)*5);/*硬い(がちゃんと壊せる)*/
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
