
#------------------------------------------------------------------------------
#  �����͕핗 �` Toho Imitation Style.  Included Makefile for custom library 
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# module music �֘A
#------------------------------------------------------------------------------


### mikmod
ifneq ($(LINK_MOD),1)
	##not include(���̍s�͎g��Ȃ��ꍇ)
else
	# �g�p���郂�W���[���`����I���B
	# �͕핗�ł� IT�`��(Impulse Tracker�`��) �����g���ĂȂ��B

	# "*.mod"�`�����g�p����ꍇ�ɐݒ�(�͕핗�ł͎g�p���Ă��Ȃ�)
#	CFLAGS += -DLINK_MOD
	# "*.s3m"�`�����g�p����ꍇ�ɐݒ�(�͕핗�ł͎g�p���Ă��Ȃ�)
#	CFLAGS += -DLINK_S3M
	# "*.xm"�`�����g�p����ꍇ�ɐݒ�(�͕핗�ł͎g�p���Ă��Ȃ�)
#	CFLAGS += -DLINK_XM
	# "*.it"�`�����g�p����ꍇ�ɐݒ�(�͕핗�Ŏg�p���Ă���)
	CFLAGS += -DLINK_IT

	# "*.mod"/"*.s3m"/"*.xm"/"*.it"�`���̂����ꂩ���g�p����ꍇ�ɐݒ�(�͕핗��"*.it"�`���̂ݎg�p���Ă���)
	CFLAGS += -DMOD_MUSIC

	# ��\�I�� MOD �`���͔��˂̌Â����Ɉȉ��� 4�`���B����ȊO�̂��̂͂��邪�A�قڐ�ł��Ă���̂ŁA�T�|�[�g����Ӗ��������B
	# MOD == music Module �`���BAmiga �̉����� 4ch PCM�������B���̌`���B�y�т�����g��(8ch PCM�Ƃ�)�����`���B
	# S3M == Scream Tracker �`���B(MOD�ɉe�����󂯂����A���˂�Amiga�Ɋ֌W�Ȃ�PC�̃f���V�[��)
	# XM == Extend Tracker �`���B ( ���˂�Amiga�Ɋ֌W�Ȃ�PC�̃f���V�[���BMOD �`���R�}���h�̏�ʌ݊���������B���f�[�^�[��ʌ݊��͖����B)
	# IT == Impulse Tracker �`���B( ���˂�Amiga�Ɋ֌W�Ȃ�PC�̃f���V�[���BS3M �`���R�}���h�̏�ʌ݊���������B���f�[�^�[��ʌ݊��͖����B)

	# ��\�I�� 2 �`���B
	# Amiga PCM���ӎ������`���B(�����APCM�ɓ��������R�}���h���g����BPCM�̓r���Đ��Ƃ����[�v�Ƃ�)
	# MOD �`�� -> ��ʌ݊��ŉ��� -> XM �`���B
	# midi���ӎ������`���B(�����Amidi �`������ϊ����\�B�R�}���h��PCM�ɓ������Ă��Ȃ��BPCM�̓r���Đ��Ƃ��͏o���Ȃ�)
	# S3M �`�� -> ��ʌ݊��ŉ��� -> IT �`���B

	# midi �`������ϊ�: �ϊ����x�͌`����̓����̈Ⴂ������̂Ō����I�ɖ���������Bmidi �͎w�莞�Ԍ`���Ȃ̂ɑ΂��ĕ\���Ԍ`���Ȃ̂ŁB

	# PC�̃f���V�[��: 80486 �Ƃ���͂�PC���g���āAPCM�炵�Ȃ���|���S���Ń����_�����O������
	# �����Əd������������B�������t���t���[���B
	# ...�̃v���O�������쐬���A�R���e�X�g�`���ŋ����B����Ƀo�C�i���T�C�Y�̐������������肷��B(intro 32kiro bytes�Ƃ�)

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
