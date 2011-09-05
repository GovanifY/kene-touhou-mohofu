
/*---------------------------------------------------------
	“Œ•û–Í•í•— ` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	“¹’†‚ÌƒUƒR
	-------------------------------------------------------
	"—Î–Ñ‹Ê1",		"XEV",
	-------------------------------------------------------
	—Î–Ñ‹Ê	’P‘Ì‚Å“Ë‚Áž‚ñ‚Å‚­‚éB
---------------------------------------------------------*/

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_midori_kedama1(OBJ *src)
{
	if ((JYUMYOU_ZAKO_HASSEI-320) > src->jyumyou)		/* —£’E */
	{
		gamen_gai_nara_zako_osimai(src);/* ‰æ–ÊŠO‚È‚ç‚¨‚µ‚Ü‚¢ */
		src->speed256++;// = t256(3.5/*6*/);
	}
	else
	{
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER];
		if ((src->cx256 >= zzz_player->cx256)||
			(src->cy256 > t256(GAME_HEIGHT-48) ) )/*480-80*/
		{
			src->jyumyou	= (JYUMYOU_ZAKO_HASSEI-320);
			src->target_x256	= t256(-100);
		//	src->target_y256	= zzz_player->cy256-t256(48);
			src->target_y256	= zzz_player->cy256-t256(48);
			zako_shot_supeka(src, ZAKO_SPEKA_0a_midori1);
		}
	}
	REG_00_SRC_X	= (src->target_x256);					/* ’eŒ¹x256 */
	REG_01_SRC_Y	= (src->target_y256);					/* ’eŒ¹y256 */
	set_REG_DEST_XY(src);	/* ’eŒ¹x256 y256 ’†S‚©‚ç”­’eB */
	tmp_angleCCW65536_src_nerai();
	//------------------
	HATSUDAN_01_speed256	= (src->speed256);
//	HATSUDAN_03_angle65536	= (HATSUDAN_03_angle65536);
	sincos256();/*(”j‰óƒŒƒWƒXƒ^‘½‚¢‚Ì‚Å’ˆÓ)*/
	src->cx256 += REG_03_DEST_Y;//sin_value_t256 // ‰ºCCW‚Ìê‡(‰ECW‚Ìê‡‚Æ‚Æxy‚ª‹t)
	src->cy256 += REG_02_DEST_X;//cos_value_t256 // ‰ºCCW‚Ìê‡(‰ECW‚Ìê‡‚Æ‚Æxy‚ª‹t)
	//------------------
	/* ƒAƒjƒ[ƒVƒ‡ƒ“ */
	src->zako_anime_rotate_angle1024 = (  5);
	zako_anime_type04(src); 	/* ƒOƒ‰‰ñ“] */
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

static void regist_zako_008_midori_kedama1(GAME_COMMAND *l, OBJ *h)
{
	h->speed256 = t256(2.5);/*(3.0)*/
	{
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER];
		h->target_x256			= zzz_player->cx256;
	//	h->target_y256			= zzz_player->cy256;		/* “ÁU‹ÖŽ~ */
		h->target_y256			= ((zzz_player->cy256)>>1);
	}
}
