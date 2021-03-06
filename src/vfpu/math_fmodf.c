
#include "psp_vfpu.h"

/*---------------------------------------------------------
	sqrtf,pow,mod
---------------------------------------------------------*/

float vfpu_fmodf(float x, float y)
{
	float result;
	// return x-y*((int)(x/y));
	__asm__ volatile (
		"mtv       %2, S001\n"			//	S001 = (y);
		"mtv       %1, S000\n"			//	S000 = (x);
		"vrcp.s    S002, S001\n"
		"vmul.s    S003, S000, S002\n"
		"vf2iz.s   S002, S003, 0\n"
		"vi2f.s    S003, S002, 0\n"		//(S003) = (float)(S002);		// int→float変換。	(所要時間1clock, latency 0)
		"vmul.s    S003, S003, S001\n"
		"vsub.s    S000, S000, S003\n"
		"mfv       %0, S000\n"
	: "=r"(result) : "r"(x), "r"(y));
	return (result);
}
