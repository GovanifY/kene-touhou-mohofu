
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	その他のカードを定義します。
---------------------------------------------------------*/

/*---------------------------------------------------------
	差分氏の妖怪1(天狗様?)が撃つカード
	-------------------------------------------------------
	28休み → 100奇数弾 緑 7方向 BULLET_UROKO14_05_MIDORI		speed256	= t256(3.25)+((difficulty)<<6);
	28休み → 100偶数弾 青 8方向 BULLET_UROKO14_04_MIZUIRO		speed256	= (t256(3.0)+((difficulty)<<6));
	-------------------------------------------------------
	使用レジスタ
	REG_08_REG0 	カウンタ。	(攻撃wait用)	  int YOKAI1_DATA_wait2 = 0;	//(10-1)
	REG_09_REG1 	カウンタ。(色値用)
	REG_0a_REG2 	色値(0か1)
---------------------------------------------------------*/
	//	REG_08_REG0--;
	//	if (0 > REG_08_REG0)
			/* 0: 20 == 20-(0*4) */
			/* 1: 16 == 20-(1*4) */
			/* 2: 12 == 20-(2*4) */
			/* 3:  8 == 20-(3*4) */
		//	REG_08_REG0 = ((20-1)-(((REG_0f_GAME_DIFFICULTY))<<2));/*(8-1)*/ /*(10-1)*/

local OBJ_CALL_FUNC(spell_create_11_tengu_shot)
{
	#if 0/*(なぜかうまくいかない???)*/
	count_up_limit_NUM(REG_NUM_09_REG1, 128);// カウンタ。(色値用)
	if (1==REG_09_REG1) 	/* 1回目( 0 に初期化されて、カウントアップして 1 になるので。) */
	{
		count_up_limit_NUM(REG_NUM_0a_REG2, 2);// (色値)
	}
	#else
	if (128 > ((REG_10_BOSS_SPELL_TIMER)))
			{	REG_0a_REG2 = (1);	}/* 偶数弾 青 8方向 */
	else	{	REG_0a_REG2 = (0);	}/* 奇数弾 緑 7方向 */
	#endif
	/* 初めの 28[frame]はお休み */
	if (100 > ((REG_10_BOSS_SPELL_TIMER)&0x7f))
	{
		count_up_limit_NUM(REG_NUM_08_REG0, ((20-1)-(((REG_0f_GAME_DIFFICULTY))<<2)));//	if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
		if (0==(REG_08_REG0))
		{
			/*(ボススペカの場合のみ省略可能、中ボスの場合省略できない)*/set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
			calculate_jikinerai();
			//
		//	HATSUDAN_01_speed256			= (t256(3.0)+(((REG_0f_GAME_DIFFICULTY))<<6))+(REG_0a_REG2<<(8-2));
			HATSUDAN_01_speed256			= (t256(2.0)+(((REG_0f_GAME_DIFFICULTY))<<6))+(REG_0a_REG2<<(8-2));
			HATSUDAN_02_speed_offset		= t256(3/*6*/);/*(テスト)*/
		//	HATSUDAN_03_angle65536			= ((HATSUDAN_03_angle65536));
		//	HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_1000_EFFECT_MIDDLE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_UROKO14_BASE + TAMA_IRO_04_MIZU_IRO) + (REG_0a_REG2);
			HATSUDAN_06_n_way				= (7)+(REG_0a_REG2);
			HATSUDAN_07_div_angle65536		= (int)(65536/24);
			hatudan_system_regist_n_way();/* (r33-) */
			AUDIO_18_voice_number	= VOICE15_BOSS_KOUGEKI_01;
			AUDIO_19_voice_truck	= TRACK04_TEKIDAN;/* テキトー */
			cpu_voice_play();
		}
	}
}


/*---------------------------------------------------------
	アリス normal通常攻撃3(の一部)
	-------------------------------------------------------
	なんか全然まったく似てない。
	妖々夢風のつもりで創っていたが怪綺談風になった。
	何を言ってるのかわからないと思うが、わたしにもわからないや。
	恐ろしいものの片鱗を以下略。
	psp解像度が全然違うから、無理に似せようとしない方が良いかも？
---------------------------------------------------------*/

/* [CW 時計回り] */
local OBJ_CALL_FUNC(spell_create_20_sonota_debug_cw_ao/*CW*/)
{
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
	{
		src->tmp_angleCCW65536 -= (int)(65536/24);
		#if 0
		src->tmp_angleCCW65536 += (int)(ra_nd() & ((1<<difficulty)-1) );
		#else
		{static const u8 masked_tbl[4] = { 0x00, 0x0f, 0x3f, 0xff };
			src->tmp_angleCCW65536 += (int)(ra_nd() & (masked_tbl[(REG_0f_GAME_DIFFICULTY)]) );
		}
		#endif
		mask65536(src->tmp_angleCCW65536);
		//
			HATSUDAN_01_speed256				= (t256(1.0));	/* 弾速 */
			HATSUDAN_02_speed_offset			= t256(1);/*(テスト)*/
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_WAKU12_BASE + TAMA_IRO_03_AOI);				/* [枠付き青丸弾] */
			HATSUDAN_06_n_way					= (6);									/* [6way] */
			HATSUDAN_07_div_angle65536			= (int)(65536/48);						/* 分割角度(1024/(6*8) ) １周を8分割した領域をさらに6分割した角度 */
		{
			HATSUDAN_03_angle65536				= (src->tmp_angleCCW65536); 							/* 角度 */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		//
			HATSUDAN_03_angle65536				= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));		/* 角度 */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
		#if (1)
		AUDIO_18_voice_number	= VOICE15_BOSS_KOUGEKI_01;
	//	AUDIO_19_voice_truck	= TRACK04_TEKIDAN;
		cpu_bullet_play_05_auto();
		#endif
	}
}

/*[CCW 反時計回り]*/
local OBJ_CALL_FUNC(spell_create_21_sonota_debug_ccw_aka/*CCW*/)
{
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
	{
		src->tmp_angleCCW65536 += (int)(65536/24);
		#if 0
		src->tmp_angleCCW65536 -= (int)(ra_nd() & ((1<<difficulty)-1) );
		#else
		{static const u8 masked_tbl[4] = { 0x00, 0x0f, 0x3f, 0xff };
			src->tmp_angleCCW65536 -= (int)(ra_nd() & (masked_tbl[(REG_0f_GAME_DIFFICULTY)]) );
		}
		#endif
		mask65536(src->tmp_angleCCW65536);
		//
			HATSUDAN_01_speed256				= (t256(1.0));	/* 弾速 */
			HATSUDAN_02_speed_offset			= t256(1);/*(テスト)*/
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_WAKU12_BASE + TAMA_IRO_01_AKA);				/* [枠付き赤丸弾] */
			HATSUDAN_06_n_way					= (6);									/* [6way] */
			HATSUDAN_07_div_angle65536			= (int)(65536/48);						/* 分割角度(1024/(6*8) ) １周を8分割した領域をさらに6分割した角度 */
		{
			HATSUDAN_03_angle65536				= (src->tmp_angleCCW65536); 							/* 角度 */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		//
			HATSUDAN_03_angle65536				= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));		/* 角度 */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
		#if (1)
		AUDIO_18_voice_number	= VOICE15_BOSS_KOUGEKI_01;
	//	AUDIO_19_voice_truck	= TRACK04_TEKIDAN;
		cpu_bullet_play_05_auto();
		#endif
	}
}

/*---------------------------------------------------------
	子供魔方陣 カード
	-------------------------------------------------------
	B.星型描くぜ
	-------------------------------------------------------
	(無駄に複雑だったので、
		A.レーザーを撃つ機能と、
		B.星型を描く機能と、
		C.360ナイフを撃つ機能と、
	を分離させた)
---------------------------------------------------------*/

#define REG_000_my_angle1024				REG_09_REG1 	/* 保持角度[星型を描く場合に使う角度] */
#define REG_001_hosi_set_position_cx256 	REG_0a_REG2 	/* 星型描いてる座標位置を保持。 */
#define REG_002_hosi_set_position_cy256 	REG_0b_REG3 	/* 星型描いてる座標位置を保持。 */
#define REG_003_hosi_vx256					REG_0c_REG4 	/* 星型の差分移動ベクトル量。vx256 */
#define REG_004_hosi_vy256					REG_0d_REG5 	/* 星型の差分移動ベクトル量。vy256 */

#define HGT_giji_jyumyou	base_time_out

/* 応用弾 */
#define HGT_DATA_angle1024			user_data03
#define HGT_DATA_speed256			user_data04 		/* 速度 */
#define HGT_DATA_add_delta256		user_data05 		/* 加算、加速度 */

#define HOSIGATA_DAN_LIMIT_01_512	(512+0)
static OBJ_CALL_FUNC(move_bullet_sakuya_hosi_gata)
{
	src->HGT_giji_jyumyou--;/* 時間経過 */
	if (HOSIGATA_DAN_LIMIT_01_512 > (src->HGT_giji_jyumyou))/* 予め設定された時間で動く */
	{
		src->HGT_giji_jyumyou = HOSIGATA_DAN_LIMIT_01_512;
		src->HGT_DATA_speed256 += (src->HGT_DATA_add_delta256); //(4) (3.84) == t256(0.015);
		//------------------
		HATSUDAN_01_speed256	= (src->HGT_DATA_speed256);
		HATSUDAN_03_angle65536	= deg1024to65536((((src->HGT_DATA_angle1024))));
		sincos256();/*(破壊レジスタ多いので注意)*/
		src->math_vector.x256 = REG_03_DEST_Y;//sin_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
		src->math_vector.y256 = REG_02_DEST_X;//cos_value_t256 // 下CCWの場合(右CWの場合ととxyが逆)
		//------------------
	}
	//move_bullet_vector(src);
	src->center.x256 += (src->math_vector.x256); 	/*fps_factor*/
	src->center.y256 += (src->math_vector.y256); 	/*fps_factor*/
//	gamen_gai_nara_osimai(src);/* 画面外ならおしまい */
	hatudan_system_B_gamen_gai_tama_kesu(src);/*(画面外弾消し)*/
}
		#if (0)//
		src->math_vector.x256 = ((si n1024((src->HGT_DATA_angle1024))*(src->HGT_DATA_speed256))>>8);/*fps_factor*/ 	/* CCWの場合 */
		src->math_vector.y256 = ((co s1024((src->HGT_DATA_angle1024))*(src->HGT_DATA_speed256))>>8);/*fps_factor*/
		//#el se
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_si nco s1024( (src->HGT_DATA_angle1024), &sin_value_t256, &cos_value_t256);
			src->math_vector.x256 = ((sin_value_t256*(src->HGT_DATA_speed256))>>8);/*fps_factor*/
			src->math_vector.y256 = ((cos_value_t256*(src->HGT_DATA_speed256))>>8);/*fps_factor*/
		}
		#endif

/*
	-------------------------------------------------------
	ToDo:
	弾システムに移行する為、廃止予定。
	カードスクリプトシステムが稼動する為には、発弾を一ヶ所に統合しないと
	速度的な面でこのままの形態では無理があるので、
	(ここの発弾処理のせいで他の弾が処理落ちする)
	ここの発弾処理を hatudan_system_regist_single に置きかえる必要がある。
---------------------------------------------------------*/



/*---------------------------------------------------------
	咲夜の		B.星型を描く魔方陣用 弾
	-------------------------------------------------------
	弾を全弾書き換え方式に移行するのでここは無くなる予定。
	-------------------------------------------------------
	Todo:
	src->giji_jyumyou を廃止して src->jyumyou で置き換える。
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_init_27_hosigata_test)
{
	spell_init_mima_kaguya(src);/*(特殊)*/	/*(DANMAKU01に雨用を割り当てる)*/
		/* 弾を撃ち始める位置(星型描き始める位置) */
		REG_001_hosi_set_position_cx256 	= (src->center.x256)+(ra_nd()&0xfff);
		REG_002_hosi_set_position_cy256 	= (src->center.y256)+(ra_nd()&0xfff);/* 咲夜 下方から描く */

		/* 星型を描く準備 */
		REG_000_my_angle1024			= (0);
	//	REG_003_hosi_vx256				= t256(1.0);	/*-t256(0.125)*/ /*t256(1.0)*/
	//	REG_004_hosi_vy256				= t256(0.0);	/* t256(2.0)*/ /*t256(0.0)*/
}


/*---------------------------------------------------------
	子供魔方陣 移動
---------------------------------------------------------*/
		/* 子供魔方陣、配置位置 */
		#if 0
		h->center.x256					= (src->center.x256);
		h->center.y256					= (src->center.y256)-t256(16);/*咲夜 上方に配置*/
		#endif
	//
//		h->boss_time_out			= ((64*8)+(5*16)+1);	/* 制限時間 */

local OBJ_CALL_FUNC(spell_create_27_hosigata_test)
{
//	if ((64*8) < src->boss_time_out)
	{
		/* 星型を描くよ */
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			REG_000_my_angle1024 += (1024*2/5);
			mask1024(REG_000_my_angle1024);
			#define HGT_DOLL_SPEED10		(4)/*8*/
			#if (0)//
			REG_003_hosi_vx256 = ((si n1024((REG_000_my_angle1024))*(HGT_DOLL_SPEED10)));/*fps_factor*/ /* CCWの場合 */
			REG_004_hosi_vy256 = ((co s1024((REG_000_my_angle1024))*(HGT_DOLL_SPEED10)));/*fps_factor*/
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (REG_000_my_angle1024), &sin_value_t256, &cos_value_t256);
				REG_003_hosi_vx256 = ((sin_value_t256*(HGT_DOLL_SPEED10)));/*fps_factor*/
				REG_004_hosi_vy256 = ((cos_value_t256*(HGT_DOLL_SPEED10)));/*fps_factor*/
			}
			#endif
			#undef HGT_DOLL_SPEED10
		}
		else
		{
/*
0123 3333 4444	0+4
4567 7777 5555	1+4
89ab bbbb 6666	2+4
cdef ffff 7777	3+4
*/
			/* CCWの場合 */
		//	bullet_crate_sakuya_hosi_gata(/*&hosi_position_obj*/);
		//	static void bullet_crate_sakuya_hosi_gata(void/*OBJ *src*/)
			{		/*丸弾８(赤ＲＧＢ緑若黄青)*/
				OBJ *h;
				h									= obj_regist_tama();
				if (NULL != h)
				{
					h->center.x256					= REG_001_hosi_set_position_cx256;
					h->center.y256					= REG_002_hosi_set_position_cy256;
					h->obj_type_set 				= (BULLET_MINI8_BASE + TAMA_IRO_01_AKA);
					reflect_sprite_spec(h, OBJ_BANK_SIZE_00_TAMA);
					h->m_Hit256R					= TAMA_ATARI_MARU16_PNG;
				//
					h->callback_mover				= move_bullet_sakuya_hosi_gata;
					h->hatudan_register_speed65536	= (0);
					h->hatudan_register_tra65536	= (0);
					h->hatudan_register_spec_data	= (DANMAKU_LAYER_03)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_8000_NON_TILT);
					h->math_vector.x256				= (0);
					h->math_vector.y256				= (0);
					/* hosi_gata_time_out */
					h->HGT_giji_jyumyou 			= HOSIGATA_DAN_LIMIT_01_512 + ((REG_10_BOSS_SPELL_TIMER)&0x1ff); //((REG_10_BOSS_SPELL_TIMER)-(64*8))
					/* hosi_gata_angle1024 */
					h->HGT_DATA_angle1024			= ((REG_000_my_angle1024))+(((REG_10_BOSS_SPELL_TIMER)&0x0f)<<4/*3*/);
					h->HGT_DATA_speed256			= t256(0);
					/* hosi_gata_add_speed256 */	//(((REG_10_BOSS_SPELL_TIMER)&0xf)|0x03);	/* 加加速度(か-かそくど) */
					h->HGT_DATA_add_delta256		= (((REG_10_BOSS_SPELL_TIMER>>2)&0x3)+4);	/* 加加速度(か-かそくど) */
				}
			}
		}
		// 動作
		REG_001_hosi_set_position_cx256 += (REG_003_hosi_vx256);/*fps_factor*/
		REG_002_hosi_set_position_cy256 += (REG_004_hosi_vy256);/*fps_factor*/
	}
//
}
