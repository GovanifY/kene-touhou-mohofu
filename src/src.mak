
#------------------------------------------------------------------------------
#  東方模倣風 ～ Toho Imitation Style.  Included Makefile for custom library 
#------------------------------------------------------------------------------

# psp専用アセンブラ関連
include $(SRC)/alegrex/alegrex.mak

# 模倣風ゲーム本体
include $(SRC)/game_core/game_core.mak

# 模倣風専用ライブラリ
#------------------------------------------------------------------------------
# OGG 関連
#------------------------------------------------------------------------------

ifneq ($(LINK_OGG),1)
	##not include
else
	### libtremor
#	CFLAGS += -D_LOW_ACCURACY_ 付けると 32bit 演算(たぶん)、付けないと64bit 演算(たぶん)
	CFLAGS += -D_LOW_ACCURACY_
	# 64kHzを再生しない。(SDL max 44.1kHz)
	CFLAGS += -DLIMIT_TO_64kHz

	INCDIR += $(SRC)/$(LIBTREMOR_DIR)
	##
ifneq ($(LINK_LIBTREMOR_LOW_MEM),1)
else
	##only LINK_LIBTREMOR_LOW_MEM
	TREMOR_OBJS += $(OBJ)/libtremor_lowmem/dsp.o
	TREMOR_OBJS += $(OBJ)/libtremor_lowmem/floor_lookup.o
endif
	##
	TREMOR_OBJS += $(OBJ)/$(LIBTREMOR_DIR)/mdct.o
	TREMOR_OBJS += $(OBJ)/$(LIBTREMOR_DIR)/mapping0.o
	#
	TREMOR_OBJS += $(OBJ)/$(LIBTREMOR_DIR)/res012.o
	TREMOR_OBJS += $(OBJ)/$(LIBTREMOR_DIR)/floor0.o
	TREMOR_OBJS += $(OBJ)/$(LIBTREMOR_DIR)/floor1.o
	#
ifneq ($(LINK_LIBTREMOR_LOW_MEM),1)
	##only large
	TREMOR_OBJS += $(OBJ)/$(LIBTREMOR_DIR)/sharedbook.o
endif
	TREMOR_OBJS += $(OBJ)/$(LIBTREMOR_DIR)/codebook.o
ifneq ($(LINK_LIBTREMOR_LOW_MEM),1)
	##only large
	TREMOR_OBJS += $(OBJ)/libtremor_large/window.o
	TREMOR_OBJS += $(OBJ)/libtremor_large/registry.o
endif
	TREMOR_OBJS += $(OBJ)/$(LIBTREMOR_DIR)/bitwise.o
ifneq ($(LINK_LIBTREMOR_LOW_MEM),1)
	##only large
	TREMOR_OBJS += $(OBJ)/libtremor_large/block.o
endif

	TREMOR_OBJS += $(OBJ)/$(LIBTREMOR_DIR)/info.o
ifneq ($(LINK_LIBTREMOR_LOW_MEM),1)
	##only large
	TREMOR_OBJS += $(OBJ)/libtremor_large/synthesis.o
endif

	TREMOR_OBJS += $(OBJ)/$(LIBTREMOR_DIR)/framing.o
	TREMOR_OBJS += $(OBJ)/$(LIBTREMOR_DIR)/vorbisfile.o

	OBJS += $(OBJ)/$(LIBTREMOR_DIR)/lib_mohou_vorbisidec.a
##
	OBJDIRS += $(OBJ)/$(LIBTREMOR_DIR)



#libvorbisidec_la_SOURCES =
# mdct.c
# block.c
# window.c 
# synthesis.c
# info.c 
# floor1.c
# floor0.c
# vorbisfile.c 
# res012.c
# mapping0.c
# registry.c
# codebook.c 
# sharedbook.c
# framing.c
# bitwise.c 
# codebook.h
# misc.h
# mdct_lookup.h
# os.h
# mdct.h
# ivorbisfile.h
# lsp_lookup.h
# registry.h
# window.h
# window_lookup.h
# codec_internal.h
# backends.h
# ogg.h 
# asm_arm.h
# ivorbiscodec.h


	#------------------------------------------------------------------------------
	### libogg
	INCDIR += $(SRC)/libogg

	#------------------------------------------------------------------------------
	### libvorbis
	INCDIR += $(SRC)/libvorbis
endif

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------

#CFLAGS = -g -O2 -I/usr/local/pspdev/psp/include/SDL 
#-Dmain=SDL_main 	# /* 非対応 */
#-DUS E_RWOPS  		# /* 非対応 */
#-DCMD_MUSIC 
#-DWAV_MUSIC 
#-DMOD_MUSIC 
#-I$(top_srcdir)/mikmod 
#-DMID_MUSIC 
#-DUSE_TIMIDITY_MIDI 
#-I$(top_srcdir)/timidity 
#-DOGG_MUSIC -G0 
#-I"/usr/local/pspdev/psp/sdk/include"


#------------------------------------------------------------------------------
# PSPL audio mixer OGG / MP3 / SMPEG 関連
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
ifneq ($(LINK_OGG),1)
	##not include(この行は使わない場合)
else
	CFLAGS += -DOGG_MUSIC
	CFLAGS += -DUSE_TREMOR
	INCDIR += $(SRC)/libtremor
endif
#------------------------------------------------------------------------------
ifneq ($(LINK_MAD_MP3),1)
	##not include(この行は使わない場合)
else
	### libmad
	CFLAGS += -DMP3_MAD_MUSIC
	# libmad は依存が無いから、何処でリンクしても大丈夫。(たぶん)
	LIBS += -lmad
endif
#------------------------------------------------------------------------------
ifneq ($(LINK_SMPEG_MP3),1)
	##not include(この行は使わない場合)
else
	### smpeg
	CFLAGS += -DMP3_MUSIC
	INCDIR += $(SRC)/sdl_mpeg
	OBJS += $(OBJ)/sdl_mpeg/audio/mpegtable.o
	OBJS += $(OBJ)/sdl_mpeg/audio/mpegtoraw.o
	OBJS += $(OBJ)/sdl_mpeg/audio/hufftable.o

	OBJS += $(OBJ)/sdl_mpeg/audio/bitwindow.o
	OBJS += $(OBJ)/sdl_mpeg/audio/filter_2.o
	OBJS += $(OBJ)/sdl_mpeg/audio/filter.o
	OBJS += $(OBJ)/sdl_mpeg/audio/mpeglayer3.o
	OBJS += $(OBJ)/sdl_mpeg/audio/mpeglayer2.o
	OBJS += $(OBJ)/sdl_mpeg/audio/mpeglayer1.o
	OBJS += $(OBJ)/sdl_mpeg/audio/MPEGaudio.o

	OBJS += $(OBJ)/sdl_mpeg/MPEGlist.o
	OBJS += $(OBJ)/sdl_mpeg/MPEGsystem.o
	OBJS += $(OBJ)/sdl_mpeg/MPEGstream.o
	OBJS += $(OBJ)/sdl_mpeg/MPEG.o
	OBJS += $(OBJ)/sdl_mpeg/smpeg.o

	OBJDIRS += $(OBJ)/sdl_mpeg
	OBJDIRS += $(OBJ)/sdl_mpeg/audio
endif

#------------------------------------------------------------------------------
# jpeg 関連
#------------------------------------------------------------------------------

	# jcはエンコード系だから要らない。(jdはデコード)
#	jcmainct.o jctrans.o jcprepct.o jcapimin.o jcmarker.o
#	jcapistd.o jcmaster.o jcparam.o jcinit.o jcdctmgr.o jccoefct.o 
#	jchuff.o jcphuff.o jccolor.o jcsample.o
#	jfdctint.o jfdctfst.o jfdctflt.o
#	jdtrans.o jdatadst.o jdatasrc.o
####
	OBJS += $(OBJ)/jpeg/jerror.o 
	OBJS += $(OBJ)/jpeg/jutils.o
####
#	OBJS += $(OBJ)/jpeg/jidctint.o	# scale / IDCT_SCALING_SUPPORTED
#	OBJS += $(OBJ)/jpeg/jidctred.o	# scale / IDCT_SCALING_SUPPORTED
#	OBJS += $(OBJ)/jpeg/jidctflt.o	# float / DCT_FLOAT_SUPPORTED
	OBJS += $(OBJ)/jpeg/jidctfst.o
	OBJS += $(OBJ)/jpeg/jddctmgr.o
####
	OBJS += $(OBJ)/jpeg/jdcoefct.o
####
	OBJS += $(OBJ)/jpeg/jdhuff.o
	OBJS += $(OBJ)/jpeg/jdphuff.o
####
	OBJS += $(OBJ)/jpeg/jdcolor.o
	OBJS += $(OBJ)/jpeg/jdsample.o
####
	OBJS += $(OBJ)/jpeg/jmemnobs.o
	OBJS += $(OBJ)/jpeg/jmemmgr.o 
####
	OBJS += $(OBJ)/jpeg/jdmerge.o
	OBJS += $(OBJ)/jpeg/jdmainct.o
	OBJS += $(OBJ)/jpeg/jdpostct.o 
	OBJS += $(OBJ)/jpeg/jdmarker.o
	OBJS += $(OBJ)/jpeg/jdinput.o
	OBJS += $(OBJ)/jpeg/jquant1.o
	OBJS += $(OBJ)/jpeg/jquant2.o
	OBJS += $(OBJ)/jpeg/jdmaster.o
#
	OBJS += $(OBJ)/jpeg/jdapistd.o 
	OBJS += $(OBJ)/jpeg/jcomapi.o
	OBJS += $(OBJ)/jpeg/jdapimin.o

	OBJDIRS += $(OBJ)/jpeg


#------------------------------------------------------------------------------
# png 関連
#------------------------------------------------------------------------------

### libpng

	OBJS += $(OBJ)/libpng/pngrtran.o
	OBJS += $(OBJ)/libpng/pngmem.o
	OBJS += $(OBJ)/libpng/pngtrans.o
	OBJS += $(OBJ)/libpng/pngerror.o
	OBJS += $(OBJ)/libpng/pngget.o
	OBJS += $(OBJ)/libpng/pngset.o
	OBJS += $(OBJ)/libpng/pngrio.o
	OBJS += $(OBJ)/libpng/png.o
	OBJS += $(OBJ)/libpng/pngrutil.o
	OBJS += $(OBJ)/libpng/pngread.o

	OBJDIRS += $(OBJ)/libpng


#------------------------------------------------------------------------------
# portable psp vfpu customized library.
#------------------------------------------------------------------------------

# vfpu
include $(SRC)/vfpu/vfpu.mak

#------------------------------------------------------------------------------
# module music.
#------------------------------------------------------------------------------

# mikmod
include $(SRC)/module_music/module_music.mak

#------------------------------------------------------------------------------
# Portable pSP sdL customized library.
#------------------------------------------------------------------------------

# PSPL
include $(SRC)/PSPL/PSPL.mak


#------------------------------------------------------------------------------
# Portable psp Gu Customized library.
#------------------------------------------------------------------------------

# pgc
include $(SRC)/libgu/libgu.mak

# pgc
include $(SRC)/debug/debug.mak
