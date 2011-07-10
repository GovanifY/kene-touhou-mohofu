
#include "psp_vfpu.h"

/*---------------------------------------------------------
	sin
- sinf(v) = 0.389418, cycles: 856
- vfpu_sinf(v) = 0.389418, cycles: 160
---------------------------------------------------------*/
#if 1
float vfpu_sinf(float angle_radian)
{
	float result;
	__asm__ volatile (
		"mtv     %1, S000\n"			// x = angle_radian;
		"vcst.s  S001, VFPU_2_PI\n" 	// load (2/PI) into S001;
		"vmul.s  S000, S000, S001\n"	// x = x * (2/PI);
		"vsin.s  S000, S000\n"			// x = vfpu_sin(x);
		"mfv     %0, S000\n"			// result = x;
		: "=r"(result) : "r"(angle_radian));
	return (result);
}
#endif
