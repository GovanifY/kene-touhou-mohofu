
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
	�Q�[���R�}���h�̐V�K�ǉ��B
	-------------------------------------------------------
	�����ɒǋL����ƃV���[�e�B���O�Q�[���{�̂��x���Ȃ�̂ŒǋL���܂���B
	������(parth)���� load_stage.c �ŗ\�ߍς܂��Ă����܂��B
---------------------------------------------------------*/
extern void game_command_00_kanji_hyouji(			GAME_COMMAND *l);/* �ǂ̓G�ł��Ȃ��ꍇ�́A�����\�� */
extern void game_command_01_game_all_clear( 		GAME_COMMAND *l);/* �S�ʃN���A�[�̏ꍇ�A���̃Q�[���R�}���h��ǉ� */
extern void game_command_02_bg_control( 			GAME_COMMAND *l);/* �w�i�R���g���[��(�X�N���[�����x�����w��) */
extern void game_command_03_check_secret_bonus( 	GAME_COMMAND *l);/* �B���{�[�i�X�`�F�b�N�I���B���ʕ\���B */
extern void game_command_04_begin_secret_bonus( 	GAME_COMMAND *l);/* �B���{�[�i�X�`�F�b�N�J�n�B */
extern void game_command_05_kaiwa_start_boss(		GAME_COMMAND *l);/* �{�X�A�V�i���I��b�N�� */
extern void game_command_06_regist_chuu_boss(		GAME_COMMAND *l);/* [���^�G]�d���A�������� */
extern void game_command_07_regist_zako(			GAME_COMMAND *l);/* �U�R�A�������� */

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


extern void set_rnd_seed(int set_seed);
extern void load_stage(void);

extern void player_init_stage(void);

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
	#if (1)/*(�����֘A�̏�����)*/
	set_kanji_origin_xy((10+6), (10+192));/*(�\�����_�̐ݒ�)*/
	set_kanji_origin_kankaku(18);/*(���Ԃ�W���ɂ���)*/
//	set_kanji_hide_line(ML_LINE_02);/*(2�s�ڈȉ����\���ɂ���B)*/
	set_kanji_hide_line(ML_LINE_04);/*(4�s�ڈȉ����\���ɂ���B)*/
	kanji_window_all_clear();				/* ������ʂ�S�s�����B�����J�[�\�����z�[���|�W�V�����ֈړ��B */
	#endif
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
	cb.main_call_func = shooting_game_core_work;
}


/*---------------------------------------------------------
	�Q�[���R�A����(�Q�[���J�n��)����̏�����
---------------------------------------------------------*/

//global void shooting_game_core_1st_init(void)
extern void score_panel_init(void);
extern void jiki_class_initialize(void);
global void stage_first_init(void)
{
	score_panel_init();
//	sprite_controller_remove_all();
//
	jiki_class_initialize();/* (C++�ɂ����ł͂Ȃ���)���@�N���X(jiki::jiki();)���쐬���������B����̂ݐݒ� */
//
	cg.game_now_stage	= cg.game_continue_stage;
	cb.main_call_func	= common_load_init;
}


/*---------------------------------------------------------
	(�V�i���I��b�������I������̂�)
	�C�x���g�V�[�������ɐi�߂�B
---------------------------------------------------------*/

global void incliment_scene(void)
{
	set_kanji_hide_line(ML_LINE_02);/*(2�s�ڈȉ����\���ɂ���B)*/
	if ((cg.state_flag & STATE_FLAG_05_IS_BOSS))
	{	/*(�u�{�X�퓬��̉�b���I��������ԁv�̏ꍇ)*/
		cb.main_call_func = stage_clear_result_screen_start;	/* �X�e�[�W�N���A�[���̃��U���g��� */
	}
	else
	{
	//	cg.state_flag |= STATE_FLAG_05_IS_BOSS; 	/* �{�X�퓬�O�̉�b�I����ݒ� */
		/*(�u�{�X�퓬�O�̉�b���I��������ԁv�̏ꍇ)*/
		/*(r32)*/cg.state_flag |= (STATE_FLAG_05_IS_BOSS|STATE_FLAG_13_DRAW_BOSS_GAUGE);
		/* �����R�}���h�ǉ��ǂݍ��ݏ������~����B */
		cg.state_flag			&= (~STATE_FLAG_14_DOUCHU_TUIKA);	/* off */
	}
}


/*---------------------------------------------------------
	�Q�[���R�}���h(��ɎG��)�ǉ�����
	-------------------------------------------------------
---------------------------------------------------------*/
extern GAME_COMMAND *stage_command_table;
static void game_core_teki_tuika(void)
{
	/*
		This routine, search back to begin.
		���̃��[�`���͋t���Ɍ������܂��B
	*/
	{
		GAME_COMMAND *aaa;
		aaa = stage_command_table;
		/* �R�}���h���X�g�̏I���(NULL)�܂Œ��ׂ� */	/* [head ==NULL] then end. */
		while (NULL != aaa)
		{
			/* �R�}���h�����ς݁H */
			if (0 < (aaa->v_time) )
			{	/* �������̃R�}���h */
				/* (���ݎ��� >= �ݒ莞��) �Ȃ�A�R�}���h�𐶐� */
				if (game_v_time >= (aaa->v_time))
				{	/* �R�}���h�����ς݂��}�[�N */
					aaa->v_time = (-1);
					/* �R�}���h��������(�R�}���h���G���G�̏ꍇ�A�G���G�𐶐�����) */
				//	regist_game_command(aaa);
				//	static void regist_game_command(GAME_COMMAND *aaa)
					{	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
						/* game_command�̐�����ԍ��ŊǗ�(load_stage.c �� ctype_name[]�ɑΉ����Ă���) */
						void (*bbb[/*(6)*/GC_CODE_MAX/*(8)*/])(GAME_COMMAND *aaa) =
						{
							game_command_00_kanji_hyouji,			/* �����ŕ����\���B */		//	NULL,/* [�ԕ���؂�] (�ŕK�v)*/
							game_command_01_game_all_clear, 		/* �Q�[�� �S�X�e�[�W �N���A�[���w���B */
							game_command_02_bg_control, 			/* �w�i�̐���R�}���h�B */
							game_command_03_check_secret_bonus, 	/* �B���{�[�i�X�`�F�b�N�I���B���ʕ\���B */
						//
							game_command_04_begin_secret_bonus, 	/* �B���{�[�i�X�`�F�b�N�J�n�B */
							game_command_05_kaiwa_start_boss,		/* �{�X��ǉ�����(r35)�B���� �{�X�̃V�i���I��b���N������B(�V�i���I��b���Ń{�X�ǉ�) */
							game_command_06_regist_chuu_boss,		/* ���{�X��ǉ�����B (����G[���^�G]) */
							game_command_07_regist_zako,			/* �U�R��ǉ�����B */
						//	game_command_01_game_all_clear, 		/* (.align)�_�~�[�B */
						//	game_command_01_game_all_clear, 		/* (.align)�_�~�[�B */
						};
						(*bbb[ (int)(aaa->user_i_code) ])(aaa); 	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
					}
				}
			}
			aaa = aaa->next;	/* ���𒲂ׂ� */	/* choice alter. */
		}
	}
	#if (1)/*(�Ƃ肠����)*/
	if (SPELL_00 == card.card_number)		/* �J�[�h�������Ȃ��ꍇ�A�����p�R�[���o�b�N���ĂԁB */
	{
		danmaku_system_callback();/* �e���R�[���o�b�N�V�X�e��(��P���ɌĂ�) */
	}
	#endif
}
	#if (0)/*(r35�I�������ĂȂ��B���̂��{�X���ɗL���ɂȂ�)*/
	/* �����̏ꍇ����ɋ�ݏo���`�F�b�N���s���e������(�b��I) */
	#if 0
//	if (0!=(cg.state_flag & STATE_FLAG_05_IS_NOT_BOSS))
	if (0==(cg.state_flag & STATE_FLAG_05_IS_BOSS))/*(r32)*/
	#endif
	{
		/*
			�e�����́u�e���R�[���o�b�N�V�X�e���v�ɕW�����ڂ���Ă�����̂��g���B
			���̃V�X�e�����g�p���Ēe�������s���ꍇ�́A�V�X�e���̏��������K�v�Ȃ̂ŁA
			���ׂ̈̏������͊e�ʊJ�n�O�̃��[�f�B���O���ɍs���B
		*/
		#if (1)
		/* �e�����́A���t���[���s���K�v���Ȃ��̂ŁA�������x���Ȃ�Adanmaku_system_callback();�𐔃t���[���Ɉ�x�ĂԂ悤�ɕύX����Ηǂ��B */
		static int aaaa=0;
		aaaa++;
		aaaa &= (0x07);
		if (0==aaaa)
		#endif
		{
			danmaku_system_callback();/* �e���R�[���o�b�N�V�X�e��(��P���ɌĂ�) */
		}
	//	danmaku_action_00_gamen_gai_nara_tama_wo_kesu();/* �p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�) */
	}
	#endif

/*---------------------------------------------------------
	����C�x���g�̎��s����
	���ꏈ��(���܂ɂ������s���Ȃ�����)
	-------------------------------------------------------
	-------------------------------------------------------
	�R�A(���C�����[�v��)�ɒǉ�����ƁA�V���[�e�B���O�Q�[���{�̂��x���Ȃ�̂ŁA
	���삪�x�����ăQ�[���ɁA�Ȃ�Ȃ��Ȃ��Ă��܂��B
	�����œ��쑬�x�I�ϓ_����A�p�x�̏��Ȃ�����@�\�͂����Ŏ��s����B
---------------------------------------------------------*/
extern void kaiwa_system_SDL_draw(void);
extern void kaiwa_system_execute_move_only_main(void);
extern void kaiwa_load_ivent(void);
/* ���ӁF(���쑬�x�ቺ����̂�)static�֐��ɂ��Ȃ� */global void my_special(void)
{
	/* �����R�}���h�ǉ��ǂݍ��ݏ��� */
	if (cg.state_flag & (STATE_FLAG_14_DOUCHU_TUIKA))
	{
		game_core_teki_tuika();
	}
	/* ���u(��炢�{����t���Ԓ���)�{�X�Ƒ��ł�����ƃn���O�A�b�v�v�o�O(�`r29)�΍� */
	if (0 < /*bomb_wait*/cg.bomber_time)		/* �{���E�F�C�g���� */
	{
		return;/* �{���������͑ҋ@ */
	}
	/* �V�i���I��b�����[�h���ĊJ�n���� */
	if (cg.state_flag & (STATE_FLAG_10_IS_LOAD_KAIWA_TXT))
	{
		cg.state_flag &= (~(STATE_FLAG_10_IS_LOAD_KAIWA_TXT));/*off*/
		kaiwa_load_ivent();/*(�V�i���I��b�����[�h���ĊJ�n����)*/
	}
	/*(��b���[�h�������I������H)*/
	if (cg.state_flag & (STATE_FLAG_12_END_KAIWA_MODE))
	{
		cg.state_flag &= (~(STATE_FLAG_12_END_KAIWA_MODE));/*off*/
		incliment_scene();/*(���̏�Ԃɐi�߂�)*/
	}
	/* ��b���[�h���삪�K�v�H */
	if (cg.state_flag & STATE_FLAG_06_IS_KAIWA_MODE)
	{
		kaiwa_system_SDL_draw();					/* �V�i���I��b SDL �`��(�x��) */
		kaiwa_system_execute_move_only_main();		/* �V�i���I��b ����(�ړ�) */
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
		/* [B] ���ꏈ���C�x���g���������Ă���ꍇ�A���ꏈ�����s���B */
		#if (1)
		/* ���ꏈ��(���܂ɂ������s���Ȃ�����)
			�R�Avoid shooting_game_core_work(void)�֐��̃T�C�Y���傫���ƁA
			CPU�̖��߃L���b�V�����t���[���鎖�ɂ�菈����������̂ŁA
			���ꏈ��(���܂ɂ������s���Ȃ�����)�̓R�A�̊O�ɒǂ��o���܂��B
			���̍ہu static�֐��ɂ��Ȃ��v�l�ɒ��ӂ��܂��B
			static�֐��ɂ���ƁAGCC������� __inline__ �֐��ɕϊ������(-O3�̏ꍇ)
			�ǂ��o�����Ӗ��������Ȃ�܂��B(�C�����C���W�J�����)
		 */
		if (cg.state_flag & (
			STATE_FLAG_10_IS_LOAD_KAIWA_TXT |	//
			STATE_FLAG_12_END_KAIWA_MODE |		//
			STATE_FLAG_06_IS_KAIWA_MODE |		//
			STATE_FLAG_14_DOUCHU_TUIKA))		// �����̓U�R�ǉ��K�v�Ȃ̂œ��ꏈ��
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
		if (0!=(cg.side_panel_draw_flag))
		{
			score_display();		/* �X�R�A�p�l�� SDL �`��(�x��) */
		}
		/* Gu�`�� */
		{
			vbl_draw_screen();	/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
		}
		/* �V���[�e�B���O�Q�[�����́A��Ƀ|�[�Y�\�B */
		if (0==(psp_pad.pad_data_alter & PSP_KEY_PAUSE))/* �������|�[�Y��������ĂȂ��� */
		{
			if (psp_pad.pad_data & PSP_KEY_PAUSE)/* ���|�[�Y�������ꂽ�� */
			{
				cb.main_call_func			= pause_menu_start;
				cb.pause_out_call_func		= shooting_game_core_work;/* �|�[�Y���A��̖߂������߂� */
			}
		}
		/* �Q�[���R�A����O�ɏo��ƁA(CPU�̖��߃L���b�V��������̂�(?))�A�ɒ[�ɑ��x�ቺ����B
			�K�v�Ȃ��ꍇ�͊O�ɏo�Ȃ��ׂ̏��u */
		if (shooting_game_core_work == cb.main_call_func)
		{
			goto my_game_core_loop;
		}
		/* �����ɗ�����A�O�ɏo��̂Œx���Ȃ�Ƃ����� */
	}
}
