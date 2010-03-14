
#include "bullet_object.h"

/*---------------------------------------------------------
	蓬莱山 輝夜
	Houraisan Kaguya.
	-------------------------------------------------------
	かぐや
	ToDo:
	4面のみ(3面には対応しません)
	ボスタイマー対応中
---------------------------------------------------------*/


/*---------------------------------------------------------

---------------------------------------------------------*/

/*int where / 0: left, 1: right, 2: bombenhagel */
//	switch (where)//	case 2:
static void enemy_boss01_nway_fire(SPRITE *src)
{
	static int hari=0;
	hari--;
	hari &= 32;
	voice_play(VOICE11_BOSS_KIRARIN/*VOICE14_BOSS_KOUGEKI_01*/, TRACK04_TEKIDAN);/*テキトー*/
//
	int hari_no_iro;
	hari_no_iro = BULLET_HARI32_00_AOI;
	/* 4面の場合---- */
#if (0)
	if (0==rd4_zero/*0==data->en emy_rank*/)
#endif
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
//
	int angle512;
	int spd;
	spd = (0x180)+(ra_nd()&0x0ff);
//	for (angle512=deg_360_to_512(0); angle512</*=*/deg_360_to_512(360/*180*/);
	for (
			angle512 = (0); 	/*deg_360_to_512CCW(0)*/
		//	angle512 < /*=*/deg_360_to_512CCW(360/*180*/);
			angle512 <	((4/*difficulty*/+1)<<9)			;	//	/*=*/ deg_360_to_512CCW(360/*180*/)
		//	angle512+=/*22*/21+hari-difficulty/*deg_360_to_512(20)*/		/* 23本(360) ← 8本(180) */
			angle512 += (/*22*/18+hari-difficulty-difficulty/*deg_360_to_512(20)*/) 	/* 最低 == 10 == 48本(360)	*/
		)
	{
		bullet_create_offset_dan_type000(src,//(pd_boss)/*obj_doll [4]*/,
			(int)(/*6*/(spd)/*t256(2.5)*/),
			angle512,
		//	/*offsx*/t256(0)/*(src->w/2)*/,
		//	/*offsy*/t256(0)/*(src->h/2)*/,
			hari_no_iro
		);
		spd += (64>>3/*difficulty*/);
	}
}

/*---------------------------------------------------------
	4面専用
---------------------------------------------------------*/

static void center_shot(SPRITE *src)
{
	/* 4面の場合---- */
#if (0)
	if (0==rd4_zero/*0==data->en emy_rank*/)
#endif
	{
		int kakudo;
		kakudo = (127-16);
		kakudo += ((ra_nd())&(32-1));
//
		send1_obj->x256 = src->x256;
		send1_obj->y256 = src->y256;
		send1_obj->BULLET_REGIST_angle512			=	kakudo;
		send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/24);
		send1_obj->BULLET_REGIST_n_way				=	(24);
//
		switch ((src->base_hp)&(0xe000) )
	//	switch (src->boss_base_bo ss_life)
		{
		case (3<<13):/*not_break;*/
			send1_obj->BULLET_REGIST_speed256			=	t256(0.6);		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_03_MIDORI;		bullet_regist_basic();
			send1_obj->BULLET_REGIST_speed256			=	t256(1.4);		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_05_KIIRO;		bullet_regist_basic();
		case (2<<13):/*not_break;*/
		case (4<<13):/*not_break;*/
			send1_obj->BULLET_REGIST_speed256			=	t256(1.0);		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_00_AOI;			bullet_regist_basic();
			send1_obj->BULLET_REGIST_speed256			=	t256(1.8);		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_04_MIZUIRO;		bullet_regist_basic();
		case (5<<13):/*not_break;*/
		case (1<<13):/*not_break;*/
			send1_obj->BULLET_REGIST_speed256			=	t256(1.2);		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_02_YUKARI;		bullet_regist_basic();
			send1_obj->BULLET_REGIST_speed256			=	t256(0.8);		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_01_AKA;			bullet_regist_basic();
			send1_obj->BULLET_REGIST_speed256			=	t256(2.0);		send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_01_AKA;			bullet_regist_basic();
			break;
		}
	}
}


/*---------------------------------------------------------
	左右ショット
---------------------------------------------------------*/

static void kaguya_sayuu_shot(SPRITE *src)
{
	if (SPELL_CARD_00_kaguya_000 < spell_card_number)
	{
	static int fire_wait1;
		fire_wait1--;
		if (0 > fire_wait1)
		{
			fire_wait1 = (45*4);
			enemy_boss01_nway_fire(src);/*enemy_boss04_fire(2);*/
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
		//switch ( (src->base_hp)&0xe000 )
		if (src->base_hp <= (8*(1000+(1024))))
		//if (src->boss_base_bo ss_life < 2)
		{
			fire_wait3--;
			if (fire_wait3 < 0)
			{
				if ((8*(200+1024)) < src->base_hp)
			//	if (0 < src->boss_base_bo ss_life)
				{	fire_wait3 = ((src->base_hp)>>(2+3));}
				else
				{	fire_wait3 = (50);}
			//
				if (bomb_n < 32)
				{	bomb_n++;}
				int i;
				for (i=0; i <= (int)((bomb_n*(0x100/3))>>8); i++)
				{
					bullet_create_hazumi_dan(src,
						t256(0.05)+(difficulty<<6), 			/* speed256 t256(5.0) */
						((512)*3*i/(bomb_n+1)), 				/* angle512 */
						(t256(0.07)),							/* delta256 */	/*17.92==t256(0.07)*/
						(difficulty)							/* bound_counts (2) */
					);		/* [***090116		微調整 */
				}
			}
		}
	}
}

/*---------------------------------------------------------
	微上下移動
---------------------------------------------------------*/

/*static*/ void kaguya_01_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait--;/*fps_factor*/
//
	{
		if (my_wait <= 0)
		{	/* 移動方向を決める */
			src->boss_base_state001++;
			src->boss_base_state001 &= (4-1);
			{
				enum
				{
					dummy_PPP_00_VX = 0,	/* x ベクトル移動量 */
					PPP_01_VY,				/* y ベクトル移動量 */
					PPP_02_WAIT_DIV_2,		/* ウェイトカウンタの半分量 */
					PPP_03_IS_RESET_ANIME,	/* アニメーションリセット 0:しない 1:する 2:特別(弾幕撃ち) */
				};
				static const s8 ppp[/*8*/4][4] =
				{
					{( 2),(-1),(100),( 1),},	/*右上へ*/
				//	{( 0),( 0),( 50),( 2),},	/*wait*/
					{( 2),( 1),(100),( 3),},	/*右下へ*/
				//	{( 0),( 0),( 10),( 0),},	/*wait*/
					{(-2),(-1),(100),( 0),},	/*左上へ*/	/* [右下] */
				//	{( 0),( 0),( 50),( 2),},	/*wait*/
					{(-2),( 1),(100),( 2),},	/*左下へ*/
				//	{( 0),( 0),( 10),( 0),},	/*wait*/
				};
			//						src->vx256 = t256(-2); src->vy256 = t256(-1);
			//	center_shot(src);	src->vx256 = t256(-2); src->vy256 = t256( 1);
			//						src->vx256 = t256( 2); src->vy256 = t256(-1);
			//	center_shot(src);	src->vx256 = t256( 2); src->vy256 = t256( 1);
			//
//				src->vx256	= ((ppp[src->boss_base_state001][PPP_00_VX])<<8);
//				src->vy256	= ((ppp[src->boss_base_state001][PPP_01_VY])<<8);
//				src->vx256	= (0);
				src->vy256	= ((ppp[src->boss_base_state001][PPP_01_VY])<<4);
				my_wait = 60;
				/* 4面の場合---- */
				#if (1)
			//	if (0!=ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME])
				if (difficulty >= ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME])
				{
					if (ra_nd()&1)		/* 50%の確率で */
					{
						center_shot(src);
					}
				}
				#endif
			}
		}
	}
}

/*---------------------------------------------------------
	斜め移動
---------------------------------------------------------*/

/*static*/ void kaguya_04_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait--;/*fps_factor*/
//
	{
		if (my_wait <= 0)
		{	/* 移動方向を決める */
			src->boss_base_state001++;
			src->boss_base_state001 &= (4-1);
			{
				enum
				{
					PPP_00_VX = 0,			/* x ベクトル移動量 */
					PPP_01_VY,				/* y ベクトル移動量 */
					PPP_02_WAIT_DIV_2,		/* ウェイトカウンタの半分量 */
					PPP_03_IS_RESET_ANIME,	/* アニメーションリセット 0:しない 1:する 2:特別(弾幕撃ち) */
				};
				static const s8 ppp[/*8*/4][4] =
				{
					{( 2),(-1),(100),( 1),},	/*右上へ*/
				//	{( 0),( 0),( 50),( 2),},	/*wait*/
					{( 2),( 1),(100),( 3),},	/*右下へ*/
				//	{( 0),( 0),( 10),( 0),},	/*wait*/
					{(-2),(-1),(100),( 0),},	/*左上へ*/	/* [右下] */
				//	{( 0),( 0),( 50),( 2),},	/*wait*/
					{(-2),( 1),(100),( 2),},	/*左下へ*/
				//	{( 0),( 0),( 10),( 0),},	/*wait*/
				};
			//						src->vx256 = t256(-2); src->vy256 = t256(-1);
			//	center_shot(src);	src->vx256 = t256(-2); src->vy256 = t256( 1);
			//						src->vx256 = t256( 2); src->vy256 = t256(-1);
			//	center_shot(src);	src->vx256 = t256( 2); src->vy256 = t256( 1);
			//
				src->vx256	= ((ppp[src->boss_base_state001][PPP_00_VX])<<8);
				src->vy256	= ((ppp[src->boss_base_state001][PPP_01_VY])<<8);
				my_wait = 60;
				/* 4面の場合---- */
				#if (1)
			//	if (0!=ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME])
				if (difficulty >= ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME])
				{
					if (ra_nd()&1)		/* 50%の確率で */
					{
						center_shot(src);
					}
				}
				#endif
			}
		}
	}
}


/*---------------------------------------------------------
	ボス行動
---------------------------------------------------------*/

static void move_kaguya(SPRITE *src)
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
	kaguya_pong_boll(src);		/* 弾幕生成 */
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_boss_kaguya(STAGE_DATA *l)
{
	fire_wait3			= 0;
	bomb_n				= 0;

//----[BOSS]
	SPRITE *h;
	#if (0==USE_BOSS_COMMON_MALLOC)
	h								= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	#else
	h								= pd_boss;/*輝夜本人*/
	#endif
	h->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	h->type 						= BOSS_00_BOSS11;
	h->callback_mover				= move_kaguya;
	h->callback_loser				= common_boss_put_items;
//
	#if 0/* 初期化済みの必要あり */
	h->vx256						= t256( 0);
	h->vy256						= t256( 0);
	#endif
	/* 4面の場合---- */ 	/* 輝夜本人 */
	h->base_score					= adjust_score_by_difficulty(score(2000000));	/* 200万 (計400万==(200万)+(5x40万)) */
//------------ スペカ関連
	spell_card_number				= SPELL_CARD_00_kaguya_000;
	spell_card_max					= SPELL_CARD_17_kaguya_hhh;
	#if (0==USE_BOSS_COMMON_MALLOC)
	spell_card_boss_init_regist(h);
	#else
	spell_card_boss_init_regist_void();
	#endif
	h->boss_base_state001			= (0);/*ST_00*/
}
