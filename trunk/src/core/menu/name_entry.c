
// このファイルには2つユニットがあります。
// 1. ハイスコア表示デモ画面(夢の記録)
// 2. ハイスコア名前入力、登録画面

//////////////////////////////////////////////////////////////////

#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ハイスコア表示デモ画面(夢の記録)
---------------------------------------------------------*/

#define MAX_7_LINES (7/*6*/)

global SCORE_FORMAT high_score_table[MAX_8_SAVE_PLAYERS][MAX_5_RANKING/*5*/];

	/*static*/static/*global*/ SDL_Surface *result_surfaces[MAX_7_LINES/*6*/];


/*---------------------------------------------------------
	ｎ位タイトル描画サブ
	サーフェイスを作成するが、開放はしないので注意。
	-------------------------------------------------------
	このサブルーチンは、名前入力画面でも使います。
---------------------------------------------------------*/
/*static*/static/*global*/ void high_score_render_sub(int show_player_num, int iii, int bbb)
{
	char ttmp[64/*50*/];
	sprintf(ttmp,
		"%1d "		/* 順位 */
		"%-8s " 	/* プレイヤー名 */
		"%09d0" 	/* スコア */
		"[%1d]",	/* 到達ステージ */
	//
		(iii+1),												/* 順位 */
		high_score_table[(show_player_num)][iii].name,			/* プレイヤー名 */
		high_score_table[(show_player_num)][iii].score, 		/* スコア */
		high_score_table[(show_player_num)][iii].final_stage	/* 到達ステージ */
	);

	result_surfaces[iii]		= font_render( ttmp, (bbb)?(FONT16R/*白*/):(FONT16W/*紅(黄)*/) );
}


/*---------------------------------------------------------
	ハイスコア表示デモ画面(夢の記録)
---------------------------------------------------------*/

/*local static*/

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

static int move_done_lines; 			/* 移動完了した行の数 */
static int show_player_num; 			/* スコア戦歴表示中のプレイヤー番号 */
//static int more_show; 				/* 別の戦歴をまだ見る */


/*---------------------------------------------------------
	ハイスコア表示デモ画面(夢の記録)(文字移動用)
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
	ハイスコア表示デモ画面(夢の記録)
---------------------------------------------------------*/

static void result_font_render(void)
{
	int i;
	for (i=0; i<MAX_7_LINES; i++)
	{
		if (i<5)
		{
			high_score_render_sub(show_player_num, i, (1));/*FONT16R*/	/*白*/
		}
		else
		if (5==i)
		{
			result_surfaces[5]		= font_render( (char*)"RESULT OF SCORE", FONT16W);/*"TOP FIVE FIGHTERS"*/
		}
		else
		if (6==i)
		{
			static const/*const*/ char *score_name[8/*5*/] =
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
			result_surfaces[6]				= font_render( (char *)score_name[(show_player_num)], FONT16R);
		}
		{
			enum
			{
				RESULT_DATA_00_XG256 = 0,
				RESULT_DATA_01_YG256,
				RESULT_DATA_02_PHASE_SPEED256,
				RESULT_DATA_03_MAX		/* 最大数 */
			};
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
					(t256( (20*0)+(144+4)/*150*/) ),		/*1ST*/ 	//(25*0)+(110+25+5)/*150*/
					(t256( (20*1)+(144+4)/*150*/) ),		/*2ND*/ 	//(25*1)+(110+25+5)/*150*/
					(t256( (20*2)+(144+4)/*150*/) ),		/*3RD*/ 	//(25*2)+(110+25+5)/*150*/
					(t256( (20*3)+(144+4)/*150*/) ),		/*4TH*/ 	//(25*3)+(110+25+5)/*150*/
					(t256( (20*4)+(144+4)/*150*/) ),		/*5TH*/ 	//(25*4)+(110+25+5)/*150*/
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
			result_my_obj[i].xg256							= result_const_status[RESULT_DATA_00_XG256][i];
			result_my_obj[i].yg256							= result_const_status[RESULT_DATA_01_YG256][i];
			result_my_obj[i].phase_speed256 				= result_const_status[RESULT_DATA_02_PHASE_SPEED256][i];
		}
		result_my_obj[i].amplifier_speed256 			= t256(10.0);//t256(1.0);
		result_my_obj[i].x256			= 0;					//	result_my_obj[5]->x 	= 0/*30*/;
		result_my_obj[i].y256			= 0/*(i*25+110)*/;		//	result_my_obj[5]->y 	= 0/*50*/;
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
		#if 1/* 文字の表示用画像を開放する */
		/* ここでハングアップ */
	//	if (result_surfaces[i])
		{
			SDL_FreeSurface(result_surfaces[i]);
		}
		#endif
	}
}


/*---------------------------------------------------------
	ハイスコア表示デモ画面(夢の記録)(本体)
---------------------------------------------------------*/

enum
{
	RESULT_01_SET_LOCATION = 0,
	RESULT_02_SLIDE_IN,
	RESULT_03_PAD_CHECK,
	RESULT_04_SLIDE_OUT,
	RESULT_05_RETRY_CHECK,
};

static u8 my_ppp_loop;
static void yume_no_kiroku_local_work(void)
{
	{
		psp_pop_screen();	/* 取って置いた背景で画面を描く */
	}
	switch ((u8)(my_ppp_loop))
	{
	case RESULT_01_SET_LOCATION:
		{
			move_done_lines = 0;
			result_font_render();
			my_ppp_loop++;
		}
		break;
	case RESULT_02_SLIDE_IN:
		if (move_done_lines==MAX_7_LINES)
		{
			my_ppp_loop++;
		}
		break;
	case RESULT_03_PAD_CHECK:
		if (0==my_pad_alter)/* さっき何も押されてなかった場合にキーチェック(原作準拠) */
		{
			/* 次の人の戦歴を見る */
			if (my_pad & (PSP_KEY_RIGHT|PSP_KEY_LEFT|PSP_KEY_UP|PSP_KEY_DOWN))	/* PSP_KEY_SHOT_OKが押された */
			{
				show_player_num++;										/* 次の人にする */
			//	if (/*4*/7<show_player_num) 	{show_player_num=0;}	/* 最後まで見たら始めから見る */
				show_player_num &= (0x07);								/* 最後まで見たら始めから見る */
			//	more_show = 1;	/* また見るよ */
				my_ppp_loop++;
			}
			/* 見るのやめてメニューに戻る */
			else
			if (my_pad & (PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL|PSP_KEY_OPTION|PSP_KEY_PAUSE|PSP_KEY_SELECT))
			{
			//	more_show = 0;	/* もうおしまい */
				return_call_func = title_menu_start;	/* タイトルメニューへ移動設定(反映するのは、まだ先) */
				my_ppp_loop++;
			}
		}
		break;
	case RESULT_04_SLIDE_OUT:
		{
			move_done_lines = 0;
			int i;
			for (i=0; i<MAX_7_LINES; i++)
			{
				result_my_obj[i].direction = 1;
				result_my_obj[i].move_done_flag = 0;
			}
			my_ppp_loop++;
		}
		break;
	case RESULT_05_RETRY_CHECK:
		if (move_done_lines==MAX_7_LINES)
		{
			result_font_free();
			my_ppp_loop = (RESULT_01_SET_LOCATION); 	/* 「また見るよ」(「もうおしまい」の場合も描画を飛ばす為に設定する) */
			main_call_func = return_call_func;	/* 「また見るよ」&「もうおしまい」の状態を反映させる。 */
		}
		break;
	}
	if ((RESULT_01_SET_LOCATION) < my_ppp_loop)
	{
		/* 移動 */
		{
		//	sp rite_work_SDL(SP_GROUP_PAUSE_S P_ME NU_TEXT);/*SP_GROUP07_FRONT*/
		//	pause_sp rite_display();/*SP_GROUP07_FRONT*/
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
				SDL_BlitSurface(result_surfaces[i], NULL, sdl_screen[SDL_00_VIEW_SCREEN], &rect_locate_offset);
			}
		}
	}
}


/*---------------------------------------------------------
	ハイスコア表示デモ画面(夢の記録)(開始、初期設定)
---------------------------------------------------------*/

extern int select_player;

global void yume_no_kiroku_start(void)
{
	//void result_init(void)
	show_player_num = (select_player & 0x07);/*0*/	/* 現在選択されているプレイヤーから記録を表示開始する。 */
	psp_push_screen();	/* 現在の表示画面を裏画面に保存 */
	my_ppp_loop = RESULT_01_SET_LOCATION;
	main_call_func		= yume_no_kiroku_local_work;/* 動作先 */
	return_call_func	= yume_no_kiroku_local_work;/* 戻り動作先、「また見るよ」に設定 */
}


//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////

//#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ハイスコア名前入力、登録画面
---------------------------------------------------------*/

extern int select_player;

//	/*static*/extern void high_score_render_sub(int show_player_num, int iii, int bbb);
//	/*static*/extern SDL_Surface *result_surfaces[];

/*---------------------------------------------------------
	ハイスコア名前入力、登録画面
---------------------------------------------------------*/

global u32 last_score;


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

static /*un*/signed int sel_aaa;

static int plate_x;
static int plate_y;


/* ランキング表示位置 */
#define RANK_LOCATE_OFS_X_32		(32)		/*	 8 24 */
#define RANK_LOCATE_OFS_Y_40		(40)

#define KEYBORD_LOCATE_OFS_X_064	(64+16) 	/*	32 48  30 */
#define KEYBORD_LOCATE_OFS_Y_168	(168)		/* 160 84 100 */


/*---------------------------------------------------------
	ハイスコア名前入力、登録画面、動作(の子関数)[表示部分]
---------------------------------------------------------*/

static void name_entry_draw(void)
{
	psp_pop_screen();	/* 取って置いた背景で画面を描く */
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
			r.w 	= s.w	= (letter_surface[i]->w);
			r.h 	= s.h	= (letter_surface[i]->h);
			r.x = (letter[i].xpos);
			r.y = (letter[i].ypos);

			if (i == sel_aaa)/* 選択中文字==キーボード文字 */
			{
//				letter[i].scale256 = t256(1.6);
				{
					static int angle1024;		/*=0*/
					angle1024 += cv1024r((5));	/*fps_factor*/
					mask1024(angle1024);
					r.x 	+= ((sin1024((angle1024)))>>5);
					r.y 	+= ((sin1024((angle1024)))>>6);
				}
			}
//			else/* 選択中文字 != キーボード文字 */
//			{
//				letter[i].scale256 = t256(1.0);
//			}
			/* 描画 */
							SDL_SetAlpha(	letter_surface[i], SDL_SRCALPHA, /*bg_alpha_aaa*/(255));
			/*blit_scaled*/ SDL_BlitSurface(letter_surface[i],&s,sdl_screen[SDL_00_VIEW_SCREEN],&r);
		}
	}
	/* 決定文字を描画 */
/*	if (0 <= sel_aaa)*/
	{
		SDL_Surface *tmp_surface;
		tmp_surface 	= font_render( entry, FONT16R);
	//	s.x 	= 0;
	//	s.y 	= 0;
		r.w 	=	s.w 	= tmp_surface->w;
		r.h 	=	s.h 	= tmp_surface->h;
		r.x 	= plate_x;
		r.y 	= plate_y;
							SDL_SetAlpha(	tmp_surface, SDL_SRCALPHA, /*bg_alpha_aaa*/(255));
		/*blit_scaled*/ 	SDL_BlitSurface(tmp_surface, &s, sdl_screen[SDL_00_VIEW_SCREEN], &r);
		SDL_FreeSurface(tmp_surface);
	}
}


/*---------------------------------------------------------
	ハイスコア名前入力、登録画面、動作[キー入力＆状態変更部分]
---------------------------------------------------------*/

static void name_entry_local_work(void)
{
	{
		if (0 == my_pad_alter)/* さっき何も押されてなかった場合にキーチェック(原作準拠) */
		{
			/* 斜め移動はしない */
				 if (my_pad & PSP_KEY_LEFT )	{	sel_aaa--;				if (sel_aaa <  (0)) 				sel_aaa  = (KEYBOARD_M40-1);	}
			else if (my_pad & PSP_KEY_RIGHT)	{	sel_aaa++;				if (sel_aaa == (KEYBOARD_M40))		sel_aaa  = (0); 				}
			else if (my_pad & PSP_KEY_UP	 )	{	sel_aaa-=KEYBOARD_W10;	if (sel_aaa <  (0)) 				sel_aaa += (KEYBOARD_M40);		}
			else if (my_pad & PSP_KEY_DOWN )	{	sel_aaa+=KEYBOARD_W10;	if (sel_aaa >  (KEYBOARD_M40-1))	sel_aaa -= (KEYBOARD_M40);		}
			//
			if (my_pad & PSP_KEY_SHOT_OK) 	/* 入力決定 == (さっき)入力決定ボタンが押された。 */
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
			else if (my_pad & PSP_KEY_BOMB_CANCEL/*PSP_KEY_OPTION*/)	/* (さっき)キャンセルボタンが押された。 */
			{
			delete_last_character:
				if (0 < now_select_name_chr) /* 名前入力の入力文字がある場合で。  at first chr? */
				{
					now_select_name_chr--;				/* [削除キー](DEL) が入力されているので、その分戻る。 */
					entry[now_select_name_chr] = ' ';	/* 消す */
				}
			}
			else if (my_pad & PSP_KEY_PAUSE)		/* (さっき)終了(強制決定)ボタンが押された。 */
			{
			agree_entry:	/* 入力終了決定 */
				if (0 < now_select_name_chr)	// 名前入力の入力文字がある場合で。何か入力されている場合で。
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
					#endif
					main_call_func = title_menu_start;	/* タイトルメニューへ移動 */
					return;/* 描画するとハングアップするので、ここで強制リターン。 */
				}
			}
		}
		name_entry_draw();/*描画部分*/
	}
}

//static void name_entry_init(void)
global void name_entry_start(void)
{
	int i;
	int j;
	/* キーボード(の文字)の初期化 */
	{
		char tmp_str[2];/*64*/ /* 96 == 3 x [32] */
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
				"0123456789[]!?" /*.align*/" "/*.align*/" "
			};
			unsigned char c;
			c = str_aaa[i];
			tmp_str[0] = c;
			letter[i].ascii = c;
			letter_surface[i] = font_render( tmp_str, FONT16R); /* */
			i++;
		}
	}
	letter[i].ascii = MY_CODE_DEL_KEY;	letter_surface[i++] = font_render( (char*)/*"DEL"*/"/", FONT16R); /* 39 */
	letter[i].ascii = MY_CODE_OK_KEY;	letter_surface[i++] = font_render( (char*)/*"OK"*/"!",  FONT16R); /* 40 */
	{
		int k;
		k = 0;
		for (j=0; j<(KEYBOARD_H04*20);/*(4*36)*/ j+=(20))	 /*(36)*/ /*25*/
		{
			for (i=0; i<(KEYBOARD_W10*20);/*(10*30)*/ i+=(20))/*(30)*/ /*25*/
			{
				letter[k].xpos = (i) + (KEYBORD_LOCATE_OFS_X_064);
				letter[k].ypos = (j) + (KEYBORD_LOCATE_OFS_Y_168);
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
	high_score_table[select_player][my_rank].final_stage	= pd.player_now_stage;/**/
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

	/** 描画 **/

	/* PLAY RESULTが付いたから、画面おもいっきり消すことにした。 */
	{
		//psp_clear_screen();	/* 表画面を消す */
		load_SDL_bg(BG_TYPE_01_name_regist);/* 裏画面にロード */
		psp_pop_screen();/* 裏画面を表にコピー */
	}
	/* ｎ位タイトル描画 */
	{
		for (j=0; j<5; j++)
		{
			high_score_render_sub(select_player, j, ((my_rank==j)));/* FONT16R 白  FONT16W 紅(黄) */
		//
			SDL_Rect r;
			r.x = (RANK_LOCATE_OFS_X_32);
			r.y = (RANK_LOCATE_OFS_Y_40)+(j*20);
			r.w = result_surfaces[j]->w;
			r.h = result_surfaces[j]->h;
			SDL_SetAlpha(	result_surfaces[j], SDL_SRCALPHA, /*bg_alpha_aaa*/(255));
			SDL_BlitSurface(result_surfaces[j], NULL, sdl_screen[SDL_00_VIEW_SCREEN], &r);
			SDL_FreeSurface(result_surfaces[j]);
		}
	}
	#if 1
	/* 決定文字の位置 */
	plate_x = (RANK_LOCATE_OFS_X_32)+((16/*FONT16WW*/)*(2));
	plate_y = (RANK_LOCATE_OFS_Y_40)+(my_rank*20);
	#endif
	psp_push_screen();	/* 現在の表示画面を裏画面に保存 */
	sel_aaa = 0;
	main_call_func = name_entry_local_work;
}
