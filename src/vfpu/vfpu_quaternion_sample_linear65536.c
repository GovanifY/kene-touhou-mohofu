#include "psp_vfpu.h"
/*
	2つの4x4行列(クォータニオン)を、合成する。

	ScePspQuatMatrix *qout					合成結果の4x4行列(クォータニオン)

	ScePspQuatMatrix *a,					1つ目の4x4行列(クォータニオン)
	ScePspQuatMatrix *b,					2つ目の4x4行列(クォータニオン)

	float multiplex_time1_00
	vfpu_quaternion_sample_linear();		合成割合は、0.00 から 1.00 の不動少数で指定する。

	int multiplex_time65536
	vfpu_quaternion_sample_linear65536(); 	合成割合は、0 から 65536 の(unsigned int)整数で指定する。

*/
#if 0
void vfpu_quaternion_sample_linear(
	ScePspQuatMatrix *qout,
	ScePspQuatMatrix *a,					/* 1つ目の4x4行列 */
	ScePspQuatMatrix *b,					/* 2つ目の4x4行列 */
	float multiplex_time1_00				/* 合成割合 float time, value from(0.00 to 1.00) */
)
{
	__asm__ volatile (
		"lv.q     C000, 0 + %1\n"			/* C000 = (a); */
		"lv.q     C010, 0 + %2\n"			/* C010 = (b); */
		"mtv      %3, S020\n"				/* S020 = (multiplex_time1_00); */
		"vocp.s   S021, S020\n" 			/* S021 = (1)-(S020);たぶん */
		"vscl.q   C000, C000, S021\n"		/* C000 *= S021; 4x4行列をスケーリング */
		"vscl.q   C010, C010, S020\n"		/* C010 *= S020; 4x4行列をスケーリング */
		"vadd.q   C000, C000, C010\n"		/* C000 += C010; */
		"sv.q     C000, 0 + %0\n"			/* (qout) = C000; */
	: "=m"(*qout) : "m"(*a), "m"(*b), "r"(multiplex_time1_00));
}
#endif

#if 1
void vfpu_quaternion_sample_linear65536(
	ScePspQuatMatrix *qout,
	ScePspQuatMatrix *a,
	ScePspQuatMatrix *b,
	u32 multiplex_time256				/* 合成割合 float time, value from(0 to 255) */
)
{
	__asm__ volatile (
		"lv.q     C000, 0 + %1\n"			/* C000 = (a); */
		"lv.q     C010, 0 + %2\n"			/* C010 = (b); */
		"mtv      %3, S020\n"				/* S020 = (multiplex_time1_00); */
		"vi2f.s   S020, S020,   16\n"		// int→float変換と同時に1/65536倍する。	(所要時間1clock, latency 0)
		"vocp.s   S021, S020\n" 			/* S021 = (1)-(S020);たぶん */
		"vscl.q   C000, C000, S021\n"		/* C000 *= S021; 4x4行列をスケーリング */
		"vscl.q   C010, C010, S020\n"		/* C010 *= S020; */
		"vadd.q   C000, C000, C010\n"		/* C000 += C010; */
		"sv.q     C000, 0 + %0\n"			/* (qout) = C000; */
	: "=m"(*qout) : "m"(*a), "m"(*b), "r"(multiplex_time256));
}
#endif
