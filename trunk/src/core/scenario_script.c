
/*---------------------------------------------------------
	�S�p�����\�����C�u���� zenkaku.c / shinonomefont.c

	PSP�ŕ��ՂɊ�����\�����邽�߂Ɍ��J���܂��B
	�t�H���g�͓��_�t�H���g���g�p�����Ē����Ă��܂��B

���v����
	���̃��C�u�����̓t���[�E�F�A�Ƃ��A���ɋ��i�͗v�����܂��񂪁A
	���̃��C�u�����𗘗p���ă\�t�g�E�F�A�����J������́A�h�L�������g����
	�ȉ��̕�����}�����Ē����܂��悤�A���肢�v���܂��B

	�u���̃\�t�g�E�F�A��mediumgauge���쐬�̑S�p�����\�����C�u�������g�p���Ă��܂��B�v

���d�l��
	���������ȃv���O�����ł��̂ŁA���ǂ݂���������΂킩��Ǝv���܂����A
	zenkaku.h �ɒ�`����Ă��� mh_print() �ɍ��W�A������A�F��n���Ε`�悳��܂��B
---------------------------------------------------------*/

#include "game_main.h"
#include "scenario_script.h"


#define FONT_WIDTH			(8)
#define FONT_HEIGHT 		(16)

/*---------------------------------------------------------
	shift jis�R�[�h�A�S�p1�o�C�g�ڂ��ǂ������肷��q�֐�
---------------------------------------------------------*/

static int is_kanji_1st(/*unsigned char*/int/*int*/ high_byte)
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


/*
�����̖��Ƃ��āA���_�t�H���g16�ɂ�
2�o�C�g���p�����̓t�H���g�f�[�^�[���Ȃ�(�쐬����ĂȂ��B�쐬����Ύg����)
���̂��� 2�o�C�g���p�����͕\������Ȃ��B(�f�[�^�[���S�ċ󔒂̈�)
//
���̏�ŁA�����I�Ȗ��Ƃ��āApsp��2�o�C�g���p�������T�|�[�g����Ӗ����Ȃ��C������B
//
	http://www.wdic.org/w/WDIC/%E3%82%B7%E3%83%95%E3%83%88JIS
//
���ߕ���(�V�t�gJIS��2�o�C�g�ڂ�0x5c�A���Ȃ킿'\'('�_')�ł��镶���̂��ƁB)
	�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
*/

/* USE_HARF_KANJI == (0): shift jis�R�[�h�A$fdxx ��2�o�C�g���p�������T�|�[�g���Ȃ��ꍇ */
/* USE_HARF_KANJI == (1): shift jis�R�[�h�A$fdxx ��2�o�C�g���p�������T�|�[�g����ꍇ */
#define USE_HARF_KANJI		(0)
#if (1==USE_HARF_KANJI)
/*---------------------------------------------------------
	�����킪shift jis�����Ɣ������Ă���ꍇ�ɁA
	(�܂�is_kanji()�����̏ꍇ��p)
	2�o�C�g���p���ǂ����𕶎��P�ʂŕԂ��q�֐�
---------------------------------------------------------*/

static int size_of_kanji(/*unsigned char*/int high_byte)
{
	high_byte &= 0xff;
	if (0xfd == high_byte)	// 2�o�C�g���p
	{			return (1); }
	else	{	return (2); }
}
#endif

/*---------------------------------------------------------
	���_�����R�[�h�K�i �̕�����`�悷��q�֐�
---------------------------------------------------------*/

static int font_color_number;

extern const char shinonome_font16p[];
/*
	x				�\�����W
	y				�\�����W
	col_n			�t�H���g�̎��
	code_offset 	����
//
	dx		�h�b�gx
	dy		�h�b�gy
	bit 	���ׂ�byte���
	bitcnt	���ׂĂ�byte���㉽�񒲂ׂ��邩:�ő�8��(��q)
			bit��&���邱�ƂŒ��ׂĂ�byte��bit���𓾂�B
*/
#if (1)
static UINT16 *bg_story_window_image;
static int bg_story_window_pitch;
static int bg_story_window_width;
#endif
#if (1)
static UINT16 *drawmap_image;
static int drawmap_pitch;
static int drawmap_width;
#endif

extern UINT16 *msg_window_image;
static int msg_window_pitch;
static int msg_window_width;

static Uint32 getpixel16(SDL_Surface *surface, int x, int y)
{
	Uint8 *p = (Uint8 *)surface->pixels+y*surface->pitch+x+x;
	return (*(Uint16 *)p);
}

static void putpixel16(/*SDL_Surface *surface,*/ int x, int y, Uint32 pixel)
{
	Uint8 *p = (Uint8 *)drawmap_image/*surface->pixels*/+y*drawmap_pitch/*surface->pitch*/+x+x;
	*(Uint16 *)p=pixel;
}

static void draw_shinonome_moji(/*SDL_Surface *drawmap,*/ SDL_Surface *backmap, int x, int y, int code_offset, int hidari_gawa)
{
//	if (1==hidari_gawa) 	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	if (0!=hidari_gawa)
	{
		x += FONT_WIDTH;
	}
	if (x > (/*drawmap->w*/drawmap_width-FONT_WIDTH) )	/* �͈͊O�Ȃ� */
	{
		return;/* �`�悵�Ȃ� */
	}
	int haikei_offset;
//	if (1==hidari_gawa) 	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	if (0!=hidari_gawa)
	{
		code_offset++;
		haikei_offset=8;
	}
	else
	{
		haikei_offset=0;
	}
	haikei_offset += (font_color_number<<4);
	code_offset<<=4;
	int bit;
	int bitcnt;
	bitcnt = 0;
	bit = 0;
	int dy;
	for (dy=0; dy<FONT_HEIGHT; dy++)
	{
		int dx;
		for (dx=0; dx<FONT_WIDTH; dx++)
		{
			bitcnt >>= 1;
			if (0==bitcnt)	// (0==bitcnt)�̎�
			{
				bit = shinonome_font16p[code_offset++];
				bitcnt = 0x80;		// 1byte�̏��ʂ�8�̈� 80 40 20 10 8 4 2 1(0�ɂȂ����炷��80�ɂȂ邽�߃J�E���g����Ȃ�)
			}
			if (bit & bitcnt)
			{
				Uint32 dot;
				dot=getpixel16(backmap, (dx+haikei_offset), dy);	/*font_color_bitmap*/
				putpixel16(/*drawmap,*/ dx+x, dy+y, dot);
			}
		}
	}
}

/*---------------------------------------------------------
http://openlab.ring.gr.jp/efont/shinonome/
	shift jis�����R�[�h�K�i �� ���_�����R�[�h�K�i �֕ϊ���A�`�悷��
---------------------------------------------------------*/

static void draw_sjis_kanji(/*SDL_Surface *drawmap,*/ SDL_Surface *backmap, int x, int y, int high_byte, int low_byte)
{
	if (0==high_byte)	/* ���p�����̏ꍇ(ascii�R�[�h) */
	{
		/* ascii�R�[�h �� shinonome�R�[�h �ϊ� */
		static	const	Uint8/*int*/ sinonome_ascii_bank_offset[16] =
		{// +0	  +1	+2	  +3	 +4    +5	 +6    +7	  +8	+9	  +a	+b	   +c	 +d    +e	 +f
			0xff, 0xff, 0x00, 0x10,  0x20, 0x30, 0x40, 0x50,  0xff, 0xff, 0x60, 0x70,  0x80, 0x90, 0xff, 0xff
		};
	//		 if (low_byte < 0x20)	{	return; }				// ascii 0x00-0x1f
	//	else if (low_byte < 0x80)	{	low_byte -= 0x20; } 	// ascii 0x20-0x7f
	//	else if (low_byte < 0xa0)	{	return; }				// ascii 0x80-0x9f
	//	else if (low_byte < 0xe0)	{	low_byte -= 0x40; } 	// ascii 0xa0-0xdf
	//	else						{	return; }				// ascii 0xe0-0xff
	//	code_offset <<= 4;	//code_offset *= 16;	/* 9 */ 	// �����ł�9�͑���1�̕����Ɏg���o�C�g���B(1byte == 8bit)*9=72 == 6*12
		Uint8 idx8;
		idx8 = sinonome_ascii_bank_offset[((low_byte>>4) & 0x0f)];
		/* ���݃}�b�v�� 0xff �̏ꍇ�́A ���_�����R�[�h�K�i �ɂȂ������Ȃ̂ŕ\���ł��Ȃ� */
		if (0xff == idx8)
		{
			return;
		}
		idx8 = ((idx8)|((low_byte & 0x0f)));
		draw_shinonome_moji(/*drawmap,*/ backmap, x, y, (idx8), 0);/*low_byte<<4*/
	}
	else				/* �S�p�����̏ꍇ(shift jis�R�[�h) */
	{
		#if 1
		/*���ʃo�C�g�̃`�F�b�N*/
		if (low_byte < 0x40)	// 0x40����(0x00-0x3f)�̏ꍇ�� shift jis�����R�[�h�̈� �ł͂Ȃ��B
		{
			return;
		}
		#endif
		low_byte -= 0x40;
		/* ��ʃo�C�g���� ���_�����R�[�h�K�i �̃C���f�b�N�X�𓾂� */
		int idx;
		#define NN (-1)
		{
			/* ���_�����R�[�h�K�i �̑��݃}�b�v */
			//#define FDxx (47)/* ���p2�o�C�g�������� */
			static	const	signed char/*int*/ sinonome_sjis_bank_offset[0x40] =
			{// +0	+1	+2	+3	 +4  +5  +6  +7   +8  +9  +a  +b   +c  +d  +e  +f
				NN,  0,  1,  2,   3, NN, NN,  4,   5,  6,  7,  8,	9, 10, 11, 12,		/* 0x80xx */
				13, 14, 15, 16,  17, 18, 19, 20,  21, 22, 23, 24,  25, 26, 27, 28,		/* 0x90xx */
				29, 30, 31, 32,  33, 34, 35, 36,  37, 38, 39, NN,  NN, NN, NN, NN,		/* 0xe0xx */
				NN, NN, NN, NN,  NN, NN, NN, NN,  NN, NN, NN, NN,  NN, NN, NN, NN		/* 0xf0xx */
			//	�ȉ��̗̈�͓��_�t�H���g16�ɂ̓t�H���g�f�[�^�[���Ȃ�(eaa4���ŏI�f�[�^�[������)
			//	29, 30, 31, 32,  33, 34, 35, 36,  37, 38, 39, NN,  NN, 40, 41, NN,		/* 0xe0xx */	���_�t�H���g16�ɂ̓t�H���g�f�[�^�[���Ȃ�
			//	NN, NN, NN, NN,  NN, NN, NN, NN,  42, 43, 44, 45,  46, 47, NN, NN		/* 0xf0xx */	���_�t�H���g16�ɂ̓t�H���g�f�[�^�[���Ȃ�
			};
			idx = sinonome_sjis_bank_offset[high_byte & 0x3f];
		}
		/* ���݃}�b�v�� NN �̏ꍇ�́A ���_�����R�[�h�K�i �ɂȂ������Ȃ̂ŕ\���ł��Ȃ� */
		if (NN == idx)
		{
			return;
		}
		#undef NN
		//
		idx = (16*10)/*���p�������X�L�b�v*/ + (idx*2*192)/*codetbl�P������192����*/;
		#if (1==USE_HARF_KANJI)
		if (1 == size_of_kanji(high_byte))	// 2�o�C�g���p
		{
			idx += (low_byte);
		//	code_offset <<= 4;	//code_offset *= 16;	/* 9 */ 	// �����ł�9�͑���1�̕����Ɏg���o�C�g���B(1byte == 8bit)*9=72 == 6*12
			draw_shinonome_moji(/*drawmap,*/ backmap, x, y, (idx) , 0);
		}
		else	/* if ( 2 == size_of_kanji(high_byte))*/	// 2�o�C�g�S�p
		#endif
		{
			idx += (low_byte+low_byte);/* x2 */
			draw_shinonome_moji(/*drawmap,*/ backmap, x, y, (idx) , 0); /* �E���� */
			draw_shinonome_moji(/*drawmap,*/ backmap, x, y, (idx) , 1); /* ������ */
		}
	}
}

/*---------------------------------------------------------
	������̕`��
---------------------------------------------------------*/

//extern int count_char;/*scenario.c*/

static /*extern*/ /*static*/ int cursor_x;
static /*extern*/ /*static*/ int cursor_y;

static int my_string_offset/*=0*/;
static void clear_my_string_offset(void)
{
	my_string_offset=0;/* ������ */
}

/*---------------------------------------------------------
	�J�[�\�����z�[���|�W�V�����ֈړ�
---------------------------------------------------------*/
static void home_cursor(void)
{
	cursor_x = 0;
	cursor_y = 0;
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static SDL_Surface *msg_window/*=NULL*/;		/* ���b�Z�[�W�E�B���h�E */
static void msg_window_clear(void)
{
	SDL_FillRect(msg_window,NULL,SDL_MapRGB(msg_window->format,0,0,0));
}
//static void msg_window_init(void)
//{
//
//}

/*---------------------------------------------------------

---------------------------------------------------------*/

		#if 0
		/*???*/
	//	if (NULL != font_color_bitmap)	{	SDL_FreeSurface(font_color_bitmap); 		font_color_bitmap	= NULL; }
		if (NULL != font_color_bitmap)	{	unloadbmp_by_surface(font_color_bitmap);	font_color_bitmap	= NULL; }
	//	font_color_bitmap=loadbmp(buffer);
	//	esc_len=0;
		{
			char fn[64];
			sp rintf(fn,"%s/" "fonts/font_bg%02d.png", data_dir, color_type);		/* �����񏈗�(sp rintf)�͔��ɒx�� */
			font_color_bitmap = IMG_Load(fn);/*�����Ń��[�h����Ɗm���ɏ������������*/
		}
		font_color_bitmap = SDL_DisplayFormat(font_color_bitmap);
		#endif
			#if 0
			unloadbmp_by_surface(font_color_bitmap);
			font_color_bitmap = NULL;
			#endif

/*---------------------------------------------------------
	shift jis���� �������Ȃ܂��菑�̂̕������`�悷�� (wait�w��ŁAframe���Ɉꕶ���`��B wait0�ŁA������S�`��)
---------------------------------------------------------*/
/*
	drawmap 			�`�����ݐ�(�������ݐ�Ȃ�source(��)����Ȃ���destination(��))
	rect				�`�����ݗ̈�
	str 				�`�����ޕ�����
	color_type			�`�����ޏ��̂̐F���w�肷��摜�ԍ�
	wait				1���Â`���ꍇ�ɁA1�̕����Ɋ|���鎞��
						0==wait �̏ꍇ�͈��őS�����`�悷��
						�������`���Ă���r������ 0�ɕύX���邱�Ƃ��\
//
	my_string_offset	�����A�h���X�̃I�t�Z�b�g / 0 �̏ꍇ�� �������t���O
	count_width 		���ɕ��ׂ��镶���̐�
	font_color_bitmap	���̂̐F���w�肷��摜
*/

//static extern SDL_Surface *font_color_bitmap; /* ���̐F�p�^�[���摜 */
//static SDL_Surface *font_color_bitmap/*=NULL*/;
static SDL_Surface *font_bg_bitmap/*=NULL*/;


//static /*extern*/ int cursor_continue;				/* �J�[�\���p�� */

/* ���̏��K�v�Ȃ����ǁA�O������`���Ȃ�B */
/*extern*/ /*static*/ int print_kanji000(/*SDL_Surface *drawmap,*/ /*SDL_Rect *dummy_rect,*/ const char *str, int color_type, int wait)
{
	font_color_number = (color_type & 0x0f);

	static int count_width=1/* 0 (����Z�Ȃ̂�0�͊댯) */;/*???*/
//	static int esc_len;
	if (0 == my_string_offset)	//������
	{
		count_width=(int)((300/*(rect->w)*/)/FONT_WIDTH)+2;
		//
//		if (0==cursor_continue)
//		{	/* �J�[�\���p�����Ȃ� */
//			cursor_x=(0);
//			cursor_y=(0);
//		}
	//	else	/* �J�[�\���p������ */
	//	{
	//		cursor_continue=0;	/* ����́A�J�[�\���p�����Ȃ� */
	//	}
	}
	int terminate_this_frame;	terminate_this_frame=0;
	int need_draw_this_flame;	need_draw_this_flame=0;
//	if (0 == wait)	{		need_draw_this_flame = 1; } else
	{	/* wait �� 0 �̏ꍇ�͈��őS�����`�悷�� */
		static unsigned int count_wait_time;
		count_wait_time++;
		if (count_wait_time > wait)
		{
			count_wait_time = 0;
			need_draw_this_flame = 1;
		}
	}
//	if (1==need_draw_this_flame)	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	if (0!=need_draw_this_flame)
	{
			#define IS_LOCK_LOCAL_SCREEN (1)
			#if (1==IS_LOCK_LOCAL_SCREEN)
/// 		if (SDL_MUSTLOCK(drawmap))				{	SDL_LockSurface(drawmap);			}	/* ���b�N */
		//	if (SDL_MUSTLOCK(font_color_bitmap))	{	SDL_LockSurface(font_color_bitmap); }	/* ���b�N */
			if (SDL_MUSTLOCK(font_bg_bitmap))		{	SDL_LockSurface(font_bg_bitmap);	}	/* ���b�N */
			#endif
		loop:
		;
		/*unsigned char*/int high_byte;
		high_byte = (*(str+my_string_offset) & 0xff);
		if (0x00 != high_byte )
		{
		//	cursor_x=((my_string_offset+count_char+esc_len)%count_width);	/* ���Z(%)�͔��ɒx�� */
		//	cursor_y=((my_string_offset+count_char+esc_len)/count_width);	/* ���Z(/)�͔��ɒx�� */
		//
			int xx;
			int yy;
			xx = 0/*(rect->x)*/+(cursor_x)*(FONT_WIDTH);
			yy = 0/*(rect->y)*/+(cursor_y)*(FONT_HEIGHT+2);
		//	�p�t�H�[�}���X���ቺ����̂łȂ�ׂ����b�N���Ȃ��B(���b�N�͍ŏ�����)
		/* �����b�N */
			if (is_kanji_1st(high_byte)) /* shift jis ���� */
			{
			/*unsigned char*/int low_byte;	low_byte = (*(str+my_string_offset+1) & 0xff);
			//	draw_sjis_kanji(/*drawmap,*/ font_color_bitmap, xx, yy, high_byte, low_byte);
				draw_sjis_kanji(/*drawmap,*/ font_bg_bitmap, xx, yy, high_byte, low_byte);
				#if (1==USE_HARF_KANJI)
				my_string_offset  += size_of_kanji(high_byte);	/* �g�����p�����̈� fdxx �̏ꍇ�͔��p�ɂȂ� */
				cursor_x		  += size_of_kanji(high_byte);	/* �g�����p�����̈� fdxx �̏ꍇ�͔��p�ɂȂ� */
				#else
				my_string_offset  += (2);
				cursor_x		  += (2);
				#endif
			}
			else				/* ascii���� */
			{
				if ('\\' == high_byte)		/* �G�X�P�[�v�V�[�N�G���X����(escape sequence) */
				{
					/*unsigned char*/int escape_sequence = (*(str+my_string_offset+1) & 0xff);
					if ('n' == escape_sequence) 	/* ���s�R�[�h */
					{
					//	esc_len += count_width-(cursor_x)-2;
						my_string_offset += 2;
						cursor_x = 999;/* �K���ɑ傫�Ȓl���w�肵�ĉ��s������ */
					//	cursor_x=0;
					//	cursor_y++;
					}
				}
				else					/* ���p���� */
				{
				//	draw_sjis_kanji(/*drawmap,*/ font_color_bitmap, xx, yy, 0, high_byte);
					draw_sjis_kanji(/*drawmap,*/ font_bg_bitmap, xx, yy, 0, high_byte);
					my_string_offset++;
					cursor_x++;
				}
			}/* �����b�N���� */
			/* ���s���� */
			if ( (count_width-1) < cursor_x)
			{
				cursor_x=0;
				cursor_y++;
				#define MAX_cursor_y (2)
				if (MAX_cursor_y < cursor_y)
				{
					cursor_y = 0;
				//	home_cursor();
					msg_window_clear();
				}
			}
		//	terminate_this_frame=0; 	/* continue frame */
			if (0 == wait)	/* wait �� 0 �̏ꍇ�͈��őS�����`�悷�� */
			{
				goto loop;	//loop����
			}
		}/*while*/
		else			//�I������
		{
			terminate_this_frame = 1;	/* end frame */
		}
			#if (1==IS_LOCK_LOCAL_SCREEN)
/// 		if (SDL_MUSTLOCK(drawmap))				{	SDL_UnlockSurface(drawmap); 			}	/* ���b�N���� */
		//	if (SDL_MUSTLOCK(font_color_bitmap))	{	SDL_UnlockSurface(font_color_bitmap);	}	/* ���b�N���� */
			if (SDL_MUSTLOCK(font_bg_bitmap))		{	SDL_UnlockSurface(font_bg_bitmap);		}	/* ���b�N���� */
			#endif
	//	if (1==terminate_this_frame)	//�I������	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
		if (0!=terminate_this_frame)	//�I������
		{
			{
			//	count_width=1/* 0 (����Z�Ȃ̂�0�͊댯) */;/*???*/
			//	count_char += my_string_offset/*+esc_len*/;
				my_string_offset = 0;	/* ����́A������ */
				//cursor_continue=0;	/* ����́A�J�[�\���p�����Ȃ� */
			}
		}
	}
	return (terminate_this_frame);		/* 0:continue, 1:end */
}

/*---------------------------------------------------------

---------------------------------------------------------*/

extern int errno;

// _script_command

// [�e�L�X�g�̃J�[�\������]

// [�e�L�X�g�\��/�N���b�N�҂�]
//		CURSOR		/*	CURSOR	�J�[�\���̏����� */
//					/*	CURSOR	�L�������J�[�\���ʒu�̌Ăяo�� */
//					/*	CURSOR	�J�[�\���ʒu�̋L�� */
//		CLICK		/* �N���b�N�҂� */
//		TEXT		/* ���b�Z�[�W�E�B���h�E�ւ̏������� */

//		BG_LOAD 	/* LO ADBG		�w�i�t�@�C���̕ύX */
//		OBJ_LOAD	/* LO ADSP		�ėp�X�v���C�g�̃��[�h */
//		OBJ_SWAP	/* RE LO ADSP	�ėp�X�v���C�g�̍ēǂݍ��� */

//		OBJ_MOVE	/* MO VESP		�X�v���C�g�̈ړ� */



//----------- [�p�~]
//		SUBG		/* [�p�~] [SCREEN]	SU BG�w�i�̕\��/��\�� */
//		SUTWINDOW	/* [�p�~] [SCREEN]	SU TWINDOW ���������G�E�B���h�E�̕\��/��\�� para2�̓X�R�A�p�l����on/off */

//----------- [�p�~]
//		CUR_HOME	/* [�p�~] [CURSOR]	CL CURSOR	�J�[�\���̏����� */
//		CUR_POP 	/* [�p�~] [CURSOR]	LO ADCUR	�L�������J�[�\���ʒu�̌Ăяo�� */
//		CUR_PUSH	/* [�p�~] [CURSOR]	SA VECUR	�J�[�\���ʒu�̋L�� */

//		SUSPRITE	/* [�p�~] [�p�~    ] SU SPRITE	�ėp�X�v���C�g�̕\��/��\�� */
//		OBJ_SET 	/* [�p�~] [�p�~    ] PA RAMSP	�ėp�X�v���C�g�̃A���t�@�Z�b�g/�A�j���[�V����	*/


//		BGTEXT		/*					 BG TEXT	�w�i�Ƀe�L�X�g��ł����� */
//		FILTER		/* [�p�~] [�p�~    ] FI LTER */
//		SUFILTER	/* [�p�~] [�p�~    ] SU FILTER	�t�B���^�[�\��/��\��&���ߓx */

//		SUWINDOW	/* [�p�~] [�p�~    ] SU WINDOW	���b�Z�[�W�E�B���h�E�̕\��/��\�� para2��bpp���x�B*/
//		SUL 		/* [�p�~] [SU SPRITE] SU L		�����G���X�v���C�g�̕\��/��\�� */
//		SUR 		/* [�p�~] [SU SPRITE] SU R		�����G�E�X�v���C�g�̕\��/��\�� */
//		JUMP		/* [�p�~] [�p�~    ] JU MP		pause�{�^��������������jump��B */ /*�p�~*/
//		LOADL		/* [�p�~] [LO ADSP ] LO ADL 	�����G�� �X�v���C�g�̃��[�h */
//		LOADR		/* [�p�~] [LO ADSP ] LO ADR 	�����G�E �X�v���C�g�̃��[�h */
//		WAIT		/* [�p�~] [�p�~    ]  wait */

//		TWINDOW 	/* TW INDOW[�p�~] [�p�~    ]  �����G�����E�B���h�E�̏����� */

enum
{
// [�e�L�X�g�̃J�[�\������]
	SCP_CUR_POP,
	SCP_CUR_PUSH,
	SCP_CUR_HOME,
// [�e�L�X�g�\��/�N���b�N�҂�]
	SCP_CLICK,
	SCP_BG_WRITE_TEXT,
	SCP_TEXT_WRITE_TEXT,
	SCP_TEXT,
//
	SCP_BGM,
//
//	SCP_FILTER,/*�p�~*/
//	SCP_JUMP,/*�p�~*/
	SCP_BG_LOAD,
//	SCP_LOAD_LR,/*�p�~*/
	SCP_OBJ_LOAD,
	SCP_OBJ_MOVE,
//	SCP_OBJ_SET,/*�p�~*/
	SCP_OBJ_SWAP,

	SCP_SCREEN_BG,
	SCP_SCREEN_TEXT,
	SCP_SCREEN_PANEL,

//	SCP_SUFILTER,/*�p�~*/
//	SCP_SUL,/*�p�~*/
//	SCP_SUR,/*�p�~*/
//	SCP_SUSPRITE,/*�p�~*/
//	SCP_SUWINDOW,/*�p�~*/
//	SCP_TWINDOW,/*�p�~*/
//	SCP_WAIT/*�p�~*/
};

typedef struct _scenario_script
{
	int command;
	int done;		// �I��������ǂ���
	int chain;		// �������s���ǂ���
					// 0=�Ⴄ�� 1=1�� 2=2��
	//void *data;/*�p�~*/
//
	int para1;
	int para2;
	int para3;
	int para4;
	struct _scenario_script *next;
//
	char para0[(7*32)/*200*/];/* 32�̔{�� */
} S_SCRIPT;/* == 256bytes */


typedef struct /*_sc_sprite*/
{
	int/*int*/ x;		/* x���W���� */
	int/*int*/ y;		/* y���W���� */
	int x256;						/* ���x�m�ۗp(256�Œ菬���_�`��) */
	int y256;						/* ���x�m�ۗp(256�Œ菬���_�`��) */
//[4]
	int w;							/* �� */			//	int cw; 	/* ���S���W(��)���� */
	int h;							/* ���� */			//	int ch; 	/* ���S���W(����)���� */
//
	int target_x;					/* �ڕW���W */
	int target_y;					/* �ڕW���W */
//[8]
	int move_wait;					/* �������������Ƃ��̐���p */
	int angle512;		/* ���m�ȕ��� */	//	int r_course;	/* ��̂̕��� */
//[12]
	SDL_Surface *img;				/* Images */
//
//	Uint8 alpha255;					/* alpha�l */
//	Uint8 flags;					/* 0:��\��, 1:�\��, 2:tachie_window(2nd screen)�ɕ\��	�����ł͕\�������邩�ǂ����������� */
} SC_SPRITE;

/*---------------------------------------------------------

---------------------------------------------------------*/

/*************�O���[�o���ϐ�**************/
/* zenkaku.c */
// static extern int cursor_continue;			/* �J�[�\���p�� */
// /*static*/ int cursor_x; 					/* �J�[�\���ʒu */
// /*static*/ int cursor_y; 					/* �J�[�\���ʒu */
//int count_char/*=0*/; 						/* �J�[�\���ʒu */
//static int cursor/*=0*/;						/* count_char�ۑ��p */
static int cursor_x_chached;					/* �J�[�\���ʒu �ۑ��p */
static int cursor_y_chached;					/* �J�[�\���ʒu �ۑ��p */
/*****************************************/

/*	psp�ł� �����ŏ��������Ă����퓮�삵�Ȃ��ꍇ������B
	�N�����ɏ��������Ȃ��ƃ_���B(PC�̃\�[�X��psp�ŃR���p�C�����ē����Ȃ������̂ЂƂ�) */
static SDL_Surface *msg_window/*=NULL*/;		/* ���b�Z�[�W�E�B���h�E */
//static SDL_Surface *std_window/*=NULL*/;		/* �E�B���h�E�̔������g */

static SDL_Surface *bg_story_window;			/* �X�N���v�g�ɂ�����w�i */

static SDL_Surface *sdl_lock_surface;			/* ���b�N����T�[�t�F�C�X */

//static SDL_Surface *font_color_bitmap/*=NULL*/;
static SDL_Surface *font_bg_bitmap/*=NULL*/;

//
static S_SCRIPT *sscript/*=NULL*/;				/* ���ߕێ��p */
static S_SCRIPT *entry_script;					/* ���ߎ��W�p */
static S_SCRIPT *start_script/*=NULL*/; 		/* ���ߎ��s�p */
//
#define SPRITE_tachie_l (29)/*-3*/
#define SPRITE_tachie_r (30)/*-2*/
#define SPRITE_tachie_m (31)/*-1*/
#define SPRITE_MAX		(32)
static SC_SPRITE *std_obj[SPRITE_MAX/*32*/ /*20*/]; 		/* �ėp�X�v���C�g */
//static SC_SPRITE *tachie_r;
//static SC_SPRITE *tachie_l;
//


//

static int is_bg/*=0*/; 						/* �w�i�\��/��\���t���O */

//static int 380/*scenario_width*/; 			/* ���� */

/*---------------------------------------------------------
	�q�֐�
---------------------------------------------------------*/

/*---------------------------------------------------------

---------------------------------------------------------*/

static SDL_Surface *load_local01(char *filename, SDL_Surface *s2/*result*/, int alpha)
{
	char fn[128];
//	strcpy(fn, data_dir);	strcat(fn, "/scenario/");
	strcpy(fn, DIRECTRY_NAME_DATA "/"/*"scenario/"*/);
	strcat(fn, filename);
//
	if (NULL != s2)
	{
		SDL_FreeSurface(s2);
		s2 = NULL;
	}
//
	SDL_Surface *s1;/*temporaly*/
//	s1 = NULL;
	s1 = IMG_Load(fn);/*�����Ń��[�h����Ɗm���ɏ������������*/
	if ( NULL == s1 )
	{
		CHECKPOINT;
		error(ERR_FATAL, "script:cant load image %s:"/*" %s"*/,fn/*,SDL_GetError()*/);
//		return (NULL);
	}
	if (	(-1==alpha) /* �X�N���v�g���ŏȗ������ꍇ(�f�t�H���g) */
		|| (0==alpha)	/* ���������ꍇ */
	)
	{
		/* �A���t�@�g��Ȃ� */
		s2 = SDL_DisplayFormat(s1);
		SDL_SetColorKey(s2,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);	/* �J���[�L�[(�����F�A�����F)�͍� */
	}
	else
	{
		/* �A���t�@�g�� */
		s2 = SDL_DisplayFormatAlpha(s1);
	}
	if ( NULL == s2 )
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant convert image %s to display format:"/*" %s"*/,fn/*,SDL_GetError()*/);
//		return (NULL);
	}
	SDL_FreeSurface(s1);
	s1 = NULL;
	return (s2);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void remove_sc_sprite(SC_SPRITE *src)
{
	if (NULL != src)
	{
		SDL_FreeSurface(src->img);
		free (src);
		src = NULL;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void swap_my_sprite(char *filename, int no, int dummy_frames, int alpha)
{
	SC_SPRITE *tmp;
	if (no==-1) 		{	return; 		}
//	else if (no==-2)	{	tmp=std_obj[SPRITE_tachie_r/*32-2*/];	}	/* right */
//	else if (no==-3)	{	tmp=std_obj[SPRITE_tachie_l/*32-3*/];	}	/* left */
	else				{	tmp=std_obj[(no)&(SPRITE_MAX-1)];	}	/* �ėp�X�v���C�g */
	if (NULL==tmp)		{	return; 		}

	SDL_FreeSurface(tmp->img);

	SDL_Surface *suf = NULL;
	suf 		= load_local01(filename,suf,alpha);
	tmp->img	= suf;
//	tmp->frames = frames;
	tmp->w		= (suf->w/*/frames*/);	//tmp->cw		= ((tmp->w)>>1);
	tmp->h		= (suf->h); 		//tmp->ch		= ((tmp->h)>>1);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static /*SC_SPRITE **/void load_my_sprite(char *filename, int sc_sp_num, int xx, int yy/*int dummy_frames, int dummy_alpha, SDL_Rect *rect*/)
{
	if (NULL != std_obj[sc_sp_num]) 	{	remove_sc_sprite(std_obj[sc_sp_num]); }
	SC_SPRITE *tmp;
	tmp 				= mmalloc(sizeof(SC_SPRITE));
	SDL_Surface *suf	= NULL;
//	suf 				= load_local02(filename,0/*alpha*/);
	suf 				= load_local01(filename,suf,0/*alpha*/);
	if (NULL==suf)		{ return /*(NULL)*/;			}
	std_obj[sc_sp_num]	= tmp;
	tmp->img			= suf;
//	if (frames==-1) 	{	tmp->frames = 1;		}
//	else				{	tmp->frames = frames;	}
	tmp->w				= (suf->w/*/frames*/);	//tmp->cw			= ((tmp->w)>>1);
	tmp->h				= (suf->h); 		//tmp->ch			= ((tmp->h)>>1);
//
//	tmp->aktframe		= 0;
//	tmp->anim_speed 	= 0;
//	tmp->anim_type		= 0;
//	tmp->anim_time		= 0;
//	tmp->anim_count 	= 0;
	/*tmp->alpha255		= 255;*/	/*tmp->color32		= 0xffffffff;*/
//	tmp->flags			= 0;
	tmp->move_wait		= 0;
	tmp->x				= xx/*rect->x*/;
	tmp->y				= yy/*rect->y*/;
//	tmp->x				= rect->x-(rect->w*tmp->w);
//	tmp->y				= rect->y-(tmp->h);
//	tmp->x				= xx-(0*tmp->w);
//	tmp->y				= yy-(tmp->h);

//	return (tmp);
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static int inits;								/* �e�X�N���v�g�R�}���h���Ƃ̏��������� */

/*---------------------------------------------------------
	SCP_OBJ_MOVE	�X�v���C�g�̈ړ�
---------------------------------------------------------*/

static int do_move_sc_sprite(char *color32_str, int n, int x, int y, int speed_aaa/*, int alpha255*/)
{
	SC_SPRITE *sc;
	{
//		int n = parth_str_right_or_str_left_or_number(l_or_r);
//		if (n==STR_ERROR/*-1*/) 		{	return (2/*-1*/);	}/*�X�N���v�g�I��*/
	//	else if (n==STR_RIGHT/*-2*/)	{	sc = tachie_r;	}	/* right */
	//	else if (n==STR_LEFT/*-3*/) 	{	sc = tachie_l;	}	/* left */
	//	else
		{	sc = std_obj[((n)&(SPRITE_MAX-1))]; }	/* �ėp�X�v���C�g */
	}
//
	if (inits)	/* ����̂ݍs�� */
	{
		/* ���炩���߈ړ������ʒu���Z�o���Ă��� */
		sc->target_x	= sc->x + x;		/* �ړ��������W */
		sc->target_y	= sc->y + y;		/* �ړ��������W */
		sc->x256		= ((sc->x)<<8); 	/* ���x�m�ۗp */
		sc->y256		= ((sc->y)<<8); 	/* ���x�m�ۗp */
		/* �ړ����������߂� */
		sc->angle512	= atan_512(y,x);
	//	sc->color32 	= ((alpha255)<<8);	/* �F */
	//	sc->alpha		= ((alpha255)<<8);	/* �F */
		if ('f'==color32_str[2])
		{
		//	sc->alpha255 = (255);	/* �F */
			SDL_SetAlpha(sc->img, SDL_SRCALPHA, (255) );
		}
		else
		{
		//	sc->alpha255 = (180);	/* �F */
			SDL_SetAlpha(sc->img, SDL_SRCALPHA, (180) );
		}
	}
//
	/* 10�Ŋ���̂͒x�߂���̂ŁA3�r�b�g�V�t�g����(1/8)�̒l�ɂ��� */
	int speed256;
	speed256 = (speed_aaa<<(8-3));		/* ���x�m�ۗp */
	#if 0
	/* CW�̏ꍇ */
//	sc->x += ((cos512(sc->angle512)*speed)/10);
//	sc->y += ((sin512(sc->angle512)*speed)/10);
//	sc->x256 += ((cos512(sc->angle512)*speed256)>>8);	/* ���x�m�ۗp */
//	sc->y256 += ((sin512(sc->angle512)*speed256)>>8);	/* ���x�m�ۗp */
	#else
	/* CCW�̏ꍇ */
	sc->x256 += ((sin512(sc->angle512)*speed256)>>8);	/* ���x�m�ۗp */
	sc->y256 += ((cos512(sc->angle512)*speed256)>>8);	/* ���x�m�ۗp */
	#endif

//
	sc->x	= (t256_floor(sc->x256));		/* ���x�m�ۗp */
	sc->y	= (t256_floor(sc->y256));		/* ���x�m�ۗp */
	/* ��̂̕���: �Ȃ̂� �S�T�x��]���Ă��炵�āA��ʂQ�r�b�g����܂��ȕ��� */
	#if 0
	/* CW�̏ꍇ */
	switch ((((sc->angle512+64/*deg_360_to_512(45)*/) & 0x180 )))/*sc->r_course*/
	{	/* �ړ��������W�ɓ��������͂ݏo������A�����Ƃ���B */
	case (0<<7)/* 1:��(512_0) */:	if (sc->x >= sc->target_x)	{	goto move_complete; 	}	break;
	case (1<<7)/* 2:��(512_1) */:	if (sc->y <= sc->target_y)	{	goto move_complete; 	}	break;
	case (2<<7)/* 3:��(512_2) */:	if (sc->x <= sc->target_x)	{	goto move_complete; 	}	break;
	case (3<<7)/* 0:��(512_3) */:	if (sc->y >= sc->target_y)	{	goto move_complete; 	}	break;
	}
	#else
	/* CCW�̏ꍇ */
	switch ((((sc->angle512-64/*deg_360_to_512(45)*/) & 0x180 )))/*sc->r_course*/
	{	/* �ړ��������W�ɓ��������͂ݏo������A�����Ƃ���B */
//	case (0<<7)/* 2:��(512_1) */:	if (sc->y <= sc->target_y)	{	goto move_complete; 	}	break;
//	case (1<<7)/* 1:��(512_0) */:	if (sc->x >= sc->target_x)	{	goto move_complete; 	}	break;
//	case (2<<7)/* 0:��(512_3) */:	if (sc->y >= sc->target_y)	{	goto move_complete; 	}	break;
//	case (3<<7)/* 3:��(512_2) */:	if (sc->x <= sc->target_x)	{	goto move_complete; 	}	break;
	case (1<<7)/* 2:��(512_1) */:	if (sc->y <= sc->target_y)	{	goto move_complete; 	}	break;
	case (0<<7)/* 1:��(512_0) */:	if (sc->x >= sc->target_x)	{	goto move_complete; 	}	break;
	case (3<<7)/* 0:��(512_3) */:	if (sc->y >= sc->target_y)	{	goto move_complete; 	}	break;
	case (2<<7)/* 3:��(512_2) */:	if (sc->x <= sc->target_x)	{	goto move_complete; 	}	break;
	}
	#endif
	return (0);/*�ړ���*/
move_complete:
	/* �ړ����������ꍇ�́A���m�ȍ��W�ɏC������B */
	sc->x = sc->target_x;
	sc->y = sc->target_y;
	return (1);/*�ړ�����*/
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static void set_write_text(int is_bg_screen)
{
//	if (0==is_bg_screen)
	{
		#if (1)
		/* ���b�Z�[�W�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
		drawmap_image	= msg_window_image;
		drawmap_pitch	= msg_window_pitch;
		drawmap_width	= msg_window_width;
		sdl_lock_surface	= msg_window;
		#endif
	}
//	else
//	{
//		#if (1)
//		/* �w�i�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
//		drawmap_image	= bg_story_window_image;
//		drawmap_pitch	= bg_story_window_pitch;
//		drawmap_width	= bg_story_window_width;
//		sdl_lock_surface	= bg_story_window;
//		#endif
//	}
}
/*---------------------------------------------------------
	�O�����烁�b�Z�[�W�E�B���h�E�Ɋ�����`�悷��ꍇ�̃��Z�b�g
---------------------------------------------------------*/

/*static*/ void script_message_window_clear(void)
{
	msg_window_clear(); 	/* ���b�Z�[�W�E�B���h�E�̓��e�������B */
//	msg_window_init();/*???*/

//	inits				= 1;/*???*/
//	is_bg				= 0;/*???*/
//	draw_script_screen	= 0;/*???*/

	home_cursor();		/* �J�[�\�����z�[���|�W�V�����ֈړ� */
//	cursor_x_chached	= 0;/*???*/ /* �J�[�\�������� */
//	cursor_y_chached	= 0;/*???*/ /* �J�[�\�������� */
//	clear_my_string_offset();/*???*/

	set_write_text(0);		/* ���b�Z�[�W�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
}

/*---------------------------------------------------------

---------------------------------------------------------*/

//static SDL_Rect msgw_rect;				/* ���b�Z�[�W�E�B���h�E�\���ʒu */
//static void msgw_rect_init(int x,int y)
//{
//	msgw_rect.x = x;
//	msgw_rect.y = y;
//}

/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/ int draw_script_screen/*=0*/;					/* ����ӃE�B���h�E�\���t���O */

/*---------------------------------------------------------
	�V�X�e������(???)
---------------------------------------------------------*/

/*---------------------------------------------------------

---------------------------------------------------------*/

static void load_script_free(void)
{
	S_SCRIPT *l = sscript;
	S_SCRIPT *n;
	while (NULL != l)
	{
		n=l->next;
		//if (NULL != l->data)	{ free (l->data);	l->data = NULL; }
		free (l);
		l = n;
	}
	sscript = (S_SCRIPT *)NULL;
	entry_script = (S_SCRIPT *)NULL;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static char *load_command(char *c, char *buffer, int *end_arg)
{
	int i = 0;
	while ((*c != ' ') && (*c != 13))
	{
		i++;
		if (i >= 10)
		{	return ((char *)NULL);}
		*buffer++ = *c++;
	}
	if (*c == 13)	/* ���s�R�[�h�� OD OA */
	{
		*end_arg = 1;
	}
	*buffer = 0;		//NULL
	return (c);
}

/*---------------------------------------------------------
	�X�N���v�g�t�@�C���̕����񕔕��̓ǂݍ���
	-------------------------------------------------------
	shift jis ������2byte�ڂ������̏ꍇ��
	�G�X�P�[�v�V�[�N�G���X������2byte�ڂ������̏ꍇ�ł�
	��肪�Ȃ�
---------------------------------------------------------*/
static char *load_script_get_str(char *str, char *buffer, int *end_arg)
{
	int string_error_limiter;
	string_error_limiter = 200;/* 200 �����ȏ�̓G���[ */
	/*unsigned char*/int high_byte;
	{loop:;
		high_byte = ((*str) & 0xff);
		/* �󕶎���̉\�������邩��A�n�߂ɔ��� */
		if (',' == high_byte)	/* ','��؂�ł����܂��̏ꍇ */
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
			flag=0;
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
					flag=1;
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
#if 0
/*---------------------------------------------------------
	�悭�킩��Ȃ�
	-------------------------------------------------------
	���̂܂܂���sjis��2byte�ڂ�'\'�̏ꍇ�Ή��ł��Ȃ���
---------------------------------------------------------*/
static char *load_script_get_str(char *c, char *buffer, int *end_arg)
{
	int i = 0;
	int is_escape_sequence_mode;		/* �G�X�P�[�v */
	is_escape_sequence_mode = 0;		/* �G�X�P�[�v */
//
loop:;
	/*while*/
	if (
		(
			((',' == *c) && (1==is_escape_sequence_mode)) || (',' != *c)
		) &&
		(*c != 13)
	)
	{	;	}
	else
	{	goto loop_end;}
//
	{
		if (0 == is_escape_sequence_mode)
		{
			if ('\\' == *c)
			{
				if (',' == *(c+1))
				{
					is_escape_sequence_mode = 1;
					c++;
				}
			}
		}
		else
		if (is_escape_sequence_mode)
		{
			is_escape_sequence_mode = 0;
		}
		i++;
		if (i >= 200)	{	return ((char *)NULL);}
		*buffer++ = *c++;
	}
	goto loop;
//
loop_end:;
	if (*c == 13)
	{
		*end_arg = 1;
	}
	*buffer = 0;		//NULL
	return (c);
}
#endif
/*---------------------------------------------------------

---------------------------------------------------------*/

static char *load_int(char *ccc, int *number, int *line_terminate_end_flag)
{
	char buffer[32/*20*/];
	char *ddd = buffer;
	int i = 0;
	while ((isdigit(*ccc))||('-'==(*ccc)))		/* �����ɂ��Ή� / isdigit : �������ǂ����̔��� */
	{
		i++;
		if (i >= 32/*20*/)
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
	if (((','==(*ccc)) || (13==(*ccc))) && (','==(*(ccc-1))))	{	*number = -1; }
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
	PARAM_01 = 0,
	PARAM_02,
	PARAM_03,
	PARAM_04,
	PARAM_05,
//	PARAM_06,/*���g�p*/
//	PARAM_07,/*���g�p*/
//	PARAM_08,/*���g�p*/
	PARAM_MAX/*�ő吔*/
};

static void regist_script(
	char *command,
	char *c_p0,
//	int c_p1,	//c_pn[0]
//	int c_p2,	//c_pn[1]
//	int c_p3,	//c_pn[2]
//	int c_p4,	//c_pn[3]
//	int c_p5,	//c_pn[4]
//	int c_p6,	//c_pn[5]
//	int c_p7,	//c_pn[6]
	const int *c_pn,
	int chain)
{
/*
	��{�`
	command para0,para1,para2,para3
*/
	S_SCRIPT *new_script;

	new_script	=	mmalloc(sizeof(S_SCRIPT));

	strcpy(new_script->para0, c_p0);
	new_script->para1		= /*c_p1*/c_pn[PARAM_01];	/* �f�t�H���g */
	new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* �f�t�H���g */
	new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* �f�t�H���g */
	new_script->para4		= /*c_p3*/c_pn[PARAM_04];	/* �f�t�H���g */
//
	new_script->done	= 0;
	new_script->chain	= chain;
	new_script->next	= NULL;
//	new_script->data	= NULL;

	switch (*command)
	{
	case 'B':
					if (0==tiny_strcmp(command, "BG_LOAD")) 	{	new_script->command = SCP_BG_LOAD;			}	/* �w�i�t�@�C���̕ύX */
			else	if (0==tiny_strcmp(command, "BGM")) 		{	new_script->command = SCP_BGM;				}	/* BGM�ύX */
		break;
	case 'C':
		if (0==tiny_strcmp(command, 			"CURSOR"))
		{
					if (0==tiny_strcmp(c_p0,	"home"))		{	new_script->command = SCP_CUR_HOME; 		}	/* �J�[�\���̏����� */
			else	if (0==tiny_strcmp(c_p0,	"load")) 		{	new_script->command = SCP_CUR_POP;			}	/* �L�������J�[�\���ʒu�̌Ăяo�� */
			else	if (0==tiny_strcmp(c_p0,	"save"))		{	new_script->command = SCP_CUR_PUSH; 		}	/* �J�[�\���ʒu�̋L�� */
			else	if (0==tiny_strcmp(c_p0,	"click"))		{	new_script->command = SCP_CLICK;			}	/* �N���b�N�҂� */
			else	if (0==tiny_strcmp(c_p0,	"text"))		{	new_script->command = SCP_TEXT_WRITE_TEXT;	}	/* ���b�Z�[�W�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
			else	if (0==tiny_strcmp(c_p0,	"bg"))			{	new_script->command = SCP_BG_WRITE_TEXT;	}	/* BG�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
		}
		break;
	case 'O':
					if (0==tiny_strcmp(command, "OBJ_MOVE"))	{	new_script->command = SCP_OBJ_MOVE; 		}	/* �X�v���C�g�̈ړ� */
			else	if (0==tiny_strcmp(command, "OBJ_LOAD"))	{	new_script->command = SCP_OBJ_LOAD; 		}	/* �ėp�X�v���C�g�̃��[�h */
			else	if (0==tiny_strcmp(command, "OBJ_SWAP"))	{	new_script->command = SCP_OBJ_SWAP; 		}	/* �X�v���C�g�̍ēǂݍ��� */
		break;
	case 'S':
		if (0==tiny_strcmp(command, 			"SCREEN"))
		{
					if (0==tiny_strcmp(c_p0,	"panel"))		{	new_script->command = SCP_SCREEN_PANEL; 	}	/* �X�R�A�p�l���̕\��/��\�� */
			else	if (0==tiny_strcmp(c_p0,	"text"))		{	new_script->command = SCP_SCREEN_TEXT;		}	/* �e�L�X�g��ʂ̕\��/��\�� */
			else	if (0==tiny_strcmp(c_p0,	"bg"))			{	new_script->command = SCP_SCREEN_BG;		}	/* �w�i�̕\��/��\�� */
		}
		break;
	case 'T':
					if (0==tiny_strcmp(command, "TEXT"))		{	new_script->command = SCP_TEXT; 			}	/* ���b�Z�[�W�E�B���h�E�ւ̏������� */
		break;
	}

	if (NULL==entry_script)
	{	entry_script		= new_script;		}
	else
	{
		entry_script->next	= new_script;
		entry_script		= entry_script->next;
	}

	if (NULL==sscript)
	{	sscript 			= entry_script; 	}
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
	fd = sceIoOpen((char *)file_name, PSP_O_RDONLY, 0777);
	if (0 == fd)
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
	�X�N���v�g�ǂݍ���
	-------------------------------------------------------
	���̊֐��́A�d�l��u�ƂĂ������������܂��v

---------------------------------------------------------*/

static int load_scenario(char *src_filename)
{
	load_script_free();
/*
	��{�`
	command para0,para1,para2,para3
*/
	char filename[128];
//	sp rintf(filename, "%s/text/%s.txt", data_dir, src_filename);
	strcpy(filename, DIRECTRY_NAME_DATA "/text/");
	strcat(filename, src_filename);
	strcat(filename, ".txt");

//	FILE *fp;
	if (NULL == (/*fp =*/my_fopen(filename/*,"r"*/)))		/* �J���Ȃ������Ƃ� */
	{
		return (0);
	}

	int opecode_entrys		= 0;	/* ���߂����������܂�Ă��邩�ǂ����B */
	int line_num			= 0;	/* �s�ԍ� */
	int opecode_chains		= 0;	/* �A���������� */
	while (/*NULL*/0 != my_fgets(/*buffer_text_1_line,255,fp*/))
	{
		/****************** script_data �p ******************/
		char char_command[16/*15*/];		/* ��{�R�}���h */
		char c_p0[256/*200*/];
		int c_pn[PARAM_MAX/*6*/];
		{
			int kk;
			for (kk=0; kk<PARAM_MAX/*6*/; kk++)
			{
				c_pn[kk] = -1;	/* ���w��̏ꍇ �𔻕ʂ���� */
			}
		}
		/****************** script_search �p ****************/
		char *c = NULL; 			/* �������̍s�̕��͗p */
		int end_arg=0;				/* �s�� == �����̎擾�̒��~ */
		c = buffer_text_1_line; 	/* �擪�A�h���X��n���� */

		line_num++;

		/* skiped lines. */
		#if 0
		/* '\n'�������̂��I�������Ȃ�(???) */
		if (*c=='\n')		{	continue;	}
		while (isspace(*c)) {	c++;		}
		#else
		{my_isspace:;
			if (*c<=' ')
			{
				c++;
				if (*c==0x0a)
				{	/*goto loop;*/continue; }	/* skiped null line. */
				else
				{	goto my_isspace;	}
			}
		}
		#endif
		if (*c=='#')		{	continue;	}
		if (*c=='-')		{	c++;	opecode_chains++;	opecode_chains &= 0x0f; }	/* ���[�N��16�܂ł����Ȃ��̂ōő�16���� */
		else				{	opecode_chains = 0; }
	#if (1==USE_PARTH_DEBUG)
		#define GOTO_ERR_WARN goto err_warn
	#else
		#define GOTO_ERR_WARN continue
	#endif
		/* parth start */
		c = load_command(c, char_command, &end_arg);		/* ��{�R�}���h����(�I�y�R�[�h)�ǂݍ���  */
		if (NULL==c)										{	GOTO_ERR_WARN;	}	/* �ǂ߂Ȃ���΃G���[ */
		if (!end_arg)/* �s�� */
		{
			if (*c++ != ' ')								{	GOTO_ERR_WARN;	}	/* ��؂� */
			c = load_script_get_str(c, c_p0, &end_arg); 							/* ������R�}���h(�I�y�����h)�ǂݍ���  */
			if (NULL==c)									{	GOTO_ERR_WARN;	}	/* �ǂ߂Ȃ���΃G���[ */
		}
		{
			int kk;
			for (kk=0; kk<PARAM_MAX/*6*/; kk++)
			{
				if (!end_arg)
				{
					if (*c++ != ',')						{	GOTO_ERR_WARN;	}	/* ��؂� */
					c = load_int(c, &c_pn[kk], &end_arg);							/* ���l�R�}���h(�I�y�����h)�ǂݍ���  */
					if (NULL==c)							{	GOTO_ERR_WARN;	}	/* �ǂ߂Ȃ���΃G���[ */
				}
			}
		}
	//	regist_script(char_command, c_p0, c_pn[0],c_pn[1],c_pn[2],c_pn[3],c_pn[4],c_pn[5],c_pn[6], opecode_chains);
		regist_script(char_command, c_p0, c_pn, opecode_chains);
		opecode_entrys++;
		continue;
	#if (1==USE_PARTH_DEBUG)
	err_warn:
		error(ERR_WARN, "syntax error in scriptfile '%s', line no: %d", filename, line_num);
		continue;
	#endif
	}
	my_fclose (/*fp*/);
	//return (opecode_entrys);
	if (0 == opecode_entrys)
	{
		#if 0
		//ps pDebugScreenPrintf("can't entrys from this file %s",filename);
		//sc eKernelDelayThread(3000000);
		#endif
		error(ERR_WARN, "can't entrys from this file %s",filename);
		return (0);
	}
	return (1);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

//static extern int my_string_offset;
//static extern void clear_my_string_offset(void);
static void script_reset(void)
{
	int i;
	for (i=0; i<SPRITE_MAX/*20*/; i++)
	{
		remove_sc_sprite(std_obj[i]);
	}
//	remove_sc_sprite(std_obj[SPRITE_tachie_r]);
//	remove_sc_sprite(std_obj[SPRITE_tachie_l]);
	if (NULL != bg_story_window)	{	SDL_FreeSurface(bg_story_window);	bg_story_window 	= NULL; }
//	if (NULL != msg_window) 		{	SDL_FreeSurface(msg_window);		msg_window			= NULL; }

//	if (NULL != filter_window)		{	SDL_FreeSurface(filter_window); 	filter_window		= NULL; }
//	if (NULL != font_color_bitmap)	{	SDL_FreeSurface(font_color_bitmap); font_color_bitmap	= NULL; }
	load_script_free();

//	count_char=0;			/* �J�[�\���ʒu */
//	cursor=0;
	home_cursor();			/* �J�[�\�����z�[���|�W�V�����ֈړ� */
	cursor_x_chached = 0;	/* �J�[�\���ʒu �ۑ��p */
	cursor_y_chached = 0;	/* �J�[�\���ʒu �ۑ��p */

	clear_my_string_offset();	//	my_string_offset=0;
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static int bg_alpha;		/* �w�i���l�p */
static void load_bg_aaa(char *filename, int alpha)
{
	bg_story_window = load_local01(filename, bg_story_window, alpha);
	{
		bg_story_window_image	= (UINT16 *)bg_story_window->pixels;
		bg_story_window_pitch	= bg_story_window->pitch;
		bg_story_window_width	= bg_story_window->w;
	}
	SDL_SetColorKey(bg_story_window,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
}
static int load_my_bg(char *filename, int alpha, int fade_command, int set_alpha_speed)
{
	static int bg_alpha_speed;		/* �w�i���l�p */
	switch (fade_command)
	{
	case (-1):
		load_bg_aaa(filename, alpha);
	//	ssc->done = 1;
		return (1); /* �������� */
//		break;
	case 1:/*fade_in_command*/
		if (inits)/*fade_in_command_start*/
		{
		//	if (filename)
			{
				load_bg_aaa(filename, 0/*alpha*/);
			}
		//	ssc->done = 1;
			//
			bg_alpha = 0;
		//	if (-1 == set_alpha_speed)/*�p�����[�^�[�ȗ���*/
			{
				set_alpha_speed = 5;
			}
			bg_alpha_speed = set_alpha_speed;
		//	inits=0;
			is_bg				= 1;
		}
		else
		{
//			psp_clear_screen();
//			SDL_SetAlpha(bg_story_window, SDL_SRCALPHA, bg_alpha);
//			SDL_BlitSurface(bg_story_window, NULL, sdl_screen[SDL_00_SCREEN], NULL);
			bg_alpha += bg_alpha_speed/*5*/;
			if (250 < bg_alpha)
			{
				bg_alpha = 255;
				#if 0/*???*/
				/*	�����ǂ��킩��Ȃ����ǁA�����͂�������???�B */
				if (NULL != bg_story_window)
				{
					unloadbmp_by_surface(bg_story_window);
				}
				#endif
				return (1); 	/* �������� */
			}
		}
		break;
//	case 2:/*fade_out_command_start*/
//		if (inits)
//		{
//			inits=0;
//		}
//		else
//		{
//		//	if ()
//			{
//			//	return (1); /* �������� */
//			}
//		}
//		break;
	}
	return (0); 	/* ������ */
}


/*---------------------------------------------------------
	�X�v���C�g�I�u�W�F�N�g�̕`��/����
---------------------------------------------------------*/

static void draw_my_sprite(int start, int end)
{
	int nnn;
	for (nnn=start; nnn<=end; nnn++)
	{
		if (NULL != std_obj[nnn])
		{
		//	if (std_obj[nnn]->flags)	/* �ėp�X�v���C�g�̕\��/��\�� */
			{
				SDL_Rect src_r;
				SDL_Rect dst_r;
				src_r.x = (0)/*std_obj[nnn]->w*std_obj[nnn]->aktframe*/;
				src_r.y = (0);
				src_r.w = std_obj[nnn]->w;
				src_r.h = std_obj[nnn]->h;
				dst_r.x = std_obj[nnn]->x;
				dst_r.y = std_obj[nnn]->y;
				SDL_BlitSurface(std_obj[nnn]->img, &src_r, sdl_screen[SDL_00_SCREEN], &dst_r);
			}
			/* ����/�A�j���[�V���� */
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void s_draw_the_script(void)
{
	/* ----- �`��n ----- */
	/* 1. �܂��w�i��`�� */
	if (is_bg)
	{
		if (255 > bg_alpha)
		{
			SDL_SetAlpha(bg_story_window, SDL_SRCALPHA, bg_alpha);
		}
		SDL_BlitSurface(bg_story_window, NULL, sdl_screen[SDL_00_SCREEN], NULL);
	}
	/* 2. ���ɗ����G��`�� */
	draw_my_sprite(SPRITE_tachie_l,SPRITE_tachie_l);/* �����G 1�� */
	draw_my_sprite(SPRITE_tachie_r,SPRITE_tachie_r);/* �����G 1�� */
}


/*---------------------------------------------------------

---- ��肽������ ----
	1�t���[���ł͏I���Ȃ��������������Ƃ��̂��߂ɂ��̃R�}���h����̖��߂ɂ͑S��
	�I��������Ƃ�m�点�������t���Ă������ƁB=> done�ɑ���ŏI���B
	���chain���m�F���A0�ȊO�̒l�������Ă����玟�̊֐������s����(next��H��)�B
---------------------------------------------------------*/

static int n9;						/* ���̖��߂ɐi�߂邩�̔��� */

static int return_the_script;
static /*int*/void work_the_script(void)
{
	static int tmp_all[16/*15*/];	/* �����R�ɂ��g�����������B */
	S_SCRIPT *ssc = start_script;
	n9 = 0; 					/* ���̖��߂ɐi�߂邩�̔��� */
	if (inits)
	{
		int i;
		for (i=0; i<16/*15*/; i++)
		{
			tmp_all[i] = 0;
		}
	}
//
	int shot_ositenai;
	shot_ositenai = 1;	/* �V���b�g�����ĂȂ� */
	if (0 == my_pad)/* ���͉����ĂȂ� */
	{
		if (my_pad_alter & PSP_KEY_BOMB_CANCEL) 	/* �L�����Z������ */
		{
			while ((NULL != ssc->next)
			//	&& (SCP_JUMP != ssc->command)/*�p�~*/
				)
			{
				ssc->done = 1;
				ssc=ssc->next;
			}
			if (NULL == ssc->next)
			{
				script_reset();
				return_the_script=(1);
				return /*(1)*/;
			}
		}
		if (my_pad_alter & PSP_KEY_SHOT_OK) 		/* �n�j���� */
		{
			shot_ositenai = 0;	/* �V���b�g�������� */
		}
	}
//
	/* ----- �X�N���v�g�R�}���h����(���s)�n ----- */
	while (1)
	{
		if (0==ssc->done)
		{
			switch (ssc->command)
			{
			// [�e�L�X�g�̃J�[�\������]
			case SCP_CUR_POP:		/* �J�[�\���ʒu�A���� */
			//	count_char=cursor;
				cursor_x = cursor_x_chached;
				cursor_y = cursor_y_chached;
				ssc->done = 1;
				break;
			case SCP_CUR_PUSH:		/* �J�[�\���ʒu�A�L�� */
			//	cursor=count_char;
				cursor_x_chached = cursor_x;
				cursor_y_chached = cursor_y;
				ssc->done = 1;
				break;
			case SCP_CUR_HOME:		/* �J�[�\���̏����� */
			//	count_char=0;
				home_cursor();				/* �J�[�\�����z�[���|�W�V�����ֈړ� */
				ssc->done = 1;
				break;
			// [�e�L�X�g�\��/�N���b�N�҂�]
			case SCP_CLICK:
				if (0 == shot_ositenai)
				{	ssc->done = 1;	}
				break;
			case SCP_BG_WRITE_TEXT:
				set_write_text(1);		/* BG�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
				ssc->done = 1;
				break;
			case SCP_TEXT_WRITE_TEXT:
				set_write_text(0);		/* ���b�Z�[�W�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
				ssc->done = 1;
				break;
			case SCP_TEXT:
				if (0 == tmp_all[(ssc->chain)])
				{
					if (0 == shot_ositenai) 	{	ssc->para2=0;	}	/* �V���b�g��������A�c��͑S������ */
					#if (1)
				//	if (SDL_MUSTLOCK(sdl_lock_surface)) 			{	SDL_LockSurface(sdl_lock_surface);			}	/* ���b�N */
					if (SDL_MUSTLOCK(msg_window)) 					{	SDL_LockSurface(msg_window);			}	/* ���b�N */
					#endif
					tmp_all[(ssc->chain)] = print_kanji000(/*sdl_lock_surface,*/ /*ssc->data,*/ ssc->para0, ssc->para1, ssc->para2);
					#if (1)
				//	if (SDL_MUSTLOCK(sdl_lock_surface)) 			{	SDL_UnlockSurface(sdl_lock_surface);		}	/* ���b�N���� */
					if (SDL_MUSTLOCK(msg_window)) 					{	SDL_UnlockSurface(msg_window);		}	/* ���b�N���� */
					#endif
				}
				else	/*if (1==tmp_all[(ssc->chain)])*/
				{
					if (0x00 == (ssc->para3 & 0xf8))	/* 0���W�X�^�Ɣ�r */
					{
						if ( 0x00 == ( (ssc->para3 & shot_ositenai) & 0x01) )	/* 0���W�X�^�Ɣ�r */
						{
							if (0x00 != (ssc->para3 & 0x02))	/* 0���W�X�^�Ɣ�r */
							{
						//	//	count_char=0;
								home_cursor();				/* �J�[�\�����z�[���|�W�V�����ֈړ� */
							}
						//	else	{	;	}	//cursor_continue = 1;	/* �J�[�\���p�� */
							if (0x00 != (ssc->para3 & 0x04))	/* 0���W�X�^�Ɣ�r */
							{
								msg_window_clear();  	/* ���b�Z�[�W�E�B���h�E�̓��e�������B */
							//	msg_window_init();/*???*/		/* �E�B���h�E������ */
							}
						//	else	{	;	}
							ssc->done = 1;
							tmp_all[(ssc->chain)]=0;
						}
					}
				}
				break;
#if 0
/*
	switch (ssc->para3)
	w: 0:���Ȃ� 	   1:�E�B���h�E������
	c: 0:�J�[�\���p��  1:�J�[�\��������
	b: 0:���s�B 	   1:�{�^��������������s�B
		   wcb
	case 0:000: ����������		 ���̖��߂ցA�J�[�\���p��
	case 1:001: �{�^������������ ���̖��߂ցA�J�[�\���p��
	case 2:010: ����������		 ���̖��߂ցA�J�[�\��������
	case 3:011: �{�^������������ ���̖��߂ցA�J�[�\��������
	case 4:100: ����������		 ���̖��߂ցA�J�[�\���p��	 �E�B���h�E������
	case 5:101: �{�^������������ ���̖��߂ցA�J�[�\���p���A  �E�B���h�E������
	case 6:110: ����������		 ���̖��߂ցA�J�[�\���������A�E�B���h�E������
	case 7:111: �{�^������������ ���̖��߂ցA�J�[�\���������A�E�B���h�E������
*/
#endif

			case SCP_BG_LOAD:
			//	bg_story_window = load_local01(ssc->para0, bg_story_window, ssc->para1);
			//	SDL_SetColorKey(bg_story_window,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
			//	ssc->done = 1;
				ssc->done=load_my_bg(ssc->para0,ssc->para1,ssc->para2,ssc->para3);
				break;
			case SCP_OBJ_LOAD:	/* �ėp�G  �����GL	�����GR */
				load_my_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3/*,ssc->data*/);
				ssc->done = 1;
				break;
			case SCP_OBJ_MOVE:
				ssc->done=do_move_sc_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3,ssc->para4);
				if (ssc->done == 2/*-1*/)
				{
					#if 0
					//ps pDebugScreenPrintf("no sprite in No.%s\n",ssc->para0);
					//sc eKernelDelayThread(2000000);
					#endif
					return_the_script=(1);
					return /*(1)*/;
				}
				break;
			case SCP_OBJ_SWAP:
				swap_my_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3);
				ssc->done = 1;
				break;
			case SCP_SCREEN_BG: 		is_bg					= ssc->para1;	ssc->done = 1;	break;
			case SCP_SCREEN_TEXT: 		draw_script_screen		= ssc->para1;	ssc->done = 1;	break;
			case SCP_SCREEN_PANEL:		draw_side_panel 		= ssc->para1;	ssc->done = 1;	break;
			case SCP_BGM:				play_music_num( (ssc->para1) ); 	ssc->done = 1;	break;
			}
		}
		if (0 == ssc->done) 	{	n9++;	}		/* �p���t���O */
		if (NULL==ssc->next)	{	break;	}		/* next��NULL�̏ꍇ */
		if ((ssc->chain >= ssc->next->chain) || (0==ssc->chain))	{ break;	}	/* ��chain����chain���傫���ꍇ */
		ssc = ssc->next;
	}
//
	/* ----- �`��n ----- */
	s_draw_the_script();
//
	inits = 0;
	if (0 == n9)					/* ���̖��߂̋��� */
	{
		inits = 1;
		while (start_script->done)		/* ���̖��߂� */
		{
			if (NULL == start_script->next)
			{
				script_reset();
				return_the_script=(1);
				return /*(1)*/;
			}
			start_script=start_script->next;
		}
	}
//
	return_the_script=(0);
	return /*(0)*/;
}


/*---------------------------------------------------------

---------------------------------------------------------*/

void script_display(void)
{
	/*�{�X�퓬��C�x���g*/	/*�{�X�퓬�O�C�x���g*/
	{
		work_the_script();
	//	if (1==return_the_script)	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
		if (0!=return_the_script)
		{
			draw_script_screen = 0; /* ����ӃE�B���h�E�\���t���O off */
			{
				PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
				pd->state_flag &= (~(STATE_FLAG_06_IS_SCRIPT));/*off*/
				pd->state_flag |= STATE_FLAG_12_END_SCRIPT; 	/*pd->bo ssmode=B08_START;*/	/*	pd->bo ssmode=B09_STAGE_LOAD;*/
			}
		}
		/*else	{	draw_the_script();}*/
	}	// [***090313	�ǉ�
}

/*---------------------------------------------------------

---------------------------------------------------------*/
//	380/*scenario_width*/=width;
//	if (NULL != bg_name)
//	{
//		load_bg_aaa(bg_name, 0/*alpha*/);
//	//	bg_story_window = load_local01(bg_name, bg_story_window, 0);
//	//	SDL_SetColorKey(bg_story_window,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
//	}

	/* �E�B���h�E�̔������g */
//	std_window			= loadbmp0("fonts/window.png", 1, 1);/*2*/
	#if 0
//	msg_window_clear(); 	/* ���b�Z�[�W�E�B���h�E�̓��e�������B */
//	msg_window_init();/*???*/
//	home_cursor();				/* �J�[�\�����z�[���|�W�V�����ֈړ� */
	#else
	#endif


static int script_start(char *filename)		/* �V�i���I�t�@�C�����Ɣw�i�t�@�C���� */	/*, char *bg_name, int width*/
{
	if (0 == load_scenario(filename))
	{
		return (0);
	}
	script_message_window_clear();
	cursor_x_chached	= 0;	/* �J�[�\�������� */
	cursor_y_chached	= 0;	/* �J�[�\�������� */

//	msgw_rect_init(20,182);
	inits				= 1;
	is_bg				= 0;
	draw_script_screen	= 1/*0*/;
//	count_char			= 0;
//	cursor				= 0;

	#if 1
	/* std_obj[]������ */
//	std_obj[SPRITE_tachie_r]			= NULL;
//	std_obj[SPRITE_tachie_l]			= NULL;
	int i;
	for (i=0; i<SPRITE_MAX/*20*/; i++)
	{
		std_obj[i]		= NULL;
	}
	#endif
	start_script		= sscript;
	return (1);
}
/*---------------------------------------------------------

---------------------------------------------------------*/
extern int select_player;
void script_load(void/*int mode*/)
{
	char file_name[16/*10*/];
	strcpy(file_name,"Z/sZ1");
	file_name[0] = ('0'+select_player);
	file_name[3] = ('0'+player_now_stage);
//
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	if (/*STATE_FLAG_05_IS_BOSS == */(pd->state_flag & STATE_FLAG_05_IS_BOSS))
	{
		;// file_name[4] = '1';
	}
	else
	{
		file_name[4] = '0';
		if (0!=boss_bgm_mode)
		{	/* �{�X�C�x���g��Ƀ{�X�� */
			set_music_volume(80);/* �ȉ��ʒቺ */
		}
		else
		{	/* �{�X�C�x���g�O�Ƀ{�X�� */
			play_music_num( (BGM_10_boss1-1)+player_now_stage );/* �{�X�Ȗ炷 */
		}
	}
//
	if (0 == script_start(file_name))		// �t�@�C�����Ȃ��ꍇ�̓C�x���g���΂�, /*NULL,*/ (GAME_WIDTH)	/*380*/
	{
		pd->state_flag |= STATE_FLAG_12_END_SCRIPT; 		/*pd->bo ssmode=B09_STAGE_LOAD;*/
	}
	else
	{
		pd->state_flag |= STATE_FLAG_06_IS_SCRIPT;	/*on*/		/*pd->bo ssmode=B00_NONE;*/ /*B06_AFTER_EVENT*/
		#if 0
		/* �V�i���I���Ƀ{�����������Ă��܂��B�o�O������̂ŁB */
		pd_bomber_time = 0;
		#endif
	}

//
//	if (0 == script_start(file_name)), /*NULL,*/ 380
//			{	pd->state_flag |= STATE_FLAG_12_END_SCRIPT; 		/*pd->bo ssmode=B08_START;*/ }
//	else	{	pd->state_flag |= STATE_FLAG_06_IS_SCRIPT;			/*pd->bo ssmode=B00_NONE;*/ /*B03_BEFORE_EVENT*/	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

enum /*_story_state_*/
{
	STORY_INIT=0,
	STORY_WORKS,
	STORY_QUIT
};

void story_work(void)
{
	switch ((Uint8)(psp_loop&0xff)/*state.substate*/)
	{
	case STORY_INIT:
//		bg_alpha=0;
		script_start("story");	//	/*,NULL*/ /*"story/arasuzi.jpg"*/ /*"story_bg.jpg"*/, 480
//		bg_alpha=0;
		psp_loop++;//newsta te(ST_STORY,STORY_FADEIN,0);
		break;
	case STORY_WORKS:
		psp_clear_screen();
		work_the_script();
		/*draw_the_script();*/
		if (return_the_script)
		{
			psp_loop++;//newsta te(ST_STORY,STORY_QUIT,0);
		}
		break;
	case STORY_QUIT:
			#if 1/*???*/
			/*	�����ǂ��킩��Ȃ����ǁA�����͂�������???�B */
			if (NULL != bg_story_window)
			{
				unloadbmp_by_surface(bg_story_window);
			}
			#endif
//		bg_alpha = 0;
		inits=1;
		draw_script_screen = 0; /* ����ӃE�B���h�E�\���t���O off */
		//
		menu_cancel_and_voice();	/* ���C�����j���[�ɖ߂� */
		break;
	}
}


/*---------------------------------------------------------
	psp�ł͊J�������퓮��o���Ȃ��̂ŁA�N�����Ɋm�ۂ���(�I�����܂�)�J�����Ȃ�
---------------------------------------------------------*/

void script_system_init(void)/* �g�ݍ��� */
{
	#if 1
//	msg_window			= NULL; 	/* ���b�Z�[�W�E�B���h�E */
//	std_window			= NULL; 	/* �E�B���h�E�̔������g */
//
	sscript 			= NULL; 	/* ���ߕێ��p */
	start_script		= NULL; 	/* ���ߎ��s�p */
//
	draw_script_screen			= 0;	/* ����ӃE�B���h�E�\���t���O */
	is_bg				= 0;	/* �w�i�\���t���O */
//
//	count_char			= 0;
//	cursor				= 0;
	home_cursor();				/* �J�[�\�����z�[���|�W�V�����ֈړ� */
	cursor_x_chached	= 0;	/* �J�[�\���ʒu �ۑ��p */
	cursor_y_chached	= 0;	/* �J�[�\���ʒu �ۑ��p */
//
	#endif
	bg_alpha			= 255;	/* 255==�����l */

	/* SDL_FreeSurface(); �� psp�ł͑��������Ɠ����Ȃ��̂� ���̑΍� */
//
		#if 1
		/*???*/
	//	font_color_bitmap=loadbmp(buffer);
	//	esc_len=0;
		{
			char fn[128/*64*/];
		//	strcpy(fn, data_dir);	strcat(fn, "/fonts/" "font_bg16.png" );
			strcpy(fn, DIRECTRY_NAME_DATA "/fonts/" "font_bg16.png" );
			font_bg_bitmap = IMG_Load(fn);/*�����Ń��[�h����Ɗm���ɏ������������*/
		}
		font_bg_bitmap = SDL_DisplayFormat(font_bg_bitmap);
		#endif

//
	/* psp�ł� SDL_FreeSurface() ������ɓ��삵�܂���̂ŁA���̂܂܂ł̓������s���H�Ńn���O�A�b�v���܂��B */
	/*msg_window_init()*/
//	if (NULL != msg_window) 	{	SDL_FreeSurface(msg_window);	msg_window = NULL;	}
	msg_window = SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_SWSURFACE,/* ���C���������֊m�ۂ��� */
			(320)/*340*/,
			(56)/*70*/ /*100*/,
			sdl_screen[SDL_00_SCREEN]->format->BitsPerPixel,
			sdl_screen[SDL_00_SCREEN]->format->Rmask,
			sdl_screen[SDL_00_SCREEN]->format->Gmask,
			sdl_screen[SDL_00_SCREEN]->format->Bmask,
			sdl_screen[SDL_00_SCREEN]->format->Amask);
	if (SDL_MUSTLOCK(msg_window))	{	SDL_LockSurface(msg_window); }
	msg_window_image	= (UINT16 *)msg_window->pixels;
	msg_window_pitch	= msg_window->pitch;
	msg_window_width	= msg_window->w;
	if (SDL_MUSTLOCK(msg_window))	{	SDL_UnlockSurface(msg_window);	}
	SDL_SetColorKey(msg_window,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
/* psp �ł� �m�ې悪 VRAM �� ���C�������� ����2��ނ�������܂���B
���̂��� VRAM �m�ۂ��ꂽ�ꍇ�Apsp��SDL�ŗL�̋@�\�ł���SDL�̉摜�L���b�V���@�\������ɓ����̂ŁA
���ꂪ�����ŕςȎ��ɂȂ�܂��B(���C���������m�ۂ��ꂽ�ꍇ�A���̖��͋N���܂���)
SDL_SWSURFACE ���w�肳��Ă���΁A�K�����C���������֊m�ۂ�����ł͂Ȃ��悤�ł��B
���󂱂̕����� VRAM �m�ۂ���Ă���݂����ł��B VRAM �m�ۂ���Ȃ��� Alpha ���g���Ȃ��̂����m��܂���H�H�H�B
 */
}

/*---------------------------------------------------------
	�I�����ɊJ�����镔��...�������ǁA
	�n���O�A�b�v�����肷��̂ŁA��芸���������ɂȂ��Ă�B
	(�J�����Ȃ��Ă��I������΁AOS���������������̂Ŗ��Ȃ����Č����Ζ��Ȃ�)
---------------------------------------------------------*/

void script_system_exit(void)/* �O�� */
{
	/*msg_window_init()*/
//	if (NULL != msg_window) 	{	SDL_FreeSurface(msg_window);	msg_window = NULL;	}
	//	if (NULL != font_color_bitmap)	{	SDL_FreeSurface(font_color_bitmap); 		font_color_bitmap	= NULL; }
//		if (NULL != font_color_bitmap)	{	unloadbmp_by_surface(font_color_bitmap);	font_color_bitmap	= NULL; }
}
