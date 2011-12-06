
#------------------------------------------------------------------------------
#  東方模倣風 ～ Toho Imitation Style.	Included Makefile for game core
#------------------------------------------------------------------------------
# game core version (r38)

#--- 基本関数 関連。
OBJS += $(OBJ)/game_core/my_math.o

#--- tama 関連。
OBJS += $(OBJ)/game_core/tama/hatudan_system.o
OBJS += $(OBJ)/game_core/tama/laser_system.o
OBJS += $(OBJ)/game_core/tama/obj_system.o
OBJS += $(OBJ)/game_core/tama/spell_system.o

#--- spell/card/boss 関連。
OBJS += $(OBJ)/game_core/boss/boss_card.o
OBJS += $(OBJ)/game_core/boss/boss_support.o
#; ボス(本体)
OBJS += $(OBJ)/game_core/boss/boss_all.o
OBJS += $(OBJ)/game_core/boss/boss_move.o
#; ボスのオプション
OBJS += $(OBJ)/game_core/boss/boss_tukaima.o
OBJS += $(OBJ)/game_core/boss/option_41_aya_momiji02.o
OBJS += $(OBJ)/game_core/boss/option_40_aya_momiji01.o
#
OBJS += $(OBJ)/game_core/boss/option_10_alice_zako_doll_type_b.o
OBJS += $(OBJ)/game_core/boss/option_11_alice_zako_doll_type_a.o

#--- zako 関連。
#; 中ボスもどき
#; ザコのオプション
OBJS += $(OBJ)/game_core/douchu/option_00_homing.o
#; 道中の敵(道中のボス/道中の中ボス/道中のザコ)
OBJS += $(OBJ)/game_core/douchu/teki.o

#; 敵以外だが敵扱い
#OBJS += $(OBJ)/game_core/douchu/kanji_text.o
OBJS += $(OBJ)/game_core/douchu/game_clear.o
OBJS += $(OBJ)/game_core/douchu/bakuhatsu.o

#--- jiki 関連。
OBJS += $(OBJ)/game_core/jiki/jiki.o
OBJS += $(OBJ)/game_core/jiki/jiki_option.o
OBJS += $(OBJ)/game_core/jiki/jiki_shot_bomber.o

#--- etc/core 関連。
OBJS += $(OBJ)/game_core/bg.o
OBJS += $(OBJ)/game_core/jiki/bonus_item.o

OBJS += $(OBJ)/game_core/game_core.o
#--- audio 関連。
OBJS += $(OBJ)/game_core/game_audio.o

#--- draw 関連。
OBJS += $(OBJ)/game_core/draw/score_panel.o
OBJS += $(OBJ)/game_core/draw/PSPL_font.o
OBJS += $(OBJ)/game_core/draw/draw_kanji.o
OBJS += $(OBJ)/game_core/draw/graphics00.o 
OBJS += $(OBJ)/game_core/draw/psp_load_texture.o
OBJS += $(OBJ)/game_core/draw/psp_draw_screen.o

#--- menu 関連。
OBJS += $(OBJ)/font/shinonome16p.o

OBJS += $(OBJ)/game_core/menu/fatal_error.o
OBJS += $(OBJ)/game_core/menu/title.o
OBJS += $(OBJ)/game_core/menu/select_stage.o
OBJS += $(OBJ)/game_core/menu/select_jiki.o

OBJS += $(OBJ)/game_core/menu/pause_menu.o
OBJS += $(OBJ)/game_core/menu/game_over.o
OBJS += $(OBJ)/game_core/menu/name_entry.o

OBJS += $(OBJ)/game_core/menu/option_menu.o
OBJS += $(OBJ)/game_core/menu/key_config.o
OBJS += $(OBJ)/game_core/menu/music_room.o
#/* 会話システムは分離予定あり */
OBJS += $(OBJ)/game_core/menu/kaiwa.o

#--- file 関連。
OBJS += $(OBJ)/game_core/my_file.o
OBJS += $(OBJ)/game_core/load_stage.o

#--- boot/initialize 関連。
OBJS += $(OBJ)/game_core/boot_main.o
OBJS += $(OBJ)/game_core/initialize.o

#--- ディレクトリ作成。
OBJDIRS += $(OBJ)/game_core
OBJDIRS += $(OBJ)/game_core/jiki
OBJDIRS += $(OBJ)/game_core/douchu
OBJDIRS += $(OBJ)/game_core/boss
OBJDIRS += $(OBJ)/game_core/tama
OBJDIRS += $(OBJ)/game_core/draw
OBJDIRS += $(OBJ)/game_core/menu

#--- ソースリストからの読みこみインクルード優先パス設定。
#INCDIR += $(SRC)/game_core/.
INCDIR += $(SRC)/game_core/include


#--- 廃止。

#OBJS += $(OBJ)/game_core/tama/bullet_vector.o
#OBJS += $(OBJ)/game_core/tama/layer_system.o # (coreに統合)
#OBJS += $(OBJ)/game_core/boss/option_62_sakuya_ryoute.o	#(スペカfestival_knifeへ移行)
#OBJS += $(OBJ)/game_core/boss/option_64_sakuya_kurukuru.o	#(スペカfestival_knifeへ移行)
#OBJS += $(OBJ)/game_core/boss/option_63_sakuya_linear.o
#OBJS += $(OBJ)/game_core/boss/option_65_sakuya_360knife.o
#OBJS += $(OBJ)/game_core/boss/option_66_sakuya_star.o		#(スペカへ移行)
#OBJS += $(OBJ)/game_core/boss/option_67_sakuya_laser.o
#OBJS += $(OBJ)/game_core/boss/option_68_sakuya_tama_oki.o	#(スペカへ移行)
#OBJS += $(OBJ)/game_core/boss/option_68_sakuya_hosigata.o
#OBJS += $(OBJ)/game_core/boss/option_50_pache_laser.o	#(未使用)
#OBJS += $(OBJ)/game_core/boss/option_51_pache_doll.o
#OBJS += $(OBJ)/game_core/boss/option_60_sakuya_baramaki1_2.o	#使い魔boss_tukaima.cに融合
#OBJS += $(OBJ)/game_core/boss/option_20_mima_kaguya.o			#使い魔boss_tukaima.cに融合
#OBJS += $(OBJ)/game_core/douchu/chuu_boss.o#zako.cに融合
#OBJS += $(OBJ)/game_core/douchu/chuu_03_rumia.o#統合
#OBJS += $(OBJ)/game_core/douchu/chuu_02_youkai2.o#統合
#OBJS += $(OBJ)/game_core/douchu/chuu_01_youkai1.o#統合

#OBJS += $(OBJ)/game_core/jiki/jiki_bomber.o#jiki_shot_bomber.cに融合

#OBJS += $(OBJ)/game_core/menu/pause.o#統合
#OBJS += $(OBJ)/game_core/menu/ask_continue.o#統合

#OBJS += $(OBJ)/game_core/douchu/clouds.o#一旦廃止。作り直した方が速い。
#OBJS += $(OBJ)/game_core/particle.o#一旦廃止。作り直した方が速い。
#OBJS += $(OBJ)/game_core/fps.o#廃止。
#OBJS += $(OBJ)/game_core/tiny_strcmp.o#ライブラリへ移動。
#OBJS += $(OBJ)/libpspmath.a#ライブラリへ移動。

#$ make
#Making object tree for kene ...
#psp-gcc $(CFLAGS_OPTIMIZE) -Werror -Isrc/core/. -I. -I/pspdev/psp/sdk/include -G0 -Wall -g 
#-I/usr/local/pspdev/psp/include -I/pspdev/psp/include/SDL	-c src/core/support.c -o
#obj/core/support.o
#cc1: warnings being treated as errors
#src/core/support.c: In function 'blit_scaled':
#src/core/support.c(573) : warning: control may reach end of non-void function 'getpixel' being inlined
#make: *** [obj/core/support.o] Error 1
# ???  警告: 制御が非 void 関数の終りに到達しました 
#$(OBJ)/core/support.o: $(SRC)/core/support.c
#	psp-gcc $(CFLAGS_OPTIMIZE)	 $(CXXFLAGS) -c $< -o $@
#	//else
#	//{
#		return (0);
#	//}
