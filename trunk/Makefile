
TARGET = kene

PSP_EBOOT_TITLE = kene_r23

#SIMPLE = 1
SIMPLE = 0
ifneq ($(SIMPLE),1)
	PSP_EBOOT_ICON = ICON0.PNG
	PSP_EBOOT_ICON1= ICON0.PMF
	PSP_EBOOT_PIC1 = PIC1.PNG
	PSP_EBOOT_SND0 = SND0.AT3
else
	PSP_EBOOT_ICON = ICON0.PNG
	PSP_EBOOT_ICON1=
	PSP_EBOOT_PIC1 =
	PSP_EBOOT_SND0 =
endif

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

	# GL使う場合
#	LIBS += -lSDL -lGL -lpspvfpu 
	# GL使わない場合
	LIBS += -lSDL_noGL

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

OBJS += $(OBJ)/core/shinonomefont.o
OBJS += $(OBJ)/core/zenkaku.o

OBJS += $(OBJ)/core/enemy.o

OBJS += $(OBJ)/core/enemy/boss_01_alice.o
OBJS += $(OBJ)/core/enemy/boss_02_cirno.o
OBJS += $(OBJ)/core/enemy/boss_03_aya.o
OBJS += $(OBJ)/core/enemy/boss_04_kaguya.o
OBJS += $(OBJ)/core/enemy/boss_05_sakuya.o

OBJS += $(OBJ)/core/enemy/chuu_01_youkai1.o

OBJS += $(OBJ)/core/enemy/zako_01_obake.o
OBJS += $(OBJ)/core/enemy/zako_02_yukari1.o
OBJS += $(OBJ)/core/enemy/zako_03_yukari2.o
OBJS += $(OBJ)/core/enemy/zako_04_aka_kedama.o
OBJS += $(OBJ)/core/enemy/zako_05_niji_kedama.o
OBJS += $(OBJ)/core/enemy/zako_06_midori_kedama1.o
OBJS += $(OBJ)/core/enemy/zako_07_midori_kedama2.o
OBJS += $(OBJ)/core/enemy/zako_08_kedama1.o
OBJS += $(OBJ)/core/enemy/zako_09_inyou1.o
OBJS += $(OBJ)/core/enemy/zako_10_tatsumaki1.o
OBJS += $(OBJ)/core/enemy/zako_11_kakomi1.o
OBJS += $(OBJ)/core/enemy/zako_12_meido1.o
OBJS += $(OBJ)/core/enemy/zako_13_meido2.o
OBJS += $(OBJ)/core/enemy/zako_14_meido3.o
OBJS += $(OBJ)/core/enemy/zako_15_meido4.o
OBJS += $(OBJ)/core/enemy/zako_16_ao_yousei1.o
OBJS += $(OBJ)/core/enemy/zako_17_ao_yousei2.o

OBJS += $(OBJ)/core/enemy/eigo_text.o
OBJS += $(OBJ)/core/enemy/game_clear.o
OBJS += $(OBJ)/core/enemy/tikei.o
OBJS += $(OBJ)/core/enemy/clouds.o

OBJS += $(OBJ)/core/support.o
OBJS += $(OBJ)/core/intro.o
OBJS += $(OBJ)/core/thegame.o
OBJS += $(OBJ)/core/loadlv.o
OBJS += $(OBJ)/core/score.o
OBJS += $(OBJ)/core/scenario.o
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
# ???  警告: 制御が非 void 関数の終りに到達しました 
#$(OBJ)/core/support.o: $(SRC)/core/support.c
#	psp-gcc -O3   $(CXXFLAGS) -c $< -o $@
#	//else
#	//{
#		return (0);
#	//}

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
