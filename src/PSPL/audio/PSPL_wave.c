
/*---------------------------------------------------------
	ìåï˚ñÕïÌïó Å` Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_wave.c,v 1.6 2004/01/04 16:49:11 slouken Exp $";
#endif

#ifndef DISABLE_FILE

/* Microsoft WAVE file loading routines */

#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "./../include/PSPL_error.h"
#include "./../include/PSPL_audio.h"
#include "PSPL_wave.h"
#include "./../include/PSPL_endian.h"

#ifndef SDL_Error_bbb
	#define SDL_Error_bbb(aaa)
#endif

#ifndef SDL_SetError_bbb
	#define SDL_SetError_bbb( ... )
#endif

#ifndef NELEMS
#define NELEMS(array)	((sizeof array)/(sizeof array[0]))
#endif

static int ReadChunk(SDL_RWops *src, Chunk *chunk);

struct MS_ADPCM_decodestate
{
	u8 hPredictor;
	u16 iDelta;
	s16 iSamp1;
	s16 iSamp2;
};
static struct MS_ADPCM_decoder
{
	WaveFMT wavefmt;
	u16 wSamplesPerBlock;
	u16 wNumCoef;
	s16 aCoeff[7][2];
	/* --- */
	struct MS_ADPCM_decodestate state[2];
} MS_ADPCM_state;

static int InitMS_ADPCM(WaveFMT *format)
{
	u8 *rogue_feel;
	u16 extra_info;
	int i;

	/* Set the rogue pointer to the MS_ADPCM specific data */
	MS_ADPCM_state.wavefmt.encoding 		= SDL_SwapLE16(format->encoding);
	MS_ADPCM_state.wavefmt.channels 		= SDL_SwapLE16(format->channels);
	MS_ADPCM_state.wavefmt.frequency		= SDL_SwapLE32(format->frequency);
	MS_ADPCM_state.wavefmt.byterate 		= SDL_SwapLE32(format->byterate);
	MS_ADPCM_state.wavefmt.blockalign		= SDL_SwapLE16(format->blockalign);
	MS_ADPCM_state.wavefmt.bitspersample	= SDL_SwapLE16(format->bitspersample);
	rogue_feel = (u8 *)format+sizeof(*format);
	if ( sizeof(*format) == 16 )
	{
		extra_info = ((rogue_feel[1]<<8)|rogue_feel[0]);
		rogue_feel += sizeof(u16);
	}
	MS_ADPCM_state.wSamplesPerBlock = ((rogue_feel[1]<<8)|rogue_feel[0]);
	rogue_feel += sizeof(u16);
	MS_ADPCM_state.wNumCoef = ((rogue_feel[1]<<8)|rogue_feel[0]);
	rogue_feel += sizeof(u16);
	if ( MS_ADPCM_state.wNumCoef != 7 )
	{
		SDL_SetError_bbb("Unknown set of MS_ADPCM coefficients");
		return (-1);
	}
	for (i=0; i<MS_ADPCM_state.wNumCoef; i++)
	{
		MS_ADPCM_state.aCoeff[i][0] = ((rogue_feel[1]<<8)|rogue_feel[0]);
		rogue_feel += sizeof(u16);
		MS_ADPCM_state.aCoeff[i][1] = ((rogue_feel[1]<<8)|rogue_feel[0]);
		rogue_feel += sizeof(u16);
	}
	return (0);
}

static s32 MS_ADPCM_nibble(
	struct MS_ADPCM_decodestate *state,
	u8 nybble, s16 *coeff)
{
	const s32 max_audioval = ((1<<(16-1))-1);
	const s32 min_audioval = -(1<<(16-1));
	const s32 adaptive[] =
	{
		230, 230, 230, 230, 307, 409, 512, 614,
		768, 614, 512, 409, 307, 230, 230, 230
	};
	s32 new_sample, delta;

	new_sample = ((state->iSamp1 * coeff[0]) +
			  (state->iSamp2 * coeff[1]))/256;
	if ( nybble & 0x08 )
	{
		new_sample += state->iDelta * (nybble-0x10);
	}
	else
	{
		new_sample += state->iDelta * nybble;
	}
	if ( new_sample < min_audioval )
	{
		new_sample = min_audioval;
	}
	else
	if ( new_sample > max_audioval )
	{
		new_sample = max_audioval;
	}
	delta = ((s32)state->iDelta * adaptive[nybble])/256;
	if ( delta < 16 )
	{
		delta = 16;
	}
	state->iDelta = delta;
	state->iSamp2 = state->iSamp1;
	state->iSamp1 = new_sample;
	return (new_sample);
}

static int MS_ADPCM_decode(u8 **audio_buf, u32 *audio_len)
{
	struct MS_ADPCM_decodestate *state[2];
	u8 *freeable, *encoded, *decoded;
	s32 encoded_len, samplesleft;
	s8 nybble, stereo;
	s16 *coeff[2];
	s32 new_sample;

	/* Allocate the proper sized output buffer */
	encoded_len 	= *audio_len;
	encoded 		= *audio_buf;
	freeable		= *audio_buf;
	*audio_len		= (encoded_len/MS_ADPCM_state.wavefmt.blockalign) *
						MS_ADPCM_state.wSamplesPerBlock*
						MS_ADPCM_state.wavefmt.channels*sizeof(s16);
	*audio_buf		= (u8 *)malloc(*audio_len);
	if ( *audio_buf == NULL )
	{
		SDL_Error_bbb(SDL_ENOMEM);
		return (-1);
	}
	decoded = *audio_buf;

	/* Get ready... Go! */
	stereo		= (MS_ADPCM_state.wavefmt.channels == 2);
	state[0]	= &MS_ADPCM_state.state[0];
	state[1]	= &MS_ADPCM_state.state[stereo];
	while ( encoded_len >= MS_ADPCM_state.wavefmt.blockalign )
	{
		/* Grab the initial information for this block */
		state[0]->hPredictor = *encoded++;
		if ( stereo )
		{
			state[1]->hPredictor = *encoded++;
		}
		state[0]->iDelta = ((encoded[1]<<8)|encoded[0]);
		encoded += sizeof(s16);
		if ( stereo )
		{
			state[1]->iDelta = ((encoded[1]<<8)|encoded[0]);
			encoded += sizeof(s16);
		}
		state[0]->iSamp1 = ((encoded[1]<<8)|encoded[0]);
		encoded += sizeof(s16);
		if ( stereo )
		{
			state[1]->iSamp1 = ((encoded[1]<<8)|encoded[0]);
			encoded += sizeof(s16);
		}
		state[0]->iSamp2 = ((encoded[1]<<8)|encoded[0]);
		encoded += sizeof(s16);
		if ( stereo )
		{
			state[1]->iSamp2 = ((encoded[1]<<8)|encoded[0]);
			encoded += sizeof(s16);
		}
		coeff[0] = MS_ADPCM_state.aCoeff[state[0]->hPredictor];
		coeff[1] = MS_ADPCM_state.aCoeff[state[1]->hPredictor];

		/* Store the two initial samples we start with */
		decoded[0] = state[0]->iSamp2&0xff;
		decoded[1] = state[0]->iSamp2>>8;
		decoded += 2;
		if ( stereo )
		{
			decoded[0] = state[1]->iSamp2&0xff;
			decoded[1] = state[1]->iSamp2>>8;
			decoded += 2;
		}
		decoded[0] = state[0]->iSamp1&0xff;
		decoded[1] = state[0]->iSamp1>>8;
		decoded += 2;
		if ( stereo )
		{
			decoded[0] = state[1]->iSamp1&0xff;
			decoded[1] = state[1]->iSamp1>>8;
			decoded += 2;
		}

		/* Decode and store the other samples in this block */
		samplesleft 	= (MS_ADPCM_state.wSamplesPerBlock-2)*
							MS_ADPCM_state.wavefmt.channels;
		while ( samplesleft > 0 )
		{
			nybble = (*encoded)>>4;
			new_sample = MS_ADPCM_nibble(state[0],nybble,coeff[0]);
			decoded[0] = new_sample&0xff;
			new_sample >>= 8;
			decoded[1] = new_sample&0xff;
			decoded += 2;

			nybble = (*encoded)&0x0F;
			new_sample = MS_ADPCM_nibble(state[1],nybble,coeff[1]);
			decoded[0] = new_sample&0xff;
			new_sample >>= 8;
			decoded[1] = new_sample&0xff;
			decoded += 2;

			++encoded;
			samplesleft -= 2;
		}
		encoded_len -= MS_ADPCM_state.wavefmt.blockalign;
	}
	free(freeable);
	return (0);
}

struct IMA_ADPCM_decodestate
{
	s32 sample;
	s8 index;
};
static struct IMA_ADPCM_decoder
{
	WaveFMT wavefmt;
	u16 wSamplesPerBlock;
	/* * * */
	struct IMA_ADPCM_decodestate state[2];
} IMA_ADPCM_state;

static int InitIMA_ADPCM(WaveFMT *format)
{
	u8 *rogue_feel;
	u16 extra_info;

	/* Set the rogue pointer to the IMA_ADPCM specific data */
	IMA_ADPCM_state.wavefmt.encoding = SDL_SwapLE16(format->encoding);
	IMA_ADPCM_state.wavefmt.channels = SDL_SwapLE16(format->channels);
	IMA_ADPCM_state.wavefmt.frequency = SDL_SwapLE32(format->frequency);
	IMA_ADPCM_state.wavefmt.byterate = SDL_SwapLE32(format->byterate);
	IMA_ADPCM_state.wavefmt.blockalign = SDL_SwapLE16(format->blockalign);
	IMA_ADPCM_state.wavefmt.bitspersample =
					 SDL_SwapLE16(format->bitspersample);
	rogue_feel = (u8 *)format+sizeof(*format);
	if ( sizeof(*format) == 16 )
	{
		extra_info = ((rogue_feel[1]<<8)|rogue_feel[0]);
		rogue_feel += sizeof(u16);
	}
	IMA_ADPCM_state.wSamplesPerBlock = ((rogue_feel[1]<<8)|rogue_feel[0]);
	return (0);
}

static s32 IMA_ADPCM_nibble(struct IMA_ADPCM_decodestate *state,u8 nybble)
{
	const s32 max_audioval = ((1<<(16-1))-1);
	const s32 min_audioval = -(1<<(16-1));
	const int index_table[16] =
	{
		-1, -1, -1, -1,
		 2,  4,  6,  8,
		-1, -1, -1, -1,
		 2,  4,  6,  8
	};
	const s32 step_table[89] =
	{
		7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31,
		34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130,
		143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408,
		449, 494, 544, 598, 658, 724, 796, 876, 963, 1060, 1166, 1282,
		1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327,
		3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630,
		9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350,
		22385, 24623, 27086, 29794, 32767
	};
	s32 delta, step;

	/* Compute difference and new sample value */
	step = step_table[state->index];
	delta = step >> 3;
	if ( nybble & 0x04 ) delta += step;
	if ( nybble & 0x02 ) delta += (step >> 1);
	if ( nybble & 0x01 ) delta += (step >> 2);
	if ( nybble & 0x08 ) delta = -delta;
	state->sample += delta;

	/* Update index value */
	state->index += index_table[nybble];
	if ( state->index > 88 )
	{
		state->index = 88;
	}
	else
	if ( state->index < 0 )
	{
		state->index = 0;
	}

	/* Clamp output sample */
	if ( state->sample > max_audioval )
	{
		state->sample = max_audioval;
	}
	else
	if ( state->sample < min_audioval )
	{
		state->sample = min_audioval;
	}
	return (state->sample);
}

/* Fill the decode buffer with a channel block of data (8 samples) */
static void Fill_IMA_ADPCM_block(
	u8 *decoded, u8 *encoded,
	int channel, int numchannels,
	struct IMA_ADPCM_decodestate *state)
{
	int i;
	s8 nybble;
	s32 new_sample;

	decoded += (channel * 2);
	for (i=0; i<4; i++)
	{
		nybble = (*encoded)&0x0F;
		new_sample = IMA_ADPCM_nibble(state, nybble);
		decoded[0] = new_sample&0xff;
		new_sample >>= 8;
		decoded[1] = new_sample&0xff;
		decoded += 2 * numchannels;

		nybble = (*encoded)>>4;
		new_sample = IMA_ADPCM_nibble(state, nybble);
		decoded[0] = new_sample&0xff;
		new_sample >>= 8;
		decoded[1] = new_sample&0xff;
		decoded += 2 * numchannels;

		encoded++;
	}
}

static int IMA_ADPCM_decode(u8 **audio_buf, u32 *audio_len)
{
	struct IMA_ADPCM_decodestate *state;
	u8 *freeable, *encoded, *decoded;
	s32 encoded_len, samplesleft;
	unsigned int ccc;
	unsigned int channels;
	/* Check to make sure we have enough variables in the state array */
	channels = IMA_ADPCM_state.wavefmt.channels;
	if ( channels > NELEMS(IMA_ADPCM_state.state) )
	{
		SDL_SetError_bbb("IMA ADPCM decoder can only handle %d channels",
						NELEMS(IMA_ADPCM_state.state));
		return (-1);
	}
	state = IMA_ADPCM_state.state;

	/* Allocate the proper sized output buffer */
	encoded_len 	= *audio_len;
	encoded 		= *audio_buf;
	freeable		= *audio_buf;
	*audio_len		= (encoded_len/IMA_ADPCM_state.wavefmt.blockalign) *
				IMA_ADPCM_state.wSamplesPerBlock*
				IMA_ADPCM_state.wavefmt.channels*sizeof(s16);
	*audio_buf = (u8 *)malloc(*audio_len);
	if ( *audio_buf == NULL )
	{
		SDL_Error_bbb(SDL_ENOMEM);
		return (-1);
	}
	decoded = *audio_buf;

	/* Get ready... Go! */
	while ( encoded_len >= IMA_ADPCM_state.wavefmt.blockalign )
	{
		/* Grab the initial information for this block */
		for (ccc=0; ccc<channels; ccc++)
		{
			/* Fill the state information for this block */
			state[ccc].sample = ((encoded[1]<<8)|encoded[0]);
			encoded += 2;
			if ( state[ccc].sample & 0x8000 )
			{
				state[ccc].sample -= 0x10000;
			}
			state[ccc].index = *encoded++;
			/* Reserved byte in buffer header, should be 0 */
			if ( *encoded++ != 0 )
			{
				/* Uh oh, corrupt data?  Buggy code? */;
			}

			/* Store the initial sample we start with */
			decoded[0] = state[ccc].sample&0xff;
			decoded[1] = state[ccc].sample>>8;
			decoded += 2;
		}

		/* Decode and store the other samples in this block */
		samplesleft = (IMA_ADPCM_state.wSamplesPerBlock-1)*channels;
		while ( samplesleft > 0 )
		{
			for (ccc=0; ccc<channels; ccc++)
			{
				Fill_IMA_ADPCM_block(decoded, encoded,
						ccc, channels, &state[ccc]);
				encoded += 4;
				samplesleft -= 8;
			}
			decoded += (channels * 8 * 2);
		}
		encoded_len -= IMA_ADPCM_state.wavefmt.blockalign;
	}
	free(freeable);
	return (0);
}

PSPL_AUDIO_SPEC * PSPL_LoadWAV_RW(
	SDL_RWops *src,
	int freesrc,
	PSPL_AUDIO_SPEC *spec,
	u8 **audio_buf,
	u32 *audio_len)
{
	int was_error;
	Chunk chunk;
	int lenread;
	int MS_ADPCM_encoded, IMA_ADPCM_encoded;
	int samplesize;

	/* WAV magic header */
	u32 RIFFchunk;
	u32 wavelen;
	u32 WAVEmagic;

	/* FMT chunk */
	WaveFMT *format = NULL;

	/* Make sure we are passed a valid data source */
	was_error = 0;
	if ( src == NULL )
	{
		was_error = 1;
		goto done;
	}

	/* Check the magic header */
	RIFFchunk	= SDL_ReadLE32(src);
	wavelen 	= SDL_ReadLE32(src);
	if ( wavelen == WAVE )		/* The RIFFchunk has already been read */
	{
		WAVEmagic = wavelen;
		wavelen   = RIFFchunk;
		RIFFchunk = RIFF;
	}
	else
	{
		WAVEmagic = SDL_ReadLE32(src);
	}
	if ( (RIFFchunk != RIFF) || (WAVEmagic != WAVE) )
	{
		SDL_SetError_bbb("Unrecognized file type (not WAVE)");
		was_error = 1;
		goto done;
	}

	/* Read the audio data format chunk */
	chunk.data = NULL;
	do {
		if ( chunk.data != NULL )
		{
			free(chunk.data);
		}
		lenread = ReadChunk(src, &chunk);
		if ( lenread < 0 )
		{
			was_error = 1;
			goto done;
		}
	} while ( (chunk.magic == FACT) || (chunk.magic == LIST) );

	/* Decode the audio data format */
	format = (WaveFMT *)chunk.data;
	if ( chunk.magic != FMT )
	{
		SDL_SetError_bbb("Complex WAVE files not supported");
		was_error = 1;
		goto done;
	}
	MS_ADPCM_encoded = IMA_ADPCM_encoded = 0;
	switch (SDL_SwapLE16(format->encoding))
	{
	case PCM_CODE:
		/* We can understand this */
		break;
	case MS_ADPCM_CODE:
		/* Try to understand this */
		if ( InitMS_ADPCM(format) < 0 )
		{
			was_error = 1;
			goto done;
		}
		MS_ADPCM_encoded = 1;
		break;
	case IMA_ADPCM_CODE:
		/* Try to understand this */
		if ( InitIMA_ADPCM(format) < 0 )
		{
			was_error = 1;
			goto done;
		}
		IMA_ADPCM_encoded = 1;
		break;
	default:
		SDL_SetError_bbb("Unknown WAVE data format: 0x%.4x",
				SDL_SwapLE16(format->encoding));
		was_error = 1;
		goto done;
	}
	memset(spec, 0, (sizeof *spec));
	spec->freq = SDL_SwapLE32(format->frequency);
	switch (SDL_SwapLE16(format->bitspersample))
	{
	case 4:
		if ( MS_ADPCM_encoded || IMA_ADPCM_encoded )
		{
			spec->format = AUDIO_S16;
		}
		else
		{
			was_error = 1;
		}
		break;
	case 8:
		spec->format = AUDIO_U8;
		break;
	case 16:
		spec->format = AUDIO_S16;
		break;
	default:
		was_error = 1;
		break;
	}
	if ( was_error )
	{
		SDL_SetError_bbb("Unknown %d-bit PCM data format",
			SDL_SwapLE16(format->bitspersample));
		goto done;
	}
	spec->channels_is_load_wave_only		= (u8)SDL_SwapLE16(format->channels);
	spec->samples 		= (4096);		/* Good default buffer size */

	/* Read the audio data chunk */
	*audio_buf = NULL;
	do {
		if ( *audio_buf != NULL )
		{
			free(*audio_buf);
		}
		lenread = ReadChunk(src, &chunk);
		if ( lenread < 0 )
		{
			was_error = 1;
			goto done;
		}
		*audio_len = lenread;
		*audio_buf = chunk.data;
	} while ( chunk.magic != DATA );

	if ( MS_ADPCM_encoded )
	{
		if ( MS_ADPCM_decode(audio_buf, audio_len) < 0 )
		{
			was_error = 1;
			goto done;
		}
	}
	if ( IMA_ADPCM_encoded )
	{
		if ( IMA_ADPCM_decode(audio_buf, audio_len) < 0 )
		{
			was_error = 1;
			goto done;
		}
	}

	/* Don't return a buffer that isn't a multiple of samplesize */
	samplesize 	= ((spec->format & 0xff)/8) * spec->channels_is_load_wave_only;
	*audio_len 	&= ~(samplesize-1);

done:
	if ( format != NULL )
	{
		free(format);
	}
	if ( freesrc && src )
	{
		SDL_RWclose(src);
	}
	if ( was_error )
	{
		spec = NULL;
	}
	return (spec);
}

/* Since the WAV memory is allocated in the shared library, it must also
   be freed here.  (Necessary under Win32, VC++)
 */
void PSPL_FreeWAV(u8 *audio_buf)
{
	if ( audio_buf != NULL )
	{
		free(audio_buf);
	}
}

static int ReadChunk(SDL_RWops *src, Chunk *chunk)
{
	chunk->magic	= SDL_ReadLE32(src);
	chunk->length	= SDL_ReadLE32(src);
	chunk->data = (u8 *)malloc(chunk->length);
	if ( chunk->data == NULL )
	{
		SDL_Error_bbb(SDL_ENOMEM);
		return (-1);
	}
	if ( SDL_RWread(src, chunk->data, chunk->length, 1) != 1 )
	{
		SDL_Error_bbb(SDL_EFREAD);
		free(chunk->data);
		return (-1);
	}
	return (chunk->length);
}

#endif /* ENABLE_FILE */
