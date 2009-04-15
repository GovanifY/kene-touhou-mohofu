#include "score.h"

extern HSC_LIST hsc_table[];
extern SDL_Surface *screen;
extern SPRITE *player;
extern int high_score;

//static int weapon_List; /*player.c*/
SDL_Surface *bg2;

//extern int is_graze;				// player1が弾幕に触れたか？
SDL_Surface *weapon_p;		// ウェポンゲージ。俺の環境だとloadbmp2で作ると普通に表示されるようになった。
SDL_Surface *boss_hp;		//[***090305		追加:ボスHPゲージ


enum
{
	R_00_Player_Star_png = 0,
	R_01_Bomb_Star_png,
};

/* サイドパネルの横表示位置 pannel x offset */
#define PPP 380


// プレイヤーの星の表示
static void player_display( int g_num, int num, int y_suf)
{
	SDL_Surface *star;
	/*const*/ char *img_name[]=
	{
		"Player_Star.png",
		"Bomb_Star.png",
	};
	SDL_Rect recAll,rec;
	star = loadbmp((char *)img_name[g_num]);
	SDL_SetColorKey(star,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	if (num<1)	{	num=0;}

	rec.x = 0;
	rec.y = 0;
	rec.w = (10*num);
	rec.h = 10;

	recAll.x = PSP_WIDTH+3-7-(10*num);
	recAll.y = y_suf;
	recAll.w = 100;
	recAll.h = 11;
	SDL_BlitSurface(star, &rec, screen, &recAll);
}


// プレイヤーのウェポンゲージの表示。ketmに差分を上書きすると何故か普通に表示される。
static void power_status(int weapon/*, int dx, int dy*/)
{
	#define WP_GAUGE_X_OFS (PPP+7+9)
	#define WP_GAUGE_Y_OFS (124-2)
	SDL_Rect srec, drec;
	srec.x=0;
	srec.y=0;
	srec.h=13;
	#if 0
//	srec.w=(int)((dou ble)weapon / 127/*128*/ * (dou ble)weapon_p->w);		//[***090123		変更
	#else
	/* 1 dot ぐらい誤差あるかもしれないけど簡略化(高速化) */
	srec.w=((int)(weapon * weapon_p->w)>>7);		//[***090123		変更
	#endif
	drec.w=weapon_p->w;
	drec.h=weapon_p->h;
	drec.x=WP_GAUGE_X_OFS/*dx*/;
	drec.y=WP_GAUGE_Y_OFS/*dy*/;
	SDL_BlitSurface(weapon_p, &srec, screen, &drec);
}


/* ボスの HP 表示 */
static void health_status(/*int dx, int dy*/)		//[***090305		変更
{
	SPRITE *s = ((PLAYER_DATA *)player->data)->boss;
	/* boss hp 画像の文字幅分 / gauge stringth x width */
	#define HPGAUGE_X_OFS (50)
	#define HPGAUGE_Y_OFS (6)
	#define HP_FONT_X_OFS (37)
	/* 1024以上の値は数字で表示 */
	{char buffer[4/*3*/];
		sprintf(buffer,"%d", ((ENEMY_DATA *)s->data)->b.health>>10);
		font_print_screen_xy(buffer, FONT03, HP_FONT_X_OFS+10/*dx*/, 0/*+dy*/);
	}
	/* 1024未満の細かい値はグラフ表示 */
	{SDL_Rect srec, drec;
		srec.x=0;
		srec.y=0;
		srec.h=10;
		srec.w=HPGAUGE_X_OFS+((((ENEMY_DATA *)s->data)->b.health  & 0x03FF)>>2); /* 1023値 → 255ドット */
		drec.w=boss_hp->w;
		drec.h=boss_hp->h;
		drec.x= 10/*dx*/;
		drec.y=  0/*dy*/+HPGAUGE_Y_OFS;
		SDL_BlitSurface(boss_hp, &srec, screen, &drec);
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


void score_display(void)
{
	SDL_Rect bg2r={GAME_WIDTH, 0, 0, 0};	// データウィンドウ用rect->w,h,x,y
	PLAYER_DATA *p=(PLAYER_DATA *)player->data;
	char buffer[64/*100*/];
	SDL_BlitSurface(bg2,NULL,screen,&bg2r);

	//sprintf(buffer,"SHIPS : %d",p->lives);		//	font_print_screen_xy(buffer,FONT01,0,10);
	//sprintf(buffer,"SPEED : %d",p->player_speed); //	font_print_screen_xy(buffer,FONT01,0,20);
	//sprintf(buffer,"STAGE : %d",p->level);		//	font_print_screen_xy(buffer,FONT01,0,30);

	if (high_score < p->score)
	{	high_score = p->score;}
	//sprintf(buffer,"H_SCORE:");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,5);
	sprintf(buffer,"%09d0", high_score);				font_print_screen_xy(buffer,FONT01,PPP+1*8+3+5,15+2);

	//sprintf(buffer,"SCORE  :");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,30);
	sprintf(buffer,"%09d0", p->score);					font_print_screen_xy(buffer,FONT01,PPP+1*8+3+5,40+2);

	//sprintf(buffer,"PLAYER"); 					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,60);
	player_display( R_00_Player_Star_png, (p->lives), 69+2);

	//sprintf(buffer,"BOMB");						//	font_print_screen_xy(buffer,FONT01,PPP+1*8,85);
	player_display( R_01_Bomb_Star_png, (p->bombs), 94+1);

	//sprintf(buffer,"POWER  :");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,110);
	power_status(p->weapon /*,PPP+7,124*/);
	if (p->weapon > (127-1) /*== 128*/)/*max==127==「128段階」*/
	{
		strcpy(buffer, STR_MAX);
	}
	else
	{
		#if 0
	//	sprintf(buffer,"%d",(int)((dou ble)p->weapon / 128 * 100 )); 	//[***090123		変更
		#else
		sprintf(buffer," %d", (((int)(p->weapon) * 200) >>8) ); 	//[***090214		変更
		#endif
	}
	font_print_screen_xy(buffer,FONT01,PPP+7*8+3,125-2);
	//font_print_screen_xy(buffer,FONT01,PPP+8*8+3,125/*+1*/-2);

	if (p->extra_type!=PLX_NONE)
	{
		sprintf(buffer, STR_TIME_"%3d",(int)(((int)p->bomber_time)/10));
		font_print_screen_xy(buffer,FONT01,PPP+3*8-6,160);
	}
	//	sprintf(buffer, STR_EXTRA_);					font_print_screen_xy(buffer,FONT01,PPP+1*8-2,170);

	{
		/*const*/ char *rank_name[4]=
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


	//sprintf(buffer,"GRAZE  :");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,140);

	sprintf(buffer," %d", p->graze);					font_print_screen_xy(buffer,FONT01,PPP+7*8+3,140);

	if (((PLAYER_DATA *)player->data)->bossmode==B01_BATTLE)
	{
		health_status(/*10, 6-6*/);
	}
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
}

//void score_cleanup()
//{
//}
