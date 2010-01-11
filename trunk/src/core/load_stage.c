
/*---------------------------------------------------------
	�X�e�[�W�G�f�[�^�[(dat)�̓ǂݍ���
---------------------------------------------------------*/

#include "game_main.h"
#include "load_stage.h"

/*---------------------------------------------------------

---------------------------------------------------------*/

//extern int errno;

STAGE_DATA *stage_data_table = NULL;

/*---------------------------------------------------------
	�q�֐�
	�O��܂Ŋm�ۂ���������������ΊJ������B
	Initial task, "free"ed the altenative entris.
---------------------------------------------------------*/

static void load_stage_free_entry(void)
{
	STAGE_DATA *l = stage_data_table;
	STAGE_DATA *n;
	while (NULL != l)
	{
		n = l->next;
		free (l);
		l = n;
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
static void load_stage_add_entry(Uint32 time60, char user_command, char *user_string, int user_x, int user_y)		// �I�u�W�F�N�g�̐���
{
	STAGE_DATA *new_entry;
	new_entry					= mmalloc(sizeof(STAGE_DATA));
//	new_entry->v_time			= (time60*6/*100*1000*/);/* �ǂݍ��݌�APSP�ɓs���̗ǂ��l�ɕϊ�(1/10[sec]-> 1/1000000[sec] == 1[nsec] == nano seconds ) */
//	new_entry->v_time			= (time60*6/*100*1000*/);/* �ǂݍ��݌�APSP�ɓs���̗ǂ��l�ɕϊ�(1/10[sec]-> 1/60[sec] ) */
	new_entry->v_time			= (time60  /*100*1000*/);/* �ǂݍ��݌�APSP�ɓs���̗ǂ��l�ɕϊ�(1/10[sec]-> 1/60[sec] ) */
	new_entry->user_x			= user_x;
	new_entry->user_y			= user_y;
//	new_entry->done 			= 0;
//
	new_entry->user_1_moji		= user_command;
	new_entry->user_i_code		= 0;

	strncpy(new_entry->user_string, user_string, (MAX_PARA1_44-1)/*63*/);

/* �Q�[�����́A���ԃR�[�h�`���̃R�}���h�������\����͂͂��Ȃ��B(�\����͂�load_stage.c�ōs��) */

//
	const char *ctype_name[CTYPE_99_MAX] =	/* ���� 32 ��� */
	{
		NULL,/* [�ԕ���؂�] */
	//	���̑�
		"QUIT", 	/* �Q�[�� �S�X�e�[�W �N���A�[ */
		"BG",		/*	"BG_CONTROL"*/		/* ���V�X�e���R�}���h�Ȃ̂ŉp��ɂ��� */
	//	�{�X
		"�d��", 	/*	extra 2 	"BOSS05",*/
		"�`���m",	/*	extra 1 	"BOSS05",*/
		"���", 	/*	stage 6 	"BOSS04",*/ 	// [***090207		�ǉ�
		"�p�`�F",	/*	stage 5 	"BOSS01",*/
		"�P��", 	/*	stage 4 	"BOSS03",*/
		"����", 	/*	stage 3 	"BOSS03",*/ 	/* 3�ʂ͂ǂ����邩����(�i��? �d��?) */
		"��",		/*	stage 2 	"BOSS02",*/
		"�A���X",	/*	stage 1 	"BOSS01",*/
	/* ����G[���^�G] */
		"���{�X6",
		"������",	/*	"GFAIRY",*/ 	//		�ǉ�
		"���",
		"�Ă�",
		"���{�X2",
		"���[�~�A", /*	"GFAIRY",*/ 	//		�ǉ�
//		�p�~?
		"�d��2",	/*	"GFAIRY",*/ 	//		�ǉ�
		"�d��1",	/*	"GFAIRY",*/ 	// [***090207		�ǉ�
//
	/* �����w */
		"�����w1",	/*	"GROUNDER",*/
		"�����w2",	/*	"MAGICF",*/
	/* ���� �A�z�� */
		"�A�z��1",	/*	"PLASMABALL",*/
		"����1",	/*	"PROBALL",*/
	/* �d�� */
		"�͗d��1",	/*	"CUBE",*/
	/* ���̑��U�R */
		"���΂�1",	/*	"DRAGER",*/
		"���΂�2",	/*	"EYEFO",*/
		"���ґ�1",	/*	"MING",*/
		"���ґ�2",	/*	"GREETER",*/
	/* �ы� */
		"�Ԗы�1",	/*	"MINE",*/
		"�Ζы�1",	/*	"XEV",*/
		"�Ζы�2",	/*	"CRUSHER",*/
		"�ы�1",	/*	"BADGUY",*/
		"�ы�2",	/*	"BADGUY",*/
	/* ���d�� */
		"���C�h1",	/*	"RWINGX",*/
		"���C�h2",	/*	"ZATAK",*/
		"���C�h3",	/*	"CURVER",*/
		"���C�h4",	/*	"CIR",*/
	/* ���d�� */
		"�d��1",	/*	"FAIRY",*/		// [***090207		�ǉ�
		"�d��2",	/*	"SPLASH",*/ 	// [***090124		�ǉ�
		"�d��3",	/*	"SPLASH",*/ 	//		�ǉ�
		"�d��4",	/*	"SPLASH",*/ 	//		�ǉ�
	};
//


	/* �ǂݍ��񂾃R�}���h�𒆊ԃR�[�h�`���ɕϊ����� */
	switch (new_entry->user_1_moji)
	{
	case 'T':	new_entry->user_i_code=ETYPE_01_SJIS_TEXT;		break;	/* english Text */
	case 'B':	new_entry->user_i_code=ETYPE_02_LOAD_BG;
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
		break;	/* Background */
	default:	// add background tiles....
		{
			for (new_entry->user_i_code = /*CT YPE_00_unknown+*/(CTYPE_99_MAX-1); /*0*/CTYPE_00_NONE/*CT YPE_00_unknown*/ < new_entry->user_i_code; new_entry->user_i_code--)
			{
				if (0==tiny_strcmp( new_entry->user_string, /*(char *)*/&/*btype_name*/ctype_name[(unsigned int)new_entry->user_i_code][0] ) )
				{
					break;
				}
			} // 0/*CT YPE_00_unknown*/ == aaa );
		}
		break;
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
#include "my_file.h"

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
		//
//		#if (1==US E_ENDING_DEBUG)
//		if (MA X_STAGE6_FOR_CHECK == player_now_stage/*continue_stage*/)
//		{
//		//	if (B07_AFTER_LOAD==pd->bo ssmode)
//			if ((STATE_FLAG_10_IS_LOAD_SCRIPT|ST ATE_FLAG_11_IS_BOSS_DESTROY)==(pd->state_flag&(STATE_FLAG_10_IS_LOAD_SCRIPT|ST ATE_FLAG_11_IS_BOSS_DESTROY)))
//			{
//				load_stage_number=9;/*�G���f�B���O�f�o�b�O�p*/
//			}
//		}
//		if (9!=load_stage_number)
//		#endif //(1==US E_ENDING_DEBUG)

extern int select_player;
extern void bg2_start_stage(void); // [***090209		�ǉ�
extern void stage_bg_load_texture(void);
void load_stage(void)	/*int level*/	/* ���Xint */
{
	bg2_start_stage();	// [***090209		�ǉ�
	enemy_set_random_seed(/*set_seed*/);	/* ���v���C�Ή��o����悤�ɁA�����n��̏������B */
//	int level = player_now_stage;
	player_now_stage++; /*(*level)++*/;
	// change music soundtrack
	play_music_num( /*1+*/	/*level*/player_now_stage/*(*level)*/ );/* n�ʓ��� */
//
//	int load_stage_number = player_now_stage;
	{	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
		{
			pd->state_flag &= (~(STATE_FLAG_05_IS_BOSS));/*�{�Xoff*/
		}
		draw_side_panel=1/*pd->state_flag |= ST ATE_FLAG_09_IS_PANEL_WINDOW*/;/* �p�l���\��on */
	}
//
	stage_bg_load_texture();
//
//
//	char my_fopen_file_name[128];
//	sp rintf(my_fopen_file_name,"%s/dat/level%02d.dat", data_dir, /*level*/player_now_stage);
//	sp rintf(my_fopen_file_name,"%s/dat/stage%01d.txt", data_dir, /*level*/player_now_stage);
//	sp rintf(my_fopen_file_name, "%s/dat/stage%c.txt", data_dir, ('0'+/*level*/load_stage_number/*player_now_stage*/) );
//	sp rintf(my_fopen_file_name, DIRECTRY_NAME_DATA "/dat/stage%c.txt", ('0'+/*level*/load_stage_number/*player_now_stage*/) );
	strcpy(my_fopen_file_name, DIRECTRY_NAME_DATA "/dat/stageZ.txt");
	my_fopen_file_name[10+DIRECTRY_NAME_LENGTH] = ('0'+player_now_stage);/*level*/ /*load_stage_number*/ /*player_now_stage*/

//	/*FILE*/char *fp;
	if (NULL==(/*fp=*/my_fopen(/*my_fopen_file_name*/ /*,"r"*/)))
	{
	//	error(ERR_FATAL, "can't read stage data %s\nerrno: %d (%s)",my_fopen_file_name,errno,strerror(errno));
		error(ERR_FATAL, "can't read stage %d data %s\n", /*load_stage_number*/player_now_stage, my_fopen_file_name );
	}
//
	load_stage_free_entry();	/* �O��܂Ŋm�ۂ���������������΁A��ɊJ������B */
	int entrys		= 0;		/* �L���s���̒��� */
	int line_num	= 0;		/* �t�@�C���̎��s�� */
	{loop:;
		if (/*NULL*/0 != my_fgets(/*buffer_text_1_line,128,fp*/))
		{
			int end_arg/*=0*/;			/* [���݁[]�s�� == �����̎擾�̒��~ */
			int time60; 				/* �o������(1/60�b�P��)  */
			char char_user_command; 	/* �P�����R�}���h(�G�⃁�b�Z�[�W���̎��) */
			char user_string[128];		/* ������(���b�Z�[�W��t�@�C����) */
			int user_x; 				/* �����p�����[�^�[�P(�o���w���W�Ȃ�) */
			int user_y; 				/* �����p�����[�^�[�Q(�o���x���W�A�G��x�Ȃ�) */
			char *c;					/* �����ʒu */
			line_num++; 				/* �t�@�C���̎��s�� */
			c = buffer_text_1_line;
//
			/* skiped lines. */
			#if 0
			/* '\n'�������̂��I�������Ȃ�(???) */
			if (*c=='\n')		{	goto loop;/*continue;*/ }	/* skiped null line. */ 	/* Leerzeilen ueberspringen */
			while (isspace(*c)) {	c++;					}	/* dust left space.  */ 	/* fuehrende leerzeichen uebergehen */
			#else
			{my_isspace:;
				if (*c<=' ')
				{
					c++;
					if (*c==0x0a)
					{	goto loop;/*continue;*/ }	/* skiped null line. */
					else
					{	goto my_isspace;	}
				}
			}
			#endif
			if (*c=='#')		{	goto loop;/*continue;*/ }	/* skiped comment line. */	/* Kommentarzeile ? */
//
		#if (1==USE_PARTH_DEBUG)
			#define GOTO_ERR_WARN goto err_warn
		#else
			#define GOTO_ERR_WARN goto loop
		#endif
			/* parth start */	/* Startzeitpunkt holen */
			c = load_stage_get_int(c, &time60); 	if (NULL == c)			{	GOTO_ERR_WARN;/*continue;*/;	}	/* load int time60 */		/* ���Ԃ̎擾 */
													if (','/*'|'*/ != *c++) {	GOTO_ERR_WARN;/*continue;*/;	}	/* load '|' */
			char_user_command = *c++;																				/* load 1 char commnd */	/* �P�����R�}���h */	/* Befehl */
													if (','/*'|'*/ != *c++) {	GOTO_ERR_WARN;/*continue;*/;	}	/* load '|' */
			c = load_my_file_get_str(c, user_string, &end_arg/*, '|'*/);
													if (NULL == c)			{	GOTO_ERR_WARN;/*continue;*/;	}	/* load str user_string */
													if (','/*'|'*/ != *c++) {	GOTO_ERR_WARN;/*continue;*/;	}	/* load '|' */
			c = load_stage_get_int(c, &user_x); 	if (NULL == c)			{	GOTO_ERR_WARN;/*continue;*/;	}	/* load int user_x */
													if (','/*'|'*/ != *c++) {	GOTO_ERR_WARN;/*continue;*/;	}	/* load '|' */
			c = load_stage_get_int(c, &user_y); 	if (NULL == c)			{	GOTO_ERR_WARN;/*continue;*/;	}	/* load int user_y */
			/* do set register entry. */
			#define MUSIC_CONVERT_TIME (10)
			/* �ǉ��o�^���� */
			load_stage_add_entry(MUSIC_CONVERT_TIME+time60, char_user_command, user_string, user_x, user_y);
			entrys++;		/* �L���s�� */
			goto loop;
		#if (1==USE_PARTH_DEBUG)
		err_warn:
			/* Load parth error. */
		//	static void load_stage_set_error(char *load_filename, int error_line_number)
		//	{
		//		error(ERR_WARN, "syntax error in stage data '%s', line no: %d", load_filename, error_line_number);
		//	}
		//	load_stage_set_error(filename, line_num);
			error(/*ERR_WARN*/ERR_FATAL, "syntax error in stage data '%s', line no: %d", filename, line_num);
			goto loop;
		#endif
		}
	}
	my_fclose (/*fp*/);
	//return (entrys);
	if (0==entrys)		/* �L���s�����Ȃ���΃G���[ */
	{
		error(/*ERR_WARN*/ERR_FATAL, "no entrys for STAGE%d.TXT",player_now_stage);	/*level*/ /*load_stage_number*/
	}
	//fps_init();/* ??? auto fps������ */
}
