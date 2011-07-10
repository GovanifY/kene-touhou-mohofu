
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�V�i���I��b ����
	-------------------------------------------------------
	���݂̖��_:
	�� ���x���x������B�Q�[���{�̂� 60fps �߂��œ����Ă�̂ɁA
	   �V�i���I������� SDL �̔��������� ���x������ 30-40fps ���x�ɒቺ����B
	�� SDL��� �� Gu ��ʂ̉摜����������ɏo���Ȃ��B
	   SDL �̔��������� �� Gu ��ʂƍ����ł��Ȃ��ׁA
		�������� ����� CPU�p���[�������� �\�t�g�E�F�A�[���������� �����Ă邪�A
		�P�ɈÂ��Ȃ邾���B
	(���̂��������Ӗ��A Gu ������� ������������ psp �̃n�[�h�E�F�A�[�ŊȒP�ɏo����̂�...)�B
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

/*static*/global KAIWA_SPRITE kaiwa_sprite[KAIWA_SPRITE_99_MAX];	/* �V�i���I��b �ėp�I�u�W�F */ /*32*/ /*20*/

/*---------------------------------------------------------
	�q�֐�
---------------------------------------------------------*/

/*---------------------------------------------------------
	�w��t�@�C�����̉摜��ǂ݂���SDL�T�[�t�F�C�X�ɐݒ肷��B
---------------------------------------------------------*/

static SDL_Surface *load_local01(char *filename, SDL_Surface *s2)// /*result*/, int alpha)
{
	char file_name[128];
	strcpy(file_name, DIRECTRY_NAME_DATA_STR "/" );
	strcat(file_name, filename);
//
	if (NULL != s2)
	{
		SDL_FreeSurface(s2);
		s2 = NULL;
	}
//
	SDL_Surface *s1;/*temporaly*/
//	s1 = NULL;
	s1 = IMG_Load(file_name);/*�����Ń��[�h����Ɗm���ɏ������������*/
	if ( NULL == s1 )
	{
		error(ERR_FATAL, (char*)"kaiwa: cant load image %s:", file_name );
//		return (NULL);
	}
#if 0
	if (	(-1==alpha) /* �V�i���I��b ���ŏȗ������ꍇ(�f�t�H���g) */
		||
			(0==alpha)	/* ���������ꍇ */
	)
#endif
	{
		/* �A���t�@�g��Ȃ� */
		s2 = SDL_DisplayFormat(s1);/*�T�[�t�F�X��\���t�H�[�}�b�g�ɕϊ�����B*/
		#if 1/* ����SDL�w�i������A story �ŗv��B */
		SDL_SetColorKey(s2, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);	/* �J���[�L�[(�����F�A�����F)�͍� */
		#endif
	}
#if 0
	else
	{
		/* �A���t�@�g�� */
		s2 = SDL_DisplayFormatAlpha(s1);/*�T�[�t�F�X��\���t�H�[�}�b�g�ɕϊ�����B*/
	}
#endif
	#if 0
	if ( NULL == s2 )
	{
		error(ERR_FATAL, (char*)"cant convert image %s to display format:", file_name );
//		return (NULL);
	}
	#endif
	SDL_FreeSurface(s1);
	s1 = NULL;
	return (s2);
}

/*---------------------------------------------------------
	�w��ԍ��̃I�u�W�F�N�g�摜��SDL�T�[�t�F�C�X���������B
---------------------------------------------------------*/

static void remove_kaiwa_sprite_num(int nnn)/*KAIWA_SPRITE *src*/
{
//	if (NULL != src)
	if (NULL != kaiwa_sprite[nnn].img)	/* �g�p���Ȃ� */
	{
		//if (NULL != std_obj[nnn])
		SDL_FreeSurface(kaiwa_sprite[nnn].img);/* �摜���J������B */
		kaiwa_sprite[nnn].img = NULL; //	free(src);	//	src = NULL;
	}
}

/*---------------------------------------------------------
	�C���^�v���^����(�I�y�R�[�h)�������������ǂށB
	10�����ȏ�̓G���[�B
---------------------------------------------------------*/

static char *read_opecode(char *c, char *buffer, int *end_arg)
{
	int i = 0;
	while ((*c != ' ') && (*c != 13))
	{
		i++;
		if (i >= 10)
		{
			return ((char *)NULL);
		}
		*buffer++ = *c++;
	}
	if (*c == 13)	/* ���s�R�[�h�� OD OA */
	{
		*end_arg = 1;
	}
	*buffer = 0;		//NULL
	return (c);
}


/*[is_digit_or_hifun();'����'��������'-'�L���̏ꍇ�B�ł��邩�𒲂ׂ�B]*/

/*---------------------------------------------------------
	�����ɓn�閽�߃p�����[�^�[(�I�y�����h)����������ǂށB
---------------------------------------------------------*/
extern int is_digit_or_hifun(char *ccc);/*(load_stage.c)*/
static char *read_operand_int_only(char *ccc, int *number, int *line_terminate_end_flag)
{
	char buffer[32];/*20*/
	char *ddd = buffer;
	int i = 0;
	while ( is_digit_or_hifun(ccc) )/*(�����ɑΉ�����)*/
	{
		i++;
		if (i >= 32)/*20*/
		{	goto ne222;}
		*ddd++ = *ccc++;
	}
/* ',' �܂��� '\n' ��������I�� */

//
	if (13==(*ccc)) /* ���s�R�[�h�� OD OA */
	{
		*line_terminate_end_flag = 1;
	}
	*ddd = 0;
	if (((','==(*ccc)) || (13==(*ccc))) && (','==(*(ccc-1))))	{	*number = (-1); }
	else														{	*number = atoi(buffer); }
	return (ccc);
/*error*/
ne222:
	return ((char *)NULL);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

enum
{
// OBJ�֘A
	I_CODE_00_OBJ_LOOK = 0,
	I_CODE_01_OBJ_XY,
	I_CODE_02_OBJ_LOAD,
	I_CODE_03_OBJ_WAIT,
// ��ʐ���
	I_CODE_04_DRAW_TEXT,
	I_CODE_05_DRAW_BG,
	I_CODE_06_DRAW_PANEL,
	I_CODE_07_DRAW_SKIP,
// �e�L�X�g�֘A
	I_CODE_08_TEXT,
	I_CODE_09_HOLD,
	I_CODE_0a_FONT_COLOR,
	I_CODE_0b_FONT_SPEED,
// �w�i/BGM
	I_CODE_0c_BG_LOAD,
	I_CODE_0d_BG_FADE,
	I_CODE_0e_BGM,
	I_CODE_0f_BGM_VOLUME,
// �{�X����
	I_CODE_10_BOSS_LOAD,
	I_CODE_11_BOSS_TERMINATE,/*(r34�V��)*/
	I_CODE_12_BOSS_START,
	I_CODE_13_BOSS_RESULT,/*(r35�V��)*/
//
	I_CODE_14_DUMMY,	// �K�v�Ȃ������S�̈�
	I_CODE_15_DUMMY,	// �K�v�Ȃ������S�̈�
	I_CODE_16_DUMMY,	// �K�v�Ȃ������S�̈�
	I_CODE_17_DUMMY,	// �K�v�Ȃ������S�̈�
//
	I_CODE_18_DUMMY,	// �K�v�Ȃ������S�̈�
	I_CODE_19_DUMMY,	// �K�v�Ȃ������S�̈�
	I_CODE_1a_DUMMY,	// �K�v�Ȃ������S�̈�
	I_CODE_1b_DUMMY,	// �K�v�Ȃ������S�̈�
//
	I_CODE_1c_DUMMY,	// �K�v�Ȃ������S�̈�
	I_CODE_1d_DUMMY,	// �K�v�Ȃ������S�̈�
	I_CODE_1e_DUMMY,	// �K�v�Ȃ������S�̈�
	I_CODE_1f_DUMMY,	// �K�v�Ȃ������S�̈�
//
	I_CODE_99_MAX/*(2^n)*/	// �K�v�Ȃ������S�̈�
};






typedef struct _kaiwa_youso_tag_
{
	struct _kaiwa_youso_tag_ *next;
	int i_code; 		/* Interprited Code. / Intermediate Language, Command ���Ԍ��� / �I��������ǂ��� */
	int para1;
//[16==4*4]
	int para2;
	int para3;
//[32==8*4]
	int str_num0;// char para0[(7*32)/*200*/];/* 32�̔{�� */
} KAIWA_YOUSO;/* == 256bytes */
/*
	64�����Ȃ����ǁA�I�������Ă�B
	�얲A end(s61.txt) �� 51 ��������Ȃ��B
	�����A�͕핗�̕W���V�i���I�Ȃ�΁A 64 �ŏ\�����Ǝv����B
	���A���S�ׁ̈A�{�� 128 �ɂ��Ƃ��B
	����I�[�o�[���Ă��A�G���["string stack over flow.(%d)\n"��
	�I�����邩���薳���B(���b�Z�[�W���猴�������邵)
*/
//#define MAX_STR_STACK 	(64)
#define MAX_STR_STACK		(128)
static int str_stack_position;
static char str_stack[MAX_STR_STACK][(7*32)/*200*/];/* 32�̔{�� */

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
					if (0==tiny_strcmp(command, "BG_LOAD")) 	{	new_kaiwa_youso->i_code = I_CODE_0c_BG_LOAD; strcpy( (&str_stack[str_stack_position][0]), c_p0);str_stack_position++;	/*push_str(c_p0);*/ 	}	/* �w�i�t�@�C���̕ύX */
			else	if (0==tiny_strcmp(command, "BG_FADE")) 	{	new_kaiwa_youso->i_code = I_CODE_0d_BG_FADE;		}	/* �w�i�̃t�F�[�h�A�E�g */
			else	if (0==tiny_strcmp(command, "BGM")) 		{	new_kaiwa_youso->i_code = I_CODE_0e_BGM;			}	/* BGM�ύX */
			else	if (0==tiny_strcmp(command, "BGM_VOLUME"))	{	new_kaiwa_youso->i_code = I_CODE_0f_BGM_VOLUME; 	}	/* BGM ���� �ύX */
		if (0==tiny_strcmp(command, 			"BOSS"))
		{
					if (0==tiny_strcmp(c_p0,	"load"))		{	new_kaiwa_youso->i_code = I_CODE_10_BOSS_LOAD;		}	/* �{�X�ǂݍ��� */
			else	if (0==tiny_strcmp(c_p0,	"term"))		{	new_kaiwa_youso->i_code = I_CODE_11_BOSS_TERMINATE; }	/* result��ɃQ�[�������I���B */
			#if (1)
			else	if (0==tiny_strcmp(c_p0,	"start"))		{	new_kaiwa_youso->i_code = I_CODE_12_BOSS_START; 	}	/* �{�X�U���J�n�B */
			else	if (0==tiny_strcmp(c_p0,	"result"))		{	new_kaiwa_youso->i_code = I_CODE_13_BOSS_RESULT;	}	/* (r35)���ݎd�l�̓s����v��B���ꂪ�����ƃ��U���g��ʂ��I���o�Ȃ��B */
			#endif
		}
	}
	else
	if ('D'==(*command))
	{
		if (0==tiny_strcmp(command, 			"DRAW"))
		{
					if (0==tiny_strcmp(c_p0,	"panel"))		{	new_kaiwa_youso->i_code = I_CODE_06_DRAW_PANEL; 	}	/* �X�R�A�p�l���̕\��/��\�� */
			else	if (0==tiny_strcmp(c_p0,	"text"))		{	new_kaiwa_youso->i_code = I_CODE_04_DRAW_TEXT;		}	/* �e�L�X�g��ʂ̕\��/��\�� */
			else	if (0==tiny_strcmp(c_p0,	"bg"))			{	new_kaiwa_youso->i_code = I_CODE_05_DRAW_BG;		}	/* �w�i�̕\��/��\�� */
			else	if (0==tiny_strcmp(c_p0,	"skip"))		{	new_kaiwa_youso->i_code = I_CODE_07_DRAW_SKIP;		}	/* */
			else	if (0==tiny_strcmp(c_p0,	"color"))		{	new_kaiwa_youso->i_code = I_CODE_0a_FONT_COLOR; 	}	/* �����̐F */
			else	if (0==tiny_strcmp(c_p0,	"speed"))		{	new_kaiwa_youso->i_code = I_CODE_0b_FONT_SPEED; 	}	/* �����̕\�����x */
		}
	}
	else
	if ('O'==(*command))
	{
					if (0==tiny_strcmp(command, "OBJ_LOAD"))	{	new_kaiwa_youso->i_code = I_CODE_02_OBJ_LOAD;		strcpy( (&str_stack[str_stack_position][0]), c_p0);str_stack_position++;			}	/* �ėp�I�u�W�F�̃��[�h / �ēǂݍ��� / �ėp�I�u�W�F�ɉ摜��ǂ݂��ށB */
		if (0==tiny_strcmp(command, 			"OBJ"))
		{
					if (0==tiny_strcmp(c_p0,	"look"))		{	new_kaiwa_youso->i_code = I_CODE_00_OBJ_LOOK;		}	/* �I�u�W�F�N�g���ڃR�}���h */
			else	if (0==tiny_strcmp(c_p0,	"xy"))			{	new_kaiwa_youso->i_code = I_CODE_01_OBJ_XY; 		}	/* �I�u�W�F�N�g�̌��_�ݒ� */
			else	if (0==tiny_strcmp(c_p0,	"wait"))		{	new_kaiwa_youso->i_code = I_CODE_03_OBJ_WAIT;		}	/* �I�u�W�F�N�g�ړ������܂ő҂B */
//			else	if (0==tiny_strcmp(c_p0,	"draw"))		{	new_kaiwa_youso->i_code = I_CODE_03_OBJ_DRAW;		}	/* �I�u�W�F�N�g�̕\��/��\�� */
//			else	if (0==tiny_strcmp(c_p0,	"speed"))		{	new_kaiwa_youso->i_code = I_CODE_OBJ_SPEED; 		}	/* �I�u�W�F�N�g�̑��x�ݒ� */
		}
	}
	else
//	if ('T'==(*command))
//	{
					if (0==tiny_strcmp(command, "TEXT"))		{	new_kaiwa_youso->i_code = I_CODE_08_TEXT;			strcpy( (&str_stack[str_stack_position][0]), c_p0);str_stack_position++;			}	/* ���b�Z�[�W�E�B���h�E�ւ̏������� */
//	}
	else
//	if ('H'==(*command))
//	{
					if (0==tiny_strcmp(command, "HOLD"))		{	new_kaiwa_youso->i_code = I_CODE_09_HOLD;				}	/* �N���b�N�҂� */
//	}

//	if ('C'==(*command))
//	{	if (0==tiny_strcmp(command, 			"CURSOR"))
//		{
//					if (0==tiny_strcmp(c_p0,	"home"))		{	new_kaiwa_youso->i_code = I_CODE_CUR_HOME;			}	/* �J�[�\���̏����� */
//			else	if (0==tiny_strcmp(c_p0,	"load"))		{	new_kaiwa_youso->i_code = I_CODE_CUR_POP;			}	/* �L�������J�[�\���ʒu�̌Ăяo�� */
//			else	if (0==tiny_strcmp(c_p0,	"save"))		{	new_kaiwa_youso->i_code = I_CODE_CUR_PUSH;			}	/* �J�[�\���ʒu�̋L�� */
//			else	if (0==tiny_strcmp(c_p0,	"click"))		{	new_kaiwa_youso->i_code = I_CODE_09_HOLD;				}	/* �N���b�N�҂� */
//			else	if (0==tiny_strcmp(c_p0,	"text"))		{	new_kaiwa_youso->i_code = I_CODE_08_TEXT_WRITE_TEXT; }	/* ���b�Z�[�W�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
//			else	if (0==tiny_strcmp(c_p0,	"bg"))			{	new_kaiwa_youso->i_code = I_CODE_BG_WRITE_TEXT; 	}	/* BG�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
//		}
//	}

	if ( (MAX_STR_STACK-1) < str_stack_position)
	{	/* ������X�^�b�N�A�I�[�o�[�t���[�ŋ����I���B */
		error(ERR_FATAL, (char*)"string stack over flow.(%d)\n", str_stack_position);
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
			ch = load_my_file_get_str(ch, c_p0, &end_arg/*, ','*/); 				/* ������R�}���h(�I�y�����h)�ǂݍ��� */
			if (NULL==ch)									{	GOTO_ERR_WARN;	}	/* �ǂ߂Ȃ���΃G���[ */
		}
		{
			int c_pn[PARAM_99_MAX];/*6*/
			{	int kk;
				for (kk=0; kk<PARAM_99_MAX; kk++)/*(6)*/
				{
					c_pn[kk] = (-1);	/* ���w��̏ꍇ �𔻕ʂ���� */
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
		error(ERR_WARN, (char*)"syntax error in kaiwa file '%s', line no: %d", my_file_common_name, debug_number_of_read_line);
		MY_FILE_LOOP_CONTINUE;
	#endif
		MY_FILE_LOOP_END;
	}
	//
	my_file_fclose(/*fp*/);
	//return (opecode_entrys);
	if (0 == opecode_entrys)
	{
		error(ERR_WARN, (char*)"can't entrys from kaiwa file %s", my_file_common_name);
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
static int is_bg;	/*=0*/					/* �w�i�\��/��\���t���O */

//static int cursor_x_chached;			/* �J�[�\���ʒu �ۑ��p */
//static int cursor_y_chached;			/* �J�[�\���ʒu �ۑ��p */

/*---------------------------------------------------------
	�V�i���I��b�V�X�e�����̔w�i
---------------------------------------------------------*/

#if (1)
static u16 *bg_story_window_surface_image;
static int bg_story_window_surface_pitch;
static int bg_story_window_surface_width;
#endif

static SDL_Surface *bg_story_window_surface;			/* �V�i���I��b�V�X�e�����̔w�i */

/*---------------------------------------------------------

---------------------------------------------------------*/

//static extern int my_string_offset;
/*static*/ extern void clear_my_string_offset(void);
static void aaa_kaiwa_system_reset(void)
{
	unsigned int i;
	for (i=0; i<KAIWA_SPRITE_99_MAX; i++)/*20*/
	{
		remove_kaiwa_sprite_num(i);
	}
//	remove_kaiwa_sprite_num(KAIWA_SPRITE_00_TACHIE_RIGHT);
//	remove_kaiwa_sprite_num(KAIWA_SPRITE_01_TACHIE_LEFT_);
	if (NULL != bg_story_window_surface)
	{
		SDL_FreeSurface(bg_story_window_surface);
		bg_story_window_surface 	= NULL;
	}
	is_bg = (0);/* �J�������񂾂��� bg �\���ł��Ȃ��A���������ċ����\�� off */
	//
	kanji_cursor_move_home_position();		/* �����J�[�\�����z�[���|�W�V�����ֈړ� */

	clear_my_string_offset();	//	my_string_offset=0;
}

/*---------------------------------------------------------
	is_bg �� 1 �ɂȂ�\���̂���ꏊ�́Abg_story_window_surface ��������
	�`��Ńn���O�A�b�v���邩��A�`�F�b�N����B
	bg_story_window_surface �������ꍇ�͋���off�B
---------------------------------------------------------*/

static void check_is_bg(void)
{
	if (NULL == bg_story_window_surface)
	{
		is_bg = (0);/* �J�������񂾂��� bg �\���ł��Ȃ��A���������ċ����\�� off */
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static int bg_alpha;		/* �w�i���l�p */

/////////////////////////////

static void load_bg_aaa(char *filename, int alpha)
{
	bg_story_window_surface = load_local01(filename, bg_story_window_surface);//, alpha);
	{
		bg_story_window_surface_image	= (u16 *)bg_story_window_surface->pixels;
		bg_story_window_surface_pitch	= bg_story_window_surface->pitch;
		bg_story_window_surface_width	= bg_story_window_surface->w;
	}
	#if 0/* ����A�v��Ȃ��H�B */
	SDL_SetColorKey(bg_story_window_surface, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
	#endif
}


/*---------------------------------------------------------
	�V�i���I��b�V�X�e���A��������
---------------------------------------------------------*/
static int re_draw_count_flag;
global void kaiwa_system_set_re_draw(void)
{
	re_draw_count_flag = (1);/* �`�悵�Ă� */
}

/* �����R�}���h�̎󂯓n���`���B */
#define MY_ICODE_FUNC(name) 		static void name(KAIWA_YOUSO *ssc)
#define MY_ICODE_GET_FUNC(name) 	static int name(void)

/*---------------------------------------------------------
	[ "i_code_" �V�i���I��b�V�X�e�������R�}���h]
---------------------------------------------------------*/
extern void load_SDL_bg_file_name(char *file_name);

//static int i_code_fade_bg_exec_fade;
//MY_ICODE_GET_FUNC(i_code_get_bg_fade)
//{
//	return (i_code_fade_bg_exec_fade);
//}

MY_ICODE_FUNC(i_code_load_bg)
{
//	pop_str();
	/* �V�i���I��b�V�X�e�������R�}���h�̃p�����[�^�[���󂯎��B */
//	char *filename; 	//	filename		= ssc->para0;
//	strcpy(filename, (&str_stack[str_stack_position][0]));str_stack_position++;
	int alpha;				alpha			= ssc->para1;
//	int fade_command;		fade_command	= ssc->para2;
//	int set_alpha_speed;//	set_alpha_speed = ssc->para3;

	re_draw_count_flag = (1);/* �`�悵�Ă� */

	load_bg_aaa(/*filename*/(&str_stack[str_stack_position][0]), alpha);str_stack_position++;/*pop_string();(������X�^�b�N����������)*/
}
//	if (filename)
//	{
//		load_bg_aaa(filename, 0/*alpha*/);
//	}

//	/*fade_in_command_start*/

	static int i_code_fade_bg_bg_alpha_speed;		/* �w�i���l�p */
	static int i_code_fade_bg_exec_fade;		/* do fade */
MY_ICODE_FUNC(i_code_fade_bg)
{
	/* �V�i���I��b�V�X�e�������R�}���h�̃p�����[�^�[���󂯎��B */
//	char *filename; 		filename		= ssc->para0;
//	int alpha;				alpha			= ssc->para1;
//	int fade_command;	//	fade_command	= ssc->para2;
//	if ((1)==fade_command)/*1==fade_in_command*/

	re_draw_count_flag = (1);/* �`�悵�Ă� */
//
	//
	bg_alpha = 0;
	int i_code_fade_bg_set_alpha_speed;//	set_alpha_speed = ssc->para3;
//	if (-1 == set_alpha_speed)/*�p�����[�^�[�ȗ���*/
	{
		i_code_fade_bg_set_alpha_speed = 5;
	}
	i_code_fade_bg_bg_alpha_speed = i_code_fade_bg_set_alpha_speed;
	is_bg					= 1;	check_is_bg();
	i_code_fade_bg_exec_fade = (1);/*on*/	/* ������ */

//	return /*(0)*/; 	/* ������ */
}
static void kaiwa_system_vbl_do_BG_fade(void)
{
		if (0!=i_code_fade_bg_exec_fade)
		{
			re_draw_count_flag = (1);/* �`�悵�Ă� */
//			psp_clear_screen();
//			SD L_SetAlpha(bg_story_window_surface, SD L_SRCALPHA, bg_alpha);
//			SD L_BlitSurface(bg_story_window_surface, NULL, sdl_screen[SD L_00_SCR EEN], NULL);
			bg_alpha += i_code_fade_bg_bg_alpha_speed/*5*/;
			if (250 < bg_alpha)
			{
				bg_alpha = 255;
				#if 0/*???*/
				/*	�����ǂ��킩��Ȃ����ǁA�����͂�������???�B */
				if (NULL != bg_story_window_surface)
				{
					unloadbmp_by_surface(bg_story_window_surface);	// �L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bbg_story_window_surface = NULL;
				}
				#endif
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
	re_draw_count_flag = (1);/* �`�悵�Ă� */
//
	unsigned int nnn;
	for (nnn=0; nnn<KAIWA_SPRITE_99_MAX; nnn++)
	{
		int hide_mode;	/*(�B��郂�[�h)*/
		int offset_x;
		int offset_y;
		KAIWA_SPRITE *my_std_obj;
		my_std_obj = &kaiwa_sprite[((nnn)&(KAIWA_SPRITE_99_MAX-1))]; /* �ėp�I�u�W�F */
		//
		my_std_obj->move_flag = KAIWA_SPRITE_MOVE_FLAG_01_MOVE_START;

		if (KAIWA_SPRITE_99_MAX>(obj_look_up))/* �ʏ탂�[�h */
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
		//	my_std_obj->alpha255 = (255);	/* �F */
			SDL_SetAlpha(my_std_obj->img, SDL_SRCALPHA, (255) );
		}
		else/* ���ڃI�u�W�F�ȊO�Ȃ�I�t�Z�b�g���� */
		{	/*(�B��郂�[�h ON)*/
			offset_x	= my_std_obj->offset_x256;
			offset_y	= my_std_obj->offset_y256;
		//	my_std_obj->alpha255 = (180);	/* �F */
			SDL_SetAlpha(my_std_obj->img, SDL_SRCALPHA, (180) );
		}

		/* ���炩���߈ړ������ʒu���Z�o���Ă��� */
	//	my_std_obj->target_x256 = my_std_obj->origin_x256 + offset_x + ((-((my_std_obj->w)>>1))<<8);	/* �ړ��������W */
	//	my_std_obj->target_y256 = my_std_obj->origin_y256 + offset_y + ((-((my_std_obj->h)>>1))<<8);	/* �ړ��������W */
		my_std_obj->target_x256 = my_std_obj->origin_x256 + offset_x + ((-((1<<(my_std_obj->w_bit))>>1))<<8);	/* �ړ��������W */
		my_std_obj->target_y256 = my_std_obj->origin_y256 + offset_y + ((-((1<<(my_std_obj->h_bit))>>1))<<8);	/* �ړ��������W */

		/* �ړ����������߂� */
	//	my_std_obj->angle1024	= at an_1024(y,x);
	//	my_std_obj->angle1024	= at an_65536(y,x);
		/*(�ړ������\��ʒu���猻�݈ʒu�������΁A�������o��)*/
		{//1579619 1579683 1579715
			REG_00_SRC_X	= (my_std_obj->target_x256);
			REG_01_SRC_Y	= (my_std_obj->target_y256);
			REG_02_DEST_X	= (my_std_obj->cx256);
			REG_03_DEST_Y	= (my_std_obj->cy256);
			tmp_angleCCW65536_src_nerai();
			my_std_obj->angle1024	= HATSUDAN_03_angle65536;
		}
		my_std_obj->angle1024	>>= (6);	/* 1���̕������� 65536 ���� 1024 �֕ϊ� */
		//
	//	my_std_obj->speed256 = ((40)<<(8-3));	/* �����x (x32) */
	//	my_std_obj->speed256 = ((20)<<(8-3));	/* �����x (x32) */
		my_std_obj->speed256 = (0); 			/* �����x (x32) */
	}
}

/*---------------------------------------------------------
	[ "i_code_" �V�i���I��b�V�X�e�������R�}���h]
	I_CODE_01_OBJ_XY �I�u�W�F�N�g�̌��_���W�̐ݒ�B
---------------------------------------------------------*/

MY_ICODE_FUNC(i_code_obj_xy)
{
	/* �V�i���I��b�V�X�e�������R�}���h�̃p�����[�^�[���󂯎��B */
//	char *color32_str;		color32_str = ssc->para0;
	int num;				num 		= ssc->para1;
	int origin_x;			origin_x	= ssc->para2;
	int origin_y;			origin_y	= ssc->para3;
//
	KAIWA_SPRITE *my_std_obj;
	my_std_obj = &kaiwa_sprite[((num)&(KAIWA_SPRITE_99_MAX-1))]; /* �ėp�I�u�W�F */
	//
	my_std_obj->origin_x256 = ((origin_x)<<8);		/* ���_���W�̐ݒ� */
	my_std_obj->origin_y256 = ((origin_y)<<8);		/* ���_���W�̐ݒ� */
	obj_move_refresh();
}

/*---------------------------------------------------------
	[ "i_code_" �V�i���I��b�V�X�e�������R�}���h]
	I_CODE_00_OBJ_LOOK ���ڃI�u�W�F�̔ԍ���ݒ�B
---------------------------------------------------------*/

MY_ICODE_FUNC(i_code_obj_look)
{
	/* �V�i���I��b�V�X�e�������R�}���h�̃p�����[�^�[���󂯎��B */
//	char *color32_str;		color32_str = ssc->para0;
/*	int mode;	*/			obj_look_up 		= ssc->para1;
	obj_move_refresh();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------
	[ "i_code_" �V�i���I��b�V�X�e�������R�}���h]
	�I�u�W�F�N�g�ɉ摜��ǂݍ���
	i_code_obj_load_gazo
	[ "i_code_" �V�i���I��b�V�X�e�������R�}���h]
	�ǂݍ��ݍς݂̃I�u�W�F�N�g�̉摜���ēǂݍ��݂��āA
	�ʒu�����̂܂܌p�����摜�͓���ւ���B
	i_code_swap_image_sprite
---------------------------------------------------------*/

//static /*KAIWA_SPRITE **/void
MY_ICODE_FUNC(i_code_obj_load_gazo)
{
	/* �V�i���I��b�V�X�e�������R�}���h�̃p�����[�^�[���󂯎��B */
//	char *filename; //	filename		= ssc->para0;
//	strcpy(filename, (&str_stack[str_stack_position][0]));str_stack_position++;
	int sitei_number;	sitei_number	= ssc->para1;
//	int xx; 			xx				= ssc->para2;
//	int yy; 			yy				= ssc->para3;
//
	re_draw_count_flag = (1);/* �`�悵�Ă� */
	sitei_number &= ((KAIWA_SPRITE_99_MAX-1));
//	build_sprite(sitei_number);
//	static void build_sprite(int sitei_number)
	remove_kaiwa_sprite_num(sitei_number);	/* �g�p���Ȃ� */	/* �摜���J������B */	/* �����I�ɊJ�� */
	SDL_Surface *suf	= NULL;
//	suf 				= load_local02(filename, 0/*alpha*/);
	suf 				= load_local01(/*filename*/(&str_stack[str_stack_position][0]), suf);str_stack_position++;/*pop_string();(������X�^�b�N����������)*/	//, 0/*alpha*/);
	if (NULL==suf)		{	return; 	}/*(NULL)*/

	KAIWA_SPRITE *my_std_obj;
	my_std_obj = &kaiwa_sprite[sitei_number]; /* �ėp�I�u�W�F */

	my_std_obj->img 	= suf;	/* �g�p���ɂȂ�B */
//	my_std_obj->frames = frames;
	/*(�c�����A�����I��2^n�ɕϊ�)*/
	my_std_obj->w_bit		= (32 - __builtin_allegrex_clz((suf->w)-1));/*/frames*/ 	//tmp->cw		= ((tmp->w)>>1);
	my_std_obj->h_bit		= (32 - __builtin_allegrex_clz((suf->h)-1));				//tmp->ch		= ((tmp->h)>>1);

}

/*---------------------------------------------------------
	���s��~�����p��
---------------------------------------------------------*/
enum /* �}���`�^�X�N�@�\�̃^�X�N�X�C�b�` */
{
	TASK_SWITCH_00_EXECUTE							= 0x00,/*(0<<0)*/
	TASK_SWITCH_01_HOLD_REASON_OBJECT_WAIT			= 0x01,/*(1<<0)*/
	TASK_SWITCH_02_HOLD_REASON_KEY_HOLD_MODE		= 0x02,/*(1<<1)*/
	TASK_SWITCH_04_HOLD_REASON_TEXT_DRAWING 		= 0x04,/*(1<<2)*/
	TASK_SWITCH_08_HOLD_REASON_KEY_CHECK_ON_BOOT	= 0x08,/*(1<<3)*/
};
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
	for (nnn=0; nnn<KAIWA_SPRITE_99_MAX; nnn++)
	{
		KAIWA_SPRITE *my_std_obj;
		my_std_obj = &kaiwa_sprite[nnn]; /* �ėp�I�u�W�F */
		if (KAIWA_SPRITE_MOVE_FLAG_00_MOVE_COMPLETE != my_std_obj->move_flag )
		{
			check_flag = (1);/* �ړ��� */
		}
	}
	/* �SOBJ����~���Ă���ꍇ�̂݁A�����B */
	if (0==check_flag)/* �SOBJ��~���Ă���B */
	{
		kaiwa_system_hold_mode &= ~TASK_SWITCH_01_HOLD_REASON_OBJECT_WAIT;/* ���� */
	}
}


/*---------------------------------------------------------
	��b�ʏ펞�̃p�b�h�`�F�b�N
	-------------------------------------------------------
	�������Ԃ́A�{�ƂƓ���10[�b]�Ƃ���B
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
			kaiwa_system_hold_mode &= ~TASK_SWITCH_08_HOLD_REASON_KEY_CHECK_ON_BOOT;/* ���� */
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
		kaiwa_system_hold_mode &= ~TASK_SWITCH_04_HOLD_REASON_TEXT_DRAWING;/* ���� */
	}
}



/*---------------------------------------------------------
	�V�i���I��b�V�X�e���A�I�u�W�F�N�g�̈ړ�
---------------------------------------------------------*/

static void kaiwa_system_vbl_sprite_move(void)
{
	int nnn;
	for (nnn=0; nnn<KAIWA_SPRITE_99_MAX; nnn++)
	{
		KAIWA_SPRITE *my_std_obj;
		my_std_obj = &kaiwa_sprite[nnn]; /* �ėp�I�u�W�F */
		if (KAIWA_SPRITE_MOVE_FLAG_00_MOVE_COMPLETE != my_std_obj->move_flag )
		{
			re_draw_count_flag = (1);/* �`�悵�Ă� */
			{
				{
				//	my_std_obj->speed256 += (5*32); 		/* 160==5x32(������) �����x(�����̗����G�͏�ɉ������Ĉړ�����) (x32) */
				//	my_std_obj->speed256 += (4*32); 		/* 128==4x32() �����x(�����̗����G�͏�ɉ������Ĉړ�����) (x32) */
					my_std_obj->speed256 += (3*32); 		/*	96==3x32() �����x(�����̗����G�͏�ɉ������Ĉړ�����) (x32) */
				//	if (t256(6.0)<my_std_obj->speed256) 	{	my_std_obj->speed256 = t256(8.0);	}	/* �ő呬�x8[dots/flame] t256(6.0) */
					my_std_obj->speed256 = psp_min(my_std_obj->speed256, (t256(8.0) ));
				//	my_std_obj->speed256 = t256(0.1);/*(�ł΂���)*/
					#if (0)//
					my_std_obj->cx256 += ((si n1024((my_std_obj->angle1024))*(my_std_obj->speed256))>>8);/*fps_factor*/ 	/* CCW�̏ꍇ */
					my_std_obj->cy256 += ((co s1024((my_std_obj->angle1024))*(my_std_obj->speed256))>>8);/*fps_factor*/
					#else
					{
						int sin_value_t256; 	//	sin_value_t256 = 0;
						int cos_value_t256; 	//	cos_value_t256 = 0;
						int256_sincos1024( (my_std_obj->angle1024), &sin_value_t256, &cos_value_t256);
						my_std_obj->cx256 += ((sin_value_t256*(my_std_obj->speed256))>>8);/*fps_factor*/
						my_std_obj->cy256 += ((cos_value_t256*(my_std_obj->speed256))>>8);/*fps_factor*/
					}
					#endif
				}
				void *aaa_sss[(4)] =
				{
					&&aaa_sss_0,
					&&aaa_sss_1,
					&&aaa_sss_2,
					&&aaa_sss_3,
				};
				/* ��̂̕���: �Ȃ̂� �S�T�x(1����8����)��]���Ă��炵�āA��ʂQ�r�b�g����܂��ȕ��� */
				goto *aaa_sss[(((my_std_obj->angle1024+1024-(1024/8))>>8) & 0x03)];/*deg_360_to_1024(45)*/
			//	goto *aaa_sss[(((my_std_obj->angle1024+(1024/8))>>8) & 0x03)];/*deg_360_to_1024(45)*/
				/* �ړ��������W�ɓ��������͂ݏo������A�����Ƃ���B */
				aaa_sss_0:/* 1:��(1024_0) */	if (my_std_obj->cx256 < my_std_obj->target_x256)	{	goto move_continue /*(0)*/;/*�ړ���*/	}	goto move_complete;
				aaa_sss_1:/* 2:��(1024_1) */	if (my_std_obj->cy256 > my_std_obj->target_y256)	{	goto move_continue /*(0)*/;/*�ړ���*/	}	goto move_complete;
				aaa_sss_2:/* 3:��(1024_2) */	if (my_std_obj->cx256 > my_std_obj->target_x256)	{	goto move_continue /*(0)*/;/*�ړ���*/	}	goto move_complete;
				aaa_sss_3:/* 0:��(1024_3) */	if (my_std_obj->cy256 < my_std_obj->target_y256)	{	goto move_continue /*(0)*/;/*�ړ���*/	}	goto move_complete;
			move_complete:
				/* �ړ����������ꍇ�́A���m�ȍ��W�ɏC������B */
				my_std_obj->cx256		= my_std_obj->target_x256;
				my_std_obj->cy256		= my_std_obj->target_y256;
				my_std_obj->move_flag	= KAIWA_SPRITE_MOVE_FLAG_00_MOVE_COMPLETE;/*�ړ�����*/
				//return /*(1)*/;/*�ړ�����*/
			move_continue:
				;
			}
		}
	}
}
/*
��̂̕����̋��ߕ��F
�p�x�͉���0�x�ŁA�����v���B(��CCW)
�p�x�� 1024 ����(0-1023)�Ȃ̂� 256�Ŋ���� 4�ɂȂ�B(0, 1, 2, 3)�̂ǂꂩ�ɂȂ�B
�܂艺�L�}�ŁA���ꂼ�� (0)�E��, (1)�E��, (2)����, (3)���� �̗̈�B
+-------------+-------------+
| +   (2)	  | 	(1)   + |
|	+ [1]	 [1]	[1] +	|
|(2)  + 	  | 	  +  (1)|
|[2]	+	  | 	+	 [0]|
|		  +   |   + 		|
|			+ | +			|
+-[2]---------+---------[0]-+
|			+ | +			|
|[2]	  +   |   + 	 [0]|
|(3)	+	  | 	+	 (0)|
|	  + 	 [3]	  + 	|
|	+ [3]	  | 	[3] +	|
| +   (3)	  | 	(0)   + |
+-------------+-------------+
���̂܂܂ł͎g���ɂ����B�}�Ō����΁A���ꂼ�� [0], [1], [2], [3] �̗̈悪�~�����B
�����ŗ\��45�x�������Ă���A256�Ŋ���Ƃ� [0]��, [1]��, [2]��, [3]�� �̗̈悪�o��B
*/


/*---------------------------------------------------------
	�V�i���I��b�V�X�e���A�������s����
	�V�i���I��b�V�X�e�� �R�}���h����(���s)�n
---------------------------------------------------------*/

static /*int*/void kaiwa_system_execute(void)
{
	KAIWA_YOUSO *ssc;
	ssc = kaiwa_youso_list_scan;/* ���s�J�n�ʒu */
//
	void *aaa[I_CODE_99_MAX] =
	{
	// OBJ�֘A
		&&case_I_CODE_00_OBJ_LOOK,
		&&case_I_CODE_01_OBJ_XY,
		&&case_I_CODE_02_OBJ_LOAD,
		&&case_I_CODE_03_OBJ_WAIT,
	// ��ʐ���
		&&case_I_CODE_04_DRAW_TEXT,
		&&case_I_CODE_05_DRAW_BG,
		&&case_I_CODE_06_DRAW_PANEL,
		&&case_I_CODE_07_DRAW_SKIP,
	// �e�L�X�g�֘A
		&&case_I_CODE_08_TEXT,
		&&case_I_CODE_09_HOLD,
		&&case_I_CODE_0a_FONT_COLOR,
		&&case_I_CODE_0b_FONT_SPEED,
	// �w�i/BGM
		&&case_I_CODE_0c_BG_LOAD,
		&&case_I_CODE_0d_BG_FADE,
		&&case_I_CODE_0e_BGM,
		&&case_I_CODE_0f_BGM_VOLUME,
	// �{�X����
		&&case_I_CODE_10_BOSS_LOAD,
		&&case_I_CODE_11_BOSS_TERMINATE,/*(r34�V��)*/
		&&I_CODE_break, /*(�K�v�_�~�[)*/	// &&case_I_CODE_12_BOSS_START,
		&&I_CODE_break, /*(�K�v�_�~�[)*/	// &&case_I_CODE_13_BOSS_RESULT,/*(r35�V��)*/
	//
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
	//
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
	//
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
		&&I_CODE_break, 	// �K�v�Ȃ������S�̈�
	//
	//	I_CODE_99_MAX/*(2^n)*/	// �K�v�Ȃ������S�̈�
	};
	goto *aaa[(ssc->i_code)&0x1f];/*(ccc)&(I_CODE_99_MAX-1)*/
	{
	case_I_CODE_00_OBJ_LOOK:		i_code_obj_look(ssc);											goto I_CODE_break;	/* �I�u�W�F�N�g���ڃR�}���h */
	case_I_CODE_01_OBJ_XY:			i_code_obj_xy(ssc); 											goto I_CODE_break;	/*(���_�ݒ�̂�)*/
	case_I_CODE_02_OBJ_LOAD:		i_code_obj_load_gazo(ssc);										goto I_CODE_break;	/*(�ǂ݂��݂̂�) �ėp�G  �����GL	�����GR */
	case_I_CODE_03_OBJ_WAIT:		kaiwa_system_hold_mode |= TASK_SWITCH_01_HOLD_REASON_OBJECT_WAIT; /* ��~ */	goto I_CODE_break;
	//
	case_I_CODE_04_DRAW_TEXT:	//�p�~	cg.dr aw_flag_kaiwa_screen	= ssc->para1;
		#if (0)/*(�ŋ߂�(gcc4.3.5)�͂��Ԃ񓯂��Bgcc4.0.2���炢�Â��ƈႤ�B)*/
		if (ssc->para1&1)	{	ml_font[(0)].haikei 		= (ML_HAIKEI_03_MESSAGE);/* [��/��b�p�w�i]����Ӕw�ion */}
		else				{	ml_font[(0)].haikei 		= (ML_HAIKEI_m1_OFF);/* ����Ӕw�ioff */}
		#else
		{	ml_font[(0)].haikei 		= (ssc->para1&1)?(ML_HAIKEI_03_MESSAGE)/* [��/��b�p�w�i]����Ӕw�ion */:(ML_HAIKEI_m1_OFF);/* ����Ӕw�ioff */}
		#endif
		goto I_CODE_break;
	case_I_CODE_05_DRAW_BG: 		is_bg						= ssc->para1;	check_is_bg();		goto I_CODE_break;
	case_I_CODE_06_DRAW_PANEL:		cg.side_panel_draw_flag 	= ssc->para1;						goto I_CODE_break;
	case_I_CODE_07_DRAW_SKIP:		kaiwa_system_skip_mode		= ssc->para1;						goto I_CODE_break;
	//
	case_I_CODE_08_TEXT:
				strcpy(i_code_text_para0, (&str_stack[str_stack_position][0]));str_stack_position++;/*pop_string();(������X�^�b�N����������)*/
			//	strcpy(i_code_text_para0, ssc->para0);
					kaiwa_system_hold_mode |= TASK_SWITCH_04_HOLD_REASON_TEXT_DRAWING;					goto I_CODE_break;
	case_I_CODE_09_HOLD:			limit_timer 				= byou60(10);/*(10[�b]�������ԁB�{�ƌ݊�)*/ 	kaiwa_system_hold_mode |= TASK_SWITCH_02_HOLD_REASON_KEY_HOLD_MODE; goto I_CODE_break;
	case_I_CODE_0a_FONT_COLOR:		i_code_text_color			= ssc->para1;						goto I_CODE_break;
	case_I_CODE_0b_FONT_SPEED:		i_code_text_speed_wait		= ssc->para1;						goto I_CODE_break;
	//
	case_I_CODE_0c_BG_LOAD: 		i_code_load_bg(ssc);											goto I_CODE_break;
	case_I_CODE_0d_BG_FADE: 		i_code_fade_bg(ssc);											goto I_CODE_break;
	case_I_CODE_0e_BGM: 			play_music_num( (ssc->para1) ); 								goto I_CODE_break;
	case_I_CODE_0f_BGM_VOLUME:		set_music_volume( (ssc->para1) );								goto I_CODE_break;
	case_I_CODE_10_BOSS_LOAD:		called_from_kaiwa_system_boss_load( (ssc->para1));				goto I_CODE_break;/*boss_number*/
	case_I_CODE_11_BOSS_TERMINATE:	cg.state_flag |= STATE_FLAG_16_GAME_TERMINATE;					goto I_CODE_break;
		/*	called_from_kaiwa_system_boss_start();*/
		/*	cg.state_flag |= STATE_FLAG_05_IS_BOSS; */
		/* �{�X�퓬�O�̉�b�I����ݒ� */
//	case_I_CODE_12_BOSS_START:		/*�_�~�[*/; 													goto I_CODE_break;
//	case_I_CODE_13_BOSS_RESULT: 	/*�_�~�[*/; 													goto I_CODE_break;
	//
//	case_I_CODE_OBJ_SPEED:			i_code_set_sprite_speed(ssc);									goto I_CODE_break;
	// [�e�L�X�g�̃J�[�\������]
//	case I_CODE_CUR_POP:			cursor_x = cursor_x_chached;	cursor_y = cursor_y_chached;		break;	/* �J�[�\���ʒu�A���� */
//	case I_CODE_CUR_PUSH:			cursor_x_chached = cursor_x;	cursor_y_chached = cursor_y;		break;	/* �J�[�\���ʒu�A�L�� */
//	case I_CODE_CUR_HOME:			kanji_cursor_move_home_position();						break;	/* �����J�[�\�����z�[���|�W�V�����ֈړ� */		/* �J�[�\���̏����� */
//	case I_CODE_BG_WRITE_TEXT:		set_write_text(1);					break;	/* BG�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
//	case I_CODE_08_TEXT_WRITE_TEXT: set_write_text(0);					break;	/* ���b�Z�[�W�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
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

#define USE_SDL_DRAW (1)
#if (1==USE_SDL_DRAW)
/*---------------------------------------------------------
	�I�u�W�F�N�g�̕`��
---------------------------------------------------------*/

static void s_draw_kaiwa_system(void)
{
	/* ----- �`��n ----- */
	/* 1. �܂��w�i��`�� */
	if (is_bg)
	{
		if (255 > bg_alpha)
		{
			SDL_SetAlpha(bg_story_window_surface, SDL_SRCALPHA, bg_alpha);
		}
		PSPL_UpperBlit(bg_story_window_surface, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], NULL);
	}
	/* 2. ���ɗ����G��`�� */
	#if (0==USE_GU_TACHIE_TEST)/* (r34)�e�X�g */
	{
		unsigned int nnn;
		for (nnn=0; nnn<KAIWA_SPRITE_99_MAX; nnn++)
		{
			if (NULL != kaiwa_sprite[nnn].img)	/* �摜��������̂̂ݕ`�悷��B(�g�p���̏ꍇ�ɕ`�悷��) */
			{
				SDL_Rect src_r;
				SDL_Rect dst_r;
				src_r.x = (0);
				src_r.y = (0);
				src_r.w = (1<<(kaiwa_sprite[nnn].w_bit));
				src_r.h = (1<<(kaiwa_sprite[nnn].h_bit));
				dst_r.x = (t256_floor(kaiwa_sprite[nnn].cx256));
				dst_r.y = (t256_floor(kaiwa_sprite[nnn].cy256));
				PSPL_UpperBlit(kaiwa_sprite[nnn].img, &src_r, cb.sdl_screen[SDL_00_VIEW_SCREEN], &dst_r);
			}
		}
	}
	#endif
}
#endif /* (1==USE_SDL_DRAW) */

/*---------------------------------------------------------
	��b�V�X�e��SDL�`��
---------------------------------------------------------*/

global void kaiwa_system_SDL_draw(void)
{
	/* SDL�`��A�x������B(Gu������Ƃ����͖����Ȃ�\��) */
	if (re_draw_count_flag)
	{
		re_draw_count_flag--;
		#if 0/* 1:�t���[���X�L�b�v�`��̃e�X�g */
		/*
		SDL�`�悪�d������AGu�����Ȃ��Ă��V�i���I�`�敔�������A�t���[���X�L�b�v(30fps�Ƃ�15fps�Ƃ�)���Ă݂��B
		���A�������ĕς��Ȃ��x���B
		*/
		static int static_game_v_time = 0;
	//	if (0==(static_game_v_time&(2-1)))/* �����I��30[flame]�`��ɂ��Ă݂�B */
		if (0==(static_game_v_time&(4-1)))/* �����I��15[flame]�`��ɂ��Ă݂�B */
		#endif/* 1:�t���[���X�L�b�v�`��̃e�X�g */
		{
			#if 1/*Gu������������v��Ȃ��Ȃ�*/
			{
				psp_clear_screen(); /* [PAUSE] ���A����SDL��ʂ������B */
			}
			#endif
			#if (1==USE_SDL_DRAW)
			s_draw_kaiwa_system();	/* ��b�V�X�e�����`�揈�� */
			#endif /* (1==USE_SDL_DRAW) */
		}
	}
}


/*---------------------------------------------------------
	��b���[�h�I������
---------------------------------------------------------*/

static void do_kaiwa_system_terminate(void)
{
//	bg_alpha = 0;
	ml_font[(0)].haikei 		= (ML_HAIKEI_m1_OFF);/* ����Ӕw�ioff */
//�p�~	cg.dr aw_flag_kaiwa_screen	= (0);	/* ����ӃE�B���h�E�\���t���O off */
	load_kaiwa_youso_free();	/* �O��̃V�i���I���������ɂ���΂��ׂĊJ���B */
	aaa_kaiwa_system_reset();
//	is_bg = 0;/* (�`��ƕ����ł��Ȃ�)�o�O�C�� */
}


/*---------------------------------------------------------
	�Q�[���R�A�p�A��b���[�h�I����Ԃ֑J��
---------------------------------------------------------*/
static void kaiwa_system_terminate_game_core(void)
{
	cg.state_flag &= (~(STATE_FLAG_06_IS_KAIWA_MODE));/*off*/
	cg.state_flag |= STATE_FLAG_12_END_KAIWA_MODE;
	/* �{�X�퓬�O��C�x���g pd_bo ssmode = B08_START; = B09_STAGE_LOAD; */
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
		if (kaiwa_system_hold_mode & TASK_SWITCH_01_HOLD_REASON_OBJECT_WAIT)
		{
			kaijyo_obj_all_stop();			/* �SOBJ����~���Ă�����A�����B */
		}
		if (kaiwa_system_hold_mode & TASK_SWITCH_02_HOLD_REASON_KEY_HOLD_MODE)
		{
			kaijyo_pad_timer_hold();		/* ���Ԍo�߂������́A�{�^����������A�����B */
		}
		if (kaiwa_system_hold_mode & TASK_SWITCH_04_HOLD_REASON_TEXT_DRAWING)
		{
			/* ==== text�`�揈��(�񓯊�) ==== */
			kaijyo_text_drawing();			/* �`��I���Ȃ�A�����B */
		}
		if (kaiwa_system_hold_mode & TASK_SWITCH_08_HOLD_REASON_KEY_CHECK_ON_BOOT)
		{
			kaijyo_pad_boot_hold(); 		/* �N������p�B�p�b�h�̃{�^���������ꂽ��A�����B */
		}
	}
	/* ==== BG�`�揈��(�񓯊�) ==== */
	kaiwa_system_vbl_do_BG_fade();
	/* ==== obj�ړ�����(�񓯊�) ==== */
	kaiwa_system_vbl_sprite_move(); /* ����̂�(�`�悵�Ȃ�) */

//	/*�ł΂���*/		re_draw_count_flag = (1);/* �`�悵�Ă� */
	if (/*(0)*/KAIWA_SYSTEM_00_CONTINUE != kaiwa_system_terminate_flag)
	{
		do_kaiwa_system_terminate();
		kaiwa_system_terminate_call_func();
	}
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
	kanji_window_all_clear();				/* ������ʂ�S�s�����B�����J�[�\�����z�[���|�W�V�����ֈړ��B */
	set_kanji_hide_line(ML_LINE_04);/*(4�s�ڈȉ����\���ɂ���B)*/
//
	kaiwa_system_skip_mode		= (0);/*off*/
	kaiwa_system_hold_mode		=
		(	TASK_SWITCH_08_HOLD_REASON_KEY_CHECK_ON_BOOT	/*(�J�n���̓p�b�h��������Ă��鎖���m�F����)*/
		|	TASK_SWITCH_01_HOLD_REASON_OBJECT_WAIT			/*(�J�n����obj���ޔ����Ă��鎖���m�F����)*/
		);
	kaiwa_system_terminate_flag = KAIWA_SYSTEM_00_CONTINUE;/*(0)*/	/* ������ */

	is_bg					= (0);
	ml_font[(0)].haikei 		= (ML_HAIKEI_03_MESSAGE);/* [��/��b�p�w�i]����Ӕw�ion */
//�p�~	cg.dr aw_flag_kaiwa_screen	= (1);/*0*/
	#if 1
	/* std_obj[]������ */
	{	unsigned int i;
		for (i=0; i<KAIWA_SPRITE_99_MAX; i++)/*20*/
		{
			kaiwa_sprite[i].img = NULL;
		}
	}
	// �v���C���[
	kaiwa_sprite[0].cx256		= t256(-96);		kaiwa_sprite[0].cy256		= t256(16);
	kaiwa_sprite[0].origin_x256 = t256(-96);		kaiwa_sprite[0].origin_y256 = t256(16);
	kaiwa_sprite[0].offset_x256 = t256(-32);		kaiwa_sprite[0].offset_y256 = t256(16);
	// �{�X
	kaiwa_sprite[1].cx256		= t256(352);		kaiwa_sprite[1].cy256		= t256(16);
	kaiwa_sprite[1].origin_x256 = t256(352);		kaiwa_sprite[1].origin_y256 = t256(16);
	kaiwa_sprite[1].offset_x256 = t256( 32);		kaiwa_sprite[1].offset_y256 = t256(16);
	obj_look_up 							= (KAIWA_SPRITE_99_MAX)+(1);/*(�S�����ڃI�u�W�F�ɐݒ�)*/
	#endif
	limit_timer 				= byou60(1);/*(1[�b]�������ԁB�N�����̃p�b�h�`�F�b�N����)*/
	i_code_text_color			= (7);
	i_code_text_speed_wait		= (0);
	str_stack_position = 0;/* ������̈� */
	return (1);
}


/*---------------------------------------------------------
	�Q�[���C�x���g���̃V�i���I��b �J�n(�X�g�[���A�G���f�B���O�ȊO)
---------------------------------------------------------*/

global void kaiwa_load_ivent(void)
{
	/* �t�@�C�����쐬 */
	{
		/* 'data' '/kaiwa/' �̕��̃I�t�Z�b�g */
		#define DIRECTRY_NAME_OFFSET	(DIRECTRY_NAME_DATA_LENGTH + DIRECTRY_NAME_KAIWA_LENGTH)
		strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR DIRECTRY_NAME_KAIWA_STR "Z/sZ1" DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR);
		my_file_common_name[DIRECTRY_NAME_OFFSET+0] = ('0'+(cg_game_select_player));
		my_file_common_name[DIRECTRY_NAME_OFFSET+3] = get_stage_chr(cg.game_now_stage);
		//
		if ((cg.state_flag & STATE_FLAG_05_IS_BOSS))/*(�{�X�퓬��̏ꍇ)*/
		{
			// my_file_common_name[DIRECTRY_NAME_OFFSET+4] = '1';
			//#if (1==US E_EASY_BADEND)
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
						my_file_common_name[DIRECTRY_NAME_OFFSET+4]++;
					}
				}
			}
			//#endif
		}
		else/*(�{�X�퓬�O�̏ꍇ)*/
		{
		//	my_file_common_name[DIRECTRY_NAME_OFFSET+4] = '0';
			my_file_common_name[DIRECTRY_NAME_OFFSET+4]--;
		}
	}
	kaiwa_system_terminate_call_func = kaiwa_system_terminate_game_core;	/* �V�i���I��b �I�������Q�[���R�A�p��ԑJ�� */
	load_kaiwa_youso_free();			/* �O��̃V�i���I���������ɂ���΂��ׂĊJ���B */
	if (0 == aaa_kaiwa_system_start())	// �t�@�C�����Ȃ��ꍇ�̓C�x���g���΂�
	{
		cg.state_flag |= STATE_FLAG_12_END_KAIWA_MODE;
	}
	else
	{
		cg.state_flag |= STATE_FLAG_06_IS_KAIWA_MODE;	/*on*/
		#if 0
		/* �V�i���I���Ƀ{�����������Ă��܂��B�o�O������̂ŁB */
		pd_bomber_time = 0;
		#endif
	}
}


#if (1)/*Gu�`��ɂȂ�Ɨv��Ȃ��Ȃ�*/
/*---------------------------------------------------------
	�u�X�g�[���[���[�h��p�v�V�i���I����
---------------------------------------------------------*/

static void story_mode_local_work(void)
{
	kaiwa_system_SDL_draw();					/* �V�i���I��b�V�X�e�� SDL �`��(�x��) */
	kaiwa_system_execute_move_only_main();		/* �V�i���I��b�V�X�e�� ����(�ړ�) */
}
#endif

/*---------------------------------------------------------
	�u�X�g�[���[���[�h��p�v�V�i���I�J�n
---------------------------------------------------------*/

global void story_mode_start(void)
{
//	bg_alpha = 0;
	/* �t�@�C�����쐬 */
	{
		strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR DIRECTRY_NAME_KAIWA_STR "story" DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR);
	}
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

global void kaiwa_system_init(void)/* �g�ݍ��� */
{
	#if 1
	kaiwa_youso_list_bigin	= NULL; 	/* ���ߗ�̊J�n�ʒu��ێ� */
	kaiwa_youso_list_scan	= NULL; 	/* ���ߑ����ʒu�Ŏg�p */
//
	ml_font[(0)].haikei 		= (ML_HAIKEI_m1_OFF);/* ����Ӕw�ioff */
//�p�~	cg.dr aw_flag_kaiwa_screen	= (0);		/* ����ӃE�B���h�E�\���t���O */
	is_bg						= (0);		/* �w�i�\���t���O */
	kanji_cursor_move_home_position();		/* �����J�[�\�����z�[���|�W�V�����ֈړ� */
	#endif
	bg_alpha					= (255);		/* 255==0xff �����l */

	/* SD L_FreeSurface(); �� psp�ł͑��������Ɠ����Ȃ��̂� ���̑΍� */
}


/*---------------------------------------------------------
	�I�����ɊJ�����镔��...�������ǁA
	�n���O�A�b�v�����肷��̂ŁA��芸���������ɂȂ��Ă�B
	(�J�����Ȃ��Ă��I������΁AOS���������������̂Ŗ��Ȃ����Č����Ζ��Ȃ�)
---------------------------------------------------------*/

global void kaiwa_system_exit(void)/* �O�� */
{
	/*msg_window_init()*/
//
	#if 0/* �{���͗v�� */
	#endif
}
