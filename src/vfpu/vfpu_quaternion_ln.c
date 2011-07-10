
#include "psp_vfpu.h"

#define USE_INT_TEST 0
#if (0==USE_INT_TEST)
	/*nothing*/
#else /*(1==USE_INT_TEST)*/
	#include <pspgu.h>/* GU_PI defined. */
	extern int atan_1024(int y, int x);
#endif

#if 1/*てすと*/
// (.s(single?)は1つ、.p(pair?)は2つ、.t(triple?)は3つ、.q(quad?)は4つ、同時に処理するという意味)
void vfpu_quaternion_ln(ScePspQuatMatrix *qout, ScePspQuatMatrix *qin)
{
	//float r  = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
	//float t  = r>0.00001f? atan2(r,a[3])/r: 0.f;
	//return quat(t*a[0],t*a[1],t*a[2],0.5*log(norm(a)));
	float r0;
	float r1;
	__asm__ volatile (
		"lv.q     C010, 0 + %1\n"			// C010.x = qin.x, C010.y = qin.y, C010.z = qin.z, C010.w = qin.w;
		"vdot.t   S020, C010, C010\n"		// S020   = (C010.x * C010.x)+(C010.y * C010.y)+(C010.z * C010.z);		// r = (x^2) + (y^2) + (z^2)
		"vsqrt.s  S020, S020\n" 			// S020   = sqrt(S020); 												// r = sqrt((x^2) + (y^2) + (z^2))
		"mfv      %0, S020\n"				// r0	  = S020;
		:"=r"(r0): "m"(*qin));
//	r1 = vfpu_atan2f(r, qin->w)/r;
//	r1 = vfpu_atan2f(r0, qin->w)/r0;
	#if (0==USE_INT_TEST)
	r1 = vfpu_atan2f(r0, qin->w);
	#else /*(1==USE_INT_TEST)*/
	/* [実験中]たぶん上手くいってない。 */
	int i0;
	int i1;
	int i2;
	i0 = (int)((r0) * 256.0);
	i1 = (int)((qin->w) * 256.0);
		#if 0
	i2 = atan_1024(i0, i1);
		#else
	i2 = atan_1024(i1, i0);
		#endif
//
	r1 = i2;
	r1 *= ((GU_PI*2.0)/1024.0);
	#endif
//
	__asm__ volatile (
		"mtv      %1, S021\n"				// S021 = atan2(r0, w) ; 	// S021 = atan2(r0, w) / r0;
		"mtv      %2, S020\n"				// S020 = r0;
		"vdiv.s   S021, S021, S020\n"		// S021 = S021 / S020;													// t = atan2(r, w) / r;
		"vdot.q   S022, C010, C010\n"		//																		// norm = (x^2) + (y^2) + (z^2) + (w^2);
		"vcst.s   S023, VFPU_LOG2E\n"
		"vrcp.s   S023, S023\n"
		"vlog2.s  S013, S022\n"
		"vmul.s   S013, S013, S023\n"
		"vmul.s   S013, S013, S013[1/2]\n"
		"vscl.t   C010, C010, S021\n"
		"sv.q     C010, 0 + %0\n"
		: "=m"(*qout) : "r"(r1), "r"(r0) );
}
#endif
