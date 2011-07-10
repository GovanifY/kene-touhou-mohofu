
#include "mohou_mikmod_internals.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
	-------------------------------------------------------
	$Id: sloader.c, v 1.3 1999/12/28 18:51:11 hercules Exp $
	Routines for loading samples. The sample loader utilizes the routines
	provided by the "registered" sample loader.
---------------------------------------------------------*/

#if 0
//
	static s16 mod_music_file_read_s16BE_sample_FILE_reader(MF_READER *reader);
	static s16 mod_music_file_read_s16LE_sample_FILE_reader(MF_READER *reader);
	static u16 mod_music_file_read_u16BE_sample_FILE_reader(MF_READER *reader);
	static u16 mod_music_file_read_u16LE_sample_FILE_reader(MF_READER *reader);

//	static s32 mod_music_file_read_s32BE_sample_FILE_reader(MF_READER *reader);
//	static s32 mod_music_file_read_s32LE_sample_FILE_reader(MF_READER *reader);
//	static u32 mod_music_file_read_u32BE_sample_FILE_reader(MF_READER *reader);
//	static u32 mod_music_file_read_u32LE_sample_FILE_reader(MF_READER *reader);

	static int mod_music_file_read_multi_s16BE_sample_FILE_reader(MF_READER *reader, s16 *buffer, int number);
	static int mod_music_file_read_multi_s16LE_sample_FILE_reader(MF_READER *reader, s16 *buffer, int number);
//	static int mod_music_file_read_multi_u16BE_sample_FILE_reader(MF_READER *reader, u16 *buffer, int number);
//	static int mod_music_file_read_multi_u16LE_sample_FILE_reader(MF_READER *reader, u16 *buffer, int number);

//	static int mod_music_file_read_multi_s32BE_sample_FILE_reader(MF_READER *reader, s32 *buffer, int number);
//	static int mod_music_file_read_multi_s32LE_sample_FILE_reader(MF_READER *reader, s32 *buffer, int number);
//	static int mod_music_file_read_multi_u32BE_sample_FILE_reader(MF_READER *reader, u32 *buffer, int number);
//	static int mod_music_file_read_multi_u32LE_sample_FILE_reader(MF_READER *reader, u32 *buffer, int number);
#endif

////////////////////////

static u16 mod_music_file_read_u16BE_sample_FILE_reader(MF_READER* reader)
{
	u16 result = (((u16)(u8)(reader->Get(reader)))<<8);
	result |= (u8)(reader->Get(reader));
	return (result);
}

static u16 mod_music_file_read_u16LE_sample_FILE_reader(MF_READER* reader)
{
	u16 result = (u8)(reader->Get(reader));
	result |= (((u16)(u8)(reader->Get(reader)))<<8);
	return (result);
}
//

static s16 mod_music_file_read_s16BE_sample_FILE_reader(MF_READER* reader)
{
	return ((s16)mod_music_file_read_u16BE_sample_FILE_reader(reader));
}

static s16 mod_music_file_read_s16LE_sample_FILE_reader(MF_READER* reader)
{
	return ((s16)mod_music_file_read_u16LE_sample_FILE_reader(reader));
}


///////////////////////


#ifdef __STDC__
#define DEFINE_MULTIPLE_READ_FUNCTION_SAMPLE(type_name, type)										\
static int mod_music_file_read_multi_##type_name##_sample_FILE_reader(MF_READER* reader, type *buffer, int number)		\
{																							\
	while (number-->0)																		\
	{	*(buffer++) = mod_music_file_read_##type_name##_sample_FILE_reader(reader); 	}				\
	return (!reader->Eof(reader));	\
}
#else
#define DEFINE_MULTIPLE_READ_FUNCTION_SAMPLE(type_name, type)										\
static int mod_music_file_read_multi_/**/type_name/**/_sample_FILE_reader/**/_sample_FILE_reader(MF_READER* reader, type *buffer, int number)	\
{																							\
	while (number-->0)																		\
	{	*(buffer++) = mod_music_file_read_/**/type_name(reader);		}		\
	return (!reader->Eof(reader));	\
}
#endif

	DEFINE_MULTIPLE_READ_FUNCTION_SAMPLE(s16BE, s16)
//	DEFINE_MULTIPLE_READ_FUNCTION_SAMPLE(u16BE, u16)
	DEFINE_MULTIPLE_READ_FUNCTION_SAMPLE(s16LE, s16)
//	DEFINE_MULTIPLE_READ_FUNCTION_SAMPLE(u16LE, u16)

//	DEFINE_MULTIPLE_READ_FUNCTION_SAMPLE(s32BE, s32)
//	DEFINE_MULTIPLE_READ_FUNCTION_SAMPLE(u32BE, u32)
//	DEFINE_MULTIPLE_READ_FUNCTION_SAMPLE(s32LE, s32)
//	DEFINE_MULTIPLE_READ_FUNCTION_SAMPLE(u32LE, u32)

///////////////////////

static	int sl_rlength;
static	s16 sl_old;
static	s16 *sl_buffer = NULL;
static	MOD_MUSIC_INTERNAL_SAMPLE_LOAD *musiclist = NULL;
static	MOD_MUSIC_INTERNAL_SAMPLE_LOAD *sndfxlist = NULL;

/* size of the loader buffer in words */
#define SLBUFSIZE 2048

/* IT-Compressed status structure */
typedef struct ITPACK
{
	u16 bits;	/* current number of bits */
	u16 bufbits; /* bits in buffer */
	s16 last;	/* last output */
	u8 buf;		/* bit buffer */
} ITPACK;

MM_BOOL SL_Init(MOD_MUSIC_INTERNAL_SAMPLE_LOAD* s)
{
	if (!sl_buffer)
	{
		sl_buffer = mod_music_malloc(SLBUFSIZE*sizeof(s16));
		if (!sl_buffer)
		{
			/*(error!)*/
			return (0);
		}
	}
	sl_rlength = s->length;
	if (s->infmt & SF_16BITS)	{sl_rlength>>=1;}
	sl_old = 0;
	return (1);
}

void SL_Exit(MOD_MUSIC_INTERNAL_SAMPLE_LOAD *s)
{
	if (sl_rlength>0)	{	mod_music_file_seek_cur(s->sample_FILE_reader, sl_rlength);}
	if (sl_buffer)
	{
		free(sl_buffer);
		sl_buffer = NULL;
	}
}

/* unpack a 8bit IT packed sample */
static MM_BOOL read_itcompr8(MF_READER *reader, ITPACK* status, s16 *sl_buffer, u16 count, u16* incnt)
{
	s16 *dest	= sl_buffer, *end=sl_buffer+count;
	u16 x, y, needbits, havebits, new_count=0;
	u16 bits 	= status->bits;
	u16 bufbits	= status->bufbits;
	s8 last		= status->last;
	u8 buf		= status->buf;

	while (dest<end)
	{
		needbits = new_count?3:bits;
		x = havebits = 0;
		while (needbits)
		{
			/* feed buffer */
			if (!bufbits)
			{
				if ((*incnt)--)
				{	buf = (u8)(reader->Get(reader));}
				else
				{	buf = 0;}
				bufbits = 8;
			}
			/* get as many bits as necessary */
			y	 = needbits<bufbits?needbits:bufbits;
			x	|= (buf & ((1<<y)- 1))<<havebits;
			buf 	>>= y;
			bufbits 	-= y;
			needbits	-= y;
			havebits	+= y;
		}
		if (new_count)
		{
			new_count = 0;
			if (++x >= bits)
			{	x++;}
			bits = x;
			continue;
		}
		if (bits<7)
		{
			if (x==(1<<(bits-1)))
			{
				new_count = 1;
				continue;
			}
		}
		else
		if (bits<9)
		{
			y = (0xff >> (9-bits)) - 4;
			if ((x>y)&&(x<=y+8))
			{
				if ((x-=y)>=bits)
				{	x++;}
				bits = x;
				continue;
			}
		}
		else
		if (bits<10)
		{
			if (x>=0x100)
			{
				bits=x-0x100+1;
				continue;
			}
		}
		else
		{
			/* error in compressed data... */
			mod_music_error_number = MOD_MUSIC_ERROR_ITPACK_INVALID_DATA;
			return (0);
		}

		if (bits<8) /* extend sign */
		{	x = ((s8)(x <<(8-bits))) >> (8-bits);}
		*(dest++)= (last+=x) << 8; /* convert to 16 bit */
	}
	status->bits	= bits;
	status->bufbits = bufbits;
	status->last	= last;
	status->buf 	= buf;
	return dest-sl_buffer;
}

/* unpack a 16bit IT packed sample */
static MM_BOOL read_itcompr16(MF_READER *reader, ITPACK *status, s16 *sl_buffer, u16 count, u16* incnt)
{
	s16 *dest	= sl_buffer, *end=sl_buffer+count;
	s32 x, y, needbits, havebits, new_count=0;
	u16 bits 	= status->bits;
	u16 bufbits	= status->bufbits;
	s16 last 	= status->last;
	u8 buf		= status->buf;

	while (dest<end)
	{
		needbits = new_count?4:bits;
		x = havebits = 0;
		while (needbits)
		{
			/* feed buffer */
			if (!bufbits)
			{
				if ((*incnt)--)
				{	buf = (u8)(reader->Get(reader));}
				else
				{	buf = 0;}
				bufbits = 8;
			}
			/* get as many bits as necessary */
			y	= needbits<bufbits?needbits:bufbits;
			x	|= (buf &((1<<y)-1))<<havebits;
			buf 		>>= y;
			bufbits 	-= y;
			needbits	-= y;
			havebits	+= y;
		}
		if (new_count)
		{
			new_count = 0;
			if (++x >= bits)
			{	x++;}
			bits = x;
			continue;
		}
		if (bits<7)
		{
			if (x==(1<<(bits-1)))
			{
				new_count = 1;
				continue;
			}
		}
		else
		if (bits<17)
		{
			y = (0xffff>>(17-bits))-8;
			if ((x>y)&&(x<=y+16))
			{
				if ((x-=y)>=bits)
				{	x++;}
				bits = x;
				continue;
			}
		}
		else
		if (bits<18)
		{
			if (x>=0x10000)
			{
				bits = x-0x10000+1;
				continue;
			}
		}
		else
		{
			 /* error in compressed data... */
			mod_music_error_number = MOD_MUSIC_ERROR_ITPACK_INVALID_DATA;
			return (0);
		}

		if (bits<16) /* extend sign */
		{	x = ((s16)(x<<(16-bits)))>>(16-bits);}
		*(dest++)	= (last+=x);
	}
	status->bits		= bits;
	status->bufbits 	= bufbits;
	status->last		= last;
	status->buf 		= buf;
	return (dest-sl_buffer);
}

static MM_BOOL SL_LoadInternal(void* buffer, u16 infmt, u16 outfmt, int scalefactor, u32 length, MF_READER* sample_FILE_reader, MM_BOOL dither)
{
	s8 *bptr 	= (s8*)buffer;
	s16 *wptr	= (s16*)buffer;
	int stodo, t, u;

	int result, c_block=0;	/* compression bytes until next block */
	ITPACK status;
	u16 incnt;
	incnt	= 0;

	status.bits 	= 9;/*???*/
	status.bufbits	= 0;
	status.last 	= 0;
	status.buf		= 0;/*???*/

	while (length)
	{
		stodo = (length<SLBUFSIZE)?length:SLBUFSIZE;

		if (infmt&SF_ITPACKED)
		{
			sl_rlength=0;
			if (!c_block)
			{
				status.bits = (infmt & SF_16BITS) ? 17 : 9;
				status.last = status.bufbits = 0;
				incnt = mod_music_file_read_u16LE_sample_FILE_reader(sample_FILE_reader);
				c_block = (infmt & SF_16BITS) ? 0x4000 : 0x8000;
				if (infmt&SF_DELTA) 	{sl_old = 0;}
			}
			if (infmt & SF_16BITS)
			{	result = read_itcompr16(sample_FILE_reader, &status, sl_buffer, stodo, &incnt);
				if (!(result))
				{	return (1);}
			}
			else
			{	result = read_itcompr8(sample_FILE_reader, &status, sl_buffer, stodo, &incnt);
				if (!(result))
				{	return (1);}
			}
			if (result!=stodo)
			{
				mod_music_error_number = MOD_MUSIC_ERROR_ITPACK_INVALID_DATA;
				return (1);
			}
			c_block -= stodo;
		}
		else
		{
			if (infmt&SF_16BITS)
			{
				if (infmt&SF_BIG_ENDIAN)
				{	mod_music_file_read_multi_s16BE_sample_FILE_reader(sample_FILE_reader, sl_buffer, stodo);}
				else
				{	mod_music_file_read_multi_s16LE_sample_FILE_reader(sample_FILE_reader, sl_buffer, stodo);}
			}
			else
			{
				s8 *src;
				s16 *dest;

				sample_FILE_reader->Read(sample_FILE_reader, sl_buffer, sizeof(s8)*stodo);
				src = (s8*)sl_buffer;
				dest  = sl_buffer;
				src += stodo;dest += stodo;

				for (t=0; t<stodo; t++)
				{
					src--; dest--;
					*dest = (*src)<<8;
				}
			}
			sl_rlength-=stodo;
		}

		if (infmt & SF_DELTA)
		{	for (t=0; t<stodo; t++)
			{
				sl_buffer[t] += sl_old;
				sl_old = sl_buffer[t];
			}
		}
		if ((infmt^outfmt) & SF_SIGNED)
		{	for (t=0; t<stodo; t++)
			{	sl_buffer[t]^= 0x8000;
			}
		}
		if (scalefactor)
		{
			int idx = 0;
			s32 scaleval;
			/* Sample Scaling... average values for better results. */
			t= 0;
			while (t<stodo && length)
			{
				scaleval = 0;
				for (u=scalefactor;u && t<stodo;u--, t++)
				{	scaleval += sl_buffer[t];}
				sl_buffer[idx++] = scaleval / (scalefactor-u);
				length--;
			}
			stodo = idx;
		}
		else
		{	length -= stodo;}

		if (dither)
		{
			if ((infmt & SF_STEREO) && !(outfmt & SF_STEREO))
			{
				/* dither stereo to mono, average together every two samples */
				s32 avgval;
				int idx = 0;
				t = 0;
				while (t<stodo && length)
				{
					avgval = sl_buffer[t++];
					avgval += sl_buffer[t++];
					sl_buffer[idx++] = avgval>>1;
					length -= 2;
				}
				stodo = idx;
			}
		}

		if (outfmt & SF_16BITS)
		{
			for (t=0; t<stodo; t++)
			{	*(wptr++) = sl_buffer[t];}
		}
		else
		{
			for (t=0; t<stodo; t++)
			{	*(bptr++) = sl_buffer[t]>>8;}
		}
	}
	return (0);
}

MM_BOOL SL_Load(void* buffer, MOD_MUSIC_INTERNAL_SAMPLE_LOAD *smp, u32 length)
{
	return SL_LoadInternal(
		buffer, smp->infmt, smp->outfmt, smp->scalefactor,
		length, smp->sample_FILE_reader, 0);
}

/* Registers a sample for loading when SL_LoadSamples() is called. */
MOD_MUSIC_INTERNAL_SAMPLE_LOAD* SL_RegisterSample(MF_READER* reader, MM_SAMPLE* s, int type)
{
	MOD_MUSIC_INTERNAL_SAMPLE_LOAD *news, **samplist, *cruise;

	if (type==MD_MUSIC)
	{
		samplist	= &musiclist;
		cruise		= musiclist;
	}
	else if (type==MD_SNDFX)
	{
		samplist	= &sndfxlist;
		cruise		= sndfxlist;
	}
	else
	{
		return (NULL);
	}
	/* Allocate and add structure to the END of the list */
	news = (MOD_MUSIC_INTERNAL_SAMPLE_LOAD*)mod_music_malloc(sizeof(MOD_MUSIC_INTERNAL_SAMPLE_LOAD));
	if (!news)
	{
		/*(error!)*/
		return (NULL);
	}
	if (cruise)
	{
		while (cruise->next)
		{	cruise		= cruise->next; 	}
		cruise->next	= news;
	}
	else
	{
		*samplist = news;
	}
	news->infmt 		= s->flags & SF_FORMATMASK;
	news->outfmt		= news->infmt;
	news->sample_FILE_reader	= reader;
	news->sample		= s;
	news->length		= s->length;
	news->loopstart 	= s->loopstart;
	news->loopend		= s->loopend;

	return (news);
}

static void FreeSampleList(MOD_MUSIC_INTERNAL_SAMPLE_LOAD* s)
{
	MOD_MUSIC_INTERNAL_SAMPLE_LOAD *old;
	while (s)
	{
		old = s;
		s = s->next;
		free(old);
	}
}

#if 1
static u32 /*VC1_SampleLength*/s_MOD_VIRTUAL_MIXER_SampleLength_soft_ware(/*int type,*/ MM_SAMPLE* s)
{
//	if (!s) 	{return (0);}
	return (s->length*((s->flags&SF_16BITS)?2:1))+16;
}
/* Returns the total amount of memory required by the samplelist queue. */
static u32 SampleTotal(MOD_MUSIC_INTERNAL_SAMPLE_LOAD* samplist/*, int type*/)
{
	int total = 0;
	while (samplist)
	{
		samplist->sample->flags = (samplist->sample->flags&~SF_FORMATMASK)|samplist->outfmt;
		if (samplist->sample)
		{
			total += s_MOD_VIRTUAL_MIXER_SampleLength_soft_ware(/*type,*/ (MM_SAMPLE*)samplist->sample);
		}
		samplist = samplist->next;
	}
	return (total);
}
#endif

static u32 RealSpeed(MOD_MUSIC_INTERNAL_SAMPLE_LOAD *s)
{
	return (s->sample->speed / (s->scalefactor?s->scalefactor:1));
}

/*global*/ void SL_SampleSigned(MOD_MUSIC_INTERNAL_SAMPLE_LOAD* s)/*virch32.c*/
{
	s->outfmt |= SF_SIGNED;
	s->sample->flags = (s->sample->flags&~SF_FORMATMASK) | s->outfmt;
}
#if 0
static void s_SL_SampleUnsigned(MOD_MUSIC_INTERNAL_SAMPLE_LOAD* s)
{
	s->outfmt &= ~SF_SIGNED;
	s->sample->flags = (s->sample->flags&~SF_FORMATMASK) | s->outfmt;
}
#endif

/*global*/ void SL_Sample8to16(MOD_MUSIC_INTERNAL_SAMPLE_LOAD* s)/*virch32.c*/
{
	s->outfmt |= SF_16BITS;
	s->sample->flags = (s->sample->flags&~SF_FORMATMASK) | s->outfmt;
}

static void s_SL_Sample16to8(MOD_MUSIC_INTERNAL_SAMPLE_LOAD* s)
{
	s->outfmt &= ~SF_16BITS;
	s->sample->flags = (s->sample->flags&~SF_FORMATMASK) | s->outfmt;
}

static void s_SL_HalveSample(MOD_MUSIC_INTERNAL_SAMPLE_LOAD* s, int factor)
{
	s->scalefactor = (0<factor) ? factor:2;

	s->sample->divfactor	= s->scalefactor;
	s->sample->length		= s->length / s->scalefactor;
	s->sample->loopstart	= s->loopstart / s->scalefactor;
	s->sample->loopend		= s->loopend / s->scalefactor;
}
#if (1)
//extern s16 MD_SampleLoad(/*struct*/ MOD_MUSIC_INTERNAL_SAMPLE_LOAD*, int);
static s16 MD_SampleLoad_soft_ware(/*struct*/ MOD_MUSIC_INTERNAL_SAMPLE_LOAD* s)//, int type)/* sloader.c */
{
//	CHECK_TYPE_AAA;
	s16 result;
	SL_Init(s);
	result = MOD_VIRTUAL_MIXER_SampleLoad_soft_ware(s);/*(たぶん)*/ 	//, MD_SOFTWARE/*type*/
	SL_Exit(s);
	return (result);
}
#endif/*(1)*/


//static	long vc_me mory = 0;
//u32 /*VC1_SampleSpace*/MOD_VIRTUAL_MIXER_SampleSpace_soft_ware(void/*int type*/)
//{
//	return (vc_me mory);
//}
static MM_BOOL DitherSamples(MOD_MUSIC_INTERNAL_SAMPLE_LOAD* samplist/*, int type*/)
{
	MOD_MUSIC_INTERNAL_SAMPLE_LOAD *c2smp = NULL;
	u32 maxsize, speed;
	MOD_MUSIC_INTERNAL_SAMPLE_LOAD *s;

	if (!samplist) return (0);
	maxsize = 0;//MOD_VIRTUAL_MIXER_SampleSpace_soft_ware(/*type*/)*(1024);/*(たぶん)*/
	if (maxsize)
	{	while (SampleTotal(samplist/*, type*/)>maxsize)
		{
			/* First Pass - check for any 16 bit samples */
			s = samplist;
			while (s)
			{
				if (s->outfmt & SF_16BITS)
				{
					s_SL_Sample16to8(s);
					break;
				}
				s = s->next;
			}
			/* Second pass (if no 16bits found above) is to take the sample with
			   the highest speed and dither it by half. */
			if (!s)
			{
				s = samplist;
				speed = 0;
				while (s)
				{
					if ((s->sample->length) && (RealSpeed(s)>speed))
					{
						speed = RealSpeed(s);
						c2smp = s;
					}
					s = s->next;
				}
				if (c2smp)
				{	s_SL_HalveSample(c2smp, 2);}
			}
		}
	}

	/* Samples dithered, now load them ! */
	s = samplist;
	while (s)
	{
		/* sample has to be loaded ? -> increase number of samples, allocate
		   memory and load sample. */
		if (s->sample->length)
		{
			if (s->sample->seekpos)
			{	mod_music_file_seek_set(s->sample_FILE_reader, s->sample->seekpos);}
			/* Call the sample load routine of the driver module. It has to
			   return a 'handle' (>=0) that identifies the sample. */
			s->sample->handle = MD_SampleLoad_soft_ware(s/*, type*/);
			s->sample->flags  = (s->sample->flags & ~SF_FORMATMASK) | s->outfmt;
			if (s->sample->handle<0)
			{
				FreeSampleList(samplist);
				if (mod_music_error_handler) mod_music_error_handler();
				return (1);
			}
		}
		s = s->next;
	}
	FreeSampleList(samplist);
	return (0);
}

MM_BOOL SL_LoadSamples(void)
{
	mod_music_critical = 0;
	if ((!musiclist)&&(!sndfxlist)) return (0);
	MM_BOOL ok;
	ok = DitherSamples(musiclist/*, MD_MUSIC*/)||DitherSamples(sndfxlist/*, MD_SNDFX*/);
	musiclist = sndfxlist = NULL;
	return (ok);
}

/* ex:set ts=4: */
