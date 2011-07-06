
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����E�B���h�E����
---------------------------------------------------------*/

#include "kanji_system.h"

#include "111_my_file.h"/*is_kanji_1st()*/


#define FONT_WIDTH			(8)
#define FONT_HEIGHT 		(16)

/*
�����̖��Ƃ��āA���_�t�H���g16�ɂ�
2�o�C�g���p�����̓t�H���g�f�[�^�[���Ȃ�(�쐬����ĂȂ��B�쐬����Ύg����)
���̂��� 2�o�C�g���p�����͕\������Ȃ��B(�f�[�^�[���S�ċ󔒂̈�)
//
���̏�ŁA�����I�Ȗ��Ƃ��āApsp��2�o�C�g���p����(==NEC9800�V���[�Y�ˑ�����)��
�T�|�[�g����Ӗ����Ȃ��C������B
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
	{			return (1); 	}
	else	{	return (2); 	}
}
#endif

/*---------------------------------------------------------
	���_�����R�[�h�K�i �̕�����`�悷��q�֐�
---------------------------------------------------------*/

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

static int font_color_number;


/*
	������`�悷��`���̃e�N�X�`���[
*/
		#if (0==USE_32BIT_DRAW_MODE)
extern UINT16 *kanji_window_screen_image;						/* ���b�Z�[�W�E�B���h�E(�摜) */
		#else
extern UINT32 *kanji_window_screen_image;						/* ���b�Z�[�W�E�B���h�E(�摜) */
		#endif


#if (0)
//512static int font_bg_bitmap_surface_pitch;
//static int font_bg_bitmap_surface_width;
#endif

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
	STR_LIMIT_MAX_X_WIDTH	���ɕ��ׂ��镶���̃h�b�g��
*/


	//#define BG_FONT_HAIKEI_LENGTH ((512*4))	/* 4[laster](����)	*/
	#define BG_FONT_HAIKEI_LENGTH ((512*8)) 	/* 8[laster](16x16�h�b�g�t�H���g��(width==16x16==��256)�Ȃ̂ŁA��512���Z��8[laster]��) */

	//#define BG_FONT_HAIKEI_OFFSET ((512*16))	/* 16[laster] */
	#define BG_FONT_HAIKEI_OFFSET ((512*10))		/* 10[laster]==(8+1+1) */
//	#define BG_FONT_HAIKEI_OFFSET_U16 ((BG_FONT_HAIKEI_OFFSET*2))		/* 10[laster]==(8+1+1) */

static void s_draw_shinonome_moji(
	int x,
	int y,
	int code_offset,
	int hidari_gawa)
{
//	if (1==hidari_gawa) 	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	if (0!=hidari_gawa)
	{
		x += FONT_WIDTH;
	}
	if (x > (/*drawmap->w*/(320)/*drawmap_width*/-FONT_WIDTH) ) 	/* �͈͊O�Ȃ� */
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
				#if (0==USE_32BIT_DRAW_MODE)
				/*
					���܂����ǂ��킩��Ȃ�(���A�Ƃ肠��������)�B
					�A�h���X�̓o�C�g�P�ʂȂ̂� u8 �|�C���^�łb����Ɍv�Z�����Ă�B
					(short �Ȃ�2�Ŋ���Ƃ��Aint �Ȃ�4�Ŋ���Ƃ�)
					�ς���ꍇ�́A�萔(BG_FONT_HAIKEI_OFFSET_U16��)��ϐ�(dy,y,��)���ς��Ȃ��ƃ_���B
				*/
			//	s_getpixel16
			volatile u8 *src_p = (u8 *)(/*font_bg_bitmap_surface_image*/kanji_window_screen_image)	/*surface*/
					+ (dy * (512/*font_bg_bitmap_surface_pitch*/))		/*surface*/
					+ (dx+dx)
					+ (haikei_offset+haikei_offset);
			//	putpixel16
			volatile u8 *dst_p = (u8 *)(kanji_window_screen_image)/*surface->pixels*/
					+ (((dy+y))*(512*2))/*surface->pitch*/
					+ (dx+dx)
					+ (x+x)
					+ (BG_FONT_HAIKEI_OFFSET*2);	/* 512[dot]x 2[bytes](short) */
				*(u16 *)dst_p = (u32)(*(u16 *)src_p);
				#else
				/*
					���܂����ǂ��킩��Ȃ�(���A�Ƃ肠��������)�B
					�A�h���X�̓o�C�g�P�ʂȂ̂� u8 �|�C���^�łb����Ɍv�Z�����Ă�B
					(short �Ȃ�2�Ŋ���Ƃ��Aint �Ȃ�4�Ŋ���Ƃ�)
					�ς���ꍇ�́A�萔(BG_FONT_HAIKEI_OFFSET_U16��)��ϐ�(dy,y,��)���ς��Ȃ��ƃ_���B
				*/
			//	s_getpixel16
			volatile u8 *src_p = (u8 *)(/*font_bg_bitmap_surface_image*/kanji_window_screen_image)	/*surface*/
					+ (dy * (2*512/*font_bg_bitmap_surface_pitch*/))		/*surface*/
					+ (dx+dx+dx+dx)
					+ (haikei_offset+haikei_offset+haikei_offset+haikei_offset);
			//	putpixel16
			volatile u8 *dst_p = (u8 *)(kanji_window_screen_image)/*surface->pixels*/
					+ (((dy+y))*(2*512*2))/*surface->pitch*/
					+ (dx+dx+dx+dx)
					+ (x+x+x+x)
					+ (BG_FONT_HAIKEI_OFFSET*4);	/* 512[dot]x 2[bytes](short) */
			//	*(u16 *)dst_p = (u32)(*(u16 *)src_p);
				*(u32 *)dst_p = (u32)(*(u16 *)src_p);
				#endif
			}
		}
	}
}
			//	u8 *p = (u8 *)drawmap_image/*surface->pixels*/+y*drawmap_pitch/*surface->pitch*/+x+x;

/*---------------------------------------------------------
http://openlab.ring.gr.jp/efont/shinonome/
	shift jis�����R�[�h�K�i �� ���_�����R�[�h�K�i �֕ϊ���A�`�悷��
---------------------------------------------------------*/

static void s_draw_sjis_kanji(int x, int y, int high_byte, int low_byte)
{
	if (0==high_byte)	/* ���p�����̏ꍇ(ascii�R�[�h) */
	{
		/* ascii�R�[�h �� shinonome�R�[�h �ϊ� */
		static	const	u8/*int*/ sinonome_ascii_bank_offset[16] =
		{// +0	  +1	+2	  +3	 +4    +5	 +6    +7	  +8	+9	  +a	+b	   +c	 +d    +e	 +f
			0xff, 0xff, 0x00, 0x10,  0x20, 0x30, 0x40, 0x50,  0xff, 0xff, 0x60, 0x70,  0x80, 0x90, 0xff, 0xff
		};
	//		 if (low_byte < 0x20)	{	return; }				// ascii 0x00-0x1f
	//	else if (low_byte < 0x80)	{	low_byte -= 0x20; } 	// ascii 0x20-0x7f
	//	else if (low_byte < 0xa0)	{	return; }				// ascii 0x80-0x9f
	//	else if (low_byte < 0xe0)	{	low_byte -= 0x40; } 	// ascii 0xa0-0xdf
	//	else						{	return; }				// ascii 0xe0-0xff
	//	code_offset <<= 4;	//code_offset *= 16;	/* 9 */ 	// �����ł�9�͑���1�̕����Ɏg���o�C�g���B(1byte == 8bit)*9=72 == 6*12
		u8 idx8;
		idx8 = sinonome_ascii_bank_offset[((low_byte>>4) & 0x0f)];
		/* ���݃}�b�v�� 0xff �̏ꍇ�́A ���_�����R�[�h�K�i �ɂȂ������Ȃ̂ŕ\���ł��Ȃ� */
		if (0xff == idx8)
		{
			return;
		}
		idx8 = ((idx8)|((low_byte & 0x0f)));
		s_draw_shinonome_moji(x, y, (idx8), 0);/*low_byte<<4*/
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
			s_draw_shinonome_moji(x, y, (idx), 0);
		}
		else	/* if ( 2 == size_of_kanji(high_byte))*/	// 2�o�C�g�S�p
		#endif
		{
			idx += (low_byte+low_byte);/* x2 */
			s_draw_shinonome_moji(x, y, (idx), 0); /* �E���� */
			s_draw_shinonome_moji(x, y, (idx), 1); /* ������ */
		}
	}
}

/*---------------------------------------------------------
	������̕`��
---------------------------------------------------------*/
// static extern int cursor_continue;	/* �J�[�\���p�� */
static int cursor_x;					/* �J�[�\���ʒu */
static int cursor_y;					/* �J�[�\���ʒu */


static int my_string_offset;/*=0*/
/*static*/global void clear_my_string_offset(void)
{
	my_string_offset = 0;/* ������ */
}

/*---------------------------------------------------------
	�J�[�\�����w��ʒu�ֈړ�
---------------------------------------------------------*/
/*static*/global void set_cursor(unsigned int set_x, unsigned int set_y)
{
	cursor_x = set_x;
	cursor_y = set_y;
}

/*---------------------------------------------------------
	�J�[�\�����z�[���|�W�V�����ֈړ�
---------------------------------------------------------*/
/*static*/global void home_cursor(void)
{
	#if 1
	cursor_x = (0);
	cursor_y = (0);
	#else
	set_cursor(0, 0);
	#endif
}

/*---------------------------------------------------------
	���b�Z�[�W�E�B���h�E�̓��e�������B
---------------------------------------------------------*/

/*static*/global void kanji_window_clear_line(unsigned int line_num)
{
//	#define TEST_OFFSET (/*0*/512*10/2) /* 10 [laster? ] */
	#define TEST_OFFSET (0) 	/* put_pixel�g������(0) */
		#if (0==USE_32BIT_DRAW_MODE)
				#if 0
		line_num *= (512*(16+2)*(1/*3 2[�s]*/)/**16*/ *2/*[short]==2[byte]�̕�*/ );
	{
	//	u8 *p = (u8 *)kanji_window_screen_image/*surface->pixels*/;//+y*(512*2)/*surface->pitch*/+x+x;	/* 512[dot]x 2[bytes](short) */
		unsigned int dx;
		for (dx=(/*0*/TEST_OFFSET); dx<(TEST_OFFSET)+(512*(16+2)*(1/*3 2[�s]*/)/**16*/); dx++)
		{
			u8 *p = (u8 *)kanji_window_screen_image/*surface->pixels*/+/*y*(512*2)*/ /*surface->pitch*/+dx+dx+(BG_FONT_HAIKEI_OFFSET*2)+line_num;	/* 512[dot]x 2[bytes](short) */
			*(u16 *)p = 0/*pixel*/;
		}
	}
				#endif
	{
	//	u8 *p = (u8 *)kanji_window_screen_image/*surface->pixels*/;//+y*(512*2)/*surface->pitch*/+x+x;	/* 512[dot]x 2[bytes](short) */
		u16 *p16;	/* VRAM��16bit���[�h */
		p16 = kanji_window_screen_image;
		p16 += (BG_FONT_HAIKEI_OFFSET); 							/* ��:16bit�|�C���^�Ƃ��Ă̒l�A�܂�(unsigned short������)2�{[byte]�����B */
		p16 += (line_num * (512*(16+2)*(1/*3 2[�s]*/)/**16*/)); 	/* ��:16bit�|�C���^�Ƃ��Ă̒l�A�܂�(unsigned short������)2�{[byte]�����B */
		unsigned int dx;
		for (dx=(0); dx<(0)+(512*(16+2)*(1/*3 2[�s]*/)/**16*/); dx++)
		{
		//	(*p16) = (0xffff)/*pixel*/;/* �Ă���(�`���Ă鏊�����Ȃ�) */
			(*p16) = (0x0000)/*pixel*/;/* ��(16bit) */
			p16++;
		}
	}
		#else
		line_num *= (512*(16+2)*(1/*3 2[�s]*/)/**16*/ *4/*[int]==4[byte]�̕�*/ );
	{
	//	u8 *p = (u8 *)kanji_window_screen_image/*surface->pixels*/;//+y*(512*2)/*surface->pitch*/+x+x;	/* 512[dot]x 2[bytes](short) */
		unsigned int dx;
		for (dx=(/*0*/TEST_OFFSET); dx<(TEST_OFFSET)+(512*(16+2)*(1/*3 2[�s]*/)/**16*/); dx++)
		{
			u8 *p = (u8 *)kanji_window_screen_image/*surface->pixels*/+/*y*(512*2)*/ /*surface->pitch*/+dx+dx+dx+dx+(BG_FONT_HAIKEI_OFFSET*4)+line_num+line_num;	/* 512[dot]x 2[bytes](short) */
			*(u32 *)p = 0/*pixel*/;
		}
	}
		#endif
}
/*static*/global void kanji_window_clear(void)
{
//	#define TEST_OFFSET (/*0*/512*10/2) /* 10 [laster? ] */
	#define TEST_OFFSET (0) 	/* put_pixel�g������(0) */
		#if (0==USE_32BIT_DRAW_MODE)
	{
	//	u8 *p = (u8 *)kanji_window_screen_image/*surface->pixels*/;//+y*(512*2)/*surface->pitch*/+x+x;	/* 512[dot]x 2[bytes](short) */
		int dx;
		for (dx=(/*0*/TEST_OFFSET); dx<(TEST_OFFSET)+(512*(16+2)*(3/*3 2[�s]*/)/**16*/); dx++)
		{
			u8 *p = (u8 *)kanji_window_screen_image/*surface->pixels*/+/*y*(512*2)*/ /*surface->pitch*/+dx+dx+(BG_FONT_HAIKEI_OFFSET*2);	/* 512[dot]x 2[bytes](short) */
			*(u16 *)p = 0/*pixel*/;
		}
	}
		#else
	{
	//	u8 *p = (u8 *)kanji_window_screen_image/*surface->pixels*/;//+y*(512*2)/*surface->pitch*/+x+x;	/* 512[dot]x 2[bytes](short) */
		int dx;
		for (dx=(/*0*/TEST_OFFSET); dx<(TEST_OFFSET)+(512*(16+2)*(3/*3 2[�s]*/)/**16*/); dx++)
		{
			u8 *p = (u8 *)kanji_window_screen_image/*surface->pixels*/+/*y*(512*2)*/ /*surface->pitch*/+dx+dx+dx+dx+(BG_FONT_HAIKEI_OFFSET*4);	/* 512[dot]x 4[bytes](int) */
			*(u32 *)p = 0/*pixel*/;
		}
	}
		#endif
}
			//	ccc_putpixel16(dx);//, 0, /*0xaaaa*/(0x0000)/*dot color16 */);
		//	*(u16 *)p = 0xaa55;
		//	p++;
		//	u8 *p = (u8 *)drawmap_image/*surface->pixels*/+y*drawmap_pitch/*surface->pitch*/+x+x;

	#if 0/*test*/
	{
		int kk;
		for (kk=0; kk<(BG_FONT_HAIKEI_LENGTH)/*6*/; kk++)
		{
			(*(kanji_window_screen_image+kk)) = (*(font_bg_bitmap_surface_image+kk));/*BG_FONT_HAIKEI_OFFSET_U16*/
		}
	}
	#endif

#if 0
/*---------------------------------------------------------
	�O�����烁�b�Z�[�W�E�B���h�E�Ɋ�����`�悷��ꍇ�̃��Z�b�g
---------------------------------------------------------*/

global /*static*/ void script_message_window_clear(void)
{
	kanji_window_clear();	/* �����E�B���h�E�̓��e�������B */
//	msg_window_init();/*???*/

//	inits				= 1;/*???*/
//	is_bg				= 0;/*???*/
//	draw_script_screen	= 0;/*???*/

	home_cursor();		/* �J�[�\�����z�[���|�W�V�����ֈړ� */
//	cursor_x_chached	= 0;/*???*/ /* �J�[�\�������� */
//	cursor_y_chached	= 0;/*???*/ /* �J�[�\�������� */
//	clear_my_string_offset();/*???*/

	//set_write_text(0);		/* ���b�Z�[�W�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
}
#endif

//static void msg_window_init(void)
//{
//
//}


/*---------------------------------------------------------
	shift jis���� �������Ȃ܂��菑�̂̕������`�悷��
	 (wait�w��ŁAframe���Ɉꕶ���`��B wait0�ŁA������S�`��)
---------------------------------------------------------*/

global /*static*/ int print_kanji000(const char *str, int color_type, int wait)
{
	font_color_number = (color_type & 0x0f);
//
	int terminate_this_frame;	terminate_this_frame=0;
	int need_draw_this_flame;	need_draw_this_flame=0;
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
			#define IS_LOCK_LOCAL_SCREEN (0)/* �ǂݍ��݂����Ȃ�A���b�N����K�v�͂Ȃ�(?) */
			#if (1==IS_LOCK_LOCAL_SCREEN)
			if (SD L_MUSTLOCK(font_bg_bitmap_surface))	{	SD L_LockSurface(font_bg_bitmap_surface);	}	/* ���b�N */
			#endif
		loop:
		;
		/*unsigned char*/int high_byte;
		high_byte = (*(str+my_string_offset) & 0xff);
		if (0x00 != high_byte )
		{
			int xx;
			int yy;
			xx = 0/*(rect->x)*/+(cursor_x)*(FONT_WIDTH);
			yy = 0/*(rect->y)*/+(cursor_y)*(FONT_HEIGHT+2);
		//	�p�t�H�[�}���X���ቺ����̂łȂ�ׂ����b�N���Ȃ��B(���b�N�͍ŏ�����)
		/* �����b�N */
			if (is_kanji_1st(high_byte)) /* shift jis ���� */
			{
			/*unsigned char*/int low_byte;	low_byte = (*(str+my_string_offset+1) & 0xff);
				s_draw_sjis_kanji(xx, yy, high_byte, low_byte);
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
						my_string_offset += 2;
						cursor_x = 999;/* �K���ɑ傫�Ȓl���w�肵�ĉ��s������ */
					//	cursor_x = 0;
					//	cursor_y++;
					}
				}
				else					/* ���p���� */
				{
					s_draw_sjis_kanji(xx, yy, 0, high_byte);
					my_string_offset++;
					cursor_x++;
				}
			}/* �����b�N���� */
			if (0==(color_type & STR_CODE_NO_ENTER))
			{
				/* ���s���� */
				#define STR_LIMIT_MAX_X_WIDTH ((int)((300/*(rect->w)*/)/FONT_WIDTH)+2)
				if ( (STR_LIMIT_MAX_X_WIDTH-1) < cursor_x)
				{
					cursor_x = 0;
					cursor_y++;
					#define MAX_cursor_y (2)
					if (MAX_cursor_y < cursor_y)
					{
						cursor_y = 0;
					//	home_cursor();
						kanji_window_clear();
					}
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
			if (SD L_MUSTLOCK(font_bg_bitmap_surface))	{	SD L_UnlockSurface(font_bg_bitmap_surface); 	}	/* ���b�N���� */
			#endif
	//	if (1==terminate_this_frame)	//�I������	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
		if (0!=terminate_this_frame)	//�I������
		{
			{
			//	STR_LIMIT_MAX_X_WIDTH=1/* 0 (����Z�Ȃ̂�0�͊댯) */;/*???*/
				my_string_offset = 0;	/* ����́A������ */
				//cursor_continue=0;	/* ����́A�J�[�\���p�����Ȃ� */
			}
		}
	}
	return (terminate_this_frame);		/* 0:continue, 1:end */
}


/*---------------------------------------------------------

---------------------------------------------------------*/

#include "graphics00.h"
global void kanji_system_init(void)/* �g�ݍ��� */
{
	#if (0==USE_32BIT_DRAW_MODE)
		/* ���b�Z�[�W�\���pRAM�m�� */	/* ���ݕ\����16bit�F�Ȃ̂�(32bit�F�\���Ȃ�ς��) */
		/* �w�i�p��10[laster]�]���Ɋm�� */
	//	kanji_window_screen_image	= (UINT16 *)malloc((512*(10+64)*2));	/* (10+ 64)[laster] */
		kanji_window_screen_image	= (UINT16 *)malloc((512*(10+128)*2));	/* (10+128)[laster] */
	#else
		/* �F32bit���[�h */
		/* ���b�Z�[�W�\���pRAM�m�� */	/* ���ݕ\����16bit�F�Ȃ̂�(32bit�F�\���Ȃ�ς��) */
		/* �w�i�p��10[laster]�]���Ɋm�� */
	//	kanji_window_screen_image	= (UINT16 *)malloc((512*(10+64)*2));	/* (10+ 64)[laster] */
		kanji_window_screen_image	= (UINT32 *)malloc((2*512*(10+128)*2)); /* (10+128)[laster] */
	#endif
//
	{
	/* �����w�i�p�摜�𕶎��`�惏�[�N�̐擪�ɓǂݍ��݁A�摜���R�s�[���Ă���A�J������B */
	#if 1
	/*static*/ /*SDL_Surface*/my_image *font_bg_bitmap_surface;/*=NULL*/
	//	{
	//		char fn[128/*64*/];
	//		strcpy(fn, DIRECTRY_NAME_DATA_STR "/fonts/" "font_bg16.png" );
	//		font_bg_bitmap_surface = IM G_Load(fn);/*�����Ń��[�h����Ɗm���ɏ������������*/
	//		font_bg_bitmap_surface = IM G_Load(DIRECTRY_NAME_DATA_STR "/fonts/" "font_bg16.png");/*�����Ń��[�h����Ɗm���ɏ������������*/
			font_bg_bitmap_surface = png_load_my_image(DIRECTRY_NAME_DATA_STR "/fonts/" "font_bg16.png");/*�����Ń��[�h����Ɗm���ɏ������������*/
	//	}
	//	font_bg_bitmap_surface			= SDL_DisplayFormat(font_bg_bitmap_surface);/*�T�[�t�F�X��\���t�H�[�}�b�g�ɕϊ�����B*/
	//	font_bg_bitmap_surface_pitch	= font_bg_bitmap_surface->pitch;
		#endif
		#if (0==USE_32BIT_DRAW_MODE)
		{
			/* ���ݕ\����16bit�F�Ȃ̂ŁA32->16�ϊ�����B(32bit�F�\���Ȃ�ς��) */
		//	/*static*/ UINT16 *font_bg_bitmap_surface_image;
		//	font_bg_bitmap_surface_image	= font_bg_bitmap_surface->pixels;
			u32 *pixsrc;
			u16 *pixdst;
			pixsrc = (void *)((font_bg_bitmap_surface->pixels));
			pixdst = (kanji_window_screen_image);
			trans_format8888to5650(pixsrc, pixdst, (BG_FONT_HAIKEI_LENGTH));
		}
		#else
		{
			/* ���ݕ\����16bit�F�Ȃ̂ŁA32->16�ϊ�����B(32bit�F�\���Ȃ�ς��) */
		//	/*static*/ UINT16 *font_bg_bitmap_surface_image;
		//	font_bg_bitmap_surface_image	= font_bg_bitmap_surface->pixels;
			u32 *pixsrc;
			u32 *pixdst;
			pixsrc = (void *)((font_bg_bitmap_surface->pixels));
			pixdst = (kanji_window_screen_image);
			{
				int kk;
				for (kk=0; kk<(BG_FONT_HAIKEI_LENGTH); kk++)
				{
				//	(*(kanji_window_screen_image+kk)) = (*(/*font_bg_bitmap_surface_image*/(u32 *)(font_bg_bitmap_surface->pixels)+kk));/*BG_FONT_HAIKEI_OFFSET_U16*/
					/* --- 16bit�F�� ���F���ĕێ� (color key �g�p�s��) */
					*pixdst =  ((*pixsrc));
					pixdst++;
					pixsrc++;
				}
			}
		}
		#endif
		/* �����w�i�p�摜���J������B */
	//	SDL_FreeSurface(font_bg_bitmap_surface);
		png_free_my_image(font_bg_bitmap_surface);
	}
}


/*---------------------------------------------------------
	�I�����ɊJ�����镔��...�������ǁA
	�n���O�A�b�v�����肷��̂ŁA��芸���������ɂȂ��Ă�B
	(�J�����Ȃ��Ă��I������΁AOS���������������̂Ŗ��Ȃ����Č����Ζ��Ȃ�)
---------------------------------------------------------*/

global void kanji_system_exit(void)/* �O�� */
{
	/*msg_window_init()*/
//
	#if 0/* �{���͗v�� */
//	kanji_system_terminate();
	if (NULL != kanji_window_screen_image)		{	free(kanji_window_screen_image);					kanji_window_screen_image = NULL;	}
	#endif
}
