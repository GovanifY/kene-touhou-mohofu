
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"draw_screen.c" からのみ間接的にインクルードします。
---------------------------------------------------------*/


/*---------------------------------------------------------
	☆ フロント面エフェクト(テクスチャ共用)
	-------------------------------------------------------
	自分がボム(カード)撃った場合に出る。文字のロゴ
	Border Power of Spiritual 霊歌で力を発揮する境界
	-------------------------------------------------------
	[ 横文字 ] / [ 縦文字 ] に関しては uvスクロール(テクスチャスクロール) を使ってます。
	ってゆーか本物も(Direct 3Dで uvスクロール は簡単に出来るので)
	実装方式は uvスクロール だと思うよ。
---------------------------------------------------------*/

static void gu_draw_front_spell_logo(void)
{
//	#define SL_20V (9*2)	/* 画面途中。16ドット足りない。((9-1)*32==256 < 272 < 288==(10-1)*32) [32slice] */
	#define SL_20V (10*2)	/* 画面喰み出す。16ドット多い。((9-1)*32==256 < 272 < 288==(10-1)*32) [32slice] */
	#define SL_24V (12*2)	/* 画面丁度。((12-1)*32 == 352 == 480[pspの画面描画幅]-128[パネル幅]) [32slice] */

	Vertex_uvcxyz_C32* vertices;
	vertices = (Vertex_uvcxyz_C32*)sceGuGetMemory((SL_24V+SL_20V)*sizeof(Vertex_uvcxyz_C32));	/* [ 横文字 + 縦文字 ] 分確保 */
//
	/* --- 半透明合成値 */
	#if (1==USE_VCOLOR)
		#if 0//(16==USE_BLIT_COLOR_BIT)
		/*ARGB4444*/
//	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = ((( 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = (((conv_bg_alpha &  0xf0) << 8) | 0x0fff);
	unsigned /*int*/short blendlevel = (0x7777);
//(conv_bg_alpha)
		#else
//	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
//	#define SL_BLENDLEVEL (0x3f3f3f3f)	/*	63 淡すぎる */
	#define SL_BLENDLEVEL (0x64646464)	/* 100 */
//	#define SL_BLENDLEVEL (0x7f7f7f7f)	/* 127 暗い */
//	#define SL_BLENDLEVEL (0xc8c8c8c8)	/* 200 暗すぎる */
		#endif
	#endif
//
	unsigned int j;
	unsigned short x5;
	unsigned short x4;

/* Border Power of Spiritual 文字のテクスチャ位置(上側) */
#define TEX_STR_Y_HIGH_217		(200)/*217*/
/* Border Power of Spiritual 文字のテクスチャ位置(下側) */
#define TEX_STR__Y_LOW_255		(255)
/* Border Power of Spiritual 文字の高さ(スライス幅64以内にしないと、大幅に速度低下すると思う) */
#define SPELL_LOGO_HEIGHT_64	(64)/*40*/
/* --- [ 横文字 ] --- */
/* SPELL_LOGO_LOCATE_Y01_160: 画面左上からの yオフセット(距離) */
#define SPELL_LOGO_LOCATE_Y01_160 (160)
#define SPELL_LOGO_LOCATE_Y02_200 (SPELL_LOGO_LOCATE_Y01_160+SPELL_LOGO_HEIGHT_64)
	x5 = (GAME_X_OFFSET+0);
	x4 = (/*val5*/(cg.bomber_time) /*& 0xff*/);
	for (j=0; j<SL_24V; j+=2)
	{
		vertices[j].x = x5;
		vertices[j].y = SPELL_LOGO_LOCATE_Y02_200;
		vertices[j].z = 0;
		vertices[j].u = x4;
		vertices[j].v = (TEX_STR__Y_LOW_255);
		#if (1==USE_VCOLOR)
		vertices[j].color	= SL_BLENDLEVEL;
		#endif
	//
		vertices[j+1].x = x5;
		vertices[j+1].y = SPELL_LOGO_LOCATE_Y01_160;
		vertices[j+1].z = 0;
		vertices[j+1].u = x4;
		vertices[j+1].v = (TEX_STR_Y_HIGH_217);
		#if (1==USE_VCOLOR)
		vertices[j+1].color = SL_BLENDLEVEL;
		#endif
		x5 += (32);/* 上/下縁側 */
		x4 += (32);/* テクスチャ */
	}

/* --- [ 縦文字 ] --- */
/* SPELL_LOGO_LOCATE_X01_200: 画面左上からの xオフセット(距離) */
#define SPELL_LOGO_LOCATE_X01_200 (GAME_X_OFFSET+(16))/*200*/
#define SPELL_LOGO_LOCATE_X02_240 (SPELL_LOGO_LOCATE_X01_200+SPELL_LOGO_HEIGHT_64)
#define y5 x5
	y5 = 0; 	//	x4 = (val5 & 0xff);
	for (/*j=0*/; j<SL_24V+SL_20V; j+=2)	/* 続きから(jは途中から) */
	{
		vertices[j].x = SPELL_LOGO_LOCATE_X01_200;
		vertices[j].y = y5;
		vertices[j].z = 0;
		vertices[j].u = x4;
		vertices[j].v = (TEX_STR__Y_LOW_255);
		#if (1==USE_VCOLOR)
		vertices[j].color	= SL_BLENDLEVEL;
		#endif
	//
		vertices[j+1].x = SPELL_LOGO_LOCATE_X02_240;
		vertices[j+1].y = y5;
		vertices[j+1].z = 0;
		vertices[j+1].u = x4;
		vertices[j+1].v = (TEX_STR_Y_HIGH_217);
		#if (1==USE_VCOLOR)
		vertices[j+1].color = SL_BLENDLEVEL;
		#endif
		y5 += (32);/* 上/下縁側 */
		x4 += (32);/* テクスチャ */
	}
	#if 0
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (SL_24V), NULL, vertices); 			/* [ 横文字 ] */
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (SL_20V), NULL, &vertices[SL_24V]);	/* [ 縦文字 ] */
	#else
	/* 注意：カリング有効の場合、同等になります：裏側ポリゴンが描かれない事を利用したハック。(描かれない==(描画前に処理を撥ねるので)速度低下しない) */
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (SL_24V+SL_20V), NULL, vertices);		/* [ 横文字 + 縦文字 ] */
	#endif
}
// 1 3 5 7 9
// 0 2 4 6 8

