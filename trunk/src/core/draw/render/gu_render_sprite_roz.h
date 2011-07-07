
/*---------------------------------------------------------
	汎用スプライトの表示 (回転拡大縮小あり)ルーチン
	-------------------------------------------------------
	現在(2009-11-13現在)横しか slice してないけど、どうも縦横に
	 slice しないと(チルノやレミリアボムの速度低下っぷりから判断して)
	ダメですね。あとで改良しよう。
	-------------------------------------------------------
	逆に弾幕は(64[dots]以下の > 大弾ぐらいなら) slice しなくても
	いいのかもしれない。
	弾幕はシステム的にも「汎用スプライトで描かない方が効率が良い」ので、
	専用ルーチンに分岐する予定(2009-11-13現在)です。
	-------------------------------------------------------
	回転しない場合と違い、
	回転する場合は、回転公式を使いますので、
	最後まで相対座標(obj中心を原点o(0,0)とする。)で計算し、
	最後にobjの中心座標(cx,cy)を加えて画面上の描画座標(Guの使い方が1:1なので)とします。
---------------------------------------------------------*/

static void render_object_use_rot_zoom(SPRITE *spr)
{
	/* --- 表示スイッチ */
//	if (spr->used ==  0/*FA LSE*/) return;
//	if (spr->Texture == NULL) return;
//
	/* --- 回転拡大前処理 */
	int sin_angle;	/* 位相  +0度進み */
	int cos_angle;	/* 位相  +90度進み */
//	int c18_angle;	/* 位相 +180度進み */
	{
		/* 角度は0-1023度 */
		unsigned int rotation_angle1024;
	//	rotation_angle512	= ((spr->rotationCCW1024) / (128/*65536/512*/));	/* 角度は0-65535度なので0-511度へ変換。 */
		rotation_angle1024	= ((spr->rotationCCW1024) /*>> (7)*/);				/* 角度は0-1023度 */
		#if 1
	//	sin_angle = (int)(int256_sin1024(/*rot_sin*/((/*OFFS_SIN512+*/			  rotation_angle512+rotation_angle512)&(1024-1)))/*<<8*/);
	//	cos_angle = (int)(int256_sin1024(/*rot_cos*/((	OFFS_COS512+OFFS_COS512+  rotation_angle512+rotation_angle512)&(1024-1)))/*<<8*/);
		sin_angle = (int)(int256_sin1024(/*rot_sin*/((/*OFFS_SIN1024+*/ 		  rotation_angle1024)&(1024-1)))/*<<8*/);
		cos_angle = (int)(int256_sin1024(/*rot_cos*/((	OFFS_COS1024+			  rotation_angle1024)&(1024-1)))/*<<8*/);
		#endif
	}
//
	/* --- 半透明合成値 */
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
	unsigned int blendlevel = (spr->color32);
		#endif
	#endif

	/* --- 汎用スプライトを一枚描画する */
	/* ある程度の大きさを持ったスプライトは短冊状に分割しないと */
	/* とてつもなく遅くなる */

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
	/* 回転するので x,y 座標は、「画面の座標ではなく」、「obj中心を原点o(0,0)とした、相対座標」で計算しておく。 */
//	unsigned int/*short*/ x_pos = ((spr->cx256>>8));
//	unsigned int/*short*/ y_pos = ((spr->cy256>>8));
//(r32)	unsigned int/*short*/ x_pos = ((spr->cx256>>8)-(spr->w >> 1));/* 中心座標から画像サイズの半分を引き、左上座標を計算 */
//(r32)	unsigned int/*short*/ y_pos = ((spr->cy256>>8)-(spr->h >> 1));/* 中心座標から画像サイズの半分を引き、左上座標を計算 */
	unsigned int/*short*/ x_pos = ( -(spr->w >> 1));/* 中心座標から画像サイズの半分を引き、左上座標を計算 */
	unsigned int/*short*/ y_pos = ( -(spr->h >> 1));/* 中心座標から画像サイズの半分を引き、左上座標を計算 */
	i = 0;
	for (; i<count4; )
	{
	//	if (((i)+1*4) < (count4))	{}	else		/* 最後 */
		if (i==(count4-4))	/* 最後 */
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
		/* --- 回転拡大処理 */
		int center_x;
		int center_y;
//		center_x = ((spr->cx256>>8)		) + (spr->w >> 1);/*/2*/
//		center_y = ((spr->cy256>>8)		) + (spr->h >> 1);/*/2*/
		center_x = ((spr->cx256>>8)		) ;/*/2*/
		center_y = ((spr->cy256>>8)		) ;/*/2*/
		int zoom_x256;
		int zoom_y256;
	//	zoom_x256 = (spr->m_zoom_x256) * ((65536/256)); /* 拡大率は0-256倍なので0-65536倍へ変換。 */
	//	zoom_y256 = (spr->m_zoom_y256) * ((65536/256)); /* 拡大率は0-256倍なので0-65536倍へ変換。 */
		zoom_x256 = (spr->m_zoom_x256) /*<< (8)*/;		/* 拡大率は0-256倍なので0-65536倍へ変換。 */
		zoom_y256 = (spr->m_zoom_y256) /*<< (8)*/;		/* 拡大率は0-256倍なので0-65536倍へ変換。 */
		unsigned int j;
		for (j=0; j<4; j++)
		{
			int ifx;
			int ify;
			int irx;
			int iry;
			ifx = vertices[(i)+j].x;
			ify = vertices[(i)+j].y;
//(r33)			ifx -= center_x;
//(r33)			ify -= center_y;
//1859267 -> 1859251
//			rx = ((fx * sprite_cos[rot]) - (fy * sprite_sin[rot])) * spr->zoomx;
//			ry = ((fx * sprite_sin[rot]) + (fy * sprite_cos[rot])) * spr->zoomy;
			#if 0
			/* [右CW]右が 0度で時計回りの角度系 */
			irx = ((ifx * cos_angle) - (ify * sin_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ifx * sin_angle) + (ify * cos_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 1
			/* [下CCW]下が 0度で反時計回りの角度系 */
			irx = ((ify * sin_angle) + (ifx * cos_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ify * cos_angle) - (ifx * sin_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 0
			/* [左CCW(?)]左が 0度で反時計回りの角度系 */
			irx = ((ifx * sin_angle) - (ify * cos_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ifx * cos_angle) + (ify * sin_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
//
			irx = ((irx * (zoom_x256))>>8);
			iry = ((iry * (zoom_y256))>>8);
		//	irx = (irx >>8/*16*/);	//	rx = rx / (256.0/*65536.0*/);
		//	iry = (iry >>8/*16*/);	//	ry = ry / (256.0/*65536.0*/);

			irx += center_x;
			iry += center_y;

			vertices[(i)+j].x = (short)irx;
			vertices[(i)+j].y = (short)iry;
		}
		i += 4;
	}
	/* --- 描画リクエスト */
//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (count4), NULL, vertices);
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (count4), NULL, vertices);
}
