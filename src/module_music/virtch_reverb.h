
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは virtch32.c からインクルードされます。
	単体ではコンパイルしません。
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
---------------------------------------------------------*/

#if (1==USE_MIK_MOD_REVERB)

/*
	 8: 8ポイントリバーブを使用する。(残響音を 8サンプル 先まで計算する)
標準 4: 4ポイントリバーブを使用する。(残響音を 4サンプル 先まで計算する)
*/
//	#define USE_REVERB_POINTS (8)
	#define USE_REVERB_POINTS (4)

/* Reverb control variables */
static	u32 RVRindex;
//
static	int RVc1;
static	int RVc2;
static	int RVc3;
static	int RVc4;
#if (8<=USE_REVERB_POINTS)
static	int RVc5;
static	int RVc6;
static	int RVc7;
static	int RVc8;
#endif /* (8<=USE_REVERB_POINTS) */

/* For Mono or Left Channel */
static	s32 *RVbufL1 = NULL;
static	s32 *RVbufL2 = NULL;
static	s32 *RVbufL3 = NULL;
static	s32 *RVbufL4 = NULL;
#if (8<=USE_REVERB_POINTS)
static	s32 *RVbufL5 = NULL;
static	s32 *RVbufL6 = NULL;
static	s32 *RVbufL7 = NULL;
static	s32 *RVbufL8 = NULL;
#endif /* (8<=USE_REVERB_POINTS) */

/* For Stereo only (Right Channel) */
static	s32 *RVbufR1 = NULL;
static	s32 *RVbufR2 = NULL;
static	s32 *RVbufR3 = NULL;
static	s32 *RVbufR4 = NULL;
#if (8<=USE_REVERB_POINTS)
static	s32 *RVbufR5 = NULL;
static	s32 *RVbufR6 = NULL;
static	s32 *RVbufR7 = NULL;
static	s32 *RVbufR8 = NULL;
#endif /* (8<=USE_REVERB_POINTS) */

static void (*MixReverb)(s32* srce, NATIVE count);

/* Reverb macros */
#define COMPUTE_LOC(n) loc##n = RVRindex % RVc##n
#define COMPUTE_LECHO(n) RVbufL##n [loc##n ]=speedup+((ReverbPct*RVbufL##n [loc##n ])>>7)
#define COMPUTE_RECHO(n) RVbufR##n [loc##n ]=speedup+((ReverbPct*RVbufR##n [loc##n ])>>7)

static void MixReverb_Monoral(s32 *srce, NATIVE count)
{
	unsigned int speedup;
	int ReverbPct;
	unsigned int loc1,loc2,loc3,loc4;
	#if (8<=USE_REVERB_POINTS)
	unsigned int loc5,loc6,loc7,loc8;
	#endif /* (8<=USE_REVERB_POINTS) */

	ReverbPct = 58+(mod_config_reverb+mod_config_reverb);

	COMPUTE_LOC(1); COMPUTE_LOC(2); COMPUTE_LOC(3); COMPUTE_LOC(4);
	#if (8<=USE_REVERB_POINTS)
	COMPUTE_LOC(5); COMPUTE_LOC(6); COMPUTE_LOC(7); COMPUTE_LOC(8);
	#endif /* (8<=USE_REVERB_POINTS) */

	while (count--)
	{
		/* Compute the left channel echo buffers */
		speedup = (*srce) >> 3;

		COMPUTE_LECHO(1); COMPUTE_LECHO(2); COMPUTE_LECHO(3); COMPUTE_LECHO(4);
		#if (8<=USE_REVERB_POINTS)
		COMPUTE_LECHO(5); COMPUTE_LECHO(6); COMPUTE_LECHO(7); COMPUTE_LECHO(8);
		#endif /* (8<=USE_REVERB_POINTS) */

		/* Prepare to compute actual finalized data */
		RVRindex++;

		COMPUTE_LOC(1); COMPUTE_LOC(2); COMPUTE_LOC(3); COMPUTE_LOC(4);
		#if (8<=USE_REVERB_POINTS)
		COMPUTE_LOC(5); COMPUTE_LOC(6); COMPUTE_LOC(7); COMPUTE_LOC(8);
		#endif /* (8<=USE_REVERB_POINTS) */

		/* left channel */
		#if (8<=USE_REVERB_POINTS)
		*srce++ += RVbufL1[loc1]-RVbufL2[loc2]+RVbufL3[loc3]-RVbufL4[loc4]+
				   RVbufL5[loc5]-RVbufL6[loc6]+RVbufL7[loc7]-RVbufL8[loc8];
		#else
		*srce++ += RVbufL1[loc1]-RVbufL2[loc2]+RVbufL3[loc3]-RVbufL4[loc4];
		#endif
	}
}


#if 0

static void MixReverb_Stereo(s32 *srce, NATIVE count)
{
	unsigned int speedup;
	int 		 ReverbPct;
	unsigned int loc1, loc2, loc3, loc4;
	unsigned int loc5, loc6, loc7, loc8;

//	ReverbPct = 92+(mod_config_reverb);
	ReverbPct = 92+(mod_config_reverb);

	COMPUTE_LOC(1); COMPUTE_LOC(2); COMPUTE_LOC(3); COMPUTE_LOC(4);
	COMPUTE_LOC(5); COMPUTE_LOC(6); COMPUTE_LOC(7); COMPUTE_LOC(8);

	while (count--)
	{
		/* Compute the left channel echo buffers */
		speedup = (*srce) >> 3;

		COMPUTE_LECHO(1); COMPUTE_LECHO(2); COMPUTE_LECHO(3); COMPUTE_LECHO(4);
		COMPUTE_LECHO(5); COMPUTE_LECHO(6); COMPUTE_LECHO(7); COMPUTE_LECHO(8);

		/* Compute the right channel echo buffers */
		speedup = (srce[1]) >> 3;

		COMPUTE_RECHO(1); COMPUTE_RECHO(2); COMPUTE_RECHO(3); COMPUTE_RECHO(4);
		COMPUTE_RECHO(5); COMPUTE_RECHO(6); COMPUTE_RECHO(7); COMPUTE_RECHO(8);

		/* Prepare to compute actual finalized data */
		RVRindex++;

		COMPUTE_LOC(1); COMPUTE_LOC(2); COMPUTE_LOC(3); COMPUTE_LOC(4);
		COMPUTE_LOC(5); COMPUTE_LOC(6); COMPUTE_LOC(7); COMPUTE_LOC(8);

		/* left channel then right channel */
		*srce++ += RVbufL1[loc1]-RVbufL2[loc2]+RVbufL3[loc3]-RVbufL4[loc4]+
				   RVbufL5[loc5]-RVbufL6[loc6]+RVbufL7[loc7]-RVbufL8[loc8];

		*srce++ += RVbufR1[loc1]-RVbufR2[loc2]+RVbufR3[loc3]-RVbufR4[loc4]+
				   RVbufR5[loc5]-RVbufR6[loc6]+RVbufR7[loc7]-RVbufR8[loc8];
	}
}
#else

/* Reverb macros */
#define COMPUTE_LOC_(n) loc##n = RVRindex % RVc##n
#define COMPUTE_LECHO_(n) RVbufL##n [loc##n ]=speedup+((ReverbPct2*RVbufL##n [loc##n ])>>8)
#define COMPUTE_RECHO_(n) RVbufR##n [loc##n ]=speedup+((ReverbPct2*RVbufR##n [loc##n ])>>8)

static void MixReverb_Stereo(s32 *srce, NATIVE count)
{
	unsigned int speedup;
	int 		 ReverbPct2;
	unsigned int loc1, loc2, loc3, loc4;
	#if (8<=USE_REVERB_POINTS)
	unsigned int loc5, loc6, loc7, loc8;
	#endif /* (8<=USE_REVERB_POINTS) */

	ReverbPct2 = (92<<1)+(mod_config_reverb+mod_config_reverb);

	COMPUTE_LOC(1); COMPUTE_LOC(2); COMPUTE_LOC(3); COMPUTE_LOC(4);
	#if (8<=USE_REVERB_POINTS)
	COMPUTE_LOC(5); COMPUTE_LOC(6); COMPUTE_LOC(7); COMPUTE_LOC(8);
	#endif /* (8<=USE_REVERB_POINTS) */
	while (count--)
	{
		/* Compute the left channel echo buffers */
		speedup = (*srce) >> 3;

		COMPUTE_LECHO_(1); COMPUTE_LECHO_(2); COMPUTE_LECHO_(3); COMPUTE_LECHO_(4);
		#if (8<=USE_REVERB_POINTS)
		COMPUTE_LECHO_(5); COMPUTE_LECHO_(6); COMPUTE_LECHO_(7); COMPUTE_LECHO_(8);
		#endif /* (8<=USE_REVERB_POINTS) */

		/* Compute the right channel echo buffers */
		speedup = (srce[1]) >> 3;

		COMPUTE_RECHO_(1); COMPUTE_RECHO_(2); COMPUTE_RECHO_(3); COMPUTE_RECHO_(4);
		#if (8<=USE_REVERB_POINTS)
		COMPUTE_RECHO_(5); COMPUTE_RECHO_(6); COMPUTE_RECHO_(7); COMPUTE_RECHO_(8);
		#endif /* (8<=USE_REVERB_POINTS) */

		/* Prepare to compute actual finalized data */
		RVRindex++;

		COMPUTE_LOC(1); COMPUTE_LOC(2); COMPUTE_LOC(3); COMPUTE_LOC(4);
		#if (8<=USE_REVERB_POINTS)
		COMPUTE_LOC(5); COMPUTE_LOC(6); COMPUTE_LOC(7); COMPUTE_LOC(8);
		#endif /* (8<=USE_REVERB_POINTS) */

		/* left channel then right channel */
		#if (8<=USE_REVERB_POINTS)
		*srce++ += RVbufL1[loc1]-RVbufL2[loc2]+RVbufL3[loc3]-RVbufL4[loc4]+
				   RVbufL5[loc5]-RVbufL6[loc6]+RVbufL7[loc7]-RVbufL8[loc8];
		//
		*srce++ += RVbufR1[loc1]-RVbufR2[loc2]+RVbufR3[loc3]-RVbufR4[loc4]+
				   RVbufR5[loc5]-RVbufR6[loc6]+RVbufR7[loc7]-RVbufR8[loc8];
		#else
		*srce++ += RVbufL1[loc1]-RVbufL2[loc2]+RVbufL3[loc3]-RVbufL4[loc4];
		*srce++ += RVbufR1[loc1]-RVbufR2[loc2]+RVbufR3[loc3]-RVbufR4[loc4];
		#endif
	}
}

#endif

/*
   Constant definitions
   --------------------

	REVERBERATION
		Controls the duration of the reverb. Larger values represent a shorter
		reverb loop. Smaller values extend the reverb but can result in more of
		an echo-ish sound.

	リバーブ(残響音効果)
		大きな値にすると、リバーブが少なくなる。
		小さな値にすると、リバーブが多くなるがエコーが、かかりすぎた音になる。
*/
static int s_reverb_init(void)
{
	#if 0
		#define REVERBERATION	110000L
		#if (8<=USE_REVERB_POINTS)
			RVc1 = (5000L * 44100/*m d_mixfreq*/) / REVERBERATION;
			RVc2 = (5078L * 44100/*m d_mixfreq*/) / REVERBERATION;
			RVc3 = (5313L * 44100/*m d_mixfreq*/) / REVERBERATION;
			RVc4 = (5703L * 44100/*m d_mixfreq*/) / REVERBERATION;
			RVc5 = (6250L * 44100/*m d_mixfreq*/) / REVERBERATION;
			RVc6 = (6953L * 44100/*m d_mixfreq*/) / REVERBERATION;
			RVc7 = (7813L * 44100/*m d_mixfreq*/) / REVERBERATION;
			RVc8 = (8828L * 44100/*m d_mixfreq*/) / REVERBERATION;
		#else
    		RVc1 = (5000L * 44100/*m d_mixfreq*/) / (REVERBERATION * 2);
    		RVc2 = (5946L * 44100/*m d_mixfreq*/) / (REVERBERATION * 2);
    		RVc3 = (7071L * 44100/*m d_mixfreq*/) / (REVERBERATION * 2);
    		RVc4 = (8409L * 44100/*m d_mixfreq*/) / (REVERBERATION * 2);
		#endif
	#else
		/* x 1.678466796875 */
		//	#define REVERBERATION	 65536L
		#if (8<=USE_REVERB_POINTS)
			RVc1 = (( 8392L * 44100/*m d_mixfreq*/) >> 16);/*  8392.333984375 */
			RVc2 = (( 8523L * 44100/*m d_mixfreq*/) >> 16);/*  8523.25439453125 */
			RVc3 = (( 8917L * 44100/*m d_mixfreq*/) >> 16);/*  8917.694091796875 */
			RVc4 = (( 9572L * 44100/*m d_mixfreq*/) >> 16);/*  9572.296142578125 */
			RVc5 = ((10490L * 44100/*m d_mixfreq*/) >> 16);/* 10490.41748046875 */
			RVc6 = ((11670L * 44100/*m d_mixfreq*/) >> 16);/* 11670.379638671875 */
			RVc7 = ((13113L * 44100/*m d_mixfreq*/) >> 16);/* 13113.861083984375 */
			RVc8 = ((14817L * 44100/*m d_mixfreq*/) >> 16);/* 14817.5048828125 */
		#else
		//	RVc1 = (( 4096L * 44100/*m d_mixfreq*/) >> 16);/*  4196.1669921875 */
		//	RVc2 = (( 4352L * 44100/*m d_mixfreq*/) >> 16);/*  4458.8470458984375 */
		//	RVc3 = (( 5120L * 44100/*m d_mixfreq*/) >> 16);/*  5245.208740234375 */
		//	RVc4 = (( 6912L * 44100/*m d_mixfreq*/) >> 16);/*  6556.9305419921875 */
			RVc1 = (( 4196L * 44100/*m d_mixfreq*/) >> 16);/*  4196.1669921875 */
			RVc2 = (( 4458L * 44100/*m d_mixfreq*/) >> 16);/*  4458.8470458984375 */
			RVc3 = (( 5245L * 44100/*m d_mixfreq*/) >> 16);/*  5245.208740234375 */
			RVc4 = (( 6556L * 44100/*m d_mixfreq*/) >> 16);/*  6556.9305419921875 */
			//
	//		RVc1 = (( 4196L * 44100/*m d_mixfreq*/) >> 16);/*  4196.1669921875 */
	//		RVc2 = (( 4990L * 44100/*m d_mixfreq*/) >> 16);/*  4990.081787109375 */
	//		RVc3 = (( 5934L * 44100/*m d_mixfreq*/) >> 16);/*  5934.2193603515625 */
	//		RVc4 = (( 7057L * 44100/*m d_mixfreq*/) >> 16);/*  7057.1136474609375 */
		#endif
	#endif
	RVbufL1 = (s32*)mod_music_calloc((RVc1+1), sizeof(s32));if (NULL==RVbufL1) return (1);
	RVbufL2 = (s32*)mod_music_calloc((RVc2+1), sizeof(s32));if (NULL==RVbufL2) return (1);
	RVbufL3 = (s32*)mod_music_calloc((RVc3+1), sizeof(s32));if (NULL==RVbufL3) return (1);
	RVbufL4 = (s32*)mod_music_calloc((RVc4+1), sizeof(s32));if (NULL==RVbufL4) return (1);
	#if (8<=USE_REVERB_POINTS)
	RVbufL5 = (s32*)mod_music_calloc((RVc5+1), sizeof(s32));if (NULL==RVbufL5) return (1);
	RVbufL6 = (s32*)mod_music_calloc((RVc6+1), sizeof(s32));if (NULL==RVbufL6) return (1);
	RVbufL7 = (s32*)mod_music_calloc((RVc7+1), sizeof(s32));if (NULL==RVbufL7) return (1);
	RVbufL8 = (s32*)mod_music_calloc((RVc8+1), sizeof(s32));if (NULL==RVbufL8) return (1);
	#endif /* (8<=USE_REVERB_POINTS) */
	RVbufR1 = (s32*)mod_music_calloc((RVc1+1), sizeof(s32));if (NULL==RVbufR1) return (1);
	RVbufR2 = (s32*)mod_music_calloc((RVc2+1), sizeof(s32));if (NULL==RVbufR2) return (1);
	RVbufR3 = (s32*)mod_music_calloc((RVc3+1), sizeof(s32));if (NULL==RVbufR3) return (1);
	RVbufR4 = (s32*)mod_music_calloc((RVc4+1), sizeof(s32));if (NULL==RVbufR4) return (1);
	#if (8<=USE_REVERB_POINTS)
	RVbufR5 = (s32*)mod_music_calloc((RVc5+1), sizeof(s32));if (NULL==RVbufR5) return (1);
	RVbufR6 = (s32*)mod_music_calloc((RVc6+1), sizeof(s32));if (NULL==RVbufR6) return (1);
	RVbufR7 = (s32*)mod_music_calloc((RVc7+1), sizeof(s32));if (NULL==RVbufR7) return (1);
	RVbufR8 = (s32*)mod_music_calloc((RVc8+1), sizeof(s32));if (NULL==RVbufR8) return (1);
	#endif /* (8<=USE_REVERB_POINTS) */
	RVRindex = 0;
	return (0);
}
static void s_reverb_stop(void)
{
	if (NULL != RVbufL1) free(RVbufL1);
	if (NULL != RVbufL2) free(RVbufL2);
	if (NULL != RVbufL3) free(RVbufL3);
	if (NULL != RVbufL4) free(RVbufL4);
	#if (8<=USE_REVERB_POINTS)
	if (NULL != RVbufL5) free(RVbufL5);
	if (NULL != RVbufL6) free(RVbufL6);
	if (NULL != RVbufL7) free(RVbufL7);
	if (NULL != RVbufL8) free(RVbufL8);
	#endif /* (8<=USE_REVERB_POINTS) */
	RVbufL1 = RVbufL2 = RVbufL3 = RVbufL4 =
	#if (8<=USE_REVERB_POINTS)
	RVbufL5 = RVbufL6 = RVbufL7 = RVbufL8 =
	#endif /* (8<=USE_REVERB_POINTS) */
		NULL;
//
	if (NULL != RVbufR1) free(RVbufR1);
	if (NULL != RVbufR2) free(RVbufR2);
	if (NULL != RVbufR3) free(RVbufR3);
	if (NULL != RVbufR4) free(RVbufR4);
	#if (8<=USE_REVERB_POINTS)
	if (NULL != RVbufR5) free(RVbufR5);
	if (NULL != RVbufR6) free(RVbufR6);
	if (NULL != RVbufR7) free(RVbufR7);
	if (NULL != RVbufR8) free(RVbufR8);
	#endif /* (8<=USE_REVERB_POINTS) */
	RVbufR1 = RVbufR2 = RVbufR3 = RVbufR4 =
	#if (8<=USE_REVERB_POINTS)
	RVbufR5 = RVbufR6 = RVbufR7 = RVbufR8 =
	#endif /* (8<=USE_REVERB_POINTS) */
		NULL;
}

#endif /* (1==USE_MIK_MOD_REVERB) */

/* ex:set ts=4: */
