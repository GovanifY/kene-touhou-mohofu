
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ボスの行動を定義する。
---------------------------------------------------------*/


/*---------------------------------------------------------
	[ボス共通カード移動]。
---------------------------------------------------------*/


/*---------------------------------------------------------
	カード生成終了ならカード設定し、生成。
---------------------------------------------------------*/
/*static*/global OBJ_CALL_FUNC(boss_move_00_card_settei_seisei)
{
	if (SPELL_00==card.spell_used_number)	/* カード生成終了ならカード生成 */
	{
		card_maikai_init_and_get_spell_number(src); 		/* 現在の形態で撃てるカード番号をカードシステムに設定。 */
	}
}

/*---------------------------------------------------------
	xランダム32[pixel]以内差分移動。
	yランダム 8[pixel]以内差分移動。
---------------------------------------------------------*/

static OBJ_CALL_FUNC(boss_set_position_differential32)
{
	#if 1
	/* 現在位置より左右 32[pixel] の範囲でランダム移動。但し最低16[pixel]は動く。 */
	const unsigned short ra_nd16	= ra_nd();
	REG_02_DEST_X	= src->center.x256;
	{	const unsigned short vector_value = ((ra_nd16&(((32)*256)-1))|(16*256));/* 移動量を決める。 但し最低16[pixel]は動く */
	//	if (ra_nd16&0x2000) /* 移動方向を決める */ /* 0x1fff == ((32)*256-1) */
	//	if (REG_02_DEST_X < (ra_nd16+ra_nd16)) /* 移動方向を決める */
		if (REG_02_DEST_X < (ra_nd16+((GAME_X_OFFSET+(GAME_320_WIDTH/2)-128)<<8))) /* 移動方向を決める */
				{	REG_02_DEST_X += (vector_value);	}
		else	{	REG_02_DEST_X -= (vector_value);	}
	}
	/* 現在位置より上下 8[pixel] の範囲でランダム移動 */
	REG_03_DEST_Y = src->center.y256;
	REG_03_DEST_Y += ((((ra_nd16))&((8+8)-1))<<8);
	REG_03_DEST_Y -= (t256(8));
	#endif
}
#if 0
/*---------------------------------------------------------
	xランダム64[pixel]以内差分移動。
	yランダム 8[pixel]以内差分移動。
---------------------------------------------------------*/

static OBJ_CALL_FUNC(boss_set_position_differential64)
{
	#if 1
	/* 現在位置より左右 64[pixel] の範囲でランダム移動。但し最低16[pixel]は動く。 */
	const unsigned short ra_nd16	= ra_nd();
	REG_02_DEST_X	= src->center.x256;
	{	const unsigned short vector_value = ((ra_nd16&(((64)*256)-1))|(16*256));/* 移動量を決める。 但し最低16[pixel]は動く */
	//	if (ra_nd16&0x2000) /* 移動方向を決める */ /* 0x1fff == ((32)*256-1) */
	//	if (REG_02_DEST_X < (ra_nd16+ra_nd16)) /* 移動方向を決める */
		if (REG_02_DEST_X < (ra_nd16+((GAME_X_OFFSET+(GAME_320_WIDTH/2)-128)<<8))) /* 移動方向を決める */
				{	REG_02_DEST_X += (vector_value);	}
		else	{	REG_02_DEST_X -= (vector_value);	}
	}
	/* 現在位置より上下 8[pixel] の範囲でランダム移動 */
	REG_03_DEST_Y = src->center.y256;
	REG_03_DEST_Y += ((((ra_nd16))&((8+8)-1))<<8);
	REG_03_DEST_Y -= (t256(8));
	#endif
}
#endif
/*---------------------------------------------------------
	カード生成の間隔と移動時間の間隔が同期。
	xyランダム差分移動。
---------------------------------------------------------*/

//obal/*static*/ OBJ_CALL_FUNC(boss_move_05_xy_douki_differential64)
global/*static*/ OBJ_CALL_FUNC(boss_move_04_xy_douki_differential32)
{
	/* カードを撃ってない場合に追加 */
	if (SPELL_00==card.spell_used_number)	/* カード生成終了なら、新移動先を決める。 */
	{
	//	boss_set_position_differential64(src);
		boss_set_position_differential32(src);
		#if (1)
		/* 移動座標を決める */
	//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* 攻撃アニメーション */
		boss_set_new_position(src);/*(誘導移動座標を設定)*/
		#endif
	}
	/*(以下は将来的に共通化される予定)*/
	boss_move_00_card_settei_seisei(src);/*(カード生成終了ならカード設定し、生成。)*/
//r36たぶん。	boss_re gacy_yuudou(src);
}


/*---------------------------------------------------------
	カード生成の間隔と移動時間の間隔が同期。
	xランダム移動。
	y移動しない。
---------------------------------------------------------*/

global OBJ_CALL_FUNC(boss_move_03_x_douki)
{
	/* カードを撃ってない場合に追加 */
	if (SPELL_00==card.spell_used_number)	/* カード生成終了なら、新移動先を決める。 */
	{
		#if 1
		const unsigned short ra_nd16	= ra_nd();
		REG_02_DEST_X	= ((ra_nd16/*&((256*256)-1)*/));	/* 0(???) =:= t256(((480-128-256)/2)-50)) */
		REG_03_DEST_Y	= (src->center.y256);/*(t256()形式)*/
		#endif
		#if (1)
		/* 移動座標を決める */
	//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* 攻撃アニメーション */
		boss_set_new_position(src);/*(誘導移動座標を設定)*/
		#endif
	}
	/*(以下は将来的に共通化される予定)*/
	boss_move_00_card_settei_seisei(src);/*(カード生成終了ならカード設定し、生成。)*/
//r36たぶん。	boss_re gacy_yuudou(src);
}


/*---------------------------------------------------------
	カード生成の間隔と移動時間の間隔が非同期。
	xランダム移動。
	yランダム移動。
---------------------------------------------------------*/

global OBJ_CALL_FUNC(boss_move_02_xy_hidouki)
{
	/* カード生成の間隔と移動時間の間隔が非同期。 */
	src->BOSS_DATA_05_move_jyumyou--;/*fps_factor*/
	{
		if (src->BOSS_DATA_05_move_jyumyou <= 0)
		{
			src->BOSS_DATA_05_move_jyumyou	= (300);	/* 移動時間、間隔 */
			#if 1
			int rand32 = ((ra_nd()));
			/* 移動座標を決める */
			REG_02_DEST_X = ((rand32&0xff00)+(((rand32<<8)&0x3f00)));	/* (256+64) == 320 == (480-128-32) */
			REG_03_DEST_Y = (((rand32>>8)&0x3f00)); 	/* (64) */
			#endif
			#if (1)
			/* 移動座標を決める */
		//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* 攻撃アニメーション */
			boss_set_new_position(src);/*(誘導移動座標を設定)*/
			#endif
		}
	}
	/*(以下は将来的に共通化される予定)*/
	boss_move_00_card_settei_seisei(src);/*(カード生成終了ならカード設定し、生成。)*/
//r36たぶん。	boss_re gacy_yuudou(src);
}
/* 出現時x座標 */
//#define BOSS_XP256		(t256(GAME_WIDTH/2))	/* 中心座標なので */
#define BOSS_XP256			(t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH/2))	/* 中心座標なので */

/*---------------------------------------------------------
	固定退避位置へ移動後、動かないでカードを撃つ。
	-------------------------------------------------------
	固定退避位置は上部、X軸は中心。とりあえず。
---------------------------------------------------------*/

global OBJ_CALL_FUNC(boss_move_01_taihi_ue_naka)
{
	#if (1)
	/* 移動座標を決める */
//	src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* 攻撃アニメーション */
//	REG_02_DEST_X	= (src->BOSS_DATA_00_target_x256);/*(t256()形式)*/
//	REG_03_DEST_Y	= (src->BOSS_DATA_01_target_y256);/*(t256()形式)*/
	REG_02_DEST_X	= (BOSS_XP256);/*(t256()形式)*/
	REG_03_DEST_Y	= (t256(16.0));/*(t256()形式)*/
	boss_set_new_position(src);/*(誘導移動座標を設定)*/
	#endif
	card.boss_move_card_callback = boss_move_00_card_settei_seisei;/*(カード生成終了ならカード設定し、生成。)*/
}

#if 1
/*---------------------------------------------------------
	第11形態: 最終形態(その3)
---------------------------------------------------------*/

global OBJ_CALL_FUNC(boss_move_17_sakuya_bimyou_idou)
{
	u32 iii;
	iii = ra_nd();
	if (0==(iii&0x07))
	{
		src->math_vector.x256 = (((signed int)((iii>> 8)&0xff))-127);
		src->math_vector.y256 = (((signed int)((iii>>16)&0xff))-127);
	}
	/*(以下は将来的に共通化される予定)*/
	boss_move_00_card_settei_seisei(src);/*(カード生成終了ならカード設定し、生成。)*/
}
#endif

/*---------------------------------------------------------
---------------------------------------------------------*/


/*---------------------------------------------------------
	[ボス共通カード行動]。
---------------------------------------------------------*/


/*---------------------------------------------------------
	ボス移動処理の共通ルーチン
	-------------------------------------------------------
★ 移動範囲の登録、機能：
	あらかじめボス移動範囲を登録しておく。
★ 範囲内ならば移動、機能：
	移動範囲内ならば、ボスが移動する。
	移動範囲外ならば、ボスは移動しない。
★ 移動判定、機能：
	ボスが移動しない場合は、「移動できなかったフラグ」がＯＮになる。
---------------------------------------------------------*/

/*---------------------------------------------------------
	ボス誘導比率計算。
---------------------------------------------------------*/

static OBJ_CALL_FUNC(boss_move_vx_vy_animation)
{
	src->center.x256 += (src->math_vector.x256);
	src->center.y256 += (src->math_vector.y256);
	#define USE_BOSS_HAMIDASI_CHECK (0)
	#if (1==USE_BOSS_HAMIDASI_CHECK)
		 if (src->center.x256 < rect_clip.boss_clip_min.x256 )	{	src->center.x256 = rect_clip.boss_clip_min.x256;	card.boss_hamidasi = (1);	}
	else if (src->center.x256 > rect_clip.boss_clip_max.x256 )	{	src->center.x256 = rect_clip.boss_clip_max.x256;	card.boss_hamidasi = (1);	}
//
		 if (src->center.y256 < rect_clip.boss_clip_min.y256 )	{	src->center.y256 = rect_clip.boss_clip_min.y256;	card.boss_hamidasi = (1);	}
	else if (src->center.y256 > rect_clip.boss_clip_max.y256 )	{	src->center.y256 = rect_clip.boss_clip_max.y256;	card.boss_hamidasi = (1);	}
	#else
	/*(とりあえず)*/
	src->center.x256 = psp_min(src->center.x256, rect_clip.boss_clip_max.x256);/* 右チェック(喰み出す) */
	src->center.x256 = psp_max(src->center.x256, rect_clip.boss_clip_min.x256);/* 左チェック(喰み出す) */
	src->center.y256 = psp_min(src->center.y256, rect_clip.boss_clip_max.y256);/* 下チェック(喰み出す) */
	src->center.y256 = psp_max(src->center.y256, rect_clip.boss_clip_min.y256);/* 上チェック(喰み出す) */
	#endif
//
	/*(攻撃アニメーション指示があれば、攻撃アニメーションを優先する。)*/
	if (0 < src->BOSS_DATA_03_kougeki_anime_count)	/* 攻撃アニメーション */	/* 攻撃アニメーション用カウンタ / 0以下なら移動アニメーション */
	{
		src->BOSS_DATA_03_kougeki_anime_count--;
				if ((32)>src->BOSS_DATA_03_kougeki_anime_count) {	src->obj_type_set = BOSS_09_32; }	/*src->an im_frame = 0x23;*/
		else	if ((40)>src->BOSS_DATA_03_kougeki_anime_count) {	src->obj_type_set = BOSS_10_33; }	/*src->an im_frame = 0x22;*/
		else													{	src->obj_type_set = BOSS_09_32; }	/*src->an im_frame = 0x21;*/
	}
	/*(攻撃アニメーション指示がなければ、移動アニメーションをする。)*/
	else	/* 移動アニメーション */
	{
		s32 aaa;
		aaa = (src->center.x256 > src->BOSS_DATA_00_target_x256)?(BOSS_00_11/*0x00*/):(BOSS_04_21/*0x10*/); /* 左右 */
		s32 bbb;
		bbb = src->BOSS_DATA_04_toutatu_wariai256;
		bbb = psp_min(bbb, (0xff) );
		bbb = 0xff-bbb;
		bbb &= 0xff;
		src->obj_type_set = aaa + ((src->BOSS_DATA_04_toutatu_wariai256>>6) );
	}
}

/*---------------------------------------------------------
	全ボス行動
	(boss->callback_mover)で呼ばれるルート(根源)。
---------------------------------------------------------*/

global OBJ_CALL_FUNC(root_boss_mover)
{
	/*(ボス行動選択処理)*/
	if (NULL != src->callback_root_supeka)
	{
		(*src->callback_root_supeka)(src);
	}
//	card_boss_move_generate_check_regist(src);	/* カード登録可能なら登録 / カード生成 */
	boss_move_vx_vy_animation(src);
	move_card_square_effect(src);				/* 回エフェクト */
	/*(スペル生成処理)*/
	card_generate(src); 						/* スペルをCPU実行し、カードを１フレーム生成する。 */
}
