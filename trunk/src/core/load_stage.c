
/*---------------------------------------------------------

---------------------------------------------------------*/

#include "game_main.h"
#include "load_stage.h"


#if 0
/*---------------------------------------------------------
	���Ԃ�ėp���̂܂�łȂ�strcmp�B�q�֐�
	-------------------------------------------------------
	�����A�召����͂���܂���Bascii�R�[�h(�ƃV�t�gJIS�R�[�h)�ȊO�͑Ή����Ă��܂���B
	������B���͕K��const�萔�Ȃ̂�(0�ŏI���ƕۏ؂���Ă���Ƃ݂Ȃ���)�G���[�`�F�b�N���Ȃ��Ă܂��B
---------------------------------------------------------*/

/*static*/ int tiny_strcmp(char *aaa, const char *bbb)
{
loop:
	if ( (Uint8)(*aaa++) != (Uint8)(*bbb) ) {	goto not_equal; }
	if (0x00 != (Uint8)(*bbb++) )			{	goto loop;	}
	return (0);
not_equal:
	return (1);
}
#else
extern int tiny_strcmp(char *aaa, const char *bbb);
/* MIPS R4000�n�ɂ��킹�āA�œK�����Ă݂܂����B */
#endif




/*---------------------------------------------------------

---------------------------------------------------------*/

extern int errno;

STAGE_DATA *leveltab = NULL;

/*---------------------------------------------------------
	�q�֐�
	Initial task, "free"ed the altenative entris.
---------------------------------------------------------*/

static void load_stage_free_entry(void)
{
	STAGE_DATA *l = leveltab;
	STAGE_DATA *n;
	while (NULL != l)
	{
		n = l->next;
		free (l);
		l = n;
	}
	leveltab = (STAGE_DATA *)NULL;/* head to set NULL */
}

/*---------------------------------------------------------
	�q�֐�
---------------------------------------------------------*/


/*---------------------------------------------------------
	�q�֐�
---------------------------------------------------------*/
extern void load_bg2_chache(char *filename, int use_alpha);
/* Do set the entry. */
static void load_stage_add_entry(Uint32 time10, char user_command, char *user_string, int user_x, int user_y)		// �I�u�W�F�N�g�̐���
{
	STAGE_DATA *new_entry;
	new_entry			= mmalloc(sizeof(STAGE_DATA));
//	new_entry->v_time	= (time10*6/*100*1000*/);/* �ǂݍ��݌�APSP�ɓs���̗ǂ��l�ɕϊ�(1/10[sec]-> 1/1000000[sec] == 1[nsec] == nano seconds ) */
	new_entry->v_time	= (time10*6/*100*1000*/);/* �ǂݍ��݌�APSP�ɓs���̗ǂ��l�ɕϊ�(1/10[sec]-> 1/60[sec] ) */
	new_entry->user_command = user_command;
	strncpy(new_entry->user_string, user_string, (MAX_PARA1_44-1)/*63*/);
	new_entry->user_x	= user_x;
	new_entry->user_y	= user_y;
	new_entry->done 	= 0;

/* �Q�[�����́A���ԃR�[�h�`���̃R�}���h�������\����͂͂��Ȃ��B(�\����͂�loadlv.c�ōs��) */

//
	const char *ctype_name[BTYPE_MAX/*CTYPE_MAX_23_*/] =
	{	/* ���� 23 ��� */
		NULL,/* [�ԕ���؂�] */
	//	���̑�
		"QUIT", 	/* �Q�[�� �S�X�e�[�W �N���A�[ */
	//	�{�X
		"�A���X",	/*	"BOSS01",*/
	//	"�`���m",	/*	"BOSS05",*/
		"��",		/*	"BOSS02",*/
		"����", 	/*	"BOSS03",*/ 		/* 3�ʂ͂ǂ����邩���� */
		"�P��", 	/*	"BOSS03",*/
		"�p�`�F",	/*	"BOSS01",*/
		"���", 	/*	"BOSS04",*/ 	// [***090207		�ǉ�
	//	����G[���^�G]
		"�d��1",	/*	"GFAIRY",*/ 	// [***090207		�ǉ�
		"�d��2",	/*	"GFAIRY",*/ 	//		�ǉ�
//
	//	���̑��U�R
		"���΂�1",	/*	"DRAGER",*/
		"���ґ�1",	/*	"MING",*/
		"���ґ�2",	/*	"GREETER",*/
	//	�ы�
		"�Ԗы�1",	/*	"MINE",*/
		"���ы�1",	/*	"EYEFO",*/
		"�Ζы�1",	/*	"XEV",*/
		"�Ζы�2",	/*	"CRUSHER",*/
		"�ы�1",	/*	"BADGUY",*/
		"�ы�2",	/*	"BADGUY",*/
	//	���� �A�z��
		"�A�z��1",	/*	"PLASMABALL",*/
		"����1",	/*	"PROBALL",*/
	//	�d��
		"�͗d��1",	/*	"CUBE",*/
	//	���d��
		"���C�h1",	/*	"RWINGX",*/
		"���C�h2",	/*	"ZATAK",*/
		"���C�h3",	/*	"CURVER",*/
		"���C�h4",	/*	"CIR",*/
	//	���d��
		"�d��1",	/*	"FAIRY",*/		// [***090207		�ǉ�
		"�d��2",	/*	"SPLASH",*/ 	// [***090124		�ǉ�
		"�d��3",	/*	"SPLASH",*/ 	//		�ǉ�
		"�d��4",	/*	"SPLASH",*/ 	//		�ǉ�
//	};
	/* ���� 6 ��� */
//	const char *btype_name[BTYPE_MAX] =
//	{
		NULL,/* [�ԕ���؂�] */
		"��",		/*	"BGPANEL",*/
		"�_",		/*	"BGPANEL2",*/
		"�����w1",	/*	"GROUNDER",*/
		"�����w2",	/*	"MAGICF",*/
		"BG",		/*	"BG_CONTROL"*/		/* ���V�X�e���R�}���h�Ȃ̂ŉp��ɂ��� */
	};
//
	/* �ǂݍ��񂾃R�}���h�𒆊ԃR�[�h�`���ɕϊ����� */
	switch (new_entry->user_command)
	{
	case 'T':	new_entry->user_command=ETYPE_01_ENGLISH_TEXT;		break;	/* english Text */
	case 'B':	new_entry->user_command=ETYPE_02_LOAD_BG;
	//	if ( 0 == tiny_strcmp(new_entry->user_string,"0") ) /* �t�@�C�������O�̏ꍇ�V�X�e���R�}���h[�g���\��] */
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
		break;	/* Background */
	/* �Q�[���������������Ȃ��悤�ɗ\�߉摜�L���b�V���ɋl�߂�����Ȃ̂ŁAP�R�}���h���Q�[���������������Ȃ��悤�ɍ��͓̂���B
	(�e�ʊJ�n�O�ɂ��̃X�e�[�W�Ŏg��P�R�}���h�摜���p�X�^�b�N�ɋl�߂�Ώo���Ȃ������Ȃ�) */
//	case 'P':	new_entry->user_command=ETYPE_03_PICTURE;	break;	/* Picture */
	case 'E':	/* add enemy */
		{
			for (new_entry->user_command = /*CTYPE_00_unknown+*/(CTYPE_MAX_23_-1); /*0*/CTYPE_00_NONE/*CTYPE_00_unknown*/ < new_entry->user_command; new_entry->user_command--)
			{
				if (0==tiny_strcmp( new_entry->user_string, /*(char *)*/&ctype_name[(unsigned int)new_entry->user_command][0] ) )
				{
					break;
				}
			} // 0/*CTYPE_00_unknown*/ == aaa );
		}
		break;
	default:	// add background tiles....
		{
			/* KETM�͖�20-30fps����ł����A���ݖ�60fps����Ȃ̂ł��܂�ɑ�������ׁA���x�l�𔼕��ɏC�����܂����B */
			#if 1
			{const unsigned short speed256_tbl[16] =
			{
				#if 0
				/*ketm09�݊�*/
				t256(0.5), t256(0.1), t256(0.2), t256(0.3),/* - - - - */
				t256(0.4), t256(0.5), t256(0.6), t256(0.7),/* - 5 6 7 */
				t256(0.8), t256(0.9), t256(0.5), t256(0.5),/* 8 9 - - */
				t256(0.5), t256(0.5), t256(0.5), t256(0.5) /* - - - - */
				#else
				/*�g��*/
				t256(0.0), t256(0.1), t256(0.2), t256(0.3),/* 0 1 2 3 */
				t256(0.4), t256(0.5), t256(0.6), t256(0.7),/* 4 5 6 7 */
				t256(0.8), t256(0.9), t256(1.0), t256(1.5),/* 8 9 J K */
				t256(2.0), t256(2.5), t256(3.0), t256(4.0) /* L M N O */
				#endif
			};
			new_entry->scroll_speed256/*ctype*/ = speed256_tbl[((new_entry->user_command) & 0x0f)];
			}
			#endif
		}
		#if 1
		{
			for (new_entry->user_command = /*CTYPE_00_unknown+*/(BTYPE_MAX-1); /*0*/BTYPE_00_NONE/*CTYPE_00_unknown*/ < new_entry->user_command; new_entry->user_command--)
			{
				if (0==tiny_strcmp( new_entry->user_string, /*(char *)*/&/*btype_name*/ctype_name[(unsigned int)new_entry->user_command][0] ) )
				{
					break;
				}
			} // 0/*CTYPE_00_unknown*/ == aaa );
		}
		#endif
		break;
	}
//
	if (NULL==leveltab) 	// �ŏ��̉�͍s���H
	{	new_entry->next = (STAGE_DATA *)NULL;}
	else
	{	new_entry->next = leveltab; 	}	// leveltab�͑O��̃f�[�^
	leveltab = new_entry;		// leveltab�ɍ����������f�[�^�̃A�h���X�����B
	// next�ɂ�dat�t�@�C���I�ɂ͌��ݑ������̍s�̏�̍s�������Ă���B
}

/* ���� */

	//	new_entry->user_x = new_entry->user_y/1000;
	//	new_entry->user_y = new_entry->user_y%1000;

			#if 0
			/*const*/ short speed256 = (short)(t256(1.0));
			switch (new_entry->user_command)
			{
			case '1':  speed256=(short)(t256(0.1)); break;
			case '2':  speed256=(short)(t256(0.2)); break;
			case '3':  speed256=(short)(t256(0.3)); break;
			case '4':  speed256=(short)(t256(0.4)); break;
			case '5':  speed256=(short)(t256(0.5)); break;
			case '6':  speed256=(short)(t256(0.6)); break;
			case '7':  speed256=(short)(t256(0.7)); break;
			case '8':  speed256=(short)(t256(0.8)); break;
			case '9':  speed256=(short)(t256(0.9)); break;
			default:   speed256=(short)(t256(0.5));
			}
			new_entry->para3 = speed256;
			#endif
	//	const short xxx = new_entry->user_y/1000;
	//	const short yyy = new_entry->user_y%1000;
	//	new_entry->user_x = xxx;
	//	new_entry->user_y = yyy;
		#if 0
			 if (0==tiny_strcmp(new_entry->user_string,"BGSPEED"))	{	new_entry->user_command=BTYPE_05_BG_SPEED;	}
		else if (0==tiny_strcmp(new_entry->user_string,"MAGICF"))	{	new_entry->user_command=BTYPE_04_MAGIC_FORMATION;	}
		else if (0==tiny_strcmp(new_entry->user_string,"GROUNDER")) {	new_entry->user_command=BTYPE_03_GROUNDER;			}
		else if (0==tiny_strcmp(new_entry->user_string,"BGPANEL2")) {	new_entry->user_command=BTYPE_02_BGPANEL2;	}
		else if (0==tiny_strcmp(new_entry->user_string,"BGPANEL"))	{	new_entry->user_command=BTYPE_01_BGPANEL1;	}
		else														{	new_entry->user_command=BTYPE_00_NONE;
				error(ERR_WARN, "unknown user_command '%c' in levelfile",new_entry->user_command);}
		#endif

/*---------------------------------------------------------
	�q�֐�
	Get ascii strings.
---------------------------------------------------------*/

static char *load_stage_get_str(char *c, char *buffer)
{
	int i = 0;
	while ('|' != (*c))
	{
		i++;
		if (i >= 128)
		{	goto ne111;}
		*buffer++ = *c++;
	}
	*buffer = 0;
	return (c);
ne111:
	return ((char *) NULL);
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

/*---------------------------------------------------------
	�ėp���̂܂�łȂ��ǂݍ��݊֘A�B�q�֐�
	-------------------------------------------------------
	�W�����o�͂�psp�ł͒x������̂�sce�̊֐����g���B(�Q�[�����̏��������y����)
	�G���[�`�F�b�N�Ƃ��S�R�����̂Œ��ӂ̎�
---------------------------------------------------------*/

//atic char buffer_text_1_line[128];	/* parth text, 1 line buffer */ 	/* ��������s�̎擾 */
static char buffer_text_1_line[256];	/* parth text, 1 line buffer */ 	/* ��������s�̎擾 */

#if 1
static unsigned long file_size;
static unsigned long file_seek;
//static char *my_buf;
//static void *malloc_buf;
static char *malloc_buf;

static void *my_fopen(const char *file_name/*, const char *dummy*/)
{
	SceUID fd;
	if (!(fd = sceIoOpen((char *)file_name, PSP_O_RDONLY, 0777)))
	{
		goto error111;
	}
	file_size = sceIoLseek32(fd, 0, PSP_SEEK_END);
	file_seek = 0;

	malloc_buf = malloc(file_size);
	if (NULL == malloc_buf)
	{
		sceIoClose(fd);
		goto error111;
	}
	sceIoLseek32(fd, 0, PSP_SEEK_SET);
	sceIoRead( fd, malloc_buf, file_size);
	sceIoClose(fd);
//	my_buf = malloc_buf;
//
	#if 0
	char strbuf[128];
	strbuf[25]='0';
	{	int i;
		for (i=0;i<20;i++){ strbuf[i]=my_buf[i];}
	}
	sp rintf(strbuf,
	"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n"
	"%x %x %x %x %x %x %x %x %x %x \n"
	"%x %x %x %x %x %x %x %x %x %x \n"
	,my_buf[ 0] ,my_buf[ 1] ,my_buf[ 2] ,my_buf[ 3] ,my_buf[ 4] ,my_buf[ 5] ,my_buf[ 6] ,my_buf[ 7]
	,my_buf[ 8] ,my_buf[ 9] ,my_buf[10] ,my_buf[11] ,my_buf[12] ,my_buf[13] ,my_buf[14] ,my_buf[15]
	,my_buf[16] ,my_buf[17] ,my_buf[18] ,my_buf[19] ,my_buf[ 0] ,my_buf[ 1] ,my_buf[ 2] ,my_buf[ 3]
	,my_buf[ 4] ,my_buf[ 5] ,my_buf[ 6] ,my_buf[ 7] ,my_buf[ 8] ,my_buf[ 9] ,my_buf[10] ,my_buf[11]
	,my_buf[12] ,my_buf[13] ,my_buf[14] ,my_buf[15] ,my_buf[16] ,my_buf[17] ,my_buf[18] ,my_buf[19]
	);
	error(ERR_FATAL, "AAA %s\nno: %d (%s)",strbuf, errno, strerror(errno));
// # kene -stage1
// #
// 23 20 6b ... 65 5f 31 0x0d 0x0a
// 23 0x0d 0x0a
	#endif
	return (malloc_buf);
error111:
	return (NULL);
}
static int my_fgets(void/*char *buffer_name, int num, char *wfp*/)
{
	int ii;
ii=0;
	char aaa;
//	char bbb;
//bbb=0;
	fgets_loop:;
	aaa = /*buffer_name*/buffer_text_1_line[ii] = /*my_buf*/malloc_buf[file_seek]/*(*my_buf)*/;
//	my_buf++;
	ii++;
	file_seek++;
	if (0x0a==aaa)	return (1);
	if (file_size < file_seek)	return (0)/*NULL*/;
	goto fgets_loop;
//	error(ERR_FATAL, "TEST %s\nno: %d (%s)",buffer_name,errno,strerror(errno));
//	return (NULL);
}
static void my_fclose(void/*void *wfp*/)
{
	free(malloc_buf);
}
#else
	#define my_fopen	fopen
	#define my_fgets	fgets
	#define my_fclose	fclose
#endif
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
extern int select_player;
extern void bg2_start_stage(void); // [***090209		�ǉ�
extern void stage_bg_load_surface(void);
void load_stage(void/*int level*/)		/* ���Xint */
{
	bg2_start_stage();	// [***090209		�ǉ�
	enemy_set_random_seed(/*set_seed*/);
//
	player_now_stage++; /*(*level)++*/;
//	int level = player_now_stage;
//
	// change music soundtrack
	play_music( /*level*/player_now_stage/*(*level)*/ );
//
	load_stage_free_entry();
//
	int load_stage_number = player_now_stage;
	{	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
		/* �H�X�q ����\�́F�X�e�[�W�N���A���Ƀ{���������� */
		if (YUYUKO==select_player)	/* �H�X�q�̏ꍇ */
		{
			#if 1/*���앗*/
			if (3 > pd->bombs)	/* �N���A�[���Ƀ{�����R�����Ȃ� */
			{	pd->bombs = 3;	}	/* �R�ɑ��₷ */
			#endif
			#if 1/*�͕핗*/
			if (9 > pd->bombs)	/* �N���A�[���Ƀ{�����X�����Ȃ� */
			{	pd->bombs++;	}	/* �P���₷ */
			#endif
			/* �{�����Ȃ��Ă��N���A�[����΃{�����S�ɂȂ� */
		}
		//
		#if (1==USE_ENDING_DEBUG)
		if (MAX_STAGE6_FOR_CHECK == player_now_stage/*continue_stage*/)
		{
		//	if (B07_AFTER_LOAD==pd->bo ssmode)
			if ((STATE_FLAG_10_IS_LOAD_SCRIPT|STATE_FLAG_11_IS_BOSS_DESTROY)==(pd->state_flag&(STATE_FLAG_10_IS_LOAD_SCRIPT|STATE_FLAG_11_IS_BOSS_DESTROY)))
			{
				load_stage_number=9;/*�G���f�B���O�f�o�b�O�p*/
			}
		}
		if (9!=load_stage_number)
		#endif //(1==USE_ENDING_DEBUG)
		{
			pd->state_flag &= (~(STATE_FLAG_05_IS_BOSS));/*�{�Xoff*/
		}
		draw_side_panel=1/*pd->state_flag |= ST ATE_FLAG_09_IS_PANEL_WINDOW*/;/* �p�l���\��on */
	}
//
	stage_bg_load_surface();
//
	char filename[128];
//	sp rintf(filename,"%s/dat/level%02d.dat", data_dir, /*level*/player_now_stage);
//	sp rintf(filename,"%s/dat/stage%01d.txt", data_dir, /*level*/player_now_stage);
//	sp rintf(filename, "%s/dat/stage%c.txt", data_dir, ('0'+/*level*/load_stage_number/*player_now_stage*/) );
//	sp rintf(filename, DIRECTRY_NAME_DATA "/dat/stage%c.txt", ('0'+/*level*/load_stage_number/*player_now_stage*/) );
	strcpy(filename, DIRECTRY_NAME_DATA "/dat/stageZ.txt");
	filename[10+DIRECTRY_NAME_LENGTH] = ('0'+/*level*/load_stage_number/*player_now_stage*/);

//	/*FILE*/char *fp;
	if (NULL==(/*fp=*/my_fopen(filename/*,"r"*/)))
	{
		error(ERR_FATAL, "can't read stage data %s\nerrno: %d (%s)",filename,errno,strerror(errno));
	}
//
	int entrys		= 0;		/* �L���s���̒��� */
	int line_num	= 0;		/* �t�@�C���̎��s�� */
	{loop:;
		if (/*NULL*/0 != my_fgets(/*buffer_text_1_line,128,fp*/))
		{
			int time10; 				/* �o������(1/10�b�P��)  */
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
			c = load_stage_get_int(c, &time10); 	if (NULL == c)		{	GOTO_ERR_WARN;/*continue;*/;	}	/* load int time10 */		/* ���Ԃ̎擾 */
													if ('|' != *c++)	{	GOTO_ERR_WARN;/*continue;*/;	}	/* load '|' */
			char_user_command = *c++;																			/* load 1 char commnd */	/* �P�����R�}���h */	/* Befehl */
													if (*c++ != '|')	{	GOTO_ERR_WARN;/*continue;*/;	}	/* load '|' */
			c = load_stage_get_str(c, user_string); if (NULL == c)		{	GOTO_ERR_WARN;/*continue;*/;	}	/* load str user_string */
													if (*c++ != '|')	{	GOTO_ERR_WARN;/*continue;*/;	}	/* load '|' */
			c = load_stage_get_int(c, &user_x); 	if (NULL == c)		{	GOTO_ERR_WARN;/*continue;*/;	}	/* load int user_x */
													if (*c++ != '|')	{	GOTO_ERR_WARN;/*continue;*/;	}	/* load '|' */
			c = load_stage_get_int(c, &user_y); 	if (NULL == c)		{	GOTO_ERR_WARN;/*continue;*/;	}	/* load int user_y */
			/* do set register entry. */
			#define MUSIC_CONVERT_TIME (10)
			/* �ǉ��o�^���� */
			load_stage_add_entry(MUSIC_CONVERT_TIME+time10, char_user_command, user_string, user_x, user_y);
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
			error(ERR_WARN, "syntax error in stage data '%s', line no: %d", filename, line_num);
			goto loop;
		#endif
		}
	}
	my_fclose (/*fp*/);
	//return (entrys);
	if (0==entrys)		/* �L���s�����Ȃ���΃G���[ */
	{
		error(ERR_WARN, "no entrys for STAGE%d.TXT",/*level*/player_now_stage);
	}
	//fps_init();/* ??? auto fps������ */
}
