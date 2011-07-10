
# ------------------------------------------------------------------------------
#   �����͕핗 �` Toho Imitation Style.  Included Makefile for custom library 
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
# psp��p�A�Z���u���֘A
# ------------------------------------------------------------------------------

OBJDIRS += $(OBJ)/alegrex
OBJDIRS += $(OBJ)/alegrex

# ------------------------------------------------------------------------------

# FPU��O����
ifneq ($(USE_FPU_HACK),1)
	#
else
	OBJS += $(OBJ)/alegrex/disable_fpu_exeptions.o
	CFLAGS += -DHACK_FPU=1
endif

# ------------------------------------------------------------------------------

# string �֘A
OBJS += $(OBJ)/alegrex/tiny_strcmp.o
OBJS += $(OBJ)/alegrex/memset32.o
OBJS += $(OBJ)/alegrex/memset32_uncached.o
OBJS += $(OBJ)/alegrex/memcpy32.o

