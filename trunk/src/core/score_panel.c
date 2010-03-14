
/*---------------------------------------------------------
	スコアパネル(表示)関連
---------------------------------------------------------*/

#include "game_main.h"
#include "bullet_object.h"

static SDL_Surface *panel_base; 	// パネルベース
static SDL_Surface *star_gauge;

/* サイドパネルの横表示位置 pannel x offset */
//#define PPP (380)
#define PPP (GAME_WIDTH)
#if 0
	#define STR_MAX 		"MAX"
	#define STR_MISSILE 	"MISSILE"
	#define STR_WEAPON_UP	"WEAPON_UP"
	#define STR_LASER		"LASER"
	#define STR_BOMB		"BOMB"
	#define STR_TIME_		"TIME :"
	#define STR_EXTRA_		"EXTRA  :"
#else
	//#define STR_MAX		"ABCD"			/*4つ*/
	//#define STR_MISSILE	"EFGHIJ"		/*6つ*/
	//#define STR_WEAPON_UP "KLMNOPQR"		/*8つ*/
	//#define STR_LASER 	"STUVW" 		/*5つ*/
	//#define STR_BOMB		"XYZab" 		/*5つ*/
	//#define STR_TIME_ 	"cdefg" 		/*5つ*/
	//#define STR_EXTRA_	"hijkl" 		/*5つ*/
//
	#define STR_MAX 		"ABCD"			/*4つ*/
	#define STR_EASY		"EFGH" "  " 	/*4つ*/
	#define STR_NORMAL		"IJKLM" " " 	/*5つ*/
	#define STR_HARD		"NOPQ" "  " 	/*4つ*/
	#define STR_LUNATIC 	"RSTUVW"		/*6つ*/
	#define STR_EXTRA		"XYZab" 		/*5つ*/
	#define STR_TIME_		"cdef"			/*4つ*/
//	#define STR_RANK_		"hijkl" 		/*5つ*/
	#define STR_FPS_		"hij"			/*3つ*/
	#define CHR_PIRIOD_ 	'g' 			/*1つ*/
	#define STR_ENEMY		"klmno" 		/*5つ*/
#endif


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
 * @return 無し
 *
---------------------------------------------------------*/
static void dec_display(
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
	//		obj_01_panel[sprite+size].m_RollZ		= 0/*.0*/;
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
	SDL_BlitSurface(star_gauge, &rect_src, sdl_screen[SDL_00_VIEW_SCREEN], &rect_dest);
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
//	rect_src.w = (int)((dou ble)weapon / 127/*128*/ * (dou ble)power_gauge->w); 	// [***090123		変更
	#else
	/* 1 dot ぐらい誤差あるかもしれないけど簡略化(高速化) */
	rect_src.w = ((int)(weapon * (80)/*power_gauge->w*/)>>7); // [***090123		変更
	#endif
	rect_dest.w = (80);/*power_gauge->w*/
	rect_dest.h = (13);/*power_gauge->h*/
	rect_dest.x = WP_GAUGE_X_OFS;/*dx*/
	rect_dest.y = WP_GAUGE_Y_OFS;/*dy*/
	SDL_BlitSurface(star_gauge/*power_gauge*/, &rect_src, sdl_screen[SDL_00_VIEW_SCREEN], &rect_dest);
}

/*---------------------------------------------------------
	ボスの HP 表示の子関数
---------------------------------------------------------*/

extern int boss_x256;
extern int boss_y256;

extern u8 es_panel[4];

static int draw_boss_hp_value_set;
static void draw_boss_gauge(void/*int dx, int dy*/) 	// [***090305		変更
{
/*???*/
//	return;
//
	SPRITE *s = pd_boss;
	if (NULL == s)
	{
		return;/* ボスが無い場合は何もしない */
	}
//
	boss_life_value = (/*((BO SS_BASE *)s->data)->boss_*/s->base_hp);/*(???)141477*/
//	int bo ss_life_value=(((BO SS_BASE *)s->data)->bo ss_life);/*(???)141477*/

	#if 0
	if (0 > boss_life_value)	return;/* 負数の場合は何もしない */
	if (9*(8*1024) < boss_life_value) return;/* 範囲外の場合は何もしない */
	#else
//	if (0 != ((boss_life_value)&(0xffffc000)) ) return;/* 範囲外の場合は何もしない */
	if (0 > boss_life_value)
	{
	//	(((BO SS_BASE *)s->data)->boss_health) = 0;/* まずいかも */
		boss_life_value = 0;
	}
	if (0==(boss_life_value/*+bo ss_life_value*/))
	{
		return;/* 範囲外の場合は何もしない */
	}
	boss_x256 = (/*((BO SS_BASE *)s->data)->boss_*/s->x256);/*(???)141477*/
	boss_y256 = (/*((BO SS_BASE *)s->data)->boss_*/s->y256);/*(???)141477*/

//	draw_boss_hp_value_set = ((boss_life_value & 0x03fc)>>2);/* ボスhp描画値 */
	draw_boss_hp_value_set = ((boss_life_value & 0x1fe0)>>(2+3));/* ボスhp描画値 */
	if (draw_boss_hp_value < (draw_boss_hp_value_set))
			{	draw_boss_hp_value++;	}
	else	{	draw_boss_hp_value--;	}
	#endif
//
	#if 1/*ボス時間経過*/
//	if ()
//	if ((STATE_FLAG_05_IS_BOSS|0) == (pd_state_flag&(STATE_FLAG_05_IS_BOSS|STATE_FLAG_06_IS_SCRIPT)))
	{
		/* (とりあえず)スペカモード時のみ時間経過 */
		if (0/*off*/!=spell_card_mode)/*on時のみ*/
		{
			/*((BO SS_BASE *)s->data)->*/spell_card_boss_timer--;/*fps_factor*/
			if (0 > (/*((BO SS_BASE *)s->data)->*/spell_card_boss_timer))	/*1*/
			{
				spell_card_boss_timer		= 0;
				spell_card_mode 			= 0/*off*/;
				/*((BO SS_BASE *)s->data)->boss_*/s->base_hp 	= spell_card_limit_health;		/* (とりあえず) */
				boss_destroy_check_type(s/*敵自体*/, DESTROY_CHECK_01_IS_TIME_OUT);/*	★ 攻撃の場合の死亡判定 	★ 時間切れの場合の死亡判定 */
				#if 000
				/*((BO SS_BASE *)s->data)->*/spell_card_boss_timer	= (60*64);		/* (とりあえず) */
				/*((BO SS_BASE *)s->data)->boss_*/s->base_hp 		= (0);			/* (とりあえず) */
				#endif
			}
		}
	}
	unsigned char boss_timer_low	= ((/*((BO SS_BASE *)s->data)->*/spell_card_boss_timer)&0x3f);/* */
	unsigned int boss_timer_value	= ((/*((BO SS_BASE *)s->data)->*/spell_card_boss_timer)>>6);/* */
	#endif
	//	99 以上は 99 表示
	if (99<boss_timer_value)
	{
		boss_timer_value = 99;
	}

/*
	カウンタについて：
	カウンタは 9 から点滅＆音を鳴らすという風に
	心理的に受けとれるようにしてある。
	これを本当に 9 から処理すると人間の感覚に合わない。
//
	音は 10 (の終わり)から鳴らす。これは 10 の始めは 9 の終わりなので、
	9 から鳴っているとみなせる。(理屈上ズレはない)
//
	点滅は 10 から点滅させる。これは 10 から本当に点滅している。
	注意深く見れば 10 が消えている。(音が鳴る前に 10 が消えている)
	しかし人間の感覚からすると「 9 から点滅しているように感じる」
	人間の視覚は予告が無いと区切りに反応しない。
//
	11 11 11 10 10 10 09 09 09 08 08 08 (時間の流れ)
	無 無 無 無 無 無 音 無 無 音 無 無 (音は 10 (の終わり)から鳴らす。理屈上ズレはない)
	点 点 点 点 点 消 点 点 消 点 点 消 (点滅は 10 から点滅させる。人間の感覚からすると「 9 から点滅しているように感じる」)
				   ↑
*/

	if (0==boss_timer_low)
	{
		/* カウント 9 から音を鳴らす． */
	//	if ((10  )>boss_timer_value)	/* (10	)==設定値 10 で、カウント 8 から音が鳴るように聞こえる． */
		if ((10+1)>boss_timer_value)	/* (10+1)==設定値 11 で、カウント 9 から音が鳴るように聞こえる． */
		{
			voice_play(VOICE15_COUNT_TIMER, TRACK03_SHORT_MUSIC);/*テキトー*/
		}
	}


	/* boss hp 画像の文字幅分 / gauge stringth x width */
//	#define HPGAUGE_X_OFS (50)
//	#define HPGAUGE_Y_OFS (6)
	#define HP_FONT_X_OFS (4)	/*(50-(8*5)-3)*/	/*(37+10) dx*/
	#define HP_FONT_Y_OFS (1)/*(0+0) dy*/
	//
	#define BOSS_TIMER_X_OFS (PPP-16-4)/* dx*/
	#define BOSS_TIMER_Y_OFS (1)/*(0+0) dy*/
	/* 1024以上の値は数字で表示 */
	{	char buffer[8/* 4 3*/];
	#if 1
		//	スペル残り時間表示
		if (
			//	(9<boss_timer_value) || 	/* 10以上は無条件で表示 */		/* カウント 8 から点滅してるように見える． */
				(10<boss_timer_value) ||	/* 11以上は無条件で表示 */		/* カウント 9 から点滅してるように見える． */
				(20<boss_timer_low) 		/* 点滅 20=(64/3) */
			)
		{
			strcpy(buffer,"00");
			dec_display( (boss_timer_value), 2, (char *)buffer);
//			font_print_screen_xy(buffer, FONT01/*FONT06*/, BOSS_TIMER_X_OFS, BOSS_TIMER_Y_OFS);
			es_panel[2] = (buffer[0]&0x0f);
			es_panel[3] = (buffer[1]&0x0f);
		}
	#endif
	//	残りライフ表示
	//	sp rintf(buffer,"%d", (boss_life_value>>10));/*(???)141477*/
		strcpy(buffer, STR_ENEMY "0");
		dec_display( /*(bo ss_life_value)*/(boss_life_value>>(10+3))/*(boss_life_value>>10)*/, 1, (char *)&buffer[5]);
//		font_print_screen_xy(buffer, FONT01/*FONT06*/, HP_FONT_X_OFS, HP_FONT_Y_OFS);
		es_panel[1] = (buffer[5]&0x0f);
		es_panel[0] = (10);/*"enemy"*/
	}
}

	/* 1024未満の細かい値はグラフ表示 */
//	{
	//	SDL_Rect rect_src;
	//	SDL_Rect rect_dest;
	//	rect_src.x = 0;
	//	rect_src.y = 0;
	//	rect_src.h = 10;
	//	rect_src.w = HPGAUGE_X_OFS+((boss_life_value	& 0x03ff)>>2); /* 1023値 → 255ドット */
	//	rect_dest.w = boss_gauge->w;
	//	rect_dest.h = boss_gauge->h;
	//	rect_dest.x = 10/*dx*/;
	//	rect_dest.y =  0/*dy*/+HPGAUGE_Y_OFS;
//		SDL_BlitSurface(boss_gauge, NULL/*&rect_src*/, sdl_screen[SDL_00_VIEW_SCREEN], NULL/*&rect_dest*/);
//	}

/*---------------------------------------------------------
	スコアパネルのステータス表示
---------------------------------------------------------*/
#define USE_DEBUG	(0)
#if (1==USE_DEBUG)
int debug_num1;
int debug_num2;
#endif

static int top_score;
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

void score_display(void)
{
	/* [ パネルベースを表示 ] */
	{
		SDL_Rect panel_base_r = {GAME_WIDTH, 0, 0, 0};	// データウィンドウ用rect_srct->w,h,x,y
		SDL_BlitSurface(panel_base, NULL, sdl_screen[SDL_00_VIEW_SCREEN], &panel_base_r);
	}
//
	/* [ ボスの体力表示 ] */
//	if (B01_BA TTLE == pd_bo ssmode)
//	if ((STATE_FLAG_05_IS_BOSS|0) == (pd_state_flag&(STATE_FLAG_05_IS_BOSS|STATE_FLAG_06_IS_SCRIPT)))
	if ((pd_state_flag&(STATE_FLAG_13_DRAW_BOSS_GAUGE)))
	{
		draw_boss_gauge(/*10, 6-6*/);
	}
//
	/* [ プレイヤー数表示 ] */
	draw_stars_status( R_00_aka_hosi_png,  (pd_zanki), 10*8+4);	/*R_01_mizu_hosi_png*/
//
	/* [ ボム数表示 ] */
	draw_stars_status( R_01_mizu_hosi_png, (pd_bombs), 14*8+1);	/*R_00_aka_hosi_png*/
//
	#if (1==USE_DEBUG)/* 各優先順位ごとどれくらいあるか調べてみる */
	/* パネルのスコア欄にdebug_num1を、グレイズ欄にdebug_num2を表示させる。っていうか書き換えちゃう。 */
	pd_score		= debug_num1;
	pd_graze_point	= debug_num2;
	#endif
//
	//{/*←何故かスコープしない方が良い(もちろんスコープあるなしで,コードが変わる)*/
		char buffer[64/*100*/];
		/* [ ハイスコア表示 ] */
		if (top_score < pd_my_score)
		{	top_score = pd_my_score;}
	//	sp rintf(buffer,"%09d0", top_score);
		strcpy(buffer,"0000000000");
		dec_display(top_score,		9/*8*/, (char *)buffer);		font_print_screen_xy(buffer,FONT01,PPP+5*8+4,3*8+2);
	//
		/* [ スコア表示 ] */
	//	sp rintf(buffer,"%09d0", pd_my_score);
		strcpy(buffer,"0000000000");
		dec_display(pd_my_score,	9/*8*/, (char *)buffer);		font_print_screen_xy(buffer,FONT01,PPP+5*8+4,6*8+7);
	//
		/* [ パワーゲージ表示 ] */
		draw_power_gauge(pd_weapon_power); /*,PPP+7,124*/
		if (pd_weapon_power > (MAX_POWER_IS_128-1) /*== 128*/)/*max==MAX_POWER_IS_128==「129段階」*/
		{
			strcpy(buffer, STR_MAX);
		}
		else
		{
			#if 0/* 100% 表記 */
		//	sp rintf(buffer, "%d", (int)((dou ble)p->weapon / 128 * 100 )); 	// [***090123		変更
		//	sp rintf(buffer," %d", (((int)(pd_weapon_power) * 200) >>8) ); 	// [***090214		変更
			strcpy(buffer,"  0");
			dec_display( (((int)(pd_weapon_power) * 200) >>8), 2, (char *)&buffer[1]);
			#endif
			#if 0/* [P]数 表記 */
			strcpy(buffer,"  0");
			dec_display( (((int)(pd_weapon_power) ) ), 3, (char *)&buffer[0]);
			#endif
			#if 1/* 5.00 表記 */
		//	sp rintf(buffer, "%d", (int)((dou ble)p->weapon / 128 * 100 )); 	// [***090123		変更
		//	sp rintf(buffer," %d", (((int)(pd_weapon_power) * 200) >>8) ); 	// [***090214		変更
			/* "5.00" */
			strcpy(buffer,"0000");
			dec_display( (((int)(pd_weapon_power) * (200*5)) >>8), 3, (char *)&buffer[0]);
		//	buffer[4] = 0;
			buffer[3] = buffer[2];
			buffer[2] = buffer[1];
			buffer[1] = CHR_PIRIOD_;
			#endif
		}
		font_print_screen_xy(buffer,FONT01,PPP+10*8+7,17*8+5);
		//font_print_screen_xy(buffer,FONT01,PPP+8*8+3,125/*+1*/-2);
	//
		/* [ ボム有効時間表示 ] */
		//if (p->ex tra_type!=PLX_NONE)
		if (0 != pd_bomber_time)
		{
		//	sp rintf(buffer, STR_TIME_"%3d",(int)(((int)pd_bomber_time)/10));
		//	font_print_screen_xy(buffer,FONT01,PPP+3*8-6,160);
			strcpy(buffer, STR_TIME_"   ");
			dec_display( (int)(((int)pd_bomber_time) ), 3, (char *)&buffer[5]);
			buffer[7] = 0/*' '*/;/* 1桁目は表示しない */
			font_print_screen_xy(buffer,FONT01,PPP+8*8+4,22*8);
		}
	//
		/* --- 妖のグレイズカンスト 99999回 (5桁) --- */
		/* [ グレイズスコア表示 ] */
		//	sp rintf(buffer," %d", pd_graze_point);
			strcpy(buffer,"   0");
			dec_display( pd_graze_point, 4, (char *)&buffer[0]);
		//	font_print_screen_xy(buffer,FONT01,PPP+7*8+3,140);/*3桁(足りない)*/
			font_print_screen_xy(buffer,FONT01,PPP+11*8+4,20*8);/*4桁(稼げる)*/
	//
		/* [ 難易度表示 ] */
		{
			/*const*/ char *rank_name[4] =
			{
			/* 0==PLX_NONE*/		STR_EASY,
			/* 1==PLX_HOMING:*/ 	STR_NORMAL,
			/* 2==PLX_HLASER:*/ 	STR_HARD,
			/* 3==PLX_SHIELD:*/ 	STR_LUNATIC,
		//	/* 4==PLX_BOMB:*/		STR_EXTRA,
			//	/*default:*/		"UNKNOWN ???",
			};
			font_print_screen_xy( (char *)rank_name[(difficulty)&(4-1)], FONT01,PPP+/*7*/1*8,256);
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
	//	//	sp rintf(buffer, STR_TIME_"%3d",(int)(((int)ttt)));
	//		strcpy(buffer, STR_TIME_"00");
	//	//	dec_display(ttt,	3, (char *)&buffer[0/*5*/]);
	//		dec_display(ttt,	2, (char *)&buffer[0/*5*/]);
	//		font_print_screen_xy(buffer,FONT01,PPP,256);
//
	//		/*"60.0fps"*/
	//		strcpy(buffer, STR_TIME_"0000");
	//		dec_display(ttt,	3, (char *)&buffer[0/*5*/]);
	//		buffer[8-5] = buffer[7-5];
	//		buffer[7-5] = CHR_PIRIOD_;/*' ' '.'*/
	//		font_print_screen_xy(buffer,FONT01,PPP,256);
//
			/*"60.00fps"*/
			strcpy(buffer, /*STR_TIME_*/"00000"STR_FPS_);
			dec_display(ttt,	4, (char *)&buffer[0/*5*/]);
			buffer[9-5] = buffer[8-5];
			buffer[8-5] = buffer[7-5];
			buffer[7-5] = CHR_PIRIOD_;/*' ' '.'*/
			font_print_screen_xy(buffer,FONT01,PPP+8*8+2,256);
		}
		#endif
	//}
}

/*---------------------------------------------------------
	パネル表示、初期化
---------------------------------------------------------*/
#include "name_entry.h"/**/
extern int select_player;
void score_panel_init(void)
{
	top_score			= high_score_table[select_player][0].score; 	// 常に表示するハイコアの取得=>score.cで利用
	panel_base			= loadbmp0("panel/panel_base.png", 0, 1);
	star_gauge			= loadbmp0("panel/hosi_gauge.png", 0, 1);		/*(char *)img_name[img_num]*/
	SDL_SetColorKey(star_gauge,(SDL_SRCCOLORKEY|SDL_RLEACCEL),0x00000000);/* 現状 SDL合成のため必要 */
}

/*---------------------------------------------------------
	コンテニュー回数の表示
---------------------------------------------------------*/
extern int	now_max_continue;
void render_continue(void)/*int now_max_continue*/
{
	char buffer[64/*100*/];
//	/* あとn回コンティニューできます */
//	sp rintf(buffer,  "TRY CHANCE STILL AT %2d", now_max_continue);
	strcpy(buffer,	"TRY CHANCE STILL AT  0");
	dec_display( now_max_continue, 2, (char *)&buffer[20]);
//
	font_print_screen_xy(buffer, FONT03, 0/*10*/, 16/*50*/);
	#if (0==USE_CONTINUED_RANKING)
//	/* コンティニューした場合、スコアランキングされません */
	font_print_screen_xy("IF YOU GOT CONTINUED,", FONT05, 0/*10*/, 200);
	font_print_screen_xy("CAN NOT HAVE RANKING.", FONT05, 16/*26*/, 220);
//
	/* あんまり長ったらしい英語にしてもしょーがないから、(長いと表示できないし)
		意味が違わない範囲で適当に省略 */
	#endif
}




/*---------------------------------------------------------
	以下
	-------------------------------------------------------
	RESULT表示(stage_clear.c)
	-------------------------------------------------------
	ゲームオーバーの表示(game_over.c)
	-------------------------------------------------------
	現在都合により score_panel.c 内にある。
	Gu化が進めば独立する予定。
---------------------------------------------------------*/

#include "game_main.h"
#include "player.h"
#include "scenario_script.h"

/*---------------------------------------------------------
	ゲームコア終了の後処理
---------------------------------------------------------*/
extern int draw_script_screen;					/* せりふウィンドウ表示フラグ */

extern void bg2_destroy(void);
extern int last_score;
//extern int last_stage;
void gamecore_term(void)
{
	draw_script_screen = 0; /* せりふウィンドウ表示フラグ off */
//
	last_score = pd_my_score;

	bg2_destroy();		// [***090126		追加
	//sprite_controller_remove_all();
	/*
		この辺でbossとかcoreとか開放しなくていいんだっけ？
	*/
	sprite_remove_all_SDL(SP_GROUP_ALL_GAME_OBJS);/*gu汎用*/
	sprite_remove_all_444(SP_GROUP_ALL_GAME_OBJS);/*弾幕専用*/
//	sprite_remove_all222(SP_GROUP_ALL_GAME_OBJS);/*弾幕用*/
//	score_cleanup();
	//stop_music(); 	// [***090123		コメントアウト
	set_music_volume(128);
	play_music_num(BGM_21_menu01);
//	last_stage = 0;
//削除	名前入力で使う。	player_now_stage = 0;		// [***090702		追加
	pd_bomber_time = 0; 		// [***090903		追加
	#if 1
	draw_boss_hp_value	= 0;/* よくわかんない */
	#endif
}


/*---------------------------------------------------------
	RESULT表示
---------------------------------------------------------*/

static void render_stage_clear_result(void/*int now_max_continue*/)
{
	/* 幽々子 特殊能力：ステージクリア時にボムが増える */
	if (YUYUKO==select_player)	/* 幽々子の場合 */
	{
		#if 1/*原作風*/
		if (3 > pd_bombs)	/* クリアー時にボムが３つ未満なら */
		{	pd_bombs = 3;	}	/* ３つに増やす */
		#endif
		#if 1/*模倣風*/
		if (8 > pd_bombs)	/* クリアー時にボムが９(8)つ未満なら */
		{	pd_bombs++;	}	/* １つ増やす */
		#endif
		/* ボムがなくてもクリアーすればボムが４つになる */
	}
//
	#if 0
	/* なんか知らんが、Lunaticでちょっとチェックしたら、オーバーフローするｗ。 */
	/* クリアボーナス チェック */
	player_add_score(adjust_score_by_difficulty((
//		(player_now_stage * score( 1000)) + /* ステージ x  1000 pts */	/*原作風*/
//		(pd_weapon_power * score(	100)) + /* パワー	x	100 pts */	/*原作風*/
//		(pd_graze_point)					/* グレイズ x	 10 pts */	/*原作風*/
//
		(player_now_stage * score(10000)) + /* ステージ x 10000 pts */	/*模倣風*/
		(pd_graze_point  * score( 1000)) + /* グレイズ x  1000 pts */	/*模倣風*/
		(pd_weapon_power * score(	100))	/* パワー	x	100 pts */	/*模倣風*/
	)));
	#else
	/* なんか知らんが、オーバーフローするので、個別に足してみる。 */
	/* クリアボーナス チェック */
	player_add_score(adjust_score_by_difficulty((	(((u32)player_now_stage) * score(10000))	)));	/* ステージ x 10000 pts */	/*模倣風*/
	player_add_score(adjust_score_by_difficulty((	(((u32)pd_graze_point)  * score( 1000))	)));	/* グレイズ x  1000 pts */	/*模倣風*/
	player_add_score(adjust_score_by_difficulty((	(((u32)pd_weapon_power) * score(  100))	)));	/* パワー	x	100 pts */	/*模倣風*/
	#endif
//
	char buffer[32/*100*/];
	strcpy(buffer,	"RESULT" ); 																			font_print_screen_xy(buffer, FONT03, 0,  32);
//	strcpy(buffer,	"STAGE   0 X 1000 PTS.");	dec_display( player_now_stage, 1, (char *)&buffer[ 8]); 	font_print_screen_xy(buffer, FONT05, 8,  60);	/*原作風*/
//	strcpy(buffer,	"POWER 000 X  100 PTS.");	dec_display( pd_weapon_power,  3, (char *)&buffer[ 6]); 	font_print_screen_xy(buffer, FONT05, 8,  80);	/*原作風*/
//	strcpy(buffer,	"GRAZE 000 X   10 PTS.");	dec_display( pd_graze_point,   3, (char *)&buffer[ 6]); 	font_print_screen_xy(buffer, FONT05, 8, 100);	/*原作風*/
	//				"012345678901234567890
	strcpy(buffer,	"STAGE   0 X 10000PTS.");	dec_display( player_now_stage, 1, (char *)&buffer[ 8]); 	font_print_screen_xy(buffer, FONT05, 8,  60);	/*模倣風*/
	strcpy(buffer,	"GRAZE 000 X  1000PTS.");	dec_display( pd_graze_point,   3, (char *)&buffer[ 6]); 	font_print_screen_xy(buffer, FONT05, 8,  80);	/*模倣風*/
	strcpy(buffer,	"POWER 000 X   100PTS.");	dec_display( pd_weapon_power,  3, (char *)&buffer[ 6]); 	font_print_screen_xy(buffer, FONT05, 8, 100);	/*模倣風*/
	pd_graze_point = 0;/* 清算して消える */
	const char *level_name[4] =
	{
	//				"EASY      X 0.5",	/*原作風*/
	//				"NORMAL    X 1.0",	/*原作風*/
	//				"HARD      X 1.2",	/*原作風*/
	//				"LUNATIC   X 1.5",	/*原作風*/
					"EASY      X 0.5",	/*模倣風*/
					"NORMAL    X 1.0",	/*模倣風*/
					"HARD      X 2.0",	/*模倣風*/
					"LUNATIC   X 5.0",	/*模倣風*/
	};
	font_print_screen_xy( (char *)level_name[(difficulty/*&0x03*/)], FONT03, 0/*26*/, 160);
//
	#if 0/* ボス倒した場合の処理にいれた */
	pd_bomber_time = 0;/*都合上*/
	set_bg_alpha(255);/* 画面を明るくする */
	#endif
//	/* ステージクリア チェック */
	/* PRACTICE 開放 チェック */
	if ( (option_config[OPTION_CONFIG_07_OPEN] & (0x07)) < (player_now_stage&0x07) )
	{
		/* PRACTICE 開放 (進んだステージを練習可能にする) */
		option_config[OPTION_CONFIG_07_OPEN] &= (~0x07);
		option_config[OPTION_CONFIG_07_OPEN] |= (player_now_stage&0x07);
	}
}


/*---------------------------------------------------------
	GAME_OVER表示
---------------------------------------------------------*/

static void render_game_over_result(void/*int now_max_continue*/)
{
	pd_use_continue--;	/* (現プログラムの都合上)コンティニュー回数0で 1回カウントされるので、その分減らして辻褄あわせをする。 */
//
	char buffer[32/*100*/];
//	strcpy(buffer,	"GAME OVER" );																			font_print_screen_xy(buffer, FONT03, 0,  32);
	strcpy(buffer,	"PLAYER DATA" );																		font_print_screen_xy(buffer, FONT03, 0,  32);
	strcpy(buffer,	"SCORE     0000000000.");	dec_display( pd_my_score,		9, (char *)&buffer[10]);	font_print_screen_xy(buffer, FONT05, 8,  60);
	strcpy(buffer,	"TOTAL MISTAKE      0.");	dec_display( pd_count_miss, 	3, (char *)&buffer[17]);	font_print_screen_xy(buffer, FONT05, 8,  80);	/* ミス回数 */
	strcpy(buffer,	"USE BOMBS          0.");	dec_display( pd_used_bomber,	3, (char *)&buffer[17]);	font_print_screen_xy(buffer, FONT05, 8, 100);	/* ボム使用回数 */
	strcpy(buffer,	"BOMB WITH REVIVAL  0.");	dec_display( pd_use_kurai_bomb, 3, (char *)&buffer[17]);	font_print_screen_xy(buffer, FONT05, 8, 120);	/* 喰らいボム成功回数 */
	strcpy(buffer,	"USE CONTINUEc       0.");	dec_display( pd_use_continue,	3, (char *)&buffer[17]);	font_print_screen_xy(buffer, FONT05, 8, 140);	/* コンティニュー回数 */
//					"01234567890123456789"
	const char *level_name[4] =
	{
					"EASY   ",
					"NORMAL ",
					"HARD   ",
					"LUNATIC",
	};
	font_print_screen_xy( (char *)level_name[(difficulty/*&0x03*/)], FONT03, 0/*26*/, 160);
}
/* counter bomb defensive revival. */


/*---------------------------------------------------------
	うーん、Gu化中なので、ちゃんと機能しない。
	(SDL画面のみに対するエフェクト)
---------------------------------------------------------*/
static void effect_dark_screen(void)
{
#if 0
	#if 1
	/* KETM互換なら ここで back buffer screen を clear screen すべき */
	psp_push_screen();
	#endif
	SDL_SetAlpha(sdl_screen[SDL_00_VIEW_SCREEN],SDL_SRCALPHA,128);
	psp_push_screen();
	SDL_SetAlpha(sdl_screen[SDL_00_VIEW_SCREEN],SDL_SRCALPHA,255);
#endif
}


/*---------------------------------------------------------
	RESULT表示(ゲーム各面クリアー時)
---------------------------------------------------------*/

extern void player_loop_quit(void);
void stage_clear_work(void)
{
	static int result_time_out;
//	if ( (ST_WORK_STAGE_CLEAR|STAGE_CLEAR_00_INIT) == (psp_loop) )
	if ( (0) == (psp_loop&0xff) )		/* 初期化 */
	{
		script_message_window_clear();
		render_stage_clear_result();
		effect_dark_screen();
		result_time_out = (60*5);
		psp_loop++;
	}
	else	/* 動作 */
	{
		if (my_pad & (PSP_KEY_LEFT|PSP_KEY_RIGHT|PSP_KEY_UP|PSP_KEY_DOWN|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL|PSP_KEY_SLOW|PSP_KEY_OPTION))
		{
			result_time_out = (0);
		}
		result_time_out--;
		if (0 > result_time_out)
		{
			msg_time = (60*5);	/* 約 5 秒 */
			print_kanji000(/*SDL_Rect *rect_srct*/ /*0,*/ /*text*/
				"CHALLENGE NEXT STAGE!" /*改行*/"\\n"
				"　　　　　　　　　　　　　少女祈祷中...", /*int color_type*/7, /*int wait*/0);
		//
			if (/*extra_stage*/(8)==player_now_stage)/* エキストラモードの場合、終了する */
			{
			//	#if 1/* この２つのセットで自動的に終了(GAME OVER)する */
			//	now_max_continue = 1;	/* コンティニューさせない */
			//	player_loop_quit();
			//	#endif
				#if (0)
				psp_clear_screen();
				psp_push_screen();
				#endif
				player_now_stage--;/* 7までしか無いので */
				psp_loop = (ST_WORK_GAME_OVER|0);
//				if (0x7f==can_player_bit)
//				{
//					can_player_bit = 0xff;	/* チルノ Q 開放 */
//				}
			}
			else
			{
				psp_loop = (ST_INIT_GAME_PLAY_common|0);
			}
		//	common_load_init();
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

void gameover_work(void)
{
	static int game_over_time_out;/*wait*/
//
//	if ( (ST_WORK_GAME_OVER|GAME_OVER_00_INIT) == (psp_loop) )
	if ( (0) == (psp_loop&0xff) )		/* 初期化 */
	{
		//void gameover_init(void)
		render_game_over_result();/* gamecore_term();より前の必要がある。 */
		gamecore_term();
		effect_dark_screen();
		game_over_time_out = (60*60);	/* 約 1 分 */
		psp_loop++;
	}
	else	/* 動作 */
	{
	//	psp_pop_screen();
		if (my_pad & (PSP_KEY_LEFT|PSP_KEY_RIGHT|PSP_KEY_UP|PSP_KEY_DOWN|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL|PSP_KEY_SLOW|PSP_KEY_OPTION))
		{
			game_over_time_out = (0);
		}
		game_over_time_out--;
		if (0 > game_over_time_out)
		{
			if (
				#if (0==USE_CONTINUED_RANKING)
				( (/*3*/DEFAULT_MAX_CONTINUE-1) == now_max_continue ) &&
				#endif
				(last_score > high_score_table[select_player][4].score)
			)
			{
				psp_loop = (ST_WORK_NAME_ENTRY|0);
			}	/* 名前入力画面へ */
			else
			{
				player_now_stage=0;/*要る？*/
				psp_loop = (ST_INIT_MENU|0);
			}	/* タイトル画面へ */
		}
	}
}
