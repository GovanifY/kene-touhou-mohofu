
#include "psp_vfpu.h"

/*---------------------------------------------------------
	arc (-1)	sin,cos,tan
- acosf(v) = 1.159279, cycles: 1433
- vfpu_acosf(v) = 1.159280, cycles: 107
---------------------------------------------------------*/

float vfpu_acosf(float x)
{
	float result;
	__asm__ volatile (
		"mtv     %1, S000\n"
		"vcst.s  S001, VFPU_PI_2\n"
		"vasin.s S000, S000\n"
		"vocp.s  S000, S000\n"
		"vmul.s  S000, S000, S001\n"
		"mfv     %0, S000\n"
		: "=r"(result) : "r"(x));
	return (result);
}
