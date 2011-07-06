
#------------------------------------------------------------------------------
#  東方模倣風 ～ Toho Imitation Style.  Makefile
#------------------------------------------------------------------------------

TARGET = kene

PSP_EBOOT_TITLE = kene_r32_debug

#RELESE_DATE = "2010/06/06"

#------------------------------------------------------------------------------
# PSP setting
#------------------------------------------------------------------------------

# psp-2000対応(らしい)
PSP_LARGE_MEMORY = 1

# よくわかんない 滝汗; が3.71以降(つまりpsp-2000拡張メモリ)対応ってこと(らしい)
PSP_FW_VERSION = 371

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

#------------------------------------------------------------------------------
# Compile option flag (r31時点で間に合わなかった。研究中)
#------------------------------------------------------------------------------
# 注意: ここのカテゴリのオプションはr31時点で間に合わなかったのでダミーです。
# 一応、USE_BUNKAI_KINOU = 1、CUSTOM_231SDL = 1 にすると、
# 模倣風起動時にSDL のマウスカーソルが表示されなくなるのが確認できます。
# しかしどこかバグがあるのか、ハングアップ(突然死)が多くなる気がします。不安定です。
#------------------------------------------------------------------------------

# libSDL.a を分解してコピーする機能。
# 1 == USE_BUNKAI_KINOU (標準は 1 : コンパイル前に libSDL.a を展開して、objにコピー)
# 0 == USE_BUNKAI_KINOU (開発中はウザイからoff: off は 0)
USE_BUNKAI_KINOU = 1
#USE_BUNKAI_KINOU = 0

# <<てすと>>
# 0 == CUSTOM_231SDL (標準SDLは0)
CUSTOM_231SDL = 1
#CUSTOM_231SDL = 0

#------------------------------------------------------------------------------
# Custom Lib option flag (カスタマイズ)
#------------------------------------------------------------------------------
# 注意: ここのカテゴリのオプションを変更した場合は、SDL_mixer のリコンパイルが必ず必要です。
# (設定変更後、make rr↓してからmake↓推奨)
#------------------------------------------------------------------------------

# 実験中
# 0 == CUSTOM_LIB
CUSTOM_LIB = 1
#CUSTOM_LIB = 0

# MP3の設定
# 注意: MP3の使用可否を変更した場合は、SDL_mixer のリコンパイルが必ず必要です。(設定変更後、make rr↓してからmake↓推奨)

	## 注意: LINK_SMPEG_MP3 と LINK_MAD_MP3 を両方 1 にする事は出来ません。
	# ok 	LINK_SMPEG_MP3 = 0	LINK_MAD_MP3 = 0	MP3 とか使わない。
	# ok 	LINK_SMPEG_MP3 = 1	LINK_MAD_MP3 = 0	smpeg の MP3 (～r30) を使う。(不安定かつ、遅い、かつ、ぶちぶちノイズ)
	# ok 	LINK_SMPEG_MP3 = 0	LINK_MAD_MP3 = 1	libmad の mp3 (r31～)を使う。(安定度は高い、遅いけど...、45fpsぐらいに低下する。)
	# NG 	LINK_SMPEG_MP3 = 1	LINK_MAD_MP3 = 1	ソースレベルで対応がないので無理、NG。

	## smpeg の MP3 (～r30)
	# 0 == LINK_SMPEG_MP3:標準設定 [ smpeg の MP3の使用可否 ] (0:使用しない、1:使用する)
	LINK_SMPEG_MP3 = 0
	#LINK_SMPEG_MP3 = 1

	## libmad の mp3 (r31～)
	# 1 == LINK_MAD_MP3:標準設定 [ libmad の MP3の使用可否 ] (0:使用しない、1:使用する)
	#LINK_MAD_MP3 = 0
	LINK_MAD_MP3 = 1

	# r31:色々やってみたけど、どうしても smpeg が不安定(突然死が多い)なので、 smpeg は止めて、 libmad にしました。
	# (もし仮に両方にバグが無いと想定すると)たぶんバッファオーバーランなんじゃないかな？

# 1 == LINK_OGG:標準設定 [ oggの使用可否 ] (0:使用しない、1:使用する)
# 注意: oggの使用可否を変更した場合は、SDL_mixer のリコンパイルが必ず必要です。
LINK_OGG = 1
#LINK_OGG = 0

# 0 == LINK_LIBTREMOR_LOW_MEM: oggを使用する場合、どちらの libtremor を使うか選択します。
# 0: libtremor_large  を使用する。
# 1: libtremor_lowmem を使用する。
# 0 == LINK_LIBTREMOR_LOW_MEM:標準設定 [ libtremor_lowmemの使用可否 ] (0:使用しない、1:使用する)
# 注意: libtremor_lowmem の選択を変更した場合は、SDL_mixer のリコンパイルが必ず必要です。
LINK_LIBTREMOR_LOW_MEM = 1
#LINK_LIBTREMOR_LOW_MEM = 0

#------------------------------------------------------------------------------
# Configurate Rule
#------------------------------------------------------------------------------

ifneq ($(SIMPLE),1)
	PSP_EBOOT_ICON	 = ICON0.PNG
	PSP_EBOOT_ICON1  = ICON0.PMF
	PSP_EBOOT_UNKPNG = ICON1.PNG
	PSP_EBOOT_PIC1	 = PIC1.PNG
	PSP_EBOOT_SND0	 = SND0.AT3
else
	PSP_EBOOT_ICON	 = ICON0.PNG
	PSP_EBOOT_ICON1  = 
	PSP_EBOOT_UNKPNG = 
	PSP_EBOOT_PIC1	 = 
	PSP_EBOOT_SND0	 = 
endif

ifneq ($(USE_BUNKAI_KINOU),1)
	# 0==通常開発
	EXTRA_TARGETS	 = mk_dir EBOOT.PBP
else
	# 1==とりあえず、自動で全部コンパイル出来る筈
	EXTRA_TARGETS	 = mk_dir SDL_lib_bunkai EBOOT.PBP
endif

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

#LDFLAGS += -Wl,-Map=mohoufu.map

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
#LIBS_org += -ljpeg 		使う
#LIBS_org += -lSDL_gfx
#LIBS_org += -lm			使う
#LIBS_org += $(shell $(SDL_CONFIG) --libs)
#LIBS_org += -lpsppower 	使う


#------------------- ogg codec
ifneq ($(LINK_OGG),1)
	##not include	## oggを使わない場合
else
	##
	ifneq ($(LINK_LIBTREMOR_LOW_MEM),1)
		##only large
		LIBTREMOR_DIR = libtremor_large
	else
		##only LINK_LIBTREMOR_LOW_MEM
		LIBTREMOR_DIR = libtremor_lowmem
	endif
	##
	ifneq ($(CUSTOM_LIB),1)
		# CUSTOM_LIB 以外の場合
		LIBS += -lvorbisidec
	else
		#LIBS += $(OBJ)/libtremor/libvorbisidec_test.a
		LIBS += $(OBJ)/$(LIBTREMOR_DIR)/libvorbisidec_test.a
	endif
endif

#------------------- 位置依存リンク

ifneq ($(CUSTOM_LIB),1)
	# CUSTOM_LIB 以外の場合

	#### 依存規則１
		# libSDL_mixer.a は libSDL.a, libSDL_noGL.a より必ず先にリンク
		LIBS += -lSDL_mixer
endif

ifneq ($(CUSTOM_231SDL),1)
	# CUSTOM_231SDL 以外の場合
	# CUSTOM_LIB 以外の場合

	#### 依存規則２
	ifneq ($(CUSTOM_SDL),1)
		# GL使う場合(位置依存がある。)
		# libSDL.a は libGL.aより必ず先にリンク
		LIBS += -lSDL
	else
		# GL使わない場合(位置依存がある。)
		# libSDL_noGL.a は libSDL.aより必ず先にリンク
		#SDL231
		LIBS += -lSDL_noGL
	#	LIBS += -lSDL_mohou00
	endif
endif

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
	#標準
	#LIBS += -lpspmath
else
	#カスタム
	#LIBS += $(OBJ)/libpspmath/libpspmath.a
endif
	LIBS += $(OBJ)/vfpu/lib_vfpu.a

LIBS += -lz

#LIBS += -lm


#-------------------
# libc 関連
#-------------------

# (gnu標準の) libc を使わないで、(PSPSDKが用意したpsp用の) libpsplibc を使う。
#USE_PSPSDK_LIBC = 1
#とりあえずコンパイルできない環境がありそうな気がするので止めとく。(-lcと同時にリンクしてる為)

# うーん正直言ってnewlibとlibpsplibc両方リンクしてるのは良くないのですがSDLの都合で。(主に signal(); )
# (r32)とりあえずnewlibのみで。(libpsplibc使わない)
LIBS += -lc

# smpeg は C++ ですが、libstdc++.a は使いません。
#LIBS += -lstdc++

#---------------------------

### SDL関連は先(位置依存がある。)

##ifneq ($(CUSTOM_LIB),1)
#	# CUSTOM_LIB 以外の場合
#	ifneq ($(CUSTOM_SDL),1)
#		# GL使う場合(位置依存がある。)
#		# libSDL.a は libGL.aより必ず先にリンク
#	else
#		# GL使わない場合(位置依存がある。)
#		# 若干小さくなる 1228029 -> 1228013 -> 1227969 (???)
#	#	LIBS += -lSDL
#	endif
##endif

#---------------------------

#LIBS += $(shell $(SDL_CONFIG) --libs)
# -lSDLmain があると色々おかしな事になる。 -lSDLmain はリンクしない。

#---------------------------

### PSPSDK関連は後(位置依存がある。)

LIBS += -lpsppower
#LIBS += -lpspgum
LIBS += -lpspgu
LIBS += -lpsphprm
LIBS += -lpspaudio
LIBS += -lpsprtc

#---------------------------

#LIBS += -lpspdebug
#LIBS += -lpspdisplay
#LIBS += -lpspge
#LIBS += -lpspctrl
#LIBS += -lpspsdk
#LIBS += -lc
#LIBS += -lpspnet
#LIBS += -lpspnet_inet
#LIBS += -lpspnet_apctl
#LIBS += -lpspnet_resolver
#LIBS += -lpsputility
#LIBS += -lpspuser
#LIBS += -lpspkernel

#---------------------------

#ifneq ($(CUSTOM_LIB),1)
#	# CUSTOM_LIB 以外の場合
#	ifneq ($(CUSTOM_SDL),1)
#		# GL使う場合
#		#manager.o obj/core/bg.o -lSDL_mixer -lvorbisidec -lSDL_image -lpng -lz -ljpeg -l
#		#m -lc -lSDL -lGL -lpspvfpu -lpsppower -lpspgu -lpsphprm -lpspaudio -lpsprtc -lps
#		#pdebug -lpspdisplay -lpspge -lpspctrl -lpspsdk -lpsplibc -lpspnet -lpspnet_inet
#		#-lpspnet_apctl -lpspnet_resolver -lpsputility -lpspuser -lpspkernel -o kene.elf
#	else
#		# GL使わない場合
#		#manager.o obj/core/bg.o -lSDL_noGL -lSDL_mixer -lvorbisidec -lSDL_image -lpng -l
#		#z -ljpeg -lm -lc -lSDL -lpsppower -lpspgu -lpsphprm -lpspaudio -lpsprtc -lpspdeb
#		#ug -lpspdisplay -lpspge -lpspctrl -lpspsdk -lpsplibc -lpspnet -lpspnet_inet -lps
#		#pnet_apctl -lpspnet_resolver -lpsputility -lpspuser -lpspkernel -o kene.elf
#	endif
#endif

#------------------------------------------------------------------------------
# Object Directory
#------------------------------------------------------------------------------

OBJDIRS += $(OBJ)
OBJDIRS += $(OBJ)/core
OBJDIRS += $(OBJ)/core/jiki
OBJDIRS += $(OBJ)/core/douchu
OBJDIRS += $(OBJ)/core/boss
OBJDIRS += $(OBJ)/core/tama
OBJDIRS += $(OBJ)/core/draw
OBJDIRS += $(OBJ)/core/menu
OBJDIRS += $(OBJ)/font

# add after the others, in custom.mak

#------------------------------------------------------------------------------
# Object
#------------------------------------------------------------------------------

#	particle.o

OBJS += $(OBJ)/core/my_math.o
OBJS += $(OBJ)/core/tama/bullet_vector.o
OBJS += $(OBJ)/core/tama/bullet_angle.o
#OBJS += $(OBJ)/core/tama/bullet_system.o
#OBJS += $(OBJ)/core/tama/sprite_old.o
#OBJS += $(OBJ)/core/tama/sprite_SDL.o
OBJS += $(OBJ)/core/tama/sprite_bullet.o
OBJS += $(OBJ)/core/tama/danmaku.o
OBJS += $(OBJ)/core/boss/spell_card.o
OBJS += $(OBJ)/core/boss/common.o
#; ボス(本体)
#OBJS += $(OBJ)/core/boss/boss_08_kene.o
#OBJS += $(OBJ)/core/boss/boss_07_cirno.o
OBJS += $(OBJ)/core/boss/boss_06_sakuya.o
OBJS += $(OBJ)/core/boss/boss_05_pache.o
OBJS += $(OBJ)/core/boss/boss_04_aya.o
OBJS += $(OBJ)/core/boss/boss_03_kaguya.o
OBJS += $(OBJ)/core/boss/boss_02_mima.o
#OBJS += $(OBJ)/core/boss/boss_01_alice.o	# 模倣風標準機能ですべて対処出来るので固有機能はなし。
OBJS += $(OBJ)/core/boss/boss_move.o
#; ボスのオプション
OBJS += $(OBJ)/core/boss/option_12_sakuya_baramaki.o
OBJS += $(OBJ)/core/boss/option_10_sakuya_ryoute.o
OBJS += $(OBJ)/core/boss/option_09_sakuya_360knife.o
OBJS += $(OBJ)/core/boss/option_08_sakuya_star.o
OBJS += $(OBJ)/core/boss/option_07_sakuya_laser.o
#
OBJS += $(OBJ)/core/boss/option_15_pache_laser.o
OBJS += $(OBJ)/core/boss/option_11_pache_doll.o
#
OBJS += $(OBJ)/core/boss/option_06_aya_momiji02.o
OBJS += $(OBJ)/core/boss/option_05_aya_momiji01.o
#
OBJS += $(OBJ)/core/boss/option_13_kaguya_dolls.o
OBJS += $(OBJ)/core/boss/option_04_kaguya_tamanoe.o
OBJS += $(OBJ)/core/boss/option_03_kaguya_houmotu.o
OBJS += $(OBJ)/core/boss/option_02_mima_hosi.o
#
OBJS += $(OBJ)/core/boss/option_14_alice_doll_type_b.o
OBJS += $(OBJ)/core/boss/option_01_alice_doll.o
#; 中ボスもどき
OBJS += $(OBJ)/core/douchu/chuu_boss.o
#OBJS += $(OBJ)/core/douchu/chuu_03_rumia.o
#OBJS += $(OBJ)/core/douchu/chuu_02_youkai2.o
#OBJS += $(OBJ)/core/douchu/chuu_01_youkai1.o
#; ザコのオプション
OBJS += $(OBJ)/core/douchu/option_50_homing.o
#; ザコ
OBJS += $(OBJ)/core/douchu/zako.o
#	/* 魔方陣[A/B] */
#OBJS += $(OBJ)/core/douchu/zako_11_mahoujin.o
#	/* 竜巻 陰陽玉 */
#OBJS += $(OBJ)/core/douchu/zako_10_inyou1.o
#OBJS += $(OBJ)/core/douchu/zako_11_tatsumaki1.o
#	/* 妖怪 */
#OBJS += $(OBJ)/core/douchu/zako_12_kakomi1.o
#OBJS += $(OBJ)/core/douchu/zako_04_aka_kedama.o
#	/* 毛玉 */
#OBJS += $(OBJ)/core/douchu/zako_03_yukari2.o
#OBJS += $(OBJ)/core/douchu/zako_06_midori_kedama1.o
#OBJS += $(OBJ)/core/douchu/zako_07_midori_kedama2.o
#OBJS += $(OBJ)/core/douchu/zako_08_kedama1.o
#OBJS += $(OBJ)/core/douchu/zako_09_kedama2.o
#	/* [C妖精]その他ザコ */
#OBJS += $(OBJ)/core/douchu/zako_01_obake1.o
#OBJS += $(OBJ)/core/douchu/zako_02_obake2.o
#OBJS += $(OBJ)/core/douchu/zako_05_karasu1.o
#	/* [B妖精]中妖精 */
#OBJS += $(OBJ)/core/douchu/zako_13_meido1.o
#OBJS += $(OBJ)/core/douchu/zako_14_meido2.o
#OBJS += $(OBJ)/core/douchu/zako_15_meido3.o
#OBJS += $(OBJ)/core/douchu/zako_16_meido4.o
#	/* [A妖精]小妖精 */
#OBJS += $(OBJ)/core/douchu/zako_17_ao_yousei1.o
#OBJS += $(OBJ)/core/douchu/zako_18_ao_yousei2.o
#OBJS += $(OBJ)/core/douchu/zako_19_ao_yousei3.o
#OBJS += $(OBJ)/core/douchu/zako_20_ao_yousei4.o
#; 敵以外だが敵扱い
OBJS += $(OBJ)/core/douchu/kanji_text.o
OBJS += $(OBJ)/core/douchu/game_clear.o
OBJS += $(OBJ)/core/douchu/clouds.o
OBJS += $(OBJ)/core/douchu/bakuhatsu.o
#
OBJS += $(OBJ)/core/game_core.o

OBJS += $(OBJ)/core/jiki/jiki.o
OBJS += $(OBJ)/core/jiki/jiki_option.o
OBJS += $(OBJ)/core/jiki/jiki_shot.o
OBJS += $(OBJ)/core/jiki/jiki_bomber.o

OBJS += $(OBJ)/core/jiki/bonus_item.o
#---
OBJS += $(OBJ)/core/draw/gu_load_texture.o
OBJS += $(OBJ)/core/draw/gu_draw_screen.o
OBJS += $(OBJ)/core/draw/score_panel.o


OBJS += $(OBJ)/core/bg.o
OBJS += $(OBJ)/core/font.o
#---
OBJS += $(OBJ)/core/audio.o
#---
#OBJS += $(OBJ)/core/tiny_strcmp.o
OBJS += $(OBJ)/core/load_stage.o

#---
OBJS += $(OBJ)/core/menu/title.o
OBJS += $(OBJ)/core/menu/pause.o
OBJS += $(OBJ)/core/menu/ask_continue.o
OBJS += $(OBJ)/core/menu/select_stage.o
OBJS += $(OBJ)/core/menu/select_jiki.o

OBJS += $(OBJ)/core/menu/game_over.o
OBJS += $(OBJ)/core/menu/name_entry.o

OBJS += $(OBJ)/core/menu/option_menu.o
OBJS += $(OBJ)/core/menu/key_config.o
OBJS += $(OBJ)/core/menu/music_room.o

OBJS += $(OBJ)/core/my_file.o

#OBJS += $(OBJ)/core/fps.o

OBJS += $(OBJ)/core/draw/graphics00.o 

#/* スクリプトシステムは分離予定あり */
OBJS += $(OBJ)/core/draw/scenario_script.o
#OBJS += $(OBJ)/core/draw/game_script.o

OBJS += $(OBJ)/core/draw/draw_kanji.o
OBJS += $(OBJ)/font/shinonome16p.o

OBJS += $(OBJ)/core/boot_main.o
OBJS += $(OBJ)/core/initialize.o

#OBJS += $(OBJ)/libpspmath.a

#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
ifneq ($(USE_FPU_HACK),1)
else
	OBJS += $(OBJ)/core/asm_first.o
	CFLAGS += -DHACK_FPU=1
endif
#------------------------------------------------------------------------------
ifeq ($(CUSTOM_LIB),1)
	include $(SRC)/custom.mak
endif
#------------------------------------------------------------------------------
# CFLAGS
#------------------------------------------------------------------------------

#INCDIR += $(SRC)/core/.
INCDIR += $(SRC)/core/include

##	CFLAGS += -O2
	# 模倣風では、ここで指定しない。(このオプションは最適化の設定)

CFLAGS += -G0
CFLAGS += -Wall
	# Warning があればすべて出力。
	# 模倣風では -Werror なので Warning扱いは、エラー扱いにして即コンパイル中止。
CFLAGS += -g

CFLAGS += -std=gnu99
	# 1999年の GNU 規格に沿った C言語コード とする。(暗黙の省略とか、voidポインタの扱いとか...)

CFLAGS += -Werror
	# Warning が発生した場合、エラー扱いにしてコンパイル中止。


##	CFLAGS += -fomit-frame-pointer
	# 模倣風では、ここで指定しない。 フレームポインタはプロファイラが使えなくなるので、後で設定。


# C/C++言語で式の型に対する最適化の設定 h ttp://www.radiumsoftware.com/0304.html
#	CFLAGS += -fstrict-aliasing
	# このオプションを「指定した場合に」速度向上のためにメモリアクセスの最適化をする。(キャストに注意しないと動かない可能性あり、速い)
	# -O2 -O3 では自動的に適用される。
#	CFLAGS += -fno-strict-aliasing
	# このオプションを「指定しない場合に」速度向上のためにメモリアクセスの最適化をする。(安全側、遅い)
# 何も指定しない場合は -O2 -O3 なら「-fstrict-aliasing」(キャストに注意しないと動かない可能性あり、速い)
##### 以下GCCの説明文
	# コンパイルされている言語に適用可能な別名規則（aliasing rule）のうち
	# 最も厳密なものをコンパイラが前提することを許します。
	# これによって、 C（およびC++）では式の型に基く最適化を動作させることになります。
	# 例えば、ある型のオブジェクトが別の型のオブジェクトと同一アドレスに位置することは、
	# それら2つの型がほとんど同一でない限り、ないものと仮定されます。
	# 例えば、 unsigned intがintの別名となることはあっても、 void*やdoubleの別名となることはありえません。
	# また、文字型は他の任意の型の別名になりえます。 以下のようなコードに特に注意してください.
	# 最後に書き込みが行われた共用体メンバとは異なるメンバから
	# 読み込みを行う習慣（「type-punning」と呼ばれる） は一般的に見られます。
	# `-fstrict-aliasing'を指定した場合でも、メモリが共用体型を通してアクセスされる場合にはtype-punningは許されます。



# 全部調べた訳じゃないけど、どういう場合に Warning扱い にするかの設定。(たぶん) 
	# 模倣風では -Werror なので Warning扱いは、エラー扱いにして即コンパイル中止。
	CFLAGS += -Wno-sign-compare
	CFLAGS += -Wunused
	CFLAGS += -Wpointer-arith
	CFLAGS += -Wundef
	CFLAGS += -Wformat
	CFLAGS += -Wwrite-strings
	CFLAGS += -Wdisabled-optimization
	CFLAGS += -Wbad-function-cast


#	CFLAGS += -ftracer
#	CFLAGS += -fstrength-reduce
#	CFLAGS += -ffast-math

CFLAGS += -ffast-math
	# このオプションを指定すると、実行速度を最適化するという観点から、
	# ある面で ANSI や IEEE の規則や仕様を破ることを GCC に許す。
	# 例えば、このオプションを指定すると、GCC は、sqrt 関数の引数が負にならないとか、
	# 浮動小数点値がNaN になることはないという仮定を行なう。 


#	CFLAGS += -funroll-all-loops
#	CFLAGS += -pipe
#	CFLAGS += -falign-functions
#	CFLAGS += -falign-jumps
#	CFLAGS += -falign-loops
#	CFLAGS += -falign-labels
#	CFLAGS += -freorder-blocks
#	CFLAGS += -fprefetch-loop-arrays

CFLAGS += -I/usr/local/pspdev/psp/include
OPTION_CFLAGS += -I/pspdev/psp/include/SDL


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
###
#CXXFLAGS += $(CFLAGS)
#CXXFLAGS += -fno-exceptions
#CXXFLAGS += -fno-rtti
#CXXFLAGS += -fsingle-precision-constant
#CXXFLAGS += -mno-check-zero-division
#CXXFLAGS += -ffast-math
###
#CXXFLAGS += $(CFLAGS)
#CXXFLAGS += -fno-exceptions
#CXXFLAGS += -fno-rtti
###
#CXXFLAGS += -fno-builtin-printf
###
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

#$(OBJ)/libtremor/libvorbisidec_test.a: $(TREMOR_OBJS)
$(OBJ)/$(LIBTREMOR_DIR)/libvorbisidec_test.a: $(TREMOR_OBJS)
$(OBJ)/vfpu/lib_vfpu.a: $(LIB_PSP_MATH_OBJS)

#---------------------------------------------------------------------
# Rules to manage files
#---------------------------------------------------------------------

#$ make
#Making object tree for kene ...
#psp-gcc -O3 -Werror -Isrc/core/. -I. -I/pspdev/psp/sdk/include -G0 -Wall -g 
#-I/usr/local/pspdev/psp/include -I/pspdev/psp/include/SDL	-c src/core/support.c -o
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
#	で warnings

# CPPゲームコアの場合c++(ライブラリとgnuプロファイラの扱いを変える)
#$(OBJ)/core/%.o: $(SRC)/core/%.cpp
#	psp-gcc -O3  $(CXXFLAGS) $(CORE_CFLAGS) -c $< -o $@

# MP3ライブラリの場合(c++ なのは、 smpeg だけ)
$(OBJ)/%.o: $(SRC)/%.cpp
	psp-gcc -O3 $(CXXFLAGS) $(LIB_CFLAGS) -c $< -o $@

# OGGの場合( alloca(); で warnings )
#$(OBJ)/libtremor/%.o: $(SRC)/libtremor/%.c
$(OBJ)/$(LIBTREMOR_DIR)/%.o: $(SRC)/$(LIBTREMOR_DIR)/%.c
	psp-gcc -O3 $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@

# Cゲームコアの場合(ライブラリとgnuプロファイラの扱いを変える)
$(OBJ)/core/%.o: $(SRC)/core/%.c
	psp-gcc -O3 -Werror $(CFLAGS) $(OPTION_CFLAGS) $(CORE_CFLAGS) -c $< -o $@

# 旧pngの場合
#$(OBJ)/png/%.o: $(SRC)/png/%.c
#	psp-gcc -O2 -Werror $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@

# SDL_mixerの場合
$(OBJ)/SDL_mixer/%.o: $(SRC)/SDL_mixer/%.c
	psp-gcc -O3 -Werror $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@

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

DELTREE_OBJ_ALL 	= @$(RM) -f -rd $(OBJ)
DELTREE_SDL_MIXER	= @$(RM) -f -rd $(OBJ)/SDL_mixer
DELTREE_OBJ_CORE	= @$(RM) -f -rd $(OBJ)/core
DELTREE_OBJ_JIKI	= @$(RM) -f -rd $(OBJ)/core/jiki

# SDL mixer のみ、リコンパイル用。
mixer:
	@echo Remove SDL mixer files.
	@$(DELTREE_SDL_MIXER)

# ゲームコアのみ、リコンパイル用。
core:
	@echo Remove shooting core files.
	@$(DELTREE_OBJ_CORE)

# ゲームコア自機のみ、リコンパイル用。 
jiki:
	@echo Remove shooting core jiki files.
	@$(DELTREE_OBJ_JIKI)

# あたり判定デバッグ機能、リコンパイル用。(game_main.hで設定)
a:
	@echo Remove atari debug files.
	@$(RM) -f $(OBJ)/core/jiki/jiki.o
	@$(RM) -f $(OBJ)/core/jiki/jiki_shot.o
	@$(RM) -f $(OBJ)/core/jiki/jiki_bomber.o
	@$(RM) -f $(OBJ)/core/my_math.o
	@$(RM) -f $(OBJ)/core/sprite_bullet.o
	@$(RM) -f $(OBJ)/core/score_panel.o

#	@$(RM) -f $(OBJ)/core/*.o
#	@$(RM) -f $(OBJ)/core/boss/*.o
#	@$(RM) -f $(OBJ)/core/douchu/*.o
#	@$(RM) -f $(OBJ)/core/draw/*.o
#	@$(RM) -f $(OBJ)/core/menu/*.o
#	@$(RM) -f $(OBJ)/core/tama/*.o

font:
	@echo Remove custom font files.
	@$(RM) -f $(OBJ)/font/*.o

rr:
	@echo Remove all temporaly files.
	@$(RM) -f PARAM.SFO
	@$(RM) -f *.elf
	@$(DELTREE_OBJ_ALL)


#------------------------------------------------------------------------------
# 231copy(テストr31時点ではダミー)
#------------------------------------------------------------------------------
# custom.mak に入れると動作しない為、この位置に必要。
#------------------------------------------------------------------------------

SDL_lib_bunkai:
	cp $(PSPDEV)/psp/lib/libSDL.a $(OBJ)/SDL231/
	ar xv $(OBJ)/SDL231/libSDL.a
	rm -f arch.o
	rm -f SDL_active.o
	rm -f SDL_cdrom.o
	rm -f SDL_cpuinfo.o
	rm -f SDL_cursor.o
	rm -f SDL_events.o
	rm -f SDL_expose.o
	rm -f SDL_gamma.o
	rm -f SDL_getenv.o
	rm -f SDL_joystick.o
	rm -f SDL_keyboard.o
	rm -f SDL_loadso.o
	rm -f SDL_mixer_m68k.o
	rm -f SDL_mixer_MMX.o
	rm -f SDL_mixer_MMX_VC.o
	rm -f SDL_mouse.o
	rm -f SDL_pspevents.o
	rm -f SDL_pspgl.o
	rm -f SDL_pspmouse.o
	rm -f SDL_quit.o
	rm -f SDL_resize.o
	rm -f SDL_stretch.o
	rm -f SDL_syscdrom.o
	rm -f SDL_syscond.o
	rm -f SDL_sysjoystick.o
	rm -f SDL_systimer.o
	rm -f SDL_timer.o
	rm -f SDL_yuv.o
	rm -f SDL_yuv_mmx.o
	rm -f SDL_yuv_sw.o
	rm -f SDL_error.o
	rm -f SDL_fatal.o
	mv *.o $(OBJ)/SDL231/
	mv $(OBJ)/SDL231/SDL_audio.o $(OBJ)/SDL231/audio/
	mv $(OBJ)/SDL231/SDL_audiocvt.o $(OBJ)/SDL231/audio/
	mv $(OBJ)/SDL231/SDL_audiodev.o $(OBJ)/SDL231/audio/
	mv $(OBJ)/SDL231/SDL_audiomem.o $(OBJ)/SDL231/audio/
	mv $(OBJ)/SDL231/SDL_mixer.o $(OBJ)/SDL231/audio/
	mv $(OBJ)/SDL231/SDL_pspaudio.o $(OBJ)/SDL231/audio/
	mv $(OBJ)/SDL231/SDL_wave.o $(OBJ)/SDL231/audio/
	mv $(OBJ)/SDL231/SDL_sysmutex.o $(OBJ)/SDL231/thread/
	mv $(OBJ)/SDL231/SDL_syssem.o $(OBJ)/SDL231/thread/
	mv $(OBJ)/SDL231/SDL_systhread.o $(OBJ)/SDL231/thread/
	mv $(OBJ)/SDL231/SDL_thread.o $(OBJ)/SDL231/thread/
	mv $(OBJ)/SDL231/SDL_blit.o $(OBJ)/SDL231/video/
	mv $(OBJ)/SDL231/SDL_blit_0.o $(OBJ)/SDL231/video/
	mv $(OBJ)/SDL231/SDL_blit_1.o $(OBJ)/SDL231/video/
	mv $(OBJ)/SDL231/SDL_blit_A.o $(OBJ)/SDL231/video/
	mv $(OBJ)/SDL231/SDL_blit_N.o $(OBJ)/SDL231/video/
	mv $(OBJ)/SDL231/SDL_bmp.o $(OBJ)/SDL231/video/
	mv $(OBJ)/SDL231/SDL_pixels.o $(OBJ)/SDL231/video/
	mv $(OBJ)/SDL231/SDL_RLEaccel.o $(OBJ)/SDL231/video/
	mv $(OBJ)/SDL231/SDL_surface.o $(OBJ)/SDL231/video/
	mv $(OBJ)/SDL231/SDL_pspvideo.o $(OBJ)/SDL231/video/
	mv $(OBJ)/SDL231/SDL_video.o $(OBJ)/SDL231/video/
