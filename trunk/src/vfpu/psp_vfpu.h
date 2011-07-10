/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * Copyright (c) 2007 Steve Galatis
 * Copyright (c) 2007 Christophe Avoinme
 *
 * General purpose math library using vfpu optimized instructions.
 * Most of the routines here run at least 5 to 20 times faster than the equivalent
 * routines in libm
 * Changes:
 * 2009-11-20: modified pow(x,y);
 *		float vfpu_powf(float y, float x); to
 *		float vfpu_powf(float x, float y); then compatible arguments of mathlib pow(x,y).
 *	VFPU�g�����C�u�����ł����APSPSDK�̌��ł� mathlib ��pow()�̈����Ɍ݊�������܂���B
 *	�����ő�����������mathlib�݊��Ɉ��������킹�Ă���܂��B
 *	�W���� mathlib(libm�A -lm) �g�����ꍇ�ɔ�ׂ� 5 �` 20 �{�A���������ł��B
 *
 */

#ifndef _psp_math_h_
#define _psp_math_h_

#include <psptypes.h>

/** @defgroup VFPUMATH VFPU Math Library (VFPU���Z���C�u����)
 *
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*---------------------------------------------------------
	�N�H�[�^�j�I�����`
---------------------------------------------------------*/

typedef struct quat 	/* �N�H�[�^�j�I�����` */
{
	float x;	/* x���v�f */
	float y;	/* y���v�f */
	float z;	/* z���v�f */
	float w;	/* w�v�f */
} __attribute__((aligned(16))) ScePspQuatMatrix;

typedef struct quat_t256 	/* �N�H�[�^�j�I�����` */
{
	int x256;	/* x���v�f */
	int y256;	/* y���v�f */
	int z256;	/* z���v�f */
	int w256;	/* w�v�f */
} __attribute__((aligned(16))) quaternion_t256;

/** @addtogroup VFPUMATH */
/*@{*/

/*---------------------------------------------------------
	���w�֐�
	-------------------------------------------------------
	psp �ł͂����̐��w�֐��� cpu �ł͒x���̂� vfpu ���g���܂��B
---------------------------------------------------------*/

/**
  * Calculate sine. ()
  *
  * @param x - input in radians. ()
  *
**/
float vfpu_sinf(float x);
//float vfpu_sini(float x);
//[sincos���g���B�g�p���Ȃ�]int int256_sin1024(int angle_1024);
//[sincos���g���B�g�p���Ȃ�]int int256_sin65536(int angle_65536);
/**
  * Calculate cosine. ()
  *
  * @param x - input in radians. ()
  *
**/
float vfpu_cosf(float x);

/**
  * Calculate tangent. ()
  *
  * @param x - input in radians. ()
  *
**/
float vfpu_tanf(float x);

/**
  * Calculate inverse sine (arcsin). ()
  *
  * @param x - input. ()
  *
**/
float vfpu_asinf(float x);

/**
  * Calculate inverse cosine (arccos). ()
  *
  * @param x - input. ()
  *
**/
float vfpu_acosf(float x);

/**
  * Calculate inverse tangent (arctan). ()
  *
  * @param x - input. ()
  *
**/
float vfpu_atanf(float x);

/**
  * Calculate inverse tangent, with proper quadrant fixup. ()
  *
  * @param x - input. ()
  *
**/
float vfpu_atan2f(float x, float y);

/**
  * Calculate hyperbolic sine. ()
  *
  * @param x - input. ()
  *
**/
float vfpu_sinhf(float x);

/**
  * Calculate hyperbolic cosine. ()
  *
  * @param x - input. ()
  *
**/
float vfpu_coshf(float x);

/**
  * Calculate hyperbolic tangent. ()
  *
  * @param x - input. ()
  *
**/
float vfpu_tanhf(float x);

/**
  * Calculate sine and cosine. ()
  *
  * @param angle     - input in radians. ()
  * @param sin_value - pointer to float for sin. ()
  * @param cos_value - pointer to float for cos. ()
**/
void vfpu_sincos(float angle, float *sin_value, float *cos_value);
void int256_sincos1024(int angle_1024, int *sin_value_t256, int *cos_value_t256);

/**
  * Calculate exponent of x. ()
  *
  * @param x - input. ()
  *
**/
float vfpu_expf(float x);

/**
  * Calculate logarithm of x. ()
  *
  * @param x - input. ()
  *
**/
float vfpu_logf(float x);
#if 1
/**
  * Calculate abs of x. ()
  *
  * @param x - input. ()
  *
**/
float vfpu_fabsf(float x);

/**
  * Calculate sqrt of x. ()
  *
  * @param x - input. ()
  *
**/
float vfpu_sqrtf(float x);
#endif
/**
  * Calculate x raised to the power of y.(psp swap perameter)
  *
  * @param x - number to raise power of. ()
  * @param y - power to raise x by. ()
  *
**/
float vfpu_powf(float x, float y);	/* for my fixed custum lib only.(compatible mathlib) */
//float vfpu_powf(float y, float x);	/* vfpu_powf(y, x) == powf(x, y) */ 	/* for PSPSDK(not compatible mathlib) */

/**
  * Calculate floating point remainder of x/y. ()
  *
  * @param x - input. ()
  * @param y - input. ()
  *
**/
float vfpu_fmodf(float x, float y);
#if 1
/**
  * Calculate minimum, x or y. ()
  *
  * @param x - input. ()
  * @param y - input. ()
  *
**/
float vfpu_fminf(float x, float y);

/**
  * Calculate maximum, x or y. ()
  *
  * @param x - input. ()
  * @param y - input. ()
  *
**/
float vfpu_fmaxf(float x, float y);
#endif

/*---------------------------------------------------------
	vfpu�����������֐��Q
---------------------------------------------------------*/

/**
  * Set to vfpu random generator seed. (VFPU����������̗����V�[�h(�)��ݒ肷��B)
  *
  * @param x - seed value. (x: �����V�[�h�l)
  *
**/
void vfpu_srand(unsigned int x);

/**
  * Return to get random float value. (���������_�`���ŁA�����l���擾����B)
  *
  * @param min - minimum value to return
  * @param max - maximum value to return
  *
**/
float vfpu_randf(float min, float max);

/**
  * Return to get random color value in 8888 format. (32bit8888�F�`���ŁA�����F���擾����B)
  * This always sets the alpha channel value to 0xff. (�A���t�@�͏�ɂȂ� == 0xffBBGGRR )
  *
  * @param min - minimum value for each color channel (0..255). (�eBGR�F�̍ŏ��l)
  * @param max - maximum value for each color channel (0..255). (�eBGR�F�̍ő�l)
  *
**/
unsigned int vfpu_rand_8888(int min, int max);


#if 1

/*---------------------------------------------------------
	�A�j���[�V�����p�⏕�֐�
---------------------------------------------------------*/

/**
  * �A�j���[�V�����p�⏕�֐��F
	ease in/out �Ƃ����̂́A
	�A�j���[�V�����œ����̋��オ
	�u�ŏ��ƍŌ���������ŁA�Ԃ͑����B�v�Ƃ��������ł��B
	linear				�������
	ease-in 			�n�߂����������
	ease-out			�Ōゾ���������
	ease-in-out 		�n�߂ƍŌゾ���������ŁA�Ԃ͑����B
	cubic-bezier		�x�W�F���
  * Perform a smooth acceleration/deceleration curve based on the input time value
  * returns 0 to 1. ()
  *
  * @param t - input (0 to 1 float). ()
  * @param t65536 - input (0 to 65536). (u32)
  *
**/
float vfpu_ease_in_out(float t);
u32 vfpu_ease_in_out65536(u32 t65536);
#endif


/*---------------------------------------------------------
	�s��������֐��Q
---------------------------------------------------------*/

/**
  * Generate an identity matrix. (�P�ʍs��̐���)
  *
  * @param m - pointer to matrix to set. (�Z�b�g���� �P�ʍs��ւ̃|�C���^)
  *
	The identity matrix.
	{
		{	1,0,0,0,	},
		{	0,1,0,0,	},
		{	0,0,1,0,	},
		{	0,0,0,1,	},
	}
**/
void vfpu_identity_matrix(ScePspFMatrix4 *m);

/**
	�����s��̕��s�ړ�
  * Generate a translation matrix. (translation �}�g���b�N�X�̐���)
  *
  * @param m - pointer to matrix. (translation �}�g���b�N�X�ւ̃|�C���^)
  * @param x - translation on x axis. (x��)
  * @param y - translation on y axis. (y��)
  * @param z - translation on z axis. (z��)
  *
**/
void vfpu_translate_matrix(ScePspFMatrix4 *m, float trans_x_axis, float trans_y_axis, float trans_z_axis);
void vfpu_translate_matrix_t256(ScePspFMatrix4 *m, int trans_x256_axis, int trans_y256_axis, int trans_z256_axis);

/**
	�����s��̐���
  * Generate a perspective projection matrix. (perspective projection �}�g���b�N�X �ˉe�s�� �����s��̐���)
  *
  * @param m	  - pointer to matrix. (perspective projection �}�g���b�N�X�ւ̃|�C���^)
  * @param fovy   - vertical field of view. ()
  * @param aspect - aspect ratio of viewport. ()
  * @param near   - near clipping plane. ()
  * @param far	  - far clipping plane. ()
  *
**/
void vfpu_perspective_matrix(ScePspFMatrix4 *m, float fovy, float aspect, float near, float far);

/**
	�����s��̐���
  * Generate an orthogonal	projection matrix. ()
  *
  * @param m	  - pointer to matrix. ()
  * @param left   - coordinate for left edge of viewport. ()
  * @param right  - coordinate for right edge of viewport. ()
  * @param bottom - coordinate for bottom edge of viewport. ()
  * @param top	  - coordinate for top edge of viewport. ()
  * @param near   - near clipping plane. ()
  * @param far	  - far clipping plane. ()
  *
**/
void vfpu_ortho_matrix(ScePspFMatrix4 *m, float left, float right, float bottom, float top, float near, float far);

/*---------------------------------------------------------

---------------------------------------------------------*/

/**
  * Normalize a 3d vector, returning a unit vector of length = 1. ()
  *
  * @param v - pointer to vector to normalize. ()
  *
**/
void vfpu_normalize_vector(ScePspFVector4 *v);

/**
  * Zero a 3d vector. ()
  *
  * @param v - pointer to vector. ()
  *
**/
void vfpu_zero_vector(ScePspFVector4 *v);

/**
  * Scale a 3d vector. ()
  *
  * @param vout  - pointer to result vector. ()
  * @param vin	 - pointer to vector to scale. ()
  * @param scale - float value to scale vector by. ()
  *
**/
void vfpu_scale_vector(ScePspFVector4 *vout, ScePspFVector4 *vin, float scale);

/**
  * Add 2 3d vectors. ()
  *
  * @param vout - pointer to result vector. ()
  * @param va	- pointer to first vector to add. ()
  * @param vb	- pointer to second vector to add. ()
**/
void vfpu_add_vector(ScePspFVector4 *vout, ScePspFVector4 *va, ScePspFVector4 *vb);


/**
  * Generate rotation matrix for environment map. ()
  *
  * @param envmat - pointer to array of 2 vectors to store envmap matrix. ()
  * @param r	  - angle to rotate in radians. ()
**/
void vfpu_envmap_matrix(ScePspFVector4 *envmat, float r);

/**
  * Transform 3d vector by 4x4 matrix. ()
  *
  * @param m	- pointer to transformation matrix. ()
  * @param vin	- pointer to vector to transform. ()
  * @param vout - pointer to result vector. ()
**/
void vfpu_transform_vector(ScePspFMatrix4 *m, ScePspFVector4 *vin, ScePspFVector4 *vout);

/**
  * Convert input sphere coordinates to cartesian coordinates. ()
  *
  * @param az  - azimuth angle (0 to PI*2). ()
  * @param ze  - zenith angle (0 to PI). ()
  * @param rad - sphere radius. ()
  * @param x   - pointer to float for x coordinate. (x��)
  * @param y   - pointer to float for y coordinate. (y��)
  * @param z   - pointer to float for z coordinate. (z��)
  *
**/
void vfpu_sphere_to_cartesian(float az, float ze, float rad, float *x, float *y, float *z);

/*---------------------------------------------------------
	�N�H�[�^�j�I���֘A�̃��C�u����
	-------------------------------------------------------
	�R�c��Ԃŉ�]��\���ꍇ�A�傫�������āA
	�u���̂̌X�����p�x(�I�C���[�p)�ŕ\�����@�v(���̂��X�����Ă��̊p�x���L�^����Ƃ����l����)��
	�u�N�H�[�^�j�I�����g���ĕ\�����@�v(���̂̌X������Ԃ𐔒l(��]�s��)�ň�ӂɕ\���Ƃ����l����)������܂��B
	-------------------------------------------------------
	�I�C���[�p==�u���̂̌X�����p�x�vx,y,z�R���B(+�v�Z����)
	-------------------------------------------------------
	�I�C���[�p���g�����@�ł́A�u�v�Z�����v���d�v�Łu�v�Z�����v���Ⴆ�΁u�v�Z���ʁv���Ⴂ�܂��B
	�Ⴆ�΁Ax����30�x��]������y����60�x��]���������̂ƁA
	y����60�x��]������x����30�x��]���������̂́A
	�������S�R�Ⴂ�܂��B
	-------------------------------------------------------
	�N�H�[�^�j�I��(�l����)�Ƃ́A(x,y,z,w)�̑g�̎��ł��B
	2�����x�N�g����(x,y)�̑g�̎��A
	3�����x�N�g����(x,y,z)�̑g�̎��Ȃ̂ŁA
	4�����x�N�g���A(x,y,z,w)�̑g�B�ƌ����Ă������x���Ȃ����ł����A
	w�͎��Ԃł͂Ȃ��̂ŁA�����͌Ăт܂���B
	-------------------------------------------------------
	�N�H�[�^�j�I��(�l����, �����񂷂�, quaternion)
	�Q�l�F http://www015.upp.so-net.ne.jp/notgeld/quaternion.html
--------------------------------------------------------- */

/**
  * Generate an identity quaternion. ()
  *
  * @param q - pointer to quaternion. ()
  *
  * this will set the quaternion's components to 0,0,0,1. ()
  *
**/
void vfpu_quaternion_identity(ScePspQuatMatrix *q);

/**
  * Copy a quaternion. ()
  *
  * @param dst - pointer to quaternion to copy to. ()
  * @param src - pointer to quaternion to copy from. ()
  *
**/
void vfpu_quaternion_copy(ScePspQuatMatrix *dst, ScePspQuatMatrix *src);

/**
  * Normalize a quaternion. ()
  *
  * @param res - pointer to quaternion to normalize. ()
  *
**/
void vfpu_quaternion_normalize(ScePspQuatMatrix *res);

/**
  * Multiply 2 quaternions. ()
  *
  * @param qout - pointer to result quaternion. ()
  * @param a	- pointer to first quaternion to multiply. ()
  * @param b	- pointer to second quaternion to multiply. ()
  *
**/
void vfpu_quaternion_multiply(ScePspQuatMatrix *qout, ScePspQuatMatrix *a, ScePspQuatMatrix *b);

/**
  * Calculate exponent of a quaternion. (�N�H�[�^�j�I�����w���X�P�[���֕ϊ�����B)
  *
  * @param qout - pointer to output quaternion. ()
  * @param qin	- pointer to input quaternion. ()
  *
**/
void vfpu_quaternion_exp(ScePspQuatMatrix *qout, ScePspQuatMatrix *qin);

/**
  * Calculate logarithm of a quaternion. (�N�H�[�^�j�I����ΐ��X�P�[���֕ϊ�����B)
  *
  * @param qout - pointer to output quaternion. (�o�̓N�H�[�^�j�I���ւ̃|�C���^)
  * @param qin	- pointer to input quaternion. (���̓N�H�[�^�j�I���ւ̃|�C���^)
  *
**/
void vfpu_quaternion_ln(ScePspQuatMatrix *qout, ScePspQuatMatrix *qin);

/**
  * Return a sample from a linear interpolation of 2 quaternions. (�Q�̃N�H�[�^�j�I���𒼐���Ԃō�������)
  *
  * @param qout - pointer to output quaternion. (�������ʂւ̃|�C���^)
  * @param a	- pointer to starting quaternion. (�P�ڂ̃N�H�[�^�j�I��)
  * @param b	- pointer to ending quaternion. (�Q�ڂ̃N�H�[�^�j�I��)
  * @param t	- time value to sample, from 0.00f to 1.00f . (��������)
  *
**/
void vfpu_quaternion_sample_linear(ScePspQuatMatrix *qout, ScePspQuatMatrix *a, ScePspQuatMatrix *b, float t);
/*
 * @param multiplex_time256 - time value to sample, from (int)0 to (int)255 . (��������)
*/
void vfpu_quaternion_sample_linear65536(ScePspQuatMatrix *qout, ScePspQuatMatrix *a, ScePspQuatMatrix *b, u32 multiplex_time256);

/**
  * Return a sample from a hermite spline interpolation. ()
  *
  * @param qout - pointer to output quaternion. ()
  * @param a	- pointer to start quaternion. ()
  * @param b	- pointer to end quaternion. ()
  * @param at	- pointer to tangent point for quaternion a. ()
  * @param bt	- pointer to tangent point for quaternion b. ()
  * @param t	- time value to sample, from 0 to 1. ()
  *
**/
void vfpu_quaternion_sample_hermite(ScePspQuatMatrix *qout, ScePspQuatMatrix *a, ScePspQuatMatrix *b, ScePspQuatMatrix *at, ScePspQuatMatrix *bt, float t);

/**
  * Return a tangent point for hermite spline interpolation. ()
  *
  * @param qout - pointer to output quaternion. ()
  * @param p1	- pointer to p-1 on spline curve for tangent. ()
  * @param p2	- pointer to p+1 on spline curve for tangent. ()
  * @param bias - value to scale difference between endpoints. ()
  * 			  for example, 0.5 results in a catmull-rom spline tangent. ()
  *
**/

void vfpu_quaternion_hermite_tangent(ScePspQuatMatrix *qout, ScePspQuatMatrix *p1, ScePspQuatMatrix *p2, float bias);

/**
  * Convert quaternion to rotation matrix. (�N�H�[�^�j�I������]�s��ɕϊ�����)
  *
  * @param q - pointer to input quaternion. ()
  * @param m - pointer to output matrix. ()
  *
**/
void vfpu_quaternion_to_matrix(ScePspQuatMatrix *q, ScePspFMatrix4 *m);

/*---------------------------------------------------------
	�I�C���[�p �� �N�H�[�^�j�I�� �ϊ��֐�(�I�C���[�p�A�x���⏕�֐�)
	The euler angles, supported functions. Make quaternion from euler angles.
	-------------------------------------------------------
	psp �̃n�[�h�E�F�A�[�� �N�H�[�^�j�I�� �݂̂������A
	�upsp �ł̓I�C���[�p�͎g�p���܂���v���A
	�upsp �ɂ̓I�C���[�p�������n�[�h�E�F�A�[�@�\�͂���܂���v
	-------------------------------------------------------
	�����ŃI�C���[�p�������ꍇ�ɂ́A�����̊֐��ŕϊ����܂��B
	�I�C���[�p�̕ϊ������� x����y����z�� �ł��B
	��]������ ���v���(Clock Wise) �ł��B
---------------------------------------------------------*/

#if 1
/* [] */
/**
  * Make a quaternion from euler angles,				(�I�C���[�p����A�N�H�[�^�j�I���𐶐����邪�A)
  * The angle divides a round on 360(0-359) degrees.	(�I�C���[�p�̊p�x�P�ʌn�͈����360���������p�x�P�ʌn�B)
  *
  * @param qout - pointer to output quaternion. (���ʂ��o�͂���N�H�[�^�j�I���ւ̃|�C���^)
  * @param rx	- rotation on x axis, in 360 degrees. (x���̉�]�p�x�A����� 0.0f �` 360.0f �͈̔͂� float �l�ŗ^����)
  * @param ry	- rotation on y axis, in 360 degrees. (y���̉�]�p�x�A����� 0.0f �` 360.0f �͈̔͂� float �l�ŗ^����)
  * @param rz	- rotation on z axis, in 360 degrees. (z���̉�]�p�x�A����� 0.0f �` 360.0f �͈̔͂� float �l�ŗ^����)
**/
void vfpu_quaternion_from_euler(		ScePspQuatMatrix *qout, float rx360, float ry360, float rz360);

/**
  * Make a quaternion from euler angles,				(�I�C���[�p����A�N�H�[�^�j�I���𐶐����邪�A)
  * The angle divides a round on 512(0-511) degrees.	(�I�C���[�p�̊p�x�P�ʌn�͈����512���������p�x�P�ʌn�B)
  *
  * @param qout - pointer to output quaternion. (���ʂ��o�͂���N�H�[�^�j�I���ւ̃|�C���^)
  * @param rx	- rotation on x axis, in 512 degrees. (x���̉�]�p�x�A����� 0.0f �` 512.0f �͈̔͂� float �l�ŗ^����)
  * @param ry	- rotation on y axis, in 512 degrees. (y���̉�]�p�x�A����� 0.0f �` 512.0f �͈̔͂� float �l�ŗ^����)
  * @param rz	- rotation on z axis, in 512 degrees. (z���̉�]�p�x�A����� 0.0f �` 512.0f �͈̔͂� float �l�ŗ^����)
**/
void vfpu_quaternion_from_euler512( 	ScePspQuatMatrix *qout, float rx512, float ry512, float rz512);
void vfpu_quaternion_from_euler512_int(ScePspQuatMatrix *res, int rot_x512, int rot_y512, int rot_z512);
#endif

#if 000

/*---------------------------------------------------------
	�f�o�b�O�p�֐�
---------------------------------------------------------*/

/** for only debugging.
  * Print a formatted block of 16 vfpu registers.
  * VFPU�����s�����\������B��̍s��� 4x4 �s��Ȃ̂�
  * �������16�̓����l������܂��B
  *
  * @param blockid - register block id (0 to 7).
  * �u���b�NID : VFPU�����s��(4x4�s��)��8����̂� 0 �` 7 �Ŕԍ����w�肷��B
  *
**/
void printVFPURegisters(int blockid);
#endif

#if defined(__cplusplus)
}
#endif

#endif
