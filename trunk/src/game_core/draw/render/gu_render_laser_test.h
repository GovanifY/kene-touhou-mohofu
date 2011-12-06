
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"draw_screen.c" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#if (1==LINE_TEST)
/*---------------------------------------------------------
	���[�U�[�e�X�g(�e�X�g��)
---------------------------------------------------------*/
static void gu_blit_laser_test(void)
{
	OBJ *obj_boss;
	obj_boss			= &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+TEKI_OBJ_00_BOSS00_HONTAI];/*(�{�X�{��)*/
	int boss_center_x = ((obj_boss->center.x256)>>8);/*+(16)*/
	int boss_center_y = ((obj_boss->center.y256)>>8);/*+(24)*/
	OBJ *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
	int player_center_x = ((zzz_player->center.x256)>>8);/*+(16)*/
	int player_center_y = ((zzz_player->center.y256)>>8);/*+(24)*/

	/* --  ��`�� */
	/* �e�N�X�`���[������ƃe�N�X�`���[�D��Ńt���b�g�|���S�����`��o���Ȃ��̂� */
	/* �ꎞ�I�Ƀe�N�X�`���[�������[�h�ɐ؂�ւ��� */
	PGC_SEND(PGC_TEXTURE_2D, PGC_OFF);	//sceGuDisable(GU_TEXTURE_2D);/* �e�N�X�`���[�������[�h�ɂ��� */
	/* ���[�U�[�`��e�X�g */
	unsigned int ii;
	for (ii=0; ii<MAX_99_LASER; ii++)
	{
		if (LASER_TYPE_00_OFF != (lz[ii].laser_type))/* lz[ii]==0;�̏ꍇ�Aoff�B(���[�U�[���[�h 0:off, 1:on) */
		{
			/* --- ��]�g��O���� */
			int sin_angle_t256;
			int cos_angle_t256;
			{
				/* �p�x��0-65535�x */
			//	static unsigned int rotation_angle65536;
			//	rotation_angle65536 += (3);/*50*/
			//
				/* �p�x��0-1023�x */
				unsigned int rotation_angle1024;
			//	rotation_angle512	= ((spr->rotation_1024z) / (128/*65536/512*/)); 	/* �p�x��0-65535�x�Ȃ̂�0-511�x�֕ϊ��B */
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
				sin_angle_t256 = ((si n1024((rotation_angle1024)) ) );/*fps_factor*/	/* CCW�̏ꍇ */
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
			int e1x;		e1x = (boss_center_x);	/* (��]���S)���Ƀ{�X�̍��Wx */
			int e1y;		e1y = (boss_center_y);	/* (��]���S)���Ƀ{�X�̍��Wy */
			int p1x;		p1x = 0/*30*/;			/* (��]�I�t�Z�b�g)���[�U�[�I�t�Z�b�g����x */
			int p1y;		p1y = 64;				/* (��]�I�t�Z�b�g)���[�U�[�I�t�Z�b�g����y */
			int p2w;		p2w = (8);				/* ���[�U�[���̔��� */
			int p2h;		p2h = (256);				/* ���[�U�[���� */
			/* [��CCW]���� 0�x�Ŕ����v���̊p�x�n */
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
			my_vertices[5].x = (e1x);				my_vertices[5].y = (e1y);	/* ���Ƀ{�X�̍��W */
			my_vertices[6].x = (player_center_x);	my_vertices[6].y = (player_center_y);
//
		//	my_vertices[0].color = (c1);
		//	my_vertices[1].color = (c1);
		//	my_vertices[2].color = (c2);
		//	my_vertices[3].color = (c2);
		//	my_vertices[4].color = (c1);
		//
		/* �{����]���S����t���̊p�x�։�]�����Ĕ��肷�� */
			ifx = (boss_center_x)-(player_center_x);/* (�{�� ifx �� -angle) */
			ify = (boss_center_y)-(player_center_y);/* (�{�� ify �� -angle) */
		//	ifx = (player_center_x)-(boss_center_x);/* ���ӁF�ȗ��v�Z�F ifx�� -ifx ���v�Z(�{�� ifx �� -angle) */
		//	ify = (player_center_y)-(boss_center_y);/* ���ӁF�ȗ��v�Z�F ify�� -ify ���v�Z(�{�� ify �� -angle) */
				#if 0
				/* [��CCW]���� 0�x�Ŕ����v���̊p�x�n */
				irx = ((ify * sin_angle_t256) + (ifx * cos_angle_t256));	irx = (irx >>8/*16*/);	/*90�x�i��*/
				iry = ((ify * cos_angle_t256) - (ifx * sin_angle_t256));	iry = (iry >>8/*16*/);	/*90�x�i��*/
				#endif
				#define sin2_angle	(-sin_angle_t256)	/* sin(-angle) == -sin(angle) */	/* �ʑ� +180�x�i�� */
				#define cos2_angle	(cos_angle_t256) 	/* cos(-angle) ==  cos(angle) */	/* �ʑ� +180�x�i�� */
			//	#define sin2_angle	(cos_angle_t256) 	/* �ʑ�  +90�x�i�� */
			//	#define cos2_angle	(-sin_angle_t256)	/* �ʑ� +180�x�i�� */
			//	#define sin2_angle	(-cos_angle_t256)	/* �ʑ�  +90�x�i�� */
			//	#define cos2_angle	(sin_angle_t256) 	/* �ʑ� +180�x�i�� */
				irx = ((ify * sin2_angle) + (ifx * cos2_angle));	irx = (irx >>8/*16*/);	/*90�x�i��*/
				iry = ((ify * cos2_angle) - (ifx * sin2_angle));	iry = (iry >>8/*16*/);	/*90�x�i��*/
		//	irx = ((ify * cos_angle_t256) + (ifx * sin_angle_t256));	irx = (irx >>8/*16*/);	/*90�x�x���*/
		//	iry = ((ify * sin_angle_t256) - (ifx * cos_angle_t256));	iry = (iry >>8/*16*/);	/*90�x�x���*/
		//
		//	sceGuColor(abgr222/*0xffff*/ /*abgr*/);
		//	sceGuColor(/*0xaaaa*/ ((abgr222&0x7f7f7f7f)>>1) /*abgr*/);
		//	sceGuColor( (0x3f878787) /*abgr*/);
			/* �_(����) vs ��`�̈�(���[�U�[)�Ŕ��� */
			if (
					((-p2w)<irx)	&&
					(irx<(p2w)) 	&&
				//	((p1y)<iry)
				//	((-p1y)<iry)
					((-p1y)>iry)	/* �ʑ� +180�x�i�݂Ȃ̂Œ��덇�킹 */
			)
			{
			//	sceGuColor( (0xafff5737) /*abgr*/);/*��*/
				sceGuColor( (0xaf3757ff) /*abgr*/);/*��*/		/* �������� */
			}
			else
			{
				sceGuColor( (0x7f878787) /*abgr*/);/*��*/		/* �������ĂȂ� */
			}
			sceGuDrawArray(GU_LINE_STRIP, LINE_TEST_TEXTURE_FLAGS, (5+2), NULL, my_vertices);
		}
	}
	/* �e�N�X�`���[�L�����[�h�ł́A����ʓ��̃t���b�g�|���S�����g���Ȃ� */
	PGC_SEND(PGC_TEXTURE_2D, PGC_ON);	//sceGuEnable(GU_TEXTURE_2D); 	/* �e�N�X�`���[�L�����[�h�ɖ߂� */
}
#endif
