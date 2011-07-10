
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	道中のザコ
	-------------------------------------------------------
		"青妖精1",		"FAIRY",
	-------------------------------------------------------
	user_x: 	速度256[speed256]
	user_y: 	横位置[dots]
---------------------------------------------------------*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_ao_yousei1(SPRITE *src)
{
	if (512 > src->jyumyou) 	/* 離脱 */
	{
		src->speed256 += (2);/*(2+1)*/
		src->tmp_angleCCW1024 += (4);
		if (511 < src->tmp_angleCCW1024)
		{
			src->tmp_angleCCW1024 = (511);/*真上*/
		}
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	}
	else	/* 攻撃 */
	{
		if ((512+50)==src->jyumyou)
		{
			if (0!=(cg.game_difficulty))
			{
				zako_shot_supeka(src, ZAKO_SPEKA_1c_ao1);
			}
		}
//		ca se 0x40: /*not_break;*/	/*0x40==064 0x46==070*/
//		ca se 0x80: /*not_break;*/	/*0x80==128 0x78==120*/
//		ca se 0xa0: /*not_break;*/	/*0x  ==170 0xaa==170*/
//		ca se 0xc0: 				/*0x  ==220 0xdc==220*/
		if (0x00==(src->jyumyou&(0x1f)))/* 8回 */
		{
			if (0!=(cg.game_difficulty))
			{
				zako_shot_supeka(src, ZAKO_SPEKA_1b_ao2);
			}
		}
		src->speed256 -= (5);
		if (1 > src->speed256)
		{
			src->speed256 = 1;
		}
	}
	/*以下rwingx.cと同じ*/
//	src->cx256 += ((si n1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/	/* CCWの場合 */
//	src->cy256 += ((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
		{
			int sin_value_t256; 		//	sin_value_t256 = 0;
			int cos_value_t256; 		//	cos_value_t256 = 0;
			int256_sincos1024( ((src->tmp_angleCCW1024)+255), &sin_value_t256, &cos_value_t256);
			src->cy256 += (( (sin_value_t256)*(src->speed256))>>8);/*fps_factor*/
		}
	/* アニメーション */
	zako_anime_type02(src);
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

static void regist_zako_019_ao_yousei1(GAME_COMMAND *l, SPRITE *h)
{
//	h->cx256				= (((u32)l->user_x)<<8);/*lv*t256(35)+t256(40)*/
//	h->cy256				= ((l->user_y)<<8);
	h->cy256				+= t256(-128);
//
	h->speed256 			= ((512));			/*t256(0.5)*/	/*+0.15*di fficulty*/ /*0.7+0.3*di fficulty*/

	h->tmp_angleCCW1024 	= cv1024r(0);/* 真下 */
	h->jyumyou				= (512+320);
}
