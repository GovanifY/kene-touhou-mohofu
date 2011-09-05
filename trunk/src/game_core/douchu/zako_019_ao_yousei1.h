
/*---------------------------------------------------------
	“Œ•û–Í•í•— ` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	“¹’†‚ÌƒUƒR
	-------------------------------------------------------
		"Â—d¸1",		"FAIRY",
	-------------------------------------------------------
	user_x: 	‘¬“x256[speed256]
	user_y: 	‰¡ˆÊ’u[pixel]
---------------------------------------------------------*/

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_ao_yousei1(OBJ *src)
{
	if (512 > src->jyumyou) 	/* —£’E */
	{
		src->speed256 += (2);/*(2+1)*/
		src->tmp_angleCCW1024 += (4);
		if (511 < src->tmp_angleCCW1024)
		{
			src->tmp_angleCCW1024 = (511);/*^ã*/
		}
		gamen_gai_nara_zako_osimai(src);/* ‰æ–ÊŠO‚È‚ç‚¨‚µ‚Ü‚¢ */
	}
	else	/* UŒ‚ */
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
		if (0x00==(src->jyumyou&(0x1f)))/* 8‰ñ */
		{
			if (0!=(cg.game_difficulty))
			{
				zako_shot_supeka(src, ZAKO_SPEKA_1b_ao2);
			}
		}
		src->speed256 -= (5/*5*/);
		if (1 > src->speed256)
		{
			src->speed256 = 1;
		}
	}
	/*ˆÈ‰ºrwingx.c‚Æ“¯‚¶*/
	//------------------
	HATSUDAN_01_speed256	= (src->speed256);
	HATSUDAN_03_angle65536	= deg1024to65536((src->tmp_angleCCW1024));
	sincos256();/*(”j‰óƒŒƒWƒXƒ^‘½‚¢‚Ì‚Å’ˆÓ)*/
	src->cx256 += REG_03_DEST_Y;//sin_value_t256 // ‰ºCCW‚Ìê‡(‰ECW‚Ìê‡‚Æ‚Æxy‚ª‹t)
	src->cy256 += REG_02_DEST_X;//cos_value_t256 // ‰ºCCW‚Ìê‡(‰ECW‚Ìê‡‚Æ‚Æxy‚ª‹t)
	//------------------
	/* ƒAƒjƒ[ƒVƒ‡ƒ“ */
	zako_anime_type02(src);
}


/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

static void regist_zako_019_ao_yousei1(GAME_COMMAND *l, OBJ *h)
{
//	h->cx256				= ((l->user_x)<<8);
//	h->cy256				= ((l->user_y)<<8);
	h->cy256				+= t256(-128);
//
	h->speed256 			= t256(2.0);

	h->tmp_angleCCW1024 	= (0);/* ^‰º */
	h->jyumyou				= (512+320);
}
