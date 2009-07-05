
/*---------------------------------------------------------

---------------------------------------------------------*/

#include "support.h"
#include "hiscore.h"/**/

/*---------------------------------------------------------
	ハイスコア表示デモ画面
---------------------------------------------------------*/

/*global*/
/*extern*/ SCORE_FORMAT high_score_table[MAX_8_SAVE_PLAYERS][MAX_5_RANKING/*5*/];

/*local static*/


typedef struct
{
	int amplifier256;		/* 横のズレ量の割合。 amplifier x offset */
	int phase256;			/* 横のズレ量。 phase x offset */
	int phaseout256;		/* 横のズレ量。 phase x offset */
//	int phaseout_speed256;
	int direction;						/* ズレる方向 */
	int move_done_flag; 				/* 移動完了したか、してないかの状態 */
//	定数
	int xg256;
	int yg256;
	int amplifier_speed256;
	int phase_speed256;
} MOVE_FONT_DATA;

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
	ハイスコア表示デモ画面(文字移動用)
---------------------------------------------------------*/

static void move_result(SPRITE *s)
{
	MOVE_FONT_DATA *d=(MOVE_FONT_DATA *)s->data;
	if (0==d->move_done_flag)	/* 移動は必要？ */
	{
		s->x256 = ((d->xg256) );
		s->y256 = ((d->yg256) );
		if (d->direction==0)	/* 移動する向き */
		{
			/* slide-in */
			d->amplifier256 -= d->amplifier_speed256;
			if (d->amplifier256 <= t256(0.3) )
			{
				d->move_done_flag=1;	/* 移動完了 */
				move_done_lines++;
			}
			else
			{
				d->phase256 += d->phase_speed256/**fps_fa ctor*/;
				s->x256 += ((d->phase256*d->amplifier256)>>8);
			}
		}
		else
		{
			d->phaseout256 += d->phase_speed256/**fps_fa ctor*/;
			s->x256 += ((d->phaseout256*d->amplifier256)>>8);
			if (1==more_show)
			{
				s->y256 -= ((d->phase256*d->amplifier256)>>8);		/* phase256 は流用 */
			}
			/* slide-out */
			d->amplifier256 += d->amplifier_speed256;
			if (
			//	(d->amplifier256 >= t256(100/*400*/) ) &&
			//	((s->x<-s->w)||(s->x>PSP_WIDTH480)) /* 画面外 */
				((s->x256<-((s->w128+s->w128)))||(s->x256>t256(PSP_WIDTH480)))||	/* 画面外 */
				(( (s->y256+t256(32)/*(s->h)*/) < 0))	/* 画面外 */
				)
			{
				d->move_done_flag=1;	/* 移動完了 */
				move_done_lines++;
			}
		}
	}
}

/*---------------------------------------------------------
	ハイスコア表示デモ画面
---------------------------------------------------------*/

static SPRITE *result_sprites[MAX_7_LINES/*6*/];
static SDL_Surface *result_surfaces[MAX_7_LINES/*6*/];

static void result_font_render(void)
{
	MOVE_FONT_DATA *hd;
	int i;
//
	static IMAGE_RESOURCE my_resource[1] =
	{
		{
			NULL,/*dummy*/
			0,/*dummy*/
			1,
			1,
			1,
			PRIORITY_01_SHOT,
			0, 0, 0
		}
	};
//
	for (i=0;i<MAX_7_LINES;i++)
	{
		if (i<5)
		{
			char ttmp[64/*50*/];
			sprintf(ttmp,"%1d %-3s %09d0",i+1,high_score_table[(show_player_num)][i].name,high_score_table[(show_player_num)][i].score);
		//	result_surfaces[i]		= font_render(ttmp,i==0?FONT05:FONT01);
		//	result_surfaces[i]		= font_render(ttmp,i==0?FONT03:FONT07);
			result_surfaces[i]		= font_render(ttmp, FONT03);
		}
		else
		if (5==i)
		{
			result_surfaces[5]		= font_render("RESULT OF SCORE"/*"TOP FIVE FIGHTERS"*/, FONT05);
		}
		else
		if (6==i)
		{
			/*const*/ char *score_name[5] =
			{
			/* 0==*/	"REIMU",
			/* 1==*/	"MARISA",
			/* 2==*/	"REMILIA",
			/* 3==*/	"CHIRNO",
			/* 4==*/	"YUYUKO",
			};
			result_surfaces[6]				= font_render((char *)score_name[(show_player_num)], FONT06);
		}
		result_sprites[i]					= sprite_add_res_list(
			result_surfaces[i],
		//	1,
		//	1,
		//	1,
		//	PRIORITY_01_SHOT,
			SP_FLAG_NOT_CACHE/*1*/,
		//	0
			(IMAGE_RESOURCE *)my_resource
			);
		hd									= mmalloc(sizeof(MOVE_FONT_DATA));
		result_sprites[i]->data 			= hd;
		hd->xg256							= result_const_status[RESULT_DATA_00_XG256][i];
		hd->yg256							= result_const_status[RESULT_DATA_01_YG256][i];
		hd->phase_speed256					= result_const_status[RESULT_DATA_02_PHASE_SPEED256][i];
		hd->amplifier_speed256				= t256(1.0);
		result_sprites[i]->flags			|= (SP_FLAG_VISIBLE);
		result_sprites[i]->type 			= SP_ETC;
		result_sprites[i]->x256 			= 0;					//	result_sprites[5]->x		= 0/*30*/;
		result_sprites[i]->y256 			= 0/*(i*25+110)*/;		//	result_sprites[5]->y		= 0/*50*/;
		result_sprites[i]->callback_mover	= move_result;
	}
	for (i=0;i<MAX_7_LINES/*5*/;i++)
	{
		hd=(MOVE_FONT_DATA *)result_sprites[i]->data;
		hd->phase256			= (i<<6);
		hd->phaseout256 		= ((4-i)<<6);
		hd->amplifier256		= t256(100/*300*/);
		hd->direction			= 0;
		hd->move_done_flag		= 0;
	}
//	hd=(MOVE_FONT_DATA *)result_sprites[5]->data;
//	hd->phase256			= (0);
//	hd->phaseout256 		= (5<<6);
//	hd->amplifier256		= t256(100/*300*/);
//	hd->direction			= 0;
//	hd->move_done_flag		= 0;
//
}
static void result_font_free(void)
{
	int i;
	for (i=0; i<MAX_7_LINES; i++)
	{
		result_sprites[i]->type=SP_DELETE;
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
		psp_pop_screen();	//SDL_BlitSurface(back,NULL,sdl_screen[SDL_00_SCREEN],NULL);
	}
	switch ((Uint8)(psp_loop&0xff)/*state.substate*/)
	{
	case RESULT_00_INIT:
		//void result_init(void)
		show_player_num=0;
		psp_push_screen();	//back=SDL_ConvertSurface(screen,screen->format,screen->flags);
		//if (NULL==back)
		//{
		//	CHECKPOINT;
		//	error(ERR_FATAL,"cant create background surface");
		//}
		psp_loop++;//newsta te(ST_RESULT,RESULT_01_SET_LOCATION,0);
		break;
	case RESULT_01_SET_LOCATION:
		{
			move_done_lines=0;
			result_font_render();
			psp_loop++;//newsta te(ST_RESULT,RESULT_02_SLIDE_IN,0);
		}
		break;
	case RESULT_02_SLIDE_IN:
		if (move_done_lines==MAX_7_LINES)
		{
			psp_loop++;//newsta te(ST_RESULT,RESULT_03_PAD_CHECK,0);
		}
		break;
	case RESULT_03_PAD_CHECK:
		if (0==my_pad)/* 今何も押されてなくて */
		{
			/* 次の人の戦歴を見る */
			if (my_pad_alter & (PSP_KEY_SHOT_OK|PSP_KEY_RIGHT|PSP_KEY_LEFT))	/* さっきPSP_KEY_SHOT_OKが押されてた */
			{
				show_player_num++;								/* 次の人にする */
				if (4<show_player_num) {show_player_num=0;} 	/* 最後まで見たら始めから見る */
				more_show=1;	/* また見るよ */
				psp_loop++;//newsta te(ST_RESULT,RESULT_04_SLIDE_OUT,0);
			}
			/* 見るのやめてメニューに戻る */
			else
			if (my_pad_alter & PSP_KEY_BOMB_CANCEL/*PSP_KEY_OPTION*/)		// キャンセルボタンの追加
			{
				more_show=0;	/* もうおしまい */
				psp_loop++;//newsta te(ST_RESULT,RESULT_04_SLIDE_OUT,0);
			}
		}
		break;
	case RESULT_04_SLIDE_OUT:
		{
			move_done_lines=0;
			int i;
			for (i=0;i<MAX_7_LINES;i++)
			{
				MOVE_FONT_DATA *hd;
				hd=(MOVE_FONT_DATA *)result_sprites[i]->data;
				hd->direction=1;
				hd->move_done_flag=0;
			}
			psp_loop++;//newsta te(ST_RESULT,RESULT_05_RETRY_CHECK,0);
		}
		break;
	case RESULT_05_RETRY_CHECK:
		if (move_done_lines==MAX_7_LINES)
		{
			result_font_free();
			if (1==more_show)	/* また見るよ */
			{
				psp_loop=(ST_WORK_RESULT|RESULT_01_SET_LOCATION);//newsta te(ST_RESULT,RESULT_01_SET_LOCATION,0);
			}
			else	/* if (0==more_show) もうおしまい */
			{
			//	SDL_FreeSurface(back);
			//	newsta te(ST_MENU,0,1);
				psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);//newsta te(ST_MENU/*ST_INTRO*/,0,1);
			}
		}
		break;
	}
	sprite_work000(SP_GROUP_ETC);
	sprite_display000(SP_GROUP_ETC);
}


/*---------------------------------------------------------
	ハイスコア
---------------------------------------------------------*/
extern int select_player;
extern int	now_max_continue;
int last_score;
void check_high_score(void)
{
	int my_flag;
	my_flag=0;

	if ( (/*3*/DEFAULT_MAX_CONTINUE-1) == now_max_continue )
	{
		if (last_score > high_score_table[select_player][4].score)
		{
			/* you made it! enter your name in the hiscore-list */
			my_flag=1;
			psp_loop=(ST_INIT_NAME_ENTRY|0);//newsta te(ST_NAME_ENTRY,0,1);
		}
	}

	if (1==my_flag)
	{
		/* you made it! enter your name in the hiscore-list */
		psp_loop=(ST_INIT_NAME_ENTRY|0);//newsta te(ST_NAME_ENTRY,0,1);
	}
	else
	{
		/* you'd better play barbie */
		psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);//newsta te(ST_MENU/*ST_INTRO*/,0,1);
	}

}

/*---------------------------------------------------------
	ハイスコア名前入力画面
---------------------------------------------------------*/

static char *entry;

#define MAX_40_LETTER	(40)
static SDL_Surface *letter_surface[MAX_40_LETTER];

typedef struct
{
	int xpos;
	int ypos;
	/*dou ble*/int scale256;
	char ascii;
} LETTER;
static LETTER letter[MAX_40_LETTER];

#define MY_CODE_DEL_KEY (-1)
#define MY_CODE_OK_KEY	(-2)

static int now_select_name_chr;
static int plate_x;
static int plate_y;

static int sel;

enum
{
	NAME_ENTRY_00_INIT = 0,
	NAME_ENTRY_01_SLIDE_IN,
	NAME_ENTRY_02_SLIDE_OUT,
	NAME_ENTRY_03_ENTRY,
	NAME_ENTRY_04_QUIT,
};

void name_entry_init(void)
{
	/*
	unsigned char c;
	for (c='A';c<='Z';c++) {	tmp_str[0] = c; 	letter[i].ascii= c; 	letter_surface[i++]=font_render(tmp_str,FONT02);	}	// 0-25
	for (c='0';c<='9';c++) {	tmp_str[0] = c; 	letter[i].ascii= c; 	letter_surface[i++]=font_render(tmp_str,FONT02);	}	// 26-36
													letter[i].ascii='.';	letter_surface[i++]=font_render(".",FONT02);			// 37
													letter[i].ascii='-';	letter_surface[i++]=font_render("-",FONT02);			// 38
													letter[i].ascii= -1;	letter_surface[i++]=font_render("DEL",FONT02);			// 39
													letter[i].ascii= -2;	letter_surface[i++]=font_render("OK",FONT02);			// 40
	for (i=0;i<40;i++) {
		SDL_SetColorKey(letter[i].surface,SDL_SRCCOLORKEY,0x00000000);
		letter[i].xpos=(i%10)*25+30;
		letter[i].ypos=(i/10)*25+100;
	}
	*/
	int i/*=0*/;
	int j;
	char tmp_str[/*128*/32/*100*/];
	tmp_str[1] = 0;
	for (i=0; i<(40-2); )
	{
		const char *str_aaa = /* 8*5==40==38(chars)+1(dummy)+1(0) */
		{ "ABCDEFG123" "HIJKLMN456" "OPQRSTU789" "VWXYZ.-0" /*.align*/" " };
		unsigned char c /*= 'A'*/;
		c = str_aaa[i];
		tmp_str[0] = c;
		letter[i].ascii = c; letter_surface[i] = font_render(tmp_str, FONT02); /* */
		i++;
	}
	letter[i].ascii= MY_CODE_DEL_KEY;	letter_surface[i++] = font_render("DEL",FONT02); /* 39 */
	letter[i].ascii= MY_CODE_OK_KEY;	letter_surface[i++] = font_render("OK", FONT02); /* 40 */
	{
		int k;
		k = 0;
		for (j=0; j<(4*36); j += (36)/*25*/)
		{
			for (i=0; i<(10*30); i += (30)/*25*/)
			{
				SDL_SetColorKey(letter_surface[k],SDL_SRCCOLORKEY,0x00000000);
				letter[k].xpos = (i) + 48/* 30*/;
				letter[k].ypos = (j) + 84/*100*/;
				k++;
			}
		}
	}
	for (i=0;i<40;i++)
	{
		letter[i].scale256=t256(0.0);
	}
	for (i=0;i<5;i++)
	{
		if (last_score > high_score_table[select_player][i].score)
		{	break;}
	}
	for (j=4;j>i;j--)
	{
		high_score_table[select_player][j] = high_score_table[select_player][j-1];
	}
	high_score_table[select_player][i].score = last_score;
	entry=high_score_table[select_player][i].name;
	entry[0] = ' ';
	entry[1] = ' ';
	entry[2] = ' ';
	entry[3] = 0;
	now_select_name_chr=0;
//	switch (i)
//	{
//	case 0: 	sp rintf(tmp_str,"NEW HISCORE");				break;
//	default:	sp rintf(tmp_str,"PLACE %d IN THE LIST",i+1);	break;
//	}
	char *aaaa[5] =
	{
		"1ST"/*"NEW HISCORE"*/,
		"2ND"/*"PLACE 2 IN THE LIST"*/,
		"3RD"/*"PLACE 3 IN THE LIST"*/,
		"4TH"/*"PLACE 4 IN THE LIST"*/,
		"5TH"/*"PLACE 5 IN THE LIST"*/,
	};
	/* ｎ位タイトル描画 */
	{	/*static*/ SDL_Surface *headline;
		headline=font_render(/*tmp_str*/(&aaaa[i][0]),FONT03);
		SDL_Rect r;
		r.x = (GAME_WIDTH/2)-((headline->w)>>1);
		r.y = 40;
		r.w = headline->w;
		r.h = headline->h;
		SDL_BlitSurface(headline,NULL,sdl_screen[SDL_00_SCREEN],&r);
		SDL_FreeSurface(headline);
	}
	/* 名前プレート描画 */
	{
		/*static*/ SDL_Surface *plate;
		plate = loadbmp0("panel/name_plate.png", 0, 0/*1*/);
		SDL_Rect r;
		plate_x = r.x = (GAME_WIDTH/2)-((plate->w)>>1);
		plate_y = r.y = (GAME_HEIGHT-12)-(plate->h);
		r.w = plate->w;
		r.h = plate->h;
		SDL_BlitSurface(plate,NULL,sdl_screen[SDL_00_SCREEN],&r);
		if (plate)
		{
			unloadbmp_by_surface(plate);/*unload_bmp_by_name("panel/name_plate.png");*/
		}
	}
	psp_push_screen();	//back=SDL_ConvertSurface(screen,screen->format,screen->flags);
	//if (NULL==back)
	//{
	//	CHECKPOINT;
	//	error(ERR_FATAL,"cant create background surface");
	//}
	sel=-1;
	psp_loop=(ST_WORK_NAME_ENTRY|NAME_ENTRY_01_SLIDE_IN);//newsta te(ST_NAME_ENTRY,H CLISTE_ENTRY,0);
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
	s.x=0;
	s.y=0;
	// 文字を描画
	{
		int i;
		for (i=0;i<40;i++)
		{
			if (i != sel)
			{
				s.w = letter_surface[i]->w;
				s.h = letter_surface[i]->h;
				r.x = letter[i].xpos-((letter_surface[i]->w*letter[i].scale256)>>(8+1));
				r.y = letter[i].ypos-((letter_surface[i]->h*letter[i].scale256)>>(8+1));
				r.w = ((s.w*letter[i].scale256)>>8);
				r.h = ((s.h*letter[i].scale256)>>8);
				blit_scaled(letter_surface[i],&s,sdl_screen[SDL_00_SCREEN],&r);
				if (sel >= 0)	{	if (letter[i].scale256 > t256(1))	{	letter[i].scale256 -= t256(0.05);}}
			}
			else
			{
				if (sel >= 0)	{	if (letter[i].scale256 <= t256(3))	{	letter[i].scale256 += t256(0.2);}}
			}
		}
	}
	//
	if (sel >= 0)
	{
		static /*dou ble*/int angle512/*=0*/;
		int xa;
	//	int ya;
		angle512 += deg_360_to_512(5/*15*/)/**fps_fa ctor*/;
		mask512(angle512);//	if (angle>360)	{	angle-=360;}
		xa		= ((/*cos512*/sin512((angle512)))>>(8-3));
	//	xa		= ((/*cos512*/sin512((angle512))*10)>>8);
	//	ya		= ((sin512((angle512))*10)>>8);
		int sw; sw	= (letter_surface[sel]->w); s.w 	= (/*letter_surface[sel]->w*/sw);
		int sh; sh	= (letter_surface[sel]->h); s.h 	= (/*letter_surface[sel]->h*/sh);
		r.w 	= ((/*s.w*/sw*(letter[sel].scale256))>>8);
		r.h 	= ((/*s.h*/sh*(letter[sel].scale256))>>8);
		r.x 	= (letter[sel].xpos)-(((/*letter[sel].surface->w*/sw)*(letter[sel].scale256))>>(8+1))+((sin512((angle512)))>>5/*xa*/);
		r.y 	= (letter[sel].ypos)-(((/*letter[sel].surface->h*/sh)*(letter[sel].scale256))>>(8+1))+((sin512((angle512)))>>6/*xa>>1*/)/*ya*/;
		blit_scaled(letter_surface[sel],&s,sdl_screen[SDL_00_SCREEN],&r);
	}
	{
		SDL_Surface *e;
		e		= font_render(entry,FONT02);
		s.x 	= 0;
		s.y 	= 0;
		s.w 	= e->w;
		s.h 	= e->h;
		r.x 	= plate_x + 5;
		r.y 	= plate_y + 5;
		r.w 	= e->w*2;
		r.h 	= e->h*2;
		blit_scaled(e,&s,sdl_screen[SDL_00_SCREEN],&r);
		SDL_FreeSurface(e);
	}
}

/*---------------------------------------------------------
	ハイスコア名前入力画面、動作[キー入力＆状態変更部分]
---------------------------------------------------------*/

void name_entry_work(void)
{
	int i;
	switch (psp_loop)
	{
	case (ST_WORK_NAME_ENTRY|NAME_ENTRY_01_SLIDE_IN):/*開始時(その１)にキーボードを拡大する*/
		for (i=0; i<40; i++)	{	letter[i].scale256 += t256(0.1);	}
		if (letter[0].scale256 >= t256(3)) {	psp_loop++; 			}
		break;
	case (ST_WORK_NAME_ENTRY|NAME_ENTRY_02_SLIDE_OUT):/*開始時(その２)にキーボードを縮小する*/
		for (i=0; i<40; i++)	{	letter[i].scale256 -= t256(0.1);	}
		if (letter[0].scale256 <= t256(1)) {	psp_loop++; sel = 0;	}
		break;
	case (ST_WORK_NAME_ENTRY|NAME_ENTRY_03_ENTRY):/* */
		if (0 == my_pad)/* 今何も押されてなくて */
		{
			/* 斜め移動はしない */
				 if (my_pad_alter & PSP_KEY_LEFT )	{		sel--;		if (sel<0)		sel=39; 	}
			else if (my_pad_alter & PSP_KEY_RIGHT)	{		sel++;		if (sel==40)	sel=0;		}
			else if (my_pad_alter & PSP_KEY_UP	 )	{		sel-=10;	if (sel<0)		sel+=40;	}
			else if (my_pad_alter & PSP_KEY_DOWN )	{		sel+=10;	if (sel>39) 	sel-=40;	}
			//
			if (my_pad_alter & PSP_KEY_SHOT_OK) 	/* 入力決定 == (さっき)入力決定ボタンが押されてた。 */
			{
				switch (letter[sel].ascii)
				{
				case MY_CODE_DEL_KEY: /* [削除キー](DEL)を決定した場合。 Delete last character */
					goto delete_last_character;
					break;
				case MY_CODE_OK_KEY: /* [入力終了キー](OK)を決定した場合。 Input completed. 入力終了。 Eingabe abgeschlossen. */
					goto agree_entry;
					break;
				default:
					if (now_select_name_chr < 3) /* 3文字以下決定した場合は文字入力。  3 chrs, name input entry. */
					{
						entry[now_select_name_chr] = letter[sel].ascii;
						now_select_name_chr++;
					}
					else /* 3文字以上の決定した場合は自動的に入力終了キーを押したことにする。 */
					{
						sel = 39;/* [入力終了キー](OK)を押したのと同じ。 force set [OK] */
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
					/* キーボードの文字を開放する */
					for (i=0; i<40; i++)
					{
						#if 0
					//	if (letter_surface[i])
						{
							/* ここでハングアップ */
							SDL_FreeSurface(letter_surface[i]);
						}
						#endif
					}
				//	SDL_FreeSurface(back);
					psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);//newsta te(ST_MENU/*ST_INTRO*/,0,1);
					return;
				}
			}
		}
		break;
//	case (ST_WORK_NAME_ENTRY|NAME_ENTRY_04_QUIT):/* */
//		break;
	}
	name_entry_draw();/*描画部分*/
}
