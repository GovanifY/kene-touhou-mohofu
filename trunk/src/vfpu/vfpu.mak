
#------------------------------------------------------------------------------
#  東方模倣風 ～ Toho Imitation Style.  Included Makefile for custom library 
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# vfpu 関連
#------------------------------------------------------------------------------

#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_fabsf.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_fminf.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_fmaxf.o

#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_sinf.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_cosf.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_tanf.o

#廃止		LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_sini.o	#廃止# old version.

#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_asinf.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_acosf.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_atanf.o

	## vfpu_atan2f(); used only vfpu_quaternion_ln();
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_atan2f.o
#廃止		LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_atan512i.o	#廃止# old version.
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_atan1024i.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_atan65536i.o

#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_sinhf.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_coshf.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_tanhf.o

			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_sincos.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_expf.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_logf.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_sqrtf.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_powf.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_fmodf.o

			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_rands.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_randf.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_rand_8888.o

#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_ease_in_out.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_ease_in_out65536.o

			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_identity_matrix.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_translate_matrix.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_perspective_matrix.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_ortho_matrix.o

#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_normalize_vector.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_zero_vector.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_scale_vector.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_add_vector.o

#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_envmap_matrix.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_sphere_to_cartesian.o

			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_identity.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_copy.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_multiply.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_normalize.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_exp.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_ln.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_sample_linear.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_sample_linear65536.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_from_euler.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_to_matrix.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_sample_hermite.o
#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_hermite_tangent.o

#未使用 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/printMatrixFloat.o

	OBJDIRS += $(OBJ)/vfpu
#	OBJDIRS += $(OBJ)/vfpu/math
	OBJS += $(OBJ)/vfpu/lib_mohou_vfpu.a
	INCDIR += $(SRC)/vfpu

