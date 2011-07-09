
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�X�e�[�W�G�f�[�^�[(dat)�̓ǂݍ���
---------------------------------------------------------*/

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
	PARAM_00_time60 = 0,		//	u32 time60; 			/* �o������(1/60�b�P��) */
	PARAM_01_user_command,		//	int user_command;		/* �P�����R�}���h(�G�⃁�b�Z�[�W���̎��) */
	PARAM_02_user_select_gazou, //	int select_gazou;		/* �A�j��/�摜��� */
	PARAM_03_user_kougeki_type, //	int user_kougeki_type;	/* �G���U������ꍇ�̃X�y�J�̎�� */
	PARAM_04_user_x,			//	int user_x; 			/* �����p�����[�^�[�P(�o���w���W�Ȃ�) */
	PARAM_05_user_y,			//	int user_y; 			/* �����p�����[�^�[�Q(�o���x���W�A�G��x�Ȃ�) */
	PARAM_06_user_hp,			//	int user_hp;			/* �����p�����[�^�[�Q(�o���x���W�A�G��x�Ȃ�) */
	PARAM_07_user_item8,		//	int user_item8; 		/* �����p�����[�^�[�Q(�o���x���W�A�G��x�Ȃ�) */
	PARAM_08_user_score,		//	int user_score; 		/* �����p�����[�^�[�Q(�o���x���W�A�G��x�Ȃ�) */
	PARAM_99_MAX/*�ő吔*/
};

/*---------------------------------------------------------
	�q�֐�
---------------------------------------------------------*/
extern void load_bg2_chache(char *filename, int use_alpha);
/* Do set the entry. */
static void load_stage_add_entry( char *user_string, const int *c_pn )		// �I�u�W�F�N�g�̐���
{
	GAME_COMMAND *new_entry;
	new_entry						= my_calloc(sizeof(GAME_COMMAND));
	#define MUSIC_CONVERT_TIME		(10)
	/* 10[flame](==0.1666[�b]���炷�B)�ǂݍ��݂ƕϊ��Ɏ��Ԃ������鎖���l�����āA���Ԃ����炷�B */
	new_entry->v_time				= MUSIC_CONVERT_TIME+(c_pn[PARAM_00_time60]);/* ���X�t���[���P�� */
	new_entry->user_select_gazou	= c_pn[PARAM_02_user_select_gazou];
	new_entry->user_kougeki_type	= c_pn[PARAM_03_user_kougeki_type];
	new_entry->user_x				= c_pn[PARAM_04_user_x];
	new_entry->user_y				= c_pn[PARAM_05_user_y];
	new_entry->user_hp				= c_pn[PARAM_06_user_hp];
	new_entry->user_item8			= c_pn[PARAM_07_user_item8];
	new_entry->user_score			= c_pn[PARAM_08_user_score];
//
	new_entry->user_255_code		= (u8)((c_pn[PARAM_01_user_command])&0xff);
	new_entry->user_i_code			= 0;

	strncpy(new_entry->user_string, user_string, (MAX_PARA1_36-1)/*63*/);

/* �Q�[�����́A���ԃR�[�h�`���̃R�}���h�������\����͂͂��Ȃ��B(�\����͂�load_stage.c�ōs��) */

	const char *ctype_name[GC_CODE_MAX] = 	/* (r34)���� 6 ��� */
	{
		NULL,/* (sjis-text�\��) [�ԕ���؂�] */
	//	���̑�
		"QUIT", 	/* �Q�[�� �S�X�e�[�W �N���A�[ */
		"BG",		/*	"BG_CONTROL"*/		/* ���V�X�e���R�}���h�Ȃ̂ŉp��ɂ��� */
	// �G
		"�{�X", 	/* (�{�X / ��-�{�X)�p �X�N���v�g�N�� */
		"���{�X",	/* �U�R��-�{�X(�X�N���v�g�N�����Ȃ�) */
		"�U�R", 	/* �U�R */
	};
//
	/* �ǂݍ��񂾃R�}���h�𒆊ԃR�[�h�`���ɕϊ����� */
	{
		for (new_entry->user_i_code = (GC_CODE_MAX-1);
			(0) < new_entry->user_i_code;
			new_entry->user_i_code--)
		{
			if (0==tiny_strcmp( new_entry->user_string, /*(char *)*/&/*btype_name*/ctype_name[(unsigned int)new_entry->user_i_code][0] ) )
			{
				break;
			}
		} // 0/*CT YPE_00_unknown*/ == aaa );
	}
//
	if (NULL==stage_command_table)	// �ŏ��̉�͍s���H
	{	new_entry->next = (GAME_COMMAND *)NULL;}
	else
	{	new_entry->next = stage_command_table;	}	// stage_command_table�͑O��̃f�[�^
	stage_command_table = new_entry;		// stage_command_table�ɍ����������f�[�^�̃A�h���X�����B
	// next�ɂ�dat�t�@�C���I�ɂ͌��ݑ������̍s�̏�̍s�������Ă���B
}


/*---------------------------------------------------------
	�q�֐�
	Get ascii a interger number.
---------------------------------------------------------*/

static char *load_stage_get_int(char *ccc, int *number)
{
	char buffer[32];/*128*/
	char *ddd = buffer;
	int i = 0;
//	while (isdigit(*ccc))			/* isdigit : �������ǂ����̔��� */
	while (
		(isdigit((int)((char)(*ccc))))	/* gcc 4.3.5 */
		||
		(((char)'-')==(*ccc))
	)		/* �����ɂ��Ή� / isdigit : �������ǂ����̔��� */
	{
		i++;
		if (i >= 32)/*128*/
		{	goto ne222;}
		*ddd++ = *ccc++;
	}
	*ddd = 0;
	*number = atoi(buffer);
	return (ccc);
/*error*/
ne222:
	return ((char *) NULL);
}
/*
gcc 4.3.5	warning: array subscript has type 'char'
isdigit( int );
*/

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
extern void bg2_start_stage(void);
extern void stage_bg_load_texture(void);
extern void gu_set_bg_u32_clear_color(u32 set_u32_clear_color);

extern void teki_random_item_table_initialize(void);/*int set_seed*/
global void load_stage(void)
{
	set_default_bullet_clip();		/* �e���͈̔͂�W���ɐݒ� */
	bg2_start_stage();
	;/* ���v���C�Ή��o����悤�ɁA�����n��̏������B */
	teki_random_item_table_initialize();/*set_seed*/	/* �����_���A�C�e���G�̏ꍇ�Ɏg���A���ʔ����e�[�u�����������B */
	cg.game_now_stage++;
	// change music soundtrack
	play_music_num(cg.game_now_stage);	/* n�ʓ��� */
	{
		/* (r32) */cg.state_flag		|= STATE_FLAG_14_ZAKO_TUIKA;	/* �G���ǉ�����������ꍇon */
		/* (r32) */cg.state_flag &= (~(STATE_FLAG_05_IS_BOSS));/*�{�Xoff*/
		cg.side_panel_draw_flag = (1);	/* �p�l���\��on */	/* cg.state_flag |= ST ATE_FLAG_09_IS_PANEL_WINDOW */
	}
	{
		const u32 bg_color_list[8] =
		{	/*AABBGGRR*/
			0xff000000,/*(��)*/
			0xff106010,/*1��(�΁A���@�̐X)*/
			0xff401010,/*3��(�A�����̒|��)*/		//	0xff104010,/*3��(��)*/
			0xff402020,/*4��(�A�i����)*/
			0xff804010,/*2��(���A�d���̎R�A��)*/	//	0xff802010,/*2��(��)*/		0xff102080,
			0xff601030,/*5��(�A�}����)*/
			0xff301060,/*6��(�ԁA�g����)*/			//	0xff601030,/*6��(��)*/
			0xff000000,/*ending(��)*/
		//	0xff601010,/**/
		};
		gu_set_bg_u32_clear_color((bg_color_list[cg.game_now_stage&0x07]));
	}
	stage_bg_load_texture();
//
//	char my_file_common_name[128];
//	sp rintf(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/stage%02d.dat", cg.game_now_stage);
//	sp rintf(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/stage%01d.txt", cg.game_now_stage);
//	sp rintf(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/stage%c.txt", ('0'+ cg.game_now_stage) );
//	sp rintf(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/stage%c.txt", ('0'+ cg.game_now_stage) );
	strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/stageZ.txt");
	my_file_common_name[10+DIRECTRY_NAME_DATA_LENGTH] = get_stage_chr(cg.game_now_stage); /*load_stage_number*/

//	/*FILE*/char *fp;
	if (NULL==(/*fp =*/my_file_fopen()))	/* �J���Ȃ������Ƃ� */	/*my_file_common_name*/ /*,"r"*/
	{	/* �ǂݍ��ݎ��s */
	//	error(ERR_FATAL, (char*)"can't read stage data %s\nerrno: %d (%s)",my_file_common_name,errno,strerror(errno));
		error(ERR_FATAL, (char*)"can't read stage %d data %s\n", /*load_stage_number*/cg.game_now_stage, my_file_common_name );
	}
//
	load_stage_free_entry();	/* �O��܂Ŋm�ۂ���������������΁A��ɊJ������B */
	int entrys		= 0;		/* �L���s���̒��� */
	{
		MY_FILE_LOOP_BEGIN;
		//
		#if (1==USE_PARTH_DEBUG)
			#define GOTO_ERR_WARN goto err_warn
		#else
			#define GOTO_ERR_WARN MY_FILE_LOOP_CONTINUE
		#endif
		/* �f�[�^�[�̒l�������ꍇ�̓G���[ */
		#define CHECK_ERROR_VALUE			if (NULL == ch) 	{	GOTO_ERR_WARN;	}
		/* �f�[�^�[�̋�؂�� ',' �������ꍇ�̓G���[ */
		#define CHECK_ERROR_VALUE_KUGIRI	CHECK_ERROR_VALUE	if (',' != *ch++)	{	GOTO_ERR_WARN;	}
		/* parth start. �\����͊J�n */
		int end_arg/*=0*/;	end_arg=0;		/* [���݁[]�s�� == �����̎擾�̒��~ */
		char user_string[128];		/* ������(���b�Z�[�W��t�@�C����) */
		{
			int c_pn[PARAM_99_MAX];/*6*/
			ch = load_stage_get_int(ch, &c_pn[PARAM_00_time60]);				CHECK_ERROR_VALUE_KUGIRI	/* load int time60. */	/* �o������[�t���[��(1/60�b)�P��]�̎擾 */
			ch = load_my_file_get_str(ch, user_string, &end_arg);				CHECK_ERROR_VALUE_KUGIRI	/* load str user_string. */ 	/*, '|'*/
			ch = load_stage_get_int(ch, &c_pn[PARAM_01_user_command]);			CHECK_ERROR_VALUE_KUGIRI	/* load 1 char commnd. �P�����R�}���h */
			ch = load_stage_get_int(ch, &c_pn[PARAM_02_user_select_gazou]); 	CHECK_ERROR_VALUE_KUGIRI	/* load graphics select. */
			ch = load_stage_get_int(ch, &c_pn[PARAM_03_user_kougeki_type]); 	CHECK_ERROR_VALUE_KUGIRI	/* load graphics select. */
			ch = load_stage_get_int(ch, &c_pn[PARAM_04_user_x]);				CHECK_ERROR_VALUE_KUGIRI	/* load int user_x. �o��x���W */
			ch = load_stage_get_int(ch, &c_pn[PARAM_05_user_y]);				CHECK_ERROR_VALUE_KUGIRI	/* load int user_y. �o��y���W */
			ch = load_stage_get_int(ch, &c_pn[PARAM_06_user_hp]);				CHECK_ERROR_VALUE_KUGIRI	/* load int user_hp. �̗� */
			ch = load_stage_get_int(ch, &c_pn[PARAM_07_user_item8]);			CHECK_ERROR_VALUE_KUGIRI	/* load int user_. �o���A�C�e�� */
			ch = load_stage_get_int(ch, &c_pn[PARAM_08_user_score]);			CHECK_ERROR_VALUE			/* load int user_. ���j���̊l���X�R�A[(1/10�P��)] */
			/* do set register entry. �ǂݍ��ݐ��������f�[�^�[���A�ǉ��o�^����B */
			/* �ǉ��o�^���� */
			load_stage_add_entry( user_string, c_pn );
		}
		entrys++;		/* �L���s�� */
		MY_FILE_LOOP_CONTINUE;
	#if (1==USE_PARTH_DEBUG)
	err_warn:
		error(/*ERR_WARN*/ERR_FATAL, (char*)"syntax error in stage data '%s', line no: %d", filename, debug_number_of_read_line);
		MY_FILE_LOOP_CONTINUE;
	#endif
		MY_FILE_LOOP_END;
	}
	my_file_fclose(/*fp*/);
	if (0==entrys)		/* �L���s�����Ȃ���΃G���[ */
	{
		error(/*ERR_WARN*/ERR_FATAL, (char*)"no entrys for STAGE%d.TXT", cg.game_now_stage);
	}
	//fps_init();/* ??? auto fps������ */
}
