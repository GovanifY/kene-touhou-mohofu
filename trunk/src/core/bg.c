
/*---------------------------------------------------------

---------------------------------------------------------*/

#include "game_main.h"

#define USE_BG_LOAD 0

#if (1==USE_BG_LOAD)
/* tile */
static SDL_Surface *bg0_bmp/*=NULL*/;
static SDL_Surface *bg1_bmp/*=NULL*/;		// [***090201		�ǉ�

static SDL_Surface *draw_bmp;
static SDL_Surface *load_bmp;
static signed int bg0_bmp_y256;

static int which_bg;						// [***090209�ǉ�:	����ǂ���ɓǂނ̂��������ԍ��B0==bg0_bmp, 1==bg1_bmp			//	�ǉ��w�i�ԍ�

static int number_of_bg;					/* bg�ǉ��������� */
//static int use_clouds;

static int exsist_tuika;					/* �ǉ�bg�͂���H�t���O */

static unsigned int laster_sprit256;		/* bg0_bmp�� bg1_bmp�� �\�����X�^�����ʒu */
static int sprit_flag;						/* �����ʒu�𔻒f����K�v�����邩�H�t���O */
#endif /* (1==USE_BG_LOAD) */


/*static*/ int current_bg0_y_scroll_speed256;	/* bg0�̃X�N���[���A���ݑ��x */
static int request_bg0_y_scroll_speed256;	/* bg0�̃X�N���[���A�\�񑬓x */

//#define USE_EXTRACT_WAIT (0)
//#if (1==USE_EXTRACT_WAIT)
//static int now_exetracting;
//#endif

#if (1==USE_BG_LOAD)
/*---------------------------------------------------------
	�q�֐�
---------------------------------------------------------*/

static void load_btile(STAGE_DATA *l, int num)
{
	char *text;
	text=l->user_string;
	switch (num)
	{
	case 0/*1*/:
		if (NULL != bg0_bmp)	{	unloadbmp_by_surface(bg0_bmp);	bg0_bmp = NULL; }
		bg0_bmp = loadbmp0(text/*filename*/, 0, 1);
//		#if (1==USE_EXTRACT_WAIT)
//		now_exetracting=60;
//		#endif
		break;
	case 1/*2*/:
		if (NULL != bg1_bmp)	{	unloadbmp_by_surface(bg1_bmp);	bg1_bmp = NULL; }
		bg1_bmp = loadbmp0(text/*filename*/, 0, 1);
//		#if (1==USE_EXTRACT_WAIT)
//		now_exetracting=60;
//		#endif
		break;
	}
}

/*---------------------------------------------------------
	�q�֐�
---------------------------------------------------------*/

static void bg2_swap(void)
{
	if (0==which_bg)/* �t���O�̈Ӗ������Ȃ̂ŋt�ɂȂ� */
	{
		draw_bmp = bg1_bmp;
		load_bmp = bg0_bmp;
	}
	else
	{
		draw_bmp = bg0_bmp;
		load_bmp = bg1_bmp;
	}
}
#endif /* (1==USE_BG_LOAD) */

/*---------------------------------------------------------

---------------------------------------------------------*/

extern unsigned int conv_bg_alpha;

static int current_bg_alpha;
static int request_bg_alpha;

void set_bg_alpha(int set_bg_alpha)
{
	request_bg_alpha = set_bg_alpha;
}
/*---------------------------------------------------------

---------------------------------------------------------*/

static void tile_work(void)
{
//
	if (current_bg_alpha == request_bg_alpha) /*�ł����肻���ȉ\����r��*/
	{
		;
	}
	else
	{
		if (current_bg_alpha > request_bg_alpha)
		{
			current_bg_alpha -= 4/*6*/;
		}
		else
		{
			current_bg_alpha += 4/*6*/;
			if (245 < current_bg_alpha) 	/* ���킶�킷��̂� */
			{	current_bg_alpha = 255; 	}
		}
		/* converted  */
		u8 aaa =(current_bg_alpha/*>>1*/);
		conv_bg_alpha = ((aaa<<24)|(aaa<<16)|(aaa<<8)|(aaa<<0));
	}

//	if (current_bg_alpha < 250/*255*/ )
//			{	current_bg_alpha += 6;		}
//	else	{	current_bg_alpha = 255; 	}
//
	if (current_bg0_y_scroll_speed256 == request_bg0_y_scroll_speed256) /*�ł����肻���ȉ\����r��*/
	{
		;
	}
	else
	if (current_bg0_y_scroll_speed256 > request_bg0_y_scroll_speed256)
	{
		current_bg0_y_scroll_speed256--;
	}
	else
	{
		current_bg0_y_scroll_speed256++;
	}
//
	#if (1==USE_BG_LOAD)
	bg0_bmp_y256 -= current_bg0_y_scroll_speed256;
	//
	if (laster_sprit256 < t256(272) )
	{
		laster_sprit256 += current_bg0_y_scroll_speed256;
		sprit_flag = 1; 	/* �����ʒu�𔻒f����K�v������ */
	}
	else
	{
		laster_sprit256 = t256(272);
		sprit_flag = 0; 	/* �����ʒu�𔻒f����K�v���Ȃ� */
	}
	if (bg0_bmp_y256 < (0) )
	{
	//	if (1==exsist_tuika)	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
		if (0!=exsist_tuika)
		{
			exsist_tuika = 0;
			if (1 < number_of_bg)	/* 2���ȏ�bg�ǉ������ꍇ�̂݁A�������C����ݒ肷�� */
			{
				laster_sprit256 = t256(0);
				sprit_flag = 1; 	/* �����ʒu�𔻒f����K�v������ */
				/*	������ sprit_flag = 1; �������ɂ�����̂́A�ꌩ�����������A
					�����̏�����ς���Ƒ��̖��(���Z�����ɂ��덷�̒��덇�킹)���o�Ă����ƌ��������Ȃ�̂ŁA
					�����Œ�������킹�Ă���B */
			}
			bg2_swap();
		}
		bg0_bmp_y256 += ((bg0_bmp->h)<<8);
	}
	#endif /* (1==USE_BG_LOAD) */
//
	#if 0/*laster_sprit�f�o�b�O�p*/
	/* �p�l���̃X�R�A����laster_sprit�� �O���C�Y���ɒǉ�bg������ �\��������B���Ă����������������Ⴄ�B */
	((PLAYER_DATA *)player->data)->score		= (t256_floor(laster_sprit256));
	((PLAYER_DATA *)player->data)->graze_point	= number_of_bg;
	#endif
}

/*---------------------------------------------------------

---------------------------------------------------------*/
	#if 0//(1 != USE_GU)
	/* psp �̉t���͎c�����c��̂ŁA���ꂮ�炢����Ă��A�w��ǈႢ������Ȃ�(�A��60fps�t�߂œ����Ă�ꍇ�̂�) */
	/* 0 < type3 < 122 < type2 < 132 < type3 <	250 < type1 */
	int blit_type;
			if (250 < current_bg_alpha )	{	blit_type = BLIT_TYPE01_255;	}/* �A���t�@�w��ǖ����Ȃ珟��ɂȂ� */
	else	if (132 < current_bg_alpha )	{	blit_type = BLIT_TYPE03_000;	}
	else	if (122 < current_bg_alpha )	{	blit_type = BLIT_TYPE02_127;	}/* �����t�߂Ȃ�A����ɔ����Œ� */
	else									{	blit_type = BLIT_TYPE03_000;	}
//
	#endif


					/* ������ �֐��|�C���^�ŕ��򂳂���ƁA�ƂĂ��Ȃ��x���Bvsync����ĂȂ��Ƃ͂��� ��60fps �� 30fps �����ɂȂ� */
					/* ���Ď��́A���������� 16[msec]�ȏ�������Ď��Ȃ̂��ȁH�H�H */
					/* psp(MIPS) �� pc�ƈ���ă|�C���^�g�����ꍇ�̃y�i���e�B�[���ƂĂ��Ȃ��ł����B */
					/* �����炭CPU�����̖��߃L���b�V��(I-Chache)������̂ŁA�ƂĂ��Ȃ��x���Ȃ�񂾂낤�� */
					#if 0//(1 != USE_GU)
					if (BLIT_TYPE01_255==blit_type)
					#endif
					#if 0//(1 != USE_GU)
					else
					if (BLIT_TYPE02_127==blit_type)
					{
						(*pd) = (((*ps)&PSP_SCREEN_FORMAT_LMASK)>>1);	/* �Â��]��(�A���t�@�����Œ�) */
					}
					else	//if (BLIT_TYPE03_000==blit_type)
					{	/* �ėp�]��(�A���t�@�C��) */
						const Uint16 aaa = (*ps);
						(*pd) = (Uint16)(/*(PSP_SCREEN_FORMAT_AMASK)|*/((
							(((aaa & PSP_SCREEN_FORMAT_BMASK)*current_bg_alpha) & (PSP_SCREEN_FORMAT_BMASK<<8)) |
							(((aaa & PSP_SCREEN_FORMAT_GMASK)*current_bg_alpha) & (PSP_SCREEN_FORMAT_GMASK<<8)) |
							(((aaa & PSP_SCREEN_FORMAT_RMASK)*current_bg_alpha) & (PSP_SCREEN_FORMAT_RMASK<<8))
						)>>8));
					}
					#endif




#if 0000

enum
{
	BLIT_TYPE01_255 = 0,	/* �P���]��(�A���t�@�Ȃ�) */
	BLIT_TYPE02_127,		/* �Â��]��(�A���t�@�����Œ�) */
	BLIT_TYPE03_000,		/* �ėp�]��(�A���t�@�C��) */
};
static void tile_draw(SDL_Surface *src)
{
	/*blit*/
	{
		SDL_Surface *dst;
		dst = sdl_screen[SDL_00_SCREEN];
		if (SDL_MUSTLOCK(src))	{	SDL_LockSurface(src);	}/*���b�N����*/
		if (SDL_MUSTLOCK(dst))	{	SDL_LockSurface(dst);	}/*���b�N����*/
		{
			int src_max_h;		src_max_h = src->h;
			unsigned int jj;	jj = (t256_floor(bg0_bmp_y256));
			unsigned int j2;	j2 = (jj*380);
			unsigned int yy256;
			for (yy256=0; yy256<GAME_HEIGHT*256; yy256+=256)
			{
				Uint16 *pd; 	pd = (Uint16 *)dst->pixels + (yy256+yy256)/*(yy<<9)*/;	/*(yy*480)*/  /* (yy<<9); (yy*512)*/
			/*	Uint16 *ps; 	ps = (Uint16 *)src->pixels + (jj*380);*/	/*(jj*480)*/
				Uint16 *ps; 	ps = (Uint16 *)src->pixels + (j2);		/*(jj*480)*/
				jj++;j2 += (380);
				if (jj > (src_max_h-1)) {jj -= src_max_h;	j2 = (jj*380);}
				unsigned int xx;
				for (xx=0; xx<GAME_WIDTH; xx++)
				{
					{
						(*pd) = (*ps);	/* �P���]��(�A���t�@�Ȃ�) */
					}
					pd++;
					ps++;
				}
				if (0 != sprit_flag)		/* �����ʒu�𔻒f����K�v==1�����邩�H */
				{
					if ( (yy256+t256(1)) >/*=*/ (laster_sprit256) )/* ��ʕ����ʒu */	/* ���ӁF 256�Œ菬���_�Ȃ̂� >= �͎g���Ȃ� */
					{
						sprit_flag = 0; 	/* �����ʒu�𔻒f����K�v���Ȃ� */
						if (SDL_MUSTLOCK(src))	{	SDL_UnlockSurface(src); 	}/*���b�N����*/
						src = load_bmp;
						if (SDL_MUSTLOCK(src))	{	SDL_LockSurface(src);	}/*���b�N����*/
					}
				}
			}
		}
		if (SDL_MUSTLOCK(src))	{	SDL_UnlockSurface(src); 	}/*���b�N����*/
		if (SDL_MUSTLOCK(dst))	{	SDL_UnlockSurface(dst); 	}/*���b�N����*/
	}
}
					#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

//	#if (1==USE_EXTRACT_WAIT)
//	{
//		/* �摜�ǂݍ��ݒ�(IMG_ Load()�����̉摜�W�J����(libpng�Ƃ�libjpeg�Ƃ�)���x���̂ŏ���������) */
//		now_exetracting--;
//		if (0 < now_exetracting)
//		{
//			return; 	/* �����������ɕ`���Ȃ����Ă݂����ǁA�S�R�ς��Ȃ� */
//		}
//		now_exetracting=1;
//	}
//	#endif

void bg_work_draw(void)
{
	#if (1==USE_BG_LOAD)
	if (0==number_of_bg)
	{
		psp_clear_screen();
	}
	else
	#endif /* (1==USE_BG_LOAD) */
	{
		tile_work();
	//	#if (1 != USE_GU)
		#if 0000
		tile_draw(draw_bmp/*bg0_bmp*/);
		#else
		/* (1 == USE_GU) */
		psp_clear_screen();
		#endif
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
extern void clouds_destroy(void);
void bg2_destroy(void)
{
	#if (1==USE_BG_LOAD)
	if (0!=number_of_bg)
	{
		number_of_bg = 0;
		//tile_remove();
		//static void tile_remove(void)
		{
			if (NULL != bg0_bmp)	{	unloadbmp_by_surface(bg0_bmp);	bg0_bmp = NULL; }
			if (NULL != bg1_bmp)	{	unloadbmp_by_surface(bg1_bmp);	bg1_bmp = NULL; }
		}
	}
	#endif /* (1==USE_BG_LOAD) */
	clouds_destroy();
}


//extern int tile_bg2_add(void/*int lev*/);
/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
extern int tiny_strcmp(char *aaa, const char *bbb);
void add_enemy_load_bg(STAGE_DATA *l)
{
#if (1==USE_BG_LOAD)
	char *text;
	text=l->user_string;
	//	if (NULL != (text) )
	//if ( 0 == tiny_strcmp(text,"0") ) /* �t�@�C�������O�̏ꍇ�V�X�e���R�}���h[�g���\��] */
	if ( '0' == text[0] )	/* �t�@�C������1���ڂ��O�̏ꍇ�V�X�e���R�}���h[�g���\��] */
	{
		;
	}
	#if (1==USE_BG_LOAD)
	else
	{
		load_btile(l, which_bg/*0*/ /*1*/);
		which_bg++;
		which_bg &= 1;
		#if 1
		number_of_bg++;
		if (/*0*/1 == number_of_bg) 	/* ����̂�*/
		#else
		/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
		/* ����ł�����???�Ȃ񂾂��ǎ��M�Ȃ��Ȃ�(���r�A����Z�̔�) */
		if (/*0*/0 == number_of_bg++)	/* ����̂�*/
		#endif
		{
		//	number_of_bg = 1;
			current_bg_alpha = 0;		/* ����̂�*/
			request_bg_alpha = 255; 	/* ����̂�*/
			bg0_bmp_y256 = 0;			/* ����̂�*/
			draw_bmp = bg0_bmp; 		/* ����̂݃��[�h���ɕ`��p�ɂ��� */
			load_bmp = bg0_bmp;
		}
		exsist_tuika			= 1;
	}
	#endif /* (1==USE_BG_LOAD) */
#endif /* (1==USE_BG_LOAD) */
}

/*---------------------------------------------------------
	�X�e�[�W�ǂݍ��݊J�n���ɁA���񏉊�������
---------------------------------------------------------*/
void bg2_start_stage(void)
{
	#if (1==USE_BG_LOAD)
	number_of_bg			= 0;	/* ����݂̂𔻒f����̂ƌ��p */
	which_bg				= 0 /*1*/ /*0*/;
	exsist_tuika			= 0;
	#endif /* (1==USE_BG_LOAD) */
	bg2_destroy();
//	if (1==use_clouds) {}
	request_bg0_y_scroll_speed256 = current_bg0_y_scroll_speed256 = t256(0.5);/*�����l*/
}

/*---------------------------------------------------------
	psp�N�����Ɉ�x��������������
---------------------------------------------------------*/
extern void clouds_system_init(void);
void bg2_system_init(void)
{
//	#if (1==USE_EXTRACT_WAIT)
//	now_exetracting 		= 1;
//	#endif

//	number_of_bg			= 0;
//	use_clouds				= 0;
	/* �摜�ǂݍ��ݗp�T�[�t�F�C�X */
	#if (1==USE_BG_LOAD)
	bg0_bmp 				= //NULL;
	bg1_bmp 				= //NULL;
	draw_bmp				= //NULL/*bg0_bmp*/;
	load_bmp				= NULL/*bg0_bmp*/;
	sprit_flag				= 0;	/* �����ʒu�𔻒f����K�v���Ȃ� */
	laster_sprit256 		= t256(272);
	#endif /* (1==USE_BG_LOAD) */
	clouds_system_init();
}

/*---------------------------------------------------------
	BG�R���g���[���R�}���h(�e�X�g��)
---------------------------------------------------------*/
enum
{
	BG2_00_ERROR = 0,
	BG2_01_SET_SCROOL_OFFSET,
	BG2_02_BG_STOP,
//	BG2_03_BG_SWAP,
	BG2_03_DESTOROY_CLOUDS,
};
void bg2_control(STAGE_DATA *l)
{
	short xxx;
	short yyy;
//	short speed256;
	xxx = l->user_x;
	yyy = l->user_y;
//	speed256 = l->scroll_speed256;
	//
	request_bg0_y_scroll_speed256 = l->scroll_speed256; 	/* bg0�̃X�N���[���A�\�񑬓x��ݒ� */
	switch (xxx)
	{
	case BG2_01_SET_SCROOL_OFFSET:	/* �X�N���[���l�𒼐ڃZ�b�g */
		#if (1==USE_BG_LOAD)
		bg0_bmp_y256 = yyy;
		#endif /* (1==USE_BG_LOAD) */
		break;
	case BG2_02_BG_STOP:	/* �e�X�g�� */
		#if (1==USE_BG_LOAD)
		laster_sprit256 = t256(272);
		sprit_flag = 0; 	/* �����ʒu�𔻒f����K�v���Ȃ� */
		#endif /* (1==USE_BG_LOAD) */
		break;
//	case BG2_03_BG_SWAP:	/* �e�X�g�� */
//		bg0_bmp_y256 = 0;
//		which_bg++;
//		which_bg &= 1;
//		bg2_swap();
//		break;
	case BG2_03_DESTOROY_CLOUDS:	/* �e�X�g�� */
		clouds_destroy();
		break;
	}
}
