
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�V�i���I��b ����
	-------------------------------------------------------
	���݂̖��_:
	-------------------------------------------------------
	�u�Q�[�����̃V�i���I�����v��
	�u�G���f�B���O���̃V�i���I�����v��
	�F�X�l�������� psp �ł͑��x�I�ɑ����������B
	�����͕�������\��ł��B
	-------------------------------------------------------
	���s���낵�����ǁu�Q�[�����̃V�i���I�����v�� SDL �Ƃ�����
	�����������������Ēx���Ęb�ɂȂ�Ȃ����B
	�u�G���f�B���O���̃V�i���I�����v�́A�ʂɑ��x�͗v��Ȃ��̂ŁA
	(�ꉞGu������) SDL �̑O�̕������ۂ��߂��������ǂ������m��Ȃ��B
	-------------------------------------------------------
	�{�Ƃ����̂Q�͑S�R�Ⴄ�����ɂȂ��Ă���ۂ��ł��B
---------------------------------------------------------*/

#include "kaiwa_system.h"
#include "kanji_system.h"
#include "111_my_file.h"
#include "kaiwa_sprite.h"
#include "../draw/gu_draw_screen.h"


/*static*/global KAIWA_OBJ kaiwa_sprite[KAIWA_OBJ_99_MAX];	/* �V�i���I��b �ėp�I�u�W�F */ /*32*/ /*20*/

/*---------------------------------------------------------
	�C���^�v���^����(�I�y�R�[�h)�������������ǂށB
	10�����ȏ�̓G���[�B
---------------------------------------------------------*/

static char *read_opecode(char *input_string, char *output_string, int *end_arg)
{
	int i = 0;
	while ((*input_string != ' ') && (*input_string != 13))
	{
		i++;
		if ((16-1) <= i)/*(15�����ȏ�̓G���[�B)*/
		{	goto ne111;}
		*output_string++ = *input_string++;/*(ascii, 1�����]��)*/
	}
	if (13==(*input_string))	/* ���s�R�[�h�� OD OA */
	{
		*end_arg = (1);
	}
	/*(������I�[������ɏ���������B)*/
	*output_string = (0);		/* [\0==EOT] */
	return (input_string);/*(???)*/
/*error*/
ne111:
	return ((char *)NULL);/*(�G���[�B)*/
}


/*[is_digit_or_hifun();'����'��������'-'�L���̏ꍇ�B�ł��邩�𒲂ׂ�B]*/

/* ���w��̏ꍇ�̈����ȗ��l�Ƒ��̒l������ʂ���ׂɎg�p���Ȃ��l�������ȗ��l�ɐݒ肷��B*/
/*(�����ȗ��l�̏ꍇ -777 ���Z�b�g�����d�l)*/
#define m1_SYOU_RYAKU_TI (-777)
/*---------------------------------------------------------
	�����ɓn�閽�߃p�����[�^�[(�I�y�����h)����������ǂށB
---------------------------------------------------------*/
extern int is_digit_or_hifun(char *read_str_position);/*(load_stage.c)*/
static char *read_operand_int_only(char *read_str_position, int *number, int *line_terminate_end_flag)
{
	char tmp_str32bytes[32];/*(atoi�̏ꍇ�A�o�b�t�@��32[bytes]==�ő�31�����ɓ���)*/
	char *tmp_str_position = tmp_str32bytes;
	int i = 0;
	while ( is_digit_or_hifun(read_str_position) )/*(�����ɑΉ�����)*/
	{
		i++;
		if ((32-1) <= i)/*(�o�b�t�@��31�����ȏ�̓G���[�B)*/
		{	goto ne222;}
		*tmp_str_position++ = *read_str_position++;
	}
	/* ',' �܂��� '\n' ��������I�� */
	if (13==(*read_str_position)) 	/* ���s�R�[�h�� OD OA */
	{
		*line_terminate_end_flag = 1;
	}
	*tmp_str_position = 0;
	if (((','==(*read_str_position)) || (13==(*read_str_position))) && (','==(*(read_str_position-1))))	{*number = m1_SYOU_RYAKU_TI; /*(�l�̓ǂ݂��݂Ɏ��s�����ꍇ�ȗ��l�Ƃ���B)*/ }
	else														{*number = atoi(tmp_str32bytes); }
	return (read_str_position);
/*error*/
ne222:
	return ((char *)NULL);/*(�G���[�B)*/
}

/*---------------------------------------------------------
	�C���^�v���^����(�I�y�R�[�h)�����R�[�h�B
---------------------------------------------------------*/

enum
{
// OBJ�֘A
	I_CODE_00_OBJ_LOOK = 0,
	I_CODE_01_OBJ_XY,
	I_CODE_02_OBJ_LOAD,
	I_CODE_03_OBJ_WAIT,
	//
	I_CODE_04_OBJ_DRAW_ON_OFF,/*(r36�V��)*/
	I_CODE_05_RESURVED, 	// [���ݖ��g�p]
	I_CODE_06_RESURVED, 	// [���ݖ��g�p]
	I_CODE_07_RESURVED, 	// [���ݖ��g�p]
// ��ʐ���
	I_CODE_08_DRAW_TEXT,
	I_CODE_09_DRAW_BG,
	I_CODE_0a_DRAW_PANEL,
	I_CODE_0b_DRAW_SKIP,
	//
	I_CODE_0c_DRAW_WAIT,	// (vsync) wait�҂��B
	I_CODE_0d_RESURVED, 	// [���ݖ��g�p]
	I_CODE_0e_RESURVED, 	// [���ݖ��g�p]
	I_CODE_0f_RESURVED, 	// [���ݖ��g�p]
// �e�L�X�g�֘A
	I_CODE_10_TEXT,
	I_CODE_11_HOLD,
	I_CODE_12_FONT_COLOR,
	I_CODE_13_FONT_SPEED,
// �w�i/BGM
	I_CODE_14_BG_LOAD,
	I_CODE_15_BG_FADE,
	I_CODE_16_BGM,
	I_CODE_17_BGM_VOLUME,
// �{�X����
	I_CODE_18_BOSS_LOAD,
	I_CODE_19_BOSS_TERMINATE,/*(r34�V��)*/
	I_CODE_1a_BOSS_START,
	I_CODE_1b_BOSS_RESULT,/*(r35�V��)*/
// �V�X�e��
	I_CODE_1c_STAGE_LOAD,/*(r36�V��)*/
	I_CODE_1d_DUMMY,	// [���ݖ��g�p]
	I_CODE_1e_DUMMY,	// [���ݖ��g�p]
	I_CODE_1f_DUMMY,	// [���ݖ��g�p]
//
	I_CODE_99_MAX/*(2^n �̕K�v������B)*/
};


typedef struct _kaiwa_youso_tag_
{
	s16 i_code; 	/* Interprited Code. / Intermediate Language, Command ���Ԍ��� / �I��������ǂ��� */
	s16 para1;
	s16 para2;
	s16 para3;
	struct _kaiwa_youso_tag_ *next;
	s32 str_num0;// char para0[(7*32)/*200*/];/* 32�̔{�� */
} KAIWA_YOUSO;/* == 16bytes */ /* == 256bytes */
//[16==4*4]
//[32==8*4]

/*---------------------------------------------------------
	�V�i���I��b�A���X�g�����B
---------------------------------------------------------*/
/*
	������ malloc ����ƒx������A�Œ� 256 ���炢�ɂ���ׂ��B
	�����A�͕핗�̕W���V�i���I�Ȃ�΁A 100 ������Ȃ��Ǝv����B
*/
static KAIWA_YOUSO *kaiwa_youso_list_scan;			/* ���ߗ�̎��s�ʒu��ێ�����B */	/*=NULL*/	/* ���ߗ�̓ǂݍ��݈ʒu��ێ�����B */
static KAIWA_YOUSO *kaiwa_youso_list_bigin; 		/* ���ߗ�̊J�n�ʒu��ێ�����B�J�����ɂ��K�v�B */	/*=NULL*/


/*---------------------------------------------------------
	�O��̃V�i���I���������ɂ���΂��ׂĊJ���B
---------------------------------------------------------*/
#include <malloc.h>/* free(); */
static void load_kaiwa_youso_free(void)
{
	KAIWA_YOUSO *l = kaiwa_youso_list_bigin;
	KAIWA_YOUSO *n;
	while (NULL != l)
	{
		n = l->next;
		free(l);	/* 1�R�}���h���J�� */
		l = n;
	}
	kaiwa_youso_list_bigin	= (KAIWA_YOUSO *)NULL;
	kaiwa_youso_list_scan	= (KAIWA_YOUSO *)NULL;
}

/*---------------------------------------------------------
	������X�^�b�N
	-------------------------------------------------------
	64�����Ȃ����ǁA�I�������Ă�B
	(���V�i���I)�얲A end(s61.txt) �� 51 ��������Ȃ��B(r34)���݂�64�ȏ����B
	�����A�͕핗�̕W���V�i���I�Ȃ�΁A 64 �ŏ\�����Ǝv����B
	���A���S�ׁ̈A�{�� 128 �ɂ��Ƃ��B
	����I�[�o�[���Ă��A�G���["string stack over flow.(%d)\n"��
	�I�����邩���薳���B(���b�Z�[�W���猴�������邵)
---------------------------------------------------------*/

//#define MAX_STR_STACK 	(64)
#define MAX_STR_STACK		(128)
static int str_stack_position;
static char str_stack[MAX_STR_STACK][(7*32)/*200*/];/* 32�̔{�� */

/*---------------------------------------------------------
	�o�^����
---------------------------------------------------------*/
enum
{
	PARAM_01 = 0,
	PARAM_02,
	PARAM_03,
//	PARAM_04,/*���g�p*/
	PARAM_99_MAX/*�ő吔*/
};
/*
	��{�`
	command para0,para1,para2,para3
*/
static void regist_kaiwa_youso(
	char *command,
	char *c_p0,
	const int *c_pn)
{
	KAIWA_YOUSO *new_kaiwa_youso;
	new_kaiwa_youso 			= my_calloc(sizeof(KAIWA_YOUSO));/* 1�R�}���h�� */

//�Ă���	strcpy(new_kaiwa_youso->para0, c_p0);
	new_kaiwa_youso->para1		= /*c_p1*/c_pn[PARAM_01];	/* �f�t�H���g */
	new_kaiwa_youso->para2		= /*c_p2*/c_pn[PARAM_02];	/* �f�t�H���g */
	new_kaiwa_youso->para3		= /*c_p3*/c_pn[PARAM_03];	/* �f�t�H���g */
//	new_kaiwa_youso->para4		= /*c_p3*/c_pn[PARAM_04];	/* �f�t�H���g */
//
//	new_kaiwa_youso->i_code 	= I_CODE_DONE;
	new_kaiwa_youso->next		= NULL;

	if ('B'==(*command))
	{
					if (0==tiny_strcmp(command, "BG_LOAD")) 	{	new_kaiwa_youso->i_code = I_CODE_14_BG_LOAD; strcpy( (&str_stack[str_stack_position][0]), c_p0);str_stack_position++;	/*push_str(c_p0);*/ 	}	/* �w�i�t�@�C���̕ύX */
			else	if (0==tiny_strcmp(command, "BG_FADE")) 	{	new_kaiwa_youso->i_code = I_CODE_15_BG_FADE;		}	/* �w�i�̃t�F�[�h�A�E�g */
			else	if (0==tiny_strcmp(command, "BGM")) 		{	new_kaiwa_youso->i_code = I_CODE_16_BGM;			}	/* BGM�ύX */
			else	if (0==tiny_strcmp(command, "BGM_VOLUME"))	{	new_kaiwa_youso->i_code = I_CODE_17_BGM_VOLUME; 	}	/* BGM ���� �ύX */
		if (0==tiny_strcmp(command, 			"BOSS"))
		{
					if (0==tiny_strcmp(c_p0,	"load"))		{	new_kaiwa_youso->i_code = I_CODE_18_BOSS_LOAD;		}	/* �{�X�ǂݍ��� */
			else	if (0==tiny_strcmp(c_p0,	"term"))		{	new_kaiwa_youso->i_code = I_CODE_19_BOSS_TERMINATE; }	/* result��ɃQ�[�������I���B */
			#if (1)
			else	if (0==tiny_strcmp(c_p0,	"start"))		{	new_kaiwa_youso->i_code = I_CODE_1a_BOSS_START; 	}	/* �{�X�U���J�n�B */
			else	if (0==tiny_strcmp(c_p0,	"result"))		{	new_kaiwa_youso->i_code = I_CODE_1b_BOSS_RESULT;	}	/* (r35)���ݎd�l�̓s����v��B���ꂪ�����ƃ��U���g��ʂ��I���o�Ȃ��B */
			#endif
		}
	}
	else
	if ('D'==(*command))
	{
		if (0==tiny_strcmp(command, 			"DRAW"))
		{
					if (0==tiny_strcmp(c_p0,	"panel"))		{	new_kaiwa_youso->i_code = I_CODE_0a_DRAW_PANEL; 	}	/* �X�R�A�p�l���̕\��/��\�� */
			else	if (0==tiny_strcmp(c_p0,	"text"))		{	new_kaiwa_youso->i_code = I_CODE_08_DRAW_TEXT;		}	/* �e�L�X�g��ʂ̕\��/��\�� */
			else	if (0==tiny_strcmp(c_p0,	"bg"))			{	new_kaiwa_youso->i_code = I_CODE_09_DRAW_BG;		}	/* �w�i�̕\��/��\�� */
			else	if (0==tiny_strcmp(c_p0,	"skip"))		{	new_kaiwa_youso->i_code = I_CODE_0b_DRAW_SKIP;		}	/* */
			else	if (0==tiny_strcmp(c_p0,	"color"))		{	new_kaiwa_youso->i_code = I_CODE_12_FONT_COLOR; 	}	/* �����̐F */
			else	if (0==tiny_strcmp(c_p0,	"speed"))		{	new_kaiwa_youso->i_code = I_CODE_13_FONT_SPEED; 	}	/* �����̕\�����x */
			else	if (0==tiny_strcmp(c_p0,	"wait"))		{	new_kaiwa_youso->i_code = I_CODE_0c_DRAW_WAIT;		}	/* vsync �E�F�C�g�҂� */
		}
	}
	else
	if ('O'==(*command))
	{
//		psp_fatal_error( (char*)
//		//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
//			"kaiwa: s_command�B" "\\n"
//			"�l: %d�B test ok�B", str_stack_position);
					if (0==tiny_strcmp(command, "OBJ_LOAD"))	{	new_kaiwa_youso->i_code = I_CODE_02_OBJ_LOAD;		strcpy( (&str_stack[str_stack_position][0]), c_p0);str_stack_position++;			}	/* �ėp�I�u�W�F�̃��[�h / �ēǂݍ��� / �ėp�I�u�W�F�ɉ摜��ǂ݂��ށB */
					if (0==tiny_strcmp(command, "OBJ_TEXT"))	{	new_kaiwa_youso->i_code = I_CODE_1c_STAGE_LOAD; 	strcpy( (&str_stack[str_stack_position][0]), c_p0);str_stack_position++;			}	/* �G���f�[�^�[�ǂ݂��� */
		if (0==tiny_strcmp(command, 			"OBJ"))
		{
					if (0==tiny_strcmp(c_p0,	"look"))		{	new_kaiwa_youso->i_code = I_CODE_00_OBJ_LOOK;			}	/* �I�u�W�F�N�g���ڃR�}���h */
			else	if (0==tiny_strcmp(c_p0,	"xy"))			{	new_kaiwa_youso->i_code = I_CODE_01_OBJ_XY; 			}	/* �I�u�W�F�N�g�̌��_�ݒ� */
			else	if (0==tiny_strcmp(c_p0,	"wait"))		{	new_kaiwa_youso->i_code = I_CODE_03_OBJ_WAIT;			}	/* �I�u�W�F�N�g�ړ������܂ő҂B */
			else	if (0==tiny_strcmp(c_p0,	"draw"))		{	new_kaiwa_youso->i_code = I_CODE_04_OBJ_DRAW_ON_OFF;	}	/* �I�u�W�F�N�g�̕\��/��\�� */
//			else	if (0==tiny_strcmp(c_p0,	"speed"))		{	new_kaiwa_youso->i_code = I_CODE_OBJ_SPEED; 			}	/* �I�u�W�F�N�g�̑��x�ݒ� */
		}
	}
	else
//	if ('T'==(*command))
//	{
					if (0==tiny_strcmp(command, "TEXT"))		{	new_kaiwa_youso->i_code = I_CODE_10_TEXT;			strcpy( (&str_stack[str_stack_position][0]), c_p0);str_stack_position++;			}	/* ���b�Z�[�W�E�B���h�E�ւ̏������� */
//	}
	else
//	if ('H'==(*command))
//	{
					if (0==tiny_strcmp(command, "HOLD"))		{	new_kaiwa_youso->i_code = I_CODE_11_HOLD;				}	/* �N���b�N�҂� */
//	}

//	if ('C'==(*command))
//	{	if (0==tiny_strcmp(command, 			"CURSOR"))
//		{
//					if (0==tiny_strcmp(c_p0,	"home"))		{	new_kaiwa_youso->i_code = I_CODE_CUR_HOME;				}	/* �J�[�\���̏����� */
//			else	if (0==tiny_strcmp(c_p0,	"load"))		{	new_kaiwa_youso->i_code = I_CODE_CUR_POP;				}	/* �L�������J�[�\���ʒu�̌Ăяo�� */
//			else	if (0==tiny_strcmp(c_p0,	"save"))		{	new_kaiwa_youso->i_code = I_CODE_CUR_PUSH;				}	/* �J�[�\���ʒu�̋L�� */
//			else	if (0==tiny_strcmp(c_p0,	"click"))		{	new_kaiwa_youso->i_code = I_CODE_11_HOLD;				}	/* �N���b�N�҂� */
//			else	if (0==tiny_strcmp(c_p0,	"text"))		{	new_kaiwa_youso->i_code = I_CODE_10_TEXT_WRITE_TEXT;	}	/* ���b�Z�[�W�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
//			else	if (0==tiny_strcmp(c_p0,	"bg"))			{	new_kaiwa_youso->i_code = I_CODE_BG_WRITE_TEXT; 		}	/* BG�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
//		}
//	}
	if ((MAX_STR_STACK-1) < str_stack_position)
	{	/* ������X�^�b�N�A�I�[�o�[�t���[�ŋ����I���B */
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
			"kaiwa: ������X�^�b�N�̈悪����܂���B" "\\n"
			"�l: %d�B �ő�ݒ�l�𑝂₵�ĉ������B", str_stack_position);
	}
	if (NULL==kaiwa_youso_list_scan)/* �擪�Ȃ� */
	{
		kaiwa_youso_list_bigin		= new_kaiwa_youso;				/* �J�����ɕK�v�Ȑ擪�ʒu��ێ�����B */
		kaiwa_youso_list_scan		= kaiwa_youso_list_bigin;		/* �n�߂̃X�L�����ʒu�͊J�n�ʒu�Ƃ���B */
	}
	else/* �擪�ȊO�Ȃ�A�������B */
	{
		kaiwa_youso_list_scan->next = new_kaiwa_youso;				/* ���݈ʒu�̎��ɐV�K��A���B */
		kaiwa_youso_list_scan		= kaiwa_youso_list_scan->next;	/* �V�K�����݈ʒu�ɕύX����B */
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/


/*---------------------------------------------------------
	�V�i���I��b �ǂݍ���
	-------------------------------------------------------
	���̊֐��́A�d�l��u�ƂĂ������������܂��v

---------------------------------------------------------*/

static int s_load_scenario(void)
{
//	FILE *fp;
	if (NULL == (/*fp =*/my_file_fopen()))	/* �J���Ȃ������Ƃ� */	/* my_file_common_name, "r" */
	{
		return (0);/* �ǂݍ��ݎ��s */
	}
	str_stack_position = 0;/* ������̈� */
	int opecode_entrys		= 0;	/* ���߂����������܂�Ă��邩�ǂ����B */
	{
		MY_FILE_LOOP_BEGIN;
		#if (1==USE_PARTH_DEBUG)
			#define GOTO_ERR_WARN goto err_warn
		#else
			#define GOTO_ERR_WARN MY_FILE_LOOP_CONTINUE
		#endif
		/* parth start. �\����͊J�n */
		int end_arg;	end_arg = 0;		/* �s�� == �����̎擾�̒��~ */
		char char_command[16];/*15*/		/* ��{�R�}���h */
		char c_p0[256];/*200*/
		ch = read_opecode(ch, char_command, &end_arg);		/* ��{�R�}���h����(�I�y�R�[�h)�ǂݍ��� */
		if (NULL==ch)										{	GOTO_ERR_WARN;	}	/* �ǂ߂Ȃ���΃G���[ */
		if (!end_arg)/* �s�� */
		{
			if (' ' != (*ch))								{	GOTO_ERR_WARN;	}	/* ��؂� */
			ch++;
			ch = load_my_file_get_str(ch, c_p0, &end_arg);/*, ','*/ 				/* ������R�}���h(�I�y�����h)�ǂݍ��� */
			if (NULL==ch)									{	GOTO_ERR_WARN;	}	/* �ǂ߂Ȃ���΃G���[ */
		}
		{
			int c_pn[PARAM_99_MAX];/*6*/
			{	int kk;
				for (kk=0; kk<PARAM_99_MAX; kk++)/*(6)*/
				{
					c_pn[kk] = m1_SYOU_RYAKU_TI;	/* ���w��̏ꍇ, �����ȗ��l�ɐݒ肷��B */
					if (!end_arg)
					{
						if (',' != (*ch))						{	GOTO_ERR_WARN;	}	/* ��؂� */
						ch++;
						ch = read_operand_int_only(ch, &c_pn[kk], &end_arg);			/* ���l�R�}���h(�I�y�����h)�ǂݍ��� */
						if (NULL==ch)							{	GOTO_ERR_WARN;	}	/* �ǂ߂Ȃ���΃G���[ */
					}
				}
			}
			regist_kaiwa_youso(char_command, c_p0, c_pn);
		}
		opecode_entrys++;
		MY_FILE_LOOP_CONTINUE;
	#if (1==USE_PARTH_DEBUG)
	err_warn:
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
			"kaiwa: ��b�ݒ�t�@�C���́A" "\\n"
			"%d�s�ڂ͈Ӗ�������܂���B" "\\n"
			"%s",
			debug_number_of_read_line,
			my_file_common_name );
		MY_FILE_LOOP_CONTINUE;
	#endif
		MY_FILE_LOOP_END;
	}
	//
	my_file_fclose(/*fp*/);
	//return (opecode_entrys);
	if (0 == opecode_entrys)
	{
		psp_fatal_error(/*ERR_WARN,*/ (char*)
		//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
			"kaiwa: ����͉�b�ݒ�t�@�C��" "\\n"
			"�ł͂���܂���B" "\\n",
			"%s", my_file_common_name);
		return (0);/* �ǂݍ��ݎ��s */
	}
	/* �ǂݍ��ݐ��� */
	kaiwa_youso_list_scan		= kaiwa_youso_list_bigin;/* �V�i���I��b ���s�ʒu���A�J�n�ʒu�Ƃ���B */
	return (1);
}

/////////////////////////////

/*---------------------------------------------------------
	�V�i���I��b�V�X�e���p
	�J�[�\���ʒu�L���b�V��
---------------------------------------------------------*/

//static int cursor_x_chached;			/* �J�[�\���ʒu �ۑ��p */
//static int cursor_y_chached;			/* �J�[�\���ʒu �ۑ��p */



/*---------------------------------------------------------
	�V�i���I��b�V�X�e���A��������
---------------------------------------------------------*/
static int re_draw_BG_count_flag;
global void kaiwa_system_set_re_draw(void)
{
	re_draw_BG_count_flag = (1);/* �`�悵�Ă� */
}

/* �����R�}���h�̎󂯓n���`���B */
#define MY_ICODE_FUNC(name) 		static void name(KAIWA_YOUSO *ssc)
#define MY_ICODE_GET_FUNC(name) 	static int name(void)
#define MY_ICODE_CALL(name) 		name(ssc)

/*---------------------------------------------------------
	[ "i_code_" �V�i���I��b�V�X�e�������R�}���h]
---------------------------------------------------------*/

//static int i_code_fade_bg_exec_fade;
//MY_ICODE_GET_FUNC(i_code_get_bg_fade)
//{
//	return (i_code_fade_bg_exec_fade);
//}

MY_ICODE_FUNC(i_code_load_bg)
{
	re_draw_BG_count_flag = (1);/* �`�悵�Ă� */
//	pop_str();
	/* �V�i���I��b�V�X�e�������R�}���h�̃p�����[�^�[���󂯎��B */
//	char *filename; 	//	filename			= ssc->para0;
//	strcpy(filename, (&str_stack[str_stack_position][0]));str_stack_position++;
//	int used_alpha_flag;	used_alpha_flag 	= ssc->para1;//�A���t�@�t���ǂ݂��݁H�H�H�H
//	int fade_command;		fade_command		= ssc->para2;
//	int set_alpha_speed;//	set_alpha_speed 	= ssc->para3;
	/*---------------------------------------------------------
		�w��t�@�C�����̉摜��ǂ݂���SDL�T�[�t�F�C�X�ɐݒ肷��B
	---------------------------------------------------------*/
	char file_name[128];
	strcpy(file_name, DIRECTRY_NAME_DATA_STR "/" );
	strcat(file_name, /*filename*/(&str_stack[str_stack_position][0]));str_stack_position++;/*pop_string();(������X�^�b�N����������)*/
//
	SDL_Surface *s1;/*temporaly*/
	SDL_Surface *s2;/*temporaly*/
//	s1 = NULL;
	s1 = IMG_Load(file_name);/*�����Ń��[�h����Ɗm���ɏ������������*/
	if (NULL == s1)
	{
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
			"kaiwa: �摜�t�@�C��������܂���B" "\\n"
			"%s", file_name );
//		return (NULL);
	}
	//
//	if (NULL != s2)
//	{
//		SDL_FreeSurface(s2);
		s2 = NULL;
//	}
//
	#if 0
	if (	(m1_SYOU_RYAKU_TI == used_alpha_flag) /* �V�i���I��b ���ŏȗ������ꍇ(�f�t�H���g) */
		||
			(0==used_alpha_flag)	/* ���������ꍇ */
	)
	#endif
	{
		/* �A���t�@�g��Ȃ� */
		s2 = SDL_DisplayFormat(s1);/*�T�[�t�F�X��\���t�H�[�}�b�g�ɕϊ�����B*/
	}
	#if (1)/*(��Ɨp�̈�̉��)*/
	SDL_FreeSurface(s1);
	s1 = NULL;
	#endif
	// �w�i�� SDL_01_BACK_SCREEN �ɕ`�悷��B
	PSPL_UpperBlit(s2, NULL, cb.sdl_screen[SDL_01_BACK_SCREEN], NULL);
	SDL_FreeSurface(s2);
	s2	= NULL;
}
		#if /*(���܂������Ȃ�)*/(0)/* ����SDL�w�i������A story �ŗv��B */
		SDL_SetColorKey(bg_story_window_surface, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);	/* �J���[�L�[(�����F�A�����F)�͍� */
		#endif
	#if 0/*(����)*/
	else
	{
		/* �A���t�@�g�� */
		bg_story_window_surface = SDL_DisplayFormatAlpha(s1);/*�T�[�t�F�X��\���t�H�[�}�b�g�ɕϊ�����B*/
	}
	#endif
	#if 0/*(SDL�ϊ��Ɏ��s==psp�̃�����������Ȃ�)*/
	if (NULL == bg_story_window_surface)
	{
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
			"kaiwa: �摜�ϊ����郁������" "\\n"
			"����܂���B" "\\n"
			"%s", file_name );
//		return (NULL);
	}
	#endif
//	bg_story_window_surface =	(bg_st ory_window_surface);
	//{
	//	bg_st ory_window_surface_image	= (u16 *)bg_st ory_window_surface->pixels;
	//	bg_st ory_window_surface_pitch	= bg_st ory_window_surface->pitch;
	//	bg_st ory_window_surface_width	= bg_st ory_window_surface->w;
	//}
	#if 0/* ����A�v��Ȃ��H�B */
	SDL_SetColorKey(bg_story_window_surface, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
	#endif


//	/*fade_in_command_start*/

	static int draw_bg; 							/* �w�i�\��:1 / ��\��:0�t���O */
	static int bg_alpha;							/* �w�i���l�p */
	static int i_code_fade_bg_bg_alpha_speed;		/* �w�i���l�p */
	static int i_code_fade_bg_exec_fade;			/* do fade */

MY_ICODE_FUNC(i_code_fade_bg)
{
	/* �V�i���I��b�V�X�e�������R�}���h�̃p�����[�^�[���󂯎��B */
//	char *filename; 		filename		= ssc->para0;
//	int alpha;				alpha			= ssc->para1;
//	int fade_command;	//	fade_command	= ssc->para2;
//	if ((1)==fade_command)/*1==fade_in_command*/

	re_draw_BG_count_flag = (1);/* �`�悵�Ă� */
//
	//
	bg_alpha = 0;
	int i_code_fade_bg_set_alpha_speed;//	set_alpha_speed = ssc->para3;
//	if (m1_SYOU_RYAKU_TI == set_alpha_speed)/*�p�����[�^�[�ȗ���*/
	{
		i_code_fade_bg_set_alpha_speed = (5);
	}
	i_code_fade_bg_bg_alpha_speed = i_code_fade_bg_set_alpha_speed;
	draw_bg 					= (1);/*on*/	/*check_draw_bg();*/
	i_code_fade_bg_exec_fade	= (1);/*on*/	/* ������ */

//	return /*(0)*/; 	/* ������ */
}
static void kaiwa_system_vbl_do_BG_fade(void)
{
	if (0!=i_code_fade_bg_exec_fade)
	{
		re_draw_BG_count_flag = (1);/* �`�悵�Ă� */
	//	psp_clear_screen();
		bg_alpha += i_code_fade_bg_bg_alpha_speed;/*(5)*/
		if (250 < bg_alpha)
		{
			bg_alpha = 255;
		//	i_code_fade_bg_exec_fade = (0); 	/* �������� */
			i_code_fade_bg_exec_fade = (0); 	/* �������� */
		//	return /*(1)*/; 	/* �������� */
		}
	//	else
	//	{
	//		i_code_fade_bg_exec_fade = (1); 	/* ������ */
	//	}
	}
}



/*---------------------------------------------------------
	move refresh
---------------------------------------------------------*/
static unsigned int obj_look_up;
static void obj_move_refresh(void)
{
	unsigned int nnn;
	for (nnn=0; nnn<KAIWA_OBJ_99_MAX; nnn++)
	{
		int hide_mode;	/*(�B��郂�[�h)*/
		int offset_x;
		int offset_y;
		KAIWA_OBJ *my_std_obj;
		my_std_obj = &kaiwa_sprite[((nnn)&(KAIWA_OBJ_99_MAX-1))]; /* �ėp�I�u�W�F */
		//
		my_std_obj->move_flag = KAIWA_OBJ_MOVE_FLAG_01_MOVE_START;

		if (KAIWA_OBJ_99_MAX>(obj_look_up))/* �ʏ탂�[�h */
		{
			if ((nnn)!=(obj_look_up))/* ���ڃI�u�W�F�ȊO�Ȃ�I�t�Z�b�g���� */
			{			hide_mode = 1;		}	/*(�B��郂�[�h ON)*/
			else/* ���ڃI�u�W�F�Ȃ�I�t�Z�b�g���� */
			{			hide_mode = 0;		}	/*(�B��郂�[�h OFF)*/
		}
		else/*(�������[�h)*/
		{
			hide_mode = 0;	/*(�B��郂�[�h OFF)*/	/*(�S���A���ڃI�u�W�F)*/
		}
		if (0==hide_mode)/* ���ڃI�u�W�F�Ȃ�I�t�Z�b�g���� */
		{	/*(�B��郂�[�h OFF)*/
			offset_x	= (0);
			offset_y	= (0);
		// /* �F */
			my_std_obj->color32 = (0xffffffff);/*(�s���� [255/alpha255])*/
		}
		else/* ���ڃI�u�W�F�ȊO�Ȃ�I�t�Z�b�g���� */
		{	/*(�B��郂�[�h ON)*/
			offset_x	= my_std_obj->offset.x256;
			offset_y	= my_std_obj->offset.y256;
		// /* �F */
			my_std_obj->color32 = (0x60ffffff);/*(������ [180/alpha255])*/
		}
		/* �����ʒu[A]�Ƃ��āA���炩���߈ړ������ʒu���Z�o���Ă��� */
	//	my_std_obj->target.x256 = my_std_obj->origin.x256 + offset_x + ((-((my_std_obj->w)>>1))<<8);	/* �ړ��������W */
	//	my_std_obj->target.y256 = my_std_obj->origin.y256 + offset_y + ((-((my_std_obj->h)>>1))<<8);	/* �ړ��������W */
	//	my_std_obj->target.x256 = my_std_obj->origin.x256 + offset_x + ((-((1<<(my_std_obj->w_bit))>>1))<<8);	/* �ړ��������W */
	//	my_std_obj->target.y256 = my_std_obj->origin.y256 + offset_y + ((-((1<<(my_std_obj->h_bit))>>1))<<8);	/* �ړ��������W */
		my_std_obj->target.x256 = my_std_obj->origin.x256 + offset_x + ((-(( (my_std_obj->width_2n))>>1))<<8);		/* �ړ��������W */
		my_std_obj->target.y256 = my_std_obj->origin.y256 + offset_y + ((-(( (my_std_obj->height_2n))>>1))<<8); 	/* �ړ��������W */
		//	my_std_obj->target.x256 	= (my_std_obj->target.x256);//�ړ������\��ʒu
		//	my_std_obj->target.y256 	= (my_std_obj->target.y256);//�ړ������\��ʒu

		/* ���݈ʒu���A�����ʒu[B]�Ƃ��ĕۑ��B */
		{
			my_std_obj->alt.x256		= (my_std_obj->center.x256);//���݈ʒu
			my_std_obj->alt.y256		= (my_std_obj->center.y256);//���݈ʒu
		}
		/*(�������B�����̐ݒ�)*/
		my_std_obj->toutatu_wariai256 = (0);			/* �����x (x32) */
	}
}

/*---------------------------------------------------------
OBJ xy,0,128,144
OBJ xy,[�I�u�W�F�N�g�w��ԍ�],[���_x���W],[���_y���W]
	[ "i_code_" �V�i���I��b�V�X�e�������R�}���h]
	I_CODE_01_OBJ_XY �I�u�W�F�N�g�̌��_���W�̐ݒ�B
---------------------------------------------------------*/

MY_ICODE_FUNC(i_code_obj_xy)
{
	/* �V�i���I��b�V�X�e�������R�}���h�̃p�����[�^�[���󂯎��B */
//	char *color32_str;		color32_str = ssc->para0;
	int num;				num 		= ssc->para1;/*(�I�u�W�F�N�g�w��ԍ�)*/
	int origin_x;			origin_x	= ssc->para2;/* ���_x���W�̐ݒ� */
	int origin_y;			origin_y	= ssc->para3;/* ���_y���W�̐ݒ� */
//
	KAIWA_OBJ *my_std_obj;
	my_std_obj = &kaiwa_sprite[((num)&(KAIWA_OBJ_99_MAX-1))]; /* �ėp�I�u�W�F */
	//
	my_std_obj->origin.x256 = ((origin_x)<<8);		/* ���_���W�̐ݒ� */
	my_std_obj->origin.y256 = ((origin_y)<<8);		/* ���_���W�̐ݒ� */
	obj_move_refresh();
}

/*---------------------------------------------------------
OBJ look,0
OBJ look,[�I�u�W�F�N�g�w��ԍ�]
	[ "i_code_" �V�i���I��b�V�X�e�������R�}���h]
	I_CODE_00_OBJ_LOOK ���ڃI�u�W�F�̔ԍ���ݒ�B
---------------------------------------------------------*/

MY_ICODE_FUNC(i_code_obj_look)
{
	/* �V�i���I��b�V�X�e�������R�}���h�̃p�����[�^�[���󂯎��B */
//	char *color32_str;		color32_str = ssc->para0;
/*	int mode;	*/			obj_look_up 		= ssc->para1;/*(�I�u�W�F�N�g�w��ԍ�)*/
	obj_move_refresh();
}


/*---------------------------------------------------------
OBJ_LOAD tachie/reimu2.png,0
OBJ_LOAD [�t�@�C����(data����̑��΃p�X)],[�I�u�W�F�N�g�w��ԍ�]
	[ "i_code_" �V�i���I��b�V�X�e�������R�}���h]
	�I�u�W�F�N�g�ɉ摜��ǂݍ���
	i_code_obj_load_gazo
	[ "i_code_" �V�i���I��b�V�X�e�������R�}���h]
	�ǂݍ��ݍς݂̃I�u�W�F�N�g�̉摜���ēǂݍ��݂��āA
	�ʒu�����̂܂܌p�����摜�͓���ւ���B
	i_code_swap_image_sprite
---------------------------------------------------------*/

MY_ICODE_FUNC(i_code_obj_load_gazo)
{
	/* �V�i���I��b�V�X�e�������R�}���h�̃p�����[�^�[���󂯎��B */
//	char *filename; //	filename		= ssc->para0;
//	strcpy(filename, (&str_stack[str_stack_position][0]));str_stack_position++;
	int sitei_number;	sitei_number	= ssc->para1;/*(�I�u�W�F�N�g�w��ԍ�)*/
//
	sitei_number &= ((KAIWA_OBJ_99_MAX-1));
	/*(Gu�̏ꍇ�́A�ǂ݂��ݏ���)*/
	{
	//	char my_file_common_name[256/*128*/];
		strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR "/" );
		strcat(my_file_common_name, (&str_stack[str_stack_position][0]));
		//
		strcpy(&my_resource[TEX_09_TACHIE_L+sitei_number].file_name[0], (&my_file_common_name[0]) );
		//
		str_stack_position++;/*pop_string();(������X�^�b�N����������)*/
	}
	psp_load_texture(TEX_09_TACHIE_L+sitei_number);
	#if 0
	/*(�v��Ȃ��H) (�ʂŐݒ肷��Ηv��Ȃ�)  */
	kaiwa_obj_set2n(sitei_number);
	#endif
}



/*---------------------------------------------------------
OBJ draw,0,1
OBJ draw,[�I�u�W�F�N�g�w��ԍ�],[�\��:1/��\��:0�̐؂�ւ��B]
	[ "i_code_" �V�i���I��b�V�X�e�������R�}���h]
	�I�u�W�F�N�g�̕\��/��\���̐؂�ւ��B
---------------------------------------------------------*/
MY_ICODE_FUNC(i_code_obj_draw)
{
	/* �V�i���I��b�V�X�e�������R�}���h�̃p�����[�^�[���󂯎��B */
//	char *filename; //	filename		= ssc->para0;
//	strcpy(filename, (&str_stack[str_stack_position][0]));str_stack_position++;
	int sitei_number;	sitei_number	= ssc->para1;/*(�I�u�W�F�N�g�w��ԍ�)*/
	int on_off; 		on_off			= ssc->para2;/*(�\��/��\���̐؂�ւ��B)*/
//	int yy; 			yy				= ssc->para3;
//	kaiwa_obj_mono_draw_on_off(sitei_number, on_off);
	kaiwa_sprite[(sitei_number&(2-1))].draw_flag = (on_off);	/* �`��on/off�B */
}
#if (1==USE_AFTER_LOAD_STAGE)
/*---------------------------------------------------------

---------------------------------------------------------*/
extern void load_stage_data(void);
extern void init_stage_start_time(void);
MY_ICODE_FUNC(i_code_load_stage)
{
	/* �V�i���I��b�V�X�e�������R�}���h�̃p�����[�^�[���󂯎��B */
//	char *filename; //	filename		= ssc->para0;
//	strcpy(filename, (&str_stack[str_stack_position][0]));str_stack_position++;
//	int sitei_number;	sitei_number	= ssc->para1;
//
//	sitei_number &= ((KAIWA_OBJ_99_MAX-1));
	/*(Gu�̏ꍇ�́A�ǂ݂��ݏ���)*/
	{
	//	char temp_file_name[256/*128*/];
		strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR "/");
		strcat(my_file_common_name, (&str_stack[str_stack_position][0]));
		//
	//	strcpy(&my_resource[TEX_09_TACHIE_L+sitei_number].file_name[0], (&my_file_common_name[0]) );
		//
		str_stack_position++;/*pop_string();(������X�^�b�N����������)*/
	}
	#if (0)/*(�f�o�b�O)*/
	{	/* �ǂݍ��݃e�X�g */
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
			"kaiwa: %d �ʂ̓����G�ݒ�" "\\n"
			"�t�@�C����ǂ݂܂��B" "\\n"
			"%s",
			/*load_stage_number*/cg.game_now_stage,
			my_file_common_name );
	}
	#endif
//	psp_load_texture(TEX_09_TACHIE_L+sitei_number);
	{
		/* Load next stage */
		load_stage_data();//	�X�e�[�W�ǂݍ��݁B
		init_stage_start_time();	// ���[�h���͏����������Ă���̂ŁA���[�h��Ɏ��Ԃ��č쐬����B
	}
}
#endif
/*---------------------------------------------------------
	���s��~�����p��
---------------------------------------------------------*/
enum /* �}���`�^�X�N�@�\�̃^�X�N�X�C�b�` */
{																		// �Ӗ��B
	TASK_SWITCH_00_EXECUTE							= 0x00,/*(0<<0)*/	// ��b�X�N���v�g�������s���B

	TASK_SWITCH_01_HOLD_REASON_KEY_CHECK_ON_BOOT	= 0x01,/*(1<<0)*/	// �X�N���v�g�J�n���ɓ��ʂɁA�L�[���͑҂����s�����ׁ̈A�X�N���v�g��~���B
	TASK_SWITCH_02_HOLD_REASON_KEY_HOLD_MODE		= 0x02,/*(1<<1)*/	// �L�[���͑҂��ŃX�N���v�g������~���B
	TASK_SWITCH_04_HOLD_REASON_WAIT_COUNT			= 0x04,/*(1<<2)*/	// �w��E�F�C�g�҂��ŃX�N���v�g������~���B
	//
	TASK_SWITCH_10_HOLD_REASON_TEXT_DRAWING 		= 0x10,/*(1<<4)*/	// �e�L�X�g�`�撆�ŃX�N���v�g��~���B
	TASK_SWITCH_20_HOLD_REASON_OBJECT_WAIT			= 0x20,/*(1<<5)*/	// �I�u�W�F�N�g�ړ��ʒu�܂ňړ�����̂�҂����B(�I�u�W�F�N�g�ړ�������Ԃ܂ŃX�N���v�g��~���đ҂�)
};
// HOLD_REASON == (��b�X�N���v�g)��~���R�B
enum
{
	KAIWA_SYSTEM_00_CONTINUE = 0,
	KAIWA_SYSTEM_01_TERMINATE,
};
static u8 kaiwa_system_hold_mode;
static u8 kaiwa_system_skip_mode;
static u8 kaiwa_system_terminate_flag;

static void kaijyo_obj_all_stop(void)
{
	int check_flag;
	check_flag = 0;
	/* �SOBJ����~���Ă��邩�`�F�b�N����B */
	int nnn;
	for (nnn=0; nnn<KAIWA_OBJ_99_MAX; nnn++)
	{
		KAIWA_OBJ *my_std_obj;
		my_std_obj = &kaiwa_sprite[nnn]; /* �ėp�I�u�W�F */
		if (KAIWA_OBJ_MOVE_FLAG_00_MOVE_COMPLETE != my_std_obj->move_flag)
		{
			check_flag = (1);/* �ړ��� */
		}
	}
	/* �SOBJ����~���Ă���ꍇ�̂݁A�����B */
	if (0==check_flag)/* �SOBJ��~���Ă���B */
	{
		kaiwa_system_hold_mode &= ~TASK_SWITCH_20_HOLD_REASON_OBJECT_WAIT;/* ���� */
	}
}


/*---------------------------------------------------------
	��b�ʏ펞�̃p�b�h�`�F�b�N
	-------------------------------------------------------
	�������Ԃ́A�{�ƂƓ���10[�b]�Ƃ���B
	-------------------------------------------------------
	�Ō�Ɋ�����ʂ�S�s�����B
---------------------------------------------------------*/

static int limit_timer;/*(��������)*/
static void kaijyo_pad_timer_hold(void)
{
	/* ==== �{�^�����͏��� ==== */
	u8 shot_osita;
	shot_osita = 0; /* �V���b�g������ */
	if (0 == psp_pad.pad_data)/* ���͉����ĂȂ� */
	{
		if (psp_pad.pad_data_alter & PSP_KEY_BOMB_CANCEL)	/* �L�����Z������ */
		{
			kaiwa_system_skip_mode = (1);/*on*/
		}
		if (psp_pad.pad_data_alter & PSP_KEY_SHOT_OK)		/* �n�j���� */
		{
			shot_osita = 1; 	/* �V���b�g�������� */
		}
	}
	{
		if (0 != (shot_osita | /*�v�����or*/kaiwa_system_skip_mode))/*(�V���b�g������)*/
		{
			limit_timer = byou60(0);/*(0[�b]���Ԑ؂�)*/
		}
		limit_timer--;/*(���Ԍo��)*/
		if (byou60(0) > limit_timer)/*(0[�b]���Ԑ؂�)*/
		{
			kaiwa_system_hold_mode &= ~TASK_SWITCH_02_HOLD_REASON_KEY_HOLD_MODE;/* ���� */
			#if 1	/* �Ō�̂ݍs�� */
			kanji_window_all_clear();				/* ������ʂ�S�s�����B�����J�[�\�����z�[���|�W�V�����ֈړ��B */
			#endif
		}
	}
}

/*---------------------------------------------------------
	vsync�҂��B
	-------------------------------------------------------
	�Ō�Ɋ�����ʂ�S�s�����B
---------------------------------------------------------*/
static int kaiwa_system_wait_mode;

static void kaijyo_vsync_wait(void)
{
	kaiwa_system_wait_mode--;
	if (0 >= kaiwa_system_wait_mode)
	{
		kaiwa_system_hold_mode &= ~TASK_SWITCH_04_HOLD_REASON_WAIT_COUNT;/* ���� */
		#if 1	/* �Ō�̂ݍs�� */
		kanji_window_all_clear();				/* ������ʂ�S�s�����B�����J�[�\�����z�[���|�W�V�����ֈړ��B */
		#endif
	}
}

/*---------------------------------------------------------
	��b�N�����̃p�b�h�`�F�b�N
	-------------------------------------------------------
	�{�^����������Ă���̂��m�F����B
	���ꂪ�����ƁA�V�i���I������O�����΂���鎖������B
	-------------------------------------------------------
	��b�N�����ɁA�{���{�^����������Ă���ƁA
	��b������O����A�X�L�b�v���[�h�ɂȂ�̂ŁA
	��b���ǂ߂Ȃ��B
	�����h�~����ׂ̓��ʋ@�\�B
	-------------------------------------------------------
	�������A�{�^���������Â��Ă���ƁA���܂Ōo���Ă�
	��b���n�܂�Ȃ��̂ŁA�������Ԃ�ݒ肷��B
	�������Ԃ́A�Ƃ肠����1[�b]�ɂ��Ƃ����B
---------------------------------------------------------*/

static void kaijyo_pad_boot_hold(void)
{
	/* ==== �{�^�����͏��� ==== */
	{
		if (0 == psp_pad.pad_data)/* ���͉����ĂȂ� */
		{
			limit_timer = byou60(0);/*(0[�b]���Ԑ؂�)*/
		}
		limit_timer--;/*(���Ԍo��)*/
		if (byou60(0) > limit_timer)/*(0[�b]���Ԑ؂�)*/
		{
			kaiwa_system_hold_mode &= ~TASK_SWITCH_01_HOLD_REASON_KEY_CHECK_ON_BOOT;/* ���� */
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static int i_code_text_color;		/* �e�L�X�g���b�Z�[�W�`��F */
static int i_code_text_speed_wait;	/* �e�L�X�g���b�Z�[�W�`��E�F�C�g */

//0static int i_code_shot_ositenai; /* */
static char i_code_text_para0[256];

static void kaijyo_text_drawing(void)
{
//	if (0 == 0/*i_code_shot_ositenai*/) 	{	i_code_text_speed_wait=0;	}	/* �V���b�g��������A�c��͑S������ */
	/*sdl_lock_surface,*/
	strcpy(my_font_text, i_code_text_para0);
	kanji_color(i_code_text_color);
	kanji_wait(i_code_text_speed_wait);
	int osimai;
	osimai = 0;
	osimai = kanji_draw();
	kanji_wait(0);
	if (0!=osimai)
	{
		kaiwa_system_hold_mode &= ~TASK_SWITCH_10_HOLD_REASON_TEXT_DRAWING;/* ���� */
	}
}


/*---------------------------------------------------------
	�V�i���I��b�V�X�e���A�I�u�W�F�N�g�̈ړ�
---------------------------------------------------------*/

static void kaiwa_system_vbl_sprite_move(void)
{
	int nnn;
	for (nnn=0; nnn<KAIWA_OBJ_99_MAX; nnn++)
	{
		KAIWA_OBJ *my_std_obj;
		my_std_obj = &kaiwa_sprite[nnn]; /* �ėp�I�u�W�F */
		if (KAIWA_OBJ_MOVE_FLAG_00_MOVE_COMPLETE != my_std_obj->move_flag )
		{
			my_std_obj->toutatu_wariai256 += (6);/*(8 �����G�A���x)*/
			/* �ړ��������W�ɓ��������͂ݏo������A�����Ƃ���B */
			if (my_std_obj->toutatu_wariai256 >= t256(1))
			{
				my_std_obj->move_flag	= KAIWA_OBJ_MOVE_FLAG_00_MOVE_COMPLETE;/*�ړ�����*/
				//return /*(1)*/;/*�ړ�����*/
			}
			//
			int move_rate256;
			move_rate256 = my_std_obj->toutatu_wariai256;
			move_rate256 = psp_min(move_rate256, 256);
			move_rate256 = psp_max(move_rate256, 0);
			#if (1)/* �񓙕��l�̌v�Z */
			{
				u32 i_rate65536;
				i_rate65536 = (move_rate256 << 8);
			//	i_rate65536 = (int)(vfpu_ease_in_out(t)*256.0);
				i_rate65536 = (int)(vfpu_ease_in_out65536(i_rate65536));	/* t65536�̓����J�E���^�l���g���A�񓙕��ɂȂ�悤�l���C������B
				�񓙕��̕����́uease_in_out�v����́A�n�߂ƍŌオ�������ŊԂ������Ƃ����z�������B */
				move_rate256 = (i_rate65536 >> 8);
			}
			#endif
			//------------------
			#if (1)/*(���B�������l�����āA��������B)*/
			REG_00_SRC_X	= ((my_std_obj->alt.x256)); 	/*(�����ʒu[A]t256()�`��)*/
			REG_01_SRC_Y	= ((my_std_obj->alt.y256)); 	/*(�����ʒu[A]t256()�`��)*/
			REG_02_DEST_X	= ((my_std_obj->target.x256));	/*(�����ʒu[B]t256()�`��)*/
			REG_03_DEST_Y	= ((my_std_obj->target.y256));	/*(�����ʒu[B]t256()�`��)*/
			REG_11_GOUSEI_WARIAI256 	= move_rate256;/*(��������t256()�`��)*/
			multiprex_rate_vector();/*(�j�󃌃W�X�^�����̂Œ���)*/
			#endif
			// ���̎��_�ŁA
			// REG_02_DEST_X: �����l
			// REG_03_DEST_Y: �����l
			//-----------------------
			my_std_obj->center.x256 = (REG_02_DEST_X);
			my_std_obj->center.y256 = (REG_03_DEST_Y);
		}
	}
}


/*---------------------------------------------------------
	�V�i���I��b�V�X�e���A�������s����
	�V�i���I��b�V�X�e�� �R�}���h����(���s)�n
---------------------------------------------------------*/
/* ��b�V�X�e������Ă΂����� */
#if 1
extern void called_from_kaiwa_system_boss_load(int boss_number);
#endif

static /*int*/void kaiwa_system_execute(void)
{
	KAIWA_YOUSO *ssc;
	ssc = kaiwa_youso_list_scan;/* ���s�J�n�ʒu */
//
	#if (1==USE_AFTER_LOAD_STAGE)
		/*(�Ȃ�)*/
	#else
		#define case_I_CODE_1c_STAGE_LOAD I_CODE_break
	#endif
	void *aaa[I_CODE_99_MAX] =
	{
	// OBJ�֘A
		&&case_I_CODE_00_OBJ_LOOK,
		&&case_I_CODE_01_OBJ_XY,
		&&case_I_CODE_02_OBJ_LOAD,
		&&case_I_CODE_03_OBJ_WAIT,
		//
		&&case_I_CODE_04_OBJ_DRAW_ON_OFF,/*(r36�V��)*/
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
	// ��ʐ���
		&&case_I_CODE_08_DRAW_TEXT,
		&&case_I_CODE_09_DRAW_BG,
		&&case_I_CODE_0a_DRAW_PANEL,
		&&case_I_CODE_0b_DRAW_SKIP,
		//
		&&case_I_CODE_0c_DRAW_WAIT,/*(r36�V��)*/
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
	// �e�L�X�g�֘A
		&&case_I_CODE_10_TEXT,
		&&case_I_CODE_11_HOLD,
		&&case_I_CODE_12_FONT_COLOR,
		&&case_I_CODE_13_FONT_SPEED,
	// �w�i/BGM
		&&case_I_CODE_14_BG_LOAD,
		&&case_I_CODE_15_BG_FADE,
		&&case_I_CODE_16_BGM,
		&&case_I_CODE_17_BGM_VOLUME,
	// �{�X����
		&&case_I_CODE_18_BOSS_LOAD,
		&&case_I_CODE_19_BOSS_TERMINATE,/*(r34�V��)*/
		&&I_CODE_break, /*(�K�v�_�~�[)*/	// &&case_I_CODE_1a_BOSS_START,
		&&I_CODE_break, /*(�K�v�_�~�[)*/	// &&case_I_CODE_1b_BOSS_RESULT,/*(r35�V��)*/
	//
		&&case_I_CODE_1c_STAGE_LOAD,/*(r36�V��)*/
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
	//
	//	I_CODE_99_MAX/*(2^n)*/	// �K�v�Ȃ������S�̈�
	};
	goto *aaa[(ssc->i_code)&(0x1f)];/*(I_CODE_99_MAX-1)*/
	{
	case_I_CODE_00_OBJ_LOOK:			MY_ICODE_CALL(i_code_obj_look); 					goto I_CODE_break;	/* �I�u�W�F�N�g���ڃR�}���h */
	case_I_CODE_01_OBJ_XY:				MY_ICODE_CALL(i_code_obj_xy);						goto I_CODE_break;	/*(���_�ݒ�̂�)*/
	case_I_CODE_02_OBJ_LOAD:			MY_ICODE_CALL(i_code_obj_load_gazo);				goto I_CODE_break;	/*(�ǂ݂��݂̂�) �ėp�G  �����GL	�����GR */
	case_I_CODE_04_OBJ_DRAW_ON_OFF: 	MY_ICODE_CALL(i_code_obj_draw); 					goto I_CODE_break;	/*(�\���� 0:off / 1:on ) �ėp�G  �����GL	�����GR */
	//
	case_I_CODE_03_OBJ_WAIT:			kaiwa_system_hold_mode |= TASK_SWITCH_20_HOLD_REASON_OBJECT_WAIT; /* ��~ */	goto I_CODE_break;
	//
	case_I_CODE_08_DRAW_TEXT:
		#if (0)/*(�ŋ߂�(gcc4.3.5)�͂��Ԃ񓯂��Bgcc4.0.2���炢�Â��ƈႤ�B)*/
		if (ssc->para1&1)	{	ml_font[(0)].haikei 		= (ML_HAIKEI_03_MESSAGE);/* [��/��b�p�w�i]����Ӕw�ion */}
		else				{	ml_font[(0)].haikei 		= (ML_HAIKEI_m1_OFF);/* ����Ӕw�ioff */}
		#else
		{	ml_font[(0)].haikei 		= (ssc->para1&1)?(ML_HAIKEI_03_MESSAGE)/* [��/��b�p�w�i]����Ӕw�ion */:(ML_HAIKEI_m1_OFF);/* ����Ӕw�ioff */}
		#endif
		goto I_CODE_break;
	case_I_CODE_09_DRAW_BG: 		draw_bg 					= ssc->para1;				goto I_CODE_break;/* view bg, 0:on, 1:off */
	case_I_CODE_0a_DRAW_PANEL:		cg.side_panel_draw_flag 	= ssc->para1;				goto I_CODE_break;
	case_I_CODE_0b_DRAW_SKIP:		kaiwa_system_skip_mode		= ssc->para1;				goto I_CODE_break;
	case_I_CODE_0c_DRAW_WAIT:		kaiwa_system_wait_mode		= ssc->para1;		kaiwa_system_hold_mode |= TASK_SWITCH_04_HOLD_REASON_WAIT_COUNT;						goto I_CODE_break;
	//
	case_I_CODE_10_TEXT:
				strcpy(i_code_text_para0, (&str_stack[str_stack_position][0]));str_stack_position++;/*pop_string();(������X�^�b�N����������)*/
			//	strcpy(i_code_text_para0, ssc->para0);
					kaiwa_system_hold_mode |= TASK_SWITCH_10_HOLD_REASON_TEXT_DRAWING;		goto I_CODE_break;
	case_I_CODE_11_HOLD:			limit_timer 				= byou60(10);/*(10[�b]�������ԁB�{�ƌ݊�)*/ 	kaiwa_system_hold_mode |= TASK_SWITCH_02_HOLD_REASON_KEY_HOLD_MODE; goto I_CODE_break;
	case_I_CODE_12_FONT_COLOR:		i_code_text_color			= ssc->para1;				goto I_CODE_break;
	case_I_CODE_13_FONT_SPEED:		i_code_text_speed_wait		= ssc->para1;				goto I_CODE_break;
	//
	case_I_CODE_14_BG_LOAD: 		MY_ICODE_CALL(i_code_load_bg);							goto I_CODE_break;
	case_I_CODE_15_BG_FADE: 		MY_ICODE_CALL(i_code_fade_bg);							goto I_CODE_break;
	case_I_CODE_16_BGM: 			play_music_num( (ssc->para1) ); 						goto I_CODE_break;
	case_I_CODE_17_BGM_VOLUME:		set_music_volume( (ssc->para1) );						goto I_CODE_break;
	case_I_CODE_18_BOSS_LOAD:		called_from_kaiwa_system_boss_load( (ssc->para1) ); 	goto I_CODE_break;/*boss_number*/
	case_I_CODE_19_BOSS_TERMINATE:
									#if (1)//(USE_r36_SCENE_FLAG)
	/* off / �����R�}���h�ǉ��ǂݍ��ݏ������~����B */
//	cg.state_flag		&= (~SCENE_NUMBER_MASK);	/*(�V�[��������)*/
	cg.state_flag		&= (0xffff00ffu);	/*(�V�[��������)*/
	cg.state_flag		|= (0x00008000u); //�v���C���[���[�v�𔲂��鏈��(�Ƃ肠����????)
									#endif
																							goto I_CODE_break;
	#if (1==USE_AFTER_LOAD_STAGE)
	case_I_CODE_1c_STAGE_LOAD:		MY_ICODE_CALL(i_code_load_stage);						goto I_CODE_break;	/*(�ǂ݂��݂̂�) �ėp�G  �����GL	�����GR */
	#endif
		/*	called_from_kaiwa_system_boss_start();*/
		/*	cg.state_flag |= STATE_FLAG_0x0800_IS_BOSS; */
		/* �{�X�퓬�O�̉�b�I����ݒ� */
//	case_I_CODE_1a_BOSS_START:		/*�_�~�[*/; 											goto I_CODE_break;
//	case_I_CODE_1b_BOSS_RESULT: 	/*�_�~�[*/; 											goto I_CODE_break;
	//
//	case_I_CODE_OBJ_SPEED:			MY_ICODE_CALL(i_code_set_sprite_speed); 				goto I_CODE_break;
	// [�e�L�X�g�̃J�[�\������]
//	case I_CODE_CUR_POP:			cursor_x = cursor_x_chached;	cursor_y = cursor_y_chached;		break;	/* �J�[�\���ʒu�A���� */
//	case I_CODE_CUR_PUSH:			cursor_x_chached = cursor_x;	cursor_y_chached = cursor_y;		break;	/* �J�[�\���ʒu�A�L�� */
//	case I_CODE_CUR_HOME:			kanji_cursor_move_home_position();						break;	/* �����J�[�\�����z�[���|�W�V�����ֈړ� */		/* �J�[�\���̏����� */
//	case I_CODE_BG_WRITE_TEXT:		set_write_text(1);					break;	/* BG�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
//	case I_CODE_10_TEXT_WRITE_TEXT: set_write_text(0);					break;	/* ���b�Z�[�W�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
	// [�e�L�X�g�\��/�N���b�N�҂�]
	}
I_CODE_break:
	;
	/* ����p�̓������ƏI�����f */
	//
	/* (�I���̏ꍇ�A���̃R�}���h�𒲂ׂ�) */
	kaiwa_youso_list_scan = kaiwa_youso_list_scan->next;/* ����͂������璲�ׂ� */
	if (NULL == kaiwa_youso_list_scan)/* �V�i���I��b���Ō�Ȃ� */
	/* (
		calloc() ���Ċm�ۂ����̂ŁA�Öق� NULL �������Ă���Ƃ����A������ƃg���b�L�[�ȃR�[�h�B(calloc() �� 0�N���A����)
		�܂� malloc ���Ƒʖڂ����B(malloc() �� 0�N���A���Ȃ�)
		���g�p����Ȃ����A���܂�D�܂����Ȃ��R�[�h�B(C����̋K�i�� 0==NULL �́A�K�i�O�Ȃ̂Ŗ{���ǂ��Ȃ��B)
		)
	*/
	{
		/* �V�i���I��b���Ō�Ȃ� */
		kaiwa_system_terminate_flag = KAIWA_SYSTEM_01_TERMINATE;/*(1)*/ 	/* �V�i���I��b�����͂����܂� */
	}
	else
	{
		kaiwa_system_terminate_flag = KAIWA_SYSTEM_00_CONTINUE;/*(0)*/		/* �V�i���I��b�����͂Â� */
	}
}


/*---------------------------------------------------------
	��b�V�X�e�� SDL_BG�`��
---------------------------------------------------------*/

global void kaiwa_system_SDL_BG_draw(void)
{
	/* SDL_BG�`��A�x������B(Gu������Ƃ����͖����Ȃ�\��) */
	if (re_draw_BG_count_flag)
	{
		re_draw_BG_count_flag--;
		{
			#define USE_SDL_DRAW (1)
			#if (1==USE_SDL_DRAW)
			/* ��b�V�X�e�����̃I�u�W�F�N�g�`�揈�� */
			#if /*???????*/0/*Gu������������v��Ȃ��Ȃ�*/
			{
				psp_clear_screen(); /* [PAUSE] ���A����SDL��ʂ������B */
			}
			#endif
			/* ----- �`��n ----- */
			/* 1. �܂��w�i��`�� */
			if (draw_bg)
			{
			//	#if 1/*(���܂������Ȃ�)*/
			//	if (255 > bg_alpha)
			//	{
			//		SDL_SetAlpha(bg_st ory_window_surface, SDL_SRCALPHA, bg_alpha);
			//	}
			//	#endif
			//	PSPL_UpperBlit(bg_st ory_window_surface, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], NULL);
				psp_pop_screen();
			}
			/* 2. ���ɗ����G��`�� */
			#endif /* (1==USE_SDL_DRAW) */
		}
	}
}


/*---------------------------------------------------------
	���Z�b�g
---------------------------------------------------------*/

//static extern int my_string_offset;
/*static*/ extern void clear_my_string_offset(void);
static void aaa_kaiwa_system_reset(void)
{
	draw_bg = (0);/* bg �\�� off */
	//
	kanji_cursor_move_home_position();		/* �����J�[�\�����z�[���|�W�V�����ֈړ� */
	clear_my_string_offset();	//	my_string_offset=0;
}

/*---------------------------------------------------------
	��b���[�h�I������
---------------------------------------------------------*/

/*static*/global void do_kaiwa_system_terminate(void)
{
//	bg_alpha = 0;
	ml_font[(0)].haikei 		= (ML_HAIKEI_m1_OFF);/* ����Ӕw�ioff */
	load_kaiwa_youso_free();	/* �O��̃V�i���I���������ɂ���΂��ׂĊJ���B */
	aaa_kaiwa_system_reset();
//	draw_bg = 0;/* (�`��ƕ����ł��Ȃ�)�o�O�C�� */
}


/*---------------------------------------------------------
	�Q�[���R�A�p�A��b���[�h�I����Ԃ֑J��
---------------------------------------------------------*/
static void kaiwa_system_terminate_game_core(void)
{
	NEXT_SCENE;
}


/*---------------------------------------------------------
	�V�i���I��b ���쒆
---------------------------------------------------------*/
static void (*kaiwa_system_terminate_call_func)(void);

global void kaiwa_system_execute_move_only_main(void)
{
	/*---------------------------------------------------------
		(r33)����ƁA����ƕ`�悪�����ł����|�B
		�����ǁAGu���A�Ԃɂ���Ȃ������B
	---------------------------------------------------------*/
	if (TASK_SWITCH_00_EXECUTE==kaiwa_system_hold_mode)/* ���s��~��ԂłȂ���Ύ��s����B */
	{	/* ���s��~��ԂłȂ��B */
		kaiwa_system_execute(); 	/* static_�V�i���I��b ���� */		/* ����̂�(�`�悵�Ȃ�) */
	}
	else	/* ���s��~��ԁB */
	{	/* ���s��~��Ԃ̏ꍇ�́A���s��~�����p�����`�F�b�N */
		/* OBJ_WAIT�̊J�n�p�� */
		if (kaiwa_system_hold_mode & TASK_SWITCH_20_HOLD_REASON_OBJECT_WAIT)
		{
			kaijyo_obj_all_stop();			/* �SOBJ����~���Ă�����A�����B */
		}
		if (kaiwa_system_hold_mode & TASK_SWITCH_04_HOLD_REASON_WAIT_COUNT)
		{
			kaijyo_vsync_wait();			/* wait�I��������A�����B */
		}
		if (kaiwa_system_hold_mode & TASK_SWITCH_10_HOLD_REASON_TEXT_DRAWING)
		{
			/* ==== text�`�揈��(�񓯊�) ==== */
			kaijyo_text_drawing();			/* �`��I���Ȃ�A�����B */
		}
	#if (1)//(USE_r36_SCENE_FLAG)
		/*
			r36���瓹���C�x���g�ŃX�N���v�g���Ă΂��悤�ɂȂ�܂����B
			�����ŃL�[���͑҂�������ƁA�V���b�g�����ĂȂ��̂ŁA
			���������Ȃ�A�L�[���͑҂��C�x���g�͏���ɉ������܂��B(����͎b��I�Ȏd�l�ł�)
			---------------------------------------------------------
			�����I�ɒ��{�X��b����������ꍇ�A
			�X�N���v�g��̃R�}���h�Ń^�X�N�֎~���R���g���[���o����悤�ɂ��܂��B
			(�^�X�N�A�}�X�N�R�}���h��V�݂���B)
		*/
		if (0==(cg.state_flag&0x0c00))// ����(�X�e�[�W�^�C�g��)�ł́A�L�[���͑҂������Ȃ��B
		{	/*(���������Ȃ�A)*/
			/*(����ɉ���)*/
			kaiwa_system_hold_mode &= ~(TASK_SWITCH_02_HOLD_REASON_KEY_HOLD_MODE|TASK_SWITCH_01_HOLD_REASON_KEY_CHECK_ON_BOOT);
		}
		else
	#endif
		{
			if (kaiwa_system_hold_mode & TASK_SWITCH_02_HOLD_REASON_KEY_HOLD_MODE)
			{
				kaijyo_pad_timer_hold();		/* ���Ԍo�߂������́A�{�^����������A�����B */
			}
			if (kaiwa_system_hold_mode & TASK_SWITCH_01_HOLD_REASON_KEY_CHECK_ON_BOOT)
			{
				kaijyo_pad_boot_hold(); 		/* �N������p�B�p�b�h�̃{�^���������ꂽ��A�����B */
			}
		}
	}
	/* ==== BG �ݒ�ύX����(�񓯊�) ==== */
	kaiwa_system_vbl_do_BG_fade();
	/* ==== obj �ړ�����(�񓯊�) ==== */
	kaiwa_system_vbl_sprite_move(); /* ����̂�(�`�悵�Ȃ�) */

	if (/*(0)*/KAIWA_SYSTEM_00_CONTINUE != kaiwa_system_terminate_flag)
	{
		do_kaiwa_system_terminate();
		kaiwa_system_terminate_call_func();
	}
}

/*---------------------------------------------------------
	init obj position.
---------------------------------------------------------*/
/*static*/ void kaiwa_init_obj_position(void)
{
	// �v���C���[
	kaiwa_sprite[0].center.x256 = t256(-96);		kaiwa_sprite[0].center.y256 = t256(16);
	kaiwa_sprite[0].origin.x256 = t256(-96);		kaiwa_sprite[0].origin.y256 = t256(16);
	kaiwa_sprite[0].offset.x256 = t256(-32);		kaiwa_sprite[0].offset.y256 = t256(16);
	// �{�X
	kaiwa_sprite[1].center.x256 = t256(352);		kaiwa_sprite[1].center.y256 = t256(16);
	kaiwa_sprite[1].origin.x256 = t256(352);		kaiwa_sprite[1].origin.y256 = t256(16);
	kaiwa_sprite[1].offset.x256 = t256( 32);		kaiwa_sprite[1].offset.y256 = t256(16);
}

/*---------------------------------------------------------
	kaiwa obj draw on / off.
---------------------------------------------------------*/
/*static*/global void kaiwa_all_obj_draw_on_off(unsigned int on_off)
{
	kaiwa_sprite[0].draw_flag = (on_off);	/* �`�悵�Ȃ��B */
	kaiwa_sprite[1].draw_flag = (on_off);	/* �`�悵�Ȃ��B */
}

/*---------------------------------------------------------
	�V�i���I��b�A����A�J�n����
---------------------------------------------------------*/

static int aaa_kaiwa_system_start(void) /* �V�i���I�t�@�C�����Ɣw�i�t�@�C���� */
{
	if (0 == s_load_scenario())
	{
		return (0);/* �ǂݍ��ݎ��s */
	}
	aaa_kaiwa_system_reset();
	#if 1
		#if 0/*(aaa_kaiwa_system_reset();�Əd��)*/
	/* std_obj[]������ */
//	{	unsigned int i;
//		for (i=0; i<KAIWA_OBJ_99_MAX; i++)/*20*/
//		{
//			kaiwa_sprite[i].img = NULL;
//		}
//	}
//	draw_bg 					= (0);
		#endif
	kaiwa_init_obj_position();
	obj_look_up 				= (KAIWA_OBJ_99_MAX)+(1);/*(�S�����ڃI�u�W�F�ɐݒ�)*/
	#endif
	//
//	kaiwa_all_obj_draw_on_off(1);	/* �����G��`�悷��B */
	//
	kanji_init_standard();/*(�����֘A�̕W��������)*/
	//
	kaiwa_system_skip_mode		= (0);/*off*/
	kaiwa_system_hold_mode		=
		(	TASK_SWITCH_01_HOLD_REASON_KEY_CHECK_ON_BOOT	/*(�J�n���̓p�b�h��������Ă��鎖���m�F����)*/
		|	TASK_SWITCH_20_HOLD_REASON_OBJECT_WAIT			/*(�J�n����obj���ޔ����Ă��鎖���m�F����)*/
		);
	kaiwa_system_terminate_flag = KAIWA_SYSTEM_00_CONTINUE;/*(0)*/	/* ������ */

	ml_font[(0)].haikei 		= (ML_HAIKEI_03_MESSAGE);/* [��/��b�p�w�i]����Ӕw�ion */
	limit_timer 				= byou60(1);/*(1[�b]�������ԁB�N�����̃p�b�h�`�F�b�N����)*/
	i_code_text_color			= (7);
	i_code_text_speed_wait		= (0);
	str_stack_position = 0;/* ������̈� */
	return (1);
}


/*---------------------------------------------------------
	�Q�[���C�x���g���̃V�i���I��b �J�n(�X�g�[���A�G���f�B���O�ȊO)
---------------------------------------------------------*/
/*
r35 r36x
-:	 0: �X�e�[�W�J�n�C�x���g
0:	 1: �{�X�퓬�O��b�C�x���g
1:	 2: �{�X�퓬���b�C�x���g1
2:	 3: �{�X�퓬���b�C�x���g2


*/
global void kaiwa_load_ivent(void)
{
/*(USE_r36_SCENE_FLAG)*/
	/* �t�@�C�����쐬 */
	{
		/* 'data' '/kaiwa/' �̕��̃I�t�Z�b�g */
		#define DIRECTRY_NAME_OFFSET	(DIRECTRY_NAME_DATA_LENGTH + DIRECTRY_NAME_KAIWA_LENGTH)
		strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR DIRECTRY_NAME_KAIWA_STR "Z/sZ0" DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR);
		my_file_common_name[DIRECTRY_NAME_OFFSET+0] = ('0'+(cg_game_select_player));
		my_file_common_name[DIRECTRY_NAME_OFFSET+3] = get_stage_chr(cg.game_now_stage);
		my_file_common_name[DIRECTRY_NAME_OFFSET+4] += ((cg.state_flag>>10) & (0x03));
		NEXT_SCENE;
		//
		//#if (1==USE_EASY_BADEND)
/*(�o�O����)*/if (/*'1'*/'2'==my_file_common_name[DIRECTRY_NAME_OFFSET+4])/*(bad���肪�K�v�Ȃ�bad���菈��)*/
		{
			/* 5�ʏI����easy�̏ꍇ�A���ꏈ��(BAD END) */
			{
			//	if ((5) == (cg.game_now_stage))/*(5�ʂ̏ꍇ)*/
				if (
					((5) == (cg.game_now_stage))/*(5�ʂ̏ꍇ)*/
					||
					((6) == (cg.game_now_stage))/*(6�ʂ̏ꍇ�A�B���G���h)*/
				)
				{
					if ((0)==(cg.game_difficulty))/*(easy�̏ꍇ)*/
					{
					//	my_file_common_name[DIRECTRY_NAME_OFFSET+4] = '2';
						my_file_common_name[DIRECTRY_NAME_OFFSET+4]++;/*(bad�m��)*/
					}
				}
			}
		}
		//#endif
	}
	kaiwa_system_terminate_call_func = kaiwa_system_terminate_game_core;	/* �V�i���I��b �I�������Q�[���R�A�p��ԑJ�� */
	load_kaiwa_youso_free();			/* �O��̃V�i���I���������ɂ���΂��ׂĊJ���B */
	if (0 == aaa_kaiwa_system_start())	// �t�@�C�����Ȃ��ꍇ�̓C�x���g���΂�
	{
		NEXT_SCENE;
	}
}


#if (1)/*Gu�`��ɂȂ�Ɨv��Ȃ��Ȃ�*/
/*---------------------------------------------------------
	�u�X�g�[���[���[�h��p�v�V�i���I����
---------------------------------------------------------*/

static MAIN_CALL_FUNC(story_mode_local_work)
{
	kaiwa_system_SDL_BG_draw(); 				/* �V�i���I��b�V�X�e�� SDL_BG �`��(�x��) */
	kaiwa_system_execute_move_only_main();		/* �V�i���I��b�V�X�e�� ����(�ړ�) */
}
#endif

/*---------------------------------------------------------
	�u�X�g�[���[���[�h��p�v�V�i���I�J�n
---------------------------------------------------------*/

global MAIN_CALL_FUNC(story_mode_start)
{
//	bg_alpha = 0;
	/* �t�@�C�����쐬 */
	{
		strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR DIRECTRY_NAME_KAIWA_STR "story" DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR);
	}
/*(�o�O����)*/	cg.state_flag |= 0x0400;
	kaiwa_system_terminate_call_func = menu_cancel_and_voice;		/* ��b���[�h�V�i���I���I�������^�C�g�����j���[�ֈړ� */
	load_kaiwa_youso_free();		/* �O��̃V�i���I���������ɂ���΂��ׂĊJ���B */
	aaa_kaiwa_system_start();		/* �X�g�[���[�p�̃V�i���I��b�V�X�e�� �J�n */
	#if 1/*Gu�`��ɂȂ�Ɨv��Ȃ��Ȃ�*/
	cb.main_call_func = story_mode_local_work;/* SDL�`��̏ꍇ */
	#else
//	cb.main_call_func = kaiwa_system_execute_move_only_main;/* Gu�`��̏ꍇ */
	#endif
}


/*---------------------------------------------------------
	psp�ł͊J�������퓮��o���Ȃ��̂ŁA�N�����Ɋm�ۂ���(�I�����܂�)�J�����Ȃ�
---------------------------------------------------------*/
/*only boot once*/global void kaiwa_system_boot_init(void)/* �g�ݍ��� */
{
	#if 1
	kaiwa_youso_list_bigin	= NULL; 	/* ���ߗ�̊J�n�ʒu��ێ� */
	kaiwa_youso_list_scan	= NULL; 	/* ���ߑ����ʒu�Ŏg�p */
//
	ml_font[(0)].haikei 		= (ML_HAIKEI_m1_OFF);/* ����Ӕw�ioff */
	draw_bg 					= (0);			/* 0:off �w�i�\���t���O */
	kanji_cursor_move_home_position();			/* �����J�[�\�����z�[���|�W�V�����ֈړ� */
	#endif
	bg_alpha					= (255);		/* 255==0xff �����l */

	/* SD L_FreeSurface(); �� psp�ł͑��������Ɠ����Ȃ��̂� ���̑΍� */
}


/*---------------------------------------------------------
	�I�����ɊJ�����镔��...�������ǁA
	�n���O�A�b�v�����肷��̂ŁA��芸���������ɂȂ��Ă�B
	(�J�����Ȃ��Ă��I������΁AOS���������������̂Ŗ��Ȃ����Č����Ζ��Ȃ�)
---------------------------------------------------------*/
#if 0/* �{���͗v�� */
/*only exit once*/global void kaiwa_system_boot_exit(void)/* �O�� */
{
	;
}
#endif
