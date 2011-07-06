
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�Q�[���I�[�o�[�̕\��
	-------------------------------------------------------
	���ݓs���ɂ�� game_over.c ���ɂ���B
	Gu�����i�߂ΓƗ�����\��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�ȉ�
	-------------------------------------------------------
	RESULT�\��(stage_clear.c)
	-------------------------------------------------------
	�Q�[���I�[�o�[�̕\��(game_over.c)
	-------------------------------------------------------
	���ݓs���ɂ�� score_panel.c ���ɂ���B
	Gu�����i�߂ΓƗ�����\��B
---------------------------------------------------------*/

#include "kanji_system.h"

extern int select_player;

extern void dec_print_format( unsigned int num, int size, char *my_str);

/*---------------------------------------------------------
	�Q�[���R�A�I���̌㏈��
---------------------------------------------------------*/
extern int draw_script_screen;					/* ����ӃE�B���h�E�\���t���O */

extern void bg2_destroy(void);
extern void gu_set_bg_u32_clear_color(u32 set_u32_clear_color);

extern u32 last_score;
//extern int last_stage;
global void gamecore_term(void)
{
	draw_script_screen = 0; /* ����ӃE�B���h�E�\���t���O off */
//
	last_score = pd.game_score;
//
	gu_set_bg_u32_clear_color(0xff000000);	/*AABBGGRR*/	/*(��)*/
	bg2_destroy();		// [***090126		�ǉ�
	//sprite_controller_remove_all();
	/*
		���̕ӂ�boss�Ƃ�core�Ƃ��J�����Ȃ��Ă����񂾂����H
	*/
	sprite_all_cleanup();
//	score_cleanup();
	//stop_music(); 	// [***090123		�R�����g�A�E�g
	set_music_volume(128);
	play_music_num(BGM_23_menu01);
//	last_stage = 0;
//�폜	���O���͂Ŏg���B	player_now_stage = 0;		// [***090702		�ǉ�
	pd.bomber_time = 0; 		// [***090903		�ǉ�
	/* ���݂�version�̓{�X��|���Ȃ��Ŕ������Gu���܂����̂ő΍� */
	pd.state_flag &= (~(STATE_FLAG_13_DRAW_BOSS_GAUGE));	/* Gu�͏����Ȃ��ƃ}�Y�C */
	#if 1
	draw_boss_hp_value	= 0;/* �悭�킩��Ȃ� */			/* Gu�͏����Ȃ��ƃ}�Y�C */
	#endif

}


/*---------------------------------------------------------
	RESULT�\��
---------------------------------------------------------*/

static void render_stage_clear_result(void/*int now_max_continue*/)
{
	/* �H�X�q ����\�́F�X�e�[�W�N���A���Ƀ{���������� */
	if (YUYUKO==select_player)	/* �H�X�q�̏ꍇ */
	{
		#if 1/*���앗*/
		if (3 > pd.bombs)	/* �N���A�[���Ƀ{�����R�����Ȃ� */
		{	pd.bombs = 3;	}	/* �R�ɑ��₷ */
		#endif
		#if 1/*�͕핗*/
		if (8 > pd.bombs)	/* �N���A�[���Ƀ{�����X(8)�����Ȃ� */
		{	pd.bombs++; }	/* �P���₷ */
		#endif
		/* �{�����Ȃ��Ă��N���A�[����΃{�����S�ɂȂ� */
	}
//
	#if 0
	/* �Ȃ񂩒m��񂪁ALunatic�ł�����ƃ`�F�b�N������A�I�[�o�[�t���[���邗�B */
	/* �N���A�{�[�i�X �`�F�b�N */
	player_dummy_add_score(adjust_score_by_difficulty((
//		(player_now_stage * score( 1000)) + /* �X�e�[�W x  1000 pts */	/*���앗*/
//		(pd_weapon_power * score(	100)) + /* �p���[	x	100 pts */	/*���앗*/
//		(pd_graze_point)					/* �O���C�Y x	 10 pts */	/*���앗*/
//
		(pd.player_now_stage * score(10000)) + /* �X�e�[�W x 10000 pts */	/*�͕핗*/
		(pd.graze_point  * score( 1000)) + /* �O���C�Y x  1000 pts */	/*�͕핗*/
		(pd.weapon_power * score(	100))	/* �p���[	x	100 pts */	/*�͕핗*/
	)));
	#else
	/* �Ȃ񂩒m��񂪁A�I�[�o�[�t���[����̂ŁA�ʂɑ����Ă݂�B */
	/* �N���A�{�[�i�X �`�F�b�N */
	player_dummy_add_score(adjust_score_by_difficulty((	(((u32)pd.player_now_stage) * score(10000))	)));	/* �X�e�[�W x 10000 pts */	/*�͕핗*/
	player_dummy_add_score(adjust_score_by_difficulty((	(((u32)pd.graze_point)	* score( 1000)) 	)));	/* �O���C�Y x  1000 pts */	/*�͕핗*/
	player_dummy_add_score(adjust_score_by_difficulty((	(((u32)pd.weapon_power) * score(  100)) 	)));	/* �p���[	x	100 pts */	/*�͕핗*/
	#endif
//
	char buffer[32/*100*/];
	strcpy(buffer,	"RESULT" ); 																			font_print_screen_xy(buffer, FONT16R, 0,  32);
//	strcpy(buffer,	"STAGE   0 X 1000 PTS.");	dec_print_format( pd.player_now_stage, 1, (char *)&buffer[ 8]);	font_print_screen_xy(buffer, FONT16W, 8,  60);	/*���앗*/
//	strcpy(buffer,	"POWER 000 X  100 PTS.");	dec_print_format( pd.weapon_power,	3, (char *)&buffer[ 6]);	font_print_screen_xy(buffer, FONT16W, 8,  80);	/*���앗*/
//	strcpy(buffer,	"GRAZE 000 X   10 PTS.");	dec_print_format( pd.graze_point,	3, (char *)&buffer[ 6]);	font_print_screen_xy(buffer, FONT16W, 8, 100);	/*���앗*/
	//				"012345678901234567890
	strcpy(buffer,	"STAGE   0 X 10000PTS.");	dec_print_format( pd.player_now_stage, 1, (char *)&buffer[ 8]);	font_print_screen_xy(buffer, FONT16W, 8,  60);	/*�͕핗*/
	strcpy(buffer,	"GRAZE 000 X  1000PTS.");	dec_print_format( pd.graze_point,	3, (char *)&buffer[ 6]);	font_print_screen_xy(buffer, FONT16W, 8,  80);	/*�͕핗*/
	strcpy(buffer,	"POWER 000 X   100PTS.");	dec_print_format( pd.weapon_power,	3, (char *)&buffer[ 6]);	font_print_screen_xy(buffer, FONT16W, 8, 100);	/*�͕핗*/
	pd.graze_point = 0;/* ���Z���ď����� */
	const char *level_name[4] =
	{
	//				"EASY      X 0.5",	/*���앗*/
	//				"NORMAL    X 1.0",	/*���앗*/
	//				"HARD      X 1.2",	/*���앗*/
	//				"LUNATIC   X 1.5",	/*���앗*/
					"EASY      X 0.5",	/*�͕핗*/
					"NORMAL    X 1.0",	/*�͕핗*/
					"HARD      X 2.0",	/*�͕핗*/
					"LUNATIC   X 5.0",	/*�͕핗*/
	};
	font_print_screen_xy( (char *)level_name[(difficulty/*&0x03*/)], FONT16R, 0/*26*/, 160);
//
	#if 0/* �{�X�|�����ꍇ�̏����ɂ��ꂽ */
	pd_bomber_time = 0;/*�s����*/
	set_bg_alpha(255);/* ��ʂ𖾂邭���� */
	#endif
//	/* �X�e�[�W�N���A �`�F�b�N */
	/* PRACTICE �J�� �`�F�b�N */
	if ( (option_config[OPTION_CONFIG_07_OPEN] & (0x07)) < (pd.player_now_stage&0x07) )
	{
		/* PRACTICE �J�� (�i�񂾃X�e�[�W����K�\�ɂ���) */
		option_config[OPTION_CONFIG_07_OPEN] &= (~0x07);
		option_config[OPTION_CONFIG_07_OPEN] |= (pd.player_now_stage&0x07);
	}
}


/*---------------------------------------------------------
	GAME_OVER�\��
---------------------------------------------------------*/

static void render_game_over_result(void/*int now_max_continue*/)
{
	pd.use_continue--;	/* (���v���O�����̓s����)�R���e�B�j���[��0�� 1��J�E���g�����̂ŁA���̕����炵�Ē��날�킹������B */
//
	char buffer[32/*100*/];
//	strcpy(buffer,	"GAME OVER" );																			font_print_screen_xy(buffer, FONT16R, 0,  32);
	strcpy(buffer,	"PLAYER DATA" );																		font_print_screen_xy(buffer, FONT16R, 0,  32);
	strcpy(buffer,	"SCORE     0000000000.");	dec_print_format( pd.game_score,		9, (char *)&buffer[10]);	font_print_screen_xy(buffer, FONT16W, 8,  60);
	strcpy(buffer,	"TOTAL MISTAKE      0.");	dec_print_format( pd.count_miss,		3, (char *)&buffer[17]);	font_print_screen_xy(buffer, FONT16W, 8,  80);	/* �~�X�� */
	strcpy(buffer,	"USE BOMBS          0.");	dec_print_format( pd.used_bomber,		3, (char *)&buffer[17]);	font_print_screen_xy(buffer, FONT16W, 8, 100);	/* �{���g�p�� */
	strcpy(buffer,	"BOMB WITH REVIVAL  0.");	dec_print_format( pd.use_kurai_bomb, 	3, (char *)&buffer[17]);	font_print_screen_xy(buffer, FONT16W, 8, 120);	/* ��炢�{�������� */
	strcpy(buffer,	"USE CONTINUE       0.");	dec_print_format( pd.use_continue,		3, (char *)&buffer[17]);	font_print_screen_xy(buffer, FONT16W, 8, 140);	/* �R���e�B�j���[�� */
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
	���[��AGu�����Ȃ̂ŁA�����Ƌ@�\���Ȃ��B
	(SDL��ʂ݂̂ɑ΂���G�t�F�N�g)
---------------------------------------------------------*/
static void effect_dark_screen(void)
{
#if 0
	#if 1
	/* KETM�݊��Ȃ� ������ back buffer screen �� clear screen ���ׂ� */
	psp_push_screen();
	#endif
	SDL_SetAlpha(sdl_screen[SDL_00_VIEW_SCREEN],SDL_SRCALPHA,128);
	psp_push_screen();
	SDL_SetAlpha(sdl_screen[SDL_00_VIEW_SCREEN],SDL_SRCALPHA,255);
#endif
}


/*---------------------------------------------------------
	RESULT�\��(�Q�[���e�ʃN���A�[��)
---------------------------------------------------------*/

extern void player_loop_quit(void);

static int result_time_out;

static void stage_clear_result_screen_local_work(void)
{
	if (0==my_pad_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N */
	{
		if (my_pad & (PSP_KEY_LEFT|PSP_KEY_RIGHT|PSP_KEY_UP|PSP_KEY_DOWN|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL|PSP_KEY_SLOW|PSP_KEY_OPTION))
		{
			result_time_out = (0);
		}
	}
	result_time_out--;
	if (0 > result_time_out)
	{
		msg_time = (60*5);	/* �� 5 �b */
		print_kanji000(/*SDL_Rect *rect_srct*/ /*0,*/ /*text*/
			"CHALLENGE NEXT STAGE!" /*���s*/"\\n"
			"�@�@�@�@�@�@�@�@�@�@�@�@�@�����F����...", /*int color_type*/7, /*int wait*/0);
	//
		if (/*extra_stage*/(8)==pd.player_now_stage)/* �G�L�X�g�����[�h�̏ꍇ�A�I������ */
		{
		//	#if 1/* ���̂Q�̃Z�b�g�Ŏ����I�ɏI��(GAME OVER)���� */
		//	now_max_continue = 1;	/* �R���e�B�j���[�����Ȃ� */
		//	player_loop_quit();
		//	#endif
			#if (0)
			psp_clear_screen();
			psp_push_screen();
			#endif
			pd.player_now_stage--;/* 7�܂ł��������̂� */
			main_call_func = gameover_start;
//			if (0x7f==can_player_bit)
//			{
//				can_player_bit = 0xff;	/* �`���m Q �J�� */
//			}
		}
		else		/* �ʏ펞�B(�t�@���^�Y���̏ꍇ��������A�t�@���^�Y��end��) */
		{
			main_call_func = common_load_init;
		}
	}
}

global void stage_clear_result_screen_start(void)
{
	kanji_window_clear(); 	/* �����E�B���h�E�̓��e�������B */
	home_cursor();			/* �J�[�\�����z�[���|�W�V�����ֈړ� */
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
	if (0==my_pad_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N */
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
			main_call_func = name_entry_start;	/* ���O���͉�ʂ� */
		}
		else	/* �^�C�g����ʂ� */
		{
			pd.player_now_stage = (0);/*�v��H*/
			main_call_func = title_menu_start;	/* �^�C�g�����j���[�ֈړ� */
		}
	}
}

global void gameover_start(void)/* init */
{
	/* ������ */
	//void gameover_init(void)
	render_game_over_result();/* gamecore_term();���O�̕K�v������B */
	gamecore_term();
	effect_dark_screen();
	game_over_time_out = (60*60);	/* �� 1 �� */
	main_call_func = gameover_local_work;
}
