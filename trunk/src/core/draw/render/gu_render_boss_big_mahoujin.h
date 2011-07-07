
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"draw_screen.c" からのみ間接的にインクルードします。
---------------------------------------------------------*/


/*---------------------------------------------------------
	ボスの後ろの魔方陣(大)
---------------------------------------------------------*/

static int aaa_yyy;
static void gu_draw_big_maho_jin(void)
{
	/* --- 半透明合成値 */
	//#if (1==USE_VCOLOR)
	//unsigned int blendlevel = (0xaa2f7fff);/* あかね */
	//#endif
	//#define big_maho_color8888 (0x997f2fff)		/* --- 半透明合成値 */
	/*								透青緑赤 AABBGGRR */
		const unsigned int maho_color_list[8] =
		{	/*AABBGGRR*/
			0x997f2fff, //		0x997f2fff,/**/
			0x77ff993f, //		0x77ff5f1f,/* 1面 */		/* アリス */
			0x553fff3f, //		0x99ff7f2f,/* 2面 */		/* 文 */
			0xaa2f7fff, //		0xaa2f7fff,/* 3面 */		/* 未定 */
			0xaaffffff, //		0x997f2fff,/* 4面 */		/* 輝夜 */
			0x33ff5f1f, //		0x33ff5f1f,/* 5面 */		/* パチェ */
			0x997f2fff, //		0x997f2fff,/* 6面 */		/* 咲夜 */
			0x997f2fff, //		0x997f2fff,/* ending */
		//	0xff601010, //	//	0xff601010,/**/
		}; /* 透青緑赤 AABBGGRR */
	const unsigned int big_maho_color8888 = maho_color_list[cg.game_now_stage&0x07];
//
	unsigned int j;
	unsigned short uv_x4;

	int boss_center_x = ((global_obj_boss->cx256)>>8);
	int boss_center_y = ((global_obj_boss->cy256)>>8);

	/* 内側 */
//	int hankei_111 = (draw_boss_hp_value>>2);/* 128==1024/8 */
//	int hankei_111 = (boss_life_value>>(2+3));/* 128==1024/8 */
//	int hankei_111 = (spell_card_boss_timer>>(2+3))+(64);/* 128==1024/8 */
//	int hankei_111 = (spell_card_boss_timer>>(7))+(64);/* 128==1024/8 */
	int hankei_111 = (spell_card_boss_timer>>(8))+(64);/* 128==1024/8 */
	/* 外側 */
	int hankei_222 = (hankei_111+(16)/*(8)*/);/* 8[dot]文字高さ */

	/* 文字のテクスチャx位置 */
	aaa_yyy++;
	uv_x4 = (/*val5*/(aaa_yyy) /*& 0xff*/);

	#if 1
		/* 分割数は、 mipmap みたいにした方が良いかも。 */
//		#define BM_32V	((16*2)+2)	/* 一周分(+2は重なる分) */
//		#define BM_32V	((16+1)*2)	/* 一周分(+2は重なる分) */
//		#define BM_32V	((32+1)*2)	/* 一周分(+2は重なる分) 32分割 */
		#define BM_32V	((64+1)*2)	/* 一周分(+2は重なる分) 64分割 */
		Vertex_uvcxyz_C32* vertices;
		vertices = (Vertex_uvcxyz_C32*)sceGuGetMemory((BM_32V)*sizeof(Vertex_uvcxyz_C32));
	//
	#endif
/* --- [ 回転文字 ] --- */
//	int rotation_angle512 = (0);
	int rotation_angle1024 = (0);
//	int rotation_angle65536 = (0);
	for (j=0; j<BM_32V; j+=2)/*32分割*/
	{
	int sin_angle;
	int cos_angle;
		#if 1
	//	sin_angle = (int)(int256_sin1024(/*rot_sin*/((/*OFFS_SIN512+*/			  rotation_angle512+rotation_angle512)&(1024-1)))/*<<8*/);
	//	cos_angle = (int)(int256_sin1024(/*rot_cos*/((	OFFS_COS512+OFFS_COS512+  rotation_angle512+rotation_angle512)&(1024-1)))/*<<8*/);
		sin_angle = (int)(int256_sin1024(/*rot_sin*/((/*OFFS_SIN1024+*/ 		  rotation_angle1024)&(1024-1)))/*<<8*/);
		cos_angle = (int)(int256_sin1024(/*rot_cos*/((	OFFS_COS1024+			  rotation_angle1024)&(1024-1)))/*<<8*/);
//		sin_angle = (int)(int256_sin65536(/*rot_sin*/((/*OFFS_SIN65536+*/		  rotation_angle65536)&(65536-1)))/*<<8*/);
//		cos_angle = (int)(int256_sin65536(/*rot_cos*/(( OFFS_COS65536+			  rotation_angle65536)&(65536-1)))/*<<8*/);
		#endif
	int ifx;
	int ify;
		ifx = ((hankei_222 * sin_angle)>>8);
		ify = ((hankei_222 * cos_angle)>>8);
		vertices[j].x = (boss_center_x + ifx);
		vertices[j].y = (boss_center_y + ify);
		vertices[j].z = 0;
		vertices[j].u = uv_x4;
		vertices[j].v = (127);	/*(文字のテクスチャy位置[下限]tex_str__y_low_127)*/
		#if (1==USE_VCOLOR)
		vertices[j].color	= (big_maho_color8888); 	/*blendlevel*/
		#endif
	//
		ifx = ((hankei_111 * sin_angle)>>8);
		ify = ((hankei_111 * cos_angle)>>8);
		vertices[j+1].x = (boss_center_x + ifx);
		vertices[j+1].y = (boss_center_y + ify);
		vertices[j+1].z = 0;
		vertices[j+1].u = uv_x4;
		vertices[j+1].v = (111)/*(111)*/;	/*111=127-16(文字のテクスチャy位置[上限]tex_str_y_high_111)*/
		#if (1==USE_VCOLOR)
		vertices[j+1].color = (big_maho_color8888); 	/*blendlevel*/
		#endif
		uv_x4				+= (32);	/*64*/	/* テクスチャ 32[dots] */
	//	rotation_angle512	+= (16);		/* 角度 */
//		rotation_angle1024	+= (32);		/* 角度 32==(1024/32分割) 1周(1024)を32分割した場合の角度 */
		rotation_angle1024	+= (16);		/* 角度 16==(1024/64分割) 1周(1024)を64分割した場合の角度 */
//		rotation_angle65536 += (1024);		/* 角度 1024==(65536/64分割) 1周(65536)を64分割した場合の角度 */
	}
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (BM_32V), NULL, &vertices[0/*SL_24V*/]);
}
// 1 3 5 7 9
// 0 2 4 6 8

