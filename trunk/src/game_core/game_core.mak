
#------------------------------------------------------------------------------
#  �����͕핗 �` Toho Imitation Style.	Included Makefile for game core
#------------------------------------------------------------------------------
# game core version (r35)

#INCDIR += $(SRC)/game_core/.
INCDIR += $(SRC)/game_core/include

#	particle.o

OBJS += $(OBJ)/game_core/my_math.o
#OBJS += $(OBJ)/game_core/tama/bullet_vector.o
OBJS += $(OBJ)/game_core/tama/hatudan_system.o
OBJS += $(OBJ)/game_core/tama/laser_system.o
OBJS += $(OBJ)/game_core/tama/layer_system.o
OBJS += $(OBJ)/game_core/tama/sprite_bullet.o
OBJS += $(OBJ)/game_core/tama/spell_system.o
OBJS += $(OBJ)/game_core/boss/boss_card.o
OBJS += $(OBJ)/game_core/boss/boss_support.o
#; �{�X(�{��)
#OBJS += $(OBJ)/game_core/boss/boss_08_kene.o
#OBJS += $(OBJ)/game_core/boss/boss_07_cirno.o	# �͕핗�W���@�\�ł��ׂđΏ��o����̂ŌŗL�@�\�͂Ȃ��B
OBJS += $(OBJ)/game_core/boss/boss_06_sakuya.o
#OBJS += $(OBJ)/game_core/boss/boss_05_pache.o	# �͕핗�W���@�\�ł��ׂđΏ��o����̂ŌŗL�@�\�͂Ȃ��B
OBJS += $(OBJ)/game_core/boss/boss_04_aya.o
OBJS += $(OBJ)/game_core/boss/boss_03_kaguya.o
OBJS += $(OBJ)/game_core/boss/boss_02_mima.o
#OBJS += $(OBJ)/game_core/boss/boss_01_alice.o	# �͕핗�W���@�\�ł��ׂđΏ��o����̂ŌŗL�@�\�͂Ȃ��B
OBJS += $(OBJ)/game_core/boss/boss_move.o
#; �{�X�̃I�v�V����
OBJS += $(OBJ)/game_core/boss/option_60_sakuya_baramaki1.o
OBJS += $(OBJ)/game_core/boss/option_61_sakuya_baramaki2.o
OBJS += $(OBJ)/game_core/boss/option_62_sakuya_ryoute.o
#OBJS += $(OBJ)/game_core/boss/option_63_sakuya_linear.o
OBJS += $(OBJ)/game_core/boss/option_64_sakuya_kurukuru.o
#OBJS += $(OBJ)/game_core/boss/option_65_sakuya_360knife.o
#OBJS += $(OBJ)/game_core/boss/option_66_sakuya_star.o		#(�X�y�J�ֈڍs)
#OBJS += $(OBJ)/game_core/boss/option_67_sakuya_laser.o
#OBJS += $(OBJ)/game_core/boss/option_68_sakuya_tama_oki.o	#(�X�y�J�ֈڍs)
#OBJS += $(OBJ)/game_core/boss/option_68_sakuya_hosigata.o
#
#OBJS += $(OBJ)/game_core/boss/option_50_pache_laser.o	#(���g�p)
#OBJS += $(OBJ)/game_core/boss/option_51_pache_doll.o
#
OBJS += $(OBJ)/game_core/boss/option_41_aya_momiji02.o
OBJS += $(OBJ)/game_core/boss/option_40_aya_momiji01.o
#
OBJS += $(OBJ)/game_core/boss/option_30_kaguya_dolls.o
#
OBJS += $(OBJ)/game_core/boss/option_20_mima_hosi.o
#
OBJS += $(OBJ)/game_core/boss/option_10_alice_doll_type_b.o
OBJS += $(OBJ)/game_core/boss/option_11_alice_doll.o
#; ���{�X���ǂ�
#OBJS += $(OBJ)/game_core/douchu/chuu_boss.o	#zako.c�ɗZ��
#OBJS += $(OBJ)/game_core/douchu/chuu_03_rumia.o
#OBJS += $(OBJ)/game_core/douchu/chuu_02_youkai2.o
#OBJS += $(OBJ)/game_core/douchu/chuu_01_youkai1.o
#; �U�R�̃I�v�V����
OBJS += $(OBJ)/game_core/douchu/option_00_homing.o
#; �����̓G(�����̃{�X/�����̒��{�X/�����̃U�R)
OBJS += $(OBJ)/game_core/douchu/teki.o

#; �G�ȊO�����G����
OBJS += $(OBJ)/game_core/douchu/kanji_text.o
OBJS += $(OBJ)/game_core/douchu/game_clear.o
#OBJS += $(OBJ)/game_core/douchu/clouds.o#��U�p�~
#(�p�~���R: ����(r34)�̃V�X�e���Ɛ���������Ȃ��B�󂪔���Ȃ����A������ȈׁB��蒼������������)
OBJS += $(OBJ)/game_core/douchu/bakuhatsu.o
#
OBJS += $(OBJ)/game_core/game_core.o

OBJS += $(OBJ)/game_core/jiki/jiki.o
OBJS += $(OBJ)/game_core/jiki/jiki_option.o
OBJS += $(OBJ)/game_core/jiki/jiki_shot.o
OBJS += $(OBJ)/game_core/jiki/jiki_bomber.o

OBJS += $(OBJ)/game_core/jiki/bonus_item.o
#---
OBJS += $(OBJ)/game_core/draw/gu_load_texture.o
OBJS += $(OBJ)/game_core/draw/gu_draw_screen.o
OBJS += $(OBJ)/game_core/draw/score_panel.o


OBJS += $(OBJ)/game_core/bg.o
OBJS += $(OBJ)/game_core/draw/SDL_font.o
#---
OBJS += $(OBJ)/game_core/audio.o
#---
#OBJS += $(OBJ)/game_core/tiny_strcmp.o
OBJS += $(OBJ)/game_core/load_stage.o

#---
OBJS += $(OBJ)/game_core/menu/title.o
#OBJS += $(OBJ)/game_core/menu/pause.o
#OBJS += $(OBJ)/game_core/menu/ask_continue.o
OBJS += $(OBJ)/game_core/menu/select_stage.o
OBJS += $(OBJ)/game_core/menu/select_jiki.o

OBJS += $(OBJ)/game_core/menu/pause_menu.o
OBJS += $(OBJ)/game_core/menu/game_over.o
OBJS += $(OBJ)/game_core/menu/name_entry.o

OBJS += $(OBJ)/game_core/menu/option_menu.o
OBJS += $(OBJ)/game_core/menu/key_config.o
OBJS += $(OBJ)/game_core/menu/music_room.o
#/* ��b�V�X�e���͕����\�肠�� */
OBJS += $(OBJ)/game_core/menu/kaiwa.o
OBJS += $(OBJ)/game_core/draw/draw_kanji.o
OBJS += $(OBJ)/font/shinonome16p.o

OBJS += $(OBJ)/game_core/my_file.o

#OBJS += $(OBJ)/game_core/fps.o

OBJS += $(OBJ)/game_core/draw/graphics00.o 



OBJS += $(OBJ)/game_core/boot_main.o
OBJS += $(OBJ)/game_core/initialize.o

#OBJS += $(OBJ)/libpspmath.a
OBJDIRS += $(OBJ)/game_core
OBJDIRS += $(OBJ)/game_core/jiki
OBJDIRS += $(OBJ)/game_core/douchu
OBJDIRS += $(OBJ)/game_core/boss
OBJDIRS += $(OBJ)/game_core/tama
OBJDIRS += $(OBJ)/game_core/draw
OBJDIRS += $(OBJ)/game_core/menu


#$ make
#Making object tree for kene ...
#psp-gcc $(CFLAGS_OPTIMIZE) -Werror -Isrc/core/. -I. -I/pspdev/psp/sdk/include -G0 -Wall -g 
#-I/usr/local/pspdev/psp/include -I/pspdev/psp/include/SDL	-c src/core/support.c -o
#obj/core/support.o
#cc1: warnings being treated as errors
#src/core/support.c: In function 'blit_scaled':
#src/core/support.c(573) : warning: control may reach end of non-void function 'getpixel' being inlined
#make: *** [obj/core/support.o] Error 1
# ???  �x��: ���䂪�� void �֐��̏I��ɓ��B���܂��� 
#$(OBJ)/core/support.o: $(SRC)/core/support.c
#	psp-gcc $(CFLAGS_OPTIMIZE)	 $(CXXFLAGS) -c $< -o $@
#	//else
#	//{
#		return (0);
#	//}
