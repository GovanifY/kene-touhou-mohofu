
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	"*.MOD"�`���̃t�@�C����ǂݍ���ŁA���j�o�[�T���`��(MikMOD�̍Đ��`��)�ɕϊ����܂��B
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
	-------------------------------------------------------
	���̃t�@�C���� "mlreg.c" ����C���N���[�h����܂��B
	-------------------------------------------------------
	$Id: load_mod.c,v 1.5 2001/09/11 18:49:18 slouken Exp $
	Generic MOD loader (Protracker, StarTracker, FastTracker, etc)
	$Id: npertab.c,v 1.3 1999/12/28 18:51:11 hercules Exp $
	MOD format period table.	Used by both the MOD and M15 (15-inst mod) Loaders.
---------------------------------------------------------*/

//#ifdef HA VE_CONFIG_H
//#include "co nfig.h"
//#endif
//
//#ifdef HA VE_UNISTD_H
//#include <un istd.h>
//#endif

//#include <ctype.h>/*isdi git*/
#include <string.h>

#include "mohou_mikmod_internals.h"

/*---------- Module structure */

typedef struct MSAMPINFO
{
	MM_s8 samplename[23]; 	/* 22 in module, 23 in memory */
	u16 length;
	u8 mm_fine_tune;
	u8 volume;
	u16 reppos;
	u16 replen;
} MSAMPINFO;

typedef struct MOD_MUSIC_MODULEHEADER
{
	MM_s8 songname[21];		/* the songname.. 20 in module, 21 in memory */
	MSAMPINFO samples[31];		/* all sampleinfo */
	u8 songlength;			/* number of patterns used */
	u8 magic1;				/* should be 127 */
	u8 positions[128];		/* which pattern to play at pos */
	u8 magic2[4];			/* string "M.K." or "FLT4" or "FLT8" */
} MOD_MUSIC_MODULEHEADER;

typedef struct MODTYPE
{
	MM_s8 id[5];
	u8 channels;
	MM_s8 *name;
} MODTYPE;

typedef struct MODNOTE
{
	u8 a, b, c, d;
} MODNOTE;

/*---------- Loader variables */

#define MOD_MUSIC_MODULEHEADERSIZE 0x438


static MOD_MUSIC_MODULEHEADER *mh_mod = NULL;
static MODNOTE *patbuf = NULL;
static int mod_mod_type = 0;

/*---------- Loader code */

/*---------------------------------------------------------
	�q�֐�
	Get ascii a interger number.
---------------------------------------------------------*/
#if (1)
static int my_is_digit(char *ccc)
{
	#if 0
	if (
		(isdi git((int)((char)(*ccc))))	/* gcc 4.3.5 */
		||
		(((char)'-')==(*ccc))
	)
	{
		return (1/*TRUE*/);
	}/*ok!==*/
	//else
	{
		return (0/*FALSE*/);/*NG!,*/
	}/*(ng!)*/
	#else
	if ( ('9') < (*ccc) )
	{
		goto my_false;/*NG!,*/
	}
	//else
	if ( ('0') > (*ccc) )
	{
		/*('-'�̉\��������B)*/
	//	if ('-'!=(*ccc))
		{
			goto my_false;
		}
	}
//	my_ok:
		return (1/*TRUE*/);
	my_false:
		return (0/*FALSE*/);/*NG!,*/
	#endif
}
#endif
/*---------------------------------------------------------
	ID����ǂ̌`�������f���܂��B

---------------------------------------------------------*/

/* given the module ID, determine the number of channels and the tracker
   description ; also alters mod_mod_type */
//�p�~		static const MM_s8 orpheus[]		= "Imago Orpheus (MOD format)"; 	/* ���̌`�������w�b�_����ȒP�ɂ͔���ł��Ȃ��B */

static MM_BOOL MOD_CheckType(u8 *id, u8 *numchn/*, MM_s8 **descr*/)
{
//�p�~		static const MM_s8 protracker[]	= "Protracker";
//�p�~		static const MM_s8 startrekker[]	= "Startrekker";
//�p�~		static const MM_s8 fasttracker[]	= "Fasttracker";
//�p�~		static const MM_s8 oktalyser[]	= "Oktalyser";
//�p�~		static const MM_s8 oktalyzer[]	= "Oktalyzer";
//�p�~		static const MM_s8 taketracker[]	= "TakeTracker";

	/* Protracker and variants */
	if (	(!memcmp(id, "M.K.", 4)) ||
			(!memcmp(id, "M!K!", 4)))
	{
	//�p�~		*descr			= protracker;
		mod_mod_type	= 0;
		*numchn 		= 4;
		return (1);
	}
	/* Star Tracker */
	if ((	(!memcmp(id, "FLT", 3)) ||
			(!memcmp(id, "EXO", 3))) &&
			(my_is_digit(&id[3])))
	{
	//�p�~		*descr			= startrekker;
		mod_mod_type	= 1;
		*numchn 		= id[3] - '0';
		if (*numchn == 4)
		{	return (1);
		}
		#ifdef MIKMOD_DEBUG
		if (*numchn == 8)
		{	error(0/*ERR_FATAL*/, "FLT8 modules not supported yet\n");		}
		else
		{	error(0/*ERR_FATAL*/, "Unknown FLT%d module type\n", *numchn);		}
		#endif
		return (0);
	}
	/* Oktalyzer (Amiga) */
	if (!memcmp(id, "OKTA", 4))
	{
	//�p�~		*descr			= oktalyzer;
		mod_mod_type	= 1;
		*numchn 		= 8;
		return (1);
	}
	/* Oktalyser (Atari) */
	if (!memcmp(id, "CD81", 4))
	{
	//�p�~		*descr			= oktalyser;
		mod_mod_type	= 1;
		*numchn 		= 8;
		return (1);
	}
	/* Fasttracker */
	if ((!memcmp(id + 1, "CHN", 3)) && (my_is_digit(&id[0])))
	{
	//�p�~		*descr			= fasttracker;
		mod_mod_type	= 1;
		*numchn 		= id[0] - '0';
		return (1);
	}
	/* Fasttracker or Taketracker */
	if ((	(!memcmp(id + 2, "CH", 2)) ||
		(!memcmp(id + 2, "CN", 2))) && (my_is_digit(&id[0])) && (my_is_digit(&id[1])))
	{
		if (id[3] == 'H')
		{
		//�p�~		*descr			= fasttracker;
			mod_mod_type	= 2;		/* this can also be Imago Orpheus */
		}
		else
		{
		//�p�~		*descr			= taketracker;
			mod_mod_type	= 1;
		}
		*numchn 			= ((id[0] - '0') * 10) + (id[1] - '0');
		return (1);
	}
	return (0);
}

static MM_BOOL MOD_Test(void)
{
	u8 id[4], numchn;
	mod_music_file_seek_set(mmff.mod_FILE_reader, MOD_MUSIC_MODULEHEADERSIZE);
	mod_music_file_read_multi_u8(mmff.mod_FILE_reader, id, 4);
//	if (0==)	{	return (0);	}
//	MM_s8 *descr;
	if (MOD_CheckType(id, &numchn/*, &descr*/))
	{	return (1);
	}
	return (0);
}

static MM_BOOL MOD_Init(void)
{
	if (!(mh_mod = (MOD_MUSIC_MODULEHEADER *)mod_music_malloc(sizeof(MOD_MUSIC_MODULEHEADER))))
		return (0);
	return (1);
}

static void MOD_Cleanup(void)
{
	mod_music_free(mh_mod);
	mod_music_free(patbuf);
}

/*
Old (amiga) noteinfo:

_____byte 1_____   byte2_	 _____byte 3_____	byte4_
/				 \ /	  \  /				  \ /	   \
0000		  0000-00000000  0000		   0000-00000000

Upper four	  12 bits for	 Lower four    Effect command.
bits of sam-  note period.	 bits of sam-
ple number. 				 ple number.

*/
//#define "npertab_inc.h"
static u16 npertab[7*OCTAVE] =
{
	/* -> Tuning 0 */
	1712,1616,1524,1440,1356,1280,1208,1140,1076,1016, 960, 906,
	 856, 808, 762, 720, 678, 640, 604, 570, 538, 508, 480, 453,
	 428, 404, 381, 360, 339, 320, 302, 285, 269, 254, 240, 226,
	 214, 202, 190, 180, 170, 160, 151, 143, 135, 127, 120, 113,
	 107, 101,	95,  90,  85,  80,	75,  71,  67,  63,	60,  56,

	  53,  50,	47,  45,  42,  40,	37,  35,  33,  31,	30,  28,
	  27,  25,	24,  22,  21,  20,	19,  18,  17,  16,	15,  14
};

static void ConvertNote(MODNOTE *n)
{
	u8 instrument, effect, effdat, note;
	u16 period;
	u8 lastnote = 0;

	/* extract the various information from the 4 bytes that make up a note */
	instrument = (n->a & 0x10) | (n->c >> 4);
	period = (((u16)n->a & 0xf) << 8) + n->b;
	effect = n->c & 0xf;
	effdat = n->d;

	/* Convert the period to a note number */
	note = 0;
	if (period)
	{
		for (note = 0; note < 7 * OCTAVE; note++)
			if (period >= npertab[note])
				break;
		if (note == 7 * OCTAVE)
			note = 0;
		else
			note++;
	}

	if (instrument)
	{
		/* if instrument does not exist, note cut */
		if ((instrument > 31) || (!mh_mod->samples[instrument - 1].length))
		{
			UniPTEffect(0xc, 0);
			if (effect == 0xc)
				effect = effdat = 0;
		}
		else
		{
			/* Protracker handling */
			if (!mod_mod_type)
			{
				/* if we had a note, then change instrument... */
				if (note)
					UniInstrument(instrument - 1);
				/* ...otherwise, only adjust volume... */
				else
				{
					/* ...unless an effect was specified, which forces a new
					   note to be played */
					if (effect || effdat)
					{
						UniInstrument(instrument - 1);
						note = lastnote;
					}
					else
					{
						UniPTEffect(0xc,
									mh_mod->samples[instrument -
												1].volume & 0x7f);
					}
				}
			}
			else
			{
				/* Fasttracker handling */
				UniInstrument(instrument - 1);
				if (!note)
					note = lastnote;
			}
		}
	}
	if (note)
	{
		UniNote(note + 2 * OCTAVE - 1);
		lastnote = note;
	}

	/* Convert pattern jump from Dec to Hex */
	if (effect == 0xd)
		effdat = (((effdat & 0xf0) >> 4) * 10) + (effdat & 0xf);

	/* Volume slide, up has priority */
	if ((effect == 0xa) && (effdat & 0xf) && (effdat & 0xf0))
		effdat &= 0xf0;

	/* Handle ``heavy'' volumes correctly */
	if ((effect == 0xc) && (effdat > 0x40))
		effdat = 0x40;

	/* Ignore 100, 200 and 300 (there is no porta memory in mod files) */
	if ((!effdat) && ((effect == 1)||(effect == 2)||(effect ==3)))
		effect = 0;

	UniPTEffect(effect, effdat);
}

static u8 *ConvertTrack(MODNOTE *n)
{
	int t;
	UniReset();
	for (t=0; t<64; t++)
	{
		ConvertNote(n);
		UniNewline();
		n += mmoo.numchn;
	}
	return UniDup();
}

/* Loads all patterns of a modfile and converts them into the 3 byte format. */
static MM_BOOL ML_LoadPatterns(void)
{
	if (!AllocPatterns())
	{	return (0);
	}
	if (!AllocTracks())
	{	return (0);
	}
	/* Allocate temporary buffer for loading and converting the patterns */
	if (!(patbuf = (MODNOTE *)mod_music_calloc(64U * mmoo.numchn, sizeof(MODNOTE))))
	{	return (0);
	}
	//
	int tracks = 0;
	int t;
	for (t=0; t<mmoo.numpat; t++)
	{
		/* Load the pattern into the temp buffer and convert it */
		unsigned int s;
		for (s=0; s<(64U * mmoo.numchn); s++)
		{
			patbuf[s].a = (u8)(music_fgetc());
			patbuf[s].b = (u8)(music_fgetc());
			patbuf[s].c = (u8)(music_fgetc());
			patbuf[s].d = (u8)(music_fgetc());
		}
		for (s=0; s<mmoo.numchn; s++)
		{
			if (!(mmoo.tracks[tracks++] = ConvertTrack(patbuf + s)))
			{
				return (0);
			}
		}
	}
	return (1);
}

static MM_BOOL MOD_Load(MM_BOOL curious)
{
	int t, scan;
	MM_SAMPLE *q;
	MSAMPINFO *s;

	/* try to read module header */
	mod_music_file_read_string(mmff.mod_FILE_reader, (MM_s8 *)mh_mod->songname, 20);
	mh_mod->songname[20]	= 0;		/* just in case */

	for (t=0; t<31; t++)
	{
		s = &mh_mod->samples[t];
		mod_music_file_read_string(mmff.mod_FILE_reader, s->samplename, 22);
		s->samplename[22]	= 0;	/* just in case */
		s->length			= mod_music_file_read_u16BE(mmff.mod_FILE_reader);
		s->mm_fine_tune 	= (u8)(music_fgetc());
		s->volume			= (u8)(music_fgetc());
		s->reppos			= mod_music_file_read_u16BE(mmff.mod_FILE_reader);
		s->replen			= mod_music_file_read_u16BE(mmff.mod_FILE_reader);
	}

	mh_mod->songlength		= (u8)(music_fgetc());
	mh_mod->magic1			= (u8)(music_fgetc());
	mod_music_file_read_multi_u8(mmff.mod_FILE_reader, mh_mod->positions, 128);
	mod_music_file_read_multi_u8(mmff.mod_FILE_reader, mh_mod->magic2, 4);

	if (mod_music_eof(mmff.mod_FILE_reader))
	{
		mod_music_error_number = MOD_MUSIC_ERROR_LOADING_HEADER;
		return (0);
	}

	/* set module variables */
	mmoo.initspeed	= 6;
	mmoo.inittempo	= 125;
//	MM_s8 *descr;
	if (!(MOD_CheckType(mh_mod->magic2, &mmoo.numchn/*, &descr*/)))
	{
		mod_music_error_number = MOD_MUSIC_ERROR_NOT_A_MOD_MUSIC_MODULE;
		return (0);
	}
//�p�~		�Ȗ�������: 	mmoo.string_songname = DupStr(mh_mod->songname, 21, 1);
	mmoo.numpos 	= mh_mod->songlength;
	mmoo.reppos 	= 0;

	/* Count the number of patterns */
	mmoo.numpat 	= 0;
	for (t=0; t<mmoo.numpos; t++)
	{	if (mh_mod->positions[t] > mmoo.numpat)
		{	mmoo.numpat = mh_mod->positions[t];
		}
	}
	/* since some old modules embed extra patterns, we have to check the
	   whole list to get the samples' file offsets right - however we can find
	   garbage here, so check carefully */
	scan	= 1;
	for (t=mmoo.numpos; t<128; t++)
	{	if (mh_mod->positions[t] >= 0x80)
		{	scan = 0;
		}
	}
	if (scan)
	{	for (t=mmoo.numpos; t<128; t++)
		{
			if (mh_mod->positions[t] > mmoo.numpat)
			{	mmoo.numpat = mh_mod->positions[t];}
			if ((curious) && (mh_mod->positions[t]))
			{	mmoo.numpos = t + 1;}
		}
	}
	mmoo.numpat++;
	mmoo.numtrk = mmoo.numpat * mmoo.numchn;

	if (!AllocPositions(mmoo.numpos))
	{	return (0);
	}
	for (t=0; t<mmoo.numpos; t++)
	{	mmoo.positions[t] = mh_mod->positions[t];
	}
	/* Finally, init the sampleinfo structures	*/
	mmoo.numins = mmoo.numsmp = 31;
	if (!AllocSamples())
	{	return (0);
	}
	s = mh_mod->samples;
	q = mmoo.samples;
	for (t=0; t<mmoo.numins; t++)
	{
		/* convert the samplename */
		q->samplename	= DupStr(s->samplename, 23, 1);
		/* init the sampleinfo variables and convert the size pointers */
		q->speed		= mm_fine_tune[s->mm_fine_tune & 0xf];
		q->volume		= s->volume & 0x7f;
		q->loopstart	= (u32)s->reppos << 1;
		q->loopend		= q->loopstart + ((u32)s->replen << 1);
		q->length		= (u32)s->length << 1;
		q->flags		= SF_SIGNED;
		/* Imago Orpheus creates MODs with 16 bit samples, check */
		if ((mod_mod_type == 2) && (s->volume & 0x80))
		{
			q->flags |= SF_16BITS;
		//�p�~		descr	= orpheus;
		}
		if (s->replen > 2)
		{	q->flags |= SF_LOOP;
		}
		/* fix replen if repend > length */
		if (q->loopend > q->length)
		{	q->loopend = q->length;
		}
		s++;
		q++;
	}
//�p�~ (MikMOD�����ʂ���)���W���[����������:		mmoo.string_modtype = strdup(descr);
	if (!ML_LoadPatterns())
	{	return (0);
	}
	return (1);
}

#if 0/*�Ȗ��p�~*/
static MM_s8 *MOD_LoadTitle(void)
{
	mod_music_file_seek_rewind(mmff.mod_FILE_reader);
	//
	MM_s8 s[21];
	mod_music_file_read_u8S(s, 20, mmff.mod_FILE_reader);
//	if (0==)	{	return (NULL);	}
	s[20] = 0;					/* just in case */
	return (DupStr(s, 21, 1));
}
#endif
/*---------- Loader information */

MOD_MUSIC_API MOD_MUSIC_INTERNAL_LOADER load_mod =
{
	NULL,
	"Standard module",
	"MOD (31 instruments)",
	MOD_Init,
	MOD_Test,
	MOD_Load,
	MOD_Cleanup,
//	/*�Ȗ��p�~*/	MOD_LoadTitle
};

/* ex:set ts=4: */
