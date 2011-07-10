/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * scr_printf.c - Debug screen functions.
 *
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 *
 * $Id: scr_printf.c 2017 2006-10-07 16:51:57Z tyranid $
 */
#include <stdio.h>
#include <psptypes.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspge.h>
#include <stdarg.h>
#include <pspdebug.h>
#include <string.h>

#define PSP_SCREEN_WIDTH 480
#define PSP_SCREEN_HEIGHT 272
#define PSP_LINE_SIZE 512

/* baseado nas libs do Duke... */

#if  1//def F_psp_debug_screen_init
void  _psp_debug_screen_clearLine( int Y);
#endif
#if 1
static int X = 0, Y = 0;
static int MX=68, MY=34;
#endif
#if 1
static u32 bg_col = 0;
#endif
#if 1
static u32 fg_col = 0xFFFFFFFF;
#endif
#if 1
static void* g_vram_base = (u32 *) 0x04000000;
#endif
#if 1
static int g_vram_offset = 0;
#endif
#if 1
static int g_vram_mode = PSP_DISPLAY_PIXEL_FORMAT_8888;
#endif
#if 1
static int init = 0;
#endif
#if 1
static u16 convert_8888_to_565(u32 color)
{
	int r, g, b;
	b = (color >> 19) & 0x1F;
	g = (color >> 10) & 0x3F;
	r = (color >> 3) & 0x1F;
	return r | (g << 5) | (b << 11);
}

static u16 convert_8888_to_5551(u32 color)
{
	int r, g, b, a;
	a = (color >> 24) ? 0x8000 : 0;
	b = (color >> 19) & 0x1F;
	g = (color >> 11) & 0x1F;
	r = (color >> 3) & 0x1F;
	return a | r | (g << 5) | (b << 10);
}

static u16 convert_8888_to_4444(u32 color)
{
	int r, g, b, a;
	a = (color >> 28) & 0xF;
	b = (color >> 20) & 0xF;
	g = (color >> 12) & 0xF;
	r = (color >> 4) & 0xF;
	return (a << 12) | r | (g << 4) | (b << 8);
}
#endif
#if 1
static void clear_screen_16(u16 color)
{
	int x;
	u16 *vram = g_vram_base;
	vram += (g_vram_offset >> 1);
	for(x = 0; x < (PSP_LINE_SIZE * PSP_SCREEN_HEIGHT); x++)
	{
		*vram++ = color;
	}
}
#endif
#if 1
static void clear_screen_32(u32 color)
{
	int x;
	u32 *vram = g_vram_base;
	vram += (g_vram_offset>>2);
	for(x = 0; x < (PSP_LINE_SIZE * PSP_SCREEN_HEIGHT); x++)
	{
		*vram++ = color;
	}
}
#endif
#if 1
static void clear_screen(u32 color)
{
	if(g_vram_mode == PSP_DISPLAY_PIXEL_FORMAT_8888)
	{
		clear_screen_32(color);
	}
	else
	{
		u16 c = 0;
		switch(g_vram_mode)
		{
		case PSP_DISPLAY_PIXEL_FORMAT_565: c = convert_8888_to_565(color);
										   break;
		case PSP_DISPLAY_PIXEL_FORMAT_5551: c = convert_8888_to_5551(color);
										   break;
		case PSP_DISPLAY_PIXEL_FORMAT_4444: c = convert_8888_to_4444(color);
										   break;
		};
		clear_screen_16(c);
	}
}
#endif
#if 1
void psp_debug_screen_initEx(void *vram_base, int mode, int setup)
{
	switch(mode)
	{
		case PSP_DISPLAY_PIXEL_FORMAT_565:
		case PSP_DISPLAY_PIXEL_FORMAT_5551:
		case PSP_DISPLAY_PIXEL_FORMAT_4444:
		case PSP_DISPLAY_PIXEL_FORMAT_8888:
			break;
		default: mode = PSP_DISPLAY_PIXEL_FORMAT_8888;
	};

   X = Y = 0;
   /* Place vram in uncached memory */
   if(vram_base == NULL)
   {
	   vram_base = (void*) (0x40000000 | (u32) sceGeEdramGetAddr());
   }
   g_vram_base = vram_base;
   g_vram_offset = 0;
   g_vram_mode = mode;
   if(setup)
   {
	   sceDisplaySetMode(0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT);
	   sceDisplaySetFrameBuf((void *) g_vram_base, PSP_LINE_SIZE, mode, 1);
   }
   clear_screen(bg_col);
   init = 1;
}
void psp_debug_screen_init(void)
{
   X = Y = 0;
   psp_debug_screen_initEx(NULL, PSP_DISPLAY_PIXEL_FORMAT_8888, 1);
}
#endif
#if 0
void psp_debug_screenSetBackColor(u32 colour)
{
   bg_col = colour;
}

void psp_debug_screenSetTextColor(u32 colour)
{
   fg_col = colour;
}

void psp_debug_screenSetColorMode(int mode)
{
	switch(mode)
	{
		case PSP_DISPLAY_PIXEL_FORMAT_565:
		case PSP_DISPLAY_PIXEL_FORMAT_5551:
		case PSP_DISPLAY_PIXEL_FORMAT_4444:
		case PSP_DISPLAY_PIXEL_FORMAT_8888:
			break;
		default: mode = PSP_DISPLAY_PIXEL_FORMAT_8888;
	};

	g_vram_mode = mode;
}

int psp_debug_screenGetX(void)
{
	return X;
}

int psp_debug_screenGetY(void)
{
	return Y;
}
#endif
#if 1
void psp_debug_screen_set_xy(int x, int y)
{
	if( x<MX && x>=0 ) X=x;
	if( y<MY && y>=0 ) Y=y;
}

void psp_debug_screen_clear(void)
{
	int y;

	if(!init)
	{
		return;
	}

	for(y=0;y<MY;y++)
		_psp_debug_screen_clearLine(y);
	psp_debug_screen_set_xy(0,0);
	clear_screen(bg_col);
}
#endif
#if 0
void psp_debug_screenSetOffset(int offset)
{
	g_vram_offset = offset;
}

void psp_debug_screenSetBase(u32* base)
{
	g_vram_base = base;
}
#endif
#if 1
extern u8 msx8[];
#endif
#if 1
static void debug_put_char_32(int x, int y, u32 color, u32 bgc, u8 ch)
{
   int	i,j, l;
   u8	*font;
   u32	pixel;
   u32 *vram_ptr;
   u32 *vram;

   if(!init)
   {
	   return;
   }

   vram = g_vram_base;
   vram += (g_vram_offset >> 2) + x;
   vram += (y * PSP_LINE_SIZE);

   font = &msx8[ (int)ch * 8];
   for (i=l=0; i < 8; i++, l+= 8, font++)
   {
	  vram_ptr	= vram;
	  for (j=0; j < 8; j++)
	{
		  if ((*font & (128 >> j)))
			  pixel = color;
		  else
			  pixel = bgc;

		  *vram_ptr++ = pixel;
	}
	  vram += PSP_LINE_SIZE;
   }
}
#endif
#if 1
static void debug_put_char_16(int x, int y, u16 color, u16 bgc, u8 ch)
{
   int	i,j, l;
   u8	*font;
   u16	pixel;
   u16 *vram_ptr;
   u16 *vram;

   if(!init)
   {
	   return;
   }

   vram = g_vram_base;
   vram += (g_vram_offset >> 1) + x;
   vram += (y * PSP_LINE_SIZE);

   font = &msx8[ (int)ch * 8];
   for (i=l=0; i < 8; i++, l+= 8, font++)
   {
	  vram_ptr	= vram;
	  for (j=0; j < 8; j++)
	{
		  if ((*font & (128 >> j)))
			  pixel = color;
		  else
			  pixel = bgc;

		  *vram_ptr++ = pixel;
	}
	  vram += PSP_LINE_SIZE;
   }
}
#endif
#if 1
void psp_debug_screenPutChar( int x, int y, u32 color, u8 ch)
{
	if(g_vram_mode == PSP_DISPLAY_PIXEL_FORMAT_8888)
	{
		debug_put_char_32(x, y, color, bg_col, ch);
	}
	else
	{
		u16 c = 0;
		u16 b = 0;
		switch(g_vram_mode)
		{
		case PSP_DISPLAY_PIXEL_FORMAT_565:
			c = convert_8888_to_565(color);
			b = convert_8888_to_565(bg_col);
			break;
		case PSP_DISPLAY_PIXEL_FORMAT_5551:
			c = convert_8888_to_5551(color);
			b = convert_8888_to_5551(bg_col);
			break;
		case PSP_DISPLAY_PIXEL_FORMAT_4444:
			c = convert_8888_to_4444(color);
			b = convert_8888_to_4444(bg_col);
			break;
		};
		debug_put_char_16(x, y, c, b, ch);
	}
}
#endif
#if 1
void  _psp_debug_screen_clearLine( int Y)
{
	int i;
	for (i=0; i < MX; i++)
	{	psp_debug_screenPutChar( i*7 , Y * 8, bg_col, 219);
	}
}
#endif

#if 1
/* Print non-nul terminated strings */
int psp_debug_screenPrintData(const char *buff, int size)
{
	int i;
	int j;
	char c;

	if(!init)
	{
		return 0;
	}

	for (i = 0; i < size; i++)
	{
		c = buff[i];
		switch (c)
		{
		case '\n':
					X = 0;
					Y ++;
					if (Y == MY)
						Y = 0;
					_psp_debug_screen_clearLine(Y);
					break;
		case '\t':
					for (j = 0; j < 5; j++) {
						psp_debug_screenPutChar( X*7 , Y * 8, fg_col, ' ');
						X++;
					}
					break;
		default:
					psp_debug_screenPutChar( X*7 , Y * 8, fg_col, c);
					X++;
					if (X == MX)
					{
						X = 0;
						Y++;
						if (Y == MY)
							Y = 0;
						_psp_debug_screen_clearLine(Y);
					}
		}
	}
	return i;
}
#endif
#if 0
int psp_debug_screenPuts(const char *str)
{
	return psp_debug_screenPrintData(str, strlen(str));
}
#endif

#if 1//def F_psp_debug_screen_printf
void psp_debug_screen_printf(const char *format, ...)
{
	va_list 	opt;
	char		buff[2048];
	int 		bufsz;
	va_start(opt, format);
	bufsz = vsnprintf( buff, (size_t) sizeof(buff), format, opt);
	(void) psp_debug_screenPrintData(buff, bufsz);
}
#endif

