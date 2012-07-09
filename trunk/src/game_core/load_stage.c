
/*(�J�[�h�V�X�e���������p)*/
#include "./boss/boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�X�e�[�W�G�f�[�^�[(dat)�̓ǂݍ���
---------------------------------------------------------*/

#include "menu/kaiwa_sprite.h"

global GAME_COMMAND *stage_command_table = NULL;

/*---------------------------------------------------------
	�q�֐�
	�O��܂Ŋm�ۂ���������������ΊJ������B
	Initial task, "free"ed the altenative entris.
---------------------------------------------------------*/
//#include <malloc.h>/* free(); */

static void load_stage_free_entry(void)
{
	GAME_COMMAND *d = stage_command_table;
	GAME_COMMAND *n;
	while (NULL != d)
	{
		n = d->next;
		free (d);
		#if 0
		d = NULL;/* �O�̈חv��H */
		#endif
		d = n;
	}
	stage_command_table = (GAME_COMMAND *)NULL;/* head to set NULL */
}

/*---------------------------------------------------------
	�q�֐�
---------------------------------------------------------*/

enum
{
	PARAM_00_time60 = 0,			//	u32 time60; 			/* �o������(1/60�b�P��) */
	PARAM_01_user_i_code,			//	int user_command;		/* �P�����R�}���h(�G�⃁�b�Z�[�W���̎��) */
	PARAM_02_user_255_code, 			//	int move_type;			/* (�U�R�G�A�ړ������̎��) */
	PARAM_03_user_select_gazou, 	//	int select_gazou;		/* �A�j��/�摜��� */
	PARAM_04_user_kougeki_type, 	//	int user_kougeki_type;	/* �G���U������ꍇ�̃J�[�h�̎�� */
	PARAM_05_user_locate_x, 		//	int user_locate_x;		/* �����p�����[�^�[�P(�o���w���W�Ȃ�) */
	PARAM_06_user_locate_y, 		//	int user_locate_y;		/* �����p�����[�^�[�Q(�o���x���W�A�G��x�Ȃ�) */
	PARAM_07_user_angle1024,		//	int user_angle1024; 	/* �����p�����[�^�[�P(�p�x[deg1024]) */
	PARAM_08_user_speed256, 		//	int user_speed256;		/* �����p�����[�^�[�Q(���x[t256]) */
	PARAM_09_user_hp,				//	int user_hp;			/* �����p�����[�^�[�Q(�G�̗�) */
	PARAM_10_user_item8,			//	int user_item8; 		/* �����p�����[�^�[�Q(�|������ɏo��A�C�e��) */
	PARAM_11_user_score,			//	int user_score; 		/* �����p�����[�^�[�Q(�|������ɓ����链�_) */
	PARAM_99_MAX/*�ő吔*/
};


/*---------------------------------------------------------
	�q�֐�
	Get ascii a interger number.
---------------------------------------------------------*/

#if (1)
/*static*/ int is_digit_or_hifun(char *read_str_position)
{
	#if 0
	/* �����ɂ��Ή� / isdi git : �������ǂ����̔��� */
	if (
		(isdi git((int)((char)(*read_str_position)))) /* gcc 4.3.5 */
		||
		(((char)'-')==(*read_str_position))
	)
	{
		return (1/*TRUE*/);
	}/*ok!==*/
	//else
	{
		return (0/*FALSE*/);/*NG!,*/
	}/*(ng!)*/
	#else
	if (('9') < (*read_str_position))
	{
		goto my_false;/*NG!,*/
	}
	//else
	if (('0') > (*read_str_position))
	{
		/*('-'�̉\��������B)*/
		if ('-'!=(*read_str_position))
		{
			goto my_false;
		}
	}
//	my_ok:
		return (1/*TRUE*/);
	my_false:
		return (0/*FALSE*/);/*NG!,*/
	#endif
}
#endif
/*[is_digit_or_hifun();'����'��������'-'�L���̏ꍇ�B�ł��邩�𒲂ׂ�B]*/
static char *load_stage_get_int(char *read_str_position, int *number)
{
	char tmp_str32bytes[32];/*(atoi�̏ꍇ�A�o�b�t�@��32[bytes]==�ő�31�����ɓ���)*/
	char *tmp_str_position = tmp_str32bytes;
	int i = 0;
	while ( is_digit_or_hifun(read_str_position) )/*(�����ɑΉ�����)*/
	{
		i++;
		if (i >= 32)/*128*/
		{	goto ne222;}
		*tmp_str_position++ = *read_str_position++;
	}
	*tmp_str_position = 0;
	*number = atoi(tmp_str32bytes);
	return (read_str_position);
/*error*/
ne222:
	return ((char *) NULL);
}

#include "111_my_file.h"

/*---------------------------------------------------------
	�X�e�[�W�ǂݍ���
	-------------------------------------------------------
	���̊֐��́A�d�l��u�ƂĂ������������܂��v
	�� libpng �� jpeglib �̉摜�W�J�A(����͎d���Ȃ��A�ڈ�200�t���[������)
	�� �ȃf�[�^�[�̓W�J�A(ogg�ɂ���ƌy�������͗l�A�ڈ�40�t���[������)
	�� �X�e�[�W�f�[�^�[�̃p�[�V���O(KETM���͂����Ԍy���A�ڈ�10�t���[�������A
	FILE�\����fopen()�Ƃ�fgets()�Ƃ��g������x���BsceIoOpen�Ƃ�sceIoRead�Ƃ��g���΂����炵��)
	��̑Ή��������ƁA��܂ŏ�����������̂ŁA
	��̑΍􂪕K�{�ł��B�i���ʂȎ��ԑ҂����j
---------------------------------------------------------*/

extern void set_default_bullet_clip(void);

extern int select_player;

extern void stage_bg_load_texture(void);
extern void gu_set_bg_u32_clear_color(u32 set_u32_clear_color);
extern void spell_cpu_douchuu_init(void);

extern void teki_random_item_table_initialize(void);/*int set_seed*/

#if (1==USE_AFTER_LOAD_STAGE)
#else
global void load_stage_make_filename(void)
{
//	char my_file_common_name[128];
//	sp rintf(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/" "stage%02d.dat", cg.game_now_stage);
//	sp rintf(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/" "stage%01d.txt", cg.game_now_stage);
//	sp rintf(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/" "stage%c.txt", ('0'+ cg.game_now_stage) );
//	sp rintf(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/" "stage%c.txt", ('0'+ cg.game_now_stage) );
	strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR "/kaiwa/s/" "stageZ.txt");
	/*(stage'Z'�̕����̈ʒu��+14������)*/
	#define DIRECTRY_DOUCHU_OFFSET_LENGTH		(14)
	my_file_common_name[DIRECTRY_DOUCHU_OFFSET_LENGTH+DIRECTRY_NAME_DATA_LENGTH] = get_stage_chr(cg.game_now_stage); /*load_stage_number*/
	//
}
#endif

/*---------------------------------------------------------
	�q�֐�
---------------------------------------------------------*/

/* Do set the entry. */
static void load_stage_add_entry(const int *c_pn)		// �I�u�W�F�N�g�̐���
{
	GAME_COMMAND *new_entry;
	new_entry						= my_calloc(sizeof(GAME_COMMAND));
	#define MUSIC_CONVERT_TIME		(10)
	/* 10[frame](==0.1666[�b]���炷�B)�ǂݍ��݂ƕϊ��Ɏ��Ԃ������鎖���l�����āA���Ԃ����炷�B */
	new_entry->v_time				= MUSIC_CONVERT_TIME+(c_pn[PARAM_00_time60]);/* ���X�t���[���P�� */
//
	new_entry->user_i_code			= (c_pn[PARAM_01_user_i_code]&0x07);
	new_entry->user_255_code		= (u8)((c_pn[PARAM_02_user_255_code])&0xff);
//
	new_entry->user_select_gazou	= c_pn[PARAM_03_user_select_gazou];
	new_entry->user_kougeki_type	= c_pn[PARAM_04_user_kougeki_type];
	new_entry->user_locate_x		= c_pn[PARAM_05_user_locate_x];
	new_entry->user_locate_y		= c_pn[PARAM_06_user_locate_y];
	new_entry->user_angle1024		= c_pn[PARAM_07_user_angle1024];
	new_entry->user_speed256		= c_pn[PARAM_08_user_speed256];
	#if 1
	new_entry->user_hp				= ((4)*c_pn[PARAM_09_user_hp]);/*(r36�Ƃ肠����)*/
	//
//�f�o�b�O//	new_entry->user_hp				= (1);/*(r39�Ƃ肠����)*/
	#endif
	new_entry->user_item8			= c_pn[PARAM_10_user_item8];
	new_entry->user_score			= c_pn[PARAM_11_user_score];
//
	if (NULL==stage_command_table)	// �ŏ��̉�͍s���H
			{	new_entry->next = (GAME_COMMAND *)NULL;}
	else	{	new_entry->next = stage_command_table;	}	// stage_command_table�͑O��̃f�[�^
	stage_command_table = new_entry;		// stage_command_table�ɍ����������f�[�^�̃A�h���X�����B
	// next�ɂ�dat�t�@�C���I�ɂ͌��ݑ������̍s�̏�̍s�������Ă���B
}

global void load_stage_data(void)
{
//	/*FILE*/char *fp;
	if (NULL==(/*fp =*/my_file_fopen()))	/* �J���Ȃ������Ƃ� */	/*my_file_common_name*/ /*,"r"*/
	{	/* �ǂݍ��ݎ��s */
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
			"load stage: %d �ʂ̓����G�ݒ�" "\\n"
			"�t�@�C��������܂���B" "\\n"
			"%s",
			/*load_stage_number*/cg.game_now_stage,
			my_file_common_name );
	}
//
	load_stage_free_entry();	/* �O��܂Ŋm�ۂ���������������΁A��ɊJ������B */
	int readed_lines		= 0;	/* �L���s���̒��� */
	{
		MY_FILE_LOOP_BEGIN;
		//
		#if (1==USE_PARTH_DEBUG)
			/*(USE_PARTH_DEBUG 1:�Ӗ��̉�����Ȃ��s���������ꍇ�́A�Q�[�����~�B�f�o�b�O�p)*/
			#define GOTO_ERR_WARN goto err_warn
		#else
			/*(USE_PARTH_DEBUG 0:�Ӗ��̉�����Ȃ��s���������ꍇ�́A���̍s�𖳎����ăQ�[���p���B�W��)*/
			#define GOTO_ERR_WARN MY_FILE_LOOP_CONTINUE
		#endif
		/* parth start. �\����͊J�n */
		{
			int c_pn[PARAM_99_MAX];/*6*/
			#if 1
			{
				int iii;
				iii = PARAM_00_time60;
		loop_iii_start:
				ch = load_stage_get_int(ch, &c_pn[iii]);
				/* �f�[�^�[�̒l�������ꍇ�̓G���[ */
				if (NULL == ch) 	{	GOTO_ERR_WARN;	}
				//
				if (PARAM_11_user_score==iii)
				{
					goto loop_iii_end;
				}
				iii++;
				/* �f�[�^�[�̋�؂�� ',' �������ꍇ�̓G���[ */
				if (',' != *ch++)	{	GOTO_ERR_WARN;	}
				goto loop_iii_start;
			}
		loop_iii_end:
			#endif
			/* do set register entry. �ǂݍ��ݐ��������f�[�^�[���A�ǉ��o�^����B */
			/* �ǉ��o�^���� */
			load_stage_add_entry(c_pn);
		}
		readed_lines++; 	/* ���ۂɓǂ߂��L���s�� */
		MY_FILE_LOOP_CONTINUE;
	#if (1==USE_PARTH_DEBUG)
	err_warn:
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
			"load stage: �����G�ݒ�t�@�C���́A" "\\n"
			"%d�s�ڂ͈Ӗ�������܂���B" "\\n"
			"%s",
			debug_number_of_read_line,
			my_file_common_name);
		MY_FILE_LOOP_CONTINUE;
	#endif
		MY_FILE_LOOP_END;
	}
	my_file_fclose(/*fp*/);
	if (0==readed_lines)		/* �L���s�����Ȃ���΃G���[ */
	{
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
			"load stage: STAGE%d.TXT �͓����G�ݒ�" "\\n"
			"�t�@�C���ł͂���܂���B", cg.game_now_stage);
	}
}


/*---------------------------------------------------------
	�X�e�[�W�ǂݍ��ݑO�̏�����
	-------------------------------------------------------
	�X�e�[�W�ǂݍ��݂��O�ɏ��������镔���B
---------------------------------------------------------*/

global void load_stage_init(void)
{
	set_default_bullet_clip();		/* �e�͈̔͂�W���ɐݒ� */
	#if (1)
	spell_cpu_douchuu_init();		/* �e�����R�[���o�b�N�𓹒��Ƌ��p���Ă���s���ケ���ɕK�v�B */
	card.spell_used_number		= SPELL_00;/*(�����R�[���o�b�N�L���t���O�Ƌ��p)*/
	#endif
	;/* ���v���C�Ή��o����悤�ɁA�����n��̏������B */
	teki_random_item_table_initialize();/*set_seed*/	/* �����_���A�C�e���G�̏ꍇ�Ɏg���A���ʔ����e�[�u�����������B */
	cg.game_now_stage++;
	{
		/*(�K�������t���O)*/
		{
			/*(�A�C�e���������W�t���O��߂�)*/
			/* �������W���[�h�t���O��S�ď����B */
			cg.state_flag &=
			(	~(	  JIKI_FLAG_0x0010_JYOU_BU_SYUU_SYUU					/* �I��� */	/* MAX���̏��� */
					| JIKI_FLAG_0x0020_BOMBER_SYUU_SYUU 					/* �I��� */	/* �{���������̂݉�� */
					| JIKI_FLAG_0x0040_BOSS_GO_ITEM_JIDOU_SYUU_SYUU 		/* �I��� */	/* �{�X���j���̈ꎞ��� */
				)
			);
		}
		cg.side_panel_draw_flag 		= (1);/*(�E�p�l���\��on)*/
		cg.player_option_mode			= (0);/* �ǂ����ŏ��������K���K�v */

		kaiwa_all_obj_draw_on_off(0);	/* �����G��`�悵�Ȃ��B */
	//	player_option_position_initilize(); 	/*(�I�v�V�����̈ʒu������������)*/
	}
	{
		const u32 bg_color_list[8] =
		{	/*AABBGGRR*/
			0xffaa9999,/*extra_stage(����)*/		//	0xff000000 (��)
			0xffaa9999,/*1��(�����A���@�̐X)*/		//	0xffcdb4b9	0xff106010, (�΁A���@�̐X)
			0xff401010,/*3��(�A�����̒|��)*/		//	0xff401010	//	0xff104010,/*3��(��)*/
			0xff402020,/*4��(�A�i����)*/			//	0xff402020
			0xff804010,/*2��(���A�d���̎R�A��)*/	//	0xff804010	//	0xff802010,/*2��(��)*/		0xff102080,
			0xff601030,/*5��(�A�}����)*/			//	0xff601030
			0xff1311ee,/*6��(�ԁA�g����)*/			//	0xff333af4	0xff301060	//	0xff601030,/*6��(��)*/
			0xff000000,/*ending(��)*/				//
		//	0xff601010,/**/
		};
		gu_set_bg_u32_clear_color((bg_color_list[cg.game_now_stage&0x07]));
	}
	stage_bg_load_texture();
}
