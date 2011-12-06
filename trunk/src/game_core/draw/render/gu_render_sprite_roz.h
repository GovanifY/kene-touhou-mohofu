
#define USE_DXP_ROZ (0)
#if (1==USE_DXP_ROZ)
/* �I�������Ȃ� */

/*---------------------------------------------------------
	DXP������
---------------------------------------------------------*/
//#include "../graphics.h"
typedef struct DXP_FVF_2DTEX_F
{
	u16 		u, v;
	float		x, y, z;
} DXP_FVF_2DTEX_F;

int DrawModiGraphFI(
	OBJ *spr,
	int x1,
	int y1,
	int x2,
	int y2,
	int x3,
	int y3,
	int x4,
	int y4,
//	int gh,
	int trans )
{//343972
	register int u0;
	register int u1;
	register int v0;
	register int v1;
//	DXPGRAPHICSHANDLE* gptr;
//	GHANDLE2GPTR(gptr, gh);
//	if(dxpGraphicsSetup2DTex(gptr->tex, trans) < 0)return -1;
	unsigned int sw_bit = 32;//dxpPsm2SliceSize_bit[gptr->tex->psm][0];
	unsigned int sh_bit = 32;//dxpPsm2SliceSize_bit[gptr->tex->psm][1];
	unsigned int sw;
	unsigned int sh;
	sw = (1<<sw_bit);
	sh = (1<<sh_bit);
	int gptr_u0;
	int gptr_u1;
	int gptr_v0;
	int gptr_v1;
	gptr_u0 = spr->tx;
	gptr_u1 = spr->tx + spr->w;
	gptr_v0 = spr->ty;
	gptr_v1 = spr->ty + spr->h;

//	u0 = gptr->u0;
//	v0 = gptr->v0;
//	u1 = gptr->u1 - u0;
//	v1 = gptr->v1 - v0;
	u0 = gptr_u0;
	v0 = gptr_v0;
	u1 = gptr_u1 - u0;
	v1 = gptr_v1 - v0;
//	unsigned int swn = (u1 + sw - 1) / sw;
//	unsigned int shn = (v1 + sh - 1) / sh;
	unsigned int swn = (u1 + sw - 1) >> sw_bit;
	unsigned int shn = (v1 + sh - 1) >> sh_bit;
//	DXP_FVF_2DTEX_F *buf = (DXP_FVF_2DTEX_F*)dxpGuGetMemory(sizeof(DXP_FVF_2DTEX_F) * (swn * 2 + 2) * shn);
	DXP_FVF_2DTEX_F *buf = (DXP_FVF_2DTEX_F*)sceGuGetMemory(sizeof(DXP_FVF_2DTEX_F) * (swn * 2 + 2) * shn);
	DXP_FVF_2DTEX_F *vtxbuf = buf;
	register float z = 0;//dxpGraphicsData.z_2d;
	__asm__ volatile(

		"lv.s       S030,   %0\n"		"lv.s   S031,   %4\n"
		"lv.s       S002,   %1\n"		"lv.s   S003,   %5\n"
		"lv.s       S012,   %2\n"		"lv.s   S013,   %6\n"
		"lv.s       S022,   %3\n"		"lv.s   S023,   %7\n"
	//
		"vi2f.p     C030,   C030,   1/2\n"		//int��float�ϊ�
		"vi2f.p     C002,   C002,   1/2\n"		//int��float�ϊ�
		"vi2f.p     C012,   C012,   1/2\n"		//int��float�ϊ�
		"vi2f.p     C022,   C022,   1/2\n"		//int��float�ϊ�
	//
		"vsub.p     C000,   C002,   C030\n"
		"vsub.p     C010,   C022,   C030\n"
		"vsub.p     C020,   C012,   C022\n"
		"vsub.p     C020,   C020,   C000\n"
		"mtv        %8,     S002\n"
		"mtv        %9,     S012\n"
		"vi2f.p     R002,   R002,   1/2\n"
		"vone.s     S033\n"
		"vrcp.p     R002,   R002\n"
		"mtv        %10,    S100\n"
		"mtv        %11,    S110\n"
		"vi2f.p     R100,   R100,   1/2\n"
		:
		:"m"(x1), "m"(x2), "m"(x3), "m"(x4),
			"m"(y1), "m"(y2), "m"(y3), "m"(y4),
			"r"(u1), "r"(v1), "r"(u0), "r"(v0)
	);

//	v0 = gptr->v0;
//	while(v0 < gptr->v1)

	v0 = gptr_v0;
	while(v0 < gptr_v1)
	{
	//	u0 = gptr->u0;
	//	v1 = psp_min(v0 + sh, gptr->v1);
		u0 = gptr_u0;
		v1 = psp_min(v0 + sh, gptr_v1);
		buf = vtxbuf;

		vtxbuf->u = u0;
		vtxbuf->v = v0;
		__asm__ volatile(
			"mtv        %2,     S003\n"
			"mtv        %3,     S013\n"
			"vi2f.p     R003,   R003,   1/2\n"
			"vsub.p     R003,   R003,   R100\n"
			"vmul.p     R003,   R003,   R002\n"
			"vmul.s     S023,   S013,   S003\n"
			"vdot.q     S022,   R003,   R000\n"
			"vdot.q     S032,   R003,   R001\n"
			"sv.s       S022,   %0\n"
			"sv.s       S032,   %1\n"
			:"=m"(vtxbuf->x), "=m"(vtxbuf->y)
			:"r"(u0), "r"(v0)
		);
		vtxbuf->z = z;
		vtxbuf++;

		vtxbuf->u = u0;
		vtxbuf->v = v1;
		__asm__ volatile(
			"mtv        %2,     S003\n"
			"mtv        %3,     S013\n"
			"vi2f.p     R003,   R003,   1/2\n"
			"vsub.p     R003,   R003,   R100\n"
			"vmul.p     R003,   R003,   R002\n"
			"vmul.s     S023,   S013,   S003\n"
			"vdot.q     S022,   R003,   R000\n"
			"vdot.q     S032,   R003,   R001\n"
			"sv.s       S022,   %0\n"
			"sv.s       S032,   %1\n"
			:"=m"(vtxbuf->x), "=m"(vtxbuf->y)
			:"r"(u0), "r"(v1)
		);
		vtxbuf->z = z;
		vtxbuf++;
	//	while(u0 < gptr->u1)
		while(u0 < gptr_u1)
		{
		//	u1 = psp_min(u0 + sw, gptr->u1);
			u1 = psp_min(u0 + sw, gptr_u1);

			vtxbuf->u = u1;
			vtxbuf->v = v0;
			__asm__ volatile(
				"mtv        %2,     S003\n"
				"mtv        %3,     S013\n"
				"vi2f.p     R003,   R003,   1/2\n"
				"vsub.p     R003,   R003,   R100\n"
				"vmul.p     R003,   R003,   R002\n"
				"vmul.s     S023,   S013,   S003\n"
				"vdot.q     S022,   R003,   R000\n"
				"vdot.q     S032,   R003,   R001\n"
				"sv.s       S022,   %0\n"
				"sv.s       S032,   %1\n"
				:"=m"(vtxbuf->x), "=m"(vtxbuf->y)
				:"r"(u1), "r"(v0)
			);
			vtxbuf->z = z;
			vtxbuf++;

			vtxbuf->u = u1;
			vtxbuf->v = v1;
			__asm__ volatile(
				"mtv        %2,     S003\n"
				"mtv        %3,     S013\n"
				"vi2f.p     R003,   R003,   1/2\n"
				"vsub.p     R003,   R003,   R100\n"
				"vmul.p     R003,   R003,   R002\n"
				"vmul.s     S023,   S013,   S003\n"
				"vdot.q     S022,   R003,   R000\n"
				"vdot.q     S032,   R003,   R001\n"
				"sv.s       S022,   %0\n"
				"sv.s       S032,   %1\n"
				:"=m"(vtxbuf->x), "=m"(vtxbuf->y)
				:"r"(u1), "r"(v1)
			);
			vtxbuf->z = z;
			vtxbuf++;

			u0 += sw;
		}
		#define DXP_VTYPE_2DTEX_F (GU_VERTEX_32BITF | GU_TEXTURE_16BIT)
		sceGuDrawArray(GU_TRIANGLE_STRIP, DXP_VTYPE_2DTEX_F | GU_TRANSFORM_2D, 2 * swn + 2, 0, buf);
		v0 += sh;
	}
	return (0);
}

//	DXP_TRANCE_DATA dxpt;
	ScePspIMatrix4 dxp_gi_pos;

static void render_object_use_rot_zoom(OBJ *spr)
//int DrawRotaGraphI256(void)
//	int x,
//	int y,
//	int scale_t256,
//	int angle_1024,
//	int gh,
//	int trans,
//	int turn)
//	dxpt.p01 = int x,					dxpt.p01 = ((x256)>>8);
//	dxpt.p02 = int y,					dxpt.p02 = ((y256)>>8);
//	dxpt.p03 = int scale_t256,			dxpt.p03 = t256(1.0)/*scale_t256*/;
//	dxpt.p04 = int angle1024,			dxpt.p04 = angle1024;
//	dxpt.p05 = int gh,					dxpt.p05 = gazo_handle;
//	dxpt.p06 = int trans,				dxpt.p06 = TRUE;
//	dxpt.p07 = int turn DXPDEFARG(0)	dxpt.p07 = 0;
{
	int x;
	int y;
	int scale_t256;
	int angle_1024;
//	int gh;
	int trans;
	int turn;
	x			 = ((spr->center.x256)>>8);//dxpt.p01;
	y			 = ((spr->center.y256)>>8);//xpt.p02;
	scale_t256	 = (spr->m_zoom.x256);//dxpt.p03;
	angle_1024	 = (spr->rotationCCW1024);//dxpt.p04;
//	gh			 = dxpt.p05;
	trans		 = 1;//dxpt.p06;
	turn		 = 0;//dxpt.p07;
	//
//	DXPGRAPHICSHANDLE *gptr;
//	GHANDLE2GPTR(gptr, gh);
	int gptr_u0;
	int gptr_u1;
	int gptr_v0;
	int gptr_v1;
	gptr_u0 = spr->tx;
	gptr_u1 = spr->tx + spr->w;
	gptr_v0 = spr->ty;
	gptr_v1 = spr->ty + spr->h;
//	register int w = gptr->u1 - gptr->u0;
//	register int h = gptr->v1 - gptr->v0;
	register int w = gptr_u1 - gptr_u0;
	register int h = gptr_v1 - gptr_v0;


//	pspvfpu_use_matrices(NULL, 0, VMAT0 | VMAT1 | VMAT2);//�Ȃ񂩗p�r���悭�킩��Ȃ�
//	ScePspFMatrix4 pos;
	ScePspIMatrix4 pos;

// ---------------- Single (�P)
// [S000] [S010] [S020] [S030]
// [S001] [S011] [S021] [S031]
// [S002] [S012] [S022] [S032]
// [S003] [S013] [S023] [S033]

	__asm__ volatile(
		"mtv        %1,     S000\n" 			//scale
		"mtv        %2,     S001\n" 			//angle
	//	"vi2f.s     S000,   S000,    8\n"		// int��float�ϊ��Ɠ�����1/256�{����B
	//	"vi2f.s     S001,   S001,    8\n"		// int��float�ϊ��Ɠ�����1/256�{����B
		"vi2f.p     C000,   C000,    8\n"		// int��float�ϊ��Ɠ�����1/256�{����B

// scale  [S010] [S020] [S030]
// angle  [S011] [S021] [S031]
// [S002] [S012] [S022] [S032]
// [S003] [S013] [S023] [S033]

	//	"vcst.s     S002,   VFPU_2_PI\n"		//angle�����W�A���P�ʂ���VFPU�����̊p�x�P�ʂɒ���
	//	"vfim.s     S002,   1.000000\n" 		// S001 = (1.0f);	// constant real
	//	"vmul.s     S001,   S001[-x],   S002\n"
		"vmov.s     S001,   S001[-x]\n"			// angle = -angle;

		//--------------�����܂ŁA��  ���o���N0�Ɋ�������

// [��]�s��] [��]�s��] [��]�s��] [S130]
// [��]�s��] [��]�s��] [��]�s��] [S131]
// [��]�s��] [��]�s��] [��]�s��] [S132]
// [S103] [S113] [S123] [S133]

		"vmidt.t    M100\n" 					// ��]�s����쐬
		"vrot.t     C100,   S001,   [ c, s, 0]\n"
		"vrot.t     C110,   S001,   [-s, c, 0]\n"
// [S100] [S110] [S120] [S130]
// [S101] [S111] [S121] [S131]
// [S102] [S112] [S122] [S132]
// [S103] [S113] [S123] [S133]

		"vmscl.t    M100,   M100,   S000\n" 	// ��]�s���scale�{����

		"mtv        %3,     S102\n" 			// x, y���W����]�s��ɓ����B����ŕϊ��p�̍s�񂪊�������
		"mtv        %4,     S112\n"
		"vi2f.p     R102,   R102,   1/2\n"		// int��float�ϊ�
		//--------------�����܂ŁA�ŕϊ��p�̍s�񂪃o���N1�Ɋ�������

		"mtv        %5,     S220\n" 			// �摜�̏c�A���̑傫��
		"mtv        %6,     S221\n"
		"vi2f.p     C220,   C220,   1\n"		// int��float�ϊ��Ɠ�����1/2�{����
		"vone.s     S222\n" 					// 1���Z�b�g�B[w, h, 1]�Ƃ����x�N�g�����ł���̂ŁA������ϊ�����

		"vmov.t     C200,   C220[-x, -y, 1]\n"	// [-w, -h-1]
		"vmov.t     C210,   C220[ x, -y, 1]\n"	// [ w, -h-1]
		"vmov.t     C230,   C220[-x,  y, 1]\n"	// [-w,  h-1]
		//--------------�����܂ŁA��  ���o���N2�Ɋ�������

		"vtfm3.t    C020,   M100,   C220\n" 	// �s��ϊ�
		"vtfm3.t    C000,   M100,   C200\n"
		"vtfm3.t    C010,   M100,   C210\n"
		"vtfm3.t    C030,   M100,   C230\n"
		//
		"vf2iu.q    C000,   C000,    1/2\n" 	// float��int�ϊ�
		"vf2iu.q    C010,   C010,    1/2\n" 	// float��int�ϊ�
		"vf2iu.q    C020,   C020,    1/2\n" 	// float��int�ϊ�
		"vf2iu.q    C030,   C030,    1/2\n" 	// float��int�ϊ�
	//	"vf2iu.q    M000,   M000,    1/2\n" 	// float��int�ϊ�	// Matrix�͎g���Ȃ��炵���B
		//
		"usv.q      C020,   32 + %0\n"			// �ϊ������f�[�^�̏����o��
		"usv.q      C000,    0 + %0\n"
		"usv.q      C010,   16 + %0\n"
		"usv.q      C030,   48 + %0\n"
		:"=m"(pos): "r"(scale_t256), "r"(angle_1024), "r"(x), "r"(y), "r"(w), "r"(h)
	);

	if(turn)
	{
		/*return*/ DrawModiGraphFI(spr,
			pos.y.x, pos.y.y,
			pos.x.x, pos.x.y,
			pos.w.x, pos.w.y,
			pos.z.x, pos.z.y,
		//	gh,
			trans);
	}
	{	/*return*/ DrawModiGraphFI(spr,
			pos.x.x, pos.x.y,
			pos.y.x, pos.y.y,
			pos.z.x, pos.z.y,
			pos.w.x, pos.w.y,
		//	gh,
			trans);
	}
}


#else
/*---------------------------------------------------------
	�ėp�X�v���C�g�̕\�� (��]�g��k������)���[�`��
	-------------------------------------------------------
	����(2009-11-13����)������ slice ���ĂȂ����ǁA�ǂ����c����
	 slice ���Ȃ���(�`���m�⃌�~���A�{���̑��x�ቺ���Ղ肩�画�f����)
	�_���ł��ˁB���Ƃŉ��ǂ��悤�B
	-------------------------------------------------------
	�t�ɒe��(64[pixel]�ȉ��� > ��e���炢�Ȃ�) slice ���Ȃ��Ă�
	�����̂�������Ȃ��B
	�e�̓V�X�e���I�ɂ��u�ėp�X�v���C�g�ŕ`���Ȃ������������ǂ��v�̂ŁA
	��p���[�`���ɕ��򂷂�\��(2009-11-13����)�ł��B
	-------------------------------------------------------
	��]���Ȃ��ꍇ�ƈႢ�A
	��]����ꍇ�́A��]�������g���܂��̂ŁA
	�Ō�܂ő��΍��W(obj���S�����_o(0,0)�Ƃ���B)�Ōv�Z���A
	�Ō��obj�̒��S���W(cx,cy)�������ĉ�ʏ�̕`����W(Gu�̎g������1:1�Ȃ̂�)�Ƃ��܂��B
---------------------------------------------------------*/

static void render_object_use_rot_zoom(OBJ *spr)
{
	/* --- �\���X�C�b�` */
//	if (spr->used ==  0/*FA LSE*/) return;
//	if (spr->Texture == NULL) return;
//
	/* --- ��]�g��O���� */
	int sin_angle_t256; 	/* �ʑ�  +0�x�i�� */
	int cos_angle_t256; 	/* �ʑ�  +90�x�i�� */
//	int c18_angle;	/* �ʑ� +180�x�i�� */
	{
		/* �p�x��0-1023�x */
		#if (0)/*(memo)*/
	//	sin_angle_t256 = (int)(int256_si n1024(/*rot_sin*/((/*OFFS_SIN512+*/			  rotation_angle512+rotation_angle512)&(1024-1)))/*<<8*/);
	//	cos_angle_t256 = (int)(int256_si n1024(/*rot_cos*/((	OFFS_COS512+OFFS_COS512+  rotation_angle512+rotation_angle512)&(1024-1)))/*<<8*/);
		sin_angle_t256 = (int)(int256_si n1024(/*rot_sin*/((/*OFFS_SIN1024+*/ 			  (spr->rotationCCW1024))&(1024-1)))/*<<8*/);
		cos_angle_t256 = (int)(int256_si n1024(/*rot_cos*/((	OFFS_COS1024+			  (spr->rotationCCW1024))&(1024-1)))/*<<8*/);
		#endif
		//
		#if (0)//
		sin_angle_t256 = ((si n1024(((spr->rotationCCW1024))) ) );/*fps_factor*/	/* CCW�̏ꍇ */
		cos_angle_t256 = ((co s1024(((spr->rotationCCW1024))) ) );/*fps_factor*/
		#else
		int256_sincos1024( ((spr->rotationCCW1024)), &sin_angle_t256, &cos_angle_t256);
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
//	unsigned int/*short*/ x_pos = ((spr->center.x256>>8));
//	unsigned int/*short*/ y_pos = ((spr->center.y256>>8));
//(r32) unsigned int/*short*/ x_pos = ((spr->center.x256>>8)-(spr->w >> 1));/* ���S���W����摜�T�C�Y�̔����������A������W���v�Z */
//(r32) unsigned int/*short*/ y_pos = ((spr->center.y256>>8)-(spr->h >> 1));/* ���S���W����摜�T�C�Y�̔����������A������W���v�Z */
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
//		center_x = ((spr->center.x256>>8) 	) + (spr->w >> 1);/*/2*/
//		center_y = ((spr->center.y256>>8) 	) + (spr->h >> 1);/*/2*/
		center_x = ((spr->center.x256>>8) 	) ;/*/2*/
		center_y = ((spr->center.y256>>8) 	) ;/*/2*/
		int zoom_x256;
		int zoom_y256;
	//	zoom_x256 = (spr->m_zoom.x256) * ((65536/256)); /* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
	//	zoom_y256 = (spr->m_zoom.y256) * ((65536/256)); /* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
		zoom_x256 = (spr->m_zoom.x256) /*<< (8)*/;		/* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
		zoom_y256 = (spr->m_zoom.y256) /*<< (8)*/;		/* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
		unsigned int j;
		for (j=0; j<4; j++)
		{
			int ifx;
			int ify;
			int irx;
			int iry;
			ifx = vertices[(i)+j].x;
			ify = vertices[(i)+j].y;
//(r33) 		ifx -= center_x;
//(r33) 		ify -= center_y;
//1859267 -> 1859251
//			rx = ((fx * sprite_cos[rot]) - (fy * sprite_sin[rot])) * spr->zoomx;
//			ry = ((fx * sprite_sin[rot]) + (fy * sprite_cos[rot])) * spr->zoomy;
			#if 0
			/* [�ECW]�E�� 0�x�Ŏ��v���̊p�x�n */
			irx = ((ifx * cos_angle_t256) - (ify * sin_angle_t256));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ifx * sin_angle_t256) + (ify * cos_angle_t256));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 1
			/* [��CCW]���� 0�x�Ŕ����v���̊p�x�n */
			irx = ((ify * sin_angle_t256) + (ifx * cos_angle_t256));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ify * cos_angle_t256) - (ifx * sin_angle_t256));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 0
			/* [��CCW(?)]���� 0�x�Ŕ����v���̊p�x�n */
			irx = ((ifx * sin_angle_t256) - (ify * cos_angle_t256));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ifx * cos_angle_t256) + (ify * sin_angle_t256));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
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
#endif
