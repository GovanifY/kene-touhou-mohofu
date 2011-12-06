
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	"*.IT"形式のファイルを読み込んで、ユニバーサル形式(MikMODの再生形式)に変換します。
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
	-------------------------------------------------------
	このファイルは "mlreg.c" からインクルードされます。
	-------------------------------------------------------
	$Id: load_it.c, v 1.6 2001/09/11 18:49:18 slouken Exp $
	Impulse tracker (IT) module loader
---------------------------------------------------------*/

//#ifdef HA VE_CONFIG_H
//#include "co nfig.h"
//#endif

//#include <ctype.h>/*isaln um tou pper*/
#include <string.h>/*memset memcpy strdup st rncmp*/

#include "mohou_mikmod_internals.h"

/*---------- Module structure */

/* header */
typedef struct ITHEADER
{
	MM_s8 songname[26];	/* '01234567890123456789012345' 曲名(但しシフトjisコード対象外) */
	u8		blank01[2];
	u16 	ordnum;
	u16 	insnum;
	u16 	smpnum;
	u16 	patnum;
	u16 	cwt;		/* Created with tracker (y.xx = 0x0yxx) */
	u16 	cmwt;		/* Compatible with tracker ver > than val. */
	u16 	flags;
	u16 	special;	/* bit 0 set = song message attached */
	u8		globvol;
	u8		mixvol; 	/* mixing volume [ignored] */
	u8		initspeed;
	u8		inittempo;
	u8		pansep; 	/* panning separation between channels */
	u8		zerobyte;
	u16 	msglength;
	u32 	msgoffset;
	u8		blank02[4];
	u8		pantable[64];
	u8		voltable[64];
} ITHEADER;

/* sample information */
typedef struct ITSAMPLE
{
	MM_s8 filename[12];
	u8	zerobyte;
	u8	globvol;
	u8	flag;
	u8	volume;
	u8	panning;
	MM_s8 sampname[28];
	u16 convert;	/* sample conversion flag */
	u32 length;
	u32 loopbeg;
	u32 loopend;
	u32 c5spd;
	u32 susbegin;
	u32 susend;
	u32 sampoffset;
	u8	vibspeed;
	u8	vibdepth;
	u8	vibrate;
	u8	vibwave;	/* 0=sine, 1=rampdown, 2=square, 3=random (speed ignored) */
} ITSAMPLE;

/* instrument information */

#define ITENVCNT 25
#define ITNOTECNT 120
typedef struct ITINSTHEADER
{
	u32 size;			/* (dword) Instrument size */
	MM_s8 filename[12];	/* (char) Instrument filename */
	u8	zerobyte;		/* (byte) Instrument type (always 0) */
	u8	volflg;
	u8	volpts;
	u8	volbeg; 		/* (byte) Volume loop start (node) */
	u8	volend; 		/* (byte) Volume loop end (node) */
	u8	volsusbeg;		/* (byte) Volume sustain begin (node) */
	u8	volsusend;		/* (byte) Volume Sustain end (node) */
	u8	panflg;
	u8	panpts;
	u8	panbeg; 		/* (byte) channel loop start (node) */
	u8	panend; 		/* (byte) channel loop end (node) */
	u8	pansusbeg;		/* (byte) channel sustain begin (node) */
	u8	pansusend;		/* (byte) channel Sustain end (node) */
	u8	pitflg;
	u8	pitpts;
	u8	pitbeg; 		/* (byte) pitch loop start (node) */
	u8	pitend; 		/* (byte) pitch loop end (node) */
	u8	pitsusbeg;		/* (byte) pitch sustain begin (node) */
	u8	pitsusend;		/* (byte) pitch Sustain end (node) */
	u16 blank;
	u8	globvol;
	u8	chanpan;
	u16 fadeout;		/* Envelope end / NNA volume fadeout */
	u8	dnc;			/* Duplicate note check */
	u8	dca;			/* Duplicate check action */
	u8	dct;			/* Duplicate check type */
	u8	nna;			/* New Note Action [0, 1, 2, 3] */
	u16 trkvers;		/* tracker version used to save [files only] */
	u8	ppsep;			/* Pitch-pan Separation */
	u8	ppcenter;		/* Pitch-pan Center */
	u8	rvolvar;		/* random volume varations */
	u8	rpanvar;		/* random panning varations */
	u16 numsmp; 		/* Number of samples in instrument [files only] */
	MM_s8 name[26];		/* Instrument name */
	u8	blank01[6];
	u16 samptable[ITNOTECNT];/* sample for each note [note / samp pairs] */
	u8	volenv[200];		 /* volume envelope (IT 1.x stuff) */
	u8	oldvoltick[ITENVCNT];/* volume tick position (IT 1.x stuff) */
	u8	volnode[ITENVCNT];	 /* amplitude of volume nodes */
	u16 voltick[ITENVCNT];	 /* tick value of volume nodes */
	s8	pannode[ITENVCNT];	 /* panenv - node points */
	u16 pantick[ITENVCNT];	 /* tick value of panning nodes */
	s8	pitnode[ITENVCNT];	 /* pitchenv - node points */
	u16 pittick[ITENVCNT];	 /* tick value of pitch nodes */
} ITINSTHEADER;

/* unpacked note */

typedef struct ITNOTE
{
	u8	note;
	u8	ins;
	u8	volpan;
	u8	cmd;
	u8	inf;
} ITNOTE;

/*---------- Loader data */

static u32 *paraptr_it	= NULL; 	/* parapointer array (see IT docs) */
static ITHEADER *mh_it		= NULL;
static ITNOTE *itpat		= NULL; 	/* allocate to space for one full pattern */
static u8 *mask 		= NULL; 	/* arrays allocated to 64 elements and used for */
static ITNOTE *last 		= NULL; 	/* uncompressing IT's pattern information */
static int numtrk			= 0;
static u8/*unsigned int*/ old_effect;	/* if set, use S3M old-effects stuffs */



/*---------- Loader code */

MM_BOOL IT_Test(void)
{
	u8 id[4];
	mod_music_file_read_multi_u8(mmff.mod_FILE_reader, id, 4);
	//if (0==)	{return (0);}
	if (!memcmp(id, "IMPM", 4)) {return (1);}
	return (0);
}

MM_BOOL IT_Init(void)
{
	if (!(mh_it 				= (ITHEADER*)	mod_music_malloc(		  sizeof(ITHEADER))))		{	return (0);}
	if (!(mm_position_look_up	= (u8*) 	mod_music_malloc(	256 * sizeof(u8)))) 		{	return (0);}
	if (!(itpat 				= (ITNOTE*) 	mod_music_malloc(200*64 * sizeof(ITNOTE)))) 		{	return (0);}
	if (!(mask					= (u8*) 	mod_music_malloc(	 64 * sizeof(u8)))) 		{	return (0);}
	if (!(last					= (ITNOTE*) 	mod_music_malloc(	 64 * sizeof(ITNOTE)))) 		{	return (0);}
	return (1);
}

void IT_Cleanup(void)
{
	FreeLinear();
	mod_music_free(mh_it);
	mod_music_free(mm_position_look_up);
	mod_music_free(itpat);
	mod_music_free(mask);
	mod_music_free(last);
	mod_music_free(paraptr_it);
	mod_music_free(origpositions);
}

/* Because so many IT files have 64 channels as the set number used, but really
   only use far less (usually from 8 to 24 still), I had to make this function,
   which determines the number of channels that are actually USED by a pattern.

   NOTE: You must first seek to the file location of the pattern before calling
		 this procedure.

   Returns 1 on error
*/
static MM_BOOL IT_GetNumChannels(u16 patrows)
{
	int row = 0;
	int flag;
	int ch;
	int skip_dummy; 	/* read skip dummy read. / ダミー読み込み用(読んだ値は使わない) */
	do
	{
		flag = (u8)(music_fgetc());
		if (EOF==flag)
		{
			mod_music_error_number = MOD_MUSIC_ERROR_LOADING_PATTERN;
			return (1);
		}
		if (!flag)
		{
			row++;
		}
		else
		{
			ch = ((flag-1)&63);
			mmff.re_map[ch] = 0;
			if (flag & 128) 	{	mask[ch]	= (u8)(music_fgetc());	}
			if (mask[ch]&1) 	{	skip_dummy	= (u8)(music_fgetc());	}
			if (mask[ch]&2) 	{	skip_dummy	= (u8)(music_fgetc());	}
			if (mask[ch]&4) 	{	skip_dummy	= (u8)(music_fgetc());	}
			if (mask[ch]&8) 	{	skip_dummy	= (u8)(music_fgetc());
									skip_dummy	= (u8)(music_fgetc());	}
		}
	} while (row<patrows);
	return (0);
}
static int panconvert64to255(int in_value)
{
	#if 0
	in_value = ((in_value&0x7f)==64)?(255):((in_value&0x7f)<<2);
	#else
	in_value &= 0x7f;
	in_value <<= 2;
	in_value = psp_max(in_value, 255);
	#endif
	return (in_value);
}
static u8* IT_ConvertTrack(ITNOTE* tr, u16 numrows)
{
	UniReset();
	int t;
	for (t=0; t<numrows; t++)
	{
		u8 note, ins, volpan;
		note	= tr[t*mmoo.numchn].note;
		ins 	= tr[t*mmoo.numchn].ins;
		volpan	= tr[t*mmoo.numchn].volpan;

		if (note!=255)
		{
			if (note==253)
			{	UniWriteByte(U231_ICODE_0x22_KEYOFF);}
			else if (note==254)
			{
				UniPTEffect(0xc, -1);	/* note cut command */
				volpan=255;
			}
			else
			{
				UniNote(note);
			}
		}
		if ((ins)&&(ins<100))	{	UniInstrument(ins-1);}
		else if (ins==253)		{	UniWriteByte(U231_ICODE_0x22_KEYOFF);}
		else if (ins!=255)	/* crap */
		{
			mod_music_error_number = MOD_MUSIC_ERROR_LOADING_PATTERN;
			return (NULL);
		}
	enum
	{
		VOL_0x00_none = 0,
		VOL_0x01_VOLUME,
		VOL_0x02_PANNING,
		VOL_0x03_VOLSLIDE,
		VOL_0x04_PITCHSLIDEDN,
		VOL_0x05_PITCHSLIDEUP,
		VOL_0x06_PORTAMENTO,
		VOL_0x07_VIBRATO,
	};

		/* process volume / panning column volume / panning effects
			 do NOT all share the same memory address yet. */
			 if (volpan <=	64) /* 0-64: Set Volume */													{	UniVolEffect(VOL_0x01_VOLUME,			volpan);}
		else if (volpan <=	74) /* 65-74: Fine Volume Up */ 		/* fine volume slide up (65-74) */	{	UniVolEffect(VOL_0x03_VOLSLIDE, 		0x0f+((volpan-65)<<4));}
		else if (volpan <=	84) /* 75-84: Fine Volume Down */		/* fine volume slide down (75-84) */{	UniVolEffect(VOL_0x03_VOLSLIDE, 		0xf0+(volpan-75));}
		else if (volpan <=	94) /* 85-94: Volume Slide Up */		/* volume slide up (85-94) */		{	UniVolEffect(VOL_0x03_VOLSLIDE, 		((volpan-85)<<4));}
		else if (volpan <= 104) /* 95-104: Volume Slide Down */ 	/* volume slide down (95-104) */	{	UniVolEffect(VOL_0x03_VOLSLIDE, 		(volpan-95));}
		else if (volpan <= 114) /* 105-114: Pitch Slide Up */		/* pitch slide down (105-114) */	{	UniVolEffect(VOL_0x04_PITCHSLIDEDN, 	((volpan-105)/*<<2*/));}/* 変 (???) <<2 */
		else if (volpan <= 124) /* 115-124: Pitch Slide Down */ 	/* pitch slide up (115-124) */		{	UniVolEffect(VOL_0x05_PITCHSLIDEUP, 	((volpan-115)/*<<2*/));}/* 変 (???) <<2 */
		else if (volpan <= 127) /* 125-127: Error */				/* error */ 	/* crap */			{	/*mod_music_error_number=MOD_MUSIC_ERROR_LOADING_PATTERN;	return (NULL);*/	}
		else if (volpan <= 192) /* 128-192: Set Panning */												{
		//	UniVolEffect(VOL_0x02_PANNING,			(((volpan-128)==64)?(255):((volpan-128)<<2)) );
			UniVolEffect(VOL_0x02_PANNING,		panconvert64to255( (volpan-128) ) );
		}
		else if (volpan <= 202) /* 193-202: Portamento To */		/* portamento to note */
		{
					/* table for porta-to-note command within volume/panning column */
					static const u8 portatable[10]= {0, 1, 4, 8, 16, 32, 64, 96, 128, 255};
					UniVolEffect(VOL_0x06_PORTAMENTO, portatable[volpan-193]);
		}
					// 203-212: Vibrato
		else if (volpan <= 212) /* 203-212: Vibrato */				/* vibrato */						{	UniVolEffect(VOL_0x07_VIBRATO, (volpan-203));}
					//	   239: ???
					//	   255: ???
					// 213-255: Error
	//	else if ((volpan!=239)&&(volpan!=255))	/* crap */
	//	{
	//		mod_music_error_number=MOD_MUSIC_ERROR_LOADING_PATTERN;
	//		return (NULL);
	//	}
		S3MIT_ProcessCmd(tr[t*mmoo.numchn].cmd, tr[t*mmoo.numchn].inf, (old_effect|S3MIT_IT) );
		UniNewline();
	}
	return (UniDup());
}

static MM_BOOL IT_ReadPattern(u16 patrows)
{
	int row = 0;
	int flag;
	int ch;
	int blah;
	ITNOTE *itt = itpat;
	ITNOTE dummy;
	ITNOTE *n;
	ITNOTE *l;
	#if 0
	memset(itt, 255, sizeof(ITNOTE)*200*64 );
	#else
	memset32( (void*)itt, /*255*/0xffffffff, sizeof(ITNOTE)*200*64/4 );/*(てすと)*/
	#endif
	do
	{	flag = (u8)(music_fgetc());
		if (EOF==flag)
		{
			mod_music_error_number = MOD_MUSIC_ERROR_LOADING_PATTERN;
			return (0);
		}
		if (!flag)
		{
			itt = &itt[mmoo.numchn];
			row++;
		}
		else
		{
			ch = mmff.re_map[(flag-1)&63];
			if (ch!=-1)
			{
				n = &itt[ch];
				l = &last[ch];
			}
			else
			{
				n = l = &dummy;
			}
			if (flag&128) mask[ch] = (u8)(music_fgetc());
			if (mask[ch]&0x01)
			{	/* convert IT note off to internal note off */
				l->note = n->note = (u8)(music_fgetc());
				if (255==(l->note))
				{	l->note=n->note = 253;}
			}
			if (mask[ch]&0x02)	{	l->ins		= n->ins	= (u8)(music_fgetc());}
			if (mask[ch]&0x04)	{	l->volpan	= n->volpan = (u8)(music_fgetc());}
			if (mask[ch]&0x08)
			{
				l->cmd = n->cmd = (u8)(music_fgetc());
				l->inf = n->inf = (u8)(music_fgetc());
			}
			if (mask[ch]&0x10)	{	n->note 	= l->note;}
			if (mask[ch]&0x20)	{	n->ins		= l->ins;}
			if (mask[ch]&0x40)	{	n->volpan	= l->volpan;}
			if (mask[ch]&0x80)
			{
				n->cmd = l->cmd;
				n->inf = l->inf;
			}
		}
	} while (row<patrows);
	for (blah=0; blah<mmoo.numchn; blah++)
	{
		if (!(mmoo.tracks[numtrk++]=IT_ConvertTrack(&itpat[blah], patrows)))
		{	return (0);}
	}
	return (1);
}

/*
	簡易版でも無問題。
	s-jisはisaln um()やtou pper()使ってる時点で端から非対応。
	下記の文字は変換されちゃうので使わない事。
	`{|}~ 変換前
	@[\]^ 変換後
*/
static int is_LoadMidiString(MF_READER* modreader, MM_s8* midiline)
{
	MM_s8 *cur;
	MM_s8 *last;
	mod_music_file_read_multi_u8(modreader, midiline, 32);
	cur = last = midiline;
	/* remove blanks and uppercase all */
	while (*last)
	{
		/* 0100 4, 0101 5, 0110 6, 0111 7, */
		/* 上位2ビットが 01(01-- ----)なら大体、文字(大体、大文字か小文字)。 */
	//	if (isaln um((int)*last))
		if ( 0==(((*last) & (0xc0))^(0x40)) )/*(正確でないテキトー版小文字っぽいならば)*/
		{
		/* 上位3ビット目を0にすれば大体、大文字になる。 */
		//	*(cur++) = tou pper((int)*last);
			*(cur++) = ((*last) & (0xdf));/*(正確でないテキトー版大文字化)*/
		}
		last++;
	}
	*cur = 0;
	// 先頭が "F0F00" 以外は NG。
	if ('F'!=midiline[0])	goto my_NG;
	if ('0'!=midiline[1])	goto my_NG;
	if ('F'!=midiline[2])	goto my_NG;
	if ('0'!=midiline[3])	goto my_NG;
	if ('0'!=midiline[4])	goto my_NG;
	// 次の文字が "0" もしくは "1" であるなら OK。
	if ('0'==midiline[5])	goto my_OK;
	if ('1'==midiline[5])	goto my_OK;
	// それ以外は NG。
		goto my_NG;
	my_OK:
		return (1/*TRUE*/);
	my_NG:
		return (0/*FALSE*/);
}
//	return (
//		(!st rncmp(midiline, "F0F00", 5)) &&
//		((midiline[5]=='0')||(midiline[5]=='1'))
//	);

/* Load embedded midi information for resonant filters */
static void IT_LoadMidiConfiguration(int use_file_mode)//MF_READER* modreader)
{
	int i;
	#if (1==USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)
	memset(mm_filter_macros, 0, sizeof(mm_filter_macros) );
	memset(filtersettings, 0, sizeof(filtersettings) );
	#endif /*(USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)*/

	if (use_file_mode/*modreader*/) /* information is embedded in file */
	{
		u16 dat;
		dat = music_fget16();
		mod_music_file_seek_cur(mmff.mod_FILE_reader, 8*dat+0x120);
		{
			MM_s8 midiline[(5*8)/*33*/];
			midiline[32] = (0);
			/* read midi macros */
			for (i=0; i<UF_MAXMACRO; i++)
			{
				if (is_LoadMidiString(mmff.mod_FILE_reader, midiline))
				{
					#if (1==USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)
					mm_filter_macros[i] = ((midiline[5]-'0')|0x80);
					#endif /*(USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)*/
				}
			}
			/* read standalone filters */
			for (i=0x80; i<0x100; i++)
			{
				if (is_LoadMidiString(mmff.mod_FILE_reader, midiline))
				{
					#if (1==USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)
					filtersettings[i].filter = ((midiline[5]-'0')|0x80);
					dat = (midiline[6])?(midiline[6]-'0'):0;
					if (midiline[7])		{dat = ((dat<<4)|(midiline[7]-'0'));}
					filtersettings[i].inf = (u8)dat;
					#endif /*(USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)*/
				}
			}
		}
	}
	else	/* use default information */
	{
		#if (1==USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)
		mm_filter_macros[0] = FILT_CUT;
		for (i=0x80; i<0x90; i++)
		{
			filtersettings[i].filter = FILT_RESONANT;
			filtersettings[i].inf = ((i&0x7f)<<3);
		}
		#endif /*(USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)*/
	}
	mmff.mm_midi_active_macro = 0;
	#if (1==USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)
	for (i=0; i<0x80; i++)
	{
		filtersettings[i].filter = mm_filter_macros[0];
		filtersettings[i].inf = i;
	}
	#endif /*(USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)*/
}

MM_BOOL IT_Load(MM_BOOL curious)
{
	int t;
	int uuu;
/*	MM_BOOL compressed=0;*/
	numtrk = 0;
	#if (1==USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)
	mmff.mm_midi_use_resonant_filters = 0;/* 新規ロード時に無効 */
	#endif /*(USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)*/
	/* try to read module header */
	music_fget32(); /* kill the 4 byte header */
	mod_music_file_read_string(mmff.mod_FILE_reader, mh_it->songname, 26);
	mod_music_file_read_multi_u8(mmff.mod_FILE_reader, mh_it->blank01, 2);
	mh_it->ordnum		= music_fget16();
	mh_it->insnum		= music_fget16();
	mh_it->smpnum		= music_fget16();
	mh_it->patnum		= music_fget16();
	mh_it->cwt			= music_fget16();
	mh_it->cmwt 		= music_fget16();
	mh_it->flags		= music_fget16();
	mh_it->special		= music_fget16();
	mh_it->globvol		= (u8)(music_fgetc());
	mh_it->mixvol		= (u8)(music_fgetc());
	mh_it->initspeed	= (u8)(music_fgetc());
	mh_it->inittempo	= (u8)(music_fgetc());
	mh_it->pansep		= (u8)(music_fgetc());
	mh_it->zerobyte 	= (u8)(music_fgetc());
	mh_it->msglength	= music_fget16();
	mh_it->msgoffset	= music_fget32();
	mod_music_file_read_multi_u8(mmff.mod_FILE_reader, mh_it->blank02,	4);
	mod_music_file_read_multi_u8(mmff.mod_FILE_reader, mh_it->pantable, 64);
	mod_music_file_read_multi_u8(mmff.mod_FILE_reader, mh_it->voltable, 64);

	if (mod_music_eof(mmff.mod_FILE_reader))
	{
		mod_music_error_number = MOD_MUSIC_ERROR_LOADING_HEADER;
		return (0);
	}

	/* set module variables */
//廃止	曲名文字列: 	mmoo.string_songname	  = DupStr(mh_it->songname, 26, 0); /* make a cstr of songname	*/
	mmoo.reppos	   = 0;
	mmoo.numpat	   = mh_it->patnum;
	mmoo.numins	   = mh_it->insnum;
	mmoo.numsmp	   = mh_it->smpnum;
	mmoo.initspeed   = mh_it->initspeed;
	mmoo.inittempo   = mh_it->inittempo;
	mmoo.initvolume  = mh_it->globvol;
	mmoo.flags	  |= UF_BGSLIDES | UF_ARPMEM;
//	if (!(mh_it->flags & 1))
//	{	mmoo.flags |= UF_PANNING;}
//	mmoo.bpmlimit = 32;

	if (mh_it->songname[25])
	{
		mmoo.numvoices = 1+mh_it->songname[25];
		#ifdef MIKMOD_DEBUG
		error(0/*ERR_FATAL*/, "Embedded IT limitation to %d voices\n", mmoo.numvoices);
		#endif
	}
	#if 0//廃止  (MikMODが判別した)モジュール名文字列:
	/* set the module type */
	{
		static const char *IT_Version[]=
		{
			"ImpulseTracker  .  ",		"Compressed ImpulseTracker  .  ",
			"ImpulseTracker 2.14p3",	"Compressed ImpulseTracker 2.14p3",
			"ImpulseTracker 2.14p4",	"Compressed ImpulseTracker 2.14p4",
		};
		/* 2.17 : IT 2.14p4 */
		/* 2.16 : IT 2.14p3 with resonant filters */
		/* 2.15 : IT 2.14p3 (improved compression) */
		if ((mh_it->cwt<=0x219)&&(mh_it->cwt>=0x217))
		{
			mmoo.string_modtype = strdup(IT_Version[mh_it->cmwt<0x214?4:5]);
		}
		else if (mh_it->cwt>=0x215)
		{
			mmoo.string_modtype = strdup(IT_Version[mh_it->cmwt<0x214?2:3]);
		}
		else
		{
			mmoo.string_modtype	   = strdup(IT_Version[mh_it->cmwt<0x214?0:1]);
			mmoo.string_modtype[mh_it->cmwt<0x214?15:26] = ((mh_it->cwt>>8)	  )+'0';
			mmoo.string_modtype[mh_it->cmwt<0x214?17:28] = ((mh_it->cwt>>4)&0x0f)+'0';
			mmoo.string_modtype[mh_it->cmwt<0x214?18:29] = ((mh_it->cwt	)&0x0f)+'0';
		}
	}
	#endif
	if (mh_it->flags&8)
	{	mmoo.flags |= (UF_XMPERIODS | UF_LINEAR);}

	if ((mh_it->cwt>=0x106)&&(mh_it->flags&16))
	{	old_effect = S3MIT_OLDSTYLE;}
	else
	{	old_effect = 0;}

	/* set panning positions */
//	if (mh_it->flags & 1)
//	{
		for (t=0; t<64; t++)
		{
			mh_it->pantable[t] &= 0x7f;
			/* 0 ... 63 の場合は、 0 ... 255 にマッピング*/
			if ((mh_it->pantable[t])<64)
			{	mmoo.panning[t] = ((mh_it->pantable[t])<<2);}
			else
			/* 64 の場合は、 255。多分(0 ... 64 が 0 ... 255 にマッピング) */
			if (mh_it->pantable[t]==64)
			{	mmoo.panning[t] = 255;}
			#if (1==USE_PAN_SURROUND)
			/* 100 の場合は、拡張形式のサラウンドモード。  */
			else
			if (mh_it->pantable[t]==100)
			{	mmoo.panning[t] = PAN_SURROUND;}
			#endif //(USE_PAN_SURROUND)
			/* 127 の場合は、中心(128)にする。	*/
			else
			if (mh_it->pantable[t]==127)
			{	mmoo.panning[t] = PAN_CENTER;}
			else
			{
				mod_music_error_number = MOD_MUSIC_ERROR_LOADING_HEADER;
				return (0);
			}
		}
//	}
//	else
//	{
//		for (t=0; t<64; t++)
//		{	mmoo.panning[t] = PAN_CENTER;}
//	}

	/* set channel volumes */
	memcpy(mmoo.chanvol, mh_it->voltable, 64);

	/* read the order data */
	if (!AllocPositions(mh_it->ordnum)) 	{	return (0); }
	origpositions = mod_music_calloc(mh_it->ordnum, sizeof(u16));
	if (!origpositions) 	{	return (0); }

	for (t=0; t<mh_it->ordnum; t++)
	{
		origpositions[t] = (u8)(music_fgetc());
		if ((origpositions[t]>(mh_it->patnum))&&(origpositions[t]<254))
		{	origpositions[t] = 255; }
	}

	if (mod_music_eof(mmff.mod_FILE_reader))
	{
		mod_music_error_number = MOD_MUSIC_ERROR_LOADING_HEADER;
		return (0);
	}

	mmff.mm_midi_position_look_up_counter = (u8)mh_it->ordnum;
	S3MIT_CreateOrders(curious);
	//
	paraptr_it = (u32*)mod_music_malloc((mh_it->insnum+mh_it->smpnum+mmoo.numpat)* sizeof(u32));
	if (!paraptr_it)
	{
		return (0);
	}
	/* read the instrument, sample, and pattern parapointers */
	mod_music_file_read_multi_u32LE(mmff.mod_FILE_reader, paraptr_it, mh_it->insnum+mh_it->smpnum+mmoo.numpat);

	if (mod_music_eof(mmff.mod_FILE_reader))
	{
		mod_music_error_number = MOD_MUSIC_ERROR_LOADING_HEADER;
		return (0);
	}

	/* Check for and load midi information for resonant filters */
	if ((mh_it->cmwt)>=0x216)
	{
		if ((mh_it->special)&8)
		{
			IT_LoadMidiConfiguration(1/*mmff.mod_FILE_reader*/);
			if (mod_music_eof(mmff.mod_FILE_reader))
			{
				mod_music_error_number = MOD_MUSIC_ERROR_LOADING_HEADER;
				return (0);
			}
		}
		else
		{	IT_LoadMidiConfiguration(0/*NULL*/);}
		#if (1==USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)
		mmff.mm_midi_use_resonant_filters = 1;
		#endif /*(USE_ITZ_ZXX_MIDI_RESONANT_FILTERS)*/
	}

	#if 0//廃止 曲のコメント文字列:
	/* Check for and load song comment */
	if ((mh_it->special&1)&&(mh_it->cwt>=0x104)&&(mh_it->msglength))
	{
		mod_music_file_seek_set(mmff.mod_FILE_reader, (long)(mh_it->msgoffset));
		if (!ReadComment(mh_it->msglength)) 	{return (0);/*(失敗)*/}
	}
	#endif

	if (!((mh_it->flags)&4)) mmoo.numins = mmoo.numsmp;
	if (!AllocSamples())	{	return (0);}

	if (!AllocLinear()) 	{	return (0);}

	/* Load all samples */
	{
		MM_SAMPLE *q;
		q = mmoo.samples;
		for (t=0; t<(mh_it->smpnum); t++)
		{
			ITSAMPLE s;
			/* seek to sample position */
			mod_music_file_seek_set(mmff.mod_FILE_reader, (long)(paraptr_it[mh_it->insnum+t]+4));
			/* load sample info */
			mod_music_file_read_string(mmff.mod_FILE_reader, s.filename, 12);
			s.zerobyte		= (u8)(music_fgetc());
			s.globvol		= (u8)(music_fgetc());
			s.flag			= (u8)(music_fgetc());
			s.volume		= (u8)(music_fgetc());
			mod_music_file_read_string(mmff.mod_FILE_reader, s.sampname, 26);
			s.convert		= (u8)(music_fgetc());
			s.panning		= (u8)(music_fgetc());
			s.length		= music_fget32();
			s.loopbeg		= music_fget32();
			s.loopend		= music_fget32();
			s.c5spd 		= music_fget32();
			s.susbegin		= music_fget32();
			s.susend		= music_fget32();
			s.sampoffset	= music_fget32();
			s.vibspeed		= (u8)(music_fgetc());
			s.vibdepth		= (u8)(music_fgetc());
			s.vibrate		= (u8)(music_fgetc());
			s.vibwave		= (u8)(music_fgetc());
			/* Generate an error if c5spd is > 512k, or samplelength > 256 megs
			   (nothing would EVER be that high) */
			if (	mod_music_eof(mmff.mod_FILE_reader) ||
				(s.c5spd   >   0x7ffffL) ||
				(s.length  > 0xfffffffUL) ||
				(s.loopbeg > 0xfffffffUL) ||
				(s.loopend > 0xfffffffUL)
			)
			{
				mod_music_error_number = MOD_MUSIC_ERROR_LOADING_SAMPLEINFO;
				return (0);
			}
			q->samplename	= DupStr(s.sampname, 26, 0);
			q->speed		= ((s.c5spd )>> 1);//((s.c5spd )/ 2)
		//	q->panning		= ((s.panning&0x7f)==64)?255:(s.panning&0x7f)<<2;
			q->panning		= panconvert64to255(s.panning);
			q->length		= s.length;
			q->loopstart	= s.loopbeg;
			q->loopend		= s.loopend;
			q->volume		= s.volume;
			q->globvol		= s.globvol;
			q->seekpos		= s.sampoffset;
			/* Convert speed to XM linear mm_fine_tune */
			if (mmoo.flags&UF_LINEAR)
			{	q->speed = speed_to_mm_fine_tune(s.c5spd, t);	}
			if (s.panning&128)	{q->flags |= SF_OWNPAN;}
			if (s.vibrate)
			{
				q->vibflags 	|= AV_IT;
				q->vibtype		= s.vibwave;
				q->vibsweep 	= s.vibrate * 2;
				q->vibdepth 	= s.vibdepth;
				q->vibrate		= s.vibspeed;
			}
			if (s.flag&2)			{q->flags |= SF_16BITS;}
			if ((s.flag&8)&&((mh_it->cwt)>=0x214))
			{
				q->flags |= SF_ITPACKED;
			/*	compressed = 1;*/
			}
			if (s.flag&16)			{q->flags |= SF_LOOP;}
			if (s.flag&64)			{q->flags |= SF_BIDI;}
			if ((mh_it->cwt)>=0x200)
			{
				if (s.convert&1)	{q->flags |= SF_SIGNED;}
				if (s.convert&4)	{q->flags |= SF_DELTA;}
			}
			q++;
		}
	}
	//
	/* Load instruments if instrument mode flag enabled */
	{
		MM_INSTRUMENT *d;
		if (mh_it->flags&4)
		{
			int skip_dummy; 	/* read skip dummy read. / ダミー読み込み用(読んだ値は使わない) */
			if (!AllocInstruments())	{return (0);}
			d = mmoo.instruments;
			mmoo.flags |= (UF_NNA|UF_INST);
			for (t=0; t<(mh_it->insnum); t++)
			{
				ITINSTHEADER ih;
				/* seek to instrument position */
				mod_music_file_seek_set(mmff.mod_FILE_reader, paraptr_it[t]+4);
				/* load instrument info */
				mod_music_file_read_string(mmff.mod_FILE_reader, ih.filename, 12);
				ih.zerobyte  = (u8)(music_fgetc());
				if ((mh_it->cwt)<0x200)
				{
					/* load IT 1.xx inst header */
					ih.volflg		= (u8)(music_fgetc());
					ih.volbeg		= (u8)(music_fgetc());
					ih.volend		= (u8)(music_fgetc());
					ih.volsusbeg	= (u8)(music_fgetc());
					ih.volsusend	= (u8)(music_fgetc());
					music_fget16();
					ih.fadeout		= music_fget16();
					ih.nna			= (u8)(music_fgetc());
					ih.dnc			= (u8)(music_fgetc());
				}
				else
				{
					/* Read IT200+ header */
					ih.nna			= (u8)(music_fgetc());
					ih.dct			= (u8)(music_fgetc());
					ih.dca			= (u8)(music_fgetc());
					ih.fadeout		= music_fget16();
					ih.ppsep		= (u8)(music_fgetc());
					ih.ppcenter 	= (u8)(music_fgetc());
					ih.globvol		= (u8)(music_fgetc());
					ih.chanpan		= (u8)(music_fgetc());
					ih.rvolvar		= (u8)(music_fgetc());
					ih.rpanvar		= (u8)(music_fgetc());
				}

				ih.trkvers			= music_fget16();
				ih.numsmp			= (u8)(music_fgetc());
				skip_dummy			= (u8)(music_fgetc());
				mod_music_file_read_string(mmff.mod_FILE_reader, ih.name, 26);
				mod_music_file_read_multi_u8(mmff.mod_FILE_reader, ih.blank01, 6);
				mod_music_file_read_multi_u16LE(mmff.mod_FILE_reader, ih.samptable, ITNOTECNT);
//	int lp;
#define lp uuu
				if ((mh_it->cwt)<0x200)
				{
					/* load IT 1xx volume envelope */
					mod_music_file_read_multi_u8(mmff.mod_FILE_reader, ih.volenv, 200);
					for (lp=0; lp<ITENVCNT; lp++)
					{
						ih.oldvoltick[lp]	= (u8)(music_fgetc());
						ih.volnode[lp]		= (u8)(music_fgetc());
					}
				}
				else
				{
					/* load IT 2xx volume, pan and pitch envelopes */
					ih.volflg			= (u8)(music_fgetc());
					ih.volpts			= (u8)(music_fgetc());
					ih.volbeg			= (u8)(music_fgetc());
					ih.volend			= (u8)(music_fgetc());
					ih.volsusbeg		= (u8)(music_fgetc());
					ih.volsusend		= (u8)(music_fgetc());
					for (lp=0; lp<ITENVCNT; lp++)
					{
						ih.volnode[lp]	= (u8)(music_fgetc());
						ih.voltick[lp]	= music_fget16();
					}
					skip_dummy			= (u8)(music_fgetc());
					ih.panflg			= (u8)(music_fgetc());
					ih.panpts			= (u8)(music_fgetc());
					ih.panbeg			= (u8)(music_fgetc());
					ih.panend			= (u8)(music_fgetc());
					ih.pansusbeg		= (u8)(music_fgetc());
					ih.pansusend		= (u8)(music_fgetc());
					for (lp=0; lp<ITENVCNT; lp++)
					{
						ih.pannode[lp]	= (s8)(music_fgetc());
						ih.pantick[lp]	= music_fget16();
					}
					skip_dummy			= (u8)(music_fgetc());
					ih.pitflg			= (u8)(music_fgetc());
					ih.pitpts			= (u8)(music_fgetc());
					ih.pitbeg			= (u8)(music_fgetc());
					ih.pitend			= (u8)(music_fgetc());
					ih.pitsusbeg		= (u8)(music_fgetc());
					ih.pitsusend		= (u8)(music_fgetc());
					for (lp=0; lp<ITENVCNT; lp++)
					{
						ih.pitnode[lp]	= (s8)(music_fgetc());
						ih.pittick[lp]	= music_fget16();
					}
					skip_dummy			= (u8)(music_fgetc());
				}
				if (mod_music_eof(mmff.mod_FILE_reader))
				{
					mod_music_error_number	= MOD_MUSIC_ERROR_LOADING_SAMPLEINFO;
					return (0);
				}

				d->volflg |= EF_VOLENV;
				d->insname = DupStr(ih.name, 26, 0);
				d->nnatype = ih.nna & NNA_MASK;

				if ((mh_it->cwt)<0x200)
				{
					d->volfade = (ih.fadeout<< 6);
					if (ih.dnc)
					{
						d->dct = DCT_NOTE;
						d->dca = DCA_CUT;
					}

					if (ih.volflg&1)	{d->volflg |= EF_ON;}
					if (ih.volflg&2)	{d->volflg |= EF_LOOP;}
					if (ih.volflg&4)	{d->volflg |= EF_SUSTAIN;}

					/* XM conversion of IT envelope Array */
					d->volbeg		= ih.volbeg;
					d->volend		= ih.volend;
					d->volsusbeg	= ih.volsusbeg;
					d->volsusend	= ih.volsusend;

					if (ih.volflg&1)
					{
						for (uuu=0; uuu<ITENVCNT; uuu++)
						{
							if (ih.oldvoltick[d->volpts]!=0xff)
							{
								d->volenv[d->volpts].val	= (ih.volnode[d->volpts]<<2);
								d->volenv[d->volpts].pos	= (ih.oldvoltick[d->volpts]);
								d->volpts++;
							}
							else
							{
								break;
							}
						}
					}
				}
				else
				{
				//	d->panning = ((ih.chanpan&0x7f)==64)?(255):((ih.chanpan&0x7f)<<2);
					d->panning = panconvert64to255(ih.chanpan);
					if (!(ih.chanpan&128))	{d->flags |= IF_OWNPAN;}
					if (!(ih.ppsep & 128))
					{
						d->pitpansep		= ih.ppsep<<2;
						d->pitpancenter 	= ih.ppcenter;
						d->flags			|= IF_PITCHPAN;
					}
					d->globvol	= (ih.globvol>>1);
					d->volfade	= (ih.fadeout<<5);
					d->dct		= ih.dct;
					d->dca		= ih.dca;
					if (mh_it->cwt >= 0x204)
					{
						d->rvolvar = ih.rvolvar;
						d->rpanvar = ih.rpanvar;
					}
	//
					if (ih.volflg&1)	{	d->volflg |=EF_ON;}
					if (ih.volflg&2)	{	d->volflg |=EF_LOOP;}
					if (ih.volflg&4)	{	d->volflg |=EF_SUSTAIN;}
					d->volpts		= ih.volpts;
					d->volbeg		= ih.volbeg;
					d->volend		= ih.volend;
					d->volsusbeg	= ih.volsusbeg;
					d->volsusend	= ih.volsusend;
					for (uuu=0; uuu<(ih.volpts); uuu++) 	{ d->volenv[uuu].pos = ih.voltick[uuu];}
					if ((d->volflg&EF_ON)&&(d->volpts<2))	{ d->volflg &= ~EF_ON;}
					for (uuu=0; uuu<ih.volpts; uuu++)		{ d->volenv[uuu].val = (ih.volnode[uuu]<<2);}
	//
					if (ih.panflg&1)	{d->panflg |=EF_ON;}
					if (ih.panflg&2)	{d->panflg |=EF_LOOP;}
					if (ih.panflg&4)	{d->panflg |=EF_SUSTAIN;}
					d->panpts		= ih.panpts;
					d->panbeg		= ih.panbeg;
					d->panend		= ih.panend;
					d->pansusbeg	= ih.pansusbeg;
					d->pansusend	= ih.pansusend;
					for (uuu=0; uuu<(ih.panpts); uuu++) 	{ d->panenv[uuu].pos = ih.pantick[uuu];}
					if ((d->panflg&EF_ON)&&(d->panpts<2))	{ d->panflg &= ~EF_ON;}
					for (uuu=0; uuu<(ih.panpts); uuu++) 	{ d->panenv[uuu].val = (ih.pannode[uuu]==32)?(255):((ih.pannode[uuu]+32)<<2);}
	//
					if (ih.pitflg&1)	{d->pitflg |=EF_ON;}
					if (ih.pitflg&2)	{d->pitflg |=EF_LOOP;}
					if (ih.pitflg&4)	{d->pitflg |=EF_SUSTAIN;}
					d->pitpts		= ih.pitpts;
					d->pitbeg		= ih.pitbeg;
					d->pitend		= ih.pitend;
					d->pitsusbeg	= ih.pitsusbeg;
					d->pitsusend	= ih.pitsusend;
					for (uuu=0; uuu<ih.pitpts; uuu++)		{ d->pitenv[uuu].pos = ih.pittick[uuu];}
					if ((d->pitflg&EF_ON)&&(d->pitpts<2))	{ d->pitflg &= ~EF_ON;}
					for (uuu=0; uuu<ih.pitpts; uuu++)		{ d->pitenv[uuu].val = ih.pitnode[uuu] + 32;}
	//
					if (ih.pitflg&0x80)
					{
						/* filter envelopes not supported yet */
						d->pitflg &= ~EF_ON;
						ih.pitpts = ih.pitbeg = ih.pitend = 0;
						#ifdef MIKMOD_DEBUG
						{
							static int warn = 0;
							if (!warn)
							{	error(0/*ERR_FATAL*/, "Filter envelopes not supported yet\n");}
							warn = 1;
						}
						#endif
					}
				}
				for (uuu=0; uuu<ITNOTECNT; uuu++)
				{
					d->samplenote[uuu]		= (ih.samptable[uuu]&255);
					d->samplenumber[uuu]	= (ih.samptable[uuu]>>8)?((ih.samptable[uuu]>>8)-1):0xffff;
					if (d->samplenumber[uuu] >= mmoo.numsmp)
					{	d->samplenote[uuu] = 255;}
					else if (mmoo.flags & UF_LINEAR)
					{
						int note	= (int)d->samplenote[uuu]+noteindex[d->samplenumber[uuu]];
						d->samplenote[uuu] = (note<0)?(0):((note>255)?(255):(note));
					}
				}
				d++;
			}
		}
		else
		if (mmoo.flags & UF_LINEAR)
		{
			if (!AllocInstruments())	{return (0);}
			d = mmoo.instruments;
			mmoo.flags |= UF_INST;
			for (t=0; t<(mh_it->smpnum); t++, d++)
			{
				for (uuu=0; uuu<ITNOTECNT; uuu++)
				{
					if ((d->samplenumber[uuu]) >= mmoo.numsmp)
					{	d->samplenote[uuu] = 255;}
					else
					{
						int note = (int)d->samplenote[uuu]+noteindex[(d->samplenumber[uuu])];
						d->samplenote[uuu] = (note<0)?(0):((note>255)?(255):(note));
					}
				}
			}
		}
	}
	/* Figure out how many channels this song actually uses */
	mmoo.numchn = 0;
	memset(mmff.re_map, -1, (UF_MAXCHAN*sizeof(u8)) );
	for (t=0; t<mmoo.numpat; t++)
	{
		u16 packlen;
		/* seek to pattern position */
		int aaa;
		aaa = paraptr_it[(mh_it->insnum)+(mh_it->smpnum)+(t)];
		if (aaa)	/* 0 -> empty 64 row pattern */
		{
			mod_music_file_seek_set(mmff.mod_FILE_reader, ((long)aaa));
			music_fget16();
			/* read pattern length (# of rows)
			   Impulse Tracker never creates patterns with less than 32 rows,
			   but some other trackers do, so we only check for more than 256
			   rows */
			packlen = music_fget16();
			if (packlen>256)
			{
				mod_music_error_number = MOD_MUSIC_ERROR_LOADING_PATTERN;
				return (0);
			}
			music_fget32();
			if (IT_GetNumChannels(packlen)) 	{return (0);}
		}
	}
	/* give each of them a different number */
	for (t=0; t<UF_MAXCHAN; t++)
	{
		if (!mmff.re_map[t])
		{	mmff.re_map[t] = mmoo.numchn++; }
	}
	mmoo.numtrk = mmoo.numpat*mmoo.numchn;
	if (mmoo.numvoices)
	{	if (mmoo.numvoices < mmoo.numchn) 	{mmoo.numvoices = mmoo.numchn;}}
	if (!AllocPatterns())	{return (0);}
	if (!AllocTracks()) 	{return (0);}
	for (t=0; t<mmoo.numpat; t++)
	{
		u16 packlen;
		/* seek to pattern position */
		int aaa;
		aaa = paraptr_it[(mh_it->insnum)+(mh_it->smpnum)+(t)];
		if (!aaa) /* 0 -> empty 64 row pattern */
		{
			mmoo.pattrows[t] = 64;
			for (uuu=0; uuu<mmoo.numchn; uuu++)
			{
				UniReset();
				{int k;
				for (k=0; k<64; k++)	{UniNewline();}}
				mmoo.tracks[numtrk++] = UniDup();
			}
		}
		else
		{
			mod_music_file_seek_set(mmff.mod_FILE_reader, ((long)aaa));
			packlen 		= music_fget16();
			mmoo.pattrows[t]	= music_fget16();
			music_fget32();
			if (!IT_ReadPattern(mmoo.pattrows[t]))	{return (0);}
		}
	}
	return (1);/*(成功)*/
}

#if 0/*曲名廃止*/
MM_s8 *IT_LoadTitle(void)
{
	MM_s8 s[26];
	mod_music_file_seek_set(mmff.mod_FILE_reader, 4);
	mod_music_file_read_multi_u8(mmff.mod_FILE_reader, s, 26);
//	if (0==)	{return (NULL);}
	return (DupStr(s, 26, 0));
}
#endif
/*---------- Loader information */

MOD_MUSIC_API MOD_MUSIC_INTERNAL_LOADER load_it =
{
	NULL,
	"IT",
	"IT (Impulse Tracker)",
	IT_Init,
	IT_Test,
	IT_Load,
	IT_Cleanup,
//	/*曲名廃止*/	IT_LoadTitle
};

/* ex:set ts=4: */
