
#include "mohou_mikmod_internals.h"

/*---------------------------------------------------------
	ìåï˚ñÕïÌïó Å` Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
	-------------------------------------------------------
	$Id: munitrk.c, v 1.5 2001/12/19 17:11:40 slouken Exp $
	All routines dealing with the manipulation of UNITRK streams
---------------------------------------------------------*/

#include <string.h>


/* Unibuffer chunk size */
#define BUFPAGE  128

/*---------- Reading routines */

static	u8 *rowstart;	/* startadress of a row */
static	u8 *rowend;		/* endaddress of a row (exclusive) */
static	u8 *rowpc;		/* current unimod(tm) programcounter */

u8 UniGetByte(void)
{
	return ((rowpc<rowend)?*(rowpc++):0);
}


void UniSetRow(u8* t)
{
	rowstart = t;
	rowpc	 = rowstart;
	rowend	 = (t?rowstart+(*(rowpc++)&0x1f):t);
}

/* Finds the address of row number 'row' in the UniMod(tm) stream 't' returns
   NULL if the row can't be found. */
u8 *UniFindRow(u8* t, u16 row)
{
	u8 c;
	u8 l;
	if (t)
	{	while (1)
		{
			c = (*t);						/* get rep/len byte */
			if (!c) 	{return (NULL);}	/* zero ? -> end of track.. */
			l = (c>>5)+1;					/* extract repeat value */
			if (l>row)	{break;}			/* reached wanted row? -> return pointer */
			row -= l;						/* haven't reached row yet.. update row */
			t += (c & 0x1f);				/* point t to the next row */
		}
	}
	return (t);
}

/*---------- Writing routines */

static	u8 *unibuf;	/* pointer to the temporary unitrk buffer */
static	u16 unimax;	/* buffer size */

static	u16 unipc;	/* buffer cursor */
static	u16 unitt;	/* current row index */
static	u16 lastp;	/* previous row index */

/* Resets index-pointers to create a new track. */
void UniReset(void)
{
	unitt		= 0;	/* reset index to rep/len byte */
	unipc		= 1;	/* first opcode will be written to index 1 */
	lastp		= 0;	/* no previous row yet */
	unibuf[0]	= 0;	/* clear rep/len byte */
}

/* Expands the buffer */
static MM_BOOL UniExpand(int wanted)
{
	if ((unipc+wanted)>=unimax)
	{
		u8 *newbuf;
		/* Expand the buffer by BUFPAGE bytes */
		newbuf = (u8*)realloc(unibuf, (unimax+BUFPAGE)*sizeof(u8));
		/* Check if realloc succeeded */
		if (newbuf)
		{
			unibuf = newbuf;
			unimax += BUFPAGE;
			return (1);
		}
		else
		{	return (0); 	}
	}
	return (1);
}

/* Appends one byte of data to the current row of a track. */
void UniWriteByte(u8 data)
{
	if (UniExpand(1))
		/* write byte to current position and update */
	{	unibuf[unipc++] = data; 	}
}

void UniWriteWord(u16 data)
{
	if (UniExpand(2))
	{
		unibuf[unipc++] = data>>8;
		unibuf[unipc++] = data&0xff;
	}
}

static MM_BOOL MyCmp(u8* a, u8* b, u16 l)
{
	u16 t;
	for (t=0; t<l; t++)
	{
		if (*(a++)!=*(b++))
		{
			return (0);
		}
	}
	return (1);
}

/* Closes the current row of a unitrk stream (updates the rep/len byte) and sets
   pointers to start a new row. */
void UniNewline(void)
{
	u16 n;
	u16 l;
	u16 len;
	n	= (unibuf[lastp]>>5)+1; 	/* repeat of previous row */
	l	= (unibuf[lastp]&0x1f); 	/* length of previous row */
	len = unipc-unitt;				/* length of current row */
	/* Now, check if the previous and the current row are identical.. when they
	   are, just increase the repeat field of the previous row */
	if (	(n<8) &&
			(len==l) &&
		MyCmp(&unibuf[lastp+1], &unibuf[unitt+1], len-1)
	)
	{
		unibuf[lastp] += 0x20;
		unipc	= unitt+1;
	}
	else
	{
		if (UniExpand(unitt-unipc))
		{
			/* current and previous row aren't equal... update the pointers */
			unibuf[unitt]	= len;
			lastp	= unitt;
			unitt	= unipc++;
		}
	}
}

/* Terminates the current unitrk stream and returns a pointer to a copy of the
   stream. */
u8* UniDup(void)
{
	u8 *d;
	if (!UniExpand(unitt-unipc))	{	return (NULL);}
	unibuf[unitt] = 0;
	d = (u8 *)mod_music_malloc(unipc);
	if (!d) 	{	return (NULL);	}
	memcpy(d, unibuf, unipc);
	return (d);
}

MM_BOOL UniInit(void)
{
	unimax = BUFPAGE;
	unibuf = (u8*)mod_music_malloc(unimax*sizeof(u8));
	if (!unibuf)	{	return (0); 	}
	return (1);
}

void UniCleanup(void)
{
	if (unibuf) free(unibuf);
	unibuf = NULL;
}

/* ex:set ts=4: */
