
/*---------------------------------------------------------
	�ėp�X�v���C�g�̕\�� (��]�g��k���Ȃ�)���[�`��
	�ėp�X�v���C�g�̕\��(��ʓ]��)
---------------------------------------------------------*/

/* --- �ėp�X�v���C�g���ꖇ�X�N���[���ɓ\��t���� */
static void render_object_no_rot_zoom(OBJ *spr)
{
	/* --- �\���X�C�b�` */
//	if (spr->used ==  0/*FA LSE*/) return;
//	if (spr->Texture == NULL) return;

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

	unsigned int/*short*/ w_rest;
	w_rest = spr->w + SLICE_64_SIZE;

	unsigned int/*short*/ count2;
	#if 0
	//count2 = (((spr->w / SLICE_64_SIZE)+1)*2);
	#else
	count2 = 0;
//	for (j=0; (j+SLICE_64_SIZE)<=(/*480*/spr->w/*512*/); j+=SLICE_64_SIZE)
	unsigned int/*short*/ i;
	i = 0;
	do
	{
		count2 += 2;
		i += SLICE_64_SIZE;
		w_rest -= SLICE_64_SIZE;
	}
	while ( (i) <(/*480*/spr->w/*512*/));
	#endif
	Vertex_uvcxyz_C32	*vertices;
	vertices = (Vertex_uvcxyz_C32*)sceGuGetMemory(count2 * sizeof(Vertex_uvcxyz_C32));
	i = 0;
	unsigned int/*short*/ pos = 0;
	unsigned int/*short*/ w_size;
	w_size	= SLICE_64_SIZE;
	/* ��]���Ȃ��̂� x,y ���W�́A�u��ʂ̍��W�Łv�܂�A�u��ʂ̍�������_o(0,0)�Ƃ����A��΍��W�v�Ōv�Z���Ă����B */
	unsigned int/*short*/ absolute_x_pos = ((spr->center.x256>>8)-(spr->w >> 1));/* ���S���W����摜�T�C�Y�̔����������A������W���v�Z */
	unsigned int/*short*/ absolute_y_pos = ((spr->center.y256>>8)-(spr->h >> 1));/* ���S���W����摜�T�C�Y�̔����������A������W���v�Z */
	for (; i<count2; )
	{
	//	if ((i1+1) < count)
	//	if (((i)+1*2) < (count2))	{}	else		/* �Ō� */
		if (i==(count2-2))		/* �Ō� */
		{
			w_size = w_rest;
		}
	//
		vertices[(i)].u = (spr->tx + pos);
		vertices[(i)].v = (spr->ty);
		#if (1==USE_VCOLOR)
		vertices[(i)].color = blendlevel;
		#endif
		vertices[(i)].x = (absolute_x_pos + pos);
		vertices[(i)].y = (absolute_y_pos);
		vertices[(i)].z = SPR_PRIORITY;
	//
		vertices[(i)+1].u	= (spr->tx + pos) + w_size;
		vertices[(i)+1].v	= (spr->ty) + spr->h;
		#if (1==USE_VCOLOR)
		vertices[(i)+1].color = blendlevel;
		#endif
		vertices[(i)+1].x	= (absolute_x_pos + pos) + w_size;
		vertices[(i)+1].y	= (absolute_y_pos) + spr->h;
		vertices[(i)+1].z	= SPR_PRIORITY;
		i += 2;
		pos += SLICE_64_SIZE;
	}
	/* --- �`�惊�N�G�X�g */
//	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS4444, (count2), NULL, vertices);
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650_C32, (count2), NULL, vertices);
}
