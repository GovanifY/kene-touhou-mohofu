
#------------------------------------------------------------------------------
#  ìåï˚ñÕïÌïó Å` Toho Imitation Style.  Included Makefile for custom library 
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# PSPL audio mixer ä÷òA
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
CFLAGS += -DWAV_MUSIC
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
### SDL_mixer
#------------------------------------------------------------------------------
#SDL_mixerì‡ ogg music ÉTÉ|Å[Ég
ifneq ($(LINK_OGG),1)
	##not include
else
	#ogg
	OBJS += $(OBJ)/PSPL/audio/mixer/load_ogg.o
	OBJS += $(OBJ)/PSPL/audio/mixer/music_ogg.o
endif
#------------------------------------------------------------------------------
#SDL_mixerì‡ madlib mpeg3 music ÉTÉ|Å[Ég
ifneq ($(LINK_MAD_MP3),1)
	##not include(Ç±ÇÃçsÇÕégÇÌÇ»Ç¢èÍçá)
else
	### libmad
	OBJS += $(OBJ)/PSPL/audio/mixer/music_mad.o
endif
#------------------------------------------------------------------------------
#SDL_mixerì‡ wave music ÉTÉ|Å[Ég
#ifneq ($(filter -DWAV_MUSIC,$(CFLAGS)),)
#	# WAV_MUSICÇ»Çµ
#	##not include(Ç±ÇÃçsÇÕégÇÌÇ»Ç¢èÍçá)
#else
	# WAV_MUSICÇ†ÇË
	OBJS += $(OBJ)/PSPL/audio/mixer/wavestream.o
#endif
#------------------------------------------------------------------------------
#SDL_mixerì‡ module music ÉTÉ|Å[Ég
ifneq ($(LINK_MOD),1)
	##not include(Ç±ÇÃçsÇÕégÇÌÇ»Ç¢èÍçá)
else
	OBJS += $(OBJ)/PSPL/audio/mixer/music_mod.o
endif
#------------------------------------------------------------------------------
#SDL_mixerñ{ëÃ
#	OBJS += $(OBJ)/PSPL/audio/mixer/effects_internal.o
	OBJS += $(OBJ)/PSPL/audio/mixer/music.o
	OBJS += $(OBJ)/PSPL/audio/mixer/mixer.o

OBJDIRS += $(OBJ)/PSPL/audio/mixer




#------------------------------------------------------------------------------
# PSPL image ä÷òA
#------------------------------------------------------------------------------

### libSDL_image

#	CFLAGS += -DLOAD_TGA
##	CFLAGS += -DLOAD_BMP
#	CFLAGS += -DLOAD_PNM
#	CFLAGS += -DLOAD_XPM
#	CFLAGS += -DLOAD_XCF	##	[not used psp sdk]
#	CFLAGS += -DLOAD_PCX
#	CFLAGS += -DLOAD_GIF
	CFLAGS += -DLOAD_JPG
#	CFLAGS += -DLOAD_TIF	##	[not used psp sdk]
#	CFLAGS += -DLOAD_LBM
	CFLAGS += -DLOAD_PNG
#	OBJS += $(OBJ)/PSPL/image/IMG_tga.o
##	OBJS += $(OBJ)/PSPL/image/IMG_bmp.o
#	OBJS += $(OBJ)/PSPL/image/IMG_pnm.o
#	OBJS += $(OBJ)/PSPL/image/IMG_xpm.o
#	OBJS += $(OBJ)/PSPL/image/IMG_xcf.o
#	OBJS += $(OBJ)/PSPL/image/IMG_pcx.o
#	OBJS += $(OBJ)/PSPL/image/IMG_gif.o
	OBJS += $(OBJ)/PSPL/image/IMG_jpg.o
#	OBJS += $(OBJ)/PSPL/image/IMG_tif.o
#	OBJS += $(OBJ)/PSPL/image/IMG_lbm.o
	OBJS += $(OBJ)/PSPL/image/IMG_png.o
	OBJS += $(OBJ)/PSPL/image/IMG.o
	OBJDIRS += $(OBJ)/PSPL
	OBJDIRS += $(OBJ)/PSPL/image



#------------------------------------------------------------------------------
# PSPL ä÷òA
#------------------------------------------------------------------------------

	CFLAGS += -DENABLE_PSP
#	CFLAGS += -DDISABLE_THREADS
#
#/audio/psp/
#(r39)	OBJS += $(OBJ)/PSPL/audio/PSPL_pspaudio.o[ãzé˚ PSPL_audio.c]
#/audio/
	OBJS += $(OBJ)/PSPL/audio/PSPL_audio.o
	OBJS += $(OBJ)/PSPL/audio/PSPL_audiocvt.o
#	OBJS += $(OBJ)/PSPL/audio/PSPL_audiodev.oîpé~
#	OBJS += $(OBJ)/PSPL/audio/PSPL_audiomem.oîpé~
	OBJS += $(OBJ)/PSPL/audio/PSPL_mixer.o
#/audio/
	OBJS += $(OBJ)/PSPL/audio/PSPL_wave.o
#/cdrom/
#/cpuinfo/
#/endian/
	OBJS += $(OBJ)/PSPL/PSPL_endian.o
#/events/
#/file/
	OBJS += $(OBJ)/PSPL/PSPL_rwops.o
#/hermes/
#/joystick/
#/main/
#/thread/
	OBJS += $(OBJ)/PSPL/thread/PSPL_syssem.o
	#
	OBJS += $(OBJ)/PSPL/thread/PSPL_sysmutex.o
#(r39)	OBJS += $(OBJ)/PSPL/thread/PSPL_systhread.o[ãzé˚ PSPL_thread.c]
	OBJS += $(OBJ)/PSPL/thread/PSPL_thread.o
#/timer/
#/video/psp/
#(r39)	OBJS += $(OBJ)/PSPL/video/PSPL_pspvideo.o[ãzé˚  .c]
#/video/
	OBJS += $(OBJ)/PSPL/video/PSPL_blit.o
	OBJS += $(OBJ)/PSPL/video/PSPL_blit_0.o
	OBJS += $(OBJ)/PSPL/video/PSPL_blit_1.o
	OBJS += $(OBJ)/PSPL/video/PSPL_blit_A.o
	OBJS += $(OBJ)/PSPL/video/PSPL_blit_N.o
	OBJS += $(OBJ)/PSPL/video/PSPL_bmp.o

	OBJS += $(OBJ)/PSPL/video/PSPL_pixels.o
#	OBJS += $(OBJ)/PSPL/video/PSPL_RLEaccel.o
	OBJS += $(OBJ)/PSPL/video/PSPL_surface.o
	OBJS += $(OBJ)/PSPL/video/PSPL_video.o
#
#	OBJS += $(OBJ)/PSPL/PSPL.o
#------------ É_É~Å[
#	OBJS += $(OBJ)/PSPL/PSPL_error.oîpé~
#	OBJS += $(OBJ)/PSPL/PSPL_timer.o
#	OBJS += $(OBJ)/PSPL/PSPL231dummy.oîpé~
#	OBJS += $(OBJ)/PSPL/PSPL_fatal.oîpé~
#	OBJS += $(OBJ)/PSPL/PSPL_systimer.oîpé~

#------------
	OBJDIRS += $(OBJ)/PSPL
	OBJDIRS += $(OBJ)/PSPL/audio
#	OBJDIRS += $(OBJ)/PSPL/endian
	OBJDIRS += $(OBJ)/PSPL/thread
	OBJDIRS += $(OBJ)/PSPL/video
#	INCDIR += $(SRC)/PSPL/include
	INCDIR += $(SRC)/PSPL/video


