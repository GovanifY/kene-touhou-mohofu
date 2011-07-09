
/*---------------------------------------------------------
	“Œ•û–Í•í•— ` Toho Imitation Style.
	ƒvƒƒWƒFƒNƒgƒy[ƒW http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"Â—d¸4",(—\’è)		"FAIRY",	(r27‚©‚çV‹K’Ç‰Á)
	-------------------------------------------------------
	user_x: 	‘¬“x256[speed256]
	user_y: 	‰¡ˆÊ’u[dots]
---------------------------------------------------------*/

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_ao_yousei4(SPRITE *src)
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
			if ((cg_game_difficulty))
			{
				br.BULLET_REGIST_00_speed256				= (t256(2.0));								/* ’e‘¬ */	/*+((difficulty)<<6)*/
				br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN;
			//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KUNAI12_00_SIRO+((ra_nd())&7); 	/* [’eƒOƒ‰] */
				br.BULLET_REGIST_06_n_way					= (8);										/* 8way’e */
				br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/24)-((cg_game_difficulty));	/* •ªŠ„Šp“x */
				bullet_regist_multi_vector_send1_xy_src(src); 	/* ’eŒ¹x256 y256 ’†S‚©‚ç”­’eB */
			}
		}
//		ca se 0x40: /*not_break;*/	/*0x40==064 0x46==070*/
//		ca se 0x80: /*not_break;*/	/*0x80==128 0x78==120*/
//		ca se 0xa0: /*not_break;*/	/*0x  ==170 0xaa==170*/
//		ca se 0xc0: 				/*0x  ==220 0xdc==220*/
		if (0x00==(src->jyumyou&(0x1f)))/* 8‰ñ */
		{
			if ((cg_game_difficulty))
			{
				br.BULLET_REGIST_00_speed256				= (t256(2.0));						/* ’e‘¬ */
				br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN;
			//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_UROKO14_03_AOI;			/* [Â’e] */
				br.BULLET_REGIST_06_n_way					= ((cg_game_difficulty));					/* [1way] */	/*1*/
				br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/24)-((cg_game_difficulty));		/* •ªŠ„Šp“x */
				bullet_regist_multi_vector_send1_xy_src(src); 	/* ’eŒ¹x256 y256 ’†S‚©‚ç”­’eB */
			}
		}
		src->speed256 -= (5);
		if (1 > src->speed256)
		{
			src->speed256 = 1;
		}
	}
	/*ˆÈ‰ºrwingx.c‚Æ“¯‚¶*/
	src->cx256 += ((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/	/* CCW‚Ìê‡ */
	src->cy256 += ((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/

	/* ƒAƒjƒ[ƒVƒ‡ƒ“ */
	zako_anime_type02(src);
}


/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

static void regist_zako_022_ao_yousei4(GAME_COMMAND *l, SPRITE *h)
{
	h->cy256				= t256(-30);
	h->cx256				= ((l->user_y)<<8);/*lv*t256(35)+t256(40)*/
	h->speed256 			= ((l->user_x));			/*t256(0.5)*/	/*+0.15*di fficulty*/ /*0.7+0.3*di fficulty*/

	h->tmp_angleCCW1024 	= cv1024r(0);/* ^‰º */
	h->jyumyou				= (512+320);
}
