
#------------------------------------------------------------------------------
#  �����͕핗 �` Toho Imitation Style.  Makefile
#------------------------------------------------------------------------------

# (r33)����m�F�ς�psp
# 1.00    (psp-1000�ŏ�����FW)
# 3.60 M33(psp-2000�ŏ�����CFW)
# ��L��psp�œ���m�F���s���܂����B
# ����ȊO�͑��������񂶂�Ȃ����Ǝv���A�m�F���Ă��܂���B

# ���̃t�@�C���� '�B' �� '.' ���s���ɂ���̂̓R�����g�ł��B

#------------------------------------------------------------------------------
# revision setting. �����͕핗�̃o�[�W�����▼�O�����߂܂��B
#------------------------------------------------------------------------------

TARGET = mohoufu

#RELESE_DATE = 2011/12/04(r39)�~
#RELESE_DATE = 2011/09/04(r38)�H
RELESE_DATE = 2011/12/04

VERSION_MAJOR =39
VERSION_MINOR =1

# psp �� XMB �ݒ�
# 0 == �^�C�g���̕��������AXMB�w�i����B�����[�X��(r34�Ƃ�)�B
# 1 == �^�C�g���̕�������AXMB�w�i�����B�A�b�v�f�[�g��(r34u1�Ƃ�)�B
#USE_EBOOT_TITLE = 0
USE_EBOOT_TITLE = 1

# ����: �^�C�g��������t����Ɣw�i���t�����܂���B

#------------------------------------------------------------------------------
# developping tools setting. �����͕핗�̃R���p�C���������߂܂��B
#------------------------------------------------------------------------------

# 0 == cygwin ���g���B(�ʏ�J��)�B
# 1 == pspsdk-setup-0.11.1.exe ���g���B( Minimalist PSP homebrew SDK for Windows. version 0.11.1 )�B
# 1 == pspsdk-setup-0.11.2r2.exe(2011-05-18) ���g���B
#// �܂������� 1 == pspsdk-setup-0.11.2r3.exe(2011-06-01) ���g���B
USE_MINIMALIST_PSP_SDK = 1
#USE_MINIMALIST_PSP_SDK = 0


# r34���� Minimalist PSP homebrew SDK for Windows.(�ȉ�minimalist) �֑Ή����܂����B
# minimalist �� make.exe �͐���\���̎������ɑΉ����Ă��Ȃ��̂ŁA
# Makefile �Ő���\���̎������͂ł��܂���B(cygwin �� make �͐���\���̎������ɑΉ����Ă��܂��B)
# r33�܂ł� Makefile �͎����������Ă���̂ŁAr33�܂ł� Makefile �� minimalist �ŃR���p�C������ꍇ��
# ��������S�ďC�����Ă��������B(���������C�����Ȃ��Ɛ���ɃR���p�C���ł��܂���)
# r33�܂ł̃R���p�C��������ꍇ�ŁAGCC�̎d�l�Ⴂ���ɂ��G���[���o��ꍇ�́A
# r33u2��r33u0�̃\�[�X����ɏC�����Ă��������B(r33��r33u0���r����)
# �͕핗�� Makefile �͐���\���̓���q�𑽗p���Ă���ׁA Makefile ���������ɂ��������m��܂���B
# r33�܂ł͕s�K�v�ł��Ȃ�ׂ� Makefile �Ɏc���Ă������j(�W��SDL�Ή�)�ł������A
# r34�����(�W��SDL�̓A���T�|�[�g�ɂ���)�Ȃ�ׂ��s�K�v�ȋ@�\�͍폜������j�ɐ؂�ւ��܂����B

# ����(231)�̊��ł� start.bat(�Y�t) �Ŋ��ϐ��ݒ肵�Ă邯�ǁA���̊��͒m��Ȃ��B

#------------------------------------------------------------------------------
# PSP setting.
#------------------------------------------------------------------------------

# psp-2000�Ή��B
PSP_LARGE_MEMORY = 1

# 3.71�ȍ~(�܂�psp-2000�g��������)�Ή����Ă���(�炵��)�B
PSP_FW_VERSION = 371

# ����: �����ł� fw1.00 �ł͋N���ł��܂���B(BUILD_PRX = 1������)
#PSP_SYOMEI_OFW = 0
PSP_SYOMEI_OFW = 1

# ����: (r38)Minimalist�̏ꍇ�A�����ł� MusicRoom �ɓ���ƃn���O�A�b�v���܂��B�����̓������s���Ȃ̂ł����A
# ���ԓ��Ɍ�����ǋ�������܂���ł����B
# (r38)cygwin�̏ꍇ�A�����ł� �������s���ɂȂ�Ȃ��̂ŁA��肠��܂���B MusicRoom �ɓ����Ă��n���O�A�b�v���܂���B

#------------------------------------------------------------------------------
# OFW setting.
#------------------------------------------------------------------------------
# �Q�l http://pspbannoukaizou.blog50.fc2.com/blog-entry-157.html
# 800200D9 �������u���b�N�����蓖�Ă��܂���ł����B  Failure to allocate memory block.
# 800200D9 = Failure to allocate memory block (ex. missing PSP)
# 80020148 �T�|�[�g����Ă��Ȃ�PRX���g�p���Ă��܂��B
# ���J�o���[���[�h���uAdvanced configration�v���uExecute boot.bin in UMD/ISO�v��Disabled�ɂ��ĉ������B 
# 80020148 = Unsupported PRX type | This error also seems to appear when you are running an iso with a fake mem. stick

# prx->eboot   // �N���Ɏ��s���܂���(800200D9) (PRX�����ǃ������s��? )
# eboot->eboot // �N���Ɏ��s���܂���(80020148) (PRX�łȂ��̂Ŗ���?)

# ENCRYPT �� BUILD_PRX �� 2���� �̓y�A�B�Â� *.elf ������ƃ��R���p�C�����s����̂Œ��ӁB
ifneq ($(PSP_SYOMEI_OFW),1)
	# 0 == �񏐖��ŁAfw1.00�Ή���
	APP_UTF8_SYMEI_STR = $(APP_UTF8_HI_SYOMEI_BAN)
	# prx->eboot   // FW1.00			�N���Ɏ��s���܂���(80020148)
	# fw1.00, prx�͖����Ȃ̂���(???)
	## �Í������Ȃ��B
	#ENCRYPT = 0
	## PRX�Ńr���g���Ȃ��B(EBOOT.PBP�Ńr���g����)
	#BUILD_PRX = 0
	# BUILD_PRX = 1 �̏ꍇ�Afw1.00�ŋN���ł��܂���B
	# src/game_core/bootmain.c�ŗ��p����B
	CORE_CFLAGS += -DAPP_SYOMEI_OFW=0
else
	APP_UTF8_SYMEI_STR = $(APP_UTF8_SYOMEI_BAN)
	# 1 == ������(cfw5.00m33-6 �ł� ok ������)
	# ����FW�ŋN���o����悤�ɏ�����t������B
	## �Í�������B
	ENCRYPT = 1
	# (???)EBOOT.PBP����Ȃ��āA*.prx���r���g����B(ENCRYPT = 1�̏ꍇ�K���K�v???)
	## PRX�Ńr���g����B(EBOOT.PBP�Ńr���g���Ȃ�)
	BUILD_PRX = 1
	# src/game_core/bootmain.c�ŗ��p����B
	CORE_CFLAGS += -DAPP_SYOMEI_OFW=1
endif
#BUILD_PRX = 1


#------------------------------------------------------------------------------
# Compile option flag. (��Ƀf�o�b�O�p)
#------------------------------------------------------------------------------

# 1 == GNU PROFILER ���g��(game core)
#USE_PROFILE = 1
USE_PROFILE = 0

# 1 == GNU PROFILER ���g��(���C�u����)(USE_PROFILE = 1���K�v)
#USE_LIB_PROFILE = 1
USE_LIB_PROFILE = 0

# 1 == FPU��O�𖳌��ɂ���
USE_FPU_HACK = 1
#USE_FPU_HACK = 0

#------------------------------------------------------------------------------
# Debug information. (�f�o�b�O�p�A������ς��Ă� EBOOT.PBP �͓���)
#------------------------------------------------------------------------------
# �f�o�b�O�p�B�O���[�o���ϐ��̔z�u�m�F�Ȃǂ������ꍇ�B
#------------------------------------------------------------------------------

# 0 == �f�o�b�O�p�̃V���{���z�u�����o�͂��Ȃ��B
# �u���C�u�����̋�̓I�Ȉˑ��֌W(���ۂ̔z�u)�v ��
# �u�O���[�o���ϐ��̔z�u�v�A
# �u�f�[�^�[�Z�N�V�����̎��ۂ�.align����(�u*fill*�v���Ă̂�GCC��.align�����z)�v 
# �����m�肽���Ȃ����ꍇ�̃f�o�b�O���B
#DEBUG_MAP = 0
DEBUG_MAP = 1

#------------------------------------------------------------------------------
# Compile option flag.
#------------------------------------------------------------------------------
# �͕핗��pSDL���C�u����: (2011-02-16)���݂�psp��SDL�͐F�X�o�O������܂��B(���ɕ`��Ɉ��SDL�����g��Ȃ��Ă�SDL���̂Ƀo�O������B)
# psp�̕W��SDL�ł́A�\�t�g�E�F�A�[�T�[�t�F�C�X�𖾎��I�Ɏw�肵�Ă��A�n�[�h�E�F�A�[�T�[�t�F�C�X���g����ꍇ������̂ł����A
# psp�̃n�[�h�E�F�A�[��ςȕ��Ɏg���Ă���ׁA�ςȏ��ŏ����������܂��B���̌��͕W��SDL���g��������s�\�ł��B
# PSPL�ł́ASDL�̃n�[�h�E�F�A�x��(gu)��S�J�b�g���Ă���܂��B���̕����ςȃ^�C�~���O��GPU���g��Ȃ��̂Ŏ��s���x�������ł��B
# �͕핗�ł� USE_PSPL = 1 �ȊO�T�|�[�g���܂���B
#------------------------------------------------------------------------------

# PSPL == SDL��͕핗�p�ɃJ�X�^�}�C�Y������p���C�u�����B
# (�͕핗�ɕK�v�����@�\���o�b�T���J�b�g�����T�u�Z�b�g�ŁB���������������Ȃ����{��SDL�Ɠ��삪�Ⴄ�B)
# ���O�Ƃ��萔�Ƃ���SDL�Ȃ񂿂��Ȃ̂ŁASDL�֘A�͓����ɂ͈�؎g�p�ł��Ȃ��B
# 1 == USE_PSPL (�͕핗�pSDL(PSPL)���g��)
# 0 == USE_PSPL (�W��SDL��0,�A���T�|�[�g)
USE_PSPL = 1
#USE_PSPL = 0

#------------------------------------------------------------------------------
# Custom Lib option flag. (�J�X�^�}�C�Y)
#------------------------------------------------------------------------------
# ����: �����̃J�e�S���̃I�v�V������ύX�����ꍇ�́Aaudio_mixer �̃��R���p�C�����K���K�v�ł��B
# (�ݒ�ύX��Amake rr�����Ă���make������)
#------------------------------------------------------------------------------
# FAQ. Q:�u audio_mixer �v���ĉ���H
# �u audio_mixer �v�� SDL_mixer.a �̉����łł��B
# audio_mixer�͒P�̂Ŏg�p�ł��܂���B(SDL.a���v��)���R�́A
# 1. SDL�̕W���t�@�C�����o�͋@�\���g�p���Ă���B
# 2. SDL�̔r�ː���@�\(�Z�}�t�H)���g�p���Ă���B(.IT��VERCH�����buffering���Ă�̂ŃZ�}�t�H�͎��ۊ֌W�Ȃ����Aogg�͍Đ��ō���B)
# �ł��B(SDL�̃^�C�}�[�͎g�p���Ă��Ȃ�)
# mp3 �ɑΉ������邽�߂ɂ́u madlib.a �v���ʓr�v��܂��B(madlib.a�Ɋւ��Ă�psp�̏ꍇ�Aversion�Ⴂ�������݂����Ȃ̂œY�t���Ă܂���)
#------------------------------------------------------------------------------
#  �u audio_mixer �v�̉������͎�ɁA
# 1. mod(mikmod)��version���Â��̂ŁA�ŐV��(�ł��O���I)�ɂ��Ă��ł�.IT�`���̓ǂݍ��݂������������B
# 2. mod�֘A�� mixing level �̐ݒ肪 ogg �� mp3 �Ɣ�ׂď���������̂ŏC���B(�A���A���p�I�Ȕ͈͂ł͉����ꂵ�Ȃ���������̉\��������)
# 3. mod�֘A�̓����� ���z�~�L�T�[ (VERCH==Virtual-Mixer)�̕��ׂ���������̂ő��������B(64bit -> 32bit�ɂ��ăt�B���^�[���C��)
# 4. �K�v�Ȃ��G�t�F�N�^�@�\(�f�W�^�� ���o�[�u)��r�����đ��������B
#    (���̃G�t�F�N�^�A�ʔ����񂾂���psp�ɂ͕��ׂ����������B�e������Ȃ����1ch(�X�e���I�Ȃ̂ł܂�2ch)���炢�͎��p�I�ȑ��x�o��)
#    (��낤�Ǝv���� ending-staff roll �Ƃ� ���ׂ����Ȃ����Ŏg���Ȃ����Ȃ��B)
# 5. ���ʉ��`���l���� 8ch�����Ȃ��̂� 16ch�ɂ����B(������128ch���炢�܂łȂ瑝�₵�Ă�(���݃t�B���^�[&�G�t�F�N�g�����̂�)���ׂȂ񂩖w��ǖ����Ǝv����)
# 6. ���̑��t�]�Đ��Ƃ����[�v�Đ��Ƃ��K�v�Ȃ��@�\�̓o�b�T���J�b�g�B(���đ��������B)
# 7. ��(�Y�ꂽ)
#------------------------------------------------------------------------------

# ������
# 0 == CUS TOM_LIB
#CUS TOM_LIB = 1
#CUS TOM_LIB = 0

# MP3�̐ݒ�
# ����: MP3�̎g�p�ۂ�ύX�����ꍇ�́Aaudio_mixer �̃��R���p�C�����K���K�v�ł��B(�ݒ�ύX��Amake rr�����Ă���make������)

## ����: LINK_SMPEG_MP3 �� LINK_MAD_MP3 �𗼕� 1 �ɂ��鎖�͏o���܂���B
# ok 	LINK_SMPEG_MP3 = 0	LINK_MAD_MP3 = 0	MP3 �Ƃ��g��Ȃ��B
# ok 	LINK_SMPEG_MP3 = 1	LINK_MAD_MP3 = 0	smpeg �� MP3 (�`r30) ���g���B(�s���肩�A�x���A���A�Ԃ��Ԃ��m�C�Y)
# ok 	LINK_SMPEG_MP3 = 0	LINK_MAD_MP3 = 1	libmad �� mp3 (r31�`)���g���B(����x�͍����A�x������...�A45fps���炢�ɒቺ����B)
# NG 	LINK_SMPEG_MP3 = 1	LINK_MAD_MP3 = 1	�\�[�X���x���őΉ����Ȃ��̂Ŗ����ANG�B

## smpeg �� MP3 (�`r30)
# 0 == LINK_SMPEG_MP3:�W���ݒ� [ smpeg �� MP3�̎g�p�� ] (0:�g�p���Ȃ��A1:�g�p����)
LINK_SMPEG_MP3 = 0
#LINK_SMPEG_MP3 = 1
# smpeg�́A�܂��o�O(��1)����܂��B
# ��1:���ݔ������Ă���o�O:
# MPEGaudio:: MPEGaudio()����(�N���X�쐬����) SDL_OpenAudio() �ĂԂ̂́A SDLmixer�̌��ݎd�l�ł͐F�X�}�Y�C�B

## libmad �� mp3 (r31�`)
# 1 == LINK_MAD_MP3:�W���ݒ� [ libmad �� MP3�̎g�p�� ] (0:�g�p���Ȃ��A1:�g�p����)
#LINK_MAD_MP3 = 0
LINK_MAD_MP3 = 1
# madlib�͒��g�m��Ȃ����ǁA SDLmixer �ƕ������Ă邩��Amadlib���Ƀo�O�Ȃ���Α������v�B

# r31:�F�X����Ă݂����ǁA�ǂ����Ă� smpeg ���s����(�ˑR��������)�Ȃ̂ŁA smpeg �͎~�߂āA libmad �ɂ��܂����B
# (�������ɗ����Ƀo�O�������Ƒz�肷���)���Ԃ�o�b�t�@�I�[�o�[�����Ȃ񂶂�Ȃ����ȁH

# 1 == LINK_OGG:�W���ݒ� [ ogg�̎g�p�� ] (0:�g�p���Ȃ��A1:�g�p����)
# ����: ogg�̎g�p�ۂ�ύX�����ꍇ�́Aaudio_mixer �̃��R���p�C�����K���K�v�ł��B
LINK_OGG = 1
#LINK_OGG = 0

# 0 == LINK_LIBTREMOR_LOW_MEM: ogg���g�p����ꍇ�A�ǂ���� libtremor ���g�����I�����܂��B
# 0: libtremor_large  ���g�p����B
# 1: libtremor_lowmem ���g�p����B
# 0 == LINK_LIBTREMOR_LOW_MEM:�W���ݒ� [ libtremor_lowmem�̎g�p�� ] (0:�g�p���Ȃ��A1:�g�p����)
# ����: libtremor_lowmem �̑I����ύX�����ꍇ�́Aaudio_mixer �̃��R���p�C�����K���K�v�ł��B
LINK_LIBTREMOR_LOW_MEM = 1
#LINK_LIBTREMOR_LOW_MEM = 0


# "MOD" �Ƃ����̂́A"Module Music"�`���̎��B
# ���̌`���̃f�[�^�[�ɂ̓T���v�����O�����Ɗy���������Ă��āA���A���^�C�������ŉ��y��炷�B
# "Module Music"�`���ɂ́A"*.mod" "*.s3m" "*.xm" "*.it"�̑�\�I��4�`��������B
# ���̂����͕핗�ł� IT�`��(Impulse Tracker�`��) ���g�p���Ă���B

# LINK_MOD = 0 �͎������ĂȂ����画��Ȃ��B
LINK_MOD = 1

#------------------------------------------------------------------------------
# Configurate Rule.
#------------------------------------------------------------------------------

SRC = src
OBJ = obj

VERSION_ALL = r$(VERSION_MAJOR)u$(VERSION_MINOR)

#CORE_CFLAGS += -DAPP_NAME_STR="KENE"
CORE_CFLAGS += -DAPP_RELEASE_VERSION=$(VERSION_MAJOR)
CORE_CFLAGS += -DAPP_UPDATE_VERSION=$(VERSION_MINOR)

include ./$(SRC)/UTF8_title.mak

#PSP_EBOOT_TITLE = kene_r34_debug
#PSP_EBOOT_TITLE = kene$(RELESE_DATE)(r35)
#PSP_EBOOT_TITLE = kene$(RELESE_DATE)(r34u0)
PSP_EBOOT_TITLE = $(APP_UTF8_TITLE)$(RELESE_DATE)$(APP_UTF8_SYMEI_STR)$(VERSION_ALL)

ifneq ($(USE_EBOOT_TITLE),1)
	# 0 == �����[�X��(�^�C�g���̕�������)
	PSP_EBOOT_ICON	 = ICON0.PNG
	PSP_EBOOT_ICON1  = ICON0.PMF
	PSP_EBOOT_UNKPNG = ICON1.PNG
	PSP_EBOOT_PIC1	 = PIC1.PNG
	PSP_EBOOT_SND0	 = SND0.AT3
else
	# PSP_EBOOT_UNKPNG �ɉ摜���w�肷��ƁA�^�C�g���̕������o�܂���B
	# PSP_EBOOT_PIC1 �ɉ摜���w�肷��ƁA�^�C�g���̕������o�܂���B
	# ICON1.PNG �͂Ƃ肠�������́A�߂�ǂ������B
	# 1 == �A�b�v�f�[�g��(�^�C�g���̕�������)
	PSP_EBOOT_ICON	 = ICON0.PNG
#test	PSP_EBOOT_ICON1  = ICON0.PMF
	PSP_EBOOT_UNKPNG = 
	PSP_EBOOT_PIC1	 = 
#test	PSP_EBOOT_SND0	 = SND0.AT3
endif

# �ʏ�J��
EXTRA_TARGETS		 = mk_dir EBOOT.PBP


ifneq ($(USE_MINIMALIST_PSP_SDK),1)
# �ʏ�J��(cygwin)
PSPDEV = $(shell psp-config --pspdev-path)
PSPSDK = $(shell psp-config --pspsdk-path)
else
# pspsdk-setup-0.11.1
PSPDEV = C:/pspsdk
PSPSDK = C:/pspsdk/psp/sdk
endif

#------------------------------------------------------------------------------
# �J���������B
#------------------------------------------------------------------------------

# �{���ǂ̊��ł��Ή����锤�̏ꍇ
# (��)�{�� PSPSDK �őz�肳��Ă��鏑�����B
#PSPDEV = $(shell psp-config --pspdev-path)
# (��)�{�� PSPSDK �őz�肳��Ă��鏑�����B
#PSPSDK = $(shell psp-config --pspsdk-path)
# (��)�{�� PSPSDK �őz�肳��Ă��鏑�����B(�A��PSPSDK�̓������΃p�X�d�l���ς��ƃR���p�C���ʂ�Ȃ�)
#PSPBIN = $(PSPDEV)/psp/bin
# (��)���܂�ǂ��������ł͂Ȃ����A$(PSPSDK)�������$(PSPDEV)�������Ă��R���p�C���ʂ�B(�A��PSPSDK�̓������΃p�X�d�l���ς��ƃR���p�C���ʂ�Ȃ�)
#PSPBIN = $(PSPSDK)/../bin

# cygwin(pspdev) �̏ꍇ(C:/cygwin/pspdev/)
##PSPDEV = /pspdev
##PSPSDK = /pspdev/psp/sdk
##PSPBIN = /pspdev/psp/bin
##PSPDEV = /usr/local/pspdev
##PSPSDK = /usr/local/pspdev/psp/sdk
##PSPBIN = /usr/local/pspdev/psp/bin

# cygwin(pspsdk) �̏ꍇ(C:/cygwin/pspsdk/)
##PSPDEV = /pspsdk
##PSPSDK = /pspsdk/psp/sdk
##PSPBIN = /pspsdk/psp/bin
##PSPDEV = /usr/local/pspsdk
##PSPSDK = /usr/local/pspsdk/psp/sdk
##PSPBIN = /usr/local/pspsdk/psp/bin

# minimalist(pspsdk) �̏ꍇ(C:/pspsdk/)
##PSPDEV = C:/pspsdk
##PSPSDK = C:/pspsdk/psp/sdk
##PSPBIN = C:/pspsdk/psp/bin

# (��)�{�� PSPSDK �őz�肳��Ă��鏑�����B
#SDL_CONFIG = $(PSPDEV)/psp/bin/sdl-config
# (��)���܂�ǂ��������ł͂Ȃ��B(�A��PSPSDK�̓������΃p�X�d�l���ς��ƃR���p�C���ʂ�Ȃ��B�A��$(PSPBIN)��ύX��d�l�ɍ��킹��΃R���p�C���ʂ�B)
#SDL_CONFIG = $(PSPBIN)/sdl-config
# (��)���܂�ǂ��������ł͂Ȃ����A$(PSPSDK)�������$(PSPDEV)�������Ă��R���p�C���ʂ�B(�A��PSPSDK�̓������΃p�X�d�l���ς��ƃR���p�C���ʂ�Ȃ�)
#SDL_CONFIG = $(PSPSDK)/../bin/sdl-config

# sdl-config ( $(shell $(SDL_CONFIG) --libs) ) �͎g�p�ł��Ȃ��B���R�͈ȉ��̂Q�_

# �P�Bsdl-config �ɁA-lSDLmain ������B������g���ƁA�V�^psp(psp-2000)�ł͋N���ł��Ȃ��B
# ���̈� libSDLmain.a �� main(); �ȊO�� main(); ������(���Rpsp-2000�ŋN�������邽�߂�)
# ��������g��(C�����main();�֐����P�̂݁A�����J�͌㒅�D��)���A
# ���ӊ֐��̖��O�������Ȃ�o�b�e�B���O����댯������B

# �Q�B���C�u�����Ԃ̈ˑ��֌W�������ł��Ȃ��B
# sdl-config �� SDL �ȊO�̃��C�u�����ƈˑ��֌W����������ꍇ���l�����Ȃ��ׁB

#------------------------------------------------------------------------------
# Library.
#------------------------------------------------------------------------------

# �Q�l http://himitsu.jpn.ph/yomimono/linux/staticlink.html
# �Q�l http://www.hakodate-ct.ac.jp/~tokai/tokai/gtkmm/etc/p1.htm

LIBDIR =
LDFLAGS =


#------------------- for debug.

ifneq ($(DEBUG_MAP),1)
# �ʏ�
else
# �}�b�s���O�o��(�f�o�b�O�p)
LDFLAGS += -Wl,-Map=$(TARGET)_map.txt
endif

#------------------- for debug.

ifneq ($(USE_PROFILE),1)
else
#ifneq ($(USE_LIB_PROFILE),1)
#else
# -lpspprof �� SDL �ƃ����N����ꍇ�A�ʒu�ˑ�������B
LIBS += -lpspprof
#endif
#	CORE_LIBS += -lpspprof
endif

# �ʒu�������ꍇ
#������
#er.o obj/game_core/hiscore.o obj/game_core/fps.o obj/game_core/soundmanager.o obj/game_core/bg.o -lS
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
# �̂悤�ɂȂ胊���N�o���Ȃ��B(PSPSDK�̃��C�u�����͍Ō�ɂ��Ă�����Č����Ă邩��-lpspprof���Ō�ɂ��Ă�̂�...)
#������
#er.o obj/game_core/hiscore.o obj/game_core/fps.o obj/game_core/soundmanager.o obj/game_core/bg.o -lp
#spprof -lSDL_noGL -lSDL_mixer -lvorbisidec -lSDL_image -lpng -lz -ljpeg -lm -L/u
#sr/local/pspdev/psp/lib -lSDLmain -lSDL -lm -L/usr/local/pspdev/psp/sdk/lib -lps
#pdebug -lpspgu -lpspctrl -lpspge -lpspdisplay -lpsphprm -lpspsdk -lpsprtc -lpspa
#udio -lc -lpspuser -lpsputility -lpspkernel -lpspnet_inet -lpsppower -lpspdebug
#-lpspdisplay -lpspge -lpspctrl -lpspsdk -lc -lpspnet -lpspnet_inet -lpspnet_apct
#l -lpspnet_resolver -lpsputility -lpspuser -lpspkernel -o kene.elf
#make: *** [kene.elf] Interrupt
# �����������Ȃ烊���N�o����B(-lpspprof�̈ʒu�݈̂Ⴄ)

# -lpspprof (�Ⴆ�� C:/cygwin/pspdev/psp/sdk/lib/libpspprof.a) �Ɍ��炸�A
#�ustubs out of order.�v���o��ꍇ�́A
# SDL �ƃ����N����ꍇ�A�ʒu�ˑ��̂��߃����N�o���Ȃ��B
# Makefile�̃��C�u�����̈ʒu���������ΕK�������N�ł��邪�A
#�u$(shell $(SDL_CONFIG) --libs)�v�̂����ŉ����ł��Ȃ��ꍇ�͌��\�D���B


#LIBS_org += -lstdc++		C++�͎g��Ȃ��B
#LIBS_org += -lSDL_mixer	�g���B
#LIBS_org += -lvorbisidec	�g���B
#LIBS_org += -lSDL_image	�g���B
#LIBS_org += -lpng			�g���B
#LIBS_org += -lz			�g���B
#LIBS_org += -ljpeg 		�g���B
#LIBS_org += -lSDL_gfx
#LIBS_org += -lm			�g���B
#LIBS_org += $(shell $(SDL_CONFIG) --libs)
#LIBS_org += -lpsppower 	�g���B


#------------------- ogg codec.
ifneq ($(LINK_OGG),1)
##not include	## ogg���g��Ȃ��ꍇ�B
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
#ifneq ($(CUS TOM_LIB),1)
#	# CUS TOM_LIB �ȊO�̏ꍇ
#	LIBS += -lvorbisidec
#else
#LIBS += $(OBJ)/libtremor/lib_mohou_vorbisidec.a
LIBS += $(OBJ)/$(LIBTREMOR_DIR)/lib_mohou_vorbisidec.a
#endif
endif

#------------------- �ʒu�ˑ������N�B

#ifneq ($(CUS TOM_LIB),1)
#	# CUS TOM_LIB �ȊO�̏ꍇ
#	LIBS += -lSDL_image 	�A���T�|�[�g
#	LIBS += -lpng			�A���T�|�[�g
#	LIBS += -ljpeg			�A���T�|�[�g
#	#�W��
#	#LIBS += -lpspmath		�A���T�|�[�g
#else
#�J�X�^��
#LIBS += $(OBJ)/libpspmath/libpspmath.a
#endif
LIBS += $(OBJ)/vfpu/lib_mohou_vfpu.a

LIBS += -lz

#LIBS += -lm


#-------------------
# libc �֘A�B
#-------------------

# (gnu�W����) libc ���g��Ȃ��ŁA(PSPSDK���p�ӂ���psp�p��) libpsplibc ���g���B
#USE_PSPSDK_LIBC = 1
#�Ƃ肠�����R���p�C���ł��Ȃ��������肻���ȋC������̂Ŏ~�߂Ƃ��B(-lc�Ɠ����Ƀ����N���Ă��)

# ���[�񐳒�������newlib��libpsplibc���������N���Ă�̂͗ǂ��Ȃ��̂ł���SDL�̓s���ŁB(��� signal(); )
# (r32)�Ƃ肠����newlib�݂̂ŁB(libpsplibc�g��Ȃ�)
LIBS += -lc

# smpeg �� C++ �ł����Alibstdc++.a �͎g���܂���B
#LIBS += -lstdc++

#---------------------------

### SDL�֘A�͐�(�ʒu�ˑ�������B)

#---------------------------

#LIBS += $(shell $(SDL_CONFIG) --libs)
# -lSDLmain ������ƐF�X�������Ȏ��ɂȂ�B -lSDLmain �̓����N���Ȃ��B

#---------------------------

### PSPSDK�֘A�͌�(�ʒu�ˑ�������B)

LIBS += -lpsppower
#LIBS += -lpspgum
#LIBS += -lpspgu
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
#bgu_tiny.a obj/libtremor_lowmem/lib_mohou_vorbisidec.a obj/vfpu/lib_mohou_vfpu.a
# -lz -lc -lpsppower -lpsphprm -lpspaudio -lpsprtc -lmad
# -lpspdebug -lpspdisplay
# -lpspge -lpspctrl -lpspsdk -lc -lpspnet -lpspnet_inet -lpspnet_apctl
# -lpspnet_resolver -lpsputility -lpspuser -lpspkernel -o kene.elf


#------------------------------------------------------------------------------
# Object Directory.
#------------------------------------------------------------------------------

OBJDIRS += $(OBJ)
OBJDIRS += $(OBJ)/font

# add after the others, in custom.mak

#------------------------------------------------------------------------------
# Object.
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Include makefiles.
#------------------------------------------------------------------------------
# �͕핗�̃o�[�W�����Ⴂ�ɂ�鍷�ق͂�����ŋz������B
#------------------------------------------------------------------------------

# Makefile �{�́B(�͕핗�{�̋y�ѐ�p���C�u����)
include $(SRC)/src.mak


#------------------------------------------------------------------------------
# CFLAGS.
#------------------------------------------------------------------------------

##	CFLAGS += -O2
	# �͕핗�ł́A�����Ŏw�肵�Ȃ��B(���̃I�v�V�����͍œK���̐ݒ�)

CFLAGS_OPTIMIZE += -O3
#???	�����Ă���
# �Q�l 2011-02-07 �L���X�V http://nekoyama2gillien.blog36.fc2.com/blog-entry-417.html
# CFLAGS_OPTIMIZE += -O2 -march=allegrex -mips2 -mabi=eabi -mgp32 -mlong32
# ���[��ǂ��킩��Ȃ��ȁ`�B


#------------------------------------------------------------------------------
# �Q�l http://www.sra.co.jp/wingnut/gcc/gcc-j.html
# GNU CC(GCC) �̃o�[�W���� 2.95 �ɑΉ����Ă���B(���{��Ńh���t�g-17 July 2001) 
# MIPS �n�R���s���[�^�p�I�v�V����
# �ȉ��� -m �I�v�V�������A��`����Ă���B 

# ============================================= cpu�̎w�� 
# -mcpu=<cpu type>  // psp�pGCC-4.3.5���_�ł͂��̃I�v�V�����͑��݂��Ȃ��B
# ���: cpu���w�肷��B �ŋ߂�GCC(4.x.x�n)�ł� -mcpu �͔p�~����āA-march �ɂȂ����B
# ���: psp�pGCC-4.3.5���_�ł� -mcpu �̓T�|�[�g����Ȃ��B
# -mcpu=<cpu type>
# ���߃X�P�W���[�����O�̍ۂ̋@��̃f�t�H���g�� <cpu type> �Ƒz�肷��B
# <cpu type> �́Ar2000�Ar3000�Ar3900�Ar4000�Ar4100�Ar4300�Ar4400�Ar4600�Ar4650�Ar5000�Ar6000�Ar8000�Aorion �̂ǂꂩ�ł���B
# ����ɁAr2000�Ar3000�Ar4000�Ar5000�Ar6000 �́Ar2k(���邢�� r2K)�Ar3k ���̂悤�ɏȗ��ł���B

#------------
# http://www7.atwiki.jp/pspprogram/pages/12.html
# CPU(Allegrex)
# MIPS 32bit�R�A R4000 
# ���g���F1�`333MHz 
# ���������_�v�Z�\�́F2.6Gflops(333MHz�쓮��) 
# VFPU�L��
# �֗��Ȓ萔���n�[�h�R�[�h�ς�

# PSP�ɍ̗p����Ă���CPU�́AMIPS�Ђ�R4000��32bit�J�X�^��CPU�uAllegrex�v�B
# �Ή��N���b�N��1�`333MHz�B�p���[�Z�[�r���O�̂��߂̊g�����߂������AFPU��VFPU����������Ă���B
CPUTYPE = allegrex
#CFLAGS_OPTIMIZE += -march=allegrex
CFLAGS_OPTIMIZE += -march=$(CPUTYPE)
# http://nekoyama2gillien.blog36.fc2.com/blog-date-200911.html
# -march=<cpu type>
#	���߃X�P�W���[�����O�̍ۂ̋@��̃f�t�H���g�� <cpu type> �Ƒz�肷��B
#	<cpu type> �́A
#	mips1, mips2, mips3, mips4, mips5, 
#	mips32, mips32r2, mips64, mips64r2, 
#	r2000, r3000, r3900, r4000, r4010, r4400, r4600, r4650,
#	r6000, r8000, r10000, r12000, orion, allegrex, 
#	vr4100, vr4111, vr4120, vr4130, vr4181, vr4300, vr5000, vr5400, vr5500, 
#	rm5200, rm5230, rm5231, rm5261, rm5721, rm7000, rm9000, 
#	4kc, 4km, 4kp, 5kc, 20kc, sb1, from-abi
#	�̂ǂꂩ�ł���B
# ���: �A�[�L�e�N�`���[(cpu�̐݌v)���w�肷��B
# ���: psp�ł� -march=allegrex �炵���B

#------------

# �������� <cpu type> ��I�ԂƂ��̓���̃`�b�v�ɓK�����X�P�W���[�����O���s�Ȃ�����ŁA
# -mipsX �� -mabi ���w�肵�Ȃ�����AMIPS ISA(Instruction Set Architecture�A���߃Z�b�g�A�[�L�e�N�`��)��
# ���x�� 1 �ɍ���Ȃ��R�[�h�͉����������Ȃ��B 

# ============================================= MIPS�^�C�v�̎w�� 
# ------ MIPS I/II/III/IV
# -mips1  // (psp�ł͎w��ł��Ȃ��B)# error: -mips1 conflicts with the other architecture options, which specify a MIPS2 processor
# -mips2  // (psp�ł͎w��ł���B  )# allegrex �A�[�L�e�N�`���[�� MIPS II �x�[�X�Ȃ̂��ȁH�B(MIPS II������r4000�n������������?)
# -mips3  // (psp�ł͎w��ł��Ȃ��B)# error: -mips3 conflicts with the other architecture options, which specify a MIPS2 processor
# -mips4  // (psp�ł͎w��ł��Ȃ��B)# error: -mips4 conflicts with the other architecture options, which specify a MIPS2 processor
#-mips1 
#	MIPS ISA �̃��x�� 1 �̖��߂𔭍s����B���ꂪ�f�t�H���g�ł���B
#	r3000 ���A���� ISA ���x���̃f�t�H���g�� CPU �^�C�v�ł���B 
#-mips2 
#	MIPS ISA �̃��x�� 2 �̖���(branch likely, square root ����)�𔭍s����B
#	r6000 ���A���� ISA ���x���̃f�t�H���g�� CPU �^�C�v�ł���B 
#-mips3 
#	MIPS ISA �̃��x�� 3 �̖���(64�r�b�g����)�𔭍s����B
#	r4000 ���A���� ISA ���x���̃f�t�H���g�� CPU �^�C�v�ł���B 
#-mips4 
#	MIPS ISA �̃��x�� 4 �̖���(�����t�� move ���߁A�v���t�F�b�`���߁A�������ꂽ FPU ����)�𔭍s����B
#	r8000 �́A���� ISA ���x���̃f�t�H���g�� cpu type �ł���B 
# CFLAGS_OPTIMIZE += -mips2
# ���: psp�ł� -mips2 �����w��ł��Ȃ��ɂ�������炸�A�f�t�H���g�Ƃ�����ł͂Ȃ��A
#	�w�肵���ꍇ�Əȗ������ꍇ�̈���������Ȃ̂Œ��ӁB
#	psp�� allegrex �� MIPS II �x�[�X�����A�ꕔ MIPS III �̖��߂����B
#	������ -mips2 ���w�肵�Ă���炪GCC�łǂ����������ɂȂ�̂�������Ɣ���Ȃ��B

#------------ CPU����FPU���W�X�^�̃r�b�g���ݒ�B(VFPU�͂܂�Ŋ֌W�����̂Œ���)
# ���: PC�̃R�v���Z�b�T(���������_�`��)�ōg�����Ŏg���Ă�����̂� 32bit, 64bit, 96bit(80bit����), 128bit�B
# 32bit �͂�����(float)�B
# 64bit �͂�����(double)�B
# 96bit(80bit?) ��(tri double �ƌĂ΂��`���炵��)�B
# 128bit �͂�����(long double)�B
# �g�����̃Q�[���G���W�����ł́A�����炭32bit, 64bit�����g���ĂȂ��Ǝv���邪�A
# Direct X �� VisualC++�̃����^�C�� ���X�^�e�B�b�N�����N���Ă���̂ŁA�������  96bit(80bit����), 128bit���g�p���Ă���B
# Direct X (�X�^�e�B�b�N�����N����Ă���)�� SSE �� MMX ���g�p���Ă���B

# ���: -mfp32 �́APC�ł���(float)���g���B
# ���: -mfp64 �́APC�ł���(double)���g���B������psp�ł͐ݒ�ł��Ȃ��Bpsp�� double �Ə����� float���x�B
#-mfp32 
#	32��32�r�b�g���������_���W�X�^�����p�\�ł��邱�Ƃ����肷��B����̓f�t�H���g�ł���B 
#-mfp64  // (psp�ł͎w��ł��Ȃ��B) // error: unsupported combination: -mfp64 -msingle-float
#	32��64�r�b�g���������_���W�X�^�����p�\�ł��邱�Ƃ����肷��B����́A-mips3 �I�v�V�������w�肳�ꂽ�Ƃ��̃f�t�H���g�ł���B 
CFLAGS_OPTIMIZE += -mfp32

#------------ CPU���̔ėp���W�X�^�ݒ�B# 1���W�X�^������̃r�b�g��
#-mgp32 
#	32��32�r�b�g�ėp���W�X�^�����p�\�ł��邱�Ƃ����肷��B����̓f�t�H���g�ł���B 
#-mgp64  // (psp�ł͎w��ł��Ȃ��B) error: '-mgp64' used with a 32-bit processor.
#	32��64�r�b�g�ėp���W�X�^�����p�\�ł��邱�Ƃ����肷��B����́A-mips3 �I�v�V�������w�肳�ꂽ�Ƃ��̃f�t�H���g�ł���B 
CFLAGS_OPTIMIZE += -mgp32

#------------ long�^�̑傫�����w�肷��B
#-mlong32 
#	long �^�Aint �^�A�|�C���^�^�������I�� 32 �r�b�g���ɂ���B 
#	-mlong32�A-mlong64�A-mint64 �̂ǂ���w�肳��Ă��Ȃ��ƁAint�Along�A�|�C���^�̃T�C�Y�́AABI �ƑI�����ꂽ ISA �Ɉˑ�����B
#	-mabi=64 �̏ꍇ�Aint �� 32 �r�b�g���� long �� 64 �r�b�g���ł���B
#	-mabi=eabi �̏ꍇ�A-mips1 �� -mips2 ���w�肳��Ă���ƁAint �� long �� 32 �r�b�g���ɂȂ�B
#	-mabi=eabi �̏ꍇ�A�����荂�� ISA ���w�肳��Ă���ƁAint �� 32 �r�b�g���ŁAlong �� 64 �r�b�g���ɂȂ�B
#	�|�C���^�^�̕��́Along �̕��Ɣėp���W�X�^�̕��̏������ق��ɂȂ�(����́AISA �Ɉˑ�����)�B 
#-mlong64 
#	long �^�������I�� 64 �r�b�g�ɂ���B�f�t�H���g�ɂ��Ă̐����ƃ|�C���^�̑傫���ɂ��Ă� -mlong32 ���Q�Ƃ̂��ƁB 
#-mint64 
#	long �^�� int �^�������I�� 64 �r�b�g���ɂ���B�f�t�H���g�ɂ��Ă̐����ƃ|�C���^�̑傫���ɂ��Ă� -mlong32 ���Q�Ƃ̂��ƁB 
# ���: psp�ł� -mlong32 ����long �� 64bit��K�v�Ƃ���A�v�����R���p�C���ł��Ȃ��BTimidity�Ƃ��� -mlong64 �łȂ��ƃR���p�C���ł��Ȃ��Ǝv���B
# ���: long long �͂��̕ӂ̃I�v�V�����ɉe�����ꂸpsp�ł͋��炭 64 �r�b�g�ɂȂ��Ċ֌W�Ȃ��Ǝv���邯�ǁA����Ȃ��B
CFLAGS_OPTIMIZE += -mlong32

#-mabi=32 
#-mabi=o64  // (psp�ł͎w��ł��Ȃ��B���Ԃ�)
#-mabi=n32  // (psp�ł͎w��ł��Ȃ��B���Ԃ�)
#-mabi=64 
#-mabi=eabi 
#	�w�肳�ꂽ ABI �p�̃R�[�h�𐶐�����B
#	�f�t�H���g�̖��߃��x���́A32 �ɑ΂��Ă� -mips1 �ł���A
#	n32 �ɑ΂��Ă�-mips3�A���̑��ɑ΂��Ă� -mips4 �ł���B
#	�t�ɁA-mips1 �� -mips2 ���w�肷��ƃf�t�H���g�� ABI �� 32 �ɂȂ�A����ȊO�� 64 �ɂȂ�B 
CFLAGS_OPTIMIZE += -mabi=eabi

#-mmips-as 
#	MIPS �A�Z���u���p�̃R�[�h�𐶐����A�ʏ�̃f�o�b�O����ǉ����邽�߂�mips-tfile ���N������B
#	����́AOSF/rose �I�u�W�F�N�g�`�����g���Ă���AOSF/1 �Q�ƃv���b�g�t�H�[���ȊO�̑S�Ẵv���b�g�t�H�[���ł̃f�t�H���g�ł���B
#	-gstabs �� -gstabs+ �̂ǂ��炩�̃I�v�V�������g���ƁAmips-tfile �v���O�������AMIPS ECOFF ���� stabs �`���̃f�o�b�O�����ݍ��ށB 
#-mgas 
#	GNU �A�Z���u���p�̃R�[�h�𐶐�����B����́AOSF/rose �I�u�W�F�N�g�`�����g���Ă���AOSF/1 �̎Q�ƃv���b�g�t�H�[���ł̓f�t�H���g�ł���B
#	�܂��Aconfigure �̃I�v�V���� --with-gnu-as ���w�肳�ꂽ�Ƃ��̃f�t�H���g�ł�����B 

#-msplit-addresses 
#-mno-split-addresses 
#	�A�h���X�萔�̏�ʕ��Ɖ��ʕ���ʁX�Ƀ��[�h����R�[�h�𐶐�����B
#	����ɂ��Agcc ���A�A�h���X�̏�ʃr�b�g�𖳑ʂɃ��[�h���Ȃ��悤�ɍœK�����邱�Ƃ��\�ɂȂ�B
#	���̍œK���ɂ́AGNU as �� GNU ld ���K�v�ɂȂ�B���̍œK���́AGNU as �� GNU ld ���W���̃c�[���ł������̑g�ݍ��݃^�[�Q�b�g�ł�
#	�f�t�H���g�ŗL���ɂȂ�B 
#-mrnames 
#-mno-rnames 
#	-mrnames ���w�肷��ƁA���W�X�^���Ƃ��āA�n�[�h�E�F�A���̑���ɁAMIPS �̃\�t�g�E�F�A��(�Ⴆ�΁A$4 �̑���� a0)���g�����R�[�h���o�͂���B
#	���̃I�v�V�������T�|�[�g���Ă���A�Z���u����Algorithmics �̃A�Z���u�������ł���B 
#-mgpopt 
#-mno-gpopt 
#	-mgpopt �I�v�V�������w�肷��ƁA�S�Ẵf�[�^�錾���A�e�L�X�g�Z�N�V�����̖��ߕ����̑O�ɏ����o���B
#	����ɂ�� MIPS �̃A�Z���u�����������O���[�o������ѐÓI�f�[�^���ڂɑ΂��A���ł͂Ȃ��A���ɂ�郁�����Q�Ƃ𐶐����邱�Ƃ��\�ɂȂ�B
#	�œK�����w�肳��Ă���ꍇ�ɂ́A����̓f�t�H���g�ŗL���ɂȂ�B 
#-mstats 
#-mno-stats 
#	-mstats �I�v�V�������w�肷��ƁA��C�����C���֐������������x�ɁA�W���G���[�o�͂Ɉ�s�o�͂��A
#	�v���O�����̓��v���(�Z�[�u���ꂽ���W�X�^���A�X�^�b�N�̑傫����)��\������B 
#-mmemcpy 
#-mno-memcpy 
#	-mmemcpy �I�v�V�������w�肷��ƁA�S�Ẵu���b�N�ړ��ɑ΂��A�C�����C���R�[�h�𐶐��������ɁA
#	�K�؂ȕ�����֐�(memcpy ��bcopy)���Ăяo���悤�ɂ���B 
#-mmips-tfile 
#-mno-mips-tfile 
#	-mno-mips-tfile ���w�肷��ƁAMIPS �̃A�Z���u�����f�o�b�O����ǉ�������̃I�u�W�F�N�g�t�@�C���ɑ΂��A
#	mips-tfile �v���O�����ɂ��㏈�����s�Ȃ�Ȃ��B
#	mips-tfile �����s���Ă����Ȃ��ƁA�f�o�b�K���烍�[�J���ϐ��̏�񂪈����Ȃ��Ȃ�B
#	����ɁAstage2 �� stage3 �̃I�u�W�F�N�g�ɂ́A�A�Z���u���ɓn���ꂽ�ꎞ�t�@�C�������A�I�u�W�F�N�g�t�@�C�����ɖ��ߍ��܂ꂽ�`�œ����Ă���B
#	���̂��߁Astage2 �̃I�u�W�F�N�g�� stage3 �̃I�u�W�F�N�g�������ɂȂ邱�Ƃ͂Ȃ��B
#	-mno-mips-tfile �I�v�V�������w�肷��̂́Amips-tfile �v���O�����Ƀo�O������A�R���p�C�����s���Ȃ��ꍇ�ɂ����ɏp��ł���B 

#-msoft-float 
#	���������_�p���C�u�����Ăяo�����܂ޏo�͂𐶐�����B
#	�x��: �K�v�ƂȂ郉�C�u������ GCC �̈ꕔ�ł͂Ȃ��B
#	���ʂ͂��̋@��̒ʏ�� C �R���p�C���̋@�\���g���邪�A�N���X�R���p�C���̏ꍇ�͂�������̂܂܍s�Ȃ����Ƃ͂ł��Ȃ��B
#	�N���X�R���p�C���̏ꍇ�́A�K�؂ȃ��C�u�����֐��������ŗp�ӂ��Ȃ���΂Ȃ�Ȃ��B 
#-mhard-float 
#	���������_���߂��܂ޏo�͂𐶐�����B����́AGCC �̃\�[�X��ύX���Ȃ���΁A�f�t�H���g�ł���B 

#-mabicalls 
#-mno-abicalls 
#	�ʒu�Ɨ��R�[�h�p�ɁASystem V.4 �p�̈ڐA�Ŏg���Ă���^������.abicalls�A.cpload�A.cprestore �𐶐�����(���邢�͐������Ȃ�)�B 

#-mlong-calls 
#-mno-long-calls 
#	�S�Ă̌ďo���� JALR ���߂ōs���B���̂��߂ɂ́A�ďo���̑O�ɁA�֐��̃A�h���X�����W�X�^�Ƀ��[�h���Ă����K�v������B
#	���̃I�v�V�������g���K�v������̂́A���݂� 512 ���K�o�C�g�̃Z�O�����g�̊O���̊֐����A�|�C���^�o�R�łȂ��Ăяo���ꍇ�ł���B 

#-mhalf-pic 
#-mno-half-pic 
#	�O���Q�Ƃւ̃|�C���^���A�e�L�X�g�Z�N�V�����ɒu���̂ł͂Ȃ��A�f�[�^�Z�N�V�����ɒu���ă��[�h����悤�ɂ���B 

#-membedded-pic 
#-mno-embedded-pic 
#	�������̑g�ݍ��݃V�X�e�������ɓK���� PIC �R�[�h�𐶐�����B�S�Ă̌ďo���� PC ���΃A�h���X���g���čs���A
#	�S�Ẵf�[�^�̃A�h���X��$gp ���W�X�^���g���Ĉ�����B
#	����ɂ́A���̂��߂̍�Ƃ̂قƂ�ǂ��s�� GNU as �� GNU ld ���K�v�ł���B
#	����́A���݁AECOFF ���g���Ă���^�[�Q�b�g�ł������삵�Ă��Ȃ��BELF �ł͓����Ă��Ȃ��B 

#-membedded-data 
#-mno-embedded-data 
#	�\�ł���΁A�ϐ����A�܂��ŏ��ɓǂݏo����p�f�[�^�Z�N�V�����Ɋ��蓖�Ă�B
#	���ɉ\�ł���΁A���f�[�^�Z�N�V�����Ɋ��蓖�Ă�B�����Ȃ���΁A�f�[�^�Z�N�V�����ɒu���B
#	��������ƁA�f�t�H���g���������x���R�[�h�ɂȂ邪�A���s���ɕK�v�� RAM �̗ʂ��������Ȃ�̂ŁA
#	�������̑g�ݍ��݃V�X�e���ɂƂ��Ă͖]�܂������낤�B 

#-msingle-float 
#-mdouble-float 
#	-msingle-float �I�v�V�������w�肷��ƁAr4650 �`�b�v�̂悤�ɁA���������_�R�v���Z�b�T���P���x���Z�����T�|�[�g���Ă��Ȃ��Ƒz�肷��B
#	-mdouble-float �I�v�V�������w�肷��ƁA�{���x���Z���g���悤�ɂȂ�B�����炪�f�t�H���g�ł���B 

#-mmad 
#-mno-mad 
#	r4650 �`�b�v�̏ꍇ�̂悤�ɁAmad�Amadu�Amul ���߂��g�����Ƃ������B 

#-m4650 
#	-msingle-float�A-mmad�A����ɏ��Ȃ��Ƃ����݂̂Ƃ���ł́A-mcpu=r4650 ��L���ɂ���B 

#-mips16 
#-mno-mips16 
#	16�r�b�g���߂�L���ɂ���B 

#-mentry 
#	�^������ entry �� exit ���g���B���̃I�v�V������ -mips16 ���ꏏ�ɂ����g���Ȃ��B 

# ------- endian (psp�� -EL Little Endian.)
#-EL
#	���g���G���f�B�A���E���[�h�̃v���Z�b�T�p�ɃR�[�h���R���p�C������B�K�v�ƂȂ郉�C�u���������݂���Ɖ��肷��B 
#-EB   // (psp�ł͎w��ł��Ȃ��B)
#	�r�b�O�G���f�B�A���E���[�h�̃v���Z�b�T�p�ɃR�[�h���R���p�C������B�K�v�ƂȂ郉�C�u���������݂���Ɖ��肷��B 
# ���: psp��cpu�̓��g���G���f�B�A���Ȃ̂� -EL (Little Endian)�B�f�t�H���g�Ȃ̂ł����Ďw�肷��Ӗ��������B

#-G<number>
#	<number> �o�C�g�ȉ��̑傫���̃O���[�o���ȃf�[�^���ڂƐÓI�ȃf�[�^���ڂ��A
#	���ʂ̃f�[�^�Z�N�V������ bss �Z�N�V�����ł͂Ȃ��A���f�[�^�Z�N�V�����⏬ bss �Z�N�V�����ɒu���B
#	����ɂ��A�A�Z���u�����A�ʏ�� 2 ���߂̃������Q�Ƃ̑���ɁA
#	�O���[�o���|�C���^(gp ���邢�� $28)�Ɋ�Â����ꖽ�߂̃������Q�Ƃ𐶐����邱�Ƃ��\�ɂȂ�B
#	<number> �̃f�t�H���g�l�́AMIPS �̃A�Z���u�����g���ꍇ�ɂ� 8 �ŁAGNU �A�Z���u�����g���ꍇ�ɂ� 0 �ł���B
#	-G<number> �I�v�V�����̓A�Z���u���ƃ����J�ɂ��n�����B
#	�S�Ẵ��W���[���́A�����l�� -G<number> �ŃR���p�C�����Ȃ���΂Ȃ�Ȃ��B 
# ���: psp�ł� GNU�A�Z���u�����g���̂ŏ����l�� -G0 �Ǝv����B
#CFLAGS += -G8

# ------- �A�Z���u���Ƀv���v���Z�b�T���g��Ȃ�
#-nocpp 
#	MIPS �A�Z���u���ɑ΂��A���[�U�̃A�Z���u���\�[�X�t�@�C��(�T�t�B�b�N�X��.s �̂���)���A�Z���u������ꍇ�ɁA
#	�A�Z���u���̃v���v���Z�b�T�����s���Ȃ��悤�Ɏw������B
#	�����̃I�v�V�����́A�}�V���L�q�� TARGET_SWITCHES �}�N���Œ�`����Ă���B
#	�����̃I�v�V�����̃f�t�H���g���A���̃}�N���Œ�`����Ă���̂ŁA�f�t�H���g��ύX���邪�\�ł���B 

# =============================================

#------------------------------------------------------------------------------


#//??? �œK���I�v�V�����Ƃ��� -O2 ���g���ꍇ�́A-Olimit 3000 ���g���K�v������B
#//??? ���̓�̃I�v�V�����͂ǂ�����Aconfigure ���\�z����Makefile �ɂ͎����I�ɒǉ������B
#//??? make �̕ϐ� CC ���㏑������ MIPS �̃R���p�C�����g���ɂ́A-Wf,-XNg1500 -Olimit 3000 ��ǉ�����K�v������B


#------------------------------------------------------------------------------
# �R�[�h�i��
#

## psp�̓A���C�����g���������Ɠ����Ȃ�����A�O�̈אݒ�B
# �֐��� 32[byte]���E�ɃA���C�����g�B
CFLAGS += -falign-functions=32
# ���[�v�̓��͕K���A���C�����g�B
CFLAGS += -falign-loops
# ���x���w����K���A���C�����g�B
CFLAGS += -falign-labels
# �W�����v����K���A���C�����g�B
CFLAGS += -falign-jumps

#------------------------------------------------------------------------------



CFLAGS += -Wall
	# Warning ������΂��ׂďo�́B
	# �͕핗�ł� -Werror �Ȃ̂� Warning�����́A�G���[�����ɂ��đ��R���p�C�����~�B

CFLAGS += -G0
	# -G0 ���w�肷��ƁA(gp==$28)���W�X�^���g�����ԐڃA�h���b�V���O���߂��g��Ȃ�(���Ԃ�)
	# �Q�l http://wfasim.dyndns.org/wordpress/?p=182
# -G<����> �� GP���W�X�^�Ɋւ���I�v�V�����B<����>�͋�̓I�ɂ̓o�C�g���B
# �f�[�^�Z�N�V����(���邢��bss �Z�N�V����)�� GP���W�X�^(==$28) ���g�����A�������Q�Ƃ��\�ɂȂ�B
# ���: GCC�ł́A -G<number> �͂��؂Ẵ��C�u����/�I�u�W�F�N�g�œ��ꂵ�Ȃ��Ɛ��퓮�삵�Ȃ��Ƃ����d�l�炵���B
#	�ɂ�������炸�A -G<number> ���قȂ郉�C�u����/�I�u�W�F�N�g�̓����N�\�ŁA
#	���ꂪ -G<number> ����̐ݒ�ŃR���p�C�����ꂽ�̂����o�̎�i�������B
#	psp�̏ꍇ�A�����ꃉ�C�u������ -G0 �ȊO�ŃR���p�C�����ꂽ���C�u����/�I�u�W�F�N�g����������Ă���ƁA
#	���̃��C�u����/�I�u�W�F�N�g�̕����͐���ɓ��삵�Ȃ��B
#	�ʏ�͈��S���̈� -G0�Ɏw�肵�Ă����������ǂ��B
#	�A���A��؂̊O�����C�u����/�I�u�W�F�N�g���g�p���Ȃ��Ȃ��(���RPSPSDK���g���Ȃ玩�O�R���p�C��) -G8 �Ƃ��͉\�B

# -G �� GP���W�X�^�Ɋւ���I�v�V������ -g �̓f�o�b�O�Ɋւ���I�v�V�����B�啶���Ə������őS�R�Ӗ����Ⴄ�B
#???	CFLAGS += -g
#	CFLAGS += -g
#	-g �̓f�o�b�O�Ɋւ���I�v�V�����B���Ď��͔��邯�Ǐڍׂ͗ǂ�����Ȃ��B(-pg�t������-g�t�����Ɖ��߂����̂��ȁH)
#	C++(smpeg)�g���ꍇ�� -g ���v�邩���m��Ȃ��B���Ă������v��Ǝv���B
#	-g �t����Ƃق�̏������R�[�h���傫���Ȃ�B(������: r33�̏ꍇ�� 36[bytes])
#	r33���� �͕핗��C++�����̂� -g�͗v��Ȃ��B(-lstdc++���v��Ȃ�)

CFLAGS += -std=gnu99
	# 1999�N�� GNU �K�i�ɉ����� C����R�[�h �Ƃ���B(�Öق̏ȗ��Ƃ��Avoid�|�C���^�̈����Ƃ�...)

#�ł΂���CFLAGS += -Werror
	# Warning �����������ꍇ�A�G���[�����ɂ��ăR���p�C�����~�B


##	CFLAGS += -fomit-frame-pointer
	# �͕핗�ł́A�����Ŏw�肵�Ȃ��B �t���[���|�C���^�̓v���t�@�C�����g���Ȃ��Ȃ�̂ŁA��Őݒ�B


# C/C++����Ŏ��̌^�ɑ΂���œK���̐ݒ� h ttp://www.radiumsoftware.com/0304.html
#	CFLAGS += -fstrict-aliasing
	# ���̃I�v�V�������u�w�肵���ꍇ�Ɂv���x����̂��߂Ƀ������A�N�Z�X�̍œK��������B(�L���X�g�ɒ��ӂ��Ȃ��Ɠ����Ȃ��\������A����)
	# -O2 $(CFLAGS_OPTIMIZE) �ł͎����I�ɓK�p�����B
#	CFLAGS += -fno-strict-aliasing
	# ���̃I�v�V�������u�w�肵�Ȃ��ꍇ�Ɂv���x����̂��߂Ƀ������A�N�Z�X�̍œK��������B(���S���A�x��)
# �����w�肵�Ȃ��ꍇ�� -O2 $(CFLAGS_OPTIMIZE) �Ȃ�u-fstrict-aliasing�v(�L���X�g�ɒ��ӂ��Ȃ��Ɠ����Ȃ��\������A����)
##### �ȉ�GCC�̐�����
	# �R���p�C������Ă��錾��ɓK�p�\�ȕʖ��K���ialiasing rule�j�̂���
	# �ł������Ȃ��̂��R���p�C�����O�񂷂邱�Ƃ������܂��B
	# ����ɂ���āA C�i�����C++�j�ł͎��̌^�Ɋ�œK���𓮍삳���邱�ƂɂȂ�܂��B
	# �Ⴆ�΁A����^�̃I�u�W�F�N�g���ʂ̌^�̃I�u�W�F�N�g�Ɠ���A�h���X�Ɉʒu���邱�Ƃ́A
	# �����2�̌^���قƂ�Ǔ���łȂ�����A�Ȃ����̂Ɖ��肳��܂��B
	# �Ⴆ�΁A unsigned int��int�̕ʖ��ƂȂ邱�Ƃ͂����Ă��A void*��double�̕ʖ��ƂȂ邱�Ƃ͂��肦�܂���B
	# �܂��A�����^�͑��̔C�ӂ̌^�̕ʖ��ɂȂ肦�܂��B �ȉ��̂悤�ȃR�[�h�ɓ��ɒ��ӂ��Ă�������.
	# �Ō�ɏ������݂��s��ꂽ���p�̃����o�Ƃ͈قȂ郁���o����
	# �ǂݍ��݂��s���K���i�utype-punning�v�ƌĂ΂��j �͈�ʓI�Ɍ����܂��B
	# `-fstrict-aliasing'���w�肵���ꍇ�ł��A�����������p�̌^��ʂ��ăA�N�Z�X�����ꍇ�ɂ�type-punning�͋�����܂��B



# �S�����ׂ��󂶂�Ȃ����ǁA�ǂ������ꍇ�� Warning���� �ɂ��邩�̐ݒ�B(���Ԃ�) 
	# �͕핗�ł� -Werror �Ȃ̂� Warning�����́A�G���[�����ɂ��đ��R���p�C�����~�B


# ������r�̓f�o�b�O���Ȃ̂łƂ肠����off�B(�{���͗ǂ��Ȃ�)�B
# off(������r�Ōx�����Ȃ�):������r�̈���
#CFLAGS += -Wno-sign-compare
# on(������r�Ōx������):������r�̈����B
CFLAGS += -Wsign-compare



CFLAGS += -Wunused
CFLAGS += -Wpointer-arith
CFLAGS += -Wundef
CFLAGS += -Wformat
CFLAGS += -Wwrite-strings
CFLAGS += -Wdisabled-optimization
CFLAGS += -Wbad-function-cast

#CFLAGS += -Wmissing-prototypes # �v���g�^�C�v�錾�������ꍇ�Ɍx��(���G���[)�B

CFLAGS += -ffast-math
	# ���̃I�v�V�������w�肷��ƁA���s���x���œK������Ƃ����ϓ_����A
	# ����ʂ� ANSI �� IEEE �̋K����d�l��j�邱�Ƃ� GCC �ɋ����B
	# �Ⴆ�΁A���̃I�v�V�������w�肷��ƁAGCC �́Asqrt �֐��̈��������ɂȂ�Ȃ��Ƃ��A
	# ���������_�l��NaN �ɂȂ邱�Ƃ͂Ȃ��Ƃ���������s�Ȃ��B 




#	CFLAGS += -pipe
#	CFLAGS += -freorder-blocks
#	CFLAGS += -fprefetch-loop-arrays

#------------------------------------------------------------------------------
# ���[�v�W�J�Ɋւ���œK���I�v�V����
#------------------------------------------------------------------------------
# �����̃I�v�V������t����� psp�ł͒x���Ȃ�B
#------------------------------------------------------------------------------

# ========
# -fstrength-reduce
# ���[�v�̋��x�팸�ƌJ��Ԃ��ϐ��̍폜�̍œK�������s���܂��B
# ���[�v�̊O�ɏo����v�Z�͊O�ɏo���āA�X�e�b�v�������炵�܂��B
# ========
# -frerun-cse-after-loop 
# ���[�v�œK���̎��s��ɋ��ʕ������̍폜��������x���s���܂��B
# -fstrength-reduce �ōs�������Ƃ�O�ꂵ�čs���܂��B
# ========
# -frerun-loop-opt 
# ���[�v�œK����O�ꂵ�čs���܂��B-fstrength-reduce�ōs�������Ƃł��B
# ���[�v���ŕω����Ȃ�����A�h���X�v�Z���`�F�b�N����܂��B
# �����āA���������v�Z�̓��[�v�̊O�Ɉڂ���A���̕]���l�����W�X�^�Ɋi�[����܂��B
# ========
# -funroll-loops
# ���[�v�W�J�œK�������s���܂��B
# ����́A�R���p�C���������s���ɌJ��Ԃ��񐔂����߂��郋�[�v�ɂ����s���܂���B
# -funroll-loops�́A�O�q�� -fstrength-reduce �� -frerun-cse-after-loop ���܂݂܂��B
# ========
# -funroll-all-loops
# ���[�v�W�J�œK�������s���܂��B����́A���ׂẴ��[�v�ɑ΂��čs���A
# ���ʂ̓v���O�����̎��s��x�����Ă��܂��܂��B
# (psp�ł̓��[�v�W�J�͂���΂���قǒx���Ȃ�B)	CFLAGS += -funroll-all-loops
# ========


#------------------------------------------------------------------------------
# cygwin�� Minimalist �͖����Ă��R���p�C���o����(?)�B
#CFLAGS += -I/usr/local/pspdev/psp/include
# (��)
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
#Minimalist �� ����Ɖ��̂��R���p�C���o���Ȃ�(????)�B
#INCDIR += $(PSPDEV)/psp/include
# (��)
#------------------------------------------------------------------------------

#INCDIR += $(PSPSDK)/../include

ifneq ($(USE_PSPL),1)
	# USE_PSPL �ȊO�̏ꍇ
	#OPTION_CFLAGS += -I/pspdev/psp/include/SDL
	INCDIR += $(PSPDEV)/psp/include/SDL
	#INCDIR += $(PSPSDK)/../include/SDL
endif


# http://www.sra.co.jp/wingnut/gcc/gcc-j.html
# -fomit-frame-pointer �t���[���|�C���^��K�v�Ƃ��Ȃ��֐��ɂ��ẮA�t���[���|�C���^�����W�X�^�ɕێ����Ȃ��悤�ɂ���B
# ����ɂ��A�t���[���|�C���^���Z�[�u�A�ݒ�A���X�g�A���閽�߂��Ȃ������Ƃ��ł���B
# �܂��A�����̊֐��ŗ��p�\�ȃ��W�X�^���������B
# �܂��A�@��ɂ���Ă̓f�o�b�O���s�\�ɂȂ�B (psp�ł� -pg ���g���Ȃ��Ȃ�)�B

# (����r36�ُ퓮��̈�off�Bpsp�ł͒x���Ȃ�?) CFLAGS += -ftracer
# (����r36�ُ퓮��̈�off�Bpsp�ł͒x���Ȃ�?) CFLAGS += -fstrength-reduce

# �ǂ���̃I�v�V�����������̂��������Ă��Ȃ����Ar36�ł́A
# ������t����ƁA�L�����I����ʂ����������A�F�X���������B
# �����̃I�v�V�����̓��[�v�W�J������炵���A
# �T�C�Y��������ƃL���b�V���Ɏ��܂肫��Ȃ��Ȃ� psp �ł͑��x�ቺ����B

ifneq ($(USE_PROFILE),1)
#	CORE_CFLAGS += -ftracer
#	CORE_CFLAGS += -fstrength-reduce
	CORE_CFLAGS += -fomit-frame-pointer
else
	CORE_CFLAGS += -DENABLE_PROFILE
	CORE_CFLAGS += -pg
endif

ifneq ($(USE_LIB_PROFILE),1)
#	LIB_CFLAGS += -ftracer
#	LIB_CFLAGS += -fstrength-reduce
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



#------------------------------------------------------------------------------
# build.mak.
#------------------------------------------------------------------------------

include $(PSPSDK)/lib/build.mak

#---------------------------------------------------------------------
# Rules to make libraries.
#---------------------------------------------------------------------
# ���ł��m��Ȃ����ǁA���C�u����(*.a)�ɂ��ẮA
# include ��� *.mak �ɏ����� Makefile ������Ă���Ȃ��̂ŁA�����ɏ����B
#---------------------------------------------------------------------

# build ogg lib.
#$(OBJ)/libtremor/lib_mohou_vorbisidec.a: $(TREMOR_OBJS)
$(OBJ)/$(LIBTREMOR_DIR)/lib_mohou_vorbisidec.a: $(TREMOR_OBJS)

# build vfpu lib.
$(OBJ)/vfpu/lib_mohou_vfpu.a: $(LIB_PSP_MATH_OBJS)

# build gu lib.
$(OBJ)/libgu/libgu_tiny.a: $(LIB_PSP_GU_OBJS)

# build debug lib.
$(OBJ)/debug/debug.a: $(LIB_PSP_DEBUG_OBJS)

#---------------------------------------------------------------------
# Rules to manage files.
#---------------------------------------------------------------------

# malloc free �� warnings.
#$(OBJ)/jpeg/%.o: $(SRC)/jpeg/%.c
#	psp-gcc $(CFLAGS_OPTIMIZE) $(CFLAGS) -c $< -o $@

# malloc free �� warnings.
#$(OBJ)/libpng/%.o: $(SRC)/libpng/%.c
#	psp-gcc $(CFLAGS_OPTIMIZE) $(CFLAGS) -c $< -o $@

# malloc free �� warnings.
#$(OBJ)/SDL_image/IMG_png.o: $(SRC)/SDL_image/IMG_png.c
#	psp-gcc $(CFLAGS_OPTIMIZE) $(CFLAGS) -c $< -o $@


#rc/libtremor/misc.h(209) : warning: implicit declaration of function '_ilog'
#rc/libtremor/misc.h(209) : warning: implicit declaration of function 'abs'
#	�� warnings

# CPP�Q�[���R�A�̏ꍇc++(���C�u������gnu�v���t�@�C���̈�����ς���)�B
#$(OBJ)/game_core/%.o: $(SRC)/game_core/%.cpp
#	psp-gcc $(CFLAGS_OPTIMIZE)  $(CXXFLAGS) $(CORE_CFLAGS) -c $< -o $@

# ��芸���� smpeg (�o�O������܂ŕX��)�A���T�|�[�g���B
# MP3���C�u�����̏ꍇ(c++ �Ȃ̂́A smpeg ����)
#$(OBJ)/%.o: $(SRC)/%.cpp
#	psp-gcc $(CFLAGS_OPTIMIZE) -Werror $(CXXFLAGS) $(LIB_CFLAGS) -c $< -o $@

# OGG�̏ꍇ( alloca(); �� warnings )�B
#$(OBJ)/libtremor/%.o: $(SRC)/libtremor/%.c
$(OBJ)/$(LIBTREMOR_DIR)/%.o: $(SRC)/$(LIBTREMOR_DIR)/%.c
	psp-gcc $(CFLAGS_OPTIMIZE) -Werror $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@

# C�Q�[���R�A�̏ꍇ(���C�u������gnu�v���t�@�C���̈�����ς���)�B
$(OBJ)/game_core/%.o: $(SRC)/game_core/%.c
	psp-gcc $(CFLAGS_OPTIMIZE) -Werror $(CFLAGS) $(OPTION_CFLAGS) $(CORE_CFLAGS) -c $< -o $@

# ��png�̏ꍇ�B
#$(OBJ)/png/%.o: $(SRC)/png/%.c
#	psp-gcc -O2 -Werror $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@

# audio_mixer�̏ꍇ�B
$(OBJ)/PSPL/audio/mixer/%.o: $(SRC)/PSPL/audio/mixer/%.c
	psp-gcc $(CFLAGS_OPTIMIZE) -Werror $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@

# �Ă��Ƃ̃��C�u�����̏ꍇ�B
#$(OBJ)/SDL231/video/%.o: $(SRC)/SDL231/video/%.c
#	psp-gcc $(CFLAGS_OPTIMIZE) $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@

# �G���[���o��̂łƂ肠�����B(???????)�B
#$(OBJ)/PSPL/video/PSPL_pspvideo.o: $(SRC)/PSPL/video/PSPL_pspvideo.c
#	psp-gcc $(CFLAGS_OPTIMIZE) $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@

# �G���[���o��̂łƂ肠�����B
$(OBJ)/PSPL/video/PSPL_bmp.o: $(SRC)/PSPL/video/PSPL_bmp.c
	psp-gcc $(CFLAGS_OPTIMIZE) $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@

# �G���[���o��̂łƂ肠�����B(???????)�B
$(OBJ)/PSPL/video/PSPL_video.o: $(SRC)/PSPL/video/PSPL_video.c
	psp-gcc $(CFLAGS_OPTIMIZE) $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@


# ���̑��̃��C�u�����̏ꍇ�B
$(OBJ)/%.o: $(SRC)/%.c
	psp-gcc $(CFLAGS_OPTIMIZE) -Werror $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@

# �A�Z���u���̏ꍇ�B
$(OBJ)/%.o: $(SRC)/%.S
	psp-gcc $(CFLAGS_OPTIMIZE) -Werror $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@

#	psp-gcc $(CFLAGS_OPTIMIZE) -Werror $(CFLAGS) -c $< -o $@
#	psp-gcc $(CFLAGS_OPTIMIZE) -Werror $(CXXFLAGS) -c $< -o $@

# ���C�u�������A�[�J�C�u(�g�ݗ��Ă�)����ꍇ�B(�A�[�J�C�u==�����k�̏��ɂ��쐬����)
$(OBJ)/%.a:
	@echo Archiving $@...
	@$(AR) -r $@ $^



#cc1: warnings being treated as errors
#src/PSPL/video/PSPL_pspvideo.c: In function 'vidmem_alloc':
#src/PSPL/video/PSPL_pspvideo.c(187) : warning: cast from function call of type 'void *' to non-matching type 'long unsigned int'
#src/PSPL/video/PSPL_pspvideo.c: In function 'PSP_GuStretchBlit':
#src/PSPL/video/PSPL_pspvideo.c(278) : warning: pointer of type 'void *' used in arithmetic
#src/PSPL/video/PSPL_pspvideo.c(285) : warning: pointer of type 'void *' used in arithmetic
#src/PSPL/video/PSPL_pspvideo.c(286) : warning: pointer of type 'void *' used in arithmetic
#src/PSPL/video/PSPL_pspvideo.c(313) : warning: pointer of type 'void *' used in arithmetic
#src/PSPL/video/PSPL_pspvideo.c: In function 'PSP_VideoQuit':
#src/PSPL/video/PSPL_pspvideo.c(615) : warning: pointer of type 'void *' used in arithmetic
#make: *** [obj/PSPL/video/PSPL_pspvideo.o] Error 1


#cc1.exe: warnings being treated as errors
#src/PSPL/video/PSPL_pspvideo.c: In function 'vidmem_alloc':
#src/PSPL/video/PSPL_pspvideo.c:167: error: cast from function call of type 'void *' to non-matching type 'long unsigned int'
#C:\PSPSDK\BIN\MAKE.EXE: *** [obj/PSPL/video/PSPL_pspvideo.o] Error 1


#------------------------------------------------------------------------------
# Utilities.
#------------------------------------------------------------------------------
# cygwin: windows���ŉ��zunix�𓮍삳����c�[���B�x���ƕ]��^^;�B

#(������TAB����Ď������o���Ȃ��̂Œ���)
#ifeq ($(PSPDEV),)
# Minimalist ???
#else
# 0==�ʏ�J��(cygwin)
#endif

ifneq ($(USE_MINIMALIST_PSP_SDK),1)
# �ʏ�J��(cygwin)
# (unix:�f�B���N�g���[ ,msdos: �f�B���N�g���[ ,windows: �t�H���_)[OS�Ŗ��̂��Ⴄ��������]
# make directory (unix:mkdir ,msdos:md) ���C�N-�f�B�A�[�A���C�N-�f�B���N�g���[(�f�B���N�g�����쐬����)
#435�ł�build.mak���ɂ���BMKDIR = mkdir.exe
RM = -rm.exe
# copy    (unix:cp ,msdos: copy) �R�s�[(�t�@�C���𕡐�����)[2�ɑ�����]
#435�ł�build.mak���ɂ���BCP = cp
CP = cp
# re-move (unix:rm ,msdos:del)  �����[�u(�t�@�C��������)
#435�ł�build.mak���ɂ���BRM = rm
# move    (unix:mv ,msdos:move)  ���[�u(�t�@�C�����ړ�����)[�ړ����͍폜�����]
MV = mv
# archiver �A�|�J�C�o(C����Ȃǂ̃R���p�C������.obj�t�@�C�������C�u����.a�t�@�C���ɓZ�߂��蕪�������肷��c�[���B�t�@�C�����k�͈�؂��Ȃ�)
#435�ł�build.mak���ɂ���B(psp-ar)AR = ar
else
# pspsdk-setup-0.11.1
#Minimalist PSP homebrew SDK for Windows.(�ȉ�Minimalist�Ɨ�)
# �������͒m��Ȃ�(���Ԃ�Minimalist�͂�����)
# Minimalist�͎��ے��gcygwin(�̊ȗ���)�œ����Ă邩��A��̈ꏏ�B
# �����Amake.exe �Ƃ� ���x�v��z��cygwin�֌W������native�œ����Ă���ۂ��B
#MKDIR = -mkdir
#RM = -rm
MKDIR = C:/pspsdk/bin/mkdir
#RM = -rm.exe
RM = C:/pspsdk/bin/rm.exe
#MV = mv
endif

# ������ obj �f�B���N�g�����쐬����@�\�B
mk_dir:
	@echo Making directry for $(TARGET) ...
	@$(MKDIR) -p $(subst //,\,$(sort $(OBJDIRS)))
	@$(RM) -f PARAM.SFO

ifneq ($(USE_MINIMALIST_PSP_SDK),1)
# �ʏ�J��(cygwin)
DELTREE_OBJ_ALL 	= @$(RM) -f -rd $(OBJ)
DELTREE_AUDIO_MIXER	= @$(RM) -f -rd $(OBJ)/audio_mixer
DELTREE_OBJ_CORE	= @$(RM) -f -rd $(OBJ)/game_core
DELTREE_OBJ_JIKI	= @$(RM) -f -rd $(OBJ)/game_core/jiki
else
# pspsdk-setup-0.11.1
DELTREE_OBJ_ALL 	= $(RM) -f -r -d $(OBJ)
DELTREE_AUDIO_MIXER	= $(RM) -f -r -d $(OBJ)/audio_mixer
DELTREE_OBJ_CORE	= $(RM) -f -r -d $(OBJ)/game_core
DELTREE_OBJ_JIKI	= $(RM) -f -r -d $(OBJ)/game_core/jiki
#C:\pspsdk\srcr34>rm --help
#Usage: C:\PSPSDK\BIN\RM.EXE [OPTION]... FILE...
#Remove (unlink) the FILE(s).
#
#  -d, --directory       unlink directory, even if non-empty (super-user only)
#  -f, --force           ignore nonexistent files, never prompt
#  -i, --interactive     prompt before any removal
#  -r, -R, --recursive   remove the contents of directories recursively
#  -v, --verbose         explain what is being done
#      --help            display this help and exit
#      --version         output version information and exit
#
# �����ł̓f�B���N�g���������悤�Ƃ���Ƃǂ�Ȏw��ł� No such file or directory �ɂȂ�B
# C:\PSPSDK\BIN\RM.EXE �Ɋւ��ẮA�����ł͐��퓮�삵�Ȃ��וs���B
endif


# audio mixer �̂݁A���R���p�C���p�B
# Make mixer �� (�A��cygwin)
mixer:
	@echo Remove audio mixer files.
	@$(DELTREE_AUDIO_MIXER)

# �Q�[���R�A�̂݁A���R���p�C���p�B
# Make core �� (�A��cygwin)
core:
	@echo Remove shooting core files.
	@$(DELTREE_OBJ_CORE)

# �Q�[���R�A���@�̂݁A���R���p�C���p�B 
# Make jiki �� (�A��cygwin)
jiki:
	@echo Remove shooting core jiki files.
	@$(DELTREE_OBJ_JIKI)

# �����蔻��f�o�b�O�@�\�A���R���p�C���p�B(game_main.h�Őݒ�)
# Make a �� (�A��cygwin)
a:
	@echo Remove atari debug files.
	@$(RM) -f $(OBJ)/game_core/jiki/jiki.o
	@$(RM) -f $(OBJ)/game_core/jiki/jiki_shot.o
	@$(RM) -f $(OBJ)/game_core/jiki/jiki_bomber.o
	@$(RM) -f $(OBJ)/game_core/my_math.o
	@$(RM) -f $(OBJ)/game_core/sprite_bullet.o
	@$(RM) -f $(OBJ)/game_core/score_panel.o

#	@$(RM) -f $(OBJ)/game_core/*.o
#	@$(RM) -f $(OBJ)/game_core/boss/*.o
#	@$(RM) -f $(OBJ)/game_core/douchu/*.o
#	@$(RM) -f $(OBJ)/game_core/draw/*.o
#	@$(RM) -f $(OBJ)/game_core/menu/*.o
#	@$(RM) -f $(OBJ)/game_core/tama/*.o

# syomei ban��minimalist�ł��ꍇ�̃p�b�`�B
patch:
	@echo syomei ban minimalist patch.
	@$(RM) -f $(OBJ)/game_core/boot_main.o
	@$(RM) -f $(TARGET).prx
	@$(RM) -f $(TARGET).elf
	@$(RM) -f PARAM.SFO
	@$(RM) -f EBOOT.PBP


# gu ���Ȃ��������ꍇ�ɁB
# Make gu �� (�A��cygwin)
gu:
	@echo Remove custom font files.
	@$(RM) -f $(OBJ)/game_core/draw/*.o

# font ���Ȃ��������ꍇ�ɁB
# Make font �� (�A��cygwin)
font:
	@echo Remove custom font files.
	@$(RM) -f $(OBJ)/font/*.o


# �Ƃɂ����S�� obj �폜�������ꍇ�ɁB
# Make rr �� (�A��cygwin)  (rr �� Remove all object for Release.�̂���)
rr:
	@echo Remove all temporaly files.
	@$(RM) -f PARAM.SFO
	@$(RM) -f *.elf
	@$(RM) -f $(TARGET)_map.txt
	@$(DELTREE_OBJ_ALL)

