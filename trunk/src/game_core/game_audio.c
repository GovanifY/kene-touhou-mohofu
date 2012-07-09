
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�I�[�f�B�I �}�l�[�W���[
---------------------------------------------------------*/

//	#include "./../SDL_mixer/music_mixer.h"/*���̂�������*/
	#include "./../PSPL/audio/mixer/music_mixer.h"/*���̂�������*/

//#ifdef ENABLE_PSP
	//# /* �J�X�^�����C�u�������g�� */
//	#include <SDL/SDL.h>//#include "SDL.h"
//	#include "SDL_image.h"
	//#include <SDL/SDL_mixer.h>//#include "SD L_mixer.h"
	//#include "./../SDL_mixer/SDL_SDL_audio.h"
	//#include "./../SDL_mixer/000SDL_mixer.h"
	/*???*/
//#else
	//# /* �W�����C�u�������g�� */
//	#include <SDL/SDL.h>
//	#include <SDL/SDL_image.h>
	//#include <SDL/SDL_mixer.h>/*#include "SD L_mixer.h"*/
	//#include "./../SDL_mixer/SDL_SDL_audio.h"
	//#include "./../SDL_mixer/000SDL_mixer.h"/*���̂�������*/
	//#include "./../SDL_mixer/music_mixer_external.h"/*���̂�������*/
//	#include "./../SDL_mixer/music_mixer.h"/*���̂�������*/
//#endif

#if (1==USE_DESIGN_TRACK)
	/* ������ �������� �͏��Ȃ��Ȃ邪�A������ƌ��ʉ��݌v(�g���b�N�ݒ�)���Ȃ��Ƃ܂Ƃ��ɖ�Ȃ��B */
	#define USE_VSYNC_SOUND 	(1)
#else
	/* ��ʂ̃A�C�e�����W�����ꍇ�ɏ��������������B */
	#define USE_VSYNC_SOUND 	(0)
#endif

/* --- �Ȃ̊Ǘ��ő吔 (�ő�ǂݍ��݃t�@�C����) */
//#define MAX_MUSIC_FILES 26/*32*/

/* --- ���ʉ��̊Ǘ��ő吔 (�ő�ǂݍ��݃t�@�C����)MAX_VOICE_ALLOC_FILES=>VOICE18_MAX_FILES */
//#define MAX_VOICE_ALLOC_FILES 18/*64*/

/*---------------------------------------------------------

---------------------------------------------------------*/


/* ----- ���ʉ��̐�(�ǂݍ��݃t�@�C����) */
//#define VOICE18_MAX_FILES 15 /*�������ǉ�*/

/* ----- �Ȃ̃g���b�N */
static Mix_Music *music_track;

/* ----- ���ʉ��̃g���b�N */
static Mix_Chunk *voice_track[VOICE18_MAX_FILES];

#define NOT_USE_TRACK (-1)/*���g�p�t���O*/

#define MAX_VOICE_TRACK 8/*���z�g���b�N��*/

#if (1==USE_VSYNC_SOUND)
static int request_voice[MAX_VOICE_TRACK];
#endif

/* ----- �T�E���h�L���t���O */
static int use_audio = 0;/*�g�p�s�\*/


/*---------------------------------------------------------
	�Ȃ̈ꎞ��~
---------------------------------------------------------*/

//global void pause_music(void)
//{
//	if (0==use_audio) return;
//	Mix_PauseMusic();
//}


/*---------------------------------------------------------
	�ꎞ��~�����Ȃ̉��t�ĊJ
---------------------------------------------------------*/

//global void resume_music(void)
//{
//	if (0==use_audio) return;
//	Mix_ResumeMusic();
//}


/*---------------------------------------------------------
	�Ȃ̃t�F�[�h�A�E�g
---------------------------------------------------------*/

//global void fadeout_music(void)
//{
//	if (0==use_audio) return;
//	Mix_FadeOutMusic(1280);
//}


/*---------------------------------------------------------
	����܂ł̋Ȃ̉��t��~(�p�~/����)	play_music_num(BGM_00_stop)���g��
---------------------------------------------------------*/

//global void stop_music(void)
//{
//	//if (0==use_audio) return;
//	if (NULL != music_track)
//	{
//		//if (Mix_PlayingMusic() )
//		{	Mix_HaltMusic();	}
//		Mix_FreeMusic(music_track);
//		music_track = NULL;
//	}
//}


/*---------------------------------------------------------
	�Ȃ̉��t
	-------------------------------------------------------
	(r39)�œ����Ȃ𕡐���w�肵���ꍇ�A�������Ȃ��悤�Ɏd�l�ύX�����B
	-------------------------------------------------------
	0==num	�ł���܂ł̋Ȃ̉��t��~
	1�` 	�ŋȂ̉��t
---------------------------------------------------------*/
#define CUSTOM_MIXER (1)
#if (0==CUSTOM_MIXER)
	#define aaa_Mix_PlayMusic	Mix_PlayMusic
	#define sdl_mixer_play_channel(aaa,bbb) 	Mix_PlayChannelTimed(aaa,bbb,0,-1)
	//#define sdl_mixer_play_channel(aaa,bbb)	Mix_PlayChannel(aaa,bbb,0)
#else
	extern void sdl_mixer_play_music(Mix_Music *music_track);
	#define aaa_Mix_PlayMusic(aaa,bbb)	sdl_mixer_play_music(aaa)
	extern int sdl_mixer_play_channel(int which, Mix_Chunk *chunk);
#endif

global void play_music_num(int next_play_num)
{
	if (0==use_audio) return;
	static int now_play_num = 0;
	/* �͈̓`�F�b�N */
	#if (1)
	if (now_play_num==next_play_num)
	{
		/*(���ɓ��Ȃ��Đ���[��~�R�}���h��2�񂫂��ꍇ�����l])*/
		return;/*(�����Ȃ̍Đ��w�����������ꍇ�A����܂ł̉��t��~���ċȂ��Đ����Ȃ����Ȃ�)*/
	}
	#endif
	#if (1)
	/*(�ԍ����L���b�V�����Ċo���Ă���)*/
	now_play_num = next_play_num;/*(��~�R�}���h�̏ꍇ���L���b�V������)*/
	#endif
	/* ----- ����܂ł̉��t��~ */
	//stop_music();
	//void stop_music(void)
	{
	//	if (0==use_audio) return;
		if (NULL != music_track)
		{
			//if (Mix_PlayingMusic() )
			{	Mix_HaltMusic();	}
			Mix_FreeMusic(music_track);
			music_track = NULL;
		}
	}
	/* ----- �ݒ�ŋȍĐ�OFF�Ȃ�Đ����Ȃ�(���̏ꍇ�͈�x�Ȓ�~����) */
	if (0==option_config[OPTION_CONFIG_02_BGM])
	{
		return;
	}
	/* ----- BGM ���t��~�R�}���h */
	if (0==next_play_num)
	{
		return;/* ----- BGM ���t��~�R�}���h */
	}
	//
	next_play_num--;
	/* ----- BGM ���t�J�n */
	char name[64/*72*/];
	{
		static const char *music_file_name[USE_31_MUSIC_FILES] =
		{
		#if (1)
		/*	0 ----- BGM ���t��~�R�}���h */
		/*	1 */	"th12_02",		//	"stage1",	/* 1�ʓ��� */
		/*	2 */	"th05_06",		//	"stage2",	/* 2�ʓ��� */
		/*	3 */	"th08_13",		//	"stage3",	/* 3�ʓ��� */
		/*	4 */	"th10_08",		//	"stage4",	/* 4�ʓ��� */
		/*	5 */	"th06_08",		//	"stage5",	/* 5�ʓ��� */
		/*	6 */	"th06_10",		//	"stage6",	/* 6�ʓ��� */
		/*	7 */	"th00_01",		//	"stage7",	/* �G���f�B���O(����) */
		/*	8 */	"th06_04",		//	"stage8",	/* �G�L�X�g���X�e�[�W���� */
		/*	9 */	"th07_04",		//	"stage9",	/* �t�@���^�Y���X�e�[�W���� */
		/* 10 */	"th06_06",		//	"menu4",	/* ��C�g���� */
		/* 11 */	"th07_07",		//	"boss1",	/* 1�ʃ{�X (�A���X) */
		/* 12 */	"th02_11",		//	"boss2",	/* 2�ʃ{�X (����) */
		/* 13 */	"th08_15",		//	"boss3",	/* 3�ʃ{�X (�P��) */
		/* 14 */	"th00_11",		//	"boss4",	/* 4�ʃ{�X (��) */
		/* 15 */	"th06_09",		//	"boss5",	/* 5�ʃ{�X (�p�`�F) */
		/* 16 */	"th06_11",		//	"boss6",	/* 6�ʃ{�X (���) */
		/* 17 */	"th06_05",		//	"boss7",	/* �`���m �G�L�X�g���X�e�[�W1�{�X */
		/* 18 */	"th05_17",		//	"boss8",	/* �A���X �G�L�X�g���X�e�[�W2�{�X */
		/* 19 */	"th08_10",		//	"boss9",	/* ������ �G�L�X�g���X�e�[�W3�{�X */
		/* 20 */	"th07_11",		//	"boss10",	/* �d�� �B���{�X */
		/* 21 */	"th06_13",		//	"boss11",	/* ���~���A �B���{�X */
		/* 22 */	"th06_15",		//	"boss0",	/* �t���� �B���{�X */
		/* 23 */	"th04_10",		//	"boss12",	/* ���� �얲 �B���{�X */
		/* 24 */	"th06_03",		//	"boss13",	/* ���[�~�A �B���{�X */
		/* 25 */	"th10_18",		//	"menu3",	/* �G���f�B���O(�V�i���I) */
		/* 26 */	"th07_02",		//	"menu2",	/* �L�[�R���t�B�O */
		/* 27 */	"th07_01",		//	"menu1",	/* �^�C�g����� */
		/* 28 */	"th04_07",		//	"stage10",	/* �����Ȃł��B */
		/* 29 */	"th08_08",		//	"stage11",	/* �����Ȃł��B */
		/* 30 */	"th13_02",		//	"stage12",	/* �����Ȃł��B */
		/* 30 */	"th06_16",		//	"menu1",	/* ���^�C�g����� */
		/* 30 */	"th07_15",		//	"stage13",	/* �����Ȃł��B�d�X��� */
		/* 30 */	"th07_06",		//	"stage14",	/* �����Ȃł��B�u�N���V���e�B�̐l�`�t */
		#else
		/*	0 ----- BGM ���t��~�R�}���h */
		/*[�_]*/
		/* 30 */	"th13_02",		//	"stage12",	/* �����Ȃł��B */
		/*[��]*/
		/*	1 */	"th12_02",		//	"stage1",	/* 1�ʓ��� */
		/*[��]*/
		/* 25 */	"th10_18",		//	"menu3",	/* �G���f�B���O(�V�i���I) */
		/*	4 */	"th10_08",		//	"stage4",	/* 4�ʓ��� */
		/*[�i]*/
		/* 13 */	"th08_15",		//	"boss3",	/* 3�ʃ{�X (�P��) */
		/*	3 */	"th08_13",		//	"stage3",	/* 3�ʓ��� */
		/* 19 */	"th08_10",		//	"boss9",	/* ������ �G�L�X�g���X�e�[�W3�{�X */
		/* 29 */	"th08_08",		//	"stage11",	/* �����Ȃł��B */
		/*[�d]*/
		/* 20 */	"th07_15",		//	"stage13",	/* �����Ȃł��B */
		/* 20 */	"th07_11",		//	"boss10",	/* �d�� �B���{�X */
		/* 11 */	"th07_07",		//	"boss1",	/* 1�ʃ{�X (�A���X) */
		/*	9 */	"th07_04",		//	"stage9",	/* �t�@���^�Y���X�e�[�W���� */
		/* 26 */	"th07_02",		//	"menu2",	/* �L�[�R���t�B�O */
		/* 20 */	"th07_01",		//	"menu1",	/* �^�C�g����� */
		/*[�g]*/
		/* 27 */	"th06_16",		//	"menu1",	/* �^�C�g����� */
		/* 22 */	"th06_15",		//	"boss0",	/* �t���� �B���{�X */
		/* 21 */	"th06_13",		//	"boss11",	/* ���~���A �B���{�X */
		/* 16 */	"th06_11",		//	"boss6",	/* 6�ʃ{�X (���) */
		/*	6 */	"th06_10",		//	"stage6",	/* 6�ʓ��� */
		/* 15 */	"th06_09",		//	"boss5",	/* 5�ʃ{�X (�p�`�F) */
		/*	5 */	"th06_08",		//	"stage5",	/* 5�ʓ��� */
		/* 10 */	"th06_06",		//	"menu4",	/* ��C�g���� */
		/* 17 */	"th06_05",		//	"boss7",	/* �`���m �G�L�X�g���X�e�[�W1�{�X */
		/*	8 */	"th06_04",		//	"stage8",	/* �G�L�X�g���X�e�[�W���� */
		/* 24 */	"th06_03",		//	"boss13",	/* ���[�~�A �B���{�X */
		/*[����]*/
		/* 18 */	"th05_17",		//	"boss8",	/* �A���X �G�L�X�g���X�e�[�W2�{�X */
		/*	2 */	"th05_06",		//	"stage2",	/* 2�ʓ��� */
		/* 23 */	"th04_10",		//	"boss12",	/* ���� �얲 �B���{�X */
		/* 28 */	"th04_07",		//	"stage10",	/* �����Ȃł��B */
		/* 12 */	"th02_11",		//	"boss2",	/* 2�ʃ{�X (����) */
		/*[���̑�]*/
		/* 14 */	"th00_11",		//	"boss4",	/* 4�ʃ{�X (��) */
		/*	7 */	"th00_01",		//	"stage7",	/* �G���f�B���O(����) */
		#endif
		};
		const char *name_extention[] =
		{
			NULL, ".mod", ".s3m", ".xm", ".it", ".mp3", ".wav", ".ogg", 	/* ���� */
//			NULL, ".ogg", ".wav", ".mp3", ".it", ".xm", ".s3m", ".mod", 	/* �t�� */
		};
		int aaa;
		aaa = 8;
		do
		{
			aaa--;
			if (0 > aaa)
			{
				#if 0
				//ps pDebugScreenPrintf("Couldn't load: %s\n", name);
				//sc eKernelDelayThread(5000000);	// �m�F�p�҂�����
				#endif
				//use_audio = 0;/*�g�p�s�\*/
				return;
			}
			strcpy(name, DIRECTRY_NAME_DATA_STR "/sounds/");
			strcat(name, music_file_name[next_play_num]);
			strcat(name, ((char *)&name_extention[aaa][0]) );
			music_track = Mix_LoadMUS(name);
		}
		while (NULL == music_track);
	}
	aaa_Mix_PlayMusic(music_track, -1);
}


/*---------------------------------------------------------
	���ʉ��̃��N�G�X�g
	���ʉ���\��B�g���b�N�͓K���Ɍ��߂�B
---------------------------------------------------------*/
#if (0==USE_DESIGN_TRACK)
global void play_voice_auto_track(int req)
{
	if (0==use_audio)	{	return; 	}
	#if (0==USE_VSYNC_SOUND)
	//�`�����l����8�܂ł����p�ӂ���Ă��Ȃ��̂�8�ȍ~�̓`�����l�����R
	if (req>=8) 	{	sdl_mixer_play_channel(-1,	voice_track[req]);	}
	else			{	sdl_mixer_play_channel(req, voice_track[req]);	}
	#else
	if (req<8)/*  0-7 �܂ł̓g���b�N�w�� */
	{
			if (NOT_USE_TRACK != request_voice[req])/*�g�p��?*/
			{
				if (req==request_voice[req])/* ���ɗ\�񂳂�Ă��� */
				{
					return;
				}
				goto search_request;	/* �󂫌��� */
			}
			else
			{
				request_voice[req] = req;/* ���ʉ� ���� �\�� */
			}
	}
	else
	{
		/* �󂫌��� */
		search_request:
			;
		unsigned int i;
		for (i=0; i<MAX_VOICE_TRACK; i++)
		{
			if (NOT_USE_TRACK != request_voice[i])/*�g�p��?*/
			{
				if (req==request_voice[i])/* ���ɗ\�񂳂�Ă��� */
				{
					return;
				}
			}
			else
			{
				request_voice[i] = req;/* ���ʉ� ���� �\�� */
			}
		}
	}
	#endif
}
#endif

/*---------------------------------------------------------
	V-SYNC�^�C�~���O�ŁA�����ł���p��
	�g���b�N�w��Ō��ʉ���\�񂵂Ă���
---------------------------------------------------------*/

#if (1==USE_DESIGN_TRACK)
//global void voice_play()
//int AUDIO_18_voice_number=req;
//int AUDIO_19_voice_truck=play_track;
global CPU_FUNC(cpu_voice_play)
{
	if (0==use_audio)	{return;}
	/* �͈̓`�F�b�N */
//
	/* ���ʉ� �\�� */
	request_voice[AUDIO_19_voice_truck] = AUDIO_18_voice_number;
}
global void voice_play_menu(int request_index_number, int play_track)
{
	AUDIO_18_voice_number	= request_index_number;
	AUDIO_19_voice_truck	= play_track;
	cpu_voice_play();
}
#endif

/*---------------------------------------------------------
	4-7 �g���b�N�����Ԃɔ���
---------------------------------------------------------*/

#if (1==USE_DESIGN_TRACK)
// ���� I/F
global CPU_FUNC(cpu_bullet_play_05_auto)
{
	if (0==use_audio)	{return;}
	/* �͈̓`�F�b�N */
//
	/* ���ʉ� �\�� */
	static u8 aaa_play_track;
	aaa_play_track++;
	aaa_play_track &= 0x03;
	aaa_play_track |= 0x04;
	request_voice[aaa_play_track] = AUDIO_18_voice_number;
}
// ���� I/F
//global void bullet_play_04_set(int request_index_number)
//{
//	AUDIO_17_voice_default	= request_index_number;
//}
// ���� I/F
global CPU_FUNC(cpu_bullet_play_15_auto)
{
	AUDIO_18_voice_number	= VOICE15_BOSS_KOUGEKI_01;//AUDIO_17_voice_default;
	cpu_bullet_play_05_auto();
}
// �O�� I/F
global void bullet_play_04_auto(int request_index_number)
{
	AUDIO_18_voice_number	= request_index_number;
	cpu_bullet_play_05_auto();
}
#endif

/*
	�O���C�Y�����p�̓��ꏈ��:
	�O���C�Y���m���ɂȂ�悤�Ɂu�O���C�Y��p�g���b�N�v��p�ӂ������A
	�p�ӂ��������ł́A�O���C�Y�͊m���ɖ�Ȃ��B
	//
	�O���C�Y��p�g���b�N�� 1 track �m�ۂ����B
	���� 1 track ���Ƃ����ƁA�O���C�Y�����d�Ȃ�ƕςȉ��ɂȂ��ăO���C�Y���ɕ������Ȃ��B
	����������ŁA���������� 1 track �ȊO���肦�Ȃ������B
	//
	�O���C�Y��p�g���b�N�� 1 track �����Ȃ��̂ŁA�u�ē��\�����v����ʂɍ��K�v������B
	����́A�u�O���C�Y�����l�Ԃ������ƔF���ł���Œᒷ���v��ێ����Ă���B
	//
	�Ⴆ�Ή��Ɂu�O���C�Y���v�̉��́u�����v�i�Ȃ��Ă��鎞�ԁj���P�b�Ƃ���B
	���̎� 0.5 �b�O���C�Y����炵�āA������x�n�߂���O���C�Y����炷�B
	���̏ꍇ 2 ��O���C�Y���������ƕ�������B
	�i�A���A�f�[�^�[�̍����������ƃO���C�Y���� 1�� �ɂ����������Ȃ��j
	//
	0.5 �b �ƌ����̂� 30 �t���[���B���̎��Ԃ������ƒZ�����Ă݂�B
	�Œ�ł� 4 - 5 [frame] �ɐݒ肵�Ȃ��Ɖ����܂Ƃ��ɖ�Ȃ��B
	�i�A���A�f�[�^�[�̍�������ł��̕ӂ͐F�X�ς��B�j
*/
//#define SAITEI_GRAZE_WAV_NAGASA	(5)
#define SAITEI_GRAZE_WAV_NAGASA 	(4)

/*---------------------------------------------------------
	�O���C�Y���𔭉�
---------------------------------------------------------*/
static int graze_wav_sync_timer;/* �O���C�Y�����蒆����1 */
//static int graze_wav_pending;/* �O���C�Y�����蒆 �̏�� */

#if (1==USE_DESIGN_TRACK)
global void voice_play_graze(void)
{
	if (0==graze_wav_sync_timer)
	{
		graze_wav_sync_timer	= SAITEI_GRAZE_WAV_NAGASA;
	}
}
#endif



/*------------------------------------------------------
	V-SYNC�^�C�~���O�ŁA���炩���ߗ\�񂵂Ă��������ʉ���
	�܂Ƃ߂čĐ�����
---------------------------------------------------------*/

#if (1==USE_VSYNC_SOUND)
global void voice_play_vbl(void)
{
	if (0==use_audio)	{return;}
	/* ----- �ݒ�Ō��ʉ��Đ�OFF�Ȃ�Đ����Ȃ� */
	if (0==option_config[OPTION_CONFIG_03_SOUND])
	{
		return;
	}
	#if (1==USE_VSYNC_SOUND)
		{
			if (0 < graze_wav_sync_timer)/* �O��̃O���C�Y�����蒆���H */
			{
				graze_wav_sync_timer--;/* �O���C�Y�����蒆����1��i�߂�B */
				if ((SAITEI_GRAZE_WAV_NAGASA-1)==graze_wav_sync_timer)/*(����̂ݔ���)*/
				//
			//	if (0==graze_wav_sync_timer)
			//	{	/*(�O���C�Y�ē��������I���̏ꍇ)*/
			//		graze_wav_pending = (0);/*(�O���C�Y�ē���������)*/
			//	}
			//	else
			//	if ((SAITEI_GRAZE_WAV_NAGASA-1)==graze_wav_sync_timer)
			//	{	/*(�O���C�Y�ē��������n�߂̏ꍇ)*/
			//		if (0==graze_wav_pending)/*(SAITEI_GRAZE_WAV_NAGASA ���Ԍo�߂��Ȃ��ƍē��o���Ȃ�)*/
					{
			//			graze_wav_pending = (1);/*(�O���C�Y�ē������֎~)*/
						/*(�O���C�Y��������)*/
						AUDIO_18_voice_number	= VOICE09_GRAZE;
						AUDIO_19_voice_truck	= TRACK07_GRAZE_AUDIO_DRIVER_INTERNAL;
						cpu_voice_play();
					}
			//	}
			}
		}
		#if 1
	/*
		�d�X���̔��e���Ƃ��A
		��� 0.025[sec] �ʂŃ��[�v���Ă�݂���
		0.025[sec]==40frame??
		//
		1frame	(0.0166[sec]==60[frame]�Ń��[�v�����Ă��A�{���Ƃ������ꂽ�r�[���݂����ȕςȉ��ɂȂ�B
		���ۂ͂т���т��Ⴕ���J�݂����ȉ�)
		0.05[sec]�}�V���K���݂����ȉ�(���߂���H)
		6[frame]	0.10[sec]�}�V���K���݂����ȉ�(�x������I)

		1[frame]	0.0166[sec]�r�[���݂����ȕςȉ�(���߂���I)
		2[frame]	0.0333[sec]
		3[frame]	0.0500[sec]
		4[frame]	0.0666[sec]�}�V���K���݂����ȉ�(���ꂮ�炢�H)
		5[frame]	0.0833[sec]�}�V���K���݂����ȉ�(�����x��)
		6[frame]	0.1000[sec]�}�V���K���݂����ȉ�(�x������I)
		//
		���ɖ{���Ɠ��������g�����Ƃ��Ă��A�������ʉ����o�����߂ɂ́A
		���ʉ��̔������@����������Ȃ��ƁA�������͖�Ȃ��B
		�{���̌��ʉ��̔������@�́A�����ς������Ȃ��B
 */
	{
		unsigned int i;
		for (i=0; i<MAX_VOICE_TRACK; i++)
		{
			if (NOT_USE_TRACK != request_voice[i])/*�g�p��?*/
			{
				/* ���ʉ� ���� */
				sdl_mixer_play_channel(i/*track*/, voice_track[request_voice[i/*track*/]]);
				request_voice[i] = NOT_USE_TRACK;/*���g�p*/
			}
		}
	}
		#else
	/*
	�g�����̏ꍇ�A����܂葬����Ȃ��悤�ɁA
	�R���g���[�����Ă�悤�ŁA����σV�X�e������Ȃ���
	�ʂ�(��̂���Ȋ�����)���䂵�Ă�̂���
 */
	{
		static int i=0;
		i++;
		i &= 7;
	//	for (i=0; i<MAX_VOICE_TRACK; i++)
		{
			if (NOT_USE_TRACK != request_voice[i])/*�g�p��?*/
			{
				/* ���ʉ� ���� */
				sdl_mixer_play_channel(i/*track*/, voice_track[request_voice[i/*track*/]]);
				request_voice[i] = NOT_USE_TRACK;/*���g�p*/
			}
		}
	}
		#endif
	#endif
}
#endif

/*---------------------------------------------------------
	���ʉ��̃~�L�V���O���ʂ�ς���
---------------------------------------------------------*/
#if 0
global void set_voice_volume(int volume)
{
	#ifdef ENABLE_PSP
	#else
	int i;
	for (i=0; i<VOICE18_MAX_FILES; i++)
	{
		Mix_VolumeChunk(voice_track[i], volume);
	}
	#endif
}
#endif

/*---------------------------------------------------------
	�Ȃ̃~�L�V���O���ʂ�ς���
---------------------------------------------------------*/

global void set_music_volume(int volume)
{
	Mix_VolumeMusic(volume);
}


/*---------------------------------------------------------
	�I�[�f�B�I�V�X�e���̏I������
---------------------------------------------------------*/

global void exit_audio(void)
{
	if (0==use_audio)	{return;}
	/* �Ȃ̉�� */
//	if (Mix_PlayingMusic() )
	{Mix_HaltMusic();}
	Mix_FreeMusic(music_track);
	music_track = NULL;
	/* ���ʉ��̉�� */
	{
		unsigned int i;
		for (i=0; i<VOICE18_MAX_FILES; i++)
		{
			if (voice_track[i])
			{
				Mix_FreeChunk(voice_track[i]);
			}
		}
	}
	/* �~�L�T�[(�I�[�f�B�I�V�X�e��)�̉�� */
	Mix_CloseAudio();
}


/*---------------------------------------------------------
	���ʉ��̏������A�ǂݍ��ݏ���
---------------------------------------------------------*/

static void voice_load(void)
{
	static const char *voice_file_name[VOICE18_MAX_FILES] =
	{
		"se_a.wav", 	/*	0 */	//	"shot.wav", 	/*	0 */	"�V���b�g",
		"se_b.wav", 	/*	1 */	//	"hit.wav",		/*	1 */	"���j���[����",
		"se_c.wav", 	/*	2 */	//	"foedst.wav",	/*	2 */	"���j���[�I��",
		"se_d.wav", 	/*	3 */	//	"bossdst.wav",	/*	3 */	"�{�X�ޏ�",
	//
		"se_e.wav", 	/*	4 */	//	"shipdst.wav",	/*	4 */	"�s�`���[��",
		"se_f.wav", 	/*	5 */	//	"bonus.wav",	/*	5 */	"�A�C�e���擾",
		"se_g.wav", 	/*	6 */	//	"extend.wav",	/*	6 */	"������",
		"se_h.wav", 	/*	7 */	//	"bomb.wav", 	/*	7 */	"�����J�[�h",
	//
		"se_i.wav", 	/*	8 */	//	"death.wav",	/*	8 */	"�U�R�|����",
		"se_j.wav", 	/*	9 */	//	"graze.wav",	/*	9 */	"�O���C�Y",
		"se_p.wav", 	/* 10 */	//	"hit.wav",		/* 15 */	"���Ԑ���",
		"se_l.wav", 	/* 11 */	//	"e_shot00.wav", /* 11 */	"�L��������",
	//
		"se_k.wav", 	/* 12 */	//	"e_shot.wav",	/* 10 */	"�{�X�V���b�g1",
		"se_m.wav", 	/* 13 */	//	"b2_shot.wav",	/* 12 */	"�{�X�V���b�g2",
		"se_n.wav", 	/* 14 */	//	"_shot.wav",	/* 13 */	"�{�X�V���b�g3",
		"se_o.wav", 	/* 15 */	//	"e_shot01.wav", /* 14 */	"�{�X�V���b�g4",
		"se_q.wav", 	/* 16 */	//	"qupiin.wav"	/* 16 */	"�{�X�V���b�g5",
	//
		"se_r.wav", 	/* 17 */	//	"wiin.wav"		/* 17 */	"�{�X����1",
	};					/* 18 */	// MAX
	unsigned int i;
	char name[64];/*52*/
	for (i=0; i<VOICE18_MAX_FILES; i++)
	{
		strcpy(name, DIRECTRY_NAME_DATA_STR "/sounds/");
		strcat(name, voice_file_name[i]);
	//	voice_track[i] = Mix_LoadWAV(name);
		voice_track[i] = Mix_LoadWAV_RW(SDL_RWFromFile(name, "rb"), 1);
		if (NULL == voice_track[i])
		{
			//pspDebugScreenPrintf("Couldn't load: %s\n", name);
			use_audio = 0;/*�g�p�s�\*/
//			return;
		}
	}
}


/*---------------------------------------------------------
	�I�[�f�B�I�V�X�e���̏���������
---------------------------------------------------------*/

global void init_audio(void)
{
	//use_audio = 0;/*�g�p�s�\*/
	#if (1)
	#if (1==USE_VSYNC_SOUND)
	/* ������(���͖����Ă����Q�͂Ȃ�) */
	{
		unsigned int i;
		for (i=0; i<MAX_VOICE_TRACK; i++)
		{
			request_voice[i] = NOT_USE_TRACK;/* ���ʉ� ���� �\�� */
		}
	}
	graze_wav_sync_timer	= (0);/* ������(���͖����Ă����Q�͂Ȃ�) */
//	graze_wav_pending		= (0);/* ������(���͖����Ă����Q�͂Ȃ�) */
	#endif
	#endif
//
	const int audio_rate		= 44100;	// 22050
	const u16 audio_format		= AUDIO_S16;
	const int audio_channels	= 2;
	const int audio_buffers 	= 1024; 	//512
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0)
	{
		//error(ERR_FATAL, "Couldn't open audio.\n");
		return;
	}
	//else
	//{
		/*	�T�E���h�J�[�h�ɁA�v���X�y�b�N���m�F���A
			�T�E���h�J�[�h�ɋ@�\�������ꍇ�̓X�y�b�N���̂��C������B
			(psp�ł͕K���v���X�y�b�N������̂ŕK�v�Ȃ�) */
		//Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	//}
	use_audio = 1;/*�g�p�\*/
	voice_load();
}
