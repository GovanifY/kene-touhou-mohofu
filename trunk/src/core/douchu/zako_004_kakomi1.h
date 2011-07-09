
/*---------------------------------------------------------
	“Œ•û–Í•í•— ` Toho Imitation Style.
	ƒvƒƒWƒFƒNƒgƒy[ƒW http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"ˆÍ—d‰ö1",		"CUBE",
	-------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_kakomi1(SPRITE *src)
{
	if ((JYUMYOU_ZAKO_HASSEI-295) > src->jyumyou)		/* —£’E */
	{
		src->radius256 += t256(1.2);		/*fps_factor*/	/*2.0*/
		if (src->radius256 > t256(350.0))
		{
			src->jyumyou = JYUMYOU_NASI;	/* ‚¨‚µ‚Ü‚¢ */
		}
	}
	else
	{
		src->radius256 -= t256(1.0);		/*fps_factor*/
		if (0==(ra_nd()&(512-1)))			/* 1/512 © 1/500 ‚ÌŠm—¦‚Å’e‘Å‚Â */ 			/*%500*/
		{
			br.BULLET_REGIST_00_speed256				= t256(1.0);				/* ’e‘¬ */
			br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN; 	/* Ž©‹@‘_‚¢’e */	/* ”­ŽË’†SŠp“x / “ÁŽê‹@”\(Ž©‹@‘_‚¢/‘¼) */
		//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_MINI8_04_MIZU_IRO; 	/* [Ô’e] */ /* ’eƒOƒ‰ */
			br.BULLET_REGIST_06_n_way					= (1);						/* [1way] */
		//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (0);						/* ƒ_ƒ~[Šp“x(–¢Žg—p) */
			bullet_regist_multi_vector_send1_xy_src(src); 	/* ’eŒ¹x256 y256 ’†S‚©‚ç”­’eB */
		}
		src->tmp_angleCCW1024 -= (2);		/*fps_factor*/	/*2*/	/* deg_360_to_512(3) ‘¬‚·‚¬‚é */
		mask1024(src->tmp_angleCCW1024);
	}
	//
	zako_move_type11_radius(src);
	/* ƒAƒjƒ[ƒVƒ‡ƒ“ */
	zako_anime_type03(src);
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

static void regist_zako_004_kakomi1(GAME_COMMAND *l, SPRITE *h)
{
	h->radius256				= t256(350);
//	#if 1
	/* r30ŒÝŠ· */
	h->tmp_angleCCW1024 		= (l->user_x + l->user_x);/*(i<<5)*/	//	/*360*/(1024/16)*i;
//	#else
//	/* r31 */
//	/*(I‚­‚¢‚©‚È‚¢A“ä)*/
//	h->tmp_angleCCW1024 		= (l->user_x);/*(i<<5)*/	//	/*360*/(1024/16)*i;
//	#endif
	h->target_x256			= t256(GAME_WIDTH/2);/* GAME_WIDTH 380 ~= 380 == 62+256+62 */
	h->target_y256			= t256(GAME_HEIGHT/2);
}

/*
;[16•ªŠ„]
  0,  32,  64,	96,
128, 160, 192, 224,
256, 288, 320, 352,
384, 416, 448, 480,
[0==512,]
;[13•ªŠ„]
  0,   0, -0
 39,  40, -1
 78,  80, -2
118, 120, -2
157, 160, -3
196, 200, -4
236, 240, -4
275, 280, -5
314, 320, -6
354, 360, -6
393, 400, -7
432, 440, -8
472, 480, -8
*/
