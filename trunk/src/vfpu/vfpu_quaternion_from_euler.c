#include "psp_vfpu.h"

#define USE_360_ANGLE 0
#define USE_512_ANGLE 1

#if (1==USE_360_ANGLE)

//#define PI 3.14159265358979323846f
//static const float piover_2_360 = PI/180.0f;
//static const float piover_2_360 = PI/360.0f;
//static const float piover_2_360 = (PI*2)/(360.0f*PI);
//static const float piover_2_360 = (2.0f)/(360.0f);

//vfpuは全部で128個のレジスタがある。4x4行列が8(バンク)個あるので全部で128個。
//(.s(single)は1つ、.p(pair)は2つ、.t(triple)は3つ、.q(quad)は4つ、同時に処理するという意味)
//S000 のSはsingle型       サイズは1行x1列。
//C000 のCはcolum型        サイズは1行x4列(4の部分は.sの幅)。
//R000 のRはrow型          サイズは4行x1列(4の部分は.sの幅)。
//M000 のMはMatrix(行列)型 サイズは4行x4列(4の部分は.sの幅)(行列の場合は必ずnxn行列つまり、1x1、2x2、3x3、4x4のどれか)。
//E000 のEは転置行列(Transpose Matrix) それ以外はMと同じ。
//Sabc のaはバンク(0から7)、bは行(0から3)、cは列(0から3)。

void vfpu_quaternion_from_euler(
	ScePspQuatMatrix *res,
	float rot_x512f,
	float rot_y512f,
	float rot_z512f)
{
	__asm__ volatile (
		// 引数をVFPUレジスタに代入。
		"mtv    %1, S000\n" 			// S000 = x
		"mtv    %2, S001\n" 			// S001 = y
		"mtv    %3, S002\n" 			// S002 = z
		// スケーリング。
	//	"lv.s   S010, 0(%4)\n"
		"vfim.s S010, 0.00555555555555555556\n" 	// S010 = (2.0f/360.0f);	// constant real
		"vscl.t C000, C000, S010\n" 	// x *= 2/360, y *= 2/360, z *= 2/360
		#if 0/*不要*/
	//	"vscl.t C000, C000, S010\n" 	// x *= pi/180, y *= pi/180, z *= pi/180
	//	"vfim.s S010, 0.5\n"			// S010 = 0.5
	//	"vscl.t C000, C000, S010\n" 	// x *= 0.5, y *= 0.5, z *= 0.5
	//	"vcst.s S010, VFPU_2_PI\n"		// load 2/PI into S010, S011 and S012
	//	"vscl.t C000, C000, S010\n" 	// x *= 2/PI, y *= 2/pi, z *= pi/2
		#endif
		// オイラー変換。(x軸→y軸→z軸の順序で回転する、s はsin関数、c はcos関数、という意味)
		"vrot.p C010, S000, [s, c]\n"		// S010 = sr, S011 = cr
		"vrot.p C020, S001, [s, c]\n"		// S020 = sp, S021 = cp
		"vrot.p C030, S002, [s, c]\n"		// S030 = sy, S031 = cy

		// fear the madness of prefixes
		"vmul.q R100, C010[x,y,y,x], C020[y,x,x,y]\n"
		"vmul.q R100, R100,          C030[y,x,y,x]\n"
		"vmul.q R101, C010[y,x,y,x], C020[y,x,y,x]\n"
		"vmul.q R101, R101,          C030[x,y,y,x]\n"
		"vadd.q C000, R100[x,z,0,0], R100[-y,w,0,0]\n"
		"vadd.q C000, C000, R101[0,0,x,z]\n"
		"vadd.q C000, C000, R101[0,0,-y,w]\n"
		"usv.q  C000, %0\n"
		:"=m"(*res) : "r"(rot_x512f), "r"(rot_y512f), "r"(rot_z512f) );
	//	:"=m"(*res) : "r"(rot_x512f), "r"(rot_y512f), "r"(rot_z512f), "r"(&piover_2_360));
	vfpu_quaternion_normalize(res);
}
#endif /* (1==USE_360_ANGLE) */

#if (1==USE_512_ANGLE)

//static void s_vfpu_quaternion_from_euler512(
void vfpu_quaternion_from_euler512_int(
	ScePspQuatMatrix *res,
	int rot_x512,
	int rot_y512,
	int rot_z512)
{
//	float rot_x512f;	rot_x512f = (float)rot_x512;
//	float rot_y512f;	rot_y512f = (float)rot_y512;
//	float rot_z512f;	rot_z512f = (float)rot_z512;
	__asm__ volatile (
		// 引数をVFPUレジスタに代入。
		"mtv    %1, S000\n" 				// S000 = x;
		"mtv    %2, S001\n" 				// S001 = y;
		"mtv    %3, S002\n" 				// S002 = z;
		// スケーリング。(.s(single?)は1つ、.p(peta?)は2つ、.t(tri?)は3つ、.q(quad?)は4つ、同時に処理するという意味)
	//	"vfim.s S010, 0.00390625\n" 		// S010 = (2.0f/512.0f);	// constant real
	//	"vscl.t C000, C000, S010\n" 		// x *= (2/512), y *= (2/512), z *= (2/512);
		"vi2f.t C000, C000,    8\n" 		// x *= (2/512), y *= (2/512), z *= (2/512);	// int→float変換と同時に1/256倍する。	(所要時間1clock, latency 0)
		// オイラー変換。(x軸→y軸→z軸の順序で回転する、s はsin関数、c はcos関数、という意味)
		"vrot.p C010, S000, [s, c]\n"		// S010 = sr, S011 = cr;
		"vrot.p C020, S001, [s, c]\n"		// S020 = sp, S021 = cp;
		"vrot.p C030, S002, [s, c]\n"		// S030 = sy, S031 = cy;

		// fear the madness of prefixes
		"vmul.q R100, C010[x,y,y,x], C020[y,x,x,y]\n"
		"vmul.q R100, R100,          C030[y,x,y,x]\n"
		"vmul.q R101, C010[y,x,y,x], C020[y,x,y,x]\n"
		"vmul.q R101, R101,          C030[x,y,y,x]\n"
		"vadd.q C000, R100[x,z,0,0], R100[-y,w,0,0]\n"
		"vadd.q C000, C000, R101[0,0,x,z]\n"
		"vadd.q C000, C000, R101[0,0,-y,w]\n"
		"usv.q  C000, %0\n"
	//	:"=m"(*res) : "r"(rot_x512f), "r"(rot_y512f), "r"(rot_z512f) );
		:"=m"(*res) : "r"(rot_x512), "r"(rot_y512), "r"(rot_z512) );
	vfpu_quaternion_normalize(res);
}

#endif /* (1==USE_512_ANGLE) */
//
//const float piover_2_512 = PI/512.0f;
//static const float piover_2_512 = (2.0f/512.0f);
//static const float piover_2_512 = (1.0f/256.0f);
//static const float piover_2_512 = (0.00390625f);
//
	//	"lv.s   S010, 0(%4)\n"				// S010 = piover_2_512 = (2.0f/512.0f);
	//	:"=m"(*res) : "r"(x), "r"(y), "r"(z), "r"(&piover_2_512));

//		"vfim.s S010, 0.5\n"				// S010 = 0.5
//		"vscl.t C000, C000, S010\n" 	// x *= 0.5, y *= 0.5, z *= 0.5
//		"vcst.s S010, VFPU_2_PI\n"		// load 2/PI into S010, S011 and S012
//		"vcst.s S010, 2.0f\n"				// load 2/PI into S010, S011 and S012
//		"vscl.t C000, C000, S010\n" 	// x *= 2/PI, y *= 2/pi, z *= pi/2
