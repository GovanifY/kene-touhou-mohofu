#------------------------------------------------------------------------------
#  東方模倣風  ～ Toho Imitation Style.  Makefile
#------------------------------------------------------------------------------

TARGET = kene

PSP_EBOOT_TITLE = kene_r27

#------------------------------------------------------------------------------
# PSP setting
#------------------------------------------------------------------------------

# psp-2000対応(らしい)
PSP_LARGE_MEMORY = 1

# よくわかんない 滝汗; が3.71以降(つまりpsp-2000拡張メモリ)対応ってこと(らしい)
PSP_FW_VERSION = 371

# (gnu標準の) libc を使わないで、(PSPSDKが用意したpsp用の) libpsplibc を使う。
USE_PSPSDK_LIBC = 1

#------------------------------------------------------------------------------
# Compile option flag (主にデバッグ用)
#------------------------------------------------------------------------------

# 1 == GNU PROFILER を使う(game core)
#USE_PROFILE = 1
USE_PROFILE = 0

# 1 == GNU PROFILER を使う(ライブラリ)(USE_PROFILE = 1が必要)
#USE_LIB_PROFILE = 1
USE_LIB_PROFILE = 0

# 1 == FPU例外を無効にする
USE_FPU_HACK = 1
#USE_FPU_HACK = 0

# 0 == シンプルにして EBOOT.PBP のサイズをチェック
#SIMPLE = 1
SIMPLE = 0

# 現状 openGL 使ってないから openGL 排除しただけ
# 0 == CUSTOM_SDL (標準SDLは0)
CUSTOM_SDL = 1
#CUSTOM_SDL = 0


# 実験中
# 0 == CUSTOM_LIB
CUSTOM_LIB = 1
#CUSTOM_LIB = 0

#------------------------------------------------------------------------------
# Configurate Rule
#------------------------------------------------------------------------------

ifneq ($(SIMPLE),1)
	PSP_EBOOT_ICON  = ICON0.PNG
	PSP_EBOOT_ICON1 = ICON0.PMF
	PSP_EBOOT_PIC1  = PIC1.PNG
	PSP_EBOOT_SND0  = SND0.AT3
else
	PSP_EBOOT_ICON  = ICON0.PNG
	PSP_EBOOT_ICON1 =
	PSP_EBOOT_PIC1  =
	PSP_EBOOT_SND0  =
endif

EXTRA_TARGETS = mk_dir EBOOT.PBP

SRC = src
OBJ = obj

PSPDEV = $(shell psp-config --pspdev-path)
PSPSDK = $(shell psp-config --pspsdk-path)

#PSPBIN = $(PSPSDK)/../bin
#SDL_CONFIG = $(PSPBIN)/sdl-config
#SDL_CONFIG = $(PSPSDK)/../bin/sdl-config
#SDL_CONFIG = $(PSPDEV)/psp/bin/sdl-config

# sdl-config ( $(shell $(SDL_CONFIG) --libs) ) は使用できない。理由は以下の２点

# １．sdl-config に、-lSDLmain がある。これを使うと、新型psp(psp-2000)では起動できない。
# その為 libSDLmain.a の main(); 以外に main(); があり(当然psp-2000で起動させるために)
# こちらを使う(C言語はmain();関数が１つのみ、リンカは後着優先)が、
# 周辺関数の名前が同じならバッティングする危険がある。

# ２．ライブラリ間の依存関係が解消できない。
# sdl-config は SDL 以外のライブラリと依存関係が発生する場合を考慮しない為。

#------------------------------------------------------------------------------
# Library
#------------------------------------------------------------------------------

# 参考 http://himitsu.jpn.ph/yomimono/linux/staticlink.html
# 参考 http://www.hakodate-ct.ac.jp/~tokai/tokai/gtkmm/etc/p1.htm

LIBDIR =
LDFLAGS =

#------------------- for debug

ifneq ($(USE_PROFILE),1)
else
	#ifneq ($(USE_LIB_PROFILE),1)
	#else
		# -lpspprof は SDL とリンクする場合、位置依存がある。
		LIBS += -lpspprof
	#endif
	#	CORE_LIBS += -lpspprof
endif

# 位置が悪い場合
#＜略＞
#er.o obj/core/hiscore.o obj/core/fps.o obj/core/soundmanager.o obj/core/bg.o -lS
#DL_noGL -lSDL_mixer -lvorbisidec -lSDL_image -lpng -lz -ljpeg -lm -L/usr/local/p
#spdev/psp/lib -lSDLmain -lSDL -lm -L/usr/local/pspdev/psp/sdk/lib -lpspdebug -lp
#spgu -lpspctrl -lpspge -lpspdisplay -lpsphprm -lpspsdk -lpsprtc -lpspaudio -lc -
#lpspuser -lpsputility -lpspkernel -lpspnet_inet -lpsppower -lpspprof -lpspdebug
#-lpspdisplay -lpspge -lpspctrl -lpspsdk -lc -lpspnet -lpspnet_inet -lpspnet_apct
#l -lpspnet_resolver -lpsputility -lpspuser -lpspkernel -o kene.elf
#psp-fixup-imports kene.elf
#Error, could not fixup imports, stubs out of order.
#Ensure the SDK libraries are linked in last to correct this error
#make: *** [kene.elf] Error 1
# のようになりリンク出来ない。(PSPSDKのライブラリは最後にしてくれって言ってるから-lpspprofを最後にしてるのに...)
#＜略＞
#er.o obj/core/hiscore.o obj/core/fps.o obj/core/soundmanager.o obj/core/bg.o -lp
#spprof -lSDL_noGL -lSDL_mixer -lvorbisidec -lSDL_image -lpng -lz -ljpeg -lm -L/u
#sr/local/pspdev/psp/lib -lSDLmain -lSDL -lm -L/usr/local/pspdev/psp/sdk/lib -lps
#pdebug -lpspgu -lpspctrl -lpspge -lpspdisplay -lpsphprm -lpspsdk -lpsprtc -lpspa
#udio -lc -lpspuser -lpsputility -lpspkernel -lpspnet_inet -lpsppower -lpspdebug
#-lpspdisplay -lpspge -lpspctrl -lpspsdk -lc -lpspnet -lpspnet_inet -lpspnet_apct
#l -lpspnet_resolver -lpsputility -lpspuser -lpspkernel -o kene.elf
#make: *** [kene.elf] Interrupt
# こういう風ならリンク出来る。(-lpspprofの位置のみ違う)

# -lpspprof (例えば C:/cygwin/pspdev/psp/sdk/lib/libpspprof.a) に限らず、
#「stubs out of order.」が出る場合は、
# SDL とリンクする場合、位置依存のためリンク出来ない。
# Makefileのライブラリの位置を見直せば必ずリンクできるが、
#「$(shell $(SDL_CONFIG) --libs)」のせいで解消できない場合は結構泥沼。


#LIBS_org += -lstdc++		C++は使わない
#LIBS_org += -lSDL_mixer	使う
#LIBS_org += -lvorbisidec	使う
#LIBS_org += -lSDL_image	使う
#LIBS_org += -lpng			使う
#LIBS_org += -lz			使う
#LIBS_org += -ljpeg			使う
#LIBS_org += -lSDL_gfx
#LIBS_org += -lm			使う
#LIBS_org += $(shell $(SDL_CONFIG) --libs)
#LIBS_org += -lpsppower		使う


#------------------- ogg codec

ifneq ($(CUSTOM_LIB),1)
	# CUSTOM_LIB 以外の場合
	LIBS += -lvorbisidec
else
	LIBS += $(OBJ)/libtremor/libvorbisidec_test.a
endif

#------------------- 位置依存リンク

ifneq ($(CUSTOM_LIB),1)
	# CUSTOM_LIB 以外の場合

	#### 依存規則１
		# libSDL_mixer.a は libSDL.a, libSDL_noGL.a より必ず先にリンク
		LIBS += -lSDL_mixer
endif

#ifneq ($(CUSTOM_LIB),1)
	# CUSTOM_LIB 以外の場合

	#### 依存規則２
	ifneq ($(CUSTOM_SDL),1)
		# GL使う場合(位置依存がある。)
		# libSDL.a は libGL.aより必ず先にリンク
		LIBS += -lSDL
	else
		# GL使わない場合(位置依存がある。)
		# libSDL_noGL.a は libSDL.aより必ず先にリンク
		LIBS += -lSDL_noGL
	endif
#endif

ifneq ($(CUSTOM_LIB),1)
	# CUSTOM_LIB 以外の場合

	#### 依存規則３
	ifneq ($(CUSTOM_SDL),1)
		# GL使う場合(位置依存がある。)
		# libGL.a は libpspvfpu.aより必ず先にリンク
		LIBS += -lGL
		LIBS += -lpspvfpu
		# 若干小さくなる 1277241 -> 1276933 -> 1276917
	else
		# GL使わない場合(位置依存がある。)
		# 若干大きくなる 1228013 -> 1228029 (???)
	endif
endif

# SDLのライブラリは、元々依存関係がある。(SDLのライブラリ同士)
# PSPSDKのライブラリはSDLと使う場合、依存関係がある。(SDLのライブラリ < PSPSDKのライブラリ)
# libSDL_noGL.aは、SDLのOpenGLを無効にしたもの、依存関係がある。(libSDL_noGL.a < SDLのライブラリ)
#
# 纏めると、最低限以下の依存関係を守れば良い。
# libSDL_mixer.a < libSDL_noGL.a < libSDL.a < libGL.a < libpspvfpu.a
# もし、複数リンクした場合は依存関係が守られないので、必ず一つだけリンクする必要がある。
# この為「sdl-configは使用できない」
#
# この依存関係が守られない場合、多重リンクで、プログラムが期待どおりに動かない可能性がある。
# 同内容の関数が名前を変えて多重リンクされてしまう。
# もし関数内部でstaticな内部変数等を用いていると、正常に動作しない。(ローカル変数が正常に動作しない。)
#
# SDL_FreeSurface();が正常動作しない原因はこの辺かも知れない。

#-------------------


ifneq ($(CUSTOM_LIB),1)
	# CUSTOM_LIB 以外の場合
	LIBS += -lSDL_image
	LIBS += -lpng
	LIBS += -ljpeg
endif

LIBS += -lz

LIBS += -lm
LIBS += -lc

### SDL関連は先(位置依存がある。)

#ifneq ($(CUSTOM_LIB),1)
	# CUSTOM_LIB 以外の場合
	ifneq ($(CUSTOM_SDL),1)
		# GL使う場合(位置依存がある。)
		# libSDL.a は libGL.aより必ず先にリンク
	else
		# GL使わない場合(位置依存がある。)
		# 若干小さくなる 1228029 -> 1228013 -> 1227969 (???)
#		LIBS += -lSDL
	endif
#endif

#LIBS += $(shell $(SDL_CONFIG) --libs)
# -lSDLmain はリンクしない。


### PSPSDK関連は後(位置依存がある。)

LIBS += -lpsppower
#LIBS += -lpspgum
LIBS += -lpspgu
LIBS += -lpsphprm
LIBS += -lpspaudio
LIBS += -lpsprtc

ifneq ($(CUSTOM_LIB),1)
	# CUSTOM_LIB 以外の場合
	ifneq ($(CUSTOM_SDL),1)
		# GL使う場合
		#manager.o obj/core/bg.o -lSDL_mixer -lvorbisidec -lSDL_image -lpng -lz -ljpeg -l
		#m -lc -lSDL -lGL -lpspvfpu -lpsppower -lpspgu -lpsphprm -lpspaudio -lpsprtc -lps
		#pdebug -lpspdisplay -lpspge -lpspctrl -lpspsdk -lpsplibc -lpspnet -lpspnet_inet
		#-lpspnet_apctl -lpspnet_resolver -lpsputility -lpspuser -lpspkernel -o kene.elf
	else
		# GL使わない場合
		#manager.o obj/core/bg.o -lSDL_noGL -lSDL_mixer -lvorbisidec -lSDL_image -lpng -l
		#z -ljpeg -lm -lc -lSDL -lpsppower -lpspgu -lpsphprm -lpspaudio -lpsprtc -lpspdeb
		#ug -lpspdisplay -lpspge -lpspctrl -lpspsdk -lpsplibc -lpspnet -lpspnet_inet -lps
		#pnet_apctl -lpspnet_resolver -lpsputility -lpspuser -lpspkernel -o kene.elf
	endif
endif

# -lSDLmain があると色々おかしな事になる。 -lSDLmain はリンクしない。

#------------------------------------------------------------------------------
# Object Directory
#------------------------------------------------------------------------------

OBJDIRS += $(OBJ)
OBJDIRS += $(OBJ)/core
OBJDIRS += $(OBJ)/core/enemy
OBJDIRS += $(OBJ)/font

# add after the others, in custom.mak

#------------------------------------------------------------------------------
# Object
#------------------------------------------------------------------------------

#	particle.o


OBJS += $(OBJ)/core/bullet_object.o
OBJS += $(OBJ)/core/danmaku.o
OBJS += $(OBJ)/core/spell_card.o
OBJS += $(OBJ)/core/enemy/common.o

OBJS += $(OBJ)/core/enemy/boss_01_alice.o
OBJS += $(OBJ)/core/enemy/boss_02_cirno.o
OBJS += $(OBJ)/core/enemy/boss_03_aya.o
OBJS += $(OBJ)/core/enemy/boss_03_mitei.o
OBJS += $(OBJ)/core/enemy/boss_04_kaguya.o
OBJS += $(OBJ)/core/enemy/boss_05_pache.o
OBJS += $(OBJ)/core/enemy/boss_06_sakuya.o

OBJS += $(OBJ)/core/enemy/chuu_01_youkai1.o
OBJS += $(OBJ)/core/enemy/chuu_02_youkai2.o

OBJS += $(OBJ)/core/enemy/zako_01_obake.o
OBJS += $(OBJ)/core/enemy/zako_02_yukari1.o
OBJS += $(OBJ)/core/enemy/zako_03_yukari2.o
OBJS += $(OBJ)/core/enemy/zako_04_aka_kedama.o
OBJS += $(OBJ)/core/enemy/zako_05_niji_kedama.o
OBJS += $(OBJ)/core/enemy/zako_06_midori_kedama1.o
OBJS += $(OBJ)/core/enemy/zako_07_midori_kedama2.o
OBJS += $(OBJ)/core/enemy/zako_08_kedama1.o
OBJS += $(OBJ)/core/enemy/zako_09_kedama2.o
OBJS += $(OBJ)/core/enemy/zako_10_inyou1.o
OBJS += $(OBJ)/core/enemy/zako_11_tatsumaki1.o
OBJS += $(OBJ)/core/enemy/zako_12_kakomi1.o
OBJS += $(OBJ)/core/enemy/zako_13_meido1.o
OBJS += $(OBJ)/core/enemy/zako_14_meido2.o
OBJS += $(OBJ)/core/enemy/zako_15_meido3.o
OBJS += $(OBJ)/core/enemy/zako_16_meido4.o
OBJS += $(OBJ)/core/enemy/zako_17_ao_yousei1.o
OBJS += $(OBJ)/core/enemy/zako_18_ao_yousei2.o
OBJS += $(OBJ)/core/enemy/zako_19_ao_yousei3.o
OBJS += $(OBJ)/core/enemy/zako_20_ao_yousei4.o

OBJS += $(OBJ)/core/enemy/kanji_text.o
OBJS += $(OBJ)/core/enemy/game_clear.o
OBJS += $(OBJ)/core/enemy/tikei.o
OBJS += $(OBJ)/core/enemy/clouds.o
OBJS += $(OBJ)/core/enemy/bakuhatsu.o

OBJS += $(OBJ)/core/stage_clear.o
OBJS += $(OBJ)/core/game_core.o
OBJS += $(OBJ)/core/player.o

OBJS += $(OBJ)/core/bonus.o
#---
OBJS += $(OBJ)/core/draw_screen.o

OBJS += $(OBJ)/core/sprite.o
OBJS += $(OBJ)/core/bg.o
OBJS += $(OBJ)/core/font.o
#---

OBJS += $(OBJ)/core/audio.o

#---
OBJS += $(OBJ)/core/score_panel.o

#OBJS += $(OBJ)/core/tiny_strcmp.o
OBJS += $(OBJ)/core/load_stage.o

#---
OBJS += $(OBJ)/core/menu.o

OBJS += $(OBJ)/core/game_over.o
OBJS += $(OBJ)/core/name_entry.o

OBJS += $(OBJ)/core/key_config.o

OBJS += $(OBJ)/core/ini_file.o

#OBJS += $(OBJ)/core/fps.o


 OBJS += $(OBJ)/core/graphics00.o 

OBJS += $(OBJ)/core/scenario_script.o
OBJS += $(OBJ)/font/shinonome16p.o

OBJS += $(OBJ)/core/boot_main.o
OBJS += $(OBJ)/core/initialize.o
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
ifneq ($(USE_FPU_HACK),1)
else
	OBJS += $(OBJ)/core/disable_fpu.o
	CFLAGS += -DHACK_FPU=1
endif
#------------------------------------------------------------------------------
ifeq ($(CUSTOM_LIB),1)
	include $(SRC)/custom.mak
endif
#------------------------------------------------------------------------------
# CFLAGS
#------------------------------------------------------------------------------

INCDIR += $(SRC)/core/.

#CFLAGS += -O2
CFLAGS += -G0
CFLAGS += -Wall
CFLAGS += -g

CFLAGS += -ffast-math
	# このオプションを指定すると、実行速度を最適化するという観点から、
	# ある面で ANSI や IEEE の規則や仕様を破ることを GCC に許す。
	# 例えば、このオプションを指定すると、GCC は、sqrt 関数の引数が負にならないとか、
	# 浮動小数点値がNaN になることはないという仮定を行なう。 


#CFLAGS += -funroll-all-loops
#CFLAGS += -pipe
#CFLAGS += -falign-functions
#CFLAGS += -falign-jumps
#CFLAGS += -falign-loops
#CFLAGS += -falign-labels
#CFLAGS += -freorder-blocks
#CFLAGS += -fprefetch-loop-arrays

CFLAGS += -I/usr/local/pspdev/psp/include
CFLAGS += -I/pspdev/psp/include/SDL


# http://www.sra.co.jp/wingnut/gcc/gcc-j.html
# -fomit-frame-pointer フレームポインタを必要としない関数については、フレームポインタをレジスタに保持しないようにする。
# これにより、フレームポインタをセーブ、設定、リストアする命令をなくすことができる。
# また、多くの関数で利用可能なレジスタが一つ増える。
# また、機種によってはデバッグが不可能になる。 (pspでは -pg が使えなくなる)

ifneq ($(USE_PROFILE),1)
	CORE_CFLAGS += -fomit-frame-pointer
else
	CORE_CFLAGS += -DENABLE_PROFILE
	CORE_CFLAGS += -pg
endif

ifneq ($(USE_LIB_PROFILE),1)
	LIB_CFLAGS += -fomit-frame-pointer
else
	LIB_CFLAGS += -DENABLE_PROFILE
	LIB_CFLAGS += -pg
endif

# $(shell $(SDL_CONFIG) --cflags) 
#CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -fsingle-precision-constant -mno-check-zero-division -ffast-math
#CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
#ASFLAGS = $(CFLAGS)

CXXFLAGS += $(CFLAGS)
CXXFLAGS += -fno-exceptions
CXXFLAGS += -fno-rtti

#BUILD_PRX = 1

#------------------------------------------------------------------------------
# libSDL_noGL
#------------------------------------------------------------------------------



#------------------------------------------------------------------------------
# build.mak
#------------------------------------------------------------------------------

include $(PSPSDK)/lib/build.mak

#---------------------------------------------------------------------
# Rules to make libraries
#---------------------------------------------------------------------

$(OBJ)/libtremor/libvorbisidec_test.a: $(TREMOR_OBJS)

#---------------------------------------------------------------------
# Rules to manage files
#---------------------------------------------------------------------

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

# malloc free で warnings
#$(OBJ)/jpeg/%.o: $(SRC)/jpeg/%.c
#	psp-gcc -O3 $(CFLAGS) -c $< -o $@

# malloc free で warnings
#$(OBJ)/libpng/%.o: $(SRC)/libpng/%.c
#	psp-gcc -O3 $(CFLAGS) -c $< -o $@

# malloc free で warnings
#$(OBJ)/SDL_image/IMG_png.o: $(SRC)/SDL_image/IMG_png.c
#	psp-gcc -O3 $(CFLAGS) -c $< -o $@


#rc/libtremor/misc.h(209) : warning: implicit declaration of function '_ilog'
#rc/libtremor/misc.h(209) : warning: implicit declaration of function 'abs'
#   で warnings

# MP3の場合(c++ なのは、 smpeg だけ)
$(OBJ)/%.o: $(SRC)/%.cpp
	psp-gcc -O3 $(CXXFLAGS) $(LIB_CFLAGS) -c $< -o $@

# OGGの場合( alloca(); で warnings )
$(OBJ)/libtremor/%.o: $(SRC)/libtremor/%.c
	psp-gcc -O3 $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@

# ゲームコアの場合(ライブラリとgnuプロファイラの扱いを変える)
$(OBJ)/core/%.o: $(SRC)/core/%.c
	psp-gcc -O3 -Werror $(CFLAGS) $(CORE_CFLAGS) -c $< -o $@

# その他のライブラリの場合
$(OBJ)/%.o: $(SRC)/%.c
	psp-gcc -O3 -Werror $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@

# アセンブラの場合
$(OBJ)/%.o: $(SRC)/%.S
	psp-gcc -O3 -Werror $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@

#	psp-gcc -O3 -Werror $(CFLAGS) -c $< -o $@
#	psp-gcc -O3 -Werror $(CXXFLAGS) -c $< -o $@

$(OBJ)/%.a:
	@echo Archiving $@...
	@$(AR) -r $@ $^

#------------------------------------------------------------------------------
# Utilities
#------------------------------------------------------------------------------

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

font:
	@echo Remove custom font files.
	@$(RM) -f $(OBJ)/font/*.o

rr:
	@echo Remove all temporaly files.
	@$(RM) -f PARAM.SFO
	@$(RM) -f *.elf
	@$(DELTREE)

