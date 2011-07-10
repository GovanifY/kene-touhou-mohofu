
# ------------------------------------------------------------------------------
#   東方模倣風 ～ Toho Imitation Style.  Included Makefile for custom library 
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
# psp専用アセンブラ関連
# ------------------------------------------------------------------------------

OBJDIRS += $(OBJ)/alegrex
OBJDIRS += $(OBJ)/alegrex

# ------------------------------------------------------------------------------

# FPU例外無効
ifneq ($(USE_FPU_HACK),1)
	#
else
	OBJS += $(OBJ)/alegrex/disable_fpu_exeptions.o
	CFLAGS += -DHACK_FPU=1
endif

# ------------------------------------------------------------------------------

# string 関連
OBJS += $(OBJ)/alegrex/tiny_strcmp.o
OBJS += $(OBJ)/alegrex/memset32.o
OBJS += $(OBJ)/alegrex/memset32_uncached.o
OBJS += $(OBJ)/alegrex/memcpy32.o

