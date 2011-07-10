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
 *	VFPU使うライブラリですが、PSPSDKの原版は mathlib とpow()の引数に互換がありません。
 *	そこで多少改造してmathlib互換に引数を合わせてあります。
 *	標準の mathlib(libm、 -lm) 使った場合に比べて 5 ～ 20 倍、速いそうです。
 *
 */

#ifndef _psp_math_h_
#define _psp_math_h_

#include <psptypes.h>

/** @defgroup VFPUMATH VFPU Math Library (VFPU演算ライブラリ)
 *
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*---------------------------------------------------------
	クォータニオンを定義
---------------------------------------------------------*/

typedef struct quat 	/* クォータニオンを定義 */
{
	float x;	/* x軸要素 */
	float y;	/* y軸要素 */
	float z;	/* z軸要素 */
	float w;	/* w要素 */
} __attribute__((aligned(16))) ScePspQuatMatrix;

typedef struct quat_t256 	/* クォータニオンを定義 */
{
	int x256;	/* x軸要素 */
	int y256;	/* y軸要素 */
	int z256;	/* z軸要素 */
	int w256;	/* w要素 */
} __attribute__((aligned(16))) quaternion_t256;

/** @addtogroup VFPUMATH */
/*@{*/

/*---------------------------------------------------------
	数学関数
	-------------------------------------------------------
	psp ではこれらの数学関数は cpu では遅いので vfpu を使います。
---------------------------------------------------------*/

/**
  * Calculate sine. ()
  *
  * @param x - input in radians. ()
  *
**/
float vfpu_sinf(float x);
//float vfpu_sini(float x);
//[sincosを使い。使用しない]int int256_sin1024(int angle_1024);
//[sincosを使い。使用しない]int int256_sin65536(int angle_65536);
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
	vfpu乱数を扱う関数群
---------------------------------------------------------*/

/**
  * Set to vfpu random generator seed. (VFPU乱数発生器の乱数シード(基数)を設定する。)
  *
  * @param x - seed value. (x: 乱数シード値)
  *
**/
void vfpu_srand(unsigned int x);

/**
  * Return to get random float value. (浮動小数点形式で、乱数値を取得する。)
  *
  * @param min - minimum value to return
  * @param max - maximum value to return
  *
**/
float vfpu_randf(float min, float max);

/**
  * Return to get random color value in 8888 format. (32bit8888色形式で、乱数色を取得する。)
  * This always sets the alpha channel value to 0xff. (アルファは常になし == 0xffBBGGRR )
  *
  * @param min - minimum value for each color channel (0..255). (各BGR色の最小値)
  * @param max - maximum value for each color channel (0..255). (各BGR色の最大値)
  *
**/
unsigned int vfpu_rand_8888(int min, int max);


#if 1

/*---------------------------------------------------------
	アニメーション用補助関数
---------------------------------------------------------*/

/**
  * アニメーション用補助関数：
	ease in/out というのは、
	アニメーションで動きの強弱が
	「最初と最後をゆっくりで、間は速い。」という方式です。
	linear				直線補間
	ease-in 			始めだけゆっくり
	ease-out			最後だけゆっくり
	ease-in-out 		始めと最後だけゆっくりで、間は速い。
	cubic-bezier		ベジェ補間
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
	行列を扱う関数群
---------------------------------------------------------*/

/**
  * Generate an identity matrix. (単位行列の生成)
  *
  * @param m - pointer to matrix to set. (セットする 単位行列へのポインタ)
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
	多分行列の平行移動
  * Generate a translation matrix. (translation マトリックスの生成)
  *
  * @param m - pointer to matrix. (translation マトリックスへのポインタ)
  * @param x - translation on x axis. (x軸)
  * @param y - translation on y axis. (y軸)
  * @param z - translation on z axis. (z軸)
  *
**/
void vfpu_translate_matrix(ScePspFMatrix4 *m, float trans_x_axis, float trans_y_axis, float trans_z_axis);
void vfpu_translate_matrix_t256(ScePspFMatrix4 *m, int trans_x256_axis, int trans_y256_axis, int trans_z256_axis);

/**
	透視行列の生成
  * Generate a perspective projection matrix. (perspective projection マトリックス 射影行列 透視行列の生成)
  *
  * @param m	  - pointer to matrix. (perspective projection マトリックスへのポインタ)
  * @param fovy   - vertical field of view. ()
  * @param aspect - aspect ratio of viewport. ()
  * @param near   - near clipping plane. ()
  * @param far	  - far clipping plane. ()
  *
**/
void vfpu_perspective_matrix(ScePspFMatrix4 *m, float fovy, float aspect, float near, float far);

/**
	直交行列の生成
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
  * @param x   - pointer to float for x coordinate. (x軸)
  * @param y   - pointer to float for y coordinate. (y軸)
  * @param z   - pointer to float for z coordinate. (z軸)
  *
**/
void vfpu_sphere_to_cartesian(float az, float ze, float rad, float *x, float *y, float *z);

/*---------------------------------------------------------
	クォータニオン関連のライブラリ
	-------------------------------------------------------
	３Ｄ空間で回転を表す場合、大きく分けて、
	「物体の傾いた角度(オイラー角)で表す方法」(物体を傾かせてその角度を記録するという考え方)と
	「クォータニオンを使って表す方法」(物体の傾いた状態を数値(回転行列)で一意に表すという考え方)があります。
	-------------------------------------------------------
	オイラー角==「物体の傾いた角度」x,y,z３つ分。(+計算順序)
	-------------------------------------------------------
	オイラー角を使う方法では、「計算順序」が重要で「計算順序」が違えば「計算結果」が違います。
	例えば、x軸に30度回転させてy軸に60度回転させた物体と、
	y軸に60度回転させてx軸に30度回転させた物体は、
	向きが全然違います。
	-------------------------------------------------------
	クォータニオン(四元数)とは、(x,y,z,w)の組の事です。
	2次元ベクトルは(x,y)の組の事、
	3次元ベクトルは(x,y,z)の組の事なので、
	4次元ベクトル、(x,y,z,w)の組。と言っても差し支えない物ですが、
	wは時間ではないので、そうは呼びません。
	-------------------------------------------------------
	クォータニオン(四元数, しげんすう, quaternion)
	参考： http://www015.upp.so-net.ne.jp/notgeld/quaternion.html
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
  * Calculate exponent of a quaternion. (クォータニオンを指数スケールへ変換する。)
  *
  * @param qout - pointer to output quaternion. ()
  * @param qin	- pointer to input quaternion. ()
  *
**/
void vfpu_quaternion_exp(ScePspQuatMatrix *qout, ScePspQuatMatrix *qin);

/**
  * Calculate logarithm of a quaternion. (クォータニオンを対数スケールへ変換する。)
  *
  * @param qout - pointer to output quaternion. (出力クォータニオンへのポインタ)
  * @param qin	- pointer to input quaternion. (入力クォータニオンへのポインタ)
  *
**/
void vfpu_quaternion_ln(ScePspQuatMatrix *qout, ScePspQuatMatrix *qin);

/**
  * Return a sample from a linear interpolation of 2 quaternions. (２つのクォータニオンを直線補間で合成する)
  *
  * @param qout - pointer to output quaternion. (合成結果へのポインタ)
  * @param a	- pointer to starting quaternion. (１つ目のクォータニオン)
  * @param b	- pointer to ending quaternion. (２つ目のクォータニオン)
  * @param t	- time value to sample, from 0.00f to 1.00f . (合成割合)
  *
**/
void vfpu_quaternion_sample_linear(ScePspQuatMatrix *qout, ScePspQuatMatrix *a, ScePspQuatMatrix *b, float t);
/*
 * @param multiplex_time256 - time value to sample, from (int)0 to (int)255 . (合成割合)
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
  * Convert quaternion to rotation matrix. (クォータニオンを回転行列に変換する)
  *
  * @param q - pointer to input quaternion. ()
  * @param m - pointer to output matrix. ()
  *
**/
void vfpu_quaternion_to_matrix(ScePspQuatMatrix *q, ScePspFMatrix4 *m);

/*---------------------------------------------------------
	オイラー角 → クォータニオン 変換関数(オイラー角、支援補助関数)
	The euler angles, supported functions. Make quaternion from euler angles.
	-------------------------------------------------------
	psp のハードウェアーは クォータニオン のみを扱い、
	「psp ではオイラー角は使用しません」し、
	「psp にはオイラー角を扱うハードウェアー機能はありません」
	-------------------------------------------------------
	そこでオイラー角を扱う場合には、これらの関数で変換します。
	オイラー角の変換順序は x軸→y軸→z軸 です。
	回転方向は 時計回り(Clock Wise) です。
---------------------------------------------------------*/

#if 1
/* [] */
/**
  * Make a quaternion from euler angles,				(オイラー角から、クォータニオンを生成するが、)
  * The angle divides a round on 360(0-359) degrees.	(オイラー角の角度単位系は一周を360分割した角度単位系。)
  *
  * @param qout - pointer to output quaternion. (結果を出力するクォータニオンへのポインタ)
  * @param rx	- rotation on x axis, in 360 degrees. (x軸の回転角度、一周が 0.0f ～ 360.0f の範囲の float 値で与える)
  * @param ry	- rotation on y axis, in 360 degrees. (y軸の回転角度、一周が 0.0f ～ 360.0f の範囲の float 値で与える)
  * @param rz	- rotation on z axis, in 360 degrees. (z軸の回転角度、一周が 0.0f ～ 360.0f の範囲の float 値で与える)
**/
void vfpu_quaternion_from_euler(		ScePspQuatMatrix *qout, float rx360, float ry360, float rz360);

/**
  * Make a quaternion from euler angles,				(オイラー角から、クォータニオンを生成するが、)
  * The angle divides a round on 512(0-511) degrees.	(オイラー角の角度単位系は一周を512分割した角度単位系。)
  *
  * @param qout - pointer to output quaternion. (結果を出力するクォータニオンへのポインタ)
  * @param rx	- rotation on x axis, in 512 degrees. (x軸の回転角度、一周が 0.0f ～ 512.0f の範囲の float 値で与える)
  * @param ry	- rotation on y axis, in 512 degrees. (y軸の回転角度、一周が 0.0f ～ 512.0f の範囲の float 値で与える)
  * @param rz	- rotation on z axis, in 512 degrees. (z軸の回転角度、一周が 0.0f ～ 512.0f の範囲の float 値で与える)
**/
void vfpu_quaternion_from_euler512( 	ScePspQuatMatrix *qout, float rx512, float ry512, float rz512);
void vfpu_quaternion_from_euler512_int(ScePspQuatMatrix *res, int rot_x512, int rot_y512, int rot_z512);
#endif

#if 000

/*---------------------------------------------------------
	デバッグ用関数
---------------------------------------------------------*/

/** for only debugging.
  * Print a formatted block of 16 vfpu registers.
  * VFPU内部行列を一つ表示する。一つの行列は 4x4 行列なので
  * 一つあたり16個の内部値があります。
  *
  * @param blockid - register block id (0 to 7).
  * ブロックID : VFPU内部行列(4x4行列)は8つあるので 0 ～ 7 で番号を指定する。
  *
**/
void printVFPURegisters(int blockid);
#endif

#if defined(__cplusplus)
}
#endif

#endif
