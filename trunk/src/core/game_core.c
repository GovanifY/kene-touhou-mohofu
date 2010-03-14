
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
extern void add_boss_mima(  			STAGE_DATA *l);/* 3�ʂ͂ǂ����邩���� */
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
extern void add_zako_aka_kedama1(		STAGE_DATA *l);		/* �� */
	/* �ы� */
extern void add_zako_inseki1(			STAGE_DATA *l); 	/* ���̑��U�R */
extern void add_zako_yukari2(			STAGE_DATA *l); 	/* ���̑��U�R */
extern void add_zako_midori_kedama1(	STAGE_DATA *l);
extern void add_zako_midori_kedama2(	STAGE_DATA *l);
extern void add_zako_kedama1(			STAGE_DATA *l);
extern void add_zako_kedama2(			STAGE_DATA *l);
	/* [C�d��]���̑��U�R */
extern void add_zako_obake1(			STAGE_DATA *l);
extern void add_zako_obake2(			STAGE_DATA *l);
extern void add_zako_karasu1(			STAGE_DATA *l);
	/* [B�d��]���d�� */
extern void add_zako_meido1(			STAGE_DATA *l);
extern void add_zako_meido2(			STAGE_DATA *l);
extern void add_zako_meido3(			STAGE_DATA *l);
extern void add_zako_meido4(			STAGE_DATA *l);
	/* [A�d��]���d�� */
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

int difficulty = RANK_EASY; 	/*  RANK_NORMAL*/

//static u32 stage_start_time;
//static u32 game_start_time;

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
		add_boss_mima,  			/*add_boss_kene*/		/* extra 2�� */
		add_boss_pache, 			/*add_boss_cirno*/		/* extra 1�� */
		add_boss_sakuya,			/* 6�ʂ͍�� */ 		// [***090207		�ǉ�
		add_boss_pache, 			/* 5�ʂ̓p�`�����[ */	//		�ǉ�
		add_boss_kaguya,			/* 4�ʂ͋P�� */
		add_boss_mima,  			/* 3�ʂ͂ǂ����邩����(�i��? �d��?) */
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
		add_zako_aka_kedama1,		/*	a"�Ԗы�1"	*/		/* �� */
	/* �ы� */
		add_zako_inseki1,			/*	C"覐�1"	*/		/*enemy_error*/ 	/* ���̑��U�R */
		add_zako_yukari2,			/*	C"���ґ�2"	*/		/*enemy_error*/ 	/* ���̑��U�R */
		add_zako_midori_kedama1,	/*	-"�Ζы�1"	*/
		add_zako_midori_kedama2,	/*	C"�Ζы�2"	*/		/*enemy_error*/
		add_zako_kedama1,			/*	-"�ы�1"	*/
		add_zako_kedama2,			/*	-"�ы�2"	*/
	/* [C�d��]���̑��U�R */
		add_zako_obake1,			/*	-"���΂�1"	*/
		add_zako_obake2,			/*	-"���΂�2"	C"���ы�1"	*/		/*enemy_error*/
		add_zako_karasu1,			/*	-"�G1"	*/
	/* [B�d��]���d�� */
		add_zako_meido1,			/*	-"���C�h1"	*/
		add_zako_meido2,			/*	-"���C�h2"	*/
		add_zako_meido3,			/*	C"���C�h3"	*/		/*enemy_error*/
		add_zako_meido4,			/*	-"���C�h4"	*/
	/* [A�d��]���d�� */
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
static u32 game_v_time;
static int v_time_hold_mode;
static void init_stage_start_time(void)
{
//	stage_start_time = psp_get_uint32_ticks();
	game_v_time = 0;
}

void hold_game_time(void)/* �Q�[�����Ԃ̈ꎞ��~(���A�V�K�i�C�x���g(�\�z��)���A�g��) */
{
	v_time_hold_mode = 1;
}

void continue_game_time(void)/* �Q�[�����Ԃ̓���J�n */
{
	v_time_hold_mode = 0;
}

/*---------------------------------------------------------
	�V���[�e�B���O�Q�[���{�̂̏�����
---------------------------------------------------------*/
extern int continue_stage;

extern void set_rnd_seed(int set_seed);
extern /*int*/void load_stage(void/*int level*/);
extern void player_init(void);
extern void player_few_muteki(void);
extern void score_panel_init(void);
void common_load_init(void)
{
	set_rnd_seed(player_now_stage); 	/* �����n��̏����� */
//
	/* Load next stage */
	load_stage();//if (0==load_stage(/*level*/))	{	error(ERR_WARN, "no entrys for level %d",level);}
	// ���[�h���͏����������Ă���̂ŁA���[�h��Ɏ��Ԃ��č쐬����B
	init_stage_start_time();
//
	player_few_muteki();/* �X�e�[�W�J�n���̂ݎ኱�̖��G��ԂɃZ�b�g */
//
	script_message_window_clear();/*�X�N���v�g���b�Z�[�W��ʂ�����*/
	continue_game_time();/* �Q�[�����Ԃ̓���J�n */
			#if 1/*Gu������������v��Ȃ��Ȃ�*/
			{
				psp_clear_screen();	/* [PAUSE] ���A����SDL��ʂ������B */
			}
			#endif
	psp_loop = (ST_WORK_GAME_PLAY|0);
}


/*---------------------------------------------------------
	�Q�[���R�A����(�Q�[���J�n��)����̏�����
---------------------------------------------------------*/

//void shooting_game_core_1st_init(void)
void stage_first_init(void)
{
	score_panel_init();
	//sprite_controller_remove_all();
//
	player_init();/* ����̂ݐݒ� */
//	player_few_muteki();/* �X�e�[�W�J�n���̂ݎ኱�̖��G��ԂɃZ�b�g */
//
	script_message_window_clear();
	player_now_stage/*data->now_stage*/ /*level*/	= continue_stage/*+1-1*/ /*1*/;
//
	psp_loop = (ST_INIT_GAME_PLAY_common|0);
//	common_load_init();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

void incliment_scene(void)
{
	{
		/*�{�X�퓬��C�x���g*/
	//	if (B09_STAGE_LOAD==pd_bo ssmode) // 9:stage�ǂݍ���
		if (/*STATE_FLAG_05_IS_BOSS == */(pd_state_flag & STATE_FLAG_05_IS_BOSS))
		{
			psp_loop = (ST_WORK_STAGE_CLEAR|0);
		}
		/*�{�X�퓬�O�C�x���g*/
		else
	//	if (B08_START == pd_bo ssmode) // 8:�{�X�Ȃ�炵�A1�{�X�Ƃ̐퓬�ցB
		{
			pd_state_flag |= (STATE_FLAG_05_IS_BOSS|STATE_FLAG_13_DRAW_BOSS_GAUGE);
		}
	}
}
//
extern void script_ivent_load(void);
/* ���ӁFstatic�֐��ɂ��Ȃ� */void my_special(void)
{
	#if 1
	/*
		���u(��炢�{����t���Ԓ���)�{�X�Ƒ��ł�����ƃn���O�A�b�v�v�o�O(�`r29)�΍�
	*/
	if (0 < /*bomb_wait*/pd_bomber_time)		/* �{���E�F�C�g���� */
	{
		return;/* �{���������͑ҋ@ */
	}
	#endif
//


	#if 0
	/*
		���u(��炢�{����t���Ԓ���)�{�X�Ƒ��ł�����ƃn���O�A�b�v�v�o�O(�`r29)�΍�
	*/
	/* �L�[���͖�����(==������) �́A�G�����蔻��͂Ȃ� */
	if (0==(pd_state_flag & (/*STATE_FLAG_06_IS_SCRIPT|*/STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL)))
	{
		return;/* �{���������͑ҋ@ */
	}
	#endif
//	if (pd_bo ssmode==B05_BEFORE_LOAD) 	// [***090313	�ǉ�
//	if (pd_bo ssmode==B07_AFTER_LOAD)		// [***090313	�ǉ�
	if (pd_state_flag & (STATE_FLAG_10_IS_LOAD_SCRIPT))		// [***090313	�ǉ�
	{
		pd_state_flag &= (~(STATE_FLAG_10_IS_LOAD_SCRIPT));/*off*/
		script_ivent_load(/*0 1*/);
	}

//	if (pd_state_flag & (ST ATE_FLAG_11_IS_BOSS_DESTROY))
//	{
//		pd_state_flag &= (~(ST ATE_FLAG_11_IS_BOSS_DESTROY));/*off*/
//		boss_destroy_aaa();
//	}
	/* �X�N���v�g���I������H */
	if (pd_state_flag & (STATE_FLAG_12_END_SCRIPT))
	{
		pd_state_flag &= (~(STATE_FLAG_12_END_SCRIPT));/*off*/ 	/*	pd_bo ssmode=B00_NONE;*/
	//	pd_state_flag &= (~(STATE_FLAG_12_END_SCRIPT));/*off*/ 	/*	pd_bo ssmode=B00_NONE;*/	/*B01_BA TTLE*/
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
extern void draw_SDL_score_chache(void);

void shooting_game_core_work(void)
{
	while ((ST_WORK_GAME_PLAY>>8) == (psp_loop>>8) )
	{

		if (0==v_time_hold_mode)
		{
			/* game_v_time=Zeit seit Spielbeginn in 1/10 sec. */
			game_v_time++;//=(psp_get_uint32_ticks()-stage_start_time);
		}
	//


		if (pd_state_flag & STATE_FLAG_14_GAME_LOOP_QUIT)
		{
			;	/* GAMEOUT�� */
		}
		else
		{
			/* �����Ă� */
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
		//	if (B00_NONE != pd_bo ssmode)
			if (pd_state_flag & (STATE_FLAG_10_IS_LOAD_SCRIPT|STATE_FLAG_12_END_SCRIPT))/*|ST ATE_FLAG_11_IS_BOSS_DESTROY*/
			{
				my_special();/* ���ӁFstatic�֐��ɂ��Ȃ� */
			}
			#endif
//
		#if 0/*�Q�[�����ԃf�o�b�O�p*/
		/* �p�l���̃X�R�A���ɃQ�[�����Ԃ� �\��������B���Ă����������������Ⴄ�B */
		pd_score		= (game_v_time);
		#endif
//
		/* ���� */
		bg_work_draw();
		//controller_work();
//		sprite_work222(SP_GROUP_ALL_SDL_WORK_TYPE);/*�e���p*/
//		sprite_work000(SP_GROUP_ALL_SDL_WORK_TYPE);
		sprite_work_SDL(SP_GROUP_ALL_SDL_CORE_TYPE);/*gu�ėp*/
		sprite_work_444(SP_GROUP_ALL_SDL_CORE_TYPE);/*�e����p*/
		/* �`�� */
//		sprite_display222(SP_GROUP_ALL_SDL_DRAW_TYPE);/*�e���p*/
//		sprite_display000((SP_GROUP_ALL_SDL_DRAW_TYPE & (~SP_GROUP_TEKI)));
//		sprite_display000(SP_GROUP_ALL_SDL_DRAW_TYPE);
	//	pause_sprite_display();/* SDL�\��(����SP_GROUP_PAUSE_S P_ME NU_TEXT�̂�SDL�`��) */
	//	draw_SDL_score_chache();/* SDL�`�� */
		// ���̕ӂ͑��x�ቺ����̂ŃR�[���o�b�N�ɂ��ׂ�
		if ((pd_state_flag & STATE_FLAG_06_IS_SCRIPT)) 	{	script_display();	}	/*STATE_FLAG_06_IS_SCRIPT==*/
		if (0!=draw_side_panel) 							{	score_display();	}	/*ST ATE_FLAG_09_IS_PANEL_WINDOW==*/	/*(pd_state_flag & ST ATE_FLAG_09_IS_PANEL_WINDOW)*/
//

// �n���O�A�b�v�΍�F��Ƀ|�[�Y�\�ɕύX����B(2010-02-11)
		//	if (0==my_pad)
			if (0==(my_pad & PSP_KEY_PAUSE))
			{
				if (my_pad_alter & PSP_KEY_PAUSE)
				{
				//	if (0==(pd_state_flag & STATE_FLAG_06_IS_SCRIPT))/*���܂ɂ��܂������Ȃ���������*/
					{
						psp_loop = (ST_INIT_MENU|ST_MENU_SUB_PAUSE);
					}
				}
			}
//
		vbl_draw_screen();	/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
	}
//
}
