
#include "psp_vfpu.h"

/*---------------------------------------------------------
	sin,cos
	-------------------------------------------------------
	Calculate sine and cosine. ()

	@param angle	 - input in radians. ()
	@param sin_value - pointer to float for sin. ()
	@param cos_value - pointer to float for cos. ()
---------------------------------------------------------*/
#if (0)
void vfpu_sincos(float angle, float *sin_value, float *cos_value)
{
	__asm__ volatile (
		"mtv      %2, S002\n"				/* S002 = (r); */
		"vcst.s   S003, VFPU_2_PI\n"		/* S003 = (2/PI); */
		"vmul.s   S002, S002, S003\n"		/* r = r * (2/PI); */
		"vrot.p   C000, S002, [s, c]\n" 	/* S000 = sin(r), S001 = cos(r); */
		"mfv      %0, S000\n"				/* sin_value = S000; */
		"mfv      %1, S001\n"				/* cos_value = S001; */
		: "=r"(*sin_value), "=r"(*cos_value): "r"(angle));
}
#endif

#if (1)
/*---------------------------------------------------------
	sin,cos
	-------------------------------------------------------
	説明:
		一周1024分割の角度を指定して、
		t256形式のsin()とcos()の値を同時に取得します。
	-------------------------------------------------------
	原理:
		sin_value_t256 = sin1024(angle_1024);
		cos_value_t256 = cos1024(angle_1024);
	-------------------------------------------------------
	使用書式:
		vfpu_sincos(angle_1024, &sin_value_t256,&cos_value_t256);
---------------------------------------------------------*/

void int256_sincos1024(int angle_1024, int *sin_value_t256, int *cos_value_t256)
{
	__asm__ volatile (
		"mtv      %2, S002\n"				/* S002 = (angle_1024); */
	//	"vcst.s   S003, VFPU_2_PI\n"		// S003 = (2/PI); */
	//	"vmul.s   S002, S002, S003\n"		// x = x * (2/PI); */
		"vi2f.s   S002, S002,    8\n"		/* S002 = ((float)(S002)/(float)(256.0f)); [int→float変換] */
		"vrot.p   C000, S002, [s, c]\n" 	/* S000 = sin(r), S001 = cos(r); */
	//	#if (0) 	// 同じ。 */
	//	"vf2iu.s  S000, S000,    8\n"		// S000 = (int)(S000 * (256.0f)); [float→int変換] */
	//	"vf2iu.s  S001, S001,    8\n"		// S001 = (int)(S001 * (256.0f)); [float→int変換] */
	//	#else
		"vf2iu.p  C000, C000,    8\n"		/* C000.p = (int)(C000.p * (256.0f)); [float→int変換] */
	//	#endif
		"mfv      %0, S000\n"				/* sin_value_t256 = S000; */
		"mfv      %1, S001\n"				/* cos_value_t256 = S001; */
		: "=r"(*sin_value_t256), "=r"(*cos_value_t256): "r"(angle_1024));
}
#endif

