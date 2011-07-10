
#------------------------------------------------------------------------------
#  東方模倣風 ～ Toho Imitation Style.  Included Makefile for custom library 
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# module music 関連
#------------------------------------------------------------------------------


### mikmod
ifneq ($(LINK_MOD),1)
	##not include(この行は使わない場合)
else
	# 使用するモジュール形式を選択。
	# 模倣風では IT形式(Impulse Tracker形式) しか使ってない。

	# "*.mod"形式を使用する場合に設定(模倣風では使用していない)
#	CFLAGS += -DLINK_MOD
	# "*.s3m"形式を使用する場合に設定(模倣風では使用していない)
#	CFLAGS += -DLINK_S3M
	# "*.xm"形式を使用する場合に設定(模倣風では使用していない)
#	CFLAGS += -DLINK_XM
	# "*.it"形式を使用する場合に設定(模倣風で使用している)
	CFLAGS += -DLINK_IT

	# "*.mod"/"*.s3m"/"*.xm"/"*.it"形式のいずれかを使用する場合に設定(模倣風で"*.it"形式のみ使用している)
	CFLAGS += -DMOD_MUSIC

	# 代表的な MOD 形式は発祥の古い順に以下の 4形式。それ以外のものはあるが、ほぼ絶滅しているので、サポートする意味が無い。
	# MOD == music Module 形式。Amiga の音源が 4ch PCMだった。その形式。及びそれを拡張(8ch PCMとか)した形式。
	# S3M == Scream Tracker 形式。(MODに影響を受けたが、発祥はAmigaに関係ないPCのデモシーン)
	# XM == Extend Tracker 形式。 ( 発祥はAmigaに関係ないPCのデモシーン。MOD 形式コマンドの上位互換性がある。がデーター上位互換は無い。)
	# IT == Impulse Tracker 形式。( 発祥はAmigaに関係ないPCのデモシーン。S3M 形式コマンドの上位互換性がある。がデーター上位互換は無い。)

	# 代表的な 2 形式。
	# Amiga PCMを意識した形式。(特徴、PCMに特化したコマンドが使える。PCMの途中再生とかループとか)
	# MOD 形式 -> 上位互換で改良 -> XM 形式。
	# midiを意識した形式。(特徴、midi 形式から変換が可能。コマンドはPCMに特化していない。PCMの途中再生とかは出来ない)
	# S3M 形式 -> 上位互換で改良 -> IT 形式。

	# midi 形式から変換: 変換精度は形式上の特性の違いがあるので原理的に無理がある。midi は指定時間形式なのに対して表時間形式なので。

	# PCのデモシーン: 80486 とか非力なPCを使って、PCM鳴らしながらポリゴンでレンダリングしたり
	# もっと重い処理をする。もちろんフルフレーム。
	# ...のプログラムを作成し、コンテスト形式で競う。さらにバイナリサイズの制限があったりする。(intro 32kiro bytesとか)

	##OBJS += $(OBJ)/module_music/npertab.o
	#OBJS += $(OBJ)/module_music/load_mod.o
	#OBJS += $(OBJ)/module_music/load_xm.o
	#OBJS += $(OBJ)/module_music/load_s3m.o
	#OBJS += $(OBJ)/module_music/load_it.o
	OBJS += $(OBJ)/module_music/mloader.o
# loader regist
	OBJS += $(OBJ)/module_music/music_loader.o

#	OBJS += $(OBJ)/module_music/extern_register.o
	OBJS += $(OBJ)/module_music/mdriver.o
# no sound
#	OBJS += $(OBJ)/module_music/drv_nos.o
# SDL
#	OBJS += $(OBJ)/module_music/drv_sdl.o

# virtual mixer2(High Quality)
#	OBJS += $(OBJ)/module_music/virtch2.o
# virtual mixer1(Normal Quality, 32+64bit mixing)
#	OBJS += $(OBJ)/module_music/virtch.o
# virtual mixer1(PSP Quality, true 32bit mixing, not Dolby Surround)
	OBJS += $(OBJ)/module_music/virtch32.o
#	OBJS += $(OBJ)/module_music/virtch_common.o

	OBJS += $(OBJ)/module_music/mmalloc.o
	OBJS += $(OBJ)/module_music/mmerror.o
	OBJS += $(OBJ)/module_music/mmio.o
	OBJS += $(OBJ)/module_music/mlutil.o
	OBJS += $(OBJ)/module_music/munitrk.o

	OBJS += $(OBJ)/module_music/voice_loader.o

	OBJS += $(OBJ)/module_music/mplayer.o
	OBJDIRS += $(OBJ)/module_music
	INCDIR += $(SRC)/module_music
	#CFLAGS += -I$(SRC)/module_music
endif
