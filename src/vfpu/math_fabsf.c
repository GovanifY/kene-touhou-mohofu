
#include "psp_vfpu.h"

/*---------------------------------------------------------
	abs
---------------------------------------------------------*/

	#if 0
float vfpu_fabsf(float x)
{
	float result;
	__asm__ volatile (
		"mtv     %1, S000\n"
		"vmov.s  S000, S000[|x|]\n"
		"mfv     %0, S000\n"
	: "=r"(result) : "r"(x));
	return (result);
}
	#endif

/*---------------------------------------------------------
	abs
- fabsf(v) = 0.400000, cycles: 7
- vfpu_fabsf(v) = 0.400000, cycles: 93	<== Slower on VFPU !
---------------------------------------------------------*/
	#if 0
	// Below Function taken from PGE - Phoenix Game Engine - Greets InsertWittyName !
	/* slow  */
//	inline float vfpu_abs(float x)
	inline float vfpu_fabsf(float x)
	{
		float result;
		__asm__ volatile (
			"mtv      %1, S000\n"
			"vabs.s   S000, S000\n"
			"mfv      %0, S000\n"
		: "=r"(result) : "r"(x));
		return (result);
	}
	#endif

#if 0
	//*****************************************************************************
	//FPU Math :D
	//*****************************************************************************
	/*
	sqrtf and fabsf are alot slower on the vfpuv, so let's do em on the fpu.
	Check above notes for cycles/comparison
	*/

	#if 1	//0=fast, 1=original //Corn
	/* original(?) */
	inline float pspFpuAbs(float fs)
	{
		register float fd;
		asm (
			"abs.s %0, %1\n"
			: "=f"(fd)
			: "f"(fs)
		);
		return (fd);
	}
	#else
	/* fast(?) */
	inline float pspFpuAbs(float fs)
	{
		union		//This could be slower than the real deal absf() due to mem access?
		{
			int tmp;
			float fpv;
		} uni;
		uni.fpv = fs;
		uni.tmp = uni.tmp & 0x7FFFFFFF;
		return (uni.fpv);
	}
	#endif

#endif
