
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ゲームオーバーの表示
	-------------------------------------------------------
	現在都合により game_over.c 内にある。
	Gu化が進めば独立する予定。
---------------------------------------------------------*/

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

#include "kanji_system.h"

extern int select_player;

extern void dec_print_format( unsigned int num, int size, char *my_str);

/*---------------------------------------------------------
	ゲームコア終了の後処理
---------------------------------------------------------*/
extern int draw_script_screen;					/* せりふウィンドウ表示フラグ */

extern void bg2_destroy(void);
extern void gu_set_bg_u32_clear_color(u32 set_u32_clear_color);

extern u32 last_score;
//extern int last_stage;
global void gamecore_term(void)
{
	draw_script_screen = 0; /* せりふウィンドウ表示フラグ off */
//
	last_score = pd.game_score;
//
	gu_set_bg_u32_clear_color(0xff000000);	/*AABBGGRR*/	/*(黒)*/
	bg2_destroy();		// [***090126		追加
	//sprite_controller_remove_all();
	/*
		この辺でbossとかcoreとか開放しなくていいんだっけ？
	*/
	sprite_all_cleanup();
//	score_cleanup();
	//stop_music(); 	// [***090123		コメントアウト
	set_music_volume(128);
	play_music_num(BGM_23_menu01);
//	last_stage = 0;
//削除	名前入力で使う。	player_now_stage = 0;		// [***090702		追加
	pd.bomber_time = 0; 		// [***090903		追加
	/* 現在のversionはボスを倒さないで抜けるとGuがまずいので対策 */
	pd.state_flag &= (~(STATE_FLAG_13_DRAW_BOSS_GAUGE));	/* Guは書かないとマズイ */
	#if 1
	draw_boss_hp_value	= 0;/* よくわかんない */			/* Guは書かないとマズイ */
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
		if (3 > pd.bombs)	/* クリアー時にボムが３つ未満なら */
		{	pd.bombs = 3;	}	/* ３つに増やす */
		#endif
		#if 1/*模倣風*/
		if (8 > pd.bombs)	/* クリアー時にボムが９(8)つ未満なら */
		{	pd.bombs++; }	/* １つ増やす */
		#endif
		/* ボムがなくてもクリアーすればボムが４つになる */
	}
//
	#if 0
	/* なんか知らんが、Lunaticでちょっとチェックしたら、オーバーフローするｗ。 */
	/* クリアボーナス チェック */
	player_dummy_add_score(adjust_score_by_difficulty((
//		(player_now_stage * score( 1000)) + /* ステージ x  1000 pts */	/*原作風*/
//		(pd_weapon_power * score(	100)) + /* パワー	x	100 pts */	/*原作風*/
//		(pd_graze_point)					/* グレイズ x	 10 pts */	/*原作風*/
//
		(pd.player_now_stage * score(10000)) + /* ステージ x 10000 pts */	/*模倣風*/
		(pd.graze_point  * score( 1000)) + /* グレイズ x  1000 pts */	/*模倣風*/
		(pd.weapon_power * score(	100))	/* パワー	x	100 pts */	/*模倣風*/
	)));
	#else
	/* なんか知らんが、オーバーフローするので、個別に足してみる。 */
	/* クリアボーナス チェック */
	player_dummy_add_score(adjust_score_by_difficulty((	(((u32)pd.player_now_stage) * score(10000))	)));	/* ステージ x 10000 pts */	/*模倣風*/
	player_dummy_add_score(adjust_score_by_difficulty((	(((u32)pd.graze_point)	* score( 1000)) 	)));	/* グレイズ x  1000 pts */	/*模倣風*/
	player_dummy_add_score(adjust_score_by_difficulty((	(((u32)pd.weapon_power) * score(  100)) 	)));	/* パワー	x	100 pts */	/*模倣風*/
	#endif
//
	char buffer[32/*100*/];
	strcpy(buffer,	"RESULT" ); 																			font_print_screen_xy(buffer, FONT16R, 0,  32);
//	strcpy(buffer,	"STAGE   0 X 1000 PTS.");	dec_print_format( pd.player_now_stage, 1, (char *)&buffer[ 8]);	font_print_screen_xy(buffer, FONT16W, 8,  60);	/*原作風*/
//	strcpy(buffer,	"POWER 000 X  100 PTS.");	dec_print_format( pd.weapon_power,	3, (char *)&buffer[ 6]);	font_print_screen_xy(buffer, FONT16W, 8,  80);	/*原作風*/
//	strcpy(buffer,	"GRAZE 000 X   10 PTS.");	dec_print_format( pd.graze_point,	3, (char *)&buffer[ 6]);	font_print_screen_xy(buffer, FONT16W, 8, 100);	/*原作風*/
	//				"012345678901234567890
	strcpy(buffer,	"STAGE   0 X 10000PTS.");	dec_print_format( pd.player_now_stage, 1, (char *)&buffer[ 8]);	font_print_screen_xy(buffer, FONT16W, 8,  60);	/*模倣風*/
	strcpy(buffer,	"GRAZE 000 X  1000PTS.");	dec_print_format( pd.graze_point,	3, (char *)&buffer[ 6]);	font_print_screen_xy(buffer, FONT16W, 8,  80);	/*模倣風*/
	strcpy(buffer,	"POWER 000 X   100PTS.");	dec_print_format( pd.weapon_power,	3, (char *)&buffer[ 6]);	font_print_screen_xy(buffer, FONT16W, 8, 100);	/*模倣風*/
	pd.graze_point = 0;/* 清算して消える */
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
	font_print_screen_xy( (char *)level_name[(difficulty/*&0x03*/)], FONT16R, 0/*26*/, 160);
//
	#if 0/* ボス倒した場合の処理にいれた */
	pd_bomber_time = 0;/*都合上*/
	set_bg_alpha(255);/* 画面を明るくする */
	#endif
//	/* ステージクリア チェック */
	/* PRACTICE 開放 チェック */
	if ( (option_config[OPTION_CONFIG_07_OPEN] & (0x07)) < (pd.player_now_stage&0x07) )
	{
		/* PRACTICE 開放 (進んだステージを練習可能にする) */
		option_config[OPTION_CONFIG_07_OPEN] &= (~0x07);
		option_config[OPTION_CONFIG_07_OPEN] |= (pd.player_now_stage&0x07);
	}
}


/*---------------------------------------------------------
	GAME_OVER表示
---------------------------------------------------------*/

static void render_game_over_result(void/*int now_max_continue*/)
{
	pd.use_continue--;	/* (現プログラムの都合上)コンティニュー回数0で 1回カウントされるので、その分減らして辻褄あわせをする。 */
//
	char buffer[32/*100*/];
//	strcpy(buffer,	"GAME OVER" );																			font_print_screen_xy(buffer, FONT16R, 0,  32);
	strcpy(buffer,	"PLAYER DATA" );																		font_print_screen_xy(buffer, FONT16R, 0,  32);
	strcpy(buffer,	"SCORE     0000000000.");	dec_print_format( pd.game_score,		9, (char *)&buffer[10]);	font_print_screen_xy(buffer, FONT16W, 8,  60);
	strcpy(buffer,	"TOTAL MISTAKE      0.");	dec_print_format( pd.count_miss,		3, (char *)&buffer[17]);	font_print_screen_xy(buffer, FONT16W, 8,  80);	/* ミス回数 */
	strcpy(buffer,	"USE BOMBS          0.");	dec_print_format( pd.used_bomber,		3, (char *)&buffer[17]);	font_print_screen_xy(buffer, FONT16W, 8, 100);	/* ボム使用回数 */
	strcpy(buffer,	"BOMB WITH REVIVAL  0.");	dec_print_format( pd.use_kurai_bomb, 	3, (char *)&buffer[17]);	font_print_screen_xy(buffer, FONT16W, 8, 120);	/* 喰らいボム成功回数 */
	strcpy(buffer,	"USE CONTINUE       0.");	dec_print_format( pd.use_continue,		3, (char *)&buffer[17]);	font_print_screen_xy(buffer, FONT16W, 8, 140);	/* コンティニュー回数 */
//					"01234567890123456789"
	const char *level_name[4] =
	{
					"EASY   ",
					"NORMAL ",
					"HARD   ",
					"LUNATIC",
	};
	font_print_screen_xy( (char *)level_name[(difficulty/*&0x03*/)], FONT16R, 0/*26*/, 160);
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

static int result_time_out;

static void stage_clear_result_screen_local_work(void)
{
	if (0==my_pad_alter)/* さっき何も押されてなかった場合にキーチェック */
	{
		if (my_pad & (PSP_KEY_LEFT|PSP_KEY_RIGHT|PSP_KEY_UP|PSP_KEY_DOWN|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL|PSP_KEY_SLOW|PSP_KEY_OPTION))
		{
			result_time_out = (0);
		}
	}
	result_time_out--;
	if (0 > result_time_out)
	{
		msg_time = (60*5);	/* 約 5 秒 */
		print_kanji000(/*SDL_Rect *rect_srct*/ /*0,*/ /*text*/
			"CHALLENGE NEXT STAGE!" /*改行*/"\\n"
			"　　　　　　　　　　　　　少女祈祷中...", /*int color_type*/7, /*int wait*/0);
	//
		if (/*extra_stage*/(8)==pd.player_now_stage)/* エキストラモードの場合、終了する */
		{
		//	#if 1/* この２つのセットで自動的に終了(GAME OVER)する */
		//	now_max_continue = 1;	/* コンティニューさせない */
		//	player_loop_quit();
		//	#endif
			#if (0)
			psp_clear_screen();
			psp_push_screen();
			#endif
			pd.player_now_stage--;/* 7までしか無いので */
			main_call_func = gameover_start;
//			if (0x7f==can_player_bit)
//			{
//				can_player_bit = 0xff;	/* チルノ Q 開放 */
//			}
		}
		else		/* 通常時。(ファンタズムの場合もこちら、ファンタズムendへ) */
		{
			main_call_func = common_load_init;
		}
	}
}

global void stage_clear_result_screen_start(void)
{
	kanji_window_clear(); 	/* 漢字ウィンドウの内容を消す。 */
	home_cursor();			/* カーソルをホームポジションへ移動 */
	render_stage_clear_result();
	effect_dark_screen();
	result_time_out = (60*5);
	main_call_func = stage_clear_result_screen_local_work;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static int game_over_time_out;/*wait*/

static void gameover_local_work(void)
{
//	psp_pop_screen();
	if (0==my_pad_alter)/* さっき何も押されてなかった場合にキーチェック */
	{
		if (my_pad & (PSP_KEY_LEFT|PSP_KEY_RIGHT|PSP_KEY_UP|PSP_KEY_DOWN|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL|PSP_KEY_SLOW|PSP_KEY_OPTION))
		{
			game_over_time_out = (0);
		}
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
			main_call_func = name_entry_start;	/* 名前入力画面へ */
		}
		else	/* タイトル画面へ */
		{
			pd.player_now_stage = (0);/*要る？*/
			main_call_func = title_menu_start;	/* タイトルメニューへ移動 */
		}
	}
}

global void gameover_start(void)/* init */
{
	/* 初期化 */
	//void gameover_init(void)
	render_game_over_result();/* gamecore_term();より前の必要がある。 */
	gamecore_term();
	effect_dark_screen();
	game_over_time_out = (60*60);	/* 約 1 分 */
	main_call_func = gameover_local_work;
}
