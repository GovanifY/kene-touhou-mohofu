
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�Q�[���R�A
	�V���[�e�B���O�Q�[�����́A�u�Q�[���R�A�v�ōs���܂��B
	��������O�ɏo��ƁACPU�̖��߃L���b�V�����j�󂳂��̂ŁA
	���x���ቺ���܂��B
	-------------------------------------------------------
	pause�����́A�O�ɏo�܂��B
	-------------------------------------------------------
�ȉ��c��:(r31���_)
	�x���͒P�Ȃ��ςł��B�����͂Ȃ��ł��B
	-------------------------------------------------------
	�x��:��������������������������������������������������������
	�X�N�V����SDL���[�`���ɍs���̂Ř_�O�ɑ��x�ቺ���܂��B
	(�X�N�V��SDL�V�K�T�[�t�F�C�X����Ă��ɁA�����J�A�N�Z�X����̂Œx���ł�)
	-------------------------------------------------------
	�x��:��������������������
	�V�i���I�́ASDL���[�`���ɍs���̂�40/60[fps]���x�܂ő��x�ቺ���܂��B
	��ɃL���b�V���ɏ���ċ��R������������܂����A���R�ł��B
	SDL�g��Ȃ��l�ɏ��������Ȃ��Ƃ����܂���B
	-------------------------------------------------------
	�x��:��������������������
	�T�C�h�̃p�l����SDL���[�`���ɍs���̂ő��x�ቺ�v���ł��B
	SDL�g��Ȃ��l�ɏ��������΂��̕����x���サ�܂��B
	SDL�g���ɂ��Ă��A�L���b�V�����ɂ���Α��x���҂��܂��B
	���Ȃ݂ɁuNoiz2sa for psp version 0.7�v�̓L���b�V�����ő��x���҂��ł����ł��B
---------------------------------------------------------*/

#include "kanji_system.h"

/*---------------------------------------------------------
	�G�̒ǉ�
	-------------------------------------------------------
	�����ɒǋL����ƃV���[�e�B���O�Q�[���{�̂��x���Ȃ�̂ŒǋL���܂���B
	������(parth)���� load_stage.c �ŗ\�ߍς܂��Ă����܂��B
---------------------------------------------------------*/
extern void game_command_00_kanji_hyouji(		GAME_COMMAND *l);/* �ǂ̓G�ł��Ȃ��ꍇ�́A�����\�� */
extern void game_command_01_game_all_clear( 	GAME_COMMAND *l);/* �S�ʃN���A�[�̏ꍇ�A���̓G��ǉ� */
extern void game_command_02_bg_control( 		GAME_COMMAND *l);/* �w�i�R���g���[��(�X�N���[�����x�����w��) */
extern void game_command_03_regist_boss(		GAME_COMMAND *l);/* �{�X�A�X�N���v�g�N�� */
extern void game_command_04_regist_chuu_boss(	GAME_COMMAND *l);/* [���^�G]�d���A�������� */
extern void game_command_05_regist_zako(		GAME_COMMAND *l);/* �U�R�A�������� */
static void add_game_command(GAME_COMMAND *l)
{
	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
	/* game_command�̐�����ԍ��ŊǗ�(load_stage.c �� ctype_name[]�ɑΉ����Ă���) */
	void (*aaa[GC_CODE_MAX])(GAME_COMMAND *l) =
	{
		game_command_00_kanji_hyouji,		/* �����ŕ����\�� */	//	NULL,/* [�ԕ���؂�] (�ŕK�v)*/
	/* ���̑� */
		game_command_01_game_all_clear, 	/* �Q�[�� �S�X�e�[�W �N���A�[ */
		game_command_02_bg_control, 		/* */
	/* �G */
		game_command_03_regist_boss,		/* �{�X���� */
		game_command_04_regist_chuu_boss,	/* ��-�{�X�̗\�� ����G[���^�G] */
		game_command_05_regist_zako,		/* �U�R */
	//
	};
	(*aaa[ (int)(l->user_i_code) ])(l); 	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static u32 game_v_time;/* �Q�[������ game flame time counter. */

#if (1==USE_HOLD_GAME_MODE)
static int v_time_hold_mode;/* ���p�Ɏ~�߂��蓮��������o����悤�ɂ��Ƃ� */
#endif /* (1==USE_HOLD_GAME_MODE) */

static void init_stage_start_time(void)
{
//	stage_start_time = psp_get_uint32_ticks();
	game_v_time = 0;
}
/*static*/global void set_core_game_time_MAX(void)
{
	game_v_time = 65535;/* �K���ɑ傫�Ȓl[flame](65535[flame]==��18[��]==18.xxx x 60 x 60 ) */
}

#if (1==USE_HOLD_GAME_MODE)
global void hold_game_mode_on(void)/* �Q�[�����Ԃ̈ꎞ��~(���A�V�K�i�C�x���g(�\�z��)���A�g��) */
{
	v_time_hold_mode = 1;
}

global void hold_game_mode_off(void)/* �Q�[�����Ԃ̓���J�n */
{
	v_time_hold_mode = 0;
}
#endif /* (1==USE_HOLD_GAME_MODE) */


/*---------------------------------------------------------
	�V���[�e�B���O�Q�[���{�̂̏�����
---------------------------------------------------------*/

extern void sprite_test_debug_init(void);/* r32:�Ƃ肠�����o�O����̂𖳗������(?) */
extern void set_rnd_seed(int set_seed);
extern void load_stage(void);
extern void player_init_first(void);
extern void player_init_stage(void);
extern void score_panel_init(void);
global void common_load_init(void)
{
	set_rnd_seed(cg.game_now_stage);	/* �����n��̏����� */
//
	/* Load next stage */
	load_stage();
	// ���[�h���͏����������Ă���̂ŁA���[�h��Ɏ��Ԃ��č쐬����B
	init_stage_start_time();
//
	player_init_stage();/* �X�e�[�W�J�n���̂ݎ኱�̖��G��ԂɃZ�b�g */
//
	kanji_window_clear();	/* �����E�B���h�E�̓��e�������B */
	home_cursor();			/* �J�[�\�����z�[���|�W�V�����ֈړ� */
	//
	#if (1==USE_HOLD_GAME_MODE)
	hold_game_mode_off();/* �Q�[�����Ԃ̓���J�n */
	#endif /* (1==USE_HOLD_GAME_MODE) */
	//
	#if 1/*Gu������������v��Ȃ��Ȃ�*/
	{
		psp_clear_screen(); /* [PAUSE] ���A����SDL��ʂ������B */
	}
	#endif
	sprite_test_debug_init();/* r32:�Ƃ肠�����o�O����̂𖳗������(?) */
	main_call_func = shooting_game_core_work;
}


/*---------------------------------------------------------
	�Q�[���R�A����(�Q�[���J�n��)����̏�����
---------------------------------------------------------*/

//global void shooting_game_core_1st_init(void)
global void stage_first_init(void)
{
	score_panel_init();
	//sprite_controller_remove_all();
//
	player_init_first();/* ����̂ݐݒ� */
//	player_init_stage();/* �X�e�[�W�J�n���̂ݎ኱�̖��G��ԂɃZ�b�g */
//
	kanji_window_clear();	/* �����E�B���h�E�̓��e�������B */
	home_cursor();			/* �J�[�\�����z�[���|�W�V�����ֈړ� */
//
	cg.game_now_stage	= cg.game_continue_stage;
//
	main_call_func = common_load_init;
}


/*---------------------------------------------------------
	�C�x���g�V�[�������ɐi�߂�B
---------------------------------------------------------*/

global void incliment_scene(void)
{
	if ((cg.state_flag & STATE_FLAG_05_IS_BOSS))
	{
		/*(�{�X�퓬��C�x���g)*/
		main_call_func = stage_clear_result_screen_start;	/* �X�e�[�W�N���A�[���̃��U���g��� */
	}
	else
	{
		/*(�{�X�퓬�O�C�x���g)*/
		/*(r32)*/cg.state_flag |= (STATE_FLAG_05_IS_BOSS|STATE_FLAG_13_DRAW_BOSS_GAUGE);
		/* �G���ǉ��ǂݍ��ݏ������~����B */
		cg.state_flag			&= (~STATE_FLAG_14_ZAKO_TUIKA); 	/* off / �G���ǉ��ǂݍ��ݏ������~����B */
	}
}


/*---------------------------------------------------------
	����C�x���g�̎��s����
	���ꏈ��(���܂ɂ������s���Ȃ�����)
	-------------------------------------------------------
	�R�A(���C�����[�v��)�ɒǉ�����ƁA�V���[�e�B���O�Q�[���{�̂��x���Ȃ�̂ŁA
	���삪�x�����Ēe���V���[�e�B���O�Q�[���ɁA�Ȃ�Ȃ��Ȃ��Ă��܂��B
	�����œ��쑬�x�I�ϓ_����A�p�x�̏��Ȃ�����@�\�͂����Ŏ��s����B
---------------------------------------------------------*/
extern GAME_COMMAND *stage_command_table;
static void game_core_zako_tuika(void)
{
	/*
		This routine, search back to begin.
		���̃��[�`���͋t���Ɍ������܂��B
	*/
	{
		GAME_COMMAND *l;
		l = stage_command_table;
		while (NULL != l)	/* �R�}���h���X�g�̏I���(NULL)�܂Œ��ׂ� */	/* [head ==NULL] then end. */
		{
			if (0 < l->v_time ) 	/* �R�}���h�����ς݁H */
			{
				if (game_v_time >= (l->v_time)) 	/* (���ݎ��� >= �ݒ莞��) �Ȃ�A�G���Z�b�g */
				{
					add_game_command(l);	/* �R�}���h��������(�R�}���h���G���G�̏ꍇ�A�G���G�𐶐�����) */
					l->v_time = (-1);		/* �R�}���h�����ς݂��}�[�N */
				}
			}
			l = l->next;	/* ���𒲂ׂ� */	/* choice alter. */
		}
	}
	/* �����̏ꍇ����ɋ�ݏo���`�F�b�N���s���e������(�b��I) */
	#if 0
//	if (0!=(cg.state_flag & STATE_FLAG_05_IS_NOT_BOSS))
	if (0==(cg.state_flag & STATE_FLAG_05_IS_BOSS))/*(r32)*/
	#endif
	{
		bullet_angle_all_gamen_gai_nara_kesu();/* �p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�) */
	}
}
//
extern void script_system_SDL_draw(void);
extern void script_move_main(void);
extern void script_ivent_load(void);
/* ���ӁF(���쑬�x�ቺ����̂�)static�֐��ɂ��Ȃ� */global void my_special(void)
{
	#if 1
	/* �G�����������o�ꂳ���鏈�� */
	{
	//	if (cg.state_flag & (ST ATE_FLAG_14_GAME_LOOP_QUIT))
	//	{
	//		;	/* GAMEOUT�� */
	//	}
	//	else
		if (cg.state_flag & (STATE_FLAG_14_ZAKO_TUIKA))
		{
			/* �����Ă� */
			game_core_zako_tuika();
			/* [��]���ꏈ���̂������ʒu */
		}
	}
	#endif

	#if 1
	/*
		���u(��炢�{����t���Ԓ���)�{�X�Ƒ��ł�����ƃn���O�A�b�v�v�o�O(�`r29)�΍�
	*/
	if (0 < /*bomb_wait*/cg.bomber_time)		/* �{���E�F�C�g���� */
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
	if (0==(cg.state_flag & (/*STATE_FLAG_06_IS_SCRIPT|*/STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL)))
	{
		return;/* �{���������͑ҋ@ */
	}
	#endif
	if (cg.state_flag & (STATE_FLAG_10_IS_LOAD_SCRIPT))
	{
		cg.state_flag &= (~(STATE_FLAG_10_IS_LOAD_SCRIPT));/*off*/
		script_ivent_load();/*0 1*/
	}

//	if (cg.state_flag & (ST ATE_FLAG_11_IS_BOSS_DESTROY))
//	{
//		cg.state_flag &= (~(ST ATE_FLAG_11_IS_BOSS_DESTROY));/*off*/
//		boss_destroy_aaa();
//	}
	/* �X�N���v�g���I������H */
	if (cg.state_flag & (STATE_FLAG_12_END_SCRIPT))
	{
		cg.state_flag &= (~(STATE_FLAG_12_END_SCRIPT));/*off*/
		incliment_scene();
	}
	/* �X�N���v�g���삪�K�v�H */
	/*(r32)*/if (cg.state_flag & STATE_FLAG_06_IS_SCRIPT)
	{
		script_system_SDL_draw();	/* �X�N���v�g SDL �`��(�x��) */
		script_move_main(); 		/* �X�N���v�g����(�ړ�) */
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

extern void score_display(void);
extern void bg2_move_main(void);
extern void draw_SDL_score_chache(void);

global void shooting_game_core_work(void)
{
	{
my_game_core_loop:
		/* [A] �Q�[�����Ԃ��o�߂�����B */
		#if (1==USE_HOLD_GAME_MODE)
		if (0==v_time_hold_mode)/* ���p�Ɏ~�߂��蓮��������o����悤�ɂ��Ƃ� */
		#endif /* (1==USE_HOLD_GAME_MODE) */
		{
			game_v_time++;	/* �Q�[�����Ԃ̓t���[���P��(game time resolutions about 1/60 seconds.) */
		}
		#if 0/* �Q�[�����ԃf�o�b�O�p */
		/* �p�l���̃X�R�A���ɃQ�[�����Ԃ� �\��������B���Ă����������������Ⴄ�B */
		pd_score		= (game_v_time);
		#endif
	//
		/* [B] ���ꏈ���C�x���g���������Ă���ꍇ�A���ꏈ�����s���B */
		#if (1)
		/*
			���u�{�X�Ƒ��ł�����ƃn���O�A�b�v�v�o�O(�`r26)�΍�
			[�V]���ꏈ���̈ʒu:
			�{�X��|�����ꍇ�Ɏ���������ł��āA��炢�{�����蒆�̏ꍇ�A
			[��]���ꏈ���̈ʒu�ł́A��炢�{������̈�ST ATE_FLAG_14_GAME_LOOP_QUIT�Ȃ̂�
			ST ATE_FLAG_11_IS_BOSS_DESTROY(���ꏈ���Ŕ���)������ł��Ȃ��o�O(�`r26)������B
			�����ňʒu�𓮂������B(ST ATE_FLAG_11_IS_BOSS_DESTROY�̔���͏��Ȃ��Ƃ������瑤�ł���K�v������)
		 */
		/* ���ꏈ��(���܂ɂ������s���Ȃ�����)
			�R�Avoid shooting_game_core_work(void)�֐��̃T�C�Y���傫���ƁA
			CPU�̖��߃L���b�V�����t���[���鎖�ɂ�菈����������̂ŁA
			���ꏈ��(���܂ɂ������s���Ȃ�����)�̓R�A�̊O�ɒǂ��o���܂��B
			���̍ہu static�֐��ɂ��Ȃ��v�l�ɒ��ӂ��܂��B
			static�֐��ɂ���ƁAGCC������� __inline__ �֐��ɕϊ������(-O3�̏ꍇ)
			�ǂ��o�����Ӗ��������Ȃ�܂��B(�C�����C���W�J�����)
		 */
		/*(r32)*/if (cg.state_flag & (
			STATE_FLAG_10_IS_LOAD_SCRIPT |	//
			STATE_FLAG_12_END_SCRIPT |		//
			STATE_FLAG_06_IS_SCRIPT |		//
			STATE_FLAG_14_ZAKO_TUIKA))		// �����̓U�R�ǉ��K�v�Ȃ̂œ��ꏈ��
			/*|ST ATE_FLAG_11_IS_BOSS_DESTROY*/
		{
			my_special();/* ���ӁF(���쑬�x�ቺ����̂�)static�֐��ɂ��Ȃ� */
		}
		#endif
		/*
			[C] ����(�ړ�)������B
			����(�ړ�)�ƕ`��͈Ⴄ�T�O�Ȃ̂ł�����ƕ������鎖�B
			�����A�������x���Ȃ��āA�`����t���[���X�L�b�v������ꍇ�ł��A
			����(�ړ�)�̓t���[���X�L�b�v�����Ȃ��B
		*/
		bg2_move_main();	/* �w�i�̈ړ����� */
		sprite_move_all();	/* �X�v���C�g�I�u�W�F�N�g�̈ړ����� */
		/* [D] �`�� */
		// ���̕ӂ͑��x�ቺ����̂ŃR�[���o�b�N�ɂ��ׂ�
		/*(r32)*/if (0!=(cg.side_panel_draw_flag))
		{
			score_display();		/* �X�R�A�p�l�� SDL �`��(�x��) */
		}	/*ST ATE_FLAG_09_IS_PANEL_WINDOW==*/	/*(cg.state_flag & ST ATE_FLAG_09_IS_PANEL_WINDOW)*/
//
	// �n���O�A�b�v�΍�F��Ƀ|�[�Y�\�ɕύX����B(2010-02-11)
	// ���j���[�̃L�[���͂��d�l�ύX�ɂȂ����̂ŁA����������ɕ����Ďd�l�ύX�B(2010-06-01)
	//	if (0==my_pad)
		if (0==(psp_pad.pad_data_alter & PSP_KEY_PAUSE))/* �������|�[�Y��������ĂȂ��� */
		{
			if (psp_pad.pad_data & PSP_KEY_PAUSE)/* ���|�[�Y�������ꂽ�� */
			{
			//	if (0==(cg.state_flag & STATE_FLAG_06_IS_SCRIPT))/*���܂ɂ��܂������Ȃ���������*/
				{
					main_call_func			= pause_menu_start;
					pause_out_call_func 	= shooting_game_core_work;/* �|�[�Y���A��̖߂������߂� */
				}
			}
		}
		/* Gu�`�� */
		{
			vbl_draw_screen();	/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
		}
		/* �Q�[���R�A����O�ɏo��ƁA(CPU�̖��߃L���b�V��������̂�(?))�A�ɒ[�ɑ��x�ቺ����B
			�K�v�Ȃ��ꍇ�͊O�ɏo�Ȃ��ׂ̏��u */
		if (shooting_game_core_work == main_call_func)
		{
			goto my_game_core_loop;
		}
		/* �����ɗ�����A�O�ɏo��̂Œx���Ȃ�Ƃ����� */
	}
}
