
#------------------------------------------------------------------------------
#  �����͕핗 �` Toho Imitation Style.  Included Makefile for custom library 
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Gu �֘A
#------------------------------------------------------------------------------

### libgu
#ifneq ($(filter -lpspgu,$(LIBS)),)
#	# �W�����C�u�������g��
#else
	# �J�X�^�����C�u�������g��
	LIB_PSP_DEBUG_OBJS += $(OBJ)/debug/scr_printf.o
	LIB_PSP_DEBUG_OBJS += $(OBJ)/debug/font.o

	OBJS += $(OBJ)/debug/debug.a
	OBJDIRS += $(OBJ)/debug
	INCDIR += $(SRC)/debug
#endif
