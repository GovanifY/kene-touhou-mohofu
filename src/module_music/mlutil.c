
#include "mohou_mikmod_internals.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
	-------------------------------------------------------
	$Id: mlutil.c,v 1.2 2000/02/01 21:16:29 hercules Exp $
	Utility functions for the module loader
---------------------------------------------------------*/

#include <string.h>
//#include <memory.h>


/*---------- Shared loader variables */

s8  mm_re_map[UF_MAXCHAN];					/* for removing empty channels */
u8* mm_position_look_up = NULL;				/* lookup table for pattern jumps after blank pattern removal */
// u8  mm_midi_position_look_up_counter;
u16* origpositions = NULL;

// MM_BOOL	 mm_midi_use_resonant_filters;		/* resonant filters in use */
// /*u8*/u32	mm_midi_active_macro;		/* active midi macro number for Sxx,xx<80h (現状 xx <= 0x0f ) */
MOD_MUSIC_INTERNAL_MIDI_FILTER mm_midi;

u8  mm_filter_macros[UF_MAXMACRO];			/* midi macro settings */
FILTER filtersettings[UF_MAXFILTER];			/* computed filter settings */

/*---------- Linear periods stuff */

int*   noteindex = NULL;		/* re-map value for linear period modules */
static int noteindexcount = 0;

int *AllocLinear(void)
{
	if (of.numsmp>noteindexcount)
	{
		noteindexcount = of.numsmp;
		noteindex = realloc(noteindex,noteindexcount*sizeof(int));
	}
	return noteindex;
}

void FreeLinear(void)
{
	if (noteindex)
	{
		free(noteindex);
		noteindex = NULL;
	}
	noteindexcount = 0;
}

int speed_to_mm_fine_tune(u32 speed, int sample)
{
	unsigned int ctmp = 0;
	unsigned int tmp;
	int note = 1;
	int mm_fine_tune = 0;
	//
	speed >>= 1;
	while ((tmp=getfrequency(of.flags,getlinearperiod(note<<1,0))) < speed)
	{
		ctmp = tmp;
		note++;
	}
	if (tmp != /*(int)*/speed)
	{
		if ( ((signed)tmp - (signed)speed) < ((signed)speed - (signed)ctmp))/* ...うーん */
		{
			while (tmp > speed)
			{	tmp=getfrequency(of.flags,getlinearperiod(note<<1,--mm_fine_tune));}
		}
		else
		{
			note--;
			while (ctmp < speed)
			{	ctmp=getfrequency(of.flags,getlinearperiod(note<<1,++mm_fine_tune));}
		}
	}
	noteindex[sample] = note - (4*OCTAVE);
	return (mm_fine_tune);
}

/*---------- Order stuff */

/* handles S3M and IT orders */
void S3MIT_CreateOrders(MM_BOOL curious)
{
	of.numpos = 0;
	memset(of.positions, 0, mm_midi.mm_midi_position_look_up_counter*sizeof(u16));
	memset(mm_position_look_up,-1,256);
	//
	unsigned int t;
	for (t=0; t<mm_midi.mm_midi_position_look_up_counter; t++)
	{
		int order = origpositions[t];
		if (order==255)
		{
			order = LAST_PATTERN;
		}
		of.positions[of.numpos] = order;
		mm_position_look_up[t] = of.numpos; /* bug fix for freaky S3Ms / ITs */
		if (origpositions[t]<254)
		{
			of.numpos++;
		}
		else
		{
			/* end of song special order */
			if ((order==LAST_PATTERN)&&(!(curious--))) break;
		//	if ((of.positions[t]==255)&&(!(curious--))) break;
		}
	}
}

/*---------- Effect stuff */

/* handles S3M and IT effects */
void S3MIT_ProcessCmd(u8 cmd, u8 inf, /*unsigned int*/u8 flags)//MM_BOOL oldeffect)
{
	u8 hi, lo;
	lo = inf&0x0f;
	hi = inf>>4;

	/* process S3M / IT specific command structure */

	if (cmd!=255)
	{
		switch (cmd)
		{
			case 1: /* Axx set speed to xx */
				UniEffect(U231_ICODE_0x30_S3MEFFECTA,inf);
				break;
			case 2: /* Bxx position jump */
				if (inf<mm_midi.mm_midi_position_look_up_counter)
				{
					/* switch to curious mode if necessary, for example
					   sympex.it, deep joy.it */
					if (((s8)mm_position_look_up[inf]<0)&&(origpositions[inf]!=255))
					{	S3MIT_CreateOrders(1);}

					if (!((s8)mm_position_look_up[inf]<0))
					{	UniPTEffect(0xb,mm_position_look_up[inf]);}
				}
				break;
			case 3: /* Cxx patternbreak to row xx */
			//	if (oldeffect==1)
			//	if ((flags & S3MIT_OLDSTYLE) && !(flags & S3MIT_IT))
				if ( (S3MIT_OLDSTYLE) == (flags & (S3MIT_IT|S3MIT_OLDSTYLE)) )
				{	UniPTEffect(0xd,(inf>>4)*10+(inf&0xf));}
				else
				{	UniPTEffect(0xd,inf);}
				break;
			case 4: /* Dxy volumeslide */
				UniEffect(U231_ICODE_0x31_S3MEFFECTD,inf);
				break;
			case 5: /* Exy toneslide down */
				UniEffect(U231_ICODE_0x32_S3MEFFECTE,inf);
				break;
			case 6: /* Fxy toneslide up */
				UniEffect(U231_ICODE_0x33_S3MEFFECTF,inf);
				break;
			case 7: /* Gxx Tone portamento, speed xx */
				if (flags & S3MIT_OLDSTYLE)
				{	UniPTEffect(0x3,inf);}
				else
				{	UniEffect(U231_ICODE_0x04_ITEFFECTG,inf);}
				break;
			case 8: /* Hxy vibrato */
				if (flags & S3MIT_OLDSTYLE)
				{	UniPTEffect(0x04,inf);}
				else
				{	UniEffect(U231_ICODE_0x05_ITEFFECTH,inf);}
				break;
			case 9: /* Ixy tremor, ontime x, offtime y */
				if (flags & S3MIT_OLDSTYLE)
				{	UniEffect(U231_ICODE_0x34_S3MEFFECTI,inf);}
				else
				{	UniEffect(U231_ICODE_0x06_ITEFFECTI,inf);}
				break;
			case 0xa: /* Jxy arpeggio */
				UniPTEffect(0x00,inf);
				break;
			case 0xb: /* Kxy Dual command H00 & Dxy */
				if (flags & S3MIT_OLDSTYLE)
				{	UniPTEffect(0x04,0);}
				else
				{	UniEffect(U231_ICODE_0x05_ITEFFECTH,0);}
				UniEffect(U231_ICODE_0x31_S3MEFFECTD,inf);
				break;
			case 0xc: /* Lxy Dual command G00 & Dxy */
				if (flags & S3MIT_OLDSTYLE)
				{	UniPTEffect(0x03,0);}
				else
				{	UniEffect(U231_ICODE_0x04_ITEFFECTG,0);}
				UniEffect(U231_ICODE_0x31_S3MEFFECTD,inf);
				break;
			case 0xd: /* Mxx Set Channel Volume */
				UniEffect(U231_ICODE_0x07_ITEFFECTM,inf);
				break;
			case 0xe: /* Nxy Slide Channel Volume */
				UniEffect(U231_ICODE_0x08_ITEFFECTN,inf);
				break;
			case 0xf: /* Oxx set sampleoffset xx00h */
				UniPTEffect(0x09,inf);
				break;
			case 0x10: /* Pxy Slide Panning Commands */
				UniEffect(U231_ICODE_0x09_ITEFFECTP,inf);
				break;
			case 0x11: /* Qxy Retrig (+volumeslide) */
				UniWriteByte(U231_ICODE_0x35_S3MEFFECTQ);
				if (inf && !lo && !(flags & S3MIT_OLDSTYLE))
					UniWriteByte(1);
				else
					UniWriteByte(inf);
				break;
			case 0x12: /* Rxy tremolo speed x, depth y */
				UniEffect(U231_ICODE_0x36_S3MEFFECTR,inf);
				break;
			case 0x13: /* Sxx special commands */
				if (inf >= 0xf0)/* 0xf0 ... 0xff はmidi fliter 切り替えコマンド */
				{
					/* do if filter enables only. */
					if (mm_midi.mm_midi_use_resonant_filters)/* filter 有効の場合のみ */
					{
						/* change resonant filter settings if necessary */
						if (((inf&0x0f) != mm_midi.mm_midi_active_macro))/* filter が実際に切り替わった場合のみ */
						{
							mm_midi.mm_midi_active_macro = (inf & 0x0f);/* 現在有効の midi fliter番号を保存。 */
							/* midi fliter 切り替え処理 */
							for (inf=0; inf<0x80; inf++)
							{
								filtersettings[inf].filter = mm_filter_macros[mm_midi.mm_midi_active_macro];
							}
						}
					}
				}
				else
				{
					/* Scream Tracker does not have samples larger than
					   64 Kb, thus doesn't need the SAx effect */
//					if ((flags & S3MIT_SCREAM) && ((inf & 0xf0) == 0xa0))
//						break;

					UniEffect(U231_ICODE_0x0f_ITEFFECTS0,inf);
				}
				break;
			case 0x14: /* Txx tempo */
				if (inf>=0x20)
					UniEffect(U231_ICODE_0x37_S3MEFFECTT,inf);
				else
				{
					if (!(flags & S3MIT_OLDSTYLE))
						/* IT Tempo slide */
					{	UniEffect(U231_ICODE_0x0a_ITEFFECTT,inf);}
				}
				break;
			case 0x15: /* Uxy Fine Vibrato speed x, depth y */
				if (flags & S3MIT_OLDSTYLE)
				{	UniEffect(U231_ICODE_0x38_S3MEFFECTU,inf);}
				else
				{	UniEffect(U231_ICODE_0x0b_ITEFFECTU,inf);}
				break;
			case 0x16: /* Vxx Set Global Volume */
				UniEffect(U231_ICODE_0x28_XMEFFECTG,inf);
				break;
			case 0x17: /* Wxy Global Volume Slide */
				UniEffect(U231_ICODE_0x0c_ITEFFECTW,inf);
				break;
			case 0x18: /* Xxx amiga command 8xx */
				if (flags & S3MIT_OLDSTYLE)
				{
					if (inf>128)
					{	UniEffect(U231_ICODE_0x0f_ITEFFECTS0,0x91); }/* surround */
					else
					{	UniPTEffect(0x8,(inf==128)?255:(inf<<1));}
				}
				else
				{	UniPTEffect(0x8,inf);}
				break;
			case 0x19: /* Yxy Panbrello  speed x, depth y */
				UniEffect(U231_ICODE_0x0d_ITEFFECTY,inf);
				break;
			case 0x1a: /* Zxx midi/resonant filters */
				if (filtersettings[inf].filter)
				{
					UniWriteByte(U231_ICODE_0x0e_ITEFFECTZ);
					UniWriteByte(filtersettings[inf].filter);
					UniWriteByte(filtersettings[inf].inf);
				}
				break;
		}
	}
}
/* ------------------- */
/* ------------------- */
//extern const/*u16*/u8 uni_operands[U231_ICODE_0x3c_MAX];//extern u16 unioperands[U231_ICODE_0x40_MAX];
static const/*u16*/u8 s_uni_operands[U231_ICODE_0x3c_MAX]=
{
	0, /* U231_ICODE_0x00_dummy not used */
	1, /* U231_ICODE_0x01_NOTE */
	1, /* U231_ICODE_0x02_INSTRUMENT */
	/* UltraTracker effects */
	2, /* U231_ICODE_0x03_ULTEFFECT9 */
	/* Impulse Tracker effects */
	1, /* U231_ICODE_0x04_ITEFFECTG */
	1, /* U231_ICODE_0x05_ITEFFECTH */
	1, /* U231_ICODE_0x06_ITEFFECTI */
	1, /* U231_ICODE_0x07_ITEFFECTM */
	1, /* U231_ICODE_0x08_ITEFFECTN */
	1, /* U231_ICODE_0x09_ITEFFECTP */
	1, /* U231_ICODE_0x0a_ITEFFECTT */
	1, /* U231_ICODE_0x0b_ITEFFECTU */
	1, /* U231_ICODE_0x0c_ITEFFECTW */
	1, /* U231_ICODE_0x0d_ITEFFECTY */
	2, /* U231_ICODE_0x0e_ITEFFECTZ */
	1, /* U231_ICODE_0x0f_ITEFFECTS0 */
	/* Protracker effects */
	1, /* U231_ICODE_0x10_PTEFFECT0 */
	1, /* U231_ICODE_0x11_PTEFFECT1 */
	1, /* U231_ICODE_0x12_PTEFFECT2 */
	1, /* U231_ICODE_0x13_PTEFFECT3 */
	1, /* U231_ICODE_0x14_PTEFFECT4 */
	1, /* U231_ICODE_0x15_PTEFFECT5 */
	1, /* U231_ICODE_0x16_PTEFFECT6 */
	1, /* U231_ICODE_0x17_PTEFFECT7 */
	1, /* U231_ICODE_0x18_PTEFFECT8 */
	1, /* U231_ICODE_0x19_PTEFFECT9 */
	1, /* U231_ICODE_0x1a_PTEFFECTA */
	1, /* U231_ICODE_0x1b_PTEFFECTB */
	1, /* U231_ICODE_0x1c_PTEFFECTC */
	1, /* U231_ICODE_0x1d_PTEFFECTD */
	1, /* U231_ICODE_0x1e_PTEFFECTE */
	1, /* U231_ICODE_0x1f_PTEFFECTF */
	/* Fast Tracker effects */
	1, /* U231_ICODE_0x20_KEYFADE */
	2, /* U231_ICODE_0x21_VOLEFFECTS */
	0, /* U231_ICODE_0x22_KEYOFF */ 		//	1, /* U231_ICODE_0x22_XMEFFECT4 */
	1, /* U231_ICODE_0x23_XMEFFECTA */
	1, /* U231_ICODE_0x24_XMEFFECTE1 */
	1, /* U231_ICODE_0x25_XMEFFECTE2 */
	1, /* U231_ICODE_0x26_XMEFFECTEA */
	1, /* U231_ICODE_0x27_XMEFFECTEB */
	1, /* U231_ICODE_0x28_XMEFFECTG */
	1, /* U231_ICODE_0x29_XMEFFECTH */
	1, /* U231_ICODE_0x2a_XMEFFECTL */
	1, /* U231_ICODE_0x2b_XMEFFECTP */
	1, /* U231_ICODE_0x2c_XMEFFECTX1 */
	1, /* U231_ICODE_0x2d_XMEFFECTX2 */
	2, /* U231_ICODE_0x2e_MEDSPEED */		/* OctaMED effects */	//	0, /* U231_ICODE_0x2e_resurved */
	1, /* U231_ICODE_0x2f_XMEFFECTZ */
	/* Scream Tracker effects */
	1, /* U231_ICODE_0x30_S3MEFFECTA */
	1, /* U231_ICODE_0x31_S3MEFFECTD */
	1, /* U231_ICODE_0x32_S3MEFFECTE */
	1, /* U231_ICODE_0x33_S3MEFFECTF */
	1, /* U231_ICODE_0x34_S3MEFFECTI */
	1, /* U231_ICODE_0x35_S3MEFFECTQ */
	1, /* U231_ICODE_0x36_S3MEFFECTR */
	1, /* U231_ICODE_0x37_S3MEFFECTT */
	1, /* U231_ICODE_0x38_S3MEFFECTU */
	0, /* U231_ICODE_0x39_MEDEFFECTF1 */	/* OctaMED effects */
	0, /* U231_ICODE_0x3a_MEDEFFECTF2 */	/* OctaMED effects */	//	0, /* U231_ICODE_0x3a_resurved */
	0, /* U231_ICODE_0x3b_MEDEFFECTF3 */	/* OctaMED effects */	//	0, /* U231_ICODE_0x3b_resurved */
};

/* Sparse description of the internal module format
   ------------------------------------------------

  A UNITRK stream is an array of bytes representing a single track of a pattern.
It's made up of 'repeat/length' bytes, opcodes and operands (sort of a assembly
language):

rrrlllll
[REP/LEN][OPCODE][OPERAND][OPCODE][OPERAND] [REP/LEN][OPCODE][OPERAND]..
^										  ^ ^
|-------ROWS 0 - 0+REP of a track---------| |-------ROWS xx - xx+REP of a track...

  The rep/len byte contains the number of bytes in the current row, _including_
the length byte itself (So the LENGTH byte of row 0 in the previous example
would have a value of 5). This makes it easy to search through a stream for a
particular row. A track is concluded by a 0-value length byte.

  The upper 3 bits of the rep/len byte contain the number of times -1 this row
is repeated for this track. (so a value of 7 means this row is repeated 8 times)

  Opcodes can range from 1 to 255 but currently only opcodes 1 to 52 are being
used. Each opcode can have a different number of operands. You can find the
number of operands to a particular opcode by using the opcode as an index into
the 'uni_operands' table.

*/

/*static*/ void UniSkipOpcode(u8 op)
{
	if (op<U231_ICODE_0x3c_MAX)
	{
		u16 t = s_uni_operands[op];
		while (t--) 	{UniGetByte();}
	}
}
/*---------- Unitrk stuff */

/* Generic effect writing routine */
void UniEffect(u16 eff, u16 dat)
{
	if ((!eff)||(eff>=U231_ICODE_0x3c_MAX)) 	{return;}

	UniWriteByte(eff);
	if (2==s_uni_operands[eff])
	{	UniWriteWord(dat);}
	else
	{	UniWriteByte(dat);}
}

/*	Appends 	U231_ICODE_xxx_U N I_PTEFFECTX opcode to the unitrk stream. */
void UniPTEffect(u8 eff, u8 dat)
{
	#ifdef MIKMOD_DEBUG
	if (eff>=0x10)
	{	error(0/*ERR_FATAL*/, "UniPTEffect called with incorrect eff value %d\n",eff);}
	else
	#endif
	if ((eff)||(dat)) UniEffect(U231_ICODE_0x10_PTEFFECT0+eff,dat);
}

/* Appends U231_ICODE_xxx_U N I_VOLEFFECT + effect/dat to unistream. */
void UniVolEffect(u16 eff, u8 dat)
{
	if ((eff)||(dat))	/* don't write empty effect */
	{
		UniWriteByte(U231_ICODE_0x21_VOLEFFECTS);
		UniWriteByte(eff);UniWriteByte(dat);
	}
}

/* ex:set ts=4: */
