
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	スコアパネル(表示)関連
	-------------------------------------------------------
	現在(r33)SDLの為、処理落ちするとかなり酷いです。
---------------------------------------------------------*/
#include "111_my_file.h"/*(my_file_common_name)*/

static SDL_Surface *panel_base; 	// パネルベース
static SDL_Surface *star_gauge;

/* サイドパネルの横表示位置 pannel x offset */
//#define PPP (380)
#define PPP (GAME_WIDTH)
#if 0/*(メモ)*/
	#define STR_MAX 		"MAX"
	#define STR_MISSILE 	"MISSILE"
	#define STR_WEAPON_UP	"WEAPON_UP"
	#define STR_LASER		"LASER"
	#define STR_BOMB		"BOMB"
	#define STR_TIME_		"TIME :"
	#define STR_EXTRA_		"EXTRA  :"
	//#define STR_MAX		"ABCD"			/*4つ*/
	//#define STR_MISSILE	"EFGHIJ"		/*6つ*/
	//#define STR_WEAPON_UP "KLMNOPQR"		/*8つ*/
	//#define STR_LASER 	"STUVW" 		/*5つ*/
	//#define STR_BOMB		"XYZab" 		/*5つ*/
	//#define STR_TIME_ 	"cdefg" 		/*5つ*/
	//#define STR_EXTRA_	"hijkl" 		/*5つ*/
//
//	#define STR_MAX 		"ABCD"			/*4つ*/
//	#define STR_EASY		"EFGH" " "  	/*4つ*/
//	#define STR_NORMAL		"IJKLM"     	/*5つ*/
//	#define STR_HARD		"NOPQ" " "  	/*4つ*/
//	#define STR_LUNATIC 	"RSTUV" 		/*5つ*/
//	#define CHR_PIRIOD_ 	'W' 			/*1つ*/
//	#define STR_FPS_		"XYZ"			/*3つ*/
//	#define STR_EXTRA		"XYZab" 		/*5つ*/ 	/* 未使用(r33) */
//	#define STR_TIME_		"cdef"			/*4つ*/ 	/* 地味に処理落ち要因になっていたので廃止。(r33) */
//	#define STR_RANK_		"hijkl" 		/*5つ*/
//	#define STR_FPS_		"hij"			/*3つ*/
//	#define CHR_PIRIOD_ 	'g' 			/*1つ*/
//	#define STR_ENEMY		"klmno" 		/*5つ*/
#endif

	//(r35font)
	#define STR_MAX 		"ABCD"			/*4つ*/
	#define STR_EASY		"EFGH" " "  	/*4つ*/
	#define STR_NORMAL		"JKLMN"     	/*5つ*/
	#define STR_HARD		"PQRS" " "  	/*4つ*/
	#define STR_LUNATIC 	"UVWXY" 		/*5つ*/
	#define CHR_PIRIOD_ 	'a' 			/*1つ*/
	#define STR_FPS_		"bcd"			/*3つ*/

/*---------------------------------------------------------
 * 10進数表示を行う。
 * 数字がない上位桁の 0 は 表示しない。
 * 常に最大桁数分の表示になる。
 *
 * @param num [in] 表示する値
 * @param size [in] 表示する最大桁数
 * @param sprite [in] 使用するスプライトワークの先頭番号
 * @param x [in] 表示するパネル内位置 X
 * @param y [in] 表示するパネル内位置 Y
---------------------------------------------------------*/
global void dec_print_format(
		unsigned int num,
		int size,
//		int sprite,
//		int x,
//		int y
		char *my_str
)
{
	int sss;
//	int j;
	//unsigned int n;
	unsigned int ch;
	const u32 sub10[10/*8*/]=
	{
		1,
		10,
		100,
		1000,
		//
		10000,
		100000,
		1000000,
		10000000,
		//
		100000000,
		1000000000//,
	//	2147483647	== 0x7fffffff == signed int 32bit  max
	};
	sss = 0;
	num++;/*最後桁の辻褄合わせ*/
//	j = 256* (x);
	while (size)
	{
		size--;
		ch = 0;
		while (num > sub10[size])
		{
			num -= sub10[size];
			ch++;
			sss = (1);
		}
		if (
			(0 != sss) //&& /* 上位桁の 0 は表示しない */
			//(0!=size) 	/* 1桁目は必ず表示する */
		)
		{
	//	//	obj_01_panel[sprite+size].x256			= 256* (x + ((size - 1 - i) * 16/DIV2));
	//		obj_01_panel[sprite+size].x256			= (j);
	//		obj_01_panel[sprite+size].y256			= 256* (y);
	//		obj_01_panel[sprite+size].w 			= TEX1_W_STR_09;
	//		obj_01_panel[sprite+size].h 			= TEX1_H_STR_09;
	//	//	obj_01_panel[sprite+size].m_Priority	= BS_DEPTH_PANEL_ITEM;
	//		obj_01_panel[sprite+size].alpha 		= 255;	/* α値(255 で不透明、0 で透明) */
	//		obj_01_panel[sprite+size].texture_x 	= TEX1_X_STR_09+(/*9 **/16* c);
	//		obj_01_panel[sprite+size].texture_y 	= TEX1_Y_STR_09;
	//	//	obj_01_panel[sprite+size].m_TexW		= TEX1_W_STR_09;
	//	//	obj_01_panel[sprite+size].m_TexH		= TEX1_H_STR_09;
	//	//	obj_01_panel[sprite+size].m_RollX		= 0.0;
	//	//	obj_01_panel[sprite+size].m_RollY		= 0.0;
	//		obj_01_panel[sprite+size].m_RollZ		= 0.0;
	//		obj_01_panel[sprite+size].m_Use 		= TRUE;
	//	//	obj_01_panel[sprite+size].m_PosCenter	= FALSE;
			(*my_str) = ('0'+ch);
		}
	//	else
	//	{
	//		;// 	obj_01_panel[sprite+size].m_Use 	= FALSE;
	//	}
		my_str++;
	//	j += ( 256* (16/DIV2) );
	}
}



/*---------------------------------------------------------
	プレイヤーの星の表示の子関数
---------------------------------------------------------*/

enum
{
	R_00_aka_hosi_png = 0,
	R_01_mizu_hosi_png = 10,
};

static void draw_stars_status(int img_num, int value, int y_offset)
{
	SDL_Rect rect_dest;
	SDL_Rect rect_src;
	if (value<1)	{	value=0;}
//
	rect_src.x = (0);
	rect_src.y = (img_num);/*(0)*/
	rect_src.w = (10*value);
	rect_src.h = (10);
//
	rect_dest.x = PSP_WIDTH480-4-(10*value);
	rect_dest.y = y_offset;
	rect_dest.w = 100;
	rect_dest.h = 11;
	PSPL_UpperBlit(star_gauge, &rect_src, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_dest);
}

/*---------------------------------------------------------
	プレイヤーのウェポンゲージの表示の子関数
---------------------------------------------------------*/

static void draw_power_gauge(int weapon)/*, int dx, int dy*/
{
	#define WP_GAUGE_X_OFS (PPP+48-2)
	#define WP_GAUGE_Y_OFS (128+8+4)
	SDL_Rect rect_dest;
	SDL_Rect rect_src;
	rect_src.x = (0);
	rect_src.y = (20);/*(0)*/
	rect_src.h = (13);
	#if 0
//	rect_src.w = (int)((dou ble)weapon / 127/*128*/ * (dou ble)power_gauge->w);
	#else
	/* 1[dot]ぐらい誤差あるかもしれないけど簡略化(高速化) */
	rect_src.w = ((int)(weapon * (80)/*power_gauge->w*/)>>7);
	#endif
	rect_dest.w = (80);/*power_gauge->w*/
	rect_dest.h = (13);/*power_gauge->h*/
	rect_dest.x = WP_GAUGE_X_OFS;/*dx*/
	rect_dest.y = WP_GAUGE_Y_OFS;/*dy*/
	PSPL_UpperBlit(star_gauge/*power_gauge*/, &rect_src, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_dest);
}

/*---------------------------------------------------------
	スコアパネルのステータス表示
---------------------------------------------------------*/

//static unsigned int psp_get_fps60(void);

#if (1==USE_EXTEND_CHECK)
extern void player_check_extend_score(void);
#endif

/*---------------------------------------------------------
	60フレームごとに一回呼ぶことにより、
	実時間から算出したfpsを取得する。
	fps は 100 倍した整数形式で返す。
	-------------------------------------------------------
	用途：fps計測用。
	注意：60フレームごとに必ず呼ぶ事。
	注意：複数ヵ所で呼ばれる事は考慮されてないので、必ず一ヶ所から呼ぶ事。
	注意：前回から約72分以内に必ず呼ぶ事。(60フレームで72分以上って...どんな状況やねん)
	そういや模倣風ってスリープ出来るんだね。
	スリープして約72分以上たった場合は、始めのfpsが正確に出ない(本当より良く出る)
	だけなので特に問題ないです。
---------------------------------------------------------*/

/*static*/static unsigned int psp_get_fps60_00(void)
{
	static u64 ticks_alt;	/* 前回の時間 the clock time of alter ticks. */
	static u64 ticks_now;	/* 今回の時間 the clock time of current ticks. */
	ticks_alt	= ticks_now;
	sceRtcGetCurrentTick(&ticks_now);
	/*	前回計測時間との差分を 1[nano sec] 単位  ( 1[nano sec] == 1/(1000*1000)[sec] )
	の unsigned int == 符号なし32bit形式へ変換 */
	unsigned int ttt;
	ttt= ((unsigned int)(ticks_now - ticks_alt));/* 差分時間 the clock time of differencial ticks. */
	/* fps を 100 倍した整数形式へ変換 */
	if (0 != ttt)/* ゼロ 0 で割る場合を回避する(Devision by zero 防止) */
//	if (0x0f < ttt)/* ゼロ 0 で割る場合を回避する(Devision by zero 防止) */
	{
	//	ttt = ( (unsigned int)(60*60*16666) / (ttt));					/*"60fps"*/
	//	ttt = ( (unsigned int)(60*60*166666) / (unsigned int)(ttt));	/*"60.0fps"*/
	//	ttt = ( (u64)(60*60*1666666) / (u64)(ttt)); 					/*"60.00fps"*/
	/*"60.00fps"(整数型で計算するには 33bit 要るので、unsigned int (32bit) で足りない)*/
		ttt = ( (unsigned int)((((60*60)/16)*(1666666))) / (unsigned int)(ttt>>4));
		/* 60*60 == 3600 は、 16 で割り切れるので、225 になる。 */
	}
	return (ttt);
}


static u32 top_score;
global void score_display(void)
{
	#if 1/*テストoff*/
	/* [ パネルベースを表示 ] */
	{
		SDL_Rect panel_base_r = {GAME_WIDTH, 0, 0, 0};	// データウィンドウ用rect_srct->w,h,x,y
		PSPL_UpperBlit(panel_base, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], &panel_base_r);
	}
	#endif
//
	/* [ プレイヤー数表示 ] */
	draw_stars_status( R_00_aka_hosi_png,  (cg.zanki), 10*8+4); /*R_01_mizu_hosi_png*/
//
	/* [ ボム数表示 ] */
	draw_stars_status( R_01_mizu_hosi_png, (cg.bombs), 14*8+1); /*R_00_aka_hosi_png*/

	//{/*←何故かスコープしない方が良い(もちろんスコープあるなしで,コードが変わる)*/

	/* 通常時(デバッグ以外は死ぬ(喰らいボムモードへ)) */
		/* [ ハイスコア表示 ] */
		if (top_score < cg.game_score)
		{	top_score = cg.game_score;}
	//	sp rintf(my_font_text,"%09d0", top_score);
		strcpy(my_font_text, "0000000000");
		dec_print_format(top_score, 	9/*8*/, (char *)my_font_text);
		//
		cg.SDL_font_type 	= FONT10W;
		cg.SDL_font_x 		= (PPP+5*8+4);
		cg.SDL_font_y 		= (3*8+2);
		font88_print_screen_xy();
	//
		/* [ スコア表示 ] */
	//	sp rintf(my_font_text,"%09d0", pd_game_score);
		strcpy(my_font_text, "0000000000");
		dec_print_format(cg.game_score, 9/*8*/, (char *)my_font_text);
		//
	//	cg.SDL_font_type 	= FONT10W;
	//	cg.SDL_font_x 		= (PPP+5*8+4);
		cg.SDL_font_y 		= (6*8+7);
		font88_print_screen_xy();
	//


		/* [ パワーゲージ表示 ] */
		draw_power_gauge(cg.weapon_power); /*,PPP+7,124*/
		if (cg.weapon_power > (MAX_POWER_IS_128-1) /*== 128*/)/*max==MAX_POWER_IS_128==「129段階」*/
		{
			strcpy(my_font_text, STR_MAX);
		}
		else
		{
			#if 0/* 100% 表記 */
		//	sp rintf(my_font_text, "%d", (int)((dou ble)p->weapon / 128 * 100 ));
		//	sp rintf(my_font_text," %d", (((int)(pd_weapon_power) * 200) >>8) );
			strcpy(my_font_text,"  0");
			dec_print_format( (((int)(cg.weapon_power) * 200) >>8), 2, (char *)&my_font_text[1]);
			#endif
			#if 1/* [P]数 表記(r33) */
			strcpy(my_font_text,"  0");
			dec_print_format( (((int)(cg.weapon_power) ) ), 3, (char *)&my_font_text[0]);
			#endif
			#if 0/* 5.00 表記(r32) */
		//	sp rintf(my_font_text, "%d", (int)((dou ble)p->weapon / 128 * 100 ));
		//	sp rintf(my_font_text," %d", (((int)(pd_weapon_power) * 200) >>8) );
			/* "5.00" */
			strcpy(my_font_text,"0000");
			dec_print_format( (((int)(cg.weapon_power) * (200*5)) >>8), 3, (char *)&my_font_text[0]);
		//	my_font_text[4] = 0;
			my_font_text[3] = my_font_text[2];
			my_font_text[2] = my_font_text[1];
			my_font_text[1] = CHR_PIRIOD_;
			#endif
		}
		//
	//	cg.SDL_font_type = FONT10W;
		cg.SDL_font_x = (PPP+10*8+7);//PPP+8*8+3
		cg.SDL_font_y = (17*8+5);//125/*+1*/-2
		font88_print_screen_xy();
	//
		#if 0/* 地味に処理落ち要因になっていたので廃止。(r33) */
		/* [ ボム有効時間表示 ] */
		//if (p->ex tra_type!=PLX_NONE)
		if (0 != cg.bomber_time)
		{
		//	sp rintf(my_font_text, STR_TIME_"%3d",(int)(((int)pd_bomber_time)/10));
		//	cg.SDL_font_type	= FONT10W;
		//	cg.SDL_font_x		= (PPP+3*8-6);
		//	cg.SDL_font_y		= (160);
		//	font88_print_screen_xy();
			strcpy(my_font_text, STR_TIME_"   ");
			dec_print_format( (int)(((int)cg.bomber_time) ), 3, (char *)&my_font_text[5]);
			my_font_text[7] = (0);	/*' '*/ 	/* 1桁目は表示しない */
		//
		//	cg.SDL_font_type	= FONT10W;
			cg.SDL_font_x		= (PPP+8*8+4);
			cg.SDL_font_y		= (22*8);
			font88_print_screen_xy();
		}
		#endif
	//
		/* --- 妖のグレイズカンスト 99999回 (5桁) --- */
		/* [ グレイズスコア表示 ] */
		//	sp rintf(my_font_text," %d", pd_graze_point);
			strcpy(my_font_text,"   0");
			dec_print_format( cg.graze_point, 4, (char *)&my_font_text[0]);
		//
		//	cg.SDL_font_type	= FONT10W;
			cg.SDL_font_x		= (PPP+11*8+4);//(PPP+7*8+3)
			cg.SDL_font_y		= (20*8);//(140)
			font88_print_screen_xy();/*4桁(稼げる)*/ /*3桁(足りない)*/
	//
		/* [ 難易度表示 ] */
		{
			static const char *rank_name[(4)] =
			{
			/* 0==PLX_NONE*/		STR_EASY,
			/* 1==PLX_HOMING:*/ 	STR_NORMAL,
			/* 2==PLX_HLASER:*/ 	STR_HARD,
			/* 3==PLX_SHIELD:*/ 	STR_LUNATIC,
		//	/* 4==PLX_BOMB:*/		STR_EXTRA,
			//	/*default:*/		"UNKNOWN ???",
			};
		//
			strcpy(my_font_text, (char *)rank_name[((cg.game_difficulty))&(4-1)]);
		//	cg.SDL_font_type	= FONT10W;
			cg.SDL_font_x		= (PPP+/*7*/1*8);
			cg.SDL_font_y		= (256);
			font88_print_screen_xy();
		}
	//
		#if 1
		/* [ fps 表示(60フレーム単位で表示) ] */
		/* (pspは1秒が59.9なんたらフレームで、厳密に60でないらしいです。 ) */
		{
		//	static int ttt;
			static unsigned int ttt;
			static int fps_draw_wait_counter;
			fps_draw_wait_counter--;
			if (fps_draw_wait_counter < 1)
			{
				fps_draw_wait_counter = 60;
				/* エクステンドチェックは1秒に1回(笑)で問題ないと思う。 */
				#if (1==USE_EXTEND_CHECK)
				player_check_extend_score();
				#endif
			//	[1/60sec]	0.016 666 [nsec] 6666666666666666666666666667
			//	16666.66666 / x == 60.0,  16666.666666/60.0 == x, x== 277.7777777777777
			//	16666.00(int) / 60.00(int) == 60.1660649819494584837545126353791 = 60.000 (int)
				ttt = psp_get_fps60_00();/* 前回計測時間との差分を 1[nano sec] 単位の signed int == 符号付32bit形で返す。 */
			}
//
			/*"60fps"*/
	//	//	sp rintf(my_font_text, STR_TIME_"%3d",(int)(((int)ttt)));
	//		strcpy(my_font_text, STR_TIME_"00");
	//	//	dec_print_format(ttt,	3, (char *)&my_font_text[0/*5*/]);
	//		dec_print_format(ttt,	2, (char *)&my_font_text[0/*5*/]);
	//		font88_print_screen_xy(my_font_text, FONT10W, PPP,256);
//
	//		/*"60.0fps"*/
	//		strcpy(my_font_text, STR_TIME_"0000");
	//		dec_print_format(ttt,	3, (char *)&my_font_text[0/*5*/]);
	//		my_font_text[8-5] = my_font_text[7-5];
	//		my_font_text[7-5] = CHR_PIRIOD_;/*' ' '.'*/
	//		font88_print_screen_xy(my_font_text, FONT10W, PPP,256);
//
			/*"60.00fps"*/
			strcpy(my_font_text, /*STR_TIME_*/"00000"STR_FPS_);
			dec_print_format(ttt,	4, (char *)&my_font_text[0/*5*/]);
			my_font_text[9-5] = my_font_text[8-5];
			my_font_text[8-5] = my_font_text[7-5];
			my_font_text[7-5] = CHR_PIRIOD_;/*' ' '.'*/
		//
		//	cg.SDL_font_type	= FONT10W;
			cg.SDL_font_x		= (PPP+8*8+2);
			cg.SDL_font_y		= (256);
			font88_print_screen_xy();
		}
		#endif
	//}
}

/*---------------------------------------------------------
	パネル表示、初期化
---------------------------------------------------------*/

global void score_panel_init(void)
{
	top_score			= high_score_table[(cg_game_select_player)][0].score;	// 常に表示するハイコアの取得=>score.cで利用
	strcpy(my_file_common_name, (char*)DIRECTRY_NAME_DATA_STR"/fonts/" "panel_base.png");	panel_base	= load_chache_bmp();//, 0, 1);
	strcpy(my_file_common_name, (char*)DIRECTRY_NAME_DATA_STR"/fonts/" "hosi_gauge.png");	star_gauge	= load_chache_bmp();//, 0, 1);	/*(char *)img_name[img_num]*/
	SDL_SetColorKey(star_gauge, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);/* 現状 SDL合成のため必要 */
}

