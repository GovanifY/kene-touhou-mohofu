
/*---------------------------------------------------------
	�I�[�f�B�I �}�l�[�W���[
---------------------------------------------------------*/

#include "game_main.h"

/*---------------------------------------------------------

---------------------------------------------------------*/

#if (1==USE_DESIGN_TRACK)
	/* ������ �������� �͏��Ȃ��Ȃ邪�A������ƌ��ʉ��݌v(�g���b�N�ݒ�)���Ȃ��Ƃ܂Ƃ��ɖ�Ȃ��B */
	#define USE_VSYNC_SOUND 	(1)
#else
	/* ��ʂ̃A�C�e�����W�����ꍇ�ɏ��������������B */
	#define USE_VSYNC_SOUND 	(0)
#endif

/* --- �Ȃ̊Ǘ��ő吔 (�ő�ǂݍ��݃t�@�C����) */
//#define MAX_MUSIC_FILES 8/*32*/

/* --- ���ʉ��̊Ǘ��ő吔 (�ő�ǂݍ��݃t�@�C����)MAX_VOICE_ALLOC_FILES=>VOICE16_MAX_FILES	*/
//#define MAX_VOICE_ALLOC_FILES 16/*64*/

/*---------------------------------------------------------

---------------------------------------------------------*/

/* ----- �Ȃ̐�(�ǂݍ��݃t�@�C����) */
#define USE_MUSIC_FILES 15/*14*/ /**/

/* ----- ���ʉ��̐�(�ǂݍ��݃t�@�C����) */
//#define VOICE16_MAX_FILES 15 /*�������ǉ�*/

/* ----- �Ȃ̃g���b�N */
static Mix_Music *music_track;

/* ----- ���ʉ��̃g���b�N */
static Mix_Chunk *voice_track[VOICE16_MAX_FILES];

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

//void pause_music(void)
//{
//	if ( 0==use_audio ) return;
//	Mix_PauseMusic();
//}

/*---------------------------------------------------------
	�ꎞ��~�����Ȃ̉��t�ĊJ
---------------------------------------------------------*/

//void resume_music(void)
//{
//	if ( 0==use_audio ) return;
//	Mix_ResumeMusic();
//}

/*---------------------------------------------------------
	�Ȃ̃t�F�[�h�A�E�g
---------------------------------------------------------*/

//void fadeout_music(void)
//{
//	if ( 0==use_audio ) return;
//	Mix_FadeOutMusic(1280);
//}

/*---------------------------------------------------------
	����܂ł̋Ȃ̉��t��~
---------------------------------------------------------*/

void stop_music(void)
{
//	if ( 0==use_audio ) return;
	if ( music_track != NULL )
	{
		//if ( Mix_PlayingMusic() )
		{	Mix_HaltMusic();	}
		Mix_FreeMusic(music_track);
		music_track = NULL;
	}
}

/*---------------------------------------------------------
	�Ȃ̉��t
---------------------------------------------------------*/

void play_music(int num)
{
	if ( 0==use_audio ) return;
	/* �͈̓`�F�b�N */
//
	static const char *music_file_name[USE_MUSIC_FILES] =
	{
		"menu1",	/*	0 */
		"stage1",	/*	1 */
		"stage2",	/*	2 */
		"stage3",	/*	3 */
		"stage4",	/*	4 */
		"stage5",	/*	5 */
		"stage6",	/*	6 */
		"stage7",	/*	7 */
		"menu2",	/*	8 */
		"boss1",	/*	9 */
		"boss2",	/* 10 */
		"boss3",	/* 11 */
		"boss4",	/* 12 */
		"boss5",	/* 13 */
		"boss6",	/* 14 */
	//	"boss7",	/* 15 */
	};		// ���낢��ǉ�
	/* ----- ����܂ł̉��t��~ */
	//stop_music();
	//void stop_music(void)
	{
	//	if ( 0==use_audio ) return;
		if ( music_track != NULL )
		{
			//if ( Mix_PlayingMusic() )
			{	Mix_HaltMusic();	}
			Mix_FreeMusic(music_track);
			music_track = NULL;
		}
	}
	/* ----- BGM ���t�J�n */
	char name[64/*72*/];
	{
		const char *name_extention[] =
		{
			NULL, ".mod", ".s3m", ".xm", ".it", ".mp3", ".wav", ".ogg",
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
		//	strcpy(name, data_dir);
			strcpy(name, DIRECTRY_NAME_DATA "/sounds/");
			strcat(name, music_file_name[num]);
			strcat(name, (char *)&name_extention[aaa][0] );
			music_track = Mix_LoadMUS(name);
		}
		while ( NULL == music_track );
	}
	Mix_PlayMusic(music_track, -1);
}

/*---------------------------------------------------------
	�I�[�f�B�I�V�X�e���̏I������
---------------------------------------------------------*/

void exit_audio(void)
{
	if ( 0==use_audio ) return;
	/* �Ȃ̉�� */
	//if ( Mix_PlayingMusic() )
	{	Mix_HaltMusic();	}
	Mix_FreeMusic(music_track);
	music_track = NULL;
	/* ���ʉ��̉�� */
	{int i;
		for (i=0; i<VOICE16_MAX_FILES; i++)
		{
			if ( voice_track[i] )
			{
				Mix_FreeChunk(voice_track[i]);
			}
		}
	}
	/* �~�L�T�[(�I�[�f�B�I�V�X�e��)�̉�� */
	Mix_CloseAudio();
}

/*---------------------------------------------------------
	���ʉ��̃��N�G�X�g
	���ʉ���\��B�g���b�N�͓K���Ɍ��߂�B
---------------------------------------------------------*/
#if (0==USE_DESIGN_TRACK)
void play_voice_auto_track(int req)
{
	if ( 0==use_audio ) return;
	#if (0==USE_VSYNC_SOUND)
	//�`�����l����8�܂ł����p�ӂ���Ă��Ȃ��̂�8�ȍ~�̓`�����l�����R
	if (req>=8) Mix_PlayChannel(-1, voice_track[req], 0);
	else Mix_PlayChannel(req, voice_track[req], 0);
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
		int i;
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
void voice_play(int req, int play_track )
{
	if ( 0==use_audio ) return;
	/* �͈̓`�F�b�N */
//
	/* ���ʉ� �\�� */
	request_voice[play_track] = req;
}
#endif

/*---------------------------------------------------------
	4-7 �g���b�N�����Ԃɔ���
---------------------------------------------------------*/

#if (1==USE_DESIGN_TRACK)
void bullet_play_04_auto(int req)
{
	if ( 0==use_audio ) return;
	/* �͈̓`�F�b�N */
//
	/* ���ʉ� �\�� */
	static u8 aaa_play_track;
	aaa_play_track++;
	aaa_play_track &= 0x03;
	aaa_play_track |= 0x04;
	request_voice[aaa_play_track] = req;
}
#endif

/*---------------------------------------------------------
	V-SYNC�^�C�~���O�ŁA���炩���ߗ\�񂵂Ă��������ʉ���
	�܂Ƃ߂čĐ�����
---------------------------------------------------------*/

#if (1==USE_VSYNC_SOUND)
void voice_play_vbl(void)
{
	#if (1==USE_VSYNC_SOUND)
	if ( 0==use_audio ) return;
		#if 0
	int i;
	for (i=0; i<MAX_VOICE_TRACK; i++)
	{
		if (NOT_USE_TRACK != request_voice[i])/*�g�p��?*/
		{
			/* ���ʉ� ���� */
			Mix_PlayChannel(i/*track*/, voice_track[request_voice[i/*track*/]], 0);
			request_voice[i] = NOT_USE_TRACK;/*���g�p*/
		}
	}
		#else
	{
		static int i=0;
		i++;
		i &= 7;
	//	for (i=0; i<MAX_VOICE_TRACK; i++)
		{
			if (NOT_USE_TRACK != request_voice[i])/*�g�p��?*/
			{
				/* ���ʉ� ���� */
				Mix_PlayChannel(i/*track*/, voice_track[request_voice[i/*track*/]], 0);
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

void set_voice_volume(int volume)
{
	#ifdef ENABLE_PSP
	#else
	int i;
	for (i=0; i<VOICE16_MAX_FILES; i++)
	{
		Mix_VolumeChunk(voice_track[i], volume);
	}
	#endif
}

/*---------------------------------------------------------
	�Ȃ̃~�L�V���O���ʂ�ς���
---------------------------------------------------------*/

void set_music_volume(int volume)
{
	Mix_VolumeMusic(volume);
}

/*---------------------------------------------------------
	���ʉ��̏������A�ǂݍ��ݏ���
---------------------------------------------------------*/

static void voice_load(void)
{
	static const char *voice_file_name[VOICE16_MAX_FILES] =
	{
		"se_a.wav", 	/*	0 */	//	"shot.wav", 	/*	0 */
		"se_b.wav", 	/*	1 */	//	"hit.wav",		/*	1 */
		"se_c.wav", 	/*	2 */	//	"foedst.wav",	/*	2 */
		"se_d.wav", 	/*	3 */	//	"bossdst.wav",	/*	3 */
		"se_e.wav", 	/*	4 */	//	"shipdst.wav",	/*	4 */
		"se_f.wav", 	/*	5 */	//	"bonus.wav",	/*	5 */
		"se_g.wav", 	/*	6 */	//	"extend.wav",	/*	6 */
		"se_h.wav", 	/*	7 */	//	"bomb.wav", 	/*	7 */
	//
		"se_i.wav", 	/*	8 */	//	"death.wav",	/*	8 */
		"se_j.wav", 	/*	9 */	//	"graze.wav",	/*	9 */
		"se_k.wav", 	/* 10 */	//	"e_shot.wav",	/* 10 */
		"se_l.wav", 	/* 11 */	//	"e_shot00.wav", /* 11 */
		"se_m.wav", 	/* 12 */	//	"b2_shot.wav",	/* 12 */
		"se_n.wav", 	/* 13 */	//	"_shot.wav",	/* 13 */
		"se_o.wav", 	/* 14 */	//	"e_shot01.wav",	/* 14 */
		"se_p.wav", 	/* 15 */	//	"hit.wav",		/* 15 */
	};		//���낢��ǉ�
	int i;
	char name[64/*52*/];
	for ( i=0; i<VOICE16_MAX_FILES; i++ )
	{
	//	strcpy(name, data_dir);
		strcpy(name, DIRECTRY_NAME_DATA "/sounds/");
		strcat(name, voice_file_name[i]);
		if ( NULL == (voice_track[i] = Mix_LoadWAV(name)) )
		{
			//ps pDebugScreenPrintf("Couldn't load: %s\n", name);
			use_audio = 0;/*�g�p�s�\*/
//			return;
		}
	}
}

/*---------------------------------------------------------
	�I�[�f�B�I�V�X�e���̏���������
---------------------------------------------------------*/

void init_audio(void)
{
	//use_audio = 0;/*�g�p�s�\*/
	#if 1
	#if (1==USE_VSYNC_SOUND)
	/* ������(���͖����Ă����Q�͂Ȃ�) */
	int i;
	for (i=0; i<MAX_VOICE_TRACK; i++)
	{
		request_voice[i] = NOT_USE_TRACK;/* ���ʉ� ���� �\�� */
	}
	#endif
	#endif
	if ( SDL_InitSubSystem(SDL_INIT_AUDIO) < 0 )
	{
		//ps pDebugScreenPrintf( "Unable to initialize SDL_AUDIO: %s\n", SDL_GetError());
		return;
	}

	const int audio_rate		= 44100;	// 22050
	const Uint16 audio_format	= AUDIO_S16;
	const int audio_channels	= 2;
	const int audio_buffers 	= 1024;
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0)
	{
		//ps pDebugScreenPrintf( "Couldn't open audio: %s\n", SDL_GetError());
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
