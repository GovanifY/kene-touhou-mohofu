
#include "bullet_object.h"
//extern void add_zako_alice_doll(SPRITE *src);/* アリス人形弾幕 */

/*---------------------------------------------------------
	アリス・マーガトロイド
	Alice Margatroid.
	-------------------------------------------------------
	ToDo:
	ボスタイマー対応中
---------------------------------------------------------*/

//	int st ate1;	/* 形態ステート */	/* 弾幕ステート */
#define vx			user_data00 	/* xベクトル量 */
#define vy			user_data01 	/* yベクトル量 */
//
static unsigned int alice_anime_count;

/*---------------------------------------------------------

---------------------------------------------------------*/

static int vvv256;
static void alice_animation(SPRITE *src)
{
	vvv256++;	/*vvv256+=vvv256*/
	if (vvv256> t256(1.0) )
	{	vvv256= t256(1.0);}
//
	if (alice_anime_count)	/* 攻撃アニメーション */
	{
		alice_anime_count--;
		if ((32)>alice_anime_count) 	{	src->type = TEKI_09_BOSS32;}	/*src->an im_frame = 0x23;*/
		else
		if ((40)>alice_anime_count) 	{	src->type = TEKI_10_BOSS33;}	/*src->an im_frame = 0x22;*/
		else
										{	src->type = TEKI_09_BOSS32;}	/*src->an im_frame = 0x21;*/
	}
	else	/* 移動アニメーション */
	{
		int aaa;
		aaa = (src->vx256>0)?(TEKI_00_BOSS11/*0x00*/):(TEKI_04_BOSS21/*0x10*/); /*左右*/
		if ((16)>vvv256)		{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else
		if ((200)>vvv256)		{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else
		if ((224)>vvv256)		{	aaa+=2;}	/*128.0==t256(0.5)*/
		else
		if ((240)>vvv256)		{	aaa+=1;}	/*179.2==t256(0.7)*/
	//	else					{	aaa+=0;}
		src->type = aaa;
	}
}


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
#if 0
//extern const u8 alice_danmaku_table[16];
static const u8 alice_danmaku_table[16] =
{
	DANMAKU_07_sakuya,
	DANMAKU_0b_alice_doll,	//DANMAKU_08_rumia,
	DANMAKU_0b_alice_doll,	//DANMAKU_04_sakuya,
	DANMAKU_0b_alice_doll,	//DANMAKU_09_alice,
	DANMAKU_0b_alice_doll,	//DANMAKU_05_sakuya,
	DANMAKU_0b_alice_doll,	//DANMAKU_0a_houka_kenran/*DANMAKU_09_alice*/,
	DANMAKU_0b_alice_doll,	//DANMAKU_06_sakuya,
	DANMAKU_0b_alice_doll,	//DANMAKU_09_alice,
//
	DANMAKU_07_sakuya,
	0,
	DANMAKU_01_sakuya,
	DANMAKU_02_sakuya,
	DANMAKU_03_sakuya,
	DANMAKU_04_sakuya,
	DANMAKU_05_sakuya,
	DANMAKU_06_sakuya,
};
#endif
/*static*/ void alice_01_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait--;/*fps_factor*/
	{
		if (my_wait <= 0)
		{	/* 移動方向を決める */
			src->boss_base_state001++;
			src->boss_base_state001 &= (8-1);
			{
				enum
				{
					PPP_00_VX = 0,			/* x ベクトル移動量 */
					PPP_01_VY,				/* y ベクトル移動量 */
					PPP_02_WAIT_COUNT,		/* ウェイトカウンタの量 */
					PPP_03_IS_RESET_ANIME,	/* アニメーションリセット 0:しない 1:する 2:特別(弾幕撃ち) */
				};
				static const s8 ppp[8][4] =
				{
					{( 2)*8,(-1)*8,(10*8),( 1),},	/*右上へ*/
					{( 0)*8,( 0)*8,( 5*8),( 2),},	/*wait*/
					{( 2)*8,( 1)*8,(10*8),( 1),},	/*右下へ*/
					{( 0)*8,( 0)*8,( 1*8),( 0),},	/*wait*/
					{(-2)*8,(-1)*8,(10*8),( 1),},	/*左上へ*/
					{( 0)*8,( 0)*8,( 5*8),( 2),},	/*wait*/
					{(-2)*8,( 1)*8,(10*8),( 1),},	/*左下へ*/
					{( 0)*8,( 0)*8,( 1*8),( 0),},	/*wait*/
				};
//				data->vx	= ppp[src->boss_base_state001][PPP_00_VX];
//				data->vy	= ppp[src->boss_base_state001][PPP_01_VY];
				my_wait 	= ppp[src->boss_base_state001][PPP_02_WAIT_COUNT];	/* 移動量 */
				if (0!=ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME])
				{
					if (2==ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME]) /* 攻撃アニメーション */
					{
						alice_anime_count = 48;
//						src->boss_base_danmaku_test++;
//						src->boss_base_danmaku_test &= 0x07;
						src->boss_base_danmaku_type 	= DANMAKU_09_alice/*alice_danmaku_table[src->boss_base_danmaku_test]*/; /*DANMAKU_01*/	/* 禊弾幕をセット */
						src->boss_base_danmaku_time_out = (DANMAKU_01_SET_TIME+DANMAKU_01_SET_TIME);			/* 禊弾幕の発生時間 x 2 */
					}
					vvv256=1;
				}
			}
		}
	}
//	src->vx256 = (data->vx)*vvv256;
//	src->vy256 = (data->vy)*vvv256;
}

/*static*/ void alice_02_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait--;/*fps_factor*/
	{
		if (my_wait <= 0)
		{	/* 移動方向を決める */
			src->boss_base_state001++;
			src->boss_base_state001 &= (8-1);
			{
				enum
				{
					PPP_00_VX = 0,			/* x ベクトル移動量 */
					PPP_01_VY,				/* y ベクトル移動量 */
					PPP_02_WAIT_COUNT,		/* ウェイトカウンタの量 */
					PPP_03_IS_RESET_ANIME,	/* アニメーションリセット 0:しない 1:する 2:特別(弾幕撃ち) */
				};
				static const s8 ppp[8][4] =
				{
					{( 2)*8,(-1)*8,(10*8),( 1),},	/*右上へ*/
					{( 0)*8,( 0)*8,( 5*8),( 2),},	/*wait*/
					{( 2)*8,( 1)*8,(10*8),( 1),},	/*右下へ*/
					{( 0)*8,( 0)*8,( 1*8),( 0),},	/*wait*/
					{(-2)*8,(-1)*8,(10*8),( 1),},	/*左上へ*/
					{( 0)*8,( 0)*8,( 5*8),( 2),},	/*wait*/
					{(-2)*8,( 1)*8,(10*8),( 1),},	/*左下へ*/
					{( 0)*8,( 0)*8,( 1*8),( 0),},	/*wait*/
				};
				src->vx = ppp[src->boss_base_state001][PPP_00_VX];
				src->vy = ppp[src->boss_base_state001][PPP_01_VY];
				my_wait = ppp[src->boss_base_state001][PPP_02_WAIT_COUNT];		/* 移動量 */
				if (0!=ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME])
				{
					if (2==ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME]) 	/* 攻撃アニメーション */
					{
						alice_anime_count = 48;
//						src->boss_base_danmaku_test++;
//						src->boss_base_danmaku_test &= 0x07;
						src->boss_base_danmaku_type 	= DANMAKU_0b_alice_doll/*alice_danmaku_table[src->boss_base_danmaku_test]*/;	/*DANMAKU_01*/	/* 禊弾幕をセット */
						src->boss_base_danmaku_time_out = (DANMAKU_01_SET_TIME+DANMAKU_01_SET_TIME);			/* 禊弾幕の発生時間 x 2 */
					}
					vvv256=1;
				}
			}
		}
	}
	src->vx256 = (src->vx)*vvv256;
	src->vy256 = (src->vy)*vvv256;
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
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_boss_alice(STAGE_DATA *l)
{
	alice_anime_count	= 0;
	vvv256	= 0;
	//----[BOSS]
	SPRITE *h;
	#if (0==USE_BOSS_COMMON_MALLOC)
	h								= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	#else
	h								= pd_boss;/*輝夜本人*/
	#endif
	h->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	h->type 						= BOSS_00_BOSS11;
	h->callback_mover				= move_alice;
	h->callback_loser				= common_boss_put_items;
//
	h->base_score					= adjust_score_by_difficulty(score( 100000));	/*	10万 x 1人 (計100万==(1人x10万)+(6人x5万)+(2人x30万)) */
//------------ スペカ関連
	spell_card_number				= SPELL_CARD_00_alice_000;
	spell_card_max					= SPELL_CARD_14_alice_eee; /*SPELL_CARD_19_alice_jjj;*/
	#if (0==USE_BOSS_COMMON_MALLOC)
	spell_card_boss_init_regist(h);
	#else
	spell_card_boss_init_regist_void();
	#endif
	h->boss_base_state001			= (0);	/*ST_00*/
	h->vx							= (0);
	h->vy							= (0);
}
