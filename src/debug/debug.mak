
#------------------------------------------------------------------------------
#  東方模倣風 ～ Toho Imitation Style.  Included Makefile for custom library 
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Gu 関連
#------------------------------------------------------------------------------

### libgu
#ifneq ($(filter -lpspgu,$(LIBS)),)
#	# 標準ライブラリを使う
#else
	# カスタムライブラリを使う
	LIB_PSP_DEBUG_OBJS += $(OBJ)/debug/scr_printf.o
	LIB_PSP_DEBUG_OBJS += $(OBJ)/debug/font.o

	OBJS += $(OBJ)/debug/debug.a
	OBJDIRS += $(OBJ)/debug
	INCDIR += $(SRC)/debug
#endif
