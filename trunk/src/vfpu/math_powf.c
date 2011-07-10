
#include "psp_vfpu.h"

/*---------------------------------------------------------
	sqrtf,pow,mod
- powf(v, v) = 0.693145, cycles: 3488
- vfpu_powf(v, v) = 0.693145, cycles: 412
---------------------------------------------------------*/
#if 0
	#if 0
	/* psp sdk(buggy) */
	float vfpu_powf(float x, float y)
	{
		float result;
		// result = exp2f(y * log2f(x));
		__asm__ volatile (
			"mtv      %1, S000\n"
			"mtv      %2, S001\n"
			"vlog2.s  S001, S001\n"
			"vmul.s   S000, S000, S001\n"
			"vexp2.s  S000, S000\n"
			"mfv      %0, S000\n"
			: "=r"(result) : "r"(x), "r"(y));
		return (result);
	}
	#else
	/* conpatible mathlib */
	float vfpu_powf(float x, float y)
	{
		float result;
		// result = exp2f(y * log2f(x));
		__asm__ volatile (
			"mtv      %2, S000\n"
			"mtv      %1, S001\n"
			"vlog2.s  S001, S001\n"
			"vmul.s   S000, S000, S001\n"
			"vexp2.s  S000, S000\n"
			"mfv      %0, S000\n"
			: "=r"(result) : "r"(x), "r"(y));
		return (result);
	}
	#endif
#endif
