
/*---------------------------------------------------------

---------------------------------------------------------*/

#include "game_main.h"
//#include "bonus.h"

#include "bullet_object.h"
#include "load_stage.h"
#include "scenario_script.h"

extern STAGE_DATA *stage_data_table;

extern int select_player;
extern int practice_mode;

/*---------------------------------------------------------
	�U�R�Ő錾���K�v�Ȃ���(�O���[�o��)
---------------------------------------------------------*/

	/* ���̑�(����@�\) */
extern void add_clouds( 				STAGE_DATA *l);/* �ǂ̓G�ł��Ȃ��ꍇ�́A���o�p�摜 */
extern void add_enemy_all_clear(		STAGE_DATA *l);/* �S�ʃN���A�[�̏ꍇ�A���̓G��ǉ� */
extern void bg2_control(				STAGE_DATA *l);

	/* �{�X */
extern void add_boss_kene(				STAGE_DATA *l);/* extra 2�� */
extern void add_boss_cirno( 			STAGE_DATA *l);/* extra 1�� */
//
extern void add_boss_sakuya(			STAGE_DATA *l);/* 6�ʂ͍�� */
extern void add_boss_pache( 			STAGE_DATA *l);/* 5�ʂ̓p�`�����[ */
extern void add_boss_kaguya(			STAGE_DATA *l);/* 4�ʂ͋P�� */
extern void add_boss_mitei( 			STAGE_DATA *l);/* 3�ʂ͂ǂ����邩���� */
extern void add_boss_aya(				STAGE_DATA *l);/* 2�ʂ͕� */
extern void add_boss_alice( 			STAGE_DATA *l);/* 1�ʂ̓A���X */
	/* [���^�G]�d�� */
extern void add_chuu_koakuma(			STAGE_DATA *l);
extern void add_chuu_rumia( 			STAGE_DATA *l);
extern void add_chuu_youkai1(			STAGE_DATA *l);
extern void add_chuu_youkai2(			STAGE_DATA *l);
	/* �����w */
extern void add_enemy_mahoujin( 		STAGE_DATA *l);

	/* ���� �A�z�� */
extern void add_zako_inyou1(			STAGE_DATA *l);
extern void add_zako_tatsumaki1(		STAGE_DATA *l);
	/* �d�� */
extern void add_zako_kakomi1(			STAGE_DATA *l);
	/* ���̑��U�R */
extern void add_zako_obake1(			STAGE_DATA *l);
extern void add_zako_obake2(			STAGE_DATA *l);
extern void add_zako_yukari1(			STAGE_DATA *l);
extern void add_zako_yukari2(			STAGE_DATA *l);
	/* �ы� */
extern void add_zako_aka_kedama1(		STAGE_DATA *l);
extern void add_zako_midori_kedama1(	STAGE_DATA *l);
extern void add_zako_midori_kedama2(	STAGE_DATA *l);
extern void add_zako_kedama1(			STAGE_DATA *l);
extern void add_zako_kedama2(			STAGE_DATA *l);
	/* ���d�� */
extern void add_zako_meido1(			STAGE_DATA *l);
extern void add_zako_meido2(			STAGE_DATA *l);
extern void add_zako_meido3(			STAGE_DATA *l);
extern void add_zako_meido4(			STAGE_DATA *l);
	/* ���d�� */
extern void add_zako_ao_yousei1(		STAGE_DATA *l);
extern void add_zako_ao_yousei2(		STAGE_DATA *l);
extern void add_zako_ao_yousei3(		STAGE_DATA *l);
extern void add_zako_ao_yousei4(		STAGE_DATA *l);
//

extern void add_enemy_load_bg(			STAGE_DATA *l);
extern void add_enemy_kanji_string( 	STAGE_DATA *l);

/* ���̕����͏��������������ɂ���(Gu�������)�̂œs���ɂ��p�~ */
//extern void add_enemy_load_picture(	STAGE_DATA *l);


/*---------------------------------------------------------

---------------------------------------------------------*/

int difficulty = RANK_EASY/*  RANK_NORMAL*/;

//static Uint32 stage_start_time;
//static Uint32 game_start_time;

/*---------------------------------------------------------
	�G�̒ǉ�
	-------------------------------------------------------
	�����ɒǋL����ƃV���[�e�B���O�Q�[���{�̂��x���Ȃ�̂ŒǋL���܂���B
	������(parth)���� load_stage.c �ŗ\�ߍς܂��Ă����܂��B
---------------------------------------------------------*/

static void add_all_teki(STAGE_DATA *l)
{
	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
	/* enemy�̐�����ԍ��ŊǗ�(load_stage.c �� ctype_name[]�ɑΉ����Ă���) */
	void (*aaa[ETYPE_MAX])(STAGE_DATA *l) =
	{
		add_clouds,/* [�ԕ���؂�] */	/*NULL*/	/* add_clouds();�����Ō�����Ȃ��ꍇ�́Aenemy_error(); */
	/* ���̑� */
		add_enemy_all_clear,		/* �Q�[�� �S�X�e�[�W �N���A�[ */
		bg2_control,/*CTYPE_02_BG_CONTROL*/
	/* �{�X */
		add_boss_mitei, 			/*add_boss_kene*/		/* extra 2�� */
		add_boss_pache, 			/*add_boss_cirno*/		/* extra 1�� */
		add_boss_sakuya,			/* 6�ʂ͍�� */ 		// [***090207		�ǉ�
		add_boss_pache, 			/* 5�ʂ̓p�`�����[ */	//		�ǉ�
		add_boss_kaguya,			/* 4�ʂ͋P�� */
		add_boss_mitei, 			/* 3�ʂ͂ǂ����邩����(�i��? �d��?) */
		add_boss_aya,				/* 2�ʂ͕� */
		add_boss_alice, 			/* 1�ʂ̓A���X */
	/* ����G[���^�G] */
		add_chuu_rumia, 			/*	="���[�~�A" */		/*	6�ʂ�? */
		add_chuu_rumia, 			/*add_chuu_koakuma*/	/*	5�ʂ�="������" */
		add_chuu_rumia, 			/*add_chuu_udonge*/ 	/*	4�ʂ͗��? */
		add_chuu_rumia, 			/*add_chuu_tei*/		/*	3�ʂ͂Ă�? */
		add_chuu_rumia, 			/*	="���[�~�A" */		/*	2�ʂ͓V��? */
		add_chuu_rumia, 			/*	="���[�~�A" */		/*	1�ʂ̓��[�~�A */
//		�p�~?
		add_chuu_youkai2,			/*	="�d��2"	*/		// [***090207	�ǉ�
		add_chuu_youkai1,			/*	="�d��1"	*/		// [***090207	�ǉ�
	/* �����w */
		add_enemy_mahoujin, 		/*	""	*/
		add_enemy_mahoujin, 		/*	""	*/
	/* ���� �A�z�� */
		add_zako_inyou1,			/*	-"�A�z��1"	*/
		add_zako_tatsumaki1,		/*	C"����1"	*/		/*enemy_error*/
	/* �d�� */
		add_zako_kakomi1,			/*	-"�͗d��1"	*/
	/* ���̑��U�R */
		add_zako_obake1,			/*	-"���΂�1"	*/
		add_zako_obake2,			/*	-"���΂�2"	C"���ы�1"	*/		/*enemy_error*/
		add_zako_yukari1,			/*	C"���ґ�1"	*/		/*enemy_error*/
		add_zako_yukari2,			/*	C"���ґ�2"	*/		/*enemy_error*/
	/* �ы� */
		add_zako_aka_kedama1,		/*	a"�Ԗы�1"	*/
		add_zako_midori_kedama1,	/*	-"�Ζы�1"	*/
		add_zako_midori_kedama2,	/*	C"�Ζы�2"	*/		/*enemy_error*/
		add_zako_kedama1,			/*	-"�ы�1"	*/
		add_zako_kedama2,			/*	-"�ы�2"	*/
	/* ���d�� */
		add_zako_meido1,			/*	-"���C�h1"	*/
		add_zako_meido2,			/*	-"���C�h2"	*/
		add_zako_meido3,			/*	C"���C�h3"	*/		/*enemy_error*/
		add_zako_meido4,			/*	-"���C�h4"	*/
	/* ���d�� */
		add_zako_ao_yousei1,		/*	="�d��1"	*/		// [***090207	�ǉ�
		add_zako_ao_yousei2,		/*	="�d��2"	*/		// [***090124	�ǉ�
		add_zako_ao_yousei3,		/*	="�d��3"	*/		//	�ǉ�
		add_zako_ao_yousei4,		/*	="�d��4"	*/		//	�ǉ�
//
//
		add_enemy_kanji_string, 	/* ETYPE_01_SJIS_TEXT */
		add_enemy_load_bg,			/* ETYPE_02_LOAD_BG */
	//	add_enemy_load_picture, 	/* ETYPE_03_PICTURE */		/* ���̕����͏��������������ɂ���(Gu�������)�̂œs���ɂ��p�~ */
	};
	(*aaa[ (int)(l->user_i_code) ])(l); 	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
}


/*---------------------------------------------------------

---------------------------------------------------------*/
static Uint32 game_v_time;
static void init_stage_start_time(void)
{
//	stage_start_time = psp_get_uint32_ticks();
	game_v_time = 0;
}


/*---------------------------------------------------------
	�V���[�e�B���O�Q�[���{�̂̏�����
---------------------------------------------------------*/
extern int continue_stage;

extern void set_rnd_seed(int set_seed);
extern /*int*/void load_stage(void/*int level*/);
extern void player_init(void);
extern void score_panel_init(void);
void common_load_init(void)
{
	set_rnd_seed(player_now_stage); 	/* �����n��̏����� */
//
	/* Load next stage */
	load_stage();//if (0==load_stage(/*level*/))	{	error(ERR_WARN, "no entrys for level %d",level);}
	// ���[�h���͏����������Ă���̂ŁA���[�h��Ɏ��Ԃ��č쐬����B
	init_stage_start_time();/*stage_start_time = psp_get_uint32_ticks();*/
//	game_start_time = psp_get_uint32_ticks();

//	stage_start_time = psp_get_uint32_ticks();
//	game_v_time = (psp_get_uint32_ticks()-stage_start_time);
//	game_v_time = (psp_get_uint32_ticks()-stage_start_time);
	//	play_music_num(BGM_01_stage1);	�R�����g�A�E�g
//
	script_message_window_clear();/*�X�N���v�g���b�Z�[�W��ʂ�����*/
	psp_loop = (ST_WORK_GAME_PLAY|0);
}
//	int stage;	stage=((PLAYER_DATA *)player->data)->stage;


/*---------------------------------------------------------
	�Q�[���R�A����(�Q�[���J�n��)����̏�����
---------------------------------------------------------*/

extern void player_result_init(void);
//void shooting_game_core_1st_init(void)
void stage_first_init(void)
{
	#if (000)/* ??? */
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE01_MENU_OK);
		#else
		voice_play(VOICE01_MENU_OK, TRACK01_EXPLODE);
		#endif
	#endif
//
	score_panel_init();
	player_result_init();
	//sprite_controller_remove_all();
	player_init();
//
	script_message_window_clear();
//
//	#if (1 == US E_ENDING_DEBUG)
//	/*player_init();����̕K�v*/
//	if (MA X_STAGE6_FOR_CHECK == continue_stage)
//	{	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
//	//	pd->bo ssmode	= B07_AF TER_LOAD;
//		pd->state_flag	|= (STATE_FLAG_10_IS_LOAD_SCRIPT|STATE_FLAG_05_IS_BOSS|ST ATE_FLAG_11_IS_BOSS_DESTROY);
//		continue_stage--;
//		practice_mode = 0;
//	}
//	#endif //(1==US E_ENDING_DEBUG)
	player_now_stage/*data->now_stage*/ /*level*/	= continue_stage/*+1-1*/ /*1*/;
//
	psp_loop = (ST_INIT_GAME_PLAY_common|0);
//	common_load_init();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

void incliment_scene(void)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	{
		/*�{�X�퓬��C�x���g*/
	//	if (B09_STAGE_LOAD==pd->bo ssmode) // 9:stage�ǂݍ���
		if (/*STATE_FLAG_05_IS_BOSS == */(pd->state_flag & STATE_FLAG_05_IS_BOSS))
		{
			psp_loop = (ST_WORK_STAGE_CLEAR|0);
		}
		/*�{�X�퓬�O�C�x���g*/
		else
	//	if (B08_START == pd->bo ssmode) // 8:�{�X�Ȃ�炵�A1�{�X�Ƃ̐퓬�ցB
		{
			if (0!=boss_bgm_mode)
			{	/* �{�X�C�x���g��Ƀ{�X�� */
				set_music_volume(128);/* �ȉ��ʖ߂� */
				play_music_num( (BGM_10_boss1-1)+player_now_stage );/* �{�X�Ȗ炷 */
			}
		//	else
		//	{	/* �{�X�C�x���g�O�Ƀ{�X�� */
		//	}
			pd->state_flag |= (STATE_FLAG_05_IS_BOSS|STATE_FLAG_13_DRAW_BOSS_GAUGE);
		}
	}
}
//
extern void script_load(void);
/* ���ӁFstatic�֐��ɂ��Ȃ� */void my_special(void)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
//	if (pd->bo ssmode==B05_BEFORE_LOAD) 	// [***090313	�ǉ�
//	if (pd->bo ssmode==B07_AFTER_LOAD)		// [***090313	�ǉ�
	if (pd->state_flag & (STATE_FLAG_10_IS_LOAD_SCRIPT))		// [***090313	�ǉ�
	{
		pd->state_flag &= (~(STATE_FLAG_10_IS_LOAD_SCRIPT));/*off*/
		script_load(/*0 1*/);
	}
//	if (pd->state_flag & (ST ATE_FLAG_11_IS_BOSS_DESTROY))
//	{
//		pd->state_flag &= (~(ST ATE_FLAG_11_IS_BOSS_DESTROY));/*off*/
//		boss_destroy_aaa();
//	}
	/* �X�N���v�g���I������H */
	if (pd->state_flag & (STATE_FLAG_12_END_SCRIPT))
	{
		pd->state_flag &= (~(STATE_FLAG_12_END_SCRIPT));/*off*/ 	/*	pd->bo ssmode=B00_NONE;*/
	//	pd->state_flag &= (~(STATE_FLAG_12_END_SCRIPT));/*off*/ 	/*	pd->bo ssmode=B00_NONE;*/	/*B01_BA TTLE*/
		incliment_scene();
	}
}

/*---------------------------------------------------------
	�V���[�e�B���O�Q�[���{�̂̃��C�����[�`��
	-------------------------------------------------------
	�����ɒǋL����΂���قǁA�V���[�e�B���O�Q�[���{�̂�
	�x���Ȃ�̂ŁA���ӂ��ĒǋL���Ă���B
	�߂����Ɏ��s���Ȃ����͊֐������ĊO�ɒǂ��o�����B
---------------------------------------------------------*/

extern void vbl_draw_screen(void);/*support.c*/

extern void script_display(void);
extern void score_display(void);
extern void bg_work_draw(void);
extern void draw_score_chache(void);
void shooting_game_core_work(void)
{
	while ((ST_WORK_GAME_PLAY>>8) == (psp_loop>>8) )
	{
	//	if (psp_loop != (ST_WORK_GAME_PLAY&0xff00) /*|| state.newsta te==1*/) return;
		/* game_v_time=Zeit seit Spielbeginn in 1/10 sec. */
		game_v_time++;//=(psp_get_uint32_ticks()-stage_start_time);
	//
		PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
		if (pd->state_flag & STATE_FLAG_14_GAME_LOOP_QUIT)
		{
			;	/* GAMEOUT�� */
		}
		else
		{
			/* �����Ă� */
		//	if (0==my_pad)
			if (0==(my_pad & PSP_KEY_PAUSE))
			{
				if (my_pad_alter & PSP_KEY_PAUSE)
				{
				//	if (0==(pd->state_flag & STATE_FLAG_06_IS_SCRIPT))/*���܂ɂ��܂������Ȃ���������*/
					{
						psp_loop=(ST_INIT_MENU|ST_MENU_SUB_PAUSE);//newsta te(ST_MENU,ST_MENU_SUB_PAUSE,1);
					}
				}
			}
			#if 1
			{
				STAGE_DATA *l;
				/*
				This routine, serch back to begin.
				���̃��[�`���͋t���Ɍ������܂��B
				*/
				l = stage_data_table;
				while (NULL != l)	/* �G���X�g�̏I���(NULL)�܂Œ��ׂ� */	/* [head ==NULL] then end. */
				{
					/* �����ς݁H */
					if (0 < l->v_time )//if (0 == l->done ) 	/* enemy set done flag */
					{
					//	#if 1
						if (game_v_time >= (l->v_time)) 	/* (���ݎ��� >= �ݒ莞��) �Ȃ�A�G���Z�b�g */
					//	#else
					//	if (v_time >= ((l->time) ) )
					//	#endif
						{
							add_all_teki(l);	/* �G���Z�b�g */
							l->v_time = (-1); 	/* �����ς݂��}�[�N */	/* enemy set done flag */	//l->done = 1;
						}
					}
					l = l->next;	/* ���𒲂ׂ� */	/* choice alter. */
				}
			}
			#else

			#endif
			/*	[��]���ꏈ���̂������ʒu */
		}
//
			/*
				���u�{�X�Ƒ��ł�����ƃn���O�A�b�v�v�o�O(�`r26)�΍�
				[�V]���ꏈ���̈ʒu:
				�{�X��|�����ꍇ�Ɏ���������ł��āA��炢�{�����蒆�̏ꍇ�A
				[��]���ꏈ���̈ʒu�ł́A��炢�{������̈�STATE_FLAG_14_GAME_LOOP_QUIT�Ȃ̂�
				ST ATE_FLAG_11_IS_BOSS_DESTROY(���ꏈ���Ŕ���)������ł��Ȃ��o�O(�`r26)������B
				�����ňʒu�𓮂������B(ST ATE_FLAG_11_IS_BOSS_DESTROY�̔���͏��Ȃ��Ƃ������瑤�ł���K�v������)
			 */
			#if (1)
			/* ���ꏈ��(���܂ɂ������s���Ȃ�����)
				�R�Avoid shooting_game_core_work(void)�֐��̃T�C�Y���傫���ƁA
				CPU�̖��߃L���b�V�����t���[���鎖�ɂ�菈����������̂ŁA
				���ꏈ��(���܂ɂ������s���Ȃ�����)�̓R�A�̊O�ɒǂ��o���܂��B
				���̍ہu static�֐��ɂ��Ȃ��v�l�ɒ��ӂ��܂��B
				static�֐��ɂ���ƁAGCC������� __inline__ �֐��ɕϊ������(-O3�̏ꍇ)
				�ǂ��o�����Ӗ��������Ȃ�܂��B(�C�����C���W�J�����)
			 */
		//	if (B00_NONE != pd->bo ssmode)
			if (pd->state_flag & (STATE_FLAG_10_IS_LOAD_SCRIPT/*|ST ATE_FLAG_11_IS_BOSS_DESTROY*/|STATE_FLAG_12_END_SCRIPT))
			{
				my_special();/* ���ӁFstatic�֐��ɂ��Ȃ� */
			}
			#endif
//
		#if 0/*�Q�[�����ԃf�o�b�O�p*/
		/* �p�l���̃X�R�A���ɃQ�[�����Ԃ� �\��������B���Ă����������������Ⴄ�B */
		((PLAYER_DATA *)player->data)->score		= (game_v_time);
		#endif
//
		/* ���� */
		bg_work_draw();
		//controller_work();
//		sprite_work222(SP_GROUP_ALL_SDL_WORK_TYPE);/*�e���p*/
//		sprite_work000(SP_GROUP_ALL_SDL_WORK_TYPE);
		sprite_work000(SP_GROUP_ALL_SDL_CORE_TYPE);
		/* �`�� */
//		sprite_display222(SP_GROUP_ALL_SDL_DRAW_TYPE);/*�e���p*/
//		sprite_display000((SP_GROUP_ALL_SDL_DRAW_TYPE & (~SP_GROUP_TEKI)));
//		sprite_display000(SP_GROUP_ALL_SDL_DRAW_TYPE);
	//	pause_sprite_display();/* SDL�\��(����SP_GROUP_PAUSE_SP_MENU_TEXT�̂�SDL�`��) */
		draw_score_chache();
		// ���̕ӂ͑��x�ቺ����̂ŃR�[���o�b�N�ɂ��ׂ�
		if ((pd->state_flag & STATE_FLAG_06_IS_SCRIPT)) 	{	script_display();	}	/*STATE_FLAG_06_IS_SCRIPT==*/	//parsys_display();
		if (0!=draw_side_panel) 							{	score_display();	}	/*ST ATE_FLAG_09_IS_PANEL_WINDOW==*/	/*(pd->state_flag & ST ATE_FLAG_09_IS_PANEL_WINDOW)*/
//
		vbl_draw_screen();	/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
	}
//
}
