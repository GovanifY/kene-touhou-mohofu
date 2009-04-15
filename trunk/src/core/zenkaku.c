/*
**************************************************************
*�S�p�����\�����C�u���� zenkaku.c / shinonomefont.c
**************************************************************

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
************************************************************/

/*---------------------------------------------------------
  zenkaku.c
  �S�p������\���T���v��

---------------------------------------------------------*/
#include <SDL/SDL_image.h>
//#include	<pspuser.h>
//#include	<pspgu.h>
//#include	<pspdisplay.h>
#include "zenkaku.h"
/*---------------------------------------------------------
  macros
---------------------------------------------------------*/
/* �L���b�V���Ȃ�VRAM  VRAM[y][x]�Ƃ��ăA�N�Z�X */
//#define	VRAM		((long(*)[BUF_WIDTH])(((char*)0x4000000)+0x40000000))


/*---------------------------------------------------------
  for script
---------------------------------------------------------*/
extern int count_char;
extern double fps_factor;

/* global �O���[�o�� */
int count_i=0;
SDL_Surface *col_sdl=NULL;


/*---------------------------------------------------------
  shift jis�R�[�h�A�S�p��o�C�g�ڂ��ǂ�������
---------------------------------------------------------*/
static int iskanji(int c)
{
	c &= 0xff;
	return ((c >= 0x81) && (c < 0xa0)) || ((c >= 0xe0) && (c <= 0xfd));
}


/*---------------------------------------------------------
  �����̕`�敝�𕶎��P�ʂŕԂ�
---------------------------------------------------------*/
static int chrwidth(int hi)
{
	hi &= 0xff;
	if (0xfd == hi) 	// �Q�o�C�g���p
	{
		return 1;
	}
	return iskanji(hi) ? 2 : 1;
}


/*---------------------------------------------------------
  ���_�����̕`��(shinonome�R�[�h)������VRAM�ɓ]�����܂���
---------------------------------------------------------*/
static void draw_char(SDL_Surface *src, int x, int y, SDL_Surface *col_n, int chr)
{
	/*
		x	�\�����W
		y	�\�����W
		col_n	�t�H���g�̎��
		chr ����
	*/
	extern	const	char	shinonomefont[];
	int dx, dy, bit, bitcnt;
	/*
		dx		�h�b�gx
		dy		�h�b�gy
		bit 	���ׂ�byte���
		bitcnt	���ׂĂ�byte���㉽�񒲂ׂ��邩:�ő�8��(��q)
				bit��&���邱�ƂŒ��ׂĂ�byte��bit���𓾂�B
	*/
	if (x > src->w-FONTWIDTH)
	{
		return;
	}
	bitcnt = 0;
	bit = 0;
	chr *= 16	/* 9 */;	// �����ł�9�͑���1�̕����Ɏg���o�C�g���B(1byte == 8bit)*9=72 == 6*12
//	col_n &= 0xffffff;
	for (dy = 0 ; dy < FONTHEIGHT ; dy++)
	{
		for (dx = 0 ; dx < FONTWIDTH ; dx++)
		{
			bitcnt >>= 1;
			if (!bitcnt)	// bitcnt==0�̎�
			{
				bit = shinonomefont[chr++];
				bitcnt = 0x80;		// 1byte�̏��ʂ�8�̈� 80 40 20 10 8 4 2 1(0�ɂȂ����炷��80�ɂȂ邽�߃J�E���g����Ȃ�)
			}
			if (bit & bitcnt)
			{
				Uint32 dot;
				dot=getpixel(col_n, dx, dy);
				putpixel(src, dx+x, dy+y, dot);
			}
		}
	}
}


/*---------------------------------------------------------
  ���p�����̕`��(ascii�R�[�h)
---------------------------------------------------------*/
static void draw_hankaku(SDL_Surface *src, int x, int y, int ch, SDL_Surface *col_n)
{
	/* ascii�R�[�h �� shinonome�R�[�h �ϊ� */
		 if (ch < 0x20) {	return; }		// ascii 0x00-0x1f
	else if (ch < 0x80) {	ch -= 0x20; }	// ascii 0x20-0x7f
	else if (ch < 0xa0) {	return; }		// ascii 0x80-0x9f
	else if (ch < 0xe0) {	ch -= 0x40; }	// ascii 0xa0-0xdf
	else				{	return; }		// ascii 0xe0-0xff
	draw_char(src,x,y,col_n,ch);
}


/*---------------------------------------------------------
  �S�p�����̕`��(shift jis�R�[�h)
---------------------------------------------------------*/
static void draw_zenkaku(SDL_Surface *src, int x, int y, int hi_byte, int lo_byte, SDL_Surface *col_n)
{
	/*
		hi	���byte
		lo	����byte
	*/
	/*���ʃo�C�g�̃`�F�b�N*/
	if (lo_byte < 0x40) 	// 0x40�ȉ��̏ꍇ�͊Y�����銿�������݂��Ȃ����߁B
	{
		return;
	}
	lo_byte -= 0x40;
	/*��ʃo�C�g����C���f�b�N�X�𓾂�*/
	int idx;
	{
		static	const	signed char/*int*/ codetbl[] =
		{
			-1, 0, 1, 2, 3,-1,-1, 4, 5, 6, 7, 8, 9,10,11,12,	/* 0x80xx */
			13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,	/* 0x90xx */
			29,30,31,32,33,34,35,36,37,38,39,-1,-1,40,41,-1,	/* 0xe0xx */
			-1,-1,-1,-1,-1,-1,-1,-1,42,43,44,45,46,47,-1,-1 	/* 0xf0xx */
		};
		idx = codetbl[hi_byte & 0x3f];
	}
	/*�\�ɂȂ������͖���*/
	if (-1 == idx)
	{
		return;
	}
	//
	idx = (16*10)/*���p�������X�L�b�v*/ + idx*2*192/*codetbl�P������192����*/;
	if (1 == chrwidth(hi_byte)) 	// �Q�o�C�g���p
	{
		idx += lo_byte;
		draw_char(src,x,y,col_n,idx);
	}
	else	/* if ( 2 == chrwidth(hi_byte))*/	// �Q�o�C�g�S�p
	{
		idx += lo_byte*2;
		draw_char(src,x,y,col_n,idx);
		draw_char(src,x+FONTWIDTH,y,col_n,idx+1);
	}
}


/*---------------------------------------------------------
  ������̕`��
---------------------------------------------------------*/
void mh_print(SDL_Surface *src,SDL_Rect *rect,int col,const char *str)
{
	/*
		src 	�������ݐ�
		rect	�������݈ʒu
		col 	�t�H���g�̒n�̐F�̉摜�ԍ�
		str 	�������ޕ���
	*/
	int ch = 0,bef = 0,init_x=rect->x;
	int cw=(int)((rect->w)/FONTWIDTH)+2;
	/*************mh_print_fps�̏�����****************/
	if (count_char) 	/* �J�[�\���ʒu���L������Ă���ꍇ */
	{
		rect->y+=(count_char/cw)*(FONTHEIGHT+2);		/* ���Z(/)�͔��ɒx�� */
		rect->x+=(count_char%cw)*FONTWIDTH; 			/* ���Z(%)�͔��ɒx�� */
		if (rect->x > init_x+rect->w)
		{
			rect->x=init_x;
			rect->y+=FONTHEIGHT+2;
		}
	}
	if (col_sdl!=NULL)	{	unloadbmp_by_surface(col_sdl);	col_sdl=NULL;	}
	//col_sdl=loadbmp(buffer);
	{
		char fn[64];
		sprintf(fn,"%s" "/fonts/" "font_bg%02d.png", moddir, col);		/* �����񏈗�(sprintf)�͔��ɒx�� */
		col_sdl = IMG_Load(fn);
	}
	col_sdl = SDL_DisplayFormat(col_sdl);
	count_i=0;
	/*******************���ۂ̏���********************/
	if (SDL_MUSTLOCK(src))		{	SDL_LockSurface(src);			}	/* ���b�N */
	if (SDL_MUSTLOCK(col_sdl))	{	SDL_LockSurface(col_sdl);		}	/* ���b�N */
	while ((ch = (*str++ & 0xff)) != 0)
	{
		if (ch=='\\')	/* �G�X�P�[�v���� */
		{
			char esc = *str++ & 0xff;
			if (esc =='n')
			{
				count_char+=(int)((rect->w-(rect->x-init_x))/FONTWIDTH)+2;
				rect->x=init_x;
				rect->y+=FONTHEIGHT+2;
			}
		}
		else
		{
			if (bef!=0) 	//2byte�������ꍇ�B���ۂ̏����B
			{
				draw_zenkaku(src,rect->x, rect->y, bef, ch, col_sdl);
				rect->x += FONTWIDTH*chrwidth(bef);
				count_char+=chrwidth(bef);
				bef=0;
			}
			else
			{
				if (iskanji(ch))	//2byte�������ꍇ�B�����ł̓t���O�����B
				{
					bef = ch;
				}
				else
				{
					draw_hankaku(src,rect->x,rect->y,ch,col_sdl);
					rect->x += FONTWIDTH;
					count_char++;
				}
			}
			if (rect->x > init_x+rect->w+FONTWIDTH) 	/*��ʂ��z�������Ȃ�*/
			{
				if (rect->x > init_x+rect->w+2*FONTWIDTH)
				{	rect->x=init_x+FONTWIDTH;}
				else
				{	rect->x=init_x;}
				rect->y+=FONTHEIGHT+2;
			}
		}
	}/*while*/
	if (SDL_MUSTLOCK(src))		{	SDL_UnlockSurface(src); 		}	/* ���b�N���� */
	if (SDL_MUSTLOCK(col_sdl))	{	SDL_UnlockSurface(col_sdl); 	}	/* ���b�N���� */
	unloadbmp_by_surface(col_sdl);
	col_sdl=NULL;
}


/*---------------------------------------------------------
  frame���Ɉꕶ���`��
---------------------------------------------------------*/
static int count_width=0;
int mh_print_fps(SDL_Surface *src,int col, SDL_Rect *rect,const char *str,int wait)
{
	/*
		count_i 	�A�h���X�̃I�t�Z�b�g
		count_width ���ɕ��ׂ��镶���̐�
		col_sdl 	�����̐F
		wait		1�̕����Ɋ|���鎞��
	*/
	static unsigned int ch_wait;
	static int esc_len;
	ch_wait++;
	if (!count_i)		//������
	{
		if (col_sdl!=NULL)	{	unloadbmp_by_surface(col_sdl);	col_sdl=NULL;	}
		count_width=(int)((rect->w)/FONTWIDTH)+2;
		esc_len=0;
		{
			char fn[64];
			sprintf(fn,"%s" "/fonts/" "font_bg%02d.png", moddir, col);
			col_sdl = IMG_Load(fn);
		}
		col_sdl = SDL_DisplayFormat(col_sdl);
	}
	int terminate_this_frame=0;
	if (ch_wait>wait)
	{
		ch_wait=0;
		int ch = 0,bef = 0;
		if ((ch = (*(str+count_i) & 0xff)) != 0)
		{
			if ('\\' == ch) 		/* �G�X�P�[�v�V�[�N�G���X����(escape sequence) */
			{
				char esc = *(str+count_i+1) & 0xff;
				if ('n' == esc) 	/* ���s�R�[�h */
				{
					esc_len+=count_width-(count_i+count_char+esc_len)%count_width-2;
					count_i+=2;
				}
			}
			else					/* �ʏ핶�� */
			{
				int xx;
				int yy;
				xx = rect->x+((count_i+count_char+esc_len)%count_width)*(FONTWIDTH);		/* ���Z(%)�͔��ɒx�� */
				yy = rect->y+((count_i+count_char+esc_len)/count_width)*(FONTHEIGHT+2); 	/* ���Z(/)�͔��ɒx�� */
			//	�p�t�H�[�}���X���ቺ����̂łȂ�ׂ����b�N���Ȃ��B(���b�N�͍ŏ�����)
				#define IS_LOCK_LOCAL_SCREEN (1)
				#if (1==IS_LOCK_LOCAL_SCREEN)
				if (SDL_MUSTLOCK(src))		{	SDL_LockSurface(src);		}	/* ���b�N */
				if (SDL_MUSTLOCK(col_sdl))	{	SDL_LockSurface(col_sdl);	}	/* ���b�N */
				#endif
				if (iskanji(ch))	/* shift jis���� */
				{
					bef = ch;
					ch = (*(str+count_i+1) & 0xff);
					draw_zenkaku(src, xx, yy, bef, ch, col_sdl);
					count_i+=chrwidth(bef);
				}
				else				/* ascii���� */
				{
					draw_hankaku(src, xx, yy, ch, col_sdl);
					count_i++;
				}
				#if (1==IS_LOCK_LOCAL_SCREEN)
				if (SDL_MUSTLOCK(src))		{	SDL_UnlockSurface(src); 		}	/* ���b�N���� */
				if (SDL_MUSTLOCK(col_sdl))	{	SDL_UnlockSurface(col_sdl); 	}	/* ���b�N���� */
				#endif
			}
		//	terminate_this_frame=0; 	/* continue frame */
		}
		else			//�I������
		{
			terminate_this_frame=1; 	/* end frame */
		}
		if (1==terminate_this_frame)	//�I������
		{
			unloadbmp_by_surface(col_sdl);
			col_sdl=NULL;
			count_width=0;
			count_char+=count_i+esc_len;
			count_i=0;
		}
	}
	return terminate_this_frame;		/* 0:continue, 1:end */
}
