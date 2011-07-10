
#include "psp_vfpu.h"

/*---------------------------------------------------------
	arc (-1)	sin,cos,tan
---------------------------------------------------------*/

float vfpu_atanf(float x)
{
	float result;
	// result = asinf(x/sqrt(x*x+1))
	__asm__ volatile (
		"mtv      %1, S000\n"
		"vmul.s   S001, S000, S000\n"
		"vadd.s   S001, S001, S001[1]\n"
		"vrsq.s   S001, S001\n"
		"vmul.s   S000, S000, S001\n"
		"vasin.s  S000, S000\n"
		"vcst.s   S001, VFPU_PI_2\n"
		"vmul.s   S000, S000, S001\n"
		"mfv      %0, S000\n"
	: "=r"(result) : "r"(x));
	return (result);
}
