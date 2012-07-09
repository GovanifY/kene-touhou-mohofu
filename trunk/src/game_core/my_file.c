
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�t�@�C���֘A����
	-------------------------------------------------------
	�t�@�C���֘A�̃T�|�[�g���[�`���́A�����ɋL�q���Ă��������B

	�G�o���f�[�^�[(kaiwa/s/xxxx.txt)�t�@�C���ǂݍ���
	�V�i���I��b(kaiwa/0/xxxx.txt)�t�@�C���ǂݍ���
	�n�C�X�R�A�t�@�C���ǂݍ��݁^�ۑ�
	�ݒ�ǂݍ��݁^�ۑ�
	��ʕۑ�
---------------------------------------------------------*/

#include "111_my_file.h"
//#include <malloc.h>/* free(); */

// '|' ',' ',' const char kugiri_1_char
#define KUGIRI_KAMMA_1_CHAR 		(',')
#define COMMENT_SEMIKORON_1_CHAR	(';')

/*---------------------------------------------------------
	shift jis�R�[�h�A�S�p1�o�C�g�ڂ��ǂ������肷��q�֐�
---------------------------------------------------------*/

/*static*/global int is_kanji_1st(/*unsigned char*/int/*int*/ high_byte)
{
	#if 1
	high_byte &= 0xff;/**/
	return (
		((high_byte >= 0x81) && (high_byte <= 0x9f)) ||
		((high_byte >= 0xe0) && (high_byte <= 0xfd))
	);
	#else
	high_byte ^= 0x20;
	high_byte += (0x100-0xa1);
	high_byte &= 0xff;/**/
	return (high_byte < 0x3du);
	#endif
}


/*---------------------------------------------------------
	��b�t�@�C���̕����񕔕��̓ǂݍ���
	-------------------------------------------------------
	shift jis ������2byte�ڂ������̏ꍇ��
	�G�X�P�[�v�V�[�N�G���X������2byte�ڂ������̏ꍇ�ł�
	��肪�Ȃ�
---------------------------------------------------------*/

/*static*/global char *load_my_file_get_str(char *str, char *buffer, int *end_arg)
{
	int string_error_limiter;
	string_error_limiter = 200;/* 200 �����ȏ�̓G���[ */
	/*unsigned char*/int high_byte;
	{loop:;
		high_byte = ((*str) & 0xff);
		/* �󕶎���̉\�������邩��A�n�߂ɔ��� */
		if (KUGIRI_KAMMA_1_CHAR == high_byte)	/* ','��؂�ł����܂��̏ꍇ */
		{
			goto kamma_end;
		}
		else
		if (13 == high_byte)	/* '\n'==13(�s��)�ł����܂��̏ꍇ */
		{
	//	ret13_end:
			*end_arg = 1;/* �s���ł��B */
		kamma_end:
			*buffer = '\0'; 	/* EOS ��ǉ� */
			return (str);
		}
		else					/* �������]������K�v�̂���ꍇ */
		{
			int flag;
			flag = 0;
			if (is_kanji_1st(high_byte)) /* shift jis ���� */
			{
				;	//	*buffer++ = *str++;/* 1byte�� */
			}
			else
			{
				if ('\\' == high_byte)		/* �G�X�P�[�v�V�[�N�G���X����(escape sequence) */
				{
					;	//	*buffer++ = *str++;/* ���� */
				}
				else					/* ���p���� */
				{
					flag = 1;
				}
			}
			if (0==flag)	/* ���p�����ȊO(shift jis �����A�G�X�P�[�v�V�[�N�G���X����)�� 2 byte�]�� */
			{
				*buffer++ = *str++;
			}
			*buffer++ = *str++; 	/* 1 byte�͕K���]�� */
			/* �G���[�`�F�b�N */
			{
				string_error_limiter--;
				if (0 >= string_error_limiter)
				{
					return ((char *)NULL);
				}
			}
		}
		goto loop;
	}
}


/*---------------------------------------------------------
	�ėp���̂܂�łȂ��ǂݍ��݊֘A�B�q�֐�
	-------------------------------------------------------
	�W�����o�͂�psp�ł͒x������̂�sce�̊֐����g���B(�Q�[�����̏��������y����)
	�G���[�`�F�b�N�Ƃ��S�R�����̂Œ��ӂ̎�
---------------------------------------------------------*/

//atic char my_file_line_buffer256[128];		/* parth text, 1 line buffer */ 	/* ��������s�̎擾 */
/*static*/ char my_file_line_buffer256[256];	// ���zfgets()�p�̍s�o�b�t�@		/* parth text, 1 line buffer */ 	/* ��������s�̎擾 */
/*static*/ char my_file_common_name[256];		// �ǂ�/��������ꍇ�̃t�@�C����	/* fopen����t�@�C���� (128) */
#if 1
static u32 file_size; 	// �ǂݍ��񂾃t�@�C���T�C�Y
static u32 file_seek; 	// ���z�t�@�C���̓ǂݏo���ʒu
//static char *my_buf;
//static void *malloc_buf;
static char *malloc_buf;			// ���z�t�@�C���o�b�t�@

/*---------------------------------------------------------
	�w�肵���t�@�C���S�����A���z�t�@�C���o�b�t�@�ɓǂݍ��݂܂��B
	���z�t�@�C���̓ǂݏo���ʒu(file_seek)��擪�Ɉړ������܂��B
---------------------------------------------------------*/

/*static*/global void *my_file_fopen(void)/*const char *my_file_common_name*/ /*, const char *dummy*/
{
	SceUID fd;
	fd = sceIoOpen((char *)my_file_common_name, PSP_O_RDONLY, 0777);	// �t�@�C�����J��
	if (0 == fd)	// �J���Ȃ�������G���[
	{
		goto error111;
	}
	file_size = sceIoLseek32(fd, 0, PSP_SEEK_END);	// (���V�[�N�ʒu���Ō�Ɉړ�������)�t�@�C���̃T�C�Y�𒲂ׂ�B
	file_seek = 0;									// ���z�V�[�N�ʒu��擪�Ɉړ��B

	malloc_buf = malloc(file_size); 				// �t�@�C���̃T�C�Y���̃�������(�擾����)�m�ۂ���B
	if (NULL == malloc_buf) 						// ���������m�ۏo���Ȃ��ꍇ�̓G���[
	{
		sceIoClose(fd);
		goto error111;
	}
	sceIoLseek32(fd, 0, PSP_SEEK_SET);				// ���V�[�N�ʒu��擪�Ɉړ��B
	sceIoRead(fd, malloc_buf, file_size);			// ��C�ɑS���������ɓǂݍ��ށB
	sceIoClose(fd);
//	my_buf = malloc_buf;
//
	return (malloc_buf);
error111:
	return (NULL);	// ���s
}
global int my_file_now_size(void)
{
	return (file_size);
}


/*---------------------------------------------------------
	�w�肵��������(char*) �� �w�肵�������ǂݍ��݂܂��B
	���z file_seek ���ړ������܂��B
---------------------------------------------------------*/

/*static*/global void my_file_read_to_mem(char *write_memory, int read_length)
{
//	error(ERR_FATAL, "TEST %s\nno: %d (%s)",buffer_name,errno,strerror(errno));
//	return (NULL);
	if (file_seek + read_length <= file_size)
	{
		memcpy(write_memory, malloc_buf + file_seek, read_length);
		file_seek += read_length;
	}
	else
	{
		//error
	}
}

/*---------------------------------------------------------
	�ǂݍ��ݍς݂̃���������fgets()���ۂ��ǂݍ��݂����܂��B
	���z file_seek ���ړ������܂��B
---------------------------------------------------------*/

static int my_fgets(void)/*char *buffer_name, int num, char *wfp*/
{
	int ii;
	ii = 0;
	char aaa;
//	char bbb;
//bbb=0;
	fgets_loop:;
	aaa = /*buffer_name*/my_file_line_buffer256[ii] = malloc_buf[file_seek];/*(*my_buf)*/
//	my_buf++;
	ii++;
	file_seek++;
	if (0x0a == aaa)			{	return (1);}
	if (file_size < file_seek)	{	return (0);}/*NULL*/
	goto fgets_loop;
//	error(ERR_FATAL, "TEST %s\nno: %d (%s)",buffer_name,errno,strerror(errno));
//	return (NULL);
}

/*---------------------------------------------------------
	��L�ɉ����A�擪�� ';' �Ŏn�܂���̂𖳌��ɂ��܂��B
---------------------------------------------------------*/

/*static*/global int my_file_fgets(void)/*char *buffer_name, int num, char *wfp*/
{
ggg_loop:;
	int ii;
	ii = my_fgets();
	/* ';'�Ŏn�܂�s�̓R�����g�s�Ȃ̂ŁA�������Ȃ��Ŏ��̍s�܂Ŕ�΂��B */
	if (COMMENT_SEMIKORON_1_CHAR == (my_file_line_buffer256[0]))
	{	goto ggg_loop;/*continue;*/ }
	return (ii);
}

/*---------------------------------------------------------
	�t�@�C���o�b�t�@��j�����܂��B
---------------------------------------------------------*/

/*static*/global void my_file_fclose(void)/*void *wfp*/
{
	free(malloc_buf);
}
//(????)
	/*static*/global void my_fseek_begin(void)/*void *wfp*/
	{
	//	file_seek = 0;
	}
#else
	#define my_file_fopen	fopen
	#define my_fgets		fgets
	#define my_file_fclose	fclose
#endif

//#define FILE_NAME_SETTING_TXT "set ting.ini"
//#define FILE_NAME_SETTING_TXT 	DIRECTRY_NAME_DATA_STR "/set ting.txt"
#define FILE_NAME_SETTING_TXT		DIRECTRY_NAME_DATA_STR "/config_r35.txt"


/*---------------------------------------------------------
	�n�C�X�R�A�t�@�C���֘A����
---------------------------------------------------------*/

/*---------------------------------------------------------
	�ݒ�t�@�C���֘A����
	-------------------------------------------------------
	�ݒ�t�@�C���̐����╶�����ǂ݂��ށB
	0�ȏ��ok
	���l�̓G���[
---------------------------------------------------------*/
static int ini_load_value(char *str_search, char *str_result)/*FILE *fp,*/
{
//	char my_file_line_buffer256[128];		/* �s�o�b�t�@ */
	char item_name[32];/*30*/			/* �R���t�B�O���ږ� */
//	int int_result; int_result = -1;
	/* ��������� */
	my_fseek_begin();//fseek(fp, 0L, SEEK_SET);
	{
		MY_FILE_LOOP_BEGIN;
		{	char *sc = item_name;
			int i=0;
			while (KUGIRI_KAMMA_1_CHAR != (*ch))	/* ��؂�',' ��T�� */
			{
				i++;
				if (30 < i) 	{return (-1);}	/* >= 30 ����������G���[ */
				*sc++ = *ch++;
			}
			ch++;		/* ��؂�',' �𖳎� (',' �̕����Ƃ΂�) */
			*sc = 0;	// '\0' ��ǋL�B  NULL
		}
		//
	//	error(ERR_FATAL, "test %s | %s\n", c, item_name);/* test 1\n | k0a */
		//
		if (0==tiny_strcmp(item_name, str_search))
		{
			if (NULL==str_result) // �Ώۂ𐮐��Ƃ��ĉ�͂���B(PARTH_MODE_INT) */
			{
				char tmp_str32bytes[32];/*(atoi�̏ꍇ�A�o�b�t�@��32[bytes]==�ő�31�����ɓ���)*/
				char *tmp_str_position = tmp_str32bytes;
			//	while ('\n' != (*ch))/* int�̕��͐�������Ȃ����͔r�����Ă����݂�����������Ȃ��B(13�łȂ���ok) */
				while (0x0a != (*ch))/* int�̕��͐�������Ȃ����͔r�����Ă����݂�����������Ȃ��B(13�łȂ���ok) */
				{
					*tmp_str_position++ = *ch++;
				}
				tmp_str_position = 0;
				/*int_result =*/return (atoi(tmp_str32bytes));
//				goto my_exit;
			}
			else // �Ώۂ𕶎���Ƃ��ĉ�͂���B (PARTH_MODE_CHAR) */
			{
				char *tmp_str_position = str_result;
			//	while (0x0d != (*ch)/*13*/) /* char�̕���\n����Ȃ���13�ɂ��Ȃ��Ƃ����Ǝ���Ă���Ȃ���B(13�łȂ���ng) */
				while (0x0a != (*ch))		/* char�̕���\n����Ȃ���13�ɂ��Ȃ��Ƃ����Ǝ���Ă���Ȃ���B(13�łȂ���ng) */
				{
					*tmp_str_position++ = *ch++;
				}
				tmp_str_position = 0;
				/*int_result =*/return (0);/* ok */
//				goto my_exit;
			}
		}
		MY_FILE_LOOP_CONTINUE;
		MY_FILE_LOOP_END;
	}
//my_exit:
//	return (int_result);
	return (-1);/* ������Ȃ�������G���[ */
}


static const char *my_config_title[OPTION_CONFIG_08_MAX]=
{
	"player",
	"bomb",
	"bgm",
	"sound",
	"current_difficulty",
	"current_player",
	"analog",
	"open",
};

extern void set_default_key(u32 *key_setting_map, int key_setting_type);
extern void set_default_option(int *option_setting_map);
extern void check_limit_value_option(int *option_setting_map);
global void ini_file_load(void)
{
	#if 1
	/* sceIoMkdir();�Ńp�X���쐬����ꍇ�A�Ōオ '/' ���ƍ쐬����Ȃ��̂Œ��ӁB */
	// �v�]���������̂őS�͂őΉ�
//	sceIoMkdir(const char *aaa_dir, SceMode mode);
	sceIoMkdir("ms0:/PICTURE", 0777);	/* "ms0:/PICTURE"���Ȃ��ꍇ�A
	"ms0:/PICTURE/�����͕핗"�����Ȃ��̂ŁA�K���K�v�B
	�Ȃ��ꍇ�A���������̂ł͂Ȃ��āA�ň��u�n���O�A�b�v�����肷��v�B
 */
//	sceIoMkdir("ms0:/PICTURE/�����͕핗/", 0777);/* �o���Ȃ� */
	sceIoMkdir("ms0:/PICTURE/�����͕핗", 0777);/* �o���� */
	#endif
//	char my_file_common_name[128/*64 50*/];
//	strcpy(my_file_common_name, "./" FILE_NAME_SETTING_TXT);
	strcpy(my_file_common_name, FILE_NAME_SETTING_TXT);
	int int_result;
	int_result = 0;/* �ُ� */
	int ng1;
	ng1 = 0;/*fopen()����*/
//
//	FILE *fp;
	if (NULL == /* fp =*/ my_file_fopen())	/* �J���Ȃ������Ƃ� */		/*my_file_common_name, "r"*/
	{	/* �ǂݍ��ݎ��s */
		ng1 = 1;/*fopen()���s*/ goto error00;
	}
	/* �L�[�R���t�B�O�ݒ�̓ǂݍ��� */
	{
		char key_config_tag[4];
		key_config_tag[0] = 'K';
		key_config_tag[1] = '0';
//		key_config_tag[2] = ('a'+i);
		key_config_tag[3] = 0;
		{	unsigned int i;
			for (i=0; i<KINOU_08_WARIATE_MAX; i++)
			{
				key_config_tag[2] = ('a'+i);
			//
				{	int value;
					value = ini_load_value(key_config_tag, NULL);/*fp,*/
					if (0 > value)	{goto error00;	}
					else			{pad_config[i] = value;	}
				}
			}
		}
	}
	/* �I�v�V�����ݒ�̓ǂݍ��� */
	{	unsigned int i;
		for (i=0; i<OPTION_CONFIG_08_MAX; i++)
		{
			int value;
			value = ini_load_value((char *)my_config_title[i], NULL);/*fp,*/
			if (0 > value)	{goto error00;	}
			else			{option_config[i] = value; }
		}
	}
	int_result = 1;/* ���� */
//
error00:
	/* high_score load */
	{	int ng2;
		int tmpscore;
		ng2 = ng1;
		tmpscore = 0;
		char search_str_name32[8];/*(�������閼�O)*/
		//-----------------01234567
		strcpy(search_str_name32, "SCORE00");
		search_str_name32[7] = 0;/* �O�̈� */
//
		int j;
		for (j=0; j<MAX_8_SAVE_PLAYERS; j++)
		{
			search_str_name32[5] = '0'+j;/* PLAYER */
			unsigned int i;
			for (i=0; i<MAX_5_RANKING; i++)
			{
				search_str_name32[6] = '0'+i;/* score rank number. ���� */
				if (0==ng2)
				{
				//#define readed_str_name32 my_file_line_buffer256
					char readed_str_name32[64/*50*/];
				//	if (1 == fscanf(fp, "%23s\n", readed_str_name32 ))
				//	if (0 != my_fgets(/*fp, "%23s\n", readed_str_name32*/ ))
					int value;
					value = ini_load_value(search_str_name32, readed_str_name32);
					if (0 > value)	{ng2 = 1;/* �G���[ */}
					else
					{
						/* ���ߍ��� */
						high_score_table[j][i].final_stage = readed_str_name32[1/*10*/]-('0');

					//	strncpy(high_score_table[j][i].name, &readed_str_name32[10], 3);
						high_score_table[j][i].name[0] = readed_str_name32[2];/*10*/
						high_score_table[j][i].name[1] = readed_str_name32[3];/*11*/
						high_score_table[j][i].name[2] = readed_str_name32[4];/*12*/
						high_score_table[j][i].name[3] = readed_str_name32[5];/*13*/
//
						high_score_table[j][i].name[4] = readed_str_name32[6];/*14*/
						high_score_table[j][i].name[5] = readed_str_name32[7];/*15*/
						high_score_table[j][i].name[6] = readed_str_name32[8];/*16*/
						high_score_table[j][i].name[7] = readed_str_name32[9];/*16*/
						{	char tmp_str32bytes[32];/*(atoi�̏ꍇ�A�o�b�t�@��32[bytes]==�ő�31�����ɓ���)*/
							strcpy(tmp_str32bytes, &readed_str_name32[10]);/*5 13*/
							tmpscore = atoi(tmp_str32bytes);
						}
					}
				}
				#if (1)
			/*	else // ���O�� ini_load_value() �ŃG���[���N�����ꍇ�ɏ���������̂� ����else�����B */
			//	if (1==ng2) /* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
				if (0!=ng2)/*(�G���[�������Ă���ꍇ�A�����I�ɃX�R�A�e�[�u��������������B)*/
				{
					high_score_table[j][i].final_stage = (6-i);/* ���B�X�e�[�W */
					static const int init_score_tbl[5] =
					{
						score(100000000),//score(70000000),//score(50000000),
						score( 50000000),//score(60000000),//score(4000000),
						score( 10000000),//score(50000000),//score(300000),
						score(	5000000),//score(10000000),//score(20000),
						score(	1000000),//score( 5000000),//score(1000),
					};
				//	strcpy(high_score_table[j][i].name, "12345678");
				//	strcpy(high_score_table[j][i].name, "ZUN     ");
					strcpy(high_score_table[j][i].name, "Nanashi ");
					tmpscore = init_score_tbl[i];
				}
				#endif
				high_score_table[j][i].score = tmpscore;
			}
		}
	//	top_score = high_score_table[0][0].score;	// ��ɕ\������n�C�R�A�̎擾=>score.c�ŗ��p
	}
//
	if (0==ng1)
	{
		my_file_fclose();/*fp*/ //fclose(fp);
	}
	//return (int_result)/*1*/;
	if (0==(int_result)) // 20090110
	{
//		chooseModDir();
		set_default_key(pad_config, 0/*0==type 01 �͕핗 �W��*/);
		set_default_option(option_config);/* OPTION_CONFIG_00_PLAYER ���� OPTION_CONFIG_03_SOUND �܂Őݒ� */
	//	option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY]	= 0;
	//	option_config[OPTION_CONFIG_05_CURRENT_PLAYER]		= 0;
	//	option_config[OPTION_CONFIG_06_ANALOG]				= 0;
		option_config[OPTION_CONFIG_07_OPEN]				= 0x0500;
	}
	/* �͈͊O�`�F�b�N(�͈͊O�̏ꍇ�͏C��) */
	check_limit_value_option(option_config);/* */
	/* �ǂݍ��񂾃f�[�^�[�ŏ����ݒ� */
	cg.game_difficulty		= option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY];
	cg_game_select_player	= option_config[OPTION_CONFIG_05_CURRENT_PLAYER];
	#if 1/*(�͈͊O�̏ꍇ�͋����I�ɏC������B)*/
	cg.game_difficulty		&= 0x03;/* (easy)0 1 2 3(Lunatic) */
	cg_game_select_player	&= 0x07;
	#endif
}


/*---------------------------------------------------------
	���C���o�b�t�@�̓��e(EOS�`��)�ɁA���s��[CR+LF]�`���Ƃ��ĕt�������܂��B
	���̌�t�@�C���ɏ����o���܂��B
---------------------------------------------------------*/

static void write_line_buffer_to_file(SceUID fd)/*FILE *fp*/ /*, char *str_buf*/
{
	char *str_pointer;
	str_pointer = my_file_line_buffer256;
//	const char k = 13;
//	fprintf(fp, "%s%c\n", str_pointer,	k);
	unsigned int len;
	len = 0;
	/* ������I�[�𒲂ׂ�B */
	{
	string_retry:/*(������x)*/
		if (0 == (*str_pointer))	/* ������I�[���B */
		{	goto string_terminate;	}
		#if (0)/*(�G���[�`�F�b�N)*/
	//	if (253 < len)/* ��������ꍇ�͋����ł��؂�B(254==0x0d, 255==0x0a, [256]�Ȃ̂�[255]�܂Ŏg����B) */
		if (250 < len)/* �e�L�g�[ */
		{	goto string_terminate;	}
		#endif
		len++;
		str_pointer++;
		goto string_retry;
	string_terminate:/*(��������)*/
		;
	}
	/* �������͕핗�̃t�@�C���`����[CR+LF]�������̂Ō݊������B */
	*str_pointer = 0x0d;	len++;		str_pointer++;/*[CR]*/
	*str_pointer = 0x0a;	len++;		str_pointer++;/*[LF]*/
//	fprintf(fp, "%s\n", str_pointer);
	sceIoWrite(fd, my_file_line_buffer256, len);/*sizeof(int)*/ /**save_data_size*/ /*, 1, fp*/
}
global void ini_file_save(void)
{
//	FILE *fp;
//	char buf[128];/*64 50*/
	strcpy(my_file_common_name, "./" FILE_NAME_SETTING_TXT);
//	fp = fopen(buf, "w");
	SceUID fd = sceIoOpen((char *)my_file_common_name, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
//	if (0 > fd)
//	{
//		/*"�Z�[�u�f�[�^���Ȃ��B"*/
//	}
//	if (NULL == fp) 	{	return; 	}
//
	/* �I���������� off. */
	#define USE_MEMO (0)
	#if (1==USE_MEMO)

	// 'CR'=='\r' �Ƃ����̂� carrige Return (���s)�̏ȗ��`�B '\r' �̈Ӗ���(���s)�R�[�h�B
	// 'LF'=='\n' �Ƃ����̂� New line (�V�s)�̏ȗ��`�B '\n' �̈Ӗ���(�V�K�Ɏ��̍s�ɂ���)�R�[�h�B
	// 'LF' �́A Line Feed (�s�ς�)�̏ȗ��`�B�s��ς���̂�����A�Ӗ��͐V�s�ƑS�������B
	// ����͌��X�[���p��ł͂Ȃ��A�v�����^(��)�p��B���ꂪ�[��(Terminal)�p��Ɍp�����ꂽ�B
	//---------------------------
	// MS-DOS/Windows �ł́A�e�L�X�g�t�@�C���̍s����[CR+LF]�`�����̗p���Ă���B
	// "������"��ms-dos��"edit"�ŐV�K�t�@�C�����쐬���A�K���ɉ��s�����ĕۑ�����Ƃ��̌`���B
	//---------------------------
	// Unix(Linux/FreeBSD��)�ł́A�e�L�X�g�t�@�C���̍s����[LF]�`�����̗p���Ă���B
	// "vi"��"editer"�ŐV�K�t�@�C�����쐬���A�K���ɉ��s�����ĕۑ�����Ƃ��̌`���B
	//---------------------------
	// �C���^�[�l�b�g�̃T�[�o�[�ł́A�W���I�Ƀe�L�X�g�t�@�C���̍s����[LF]�`�����̗p���Ă���B
	//---------------------------
	// [Windows�n PC]->[�T�[�o�[]��upload���ɁA�e�L�X�g�t�@�C����[CR+LF]->[LF]�ϊ����A
	// [Windows�n PC]<-[�T�[�o�[]��download���ɁA�e�L�X�g�t�@�C����[LF]->[CR+LF]�ϊ����Ă���B
	// �T�[�o�[�̑��肪[Unix�n PC]�̏ꍇ�́A�����ϊ����Ȃ��B
	//---------------------------
	// �A���A�e�L�X�g�t�@�C���łȂ����̂��e�L�X�g�t�@�C���ƔF�������ꍇ�A�ϊ��@�\������ƃt�@�C�����j�󂳂��ׁA
	// �e�L�X�g�t�@�C���������I��"�o�C�i���t�@�C��"�Ƃ��Ĉ����A�ϊ����Ȃ��T�[�o�[������B
	// �t�ɖ��炩��".png"�摜�Ȃ̂ɁA(Windows�n PC��download����)�����[LF]->[CR+LF]�`���ɕϊ����A
	// �ǂ߂Ȃ��Ȃ����Ⴄ�T�[�o�[������B(�������Ǘ��҂̐ݒ肪����)�B�����Ƃ͌���Ȃ����B
	//---------------------------
	#define KAIGYOU_CR_LF "\r\n"
	strcpy(my_file_line_buffer256,
		";-------------------------------------" KAIGYOU_CR_LF
		"; �����͕핗(r35) configuration file."  KAIGYOU_CR_LF
		";-------------------------------------" KAIGYOU_CR_LF
		KAIGYOU_CR_LF
		"; �L�[�R���t�B�O�ݒ�" KAIGYOU_CR_LF
	);
	write_line_buffer_to_file(fd);/*fp*/
	#endif	/* (USE_MEMO) */
	//
	{
		unsigned int i;
		for (i=0; i<KINOU_08_WARIATE_MAX; i++)
		{
			sprintf(my_file_line_buffer256,
				"K0%c,%d",
				('a'+i),
				(signed)pad_config[i]);
			write_line_buffer_to_file(fd);/*fp*/
		}
	}
	//---------------------------
	#if (1==USE_MEMO)
	#define TITLE_OPTION_SETTEI 	KAIGYOU_CR_LF "; �I�v�V�����ݒ�" KAIGYOU_CR_LF
	strcpy(my_file_line_buffer256, TITLE_OPTION_SETTEI );
	write_line_buffer_to_file(fd);/*fp*/
	#endif	/* (USE_MEMO) */
	option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY]	= (cg.game_difficulty);
	option_config[OPTION_CONFIG_05_CURRENT_PLAYER]		= (cg_game_select_player);

	{
		unsigned int i;
		for (i=0; i<OPTION_CONFIG_08_MAX; i++)
		{
			sprintf(my_file_line_buffer256,
				"%s,%d",
				my_config_title[i],
				option_config[i]);
			write_line_buffer_to_file(fd);/*fp*/
		}
	}
	//---------------------------
	#if (1==USE_MEMO)
	#define TITLE_YUME_NO_KIROKU	KAIGYOU_CR_LF "; ���̋L�^" KAIGYOU_CR_LF
	strcpy(my_file_line_buffer256, TITLE_YUME_NO_KIROKU );
	write_line_buffer_to_file(fd);/*fp*/
	#endif	/* (USE_MEMO) */
	/* high_score save */
	{	unsigned int j;
		for (j=0; j<MAX_8_SAVE_PLAYERS; j++)
		{	unsigned int i;
			for (i=0; i<MAX_5_RANKING; i++)
			{
				sprintf(my_file_line_buffer256,
					"SCORE" 		/* == dummy */
					"%c"			/* player number */
					"%c"			/* rank number */
					"," 			/* == ��؂� dummy */
					"0" 			/* practice mode */
					"%c"			/* final stage */
					"%8s"			/* name */
					"%09d"/*"0"*/,	/* score */
				//
					(j+'0'),		/* player number */
					(i+'0'),		/* score rank number */
									/* practice mode */
					(high_score_table[j][i].final_stage+'0'),/* final stage */
					high_score_table[j][i].name,
					(int)high_score_table[j][i].score/* gcc 4.3.5 */
				);
				write_line_buffer_to_file(fd);/*fp*/
			}
		}
	}
	sceIoClose(fd);/*fclose(fp);*/
}

/*---------------------------------------------------------
	��ʕۑ��@�\
---------------------------------------------------------*/

global void save_screen_shot(void)
{
	static int screen_num = 0;
//	sp rintf(my_file_common_name, "ms0:/PICTURE/Toho_Moho%d.bmp", screen_num);	// �ۑ��ꏊ�̕ύX�B
//	strcpy(my_file_common_name, "ms0:/PICTURE/�����͕핗/Toho_MohoZ.bmp");	//	�u33�����ڂ��������������v
//	strcpy(my_file_common_name, "ms0:/PICTURE/kene/MohoZ.bmp"); /* "ms0:/PICTURE/kene/"�̍쐬���@���킩��Ȃ� */
//	strcpy(my_file_common_name, "ms0:/PICTURE/Toho_MohoZ.bmp"); 			//	�u22�����ڂ��������������v
	strcpy(my_file_common_name, "ms0:/PICTURE/�����͕핗/�X�N�V��_Z.bmp");	//	�u33�����ڂ��������������v
//	-----------------------------0123456789012345678901234567890123
//	-----------------------------0000000000111111111122222222223333
	screen_num++;
	screen_num &= 0x1f;
	my_file_common_name[33/*22*/]= ((9+1)<screen_num)?(('A'-(9+1)-1)+screen_num):(('0'-1)+screen_num);
	#if (0==USE_32BIT_DRAW_MODE)
	PSPL_save_vram16_to_file(my_file_common_name);	/*(��ʃ��[�h��16bit�̏ꍇ�ɉ�ʂ��Z�[�u)*/
	#else
	/*(����ĂȂ�)*/								/*(��ʃ��[�h��32bit�̏ꍇ�ɉ�ʂ��Z�[�u)*/
	#endif
}
