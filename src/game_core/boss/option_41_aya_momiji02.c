
#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	カード
	-------------------------------------------------------
	ボス共通規格使用データー:
		BOSS_DATA_05_move_jyumyou	user_data05 	たぶん。経過時間。
---------------------------------------------------------*/

#define MOMIJI_KODOMO_DATA_angle1024			user_data00 	/*union BULLET_ANGLE_DATA*/
#define MOMIJI_KODOMO_DATA_speed256 			user_data01 	/*union BULLET_ANGLE_DATA*/
//
// バグ対策。 user_data02 良くワカンナイけど空けとく。()

//
#define MOMIJI_KODOMO_DATA_angle222_1024		user_data03
#define MOMIJI_KODOMO_DATA_adjust_speed256		user_data07


/*---------------------------------------------------------
	分裂(last)
---------------------------------------------------------*/

static OBJ_CALL_FUNC(move_aya_doll_last_burrets)
{
//	/*[r35要半分速]*/HATSUDAN_01_speed256	= (t256(0.75)+(ra_nd()&0xff));					/* 弾速 */
	/*[r35半分速]*/HATSUDAN_01_speed256 	= (t256(0.375)+(ra_nd()&0xff)); 				/* 弾速 */
	HATSUDAN_02_speed_offset				= (2);/*(とりあえず)*/
	HATSUDAN_04_tama_spec					= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
	HATSUDAN_03_angle65536					= ((ra_nd()&(65536-1)));						/* src->tmp_angleCCW1024 */
	HATSUDAN_05_bullet_obj_type 			= (BULLET_KOME_BASE + TAMA_IRO_01_AKA) + (((REG_0f_GAME_DIFFICULTY)&0x02)<<1);	/* [赤(0x01)/緑(0x05)米弾] */
	HATSUDAN_06_n_way						= (1+(REG_0f_GAME_DIFFICULTY)); 				/* [7way] [8way] */
	HATSUDAN_07_div_angle65536				= (int)(65536/23);								/* 角度 (65536/27) (65536/24) */
	set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
	hatudan_system_regist_n_way();/* (r33-) */
//	bullet_regist_multi_vector_direct();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static OBJ_CALL_FUNC(move_bullet_momiji)
{
	src->BOSS_DATA_05_move_jyumyou--;
	if ((0 > src->BOSS_DATA_05_move_jyumyou))
	{
		move_aya_doll_last_burrets(src);
		src->jyumyou = JYUMYOU_NASI;	/* おしまい */
	}
	else
	{
		if (src->MOMIJI_KODOMO_DATA_speed256 < t256(0.5) )
		{
			src->MOMIJI_KODOMO_DATA_angle222_1024 += (1024/2);	/* (1024/2) == 半周 == [180/360 度] */
			#if 1
			/* ねんのため */
			mask1024(src->MOMIJI_KODOMO_DATA_angle222_1024);
			#endif
			src->MOMIJI_KODOMO_DATA_angle1024	= src->MOMIJI_KODOMO_DATA_angle222_1024;
			src->MOMIJI_KODOMO_DATA_adjust_speed256 = (-(src->MOMIJI_KODOMO_DATA_adjust_speed256));
		}
		src->MOMIJI_KODOMO_DATA_speed256 += src->MOMIJI_KODOMO_DATA_adjust_speed256;
		if (src->MOMIJI_KODOMO_DATA_angle1024-src->MOMIJI_KODOMO_DATA_angle222_1024 < (int)(1024*1/12) )
		{
			src->MOMIJI_KODOMO_DATA_angle1024 += (1+1);/*0.01*/
		}
		#if 1
		/* ねんのため */
		mask1024(src->MOMIJI_KODOMO_DATA_angle1024);
		#endif
	//	mono_angle_move(src,(BULLET_ANGLE_DATA *)data);
		#if (0)//
		src->math_vector.x256			= ((si n1024((src->MOMIJI_KODOMO_DATA_angle1024))*(src->MOMIJI_KODOMO_DATA_speed256))>>8);/*fps_factor*/	/* CCWの場合 */
		src->math_vector.x256			= ((si n1024((src->MOMIJI_KODOMO_DATA_angle1024))*(src->MOMIJI_KODOMO_DATA_speed256))>>8);/*fps_factor*/
		src->math_vector.y256			= ((co s1024((src->MOMIJI_KODOMO_DATA_angle1024))*(src->MOMIJI_KODOMO_DATA_speed256))>>8);/*fps_factor*/
		src->math_vector.y256			= ((co s1024((src->MOMIJI_KODOMO_DATA_angle1024))*(src->MOMIJI_KODOMO_DATA_speed256))>>8);/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (src->MOMIJI_KODOMO_DATA_angle1024), &sin_value_t256, &cos_value_t256);
			src->math_vector.x256			= ((sin_value_t256*(src->MOMIJI_KODOMO_DATA_speed256))>>8);/*fps_factor*/
			src->math_vector.y256			= ((cos_value_t256*(src->MOMIJI_KODOMO_DATA_speed256))>>8);/*fps_factor*/
		}
		#endif
		src->center.x256 += (src->math_vector.x256);/*fps_factor*/
		src->center.y256 += (src->math_vector.y256);/*fps_factor*/

	}
}

/*---------------------------------------------------------
	5方向もみじ小弾を生成
---------------------------------------------------------*/
#define MOMIJI_KODOMO_next_angle1024	tmp_angleCCW1024	/* 子供弾、生成角度 */

/*static*/ OBJ_CALL_FUNC(add_zako_aya_5_momiji)
{
	int angle1024;
	angle1024 = (src->MOMIJI_KODOMO_next_angle1024)-((int)(1024*1/20));
//
	int jj;
	for (jj=(0); jj<(5); jj++)
	{
		OBJ *h;
		h							= obj_regist_teki();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->obj_type_set 		= (TEKI_32_20)+((angle1024>>7)&0x07);/* 0 ... 8 */
			h->atari_hantei 		= (1/*スコア兼用*/);
			h->callback_mover		= move_bullet_momiji;
			h->BOSS_DATA_05_move_jyumyou			= (50); 	/* 200寿命 */
			{
				/* 初期位置 */
				h->center.x256			= src->center.x256;
				h->center.y256			= src->center.y256;
			}
		//
			h->MOMIJI_KODOMO_DATA_angle222_1024 	= (angle1024);
			h->MOMIJI_KODOMO_DATA_angle1024 		= (angle1024);
			h->MOMIJI_KODOMO_DATA_speed256			= t256(3.0);
			h->MOMIJI_KODOMO_DATA_adjust_speed256	= t256(-0.04);
		//
			angle1024 += (int)(1024*1/20);/*offset_angle*/
			mask1024(angle1024);
		}
	}
}
