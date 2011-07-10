#include "psp_vfpu.h"

#define USE_360_ANGLE 0
#define USE_512_ANGLE 1

#if (1==USE_360_ANGLE)

//#define PI 3.14159265358979323846f
//static const float piover_2_360 = PI/180.0f;
//static const float piover_2_360 = PI/360.0f;
//static const float piover_2_360 = (PI*2)/(360.0f*PI);
//static const float piover_2_360 = (2.0f)/(360.0f);

//vfpu�͑S����128�̃��W�X�^������B4x4�s��8(�o���N)����̂őS����128�B
//(.s(single)��1�A.p(pair)��2�A.t(triple)��3�A.q(quad)��4�A�����ɏ�������Ƃ����Ӗ�)
//S000 ��S��single�^       �T�C�Y��1�sx1��B
//C000 ��C��colum�^        �T�C�Y��1�sx4��(4�̕�����.s�̕�)�B
//R000 ��R��row�^          �T�C�Y��4�sx1��(4�̕�����.s�̕�)�B
//M000 ��M��Matrix(�s��)�^ �T�C�Y��4�sx4��(4�̕�����.s�̕�)(�s��̏ꍇ�͕K��nxn�s��܂�A1x1�A2x2�A3x3�A4x4�̂ǂꂩ)�B
//E000 ��E�͓]�u�s��(Transpose Matrix) ����ȊO��M�Ɠ����B
//Sabc ��a�̓o���N(0����7)�Ab�͍s(0����3)�Ac�͗�(0����3)�B

void vfpu_quaternion_from_euler(
	ScePspQuatMatrix *res,
	float rot_x512f,
	float rot_y512f,
	float rot_z512f)
{
	__asm__ volatile (
		// ������VFPU���W�X�^�ɑ���B
		"mtv    %1, S000\n" 			// S000 = x
		"mtv    %2, S001\n" 			// S001 = y
		"mtv    %3, S002\n" 			// S002 = z
		// �X�P�[�����O�B
	//	"lv.s   S010, 0(%4)\n"
		"vfim.s S010, 0.00555555555555555556\n" 	// S010 = (2.0f/360.0f);	// constant real
		"vscl.t C000, C000, S010\n" 	// x *= 2/360, y *= 2/360, z *= 2/360
		#if 0/*�s�v*/
	//	"vscl.t C000, C000, S010\n" 	// x *= pi/180, y *= pi/180, z *= pi/180
	//	"vfim.s S010, 0.5\n"			// S010 = 0.5
	//	"vscl.t C000, C000, S010\n" 	// x *= 0.5, y *= 0.5, z *= 0.5
	//	"vcst.s S010, VFPU_2_PI\n"		// load 2/PI into S010, S011 and S012
	//	"vscl.t C000, C000, S010\n" 	// x *= 2/PI, y *= 2/pi, z *= pi/2
		#endif
		// �I�C���[�ϊ��B(x����y����z���̏����ŉ�]����As ��sin�֐��Ac ��cos�֐��A�Ƃ����Ӗ�)
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
		// ������VFPU���W�X�^�ɑ���B
		"mtv    %1, S000\n" 				// S000 = x;
		"mtv    %2, S001\n" 				// S001 = y;
		"mtv    %3, S002\n" 				// S002 = z;
		// �X�P�[�����O�B(.s(single?)��1�A.p(peta?)��2�A.t(tri?)��3�A.q(quad?)��4�A�����ɏ�������Ƃ����Ӗ�)
	//	"vfim.s S010, 0.00390625\n" 		// S010 = (2.0f/512.0f);	// constant real
	//	"vscl.t C000, C000, S010\n" 		// x *= (2/512), y *= (2/512), z *= (2/512);
		"vi2f.t C000, C000,    8\n" 		// x *= (2/512), y *= (2/512), z *= (2/512);	// int��float�ϊ��Ɠ�����1/256�{����B	(���v����1clock, latency 0)
		// �I�C���[�ϊ��B(x����y����z���̏����ŉ�]����As ��sin�֐��Ac ��cos�֐��A�Ƃ����Ӗ�)
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
