TARGET = kene
PSPSDK=$(shell psp-config --pspsdk-path)
PSPBIN = $(PSPSDK)/../bin
SDL_CONFIG = $(PSPBIN)/sdl-config

PSP_EBOOT_ICON = ICON0.PNG
PSP_EBOOT_PIC1 = PIC1.PNG
PSP_EBOOT_SND0 = SND0.AT3
PSP_LARGE_MEMORY = 1


OBJS =	main.o \
	font.o \
	sprite.o \
	bonus.o \
	enemy.o \
	enemy_fairy.o \
	enemy_greatfairy.o \
	enemy_greeter.o \
	enemy_curver.o \
	enemy_crusher.o \
	enemy_cube.o \
	enemy_drager.o \
	enemy_eyefo.o \
	enemy_xev.o \
	enemy_mine.o \
	enemy_rwingx.o \
	enemy_cir.o \
	enemy_zatak.o \
	enemy_badguy.o \
	enemy_grounder.o \
	enemy_magicformatiion.o \
	enemy_bgpanel.o \
	enemy_bgpanel2.o \
	enemy_proball.o \
	enemy_plasmaball.o \
	enemy_ming.o \
	enemy_boss01.o \
	enemy_boss02.o \
	enemy_boss03.o \
	enemy_boss04.o \
	enemy_gametext.o \
	enemy_splash.o \
	support.o \
	intro.o \
	thegame.o \
	loadlv.o \
	score.o \
	player.o \
	menu.o \
	startintro.o \
	gameover.o \
	hiscore.o \
	fps.o \
	soundmanager.o \
	bg.o
	

INCDIR = 
CFLAGS = -O2  -G0 -Wall -g $(shell $(SDL_CONFIG) --cflags) 
#CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -fsingle-precision-constant -mno-check-zero-division -ffast-math
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

#BUILD_PRX = 1
PSP_FW_VERSION = 371

LIBDIR =
LDFLAGS =
LIBS = -lstdc++  -lSDL_mixer  -lvorbisidec -lSDL_image -lpng -lz -ljpeg \
        -lSDL_gfx -lm $(shell $(SDL_CONFIG) --libs)  -lpsppower
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = kene

include $(PSPSDK)/lib/build.mak
