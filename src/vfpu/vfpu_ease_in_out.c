#include "psp_vfpu.h"
/*
	ease in/out というのは、
	アニメーションで動きの強弱が
	「最初と最後をゆっくりで、間は速い。」という方式です。
	linear				直線補間
	ease-in 			始めだけゆっくり
	ease-out			最後だけゆっくり
	ease-in-out 		始めと最後だけゆっくりで、間は速い。
	cubic-bezier		ベジェ補間
*/
// simple ease in/out function
// input will be clamped to range of (0.00f < anime_time < 1.00f).

float vfpu_ease_in_out(float in_anime_time)
{
	float out_anime_time;
	__asm__ volatile (
		"mtv     %1, S000\n"							// S000 = in_anime_time;
		"vmov.q  C000[0:1,,,], C000[x, 2, 1, 1/2]\n"	// C000 = [0<in_anime_time<1, 2, 1, 1/2];
		"vmul.s  S000, S000, S001\n"					// raise S000 to 0..2 range;
		"vsub.s  S000, S000, S002\n"					// S000 now in -1 to 1 range;
		"vsin.s  S000, S000\n"							// S000 = sin(S000);
		"vmul.s  S000, S000, S003\n"					// S000 = sin(in_anime_time)/2;
		"vadd.s  S000, S000, S003\n"					// S000 = 0.5 + sin(in_anime_time)/2;
		"mfv     %0, S000\n"							// out_anime_time = S000;
	: "=r"(out_anime_time) : "r"(in_anime_time));
	return (out_anime_time);
}
