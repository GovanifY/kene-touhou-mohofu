
/*---------------------------------------------------------
	�ėp�X�v���C�g�̕\�� (��]�g��k������)���[�`��
	-------------------------------------------------------
	����(2009-11-13����)������ slice ���ĂȂ����ǁA�ǂ����c����
	 slice ���Ȃ���(�`���m�⃌�~���A�{���̑��x�ቺ���Ղ肩�画�f����)
	�_���ł��ˁB���Ƃŉ��ǂ��悤�B
	-------------------------------------------------------
	�t�ɒe����(64[dots]�ȉ��� > ��e���炢�Ȃ�) slice ���Ȃ��Ă�
	�����̂�������Ȃ��B
	�e���̓V�X�e���I�ɂ��u�ėp�X�v���C�g�ŕ`���Ȃ������������ǂ��v�̂ŁA
	��p���[�`���ɕ��򂷂�\��(2009-11-13����)�ł��B
	-------------------------------------------------------
	��]���Ȃ��ꍇ�ƈႢ�A
	��]����ꍇ�́A��]�������g���܂��̂ŁA
	�Ō�܂ő��΍��W(obj���S�����_o(0,0)�Ƃ���B)�Ōv�Z���A
	�Ō��obj�̒��S���W(cx,cy)�������ĉ�ʏ�̕`����W(Gu�̎g������1:1�Ȃ̂�)�Ƃ��܂��B
---------------------------------------------------------*/

static void render_object_use_rot_zoom(SPRITE *spr)
{
	/* --- �\���X�C�b�` */
//	if (spr->used ==  0/*FA LSE*/) return;
//	if (spr->Texture == NULL) return;
//
	/* --- ��]�g��O���� */
	int sin_angle;	/* �ʑ�  +0�x�i�� */
	int cos_angle;	/* �ʑ�  +90�x�i�� */
//	int c18_angle;	/* �ʑ� +180�x�i�� */
	{
		/* �p�x��0-1023�x */
		unsigned int rotation_angle1024;
	//	rotation_angle512	= ((spr->rotationCCW1024) / (128/*65536/512*/));	/* �p�x��0-65535�x�Ȃ̂�0-511�x�֕ϊ��B */
		rotation_angle1024	= ((spr->rotationCCW1024) /*>> (7)*/);				/* �p�x��0-1023�x */
		#if 1
	//	sin_angle = (int)(int256_sin1024(/*rot_sin*/((/*OFFS_SIN512+*/			  rotation_angle512+rotation_angle512)&(1024-1)))/*<<8*/);
	//	cos_angle = (int)(int256_sin1024(/*rot_cos*/((	OFFS_COS512+OFFS_COS512+  rotation_angle512+rotation_angle512)&(1024-1)))/*<<8*/);
		sin_angle = (int)(int256_sin1024(/*rot_sin*/((/*OFFS_SIN1024+*/ 		  rotation_angle1024)&(1024-1)))/*<<8*/);
		cos_angle = (int)(int256_sin1024(/*rot_cos*/((	OFFS_COS1024+			  rotation_angle1024)&(1024-1)))/*<<8*/);
		#endif
	}
//
	/* --- �����������l */
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

	/* --- �ėp�X�v���C�g���ꖇ�`�悷�� */
	/* ������x�̑傫�����������X�v���C�g�͒Z����ɕ������Ȃ��� */
	/* �ƂĂ��Ȃ��x���Ȃ� */

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
	/* ��]����̂� x,y ���W�́A�u��ʂ̍��W�ł͂Ȃ��v�A�uobj���S�����_o(0,0)�Ƃ����A���΍��W�v�Ōv�Z���Ă����B */
//	unsigned int/*short*/ x_pos = ((spr->cx256>>8));
//	unsigned int/*short*/ y_pos = ((spr->cy256>>8));
//(r32)	unsigned int/*short*/ x_pos = ((spr->cx256>>8)-(spr->w >> 1));/* ���S���W����摜�T�C�Y�̔����������A������W���v�Z */
//(r32)	unsigned int/*short*/ y_pos = ((spr->cy256>>8)-(spr->h >> 1));/* ���S���W����摜�T�C�Y�̔����������A������W���v�Z */
	unsigned int/*short*/ x_pos = ( -(spr->w >> 1));/* ���S���W����摜�T�C�Y�̔����������A������W���v�Z */
	unsigned int/*short*/ y_pos = ( -(spr->h >> 1));/* ���S���W����摜�T�C�Y�̔����������A������W���v�Z */
	i = 0;
	for (; i<count4; )
	{
	//	if (((i)+1*4) < (count4))	{}	else		/* �Ō� */
		if (i==(count4-4))	/* �Ō� */
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
		/* --- ��]�g�又�� */
		int center_x;
		int center_y;
//		center_x = ((spr->cx256>>8)		) + (spr->w >> 1);/*/2*/
//		center_y = ((spr->cy256>>8)		) + (spr->h >> 1);/*/2*/
		center_x = ((spr->cx256>>8)		) ;/*/2*/
		center_y = ((spr->cy256>>8)		) ;/*/2*/
		int zoom_x256;
		int zoom_y256;
	//	zoom_x256 = (spr->m_zoom_x256) * ((65536/256)); /* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
	//	zoom_y256 = (spr->m_zoom_y256) * ((65536/256)); /* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
		zoom_x256 = (spr->m_zoom_x256) /*<< (8)*/;		/* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
		zoom_y256 = (spr->m_zoom_y256) /*<< (8)*/;		/* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
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
			/* [�ECW]�E�� 0�x�Ŏ��v���̊p�x�n */
			irx = ((ifx * cos_angle) - (ify * sin_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ifx * sin_angle) + (ify * cos_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 1
			/* [��CCW]���� 0�x�Ŕ����v���̊p�x�n */
			irx = ((ify * sin_angle) + (ifx * cos_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ify * cos_angle) - (ifx * sin_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 0
			/* [��CCW(?)]���� 0�x�Ŕ����v���̊p�x�n */
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
	/* --- �`�惊�N�G�X�g */
//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (count4), NULL, vertices);
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (count4), NULL, vertices);
}
