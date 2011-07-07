
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"game_main.h" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _BGM_VOICE_H_
#define _BGM_VOICE_H_

enum
{
	BGM_00_stop = 0,	/* ----- BGM ���t��~�R�}���h */
	BGM_01_stage1,		/* 1�ʓ��� */
	BGM_02_stage2,		/* 2�ʓ��� */
	BGM_03_stage3,		/* 3�ʓ��� */
	BGM_04_stage4,		/* 4�ʓ��� */
	BGM_05_stage5,		/* 5�ʓ��� */
	BGM_06_stage6,		/* 6�ʓ��� */
	BGM_07_stage7,		/* �G���f�B���O(����) */
	BGM_08_stage8,		/* �G�L�X�g���X�e�[�W���� */
	BGM_09_stage9,		/* �t�@���^�Y���X�e�[�W���� */
	BGM_10_menu04,		/* �G���f�B���O(��C�g����) */
	BGM_11_boss01,		/* 1�ʃ{�X */
	BGM_12_boss02,		/* 2�ʃ{�X */
	BGM_13_boss03,		/* 3�ʃ{�X */
	BGM_14_boss04,		/* 4�ʃ{�X */
	BGM_15_boss05,		/* 5�ʃ{�X */
	BGM_16_boss06,		/* 6�ʃ{�X */
	BGM_17_boss07,		/* �G�L�X�g���X�e�[�W1�{�X */
	BGM_18_boss08,		/* �G�L�X�g���X�e�[�W2�{�X */
	BGM_19_boss09,		/* �G�L�X�g���X�e�[�W3�{�X */
	BGM_20_boss10,		/* �B���{�X */
	BGM_21_boss00,		/* �B���{�X */
	BGM_21_menu03,		/* �G���f�B���O(�V�i���I) */
	BGM_22_menu02,		/* �L�[�R���t�B�O */
	BGM_23_menu01,		/* �^�C�g����� */
/* ----- �Ȃ̐�(�ǂݍ��݃t�@�C����) */
//#define USE_MUSIC_FILES 15/*14*/ /**/
	USE_MUSIC_FILES
};

enum
{
	VOICE00_SHOT = 0,			//	"�V���b�g", 		/* a				�V���b�g��							[���肿�肿��] */
	VOICE01_MENU_OK,			//	"���j���[����", 	/*[b]	[s(?)]		��									[�т傢�[] */
	VOICE02_MENU_SELECT,		//	"���j���[�I��", 	/* c				�T�R���ɉ� + �e������ + ���j���[	[����] */
	VOICE03_BOSS_HAKAI, 		//	"�{�X�ޏ�", 		/*[d];	[s(?)]		�{�X���ɉ�? 						[�ڂ��[�[��] */
	VOICE04_SHIP_HAKAI, 		//	"�s�`���[��",		/* e :				���@�j��							[�҂���[��] */
	VOICE05_BONUS,				//	"�A�C�e���擾", 	/* f				�A�C�e���擾��						[����] */
	VOICE06_EXTEND, 			//	"������",			/* g :				�c�@������							[�т������] */
	VOICE07_BOMB,				//	"�����X�y�J",		/*[h]	[s(?)]		��									[������邵�Ⴕ���] */
//
	VOICE08_DEATH,				//	"�U�R�|����",		/* i				�T�R���ɉ���p						[�ǂ��[�[�[] */
	VOICE09_GRAZE,				//	"�O���C�Y", 		/* j				�O���C�Y��							[��] */
	VOICE15_COUNT_TIMER,		//	"���Ԑ���", 		/* p :				���Ԑ����J�E���g��					[�ւɂ��] */
	VOICE11_BOSS_KIRARIN,		//	"�L��������",		/* l				������ */
//
	VOICE10_MAGICF_E_SHOT,		//	"�{�X�V���b�g1",	/* k				��									[�ӂ�] */
	VOICE12_SAKUYA_B2_SHOT, 	//	"�{�X�V���b�g2",	/* m				��� �� 							[�ς���] */
	VOICE13_SAKUYA_SHOT02,		//	"�{�X�V���b�g3",	/* n				��邶���V���b�g��				[����邿��邿���] */
	VOICE14_BOSS_KOUGEKI_01,	//	"�{�X�V���b�g4",	/*[o];	[s(?)]		�{�X�U����? 						[�΂���] */
	VOICE16_BOSS_KYUPIN,		//	"�{�X�V���b�g5",	/* l				�L���s�[���� */
//
	VOICE17_BOSS_TAME01,		//	"�{�X����1",		/* r				�����[�� */
	VOICE16_MAX_FILES,
};
	/* �{�Ƃɒቹ�n�̌��ʉ��͖�����������Ȃ��̂Ŕp�~�B ��� �� [�ǂǂǂǂ�] */
enum
{
	TRACK00_BULLETS = 0,
	TRACK01_EXPLODE,
	TRACK02_ALEART_IVENT,
	TRACK03_SHORT_MUSIC,
	TRACK04_TEKIDAN,
	TRACK05_ZAKO_DEATH,
	TRACK06_ALEART_IVENT_02,
	TRACK07_GRAZE
};

extern void init_audio(void);
extern void exit_audio(void);

extern void set_voice_volume(int volume);
extern void set_music_volume(int volume);

//extern void stop_music(void);/*�p�~*/
//#define stop_music(aaa)	play_music_num(BGM_00_stop)
extern void fadeout_music(void);
extern void pause_music(void);
extern void resume_music(void);

extern void play_music_num(int num);

extern void play_voice_auto_track(int req_idx);
extern void bullet_play_04_auto(int req_idx);

extern void voice_play_vbl(void);
extern void voice_play(int req, int play_track );

/* ���ʉ��̉����݌v���s��(�g���b�N����U��) */
#define USE_DESIGN_TRACK	(1)

#endif /* _BGM_VOICE_H_ */
