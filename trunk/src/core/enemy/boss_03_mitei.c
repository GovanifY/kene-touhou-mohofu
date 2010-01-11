
#include "bullet_object.h"

/*---------------------------------------------------------
	3面 未定 永琳? (元、輝夜、かぐや)
	Mitei Madakimattenai.
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

typedef struct /*_boss04_data*/
{
	BOSS_BASE boss_base;
//------------ 移動関連
	int state1; 		/* 形態ステート */		/* 弾幕ステート */
} BOSS04_DATA;

typedef struct /*_boss04_data*/
{
//	ENEMY_BASE base;
//
	#if (1==USE_KEISYOU)
	SPRITE *sakuya_obj; /* 継承 */
	#endif
	int identity_bit;	/* 固有ビット番号 */
	int fix_angle512;	/* てすと固定値 */
//------------ doll02専用
} DOLL_DATA;

// FLG_FIRE1 弾フラグ。真ん中の宝物が壊れていて、かつ指定位置に付いた時。onになる。
//#define FLG_FIRE1 0x01/*左上の藍い娘(宝物)が指定位置に付いたか*/
//#define FLG_FIRE2 0x02/*右上の碧の娘(宝物)が指定位置に付いたか*/
//
#define FLG_DOLL0 0x04/*輝夜本人*/
#define FLG_DOLL1 0x08/*左上の藍い娘(宝物)*/
#define FLG_DOLL2 0x10/*右上の碧の娘(宝物)*/
#define FLG_DOLL3 0x20/*左前の宝物*/
#define FLG_DOLL4 0x40/*真中の紅い娘(宝物)*/
#define FLG_DOLL5 0x80/*右前の宝物*/

#define FLG_ALL_DOLL	(FLG_DOLL1|FLG_DOLL2|FLG_DOLL3|FLG_DOLL4|FLG_DOLL5)/*輝夜以外*/
#define FLG_ALL_CAST	(FLG_ALL_DOLL)/*全員*/


//#define FLG_ALL_CAST	(FLG_DOLL0|FLG_ALL_DOLL)/*全員*/

/*---------------------------------------------------------

---------------------------------------------------------*/

//----[ZAKO]

static int bb_angle512;

/*---------------------------------------------------------
人員の配置メモ
[1][0][2]
[3][4][5]
---------------------------------------------------------*/

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

	#if 0/*???*/
	bakuhatsu_add_zako04(src);
	#endif

	#if 0/*???*/
	player_add_score(data->base.score);/*輝夜以外の小物のスコア*/
	#endif

/*---------------------------------------------------------
	3面専用
---------------------------------------------------------*/
#if (1)
static void enemy_boss04_LR_dole_fire(SPRITE *src)
{
//	if (0!=(common_boss_flags&FLG_DOLL1)/*obj_doll [3]->flags&SP_FLAG_VISIBLE*/) /*LEFT 0*/
//	{
//	//identity_bit
//			bullet_create_aka_maru_jikinerai(/*obj_doll [1]*/src, t256(difficulty+1)/*5*/);
//		//	bullet_create_n_way_dan_sa_type(obj_doll [1], t256(difficulty+1)/*5*/, ANGLE_JIKI_NERAI_DAN, (int)(512/24), BULLET_MARU8_00_AKA, 1);
//	}
	DOLL_DATA *data = (DOLL_DATA *)src->data;
	/* 生きている場合に撃つ */
	if (0 != (common_boss_flags & (data->identity_bit)/*FLG_DOLL2*/)/*obj_doll [5]->flags&SP_FLAG_VISIBLE*/) /*RIGHT 1*/
	{
	//	if (data->identity_bit)
		{
			bullet_create_aka_maru_jikinerai(/*obj_doll [2]*/src, t256(difficulty+1)/*5*/);
		//	bullet_create_n_way_dan_sa_type(obj_doll [2], t256(difficulty+1)/*5*/, ANGLE_JIKI_NERAI_DAN, (int)(512/24), BULLET_MARU8_00_AKA, 1);
		}
	}
}
#endif
/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_doll01(SPRITE *src)
{
	DOLL_DATA *data = (DOLL_DATA *)src->data;
	{
		bb_angle512++;
	int aa_angle512;
		aa_angle512 = bb_angle512;
		{
		//	aa_angle512 += (102);/* 102.4== 512/5 */
			aa_angle512 += data->fix_angle512;/* 102.4== 512/5 */
			mask512(aa_angle512);
			/*CCW*/
			src->x256 = (((PLAYER_DATA *)player->data)->boss)->x256 + ((sin512((aa_angle512))*45));
			src->y256 = (((PLAYER_DATA *)player->data)->boss)->y256 + ((cos512((aa_angle512))*45));
		}
	}
//
	#if (1)
	//	左右ショット
		if (SPELL_CARD_00_mitei_000 < spell_card_number)
		{
		static int firewait1=45;
			firewait1-=1;
			if (firewait1<=0)
			{
				firewait1=45;
				static unsigned int firewait2=0;
				firewait2--;
				firewait2 &= (4-1);
				if (0==firewait2)
				{
				//	enemy_boss01_nway_fire(src);/*enemy_boss04_fire(2);*/
				}
				else
				{
					/* 3面の場合左右が撃つ */
//	#if (0)
//					if (0!=rd4_zero/*0!=data->en emy_rank*/)
//	#endif
					{
						enemy_boss04_LR_dole_fire(src);/*enemy_boss04_fire(0);enemy_boss04_fire(1);*/
					}
				}
			}
		}
	#endif
//
	#if 1
	/* 輝夜を倒すと皆破壊される。 */
	if (0==common_boss_flags)
	{
		src->type = SP_DELETE;
	}
	#endif
}


/*--------------------------------------*/
/*--------------------------------------*/
/*--------------------------------------*/

/*---------------------------------------------------------

---------------------------------------------------------*/

/*int where / 0: left, 1: right, 2: bombenhagel */
//	switch (where)//	case 2:
static void enemy_boss01_nway_fire(SPRITE *src)
{
//	BOSS04_DATA *data = (BOSS04_DATA *)src->data;
	static int hari=0;
	hari--;
	hari &= 32;
	#if (0==USE_DESIGN_TRACK)
	play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
	#else
	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*テキトー*/
	#endif
//
	int hari_no_iro;
	hari_no_iro = BULLET_HARI32_00_AOI;
#if (0)
	/* 4面の場合---- */
	if (0==rd4_zero/*0==data->en emy_rank*/)
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
		hari_no_iro = iro_tbl[(((src->base_health)&(0xfc00) )>>10)];
	//	hari_no_iro = iro_tbl[(data->boss_base.bo ss_life)];
	}
#endif
//
	int angle512;
//	for (angle512=deg_360_to_512(0); angle512</*=*/deg_360_to_512(360/*180*/);
	for (angle512=deg_360_to_512CCW(0); angle512</*=*/deg_360_to_512CCW(360/*180*/);
	//	angle512+=/*22*/21+hari-difficulty/*deg_360_to_512(20)*/		/* 23本(360) ← 8本(180) */
		angle512+=(/*22*/18+hari-difficulty-difficulty/*deg_360_to_512(20)*/)		/* 最低 == 10 == 48本(360)	*/
		)
	{
		bullet_create_offset_dan_type000(
			src,//(((PLAYER_DATA *)player->data)->boss)/*obj_doll [4]*/,
			(int)(/*6*/ (0x100)+(ra_nd()&0x2ff)/*t256(2.5)*/),
			angle512,
		//	/*offsx*/t256(0)/*(src->w/2)*/,
		//	/*offsy*/t256(0)/*(src->h/2)*/,
			hari_no_iro
		);
	}
}

/*---------------------------------------------------------
	左右ショット
---------------------------------------------------------*/

static void kaguya_sayuu_shot(SPRITE *src)
{
	if (SPELL_CARD_00_mitei_000 < spell_card_number)
	{
	static int firewait1=45;
		firewait1-=1;
		if (firewait1<=0)
		{
			firewait1=45;
			static unsigned int firewait2=0;
			firewait2--;
			firewait2 &= (4-1);
			if (0==firewait2)
			{
				enemy_boss01_nway_fire(src);/*enemy_boss04_fire(2);*/
			}
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/ void mitei_01_keitai(SPRITE *src)
{
	/* kene 変身 */
	if (BOSS_00_BOSS11==src->type)
	{
		src->type						= BOSS_00_BOSS11+4;
	}
//
	static int my_wait;
	my_wait -= (1)/*fps_fa ctor*/;
//
	BOSS04_DATA *data = (BOSS04_DATA *)src->data;
	{
		if (my_wait <= 0)
		{	/* 移動方向を決める */
			data->state1++;
			data->state1 &= (4-1);
			{
				enum
				{
					PPP_00_VX = 0,			/* x ベクトル移動量 */
					PPP_01_VY,				/* y ベクトル移動量 */
					PPP_02_WAIT_DIV_2,		/* ウェイトカウンタの半分量 */
					PPP_03_IS_RESET_ANIME,	/* アニメーションリセット 0:しない 1:する 2:特別(弾幕撃ち) */
				};
				s8 ppp[/*8*/4][4] =
				{
					{( 2),(-1),(100),( 0),},	/*右上へ*/
				//	{( 0),( 0),( 50),( 2),},	/*wait*/
					{( 2),( 1),(100),( 0),},	/*右下へ*/
				//	{( 0),( 0),( 10),( 0),},	/*wait*/
					{(-2),(-1),(100),( 1),},	/*左上へ*/
				//	{( 0),( 0),( 50),( 2),},	/*wait*/
					{(-2),( 1),(100),( 0),},	/*左下へ*/
				//	{( 0),( 0),( 10),( 0),},	/*wait*/
				};
			//						src->vx256 = t256(-2); src->vy256 = t256(-1);
			//	center_shot(src);	src->vx256 = t256(-2); src->vy256 = t256( 1);
			//						src->vx256 = t256( 2); src->vy256 = t256(-1);
			//	center_shot(src);	src->vx256 = t256( 2); src->vy256 = t256( 1);
			//
				src->vx256	= ((ppp[data->state1][PPP_00_VX])<<8);
				src->vy256	= ((ppp[data->state1][PPP_01_VY])<<8);
				my_wait=60;
				/* 4面の場合---- */
				#if (0)
			//	if (0!=ppp[data->state1][PPP_03_IS_RESET_ANIME])
			//	{
			//		center_shot(src);
			//	}
				#endif
			}
		}
	}
}


/*---------------------------------------------------------
	ボス行動
---------------------------------------------------------*/

static void move_mitei(SPRITE *src)
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
	boss_effect(src);			/* 回エフェクト */
	kaguya_sayuu_shot(src); 	/* 弾幕生成 */
//	kaguya_pong_boll(src);		/* 弾幕生成 */	/* 4面の場合跳ねる珠 */
//
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
//		data->identity			= 0/*i*/;		// 固有番号
//		data->health_flag		= 0;
	//	sakuya->base_health = (difficulty<<6/*[x64]*/)+ 200+1024;
	//	sakuya->base_health = (1024-1);
	//	sakuya->base_health = (1024-1)*(((difficulty<<6/*[x64]*/)+ 200+1024)>>10);
	//	sakuya->base_score			= score(500)*(difficulty+1);
	//	firewait1		= 45;
	//	firewait2		= 4;
	//	firewait3		= 0;
	//	bomb_n			= 0;

void add_boss_mitei(STAGE_DATA *l)/*int lv*/
{
	boss_bgm_mode		= (l->user_y);
//
//----[ZAKO]
	bb_angle512 		= 0;

	common_boss_flags	= (FLG_ALL_CAST);

//----[BOSS]
		SPRITE *sakuya;
	//	sakuya								= sprite_add_res(BASE_BOSS_KAGUYA_PNG);
		sakuya								= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		sakuya->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	//	sakuya->anim_speed					= 0;
	//	sakuya->yx_an im_frame				= 0/*0*/+(1/*rd4_zero*/<<4);
		sakuya->type						= BOSS_00_BOSS11; 	/*SP_BOSS*/ 	/*SP_BOSS03*/
		sakuya->callback_mover				= move_mitei;
		sakuya->callback_loser				= common_boss_put_items;
//
		#if 0/* 初期化済みの必要あり */
		sakuya->vx256						= t256( 0);
		sakuya->vy256						= t256( 0); 	/**fps_fa ctor*/
		#endif
		/* 3面の場合---- */ 	/* 全部一緒 */
		sakuya->base_health 				= (((difficulty<<6/*[x64]*/)+ 200+1024) );/*test*/
		sakuya->base_score					= adjust_score_by_difficulty(score( 500000));	/* 50万 (計300万==6x50万) */
//
	//------------ スペカ関連
		spell_card_number					= SPELL_CARD_00_mitei_000;
		spell_card_max						= SPELL_CARD_17_mitei_hhh;
		spell_card_boss_init_regist(sakuya);
	{
		BOSS04_DATA *data;
		data								= mmalloc(sizeof(BOSS04_DATA));
		sakuya->data						= data;
		data->state1						= (0)/*ST_00*/;
		#if 1
	//------------ 弾幕関連
		#endif
	}

//----[ZAKO]
	//	aa_angle512 += (102);/* 102.4== 512/5 */
	/* 85.33==512/6 170.66== 512/3 */	/*171*/
//	#define AA_OFS85 (85)
	#define AA_OFS85 (102)
	int jj_angle512;
	jj_angle512= -(int)(AA_OFS85/2);	/*0*/
	int i;
	for (i=1/*0*/; i<6; i++)
	{
		/*static*/ SPRITE *obj_doll; 	/* 人形達 */
	//	obj_doll 						= sprite_add_res(BASE_BOSS_KAGUYA_PNG);
		obj_doll 						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
//		obj_doll->anim_speed 			= 0;
		obj_doll->type					= BOSS_16_YOUSEI11;		/*SP_ZAKO*/	 	/*SP_BOSS03*/
		obj_doll->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//
		obj_doll->callback_loser 		= lose_doll;
		obj_doll->callback_hit_enemy 	= callback_hit_zako;
//
		DOLL_DATA *data;
		data							= mmalloc(sizeof(BOSS04_DATA));
		#if (1==USE_KEISYOU)
		data->sakuya_obj				= sakuya;	/* 継承させる */
		#endif
		obj_doll->data					= data;
		data->identity_bit				= (FLG_DOLL0<<i);		/* 固有ビット番号 */
//		data->health_flag				= (0);
		obj_doll->base_health			= (1024-1);
//
		obj_doll->callback_mover 		= move_doll01;
		data->fix_angle512				= jj_angle512;
		jj_angle512 += (AA_OFS85);
	//		/* 3面の場合 */ 		/* 全部一緒 */
		//	data->base.health			= (difficulty<<6/*[x64]*/)+ 200+1024;
//			data->base.life 			= (((difficulty<<6/*[x64]*/)+ 200+1024)>>10);
		//	data->base.score 			= score(500)*(difficulty+1);
			obj_doll->base_score 		= adjust_score_by_difficulty(score( 500000));	/* 50万 (計300万==6x50万) */
	}
}
