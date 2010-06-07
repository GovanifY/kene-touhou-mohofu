
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	パチェ人形
	-------------------------------------------------------

---------------------------------------------------------*/
#if 0/*メモ*/
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif
//	ボス共通規格と同じ(boss.hインクルードしてもしなくても対応)
#ifndef time_out
	#define time_out			user_data03 	/* 制限時間 */
#endif
//
//----[ZAKO]
#define DOLL_DATA_fix_angle1024 	user_data07 	/* てすと固定値 */

//------------ doll02専用

#define DOLL_DATA_state222			user_data04 	/* 形態 */
#define DOLL_DATA_bwait 			user_data05 	/* 弾発射までの待ち時間。 */
#define shot_angle1024				user_data06 	/* 弾の角度 */

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_doll(SPRITE *src)
{
	bakuhatsu_add_type_ddd(src, BAKUHATSU_FIRE08);
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
//
	/* 普段の処理を速く(R4000系が得意な0比較を多用する)して、ここは(滅多に来ないので)遅くします */
//	com mon_boss_flags &= (~(src->DOLL_DATA_identity_bit));
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static int bb_angle1024;
static int cc1024;
static void move_doll01(SPRITE *src)
{
	check_boss_option_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
//
	#if 1
	{
		bb_angle1024++;
	int aa_angle1024;
		aa_angle1024 = bb_angle1024;
	//int jj;
		/* 紅い娘の人形(人形) 旧左前の人形 旧右前の人形 */
	//	for (jj=2; jj<8; jj++)/*2 3 4 5 6 7*/
		{
//			aa_angle1024 += (85*2)/*171*/;/* 85.33*2==1024/6 170.66*2== 1024/3 */
			aa_angle1024 += src->DOLL_DATA_fix_angle1024;
			mask1024(aa_angle1024);
			int radius;
		//	radius = (90)/*45*/;
			{
				radius = ((sin1024(cc1024))>>2)+16; /* 80==16+64 */
			}
		/*CCW*/
			src->x256 = (obj_boss)->x256 + (/*(short)*/((sin1024((aa_angle1024))*radius)));
			src->y256 = (obj_boss)->y256 + (/*(short)*/((cos1024((aa_angle1024))*radius)));
		}
	}
	if (SPELL_CARD_00_pache_000 < spell_card_number)
	{
		if (0==(ra_nd()&(0xff)))
		{
			/* ショット */
		//	b05_fire_flags &= (~(doll_data->identity_bit));/* off */
			obj_send1->x256 					= (src->x256);
			obj_send1->y256 					= (src->y256);
			br.BULLET_REGIST_speed256			= (t256(1.5)+(difficulty<<6));
			br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
			br.BULLET_REGIST_div_angle1024		= (int)(1024/64);
			br.BULLET_REGIST_bullet_obj_type	= BULLET_CAP16_04_KOME_SIROI;			/* [青白米弾] */	/* 弾グラ */
			br.BULLET_REGIST_n_way				= (4+difficulty);					/*(4-difficulty)*/
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
			bullet_regist_vector();
		}
	}
	#endif
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_doll02(SPRITE *src)
{
	check_boss_option_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
//
	cc1024 += (2);

	switch (src->DOLL_DATA_state222)
	{
	case 0:
		src->x256 = (obj_boss)->x256 + (((src->DOLL_DATA_fix_angle1024))<<6);
		src->y256 = (obj_boss)->y256 - t256(16);
	//	if (0==(com mon_boss_flags & FLG_MINI_DOLL))
		{
			src->DOLL_DATA_state222++;
		}
		break;
	case 1:
		/* 下過ぎ(下過ぎると上が空きすぎるし、下に近すぎる) */
		if (t256(28)/*offset*/ < src->y256)
		{
			src->y256 -= t256(1.414);/*fps_factor*/
		}
		/* 適正範囲内(t256(25)ぐらい)、次へ */
		if (t256(22)/*offset*/ < src->y256)
		{
			src->DOLL_DATA_state222++;
		}
		/* 上過ぎ(上過ぎると弾が落ちてこない) */
		else
		{
			src->y256 += t256(1.414);/*fps_factor*/
		}
		break;
	case 2:
		{	src->DOLL_DATA_bwait--;}
		if (src->DOLL_DATA_bwait <= 0)
		{
			src->DOLL_DATA_bwait = 5+(3-difficulty)*5;
			src->shot_angle1024 -= (16);		/* cv1024r(10)*/
//
				obj_send1->x256 						= (src->x256);
				obj_send1->y256 						= (src->y256);
				br.BULLET_REGIST_speed256				= t256(1.0);//t256(1+difficulty)/*(3+difficulty)*/ /*(4+difficulty)*/;
//				br.BULLET_REGIST_angle1024				= (src->shot_angle1024&(256-1))+512+128;// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
				br.BULLET_REGIST_angle1024				= (src->shot_angle1024&(256-1))-512-128;// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
			//	br.BULLET_REGIST_jyuryoku_dan_delta256	= ((ra_nd()&0x03)+1);//t256(0.04)/*10*/
				br.BULLET_REGIST_jyuryoku_dan_delta256	= ((ra_nd()&0x03)+2);//t256(0.04)
				br.BULLET_REGIST_bullet_obj_type		= (BULLET_MARU8_00_AKA+(7));	/* 弾グラ */
				br.BULLET_REGIST_regist_type			= REGIST_TYPE_02_GRAVITY02;
				bullet_regist_vector();
		}
		break;
	}
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_pache_dolls(SPRITE *src)
{
//----[ZAKO]
	bb_angle1024		= (0);

//----[ZAKO]
	/* 85.33*2==512*2/6 170.66*2== 512*2/3 */	/*171*2*/
//	#define AA_OFS85 (85*2)
	#define AA_OFS85 (100*2)
	int jj_angle1024;
	jj_angle1024= -(int)(AA_OFS85/2);	/*0*/
	int i;
	for (i=0; i<8; i++)
	{
		SPRITE *h;	/* 人形達 */
		h						= sprite_add_gu_error();
		if (NULL!=h)/* 登録できた場合のみ */	/* 強制登録 */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
			h->type 				= BOSS_16_YOUSEI11; 	/*S P_ZA KO*/	/*S P_BO SS05*/
//
			h->callback_loser		= lose_doll;
			h->callback_hit_enemy	= callback_hit_zako;
//
			h->DOLL_DATA_bwait			= 0;
	//		h->shot_angle1024	= cv1024r(0);/*CW*/
			h->shot_angle1024	= (0);/*CCW*/
		//	h->base_score				= score( 500)*(difficulty+1);
			if (3/*1*/<i)
			{
				h->callback_mover		= move_doll01;
				h->type 				= /*0x20*/8+BOSS_16_YOUSEI11+((i&1)<<2/*4*/);		/* ミニ人形 / 紅い娘の人形 */
				h->base_score			= adjust_score_by_difficulty(score(  50000));		/*	 5万 x 4人 */
				h->base_hp				= (((128*8)-1));/*すぐ死ぬ*/
			}
			else
			{
				h->callback_mover		= move_doll02;
				h->type 				= BOSS_16_YOUSEI11+(i<<2/*4*/); 			/*	0:藍い娘の人形(旧左上) / 1:碧の娘の人形(旧右上) */
				h->base_score			= adjust_score_by_difficulty(score( 300000));		/*	30万 x 4人 */
				h->base_hp				= (((1024*8)-1)*5);/*硬い(がちゃんと壊せる)*/
			}
			h->DOLL_DATA_fix_angle1024	= jj_angle1024;
			jj_angle1024 += (AA_OFS85);
			h->DOLL_DATA_state222		= (0);
			h->time_out 				= (0x01ff); 	/* 制限時間 */
		}
	}
}

//新スコア配分: 	藍い娘と碧の娘はなるべく倒そう。倍以上スコアが違うよ。
//		data->boss_base.score				= adjust_score_by_difficulty(score( 100000));	/*	10万 x 1人 (計100万==(1人x10万)+(6人x5万)+(2人x30万)) */
//		data->base.score					= adjust_score_by_difficulty(score( 300000));	/*	30万 x 2人 */
//		data->base.score					= adjust_score_by_difficulty(score(  50000));	/*	 5万 x 6人 */

//旧スコア配分
//		data->boss_base.score				= adjust_score_by_difficulty(score( 500000));	/*	50万 x 1人 (計100万==(1人x50万)+(5人x10万)) */
//		data->base.score					= adjust_score_by_difficulty(score( 100000));	/*	10万 x 5人 */
