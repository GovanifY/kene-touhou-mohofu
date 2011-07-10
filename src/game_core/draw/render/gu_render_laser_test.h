
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"draw_screen.c" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#if (1==LINE_TEST)
/*---------------------------------------------------------
	レーザーテスト(テスト版)
---------------------------------------------------------*/
static void gu_blit_laser_test(void)
{
	SPRITE *obj_boss;
	obj_boss			= global_obj_boss;
//	obj_boss			= あたり判定の都合上無理&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
	int boss_center_x = ((obj_boss->cx256)>>8);/*+(16)*/
	int boss_center_y = ((obj_boss->cy256)>>8);/*+(24)*/
	SPRITE *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	int player_center_x = ((zzz_player->cx256)>>8);/*+(16)*/
	int player_center_y = ((zzz_player->cy256)>>8);/*+(24)*/

	/* --  を描画 */
	/* テクスチャーがあるとテクスチャー優先でフラットポリゴンが描画出来ないので */
	/* 一時的にテクスチャー無効モードに切り替える */
	PGC_SEND(PGC_TEXTURE_2D, PGC_OFF);	//sceGuDisable(GU_TEXTURE_2D);/* テクスチャー無効モードにする */
	/* レーザー描画テスト */
	unsigned int ii;
	for (ii=0; ii<MAX_99_LASER; ii++)
	{
		if (LASER_TYPE_00_OFF != (lz[ii].laser_type))/* lz[ii]==0;の場合、off。(レーザーモード 0:off, 1:on) */
		{
			/* --- 回転拡大前処理 */
			int sin_angle_t256;
			int cos_angle_t256;
			{
				/* 角度は0-65535度 */
			//	static unsigned int rotation_angle65536;
			//	rotation_angle65536 += (3);/*50*/
			//
				/* 角度は0-1023度 */
				unsigned int rotation_angle1024;
			//	rotation_angle512	= ((spr->rotation_1024z) / (128/*65536/512*/)); 	/* 角度は0-65535度なので0-511度へ変換。 */
			//	rotation_angle1024	= ((spr->rotation_1024z) /*>> (7)*/);
			//	rotation_angle512	= ((rotation_angle65536) >> (7));
			//	rotation_angle1024	= ((rotation_angle65536) >> (6));
				rotation_angle1024	= (((lz[ii].laser_angle65536)) >> (6));
				#if (0)/*(memo)*/
			//	sin_angle_t256 = (int)(int256_si n1024(/*rot_sin*/((/*OFFS_SIN512+*/				rotation_angle512+rotation_angle512)&(1024-1)))/*<<8*/);
			//	cos_angle_t256 = (int)(int256_si n1024(/*rot_cos*/((	OFFS_COS512+OFFS_COS512+	rotation_angle512+rotation_angle512)&(1024-1)))/*<<8*/);
				sin_angle_t256 = (int)(int256_si n1024(/*rot_sin*/((/*OFFS_SIN1024+*/ 				rotation_angle1024)&(1024-1)))/*<<8*/);
				cos_angle_t256 = (int)(int256_si n1024(/*rot_cos*/((	OFFS_COS1024+				rotation_angle1024)&(1024-1)))/*<<8*/);
				#endif
				//
				#if (0)//
				sin_angle_t256 = ((si n1024((rotation_angle1024)) ) );/*fps_factor*/	/* CCWの場合 */
				cos_angle_t256 = ((co s1024((rotation_angle1024)) ) );/*fps_factor*/
				#else
				int256_sincos1024( (rotation_angle1024), &sin_angle_t256, &cos_angle_t256);
				#endif
			}

		//	#define LINE_TEST_TEXTURE_FLAGS 	(GU_TEXTURE_16BIT | 				GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define LINE_TEST_TEXTURE_FLAGS 	(									GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		//	#define LINE_TEST_TEXTURE_FLAGS 	(				GU_COLOR_8888 | 	GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			//GU_VERTEX_32BITF|GU_TRANSFORM_2D
			// we make local copies of the line into the main buffer here, so we don't have to flush the cache
			Point_Vertex/*_color*/* my_vertices = sceGuGetMemory((5+2) * sizeof(Point_Vertex/*_color*/));
			// create a lineloop
			int e1x;		e1x = (boss_center_x);	/* (回転中心)仮にボスの座標x */
			int e1y;		e1y = (boss_center_y);	/* (回転中心)仮にボスの座標y */
			int p1x;		p1x = 0/*30*/;			/* (回転オフセット)レーザーオフセット距離x */
			int p1y;		p1y = 64;				/* (回転オフセット)レーザーオフセット距離y */
			int p2w;		p2w = (8);				/* レーザー幅の半分 */
			int p2h;		p2h = (256);				/* レーザー長さ */
			/* [下CCW]下が 0度で反時計回りの角度系 */
			int ifx;
			int ify;
			int irx;
			int iry;
	//
			ifx = (p1x)-(p2w);
			ify = (p1y);
			irx = ((ify * sin_angle_t256) + (ifx * cos_angle_t256));	irx = (irx >>8/*16*/);
			iry = ((ify * cos_angle_t256) - (ifx * sin_angle_t256));	iry = (iry >>8/*16*/);
			my_vertices[0].x = (e1x)+(irx); 		my_vertices[0].y = (e1y)+(iry);
			my_vertices[4].x = my_vertices[0].x;	my_vertices[4].y = my_vertices[0].y;
	//
			ifx = (p1x)+(p2w);
			ify = (p1y);
			irx = ((ify * sin_angle_t256) + (ifx * cos_angle_t256));	irx = (irx >>8/*16*/);
			iry = ((ify * cos_angle_t256) - (ifx * sin_angle_t256));	iry = (iry >>8/*16*/);
			my_vertices[1].x = (e1x)+(irx); 		my_vertices[1].y = (e1y)+(iry);
	//
			ifx = (p1x)+(p2w);
			ify = (p1y)+(p2h);
			irx = ((ify * sin_angle_t256) + (ifx * cos_angle_t256));	irx = (irx >>8/*16*/);
			iry = ((ify * cos_angle_t256) - (ifx * sin_angle_t256));	iry = (iry >>8/*16*/);
			my_vertices[2].x = (e1x)+(irx); 		my_vertices[2].y = (e1y)+(iry);
	//
			ifx = (p1x)-(p2w);
			ify = (p1y)+(p2h);
			irx = ((ify * sin_angle_t256) + (ifx * cos_angle_t256));	irx = (irx >>8/*16*/);
			iry = ((ify * cos_angle_t256) - (ifx * sin_angle_t256));	iry = (iry >>8/*16*/);
			my_vertices[3].x = (e1x)+(irx); 		my_vertices[3].y = (e1y)+(iry);
	//
		//	ifx = (p1x);
		//	ify = (p1y);
		//	irx = ((ify * sin_angle_t256) + (ifx * cos_angle_t256));	irx = (irx >>8/*16*/);
		//	iry = ((ify * cos_angle_t256) - (ifx * sin_angle_t256));	iry = (iry >>8/*16*/);
		//	my_vertices[4].x = (irx);				my_vertices[4].y = (iry);
	//
			my_vertices[5].x = (e1x);				my_vertices[5].y = (e1y);	/* 仮にボスの座標 */
			my_vertices[6].x = (player_center_x);	my_vertices[6].y = (player_center_y);
//
		//	my_vertices[0].color = (c1);
		//	my_vertices[1].color = (c1);
		//	my_vertices[2].color = (c2);
		//	my_vertices[3].color = (c2);
		//	my_vertices[4].color = (c1);
		//
		/* 本来回転中心から逆回りの角度へ回転させて判定する */
			ifx = (boss_center_x)-(player_center_x);/* (本来 ifx と -angle) */
			ify = (boss_center_y)-(player_center_y);/* (本来 ify と -angle) */
		//	ifx = (player_center_x)-(boss_center_x);/* 注意：省略計算： ifxは -ifx を計算(本来 ifx と -angle) */
		//	ify = (player_center_y)-(boss_center_y);/* 注意：省略計算： ifyは -ify を計算(本来 ify と -angle) */
				#if 0
				/* [下CCW]下が 0度で反時計回りの角度系 */
				irx = ((ify * sin_angle_t256) + (ifx * cos_angle_t256));	irx = (irx >>8/*16*/);	/*90度進む*/
				iry = ((ify * cos_angle_t256) - (ifx * sin_angle_t256));	iry = (iry >>8/*16*/);	/*90度進む*/
				#endif
				#define sin2_angle	(-sin_angle_t256)	/* sin(-angle) == -sin(angle) */	/* 位相 +180度進み */
				#define cos2_angle	(cos_angle_t256) 	/* cos(-angle) ==  cos(angle) */	/* 位相 +180度進み */
			//	#define sin2_angle	(cos_angle_t256) 	/* 位相  +90度進み */
			//	#define cos2_angle	(-sin_angle_t256)	/* 位相 +180度進み */
			//	#define sin2_angle	(-cos_angle_t256)	/* 位相  +90度進み */
			//	#define cos2_angle	(sin_angle_t256) 	/* 位相 +180度進み */
				irx = ((ify * sin2_angle) + (ifx * cos2_angle));	irx = (irx >>8/*16*/);	/*90度進む*/
				iry = ((ify * cos2_angle) - (ifx * sin2_angle));	iry = (iry >>8/*16*/);	/*90度進む*/
		//	irx = ((ify * cos_angle_t256) + (ifx * sin_angle_t256));	irx = (irx >>8/*16*/);	/*90度遅れる*/
		//	iry = ((ify * sin_angle_t256) - (ifx * cos_angle_t256));	iry = (iry >>8/*16*/);	/*90度遅れる*/
		//
		//	sceGuColor(abgr222/*0xffff*/ /*abgr*/);
		//	sceGuColor(/*0xaaaa*/ ((abgr222&0x7f7f7f7f)>>1) /*abgr*/);
		//	sceGuColor( (0x3f878787) /*abgr*/);
			/* 点(自分) vs 矩形領域(レーザー)で判定 */
			if (
					((-p2w)<irx)	&&
					(irx<(p2w)) 	&&
				//	((p1y)<iry)
				//	((-p1y)<iry)
					((-p1y)>iry)	/* 位相 +180度進みなので辻褄合わせ */
			)
			{
			//	sceGuColor( (0xafff5737) /*abgr*/);/*青*/
				sceGuColor( (0xaf3757ff) /*abgr*/);/*赤*/		/* あたった */
			}
			else
			{
				sceGuColor( (0x7f878787) /*abgr*/);/*白*/		/* あたってない */
			}
			sceGuDrawArray(GU_LINE_STRIP, LINE_TEST_TEXTURE_FLAGS, (5+2), NULL, my_vertices);
		}
	}
	/* テクスチャー有効モードでは、線や面等のフラットポリゴンが使えない */
	PGC_SEND(PGC_TEXTURE_2D, PGC_ON);	//sceGuEnable(GU_TEXTURE_2D); 	/* テクスチャー有効モードに戻す */
}
#endif
