
TARGET = kene

PSP_EBOOT_TITLE = kene_r12_test01
PSP_EBOOT_ICON = ICON0.PNG
PSP_EBOOT_PIC1 = PIC1.PNG
PSP_EBOOT_SND0 = SND0.AT3

EXTRA_TARGETS = mk_dir EBOOT.PBP

PSP_LARGE_MEMORY = 1

#

SRC = src
OBJ = obj


PSPSDK=$(shell psp-config --pspsdk-path)
PSPBIN = $(PSPSDK)/../bin
SDL_CONFIG = $(PSPBIN)/sdl-config

#------------------------------------------------------------------------------
# Library
#------------------------------------------------------------------------------

LIBDIR =
LDFLAGS =
#LIBS_org = -lstdc++  -lSDL_mixer  -lvorbisidec -lSDL_image -lpng -lz -ljpeg
#LIBS_org +=  -lSDL_gfx -lm
#LIBS_org += $(shell $(SDL_CONFIG) --libs) -lpsppower

	# GLégÇ§èÍçá
	LIBS += -lSDL -lGL -lpspvfpu 
	# GLégÇÌÇ»Ç¢èÍçá
#	LIBS += -lSDL_noGL

LIBS += -lSDL_mixer
LIBS += -lvorbisidec
LIBS += -lSDL_image
LIBS += -lpng
LIBS += -lz
LIBS += -ljpeg
LIBS += -lm $(shell $(SDL_CONFIG) --libs)
LIBS += -lpsppower


#	particle.o

OBJDIRS += $(OBJ)
OBJDIRS += $(OBJ)/core
OBJDIRS += $(OBJ)/core/enemy

# ------------------


OBJS += $(OBJ)/core/main.o
OBJS += $(OBJ)/core/font.o
OBJS += $(OBJ)/core/sprite.o
OBJS += $(OBJ)/core/bonus.o

OBJS += $(OBJ)/core/enemy.o

OBJS += $(OBJ)/core/enemy/enemy_fairy.o
OBJS += $(OBJ)/core/enemy/enemy_greatfairy.o
OBJS += $(OBJ)/core/enemy/enemy_greeter.o
OBJS += $(OBJ)/core/enemy/enemy_curver.o
OBJS += $(OBJ)/core/enemy/enemy_crusher.o
OBJS += $(OBJ)/core/enemy/enemy_cube.o
OBJS += $(OBJ)/core/enemy/enemy_drager.o
OBJS += $(OBJ)/core/enemy/enemy_eyefo.o
OBJS += $(OBJ)/core/enemy/enemy_xev.o
OBJS += $(OBJ)/core/enemy/enemy_mine.o
OBJS += $(OBJ)/core/enemy/enemy_rwingx.o
OBJS += $(OBJ)/core/enemy/enemy_cir.o
OBJS += $(OBJ)/core/enemy/enemy_zatak.o
OBJS += $(OBJ)/core/enemy/enemy_badguy.o
OBJS += $(OBJ)/core/enemy/enemy_grounder.o
OBJS += $(OBJ)/core/enemy/enemy_magicformatiion.o
OBJS += $(OBJ)/core/enemy/enemy_proball.o
OBJS += $(OBJ)/core/enemy/enemy_plasmaball.o
OBJS += $(OBJ)/core/enemy/enemy_ming.o
OBJS += $(OBJ)/core/enemy/enemy_splash.o

OBJS += $(OBJ)/core/enemy/enemy_boss01.o
OBJS += $(OBJ)/core/enemy/enemy_boss02.o
OBJS += $(OBJ)/core/enemy/enemy_boss03.o
OBJS += $(OBJ)/core/enemy/enemy_boss04.o

OBJS += $(OBJ)/core/enemy/enemy_bgpanel.o
OBJS += $(OBJ)/core/enemy/enemy_bgpanel2.o
OBJS += $(OBJ)/core/enemy/enemy_gametext.o

OBJS += $(OBJ)/core/support.o
OBJS += $(OBJ)/core/intro.o
OBJS += $(OBJ)/core/thegame.o
OBJS += $(OBJ)/core/loadlv.o
OBJS += $(OBJ)/core/score.o
OBJS += $(OBJ)/core/player.o
OBJS += $(OBJ)/core/menu.o
OBJS += $(OBJ)/core/startintro.o
OBJS += $(OBJ)/core/gameover.o
OBJS += $(OBJ)/core/hiscore.o
OBJS += $(OBJ)/core/fps.o
OBJS += $(OBJ)/core/soundmanager.o
OBJS += $(OBJ)/core/bg.o

INCDIR += $(SRC)/core/.

#CFLAGS += -O2
CFLAGS += -G0
CFLAGS += -Wall
CFLAGS += -g

CFLAGS += -I/usr/local/pspdev/psp/include
CFLAGS += -I/pspdev/psp/include/SDL


# $(shell $(SDL_CONFIG) --cflags) 
#CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -fsingle-precision-constant -mno-check-zero-division -ffast-math
#CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
#ASFLAGS = $(CFLAGS)

#BUILD_PRX = 1
PSP_FW_VERSION = 371

include $(PSPSDK)/lib/build.mak

#$ make
#Making object tree for kene ...
#psp-gcc -O3 -Werror -Isrc/core/. -I. -I/pspdev/psp/sdk/include -G0 -Wall -g 
#-I/usr/local/pspdev/psp/include -I/pspdev/psp/include/SDL  -c src/core/support.c -o
#obj/core/support.o
#cc1: warnings being treated as errors
#src/core/support.c: In function 'blit_scaled':
#src/core/support.c(573) : warning: control may reach end of non-void function 'getpixel' being inlined
#make: *** [obj/core/support.o] Error 1
# ???
$(OBJ)/core/support.o: $(SRC)/core/support.c
	psp-gcc -O3   $(CXXFLAGS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/%.c
	psp-gcc -O3 -Werror $(CXXFLAGS) -c $< -o $@


ifeq ($(PSPDEV),)
MD = -mkdir
RM = -rm
else
MD = -mkdir.exe
RM = -rm.exe
endif

mk_dir:
	@echo Making directry for $(TARGET) ...
	@$(MD) -p $(subst //,\,$(sort $(OBJDIRS)))
	@$(RM) -f PARAM.SFO

DELTREE = @$(RM) -f -rd obj

core:
	@echo Remove shooting core files.
	@$(RM) -f $(OBJ)/core/*.o
	@$(RM) -f $(OBJ)/core/enemy/*.o

rr:
	@echo Remove all temporaly files.
	@$(RM) -f PARAM.SFO
	@$(RM) -f *.elf
	@$(DELTREE)
