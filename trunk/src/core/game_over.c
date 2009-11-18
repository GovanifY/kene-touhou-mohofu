
/*---------------------------------------------------------
	ゲームオーバーの表示
---------------------------------------------------------*/

#include "game_main.h"
#include "player.h"

/*---------------------------------------------------------
	ゲームコア終了の後処理
---------------------------------------------------------*/
extern int draw_script_screen;					/* せりふウィンドウ表示フラグ */

extern void bg2_destroy(void);
extern int last_score;
void gamecore_term(void)
{
	draw_script_screen = 0; /* せりふウィンドウ表示フラグ off */
//
	last_score=((PLAYER_DATA *)player->data)->my_score;

	bg2_destroy();		// [***090126		追加
	//sprite_controller_remove_all();
	sprite_remove_all000(SP_GROUP_ALL_GAME_OBJS);
//	sprite_remove_all222(SP_GROUP_ALL_GAME_OBJS);/*弾幕用*/
//	parsys_remove_all();
//	score_cleanup();
//
//	//play_voice_auto_track(VO ICE04_SHIP_HAKAI);	// [***090123		コメントアウト
	//stop_music(); 	// [***090123		コメントアウト
	set_music_volume(128);
	play_music_num(BGM_09_menu1);
	player_now_stage = 0;		// [***090702		追加
	pd_bomber_time = 0; 		// [***090903		追加
	#if 1
	draw_boss_hp_value	= 0;/* よくわかんない */
	#endif
}

/*---------------------------------------------------------
	ゲームオーバー文字とスコア文字を拡大して表示する子関数
---------------------------------------------------------*/

static SDL_Surface *str_01_game_over_surface;
//static SDL_Surface *str_02_score_surface;

//static int go_size1_256;
//static int go_size2_256;

static void gameover_display(void)
{
	SDL_Rect s,d;
	s.x = 0;
	s.y = 0;
//
	s.w = (str_01_game_over_surface->w);
	s.h = (str_01_game_over_surface->h);
//	d.w = ((str_01_game_over_surface->w*go_size1_256)>>8);
//	d.h = ((str_01_game_over_surface->h*go_size1_256)>>8);
	d.w = ((str_01_game_over_surface->w));
	d.h = ((str_01_game_over_surface->h));
//	d.x = (GAME_WIDTH /2)	-((d.w)/2); 	// [***090203
//	d.y = (96);//98;//(GAME_HEIGHT/2)-30-((d.h)/2); 	// [***090203
	d.x = (GAME_WIDTH /2)	-((d.w)/2); 	// [***090203
	d.y = (128+8);//98;//(GAME_HEIGHT/2)-30-((d.h)/2); 	// [***090203
	/*blit_scaled*/SDL_BlitSurface(str_01_game_over_surface, &s, sdl_screen[SDL_00_SCREEN], &d);
//
	#if 0
	s.w = (str_02_score_surface->w);
	s.h = (str_02_score_surface->h);
//	d.w = ((str_02_score_surface->w*go_size2_256)>>8);
//	d.h = ((str_02_score_surface->h*go_size2_256)>>8);
	d.w = (str_02_score_surface->w);
	d.h = (str_02_score_surface->h);
//	d.x = (GAME_WIDTH /2)	-((d.w)/2); 	// [***090203
//	d.y = (160);//158;//(GAME_HEIGHT/2)+30-((d.h)/2);	// [***090203
	d.x = (128-48); 	// [***090203
	d.y = (48);//158;//(GAME_HEIGHT/2)+30-((d.h)/2);	// [***090203
	/*blit_scaled*/SDL_BlitSurface(str_02_score_surface, &s, sdl_screen[SDL_00_SCREEN], &d);
	#endif
}

/*---------------------------------------------------------

---------------------------------------------------------*/

//	enum /*_gameover_states_*/
//	{
//		GAME_OVER_00_INIT = 0,
//	//	GAME_OVER_01_ZOOM_GAME_OVER,
//	//	GAME_OVER_02_ZOOM_SCORE,
//		GAME_OVER_03_WAIT_QUIT,
//	};

extern void check_high_score(void);
void gameover_work(void)
{
//	SDL_Surface *tmpsurface;
//	SDL_Rect s,d;
	static int game_over_time_out;/*wait*/
//
//	if ( (ST_WORK_GAME_OVER|GAME_OVER_00_INIT) == (psp_loop) )
	if ( (0) == (psp_loop&0xff) )
	{
		//void gameover_init(void)
		gamecore_term();
	//
		str_01_game_over_surface	= font_render("GAME OVER",FONT03/*FONT06*/ /*FONT05*/);
		#if 0
		{
			char scoretext[32/*50*/];
			sprintf(scoretext,"SCORE: %d0",last_score);
			str_02_score_surface	= font_render(scoretext,FONT03/*FONT06*/ /*FONT05*/);
		}
		#endif
		#if 1
		/* KETM互換なら ここで back buffer screen を clear screen すべき */
		psp_push_screen();
		#endif
		SDL_SetAlpha(sdl_screen[SDL_00_SCREEN],SDL_SRCALPHA,128);
		psp_push_screen();	//SDL_BlitSurface(sdl_screen[SDL_00_SCREEN], NULL, back_screen, NULL);
		SDL_SetAlpha(sdl_screen[SDL_00_SCREEN],SDL_SRCALPHA,255);
//		go_size1_256		= 0;
//		go_size2_256		= 0;
//		game_over_time_out	= 0;/*wait*/
//		psp_loop++;//psp_loop=(ST_WORK_GAME_OVER|GAME_OVER_01_ZOOM_GAME_OVER);//newsta te(ST_GAME_OVER,GAME_OVER_01_ZOOM_GAME_OVER,0);
//		break;
//
//	case GAME_OVER_01_ZOOM_GAME_OVER:
//		gameover_display();
//		go_size1_256 += t256(0.05);
//		if (go_size1_256 >= t256(1.5))
//		{
//			psp_loop++;//newsta te(ST_GAME_OVER,GAME_OVER_02_ZOOM_SCORE,0);
//		}
//		break;
//	case GAME_OVER_02_ZOOM_SCORE:
//		gameover_display();
//		go_size2_256 += t256(0.05);
//		if (go_size2_256 >= t256(1.0))
//		{
			game_over_time_out = 200;
			psp_loop++;//newsta te(ST_GAME_OVER,GAME_OVER_03_WAIT_QUIT,0);
//		}
	}
	else
	{
		psp_pop_screen();	//SDL_BlitSurface(back_screen, NULL, sdl_screen[SDL_00_SCREEN], NULL);
		gameover_display();
		game_over_time_out--;
		if (1 > game_over_time_out)
		{
			//SDL_FreeSurface(str_01_game_over_surface);
			//SDL_FreeSurface(str_02_score_surface);
			//SDL_FreeSurface(back_screen);
			check_high_score();/* ←内部で(psp_loopで)状態変更する */
			//psp_loop++;//newsta te(ST_GAME_OVER,GAME_OVER_05_QUIT,0);
		}
	}
//	switch ((Uint8)(psp_loop&0xff)/*state.substate*/)
//	{
//	case GAME_OVER_00_INIT: 			break;
//	case GAME_OVER_03_WAIT_QUIT:		break;
//	}
}
