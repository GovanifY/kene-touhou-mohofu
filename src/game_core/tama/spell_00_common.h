
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	共通部分。
	-------------------------------------------------------
	(r32-)喰み出しチェックについて、
	「移動時に」喰み出しチェックをしない事が前提で設計されているカードが多い。
	この為「移動時に」システムで喰み出しチェックをしません。
	例えば画面端で跳ね返る弾は「移動時に」喰み出しチェックをしない事により実現している機能です。
	「移動と別で」全弾配列を調べて「喰み出しチェック」を行います。
---------------------------------------------------------*/

/*---------------------------------------------------------
	交差弾は色々弾によって違いが多すぎる。(個々の時間とか)
	全部システム側で吸収すると、遅くなる気がする。
	交差弾システム側で吸収するなら、角度弾と統合して角度弾無くすべき
	だけど(将来はともかく)現状では角度弾汎用にすると遅すぎる。
	-------------------------------------------------------
	現状の速度なら、交差弾は全弾配列書き換えで対応すべき。
	(パチェとか書き換えの嵐になりそうですが。)
	システム側交差弾対応は、無くなりそう。
---------------------------------------------------------*/

#if (1)
	/*(tama_system.cの特殊機能)*/
	#define hatudan_system_kousadan_angle65536 tmp_angleCCW1024
#endif
/*---------------------------------------------------------
	#001 角度弾の移動を行う(通常弾用)
---------------------------------------------------------*/

static OBJ_CALL_FUNC(hatudan_system_B_move_angle_001)/*(角度弾移動)*/
{
	#if 1/* 検討中(無くても出来るけど、あった方が簡単) */
	src->hatudan_register_speed65536		+= (src->hatudan_register_tra65536);		/* 加減速調整 */
	#endif
	/*( 1[frame]あたりに進む距離。(半径) )*/int aaa 		 = (src->hatudan_register_speed65536>>8);	/* 速度 */
//
	#if (0)//
	src->center.x256 += ((si n1024((src->rotationCCW1024))*(aaa))>>8);/*fps_factor*/
	src->center.y256 += ((co s1024((src->rotationCCW1024))*(aaa))>>8);/*fps_factor*/
	#else
	{
		int sin_value_t256; 	//	sin_value_t256 = 0;
		int cos_value_t256; 	//	cos_value_t256 = 0;
		int256_sincos1024( (src->rotationCCW1024), &sin_value_t256, &cos_value_t256);
		src->center.x256 += ((sin_value_t256*(aaa))>>8);/*fps_factor*/
		src->center.y256 += ((cos_value_t256*(aaa))>>8);/*fps_factor*/
	}
	#endif
}

static OBJ_CALL_FUNC(danmaku_00_standard_angle_mover)/*(角度弾移動+画面外弾消し)*/
{
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
	hatudan_system_B_gamen_gai_tama_kesu(src);/*(画面外弾消し)*/
}
static OBJ_CALL_FUNC(danmaku_01_standard_angle_sayuu_hansya_mover)/*(角度弾移動+画面左右反射弾消し)*/
{
	hatudan_system_B_move_angle_001(src);/*(角度弾移動)*/
	hatudan_system_B_side_hansya(src);/*(画面左右反射弾消し)*/
}
local OBJ_CALL_FUNC(angle_to_vector)/*(角度をX軸、Y軸のベクトル速度へ変換する)*/
{
	//	REG_0b_REG3 = ((src->rotationCCW1024)<<6);/*1024to65536*/
		REG_0b_REG3 = ((src->hatudan_system_kousadan_angle65536));/*(交差弾用に発弾時の角度が65536であるので使う。)*/
	//	REG_0b_REG3 = (src->rotationCCW1024);
//		REG_0b_REG3 = ((HATSUDAN_03_angle65536));
		#if (0)
		// ベクトル弾登録。 regist vector.
		sincos65536_NUM(REG_NUM_0b_REG3, REG_NUM_0c_REG4);
		REG_02_DEST_X	 = (((REG_0c_REG4)*(REG_09_REG1))>>8);
		REG_03_DEST_Y	 = (((REG_0d_REG5)*(REG_09_REG1))>>8);
		//
		#else
		{
//			unsigned int angle1024;
//		//	angle1024 = HATSUDAN_03_angle65536;//spell_register[register_number_angle];
//			angle1024 = REG_0b_REG3;//spell_register[register_number_angle];
//			angle1024 = deg65536to1024(angle1024);	/*(65536分割を1024分割に変更)*/

			#if (0)//
			spell_register[register_number_value]		= (((si n1024((angle1024))) ) );		/* CCWの場合 */
			spell_register[register_number_value+1] 	= (((co s1024((angle1024))) ) );
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (REG_0b_REG3>>6), &sin_value_t256, &cos_value_t256);
				/*	spell_register[register_number_value]	*/	REG_0c_REG4 = (sin_value_t256);
				/*	spell_register[register_number_value+1] */	REG_0d_REG5 = (cos_value_t256);
				REG_02_DEST_X	= (((REG_0c_REG4)*(REG_09_REG1))>>8);
				REG_03_DEST_Y	= (((REG_0d_REG5)*(REG_09_REG1))>>8);
			}
			#endif
		}
		#endif
	//	src->rotationCCW1024	= (0);/*(重いので角度０にする)*/
		src->math_vector.x256 = (REG_02_DEST_X);/*fps_factor*/
		src->math_vector.y256 = (REG_03_DEST_Y);/*fps_factor*/
}


local OBJ_CALL_FUNC(move_vector_gamen_sita)
{
	// ベクトル弾移動。move vector.
	src->center.x256 += (src->math_vector.x256);	/*fps_factor*/
	src->center.y256 += (src->math_vector.y256);	/*fps_factor*/
//	src->center.x256 += 1;//(t256( 0.1));	/*fps_factor*/
//	src->center.y256 += 1;//(t256(-0.1));	/*fps_factor*/
//	src->math_vector.y256 += ((src->hatudan_register_tra65536)<<8);	/* 加減速調整 */
	src->math_vector.y256 += ((src->hatudan_register_tra65536) ); /* 加減速調整 */
	// 消去判定
	#if 1
	// 画面外は消す。
//	hatudan_system_B_gamen_gai_tama_kesu(src);/*(通常弾用)*/
	/* 画面外の場合は弾を消す。 */
	if (
//	(0 < (rect_clip.bullet_clip_min.x256)-(src->center.x256) ) ||
//	(0 > (rect_clip.bullet_clip_max.x256)-(src->center.x256) ) ||
//	(0 < (rect_clip.bullet_clip_min.y256)-(src->center.y256) ) ||
//	(0 > (rect_clip.bullet_clip_max.y256)-(src->center.y256) ) )	/*(画面下のみ)*/
//	( (src->center.y256) > (rect_clip.bullet_clip_max.y256) ) ) /*(画面下のみ)*/
	( (src->center.y256) > (t256(272)) ) )	/*(画面下のみ)*/
	{
		src->jyumyou = JYUMYOU_NASI;
	}
	#endif
}

/* HATUDAN_ITI_NO_JIKAN==発弾エフェクト後から[0]カウント経過した弾。発弾エフェクト時間==[64]カウント。 */
#define HATUDAN_ITI_NO_JIKAN	(JYUMYOU_1MIN-64)

/*---------------------------------------------------------
	雨降り弾
	-------------------------------------------------------
	使用レジスタ
//	REG_08_REG0 	カウンタ。
	REG_09_REG1 	[定数1]雨の速度
//	REG_0a_REG2 	[定数2]赤青クナイが曲がる角度(180/360ちょい回転)
	-------------------------------------------------------
	sincos計算用
	REG_0b_REG3 	angle_65536
	REG_0c_REG4 	sine   の値用。
	REG_0d_REG5 	cosine の値用。
---------------------------------------------------------*/

/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(common_danmaku_01_amefuri_callback)
{
	#if (1)
	if ((HATUDAN_ITI_NO_JIKAN-64) < src->jyumyou)/* 発弾エフェクト時は無効 */
	{
		return;
	}
	#endif
	#if (1)
	if ((HATUDAN_ITI_NO_JIKAN-64) == src->jyumyou)/* 発弾エフェクト後から64カウント経過した弾 */
	{
		angle_to_vector(src);
	}
	#endif
	move_vector_gamen_sita(src);/*(ベクトル移動+画面下消去)*/
}
/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(meirin_danmaku_02_aka_ao_kunai_callback);
local OBJ_CALL_FUNC(meirin_danmaku_03_aka_ao_kunai_time256_callback);
local OBJ_CALL_FUNC(spell_init_12_common_amefuri)
{
	REG_09_REG1 	= (t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<6));//[定数1]雨の速度
	REG_0a_REG2 	= ((1024/2)+(1024/24)+(REG_0f_GAME_DIFFICULTY<<3));//[定数2]赤青クナイが曲がる角度
	card.danmaku_callback[1] = common_danmaku_01_amefuri_callback;/*(雨用)*/
	card.danmaku_callback[2] = meirin_danmaku_02_aka_ao_kunai_callback;/*(赤青クナイ用1)*/
	card.danmaku_callback[3] = meirin_danmaku_03_aka_ao_kunai_time256_callback;/*(赤青クナイ用2)*/
}

/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_common_amefuri)
{
	#if 0
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))	// 4カウントに1回上に8way弾を撃つ
	{
			HATSUDAN_01_speed256				= (REG_09_REG1);
			HATSUDAN_05_bullet_obj_type 		= (BULLET_KOME_BASE + TAMA_IRO_03_AOI); 	/* [青白米弾] */	/* 弾グラ */
	//		HATSUDAN_06_n_way					= (8);
//			HATSUDAN_07_div_angle65536			= (int)(65536/64);
		unsigned int j;
		for (j=(0); j<(8); j++)
		{
			/* ショット */
		//	b05_fire_flags &= (~(doll_data->identity_bit));/* off */
			HATSUDAN_03_angle65536		= (65536/4)+(65536/8)+(j<<(5+6))+((REG_10_BOSS_SPELL_TIMER>>(2-6))&((0x3f)<<6));
			HATSUDAN_07_VECTOR_leg acy_dan_delta256 = ((ra_nd()&0x03)+2);//t256(0.04)
			bul let_reg ist_leg acy_vec tor_dir ect();
		}
	}
	#endif
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))	// 4カウントに1回上に8way弾を撃つ
		{
				HATSUDAN_01_speed256				= REG_09_REG1;// [定数1]雨の速度
				#if 0
					/*古い*/
				#else
					/*新しい*/
//				HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|/*(TAMA_SPEC_4000_NON_MOVE)|*/(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
//				HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
				HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
				#endif
				HATSUDAN_05_bullet_obj_type 		= (BULLET_KOME_BASE + TAMA_IRO_00_SIRO);	/* 弾グラ */	/* [白米弾] */
			unsigned int jj;
			for (jj=(0); jj<(8); jj++)
			{
				#if 0
				/* ショット */
			//	b05_fire_flags &= (~(doll_data->identity_bit));/* off */
				HATSUDAN_03_angle65536				= (65536/4)+(65536/8)+(jj<<(5+6))+((REG_10_BOSS_SPELL_TIMER>>(2-6))&((0x3f)<<6));
	//			HATSUDAN_06_n_way					= (8);
//				HATSUDAN_07_div_angle65536					= (int)(65536/64);
				HATSUDAN_07_VECTOR_leg acy_dan_delta256 	= ((ra_nd()&0x03)+2);//t256(0.04)
				bul let_reg ist_leg acy_vec tor_dir ect();
				#else
				/* ショット */
			//	b05_fire_flags &= (~(doll_data->identity_bit));/* off */
				HATSUDAN_02_speed_offset			= ((ra_nd()&0x03)+2);//t256(0.04)
			//	HATSUDAN_03_angle65536				= (65536/4)+(65536/8)+(jj<<(5+6))+((REG_10_BOSS_SPELL_TIMER<<(6-2))&(0x3f<<6));
	//			int aaa_1024;
	//							aaa_1024				= (1024/4)+(1024/8)+(jj<<5)+((REG_10_BOSS_SPELL_TIMER>>2)&0x3f);
	//			HATSUDAN_03_angle65536				= ((aaa_1024)<<6);

			//	HATSUDAN_03_angle65536				= (65536/4)+(65536/8)+(jj<<(11));//+((REG_10_BOSS_SPELL_TIMER<<(6-2))&(0x0fff));
		//		HATSUDAN_03_angle65536				= (65536/4)+(65536/8)+(jj<<(11))+((REG_10_BOSS_SPELL_TIMER<<(4))&(0x0fff));
			//	HATSUDAN_03_angle65536				= (65536/4)+(65536/8)+(jj<<(11))+((REG_10_BOSS_SPELL_TIMER<<(5))&(0x0fff));
				HATSUDAN_03_angle65536				= (65536/4)+(65536/8)+(jj<<(11))+((REG_10_BOSS_SPELL_TIMER<<(4))&(0x0fff));

	//			HATSUDAN_06_n_way					= (8);
//				HATSUDAN_07_div_angle65536			= (int)(65536/64);
				hatudan_system_regist_single();
				#endif
			}
		}
}

/*---------------------------------------------------------
	[初期化セクション]
	-------------------------------------------------------
	とりあえず共通規格
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_mima_kaguya)
{
	REG_09_REG1 	= (t256(1.0));//[定数1]雨の速度
//	REG_0a_REG2 	= ((1024/2)+(1024/24)+(REG_0f_GAME_DIFFICULTY<<3));//[定数2]赤青クナイが曲がる角度
	card.danmaku_callback[1] = common_danmaku_01_amefuri_callback;/*(雨用)*/
//	card.danmaku_callback[2] = NULL;/*(未使用)*/
//	card.danmaku_callback[3] = NULL;/*(未使用)*/
}

/*---------------------------------------------------------
	[発弾セクション]
	-------------------------------------------------------
	使用レジスタ
	REG_0e_REG6 	src_shot_angle65536 開始地点
---------------------------------------------------------*/
global/*local*/ OBJ_CALL_FUNC(shot_common_gin_tama)
{
	#if (1)/*(共通部分)*/
	HATSUDAN_01_speed256					= t256(1.0);//t256(1+difficulty)/*(3+difficulty)*/ /*(4+difficulty)*/;
//	HATSUDAN_02_speed_offset				= ((ra_nd()&0x03)+1);//t256(0.04)/*10*/
	HATSUDAN_02_speed_offset				= ((ra_nd()&0x03)+2);//t256(0.04)
//	HATSUDAN_03_angle65536					= (src->shot_angle65536&(256*64-1))+512*64+128*64;// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
//	HATSUDAN_03_angle65536					= (src->shot_angle65536&(256*64-1))+(65536/2)+(65536/8);// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
	HATSUDAN_03_angle65536					= (REG_0e_REG6&((65536/4)-1))+(65536/4)+(65536/8);// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
	HATSUDAN_04_tama_spec					= (DANMAKU_LAYER_01)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
	HATSUDAN_05_bullet_obj_type 			= (BULLET_MINI8_BASE + TAMA_IRO_03_AOI);	/* 弾グラ */
	hatudan_system_regist_single();
	#endif
}
	#if (0)/*(旧)*/
	HATSUDAN_01_speed256					= t256(1.0);//t256(1+difficulty)/*(3+difficulty)*/ /*(4+difficulty)*/;
//	HATSUDAN_03_angle65536					= (((src->shot_angle65536&(256*64-1))+512*64+128*64));// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
	HATSUDAN_03_angle65536					= (((REG_0e_REG6&(256-1))-512-128)<<6);// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
	HATSUDAN_05_bullet_obj_type 			= (BULLET_MINI8_BASE + TAMA_IRO_03_AOI);	/* 弾グラ */
//	HATSUDAN_06_n_way						= (8);
//	HATSUDAN_07_VECTOR_leg acy_dan_delta256 = ((ra_nd()&0x03)+1);//t256(0.04)/*10*/
	HATSUDAN_07_VECTOR_leg acy_dan_delta256 = ((ra_nd()&0x03)+2);//t256(0.04)
	bullet_regist_leg acy_vector_direct();
	#endif

#if (1)/*(必要)*/
/*---------------------------------------------------------
	[r34互換機能]。
	-------------------------------------------------------
	ここは何も処理を行いませんが、r35現在、必要です。
	-------------------------------------------------------
	(r35現在)文と咲夜でこの機能(r34互換機能)を使用している為に必要。
	ここが無いとr35の仕様上、通常弾が撃てない。(通常弾の処理が正常に行えない)
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_48_r34_gokan_kinou)
{
	/* (ここは何も処理を行いませんが、r35現在、必要です) */
	// (r35-)システム移行したので、(r35-)システムの都合上、(-r34まで)のスペカを再現する為に必要なダミー。
};
#endif

