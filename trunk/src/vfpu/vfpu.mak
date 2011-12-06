
#------------------------------------------------------------------------------
#  �����͕핗 �` Toho Imitation Style.  Included Makefile for custom library 
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# vfpu �֘A
#------------------------------------------------------------------------------

#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_fabsf.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_fminf.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_fmaxf.o

#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_sinf.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_cosf.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_tanf.o

#�p�~		LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_sini.o	#�p�~# old version.

#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_asinf.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_acosf.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_atanf.o

	## vfpu_atan2f(); used only vfpu_quaternion_ln();
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_atan2f.o
#�p�~		LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_atan512i.o	#�p�~# old version.
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_atan1024i.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_atan65536i.o

#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_sinhf.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_coshf.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_tanhf.o

			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_sincos.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_expf.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_logf.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_sqrtf.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_powf.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_fmodf.o

			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_rands.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_randf.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/math_rand_8888.o

#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_ease_in_out.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_ease_in_out65536.o

			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_identity_matrix.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_translate_matrix.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_perspective_matrix.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_ortho_matrix.o

#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_normalize_vector.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_zero_vector.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_scale_vector.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_add_vector.o

#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_envmap_matrix.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_sphere_to_cartesian.o

			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_identity.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_copy.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_multiply.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_normalize.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_exp.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_ln.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_sample_linear.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_sample_linear65536.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_from_euler.o
			LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_to_matrix.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_sample_hermite.o
#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/vfpu_quaternion_hermite_tangent.o

#���g�p 	LIB_PSP_MATH_OBJS += $(OBJ)/vfpu/printMatrixFloat.o

	OBJDIRS += $(OBJ)/vfpu
#	OBJDIRS += $(OBJ)/vfpu/math
	OBJS += $(OBJ)/vfpu/lib_mohou_vfpu.a
	INCDIR += $(SRC)/vfpu

