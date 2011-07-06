
/*---------------------------------------------------------
	“Œ•û–Í•í•—	` Toho Imitation Style.
	ƒvƒƒWƒFƒNƒgƒy[ƒW http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"–Ñ‹Ê2",	"BADGUY",	(r27‚©‚çV‹K’Ç‰Á)
	-------------------------------------------------------
	–{‰Æ–Ñ‹Ê
---------------------------------------------------------*/

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/

static void move_kedama2(SPRITE *src)
{
	if (512 > src->jyumyou)
	{
		gamen_gai_nara_zako_osimai(src);/* ‰æ–ÊŠO‚È‚ç‚¨‚µ‚Ü‚¢ */
	}
	else
	if (512+64 > src->jyumyou)	/* UŒ‚ */
	{
		{	enum
			{
				K00_KOUGEKI_KANKAKU_MASK = 0,
				K01_SUKIMA_KAKUDO,
				K02_TAMA_KAZU,
				K99_MAX/* Å‘å” */
			};
			const u16 kougeki_tbl[(4*2)][K99_MAX] =
			{
				/* 0:g‚Á‚Û‚¢–Ñ‹Ê(‹·Šp“x Šï”’e) */
				{0x3f, (u16)(1024/12), (5)},	/* (u32)(1024/12):LŠp‚È‚Ì‚ÅŠÈ’P */
				{0x1f, (u16)(1024/16), (3)},	/* 0x0f:•’Ê‚É”ð‚¯‚ê‚éŠÔŠu */
				{0x0f, (u16)(1024/20), (3)},	/* 0x0f:•’Ê‚É”ð‚¯‚ê‚éŠÔŠu */
				{0x07, (u16)(1024/24), (5)},	/* (u32)(1024/24):•’Ê‚Ì‹·Šp */
				/* 1:—d‚Á‚Û‚¢–Ñ‹Ê(90“x 4•ûŒü) (–³‘Ê’e‚È‚Ì‚Å“ïˆÕ“x‚Í’á‚¢”¤) */
				{0x3f, (u16)(1024/8),  (8)},	/* 8:Œ©‚¹’e(–³‘Ê’e‚È‚Ì‚Å“ïˆÕ“x‚Í’á‚¢”¤) */
				{0x1f, (u16)(1024/8),  (8)},	/* 0x1f:•’Ê‚É”ð‚¯‚ê‚éŠÔŠu */
				{0x07, (u16)(1024/4),  (4)},
				{0x03, (u16)(1024/4),  (4)},	/* 0x03:˜A‘±’e‚Á‚Û‚­ */
			};
			if (0==(src->jyumyou & kougeki_tbl[src->kougeki_type][K00_KOUGEKI_KANKAKU_MASK]))/*0x0f*/
			{
				obj_send1->cx256 = src->cx256;
				obj_send1->cy256 = src->cy256;
				br.BULLET_REGIST_speed256			= (t256(2.5)+((difficulty)<<6));/* [2.5 - 3.25] */		//	(t256(2.0)+((difficulty)<<6)),/* [2.0 - 2.75] */
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_div_angle1024		= kougeki_tbl[src->kougeki_type][K01_SUKIMA_KAKUDO];	//	(int)(1024/24),
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KOME_00_SIRO+(src->cx256&0x07);
				br.BULLET_REGIST_n_way				= kougeki_tbl[src->kougeki_type][K02_TAMA_KAZU];		//	(3/*+difficulty+difficulty*/)
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
	}
	else
	{
		/* ‚Î‚ç‚Î‚ç‚É“oê‚³‚¹‚é */
		if (
			(0 >= src->cx256)
			|| (t256(GAME_WIDTH) < src->cx256)
			#if (1==USE_X_HOUKOU)
			|| (0 >= src->cy256)
			|| (t256(GAME_HEIGHT) < src->cy256)
			#endif
		)
		{
			/*‰æ–ÊŠO*/;
		}
		else
		{
			src->jyumyou	= (512+64-1); /* “oê‚·‚é */
		}
	}

	/* ˆÚ“®‚·‚é */
	src->cx256 += (src->vx256);
	src->cy256 += (src->vy256);
	/* ƒAƒjƒ(‰ñ“]) */
	src->rotationCCW1024 += (10);
	mask1024(src->rotationCCW1024);
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

global void add_zako_kedama2(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* “o˜^‚Å‚«‚½ê‡‚Ì‚Ý */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_59_HAI_KEDAMA;
		h->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0xaa);		/*”’‚Á‚Û‚­ */
		h->callback_mover		= move_kedama2;
		//
		h->cy256 				= ((l->user_y)<<8);
		h->vx256				= ((l->user_x));
		h->vy256				= (0);
		//
		{
			if (0 < (h->vx256))
			{/* [ˆø” user_y ‚ª³•ûŒü‚Ìê‡A‰E‚ÖˆÚ“®(¨)] [ˆø”y‚ª•‰•ûŒü‚Ìê‡A‰º‚ÖˆÚ“®(«)] */
				h->cx256 			= t256(-30)/*-(i<<(5+8))*/;
			}
			else
			{/* [ˆø” user_y ‚ª³•ûŒü‚Ìê‡A¶‚ÖˆÚ“®(©)] [ˆø”y‚ª•‰•ûŒü‚Ìê‡Aã‚ÖˆÚ“®(ª)] */
				h->cx256 			= t256(360)/*+(i<<(5+8))*/;/* 360 > 352(x_max) > 272(y_max) */
			}
		}
		//
		if (0 < (h->cy256))
		{	;	}
		else
		{
			/* yÀ•W‚ª•‰•ûŒü‚Ìê‡‚ÍAxÀ•WŽw’è‚È‚Ì‚Å(xÀ•W yÀ•W ‚ð‹t‚É‚·‚é) */
			{
				int s_sss;
				s_sss				= h->cx256;
				h->cx256 			= -(h->cy256);
				h->cy256 			= s_sss;
			}
			h->vy256			= (h->vx256);
			h->vx256			= (0);
		}
//		h->jyumyou			= 512+64+512+64;	/* “K“–‚É‘å‚«‚È’l */	/*+(i<<4)*/
		h->kougeki_type 	= (((l->user_255_code)&1)<<2)|(difficulty/*&0x03*/);
	}
}
