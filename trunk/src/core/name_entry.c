
/*---------------------------------------------------------

---------------------------------------------------------*/

#include "game_main.h"
#include "name_entry.h"/**/
extern int select_player;

/*---------------------------------------------------------
	ハイスコア表示デモ画面
---------------------------------------------------------*/

/*global*/
/*extern*/ SCORE_FORMAT high_score_table[MAX_8_SAVE_PLAYERS][MAX_5_RANKING/*5*/];

/*local static*/

static int move_done_lines; 			/* 移動完了した行の数 */
static int more_show;					/* 別の戦歴をまだ見る */
static int show_player_num; 			/* スコア戦歴表示中のプレイヤー番号 */

enum
{
	RESULT_DATA_00_XG256 = 0,
	RESULT_DATA_01_YG256,
	RESULT_DATA_02_PHASE_SPEED256,
	RESULT_DATA_03_MAX		/* 最大数 */
};
#define MAX_7_LINES (7/*6*/)
static const signed int result_const_status[RESULT_DATA_03_MAX][MAX_7_LINES] =
{
	{/* RESULT_DATA_00_XG256 */
		/*(t256( 4)*0) +*/ t256(64/* 20*/), 	/*1ST*/
		/*(t256( 4)*1) +*/ t256(64/* 20*/), 	/*2ND*/
		/*(t256( 4)*2) +*/ t256(64/* 20*/), 	/*3RD*/
		/*(t256( 4)*3) +*/ t256(64/* 20*/), 	/*4TH*/
		/*(t256( 4)*4) +*/ t256(64/* 20*/), 	/*5TH*/
		t256( (64+32)/*30*/ ),					/*"RESULT OF SCORE"*/
		t256( (64-32)/*30*/ ),					/*"REIMU"*/
	},
	{/* RESULT_DATA_01_YG256 */
		(t256( (20*0)+(144+4)/*150*/) ),	/*1ST*/ 	//(25*0)+(110+25+5)/*150*/
		(t256( (20*1)+(144+4)/*150*/) ),	/*2ND*/ 	//(25*1)+(110+25+5)/*150*/
		(t256( (20*2)+(144+4)/*150*/) ),	/*3RD*/ 	//(25*2)+(110+25+5)/*150*/
		(t256( (20*3)+(144+4)/*150*/) ),	/*4TH*/ 	//(25*3)+(110+25+5)/*150*/
		(t256( (20*4)+(144+4)/*150*/) ),	/*5TH*/ 	//(25*4)+(110+25+5)/*150*/
		t256(110),								/*"RESULT OF SCORE"*/
		t256(128-2),							/*"REIMU"*/
	},
	{/* RESULT_DATA_02_PHASE_SPEED256 */
		4,		/*1ST*/
		4,		/*2ND*/
		4,		/*3RD*/
		4,		/*4TH*/
		4,		/*5TH*/
		6,		/*"RESULT OF SCORE"*/
		5,		/*"REIMU"*/
	},
};

/*---------------------------------------------------------
	ハイスコア表示デモ画面
---------------------------------------------------------*/

typedef struct
{
	int x256;
	int y256;
//
	int amplifier256;		/* 横のズレ量の割合。 amplifier x offset */
	int phase256;			/* 横のズレ量。 phase x offset */
	int phaseout256;		/* 横のズレ量。 phase x offset */
	int direction;			/* ズレる方向 */
	int move_done_flag; 	/* 移動完了したか、してないかの状態 */
//	定数
	int xg256;
	int yg256;
	int amplifier_speed256;
	int phase_speed256;
} RR_OBJ;

static RR_OBJ result_my_obj[MAX_7_LINES/*6*/];
static SDL_Surface *result_surfaces[MAX_7_LINES/*6*/];


/*---------------------------------------------------------
	ハイスコア表示デモ画面(文字移動用)
---------------------------------------------------------*/

static void move_result(void)
{
	int i;
	for (i=0; i<MAX_7_LINES; i++)
	{
		if (0==result_my_obj[i].move_done_flag) 	/* 移動は必要？ */
		{
			result_my_obj[i].x256 = ((result_my_obj[i].xg256) );
			result_my_obj[i].y256 = ((result_my_obj[i].yg256) );
			if (0==result_my_obj[i].direction)		/* 移動する向き */
			{
				/* slide-in */
				result_my_obj[i].amplifier256 -= result_my_obj[i].amplifier_speed256;
				if (result_my_obj[i].amplifier256 <= t256(0.3) )
				{
					result_my_obj[i].move_done_flag=1;	/* 移動完了 */
					move_done_lines++;
				}
				else
				{
					result_my_obj[i].phase256 += result_my_obj[i].phase_speed256;/*fps_factor*/
					result_my_obj[i].x256 += ((result_my_obj[i].phase256*result_my_obj[i].amplifier256)>>8);
				}
			}
			else
			{
				result_my_obj[i].phaseout256 += result_my_obj[i].phase_speed256;/*fps_factor*/
				result_my_obj[i].x256 += ((result_my_obj[i].phaseout256*result_my_obj[i].amplifier256)>>8);
				/* slide-out */
				result_my_obj[i].amplifier256 += result_my_obj[i].amplifier_speed256;
				if (
						(t256(PSP_WIDTH480)  < result_my_obj[i].x256)	/* 画面外 */
					)
				{
					result_my_obj[i].move_done_flag=1;	/* 移動完了 */
					move_done_lines++;
				}
			}
		}
	}
}


/*---------------------------------------------------------
	ハイスコア表示デモ画面
---------------------------------------------------------*/
static void result_font_render(void)
{
	int i;
	for (i=0; i<MAX_7_LINES; i++)
	{
		if (i<5)
		{
			char ttmp[64/*50*/];
			sprintf(ttmp,
				"%1d "		/* 順位 */
				"%-8s " 	/* プレイヤー名 */
				"%09d0" 	/* スコア */
				"[%1d]",	/* 到達ステージ */
				(i+1),	/* 順位 */
				high_score_table[(show_player_num)][i].name,
				high_score_table[(show_player_num)][i].score,
				high_score_table[(show_player_num)][i].final_stage
				);
		//	result_surfaces[i]		= font_render(ttmp,(0==i)?FONT05:FONT01);
		//	result_surfaces[i]		= font_render(ttmp,(0==i)?FONT03:FONT07);
			result_surfaces[i]		= font_render(ttmp, FONT03);
		}
		else
		if (5==i)
		{
			result_surfaces[5]		= font_render("RESULT OF SCORE", FONT05);/*"TOP FIVE FIGHTERS"*/
		}
		else
		if (6==i)
		{
			/*const*/ char *score_name[8/*5*/] =
			{
			/* 0==*/	"REIMU_A",
			/* 1==*/	"REIMU_B",
			/* 2==*/	"MARISA_A",
			/* 3==*/	"MARISA_B",
			/* 4==*/	"REMILIA",
			/* 5==*/	"YUYUKO",
			/* 6==*/	"CHIRNO_A",
			/* 7==*/	"CHIRNO_Q",
			};
			result_surfaces[6]				= font_render((char *)score_name[(show_player_num)], FONT03/*FONT06*/);
		}

//		result_my_obj[i]->data			= hd;
		result_my_obj[i].xg256							= result_const_status[RESULT_DATA_00_XG256][i];
		result_my_obj[i].yg256							= result_const_status[RESULT_DATA_01_YG256][i];
		result_my_obj[i].phase_speed256 				= result_const_status[RESULT_DATA_02_PHASE_SPEED256][i];
		result_my_obj[i].amplifier_speed256 			= t256(10.0);//t256(1.0);
//		result_my_obj[i].flags			|= (SP_FLAG_VISIBLE);
//		result_my_obj[i].type			= S P_ME NU_TEXT/*S P_M UTEKI*/;
		result_my_obj[i].x256			= 0;					//	result_my_obj[5]->x 	= 0/*30*/;
		result_my_obj[i].y256			= 0/*(i*25+110)*/;		//	result_my_obj[5]->y 	= 0/*50*/;
//		result_my_obj[i].callback_mover = move_result;
	}
	for (i=0; i<MAX_7_LINES/*5*/; i++)
	{

		result_my_obj[i].phase256			= (i<<6);/*(0)*/
		result_my_obj[i].phaseout256		= ((4-i)<<6);/*(5<<6)*/
		result_my_obj[i].amplifier256		= t256(100/*300*/);
		result_my_obj[i].direction			= 0;
		result_my_obj[i].move_done_flag 	= 0;
	}
}
static void result_font_free(void)
{
	int i;
	for (i=0; i<MAX_7_LINES; i++)
	{
//		result_my_obj[i].type = SP_DELETE;
		#if 1
		/* ここでハングアップ */
	//	if (result_surfaces[i])
		{
			SDL_FreeSurface(result_surfaces[i]);
		}
		#endif
	}
}
/*---------------------------------------------------------
	ハイスコア表示デモ画面
---------------------------------------------------------*/

//static SDL_Surface *back;
//extern int select_player;
enum
{
	RESULT_00_INIT = 0,
	RESULT_01_SET_LOCATION,
	RESULT_02_SLIDE_IN,
	RESULT_03_PAD_CHECK,
	RESULT_04_SLIDE_OUT,
	RESULT_05_RETRY_CHECK,
};

void result_work(void)
{
	if ( (ST_WORK_RESULT|RESULT_00_INIT) != (psp_loop) )
	{
		psp_pop_screen();	//SDL_BlitSurface(back,NULL,sdl_screen[SDL_00_VIEW_SCREEN],NULL);
	}
	switch ((u8)(psp_loop&0xff)/*state.substate*/)
	{
	case RESULT_00_INIT:
		//void result_init(void)
		show_player_num = (select_player & 7);/*0*/
		psp_push_screen();	//back=SDL_ConvertSurface(screen,screen->format,screen->flags);
		//if (NULL==back)
		//{
		//	CHECKPOINT;
		//	error(ERR_FATAL, "cant create background surface");
		//}
		psp_loop++;
		break;
	case RESULT_01_SET_LOCATION:
		{
			move_done_lines=0;
			result_font_render();
			psp_loop++;
		}
		break;
	case RESULT_02_SLIDE_IN:
		if (move_done_lines==MAX_7_LINES)
		{
			psp_loop++;
		}
		break;
	case RESULT_03_PAD_CHECK:
		if (0==my_pad)/* 今何も押されてなくて */
		{
			/* 次の人の戦歴を見る */
			if (my_pad_alter & (PSP_KEY_RIGHT|PSP_KEY_LEFT|PSP_KEY_UP|PSP_KEY_DOWN))	/* さっきPSP_KEY_SHOT_OKが押されてた */
			{
				show_player_num++;								/* 次の人にする */
			//	if (/*4*/7<show_player_num) {show_player_num=0;}	/* 最後まで見たら始めから見る */
				show_player_num &= 7;								/* 最後まで見たら始めから見る */
				more_show=1;	/* また見るよ */
				psp_loop++;
			}
			/* 見るのやめてメニューに戻る */
			else
			if (my_pad_alter & (PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL|PSP_KEY_OPTION|PSP_KEY_PAUSE|PSP_KEY_SELECT))
			{
				more_show=0;	/* もうおしまい */
				psp_loop++;
			}
		}
		break;
	case RESULT_04_SLIDE_OUT:
		{
			move_done_lines=0;
			int i;
			for (i=0;i<MAX_7_LINES;i++)
			{
				result_my_obj[i].direction = 1;
				result_my_obj[i].move_done_flag = 0;
			}
			psp_loop++;
		}
		break;
	case RESULT_05_RETRY_CHECK:
		if (move_done_lines==MAX_7_LINES)
		{
			result_font_free();
		//	if (1==more_show)	/* pspは0レジスタがあるので0と比較したほうが速い */
			if (0!=more_show)	/* また見るよ */
			{
				psp_loop = (ST_WORK_RESULT|RESULT_01_SET_LOCATION);
			}
			else	/* if (0==more_show) もうおしまい */
			{
			//	SDL_FreeSurface(back);
				psp_loop = (ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);
			}
		}
		break;
	}
	if ((ST_WORK_RESULT|RESULT_01_SET_LOCATION) < psp_loop)
	{
//	sp rite_work_SDL(SP_GROUP_PAUSE_S P_ME NU_TEXT);/*SP_GROUP_ETC*/
//	pause_sp rite_display();/*SP_GROUP_ETC*/
		{
			move_result();
		}
		/* 描画 */
		{			/*static*/ SDL_Rect rect_locate_offset; 	/* 表示位置 */
			int i;
			for (i=0; i<MAX_7_LINES; i++)
			{
				rect_locate_offset.x = ((result_my_obj[i].x256)>>8);
				rect_locate_offset.y = ((result_my_obj[i].y256)>>8);
				SDL_SetAlpha(	result_surfaces[i], SDL_SRCALPHA, /*bg_alpha_aaa*/(255));
				SDL_BlitSurface(result_surfaces[i], NULL,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
			}
		}
	}
}

/*---------------------------------------------------------
	ハイスコア名前入力画面
---------------------------------------------------------*/

/*global*/int last_score;


static char *entry;

#define KEYBOARD_W10	(16)
#define KEYBOARD_H04	(5)
#define KEYBOARD_M40	((KEYBOARD_W10)*(KEYBOARD_H04))

#define MAX_40_LETTER	(KEYBOARD_M40)
static SDL_Surface *letter_surface[MAX_40_LETTER];

typedef struct
{
	int xpos;
	int ypos;
	int dummy_scale256;
	char ascii;
} LETTER;
static LETTER letter[MAX_40_LETTER];

#define MY_CODE_DEL_KEY (-1)
#define MY_CODE_OK_KEY	(-2)

static int now_select_name_chr;
static int plate_x;
static int plate_y;

static /*un*/signed int sel_aaa;

enum
{
	NAME_ENTRY_00_INIT = 0,
	NAME_ENTRY_03_ENTRY,
};

/* ランキング表示y */
#define RANK_LOCATE_X_48		(40)
#define RANK_LOCATE_OFS_X_08	(8)/*24*/

static void name_entry_init(void)
{
	int i;
	int j;
	/* キーボード(の文字)の初期化 */
	char tmp_str[96];/*64*/ /* 96 == 3 x [32] */
	tmp_str[1] = '\0';/* 0 == EOS 文字列の終わり / End of String */
	for (i=0; i<(KEYBOARD_M40-2); )
	{
		const char *str_aaa = /* 8*5==40==38(chars)+1(dummy)+1(0) */
	//	{ "ABCDEFG123" "HIJKLMN456" "OPQRSTU789" "VWXYZ.-0" /*.align*/" " };
		{
			/* 現在定義されているフォントが少ない為 */
			"ABCDEFGHIJKLMNOP"
			"QRSTUVWXYZ..:://"	/* 注意:スコア記録形式を変えるまで ','文字は使えない。 */
			"ABCDEFGHIJKLMNOP"
			"QRSTUVWXYZ--//--"
			"0123456789[]!?" /*.align*/"  "
		};
		unsigned char c;
		c = str_aaa[i];
		tmp_str[0] = c;
		letter[i].ascii = c; letter_surface[i] = font_render(tmp_str, FONT03/*FONT02*/); /* */
		i++;
	}
	letter[i].ascii = MY_CODE_DEL_KEY;	letter_surface[i++] = font_render(/*"DEL"*/"/",FONT03/*FONT02*/); /* 39 */
	letter[i].ascii = MY_CODE_OK_KEY;	letter_surface[i++] = font_render(/*"OK"*/"!", FONT03/*FONT02*/); /* 40 */
	{
		int k;
		k = 0;
		for (j=0; j<(KEYBOARD_H04*20);/*(4*36)*/ j+=(20))	 /*(36)*/ /*25*/
		{
			for (i=0; i<(KEYBOARD_W10*20);/*(10*30)*/ i+=(20))/*(30)*/ /*25*/
			{
				#if 0/* Gu側と合成できる訳はないので、現状合っても無くても結果はほぼ同じ。 */
				/* ここを有効にした場合、よく見ると文字文字同士の SDL-SDL 合成は出来てる。 */
				SDL_SetColorKey(letter_surface[k],(SDL_SRCCOLORKEY),0x00000000);	/* 現状 SDL合成だが、やめてみるてすと。 */
				#endif
				letter[k].xpos = (i) + (16);	/*32*/		/*48*/ /* 30*/
				letter[k].ypos = (j) + (168);	/*160*/ 	/*84*/ /*100*/
				k++;
			}
		}
	}
	/* キーボード(の文字)の拡大率を初期化 */
//	for (i=0; i<KEYBOARD_M40; i++)
//	{
//		letter[i].scale256 = t256(1.0);/*0.0*/
//	}
	/* 新たにランクインしたスコア位置を調べる */
	for (i=0; i<5; i++)
	{
		if (last_score > high_score_table[select_player][i].score)
		{
			break;
		}
	}
	/* 以降 i がランクインした位置。判りにくいので、別名で定義しとく。 */
	#define 	my_rank 	i
	/* スコア位置より下を移動させる */
	for (j=4; j>my_rank; j--)
	{
		high_score_table[select_player][j] = high_score_table[select_player][j-1];
	}
	/* 新たにランクインしたスコア位置へ挿入 */
	high_score_table[select_player][my_rank].score			= last_score;
	high_score_table[select_player][my_rank].final_stage	= player_now_stage;/**/

//
	entry = high_score_table[select_player][my_rank].name;
	entry[0] = ' ';
	entry[1] = ' ';
	entry[2] = ' ';
	entry[3] = ' ';
//
	entry[4] = ' ';
	entry[5] = ' ';
	entry[6] = ' ';
	entry[7] = ' ';
	entry[8] = 0;
	now_select_name_chr = 0;

	/* ｎ位タイトル描画 */
	{
		for (j=0; j<5; j++)
		{// /*static*/ SDL_Surface *headline;
		//	result_surfaces[j]		= font_render(/*tmp_str*/(&aaaa[my_rank][0]),FONT03);
			{// char ttmp[64/*50*/];
				sprintf(tmp_str,
					"%1d %-8s %09d0",
					(j+1),
					high_score_table[(select_player)][j].name,
					high_score_table[(select_player)][j].score);
				result_surfaces[j]		= font_render(tmp_str, (my_rank==j)?(FONT03/*白*/):(FONT05/*紅(黄)*/) );
			}
			SDL_Rect r;
		//	r.x = (GAME_WIDTH/2)-((result_surfaces[j]->w)>>1);
		//	r.y = (40);
			r.x = (RANK_LOCATE_OFS_X_08);
			r.y = (RANK_LOCATE_X_48)+(j*20);
			r.w = result_surfaces[j]->w;
			r.h = result_surfaces[j]->h;
			SDL_BlitSurface(result_surfaces[j], NULL, sdl_screen[SDL_00_VIEW_SCREEN], &r);
			SDL_FreeSurface(result_surfaces[j]);
		}
	}
	#if 0
	/* 名前プレート描画 */
	{
		/*static*/ SDL_Surface *plate;
		plate = loadbmp0("panel/name_plate.png", 0, 0/*1*/);
		SDL_Rect r;
	//	plate_x = r.x = (GAME_WIDTH/2)-((plate->w)>>1);
	//	plate_y = r.y = (GAME_HEIGHT-12)-(plate->h);
		plate_x = r.x = (RANK_LOCATE_OFS_X_08+32);
		plate_y = r.y = (RANK_LOCATE_X_48)+(my_rank*20);
		r.w = plate->w;
		r.h = plate->h;
		SDL_BlitSurface(plate,NULL,sdl_screen[SDL_00_VIEW_SCREEN],&r);
		if (plate)
		{
			unloadbmp_by_surface(plate);/*unload_bmp_by_name("panel/name_plate.png");*/
		}
	}
	#else
	plate_x = (RANK_LOCATE_OFS_X_08+32);
	plate_y = (RANK_LOCATE_X_48)+(my_rank*20);
	#endif
	psp_push_screen();	//back=SDL_ConvertSurface(screen,screen->format,screen->flags);
	//if (NULL==back)
	//{
	//	CHECKPOINT;
	//	error(ERR_FATAL, "cant create background surface");
	//}
//	sel_aaa = -1;
	sel_aaa = 0;
	psp_loop++;// = (ST_WORK_NAME_ENTRY|NAME_ENTRY_03_ENTRY);
}


/*---------------------------------------------------------
	ハイスコア名前入力画面、動作(の子関数)[表示部分]
---------------------------------------------------------*/

static void name_entry_draw(void)
{
	psp_pop_screen();	//SDL_BlitSurface(back,NULL,screen,NULL);	/* 取って置いた背景で画面を描く */
//
	SDL_Rect s;
	SDL_Rect r;
	s.x = 0;
	s.y = 0;
	/* キーボード文字を描画 */
	{
		int i;
		for (i=0; i<KEYBOARD_M40; i++)
		{
			int sw; sw	= (letter_surface[i]->w); s.w	= (/*letter_surface[i]->w*/sw); 		r.w 	= ((/*s.w*/sw));
			int sh; sh	= (letter_surface[i]->h); s.h	= (/*letter_surface[i]->h*/sh); 		r.h 	= ((/*s.h*/sh));
			r.x = (letter[i].xpos);//-(((s.w)*letter[i].scale256)>>(8+3));/*(8+1)*/
			r.y = (letter[i].ypos);//-(((s.h)*letter[i].scale256)>>(8+3));/*(8+1)*/

			if (i == sel_aaa)/* 選択中文字==キーボード文字 */
			{
//				letter[i].scale256 = t256(1.6);
				{
					static int angle512/*=0*/;
					angle512 += deg_360_to_512CCW(360-(5)/*15*/);/*fps_factor*/ 	/* CCWの場合 */
					mask512(angle512);//	if (angle>360)	{	angle-=360;}
					r.x 	+= ((sin512((angle512)))>>5);/*xa*/
					r.y 	+= ((sin512((angle512)))>>6);/*ya*/ /*xa>>1*/
				}
			}
//			else/* 選択中文字 != キーボード文字 */
//			{
//				letter[i].scale256 = t256(1.0);
//			}
			/* 描画 */
			/*blit_scaled*/SDL_BlitSurface(letter_surface[i],&s,sdl_screen[SDL_00_VIEW_SCREEN],&r);
		}
	}
	/* 決定文字を描画 */
/*	if (0 <= sel_aaa)*/
	{
		SDL_Surface *e;
		e		= font_render(entry,FONT03/*FONT02*/);
	//	s.x 	= 0;
	//	s.y 	= 0;
		s.w 	= e->w; 	r.w 	= e->w;/**2*/
		s.h 	= e->h; 	r.h 	= e->h;/**2*/
		r.x 	= plate_x; /*+ 5*/
		r.y 	= plate_y; /*+ 5*/
		/*blit_scaled*/SDL_BlitSurface(e,&s,sdl_screen[SDL_00_VIEW_SCREEN],&r);
		SDL_FreeSurface(e);
	}
}

/*---------------------------------------------------------
	ハイスコア名前入力画面、動作[キー入力＆状態変更部分]
---------------------------------------------------------*/

void name_entry_work(void)
{
	if ((ST_WORK_NAME_ENTRY|0)==psp_loop)
	{
		name_entry_init();
	}
	else
	//if ((ST_WORK_NAME_ENTRY|NAME_ENTRY_03_ENTRY)==psp_loop)
	{
		if (0 == my_pad)/* 今何も押されてなくて */
		{
			/* 斜め移動はしない */
				 if (my_pad_alter & PSP_KEY_LEFT )	{	sel_aaa--;				if (sel_aaa <  (0)) 				sel_aaa  = (KEYBOARD_M40-1);	}
			else if (my_pad_alter & PSP_KEY_RIGHT)	{	sel_aaa++;				if (sel_aaa == (KEYBOARD_M40))		sel_aaa  = (0); 				}
			else if (my_pad_alter & PSP_KEY_UP	 )	{	sel_aaa-=KEYBOARD_W10;	if (sel_aaa <  (0)) 				sel_aaa += (KEYBOARD_M40);		}
			else if (my_pad_alter & PSP_KEY_DOWN )	{	sel_aaa+=KEYBOARD_W10;	if (sel_aaa >  (KEYBOARD_M40-1))	sel_aaa -= (KEYBOARD_M40);		}
			//

			//
			if (my_pad_alter & PSP_KEY_SHOT_OK) 	/* 入力決定 == (さっき)入力決定ボタンが押されてた。 */
			{
				switch (letter[sel_aaa].ascii)
				{
				case MY_CODE_DEL_KEY: /* [削除キー](DEL)を決定した場合。 Delete last character */
					goto delete_last_character;
					break;
				case MY_CODE_OK_KEY: /* [入力終了キー](OK)を決定した場合。 Input completed. 入力終了。 Eingabe abgeschlossen. */
					goto agree_entry;
					break;
				default:
					if (now_select_name_chr < 8) /* 8[3]文字以下決定した場合は文字入力。  3 chrs, name input entry. */
					{
						entry[now_select_name_chr] = letter[sel_aaa].ascii;
						now_select_name_chr++;
					}
					else /* 8[3]文字以上の決定した場合は自動的に入力終了キーを押したことにする。 */
					{
						sel_aaa = (KEYBOARD_M40-1);/* [入力終了キー](OK)を押したのと同じ。 force set [OK] */
					}
					break;
				}
			}
			else if (my_pad_alter & PSP_KEY_BOMB_CANCEL/*PSP_KEY_OPTION*/)	/* (さっき)キャンセルボタンが押されてた。 */
			{
			delete_last_character:
				if (0 < now_select_name_chr) /* 名前入力の入力文字がある場合で。  at first chr? */
				{
					now_select_name_chr--;				/* [削除キー](DEL) が入力されているので、その分戻る。 */
					entry[now_select_name_chr] = ' ';	/* 消す */
				}
			}
			else if (my_pad_alter & PSP_KEY_PAUSE)		/* (さっき)終了(強制決定)ボタンが押されてた。 */
			{
			agree_entry:	/* 入力終了決定 */
			//	if (str cmp(entry,"   "))		// 名前入力の入力文字がある場合で。何か入力されている場合で。(str cmpは==だと0を返す)
				if (0 < now_select_name_chr)	// 名前入力の入力文字がある場合で。何か入力されている場合で。(str cmpは==だと0を返す)
				{
					#if 0
					/* キーボードの文字を開放する */
					{	int i;
						for (i=0; i<KEYBOARD_M40; i++)
						{
							#if 0
						//	if (letter_surface[i])
							{
								/* ここでハングアップ */
								SDL_FreeSurface(letter_surface[i]);
							}
							#endif
						}
					}
				//	SDL_FreeSurface(back);
					#endif
					psp_loop = (ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);
					return;
				}
			}
		}
		name_entry_draw();/*描画部分*/
	}
}
