/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis 'TREMOR' CODEC SOURCE CODE.   *
 *                                                                  *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis 'TREMOR' SOURCE CODE IS (C) COPYRIGHT 1994-2003    *
 * BY THE Xiph.Org FOUNDATION http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

  function: packing variable sized words into an octet stream

 ********************************************************************/

/* We're 'LSb' endian; if we write a word but read individual bits,
   then we'll read the lsb first */

#include <stdlib.h>
#include <string.h>

#include "misc.h"
#include "tremor/ogg.h"


/* spans forward, skipping as many bytes as headend is negative; if
   headend is zero, simply finds next byte.  If we're up to the end
   of the buffer, leaves headend at zero.  If we've read past the end,
   halt the decode process. */

static void _span(oggpack_buffer *b)
{
	while (b->headend-(b->headbit>>3)<1)
	{
		b->headend -= (b->headbit>>3);
		b->headbit &= 0x7;
		if (b->head->next)
		{
			b->count	+= b->head->length;
			b->head 	=  b->head->next;
			if (b->headend+b->head->length>0)
			{	b->headptr=b->head->buffer->data+b->head->begin-b->headend;}
			b->headend	+= b->head->length;
		}
		else
		{
			/* we've either met the end of decode, or gone past it. halt only if we're past */
			if ((b->headend*8) < b->headbit)
			{
				/* read has fallen off the end */
				b->headend = (-1);
			}
			break;
		}
	}
}

void oggpack_readinit(oggpack_buffer *b,ogg_reference *r)
{
	memset(b,0,sizeof(*b));

	b->tail=b->head=r;
	b->count=0;
	b->headptr=b->head->buffer->data+b->head->begin;
	b->headend=b->head->length;
	_span(b);
}

#define _lookspan() \
	while (!end)\
	{\
		head=head->next;\
		if (!head) return (-1);\
		ptr=head->buffer->data + head->begin;\
		end=head->length;\
	}


//#define USE_SHIFT_TBL (1)
#define USE_SHIFT_TBL (0)
#if (1==USE_SHIFT_TBL)
/* psp�ł͒x���Ȃ�̂ŗv��Ȃ� */
static unsigned long mask[33] =
{
	0x00000000,0x00000001,0x00000003,0x00000007,
	0x0000000f,0x0000001f,0x0000003f,0x0000007f,
	0x000000ff,0x000001ff,0x000003ff,0x000007ff,
	0x00000fff,0x00001fff,0x00003fff,0x00007fff,
	0x0000ffff,0x0001ffff,0x0003ffff,0x0007ffff,
	0x000fffff,0x001fffff,0x003fffff,0x007fffff,
	0x00ffffff,0x01ffffff,0x03ffffff,0x07ffffff,
	0x0fffffff,0x1fffffff,0x3fffffff,0x7fffffff,
	0xffffffff
};
#endif

/* Read in bits without advancing the bitptr; bits <= 32 */
long oggpack_look(oggpack_buffer *b, int bits)
{
	unsigned long ret = (-1);/*???*/
	int read_bits;
	read_bits = bits;	/* �ꌩ������Ɍ����邪�A���̕���psp�͌����I */
//
	bits += b->headbit;
	if (bits >= (b->headend<<3) )
	{
		int 			end 	= b->headend;
		unsigned char	*ptr	= b->headptr;
		ogg_reference	*head	= b->head;

		if (end<0)		{	return (-1);	}

		if (bits)
		{
			_lookspan();
			ret = (*ptr++) >> (b->headbit);
			if (bits>8)
			{
				end--;	_lookspan();
				ret |= (*ptr++) << (8-b->headbit);
				if (bits>16)
				{
					end--;	_lookspan();
					ret |= (*ptr++) << (16-b->headbit);
					if (bits>24)
					{
						end--;	_lookspan();
						ret |= (*ptr++) << (24-b->headbit);
						if (bits>32 && b->headbit)
						{
							end--;	_lookspan();
							ret |= (*ptr) << (32-b->headbit);
						}
					}
				}
			}
		}
	}
	else
	{
		/* make this a switch jump-table */
		ret = b->headptr[0]>>b->headbit;
		if (bits>8)
		{
			ret |= b->headptr[1]<<(8-b->headbit);
			if (bits>16)
			{
				ret |= b->headptr[2]<<(16-b->headbit);
				if (bits>24)
				{
					ret |= b->headptr[3]<<(24-b->headbit);
					if (bits>32 && b->headbit)
					{	ret |= b->headptr[4]<<(32-b->headbit);}
				}
			}
		}
	}
	/* mask returned bits, and return. */
	{
		#if (1==USE_SHIFT_TBL)
		unsigned long ret_mask = mask[read_bits];
		#else
		unsigned long ret_mask = (((0xffffffffu) >> (32-read_bits)));
		#endif
		return ((ret & ret_mask));
	}
}

/* limited to 32 at a time */
void oggpack_adv(oggpack_buffer *b, int bits)
{
	bits += b->headbit;
	b->headbit	= (bits&7);
	b->headptr += (bits>>3);/*/8*/
	b->headend -= (bits>>3);/*/8*/
	if ( b->headend < 1 )
	{
		_span(b);
	}
}

/* bits <= 32 */
long oggpack_read(oggpack_buffer *b,int bits)
{
	long ret = oggpack_look(b,bits);
	oggpack_adv(b,bits);
	return (ret);
}

#if 1
int oggpack_eop(oggpack_buffer *b)
{
	if (b->headend<0)
	{
		return (-1);
	}
	return (0);
}
#endif

#if 0
long oggpack_bytes(oggpack_buffer *b)
{
	if (b->headend<0)
	{
		return (b->count+b->head->length);
	}
	return (b->count + b->head->length-b->headend + ((b->headbit+7)>>3));/*/8*/
}

long oggpack_bits(oggpack_buffer *b)
{
	if (b->headend<0)
	{
		return ((b->count+b->head->length)<<3); 		/*x8*/
	}
	return (((b->count + b->head->length-b->headend)<<3) + (b->headbit));/*x8*/
}
#endif
