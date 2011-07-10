
#include "psp_vfpu.h"

/*---------------------------------------------------------
	vfpu乱数を扱う関数群
ABGR 8888 として考えた場合の、乱数色を生成する。
(pspは画面モードと関係なく、色指定がABGR8888に固定の部分がある)
---------------------------------------------------------*/

unsigned int vfpu_rand_8888(int min, int max)
{
	unsigned int result;
	__asm__ volatile (
		"mtv      %1, S020\n"				/* S020 = (min); */
		"mtv      %2, S021\n"				/* S021 = (max); */
		"vmov.t   C000, C020[x, x, x]\n"	/* [x = S020 = (min);] S000 = (min); S001 = (min); S002 = (min); */
		"vmov.t   C010, C020[y, y, y]\n"	/* [y = S021 = (max);] S010 = (max); S011 = (max); S012 = (max); */
		"vi2f.t   C000, C000, 0\n"			// (S000) = (float)(S000);	(S001) = (float)(S001); (S002) = (float)(S002); 	// [int→float変換。] */
		"vi2f.t   C010, C010, 0\n"			// (S010) = (float)(S010);	(S011) = (float)(S011); (S012) = (float)(S012); 	// [int→float変換。] */
		"vsub.t   C010, C010, C000\n"
		"vrndf1.t C020\n"
		"vsub.t   C020, C020, C020[1, 1, 1]\n"
		"vmul.t   C020, C020, C010\n"
		"vadd.t   C020, C020, C000\n"
		"vf2iz.t  C020, C020, 23\n"
		"viim.s   S023, 255\n"
		"vf2iz.s  S023, S023, 23\n"
		"vi2uc.q  S000, C020\n"
		"mfv      %0, S000\n"
	:"=r"(result): "r"(min), "r"(max));
	return (result);
}
