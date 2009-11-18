
/*
�t�@�C���֘A�̃T�|�[�g���[�`���������ɏ����ƒx���Ȃ�̂�
�����ɂ͒ǉ����Ȃ��ŉ������B
�t�@�C���֘A�̃T�|�[�g���[�`���́A ini_file.c �ɋL�q���Ă��������B

�t�@�C���֘A�̃T�|�[�g���[�`���F
	�ǉ����W���[��(MOD)�t�@�C���ǂݍ���
	�n�C�X�R�A�t�@�C���ǂݍ��݁^�ۑ�
	�ݒ�ǂݍ��݁^�ۑ�
	��ʕۑ�
*/

/*---------------------------------------------------------
	�w�b�_
---------------------------------------------------------*/

#include "game_main.h"


/*---------------------------------------------------------
	�萔
---------------------------------------------------------*/


/*---------------------------------------------------------
	�O���[�o���ϐ�
---------------------------------------------------------*/
// /*extern*/SDL_Surface *screen;
// SDL_Surface *back_screen;
// SDL_Surface *tex_screen;

SDL_Surface *sdl_screen[4];


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
	��Փx�X�R�A�␳
---------------------------------------------------------*/

int adjust_score_by_difficulty(int convert_score)
{
	int mul_tbl256[4] = /* ��Փx�␳ */
	{
	/* easy */		t256(0.5),
	/* normal */	t256(1.0),
	/* hard */		t256(1.2),
	/* lunatic */	t256(1.5),
	};
	return (((convert_score)*(mul_tbl256[difficulty]))>>8);
}

/*---------------------------------------------------------
	�L�[���͊֘A�̏���(�ėp)
---------------------------------------------------------*/

/*extern*/ int keyconfig[KEY_NUM12_MAX];
static int keyboard[16/*15*/];
	#if 1
static void keyboard_clear(void)
{
	int i;
	for (i=0;i<15;i++)
	{
		keyboard[i] = 0;
	}
}
/* ���̊֐��͂����̏������B���߃L���b�V���ɏ��Ȃ�(�͂���)��B */
void init_math(void)
{
	unsigned int i;
	for (i=0; i<SINTABLE_SIZE; i++)
	{
	//	sin_tbl512[i] = (int)(sinf( (i*(GU_PI*2) / SINTABLE_SIZE) )*256/**65536.0*/ );
		sin_tbl512[i] = (int)(sin( (i*(GU_PI*2) / SINTABLE_SIZE) )*256/**65536.0*/ );
	}
	keyboard_clear();
}
	#endif
extern void save_screen_shot(void);
/*global*/Uint32 my_pad;		/*�������*/
/*global*/Uint32 my_pad_alter;	/*�O�����*/
/*global*/short my_analog_x;	/* �A�i���O�ʁA�␳�ς� */
/*global*/short my_analog_y;	/* �A�i���O�ʁA�␳�ς� */

static void do_input_vbl(void)
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
		if (pad.Lx < 64/*70*/)			{		pad_data |= PSP_CTRL_LEFT;		my_analog_x =  ((64-pad.Lx)<<2);	}
		else if (pad.Lx > 192/*185*/)	{		pad_data |= PSP_CTRL_RIGHT; 	my_analog_x =  ((pad.Lx-192)<<2);	}
		//
		if (pad.Ly < 64/*70*/)			{		pad_data |= PSP_CTRL_UP;		my_analog_y =  ((64-pad.Ly)<<2);	}
		else if (pad.Ly > 192/*185*/)	{		pad_data |= PSP_CTRL_DOWN;		my_analog_y =  ((pad.Ly-192)<<2);	}
	}
	#if (1==USE_KEY_CONFIG)
	/* PSP�̃f�W�^�����͂��L�[�R���t�B�O�̌`���ɕϊ� */
	if (pad_data & PSP_CTRL_SELECT) 	{keyboard[keyconfig[KEY_NUM00_SELECT]]		|= (pad_data & PSP_CTRL_SELECT);}	else	{keyboard[keyconfig[KEY_NUM00_SELECT]]		&= (~PSP_CTRL_SELECT);}
	if (pad_data & PSP_CTRL_START)		{keyboard[keyconfig[KEY_NUM01_START]]		|= (pad_data & PSP_CTRL_START);}	else	{keyboard[keyconfig[KEY_NUM01_START]]		&= (~PSP_CTRL_START);}

	#if (1==USE_KEY_CONFIG_ALLOW)
	if (pad_data & PSP_CTRL_UP) 		{keyboard[keyconfig[KEY_NUM02_UP]]			|= (pad_data & PSP_CTRL_UP);}		else	{keyboard[keyconfig[KEY_NUM02_UP]]			&= (~PSP_CTRL_UP);}
	if (pad_data & PSP_CTRL_RIGHT)		{keyboard[keyconfig[KEY_NUM03_RIGHT]]		|= (pad_data & PSP_CTRL_RIGHT);}	else	{keyboard[keyconfig[KEY_NUM03_RIGHT]]		&= (~PSP_CTRL_RIGHT);}
	if (pad_data & PSP_CTRL_DOWN)		{keyboard[keyconfig[KEY_NUM04_DOWN]]		|= (pad_data & PSP_CTRL_DOWN);} 	else	{keyboard[keyconfig[KEY_NUM04_DOWN]]		&= (~PSP_CTRL_DOWN);}
	if (pad_data & PSP_CTRL_LEFT)		{keyboard[keyconfig[KEY_NUM05_LEFT]]		|= (pad_data & PSP_CTRL_LEFT);} 	else	{keyboard[keyconfig[KEY_NUM05_LEFT]]		&= (~PSP_CTRL_LEFT);}
	#endif /* (1==USE_KEY_CONFIG_ALLOW) */

	if (pad_data & PSP_CTRL_LTRIGGER)	{keyboard[keyconfig[KEY_NUM06_L_TRIG]]		|= (pad_data & PSP_CTRL_LTRIGGER);} else	{keyboard[keyconfig[KEY_NUM06_L_TRIG]]		&= (~PSP_CTRL_LTRIGGER);}
	if (pad_data & PSP_CTRL_RTRIGGER)	{keyboard[keyconfig[KEY_NUM07_R_TRIG]]		|= (pad_data & PSP_CTRL_RTRIGGER);} else	{keyboard[keyconfig[KEY_NUM07_R_TRIG]]		&= (~PSP_CTRL_RTRIGGER);}
	if (pad_data & PSP_CTRL_TRIANGLE)	{keyboard[keyconfig[KEY_NUM08_TRIANGLE]]	|= (pad_data & PSP_CTRL_TRIANGLE);} else	{keyboard[keyconfig[KEY_NUM08_TRIANGLE]]	&= (~PSP_CTRL_TRIANGLE);}
	if (pad_data & PSP_CTRL_CIRCLE) 	{keyboard[keyconfig[KEY_NUM09_CIRCLE]]		|= (pad_data & PSP_CTRL_CIRCLE);}	else	{keyboard[keyconfig[KEY_NUM09_CIRCLE]]		&= (~PSP_CTRL_CIRCLE);}
	if (pad_data & PSP_CTRL_CROSS)		{keyboard[keyconfig[KEY_NUM10_CROSS]]		|= (pad_data & PSP_CTRL_CROSS);}	else	{keyboard[keyconfig[KEY_NUM10_CROSS]]		&= (~PSP_CTRL_CROSS);}
	if (pad_data & PSP_CTRL_SQUARE) 	{keyboard[keyconfig[KEY_NUM11_SQUARE]]		|= (pad_data & PSP_CTRL_SQUARE);}	else	{keyboard[keyconfig[KEY_NUM11_SQUARE]]		&= (~PSP_CTRL_SQUARE);}
	/* �L�[�R���t�B�O�̌`���ł́A�Q�[�������x�I�ɖ����Ȃ̂ŁAPSP�̃f�W�^�����͂Ɏ��������ɍĂѕϊ� */
	my_pad_alter = my_pad;
	my_pad = 0;
//	if (keyboard[KINOU_01_SELECT])		{my_pad |= (PSP_KEY_SELECT);}
	if (keyboard[KINOU_02_PAUSE])		{my_pad |= (PSP_KEY_PAUSE);}
//
	#if (1==USE_KEY_CONFIG_ALLOW)
		 if (keyboard[KINOU_03_UP]) 	{my_pad |= (PSP_KEY_UP);}
	else if (keyboard[KINOU_05_DOWN])	{my_pad |= (PSP_KEY_DOWN);}
		 if (keyboard[KINOU_04_RIGHT])	{my_pad |= (PSP_KEY_RIGHT);}
	else if (keyboard[KINOU_06_LEFT])	{my_pad |= (PSP_KEY_LEFT);}
	#else
//		 if (pad_data & PSP_CTRL_UP)	{my_pad |= (PSP_KEY_UP);}
//	else if (pad_data & PSP_CTRL_DOWN)	{my_pad |= (PSP_KEY_DOWN);}
//		 if (pad_data & PSP_CTRL_RIGHT) {my_pad |= (PSP_KEY_RIGHT);}
//	else if (pad_data & PSP_CTRL_LEFT)	{my_pad |= (PSP_KEY_LEFT);}
	my_pad |= (pad_data & (PSP_CTRL_UP|PSP_CTRL_RIGHT|PSP_KEY_DOWN|PSP_CTRL_LEFT));
	#endif /* (1==USE_KEY_CONFIG_ALLOW) */
//
//	if (keyboard[KINOU_07_SNAP_SHOT])	{my_pad |= (PSP_KEY_SC_SHOT);}/*�����ł����g��Ȃ��̂ŕK�v�Ȃ�*/
	if (keyboard[KINOU_08_SYSTEM])		{my_pad |= (PSP_KEY_SYSTEM);}
	if (keyboard[KINOU_09_SLOW])		{my_pad |= (PSP_KEY_SLOW);}
	if (keyboard[KINOU_10_OPTION])		{my_pad |= (PSP_KEY_OPTION);}
	if (keyboard[KINOU_11_SHOT])		{my_pad |= (PSP_KEY_SHOT_OK);}
	if (keyboard[KINOU_12_BOMB])		{my_pad |= (PSP_KEY_BOMB_CANCEL);}

	/* �X�N���[���V���b�g�@�\�B */
	// keypoll�ɓ����Ɖ��̂����܂������Ȃ������̂ł������ɏꏊ��ύX�B
	if (keyboard[KINOU_07_SNAP_SHOT]/*my_pad & PSP_KEY_SC_SHOT*/) { 	save_screen_shot(); }
	#else
	if (/*keyboard[KINOU_07_SNAP_SHOT]*/my_pad & PSP_KEY_SC_SHOT) { 	save_screen_shot(); }
	#endif /* (1==USE_KEY_CONFIG) */

	/* �A�i���O�T�|�[�g�@�\ */
//	if (1==use_analog)
	{
		/* �f�W�^�����A�i���O�D�� */
		if (0 == (my_analog_x+my_analog_y) )
		/*�A�i���O�����ĂȂ��Ǝv����ꍇ(�A�i���O�����Ă�ꍇ�̓A�i���O�ʂ����̂܂܎g��)*/
		{
			/* �f�W�^�����A�i���O�ʂ��Z�o */
				 if (pad_data & PSP_CTRL_UP)	{my_analog_y =	256;}
			else if (pad_data & PSP_CTRL_DOWN)	{my_analog_y =	256;}
				 if (pad_data & PSP_CTRL_RIGHT) {my_analog_x =	256;}
			else if (pad_data & PSP_CTRL_LEFT)	{my_analog_x =	256;}
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
	return (ptr);
}

/*---------------------------------------------------------
	�摜�֘A
---------------------------------------------------------*/

void psp_clear_screen(void)
{
	/* ����Gu�ŕ`�����ꍇ�B�n�[�h�E�F�A�[�@�\�ŁA�u����������̂ō��̂����ɂ܂Ƃ߂Ƃ� */
	SDL_FillRect(sdl_screen[SDL_00_SCREEN],NULL,SDL_MapRGB(sdl_screen[SDL_00_SCREEN]->format,0,0,0));
}
//void psp_move_screen(SDL_Surface *src_screen, SDL_Surface *dst_screen )
void psp_move_screen(int src_screen_number, int dst_screen_number )
{
	SDL_BlitSurface(sdl_screen[src_screen_number],NULL,sdl_screen[dst_screen_number],NULL);
}
//void psp_push_screen(void)
//{
//	SDL_BlitSurface(sdl_screen[SDL_00_SCREEN],NULL,sdl_screen[SDL_01_BACK_SCREEN],NULL);
//}
//void psp_pop_screen(void)
//{
//	SDL_BlitSurface(sdl_screen[SDL_01_BACK_SCREEN],NULL,sdl_screen[SDL_00_SCREEN],NULL);
//}
#if (1==USE_GU)
static UINT16 *render_image;

static UINT16 *render_image_back;
static UINT16 *render_image_tex;

/*static*/ UINT16 *msg_window_image;

//static UINT16 *bullet_image;
static SDL_Surface *SDL_VRAM_SCREEN;

#endif /* (1==USE_GU) */

void gu_save_screen(void)
{
	if (SDL_MUSTLOCK(sdl_screen[SDL_00_SCREEN]))	{	SDL_LockSurface(sdl_screen[SDL_00_SCREEN]); 	}	/* ���b�N */
	u16 *dest;	dest = render_image;
	u16 *src;	src = (UINT16 *)((UINT32) 0x44000000);/*VRAM*/
	#if 1
	{
		int ii;
		for (ii=0; ii<(512*272); ii++)
		{
			*dest = *src;
			dest++;
			src++;
		}
	}
	#else
	/* �_�� */
//	sceGuStart(GU_DIRECT, gulist );
//	sceGuCopyImage(
//		/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/,	//	int psm,	GU_PSM_8888 /* Image format */
//		0,				//	int sx, 		/* src location */
//		0,				//	int sy, 		/* src location */
//		512,			//	int width,		/* Image size */
//		272,			//	int height, 	/* Image size */
//		512,			//	int srcw,		/* src buffer width */
//		src,			//	void* src,		/* src Image from RAM */
//		0,				//	int dx, 		/* dest location */
//		0,				//	int dy, 		/* dest location */
//		512,			//	int destw,		/* dest buffer width */
//		dest			//	void* dest		/* dest Image to VRAM */
//	);
//	sceGuFinish();
//	sceGuSync(0, 0);
	#endif
	if (SDL_MUSTLOCK(sdl_screen[SDL_00_SCREEN]))	{	SDL_UnlockSurface(sdl_screen[SDL_00_SCREEN]);	}	/* ���b�N���� */
}

/*---------------------------------------------------------
	�摜�L���b�V���֘A

	�����摜�𕡐��ǂݍ��񂾏ꍇ�Ƀ����������ʂɂȂ���������Ȃ��B
	�����œ����摜��ǂݍ��񂾏ꍇ�ɂ́A���ۂɂ͓ǂݍ��܂Ȃ��ŁA
	�O�ɓǂݍ��񂾉摜�Ɠ������̂��g���B
	�g�[�^���ł��������摜��ǂݍ��񂾂��́A���ꂼ��̉摜�L���b�V���̎Q�Ɛ��ł킩��B
---------------------------------------------------------*/

/*
	MY_IMAGE_LIST�̍\��
	struct ImgList
	{
		char name[64];
		SDL_Surface* pSurface;
		ImgList* Pre;
		ImgList* Next;
	};
*/

typedef struct _imglist
{
	SDL_Surface *img;			/* �ǂݍ��񂾃T�[�t�F�C�X��ێ� */
	struct _imglist *next;		/* ���̉摜�ւ̃��X�g�|�C���^ */
	int refcount;				/* �����摜�̎Q�Ɛ� */
	char name[128/*256*/];		/* ���O */
} MY_IMAGE_LIST;

/*�摜�L���b�V���̃��X�g*/
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

extern int tiny_strcmp(char *aaa, const char *bbb);
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
		error(ERR_FATAL, "cant load image %s:\n"/*" %s"*/,fn/*,SDL_GetError()*/);
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



/*------------------------------------------------------------- */

#if (1==USE_GU)


//#define USE_16BIT_BLIT 0
#define USE_16BIT_BLIT 1

//#define USE_BLIT_COLOR_BIT 32
#define USE_BLIT_COLOR_BIT 16

/*	*/
//#if (1==USE_16BIT_BLIT)
//	#define USE_COLOR_16_AND_32 0
//#endif
	#define USE_COLOR_16_AND_32 1


/* �e�e�N�X�`���̕� */
#define TW128 128
/* �e�e�N�X�`���̍��� */
#define TH128 128

/* �e�e�N�X�`����`�悷��ꍇ�ɁA�`�惏�[�N�G���A�̕� */
#define WK512 512


/* �傫�ȃX�v���C�g��`�悷��Ƃ��̕����T�C�Y */
#define SLICE_SIZE	(64)

/* 0:���_�J���[�Ƃ��g��Ȃ��D */
#define USE_VCOLOR 1/*1*/

/* --- GU �`��̂��߂̃p�P�b�g�T�C�Y */
#define PACKET_SIZE  (262144)

/* gulist�ݒ肪����������Ɠs�������� */
#if (PACKET_SIZE<(512*512)) /* shere short16==(512*512/2*short) and int32 ==(512*512*int) */
	#undef	PACKET_SIZE
	#define PACKET_SIZE  (512*512)
#endif

static	unsigned int __attribute__((aligned(16))) gulist[PACKET_SIZE];

//#define UNKNOWN_MY_FLAGS (GU_COLOR_5650)

	#if (1==USE_VCOLOR)
		/* ���_�J���[���g���ꍇ�B��ʓI�B */
		#if (16==USE_BLIT_COLOR_BIT)
			/* 15/16bit�`�� */
//		//	#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
//		//	#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
//			#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
//
		//	#define TEXTURE_FLAGS4444		(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS5650		(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS5650_C32	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		//	#define TEXTURE_FLAGS4444		(GU_TEXTURE_16BIT | GU_COLOR_5551 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS4444		(GU_TEXTURE_16BIT | GU_COLOR_4444 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		#else
			/* 32bit�`�� */
			#define TEXTURE_FLAGS5650	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		#endif
	#else
		/* ���_�J���[���g��Ȃ��ꍇ�B�uGU_COLOR�v��t����Ɠ����Ȃ��B */
		//	#define TEXTURE_FLAGS		(GU_TEXTURE_16BIT | 				GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS		(									GU_VERTEX_16BIT | GU_TRANSFORM_2D)
	#endif







// /* Vertex Declarations Begin */
//#define GU_TEXTURE_SHIFT(n)	((n)<<0)
//#define GU_TEXTURE_8BIT		GU_TEXTURE_SHIFT(1) 	0x01
//#define GU_TEXTURE_16BIT		GU_TEXTURE_SHIFT(2) 	0x02
//#define GU_TEXTURE_32BITF 	GU_TEXTURE_SHIFT(3) 	0x03
//#define GU_TEXTURE_BITS		GU_TEXTURE_SHIFT(3) 	0x03

//	*	- GU_COLOR_5650 - 16-bit color (R5G6B5A0)
//	*	- GU_COLOR_5551 - 16-bit color (R5G5B5A1)
//	*	- GU_COLOR_4444 - 16-bit color (R4G4B4A4)
//	*	- GU_COLOR_8888 - 32-bit color (R8G8B8A8)

//#define GU_COLOR_SHIFT(n) ((n)<<2)
//#define GU_COLOR_RES1 	GU_COLOR_SHIFT(1)	0x04
//#define GU_COLOR_RES2 	GU_COLOR_SHIFT(2)	0x08
//#define GU_COLOR_RES3 	GU_COLOR_SHIFT(3)	0x0c
//#define GU_COLOR_5650 	GU_COLOR_SHIFT(4)	0x10
//#define GU_COLOR_5551 	GU_COLOR_SHIFT(5)	0x14
//#define GU_COLOR_4444 	GU_COLOR_SHIFT(6)	0x18
//#define GU_COLOR_8888 	GU_COLOR_SHIFT(7)	0x1c
//#define GU_COLOR_BITS 	GU_COLOR_SHIFT(7)	0x1c


//#define USE_SWIZZLE 0
#define USE_SWIZZLE 1


/* �Ǘ�����e�N�X�`���[ */
enum
{
	TEX_00_BACK_GROUND = 0, 	/* [256x256]3D�w�i1 */
//	TEX_01_BACK_TEXTURE,		/* �w�i��Q�� */
	TEX_02_MAHOUJIN,			/* [128x128]�����w */
	TEX_03_JIKI,				/* [256x256]���e/���@ */
//	TEX_04_TEKI,				/* �{�X/�U�R�G */
//	TEX_05_ITEM,				/* �A�C�e��/�����X�R�A */
	TEX_06_BULLET,				/* [128x128]�G�e */
	TEX_07_FRONT,				/* [256x256]���@������\��/����/[�R���e�B�j���[����(bank00)/���j���[����(bank01)/���j���[����(bank02)] */
//	TEX_08_SCORE_PANEL, 		/* [TEX_07_FRONT�ƗZ������\��������(256x512�e�N�X�`���H)] �X�R�A�p�l��/�X�R�A�t�H���g���� */
//	TEX_09_TACHIE,				/* �����G */
//	TEX_10_MESSAGE, 			/* [TEX_07_FRONT�ƗZ������\��������(512x512�e�N�X�`���H)]���b�Z�[�W�Œ蕶�� */
	TEXTURE_MAX 				/* --- �Ǘ�����ő�e�N�X�`���[�� */
};

/*---------------------------------------------------------
	�X�N���[���Ǘ�
---------------------------------------------------------*/

//#define USE_SDL_image 1
#define USE_SDL_image 0
#if (1==USE_SDL_image)
	//#include "_SDL_image.h"//#include "_SDL_image.h"
	#define MY_DIB_SURFACE	SDL_Surface
	#define MY_DIB_DATA 	pixels
	#define MY_DIB_WK512	unused1
#else
	#include "graphics.h"
	#define MY_DIB_SURFACE	my_image
//	#define MY_DIB_DATA 	data
	#define MY_DIB_DATA 	pixels
	#define MY_DIB_WK512	wk512
#endif

typedef struct
{
	MY_DIB_SURFACE *my_texture; 		/* �e�N�X�`���摜 */
	int 			texture_width;		/* �e�N�X�`���� */
	int 			texture_height; 	/* �e�N�X�`������ */
	int 			buffer_width;		/* 512 �Œ�H */
//
	int 			color_format;		/* �ϊ��ς݉摜�`�� */
	/*	�ǂݍ��ݎ���ARGB8888����摜�ϊ����s�����A
		�ǂ������`���ɉ摜�ϊ����邩�w�肷��B
		16bit�`���ł� ABGR0565 �Ƃ� ABGR1555 �Ƃ� ABGR4444 ������B
	 */
	int 			hh;/*�\��*/
	int 			jj;/*�\��*/
	int 			kk;/*�\��*/
} MY_TEXTURE_RESOURCE;

static	TGameSprite  ggg_Sprites[SPRITEMAX];
//static	MY_DIB_SURFACE *my_texture[TEXTURE_MAX];
static	MY_TEXTURE_RESOURCE 	my_resource[TEXTURE_MAX];
/*static*/	const MY_TEXTURE_RESOURCE	initial_resource[TEXTURE_MAX] =
{
	{NULL,256,256,256,	0,0,0,0},	//		TEX_00_BACK_GROUND = 0, 	/* 3D�w�i1 */
//	{NULL,256,256,512,	0,0,0,0},	//	//	TEX_01_BACK_TEXTURE,		/* �w�i��Q�� */
	{NULL,128,128,128,	0,0,0,0},	//		TEX_02_MAHOUJIN,			/* �����w */
	{NULL,256,256,256,	0,0,0,0},	//		TEX_03_JIKI,				/* ���e/���@ */
//	{NULL,256,256,512,	0,0,0,0},	//	//	TEX_04_TEKI,				/* �{�X/�U�R�G */
//	{NULL,256,256,512,	0,0,0,0},	//	//	TEX_05_ITEM,				/* �A�C�e��/�����X�R�A */
	{NULL,128,128,128,	0,0,0,0},	//		TEX_06_BULLET,				/* �G�e */
	{NULL,256,256,256,	0,0,0,0},	//		TEX_07_FRONT,				/* ���@������\��/����/[�R���e�B�j���[����(bank00)/���j���[����(bank01)/���j���[����(bank02)] */
//	{NULL,256,256,512,	0,0,0,0},	//	//	TEX_08_SCORE_PANEL, 		/* �X�R�A�p�l��/�X�R�A�t�H���g���� */
//	{NULL,256,256,512,	0,0,0,0},	//	//	TEX_09_TACHIE,				/* �����G */
//	{NULL,256,256,512,	0,0,0,0},	//	//	TEX_10_MESSAGE, 			/* ���b�Z�[�W�Œ蕶�� */
};

typedef struct
{
	unsigned short	u;
	unsigned short	v;
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
			unsigned short	color;/*GU_COLOR_5650 GU_COLOR_5551 GU_COLOR_4444*/
		#else
			unsigned int	color;
		#endif
	#endif
	short			x;
	short			y;
	short			z;
} Vertex_uvcxyz;

typedef struct
{
	unsigned short	u;
	unsigned short	v;
	#if (1==USE_VCOLOR)
		#if 0//(16==USE_BLIT_COLOR_BIT)
			unsigned short	color;/*GU_COLOR_5650 GU_COLOR_5551 GU_COLOR_4444*/
		#else
			unsigned int	color;
		#endif
	#endif
	short			x;
	short			y;
	short			z;
} Vertex_uvcxyz_C32;

typedef struct
{
	/*float*/unsigned short x;
	/*float*/unsigned short y;
	/*float*/unsigned short z;
} Point_Vertex;



#endif /* (1==USE_GU) */


#if (1==USE_GU)

void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;
int draw_side_panel;
int draw_boss_hp_value;

unsigned int conv_bg_alpha;


static void gu_draw_bg_fake3D(void);
static void gu_draw_bg_2D(void);
static void gu_draw_bg_eientei(void);

extern int select_player;
static void TGameTexture_Load_Surface(int num, char *filename);//, /*int*/UINT8 true32bit);
void select_jiki_load_surface(void)
{
	char filename[128];
	strcpy(filename, DIRECTRY_NAME_DATA "/jiki/jiki0Z.png");
	filename[11+DIRECTRY_NAME_LENGTH] = ('0'+select_player);
	TGameTexture_Load_Surface( TEX_03_JIKI, filename);//, 0/*TR UE*/ /*FA LSE*/);/*game*/
//	draw_jiki_screen = 1;
}
void stage_bg_load_surface(void)
{
	char filename[128];
	strcpy(filename, DIRECTRY_NAME_DATA "/bg/backZ_256.png");
	filename[8+DIRECTRY_NAME_LENGTH] = ('0'+/*level*/player_now_stage/*load_stage_number*/ /*player_now_stage*/);
	TGameTexture_Load_Surface( TEX_00_BACK_GROUND, filename);//, 0/*TR UE*/ /*FA LSE*/);/*game*/
	void (*aaa[8])(void)  =
	{
		gu_draw_bg_fake3D,	/**/
		gu_draw_bg_fake3D,	/*1��"���@�̐X"*/
		gu_draw_bg_2D,		/*2��"�H�߂���"*/
		gu_draw_bg_eientei, /*3��"�|��"*/
		gu_draw_bg_eientei, /*4��"�i���� �L��"*/
		gu_draw_bg_fake3D,	/*5��"�g���� ����"*/
		gu_draw_bg_fake3D,	/*6��"�g���� ��L��"*/
		gu_draw_bg_fake3D,	/*ending*/
	};
	callback_gu_draw_haikei = aaa[player_now_stage&0x07];// 	dr aw_bg_screen = 1;
//	callback_gu_draw_haikei = callback_gu_draw_haikei_all;//	dr aw_bg_screen = 1;
}

#endif


extern void sendCommandi(int cmd, int argument);
void psp_video_init(void)
{
	draw_side_panel = 0;
	draw_boss_hp_value = 0;
	callback_gu_draw_haikei = NULL;//dr aw_bg_screen = 0;
	conv_bg_alpha=0xffffffff;
//	conv_bg_alpha=0x7f7f7f7f;
//
	unsigned int i;
	if (atexit(SDL_Quit))
	{
		CHECKPOINT;
		error(ERR_WARN, "atexit dont returns zero");
	}
	SDL_VRAM_SCREEN = SDL_SetVideoMode(
		PSP_WIDTH480,
		PSP_HEIGHT272,
		/*int depth 		=*/ SDL_5551_15/*PSP_DEPTH16*/,
		/*UINT32 videoflags =*/ (SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_HWPALETTE | SDL_HWACCEL)
	//	/*UINT32 videoflags =*/ (SDL_FULLSCREEN /*| SDL_DOUBLEBUF*/ | SDL_HWSURFACE | SDL_HWPALETTE | SDL_HWACCEL)
		);
	if (NULL == SDL_VRAM_SCREEN)
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant open screen: "/*"%s", SDL_GetError()*/);
	}
	#if (1==USE_GU)
	//#define SDL_BUF_WIDTH512 (512)
	sdl_screen[SDL_00_SCREEN] = SDL_CreateRGBSurface(
		/*SDL_SWSURFACE*/SDL_HWSURFACE,/*VRAM��*/
		SDL_BUF_WIDTH512/*PSP_WIDTH480*/,/*sdl_screen[SDL_00_SCREEN]->w*/
		PSP_HEIGHT272,/*sdl_screen[SDL_00_SCREEN]->h*/
		PSP_DEPTH16,/*SDL_VRAM_SCREEN->format->BitsPerPixel*/
		/*0x001f*/PSP_SCREEN_FORMAT_RMASK/*SDL_VRAM_SCREEN->format->Rmask*/,	/*5*/
		/*0x07e0*/PSP_SCREEN_FORMAT_GMASK/*SDL_VRAM_SCREEN->format->Gmask*/,	/*6*/
		/*0xf800*/PSP_SCREEN_FORMAT_BMASK/*SDL_VRAM_SCREEN->format->Bmask*/,	/*5*/
		/*0x0000*/PSP_SCREEN_FORMAT_AMASK/*SDL_VRAM_SCREEN->format->Amask*/);	/*0*/
		if (SDL_MUSTLOCK(sdl_screen[SDL_00_SCREEN]))	{	SDL_LockSurface(sdl_screen[SDL_00_SCREEN]); }
		render_image = (UINT16 *)sdl_screen[SDL_00_SCREEN]->pixels;
		if (SDL_MUSTLOCK(sdl_screen[SDL_00_SCREEN]))	{	SDL_UnlockSurface(sdl_screen[SDL_00_SCREEN]);	}
	#endif
//	sdl_screen[SDL_01_BACK_SCREEN] = NULL;
	sdl_screen[SDL_01_BACK_SCREEN] = SDL_CreateRGBSurface(
		SDL_SWSURFACE/*SDL_HWSURFACE*/,/*���C����������*/
		SDL_BUF_WIDTH512/*PSP_WIDTH480*/,/*screen->w*/
		PSP_HEIGHT272,/*screen->h*/
		PSP_DEPTH16,/*SDL_VRAM_SCREEN->format->BitsPerPixel*/
		/*0x001f*/PSP_SCREEN_FORMAT_RMASK/*SDL_VRAM_SCREEN->format->Rmask*/,	/*5*/
		/*0x07e0*/PSP_SCREEN_FORMAT_GMASK/*SDL_VRAM_SCREEN->format->Gmask*/,	/*6*/
		/*0xf800*/PSP_SCREEN_FORMAT_BMASK/*SDL_VRAM_SCREEN->format->Bmask*/,	/*5*/
		/*0x0000*/PSP_SCREEN_FORMAT_AMASK/*SDL_VRAM_SCREEN->format->Amask*/);	/*0*/
	#if (1==USE_GU)
		if (SDL_MUSTLOCK(sdl_screen[SDL_01_BACK_SCREEN]))	{	SDL_LockSurface(sdl_screen[SDL_01_BACK_SCREEN]);	}
		render_image_back = (UINT16 *)sdl_screen[SDL_01_BACK_SCREEN]->pixels;
		if (SDL_MUSTLOCK(sdl_screen[SDL_01_BACK_SCREEN]))	{	SDL_UnlockSurface(sdl_screen[SDL_01_BACK_SCREEN]); }
	#endif
	#if (0)
	if (NULL == sdl_screen[SDL_01_BACK_SCREEN])
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant create SDL_Surface: "/*"%s", SDL_GetError()*/);
	}
	#endif
	sdl_screen[SDL_02_TEX_SCREEN] = SDL_CreateRGBSurface(
		SDL_SWSURFACE/*SDL_HWSURFACE*/,/*���C����������*/
		SDL_BUF_WIDTH512/*PSP_WIDTH480*/,/*screen->w*/
		PSP_HEIGHT272,/*screen->h*/
		PSP_DEPTH16,/*SDL_VRAM_SCREEN->format->BitsPerPixel*/
		/*0x001f*/PSP_SCREEN_FORMAT_RMASK/*SDL_VRAM_SCREEN->format->Rmask*/,	/*5*/
		/*0x07e0*/PSP_SCREEN_FORMAT_GMASK/*SDL_VRAM_SCREEN->format->Gmask*/,	/*6*/
		/*0xf800*/PSP_SCREEN_FORMAT_BMASK/*SDL_VRAM_SCREEN->format->Bmask*/,	/*5*/
		/*0x0000*/PSP_SCREEN_FORMAT_AMASK/*SDL_VRAM_SCREEN->format->Amask*/);	/*0*/
	#if (1==USE_GU)
		if (SDL_MUSTLOCK(sdl_screen[SDL_02_TEX_SCREEN]))	{	SDL_LockSurface(sdl_screen[SDL_02_TEX_SCREEN]); }
		render_image_tex = (UINT16 *)sdl_screen[SDL_02_TEX_SCREEN]->pixels;
		if (SDL_MUSTLOCK(sdl_screen[SDL_02_TEX_SCREEN]))	{	SDL_UnlockSurface(sdl_screen[SDL_02_TEX_SCREEN]); }
	#endif





//	#if ((1==USE_GU)|| defined(ENABLE_PSP))
	#if (1==USE_GU)
	//# /* �J�X�^�����C�u������Gu���g���ꍇ */



#if (1==USE_16BIT_BLIT)
	#define  SCREEN_DEPTH	16
#else
	#define  SCREEN_DEPTH	32
#endif

	/* setup GU initialise */
	sceGuInit();
	#if 0
	/* �����Ă��w�Ǔ��� */
	sceGuDisplay(GU_FALSE);/*���OFF*/
	#endif
	sceGuStart(GU_DIRECT, gulist);
	#if (16 == SCREEN_DEPTH/*depth*/)/*16 bit(5650), 15 bit(5551), 12 bit(4444) */
	//	sceGuDrawBuffer(GU_PSM_5551, (void*)0, BUF_WIDTH512);
		sceGuDrawBuffer(SDL_GU_PSM_0000, (void*)0, BUF_WIDTH512);
		sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)0x44000, BUF_WIDTH512);
		#if (1==USE_ZBUFFER)
		sceGuDepthBuffer((void*)0x88000, BUF_WIDTH512);
		#endif /* (1==USE_ZBUFFER) */
	#else // (32 == SCREEN_DEPTH/*depth*/)/*32 bit(8888) */
		sceGuDrawBuffer(GU_PSM_8888, (void*)0, BUF_WIDTH512);
		sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)0x88000, BUF_WIDTH512);
		#if (1==USE_ZBUFFER)
		sceGuDepthBuffer((void*)0x110000, BUF_WIDTH512);
		#endif /* (1==USE_ZBUFFER) */
	#endif /* SCREEN_DEPTH */
	sceGuOffset(2048 - (PSP_WIDTH480 / 2), 2048 - (PSP_HEIGHT272 / 2));
	sceGuViewport(2048, 2048, PSP_WIDTH480, PSP_HEIGHT272);

	#if 1
	/* �`��͈͂�ݒ肷�� */
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);
	#endif

	#if 1/*???*/
	//sceGuAlphaFunc(GU_ALWAYS,0,0xff);
	//sceGuEnable(GU_ALPHA_TEST);		/*sceGuDisable(GU_ALPHA_TEST);noiz07*/
	//sceGuDepthFunc(GU_GEQUAL);
	//sceGuEnable(GU_DEPTH_TEST);		/*sceGuDisable(GU_DEPTH_TEST);noiz07*/
	#endif/*???*/

#if 000/*???*/
	#if 1/*???*/
	/*�u�A���t�@�u�����f�B���O(�����x�t���A�F�d�ˍ��킹)�v*/
	sceGuAlphaFunc(GU_ALWAYS,0,0xff);//sceGuAlphaFunc(GU_LEQUAL, 0, 0x01);
	sceGuEnable(GU_ALPHA_TEST); 	/*sceGuDisable(GU_ALPHA_TEST);noiz07*/
	//sceGuAlphaFunc(GU_ALWAYS,0,0xff);
	//sceGuEnable(GU_ALPHA_TEST);			/* �L�� */
	//sceGuDisable(GU_ALPHA_TEST);			/* ���� */	/*noiz07*/
	#endif/*???*/
#endif/*000???*/

	#if (1==USE_ZBUFFER)
	//sceGuEnable(GU_DEPTH_TEST);			/* �L�� */
	//sceGuDisable(GU_DEPTH_TEST);			/* ���� */	/*noiz07*/
	sceGuDisable(GU_DEPTH_TEST);
	/* psp �� z�l�� 65535(�߂�) ���� 0(����)�ŁAZ�l���ʏ�Ƌt�B */
	/* ����z�l�� unsigned short �Ȃ̂ŁA�����͂ݏo���ƁA��荞��ŕςȕ`��ɂȂ��Ă��܂��B */
	/* �����ŁA��荞�܂Ȃ��悤�ɁA�K���ɗ]�T�������U���BPSPSDK �̃T���v���ł� */
	/* 0xc350(50000) �߂����̋��E�ʁA0x2710(10000)�������̋��E�� �ƂȂ��Ă��邪�A�K���Ȓl�Ȃ牽�ł��悢 */
	/* sceGuDepthRange(0xc350, 0x2710); */
	#define  Z_CLIP_LENGTH	(10000)
	sceGuDepthRange((0xffff-Z_CLIP_LENGTH), Z_CLIP_LENGTH);
//	sceGuDepthRange(65535, 0);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuDepthMask(GU_TRUE);
	#endif/* (1==USE_ZBUFFER) */

	#if 1
	/* �J�����O���[�h(�������|���S���͕`�悵�Ȃ�)�ŁA�L�����W�n(�E����W�n�ɂ��邩������W�n�ɂ��邩)�����߂� */
	sceGuEnable(GU_CULL_FACE);	/* �J�����O�L��(�������|���S���͕`�悵�Ȃ�) */
	sceGuFrontFace(GU_CW);		/* clock wise(���v����) 		  �E���A�E����W�n(OpenGL �W��?)	 */
//	sceGuFrontFace(GU_CCW); 	/* counter clock wise(�����v����) �����A������W�n(Direct X �W��?) */
/*
  ���v���(�Ƃ����܂��)�Ƃ����̂́A���v�̐j���i�ޕ����Ɠ�����]�����̎��ł���B
�と�E���������A���邢�͖k�������쁨���̏��ɉ�]��������ł���B�E���Ƃ�������B
  �t�̉�]�������u�����v���v(�͂�Ƃ����܂��)�Ƃ����B���l�ɁA����͍����ƌ�����B
�����v���͉p�ꌗ�ł� counterclockwise �ƌĂ΂�邪�A����� CCW �Ɨ����ꍇ�������B

  ���v���̉�]�����͓����v��͂��Ă���B�����v�͖k��A�����k�Ŕ������ꂽ�B
�����v�̉�]�����́A�k��A�����k�ɂ����Ă͎��v���ɂȂ邪��ł͔����v���ɂȂ�B

  ����(���ʊp)�́A�k��0���Ƃ��Ď��v���ɐ��𑝂₷���A
�t�ɐ��w�ɂ����ẮA���ʊp�͔����v���̕����𐳂Ƃ��Čv�������B

  �r�̂ӂ���l�W���́A���v���ɉ񂷂ƒ��܂�A�����v���ɉ񂷂Ɗɂނ悤�ɍ���Ă��邪�A
�����͉E�����̏ꍇ�A���v���̕����͂�����Ղ��Ƃ������R�ɂ��B
�͂̓��ꂷ���Ŕj������̂�h���ׂɃK�X�ǂł͂����Ƃ͋t�����ɂȂ��Ă���B

���[�Ȃ̂��[
*/
	#endif

#if 1/*???*/
	#if 1/*???*/
	sceGuShadeModel(GU_SMOOTH);
	#endif/*???*/
#endif/*1???*/

	#if 0/*???*/
	sceGuDisable(GU_BLEND);
	#else
	sceGuEnable(GU_BLEND);			/* �A���t�@�u�����f�B���O�L�� */
	#endif/*???*/

	#if 1
	/* �e�N�X�`���[�L�����[�h�ł́A����ʓ��̃t���b�g�|���S�����g���Ȃ� */
	sceGuEnable(GU_TEXTURE_2D); 	/* �e�N�X�`���[�L�� */
	#else
	/* �e�N�X�`���[�������[�h�ł́A����ʓ��̃t���b�g�|���S�����g����(�e�N�X�`���[�|���S���A2D�摜�]�����͏o���Ȃ�) */
	sceGuDisable(GU_TEXTURE_2D);	/* �e�N�X�`���[���� */
	#endif

	#if (1==USE_ZBUFFER)
	sceGuClearDepth(0);//Z_BUFFER
/*??*/	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);	/*noiz07*/
	#endif/* (1==USE_ZBUFFER) */
//	sceGuClear(GU_COLOR_BUFFER_BIT/*|GU_DEPTH_BUFFER_BIT*/);	/*noiz07*/

	#if 1
		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/*������*/
	#else
		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);/*�������s�H*/
	#endif

//	sceGuTexFilter(GU_NEAREST, GU_NEAREST);/*��������g����(�g�厞�o�Ȑ��⊮)*/
	sceGuTexFilter(GU_LINEAR, GU_LINEAR);/*�ڂ�ڂ�g����(�g�厞�����⊮)*/
	sceGuTexScale(1.0f, 1.0f);//sceGuTexScale(1,1);
	sceGuTexOffset(0.0f, 0.0f);
//
	/* ���F�����߂�H */
	sceGuTexEnvColor(	0x00000000);
//	sceGuTexEnvColor(	0x00ffff00);
	sceGuAmbientColor(	0xffffffff);
	sceGuColor( 		0xff000000);

//


//	u8	ColorKey	= 1;
/* �J���[�L�[��ݒ肷�� */
//	if (ColorKey)
	{
//		if (!sceGuGetStatus(GU_COLOR_TEST))
		{	sceGuEnable(GU_COLOR_TEST);}/*�J���[�L�[�L��*/
	//	sceGuColorFunc(GU_NOTEQUAL, 0/*texptr->colorkey*/, 0x00fefefe);
		sceGuColorFunc(GU_NOTEQUAL, 0/*texptr->colorkey*/, 0x00ffffff);
	//	sendCommandi(216, GU_NOTEQUAL & 0x03);
	//	sendCommandi(217, 0 & 0xffffff);
	//	sendCommandi(218, 0x00fefefe);
	}
//	else
//	{
//		if (sceGuGetStatus(GU_COLOR_TEST))
//		{	sceGuDisable(GU_COLOR_TEST);}/*�J���[�L�[����*/
//	}
	#if 1
	sceGuEnable(GU_CLIP_PLANES);
	#endif
//
	sceGuFinish();
	sceGuSync(0, 0);

//	sceDisplayWaitVblankStart();/*vsync*/
	sceGuDisplay(GU_TRUE/*1*/);/*���ON*/
	/* �����܂ŏ����ݒ� */

//	sceDisplayWaitVblankStart();/*vsync*/

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	/* --- �e�N�X�`���}�l�[�W���̏����� */
	//static TGameTexture *TGameTexture_Create(void)
	{
		for (i=0; i<TEXTURE_MAX; i++)
		{
		//	my_resource[i].my_texture = NULL;
			my_resource[i].my_texture		= initial_resource[i].my_texture;
			my_resource[i].texture_width	= initial_resource[i].texture_width;
			my_resource[i].texture_height	= initial_resource[i].texture_height;
			my_resource[i].buffer_width 	= initial_resource[i].buffer_width;
		//
			my_resource[i].color_format 	= initial_resource[i].color_format;
			my_resource[i].hh				= initial_resource[i].hh;/*�\��*/
			my_resource[i].jj				= initial_resource[i].jj;/*�\��*/
			my_resource[i].kk				= initial_resource[i].kk;/*�\��*/
		}
	}

	/* --- �X�v���C�g�}�l�[�W���̏����� */
	for (i=0; i<SPRITEMAX; i++)
	{
		ggg_Sprites[i].texture_id	= 0/*-1*/;
	//	ggg_Sprites[i].Texture_ptr	= NULL;
		ggg_Sprites[i].used 		= 0/*FA LSE*/;
		#if (1==USE_ZOOM_XY)
		ggg_Sprites[i].zoom_x256	= 256/*1.0*/;
		ggg_Sprites[i].zoom_y256	= 256/*1.0*/;
		#else //(0==USE_ZOOM_XY)
		ggg_Sprites[i].zoom_xy256	= 256/*1.0*/;
		#endif/* (1==USE_ZOOM_XY) */
		ggg_Sprites[i].rotation_z	= 0/*.0*/;
	}

	/* --- �e�N�X�`���̏����� */
	{
	/* --- �e�N�X�`���t�@�C���� */
	//char* texture_name[] =
	//{
	//	#if 1
			#define TEXTURE_NAME_00 DIRECTRY_NAME_DATA "/tama/bullet.png"
			#define TEXTURE_NAME_01 DIRECTRY_NAME_DATA "/effect/front256.png"
			#define TEXTURE_NAME_02 DIRECTRY_NAME_DATA "/effect/mahoujin128.png"
//			#define TEXTURE_NAME_01 "texture02.png"
	//	#else
	//		#define TEXTURE_NAME_00 "texture1.bmp"/*"texture1.png"*/
//	//		#define TEXTURE_NAME_01 "clip1.png"
	//	#endif
	//};
//		#ifdef PSP
	//	TGameTexture_Load_Surface( TEX_06_BULLET,	TEXTURE_NAME_00);//, 1/*TR UE*/ /*FA LSE*/);/*game*/
		TGameTexture_Load_Surface( TEX_06_BULLET,	TEXTURE_NAME_00);//, 0/*TR UE*/ /*FA LSE*/);/*game*/
		TGameTexture_Load_Surface( TEX_07_FRONT,	TEXTURE_NAME_01);//, 0/*TR UE*/ /*FA LSE*/);/*game*/
		TGameTexture_Load_Surface( TEX_02_MAHOUJIN, TEXTURE_NAME_02);//, 0/*TR UE*/ /*FA LSE*/);/*game*/
//		TGameTexture_Load_Surface( TEX_00_BACK_GROUND, TEXTURE_NAME_01);//, 0/*FA LSE*/);/*title*/
//		#else
//		TGameTexture_Load_Surface( TEX_06_BULLET, TEXTURE_NAME_00);//, 1/*TR UE*/);/*Pure*/
//		TGameTexture_Load_Surface( TEX_00_BACK_GROUND, TEXTURE_NAME_01);//, 1/*TR UE*/);/*Pure*/
//		#endif
	}
	#endif /* (1==USE_GU) */

	/* ���͑��u�̏����ݒ� */

	/* �̏����ݒ� */

}
/*-------------*/
/*-------------*/

/*---------------------------------------------------------
	�e�N�X�`�������[�h����
---------------------------------------------------------*/

#define USE_T128_SWIZZLE 0
#if (1==USE_T128_SWIZZLE)
static void convert_swizzle_RGB8888_T128/*512*/
(
		  u8	*out_data,
	const u8	*in_data
)
{
	const u8	*src_y = in_data;
	u32 		*dst  = (u32*)out_data;
	u8 block_y;
	block_y = /*height_blocks*/((TH128/*512*/) >>3/*/  8*/);/*max: 64-1*/
	while (block_y--)
	{
		const u8	*src_x = src_y;
		u8 block_x;
		block_x = /*width_blocks*/((TW128/*512*/*4) >>4/*/ 16*/);/*max: 32-1*/
		while (block_x--)
		{
			const u32	*src = (u32*)src_x;
			u8 j;
			j=8;	/*max: 8-1*/
			while (j--)
			{
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				src += /*src_pitch*/(((TW128/*512*/*4)-16)>>2/*/4*/);
			}
			src_x += 16;
		}
		src_y += /*src_row*/((TH128/*512*/*4) <<3/** 8*/);
	}
}
#endif /*(0)*/

//#if 1//(1==USE_COLOR_16_AND_32)
#if (1==USE_COLOR_16_AND_32)
static void convert_swizzle
(
		  u8	*out_data,		/* �o�͉摜�̐擪�|�C���^ */
	const u8	*in_data,		/* ���͉摜�̐擪�|�C���^ */
/* (�Ⴆ�΁A512x512�e�N�X�`���[�̏ꍇ) */
	const u32	width_x_size,	/* ��������f�[�^�[�T�C�Y */	/*max: 512*4 */
	/* (�Ⴆ�΁AARGB8888 �Ȃ� 512*4==width_x_size) */
	/* (�Ⴆ�΁AARGB1555 �Ȃ� 512*2==width_x_size) */
	const u32	height			/* ���� */		/*max: 512*/
	/* (�Ⴆ�΁A512==height) */
)
{
	#if (1==USE_SWIZZLE)
	const u8	*src_y = in_data;
	u32 		*dst  = (u32*)out_data;
	u8 block_y;
	for (block_y = 0; block_y < /*height_blocks*/(height >>3/* /8 */); block_y++)/*max: 64-1*/
	{
		const u8	*src_x = src_y;
		u8 block_x;
		for (block_x = 0; block_x < /*width_blocks*/(width_x_size  >>4/* /16 */); block_x++)/*max: 32-1*/
		{
			const u32	*src = (u32*)src_x;
			u8 j;
			for (j = 0; j < 8; j++)/*max: 8-1*/
			{
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				src += /*src_pitch*/((width_x_size-16)>>2/* /4 */);
			}
			src_x += 16;
		}
		src_y += /*src_row*/(width_x_size <<3/* *8 */);
	}
	#else
	u32 		*src = (u32*)in_data;
	u32 		*dst = (u32*)out_data;
	u8 block_y;
	for (block_y = 0; block_y < /*height_blocks*/(height >>3/* /8 */); block_y++)/*max: 64-1*/
	{
		u8 block_x;
		for (block_x = 0; block_x < /*width_blocks*/(width_x_size  >>4/* /16 */); block_x++)/*max: 32-1*/
		{
			u8 j;
			for (j = 0; j < 8; j++)/*max: 8-1*/
			{
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
			}
		}
	}
	#endif
}
#endif
//#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	//#define RMASK 0xff000000
	//#define GMASK 0x00ff0000
	//#define BMASK 0x0000ff00
	//#define AMASK 0x000000ff
	//#define RMASK16 0x0000f800
	//#define GMASK16 0x000007c0
	//#define BMASK16 0x0000003f
	//#define AMASK16 0x00000001
//#else
	//#define RMASK 0x000000ff
	//#define GMASK 0x0000ff00
	//#define BMASK 0x00ff0000
	//#define AMASK 0xff000000
	#define RMASK16 0x0000001f
	#define GMASK16 0x000003e0
	#define BMASK16 0x00007c00
	#define AMASK16 0x00008000
//#endif

/*---------------------------------------------------------
	�e�N�X�`���[�̓ǂݍ��݁A�o�^
---------------------------------------------------------*/

static void TGameTexture_Load_Surface(
		int num,
		char *filename//,
//		MY_DIB_SURFACE *GameScreen,
		)
{
//TGameTexture *pclass=/*pclass*/ggg_Texture;
	//Image *plane;//
	MY_DIB_SURFACE *plane;
//	MY_DIB_SURFACE *standard;
	void *nonalign;
	unsigned int msize;
	int loop;
/*int*/UINT8 true32bit = 0;
//	standard = 0;
	nonalign = 0;
	msize = 0;
	loop = 0;
	//if (ggg_Texture/*pclass*/ == NULL)
	//{
	//	return;
	//}

	/* ----- �e�N�X�`���ԍ����s����������I�� */
//	if (num < 0) return;
//	if (num > TEXTURE_MAX) return;

	/* ----- ���Ƀe�N�X�`�������������� */
	if (NULL != my_resource[num].my_texture)
	{
		#if (1==USE_SDL_image)
		SDL_FreeSurface(my_resource[num].my_texture);
		#else
		png_free_my_image(my_resource[num].my_texture);
		#endif
		my_resource[num].my_texture = NULL;
	}

	/* ----- �e�N�X�`���[�̓ǂݍ��� */
	#if (1==USE_SDL_image)
	plane = IMG_Load(filename);
//	plane = SDL_LoadBMP(filename);
	#else
	plane = png_load_my_image(filename);//
	#endif

	#if 1
	int mymap_128;
	//	mymap_128 = (my_resource[num].my_texture)->texture_width;
		mymap_128 = my_resource[num].texture_width;
	#else
		int mymap_128 = 128;
		switch (num)
		{
		case TEX_02_MAHOUJIN:
		case TEX_06_BULLET:
									mymap_128 = 128;
			break;
		default:
		#if 0
		case TEX_00_BACK_GROUND:
		case TEX_03_JIKI:
		case TEX_07_FRONT:
		#endif
									mymap_128 = 256;
		}
	#endif
		#define my_map_TW128 mymap_128
		#define my_map_TH128 mymap_128

	#if (0)
	if (mymap_128 != my_texture[num]->texture_width)
	{
	//	pspDebugScreenInit();/*�v��*/
	//	pspDebugScreenClear();
	//	pspDebugScreenSetXY(0,0);
	//	pspDebugScreenPrintf(
		error(ERR_FATAL,
			"[%d] (%d) : %d : %d",
			num,
			mymap_128,
			my_texture[num]->texture_width,
			my_texture[num]->texture_height);
		hit_any_key();
	}
	#endif


	if (NULL == plane)
	{
		#if (1)
		CHECKPOINT;
		error(ERR_FATAL, "cant load image %s:"/*" %s"*/,filename/*,SDL_GetError()*/);
		#endif
		my_resource[num].my_texture = NULL;
		return;
	}
	//#if 1
	#ifndef PSP
	/* --- Normal SDL work for PC */
	if (true32bit ==  0/*FA LSE*/)/*16bit mode*/
	{
		my_resource[num].my_texture = SDL_ConvertSurface(plane,
				GameScreen->format,
				SDL_SWSURFACE|SDL_SRCALPHA);
		if ((plane != NULL) &&
			(plane != my_resource[num].my_texture))
		{
			SDL_FreeSurface(plane);
		}
	}
	else/*32bit mode*/
	{
		my_resource[num].my_texture = plane;
	}
	#else
	/* --- PSP �����Ƀe�N�X�`�������H���� */

	my_resource[num].my_texture = plane;

	/* --- PSP�� 16byte align �ɖ����� �s��������(�x��?) �炵���B */
	/* --- ������ memalign(); ���g���A���������E�����킹��B */
	nonalign = (my_resource[num].my_texture)->MY_DIB_DATA;

	#if (1==USE_SDL_image)
	msize = ((my_resource[num].my_texture)->w * (my_resource[num].my_texture)->h)
		 * (my_resource[num].my_texture)->format->BytesPerPixel;
	#else
	msize = (/*w*/my_map_TW128/*512*/ * /*h*/my_map_TH128/*512*/) * /*bpp*/4;
	#endif

#if (1==USE_COLOR_16_AND_32)
	/* --- PSP �ŊȈ�16�r�b�g�F���w�肵���ꍇ��16bit�ɗ��Ƃ� */
	#if (1==USE_SDL_image)
	if ((true32bit ==  0/*FA LSE*/) &&
		(my_texture[num]->format->BytesPerPixel == 4))	/* PSP16bit mode && DIB==32bit color */
	#else
	if ((true32bit ==  0/*FA LSE*/) &&
		(/*BytesPerPixel*/4 == 4))	/* PSP16bit mode && DIB==32bit color */
	#endif
	//if (0)
	{
	unsigned short *pixdst;
	unsigned long  *pixsrc;
		/* --- 16bit�F�� ���F���ĕێ� */
		(my_resource[num].my_texture)->pixels = (void*)memalign(16, (msize>>1)/*(msize / 2)*/);
		#if (1==USE_SDL_image)
		msize = ((my_resource[num].my_texture)->w * (my_resource[num].my_texture)->h);
		#else
		msize = (my_map_TW128 * my_map_TH128);
		#endif
		pixdst = (unsigned short *)/*convert_works*/gulist/*pclass->bitmap[num]->pixels*/;
		pixsrc = (unsigned long *)nonalign;
		for (loop=0; loop<msize; loop++)
		{
			#if 0
			/* --- 15bit�F�� ���F���ĕێ� (color key �g�p��) */
			*pixdst =
				((*pixsrc & 0x80000000) >> 16) |
				((*pixsrc & 0x00f80000) >>	9) |
				((*pixsrc & 0x0000f800) >>	6) |
				((*pixsrc & 0x000000f8) >>	3);
		//	*pixdst = MAKECOL15A(GETR32(*pixsrc), GETG32(*pixsrc), GETB32(*pixsrc), GETA32(*pixsrc));
			#else
			/* --- 16bit�F�� ���F���ĕێ� (color key �g�p�s��) */
			*pixdst = MAKECOL16(GETR32(*pixsrc), GETG32(*pixsrc), GETB32(*pixsrc));
			#endif
			pixdst++;
			pixsrc++;
		}

	//	unsigned short *pixsrc2;
	//	pixsrc2 = (unsigned short *)pclass->bitmap[num]->pixels;
	//	pixdst = (unsigned short *)gulist/*convert_works*/;
	//	for (loop=0; loop<msize; loop++)
	//	{
	//		(*pixdst) = (*pixsrc2);
	//		pixdst++;
	//		pixsrc2++;
	//	}
		#if (1==USE_SDL_image)
		my_texture[num]->format->BytesPerPixel =  2;
		my_texture[num]->format->BitsPerPixel  = 16;
		my_texture[num]->format->Rmask = /*PSP_SCREEN_FORMAT_RMASK*/RMASK16/*RMASK16*/;
		my_texture[num]->format->Gmask = /*PSP_SCREEN_FORMAT_GMASK*/GMASK16/*GMASK16*/;
		my_texture[num]->format->Bmask = /*PSP_SCREEN_FORMAT_BMASK*/BMASK16/*BMASK16*/;
		my_texture[num]->format->Amask = /*PSP_SCREEN_FORMAT_AMASK*/AMASK16/*AMASK16*/;
		#endif
	// 16bit�F
		convert_swizzle(
			(u8*)((my_resource[num].my_texture)->MY_DIB_DATA),
			(const u8*)gulist/*convert_works*/,
			/* texture width x 2 */  my_map_TW128/*512*/*2, 	/*short ������ 2�{*/
			/* texture height	 */  my_map_TH128/*512*/);
		// 512*2 because swizzle operates in bytes, and each pixel in a 16-bit texture is 2 bytes
	}
	else/*32bit mode*/
#endif
	{
		/* --- 32bit�F �ʏ탂�[�h */
		((my_resource[num].my_texture)->MY_DIB_DATA) = (void*)memalign(16, msize);
		memcpy(/*convert_works*/gulist/*pclass->bitmap[num]->pixels*/, nonalign, msize);
		#if (1==USE_T128_SWIZZLE)/*0*/
	// 32bit�F
	//	convert_swizzle(
		convert_swizzle_RGB8888_T128/*512*/(
			(u8*)((my_resource[num].my_texture)->MY_DIB_DATA),
			(const u8*)gulist/*convert_works*/
	//	,
	//		/* texture width x 4 */ 512*4,	/*int ������ 4�{*/
	//		/* texture height	 */ 512
		);
		// 512*2 because swizzle operates in bytes, and each pixel in a 16-bit texture is 2 bytes
		#else
	// 32bit�F
		convert_swizzle(
			(u8*)((my_resource[num].my_texture)->MY_DIB_DATA),
			(const u8*)gulist/*convert_works*/,
			/* texture width x 4 */  my_map_TW128/*512*/*4, 	/*int ������ 2�{*/
			/* texture height	 */  my_map_TH128/*512*/);
		// 512*2 because swizzle operates in bytes, and each pixel in a 16-bit texture is 2 bytes
		#endif
	}
	free(nonalign);
	#endif
}

/*---------------------------------------------------------
	�X�v���C�g�̕\�� (��]�g��k���Ȃ�)
	�X�v���C�g�̕\��(��ʓ]��)
---------------------------------------------------------*/

#if (1==USE_ZBUFFER)
	/* �n�[�h�E�F�A�[�Ń\�[�g */
	#define SPR_PRIORITY	(spr->priority)
#else
	/* �\�t�g�E�F�A�[�� Z�\�[�g */
	#define SPR_PRIORITY	(0)
#endif
/* --- �X�v���C�g���ꖇ�X�N���[���ɓ\��t���� */
static void render_object_no_rot_zoom(/*TGameScreen *pclass,*/ TGameSprite *spr)
{
	/* --- �\���X�C�b�` */
//	if (spr->used ==  0/*FA LSE*/) return;
//	if (spr->Texture == NULL) return;

	/* --- �����������l */
	#if (1==USE_VCOLOR)
		#if 0//(16==USE_BLIT_COLOR_BIT)
			#if 0
				/*ARGB4444*/
				unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
			#endif
			#if 1
				/*ARGB0565*/
				unsigned /*int*/short blendlevel = MAKECOL16( (spr->alpha), (spr->alpha), (spr->alpha) );
			#endif
		#else
//	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
	unsigned int blendlevel = (spr->color8888);
		#endif
	#endif

	/* --- �X�v���C�g���ꖇ�`�悷�� */
	/* ������x�̑傫�����������X�v���C�g�͒Z����ɕ������Ȃ��� */
	/* �ƂĂ��Ȃ��x���Ȃ� */

	unsigned int/*short*/ w_rest;
	w_rest = spr->w + SLICE_SIZE;

	unsigned int/*short*/ count2;
	#if 0
	//count2 = (((spr->w / SLICE_SIZE)+1)*2);
	#else
	count2 = 0;
//	for (j=0; (j+SLICE_SIZE)<=(/*480*/spr->w/*512*/); j+=SLICE_SIZE)
	unsigned int/*short*/ i;
	i = 0;
	do
	{
		count2 += 2;
		i += SLICE_SIZE;
		w_rest -= SLICE_SIZE;
	}
	while ( (i) <(/*480*/spr->w/*512*/));
	#endif
	Vertex_uvcxyz_C32	*vertices;
	vertices = (Vertex_uvcxyz_C32*)sceGuGetMemory(count2 * sizeof(Vertex_uvcxyz_C32));
	i = 0;
	unsigned int/*short*/ pos = 0;
	unsigned int/*short*/ w_size;
	w_size	= SLICE_SIZE;
	unsigned int/*short*/ x_pos = ((spr->x256>>8));
	unsigned int/*short*/ y_pos = ((spr->y256>>8));
	for (; i<count2; )
	{
	//	if ((i1+1) < count)
	//	if (((i)+1*2) < (count2))	{}	else		/* �Ō� */
		if (i==(count2-2))		/* �Ō� */
		{
			w_size = w_rest;
		}
	//
		vertices[(i)].u = (spr->tx + pos);
		vertices[(i)].v = spr->ty;
		#if (1==USE_VCOLOR)
		vertices[(i)].color = blendlevel;
		#endif
		vertices[(i)].x = (x_pos + pos);
		vertices[(i)].y = y_pos;
		vertices[(i)].z = SPR_PRIORITY;
	//
		vertices[(i)+1].u	= (spr->tx + pos) + w_size;
		vertices[(i)+1].v	= spr->ty + spr->h;
		#if (1==USE_VCOLOR)
		vertices[(i)+1].color = blendlevel;
		#endif
		vertices[(i)+1].x	= (x_pos + pos) + w_size;
		vertices[(i)+1].y	= y_pos + spr->h;
		vertices[(i)+1].z	= SPR_PRIORITY;
		i += 2;
		pos += SLICE_SIZE;
	}
	/* --- �`�惊�N�G�X�g */
//	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS4444, (count2), NULL, vertices);
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650_C32, (count2), NULL, vertices);
}

/*---------------------------------------------------------
	�X�v���C�g�̕\�� (��]�g��k������)
---------------------------------------------------------*/

static void render_object_use_rot_zoom(/*TGameScreen *pclass,*/ TGameSprite *spr)
{
	/* --- �\���X�C�b�` */
//	if (spr->used ==  0/*FA LSE*/) return;
//	if (spr->Texture == NULL) return;
//
	/* --- ��]�g��O���� */
	int sin_angle;
	int cos_angle;
	{
		/* �p�x��0-511�x */
		unsigned int rotation_angle512;
	//	rotation_angle512	= ((spr->rotation_z) / (128/*65536/512*/)); 	/* �p�x��0-65535�x�Ȃ̂�0-511�x�֕ϊ��B */
		rotation_angle512	= ((spr->rotation_z) /*>> (7)*/);
		sin_angle = (sin_tbl512[/*rot_sin*/((/*OFFS_SIN+*/rotation_angle512)&(512-1))]/*<<8*/);
		cos_angle = (sin_tbl512[/*rot_cos*/((  OFFS_COS+  rotation_angle512)&(512-1))]/*<<8*/);
	}
//
	/* --- �����������l */
	#if (1==USE_VCOLOR)
		#if 0//(16==USE_BLIT_COLOR_BIT)
			#if 0
					/*ARGB4444*/
				unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
			#endif
			#if 1
				/*ARGB0565*/
				unsigned /*int*/short blendlevel = MAKECOL16( (spr->alpha), (spr->alpha), (spr->alpha) );
			#endif
		#else
//	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
	unsigned int blendlevel = (spr->color8888);
		#endif
	#endif

	/* --- �X�v���C�g���ꖇ�`�悷�� */
	/* ������x�̑傫�����������X�v���C�g�͒Z����ɕ������Ȃ��� */
	/* �ƂĂ��Ȃ��x���Ȃ� */

	unsigned int w_rest;
	w_rest = spr->w + SLICE_SIZE;

	unsigned int count4;
	#if 0
	//count4 = (((spr->w / SLICE_SIZE)+1)*4);
	#else
	count4 = 0;
	unsigned int i;
	i = 0;
	do
	{
		count4 += 4;
		i += SLICE_SIZE;
		w_rest -= SLICE_SIZE;
	}
	while ( (i) <(/*480*/spr->w/*512*/));
	#endif
	Vertex_uvcxyz_C32	*vertices;
	vertices = (Vertex_uvcxyz_C32*)sceGuGetMemory(count4 * sizeof(Vertex_uvcxyz_C32));
	i = 0;
	unsigned int/*short*/ pos = 0;
	unsigned int/*short*/ w_size;
	w_size	= SLICE_SIZE;
	unsigned int/*short*/ x_pos = ((spr->x256>>8));
	unsigned int/*short*/ y_pos = ((spr->y256>>8));
	for (; i<count4; )
	{
	//	if (((i)+1*4) < (count4))	{}	else		/* �Ō� */
		if (i==(count4-4))	/* �Ō� */
		{
			w_size = w_rest;
		}
		unsigned int pos_w_size = (pos + w_size);
		vertices[(i)].u 		= spr->tx + pos;
		vertices[(i)].v 		= spr->ty;
		#if (1==USE_VCOLOR)
		vertices[(i)].color 	= blendlevel;
		#endif
		vertices[(i)].x 		= x_pos + pos;
		vertices[(i)].y 		= y_pos;
		vertices[(i)].z 		= SPR_PRIORITY;
	//
		vertices[(i)+1].u		= spr->tx + (pos_w_size);
		vertices[(i)+1].v		= spr->ty;
		#if (1==USE_VCOLOR)
		vertices[(i)+1].color	= blendlevel;
		#endif
		vertices[(i)+1].x		= x_pos + (pos_w_size);
		vertices[(i)+1].y		= y_pos;
		vertices[(i)+1].z		= SPR_PRIORITY;
	//
		vertices[(i)+2].u		= spr->tx + pos;
		vertices[(i)+2].v		= spr->ty + spr->h;
		#if (1==USE_VCOLOR)
		vertices[(i)+2].color	= blendlevel;
		#endif
		vertices[(i)+2].x		= x_pos + pos;
		vertices[(i)+2].y		= y_pos + spr->h;
		vertices[(i)+2].z		= SPR_PRIORITY;
	//
		vertices[(i)+3].u		= spr->tx + (pos_w_size);
		vertices[(i)+3].v		= spr->ty + spr->h;
		#if (1==USE_VCOLOR)
		vertices[(i)+3].color	= blendlevel;
		#endif
		vertices[(i)+3].x		= x_pos + (pos_w_size);
		vertices[(i)+3].y		= y_pos + spr->h;
		vertices[(i)+3].z		= SPR_PRIORITY;
	//
		pos += SLICE_SIZE;
		/* --- ��]�g�又�� */
		int center_x;
		int center_y;
		center_x = ((spr->x256>>8)		) + (spr->w >> 1);/*/2*/
		center_y = ((spr->y256>>8)		) + (spr->h >> 1);/*/2*/
		#if (1==USE_ZOOM_XY)
		int zoom_x256;
		int zoom_y256;
	//	zoom_x256 = (spr->zoom_x256) * ((65536/256));	/* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
	//	zoom_y256 = (spr->zoom_y256) * ((65536/256));	/* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
		zoom_x256 = (spr->zoom_x256) /*<< (8)*/;		/* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
		zoom_y256 = (spr->zoom_y256) /*<< (8)*/;		/* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
		#else //(0==USE_ZOOM_XY)
		int zoom_xy256;
	//	zoom_xy256 = (spr->zoom_xy256) * ((65536/256)); /* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
		zoom_xy256 = (spr->zoom_xy256) /*<< (8)*/;		/* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
		#endif/* (1==USE_ZOOM_XY) */
		unsigned int j;
		for (j=0; j<4; j++)
		{
			int ifx;
			int ify;
			int irx;
			int iry;
			ifx = vertices[(i)+j].x;
			ify = vertices[(i)+j].y;
			ifx -= center_x;
			ify -= center_y;
//			rx = ((fx * sprite_cos[rot]) - (fy * sprite_sin[rot])) * spr->zoomx;
//			ry = ((fx * sprite_sin[rot]) + (fy * sprite_cos[rot])) * spr->zoomy;
			#if 0
			/* [�ECW]�E�� 0�x�Ŏ��v���̊p�x�n */
			irx = ((ifx * cos_angle) - (ify * sin_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ifx * sin_angle) + (ify * cos_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 1
			/* [��CCW]���� 0�x�Ŕ����v���̊p�x�n */
			irx = ((ify * sin_angle) + (ifx * cos_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ify * cos_angle) - (ifx * sin_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 0
			/* [��CCW(?)]���� 0�x�Ŕ����v���̊p�x�n */
			irx = ((ifx * sin_angle) - (ify * cos_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ifx * cos_angle) + (ify * sin_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
//
			#if (1==USE_ZOOM_XY)
			irx = ((irx * (zoom_x256))>>8);
			iry = ((iry * (zoom_y256))>>8);
			#else //(0==USE_ZOOM_XY)
			irx = ((irx * (zoom_xy256))>>8);
			iry = ((iry * (zoom_xy256))>>8);
			#endif/* (1==USE_ZOOM_XY) */
		//	irx = (irx >>8/*16*/);	//	rx = rx / (256.0/*65536.0*/);
		//	iry = (iry >>8/*16*/);	//	ry = ry / (256.0/*65536.0*/);

			irx += center_x;
			iry += center_y;

			vertices[(i)+j].x = (short)irx;
			vertices[(i)+j].y = (short)iry;
		}
		i += 4;
	}
	/* --- �`�惊�N�G�X�g */
//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (count4), 0, vertices);
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (count4), 0, vertices);
}

/*---------------------------------------------------------
	�t���[���^�C�~���O�ɂ��S�`��
---------------------------------------------------------*/

#if 000
static /*int*/UINT8 texture_cacheId = 127/*-1*/;
static void trans_texture(void)
{
	MY_DIB_SURFACE *texture_cache_ptr;
	//	texture_cache_ptr = NULL/*0*/;
	//	texture_cache_ptr = ggg_Sprites[i].Texture_ptr;
		texture_cache_ptr = my_texture[texture_cacheId];
		/* - �e�N�X�`���]���R�}���h */
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
		#if (1==USE_COLOR_16_AND_32)
		if (/*g_scr.*/ texture_cache_ptr->format->BitsPerPixel == 16)
		{
			sceGuTexMode(GU_PSM_5551, 0, 0, /*1*/(USE_SWIZZLE)/*0 swizzle*/);
		}
		else
		#endif
		{
			sceGuTexMode(GU_PSM_8888, 0, 0, /*1*/(USE_SWIZZLE)/*0 swizzle*/);
		}
		sceGuTexImage(0,
			 512/*texture_cache_ptr->w*/,
			 512/*texture_cache_ptr->h*/,
			 512/*texture_cache_ptr->w*/,
			 texture_cache_ptr->MY_DIB_DATA);
	//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
//test		sceGuTexFilter(GU_LINEAR, GU_LINEAR);
	//	sceGuTexScale(1.0f,1.0f);
	//	sceGuTexOffset(0.0f,0.0f);
}
#endif

//	/*extern*/ int debug_view_objs;
static void blit_all_objects(void /*TGameScreen *pclass*/)
{
//const TGameScreen *pclass =g_scr;

//	debug_view_objs=0;
	/* --- �X�v���C�g�`�� */
	//for (i=0; i<SPRITEMAX; i++)
	offs_t i;
	i = SPRITEMAX;
	do
	{
		i--;/*���F�����*/
		if (/*g_scr.*/ ggg_Sprites[i]./*->*/used == 1/*TR UE*/)
		{
		//	if ( ggg_Sprites[i].texture_id	!= 127/*-1*/)
		//	if ( ggg_Sprites[i].Texture_ptr != NULL)
			{
//				debug_view_objs++;
				/* --- �e�N�X�`���̓]�����K�v�Ȃ� */
#if 000
				{
					if (( ggg_Sprites[i].texture_id  != texture_cacheId) /*||
						( ggg_Sprites[i].Texture_ptr != texture_cache_ptr)*/)
					{
						texture_cacheId   = ggg_Sprites[i].texture_id;
						trans_texture();
					}
				}
#endif
				/* --- �X�v���C�g�̕`�� */
				if (ggg_Sprites[i].used == 1/*TR UE*/)
				{
					//if (ggg_Sprites[i].Texture_ptr != NULL)
					{
						if ((/*g_scr.*/ ggg_Sprites[i]./*->*/rotation_z == 0/*.0*/) &&
							#if (1==USE_ZOOM_XY)
							(/*g_scr.*/ ggg_Sprites[i]./*->*/zoom_x256 == 256/*1.0*/) &&
							(/*g_scr.*/ ggg_Sprites[i]./*->*/zoom_y256 == 256/*1.0*/)
							#else //(0==USE_ZOOM_XY)
							(/*g_scr.*/ ggg_Sprites[i]./*->*/zoom_xy256 == 256/*1.0*/)
							#endif/* (1==USE_ZOOM_XY) */
						)
						{
							/* ��]�g��k���Ȃ��A�P����`�R�s�[ */
							render_object_no_rot_zoom( &ggg_Sprites[i]);
						}
						else
						{
							/* ��]�g��k������ */
							render_object_use_rot_zoom( &ggg_Sprites[i]);
						}
					}
				}
			}
		}
	} while (0 != i);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static	int ggg_SpriteSerial;

/*---------------------------------------------------------
	�C���N�������g�ŃX�v���C�g��n��
---------------------------------------------------------*/

TGameSprite *TGameScreen_GetSpriteSerial(void/*TGameScreen *pclass*/)
{
	if (/*pclass*/ggg_SpriteSerial < SPRITEMAX)
	{
		/*pclass*/ggg_SpriteSerial++;
	}
	return (/*pclass*/ /*g_scr->*/&ggg_Sprites[(/*pclass*/ggg_SpriteSerial - 1)]);
}

/*---------------------------------------------------------
	�X�v���C�g�̑S�N���A
---------------------------------------------------------*/

void TGameScreen_ClearSprite(void/*TGameScreen *pclass*/)
{
	int i;
	for (i=0; i<SPRITEMAX; i++)
	{
		/*pclass*/ /*g_scr->*/ggg_Sprites[i]./*->*/used =  0/*FA LSE*/;
	}
	/*pclass*/ggg_SpriteSerial = 0;
}

/*---------------------------------------------------------

---------------------------------------------------------*/
extern SPRITE *sprite_list000_head;

enum
{
	OBJ_BANK_00_TAMA = 0,
	OBJ_BANK_01_REIMU,
	OBJ_BANK_02_MARISA,
	OBJ_BANK_03_REMILIA,
	OBJ_BANK_04_CIRNO,
	OBJ_BANK_05_YUYUKO,
//	OBJ_BANK_06_ITEM,
	OBJ_BANK_07_FRONT_BANK0,
//	OBJ_BANK_07_FRONT_BANK1,
//	OBJ_BANK_07_FRONT_BANK2,
//	OBJ_BANK_08_PANEL,
//	OBJ_BANK_09_EFFECT, /*[�\��]*/
//	OBJ_BANK_0a_TEKI_STAGE1,
//	OBJ_BANK_0b_TEKI_STAGE2,
//	OBJ_BANK_0c_TEKI_STAGE3,
//	OBJ_BANK_0d_TEKI_STAGE4,
//	OBJ_BANK_0e_TEKI_STAGE5,
//	OBJ_BANK_0f_TEKI_STAGE6,
	OBJ_BANK_MAX
};
#define 	OBJ_BANK_SIZE (8*8)

typedef struct
{
	unsigned char	u;
	unsigned char	v;
//
	unsigned char	w;
	unsigned char	h;
} VIRTUAL_OBJ_STATE;
static VIRTUAL_OBJ_STATE obj_status_table[(OBJ_BANK_MAX*OBJ_BANK_SIZE)/*(6*8*8)*/] =
{
/* [OBJ_BANK_00_TAMA] */
//1
	{  1, 1,  8, 8},// BULLET_MARU8_00_AKA
	{ 10, 1,  8, 8},// BULLET_MARU8_01_YUKARI,
	{ 19, 1,  8, 8},// BULLET_MARU8_02_AOI,
	{ 28, 1,  8, 8},// BULLET_MARU8_03_MIDORI,
	{ 37, 1,  8, 8},// BULLET_MARU8_04_MIDORI,
	{ 46, 1,  8, 8},// BULLET_MARU8_05_MIDORI,
	{ 55, 1,  8, 8},// BULLET_MARU8_06_KIIRO,
	{ 64, 1,  8, 8},// BULLET_MARU8_07_AOI,
//2
	{ 73, 1,  6, 6},// BULLET_MINI8_00_AKA,
	{ 80, 1,  6, 6},// BULLET_MINI8_01_AKA,
	{ 87, 1,  6, 6},// BULLET_MINI8_02_KIIRO,
	{ 94, 1,  6, 6},// BULLET_MINI8_03_AKA,
	{101, 1,  6, 6},// BULLET_MINI8_04_KIIRO,
	{108, 1,  6, 6},// BULLET_MINI8_05_AOI,
	{115, 1,  6, 6},// BULLET_MINI8_06_AOI,
	{122, 1,  6, 6},// BULLET_MINI8_07_YUKARI,
//3
	{  1,10, 12,12},// BULLET_MARU12_00_SIRO,
	{ 14,10, 12,12},// BULLET_MARU12_01_AKA,
	{ 27,10, 12,12},// BULLET_MARU12_02_MIDORI,
	{ 40,10, 12,12},// BULLET_MARU12_03_AOI,
	{ 53,11, 10,10},// BULLET_MARU10_00_AOI,
	{ 64,11, 10,10},// BULLET_MARU10_01_MIDORI,
	{ 75,11, 10,10},// BULLET_MARU10_02_MIDORI,
	{ 86,11, 10,10},// BULLET_MARU10_03_MIDORI,
//4
	{105, 9,  5,32},// BULLET_HARI32_00_AOI
	{111, 9,  5,32},// BULLET_HARI32_01_AKA
	{117, 9,  5,32},// BULLET_HARI32_02_KIIRO
	{123, 9,  5,32},// BULLET_HARI32_03_DAIDAI
	{ 89,42,  9,21},// BULLET_KNIFE20_04_AOI,
	{ 99,42,  9,21},// BULLET_KNIFE20_05_AKA,
	{109,42,  9,21},// BULLET_KNIFE20_06_YUKARI,
	{119,42,  9,21},// BULLET_KNIFE20_07_MIDORI,
//5
	{  1,23, 15,14},// BULLET_UROKO14_00_AOI,
	{ 17,23, 15,14},// BULLET_UROKO14_01_AKA,
	{ 33,23, 15,14},// BULLET_UROKO14_02_YUKARI,
	{ 49,23, 15,14},// BULLET_UROKO14_03_MIDORI,
	{ 65,23, 15,14},// BULLET_UROKO14_04_MIZUIRO,
	{ 81,23, 15,14},// BULLET_UROKO14_05_KIIRO,
	{ 97,10,  7,14},// BULLET_CAP16_04_SIROI
	{ 97,26,  7,14},// BULLET_CAP16_05_SIROI
//6
	{ 49,53,  7,10},// BULLET_KOME_01_AOI,
	{ 57,53,  7,10},// BULLET_KOME_02_AKA,
	{ 65,53,  7,10},// BULLET_KOME_03_YUKARI,
	{ 73,53,  7,10},// BULLET_KOME_04_MIDORI,
	{ 81,53,  7,10},// BULLET_KOME_05_KIIRO,
	{  1,38,  7,15},// BULLET_KUNAI12_06_AOI
	{  9,38,  7,15},// BULLET_KUNAI12_00_AKA,
	{ 17,38,  7,15},// BULLET_KUNAI12_07_MIDORI
//7
	{  9,38,  7,15},// BULLET_KUNAI12_01_AKA,
	{ 25,38,  7,15},// BULLET_KUNAI12_02_MIDORI,
	{ 33,38,  7,15},// BULLET_KUNAI12_03_YUKARI,
	{ 41,38,  7,15},// BULLET_KUNAI12_04_AOI,
	{ 49,38,  9,14},// BULLET_OFUDA12_00_AOI,
	{ 59,38,  9,14},// BULLET_OFUDA12_01_AKA,
	{ 69,38,  9,14},// BULLET_OFUDA12_02_YUKARI,
	{ 79,38,  9,14},// BULLET_OFUDA12_03_MIDORI,
//8
	{  1,64, 32,32},// BULLET_OODAMA32_00_SIROI,
	{ 32,64, 32,32},// BULLET_OODAMA32_01_AOI,
	{ 64,64, 32,32},// BULLET_OODAMA32_02_AKA,
	{ 96,64, 32,32},// BULLET_OODAMA32_03_YUKARI,
	{  1,96, 32,32},// BULLET_OODAMA32_04_MIDORI,
	{ 32,96, 32,32},// BULLET_OODAMA32_05_AOI,
	{ 64,96, 32,32},// BULLET_OODAMA32_06_KIIRO,
	{ 96,96, 32,32},// BULLET_OODAMA32_07_PINK,
//
/* [OBJ_BANK_01_REIMU] */
//[00�얲]
//						//	���ֈړ��̃A�j���[�V����	[���S������4������3������2������1������1������1��...]
// [1]					//	JIKI_PLAYER_00_LEFT 	/* ����1 */ 	/* �ł��� */			= SP_GROUP_JIKI_GET_ITEM/*0x08
	{  0,  0, 31, 31},//	JIKI_PLAYER_01_LEFT,	/* ����2 */
	{ 32,  0, 31, 31},//	JIKI_PLAYER_02_LEFT,	/* ����3 */
	{ 64,  0, 31, 31},//	JIKI_PLAYER_03_LEFT,	/* ����4 */ 	/* ����荶 */
	{ 96,  0, 31, 31},//	�J��Ԃ��A�j���[�V����		[���S1�����S2�����S3�����S4�����S1�����S2�����S3�����S4��...]
	{  0, 32, 31, 31},//	JIKI_PLAYER_04_MID, 	/* ���S1 */
	{ 32, 32, 31, 31},//	JIKI_PLAYER_05_MID, 	/* ���S2 */
	{ 64, 32, 31, 31},//	JIKI_PLAYER_06_MID, 	/* ���S3 */
	{ 96, 32, 31, 31},//	JIKI_PLAYER_07_MID, 	/* ���S4 */
// [2]				//	�E�ֈړ��̃A�j���[�V����	[���S���E��1���E��2���E��3���E��4���E��4���E��4��...]
	{  0, 64, 31, 31},//	JIKI_PLAYER_08_RIGHT,	/* �E��1 */ 	/* �����E */
	{ 32, 64, 31, 31},//	JIKI_PLAYER_09_RIGHT,	/* �E��2 */
	{ 64, 64, 31, 31},//	JIKI_PLAYER_10_RIGHT,	/* �E��3 */
	{ 96, 64, 31, 31},//	JIKI_PLAYER_11_RIGHT,	/* �E��4 */ 	/* �ł��E */	JIKI_ETC_00,		/* ���[�U�[1 */
	{  1, 64, 32, 32},//	JIKI_ETC_00,		/* ���[�U�[1 */
	{  1, 64, 32, 32},//	JIKI_ETC_01,		/* ���[�U�[2 */
	{  1, 64, 32, 32},//	JIKI_ETC_02,		/* ���[�U�[3 */
	{  1, 64, 32, 32},//	JIKI_ETC_03,		/* ���[�U�[4 */
// [3]
	{  1, 64, 32, 32},//	JIKI_BOMBER_00, 	/* ���[�U�[5 */
	{  1, 64, 32, 32},//	JIKI_BOMBER_01, 	/* ���[�U�[6 */
	{222,  0, 32, 32},//	JIKI_BOMBER_02, 	/* ��1 / ��1 / �\����1 */
	{222, 32, 32, 32},//	JIKI_BOMBER_03, 	/* ��2 / ��2 / �\����2 */
	{222, 64, 32, 32},//	JIKI_BOMBER_04, 	/* ��3 / ��3 / �\����3 */
	{222, 96, 32, 32},//	JIKI_BOMBER_05, 	/* ��4 / ��4 / �\����4 */
	{  0,128,127,127},//	JIKI_BOMBER_06, 	/* ��5 / ��5 / ����1 / ���E�� */
	{128,128,127,127},//	JIKI_BOMBER_07, 	/* ��6 / ��6 / ����2 / ���E��  */
// [4]
	{222, 96, 32, 32},//	JIKI_SHOT_00,		/* ����3 / �ԎDC */
	{222, 96, 32, 32},//	JIKI_SHOT_01,		/* ����4 / �ԎDB(�얲�̉�]�V���b�g) */
	{128,  0, 10, 36},//	JIKI_SHOT_02,		/* �ԎDA / ���� / ���� / �X / �� */
	{  1, 64, 32, 32},//	JIKI_SHOT_03,		/* ��������e1 / ���D1 */
	{  1, 64, 32, 32},//	JIKI_SHOT_04,		/* ��������e2 / ���D2 */
	{  1, 64, 32, 32},//	JIKI_SHOT_05,		/* ��������e3 / ���D3 */
	{  1, 64, 32, 32},//	JIKI_SHOT_06,		/* ��������e4 / ���D4 */
	{139,  0,  5, 32},//	JIKI_SHOT_07,		/* ��������e5 / �j�e / �X�e / �E�F�C�u�e / �s���N���e */
// [5]
	{  1,116, 11, 11},//	JIKI_OPTION_00_00,
	{  1,116, 11, 11},//	JIKI_OPTION_00_01,
	{  1,116, 11, 11},//	JIKI_OPTION_00_02,
	{  1,116, 11, 11},//	JIKI_OPTION_00_03,
	{  1,116, 11, 11},//	JIKI_OPTION_00_04,
	{  1,116, 11, 11},//	JIKI_OPTION_00_05,
	{  1,116, 11, 11},//	JIKI_OPTION_00_06,
	{  1,116, 11, 11},//	JIKI_OPTION_00_07,
// [6]
	{  1,116, 11, 11},//	JIKI_OPTION_01_00,
	{  1,116, 11, 11},//	JIKI_OPTION_01_01,
	{  1,116, 11, 11},//	JIKI_OPTION_01_02,
	{  1,116, 11, 11},//	JIKI_OPTION_01_03,
	{  1,116, 11, 11},//	JIKI_OPTION_01_04,
	{  1,116, 11, 11},//	JIKI_OPTION_01_05,
	{  1,116, 11, 11},//	JIKI_OPTION_01_06,
	{  1,116, 11, 11},//	JIKI_OPTION_01_07,
// [7]
	{  1,116, 11, 11},//	JIKI_OPTION_02_00,
	{  1,116, 11, 11},//	JIKI_OPTION_02_01,
	{  1,116, 11, 11},//	JIKI_OPTION_02_02,
	{  1,116, 11, 11},//	JIKI_OPTION_02_03,
	{  1,116, 11, 11},//	JIKI_OPTION_02_04,
	{  1,116, 11, 11},//	JIKI_OPTION_02_05,
	{  1,116, 11, 11},//	JIKI_OPTION_02_06,
	{  1,116, 11, 11},//	JIKI_OPTION_02_07,
// [8]
	{  1,116, 11, 11},//	JIKI_OPTION_03_00,
	{  1,116, 11, 11},//	JIKI_OPTION_03_01,
	{  1,116, 11, 11},//	JIKI_OPTION_03_02,
	{  1,116, 11, 11},//	JIKI_OPTION_03_03,
	{  1,116, 11, 11},//	JIKI_OPTION_03_04,
	{  1,116, 11, 11},//	JIKI_OPTION_03_05,
	{  1,116, 11, 11},//	JIKI_OPTION_03_06,
	{  1,116, 11, 11},//	JIKI_OPTION_03_07,

//
/* [OBJ_BANK_02_MARISA] */
// [01������]
//					//	���ֈړ��̃A�j���[�V����	[���S������4������3������2������1������1������1��...]
// [1]				//	JIKI_PLAYER_00_LEFT 	/* ����1 */ 	/* �ł��� */			= SP_GROUP_JIKI_GET_ITEM/*0x08
	{  0,  0, 31, 31},//	JIKI_PLAYER_01_LEFT,	/* ����2 */
	{ 32,  0, 31, 31},//	JIKI_PLAYER_02_LEFT,	/* ����3 */
	{ 64,  0, 31, 31},//	JIKI_PLAYER_03_LEFT,	/* ����4 */ 	/* ����荶 */
	{ 96,  0, 31, 31},//	�J��Ԃ��A�j���[�V����		[���S1�����S2�����S3�����S4�����S1�����S2�����S3�����S4��...]
	{  0, 32, 31, 31},//	JIKI_PLAYER_04_MID, 	/* ���S1 */
	{ 32, 32, 31, 31},//	JIKI_PLAYER_05_MID, 	/* ���S2 */
	{ 64, 32, 31, 31},//	JIKI_PLAYER_06_MID, 	/* ���S3 */
	{ 96, 32, 31, 31},//	JIKI_PLAYER_07_MID, 	/* ���S4 */
// [2]				//	�E�ֈړ��̃A�j���[�V����	[���S���E��1���E��2���E��3���E��4���E��4���E��4��...]
	{  0, 64, 31, 31},//	JIKI_PLAYER_08_RIGHT,	/* �E��1 */ 	/* �����E */
	{ 32, 64, 31, 31},//	JIKI_PLAYER_09_RIGHT,	/* �E��2 */
	{ 64, 64, 31, 31},//	JIKI_PLAYER_10_RIGHT,	/* �E��3 */
	{ 96, 64, 31, 31},//	JIKI_PLAYER_11_RIGHT,	/* �E��4 */ 	/* �ł��E */	JIKI_ETC_00,		/* ���[�U�[1 */
	{  1, 64, 32, 32},//	JIKI_ETC_00,		/* ���[�U�[1 */
	{  1, 64, 32, 32},//	JIKI_ETC_01,		/* ���[�U�[2 */
	{  1, 64, 32, 32},//	JIKI_ETC_02,		/* ���[�U�[3 */
	{  1, 64, 32, 32},//	JIKI_ETC_03,		/* ���[�U�[4 */
// [3]
	{  1, 64, 32, 32},//	JIKI_BOMBER_00, 	/* ���[�U�[5 */
	{  1, 64, 32, 32},//	JIKI_BOMBER_01, 	/* ���[�U�[6 */
	{169,  0, 86, 81},//	JIKI_BOMBER_02, 	/* ��1 / ��1 / �\����1 */
	{169, 82, 86, 81},//	JIKI_BOMBER_03, 	/* ��2 / ��2 / �\����2 */
	{169,164, 86, 81},//	JIKI_BOMBER_04, 	/* ��3 / ��3 / �\����3 */
	{169,  0, 86, 81},//	JIKI_BOMBER_05, 	/* ��4 / ��4 / �\����4 */
	{169, 82, 86, 81},//	JIKI_BOMBER_06, 	/* ��5 / ��5 / ����1 / ���E�� */
	{169,164, 86, 81},//	JIKI_BOMBER_07, 	/* ��6 / ��6 / ����2 / ���E��  */
// [4]
	{  1, 64, 32, 32},//	JIKI_SHOT_00,		/* ����3 */
	{  1, 64, 32, 32},//	JIKI_SHOT_01,		/* ����4 */
	{128,  0, 10, 32},//	JIKI_SHOT_02,		/* �ԎD / ���� / ���� / �X / �� */
	{  1, 64, 32, 32},//	JIKI_SHOT_03,		/* ��������e1 / ���D1 */
	{  1, 64, 32, 32},//	JIKI_SHOT_04,		/* ��������e2 / ���D2 */
	{  1, 64, 32, 32},//	JIKI_SHOT_05,		/* ��������e3 / ���D3 */
	{  1, 64, 32, 32},//	JIKI_SHOT_06,		/* ��������e4 / ���D4 */
	{139,  0, 15, 18},//	JIKI_SHOT_07,		/* ��������e5 / �j�e / �X�e / �E�F�C�u�e / �s���N���e */
// [5]
	{  0,120,  9,  9},//	JIKI_OPTION_00_00,
	{  0,120,  9,  9},//	JIKI_OPTION_00_01,
	{  0,120,  9,  9},//	JIKI_OPTION_00_02,
	{  0,120,  9,  9},//	JIKI_OPTION_00_03,
	{  0,120,  9,  9},//	JIKI_OPTION_00_04,
	{  0,120,  9,  9},//	JIKI_OPTION_00_05,
	{  0,120,  9,  9},//	JIKI_OPTION_00_06,
	{  0,120,  9,  9},//	JIKI_OPTION_00_07,
// [6]
	{  0,120,  9,  9},//	JIKI_OPTION_01_00,
	{  0,120,  9,  9},//	JIKI_OPTION_01_01,
	{  0,120,  9,  9},//	JIKI_OPTION_01_02,
	{  0,120,  9,  9},//	JIKI_OPTION_01_03,
	{  0,120,  9,  9},//	JIKI_OPTION_01_04,
	{  0,120,  9,  9},//	JIKI_OPTION_01_05,
	{  0,120,  9,  9},//	JIKI_OPTION_01_06,
	{  0,120,  9,  9},//	JIKI_OPTION_01_07,
// [7]
	{  0,120,  9,  9},//	JIKI_OPTION_02_00,
	{  0,120,  9,  9},//	JIKI_OPTION_02_01,
	{  0,120,  9,  9},//	JIKI_OPTION_02_02,
	{  0,120,  9,  9},//	JIKI_OPTION_02_03,
	{  0,120,  9,  9},//	JIKI_OPTION_02_04,
	{  0,120,  9,  9},//	JIKI_OPTION_02_05,
	{  0,120,  9,  9},//	JIKI_OPTION_02_06,
	{  0,120,  9,  9},//	JIKI_OPTION_02_07,
// [8]
	{  0,120,  9,  9},//	JIKI_OPTION_03_00,
	{  0,120,  9,  9},//	JIKI_OPTION_03_01,
	{  0,120,  9,  9},//	JIKI_OPTION_03_02,
	{  0,120,  9,  9},//	JIKI_OPTION_03_03,
	{  0,120,  9,  9},//	JIKI_OPTION_03_04,
	{  0,120,  9,  9},//	JIKI_OPTION_03_05,
	{  0,120,  9,  9},//	JIKI_OPTION_03_06,
	{  0,120,  9,  9},//	JIKI_OPTION_03_07,

//
/* [OBJ_BANK_03_REMILIA] */
// [02���~]
//					//	���ֈړ��̃A�j���[�V����	[���S������4������3������2������1������1������1��...]
// [1]				//	JIKI_PLAYER_00_LEFT 	/* ����1 */ 	/* �ł��� */			= SP_GROUP_JIKI_GET_ITEM/*0x08
	{  0,  0, 31, 31},//	JIKI_PLAYER_01_LEFT,	/* ����2 */
	{ 32,  0, 32, 31},//	JIKI_PLAYER_02_LEFT,	/* ����3 */
	{ 64,  0, 31, 31},//	JIKI_PLAYER_03_LEFT,	/* ����4 */ 	/* ����荶 */
	{ 96,  0, 31, 31},//	�J��Ԃ��A�j���[�V����		[���S1�����S2�����S3�����S4�����S1�����S2�����S3�����S4��...]
	{  0, 32, 31, 31},//	JIKI_PLAYER_04_MID, 	/* ���S1 */
	{ 32, 32, 31, 31},//	JIKI_PLAYER_05_MID, 	/* ���S2 */
	{ 64, 32, 31, 31},//	JIKI_PLAYER_06_MID, 	/* ���S3 */
	{ 96, 32, 31, 31},//	JIKI_PLAYER_07_MID, 	/* ���S4 */
// [2]				//	�E�ֈړ��̃A�j���[�V����	[���S���E��1���E��2���E��3���E��4���E��4���E��4��...]
	{  0, 64, 31, 31},//	JIKI_PLAYER_08_RIGHT,	/* �E��1 */ 	/* �����E */
	{ 32, 64, 31, 31},//	JIKI_PLAYER_09_RIGHT,	/* �E��2 */
	{ 64, 64, 31, 31},//	JIKI_PLAYER_10_RIGHT,	/* �E��3 */
	{ 96, 64, 31, 31},//	JIKI_PLAYER_11_RIGHT,	/* �E��4 */ 	/* �ł��E */	JIKI_ETC_00,		/* ���[�U�[1 */
	{  1, 64, 32, 32},//	JIKI_ETC_00,		/* ���[�U�[1 */
	{  1, 64, 32, 32},//	JIKI_ETC_01,		/* ���[�U�[2 */
	{  1, 64, 32, 32},//	JIKI_ETC_02,		/* ���[�U�[3 */
	{  1, 64, 32, 32},//	JIKI_ETC_03,		/* ���[�U�[4 */
// [3]
	{  1, 64, 32, 32},//	JIKI_BOMBER_00, 	/* ���[�U�[5 */
	{  1, 64, 32, 32},//	JIKI_BOMBER_01, 	/* ���[�U�[6 */
	{ 16,128,  4, 16},//	JIKI_BOMBER_02, 	/* ��1 / ��1 / �c��A1 / �\����1 */
	{ 32,128,  4, 16},//	JIKI_BOMBER_03, 	/* ��2 / ��2 / �c��A2 / �\����2 */
	{ 48,128,  4, 16},//	JIKI_BOMBER_04, 	/* ��3 / ��3 / �c��A3 / �\����3 */
	{ 64,128,  4, 16},//	JIKI_BOMBER_05, 	/* ��4 / ��4 / �c��A4 / �\����4 */
	{ 80,128,  4, 16},//	JIKI_BOMBER_06, 	/* ��5 / ��5 / �c��B1 / ����1 / ���E�� */
	{ 96,128,  4, 16},//	JIKI_BOMBER_07, 	/* ��6 / ��6 / �c��B2 / ����2 / ���E��	*/
// [4]
	{ 96,128,  4, 16},//	JIKI_SHOT_00,		/* �c��B3 / ����3 */
	{112,128,  4, 16},//	JIKI_SHOT_01,		/* �c��B4 / ����4 */
	{128,  0,  6, 26},//	JIKI_SHOT_02,		/* �ԎD / ���� / ���� / �X / �� */
	{  1, 64, 32, 32},//	JIKI_SHOT_03,		/* ��������e1 / ���D1 */
	{  1, 64, 32, 32},//	JIKI_SHOT_04,		/* ��������e2 / ���D2 */
	{  1, 64, 32, 32},//	JIKI_SHOT_05,		/* ��������e3 / ���D3 */
	{  1, 64, 32, 32},//	JIKI_SHOT_06,		/* ��������e4 / ���D4 */
	{135,  0, 10, 10},//	JIKI_SHOT_07,		/* ��������e5 / �j�e / �X�e / �E�F�C�u�e / �s���N���e */
// [5]
	{  0, 99, 13, 13},//	JIKI_OPTION_00_00,
	{  0, 99, 13, 13},//	JIKI_OPTION_00_01,
	{  0, 99, 13, 13},//	JIKI_OPTION_00_02,
	{  0, 99, 13, 13},//	JIKI_OPTION_00_03,
	{  0, 99, 13, 13},//	JIKI_OPTION_00_04,
	{  0, 99, 13, 13},//	JIKI_OPTION_00_05,
	{  0, 99, 13, 13},//	JIKI_OPTION_00_06,
	{  0, 99, 13, 13},//	JIKI_OPTION_00_07,
// [6]
	{  0, 99, 13, 13},//	JIKI_OPTION_01_00,
	{  0, 99, 13, 13},//	JIKI_OPTION_01_01,
	{  0, 99, 13, 13},//	JIKI_OPTION_01_02,
	{  0, 99, 13, 13},//	JIKI_OPTION_01_03,
	{  0, 99, 13, 13},//	JIKI_OPTION_01_04,
	{  0, 99, 13, 13},//	JIKI_OPTION_01_05,
	{  0, 99, 13, 13},//	JIKI_OPTION_01_06,
	{  0, 99, 13, 13},//	JIKI_OPTION_01_07,
// [7]
	{  0, 99, 13, 13},//	JIKI_OPTION_02_00,
	{  0, 99, 13, 13},//	JIKI_OPTION_02_01,
	{  0, 99, 13, 13},//	JIKI_OPTION_02_02,
	{  0, 99, 13, 13},//	JIKI_OPTION_02_03,
	{  0, 99, 13, 13},//	JIKI_OPTION_02_04,
	{  0, 99, 13, 13},//	JIKI_OPTION_02_05,
	{  0, 99, 13, 13},//	JIKI_OPTION_02_06,
	{  0, 99, 13, 13},//	JIKI_OPTION_02_07,
// [8]
	{  0, 99, 13, 13},//	JIKI_OPTION_03_00,
	{  0, 99, 13, 13},//	JIKI_OPTION_03_01,
	{  0, 99, 13, 13},//	JIKI_OPTION_03_02,
	{  0, 99, 13, 13},//	JIKI_OPTION_03_03,
	{  0, 99, 13, 13},//	JIKI_OPTION_03_04,
	{  0, 99, 13, 13},//	JIKI_OPTION_03_05,
	{  0, 99, 13, 13},//	JIKI_OPTION_03_06,
	{  0, 99, 13, 13},//	JIKI_OPTION_03_07,

//
/* [OBJ_BANK_04_CIRNO] */
// [03�`���m]
//					//	���ֈړ��̃A�j���[�V����	[���S������4������3������2������1������1������1��...]
// [1]				//	JIKI_PLAYER_00_LEFT 	/* ����1 */ 	/* �ł��� */			= SP_GROUP_JIKI_GET_ITEM/*0x08
	{  0,  0, 31, 31},//	JIKI_PLAYER_01_LEFT,	/* ����2 */
	{ 32,  0, 31, 31},//	JIKI_PLAYER_02_LEFT,	/* ����3 */
	{ 64,  0, 31, 31},//	JIKI_PLAYER_03_LEFT,	/* ����4 */ 	/* ����荶 */
	{ 96,  0, 31, 31},//	�J��Ԃ��A�j���[�V����		[���S1�����S2�����S3�����S4�����S1�����S2�����S3�����S4��...]
	{  0, 32, 31, 31},//	JIKI_PLAYER_04_MID, 	/* ���S1 */
	{ 32, 32, 31, 31},//	JIKI_PLAYER_05_MID, 	/* ���S2 */
	{ 64, 32, 31, 31},//	JIKI_PLAYER_06_MID, 	/* ���S3 */
	{ 96, 32, 31, 31},//	JIKI_PLAYER_07_MID, 	/* ���S4 */
// [2]				//	�E�ֈړ��̃A�j���[�V����	[���S���E��1���E��2���E��3���E��4���E��4���E��4��...]
	{  0, 64, 31, 31},//	JIKI_PLAYER_08_RIGHT,	/* �E��1 */ 	/* �����E */
	{ 32, 64, 31, 31},//	JIKI_PLAYER_09_RIGHT,	/* �E��2 */
	{ 64, 64, 31, 31},//	JIKI_PLAYER_10_RIGHT,	/* �E��3 */
	{ 96, 64, 31, 31},//	JIKI_PLAYER_11_RIGHT,	/* �E��4 */ 	/* �ł��E */	JIKI_ETC_00,		/* ���[�U�[1 */
	{  1, 64, 32, 32},//	JIKI_ETC_00,		/* ���[�U�[1 */
	{  1, 64, 32, 32},//	JIKI_ETC_01,		/* ���[�U�[2 */
	{  1, 64, 32, 32},//	JIKI_ETC_02,		/* ���[�U�[3 */
	{  1, 64, 32, 32},//	JIKI_ETC_03,		/* ���[�U�[4 */
// [3]
	{  1, 64, 32, 32},//	JIKI_BOMBER_00, 	/* ���[�U�[5 */
	{  1, 64, 32, 32},//	JIKI_BOMBER_01, 	/* ���[�U�[6 */
	{169,  0, 86, 85},//	JIKI_BOMBER_02, 	/* ��1 / ��1 / �\����1 */
	{169, 86, 86, 84},//	JIKI_BOMBER_03, 	/* ��2 / ��2 / �\����2 */
	{169,171, 86, 84},//	JIKI_BOMBER_04, 	/* ��3 / ��3 / �\����3 */
	{169,  0, 86, 85},//	JIKI_BOMBER_05, 	/* ��4 / ��4 / �\����4 */
	{169, 86, 86, 84},//	JIKI_BOMBER_06, 	/* ��5 / ��5 / ����1 / ���E�� */
	{169,171, 86, 84},//	JIKI_BOMBER_07, 	/* ��6 / ��6 / ����2 / ���E��  */
// [4]
	{  1, 64, 32, 32},//	JIKI_SHOT_00,		/* ����3 */
	{  1, 64, 32, 32},//	JIKI_SHOT_01,		/* ����4 */
	{128,  0,  9, 34},//	JIKI_SHOT_02,		/* �ԎD / ���� / ���� / �X / �� */
	{  1, 64, 32, 32},//	JIKI_SHOT_03,		/* ��������e1 / ���D1 */
	{  1, 64, 32, 32},//	JIKI_SHOT_04,		/* ��������e2 / ���D2 */
	{  1, 64, 32, 32},//	JIKI_SHOT_05,		/* ��������e3 / ���D3 */
	{  1, 64, 32, 32},//	JIKI_SHOT_06,		/* ��������e4 / ���D4 */
	{138,  0, 15, 14},//	JIKI_SHOT_07,		/* ��������e5 / �j�e / �X�e / �E�F�C�u�e / �s���N���e */
// [5]
	{  0,128, 16, 15},//	JIKI_OPTION_00_00,
	{ 16,128, 16, 15},//	JIKI_OPTION_00_01,
	{ 32,128, 16, 15},//	JIKI_OPTION_00_02,
	{ 48,128, 16, 15},//	JIKI_OPTION_00_03,
	{ 64,128, 16, 15},//	JIKI_OPTION_00_04,
	{ 80,128, 16, 15},//	JIKI_OPTION_00_05,
	{ 96,128, 16, 15},//	JIKI_OPTION_00_06,
	{112,128, 16, 15},//	JIKI_OPTION_00_07,
// [6]
	{  0,144, 16, 15},//	JIKI_OPTION_01_00,
	{ 16,144, 16, 15},//	JIKI_OPTION_01_01,
	{ 32,144, 16, 15},//	JIKI_OPTION_01_02,
	{ 48,144, 16, 15},//	JIKI_OPTION_01_03,
	{ 64,144, 16, 15},//	JIKI_OPTION_01_04,
	{ 80,144, 16, 15},//	JIKI_OPTION_01_05,
	{ 96,144, 16, 15},//	JIKI_OPTION_01_06,
	{112,144, 16, 15},//	JIKI_OPTION_01_07,
// [7]
	{  0,160, 16, 15},//	JIKI_OPTION_02_00,
	{ 16,160, 16, 15},//	JIKI_OPTION_02_01,
	{ 32,160, 16, 15},//	JIKI_OPTION_02_02,
	{ 48,160, 16, 15},//	JIKI_OPTION_02_03,
	{ 64,160, 16, 15},//	JIKI_OPTION_02_04,
	{ 80,160, 16, 15},//	JIKI_OPTION_02_05,
	{ 96,160, 16, 15},//	JIKI_OPTION_02_06,
	{112,160, 16, 15},//	JIKI_OPTION_02_07,
// [8]
	{  0,176, 16, 15},//	JIKI_OPTION_03_00,
	{ 16,176, 16, 15},//	JIKI_OPTION_03_01,
	{ 32,176, 16, 15},//	JIKI_OPTION_03_02,
	{ 48,176, 16, 15},//	JIKI_OPTION_03_03,
	{ 64,176, 16, 15},//	JIKI_OPTION_03_04,
	{ 80,176, 16, 15},//	JIKI_OPTION_03_05,
	{ 96,176, 16, 15},//	JIKI_OPTION_03_06,
	{112,176, 16, 15},//	JIKI_OPTION_03_07,

//
/* [OBJ_BANK_05_YUYUKO] */
// [04�H�X�q]
//					//	���ֈړ��̃A�j���[�V����	[���S������4������3������2������1������1������1��...]
// [1]				//	JIKI_PLAYER_00_LEFT 	/* ����1 */ 	/* �ł��� */			= SP_GROUP_JIKI_GET_ITEM/*0x08
	{  0,  0, 31, 31},//	JIKI_PLAYER_01_LEFT,	/* ����2 */
	{ 32,  0, 31, 31},//	JIKI_PLAYER_02_LEFT,	/* ����3 */
	{ 64,  0, 31, 31},//	JIKI_PLAYER_03_LEFT,	/* ����4 */ 	/* ����荶 */
	{ 96,  0, 31, 31},//	�J��Ԃ��A�j���[�V����		[���S1�����S2�����S3�����S4�����S1�����S2�����S3�����S4��...]
	{  0, 32, 31, 31},//	JIKI_PLAYER_04_MID, 	/* ���S1 */
	{ 32, 32, 31, 31},//	JIKI_PLAYER_05_MID, 	/* ���S2 */
	{ 64, 32, 31, 31},//	JIKI_PLAYER_06_MID, 	/* ���S3 */
	{ 96, 32, 31, 31},//	JIKI_PLAYER_07_MID, 	/* ���S4 */
// [2]				//	�E�ֈړ��̃A�j���[�V����	[���S���E��1���E��2���E��3���E��4���E��4���E��4��...]
	{  0, 64, 31, 31},//	JIKI_PLAYER_08_RIGHT,	/* �E��1 */ 	/* �����E */
	{ 32, 64, 31, 31},//	JIKI_PLAYER_09_RIGHT,	/* �E��2 */
	{ 64, 64, 31, 31},//	JIKI_PLAYER_10_RIGHT,	/* �E��3 */
	{ 96, 64, 31, 31},//	JIKI_PLAYER_11_RIGHT,	/* �E��4 */ 	/* �ł��E */	JIKI_ETC_00,		/* ���[�U�[1 */
	{  1, 64, 32, 32},//	JIKI_ETC_00,		/* ���[�U�[1 */
	{  1, 64, 32, 32},//	JIKI_ETC_01,		/* ���[�U�[2 */
	{  1, 64, 32, 32},//	JIKI_ETC_02,		/* ���[�U�[3 */
	{  1, 64, 32, 32},//	JIKI_ETC_03,		/* ���[�U�[4 */
// [3]
	{  1, 64, 32, 32},//	JIKI_BOMBER_00, 	/* ���[�U�[5 */
	{  1, 64, 32, 32},//	JIKI_BOMBER_01, 	/* ���[�U�[6 */
	{168,  0, 46, 37},//	JIKI_BOMBER_02, 	/* ��1 / ��1 / �\����1 */
	{168, 40, 46, 37},//	JIKI_BOMBER_03, 	/* ��2 / ��2 / �\����2 */
	{168,  0, 46, 37},//	JIKI_BOMBER_04, 	/* ��3 / ��3 / �\����3 */
	{168, 40, 46, 37},//	JIKI_BOMBER_05, 	/* ��4 / ��4 / �\����4 */
	{168,  0, 46, 37},//	JIKI_BOMBER_06, 	/* ��5 / ��5 / ����1 / ���E�� */
	{168, 40, 46, 37},//	JIKI_BOMBER_07, 	/* ��6 / ��6 / ����2 / ���E��  */
// [4]
	{  0,151,197,104},//	JIKI_SHOT_00,		/* ����3 / [�H�X�q�{���̐�{��] */
	{  1, 64, 32, 32},//	JIKI_SHOT_01,		/* ����4 */
	{128,  0, 18, 31},//	JIKI_SHOT_02,		/* �ԎD / ���� / ���� / �X / �� */
	{  1, 64, 32, 32},//	JIKI_SHOT_03,		/* ��������e1 / ���D1 */
	{  1, 64, 32, 32},//	JIKI_SHOT_04,		/* ��������e2 / ���D2 */
	{  1, 64, 32, 32},//	JIKI_SHOT_05,		/* ��������e3 / ���D3 */
	{  1, 64, 32, 32},//	JIKI_SHOT_06,		/* ��������e4 / ���D4 */
	{147,  0, 20, 38},//	JIKI_SHOT_07,		/* ��������e5 / �j�e / �X�e / �E�F�C�u�e / �s���N���e */
// [5]
	{  0,112, 15, 15},//	JIKI_OPTION_00_00,
	{ 16,112, 15, 15},//	JIKI_OPTION_00_01,
	{ 32,112, 15, 15},//	JIKI_OPTION_00_02,
	{ 48,112, 15, 15},//	JIKI_OPTION_00_03,
	{ 64,112, 15, 15},//	JIKI_OPTION_00_04,
	{ 80,112, 15, 15},//	JIKI_OPTION_00_05,
	{ 96,112, 15, 15},//	JIKI_OPTION_00_06,
	{112,112, 15, 15},//	JIKI_OPTION_00_07,
// [6]
	{  0,112, 15, 15},//	JIKI_OPTION_01_00,
	{  0,112, 15, 15},//	JIKI_OPTION_01_01,
	{  0,112, 15, 15},//	JIKI_OPTION_01_02,
	{  0,112, 15, 15},//	JIKI_OPTION_01_03,
	{  0,112, 15, 15},//	JIKI_OPTION_01_04,
	{  0,112, 15, 15},//	JIKI_OPTION_01_05,
	{  0,112, 15, 15},//	JIKI_OPTION_01_06,
	{  0,112, 15, 15},//	JIKI_OPTION_01_07,
// [7]
	{  0,112, 15, 15},//	JIKI_OPTION_02_00,
	{  0,112, 15, 15},//	JIKI_OPTION_02_01,
	{  0,112, 15, 15},//	JIKI_OPTION_02_02,
	{  0,112, 15, 15},//	JIKI_OPTION_02_03,
	{  0,112, 15, 15},//	JIKI_OPTION_02_04,
	{  0,112, 15, 15},//	JIKI_OPTION_02_05,
	{  0,112, 15, 15},//	JIKI_OPTION_02_06,
	{  0,112, 15, 15},//	JIKI_OPTION_02_07,
// [8]
	{  0,112, 15, 15},//	JIKI_OPTION_03_00,
	{  0,112, 15, 15},//	JIKI_OPTION_03_01,
	{  0,112, 15, 15},//	JIKI_OPTION_03_02,
	{  0,112, 15, 15},//	JIKI_OPTION_03_03,
	{  0,112, 15, 15},//	JIKI_OPTION_03_04,
	{  0,112, 15, 15},//	JIKI_OPTION_03_05,
	{  0,112, 15, 15},//	JIKI_OPTION_03_06,
	{  0,112, 15, 15},//	JIKI_OPTION_03_07,

/* [OBJ_BANK_06_ITEM] */


/* --- �t�����g�� --- */
/* [OBJ_BANK_07_FRONT_BANK0] */
// [1]
	{  1,  1, 28, 28},//	J,
	{ 30,  1, 28, 28},//	J,
	{  1, 30, 28, 28},//	J,
	{ 30, 30, 28, 28},//	J,
	{  1, 60, 28, 28},//	J,
	{ 30, 60, 28, 28},//	J,
	{  1,  1, 28, 28},//	J,
	{  1,  1, 28, 28},//	J,
// [2]
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
// [3]
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
// [4]
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
// [5]
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
// [6]
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
// [7]
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
// [8]
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,
	{  0,112, 15, 15},//	J,

/* [OBJ_BANK_08_PANEL] */

/* --- �����w --- */
/* [OBJ_BANK_09_EFFECT] */	/*[�\��]*/


/* [OBJ_BANK_0a_TEKI_STAGE1] */
/* [OBJ_BANK_0b_TEKI_STAGE2] */
/* [OBJ_BANK_0c_TEKI_STAGE3] */
/* [OBJ_BANK_0d_TEKI_STAGE4] */
/* [OBJ_BANK_0e_TEKI_STAGE5] */
/* [OBJ_BANK_0f_TEKI_STAGE6] */

};

void common_transfer_objects(int num, int obj_group, VIRTUAL_OBJ_STATE *head_obj_status_table)
{
	TGameSprite *obj;
	SPRITE *sss = sprite_list000_head;/* �X�v���C�g ���X�g�̐擪 ����T�� */
	while (NULL != sss)/* �X�v���C�g ���X�g�̍Ō�܂Œ��ׂ� */
	{
		if (/*SP_BULLET*/obj_group/*SP_GROUP_BULLETS*/ & sss->type)
		{
			/* --- �I�u�W�F�N�g�|�C���^�󂯎�� */
			obj 				= TGameScreen_GetSpriteSerial();
			obj->used			= 1;					/*TR UE*/
		//	obj->alpha			= sss->alpha;			/* ���l(0xff==255 �ŕs�����A0 �œ���) */
			obj->color8888		= sss->color32; 		/* ���l(0xff==255 �ŕs�����A0 �œ���) */
			obj->texture_id 	= TEX_06_BULLET;//sss->m_texture_id ;

			/* --- �I�u�W�F�N�g�\���ݒ� */
			obj->x256			= ((int)sss->x256); 	/*>>8*/ /*/(2 *256)*/  /*+(56)*/ /*+ 80*/
			obj->y256			= ((int)sss->y256); 	/*>>8*/ /*/(2 *256)*/  /*+ 16*/
		//	if (sss->m_PosCenter == 1/*TR UE*/)
		//	{
		//		obj->x -= (sss->w / 2);
		//		obj->y -= (sss->h / 2);
		//	}
			{
				int tama_index = (SP_GROUP_SUB_TYPE_128 & sss->type);
				obj->w			= head_obj_status_table[tama_index].w;		/*((sss->w128)>>7)*/
				obj->h			= head_obj_status_table[tama_index].h;		/*((sss->h128)>>7)*/
				obj->tx 		= head_obj_status_table[tama_index].u;		/*1*/	//sss->texture_x;
				obj->ty 		= head_obj_status_table[tama_index].v;		//sss->texture_y;
			}
			#if (1==USE_ZOOM_XY)
			obj->zoom_x256	= sss->m_zoom_xy256;		/* �g�嗦 0x100==256 �� x1.0�{ */
			obj->zoom_y256	= sss->m_zoom_xy256;		/* �g�嗦 0x100==256 �� x1.0�{ */
			#else //(0==USE_ZOOM_XY)
		//	obj->zoom_xy256 = 256;//sss->m_zoom_xy256;		/* �g�嗦 0x100==256 �� x1.0�{ */
			obj->zoom_xy256 = sss->m_zoom_x256; 			//sss->m_zoom_xy256;	/* �g�嗦 0x100==256 �� x1.0�{ */
			#endif/* (1==USE_ZOOM_XY) */
			/* �`��p�p�x(����0�x�ō����(�����v���)) */
			obj->rotation_z = (sss->m_angleCCW512/*<<(7)*/);	/*m_RollZ*/ 	/* �p�x��0-512�x */ 	/* �p�x��0-65535�x */
//
			#if (1==USE_ZBUFFER)
			obj->priority	= sss->m_priority;			/*aaa*/ 	/* �\���D�揇�� */
			#endif/* (1==USE_ZBUFFER) */
		//	s->type = SP_DELETE;
		}
		sss = sss->next;/*��*/
	}
}

/*---------------------------------------------------------
	�w�i(���쒆)
	-------------------------------------------------------
---------------------------------------------------------*/
extern /*static*/ int current_bg0_y_scroll_speed256;	/* bg0�̃X�N���[���A���ݑ��x */
/*	int val1 = 0;*/

static int val3 /*= 0*/;
static int val3_256 /*= 0*/;

/*---------------------------------------------------------
	�[��3D�w�i(���쒆)
	-------------------------------------------------------
	���x���Ԃɍ����̂Ȃ�AGum��3D�ɂ��������A���݉��f�ށB
	Gum�Ɉڍs���Ă����炩�̃T�|�[�g��Gu�͗v�邩���m��Ȃ��B(��ʂ̒[�����̕��Ƃ�)
	(3D�|���S���ł��ׂĖ��ߐs�����̂͑��x�I�ȃR�X�g�����������m��Ȃ��B����ĂȂ����画��܂���)
---------------------------------------------------------*/

		#if 1//(1==USE_F3D)
static void gu_draw_bg_fake3D(void)
{
//	#define GS012V (5*2)
	#define GS012V (6*2)
//	#define GS012V (8*2)
//	#define GS022V (GS012V+(8*2))
//	#define GS022V (GS012V+(4*2))	/*dame*/
	#define GS022V (GS012V+(5*2))

	Vertex_uvcxyz* vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory((/*GS012V*/GS022V)*sizeof(Vertex_uvcxyz));	/* GPU���Ԃɍ����̂ŏȗ��m�� */
	/* ��:������͍Ŕw�ʂȂ̂ŏȗ����Ă�GPU�]�����Ԃɍ���(�{���͏ȗ�������ǂ��Ȃ��̂����m��Ȃ�???) */
//
	/* --- �����������l */
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
		/*ARGB4444*/
//	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = ((( 0xf0) << 8) | 0x0fff);
	unsigned /*int*/short blendlevel = (((conv_bg_alpha &  0xf0) << 8) | 0x0fff);
//(conv_bg_alpha)
		#else
	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
		#endif
	#endif

//	val1 = (player->x256>>8);
		#if 1
		val3_256 -= (current_bg0_y_scroll_speed256);
	//	val3--;
		val3 = t256_floor(val3_256);
		val3 &= (0xff);
		#endif

#define Y000 (0)
#define Y100 (100)
#define Y272 ((PSP_HEIGHT272)/*-1*/)
		unsigned int j;
		unsigned short x2;
		unsigned short x3;
		unsigned short x4;
/*	�㑤 */
		/* 176==(GAME_WIDTH/2) */
		x2 = ((GAME_WIDTH/2) - 176 -8) /*+ ((val1&127)-64)*/;
		x3 = ((GAME_WIDTH/2) - 256 -32 -64 +8+1);
		x4 = 180+1;
		for (j=0; j<GS022V; j+=2)
		{
			vertices[j].x = x3;
			vertices[j].y = Y100;
			vertices[j].z = 0;
			vertices[j].u = x4;
			vertices[j].v = (0xff)+(val3);
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x2;
			vertices[j+1].y = Y000;
			vertices[j+1].z = 0;
			vertices[j+1].u = x4;
			vertices[j+1].v = 0+(val3);
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel;
			#endif
			x2 +=  36;/*�㉏��*/
			x3 +=  72;/*������*/
			x4 +=  32/*64*/;/*�e�N�X�`��*/
		}
// 1 3 5 7 9
// 0 2 4 6 8
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (GS022V), NULL, vertices);
/*	���� */
		x2 = ((GAME_WIDTH/2) -176) /*+ ((val1&127)-64)*/;/*(�Œ�[)*/
		x3 = ((GAME_WIDTH/2) -256 -32);
		x4 = 256-1;
		for (j=0; j<GS012V; j+=2)
		{
			vertices[j].x = x3;
			vertices[j].y = Y272;
			vertices[j].z = 0;
			vertices[j].u = x4;
			vertices[j].v = (0xff)+(val3);
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x2;
			vertices[j+1].y = Y100;
			vertices[j+1].z = 0;
			vertices[j+1].u = x4;
			vertices[j+1].v = 0+(val3);
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel;
			#endif
			x2 +=  72;/*�㉏��*/
			x3 += 128;/*������*/
			x4 +=  32/*64*/;/*�e�N�X�`��*/
		}
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (GS012V), NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (GS012V), NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLES, TEXTURE_FLAGS, 3, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_FAN, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 4, NULL, vertices);/*2D��p�H*/
}
		#endif

/*---------------------------------------------------------
	�i�����w�i(���쒆)
	-------------------------------------------------------
---------------------------------------------------------*/

		#if 1//(1==USE_F3D)
static void gu_draw_bg_eientei(void)
{
//	#define EH024V (5*2)
	#define EH024V (12*2)
//	#define EH024V (8*2)
	Vertex_uvcxyz* vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory((EH024V)*sizeof(Vertex_uvcxyz));	/* GPU���Ԃɍ����̂ŏȗ��m�� */
	/* ��:������͍Ŕw�ʂȂ̂ŏȗ����Ă�GPU�]�����Ԃɍ���(�{���͏ȗ�������ǂ��Ȃ��̂����m��Ȃ�???) */
//
	/* --- �����������l */
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
		/*ARGB4444*/
//	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = ((( 0xf0) << 8) | 0x0fff);
	unsigned /*int*/short blendlevel = (((conv_bg_alpha &  0xf0) << 8) | 0x0fff);
//(conv_bg_alpha)
		#else
	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
		#endif
	#endif

//	val1 = (player->x256>>8);
		#if 1
		val3_256 -= (current_bg0_y_scroll_speed256);
	//	val3--;
		val3 = t256_floor(val3_256);
		val3 &= (0xff);
		#endif

#define cccY000 (0)

		unsigned int j;
		unsigned short x2;
		unsigned short x3;
		unsigned short x4;
#define yyy02 x2
#define yyy03 x3

/* ======== �㑤�A�E�ӂ��� ================================================ */
		yyy02	= ( 80+16+7-(4*2) );
		yyy03	= (272+16+7-(4*23) );
		x4		= 256-1/*180+1*/;
	#define EH_R08V (4*2)
		for (j=0; j<EH_R08V; j+=2)
		{
			vertices[j].x = x4/*+10*/ /*0*/ /*100*/ /*yyy03*/;
			vertices[j].y = yyy03/*EH_Y_LOCATE_199*/;
			vertices[j].z = 0;
			vertices[j].u = (0x7f)/*x4*/;
			vertices[j].v = x4+(val3)/*(0xff)+(val3)*/;
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x4/*0*/ /*yyy02*/;
			vertices[j+1].y = yyy02/*cccY000*/;
			vertices[j+1].z = 0;
			vertices[j+1].u = 0/*x4*/;
			vertices[j+1].v = x4+(val3)/*0+(val3)*/;
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel;
			#endif
			yyy02 -=  2;/*�㉏��*/
			yyy03 +=  (24-1)/*16*/;/*������*/
			x4 +=  32/*32*/ /*64*/;/*�e�N�X�`��*/
		}
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (EH_R08V), NULL, vertices);

/* ======== �㑤�A���ӂ��� ================================================ */
		yyy02	= (80);
		yyy03	= (272-1);
		x4		= 0/*180+1*/;
	#define EH_L08V (4*2)
		for (j=0; j<EH_L08V; j+=2)
		{
			vertices[j].x = x4/*+10*/ /*0*/ /*100*/ /*yyy03*/;
			vertices[j].y = yyy03/*EH_Y_LOCATE_199*/;
			vertices[j].z = 0;
			vertices[j].u = (0x7f)/*x4*/;
			vertices[j].v = x4+((-val3)&(0xff))/*(0xff)+(val3)*/;
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x4/*0*/ /*yyy02*/;
			vertices[j+1].y = yyy02/*cccY000*/;
			vertices[j+1].z = 0;
			vertices[j+1].u = 0/*x4*/;
			vertices[j+1].v = x4+((-val3)&(0xff))/*0+(val3)*/;
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel;
			#endif
			yyy02 +=  2;/*�㉏��*/
			yyy03 -=  24/*16*/;/*������*/
			x4 +=  32/*32*/ /*64*/;/*�e�N�X�`��*/
		}
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (EH_L08V), NULL, vertices);


/* ======== ������ ================================================ */
		x2 = ( 96) /*+ ((val1&127)-64)*/;/*(�Œ�[)*/
		x3 = (	0);
		x4 = /*0x7f*/0xbf/*128*/ /*256-1*/;
#define EH_Y_LOCATE_199 (200/*100*/-1)
#define EH_Y_LOCATE_271 ((PSP_HEIGHT272-1)/*-1*/)
		int val333=(val3<<2);
		for (j=0; j<EH024V; j+=2)
		{
			vertices[j].x = x3;
			vertices[j].y = EH_Y_LOCATE_271;
			vertices[j].z = 0;
			vertices[j].u = x4;
			vertices[j].v = (0xff)+(val333);
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x2;
			vertices[j+1].y = EH_Y_LOCATE_199;
			vertices[j+1].z = 65535/*0*/;
			vertices[j+1].u = x4;
			vertices[j+1].v = 0+(val333);
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel;
			#endif
			x2 += 14   /*72/2*/;/*�㉏��*/
			x3 += 32   /*128/2*/;/*������*/
			x4 ^= 0x40/*0x80*/;  /*x4 +=  32*//*64*/;/*�e�N�X�`��*/
		}
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (EH024V), NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (EH024V), NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLES, TEXTURE_FLAGS, 3, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_FAN, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 4, NULL, vertices);/*2D��p�H*/
}


/*
0111 7

1000 8
1001 9
1010 a
1011 b

1100 c
1101 d
1110 e
1111 f

*/

// 1 3 5 7 9
// 0 2 4 6 8

		#endif

/*---------------------------------------------------------
	2D�w�i(���쒆)
	-------------------------------------------------------
---------------------------------------------------------*/
		#if (1)
static void gu_draw_bg_2D(void)
{
//	#define FB012V (5*2)
	#define FB012V (6*2)
//	#define FB012V (8*2)
//	#define FB024V (FB012V+(8*2))
	#define FB024V (FB012V+(6*2))

	Vertex_uvcxyz* vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory((/*FB012V*/FB024V)*sizeof(Vertex_uvcxyz));	/* GPU���Ԃɍ����̂ŏȗ��m�� */
	/* ��:������͍Ŕw�ʂȂ̂ŏȗ����Ă�GPU�]�����Ԃɍ���(�{���͏ȗ�������ǂ��Ȃ��̂����m��Ȃ�???) */
//
	/* --- �����������l */
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
		/*ARGB4444*/
//	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = ((( 0xf0) << 8) | 0x0fff);
	unsigned /*int*/short blendlevel = (((conv_bg_alpha &  0xf0) << 8) | 0x0fff);
//(conv_bg_alpha)
		#else
	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
		#endif
	#endif

//	val1 = (player->x256>>8);
		#if 1
		val3_256 -= (current_bg0_y_scroll_speed256);
	//	val3--;
		val3 = t256_floor(val3_256);
		val3 &= (0xff);
		#endif

#define bbbY000 (0)
#define bbbY272 ((255)/*-1*/)
//#define bbbY272 ((PSP_HEIGHT272)/*-1*/)
		unsigned int j;
		unsigned short x2;
		unsigned short x3;
		unsigned short x4;
/*	�㑤 */
		/* 176==(GAME_WIDTH/2) */
		x2 = (0) /*+ ((val1&127)-64)*/;
		x3 = (0);
		x4 = (0);
		for (j=0; j<FB024V; j+=2)
		{
			vertices[j].x = x3;
			vertices[j].y = bbbY272;
			vertices[j].z = 0;
			vertices[j].u = x4;
			vertices[j].v = (0xff)+(val3);
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x2;
			vertices[j+1].y = bbbY000;
			vertices[j+1].z = 0;
			vertices[j+1].u = x4;
			vertices[j+1].v = 0+(val3);
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel;
			#endif
			x2 +=  32;/*�㉏��*/
			x3 +=  32;/*������*/
			x4 +=  32/*64*/;/*�e�N�X�`��*/
		}
// 1 3 5 7 9
// 0 2 4 6 8
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (FB024V), NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (FB012V), NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLES, TEXTURE_FLAGS, 3, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_FAN, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 4, NULL, vertices);/*2D��p�H*/
}
		#endif


/*---------------------------------------------------------
	�{�X�̌��̖����w
---------------------------------------------------------*/

 int boss_x256;
 int boss_y256;
static int aaa_yyy;
static void gu_draw_big_maho_jin(void)
{
//	#define BM016V ((16*2)+2)	/* �����(+2�͏d�Ȃ镪) */
	#define BM016V ((16+1)*2)	/* �����(+2�͏d�Ȃ镪) */

	Vertex_uvcxyz_C32* vertices;
	vertices = (Vertex_uvcxyz_C32*)sceGuGetMemory((BM016V)*sizeof(Vertex_uvcxyz_C32));
//
	/* --- �����������l */
	//#if (1==USE_VCOLOR)
	//unsigned int blendlevel = (0xaa2f7fff);/*������*/
	//#endif
	//#define big_maho_color8888 (0x997f2fff)		/* --- �����������l */
	/*							  ���ΐ� AABBGGRR */
		const unsigned int maho_color_list[8] =
		{	/*AABBGGRR*/
			0x997f2fff, //		0x997f2fff,/**/
			0x77ff993f, //		0x77ff5f1f,/*1��*/		/* ���肷 */
			0x553fff3f, //		0x99ff7f2f,/*2��*/		/* ����   */
			0xaa2f7fff, //		0xaa2f7fff,/*3��*/		/* �݂Ă� */
			0x997f2fff, //		0x997f2fff,/*4��*/		/* ������ */
			0x33ff5f1f, //		0x33ff5f1f,/*5��*/		/* �ς��� */
			0x997f2fff, //		0x997f2fff,/*6��*/		/* ������ */
			0x997f2fff, //		0x997f2fff,/*ending*/
		//	0xff601010, //	//	0xff601010,/**/
		}; /* ���ΐ� AABBGGRR */
	const unsigned int big_maho_color8888 = maho_color_list[player_now_stage&0x07];
//
	unsigned int j;
	unsigned short uv_x4;

//	int boss_center_x = ((GAME_WIDTH)/2);
//	int boss_center_y = ((GAME_HEIGHT)/2);
	int boss_center_x = ((boss_x256)>>8)+(16);
	int boss_center_y = ((boss_y256)>>8)+(24);

	int hankei_111 = (draw_boss_hp_value>>2);/* 128==1024/8 */
	int hankei_222 = (hankei_111+(8));/* 8[dot]�������� */

	/* �����̃e�N�X�`��x�ʒu */
	aaa_yyy++;
	uv_x4 = (/*val5*/(aaa_yyy) /*& 0xff*/);

/* --- [ ��]���� ] --- */
	int rotation_angle512 = (0);
	for (j=0; j<BM016V; j+=2)/*32����*/
	{
	int sin_angle;
	int cos_angle;
		sin_angle = (sin_tbl512[/*rot_sin*/((/*OFFS_SIN+*/rotation_angle512)&(512-1))]/*<<8*/);
		cos_angle = (sin_tbl512[/*rot_cos*/((  OFFS_COS+  rotation_angle512)&(512-1))]/*<<8*/);
	int ifx;
	int ify;
		ifx = ((hankei_222 * sin_angle)>>8);
		ify = ((hankei_222 * cos_angle)>>8);
		vertices[j].x = (boss_center_x + ifx);
		vertices[j].y = (boss_center_y + ify);
		vertices[j].z = 0;
		vertices[j].u = uv_x4;
		vertices[j].v = (127);	/*(�����̃e�N�X�`��y�ʒu[����]tex_str__y_low_127)*/
		#if (1==USE_VCOLOR)
		vertices[j].color	= (big_maho_color8888); 	/*blendlevel*/
		#endif
	//
		ifx = ((hankei_111 * sin_angle)>>8);
		ify = ((hankei_111 * cos_angle)>>8);
		vertices[j+1].x = (boss_center_x + ifx);
		vertices[j+1].y = (boss_center_y + ify);
		vertices[j+1].z = 0;
		vertices[j+1].u = uv_x4;
		vertices[j+1].v = (110);	/*(�����̃e�N�X�`��y�ʒu[���]tex_str_y_high_110)*/
		#if (1==USE_VCOLOR)
		vertices[j+1].color = (big_maho_color8888); 	/*blendlevel*/
		#endif
		uv_x4				+=	32; 	/*64*/	/*�e�N�X�`��*/
		rotation_angle512	+=	32; 	/*64*/	/* �p�x */
	}
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (BM016V), /*0*/NULL, &vertices[0/*SL024V*/]);
}
// 1 3 5 7 9
// 0 2 4 6 8



/*---------------------------------------------------------

---------------------------------------------------------*/


	/* �� �t�����g�ʃG�t�F�N�g(�e�N�X�`�����p) */
/*---------------------------------------------------------
	�������{��(�X�y�J)�������ꍇ�ɏo��B�����̃��S
	Border Power of Spiritual ��̂ŗ͂𔭊����鋫�E
	-------------------------------------------------------
	���݁A�������͍���ĂȂ����A
	���������o�Ȃ��ƌ��w�B
	-------------------------------------------------------
	[ ������ ] / [ �c���� ] �Ɋւ��Ă� uv�X�N���[�� ���g���Ă܂��B
	���Ă�[���{�������������� uv�X�N���[�� ���Ǝv����B
---------------------------------------------------------*/

static void gu_draw_front_spell_logo(void)
{
//	#define SL020V (9*2)	/* ��ʓr���B16�h�b�g����Ȃ��B((9-1)*32==256 < 272 < 288==(10-1)*32) [32slice] */
	#define SL020V (10*2)	/* ��ʋ�ݏo���B16�h�b�g�����B((9-1)*32==256 < 272 < 288==(10-1)*32) [32slice] */
	#define SL024V (12*2)	/* ��ʒ��x�B((12-1)*32 == 352 == 480[psp�̉�ʕ`�敝]-128[�p�l����]) [32slice] */

	Vertex_uvcxyz_C32* vertices;
	vertices = (Vertex_uvcxyz_C32*)sceGuGetMemory((SL024V+SL020V)*sizeof(Vertex_uvcxyz_C32));	/* GPU���Ԃɍ���Ȃ��̂ł����Ɗm�� */
	/* ��:������͍őO�ʂȂ̂ŏȗ������GPU�]�����Ԃɍ���Ȃ� */
//
	/* --- �����������l */
	#if (1==USE_VCOLOR)
		#if 0//(16==USE_BLIT_COLOR_BIT)
		/*ARGB4444*/
//	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = ((( 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = (((conv_bg_alpha &  0xf0) << 8) | 0x0fff);
	unsigned /*int*/short blendlevel = (0x7777);
//(conv_bg_alpha)
		#else
//	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
	unsigned int blendlevel = (0x7f7f7f7f);
		#endif
	#endif
//
	unsigned int j;
	unsigned short x5;
	unsigned short x4;

/* Border Power of Spiritual �����̃e�N�X�`���ʒu */
#define TEX_STR_Y_HIGH_217	(217)
#define TEX_STR__Y_LOW_255	(255)
/* --- [ ������ ] --- */
/* SPELL_LOGO_LOCATE_Y01_160: ��ʍ��ォ��� y�I�t�Z�b�g(����) */
#define SPELL_LOGO_LOCATE_Y01_160 (160)
#define SPELL_LOGO_LOCATE_Y02_200 (SPELL_LOGO_LOCATE_Y01_160+40)
	x5 = 0;
	x4 = (/*val5*/(pd_bomber_time) /*& 0xff*/);
	for (j=0; j<SL024V; j+=2)
	{
		vertices[j].x = x5;
		vertices[j].y = SPELL_LOGO_LOCATE_Y02_200;
		vertices[j].z = 0;
		vertices[j].u = x4;
		vertices[j].v = (TEX_STR__Y_LOW_255);
		#if (1==USE_VCOLOR)
		vertices[j].color	= blendlevel;
		#endif
	//
		vertices[j+1].x = x5;
		vertices[j+1].y = SPELL_LOGO_LOCATE_Y01_160;
		vertices[j+1].z = 0;
		vertices[j+1].u = x4;
		vertices[j+1].v = (TEX_STR_Y_HIGH_217);
		#if (1==USE_VCOLOR)
		vertices[j+1].color = blendlevel;
		#endif
		x5 +=  32/*36*/;/*��/������*/
		x4 +=  32/*64*/;/*�e�N�X�`��*/
	}
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (SL024V), NULL, vertices);

/* --- [ �c���� ] --- */
/* SPELL_LOGO_LOCATE_X01_200: ��ʍ��ォ��� x�I�t�Z�b�g(����) */
#define SPELL_LOGO_LOCATE_X01_200 (200)
#define SPELL_LOGO_LOCATE_X02_240 (SPELL_LOGO_LOCATE_X01_200+40)
#define y5 x5
	y5 = 0; 	//	x4 = (val5 & 0xff);
	for (/*j=0*/; j<SL024V+SL020V; j+=2)	/* GPU�]�����Ԃɍ���Ȃ��̂ŁA�����Ɗm��(j�͓r������) */
	{
		vertices[j].x = SPELL_LOGO_LOCATE_X01_200;
		vertices[j].y = y5;
		vertices[j].z = 0;
		vertices[j].u = x4;
		vertices[j].v = (TEX_STR__Y_LOW_255);
		#if (1==USE_VCOLOR)
		vertices[j].color	= blendlevel;
		#endif
	//
		vertices[j+1].x = SPELL_LOGO_LOCATE_X02_240;
		vertices[j+1].y = y5;
		vertices[j+1].z = 0;
		vertices[j+1].u = x4;
		vertices[j+1].v = (TEX_STR_Y_HIGH_217);
		#if (1==USE_VCOLOR)
		vertices[j+1].color = blendlevel;
		#endif
		y5 +=  32/*36*/;/*��/������*/
		x4 +=  32/*64*/;/*�e�N�X�`��*/
	}
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (SL020V), /*0*/NULL, &vertices[SL024V]);
}
// 1 3 5 7 9
// 0 2 4 6 8

/*---------------------------------------------------------

---------------------------------------------------------*/

#if (1==USE_GU)
#if (1)

static void gu_draw_sdl_screen(void)
{
/* ����:
(a) SLICE_SIZE 64 �� 512 x 272 �]�� (�ꍇ�����Ȃ�)
(b) SLICE_SIZE 64 �� 480 x 272 �]�� (�ꍇ��������)
(c) SLICE_SIZE 32 �� 480 x 272 �]�� (�ꍇ�����Ȃ�)
����: ���� : (b) > (c) > (a)  : �x��
 */
#define USE_SLICE_32 (0)
#if (0==USE_SLICE_32)
	#define SLICE_SIZE32	(64)
#else
	#define SLICE_SIZE32	(32)
#endif
	unsigned int j;
	unsigned int i;
	#if 0
	unsigned int count2;
	count2 = 0;
	for (j=0; (j+SLICE_SIZE32)<=(/*480*/512/*512*/); j+=SLICE_SIZE32)
	{
		count2 += 2;
	}
	#else
		#if (0==USE_SLICE_32)
			#define count2 (16)/* 16 == 2 x (512/64[SLICE_SIZE32] ) */
		#else
			#define count2 (30)/* 30 == 2 x (480/32[SLICE_SIZE32] ) */
		#endif
	#endif
	Vertex_uvcxyz	*vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(count2 * sizeof(Vertex_uvcxyz));
	i=0;
	j=0;
	#if (0==USE_SLICE_32)
	unsigned int w_size;
	w_size = SLICE_SIZE32;
	#else
		#define w_size SLICE_SIZE32
	#endif
//	for (; (j+SLICE_SIZE32)<=(/*480*/512/*512*/); )
	for (; i<count2; )
	{
		#if (0==USE_SLICE_32)
		/* �Ō� */
		if (i==(count2-2))
		{
			w_size = 32;	/* 32 == 512-480 */
		}
		#endif
		vertices[(i)].u = j;
		vertices[(i)].v = 0;
		vertices[(i)].x = j/**480/512*/;
		vertices[(i)].y = 0;
		vertices[(i)].z = 0;
		vertices[(i)+1].u = (j+w_size);
		vertices[(i)+1].v = 272/*320*/;
		vertices[(i)+1].x = (j+w_size)/**480/512*/;
		vertices[(i)+1].y = 272;
		vertices[(i)+1].z = 0;
		i += 2;
		j += SLICE_SIZE32;
	}
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650, (count2), NULL, vertices);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

#if 0
	#define SCRIPT_WINDOW_X_SIZE_340		(340)
	#define SCRIPT_WINDOW_Y_SIZE_070		(70)
	#define SCRIPT_WINDOW_SPACE_020 		(20)	/* [��ʒ[�}�[�W��] */
	#define SCRIPT_WINDOW_X_POSITION_020	(SCRIPT_WINDOW_SPACE_020)
	#define SCRIPT_WINDOW_Y_POSITION_182	(272-70-20)/* 182 == (272-70-20) == (GAME_HEIGHT-SCRIPT_WINDOW_Y_SIZE_070-SCRIPT_WINDOW_SPACE_020) */
#else
	#define SCRIPT_WINDOW_X_SIZE_340		(340)
	#define SCRIPT_WINDOW_Y_SIZE_070		(70)
	/* 6 == ((480-128-340)/2) == ((GAME_WIDTH-SCRIPT_WINDOW_X_SIZE_340)/2) == SCRIPT_WINDOW_SPACE_020 */
	#define SCRIPT_WINDOW_SPACE_020 		(6)
	#define SCRIPT_WINDOW_X_POSITION_020	(SCRIPT_WINDOW_SPACE_020)
	#define SCRIPT_WINDOW_Y_POSITION_182	(GAME_HEIGHT-SCRIPT_WINDOW_Y_SIZE_070-SCRIPT_WINDOW_SPACE_020)
#endif
static void gu_draw_serifu_screen(void)
{
/* ����:
(a) SLICE_SIZE 64 �� 512 x 272 �]�� (�ꍇ�����Ȃ�)
(b) SLICE_SIZE 64 �� 480 x 272 �]�� (�ꍇ��������)
(c) SLICE_SIZE 32 �� 480 x 272 �]�� (�ꍇ�����Ȃ�)
����: ���� : (b) > (c) > (a)  : �x��
 */
	unsigned int j;
	unsigned int i;
	#define FONT_SCREEN_MARGIN_010		(10)	/* [�t�H���g�[�}�[�W��] */
	#if 0
	unsigned int count29;
	count29 = 0;
	for (j=0; (j+SLICE_SIZE32)<=(/*480*/512/*512*/); j+=SLICE_SIZE32)
	{
		count29 += 2;
	}
	#else
		#if (0==USE_SLICE_32)
			/*
			320 == 480-((6[��ʒ[�}�[�W��] + 10[�t�H���g�[�}�[�W��] )x 2) -(128[�p�l���E�B���h�E��])
			*/
		//	#define count29 (16)/* 16 == 2 x (512/64[SLICE_SIZE32] ) */
		//	#define count29 (10)/* 16 == 2 x (512/64[SLICE_SIZE32] ) */
			#define count29 (10)/* 10 == 2 x (320/64[SLICE_SIZE32] ) */
		//	#define count29 (12)/* 10 == 2 x (320/64[SLICE_SIZE32] ) */
		#else
			#define count29 (30)/* 30 == 2 x (480/32[SLICE_SIZE32] ) */
		#endif
	#endif
	Vertex_uvcxyz	*vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(count29 * sizeof(Vertex_uvcxyz));
	i=0;
	j=0;
	//#if (0==USE_SLICE_32)
	//unsigned int w_size;
	//w_size = SLICE_SIZE32;
	//#else
		#define w_size99 SLICE_SIZE32
	//#endif
//	for (; (j+SLICE_SIZE32)<=(/*480*/512/*512*/); )
	for (; i<count29; )
	{
	//	#if (0==USE_SLICE_32)
	//	/* �Ō� */
	//	if (i==(count29-2))
	//	{
	//	//	w_size = 32;	/* 32 == 512-480 */
	//		w_size = 44;	/* 44 == 384-340 */
	//	}
	//	#endif
		vertices[(i)].u = j;
		vertices[(i)].v = 1/*+(0)*/;
		vertices[(i)].x = (FONT_SCREEN_MARGIN_010)+(SCRIPT_WINDOW_X_POSITION_020)+j/**480/512*/;
		vertices[(i)].y = (FONT_SCREEN_MARGIN_010)+(SCRIPT_WINDOW_Y_POSITION_182)/*+(0)*/;
		vertices[(i)].z = 0;
		vertices[(i)+1].u = (j+w_size99);
		vertices[(i)+1].v = 1+(1+4+47/*70*/)/*272*/ /*320*/;
		vertices[(i)+1].x = (FONT_SCREEN_MARGIN_010)+(SCRIPT_WINDOW_X_POSITION_020)+(j+w_size99)/**480/512*/;
		vertices[(i)+1].y = (FONT_SCREEN_MARGIN_010)+(SCRIPT_WINDOW_Y_POSITION_182)+(1+4+47/*70*/)/*272*/;
		vertices[(i)+1].z = 0;
		i += 2;
		j += SLICE_SIZE32;
	}
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650, (count29), NULL, vertices);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void gu_draw_side_panel(void)
{
	if (0!=draw_boss_hp_value)
	{
	//	#define draw_boss_hp_value			(340)
		#define SIDE_WINDOW_Y_SIZE_070		(4)/*70*/
		/* 6 == ((480-128-340)/2) == ((GAME_WIDTH-draw_boss_hp_value)/2) == SIDE_WINDOW_X_POSITION_020 */
		#define SIDE_WINDOW_X_POSITION_020	(64)/* (50)HPGAUGE_X_OFS+(10)drec.x */
		#define SIDE_WINDOW_Y_POSITION_182	(1+3)/*GAME_HEIGHT-SIDE_WINDOW_Y_SIZE_070-SIDE_WINDOW_X_POSITION_020*/

		/* -- ��`�� */
		/* �e�N�X�`���[������ƃe�N�X�`���[�D��Ńt���b�g�|���S�����`��o���Ȃ��̂� */
		/* �ꎞ�I�Ƀe�N�X�`���[�������[�h�ɐ؂�ւ��� */
		sceGuDisable(GU_TEXTURE_2D);/* �e�N�X�`���[�������[�h�ɂ��� */
		{
			//#define TEXTURE_FLAGS1111 	(GU_TEXTURE_16BIT | 				GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS1111aaa		(									GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			//GU_VERTEX_32BITF|GU_TRANSFORM_2D
			// we make local copies of the line into the main buffer here, so we don't have to flush the cache
			Point_Vertex* my_vertices = sceGuGetMemory((5) * sizeof(Point_Vertex));

			// create a lineloop
			my_vertices[0].x = (SIDE_WINDOW_X_POSITION_020)    /*x-width*/; 					my_vertices[0].y = (SIDE_WINDOW_Y_POSITION_182)   /*y-height*/;
			my_vertices[1].x = (SIDE_WINDOW_X_POSITION_020)+(draw_boss_hp_value)/*x+width*/;	my_vertices[1].y = (SIDE_WINDOW_Y_POSITION_182)   /*y-height*/;
			my_vertices[2].x = (SIDE_WINDOW_X_POSITION_020)+(draw_boss_hp_value)/*x+width*/;	my_vertices[2].y = (SIDE_WINDOW_Y_POSITION_182)+(SIDE_WINDOW_Y_SIZE_070)/*y+height*/;
			my_vertices[3].x = (SIDE_WINDOW_X_POSITION_020)    /*x-width*/; 					my_vertices[3].y = (SIDE_WINDOW_Y_POSITION_182)+(SIDE_WINDOW_Y_SIZE_070)/*y+height*/;
			my_vertices[4].x = (SIDE_WINDOW_X_POSITION_020)    /*x-width*/; 					my_vertices[4].y = (SIDE_WINDOW_Y_POSITION_182)   /*y-height*/;
		//
		//	#define argb111 0xffffffff
	//		#define argb111 0x7f0f0f0f
		//	#define argb111 0x7f7f7f7f
		//	sceGuColor(/*0xaaaa*/ ((argb111&0x7f7f7f7f)>>1) /*argb*/);
		//	sceGuColor(argb111/*0xffff*/ /*argb*/);
		//	sceGuColor(/*0xaaaa*/ ((argb111&0x7f7f7f7f)>>2) /*argb*/);
			sceGuColor(/*0xaaaa*/ ((0x7fffffff)) /*argb*/);
			sceGuDrawArray(GU_TRIANGLES/*GU_LINE_STRIP*/, TEXTURE_FLAGS1111aaa, (3), NULL, my_vertices);
			sceGuDrawArray(GU_TRIANGLES/*GU_LINE_STRIP*/, TEXTURE_FLAGS1111aaa, (3), NULL, &my_vertices[2]);
		//
		//	sceGuColor(argb111/*0xffff*/ /*argb*/);
		//	sceGuColor(/*0xaaaa*/ ((argb111&0x7f7f7f7f)>>1) /*argb*/);
			sceGuColor(/*0xaaaa*/ ((0x3f3f3f3f)) /*argb*/);
		//	sceGuColor(argb111/*0xffff*/ /*argb*/);
			sceGuDrawArray(GU_LINE_STRIP, TEXTURE_FLAGS1111aaa, (5), NULL, my_vertices);
		}
		/* �e�N�X�`���[�L�����[�h�ł́A����ʓ��̃t���b�g�|���S�����g���Ȃ� */
		sceGuEnable(GU_TEXTURE_2D); 	/* �e�N�X�`���[�L�����[�h�ɖ߂� */
//
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
extern int draw_script_screen;					/* ����ӃE�B���h�E�\���t���O */
/*extern*/ int msg_time;							/* ����ӃE�B���h�E�\������(��) */
static void gu_draw_script_window(void)
{
	if (0 != draw_script_screen)
	{
		/* -- ����ӃE�B���h�E��`�� */
		/* �e�N�X�`���[������ƃe�N�X�`���[�D��Ńt���b�g�|���S�����`��o���Ȃ��̂� */
		/* �ꎞ�I�Ƀe�N�X�`���[�������[�h�ɐ؂�ւ��� */
		sceGuDisable(GU_TEXTURE_2D);/* �e�N�X�`���[�������[�h�ɂ��� */
		{
			//#define TEXTURE_FLAGS1111bbb	(GU_TEXTURE_16BIT | 				GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS1111bbb		(									GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			//GU_VERTEX_32BITF|GU_TRANSFORM_2D
			// we make local copies of the line into the main buffer here, so we don't have to flush the cache
			Point_Vertex* my_vertices = sceGuGetMemory((5) * sizeof(Point_Vertex));
			// create a lineloop
			my_vertices[0].x = (SCRIPT_WINDOW_X_POSITION_020)	   /*x-width*/; 						my_vertices[0].y = (SCRIPT_WINDOW_Y_POSITION_182)	  /*y-height*/;
			my_vertices[1].x = (SCRIPT_WINDOW_X_POSITION_020)+(SCRIPT_WINDOW_X_SIZE_340)/*x+width*/;	my_vertices[1].y = (SCRIPT_WINDOW_Y_POSITION_182)	  /*y-height*/;
			my_vertices[2].x = (SCRIPT_WINDOW_X_POSITION_020)+(SCRIPT_WINDOW_X_SIZE_340)/*x+width*/;	my_vertices[2].y = (SCRIPT_WINDOW_Y_POSITION_182)+(SCRIPT_WINDOW_Y_SIZE_070)/*y+height*/;
			my_vertices[3].x = (SCRIPT_WINDOW_X_POSITION_020)	   /*x-width*/; 						my_vertices[3].y = (SCRIPT_WINDOW_Y_POSITION_182)+(SCRIPT_WINDOW_Y_SIZE_070)/*y+height*/;
			my_vertices[4].x = (SCRIPT_WINDOW_X_POSITION_020)	   /*x-width*/; 						my_vertices[4].y = (SCRIPT_WINDOW_Y_POSITION_182)	  /*y-height*/;
		//
		//	#define argb222 0xffffffff
		//	#define argb222 0x7f0f0f0f
		//	#define argb222 0x7f7f7f7f
		//	sceGuColor(/*0xaaaa*/ ((argb222&0x7f7f7f7f)>>1) /*argb*/);
		//	sceGuColor(argb222/*0xffff*/ /*argb*/);
			sceGuColor( (0x7f0f0f0f) /*argb*/);
			sceGuDrawArray(GU_TRIANGLES/*GU_LINE_STRIP*/, TEXTURE_FLAGS1111bbb, (3), NULL, my_vertices);
			sceGuDrawArray(GU_TRIANGLES/*GU_LINE_STRIP*/, TEXTURE_FLAGS1111bbb, (3), NULL, &my_vertices[2]);
		//
		//	sceGuColor(argb222/*0xffff*/ /*argb*/);
		//	sceGuColor(/*0xaaaa*/ ((argb222&0x7f7f7f7f)>>1) /*argb*/);
			sceGuColor( (0x3f878787) /*argb*/);
			sceGuDrawArray(GU_LINE_STRIP, TEXTURE_FLAGS1111bbb, (5), NULL, my_vertices);
		}
		/* �e�N�X�`���[�L�����[�h�ł́A����ʓ��̃t���b�g�|���S�����g���Ȃ� */
		sceGuEnable(GU_TEXTURE_2D); 	/* �e�N�X�`���[�L�����[�h�ɖ߂� */
	}
//
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);/*???*/		/*�����double buffer???*/
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	sceGuTexMode(/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/, 0, 0, 0/*0 swizzle*/);
	sceGuTexImage(0, 512, 512, 512, msg_window_image);
	gu_draw_serifu_screen();
}

#endif/*(000)*/
#endif

/*---------------------------------------------------------
	�e�N�X�`�����Z�b�g
	-------------------------------------------------------
	�e�N�X�`���ݒ�R�}���h(sceGuTexImage)�͓����ŁA�L���b�V��(GPU)��
	�t���b�V���o�b�N����A���x�ቺ���܂��B
	�g���߂���(100��ȉ����炢�Ȃ���Ȃ�����)�� 60fps��ۂ͖̂����ł��̂ŁA
	���Ȃ��Ȃ�悤�ɃQ�[����݌v���܂��B
	-------------------------------------------------------
	�Q�l�FPSP����\�t�g�J���X�� ver.8 (262-322���炢����̗���)
	(���Ă�[�����̃X���B���^�V(312)�����e���Ă񂾂���...�����ł����˂�
	313(281)��(�����I��)����PG���Ǝv����B
	���̂��ƕ���������Ȃ��̂ɒe�����ǂ����������Ă邵��(��))
---------------------------------------------------------*/
static void gu_set_texture(int texture_number)
{
	//	sceGuTexFunc(GU_TFX_ADD,GU_TCC_RGB);//???
	//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/*������*/
		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGB);
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_ONE_MINUS_SRC_ALPHA, (conv_bg_alpha), 0xffffffff);
			#if (16==USE_BLIT_COLOR_BIT)
	//	sceGuTexMode(/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
	//	sceGuTexMode(/*GU_PSM_5551*/GU_PSM_5551/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
	//	sceGuTexMode(/*GU_PSM_5551*/GU_PSM_4444/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
		sceGuTexMode(/*GU_PSM_5551*/GU_PSM_5650/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
			#else
		sceGuTexMode(/*GU_PSM_5551*/GU_PSM_8888/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
			#endif
	//	sceGuTexImage(0, TW128, TH128, 512, bullet_image);
	//	sceGuTexImage(0, TW128/2, TH128/2, TW128/2/*WK512*/, my_texture[TEX_06_BULLET]->pixels);
	//	sceGuTexImage(0, TW128, TH128, TW128/*WK512*/, my_texture[TEX_00_BACK_GROUND]->pixels);
	//	sceGuTexImage(0, 256, 256, 256/*WK512*/, ((my_resource[texture_number].my_texture)->pixels));
		sceGuTexImage(0,
			(my_resource[texture_number].texture_width),
			(my_resource[texture_number].texture_height),
			(my_resource[texture_number].buffer_width), 	/*WK512*/
			((my_resource[texture_number].my_texture)->pixels));
//test		sceGuTexFilter(GU_LINEAR, GU_LINEAR);
}
/*---------------------------------------------------------

---------------------------------------------------------*/

//static void callback_gu_draw_haikei_all(void)
static void gu_blit_haikei_maho_jiki(void)
{
/* -- BG ��ʂ�`�� */
	#if (1)
	{
		gu_set_texture(TEX_00_BACK_GROUND);
	//	sceGuEnable(GU_CLIP_PLANES);
	//	#if (1==USE_F3D)
	//	gu_draw_bg_fake3D();
	//	#else
	//	gu_draw_bg_2D();
	//	#endif
		(callback_gu_draw_haikei)();	//callback_gu_draw_haikei_all();
	//	sceGuDisable(GU_CLIP_PLANES);
	}
	#endif
/* -- �w�i��Q����`�� */
	#if (1)
	#endif
/* -- �����w��`�� */
	#if (1)
	{
		if (0!=draw_boss_hp_value)
		{
			gu_set_texture(TEX_02_MAHOUJIN);	/* �e�N�X�`�����Z�b�g */
			gu_draw_big_maho_jin();
		}
	}
	#endif
/* -- jiki��`�� */
	#if (1)
//	if (NULL != callback_gu_draw_haikei)	//���pif (0!=dr aw_bg_screen)
	{
		gu_set_texture(TEX_03_JIKI);
		/*jiki_transfer_object();*/
		{	TGameScreen_ClearSprite();
		//	common_transfer_objects(TEX_03_JIKI, SP_GROUP_JIKI_GET_ITEM, obj_status_table+(2*4*8*8));
		//	common_transfer_objects(TEX_03_JIKI, SP_GROUP_JIKI_GET_ITEM, obj_status_table+(8*8)+(select_player<<6) );
			common_transfer_objects(TEX_03_JIKI, SP_GROUP_JIKI_GET_ITEM, obj_status_table+(OBJ_BANK_01_REIMU*OBJ_BANK_SIZE)+(select_player<<6) );
		}
		blit_all_objects();/*PRIORITY_02_PLAYER*/
	}
	#endif /*(000)*/

}
/*---------------------------------------------------------

---------------------------------------------------------*/

#define USE_VSYNC			(0)
#define USE_MAX_GU_TIME 	(0)

void vbl_draw_screen(void)
{
	do_input_vbl();/*�L�[����(�����̓s����A�����Ɉړ�)*/
	#if (1==USE_GU)
//	fps_show();
//
	#if (0!=USE_MAX_GU_TIME)
	/* [Gu���Ԃ��ő�Ɏ��^�C�v](�F�X��������) */
//	sceGuFinish();����
	/* - �p�P�b�g�`��I���҂� */
	sceGuSync(0, 0);/* (�����܂� Gu) */

	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
//	do_input_vbl();
	sceGuSwapBuffers();
	#endif
	/* �������� Gu */
	/* --- �X�N���[���A�b�v�f�[�g */
	sceGuStart(GU_DIRECT, gulist );

	/* --- ��ʃN���A */
	#if 1
	{
		const unsigned int bg_color_list[8] =
		{	/*AABBGGRR*/
			0xff000000,/**/
			0xff106010,/*1��*/
			0xff102080,/*2��*/
			0xff104010,/*3��*/
			0xff402020,/*4��*/
			0xff601030,/*5��*/
			0xff601030,/*6��*/
			0xff000000,/*ending*/
		//	0xff601010,/**/
		};
		sceGuClearColor(bg_color_list[player_now_stage&0x07]);/*AABBGGRR*/
	//	sceGuClearColor(/*0*/0xff601010/*0xff601010*/);/*AABBGGRR*/
	}
	#endif
	#if (1==USE_ZBUFFER)
	sceGuClearDepth(0);// Z_BUFFER
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_FALSE);
	#else // (0==USE_ZBUFFER)
	sceGuClear(GU_COLOR_BUFFER_BIT/*|GU_DEPTH_BUFFER_BIT*/);
	#endif/* (1==USE_ZBUFFER) */

	/* -- ��ʍX�V */

//	sceDisplayWaitVblankStart();/*vsync*/
//	SDL_BlitSurface(sdl_screen[SDL_00_SCREEN], NULL, SDL_VRAM_SCREEN, NULL);
//	{
//	//	memcpy( (UINT16 *)((UINT32) 0x44000000), render_image, (512*256) );/*test*/
//	//	memcpy( (UINT16 *)((UINT32) 0x44000000), render_image, (512*272*2) );
//	}
//	SDL_Flip(SDL_VRAM_SCREEN);

	//#if (0==USE_MAX_TRANS)
	sceKernelDcacheWritebackAll();
	//#endif // (0==USE_MAX_TRANS)
//
	sceGuScissor(0, 0, GAME_WIDTH, GAME_HEIGHT);	/* �`��͈͂�ݒ肷�� */
//
	/* -- BG�w�i ��ʂ�`�� */
//	if (1==dr aw_bg_screen) /* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	if (NULL != callback_gu_draw_haikei)	//if (0!=dr aw_bg_screen)
	{
		/* -- BG ��ʂ�`�� */
		gu_blit_haikei_maho_jiki();
	}
//
	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* �`��͈͂�ݒ肷�� */
//
/* -- SDL ��ʂ�`�� */
	#if (1)
	{
	//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/*������*/
	//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGB);
		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);/*???*/		/*�����double buffer???*/
	//
	//	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

	//	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	//	sceGuBlendFunc(GU_SUBTRACT, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	//	sceGuBlendFunc(GU_ADD, GU_SRC_COLOR, GU_DST_COLOR, 0, 0);
	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0x7f007f7f, 0x3f3f3f00);
	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0x7f7f7f7f, 0x7f7f7f7f);
	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_ONE_MINUS_SRC_ALPHA, (conv_bg_alpha), 0xffffffff);
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0, 0);
	//	sceGuBlendFunc(GU_MIN, GU_FIX, GU_FIX, 0xffffffff, 0xffffffff);
	//	sceGuBlendFunc(GU_MIN, GU_FIX, GU_FIX, 0, 0);
	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0x7f7f7f7f, 0x7f7f7f7f);
	//
		sceGuTexMode(/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/, 0, 0, 0/*0 swizzle*/);
	//	sceGuTexMode(/*GU_PSM_5551*/GU_PSM_5551/*GU_PSM_5650*/, 0, 0, 0/*0 swizzle*/);
		sceGuTexImage(0, 512, 512, 512, render_image);
		sceGuTexFilter(GU_NEAREST, GU_NEAREST);/*��������g����*/
		gu_draw_sdl_screen();
		sceGuTexFilter(GU_LINEAR, GU_LINEAR);/*�ڂ�ڂ�g����*/
	}
	#endif /*(000)*/
//
	sceGuScissor(0, 0, GAME_WIDTH, GAME_HEIGHT);	/* �`��͈͂�ݒ肷�� */
//
/* -- �G��`�� */
	/* �� �G�ʃG�t�F�N�g */
	/* �U�R(�G�ʃG�t�F�N�g) */
	/* �{�X(�G�ʃG�t�F�N�g) */
	#if (1)
	#endif /*(000)*/
//
/* -- ITEM��`�� */
//	/* ���̑��̎��@�f��(�A�C�e���ʃG�t�F�N�g) */
	#if (1)
	#endif /*(000)*/
//
/* -- �G�e��`�� */
	/* �� �e���ʃG�t�F�N�g */
	#if (1)
	{
		gu_set_texture(TEX_06_BULLET);	/* �e�N�X�`�����Z�b�g */
		/*bullet_transfer_object();*/
		{
			TGameScreen_ClearSprite();
		//	common_transfer_objects(TEX_06_BULLET, SP_GROUP_BULLETS, obj_status_table+0);
			common_transfer_objects(TEX_06_BULLET, SP_GROUP_BULLETS, obj_status_table+(OBJ_BANK_00_TAMA*OBJ_BANK_SIZE));
		}
		blit_all_objects();/*PRIORITY_05_BULLETS*/
	}
	#endif /*(000)*/
//
/* -- �t�����g�ʂ�`�� */
	/* �� �t�����g�ʃG�t�F�N�g */
//	/* �����̂����蔻��ʒu�\���p�R�A(�t�����g�ʃG�t�F�N�g) */
//	/* ������ / �U�R���Ŕ���(�t�����g�ʃG�t�F�N�g) */
//	/* �Ή�����(�t�����g�ʃG�t�F�N�g) */
	/* �{���o�[���S(�t�����g�ʃG�t�F�N�g) */
	/* /[�R���e�B�j���[����(bank00)/���j���[����(bank01)/���j���[����(bank02)] */
	#if (1)
	{
		gu_set_texture(TEX_07_FRONT);	/* �e�N�X�`�����Z�b�g */
		/*front_transfer_object();*/
		{
			TGameScreen_ClearSprite();
			#define SP_GROUP_FRONT			(SP_GROUP_ETC)/* �t�����g�� */
		//	common_transfer_objects(TEX_07_FRONT, SP_GROUP_FRONT, obj_status_table+0);
			common_transfer_objects(TEX_07_FRONT, SP_GROUP_FRONT, obj_status_table+(OBJ_BANK_07_FRONT_BANK0*OBJ_BANK_SIZE));
		}
		blit_all_objects();/*PRIORITY_06_FRONT*/
	}
	/* �� �t�����g�ʃG�t�F�N�g(�e�N�X�`�����p) */
	//{//PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	if (0 != pd_bomber_time)
	{
		gu_draw_front_spell_logo();
	}
	//}
	#endif /*(000)*/
//
	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* �`��͈͂�ݒ肷�� */
//
/* -- �p�l���ʂ�`�� */
	#if (1)
	if (0 != draw_side_panel)
	{
		gu_draw_side_panel();
	}
	#endif /*(000)*/
//
	/* -- �X�N���v�g��ʂ�`�� */
//	if (1==draw_script_screen)		/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	if (0 != (draw_script_screen+msg_time))
	{
		if (0 != (msg_time))/* ���b�Z�[�W(���Ή�)�\������ */
		{
			msg_time--;
		}
		gu_draw_script_window();
	}

	/* -- �J���f�o�b�O�t�H���g */
	#if (1==DEBUG)
	TDebugDisp();
	#endif // (1==DEBUG)

	#if (1==USE_ZBUFFER)
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_TRUE);
	#endif/* (1==USE_ZBUFFER) */

	/* - �p�P�b�g�ς݁A�����܂� */
	sceGuFinish();
//
	#if (0==USE_MAX_GU_TIME)
	/* [�ʏ�^�C�v]  */
	sceGuSync(0, 0);/* (�����܂� Gu) */
	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
	sceGuSwapBuffers();
	#endif /* (0==USE_MAX_GU_TIME) */
//
	#else
	//fps_show();
	SDL_Flip(sdl_screen[SDL_00_SCREEN]);
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
	//SDL_UpdateRect(sdl_screen[SDL_00_SCREEN], 0, 0, 0, 0);
	#endif
//
	//fps_newframe();

	#if (1==USE_DESIGN_TRACK)
	/* �\�� voice ���Đ� */
	voice_play_vbl();
	#endif
}
