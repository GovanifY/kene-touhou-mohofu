
/*---------------------------------------------------------
	スコアパネル(表示)関連
---------------------------------------------------------*/

#include "support.h"
#include "enemy.h"

static SDL_Surface *panel_base; 	// パネルベース
static SDL_Surface *power_gauge;	// パワーゲージ。俺の環境だとloadbmp2で作ると普通に表示されるようになった。
static SDL_Surface *boss_gauge; 	// [***090305		追加:ボスHPゲージ

/* サイドパネルの横表示位置 pannel x offset */
#define PPP 380

/*---------------------------------------------------------
	プレイヤーの星の表示の子関数
---------------------------------------------------------*/

enum
{
	R_00_mizu_hosi_png = 0,
	R_01_aka_hosi_png,
};

static void draw_stars_status( int g_num, int num, int y_suf)
{
	SDL_Surface *star;
	/*const*/ char *img_name[2] =
	{
		"panel/mizu_hosi.png",
		"panel/aka_hosi.png",
	};
	SDL_Rect recAll,rec;
	star = loadbmp0((char *)img_name[g_num], 0, 1);
	SDL_SetColorKey(star,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	if (num<1)	{	num=0;}

	rec.x = 0;
	rec.y = 0;
	rec.w = (10*num);
	rec.h = 10;

	recAll.x = PSP_WIDTH480+3-7-(10*num);
	recAll.y = y_suf;
	recAll.w = 100;
	recAll.h = 11;
	SDL_BlitSurface(star, &rec, screen, &recAll);
}

/*---------------------------------------------------------
	プレイヤーのウェポンゲージの表示の子関数
---------------------------------------------------------*/

// ketmに差分を上書きすると何故か普通に表示される。
static void draw_power_gauge(int weapon/*, int dx, int dy*/)
{
	#define WP_GAUGE_X_OFS (PPP+7+9)
	#define WP_GAUGE_Y_OFS (124-2)
	SDL_Rect srec, drec;
	srec.x = 0;
	srec.y = 0;
	srec.h = 13;
	#if 0
//	srec.w = (int)((dou ble)weapon / 127/*128*/ * (dou ble)power_gauge->w); 	// [***090123		変更
	#else
	/* 1 dot ぐらい誤差あるかもしれないけど簡略化(高速化) */
	srec.w = ((int)(weapon * power_gauge->w)>>7);	// [***090123		変更
	#endif
	drec.w = power_gauge->w;
	drec.h = power_gauge->h;
	drec.x = WP_GAUGE_X_OFS/*dx*/;
	drec.y = WP_GAUGE_Y_OFS/*dy*/;
	SDL_BlitSurface(power_gauge, &srec, screen, &drec);
}

/*---------------------------------------------------------
	ボスの HP 表示の子関数
---------------------------------------------------------*/

static void draw_boss_gauge(/*int dx, int dy*/) 	// [***090305		変更
{
	SPRITE *s = ((PLAYER_DATA *)player->data)->boss;
	/* boss hp 画像の文字幅分 / gauge stringth x width */
	#define HPGAUGE_X_OFS (50)
	#define HPGAUGE_Y_OFS (6)
	#define HP_FONT_X_OFS (50)/*(37+10) dx*/
	#define HP_FONT_Y_OFS (6)/*(0+0) dy*/
	/* 1024以上の値は数字で表示 */
	{char buffer[4/*3*/];
		sprintf(buffer,"%d", ((ENEMY_DATA *)s->data)->b.health>>10);
		font_print_screen_xy(buffer, FONT01/*FONT06*/, HP_FONT_X_OFS, HP_FONT_Y_OFS);
	}
	/* 1024未満の細かい値はグラフ表示 */
	{SDL_Rect srec, drec;
		srec.x = 0;
		srec.y = 0;
		srec.h = 10;
		srec.w = HPGAUGE_X_OFS+((((ENEMY_DATA *)s->data)->b.health	& 0x03FF)>>2); /* 1023値 → 255ドット */
		drec.w = boss_gauge->w;
		drec.h = boss_gauge->h;
		drec.x = 10/*dx*/;
		drec.y =  0/*dy*/+HPGAUGE_Y_OFS;
		SDL_BlitSurface(boss_gauge, &srec, screen, &drec);
	}
}
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
	#define STR_EASY		"  EFGH"		/*4つ*/
	#define STR_NORMAL		" IJKLM"		/*5つ*/
	#define STR_HARD		"  NOPQ"		/*4つ*/
	#define STR_LUNATIC 	"RSTUVW"		/*6つ*/
	#define STR_EXTRA		"XYZab" 		/*5つ*/
	#define STR_TIME_		"cdefg" 		/*5つ*/
	#define STR_RANK_		"hijkl" 		/*5つ*/
#endif



/*---------------------------------------------------------
	スコアパネルのステータス表示
---------------------------------------------------------*/
#define USE_DEBUG	(0)
#if (1==USE_DEBUG)
int debug_num1;
int debug_num2;
#endif

static int top_score;
extern unsigned int psp_get_differencial_ticks(void);
void score_display(void)
{
	/* [ パネルベースを表示 ] */
	{
		SDL_Rect panel_base_r={GAME_WIDTH, 0, 0, 0};	// データウィンドウ用rect->w,h,x,y
		SDL_BlitSurface(panel_base,NULL,screen,&panel_base_r);
	}
//
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	/* [ ボスの体力表示 ] */
//	if (B01_BA TTLE == pd->bo ssmode)
//	if ((STATE_FLAG_05_IS_BOSS|0) == (pd->state_flag&(STATE_FLAG_05_IS_BOSS|STATE_FLAG_06_IS_SCRIPT)))
	if ((pd->state_flag&(STATE_FLAG_13_DRAW_BOSS_GAUGE)))
	{
		draw_boss_gauge(/*10, 6-6*/);
	}
//
	/* [ プレイヤー数表示 ] */
	draw_stars_status( R_00_mizu_hosi_png, (pd->zanki), 69+2);
//
	/* [ ボム数表示 ] */
	draw_stars_status( R_01_aka_hosi_png, (pd->bombs), 94+1);
//
	#if (1==USE_DEBUG)/* 各優先順位ごとどれくらいあるか調べてみる */
	/* パネルのスコア欄にdebug_num1を、グレイズ欄にdebug_num2を表示させる。っていうか書き換えちゃう。 */
	((PLAYER_DATA *)player->data)->score		= debug_num1;
	((PLAYER_DATA *)player->data)->graze_point	= debug_num2;
	#endif
//
	//{/*←何故かスコープしない方が良い(もちろんスコープあるなしで,コードが変わる)*/
		char buffer[64/*100*/];
		/* [ ハイスコア表示 ] */
		if (top_score < pd->my_score)
		{	top_score = pd->my_score;}
		sprintf(buffer,"%09d0", top_score); 			font_print_screen_xy(buffer,FONT01,PPP+1*8+3+5,15+2);
	//
		/* [ スコア表示 ] */
		sprintf(buffer,"%09d0", pd->my_score);			font_print_screen_xy(buffer,FONT01,PPP+1*8+3+5,40+2);
	//
		/* [ パワーゲージ表示 ] */
		draw_power_gauge(pd->weapon_power /*,PPP+7,124*/);
		if (pd->weapon_power > (127-1) /*== 128*/)/*max==127==「128段階」*/
		{
			strcpy(buffer, STR_MAX);
		}
		else
		{
			#if 0
		//	sprintf(buffer,"%d",(int)((dou ble)p->weapon / 128 * 100 ));	// [***090123		変更
			#else
			sprintf(buffer," %d", (((int)(pd->weapon_power) * 200) >>8) );		// [***090214		変更
			#endif
		}
		font_print_screen_xy(buffer,FONT01,PPP+7*8+3,125-2);
		//font_print_screen_xy(buffer,FONT01,PPP+8*8+3,125/*+1*/-2);
	//
		/* [ ボム有効時間表示 ] */
		//if (p->ex tra_type!=PLX_NONE)
		if (0 != pd->bomber_time)
		{
			sprintf(buffer, STR_TIME_"%3d",(int)(((int)pd->bomber_time)/10));
			font_print_screen_xy(buffer,FONT01,PPP+3*8-6,160);
		}
	//
		/* [ グレイズスコア表示 ] */
		sprintf(buffer," %d", pd->graze_point); 				font_print_screen_xy(buffer,FONT01,PPP+7*8+3,140);
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
			font_print_screen_xy( (char *)rank_name[(difficulty)&(4-1)], FONT01,PPP+7*8-4,256);
		}
	//
		#if 1
		/* [ fps 表示(60フレーム単位で表示) ] */
		/* (pspは1秒が59.なんたらフレームで、厳密に60でないらしいです。 ) */
		{
			static int ttt;
			static int fps_draw_wait_counter;
			fps_draw_wait_counter--;
			if (fps_draw_wait_counter < 1)
			{
				fps_draw_wait_counter = 60;
			//	[1/60sec]	0.016 666 [nsec] 6666666666666666666666666667
			//	16666.66666 / x == 60.0,  16666.666666/60.0 == x, x== 277.7777777777777
			//	16666.00(int) / 60.00(int) == 60.1660649819494584837545126353791 = 60.000 (int)
				ttt = psp_get_differencial_ticks();/* 前回計測時間との差分を 1[nano sec] 単位の signed int == 符号付32bit形で返す。 */
				if (0 != ttt)/* ゼロ 0 で割る場合を回避する(Devision by zero 防止) */
				{
					ttt = ( (60*60*16666) / ttt);
				}
			}
			sprintf(buffer, STR_TIME_"%3d",(int)(((int)ttt)));
			font_print_screen_xy(buffer,FONT01,PPP/*+8*4*/,240);
		}
		#endif
	//}
}

/*---------------------------------------------------------
	パネル表示、初期化
---------------------------------------------------------*/
#include "hiscore.h"/**/
extern int select_player;
void score_panel_init(void)
{
	top_score = high_score_table[select_player][0].score;	// 常に表示するハイコアの取得=>score.cで利用
	panel_base			= loadbmp0("panel/panel_base.png", 0, 1);
	power_gauge 		= loadbmp0("panel/power_gauge.png", 0, 1);
	SDL_SetColorKey(power_gauge,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	boss_gauge			= loadbmp0("panel/boss_gauge.png", 1, 1);/*2*/
}
/*---------------------------------------------------------
	以下メモ(フォントの形式が変わったので表示できない等)
---------------------------------------------------------*/

	//	sp rintf(buffer, STR_EXTRA_);					font_print_screen_xy(buffer,FONT01,PPP+1*8-2,170);

	//sp rintf(buffer,"SHIPS : %d",p->zanki);		//	font_print_screen_xy(buffer,FONT01,0,10);
	//sp rintf(buffer,"SPEED : %d",p->player_speed); //	font_print_screen_xy(buffer,FONT01,0,20);
	//sp rintf(buffer,"STAGE : %d",p->level);		//	font_print_screen_xy(buffer,FONT01,0,30);

	//sp rintf(buffer,"H_SCORE:");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,5);
	//sp rintf(buffer,"SCORE  :");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,30);
	//sp rintf(buffer,"GRAZE  :");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,140);
	//sp rintf(buffer,"POWER  :");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,110);
	//sp rintf(buffer,"PLAYER"); 					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,60);
	//sp rintf(buffer,"BOMB");						//	font_print_screen_xy(buffer,FONT01,PPP+1*8,85);

	/*
	switch (weapon_List) {
	case WP_PLASMA: 			strcat(buffer,"REIFU-1");		break;
	case WP_DOUBLEPLASMA:		strcat(buffer,"REIFU-2");		break;
	case WP_QUADPLASMA: 		strcat(buffer,"REIFU-4");		break;
	case WP_FIREBALL:			strcat(buffer,"YUMEFU-1");		break;
	case WP_DOUBLEFIREBALL: 	strcat(buffer,"YUMEFU-2");		break;
	case WP_QUADFIREBALL:		strcat(buffer,"YUMEFU-4");		break;
	case WP_FIFTHFIREBALL:		strcat(buffer,"YUMEFU-5");		break;
	case WP_KILLRAY:			strcat(buffer,"OFUDA"); 		break;
	default:					strcat(buffer,"UNKNOWN ???");	break;
	}
	font_print_screen_xy(buffer,FONT01,PPP+3*8,120);
	*/

//void score_cleanup()
//{
//}

