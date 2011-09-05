
/*---------------------------------------------------------
	“Œ•û–Í•í•— ` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	“¹’†‚ÌƒUƒR
	-------------------------------------------------------
		"–Ñ‹Ê1",	"BADGUY",
	-------------------------------------------------------
	–{‰Æ–Ñ‹Ê
---------------------------------------------------------*/

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void my_nerai(OBJ *src)
{
	REG_00_SRC_X	= (src->target_x256);						/* ’eŒ¹x256 */
	REG_01_SRC_Y	= (src->target_y256-t256(64)/*+(20)*/); 	/* ’eŒ¹y256 */
	set_REG_DEST_XY(src);
	tmp_angleCCW65536_src_nerai();
	//------------------
	HATSUDAN_01_speed256	= (t256(1.0));
//	HATSUDAN_03_angle65536	= (HATSUDAN_03_angle65536);
	sincos256();/*(”j‰óƒŒƒWƒXƒ^‘½‚¢‚Ì‚Å’ˆÓ)*/
	src->vx256 = REG_03_DEST_Y;//sin_value_t256 // ‰ºCCW‚Ìê‡(‰ECW‚Ìê‡‚Æ‚Æxy‚ª‹t)
	src->vy256 = REG_02_DEST_X;//cos_value_t256 // ‰ºCCW‚Ìê‡(‰ECW‚Ìê‡‚Æ‚Æxy‚ª‹t)
	//------------------
}

static void move_kedama1(OBJ *src)
{
	if (512 > src->jyumyou)/* ‘Þê */
	{
		my_nerai(src);
		gamen_gai_nara_zako_osimai(src);/* ‰æ–ÊŠO‚È‚ç‚¨‚µ‚Ü‚¢ */
	}
	else
	if (512+64 > src->jyumyou)/* */
	{
		if (0==(src->jyumyou & 0x0f))
		{
			if (ENEMY_LAST_SHOT_LINE256 > src->cy256)	/* ‚±‚Ìƒ‰ƒCƒ“‚æ‚è‰º‚©‚ç‚Í“G‚ªŒ‚‚½‚È‚¢ */
			{
				zako_shot_supeka(src, ZAKO_SPEKA_1e_kedama1);
			}
		}
	}
	else
//	if (512+64+64 > src->jyumyou)	/* “oê‚·‚é */
	{
		my_nerai(src);
		{
			OBJ *zzz_player;
			zzz_player = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER];
			if (//(src->cx256 >= zzz_player->cx256) ||
				(src->cy256 > t256(GAME_HEIGHT/2) ) /*(480-80)*/ /*(272-32)*/
			 )
			{
				src->vy256 = (-src->vy256);
				src->jyumyou	= (512+64-1);
				/* ‘æ“ñ–Ú•W‚ÍA */
				src->target_x256		= t256(((480-128)/2));	// t256(-100)
				src->target_y256		= t256(0);/*((zzz_player->cy256)&(128-1))*/
				src->speed256			= t256(2.5/*5.0*/);/*fps_factor*/	/*6*/
			}
		}
	}
	/* ˆÚ“®‚·‚é */
	src->cx256 += (((src->vx256)*(src->speed256))>>8);
	src->cy256 += (((src->vy256)*(src->speed256))>>8);
	/* ƒAƒjƒ[ƒVƒ‡ƒ“ */
	src->zako_anime_rotate_angle1024 = ( 10);
	zako_anime_type04(src); 	/* ƒOƒ‰‰ñ“] */
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

static void regist_zako_010_kedama1(GAME_COMMAND *l, OBJ *h)
{
	h->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0xaa);		/*”’‚Á‚Û‚­ */
//
	h->jyumyou			= (512+64+64);
	{
		/* ‘æˆê–Ú•W‚ÍƒvƒŒƒCƒ„[‚Ì‚¢‚½ˆÊ’u */
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER];
		h->target_x256				= (zzz_player->cx256);
		h->target_y256				= (zzz_player->cy256);
	}
	h->vx256				= (0);
	h->vy256				= (0);
//	h->speed256 			= ((ra_nd()&((256*2)-1)))*(1+difficulty+teki_rank/3);/*fps_factor*/
//	h->speed256 			= ((ra_nd()&((256*2)-1))) + (((cg_game_di fficulty)<<8)+ (/*teki_rank*/5<<7/**86*2*/) );/*fps_factor*/
//	h->speed256 			= ((ra_nd()&((256*2)-1))) + (((3)<<8)+ (/*teki_rank*/5<<7/**86*2*/) );/*fps_factor*/
//	h->speed256 			= ((ra_nd()&((256*2)-1))) + ((t256(5.5) ) );/*fps_factor*/ /*(easy==2.5 normal==3.5 hard==4.5 lunatic==5.5)*/
	h->speed256 			= ((ra_nd()&((256*2)-1))) + ((t256(4)));/*fps_factor*/
}
