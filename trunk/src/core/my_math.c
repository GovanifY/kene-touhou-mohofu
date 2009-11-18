
/*---------------------------------------------------------
	�w�b�_
---------------------------------------------------------*/

#include "game_main.h"


/*---------------------------------------------------------
	math.h
---------------------------------------------------------*/

/* ���̔z���CPU�����̖��߃L���b�V���ɏ��(�Ƃ�����)��B */
/*extern*/ int sin_tbl512[SINTABLE_SIZE];


#if 0
/* CW�̏ꍇ */
#else
/* CCW�̏ꍇ */
	/*�Ƃ肠����*/
	#define rad2deg512(x)		( (int)((x)*((512.0)/(T_GU_M_PI*2))/*+512*/)&(512-1) )
#endif
/* ���̊֐���CPU�����̖��߃L���b�V���ɏ���(�Ă��B�Ƃ���������Ă�͂�)�B */
int atan_512( int y, int x )
{
	#if 0
/* CW�̏ꍇ */
//	return (rad2deg512(atan2(y,x)));	/* �ECW�̏ꍇ�B  ��ʓI�ȉECW �̏ꍇ�B */
	#else
/* CCW�̏ꍇ */
	return (rad2deg512(atan2(x,y)));	/* ��CCW�̏ꍇ�B (x y �����ւ��đΏ����Ă���܂��B) */
	#endif
}

/*
psp �ł́A atan2(), sin(), sqrt() ���̒��z�֐��n���߂́A
psp �� MIPS CPU �� �̃R�v���Z�b�T������������B
�R�v���Z�b�T�ϊ����������������ɂ���ƁA���Ƀp�t�H�[�}���X�������̂ŁA
�ꃖ���ɓZ�߂��������s���x�͗y���ɑ����B
(CPU�����̖��߃L���b�V���ɏ�邽�߂Ɏ��s���x�������Ȃ�)

�Q�l:TECH I Vol.39 MIPS�v���Z�b�T����	http://www.cqpub.co.jp/interface/TechI/Vol39/
*/

/*---------------------------------------------------------
	�����I�Ƀ��v���C�ɑΉ��ł���悤�ɁA
	���O�̗������[�`����p�ӂ��Ƃ��B
	(���Ȃ�e�L�g�[�ł���)
---------------------------------------------------------*/

static int rnd;
int ra_nd(void)
{
	rnd = (rnd * 8513/*multiplier*/) + 179/*addend*/;
	rnd = ((rnd) ^ (rnd>>8));
	return (rnd);
}


/*---------------------------------------------------------
	���������l�ݒ�(�����I�Ƀ��v���C�ɑΉ��ł���悤�ɁA
	�ʂ̎n�߂ɌŒ�l��ݒ肷��)
---------------------------------------------------------*/

void set_rnd_seed(int set_seed)
{
	rnd = set_seed;
}


/*---------------------------------------------------------
	���̊֐��͂����̏������B���߃L���b�V���ɏ��Ȃ�(�͂���)��B
---------------------------------------------------------*/

void init_math(void)
{
	unsigned int i;
	for (i=0; i<SINTABLE_SIZE; i++)
	{
	//	sin_tbl512[i] = (int)(sinf( (i*(GU_PI*2) / SINTABLE_SIZE) )*256/**65536.0*/ );
		sin_tbl512[i] = (int)(sin( (i*(GU_PI*2) / SINTABLE_SIZE) )*256/**65536.0*/ );
	}
}


/*---------------------------------------------------------
	�L�[���͊֘A�̏���(�ėp)
---------------------------------------------------------*/

/*extern*/ int pad_config[KEY_NUM12_MAX];


extern void save_screen_shot(void);
/*global*/Uint32 my_pad;		/*�������*/
/*global*/Uint32 my_pad_alter;	/*�O�����*/
/*global*/short my_analog_x;	/* �A�i���O�ʁA�␳�ς� */
/*global*/short my_analog_y;	/* �A�i���O�ʁA�␳�ς� */

/*static*/ void do_input_vbl(void)
{
	SceCtrlData pad;
	sceCtrlReadBufferPositive(&pad, 1);
	#if (1==USE_KEY_CONFIG)
		int
	#else
		my_pad_alter = my_pad;
		#define pad_data my_pad
	#endif /* (1==USE_KEY_CONFIG) */
	pad_data = pad.Buttons;
	my_analog_x = 0;
	my_analog_y = 0;
	/* �W���A�i���O�L�[�@�\ */
//	if (1==use_analog)
	{
		/* PSP�̃A�i���O���͂̓f�W�^�����͂֕ϊ��A�A�i���O�ʂ͒��S���폜��256�Œ菬���_�`���֕␳ */
		if (pad.Lx < 64/*70*/)			{	pad_data |= PSP_CTRL_LEFT;		my_analog_x = ((64-pad.Lx)<<2); 	}
		else if (pad.Lx > 192/*185*/)	{	pad_data |= PSP_CTRL_RIGHT; 	my_analog_x = ((pad.Lx-192)<<2);	}
		//
		if (pad.Ly < 64/*70*/)			{	pad_data |= PSP_CTRL_UP;		my_analog_y = ((64-pad.Ly)<<2); 	}
		else if (pad.Ly > 192/*185*/)	{	pad_data |= PSP_CTRL_DOWN;		my_analog_y = ((pad.Ly-192)<<2);	}
	}
	#if (1==USE_KEY_CONFIG)
	my_pad_alter = my_pad;
	my_pad = 0;
	/* PSP�̃f�W�^�����͂��L�[�R���t�B�O�̌`���ɕϊ� */
	if (pad_data & PSP_CTRL_SELECT) 	{	my_pad |= pad_config[KEY_NUM00_SELECT]; 	}	//	if (keyboard[KINOU_01_SELECT])		{my_pad |= (PSP_KEY_SELECT);}
	if (pad_data & PSP_CTRL_START)		{	my_pad |= pad_config[KEY_NUM01_START];		}	//	if (keyboard[KINOU_02_PAUSE])		{my_pad |= (PSP_KEY_PAUSE);}
//
	if (pad_data & PSP_CTRL_LTRIGGER)	{	my_pad |= pad_config[KEY_NUM06_L_TRIG]; 	}	//	if (keyboard[KINOU_07_SNAP_SHOT])	{my_pad |= (PSP_KEY_SNAP_SHOT);}
	if (pad_data & PSP_CTRL_RTRIGGER)	{	my_pad |= pad_config[KEY_NUM07_R_TRIG]; 	}	//	if (keyboard[KINOU_08_SYSTEM])		{my_pad |= (PSP_KEY_SYSTEM);}
	if (pad_data & PSP_CTRL_TRIANGLE)	{	my_pad |= pad_config[KEY_NUM08_TRIANGLE];	}	//	if (keyboard[KINOU_09_SLOW])		{my_pad |= (PSP_KEY_SLOW);}
	if (pad_data & PSP_CTRL_CIRCLE) 	{	my_pad |= pad_config[KEY_NUM09_CIRCLE]; 	}	//	if (keyboard[KINOU_10_OPTION])		{my_pad |= (PSP_KEY_OPTION);}
	if (pad_data & PSP_CTRL_CROSS)		{	my_pad |= pad_config[KEY_NUM10_CROSS];		}	//	if (keyboard[KINOU_11_SHOT])		{my_pad |= (PSP_KEY_SHOT_OK);}
	if (pad_data & PSP_CTRL_SQUARE) 	{	my_pad |= pad_config[KEY_NUM11_SQUARE]; 	}	//	if (keyboard[KINOU_12_BOMB])		{my_pad |= (PSP_KEY_BOMB_CANCEL);}

	/* �L�[�R���t�B�O�̌`���ł́A�Q�[�������x�I�ɖ����Ȃ̂ŁAPSP�̃f�W�^�����͂Ɏ��������ɍĂѕϊ� */

	my_pad |= (pad_data & (PSP_CTRL_UP|PSP_CTRL_RIGHT|PSP_KEY_DOWN|PSP_CTRL_LEFT));

	#endif /* (1==USE_KEY_CONFIG) */
	/* �X�N���[���V���b�g�@�\�B */
	// keypoll�ɓ����Ɖ��̂����܂������Ȃ������̂ł������ɏꏊ��ύX�B
	if (/*keyboard[KINOU_07_SNAP_SHOT]*/my_pad & PSP_KEY_SNAP_SHOT) { 	save_screen_shot(); }

	/* �A�i���O�T�|�[�g�@�\ */
//	if (1==use_analog)
	{
		/* �f�W�^�����A�i���O�D�� */
		if (0 == (my_analog_x+my_analog_y) )
		/*�A�i���O�����ĂȂ��Ǝv����ꍇ(�A�i���O�����Ă�ꍇ�̓A�i���O�ʂ����̂܂܎g��)*/
		{
			/* �f�W�^�����A�i���O�ʂ��Z�o */
			#if (0)
				 if (pad_data & PSP_CTRL_UP)		{	my_analog_y =	256;	}
			else if (pad_data & PSP_CTRL_DOWN)		{	my_analog_y =	256;	}
				 if (pad_data & PSP_CTRL_RIGHT) 	{	my_analog_x =	256;	}
			else if (pad_data & PSP_CTRL_LEFT)		{	my_analog_x =	256;	}
			#else
			if (pad_data & (PSP_CTRL_UP|PSP_CTRL_DOWN)) 	{	my_analog_y =	256;	}	/* �㉺�̃A�i���O�� */
			if (pad_data & (PSP_CTRL_RIGHT|PSP_CTRL_LEFT))	{	my_analog_x =	256;	}	/* ���E�̃A�i���O�� */
			#endif
		}
	}
}
//		 if (keyboard[KINOU_06_LEFT])	{my_pad |= PSP_CTRL_LEFT;	/*direction=-1;*/		}
//	else if (keyboard[KINOU_04_RIGHT])	{my_pad |= PSP_CTRL_RIGHT;	/*direction=1;*/		}
//		 if (keyboard[KINOU_03_UP]) 	{my_pad |= PSP_CTRL_UP; 	}
//	else if (keyboard[KINOU_05_DOWN])	{my_pad |= PSP_CTRL_DOWN;	}

//int keyboard_keypressed()
//{
//	int i;
//	for (i=0;i<15;i++)
//	{
//		if (keyboard[i]) return (1);
//	}
//	return (0);
//}

/*---------------------------------------------------------
	�L�[���͊֘A�̏���(�{���f�o�b�O�p)
	��̃L�[���͂ɓ������Ă��ǂ����A�L�[���͎��̂�
	���������Ȃ�ꍇ������̂ŁA�b��I�ɍŒ���̓��͂Ƃ���
	�c���Ă���B
---------------------------------------------------------*/

void hit_any_key(void)
{
	SceCtrlData cpad;
	/* �������܂ő҂� */
	while (1)
	{
		sceCtrlReadBufferPositive(&cpad, 1);
		if (0 == cpad.Buttons)
		{
			goto l_end1;
		//	break;
		}
	}
	l_end1:
	;
	/* �������܂ő҂� */
	while (1)
	{
		sceCtrlReadBufferPositive(&cpad, 1);
		/*Any Key*/
		if (cpad.Buttons & (PSP_CTRL_SQUARE|PSP_CTRL_CROSS|PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE) )
		{
			goto l_end2;
		//	break;
		}
	}
	l_end2:
	;
}

/*---------------------------------------------------------
	�G���[����
---------------------------------------------------------*/
void error(int errorlevel, char *msg, ...)
{
	char msgbuf[128];
	va_list argptr;

	va_start(argptr, msg);
	vsprintf(msgbuf, msg, argptr);
	va_end(argptr);

	switch (errorlevel)
	{
	//case ERR_DEBUG:	if (debug) { fprintf(stdout,"DEBUG: %s\n",msgbuf); } break;
	//case ERR_INFO:		fprintf(stdout,"INFO: %s\n",msgbuf); break;

	#if 0
	/*�f�o�b�O�p*/
	case ERR_WARN:	//fprintf(stdout,"WARNING: %s\n",msgbuf);
		pspDebugScreenSetXY(2,3);
		pspDebugScreenPrintf("WARNING");
		hit_any_key();
		{
		char msgbuf2[32/*128*/];
			int j;
			for (j=0;j<5;j++)
			{
				pspDebugScreenSetXY(2,5+j);
				strncpy(msgbuf2, &msgbuf[j*24], 24);	/*24�������\��*/
				/* [0] ... [23] ��24���� */
				msgbuf2[24] = 0;/* strncpy�� '\0' ����Ă���Ȃ��݂����Ȃ̂ŁA��؂������ */
				msgbuf2[25] = 0;/* strncpy�� '\0' ����Ă���Ȃ��݂����Ȃ̂ŁA��؂������ */
				msgbuf2[26] = 0;/* strncpy�� '\0' ����Ă���Ȃ��݂����Ȃ̂ŁA��؂������ */
				msgbuf2[27] = 0;/* strncpy�� '\0' ����Ă���Ȃ��݂����Ȃ̂ŁA��؂������ */
				pspDebugScreenPrintf("%s",	msgbuf2 );
			}
			hit_any_key();
		}
		break;
	#endif
	case ERR_FATAL: //	fprintf(stdout,"FATAL: %s\n",msgbuf);
		pspDebugScreenSetXY(2,3);
		pspDebugScreenPrintf("FATAL ERROR");
		hit_any_key();
		pspDebugScreenInit();/*�v��*/
		pspDebugScreenClear();
		pspDebugScreenSetXY(0,0);
		pspDebugScreenPrintf("%s",	msgbuf	);
		hit_any_key();
		sceKernelExitGame();	//if (errorlevel==ERR_FATAL) exit(1);/*exit(1)��psp�Ŏg���Ȃ��̂Œ���*/
		break;
	}
}




/*---------------------------------------------------------
	�V�X�e���̊�b����
---------------------------------------------------------*/

static void imglist_garbagecollect(void);
void *mmalloc(size_t size)
{
	void *ptr;
	ptr = malloc(size);
	if (NULL == ptr)
	{
		error(ERR_WARN, "can't alloc %d bytes, trying garbage collection",size);
		imglist_garbagecollect();
		ptr = malloc(size);
		if (NULL == ptr)
		{
			error(ERR_FATAL, "I'm sorry, but you're out of memory!");
		}
	}
	memset(ptr, 0, size);/* calloc()���ɁA�K��0�N���A�[����悤�ɕύX */
	return (ptr);
}


/*---------------------------------------------------------
	�摜�L���b�V���֘A

	�����摜�𕡐��ǂݍ��񂾏ꍇ�Ƀ����������ʂɂȂ���������Ȃ��B
	�����œ����摜��ǂݍ��񂾏ꍇ�ɂ́A���ۂɂ͓ǂݍ��܂Ȃ��ŁA
	�O�ɓǂݍ��񂾉摜�Ɠ������̂��g���B
	�g�[�^���ł��������摜��ǂݍ��񂾂��́A���ꂼ��̉摜�L���b�V���̎Q�Ɛ��ł킩��B
---------------------------------------------------------*/

typedef struct _imglist
{
	SDL_Surface *img;			/* �ǂݍ��񂾃T�[�t�F�C�X��ێ� */
	struct _imglist *next;		/* ���̉摜�ւ̃��X�g�|�C���^ */
	int refcount;				/* �����摜�̎Q�Ɛ� */
	char name[128/*256*/];		/* ���O */
} MY_IMAGE_LIST;

/* �摜�L���b�V���̃��X�g */
static MY_IMAGE_LIST *my_image_list /*= NULL*/;/*�����̏�����������psp�ł͐���ɓ��삵�Ȃ������H*/

/*extern*/ void init_imglist(void)
{
	my_image_list = NULL;
}

/*---------------------------------------------------------
	�摜�L���b�V���ɐV�K�摜��ǉ�
---------------------------------------------------------*/

static void imglist_add(SDL_Surface *s, char *name)
{
	MY_IMAGE_LIST *new_list;
	new_list			= mmalloc(sizeof(MY_IMAGE_LIST));
	strcpy(new_list->name,name);
	new_list->refcount	= 1;
	new_list->img		= s;
	if (NULL == my_image_list)
	{
		my_image_list	= new_list;
		new_list->next	= NULL;
	}
	else
	{
		new_list->next	= my_image_list;
		my_image_list	= new_list;
	}
}

/*---------------------------------------------------------
	�摜�L���b�V���ɓ����摜���Ȃ�������
---------------------------------------------------------*/
static SDL_Surface *imglist_search(char *name)
{
	MY_IMAGE_LIST *i = my_image_list;
	while (NULL != i)
	{
		if (0 == tiny_strcmp(name, i->name))
		{
			i->refcount++;
			return (i->img);
		}
		i = i->next;
	}
	return (NULL);
}

/*---------------------------------------------------------
	�摜�L���b�V�����J��
	������������Ȃ��Ȃ����̂ŉ摜�L���b�V�����Ŏg���ĂȂ����̂��J��
---------------------------------------------------------*/

static void imglist_garbagecollect(void)
{
	MY_IMAGE_LIST *s = my_image_list;
	MY_IMAGE_LIST *p = NULL;
	MY_IMAGE_LIST *n = NULL;
	while (NULL != s)
	{
		n = s->next;
		if (s->refcount == 0)
		{
			if (p == NULL)
			{
				my_image_list = n;
			}
			else
			{
				p->next = n;
			}
		//	SDL_FreeSurface(s->img);
			free (s);
		}
		else
		{
			p = s;
		}
		s = n;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

SDL_Surface *loadbmp0(char *filename, int use_alpha, int use_chache)
{
	char fn[128/*64 50*/];
//	strcpy(fn, data_dir);	strcat(fn, "/");
	strcpy(fn, DIRECTRY_NAME_DATA "/");
	strcat(fn, filename);
//
	SDL_Surface *s1;
	SDL_Surface *s2;
//
	s1 = imglist_search(fn);
	if ( NULL != s1 )
	{
		return (s1);
	}
	//if ((s1=SDL_LoadBMP(fn))==NULL)
//
	s1 = IMG_Load(fn);
	if ( NULL == s1 )
	{
		CHECKPOINT;
		error(ERR_FATAL, "load0:cant load image %s:\n"/*" %s"*/,fn/*,SDL_GetError()*/);
	}
	if (use_alpha)
	{
		s2 = SDL_DisplayFormatAlpha(s1);	// ���l��������surface
	}
	else
	{
		s2 = SDL_DisplayFormat(s1);
	}
	if ( NULL == s2 )
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant convert image %s to display format:"/*" %s"*/,fn/*,SDL_GetError()*/);
	}
	SDL_FreeSurface(s1);
	s1 = NULL;
	imglist_add(s2,fn);
	return (s2);
}
//SDL_Surface *loadbmp1(char *filename/*, int use_alpha*/)
//{
//	return	(loadbmp0(filename, 0, 1));
//}
//SDL_Surface *loadbmp2(char *filename/*, int use_alpha*/)
//{
//	return	(loadbmp0(filename, 1, 1));
//}

/*---------------------------------------------------------
	*s��MY_IMAGE_LIST���ɂ��邩�m�F�����A
	�摜�����������ꍇ�A�}�`�L���b�V�����X�g�̎Q�Ɛ�������炷�B
---------------------------------------------------------*/

void unloadbmp_by_surface(SDL_Surface *s)
{
	MY_IMAGE_LIST *i = my_image_list;/* �}�`�L���b�V�����X�g�̐擪 */
	while (NULL != i)
	{
		if (s == i->img)	/* �摜���������� */
		{
			if (0 == i->refcount)	/* �}�`�L���b�V�����X�g�̎Q�Ɛ� */
			{
				/* ���[�h���ĂȂ��̂ɊJ���B */
				CHECKPOINT;
				error(ERR_WARN, "unloadbmp_by_surface: refcount for object %s is already zero",i->name);
			}
			else
			{
				i->refcount--;	/* ����炷�B */
			}
			return; 	/* ����I�� */
		}
		i = i->next;
	}
	/* ������Ȃ��B */
	CHECKPOINT;
	error(ERR_WARN, "unloadbmp_by_surface: object not found");
//	return; 	/* �ُ�I�� */
}

