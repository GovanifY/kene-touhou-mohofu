
#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	もみじカード
	-------------------------------------------------------
	ボス共通規格使用データー:
		BOSS_DATA_05_move_jyumyou	user_data05 	たぶん。経過時間。
---------------------------------------------------------*/

// バグ対策。 user_data02 良くワカンナイけど空けとく。()

#define MOMIJI_KODOMO_next_angle1024	tmp_angleCCW1024					/* 子供弾、生成角度 */


/*---------------------------------------------------------

---------------------------------------------------------*/
/*static*/extern OBJ_CALL_FUNC(add_zako_aya_5_momiji);
static OBJ_CALL_FUNC(move_aya_momiji_oya)
{
	src->BOSS_DATA_05_move_jyumyou--;/*fps_factor*/
	if ((0 > src->BOSS_DATA_05_move_jyumyou))
	{
		add_zako_aya_5_momiji(src); 	/* 5方向もみじ小弾を生成 */
		src->jyumyou = JYUMYOU_NASI;
	}
	src->center.x256 += (src->math_vector.x256);/*fps_factor*/
	src->center.y256 += (src->math_vector.y256);/*fps_factor*/
//
	/* 親が画面外なら分裂しない */
	gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
}


/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/ OBJ_CALL_FUNC(add_zako_aya_doll)
{
	int angle1024;
//	for (angle1024=(int)((1024)-(1024*2/24)); angle1024<(int)((1024)+(1024*3/24)); angle1024+=(int)(1024*1/24) )
	for (angle1024=(0); angle1024<(1024); angle1024+=(int)(1024*1/24) )
	{
		OBJ *h;
		h							= obj_regist_teki();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->obj_type_set 		= (TEKI_32_20)+((angle1024>>7)&0x07);/* 0 ... 8 */
			/* あたり判定無し */
			h->atari_hantei 		= (ATARI_HANTEI_OFF/*スコア兼用*/);
			h->callback_mover		= move_aya_momiji_oya;
		//	h->callback_loser		= NULL;
	//		h->callback_hit_teki	= callback_hit_zako;/*???*/
			h->rotationCCW1024		= (angle1024);
	//
	//		h->base_score			= score(25*2);/*ダミー*/
			h->base_hp				= (9999);/*倒せない*/
	//
			h->BOSS_DATA_05_move_jyumyou			= ((30+15)/*+1*/);
			{
				/* 初期位置 */
				h->center.x256			= src->center.x256;
				h->center.y256			= src->center.y256;
			}
			#if 1/*???*/
			h->MOMIJI_KODOMO_next_angle1024 	= (angle1024);
			#endif
	//		regist_vector(h, speed256, angle512);
		//	const int speed256 = t256(1.00);
		//	h->math_vector.x256 = ((si n1024((angle1024))*(speed256))>>8);	/*fps_factor*/
		//	h->math_vector.y256 = ((co s1024((angle1024))*(speed256))>>8);	/*fps_factor*/
			#if (0)//
			h->math_vector.x256 = ((si n1024((angle1024))));	/*fps_factor*/	/* CCWの場合 */
			h->math_vector.y256 = ((co s1024((angle1024))));	/*fps_factor*/
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (angle1024), &sin_value_t256, &cos_value_t256);
				h->math_vector.x256 		= ((sin_value_t256));/*fps_factor*/
				h->math_vector.y256 		= ((cos_value_t256));/*fps_factor*/
			}
			#endif
		}
	}
}
