#include "psp_vfpu.h"
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
void vfpu_translate_matrix(
	ScePspFMatrix4 *m,
	float trans_x_axis,
	float trans_y_axis,
	float trans_z_axis
	)
{
	__asm__ volatile (
		"vmidt.q M000\n"
		"mtv        %1, S030\n" 		// S030 = trans_x_axis;
		"mtv        %2, S031\n" 		// S031 = trans_y_axis;
		"mtv        %3, S032\n" 		// S032 = trans_z_axis;
		//"vmidt.q M100\n"
		//"lv.q   C200, %1\n"
		//"vmov.t  C130, C200\n"
		//"vmmul.q M200, M100, M000\n"
		"sv.q C000,  0 + %0\n"
		"sv.q C010, 16 + %0\n"
		"sv.q C020, 32 + %0\n"
		"sv.q C030, 48 + %0\n"
		: "=m"(*m) : "r"(trans_x_axis), "r"(trans_y_axis), "r"(trans_z_axis));
}
