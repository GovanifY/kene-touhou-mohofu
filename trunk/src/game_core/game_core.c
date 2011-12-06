
/*(�����R�[���o�b�N�ŃJ�[�h�V�X�e�����g��)*/
#include "./boss/boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
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
//#include "./menu/kaiwa_sprite.h"

/*---------------------------------------------------------
	�Q�[���R�}���h�̐V�K�ǉ��B
	-------------------------------------------------------
	�����ɒǋL����ƃV���[�e�B���O�Q�[���{�̂��x���Ȃ�̂ŒǋL���܂���B
	������(parth)���� load_stage.c �ŗ\�ߍς܂��Ă����܂��B
---------------------------------------------------------*/

extern void game_command_04_game_all_clear( 		GAME_COMMAND *l);/* �S�ʃN���A�[�̏ꍇ�A���̃Q�[���R�}���h��ǉ� */
extern void game_command_03_bg_control( 			GAME_COMMAND *l);/* �w�i�R���g���[��(�X�N���[�����x�����w��) */
extern void game_command_02_kaiwa_start_boss(		GAME_COMMAND *l);/* �{�X�A�V�i���I��b�N�� */
extern void game_command_01_regist_chuu_boss(		GAME_COMMAND *l);/* [���^�G]�d���A�������� */
extern void game_command_00_regist_zako(			GAME_COMMAND *l);/* �U�R�A�������� */

/*---------------------------------------------------------

---------------------------------------------------------*/

static u32 game_v_time;/* �Q�[������ game frame time counter. */

#if (1==USE_HOLD_GAME_MODE)
static int v_time_hold_mode;/* ���p�Ɏ~�߂��蓮��������o����悤�ɂ��Ƃ� */
#endif /* (1==USE_HOLD_GAME_MODE) */

global void init_stage_start_time(void)
{
//	stage_start_time = psp_get_uint32_ticks();
	game_v_time = 0;
}
/*static*/global void set_core_game_time_MAX(void)
{
	game_v_time = 65535;/* �K���ɑ傫�Ȓl[frame](65535[frame]==��18[��]==18.xxx x 60 x 60 ) */
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

	#if (1==USE_AFTER_LOAD_STAGE)
	#else
extern void load_stage_make_filename(void);
extern void load_stage_data(void);
	#endif

extern void set_rnd_seed(int set_seed);
extern void load_stage_init(void);

extern void player_init_stage(void);

global MAIN_CALL_FUNC(common_load_init)
{
	//
	set_rnd_seed(cg.game_now_stage);	/* �����n��̏����� */
//
	#if (1==USE_r36_SCENE_FLAG)
	/*(���d�l�A�X�e�[�W�^�C�g���\���͂܂�����ĂȂ��̂ŋ����I�ɕ\����̏�Ԃɂ���)*/
	/* off / �����R�}���h�ǉ��ǂݍ��ݏ������~����B */
//	cg.state_flag		&= (~SCENE_NUMBER_MASK); 	/*(�V�[��������)*/
	cg.state_flag		&= (0xffff00ffu); 	/*(�V�[��������)*/
//	cg.state_flag		|= (SCENE_NUMBER_0x0400_DOUCHUU); // �X�e�[�W�^�C�g���\����̏�Ԃɂ���B
	cg.state_flag		|= (SCENE_FLAG_0x0100_KAIWA_LOAD); // �^�C�g���ǂ݂��݃e�X�g
	#endif
	/*(�G����~)*/
	//set_core_game_time_MAX();

	/* Load stage init */
	load_stage_init();//	�X�e�[�W�ǂݍ��݂��O�ɏ��������镔���B
	#if (1==USE_AFTER_LOAD_STAGE)
	#else
	{
		load_stage_make_filename();
		load_stage_data();
		init_stage_start_time();	// ���[�h��Ɏ��Ԃ��č쐬����B
	}
	#endif
//
	player_init_stage();/* �X�e�[�W�J�n���̂ݎ኱�̖��G��ԂɃZ�b�g */
	//
	kanji_init_standard();/*(�����֘A�̕W��������)*/
	//
	play_music_num(BGM_00_stop);/*(����܂Ŗ��Ă���BGM�̋�����~[��蒼�����͎n�߂���BGM���Đ�����])*/
	//
	#if (1==USE_HOLD_GAME_MODE)
	hold_game_mode_off();/* �Q�[�����Ԃ̓���J�n */
	#endif /* (1==USE_HOLD_GAME_MODE) */
	//
	#if (1)/*(�Q�[�����n�܂�O��SDL��ʂ�����)*/ /*Gu������������v��Ȃ��Ȃ�*/
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
global MAIN_CALL_FUNC(stage_first_init)
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
							game_command_00_regist_zako,			/* 0"�d��" �U�R��ǉ�����B */
							game_command_01_regist_chuu_boss,		/* 1"���@" ���{�X��ǉ�����B (����G[���^�G]) */
							game_command_02_kaiwa_start_boss,		/* 2"��b" �{�X��ǉ�����(r35)�B���� �{�X�̃V�i���I��b���N������B(�V�i���I��b���Ń{�X�ǉ�) */
							game_command_03_bg_control, 			/* 3"BG" �w�i�̐���R�}���h�B */
							//
							game_command_04_game_all_clear, 		/* 4"QUIT" �Q�[�� �S�X�e�[�W �N���A�[���w���B */
							game_command_04_game_all_clear, 		/* (.align)�_�~�[�B */		//	NULL,/* [�ԕ���؂�] (�ŕK�v)*/
							game_command_04_game_all_clear, 		/* (.align)�_�~�[�B */		//	NULL,/* [�ԕ���؂�] (�ŕK�v)*/
							game_command_04_game_all_clear, 		/* (.align)�_�~�[�B */		//	NULL,/* [�ԕ���؂�] (�ŕK�v)*/
						};
						(*bbb[ (int)(aaa->user_i_code) ])(aaa); 	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
					}
				}
			}
			aaa = aaa->next;	/* ���𒲂ׂ� */	/* choice alter. */
		}
	}
}


/*---------------------------------------------------------
	����C�x���g�̎��s����
	���ꏈ��(���܂ɂ������s���Ȃ�����)
	-------------------------------------------------------
	-------------------------------------------------------
	�R�A(���C�����[�v��)�ɒǉ�����ƁA�V���[�e�B���O�Q�[���{�̂��x���Ȃ�̂ŁA
	���삪�x�����ăQ�[���ɁA�Ȃ�Ȃ��Ȃ��Ă��܂��B
	�����œ��쑬�x�I�ϓ_����A�p�x�̏��Ȃ�����@�\�͂����Ŏ��s����B
---------------------------------------------------------*/
extern void kaiwa_system_SDL_BG_draw(void);
extern void kaiwa_system_execute_move_only_main(void);
extern void kaiwa_load_ivent(void);
/* ���ӁF(���쑬�x�ቺ����̂�)static�֐��ɂ��Ȃ� */global void my_special(void)
{
	/* �����R�}���h�ǉ��ǂݍ��ݏ��� */
	if (IS_SCENE_DOUCHU_TUIKA)
	{
		game_core_teki_tuika();
	}
	/* ���u(��炢�{����t���Ԓ���)�{�X�Ƒ��ł�����ƃn���O�A�b�v�v�o�O(�`r29)�΍� */
	if (0 < /*bomb_wait*/cg.bomber_time)		/* �{���E�F�C�g���� */
	{
		return;/* �{���������͑ҋ@ */
	}
	/* �V�i���I��b�����[�h���ĊJ�n���� */
	if (IS_SCENE_KAIWA_LOAD)
	{
	//	NEXT_SCENE;/*(���̏�ʂ֐ݒ�)*/kaiwa_load_ivent();�����Ői�߂�B�̂Ń_�u��B
		kaiwa_load_ivent();/*(�V�i���I��b�����[�h���ĊJ�n����)*/
	}
	#if (1==USE_r36_SCENE_FLAG)
	else
	#endif
	/*(��b���[�h�������I������H)*/
	if (IS_SCENE_END_KAIWA_MODE)
	{
		NEXT_SCENE;/*(���̏�ʂ֐ݒ�)*/ //�_�u��B�H�H�H�H�H�H�H�H�H�H�H�H�H�H�H
		//inc liment_scene();/*(���̏�Ԃɐi�߂�)*/
		/* (�V�i���I��b�������I������̂�)
			�C�x���g�V�[�������ɐi�߂�B */
		{
			set_kanji_hide_line(ML_LINE_02);/*(2�s�ڈȉ����\���ɂ���B)*/
			if (0x0c00==((cg.state_flag & SCENE_NUMBER_MASK)))/*(good end �̏ꍇ)*/
			{
				cg.state_flag += 0x0400;/*(�{�X�I����֐i�߂�)*/
			}
		//	if (0x1000==((cg.state_flag & SCENE_NUMBER_MASK)))
			if (0x1000u <= ((unsigned)(cg.state_flag & SCENE_NUMBER_MASK)))
			{	/*(�u�{�X�퓬��̉�b���I��������ԁv�̏ꍇ)*/
				cb.main_call_func = stage_clear_result_screen_start;	/* �X�e�[�W�N���A�[���̃��U���g��� */
			}
		}
	}
	#if (1==USE_r36_SCENE_FLAG)
	else
	#endif
	/* ��b���[�h���삪�K�v�H */
	if (IS_SCENE_KAIWA_MODE) 	// ��b���[�h���삪�K�v�H
	{
		kaiwa_system_SDL_BG_draw(); 				/* �V�i���I��b�V�X�e�� SDL_BG �`��(�x��) */
		kaiwa_system_execute_move_only_main();		/* �V�i���I��b�V�X�e�� ����(�ړ�) */
	}
}


/*---------------------------------------------------------
	�e�����C���[�V�X�e��
	-------------------------------------------------------
	���e���ɒe���R���g���[��(�R�[���o�b�N)��o�^���Ă����A
	����𑀍삷��B
---------------------------------------------------------*/

/*global*/static void game_core_danmaku_layer_system(void)
{
	unsigned int jj;
	for (jj=0; jj<DANMAKU_LAYER_04_MAX; jj++)/*(�o�^��S�����ׂ�B)*/
	{
		/*(���C���[���o�^����Ă��Ȃ��ꍇ�͉������Ȃ��B)*/
		if (NULL != card.danmaku_callback[jj])
		{
			unsigned int ii;
			for (ii=0; ii<OBJ_POOL_00_TAMA_1024_MAX; ii++)/*(�e��S�e���ׂ�B)*/
			{
				OBJ *obj;
				obj = &obj99[OBJ_HEAD_00_0x0000_TAMA+ii];
				if (ATARI_HANTEI_OFF != (obj->atari_hantei))/*(�����蔻��̖����e�͑ΏۊO�B���e�G�t�F�N�g�p�B)*/
				{
					if (jj == ((obj->hatudan_register_spec_data)&(0x03)) )	/* �e�����C���[[n]�Ȃ� */
					{
						(card.danmaku_callback[jj])(obj);
					}
				}
			}
		}
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
extern void bg_move_all(void);
extern void draw_SDL_score_chache(void);
global MAIN_CALL_FUNC(shooting_game_core_work)
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
			�R�A MAIN_CALL_FUNC(shooting_game_core_work)�֐��̃T�C�Y���傫���ƁA
			CPU�̖��߃L���b�V�����t���[���鎖�ɂ�菈����������̂ŁA
			���ꏈ��(���܂ɂ������s���Ȃ�����)�̓R�A�̊O�ɒǂ��o���܂��B
			���̍ہu static�֐��ɂ��Ȃ��v�l�ɒ��ӂ��܂��B
			static�֐��ɂ���ƁAGCC������� __inline__ �֐��ɕϊ������(-O3�̏ꍇ)
			�ǂ��o�����Ӗ��������Ȃ�܂��B(�C�����C���W�J�����)
		 */
		if (0x0800!=(cg.state_flag & (0xff00))) 	// �{�X�퓬���ȊO�̏�ԂȂ�΁A���ׂē��ꏈ�����K�v�B
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
		bg_move_all();/* �w�i�̈ړ����� */
		game_core_danmaku_layer_system();/*(�e�����C���[�V�X�e��)*/
		obj_system_move_all();/*(�X�v���C�g�I�u�W�F�N�g�̈ړ�����)*/
		/* [D] �`�� */
		// ���̕ӂ͑��x�ቺ����̂ŃR�[���o�b�N�ɂ��ׂ�
		if (0!=(cg.side_panel_draw_flag))
		{
			score_display();/* �X�R�A�p�l�� SDL �`��(�x��) */
		}
		/* Gu�`�� */
		{
			vbl_draw_screen();/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
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
