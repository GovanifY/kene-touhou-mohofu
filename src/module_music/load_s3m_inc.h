
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	"*.S3M"形式のファイルを読み込んで、ユニバーサル形式(MikMODの再生形式)に変換します。
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
	-------------------------------------------------------
	このファイルは "mlreg.c" からインクルードされます。
	-------------------------------------------------------
	$Id: load_s3m.c,v 1.5 2001/09/11 18:49:18 slouken Exp $
	Screamtracker (S3M) module loader
---------------------------------------------------------*/

//#ifdef HA VE_CONFIG_H
//#include "co nfig.h"
//#endif

#include <string.h>/*memset*/

#include "mohou_mikmod_internals.h"

/*---------- Module structure */

/* header */
typedef struct S3MHEADER
{
	MM_CHAR  songname[28];
	u8 t1a;
	u8 type;
	u8 unused1[2];
	u16 ordnum;
	u16 insnum;
	u16 patnum;
	u16 flags;
	u16 tracker;
	u16 fileformat;
	MM_CHAR  scrm[4];
	u8 mastervol;
	u8 initspeed;
	u8 inittempo;
	u8 mastermult;
	u8 ultraclick;
	u8 pantable;
	u8 unused2[8];
	u16 special;
	u8 channels[32];
} S3MHEADER;

/* sample information */
typedef struct S3MSAMPLE
{
	u8 type;
	MM_CHAR  filename[12];
	u8 memsegh;
	u16 memsegl;
	u32 length;
	u32 loopbeg;
	u32 loopend;
	u8 volume;
	u8 dsk;
	u8 pack;
	u8 flags;
	u32 c2spd;
	u8 unused[12];
	MM_CHAR  sampname[28];
	MM_CHAR  scrs[4];
} S3MSAMPLE;

typedef struct S3MNOTE
{
	u8 note,ins,vol,cmd,inf;
} S3MNOTE;

/*---------- Loader variables */

static S3MNOTE		*s3mbuf 		= NULL; /* pointer to a complete S3M pattern */
static S3MHEADER	*mh_s3m 		= NULL;
static u16		*paraptr_s3m	= NULL; /* parapointer array (see S3M docs) */
static unsigned int tracker;	/* tracker id */


/*---------- Loader code */

MM_BOOL S3M_Test(void)
{
	u8 id[4];
	mod_music_file_seek_set(mm_midi.mod_FILE_reader, 0x2c);
	if (!mod_music_file_read_multi_u8(mm_midi.mod_FILE_reader, id, 4))	{	return (0);}
	if (!memcmp(id,"SCRM",4))	{	return (1);}
	return (0);
}

MM_BOOL S3M_Init(void)
{
	s3mbuf 					= (S3MNOTE*)mod_music_malloc(32*64*sizeof(S3MNOTE));	if (!(s3mbuf))					{	return (0);}
	mh_s3m 					= (S3MHEADER*)mod_music_malloc(sizeof(S3MHEADER));		if (!(mh_s3m))					{	return (0);}
	mm_position_look_up 	= (u8*)mod_music_malloc(sizeof(u8)*256);			if (!(mm_position_look_up)) 	{	return (0);}
	memset(mm_position_look_up,-1,256);
	return (1);
}

void S3M_Cleanup(void)
{
	mod_music_free(s3mbuf);
	mod_music_free(paraptr_s3m);
	mod_music_free(mm_position_look_up);
	mod_music_free(mh_s3m);
	mod_music_free(origpositions);
}

/* Because so many s3m files have 16 channels as the set number used, but really
   only use far less (usually 8 to 12 still), I had to make this function, which
   determines the number of channels that are actually USED by a pattern.

   For every channel that's used, it sets the appropriate array entry of
	the global variable 'mm_re_map'

   NOTE: You must first seek to the file location of
	the pattern before calling this procedure.

   Returns 1 on fail.														  */
static MM_BOOL S3M_GetNumChannels(void)
{
	int row = 0;
	while (row<64)
	{
		/*u8*/int flag;
		flag = (u8)(music_fgetc());
		if (mod_music_eof(mm_midi.mod_FILE_reader))
		{
			mod_music_error_number = MOD_MUSIC_ERROR_LOADING_PATTERN;
			return (1);
		}
		if (flag)
		{
			if (mh_s3m->channels[/*ch*/(flag&31)]<32) 	{	mm_re_map[/*ch*/(flag&31)] = 0;}
			if (flag&32)	{	(u8)(music_fgetc());(u8)(music_fgetc());	}
			if (flag&64)	{	(u8)(music_fgetc());									}
			if (flag&128)	{	(u8)(music_fgetc());(u8)(music_fgetc());	}
		}
		else
		{
			row++;
		}
	}
	return (0);
}

static MM_BOOL S3M_ReadPattern(void)
{
	int row=0;
	int flag;
	S3MNOTE *n,dummy;

	/* clear pattern data */
	memset(s3mbuf,255,32*64*sizeof(S3MNOTE));

	while (row<64)
	{
		flag = (u8)(music_fgetc());
		if (mod_music_eof(mm_midi.mod_FILE_reader))
		{
			mod_music_error_number = MOD_MUSIC_ERROR_LOADING_PATTERN;
			return (0);
		}

		if (flag)
		{
			int ch;
			ch = mm_re_map[flag&31];
			if (ch!=-1)
			{	n = &s3mbuf[(64U*ch)+row];}
			else
			{	n = &dummy;}

			if (flag&32)
			{
				n->note	= (u8)(music_fgetc());
				n->ins	= (u8)(music_fgetc());
			}
			if (flag&64)
			{
				n->vol	= (u8)(music_fgetc());
				if (n->vol>64) n->vol=64;
			}
			if (flag&128)
			{
				n->cmd	= (u8)(music_fgetc());
				n->inf	= (u8)(music_fgetc());
			}
		}
		else
		{	row++;}
	}
	return (1);
}

static u8* S3M_ConvertTrack(S3MNOTE* tr)
{
	UniReset();
	int t;
	for (t=0; t<64; t++)
	{
		u8 note,ins,vol;
		note= tr[t].note;
		ins = tr[t].ins;
		vol = tr[t].vol;

		if ((ins)&&(ins!=255)) UniInstrument(ins-1);
		if (note!=255)
		{
			if (note==254)
			{
				UniPTEffect(0xc,0); /* note cut command */
				vol=255;
			}
			else
			{
				UniNote(((note>>4)*OCTAVE)+(note&0xf)); /* normal note */
			}
		}
		if (vol<255) UniPTEffect(0xc,vol);

		S3MIT_ProcessCmd(tr[t].cmd,tr[t].inf,
			tracker == 1 ? S3MIT_OLDSTYLE | S3MIT_SCREAM : S3MIT_OLDSTYLE);
	//	(S3MIT_OLDSTYLE | S3MIT_SCREAM) );
		UniNewline();
	}
	return (UniDup());
}

MM_BOOL S3M_Load(MM_BOOL curious)
{
	int t,u,track = 0;
	MM_SAMPLE *q;
	u8 pan[32];

	/* try to read module header */
	mod_music_file_read_string(mm_midi.mod_FILE_reader, mh_s3m->songname, 28);
	mh_s3m->t1a 		= (u8)(music_fgetc());
	mh_s3m->type		= (u8)(music_fgetc());
	mod_music_file_read_multi_u8(mm_midi.mod_FILE_reader, mh_s3m->unused1, 2);
	mh_s3m->ordnum		= music_fget16();
	mh_s3m->insnum		= music_fget16();
	mh_s3m->patnum		= music_fget16();
	mh_s3m->flags		= music_fget16();
	mh_s3m->tracker 	= music_fget16();
	mh_s3m->fileformat	= music_fget16();
	mod_music_file_read_string(mm_midi.mod_FILE_reader, mh_s3m->scrm, 4);
	mh_s3m->mastervol	= (u8)(music_fgetc());
	mh_s3m->initspeed	= (u8)(music_fgetc());
	mh_s3m->inittempo	= (u8)(music_fgetc());
	mh_s3m->mastermult	= (u8)(music_fgetc());
	mh_s3m->ultraclick	= (u8)(music_fgetc());
	mh_s3m->pantable	= (u8)(music_fgetc());
	mod_music_file_read_multi_u8(mm_midi.mod_FILE_reader, mh_s3m->unused2, 8);
	mh_s3m->special 	= music_fget16();
	mod_music_file_read_multi_u8(mm_midi.mod_FILE_reader, mh_s3m->channels, 32);

	if (mod_music_eof(mm_midi.mod_FILE_reader))
	{
		mod_music_error_number = MOD_MUSIC_ERROR_LOADING_HEADER;
		return (0);
	}

	/* then we can decide the module type */
	tracker = mh_s3m->tracker>>12;
	#define NUMTRACKERS 	(4)/* tracker identifiers */
	if ((!tracker)||(tracker>=NUMTRACKERS))/*>3*/
	{			tracker = NUMTRACKERS-1;}	/* unknown tracker */
	else
	{
		if (mh_s3m->tracker>=0x3217)
				{tracker = NUMTRACKERS+1;}	/* IT 2.14p4 */
		else
		if (mh_s3m->tracker>=0x3216)
				{tracker = NUMTRACKERS;}	/* IT 2.14p3 */
		else	{tracker--;}
	}
	#if 0//廃止 	(MikMODが判別した)モジュール名文字列: string_modtype
	{
		static const MM_CHAR* S3M_Version[] =
		{
			"Screamtracker x.xx",
			"Imago Orpheus x.xx (S3M format)",
			"Impulse Tracker x.xx (S3M format)",
			"Unknown tracker x.xx (S3M format)",
			"Impulse Tracker 2.14p3 (S3M format)",
			"Impulse Tracker 2.14p4 (S3M format)"
		};
		of.string_modtype = strdup(S3M_Version[tracker]);
		if (tracker<NUMTRACKERS)
		{
			/* version number position in above array */
			static int numeric[NUMTRACKERS] =	{14,14,16,16};
			of.string_modtype[numeric[tracker]	] = ((mh_s3m->tracker>>8)&0x0f)+'0';
			of.string_modtype[numeric[tracker]+2] = ((mh_s3m->tracker>>4)&0x0f)+'0';
			of.string_modtype[numeric[tracker]+3] = ((mh_s3m->tracker	)&0x0f)+'0';
		}
	}
	#endif
	/* set module variables */
//廃止 曲名文字列:	of.string_songname		= DupStr(mh_s3m->songname,28,0);
	of.numpat		= mh_s3m->patnum;
	of.reppos		= 0;
	of.numins		= of.numsmp 	= mh_s3m->insnum;
	of.initspeed	= mh_s3m->initspeed;
	of.inittempo	= mh_s3m->inittempo;
	of.initvolume	= mh_s3m->mastervol<<1;
	of.flags		|= UF_ARPMEM /*| UF_PANNING*/;
	if ((mh_s3m->tracker==0x1300)||(mh_s3m->flags&64))
	{	of.flags	|= UF_S3MSLIDES;}
//	of.bpmlimit 	= 32;

	/* read the order data */
	if (!AllocPositions(mh_s3m->ordnum)) 	{	return (0);}
	origpositions = mod_music_calloc(mh_s3m->ordnum,sizeof(u16));
	if (!(origpositions)) 	{	return (0);}

	for (t=0; t<(mh_s3m->ordnum); t++)
	{
		origpositions[t] = (u8)(music_fgetc());
		if ((origpositions[t]>=mh_s3m->patnum)&&(origpositions[t]<254))
		{	origpositions[t] = 255/*mh_s3m->patnum-1*/;
		}
	}

	if (mod_music_eof(mm_midi.mod_FILE_reader))
	{
		mod_music_error_number = MOD_MUSIC_ERROR_LOADING_HEADER;
		return (0);
	}

	mm_midi.mm_midi_position_look_up_counter = (u8)mh_s3m->ordnum;
	S3MIT_CreateOrders(curious);
	paraptr_s3m = (u16*)mod_music_malloc((of.numins+of.numpat)*sizeof(u16));
	if (!(paraptr_s3m))
	{	return (0);
	}
	/* read the instrument+pattern parapointers */
	mod_music_file_read_multi_u16LE(mm_midi.mod_FILE_reader, paraptr_s3m, of.numins+of.numpat);

	if (mh_s3m->pantable==252)
	{
		/* read the panning table(ST 3.2 addition.  See below for further
		   portions of channel panning [past reampper]). */
		mod_music_file_read_multi_u8(mm_midi.mod_FILE_reader, pan, 32);
	}

	if (mod_music_eof(mm_midi.mod_FILE_reader))
	{
		mod_music_error_number = MOD_MUSIC_ERROR_LOADING_HEADER;
		return (0);
	}

	/* load samples */
	if (!AllocSamples())	{	return (0);}
	q = of.samples;
	for (t=0; t<of.numins; t++)
	{
		S3MSAMPLE s;
		/* seek to instrument position */
		mod_music_file_seek_set(mm_midi.mod_FILE_reader, ((long)paraptr_s3m[t])<<4);
		/* and load sample info */
		s.type			= (u8)(music_fgetc());
		mod_music_file_read_string(mm_midi.mod_FILE_reader, s.filename, 12);
		s.memsegh		= (u8)(music_fgetc());
		s.memsegl		= music_fget16();
		s.length		= music_fget32();
		s.loopbeg		= music_fget32();
		s.loopend		= music_fget32();
		s.volume		= (u8)(music_fgetc());
		s.dsk			= (u8)(music_fgetc());
		s.pack			= (u8)(music_fgetc());
		s.flags 		= (u8)(music_fgetc());
		s.c2spd 		= music_fget32();
		mod_music_file_read_multi_u8(mm_midi.mod_FILE_reader, s.unused, 12);
		mod_music_file_read_string(mm_midi.mod_FILE_reader, s.sampname, 28);
		mod_music_file_read_string(mm_midi.mod_FILE_reader, s.scrs, 4);

	//	/* ScreamTracker imposes a 64000 bytes (not 64k !) limit */
	//	if (s.length > 64000)
	//	{	s.length = 64000;}
		if (mod_music_eof(mm_midi.mod_FILE_reader))
		{
			mod_music_error_number	= MOD_MUSIC_ERROR_LOADING_SAMPLEINFO;
			return (0);
		}

		q->samplename	= DupStr(s.sampname, 28, 0);
		q->speed		= s.c2spd;
		q->length		= s.length;
		q->loopstart	= s.loopbeg/*(s.loopbeg>s.length?s.length:s.loopbeg)*/;
		q->loopend		= s.loopend/*(s.loopend>s.length?s.length:s.loopend)*/;
		q->volume		= s.volume;
		q->seekpos		= (((long)s.memsegh)<<16|s.memsegl)<<4;

		if (s.flags&1)				{q->flags |= SF_LOOP;}
		if (s.flags&4)				{q->flags |= SF_16BITS;}
		if (mh_s3m->fileformat==1)	{q->flags |= SF_SIGNED;}

		/* don't load sample if it doesn't have the SCRS tag */
		if (memcmp(s.scrs,"SCRS",4))	{q->length = 0;}
		q++;
	}

	/* determine the number of channels actually used. */
	of.numchn = 0;
	memset(mm_re_map,-1,32*sizeof(u8));
	for (t=0; t<of.numpat; t++)
	{
		/* seek to pattern position (+2 skip pattern length) */
		mod_music_file_seek_set(mm_midi.mod_FILE_reader, (long)((paraptr_s3m[of.numins+t])<<4)+2);
		if (S3M_GetNumChannels())	{return (0);}
	}

	/* build the mm_re_map array  */
	for (t=0; t<32; t++)
	{	if (!mm_re_map[t])
		{	mm_re_map[t] = of.numchn++;
		}
	}
	/* set panning positions after building mm_re_map chart! */
	for (t=0; t<32; t++)
	{	if ((mh_s3m->channels[t]<32)&&(mm_re_map[t]!=-1))
		{
			if (mh_s3m->channels[t]<8)
			{	of.panning[mm_re_map[t]] = 0x30;	/* 0x30 = std s3m val / 0x20 */
			}
			else
			{	of.panning[mm_re_map[t]] = 0xc0;	/* 0xc0 = std s3m val / 0xd0 */
			}
		}
	}
	if (mh_s3m->pantable==252)
	{	/* set panning positions according to panning table (new for st3.2) */
		for (t=0; t<32; t++)
		{	if ((pan[t]&0x20)&&(mh_s3m->channels[t]<32)&&(mm_re_map[t]!=-1))
			{	of.panning[mm_re_map[t]] = ((pan[t]&0xf)<<4);
			}
		}
	}
	/* load pattern info */
	of.numtrk = (of.numpat * of.numchn);
	if (!AllocTracks()) 	{return (0);}
	if (!AllocPatterns())	{return (0);}

	for (t=0; t<of.numpat; t++)
	{
		/* seek to pattern position (+2 skip pattern length) */
		mod_music_file_seek_set(mm_midi.mod_FILE_reader, (((long)paraptr_s3m[of.numins+t])<<4)+2);
		if (!S3M_ReadPattern()) 	{return (0);}
		for (u=0; u<of.numchn; u++)
		{	if (!(of.tracks[track++]=S3M_ConvertTrack(&s3mbuf[u*64])))
			{
				return (0);
			}
		}
	}
	return (1);
}

#if 0/*曲名廃止*/
MM_CHAR *S3M_LoadTitle(void)
{
	MM_CHAR s[28];
	mod_music_file_seek_rewind(mm_midi.mod_FILE_reader);
	if (!mod_music_file_read_u8S(s, 28, mm_midi.mod_FILE_reader)) 	{return (NULL);}
	return (DupStr(s,28,0));
}
#endif
/*---------- Loader information */

MOD_MUSIC_API MOD_MUSIC_INTERNAL_LOADER load_s3m =
{
	NULL,
	"S3M",
	"S3M (Scream Tracker 3)",
	S3M_Init,
	S3M_Test,
	S3M_Load,
	S3M_Cleanup,
//	/*曲名廃止*/	S3M_LoadTitle
};

/* ex:set ts=4: */
