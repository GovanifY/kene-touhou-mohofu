
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"draw_screen.c" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�{�X�̌��̖����w(��)
---------------------------------------------------------*/

static int aaa_yyy;
static void gu_draw_big_maho_jin(void)
{
	/* --- �����������l */
	//#if (1==USE_VCOLOR)
	//unsigned int blendlevel = (0xaa2f7fff);/* ������ */
	//#endif
	//#define big_maho_color8888 (0x997f2fff)		/* --- �����������l */
	/*								���ΐ� AABBGGRR */
		const unsigned int maho_color_list[8] =
		{	/*AABBGGRR*/
			0x997f2fff, //		0x997f2fff,/**/
			0x77ff993f, //		0x77ff5f1f,/* 1�� �A���X */
			0x553fff3f, //		0x99ff7f2f,/* 2�� �� */
			0xaa2f7fff, //		0xaa2f7fff,/* 3�� ���� */
			0xaaffffff, //		0x997f2fff,/* 4�� �P�� */
			0x33ff5f1f, //		0x33ff5f1f,/* 5�� �p�`�F */
			0x997f2fff, //		0x997f2fff,/* 6�� ��� */
			0x997f2fff, //		0x997f2fff,/* ending */
		//	0xff601010, //	//	0xff601010,/**/
		}; /* ���ΐ� AABBGGRR */
	const unsigned int big_maho_color8888 = maho_color_list[cg.game_now_stage&0x07];
//
	unsigned int j;
	unsigned short uv_x4;
	OBJ *obj_boss;
	obj_boss			= &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+TEKI_OBJ_00_BOSS00_HONTAI];/*(�{�X�{��)*/
	int boss_center_x	= ((obj_boss->center.x256)>>8);
	int boss_center_y	= ((obj_boss->center.y256)>>8);

	/* ���� */
//	int hankei_111 = (draw_bo ss_hp_value>>2);/* 128==1024/8 */
//	int hankei_111 = (bo ss_life_value>>(2+3));/* 128==1024/8 */
//	int hankei_111 = (card.card_timer>>(2+3))+(64);/* 128==1024/8 */
//	int hankei_111 = (card.card_timer>>(7))+(64);/* 128==1024/8 */
	int hankei_111 = (card.card_timer>>(8))+(64);/* 128==1024/8 */
	hankei_111 = psp_max(hankei_111, 64);
	/* �O�� */
	int hankei_222 = (hankei_111+(16)/*(8)*/);/* 8[pixel]�������� */

	/* �����̃e�N�X�`��x�ʒu */
	aaa_yyy++;
	uv_x4 = (/*val5*/(aaa_yyy) /*& 0xff*/);

	#if 1
		/* �������́A mipmap �݂����ɂ��������ǂ������B */
//		#define BM_32V	((16*2)+2)	/* �����(+2�͏d�Ȃ镪) */
//		#define BM_32V	((16+1)*2)	/* �����(+2�͏d�Ȃ镪) */
//		#define BM_32V	((32+1)*2)	/* �����(+2�͏d�Ȃ镪) 32���� */
		#define BM_32V	((64+1)*2)	/* �����(+2�͏d�Ȃ镪) 64���� */
		Vertex_uvcxyz_C32* vertices;
		vertices = (Vertex_uvcxyz_C32*)sceGuGetMemory((BM_32V)*sizeof(Vertex_uvcxyz_C32));
	//
	#endif
/* --- [ ��]���� ] --- */
//	int rotation_angle512 = (0);
	int rotation_angle1024 = (0);
//	int rotation_angle65536 = (0);
	for (j=0; j<BM_32V; j+=2)/*32����*/
	{
	int sin_angle_t256;
	int cos_angle_t256;
		#if (0)/*(memo)*/
	//	sin_angle_t256 = (int)(int256_si n1024(/*rot_sin*/((/*OFFS_SIN512+*/			  rotation_angle512+rotation_angle512)&(1024-1)))/*<<8*/);
	//	cos_angle_t256 = (int)(int256_si n1024(/*rot_cos*/((	OFFS_COS512+OFFS_COS512+  rotation_angle512+rotation_angle512)&(1024-1)))/*<<8*/);
		sin_angle_t256 = (int)(int256_si n1024(/*rot_sin*/((/*OFFS_SIN1024+*/			  rotation_angle1024)&(1024-1)))/*<<8*/);
		cos_angle_t256 = (int)(int256_si n1024(/*rot_cos*/((	OFFS_COS1024+			  rotation_angle1024)&(1024-1)))/*<<8*/);
//		sin_angle_t256 = (int)(int256_si n65536(/*rot_sin*/((/*OFFS_SIN65536+*/ 		  rotation_angle65536)&(65536-1)))/*<<8*/);
//		cos_angle_t256 = (int)(int256_si n65536(/*rot_cos*/(( OFFS_COS65536+			  rotation_angle65536)&(65536-1)))/*<<8*/);
		#endif
		//
		#if (0)//
		sin_angle_t256 = ((si n1024((rotation_angle1024)) ) );/*fps_factor*/	/* CCW�̏ꍇ */
		cos_angle_t256 = ((co s1024((rotation_angle1024)) ) );/*fps_factor*/
		#else
		int256_sincos1024( (rotation_angle1024), &sin_angle_t256, &cos_angle_t256);
		#endif
	int ifx;
	int ify;
		ifx = ((hankei_222 * sin_angle_t256)>>8);
		ify = ((hankei_222 * cos_angle_t256)>>8);
		vertices[j].x = (boss_center_x + ifx);
		vertices[j].y = (boss_center_y + ify);
		vertices[j].z = 0;
		vertices[j].u = uv_x4;
		vertices[j].v = (127);	/*(�����̃e�N�X�`��y�ʒu[����]tex_str__y_low_127)*/
		#if (1==USE_VCOLOR)
		vertices[j].color	= (big_maho_color8888); 	/*blendlevel*/
		#endif
	//
		ifx = ((hankei_111 * sin_angle_t256)>>8);
		ify = ((hankei_111 * cos_angle_t256)>>8);
		vertices[j+1].x = (boss_center_x + ifx);
		vertices[j+1].y = (boss_center_y + ify);
		vertices[j+1].z = 0;
		vertices[j+1].u = uv_x4;
		vertices[j+1].v = (111)/*(111)*/;	/*111=127-16(�����̃e�N�X�`��y�ʒu[���]tex_str_y_high_111)*/
		#if (1==USE_VCOLOR)
		vertices[j+1].color = (big_maho_color8888); 	/*blendlevel*/
		#endif
		uv_x4				+= (32);	/*64*/	/* �e�N�X�`�� 32[pixel] */
	//	rotation_angle512	+= (16);		/* �p�x */
//		rotation_angle1024	+= (32);		/* �p�x 32==(1024/32����) 1��(1024)��32���������ꍇ�̊p�x */
		rotation_angle1024	+= (16);		/* �p�x 16==(1024/64����) 1��(1024)��64���������ꍇ�̊p�x */
//		rotation_angle65536 += (1024);		/* �p�x 1024==(65536/64����) 1��(65536)��64���������ꍇ�̊p�x */
	}
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (BM_32V), NULL, &vertices[0/*SL_24V*/]);
}
// 1 3 5 7 9
// 0 2 4 6 8

