

#if (1)/* CORDIC */

// 1851352 CORDIC
// 1851336 vfpu

#include "psp_vfpu.h"
/*---------------------------------------------------------
	atan 65536 int
---------------------------------------------------------*/

static int vfpu_atani65536(float xxx)
{
//	float result;
	int r65536;
	// result = asinf(x/sqrt(x*x+1))
	__asm__ volatile (
		"mtv      %1, S000\n"				// S000 = xxx;
		"vmul.s   S001, S000, S000\n"		// S001 = S000 * S000;	// S001 = (x*x);
		"vadd.s   S001, S001, S001[1]\n"	// S001 = S001 + 1;(constant 1, ���ʒ萔1???)	// S001 = (x*x+1);
		"vrsq.s   S001, S001\n" 			// S001 = 1/sqrt(S001)
		"vmul.s   S000, S000, S001\n"		// S000 = S000 * S001;	// S000 = x*(1/sqrt(x*x+1));
		"vasin.s  S000, S000\n" 			// S000 = asinf(S000);
//		"vcst.s   S001, VFPU_PI_2\n"		// VFPU_PI_2 = PI/2;
	//	"vfim.s   S001, 128.0\n"			// S001 = 128.0;	// constant real
	//	"viim.s   S001, 128\n"				// S001 = 128;		// constant integer (between -32768 and 32767 ?);
	//	"viim.s   S001, 256\n"				// S001 = 256;		// constant integer (between -32768 and 32767 ?);
	//	"viim.s   S001, 16384\n"			// S001 = (256*64);		// constant integer (between -32768 and 32767 ?);
		"viim.s   S001, 64\n"				// S001 = (64); 		// constant integer (between -32768 and 32767 ?);
		"vmul.s   S000, S000, S001\n"		// S000 = S000 * S001;
		"vf2iu.s  S000, S000,    8\n"		// S000 = (int)(S000 * (256.0f)); [float��int�ϊ�] */
		"mfv      %0,   S000\n" 			// result = (S000); 					(���v���ԕs��(���Ԃ�1clock), latency �s��)
//	: "=r"(result) : "r"(xxx));
	: "=r"(r65536) : "r"(xxx));
//	int r65536;
//	r65536 = (((result)) /** (256.0f/2.0f)*/);
	//	r *= (256.0f/2.0f);
	return (r65536);
}

#if 1
	#include <stdlib.h>/*abs()*/
	int atan_65536( int yyy, int xxx )
	{
	//	float y;	y=xxx;
	//	float x;	x=yyy;
		int r65536;
		#if 1
		if ( abs(yyy) >= abs(xxx))
		{
			{
				float y;	y=xxx;
				float x;	x=yyy;
				r65536 = vfpu_atani65536((float)(y/x));
			}
			if (yyy < 0 )
			{
				#if 0
				r65536 += (xxx < 0  ? -(512*64) : (512*64) );
				#else
				r65536 += ( (512*64) );/*180�x��]*/
			//	r65536 += (xxx < 0  ? -(512*64)-(512*64) : (512*64)-(512*64) );
			//	r65536 += (xxx < 0  ? -(65536) : 0 );
				#endif
			}
		}
		else
		{
			{
				float y;	y=xxx;
				float x;	x=yyy;
				r65536 = vfpu_atani65536((float)(x/y));
			}
			r65536 = -r65536;
			#if 0
			r65536 += (xxx < 0  ? -(512*64 /2 ) : (512*64 /2 ));
			#else
			r65536 += ( (512*64 /2 ));/*90�x��]*/
		//	r65536 += (xxx < 0  ? -(512*64 /2 )-(512*64 /2 ) : (512*64 /2 )-(512*64 /2 ));
		//	r65536 += (xxx < 0  ? -(512*64 ) : (0));
		//	r65536 -= (xxx < 0  ?  (512*64 ) : (0));/*180�x��]*/
			r65536 += (xxx < 0  ?  (512*64 ) : (0));/*180�x��]*/
			#endif
		}
		#else
		#endif
	//	r512 = r512/**((512.0f)/(256.0f*2.0f))*/;
		r65536 += (65536*2);/* �K�����l�ɂ���B */
		r65536 &= (65536-1);
		return (r65536);
	}
#endif
#if 00
static float s_vfpu_atan2f(float y, float x)
{	float r;
	if (s_fabsf(x) >= s_fabsf(y))
	{
		r = s_vfpu_atanf(y/x);
		if (x < 0.0f)
		{
			if (y < 0.0f)
			{	r += (-s_PI);/* -180�x��] */}
			else
			{	r += ( s_PI);/*+180�x��]*/}
		}
	}
	else
	{
		r = s_vfpu_atanf(x/y);
		r = -r;
		{
			if (y < 0.0f)
			{	r += (-s_PI_2);/* -90�x��] */}
			else
			{	r += ( s_PI_2);/*+90�x��]*/}
		}
	}
	return (r);
}
#endif

#if 0
/* CW�̏ꍇ */
#else
/* CCW�̏ꍇ */
	/*�Ƃ肠����*/
//	#define rad2deg512(x)		( (int)((x)*((512.0)/(GU_PI*2))/*+512*/)&(512-1) )
#endif
	#if 0
/* CW�̏ꍇ */
//	return (rad2deg512(atan2(y,x)));	/* �ECW�̏ꍇ�B  ��ʓI�ȉECW �̏ꍇ�B */
	#else
/* CCW�̏ꍇ */
//	return (rad2deg512(atan2(x,y)));	/* ��CCW�̏ꍇ�B (x y �����ւ��đΏ����Ă���܂��B) */
//	return (rad2deg512(vfpu_atan2f(x,y)));	/* ��CCW�̏ꍇ�B (x y �����ւ��đΏ����Ă���܂��B) */
	#endif

#else/* CORDIC */
	#include "cordic_atan65536.h"
#endif/* CORDIC */
