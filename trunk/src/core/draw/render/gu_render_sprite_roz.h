
/*---------------------------------------------------------
	”Ä—pƒXƒvƒ‰ƒCƒg‚Ì•\Ž¦ (‰ñ“]Šg‘åk¬‚ ‚è)ƒ‹[ƒ`ƒ“
	-------------------------------------------------------
	Œ»Ý(2009-11-13Œ»Ý)‰¡‚µ‚© slice ‚µ‚Ä‚È‚¢‚¯‚ÇA‚Ç‚¤‚àc‰¡‚É
	 slice ‚µ‚È‚¢‚Æ(ƒ`ƒ‹ƒm‚âƒŒƒ~ƒŠƒAƒ{ƒ€‚Ì‘¬“x’á‰º‚Á‚Õ‚è‚©‚ç”»’f‚µ‚Ä)
	ƒ_ƒ‚Å‚·‚ËB‚ ‚Æ‚Å‰ü—Ç‚µ‚æ‚¤B
	-------------------------------------------------------
	‹t‚É’e–‹‚Í(64[dots]ˆÈ‰º‚Ì > ‘å’e‚®‚ç‚¢‚È‚ç) slice ‚µ‚È‚­‚Ä‚à
	‚¢‚¢‚Ì‚©‚à‚µ‚ê‚È‚¢B
	’e–‹‚ÍƒVƒXƒeƒ€“I‚É‚àu”Ä—pƒXƒvƒ‰ƒCƒg‚Å•`‚©‚È‚¢•û‚ªŒø—¦‚ª—Ç‚¢v‚Ì‚ÅA
	ê—pƒ‹[ƒ`ƒ“‚É•ªŠò‚·‚é—\’è(2009-11-13Œ»Ý)‚Å‚·B
---------------------------------------------------------*/

static void render_object_use_rot_zoom(/*TGameScreen *pclass,*/ TGameSprite *spr)
{
	/* --- •\Ž¦ƒXƒCƒbƒ` */
//	if (spr->used ==  0/*FA LSE*/) return;
//	if (spr->Texture == NULL) return;
//
	/* --- ‰ñ“]Šg‘å‘Oˆ— */
	int sin_angle;	/* ˆÊ‘Š  +0“xi‚Ý */
	int cos_angle;	/* ˆÊ‘Š  +90“xi‚Ý */
//	int c18_angle;	/* ˆÊ‘Š +180“xi‚Ý */
	{
		/* Šp“x‚Í0-1023“x */
		unsigned int rotation_angle1024;
	//	rotation_angle512	= ((spr->rotation_1024z) / (128/*65536/512*/)); 	/* Šp“x‚Í0-65535“x‚È‚Ì‚Å0-511“x‚Ö•ÏŠ·B */
		rotation_angle1024	= ((spr->rotation_1024z) /*>> (7)*/);				/* Šp“x‚Í0-1023“x */
		#if (1==USE_SIN_TABLE)
		sin_angle = (sin_tbl 512[/*rot_sin*/((/*OFFS_SIN512+*/rotation_angle512)&(512-1))]/*<<8*/);
		cos_angle = (sin_tbl 512[/*rot_cos*/((	OFFS_COS512+  rotation_angle512)&(512-1))]/*<<8*/);
		#else
	//	sin_angle = (int)(int256_sin1024(/*rot_sin*/((/*OFFS_SIN512+*/			  rotation_angle512+rotation_angle512)&(1024-1)))/*<<8*/);
	//	cos_angle = (int)(int256_sin1024(/*rot_cos*/((	OFFS_COS512+OFFS_COS512+  rotation_angle512+rotation_angle512)&(1024-1)))/*<<8*/);
		sin_angle = (int)(int256_sin1024(/*rot_sin*/((/*OFFS_SIN1024+*/ 		  rotation_angle1024)&(1024-1)))/*<<8*/);
		cos_angle = (int)(int256_sin1024(/*rot_cos*/((	OFFS_COS1024+			  rotation_angle1024)&(1024-1)))/*<<8*/);
		#endif
	}
//
	/* --- ”¼“§–¾‡¬’l */
	#if (1==USE_VCOLOR)
		#if 0//(16==USE_BLIT_COLOR_BIT)
			#if 0
				/*ARGB4444*/
				unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
			#endif
			#if 1
				/*ARGB0565*/
				unsigned /*int*/short blendlevel = MAKECOL16( (spr->alpha), (spr->alpha), (spr->alpha) );
			#endif
		#else
//	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
	unsigned int blendlevel = (spr->color8888);
		#endif
	#endif

	/* --- ”Ä—pƒXƒvƒ‰ƒCƒg‚ðˆê–‡•`‰æ‚·‚é */
	/* ‚ ‚é’ö“x‚Ì‘å‚«‚³‚ðŽ‚Á‚½ƒXƒvƒ‰ƒCƒg‚Í’Zûó‚É•ªŠ„‚µ‚È‚¢‚Æ */
	/* ‚Æ‚Ä‚Â‚à‚È‚­’x‚­‚È‚é */

	unsigned int w_rest;
	w_rest = spr->w + SLICE_64_SIZE;

	unsigned int count4;
	#if 0
	//count4 = (((spr->w / SLICE_64_SIZE)+1)*4);
	#else
	count4 = 0;
	unsigned int i;
	i = 0;
	do
	{
		count4 += 4;
		i += SLICE_64_SIZE;
		w_rest -= SLICE_64_SIZE;
	}
	while ( (i) <(/*480*/spr->w/*512*/));
	#endif
	Vertex_uvcxyz_C32	*vertices;
	vertices = (Vertex_uvcxyz_C32*)sceGuGetMemory(count4 * sizeof(Vertex_uvcxyz_C32));
	unsigned int/*short*/ pos = 0;
	unsigned int/*short*/ w_size;
	w_size	= SLICE_64_SIZE;
	unsigned int/*short*/ x_pos = ((spr->x256>>8));
	unsigned int/*short*/ y_pos = ((spr->y256>>8));
	i = 0;
	for (; i<count4; )
	{
	//	if (((i)+1*4) < (count4))	{}	else		/* ÅŒã */
		if (i==(count4-4))	/* ÅŒã */
		{
			w_size = w_rest;
		}
		unsigned int pos_w_size = (pos + w_size);
		vertices[(i)].u 		= spr->tx + pos;
		vertices[(i)].v 		= spr->ty;
		#if (1==USE_VCOLOR)
		vertices[(i)].color 	= blendlevel;
		#endif
		vertices[(i)].x 		= x_pos + pos;
		vertices[(i)].y 		= y_pos;
		vertices[(i)].z 		= SPR_PRIORITY;
	//
		vertices[(i)+1].u		= spr->tx + (pos_w_size);
		vertices[(i)+1].v		= spr->ty;
		#if (1==USE_VCOLOR)
		vertices[(i)+1].color	= blendlevel;
		#endif
		vertices[(i)+1].x		= x_pos + (pos_w_size);
		vertices[(i)+1].y		= y_pos;
		vertices[(i)+1].z		= SPR_PRIORITY;
	//
		vertices[(i)+2].u		= spr->tx + pos;
		vertices[(i)+2].v		= spr->ty + spr->h;
		#if (1==USE_VCOLOR)
		vertices[(i)+2].color	= blendlevel;
		#endif
		vertices[(i)+2].x		= x_pos + pos;
		vertices[(i)+2].y		= y_pos + spr->h;
		vertices[(i)+2].z		= SPR_PRIORITY;
	//
		vertices[(i)+3].u		= spr->tx + (pos_w_size);
		vertices[(i)+3].v		= spr->ty + spr->h;
		#if (1==USE_VCOLOR)
		vertices[(i)+3].color	= blendlevel;
		#endif
		vertices[(i)+3].x		= x_pos + (pos_w_size);
		vertices[(i)+3].y		= y_pos + spr->h;
		vertices[(i)+3].z		= SPR_PRIORITY;
	//
		pos += SLICE_64_SIZE;
		/* --- ‰ñ“]Šg‘åˆ— */
		int center_x;
		int center_y;
		center_x = ((spr->x256>>8)		) + (spr->w >> 1);/*/2*/
		center_y = ((spr->y256>>8)		) + (spr->h >> 1);/*/2*/
		#if (1==USE_ZOOM_XY)
		int zoom_x256;
		int zoom_y256;
	//	zoom_x256 = (spr->zoom_x256) * ((65536/256));	/* Šg‘å—¦‚Í0-256”{‚È‚Ì‚Å0-65536”{‚Ö•ÏŠ·B */
	//	zoom_y256 = (spr->zoom_y256) * ((65536/256));	/* Šg‘å—¦‚Í0-256”{‚È‚Ì‚Å0-65536”{‚Ö•ÏŠ·B */
		zoom_x256 = (spr->zoom_x256) /*<< (8)*/;		/* Šg‘å—¦‚Í0-256”{‚È‚Ì‚Å0-65536”{‚Ö•ÏŠ·B */
		zoom_y256 = (spr->zoom_y256) /*<< (8)*/;		/* Šg‘å—¦‚Í0-256”{‚È‚Ì‚Å0-65536”{‚Ö•ÏŠ·B */
		#else //(0==USE_ZOOM_XY)
		int zoom_xy256;
	//	zoom_xy256 = (spr->zoom_xy256) * ((65536/256)); /* Šg‘å—¦‚Í0-256”{‚È‚Ì‚Å0-65536”{‚Ö•ÏŠ·B */
		zoom_xy256 = (spr->zoom_xy256) /*<< (8)*/;		/* Šg‘å—¦‚Í0-256”{‚È‚Ì‚Å0-65536”{‚Ö•ÏŠ·B */
		#endif/* (1==USE_ZOOM_XY) */
		unsigned int j;
		for (j=0; j<4; j++)
		{
			int ifx;
			int ify;
			int irx;
			int iry;
			ifx = vertices[(i)+j].x;
			ify = vertices[(i)+j].y;
			ifx -= center_x;
			ify -= center_y;
//			rx = ((fx * sprite_cos[rot]) - (fy * sprite_sin[rot])) * spr->zoomx;
//			ry = ((fx * sprite_sin[rot]) + (fy * sprite_cos[rot])) * spr->zoomy;
			#if 0
			/* [‰ECW]‰E‚ª 0“x‚ÅŽžŒv‰ñ‚è‚ÌŠp“xŒn */
			irx = ((ifx * cos_angle) - (ify * sin_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ifx * sin_angle) + (ify * cos_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 1
			/* [‰ºCCW]‰º‚ª 0“x‚Å”½ŽžŒv‰ñ‚è‚ÌŠp“xŒn */
			irx = ((ify * sin_angle) + (ifx * cos_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ify * cos_angle) - (ifx * sin_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 0
			/* [¶CCW(?)]¶‚ª 0“x‚Å”½ŽžŒv‰ñ‚è‚ÌŠp“xŒn */
			irx = ((ifx * sin_angle) - (ify * cos_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ifx * cos_angle) + (ify * sin_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
//
			#if (1==USE_ZOOM_XY)
			irx = ((irx * (zoom_x256))>>8);
			iry = ((iry * (zoom_y256))>>8);
			#else //(0==USE_ZOOM_XY)
			irx = ((irx * (zoom_xy256))>>8);
			iry = ((iry * (zoom_xy256))>>8);
			#endif/* (1==USE_ZOOM_XY) */
		//	irx = (irx >>8/*16*/);	//	rx = rx / (256.0/*65536.0*/);
		//	iry = (iry >>8/*16*/);	//	ry = ry / (256.0/*65536.0*/);

			irx += center_x;
			iry += center_y;

			vertices[(i)+j].x = (short)irx;
			vertices[(i)+j].y = (short)iry;
		}
		i += 4;
	}
	/* --- •`‰æƒŠƒNƒGƒXƒg */
//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (count4), NULL, vertices);
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (count4), NULL, vertices);
}
