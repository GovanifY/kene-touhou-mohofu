
#include "mohou_mikmod_internals.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	MikMod のミキサーを psp 向けに改変してあります。
	★ psp専用命令で負荷軽減。(かなり効果あり)
	★ 16bit ステレオ。
	★ 整数ミキシング。
	★ 4ポイントリバーブ。(現在は遅すぎるので標準ではオミット、ソースレベルではサポート)
	★ パンニング機能は無し。
	★ ドルビーサラウンド機能は無し。
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
	-------------------------------------------------------
	$Id: virtch.c, v 1.4 2000/02/11 04:49:19 hercules Exp $
	Sample mixing routines, using a 32 bits mixing buffer.
	-------------------------------------------------------
	Optional features include:
		(a) 4-step reverb (for 16 bit output only)
		(b) Interpolation of sample data during mixing
		(c) Dolby Surround Sound
---------------------------------------------------------*/

#include <stddef.h>
#include <string.h>


/*---------------------------------------------------------
	Constant definitions
	--------------------

	BITSHIFT
		Controls the maximum volume of the sound output.  All data is shifted
		right by BITSHIFT after being mixed. Higher values result in quieter
		sound and less chance of distortion.

	ミキシング割合です。
		オリジナルは小さすぎる(>>9)ので妥当な値(>>7)に変更しました。
		当然大きくすれば､その分歪みます。（というか歪んだ分は音が出ない）
		妥当というのは、SDLmixer での ogg 等 のミキシング音量に対して、
		mikmod関連は音量が小さ過ぎるので、SDLmixer で同等の音量になるように
		修正するという意味です。
---------------------------------------------------------*/

//	#define BITSHIFT		(9)/*小さ過ぎる*/
//	#define BITSHIFT		(8)
	#define BITSHIFT		(7)
//	#define BITSHIFT		(6)

#define FRACBITS 11
#define FRACMASK ((1L<<FRACBITS)-1L)

#define TICKLSIZE 8192
#define TICKWSIZE (TICKLSIZE<<1)
#define TICKBSIZE (TICKWSIZE<<1)

#define CLICK_SHIFT  6
#define CLICK_BUFFER (1L<<CLICK_SHIFT)

#define USE_RAMP_VOLUME (0)

//#define __FAST_REVERB__ 1768496







typedef struct VINFO
{
	u8 kick; 			/* =1 -> sample has to be restarted */
	u8 active;			/* =1 -> sample is playing */
	#if (1==USE_PAN_SURROUND)
	u8/*int */  vol; 			/* current volume */
	s16/*int */	pan;			/* current panning position */
	#else //(0==USE_PAN_SURROUND)
	u8/*int */  vol; 			/* current volume */
	u8/*int */  pan; 			/* current panning position */
	#endif //(USE_PAN_SURROUND)

	u16	flags;			/* 16/8 bits looping/one-shot */
	s16	handle; 		/* identifies the sample */

	u32	start;			/* start index */
	u32	size;			/* samplesize */
	u32	reppos; 		/* loop start */
	u32	repend; 		/* loop end */

	u32	frq;				/* current frequency */
	#if (1==USE_RAMP_VOLUME)
	int 	rampvol;
	#endif /* (1==USE_RAMP_VOLUME) */
	int 	lvolsel, rvolsel;	/* Volume factor in range 0-255 */
	int 	oldlvol, oldrvol;

	s32/*s32LONG*/	current;	/* current index in the sample */
	s32/*s32LONG*/	increment;	/* increment value */
} VINFO;

static	s16 **Samples;
static	VINFO *vinf = NULL;
static	VINFO *vnf;
static	int tickleft;
static	long samplesthatfit;

static	unsigned int vc_softchn;
static	s32/*s32LONG*/ idxsize;
static	s32/*s32LONG*/ idxlpos;
static	s32/*s32LONG*/ idxlend;
static	s32 *vc_tickbuf = NULL;
//static	u16 vc_mode;
//#define vc_mode_1_DMODE_STEREO		(1)
//#define vc_mode_1_DMODE_16BITS		(1)
//#define vc_mode_1_DMODE_SOFT_MUSIC	(1)

#ifdef NATIVE2_64BIT_INT
	//#define NATIVE s32LONG
#else
	#define NATIVE s32
#endif


#if (1==USE_MIK_MOD_REVERB)
	#include "virtch_reverb.h"
#endif /* (1==USE_MIK_MOD_REVERB) */


/*---------- 32 bit sample mixers - only for 32 bit platforms */
#ifndef NATIVE2_64BIT_INT

//static s32 Mix32MonoNormal(s16* srce, s32* dest, s32 index, s32 increment, s32 todo)
//{
//	while (todo--)
//	{
//		s16 sample;
//		sample = srce[index >> FRACBITS];
//		index += increment;
//		*dest++ += (vnf->lvolsel * sample);
//	}
//	return (index);
//}

static s32 Mix32StereoNormal(s16* srce, s32* dest, s32 index, s32 increment, s32 todo)
{
	while (todo--)
	{
		s16 sample;
		sample = srce[index >> FRACBITS];
		index += increment;

		*dest++ += (vnf->lvolsel * sample);
		*dest++ += (vnf->rvolsel * sample);
	}
	return (index);
}

#if (1==USE_PAN_SURROUND)
static s32 Mix32SurroundNormal(s16* srce, s32* dest, s32 index, s32 increment, s32 todo)
{
	while (todo--)
	{
		s16 sample;
		sample = srce[index >> FRACBITS];
		index += increment;

		if (vnf->lvolsel >= vnf->rvolsel)
		{
			*dest++ += (vnf->lvolsel * sample);
			*dest++ -= (vnf->lvolsel * sample);
		}
		else
		{
			*dest++ -= (vnf->rvolsel * sample);
			*dest++ += (vnf->rvolsel * sample);
		}
	}
	return (index);
}
#endif //(1==USE_PAN_SURROUND)

//static s32 Mix32MonoInterp(s16* srce, s32* dest, s32 index, s32 increment, s32 todo)
//{
//	while (todo--)
//	{
//		s32 sample;
//		sample =	(s32)srce[index>>FRACBITS] +
//				(	(s32)(srce[(index>>FRACBITS)+1]-srce[index>>FRACBITS])
//					* (index&FRACMASK)>>FRACBITS
//				);
//		index += increment;
//		#if (1==USE_RAMP_VOLUME)
//		if (vnf->rampvol)
//		{
//			*dest++ += (	((s32)vnf->lvolsel<<CLICK_SHIFT) +
//							(s32)(vnf->oldlvol-vnf->lvolsel) * vnf->rampvol
//					   ) * sample>>CLICK_SHIFT;
//			vnf->rampvol--;
//		}
//		else
//		#endif /* (1==USE_RAMP_VOLUME) */
//		{
//			*dest++ += (vnf->lvolsel * sample);
//		}
//	}
//	return (index);
//}
#define USE_INTERPOLATION	(0)
#if (1==USE_INTERPOLATION)
static s32 Mix32StereoInterp(s16* srce, s32* dest, s32 index, s32 increment, s32 todo)
{
	while (todo--)
	{
		s32 sample;
		sample =	(s32)srce[index>>FRACBITS]+
				(	(s32)(srce[(index>>FRACBITS)+1]-srce[index>>FRACBITS])
					* (index&FRACMASK)>>FRACBITS);
		index += increment;
		#if (1==USE_RAMP_VOLUME)
		if (vnf->rampvol)
		{
			*dest++ +=	(	((s32)vnf->lvolsel<<CLICK_SHIFT)+
							(s32)(vnf->oldlvol-vnf->lvolsel) * vnf->rampvol
						) * sample>>CLICK_SHIFT;
			*dest++ +=	(	((s32)vnf->rvolsel<<CLICK_SHIFT)+
							(s32)(vnf->oldrvol-vnf->rvolsel) * vnf->rampvol
						) * sample>>CLICK_SHIFT;
			vnf->rampvol--;
		}
		else
		#endif /* (1==USE_RAMP_VOLUME) */
		{
			*dest++ += (vnf->lvolsel * sample);
			*dest++ += (vnf->rvolsel * sample);
		}
	}
	return (index);
}
#endif /*(1==USE_INTERPOLATION)*/

#if (1==USE_PAN_SURROUND)
static s32 Mix32SurroundInterp(s16* srce, s32* dest, s32 index, s32 increment, s32 todo)
{
	while (todo--)
	{
		s32 sample;
		sample =	(s32)srce[index>>FRACBITS]+
					((s32)(srce[(index>>FRACBITS)+1]-srce[index>>FRACBITS])
				* (index&FRACMASK)>>FRACBITS);
		index += increment;
		//
	//	int oldvol;
		int vol;
		if (vnf->lvolsel >= vnf->rvolsel)
		{
			vol = vnf->lvolsel; //	oldvol = vnf->oldlvol;
		}
		else
		{
			vol = vnf->rvolsel; //	oldvol = vnf->oldrvol;
		}

		#if (1==USE_RAMP_VOLUME)
		if (vnf->rampvol)
		{
			sample = (	((s32)vnf->lvolsel<<CLICK_SHIFT)+
						(s32)(vnf->oldlvol-vnf->lvolsel) * vnf->rampvol
					) * sample>>CLICK_SHIFT;
			*dest++ += (sample);
			*dest++ -= (sample);
			vnf->rampvol--;
		}
		else
		#endif /* (1==USE_RAMP_VOLUME) */
		{
			*dest++ += (vol * sample);
			*dest++ -= (vol * sample);
		}
	}
	return (index);
}
#endif //(1==USE_PAN_SURROUND)

#endif

#if 0
/*---------- 64 bit sample mixers - all platforms */
#endif



////////// include "reverb"

/* Mixing macros */
#define EXTRACT_SAMPLE(var, size)	var = ((*srce++) >> (BITSHIFT+16-size))
#define PUT_SAMPLE(var) 			*dste++ = (var)

#if (0)/*(pspでは遅い)*/
	/*(psp以外の場合)*/
	#define CHECK_SAMPLE(my_sample, bound) 	my_sample = ((my_sample>=bound)?(bound-1):((my_sample<-bound)?(-bound):(my_sample)))
#else/*(psp専用)*/
//	#define Limit(my_sample, my_MAXOUT, my_MINOUT)
	#define CHECK_SAMPLE(my_sample, bound) \
		/*	if (0<my_sample) (正値) */	{	my_sample = psp_min(my_sample, (bound-1)/*my_MAXOUT*/); 	} \
		/*	else			 (負値) */	{	my_sample = psp_max(my_sample, (-bound) /*my_MINOUT*/); 	}
		/* クリップ範囲最大値でクリッピングしたいのだから、クリップ範囲最大値とどちらか小さい方 */
		/* クリップ範囲最小値でクリッピングしたいのだから、クリップ範囲最小値とどちらか大きい方 */
		/* この関数は signed int の仕様なので、正負の場合分けとかは元々要らない。 */
#endif



//static void Mix32To8(s8* dste, s32 *srce, NATIVE count)
//{
//	s16 x1, x2, x3, x4;
//	int remain;
//	remain = (count&3);
//	for (count>>=2; count; count--)
//	{
//		EXTRACT_SAMPLE(x1, 8); EXTRACT_SAMPLE(x2, 8);
//		EXTRACT_SAMPLE(x3, 8); EXTRACT_SAMPLE(x4, 8);
//
//		CHECK_SAMPLE(x1, 128); CHECK_SAMPLE(x2, 128);
//		CHECK_SAMPLE(x3, 128); CHECK_SAMPLE(x4, 128);
//
//		PUT_SAMPLE(x1+128); PUT_SAMPLE(x2+128);
//		PUT_SAMPLE(x3+128); PUT_SAMPLE(x4+128);
//	}
//	while (remain--)
//	{
//		EXTRACT_SAMPLE(x1, 8);
//		CHECK_SAMPLE(x1, 128);
//		PUT_SAMPLE(x1+128);
//	}
//}

static void AddChannel(s32* ptr, NATIVE todo)
{
	s32/*s32LONG*/ end;
	s32 done;
	s16 *s;
	s = Samples[vnf->handle];
	if (!(s))
	{
		vnf->current = vnf->active	= 0;
		return;
	}
	/* update the 'current' index so the sample loops, or stops playing if it
	   reached the end of the sample */
	while (todo>0)
	{
		s32/*s32LONG*/ endpos;
		if (vnf->flags & SF_REVERSE)
		{
			/* The sample is playing in reverse */
			if ((vnf->flags&SF_LOOP)&&(vnf->current<idxlpos))
			{
				/* the sample is looping and has reached the loopstart index */
				if (vnf->flags & SF_BIDI)
				{
					/* sample is doing bidirectional loops, so 'bounce' the
					   current index against the idxlpos */
					vnf->current = idxlpos+(idxlpos-vnf->current);
					vnf->flags &= ~SF_REVERSE;
					vnf->increment = -vnf->increment;
				}
				else
				{
					/* normal backwards looping, so set the current position to
					   loopend index */
					vnf->current=idxlend-(idxlpos-vnf->current);
				}
			}
			else
			{
				/* the sample is not looping, so check if it reached index 0 */
				if (vnf->current < 0)
				{
					/* playing index reached 0, so stop playing this sample */
					vnf->current = vnf->active	= 0;
					break;
				}
			}
		}
		else
		{
			/* The sample is playing forward */
			if ((vnf->flags & SF_LOOP) && (vnf->current >= idxlend))
			{
				/* the sample is looping, check the loopend index */
				if (vnf->flags & SF_BIDI)
				{
					/* sample is doing bidirectional loops, so 'bounce' the current index against the idxlend. */
					vnf->flags |= SF_REVERSE;
					vnf->increment = -vnf->increment;
					vnf->current = idxlend-(vnf->current-idxlend);
				}
				else
				{
					/* normal backwards looping, so set the current position to loopend index. */
					vnf->current = idxlpos+(vnf->current-idxlend);
				}
			}
			else
			{
				/* sample is not looping, so check if it reached the last position. */
				if (vnf->current >= idxsize)
				{
					/* yes, so stop playing this sample */
					vnf->current = vnf->active	= 0;
					break;
				}
			}
		}

		end =	(vnf->flags&SF_REVERSE) ?
				(vnf->flags&SF_LOOP) ? (idxlpos):(0):
				(vnf->flags&SF_LOOP) ? (idxlend):(idxsize);

		/* if the sample is not blocked... */
		if ((end==vnf->current) || (0==vnf->increment))
		{	done = 0;}
		else
		{
			done = psp_min( ((end-vnf->current) / (vnf->increment)+1), todo );/* ??? */
			done = psp_max( done, 0);//if (done < 0)	{	done = 0;}
		}

		if (0==done)
		{
			vnf->active = 0;
			break;
		}

		endpos = vnf->current + done * vnf->increment;

		if (vnf->vol)
		{
//			#ifndef NATIVE2_64BIT_INT
			/* use the 32 bit mixers as often as we can (they're much faster) */
//			if ((vnf->current<0x7fffffff)&&(endpos<0x7fffffff))
			{
				#if (1==USE_INTERPOLATION)
				if ((mm_d_mode & DMODE_INTERP))
				{
				//	if (vc_mode_1_DMODE_STEREO)
					{
						#if (1==USE_PAN_SURROUND)
					//	if ((vnf->pan==PAN_SURROUND)&&(vc_mode&DMODE_SURROUND))
					//	{	vnf->current = Mix32SurroundInterp		(s, ptr, vnf->current, vnf->increment, done);	}
					//	else
						#endif //(1==USE_PAN_SURROUND)
						{	vnf->current = Mix32StereoInterp		(s, ptr, vnf->current, vnf->increment, done);	}
					}
				//	else
				//	{		vnf->current = Mix32MonoInterp			(s, ptr, vnf->current, vnf->increment, done);	}
				}
				else
				#endif /*(1==USE_INTERPOLATION)*/
			//	if (vc_mode_1_DMODE_STEREO)
				{
					#if (1==USE_PAN_SURROUND)
				//	if ((vnf->pan==PAN_SURROUND)&&(vc_mode & DMODE_SURROUND))
				//	{	vnf->current = Mix32SurroundNormal			(s, ptr, vnf->current, vnf->increment, done);	}
				//	else
					#endif //(1==USE_PAN_SURROUND)
					{		vnf->current = Mix32StereoNormal		(s, ptr, vnf->current, vnf->increment, done);	}
				}
			//	else
			//	{			vnf->current = Mix32MonoNormal			(s, ptr, vnf->current, vnf->increment, done);	}
			}
//			else
//			#endif
//			{
//				if ((mm_d_mode & DMODE_INTERP))
//				{
//					if (vc_mode_1_DMODE_STEREO)
//					{
//						if ((vnf->pan==PAN_SURROUND)&&(vc_mode & DMODE_SURROUND))
//						{	vnf->current = MixSurroundInterp		(s, ptr, vnf->current, vnf->increment, done);	}
//						else
//						{	vnf->current = MixStereoInterp			(s, ptr, vnf->current, vnf->increment, done);	}
//					} else
//					{		vnf->current = MixMonoInterp			(s, ptr, vnf->current, vnf->increment, done);	}
//				}
//				else if (vc_mode_1_DMODE_STEREO)
//				{
//					if ((vnf->pan==PAN_SURROUND)&&(vc_mode & DMODE_SURROUND))
//					{		vnf->current = MixSurroundNormal		(s, ptr, vnf->current, vnf->increment, done);	}
//					else
//					{		vnf->current = MixStereoNormal			(s, ptr, vnf->current, vnf->increment, done);	}
//				}
//				else
//				{			vnf->current = MixMonoNormal			(s, ptr, vnf->current, vnf->increment, done);	}
//			}
		}
		else
		{
			/* update sample position */
			vnf->current = endpos;
		}

		todo -= done;
		#if 1
	//	if (vc_mode_1_DMODE_STEREO)
		{	ptr += done+done;}
	//	else
	//	{	ptr += done;}
		#else
	//	ptr += (vc_mode_1_DMODE_STEREO)?(done<<1):done;
		#endif
	}
}


/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
	-------------------------------------------------------
	$Id: virtch_common.c, v 1.4 2003/09/21 18:15:31 slouken Exp $
	Common source parts between the two software mixers.
	This file is probably the ugliest part of libmikmod...
---------------------------------------------------------*/

/* [[ virtch32.c からの include 部分 ]] */

#if 1 //ndef _VIRTCH_COMMON_
//#define _VIRTCH_COMMON_

static u32 samples2bytes(u32 samples)
{
/*	if (vc_mode_1_DMODE_16BITS) */	{samples <<= 1;}
/*	if (vc_mode_1_DMODE_STEREO) */	{samples <<= 1;}
	return (samples);
}

static u32 bytes2samples(u32 bytes)
{
/*	if (vc_mode_1_DMODE_16BITS) */	{bytes >>= 1;}
/*	if (vc_mode_1_DMODE_STEREO) */	{bytes >>= 1;}
	return (bytes);
}





void /*VC1_Exit*/MOD_VIRTUAL_MIXER_Exit(void)
{
	if (vc_tickbuf) 	{	free(vc_tickbuf);}
	if (vinf)			{	free(vinf);}
	if (Samples)		{	free(Samples);}

	vc_tickbuf	= NULL;
	vinf		= NULL;
	Samples 	= NULL;

//	MOD_VIRTUAL_MIXER_SetupPointers();/*(r35-無し)*/
}

u16 /*VC1_VoiceGetVolume*/MOD_VIRTUAL_MIXER_VoiceGetVolume(u8 voice)
{
	return vinf[voice].vol;
}

u32 /*VC1_VoiceGetPanning*/MOD_VIRTUAL_MIXER_VoiceGetPanning(u8 voice)
{
	return vinf[voice].pan;
}

void /*VC1_VoiceSetFrequency*/MOD_VIRTUAL_MIXER_VoiceSetFrequency(u8 voice, u32 frq)
{
	vinf[voice].frq=frq;
}

u32 /*VC1_VoiceGetFrequency*/MOD_VIRTUAL_MIXER_VoiceGetFrequency(u8 voice)
{
	return vinf[voice].frq;
}

void /*VC1_VoicePlay*/MOD_VIRTUAL_MIXER_VoicePlay(
	u8 voice,
	s16 handle,
	u32 start,
	u32 size,
	u32 reppos,
	u32 repend,
	u16 flags)
{
	vinf[voice].flags	 = flags;
	vinf[voice].handle	 = handle;
	vinf[voice].start	 = start;
	vinf[voice].size	 = size;
	vinf[voice].reppos	 = reppos;
	vinf[voice].repend	 = repend;
	vinf[voice].kick	 = 1;
}

void /*VC1_VoiceStop*/MOD_VIRTUAL_MIXER_VoiceStop(u8 voice)
{
	vinf[voice].active = 0;
}

MM_BOOL /*VC1_VoiceStopped*/MOD_VIRTUAL_MIXER_VoiceStopped(u8 voice)
{
	return (vinf[voice].active==0);
}

s32 /*VC1_VoiceGetPosition*/MOD_VIRTUAL_MIXER_VoiceGetPosition(u8 voice)
{
	return (vinf[voice].current>>FRACBITS);
}

#if 1/* チャンネルごとの音量指定機能 */

/* ここは必ず virtch32.cからincludeされてるので既に設定済み */
//#define USE_RAMP_VOLUME (0)

void /*VC1_VoiceSetVolume*/MOD_VIRTUAL_MIXER_VoiceSetVolume(u8 voice, u16 vol)
{
	/* protect against clicks if volume variation is too high */
	#if (1==USE_RAMP_VOLUME)
	if (abs((int)vinf[voice].vol-(int)vol)>32)
	{
		vinf[voice].rampvol = CLICK_BUFFER;
	}
	#endif /* (1==USE_RAMP_VOLUME) */
	vinf[voice].vol = vol;
}

void /*VC1_VoiceSetPanning*/MOD_VIRTUAL_MIXER_VoiceSetPanning(u8 voice, u32 pan)
{
	#if (1==USE_RAMP_VOLUME)
	/* protect against clicks if panning variation is too high */
	if (abs((int)vinf[voice].pan-(int)pan)>48)
	{
		vinf[voice].rampvol = CLICK_BUFFER;
	}
	#endif /* (1==USE_RAMP_VOLUME) */
	vinf[voice].pan = pan;
}
#endif
/*---------- External mixer interface */

void /*VC1_SampleUnload*/MOD_VIRTUAL_MIXER_SampleUnload(s16 handle)
{
	if (Samples && handle<MAXSAMPLEHANDLES)
	{
		if (Samples[handle])
		{	free(Samples[handle]);}
		Samples[handle] = NULL;
	}
}

s16 /*VC1_SampleLoad*/MOD_VIRTUAL_MIXER_SampleLoad_soft_ware(/*struct*/ MOD_MUSIC_INTERNAL_SAMPLE_LOAD* sload)//, int type)
{
	MM_SAMPLE *s = sload->sample;
	int handle;
	u32 t, length, loopstart, loopend;

//	if (type==MD_HARDWARE)	{return (-1);}

	/* Find empty slot to put sample address in */
	for (handle=0; handle<MAXSAMPLEHANDLES; handle++)
	{	if (!Samples[handle])	{break;}}

	if (handle==MAXSAMPLEHANDLES)
	{
		mod_music_error_number = MOD_MUSIC_ERROR_OUT_OF_HANDLES;
		return (-1);
	}

	length	  	= s->length;
	loopstart 	= s->loopstart;
	loopend   	= s->loopend;

	/* Fix loops */
	if (((loopend-loopstart)>2) && (loopstart > loopend) && (loopstart>length))
	{
		loopstart /= 2;
	}

	SL_SampleSigned(sload);
	SL_Sample8to16(sload);
	Samples[handle] = (s16*)mod_music_malloc((length+20)<<1);
	if (!Samples[handle])
	{
		mod_music_error_number = MOD_MUSIC_ERROR_SAMPLE_TOO_BIG;
		return (-1);
	}

	/* read sample into buffer */
	if (SL_Load(Samples[handle], sload, length))
	{	return (-1);}

	/* Unclick sample */
	if (s->flags & SF_LOOP)
	{
		if (s->flags & SF_BIDI)
		{	for (t=0; t<16; t++)
			{	Samples[handle][loopend+t] = Samples[handle][(loopend-t)-1];}}
		else
		{	for (t=0; t<16; t++)
			{	Samples[handle][loopend+t] = Samples[handle][t+loopstart];}}
	}
	else
	{	for (t=0; t<16; t++)
		{	Samples[handle][t+length] = 0;
		}
	}
	return (handle);
}




u32 /*VC1_VoiceRealVolume*/MOD_VIRTUAL_MIXER_VoiceRealVolume(u8 voice)
{
	u32 i, s, size;
	int k, j;
	s16 *smp;
	s32 t;

	t = vinf[voice].current>>FRACBITS;
	if (!vinf[voice].active)	{return (0);}

	s = vinf[voice].handle;
	size = vinf[voice].size;

	i=64; t-=64; k=0; j=0;
	if (i>size) 		{i = size;}
	if (t<0)			{t = 0;}
	if (t+i > size) 	{t = size-i;}

	i &= ~1;  /* make sure it's EVEN. */

	smp = &Samples[s][t];
	for (; i; i--, smp++)
	{
		if (k < (*smp)) 	{k = (*smp);}
		if (j > (*smp)) 	{j = (*smp);}
	}
	return (abs(k-j));
}

#endif /* _VIRTCH_COMMON_ */

/* ex:set ts=4: */

static void Mix32To16(s16* dste, s32 *srce, NATIVE count)
{
	s32 x1, x2, x3, x4;
	int remain = (count&3);
	/* 4つ単位で転送 */
	for (count>>=2; count; count--)
	{
		EXTRACT_SAMPLE(x1, 16); EXTRACT_SAMPLE(x2, 16);
		EXTRACT_SAMPLE(x3, 16); EXTRACT_SAMPLE(x4, 16);

		CHECK_SAMPLE(x1, 32768); CHECK_SAMPLE(x2, 32768);
		CHECK_SAMPLE(x3, 32768); CHECK_SAMPLE(x4, 32768);

		PUT_SAMPLE(x1); PUT_SAMPLE(x2); PUT_SAMPLE(x3); PUT_SAMPLE(x4);
	}
	/* 残りを転送 */
	while (remain--)
	{
		EXTRACT_SAMPLE(x1, 16);
		CHECK_SAMPLE(x1, 32768);
		PUT_SAMPLE(x1);
	}
}
/*static*/ void s_VC1_WriteSamples(s8 *buf, u32 todo)
{
	while (todo)
	{
		#if (1)/*(いまいち良く判らない)*/
		if (!tickleft)
		{
		/*	if (vc_mode_1_DMODE_SOFT_MUSIC) */	{	md_player();}
		//	tickleft = (/*m d_mixfreq*/44100*125L)/(md_bpm*50L);
			tickleft = (/*m d_mixfreq*/(44100/50)*125L) / (md_bpm);
		}
		#endif
		int left;
		left = psp_min(tickleft, ((signed)todo) );
		tickleft -= left;
		todo	 -= left;

		s8  *buffer;
		buffer	  = buf;
		buf += samples2bytes(left);

		while (left)
		{
			int count;
			int portion /*= 0*/;
			portion = psp_min(left, samplesthatfit);
			count	= /*(vc_mode_1_DMODE_STEREO)?*/(portion<<1)/*:portion*/;
			#if 0
			memset(vc_tickbuf, 0, count<<2);
			#else
			memset32(vc_tickbuf, 0, count);/*(てすと)*/
			#endif
			{	unsigned int t;
				for (t=0; t<vc_softchn; t++)
				{
					vnf = &vinf[t];

					if (vnf->kick)
					{
						vnf->current	= ((s32/*s32LONG*/)vnf->start)<<FRACBITS;
						vnf->kick		= 0;
						vnf->active 	= 1;
					}

					if (!vnf->frq)	{	vnf->active = 0;}

					if (vnf->active)
					{
						vnf->increment = ((s32/*s32LONG*/)(vnf->frq<<FRACBITS))/44100/*m d_mixfreq*/;
						if (vnf->flags&SF_REVERSE)	{	vnf->increment = -vnf->increment;	}
						int vol = vnf->vol;
						int pan = vnf->pan;

						vnf->oldlvol = vnf->lvolsel;
						vnf->oldrvol = vnf->rvolsel;
					//	if (vc_mode_1_DMODE_STEREO)
						{
							#if (1==USE_PAN_SURROUND)
						//	if (pan != PAN_SURROUND)
							#endif //(1==USE_PAN_SURROUND)
							{
								vnf->lvolsel = (vol*(PAN_RIGHT-pan))>>8;
								vnf->rvolsel = (vol*(		   pan))>>8;
							}
							#if (1==USE_PAN_SURROUND)
						//	else
						//	{
						//		vnf->lvolsel = vnf->rvolsel=vol/2;
						//	}
							#endif //(1==USE_PAN_SURROUND)
						}
					//	else
					//	{
					//		vnf->lvolsel = vol;
					//	}

						idxsize = (vnf->size)  ? ((s32/*s32LONG*/)vnf->size   << FRACBITS)-1 : 0;
						idxlend = (vnf->repend)? ((s32/*s32LONG*/)vnf->repend << FRACBITS)-1 : 0;
						idxlpos = (s32/*s32LONG*/)vnf->reppos << FRACBITS;
						AddChannel(vc_tickbuf, portion);
					}
				}
			}
			#if (1==USE_MIK_MOD_REVERB)
		//	mod_config_reverb = 7;/* 7 == (m d_15)test */
		//	mod_config_reverb = 3;/* 3 == (m d_7) test */
			mod_config_reverb = 2;/* 2 == (m d_4) test */
			if (mod_config_reverb)
			{
				if (7 < mod_config_reverb)
				{
					mod_config_reverb = 7;
				}
				MixReverb(vc_tickbuf, portion);
			}
			#endif /* (1==USE_MIK_MOD_REVERB) */
		//	if (vc_mode_1_DMODE_16BITS)
			{
				Mix32To16((s16*) buffer, vc_tickbuf, count);
			}
		//	else
		//	{	Mix32To8((s8*) buffer, vc_tickbuf, count);}
			buffer += samples2bytes(portion);
			left   -= portion;
		}
	}
}
//	/*static*/ void s_VC1_WriteSamples(s8 *buf, u32 todo);

/*
	Writes 'todo' mixed s8S (!!) to 'buf'.
	It returns the number of s8S actually written to 'buf'
	(which is rounded to number of samples that fit into 'todo' bytes).
*/
u32 /*VC1_WriteBytes*/MOD_VIRTUAL_MIXER_WriteBytes(s8* buf, u32 todo)
{
	if (0==vc_softchn)
	{
	//	return /*VC1_SilenceBytes*/s_MOD_VIRTUAL_MIXER_SilenceBytes(buf, todo);
		/* Fill the buffer with 'todo' bytes of silence (it depends on the mixing mode
		   how the buffer is filled) */
	//	static u32 /*VC1_SilenceBytes*/s_MOD_VIRTUAL_MIXER_SilenceBytes(s8* buf, u32 todo)
	//	{
		#if 0/*on(memset32)*/
			todo = samples2bytes(bytes2samples(todo));
		#endif
			/* clear the buffer to zero (16 bits signed) or 0x80 (8 bits unsigned) */
		//	if (vc_mode_1_DMODE_16BITS)
			{	memset(buf, 0, todo);}/*memset32*/
		//	else
		//	{	memset(buf, 0x80, todo);}
			return (todo);
	//	}
	}
	todo = bytes2samples(todo);
	s_VC1_WriteSamples(buf, todo);
	return (samples2bytes(todo));
}

MM_BOOL /*VC1_Init*/MOD_VIRTUAL_MIXER_Init(void)
{
//	MOD_VIRTUAL_MIXER_SetupPointers();/*(r35-無し)*/
//	if (mm_d_mode & DMODE_HQMIXER)
//	{	return VC2_Init();}
	Samples = (s16**)mod_music_calloc(MAXSAMPLEHANDLES, sizeof(s16*));
	if (NULL==Samples)
	{
		mod_music_error_number = MOD_MUSIC_ERROR_INITIALIZING_MIXER;
		return (1);
	}
	if (NULL==vc_tickbuf)
	{
		vc_tickbuf = (s32*)mod_music_malloc((TICKLSIZE+32)*sizeof(s32));
		if (NULL==vc_tickbuf)
		{
			mod_music_error_number = MOD_MUSIC_ERROR_INITIALIZING_MIXER;
			return (1);
		}
	}
	#if (1==USE_MIK_MOD_REVERB)
	MixReverb = /*(mm_d_mode & DMODE_STEREO)?*/(MixReverb_Stereo)/*:(MixReverb_Monoral)*/;
	#endif /* (1==USE_MIK_MOD_REVERB) */
//	vc_mode = mm_d_mode;
	return (0);
}


MM_BOOL /*VC1_PlayStart*/MOD_VIRTUAL_MIXER_PlayStart(void)
{
	samplesthatfit = TICKLSIZE;
/*	if (vc_mode_1_DMODE_STEREO) */	{samplesthatfit >>= 1;}
	tickleft = (0);
	#if (1==USE_MIK_MOD_REVERB)
	return s_reverb_init();
	#else
	return (0);
	#endif /* (1==USE_MIK_MOD_REVERB) */
}

void /*VC1_PlayStop*/MOD_VIRTUAL_MIXER_PlayStop(void)
{
	#if (1==USE_MIK_MOD_REVERB)
	s_reverb_stop();
	#endif /* (1==USE_MIK_MOD_REVERB) */
}

MM_BOOL /*VC1_SetNumVoices*/MOD_VIRTUAL_MIXER_SetNumVoices(void)
{
	vc_softchn = md_softchn;
	if (0==vc_softchn)	{return (0);}
	//
	if (vinf)	{free(vinf);}
	vinf = mod_music_calloc(sizeof(VINFO), vc_softchn);
	if (NULL==vinf) 	{return (1);}
	//
	{	unsigned int t;
		for (t=0; t<vc_softchn; t++)
		{
			vinf[t].frq = (10000);
			vinf[t].pan = ((t&1)?PAN_LEFT:PAN_RIGHT);
		}
	}
	return (0);
}

/* ex:set ts=4: */
