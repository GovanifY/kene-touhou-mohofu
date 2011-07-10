
#include "psp_vfpu.h"

/*---------------------------------------------------------
	sqrtf,pow,mod

- sqrtf(v) = 0.632456, cycles: 40
- vfpu_sqrtf(v) = 0.632455, cycles: 240 	<== Slower on VFPU !

---------------------------------------------------------*/
#if 0
float vfpu_sqrtf(float x)
{
	float result;
	__asm__ volatile (
		"mtv     %1, S000\n"
		"vsqrt.s S000, S000\n"
		"mfv     %0, S000\n"
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
	#define pspFpuSqrt vfpu_sqrtf
	#if 1	//0=fast, 1=original
	/* original(?) */
	inline float pspFpuSqrt(float fs)
	{
		return (__builtin_allegrex_sqrt_s(fs));
	}
	#else
	/* fast(?) */
	inline float pspFpuSqrt(float fs)
	{
		union
		{
			int tmp;
			float fpv;
		} uni;
		uni.fpv = fs;
		uni.tmp = (1<<29) + (uni.tmp >> 1) - (1<<22) - 311296;
		return (uni.fpv);
	}
	#endif
#endif
