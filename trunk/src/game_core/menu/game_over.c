
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
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

#include "kaiwa_sprite.h"

//extern void dec_print_format(unsigned int num, int size, char *my_str);



/*---------------------------------------------------------
	�Q�[���R�A�I���̌㏈��
---------------------------------------------------------*/

//extern void bg2_destroy(void);
extern void gu_set_bg_u32_clear_color(u32 set_u32_clear_color);
extern void do_kaiwa_system_terminate(void);
extern void kaiwa_obj_set_256(void);
global void gamecore_term(void)
{
	#if 1
	/*(��b���[�h�I������)*/
	do_kaiwa_system_terminate();
	#else
	/*(do_kaiwa_system_terminate();�Əd��)*/
	ml_font[(0)].haikei 		= (ML_HAIKEI_m1_OFF);/* ����Ӕw�ioff */
	#endif
	kaiwa_obj_set_256();
	kaiwa_all_obj_draw_on_off(0);	/* �����G��`�悵�Ȃ��B */
	//
	cg.bomber_time = (0);
	#if (1==USE_r36_SCENE_FLAG)
	/*(??????????????)*/
	/* off / �����R�}���h�ǉ��ǂݍ��ݏ������~����B */
//	cg.state_flag		&= (~SCENE_NUMBER_MASK); 	/*(�V�[��������)*/
	cg.state_flag		&= (0xffff00ffu); 	/*(�V�[��������)*/
	cg.state_flag		|= (0x00008000u); //�v���C���[���[�v�𔲂��鏈��(�Ƃ肠����??????)
	#else
	/* ���݂� version �̓{�X��|���Ȃ��Ŕ������ Gu ���܂����̂ő΍� */
	cg.state_flag &= (~(STATE_FLAG_15_DRAW_BOSS_GAUGE));	/* Gu�͏����Ȃ��ƃ}�Y�C */
	#endif
	#if (1)
	cg.draw_boss_hp_value	= (0);/* �悭�킩��Ȃ� */			/* Gu�͏����Ȃ��ƃ}�Y�C */
	#endif
//
	gu_set_bg_u32_clear_color(0xff000000);	/*AABBGGRR*/	/*(��)*/
//	bg2_destroy();
	//sprite_controller_remove_all();
	/*
		���̕ӂ� boss �Ƃ� core �Ƃ��J�����Ȃ��Ă����񂾂����H
	*/
	obj_system_cleanup_all();/* [A00�e�̈�]��[A01�Œ�G�̈�]��[A02�G�̈�]��[A03�Œ�̈�]��[A04�p�l���̈�]��OBJ��S�����B */
//	score_cleanup();
	set_music_volume(128);
}


/*---------------------------------------------------------
	RESULT�\��
---------------------------------------------------------*/

static void render_stage_clear_result(void)
{
	/* �H�X�q ����\�́F�X�e�[�W�N���A���Ƀ{���������� */
	if (YUYUKO==(cg_game_select_player))	/* �H�X�q�̏ꍇ */
	{
		#if 1/*���앗*/
		if (3 > cg.bombs)	/* �N���A�[���Ƀ{�����R�����Ȃ� */
		{	cg.bombs = 3;	}	/* �R�ɑ��₷ */
		#endif
		#if 1/*�͕핗*/
		if (8 > cg.bombs)	/* �N���A�[���Ƀ{�����X(8)�����Ȃ� */
		{	cg.bombs++; }	/* �P���₷ */
		#endif
		#if 0/*�͕핗*/
		/*(MAX�ɂ���)*/
		cg.bombs = (8); 	/* �X(8)�ɑ��₷ (9��==8) */
		#endif
		/* �{�����Ȃ��Ă��N���A�[����΃{�����S�ɂȂ� */
	}
//
	#if 0
	/* �Ȃ񂩒m��񂪁ALunatic�ł�����ƃ`�F�b�N������A�I�[�o�[�t���[���邗�B */
	/* �N���A�{�[�i�X �`�F�b�N */
	player_dummy_add_score(adjust_score_by_difficulty((
//		(cg.game_now_stage	* score( 1000)) + /* �X�e�[�W x 1000 pts */ 	/* ���앗 */
//		(cg.weapon_power	* score(  100)) + /* �p���[   x  100 pts */ 	/* ���앗 */
//		(cg.graze_point)					  /* �O���C�Y x   10 pts */ 	/* ���앗 */
//
		(cg.game_now_stage	* score(10000)) + /* �X�e�[�W x 10000 pts */	/* �͕핗 */
		(cg.graze_point 	* score( 1000)) + /* �O���C�Y x  1000 pts */	/* �͕핗 */
		(cg.weapon_power	* score(  100)) /* �p���[	  x   100 pts */	/* �͕핗 */
	)));
	#else
	/* �Ȃ񂩒m��񂪁A�I�[�o�[�t���[����̂ŁA�ʂɑ����Ă݂�B */
	/* �N���A�{�[�i�X �`�F�b�N */
	player_dummy_add_score(adjust_score_by_difficulty(( (((u32)cg.game_now_stage)	* score(10000)) 	)));	/* �X�e�[�W x 10000 pts */	/* �͕핗 */
	player_dummy_add_score(adjust_score_by_difficulty(( (((u32)cg.graze_point)		* score( 1000)) 	)));	/* �O���C�Y x  1000 pts */	/* �͕핗 */
	player_dummy_add_score(adjust_score_by_difficulty(( (((u32)cg.weapon_power) 	* score(  100)) 	)));	/* �p���[	x	100 pts */	/* �͕핗 */
	#endif
//
	{
		//					 "012345678901234567890"
		strcpy(my_font_text, "Result" );
		//
		ml_font[ML_LINE_01].x		= ( 40);
		ml_font[ML_LINE_01].y		= ( 32);
		ml_font[ML_LINE_01].timer	= ML_ON;
		kanji_window_clear_line(ML_LINE_01);	/* �����E�B���h�E��1�s��(==0)�̓��e�������B */
		set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (ML_LINE_01*18) ); /* �J�[�\����1�s��(==0)�ֈړ� */
		kanji_color((7)/*|STR_CODE_NO_ENTER*/);
		kanji_draw();
	}
//	strcpy(my_font_text, "Stage   0 x 1000 pts."); /* ���앗 */
//	strcpy(my_font_text, "Power 000 x  100 pts."); /* ���앗 */
//	strcpy(my_font_text, "Graze 000 x   10 pts."); /* ���앗 */
	/* �͕핗 */
	{
		//					 "012345678901234567890"
	//	strcpy(my_font_text, "Stage   0 x 10000 pts.");
		strcpy(my_font_text, "Stage   0 x 10000 pts.");
		dec_print_format( cg.game_now_stage, 1, (char *)&my_font_text[ 8]);
		//
		ml_font[ML_LINE_02].x		= ( 48);
		ml_font[ML_LINE_02].y		= ( 60);
		ml_font[ML_LINE_02].timer	= ML_ON;
		kanji_window_clear_line(ML_LINE_02);	/* �����E�B���h�E��1�s��(==0)�̓��e�������B */
		set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (ML_LINE_02*18) ); /* �J�[�\����1�s��(==0)�ֈړ� */
		kanji_color(9);
		kanji_draw();
	}
	{
		//					 "012345678901234567890"
	//	strcpy(my_font_text, "Graze0000 x  1000 pts.");
		strcpy(my_font_text, "Graze   0 x  1000 pts.");
		dec_print_format( cg.graze_point,	 4, (char *)&my_font_text[ 5]);
		//
		ml_font[ML_LINE_03].x		= (  48);
		ml_font[ML_LINE_03].y		= (  80);
		ml_font[ML_LINE_03].timer	= ML_ON;
		kanji_window_clear_line(ML_LINE_03);	/* �����E�B���h�E��1�s��(==0)�̓��e�������B */
		set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (ML_LINE_03*18) ); /* �J�[�\����1�s��(==0)�ֈړ� */
		kanji_color(9);
		kanji_draw();
	}
	{
		//					 "012345678901234567890"
	//	strcpy(my_font_text, "Power 000 x   100 pts.");
		strcpy(my_font_text, "Power   0 x   100 pts.");
		dec_print_format( cg.weapon_power,	 3, (char *)&my_font_text[ 6]);
		//
		ml_font[ML_LINE_04].x		= (  48);
		ml_font[ML_LINE_04].y		= ( 100);
		ml_font[ML_LINE_04].timer	= ML_ON;
		kanji_window_clear_line(ML_LINE_04);	/* �����E�B���h�E��1�s��(==0)�̓��e�������B */
		set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (ML_LINE_04*18) ); /* �J�[�\����1�s��(==0)�ֈړ� */
		kanji_color(9);
		kanji_draw();
	}
	cg.graze_point = 0;/* ���Z���ď����� */
	const char *level_name[4] =
	{
	//	"Easy      x 0.5",	/* ���앗 */
	//	"Normal    x 1.0",	/* ���앗 */
	//	"Hard      x 1.2",	/* ���앗 */
	//	"Lunatic   x 1.5",	/* ���앗 */
		"Easy      x 0.5",	/* �͕핗 */
		"Normal    x 1.0",	/* �͕핗 */
		"Hard      x 2.0",	/* �͕핗 */
		"Lunatic   x 5.0",	/* �͕핗 */
	};
	{
		strcpy(my_font_text, (char *)level_name[((cg.game_difficulty)/*&0x03*/)]);
		//
		ml_font[ML_LINE_05].x		= ( 40);
		ml_font[ML_LINE_05].y		= (160);
		ml_font[ML_LINE_05].timer	= ML_ON;
		kanji_window_clear_line(ML_LINE_05);	/* �����E�B���h�E��1�s��(==0)�̓��e�������B */
		set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (ML_LINE_05*18) ); /* �J�[�\����1�s��(==0)�ֈړ� */
		kanji_color(7);
		kanji_draw();
	}
//
	#if 0/* �{�X�|�����ꍇ�̏����ɂ��ꂽ */
	pd_bomber_time = 0;/* �s���� */
	set_bg_alpha(255);/* ��ʂ𖾂邭���� */
	#endif
//	/* �X�e�[�W�N���A �`�F�b�N */
	/* PRACTICE �J�� �`�F�b�N */
	if ((option_config[OPTION_CONFIG_07_OPEN] & (0x07)) < (cg.game_now_stage&0x07))
	{
		/* PRACTICE �J�� (�i�񂾃X�e�[�W����K�\�ɂ���) */
		option_config[OPTION_CONFIG_07_OPEN] &= (~0x07);
		option_config[OPTION_CONFIG_07_OPEN] |= (cg.game_now_stage&0x07);
	}
	cg.msg_time = (65536);	/* �� 18 �� */
}


/*---------------------------------------------------------
	GAME_OVER�\��
---------------------------------------------------------*/

typedef struct
{
	int xxx;
	int yyy;
	const char *sss1;
	int color1;
	const char *sss2;
	int color2;
} GMENU_TEST;

static GMENU_TEST result_my_obj[8] =
{	//																			"00000000001"
//																				"01234567890"
	{	32+(64),	( 32),	"���Ȃ��̘r�O", 		((15)|STR_CODE_NO_ENTER),	"", 			((7)|STR_CODE_NO_ENTER) },//"PLAYER DATA"/*"GAME OVER"*/
	{	32+(48),	( 60),	"�ŏI���_          ",	((6)|STR_CODE_NO_ENTER),	"0000000000.",	((7)|STR_CODE_NO_ENTER) },
	{	32+(48),	( 80),	"�~�X��          ",	((5)|STR_CODE_NO_ENTER),	"         0.",	((7)|STR_CODE_NO_ENTER) },	/* �W�v�V�X�e��(player_data)���~�X�� */
	{	32+(48),	(100),	"�{���g�p��      ",	((4)|STR_CODE_NO_ENTER),	"         0.",	((7)|STR_CODE_NO_ENTER) },	/* �W�v�V�X�e��(player_data)���{���g�p�� */
	{	32+(48),	(120),	"��炢�{��������",	((3)|STR_CODE_NO_ENTER),	"         0.",	((7)|STR_CODE_NO_ENTER) },	/* �W�v�V�X�e��(player_data)����炢�{�������� */
	{	32+(48),	(140),	"�R���e�B�j���[��",	((2)|STR_CODE_NO_ENTER),	"         0.",	((7)|STR_CODE_NO_ENTER) },	/* �W�v�V�X�e��(player_data)���R���e�B�j���[�� */
	{	32+(48),	(160),	"��Փx            ",	((1)|STR_CODE_NO_ENTER),	"   Lunatic.",	((7)|STR_CODE_NO_ENTER) },
};

static void render_game_over_result(void)
{
	const char *level_name[4] =
	{
		"      Easy.",
		"    Normal.",
		"      Hard.",
		"   Lunatic.",
	};
	cg.player_data_use_continue--;	/* �W�v�V�X�e��(player_data)_ (���v���O�����̓s����)�R���e�B�j���[��0�� 1��J�E���g�����̂ŁA���̕����炵�Ē��날�킹������B */
//
	unsigned int yyy18;
	yyy18 = 0;
	unsigned int i;
	for (i=0; i<7; i++)//ML_LINE_01 ... ML_LINE_07
	{
		ml_font[i].x		= result_my_obj[i].xxx;
		ml_font[i].y		= result_my_obj[i].yyy;
		ml_font[i].timer	= ML_ON;
		kanji_window_clear_line(i); /* �����E�B���h�E��1�s��(==0)�̓��e�������B */
		set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (yyy18) ); /* �J�[�\����1�s��(==0)�ֈړ� */
		//
		kanji_color((result_my_obj[i].color1));
		strcpy(my_font_text, &result_my_obj[i].sss1[0]	 );
		kanji_draw();
		//
		kanji_color((result_my_obj[i].color2));
		strcpy(my_font_text, &result_my_obj[i].sss2[0]	 );
		//
		void *aaa[(8)] =
		{
			&&bbb7, 		&&bbb1, 		&&bbb2, 		&&bbb3,
			&&bbb4, 		&&bbb5, 		&&bbb6, 		&&bbb7,
		};
		goto *aaa[(i)];
	bbb1:	dec_print_format( cg.game_score,					9, (char *)&my_font_text[0]);		goto bbb7;
	bbb2:	dec_print_format( cg.player_data_count_miss,		3, (char *)&my_font_text[7]);		goto bbb7;
	bbb3:	dec_print_format( cg.player_data_used_bomber,		3, (char *)&my_font_text[7]);		goto bbb7;
	bbb4:	dec_print_format( cg.player_data_use_kurai_bomb,	3, (char *)&my_font_text[7]);		goto bbb7;
	bbb5:	dec_print_format( cg.player_data_use_continue,		3, (char *)&my_font_text[7]);		goto bbb7;
	bbb6:	strcpy(my_font_text, (char *)level_name[((cg.game_difficulty)/*&0x03*/)]);				goto bbb7;
	bbb7:
		kanji_draw();
		yyy18 += (18);
	}
	cg.msg_time = (65536);	/* �� 18 �� */
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
/* �Ȃ��t�F�[�h�A�E�g���鎞�Ԃ͋����I�ɑ҂� */
#define MUSIC_SKIP_TIME 	byou60(5)	/* �� 5 �b(�{�^���Ŕ�΂��鎞��) */
#define MUSIC_FADE_OUT_TIME byou60(4)	/* �� 4 �b(��΂��Ȃ�����) */

static MAIN_CALL_FUNC(stage_clear_result_screen_local_work)
{
	result_time_out--;
	/* �{�^���Ŕ�΂��鎞�ԂȂ�L�[�`�F�b�N */
	if (MUSIC_FADE_OUT_TIME < result_time_out)
	{
		if (0==psp_pad.pad_data_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N */
		{
			if (psp_pad.pad_data & (PSP_KEY_LEFT|PSP_KEY_RIGHT|PSP_KEY_UP|PSP_KEY_DOWN|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL|PSP_KEY_SLOW|PSP_KEY_OPTION))
			{
				result_time_out = (MUSIC_FADE_OUT_TIME+1);
			}
		}
	}
	else
	/* ��΂��Ȃ����ԂɂȂ�����A�����F������\���B��(BGM)���t�F�[�h�A�E�g�J�n�B */
	if (MUSIC_FADE_OUT_TIME == result_time_out) 	/* */
	{
	//	cg.msg_time = byou60(5);	/* �� 5 �b */
		cg.msg_time = (65536);	/* �� 18 �� */
		{
		strcpy(my_font_text, "CHALLENGE NEXT STAGE!�@�@�����F����..." );
		//
		ml_font[ML_LINE_08].x		= (40);
		ml_font[ML_LINE_08].y		= (230);
		ml_font[ML_LINE_08].timer	= ML_ON;
		kanji_window_clear_line(ML_LINE_08);	/* �����E�B���h�E��1�s��(==0)�̓��e�������B */
		set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (ML_LINE_08*18) ); /* �J�[�\����1�s��(==0)�ֈړ� */
		kanji_color(7);
		kanji_draw();
		}
	}
	else
	if (0 == result_time_out)	/* �����܂� */
	{
		#if (1)/* �Ȃ̃t�F�[�h�A�E�g�������v��Ȃ��Ȃ�B */
		play_music_num(BGM_00_stop);
		set_music_volume(127/*SDL_MAXVOLUME*/);/* ���Ԃ� */
		#endif
		#if (1==USE_r36_SCENE_FLAG)
		/*(??????????????)*/
		if (cg.state_flag & 0x00008000u)//�v���C���[���[�v�𔲂��鏈��(�Ƃ肠����??????)
		/* ��b�V�X�e���ŏI���w�����ꂽ��A�I������ */
		#endif
		{
		//	#if 1/* ���̂Q�̃Z�b�g�Ŏ����I�ɏI��(GAME OVER)���� */
		//	cg_game_now_max_continue = 1;	/* �R���e�B�j���[�����Ȃ� */
		//	player_loop_quit();
		//	#endif
			#if (0)
			psp_clear_screen();
			psp_push_screen();
			#endif
			cg.game_now_stage--;/* 7�܂ł��������̂� */
			cb.main_call_func = gameover_start;
//			if (0x7f==can_player_bit)
//			{
//				can_player_bit = 0xff;	/* �`���m Q �J�� */
//			}
		}
		else		/* �ʏ펞�B(�t�@���^�Y���̏ꍇ��������A�t�@���^�Y��end��) */
		{
			cb.main_call_func = common_load_init;
		}
	}
	else
	{
		/* �Ȃ̃t�F�[�h�A�E�g���� */
		if (127 > result_time_out)/* SDL mixer�̎d�l(0-126�Ŏw��) */
		{
			set_music_volume(result_time_out);
		}
	}
}


/*---------------------------------------------------------
	RESULT�\��(�Q�[���e�ʃN���A�[��)�̏����J�n
---------------------------------------------------------*/

global MAIN_CALL_FUNC(stage_clear_result_screen_start)
{
	kanji_window_all_clear();				/* ������ʂ�S�s�����B�����J�[�\�����z�[���|�W�V�����ֈړ��B */
	render_stage_clear_result();
	effect_dark_screen();
	result_time_out 	= MUSIC_FADE_OUT_TIME + MUSIC_SKIP_TIME;	/* (�� 4 �b)+(�� 5 �b) */
	cb.main_call_func = stage_clear_result_screen_local_work;
}

/*---------------------------------------------------------
	�Q�[���I�[�o�[�̏ꍇ

---------------------------------------------------------*/

static int game_over_time_out;/*wait*/

static MAIN_CALL_FUNC(gameover_local_work)
{
//	psp_pop_screen();
	if (0==psp_pad.pad_data_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N */
	{
		if (psp_pad.pad_data & (PSP_KEY_LEFT|PSP_KEY_RIGHT|PSP_KEY_UP|PSP_KEY_DOWN|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL|PSP_KEY_SLOW|PSP_KEY_OPTION))
		{
			game_over_time_out = (0);
		}
	}
	game_over_time_out--;
	if (0 > game_over_time_out)
	{
		/*(r39)*/cg.side_panel_draw_flag = (0);/*(Gu�`��: �E�T�C�h�̃X�R�A�p�l���\�����\���ɂ���B�łȂ��Ɩ��O���͉�ʂ����������Ȃ�)*/
		if (
			#if (0==USE_CONTINUED_RANKING)
		//	( (/*3*/DEFAULT_MAX_CONTINUE-1) == cg.game_now_max_continue ) &&
			( 0 == cg.player_data_use_continue ) && 							/* �m�[�R���e�B�j���[�̏ꍇ�̂݃X�R�A�����L���O */
			#endif
			(cg.game_score > high_score_table[(cg_game_select_player)][4].score)
		)
		{
			/* �ŏI�X�R�A�������N�C�����Ă��ꍇ */
			cb.main_call_func = name_entry_start;	/* ���O���͉�ʂ� */
		}
		else	/* �^�C�g����ʂ� */
		{
			/* �ŏI�X�R�A�������N�C�����ĂȂ��ꍇ */
			cg.game_now_stage = (0);/*�v��H*/
			cb.main_call_func = title_menu_start;	/* �^�C�g�����j���[�ֈړ� */
		}
	}
}

/*---------------------------------------------------------
	�Q�[���I�[�o�[�̏����J�n
---------------------------------------------------------*/

global MAIN_CALL_FUNC(gameover_start)/* init */
{
	/* ������ */
	//void gameover_init(void)
	render_game_over_result();/* gamecore_term();���O�̕K�v������B */
	gamecore_term();
	effect_dark_screen();
	game_over_time_out = byou60(60);	/* �� 1 �� */
	cb.main_call_func = gameover_local_work;
}
