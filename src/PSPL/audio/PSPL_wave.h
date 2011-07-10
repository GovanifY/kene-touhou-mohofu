
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_wave.h,v 1.6 2004/01/04 16:49:11 slouken Exp $";
#endif

/* WAVE files are little-endian */

/*******************************************/
/* Define values for Microsoft WAVE format */
/*******************************************/
#define RIFF			0x46464952			/* "RIFF" */
#define WAVE			0x45564157			/* "WAVE" */
#define FACT			0x74636166			/* "fact" */
#define LIST			0x5453494c			/* "LIST" */
#define FMT 			0x20746D66			/* "fmt " */
#define DATA			0x61746164			/* "data" */
#define PCM_CODE		0x0001
#define MS_ADPCM_CODE	0x0002
#define IMA_ADPCM_CODE	0x0011
#define WAVE_MONO		1
#define WAVE_STEREO 	2

/* Normally, these three chunks come consecutively in a WAVE file */
typedef struct WaveFMT
{
/* Not saved in the chunk we read:
	u32	FMTchunk;
	u32	fmtlen;
*/
	u16	encoding;
	u16	channels;			/* 1 = mono, 2 = stereo */
	u32	frequency;			/* One of 11025, 22050, or 44100 Hz */
	u32	byterate;			/* Average bytes per second */
	u16	blockalign; 		/* Bytes per sample block */
	u16	bitspersample;		/* One of 8, 12, 16, or 4 for ADPCM */
} WaveFMT;

/* The general chunk found in the WAVE file */
typedef struct Chunk
{
	u32 magic;
	u32 length;
	u8 *data;
} Chunk;
