
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�X�e�[�W�G�f�[�^�[(dat)�̓ǂݍ���
---------------------------------------------------------*/

STAGE_DATA *stage_data_table = NULL;

/*---------------------------------------------------------
	�q�֐�
	�O��܂Ŋm�ۂ���������������ΊJ������B
	Initial task, "free"ed the altenative entris.
---------------------------------------------------------*/
//#include <malloc.h>/* free(); */

static void load_stage_free_entry(void)
{
	STAGE_DATA *d = stage_data_table;
	STAGE_DATA *n;
	while (NULL != d)
	{
		n = d->next;
		free (d);
		#if 0
		d = NULL;/* �O�̈חv��H */
		#endif
		d = n;
	}
	stage_data_table = (STAGE_DATA *)NULL;/* head to set NULL */
}

/*---------------------------------------------------------
	�q�֐�
---------------------------------------------------------*/


/*---------------------------------------------------------
	�q�֐�
---------------------------------------------------------*/
extern void load_bg2_chache(char *filename, int use_alpha);
/* Do set the entry. */
static void load_stage_add_entry(
	u32 time60,
	char *user_string,
	int user_command,
	int user_select_gazou,
	int user_x,
	int user_y,
	int user_hp,
	int user_item8,
	int user_score
	)		// �I�u�W�F�N�g�̐���
{
	STAGE_DATA *new_entry;
	new_entry						= my_calloc(sizeof(STAGE_DATA));
//	new_entry->v_time				= (time60*6/*100*1000*/);/* �ǂݍ��݌�APSP�ɓs���̗ǂ��l�ɕϊ�(1/10[sec]-> 1/1000000[sec] == 1[nsec] == nano seconds ) */
//	new_entry->v_time				= (time60*6/*100*1000*/);/* �ǂݍ��݌�APSP�ɓs���̗ǂ��l�ɕϊ�(1/10[sec]-> 1/60[sec] ) */
	new_entry->v_time				= (time60  /*100*1000*/);/* �ǂݍ��݌�APSP�ɓs���̗ǂ��l�ɕϊ�(1/10[sec]-> 1/60[sec] ) */
	new_entry->user_x				= user_x;
	new_entry->user_y				= user_y;
	new_entry->user_hp				= user_hp;
	new_entry->user_item8			= user_item8;
	new_entry->user_select_gazou	= user_select_gazou;
	new_entry->user_score			= user_score;
//	new_entry->done 				= 0;
//
	new_entry->user_255_code		= (u8)(user_command&0xff);
	new_entry->user_i_code			= 0;

	strncpy(new_entry->user_string, user_string, (MAX_PARA1_36-1)/*63*/);

/* �Q�[�����́A���ԃR�[�h�`���̃R�}���h�������\����͂͂��Ȃ��B(�\����͂�load_stage.c�ōs��) */

	//	"�d��", 	/*	extra 2 	"BOSS05",*/
	//	"�`���m",	/*	extra 1 	"BOSS05",*/
	//	"���", 	/*	stage 6 	"BOSS04",*/ 	// [***090207		�ǉ�
	//	"�p�`�F",	/*	stage 5 	"BOSS01",*/
	//	"�P��", 	/*	stage 4 	"BOSS03",*/
	//	"����", 	/*	stage 3 	"BOSS03",*/ 	/* 3�ʂ͂ǂ����邩����(�i��? �d��?) */
	//	"��",		/*	stage 2 	"BOSS02",*/
	//	"�A���X",	/*	stage 1 	"BOSS01",*/

	//	"���{�X6",
	//	"������",	/*	"GFAIRY",*/ 	//		�ǉ�
	//	"���",
	//	"�Ă�",
	//	"���{�X2",
	//	"���[�~�A", /*	"GFAIRY",*/ 	//		�ǉ�
//		�p�~
	//	"�d��2",	/*	"GFAIRY",*/ 	//		�ǉ�
	//	"�d��1",	/*	"GFAIRY",*/ 	// [***090207		�ǉ�


//
	const char *ctype_name[CTYPE_99_MAX] =	/* ���� 32 ��� */
	{
		NULL,/* [�ԕ���؂�] */
	//	���̑�
		"QUIT", 	/* �Q�[�� �S�X�e�[�W �N���A�[ */
		"BG",		/*	"BG_CONTROL"*/		/* ���V�X�e���R�}���h�Ȃ̂ŉp��ɂ��� */
	//	�{�X
		"�{�X", 	/* ���ʉ�(�{�X�X�N���v�g�N��) */
	/* ����G[���^�G] */
		"���{�X",
//
	/* �����w */
		"�����w1",	/*	"GROUNDER",*/
		"�����w2",	/*	"MAGICF",*/
	/* ���� �A�z�� */
		"�A�z��1",	/*	"PLASMABALL",*/
		"����1",	/*	"PROBALL",*/
	/* �d�� */
		"�͗d��1",	/*	"CUBE",*/
		"�Ԗы�1",	/*	"MINE",*/		/* �� */
	/* �ы� */
		"覐�1",	/*	"MING",*/		/* ���̑��U�R */
		"���ґ�2",	/*	"GREETER",*/	/* ���̑��U�R */
		"�Ζы�1",	/*	"XEV",*/
		"�Ζы�2",	/*	"CRUSHER",*/
		"�ы�1",	/*	"BADGUY",*/
		"�ы�2",	/*	"BADGUY",*/
	/* [C�d��]���̑��U�R */
		"���΂�1",	/*	"DRAGER",*/
		"���΂�2",	/*	"EYEFO",*/
		"�G1",		/*	"",*/
	/* [B�d��]���d�� */
		"���C�h1",	/*	"RWINGX",*/
		"���C�h2",	/*	"ZATAK",*/
		"���C�h3",	/*	"CURVER",*/
		"���C�h4",	/*	"CIR",*/
	/* [A�d��]���d�� */
		"�d��1",	/*	"FAIRY",*/		// [***090207		�ǉ�
		"�d��2",	/*	"SPLASH",*/ 	// [***090124		�ǉ�
		"�d��3",	/*	"SPLASH",*/ 	//		�ǉ�
		"�d��4",	/*	"SPLASH",*/ 	//		�ǉ�
	};
//
	/* �ǂݍ��񂾃R�}���h�𒆊ԃR�[�h�`���ɕϊ����� */
	if ((255)==new_entry->user_255_code)
	{
		new_entry->user_i_code = ETYPE_01_SJIS_TEXT;	/* 'T'ext. */
	}
	else
	if ((128)==new_entry->user_255_code)
	{
		new_entry->user_i_code = ETYPE_02_LOAD_BG;	/* 'B'ack 'G'round. */		/* Background */
		#if 000
	//	if ( 0 == ti ny_strcmp(new_entry->user_string,"0") ) /* �t�@�C�������O�̏ꍇ�V�X�e���R�}���h[�g���\��] */
		if ( '0' == new_entry->user_string[0] ) /* �t�@�C������1���ڂ��O�̏ꍇ�V�X�e���R�}���h[�g���\��] */
		{
			;
		}
		else
		{
			load_bg2_chache(new_entry->user_string, 0); /* �Q�[�����摜�W�J����Ə��������������̂ŃL���b�V���ɋl�߂�B */
			/* ���ӁF��i�t����Ȃ��񂾂���A�摜�W�J���C�u����(libpng�Ƃ�libjpeg)���g������A�ǂ����ŏ������������B
				(�摜�W�J���C�u����(libpng�Ƃ�libjpeg)��CPU�ɕ��ׂ�������Ȃ��悤�ɁA�ʃX���b�h�ł������W�J������@�\������)
				�܂�A�����̏ꏊ�œW�J���Ă�񂾂���A�����̏ꏊload_stage()�ŏ��������������Ƃ���������B */
		}
		#endif
	}
	else
	{
		{
			for (new_entry->user_i_code = /*CT YPE_00_unknown+*/(CTYPE_99_MAX-1); /*0*/CTYPE_00_NONE/*CT YPE_00_unknown*/ < new_entry->user_i_code; new_entry->user_i_code--)
			{
				if (0==tiny_strcmp( new_entry->user_string, /*(char *)*/&/*btype_name*/ctype_name[(unsigned int)new_entry->user_i_code][0] ) )
				{
					break;
				}
			} // 0/*CT YPE_00_unknown*/ == aaa );
		}
	}
//
	if (NULL==stage_data_table) 	// �ŏ��̉�͍s���H
	{	new_entry->next = (STAGE_DATA *)NULL;}
	else
	{	new_entry->next = stage_data_table; 	}	// stage_data_table�͑O��̃f�[�^
	stage_data_table = new_entry;		// stage_data_table�ɍ����������f�[�^�̃A�h���X�����B
	// next�ɂ�dat�t�@�C���I�ɂ͌��ݑ������̍s�̏�̍s�������Ă���B
}


/*---------------------------------------------------------
	�q�֐�
	Get ascii a interger number.
---------------------------------------------------------*/

static char *load_stage_get_int(char *ccc, int *number)
{
	char buffer[32/*128*/];
	char *ddd = buffer;
	int i = 0;
//	while (isdigit(*c)) 			/* isdigit : �������ǂ����̔��� */
	while ((isdigit(*ccc))||('-'==(*ccc)))		/* �����ɂ��Ή� / isdigit : �������ǂ����̔��� */
	{
		i++;
		if (i >= 32/*128*/)
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
extern void bg2_start_stage(void);	// [***090209		�ǉ�
extern void stage_bg_load_texture(void);
extern void gu_set_bg_u32_clear_color(u32 set_u32_clear_color);
void load_stage(void)	/*int level*/	/* ���Xint */
{
	set_default_bullet_clip();		/* �e���͈̔͂�W���ɐݒ� */
	bg2_start_stage();				// [***090209		�ǉ�
	enemy_set_random_seed(/*set_seed*/);	/* ���v���C�Ή��o����悤�ɁA�����n��̏������B */
//	int level = player_now_stage;
	pd.player_now_stage++;				/*(*level)++*/
	// change music soundtrack
	play_music_num(pd.player_now_stage);	/* n�ʓ��� */	 /*1+(*level)*/
//
//	int load_stage_number = player_now_stage;
	{
		{
			pd.state_flag &= (~(STATE_FLAG_05_IS_BOSS));/*�{�Xoff*/
		}
		draw_side_panel = 1/*pd.state_flag |= ST ATE_FLAG_09_IS_PANEL_WINDOW*/;/* �p�l���\��on */
	}
//
	{
		const u32 bg_color_list[8] =
		{	/*AABBGGRR*/
			0xff000000,/*(��)*/
			0xff106010,/*1��(�΁A���@�̐X)*/
			0xff401010,/*3��(�A�����̒|��)*/		//	0xff104010,/*3��(��)*/
			0xff402020,/*4��(�A�i����)*/
			0xff804010,/*2��(���A�d���̎R�A��)*/	//	0xff802010,/*2��(��)*/		0xff102080,/*2��*/
			0xff601030,/*5��(�A�}����)*/
			0xff301060,/*6��(�ԁA�g����)*/			//	0xff601030,/*6��(��)*/
			0xff000000,/*ending(��)*/
		//	0xff601010,/**/
		};
		gu_set_bg_u32_clear_color((bg_color_list[pd.player_now_stage&0x07]));
	}
	stage_bg_load_texture();
//
//
//	char my_fopen_file_name[128];
//	sp rintf(my_fopen_file_name, DIRECTRY_NAME_DATA_STR "/dat/stage%02d.dat", player_now_stage);
//	sp rintf(my_fopen_file_name, DIRECTRY_NAME_DATA_STR "/dat/stage%01d.txt", player_now_stage);
//	sp rintf(my_fopen_file_name, DIRECTRY_NAME_DATA_STR "/dat/stage%c.txt", ('0'+ player_now_stage) );
//	sp rintf(my_fopen_file_name, DIRECTRY_NAME_DATA_STR "/dat/stage%c.txt", ('0'+ player_now_stage) );
	strcpy(my_fopen_file_name, DIRECTRY_NAME_DATA_STR "/dat/stageZ.txt");
	my_fopen_file_name[10+DIRECTRY_NAME_DATA_LENGTH] = get_stage_chr(pd.player_now_stage); /*load_stage_number*/

//	/*FILE*/char *fp;
	if (NULL==(/*fp =*/my_fopen(/*my_fopen_file_name*/ /*,"r"*/)))
	{
	//	error(ERR_FATAL, (char*)"can't read stage data %s\nerrno: %d (%s)",my_fopen_file_name,errno,strerror(errno));
		error(ERR_FATAL, (char*)"can't read stage %d data %s\n", /*load_stage_number*/pd.player_now_stage, my_fopen_file_name );
	}
//
	load_stage_free_entry();	/* �O��܂Ŋm�ۂ���������������΁A��ɊJ������B */
	int entrys		= 0;		/* �L���s���̒��� */
	int line_num	= 0;		/* �t�@�C���̎��s�� */
	{loop:;
		if (/*NULL*/0 != my_file_fgets(/*buffer_text_1_line,128,fp*/))
		{
			int end_arg/*=0*/;			/* [���݁[]�s�� == �����̎擾�̒��~ */
			int time60; 				/* �o������(1/60�b�P��)  */
			char user_string[128];		/* ������(���b�Z�[�W��t�@�C����) */
			int int_user_command;		/* �P�����R�}���h(�G�⃁�b�Z�[�W���̎��) */
			int select_gazou;			/* �A�j��/�摜��� */
			int user_x; 				/* �����p�����[�^�[�P(�o���w���W�Ȃ�) */
			int user_y; 				/* �����p�����[�^�[�Q(�o���x���W�A�G��x�Ȃ�) */
			int user_hp;				/* �����p�����[�^�[�Q(�o���x���W�A�G��x�Ȃ�) */
			int user_item8; 			/* �����p�����[�^�[�Q(�o���x���W�A�G��x�Ȃ�) */
			int user_score; 			/* �����p�����[�^�[�Q(�o���x���W�A�G��x�Ȃ�) */
			line_num++; 				/* �t�@�C���̎��s�� */
//
			char *ch;					/* �����ʒu */
			ch = buffer_text_1_line;
			/* skiped lines. */
			#if 0
			/* '\n'�������̂��I�������Ȃ�(???) */
			if ('\n'==(*ch))		{	goto loop;/*continue;*/ }	/* ���s�݂̂̍s�͋�s�Ȃ̂ł��Ȃ��łƂ΂� */
			while (isspace(*ch))	{	ch++;					}	/* �󔒂�TAB������ */
			#else
			{my_isspace:;
				if (' '>=(*ch))
				{
					ch++;
					if (0x0a==(*ch))
					{	goto loop;/*continue;*/ }	/* skiped null line. */
					else
					{	goto my_isspace;	}
				}
			}
			#endif
			/* ';'�Ŏn�܂�s�̓R�����g�s�Ȃ̂ŁA���̍s�܂Ŕ�΂��B */
//			if (';'==(*ch)) 	{	goto loop;/*continue;*/ }	/* ';'�Ŏn�܂�s�̓R�����g�Ȃ̂ł��Ȃ��łƂ΂� */
			//
		#if (1==USE_PARTH_DEBUG)
			/*continue;*/;
			#define GOTO_ERR_WARN goto err_warn
		#else
			#define GOTO_ERR_WARN goto loop
		#endif
			/* �f�[�^�[�̒l�������ꍇ�̓G���[ */
			#define CHECK_ERROR_VALUE			if (NULL == ch) 	{	GOTO_ERR_WARN;	}
			/* �f�[�^�[�̋�؂�� ',' �������ꍇ�̓G���[ */
			#define CHECK_ERROR_VALUE_KUGIRI	CHECK_ERROR_VALUE	if (',' != *ch++)	{	GOTO_ERR_WARN;	}
			/* parth start. �\����͊J�n */
			ch = load_stage_get_int(ch, &time60);					CHECK_ERROR_VALUE_KUGIRI	/* load int time60. */	/* �o������[�t���[��(1/60�b)�P��]�̎擾 */
			ch = load_my_file_get_str(ch, user_string, &end_arg);	CHECK_ERROR_VALUE_KUGIRI	/* load str user_string. */ 	/*, '|'*/
			ch = load_stage_get_int(ch, &int_user_command); 		CHECK_ERROR_VALUE_KUGIRI	/* load 1 char commnd. �P�����R�}���h */
			ch = load_stage_get_int(ch, &select_gazou); 			CHECK_ERROR_VALUE_KUGIRI	/* load graphics select. */
			ch = load_stage_get_int(ch, &user_x);					CHECK_ERROR_VALUE_KUGIRI	/* load int user_x. �o��x���W */
			ch = load_stage_get_int(ch, &user_y);					CHECK_ERROR_VALUE_KUGIRI	/* load int user_y. �o��y���W */
			ch = load_stage_get_int(ch, &user_hp);					CHECK_ERROR_VALUE_KUGIRI	/* load int user_hp. �̗� */
			ch = load_stage_get_int(ch, &user_item8);				CHECK_ERROR_VALUE_KUGIRI	/* load int user_. �o���A�C�e�� */
			ch = load_stage_get_int(ch, &user_score);				CHECK_ERROR_VALUE			/* load int user_. ���j���̊l���X�R�A[(1/10�P��)] */
			/* do set register entry. �ǂݍ��ݐ��������f�[�^�[���A�ǉ��o�^����B */
			#define MUSIC_CONVERT_TIME (10)
			/* �ǉ��o�^���� */
			load_stage_add_entry(
				MUSIC_CONVERT_TIME+time60,
				user_string,
				int_user_command,
				select_gazou,
				user_x,
				user_y,
				user_hp,
				user_item8,
				user_score
			);
			entrys++;		/* �L���s�� */
			goto loop;
		#if (1==USE_PARTH_DEBUG)
		err_warn:
			/* Load parth error. */
		//	static void load_stage_set_error(char *load_filename, int error_line_number)
		//	{
		//		error(ERR_WARN, (char*)"syntax error in stage data '%s', line no: %d", load_filename, error_line_number);
		//	}
		//	load_stage_set_error(filename, line_num);
			error(/*ERR_WARN*/ERR_FATAL, (char*)"syntax error in stage data '%s', line no: %d", filename, line_num);
			goto loop;
		#endif
		}
	}
	my_fclose(/*fp*/);
	//return (entrys);
	if (0==entrys)		/* �L���s�����Ȃ���΃G���[ */
	{
		error(/*ERR_WARN*/ERR_FATAL, (char*)"no entrys for STAGE%d.TXT", pd.player_now_stage); /*level*/ /*load_stage_number*/
	}
	//fps_init();/* ??? auto fps������ */
}
