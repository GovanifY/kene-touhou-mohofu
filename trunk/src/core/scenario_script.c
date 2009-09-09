
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

#define USE_2ND_SCREEN		(0)


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

static Uint32 getpixel16(SDL_Surface *surface, int x, int y)
{
	Uint8 *p = (Uint8 *)surface->pixels+y*surface->pitch+x+x;
	return (*(Uint16 *)p);
}

static void putpixel16(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	Uint8 *p = (Uint8 *)surface->pixels+y*surface->pitch+x+x;
	*(Uint16 *)p=pixel;
}
static void draw_shinonome_moji(SDL_Surface *drawmap, SDL_Surface *backmap, int x, int y, int code_offset, int hidari_gawa)
{
//	if (1==hidari_gawa) 	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	if (0!=hidari_gawa)
	{
		x += FONT_WIDTH;
	}
	if (x > (drawmap->w-FONT_WIDTH) )	/* �͈͊O�Ȃ� */
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
				putpixel16(drawmap, dx+x, dy+y, dot);
			}
		}
	}
}

/*---------------------------------------------------------
http://openlab.ring.gr.jp/efont/shinonome/
	shift jis�����R�[�h�K�i �� ���_�����R�[�h�K�i �֕ϊ���A�`�悷��
---------------------------------------------------------*/

static void draw_sjis_kanji(SDL_Surface *drawmap, SDL_Surface *backmap, int x, int y, int high_byte, int low_byte)
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
		draw_shinonome_moji(drawmap, backmap, x, y, (idx8), 0);/*low_byte<<4*/
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
		{
			/* ���_�����R�[�h�K�i �̑��݃}�b�v */
			//#define FDxx (47)/* ���p2�o�C�g�������� */
			static	const	signed char/*int*/ sinonome_sjis_bank_offset[0x40] =
			{// +0	+1	+2	+3	 +4  +5  +6  +7   +8  +9  +a  +b   +c  +d  +e  +f
				-1,  0,  1,  2,   3, -1, -1,  4,   5,  6,  7,  8,	9, 10, 11, 12,		/* 0x80xx */
				13, 14, 15, 16,  17, 18, 19, 20,  21, 22, 23, 24,  25, 26, 27, 28,		/* 0x90xx */
				29, 30, 31, 32,  33, 34, 35, 36,  37, 38, 39, -1,  -1, -1, -1, -1,		/* 0xe0xx */
				-1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1		/* 0xf0xx */
			//	�ȉ��̗̈�͓��_�t�H���g16�ɂ̓t�H���g�f�[�^�[���Ȃ�(eaa4���ŏI�f�[�^�[������)
			//	29, 30, 31, 32,  33, 34, 35, 36,  37, 38, 39, -1,  -1, 40, 41, -1,		/* 0xe0xx */	���_�t�H���g16�ɂ̓t�H���g�f�[�^�[���Ȃ�
			//	-1, -1, -1, -1,  -1, -1, -1, -1,  42, 43, 44, 45,  46, 47, -1, -1		/* 0xf0xx */	���_�t�H���g16�ɂ̓t�H���g�f�[�^�[���Ȃ�
			};
			idx = sinonome_sjis_bank_offset[high_byte & 0x3f];
		}
		/* ���݃}�b�v�� -1 �̏ꍇ�́A ���_�����R�[�h�K�i �ɂȂ������Ȃ̂ŕ\���ł��Ȃ� */
		if (-1 == idx)
		{
			return;
		}
		//
		idx = (16*10)/*���p�������X�L�b�v*/ + (idx*2*192)/*codetbl�P������192����*/;
		#if (1==USE_HARF_KANJI)
		if (1 == size_of_kanji(high_byte))	// 2�o�C�g���p
		{
			idx += (low_byte);
		//	code_offset <<= 4;	//code_offset *= 16;	/* 9 */ 	// �����ł�9�͑���1�̕����Ɏg���o�C�g���B(1byte == 8bit)*9=72 == 6*12
			draw_shinonome_moji(drawmap, backmap, x, y, (idx) , 0);
		}
		else	/* if ( 2 == size_of_kanji(high_byte))*/	// 2�o�C�g�S�p
		#endif
		{
			idx += (low_byte+low_byte);/* x2 */
			draw_shinonome_moji(drawmap, backmap, x, y, (idx) , 0); /* �E���� */
			draw_shinonome_moji(drawmap, backmap, x, y, (idx) , 1); /* ������ */
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
			font_color_bitmap = IMG_Load(fn);/*�����Ń��[�h����Ɗm���ɏ��u���������*/
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
/*extern*/ static int print_kanji(SDL_Surface *drawmap, SDL_Rect *rect, const char *str, int color_type, int wait)
{
	font_color_number = (color_type & 0x0f);

	static int count_width=1/* 0 (����Z�Ȃ̂�0�͊댯) */;/*???*/
//	static int esc_len;
	if (0 == my_string_offset)	//������
	{
		count_width=(int)((rect->w)/FONT_WIDTH)+2;
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
			if (SDL_MUSTLOCK(drawmap))				{	SDL_LockSurface(drawmap);			}	/* ���b�N */
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
			xx = rect->x+(cursor_x)*(FONT_WIDTH);
			yy = rect->y+(cursor_y)*(FONT_HEIGHT+2);
		//	�p�t�H�[�}���X���ቺ����̂łȂ�ׂ����b�N���Ȃ��B(���b�N�͍ŏ�����)
		/* �����b�N */
			if (is_kanji_1st(high_byte)) /* shift jis ���� */
			{
			/*unsigned char*/int low_byte;	low_byte = (*(str+my_string_offset+1) & 0xff);
			//	draw_sjis_kanji(drawmap, font_color_bitmap, xx, yy, high_byte, low_byte);
				draw_sjis_kanji(drawmap, font_bg_bitmap, xx, yy, high_byte, low_byte);
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
				//	draw_sjis_kanji(drawmap, font_color_bitmap, xx, yy, 0, high_byte);
					draw_sjis_kanji(drawmap, font_bg_bitmap, xx, yy, 0, high_byte);
					my_string_offset++;
					cursor_x++;
				}
			}/* �����b�N���� */
			/* ���s���� */
			if ( (count_width-1) < cursor_x)
			{
				cursor_x=0;
				cursor_y++;
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
			if (SDL_MUSTLOCK(drawmap))				{	SDL_UnlockSurface(drawmap); 			}	/* ���b�N���� */
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
//		CLCURSOR	/* �J�[�\���̏����� */
//		LOADCUR 	/* �L�������J�[�\���ʒu�̌Ăяo�� */
//		SAVECUR 	/* �J�[�\���ʒu�̋L�� */
// [�e�L�X�g�\��/�N���b�N�҂�]
//		CLICK		/* �N���b�N�҂� */
//		BGTEXT		/* �w�i�Ƀe�L�X�g��ł����� */
//		TEXT		/* ���b�Z�[�W�E�B���h�E�ւ̏������� */


//		FILTER
//		JUMP		/* pause�{�^��������������jump��B */
//		LOADBG		/* �w�i�t�@�C���̕ύX */
//		LOADL		/* �����G�� �X�v���C�g�̃��[�h */
//		LOADR		/* �����G�E �X�v���C�g�̃��[�h */
//		LOADSP		/* �ėp�X�v���C�g�̃��[�h */
//		MOVESP		/* �X�v���C�g�̈ړ� */
//		PARAMSP
//		RELOADSP	/* �X�v���C�g�̍ēǂݍ��� */
//		SUBG		/* �w�i�̕\��/��\�� */
//		SUFILTER	/* �t�B���^�[�\��/��\��&���ߓx */

//		SUL 		/* �����G���X�v���C�g�̕\��/��\�� */
//		SUR 		/* �����G�E�X�v���C�g�̕\��/��\�� */
//		SUSPRITE	/* �ėp�X�v���C�g�̕\��*/

//		SUTWINDOW	/* ���������G�E�B���h�E�̕\��/��\�� para2�̓X�R�A�p�l����on/off */
//		SUWINDOW	/* ���b�Z�[�W�E�B���h�E�̕\��/��\�� para2��bpp���x�B*/

//		TWINDOW 	/* �����G�����E�B���h�E�̏����� */
//		WAIT		/* wait */

enum
{
// [�e�L�X�g�̃J�[�\������]
	SCP_LOADCUR,
	SCP_SAVECUR,
	SCP_CLCURSOR,
// [�e�L�X�g�\��/�N���b�N�҂�]
	SCP_CLICK,
	SCP_BGTEXT,
	SCP_TEXT,
//

	SCP_FILTER,
	SCP_JUMP,
	SCP_LOADBG,
	SCP_LOAD_LR,
	SCP_LOADSP,
	SCP_MOVESP,
	SCP_PARAMSP,
	SCP_RELOADSP,
	SCP_SUBG,
	SCP_SUFILTER,
	SCP_SUL,
	SCP_SUR,
	SCP_SUSPRITE,
	SCP_SUTWINDOW,
	SCP_SUWINDOW,
	SCP_TWINDOW,
	SCP_WAIT
};

typedef struct _scenario_script
{
	int command;
	int done;		// �I��������ǂ���
	int chain;		// �������s���ǂ���
					// 0=�Ⴄ�� 1=1�� 2=2��
	void *data;
//
	int para1;
	int para2;
	int para3;
	struct _scenario_script *next;
//
	char para0[(7*32)/*200*/];/* 32�̔{�� */
} S_SCRIPT;/* == 256bytes */


typedef struct /*_sc_sprite*/
{
	SDL_Surface *img;				/* Images */
	Uint8 frames;					/* �A�j���[�V����������\�� */
	Uint8 anim_speed;				/* ��ɓ��� */
	Uint8 anim_type;				/* �ǂ��������ɃA�j���[�V���������邩 */
	Uint8 anim_time;				/* �A�j���[�V������ */
	Uint8 alpha;					/* alpha�l */
	Uint8 flags;					/* 0:��\��, 1:�\��, 2:tachie_window(2nd screen)�ɕ\��	�����ł͕\�������邩�ǂ����������� */
	int aktframe;					/* ���݂̃t���[�� */
	int anim_count; 	/* �A�j���[�V�����̃^�C�~���O */
	int/*int*/ x;		/* x���W���� */
	int/*int*/ y;		/* y���W���� */
	int w;							/* �� */			//	int cw; 	/* ���S���W(��)���� */
	int h;							/* ���� */			//	int ch; 	/* ���S���W(����)���� */
	int move_wait;					/* �������������Ƃ��̐���p */
	int target_x;					/* �ڕW���W */
	int target_y;					/* �ڕW���W */
	int x256;						/* ���x�m�ۗp(256�Œ菬���_�`��) */
	int y256;						/* ���x�m�ۗp(256�Œ菬���_�`��) */
	int angle512;		/* ���m�ȕ��� */	//	int r_course;	/* ��̂̕��� */
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
#if (1==USE_2ND_SCREEN)
static SDL_Surface *tachie_window/*=NULL*/; 	/* �D�旧���G�E�B���h�E */
#endif /* (1==USE_2ND_SCREEN) */
static SDL_Surface *bg_story_window;			/* �X�N���v�g�ɂ�����w�i */

static SDL_Surface *filter_window;				/* ���o�p�E�B���h�E */
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

#if (1==USE_2ND_SCREEN)
static int is_tachie_window/*=0*/;				/* �����G�����\���t���O */
#endif /* (1==USE_2ND_SCREEN) */
static int is_bg/*=0*/; 						/* �w�i�\��/��\���t���O */
static int is_filter/*=0*/; 					/* �t�B���^�[�\���t���O */
//static int 380/*scenario_width*/; 			/* ���� */

/*---------------------------------------------------------
	�q�֐�
---------------------------------------------------------*/
extern int tiny_strcmp(char *aaa, const char *bbb);
static int cha_search(char *str)
{
	int n;
	if (0==tiny_strcmp(str, "right"))		{	return (-2);	}
	else if (0==tiny_strcmp(str, "left"))	{	return (-3);	}
	else
	{
		char buffer[32/*20*/];
		char *d = buffer;
		while (isdigit(*str))		{	*d++ = *str++;	}
		n=atoi(buffer);
		if (0<=n && n<15)			{	return (n); }
	}
	return (-1);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static SDL_Surface *load_Surface(char *filename, int alpha)
{
	char fn[128];
//	strcpy(fn, data_dir);	strcat(fn, "/scenario/");
	strcpy(fn, DIRECTRY_NAME_DATA "/"/*"scenario/"*/);
	strcat(fn, filename);
//
	SDL_Surface *s1;/*temporaly*/
	SDL_Surface *s2;/*result*/
//
	s1 = IMG_Load(fn);/*�����Ń��[�h����Ɗm���ɏ��u���������*/
	if ( NULL == s1 )
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant load image %s:"/*" %s"*/,fn/*,SDL_GetError()*/);
		return (NULL);
	}
	if ((-1==alpha) || (0==alpha))
	{
		s2 = SDL_DisplayFormat(s1);
		SDL_SetColorKey(s2,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	}
	else
	{
		s2 = SDL_DisplayFormatAlpha(s1);
	}
	if ( NULL == s2 )
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant convert image %s to display format:"/*" %s"*/,fn/*,SDL_GetError()*/);
		return (NULL);
	}
	SDL_FreeSurface(s1);
	s1 = NULL;
	return (s2);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static SDL_Surface *load_local(char *str, SDL_Surface *s, int alpha)
{
	char fn[128];
//	strcpy(fn, data_dir);	strcat(fn, "/scenario/");
	strcpy(fn, DIRECTRY_NAME_DATA "/"/*"scenario/"*/);
	strcat(fn, str);
	SDL_Surface *tmp = NULL;

	if (NULL != s)
	{
		SDL_FreeSurface(s);
		s = NULL;
	}

	tmp = IMG_Load(fn);/*�����Ń��[�h����Ɗm���ɏ��u���������*/
	if ( NULL == tmp )
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant load image %s:"/*" %s"*/,fn/*,SDL_GetError()*/);
	}
	if (	(-1==alpha) /* �X�N���v�g���ŏȗ������ꍇ(�f�t�H���g) */
		|| (0==alpha)	/* ���������ꍇ */
	)
	{
		/* �A���t�@�g��Ȃ� */
		s = SDL_DisplayFormat(tmp);
		SDL_SetColorKey(s,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000); 	/* �J���[�L�[(�����F�A�����F)�͍� */
	}
	else
	{
		/* �A���t�@�g�� */
		s = SDL_DisplayFormatAlpha(tmp);
	}
	if ( NULL == s )
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant convert image %s to display format:"/*" %s"*/,fn/*,SDL_GetError()*/);
	}
	SDL_FreeSurface(tmp);
	tmp = NULL;
	return (s);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void remove_sc_sprite(SC_SPRITE *s)
{
	if (NULL != s)
	{
		SDL_FreeSurface(s->img);
		free (s);
		s = NULL;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void reload_sc_sprite(char *filename, int no, int frames, int alpha)
{
	SC_SPRITE *sc;
	if (no==-1) 		{	return; 		}
//	else if (no==-2)	{	sc=std_obj[SPRITE_tachie_r/*32-2*/];	}	/* right */
//	else if (no==-3)	{	sc=std_obj[SPRITE_tachie_l/*32-3*/];	}	/* left */
	else				{	sc=std_obj[(no)&(SPRITE_MAX-1)];	}	/* �ėp�X�v���C�g */
	if (NULL==sc)		{	return; 		}

	SDL_FreeSurface(sc->img);

	SDL_Surface *tmp = NULL;
	tmp 		= load_local(filename,tmp,alpha);
	sc->img 	= tmp;
	sc->frames	= frames;
	sc->w		= (tmp->w/frames);	//sc->cw		= ((sc->w)>>1);
	sc->h		= (tmp->h); 		//sc->ch		= ((sc->h)>>1);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static SC_SPRITE *load_sc_sprite(char *filename, int sc_sp_num, int frames, int alpha, SDL_Rect *rect)
{
	if (NULL != std_obj[sc_sp_num]) 	{	remove_sc_sprite(std_obj[sc_sp_num]); }
	SC_SPRITE *tmp;
	tmp 				= mmalloc(sizeof(SC_SPRITE));
	SDL_Surface *suf	= NULL;
	suf 				= load_Surface(filename,alpha);
	if (NULL==suf)		{ return (NULL);			}
	tmp->img			= suf;
	if (frames==-1) 	{	tmp->frames = 1;		}
	else				{	tmp->frames = frames;	}
	tmp->aktframe		= 0;
	tmp->anim_speed 	= 0;
	tmp->anim_type		= 0;
	tmp->anim_time		= 0;
	tmp->anim_count 	= 0;
	tmp->alpha			= 255;
	tmp->w				= (suf->w/frames);	//tmp->cw			= ((tmp->w)>>1);
	tmp->h				= (suf->h); 		//tmp->ch			= ((tmp->h)>>1);
	tmp->flags			= 0;
	tmp->move_wait		= 0;
	tmp->x				= rect->x;
	tmp->y				= rect->y;
	std_obj[sc_sp_num]	= tmp;
	return (tmp);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void load_local_sprite(char *filename, int sc_sp_num, int frames, int alpha, SDL_Rect *rect)
{
	if (NULL != std_obj[sc_sp_num]) 	{	remove_sc_sprite(std_obj[sc_sp_num]); }
	SC_SPRITE *tmp;
	tmp 				= mmalloc(sizeof(SC_SPRITE));
	SDL_Surface *suf	= NULL;
	suf 				= load_local(filename,suf,alpha);
	if (NULL==suf)		{	return; }
	std_obj[sc_sp_num]	= tmp;
	tmp->img			= suf;
	if (frames==-1) 	{	tmp->frames=1;	}
	else				{	tmp->frames=frames; }
	tmp->aktframe		= 0;
	tmp->anim_speed 	= 0;
	tmp->anim_type		= 0;
	tmp->anim_time		= 0;
	tmp->anim_count 	= 0;
	tmp->alpha			= 255;
	tmp->w				= (suf->w/frames);	//tmp->cw			= ((tmp->w)>>1);
	tmp->h				= (suf->h); 		//tmp->ch			= ((tmp->h)>>1);
	tmp->flags			= 0;
	tmp->move_wait		= 0;
	tmp->x				= rect->x-(rect->w*tmp->w);
	tmp->y				= rect->y-tmp->h;
}
//	tmp->cw 			= suf->w/frames/2;	//tachie_r
//	tmp->cw 			= suf->w/frames;	//tachie_l


/*---------------------------------------------------------

---------------------------------------------------------*/

static int inits;								/* �e�X�N���v�g�R�}���h���Ƃ̏��������� */

/*---------------------------------------------------------
	SCP_MOVESP	�X�v���C�g�̈ړ�
---------------------------------------------------------*/

static int do_move_sc_sprite(char *l_or_r, int x, int y, int speed_aaa)
{
	SC_SPRITE *sc;
	int n=cha_search(l_or_r);
	if (n==-1)			{	return (-1);	}
//	else if (n==-2) 	{	sc = tachie_r;	}	/* right */
//	else if (n==-3) 	{	sc = tachie_l;	}	/* left */
	else				{	sc = std_obj[((n)&(SPRITE_MAX-1))]; }	/* �ėp�X�v���C�g */
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
	}
//
	/* 10�Ŋ���̂͒x�߂���̂ŁA3�r�b�g�V�t�g����(1/8)�̒l�ɂ��� */
	int speed256;
	speed256 = (speed_aaa<<(8-3));		/* ���x�m�ۗp */
//	sc->x += ((cos512(sc->angle512)*speed)/10);
//	sc->y += ((sin512(sc->angle512)*speed)/10);
	sc->x256 += ((cos512(sc->angle512)*speed256)>>8);	/* ���x�m�ۗp */
	sc->y256 += ((sin512(sc->angle512)*speed256)>>8);	/* ���x�m�ۗp */
//
	sc->x	= (t256_floor(sc->x256));		/* ���x�m�ۗp */
	sc->y	= (t256_floor(sc->y256));		/* ���x�m�ۗp */
	/* ��̂̕���: �Ȃ̂� �S�T�x��]���Ă��炵�āA��ʂQ�r�b�g����܂��ȕ��� */
	switch ((((sc->angle512+64/*deg_360_to_512(45)*/) & 0x180 )))/*sc->r_course*/
	{	/* �ړ��������W�ɓ��������͂ݏo������A�����Ƃ���B */
	case (0<<7)/* 1:��(512_0) */:	if (sc->x >= sc->target_x)	{	goto move_complete; 	}	break;
	case (1<<7)/* 2:��(512_1) */:	if (sc->y <= sc->target_y)	{	goto move_complete; 	}	break;
	case (2<<7)/* 3:��(512_2) */:	if (sc->x <= sc->target_x)	{	goto move_complete; 	}	break;
	case (3<<7)/* 0:��(512_3) */:	if (sc->y >= sc->target_y)	{	goto move_complete; 	}	break;
	}
	return (0);/*�ړ���*/
move_complete:
	/* �ړ����������ꍇ�́A���m�ȍ��W�ɏC������B */
	sc->x = sc->target_x;
	sc->y = sc->target_y;
	return (1);/*�ړ�����*/
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static int script_wait(int n)
{
	static int w_tick;
	if (inits)
	{
		w_tick = n;
	}
	w_tick -= 1/*fps_fa ctor*/;
	if (w_tick < 0/*1?*/)
	{
		w_tick = 0;
		return (1);
	}
	return (0);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void msg_window_clear(void)
{
	SDL_FillRect(msg_window,NULL,SDL_MapRGB(msg_window->format,0,0,0));
}
static void msg_window_init(void)
{

}

/*---------------------------------------------------------

---------------------------------------------------------*/
#if (1==USE_2ND_SCREEN)
static void tachie_window_init(void)
{
}
#endif /* (1==USE_2ND_SCREEN) */
/*---------------------------------------------------------

---------------------------------------------------------*/

static void filter_init(int r,int g,int b)
{
	/*filter_init()*/
	SDL_FillRect(filter_window,NULL,SDL_MapRGB(filter_window->format,r,g,b));
	SDL_SetAlpha(filter_window, SDL_SRCALPHA, 0);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static SDL_Rect msgw_rect;				/* ���b�Z�[�W�E�B���h�E�\���ʒu */
static void msgw_rect_init(int x,int y)
{
	msgw_rect.x = x;
	msgw_rect.y = y;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/ int draw_script_screen/*=0*/;					/* ����ӃE�B���h�E�\���t���O */

static int window_effect(int w_is, int efe)
{
	/* ���̂����ʂ��Ȃ������s�� */
	if (-1 == efe)
	{
		draw_script_screen = w_is;
		return (1);
	}
//
	static int lines;
	static int st_line;
//	else
//	{
	//
	if (inits)
	{
		lines = 0;
		draw_script_screen = 2;
		msg_window_init();
		if (efe<1)
		{	st_line = 1;}
		else
		{	st_line = efe;}
	}
	Sint32 start_y;
	Sint32 end_y;
	if (w_is)
	{
		start_y 	= lines;
		end_y		= lines+st_line;
	}
	else
	{
		msg_window_init();/*???*/
		start_y 	= (0);
		end_y		= (56)/*std_window->h*/-(lines);
	}
	#if (000)
	if (SDL_MUSTLOCK(msg_window))	{	SDL_LockSurface(msg_window);	}
	if (SDL_MUSTLOCK(std_window))	{	SDL_LockSurface(std_window);	}
	{
		Sint32 y;
		for (y = start_y; y<end_y; y++)
		{
			Sint32 x;
			for (x = 0; x<(320)/*std_window->w*/; x++)
			{
				Uint32 col = getpixel16(std_window, x, y);
				putpixel16(msg_window, x, y, col);
			}
		}
	}
	if (SDL_MUSTLOCK(msg_window))	{	SDL_UnlockSurface(msg_window);	}
	if (SDL_MUSTLOCK(std_window))	{	SDL_UnlockSurface(std_window);	}
	#endif
	if (lines>(56)/*std_window->h*/)		{	msg_window_init();	draw_script_screen=w_is; return (1); }
	lines += st_line;
//	}
	return (0);
}

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
		if (NULL != l->data)	{ free (l->data);	l->data = NULL; }
		free (l);
		l = n;
	}
	sscript = (S_SCRIPT *)NULL;
	entry_script = (S_SCRIPT *)NULL;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static char *load_command(char *c, char *buffer, int *end)
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
		*end = 1;
	}
	*buffer = 0;		//NULL
	return (c);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static char *load_str(char *c, char *buffer, int *end)
{
	int i = 0;
	int e = 0;		/* �G�X�P�[�v */
	while ((((*c == ',') && (e==1))||(*c != ',')) && (*c != 13))
	{
		if ((*c == '\\') && (!e) && (*(c+1) == ','))	{	e = 1;	c++;	}
		if (e)			{	e=0;	}
		i++;
		if (i >= 200)	{	return ((char *)NULL);}
		*buffer++ = *c++;
	}
	if (*c == 13)
	{
		*end = 1;
	}
	*buffer = 0;		//NULL
	return (c);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static char *load_int(char *c, int *nr, int *end)
{
	char buffer[32/*20*/];
	char *d = buffer;
	int i = 0;
	while (isdigit(*c)||*c=='-') /* ',' �܂��� '\n' ��������I�� */
	{
		i++;
		if (i >= 20)
		{	return ((char *)NULL);}
		*d++ = *c++;
	}
	if (*c == 13)	{	/* ���s�R�[�h�� OD OA */
		*end = 1;
	}
	*d = 0;
	if ((*c==',' || *c==13) && *(c-1)==',') {	*nr=-1; }
	else									{	*nr = atoi(buffer); }
	return (c);
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
//
	new_script->done	= 0;
	new_script->chain	= chain;
	new_script->next	= NULL;
	new_script->data	= NULL;

	switch (*command)
	{
	case 'B':
		if (0==tiny_strcmp(command, "BGTEXT"))	/* �w�i�Ƀe�L�X�g��ł����� */
		{
			SDL_Rect *new_rect0;
			new_script->command 	= SCP_BGTEXT;
			new_rect0				= mmalloc(sizeof(SDL_Rect));
			new_rect0->x			= new_script->para2;/* <- �f�t�H���g */ 	//	/*c_p2*/c_pn[PARAM_02];
			new_rect0->y			= new_script->para3;/* <- �f�t�H���g */ 	//	/*c_p3*/c_pn[PARAM_03];
			new_rect0->w			= /*c_p4*/c_pn[PARAM_04];
			new_rect0->h			= 1500;
			new_script->data		= new_rect0;
			new_script->para2		= /*c_p5*/c_pn[PARAM_05];		/* �������ݑ��x */
		}
		break;
	case 'C':
		if (0==tiny_strcmp(command, "CLCURSOR"))		{	new_script->command=SCP_CLCURSOR;	}	/* �J�[�\���̏����� */
		else if (0==tiny_strcmp(command, "CLICK"))		{	new_script->command=SCP_CLICK;		}	/* �N���b�N�҂� */
		break;
	case 'F':
		if (0==tiny_strcmp(command, "FILTER"))
		{
			new_script->command 	= SCP_FILTER;
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* �f�t�H���g */
//			new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* �f�t�H���g */
		}
		break;
	case 'J':
		if (0==tiny_strcmp(command, "JUMP"))			{	new_script->command=SCP_JUMP;		}	/* pause�{�^��������������jump��B */
	case 'L':
		if (0==tiny_strcmp(command, "LOADBG"))	/* �w�i�t�@�C���̕ύX */
		{
			new_script->command=SCP_LOADBG;
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* �f�t�H���g */	/* �t�F�C�h�C���^�A�E�g */
//			new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* �f�t�H���g */
		}
		else if (0==tiny_strcmp(command, "LOADCUR"))	{	new_script->command=SCP_LOADCUR;	}	/* �L�������J�[�\���ʒu�̌Ăяo�� */
		else if (0==tiny_strcmp(command, "LOADL"))	/*	�����GL �X�v���C�g�̃��[�h */
		{
			SDL_Rect *new_rect3;
			new_script->command=SCP_LOAD_LR;
			new_rect3 = mmalloc(sizeof(SDL_Rect));
			if (   (-1==/*c_p1*/c_pn[PARAM_01])
				&& (-1==/*c_p2*/c_pn[PARAM_02]) )	/* ���w��̏ꍇ */
			{
				new_rect3->x = 0;		/* x */
				new_rect3->y = 272; 	/* y */
			}
			else
			{
				new_rect3->x = /*c_p1*/c_pn[PARAM_01];		/* x */
				new_rect3->y = /*c_p2*/c_pn[PARAM_02];		/* y */
			}
			new_rect3->w		= 1;				/* w */
			new_rect3->h		= 0;				/* h */
			new_script->data	= new_rect3;
			new_script->para1	= SPRITE_tachie_l;		/* SPRITE_tachie_l */
			new_script->para2	= /*c_p3*/c_pn[PARAM_03];		/* flame */
			new_script->para3	= /*c_p4*/c_pn[PARAM_04];		/* alpha */
		}
		else if (0==tiny_strcmp(command, "LOADR"))	/*	�����GR �X�v���C�g�̃��[�h */
		{
			SDL_Rect *new_rect4;
			new_script->command=SCP_LOAD_LR;
			new_rect4 = mmalloc(sizeof(SDL_Rect));
			if ( (-1==/*c_p1*/c_pn[PARAM_01]) && (-1==/*c_p2*/c_pn[PARAM_02]) ) 	/* ���w��̏ꍇ */
			{
				new_rect4->x = 380; 	/* x */ 	/*380 scenario_width*/
				new_rect4->y = 272; 	/* y */
			}
			else
			{
				new_rect4->x	= /*c_p1*/c_pn[PARAM_01];	/* x */
				new_rect4->y	= /*c_p2*/c_pn[PARAM_02];	/* y */
			}
			new_rect4->w		= 0;			/* w */
			new_rect4->h		= 0;			/* h */
			new_script->data	= new_rect4;
			new_script->para1	= SPRITE_tachie_r;	/* SPRITE_tachie_r */
			new_script->para2	= /*c_p3*/c_pn[PARAM_03];	/* flame */
			new_script->para3	= /*c_p4*/c_pn[PARAM_04];	/* alpha */
		}
		else if (0==tiny_strcmp(command, "LOADSP")) /* �ėp�X�v���C�g�̃��[�h */
		{
			SDL_Rect *new_rect1;
			new_script->command=SCP_LOADSP;
			new_rect1 = mmalloc(sizeof(SDL_Rect));
			new_rect1->x		= new_script->para2;/* <- �f�t�H���g */ 	///*c_p2*/c_pn[PARAM_02];		/* x */
			new_rect1->y		= new_script->para3;/* <- �f�t�H���g */ 	///*c_p3*/c_pn[PARAM_03];		/* y */
			new_rect1->w		= 0;			/* w */
			new_rect1->h		= 0;			/* h */
			new_script->data	=new_rect1;
			new_script->para2	= /*c_p4*/c_pn[PARAM_04];	/* flame */
			new_script->para3	= /*c_p5*/c_pn[PARAM_05];	/* alpha */
		}
		break;
	case 'M':
		if (0==tiny_strcmp(command, "MOVESP"))		/* �X�v���C�g�̈ړ� */
		{
			new_script->command 	= SCP_MOVESP; /* para1=x */
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* �f�t�H���g */		/* y */
//			new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* �f�t�H���g */		/* speed */
			/* �ُ�l�̏ꍇ�A�C������ */
			if (new_script->para3 < 1)
			{	new_script->para3 = 10;}
		}
		break;
	case 'P':
		if (0==tiny_strcmp(command, "PARAMSP"))
		{
			new_script->command 	= SCP_PARAMSP;					/* para1=���l */
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* �f�t�H���g */		/* anim_speed */
//			new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* �f�t�H���g */		/* anim_type */
		}
		break;
	case 'R':
		if (0==tiny_strcmp(command, "RELOADSP"))		/* �X�v���C�g�̍ēǂݍ��� */
		{
			new_script->command 	= SCP_RELOADSP; 	/* para1=no */
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* �f�t�H���g */		/* flames */
//			new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* �f�t�H���g */		/* alpha */
		}
		break;
	case 'S':
			 if (0==tiny_strcmp(command, "SAVECUR"))	{	new_script->command = SCP_SAVECUR;	}	/* �J�[�\���ʒu�̋L�� */
		else if (0==tiny_strcmp(command, "SUBG"))		{	new_script->command = SCP_SUBG; 	}	/* �w�i�̕\��/��\�� */
		else if (0==tiny_strcmp(command, "SUFILTER"))	{	new_script->command = SCP_SUFILTER; }	/* �t�B���^�[�\��/��\��&���ߓx */
		else if (0==tiny_strcmp(command, "SUL"))		{	new_script->command = SCP_SUL;		}	/* �����G���̕\��/��\�� */
		else if (0==tiny_strcmp(command, "SUR"))		{	new_script->command = SCP_SUR;		}	/* �����G�E�̕\��/��\�� */
		else if (0==tiny_strcmp(command, "SUSPRITE"))
		{
			new_script->command 	= SCP_SUSPRITE;
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* �f�t�H���g */
		}/*�X�v���C�g�̕\��*/
		else if (0==tiny_strcmp(command, "SUTWINDOW"))
		{
			new_script->command 	= SCP_SUTWINDOW;
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* �f�t�H���g */
		}	/* ���������G�E�B���h�E�̕\��/��\�� para2�̓X�R�A�p�l����on/off */
		else if (0==tiny_strcmp(command, "SUWINDOW"))
		{
			new_script->command 	= SCP_SUWINDOW;
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* �f�t�H���g */
		}	/* ���b�Z�[�W�E�B���h�E�̕\��/��\�� para2��bpp���x�B*/
		break;
	case 'T':
		if (0==tiny_strcmp(command, "TEXT"))				/* ���b�Z�[�W�E�B���h�E�ւ̏������� */
		{
			SDL_Rect *new_rect2;
			new_script->command=SCP_TEXT;
			new_rect2 = mmalloc(sizeof(SDL_Rect));
			new_rect2->x			= 0/*10*/;
			new_rect2->y			= 0/*10*/;
			new_rect2->w			= 310;
			new_rect2->h			= 64/*1500*/;
			new_script->data		= new_rect2;
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* �f�t�H���g */		/* �������ݑ��x */
//			new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* �f�t�H���g */		/* �������݌��count_char�̏��� */
		}
		else if (0==tiny_strcmp(command, "TWINDOW"))	{	new_script->command=SCP_TWINDOW;		}/* �����G�����E�B���h�E�̏����� */
		break;
	case 'W':
		if (0==tiny_strcmp(command, "WAIT"))			{	new_script->command=SCP_WAIT;			}	/* wait */
		break;

	default:
		#if 0
		//ps pDebugScreenPrintf("unknown command :%s",command);
		//sc eKernelDelayThread(3000000);
		#endif
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

	int entrys		= 0;		/* ���߂����������܂�Ă��邩�ǂ����B */
	int line_num	= 0;		/* �s�ԍ� */
	int chains		= 0;
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
		char *c = NULL; 				/* �������̍s�̕��͗p */
		int end_arg=0;				/* �����̎擾�̒��~ */
		c = buffer_text_1_line; 	/* �擪�A�h���X��n���� */

		line_num++;

		/* skiped lines. */
		if (*c=='\n')		{	continue;	}
		while (isspace(*c)) {	c++;		}
		if (*c=='#')		{	continue;	}
		if (*c=='-')		{	chains++;	c++;	}		/* �A���������� */
		else				{	chains=0;	}

		if (NULL==(c = load_command(c, char_command, &end_arg)))	{	error(ERR_WARN, "syntax error in scriptfile '%s', line no: %d", filename, line_num); continue;	}
		if (!end_arg)
		{
			if (*c++ != ' ')									{	error(ERR_WARN, "syntax error in scriptfile '%s', line no: %d", filename, line_num); continue;	}
			if (NULL==(c = load_str(c, c_p0, &end_arg)))		{	error(ERR_WARN, "syntax error in scriptfile '%s', line no: %d", filename, line_num); continue;	}
		}
		{
			int kk;
			for (kk=0; kk<PARAM_MAX/*6*/; kk++)
			{
				if (!end_arg)
				{
					if (*c++ != ',')									{	error(ERR_WARN, "syntax error in scriptfile '%s', line no: %d", filename, line_num); continue;	}
					if (NULL==(c = load_int(c, &c_pn[kk], &end_arg)))	{	error(ERR_WARN, "syntax error in scriptfile '%s', line no: %d", filename, line_num); continue;	}
				}
			}
		}
	//	regist_script(char_command, c_p0, c_pn[0],c_pn[1],c_pn[2],c_pn[3],c_pn[4],c_pn[5],c_pn[6], chains);
		regist_script(char_command, c_p0, c_pn, chains);
		entrys++;
	}
	my_fclose (/*fp*/);
	//return (entrys);
	if (0 == entrys)
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
	for (i=0;i<SPRITE_MAX/*20*/;i++)
	{
		remove_sc_sprite(std_obj[i]);
	}
//	remove_sc_sprite(std_obj[SPRITE_tachie_r]);
//	remove_sc_sprite(std_obj[SPRITE_tachie_l]);
	if (NULL != bg_story_window)	{	SDL_FreeSurface(bg_story_window);	bg_story_window 	= NULL; }
//	if (NULL != msg_window) 		{	SDL_FreeSurface(msg_window);		msg_window			= NULL; }
#if (1==USE_2ND_SCREEN)
//	if (NULL != tachie_window)		{	SDL_FreeSurface(tachie_window); 	tachie_window		= NULL; }
#endif /* (1==USE_2ND_SCREEN) */
//	if (NULL != filter_window)		{	SDL_FreeSurface(filter_window); 	filter_window		= NULL; }
//	if (NULL != font_color_bitmap)	{	SDL_FreeSurface(font_color_bitmap); font_color_bitmap	= NULL; }
	load_script_free();

//	count_char=0;			/* �J�[�\���ʒu */
//	cursor=0;
	cursor_x=0;
	cursor_y=0;
	cursor_x_chached=0; 	/* �J�[�\���ʒu �ۑ��p */
	cursor_y_chached=0; 	/* �J�[�\���ʒu �ۑ��p */

	clear_my_string_offset();	//	my_string_offset=0;
}

/*---------------------------------------------------------
	�X�v���C�g�I�u�W�F�N�g�̓���(����/�A�j���[�V����)
---------------------------------------------------------*/

static /*SC_SPRITE **/void animate_work_my_sprite(SC_SPRITE *sp)
{
//	if (NULL == sp) 	{	return (NULL);	}/* �d���`�F�b�N�Ȃ̂ŕs�v */
	if (sp->frames == 1)	{ return /*(sp)*/;	}/*  */
	/*
	/// anim_type�ɂ��� ///
	0:	������E�ɁB�Ō�܂ł������獶�֖߂�B(���[�v)
	1:	�E���獶�ɁB�Ō�܂ł�������E�֖߂�B(���[�v)
	2:	������E�ɁB�Ō�܂ł������炻�̂܂܁B(�J��Ԃ�����)
	3:	�E���獶�ɁB�Ō�܂ł������炻�̂܂܁B(�J��Ԃ�����)
	4:	������E�ɁB�Ō�܂ł�������E���獶�ɁB(���[�v)
	5:	�E���獶�ɁB�Ō�܂ł������獶����E�ɁB(���[�v)
	6:	�A�j���[�V�������Ȃ��B(��~)
	*/
	sp->anim_count += 1/*fps_fa ctor*/;
	if (sp->anim_speed < sp->anim_count)
	{
		sp->anim_count=0;
		switch (sp->anim_type)
		{
		case 0: 	sp->aktframe++; 	if (sp->aktframe>=sp->frames)	{	sp->aktframe=0; 								}	break;
		case 1: 	sp->aktframe--; 	if (sp->aktframe<0) 			{	sp->aktframe=sp->frames-1;						}	break;
		case 2: 	sp->aktframe++; 	if (sp->aktframe>=sp->frames)	{	sp->aktframe=sp->frames-1;	sp->anim_type=6;	}	break;
		case 3: 	sp->aktframe--; 	if (sp->aktframe<0) 			{	sp->aktframe=0; 			sp->anim_type=6;	}	break;
		case 4: 	sp->aktframe++; 	if (sp->aktframe>=sp->frames)	{	sp->aktframe-=2;			sp->anim_type=5;	}	break;
		case 5: 	sp->aktframe--; 	if (sp->aktframe<0) 			{	sp->aktframe+=2;			sp->anim_type=4;	}	break;
		case 6: 	break;
		}
	}
//	return /*(sp)*/;
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
			if (std_obj[nnn]->flags)
			{
				SDL_Rect src_r;
				SDL_Rect dst_r;
				src_r.x = std_obj[nnn]->w*std_obj[nnn]->aktframe;
				src_r.y = 0;
				src_r.w = std_obj[nnn]->w;
				src_r.h = std_obj[nnn]->h;
				dst_r.x = std_obj[nnn]->x;
				dst_r.y = std_obj[nnn]->y;
				#if (1==USE_2ND_SCREEN)
				if (std_obj[nnn]->flags==1)
				{	SDL_BlitSurface(std_obj[nnn]->img, &src_r, sdl_screen[SDL_00_SCREEN], &dst_r); }
				else if (std_obj[nnn]->flags==2)
				{	SDL_BlitSurface(std_obj[nnn]->img, &src_r, tachie_window, &dst_r);	}
				#else
				{	SDL_BlitSurface(std_obj[nnn]->img, &src_r, sdl_screen[SDL_00_SCREEN], &dst_r); }
				#endif /* (1==USE_2ND_SCREEN) */
			}
			/*std_obj[nnn] =*/ animate_work_my_sprite(std_obj[nnn]);
		}
	}
}
#if (0)
static void tachie_draw_my_sprite(int nnn)
{
	{
		if (NULL != std_obj[nnn])
		{
			if (std_obj[nnn]->flags)
			{
				SDL_Rect src_r,dst_r;
				src_r.x = std_obj[nnn]->w*std_obj[nnn]->aktframe;
				src_r.y = 0;
				src_r.w = std_obj[nnn]->w;
				src_r.h = std_obj[nnn]->h;
				dst_r.x = std_obj[nnn]->x;
				dst_r.y = std_obj[nnn]->y;
				if (std_obj[nnn]->flags==1)
				{	SDL_BlitSurface(std_obj[nnn]->img, &src_r, sdl_screen[SDL_00_SCREEN], &dst_r); }
				#if (1==USE_2ND_SCREEN)
				else if (std_obj[nnn]->flags==2)
				{	SDL_BlitSurface(std_obj[nnn]->img, &src_r, tachie_window, &dst_r);	}
				#endif /* (1==USE_2ND_SCREEN) */
			}
			std_obj[nnn] = work_my_sprite(std_obj[nnn]);
		}
	}
}
#endif
/*---------------------------------------------------------

---------------------------------------------------------*/
static int bg_alpha;		/* �w�i���l�p */
static void load_bg_aaa(char *filename, int alpha)
{
	bg_story_window = load_local(filename, bg_story_window, alpha);
	SDL_SetColorKey(bg_story_window,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
}
static int load_bg_local(char *filename, int alpha, int fade_command, int set_alpha_speed)
{
	static int bg_alpha_speed;		/* �w�i���l�p */
	switch (fade_command)
	{
	case -1:
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

---------------------------------------------------------*/

#define USE_KOMONO 0

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
	#if (1==USE_KOMONO)
	dr aw_my_sprite(0,9);/* ���� 10�� */
	#endif /* (1==USE_KOMONO) */
	draw_my_sprite(SPRITE_tachie_l,SPRITE_tachie_l);/* �����G 1�� */
	#if (1==USE_KOMONO)
	dr aw_my_sprite(10,11);/* ���� 2�� */
	#endif /* (1==USE_KOMONO) */
	draw_my_sprite(SPRITE_tachie_r,SPRITE_tachie_r);/* �����G 1�� */
	#if (1==USE_KOMONO)
	dr aw_my_sprite(12,15);/* ���� 4�� */

	/* 7.  ��`�� */
	if (is_filter)
	{
		SDL_BlitSurface(filter_window, NULL, sdl_screen[SDL_00_SCREEN], NULL);
	}

	/* 8.  ������`�� */
	dr aw_my_sprite(16,16);/* ���� 1�� */
	#endif /* (1==USE_KOMONO) */

	#if (1==USE_2ND_SCREEN)
	/* 9.  ��`��(�����G����) */
	if (is_tachie_window)
	{
		SDL_BlitSurface(tachie_window, NULL, sdl_screen[SDL_00_SCREEN], NULL);
	}
	#endif /* (1==USE_2ND_SCREEN) */

	#if (1==USE_KOMONO)
	/* 10.	������`�� */
	dr aw_my_sprite(17,17);/* ���� 1�� */

	/* 12.	������`�� */
	dr aw_my_sprite(18,19);/* ���� 2�� */
	#endif /* (1==USE_KOMONO) */
//


#if (000)

	/* 11.	����ӃE�B���h�E��`�� */
	if (draw_script_screen)
	{
		/* �E�B���h�E�̔������g ��`�� */
	//	if (draw_script_screen != 2)
		{
			#if (000)
			SDL_BlitSurface(std_window, NULL, sdl_screen[SDL_00_SCREEN], &msgw_rect);
			#endif /* (000) */
		}
		/*	*/
			SDL_BlitSurface(msg_window, NULL, sdl_screen[SDL_00_SCREEN], &msgw_rect);
	}
#endif /* (000) */
//
}


/*---------------------------------------------------------

---- ��肽������ ----
	1�t���[���ł͏I���Ȃ��������������Ƃ��̂��߂ɂ��̃R�}���h����̖��߂ɂ͑S��
	�I��������Ƃ�m�点�������t���Ă������ƁB=>done�ɑ���ŏI���B
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
			while ((NULL != ssc->next) && (SCP_JUMP != ssc->command))
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
			case SCP_LOADCUR:		/* �J�[�\���ʒu�A���� */
			//	count_char=cursor;
				cursor_x = cursor_x_chached;
				cursor_y = cursor_y_chached;
				ssc->done = 1;
				break;
			case SCP_SAVECUR:		/* �J�[�\���ʒu�A�L�� */
			//	cursor=count_char;
				cursor_x_chached = cursor_x;
				cursor_y_chached = cursor_y;
				ssc->done = 1;
				break;
			case SCP_CLCURSOR:		/* �J�[�\���̏����� */
			//	count_char=0;
				cursor_x=0;
				cursor_y=0;
				ssc->done = 1;
				break;
			// [�e�L�X�g�\��/�N���b�N�҂�]
			case SCP_CLICK:
				if (0==shot_ositenai)
				{	ssc->done = 1;	}
				break;
			case SCP_BGTEXT:
				if (0==shot_ositenai)	{	ssc->para2=0;	}	/* �V���b�g��������A�c��͑S������ */
				ssc->done = print_kanji(bg_story_window, ssc->data, ssc->para0, ssc->para1, ssc->para2);
				break;
			case SCP_TEXT:
				if (0 == tmp_all[ssc->chain])
				{
					if (0==shot_ositenai)	{	ssc->para2=0;	}	/* �V���b�g��������A�c��͑S������ */
					tmp_all[ssc->chain] = print_kanji(msg_window, ssc->data, ssc->para0, ssc->para1, ssc->para2);
				}
				else	/*if (1==tmp_all[ssc->chain])*/
				{
					if (0==(ssc->para3 & 0xf8))
					{
						if ( 0x00 == ( (ssc->para3 & shot_ositenai) & 0x01) )
						{
							if (0x00==(ssc->para3 & 0x02))
							{
								;//cursor_continue = 1; 	/* �J�[�\���p�� */
							}
							else
							{
						//	//	count_char=0;
								cursor_x=0; 	/* �J�[�\�������� */
								cursor_y=0; 	/* �J�[�\�������� */
							}
							if (0x04==(ssc->para3 & 0x04))
							{
								msg_window_clear(); 	/* �E�B���h�E������ */
								msg_window_init();		/* �E�B���h�E������ */
							}
							ssc->done = 1;
							tmp_all[ssc->chain]=0;
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
			case SCP_FILTER:
				filter_init(ssc->para1,ssc->para2,ssc->para3);
				ssc->done = 1;
				break;
			case SCP_JUMP:
				ssc->done = 1;
				break;
			case SCP_LOADBG:
			//	bg_story_window = load_local(ssc->para0, bg_story_window, ssc->para1);
			//	SDL_SetColorKey(bg_story_window,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
			//	ssc->done = 1;
				ssc->done=load_bg_local(ssc->para0,ssc->para1,ssc->para2,ssc->para3);
				break;
			case SCP_LOAD_LR:	/* �����GL �����GR */
				load_local_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3,ssc->data);
				ssc->done = 1;
				break;
			case SCP_LOADSP:
				load_sc_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3,ssc->data);
				ssc->done = 1;
				break;
			case SCP_MOVESP:
				ssc->done=do_move_sc_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3);
				if (ssc->done == -1)
				{
					#if 0
					//ps pDebugScreenPrintf("no sprite in No.%s\n",ssc->para0);
					//sc eKernelDelayThread(2000000);
					#endif
					return_the_script=(1);
					return /*(1)*/;
				}
				break;
			case SCP_PARAMSP:
				tmp_all[ssc->chain] = cha_search(ssc->para0);
				if (tmp_all[ssc->chain] == -1)
				{
					return_the_script=(-1);
					return /*(-1)*/;
				}
				{
					SC_SPRITE *sc_tmp;
//					sc_tmp = NULL;
				//	else if (tmp_all[ssc->chain] == -2) {	sc_tmp=std_obj[SPRITE_tachie_r];	}
				//	else if (tmp_all[ssc->chain] == -3) {	sc_tmp=std_obj[SPRITE_tachie_l];	}
//					else
					{	sc_tmp=std_obj[((tmp_all[ssc->chain])&(SPRITE_MAX-1))]; }
					if (ssc->para1 != -1)
					{
						sc_tmp->alpha=ssc->para1;
						SDL_SetAlpha(sc_tmp->img, SDL_SRCALPHA, sc_tmp->alpha);
					}
					if (ssc->para2 != -1)
					{
						sc_tmp->anim_speed=ssc->para2;
					}
					if (ssc->para3 > 4) 		{	sc_tmp->anim_type = 0;				}
					else if (ssc->para3 == 3)	{	sc_tmp->anim_type = 3;	sc_tmp->aktframe = sc_tmp->frames-1;	}
					else if (ssc->para3 != -1)	{	sc_tmp->anim_type = ssc->para3; 	}
				}
					ssc->done = 1;
				break;
			case SCP_RELOADSP:
				reload_sc_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3);
				ssc->done = 1;
				break;
			case SCP_SUBG:
				is_bg = ssc->para1; 	ssc->done = 1;
				break;
			case SCP_SUFILTER:
				is_filter = ssc->para1;
				SDL_SetAlpha(filter_window, SDL_SRCALPHA, ssc->para2);
				ssc->done = 1;
				break;
			case SCP_SUL:
				std_obj[SPRITE_tachie_l]->flags = ssc->para1; ssc->done = 1;
				break;
			case SCP_SUR:
				std_obj[SPRITE_tachie_r]->flags = ssc->para1; ssc->done = 1;
				break;
			case SCP_SUSPRITE:
			//		 if (ssc->para1==-2)	{	std_obj[SPRITE_tachie_r]->flags = ssc->para2; }
			//	else if (ssc->para1==-3)	{	std_obj[SPRITE_tachie_l]->flags = ssc->para2; }
			/*	else	*/					{	std_obj[((ssc->para1)&(SPRITE_MAX-1))]->flags = ssc->para2; }
				ssc->done = 1;
				break;
			case SCP_SUTWINDOW:
				#if (1==USE_2ND_SCREEN)
				is_tachie_window=ssc->para1;
				#endif /* (1==USE_2ND_SCREEN) */
			//	is_pa nel_window=ssc->para2;
				{
					PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
					pd->state_flag &= (~(STATE_FLAG_09_IS_PANEL_WINDOW));
				//	pd->state_flag |= ((ssc->para2&1)<<STATE_FLAG_IS_PANEL_WINDOW_SHIFT);
					if (1 == ssc->para2)
					{
						pd->state_flag |= STATE_FLAG_09_IS_PANEL_WINDOW;
					}
				}
				ssc->done = 1;
				/*�ǉ� is_pa nel_window 20090406 */
				break;
			case SCP_SUWINDOW:		/* ���܂������ĂȂ� */
				ssc->done=window_effect(ssc->para1,ssc->para2);
				break;

			case SCP_TWINDOW:
				#if (1==USE_2ND_SCREEN)
				tachie_window_init();
				#endif /* (1==USE_2ND_SCREEN) */
				ssc->done = 1;
				break;
			case SCP_WAIT:
				ssc->done = script_wait(ssc->para1);
				break;
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
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	/*�{�X�퓬��C�x���g*/
#if 0
	if (/*STATE_FLAG_05_IS_BOSS == */(pd->state_flag & STATE_FLAG_05_IS_BOSS))
	//if (pd->bo ssmode==B06_AFTER_EVENT)
	{
		work_the_script();
		if (1==return_the_script)
		{
			pd->state_flag &= (~(STATE_FLAG_06_IS_SCRIPT));
			pd->state_flag |= STATE_FLAG_12_END_SCRIPT; 	/*	pd->bo ssmode=B09_STAGE_LOAD;*/
		}
		/*draw_the_script();*/
	}	// [***090313	�ǉ�	�����Ɉړ��B
	/*�{�X�퓬�O�C�x���g*/
	else
	//if (pd->bo ssmode==B03_BEFORE_EVENT)
#endif
	{
		work_the_script();
	//	if (1==return_the_script)	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
		if (0!=return_the_script)
		{
			draw_script_screen = 0; /* ����ӃE�B���h�E�\���t���O off */
			pd->state_flag &= (~(STATE_FLAG_06_IS_SCRIPT));
			pd->state_flag |= STATE_FLAG_12_END_SCRIPT; 	/*pd->bo ssmode=B08_START;*/
		}
		/*else	{	draw_the_script();}*/
	}	// [***090313	�ǉ�
}

/*---------------------------------------------------------

---------------------------------------------------------*/

extern int select_player;
void script_load(void/*int mode*/)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	char buffer1[16/*10*/];
	#if 0
	/*�{�X�퓬��C�x���g*/
	if (/*STATE_FLAG_05_IS_BOSS == */(pd->state_flag & STATE_FLAG_05_IS_BOSS))
	/*B07_AF TER_LOAD*/
	{
		sp rintf(buffer1,"stage%d-%d_end",player_now_stage,select_player);	/* sp rintf()�͒x�� */
	}
	/*�{�X�퓬�O�C�x���g*/
	else
//	if (0==mode)/*B05_BE FORE_LOAD*/
	{
		set_music_volume(80);
		sp rintf(buffer1,"stage%d-%d",player_now_stage,select_player);	/* sp rintf()�͒x�� */
	}
	#else
	strcpy(buffer1,"stageZ-Z_end");
	buffer1[5] =('0'+player_now_stage);
	buffer1[7] =('0'+select_player);
	if (/*STATE_FLAG_05_IS_BOSS == */(pd->state_flag & STATE_FLAG_05_IS_BOSS))
	{;}
	else
	{	buffer1[8] = 0; 	set_music_volume(80);	}
	#endif
//
	if (0 == script_init(buffer1, NULL, 380))		// �t�@�C�����Ȃ��ꍇ�̓C�x���g���΂�
			{	pd->state_flag |= STATE_FLAG_12_END_SCRIPT; 		/*pd->bo ssmode=B09_STAGE_LOAD;*/	}
	else	{	pd->state_flag |= STATE_FLAG_06_IS_SCRIPT;			/*pd->bo ssmode=B00_NONE;*/ /*B06_AFTER_EVENT*/ 	}
//
//	if (0 == script_init(buffer1, NULL, 380))
//			{	pd->state_flag |= STATE_FLAG_12_END_SCRIPT; 		/*pd->bo ssmode=B08_START;*/ }
//	else	{	pd->state_flag |= STATE_FLAG_06_IS_SCRIPT;			/*pd->bo ssmode=B00_NONE;*/ /*B03_BEFORE_EVENT*/	}
}
/*---------------------------------------------------------

---------------------------------------------------------*/

int script_init(char *filename, char *bg_name, int width)		/* �V�i���I�t�@�C�����Ɣw�i�t�@�C���� */
{
//	380/*scenario_width*/=width;
	if (NULL != bg_name)
	{
		load_bg_aaa(bg_name, 0/*alpha*/);
	//	bg_story_window = load_local(bg_name, bg_story_window, 0);
	//	SDL_SetColorKey(bg_story_window,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	}
	if (0 == load_scenario(filename))
	{
		return (0);
	}
	/* �E�B���h�E�̔������g */
//	std_window			= loadbmp0("fonts/window.png", 1, 1);/*2*/
	msg_window_clear(); 	/* �E�B���h�E������ */
	msg_window_init();
	msgw_rect_init(20,182);
	#if (1==USE_2ND_SCREEN)
	tachie_window_init();
	#endif /* (1==USE_2ND_SCREEN) */
	inits				= 1;
	is_bg				= 0;
	draw_script_screen			= 0;
	#if (1==USE_2ND_SCREEN)
	is_tachie_window	= 0;
	#endif /* (1==USE_2ND_SCREEN) */
	is_filter			= 0;
//	count_char			= 0;
//	cursor				= 0;
	cursor_x=0; 			/* �J�[�\�������� */
	cursor_y=0; 			/* �J�[�\�������� */
	cursor_x_chached=0; 	/* �J�[�\�������� */
	cursor_y_chached=0; 	/* �J�[�\�������� */

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

enum /*_story_state_*/
{
	STORY_INIT=0,
//	STORY_FADEOUT1,
//	STORY_LOAD,
//	STORY_FADEIN,
	STORY_WORKS,
//	STORY_FADEOUT2,
	STORY_QUIT
};
//	static SDL_Surface *fade_out_window;			/* ���o�p */
//	static int tick;								/* ���ԑ���p */

void story_work(void)
{
	switch ((Uint8)(psp_loop&0xff)/*state.substate*/)
	{
	case STORY_INIT:
#if 0
//		//void story_init(void)
//		if (NULL==fade_out_window)
//		{
//			fade_out_window=loadbmp2("fonts/fade_black.png");
//		}
//		bg_alpha=0;
//		//keyboa rd_clear();
//		tick=0;
//		psp_loop++;//newsta te(ST_STORY,STORY_FADEOUT1,0);
//		break;
//	case STORY_FADEOUT1:
//		if (tick>30)
//		{
//			psp_loop++;//newsta te(ST_STORY,STORY_LOAD,0);
//			#if 1
//			/*	�����ǂ��킩��Ȃ����ǁA������ok???�B	*/
//			if (NULL!=fade_out_window)
//			{
//				unloadbmp_by_surface(fade_out_window);
//			}
//			#endif
//		}
//		else
//		{
//			SDL_BlitSurface(fade_out_window, NULL, sdl_screen[SDL_00_SCREEN], NULL);
//			tick++;
//		}
//		break;
//	case STORY_LOAD:
#endif
//		bg_alpha=0;
		script_init("story", /*NULL*/ NULL/*"story/arasuzi.jpg"*/ /*"story_bg.jpg"*/, 480);
//		bg_alpha=0;
		psp_loop++;//newsta te(ST_STORY,STORY_FADEIN,0);
		break;
//	case STORY_FADEIN:
//			psp_clear_screen();
//			SDL_SetAlpha(bg_story_window, SDL_SRCALPHA, bg_alpha);
//			SDL_BlitSurface(bg_story_window, NULL, sdl_screen[SDL_00_SCREEN], NULL);
//			if (bg_alpha<255)
//			{
//				bg_alpha += 5;
//			}
//			else
//			{
//				bg_alpha = 255;
//				#if 1
//				/*	�����ǂ��킩��Ȃ����ǁA�����͂�������???�B */
//				if (NULL!=bg_story_window)
//				{
//					unloadbmp_by_surface(bg_story_window);
//				}
//				#endif
//				psp_loop++;//newsta te(ST_STORY,STORY_WORKS,0);
//			}
//			break;
	case STORY_WORKS:
		psp_clear_screen();
		work_the_script();
		/*draw_the_script();*/
		if (return_the_script)
		{
			psp_loop++;//newsta te(ST_STORY,STORY_QUIT,0);
		}
		break;
//	case STORY_FADEOUT2:
//		break;
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
		/* ���C�����j���[�ɖ߂� */
		psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);//newsta te(ST_MENU/*ST_INTRO*/,0/*1*/,1);
		break;
	}
}


/*---------------------------------------------------------
	psp�ł͊J�������퓮��o���Ȃ��̂ŁA�N�����Ɋm�ۂ���(�I�����܂�)�J�����Ȃ�
---------------------------------------------------------*/
extern UINT16 *msg_window_image;
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
	#if (1==USE_2ND_SCREEN)
	is_tachie_window	= 0;	/* �����G�����\���t���O */
	#endif /* (1==USE_2ND_SCREEN) */
	is_bg				= 0;	/* �w�i�\���t���O */
	is_filter			= 0;	/* �t�B���^�[�\���t���O */
//
//	count_char			= 0;
//	cursor				= 0;
	cursor_x			= 0;	/* �J�[�\���ʒu */
	cursor_y			= 0;	/* �J�[�\���ʒu */
	cursor_x_chached	= 0;	/* �J�[�\���ʒu �ۑ��p */
	cursor_y_chached	= 0;	/* �J�[�\���ʒu �ۑ��p */
//
	#endif
	bg_alpha			= 255;	/* 255==�����l */

	/* SDL_FreeSurface(); �� psp�ł͑��������Ɠ����Ȃ��̂� ���̑΍� */

	#if (1==USE_2ND_SCREEN)
	/* psp�ł͊J�������퓮��o���Ȃ��̂ŁA�N�����Ɋm�ۂ��ĊJ�����Ȃ� */
	/*tachie_window_init()*/
//	tachie_window	= NULL; 	/* �D�旧���G�E�B���h�E */
//	if (NULL != tachie_window)	{	SDL_FreeSurface(tachie_window); tachie_window = NULL; }
	tachie_window	= SDL_CreateRGBSurface(/*SDL_SRCCOLORKEY|*/SDL_SWSURFACE,/* ���C���������֊m�ۂ��� */
			380/*scenario_width*/,
			272,
			sdl_screen[SDL_00_SCREEN]->format->BitsPerPixel,
			sdl_screen[SDL_00_SCREEN]->format->Rmask,
			sdl_screen[SDL_00_SCREEN]->format->Gmask,
			sdl_screen[SDL_00_SCREEN]->format->Bmask,
			sdl_screen[SDL_00_SCREEN]->format->Amask);
	SDL_SetColorKey(tachie_window,SDL_SRCCOLORKEY/*|SDL_RLEACCEL*/,0x00000000);
	#endif /* (1==USE_2ND_SCREEN) */
//
	/* psp�ł͊J�������퓮��o���Ȃ��̂ŁA�N�����Ɋm�ۂ��ĊJ�����Ȃ� */
	/*filter_init()*/
	#if 0
	if (NULL != filter_window)	{	SDL_FreeSurface(filter_window); filter_window = NULL; }
	#endif
	filter_window = SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_SWSURFACE,/* ���C���������֊m�ۂ��� */
			380/*scenario_width*/,
			272,
			sdl_screen[SDL_00_SCREEN]->format->BitsPerPixel,
			sdl_screen[SDL_00_SCREEN]->format->Rmask,
			sdl_screen[SDL_00_SCREEN]->format->Gmask,
			sdl_screen[SDL_00_SCREEN]->format->Bmask,
			sdl_screen[SDL_00_SCREEN]->format->Amask);
	SDL_SetColorKey(filter_window, SDL_RLEACCEL, 0x00000000);
//
		#if 1
		/*???*/
	//	font_color_bitmap=loadbmp(buffer);
	//	esc_len=0;
		{
			char fn[128/*64*/];
		//	strcpy(fn, data_dir);	strcat(fn, "/fonts/" "font_bg16.png" );
			strcpy(fn, DIRECTRY_NAME_DATA "/fonts/" "font_bg16.png" );
			font_bg_bitmap = IMG_Load(fn);/*�����Ń��[�h����Ɗm���ɏ��u���������*/
		}
		font_bg_bitmap = SDL_DisplayFormat(font_bg_bitmap);
		#endif

//
	/* psp�ł� SDL_FreeSurface() ������ɓ��삵�܂���̂ŁA���̂܂܂ł̓������s���H�Ńn���O�A�b�v���܂��B */
	/*msg_window_init()*/
//	if (NULL != msg_window) 	{	SDL_FreeSurface(msg_window);	msg_window = NULL;	}
	msg_window=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_SWSURFACE,/* ���C���������֊m�ۂ��� */
			320/*340*/,
			56/*70*/ /*100*/,
			sdl_screen[SDL_00_SCREEN]->format->BitsPerPixel,
			sdl_screen[SDL_00_SCREEN]->format->Rmask,
			sdl_screen[SDL_00_SCREEN]->format->Gmask,
			sdl_screen[SDL_00_SCREEN]->format->Bmask,
			sdl_screen[SDL_00_SCREEN]->format->Amask);
	if (SDL_MUSTLOCK(msg_window))	{	SDL_LockSurface(msg_window); }
	msg_window_image = (UINT16 *)msg_window->pixels;
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
	#if (1==USE_2ND_SCREEN)
	/*tachie_window_init()*/
//	if (NULL != tachie_window)	{	SDL_FreeSurface(tachie_window); tachie_window = NULL; }
	#endif /* (1==USE_2ND_SCREEN) */
	/*filter_init()*/
//	if (NULL != filter_window)	{	SDL_FreeSurface(filter_window); filter_window = NULL; }

	//	if (NULL != font_color_bitmap)	{	SDL_FreeSurface(font_color_bitmap); 		font_color_bitmap	= NULL; }
//		if (NULL != font_color_bitmap)	{	unloadbmp_by_surface(font_color_bitmap);	font_color_bitmap	= NULL; }
}
