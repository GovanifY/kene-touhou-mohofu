
/*---------------------------------------------------------
	“Œ•û–Í•í•—	` Toho Imitation Style.
	ƒvƒƒWƒFƒNƒgƒy[ƒW http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"Â—d¸1",		"FAIRY",
	-------------------------------------------------------
	user_x: 	‘¬“x256[speed256]
	user_y: 	‰¡ˆÊ’u[dots]
---------------------------------------------------------*/

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_ao_yousei1(SPRITE *src)
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
			if (difficulty)
			{
				obj_send1->cx256					= src->cx256;
				obj_send1->cy256					= src->cy256;
				br.BULLET_REGIST_speed256			= (t256(2.0));								/* ’e‘¬ */	/*+((difficulty)<<6)*/
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_div_angle1024		= (int)(1024/24)-(difficulty);				/* •ªŠ„Šp“x */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_04_YUKARI+((ra_nd())&7);	/* [’eƒOƒ‰] */
				br.BULLET_REGIST_n_way				= (8);										/* 8way’e */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
//		ca se 0x40: /*not_break;*/	/*0x40==064 0x46==070*/
//		ca se 0x80: /*not_break;*/	/*0x80==128 0x78==120*/
//		ca se 0xa0: /*not_break;*/	/*0x  ==170 0xaa==170*/
//		ca se 0xc0: 				/*0x  ==220 0xdc==220*/
		if (0x00==(src->jyumyou&(0x1f)))/* 8‰ñ */
		{
			if (difficulty)
			{
				obj_send1->cx256					= src->cx256;
				obj_send1->cy256					= src->cy256;
				br.BULLET_REGIST_speed256			= (t256(2.0));						/* ’e‘¬ */
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_div_angle1024		= (int)(1024/24)-(difficulty);		/* •ªŠ„Šp“x */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_00_AOI;			/* [Ô’e] */	//	BULLET_MARU8_00_AKA,
				br.BULLET_REGIST_n_way				= (difficulty); 					/* [1way] */	/*1*/
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
		src->speed256 -= (5);
		if (1 > src->speed256)
		{
			src->speed256 = 1;
		}
	}
	/*ˆÈ‰ºrwingx.c‚Æ“¯‚¶*/
//	src->cx256 += ((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/	/* CCW‚Ìê‡ */
	src->cy256 += ((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/

	/* ƒAƒjƒ[ƒVƒ‡ƒ“ */
	{
		zako_anime_type02(src, (TEKI_12_YOUSEI1_1)+(0x04));
	}
}


/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

global void add_zako_ao_yousei1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* “o˜^‚Å‚«‚½ê‡‚Ì‚Ý */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_12_YOUSEI1_1;
		h->callback_mover		= move_ao_yousei1;
	//
	//	h->cx256				= (((u32)l->user_x)<<8);/*lv*t256(35)+t256(40)*/
	//	h->cy256				= ((l->user_y)<<8);
		h->cy256				+= t256(-128);
	//
		h->speed256 			= ((512));			/*t256(0.5)*/	/*+0.15*di fficulty*/ /*0.7+0.3*di fficulty*/

		h->tmp_angleCCW1024 	= cv1024r(0);/* ^‰º */
		h->jyumyou				= (512+320);
	}
}
