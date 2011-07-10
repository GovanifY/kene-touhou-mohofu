
#------------------------------------------------------------------------------
#  東方模倣風 ～ Toho Imitation Style.  Included Makefile for custom library 
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# PSPL audio mixer 関連
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
CFLAGS += -DWAV_MUSIC
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
### SDL_mixer
#------------------------------------------------------------------------------
#SDL_mixer内 ogg music サポート
ifneq ($(LINK_OGG),1)
	##not include
else
	#ogg
	OBJS += $(OBJ)/PSPL/audio/mixer/load_ogg.o
	OBJS += $(OBJ)/PSPL/audio/mixer/music_ogg.o
endif
#------------------------------------------------------------------------------
#SDL_mixer内 madlib mpeg3 music サポート
ifneq ($(LINK_MAD_MP3),1)
	##not include(この行は使わない場合)
else
	### libmad
	OBJS += $(OBJ)/PSPL/audio/mixer/music_mad.o
endif
#------------------------------------------------------------------------------
#SDL_mixer内 wave music サポート
#ifneq ($(filter -DWAV_MUSIC,$(CFLAGS)),)
#	# WAV_MUSICなし
#	##not include(この行は使わない場合)
#else
	# WAV_MUSICあり
	OBJS += $(OBJ)/PSPL/audio/mixer/wavestream.o
#endif
#------------------------------------------------------------------------------
#SDL_mixer内 module music サポート
ifneq ($(LINK_MOD),1)
	##not include(この行は使わない場合)
else
	OBJS += $(OBJ)/PSPL/audio/mixer/music_mod.o
endif
#------------------------------------------------------------------------------
#SDL_mixer本体
#	OBJS += $(OBJ)/PSPL/audio/mixer/effects_internal.o
	OBJS += $(OBJ)/PSPL/audio/mixer/music.o
	OBJS += $(OBJ)/PSPL/audio/mixer/mixer.o

OBJDIRS += $(OBJ)/PSPL/audio/mixer




#------------------------------------------------------------------------------
# PSPL image 関連
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
# PSPL 関連
#------------------------------------------------------------------------------

	CFLAGS += -DENABLE_PSP
#	CFLAGS += -DDISABLE_THREADS
#
#/audio/psp/
	OBJS += $(OBJ)/PSPL/audio/PSPL_pspaudio.o
#/audio/
	OBJS += $(OBJ)/PSPL/audio/PSPL_audio.o
	OBJS += $(OBJ)/PSPL/audio/PSPL_audiocvt.o
#	OBJS += $(OBJ)/PSPL/audio/PSPL_audiodev.o廃止
#	OBJS += $(OBJ)/PSPL/audio/PSPL_audiomem.o廃止
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
	OBJS += $(OBJ)/PSPL/thread/PSPL_systhread.o
	OBJS += $(OBJ)/PSPL/thread/PSPL_thread.o
#/timer/
#/video/psp/
	OBJS += $(OBJ)/PSPL/video/PSPL_pspvideo.o
#/video/
	OBJS += $(OBJ)/PSPL/video/PSPL_blit.o
	OBJS += $(OBJ)/PSPL/video/PSPL_blit_0.o
	OBJS += $(OBJ)/PSPL/video/PSPL_blit_1.o
	OBJS += $(OBJ)/PSPL/video/PSPL_blit_A.o
	OBJS += $(OBJ)/PSPL/video/PSPL_blit_N.o
	OBJS += $(OBJ)/PSPL/video/PSPL_bmp.o

	OBJS += $(OBJ)/PSPL/video/PSPL_pixels.o
	OBJS += $(OBJ)/PSPL/video/PSPL_RLEaccel.o
	OBJS += $(OBJ)/PSPL/video/PSPL_surface.o
	OBJS += $(OBJ)/PSPL/video/PSPL_video.o
#
	OBJS += $(OBJ)/PSPL/PSPL.o
#------------ ダミー
#	OBJS += $(OBJ)/PSPL/PSPL_error.o廃止
#	OBJS += $(OBJ)/PSPL/PSPL_timer.o
#	OBJS += $(OBJ)/PSPL/PSPL231dummy.o廃止
#	OBJS += $(OBJ)/PSPL/PSPL_fatal.o廃止
#	OBJS += $(OBJ)/PSPL/PSPL_systimer.o廃止

#------------
	OBJDIRS += $(OBJ)/PSPL
	OBJDIRS += $(OBJ)/PSPL/audio
#	OBJDIRS += $(OBJ)/PSPL/endian
	OBJDIRS += $(OBJ)/PSPL/thread
	OBJDIRS += $(OBJ)/PSPL/video
#	INCDIR += $(SRC)/PSPL/include
	INCDIR += $(SRC)/PSPL/video


