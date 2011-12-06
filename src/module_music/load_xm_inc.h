
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	"*.XM"形式のファイルを読み込んで、ユニバーサル形式(MikMODの再生形式)に変換します。
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
	-------------------------------------------------------
	このファイルは "mlreg.c" からインクルードされます。
	-------------------------------------------------------
	$Id: load_xm.c,v 1.7 2001/12/19 17:11:40 slouken Exp $
	Fasttracker (XM) module loader
---------------------------------------------------------*/

//#ifdef HA VE_CONFIG_H
//#include "co nfig.h"
//#endif

#include <string.h>

#include "mohou_mikmod_internals.h"

/*---------- Module structure */

typedef struct XMHEADER
{								/*               012345678901234567 */
	MM_s8  id[17];			/* ID text:     'Extended module: ' ID */
	MM_s8  songname[21];		/* Module name: '012345678901234567890' 曲名(但しシフトjisコード対象外) */
	MM_s8  trackername[20];	/* Tracker name:'01234567890123456789'  作曲ソフト名(但しシフトjisコード対象外) */
	u16 version;				/* Version number */
	u32 headersize; 			/* Header size */
	u16 songlength; 			/* Song length (in patten order table) */
	u16 restart;				/* Restart position */
	u16 numchn; 				/* Number of channels (2,4,6,8,10,...,32) */
	u16 numpat; 				/* Number of patterns (max 256) */
	u16 numins; 				/* Number of instruments (max 128) */
	u16 flags;
	u16 tempo;					/* Default tempo */
	u16 bpm;					/* Default BPM */
	u8 orders[256]; 			/* Pattern order table	*/
} XMHEADER;

typedef struct XMINSTHEADER
{
	u32 size;					/* Instrument size */
	MM_s8  name[22];			/* Instrument name */
	u8 type;					/* Instrument type (always 0) */
	u16 numsmp; 				/* Number of samples in instrument */
	u32 ssize;
} XMINSTHEADER;

#define XMENVCNT_2	(12)
#define XMENVCNT_1	(XMENVCNT_2*2)

#define XMNOTECNT (8*OCTAVE)
typedef struct XMPATCHHEADER
{
	u8 what[XMNOTECNT]; 		/* Sample number for all notes */
	u16 volenv[(XMENVCNT_1)];	/* Points for volume envelope */
	u16 panenv[(XMENVCNT_1)];	/* Points for panning envelope */
	u8 volpts;					/* Number of volume points */
	u8 panpts;					/* Number of panning points */
	u8 volsus;					/* Volume sustain point */
	u8 volbeg;					/* Volume loop start point */
	u8 volend;					/* Volume loop end point */
	u8 pansus;					/* Panning sustain point */
	u8 panbeg;					/* Panning loop start point */
	u8 panend;					/* Panning loop end point */
	u8 volflg;					/* Volume type: bit 0: On; 1: Sustain; 2: Loop */
	u8 panflg;					/* Panning type: bit 0: On; 1: Sustain; 2: Loop */
	u8 vibflg;					/* Vibrato type */
	u8 vibsweep;				/* Vibrato sweep */
	u8 vibdepth;				/* Vibrato depth */
	u8 vibrate; 				/* Vibrato rate */
	u16 volfade;				/* Volume fadeout */
} XMPATCHHEADER;

typedef struct XMWAVHEADER
{
	u32 length; 				/* Sample length */
	u32 loopstart;				/* Sample loop start */
	u32 looplength; 			/* Sample loop length */
	u8 volume;					/* Volume  */
	s8 mm_fine_tune;			/* Finetune (signed byte -128..+127) */
	u8 type;					/* Loop type */
	u8 panning; 				/* Panning (0-255) */
	s8 relnote; 				/* Relative note number (signed byte) */
	u8 reserved;
	MM_s8  samplename[22];	/* Sample name */
	u8 vibtype; 				/* Vibrato type */
	u8 vibsweep;				/* Vibrato sweep */
	u8 vibdepth;				/* Vibrato depth */
	u8 vibrate; 				/* Vibrato rate */
} XMWAVHEADER;

typedef struct XMPATHEADER
{
	u32 size;		/* Pattern header length  */
	u8 packing; 	/* Packing type (always 0) */
	u16 numrows;	/* Number of rows in pattern (1..256) */
	s16 packsize;	/* Packed patterndata size */
} XMPATHEADER;

typedef struct XMNOTE
{
	u8 note;
	u8 ins;
	u8 vol;
	u8 eff;
	u8 dat;
} XMNOTE;

/*---------- Loader variables */

static	XMNOTE *xmpat		= NULL;
static	XMHEADER *mh_xm 	= NULL;

/* increment unit for sample array reallocation */
#define XM_SMPINCR 64
static	u32 *nextwav	= NULL;
static	XMWAVHEADER *wh 	= NULL;
static	XMWAVHEADER *s		= NULL;

/*---------- Loader code */

MM_BOOL XM_Test(void)
{
	u8 id[38];
	mod_music_file_read_multi_u8(mmff.mod_FILE_reader, id, 38);
//	if (0==) {	return (0);}
	if (memcmp(id, "Extended Module: ", 17))		{	return (0);}
	if (id[37]==0x1a)	{	return (1);}
	return (0);
}

MM_BOOL XM_Init(void)
{
	mh_xm = (XMHEADER *)mod_music_malloc(sizeof(XMHEADER));
	if (!mh_xm);
	{
		return (0);
	}
	return (1);
}

void XM_Cleanup(void)
{
	mod_music_free(mh_xm);
}

static int XM_ReadNote(XMNOTE* n)
{
	u8 cmp,result=1;

	memset(n,0,sizeof(XMNOTE));
	cmp = (u8)(music_fgetc());

	if (cmp&0x80)
	{
		if (cmp&1)		{ result++;n->note = (u8)(music_fgetc()); }
		if (cmp&2)		{ result++;n->ins  = (u8)(music_fgetc()); }
		if (cmp&4)		{ result++;n->vol  = (u8)(music_fgetc()); }
		if (cmp&8)		{ result++;n->eff  = (u8)(music_fgetc()); }
		if (cmp&16) 	{ result++;n->dat  = (u8)(music_fgetc()); }
	}
	else
	{
		n->note = cmp;
		n->ins	= (u8)(music_fgetc());
		n->vol	= (u8)(music_fgetc());
		n->eff	= (u8)(music_fgetc());
		n->dat	= (u8)(music_fgetc());
		result += 4;
	}
	return (result);
}

static u8* XM_Convert(XMNOTE* xmtrack, u16 rows)
{
	UniReset();
	//
	int t;
	for (t=0; t<rows; t++)
	{
		u8 note;
		u8 ins;
		u8 vol;
		u8 eff;
		u8 dat;
		note	= xmtrack->note;
		ins 	= xmtrack->ins;
		vol 	= xmtrack->vol;
		eff 	= xmtrack->eff;
		dat 	= xmtrack->dat;

		if (note)
		{
			if (note>XMNOTECNT)
			{	UniEffect(U231_ICODE_0x20_KEYFADE,0);}
			else
			{	UniNote(note-1);}
		}
		if (ins) UniInstrument(ins-1);

		switch (vol>>4)
		{
		case 0x6: /* volslide down */
			if (vol&0xf)	{UniEffect(U231_ICODE_0x23_XMEFFECTA,vol&0xf);}
			break;
		case 0x7: /* volslide up */
			if (vol&0xf)	{UniEffect(U231_ICODE_0x23_XMEFFECTA,vol<<4);}
			break;
			/* volume-row fine volume slide is compatible with protracker
			   EBx and EAx effects i.e. a zero nibble means DO NOT SLIDE, as
			   opposed to 'take the last sliding value'. */
		case 0x8: /* finevol down */
			UniPTEffect(0xe,0xb0|(vol&0xf));
			break;
		case 0x9: /* finevol up */
			UniPTEffect(0xe,0xa0|(vol&0xf));
			break;
		case 0xa: /* set vibrato speed */
			UniPTEffect(0x4,vol<<4);
			break;
		case 0xb: /* vibrato */
			UniPTEffect(0x4,vol&0xf);
			break;
		case 0xc: /* set panning */
			UniPTEffect(0x8,vol<<4);
			break;
		case 0xd: /* panning slide left (only slide when data not zero) */
			if (vol&0xf) UniEffect(U231_ICODE_0x2b_XMEFFECTP,vol&0xf);
			break;
		case 0xe: /* panning slide right (only slide when data not zero) */
			if (vol&0xf) UniEffect(U231_ICODE_0x2b_XMEFFECTP,vol<<4);
			break;
		case 0xf: /* tone porta */
			UniPTEffect(0x3,vol<<4);
			break;
		default:
			if ((vol>=0x10)&&(vol<=0x50))
				UniPTEffect(0xc,vol-0x10);
		}

		switch (eff)
		{
			case 0x4:			UniEffect(U231_ICODE_0x14_XMEFFECT4,dat);				break;
			case 0xa:			UniEffect(U231_ICODE_0x23_XMEFFECTA,dat);				break;
			case 0xe: /* Extended effects */	/* 拡張 MOD コマンド */
				switch (dat>>4)
				{
				case 0x1:		UniEffect(U231_ICODE_0x24_XMEFFECTE1,dat&0xf);			break; /* XM fine Portamento up */
				case 0x2:		UniEffect(U231_ICODE_0x25_XMEFFECTE2,dat&0xf);			break; /* XM fine Portamento down */
			//	case 0x3:		// E3x: Grissando Control
			//	case 0x4:		// E4x: Vibrato waveform
			//	case 0x5:		// E5x: Set Finetune
			//	case 0x6:		// E6x: Pattern loop
			//	case 0x7:		// E7x: Tremolo waveform
			//	case 0x8:		// E8x: Set Pannning
			//	case 0x9:		// E9x: Retrigger note
				case 0xa:		UniEffect(U231_ICODE_0x26_XMEFFECTEA,dat&0xf);			break; /* XM fine volume slide up */
				case 0xb:		UniEffect(U231_ICODE_0x27_XMEFFECTEB,dat&0xf);			break; /* XM fine volume slide down */
			//	case 0xc:		// ECx: Note cut
			//	case 0xd:		// EDx: Note delay
			//	case 0xe:		// EEx: Pattern delay
			//	case 0xf:		// EFx: Set active macro
				default:		UniPTEffect(eff, dat);
				}
				break;
			case 'G'-55:		{int temp_dat = dat;	temp_dat = psp_min(temp_dat, 64);	dat = temp_dat;}
								UniEffect(U231_ICODE_0x28_XMEFFECTG,dat);				break; /* G - set global volume */
			case 'H'-55:		UniEffect(U231_ICODE_0x29_XMEFFECTH,dat);				break; /* H - global volume slide */
			case 'K'-55:		UniEffect(U231_ICODE_0x20_KEYFADE,dat); 				break; /* K - keyOff and KeyFade */
			case 'L'-55:		UniEffect(U231_ICODE_0x2a_XMEFFECTL,dat);				break; /* L - set envelope position */
			case 'P'-55:		UniEffect(U231_ICODE_0x2b_XMEFFECTP,dat);				break; /* P - panning slide */
			case 'R'-55:		UniEffect(U231_ICODE_0x35_S3MEFFECTQ,dat);				break; /* R - multi retrig note */
			case 'T'-55:		UniEffect(U231_ICODE_0x34_S3MEFFECTI,dat);				break; /* T - Tremor */
			case 'X'-55:
				switch (dat>>4)
				{
				case 1: 		UniEffect(U231_ICODE_0x2c_XMEFFECTX1,dat&0xf);			break; /* X1 - Extra Fine Porta up */
				case 2: 		UniEffect(U231_ICODE_0x2d_XMEFFECTX2,dat&0xf);			break; /* X2 - Extra Fine Porta down */
				}
				break;
			case 'Z'-55:		UniEffect(U231_ICODE_0x2f_XMEFFECTZ,dat);				break; /* Z - synchro */
			default:
				if (eff<=0xf)
				{
					/* the pattern jump destination is written in decimal,
					   but it seems some poor tracker software writes them
					   in hexadecimal... (sigh) */
					if (eff==0xd)
					{	/* don't change anything if we're sure it's in hexa */
						if ((((dat&0xf0)>>4)<=9)&&((dat&0xf)<=9))
							/* otherwise, convert from dec to hex */
						{	dat = (((dat&0xf0)>>4)*10)+(dat&0xf);}
					}
					UniPTEffect(eff,dat);
				}
				break;
		}
		UniNewline();
		xmtrack++;
	}
	return UniDup();
}

static MM_BOOL LoadPatterns(MM_BOOL dummypat)
{
	int t,u,v,numtrk;

	if (!AllocTracks()) 	{	return (0);}
	if (!AllocPatterns())	{	return (0);}

	numtrk = 0;
	for (t=0; t<mh_xm->numpat; t++)
	{
		XMPATHEADER ph;
		ph.size 	= music_fget32();
		if (ph.size<(mh_xm->version==0x0102?8:9))
		{
			mod_music_error_number=MOD_MUSIC_ERROR_LOADING_PATTERN;
			return (0);
		}
		ph.packing	= (u8)(music_fgetc());
		if (ph.packing)
		{
			mod_music_error_number=MOD_MUSIC_ERROR_LOADING_PATTERN;
			return (0);
		}
		if (mh_xm->version==0x0102)
			ph.numrows	= (u8)(music_fgetc()) + 1;
		else
			ph.numrows	= music_fget16();
		ph.packsize = music_fget16();

		ph.size -= (mh_xm->version==0x0102?8:9);
		if (ph.size)
		{	mod_music_file_seek_cur(mmff.mod_FILE_reader, ph.size);
		}
		mmoo.pattrows[t]=ph.numrows;

		if (ph.numrows)
		{
			xmpat = (XMNOTE*)mod_music_calloc(ph.numrows*mmoo.numchn, sizeof(XMNOTE));
			if (!xmpat)
			{	return (0);
			}
			/* when packsize is 0, don't try to load a pattern.. it's empty. */
			if (ph.packsize)
			{	for (u=0; u<ph.numrows; u++)
				{	for (v=0; v<mmoo.numchn; v++)
					{
						if (!ph.packsize)	{break;}
						ph.packsize -= XM_ReadNote(&xmpat[(v*ph.numrows)+u]);
						if (ph.packsize<0)
						{
							free(xmpat);xmpat = NULL;
							mod_music_error_number = MOD_MUSIC_ERROR_LOADING_PATTERN;
							return (0);
						}
					}
				}
			}
			if (ph.packsize)
			{
				mod_music_file_seek_cur(mmff.mod_FILE_reader, ph.packsize);
			}

			if (mod_music_eof(mmff.mod_FILE_reader))
			{
				free(xmpat);xmpat = NULL;
				mod_music_error_number = MOD_MUSIC_ERROR_LOADING_PATTERN;
				return (0);
			}

			for (v=0; v<(mmoo.numchn); v++)
			{	mmoo.tracks[numtrk++] = XM_Convert(&xmpat[v*ph.numrows],ph.numrows);
			}
			free(xmpat);xmpat = NULL;
		}
		else
		{
			for (v=0; v<(mmoo.numchn); v++)
			{	mmoo.tracks[numtrk++]=XM_Convert(NULL,ph.numrows);
			}
		}
	}

	if (dummypat)
	{
		mmoo.pattrows[t] = 64;
		xmpat = (XMNOTE*)mod_music_calloc(64*mmoo.numchn, sizeof(XMNOTE));
		if (!xmpat) 	{return (0);}
		for (v=0; v<(mmoo.numchn); v++)
		{	mmoo.tracks[numtrk++] = XM_Convert(&xmpat[v*64],64);
		}
		free(xmpat);xmpat = NULL;
	}
	return (1);
}

static MM_BOOL LoadInstruments(void)
{
	int t,u;
	MM_INSTRUMENT *d;
	u32 next = 0;
	u16 wavcnt = 0;

	if (!AllocInstruments())	{	return (0); }
	d = mmoo.instruments;
	for (t=0; t<(mmoo.numins); t++,d++)
	{
		XMINSTHEADER ih;
		long headend;

		memset(d->samplenumber, 0xff, INSTNOTES*sizeof(u16));

		/* read instrument header */
		headend 	= mod_music_ftell(mmff.mod_FILE_reader);
		ih.size 	= music_fget32();
		headend    += ih.size;
		mod_music_file_read_string(mmff.mod_FILE_reader, ih.name, 22);
		ih.type 	= (u8)(music_fgetc());
		ih.numsmp	= music_fget16();

		d->insname	= DupStr(ih.name, 22, 1);

		if ((s16)ih.size>29)
		{
			ih.ssize	= music_fget32();
			if (((s16)ih.numsmp>0)&&(ih.numsmp<=XMNOTECNT))
			{
				XMPATCHHEADER pth;
				int p;
				mod_music_file_read_multi_u8(	mmff.mod_FILE_reader, pth.what,	 (XMNOTECNT ));
				mod_music_file_read_multi_u16LE(	mmff.mod_FILE_reader, pth.volenv, (XMENVCNT_1));
				mod_music_file_read_multi_u16LE(	mmff.mod_FILE_reader, pth.panenv, (XMENVCNT_1));
				pth.volpts		= (u8)(music_fgetc());
				pth.panpts		= (u8)(music_fgetc());
				pth.volsus		= (u8)(music_fgetc());
				pth.volbeg		= (u8)(music_fgetc());
				pth.volend		= (u8)(music_fgetc());
				pth.pansus		= (u8)(music_fgetc());
				pth.panbeg		= (u8)(music_fgetc());
				pth.panend		= (u8)(music_fgetc());
				pth.volflg		= (u8)(music_fgetc());
				pth.panflg		= (u8)(music_fgetc());
				pth.vibflg		= (u8)(music_fgetc());
				pth.vibsweep	= (u8)(music_fgetc());
				pth.vibdepth	= (u8)(music_fgetc());
				pth.vibrate 	= (u8)(music_fgetc());
				pth.volfade 	= music_fget16();

				/* read the remainder of the header
				   (2 bytes for 1.03, 22 for 1.04) */
				for (u=headend-mod_music_ftell(mmff.mod_FILE_reader); u; u--)
				{	(u8)(music_fgetc());
				}
				/* we can't trust the envelope point count here, as some
				   modules have incorrect values (K_OSPACE.XM reports 32 volume
				   points, for example). */
				if (pth.volpts>(XMENVCNT_2))	{pth.volpts=(XMENVCNT_2);}
				if (pth.panpts>(XMENVCNT_2))	{pth.panpts=(XMENVCNT_2);}

				if ((mod_music_eof(mmff.mod_FILE_reader))||(pth.volpts>(XMENVCNT_2))||(pth.panpts>(XMENVCNT_2)))
				{
					if (nextwav)	{ free(nextwav);nextwav = NULL; }
					if (wh) 		{ free(wh);wh = NULL; }
					mod_music_error_number = MOD_MUSIC_ERROR_LOADING_SAMPLEINFO;
					return (0);
				}

				for (u=0; u<XMNOTECNT; u++)
				{	d->samplenumber[u]=pth.what[u]+mmoo.numsmp;}
				d->volfade = pth.volfade;
				#if 1
				memcpy(d->volenv, pth.volenv, (XMENVCNT_1));
				if (pth.volflg&1)	{d->volflg |= EF_ON;}
				if (pth.volflg&2)	{d->volflg |= EF_SUSTAIN;}
				if (pth.volflg&4)	{d->volflg |= EF_LOOP;}
				d->volsusbeg=d->volsusend=pth.volsus;
				d->volbeg=pth.volbeg;
				d->volend=pth.volend;
				d->volpts=pth.volpts;
				/* scale envelope */
				for (p=0; p<(XMENVCNT_2); p++)
				{	d->volenv[p].val <<= 2;}
				if ((d->volflg&EF_ON)&&((d->volpts)<2))
				{	d->volflg &= ~EF_ON;}
				#endif
				#if 1
				memcpy(d->panenv, pth.panenv, (XMENVCNT_1));
				if (pth.panflg&1)	{d->panflg |= EF_ON;}
				if (pth.panflg&2)	{d->panflg |= EF_SUSTAIN;}
				if (pth.panflg&4)	{d->panflg |= EF_LOOP;}
				d->pansusbeg=d->pansusend=pth.pansus;
				d->panbeg=pth.panbeg;
				d->panend=pth.panend;
				d->panpts=pth.panpts;
				/* scale envelope */
				for (p=0; p<(XMENVCNT_2); p++)
				{	d->panenv[p].val <<= 2;}
				if ((d->panflg&EF_ON)&&((d->panpts)<2))
				{	d->panflg &= ~EF_ON;}
				#endif
				/* Samples are stored outside the instrument struct now, so we
				   have to load them all into a temp area, count the mmoo.numsmp
				   along the way and then do an AllocSamples() and move
				   everything over */
				if ((mh_xm->version)>0x0103)	{	next = 0;}
				for (u=0; u<(ih.numsmp); u++,s++)
				{
					/* Allocate more room for sample information if necessary */
					if (mmoo.numsmp+u==wavcnt)
					{
						wavcnt += XM_SMPINCR;
						nextwav = realloc(nextwav, wavcnt*sizeof(u32));
						if (!nextwav)
						{
							if (wh) 	{ free(wh);wh = NULL; }
							mod_music_error_number = MOD_MUSIC_ERROR_OUT_OF_MEMORY;
							return (0);
						}
						wh = realloc(wh, wavcnt*sizeof(XMWAVHEADER));
						if (!wh)
						{
							free(nextwav);nextwav = NULL;
							mod_music_error_number = MOD_MUSIC_ERROR_OUT_OF_MEMORY;
							return (0);
						}
						s = wh+(wavcnt-XM_SMPINCR);
					}

					s->length			= music_fget32();
					s->loopstart		= music_fget32();
					s->looplength		= music_fget32();
					s->volume			= (u8)(music_fgetc());
					s->mm_fine_tune 	= (s8)(music_fgetc());
					s->type 			= (u8)(music_fgetc());
					s->panning			= (u8)(music_fgetc());
					s->relnote			= (s8)(music_fgetc());
					s->vibtype			= pth.vibflg;
					s->vibsweep 		= pth.vibsweep;
					s->vibdepth 		= pth.vibdepth*4;
					s->vibrate			= pth.vibrate;
					s->reserved 		= (u8)(music_fgetc());
					mod_music_file_read_string(mmff.mod_FILE_reader, s->samplename, 22);

					nextwav[mmoo.numsmp+u] = next;
					next += s->length;

					if (mod_music_eof(mmff.mod_FILE_reader))
					{
						free(nextwav);nextwav = NULL;
						free(wh);wh = NULL;
						mod_music_error_number = MOD_MUSIC_ERROR_LOADING_SAMPLEINFO;
						return (0);
					}
				}

				if (mh_xm->version>0x0103)
				{
					for (u=0; u<ih.numsmp; u++)
					{	nextwav[mmoo.numsmp++] += mod_music_ftell(mmff.mod_FILE_reader);
					}
					mod_music_file_seek_cur(mmff.mod_FILE_reader, next);
				}
				else
				{	mmoo.numsmp += ih.numsmp;
				}
			}
			else
			{
				/* read the remainder of the header */
				for (u=headend-mod_music_ftell(mmff.mod_FILE_reader); u; u--)	{(u8)(music_fgetc());}

				if (mod_music_eof(mmff.mod_FILE_reader))
				{
					free(nextwav);nextwav = NULL;
					free(wh);wh = NULL;
					mod_music_error_number = MOD_MUSIC_ERROR_LOADING_SAMPLEINFO;
					return (0);
				}
			}
		}
	}

	/* sanity check */
	if (!mmoo.numsmp)
	{
		if (nextwav)	{ free(nextwav);nextwav = NULL; }
		if (wh) 		{ free(wh);wh = NULL; }
		mod_music_error_number = MOD_MUSIC_ERROR_LOADING_SAMPLEINFO;
		return (0);
	}
	return (1);
}

MM_BOOL XM_Load(MM_BOOL curious)
{
	MM_INSTRUMENT *d;
	MM_SAMPLE *q;
	int t,u;
	MM_BOOL dummypat = 0;
	#define USE_TRACKER_NAME_COPY (0)
	#if (1==USE_TRACKER_NAME_COPY)
	char tracker[21];
	char modtype[64/*60*/];
	#endif

	/* try to read module header */
	mod_music_file_read_string(mmff.mod_FILE_reader, mh_xm->id,			17);
	mod_music_file_read_string(mmff.mod_FILE_reader, mh_xm->songname,	21);
	mod_music_file_read_string(mmff.mod_FILE_reader, mh_xm->trackername, 20);
	mh_xm->version	   = music_fget16();
	if ((mh_xm->version<0x102)||(mh_xm->version>0x104))
	{
		mod_music_error_number=MOD_MUSIC_ERROR_NOT_A_MOD_MUSIC_MODULE;
		return (0);
	}
	mh_xm->headersize	= music_fget32();
	mh_xm->songlength	= music_fget16();
	mh_xm->restart		= music_fget16();
	mh_xm->numchn		= music_fget16();
	mh_xm->numpat		= music_fget16();
	mh_xm->numins		= music_fget16();
	mh_xm->flags		= music_fget16();
	mh_xm->tempo		= music_fget16();
	mh_xm->bpm			= music_fget16();
	if (!mh_xm->bpm)
	{
		mod_music_error_number = MOD_MUSIC_ERROR_NOT_A_MOD_MUSIC_MODULE;
		return (0);
	}
	mod_music_file_read_multi_u8(mmff.mod_FILE_reader, mh_xm->orders, 256);

	if (mod_music_eof(mmff.mod_FILE_reader))
	{
		mod_music_error_number = MOD_MUSIC_ERROR_LOADING_HEADER;
		return (0);
	}

	/* set module variables */
	mmoo.initspeed = (u8)mh_xm->tempo;
	mmoo.inittempo = (u8)mh_xm->bpm;

	#if (1==USE_TRACKER_NAME_COPY)
	strncpy(tracker, mh_xm->trackername, 20);	tracker[20] = 0;
	for (t=20; (tracker[t]<=' ')&&(t>=0); t--)	{tracker[t] = 0;}
	/* some modules have the tracker name empty */
	if (!tracker[0])
	{	strcpy(tracker, "Unknown tracker");
	}
	#ifdef HAVE_SNPRINTF
	snprintf(modtype, 60,	"%s (XM format %d.%02d)",	tracker, mh_xm->version>>8, mh_xm->version&0xff);
	#else
	sprintf(modtype,		"%s (XM format %d.%02d)",	tracker, mh_xm->version>>8, mh_xm->version&0xff);
	#endif
//廃止		mmoo.string_modtype		= strdup(modtype);
	#endif

	mmoo.numchn		= mh_xm->numchn;
	mmoo.numpat		= mh_xm->numpat;
	mmoo.numtrk		= (u16)mmoo.numpat*mmoo.numchn; /* get number of channels */
//廃止	曲名文字列: 		mmoo.string_songname	= DupStr(mh_xm->songname,20,1);
	mmoo.numpos		= mh_xm->songlength;				/* copy the songlength */
	mmoo.reppos		= mh_xm->restart<mh_xm->songlength?mh_xm->restart:0;
	mmoo.numins		= mh_xm->numins;
	mmoo.flags	   |= UF_XMPERIODS|UF_INST|UF_NOWRAP|UF_FT2QUIRKS;
	if (mh_xm->flags&1) mmoo.flags |= UF_LINEAR;

	memset(mmoo.chanvol, 64, mmoo.numchn);			 /* store channel volumes */

	if (!AllocPositions(mmoo.numpos+1))		{	return (0);}
	for (t=0; t<mmoo.numpos; t++)
	{	mmoo.positions[t] = mh_xm->orders[t];
	}
	/* We have to check for any pattern numbers in the order list greater than
	   the number of patterns total. If one or more is found, we set it equal to
	   the pattern total and make a dummy pattern to workaround the problem */
	for (t=0; t<mmoo.numpos; t++)
	{
		if (mmoo.positions[t]>=mmoo.numpat)
		{
			mmoo.positions[t] = mmoo.numpat;
			dummypat = 1;
		}
	}
	if (dummypat)
	{
		mmoo.numpat++;
		mmoo.numtrk += mmoo.numchn;
	}

	if (mh_xm->version<0x0104)
	{
		if (!LoadInstruments()) 		{	return (0);}
		if (!LoadPatterns(dummypat))	{	return (0);}
		for (t=0; t<mmoo.numsmp; t++)
		{	nextwav[t] += mod_music_ftell(mmff.mod_FILE_reader);
		}
	}
	else
	{
		if (!LoadPatterns(dummypat))	{	return (0);}
		if (!LoadInstruments()) 		{	return (0);}
	}

	if (!AllocSamples())
	{
		free(nextwav);	nextwav = NULL;
		free(wh);		wh = NULL;
		return (0);
	}
	q = mmoo.samples;
	s = wh;
	for (u=0; u<mmoo.numsmp; u++,q++,s++)
	{
		q->samplename	= DupStr(s->samplename,22,1);
		q->length		= s->length;
		q->loopstart	= s->loopstart;
		q->loopend		= s->loopstart+s->looplength;
		q->volume		= s->volume;
		q->speed		= s->mm_fine_tune+128;
		q->panning		= s->panning;
		q->seekpos		= nextwav[u];
		q->vibtype		= s->vibtype;
		q->vibsweep 	= s->vibsweep;
		q->vibdepth 	= s->vibdepth;
		q->vibrate		= s->vibrate;

		if (s->type & 0x10)
		{
			q->length	 >>= 1;
			q->loopstart >>= 1;
			q->loopend	 >>= 1;
		}

		q->flags |= (SF_OWNPAN);
		if (s->type&0x3)	{	q->flags |= SF_LOOP;}
		if (s->type&0x2)	{	q->flags |= SF_BIDI;}
		if (s->type&0x10)	{	q->flags |= SF_16BITS;}
		q->flags |= (SF_DELTA|SF_SIGNED);
	}
	d = mmoo.instruments;
	s = wh;
	for (u=0; u<mmoo.numins; u++,d++)
	{
		for (t=0; t<XMNOTECNT; t++)
		{
			if (d->samplenumber[t]>=mmoo.numsmp)
			{	d->samplenote[t] = 255;}
			else
			{
				int note = t+s[d->samplenumber[t]].relnote;
				d->samplenote[t] = (note<0)?0:note;
			}
		}
	}
	free(nextwav);nextwav = NULL;
	free(wh);	wh = NULL;
	return (1);
}

#if 0/*曲名廃止*/
MM_s8 *XM_LoadTitle(void)
{
	MM_s8 s[21];
	mod_music_file_seek_set(mmff.mod_FILE_reader, 17);
	mod_music_file_read_u8S(s, 21, mmff.mod_FILE_reader);
//	if (0==)	{return (NULL);}
	return (DupStr(s,21,1));
}
#endif
/*---------- Loader information */

MOD_MUSIC_API MOD_MUSIC_INTERNAL_LOADER load_xm =
{
	NULL,
	"XM",
	"XM (FastTracker 2)",
	XM_Init,
	XM_Test,
	XM_Load,
	XM_Cleanup,
//	/*曲名廃止*/	XM_LoadTitle
};

/* ex:set ts=4: */
